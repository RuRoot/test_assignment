#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <test_draw.h>
#include <cur_line3d.h>
#include <cur_line_segment3d.h>
#include <surf_plane.h>
#include <cur_plane_curve.h>
#include <cur_arc3d.h>
#include <surf_curve_bounded_surface.h>
#include <action_surface.h>
#include <space_instance.h>
#include <plane_instance.h>
#include <last.h>

using namespace c3d;
using namespace TestVariables;


//------------------------------------------------------------------------------
// Создание плоскости.
// Create a plane.
// ---
void CreatePlane()
{
  MbCartPoint3D p;

  while (  GetPoint( IDS_SELECT_POINT, p ) ) {
//#ifdef _DRAWGI
    drawImp->DrawPoint( &p, TRGB_LIGHTGREEN );
//#endif // _DRAWGI
    WorkWindow *currentView = viewManager->GetActiveWindow();
    MbPlacement3D place( currentView->GetNullPlacement() );
    place.SetOrigin( p );
    MbPlane *plane = new MbPlane( place );

    viewManager->AddObject( ELEMENTARY_Style, plane );
  }

}


//------------------------------------------------------------------------------
// Создание плоскости.
// Create a plane by three points.
// ---
void CreatePlane3Points() {
  MbCartPoint3D p[3];

  while ( GetPoint( IDS_SELECT_POINT, p[0] ) ) {
//#ifdef _DRAWGI
    drawImp->DrawPoint( &p[0], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
    if ( GetPoint( IDS_SELECT_POINT, p[1] ) ) {
//#ifdef _DRAWGI
      drawImp->DrawPoint( &p[1], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
      if ( GetPoint( IDS_SELECT_POINT, p[2] ) ) {
//#ifdef _DRAWGI
        drawImp->DrawPoint( &p[2], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
        MbSurface * surf = 0;
        ::ElementarySurface( p[0], p[1], p[2], st_Plane, surf );
        viewManager->AddObject( ELEMENTARY_Style, surf );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание плоскости по ребру параллельно оси координат.
// Create a plane by edge and parallel to the coordinate axis. 
// ---
void CreateEdgePlane()
{
  MbItem * obj = nullptr;
  if ( GetAnyCurve( IDS_SELECT_CURVE, &obj ) ) {
    viewManager->ToggleObject(obj);
    if ( obj != nullptr ) {
      MbCurve3D * curve = nullptr;
      
      if ( obj->IsA() == st_SpaceInstance ) {
        const MbSpaceItem * spaceItem = ((MbSpaceInstance *)obj)->GetSpaceItem();
        if ( (spaceItem != nullptr) && (spaceItem->Family() == st_Curve3D) ) {
          curve = (MbCurve3D *)spaceItem;
        }
      }
      if ( obj->IsA() == st_PlaneInstance ) {
        const MbPlaneItem * planeItem = ((MbPlaneInstance *)obj)->GetPlaneItem();
        if ( (planeItem != nullptr) && (planeItem->Family() == pt_Curve) ) {
          MbPlacement3D place = ((MbPlaneInstance *)obj)->GetPlacement();
          const MbCurve * planeCurve = static_cast<const MbCurve *>(planeItem);
          curve = new MbPlaneCurve( place, *planeCurve, true );
        }
      }    
      
      if ( curve != nullptr ) {
        curve->AddRef();
        if ( curve->IsStraight() ) {
          int axisNumber = 0;
          GetInt( IDS_ENTER_AXIS_IND, axisNumber ); // Номер оси. // Axis number.
          if ( axisNumber != 0/*nullptr*/ ) {
            MbVector3D axis;
            switch ( axisNumber ) {
              case 1:
                axis.Init( 1.0, 0.0, 0.0 );
                break;
              case 2:
                axis.Init( 0.0, 1.0, 0.0 );
                break;
              default:
                axis.Init( 0.0, 0.0, 1.0 );
                break;     
            }
            MbCartPoint3D p1 = curve->GetLimitPoint( 1 );
            MbCartPoint3D p2 = curve->GetLimitPoint( 2 );
            MbCartPoint3D p3 = p2;
            p3.Move( axis );

            MbSurface * surf = 0;
            ::ElementarySurface( p1, p2, p3, st_Plane, surf );
            viewManager->AddObject( ELEMENTARY_Style, surf ); 
          }
        } 
        curve->Release();
      }
    }
    viewManager->ToggleObject(obj);
  }
}


//------------------------------------------------------------------------------
// Создание плоскости по плоской кривой
// ---
bool CreatePlaneByCurve()
{
  bool res = false;
  PathMatrix pathFrom;
  SpaceCurveSPtr spaceCurve( SelectAnyCurveCopy( pathFrom ) );

  if ( spaceCurve != nullptr ) {
    TestVariables::viewManager->ToggleObject( pathFrom.first );

    if ( !pathFrom.second.IsSingle() )
      spaceCurve->Transform( pathFrom.second );

    PlaneCurveSPtr pCurve;
    MbPlacement3D place;

    if ( spaceCurve->GetPlaneCurve( pCurve, place, false ) ) {
      MbRect rect;
      pCurve->AddYourGabaritTo( rect );

      SPtr<MbPlane> plane;

      plane = new MbPlane( place );
      plane->SetLimit( rect.GetXMin(), rect.GetYMin(), rect.GetXMax(), rect.GetYMax() );

      TestVariables::viewManager->AddObject( TestVariables::ELEMENTARY_Style, plane );
      res = true;
    }

    TestVariables::viewManager->ToggleObject( pathFrom.first );
  }

  return res;
}


//------------------------------------------------------------------------------
// Создание цилиндра.
// Create a cylinder surface.
// ---
void CreateCylinder3Points() {
  MbCartPoint3D p[3];

  while ( GetPoint( IDS_SELECT_POINT, p[0] ) ) {
//#ifdef _DRAWGI
    drawImp->DrawPoint( &p[0], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
    if ( GetPoint( IDS_SELECT_POINT, p[1] ) ) {
//#ifdef _DRAWGI
      drawImp->DrawPoint( &p[1], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
      if ( GetPoint( IDS_SELECT_POINT, p[2] ) ) {
//#ifdef _DRAWGI
        drawImp->DrawPoint( &p[2], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
        MbSurface * surf = 0;
        ::ElementarySurface( p[0], p[1], p[2], st_CylinderSurface, surf );
        viewManager->AddObject( ELEMENTARY_Style, surf );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание конуса.
// Create a cone surface.
// ---
void CreateCone3Points() {
  MbCartPoint3D p[3];

  while ( GetPoint( IDS_SELECT_POINT, p[0] ) ) {
//#ifdef _DRAWGI
    drawImp->DrawPoint( &p[0], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
    if ( GetPoint( IDS_SELECT_POINT, p[1] ) ) {
//#ifdef _DRAWGI
      drawImp->DrawPoint( &p[1], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
      if ( GetPoint( IDS_SELECT_POINT, p[2] ) ) {
//#ifdef _DRAWGI
        drawImp->DrawPoint( &p[2], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
        MbSurface * surf = 0;
        ::ElementarySurface( p[0], p[1], p[2], st_ConeSurface, surf );
        viewManager->AddObject( ELEMENTARY_Style, surf );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание сферы.
// Create a sphere surface.
// ---
void CreateSphere3Points() {
  MbCartPoint3D p[3];

  while ( GetPoint( IDS_SELECT_POINT, p[0] ) ) {
//#ifdef _DRAWGI
    drawImp->DrawPoint( &p[0], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
    if ( GetPoint( IDS_SELECT_POINT, p[1] ) ) {
//#ifdef _DRAWGI
      drawImp->DrawPoint( &p[1], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
      if ( GetPoint( IDS_SELECT_POINT, p[2] ) ) {
//#ifdef _DRAWGI
        drawImp->DrawPoint( &p[2], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
        MbSurface * surf = 0;
        ::ElementarySurface( p[0], p[1], p[2], st_SphereSurface, surf );
        viewManager->AddObject( ELEMENTARY_Style, surf );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание тора
// Create a torus surface.
// ---
void CreateTorus3Points() {
  MbCartPoint3D p[3];

  while ( GetPoint( IDS_SELECT_POINT, p[0] ) ) {
//#ifdef _DRAWGI
    drawImp->DrawPoint( &p[0], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
    if ( GetPoint( IDS_SELECT_POINT, p[1] ) ) {
//#ifdef _DRAWGI
      drawImp->DrawPoint( &p[1], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
      if ( GetPoint( IDS_SELECT_POINT, p[2] ) ) {
//#ifdef _DRAWGI
        drawImp->DrawPoint( &p[2], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
        MbSurface * surf = 0;
        ::ElementarySurface( p[0], p[1], p[2], st_TorusSurface, surf );
        viewManager->AddObject( ELEMENTARY_Style, surf );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание поверхности выдавливания.
// Create an extrusion surface.
// ---
void CreateExtrusionSurface()
{
  MbItem * curveItem = nullptr; // Образующая. // Generating curve.

  if ( GetAnyCurve( IDS_SELECT_GENER_CURVE, &curveItem ) ) {
    viewManager->ToggleObject( curveItem );

    MbItem * axisItem = nullptr;   // Вектор движения. // Direction vector.
    MbVector3D vect;

    if ( GetGObj( IDS_SELECT_GUIDE_LINE, st_Curve3D, &axisItem ) && (axisItem->IsA() == st_SpaceInstance) ) {
      viewManager->ToggleObject( axisItem );
      
      if ( ((MbSpaceInstance *)axisItem)->GetSpaceItem() != nullptr && (((MbSpaceInstance *)axisItem)->GetSpaceItem()->Family() == st_Curve3D) ) {
        const MbCurve3D * axisCurve = (const MbCurve3D *)(((MbSpaceInstance *)axisItem)->GetSpaceItem());
        if ( axisCurve->IsStraight() )
          axisCurve->GetLimitTangent( 1, vect );
      }
    }

    SPtr<MbCurve3D> crv( ::GetCurve3D( curveItem, false ) );

    if ( crv != nullptr ) {
      if ( vect.Length() < LENGTH_EPSILON ) {
        MbPlacement3D crvPlace;
        if ( crv->GetPlacement( crvPlace ) )
          vect = crvPlace.GetAxisZ();
        else {
          WorkWindow * vc = viewManager->GetActiveWindow();
          if ( vc != nullptr )
            vect = vc->GetAxisZ();
        }
      }  
      if ( vect.Length() > LENGTH_EPSILON ) {
        double length = 50.0;
        if ( GetDouble( IDS_ENTER_SURF_LENGHT, length )) {
          vect.Normalize();
          vect *= length;
          bool simplify = true;
          MbSurface * surf = nullptr;
          ::ExtrusionSurface( *crv, vect, simplify, surf );
          if ( surf != nullptr )
            viewManager->AddObject( SWEPT_Style, surf );
        }
      }
    }

    if ( axisItem != nullptr )
      viewManager->ToggleObject( axisItem );

    viewManager->ToggleObject( curveItem );
  }
}


//------------------------------------------------------------------------------
// Создание поверхности вращения.
// Create a revolution surface.
// ---
void CreateRevolutionSurface() {
  MbItem  *curve; // Образующая. // Generating curve.
  MbItem  *axis = nullptr;   // Ось вращения. // Rotation axis.
  MbVector3D vect(0,0,0);
  MbCartPoint3D orig, p;

  if ( GetAnyCurve( IDS_SELECT_GENER_CURVE, &curve ) ) {
    viewManager->ToggleObject(curve);

    if ( GetPoint( IDS_SELECT_ROTATE_AXIS, orig ) ) {
      if ( FindNearestObj( orig, st_Line3D, &axis ) ) {
        const MbSpaceItem * geom = ((MbSpaceInstance *)axis)->GetSpaceItem();
        viewManager->ToggleObject(axis);
        orig = ((MbLine3D *)geom)->GetOrigin();
        vect = ((MbLine3D *)geom)->GetDirection();
      }
      else {
        if ( GetPoint( IDS_SELECT_POINT, p )) 
          vect.Init(orig,p);
      }
    }
    if (vect.Length()>Math::lengthEpsilon) {
      double angle = 360;
      if ( GetDouble( IDS_ENTER_ANGLE, angle )) {
        if(fabs(angle-360)<PARAM_EPSILON) angle = M_PI2;
        else angle = angle*M_PI2/360;

        SPtr<MbCurve3D> crv( ::GetCurve3D( curve, false ) );
        bool simplify = true;

        MbSurface * surf = nullptr;
        ::RevolutionSurface( *crv, orig, vect, angle, simplify, surf );
        if ( surf != nullptr )
          viewManager->AddObject( SWEPT_Style, surf );
      }
    }
    if ( axis )
      viewManager->ToggleObject(axis);
    viewManager->ToggleObject(curve);
  }
}


//------------------------------------------------------------------------------
// Создание кинематической (или спиральной) поверхности.
// Create an evolution (or spiral) surface.
// ---
void CreateEvolutionSurface() {
  MbItem *curve;   // Образующая. // Generating curve.
  MbItem *spine;   // Направляющая. // Guide curve.

  if ( GetAnyCurve( IDS_SELECT_GENER_CURVE, &curve ) ) {
    viewManager->ToggleObject(curve);
   
    if ( GetAnyCurve( IDS_SELECT_GUIDE_CURVE, &spine ) ) {
      viewManager->ToggleObject(spine);
      SPtr<MbCurve3D> crv( ::GetCurve3D( curve, false ) );
      SPtr<MbCurve3D> spn( ::GetCurve3D( spine, false ) );

      MbSurface * surf = nullptr;
      ::EvolutionSurface( *crv, *spn, surf );
      viewManager->AddObject( SWEPT_Style, surf );

      viewManager->ToggleObject(spine);
    }

    viewManager->ToggleObject(curve);
  }
}


//------------------------------------------------------------------------------
// Создание спиральной поверхности.
// Create a spiral surface.
// ---
void CreateSpiralSurface() { 
  MbItem *curve;    // Образующая. // Generating curve.

  if ( GetAnyCurve( IDS_SELECT_GENER_CURVE, &curve ) ) {
    viewManager->ToggleObject(curve);

    MbCartPoint3D p[3]; // Точки на оси. // Points on the axis.
    ptrdiff_t i = 0;

    while ( i < 3 && GetPoint( IDS_SELECT_POINT, p[i] ) ) {
//#ifdef _DRAWGI
      drawImp->DrawPoint( &p[i], TRGB_LIGHTGREEN );
//#endif
      i++;
    }

    if ( i == 3 ) {
      double d = 20;
      if ( GetDouble( IDS_ENTER_STEP, d ) ) {
        if ( fabs(d)>Math::lengthEpsilon ) {
          SPtr<MbCurve3D> crv( ::GetCurve3D( curve, false ) );

          MbSurface * surf = nullptr;
          ::SpiralSurface( *crv, true, p[0], p[1], p[2], d, surf );
          viewManager->AddObject( SWEPT_Style, surf );
        }
      }
    }

    viewManager->ToggleObject(curve);
  }
}


//------------------------------------------------------------------------------
// Создание кинематической поверхности.
// Create an expansion surface.
// ---
void CreateExpansionSurface() {
  MbItem * curve( nullptr );   // Образующая. // Generating curve.
  MbItem * spine( nullptr );   // Направляющая. // Guide curve.

  if ( GetAnyCurve( IDS_SELECT_GENER_CURVE, &curve ) ) {
    viewManager->ToggleObject(curve);

    if ( GetAnyCurve( IDS_SELECT_GUIDE_CURVE, &spine ) ) {
      viewManager->ToggleObject(spine);
      SPtr<MbCurve3D> crv( ::GetCurve3D( curve, false ) );
      SPtr<MbCurve3D> spn( ::GetCurve3D( spine, false ) );
      MbItem * curve1( nullptr );   // Образующая. // Generating curve.
      SPtr<MbCurve3D> crv1( nullptr );
      GetAnyCurve( IDS_SELECT_GENER_CURVE, &curve1 );

      if ( curve1 != nullptr )
        crv1 = ::GetCurve3D( curve1, false );

      MbSurface * surf = 0;
      ::ExpansionSurface( *crv, *spn, crv1, surf );
      viewManager->AddObject( SWEPT_Style, surf );

      viewManager->ToggleObject(spine);
    }

    viewManager->ToggleObject(curve);
  }
}


//------------------------------------------------------------------------------
// Создание линейчатой поверхности.
// Create a ruled surface.
// ---
void CreateRuledSurface()
{
  MbItem * curves[2]; // Две кривые. // Two curves.

  if ( GetAnyCurve( IDS_SELECT_FIRST_CURVE, &curves[0] ) ) {
    viewManager->ToggleObject(curves[0]);

    if ( GetAnyCurve( IDS_SELECT_SECOND_CURVE, &curves[1] ) ) {
      viewManager->ToggleObject(curves[1]);

      SPtr<MbCurve3D> crv1( ::GetCurve3D( curves[0], false ) );
      SPtr<MbCurve3D> crv2( ::GetCurve3D( curves[1], false ) );
      bool simplify = true;

      MbSurface * surf = nullptr;
      ::RuledSurface( *crv1, *crv2, simplify, surf );
      viewManager->AddObject( SURFACE_Style, surf );

      viewManager->ToggleObject(curves[1]);
    }

    viewManager->ToggleObject(curves[0]);
  }
}


//------------------------------------------------------------------------------
// Создание секториальной поверхности.
// Create a sectorial surface.
// ---
void CreateSectorSurface()
{
  MbCartPoint3D point;
  MbItem * curve;

  bool c = GetAnyCurve( IDS_SELECT_CURVE, &curve );
  if ( c ) viewManager->ToggleObject(curve);

  bool p = GetPoint( IDS_SELECT_POINT, point );
  if ( p ) drawImp->DrawPoint( &point, TRGB_LIGHTGREEN );

  SPtr<MbCurve3D> crv( ::GetCurve3D( curve, false ) );

  if ( crv && p ) {
    MbSurface * surf = nullptr;
    ::SectorSurface( (MbCurve3D &)crv->Duplicate(), point, surf );
    viewManager->AddObject( SURFACE_Style, surf );
  }
  if ( crv && !p ) {
    MbCartPoint3D p0, p1;
    double        t0, t1;
    t0 = crv->GetTMin();
    t1 = crv->GetTMax();
    crv->PointOn(t0,p0);
    crv->PointOn(t1,p1);

    if (p0==p1)
      crv->GetWeightCentre( point );
    else   point = (p0 + p1) * 0.5;

    MbSurface * surf = nullptr;
    ::SectorSurface( (MbCurve3D &)crv->Duplicate(), point, surf );
    viewManager->AddObject( SURFACE_Style, surf );
  }

  if ( !crv && p ) {
    double radius=10;
    if ( GetDouble( IDS_ENTER_RADIUS, radius ) ) {
      WorkWindow *currentView = viewManager->GetActiveWindow();
      MbPlacement3D plane( currentView->GetNullPlacement() );
      plane.SetOrigin( point );
      MbArc3D * crv_ = new MbArc3D( plane, radius, radius, M_PI2 );

      MbSurface * surf = nullptr;
      ::SectorSurface( *crv_, point, surf );
      viewManager->AddObject( SURFACE_Style, surf );
    }
  }

  if ( c ) viewManager->ToggleObject(curve);
}


//------------------------------------------------------------------------------
// Создание NURBS поверхности по точкам.
// Create a NURBS-surface given points.
// ---
void CreateSplineSurface()
{
  MbCartPoint3D p;
  SArray<MbCartPoint3D> pList;

  while ( GetPoint( IDS_SELECT_POINT, p ) ) {
    drawImp->DrawPoint( &p, TRGB_LIGHTGREEN );
    pList.push_back( p );
  }

  ptrdiff_t uCount = pList.size();
  while ( GetPoint( IDS_SELECT_POINT, p ) ) {
    drawImp->DrawPoint( &p, TRGB_LIGHTGREEN );
    pList.push_back( p );
  }

  ptrdiff_t vCount = pList.size()/uCount;

  if ( uCount > 1 && vCount > 1 ) {
    ptrdiff_t uDegree = Math::uSurfaceDegree;
    ptrdiff_t vDegree = Math::uSurfaceDegree;
    GetInt( IDS_ENTER_SPLINE_ORDER, uDegree );
    GetInt( IDS_ENTER_SPLINE_ORDER, vDegree );
    uDegree = std_max( (size_t)2, (size_t)Math::uSurfaceDegree );
    vDegree = std_max( (size_t)2, (size_t)Math::uSurfaceDegree );
    bool ucl = false;
    bool vcl = false;
    SArray<double> initW;
    SArray<double> initUk;
    SArray<double> initVk;

    MbSurface * surf = nullptr;
    ::SplineSurface( pList, initW, uCount, vCount, uDegree, initUk, ucl, vDegree, initVk, vcl, surf );
    viewManager->AddObject( SURFACE_Style, surf );
  }
  pList.clear();
}


//------------------------------------------------------------------------------
// Создание четырехугольной NURBS поверхности.
// Create a quadrangle NURBS - surface.
// ---
void CreateQuadrangleNURBSSurface()
{
  MbCartPoint3D p1, p2, p3, p4;

  while ( GetPoint( IDS_SELECT_POINT, p1 ) ) {
    drawImp->DrawPoint( &p1, TRGB_LIGHTGREEN );

    if ( GetPoint( IDS_SELECT_POINT, p2 ) ) {
      drawImp->DrawPoint( &p2, TRGB_LIGHTGREEN );

      if ( GetPoint( IDS_SELECT_POINT, p3 ) ) {
        drawImp->DrawPoint( &p3, TRGB_LIGHTGREEN );

        if ( GetPoint( IDS_SELECT_POINT, p4 ) ) {
          drawImp->DrawPoint( &p4, TRGB_LIGHTGREEN );

          ptrdiff_t uDegree = Math::uSurfaceDegree;
          ptrdiff_t vDegree = Math::uSurfaceDegree;
          GetInt( IDS_ENTER_SPLINE_ORDER, uDegree );
          GetInt( IDS_ENTER_SPLINE_ORDER, vDegree );
          if (uDegree < 2 ) uDegree = Math::uSurfaceDegree;
          if (vDegree < 2 ) vDegree = Math::uSurfaceDegree;
          ptrdiff_t uCount = uDegree;
          ptrdiff_t vCount = vDegree;

          if ( GetInt( IDS_ENTER_SPLIT_COUNT_U, uCount ) && GetInt( IDS_ENTER_SPLIT_COUNT_V, vCount ) ) {
            MbSurface * surf = nullptr;
            ::SplineSurface( p1, p2, p3, p4, uCount, vCount, uDegree, vDegree, surf );
            viewManager->AddObject( SURFACE_Style, surf );
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание поверхности по трем кривым.
// Create a corner surface.
// ---
void CreateCornerSurface()
{
  MbItem  *object1, *object2, *object3;

  SPtr<MbCurve3D> curve1, curve2, curve3;

  if ( GetAnyCurve( IDS_SELECT_CURVE, &object1 ) ) {
    viewManager->ToggleObject(object1);
    if ( GetAnyCurve( IDS_SELECT_CURVE, &object2 ) ) {
      viewManager->ToggleObject(object2);
      if ( GetAnyCurve( IDS_SELECT_CURVE, &object3 ) ) {
        viewManager->ToggleObject(object3);

        curve1 = ::GetCurve3D( object1, false );
        curve2 = ::GetCurve3D( object2, false );
        curve3 = ::GetCurve3D( object3, false );

        if ( curve1 != nullptr && curve2 != nullptr && curve3 != nullptr ) {
          MbSurface * surf = 0;
          ::CornerSurface( *curve1, *curve2, *curve3, surf );
          viewManager->AddObject( SURFACE_Style, surf );
        }
        viewManager->ToggleObject(object3);
      }
      viewManager->ToggleObject(object2);
    }
    else {
      SPtr<MbCurve3D> initCurve( ::GetCurve3D( object1, false ) );

      if ( initCurve != nullptr ) {
        if ( initCurve->IsClosed() ) {
          double t0 = initCurve->GetTMin();
          double t3 = initCurve->GetTMax();
          double t1 = t0+(t3-t0)/3.0;
          double t2 = t0+(t3-t0)*2.0/3.0;
          curve1 = initCurve->Trimmed( t0, t1, 1 );
          curve2 = initCurve->Trimmed( t1, t2, 1 );
          curve3 = initCurve->Trimmed( t2, t0, 1 );
        }
        else {
          double t0 = initCurve->GetTMin();
          double t2 = initCurve->GetTMax();
          double t1 = t0+(t2-t0)/2.0;
          MbCartPoint3D p0, p2;
          initCurve->PointOn(t0,p0);
          initCurve->PointOn(t2,p2);
          curve1 = initCurve->Trimmed( t0, t1, 1 );
          curve2 = initCurve->Trimmed( t1, t2, 1 );
          curve3 = new MbLineSegment3D( p2, p0 );
        }
      }
      if ( curve1 != nullptr && curve2 != nullptr && curve3 != nullptr ) {
        MbSurface * surf = nullptr;
        ::CornerSurface( *curve1, *curve2, *curve3, surf );
        viewManager->AddObject( SURFACE_Style, surf );
      }
    }
    viewManager->ToggleObject(object1);
  }
}


//------------------------------------------------------------------------------
// Создание поверхности по четырем кривым.
// Create a cover surface.
// ---
void CreateCoverSurface()
{
  MbItem  *object1, *object2, *object3, *object4;

  SPtr<MbCurve3D> curve1, curve2, curve3, curve4;

  if ( GetAnyCurve( IDS_SELECT_CURVE, &object1 ) ) {
    viewManager->ToggleObject(object1);
    if ( GetAnyCurve( IDS_SELECT_CURVE, &object2 ) ) {
      viewManager->ToggleObject(object2);
      if ( GetAnyCurve( IDS_SELECT_CURVE, &object3 ) ) {
        viewManager->ToggleObject(object3);
        if ( GetAnyCurve( IDS_SELECT_CURVE, &object4 ) ) {
          viewManager->ToggleObject(object4);

          curve1 = ::GetCurve3D( object1, false );
          curve2 = ::GetCurve3D( object2, false );
          curve3 = ::GetCurve3D( object3, false );
          curve4 = ::GetCurve3D( object4, false );

          if ( curve1 != nullptr && curve2 != nullptr && curve3 != nullptr && curve4 != nullptr ) {
            MbSurface * surf = 0;
            ::CoverSurface( *curve1, *curve2, *curve3, *curve4, surf );
            viewManager->AddObject( SURFACE_Style, surf );
          }

          viewManager->ToggleObject(object4);
        }
        viewManager->ToggleObject(object3);
      }
      viewManager->ToggleObject(object2);
    }
    else {
      SPtr<MbCurve3D> initCurve( ::GetCurve3D( object1, false ) );

      if ( initCurve != nullptr ) {
        if ( initCurve->IsClosed() ) {
          double t0 = initCurve->GetTMin();
          double t4 = initCurve->GetTMax();
          double t1 = t0+(t4-t0)*0.25;
          double t2 = t0+(t4-t0)*0.50;
          double t3 = t0+(t4-t0)*0.75;
          curve1 = initCurve->Trimmed( t0, t1, 1 );
          curve2 = initCurve->Trimmed( t1, t2, 1 );
          curve3 = initCurve->Trimmed( t3, t2,-1 );
          curve4 = initCurve->Trimmed( t4, t3,-1 );
        }
        else {
          double t0 = initCurve->GetTMin();
          double t3 = initCurve->GetTMax();
          double t1 = t0+(t3-t0)/3.0;
          double t2 = t0+(t3-t0)/3*2;
          MbCartPoint3D p0, p3;
          initCurve->PointOn(t0,p0);
          initCurve->PointOn(t3,p3);
          curve1 = initCurve->Trimmed( t0, t1, 1 );
          curve2 = initCurve->Trimmed( t1, t2, 1 );
          curve3 = initCurve->Trimmed( t3, t2,-1 );
          curve4 = new MbLineSegment3D( p0, p3 );
        }
      }
     
      if ( curve1 != nullptr && curve2 != nullptr && curve3 != nullptr && curve4 != nullptr ) {
        MbSurface * surf = nullptr;
        ::CoverSurface( *curve1, *curve2, *curve3, *curve4, surf );
        viewManager->AddObject( SURFACE_Style, surf );
      }
    }
    viewManager->ToggleObject(object1);
  }
}


//------------------------------------------------------------------------------
// Создание поверхности по кривой.
// Create a bottom surface by curve.
// ---
void CreateBottomSurface()
{
  MbItem * object = nullptr;

  SPtr<MbCurve3D> curve1, curve2, curve3, curve4;

  if ( GetAnyCurve( IDS_SELECT_CURVE, &object ) ) {
    viewManager->ToggleObject(object);

    SPtr<MbCurve3D> initCurve( ::GetCurve3D( object, false ) );

    if ( initCurve != nullptr ) {
      if ( initCurve->IsClosed() ) {
        double t0 = initCurve->GetTMin();
        double t4 = initCurve->GetTMax();
        double t1 = t0+(t4-t0)*0.25;
        double t2 = t0+(t4-t0)*0.50;
        double t3 = t0+(t4-t0)*0.75;
        curve1 = initCurve->Trimmed( t0, t1, 1 );
        curve2 = initCurve->Trimmed( t1, t2, 1 );
        curve3 = initCurve->Trimmed( t3, t2,-1 );
        curve4 = initCurve->Trimmed( t4, t3,-1 );
      }
      else {
        double t0 = initCurve->GetTMin();
        double t3 = initCurve->GetTMax();
        double t1 = t0+(t3-t0)/3.0;
        double t2 = t0+(t3-t0)/3*2;
        MbCartPoint3D p0, p3;
        initCurve->PointOn(t0,p0);
        initCurve->PointOn(t3,p3);
        curve1 = initCurve->Trimmed( t0, t1, 1 );
        curve2 = initCurve->Trimmed( t1, t2, 1 );
        curve3 = initCurve->Trimmed( t3, t2,-1 );
        curve4 = new MbLineSegment3D( p0, p3 );
      }
    }

    if ( curve1 != nullptr && curve2 != nullptr && curve3 != nullptr && curve4 != nullptr ) {
      MbSurface * surf = nullptr;
      ::CoverSurface( *curve1, *curve2, *curve3, *curve4, surf );
      viewManager->AddObject( SURFACE_Style, surf );
    }

    viewManager->ToggleObject(object);
  }
}


//------------------------------------------------------------------------------
// Создание Hermit сплайн поверхности по кривым.
// Create a lofted surface.
// ---
void CreateLoftedSurface() {
  RPArray<MbCurve3D> curves( 4, 1 ); // Множество не владеет объектами. // The set doesn't hold objects.
  MbItem * nearest = nullptr;

  while ( GetAnyCurve( IDS_SELECT_CURVE, &nearest ) ) {
    viewManager->ToggleObject(nearest);
    SPtr<MbCurve3D> curv( ::GetCurve3D( nearest, false ) );
    curves.Add( ::DetachItem(curv) );
    viewManager->ToggleObject(nearest);
  }

  MbVector3D v1(0,0,0), v2(0,0,0);
  bool cl = false;
  MbSurface * surf = nullptr;
  ::LoftedSurface( curves, cl, v1, v2, surf );
  viewManager->AddObject( SURFACE_Style, surf );

  ::DeleteItems( curves );
}


//------------------------------------------------------------------------------
// Создание кинематической поверхности по сечениям.
// Create an elevation surface.
// ---
void CreateElevationSurface()
{
  RPArray<MbCurve3D> curves( 4, 1 ); // Множество не владеет объектами. // The set doesn't hold objects.
  MbItem * nearest = nullptr;

  while ( GetAnyCurve( IDS_SELECT_CURVE, &nearest ) ) {
    viewManager->ToggleObject(nearest);
    SPtr<MbCurve3D> curv( ::GetCurve3D( nearest, false ) );
    curves.Add( ::DetachItem(curv) );
    viewManager->ToggleObject(nearest);
  }

  if ( curves.Count() > 1 ) {
    MbItem * spine = nullptr;
    if ( GetAnyCurve( IDS_SELECT_GUIDE_CURVE, &spine ) ) {
      viewManager->ToggleObject(spine);
      SPtr<MbCurve3D> spn( ::GetCurve3D( spine, false ) );
      
      MbSurface * surf = nullptr;
      ::LoftedSurface( curves, *spn, surf );
      viewManager->AddObject( SURFACE_Style, surf );
    }
  }
  ::DeleteItems( curves );
}


//------------------------------------------------------------------------------
// Создание поверхности по сетке кривых.
// Create a surface constructed by the grid curves.
// ---
void CreateMeshSurface()
{
  RPArray<MbCurve3D> curveU( 4, 1 ); // Множество не владеет объектами. // The set doesn't hold objects.
  RPArray<MbCurve3D> curveV( 4, 1 ); // Множество не владеет объектами. // The set doesn't hold objects.
  MbItem * nearest = nullptr;

  // Ввод семейства кривых по U. // Input of the set of curves by U.
  while ( GetAnyCurve( IDS_SELECT_CURVE, &nearest ) ) {
    SPtr<MbCurve3D> curv( ::GetCurve3D( nearest, false ) );
    if ( curv != nullptr ) {
      curveU.Add( ::DetachItem( curv ) );
    }
    viewManager->ToggleObject(nearest);
  }

  // Ввод семейства кривых по V. // Input of the set of curves by V.
  while ( GetAnyCurve( IDS_SELECT_CURVE, &nearest ) ) {
    SPtr<MbCurve3D> curv( ::GetCurve3D( nearest, false ) );
    if ( curv != nullptr )
      curveV.Add( ::DetachItem( curv ) );
    viewManager->ToggleObject(nearest);
  }

  ::AddRefItems( curveU );
  ::AddRefItems( curveV );

  MbSurface * surf = nullptr;
  ::MeshSurface( curveU, curveV, surf );

  ::ReleaseItems( curveU );
  ::ReleaseItems( curveV );

  viewManager->AddObject( SURFACE_Style, surf );
} // CreateMeshSurface


//------------------------------------------------------------------------------
// Создание треугольной поверхности Безье.
// Create a triangular Bezier surface.
// ---
void CreateTriBezierSurface()
{
  MbCartPoint3D p[3];

  while ( GetPoint( IDS_SELECT_POINT, p[0] ) ) {
    drawImp->DrawPoint( &p[0], TRGB_LIGHTGREEN );

    if ( GetPoint( IDS_SELECT_POINT, p[1] ) ) {
      drawImp->DrawPoint( &p[1], TRGB_LIGHTGREEN );

      if ( GetPoint( IDS_SELECT_POINT, p[2] ) ) {
        drawImp->DrawPoint( &p[2], TRGB_LIGHTGREEN );

        ptrdiff_t k = Math::curveDegree;
        if ( GetInt( IDS_ENTER_QUANTITY, k ) ) {

          //MbTriBezierSurface *surf = new MbTriBezierSurface( k,p[0],p[1],p[2] );
          MbSurface *surf = nullptr;
          MbResultType res = ::TriBezierSurface( k,p[0],p[1],p[2], surf );

          if ( res == rt_Success )
            viewManager->AddObject( ELEMENTARY_Style, surf );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание симплексной поверхности.
// Create a simplex surface.
// ---
void CreateSimplexSurface() 
{
  MbItem * obj( nullptr );

  if ( GetGObj ( IDS_SELECT_SURFACE, st_Surface, &obj, TestVariables::itemPathFrom ) ) {
    MbSurface * surface = (MbSurface *)((MbSpaceInstance *)obj)->GetSpaceItem();
    if ( surface != nullptr ) {

//#ifdef _DRAWGI
      drawImp->DrawItem( surface, TRGB_LIGHTGREEN );
//#endif // _DRAWGI
      MbSurface * surf = nullptr;
      int d = 1, uCount = TestVariables::count1, vCount = TestVariables::count2;
      if ( GetInt( IDS_ENTER_SPLINE_ORDER, d ) &&
           GetInt( IDS_ENTER_SPLIT_COUNT_U, uCount ) &&
           GetInt( IDS_ENTER_SPLIT_COUNT_V, vCount ) ) {
        TestVariables::count1 = uCount;
        TestVariables::count2 = vCount;
        SetWaitCursor( true );

        ::SimplexSurface( *surface, d, uCount, vCount, surf );
        SetWaitCursor( false );
      }
      if ( surf != nullptr ) {
        MbContour & contour = surf->MakeContour( true );
        RPArray<MbContour> initCurves( 0, 1 );
        initCurves.Add( &contour );
        MbCurveBoundedSurface * bndsurf = new MbCurveBoundedSurface( *surf, initCurves, true );

        viewManager->AddObject( ELEMENTARY_Style, bndsurf );

        SetWaitCursor( false );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание поверхности симплексного сплайна.
// Create a simplex spline surface.
// ---
void CreateSimplexSplineSurface() {
  MbCartPoint3D p;
  SArray <MbCartPoint3D> pList( COUNT_DELTA, 1 );
  MbItem * object = nullptr;
  SPtr<MbCurve3D> curve;

  if ( GetAnyCurve( IDS_SELECT_CURVE, &object ) && (object != nullptr) ) {
    viewManager->ToggleObject(object);
    curve = ::GetCurve3D(object, false);
//#ifdef _DRAWGI
    drawImp->DrawItem(curve, TRGB_LIGHTGREEN);
//#endif // _DRAWGI
    if ( curve != nullptr ) {
      ptrdiff_t n = 6;
      if ( GetInt(IDS_ENTER_QUANTITY, n) ) {
        curve->GetPointsByEvenParamDelta( (size_t)n, pList );
      }
    }
  }

  size_t cnt = pList.Count();
  if (cnt == 0 ) {
    p = cursorPoint;
//#ifdef _DRAWGI
    drawImp->DrawPoint( &p, TRGB_LIGHTGREEN );
//#endif // _DRAWGI
    pList.Add(p);
    while ( GetPoint( IDS_SELECT_POINT, p )) {
//#ifdef _DRAWGI 
      drawImp->DrawPoint( &p, TRGB_LIGHTGREEN );
//#endif // _DRAWGI
      pList.Add(p);
    }
  }

  cnt = pList.Count();
  if ( cnt >= 3 ) {
    SetWaitCursor( true );

    MbSurface * surf = nullptr;
    ::SimplexSplineSurface( pList, surf );

    SetWaitCursor( false );
    viewManager->AddObject( ELEMENTARY_Style, surf );

  }
}
