#include <test_samples.h>
#include <test_manager.h>
#include <action_solid.h>
#include <assembly.h>
#include <solid.h>
#include <instance_item.h>
//
#include <generic_utility.h>
#include <gcm_reposition.h>
#include <gcm_manager.h>
//
#include <ctime>
#include <last.h>

using namespace c3d;

//------------------------------------------------------------------------------
// Get vertices of the regular pentagon in plane Z=0.
// ---
template< class PointsArray >
static void GetPentagonVertices( const double radius, PointsArray & vert )
{
  C3D_ASSERT( vert.size() >= 5 );
  MbCartPoint3D pnt( radius, 0.0, 0.0/*Z=0*/ );
  for ( size_t idx = 0; idx < 5; ++idx )
  {
    vert[idx] = pnt;
    pnt.Rotate( MbAxis3D::zAxis, M_PI2 / 5.0 );
  }
}

//----------------------------------------------------------------------------------------
// Return a solid of regular pentagonal prism.
// ---
static SPtr<MbSolid> PentagonalPrism( double radius )
{
  SolidSPtr solidPtr;
  MbSNameMaker names( 1, MbSNameMaker::i_SideNone, 0 );
  SpacePointsVector points(6);
  GetPentagonVertices( radius, points );
  points.back() = points.front() + MbVector3D::zAxis * radius / 16.0;
  MbElementarySolidResults results;
  ::ElementarySolid( MbElementarySolidParams( et_Prism, points, names ), results ); // ElementarySolid( points, et_Prism, names, prism );

  return results.GetResultSolid();
}

/*
  To avoid conflicts of the names declare an unique namespace of the sample.
*/
namespace ddh_sample
{
  //-----------------------------------------------------------------------------
  /*
    It adapts MbItem interface to solver's abstraction ItGeom.
  */
  //---
  struct GeomNode : public MtRefItem
                  , public ItGeom
  {
    SPtr<MbItem>  item; // An element of the assembly.
    GeomNode( MbItem* _item ) : MtRefItem(), ItGeom(), item( _item ) {}

    GCM_g_type    GeomType() const { return GCM_LCS; } // The solver interprets it as a LCS of a solid.
    const TCHAR* GetName() const { static const TCHAR nullStr = _T( '\0' ); return &nullStr; };
    void          GetPlacement( MbPlacement3D& pl ) const { if (item) item->GetPlacement( pl ); }

    /*
      Reference counting support.
    */
    refcount_t  AddRef() const { return MtRefItem::AddRef(); }
    refcount_t  Release() const { return MtRefItem::Release(); }
    OBVIOUS_PRIVATE_COPY( GeomNode );
  };

  //-----------------------------------------------------------------------------
  // Geometric object that has constant LCS in 'standart position'.
  /*
    Ground node has constant identifier GCM_GROUND.
  */
  //---
  struct GroundNode : public MtRefItem
                    , public ItGeom
  {
    GroundNode()
      : MtRefItem()
      , ItGeom() {}

    GCM_g_type    GeomType() const { return GCM_LCS; } // The solver interprets it as a LCS of a solid.
    const TCHAR * GetName() const { static const TCHAR gnd[] = _T( "gnd" ); return gnd; };
    void          GetPlacement( MbPlacement3D& pl ) const { pl = MbPlacement3D::global; }

    /*
      Reference counting support.
    */
    refcount_t  AddRef() const { return MtRefItem::AddRef(); }
    refcount_t  Release() const { return MtRefItem::Release(); }
    OBVIOUS_PRIVATE_COPY( GroundNode );
  };

  //-----------------------------------------------------------------------------
  /*
    Geom's set contains enumerated list of solids of the assembly.
  */
  //---
  class GeomsSet : public MtRefItem
                 , public ItPositionManager
  {
  public:
    GeomsSet()
      : MtRefItem()
      , gnd( new GroundNode )
    {}

  public:
    size_t          Count() const { return m_geoms.size(); }
    const ItGeom * operator[]( size_t geomIdx ) const
    {
      return m_geoms[geomIdx].get();
    }
  
    // Add to the set an element of the model.
    ItGeom *  AddItem( MbItem * item )
    {
      if ( nullptr != item )
      {
        SPtr<GeomNode> gNode( new GeomNode(item) );
        m_geoms.push_back( gNode );
        gNode.get();
      }
      return nullptr;
    }
    /*
      'geomIdx' is a number of a solid of the set.
      'subGeom' is a geometric object specified in local coordinates of the geomIdx-th solid.
    */
    template<class _SubGeom>
    MtArgument  SubGeom( size_t geomIdx, const _SubGeom & subGeom ) const
    {
      if ( geomIdx < m_geoms.size() )
      {
        return MtArgument( m_geoms[geomIdx], subGeom );
      }
      return MtArgument();
    }

  public:
    // Set new position of the object.
    void            Reposition( ItGeom&, const MbPlacement3D& );
    // The assembly coordinate system.
    ItGeom &        GetGround() const { return *gnd; }
    // Get the link character for pair of mating solids (direct connection) 
    GCM_dependency  GetJointStatus( const ItGeom&, const ItGeom& ) const { return GCM_NO_DEPENDENCY; }

    /*
      Garbage collection based on reference count.
    */
    refcount_t AddRef() const { return MtRefItem::AddRef(); }
    refcount_t Release() const { return MtRefItem::Release(); }

  private:
    std::vector<SPtr<GeomNode>> m_geoms;
    SPtr<GroundNode>            gnd;

    OBVIOUS_PRIVATE_COPY( GeomsSet );
  };

  //-----------------------------------------------------------------------------
  // Set new position of the instance.
  //---
  void GeomsSet::Reposition( ItGeom& geom, const MbPlacement3D& pos )
  {
    if ( geom.GeomType() == GCM_LCS )
    {
      if ( GeomNode* gNode = dynamic_cast<GeomNode*>(&geom) )
      {
        if ( gNode->item )
        {
          gNode->item->SetPlacement( pos );
        }
      }
    }
  }

  //-----------------------------------------------------------------------------
  /*
    The solver that takes control of assembly geoms.
  */
  //---
  class AssemblySolver
  {
  public:
    AssemblySolver()
      : geomsSet( new GeomsSet )
      , solver()
    {
      solver = GCM_CreateSolver( geomsSet );
      //GCM_SetJournal( solver->System(), "d:\\dodecahedron.jrn" );
    }
    ~AssemblySolver()
    {
      solver = nullptr;
      geomsSet = nullptr;
    }

  public:
    // List of geometric objects the solver works with.
    const GeomsSet & Geoms() const { return *geomsSet; }
    // Add a model item to the set of solver.
    void  AddItem( MbItem * gItem ) { geomsSet->AddItem( gItem ); }
    // Fix geomIdx-th geometric item.
    void  FixGeom( size_t geomIdx );

    // Get an geometric object (point, line, plane etc...) specified in the solid.
    /*
      The returned value is used as an argument of constraint.
    */
    template<class _SubGeom>
    MtArgument  SubGeom( size_t geomIdx, const _SubGeom& subGeom ) const;
    // Add a coincidence constraint for a pair of objects.
    void Coincident( MtArgument a1, MtArgument a2, GCM_alignment orient = GCM_CLOSEST )
    {
      solver->AddConstraint( GCM_COINCIDENT, a1, a2, orient );
    }

    // Declare an angular dimension
    GCM_constraint  Angle( MtArgument a1, MtArgument a2, MtArgument axis, double alpha );
    // Remove a constraint from the system.
    void            RemoveConstraint( GCM_constraint );
    // Change driving dimension and resolve the scene.
    // Поменять управляющий параметр и перерешать сцену.
    MtResultCode3D  ChangeDrivingDimension( GCM_constraint dim, double dVal );
    // Reevaluate the geometric constraint system.
    MtResultCode3D  Evaluate() { return solver->Evaluate(); }
    // Perform some final operations
    void            Finalize();

  private:
    SPtr<GeomsSet>      geomsSet;
    SPtr<MtGeomSolver>  solver;

    OBVIOUS_PRIVATE_COPY( AssemblySolver );
  };
    
  //-----------------------------------------------------------------------------
  // Fix geomIdx-th geometric item.
  //---
  void AssemblySolver::FixGeom( size_t geomIdx )
  {
    solver->FixGeom( (*geomsSet)[geomIdx] );
  }

  //-----------------------------------------------------------------------------
  // Get sub object - argument of constraint.
  //--
  template<class _SubGeom>
  MtArgument AssemblySolver::SubGeom( size_t geomIdx, const _SubGeom& subGeom ) const
  {
    return Geoms().SubGeom( geomIdx, subGeom );
  }

  //-----------------------------------------------------------------------------
  // Create an angular dimension
  //---
  GCM_constraint AssemblySolver::Angle( MtArgument a1, MtArgument a2, MtArgument axis, double alpha )
  {
    if ( const ItConstraintItem * dim = solver->AddConstraint(GCM_ANGLE, a1, a2, axis, alpha) )
    {
      return GCM_ConstraintId( solver->System(), dim );
    }
    return GCM_NULL;
  }

  //----------------------------------------------------------------------------------------
  //
  //---
  void AssemblySolver::RemoveConstraint( GCM_constraint cItem )
  {
    return GCM_RemoveConstraint( solver->System(), cItem );
  }

  //----------------------------------------------------------------------------------------
  // Change driving dimension and resolve the scene.
  // Поменять управляющий параметр и перерешать сцену.
  //---
  MtResultCode3D AssemblySolver::ChangeDrivingDimension( GCM_constraint dim, double dVal )
  {
    if (OK( GCM_ChangeDrivingDimension( solver->System(), dim, dVal ) ))
    {
      return solver->Evaluate();
    }
    return GCM_RESULT_None;
  }
  //----------------------------------------------------------------------------------------  
  // Perform some final operations
  //---
  void AssemblySolver::Finalize()
  {
    // Call serially GCM_Placement to journalize the final state of geometry.
    for ( size_t gIdx = 0, nGeoms = Geoms().Count(); gIdx<nGeoms; ++gIdx ) 
    {
      const GCM_geom gId = GCM_GeomId( solver->System(), (*geomsSet)[gIdx] );
      GCM_Placement( solver->System(), gId );
    }
    solver->Flush();
  }
}

//----------------------------------------------------------------------------------------
// Demonstrating sample: assembling dodecahedron
/*
  This sample demonstrates how to assembly a regular dodecahedron using C3D Solver.
  To illustrate the step-by-step scenario, time delays are used.
  'AssemblySolver' is a simple example how to integrate the solver into the application.
*/
// ---
void SampleDodecahedron()
{
  const double radius = 10.0;

  /*
    Pentagon vertices specified in the local coordinate system.
  */
  static_array<MbCartPoint3D, 5> vert( MbCartPoint3D::origin );
  GetPentagonVertices( radius, vert );

  /*
    First of all create a set of pentagonal faces.
  */
  SPtr<MbAssembly> assm( new MbAssembly );
  
  TestVariables::viewManager->DeleteModel();


  SPtr<MbSolid> pentaPrism( PentagonalPrism( radius ) ); // Re-used solid of pentagonal plate.

  MbPlacement3D lcs;
  ddh_sample::AssemblySolver solver;

  for ( int prismIdx = 0; prismIdx < 12; ++prismIdx )
  {
    lcs.Move( MbVector3D::xAxis * radius * 2.0 ).Rotate( MbAxis3D::zAxis, M_PI2 / 12.0 );
    solver.AddItem( assm->AddInstance( *pentaPrism, lcs ) );
  }

  TestVariables::viewManager->AddObject( TestVariables::ELEMENTARY_Style, assm );
  TestVariables::viewManager->ShowModel();

  /*
    Assembly dodecahedron using the solver.
  */  

  const double delta = CLOCKS_PER_SEC * 0.3;

  // Union the plates in triples.
  for ( size_t gIdx = 0, nGeoms = solver.Geoms().Count(); gIdx < nGeoms; gIdx += 3 )
  {
    if ( MbItem * item = assm->SetItem(gIdx) )
    {
      item->SetColor( RGB(50,90,100) );
    }
    solver.Coincident( solver.SubGeom( gIdx, vert[2] ), solver.SubGeom( gIdx + 1, vert[1] ) );
    solver.Coincident( solver.SubGeom( gIdx, vert[3] ), solver.SubGeom( gIdx + 1, vert[0] ) );
    solver.Coincident( solver.SubGeom( gIdx + 1, vert[0] ), solver.SubGeom( gIdx + 2, vert[1] ) );
    solver.Coincident( solver.SubGeom( gIdx + 1, vert[4] ), solver.SubGeom( gIdx + 2, vert[2] ) );

    const clock_t t = std::clock();
    solver.Evaluate();
    TestVariables::viewManager->RefreshModel();
    TestVariables::viewManager->ShowModel();
    while ( std::clock() - t < delta ); // delay
  }  

  for ( size_t idx = 0, nGeoms = solver.Geoms().Count(); idx < nGeoms; idx += 3 )
  {
    solver.Coincident( solver.SubGeom( idx, vert[4] ), solver.SubGeom( idx + 2, vert[0] ) );

    const clock_t t = std::clock();
    solver.Evaluate();
    TestVariables::viewManager->RefreshModel();
    TestVariables::viewManager->ShowModel();
    while ( std::clock() - t < delta ); // delay
  }

  // Connect the triplets each other.
  for ( size_t idx = 0, nGeoms = solver.Geoms().Count(); idx < nGeoms; idx += 6 )
  {
    solver.Coincident( solver.SubGeom( idx, vert[0] ), solver.SubGeom( idx + 4, vert[1] ) );
    solver.Coincident( solver.SubGeom( idx, vert[4] ), solver.SubGeom( idx + 4, vert[2] ) );
    const clock_t t = std::clock();
    solver.Evaluate();
    TestVariables::viewManager->RefreshModel();
    TestVariables::viewManager->ShowModel();
    while ( std::clock() - t < delta ); // demonstrating delay
  }

  MtGeomVariant axis;  // Auxiliary line - an rotation axis of the angular constraint.
  axis.SetAsLine( vert[0], vert[0]-vert[4] ); // line of edge 0-4  
  MtGeomVariant zPlane;
  zPlane.SetAsPlane( MbCartPoint3D::origin, MbVector3D::zAxis );

  /*
    To improve start position before connecting the plates declare 
    two angular dimensions rotating the geometry closer to the desired position.
    Note the evaluation result depends on the start position of the geometry.
  */

  // Temporary dimensions for rotating the pieces of dodecahedron.
  GCM_constraint a1 = solver.Angle( solver.SubGeom( 0, zPlane ), solver.SubGeom( 4, zPlane ), solver.SubGeom( 0, axis ), 0.0 );  
  GCM_constraint a2 = solver.Angle( solver.SubGeom( 6, zPlane ), solver.SubGeom( 10, zPlane ), solver.SubGeom( 6, axis ), 0.0 );  

  for ( double alpha = 0.1; alpha < 1.2; alpha += 0.3 )
  {
    const clock_t t = std::clock();
    solver.ChangeDrivingDimension( a1, alpha );
    solver.ChangeDrivingDimension( a2, alpha );
    TestVariables::viewManager->RefreshModel();
    TestVariables::viewManager->ShowModel();
    while ( std::clock() - t < delta ); // demonstrating delay
  }

  // Remove auxiliary constraints.
  solver.RemoveConstraint( a1 );
  solver.RemoveConstraint( a2 );

  /*
    Finally, close the dodecahedron connecting its faces. 
  */
  for ( size_t idx = 0, nGeoms = solver.Geoms().Count(); idx < nGeoms; idx += 6 )
  {
    solver.Coincident( solver.Geoms().SubGeom( idx+2, vert[4] ), solver.Geoms().SubGeom( idx + 4, vert[3] ) );
    const clock_t t = std::clock();
    solver.Evaluate();
    TestVariables::viewManager->RefreshModel();
    TestVariables::viewManager->ShowModel();
    while ( std::clock() - t < delta ); // demonstrating delay
  }

  solver.Coincident( solver.Geoms().SubGeom( 6, vert[2] ), solver.Geoms().SubGeom( 1, vert[2] ) );
  solver.Coincident( solver.Geoms().SubGeom( 6, vert[1] ), solver.Geoms().SubGeom( 1, vert[3] ) );
  solver.Coincident( solver.Geoms().SubGeom( 7, vert[2] ), solver.Geoms().SubGeom( 1, vert[1] ) );
  solver.Evaluate();
  solver.Finalize();
  
  TestVariables::viewManager->RefreshModel();
  TestVariables::viewManager->ShowModel();
}

// eof