#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <surf_plane.h>
#include <surf_spline_surface.h>
#include <surf_curve_bounded_surface.h>
#include <cur_nurbs.h>
#include <solid.h>
#include <space_instance.h>
#include <action_curve.h>
#include <action_surface.h>
#include <last.h>

//#if defined(C3D_STANDARD_CXX_11_PARTIAL) //&& defined C3D_NATIVE_LOCK // Тестирование распараллеливания с использованием любых API (не OpenMP)
  #define MULTITHREADING_WITH_NON_OMP
  #include <thread>
//#endif

using namespace c3d;


//------------------------------------------------------------------------------
// Преобразовать поверхность в NURBS.
// Translate surface to NURBS.
// ---
bool TranslateSurfaceToNurbs()
{
  bool bRes = false;
  MbItem * surfObj = nullptr;
  MbItem * faceObj = nullptr;
  MbTopItem * faceItem = nullptr;

  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &surfObj ) ) {
    TestVariables::viewManager->ToggleObject( surfObj );

    const MbSpaceItem * sItem = ((MbSpaceInstance *)surfObj)->GetSpaceItem(); // Дать трёхмерный геометрический объект. // Give a three-dimensional geometric object.
    if ( (sItem != nullptr) && (sItem->Family() == st_Surface) ) {
      const MbSurface * surf = (const MbSurface *)sItem; // Дать поверхность.  // Give a surface.
      SetWaitCursor( true );
      MbSurface * nurbsSurf = nullptr;

      ptrdiff_t uDegree = NURBS_DEGREE, vDegree = NURBS_DEGREE;
      if ( GetInt ( IDS_ENTER_SPLINE_ORDER, uDegree ) && GetInt ( IDS_ENTER_SPLINE_ORDER, vDegree ) ) {
        ptrdiff_t uCount = 0, vCount = 0;
        GetInt ( IDS_ENTER_POINT_NUMBER, uCount );
        GetInt ( IDS_ENTER_POINT_NUMBER, vCount );
        uCount = std_max( uCount, (ptrdiff_t)0 );
        vCount = std_max( vCount, (ptrdiff_t)0 );
        double u1 = 0.0, u2 = 1.0, v1 = 0.0, v2 = 1.0, accuracy = 0.001;
        GetDouble ( IDS_ENTER_PAR_ON_CURVE, u1 );
        GetDouble ( IDS_ENTER_PAR_ON_CURVE, u2 );
        GetDouble ( IDS_ENTER_PAR_ON_CURVE, v1 );
        GetDouble ( IDS_ENTER_PAR_ON_CURVE, v2 );
        GetDouble ( IDS_ENTER_APPROX_ACCURACY, accuracy );
        int uApprox = 1; // Использовать аппроксимацию при построении кривой. // Use approximation when build the curve.
        int vApprox = 1;
        GetInt ( IDS_ENTER_U_USE_APPROX, uApprox );
        GetInt ( IDS_ENTER_V_USE_APPROX, vApprox );
        //double umin = surf->GetUMin();
        //double umax = surf->GetUMax();
        //double vmin = surf->GetVMin();
        //double vmax = surf->GetVMax();
        MbNurbsParameters uParam( uDegree, uCount, u1, u2, uApprox == 1 );
        MbNurbsParameters vParam( vDegree, vCount, v1, v2, vApprox == 1 );
        uParam.SetPrecision( accuracy );
        vParam.SetPrecision( accuracy );
        //nurbsSurf = surf->NurbsSurface ( uParam, vParam );
        SurfaceSPtr nurbsSurfSptr;
        ::NurbsSurface( *surf, uParam, vParam, nurbsSurfSptr );
        nurbsSurf = nurbsSurfSptr.detach();
      }
      else { // Степень не введена - старый вариант вызываем. // Degree not entered - the old version caused.
        //KYA K12 nurbsSurf = surf->Nurbs( surf->GetUMin(), surf->GetUMax(), surf->GetVMin(), surf->GetVMax() );
        ::NurbsSurface( *surf, Math::DefaultMathVersion(), nurbsSurf );
      }
      SetWaitCursor( false );
      if( nurbsSurf != nullptr ) {
        TestVariables::viewManager->AddObject( TestVariables::BOOLEAN_Style, nurbsSurf );
        bRes = true;
      }
      else
      {  
        MessageBoxEx( RT_CONVERT_TO_NURBS_FAILED, _T("") );
      }  

    }
    TestVariables::viewManager->ToggleObject( surfObj );
  }
  else if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &faceObj, &faceItem ) ) {
    TestVariables::viewManager->ToggleObject( faceObj );
    MbFace * face = (MbFace *)faceItem;

    const MbSurface & faceSurf = face->GetSurface();
    //const MbSurface & baseSurf = faceSurf.GetSurface();

    SetWaitCursor( true );

    MbSurface * nurbsSurf = nullptr;
    //nurbsSurf = (MbSurface *)&baseSurf.Duplicate();
    //::NurbsSurface( baseSurf, Math::DefaultVersion(), nurbsSurf );
    ::NurbsSurface( faceSurf, Math::DefaultMathVersion(), nurbsSurf );
    MbSurface * nurbsBndSurf = nullptr;

    if ( nurbsSurf != nullptr ) {
      RPArray<MbContour> contours( 0, 1 );

      for ( size_t iLoop = 0, cntLoops = face->GetLoopsCount(); iLoop < cntLoops; iLoop++ ) {
        MbLoop * loop = face->GetLoop( iLoop );
        if ( loop != nullptr ) {
          MbContour * contour = new MbContour;

          for ( size_t iEdge = 0, cntEdges = loop->GetEdgesCount(); iEdge < cntEdges && (contour != nullptr); iEdge++ ) {
            MbCurve * curve = nullptr;
            ::FaceBoundSegment( *face, iLoop, iEdge, *nurbsSurf, Math::DefaultMathVersion(), curve );

            if ( curve != nullptr ) {
              MbNurbs * nurbs = curve->NurbsCurve();
              if ( nurbs != nullptr ) {
                if ( contour != nullptr && !contour->AddCurveWithRuledCheck( *nurbs, Math::LengthEps ) )
                  ::DeleteItem( contour );
              }
              ::DeleteItem( nurbs );
              ::DeleteItem( curve );
            }
          }

          if ( contour != nullptr )
            contours.Add( contour );
          else {
            ::DeleteItems( contours );
          }
        }
      }

      if ( contours.Count() > 0 ) {
        nurbsBndSurf = new MbCurveBoundedSurface( nurbsSurf->SetSurface(), contours, true );
      }
    }

    SetWaitCursor( false );

    if ( nurbsBndSurf != nullptr ) {
      TestVariables::viewManager->AddObject( TestVariables::BOOLEAN_Style, nurbsBndSurf );
      bRes = true;
    }
    else if ( nurbsSurf != nullptr ) {
      TestVariables::viewManager->AddObject( TestVariables::BOOLEAN_Style, nurbsSurf );
      MessageBoxEx( RT_EDGE_PROJECT_ERROR, _T("") );
    }
    else {
      MessageBoxEx( RT_CONVERT_TO_NURBS_FAILED, _T("") );
    }

    ::DeleteItem( nurbsBndSurf );
    ::DeleteItem( nurbsSurf );

    TestVariables::viewManager->ToggleObject( faceObj );
  }

  return bRes;
}


//------------------------------------------------------------------------------
// Провести NURBS поверхность через заданную точку.
// Conduct a NURBS surface through a given point.
// ---
bool GoSurfaceThroughPoint()
{
  MbItem * surface = nullptr;

  if ( GetGObj( IDS_SELECT_SURFACE, st_SplineSurface, &surface ) ) {
    TestVariables::viewManager->ToggleObject(surface);
    MessageBoxEx( RT_PASSAGE_OF_POINTS_NOT_EXECUTED, _T("") );
    TestVariables::viewManager->ToggleObject(surface);
    TestVariables::viewManager->RefreshObjectModel( surface );
    return true;

  }

  return false;
}


//------------------------------------------------------------------------------
// Коснуться NURBS поверхностью заданной плоскости.
// Touch the NURBS surface to the specified plane.
// ---
bool GoSurfaceThroughPlane()
{
  MbItem * surface( nullptr );
  MbItem * nearest( nullptr );

  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &surface ) ) {
    TestVariables::viewManager->ToggleObject(surface);

    if ( GetGObj( IDS_SELECT_PLANE, st_Plane, &nearest ) ) {
      TestVariables::viewManager->ToggleObject(nearest);

      MbSurface * srf = (MbSurface *)((MbSpaceInstance *)surface)->GetSpaceItem();
      MbPlane * plane = (MbPlane *)((MbSpaceInstance *)nearest)->GetSpaceItem();
      MbCartPoint3D p;
      MbVector3D nor = plane->GetPlacement().GetAxisZ();

      SArray<MbCartPoint> uv;
      srf->GetIsoclinal( nor, uv );
      ptrdiff_t count = uv.Count();

      if( count > 0 ) { // Определяем ближайшую точку.  // Define the nearest point.
        double u, v, d, dmin = MB_MAXDOUBLE;
        MbCartPoint3D pmin, pnt;
        for ( ptrdiff_t i = 0; i < count; i++ ) {
          srf->PointOn(uv[i].x, uv[i].y, pnt);
          d = plane->DistanceToPoint( pnt );
          if ( d < dmin ) {
            pmin = pnt;
            dmin = d;
            u=uv[i].x;
            v=uv[i].y;
          }
        }
        double x, y;
        x = y = UNDEFINED_DBL;
        plane->NearPointProjection( pmin, x, y, false );
        plane->PointOn( x, y, pnt );

        MbVector3D deru, derv;
        srf->DeriveU(u,v,deru);
        srf->DeriveV(u,v,derv);
/*
        if ( srf->IsA()==st_SplineSurface && ((MbSplineSurface *)srf)->GoThroughPointWithDerive( u, v, pnt, deru, derv ) ) {
          TestVariables::viewManager->ToggleObject(surface);
          TestVariables::viewManager->ToggleObject(nearest);
          TestVariables::viewManager->RefreshObjectModel( surface );
          return true;
        }
        else
*/      
        MessageBoxEx( RT_TOUCHING_PLANE_FAILED, _T("") );
      }
      else
      {  
        MessageBoxEx( RT_ISOCLINES_NO_SURFACAE, _T("") );
      }  

      TestVariables::viewManager->ToggleObject(nearest);
    }

    TestVariables::viewManager->ToggleObject(surface);
  }

  return false;
}


//------------------------------------------------------------------------------
// Удалить поверхность, оставив базовые объекты.
// Remove the surface, leaving the base objects.
// ---
bool RemoveSurfaceBasis()
{
  MbItem * nearest( nullptr );

  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &nearest ) ) {
    TestVariables::viewManager->ToggleObject(nearest);

    RPArray<MbSpaceItem> s( 0, 1 );
    ((MbSurface *)((MbSpaceInstance *)nearest)->GetSpaceItem())->GetBasisItems( s );
    int width = nearest->GetWidth();
    uint32 color = nearest->GetColor();
    ::AddRefItems( s );

    ptrdiff_t n = s.Count();
    TestVariables::viewManager->ToggleObject(nearest);

    if ( n > 0 ) {
      TestVariables::viewManager->DeleteObject( nearest ); // Удаление элемента.  // Delete element.

      Style st( width, color );
      bool showItem = false; // SD#739011 = true;
      for ( ptrdiff_t i = 0; i < n; i++ )
        TestVariables::viewManager->AddObject( st, &(s[i]->Duplicate()), showItem );
      if ( !showItem )
        TestVariables::viewManager->ShowModel();
    }

    ::ReleaseItems( s );
    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Переопределение базисных точек NURBS из Close в Open при замкнутом узловом векторе.
// Overriding NURBS basis points from Close to Open with a closed nodal vector.
// ---
bool ConvertNURBSSurfaceIntoOpen() {

  MbItem * obj( nullptr );

  if ( GetGObj( IDS_SELECT_CURVE, st_SplineSurface, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    const MbSpaceItem * spaceItem = ((MbSpaceInstance *)obj)->GetSpaceItem();
    if ( spaceItem != nullptr && spaceItem->IsA() == st_SplineSurface) {
      MbSplineSurface * nurbs = (MbSplineSurface *)spaceItem;

      if ( nurbs->UnClampedU( false ) || nurbs->UnClampedV( false ) ) { // Переопределение базисных точек.  // Overriding basis points.
        TestVariables::viewManager->ToggleObject(obj);
        TestVariables::viewManager->RefreshObjectModel( obj );
        return true;
      }
    }

    TestVariables::viewManager->ToggleObject(obj);
  }

  return false;
}


//------------------------------------------------------------------------------
// Переопределение базисных точек NURBS из Open в Close при замкнутом узловом векторе.
// Overriding NURBS basis points from Open to Close with a closed nodal vector.
// ---
bool ConvertNURBSSurfaceIntoClose()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_CURVE, st_SplineSurface, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    const MbSpaceItem * spaceItem = ((MbSpaceInstance *)obj)->GetSpaceItem();
    if ( spaceItem != nullptr && spaceItem->IsA() == st_SplineSurface ) {
      MbSplineSurface * nurbs = (MbSplineSurface *)spaceItem;

      if ( nurbs->UnClampedU( true ) || nurbs->UnClampedV( true ) ) { // Переопределение базисных точек.  // Overriding basis points.
        TestVariables::viewManager->ToggleObject(obj);
        TestVariables::viewManager->RefreshObjectModel( obj );
        return true;
      }
    }

    TestVariables::viewManager->ToggleObject(obj);
  }

  return false;
}


//------------------------------------------------------------------------------
//
// ---
static void CalculatePointsOnShell ( MbFaceShell * shell, MbControlData3D * pnts, std::vector<size_t> * faceIndices, size_t startInd, size_t endInd )
{
  for ( size_t i = startInd; i < endInd; i++ ) {
    MbCartPoint3D pnt;
    double u, v;
    pnts->GetPoint( i, pnt );
    size_t faceIndex = SYS_MAX_T;
    shell->NearPointProjection( pnt, faceIndex, u, v ); // Ближайшая проекция точки на поверхность.  // Nearest projection of the point on the surface.
    shell->PointOn( faceIndex, u, v, pnt ); 
    pnts->SetPoint( i, pnt );
    (*faceIndices)[i] = faceIndex;
  }
}

//------------------------------------------------------------------------------
//
// ---
static void CalculatePointsOnSurface ( const MbSurface * surf1, MbControlData3D * pnts, size_t startInd, size_t endInd )
{
  for ( size_t i = startInd; i < endInd; i++ ) {
    MbCartPoint3D pnt;
    double u, v;
    pnts->GetPoint( i, pnt );
    surf1->NearPointProjection( pnt, u, v, true, nullptr ); // Ближайшая проекция точки на поверхность.  // Nearest projection of the point on the surface.
    surf1->_PointOn( u, v, pnt ); 
    pnts->SetPoint( i, pnt );
  }
}

//------------------------------------------------------------------------------
// Спроецировать базовые точки объекта на поверхность или тело.
// Project base points of the object on a surface or body.
// ---
void SurfaceBasisPointProjection( bool multithreading )
{
  MbItem * object0;
  MbItem * object1;

  // \ru Для теста многопоточности. \en For multithreading test.
  bool res = false, valid = false;
  MbAccurateTimer ac;
  const size_t bufLen = 1024;
  TCHAR buf[bufLen];
  size_t len = 0;
  // \ru Текущий режим многопоточности.
  // \en Current multithreaded mode.
  MbeMultithreadedMode mode =  Math::MultithreadedMode();

  if ( GetGObj( IDS_SELECT_SURFACE, st_SpaceItem, &object0 ) ) {
    TestVariables::viewManager->ToggleObject(object0);

    MbControlData3D pnts, pnts2;
    MbCartPoint3D pnt;

    object0->GetBasisPoints( pnts ); // Дать базовые точки.  //Give base points.
    if ( multithreading ) {
      object0->GetBasisPoints ( pnts2 ); // \ru Базовые точки для теста многопоточности.  \en Base points for multithreading test.
      PRECONDITION ( pnts.SetSharePoints() == pnts2.SetSharePoints() && pnts.SetTotalPoints() == pnts2.SetTotalPoints() );
    }

    size_t n = pnts.Count();
    TestVariables::viewManager->ToggleObject(object0);

    if ( n > 0 ) {
      if ( GetGObj( IDS_SELECT_GEOM_OBJECT, st_SpaceItem, &object1 ) ) {
        TestVariables::viewManager->ToggleObject(object1);

        if ( object1->IsA() == st_Solid ) {
          MbSolid * solid = (MbSolid *)object1;
          MbFaceShell * shell = solid->GetShell();
          if ( shell != nullptr ) {
            len += (size_t)_sntprintf( buf+len, bufLen-len, THREAD_PROJECTION_POINTS );
            len += (size_t)_sntprintf( buf+len, bufLen-len, THREAD_PROJECTION_FORMAT1, LoUint32(pnts.Count()) );
            res = true;
            valid = true;
            std::vector<size_t> faceIndices;
            faceIndices.resize ( n, SYS_MAX_T );
            // \ru Для теста многопоточности: установить стандартный многопоточный режим.
            // \en For multithreading test: set standard multithreaded mode.
            if ( multithreading )
              Math::SetMultithreadedMode( mtm_Standard );

            SetWaitCursor ( true );
            ac.Begin();
            CalculatePointsOnShell ( shell, &pnts, &faceIndices, 0, n );
            ac.End();
            std::set<size_t> indSet;
            for ( size_t i = 0; i < faceIndices.size(); ++i )
              if ( faceIndices[i] != SYS_MAX_T)
                indSet.insert ( faceIndices[i] );
            len += (size_t)_sntprintf( buf+len, bufLen-len, THREAD_PROJECTION_FACES );
            len += (size_t)_sntprintf( buf+len, bufLen-len, THREAD_PROJECTION_FORMAT2, LoUint32(indSet.size()) );

            len += (size_t)_sntprintf( buf+len, bufLen-len, THREAD_TEST_SERIAL );
            len += (size_t)_sntprintf( buf+len, bufLen-len, RT_TIME_SEC, ac.GetLast() );

            if ( multithreading ) {
              // \ru Для теста многопоточности: установить многопоточный доступ к объектам.
              // \en For multithreading test: set multithreaded access to items.
              Math::SetMultithreadedMode( mtm_Items );
              size_t num_threads = 8;
              while ( n < num_threads && num_threads > 2 ){
                num_threads /= 2;
              }

              ac.Begin();

#ifdef MULTITHREADING_WITH_NON_OMP
              if ( n > 0 ) {
                size_t step = n / num_threads;
                std::vector<std::thread> threads;
                threads.reserve(num_threads);
                size_t i = 0;
                for ( ; i < num_threads - 1; ++i )
                  threads.push_back ( std::thread ( CalculatePointsOnShell, shell, &pnts2, &faceIndices, step*i, step*(i+1) ) );
                threads.push_back ( std::thread ( CalculatePointsOnShell, shell, &pnts2, &faceIndices, step*i, n ) );
                for ( i = 0; i < num_threads; ++i )
                  threads[i].join();
              }
#else
              
              ENTER_PARALLEL_FORCED;
#pragma omp parallel for shared( n, pnts2, shell ) private ( pnt )
              for ( ptrdiff_t i = 0; i < n; i++ ) {
                pnts2.GetPoint( i, pnt );
                size_t faceIndex = SYS_MAX_T;
                double u, v;
                shell->NearPointProjection( pnt, faceIndex, u, v ); // Ближайшая проекция точки на поверхность.  // Nearest projection of the point on the surface.
                shell->PointOn( faceIndex, u, v, pnt ); 
                pnts2.SetPoint( i, pnt );
                faceIndices[i] = faceIndex;
              }
              EXIT_PARALLEL_FORCED;
#endif
              ac.End();
              len += (size_t)_sntprintf( buf+len, bufLen-len, THREAD_TEST_MULTITHREAD );
              len += (size_t)_sntprintf( buf+len, bufLen-len, RT_TIME_SEC, ac.GetLast() );

              // \ru Проверка данных. \en Data check.
              SArray<MbCartPoint3D>& total = pnts.SetTotalPoints();
              SArray<MbCartPoint3D>& total2 = pnts2.SetTotalPoints();
              SArray<MbCartPoint3D>& share = pnts.SetSharePoints();
              SArray<MbCartPoint3D>& share2 = pnts2.SetSharePoints();
              if ( total.Count() != total2.Count() ||
                   share.Count() != share2.Count() )
                valid = false;
              if ( valid ) {
                for ( size_t i = 0; i < total.Count(); ++i ) {
                  if ( !total2.IsExist ( total[i] ) ) {
                    valid = false;
                    break;
                  }
                }
              }
              if ( valid ) {
                for ( size_t i = 0; i < share.Count(); ++i ) {
                  if ( !share2.IsExist ( share[i] ) ) {
                    valid = false;
                    break;
                  }
                }
              }
            }
            SetWaitCursor ( false );
          }
        }
        if ( object1->IsA() == st_SpaceInstance ) {
          const MbSpaceItem * gitem1 = ((MbSpaceInstance *)object1)->GetSpaceItem();
          if ( gitem1 != nullptr && (gitem1->Family() == st_Surface) ) {
            const MbSurface * surf1 = (const MbSurface*)gitem1;
            len += (size_t)_sntprintf( buf+len, bufLen-len, THREAD_PROJECTION_POINTS );
            len += (size_t)_sntprintf( buf+len, bufLen-len, THREAD_PROJECTION_FORMAT1, LoUint32(pnts.Count()) );
            res = true;
            valid = true;
            // \ru Для теста многопоточности: установить стандартный многопоточный режим.
            // \en For multithreading test: set standard multithreaded mode.
            if ( multithreading )
              Math::SetMultithreadedMode( mtm_Standard );
            SetWaitCursor ( true );
            ac.Begin();
            CalculatePointsOnSurface ( surf1, &pnts, 0, n );
            ac.End();
            len += (size_t)_sntprintf( buf+len, bufLen-len, THREAD_PROJECTION_SURFACES );
            len += (size_t)_sntprintf( buf+len, bufLen-len, THREAD_PROJECTION_FORMAT2, 1 );

            len += (size_t)_sntprintf( buf+len, bufLen-len, THREAD_TEST_SERIAL );
            len += (size_t)_sntprintf( buf+len, bufLen-len, RT_TIME_SEC, ac.GetLast() );

            if ( multithreading ) {
              // \ru Установить многопоточный доступ к объектам.
              // \en Set multithreaded access to items.
              Math::SetMultithreadedMode( mtm_Items );
              size_t num_threads = 8;
              while ( n < num_threads && num_threads > 2 ){
                num_threads /= 2;
              }
              
              ac.Begin();

#ifdef MULTITHREADING_WITH_NON_OMP
              if ( n > 0 ) {
                size_t step = n / num_threads;
                std::vector<std::thread> threads;
                threads.reserve(num_threads);
                size_t i = 0;
                for ( ; i < num_threads - 1; ++i )
                  threads.push_back ( std::thread ( CalculatePointsOnSurface, surf1, &pnts2, step*i, step*(i+1) ) );
                threads.push_back ( std::thread ( CalculatePointsOnSurface, surf1, &pnts2, step*i, n ) );
                for ( i = 0; i < num_threads; ++i )
                  threads[i].join();
              }
#else

              ENTER_PARALLEL_FORCED;
#pragma omp parallel for shared( n, pnts2, surf1 ) private ( pnt )
              for ( ptrdiff_t i = 0; i < n; i++ ) {
                pnts2.GetPoint( i, pnt );
                double u, v;
                surf1->NearPointProjection( pnt, u, v, true, nullptr ); // Ближайшая проекция точки на поверхность.  // Nearest projection of the point on the surface.
                surf1->_PointOn( u, v, pnt ); 
                pnts2.SetPoint( i, pnt );
              }
              EXIT_PARALLEL_FORCED;
#endif
              ac.End();

              // \ru Проверка данных. \en Data check.
              SArray<MbCartPoint3D> & total = pnts.SetTotalPoints();
              SArray<MbCartPoint3D> & total2 = pnts2.SetTotalPoints();
              SArray<MbCartPoint3D> & share = pnts.SetSharePoints();
              SArray<MbCartPoint3D> & share2 = pnts2.SetSharePoints();

              if ( total.Count() != total2.Count() ||
                   share.Count() != share2.Count() )
                valid = false;
              if ( valid ) {
                for ( size_t i = 0; i < total.Count(); ++i ) {
                  if ( !total2.IsExist ( total[i] ) ) {
                    valid = false;
                    break;
                  }
                }
              }
              if ( valid ) {
                for ( size_t i = 0; i < share.Count(); ++i ) {
                  if ( !share2.IsExist ( share[i] ) ) {
                    valid = false;
                    break;
                  }
                }
              }
              len += (size_t)_sntprintf( buf+len, bufLen-len, THREAD_TEST_MULTITHREAD );
              len += (size_t)_sntprintf( buf+len, bufLen-len, RT_TIME_SEC, ac.GetLast() );
            }
            SetWaitCursor ( false );
          }
        }
        TestVariables::viewManager->RefreshObjectModel( object0 );
        TestVariables::viewManager->ToggleObject(object1);
      }
    }
    TestVariables::viewManager->ToggleObject(object0);
  }

  if ( res && multithreading) {
    // \ru Показать результаты для теста многопоточности. \en Report results for multithreading test.
    if ( valid )
      ::MessageBoxEx ( buf, THREAD_PROJECTION_TEST_TITLE );
    else
      ::MessageBoxEx ( THREAD_PROJECTION_TEST_FAILED, THREAD_PROJECTION_TEST_TITLE );

    // \ru Востановить уровень многопоточности
    // \en Restore level of multithreading
    Math::SetMultithreadedMode( mode );
  }
}

