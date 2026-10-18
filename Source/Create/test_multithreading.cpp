////////////////////////////////////////////////////////////////////////////////
//
// Примеры использования многопоточности
// Examples of multithreading usage
//
////////////////////////////////////////////////////////////////////////////////
#include <test.h>
#include <test_space.h>
#include <test_manager.h>
#include <test_service.h>
#include <test_draw.h>
#include <test_window.h>
#include <test_edit_space.h>
#include <test_converter.h>

#include <map_section.h>
#include <map_section_complex.h>       
#include <map_thread.h>

#include <solid.h>
#include <mb_cart_point3d.h>
#include <templ_s_array.h>

#include <curve3d.h>
#include <cur_contour3d.h>
#include <space_instance.h>

#include <mb_placement3d.h>
#include <generic_utility.h>
#include <ctime>

#include <mip_curve_properties.h>
#include <mip_solid_mass_inertia.h>

#include <tool_time_test.h>

#include <last.h>

using namespace c3d;

#if defined C3D_NATIVE_LOCK // Тестирование распараллеливания с использованием других API (не OpenMP)
  #define MULTITHREADING_WITH_NON_OMP
  #include <thread>
#endif

//#define COMPARE_SERIAL_PARALLEL_CALCULATIONS

inline static void CallGetVestiges(
  const MbPlacement3D *       place,      // проекционная плоскость
  double *                    znear,      // параметр перспективного изображения
  const RPArray<MbLump> *     lumps,      // проецируемые объекты
  MbVEFVestiges *             outdata,    // результат
  const MbMapVisibilityMode * visMode)
{
  MbProjectionsObjects prObjects;
  ::GetVestiges( *place, *znear, *lumps, prObjects, *outdata, *visMode, Math::DefaultMathVersion() );
}

//------------------------------------------------------------------------------
// Многопоточное проецирование. 
// Multithreaded mapping.
//  internalParallel - enable internal parallelism
// ---
void BetaMultithreaded0( bool internalParallel )
{
  ///  Собрать все Solid'ы.
  ///  Gather all solids
  PArray<MbLump> lumps  ( 1, 1, true );
  MbMatrix3D from;
  MbItem * obj = nullptr;
  PArray<MbItem> objs( 0, 1, false );
  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
  SArray<MbMatrix3D> matrs;
  if ( wind != nullptr )
    wind->GetObjects( st_Solid, objs, matrs );
  size_t objsCnt = objs.Count();
  for ( size_t i = 0; i < objsCnt; i++ )
  {
    obj = objs[i];
    if ( obj != nullptr && obj->IsA() == st_Solid )
      lumps.Add( new MbLump( *(MbSolid *)obj, matrs[i], 0/*nullptr*/ ) );
  }

  const size_t lumpsCnt   = lumps.Count();             
  if ( (lumpsCnt > 0) ) {

    WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();
    
    ///  Создать 4 плэйсмента с началами в центре габаритного куба
    ///  Create 4 placement with origin in center of bounding box
    MbCube cube;
    for ( size_t i = 0; i < lumpsCnt; i++ )
    {
      MbCube c;
      lumps[i]->GetItem().AddYourGabaritTo(c);
      c.Transform( lumps[i]->GetMatrixFrom() );
      cube |= c;
    }
    SArray<MbPlacement3D> places;
    MbCartPoint3D p;
    p.Init( cube.Centre().x, cube.Centre().y, cube.Centre().z);
    MbPlacement3D placeTop( MbVector3D::xAxis, MbVector3D::yAxis, p );
    places.Add( placeTop );
    MbPlacement3D placeFront( MbVector3D::yAxis, MbVector3D::zAxis, p );
    places.Add( placeFront );
    MbPlacement3D placeSide( MbVector3D::zAxis, MbVector3D::xAxis, p );
    places.Add( placeSide );
    MbVector3D z( 1, 1, 1 );
    MbVector3D x( 1, 0, -1 );
    z.Normalize();
    x.Normalize();
    MbPlacement3D placeIso( p, z, x );
    places.Add( placeIso );

    double znear = currentView3D->GetZVista(); 
    MbMapVisibilityMode visMode( false, false );
    
    SetWaitCursor( true );

    uint miliSec = ::GetTickCountEx();
    Style ns;
    MbAccurateTimer ac;
    const size_t bufLen = 1024;
    TCHAR buf[bufLen];
    size_t len = 0;

    size_t visCount = 0;
    int m, placesCount = (int)places.Count();
    
    // проверка окружения
#ifndef _OPENMP
    len += ( size_t )_sntprintf( buf + len, bufLen - len, OPENMP_IS_OFF );
#endif
#ifdef C3D_NATIVE_LOCK
    len += ( size_t )_sntprintf( buf + len, bufLen - len, SYSTEM_MUTEX_ON );
#endif

    ///  Установить уровень многопоточности
    ///  Set level of multithreading
    MbeMultithreadedMode mode =  Math::MultithreadedMode();

    Math::SetMultithreadedMode( mtm_Items );
    len += ( size_t )_sntprintf( buf + len, bufLen - len, Math::CheckMultithreadedMode(mtm_Items) ? INTERNAL_MUTLTHREADING_IS_ON : INTERNAL_MUTLTHREADING_IS_OFF );

    ///  Многопоточное проецирование
    ///  Multithreading projection

#ifdef MULTITHREADING_WITH_NON_OMP
    {
      ParallelRegionGuard gg;
      ///  Создать 4 массива следов
      ///  Create 4 vestiges array
      PArray<MbVEFVestiges> vestigesArray2( placesCount, 1 );
      for ( m = 0; m < placesCount; m++ )
        vestigesArray2.Add( new MbVEFVestiges() );
      int threadsCnt = 4;
      std::vector<std::thread> threads;
      threads.reserve( threadsCnt );
      size_t threadInd = 0;

      ac.Begin();
      for ( ; threadInd < threadsCnt - 1; ++threadInd ) {
        MbProjectionsObjects prObjects;
        MbPlacement3D & pp = places[threadInd];
        MbVEFVestiges * result = vestigesArray2[threadInd];
        threads.push_back ( std::thread ( CallGetVestiges, &pp, &znear, &lumps, result, &visMode ) );
      }
      CallGetVestiges( &places[threadInd], &znear, &lumps, vestigesArray2[threadInd], &visMode );
      for ( threadInd = 0; threadInd < threadsCnt - 1; ++threadInd )
        threads[threadInd].join();
      ac.End();

      len += ( size_t )_sntprintf( buf + len, bufLen - len, PARALLEL_CYCLE_STD_THREAD );
      len += ( size_t )_sntprintf( buf + len, bufLen - len, RT_TIME_SEC, ac.GetLast() );
    }

    Math::SetMultithreadedMode( mtm_SafeItems );
    len += ( size_t )_sntprintf( buf + len, bufLen - len, INTERNAL_MUTLTHREADING_IS_OFF );
    
    {
      EnterParallelRegion();

      ///  Создать 4 массива следов
      ///  Create 4 vestiges array
      PArray<MbVEFVestiges> vestigesArray4( placesCount, 1 );
      for ( m = 0; m < placesCount; m++ )
        vestigesArray4.Add( new MbVEFVestiges() );
      int threadsCnt = 4;
      std::vector<std::thread> threads;
      threads.reserve( threadsCnt );
      size_t threadInd = 0;

      ac.Begin();
      for ( ; threadInd < threadsCnt - 1; ++threadInd ) {
        MbProjectionsObjects prObjects;
        MbPlacement3D & pp = places[threadInd];
        MbVEFVestiges * result = vestigesArray4[threadInd];
        threads.push_back ( std::thread ( CallGetVestiges, &pp, &znear, &lumps, result, &visMode ) );
      }
      CallGetVestiges( &places[threadInd], &znear, &lumps, vestigesArray4[threadInd], &visMode );
      for ( threadInd = 0; threadInd < threadsCnt - 1; ++threadInd )
        threads[threadInd].join();
      ac.End();

      ExitParallelRegion();
    }

    len += ( size_t )_sntprintf( buf + len, bufLen - len, PARALLEL_CYCLE_STD_THREAD );
    len += ( size_t )_sntprintf( buf + len, bufLen - len, RT_TIME_SEC, ac.GetLast() );

#endif

    // compare with enabled internal parallelism 
    if ( Math::MultithreadedMode() < mtm_Items ) {
      Math::SetMultithreadedMode( mtm_Max );
      len += ( size_t )_sntprintf( buf + len, bufLen - len, INTERNAL_MUTLTHREADING_IS_ON );
    }

    ///  Создать 4 массива следов
    ///  Create 4 vestiges array
    PArray<MbVEFVestiges> vestigesArray3( placesCount, 1 );
    for ( m = 0; m < placesCount; m++ )
      vestigesArray3.Add( new MbVEFVestiges() );

    ac.Begin();
    ENTER_PARALLEL_FORCED;
#pragma omp parallel for reduction(+:visCount) shared( places, znear, lumps ) num_threads (placesCount)
    for( int placeInd = 0; placeInd < placesCount; placeInd++ )
    {
      MbProjectionsObjects prObjects;
      ::GetVestiges( places[placeInd], znear, lumps, prObjects, *vestigesArray3[placeInd], visMode, Math::DefaultMathVersion() );
    }
    EXIT_PARALLEL_FORCED;
    ac.End();

    len += ( size_t )_sntprintf( buf + len, bufLen - len, PARALLEL_CYCLE_OMP );
    len += ( size_t )_sntprintf( buf + len, bufLen - len, RT_TIME_SEC, ac.GetLast() );

    if ( !internalParallel ) { // internal parallelism off
      Math::SetMultithreadedMode( mtm_SafeItems );

      len += ( size_t )_sntprintf( buf + len, bufLen - len, INTERNAL_MUTLTHREADING_IS_OFF );
      ///  Создать 4 массива следов
      ///  Create 4 vestiges array
      PArray<MbVEFVestiges> vestigesArray( placesCount, 1 );
      for ( m = 0; m < placesCount; m++ )
        vestigesArray.Add( new MbVEFVestiges() );

      ac.Begin();
      ENTER_PARALLEL_FORCED;
#pragma omp parallel for reduction(+:visCount) shared( places, znear, lumps ) num_threads (placesCount)
      for ( int placeInd = 0; placeInd < placesCount; placeInd++ ) {
        MbProjectionsObjects prObjects;
        ::GetVestiges( places[placeInd], znear, lumps, prObjects, *vestigesArray[placeInd], visMode, Math::DefaultMathVersion() );
      }
      EXIT_PARALLEL_FORCED;
      ac.End();

      len += ( size_t )_sntprintf( buf + len, bufLen - len, PARALLEL_CYCLE_OMP );
      len += ( size_t )_sntprintf( buf + len, bufLen - len, RT_TIME_SEC, ac.GetLast() );
    }


    ///  Востановить уровень многопоточности
    ///  Restore level of multithreading
    Math::SetMultithreadedMode( mode );

    ///  Добавить следы в модель
    ///  Add vestiges to a model
    miliSec = ::GetTickCountEx() - miliSec;

    /*for ( m = 0; m < placesCount; m++ )
    {
      for ( size_t i = 0, count = vestigesArray[m]->edgeVestiges.Count(); i < count; i++ ) { // Следы ребер, владеет
        MbCurveVestige & vest = vestigesArray[m]->edgeVestiges[i]->SetCurveInfo(); // Информация о следе кривой

        size_t cnt = vest.GetVisiblePartsCount();

        if ( cnt > 0 ) {
          for ( size_t j = 0; j < cnt; j++ ) {
            MbCurve * curve = vest._PickupVisibleCurve( j );
            TestVariables::viewManager->AddObject( ns, curve, &places[m], false );
            visCount++;
          }
        }
      }
      for ( size_t i = 0, count = vestigesArray[m]->faceVestiges.Count(); i < count; i++ ) { // Следы граней, владеет
        PArray<MbCurveVestige> & curveInfos = vestigesArray[m]->faceVestiges[i]->curveInfos;

        for ( ptrdiff_t k = 0, c = curveInfos.Count(); k < c; k++ ) { // Следы граней, владеет
          MbCurveVestige * vest = curveInfos[k]; // Информация о следе кривой
          size_t cnt = vest->GetVisiblePartsCount(); // Не видимые линии очерка

          if ( cnt > 0 ) {
            for ( size_t j = 0; j < cnt; j++ ) {
              MbCurve * curve = vest->_PickupVisibleCurve( j );
              TestVariables::viewManager->AddObject( ns, curve, &places[m] );
              visCount++;
            }
          }
        }
      }
    }*/

    //TimeTest * testResult = nullptr;
    //ShowTimeTestResult( testResult, visCount, 0, miliSec, false, IDS_PROJECTION_BODIES );

    SetWaitCursor( false );

    // \ru Показать результаты. \en Report results.
    ::MessageBoxEx( buf, THREAD_TEST_MULTITHREAD );
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// \ru Тесты для многопоточности. 
// \en Multithreading tests.
//
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// \ru Создание полигональных представлений для данных объектов.
// \en Creation of meshes for given items.
// ---
static void CreateMeshes ( std::vector< SPtr<MbItem> >& meshes, std::vector< SPtr<MbItem> >& items )
{
  bool wire = true; // Векторное отображение
  bool grid = true; // Растровое отображение
  double sag = Math::visualSag;
  MbStepData stepData(ist_SpaceStep, sag);
  MbFormNote note(wire, grid, true, TestVariables::exactMesh);
  ptrdiff_t count = (ptrdiff_t)items.size();

  for ( ptrdiff_t i = 0; i < count; i++ ) 
  {
    SPtr<MbItem> meshItem (items[i]->CreateMesh ( stepData, note, nullptr) );
    meshes.push_back ( meshItem );
  } 
}

//------------------------------------------------------------------------------
// \ru Создание копий объектов.
// \en Creation of item copies.
// ---
static void CopyItems( PArray<MbItem> & items, std::vector< SPtr<MbItem> > & copy )
{ 
  ptrdiff_t count = items.size();
  for ( ptrdiff_t i = 0; i < count; i++ ) {
    MbItem *item = items[i];
    SPtr<MbItem> itemCopy( static_cast<MbItem *>(&item->Duplicate()) );
    copy.push_back( itemCopy );
  }
}

//------------------------------------------------------------------------------
// \ru Тест сравнивает время создания полигональных объектов при последовательном и параллельном выполнении вычислений
//     (если compareSerial == true, сравнить результаты двух последовательных вычислений).
// \en The test compares times of serial and parallel calculations of meshes
//     (if compareSerial == true, compare results of two serial calculations).
// ---
void MultithreadingTest( bool compareSerial )
{
  PArray<MbItem> items( 0, 1, false ), items2( 0, 1, false );
  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
  SArray<MbMatrix3D> matrs, matrs2;
  if ( wind != nullptr ) {
    wind->GetObjects( st_Solid, items, matrs );
    wind->GetObjects( st_WireFrame, items2, matrs2 );
  }

  if ( (items.Count() + items2.Count()) == 0 )
    return;

  MbAccurateTimer ac;
  SetWaitCursor( true );

  // \ru Сохранить текущий режим многопоточности.
  // \en Save current multithreaded mode.
  MbeMultithreadedMode mode =  Math::MultithreadedMode();

  const size_t bufLen = 1024;
  TCHAR buf[bufLen];
  size_t len = 0;

  bool res = true;
  std::vector< SPtr<MbItem> > meshes1, meshes2;
  std::vector< SPtr<MbItem> > copy1, copy2;
  ::CopyItems( items, copy1 );
  ::CopyItems( items2, copy1 );
  ::CopyItems( items, copy2 );
  ::CopyItems( items2, copy2 );
  PRECONDITION ( copy1.size() == copy2.size() );
  for ( size_t i = 0; i < copy1.size(); ++i ) {
    if ( !copy1[i]->IsSame( *copy2[i] ) ) {
        res = false;
        break;
    }
  }
  if ( !res ) {
    ::MessageBoxEx( THREAD_MESH_TEST_COPY_FAILED, IDS_TITLE_ERR );
    return;
  }

  len += (size_t)_sntprintf( buf+len, bufLen-len, THREAD_MESH_TEST_NUM );
  len += (size_t)_sntprintf( buf+len, bufLen-len, THREAD_PROJECTION_FORMAT2, LoUint32(copy1.size()) );

  // \ru Последовательный расчет полигональных объектов. \en Serial calculation of meshes.
  {
    // \ru Установить последовательный доступ к объектам.
    // \en Set serial access to items.
    Math::SetMultithreaded( false );

    ac.Begin();
    CreateMeshes( meshes1, copy1 );
    ac.End();
    len += (size_t)_sntprintf( buf+len, bufLen-len, THREAD_TEST_SERIAL );
    len += (size_t)_sntprintf( buf+len, bufLen-len, RT_TIME_SEC, ac.GetLast() );
  }

  // \ru Параллельный расчет полигональных объектов. \en Parallel calculation of meshes.
  {
    // \ru Установить многопоточный доступ к объектам.
    // \en Set multithreaded access to items.
    if ( !compareSerial )
      Math::SetMultithreadedMode( mtm_Items );

    ac.Begin();
    CreateMeshes( meshes2, copy2 );
    ac.End();
    len += (size_t)_sntprintf( buf+len, bufLen-len, compareSerial ? THREAD_TEST_SERIAL : THREAD_TEST_MULTITHREAD );
    len += (size_t)_sntprintf( buf+len, bufLen-len, RT_TIME_SEC, ac.GetLast() );
  }

  SetWaitCursor ( false );

  if ( compareSerial ) {
    res = meshes1.size() == meshes2.size();
    if ( res ) {
      for ( size_t i = 0; i < meshes1.size(); ++i ) {
        MbMesh* m1 = static_cast< MbMesh* >( meshes1[i].get() );
        MbMesh* m2 = static_cast< MbMesh* >( meshes2[i].get() );
        if ( m1 == nullptr && m2 == nullptr )
          continue;
        if ( m1 == nullptr || m2 == nullptr || !m1->IsSame( *m2 ) ) {
          if ( m1 != nullptr && m2 != nullptr )
            m1->IsSame( *m2 );
          res = false;
          break;
        }
      }
    }
  }

  if ( !res ) {
    len += (size_t)_sntprintf( buf+len, bufLen-len, THREAD_PROJECTION_FORMAT3 );
    len += (size_t)_sntprintf( buf+len, bufLen-len, THREAD_MESH_TEST_FAILED );
  }

  // \ru Восстановить уровень многопоточности
  // \en Restore level of multithreading
  Math::SetMultithreadedMode( mode );

  // \ru Показать результаты. \en Report results.
  ::MessageBoxEx( buf, res ? THREAD_TEST_TITLE : IDS_TITLE_ERR );
}

//------------------------------------------------------------------------------
// \ru Тест меряет время проецирования базовых точек объекта на поверхность
//     или тело при последовательном и параллельном выполнении вычислений.
//     Кроме того, сравниваются результаты последовательных и параллельных вычислений.
// \en The test measures times of serial and parallel projections of base points
//     of the object on a surface or body.
//     Besides, results of serial and parallel calculations are compared.
// ---
void MultithreadingProjection()
{
  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
  if ( wind )
    wind->DeleteModel ();

  TCHAR * fileName = nullptr;
#ifndef __USE_QT__

  if ( _GetOpenFileName ( TestVariables::nameBuffer, SELECT_FILE_INFO, GetFilenameFilter().c_str(), NON_EXT ) )
    fileName = TestVariables::nameBuffer;
#endif
  bool result = false;
  // \ru Чтение модели из файла. \en Reading model from the file.
  if ( fileName != nullptr && ::SameExtension( fileName, C3D_EXT ) ) {
    if ( wind == nullptr ) {
      wind = new WorkWindow ( v_Isometry );
      wind->CreateWorkWindow ( IDS_TITLE_VIEWISOMETRY, TestVariables::viewManager->GetBackgrColor(), TestVariables::viewManager->GetLightDirection()/*lightDirection*/ );
      wind->OrientateWindow ( v_Isometry );

      ptrdiff_t newIndex = TestVariables::viewManager->AddWindow ( wind );
      TestVariables::viewManager->SetFocus( newIndex, false );
    }

    std::unique_ptr<reader_ex> pl = reader_ex::CreateReaderEx(std::unique_ptr<iobuf_Seq>(&::createiobuf(fileName)), io::in);
    if ( pl->good() ) {
      SetWaitCursor ( true );
      wind->ReadModel ( *pl );
      SetWaitCursor ( false );
      result = pl->good();
    }
  }
  if ( !result ) {
    MessageBoxEx( RT_FILE_NOT_OPEN, IDS_TITLE_ERR );
    return;
  }

  // \ru Спроецировать базовые точки объекта на поверхность или тело.
  // \en Project base points of the object on a surface or body.
  SurfaceBasisPointProjection ( true );
}

//------------------------------------------------------------------------------
// \ru Прочитать кривые из файла.
// \en Read curves from a file.
// ---
std::vector<MbCurve3D*> readCurves ( const TCHAR* path )
{
  MbModel model;
  reader::reader_ptr rc = reader::CreateReader( std::unique_ptr<iobuf_Seq>( &::createiobuf( path ) ), io::in );
  if ( !rc->good() )
    return std::vector<MbCurve3D*>();
  ::ReadModelItems ( *rc, model );

  RPArray<MbItem> items;
  SArray<MbMatrix3D> matrices;
  model.GetItems ( st_SpaceInstance, items, matrices );

  std::vector<MbCurve3D*> result;
  for ( size_t i = 0; i < items.size(); ++i ) {
    MbCurve3D* pItem = static_cast<MbCurve3D*>(&(dynamic_cast<const MbCurve3D*>(dynamic_cast<MbSpaceInstance*>(items[i])->GetSpaceItem()))->Duplicate(nullptr));
    pItem->Transform ( matrices[i] );
    result.push_back ( pItem );
  }

  return result;
}

//------------------------------------------------------------------------------
// \ru Проверка потокобезопасности кривых.
// \en Check for thread safety of curves.
// ---
void TestThreadSafety()
{
  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
  TCHAR * fileName = nullptr;
#ifndef __USE_QT__
  if ( _GetOpenFileName ( TestVariables::nameBuffer, SELECT_FILE_INFO, GetFilenameFilter().c_str(), NON_EXT ) )
    fileName = TestVariables::nameBuffer;
#endif
  std::vector<MbCurve3D*> curves;
  if ( fileName != nullptr && ::SameExtension( fileName, C3D_EXT ) ) {
    if ( wind )
      wind->DeleteModel();
    if ( wind == nullptr ) {
      wind = new WorkWindow( v_Isometry );
      wind->CreateWorkWindow( IDS_TITLE_VIEWISOMETRY, TestVariables::viewManager->GetBackgrColor(), TestVariables::viewManager->GetLightDirection()/*lightDirection*/ );
      wind->OrientateWindow( v_Isometry );

      ptrdiff_t newIndex = TestVariables::viewManager->AddWindow ( wind );
      TestVariables::viewManager->SetFocus( newIndex, false );
    }
    // \ru Чтение объектов из файла. \en Reading objects from the file.
    curves = readCurves ( fileName );

  }
  if ( !curves.size() ) {
    MessageBoxEx( _T("Load model: not found curves"), IDS_TITLE_ERR );
    return;
  }
  
  const MbeSpaceType curveType = (*curves.begin())->IsA();
  if ( curveType != st_Contour3D ) {
    MessageBoxEx( _T("Load model: not found contour"), IDS_TITLE_ERR );
    return;
  }

  const MbContour3D& contour = static_cast<const MbContour3D&>(*(*curves.begin()));
  const size_t kk = 5;
  if ( kk >= contour.GetSegmentsCount() ) {
    MessageBoxEx( _T("Load model: contour with insufficient segments number"), IDS_TITLE_ERR );
    return;
  }

  const MbCurve3D* pCurve = contour.GetSegment ( kk );
  const double sag = DEVIATION_SAG * 10.0;

  std::vector< std::vector<MbCartPoint3D> > points;
  points.resize(1000);
  std::vector<size_t> threadIds;
  threadIds.resize ( points.size() );
  std::vector< std::vector<double> > params;
  params.resize(1000);

  Math::SetMultithreadedMode( mtm_Items );

  ENTER_PARALLEL_FORCED;
#pragma omp parallel for
  for ( int i = 0; i < static_cast<int>(points.size()); ++i ) {
    std::vector<MbCartPoint3D>& iterationPoints = points[i];
    std::vector<double>& iterationParams = params[i];
    threadIds[i] = omp_get_thread_num();

    double t = pCurve->GetTMin();
    const double tmax = pCurve->GetTMax();

    bool hasNext = true;
    while (hasNext) {
      t += pCurve->Step ( t, sag );
      if ( t >= tmax - Math::paramEpsilon ) {
        t = tmax;
        hasNext = false;
      }
      iterationParams.push_back( t );

      MbCartPoint3D point;
//#pragma omp critical (PointOnTest)
      pCurve->PointOn ( t, point );
      iterationPoints.push_back( point );
    }
  }
  EXIT_PARALLEL_FORCED;

  bool failed = false;
  for ( size_t i = 0, ie = params.size() - 1; i < ie; ++i ) {
    for ( size_t j = i, je = params.size(); j < je; ++j ) {
      const bool equal = (params[i] == params[j]);
      if ( equal )
        continue;

      failed |= true;
      break;
    }
  }

  for ( size_t i = 0, ie = points.size() - 1; i < ie; ++i ) {
    for ( size_t j = i, je = points.size(); j < je; ++j ) {
      const bool equal = (points[i] == points[j]);
      if ( equal )
        continue;

      failed |= true;

      assert(points[i].size() == points[j].size());
      if ( points[i].size() == points[j].size() ) {
        for ( size_t k = 0, ke = points[i].size(); k < ke; ++k )
        {
          const bool equalPoint = (points[i][k] == points[j][k]);
          const bool equalParam = (params[i][k] == params[j][k]);
          //assert( equalPoint );
          if ( !equalPoint || !equalParam)
            break;
        }
      }
    }
  }

  //assert ( !failed );

  if ( !failed )
    MessageBoxEx( IDS_TITLE_DIAG_ALL_RIGHT, IDS_TITLE_MSG );
  else
     MessageBoxEx( IDS_TITLE_DIAG_ERR, IDS_TITLE_ERR );

}

static size_t GetVestigesCount( const MbVEFVestiges & vestiges )
{
  size_t visCount = 0;

  for ( size_t i = 0, count = vestiges.EdgeVestigesCount(); i < count; i++ ) { // Следы ребер, владеет
    const MbCurveVestige & vest = vestiges.GetEdgeVestige( i )->SetCurveInfo(); // Информация о следе кривой
    visCount += vest.GetVisiblePartsCount();
  }
  for ( size_t i = 0, count = vestiges.FaceVestigesCount(); i < count; i++ ) { // Следы граней, владеет
    const PArray<MbCurveVestige> & curveInfos = vestiges.GetFaceVestige( i )->curveInfos;
    for ( ptrdiff_t k = 0, c = curveInfos.Count(); k < c; k++ ) { // Следы граней, владеет
      const MbCurveVestige * vest = curveInfos[k]; // Информация о следе кривой
      visCount += vest->GetVisiblePartsCount();
    }
  }

  return visCount;
}

// return 0 - if match; 1 - if counts of vestiges do not match; 2 - if vestiges do not match
static int CompareVestiges( const MbVEFVestiges& vestiges1, const MbVEFVestiges& vestiges2 )
{
  int result = 0;
  if ( vestiges1.EdgeVestigesCount()== vestiges2.EdgeVestigesCount() &&
       vestiges1.FaceVestigesCount() == vestiges2.FaceVestigesCount() ) {
    for ( size_t i = 0, count = vestiges1.EdgeVestigesCount(); result == 0 && i < count; i++ ) { // Следы ребер, владеет
      const MbCurveVestige & vest1 = vestiges1.GetEdgeVestige( i )->SetCurveInfo(); // Информация о следе кривой
      const MbCurveVestige & vest2 = vestiges2.GetEdgeVestige( i )->SetCurveInfo(); // Информация о следе кривой

      const size_t cnt1 = vest1.GetVisiblePartsCount();
      const size_t cnt2 = vest2.GetVisiblePartsCount();
      const bool b1 = vest1.IsVisibleCurvesArray();
      const bool b2 = vest2.IsVisibleCurvesArray();

      if ( cnt1 > 0 && cnt1 == cnt2 ) {
        for ( size_t j = 0; j < cnt1; j++ ) {
          const MbCurve * crv1 = vest1._GetVisibleCurve( j );
          const MbCurve * crv2 = vest2._GetVisibleCurve( j );
          if ( !crv1->IsSame( *crv2 ) ) {
            result = 2;
            break;
          }
        }
      }
      else if ( b1 != b2 ) {
        result = 2;
        break;
      }
    }
    for ( size_t i = 0, count = vestiges1.FaceVestigesCount(); result == 0 && i < count; i++ ) { // Следы граней, владеет
      const PArray<MbCurveVestige> & curveInfos1 = vestiges1.GetFaceVestige( i )->curveInfos;
      const PArray<MbCurveVestige> & curveInfos2 = vestiges2.GetFaceVestige( i )->curveInfos;

      if ( curveInfos1.Count() != curveInfos2.Count() ) {
        result = 2;
        break;
      }
      for ( ptrdiff_t k = 0, c = curveInfos1.Count(); k < c; k++ ) { // Следы граней, владеет
        MbCurveVestige * vest1 = curveInfos1[k]; // Информация о следе кривой
        MbCurveVestige * vest2 = curveInfos2[k]; // Информация о следе кривой

        const size_t cnt1 = vest1->GetVisiblePartsCount();
        const size_t cnt2 = vest2->GetVisiblePartsCount();
        const bool b1 = vest1->IsVisibleCurvesArray();
        const bool b2 = vest2->IsVisibleCurvesArray();

        if ( cnt1 > 0 && cnt1 == cnt2 ) {
          for ( size_t j = 0; j < cnt1; ++j ) {
            const MbCurve * crv1 = vest1->_GetVisibleCurve( j );
            const MbCurve * crv2 = vest2->_GetVisibleCurve( j );
            if ( !crv1->IsSame( *crv2 ) ) {
              result = 2;
              break;
            }
          }
        }
        else if ( b1 != b2 ) {
          result = 2;
          break;
        }
      }
    }
  }
  else {
    result = 1;
  }
  return result;
}

//------------------------------------------------------------------------------
// Распараллеливание проецирования.
// Parallelized projection.
// runMode:
//     0 - run serial only
//     1 - run parallel only
//     2 - run both
// ---
void MultithreadedGetVestiges( int runMode )
{
  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
  if ( !wind ) {
    MessageBoxEx( _T("No data"), IDS_TITLE_ERR );
    return;
  }

  ///  Собрать все Solid'ы.
  ///  Gather all solids
  PArray<MbLump> lumps  ( 1, 1, true );
  MbMatrix3D from;
  MbItem * obj = nullptr;
  PArray<MbItem> objs( 0, 1, false );
  wind = TestVariables::viewManager->GetActiveWindow();
  SArray<MbMatrix3D> matrs;
  if ( wind != nullptr )
    wind->GetObjects( st_Solid, objs, matrs );
  size_t objsCnt = objs.Count();
  for ( size_t i = 0; i < objsCnt; i++ )
  {
    obj = objs[i];
    if ( obj != nullptr && obj->IsA() == st_Solid )
      lumps.Add( new MbLump( *(MbSolid *)obj, matrs[i], 0/*nullptr*/ ) );
  }

  const size_t lumpsCnt   = lumps.Count();             
  if ( (lumpsCnt == 0) ) {
    MessageBoxEx( _T("No solids selected for projection"), IDS_TITLE_ERR );
    return;
  }

  MbCube cube;
  for ( size_t i = 0; i < lumpsCnt; i++ )
  {
    MbCube c;
    lumps[i]->GetItem().AddYourGabaritTo(c);
    c.Transform( lumps[i]->GetMatrixFrom() );
    cube |= c;
  }
  MbCartPoint3D p;
  p.Init( cube.Centre().x, cube.Centre().y, cube.Centre().z);
  MbPlacement3D placeTop( MbVector3D::xAxis, MbVector3D::yAxis, p );
  MbPlacement3D placeFront( MbVector3D::yAxis, MbVector3D::zAxis, p );
  MbPlacement3D placeSide( MbVector3D::zAxis, MbVector3D::xAxis, p );
  MbVector3D z( 1, 1, 1 );
  MbVector3D x( 1, 0, -1 );
  z.Normalize();
  x.Normalize();
  MbPlacement3D placeIso( p, z, x );

  double znear = wind->GetZVista(); 
  MbMapVisibilityMode visMode( false, false );

  SetWaitCursor( true );

  size_t visCount1 = 0, visCount2 = 0;

  MbAccurateTimer ac;
  const size_t bufLen = 1024;
  TCHAR buf[bufLen];
  size_t len = 0;

  MbVEFVestiges vestiges1;
  MbeMultithreadedMode mode =  Math::MultithreadedMode();
  MbeMultithreadedMode runMode1 = mtm_Standard;
  MbeMultithreadedMode runMode2 = mtm_Items;

  // проверка окружения
#ifndef _OPENMP
  len += ( size_t )_sntprintf( buf + len, bufLen - len, OPENMP_IS_OFF );
#endif
#ifdef C3D_NATIVE_LOCK
  len += ( size_t )_sntprintf( buf + len, bufLen - len, SYSTEM_MUTEX_ON );
#endif

//#define MAP_INTERSECT_TIMING

#ifdef MAP_INTERSECT_TIMING
  ::SetTimeTest( true );
#endif

  if ( runMode == 0 || runMode == 2 )
  {
    ///  Установить стандартный уровень многопоточности
    ///  Set Standard level of multithreading
    Math::SetMultithreadedMode( runMode1 );

#ifdef MAP_INTERSECT_TIMING
    ::BeginTime( _T("MultithreadedGetVestiges: Serial execution"), _T("TOTAL:")  );
#endif

    ///  Проецирование
    ///  Projection
    ac.Begin();
    MbProjectionsObjects prObjects;
    ::GetVestiges( placeFront, znear, lumps, prObjects, vestiges1, visMode, Math::DefaultMathVersion() );
    ac.End();

#ifdef MAP_INTERSECT_TIMING
    ::EndTime( _T("MultithreadedGetVestiges: Serial execution") );
#endif

    visCount1 = GetVestigesCount( vestiges1 );

    len += (size_t)_sntprintf( buf+len, bufLen-len, STANDARD_CALCULATION );
    len += (size_t)_sntprintf( buf+len, bufLen-len, RT_TIME_SEC, ac.GetLast() );
  }
  
  bool result = true;
  if ( runMode >= 1 )
  {
    ///  Установить Items уровень многопоточности
    ///  Set Items level of multithreading
    Math::SetMultithreadedMode( runMode2 );

#ifdef MAP_INTERSECT_TIMING
    ::BeginTime( _T("MultithreadedGetVestiges::Parallel execution"), _T("TOTAL:")  );
#endif

    ///  Проецирование
    ///  Projection
    ac.Begin();
    MbProjectionsObjects prObjects;
    MbVEFVestiges vestiges2;
    ::GetVestiges( placeFront, znear, lumps, prObjects, vestiges2, visMode, Math::DefaultMathVersion() );
    ac.End();

#ifdef MAP_INTERSECT_TIMING
    ::EndTime( _T("MultithreadedGetVestiges::Parallel execution") );
#endif

    visCount2 = GetVestigesCount( vestiges2 );

    len += (size_t)_sntprintf( buf+len, bufLen-len, PARALLEL_CALCULATION );
    len += (size_t)_sntprintf( buf+len, bufLen-len, RT_TIME_SEC, ac.GetLast() );

#ifdef COMPARE_SERIAL_PARALLEL_CALCULATIONS
    if ( runMode == 2 ) {
      int res = visCount1 == visCount2 ? CompareVestiges( vestiges1, vestiges2 ) : 1;
      result = res == 0;
      if ( res == 1 )
        len += (size_t)_sntprintf( buf+len, bufLen-len, _T("---\nWARNING: Vestiges do not match!") );
      else  if ( res == 2)
        len += (size_t)_sntprintf( buf+len, bufLen-len, _T("---\nWARNING: Different number of vestiges!") );
    }
#endif
  }

  
  if ( result )
    len += (size_t)_sntprintf( buf+len, bufLen-len, _T("---\nNumber of vestiges: %d"), LoUint32(std_max(visCount1, visCount2)) );

#ifdef MAP_INTERSECT_TIMING
  ::CheckTimeTest();
  ::TimeTestReport( _T("_GetVestiges.log") );
  ::SetTimeTest( false );
#endif

  SetWaitCursor( false );
  ::MessageBoxEx ( buf, VIEW_PROJECTION_CREATION );

  ///  Востановить уровень многопоточности
  ///  Restore level of multithreading
  Math::SetMultithreadedMode( mode );
}

//------------------------------------------------------------------------------
// Многопоточное проецирование. 
// Multithreaded mapping.
// runMode:
//     0 - call GetVesiges() for each view in a cycle (sequentially)
//     1 - call GetVesiges() for each view in a parallel cycle
//     2 - call another overloaded GetVestiges() to calculate all views
//     3 - run 1 & 2 and compare results
// ---
void MultithreadedGetVestiges2( int runMode )
{
  ///  Собрать все Solid'ы.
  ///  Gather all solids
  PArray<MbLump> lumps  ( 1, 1, true );
  MbMatrix3D from;
  MbItem * obj = nullptr;
  PArray<MbItem> objs( 0, 1, false );
  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
  SArray<MbMatrix3D> matrs;
  if ( wind != nullptr )
    wind->GetObjects( st_Solid, objs, matrs );
  size_t objsCnt = objs.Count();
  for ( size_t i = 0; i < objsCnt; i++ )
  {
    obj = objs[i];
    if ( obj != nullptr && obj->IsA() == st_Solid )
      lumps.Add( new MbLump( *(MbSolid *)obj, matrs[i], 0/*nullptr*/ ) );
  }

  const size_t lumpsCnt   = lumps.Count();             
  if ( (lumpsCnt > 0) ) {

    WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();

    ///  Создать 4 плэйсмента с началами в центре габаритного куба
    ///  Create 4 placement with origin in center of bounding box
    MbCube cube;
    for ( size_t i = 0; i < lumpsCnt; i++ )
    {
      MbCube c;
      lumps[i]->GetItem().AddYourGabaritTo(c);
      c.Transform( lumps[i]->GetMatrixFrom() );
      cube |= c;
    }
    SArray<MbPlacement3D> places;
    MbCartPoint3D p;
    p.Init( cube.Centre().x, cube.Centre().y, cube.Centre().z);
    MbPlacement3D placeTop( MbVector3D::xAxis, MbVector3D::yAxis, p );
    places.Add( placeTop );
    MbPlacement3D placeFront( MbVector3D::yAxis, MbVector3D::zAxis, p );
    places.Add( placeFront );
    MbPlacement3D placeSide( MbVector3D::zAxis, MbVector3D::xAxis, p );
    places.Add( placeSide );
    MbVector3D z( 1, 1, 1 );
    MbVector3D x( 1, 0, -1 );
    z.Normalize();
    x.Normalize();
    MbPlacement3D placeIso( p, z, x );
    places.Add( placeIso );

    double znear = currentView3D->GetZVista(); 
    MbMapVisibilityMode visMode( false, false );

    SetWaitCursor( true );

    MbAccurateTimer ac;
    const size_t bufLen = 1024;
    TCHAR buf[bufLen];
    size_t len = 0;
    int placesCount = (int)places.Count();
    PArray<MbVEFVestiges>vestigesArray;
    PArray<MbVEFVestiges>vestigesArray2;

    // проверка окружения
#ifndef _OPENMP
    len += ( size_t )_sntprintf( buf + len, bufLen - len, OPENMP_IS_OFF );
#endif
#ifdef C3D_NATIVE_LOCK
    len += ( size_t )_sntprintf( buf + len, bufLen - len, SYSTEM_MUTEX_ON );
#endif

    ///  Установить максимальный кровень многопоточности
    ///  Set maximum level of multithreading
    MbeMultithreadedMode mode =  Math::MultithreadedMode();
    Math::SetMultithreadedMode( mtm_Max );

    if ( runMode == 0 || runMode == 3 ) {
      ///  Последовательное проецирование
      ///  Sequential projection
      PArray<MbVEFVestiges>vestigesArray1;
      for( int m = 0; m < placesCount; m++ )
        vestigesArray1.Add( new MbVEFVestiges() );

      ac.Begin();
      for( int m = 0; m < placesCount; m++ )
      {
        MbProjectionsObjects prObjects;
        ::GetVestiges( places[m], znear, lumps, prObjects, *vestigesArray1[m], visMode, Math::DefaultMathVersion() );
      }
      ac.End();
      len += (size_t)_sntprintf( buf+len, bufLen-len, SEQUENTIAL_CALCULATION );
      len += (size_t)_sntprintf( buf+len, bufLen-len, RT_TIME_SEC, ac.GetLast() );
    }

    if ( runMode == 1 || runMode == 3 ) {
      ///  Многопоточное проецирование с использованием отдельной функции для каждого вида
      ///  Multithreading projection using a separate function for each view
      for( int m = 0; m < placesCount; m++ )
        vestigesArray.Add( new MbVEFVestiges() );
      ac.Begin();

      ENTER_PARALLEL( Math::CheckMultithreadedMode(mtm_Max) );
#pragma omp parallel for shared( places, znear, lumps ) num_threads (placesCount) if( Math::CheckMultithreadedMode(mtm_Max) )
      for( int m = 0; m < placesCount; m++ )
      {
        MbProjectionsObjects prObjects;
        ::GetVestiges( places[m], znear, lumps, prObjects, *vestigesArray[m], visMode, Math::DefaultMathVersion() );
      }
      EXIT_PARALLEL( Math::CheckMultithreadedMode(mtm_Max) );
      ac.End();
      len += (size_t)_sntprintf( buf+len, bufLen-len, PARALLEL_CYCLE_CALCULATION );
      len += (size_t)_sntprintf( buf+len, bufLen-len, RT_TIME_SEC, ac.GetLast() );
    }

    if ( runMode >= 2 ) {
      ///  Многопоточное проецирование с использованием одной функции
      ///  Multithreading projection using one function for all views
      MbProjectionsObjects prObjects;
      SArray<MbMapSettings> settings;
      for( int m = 0; m < placesCount; m++ )
        settings.Add( MbMapSettings( visMode, places[m], znear ) );
      ac.Begin();
      GetVestiges( settings, lumps, prObjects, vestigesArray2, Math::DefaultMathVersion() );
      ac.End();
      len += (size_t)_sntprintf( buf+len, bufLen-len, ONE_FUNCTION_ALL_VIEWS );
      len += (size_t)_sntprintf( buf+len, bufLen-len, RT_TIME_SEC, ac.GetLast() );
    }

    SetWaitCursor( false );

    ///  Востановить уровень многопоточности
    ///  Restore level of multithreading
    Math::SetMultithreadedMode( mode );

    if ( runMode == 2 ) {
      // проверка данных
      int result = 0;
      for( size_t i = 0; i < placesCount; i++ ) {
        const MbVEFVestiges& vestiges1 = *vestigesArray[i];
        const MbVEFVestiges& vestiges2 = *vestigesArray2[i];
        size_t visCount1 = GetVestigesCount( vestiges1 );
        size_t visCount2 = GetVestigesCount( vestiges2 );
        if ( visCount1 != visCount2 ) {
          result = 1;
          break;
        }
        result = CompareVestiges( vestiges1, vestiges2 );
        if ( result != 0 )
          break;
      }
      if ( result == 1 )
        len += (size_t)_sntprintf( buf+len, bufLen-len, _T("---\nWARNING: Vestiges do not match!") );
      else  if ( result == 2)
        len += (size_t)_sntprintf( buf+len, bufLen-len, _T("---\nWARNING: Different number of vestiges!") );
    }

    ::MessageBoxEx ( buf, FOUR_VIEWS_CREATION );
  }
}


//------------------------------------------------------------------------------
//  оператор
// ---
static
bool CompareInertiaProperties ( const InertiaProperties &first, const InertiaProperties &other )
{
  if ( ::fabs(first.area - other.area) > Math::metricRegion )
    return false;
  if ( ::fabs( first.volume - other.volume ) > Math::metricRegion )
    return false;
  if ( ::fabs( first.mass - other.mass ) > Math::metricRegion )
    return false;
  for ( size_t i = 0; i < SPACE_DIM; i++ ) {
    if ( ::fabs( first.inertia[i] - other.inertia[i] ) > Math::metricRegion )
      return false;
    for ( size_t j = 0; j < SPACE_DIM; j++ ) {
      if ( ::fabs( first.initial[i][j] - other.initial[i][j] ) > Math::metricRegion )
        return false;
      if ( ::fabs( first.moments[i][j] - other.moments[i][j] ) > Math::metricRegion )
        return false;
    }
    if ( ::fabs( first.general[i] - other.general[i] ) > Math::metricRegion )
      return false;
    if ( first.direction[i] != other.direction[i] )
      return false;
  }
  return first.center == other.center;
}


//------------------------------------------------------------------------------
// Вывод на экран массо-центровочных характеристик
// ---
static
void OutputMassInertia( InertiaProperties & mp, double deviateAngle,
  clock_t first, clock_t second )
{
  TCHAR s[2048];
  TCHAR s0[512];
  double density = 0.0;
  if ( ::fabs( mp.volume ) > NULL_EPSILON )
    density = mp.mass / mp.volume;
  else {
    if ( ::fabs( mp.area ) > NULL_EPSILON )
      density = mp.mass / mp.area;
  }
  _sntprintf( s, 2048, RT_AREA_VOLUME_MASS_AND_DENSITY, mp.area, mp.volume, mp.mass, density );
  _sntprintf( s0, 512, RT_WEIGHT_CENTRE, mp.center.x, mp.center.y, mp.center.z );
  _tcscat( s, s0 );
  _sntprintf( s0, 512, RT_MATRIX_OF_MOMENTS_OF_INERTIA_IN_CURRENT_PLACE,
    mp.initial[0][0], mp.initial[0][1], mp.initial[0][2],
    mp.initial[1][0], mp.initial[1][1], mp.initial[1][2],
    mp.initial[2][0], mp.initial[2][1], mp.initial[2][2] );
  _tcscat( s, s0 );
  _sntprintf( s0, 512, RT_MATRIX_OF_CENTRAL_MOMENTS_OF_INERTIA,
    mp.moments[0][0], mp.moments[0][1], mp.moments[0][2],
    mp.moments[1][0], mp.moments[1][1], mp.moments[1][2],
    mp.moments[2][0], mp.moments[2][1], mp.moments[2][2] );
  _tcscat( s, s0 );
  if ( mp.direction[1].Length()>Math::lengthEpsilon ) {
    TCHAR s1[512];
    _sntprintf( s0, 512, RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA, mp.general[0], mp.general[1], mp.general[2] );
    _sntprintf( s1, 512, RT_DIRECTIONS_OF_PRINCIPAL_AXES,
      mp.direction[0].x, mp.direction[0].y, mp.direction[0].z,
      mp.direction[1].x, mp.direction[1].y, mp.direction[1].z,
      mp.direction[2].x, mp.direction[2].y, mp.direction[2].z );
    _tcscat( s0, s1 );
  }
  else {
    if ( mp.direction[0].Length()>Math::lengthEpsilon )
      _sntprintf( s0, 512, RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA_AND_AXIS_1,
        mp.general[0], mp.general[1], mp.general[2],
        mp.direction[0].x, mp.direction[0].y, mp.direction[0].z );
    else
      if ( mp.direction[2].Length()>Math::lengthEpsilon )
        _sntprintf( s0, 512, RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA_AND_AXIS_3,
          mp.general[0], mp.general[1], mp.general[2],
          mp.direction[2].x, mp.direction[2].y, mp.direction[2].z );
      else
        _sntprintf( s0, 512, RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA_AND_ANY_AXIS,
          mp.general[0], mp.general[1], mp.general[2] );
  }
  _tcscat( s, s0 );
  {
    TCHAR s2[128];
    _sntprintf( s2, 128, TOLERANCE_VALUE  _T( "\n\n\r-------------" ), deviateAngle );
    _tcscat( s, s2 );
  }
  {
    TCHAR s2[128];
    _sntprintf( s2, 128, _T( "\n\n\r") THREAD_TEST_SERIAL RT_TIME_VALUE, first );
    _tcscat( s, s2 );
  }
  {
    TCHAR s2[128];
    _sntprintf( s2, 128, _T( "\n\n\r") THREAD_TEST_MULTITHREAD RT_TIME_VALUE, second );
    _tcscat( s, s2 );
  }

  MessageBoxEx( s, IDS_RESULTS );
}

void MassInertiaCompare()
{
  MbItem * obj = nullptr;
  Style ns = TestVariables::SOLID_Style;
  double d = 1.0; //2.5;
  double deviateAngle = Math::deviateSag; // Параметр управления точностью расчёта (0.35 - 0.01)

  GetDouble( IDS_ENTER_ASSEMBLYDENSITY, d );
  GetDouble( IDS_ENTER_ANG_TOLERANCE, deviateAngle );
  RPArray<MbItem> items( 3, 1 );         //-V112
  SArray<MbMatrix3D> matrs( 3, 1 );        //-V112
  SArray<double> densities( 3, 1 );        //-V112
  MbMatrix3D matr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {
    TestVariables::viewManager->ToggleObject( obj );

    MbItem * pObj = nullptr;

    items.Add( obj );
    matrs.Add( matr );
    densities.Add( d );
    while ( GetGObj( IDS_SELECT_SOLID, st_Solid, &pObj ) ) {
      TestVariables::viewManager->ToggleObject( obj );
      TestVariables::viewManager->ToggleObject( pObj );
      items.Add( pObj );
      matrs.Add( matr );
      densities.Add( d );
      obj = pObj;
    }
    if ( pObj != nullptr )
      TestVariables::viewManager->ToggleObject( pObj );
    else
      TestVariables::viewManager->ToggleObject( obj );
  }
  else {
    int i = 1;
    if ( GetInt( IDS_ENTER_QUANTITY, i ) && ( i > 0 ) ) {
      TestVariables::viewManager->GetObjects( st_Solid, items, matrs ); // Дать все объекты указанного типа.
      densities.Reserve( items.Count() );
      for ( size_t j = 0, jCount = items.Count(); j < jCount; j++ )
        densities.Add( d );
    }
  }
  if ( ( items.Count() > 0 ) && ( items.Count() == matrs.Count() ) && ( items.Count() == densities.Count() ) ) {
    PArray<InertiaProperties> mpSolids( 0, 1, true ); // Имеющиеся характеристики тел (или nullptr)
    InertiaProperties mp1, mp2;

    // Используем предустановку свойств сборки
    ptrdiff_t sCount = items.Count();
    RPArray<AssemblyMIAttire> assembleAttire( 0, 1 );
    RPArray<SolidMIAttire>    solidsAttire( sCount, 1 );
    bool bSet = true;
    double m2 = 0.0;

    for ( ptrdiff_t i = 0; i < sCount; i++ ) {
      MbItem * item = items[i];
      if ( item->IsA() == st_Solid ) {
        MbSolid * sol = ( MbSolid * )item;
        matr = matrs[i];
        double dens = densities[i];
        SolidMIAttire * sAttire = new SolidMIAttire( *sol, dens, matr );

        if ( bSet ) {
          double m1 = 0.0;
          if ( m1 > METRIC_REGION ) {
            InertiaProperties prop;
            prop.mass = std_max( 0.0, m1 ); // Масса
            sAttire->SetProperties( prop );
            m2 += m1;
          }
          else
            bSet = false;
        }

        solidsAttire.Add( sAttire );
      }
    }
    MbMatrix3D matrA;
    AssemblyMIAttire assemblyMI( assembleAttire, solidsAttire, matrA );

    if ( m2 > METRIC_NEAR ) {
      InertiaProperties prop;
      prop.mass = std_max( 0.0, m2 ); // Масса
      assemblyMI.SetProperties( prop );
    }

    MbeMultithreadedMode mode = Math::MultithreadedMode();

    Math::SetMultithreadedMode( mtm_Off );
    clock_t start_time = clock();

    SetWaitCursor( true );
    ::MassInertiaProperties( assemblyMI, deviateAngle, mp1, nullptr );
    SetWaitCursor( false );

    clock_t end_time = clock();
    clock_t first = end_time - start_time;

    Math::SetMultithreadedMode( mtm_Max );
    start_time = clock();

    SetWaitCursor( true );
    ::MassInertiaProperties( assemblyMI, deviateAngle, mp2, nullptr );
    SetWaitCursor( false );

    end_time = clock();
    clock_t second = end_time - start_time;

    Math::SetMultithreadedMode( mode );

    if ( CompareInertiaProperties(mp1, mp2) )
      OutputMassInertia( mp1, deviateAngle, first, second );
    else {
      const size_t bufLen = 1024;
      TCHAR buf[bufLen];
      size_t len = 0;
      len += ( size_t )_sntprintf( buf + len, bufLen - len, _T( "---\nWARNING: Mass-inertial properties do not match!" ) );
      MessageBoxEx( buf, IDS_RESULTS );
    }
  }

}