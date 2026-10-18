//////////////////////////////////////////////////////////////////////////////////////////
//  The file contains a source code demonstrating a sample how to implement a wrapper 
// for 2D solver. Such wrapper aimed to subordinate a 2D sketch to geometric constraints
// i.e. to make the sketch driven by constraints and dimensions. 
//  CAD system that uses the solver have to have such wrapper as a bridge connecting 
// CAD-specific geometry to Solver data through its API (header gce_api.h).
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
#include <last.h>

using namespace c3d;


//----------------------------------------------------------------------------------------
// Parametric node
/*
  This class represents a geometric and dimensional entities which are under control of the solver.
*/
//---
class ToyParametricNode: public MtRefItem
{  
public:  
  virtual void  Formulate( GCE_system ) = 0;      // This function formulates node's individual behavior in the solver
  virtual void  ApplySolution( GCE_system ) = 0;  // This function take a computed coordinates from the solver

protected:
  virtual ~ToyParametricNode() {}
};

//----------------------------------------------------------------------------------------
// Constraint node
//---
class ToyConstraintNode: public ToyParametricNode {};

//----------------------------------------------------------------------------------------
// This node represents some geometric object in constraint system
//---
class ToyGeomNode: public ToyParametricNode
{ 
protected:
           ToyGeomNode(): ToyParametricNode() {}
  virtual ~ToyGeomNode() {};

public:
  // Descriptor of geometrical object registered in the solver. 
  virtual geom_item GceItem() const = 0;
};

//----------------------------------------------------------------------------------------
// The node controlling an arc
/*
  This class takes control of object of type 'MbArc'. 'ArcNode' able to apply solver result
  to a circular arc. An object, which under control, can be replaced by any other type.
  This may be not only 'MbArc' but any native geometric type (presenting circular arc) of 
  client application.
*/
//---
class ToyArcNode: public ToyGeomNode
{
  SPtr<MbArc> arc;        // Controlled object, circle or arc
  geom_item   arcItem;    // Descriptor of the arc
  geom_item   circItem;   // Descriptor of the circle
  geom_item   endPnts[2]; // Pair of descriptors of the bounding points

public:
  ToyArcNode( MbArc & _arc ) 
    : ToyGeomNode()
    , arc( &_arc )
    , arcItem( GCE_NULL_G )
    , circItem( GCE_NULL_G )
  {
    endPnts[1] = endPnts[0] = GCE_NULL_G;        
  }

public:
  virtual geom_item GceItem() const { return arcItem; }
  virtual void      Formulate( GCE_system );
  // Apply a state of evaluated geometry from the solver to the driven arc
  virtual void      ApplySolution( GCE_system );

private:
  ToyArcNode( const ToyArcNode & );
  ToyArcNode & operator = ( const ToyArcNode & );  
};

//----------------------------------------------------------------------------------------
// Parametric node controlling a line segment
/*
  This class takes control of object of type 'MbLineSegment'. 'LineSegNode' able to apply 
  solver result to a line segment. An object, which under control, can be replaced by any 
  other type. This may be not only C3D-object 'MbLineSegment' but any native geometric 
  type (presenting line segment) of client application.
*/
//---
class ToyLineSegNode: public ToyGeomNode
{
  SPtr<MbLineSegment> lSeg;       // Controlled object, line segment
  geom_item           lSegItem;   // Descriptor of the line segment
  geom_item           endPnts[2]; // Pair of descriptors of the bounding points

public:
  ToyLineSegNode( MbLineSegment & _lSeg ) 
    : ToyGeomNode()
    , lSeg( &_lSeg ) 
    , lSegItem( GCE_NULL_G )    
  {
    endPnts[1] = endPnts[0] = GCE_NULL_G;    
  }

public:
  virtual geom_item GceItem() const { return lSegItem; }
  virtual void      Formulate( GCE_system );
  // Apply a state of evaluated geometry from the solver to the driven line segment
  virtual void      ApplySolution( GCE_system );

private:
  ToyLineSegNode( const ToyLineSegNode & );
  ToyLineSegNode & operator = ( const ToyLineSegNode & );  
};

//----------------------------------------------------------------------------------------
/*
  This subclass of GeomNode represents a control point of some geometric object. 
  For example, an instance of 'ControlPoint' can represent center of circle, 
  end of arc, end of lineseg and so on.
*/
//---
class ToyCPointNode: public ToyGeomNode
{
  geom_item cPnt; // Descriptor of a control point

public:
  ToyCPointNode( geom_item g ): ToyGeomNode(), cPnt(g) {}

public:
  virtual geom_item GceItem() const { return cPnt; }
  virtual void      Formulate( GCE_system /*gSys*/ ) {}
  // Apply a state of evaluated geometry from the solver to the model object
  virtual void      ApplySolution( GCE_system ) {}

private:
  ToyCPointNode( const ToyCPointNode & );
  ToyCPointNode & operator = ( const ToyCPointNode & );  
};

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

//----------------------------------------------------------------------------------------
//
//---
void ToyArcNode::Formulate( GCE_system gSys )
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
void ToyArcNode::ApplySolution( GCE_system gSys )
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

//----------------------------------------------------------------------------------------
//
//---
void ToyLineSegNode::Formulate( GCE_system gSys )
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
void ToyLineSegNode::ApplySolution( GCE_system gSys )
{   
  GCE_point p0 = GCE_GetPointXY( gSys, endPnts[0] );
  GCE_point p1 = GCE_GetPointXY( gSys, endPnts[1] );
  lSeg->Init( _CPnt(p0), _CPnt(p1) );
}

//----------------------------------------------------------------------------------------
// Constraint system of the sketch
//---
class ToyConstraintSystem
{  
  typedef std::vector<SPtr<ToyGeomNode>>         GeomsVector;
  typedef std::map<geom_item,SPtr<ToyGeomNode>>  ControlPoints;
  GCE_system    system;
  GeomsVector   gNodes;   // Set of geometric nodes which are under control of the solver
  ControlPoints cPoints;  // Associative array of control points

public:
   ToyConstraintSystem()
     : system( GCE_CreateSystem() )
     , gNodes()
     , cPoints()
   {
     GCE_SetJournal( system, "gce_sample.jrn" );
   }
  ~ToyConstraintSystem()
  {
    GCE_RemoveSystem( system );    
  }

public:  
  bool            IsEmpty() const { return gNodes.empty(); }
  ToyGeomNode  *  AddArc( MbArc & );
  ToyGeomNode  *  AddLineSegment( MbLineSegment & );
  ToyGeomNode  *  ControlPoint( ToyGeomNode *, point_type ); // Query a control point of the geometric node
  void            Remove( ToyParametricNode * ); // Not implemented

public: /* Functions formulating constraints */
  constraint_item Fix( ToyGeomNode * );  
  constraint_item ConnectSegments( ToyGeomNode *, ToyGeomNode * );
  constraint_item Concentric( ToyGeomNode *, ToyGeomNode * ); // Coincidence of circle centers
  constraint_item Tangent( ToyGeomNode *, ToyGeomNode * );    // Tangency of two curves
  constraint_item Length( ToyGeomNode * lSeg ); // Distance between the ends of line segment
  constraint_item Radius( ToyGeomNode * circ ); // Radial dimension for arc or circle
  constraint_item Perpendicular( ToyGeomNode *, ToyGeomNode * );  
  constraint_item UnaryConstraint( constraint_type, ToyGeomNode * );     // Horizontal / vertical
  constraint_item Distance( ToyGeomNode *, ToyGeomNode *, double dimVal );  // Distance dimension for a pair of geometric objects

public:
  GCE_result  Evaluate(); // Solve the sketch
  void        ChangeDimension( constraint_item, double dVal );  // Set a new value of dimension

private:
  geom_item  _GceItem( ToyGeomNode * );
  geom_item  _Register( SPtr<ToyGeomNode> ); // Registers a node in C3D Solver with its formulation
  void       _Unregister( ToyGeomNode * );   // Removes a node from C3D Solver and its constraint system (not implemented)
  
private:
  ToyConstraintSystem( const ToyConstraintSystem & );
  ToyConstraintSystem & operator = ( const ToyConstraintSystem & );
};

//----------------------------------------------------------------------------------------
//
//---
ToyGeomNode * ToyConstraintSystem::AddArc( MbArc & arc )
{
  SPtr<ToyGeomNode> arcNode( new ToyArcNode(arc) );
  _Register( arcNode );
  return arcNode;
}

//----------------------------------------------------------------------------------------
//
//---
ToyGeomNode * ToyConstraintSystem::AddLineSegment( MbLineSegment & lSeg )
{
  SPtr<ToyGeomNode> lSegNode( new ToyLineSegNode(lSeg) );
  _Register( lSegNode );
  return lSegNode;
}

//----------------------------------------------------------------------------------------
//
//---
ToyGeomNode * ToyConstraintSystem::ControlPoint( ToyGeomNode * gNode, point_type pType )
{
  const geom_item gItem = _GceItem( gNode );
  if ( gItem != GCE_NULL )
  {
    if ( GCE_GeomType(system,gItem) == GCE_POINT )
    {
      return gNode;
    }
    geom_item pntItem = GCE_PointOf( system, _GceItem(gNode), pType );
    SPtr<ToyGeomNode> & pNode = cPoints[pntItem];
    if ( pNode == nullptr )
    {
      pNode = new ToyCPointNode( pntItem );
    }
    return pNode;
  }
  return nullptr;
}

//----------------------------------------------------------------------------------------
// Fixing constraint
//---
constraint_item ToyConstraintSystem::Fix( ToyGeomNode * gNode )
{
  return GCE_FixGeom( system, _GceItem(gNode) );
}

//----------------------------------------------------------------------------------------
// Sets a radial dimension for arc or circle
//---
constraint_item ToyConstraintSystem::Radius( ToyGeomNode * circ )
{
  return GCE_FixRadius( system, _GceItem(circ) );
}

//----------------------------------------------------------------------------------------
// Connects two bounded curves at the its ends 
/*
  The function connects end of the first curve to begin of the second curve.
*/
//---
constraint_item ToyConstraintSystem::ConnectSegments( ToyGeomNode * bndCrv1, ToyGeomNode * bndCrv2 )
{
  geom_item ends[2] = { GCE_NULL_G, GCE_NULL_G };  // end points
  ends[0] = GCE_PointOf( system, _GceItem(bndCrv1), GCE_SECOND_END );
  ends[1] = GCE_PointOf( system, _GceItem(bndCrv2), GCE_FIRST_END );
  return GCE_AddCoincidence( system, ends );
}

//----------------------------------------------------------------------------------------
// Coincidence of circle centers
//---
constraint_item ToyConstraintSystem::Concentric( ToyGeomNode * circ1, ToyGeomNode * circ2 )
{
  geom_item centers[2] = { GCE_NULL_G, GCE_NULL_G };
  centers[0] = GCE_PointOf( system, _GceItem(circ1), GCE_CENTRE );
  centers[1] = GCE_PointOf( system, _GceItem(circ2), GCE_CENTRE );
  return GCE_AddCoincidence( system, centers );
}

//----------------------------------------------------------------------------------------
// Tangency of two curves
//---
constraint_item ToyConstraintSystem::Tangent( ToyGeomNode * crv1, ToyGeomNode * crv2 )
{
  geom_item crvs[2] = { _GceItem(crv1), _GceItem(crv2) };
  var_item  tPars[2] = { GCE_NULL_V, GCE_NULL_V };  // it is only used for splines or parametric curves
  return GCE_AddTangent( system, crvs, tPars );
}

//----------------------------------------------------------------------------------------
// Fixes distance between the ends of line segment
//---
constraint_item ToyConstraintSystem::Length( ToyGeomNode * lSeg )
{
  return GCE_FixLength( system, _GceItem(lSeg) );
}

//----------------------------------------------------------------------------------------
// 
//---
constraint_item ToyConstraintSystem::Perpendicular( ToyGeomNode * line1, ToyGeomNode * line2 )
{
  geom_item lines[2] = { _GceItem(line1), _GceItem(line2) };
  return GCE_AddPerpendicular( system, lines );
}

//----------------------------------------------------------------------------------------
// Horizontal / vertical
//---
constraint_item ToyConstraintSystem::UnaryConstraint( constraint_type cType, ToyGeomNode * gNode )
{
  return GCE_AddUnaryConstraint( system, cType, _GceItem(gNode) );
}

//----------------------------------------------------------------------------------------
// Distance between two geometric objects
//---
constraint_item ToyConstraintSystem::Distance( ToyGeomNode * gNode1, ToyGeomNode * gNode2, double dimVal )
{
  GCE_ldim_pars pars;
  pars.dPars.dimValue = dimVal;
  geom_item g[2] = { _GceItem(gNode1), _GceItem(gNode2) };
  return GCE_AddDistance( system, g, pars );
}

//----------------------------------------------------------------------------------------
// Solve the sketch
//---
GCE_result ToyConstraintSystem::Evaluate()
{
  GCE_result resCode = GCE_Evaluate( system );
  if ( resCode == GCE_RESULT_Ok )
  {
    /*
      Apply evaluation results to the sketch geometry
    */
    GeomsVector::iterator iter, last;
    for ( iter = gNodes.begin(), last = gNodes.end(); iter!=last; ++iter )
    {
      (*iter)->ApplySolution( system );
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
void ToyConstraintSystem::ChangeDimension( constraint_item dim, double dVal )
{
  GCE_ChangeDrivingDimension( system, dim, dVal );
}

//----------------------------------------------------------------------------------------
//
//---
geom_item ToyConstraintSystem::_GceItem( ToyGeomNode * gNode )
{
  return ( nullptr!=gNode ) ? gNode->GceItem() : GCE_NULL_G;
}

//----------------------------------------------------------------------------------------
//
//---
geom_item ToyConstraintSystem::_Register( SPtr<ToyGeomNode> gNode )
{
  gNode->Formulate( system ); // Formulate parametric behavior of the object
  if ( gNode->GceItem() != GCE_NULL )
  {
    gNodes.push_back( gNode );
  }  
  return gNode->GceItem();  
}

//----------------------------------------------------------------------------------------
// The sample of parametric sketch
/*
  This sample demonstrates how to change parametric sketch using C3D Solver. 
  The sketch is driven by three parameters: 
    A - hight of the sketch (tangential linear dimension );
    B - width of the sketch;
    C - another parameter, hight of the right part of the sketch.

  The first call of this function creates a sketch represented by a class-instance of
 'MbPlaneInstance', and other calls allow to change the parameters observing
  behavior of the parametric sketch.
*/
// ---
void SampleParametricSketch()
{
  typedef SPtr<MbLineSegment> LSegPtr;  // smart pointer type name
  /*
    The sample demonstrating how to change parametric sketch using C3D Solver
  */
  
  const MbCartPoint center( -150.0, 100.0 );  // A center of the circles
  
  // The values of dimensions of the sketch, A, B, C; 
  static double aVal = 150.0, bVal = 200.0, cVal = 100.0;
  static SPtr<MbPlaneInstance> sketch;
  static ToyConstraintSystem coSystem;
  static constraint_item  aDim, bDim, cDim, rDim;  

  if ( coSystem.IsEmpty() && (sketch == nullptr) )
  {
    /*
      First time - create the constraint system of sketch
    */
    GetDouble( _T("Input dimension 'A'"), aVal );
    GetDouble( _T("Dimension 'B'"), bVal );
    GetDouble( _T("And dimension 'C'"), cVal );

    MbPlacement3D xyPlane;  // a local coordinate system of the sketch
    SPtr<MbArc> circ( new MbArc(center,20.0) ); // The internal circle;
    sketch = new MbPlaneInstance( *circ, xyPlane );

    /*
      Create a geometry of the sketch
    */
    const MbCartPoint origin; // the origin of the sketch coordinate system 
    MbCartPoint arcBegin(-133.0, 147.0), arcMid(-176.0, 142.0), arcEnd(-200.0, 100.0);
    SPtr<MbArc> arc( MbArc::Create(arcBegin, arcMid, arcEnd) );    
    MbCartPoint vertexAB(-200, 0), vertexC(0, 100);
    
    LSegPtr segA( new MbLineSegment(arcEnd,vertexAB) ); // Use smart pointer to avoid memory leaks
    LSegPtr segB( new MbLineSegment(vertexAB,origin) );
    LSegPtr segC( new MbLineSegment(origin,vertexC) );
    LSegPtr segD( new MbLineSegment(vertexC,arcBegin) );
    /*
      Add the geometry to the sketch
    */
    sketch->AddPlaneItem( arc );
    sketch->AddPlaneItem( segA );
    sketch->AddPlaneItem( segB );    
    sketch->AddPlaneItem( segC );
    sketch->AddPlaneItem( segD ); // This lineseg closes the contour 

    /*
      Add the geoms to the constraint system
    */
    ToyGeomNode * circNode = coSystem.AddArc( *circ );
    ToyGeomNode * arcNode  = coSystem.AddArc( *arc );
    ToyGeomNode * aSeg     = coSystem.AddLineSegment( *segA );
    ToyGeomNode * bSeg     = coSystem.AddLineSegment( *segB );
    ToyGeomNode * cSeg     = coSystem.AddLineSegment( *segC );
    ToyGeomNode * dSeg     = coSystem.AddLineSegment( *segD );

    /*
      Add geometric constraints to the sketch
    */
    coSystem.ConnectSegments( arcNode, aSeg );
    coSystem.Tangent( arcNode, aSeg );  // it provides smooth connection of the arc and seg.A (fillet)
    coSystem.ConnectSegments( aSeg, bSeg );    
    coSystem.ConnectSegments( bSeg, cSeg );
    coSystem.ConnectSegments( cSeg, dSeg );
    coSystem.ConnectSegments( dSeg, arcNode );    
    coSystem.Tangent( arcNode, dSeg );  // it provides smooth connection of the arc and seg.D (fillet)
    coSystem.UnaryConstraint( GCE_VERTICAL, aSeg );
    coSystem.UnaryConstraint( GCE_HORIZONTAL, bSeg );
    coSystem.Perpendicular( bSeg, cSeg );
    coSystem.Fix( coSystem.ControlPoint(cSeg, GCE_FIRST_END) ); // fix one of corner vertices    
    coSystem.Concentric( arcNode, circNode );
    coSystem.Evaluate();

    /*
      Add driving dimensions A, B, C, R
    */
    aDim = coSystem.Distance( bSeg, arcNode, aVal );
    bDim = coSystem.Length( bSeg );
    cDim = coSystem.Length( cSeg );
    rDim = coSystem.Radius( arcNode );

    GCE_result resCode = coSystem.Evaluate();  // Satisfy all of constraints of the sketch
    if ( resCode != GCE_RESULT_Ok )
    {
      ::MessageBoxEx( _T("The sketch constraints is not fulfiled. Please, check your parameter values."), _T("Parametric sketch sample") );
    }

    TestVariables::viewManager->AddItem( *sketch, Style() );
  }
  else
  {
    GetDouble( _T("Change dimension 'A'"), aVal );
    GetDouble( _T("Dimension 'B'"), bVal );
    GetDouble( _T("And dimension 'C'"), cVal );
    /*
      Recalculate the sketch with new parameters A, B, C
    */
    coSystem.ChangeDimension( aDim, aVal );
    coSystem.ChangeDimension( bDim, bVal );
    coSystem.ChangeDimension( cDim, cVal );  
    GCE_result resCode = coSystem.Evaluate();
    if ( resCode != GCE_RESULT_Ok )
    {      
      ::MessageBoxEx( _T("The sketch is not solved. Please, check your parameter values."), _T("Parametric sketch sample") );
    }
    TestVariables::viewManager->RedrawObject( sketch );
  }  
}

#ifdef C3D_DEBUG

//----------------------------------------------------------------------------------------
// Sample code for C3D Manual. Creation of a circular arc
//---
void GCE_ExampleCircularArc()
{ 
  GCE_system gSys = GCE_CreateSystem();
  GCE_circle circPars;    // Circle values
  GCE_point endP1, endP2; // Coordinates of the end points.
  // … …
  // It is  required to insert the code that assigns start values of the circle and its endpoints.
  // … …
  geom_item p[2] = { GCE_AddPoint(gSys, endP1), GCE_AddPoint(gSys, endP2) };
  geom_item circItem = GCE_AddCircle( gSys, circPars );
  geom_item arc = GCE_AddBoundedCurve( gSys, circItem, p );
  // … …
  GCE_RemoveGeom( gSys, arc );
  GCE_RemoveSystem( gSys );
}

//----------------------------------------------------------------------------------------
// Example code of journalling function for C3D Manual
//---
void GCE_ExampleJournal()
{
  // Create an empty constraint system.  
  GCE_system gSys = GCE_CreateSystem();  
  // Switch on journalling for the constraint system.
  GCE_SetJournal( gSys, "C:\\Logs\\gce_sample.jrn" );
  // Make some calls of C3D Solver
  GCE_point p1Val, p2Val;
  p1Val.x = p1Val.y = 1.0;
  p2Val.x = p2Val.y = 2.0;
  geom_item pnt[2] = { GCE_AddPoint(gSys, p1Val), GCE_AddPoint(gSys, p2Val) };
  GCE_AddCoincidence( gSys, pnt );
  // ...
  // ...
  GCE_Evaluate( gSys );
  // Finalize the constraint system
  GCE_RemoveSystem( gSys );  
}

#endif // C3D_DEBUG
// eof
