//////////////////////////////////////////////////////////////////////////////////////////
//
//    The file contains source codes demonstrating samples how to use multiple classification
//    of solids.
//
//    WARNING: This code is only an example. The real application can require more 
//             functionality and data types for implementation.
// 
//////////////////////////////////////////////////////////////////////////////////////////

#include <test_samples.h>
#include <test_variables.h>
#include <test_manager.h>
#include <test_service.h>

#include <action_general.h>
#include <action_solid.h>

#include <last.h>


//------------------------------------------------------------------------------
// Получить общий габарит тел в открытой модели.
// ---
static
void GetGeneralCube( MbCube & generalCube, RPArray<MbSolid> & enterSolids )
{
  RPArray<MbItem> objs( 0, 1 );
  SArray<MbMatrix3D> matrs( 0, 1 );

  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    
  if ( wind != nullptr )
    wind->GetObjects( st_Solid, objs, matrs );

  size_t objsCnt = objs.size();

  if ( objsCnt > 0 && wind != nullptr ) {
    MbCube cube;
    for ( size_t i = 0; i < objsCnt; ++i ) {
      MbItem * solidItem = objs[i];
      if ( solidItem->IsA() == st_Solid ) {
        MbSolid * solid = static_cast< MbSolid* >( solidItem );
        solid->CalculateGabarit( cube );
        generalCube.Union( generalCube, cube );
        enterSolids.Add( solid );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создать элементарный блок с заданными размерами.
// ---
static
void CreateElementaryBlocks( const MbCube & generalCube, size_t countX, size_t countY, size_t countZ, RPArray<MbSolid> & blocks )
{
  double dx = ( generalCube.pmax.x - generalCube.pmin.x ) / countX; // Размеры элементарного блока.
  double dy = ( generalCube.pmax.y - generalCube.pmin.y ) / countY;
  double dz = ( generalCube.pmax.z - generalCube.pmin.z ) / countZ;

  for ( size_t i = 0; i < countX; i++ ) {
    for ( size_t j = 0; j < countY; j++ ) {
      for ( size_t k = 0; k < countZ; k++ ) {

        c3d::SpacePointsVector points;
        MbCartPoint3D p0, pnt;
        p0 = generalCube.pmin;
        generalCube.GetVertex( 1, pnt );
        MbVector3D vx( p0, pnt );
        vx.Normalize();
        generalCube.GetVertex( 3, pnt );
        MbVector3D vy( p0, pnt );
        vy.Normalize();
        generalCube.GetVertex( 4, pnt );
        MbVector3D vz( p0, pnt );
        vz.Normalize();
        points.push_back( p0 );
        points.push_back( p0 + vx * dx );
        points.push_back( p0 + vy * dy );
        points.push_back( p0 + vz * dz );
        MbSNameMaker names( ct_ElementarySolid, MbSNameMaker::i_SideNone, 0 );
        MbElementarySolidResults results;
        ElementarySolid( MbElementarySolidParams(et_Block, points, names), results );
        MbSolid * block = results.DetachSolid();
        if ( block != nullptr ) {
          MbVector3D to( i * dx, j * dy, k * dz );
          block->Move( to ); // Сдвинуть на следующую позицию.
        }
        blocks.Add( block );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Отрисовать элементарный блок.
// ---
void DrawElementaryBlock( const RPArray<MbSolid> & blocks, const std::vector<MbUnitInfo> & info,
                          size_t indBlock, int kindOfSOlid )
{
  MbSolid *       block    = blocks[indBlock];
  MbeItemLocation location = info[indBlock].location;
  size_t          index    = info[indBlock].index;

  if ( kindOfSOlid == 3 || // Все блоки.
       (kindOfSOlid == 0 && location == iloc_OnItem) ||    // Только пересекающиеся.
       (kindOfSOlid == 1 && location == iloc_OutOfItem) || // Только внешние.
       (kindOfSOlid == 2 && location == iloc_InItem) ) {   // Только внутренние.
    Style style;
    if ( location == iloc_OnItem ) // Пересечение. 
      style = TestVariables::EQU_Style;
    else if ( location == iloc_OutOfItem ) // Вне тел.
      style = TestVariables::CURVE_Style;
    else if ( index == 0 ) // Внутри первого тела.
      style = TestVariables::GOLDEN_Style;
    else if ( index == 1 ) // Внутри второго тела.
      style = TestVariables::POLYLINE_Style;
    else if ( index == 2 )
      style = TestVariables::BSPLINE_Style;
    else if ( index != SYS_MAX_T )
      style = TestVariables::OFFSET_Style;

    //TestVariables::viewManager->AddObject( style, &block->GetFace( 3 )->SetSurface() ); // Отрисовать только сторону блока.
    TestVariables::viewManager->AddObject( style, block ); // Отрисовать блок.
  }
}


//------------------------------------------------------------------------------
// Отрсисовать результаты классификации.
// ---
static
void DrawResults( const RPArray<MbSolid> & blocks, const std::vector<MbUnitInfo> & info,
                  int countx, int county, int countz, int kindOfSOlid, int drawSection )
{
  size_t countX = static_cast< size_t >( countx );
  size_t countY = static_cast< size_t >( county );
  size_t countZ = static_cast< size_t >( countz );
  if ( kindOfSOlid < 0 )
    kindOfSOlid = 0;
  else if ( kindOfSOlid > 3 )
    kindOfSOlid = 3;

  if ( drawSection <= 0 ) {
    size_t i = countX / 2; // Отрисовать сечение при х = const. 
    for ( size_t j = 0; j < countY; j++ ) {
      for ( size_t k = 0; k < countZ; k++ ) {
        size_t indBlock = i * countY * countZ + j * countZ + k; // Индекс блока.
        ::DrawElementaryBlock( blocks, info, indBlock, kindOfSOlid );
      }
    }
  }
  else if ( drawSection == 1 ) {
    for ( size_t i = 0; i < countX; i++ ) {
      size_t j = countY / 2;
      for ( size_t k = 0; k < countZ; k++ ) {
        size_t indBlock = i * countY * countZ + j * countZ + k; // Индекс блока.
        ::DrawElementaryBlock( blocks, info, indBlock, kindOfSOlid );
      }
    }
  }
  else if ( drawSection == 2 ) {
    for ( size_t i = 0; i < countX; i++ ) {
      for ( size_t j = 0; j < countY; j++ ) {
        size_t k = countZ / 2;
        size_t indBlock = i * countY * countZ + j * countZ + k; // Индекс блока.
        ::DrawElementaryBlock( blocks, info, indBlock, kindOfSOlid );
      }
    }
  }
  else if ( drawSection >= 3 ) {
    for ( size_t i = 0; i < countX; i++ ) {
      for ( size_t j = 0; j < countY; j++ ) {
        for ( size_t k = 0; k < countZ; k++ ) {
          size_t indBlock = i * countY * countZ + j * countZ + k; // Индекс блока.
          ::DrawElementaryBlock( blocks, info, indBlock, kindOfSOlid );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создат цилиндр.
// ---
static
MbSolid * CreateCylinderSolid( SimpleName uniqueMainName )
{
  double radius = 20;
  double height = 100;

  MbCartPoint3D p0, pz, px;
  p0.SetZero();
  pz.Init( 0, 0, height );
  px.Init( radius, 0, 0 );

  c3d::SpacePointsVector points;
  points.push_back( p0 );
  points.push_back( pz );
  points.push_back( px );

  MbSNameMaker names( uniqueMainName, MbSNameMaker::i_SideNone, 0 );
  c3d::ShellSPtr shell0;
  MbElementarySolidResults results;
  ElementarySolid( MbElementarySolidParams(et_Cylinder, points, names), results );
  MbSolid * cylinder = results.DetachSolid();

  return cylinder;
}


//------------------------------------------------------------------------------
// Разбить общий габарит тел модели на элементарные блоки и классифицированить их относительно тел.
// ---
void SampleSolidsClassification() 
{
  MbCube generalCube;
  RPArray<MbSolid> enterSolids;
  ::GetGeneralCube( generalCube, enterSolids ); // Получить габарит модели.

  int useYourselfSolid = generalCube.IsEmpty() ? 0 : 1;

  if ( useYourselfSolid == 1 ) // Если есть открытые тела уточнить, использовать их или создать новое.
    ::GetInt( IDS_ENTER_USE_YOURSELF, useYourselfSolid );

  if ( useYourselfSolid == 0 ) { // Создать новое тело.
    enterSolids.clear();
    enterSolids.Add( ::CreateCylinderSolid(0) );
    if ( enterSolids[0] != nullptr )
      enterSolids[0]->CalculateGabarit( generalCube );

    TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, enterSolids[0] );
  }

  int countX = 15; // Количество элементов по осям.
  int countY = 10;
  int countZ = 30;
  if ( ::GetInt(IDS_ENTER_NUMBER_X, countX) && countX > 0 && countX < 150 &&
       ::GetInt(IDS_ENTER_NUMBER_Y, countY) && countY > 0 && countY < 150 &&
       ::GetInt(IDS_ENTER_NUMBER_Z, countZ) && countZ > 0 && countZ < 150 ) {

    size_t reserv = countX * countY * countZ;
    RPArray<MbSolid> blocks( reserv, 1 );
    ::CreateElementaryBlocks( generalCube, countX, countY, countZ, blocks ); // Создать элементарные блоки.

    SetWaitCursor( true );
    MbAccurateTimer ac;
    ac.Begin();

    std::vector<MbUnitInfo> info;
    ::MultipleSolidsClassification( blocks, enterSolids, info ); // Классификация блоков.

    ac.End();
    SetWaitCursor( false );

    size_t insideFirst = 0, insideOther = 0, intersect = 0, outOf = 0;
    for ( size_t i = 0, iSize = blocks.Count(); i < iSize; i++ ) {
      if ( info[i].location == iloc_OnItem )
        intersect++;
      else if ( info[i].location == iloc_OutOfItem )
        outOf++;
      else if ( info[i].index == 0 )
        insideFirst++;
      else if ( info[i].index >= 1 )
        insideOther++;
    }
    size_t summ = outOf + intersect + insideFirst + insideOther;
    
    if ( static_cast<size_t>(countX * countY * countZ) == summ ) { // Если все блоки классифицированы.
      const size_t bufLen = 1024;
      TCHAR buf[bufLen];
      size_t len = 0;
      len += ( size_t ) _sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
      len += ( size_t ) _sntprintf( buf + len, bufLen - len, _T("Outside blocks: %d \n"), (int)outOf );
      len += ( size_t ) _sntprintf( buf + len, bufLen - len, _T("Intersecting blocks: %d \n"), (int)intersect );
      len += ( size_t ) _sntprintf( buf + len, bufLen - len, _T("Blocks in solid 1: %d \n"), (int)insideFirst );
      len += ( size_t ) _sntprintf( buf + len, bufLen - len, _T("Blocks in other solid: %d \n"), (int)insideOther );
      len += ( size_t ) _sntprintf( buf + len, bufLen - len, _T("Total blocks: %d \n"), (int)summ );
      ::MessageBoxEx( buf, IDS_RESULTS );
    
      int kindOfSolid = 3; // Какие блоки рисовать.
      int drawSection = 0; // Где рисовать блоки.
      if ( ::GetInt(IDS_ENTER_KIND_OF_SOLID, kindOfSolid) &&
           ::GetInt(IDS_ENTER_DRAW_SECTION, drawSection) ) {
        SetWaitCursor( true );
        ::DrawResults( blocks, info, countX, countY, countZ, kindOfSolid, drawSection ); // Отрисовать результат.
        SetWaitCursor( false );
        ::MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );
      }
    }
    else
      ::MessageBoxEx( RT_ERROR_RESULT_NOT_OBTAINED, IDS_TITLE_MSG );

    ::DeleteItems( blocks );
  }
}
