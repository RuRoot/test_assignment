#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <test_draw.h>
#include <cur_contour.h>
#include <cur_contour3d.h>
#include <cur_contour_on_surface.h>
#include <cur_line_segment.h>
#include <cur_nurbs3d.h>
#include <cur_arc.h>
#include <cur_nurbs.h>
#include <cur_hermit.h>
#include <cur_line_segment3d.h>
#include <cur_bridge3d.h>
#include <cur_plane_curve.h>
#include <cur_polyline3d.h>
#include <cur_trimmed_curve3d.h>
#include <cur_contour3d.h>
#include <cur_line3d.h>
#include <cur_arc3d.h>
#include <cur_character_curve3d.h>
#include <cur_b_spline.h>
#include <cur_character_curve.h>
#include <cur_offset_curve3d.h>
#include <cur_projection_curve.h>
#include <cur_surface_intersection.h>
#include <surf_curve_bounded_surface.h>
#include <point_frame.h>
#include <function.h>
#include <function_factory.h>
#include <surf_plane.h>
#include <alg_polyline.h>
#include <action_curve.h>
#include <action_curve3d.h>
#include <action_surface_curve.h>
#include <action_surface.h>
#include <action_solid.h>
#include <action_shell.h>
#include <action_wireframe.h>
#include <action_general.h>
#include <solid.h>
#include <space_instance.h>
#include <plane_instance.h>
#include <wire_frame.h>
#include <cr_unwrap_curve.h>
#include <cr_wrap_curve.h>
#include <point3d.h>
#include <alg_nurbs_conic.h>
#include <op_wireframe_parameter.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <numeric>
#include <attr_identifier.h>
#include <mb_smooth_nurbs_fit_curve.h>
#include <last.h>

using namespace c3d;


#ifdef _DRAWGI  // включатели отрисовки во время отладки
namespace DrawTestCurve3D {
  bool drawItems = false;
};
#endif // _DRAWGI


//------------------------------------------------------------------------------
// Создание В-сплайна в плоскости.
// Construct B-spline on plane.
// ---
void CreatePlaneNurbs()
{
  MbCartPoint3D pnt;
  WorkWindow * currentView = TestVariables::viewManager->GetActiveWindow();

  if ( currentView ) {
    MbPlacement3D plane( currentView->GetOwnPlace() );

    SArray<MbCartPoint> pList;

    while ( GetPoint( IDS_SELECT_POINT, pnt ) ) {

  //#ifdef _DRAWGI
      TestVariables::drawImp->DrawPoint( &pnt, TRGB_LIGHTGREEN );
  //#endif

      pnt.Transform( plane.GetMatrixInto() );

      MbCartPoint p2D( pnt.x, pnt.y );
      pList.push_back( p2D );
    }

    if ( pList.size() >= static_cast<size_t>(TestVariables::degree) ) {
      int cl = 0;
      GetInt( IDS_ENTER_CLOSED, cl );
      SArray<double> knots;
      SArray<double> wList;
      MbCurve * crv = nullptr;
      ::NurbsCurve( pList, wList, TestVariables::degree, knots, cl > 0, crv );

      if ( crv != nullptr ) {
        MbCurve3D * curv = nullptr;
        ::PlaneCurve( plane, *crv, curv );
        TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, curv ); 
      }
      pList.clear();
    }
  }
}


//------------------------------------------------------------------------------
// Создание эквидистантной кривой.
// Construct an equidistant curve.
// ---
void CreatePlaneOffsetCurve()
{
  MbItem * nearest = nullptr;

  while ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest ) ) {
    if ( nearest->IsA() == st_SpaceInstance ) {
      TestVariables::viewManager->ToggleObject(nearest);
      double d = 10;
      if ( GetDouble( IDS_ENTER_STRETCHING, d )) {
        MbCurve3D * curv = (MbCurve3D *)((MbSpaceInstance *)nearest)->GetSpaceItem();
        if ( curv != nullptr ) {
          MbCurve3D * curve = nullptr;
          ::OffsetPlaneCurve( *curv, d, curve );
          TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, curve ); 
        }
      }
      TestVariables::viewManager->ToggleObject(nearest);
    }
  }
}


//------------------------------------------------------------------------------
// Создание косинусоиды.
// Construct a cosine curve.
// ---
void CreatePlaneCosinusoid( bool byThreePoints )
{
  bool createCosinusoid = false;
  MbCartPoint3D p0, p1, p2;

  if ( byThreePoints ) {
    if ( GetPoint( IDS_SELECT_POINT, p0 ) ) {
      if ( GetPoint( IDS_SELECT_POINT, p1 ) && GetPoint( IDS_SELECT_POINT, p2 ) )
        createCosinusoid = true;
    }
  }
  else if ( GetPoint( IDS_SELECT_POINT, p0 ) ) {
    createCosinusoid = true;
  }

  if ( createCosinusoid ) {
    MbPlacement3D place;
    MbCurve * pCurve = nullptr;

    double phase = 0.0;
    GetDouble( IDS_ENTER_PHASE, phase );

    if ( byThreePoints ) {
      createCosinusoid = false;
      double eps = LENGTH_EPSILON;
      if ( !c3d::EqualPoints( p0, p1, eps ) && !c3d::EqualPoints( p0, p2, eps ) ) {
        MbVector3D v1( p0, p1 ), v2( p0, p2 );
        if ( !v1.Colinear( v2 ) )
          createCosinusoid = true;
      }
      if ( createCosinusoid ) {
        place.Init( p0, p1, p2 );
        double waveLength = p0.DistanceToPoint( p1 );
        MbCartPoint q0, q1, q2;
        place.PointProjection( p0, q0.x, q0.y );
        place.PointProjection( p1, q1.x, q1.y );
        place.PointProjection( p2, q2.x, q2.y );
        ::Cosinusoid( q0, q1, q2, phase, waveLength, pCurve ); 
      }
    }
    else {
      place.SetOrigin( p0 );
      MbCartPoint q0;

      double amplitude = 1.0;
      GetDouble( IDS_ENTER_AMPLITUDE, amplitude );
      double waveLength = 1.0;
      GetDouble( IDS_ENTER_WAVELENGTH, waveLength );
      double wavesCount = 1.0;
      GetDouble( IDS_ENTER_WAVESCOUNT, wavesCount );

      ::Cosinusoid( q0, amplitude, waveLength, wavesCount, phase, pCurve ); 
    }

    ::AddRefItem( pCurve );

    if ( pCurve != nullptr ) {
      MbCurve3D * sCurve = nullptr;
      ::PlaneCurve( place, *pCurve, sCurve );
      if ( sCurve != nullptr )
        TestVariables::viewManager->AddObject( TestVariables::TEXT_Style, sCurve );
    }

    ::ReleaseItem( pCurve );
  }
}


//------------------------------------------------------------------------------
// Создание плоской кривой по двумерной кривой.
// Construct a space curve from a two-dimensional curve lying on plane.
// ---
void CreateCurveByCurveOnPlane()
{
  MbItem * obj = nullptr;

  while ( GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &obj) )
  {
    TestVariables::viewManager->ToggleObject(obj);
    // Дать двумерный геометрический объект.
    // Get a 2-dimensional geometric object.
    const MbPlaneItem * pItem = static_cast<const MbPlaneInstance *>(obj)->GetPlaneItem(); 
    if ( (pItem != nullptr) && (pItem->Family() == pt_Curve) ) {
      MbCurve3D * crv = nullptr;
      ::PlaneCurve( static_cast<const MbPlaneInstance *>(obj)->GetPlacement(), static_cast<MbCurve &>(pItem->Duplicate()), crv );
      TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, crv ); 
    }
    TestVariables::viewManager->ToggleObject(obj);
  }
}

//------------------------------------------------------------------------------
// Создание пространственной кривой по двумерной кривой.
// Construct a space curve from by two-dimensional curve.
// ---
void CreateSpaceCurveByCurveOnPlane()
{
  MbItem * obj = nullptr;

  while ( GetMObj( IDS_SELECT_CRV_ON_PLANE, pt_Curve, &obj ) ) 
  {
    SPtr<MbCurve3D> curv;
    TestVariables::viewManager->ToggleObject(obj);
    ::GetSpaceCurve( *obj, true, curv );
    TestVariables::viewManager->ToggleObject(obj);
    if ( curv != nullptr ) {
      curv = ::DuplicateIfUsed( curv, (MbRegDuplicate *)nullptr );
      TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, curv );
    }
  }
}

//------------------------------------------------------------------------------
// Создать кривую по плоской пространственной.
// Construct a space curve from a two-dimensional curve lying on plane.
// ---
void CreateGetPlaneCurve()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) ) {
    TestVariables::viewManager->ToggleObject( obj );
    if ( obj->IsA() == st_SpaceInstance ) {
      const MbCurve3D * curve = (const MbCurve3D *)((MbSpaceInstance *)obj)->GetSpaceItem();
      if ( curve != nullptr ) {
        MbCurve * pCurve = nullptr;
        MbPlacement3D curvePlace;
        if ( curve->GetPlaneCurve( pCurve, curvePlace, true ) ) {
          MbCurve3D * planeCurve = nullptr;
          ::PlaneCurve( curvePlace, static_cast<MbCurve &>( pCurve->Duplicate() ), planeCurve );
          if ( planeCurve != nullptr ) {
            TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, planeCurve ); 

            if ( !curve->IsPlanar() || !curve->GetPlacement( curvePlace ) ) // Рассогласование функций планарности. Mismatch functions planarity.
              MessageBoxEx( RT_MISMATCH_FUNCTION_PLANARITY, IDS_TITLE_MSG );
          }
          else {
            MessageBoxEx( RT_CURVE3D_NOT_PLANAR, IDS_TITLE_MSG );
          }
          ::DeleteItem( pCurve );
        }
      }
    }
    TestVariables::viewManager->ToggleObject( obj );
  }
}


//------------------------------------------------------------------------------
// Создание линии.
// Consruct a line.
// ---
void CreateLine3DPointPoint()
{
  MbCartPoint3D points[2];

  while ( GetPoint( IDS_SELECT_POINT, points[0] ) ) 
  {
//#ifdef _DRAWGI
    TestVariables::drawImp->DrawPoint( &points[0], TRGB_LIGHTGREEN );
//#endif

    if ( GetPoint( IDS_SELECT_POINT, points[1] ) ) 
    {
//#ifdef _DRAWGI
      TestVariables::drawImp->DrawPoint( &points[1], TRGB_LIGHTGREEN );
//#endif

      MbLine3D * line = new MbLine3D( points[0], points[1] );
      TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, line );
    }
  }
}


//------------------------------------------------------------------------------
// Создание прямой линии пересечения плоскостей.
// Construct a line of intersection of two planes. 
// ---
void CreateLine3DPlanesIntersection()
{
  MbItem * nearest[2];
  MbLine3D * line = nullptr;

  if ( GetGObj( IDS_SELECT_SURFACE, st_Plane, &nearest[0] ) ) {
    TestVariables::viewManager->ToggleObject(nearest[0]);

    if ( GetGObj( IDS_SELECT_SURFACE, st_Plane, &nearest[1] ) ) {
      TestVariables::viewManager->ToggleObject(nearest[1]);

      line = PlanesIntersection( *(MbPlane *)((MbSpaceInstance *)nearest[0])->GetSpaceItem(),
                                 *(MbPlane *)((MbSpaceInstance *)nearest[1])->GetSpaceItem() );

      TestVariables::viewManager->ToggleObject(nearest[1]);
    }

    TestVariables::viewManager->ToggleObject(nearest[0]);
  }

  if ( line != nullptr )
    TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, line );
}


//------------------------------------------------------------------------------
// Создание окружности.
// Construct a circle.
// ---
void CreateCircle3D()
{
  MbCartPoint3D p[2];

  while ( GetPoint( IDS_SELECT_CENTER, p[0] ) ) {
//#ifdef _DRAWGI
    TestVariables::drawImp->DrawPoint( &p[0], TRGB_LIGHTGREEN );
//#endif
    if ( GetPoint( IDS_SELECT_POINT_ON_CRV, p[1] ) ) {
//#ifdef _DRAWGI
      TestVariables::drawImp->DrawPoint( &p[1], TRGB_LIGHTGREEN );
//#endif
      const WorkWindow * currentView = TestVariables::viewManager->GetActiveWindow();
      if ( currentView ) {
        MbPlacement3D plane( currentView->GetNullPlacement() );
        plane.SetOrigin( p[0] );
        double r = p[0].DistanceToPoint( p[1] );

        MbArc3D * curv = new MbArc3D( plane, r, r, M_PI2 );
        TestVariables::viewManager->AddObject( TestVariables::CONIC_Style, curv );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание эллипса.
// Construct an ellipse.
// ---
void CreateEllipse3D()
{
  MbCartPoint3D centre, pnt; 
  SArray<MbCartPoint3D> points( 4, 1 ); 

  if ( GetPoint(IDS_SELECT_CENTER, centre) ) {
//#ifdef _DRAWGI
    TestVariables::drawImp->DrawPoint( &centre, TRGB_GREEN );
//#endif // _DRAWGI
    while ( GetPoint(IDS_SELECT_POINT, pnt) ) {
      points.Add( pnt );
//#ifdef _DRAWGI
      TestVariables::drawImp->DrawPoint( &pnt, TRGB_GREEN );
//#endif // _DRAWGI
    }

    MbCurve3D * result( nullptr );
    double angle( M_PI*2/3);
    double a( 0.0 ), b( 0.0 );
    int closed( 1 );
    GetInt( IDS_ENTER_CLOSED_BOOL, closed );
    GetDouble( IDS_ENTER_ANGLE, angle );

    ::Arc( centre, points, closed != 0, angle, a, b, result );

    if ( result != nullptr )
      TestVariables::viewManager->AddObject( TestVariables::ELLIPSE_Style, result );
  }
}


//------------------------------------------------------------------------------
// Создание отрезка прямой линии.
// Construct a line segment.
// ---
void CreateLineSeg3D()
{
  MbCartPoint3D points[2];

  while ( GetPoint( IDS_SELECT_POINT, points[0] ) ) {

//#ifdef _DRAWGI
    TestVariables::drawImp->DrawPoint( &points[0], TRGB_LIGHTGREEN );
//#endif

    if (  GetPoint( IDS_SELECT_POINT, points[1] ) ) {

//#ifdef _DRAWGI
      TestVariables::drawImp->DrawPoint( &points[1], TRGB_LIGHTGREEN );
//#endif
      MbCurve3D * curve = nullptr;
      ::Segment( points[0], points[1], curve );
      TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, curve );
    }
  }
}


//------------------------------------------------------------------------------
// Создание дуги окружности.
// Construct a circular arc.
// ---
void CreateArc3D( int n, bool closed )
{
  MbCartPoint3D p[3];

  while ( GetPoint( IDS_SELECT_CENTER, p[0] ) ) {

//#ifdef _DRAWGI
    TestVariables::drawImp->DrawPoint( &p[0], TRGB_LIGHTGREEN );
//#endif

    if ( GetPoint( IDS_SELECT_POINT, p[1] ) ) {

//#ifdef _DRAWGI
      TestVariables::drawImp->DrawPoint( &p[1], TRGB_LIGHTGREEN );
//#endif

      if ( GetPoint( IDS_SELECT_POINT, p[2] ) ) {

//#ifdef _DRAWGI
        TestVariables::drawImp->DrawPoint( &p[2], TRGB_LIGHTGREEN );
//#endif

        if ( !closed )
          GetInt( IDS_ENTER_NUMBER, n );

        MbArc3D * curv = MbArc3D::Create( p[0], p[1], p[2], n, closed );
        TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, curv );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание тела спирали.
// Construct a spiral curve.
// ---
void CreateSpiral()
{
  MbCartPoint3D p[3]; // Точки на оси. Axis points.
  ptrdiff_t i = 0;

  while ( i < 3 && GetPoint( IDS_SELECT_POINT, p[i] ) ) {
//#ifdef _DRAWGI
    TestVariables::drawImp->DrawPoint( &p[i], TRGB_LIGHTGREEN );
//#endif
    i++;
  }

  MbItem * curve = nullptr; // Образующая. A guide curve.
  const MbCurve  * pCurve = nullptr; // Образующая кривая. A guide curve.
  MbPlacement3D pl;
  double step = 10;

  if ( i == 3 )
    FindNearestObj( p[2], pt_Curve, &curve );
  if ( i == 2 )
    GetMObj( IDS_SELECT_GENER_CURVE, pt_Curve, &curve );

  if ( curve != nullptr ) {
    pl.Init(((MbPlaneInstance *)curve)->GetPlacement());
    pCurve = static_cast<const MbCurve *>( ((MbPlaneInstance*)curve)->GetPlaneItem() );
  }

  if ( pCurve != nullptr && curve != nullptr )
  {
    TestVariables::viewManager->ToggleObject( curve );
    if ( GetDouble( IDS_ENTER_STEP, step ) ) 
    {
      int spiralAxis = 1;
      GetInt( IDS_ENTER_SPIRAL_TYPE, spiralAxis );
      MbVector3D axisX( p[0], p[1] ), axisY( p[0], p[2] );
      MbPlacement3D posXY ( axisX, axisY, p[0] );
      MbMatrix      matr;

      if ( pl.GetMatrixToPlace( posXY, matr ) ) 
      {
        double radius = ( posXY.GetAxisX() | axisY ).Length();
        if ( spiralAxis > 0 )
          GetDouble( IDS_ENTER_RADIUS, radius );
        MbCurve * lawCurve = static_cast<MbCurve *>( &pCurve->Duplicate() );
        lawCurve->Transform( matr );

        MbCurve3D * spiral = nullptr;
        MbResultType res = ::SpiralCurve( p[0], p[1], p[2], radius, step, 0.0, lawCurve, spiralAxis > 0, spiral ); 

        if ( spiral != nullptr )
          TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, spiral );
        else
          ::PutErrorMessage( res, IDS_TITLE_ERR );
      }
    }
  }
  else if ( i == 3 ) {
    if ( GetDouble( IDS_ENTER_STEP, step ) ) {
      MbVector3D axisX( p[0], p[1] ), axisY( p[0], p[2] );
      axisX.Normalize();
      double radius = ( axisX | axisY ).Length();

      MbCurve3D * spiral = nullptr;
      MbResultType res = ::SpiralCurve( p[0], p[1], p[2], radius, step, 0.0, nullptr, false, spiral );

      if ( spiral != nullptr )
        TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, spiral );
      else
        ::PutErrorMessage( res, IDS_TITLE_ERR );
    }
  }
}

//----------------------------------------------------------------------------------------
// Создание сплайна указанного типа.
// Construct a spline of a given type.
// ---
bool CreateSpline3D( MbeSpaceType curveType )
{ 
  SArray<MbCartPoint3D> pList;
  MbCartPoint3D pnt;

  while ( GetPoint( IDS_SELECT_POINT, pnt ) ) 
  {
//#ifdef _DRAWGI
    TestVariables::drawImp->DrawPoint( &pnt, TRGB_LIGHTGREEN );
//#endif
    pList.push_back( pnt );
  }

  bool resu( pList.size() > 1 );
  if ( resu ) 
  {
    int iCls = 0;
    GetInt( IDS_ENTER_CLOSED, iCls );
    MbCurve3D * result( nullptr );
    ::SplineCurve( pList, (iCls > 0), curveType, result );
    TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, result );
  }
  pList.clear();

  return resu;
}


//------------------------------------------------------------------------------
// Создание кубического сплайна в плоскости.
// Construct a cubic spline.
// ---
void CreatePlaneSpline( int curveType )
{
  if ( !TestVariables::viewManager->GetActiveWindow() )
    return;

  MbPlacement3D plane( TestVariables::viewManager->GetActiveWindow()->GetOwnPlace() );
  SArray<MbCartPoint> pList;
  MbCartPoint3D pnt;

  while ( GetPoint( IDS_SELECT_POINT, pnt ) ) {
//#ifdef _DRAWGI
    TestVariables::drawImp->DrawPoint( &pnt, TRGB_LIGHTGREEN );
//#endif
    pnt.Transform( plane.GetMatrixInto() );
    pList.push_back( MbCartPoint(pnt.x, pnt.y) );
  }

  if ( pList.size() > 1 ) {
    int iCls = 0;
    GetInt( IDS_ENTER_CLOSED, iCls );
    MbCurve * curv = nullptr;
    ::SplineCurve( pList, (iCls > 0), (MbePlaneType)curveType, curv );
    if ( curv != nullptr ) {
      MbCurve3D * result( nullptr );
      ::PlaneCurve( plane, *curv, result );
      TestVariables::viewManager->AddObject( Style(1, RGB(255, 0, 0  )), result ); 
    }
  }

  pList.clear();
}


//------------------------------------------------------------------------------
// Создание В-сплайна.
// Construct a B-spline.
// ---
void CreateNurbs3D( bool useMating )
{
  if ( GetInt( IDS_ENTER_SPLINE_ORDER, TestVariables::degree ) ) {
    TestVariables::degree = std_max( TestVariables::degree, 2 );

    if ( useMating )
      GetBoolean( IDS_ENTER_USE_OR_NOT_BOOL, useMating, false );
    
    MbCartPoint3D pnt;
    SpacePointsVector pnts;

    RPArray<PntMatingData3D> matingData;
    SArray<ptrdiff_t> * dummyInds = nullptr;

    static bool allowMovePoints = false; // CAD Kompas mode
    static bool allowAttachMode = false; // default mating mode
    static bool allowZeroMatingPointers = true; // allow zero mating pointers

    while ( GetPoint( IDS_SELECT_POINT, pnt ) ) {
//#ifdef _DRAWGI
      TestVariables::drawImp->DrawPoint( &pnt, TRGB_LIGHTGREEN );
//#endif
      pnts.push_back( pnt );

      if ( useMating ) {
        PntMatingData3D * matingItem = nullptr;

        if ( !allowZeroMatingPointers ) {
          matingItem = new PntMatingData3D;
          matingItem->Init( trt_Position, nullptr, nullptr, nullptr, dummyInds, allowMovePoints, allowAttachMode );
        }

        int type = trt_Position;
        if ( GetInt( IDS_ENTER_CONN_TYPE, type ) && (type >= trt_Tangent && type <= trt_SmoothG3) ) {
          if ( nullptr == matingItem ) {
            matingItem = new PntMatingData3D;
          }

          MbVector3D * fder = nullptr;
          MbCartPoint3D pnt1( pnt );

          if ( GetPoint( IDS_SELECT_SECOND_POINT, pnt1 ) ) {
//#ifdef _DRAWGI
            TestVariables::drawImp->DrawLine( pnt, pnt1, TRGB_LIGHTGRAY );
            TestVariables::drawImp->DrawPoint( &pnt1, TRGB_LIGHTGRAY );
            TestVariables::drawImp->DrawPoint( &pnt, TRGB_LIGHTGREEN );
//#endif
            fder = new MbVector3D( pnt, pnt1 );
          }
          else {
            PathMatrix curvePathFrom;
            SpaceCurveSPtr curve( SelectAnyCurveCopy( curvePathFrom ) );

            if ( curve != nullptr ) {
              double t = UNDEFINED_DBL;
              if ( curve->NearPointProjection( pnt, t, true ) ) {
                fder = new MbVector3D;
                curve->_FirstDer( t, *fder );
                double fderLen = fder->Length();

                GetDouble( IDS_POCKET_LENGTH, fderLen );

                fder->Normalize();
                (*fder) *= fderLen;
              }
            }
          }

          MbVector3D * sder = nullptr;
          pnt1 = pnt;
          if ( (type == trt_SmoothG2 || type == trt_Normal) ) {
            if ( (fder != nullptr) && GetPoint( IDS_SELECT_POINT, pnt1 ) ) {
              sder = new MbVector3D( pnt, pnt1 );
              if ( !sder->Orthogonal( *fder ) )
                sder->NormalComponent( *fder );
            }
          }

          switch ( type ) {
            case trt_Tangent   :
              matingItem->Init( trt_Tangent,  fder, nullptr, nullptr, dummyInds, allowMovePoints, allowAttachMode );
              break;
            case  trt_Normal   :
              matingItem->Init( trt_Normal,   fder, sder, nullptr, dummyInds, allowMovePoints, allowAttachMode );
              break;
            case  trt_SmoothG2 :
              matingItem->Init( trt_SmoothG2, fder, sder, nullptr, dummyInds, allowMovePoints, allowAttachMode );
              break;
            default:
              matingItem->Init( trt_Position, nullptr, nullptr, nullptr, dummyInds, allowMovePoints, allowAttachMode );
          }

          if ( !matingItem->IsValid() )
            matingItem->Init( trt_Position, nullptr, nullptr, nullptr, dummyInds, allowMovePoints, allowAttachMode );

          ::DeleteMatItem( fder );
          ::DeleteMatItem( sder );
        }

        matingData.push_back( matingItem );
      }
    }

    if ( pnts.size() > 1 && TestVariables::degree > 1 ) {
      bool closed = false;
      GetBoolean( IDS_ENTER_CLOSED_BOOL, closed, false );

      bool throughItems = false;
      GetBoolean( IDS_ENTER_THROUGH_ITEMS_BOOL, throughItems, false );

      if ( useMating ) {
        SArray<MbCartPoint3D> points( pnts ); 

        MbWireFrame * resFrame = nullptr;
        MbSNameMaker operNames( ct_Nurbs3DCreator, MbSNameMaker::i_SideNone, 0 );
        Style style = TestVariables::WIRE_Style;
        MbeSplineParamType spType = spt_ChordLength;
        MbResultType resType = rt_Error;

        if ( throughItems ) {
          resType = ::SpaceSplineThrough( points, spType, TestVariables::degree, closed, 
                                          matingData, operNames,
                                          resFrame );
        }
        else {
          resType = ::SpaceSplineBy( points, TestVariables::degree, closed, nullptr, nullptr,
                                     matingData.front(), matingData.back(), operNames,
                                     resFrame );
        }

        if ( resType == rt_Success || resType == rt_SelfIntersect ) {
          PRECONDITION( resFrame != nullptr );
          TestVariables::viewManager->AddObject( style, resFrame );
        }
      }
      else {
        MbCurve3D * resCurve = nullptr;
        Style style = TestVariables::BSPLINE_Style;

        MbeSplineParamType pType = spt_ChordLength;

        if ( throughItems ) {
          // degree 4 only
          //::SplineCurve( pnts, closed, st_Nurbs3D, resCurve );

          // doesn't work if a degree is less than number of control points
          //DoubleVector params;
          //CreateSplineParameters( pnts, pType, closed, params );
          //resCurve = MbNurbs3D::CreateThrough( degree, closed, pnts, params );

          SArray<MbCartPoint3D> spnts( pnts );

          SArray<double> params;
          CreateSplineParameters( pnts, pType, closed, params );

          RPArray<c3d::PntMatingData3D> matingData_;

          resCurve = MbNurbs3D::CreateNURBS( TestVariables::degree, spnts, params, closed, matingData_ );
        }
        else {
          SArray<double> knots;
          SArray<double> wts;
          SArray<MbCartPoint3D> spnts( pnts );
          ::NurbsCurve( spnts, wts, TestVariables::degree, knots, closed, resCurve );
        }
        TestVariables::viewManager->AddObject( style, resCurve );
      }
    }

    ::DeleteMatItems( matingData );
  }
}


//------------------------------------------------------------------------------
// Создание усеченной кривой.
// Construct a trimmed curve.
// ---
void CreateTrimmedCurve3D()
{
  MbItem * obj = nullptr;
  SPtr<MbCurve3D> resCurve;
  PathMatrix pathFrom;

  MbCartPoint3D pnt1, pnt2;

  while ( (resCurve = ::SelectAnyCurveCopy( pathFrom, nullptr, &obj )) ) {
    TestVariables::viewManager->ToggleObject( obj );

    if ( GetPoint( IDS_SELECT_POINT, pnt1 ) ) {
      TestVariables::drawImp->DrawPoint( &pnt1, TRGB_LIGHTGREEN );
      if ( GetPoint( IDS_SELECT_POINT, pnt2 ) ) {
        TestVariables::drawImp->DrawPoint( &pnt2, TRGB_LIGHTGREEN );
        resCurve->Transform( pathFrom.second );
        resCurve = new MbTrimmedCurve3D( *resCurve, pnt1, pnt2 );
      }
    }

    TestVariables::viewManager->ToggleObject( obj );

    if ( resCurve != nullptr) {
      TestVariables::viewManager->DeleteObject( obj );
      TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, resCurve );
    }
  }
}


//------------------------------------------------------------------------------
// Создание удлиненной кривой.
// Construct an elongated curve.
// ---
void CreateExtendedCurve3D()
{
  MbItem * obj = nullptr;
  MbCurve3D * resCurve = nullptr;
  MbCartPoint3D pnt1, pnt2;

  while ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) && obj != nullptr ) {
    TestVariables::viewManager->ToggleObject(obj);
    if ( obj->IsA() == st_SpaceInstance ) {
      // Дать трёхмерный геометрический объекта.
      // Get a 3-dimensional geometric object.
      const MbSpaceItem * sItem = ((MbSpaceInstance *)obj)->GetSpaceItem(); 
      if ( (sItem != nullptr) && (sItem->Family() == st_Curve3D) ) {
        const MbCurve3D * initCurve = (const MbCurve3D *)sItem;
        initCurve->GetLimitPoint( 1, pnt1 );
        initCurve->GetLimitPoint( 2, pnt2 );

        double t1 = UNDEFINED_DBL;
        double t2 = UNDEFINED_DBL;

        if ( GetPoint( IDS_SELECT_POINT, pnt1 ) ) {
          TestVariables::drawImp->DrawPoint( &pnt1, TRGB_LIGHTGREEN );
          if ( GetPoint( IDS_SELECT_POINT, pnt2 ) ) {
            TestVariables::drawImp->DrawPoint( &pnt2, TRGB_LIGHTGREEN );
          }

          initCurve->NearPointProjection( pnt1, t1, true );
          initCurve->NearPointProjection( pnt2, t2, true );
        }
        else {
          t1 = initCurve->GetTMin();
          t2 = initCurve->GetTMax();

          if ( GetDouble( IDS_ENTER_PARAM_VALUE, t1 ) ) {
            initCurve->_PointOn( t1, pnt1 );
            TestVariables::drawImp->DrawPoint( &pnt1, TRGB_GREEN );
            if ( GetDouble( IDS_ENTER_PARAM_VALUE, t2 ) ) {
              initCurve->_PointOn( t2, pnt2 );
              TestVariables::drawImp->DrawPoint( &pnt2, TRGB_GREEN );
            }
          }
        }

        if ( t2 < t1 ) std::swap( t1, t2 );

        resCurve = initCurve->Trimmed( t1, t2, 1 );

        if ( resCurve == nullptr )
          resCurve = new MbTrimmedCurve3D( *initCurve, t1, t2, 1, true );

        if ( resCurve->IsDegenerate() )
          ::DeleteItem( resCurve );
      }
      else {
        PRECONDITION( false );
      }
    }
    TestVariables::viewManager->ToggleObject( obj );

    if ( resCurve != nullptr ) {
      TestVariables::viewManager->DeleteObject( obj );
      TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, resCurve );
    }
  }
}


//------------------------------------------------------------------------------
// Создание поверхностной кривой по пространственной кривой
// ---
bool TranslateSpaceCurveToSurfaceCurve()
{
  MbItem * curveItem = nullptr;
  PathMatrix pathMatr;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &curveItem, pathMatr ) && (curveItem->IsA() == st_SpaceInstance) ) {
    TestVariables::viewManager->ToggleObject(curveItem);

    const MbSpaceInstance * spaceInst = static_cast<const MbSpaceInstance *>(curveItem);

    if ( spaceInst->GetSpaceItem() != nullptr && spaceInst->GetSpaceItem()->Family() == st_Curve3D ) {
      const MbCurve3D * spaceCurve = static_cast<const MbCurve3D *>( spaceInst->GetSpaceItem() );
      const MbMatrix3D & curveMatrFrom = pathMatr.second;

      MbMatrix3D surfaceMatrFrom;
      SurfaceSPtr surface( ::SelectSurface( &surfaceMatrFrom ) );

      if ( surface == nullptr ) {
        FaceSPtr face( ::SelectFace( &surfaceMatrFrom ) );
        if ( face != nullptr )
          surface = &face->SetSurface().SetSurface();
      }

      if ( surface != nullptr ) {
        surface = static_cast<MbSurface *>( &surface->Duplicate() );
        if ( !surfaceMatrFrom.IsSingle() )
          surface->Transform( surfaceMatrFrom ); // Трансформируем поверхность в мировую систему координат

        if ( !curveMatrFrom.IsSingle() ) {
          MbMatrix3D curveMatrInto;
          curveMatrFrom.Div( curveMatrInto );
          surface->Transform( curveMatrInto ); // Трансфомируем поверхность в систему координат кривой
        }
      }

      SpaceCurveSPtr surfaceCurve;
      {
        PlaneCurveSPtr pCurve;
        if ( spaceCurve->GetSurfaceCurve( pCurve, surface ) ) {
          pCurve = ::DuplicateIfUsed( pCurve );

          bool useContourOnSurface = true;
          useContourOnSurface = GetBool( IDS_BOOL_USE_CNTR_ON_SURFACE, useContourOnSurface );

          if ( useContourOnSurface ) {
            MbCurve3D * sCurvePtr = nullptr;
            ::SurfaceCurve( *surface, *pCurve, sCurvePtr );
            surfaceCurve = sCurvePtr;
          }
          else {
            surfaceCurve = new MbSurfaceCurve( *surface, *pCurve, true );
          }
          if ( surfaceCurve != nullptr ) {
            if ( !curveMatrFrom.IsSingle() )
              surfaceCurve->Transform( curveMatrFrom ); // Трансформируем новую кривую в мировую систему координат (Альтернатива - делать вставку с локальной системой координат)
            TestVariables::viewManager->DeleteObject( curveItem );
            TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, surfaceCurve );
            MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );
            return true;
          }
        }
      }
      if ( surfaceCurve == nullptr )
        MessageBoxEx( RT_CURVE_NOT_CREATED, IDS_TITLE_MSG );
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Создание кривой на плоскости кривой по пространственной кривой.
// Construct a plane curve by a space curve if the space curve is flat.
// ---
void TranslateCurve3DToPlaneCurve( bool same )
{
  MbItem * nearest = nullptr;
  MbCurve * curv = nullptr;

  PlanarCheckParams params;
  params.accuracy = TestVariables::precision;

  while ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest ) && (nearest->IsA() == st_SpaceInstance) ) {
    TestVariables::viewManager->ToggleObject(nearest);

    const MbCurve3D * scurv = static_cast<const MbCurve3D *>( static_cast<const MbSpaceInstance *>(nearest)->GetSpaceItem() );
    WorkWindow * vc = TestVariables::viewManager->GetActiveWindow();
    MbPlacement3D * curvePlace = nullptr;

    if ( vc != nullptr ) {
      curvePlace = new MbPlacement3D( vc->GetNullPlacement() );

      if ( scurv->GetPlaneCurve( curv, *curvePlace, true, params ) ) // Дать плоскую кривую. Get a flat curve.
        curv = &::DuplicateIfUsed( *curv, (MbRegDuplicate *)nullptr );
      else {
        TestVariables::viewManager->ToggleObject(nearest);
      }
    }
    
    if ( curv != nullptr ) {
      if ( same )
        TestVariables::viewManager->DeleteObject( nearest );
      TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, curv, curvePlace );
    }
    else {
      MessageBoxEx( RT_CURVE2D_CREATE_ERROR, IDS_TITLE_MSG );
    }

    ::DeleteMatItem( curvePlace );
  }
}


//------------------------------------------------------------------------------
// Создание пространственной кривой по кривой на плоскости.
// Construct a space curve by plane curve.
// ---
void TranslatePlaneCurveToCurve3D( bool same )
{
  MbItem * nearest = nullptr;

  while ( GetMObj( IDS_SELECT_CRV_ON_PLANE, pt_Curve, &nearest ) && (nearest->IsA() == st_PlaneInstance) ) {
    TestVariables::viewManager->ToggleObject(nearest);

    MbPlaneInstance * pi = static_cast<MbPlaneInstance*>( nearest );
    if ( pi != nullptr ) {

      const MbPlacement3D & place = pi->GetPlacement();
      for ( size_t i = 0, iCount = pi->PlaneItemsCount(); i < iCount; i++ ) {

        const MbPlaneItem * pItem = pi->GetPlaneItem( i ); 
        if ( (pItem != nullptr) && (pItem->Family() == pt_Curve) ) {

          const MbCurve * pcurv = static_cast<const MbCurve *>( pItem );
          if ( pcurv != nullptr ) {

            MbCurve3D * curv = ::MakeCurve3D( *pcurv, place );
            TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, curv );
          }
        }
      }
      if ( same )
        TestVariables::viewManager->DeleteObject( nearest );
      else
        TestVariables::viewManager->ToggleObject(nearest);
    }
  }

}


//------------------------------------------------------------------------------
// Создание контура на плоскости по кривой на плоскости.
// Construct a 2-dimensional contour by plane curve.
// ---
void TranslatePlaneCurveToContourOnPlane( bool same )
{
  MbItem * nearest = nullptr;

  while ( GetMObj( IDS_SELECT_CRV_ON_PLANE, pt_Curve, &nearest ) && (nearest->IsA() == st_PlaneInstance) ) {
    TestVariables::viewManager->ToggleObject(nearest);
    // Дать двумерный геометрический объект.
    // Get a 2-dimensional geometric object.
    MbPlaneInstance * pi = static_cast<MbPlaneInstance*>( nearest );
    if ( pi != nullptr ) {

      const MbPlacement3D & place = pi->GetPlacement();
      for ( size_t i = 0, iCount = pi->PlaneItemsCount(); i < iCount; i++ ) {

        const MbPlaneItem * pItem = pi->GetPlaneItem( i ); 
        if ( (pItem != nullptr) && (pItem->Family() == pt_Curve) ) {

          const MbCurve * curv = static_cast<const MbCurve *>( pItem );
          if ( curv != nullptr ) {
            MbContour * contour = ( curv->Type() == pt_Contour ) ? (MbContour *)curv : new MbContour( *curv, true );
            MbCurve3D * crv = nullptr;
            ::PlaneCurve( place, same ? *contour : (MbContour &)contour->Duplicate(), crv );

            TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, crv ); 
            ::DeleteItem( contour );
          }
        }
      }
      if (same)
        TestVariables::viewManager->DeleteObject( nearest );
      else
        TestVariables::viewManager->ToggleObject(nearest);
    }
  }
}


//------------------------------------------------------------------------------
// Создание контура на плоскости по контуру на плоскости.
// Construct a space contour by plane contour.
// ---
void TranslatePlaneContourToContourOnPlane( bool same )
{
  MbItem * nearest = nullptr;

  while ( GetMObj( IDS_SELECT_CNTR_ON_PLANE, pt_Contour, &nearest ) && (nearest->IsA() == st_PlaneInstance) ) {
    TestVariables::viewManager->ToggleObject(nearest);
    // Дать двумерный геометрический объект
    // Get a 2-dimensional geometric object.
    MbPlaneInstance * pi = static_cast<MbPlaneInstance*>( nearest );
    if ( pi != nullptr ) {

      const MbPlacement3D & place = pi->GetPlacement();
      for ( size_t i = 0, iCount = pi->PlaneItemsCount(); i < iCount; i++ ) {

        const MbPlaneItem * pItem = pi->GetPlaneItem( i ); 
        if ( (pItem != nullptr) && (pItem->Family() == pt_Curve) ) {

          const MbCurve * curv = static_cast<const MbCurve *>( pItem );
          if ( curv != nullptr ) {
            MbCurve3D * crv = nullptr;
            ::PlaneCurve( place,  same ? *curv : static_cast<MbCurve &>(curv->Duplicate()), crv );

            TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, crv ); 
          }
        }
      }
      if (same)
        TestVariables::viewManager->DeleteObject( nearest );
      else
        TestVariables::viewManager->ToggleObject(nearest);
    }
  }

}


//------------------------------------------------------------------------------
// Создать пространственный контур.
// Construct a space contour.
// ---
void CreateContour3D()
{
  MbContour3D * contour = nullptr;
  bool first = true;
  MbCartPoint3D currPoint, startPoint;
  MbItem * obj = nullptr;

  ptrdiff_t res = IDC_PUSHBUTTON_SPLINE;

  do {
    res = ExecuteContourDlg();

    switch ( res ) {
      case IDC_PUSHBUTTON_SPLINE : // Сплайн. Spline
      {
        SArray<MbCartPoint3D> pList;

        if ( first )
          contour = new MbContour3D();
        else
          pList.push_back( currPoint );

        while ( GetPoint( IDS_SELECT_POINT, currPoint ) ) {
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawPoint( &currPoint, TRGB_LIGHTGREEN );
//#endif
          pList.push_back( currPoint );

          if ( first ) {
            first = false;
            startPoint = currPoint;
            obj = TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPoint3D(startPoint) );
          }
        }

        if ( pList.size() >= Math::curveDegree ) {
          SArray<double> arParams;
          SArray<double> wList;
          MbCurve3D * seg = nullptr;
          ::NurbsCurve( pList, wList, Math::curveDegree, arParams, false, seg );
          contour->AddSegment( *seg, true );
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawItem( seg, TRGB_LIGHTGREEN );
//#endif
        }
        pList.clear();
      }
      break;

      case IDC_PUSHBUTTON_SEGMENT : // Отрезок прямой. Line segment.
      {
        ptrdiff_t idx = 1;
        MbCartPoint3D pt[2];

        if ( first ) {
          contour = new MbContour3D();
          idx = 0;
        }
        else
          pt[0] = currPoint;

        for ( ptrdiff_t i = idx; i < 2; i++ ) {
          GetPoint( IDS_SELECT_POINT, currPoint );
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawPoint( &currPoint, TRGB_LIGHTGREEN );
//#endif
          pt[i] = currPoint;

          if ( first ) {
            first = false;
            startPoint = currPoint;
            obj = TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPoint3D(startPoint) );
          }
        }

        MbCurve3D * seg = nullptr;
        //MbResultType resType = 
        ::Segment( pt[0], pt[1], seg );

        if ( seg )
          contour->AddSegment( *seg, true );
//#ifdef _DRAWGI
        TestVariables::drawImp->DrawItem( seg, TRGB_LIGHTGREEN );
//#endif
      }
      break;

      case IDC_PUSHBUTTON_POLYLINE : // Полилиния. Polyline.
      {
        SArray<MbCartPoint3D> pList;

        if ( first )
          contour = new MbContour3D();
        else
          pList.push_back( currPoint );

        while ( GetPoint( IDS_SELECT_POINT, currPoint ) ) {
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawPoint( &currPoint, TRGB_LIGHTGREEN );
//#endif
          pList.push_back( currPoint );

          if ( first ) {
            first = false;
            startPoint = currPoint;
            obj = TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPoint3D(startPoint) );
          }
        }

        if ( pList.size() > 1 ) {
          MbCurve3D * seg = nullptr;
          ::SplineCurve( pList, false, st_Polyline3D, seg );
          contour->AddSegment( *seg, true );
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawItem( seg, TRGB_LIGHTGREEN );
//#endif
        }
        pList.clear();
      }
      break;

      case IDC_PUSHBUTTON_ARC : // Дуга. Arc.
      {
        ptrdiff_t idx = 1;
        MbCartPoint3D pt[3];

        if ( first ) {
          contour = new MbContour3D();
          idx = 0;
        }
        else
          pt[0] = currPoint;

        for ( ptrdiff_t i = idx; i < 3; i++ ) {
          GetPoint( IDS_SELECT_POINT, currPoint );
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawPoint( &currPoint, TRGB_LIGHTGREEN );
//#endif
          pt[i] = currPoint;

          if ( first ) {
            first = false;
            startPoint = currPoint;
            obj = TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPoint3D(startPoint) );
          }
        }

        MbArc3D * seg = MbArc3D::Create( pt[0], pt[1], pt[2], 1, false );
        if ( seg != nullptr ) {
          contour->AddSegment( *seg, true );
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawItem( seg, TRGB_LIGHTGREEN );
//#endif
        }
      }
      break;

      case IDC_PUSHBUTTON_CLOSE : // Замкнуть. Close.
      {
        if ( contour != nullptr ) {
          MbCurve3D * seg = nullptr;
          ::Segment( currPoint, startPoint, seg );
          if ( seg )
            contour->AddSegment( *seg, true );

//#ifdef _DRAWGI
          TestVariables::drawImp->DrawItem( seg, TRGB_GREEN );
//#endif
        }
      }
      break;

      case IDC_PUSHBUTTON_EXIT : // Выход. Exit.
      {}

      break;
    }

  } while ( ( res != IDC_PUSHBUTTON_CLOSE ) && ( res != IDC_PUSHBUTTON_EXIT ) );

  if (obj != nullptr)
    TestVariables::viewManager->DeleteObject( obj );

  if ( contour != nullptr ) {
    TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, contour );
    {
      bool contLength = true, contDirect = true;
      contour->IsContinuousDerivative( contLength, contDirect ); // C3D-1701
      ptrdiff_t n = 0;
      GetInt( IDS_DERIVATIVES_CONTINUOUS, n );
      if ( !contLength && n > 0 ) {
        contour->SetContinuousDerivativeLength( Math::DefaultMathVersion() );
        contour->IsContinuousDerivative( contLength, contDirect );
        if ( contLength ) {
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создать пространственные контуры по пространственным кривым.
// Create space contours by space curves.
// ---
void CreateContour3DByCurves()
{
  TCHAR msgInfoTitle[] = IDS_TITLE_MSG;

  RPArray<MbContour3D> contours;
  double metricAcc = Math::metricPrecision;

  // Работаем на копиях, т.к. можем разворачивать
  // We use copies, because it may need to change their directions.

  { // Collect contours using all selected curves
    RPArray<MbItem> objItems;
    TestVariables::viewManager->GetSelectedObjects( objItems );

    RPArray<MbCurve3D> curves;

    if ( objItems.size() >= 1 ) { // Автосбор, если несколько кривых выбрано. Auto collect if several curves is selected.
      SPtr<MbCurve3D> objCurve;
      std::vector< SPtr<MbCurve3D> > addCurves;
      for ( size_t k = 0, objCnt = objItems.size(); k < objCnt; k++ ) {
        if ( objItems[k] != nullptr ) {
          ::GetSpaceCurve( *objItems[k], true, objCurve, &addCurves );
          objCurve = ::DuplicateIfUsed( objCurve );
          curves.push_back( ::DetachItem( objCurve ) );
          if ( addCurves.size() > 0 ) {
            for ( size_t l = 0, addCnt = addCurves.size(); l < addCnt; l++ ) {
              SPtr<MbCurve3D> & addCurve = addCurves[l];
              if ( addCurve != nullptr ) {
                addCurve = ::DuplicateIfUsed( addCurve );
                curves.push_back( ::DetachItem( addCurve ) );
              }
            }
            addCurves.clear();
          }
        }
      }

      bool decomposeSpaceContours = true;

      if ( decomposeSpaceContours ) {
        RPArray<MbCurve3D> tmpCurves;
        tmpCurves.AddArray( curves );
        curves.DetachAll();

        size_t curvesCnt = tmpCurves.size();
        for ( size_t k = 0; k < curvesCnt; k++ ) {
          MbCurve3D *& curve = tmpCurves[k];
          if ( curve != nullptr ) {
            if ( curve->Type() == st_Contour3D ) {
              MbContour3D * cntr = static_cast<MbContour3D *>( curve );
              for ( size_t l = 0, segmentsCnt = cntr->GetSegmentsCount(); l < segmentsCnt; l++ ) {
                MbCurve3D * segment = cntr->SetSegment( l );
                curves.push_back( segment );
              }
              cntr->DetachSegments();
              ::DeleteItem( curve );
            }
            else {
              curves.push_back( curve );
              curve = nullptr;
            }
          }
        }
        ::AddRefItems( curves );
        ::DeleteItems( tmpCurves );
        ::DecRefItems( curves );
      }

      size_t usedTime = ::GetTickCountEx();  // SKIP_SA

      ::CreateContours( curves, metricAcc, contours );

      usedTime = ::GetTickCountEx() - usedTime; // SKIP_SA

#ifdef C3D_WINDOWS
      if ( TestVariables::checkOperationTime ) {
        double usedTimeD = usedTime / 1000.0;

        TCHAR buf[1024];
        size_t len = 0;
        len += (size_t)_stprintf( buf + len, RT_TEST_RESULTS );
        len += (size_t)_stprintf( buf+len, RT_TIME_SEC, usedTimeD );
        ::MessageBoxEx( buf, IDS_INFORMATION );
      }
#endif

      ::AddRefItems( contours );
      ::DeleteItems( curves );
      ::DecRefItems( contours );
    }
  }

  if ( contours.size() < 1 ) { // Collect a contour, choosing segments by hand
    SPtr<MbContour3D> cntr( new MbContour3D );

    MbItem * obj = nullptr;
    SPtr<MbCurve3D> crv;
    while ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) && (obj->IsA() == st_SpaceInstance) ) {
      const MbSpaceItem * item = (static_cast<MbSpaceInstance *>(obj))->GetSpaceItem();
      if ( item != nullptr && (item->Family() == st_Curve3D) ) {
        TestVariables::viewManager->ToggleObject( obj );

        crv = static_cast<MbCurve3D *>(&item->Duplicate());
        if ( cntr->GetSegmentsCount() < 1 )
          cntr->AddCurveWithRuledCheck( *crv, metricAcc );
        else {
          bool bAdd = true;
          size_t kDel = SYS_MAX_T;
          for ( size_t k = 0, kcnt = cntr->GetSegmentsCount(); k < kcnt; k++ ) {
            const MbCurve3D * segm = cntr->GetSegment( k );
            if ( segm != nullptr && segm->IsSame( *crv ) ) {
              kDel = k;
              bAdd = false;
              break;
            }
          }

          if ( bAdd ) {
            bAdd = cntr->AddCurveWithRuledCheck( *crv, metricAcc );
            if ( !bAdd ) {
              MessageBoxEx( RT_CURVE_IS_NOT_ADDED, msgInfoTitle );
            }
          }
          else {
            cntr->DeleteSegment( kDel );
            MessageBoxEx( RT_CURVE_REMOVED_FROM_CONTOUR, msgInfoTitle );
          }
        }
      }
    }
    if ( cntr->GetSegmentsCount() > 0 )
      contours.push_back( ::DetachItem( cntr ) );
  }

  if ( contours.size() > 0 ) {
    for ( size_t k = 0, contoursCnt = contours.size(); k < contoursCnt; k++ ) {
      if ( contours[k] != nullptr )
        TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, contours[k] );
    }
    MessageBoxEx( IDS_INFO_ITEMS_CREATED, msgInfoTitle );
  }
  else {
    MessageBoxEx( RT_NOTHING_SELECTED, msgInfoTitle );
  }

  ::DeleteItems( contours );
}     


//------------------------------------------------------------------------------
// Создание контуров по ребрам проволочного каркаса.
// Construct a contours along the edges of a wire frame.
// ---
void CreateContours3DByWireFrame() 
{
  MbWireFrame * wireFrame = SelectWireFrame();

  if ( wireFrame != nullptr ) {
    MbSpaceContourCreationParams params( false, true );
    MbSpaceContourCreationResults<SpaceCurveSPtr> results;
    wireFrame->CreateContours( params, results );

    std::vector<SpaceCurveSPtr> & contours = results.SetContours();
    for ( auto & contour : contours ) {
      if ( contour != nullptr )
        TestVariables::viewManager->AddObject( TestVariables::CONTOUR_Style, contour, true );
    }
  }
}


//------------------------------------------------------------------------------
// Создание контуров в виде отдельных каркасов по ребрам проволочного каркаса.
// Construct a contours in the form of separate frames along the edges of the wire frame.
// ---
void CreateWireFramesByWireFrame()
{
  MbWireFrame * wireFrame = SelectWireFrame();

  if ( wireFrame != nullptr ) {
    MbSpaceContourCreationParams params( false, true );
    MbSpaceContourCreationResults<WireFrameSPtr> wire_results;
    wireFrame->CreateContours( params, wire_results );

    std::vector<WireFrameSPtr> & wireFrames = wire_results.SetContours();
    for ( auto & wFrame : wireFrames ) {
      if ( wFrame != nullptr )
        TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, wFrame, true );
    }
  }
}


//------------------------------------------------------------------------------
// Отделение частей каркаса.
// Detachment of frame parts.
// ---
void CreatePartsByWireFrame()
{
  MbWireFrame * wireFrame = SelectWireFrame();

  if ( wireFrame != nullptr ) {
    WireFramesSPtrVector parts;
    wireFrame->CreateParts( MbCreatePartsParams(), parts );

    for ( auto & part : parts ) {
      if ( part != nullptr )
        TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, part, true );
    }
  }
}


//------------------------------------------------------------------------------
// Создание контура со скруглениями по существующим кривым.
// Construct a contour with fillets by given curves.
// ---
void CreateFilletContour3DByCurves()
{
  TCHAR msgInfoTitle[] = IDS_TITLE_MSG;

  MbItem * obj = nullptr;
  MbContour3D * contour = new MbContour3D;

  // Работаем на копиях, т.к. можем разворачивать
  // We use copies, because it may need to change their directions.

  while ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) && (obj->IsA() == st_SpaceInstance) ) {
    const MbSpaceItem * item = ((MbSpaceInstance *)obj)->GetSpaceItem();
    if ( item != nullptr && (item->Family() == st_Curve3D) ) {
      MbCurve3D * crv = (MbCurve3D *)&item->Duplicate();
      if ( contour->GetSegmentsCount() < 1 )
        contour->AddSegment( *crv, false );
      else {
        bool bAdd = true;
        size_t kDel = SYS_MAX_T;
        for ( size_t k = 0, kcnt = contour->GetSegmentsCount(); k < kcnt; k++ ) {
          const MbCurve3D * segm = contour->GetSegment( k );
          if ( segm != nullptr && segm->IsSame( *crv ) ) {
            kDel = k;
            bAdd = false;
            break;
          }
        }

        if ( bAdd ) {
          bAdd = contour->AddCurveWithRuledCheck( *crv, Math::metricPrecision );
          if ( !bAdd )
            MessageBoxEx( RT_CURVE_IS_NOT_ADDED, msgInfoTitle );
        } else {
          contour->DeleteSegment( kDel );
          MessageBoxEx( RT_CURVE_REMOVED_FROM_CONTOUR, msgInfoTitle );
        }
      }
      ::DeleteItem( crv ); // Удалим, если никому не нужна. Delete curve if it doesn't use.
    }
  }

  SArray<double> radii;
  ptrdiff_t fillNumber = contour->GetSegmentsCount ();
  fillNumber -= ( contour->IsClosed() ) ? 0 : 1;
  for ( ptrdiff_t i = 0; i < fillNumber; ++i )  {
    double r = 0.0;
    if ( !GetDouble ( IDS_ENTER_RADIUS, r ) ) 
      break;
    radii.Add ( r );
  }


  MbCurve3D * filletCurve = nullptr;
  MbResultType res = ::CreateContourFillets ( *contour, radii, filletCurve, ft_Fillet );

  if ( res == rt_Success && filletCurve != nullptr ) {
    TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, filletCurve );
    MessageBoxEx( IDS_INFO_ITEMS_CREATED, msgInfoTitle );
  } else {
    ::DeleteItem ( filletCurve );
    MessageBoxEx( RT_CONTOUR_CREATE_ERROR, msgInfoTitle );
  }
  ::DeleteItem ( (MbCurve3D *&)contour );
}


//------------------------------------------------------------------------------
// Создание фаски контура по существующему 3д-контуру, контуру на поверхности.
// Может принимать и другие типы кривых.
// Возможно сделать одну фаску на все узлы.
// Construct the chamfered 3d-contour based on the given one.
// One may give any MbCurve3d as input.
// It is possibile to create same chamfer on all the joints.
// ---
void CreateChamferContour3DByCurves()
{
  MbItem * obj = nullptr;
  ::GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ); // выбираем контур
  MbResultType result = rt_CurveError;
  if ( obj != nullptr && obj->IsA() == st_SpaceInstance )
  {
    MbCurve3D * curve = static_cast< MbCurve3D * >(
      static_cast< MbSpaceInstance * >(obj)->SetSpaceItem() );
    if ( curve != nullptr && curve->IsA() == st_Contour3D )
      {
        MbContourChamferParams parameters( *curve );
        size_t nVertexes = 100;
        MbContour3D * contour = static_cast < MbContour3D * >( curve );
        if ( contour != nullptr )
          nVertexes = contour->GetSegmentsCount();
        if ( !contour->IsClosed() && nVertexes > 0 )
          --nVertexes;
        std::vector<MbCornerChamferParams> & parVector = parameters.SetParameters();
        #ifdef C3D_WINDOWS
        #ifndef __USE_QT__
        // костыль для починки сборки под *nix до
          GetChamferParameters( &parVector, nVertexes ); // сигнал на мышь
        #endif
        #endif
        if ( parVector.size() != 0 )
        {
          MbCurve3D * resultCurve = nullptr;
          result = ::CreateContourChamfer( parameters, resultCurve ); //делаем фаску
          if ( resultCurve != nullptr )
            TestVariables::viewManager->AddObject( *static_cast< MbSpaceItem * >(resultCurve), true );
        }
        else
          result = rt_ParameterError;
      }
  }
  if ( result != rt_Success )
    ::MessageBoxEx( RT_MISMATCH_IN_OPERATIONS_RESULTS, IDS_TITLE_ERR );
}


//------------------------------------------------------------------------------
// Создание контура на плоскости.
// Construct contour on plane.
// ---
void CreateContourOnPlane()
{
  MbContour * contour = nullptr;
  MbPlacement3D place;
  bool first = true;
  MbCartPoint currPoint, startPoint;
  WorkWindow * vc = TestVariables::viewManager->GetActiveWindow();
  MbItem * obj = nullptr;

  ptrdiff_t res = IDC_PUSHBUTTON_SPLINE;
  
  do {
    res = ExecuteContourDlg();

    switch ( res ) {
      case IDC_PUSHBUTTON_SPLINE : // Сплайн. Spline.
      {
        SArray<MbCartPoint> pList;

        if ( first ) {
          vc = TestVariables::viewManager->GetActiveWindow();
          place.Init(vc->GetOwnPlace());
          contour = new MbContour();
        }
        else
          pList.push_back( currPoint );

        while ( GetPoint( IDS_SELECT_POINT, currPoint ) ) {
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawPoint( &currPoint, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
//#endif // _DRAWGI
          pList.push_back( currPoint );

          if ( first ) {
            first = false;
            startPoint = currPoint;
            MbCartPoint3D p(currPoint.x,currPoint.y,0);
            MbMatrix3D from( vc->GetNullMatrixFrom(false) );
            p.Transform( from );
            obj = TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPoint3D(p) );
          }
        }

        if ( pList.size() >= Math::curveDegree ) {
          SArray<double> knots; 
          SArray<double> wList;
          MbCurve * seg = nullptr;
          ::NurbsCurve( pList, wList, Math::curveDegree, knots, false, seg );
          contour->AddSegment( seg );
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawItem( seg, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
//#endif // _DRAWGI
        }
        pList.clear();
      }
      break;

      case IDC_PUSHBUTTON_SEGMENT : // Отрезок прямой. Line segment.
      {
        ptrdiff_t idx = 1;
        MbCartPoint pt[2];

        if ( first ) {
          vc = TestVariables::viewManager->GetActiveWindow();
          place.Init(vc->GetOwnPlace());
          contour = new MbContour();
          idx = 0;
        }
        else
          pt[0] = currPoint;

        for ( ptrdiff_t i = idx; i < 2; i++ ) {
          GetPoint( IDS_SELECT_POINT, currPoint );
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawPoint( &currPoint, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
//#endif // _DRAWGI
          pt[i] = currPoint;

          if ( first ) {
            first = false;
            startPoint = currPoint;
            MbCartPoint3D p(currPoint.x,currPoint.y,0);
            MbMatrix3D from( vc->GetNullMatrixFrom(false) );
            p.Transform( from );
            obj = TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPoint3D(p) );
          }
        }

        MbLineSegment * seg = new MbLineSegment( pt[0], pt[1] );
        contour->AddSegment( seg );
//#ifdef _DRAWGI
        TestVariables::drawImp->DrawItem( seg, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
//#endif // _DRAWGI
      }
      break;

      case IDC_PUSHBUTTON_POLYLINE : // Полилиния. Polyline.
      {
        SArray<MbCartPoint> pList;

        if ( first ) {
          vc = TestVariables::viewManager->GetActiveWindow();
          place.Init(vc->GetOwnPlace());
          contour = new MbContour();
        }
        else
          pList.push_back( currPoint );

        while ( GetPoint( IDS_SELECT_POINT, currPoint ) ) {
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawPoint( &currPoint, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
//#endif // _DRAWGI
          pList.push_back( currPoint );

          if ( first ) {
            first = false;
            startPoint = currPoint;
            MbCartPoint3D p(currPoint.x,currPoint.y,0);
            MbMatrix3D from( vc->GetNullMatrixFrom(false) );
            p.Transform( from );
            obj = TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(p) );
          }
        }

        if ( pList.size() > 1 ) {
          MbCurve * seg = nullptr;
          ::SplineCurve( pList, false, pt_Polyline, seg );
          contour->AddSegment( seg );
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawItem( seg, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
//#endif // _DRAWGI
        }
        pList.clear();
      }
      break;

      case IDC_PUSHBUTTON_ARC : // Дуга. Arc.
      {
        ptrdiff_t idx = 1;
        MbCartPoint pt[3];

        if ( first ) {
          vc = TestVariables::viewManager->GetActiveWindow();
          place.Init(vc->GetOwnPlace());
          contour = new MbContour();
          idx = 0;
        }
        else
          pt[0] = currPoint;

        for ( ptrdiff_t i = idx; i < 3; i++ ) {
          GetPoint( IDS_SELECT_POINT, currPoint );
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawPoint( &currPoint, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
//#endif // _DRAWGI
          pt[i] = currPoint;

          if ( first ) {
            first = false;
            startPoint = currPoint;
            MbCartPoint3D p(currPoint.x,currPoint.y,0);
            MbMatrix3D from( vc->GetNullMatrixFrom(false) );
            p.Transform( from );
            obj = TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(p) );
          }
        }

        MbCurve * seg = MbArc::Create( pt[0], pt[1], pt[2] );
        if ( seg != nullptr ) {
          contour->AddSegment( seg );
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawItem( seg, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
//#endif // _DRAWGI
        }
      }
      break;

      case IDC_PUSHBUTTON_CLOSE : // Замкнуть. Close.
      {
        if ( contour != nullptr ) {
          MbLineSegment * seg = new MbLineSegment( currPoint, startPoint );
          contour->AddSegment( seg );
          contour->CalculateParamLength(); // Вычислить параметрическую длину. Calculate a parametric length.
        }
      }
      break;

      case IDC_PUSHBUTTON_EXIT : // Выход. Exit.
      {
        if ( contour != nullptr )
          contour->CalculateParamLength(); // Вычислить параметрическую длину. Calculate a parametric length.
      }

      break;
    }

  } while ( ( res != IDC_PUSHBUTTON_CLOSE ) && ( res != IDC_PUSHBUTTON_EXIT ) );

  if (obj != nullptr)
    TestVariables::viewManager->DeleteObject( obj );

  if ( contour != nullptr )
    TestVariables::viewManager->AddObject( TestVariables::ELEMENTARY_Style, contour, &place );
}


//------------------------------------------------------------------------------
// Создание контура в пространстве по множеству ребер. Первая точка - ближайшая к p.
// Construct a space contour by set of edges. First point is the closest point to p.
// ---
static MbCurve3D * MakeCurve3D( RPArray<MbCurveEdge> & edges )
{
  ptrdiff_t count = edges.Count();

  if ( count < 1 )
    return nullptr;

  ptrdiff_t i = 0;
  // KOMPAS-43130 MbCurve3D * curve = edges[i]->MakeCurve();
  MbCurve3D * curve = (MbCurve3D * )&( edges[i]->GetCurve().Duplicate() );

  if ( (curve == nullptr) || count < 2 )
    return curve;

  for ( i = edges.MaxIndex(); i > 0; i-- ) {
    if ( edges[i]->IsConnectedWith( *edges[i-1] ) == nullptr )
      edges.DetachInd( i );
  }

  count = edges.Count();
  if ( count < 2 )
    return curve;

  MbCartPoint3D p1, p2, r0, r1, r2;
  double t;

  MbContour3D * cntr = new MbContour3D();
  t = curve->GetTMin();
  curve->PointOn( t, p1 );
  t = curve->GetTMax();
  curve->PointOn( t, p2 );

  MbCurve3D * curve0 = curve;
  for ( i = 1; i < count; i++ ) {
    curve = edges[i]->MakeCurve();
    if ( curve != nullptr ) {
      t = curve->GetTMin();
      curve->PointOn( t, r1 );
      t = curve->GetTMax();
      curve->PointOn( t, r2 );

      if ( i > 1 ) {
        if ( p2.DistanceToPoint(r1) > p2.DistanceToPoint(r2) ) {
          curve->Inverse();
          r0 = r1;
          r1 = r2;
          r2 = r0;
        }
      }
      else {
        double p1r1, p1r2, p2r1, p2r2;
        p1r1 = p1.DistanceToPoint(r1);
        p1r2 = p1.DistanceToPoint(r2);
        p2r1 = p2.DistanceToPoint(r1);
        p2r2 = p2.DistanceToPoint(r2);

        if ( p2r2 < p2r1 && p2r2 < p1r2 ) {
          curve->Inverse();
          r0 = r1;
          r1 = r2;
          r2 = r0;
        }
        else {
          if ( p1r1 < p1r2 && p1r1 < p2r1 ) {
            curve0->Inverse();
            r0 = p1;
            p1 = p2;
            p2 = r0;
          }
          else {
            if ( p1r2 < p1r1 && p1r2 < p2r2 ) {
              curve0->Inverse();
              r0 = p1;
              p1 = p2;
              p2 = r0;
              curve->Inverse();
              r0 = r1;
              r1 = r2;
              r2 = r0;
            }
            if ( curve0 ) {
              cntr->AddSegment (*curve0, true );
            }
          }
        }
      }

      cntr->AddSegment( *curve, true );
      curve0 = curve;
      p1 = r1;
      p2 = r2;
    }
  }

  return cntr;
}


//------------------------------------------------------------------------------
// Создание контура по ребрам тела.
// Construct a contour by solid edges.
// ---
bool CreateEdgeContour()
{
  MbItem * obj  = nullptr;
  MbItem * obj0 = nullptr;
  MbCurve3D * curv = nullptr;
  MbTopItem * curve = nullptr;
  RPArray<MbCurveEdge> initCurves( 4, 4 );

  if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &curve ) ) {
    TestVariables::viewManager->ToggleObject(obj);
//#ifdef _DRAWGI
    TestVariables::drawImp->DrawItem( curve, TRGB_WHITE );
//#endif
    initCurves.Add((MbCurveEdge *)curve);
    while ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj0, &curve ) ) {
      if (obj==obj0) {
//#ifdef _DRAWGI
        TestVariables::drawImp->DrawItem( curve, TRGB_WHITE );
//#endif
        initCurves.Add((MbCurveEdge *)curve);
      }
    }
  }

  if (initCurves.Count()>0) {
    curv = MakeCurve3D( initCurves );
    TestVariables::viewManager->ToggleObject(obj);
  }

  if (curv != nullptr)
    TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, curv );
  
  return (curv != nullptr);
}


//------------------------------------------------------------------------------
// Создание контура по ребрам грани.
// Construct a contour by face edges.
// ---
bool CreateFaceEdgeContour()
{
  MbItem * obj  = nullptr;
  MbTopItem * surface = nullptr;

  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &surface ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    MbFace * face = (MbFace *)surface;

    for ( ptrdiff_t i = 0, iCount = face->GetLoopsCount(); i < iCount; i++ ) {
      MbLoop * loop = face->GetLoop( i );
      if ( loop == nullptr )
        continue;
      MbContour3D & cntr = loop->MakeContour3D();
      TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, &cntr );
    }
    TestVariables::viewManager->ToggleObject(obj);
  }
  
  return (surface != nullptr);
}


//------------------------------------------------------------------------------
// Проекция грани на плоскость.
// The projection of the face on the plane.
// ---
bool CreateFaceProjection()
{
  MbItem      * obj     = nullptr;
  MbTopItem * surface = nullptr;
  MbTopItem * plane   = nullptr;

  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &surface ) ) {
    TestVariables::viewManager->ToggleObject(obj);

//#ifdef _DRAWGI
    TestVariables::drawImp->DrawItem( surface, TRGB_WHITE );
//#endif

    MbFace * face = (MbFace *)surface;
    WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();
    MbPlacement3D place(currentView3D->GetNullPlacement());

    if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &plane ) ) {
      ((MbFace *)plane)->GetPlacement( &place ); 

      RPArray<MbCurveEdge> edges( 0, 1 );
      face->GetEdges( edges ); // Выдать множество математических ребер грани. Get a set of math edges of the face.
      size_t edgesCount = edges.Count();
      RPArray<MbCurve3D> outlines(edgesCount,1);

      MbMatrix3D into;
      place.GetMatrixInto( into );

      int cls = 0;
      if ( GetInt( IDS_ENTER_CLOSED, cls ) ) {
        cls = (cls > 0) ? 1 : 0;

        size_t i;

        if ( cls > 0 ) {
          MbContour * cntr2d = new MbContour();

          for ( i = 0; i < edgesCount; i++ ) {
            MbCurve * curve2d = ::GetFlatCurve( edges[i]->GetCurve(), into );
            if ( curve2d != nullptr ) {
              if ( !cntr2d->AddCurveWithRuledCheck( *curve2d, METRIC_PRECISION ) )
                ::DeleteItem( curve2d );
            }
          }

          if ( cntr2d->GetSegmentsCount() < 1 )
            ::DeleteItem( (MbCurve *&)cntr2d );

          if ( cntr2d != nullptr )
            outlines.Add( new MbPlaneCurve( place, *cntr2d, true ) );
        }
        else {
          for ( i = 0; i < edgesCount; i++ ) {
            MbCurve * curve2d = ::GetFlatCurve( edges[i]->GetCurve(), into );
            if ( curve2d != nullptr )
              outlines.Add( new MbPlaneCurve( place, *curve2d, true ) );
          }
        }

        TestVariables::viewManager->ToggleObject(obj);

        for ( i = 0; i < outlines.Count(); i++ ) 
          TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, outlines[i] );
      }

      return (outlines.Count() > 0);
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Создать кривую по двум плоским проекциям.
// Construct a curve by two flat projections.
// ---
bool CreateCurveByTwoProjections()
{
  bool isDone = false;

  MbItem * obj1 = nullptr;
  MbItem * obj2 = nullptr;

  SPtr<MbCurve3D> curve1;
  SPtr<MbCurve3D> curve2;

  if ( GetGObj( IDS_SELECT_FIRST_CURVE, st_Curve3D, &obj1 ) ) {
    if ( !::GetSpaceCurve( *obj1, true, curve1 ) ) {
      if ( GetMObj( IDS_SELECT_CRV_ON_PLANE, pt_Curve, &obj1 ) )
        ::GetSpaceCurve( *obj1, true, curve1 );
    }
  }

  if ( GetGObj( IDS_SELECT_SECOND_CURVE, st_Curve3D, &obj2 ) ) {
    if ( !::GetSpaceCurve( *obj2, true, curve2 ) ) {
      if ( GetMObj( IDS_SELECT_CRV_ON_PLANE, pt_Curve, &obj2 ) )
        ::GetSpaceCurve( *obj2, true, curve2 );
    }
  }

  if ( curve1 != nullptr && curve2 != nullptr ) {
    MbPlacement3D place1, place2;
    SPtr<MbCurve> pCurve1;
    SPtr<MbCurve> pCurve2;
    curve1->GetPlaneCurve( pCurve1, place1, true );
    curve2->GetPlaneCurve( pCurve2, place2, true );

    if ( pCurve1 != nullptr && pCurve2 != nullptr  ) {
      RPArray<MbCurve3D> resCurves( 0, 1 );

      ::CurveByTwoProjections( place1, *pCurve1, place2, *pCurve2, resCurves );

      if ( resCurves.Count() > 0 ) {
        for ( size_t k = 0, cnt = resCurves.Count(); k < cnt; k++ ) {
          MbCurve3D * resCurve = resCurves[k];
          if ( resCurve != nullptr )
            TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, resCurve );
        }
        MessageBoxEx( IDS_INFO_ITEMS_CREATED, IDS_TITLE_MSG );
        isDone = true;
      }
      else {
        MessageBoxEx( IDS_INFO_ITEMS_NOT_CREATED, IDS_TITLE_ERR );
      }

      ::DeleteItems( resCurves );
    }
  }

  return isDone;
}


//------------------------------------------------------------------------------
// Кривую на поверхности по точкам.
// Construct a curve on the surface by points.
// ---
bool CreateCurveOnSurfaceByPoints( const MbItem *& modelItem )  
{
  bool isDone = false;

  if ( modelItem == nullptr )
    GetGObj( IDS_SELECT_SURFACE, st_Surface, &((MbItem *&)modelItem) );

  if ( modelItem != nullptr ) {
    const MbSpaceItem * spaceItem = ((MbSpaceInstance *)modelItem)->GetSpaceItem();

    if ( spaceItem != nullptr && (spaceItem->Family() == st_Surface) ) {
      const MbSurface & surface = (const MbSurface &)(*spaceItem);

      SArray<MbCartPoint> paramPnts( 0, 1 );
      SArray<double>      paramWts ( 0, 1 );
      PArray<PntMatingData3D> transitions( 0, 1, true );

      // Нужно ли проводить через точки.
      // Construct it through points or not.
      bool throughPoints = GetBoolean( IDS_ENTER_THROUGH_ITEMS_BOOL );

      // Нужно ли задавать сопряжения.
      // Set conjugations or not.
      bool useTransitions = false;
      {
        int iUseTransitions = 0;
        GetInt( IDS_ENTER_USE_OR_NOT, iUseTransitions );
        useTransitions = (iUseTransitions != 0);
      }

      // Ввод точек.
      // Input of the points.
      bool ext = false;
      MbCartPoint paramPnt;
      MbCartPoint3D spacePnt, spacePntOn;

      while ( GetPoint( IDS_SELECT_POINT, spacePnt ) ) {
        surface.NearPointProjection( spacePnt, paramPnt.x, paramPnt.y, ext );
        paramPnts.Add( paramPnt );
        surface._PointOn( paramPnt.x, paramPnt.y, spacePntOn );
//#ifdef _DRAWGI
        TestVariables::drawImp->DrawLine( spacePnt, spacePntOn, TRGB_BROWN );
        TestVariables::drawImp->DrawPoint( &spacePnt, TRGB_RED );
        TestVariables::drawImp->DrawPoint( &spacePntOn, TRGB_BLUE );
//#endif // _DRAWGI
        if ( useTransitions ) { // Ввод сопряжения. Input of the conjugation.
          PntMatingData3D * transition = nullptr;

          MbeMatingType trType = trt_Tangent;
          if ( GetInt( IDS_ENTER_CONN_TYPE, (int &)trType ) && 
               trType >= trt_None && trType <= trt_SmoothG3 )
          {
            MbVector3D dir;

            if ( trType > trt_Position ) {
              double angle = 0.0;
              GetDouble( IDS_ENTER_ANGLE, angle );
              angle = M_PI * angle / 180.0;

              MbVector3D uDer, vDer;
              surface._PointOn( paramPnt.x, paramPnt.y, spacePnt );
              surface._DeriveU( paramPnt.x, paramPnt.y, uDer );
              surface._DeriveV( paramPnt.x, paramPnt.y, vDer );

              MbAxis3D rotAxis( spacePnt, (uDer | vDer ) );
              dir = uDer;
              dir.Rotate( rotAxis, angle );

              double dirLen = dir.Length();
              double newLen = dirLen;
              GetDouble( IDS_ENTER_DISTANCE, newLen );
              if ( newLen > 0.0 ) {
                dir /= dirLen;
                dir *= newLen;
                dirLen = newLen;
              }
//#ifdef _DRAWGI
              TestVariables::drawImp->DrawLine( spacePnt, dir, TRGB_ORANGE );
//#endif // _DRAWGI
            }

            if ( trType != trt_None )
              transition = new PntMatingData3D;

            if ( transition != nullptr ) {
              SArray<intptr_t> * changedInds = nullptr;
              switch ( trType ) {
                case trt_Position:
                  transition->Init( trType, nullptr, nullptr, nullptr, changedInds, !throughPoints, false );
                case trt_Tangent:
                  transition->Init( trType, &dir, nullptr, nullptr, changedInds, !throughPoints, false );
                  break;
                case trt_Normal :
                  transition->Init( trType, &dir, nullptr, nullptr, changedInds, !throughPoints, false );
                  break;
                case trt_SmoothG2 :
                  MessageBoxEx( RT_MATE_NOT_IMPLEMENTED, IDS_TITLE_WRN );
                  if ( transition != nullptr ) {
                    delete transition;
                    transition = nullptr;
                  }
                  break;
                break;
                case trt_SmoothG3 :
                  MessageBoxEx( RT_MATE_NOT_IMPLEMENTED, IDS_TITLE_WRN );
                  if ( transition != nullptr ) {
                    delete transition;
                    transition = nullptr;
                  }
                  break;

                default: break;
              }
            }

            transitions.Add( transition );
          }
          else {
            transitions.Add( nullptr );
          }
        }
      }

      size_t pntsCnt = paramPnts.Count();
      if ( pntsCnt > 1 ) {
        SetWaitCursor( true );

        bool paramClosed = GetBoolean( IDS_ENTER_CLOSED_BOOL );

        bool checkNurbsCreation = false;
        if ( checkNurbsCreation && throughPoints && paramPnts.Count() > 1 ) {
          SArray<MbCartPoint3D> spacePnts;
          for ( size_t k = 0; k < paramPnts.Count(); k++ ) {
            surface._PointOn( paramPnts[k].x, paramPnts[k].y, spacePnt );
            spacePnts.Add( spacePnt );
          }
          SArray<double> params( 0, 1 );
          ::DefineThroughPointsParams( spacePnts, spt_ChordLength, false, params );

          MbNurbs3D * nurbs1 = nullptr;
          MbNurbs3D * nurbs2 = nullptr;
#ifdef C3D_DEBUG
          nurbs1 = MbNurbs3D::CreateNURBS4( spacePnts, params, false );
          nurbs2 = MbNurbs3D::CreateThrough( 4, false, spacePnts, params );
          PRECONDITION( nurbs1 != nullptr );
          PRECONDITION( nurbs2 != nullptr );
#else
          nurbs1 = MbNurbs3D::CreateNURBS4( spacePnts, params, false );
          nurbs2 = MbNurbs3D::CreateThrough( 4, false, spacePnts, params );
#endif
          TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, nurbs1 );
          TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, nurbs2 );
        }

        /*
        if ( transitions.empty() ) {
          SArray<ptrdiff_t> * inds = nullptr;
          for ( size_t k = 0, pntsCnt = paramPnts.size(); k < pntsCnt; ++k ) {
            PntMatingData3D * transition = new PntMatingData3D( MbeMatingType::trt_Position, nullptr, nullptr, nullptr, inds, false, true );
            transitions.push_back( transition );
          }
        }
        // */

        MbWireFrame * resFrame = nullptr;
        MbSNameMaker operNames( ct_Curve3DCreator, MbSNameMaker::i_SideNone, 0 );
        MbResultType resType = ::SurfaceSpline( surface, throughPoints, paramPnts, paramWts, paramClosed,
                                                transitions, operNames, resFrame );

        if ( resType == rt_Success || resType == rt_SelfIntersect ) {
          PRECONDITION( resFrame != nullptr );
          uintptr_t curvesCnt = (resFrame != nullptr) ? resFrame->GetEdgesCount() : 0;

          if ( curvesCnt > 0 ) {
            TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, resFrame );
            isDone = true;
          }
          else {
            MessageBoxEx( RT_MISMATCH_IN_OPERATIONS_RESULTS, IDS_TITLE_ERR );
          }
        }
        else {
          MessageBoxEx( RT_CURVE_NOT_CREATED, IDS_TITLE_ERR );
        }

        ::DeleteItem( resFrame );

        SetWaitCursor( false );
      }
      else {
        MessageBoxEx( RT_TOO_FEW_POINTS, IDS_TITLE_ERR );
      }
    }
    else {
      MessageBoxEx( RT_SELECTED_OBJECT_ERROR, IDS_TITLE_ERR );
    }
  }
  else {
    MessageBoxEx( RT_SURFACE_NOT_SELECTED, IDS_TITLE_ERR );
  }

  return isDone;
}


//------------------------------------------------------------------------------
// Создать проекционную кривую на поверхности.
// Construct a projection curve on the surface.
// 1. Если на входе - кривая, то на выходе - кривая.
// 2. Если на входе - каркас, то на выходе - каркас.
// ---
bool CreateProjectionCurveOnSurface()
{
  bool isDone = false;

  MbItem * modelItem = nullptr;
  SpaceCurveSPtr curve;
  bool firstItemEntered = false;
  MbSolid * solid = nullptr;
  bool isWireFrame = false;  // каркас или кривая?
  WireFrameSPtr wireframe;

  // проверяем объект какого типа выбрал пользователь
  if ( GetObj( IDS_SELECT_CURVE, &modelItem ) && modelItem != nullptr ) {
    if ( modelItem->IsA() == st_WireFrame ) {    // каркас
      wireframe = static_cast<MbWireFrame *>( modelItem );
      TestVariables::viewManager->ToggleObject( modelItem );
      isWireFrame = true;
      firstItemEntered = true;
    }
    else {
      curve = ::GetCurve3D( modelItem, false ); // кривая
      isWireFrame = false;
      firstItemEntered = ( curve != nullptr );
      if ( firstItemEntered )
        TestVariables::viewManager->ToggleObject( modelItem );
    }
  }

  if ( firstItemEntered ) {
    if ( curve != nullptr || wireframe != nullptr ) {
      modelItem = nullptr;
      RPArray<MbItem> modelItems( 0 , 1 );
      RPArray<MbFace> faces( 0, 1 );

      if ( GetGObj( IDS_SELECT_SHELL, st_Solid, &modelItem ) &&
           modelItem != nullptr && modelItem->IsA() == st_Solid ) {
          solid = (MbSolid *) modelItem;

        solid->GetFaces( faces );
        if ( faces.Count() > 0 ) {
          TestVariables::viewManager->ToggleObject( modelItem );
          modelItems.Add( modelItem );
        }
      }

      if ( faces.Count() < 1 ) {
        modelItem = nullptr;
        MbTopItem * tItem = nullptr;
        if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &modelItem, &tItem ) && 
             tItem != nullptr && tItem->IsAFace() )
        {
          MbFace * face = (MbFace *)tItem;

          TestVariables::viewManager->ToggleObject( modelItem );
          modelItems.Add( modelItem );

//#ifdef _DRAWGI
          TestVariables::drawImp->DrawItem( face, TRGB_WHITE );
//#endif // _DRAWGI
          faces.Add( face );

          modelItem = nullptr;
          tItem = nullptr;
          while ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &modelItem, &tItem ) && 
                  tItem != nullptr && tItem->IsAFace() )
          {
            face = (MbFace *)tItem;

            if ( modelItems.FindIt( modelItem ) == SYS_MAX_T ) {
              TestVariables::viewManager->ToggleObject( modelItem );
              modelItems.Add( modelItem );
            }

            uintptr_t ind = faces.FindIt( face );

            if ( ind == SYS_MAX_T ) {
//#ifdef _DRAWGI
              TestVariables::drawImp->DrawItem( face, TRGB_WHITE );
//#endif // _DRAWGI
              faces.Add( face );
            }
            else {
//#ifdef _DRAWGI
              TestVariables::drawImp->DrawItem( face, TRGB_LIGHTGREEN );
//#endif // _DRAWGI
              faces.DetachInd( ind );
            }
//#ifdef _DRAWGI
            for ( uintptr_t k = 0, kcnt = faces.Count(); k < kcnt; k++ )
              TestVariables::drawImp->DrawItem( faces[k], TRGB_WHITE  );
//#endif // _DRAWGI
            modelItem = nullptr;
            tItem = nullptr;
          }
        }
        else {
          RPArray<MbSurface> surfaces( 0, 1 );

          modelItem = nullptr;
          MbSpaceItem * sItem = nullptr;
          while ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &modelItem, &sItem ) && 
                  sItem != nullptr && (sItem->Family() == st_Surface) )
          {
            if ( modelItems.FindIt( modelItem ) == SYS_MAX_T ) {
              TestVariables::viewManager->ToggleObject( modelItem );
              modelItems.Add( modelItem );
            }

            uintptr_t ind = surfaces.FindIt( static_cast<MbSurface *>(sItem) );

            if ( ind == SYS_MAX_T ) {
              MbFace * newFace = new MbFace( static_cast<MbSurface &>(sItem->Duplicate()), true );
//#ifdef _DRAWGI
              TestVariables::drawImp->DrawItem( sItem, TRGB_WHITE );
//#endif // _DRAWGI
              faces.Add( newFace );
              surfaces.Add( (MbSurface *)sItem );
            }
            else {
//#ifdef _DRAWGI
              TestVariables::drawImp->DrawItem( sItem, TRGB_GREEN );
//#endif // _DRAWGI
              ::DeleteItem( faces[ind] );
              faces.DetachInd( ind );
              surfaces.DetachInd( ind );
            }
//#ifdef _DRAWGI
            for ( uintptr_t k = 0, kcnt = faces.Count(); k < kcnt; k++ )
              TestVariables::drawImp->DrawItem( faces[k], TRGB_WHITE  );
//#endif // _DRAWGI
            modelItem = nullptr;
            sItem = nullptr;
          }

          surfaces.DetachAll();
        }
      }

      if ( faces.Count() > 0 ) {
        MbVector3D * dir = nullptr;
        MbCartPoint3D dirPnt;
        if ( GetPoint( IDS_ENTER_DIR, dirPnt ) )
          dir = new MbVector3D( dirPnt );

        bool createExact = true;
        bool truncateByBounds = GetBoolean( IDS_ENTER_TRUNCATE_BOOL );

        MbSNameMaker snMaker( ct_Curve3DCreator, MbSNameMaker::i_SideNone, 0 );

        RPArray<MbCurve3D> resCurves ( 0, 1 );
        SArray<size_t>     resIndices( 0, 1 );

        MbResultType res = rt_Error;

        bool bySegments = false; // Проецировать контур по-сегментно. Project the contour segment by segment.
        if ( !isWireFrame && curve->Type() == st_Contour3D ) {
          bySegments = GetBoolean( IDS_ENTER_ELEMENTWISE_BOOL );
          if ( bySegments ) {
            const MbContour3D * cntr = static_cast<const MbContour3D *>(curve.get());
            RPArray<MbCurve3D> locCurves( 0, 1 );
            SArray<size_t>     locIndices( 0, 1 );

            for ( size_t k = 0, segmentsCnt = cntr->GetSegmentsCount(); k < segmentsCnt; k++ ) {
              const MbCurve3D * segm = cntr->GetSegment( k );
              ConstFacesVector facesData;

              ::DeleteItems( locCurves );
              locIndices.clear();
              facesData.reserve( faces.size() );

              for ( MbFace * f : faces ) {
                facesData.push_back( f );
              }

              MbProjectionWireFrameParams params( *segm, facesData, dir, false, createExact, truncateByBounds, snMaker );
              MbProjectionWireFrameResults results;
              MbResultType locRes = ::ProjectionWireFrame( params, results );

              if ( locRes == rt_Success ) {
                results.DetachCurves( locCurves );
                results.GetResIndices( locIndices );
                res = locRes;
                resCurves.AddArray( locCurves );
                resIndices += locIndices;
                locCurves.clear();
                locIndices.clear();
              }
            }

            if ( resCurves.Count() > 0 ) {
              ::EliminateProjectionCurveOverlay( resCurves, &resIndices );
            }
          }
        }

        if ( !isWireFrame && !bySegments ) {
          ConstFacesVector facesData;

          facesData.reserve( faces.size() );

          for ( MbFace * f : faces ) {
            facesData.push_back( f );
          }

          MbProjectionWireFrameParams params( *curve, facesData, dir, false, createExact, truncateByBounds, snMaker );
          MbProjectionWireFrameResults results;

          res = ::ProjectionWireFrame( params, results );
          results.DetachCurves( resCurves );
          results.GetResIndices( resIndices );
        }

        if ( isWireFrame ) {
          MbWireFrame * resFrame = nullptr;
          
          size_t cnt = solid->GetFacesCount();
          IndicesVector indices( cnt );

          std::iota( indices.begin(), indices.end(), 0 );

          MbProjectionWireFrameParams params( *wireframe, true, *solid, true, indices, dir, false,
                                              createExact, truncateByBounds, snMaker );
          MbProjectionWireFrameResults results;

          res = ::ProjectionWireFrame( params, results );
          resFrame = results.DetachWireFrame();

          if ( resFrame != nullptr ) {
            for ( size_t i = 0, iCnt = resFrame->GetEdgesCount(); i < iCnt; i++ ) {
              const MbEdge * edge = resFrame->GetEdge( i );
              MbNameAttribute nameAttr;
              nameAttr.Init( *edge->GetSimpleAttribute( nameAttr.AttributeType() ) );
              resIndices.Add( solid->GetFaceIndex( *solid->FindFaceByName( nameAttr.GetName() ) ) );
            }
            
            TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, resFrame );
//            ::ExtractCurvesDeleteFrame( resFrame, resCurves );
          }
        }

        if ( !isWireFrame )
        {
          if ( (res == rt_Success) == (resCurves.Count() > 0) ) {
            ::CreateContours( resCurves, Math::metricPrecision );
            
            uintptr_t curvesCnt = resCurves.Count();
            
            if ( curvesCnt > 0 ) {
              for ( uintptr_t k = 0; k < curvesCnt; k++ )
                TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, resCurves[k] );
              
              MessageBoxEx( RT_PROJ_CURVES_CREATED, IDS_TITLE_MSG );
              isDone = true;
            }
            else {
              if ( res == rt_NoIntersect )
                MessageBoxEx( RT_PROJ_CURVE_OUT_FACES, IDS_TITLE_ERR );
              else
                MessageBoxEx( RT_PROJ_CURVE_ERROR, IDS_TITLE_ERR );
            }
          }
          else {
            MessageBoxEx( RT_MISMATCH_IN_OPERATIONS_RESULTS, IDS_TITLE_ERR );
          }
        }
        if ( dir != nullptr ) {
          delete dir;
          dir = nullptr;
        }

        ::DeleteItems( resCurves );
        resIndices.Flush();
      }
      else
      {  
        MessageBoxEx( RT_NOT_SELECTED_FACE_OR_SURFACE, IDS_TITLE_WRN );
      }
      
      ::DeleteItems( faces );
      for ( uintptr_t l = 0, modelItemsCnt = modelItems.Count(); l < modelItemsCnt; l++ )
        TestVariables::viewManager->ToggleObject( modelItems[l] );

      if ( isWireFrame )
        TestVariables::viewManager->ToggleObject( wireframe );
    }
    else
    {  
      MessageBoxEx( RT_SELECT_CURVE_ERROR, IDS_TITLE_ERR );
    }
  }
  else
  {  
    MessageBoxEx( RT_CURVE_NOT_SELECTED, IDS_TITLE_WRN );
  }
  
  return isDone;
}


//------------------------------------------------------------------------------
// Создать осевые (центральные) линии для грани оболочки.
// Create center lines of a shell face.
// ---
bool CreateFaceCenterLines()
{
  bool isDone = false;
  MbItem * obj = nullptr;
  MbTopItem * topoItem = nullptr;

  if ( GetTObj(IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &topoItem) ) {
    MbFace * face = static_cast<MbFace *>( topoItem );
    if ( face != nullptr ) {
      c3d::SpaceCurvesVector clCurves;
      if ( ::CreateCenterLineCurves(*face, clCurves) ) {
        for ( size_t clInd = 0, clCount = clCurves.size(); clInd < clCount; clInd++ ) {
          if ( clCurves[clInd] != nullptr ) {
            TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, clCurves[clInd] );
            isDone = true;
          }
        }
      }
      else
        MessageBoxEx( RT_EMD_WARNING_UNDIFINED_CURVE, IDS_TITLE_MSG );
    }
    else
      MessageBoxEx( RT_SELECTED_OBJECT_ERROR, IDS_TITLE_ERR );
  }
  else
    MessageBoxEx( RT_SELECTED_OBJECT_ERROR, IDS_TITLE_ERR );
  return isDone;
}


//------------------------------------------------------------------------------
// Кривая между вершинами грани. Segment by vertexes of a face.
// ---
void CreateCurveBetweenVertices() 
{
  MbItem * obj = nullptr;
  MbItem * obj1 = nullptr,  * obj2 = nullptr;
  MbTopItem * topoItem = nullptr;
  MbTopItem * topoItem1 = nullptr, * topoItem2 = nullptr;

  if ( GetTObj(IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &topoItem) ) {
    if ( topoItem != nullptr && topoItem->IsA() == tt_Face ) {
      MbFace * face = static_cast<MbFace *>( topoItem );

      if ( GetTObj(IDS_SELECT_FACE, st_Solid, tt_Vertex, &obj1, &topoItem1) &&
           GetTObj(IDS_SELECT_FACE, st_Solid, tt_Vertex, &obj2, &topoItem2)) {
        if ( obj1 == obj && obj2 == obj &&
             topoItem1 != nullptr && topoItem1->IsA() == tt_Vertex &&
             topoItem1 != nullptr && topoItem1->IsA() == tt_Vertex ) {
          MbVertex * vert1 = static_cast<MbVertex *>( topoItem1 );
          MbVertex * vert2 = static_cast<MbVertex *>( topoItem2 );

          size_t indLoop1, indLoop2, indEdge1, indEdge2;
          // Принадлежит ли вершина грани? Does a vertex belong an edge? 
          if ( face->IsVertexOn( vert1, &indLoop1, &indEdge1 ) &&
               face->IsVertexOn( vert2, &indLoop2, &indEdge2 ) ) {
            MbLoop * loop1 = face->GetLoop( indLoop1 );
            MbLoop * loop2 = face->GetLoop( indLoop2 );
            if ( loop1 != nullptr && loop2 != nullptr ) {
              const MbCurveEdge & edge1 = loop1->GetOrientedEdge( indEdge1 )->GetCurveEdge();
              const MbCurveEdge & edge2 = loop2->GetOrientedEdge( indEdge2 )->GetCurveEdge();
              bool orient1 = loop1->GetOrientedEdge( indEdge1 )->GetOrientation();
              bool orient2 = loop2->GetOrientedEdge( indEdge2 )->GetOrientation();
              double t1 = (&edge1.GetBegVertex() == vert1) ? edge1.GetTBegVertex() : edge1.GetTEndVertex();
              double t2 = (&edge2.GetBegVertex() == vert2) ? edge2.GetTBegVertex() : edge2.GetTEndVertex();
              MbCartPoint p1, p2;

              if ( edge1.PointOn( face, orient1, t1, p1 ) &&
                   edge2.PointOn( face, orient2, t2, p2 ) ) {
                MbSurfaceCurve * sCurve = new MbSurfaceCurve( face->GetSurface().GetSurface(), p1, p2, pt_LineSegment );
                Style style = TestVariables::WIRE_Style;
                TestVariables::viewManager->AddObject( style, sCurve );
                ::DeleteItem( sCurve );
              }
            }
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создать сплайн на поверхности грани между заданными точками.
// Create a spline on the surface of the face between between the specified points.
// ---
void CreateFaceCurve()
{
  MbItem * obj = nullptr;
  MbTopItem * topoItem = nullptr;

  if ( GetTObj(IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &topoItem) ) {
    if ( topoItem != nullptr && topoItem->IsA() == tt_Face ) {
      MbFace * face = static_cast<MbFace *>( topoItem );
      std::vector<MbCartPoint3D> points;
      MbCartPoint3D p;

      while ( GetPoint( IDS_SELECT_POINT, p ) ) { // Набиранем точки.
        points.push_back( p );
      }

      size_t pCount = points.size();

      if ( pCount >= 2 ) {
        MbCartPoint3D p1( points[0] ), p2( points[pCount - 1] ); // Первая и последняя точка определяют края кривой.
        WorkWindow * currentView = TestVariables::viewManager->GetActiveWindow();
        MbPlacement3D plane( currentView->GetNullPlacement() );
        MbVector3D axisZ( plane.GetAxisZ() );
        MbCartPoint3D q1( p1 ), q2( p2 );
        q1.Add( axisZ, 10 );
        q2.Add( axisZ, 10 );
        MbLineSegment3D segment1( p1, q1 );
        MbLineSegment3D segment2( p2, q2 );
        const MbSurfaceIntersectionCurve * intCurve1( nullptr );
        const MbSurfaceIntersectionCurve * intCurve2( nullptr );
        double t1, t2, w1, w2, tM1 = 0.0, tM2 = 0.0, tS1, tS2;
        double d1, d2, d1Min = MB_MAXDOUBLE, d2Min = MB_MAXDOUBLE;

        for ( size_t k = 0, kCount = face->GetLoopsCount(); k < kCount; k++ ) {
          MbLoop * loop = face->GetLoop( k );
          for ( size_t j = 0, jCount = loop->GetEdgesCount(); j < jCount; j++ ) {
            const MbCurveEdge & edge = loop->GetOrientedEdge(j)->GetCurveEdge();
            const MbSurfaceIntersectionCurve & curve = edge.GetIntersectionCurve();
            ::NearestPoints( (const MbCurve3D &)segment1, true, (const MbCurve3D &)curve, false, w1, t1, d1 );
            ::NearestPoints( (const MbCurve3D &)segment2, true, (const MbCurve3D &)curve, false, w2, t2, d2 );
            if ( d1Min > d1 ) {
              d1Min = d1;
              intCurve1 = &curve;
              tM1 = t1;
              tS1 = w1;
            }
            if ( d2Min > d2 ) {
              d2Min = d2;
              intCurve2 = &curve;
              tM2 = t2;
              tS2 = w2;
            }
          }
        }

        if ( (intCurve1 != nullptr) && (intCurve2 != nullptr) ) {
          const MbSurface * surfaceOne1 = intCurve1->GetSurfaceOne();
          const MbSurface * surfaceOne2 = intCurve2->GetSurfaceOne();
          const MbSurface & surface = face->GetSurface().GetSurface();
          MbCartPoint r1, r2, r;

          if ( &surface == surfaceOne1 )
            intCurve1->PointOn( tM1, r1, r );
          else
            intCurve1->PointOn( tM1, r, r1 );

          if ( &surface == surfaceOne2 )
            intCurve2->PointOn( tM2, r2, r );
          else
            intCurve2->PointOn( tM2, r, r2 );

          const MbSurface & bSurface = face->GetSurface();
          SArray<double> params;
          SArray<MbCartPoint> rPoints;
          double len = 0.0;

          bSurface._PointOn( r1.x, r1.y, q1 );
          params.reserve( pCount );
          rPoints.reserve( pCount );
          params.push_back( len );
          rPoints.push_back( r1 );
          for ( size_t i = 1; i < pCount - 1; i++ ) { // Промежуточные точки кривой.
            if ( bSurface.NearDirectPointProjection( points[i], axisZ, r.x, r.y, false, nullptr, false ) ) {
              bSurface._PointOn( r.x, r.y, q2 );
              len += q1.DistanceToPoint( q2 );
              params.push_back( len );
              rPoints.push_back( r );
              q1 = q2;
            }
          }
          bSurface._PointOn( r2.x, r2.y, q2 );
          len +=  q1.DistanceToPoint( q2 );
          params.push_back( len );
          rPoints.push_back( r2 );
          ::SetLimitParam( params, 0.0, 1.0 );
          {
            int nc = (int)rPoints.size();
            ::GetInt( IDS_FUNCTION_TYPE, nc );
            MbCurve * curve( nullptr );
            size_t deg = NURBS_DEGREE + NURBS_DEGREE;
            if ( nc > 1 && deg <= rPoints.size() )
              curve = MbNurbs::CreateThrough( deg, false, rPoints, params );
            if ( curve == nullptr )
              curve = MbHermit::Create( params, rPoints, false );
            if ( curve != nullptr ) {
              MbSurfaceCurve * sCurve = new MbSurfaceCurve( surface, *curve, true );
              Style style = TestVariables::WIRE_Style;
              TestVariables::viewManager->AddObject( style, sCurve );
              ::DeleteItem( sCurve );
            }
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создать проекцию кривой на поверхности грани.
// Create a curve projection on the surface of the face.
// ---
bool CreateCurveFaceProjection()
{
  MbItem * obj = nullptr;
  MbTopItem * topoItem = nullptr;
  bool res = false;

  if ( GetTObj(IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &topoItem) ) {
    if ( topoItem != nullptr && topoItem->IsA() == tt_Face ) {
      TestVariables::viewManager->ToggleObject( obj );
      MbFace * face = static_cast<MbFace *>( topoItem );
      MbItem * nearest = nullptr;

      if ( face != nullptr ) { 
        if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest ) ) {
          const MbSurface & surface = face->GetSurface();
          TestVariables::viewManager->ToggleObject( nearest );
          SPtr<MbCurve3D> crv( ::GetCurve3D( nearest, false ) );
          if ( crv != nullptr ) {
            int n( TestVariables::part );
            GetInt( IDS_ENTER_EXT_FLAG, n );
            TestVariables::part = n;
            RPArray<MbCurve3D> curves( 0, 1 );
            MbProjectionWireFrameParams params( surface, *crv, nullptr, true, n <= 0 );
            MbProjectionWireFrameResults results;

            ::ProjectionWireFrame( params, results );
            results.DetachCurves( curves );

            for ( size_t i = 0, iCount = curves.Count(); i < iCount; i++ ) 
              TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, curves[i] ); 

            res = true;
          }
          TestVariables::viewManager->ToggleObject( nearest );
        }
        else { // KOMPAS-48114
          MbTopItem * topItem = nullptr;
          std::vector<const MbCurveEdge *> edges;

          while ( GetTObj(IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &topItem) ) {
            if ( topItem != nullptr && topItem->IsA() == tt_CurveEdge ) {
              MbCurveEdge * edge = static_cast<MbCurveEdge *>( topItem );
              edges.push_back( edge );
            }
          }

          MbCurve3D * result = ::MakeCurveOnSurface( *face, edges, PARAM_ACCURACY );

          if ( result != nullptr )
            TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, result ); 
          /*
          const MbSurface & surface = face->GetSurface().GetSurface();
          MbTopItem * topItem = nullptr;
          std::vector<MbCurve *> curves;
          curves.reserve( 3 );
          MbRegDuplicate * iReg( nullptr );
          MbAutoRegDuplicate autoReg( iReg );
          while ( GetTObj(IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &topItem) ) {
            if ( topItem != nullptr && topItem->IsA() == tt_CurveEdge ) {
              MbCurveEdge * edge = static_cast<MbCurveEdge *>( topItem );
              MbFace * faceP = edge->GetFacePlus(); 
              MbFace * faceM = edge->GetFaceMinus(); 
              if ( faceP != nullptr && faceM != nullptr && faceP != faceM ) {
                const MbSurfaceIntersectionCurve & intCurve = edge->GetIntersectionCurve();
//#ifdef _DRAWGI
                TestVariables::drawImp->DrawItem( &intCurve, TRGB_WHITE );
//#endif
                if ( &surface == intCurve.GetSurfaceOne() ) {
                  MbCurve & crv = static_cast<MbCurve &>( intCurve.GetCurveOneCurve().Duplicate( iReg ) );
                  curves.push_back( &crv );
                }
                if ( &surface == intCurve.GetSurfaceTwo() ) {
                  MbCurve & crv = static_cast<MbCurve &>( intCurve.GetCurveTwoCurve().Duplicate( iReg ) );
                  curves.push_back( &crv );
                }
              }
            }
          }
          if ( curves.size() == 1 ) {
            if ( curves[0] != nullptr ) {
              MbSurfaceCurve * spaceCurve = new MbSurfaceCurve( surface, *(curves[0]), true );
              TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, spaceCurve ); 
            }
          }
          else {
            MbContour * contour = ::MakeContour( curves, PARAM_ACCURACY );
          
            if ( contour != nullptr ) {
              MbContourOnSurface * spaceCurve = new MbContourOnSurface( surface, *contour, true );
              TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, spaceCurve ); 
            }
          }
          */
        }
      }
      TestVariables::viewManager->ToggleObject( obj );
    }
  }
  return res;
}


//------------------------------------------------------------------------------
// Добавить кривую в каркас
// Add Curve to Wireframe
// ---
static
void AddCurveInWireFrame( const MbCurve3D & curve, MbWireFrame & wireFrame )
{
  if ( curve.Type() == st_Contour3D ) {
    const MbContour3D & contour3d = static_cast<const MbContour3D &>( curve );
    double oldTol = 0.0;
    double newTol = 0.0;
    MbCartPoint3D commonPt, prevPt;
    MbEdge * currentEdge = nullptr;
    bool isClosed = contour3d.IsClosed();
    for ( size_t segInd = 0, segCount = contour3d.GetSegmentsCount(); segInd < segCount; ++ segInd ) {
      const MbCurve3D * segment = contour3d.GetSegment( segInd );
      if ( segment != nullptr ) {
        wireFrame.AddEdge( *segment, true );
        if ( segInd < segCount - 1 || !isClosed ){
          // Добавляем толерантные вершины в каркас.
          const MbCurve3D * segment2 = contour3d.GetSegment( (segInd + 1) % segCount );
          if ( segment2 != nullptr ) {
            size_t edgesCount = wireFrame.GetEdgesCount();
            if ( edgesCount > 0 ) {
              MbCartPoint3D endPt, begPt;
              segment->GetLimitPoint( 2, endPt );
              segment2->GetLimitPoint( 1, begPt );
              newTol = ONE_HALF * endPt.DistanceToPoint( begPt );
              commonPt.Set( endPt, ONE_HALF, begPt, ONE_HALF );
              currentEdge = wireFrame.SetEdge( edgesCount - 1 );
              if ( currentEdge != nullptr ) {
                currentEdge->SetBegVertex().SetTolerance( oldTol );
                if ( segInd > 0 )
                  currentEdge->SetBegVertex().SetCartPoint( prevPt );
                if ( segInd < segCount-1 || isClosed ) {
                  currentEdge->SetEndVertex().SetTolerance( newTol );
                  currentEdge->SetEndVertex().SetCartPoint( commonPt );
                  if ( segInd == segCount - 1 && isClosed ) {
                    currentEdge = wireFrame.SetEdge( 0 );
                    if ( currentEdge != nullptr ) {
                      currentEdge->SetBegVertex().SetTolerance( newTol );
                      currentEdge->SetBegVertex().SetCartPoint( commonPt );
                    }
                  }
                }
              }
            }
          }
        }
      }
      oldTol = newTol;
      prevPt = commonPt;
    }
  }
  else {
    wireFrame.AddEdge( curve, true );
  }
}


//------------------------------------------------------------------------------
// Создать проволочный каркас на основе кривых.
// Construct a wire frame based on the curves.
// ---
void CreateWireFrameByCurves()
{
  MbItem * curItem = nullptr;
  RPArray<MbItem> delItems( 0,1 );
  
  MbWireFrame * wireFrame = new MbWireFrame;

  bool add = true;
  while ( add ) {
    add = false;
    MbCurve3D * curve = nullptr;

    if ( GetAnyCurve( IDS_SELECT_CURVE, &curItem ) ) {
      if ( delItems.FindIt( curItem ) == SYS_MAX_T ) {
        TestVariables::viewManager->ToggleObject( curItem );
        if ( curItem->IsA() == st_SpaceInstance ) {
          curve = (MbCurve3D *)((MbSpaceInstance *)curItem)->GetSpaceItem();
          delItems.Add( curItem );
          ::AddCurveInWireFrame( *curve, *wireFrame );
          add = true;
        }
        else if ( curItem->IsA() == st_PlaneInstance ) {
          const MbPlaneInstance * planeInstance = static_cast<const MbPlaneInstance *>( curItem );
          size_t count = planeInstance->PlaneItemsCount();
          if ( count > 0 ) {
            for ( size_t i = 0; i < count; ++i ) {
              const MbCurve * pCurve = static_cast<const MbCurve *>( planeInstance->GetPlaneItem(i) );
              ::PlaneCurve( planeInstance->GetPlacement(), *pCurve, curve );
              ::AddCurveInWireFrame( *curve, *wireFrame );
            }
            delItems.Add( curItem );
            add = true;
          }
        }
      }
    }
  }

  if ( wireFrame->GetEdgesCount() == 0 )
    ::DeleteItem( wireFrame );

  if ( wireFrame != nullptr ) {
    wireFrame->NormalizeWire( MbCreatePartsParams() );

    for ( ptrdiff_t i = delItems.MaxIndex(); i >= 0; i-- ) {
      TestVariables::viewManager->ToggleObject( delItems[i] );
      TestVariables::viewManager->DeleteObject( delItems[i] );
    }
    delItems.DetachAll();

    Style style = TestVariables::WIRE_Style;
    TestVariables::viewManager->AddObject( style, wireFrame );
  }
}

//------------------------------------------------------------------------------
// Создать проволочный каркас по точкам (со скруглениями).
// Construct a wire frame by points (filleted).
//---
void CreateWireFrameByPoints()
{ 
  std::vector<MbCartPoint3D> pList;
  MbCartPoint3D pnt;

  while ( GetPoint( IDS_SELECT_POINT, pnt ) ) 
  {
    //#ifdef _DRAWGI
    TestVariables::drawImp->DrawPoint( &pnt, TRGB_LIGHTGREEN );
    //#endif
    pList.push_back( pnt );
  }

  if ( pList.size() > 1 ) 
  {    
    int iCls = 0;
    GetInt( IDS_ENTER_CLOSED, iCls );   
    {
      double radius = 0.0; // Радиус скругления. The radius of curvature.
      // Отладка скругленной полилинии (контура).
      // Checkout filleted polyline of the contour.
      bool res = pList.size() > 0;
      SArray<Polyline3DPoint> initList(pList.size(),2);
      SArray<ptrdiff_t> errorIndexes(pList.size(),2);
      for ( size_t vIdx = 0; vIdx < pList.size(); ++vIdx ) 
      {
        Polyline3DPoint * pp = initList.Add();

        if ( !GetDouble(IDS_ENTER_RADIUS, radius) )  
        {
          res = false;
          break;
        }
        pp->m_oldIndex = vIdx;
        pp->m_point    = pList[vIdx];
        pp->m_radius   = fabs(radius);
        pp->m_lineSeg  = nullptr; ///< Прямолинейный сегмент из этой вершины в следующую. A line segment from the current vertex to the next.
        pp->m_arcSeg   = nullptr; ///< Дуга скругления в этой вершине (если m_radius > 0). The arc of the fillet at the vertex (if m_radius > 0).
      }
      if ( res ) 
      {
        MbContour3D contour;
        // Построить контур со скруглениями или без из отрезков по заданным точкам.
        // Construct contour with or without fillets from line segments by given points.
        if ( ::InitContour3D( contour, (iCls > 0), initList, errorIndexes ) ) 
        {
          size_t i, iCount = contour.GetSegmentsCount();
          RPArray<MbCurve3D> segments( iCount, 1 );
          for ( i = 0; i < iCount; i++ ) 
            segments.Add( contour.SetSegment(i) );
          MbWireFrame * wireFrame = MbWireFrame::Create( segments, nullptr, Math::DefaultMathVersion() );
          TestVariables::viewManager->AddObject( TestVariables::POINT_Style, wireFrame );
        }
      }
    }
  }
  pList.clear();
}


//------------------------------------------------------------------------------
// Создать проволочный каркас по грани тела.
// Create wire frame by face of solid.
// ---
void CreateWireFrameByFace()
{
  MbItem * obj = nullptr;
  MbTopItem * topItem = nullptr;
  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &topItem, TestVariables::itemPathFrom.second ) ) {
    MbFace * face = static_cast<MbFace *>( topItem );

    if ( face != nullptr ) {
      WireEdgesSPtrVector edges;
      face->GetEdges( edges );
      SPtr<MbWireFrame> wireFrame( MbWireFrame::Create(edges, nullptr, nullptr, false, Math::DefaultMathVersion()) ); // C3D-14026. Создание каркаса на копии исходных ребер.
      TestVariables::viewManager->AddObject( TestVariables::POINT_Style, wireFrame );
    }
  }
}


//------------------------------------------------------------------------------
// Создание проволочного каркаса NURNS кривых.
// Create a wire frame of NURNS curves.
// ---
void CreateNurbsCurveWireFrame()
{
  MbItem * wireFrameItem = nullptr;

  if ( GetGObj( IDS_SELECT_WIREFRAME, st_SpaceItem, &wireFrameItem) && (wireFrameItem != nullptr) ) {
    if ( wireFrameItem->IsA() == st_WireFrame ) {
      WireFrameSPtr wireFrame( static_cast<MbWireFrame *>( wireFrameItem ) );

      bool byProjections = true;
      if ( ::GetBool( IDS_SPECIFY_BY_PROJECTIONS, byProjections ) ) {

        MbWireFramePoint p1, p2;
        if ( byProjections ) {
          MbCartPoint3D point1, point2;
          if ( ::GetPoint( IDS_SELECT_POINT, point1 ) &&
               ::GetPoint( IDS_SELECT_POINT, point2 ) ) {
#ifdef _DRAWGI
            if ( DrawTestCurve3D::drawItems ) {
              DrawGI::DrawPoint( &point1, TRGB_RED, 3 );
              DrawGI::DrawPoint( &point2, TRGB_ORANGE, 3 );
            }
#endif
            MbWireFrameProjParams params1( point1, false ), params2( point2, false );
            MbWireFrameProjResult result1, result2;
            if ( ::NearPointProjection( *wireFrame, params1, result1 ) == rt_Success &&
                 ::NearPointProjection( *wireFrame, params2, result2 ) == rt_Success ) {
              p1 = result1.GetWireFramePoint();
              p2 = result2.GetWireFramePoint();

#ifdef _DRAWGI
              if ( DrawTestCurve3D::drawItems ) {
                DrawGI::DrawPoint( &p1.GetEdgePoint(), TRGB_BLACK, 2 );
                DrawGI::DrawPoint( &p2.GetEdgePoint(), TRGB_BLACK, 2 );
              }
#endif
            }
          }
        }
        else {
          int ind1 = 0, ind2 = 0;
          if ( ::GetInt( IDS_SELECT_WIREFRAME_EDGE_INDEX, ind1 ) && 
               ::GetInt( IDS_SELECT_WIREFRAME_EDGE_INDEX, ind2 ) ) {
            double t1 = 0.0, t2 = 0.0;
            if ( ::GetDouble( IDS_ENTER_PAR_ON_EDGE, t1 ) && 
                 ::GetDouble( IDS_ENTER_PAR_ON_EDGE, t2 ) ) {
              p1.InitPointOnEdge( *wireFrame, ind1, t1 ) &&
              p2.InitPointOnEdge( *wireFrame, ind2, t2 );
            }
          }
        }

        if ( p1.IsValid() && p2.IsValid() ) {
          bool sense = true;
          bool match = true;
          bool ext = false;
          MbSNameMaker snMaker( ct_WireFrameIntoNurbsCreator, MbSNameMaker::i_SideNone, 0 );
          MbNurbsWireFrameParams params( p1, p2, sense, match, ext, snMaker, Math::DefaultMathVersion() );

          MbeCopyMode sameWire = cm_Copy;
          WireFrameSPtr resWireFrame;
          MbResultType res = ::CreateNurbsWireFrame( wireFrame, sameWire, params, resWireFrame );

          if ( res == rt_Success && resWireFrame != nullptr ) {
            TestVariables::viewManager->AddObject( TestVariables::CONIC_Style, resWireFrame );
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание усеченного проволочного каркаса.
// Create a trimmed wire frame.
// ---
void CreateTrimmedEdgeWireFrame() {
  MbItem * wireFrameItem = nullptr;

  if ( GetGObj(IDS_SELECT_WIREFRAME, st_SpaceItem, &wireFrameItem) && (wireFrameItem != nullptr) ) {
    if ( wireFrameItem->IsA() == st_WireFrame ) {
      WireFrameSPtr wireFrame( static_cast<MbWireFrame*>(wireFrameItem) );

      int ind1 = 0, ind2 = 0;
      if ( ::GetInt(IDS_SELECT_WIREFRAME_EDGE_INDEX, ind1) &&
           ::GetInt(IDS_SELECT_WIREFRAME_EDGE_INDEX, ind2 )) {

        double t1 = 0.0, t2 = 0.0;
        if ( ::GetDouble(IDS_ENTER_PAR_ON_EDGE, t1) &&
             ::GetDouble(IDS_ENTER_PAR_ON_EDGE, t2) ) {

          MbWireFramePoint p1, p2;
          if ( p1.InitPointOnEdge(*wireFrame, ind1, t1) &&
               p2.InitPointOnEdge(*wireFrame, ind2, t2) ) {

            bool sense = true;
            bool ext = false;
            MbSNameMaker snMaker( ct_TrimmedWireFrameCreator, MbSNameMaker::i_SideNone, 0 );
            MbTrimmedWireFrameParams params( p1, p2, sense, ext, snMaker );

            MbeCopyMode sameWire = cm_Copy;
            WireFrameSPtr resWireFrame;
            MbResultType res = ::CreateTrimmedWireFrame( wireFrame, sameWire, params, resWireFrame );

            if ( res == rt_Success && resWireFrame != nullptr ) {
              TestVariables::viewManager->AddObject(TestVariables::CONIC_Style, resWireFrame);
            }
          }
        }
      }
    }
  }
}



namespace PrepareExtCurve {
  //------------------------------------------------------------------------------
  // Получить параметры удлинения конца кривой.
  // Get the extension parameters at end of curve.
  // ---
  static
  bool GetExtensionParamAtEnd( const ptrdiff_t              extWay,
                               const double                 extLength,
                                     MbCurveExtensionEnds & extParam )
  {
    bool res = true;

    switch ( extWay ) {
      case 0:
        extParam = MbCurveExtensionLinear( extLength );
        break;
      case 1:
        extParam = MbCurveExtensionCircular( extLength );
        break;
      case 2:
      case 3:
        extParam = MbCurveExtensionNatural( extLength );
        break;
      default:
        res = false;
        break;
    }

    return res;
  }

  //------------------------------------------------------------------------------
  // \ru Проверить корректность параметров. Вернуть код ошибки или rt_Success, если все хорошо.
  //     Внимание! Это лишь пример собственной диагностики (на уровне пользователя). Фактическая
  //     диагностика внутри алгоритма может отличаться.
  // \en Check the parameters validity. Return the error code or rt_Success if all is OK.
  //     Attention! This is only example of an own diagnostic (on the user level). Real diagnostic
  //     inside the algorithm can be another.
  // ---
  MbResultType CheckExtParametersValid( const MbCurveExtensionEnds & extParam )
  {
    if ( extParam.GetWayToExtend() >= MbeCurveExtensionWays::cew_Undefined )
      return rt_WrongExtensionWayValue;
    else {
      const double extensionLength = extParam.GetExtensionLength();
      if ( (extensionLength == DBL_MAX) ||
           (extensionLength < MbCurveExtensionEnds::MinLengthAllowed()) ||
           (extensionLength > MbCurveExtensionEnds::MaxLengthAllowed()) )
        return rt_WrongExtensionLength;
    }

    return rt_Success;
  }

  //------------------------------------------------------------------------------
  // Вычислить длину каркаса.
  // Compute a length of the wire frame.
  // ---
  static inline
  double ComputeWireFrameLength( const MbWireFrame & wf )
  {
    double sumLength( 0.0 ); // Общая длина результирующей кривой. // Total length of the result curve.
    for ( size_t i = wf.GetEdgesCount(); i-- > 0; ) {
      sumLength += wf.GetEdge( i )->GetMetricLength();
    }

    return sumLength;
  }
}


//------------------------------------------------------------------------------
// Создание продленного проволочного каркаса.
// Create an extended wire frame.
// ---
void CreateExtendedWireFrame()
{
  const size_t maxStringBufSize = 1000; // Максимальная длина строки в сообщениях.  // Maximal string length in messages.
  MbItem * wireFrameItem = nullptr;

  if ( GetGObj( IDS_SELECT_WIREFRAME, st_SpaceItem, &wireFrameItem ) && ( wireFrameItem != nullptr ) ) {
    if ( wireFrameItem->IsA() == st_WireFrame ) {
      WireFrameSPtr srcWireFrame( static_cast<MbWireFrame *>( wireFrameItem ) );

      TCHAR buff[maxStringBufSize];
      _sntprintf( buff, maxStringBufSize, _T("%s: 0...%d"), IDS_ENTER_NUMBER, static_cast<unsigned int>(MbeCurveExtensionWays::cew_Undefined) - 1 );
    
      // Способ продления. // Extension way.
      ptrdiff_t extendWayStart( -1 ), extendWayEnd( -1 );
      ::GetInt( buff, extendWayStart );
      ::GetInt( buff, extendWayEnd );

      // Длина продления. // Extension length.
      double extendLengthStart( 0.0 ), extendLengthEnd( 0.0 );
      ::GetDouble( IDS_POCKET_LENGTH, extendLengthStart );
      ::GetDouble( IDS_POCKET_LENGTH, extendLengthEnd );

      // Параметры продления концов. // Extension parameters of ends.
      MbCurveExtensionEnds paramStart, paramEnd;
      PrepareExtCurve::GetExtensionParamAtEnd( extendWayStart, extendLengthStart, paramStart );
      PrepareExtCurve::GetExtensionParamAtEnd( extendWayEnd, extendLengthEnd, paramEnd );

      const bool allowClosed( ::GetBoolean(IDS_ENTER_CLOSED_BOOL) );

      const double l0 = PrepareExtCurve::ComputeWireFrameLength( *srcWireFrame ); // Длина исходного каркаса. // Length of the source wire frame.
      const MbSNameMaker snMaker( SimpleName(-1), MbSNameMaker::i_SideNone, 0 );
      WireFrameSPtr resWire( nullptr );
      MbResultType res( rt_Error );

      bool alongSurface( ::GetBoolean(IDS_BOOL_USE_CNTR_ON_SURFACE) ); // Удлинять вдоль поверхности. // Extend along surface.

      MbCurveExtensionParameters3D opParameters( paramStart, paramEnd, allowClosed, alongSurface, snMaker );  // Параметры продления. // Extension parameters.
      res = ::ExtendWireFrame( srcWireFrame, cm_Copy, opParameters, resWire );

      if ( !resWire.is_null() ) {
        TestVariables::viewManager->AddItem( *resWire );

        const double sumLength = PrepareExtCurve::ComputeWireFrameLength( *resWire ); // Общая длина результирующего каркаса. // Total length of the result wire frame.
        
        TCHAR format[maxStringBufSize];
        _sntprintf( format, maxStringBufSize, _T("%s (%d). %s. %s"), IDS_DONE, static_cast<unsigned int>( res ), RT_CURVE_LENGTH_THE_ARC, _T("d = %22.15e") );
        _sntprintf( buff, maxStringBufSize, format, sumLength, sumLength - l0 );
        
        MessageBoxEx( buff, IDS_DONE );
      }
      else {
        if ( res == rt_ParameterError ) { // Ошибка входных данных. // Error in input data.
          // Делаем диагностику для каждого из концов.  // Check parameter of every end.
          _sntprintf( buff, maxStringBufSize, _T("%s\n1: %d"), IDS_ERROR, static_cast<unsigned int>(PrepareExtCurve::CheckExtParametersValid(paramStart)) );
          MessageBoxEx( buff, IDS_ERROR );
          _sntprintf( buff, maxStringBufSize, _T("%s\n2: %d"), IDS_ERROR, static_cast<unsigned int>(PrepareExtCurve::CheckExtParametersValid(paramEnd)) );
          MessageBoxEx( buff, IDS_ERROR );
        }
        else {  // Ошибка выполнения. // Execution error.
          _sntprintf( buff, maxStringBufSize, _T( "%s: %d" ), RT_ERROR_, static_cast<unsigned int>(res) );
          MessageBoxEx( buff, IDS_ERROR );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание эквидистантного проволочного каркаса.
// Create an offset wire frame.
// ---
void CreateOffsetWireFrame()
{
  WireFrameSPtr wireFrame( ::SelectWireFrame() );
  if ( wireFrame != nullptr ) {
    MbCartPoint3D dirPnt;
    if ( ::GetPoint(IDS_ENTER_DIR, dirPnt) ) {
      MbVector3D dir( dirPnt );
      WireFrameSPtr offsetWireFrame;
      MbeCopyMode sameWire( cm_Copy );
      MbSNameMaker snMaker( ct_OffsetWireFrameCreator, MbSNameMaker::i_SideNone, 0 );
      MbResultType res( rt_Error );
      FacesVector faces; // Грани исходного тела.
      ConstSolidSPtr initSolid( ::SelectShellFaces(faces, &TestVariables::itemPathFrom, true) );
      if ( initSolid != nullptr && initSolid->GetShell() != nullptr ) {
        IndicesVector facesIndices;
        initSolid->GetShell()->FindFacesIndexByFaces( faces, facesIndices );
        dirPnt.Init( wireFrame->GetBegVertex()->GetCartPoint() );
        bool extWire( ::GetBool(IDS_EXTEND_WIRE_FRAME, false) );
        bool extShell( ::GetBool(IDS_EXTEND_SHELL, false) );
        MbOffsetWireFrameParams params( initSolid, facesIndices, MbAxis3D(dirPnt, dir), dir.Length(), MbOffsetWireFrameParams::OffsetMethod::chordal, extWire, extShell, snMaker );
        res = ::CreateOffsetWireFrame( wireFrame, sameWire, params, offsetWireFrame );
      }
      else {
        MbOffsetWireFrameParams params( dir, snMaker );
        res = ::CreateOffsetWireFrame( wireFrame, sameWire, params, offsetWireFrame );
      }
      if ( (res == rt_Success || res == rt_SelfIntersect) && offsetWireFrame != nullptr ) {
        TestVariables::viewManager->AddObject( TestVariables::CONIC_Style, offsetWireFrame );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание B-spline.
// Construct B-spline.
// ---
void CreateBaseSpline()
{
  MbItem * obj = nullptr;
  MbCartPoint3D p[3];

  if ( GetPoint( IDS_SELECT_POINT, p[0] ) ) {
    TestVariables::drawImp->DrawPoint( &p[0], TRGB_LIGHTGREEN );
    if ( GetPoint( IDS_SELECT_POINT, p[1] ) ) {
      TestVariables::drawImp->DrawPoint( &p[1], TRGB_LIGHTGREEN );
      if ( GetPoint( IDS_SELECT_POINT, p[2] ) ) {
        TestVariables::drawImp->DrawPoint( &p[2], TRGB_LIGHTGREEN );

        MbVector3D vx, vy;
        vx.Init( p[0], p[1] );
        vy.Init( p[0], p[2] );

        bool canInitPlacement = false;
        if ( !vx.IsDegenerate() && !vy.IsDegenerate() && !vx.Colinear( vy ) )
          canInitPlacement = true;

        MbCartPoint3D pp;
        if ( canInitPlacement && GetPoint( IDS_SELECT_NURBS_CURVE, pp ) ) {
          MbBSpline * bspline = nullptr;

          if ( FindNearestObj( pp, st_Nurbs3D, &obj ) && (obj->IsA() == st_SpaceInstance) ) {
            MbSpaceInstance * sInst = static_cast<MbSpaceInstance *>(obj);
            if ( (sInst->GetSpaceItem() != nullptr) && (sInst->GetSpaceItem()->IsA() == st_Nurbs3D) ) {
              const MbNurbs3D * geom = static_cast<const MbNurbs3D *>(sInst->GetSpaceItem());
              bspline = new MbBSpline( p[0], p[1], p[2], *geom );
            }
          }
          if ( bspline == nullptr ) {
            if ( FindNearestObj( pp, pt_Nurbs, &obj ) && (obj->IsA() == st_PlaneInstance) ) {
              MbPlaneInstance * pInst = static_cast<MbPlaneInstance *>(obj);
              if ( (pInst->GetPlaneItem() != nullptr) && (pInst->GetPlaneItem()->IsA() == pt_Nurbs) ) {
                const MbNurbs * geom = static_cast<const MbNurbs *>(pInst->GetPlaneItem());
                bspline = new MbBSpline( p[0], p[1], p[2], *geom );
              }
            }
            if ( bspline == nullptr ) {
              MessageBoxEx( RT_CURVE_NOT_SELECTED, IDS_TITLE_WRN );

              SArray<double> knots( 10, 1 );
              MbCurve3D * segment = nullptr;
              ::Segment( p[0], p[1], segment );

              double t = 0.0;
              knots.Add( t );
              do {
                segment->NearPointProjection( pp, t, false, nullptr );
                if ( (t >= -EXTENT_EPSILON) && (t <= 1.0+EXTENT_EPSILON) )
                  knots.Add( t );
              } while ( GetPoint( IDS_SELECT_POINT, pp ) );
              t = 1.0;
              knots.Add( t );
              if ( knots.Count() > 2 )
                bspline = new MbBSpline( p[0], p[1], p[2], knots );
              ::DeleteItem( segment );
            }
          }
          if ( bspline != nullptr ) {
            TestVariables::viewManager->AddObject( TestVariables::TEXT_Style, bspline );
          }
        }
      }
    }
  }

}


//------------------------------------------------------------------------------
// Преобразовать кривую в NURBS.
// Convert curve to NURBS.
// ---
bool TranslateCurveToNurbs()
{
  MbItem * curveItem = nullptr;
  SPtr<MbCurve3D> spaceCurve;
  PathMatrix pathFrom;

  if ( (spaceCurve = ::SelectAnyCurveCopy( pathFrom, nullptr, &curveItem ) ) ) {
    TestVariables::viewManager->ToggleObject( curveItem );
    {
      ptrdiff_t degree = NURBS_DEGREE;
      MbCurve3D * spaceNurbs = nullptr;
      if ( GetInt ( IDS_ENTER_SPLINE_ORDER, degree ) ) {
        ptrdiff_t pCount = 0;
        GetInt ( IDS_ENTER_POINT_NUMBER, pCount );
        pCount = std_max( pCount, (ptrdiff_t)0 );
        double k1 = 0.0, k2 = 1.0, accuracy = 0.001;
        GetDouble ( IDS_ENTER_PAR_ON_CURVE, k1 );
        GetDouble ( IDS_ENTER_PAR_ON_CURVE, k2 );
        GetDouble ( IDS_ENTER_APPROX_ACCURACY, accuracy );
        int useApprox = 1; // Использовать аппроксимацию при построении кривой. Use approximation when constructing the curve.
        GetInt ( IDS_ENTER_USE_APPROX, useApprox );
        MbNurbsParameters param ( degree, pCount, k1, k2, useApprox == 1 );
        param.SetPrecision( accuracy );
        MbNurbsCopyCurveResult nurbsCopyResult;
        nurbsCopyResult.InitMaxDeviation(accuracy);
        ::NurbsCopy( *spaceCurve, param, nurbsCopyResult );
        spaceNurbs = nurbsCopyResult.SetNurbsCopy().detach();
      }
      else { // Степень не введена - старый вариант вызываем. Because the degree is not specified, call the old version.
        ::NurbsCopy( *spaceCurve, spaceNurbs ); 
      }
      if ( spaceNurbs != nullptr ) {
        spaceNurbs->Transform( pathFrom.second );
        TestVariables::viewManager->AddObject( TestVariables::BOOLEAN_Style, spaceNurbs );
      }
    }
    TestVariables::viewManager->ToggleObject( curveItem );

    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// Создание символьной кривой.
// Construct a character curve.
// ---
void CreateCharCurve3D()
{
  MbCartPoint3D point;
  while ( GetPoint( IDS_SELECT_PLACEMENT, point ) ) {
    string_t fx; 
    string_t fy;
    string_t fz;
    string_t arg;                                            
    if ( GetDataCharCurve3D( fx, fy, fz, arg ) )
    {
      int functionCase( 0 );
      double t1 = 0, t2 = 10*M_PI;
      if ( GetDouble( IDS_ENTER_INITIAL_PRM, t1 ) && 
           GetDouble( IDS_ENTER_FINAL_PRM, t2 ) &&
           GetInt( IDS_SPECIFY_FUNCTION_CASE, functionCase ) ) 
      {
        bool useFirstVar = static_cast<bool>( functionCase );
        WorkWindow * vc = TestVariables::viewManager->GetActiveWindow();

        MbFunctionFactory factory;
        // Конвертация. Conversion.
        MbFunction * x = factory.CreateAnalyticalFunction( fx, arg, t1, t2, useFirstVar );
        MbFunction * y = factory.CreateAnalyticalFunction( fy, arg, t1, t2, useFirstVar );
        MbFunction * z = factory.CreateAnalyticalFunction( fz, arg, t1, t2, useFirstVar );
        if ( x != nullptr &&
             y != nullptr &&
             z != nullptr )
        {
          MbPlacement3D place( point, vc->GetAxisZ(), vc->GetAxisX() );
          MbeLocalSystemType3D cs = ls_CartesianSystem;
          MbCharacterCurve3D * curve = new MbCharacterCurve3D( *x, *y, *z, cs, place, t1, t2 );
          if ( curve != nullptr )
            TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, curve );
        }
        else {
          PRECONDITION(false);
          delete x;
          delete y;
          delete z;
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание эквидистантной кривой.
// Construct an equidistant curve.
// ---
void CreateOffsetCurve3D()
{
  MbItem * curveObj = nullptr;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &curveObj ) ) {
    SPtr<MbCurve3D> curve;

    ::GetSpaceCurve( *curveObj, true, curve );

    if ( curve != nullptr ) {
      TestVariables::viewManager->ToggleObject( curveObj );
      MbCartPoint3D dirPnt; // Точка. Point.

      if ( GetPoint( IDS_ENTER_DIR, dirPnt ) ) {
        MbVector3D dir( dirPnt );

        MbCartPoint3D cp;
        curve->GetLimitPoint( 0, cp );

        SPtr<MbFace> face;

        MbItem * surfObj = nullptr;
        MbTopItem * tItem = nullptr;
        if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &surfObj, &tItem ) ) {
          if ( tItem->IsAFace() )
            face = ((MbFace *)tItem)->DataDuplicate();
        }
        if ( face == nullptr ) {
          MbSurface * surf = nullptr;
          if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &surfObj ) ) {
            surf = (MbSurface *)&(((MbSpaceInstance *)surfObj)->GetSpaceItem()->Duplicate());
            face = new MbFace( *surf, true );
            ::DeleteItem( surf );
          }
        }

        MbResultType res = rt_Error;
        WireFrameSPtr resFrame;

        if ( face == nullptr ) {
          // Для отладки. For debugging.
          double dirLen = dir.Length();
          dir.Normalize();

          GetDouble( IDS_ENTER_DISTANCE, dirLen );
          dirLen = ::fabs( dirLen );
          dir *= dirLen;

          int choice = 0;
          GetInt( IDS_ENTER_BYPASS_TYPE, choice );

//#ifdef _DRAWGI
          TestVariables::drawImp->DrawPoint( &cp, TRGB_BLACK );
          TestVariables::drawImp->DrawLine( cp, dir, TRGB_BLACK );
//#endif // _DRAWGI

          MbSNameMaker snMaker( ct_Curve3DCreator, MbSNameMaker::i_SideNone, 0 );

          MbSpatialOffsetCurveParams params( dir, snMaker );
          
          params.useFillet = (choice == 1 ? false : true);
          params.keepRadius = false;
          params.bluntAngle = true;
          params.fromBeg = true;

          params.SetBySurfaceNormal( false );

          res = ::OffsetCurve( *curve, params, resFrame );
        }
        else {
          double u, v;
          u = v = UNDEFINED_DBL;
          face->GetSurface().NearPointProjection( cp, u, v, false );
          MbVector3D norm;
          face->GetSurface().Normal( u, v, norm );
          MbVector3D tau;
          curve->GetLimitTangent( 1, tau );
          tau = tau | norm;
          tau.Normalize();

          double dist = dir.Length();
          if ( dist > LENGTH_EPSILON )
            dir /= dist;

          bool invert = (dir * tau) < -ANGLE_EPSILON;

          dir = tau;
          if ( invert )
            dir.Invert();

          GetDouble( IDS_ENTER_DISTANCE, dist );
          
          MbAxis3D dirAxis( cp, dir );
          MbSNameMaker snMaker( ct_Curve3DCreator, MbSNameMaker::i_SideNone, 0 );

          MbSurfaceOffsetCurveParams params( *face, dirAxis, dist, snMaker );

//#ifdef _DRAWGI
          MbCartPoint3D cp1;
          face->GetSurface()._PointOn( u, v, cp1 );
          TestVariables::drawImp->DrawPoint( &cp1, TRGB_BLACK );
          cp1.Set( cp, dir, dist );
          TestVariables::drawImp->DrawPoint( &cp, TRGB_PURPLE );
          TestVariables::drawImp->DrawPoint( &cp1, TRGB_AZURE );
          TestVariables::drawImp->DrawLine( cp, cp1, TRGB_BROWN );
//#endif // _DRAWGI
          res = ::OffsetCurve( *curve, params, resFrame );
        }

        if ( res == rt_Success && resFrame != nullptr )
          TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, resFrame );
        else
          MessageBoxEx( RT_CURVE_IS_NOT_ADDED, IDS_TITLE_ERR );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание кривой, соединяющей две осевые кривые трубопроводов.
// Construct a curve that connects two axial curves of the pipelines.
// ---
void CreateCurvesCouple3D()
{
  MbItem * nearest1 = nullptr;
  MbItem * nearest2 = nullptr;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest1 ) && (nearest1->IsA() == st_SpaceInstance) ) {
    MbCurve3D * curve1 = (MbCurve3D *)(((MbSpaceInstance *)nearest1)->GetSpaceItem());
    if ( curve1 != nullptr ) {
      TestVariables::viewManager->ToggleObject(nearest1);
      MbCartPoint3D cPoint1( TestVariables::cursorPoint );

      if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest2 ) && (nearest2->IsA() == st_SpaceInstance) ) {
        MbCurve3D * curve2 = (MbCurve3D *)(((MbSpaceInstance *)nearest2)->GetSpaceItem());
        if ( curve2 != nullptr ) {
          TestVariables::viewManager->ToggleObject(nearest2);
          MbCartPoint3D cPoint2( TestVariables::cursorPoint );
          WorkWindow * workWindow = TestVariables::viewManager->GetActiveWindow();
          MbVector3D direct( workWindow->GetAxisZ() );

          double r1 = 10.0;
          if ( GetDouble( IDS_ENTER_RADIUS, r1 ) ) {
            double r2 = r1;
            if ( GetDouble( IDS_ENTER_RADIUS, r2 ) ) {

              double t1 = ::GetNearCurveParam( *curve1, cPoint1, direct );
              double t2 = ::GetNearCurveParam( *curve2, cPoint2, direct );
              double t1min = curve1->GetTMin();
              double t1max = curve1->GetTMax();
              double t2min = curve2->GetTMin();
              double t2max = curve2->GetTMax();
              bool isBegin1 = ( ::fabs(t1-t1min) < ::fabs(t1-t1max) );
              bool isBegin2 = ( ::fabs(t2-t2min) < ::fabs(t2-t2max) );
              t1 = isBegin1 ? t1min : t1max;
              t2 = isBegin2 ? t2min : t2max;

              MbCartPoint3D origin1, origin2, result1, result2;
              MbVector3D direction1, direction2;
              double length1 = 0.0, length2 = 0.0;

              GetDouble( IDS_ENTER_STRETCHING, length1 );
              GetDouble( IDS_ENTER_STRETCHING, length2 );

              if ( length1 < 0 ) length1 = 0.0;
              if ( length2 < 0 ) length2 = 0.0;

              curve1->PointOn( t1, origin1 );
              curve2->PointOn( t2, origin2 );
              curve1->Tangent( t1, direction1 );
              curve2->Tangent( t2, direction2 );
              if ( isBegin1 ) direction1.Invert();
              if ( isBegin2 ) direction2.Invert();

              MbResultType res = ::CalculatePipePoints( origin1, direction1, length1, r1,
                                                        origin2, direction2, length2, r2,
                                                        result1, result2 ); 

              if ( res == rt_Success ) {
                SArray<MbCartPoint3D> initList(4,1);
                initList.Add( origin1 );
                initList.Add( result1 );
                initList.Add( result2 );
                initList.Add( origin2 );
                MbCurve3D * polyline = new MbPolyline3D( initList, false );
                TestVariables::viewManager->AddObject( TestVariables::POINT_Style, polyline );
              }

              MbWireFrame * result = nullptr;
              MbSNameMaker names( ct_Curve3DCreator, MbSNameMaker::i_SideNone, 0 );

              if ( length1 > NULL_EPSILON && length2 > NULL_EPSILON ) {
                MbCartPoint3D point1( origin1 ), point2( origin2 );
                point1.Add( direction1, length1 );
                point2.Add( direction2, length2 );
                MbCurve3D * segment1 = nullptr;
                MbCurve3D * segment2 = nullptr;
                MbResultType const resType1 = ::Segment( origin1, point1, segment1 );
                MbResultType const resType2 = ::Segment( origin2, point2, segment2 );
                if ( resType1 == rt_Success
                  && resType2 == rt_Success
                  && segment1 != nullptr
                  && segment2 != nullptr
                ) {
                  ::GetNearCurveParam( *segment1, point1, direct, true, isBegin1 );
                  ::GetNearCurveParam( *segment2, point2, direct, true, isBegin2 );
                  res = ::ConnectingCurve( *segment1, isBegin1, r1, *segment2, isBegin2, r2, names, result ); 
                }
                ::DeleteItem( segment1 );
                ::DeleteItem( segment2 );
              }
              else {
                ::GetNearCurveParam( *curve1, cPoint1, direct, true, isBegin1 );
                ::GetNearCurveParam( *curve2, cPoint2, direct, true, isBegin2 );
                res = ::ConnectingCurve( *curve1, isBegin1, r1, *curve2, isBegin2, r2, names, result ); 
              }

              if ( result != nullptr  ) {
                TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, result );
              }
              else
                ::PutErrorMessage( res, IDS_TITLE_ERR );
            }
          }
        }
        TestVariables::viewManager->ToggleObject(nearest2);
      }
    }
    TestVariables::viewManager->ToggleObject(nearest1);
  }
}


//------------------------------------------------------------------------------
// Создание кривой, скругляющей две кривые.
// Construct a curve that fillets the two curves.
// ---
void CreateCurvesFillet3D()
{
  MbItem * nearest1 = nullptr;
  MbItem * nearest2 = nullptr;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest1 ) && (nearest1->IsA() == st_SpaceInstance) ) {
    MbCurve3D * curve1 = (MbCurve3D *)(((MbSpaceInstance *)nearest1)->GetSpaceItem());
    if ( curve1 != nullptr ) {
      TestVariables::viewManager->ToggleObject(nearest1);
      MbCartPoint3D cPoint1( TestVariables::cursorPoint );

      if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest2 ) && (nearest2->IsA() == st_SpaceInstance) ) {
        MbCurve3D * curve2 = (MbCurve3D *)(((MbSpaceInstance *)nearest2)->GetSpaceItem());
        if ( curve2 != nullptr ) {
          TestVariables::viewManager->ToggleObject(nearest2);
          MbCartPoint3D cPoint2( TestVariables::cursorPoint );
          WorkWindow * workWindow = TestVariables::viewManager->GetActiveWindow();
          MbVector3D direct( workWindow->GetAxisZ() );

          double r = 10.0;
          if ( GetDouble( IDS_ENTER_RADIUS, r ) ) {
            bool add = ( r < NULL_EPSILON );
            MbWireFrame * result = nullptr;
            MbSNameMaker names( ct_Curve3DCreator, MbSNameMaker::i_SideNone, 0 );
            MbElementarySurface * surface = nullptr;
            double t1 = ::GetNearCurveParam( *curve1, cPoint1, direct );
            double t2 = ::GetNearCurveParam( *curve2, cPoint2, direct );
            double w1, w2;
            int sense = 1;
            int type = 0;
            bool unchanged = true;
    
            GetInt( IDS_ENTER_SENSE, sense );
            GetInt( IDS_ENTER_CONN_TYPE, type );

            MbResultType res = ::FilletCurve( *curve1, t1, w1, *curve2, t2, w2, r, (sense > 0), unchanged, (MbeConnectingType)type, names, surface, result ); 

            if ( result != nullptr  ) {
              Style ns = TestVariables::SURFACECURVE_Style;
              TestVariables::viewManager->AddObject( ns, result );
              if ( surface != nullptr  ) {
                MbSNameMaker names_( ct_Curve3DCreator, MbSNameMaker::i_SideNone, 0 );
                MbElementarySolidResults results;
                if ( surface->Type() == st_ElementarySurface ) {
                  SPtr<const MbElementarySurface> auxSurface( &( static_cast<const MbElementarySurface &>( *surface ) ) );
                  res = ::ElementarySolid( MbElementarySolidParams(auxSurface, names_), results ); // ::ElementarySolid( *surface, names_, solid );
                  auxSurface.detach();
                }

                if ( results.GetResultSolid() != nullptr )
                  TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, results.DetachSolid() );
              }
              if ( add ) {
                MbWireFrame * result0 = nullptr;
                MbElementarySurface * surface0 = nullptr;
                double t01 = t1;
                double t02 = t2;
                double w01, w02;
                w01 = w02 = UNDEFINED_DBL;
                bool unchanged_ = true;
                double r0 = r;
    
                ::FilletCurve( *curve1, t01, w01, *curve2, t02, w02, r0, (sense > 0), unchanged_, (MbeConnectingType)type, names, surface0, result0 ); 

                ::DeleteItem( result0 );
                ::DeleteItem( surface0 ); 
              }
              if ( !curve1->IsClosed() ) {
                MbCurve3D * trimm1 = curve1->Trimmed( std_min(t1, w1), std_max(t1, w1), 1 );
                TestVariables::viewManager->AddObject( ns, trimm1 );
              }
              if ( !curve2->IsClosed() ) {
                MbCurve3D * trimm2 = curve2->Trimmed( std_min(t2, w2), std_max(t2, w2), 1 );
                TestVariables::viewManager->AddObject( ns, trimm2 );
              }
            }
            else
              ::PutErrorMessage( res, IDS_TITLE_ERR );
          }
        }
        TestVariables::viewManager->ToggleObject(nearest2);
      }
    }
    TestVariables::viewManager->ToggleObject(nearest1);
  }
}


//------------------------------------------------------------------------------
// Создание кривой-мостика, соединяющего две кривые.
// Construct a curve which will be a bridge connecting the two curves.
// ---
void CreateCurvesBridge3D()
{
  MbItem * nearest1 = nullptr;
  MbItem * nearest2 = nullptr;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest1 ) && (nearest1->IsA() == st_SpaceInstance) ) {
    MbCurve3D * curve1 = (MbCurve3D *)(((MbSpaceInstance *)nearest1)->GetSpaceItem());
    if ( curve1 != nullptr ) {
      TestVariables::viewManager->ToggleObject(nearest1);
      MbCartPoint3D cPoint1( TestVariables::cursorPoint );

      if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest2 ) && (nearest2->IsA() == st_SpaceInstance) ) {
        MbCurve3D * curve2 = (MbCurve3D *)(((MbSpaceInstance *)nearest2)->GetSpaceItem());
        if ( curve2 != nullptr ) {
          TestVariables::viewManager->ToggleObject(nearest2);
          MbCartPoint3D cPoint2( TestVariables::cursorPoint );
          WorkWindow * workWindow = TestVariables::viewManager->GetActiveWindow();
          MbVector3D direct( workWindow->GetAxisZ() ), vect( cPoint1, cPoint2 ), tau1, tau2;

          bool sense1( true ), sense2( true );
          // Дать ближайший к лучу параметр кривой.
          // Get the closest to the ray parameter of the curve.
          double t1 = ::GetNearCurveParam( *curve1, cPoint1, direct, false, sense1 );
          double t2 = ::GetNearCurveParam( *curve2, cPoint2, direct, false, sense2 );
          curve1->_Tangent( t1, tau1 );
          curve2->_Tangent( t2, tau2 );
          vect.Normalize();
          sense1 = ( (vect * tau1) > NULL_EPSILON );
          sense2 = ( (vect * tau2) > NULL_EPSILON );
          MbWireFrame * result = nullptr;
          MbSNameMaker names( ct_Curve3DCreator, MbSNameMaker::i_SideNone, 0 );

          MbResultType res = ::BridgeCurve( *curve1, t1, sense1, *curve2, t2, sense2, names, result ); 

          if ( result != nullptr  ) {
            TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, result );
          }
          else
            ::PutErrorMessage( res, IDS_TITLE_ERR );
        }
        TestVariables::viewManager->ToggleObject(nearest2);
      }
    }
    TestVariables::viewManager->ToggleObject(nearest1);
  }
}


//------------------------------------------------------------------------------
// Создание кривой соединяющей две кривые по условиям сопряжения.
// Construct a curve connecting two curves on the conjugation conditions.
//---
void CreateConnectingCurve3D()
{
  MbItem * nearest1 = nullptr;
  MbItem * nearest2 = nullptr;
  
  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest1 ) && (nearest1->IsA() == st_SpaceInstance) ) {
    MbCurve3D * curve1 = (MbCurve3D *)(((MbSpaceInstance *)nearest1)->GetSpaceItem());
    if ( curve1 != nullptr ) {
      TestVariables::viewManager->ToggleObject(nearest1);
      WorkWindow * workWindow = TestVariables::viewManager->GetActiveWindow(); 
      MbVector3D direct( workWindow->GetAxisZ() ); 
      MbCartPoint3D cPoint1( TestVariables::cursorPoint ); // Точка на первой кривой. Point on curve.
      int itype1 = TestVariables::operateType;
      GetInt( IDS_ENTER_CONN_TYPE, itype1 ); 
      if ( itype1 >= 0 && itype1 <=4 )
      {
        MbeMatingType type1 = (MbeMatingType &)( itype1 ); // Типы сопряжений для кривых. Conjugation types for curves.
        if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest2 ) && (nearest2->IsA() == st_SpaceInstance) ) {
          MbCurve3D * curve2 = (MbCurve3D *)(((MbSpaceInstance *)nearest2)->GetSpaceItem());
          if ( curve2 != nullptr ) {
            TestVariables::viewManager->ToggleObject(nearest2);
            MbCartPoint3D cPoint2( TestVariables::cursorPoint ); // Точка на второй кривой. Point on the second curve.
            
            int itype2 = itype1;
            GetInt( IDS_ENTER_CONN_TYPE, itype2 );
            if ( itype2 >= 0 && itype2 <=4 ) 
            {
              MbeMatingType type2 = (MbeMatingType &)( itype2 ); // Типы сопряжений для кривых. Conjugation types for curves.
              // Натяжение соединительной кривой на стыке с первой кривой.
              // The tension of the connecting curve at the junction with the first curve.
              double tension1 = 0.25; 
              GetDouble( IDS_ENTER_TENSION, tension1 );
              // Натяжение соединительной кривой на стыке со второй кривой.
              // The tension of the connecting curve at the junction with the second curve.
              double tension2 = tension1;
              GetDouble( IDS_ENTER_TENSION, tension2 );
              bool isBegin1, isBegin2; 
              double t1 = ::GetNearCurveParam( *curve1, cPoint1, direct, true, isBegin1 );
              double t2 = ::GetNearCurveParam( *curve2, cPoint2, direct, true, isBegin2 );
              MbWireFrame * result = nullptr;
              MbSNameMaker names( ct_Curve3DCreator, MbSNameMaker::i_SideNone, 0 );

              MbResultType res = ::ConnectingSpline( *curve1, t1, type1, *curve2, t2, type2, 
                                                     tension1, tension2, names, result ); 
          
              if ( result != nullptr && res == rt_Success ) {
                TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, result );
              }
              else
                ::PutErrorMessage( res, IDS_TITLE_ERR );
           }
        }
      }
      TestVariables::viewManager->ToggleObject(nearest2);
    }
    }
    TestVariables::viewManager->ToggleObject(nearest1);
  }
}


//------------------------------------------------------------------------------
// Создание кривой, трансформированной по закону кинематической операции.
// Construct a curve transformed by a law of kinematic operation.
// ---
void CreateEvolutionSection()
{
  SpaceCurvesSPtrVector generCurves;
  SpaceCurveSPtr guideCurve;

  { // Набрать образующие кривые и направляющую.
    // Collect generating curves and a guide curve.
    MbItem * item = nullptr;
    std::vector< MbItem * > items;

    while ( ::GetAnyCurve( IDS_SELECT_GENER_CURVE, &item ) && (item != nullptr) )
    {
      ItemsVector::iterator it = std::find( items.begin(), items.end(), item );

      if ( it == items.end() ) {
        SpaceCurveSPtr generCurve;
        if ( ::GetSpaceCurve( *item, true, generCurve ) && (generCurve != nullptr) ) {
          generCurve = ::DuplicateIfUsed( generCurve, (MbRegDuplicate *)nullptr );

          TestVariables::viewManager->ToggleObject( item );
          generCurves.push_back( generCurve );
          items.push_back( item );
        }
      }
      else {
        TestVariables::viewManager->ToggleObject( item );
        generCurves.erase( generCurves.begin() + std::distance( items.begin(), it ) );
        items.erase( it );
      }
    }

    if ( generCurves.size() > 0 ) {
      if ( ::GetAnyCurve( IDS_SELECT_GUIDE_CURVE, &item ) && (item != nullptr) ) {
        ItemsVector::iterator it = std::find( items.begin(), items.end(), item );

        if ( it == items.end() ) {
          ::GetSpaceCurve( *item, true, guideCurve );
          if ( guideCurve != nullptr ) {
             guideCurve = ::DuplicateIfUsed( guideCurve, (MbRegDuplicate *)nullptr );
             TestVariables::viewManager->ToggleObject( item );
          }
        }
      }
    }
  }

  if ( generCurves.size() > 0 && (guideCurve != nullptr) ) {
    SurfaceSPtr generSurface;
    PlaneContoursSPtrVector pContours;

    size_t k, generCnt = generCurves.size();
    for ( k = 0; k < generCnt; k++ ) {
      SpaceCurveSPtr & generCurve = generCurves[k];
      if ( generCurve != nullptr ) {
        PlaneCurveSPtr pCurve;
        SurfaceSPtr surface;
        if ( generCurve->GetSurfaceCurve( pCurve, surface ) ) {
          pCurve = ::DuplicateIfUsed( pCurve, (MbRegDuplicate *)nullptr );
          PlaneContourSPtr pContour;

          while ( pCurve->IsA() == pt_ReparamCurve )
            pCurve = &pCurve->SetBasisCurve();

          if ( pCurve->Type() == pt_Contour )
            pContour = &static_cast<MbContour &>( *pCurve );
          else
            pContour = new MbContour( *pCurve, true );

          if ( generSurface == nullptr ) {
            generSurface = surface;
            pContours.push_back( pContour );
          }
          else if ( surface->IsSimilarToSurface( *generSurface, Math::DefaultMathVersion() ) ) {
            MbMatrix matr;
            if ( surface->GetMatrixToSurface( *generSurface, matr, Math::DefaultMathVersion() ) ) {
              pContour->Transform( matr );
              pContours.push_back( pContour );
            }
          }
        }
      }
    }

    if ( pContours.size() > 0 ) {
      MbSweptData sweptData0, sweptData;

      double guideParam = guideCurve->GetTMin();
      MbRect1D guideRange( guideCurve->GetTMin(), guideCurve->GetTMax() );
      ::GetDouble( IDS_ENTER_PARAMETER, guideParam, &guideRange );

      sweptData0.AddData( *generSurface, pContours );

      EvolutionValues params;
      params.shellClosed = true;

      if ( ::GetDouble( IDS_ENTER_THICKNESS, TestVariables::thickness1 ) && 
           ::GetDouble( IDS_ENTER_THICKNESS, TestVariables::thickness2 ) )
      {
        params.thickness1 = TestVariables::thickness1;
        params.thickness2 = TestVariables::thickness2;
        int pr = 1;
        if ( ::GetInt( IDS_ENTER_METHOD, pr ) ) {
          if ( pr < 1 ) pr = 0;
          if ( pr > 1 ) pr = 2;

          if ( pr == 0 )
            params.SetParallel();
          else if ( pr == 1 )
            params.SetKeepingAngle();
          else
            params.SetOrthogonal();

          pr = 0;
        }
      }

      // **************************************************

      ::SetWaitCursor( true );
      MbResultType res = ::EvolutionSection( sweptData0, *guideCurve, guideParam, params, sweptData );
      ::SetWaitCursor( false );

      // **************************************************

      if ( sweptData.GetSurface() == nullptr ) {
        PRECONDITION( false ); // Error
        res = rt_Error;
      }

      if ( res == rt_Success ) {
        Style style = TestVariables::WIRE_Style;

        generSurface = sweptData.SetSurface();
        pContours.clear();
        pContours = sweptData.GetContours();

        static bool createEvolutionSolid = false;

        if ( createEvolutionSolid ) {
          MbSNameMaker operNames( ct_CurveEvolutionSolid, MbSNameMaker::i_SideNone, 0 );
          operNames.SetVersion( MbNameVersion() );

          MbSNameMaker cNames( ct_CurveSweptSolid, MbSNameMaker::i_SideNone, 0 );
          MbSNameMaker sNames( ct_CurveSweptSolid, MbSNameMaker::i_SideNone, 0 );

          PArray<MbSNameMaker> contoursNames( 1, 0, false );
          contoursNames.Add( &cNames );

          SolidSPtr resSolid;

          ::SetWaitCursor( true );
          const MbEvolutionShellParams evShellParams{ sweptData0, *guideCurve, params, operNames, contoursNames, sNames };
          MbResultType solidRes = ::EvolutionShell( evShellParams, resSolid );
          ::SetWaitCursor( false );

          if ( resSolid != nullptr ) {
            Style solidStyle = TestVariables::HIDE_Style;
            TestVariables::viewManager->AddObject( solidStyle, resSolid.detach() );
          }
          else {
            ::PutErrorMessage( solidRes, IDS_TITLE_ERR );
          }
        }

        ::MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );

        size_t resCnt = pContours.size();
        for ( k = 0; k < resCnt; k++ ) {
          if ( pContours[k] != nullptr ) {
            SPtr<MbSurface> surface( static_cast<MbSurface *>( &generSurface->Duplicate() ) );
            MbCurve3D * resCurve = nullptr;
            ::SurfaceCurve( *surface, *pContours[k], resCurve );
            TestVariables::viewManager->AddObject( style, resCurve );
          }
        }
        if ( resCnt != generCnt ) {
          PRECONDITION( false ); // Used not all generator curves!!!
        }
      }
      else {
        ::PutErrorMessage( res, IDS_TITLE_ERR );
      }
    }
    else {
      ::MessageBoxEx( RT_ERROR_IN_PARAMS, IDS_TITLE_ERR );
    }
  }
}


//------------------------------------------------------------------------------
// Создание конического сечения по 2-ум точкам, вершине и дискриминанту.
// Construct a conic section by two points, an angle vertex and a discriminant.
// ---
void CreateNurbsConic_1()
{
  MbCartPoint3D point[3];

  while ( GetPoint( IDS_SELECT_POINT_ON_CRV, point[0] ) ) {

    GetExactPoint(point[0]);
//#ifdef _DRAWGI
    TestVariables::drawImp->DrawPoint( &point[0], TRGB_LIGHTGREEN );
//#endif // _DRAWGI

    if ( GetPoint( IDS_SELECT_POINT_ON_CRV, point[1] ) ) {
      GetExactPoint(point[1]);
//#ifdef _DRAWGI
      TestVariables::drawImp->DrawPoint( &point[1], TRGB_LIGHTGREEN );
//#endif // _DRAWGI

      if ( GetPoint( IDS_SELECT_POINT, point[2] ) ) {
        GetExactPoint(point[2]);
//#ifdef _DRAWGI
        TestVariables::drawImp->DrawPoint( &point[2], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
        double discr = 0.0;
        GetDouble( IDS_ENTER_DISCR, discr );
        MbCurve3D * nurbs = nullptr;
        nurbs = NurbsConic_1( point[0], point[1], point[2], discr );
        if( nurbs!=nullptr )
        {
          TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, nurbs );
          TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[0]) );
          TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[2]) );
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
// Создание конического сечения по 3-ем точкам и вершине.
// Construct a conic section by three points, and an angle vertex.
// ---
void CreateNurbsConic_2()
{
  MbCartPoint3D point[4];

  while ( GetPoint( IDS_SELECT_POINT_ON_CRV, point[0] ) ) 
  {
//#ifdef _DRAWGI
    TestVariables::drawImp->DrawPoint( &point[0], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
    if ( GetPoint( IDS_SELECT_POINT_ON_CRV, point[1] ) ) 
    {
//#ifdef _DRAWGI
      TestVariables::drawImp->DrawPoint( &point[1], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
      if ( GetPoint( IDS_SELECT_POINT, point[2] ) ) 
      {
//#ifdef _DRAWGI
        TestVariables::drawImp->DrawPoint( &point[2], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
        if ( GetPoint( IDS_SELECT_POINT, point[3] ) ) 
        {
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawPoint( &point[3], TRGB_LIGHTGREEN );
          TestVariables::drawImp->DrawLine( point[0], point[3], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
          std::vector <MbCartPoint3D> vPoints;
          vPoints.push_back(point[0]);
          vPoints.push_back(point[1]);
          vPoints.push_back(point[2]);
          MbCurve3D * nurbs = nullptr;
          nurbs = NurbsConic_2( vPoints, point[3] );
          if( nurbs!=nullptr )
          {
            TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, nurbs );
            TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[0]) );
            TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[1]) );
            TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[2]) );
          }
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
// Создание конического сечения по 3-ем точкам и 2-ум наклонам.
// Construct a conic section by three points and two inclinations.
// ---
void CreateNurbsConic_3()
{
  MbCartPoint3D point[5];

  while ( GetPoint( IDS_SELECT_POINT_ON_CRV, point[0] ) ) 
  {
    GetExactPoint(point[0]);
//#ifdef _DRAWGI
    TestVariables::drawImp->DrawPoint( &point[0], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
    if ( GetPoint( IDS_SELECT_POINT_ON_CRV, point[1] ) ) 
    {
      GetExactPoint(point[1]);
//#ifdef _DRAWGI
      TestVariables::drawImp->DrawPoint( &point[1], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
      if ( GetPoint( IDS_SELECT_POINT, point[2] ) ) 
      {
        GetExactPoint(point[2]);
//#ifdef _DRAWGI
        TestVariables::drawImp->DrawPoint( &point[2], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
        if ( GetPoint( IDS_SELECT_POINT, point[3] ) ) 
        {
          GetExactPoint(point[3]);
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawPoint( &point[3], TRGB_LIGHTGREEN );
          TestVariables::drawImp->DrawLine( point[0], point[3], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
          if ( GetPoint( IDS_SELECT_POINT, point[4] ) ) 
          {
            GetExactPoint(point[4]);
//#ifdef _DRAWGI
            TestVariables::drawImp->DrawPoint( &point[4], TRGB_LIGHTGREEN );
            TestVariables::drawImp->DrawLine( point[2], point[4], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
            std::vector <MbCartPoint3D> vPoints;
            vPoints.push_back(point[0]);
            vPoints.push_back(point[1]);
            vPoints.push_back(point[2]);
            MbVector3D mbTangentVec1( point[0], point[3] );
            MbVector3D mbTangentVec2( point[2], point[4] );
            MbCurve3D * nurbs = nullptr;

            nurbs = NurbsConic_3( vPoints, mbTangentVec1, mbTangentVec2 );

            if( nurbs!=nullptr )
            {
              TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, nurbs );
              TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[0]) );
              TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[1]) );
              TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[2]) );
            }
          }
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
// Создание конического сечения по 2-ум точкам, 2-ум наклонам и дискриминанту.
// Construct a conic section by two points, two inclinations and a discriminant.
// ---
void CreateNurbsConic_4()
{
  MbCartPoint3D point[4];

  while ( GetPoint( IDS_SELECT_POINT_ON_CRV, point[0] ) ) 
  {
//#ifdef _DRAWGI
    TestVariables::drawImp->DrawPoint( &point[0], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
    if ( GetPoint( IDS_SELECT_POINT_ON_CRV, point[1] ) ) 
    {
//#ifdef _DRAWGI
      TestVariables::drawImp->DrawPoint( &point[1], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
      if ( GetPoint( IDS_SELECT_POINT, point[2] ) ) 
      {
//#ifdef _DRAWGI
        TestVariables::drawImp->DrawPoint( &point[2], TRGB_LIGHTGREEN );
        TestVariables::drawImp->DrawLine( point[0], point[2], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
        if ( GetPoint( IDS_SELECT_POINT, point[3] ) ) 
        {
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawPoint( &point[3], TRGB_LIGHTGREEN );
          TestVariables::drawImp->DrawLine( point[1], point[3], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
          double discr = 0.0;
          GetDouble( IDS_ENTER_DISCR, discr );
          MbCurve3D * nurbs = nullptr;
          nurbs = NurbsConic_4( point[0], point[1], MbVector3D(point[0], point[2]), MbVector3D(point[1], point[3]), discr );
          if( nurbs!=nullptr )
          {
            TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, nurbs );
            TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[0]) );
            TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[1]) );
          }
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
// Создание конического сечения по 4-ем точкам и наклону.
// Construct a conic section by four points, and an inclination.
// ---
void CreateNurbsConic_5()
{
  MbCartPoint3D point[5];

  while ( GetPoint( IDS_SELECT_POINT_ON_CRV, point[0] ) ) 
  {
//#ifdef _DRAWGI
    TestVariables::drawImp->DrawPoint( &point[0], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
    //GetExactPoint(point[0]);
    if ( GetPoint( IDS_SELECT_POINT_ON_CRV, point[1] ) ) 
    {
//#ifdef _DRAWGI
      TestVariables::drawImp->DrawPoint( &point[1], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
      //GetExactPoint(point[1]);
      if ( GetPoint( IDS_SELECT_POINT_ON_CRV, point[2] ) ) 
      {
//#ifdef _DRAWGI
        TestVariables::drawImp->DrawPoint( &point[2], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
        //GetExactPoint(point[2]);
        if ( GetPoint( IDS_SELECT_POINT_ON_CRV, point[3] ) ) 
        {
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawPoint( &point[3], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
          //GetExactPoint(point[3]);
          if ( GetPoint( IDS_SELECT_POINT_ON_CRV, point[4] ) ) 
          {
//#ifdef _DRAWGI
            TestVariables::drawImp->DrawPoint( &point[4], TRGB_LIGHTGREEN );
            TestVariables::drawImp->DrawLine( point[0], point[4], TRGB_LIGHTGREEN );
//#endif // _DRAWGI
            //GetExactPoint(point[4]);
            std::vector <MbCartPoint3D> vPoints;
            vPoints.push_back(point[0]);
            vPoints.push_back(point[1]);
            vPoints.push_back(point[2]);
            vPoints.push_back(point[3]);
            MbVector3D mbTangentVec1( point[0], point[4] );
            MbCurve3D * nurbs = nullptr;

            nurbs = NurbsConic_5( vPoints, mbTangentVec1 );

            if ( nurbs != nullptr ) {
              TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, nurbs );
              TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[0]) );
              TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[1]) );
              TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[2]) );
              TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[3]) );
            }
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание конического сечения по 5-ти точкам.
// Construct a conic section by five points.
// ---
void CreateNurbsConic_6()
{
  MbCartPoint3D point[5];

  while ( GetPoint( IDS_SELECT_POINT_ON_CRV, point[0] ) ) 
  {
    //GetExactPoint(point[0]);
//#ifdef _DRAWGI
    TestVariables::drawImp->DrawPoint( &point[0], TRGB_LIGHTGREEN );
//#endif
    if ( GetPoint( IDS_SELECT_POINT_ON_CRV, point[1] ) ) 
    {
//#ifdef _DRAWGI
      TestVariables::drawImp->DrawPoint( &point[1], TRGB_LIGHTGREEN );
//#endif
      if ( GetPoint( IDS_SELECT_POINT, point[2] ) ) 
      {
//#ifdef _DRAWGI
        TestVariables::drawImp->DrawPoint( &point[2], TRGB_LIGHTGREEN );
//#endif
        if ( GetPoint( IDS_SELECT_POINT, point[3] ) ) 
        {
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawPoint( &point[3], TRGB_LIGHTGREEN );
//#endif
          if ( GetPoint( IDS_SELECT_POINT, point[4] ) ) 
          {
            //GetExactPoint(point[4]);
//#ifdef _DRAWGI
            TestVariables::drawImp->DrawPoint( &point[4], TRGB_LIGHTGREEN );
//#endif
            std::vector <MbCartPoint3D> vPoints;
            vPoints.push_back(point[0]);
            vPoints.push_back(point[1]);
            vPoints.push_back(point[2]);
            vPoints.push_back(point[3]);
            vPoints.push_back(point[4]);
            MbCurve3D * nurbs = nullptr;

            nurbs = NurbsConic_6( vPoints );

            if ( nurbs != nullptr ) {
              TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, nurbs );
              TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[0]) );
              TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[1]) );
              TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[2]) );
              TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[3]) );
              TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPointFrame(point[4]) );
            }
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Аппроксимация кривой дугами и отрезками.
// Approximate a contour with arcs and line segments.
// ---
void MakeArcsApproximation()
{
  MbItem * obj = nullptr;

  while ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) ) {
    SPtr<MbCurve3D> curve( ::GetCurve3D( obj, false ) );
    if ( curve != nullptr ) {
      double eps = 0.1;
      double radius1_ = Math::minRadius;
      double radius2_ = Math::maxRadius;

      if ( GetDouble(IDS_ENTER_PRECISION, eps) ) {
        if ( GetDouble(IDS_ENTER_RADIUS, radius1_) ) {
          if ( GetDouble(IDS_ENTER_RADIUS, radius2_) ) {
            MbCurve3D * poly = nullptr;
            MbResultType res = ::CreatePolyArcCurve3D( *curve, poly, eps, radius1_,radius2_ );

            if ( res == rt_Success ) {
              if ( poly->IsA() == st_Contour3D ) {
                MbContour3D * cntr = static_cast<MbContour3D *>( poly );
                size_t cnt = cntr->GetSegmentsCount();
                for ( size_t i = 0; i < cnt; i++ ) {
                  MbCurve3D * segment = dynamic_cast<MbCurve3D *>( &cntr->GetSegment(i)->Duplicate() );
                  if ( i % 2 == 0 ) {
                    TestVariables::drawImp->DrawItem( segment, TRGB_CERISE );
                    TestVariables::viewManager->AddObject( TestVariables::CONIC_Style, segment );
                  }
                  else {
                    TestVariables::drawImp->DrawItem( segment, TRGB_AZURE );
                    TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, segment );
                  }
                }
              }
              ::DeleteItem( poly );
            }
            else if ( res == rt_AccuracyError ) {
              TestVariables::viewManager->AddObject( TestVariables::ARC_Style, poly );
              GetDouble(RT_ACCURACY, eps);
            }
            else
              ::PutErrorMessage( res, IDS_FAILED_TO_APPROXIMATE );
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создать развертку кривой.
// Construct an unwrapping curve.
// ---
void CreateUnwrappingCurve()
{
  MbCartPoint3D p[2];
  MbItem * nearest = nullptr;

  if ( GetGObj(IDS_SELECT_CURVE, st_Curve3D, &nearest) && (nearest->IsA() == st_SpaceInstance) ) {
    MbCurve3D * curve = (MbCurve3D *)( ((MbSpaceInstance *)nearest)->GetSpaceItem() );

    if ( GetPoint( IDS_SELECT_POINT, p[1] ) ) 
    {
      TestVariables::drawImp->DrawPoint( &p[1], TRGB_LIGHTGREEN );

      WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();
      MbPlacement3D place(currentView3D->GetNullPlacement());
      MbPlacement3D placement( place );

      MbCurve3D * unwrap = nullptr;
      unwrap = UnwrapCurve( *curve, placement, p[1], Math::deviateSag );

      if ( unwrap != nullptr )
        TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, unwrap );
    }
  }
}


//------------------------------------------------------------------------------
// Создать кривые, оборачивающие поверхность.
// Construct curves that wrap the surface.
// ---
void SurfaceCurvesWrapping()
{
  MbItem * item = nullptr; // Образующая. Generatrix.
  MbItem * item0 = nullptr;
  ConstPlaneCurvesSPtrVector curves;
  ParamPointsVector points;
  MbPlacement3D place;

  // Собираем кривые. Collect curves.
  while ( ::GetAnyCurve( IDS_SELECT_GENER_CURVE, &item ) ) {
    if ( item != item0 ) {
      MbPlacement3D crvPlace;
      MbCurve * curve( ::GetCurveOnPlace(item, crvPlace) );

      if ( curve != nullptr ) {
        if ( item0 != nullptr ) {
          TestVariables::viewManager->ToggleObject( item0 );
          if ( !place.Colinear( crvPlace ) )
            break;
          MbMatrix mtr;
          if ( crvPlace.GetMatrixToPlace( place, mtr ) && !mtr.IsSingle() )
            curve->Transform( mtr );
        }
        else {
          place.Init( crvPlace );
        }
        curves.push_back( ConstPlaneCurveSPtr(curve) );
        TestVariables::viewManager->ToggleObject( item );
        item0 = item;
      }
    }
  }
  if ( item0 != nullptr )
    TestVariables::viewManager->ToggleObject( item0 );

  if ( curves.size() > 0 ) {
    MbCartPoint3D p[2];
  
    // Указываем точку соответствия на эскизе. Specify the matching point on the sketch.
    if ( ::GetPoint( IDS_SELECT_SKETCH_POINT, p[1] ) ) {
      TestVariables::drawImp->DrawPoint( &p[1], TRGB_LIGHTGREEN );

      WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();
      MbPlacement3D placement( currentView3D->GetNullPlacement() );
      MbCartPoint xy; // Точка соответствия на эскизе. The matching point on the sketch.
      MbMatrix3D matr;

      place.DirectPointProjection( p[1], placement.GetAxisZ(), xy.x, xy.y );

      // Указываем точку соответствия на поверхности/грани. Specify the matching point on the surface/face.
      if ( ::GetPoint( IDS_SELECT_SURFACE_POINT, p[0] ) ) {
        TestVariables::drawImp->DrawPoint( &p[0], TRGB_LIGHTGREEN );
        placement.Init( currentView3D->GetNullPlacement() );

        SurfaceSPtr surface;
        MbFace * face = nullptr;

        // Ищем поверхность. Looking for a surface.
        surface = SelectSurface( &matr );
        // Ищем грань тела. Looking for a face of the solid.
        if ( surface.is_null() ) {
          face = SelectFace( &matr );
          surface = &face->SetSurface().SetSurface();
        }

        if ( !surface.is_null() ) {
          MbCartPoint uv; // Точка соответствия на поверхности. The matching point on the surface.
          double angle = 0.; // Угол наклона кривых на поверхности. The angle of inclination of the curves on the surface.
          bool keepLeng = true;
          bool isBoundsCut = true;  // Усекать кривую по границам поверхности. // Shall the result be cut by the surface boundary.

          ::GetDouble( IDS_ENTER_SLOPE_ANGLE, angle );
          angle *= M_DEGRAD; // Перевод в радианы. Translation to radians.

          int sl = 1;
          ::GetInt( IDS_ENTER_TRUNCATE, sl );
          if ( sl != 0 )
            isBoundsCut = true;
          else
            isBoundsCut = false;

          sl = 1;
          ::GetInt( IDS_SAVE_LENGTH, sl );
          if ( sl != 0 )
            keepLeng = true;
          else
            keepLeng = false;

          if ( surface->NearDirectPointProjection( p[0], placement.GetAxisZ(), uv.x, uv.y, true, nullptr ) ) {
            MbSNameMaker operNames( ct_WrapCurveCreator, MbSNameMaker::i_SideNone, 0 );  // Именователь операции. // Operation name.
            MbVector3D norm;
            face == nullptr ? surface->Normal( uv.x, uv.y, norm ) : face->Normal( uv.x, uv.y, norm );
            bool sense = ( (placement.GetAxisZ() * norm) > NULL_EPSILON );
            MbWrapValues vals( place, xy, *surface, uv, false, angle, sense, keepLeng );
            MbCurvesWrappingParams parameters( curves, points, vals, operNames, isBoundsCut, false );
            parameters.SetPrecision( Math::paramEpsilon );

            MbCurvesWrappingResults resPar; // Результат операции. // Operation result.
            MbResultType res = ::WrapCurve( parameters, resPar );
            if ( res == rt_Success ) {
              if ( ::GetBool(CURVE_AS_WIREFRAME, false) ) { // С wireFrame на выходе или с набором кривых. // With the resulting wireFrame or with the curves array.
                TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, resPar.GetWireFrameCurves() );  // Отображаем каркас.  // Show the wireframe.
              }
              else {
                SpaceCurvesSPtrVector resCurves;
                if ( resPar.GetCurves(resCurves) ) {
                  for ( SpaceCurvesSPtrVector::iterator it = resCurves.begin(); it != resCurves.end(); ++it ) { // Отображаем получившиеся кривые.  // Show the resulting curves.
                    TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, *it );
                  }
                }
              }
            }
            else {
              ::PutErrorMessage( res, IDS_TITLE_ERR );
            }
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Получить параметры сворачивания/разворачивания точек
// Get parameters to wrapping/unwrapping points.
// ---
static
bool GetPointsWrapUnwrapParameters( SurfaceSPtr &   surface,
                                    MbPlacement3D & place,
                                    MbCartPoint &   xy,
                                    MbCartPoint &   uv,
                                    bool            isWrap, // True для сворачивания, false для разворачивания. // True for wrapping and false for unwrapping.
                                    double &        angle,
                                    double &        cutParam,
                                    bool &          sense,
                                    bool &          isBoundsCut )
{
  bool res( false );

  surface = nullptr;
  angle = 0.0;
  sense = true;
  cutParam = 0.0;
  isBoundsCut = true;

  MbItem * obj = nullptr;
  if ( ::GetGObj( IDS_SELECT_PLACEMENT, st_Surface, &obj ) ) {  // Выделить поверхность для задания плейсмента. // Select a surface to initialize placement
    MbSpaceInstance * spInst = static_cast<MbSpaceInstance *>( obj );
    if ( spInst->GetSpaceItem() != nullptr ) {
      MbSpaceItem * spItem = static_cast<MbSpaceItem *>( spInst->SetSpaceItem() );
      if ( spItem->Family() == st_Surface ) {
        surface = static_cast<MbSurface *>( spItem );
      }
    }
  }

  if ( surface.is_null() || !surface->GetPlacement( &place ) ) {
    place = TestVariables::viewManager->GetActiveWindow()->GetNullPlacement();
  }

  // Ищем поверхность на которую будем сворачивать. // Looking for a surface, which we will wrap to.
  surface = SelectSurface();
  if ( !surface.is_null() ) {
    MbCartPoint3D pt[2];
    if ( ::GetPoint(IDS_SELECT_SKETCH_POINT, pt[0]) && ::GetPoint(IDS_SELECT_SURFACE_POINT, pt[1]) ) {  // Начала координат на плоскости и поверхности.  // Origins in the plane and in the surface.
      if ( surface->NearPointProjection(pt[1], uv.x, uv.y, true) ) {
        place.PointProjection( pt[0], xy.x, xy.y );
        res = true;
      }
    }
    else {
      // Соответствующие точки на поверхности и плоскости берем из критерия близости.
      // Corresponding points in the surface and in the plane are taken according to "minimal distance" criteria.
      surface->DistanceToSurface( MbPlane(place), uv.x, uv.y, xy.x, xy.y );
      res = true;
    }

    if ( res ) {
      MbVector3D norm;
      surface->Normal( uv.x, uv.y, norm );
      sense = ( (place.GetAxisZ() * norm) > NULL_EPSILON );

      ::GetDouble( IDS_ENTER_SLOPE_ANGLE, angle );
      angle *= M_DEGRAD; // Перевод в радианы. Translation to radians.

      if ( isWrap ) {
        ::GetDouble( IDS_ENTER_PAR_ON_EDGE, cutParam ); // Место резки периодической поверхности. // The place, where the periodic surface is to be cut.
      }
      else {
        int sl = 1;
        ::GetInt( IDS_ENTER_TRUNCATE, sl ); // Нужно ли обрезать полученные кривые по границам поверхности? // Whether the result curves to be cut with the surface bounds?
        if ( sl != 0 )
          isBoundsCut = true;
        else
          isBoundsCut = false;
      }
    }
  }

  return res;
}


//------------------------------------------------------------------------------
// Перенести точки с плоскости на поверхность.
// Translate points from the plane into the surface.
// ---
void SurfacePointsWrapping()
{
  MbPlacement3D place;  // Локальная система координат (ЛСК). // The local coordinate system (LCS).
  SurfaceSPtr surface;  // Ищем поверхность на которую будем сворачивать. // Looking for a surface, which we will wrap to.
  MbCartPoint xy, uv;   // Точки соответствия ЛСК и поверхности.  // Points to correspondence LCS and surface.
  double angle = 0.0;   // Угол наклона кривых на поверхности. // The angle of inclination of the curves on the surface.
  bool sense( false );  //Совпадают ли направления оси "X" ЛСК и оси "U" поверхности? // Whether the directions of the "X" axis of the LSC and the "U" axis of the surface coincide?
  double cutParam = 0.0;  // Параметр "резки" периодической поверхности. // Periodic surface cutting parameters.
  bool isBoundsCut = true;  // Усекать кривую по границам поверхности. // Shall the result be cut by the surface boundary.

  if ( ::GetPointsWrapUnwrapParameters(surface, place, xy, uv, true, angle, cutParam, sense, isBoundsCut) ) {  // Параметры проинициализированы. // Parameters have been initialized.
    MbCartPoint3D pt;
    ParamPointsVector pointsToWrap;
    ConstPlaneCurvesSPtrVector curvesToWrap;

    while ( ::GetPoint(IDS_SELECT_POINT, pt) ) {  // Получить набор сворачиваемых точек
      pointsToWrap.push_back( MbCartPoint() );
      place.PointProjection( pt, pointsToWrap.back().x, pointsToWrap.back().y );
      TestVariables::drawImp->DrawPoint( &pt, TRGB_LIGHTGREEN );
    }

    MbSNameMaker operNames( ct_WrapPointCreator, MbSNameMaker::i_SideNone, 0 );  // Именователь операции. // Operation name.
    MbWrapValues vals( place, xy, *surface, uv, false, angle, sense, true );
    MbCurvesWrappingParams parameters( curvesToWrap, pointsToWrap, vals, operNames, isBoundsCut, false );
    parameters.SetPrecision( Math::paramEpsilon );

    MbCurvesWrappingResults resPar; // Результат операции. // Operation result.
    MbResultType res = ::WrapCurve( parameters, resPar );
    if ( res == rt_Success ) {
      if ( ::GetBool(POINTSET_AS_POINTFRAME, false) ) { // С wireFrame из MbPointCurve на выходе или с набором точек. // With the resulting wireFrame of MbPointCurve or with the curves array.
        TestVariables::viewManager->AddObject( TestVariables::POINT_Style, resPar.GetWireFramePoints() );  // Отображаем каркас.  // Show the wireframe.
      }
      else {
        SpacePointsVector resPoints;
        if ( resPar.GetPoints(resPoints) ) {
          for ( SpacePointsVector::iterator it = resPoints.begin(); it != resPoints.end(); ++it ) { // Отображаем получившиеся точки.  // Show the resulting points.
            TestVariables::viewManager->AddObject( Style(5, TestVariables::LIGHTBLUE), new MbPoint3D(*it) );
          }
        }
      }
    }
    else {
      ::PutErrorMessage( res, IDS_TITLE_ERR );
    }
  }
  else {
    ::MessageBoxEx( IDS_INFO_ITEMS_NOT_CREATED, IDS_TITLE_ERR );
  }
}


//------------------------------------------------------------------------------
// Создать кривые, развёрнутые с поверхности на плоскость.
// Create curves, unwrapped from a surface to a plane.
// ---
void SurfaceCurvesUnwrapping()
{
  MbItem * item = nullptr;
  MbSpaceItem * sitem = nullptr;
  SurfaceSPtr unwrapSurface; // Поверхность, с которой разворачиваем. Surface to unwrap from.
  ParamPointsVector points;
  ConstSpaceCurvesSPtrVector curves;
  if ( ::GetGObj(IDS_SELECT_SURFACE, st_Surface, &item) &&  // Попробуем найти. Trying to find.
       item != nullptr && item->IsA() == st_SpaceInstance )
  {
    sitem = static_cast<MbSpaceInstance *>( item )->SetSpaceItem();
    if ( sitem != nullptr && sitem->Family() == st_Surface ) 
      unwrapSurface = static_cast< MbSurface * >( sitem );
  }
  if ( unwrapSurface == nullptr ) // Поищем как грань тела. Look for the surface as a face of a solid.
  {
    MbItem * dummy = nullptr;
    MbTopItem * face = nullptr;
    if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &dummy, &face ) ) {
      TestVariables::viewManager->ToggleObject(dummy);
      MbFace * sface = static_cast<MbFace*>( face );
      unwrapSurface = &sface->SetSurface();
    }
  }
  if ( unwrapSurface == nullptr ) {
    ::MessageBoxEx( RT_SURFACE_NOT_SELECTED, IDS_TITLE_ERR );
    return;
  }
  item = nullptr;
  sitem = nullptr;
  while ( GetGObj(IDS_SELECT_CURVE, st_Curve3D, &item) )      // Набираем кривые на поверхности. Get surface curves.
  {
    if ( item == nullptr || item->IsA() != st_SpaceInstance ) // Аккуратно выбираем. Carefully choose.
      break;
    sitem = static_cast<MbSpaceInstance *>( item )->SetSpaceItem();
    if ( sitem == nullptr || sitem->Family() != st_Curve3D ) 
      break;
    TestVariables::viewManager->ToggleObject( item );         // Отрисовка. Drawing.
    ConstSpaceCurveSPtr curve3d ( static_cast<MbCurve3D *>(sitem) );
    curves.push_back( curve3d ); // Неважно, что там внутри, конструктор переварит всё. Does not matter what is inside, the constructor will take care.
  }

  MbPlacement3D place;
  unwrapSurface->GetPlacement( &place );
  item = nullptr;

  MbSurface * surf = nullptr;
  MbPlane * plane = nullptr;
  if ( ::GetGObj( IDS_SELECT_PLANE, st_Surface, &item ) ) // Попробуем найти плоскость, куда разворачивать. Try to get a plane to unwrap onto.
  {
    sitem = nullptr;
    if ( item != nullptr && item->IsA() == st_SpaceInstance )
    {
      sitem = static_cast<MbSpaceInstance *>( item )->SetSpaceItem();
      if ( sitem != nullptr && sitem->Family() == st_Surface ) 
        surf = static_cast< MbSurface * >( sitem );
    }
    if ( surf == nullptr || surf->IsA() != st_Plane ) // Если плоскость не нашлась, создадим её рядом с поверхностью.
    {                                                 // If the plane was not given, then create it near the surface.
      MbAxis3D x ( place.GetOrigin(), place.GetAxisY() );
      place.Rotate( x, M_PI_2 );                      // Повернём для красоты картинки, чтобы сбоку была.
      plane = new MbPlane(place);                     // Rotate to the side just for a better view.
      TestVariables::viewManager->AddObject( *plane );
    }
    else
      plane = static_cast< MbPlane * >( surf );

    place = plane->GetPlacement();
  }

  MbSNameMaker operNames( ct_UnwrapCurveCreator, MbSNameMaker::i_SideNone, 0 );  // Именователь операции. // Operation name.

  MbCartPoint uv( 0.0, 0.0 ), xy( 0.0, 0.0 ); // Точки соответствий для систем координат поверхности и плоскости развертки. // Correspondence points between the coordinate system of the surface and unwrap plane.
  double angle( 0.0 );  // Угол поворота.  // Rotation angle.
  bool sense( true );   // Признак совпадения осей. // The marker of axes coincidence.
  double cutParam = 0.0;  // Параметр "резки" периодической поверхности. // Periodic surface cutting parameters.
  bool keepLength = true; // Сохранять длины кривых? // Save curves length?

  if ( (unwrapSurface != nullptr) && (plane != nullptr) ) {
    // Соответствующие точки на поверхности и плоскости берем из критерия близости.
    // Corresponding points in the surface and in the plane are taken according to "minimal distance" criteria.
    unwrapSurface->DistanceToSurface( *plane, uv.x, uv.y, xy.x, xy.y );
  }

  MbWrapValues vals( place, xy, *unwrapSurface, uv, false, angle, sense, keepLength );
  MbCurvesWrappingParams params( curves, points, vals, operNames, cutParam, false );
  params.SetPrecision( Math::accuracy );

  MbCurvesWrappingResults resPar; // Результат операции. // Operation result.
  MbResultType res = ::UnwrapCurve( params, resPar );

  if ( res == rt_Success ) {
    if ( ::GetBool(CURVE_AS_WIREFRAME, false) ) { // С wireFrame на выходе или с набором кривых. // With the resulting wireFrame or with the curves array.
      TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, resPar.GetWireFrameCurves() );  // Отображаем каркас.  // Show the wireframe.
    }
    else {
      SpaceCurvesSPtrVector resCurves;
      if ( resPar.GetCurves(resCurves) ) {
        for ( SpaceCurvesSPtrVector::iterator it = resCurves.begin(); it != resCurves.end(); ++it ) { // Отображаем получившиеся кривые.  // Show the resulting curves.
          TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, *it );
        }
      }
    }
  }
  else {
    ::MessageBoxEx( RT_ERROR_RESULT_NOT_OBTAINED, IDS_TITLE_ERR );
  }
}


//------------------------------------------------------------------------------
// Перенести точки на с поверхности на плоскость.
// Translate points from the surface into the plane.
// ---
void SurfacePointsUnwrapping()
{
  MbPlacement3D place;  // Локальная система координат (ЛСК). // The local coordinate system (LCS).
  SurfaceSPtr surface;  // Ищем поверхность на которую будем сворачивать. // Looking for a surface, which we will wrap to.
  MbCartPoint xy, uv;   // Точки соответствия ЛСК и поверхности.  // Points to correspondence LCS and surface.
  double angle = 0.0;   // Угол наклона кривых на поверхности. // The angle of inclination of the curves on the surface.
  bool sense( false );  //Совпадают ли направления оси "X" ЛСК и оси "U" поверхности? // Whether the directions of the "X" axis of the LSC and the "U" axis of the surface coincide?
  double cutParam = 0.0;  // Параметр "резки" периодической поверхности. // Periodic surface cutting parameters.
  bool isBoundsCut = true;  // Усекать кривую по границам поверхности. // Shall the result be cut by the surface boundary.

  if ( ::GetPointsWrapUnwrapParameters(surface, place, xy, uv, false, angle, cutParam, sense, isBoundsCut) ) {  // Параметры проинициализированы. // Parameters have been initialized.
    MbCartPoint3D pt;
    ConstPlaneCurvesSPtrVector curvesToUnWrap;
    ParamPointsVector pointsToUnWrap;

    while ( ::GetPoint(IDS_SELECT_POINT, pt) ) {  // Получить набор сворачиваемых точек
      pointsToUnWrap.push_back( MbCartPoint() );
      if ( !surface->NearPointProjection(pt, pointsToUnWrap.back().x, pointsToUnWrap.back().y, true) )
        pointsToUnWrap.pop_back();  // Исключаем. // Exclude.
      else {
        TestVariables::drawImp->DrawPoint( &pt, TRGB_LIGHTGREEN );
        MbCartPoint3D pp;
        surface->_PointOn( pointsToUnWrap.back(), pp );
        TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPoint3D(pp) );
      }
    }

    MbSNameMaker operNames( ct_UnwrapPointCreator, MbSNameMaker::i_SideNone, 0 );  // Именователь операции. // Operation name.
    MbWrapValues vals( place, xy, *surface, uv, false, angle, sense, true );
    MbCurvesWrappingParams parameters( curvesToUnWrap, pointsToUnWrap, vals, operNames, cutParam, false );
    parameters.SetPrecision( Math::paramEpsilon );

    MbCurvesWrappingResults resPar; // Результат операции. // Operation result.
    MbResultType res = ::UnwrapCurve( parameters, resPar );
    if ( res == rt_Success ) {
      if ( ::GetBool(POINTSET_AS_POINTFRAME, false) ) { // С wireFrame из MbPointCurve на выходе или с набором точек. // With the resulting wireFrame of MbPointCurve or with the curves array.
        TestVariables::viewManager->AddObject( TestVariables::POINT_Style, resPar.GetWireFramePoints() ); // Отображаем каркас.  // Show the wireframe.
      }
      else {
        SpacePointsVector resPoints;
        if ( resPar.GetPoints(resPoints) ) {
          for ( SpacePointsVector::iterator it = resPoints.begin(); it != resPoints.end(); ++it ) { // Отображаем получившиеся точки.  // Show the resulting points.
            TestVariables::viewManager->AddObject( Style(5, TestVariables::LIGHTBLUE), new MbPoint3D(*it));
          }
        }
      }
    }
    else {
      ::PutErrorMessage( res, IDS_TITLE_ERR );
    }
  }
  else {
    ::MessageBoxEx( IDS_INFO_ITEMS_NOT_CREATED, IDS_TITLE_ERR );
  }
}


//------------------------------------------------------------------------------
// Вспомогательный класс для сохранения и визуализации констрейнов на точки и производные 
// при аппроксимации полилиний.
// ---
class SmoothConstraintsData
{
private:

  struct ConstraintData
  {
    MbItem * _point;      // Указатель на точку, которая создается для визуализации на месте фиксируемой точки полилинии.
    MbItem * _derivative; // Указатель на отрезок, который создается для визуализации констрейна на производную.
                          // Начинается в фиксируемой точки, его направление и длина используются для производной 
                          // (можно редактировать перед следующим вызовом аппроксимации).

    ConstraintData() : _point( nullptr ), _derivative( nullptr ) {}
    ConstraintData( MbItem * point, MbItem * derivative ) : _point( point ), _derivative( derivative ) {}
    ~ConstraintData() {}
  };

  std::map<size_t, ConstraintData> _mapConstraints; // Контейнер для данных фиксируемых точек.
  const MbPolyline3D *             _pPoly;          // Указатель на полилинию, к которой относятся констрейны.
  double                           _length;         // Длина полилинии (используется для оценки длин производных).
  mutable c3d::SpaceVectorsVector  _ders;           // Вспомогательный массив векторов производных.

public:
  SmoothConstraintsData() : _pPoly( nullptr ), _length( 0. ) {}
  ~SmoothConstraintsData() {}

  /// \ru Инициализировать полилинией.
  void Init( const MbPolyline3D * pPoly )
  {
    _pPoly = pPoly;
    _length = pPoly->GetLengthEvaluation();
  }

  /// \ru Получить указатель на полилинию.
  const MbPolyline3D * Poly() const { return _pPoly; }

  /// \ru Очистить данные, удалить вспомогательные объекты из модели.
  void Clear()
  {
    for ( auto it : _mapConstraints ) {
      auto & data = it.second;
      if ( data._point != nullptr )
        TestVariables::viewManager->DeleteObject( data._point );
      if ( data._derivative != nullptr )
        TestVariables::viewManager->DeleteObject( data._derivative );
    }
    _mapConstraints.clear();
    _pPoly = nullptr;
  }

  /// \ru Присутствует ли уже точка с таким индексом в наборе.
  bool IsPresent( size_t idx ) { return _mapConstraints.find( idx ) != _mapConstraints.end(); }

  /// \ru Есть ли хотя бы одно ограничение.
  bool Empty() const { return _mapConstraints.empty(); }

  /// \ru Получить ограничения для аппроксимации.
  void GetConstraints( std::vector<std::pair<size_t, const MbVector3D *>> & constraints ) const
  {
    const auto count = _mapConstraints.size();
    constraints.clear();
    _ders.clear();
    c3d::IndicesVector aux;
    aux.reserve( count );
    _ders.reserve( count );
    constraints.reserve( count );
    for ( auto it : _mapConstraints ) {
      constraints.emplace_back();
      auto & constr = constraints.back();
      constr.first = it.first;
      const auto & data = it.second;
      if ( data._derivative != nullptr ) {
        const MbLineSegment3D * pSeg = (MbLineSegment3D *)( (MbSpaceInstance *)data._derivative )->GetSpaceItem();
        aux.push_back( constraints.size() - 1 );
        _ders.emplace_back( pSeg->GetPoint1(), pSeg->GetPoint2() );
      }
    }

    for ( size_t i = 0, n = aux.size(); i < n; ++i )
      constraints[aux[i]].second = &_ders[i];
  }

  /// \ru Удалить точку с указанным индексом из набора.
  void Remove( size_t idx ) 
  {
    auto it = _mapConstraints.find( idx );
    if ( it != _mapConstraints.end() ) {
      auto & data = it->second;
      if ( data._point != nullptr )
        TestVariables::viewManager->DeleteObject( data._point );      // Удаляем визуализирующую точку
      if ( data._derivative != nullptr )
        TestVariables::viewManager->DeleteObject( data._derivative ); // Удаляем визуализирующий производную отрезок
      _mapConstraints.erase( idx );
    }
  }

  /// \ru Добавить точку с указанным индексом в набор вместе с ее производной.
  void Add( size_t idx, bool bDer )
  {
    if ( _pPoly == nullptr )
      return;

    if ( _mapConstraints.find( idx ) == _mapConstraints.end() ) {
      MbCartPoint3D pnt;
      _pPoly->GetPoint( idx, pnt ); // Положение фиксируемой точки.
      MbPoint3D * pApex = new MbPoint3D( pnt );
      MbItem * itemPoint = TestVariables::viewManager->AddObject( TestVariables::POINT_Style, pApex ); // Создаем точку в модели для визуализации
      MbItem * itemDer( nullptr );
      if ( bDer ) {
        double prm;
        _pPoly->NearPointProjection( pnt, prm, false );
        MbVector3D dir;
        _pPoly->_FirstDer( prm, dir );
        dir.Normalize();
        MbCartPoint3D pt2( pnt );
        pt2.Add( dir, _length ); // Оцениваем направление производной по полилинии, длину берем равной длине полилинии
        MbLineSegment3D * pSeg = new MbLineSegment3D( pnt, pt2 );
        itemDer = TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, pSeg ); // Создаем отрезок в модели для визуализации.
                                                                                             // Перед следующей аппроксимацией его можно редактировать путем
                                                                                             // передвигания конца, не лежащего на полилинии.
      }

      _mapConstraints[idx] = ConstraintData( itemPoint, itemDer );
    }
  }
};


//------------------------------------------------------------------------------
// Аппроксимация гладкой нурбс кривой.
// ---
void FitSmoothNurbs( size_t mode )
{
  MbItem * nearest = nullptr;
  if ( GetGObj( IDS_SELECT_POLYLINE, st_Curve3D, &nearest ) )
  {
    if ( nearest != nullptr && ( nearest->IsA() == st_SpaceInstance ) && ( (MbSpaceInstance *)nearest )->GetSpaceItem()->IsA() == st_Polyline3D )
    {
      const MbPolyline3D * pPoly = (MbPolyline3D *)( (MbSpaceInstance *)nearest )->GetSpaceItem();
      MbApproxNurbsParameters<MbCartPoint3D, MbVector3D, MbNurbs3D> param;

      if ( mode == 0 )
      {
        // Аппроксимировать точки с заданной точностью. Узловой вектор и сглаживание будут подобраны автоматически.
        static SmoothConstraintsData dataConstr;
        if ( dataConstr.Poly() == nullptr || dataConstr.Poly() != pPoly ) // Заданных констрейнов не существует, либо выбрали другую полилинию - подготавливаем контейнер к заполнению
        {
          dataConstr.Clear();
          dataConstr.Init( pPoly );
        }

        if ( !dataConstr.Empty() ) // Уже есть заданные и, возможно, редактированные ограничения - используем их, либо удаляем и сбрасываем контейнер
        {
          const bool bUse = GetBoolean( IDS_ENTER_USE_CONSTRAINTS );
          if ( !bUse )
          {
            dataConstr.Clear();
            dataConstr.Init( pPoly );
            TestVariables::viewManager->RefreshModel();
            return;
          }
        }

        int closed = 0;
        if ( GetInt( IDS_ENTER_SPLINE_ORDER, TestVariables::degree )
             && GetDouble( IDS_ENTER_APPROX_ACCURACY, TestVariables::tolerance )
             && GetInt( IDS_ENTER_CLOSED_BOOL, closed ) )
        {
          c3d::SpacePointsVector  aPt;
          pPoly->GetPointList( aPt );

          if ( dataConstr.Empty() )
          {
            const size_t idMax = aPt.size() - 1; // Подсказка - максимальный индекс в полилинии

            size_t idx = idMax;
            int idxInt {};
            while ( GetInt( IDS_ENTER_FIXED_POINT_ID, idxInt ) ) // Запрашиваем индекс фиксируемой точки
            {
              if (idxInt >= 0) {
                idx = static_cast<size_t>(idxInt);
                if ( idx <= idMax )
                {
                  if ( dataConstr.IsPresent( idx ) ) // Если она уже есть - удаляем
                  {
                    dataConstr.Remove( idx );
                    TestVariables::viewManager->RefreshModel();
                  }
                  else
                  {
                    const bool bDer = GetBoolean( IDS_ENTER_FIXED_DERIVATIVE ); // Запрашиваем, фиксировать ли производную
                    dataConstr.Add( idx, bDer );
                  }
                }
              }
            }
          }

          std::vector<std::pair<size_t, const MbVector3D *>> constraints;
          dataConstr.GetConstraints( constraints );
          param.InitConstraints( aPt, TestVariables::degree, TestVariables::tolerance, closed == 1, constraints );
        }
      }
      else if ( mode == 1 )
      {
        // Аппроксимировать точки с заданным количеством сегментов в узловом векторе.
        double smooth = 0.0000001;
        int orderSmooth = 3;
        int nSpan = 16;
        double extBeg = 0.;
        double extEnd = 0.;
        int closed = 0;
        if ( GetInt( IDS_ENTER_SPLINE_ORDER, TestVariables::degree )
             /*&& GetInt( IDS_ENTER_NUMBER_N, orderSmooth )*/
             && GetInt( IDS_ENTER_NUMBER_N, nSpan )
             && GetDouble( IDS_SMOOTH_COEF, smooth )
             && GetDouble( IDS_EXTEND_CRV_BEG, extBeg )
             && GetDouble( IDS_EXTEND_CRV_END, extEnd )
             && GetDouble( IDS_ENTER_APPROX_ACCURACY, TestVariables::tolerance )
             && GetInt( IDS_ENTER_CLOSED_BOOL, closed ) )
        {
          c3d::SpacePointsVector  aPt;
          pPoly->GetPointList( aPt );

          param.InitUniform( aPt, TestVariables::degree, nSpan, smooth, orderSmooth == 3 ? MbeSmoothingMethod::sm_CurvatureVariance
                             : MbeSmoothingMethod::sm_Curvature, TestVariables::tolerance, closed == 1, extBeg, extEnd );
        }
      }

      if ( !param.GetPoints().empty() )
      {
        MbApproxNurbsCurveResult<MbNurbs3D> result;
        const MbResultType res = ApproximatePolylineByNurbs( param, result );
        if ( res != rt_ApproxError )
        {
          TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, result.GetNurbsCurve() );
          TCHAR aux[60];
          _sntprintf( aux, 60, _T( "max = %g \n mean = %g" ), result.GetError().GetMaxError(), result.GetError().GetMeanError() );
          if ( res == rt_Success )
            MessageBoxEx( aux, _T( "Success" ) );
          else
            MessageBoxEx( aux, _T( "Accuracy error" ) );
        }
        else
          ::PutErrorMessage( res, IDS_TITLE_ERR );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Аппроксимация аналитической кривой.
// ---
void FitCurveLs( size_t mode )
{
  MbItem * nearest = nullptr;
  if ( GetGObj( IDS_SELECT_POLYLINE, st_Curve3D, &nearest ) ) {
    if ( nearest != nullptr && ( nearest->IsA() == st_SpaceInstance ) && ( (MbSpaceInstance *)nearest )->GetSpaceItem()->IsA() == st_Polyline3D ) {
      const MbPolyline3D * pPoly = static_cast<const MbPolyline3D *>( ( (MbSpaceInstance *)nearest )->GetSpaceItem() );

      SpacePointsVector points;
      pPoly->GetPoints( points );

      MbAccurateTimer timer;
      timer.Begin();

      MbResultType res = rt_Error;
      MbCurveFitResults3D results;
      const double tol( 0.001 );
      if ( mode == 0 ) {
        MbCurveFitParameters3D params( MbCurveFitParametersBase::cft_Circle, tol );
        params.SetPoints( points );
        res = FitCurve( params, results );
      }
      else if ( mode == 1 ) {
        MbCurveFitParameters3D params( MbCurveFitParametersBase::cft_Line, tol );
        params.SetPoints( points );
        res = FitCurve( params, results );
      }
      else if ( mode == 2 ) {
        MbCurveFitParameters3D params( MbCurveFitParametersBase::cft_Ellipse, tol );
        params.SetPoints( points );
        res = FitCurve( params, results );
      }
      timer.End();

      if ( TestVariables::checkOperationTime ) {
        
        const size_t bufLen = 1024;
        TCHAR buf[bufLen];
        size_t len = 0;
        len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, timer.GetLast() );
        ::MessageBoxEx( buf, IDS_RESULTS );
      }

      if ( res == rt_Success || res == rt_AccuracyError ) {
        TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, results.GetCurve() );
        TCHAR aux[256];
        _sntprintf( aux, 256, _T( "max = %g \n mean = %g \n rmsd = %g" ), results.GetTolerance(), results.GetMeanDeviation(), results.GetRmsd() );
        MessageBoxEx( aux, _T( "Success" ) );
      }
      else
        ::PutErrorMessage( res, IDS_TITLE_ERR );
    }
  }
}


//------------------------------------------------------------------------------
// Создать сплайн между заданными точками заданной длины.
// Create a spline between specified points of the specified length.
// ---
void CreateCablingCurve()
{
  MbCartPoint3D points[2];
  MbVector3D direction1, direction2;
  //double length = 0.0;
  bool res1 = false;

  if ( GetPoint( IDS_SELECT_POINT, points[0] ) &&
       GetExactPoint( (MbCartPoint3D &)direction1 ) ) {

//#ifdef _DRAWGI
    TestVariables::drawImp->DrawPoint( &points[0], TRGB_LIGHTGREEN );
    TestVariables::drawImp->DrawLine( points[0], points[0]+(direction1*20), TRGB_LIGHTGREEN );
//#endif

    if ( GetPoint( IDS_SELECT_POINT, points[1] ) &&
         GetExactPoint( (MbCartPoint3D &)direction2 ) ) {

//#ifdef _DRAWGI
      TestVariables::drawImp->DrawPoint( &points[1], TRGB_LIGHTGREEN );
      TestVariables::drawImp->DrawLine( points[1], points[1]+(direction2*20), TRGB_LIGHTGREEN );
//#endif
      res1 = true;
    }
  }

  if ( !res1 ) {
    MbItem * nearest = nullptr;

    if ( GetGObj(IDS_SELECT_CURVE, st_Curve3D, &nearest) && (nearest->IsA() == st_SpaceInstance) ) {
      MbCurve3D * curve = (MbCurve3D *)( ((MbSpaceInstance *)nearest)->GetSpaceItem() );
      double t1 = curve->GetTMin(), t2 = curve->GetTMax();
      curve->Explore( t1, false, points[0], direction1, nullptr, nullptr );
      curve->Explore( t2, false, points[1], direction2, nullptr, nullptr );
      direction2.Invert();
      res1 = true;
    }
  }

  if ( res1 ) {
    double length = points[0].DistanceToPoint( points[1] ) * ( 1.0 + ONE_THIRD );
    double maxCurvature = ONE_FIFTH;
    double accuracy = Math::metricRegion;

    ::GetDouble( IDS_POCKET_LENGTH, length );
    ::GetDouble( IDS_POCKET_CURVATURE, maxCurvature );

    MbCurve3D * result( nullptr );

    MbResultType res = ::Cabling( points[0], direction1, points[1], direction2, length, accuracy, maxCurvature, result );

    if ( result != nullptr ) {
      TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, result );
      double len = result->GetMetricLength();
      TCHAR s[128];
      _sntprintf( s, 128, RT_LENGTH_MAX_CURVATURE, len, maxCurvature );
      MessageBoxEx( s, IDS_RESULTS );
    }
    else
      ::PutErrorMessage( res, IDS_TITLE_ERR );
  }
}


//------------------------------------------------------------------------------
// Найти все балочные элементы, присутствующие в заданной модели. Find all beam elements existing in the model.
// ---
void ExtractBeam()
{
  SolidSPtr solid( ::SelectSolid() );
  if ( !solid.is_null() ) {
    MbBeamElementResults beams;  // Результат операции.  // Operation result.

    MbSNameMaker snMaker( (SimpleName)ct_BeamCurveCreator, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
    MbBeamElementParams params( snMaker );

    params.SetSectionRatio( 0.0 ); // Сечения будем располагать в начале балочных кривых. // The sections will be placed at beam curves beginning.
    const MbResultType res = ::ExtractBeamElement( *solid, params, beams );
    if ( res == rt_Success ) {

      if ( ::GetBoolean(CURVE_AS_WIREFRAME) ) { // Отображать балочный элемент в виде единого каркаса. // Show the beam element as unite wireframe.
        TestVariables::viewManager->AddObject( Style( 3, TestVariables::colors[11] )/*TestVariables::ELEMENTARY_Style*/, beams.GetWireFrame() );
      }
      else {  // Отображать каждый балочный элемент по отдельности.  // Show each beam element separately.
        SpaceCurveSPtr beamCurve;   // Балочная кривая. // The beam curve.
        SPtr<MbCurveBoundedSurface> section;  // Плоское сечение. // The planar section.

        for ( size_t idx = 0, iMax = beams.GetElementsCount(); idx < iMax; ++idx ) {
          if ( !beams.GetBeamElement( idx, beamCurve, section ) )
            continue;

          TestVariables::viewManager->AddObject( Style( 3, TestVariables::colors[11] )/*TestVariables::ELEMENTARY_Style*/, beamCurve );
          TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, section );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Слияние ребер проволочного каркаса.
// Merge edges of wire frame.
// ---
void MergeEdgesWireFrame()
{
  WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow(); 

  if ( currentView3D != nullptr ) {
    c3d::WireEdgesVector selectedEdges;
    std::vector<MbItem *> items;
    MbItem * topoItem = nullptr;
    MbEdge * edge = nullptr;
    Style select( TestVariables::SELECT_Style );
    select.SetWidth( 2 );
    while ( (edge = ::SelectEdge(topoItem)) != nullptr ) {
      selectedEdges.push_back( edge );
      items.push_back( currentView3D->AddObject(select, &edge->SetCurve(), true) );// Добавляем выделенные ребра в модель
    }
    for ( const auto & it : items ) // Удаляем выделенные ребра из модели
      currentView3D->DeleteObject( it );
    bool isExsist = false;
    const MbWireFrame * wireFrame = nullptr;
    if ( topoItem != nullptr && topoItem->IsA() == st_WireFrame ) {
      wireFrame = static_cast<MbWireFrame *>( topoItem );
      isExsist = true;
    }
    WireFrameSPtr resWireFrame( (isExsist) ? static_cast<MbWireFrame *>(&wireFrame->Duplicate()) : MbWireFrame::Create(selectedEdges, nullptr, nullptr, true, Math::DefaultMathVersion()) );
    if ( resWireFrame != nullptr ) {
      c3d::WireEdgesVector edges;
      if ( wireFrame != nullptr ) {
        for ( auto & ed:selectedEdges ) {
          size_t index = wireFrame->GetEdgeIndex( *ed );
          edges.push_back( resWireFrame->SetEdge(index) );
        }
      }
      if ( resWireFrame->MergeEdges(edges, METRIC_PRECISION) )
        currentView3D->AddObject( TestVariables::CONIC_Style, resWireFrame, true );
    }
  }
  
}
