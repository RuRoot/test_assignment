#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <test_computation.h>
#include <test_draw.h>
#include <cur_contour.h>
#include <cur_contour_on_surface.h>
#include <cur_surface_curve.h>
#include <cur_surface_intersection.h>
#include <surface.h>
#include <solid.h>
#include <function_factory.h>
#include <func_line_function.h>
#include <func_sinus_function.h>
#include <func_cubic_function.h>
#include <action_surface_curve.h>
#include <action_wireframe.h>
#include <action_surface.h>
#include <action_shell.h>
#include <action_solid.h>
#include <action_phantom.h>
#include <op_wireframe_parameter.h>
#include <space_instance.h>
#include <plane_instance.h>
#include <wire_frame.h>
#include <tri_face.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Создание граничной кривой поверхности.
// Create a border curve of the surface.
// ---
void CreateSurfaceBorder()
{
  MbItem    * surface = nullptr;
  MbCurve3D * curv    = nullptr;
  MbSurface * surf    = nullptr;

  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &surface ) ) {
    TestVariables::viewManager->ToggleObject(surface);

    ptrdiff_t n = 1;
    if ( GetInt( IDS_ENTER_POINT_NUMB_1_4, n ) ) {
      surf = (MbSurface *)((MbSpaceInstance *)surface)->GetSpaceItem();
      while (n<1) n += 4;         //-V112
      while (n>4) n -= 4;         //-V112
      switch (n) {
        case 1: 
          curv = surf->CurveU( surf->GetVMin(), nullptr );
          break;
        case 2:
          curv = surf->CurveV( surf->GetUMax(), nullptr );
          break;
        case 3:
          curv = surf->CurveU( surf->GetVMax(), nullptr );
          break;
        case 4:
          curv = surf->CurveV( surf->GetUMin(), nullptr );
          break;         //-V112
      }
    }

    TestVariables::viewManager->ToggleObject(surface);
  }

  if (curv != nullptr)
    TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, curv );
}


//------------------------------------------------------------------------------
// Создание проекции кривой на поверхность.
// Create a curve projection on surface.
// ---
void CreateSurfaceCurveProjection()
{
  MbItem *nearest[2];

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest[0] ) ) {
    TestVariables::viewManager->ToggleObject(nearest[0]);

    if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &nearest[1] ) ) {
      TestVariables::viewManager->ToggleObject(nearest[1]);
      SPtr<MbCurve3D> crv( ::GetCurve3D( nearest[0], false ) );

      if ( crv != nullptr ) {
        int n( 1 );
        GetInt( IDS_ENTER_EXT_FLAG, n );
        RPArray<MbCurve3D> curves( 0, 1 );
        SPtr<MbSurface> surf( static_cast<MbSurface *>(((MbSpaceInstance *)nearest[1])->SetSpaceItem()) );
        MbProjectionWireFrameParams params( *surf, *crv, nullptr, true, n <= 0 );
        MbProjectionWireFrameResults results;

        ::ProjectionWireFrame( params, results );
        results.DetachCurves( curves );

        //MbResultType resType = ::CurveProjection( *(MbSurface *)nearest[1]->GetSpaceItem(), *crv, dir, curves );

        for ( size_t i = 0, iCount = curves.Count(); i < iCount; i++ ) 
          TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, curves[i] ); 
      }
      TestVariables::viewManager->ToggleObject(nearest[1]);
    }

    //TestVariables::viewManager->ToggleObject(nearest[0]);
  }
}


//------------------------------------------------------------------------------
// Создание проекции кривой на поверхность по направлению.
// Create a curve projection on surface by direction.
// ---
void CreateSurfaceCurveDirectProjection()
{
  MbItem *nearest[2];
  MbCartPoint3D points[2];

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest[0] ) && (nearest[0]->IsA() == st_SpaceInstance) ) {
    TestVariables::viewManager->ToggleObject(nearest[0]);

    if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &nearest[1] ) && (nearest[1]->IsA() == st_SpaceInstance) ) {
      TestVariables::viewManager->ToggleObject(nearest[1]);

      if ( GetPoint( IDS_SELECT_START_POINT, points[0] ) &&
           GetPoint( IDS_SELECT_FINAL_POINT, points[1] ) ) {
        MbVector3D dir( points[0], points[1] );
        MbCurve3D * crv = (MbCurve3D *)((MbSpaceInstance *)nearest[0])->GetSpaceItem();
        if ( crv != nullptr ) {
          int n( 1 );
          GetInt( IDS_ENTER_EXT_FLAG, n );
          RPArray<MbCurve3D> curves( 0, 1 );
          const MbSurface & surface( static_cast<const MbSurface &>( *((MbSpaceInstance *) nearest[1])->GetSpaceItem()) );
          MbProjectionWireFrameParams params( surface, *crv, &dir, true, n <= 0 );
          MbProjectionWireFrameResults results;

          ::ProjectionWireFrame( params, results );
          results.DetachCurves( curves );

          for ( size_t i = 0, iCount = curves.Count(); i < iCount; i++ ) 
            TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, curves[i] ); 
        }
      }

      TestVariables::viewManager->ToggleObject(nearest[1]);
    }

    TestVariables::viewManager->ToggleObject(nearest[0]);
  }
}


//------------------------------------------------------------------------------
// Создание кривой пересечения поверхностей.
// Create a surface intersection curve.
// ---
void CreateSurfaceIntersectionCurve()
{
  MbItem *nearest[2];

  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &nearest[0] ) ) {
    TestVariables::viewManager->ToggleObject(nearest[0]);

    if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &nearest[1] ) ) {
      TestVariables::viewManager->ToggleObject(nearest[1]);
      SetWaitCursor( true );

      MbSNameMaker snMaker( ct_Curve3DCreator, MbSNameMaker::i_SideNone, 0 );
      MbIntCurveParams params( snMaker );

      MbWireFrame * resFrame = nullptr;

      // Дублируем, чтобы не было вылета из-за распараллеливания.
      // Need to duplicate to prevent crashes with parallel running.
      SurfaceSPtr surface0( static_cast<MbSurface *>(&((MbSpaceInstance *)nearest[0])->GetSpaceItem()->Duplicate()) );
      SurfaceSPtr surface1( static_cast<MbSurface *>(&((MbSpaceInstance *)nearest[1])->GetSpaceItem()->Duplicate()) );

      MbIntCurveResults intCurveResults;
      ::IntersectionCurve( *surface0, *surface1, params, intCurveResults );

      resFrame = intCurveResults._wireFrame.detach();

      bool keepWireFrame = false;
      keepWireFrame = GetBool( IDS_SELECT_WIREFRAME, keepWireFrame );

      if ( keepWireFrame ) {
        TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, resFrame );
      }
      else {
        SpaceCurvesSPtrVector cross;
        ::ExtractCurvesDeleteFrame( resFrame, cross );

        size_t crossCnt = cross.size();

        if ( crossCnt > 0 ) {
          TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, cross[0], false );
          for ( size_t i = 1; i < crossCnt; ++i ) {
            cross[i] = static_cast<MbCurve3D *>(&cross[i]->Duplicate()); // недостатки паралелльной отрисовки с блокировкой только по MbItem
            if ( i + 1 < crossCnt )
              TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, cross[i], false );
            else
              TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, cross[i], true );
          }
        }
      }

      SetWaitCursor( false );
      TestVariables::viewManager->ToggleObject(nearest[1]);
    }

    TestVariables::viewManager->ToggleObject(nearest[0]);
  }
}


//------------------------------------------------------------------------------
// Создание кривой пересечения поверхностей по начальной точке и направлению.
// ---
void CreateSurfaceIntersectionCurveOnePoint()
{
  MbItem * surface1 = nullptr;
  MbItem * surface2 = nullptr;
  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &surface1 ) ) 
  {
    TestVariables::viewManager->ToggleObject(surface1);
    if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &surface2 ) )
    {
      TestVariables::viewManager->ToggleObject(surface2);
      const MbSurface * csurface1 = static_cast<const MbSurface *>(static_cast<MbSpaceInstance*>(surface1)->GetSpaceItem());
      const MbSurface * csurface2 = static_cast<const MbSurface *>(static_cast<MbSpaceInstance*>(surface2)->GetSpaceItem());
      MbCartPoint uv1, uv2;

      MbCartPoint3D point1(-120, -120, -120);
      MbCartPoint3D point2( point1 );
      bool found = false;
      size_t tries = 0;
      do // Ищем начальную точку пересечения поверхностей, сделаем несколько попыток, потому что поиск может не сойтись
      {  // в зависимости от начальной точки
        if ( GetPoint( IDS_SELECT_START_POINT, point1) ) // выбираем точку
        {
          size_t safei = 0;
          while ( !found && ++safei < 20 ) 
          { // Ищем начальную точку пересечения поверхностей, пошагово проецируя от заданной пользователем точки
            point2 += point1;
            point2 *= 0.5;
            csurface1->NearPointProjection( point2, uv1.x, uv1.y, false, nullptr );
            csurface1->PointOn( uv1, point1 );
            csurface2->NearPointProjection( point1, uv2.x, uv2.y, false, nullptr );
            csurface2->PointOn( uv2, point2 );
            found = point1.DistanceToPoint2( point2 ) < Math::accuracy; // Примерно, этого достаточно
            #ifdef _DRAWGI
              DrawGI::DrawPoint( &point1, TRGB_RED, 6 );
              DrawGI::DrawPoint( &point2, TRGB_BROWN, 6 );
              if ( found )
                DrawGI::DrawPoint( &point1, TRGB_GOLD, 4 ); // Искомая точка
            #endif
          }
        }
        else 
        {
          break; // Esc - прекращаем поиск
        }
      } while ( !found && ++tries < 15 );

      if ( found ) // Поиск сошёлся
      {
        MbCartPoint tmp1 (-10, -10), tmp2(10, 10); // Ограничения на поверхностях, вводим в окошках
        GetExactPlanePoint( tmp1 );
        GetExactPlanePoint( tmp2 );
        MbRect2D rect1 (tmp1, tmp2);
        GetExactPlanePoint( tmp1 );
        GetExactPlanePoint( tmp2 );
        MbRect2D rect2 (tmp1, tmp2);

        SetWaitCursor( true );
        MbVector3D dir (1, 0, 0); // Направление поиска
        if ( TestVariables::viewManager != nullptr )
        {
          WorkWindow * window = TestVariables::viewManager->GetWindow( TestVariables::viewManager->GetActiveWindowIndex() ) ;
          if ( window != nullptr &&  point1.MaxFactor() > METRIC_PRECISION ) // Запросить вектор взгляда.
          {
            MbVector3D Z( window->GetAxisZ() * 1e6 );    // Где-то на уровне глаз
            dir.Init( window->GetOrigin() + Z, point1 ); // Вгляд из точки зрения в заданную точку.
            dir.Normalize(); 
            MbLineSegment3D * ldir = new MbLineSegment3D( point1, dir*1000 ); // Отрисуем направление поиска
            TestVariables::viewManager->AddObject( *ldir ); // Отрисуем направление поиска
          }
        }
        MbSNameMaker snMaker( ct_Curve3DCreator, MbSNameMaker::i_SideNone, 0 );
        MbIntCurveParams params( rect1, rect2, uv1, uv2, dir, snMaker ); // Параметры поиска пересечения
        MbIntCurveResults results;
        if ( ::IntersectionCurve( *csurface1, *csurface2, params, results ) == rt_Success ) // Успешно пересекли
        {
          MbeCurveBuildType type = cbt_Boundary ;
          SPtr<MbSurfaceIntersectionCurve> ic ( 
            new MbSurfaceIntersectionCurve (*csurface1, *results._curve1, *csurface2, *results._curve2, type, true, true) );
          TestVariables::viewManager->AddObject(*ic); // Отррисуем полученную кривую.
        }
        SetWaitCursor( false );
      }
      TestVariables::viewManager->ToggleObject(surface2);
    }
    TestVariables::viewManager->ToggleObject(surface1);
  }
}


//------------------------------------------------------------------------------
// Создание эквидистантной поверхности.
// Create an offset surface.
// ---
void CreateOffsetSurface()
{
  MbItem  *surface; // Образующая. // Generating curve.

  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &surface ) ) {
    TestVariables::viewManager->ToggleObject(surface);

    const MbSpaceItem * sItem = ((MbSpaceInstance *)surface)->GetSpaceItem();
    if ( (sItem != nullptr) && (sItem->Family() == st_Surface) ) {
      if ( GetInt( IDS_ENTER_OFFSET_TYPE, TestVariables::offsetType ) ) {
        if ( GetDouble( IDS_ENTER_DISTANCE, TestVariables::offset0 ) ) {
          if ( TestVariables::offsetType > 0 ) {
            GetDouble( IDS_ENTER_DISTANCE, TestVariables::offset1 );
            GetDouble( IDS_ENTER_DISTANCE, TestVariables::offset2 );
            GetDouble( IDS_ENTER_DISTANCE, TestVariables::offset3 );
          }

          SPtr<MbSurface> s( static_cast<MbSurface *>(&sItem->Duplicate()) );
          MbSurface * surf( nullptr );

          if ( TestVariables::offsetType > 0 )
            ::OffsetSurface( *s, TestVariables::offset0, TestVariables::offset1, TestVariables::offset2, TestVariables::offset3,
                             (MbeOffsetType)TestVariables::offsetType, surf );
          else
            ::OffsetSurface( *s, TestVariables::offset0, surf );

          TestVariables::viewManager->AddObject( TestVariables::SURFACE_Style, surf );
        }
      }

      TestVariables::viewManager->ToggleObject(surface);
    }
  }
}


//------------------------------------------------------------------------------
// Создание деформированной поверхности.
// Create a deformed surface.
// ---
void CreateDeformedSurface()
{
  MbItem * surface = nullptr; // Базовая поверхность. // Basic surface.

  if ( GetGObj(IDS_SELECT_SURFACE, st_Surface, &surface) ) {
    TestVariables::viewManager->ToggleObject( surface );
    
    ptrdiff_t uDegree = Math::uSurfaceDegree;
    ptrdiff_t vDegree = Math::uSurfaceDegree;
    GetInt( IDS_ENTER_SPLINE_ORDER, uDegree );
    GetInt( IDS_ENTER_SPLINE_ORDER, vDegree );
    if (uDegree < 2 ) uDegree = Math::uSurfaceDegree;
    if (vDegree < 2 ) vDegree = Math::uSurfaceDegree;
    ptrdiff_t uCount = uDegree;
    ptrdiff_t vCount = vDegree;
    double distance_ = 0;
    
    if ( GetInt(IDS_ENTER_SPLIT_COUNT_U, uCount) && GetInt(IDS_ENTER_SPLIT_COUNT_V, vCount)
                                                 && GetDouble( IDS_ENTER_DISTANCE, distance_ )) {
      const MbSpaceItem * sItem = ((MbSpaceInstance *)surface)->GetSpaceItem();
      if ( (sItem != nullptr) && (sItem->Family() == st_Surface) ) {
        SPtr<MbSurface> s( static_cast<MbSurface *>(&sItem->Duplicate()) );
        MbSurface * surf( nullptr );
        ::DeformedSurface( *s, uCount, vCount, uDegree, vDegree, distance_, surf );
        TestVariables::viewManager->AddObject( TestVariables::SURFACE_Style, surf );
      }
    }

    TestVariables::viewManager->ToggleObject(surface);
  }
}


//------------------------------------------------------------------------------
// Создание расширенной поверхности.
// Create an extended surface.
// ---
void CreateExtendedSurface()
{
  MbItem *surface;

  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &surface ) ) {
    TestVariables::viewManager->ToggleObject(surface);
    double d = 1.0;
    if ( GetDouble( IDS_ENTER_STRETCHING, d )) {
      if ( fabs(d)>Math::lengthEpsilon ) {
        SPtr<MbSurface> s( static_cast<MbSurface *>(&((MbSpaceInstance *)surface)->GetSpaceItem()->Duplicate()) );
        double du1 = s->GetUMin()-d;
        double du2 = s->GetUMax()+d;
        double dv1 = s->GetVMin()-d;
        double dv2 = s->GetVMax()+d;

        MbSurface * surf( nullptr );
        ::ExtendedSurface( *s, du1, du2, dv1, dv2, surf );
        TestVariables::viewManager->AddObject( TestVariables::SURFACE_Style, surf );
      }
    }
    TestVariables::viewManager->ToggleObject(surface);
  }
}


//------------------------------------------------------------------------------
// Создание новой поверхности, усеченной кривыми на поверхности.
// Create a surface with the given boundary.
// ---
void CreateCurveBoundedSurface()
{
  MbItem * initSurface;
  MbItem * curve;
  RPArray<MbCurve>curves( 4, 1 );

  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &initSurface ) ) {
    TestVariables::viewManager->ToggleObject(initSurface);
    const MbSpaceItem * ssItem = ((MbSpaceInstance *)initSurface)->GetSpaceItem();
    if ( (ssItem != nullptr) && (ssItem->Family() == st_Surface) ) {
      const MbSurface * s = (const MbSurface *)ssItem;
      bool implicitOuter = GetBoolean( IDS_USE_BOUNDS_BOOL );
      if ( implicitOuter ) {
        curves.Add( &s->MakeContour( true ) ); // Граничный двумерный контур. // Border plane contour.
      }
      while ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &curve ) ) {
        TestVariables::viewManager->ToggleObject(curve);
        if ( (curve->IsA() == st_SpaceInstance) ) {
          const MbSpaceInstance * sInstance = static_cast<MbSpaceInstance *>(curve);
          const MbSpaceItem * sItem = sInstance->GetSpaceItem();
          if ( sItem != nullptr ) {
            if ( sItem->Type() == st_ContourOnSurface ) 
              curves.Add( static_cast<MbCurve *>( &((MbContourOnSurface *)sItem)->GetContour().Duplicate() ) );
            else
            if ( sItem->Type() == st_SurfaceCurve ) {
              const MbCurve & pCurve = ((MbSurfaceCurve *)sItem)->GetCurve();
              if ( pCurve.IsClosed() ) {
                MbContour * contour = new MbContour( pCurve, false );
                curves.Add( contour );
              }
            }
          }
        }
      }
      // Создать усеченную кривыми поверхность.
      // Create a surface with the given boundary.
      MbSurface * surf = 0;
      ::BoundedSurface( (MbSurface &)*s, curves, surf );
      TestVariables::viewManager->AddObject( TestVariables::SURFACE_Style, surf );
    }

    TestVariables::viewManager->ToggleObject(initSurface);
  }
}


//------------------------------------------------------------------------------
// Создание поверхности соединения.
// Create a join surface.
// ---
void CreateJoinSurface()
{
  //MbSurface * surface = ::MakeJoinSurface( *curve1, *curve2, p, checkIntersect, res );
  PRECONDITION( false );
}


//------------------------------------------------------------------------------
// Дать кривую на поверхности.
// Get curve on surface.
// ---
static
const MbSurfaceCurve * FindCurve( MbCurveEdge & edge, const MbVector3D & axisZ )
{
  const MbSurfaceIntersectionCurve & intCurve = edge.GetIntersectionCurve();
  const MbSurfaceCurve & curve1 = intCurve.GetCurveOne();
  const MbSurfaceCurve & curve2 = intCurve.GetCurveTwo();
  MbFace * faceP = edge.GetFacePlus();
  MbFace * faceM = edge.GetFaceMinus();

  if ( faceP != nullptr && faceM == nullptr ) {
    const MbSurface * surface = &faceP->GetSurface().GetSurface();
    if ( surface == &curve1.GetSurface() )
      return &curve1;
    if ( surface == &curve2.GetSurface() )
      return &curve2;
  }
  if ( faceM != nullptr && faceP == nullptr ) {
    const MbSurface * surface = &faceM->GetSurface().GetSurface();
    if ( surface == &curve1.GetSurface() )
      return &curve1;
    if ( surface == &curve2.GetSurface() )
      return &curve2;
  }

  MbVector3D vec1, vec2;
  double t = intCurve.GetTMid();
  curve1.SurfaceNormal( t, vec1 );
  curve2.SurfaceNormal( t, vec2 );
  if ( ::fabs(vec1 * axisZ) > ::fabs(vec2 * axisZ) )
    return &intCurve.GetCurveOne();
  else
    return &intCurve.GetCurveTwo();
}


//------------------------------------------------------------------------------
// Поверхность переменного сечения. Mutable section surface construction.
// ---
void CreateSectionSurface()
{
  MbItem * obj  = nullptr;
  MbItem * objCurve  = nullptr;
  MbTopItem * topoItem1 = nullptr, * topoItem2 = nullptr;
  PathMatrix pathFrom0, pathFrom1, pathFrom2;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &objCurve ) ) {
    const MbCurve3D * reference( nullptr );
    if ( objCurve!= nullptr && objCurve->IsA() == st_SpaceInstance ) {
      const MbSpaceItem * sItem = static_cast<MbSpaceInstance *>( objCurve )->GetSpaceItem();
      if ( sItem != nullptr && sItem->Family() == st_Curve3D )
        reference = static_cast<const MbCurve3D *>( sItem );
    }

    if ( reference != nullptr) {
      const MbCurve3D * curve1( nullptr );
      const MbCurve3D * curve2( nullptr );

      if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &topoItem1 ) &&
           GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &topoItem2 ) ) {
        MbCurveEdge * edge1 = (MbCurveEdge *)topoItem1;
        MbCurveEdge * edge2 = (MbCurveEdge *)topoItem2;

        if ( (edge1 != nullptr) && (edge2 != nullptr) ) {
          MbVector3D axisZ( TestVariables::cursorAxis.GetAxisZ() );
          curve1 = ::FindCurve( *edge1, axisZ );
          curve2 = ::FindCurve( *edge2, axisZ );
        }
      }
      else {
        if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) ) {
          if ( obj->IsA() == st_SpaceInstance ) {
              const MbSpaceInstance * sInstance = static_cast<const MbSpaceInstance *>( obj );
            if ( sInstance != nullptr && sInstance->GetSpaceItem()->Family() == st_Curve3D ) {
              curve1 = (const MbCurve3D *)sInstance->GetSpaceItem();
              curve2 = curve1;
            }
          }
        }
      }

      if ( (curve1 != nullptr) && (curve2 != nullptr) ) {
        SetWaitCursor( true );

        int form = ( curve1 != curve2 ) ? 2 : 0;
        ::GetInt( IDS_SPECIFY_SURFACE_SECTION, form );

        double uBeg = 0.0; 
        double uEnd = ( curve1 == curve2 ) ? M_PI2 : 1.0; 
        if ( form != 4 )
          GetDouble( IDS_ENTER_FINAL_PRM, uEnd );
        MbFunction * function( nullptr );
        double d1 = 0.4142, d2 = 0.4142;
        if ( form == 0 ) {
          double len = reference->GetMetricLength();
          d1 = len * DELTA_MOD;
          d2 = d1;
        }
        MbCurve * pattern( nullptr );
        if ( form > 3 ) {
          MbItem * pObj  = nullptr;
          if ( GetMObj( IDS_SELECT_CURVE, pt_Curve, &pObj ) ) {
            const MbPlaneItem * pItem = static_cast<MbPlaneInstance *>( pObj )->GetPlaneItem( 0 );
            if ( pItem != nullptr && pItem->Family() == pt_Curve ) {
              pattern = (MbCurve *)&( pItem->Duplicate() );
              uBeg = pattern->GetTMin(); 
              uEnd = pattern->GetTMax();
            }
          }
        }
        else {
          if ( GetDouble( IDS_ENTER_SCALE, d1 ) &&
               GetDouble( IDS_ENTER_SCALE, d2 ) ) {
            double st1 = reference->GetTMin(), st2 = reference->GetTMax();
            if ( ::fabs(d1) > EPSILON && ::fabs(d2) > EPSILON )
              function = new MbLineFunction( d1, d2, st1, st2 ); 
          }
          else {
            if ( GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &obj) && obj->IsA() == st_PlaneInstance ) {
              const MbPlaneItem * planeItem = static_cast<const MbPlaneInstance *>( obj )->GetPlaneItem();
              if ( planeItem != nullptr && planeItem->Family() == pt_Curve ) {
                const MbCurve * curve = static_cast<const MbCurve *>( planeItem );
                function = ( ::CreateFunction( *curve, MbFunctionParameters()) ).detach();
              }
            }
          }
        }
        bool sense = true; // \ru Направление нормали поверхности направляющей кривой (для guide1==guide2). \en The normal direction of guide curve surface (for guide1==guide2). 
        MbSurface * surface( nullptr );

        if ( (curve1 != nullptr) && (curve2 != nullptr) )
          ::SectionSurface( *reference, *curve1, *curve2, nullptr, form, sense,
                            uBeg, uEnd, function, pattern, METRIC_EPSILON, Math::DefaultMathVersion(), surface );

        if ( surface != nullptr ) {
          MbSolid * solid( nullptr );
          //SweptValues p( 0.0, 0.0, false );
          //MbSNameMaker names( ct_ThinShellCreator, MbSNameMaker::i_SideNone, 0/*buttAdd*/ );
          //SimpleName name(0); 
          //::ThinSolid( *surface, true, p, names, name, solid ); 
          if ( solid != nullptr)
            TestVariables::viewManager->AddObject( TestVariables::GOLDEN_Style, solid );
          else
            TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, surface ); 
        }
        ::DeleteItem( pattern );
        ::DeleteItem( surface );
        SetWaitCursor( false );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Построение поверхности Кунса касательной к четырём кривым на поверхностях.
// The construction of Coons surface tangent to four curves on surfaces.
// ---
void CoonsPatchSurface()
{
  MbItem * obj  = nullptr;
  MbTopItem * topoItem0 = nullptr, * topoItem1 = nullptr, * topoItem2 = nullptr, * topoItem3 = nullptr;

  if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &topoItem0 ) &&
       GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &topoItem1 ) &&
       GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &topoItem2 ) &&
       GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &topoItem3 ) ) {
    SetWaitCursor( true );
    MbCurveEdge * edge0 = (MbCurveEdge *)topoItem0;
    MbCurveEdge * edge1 = (MbCurveEdge *)topoItem1;
    MbCurveEdge * edge2 = (MbCurveEdge *)topoItem2;
    MbCurveEdge * edge3 = (MbCurveEdge *)topoItem3;

    if ( (edge0 != nullptr) && (edge1 != nullptr) && (edge2 != nullptr) && (edge3 != nullptr) &&
         (edge0 != edge1) && (edge0 != edge2) && (edge0 != edge3) && (edge1 != edge2) && (edge1 != edge3) && (edge2 != edge3) ) {
      if ( TestVariables::viewManager->GetActiveWindow() != nullptr ) {
        MbVector3D axisZ( TestVariables::cursorAxis.GetAxisZ() );
        const MbSurfaceCurve * curve0 = ::FindCurve( *edge0, axisZ );
        const MbSurfaceCurve * curve1 = ::FindCurve( *edge1, axisZ );
        const MbSurfaceCurve * curve2 = ::FindCurve( *edge2, axisZ );
        const MbSurfaceCurve * curve3 = ::FindCurve( *edge3, axisZ );
        MbSurface * surface( nullptr );

        if ( (curve0 != nullptr) && (curve1 != nullptr) && (curve2 != nullptr) && (curve3 != nullptr) )
          ::CreateCoonsSurface( *curve0, *curve1, *curve2, *curve3, surface );

        if ( surface != nullptr ) {
          MbSolid * solid( nullptr );
          //SweptValues p( 0.0, 0.0, false );
          //MbSNameMaker names( ct_ThinShellCreator, MbSNameMaker::i_SideNone, 0/*buttAdd*/ );
          //SimpleName name(0); 
          //::ThinSolid( *surface, true, p, names, name, solid ); 
          if ( solid != nullptr)
            TestVariables::viewManager->AddObject( TestVariables::SHELL_Style, solid );
          else
            TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, surface ); 
        }
        ::DeleteItem( surface );
      }
    }
    SetWaitCursor( false );
  }
}


//------------------------------------------------------------------------------
// Построение поверхности-заплатки для заданных рёбер касательной к четырём кривым на поверхностях.
// The construction of a surface-patch by the edges.
// ---
void SplinePatchSurface()
{
  MbItem * obj  = nullptr;
  MbItem * obj0 = nullptr;
  MbTopItem * curve( nullptr );
  MbMatrix3D matr;
  std::vector<const MbCurveEdge *> edges; 

  if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &curve, matr ) ) {
    TestVariables::viewManager->ToggleObject(obj);
    if ( curve != nullptr && curve->IsA() == tt_CurveEdge ) {
      MbCurveEdge * edge = static_cast<MbCurveEdge *> (curve);
      edges.push_back( edge );
      while ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj0, &curve, TestVariables::itemPathFrom.second ) ) {
        TestVariables::viewManager->ToggleObject(obj);
        TestVariables::viewManager->ToggleObject(obj0);
        obj = obj0;
        if ( curve != nullptr && curve->IsA() == tt_CurveEdge ) {
          edge = static_cast<MbCurveEdge *> (curve);
          edges.push_back( edge );
        }
      }
      TestVariables::viewManager->ToggleObject(obj0);
    }
  }

  if ( edges.size() > 0 ) {
    SetWaitCursor( true );

    std::vector<MbSurface *> surfaces;
    MbResultType res = ::CreateSplinePatch( edges, surfaces );

    for ( size_t i = 0, iCount = surfaces.size(); i < iCount; i++ ) {
      MbSurface * surface = surfaces[i];
      if ( surface != nullptr ) {
        MbSolid * solid( nullptr );
        SweptValues p( 0.0, 0.0, false );
        MbSNameMaker names( ct_ThinShellCreator, MbSNameMaker::i_SideNone, 0/*buttAdd*/ );
        SimpleName name(0); 
        ::ThinSolid( *surface, true, p, names, name, solid ); 
        if ( solid != nullptr)
          TestVariables::viewManager->AddObject( TestVariables::SHELL_Style, solid );
        else
          TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, surface ); 
        if ( res != rt_Success )
          ::PutErrorMessage( res, IDS_TITLE_ERR );
      }
      ::DeleteItem( surface );
    }
    SetWaitCursor( false );
  }
}


//------------------------------------------------------------------------------
// Создание поверхности на точках триангуляции.
// Create a grid surface (surface based on triangulation).
// ---
void CreateGridSurface()
{
  MbItem * obj  = nullptr;
  MbTopItem * topoItem = nullptr;

  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &topoItem ) ) {
    MbSolid * solid = (MbSolid *)obj;
    MbFace * face = (MbFace *)topoItem;

    if ( (solid != nullptr) && (face != nullptr) ) {
      MbCube cube;
      solid->AddYourGabaritTo( cube );
      MbStepData stepData;
      double diagonal = cube.GetDiagonal();
      stepData.SetSag( diagonal / DELTA_MAX );
      stepData.SetLength( diagonal / NEWTON_COUNT_2X );

      // Запросить параметры для триангуляции.
      // Input parameters for the triangulation.
      int mode = TestVariables::operateType;
      ::InitStepData( stepData, true, mode );

      SetWaitCursor( true );

      MbFloatGrid grid; // MbExactGrid grid;
      ::CalculateGrid( *face, stepData, grid, true, false, false );

      MbSurface * surface = nullptr;
      ::GridSurface( grid, surface );

      if ( surface != nullptr )
        TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, surface ); 

      SetWaitCursor( false );
    }
  }
}


//------------------------------------------------------------------------------
// Построение развёртки поверхности на плоскость.
// Construction of a surface sweep on a plane.
// ---
void RectifiedSurface()
{
  MbItem * obj  = nullptr;
  MbTopItem * topoItem = nullptr;

  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &topoItem ) ) {
    MbSolid * solid = (MbSolid *)obj;
    MbFace * face = (MbFace *)topoItem;
    MbPlacement3D place;
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow(); // Вернуть указатель на активное окно.
    if ( wind != nullptr ) {
      MbVector3D norm( wind->GetAxisZ() );
      MbVector3D aX( wind->GetAxisX() );
      place.Init( TestVariables::cursorPoint, norm, aX );
    }

    if ( (solid != nullptr) && (face != nullptr) ) {
      MbCube cube;
      face->AddYourGabaritTo( cube );
      double visSag = cube.GetDiagonal() * ONE_FIFTH / POWER_3;
      double devSag = Math::deviateSag * ONE_HALF;
      int in = 0;

      if ( GetDouble( IDS_DEVIATE_TOL, devSag ) )
        TestVariables::deviateAngle = devSag;
      else {
        devSag = TestVariables::deviateAngle;
        if ( GetDouble( IDS_DEVIATE_TOL, devSag ) )
          TestVariables::deviateAngle = devSag;
      }

      if ( GetDouble( IDS_VISUAL_TOL, visSag ) )
        TestVariables::visualSag = visSag;
      else {
        visSag = TestVariables::visualSag;
        if ( GetDouble( IDS_VISUAL_TOL, visSag ) )
          TestVariables::visualSag = visSag;
      }
      GetInt( IDS_ENTER_TRACING_MODE, in );

      MbStepData stepData( ist_DeviationStep, devSag );
      stepData.SetStepType( ist_ParamStep, true ); // Запомнить параметрические точки.
      stepData.SetSag( visSag );
      const MbSurface & surf = face->GetSurface().GetSurface();
      MbCartPoint init;
      MbCartPoint3D pnt, origin( place.GetOrigin() );
      MbVector3D uDer, vDer, norm( place.GetAxisZ() );
      MbSolid * result( nullptr );

      surf.NearDirectPointProjection( origin, norm, init.x, init.y, false, nullptr );
      surf.Explore( init.x, init.y, true, pnt, uDer, vDer, nullptr, nullptr, nullptr, nullptr );
      uDer = uDer.NormalComponent( norm );
      vDer = vDer.NormalComponent( norm );
      norm.SetVecM( uDer, vDer );
      if ( !face->IsSameSense() )
        norm.Invert();
      place.Init( pnt, norm, uDer, false );
      double myu = 0.25; // Коэффициент Пуассона материала грани. The Poisson's ratio of face material.
      bool faceted = in > 0; // Построить мозаичный объект (true) или одну плоскую грань (false). Build a mosaic object (true), or one flat face (false).
      RectifyValues values( place, init, stepData, myu, faceted );

      SetWaitCursor( true );

      ::RectifyFace( *face, values, result );

      if ( result != nullptr ) {
        TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, result );
      }

      SetWaitCursor( false );
      ::DeleteItem( result );
    }
  }
}
