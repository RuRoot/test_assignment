////////////////////////////////////////////////////////////////////////////////
//
// Примеры создания пространственных точек.
// Examples of creation of spatial points.
//
////////////////////////////////////////////////////////////////////////////////

#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <test_draw.h>
#include <cur_hermit.h>
#include <cur_line_segment3d.h>
#include <cur_contour3d.h>
#include <cur_surface_intersection.h>
#include <point_frame.h>
#include <action_surface_curve.h>
#include <action_point.h>
#include <action_curve.h>
#include <action_curve3d.h>
#include <action_wireframe.h>
#include <surface.h>
#include <surf_plane.h>
#include <solid.h>
#include <point3d.h>
#include <space_instance.h>
#include <plane_instance.h>
#include <list>
#include <last.h>

using namespace c3d;
using namespace TestVariables;


//------------------------------------------------------------------------------
// Точка
// Create a point
// ---
void CreateCartPoints()
{
  MbCartPoint3D pnt;
  SArray<MbCartPoint3D> pList;

  while ( GetPoint( IDS_SELECT_POINT, pnt ) ) {
//#ifdef _DRAWGI
    drawImp->DrawPoint( &pnt, TRGB_LIGHTRED );
//#endif
    pList.Add( pnt );
  }
  size_t pCount = pList.Count();
  if ( pCount > 0 ) {
    if ( pCount == 1 ) 
      viewManager->AddObject( POINT_Style, new MbPoint3D(pList[0]) );
    else
      viewManager->AddObject( POINT_Style, new MbPointFrame(pList) );
  }
}


//------------------------------------------------------------------------------
// Проекция точки на кривую
// Projections of a point on a curve
// ---
void CreateCurvePointProjection()
{
  MbItem * nearest = nullptr;
  MbCartPoint3D point;

  if ( GetPoint( IDS_SELECT_POINT, point ) ) {
    if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest ) && (nearest->IsA() == st_SpaceInstance) ) {
      viewManager->ToggleObject(nearest);
      MbCurve3D * curve = (MbCurve3D *)(((MbSpaceInstance *)nearest)->GetSpaceItem());
      SArray<double> tProj; // Множество параметров. Set of parameters.
      MbCartPoint3D p;

//      curve->PointProjection( point, tProj, false );

      viewManager->ToggleObject(nearest);
      if (tProj.Count()==0) {
        double t;
        curve->NearPointProjection( point, t, true );
        curve->PointOn( t, p );
        viewManager->AddObject( POINT_Style, new MbPointFrame(p) );
      }
      else {
        for ( ptrdiff_t i = 0; i < (ptrdiff_t)tProj.Count(); i++ ) {
          curve->PointOn( tProj[i], p );
          viewManager->AddObject( POINT_Style, new MbPointFrame(p) );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Ближайшая проекция точки на кривую
// The nearest projection of a point on a curve
// ---
void CreateCurveNearPointProjection() 
{
  MbItem * nearest = nullptr;
  MbCartPoint3D point;

  if ( GetPoint( IDS_SELECT_POINT, point ) ) {
    if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest ) ) {
      viewManager->ToggleObject(nearest);
      SPtr<MbCurve3D> curve( GetCurve3D( nearest, false ) );

      double tProj; // Параметр. Parameter.
      MbCartPoint3D p;

      int iExt = 0;
      GetInt( IDS_ENTER_EXT_FLAG, iExt );
      bool bExt = (iExt > 0) ? true : false;

      MbRect1D * tRange = nullptr;

      int iUseRange = 0;
      if ( GetInt( IDS_ENTER_USE_RANGE, iUseRange ) ) {
        if ( iUseRange > 0 ) {
          double tmin = curve->GetTMin();
          double tmax = curve->GetTMax();
          double t1 = tmin;
          double t2 = tmax;
          if ( GetDouble( IDS_ENTER_INITIAL_PRM, t1 ) &&
               GetDouble( IDS_ENTER_FINAL_PRM,   t2  ) ) {
            if ( t2 < t1 ) {
              double t = t1;
              t1 = t2;
              t2 = t;
              MessageBoxEx( RT_CHECK_PERMUTATION_PARAMETERS, IDS_TITLE_MSG );
            }
            if ( ::fabs(t1 - t2) < PARAM_REGION ) {
              t1 = tmin;
              t2 = tmax;
              MessageBoxEx( RT_SMALL_RANGE_CURVE, IDS_TITLE_MSG );
            }
          }
          tRange = new MbRect1D( t1, t2 );
        }
      }

      bool isNormProj = curve->NearPointProjection( point, tProj, bExt, tRange );

      if ( isNormProj )
        MessageBoxEx( RT_FOUND_NORMAL_PROJECTION, IDS_TITLE_MSG );
      else
        MessageBoxEx( RT_NOT_FOUND_NORMAL_PROJECTION, IDS_TITLE_MSG );

      if ( tRange != nullptr )
        delete tRange;

      viewManager->ToggleObject(nearest);

      curve->_PointOn( tProj, p );

      viewManager->AddObject( POINT_Style, new MbPointFrame(p) );

      viewManager->AddObject( LINE_Style, new MbLineSegment3D( point, p ) );

      double dist = point.DistanceToPoint( p );
      {
        TCHAR spaceMsg[512];
        size_t len = 0;
        len += (size_t)_sntprintf( spaceMsg + len, 512 - len, RT_DISTANCE_BETWEEN_POINTS, dist );
        MessageBoxEx( spaceMsg, IDS_INFORMATION );
      }
    }
    else {
      MessageBoxEx( RT_CURVE_NOT_SELECTED, IDS_TITLE_MSG );
    }
  }
}


//------------------------------------------------------------------------------
// Создать особые точки функции кривизны кривой.
// Create special points of the curvature function of the curve.
//------------------------------------------------------------------------------
void CreateCurveCurvaturePoints()
{
  MbItem * nearest = nullptr;
  MbCartPoint3D point;

  if ( GetGObj(IDS_SELECT_CURVE, st_Curve3D, &nearest) ) {
    viewManager->ToggleObject( nearest );
    SPtr<MbCurve3D> curve( GetCurve3D(nearest, false) );

    std::vector<c3d::DoublePair> points;
    curve->GetCurvatureSpecialPoints( points );

    viewManager->ToggleObject( nearest );
    SPtr<MbPointFrame> drawPoints1( new MbPointFrame() );
    SPtr<MbPointFrame> drawPoints2( new MbPointFrame() );
    SPtr<MbPointFrame> drawPoints3( new MbPointFrame() );
    SPtr<MbPointFrame> drawPoints4( new MbPointFrame() );
    SPtr<MbPointFrame> drawPoints5( new MbPointFrame() );
    for ( size_t i = 0, icount = points.size(); i < icount; i++ ) {
      MbCartPoint3D p;
      curve->_PointOn( points[i].first, p );
      if ( points[i].second == MB_MAXDOUBLE ) // Точки разрыва кривизны выводим черным цветом.
        drawPoints1->AddVertex( p );
      else if ( points[i].second == MB_MINDOUBLE ) // Точки излома кривизны выводим светло-серым цветом.
        drawPoints2->AddVertex( p );
      else if ( points[i].second == 0.0 ) // Точки перегиба выводим светло-серым цветом.
        drawPoints3->AddVertex( p );
      else if( points[i].second > 0.0 )
        drawPoints4->AddVertex( p );
      else
        drawPoints5->AddVertex( p );
    }
    viewManager->AddObject( Style(2, RGB( 0, 0, 0)), drawPoints1 );
    viewManager->AddObject( Style(2, RGB( 200, 200, 200)), drawPoints2 );
    viewManager->AddObject( Style(2, RGB( 0, 255, 0)), drawPoints3 );
    viewManager->AddObject( Style(2, RGB( 255, 0, 0)), drawPoints4 );
    viewManager->AddObject( Style(2, RGB( 0, 0, 255)), drawPoints5 );
  }
}


//------------------------------------------------------------------------------
// Проекция точки на поверхность
// Projections of a point on a surface
// ---
void CreateSurfacePointProjection()
{
  MbItem * nearest = nullptr;
  MbCartPoint3D point;

  if ( GetPoint( IDS_SELECT_POINT, point ) ) {
    if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &nearest ) ) {
      viewManager->ToggleObject(nearest);
      MbSurface * surface = (MbSurface *)((MbSpaceInstance *)nearest)->GetSpaceItem();
      SArray<MbCartPoint> uv(2,2);
      MbCartPoint3D p;

//      surface->PointProjection( point, uv, false );

      viewManager->ToggleObject(nearest);

      if ( uv.Count() < 1 ) {
        double u, v;
        u = v = UNDEFINED_DBL;
        surface->NearPointProjection( point, u, v, true );
        surface->PointOn( u, v, p );
        viewManager->AddObject( POINT_Style, new MbPointFrame(p) );
      }
      else {
        MbVector3D surfaceNormal, dirVector;
        for ( ptrdiff_t i = 0; i < (ptrdiff_t)uv.Count(); i++ ) {
          surface->PointOn( uv[i].x, uv[i].y, p );

          surface->_Normal( uv[i].x, uv[i].y, surfaceNormal );
          dirVector.Init( point, p );
          PRECONDITION( dirVector.Colinear( surfaceNormal ) );

          viewManager->AddObject( POINT_Style, new MbPointFrame(p) );
          viewManager->AddObject( LINE_Style, new MbLineSegment3D( point, p ) );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание группы точек на поверхности
// Create a group of points on a surface
// ---
void CreateSurfacePoints()
{
  MbItem * obj = nullptr;
  MbSurface * surface = nullptr;

  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &obj ) )
    surface = (MbSurface *)((MbSpaceInstance *)obj)->GetSpaceItem();
  else {
    MbTopItem * tItem = nullptr;
    if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &tItem ) ) {
      if ( tItem->IsAFace() )
        surface = &((MbFace *)tItem)->SetSurface();
    }
  }

  if ( surface != nullptr ) {
    viewManager->ToggleObject( obj );

    MbeItemGridType gridType = igt_Rectangular;
    MbeStepType stepType = ist_ParamStep;

    bool aroundPoint = false;
    MbCartPoint3D spacePnt0;
    MbCartPoint paramPnt0;
    if ( GetPoint( IDS_SELECT_POINT, spacePnt0 ) ) {
      surface->NearPointProjection( spacePnt0, paramPnt0.x, paramPnt0.y, true );
//#ifdef _DRAWGI
      MbCartPoint3D drawPnt;
      surface->_PointOn( paramPnt0.x, paramPnt0.y, drawPnt );
      drawImp->DrawPoint( &drawPnt, TRGB_BLACK );
//#endif

      int iGridType = 0;
      if ( GetInt( IDS_ENTER_ITEM_GRID_TYPE, iGridType ) ) {
        switch( iGridType ) {
          case 0 : { gridType = igt_Rectangular; stepType = ist_ParamStep;  } break;
          case 1 : { gridType = igt_Concentric;  stepType = ist_MetricStep; } break;
          case 2 : { gridType = igt_Hexagonal;   stepType = ist_MetricStep; } break;
          default: { gridType = igt_Rectangular; stepType = ist_ParamStep;  } break;
        }
      }
      aroundPoint = true;
    }

    int iStepType = 0;
    switch( stepType ) {
      case ist_ParamStep     : iStepType = 0; break;
      case ist_SpaceStep     : iStepType = 1; break;
      case ist_DeviationStep : iStepType = 2; break;
      case ist_MetricStep    : iStepType = 3; break;
      default: iStepType = 0;
    }

    if ( GetInt( IDS_ENTER_ITEM_STEP_TYPE, iStepType ) ) {
      if ( aroundPoint ) {
        switch( iStepType ) {
          case 0  : stepType = ist_ParamStep;   break;
          case 3  : stepType = ist_MetricStep;  break;
        }
      }
      else {
        switch( iStepType ) {
          case 0  : stepType = ist_ParamStep;     break;
          case 1  : stepType = ist_SpaceStep;     break;
          case 2  : stepType = ist_DeviationStep; break;
        }
      }

      {
        MbResultType res = rt_Error;

        RPArray< SArray<MbLocPnt> > pntsData( 0, 1 );

        double step1 = 0.0;
        double step2 = 0.0;

        if ( aroundPoint ) { // Вокруг точки
          MbCube cube;
          surface->AddYourGabaritTo( cube );
          double dist = DELTA_MOD * cube.GetDiagonal();

          double r = surface->GetRadius();
          if ( r > METRIC_NEAR )
            dist = DELTA_MOD * r * M_PI;

          if ( gridType == igt_Rectangular ) {
            if ( stepType == ist_ParamStep ) {
              step1 = DELTA_MOD * surface->GetURange();
              step2 = DELTA_MOD * surface->GetVRange();
              ::GetDouble( IDS_ENTER_STEP, step1 );
              ::GetDouble( IDS_ENTER_STEP, step2 );
            }
            else if ( stepType == ist_MetricStep ) {
              step1 = dist;
              GetDouble( IDS_ENTER_STEP, step1 );
              step2 = step1;
              GetDouble( IDS_ENTER_STEP, step2 );
            }
          }
          else if ( gridType == igt_Concentric ) {
            ptrdiff_t uPntsCnt = 5;
            GetInt( IDS_ENTER_NUMBER, uPntsCnt );
            step1 = (double)uPntsCnt;
            step2 = dist;
            GetDouble( IDS_ENTER_STEP, step2 );
          }
          else if ( gridType == igt_Hexagonal ) {
            GetDouble( IDS_ENTER_STEP, dist );
            step1 = step2 = dist;
          }
        }
        else {
          if ( stepType == ist_ParamStep ) {
            ptrdiff_t uPntsCnt = 2;
            ptrdiff_t vPntsCnt = 2;
            GetInt( IDS_ENTER_NUMBER, uPntsCnt );
            GetInt( IDS_ENTER_NUMBER, vPntsCnt );
            step1 = (double)std_max( uPntsCnt, (ptrdiff_t)2 );
            step2 = (double)std_max( vPntsCnt, (ptrdiff_t)2 );
          }
          else if ( stepType == ist_SpaceStep ) {
            step1 = DELTA_MOD;
            double dist = step1;
            GetDouble( IDS_ENTER_DISTANCE, dist );
            step1 = step2 = dist;
          }
          else if ( stepType == ist_DeviationStep ) {
            step1 = Math::deviateSag;
            double angle = (step1 * 180.0 / M_PI);
            GetDouble( IDS_ENTER_ANGLE, angle );
            step1 = step2 = (angle * M_PI / 180.0);
          }
        }        

        double angle = 0.0;
        if ( aroundPoint ) {
          GetDouble( IDS_ENTER_ANGLE, angle );
          angle *= (M_PI / 180.0);
        }

        bool truncateByBounds = GetBoolean( IDS_ENTER_TRUNCATE_BOOL );

        SetWaitCursor( true );

        if ( aroundPoint ) {
          res = ::PointsOnSurface( *surface,          // Поверхность-источник. Source surface.
                                   gridType,          // Тип сетки. Type of a grid.
                                   paramPnt0,         // Базовая точка на поверхности. Base point on the surface.
                                   angle,             // Угол доворота. Rotation angle.
                                   stepType,          // Тип шага по поверхности. Type of a step on the surface.
                                   step1,             // Величина шага 1. A first step value.
                                   step2,             // Величина шага 2. A second step value.
                                   truncateByBounds,  // Усечь границами поверхности. Truncate by the surface boundaries.
                                   pntsData );        // Индексированные пространственно-параметрические точки. Indexed spatial-parametric points.
        }
        else {
          res = ::PointsOnSurface( *surface,          // Поверхность-источник. Source surface.
                                   stepType,          // Тип шага по поверхности. Type of a step.
                                   step1,             // Величина шага по u или количество точек по u при шаге по параметру
                                                      // Step by U value or number of points by U if the step is by parameter
                                   step2,             // Величина шага по v или количество точек по v при шаге по параметру
                                                      // Step by V value or number of points by V if the step is by parameter
                                   truncateByBounds,  // Усечь границами поверхности. Truncate by the surface boundaries.
                                   pntsData );        // Индексированные пространственно-параметрические точки. Indexed spatial-parametric points.
        }

        SetWaitCursor( false );

        TCHAR msgTitle[] = IDS_TITLE_MSG;

        if ( res == rt_Success ) {
          MbPointFrame * pFrame = new MbPointFrame;

          SArray<MbCartPoint3D> spacePnts( 0, 1 );
          for ( size_t i = 0, icnt = pntsData.Count(); i < icnt; i++ ) {
            SArray<MbLocPnt> * pntsItem = pntsData[i];
            if ( pntsItem != nullptr ) {
              for ( size_t j = 0, jcnt = pntsItem->Count(); j < jcnt; j++ ) {
                const MbLocPnt & locPnt = (*pntsItem)[j];
                pFrame->AddVertex( locPnt.first.GetSpacePnt() );
              }
            }
          }

          viewManager->AddObject( POINT_Style, pFrame );

          int iProjCheck = 0;
          ::GetInt( IDS_ENTER_CHECKRESULT, iProjCheck );

          if ( iProjCheck != 0 ) {
            bool isAnyProjError = false;
            double u, v;
            u = v = UNDEFINED_DBL;
            MbCartPoint3D spaceProjPnt;

//#ifdef _DRAWGI
            double umin = surface->GetUMin();
            double umax = surface->GetUMax();
            double vmin = surface->GetVMin();
            double vmax = surface->GetVMax();
//#endif

            for ( size_t k = 0, kcnt = spacePnts.Count(); k < kcnt; k++ ) {
              const MbCartPoint3D & spacePnt = spacePnts[k];
              bool isProj = surface->NearPointProjection( spacePnt, u, v, true );
//#ifdef _DRAWGI
              if ( isProj ) {
                if ( u < umin - PARAM_EPSILON || u > umax + PARAM_EPSILON || 
                     v < vmin - PARAM_EPSILON || v > vmax + PARAM_EPSILON ) {
                  if ( !surface->IsPole( u, v ) ) {
                    drawImp->DrawPoint( &spacePnt, TRGB_WHITE );
                    drawImp->DrawPoint( &spacePnt, TRGB_BLACK );
                  }
                }
              }
//#endif
              surface->_PointOn( u, v, spaceProjPnt );

              if ( !c3d::EqualPoints( spacePnt, spaceProjPnt, METRIC_PRECISION ) ) {
//#ifdef _DRAWGI
                drawImp->DrawPoint( &spacePnt, TRGB_WHITE );
                drawImp->DrawPoint( &spaceProjPnt, TRGB_WHITE );
                drawImp->DrawLine( spacePnt, spaceProjPnt, TRGB_WHITE );
                drawImp->DrawPoint( &spacePnt, TRGB_ORANGE );
                drawImp->DrawPoint( &spaceProjPnt, TRGB_ORANGE );
                drawImp->DrawLine( spacePnt, spaceProjPnt, TRGB_ORANGE );
//#endif
                viewManager->AddObject( BACKGROUND_Style, new MbLineSegment3D( spacePnt, spaceProjPnt ) );
                isAnyProjError = true;
              }
              else if ( !isProj ) {
//#ifdef _DRAWGI
                drawImp->DrawPoint( &spacePnt, TRGB_WHITE );
                drawImp->DrawPoint( &spacePnt, TRGB_BLACK );
//#endif
                viewManager->AddObject( BACKGROUND_Style, new MbLineSegment3D( MbCartPoint3D(), spacePnt ) );
                isAnyProjError = true;
              }
            }

            if ( isAnyProjError )
              MessageBoxEx( RT_PROJECTION_ERROR, msgTitle );
            else
              MessageBoxEx( IDS_TITLE_ALL_RIGHT, msgTitle );
          }
        }
        else {
          MessageBoxEx( RT_ERROR_IN_PARAMS, msgTitle );
        }
        for ( size_t j = 0, jCount = pntsData.size(); j < jCount; j++ ) 
          delete pntsData[j];
      }
    }

    viewManager->ToggleObject( obj );
  }
}


//------------------------------------------------------------------------------
// Ближайшая проекция точки на поверхность
// The nearest project of a point on a surface.
// ---
void CreateSurfaceNearPointProjection()
{
  MbItem * nearest = nullptr;
  MbCartPoint3D point;

  if ( GetPoint( IDS_SELECT_POINT, point ) ) {
    if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &nearest ) ) {
      viewManager->ToggleObject(nearest);
      MbSurface * surface = (MbSurface *)((MbSpaceInstance *)nearest)->GetSpaceItem();
      
      MbCartPoint uvProj;
      MbCartPoint3D p;

      int iExt = 0;
      GetInt( IDS_ENTER_EXT_FLAG, iExt );
      bool bExt = (iExt > 0) ? true : false;

      MbRect2D * uvRange = nullptr;

      TCHAR msgTitle[] = IDS_TITLE_MSG;

      int iUseRange = 0;
      if ( GetInt( IDS_ENTER_USE_RANGE, iUseRange ) ) {
        if ( iUseRange > 0 ) {
          double umin = surface->GetUMin();
          double umax = surface->GetUMax();
          double vmin = surface->GetVMin();
          double vmax = surface->GetVMax();

          MbRect1D uRange( umin, umax );
          MbRect1D vRange( vmin, vmax );

          ptrdiff_t len = 0;
          TCHAR buf[1024];
          len += (ptrdiff_t)_sntprintf( buf+len, 1024-len, _T(" Surface parameters: \n\r") );
          len += (ptrdiff_t)_sntprintf( buf+len, 1024-len, _T("---------------------\n\r") );
          len += (ptrdiff_t)_sntprintf( buf+len, 1024-len, _T(" umin = %.8f\n\r"), umin );
          len += (ptrdiff_t)_sntprintf( buf+len, 1024-len, _T(" umax = %.8f\n\r"), umax );
          len += (ptrdiff_t)_sntprintf( buf+len, 1024-len, _T(" vmin = %.8f\n\r"), vmin );
          len += (ptrdiff_t)_sntprintf( buf+len, 1024-len, _T(" vmax = %.8f\n\r"), vmax );
          len += (ptrdiff_t)_sntprintf( buf+len, 1024-len, _T("---------------------\n\r") );
          MessageBoxEx( buf, msgTitle );

          TCHAR msgSwapParams[] = RT_CHECK_PERMUTATION_PARAMETERS;
          TCHAR msgNearParams[] = RT_SMALL_RANGE_SURFACE;

          if ( GetDouble( IDS_ENTER_INITIAL_PRM, uRange.zmin ) &&
               GetDouble( IDS_ENTER_FINAL_PRM,   uRange.zmax  ) ) {
            if ( uRange.zmax < uRange.zmin ) {
              uRange.Equalize();
              MessageBoxEx( msgSwapParams, msgTitle );
            }
            if ( ::fabs(uRange.zmax - uRange.zmin) < PARAM_REGION ) {
              uRange.Init( umin, umax );
              MessageBoxEx( msgNearParams, msgTitle );
            }
          }

          if ( GetDouble( IDS_ENTER_INITIAL_PRM, vRange.zmin ) &&
               GetDouble( IDS_ENTER_FINAL_PRM,   vRange.zmax  ) ) {
            if ( vRange.zmax < vRange.zmin ) {
              vRange.Equalize();
              MessageBoxEx( msgSwapParams, msgTitle );
            }
            if ( ::fabs(vRange.zmax - vRange.zmin) < PARAM_REGION ) {
              vRange.Init( vmin, vmax );
              MessageBoxEx( msgNearParams, msgTitle );
            }
          }

          uvRange = new MbRect2D( uRange.zmin, vRange.zmin,
                                  uRange.zmax, vRange.zmax );
        }
      }

      bool bRes = surface->NearPointProjection( point, uvProj.x, uvProj.y, bExt, uvRange );

      if ( bRes ) {
        MessageBoxEx(RT_FOUND_NORMAL_PROJECTION, msgTitle );

        MbCartPoint3D surfPoint;
        surface->_PointOn( uvProj.x, uvProj.y, surfPoint );
        MbVector3D surfNormal;
        surface->_Normal( uvProj.x, uvProj.y, surfNormal );
        MbVector3D dirVect( point, surfPoint );
        PRECONDITION( dirVect.Colinear( surfNormal ) );
      }

      if ( uvRange != nullptr )
        delete uvRange;

      viewManager->ToggleObject(nearest);
      
      surface->_PointOn( uvProj.x, uvProj.y, p );

      viewManager->AddObject( POINT_Style, new MbPointFrame(p) );

      viewManager->AddObject( LINE_Style, new MbLineSegment3D( point, p ) );
    }
  }
}


//------------------------------------------------------------------------------
// Создание проекции точки на поверхность по направлению
// Create a projection of a point on a surface along a direction
// ---
void CreateSurfaceDirectPointProjection()
{
  MbItem * obj = nullptr;
  MbCartPoint3D pnt;

  if ( GetPoint( IDS_SELECT_POINT, pnt ) ) {
    if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &obj ) ) {
      viewManager->ToggleObject( obj );
      MbSurface * surface = (MbSurface *)((MbSpaceInstance *)obj)->GetSpaceItem();

      MbVector3D * direct = nullptr;

      if ( ::GetGObj( IDS_SELECT_LINE, st_Curve3D, &obj ) && (obj->IsA() == st_SpaceInstance) && 
           (((MbSpaceInstance *)obj)->GetSpaceItem()->Family() == st_Curve3D) ) {
        const MbCurve3D & dirCurve = *(MbCurve3D *)((MbSpaceInstance *)obj)->GetSpaceItem();
        if ( dirCurve.IsStraight() ) {
          direct = new MbVector3D;
          dirCurve.GetLimitTangent( 1, *direct);
        }
        else if ( dirCurve.Type() == st_Contour3D ) {
          const MbContour3D & dirCntr = (const MbContour3D &)dirCurve;
          if ( dirCntr.GetSegmentsCount() == 1 && dirCntr.GetSegment( 0 ) != nullptr ) {
            const MbCurve3D & dirSegment = *dirCntr.GetSegment( 0 );
            if ( dirSegment.IsStraight() ) {
              direct = new MbVector3D;
              dirSegment.GetLimitTangent( 1, *direct );
            }
          }
        }
      }

      if ( direct != nullptr ) {
        SArray<MbCartPoint> uvPnts( 2, 2 );
        surface->DirectPointProjection( pnt, *direct, uvPnts, false  );

        size_t k, cnt = uvPnts.Count();
        SArray<MbCartPoint3D> pnts( 2, 2 );
        double dmax = -MB_MAXDOUBLE;
        size_t kmax = SYS_MAX_T;

        for ( k = 0; k < cnt; k++ ) {
          MbCartPoint3D & projPnt = *(pnts.Add());
          surface->_PointOn( uvPnts[k].x, uvPnts[k].y, projPnt );
          viewManager->AddObject( POINT_Style, new MbPointFrame( projPnt ) );
          double dcur = pnt.DistanceToPoint( projPnt );
          if ( dcur > dmax ) {
            dmax = dcur;
            kmax = k;
          }
        }

        if ( kmax != SYS_MAX_T )
          viewManager->AddObject( LINE_Style, new MbLineSegment3D( pnt, pnts[kmax] ) );

        if ( cnt < 1 )
          MessageBoxEx( RT_PROJECTIONS_IN_DIRECTION_NOT_FOUND, IDS_TITLE_WRN );
      }
      else
      {
        MessageBoxEx( RT_NOT_SELECT_GUIDE_SEGMENT, IDS_TITLE_ERR );
      }
      
      viewManager->ToggleObject( obj );

      if ( direct != nullptr ) {
        delete direct;
        direct = nullptr;
      }
    }
  }
}


//------------------------------------------------------------------------------
// Проекция точки на тело
// A point projection on a solid body.
// ---
void CreateSolidPointProjection()
{
  MbItem * nearest = nullptr;
  MbCartPoint3D point;

  if ( GetPoint( IDS_SELECT_POINT, point ) ) {
    if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &nearest ) ) {
      viewManager->ToggleObject(nearest);
      const MbSolid * solid = static_cast<MbSolid *>(nearest);
      
      const MbFaceShell * shell = (solid != nullptr) ? solid->GetShell() : nullptr;

      if ( shell != nullptr ) {
        SArray<MbCartPoint> uvArr(2,2);
        SArray<size_t> fnArr(2,2);
        shell->NearPointProjection( point, fnArr, uvArr );

        if ( uvArr.Count() < 1 ) {
          size_t fn = SYS_MAX_T;
          MbCartPoint uv;
          shell->NearPointProjection( point, fn, uv.x, uv.y );
          if ( fn != SYS_MAX_T ) {
            fnArr.Add( fn );
            uvArr.Add( uv );
          }
        }

        viewManager->ToggleObject(nearest);

        MbCartPoint3D p;
        for ( size_t i = 0; i < uvArr.size(); i++ ) {
          shell->PointOn( fnArr[i], uvArr[i].x, uvArr[i].y, p );
          viewManager->AddObject( POINT_Style, new MbPointFrame(p) );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание точечного каркаса по вершинам тела
// Create point frame by solid vertices
// ---
bool CreatePointFrameBySolidVertices()
{
  bool wasProcessed = false;

  MbItem * item = nullptr;

  PathMatrix pathFrom;
  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &item, pathFrom ) ) {
    MbSolid * solid = static_cast<MbSolid *>(item);

    wasProcessed = true;

    TestVariables::viewManager->SelectObject( pathFrom.first );

    if ( solid->GetFacesCount() > 0 ) {
      c3d::ConstVerticesSPtrVector vertices;
      solid->GetShell()->GetVertices( vertices );
      PointFrameSPtr pointFrame( new MbPointFrame( vertices, false ) );
      pointFrame->Transform( pathFrom.second );
      TestVariables::viewManager->AddObject( TestVariables::POINT_Style, pointFrame );
    }
  }

  return wasProcessed;
}


//------------------------------------------------------------------------------
// Объединить точечные каркасы 
// Create point frames union
// ---
void CreatePointFramesUnion()
{
  MbItem * item = nullptr;

  PointFramesSPtrVector pointFrames;
  pointFrames.reserve( 10 );

  std::vector<MbPath> paths;
  paths.reserve( 10 );

  std::set<SimpleName> pathHashes;
  std::set<SimpleName>::iterator pathHashesIt;

  PathMatrix pathFrom;
  while ( GetGObj( IDS_SELECT_POINT, st_Point3D, &item, pathFrom ) ) {
    PointFrameSPtr pointFrame;

    SimpleName pathHash( pathFrom.first.Hash() );

    pathHashesIt = pathHashes.find( pathHash );

    if ( pathHashesIt == pathHashes.end() ) {
      if ( item->IsA() == st_PointFrame ) {
        pointFrame = static_cast<MbPointFrame *>(&static_cast<const MbPointFrame *>(item)->Duplicate());
        if ( pointFrame != nullptr ) {
          TestVariables::viewManager->ToggleObject( pathFrom.first );

          pointFrame->Transform( pathFrom.second );
          pointFrames.push_back( pointFrame );

          pathHashes.insert( pathHash );
          paths.push_back( pathFrom.first );
        }
      }
      else if ( item->IsA() == st_SpaceInstance ) {
        const MbSpaceInstance * spaceInst = static_cast<const MbSpaceInstance *>(item);
        if ( spaceInst->GetSpaceItem() != nullptr ) {
          const MbSpaceItem * spaceItem = spaceInst->GetSpaceItem();
          if ( spaceItem != nullptr && spaceItem->IsA() == st_Point3D ) {
            const MbPoint3D & point = static_cast<const MbPoint3D &>(*spaceItem);

            TestVariables::viewManager->ToggleObject( pathFrom.first );

            pointFrame = new MbPointFrame( point );
            pointFrame->Transform( pathFrom.second );
            pointFrames.push_back( pointFrame );

            pathHashes.insert( pathHash );
            paths.push_back( pathFrom.first );
          }
        }
      }
    }
  }

  size_t pointFramesCnt = pointFrames.size();

  if ( pointFramesCnt > 1 ) {
    PointFrameSPtr pointFrame( new MbPointFrame );

    size_t k;
    for ( k = 0; k < pointFramesCnt; ++k ) {
      pointFrame->AddVertices( *pointFrames[k], true );
    }
    pointFrames.clear();

    for ( k = paths.size(); k--; )
      TestVariables::viewManager->DeleteObject( paths[k] );
    TestVariables::viewManager->AddObject( TestVariables::POINT_Style, pointFrame );
  }
}


//------------------------------------------------------------------------------
// Точки пересечения проволочных каркасов
// Points of intersection of two wireframes
// ---
void CreateWireFrameWireFrameInters()
{
  MbItem * wireFrameItem1 = nullptr;

  if ( GetGObj(IDS_SELECT_WIREFRAME, st_SpaceItem, &wireFrameItem1) && (wireFrameItem1 != nullptr) ) {
    if ( wireFrameItem1->IsA() == st_WireFrame ) {
      WireFrameSPtr wireFrame1( static_cast<MbWireFrame*>(wireFrameItem1) );
      MbItem * wireFrameItem2 = nullptr;
      if ( GetGObj(IDS_SELECT_WIREFRAME, st_SpaceItem, &wireFrameItem2) && (wireFrameItem2 != nullptr) ) {
        if ( wireFrameItem2->IsA() == st_WireFrame ) {
          WireFrameSPtr wireFrame2( static_cast<MbWireFrame*>(wireFrameItem2) );
          WireFramePointsPairsVector result;
          ::WireFramesIntersect( *wireFrame1, *wireFrame2, METRIC_EPSILON, result );
          MbCartPoint3D p1, p2, p3;
          for ( size_t i = 0, size = result.size(); i < size; i++ ) {
            p1 = result[i].first.GetEdgePoint();
            p2 = result[i].second.GetEdgePoint();
            viewManager->AddObject( POINT_Style, new MbPointFrame(p1) );
            viewManager->AddObject( POINT_Style, new MbPointFrame(p2) );
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Точки пересечения кривых
// Points of intersection of two curves
// ---
void CreateCurveCurveInters()
{
  SPtr<MbCurve3D> curve1;
  PathMatrix pathFrom1;
  MbItem * obj1 = nullptr;

  if ( (curve1 = ::SelectAnyCurveCopy( pathFrom1, nullptr, &obj1 )) ) {
    viewManager->ToggleObject( obj1 );

    SPtr<MbCurve3D> curve2;
    PathMatrix pathFrom2;
    MbItem * obj2 = nullptr;

    if ( (curve2 = ::SelectAnyCurveCopy( pathFrom2, nullptr, &obj2 )) ) {
      viewManager->ToggleObject( obj2 );

      curve1->Transform( pathFrom1.second );
      curve2->Transform( pathFrom2.second );

      MbCartPoint3D p1, p2;
      SArray<double> t1( 0, 1 );
      SArray<double> t2( 0, 1 );

      MbAccurateTimer ac;
      ac.Begin();

      ::CurveCurveIntersection( *curve1, *curve2, t1, t2, Math::metricRegion, Math::DefaultMathVersion() );

      ac.End();

      viewManager->ToggleObject( obj2 );
      viewManager->ToggleObject( obj1 );
      for ( ptrdiff_t i = 0; i < (ptrdiff_t)t1.Count(); i++ ) {
        curve1->PointOn( t1[i], p1 );
        viewManager->AddObject( POINT_Style, new MbPointFrame(p1) );
        curve2->PointOn( t2[i], p2 );
        if ( !c3d::EqualPoints( p1, p2, METRIC_EPSILON ) )
          viewManager->AddObject( SURFACECURVE_Style, new MbPointFrame(p2) );
      }

      if ( TestVariables::checkOperationTime ) {
        const size_t bufLen = 256;
        TCHAR buf[bufLen];
        size_t len = 0;
        len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
        ::MessageBoxEx( buf, IDS_RESULTS );
      }
    }
    else
      viewManager->ToggleObject( obj1 );
  }
}


//------------------------------------------------------------------------------
// Точки пересечения кривой с поверхностью
// Points of intersection of a curve and a surface
// ---
void CreateCurveSurfaceInters()
{
  MbItem *nearest[2];

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest[0] ) ) {
    viewManager->ToggleObject(nearest[0]);
    if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &nearest[1] ) ) {
      viewManager->ToggleObject(nearest[1]);
      SPtr<MbCurve3D> curve( ::GetCurve3D( nearest[0], true ) );
      const MbSurface * surface = (const MbSurface *)((const MbSpaceInstance *)nearest[1])->GetSpaceItem();
      {
        SArray <double> tt(2,2);
        SArray <MbCartPoint> uv(2,2); // Множество параметров на поверхности
        MbCartPoint3D p;

        bool touchInclude = true;
        uint32 start_time = ::GetTickCountEx();
        ::IntersectionPoints( *surface, false, *curve, false, uv, tt, touchInclude );
        uint32 end_time = ::GetTickCountEx();

        viewManager->ToggleObject(nearest[1]);
        viewManager->ToggleObject(nearest[0]);
        for ( ptrdiff_t i = 0; i < (ptrdiff_t)uv.Count(); i++ ) {
          surface->PointOn( uv[i].x, uv[i].y, p );
          viewManager->AddObject( POINT_Style, new MbPointFrame(p) );
        }

        if ( TestVariables::checkOperationTime ) {
          TCHAR msgTime[2048];
          _sntprintf( msgTime, 2048, RT_TIME_MSEC, end_time - start_time );
          ::MessageBoxEx( msgTime, RT_RUNNING_TIME );
        }
      } 

    }
    else
      viewManager->ToggleObject(nearest[0]);
  }
}


//------------------------------------------------------------------------------
// Точки пересечения кривой с телом
// Points of intersection of a curve with a solid body.
// ---
void CreateCurveSolidInters()
{
  MbItem *nearest[2];

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest[0] ) ) {
    viewManager->ToggleObject(nearest[0]);
    if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &nearest[1] ) ) {
      viewManager->ToggleObject(nearest[1]);
      SPtr<MbCurve3D> curve( ::GetCurve3D( nearest[0], true ) );
      MbSolid * solid = (MbSolid *)nearest[1];

      MbFaceShell * shell = solid->GetShell();
      if ( shell != nullptr ) {
        SArray<double> tt(2,2);
        SArray<MbCartPoint> uv(2,2); // Множество параметров на поверхности. Set of parameters of the surface.
        SArray<size_t> n(2,2);
        MbCartPoint3D p;

        shell->CurveIntersection( *curve, n, uv, tt );

        for ( size_t i = 0; i < uv.Count(); i++ ) {
          shell->PointOn( n[i], uv[i].x, uv[i].y, p );
          viewManager->AddObject( POINT_Style, new MbPointFrame(p) );
        }
      }
      viewManager->ToggleObject(nearest[1]);
      viewManager->ToggleObject(nearest[0]);
    }
    else
      viewManager->ToggleObject(nearest[0]);
  }
}


//------------------------------------------------------------------------------
// Выдать n точек кривой с равными интервалами по параметру
// Get n points of a curve with equal intervals of the parameter of the curve.
// ---
void CreateCurveEvenParamPoints()
{
  MbItem * nearest = nullptr;
  std::vector<MbCartPoint3D> pnts;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest ) && (nearest->IsA() == st_SpaceInstance) ) {
    viewManager->ToggleObject(nearest);

    if ( GetInt( IDS_ENTER_QUANTITY, TestVariables::count ) ) {
      MbCurve3D * curve = (MbCurve3D *)(((MbSpaceInstance *)nearest)->GetSpaceItem());
      if ( TestVariables::count == 0 ) {
        if ( curve->IsA() == st_SurfaceIntersectionCurve ) {
          const MbSurfaceIntersectionCurve * intCurve = (const MbSurfaceIntersectionCurve *)curve;
          const MbCurve & pCurve = intCurve->GetCurveOneCurve();
          if ( pCurve.IsA() == pt_Hermit ) {
            const MbHermit & hermit = (const MbHermit &)pCurve;
            MbCartPoint3D pnt;
            for ( size_t i = 0, icount = hermit.GetPointsCount(); i < icount; i++ ) {
              double t = hermit.GetParam( i );
              curve->PointOn( t, pnt );
              pnts.push_back( pnt );
            }
          }
        }
      }
      else
        curve->GetPointsByEvenParamDelta( TestVariables::count, pnts );
    }
    viewManager->ToggleObject(nearest);
  }

  for( size_t i = 0; i < pnts.size(); i++ ) {
    viewManager->AddObject( POINT_Style, new MbPointFrame(pnts[i]) );
  }
}


//------------------------------------------------------------------------------
// Выдать n точек кривой с равными интервалами по по длине дуги
// Get n points of a curve with equal intervals along the curve
// ---
void CreateCurveEvenLengthPoints()
{
  MbItem * nearest = nullptr;
  std::vector<MbCartPoint3D> pnts;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest ) && (nearest->IsA() == st_SpaceInstance) ) {
    viewManager->ToggleObject(nearest);

    if ( GetInt( IDS_ENTER_QUANTITY, TestVariables::count ) ) {
      MbCurve3D *curve = (MbCurve3D *)(((MbSpaceInstance *)nearest)->GetSpaceItem());
      curve->GetPointsByEvenLengthDelta( TestVariables::count, pnts );
    }
    viewManager->ToggleObject(nearest);
  }

  for( size_t i = 0; i < pnts.size(); i++ ) {
    viewManager->AddObject( POINT_Style, new MbPointFrame(pnts[i]) );
  }
}


//------------------------------------------------------------------------------
// Дать точки на кривой, отстоящие на расстояние len по дуге
// Get points of a curve with interval len along the curve
// ---
void CreateCurvePointDistanceAlong()
{
  MbItem * nearest = nullptr;
  MbCartPoint3D p0, pnt;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest ) && (nearest->IsA() == st_SpaceInstance) ) {
    viewManager->ToggleObject(nearest);

    MbCurve3D * curve = (MbCurve3D *)(((MbSpaceInstance *)nearest)->GetSpaceItem());
    if ( GetPoint( IDS_SELECT_POINT, pnt ) ) {
      double t = UNDEFINED_DBL;
      curve->NearPointProjection( pnt, t, false );
      curve->PointOn( t, pnt );

      int n = 1;
      double ln = 20;
      if ( GetInt( IDS_ENTER_SENSE, n ) && GetDouble( IDS_ENTER_DISTANCE, ln ) ) {
        if ( curve->DistanceAlong( t, ln, n ) ) {
          viewManager->AddObject( POINT_Style, new MbPointFrame(pnt) );
          curve->PointOn( t, pnt );
          viewManager->AddObject( POINT_Style, new MbPointFrame(pnt) );
        }
      }
    }
    viewManager->ToggleObject(nearest);
  }
}


//------------------------------------------------------------------------------
// Дать точки касания кривой по отношению к направляющим вращения \ выдавливания
// Get tangent points of a curve with a guide curve of a rotation/extrusion
// ---
void CreateCurveIsoPoints() {
/*
  MbItem * obj;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) ) {
    const MbSpaceItem * sItem = obj->GetSpaceItem(); // Дать трёхмерный геометрический объекта. Get a spatial geometric object.
    if ( (sItem != nullptr) && (sItem->Family() == st_Curve3D) ) {
      MbCurve3D * curve = (MbCurve3D *)sItem;
      MbCartPoint3D pnt1, pnt2;
      int rotate = 0;
      if ( GetPoint( IDS_SELECT_START_POINT, pnt1 ) && 
           GetPoint( IDS_SELECT_FINAL_POINT, pnt2 ) &&
           GetInt  ( IDS_ENTER_ROTATIOBAXIS, rotate ) ) { // Ввод точки для вектора направления. Get a point for a direction vector.
         MbVector3D direction( pnt1, pnt2 );
         MbAxis3D   axis( pnt1, pnt2 );
         bool       rotation( rotate > 0 );
         SArray<double> tTouch( 0, 1 );

        ::TouchPointsTowardGuide( *curve, direction, axis, rotation, tTouch ); 

        for( size_t i = 0; i < tTouch.Count(); i++ ) {
          curve->PointOn( tTouch[i], pnt1 );
          viewManager->AddObject( POINT_Style, new MbPointFrame(pnt1) );
        }
      }
    }
  }
*/
}

