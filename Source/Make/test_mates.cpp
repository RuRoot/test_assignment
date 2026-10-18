#include "test_mates.h"

#include <ctime>

#include <tool_time_test.h>
#include <test.h>

#include <solid.h>
#include <creator.h>
#include <action_solid.h>
#include <generic_utility.h>
#include <gcm_mates_generator.h>

#include <test.h>
#include <test_style.h>
#include <test_variables.h>
#include <test_manager.h>
#include <test_service.h>

#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
//
// ---
static void Get4BoxVertices( const MtTestBox & box, SArray<MbCartPoint3D> & points )
{
  points.Flush();
  MbPlacement3D place;
  box.GetPlacement( place );
  MbCartPoint3D origin( -0.5 * box.Length(), -0.5 * box.Width(), - 0.5 * box.Height() );
  place.GetPointFrom( origin );
  points.Add( origin );

  MbCartPoint3D pnt;
  pnt.Init( 0.5 * box.Length(), -0.5 * box.Width(), -0.5 * box.Height() );
  place.GetPointFrom( pnt );
  points.Add( pnt );
  pnt.Init( -0.5 * box.Length(), 0.5 * box.Width(), -0.5 * box.Height() );
  place.GetPointFrom( pnt );
  points.Add( pnt );
  pnt.Init( -0.5 * box.Length(), -0.5 * box.Width(), 0.5 * box.Height() );
  place.GetPointFrom( pnt );
  points.Add( pnt );
}

//------------------------------------------------------------------------------
//
// ---
static void Get3PointsForCylinder( const MtTestBox & box, SArray<MbCartPoint3D> & points )
{
  points.Flush();
  MbPlacement3D place;
  box.GetPlacement( place );
  const double h = max_of( box.Length(), max_of(box.Width(), box.Height()) );
  MbCartPoint3D origin( -h, 0., 0. );
  place.GetPointFrom( origin );
  points.Add( origin );

  MbCartPoint3D pnt;
  pnt.Init( 2. * h, 0., 0. ); //  точка на оси OZ, по которой будет задана высота
  place.GetPointFrom( pnt );
  points.Add( pnt );
  pnt.Init( -h, 0., box.Radius() );// Точка на оси OX, по которой будет задан радиус
  place.GetPointFrom( pnt );
  points.Add( pnt );
}

//------------------------------------------------------------------------------
//
// ---
static void ShowMtBox( const MtTestBox & box, Style style, bool showItem )
{
  MbSolid * solid = nullptr;
  MbSNameMaker names1;
  SArray<MbCartPoint3D> points( 4, 1 );
  ::Get4BoxVertices( box, points );
  
  {
    MbElementarySolidResults results;
    SpacePointsVector auxPoints( points.begin(), points.end() );
    ::ElementarySolid( MbElementarySolidParams( et_Block, auxPoints, names1 ), results ); // ::ElementarySolid( points, et_Block, names1, solid ); 
    solid = results.DetachSolid();
  }

  if ( box.IsHoled() )
  {
    MbSolid * solid2 = nullptr;
    MbSNameMaker names2;
    Get3PointsForCylinder( box, points );
    {
      MbElementarySolidResults results;
      SpacePointsVector auxPoints( points.begin(), points.end() );
      ::ElementarySolid( MbElementarySolidParams( et_Cylinder, auxPoints, names2 ), results ); // ::ElementarySolid( points, et_Cylinder, names2, solid2 ); 
      solid2 = results.DetachSolid();
    }

    MbSolid * solid1 = solid;
    MbSNameMaker operNames( ct_BooleanSolid, MbSNameMaker::i_SideNone, 0 );

    SolidSPtr sol1( solid1 ), sol2( solid2 ), newSolid;
    ::BooleanResult( sol1, cm_Copy, sol2, cm_Copy, MbBooleanOperationParams( bo_Difference, true, operNames ),
                     newSolid ); // ::BooleanSolid ( *solid1, cm_Copy, *solid2, cm_Copy, bo_Difference, operNames, solid );
    solid = newSolid.detach();
  }
  if ( solid != nullptr ) {
    TestVariables::viewManager->AddObject( style, solid, showItem );
  }
}

//------------------------------------------------------------------------------
//
// ---
static void ShowAssembly( const MtBoxVector & boxes )
{
  Style style = TestVariables::ELEMENTARY_Style;
  for ( size_t i = 0, cnt = boxes.size(); i < cnt; ++i )
  {
    style = ( (i == 0) || (i % 2 == 0) ) ? TestVariables::ELEMENTARY_Style : TestVariables::PPOINT_Style;
    ::ShowMtBox( *boxes[i], style, (i == cnt-1) );
  }
}

//------------------------------------------------------------------------------
//
// ---
static void ShowAssembly( const MtBlocksVector & boxes )
{
  for ( size_t i = 0, cnt = boxes.size(); i < cnt; ++i )
  {
    ShowAssembly( boxes[i] );
  }
}

//------------------------------------------------------------------------------
//
// ---
static void CheckResult( MtResultCode3D res, size_t mateCnt )
{
  ptrdiff_t tmp = 0;
  if ( res != mtResCode_Ok )
  {
    tmp = 0;
    ::GetInt( IDS_MATES_NUMBER, tmp );
  }
  else
  {
    tmp = static_cast<ptrdiff_t>( mateCnt );
    ::GetInt( IDS_MATES_NUMBER, tmp );
  }
}

//------------------------------------------------------------------------------
//
// ---
bool RigidConnectedBricks()
{
  MtResultCode3D res = GCM_RESULT_None;
  ptrdiff_t tmp = 1;
  size_t type = 1;
  ::GetInt( IDS_ENTER_RIGASSAMBLY_TYPE, tmp );
  type = static_cast<size_t>( (tmp == 2 || tmp == 3) ? tmp : 1 );

  size_t brickCnt = 2;
  tmp = 2;
  ::GetInt( IDS_ENTER_BRICK_COUNT_SIDE, tmp );
  brickCnt = static_cast<size_t>( tmp > 2 ? tmp : 2 );

  SPtr<TMGround> ground( new TMGround(MbPlacement3D::global) );
  SPtr<TMBoxPositioner> positioner( new TMBoxPositioner(*ground) );
  SPtr<MtGeomSolver> manager = GCM_CreateSolver( positioner );

  SET_TIME_TEST_REL( true );
  AssemblyGenerator assembly( *manager );
  MtBoxVector cube;
  size_t mateCnt = 0;
  BEGIN_TIME_REL( _T("GenerateBoxesCube"), _T("Test.exe:") );
  if ( type == 1 )
  {
    mateCnt = assembly.GenerateLine( cube, brickCnt );
  }
  else if ( type == 2 )
  {
    mateCnt = assembly.GenerateWall( cube, brickCnt );
  }
  else if ( type == 3 )
  {
    mateCnt = assembly.GenerateCube( cube, brickCnt );
  }
  else
  {
    PRECONDITION( false );
  }
  END_TIME_REL( _T("GenerateBoxesCube") );

    uint32 miliSec = ::GetTickCountEx();
  BEGIN_TIME_REL( _T("MtGeomSolver::Evaluate"), _T("MATH_CONSTRAINTS:") );
    res = manager->Evaluate();
  END_TIME_REL( _T("MtGeomSolver::Evaluate") );
    miliSec = ::GetTickCountEx() - miliSec;
  ::CheckResult( res, mateCnt );

  if ( ::GetBoolean(IDS_ENTER_SHOW_ASSEMBLY) )
  {
    ShowAssembly( cube );
  }

  CHECK_TIME_TEST();
  TimeTest * testResult = nullptr;
  testResult = ::GetTimeTestResults();
  string_t mateMsg( IDS_MATES );
  ::ShowMateTimeTestResult( testResult, mateCnt, cube.size(), res, mateMsg.c_str() );
  SET_TIME_TEST_REL( false );
  return OK( res );
}

//------------------------------------------------------------------------------
//
// ---
bool MateFractal()
{
  MtResultCode3D res = GCM_RESULT_None;
  size_t brickCnt = 2;
  ptrdiff_t tmp = 2;
  ::GetInt( IDS_ENTER_BLOCK_COUNT, tmp );
  brickCnt = static_cast<size_t>( tmp > 2 ? tmp : 2 );

  AssemblyGenerator::TMMateType mateType = AssemblyGenerator::tmt_NonRigid2Axis;
  if ( ::GetBoolean(IDS_SPECIFY_RIGID_STATUS) )
  {
    tmp = 1;
    ::GetInt( IDS_ENTER_RIGID_TYPE, tmp );
    if ( tmp == 2 )
    {
      mateType = AssemblyGenerator::tmt_Rigid3Bricks;
    }
    else if ( tmp == 3 )
    {
      mateType = AssemblyGenerator::tmt_Rigid2Axis;
    }
    else
    {
      mateType = AssemblyGenerator::tmt_Rigid1Brick;
    }
  }

  MbPlacement3D place( MbVector3D::xAxis, MbVector3D::yAxis, MbCartPoint3D::origin );
  SPtr<TMGround> ground( new TMGround(place) );
  SPtr<TMBoxPositioner> positioner( new TMBoxPositioner(*ground) );
  SPtr<MtGeomSolver> manager = GCM_CreateSolver( positioner );

  SET_TIME_TEST_REL( true );
  AssemblyGenerator assembly( *manager );
  MtBlocksVector fractal;
  BEGIN_TIME_REL( _T("GenerateFractal"), _T("Test.exe:") )
    size_t mateCnt = assembly.GenerateFractal( fractal, brickCnt, mateType );
  END_TIME_REL( _T("GenerateFractal") )

    uint32 miliSec = ::GetTickCountEx();
  BEGIN_TIME_REL( _T("MtGeomSolver::Evaluate"), _T("Test.exe:") )
    res = manager->Evaluate();
  END_TIME_REL( _T("MtGeomSolver::Evaluate") )
    miliSec = ::GetTickCountEx() - miliSec;
  ::CheckResult( res, mateCnt );

  if ( ::GetBoolean(IDS_ENTER_SHOW_ASSEMBLY) )
  {
    ShowAssembly( fractal );
  }
  CHECK_TIME_TEST();
  TIME_TEST_REPORT_REL( _T("MatesLog.txt") );
  TimeTest * testResult = nullptr;
  testResult = ::GetTimeTestResults();
  string_t mateMsg( IDS_MATES );
  ::ShowMateTimeTestResult( testResult, mateCnt, fractal.size(), res, mateMsg.c_str() );
  SET_TIME_TEST_REL( false );
  return res == mtResCode_Ok;
}

//------------------------------------------------------------------------------
//
// ---
bool DistributedDoFMates()
{
  MtResultCode3D res = GCM_RESULT_None;
  size_t brickCnt = 3;
  size_t ringsCnt = 1;  
  
  AssemblyGenerator::TMMateType mateType = AssemblyGenerator::tmt_NonRigid2Axis;
  bool isRigid = ::GetBoolean( IDS_SPECIFY_RIGID_STATUS );
  if ( isRigid )
  {
    ptrdiff_t tmp = 12;
    ::GetInt( IDS_ENTER_BLOCK_COUNT, tmp );
    brickCnt = static_cast<size_t>( tmp > 12 ? tmp : 12 );
    tmp = 1;
    ::GetInt( IDS_ENTER_RINGS_COUNT, tmp );
    ringsCnt = static_cast<size_t>( tmp > 0 ? tmp : 1 );

    tmp = 1;
    ::GetInt( IDS_ENTER_RIGID_TYPE, tmp );
    if ( tmp == 2 )
    {
      mateType = AssemblyGenerator::tmt_Rigid3Bricks;
    }
    else if ( tmp == 3 )
    {
      mateType = AssemblyGenerator::tmt_Rigid2Axis;
    }
    else
    {
      mateType = AssemblyGenerator::tmt_Rigid1Brick;
    }
  }
  else
  {
    ptrdiff_t tmp = 3;
    ::GetInt( IDS_ENTER_BLOCK_COUNT, tmp );
    brickCnt = static_cast<size_t>( tmp > 3 ? tmp : 3 );
    
    tmp = 1;
    ::GetInt( IDS_ENTER_NONRIGID_TYPE, tmp );
    if ( tmp == 2 )
    {
      mateType = AssemblyGenerator::tmt_NonRigidAxisDist;
    }
    else if ( tmp == 3 )
    {
      mateType = AssemblyGenerator::tmt_NonRigidAxis;
    }
    else
    {
      PRECONDITION( mateType == AssemblyGenerator::tmt_NonRigid2Axis );
    }
  }

  MbPlacement3D place( MbVector3D::xAxis, MbVector3D::yAxis, MbCartPoint3D::origin );
  SPtr<TMGround> ground( new TMGround(place) );
  SPtr<TMBoxPositioner> positioner( new TMBoxPositioner(*ground) );
  SPtr<MtGeomSolver> manager = GCM_CreateSolver( positioner );

  SET_TIME_TEST_REL( true );
  AssemblyGenerator assembly( *manager );
  MtBlocksVector distributed;
  size_t mateCnt = 0;
  BEGIN_TIME_REL( _T("DistributedDoF"), _T("Test.exe:") )
  if ( isRigid )
  {
    mateCnt = assembly.RigidDistributedDoF( distributed, ringsCnt, brickCnt, mateType );
  }
  else
  {
    mateCnt = assembly.NonRigidDistributedDoF( distributed, brickCnt, mateType );
  }
  END_TIME_REL( _T("DistributedDoF") )
  
  uint32 miliSec = ::GetTickCountEx();
  BEGIN_TIME_REL( _T("MtGeomSolver::Evaluate"), _T("MATH_CONSTRAINTS:") )  
  res = manager->Evaluate();
  if ( res != mtResCode_Ok )
  {
    res = manager->Evaluate();
  }
  END_TIME_REL( _T("MtGeomSolver::Evaluate") )
  miliSec = ::GetTickCountEx() - miliSec;
  ::CheckResult( res, mateCnt );

  if ( ::GetBoolean(IDS_ENTER_SHOW_ASSEMBLY) )
  {
    ShowAssembly( distributed );
  }

  //----------------------------------------------------------------------------
  BEGIN_TIME_REL( _T("MtBoxConstraint::SetDistance"), _T("MATH_CONSTRAINTS:") )  
  for ( std::list<SPtr<MtBoxConstraint>>::iterator iter = assembly.dimConstrs.begin(), last = assembly.dimConstrs.end(); iter != last; ++iter )
  {
    (*iter)->SetDistance( 50. );
  }
  END_TIME_REL( _T("MtBoxConstraint::SetDistance") )
  MtResultCode3D dimRes = mtResCode_Ok;
  BEGIN_TIME_REL( _T("MtGeomSolver::ChangeDimension"), _T("MATH_CONSTRAINTS:") )
  for ( std::list<SPtr<MtBoxConstraint>>::iterator iter = assembly.dimConstrs.begin(), last = assembly.dimConstrs.end(); iter != last && dimRes == mtResCode_Ok; ++iter )
  {
    dimRes = manager->ChangeDimension( **iter );
  }
  END_TIME_REL( _T("MtGeomSolver::ChangeDimension") )
  if ( dimRes != mtResCode_Ok )
  {
    MessageBoxEx( IDS_ERROR, IDS_MESSAGE );
  }
  if ( ::GetBoolean(IDS_ENTER_SHOW_ASSEMBLY) )
  {
    ShowAssembly( distributed );
  }
  
  //----------------------------------------------------------------------------

  CHECK_TIME_TEST();
  TIME_TEST_REPORT_REL( _T("MatesLog.txt") );
  TimeTest * testResult = nullptr;
  testResult = ::GetTimeTestResults();
  string_t mateMsg( IDS_MATES );
  ::ShowMateTimeTestResult( testResult, mateCnt, distributed.size(), res, mateMsg.c_str() );
  SET_TIME_TEST_REL( false );
  return res == mtResCode_Ok;
}

//------------------------------------------------------------------------------
//
// ---
bool CommonMatesTest()
{
  MbPlacement3D place( MbVector3D::xAxis, MbVector3D::yAxis, MbCartPoint3D::origin );
  SPtr<TMGround> ground( new TMGround(place) );
  SPtr<TMBoxPositioner> positioner( new TMBoxPositioner(*ground) );
  SPtr<MtGeomSolver> manager = GCM_CreateSolver( positioner );

  std::vector<size_t> matesCnt( 4, SYS_MAX_T );
  std::vector<size_t> blocksCnt( 4, SYS_MAX_T );
  std::vector<MtResultCode3D> results( 4, GCM_RESULT_None );

  SET_TIME_TEST_REL( true );
  AssemblyGenerator assembly( *manager );
  MtBoxVector cube;
  BEGIN_TIME_REL( _T("GenerateCube"), _T("Test.exe:") )
  matesCnt[0] = assembly.GenerateCube( cube, 10 );
  END_TIME_REL( _T("GenerateCube") )
  BEGIN_TIME_REL( _T("GCM_Evaluate - Cube"), _T("MATH_CONSTRAINTS:") )  
  results[0] = manager->Evaluate();
  END_TIME_REL( _T("GCM_Evaluate - Cube") )
  blocksCnt[0] = cube.size();

  CHECK_TIME_TEST();
  TIME_TEST_REPORT_REL( _T("MatesLog.txt") );
  TimeTest * testResult0 = nullptr;
  testResult0 = ::GetTimeTestResults();
  string_t mateMsg0( IDS_MATES );
  ::ShowMateTimeTestResult( testResult0, matesCnt[0], blocksCnt[0], results[0], mateMsg0.c_str() );
  SET_TIME_TEST_REL( false );

  SET_TIME_TEST_REL( true );
  manager->Flush();
  MtBlocksVector blocks;
  BEGIN_TIME_REL( _T("GenerateFractal"), _T("Test.exe:") )
  matesCnt[1] = assembly.GenerateFractal( blocks, 125 );
  END_TIME_REL( _T("GenerateFractal") )
  BEGIN_TIME_REL( _T("GCM_Evaluate - Fractal"), _T("MATH_CONSTRAINTS:") )  
  results[1] = manager->Evaluate();
  END_TIME_REL( _T("GCM_Evaluate - Fractal") )
  blocksCnt[1] = blocks.size();

  CHECK_TIME_TEST();
  TIME_TEST_REPORT_REL( _T("MatesLog.txt") );
  TimeTest * testResult1 = nullptr;
  testResult1 = ::GetTimeTestResults();
  string_t mateMsg1( IDS_MATES );
  ::ShowMateTimeTestResult( testResult1, matesCnt[1], blocksCnt[1], results[1], mateMsg1.c_str() );
  SET_TIME_TEST_REL( false );

  SET_TIME_TEST_REL( true );
  manager->Flush();
  blocks.clear();
  BEGIN_TIME_REL( _T("RigidDistributedDoF"), _T("Test.exe:") )
  matesCnt[2] = assembly.RigidDistributedDoF( blocks, 7, 20 );
  END_TIME_REL( _T("RigidDistributedDoF") )
  BEGIN_TIME_REL( _T("GCM_Evaluate - RigidDistr"), _T("MATH_CONSTRAINTS:") )  
  results[2] = manager->Evaluate();
  END_TIME_REL( _T("GCM_Evaluate - RigidDistr") )
  blocksCnt[2] = blocks.size();

  CHECK_TIME_TEST();
  TIME_TEST_REPORT_REL( _T("MatesLog.txt") );
  TimeTest * testResult2 = nullptr;
  testResult2 = ::GetTimeTestResults();
  string_t mateMsg2( IDS_MATES );
  ::ShowMateTimeTestResult( testResult2, matesCnt[2], blocksCnt[2], results[2], mateMsg2.c_str() );
  SET_TIME_TEST_REL( false );

  SET_TIME_TEST_REL( true );
  manager->Flush();
  blocks.clear();
  BEGIN_TIME_REL( _T("NonRigidDistributedDoF"), _T("Test.exe:") )
  matesCnt[3] = assembly.NonRigidDistributedDoF( blocks, 125 );
  END_TIME_REL( _T("NonRigidDistributedDoF") )
  BEGIN_TIME_REL( _T("GCM_Evaluate - NonRigidDistr"), _T("MATH_CONSTRAINTS:") )  
  results[3] = manager->Evaluate();
  END_TIME_REL( _T("GCM_Evaluate - NonRigidDistr") )
  blocksCnt[3] = blocks.size();

  CHECK_TIME_TEST();
  TIME_TEST_REPORT_REL( _T("MatesLog.txt") );
  TimeTest * testResult3 = nullptr;
  testResult3 = ::GetTimeTestResults();
  string_t mateMsg3( IDS_MATES );
  ::ShowMateTimeTestResult( testResult3, matesCnt[3], blocksCnt[3], results[3], mateMsg3.c_str() );
  SET_TIME_TEST_REL( false );

  return results[0] == mtResCode_Ok && results[1] == mtResCode_Ok && results[2] == mtResCode_Ok && results[3] == mtResCode_Ok;
}

//------------------------------------------------------------------------------
// Задержка по времени в секундах.
// ---
static void SleepSec( double delay )
{
  time_t begTime = time(nullptr);
  double currDiff = 0;
  while ( currDiff < delay )
  {
    currDiff = difftime( time(nullptr), begTime );
  }
}

//------------------------------------------------------------------------------
// Нарисовать линейный паттерн.
// ---
static void ShowPattern( const MtGeomSolver * solver, 
                        std::vector<SPtr<MtTestBox>>::iterator iter, std::vector<SPtr<MtTestBox>>::iterator last )
{
  TestVariables::viewManager->DeleteModel(); // Очистить модель
  TestVariables::viewManager->EraseBackground( TestVariables::viewManager->GetActiveWindowIndex() );
  for ( size_t i=0; iter!=last; ++iter, ++i )
  {
    MbPlacement3D place( solver->TransMatrix(*iter) );
    ( *iter )->SetPlacement( place );
    ::ShowMtBox( **iter, i%2==0? TestVariables::ELEMENTARY_Style : TestVariables::PPOINT_Style, true );
  }
  SleepSec( 2. );
}

//------------------------------------------------------------------------------
/// Создать линейный массив компонент.
// ---
bool LinearPattern()
{
  MtResultCode3D res = GCM_RESULT_None;
  SPtr<MtGeomSolver> solver ( GCM_CreateSolver(nullptr) );

  std::vector<SPtr<MtTestBox>> arrBoxes;
  // 1. Сгенерируем 5 кубиков.
  MbPlacement3D place( MbVector3D::xAxis, MbVector3D::yAxis, MbCartPoint3D::origin );
  TMBoxSize bSize( 3., 3., 3., 3. );
  arrBoxes.push_back( c3d::make_sptr<MtTestBox>(place, bSize) );
  place.Rotate( MbAxis3D::xAxis, M_PI_4 );
  place.Rotate( MbAxis3D::zAxis, M_PI_4 );
  
  arrBoxes.push_back( c3d::make_sptr<MtTestBox>(place, bSize) );
  place.Rotate( MbAxis3D::xAxis,  M_PI_4 );
  place.Rotate( MbAxis3D::zAxis, -M_PI_4 );
  
  arrBoxes.push_back( c3d::make_sptr<MtTestBox>(place, bSize) );
  place.Rotate( MbAxis3D::xAxis, M_PI_4 );
  place.Rotate( MbAxis3D::zAxis, M_PI_4 );
  
  arrBoxes.push_back( c3d::make_sptr<MtTestBox>(place, bSize) );
  place.Rotate( MbAxis3D::xAxis, -M_PI_4 );
  place.Rotate( MbAxis3D::zAxis,  M_PI_4 );
  
  arrBoxes.push_back( c3d::make_sptr<MtTestBox>(place, bSize) );
  place.Rotate( MbAxis3D::xAxis, M_PI_4 );
  place.Rotate( MbAxis3D::zAxis, M_PI_4 );

  // 2. Сгенерируем вспомогательный кубик для задания направления массива.
  place.Init( MbCartPoint3D::origin, MbVector3D::zAxis, MbVector3D::xAxis );
  place.Rotate( MbAxis3D::xAxis, M_PI_4 );
  place.Rotate( MbAxis3D::zAxis, -M_PI_4 );
  SPtr<MtTestBox> dirBox( new MtTestBox(place, bSize) );

  // 3. Зададим линейный массив компонент
  MtGeomVariant gcmDirLine;
  gcmDirLine.SetAsLine( place.GetOrigin(), place.GetAxisX() );
  MtArgument dirAxis( dirBox, gcmDirLine );  // Направляющая

  res = solver->FixGeom( arrBoxes[0] );
  PRECONDITION( res == mtResCode_Ok ); 
  res = solver->FixGeom( dirBox );
  PRECONDITION( res == mtResCode_Ok );

  MtPatternId ptrn = solver->AddPattern( GCM_LINEAR_PATTERN, MtArgument(arrBoxes[0]), dirAxis );
  solver->AddGeomToPattern( ptrn, MtArgument(arrBoxes[1]),  10., GCM_NO_ALIGNMENT );
  solver->AddGeomToPattern( ptrn, MtArgument(arrBoxes[2]),  20., GCM_ALIGNED );
  solver->AddGeomToPattern( ptrn, MtArgument(arrBoxes[3]), -10., GCM_NO_ALIGNMENT );
  solver->AddGeomToPattern( ptrn, MtArgument(arrBoxes[4]), -20., GCM_ALIGNED );
  
  // Решить систему и нарисовать кубики
  res = solver->Evaluate();
  PRECONDITION( res == mtResCode_Ok );
  ::ShowPattern( solver, arrBoxes.begin(), arrBoxes.end() );
  
  res = solver->UnfixGeom( *arrBoxes[0] );
  PRECONDITION( res == mtResCode_Ok );
  // Сдвинем оригинал вдоль оси X.
  place.Init( solver->TransMatrix(dirBox) );
  solver->PrepareReposition( *arrBoxes[0], place, MbCartPoint3D::origin );
  for ( size_t i=0; i<5; ++i ) // Число сдвигов
  {
    const double delta = i < 2 ?  double( i+1 ) : double( 5-i );
    res = solver->SolveReposition( MbCartPoint3D(15.*delta, 0., 0.) );
    PRECONDITION( res == mtResCode_Ok );
    ShowPattern( solver, arrBoxes.begin(), arrBoxes.end() );
  }
  solver->FinishReposition();
  
  // Повернем копию вокруг оси.
  res = solver->UnfixGeom( *dirBox );
  PRECONDITION( res == mtResCode_Ok ); 
  res = solver->FixGeom( arrBoxes[0] );
  PRECONDITION( res == mtResCode_Ok ); 
  MbMatrix3D trans = solver->TransMatrix( dirBox );
  place.SetOrigin( trans.GetOrigin() );
  solver->PrepareReposition( *dirBox, place.GetOrigin(), place.GetAxisX() );
  for ( size_t i=0; i < 6; ++i ) // Число сдвигов
  {
    res = solver->SolveReposition( double(i+1) * M_PI/6. );
    PRECONDITION( res == mtResCode_Ok );
    ShowPattern( solver, arrBoxes.begin(), arrBoxes.end() );
  }
  solver->FinishReposition();

  
  return true;
}

//------------------------------------------------------------------------------
//  Свойства кубика.
// ---
struct TMBoxProp
{
  SPtr<MtTestBox>    box;  // Кубик
  Style          style;  // Стиль прорисовки
  GCM_alignment  align;  // Тип выравнивания

  TMBoxProp( MtTestBox * b, Style s, GCM_alignment a=GCM_NO_ALIGNMENT ) : box( b ), style( s ), align( a ) {}
};

//------------------------------------------------------------------------------
// Нарисовать паттерн.
// ---
static void ShowPattern( const MtGeomSolver * solver,
                        std::vector<TMBoxProp>::iterator boxIt, std::vector<TMBoxProp>::iterator boxEnd )
{
  TestVariables::viewManager->DeleteModel(); // Очистить модель
  TestVariables::viewManager->EraseBackground( TestVariables::viewManager->GetActiveWindowIndex() );
  MbMatrix3D trans; MbPlacement3D place;
  for ( ; boxIt != boxEnd; ++boxIt )
  {
    if ( solver )
    {
      trans = solver->TransMatrix( boxIt->box );
      place.Init( trans );
      boxIt->box->SetPlacement( place );
    }
    ::ShowMtBox( *boxIt->box, boxIt->style, true );
  }
  SleepSec( 2. );
}

//------------------------------------------------------------------------------
/// Создать угловой массив компонент.
// ---
bool AngularPattern()
{
  MtResultCode3D res = GCM_RESULT_None;
  //GCM_system solver = GCM_CreateSystem();
  SPtr<MtGeomSolver> solver ( GCM_CreateSolver(nullptr) );

  // 1. Сгенерируем кубики.
  std::vector<TMBoxProp> boxes;
  TMBoxSize bSize( 10., 10., 10., 10. );

  MbPlacement3D place( MbVector3D::xAxis, MbVector3D::yAxis, MbCartPoint3D::origin );
  boxes.push_back( TMBoxProp(new MtTestBox(place, bSize), TestVariables::ARC_Style) );

  place.Move( MbVector3D(100., 0., 0.) );
  boxes.push_back( TMBoxProp(new MtTestBox(place, bSize), TestVariables::PPOINT_Style) );
  
  // Копии
  place.InitXY( MbCartPoint3D::origin, MbVector3D::xAxis, MbVector3D::yAxis, true );
  place.Rotate( MbAxis3D::xAxis, M_PI_4 );
  place.Rotate( MbAxis3D::zAxis, M_PI_4 );
  boxes.push_back( TMBoxProp(new MtTestBox(place, bSize), TestVariables::ELEMENTARY_Style, GCM_NO_ALIGNMENT) );

  place.Rotate( MbAxis3D::xAxis, M_PI_4 );
  place.Rotate( MbAxis3D::zAxis, -M_PI_4 );
  boxes.push_back( TMBoxProp(new MtTestBox(place, bSize), TestVariables::ELEMENTARY_Style, GCM_NO_ALIGNMENT) );

  place.Rotate( MbAxis3D::xAxis, M_PI_4 );
  place.Rotate( MbAxis3D::zAxis, -M_PI_4 );
  boxes.push_back( TMBoxProp(new MtTestBox(place, bSize), TestVariables::ELEMENTARY_Style, GCM_NO_ALIGNMENT) );

  place.Rotate( MbAxis3D::xAxis, -M_PI_4 );
  place.Rotate( MbAxis3D::zAxis, M_PI_4 );
  boxes.push_back( TMBoxProp(new MtTestBox(place, bSize), TestVariables::ELEMENTARY_Style, GCM_ALIGNED) );

  place.Rotate( MbAxis3D::xAxis, M_PI_4 );
  place.Rotate( MbAxis3D::zAxis, M_PI_4 );
  boxes.push_back( TMBoxProp(new MtTestBox(place, bSize), TestVariables::ELEMENTARY_Style, GCM_ALIGNED) );

  // Ось
  boxes[0].box->GetPlacement( place );
  MtGeomVariant gcmDirLine;
  gcmDirLine.SetAsLine( place.GetOrigin(), place.GetAxisZ() );
  MtArgument dirAxis( boxes[0].box, gcmDirLine );
  ShowPattern( nullptr, boxes.begin(), boxes.end() );
  // Зафиксируем ось
  res = solver->FixGeom( boxes[0].box );
  PRECONDITION( res == mtResCode_Ok );
  
  // Зададим угловой паттерн в системе ограничений.
  MtPatternId ptrn = solver->AddPattern( GCM_ANGULAR_PATTERN, MtArgument(boxes[1].box), dirAxis );
  for ( size_t i=1; i<6; ++i )
  {
    solver->AddGeomToPattern( ptrn, MtArgument(boxes[i+1].box), double(i)*M_PI2/6., boxes[i+1].align );
  }

  res = solver->Evaluate();
  PRECONDITION( res == mtResCode_Ok );
  ShowPattern( solver, boxes.begin(), boxes.end() );

  if ( res == mtResCode_Ok )
  {
    // Сдвинем образец вдоль вектора, чтобы продемонстрировать вариацию скалярной переменной.
    MbMatrix3D trans = solver->TransMatrix( boxes[0].box ); 
    place.Init( trans );
    place.Rotate( MbAxis3D::xAxis, M_PI_4 );
    place.Rotate( MbAxis3D::zAxis, M_PI_4 );
    solver->PrepareReposition( *boxes[1].box, place, MbCartPoint3D::origin );
    for ( size_t i=0; i<5; ++i ) // Число сдвигов
    {
      const double delta = i < 2 ?  double( i + 1 ) : double( 5-i );
      const double xRot = 25. * delta * cos( i * M_PI/6. );
      const double yRot = 25. * delta * sin( i * M_PI/6. );
      res = solver->SolveReposition( MbCartPoint3D(xRot, yRot, 15. * double(i)) );
      PRECONDITION( res == mtResCode_Ok );
      ShowPattern( solver, boxes.begin(), boxes.end() );
    }
    solver->FinishReposition();

    // Повернем копию вокруг оси, чтобы продемонстрировать работу выравнивания.
    trans = solver->TransMatrix( boxes[5].box );
    place.SetOrigin( trans.GetOrigin() );
    solver->PrepareReposition( *boxes[5].box, place.GetOrigin(), place.GetAxisZ() );
    for ( size_t i=0; i<6; ++i ) // Число сдвигов
    {
      res = solver->SolveReposition( double(i+1) * M_PI/6. );
      PRECONDITION( res == mtResCode_Ok );
      ShowPattern( solver, boxes.begin(), boxes.end() );
    }
    solver->FinishReposition();
  }
  
  return true;
}
