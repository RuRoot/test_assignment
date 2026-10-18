#include <test_samples.h>
//
#include <test_manager.h>
#include <action_solid.h>
#include <solid.h>
#include <instance_item.h>
#include <mb_class_traits.h>
#include <cdet_utility.h>
#include <last.h>

using namespace c3d;


#ifndef _UNUSED_VAR
#define _UNUSED_VAR( var ) (std::ignore = var)
#endif

#ifndef NULL_PTR
const void * NULL_PTR = 0; // Перенести в math_define.h ?
#endif

//----------------------------------------------------------------------------------------
// The func simulates a semantic environment of google unit testing
//---
template<class _Type1,class _Type2>
static void EXPECT_EQ( const _Type1 & t1, const _Type2 & t2 )
{
  _UNUSED_VAR(t1);
  _UNUSED_VAR(t2);
  PRECONDITION( t1 == t2 );
}

//----------------------------------------------------------------------------------------
// The func simulates a semantic environment of google unit testing
//---
template<class _Type1,class _Type2>
static void EXPECT_NE( const _Type1 & t1, const _Type2 & t2 )
{
  _UNUSED_VAR(t1);
  _UNUSED_VAR(t2);
  PRECONDITION( t1 != t2 );
}

//----------------------------------------------------------------------------------------
// Return an elementary solid by the shape type and characteristic points.
//---
static SPtr<MbSolid> _ElementarySolid(  const SArray<MbCartPoint3D> & points
                                      , ElementaryShellType solidType
                                      , const MbSNameMaker & names )
{  
  MbElementarySolidResults results;
  SpacePointsVector auxPoints( points.begin(), points.end() );
  ::ElementarySolid( MbElementarySolidParams( solidType, auxPoints, names ), results ); // ElementarySolid( points, solidType, names, solid );

  return results.GetResultSolid();
}

//----------------------------------------------------------------------------------------
// Function for creating an elementary body by 4 support points.
//---
template<ElementaryShellType elShellType>
SPtr<MbSolid> _CreateElementarySolid( const MbCartPoint3D & p1, const MbCartPoint3D & p2,
                                      const MbCartPoint3D & p3, const MbCartPoint3D & p4,
                                      const MbSNameMaker & solidName )
{
  return _ElementarySolid( SArray<MbCartPoint3D>{std::vector<MbCartPoint3D>{p1,p2,p3,p4}}, elShellType, solidName );
}

//----------------------------------------------------------------------------------------
// Function for creating elementary polygonal body by 4 support points.
//---
template<ElementaryShellType elShellType>
SPtr<MbMesh> _CreateElementaryMesh( const MbCartPoint3D & p1, const MbCartPoint3D & p2,
                                    const MbCartPoint3D & p3, const MbCartPoint3D & p4,
                                    const MbSNameMaker & solidName )
{
  SPtr<MbSolid> solid = _CreateElementarySolid<elShellType>( p1, p2, p3, p4, solidName );
  MbStepData stepData;
  stepData.SetStepType( ist_MetricStep );
  stepData.SetSag( 0. );
  return SPtr<MbMesh>{ static_cast<MbMesh *>(solid->CreateMesh(stepData, MbFormNote(false, true), nullptr)) };
}

//----------------------------------------------------------------------------------------
// Demonstrating sample: collision detection
/*
  This sample demonstrates how to apply collision detection utility to show
  interferencies of solids (the code is able to use in unit-tests).
*/
// ---
void SampleCollisionDetection()
{  
  /*
    Create a box, first solid.
  */
  MbSNameMaker names1( 1, MbSNameMaker::i_SideNone, 0 );
  SArray<MbCartPoint3D> points;
  points.push_back( MbCartPoint3D(0, 0, 0) );
  points.push_back( MbCartPoint3D(100, 0, 0) );
  points.push_back( MbCartPoint3D(0, 30, 0) );
  points.push_back( MbCartPoint3D(0, 0, 60) );
  SPtr<MbSolid> box1( _ElementarySolid(points, et_Block, names1) );

  /*
    Create a box, second solid.
  */
  MbSNameMaker names2( 2, MbSNameMaker::i_SideNone, 0 );
  points.clear();
  points.push_back( MbCartPoint3D(10, 20, 1) );
  points.push_back( MbCartPoint3D(100, 0, 0) );
  points.push_back( MbCartPoint3D(0, 30, 0) );
  points.push_back( MbCartPoint3D(0, 0, 60) );
  SPtr<MbSolid> box2( _ElementarySolid(points, et_Block, names2) );
  EXPECT_NE( box1, NULL_PTR );
  EXPECT_NE( box2, NULL_PTR );
  if ( box1 && box2 )
  {
    /*
      Both solids positioned as instancies having their own LCS (placement).
    */
    const MbPlacement3D lcs2( MbCartPoint3D(50, 15, 10) );
    SPtr<MbInstance> item1( new MbInstance(*box1, MbPlacement3D::global) );
    SPtr<MbInstance> item2( new MbInstance(*box2, lcs2) );

    TestVariables::viewManager->AddObject( TestVariables::ELEMENTARY_Style, item1 );
    TestVariables::viewManager->AddObject( TestVariables::ELEMENTARY_Style, item2 );    

    CdCollisionDetection cdet;

    cdet_item cdRep1 = cdet.AddItem( *box1, item1->GetPlacement(), (void*)item1.get() );
    cdet_item cdRep2 = cdet.AddItem( *box2, item2->GetPlacement(), (void*)item2.get() );
    EXPECT_NE( cdRep1, CDET_NULL );
    EXPECT_NE( cdRep2, CDET_NULL );
    
    /*
      Detect the collisions of the solids.
    */

    cdet_collided_faces colQuery;
    cdet_result res = cdet.CheckCollisions( colQuery );
    EXPECT_EQ( res, CDM_RESULT_CollisionDetected );

    /*
      The cycle colouring faces interfering with each other.
    */

    cdet_collided_faces::collided_faces::iterator iter, last;
    for ( iter = colQuery.faces.begin(), last = colQuery.faces.end(); iter != last; ++iter )
    {
      const MbFace * face = isa_cast<const MbFace*>( iter->second );
      EXPECT_NE( face, NULL_PTR );
      const_cast<MbFace*>(face)->SetColor( iter->first == item1 ? RGB(0xFF,0,0):RGB(0xFF,0xFF,0) );
    }
    EXPECT_EQ( colQuery.faces.size(), 4U );
    TestVariables::viewManager->RefreshModel();    
  }
}


//----------------------------------------------------------------------------------------
// Demonstrating sample: collision detection
/*
  This sample demonstrates how to apply collision detection utility to show
  interferencies of mixed objects (solids or meshes) grouped into components.
  Collisions between objects from same components are skipped.
*/
// ---
void SampleCollisionDetectionGroups()
{
  /*
    Create a box, first solid.
  */
  SPtr<MbSolid> boxSol1 = _CreateElementarySolid<et_Block>(
    MbCartPoint3D{0.0, 0.0, 0.0}, MbCartPoint3D{3.0, 0.0, 0.0},
    MbCartPoint3D{0.0, 3.0, 0.0}, MbCartPoint3D{0.0, 0.0, 3.0},
    MbSNameMaker{1, MbSNameMaker::i_SideNone, 0} );

  /*
    Create a box, second solid.
  */
  SPtr<MbSolid> boxSol2 = _CreateElementarySolid<et_Block>(
    MbCartPoint3D{0.0, 0.0, 0.0}, MbCartPoint3D{3.0, 0.0, 0.0},
    MbCartPoint3D{0.0, 3.0, 0.0}, MbCartPoint3D{0.0, 0.0, 3.0},
    MbSNameMaker{2, MbSNameMaker::i_SideNone, 0} );

  /*
    Create a tetrahedron a polygonal object.
  */
  SPtr<MbMesh> tetMesh = _CreateElementaryMesh<et_Pyramid>(
    MbCartPoint3D{0.0, 0.0, 0.0}, MbCartPoint3D{3.0, 0.0, 0.0},
    MbCartPoint3D{0.0, 3.0, 0.0}, MbCartPoint3D{0.0, 0.0, 3.0},
    MbSNameMaker{3, MbSNameMaker::i_SideNone, 0} );

  EXPECT_NE( boxSol1, NULL_PTR );
  EXPECT_NE( boxSol2, NULL_PTR );
  EXPECT_NE( tetMesh, NULL_PTR );
  if ( boxSol1 && boxSol2 && tetMesh )
  {
    CdCollisionDetection cDet;

    /*
      Add components in the scene.
    */
    size_t component1{};
    size_t component2{};

    cdet_item comp1 = cDet.AddComponent( (void*) &component1 );
    cdet_item comp2 = cDet.AddComponent( (void*) &component2 );

    /*
      Create instances positioned in the global coordinate system (placement).
    */
    SPtr<MbInstance> inst11{ new MbInstance{ *tetMesh, MbPlacement3D{{0.0, 0.0, 0.0}} } };
    SPtr<MbInstance> inst12{ new MbInstance{ *boxSol1, MbPlacement3D{{3.1, 1.2, 0.0}} } };
    SPtr<MbInstance> inst13{ new MbInstance{ *tetMesh, MbPlacement3D{{6.2, 0.0, 0.0}} } };
    SPtr<MbInstance> inst21{ new MbInstance{ *tetMesh, MbPlacement3D{{2.9, 5.3, 0.0}} } };
    SPtr<MbInstance> inst22{ new MbInstance{ *boxSol2, MbPlacement3D{{6.0, 3.1, 2.0}} } };

    /*
      Draw instances
    */
    for ( const auto & inst : {inst11, inst12, inst13} )
    {
      TestVariables::viewManager->AddObject( TestVariables::ELEMENTARY_Style, inst );
    }
    for ( const auto & inst : {inst21, inst22} )
    {
      TestVariables::viewManager->AddObject( TestVariables::BACKGROUND_Style, inst );
    }

    /*
      Add instances of objects to the components of the scene with their own placement.
    */
    cDet.AddInstance( comp1, inst11->GetItem(), inst11->GetPlacement(), (void*)inst11.get() );
    cDet.AddInstance( comp1, inst12->GetItem(), inst12->GetPlacement(), (void*)inst12.get() );
    cDet.AddInstance( comp1, inst13->GetItem(), inst13->GetPlacement(), (void*)inst13.get() );
    cDet.AddInstance( comp2, inst21->GetItem(), inst21->GetPlacement(), (void*)inst21.get() );
    cDet.AddInstance( comp2, inst22->GetItem(), inst22->GetPlacement(), (void*)inst22.get() );

    /*
      The structure of the query to find collisions.
    */
    cdet_collided_faces colQuery{};

    /*
      Define groups which collision detection must skip.
    */
    colQuery.Group( inst11.get(), inst12.get() );
    colQuery.Group( inst11.get(), inst13.get() );
    colQuery.Group( inst21.get(), inst22.get() );

    /*
      Detect the collisions of the scene.
    */
    cdet_result res = cDet.CheckCollisions( colQuery );
    EXPECT_EQ( CDM_RESULT_CollisionDetected, res );
    EXPECT_EQ( 6, colQuery.faces.size() );

    /*
      The cycle colouring faces interfering with each other.
    */
    for ( auto iter = std::cbegin(colQuery.faces), last = std::cend(colQuery.faces); iter != last; ++iter )
    {
      if ( const MbFace * face = isa_cast<const MbFace*>( iter->second ) )
      {
        bool isSameGroup = iter->first == inst11 || iter->first == inst12 || iter->first == inst13;
        const_cast<MbFace*>(face)->SetColor( isSameGroup ? RGB(0xFF,0,0) : RGB(0xFF,0xFF,0) );
      }
    }
    TestVariables::viewManager->RefreshModel();
  }
}

// eof