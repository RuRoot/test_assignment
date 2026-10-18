//////////////////////////////////////////////////////////////////////////////////////////
//
//    The file contains a source code demonstrating a sample how to implement a wrapper
// for 2D solver. Such wrapper aimed to subordinate a 2D sketch to geometric constraints
// i.e. to make the sketch driven by constraints and dimensions. 
//    CAD system that uses the solver have to have such wrapper as a bridge connecting 
// CAD-specific geometry to Solver data through its API (header gce_api.h). 
//    Also in this sample implemented read/write facilities, that able to save a parametric 
// sketch with constraints to app specific document. To implement this it is used C3D-kernel 
// serializing functionality with inheritance from 'TapeBase' class.
//
//    WARNING: This code is only an example. The real CAD application can require more 
//             functionality and data types to implement real wrapper for the solver.
// 
//////////////////////////////////////////////////////////////////////////////////////////

#include <test_samples.h>
#include <test_variables.h>
#include <test_manager.h>
#include <test_service.h>
//
#include <cur_arc.h>
#include <cur_line_segment.h>
#include <plane_instance.h>
//
#include <gce_api.h>
#include <mt_ref_item.h>
//
#include <set>
#include <last.h>

#include <io_tape.h>

using namespace c3d;

// Unique ID of an application that writes the data to a file stream         
static const MbUuid SAMPLE_APP_ID = string_generator()( "{01234567-TEST-2016-PARAMETRICSKETCH}" );

//----------------------------------------------------------------------------------------
// Parametric node
/*
  This class represents a geometric and dimensional entities which are under control of the solver.
  To serialize parametric sketch its nodes of 'ParametricNode' should be inherited from 
  TapeBase class. 'TapeBase' gives the class the ability to save/restore its data.
*/
//---
class ParametricNode: public TapeBase
                    , public MtRefItem
{  
public:
  virtual void  Formulate( GCE_system ) = 0;     // This function formulates node's individual behavior in the solver
  virtual void  ApplySolution( GCE_system ) = 0; // This function take a computed coordinates from the solver

protected:
           ParametricNode(): TapeBase( registrable ), MtRefItem() {}
  virtual ~ParametricNode() {}  // It should be 'protected' to avoid destruction without 'AddRef/Release'.

DECLARE_PERSISTENT_CLASS( ParametricNode )
OBVIOUS_PRIVATE_COPY( ParametricNode )
};

IMP_A_PERSISTENT_CLASS( SAMPLE_APP_ID, ParametricNode );
IMPL_PERSISTENT_OPS( ParametricNode )

//----------------------------------------------------------------------------------------
// This node represents some geometric object in constraint system
//---
class GeomNode: public ParametricNode
{ 
protected:
           GeomNode(): ParametricNode() {}
  virtual ~GeomNode() {};

public:
  // Descriptor of geometrical object registered in the solver. 
  virtual geom_item GceItem() const = 0;

private:
  DECLARE_PERSISTENT_CLASS( GeomNode );
  OBVIOUS_PRIVATE_COPY( GeomNode );
};

IMP_A_PERSISTENT_CLASS( SAMPLE_APP_ID, GeomNode );
IMPL_PERSISTENT_OPS( GeomNode )

//----------------------------------------------------------------------------------------
// The node controlling an arc
/*
  This class takes control of object of type 'MbArc'. 'ArcNode' able to apply solver result
  to a circular arc. An object, which under control, can be replaced by any other type.
  This may be not only 'MbArc' but any native geometric type (presenting circular arc) of 
  client application.
*/
//---
class ArcNode: public GeomNode
{
  SPtr<MbArc> arc;        // Controlled object, circle or arc
  geom_item   arcItem;    // Descriptor of the arc
  geom_item   circItem;   // Descriptor of the circle
  geom_item   endPnts[2]; // Pair of descriptors of the bounding points

public:
  ArcNode( MbArc & _arc ) 
    : GeomNode()
    , arc( &_arc )
    , arcItem( GCE_NULL_G )
    , circItem( GCE_NULL_G )
  {
    endPnts[1] = endPnts[0] = GCE_NULL_G;        
  }

public:
  geom_item GceItem() const override { return arcItem; }
  void      Formulate( GCE_system ) override;
  // Apply a state of evaluated geometry from the solver to the driven arc
  void      ApplySolution( GCE_system ) override;

private:
  DECLARE_PERSISTENT_CLASS( ArcNode );
  OBVIOUS_PRIVATE_COPY( ArcNode );
};
IMPL_PERSISTENT_OPS( ArcNode )

//----------------------------------------------------------------------------------------
// Parametric node controlling a line segment
/*
  This class takes control of object of type 'MbLineSegment'. 'LineSegNode' able to apply 
  solver result to a line segment. An object, which under control, can be replaced by any 
  other type. This may be not only C3D-object 'MbLineSegment' but any native geometric 
  type (presenting line segment) of client application.
*/
//---
class LineSegNode: public GeomNode
{
  SPtr<MbLineSegment> lSeg;       // Controlled object, line segment
  geom_item           lSegItem;   // Descriptor of the line segment
  geom_item           endPnts[2]; // Pair of descriptors of the bounding points

public:
  LineSegNode( MbLineSegment & _lSeg ) 
    : GeomNode()
    , lSeg( &_lSeg ) 
    , lSegItem( GCE_NULL_G )    
  {
    endPnts[1] = endPnts[0] = GCE_NULL_G;    
  }

public:
  geom_item GceItem() const override { return lSegItem; }
  void      Formulate( GCE_system ) override;
  // Apply a state of evaluated geometry from the solver to the driven line segment
  void      ApplySolution( GCE_system ) override;

private:
  DECLARE_PERSISTENT_CLASS( LineSegNode );
  OBVIOUS_PRIVATE_COPY( LineSegNode );
};
IMPL_PERSISTENT_OPS( LineSegNode )

//----------------------------------------------------------------------------------------
/*
  This subclass of GeomNode represents a control point of some geometric object. 
  For example, an instance of 'ControlPoint' can represent center of circle, 
  bounding point of arc, end of lineseg and so on.    
*/
//---
class CPointNode: public GeomNode
{
  SPtr<const GeomNode>  m_owner;  // The object that owns the control point.
  point_type            m_pType;  // What's a control point. Center of circle, bounding point of curve ...
  geom_item             m_pItem;  // Descriptor of the control point.

public:
  CPointNode( const GeomNode * owner, point_type pType )
    : GeomNode()
    , m_owner( owner )
    , m_pType( pType )
    , m_pItem( GCE_NULL_G )
  {}

public:
  const GeomNode  * Owner() const { return m_owner; } // An owner to which the point belongs.
  point_type        WhatIsPoint() const { return m_pType; }
  geom_item GceItem() const override { return m_pItem; }
  void      Formulate( GCE_system gSys ) override
  {
    PRECONDITION( m_owner && m_pItem == GCE_NULL_G );
    m_pItem = GCE_PointOf( gSys, m_owner->GceItem(), m_pType );
  }
  // Apply a state of evaluated geometry from the solver to the model object
  void  ApplySolution( GCE_system ) override { /* the implemention is not requared */ }
  // Compare function for sorting a set of control points
  template<class _Pointer>
  bool Compare( const _Pointer & other ) const
  {
    if ( m_owner < other->m_owner )
      return true;
    return ( m_owner == other->m_owner ) ? m_pType < other->m_pType : false;
  }

private:
  DECLARE_PERSISTENT_CLASS( CPointNode );
  OBVIOUS_PRIVATE_COPY( CPointNode );
};
IMPL_PERSISTENT_OPS( CPointNode )

//----------------------------------------------------------------------------------------
// Convert to MbCartPoint type
//---
template<class XYType>
static MbCartPoint _CPnt ( const XYType & crd ) { return MbCartPoint(crd.x,crd.y); }

//----------------------------------------------------------------------------------------
// Convert to GCE_point type
//---
template<class XYType>
static GCE_point _GPnt ( const XYType & crd )
{ 
  GCE_point pnt;
  pnt.x = crd.x, pnt.y = crd.y; 
  return pnt;
}

IMP_PERSISTENT_CLASS( SAMPLE_APP_ID, CPointNode );

//----------------------------------------------------------------------------------------
// This constructor is needed to create an instance during reading from a stream
// ---
CPointNode::CPointNode( TapeInit )
  : GeomNode()
  , m_owner( nullptr )
  , m_pType( GCE_IMPROPER_POINT )
  , m_pItem( GCE_NULL_G )
{}

//----------------------------------------------------------------------------------------
// This static member-function is necessary to save this class to a data storage
//---
void CPointNode::Write( writer & out, const CPointNode * obj )
{
  PRECONDITION( obj->GetRegistrable() == registrable );
  out << obj->m_owner;
  out << (uint32)obj->m_pType; // encoded what type of the control point
}

//----------------------------------------------------------------------------------------
// This static member-function is necessary to restore this class from a data storage
//---
void CPointNode::Read( reader & in, CPointNode * obj )
{
  uint32 pCode = uint32(-1);
  in >> obj->m_owner;
  in >> pCode;
  obj->m_pType = (point_type)pCode;
  if ( (GCE_FIRST_PTYPE > obj->m_pType) || (obj->m_pType >= GCE_LAST_PTYPE) )
  {
    in.setState( io::badData );
  }
}

//----------------------------------------------------------------------------------------
//
//---
void ArcNode::Formulate( GCE_system gSys )
{ 
  /*
    Register the base circle of the arc
  */
  GCE_circle  circPars;
  MbCartPoint centre; 
  arc->GetCentre( centre );
  circPars.centre = _GPnt( centre );
  circPars.radius = arc->GetRadius();
  arcItem = circItem = GCE_AddCircle( gSys, circPars );

  /*
    Register the bounding points and the arc
  */
  if ( !arc->IsClosed() )
  {
    endPnts[0] = GCE_AddPoint( gSys, _GPnt(arc->GetLimitPoint(1)) );
    endPnts[1] = GCE_AddPoint( gSys, _GPnt(arc->GetLimitPoint(2)) );
    arcItem    = GCE_AddBoundedCurve( gSys, circItem, endPnts );
  }
}

//----------------------------------------------------------------------------------------
// Apply a state of evaluated geometry from the solver to the model object
//---
void ArcNode::ApplySolution( GCE_system gSys )
{  
  GCE_point center = GCE_GetPointXY( gSys, GCE_PointOf(gSys,circItem,GCE_CENTRE) );
  double    radius = GCE_GetCoordValue( gSys, circItem, GCE_RADIUS );
  if ( arc->IsClosed() && (endPnts[0] == endPnts[1]) && (endPnts[1] == GCE_NULL_G) )  // a case of circle
  {
    arc->Init( _CPnt(center), radius );
  }
  else // case of bounded circle, i.e. arc
  {
    GCE_point p0 = GCE_GetPointXY( gSys, endPnts[0] );
    GCE_point p1 = GCE_GetPointXY( gSys, endPnts[1] );
    arc->Init( _CPnt(center), radius, _CPnt(p0), _CPnt(p1), false/*clockwise*/ );
  }   
}

IMP_PERSISTENT_CLASS( SAMPLE_APP_ID, ArcNode );

//----------------------------------------------------------------------------------------
// This constructor is needed to create an instance during reading from a stream
// ---
ArcNode::ArcNode( TapeInit )
  : GeomNode()
  , arc( nullptr )
  , arcItem( GCE_NULL_G )
  , circItem( GCE_NULL_G )
{
  endPnts[1] = endPnts[0] = GCE_NULL_G;
}

//----------------------------------------------------------------------------------------
// This static member-function is necessary to save this class to a data storage
//---
void ArcNode::Write( writer & out, const ArcNode * node )
{
  PRECONDITION( node->GetRegistrable() == registrable );
  out << node->arc;
}

//----------------------------------------------------------------------------------------
// This static member-function is necessary to restore this class from a data storage
//---
void ArcNode::Read( reader & in, ArcNode * node )
{
  in >> node->arc;
}

//----------------------------------------------------------------------------------------
//
//---
void LineSegNode::Formulate( GCE_system gSys )
{   
  MbCartPoint bnd0 = lSeg->GetLimitPoint(1);
  MbCartPoint bnd1 = lSeg->GetLimitPoint(2);
  GCE_point p0, p1;
  p0.x = bnd0.x, p0.y = bnd0.y;
  p1.x = bnd1.x, p1.y = bnd1.y;
  endPnts[0] = GCE_AddPoint( gSys, p0 );
  endPnts[1] = GCE_AddPoint( gSys, p1 );
  lSegItem = GCE_AddLineSeg( gSys, endPnts );
}

//----------------------------------------------------------------------------------------
//
//---
void LineSegNode::ApplySolution( GCE_system gSys )
{   
  GCE_point p0 = GCE_GetPointXY( gSys, endPnts[0] );
  GCE_point p1 = GCE_GetPointXY( gSys, endPnts[1] );
  lSeg->Init( _CPnt(p0), _CPnt(p1) );
}

IMP_PERSISTENT_CLASS( SAMPLE_APP_ID, LineSegNode );

//----------------------------------------------------------------------------------------
// This constructor is needed to create an instance during reading from a stream
// ---
LineSegNode::LineSegNode( TapeInit )
  : GeomNode()
  , lSeg( nullptr )
  , lSegItem( GCE_NULL_G )    
{
  endPnts[1] = endPnts[0] = GCE_NULL_G;
}

//----------------------------------------------------------------------------------------
// This static member-function is necessary to save this class to a data storage
//---
void LineSegNode::Write( writer & out, const LineSegNode * node )
{
  PRECONDITION( node->GetRegistrable() == registrable );
  out << node->lSeg;
}

//----------------------------------------------------------------------------------------
// This static member-function is necessary to restore this class from a data storage
//---
void LineSegNode::Read( reader & in, LineSegNode * node )
{
  in >> node->lSeg;
}

//----------------------------------------------------------------------------------------
// Integer identifier of constraint type. These values are written to the file.
//---
enum ConstraintType
{
    CT_UNKNOWN  = 0

    /* Unary constraints */
  , CT_FIX      = 1
  , CT_HORIZONTAL
  , CT_VERTICAL
  , CT_LENGTH
  , CT_RADIUS
  
    /* Binary constraints */
  , CT_COINCIDENT
  , CT_CONCENTRIC
  , CT_PERPENDICULAR
  , CT_PARALLEL
  , CT_TANGENT

    /* Dimensional constraints */
  , CT_DISTANCE
  , CT_ANGLE
  , CT_LAST // The number of enum values
};

// Argument of geometric constraint
template<size_t _idx>
struct argument_num 
{ 
  static const size_t index = _idx; 
  argument_num() {} 
};

/*
  Names of arguments of geometric constraint
*/
const argument_num<0> FirstArg;
const argument_num<1> SecondArg;
const argument_num<2> ThirdArg;


//----------------------------------------------------------------------------------------
// Constraint node
//---
class ConstraintNode: public ParametricNode 
{
  constraint_item             m_cItem;  // Descriptor of the constraint formulating the node.
  ConstraintType              m_cType;  // Type of the geometric constraint. Its value is written to the file.  
  std::vector<SPtr<GeomNode>> m_args;   // Geometric arguments, i.e. geometric objects that are connected by the constraint.
  double                      m_dVal;   // Value of dimension (in case of dimensional constraint).

public:
  // Constructor for binary constraints
  ConstraintNode( ConstraintType cType, GeomNode * g1, GeomNode * g2, double dVal = GCE_UNDEFINED_DBL )
    : ParametricNode()
    , m_cItem( GCE_NULL_C )
    , m_cType( cType )
    , m_args( 2, SPtr<GeomNode>(nullptr) )
    , m_dVal( dVal )
  {
    m_args.front() = g1;
    m_args.back() = g2;
  }
  // Constructor for unary constraints
  ConstraintNode( ConstraintType cType, GeomNode * g )
    : ParametricNode()
    , m_cItem( GCE_NULL_C )
    , m_cType( cType )
    , m_args( 1, SPtr<GeomNode>(g) ) 
    , m_dVal( GCE_UNDEFINED_DBL )
  {}
  // Descriptor of geometrical object registered in the solver. 
  constraint_item GceItem() const { return m_cItem; }
  // The constraint has driving parameter
  bool    IsDrivingDimension() const;
  // Get a current value of dimension parameter (for dimensional only)
  double  DimParameter() const { return m_dVal; }
  // Formulate this node in the solver
  void    Formulate( GCE_system ) override;
  // Apply a state of evaluated geometry from the solver to the driven line segment
  void    ApplySolution( GCE_system ) override {}
  // Change value of dimension
  void    SetDimValue( double dVal ) { m_dVal = dVal; }

protected:
  // Returns an argument of constraint by the number from 1 up to 'arity'
  template<size_t argIdx>
  geom_item _GeomArg( const argument_num<argIdx> & )
  {
    if ( (argIdx < m_args.size()) && (m_args[argIdx] != nullptr) )
    {
      return m_args[argIdx]->GceItem();
    }
    return GCE_NULL_G;
  }

private:
  DECLARE_PERSISTENT_CLASS( ConstraintNode );
  OBVIOUS_PRIVATE_COPY( ConstraintNode );
};
IMPL_PERSISTENT_OPS( ConstraintNode )

//----------------------------------------------------------------------------------------
//
//---
ConstraintNode::ConstraintNode( TapeInit )
  : ParametricNode()
  , m_cItem( GCE_NULL_C )
  , m_cType( CT_UNKNOWN )
  , m_args()
  , m_dVal( 0.0 )
{}

//----------------------------------------------------------------------------------------
// The constraint has driving parameter
//---
bool ConstraintNode::IsDrivingDimension() const
{
  switch ( m_cType )
  {
    /*
      Constraints of these types can be driven by one parameter as dimensions.
    */
  case CT_RADIUS:
  case CT_LENGTH:
  case CT_DISTANCE:
  case CT_ANGLE:
    return true;

  default:
    return false;
  }
}

//----------------------------------------------------------------------------------------
// Formulate the constraint node in the solver
//---
void ConstraintNode::Formulate( GCE_system gSys )
{  
  PRECONDITION( m_cItem == GCE_NULL_C );
  geom_item gPair[2] = { _GeomArg(FirstArg), _GeomArg(SecondArg) };
  switch ( m_cType )
  {
    case CT_FIX:
    {
      m_cItem = GCE_FixGeom( gSys, _GeomArg(FirstArg) );
      break;
    }
    case CT_HORIZONTAL:
    {
      m_cItem = GCE_AddUnaryConstraint( gSys, GCE_HORIZONTAL, _GeomArg(FirstArg) );
      break;
    }
    case CT_VERTICAL:
    {
      m_cItem = GCE_AddUnaryConstraint( gSys, GCE_VERTICAL, _GeomArg(FirstArg) );
      break;
    }
    case CT_LENGTH:
    {
      m_cItem = GCE_FixLength( gSys, _GeomArg(FirstArg) );
      break;
    }
    case CT_RADIUS:
    {
      m_cItem = GCE_FixRadius( gSys, _GeomArg(FirstArg) );
      break;
    }
    case CT_COINCIDENT:
    {
      m_cItem = GCE_AddCoincidence( gSys, gPair );      
      break;
    }
    case CT_CONCENTRIC:
    {
      geom_item centers[2] = { GCE_NULL_G, GCE_NULL_G };
      centers[0] = GCE_PointOf( gSys, _GeomArg(FirstArg), GCE_CENTRE );
      centers[1] = GCE_PointOf( gSys, _GeomArg(SecondArg), GCE_CENTRE );
      m_cItem = GCE_AddCoincidence( gSys, centers );
      break;
    }
    case CT_PERPENDICULAR:
    {      
      m_cItem = GCE_AddPerpendicular( gSys, gPair );
      break;
    }
    case CT_PARALLEL:
    {
      m_cItem = GCE_AddParallel( gSys, gPair );
      break;
    }
    case CT_TANGENT:
    {
      var_item tPars[2] = { GCE_NULL_V, GCE_NULL_V };  // These variable descriptors are only needed for spline and ellipse curves.
      m_cItem = GCE_AddTangent( gSys, gPair, tPars );
      break;
    }    
    case CT_DISTANCE:
    {
      GCE_ldim_pars pars;
      pars.dPars.dimValue = m_dVal;
      m_cItem = GCE_AddDistance( gSys, gPair, pars );
      break;
    }

  default:
    break;
  }
  if ( m_dVal == GCE_UNDEFINED_DBL )
  {
    m_dVal = GCE_DimensionParameter( gSys, m_cItem );
  }
  PRECONDITION( m_cItem != GCE_NULL_C );
}

IMP_PERSISTENT_CLASS( SAMPLE_APP_ID, ConstraintNode );

//----------------------------------------------------------------------------------------
// Write to the file content of an array
//---
template<class _Cont> // _Cont is STL-like container
void _WriteArray( const _Cont & arr, writer & out ) 
{
  out << (uint32)arr.size(); // Write a number of elements
  typename _Cont::const_iterator iter, last;
  for ( iter = arr.begin(), last = arr.end(); out.good() && (iter!=last); ++iter )
  {
    out << *iter;
  }
}

//----------------------------------------------------------------------------------------
// Read content of an array from the file
//---
template<class _Cont> // _Cont is STL-like container
void _ReadArray( _Cont & arr, reader & in ) 
{
  uint32 _size = 0;
  in >> _size; // Read a number of elements
  typename _Cont::value_type val;
  for ( uint32 idx = 0; in.good() && (idx < _size); ++idx )
  {
    in >> val;
    arr.insert( arr.end(), val );
  }
  if ( !in.good() )
  {
    arr.clear();
  }
}

//----------------------------------------------------------------------------------------
// Compare function for sorting a set of control points.
/*
  Assertion: Two control points are equal, if they have same owners and types of control points.
*/
//---
struct _CpLess
{
  template<class _CPoint1,class _CPoint2>
  bool operator()( const _CPoint1 & cp1, const _CPoint2 & cp2 ) const
  {
    return cp1->Compare( cp2 );
  }
};

//----------------------------------------------------------------------------------------
// This static member-function is necessary to save this class to a data storage
//---
void ConstraintNode::Write( writer & out, const ConstraintNode * node )
{
  PRECONDITION( node->GetRegistrable() == registrable );
  out << (uint16) node->m_cType; // Write an id of the constraint type.  
  out << node->m_dVal; // Write a value of dimension.
  _WriteArray( node->m_args, out ); // Write a sequence of arguments.  
}

//----------------------------------------------------------------------------------------
// This static member-function is necessary to restore this class from a data storage
//---
void ConstraintNode::Read( reader & in, ConstraintNode * node )
{
  uint16 conId = CT_UNKNOWN;
  in >> conId;  
  in >> node->m_dVal;
  _ReadArray( node->m_args, in );
  node->m_cType = (ConstraintType)conId;
  if ( conId >= CT_LAST )
  {
    in.setState( io::badData );
  }
}

//----------------------------------------------------------------------------------------
// Constraint system of the sketch
//---
class ConstraintSystem
{  
  typedef std::vector<SPtr<GeomNode>> GeomsVector;
  typedef std::vector<SPtr<ConstraintNode>> Constraints;
  typedef std::set<SPtr<CPointNode>,_CpLess> ControlPoints;
  GCE_system    gcSystem; // Computational data of the solver.
  GeomsVector   gNodes;   // Set of geometric nodes which are under control of the solver.
  ControlPoints cPoints;  // Associative array of control points.
  Constraints   cNodes;   // Set of constraints consisting the system of the sketch.  

public:
  typedef Constraints::iterator c_node_iterator;  // Iterator of constraint nodes

public:
   ConstraintSystem()
     : gcSystem( GCE_CreateSystem() )
     , gNodes()
     , cPoints()
     , cNodes()
   {
     GCE_SetJournal( gcSystem, "gce_sample.jrn" );
   }
  ~ConstraintSystem()
  {
    GCE_RemoveSystem( gcSystem );    
  }

public:
  bool        IsEmpty() const { return gNodes.empty() && cNodes.empty() && cPoints.empty(); }
  GeomNode  * AddArc( MbArc & );
  GeomNode  * AddLineSegment( MbLineSegment & );
  GeomNode  * ControlPoint( GeomNode *, point_type ); // Query a control point of the geometric node
  template<class _CIter>
  void        GetConstraints( _CIter & cFirst, _CIter & cLast ) { cFirst = cNodes.begin(); cLast = cNodes.end(); }
  void        Remove( ParametricNode * ); // Not implemented

public: /* Functions formulating constraints */
  ConstraintNode * AddConstraint( ConstraintType, GeomNode * ); // Fixation, Horizontal, Vertical
  ConstraintNode * AddConstraint( ConstraintType, GeomNode *, GeomNode * ); // Add a binary constraint
  ConstraintNode * AddConstraint( SPtr<ConstraintNode> ); // Add a new constraint to the system
  ConstraintNode * ConnectSegments( GeomNode *, GeomNode * ); // Connects two bounded curves at its ends
  ConstraintNode * Distance( GeomNode *, GeomNode *, double dimVal );  // Distance dimension for a pair of geometric objects

public:
  GCE_result  Evaluate(); // Solve the sketch
  void        ChangeDimension( ConstraintNode *, double dVal );  // Set a new value of dimension

public: // Read/Write functions
  void  Write( writer & ) const;
  void  Read( reader & );

private:
  geom_item _GceItem( GeomNode * gNode) { return nullptr != gNode ? gNode->GceItem() : GCE_NULL_G; }
  geom_item _Register( SPtr<GeomNode> ); // Registers a node in C3D Solver with its formulation
  void      _Unregister( GeomNode * );   // Removes a node from C3D Solver and its constraint system (not implemented)
  void      _Reformulate(); // Reinitialize the constraint system
 
private:
  ConstraintSystem( const ConstraintSystem & );
  ConstraintSystem & operator = ( const ConstraintSystem & );
};

//----------------------------------------------------------------------------------------
//
//---
GeomNode * ConstraintSystem::AddArc( MbArc & arc )
{
  SPtr<GeomNode> arcNode( new ArcNode(arc) );
  _Register( arcNode );
  return arcNode;
}

//----------------------------------------------------------------------------------------
//
//---
GeomNode * ConstraintSystem::AddLineSegment( MbLineSegment & lSeg )
{
  SPtr<GeomNode> lSegNode( new LineSegNode(lSeg) );
  _Register( lSegNode );
  return lSegNode;
}

//----------------------------------------------------------------------------------------
// Query an existing or new created control point of the object 'gOwner'
/*
  gOwner - some geometric object whose control point is requested;
  pType - what kind of control point belonging to the owner (center, end point, etc..);
*/
//---
GeomNode * ConstraintSystem::ControlPoint( GeomNode * gOwner, point_type pType )
{
  const geom_item gItem = _GceItem( gOwner );
  if ( gItem != GCE_NULL )
  {
    if ( GCE_GeomType(gcSystem, gItem) == GCE_POINT )
    {
      return gOwner; // the owner is a point itself
    }
    
    SPtr<CPointNode> cPnt( new CPointNode(gOwner, pType) );
    std::pair<ControlPoints::iterator,bool> insRes = cPoints.insert( cPnt );
    if ( insRes.second )
    {
      cPnt->Formulate( gcSystem );
      return cPnt;
    }
    PRECONDITION( insRes.first != cPoints.end() );
    return *insRes.first;
  }
  return nullptr;
}

//----------------------------------------------------------------------------------------
// Solve the sketch
//---
GCE_result ConstraintSystem::Evaluate()
{
  GCE_result resCode = GCE_Evaluate( gcSystem );
  if ( resCode == GCE_RESULT_Ok )
  {
    /*
      Apply evaluation results to the sketch geometry
    */
    GeomsVector::iterator iter, last;
    for ( iter = gNodes.begin(), last = gNodes.end(); iter!=last; ++iter )
    {
      (*iter)->ApplySolution( gcSystem );
    }
  }
  return resCode;
}

//----------------------------------------------------------------------------------------
// Set a new value of dimension
/*
  NOTE: This function only sets the parameter without solving the constraint system. 
        Call the function Evaluate() to apply the changes.
*/
//---
void ConstraintSystem::ChangeDimension( ConstraintNode * dim, double dVal )
{
  if ( dim )
  {    
    if ( GCE_ChangeDrivingDimension(gcSystem, dim->GceItem(), dVal) == GCE_RESULT_Ok )
    {
      dim->SetDimValue( dVal );
    }    
  }  
}

//----------------------------------------------------------------------------------------
// Fixation, Horizontal, Vertical
//---
ConstraintNode * ConstraintSystem::AddConstraint( ConstraintType cType, GeomNode * g )
{
  SPtr<ConstraintNode> cPtr( new ConstraintNode(cType, g) );
  return AddConstraint( cPtr );
}

//----------------------------------------------------------------------------------------
// Add new binary constraint
//---
ConstraintNode * ConstraintSystem::AddConstraint( ConstraintType cType, GeomNode * g1, GeomNode * g2 )
{
  SPtr<ConstraintNode> cPtr( new ConstraintNode(cType, g1, g2) );
  return AddConstraint( cPtr );
}

//----------------------------------------------------------------------------------------
// Connects two bounded curves at its ends 
/*
  The function connects end of the first curve to begin of the second curve.
*/
//---
ConstraintNode * ConstraintSystem::ConnectSegments( GeomNode * bndCrv1, GeomNode * bndCrv2 )
{
  return AddConstraint( CT_COINCIDENT, ControlPoint(bndCrv1,GCE_SECOND_END), ControlPoint(bndCrv2,GCE_FIRST_END) );
}

//----------------------------------------------------------------------------------------
// Distance between two geometric objects
//---
ConstraintNode * ConstraintSystem::Distance( GeomNode * gNode1, GeomNode * gNode2, double dimVal )
{
  SPtr<ConstraintNode> cPtr( new ConstraintNode(CT_DISTANCE, gNode1, gNode2, dimVal) );
  return AddConstraint( cPtr );
}

//----------------------------------------------------------------------------------------
// Add a new constraint to the system
//---
ConstraintNode * ConstraintSystem::AddConstraint( SPtr<ConstraintNode> cPtr )
{
  PRECONDITION( cPtr->GceItem() == GCE_NULL_C );
  if ( cPtr->GceItem() == GCE_NULL_C )
  {
    cPtr->Formulate( gcSystem );
    if ( cPtr->GceItem() != GCE_NULL_C )
    {
      cNodes.push_back( cPtr );
      return cPtr;
    }
  }
  return nullptr;
}

//----------------------------------------------------------------------------------------
//
//---
geom_item ConstraintSystem::_Register( SPtr<GeomNode> gNode )
{
  gNode->Formulate( gcSystem ); // Formulate parametric entity of the object
  if ( gNode->GceItem() != GCE_NULL )
  {
    gNodes.push_back( gNode );
  }  
  return gNode->GceItem();  
}

//----------------------------------------------------------------------------------------
// Reinitialize the constraint system
/*
  This function fully reloads the computational constraint system.  Actually algorithm
  '_Reformulate' restores the computational data of the solver (GCE_system) with use 
  of sketch modeling data, which is stored in containers 'gNodes' and 'cNodes'.
*/
//---
void ConstraintSystem::_Reformulate()
{
  GCE_ClearSystem( gcSystem );
  /*
    Load to the solver all the geometric nodes.
  */
  GeomsVector::iterator gIter, gLast;
  for ( gIter = gNodes.begin(), gLast = gNodes.end(); gIter!=gLast; ++gIter )
  {
    (*gIter)->Formulate( gcSystem );
  }

  /*
    Restore descriptors of control points.
  */
  ControlPoints::iterator cpIter, cpLast;
  for ( cpIter = cPoints.begin(), cpLast = cPoints.end(); cpIter!=cpLast; ++cpIter )
  {
    (*cpIter)->Formulate( gcSystem );
  }

  /*
    Load to the solver all the constraint nodes.
  */
  Constraints::iterator cIter, cLast;
  for ( cIter = cNodes.begin(), cLast = cNodes.end(); cIter!=cLast; ++cIter )
  {
    (*cIter)->Formulate( gcSystem );
  }
}

//----------------------------------------------------------------------------------------
//
//---
void ConstraintSystem::Write( writer & out ) const
{  
  _WriteArray( gNodes, out );
  _WriteArray( cPoints, out ); 
  _WriteArray( cNodes, out );  
}

//----------------------------------------------------------------------------------------
//
//---
void ConstraintSystem::Read( reader & in )
{
  PRECONDITION( IsEmpty() ); // The system must be empty
  if ( IsEmpty() )
  {
    _ReadArray( gNodes, in );
    _ReadArray( cPoints, in );
    _ReadArray( cNodes, in );    
    _Reformulate(); // Restore the solver
  }
  else
    in.setState( io::fail );
}

// Constrained 2D sketch file extension
#define SMP_SKETCH_EXT       _T(".s2d\0")
#define SMP_SKETCH_EXT_INFO  _T("Sample sketch(*.S2D)\0*.s2d\0\0")

//----------------------------------------------------------------------------------------
// The function calls an 'save file' dialog and save a sketch constraint system to the specified file *.s2d
//---
void _SaveSketch( const SPtr<MbPlaneInstance> & sketch, const ConstraintSystem & gcSystem )
{
  if ( _tcslen( TestVariables::nameBuffer ) == 0 )
  {
    _tcscpy( TestVariables::nameBuffer, _T("sketch.s2d") ); // default name
  }
   
  if ( _GetSaveFileName( TestVariables::nameBuffer, WRITE_FILE_INFO, SMP_SKETCH_EXT_INFO, SMP_SKETCH_EXT) )
  {
    membuf buf; // Создаем membuf
    {
      writer::writer_ptr out = writer::CreateMemWriter( buf, 0 );
      if ( out->good() )
      {
        SetWaitCursor( true );
        *out << sketch;
        gcSystem.Write( *out );        
        buf.setMaxRegCount( out->GetMaxRegisteredCount() ); // The maximum number of registered facilities.
        SetWaitCursor( false );
      }
      
      if ( !out->good() )
      {
        MessageBoxEx( RT_FILE_NOT_WRITTEN, IDS_TITLE_ERR );
        return;
      }
    }
    writeiobuftodisk( TestVariables::nameBuffer, buf ); // Write to the disk.
  }
}

//----------------------------------------------------------------------------------------
// The function calls an 'open file' dialog and restore a sketch constraint system from the specified file *.s2d
//---
void _ReadSketch( SPtr<MbPlaneInstance> & sketch, ConstraintSystem & gcSystem )
{
  if ( _GetOpenFileName( TestVariables::nameBuffer, SELECT_FILE_INFO, SMP_SKETCH_EXT_INFO, SMP_SKETCH_EXT ) )
  {
    reader::reader_ptr in = reader::CreateReader( std::unique_ptr<iobuf_Seq>( &::createiobuf( TestVariables::nameBuffer ) ), io::in );

    if ( in->good() ) 
    {
      SetWaitCursor( true );
      *in >> sketch;
      gcSystem.Read( *in );
      SetWaitCursor( false );

      if ( !in->good() )
        MessageBoxEx( RT_ERROR_READING_PARTLY, IDS_TITLE_ERR );
    }
    else
      MessageBoxEx( RT_FILE_NOT_OPEN, IDS_TITLE_ERR );
  }
}

//----------------------------------------------------------------------------------------
// The sample how to write constraints of parametric sketch
/*
  This sample demonstrates how to save geometric constraints of the parametric sketch 
  using writing/reading facilities of C3D kernel.
  The sketch is driven by three parameters: 
    A - hight of the sketch (tangential linear dimension );
    B - width of the sketch;
    C - another parameter, hight of the right part of the sketch.

  The call of this function creates a sketch represented by a class-instance of
 'MbPlaneInstance', and writes its constraint system to the file *.s2d.
*/
// ---
void SampleWriteSketch()
{
  typedef SPtr<MbLineSegment> LSegPtr;  // smart pointer type name

  const MbCartPoint center( -150.0, 100.0 );  // A center of the circles
  
  // The values of dimensions of the sketch, A, B, C;
  SPtr<MbPlaneInstance> sketchInst;  // a geometric representation of the written sketch
  const double aVal = 150.0;
  ConstraintSystem coSystem;
  SPtr<ConstraintNode> aDim, bDim, cDim, rDim;  

  /*
    First time - create the constraint system of sketch
  */
  MbPlacement3D xyPlane;  // a local coordinate system of the sketch
  SPtr<MbArc> circ( new MbArc(center,20.0) ); // The internal circle;
  sketchInst = new MbPlaneInstance( *circ, xyPlane );

  /*
    Create a geometry of the sketch
  */
  const MbCartPoint origin; // the origin of the sketch coordinate system 
  MbCartPoint arcBegin(-133.0, 147.0), arcMid(-176.0, 142.0), arcEnd(-200.0, 100.0);
  SPtr<MbArc> arc( MbArc::Create( arcBegin, arcMid, arcEnd ) );
  MbCartPoint vertexAB(-200, 0), vertexC(0, 100);
    
  LSegPtr segA( new MbLineSegment(arcEnd,vertexAB) ); // Use smart pointer to avoid memory leaks
  LSegPtr segB( new MbLineSegment(vertexAB,origin) );
  LSegPtr segC( new MbLineSegment(origin,vertexC) );
  LSegPtr segD( new MbLineSegment(vertexC,arcBegin) );
  /*
    Add the geometry to the sketch
  */
  sketchInst->AddPlaneItem( arc );
  sketchInst->AddPlaneItem( segA );
  sketchInst->AddPlaneItem( segB );    
  sketchInst->AddPlaneItem( segC );
  sketchInst->AddPlaneItem( segD ); // This lineseg closes the contour 

  /*
    Add the geoms to the constraint system
  */
  GeomNode * circNode = coSystem.AddArc( *circ );
  GeomNode * arcNode  = coSystem.AddArc( *arc );
  GeomNode * aSeg     = coSystem.AddLineSegment( *segA );
  GeomNode * bSeg     = coSystem.AddLineSegment( *segB );
  GeomNode * cSeg     = coSystem.AddLineSegment( *segC );
  GeomNode * dSeg     = coSystem.AddLineSegment( *segD );

  /*
    Add geometric constraints to the sketch
  */
  coSystem.ConnectSegments( arcNode, aSeg );
  coSystem.AddConstraint( CT_TANGENT, arcNode, aSeg );  // it provides smooth connection of the arc and seg.A (fillet)
  coSystem.ConnectSegments( aSeg, bSeg );    
  coSystem.ConnectSegments( bSeg, cSeg );
  coSystem.ConnectSegments( cSeg, dSeg );
  coSystem.ConnectSegments( dSeg, arcNode );    
  coSystem.AddConstraint( CT_TANGENT, arcNode, dSeg );  // it provides smooth connection of the arc and seg.D (fillet)
  coSystem.AddConstraint( CT_VERTICAL, aSeg );
  coSystem.AddConstraint( CT_HORIZONTAL, bSeg );
  coSystem.AddConstraint( CT_PERPENDICULAR, bSeg, cSeg );
  coSystem.AddConstraint( CT_FIX, coSystem.ControlPoint(cSeg, GCE_FIRST_END) ); // fix one of corner vertices
  coSystem.AddConstraint( CT_CONCENTRIC, arcNode, circNode );
  coSystem.Evaluate();

  /*
    Add driving dimensions A, B, C, R
  */
  aDim = coSystem.Distance( bSeg, arcNode, aVal );
  bDim = coSystem.AddConstraint( CT_LENGTH, bSeg );
  cDim = coSystem.AddConstraint( CT_LENGTH, cSeg );
  rDim = coSystem.AddConstraint( CT_RADIUS, arcNode );

  GCE_result resCode = coSystem.Evaluate();  // Satisfy all of constraints of the sketch
  if ( resCode != GCE_RESULT_Ok )
  {
    ::MessageBoxEx( _T("The sketch constraints is not fulfiled. Please, check your parameter values."), _T("Parametric sketch sample") );
  }

  TestVariables::viewManager->AddItem( *sketchInst, Style() );
  _SaveSketch( sketchInst, coSystem );  
}

//----------------------------------------------------------------------------------------
// The sample how to read constraints of parametric sketch
/*
  This sample demonstrates how to restore geometric constraints of the parametric sketch 
  from a file using writing/reading facilities of C3D kernel. After the sketch is read
  all driving dimensions can be changed and resolved to be sure that all constraints
  is restored successfully and the constraint system works well.
*/
// ---
void SampleReadSketch()
{
  //typedef SPtr<MbLineSegment> LSegPtr;  // smart pointer type name

  // The values of dimensions of the sketch, A, B, C; 
  SPtr<MbPlaneInstance> sketchInst;  // geometric representation of the sketch restored from the file
  ConstraintSystem coSystem;

  _ReadSketch( sketchInst, coSystem );

  if ( sketchInst != nullptr )
  {
    /*      
      To test the constraint system after it read, change all the dimensions and recalculate geometry.
    */
    ConstraintSystem::c_node_iterator cIter, cLast;
    for ( coSystem.GetConstraints(cIter,cLast); cIter != cLast; ++cIter )
    {
      if ( (*cIter)->IsDrivingDimension() )
      {
        double dVal = (*cIter)->DimParameter();
        GetDouble( _T("Change dimension or remain it unchanged:"), dVal );
        coSystem.ChangeDimension( *cIter, dVal );
      }
    }

    GCE_result resCode = coSystem.Evaluate();  // Satisfy all of constraints of the sketch
    if ( resCode == GCE_RESULT_Ok )
    {
      TestVariables::viewManager->AddItem( *sketchInst, Style() );
      TestVariables::viewManager->RedrawObject( sketchInst );
    }
    else
    {
      MessageBoxEx( _T("The sketch constraints is not fulfiled. Please, check your parameter values."), _T("Parametric sketch sample") );
    }
  }  
}

// eof
