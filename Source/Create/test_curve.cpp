#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <test_draw.h>
#include <cur_line_segment.h>
#include <cur_polyline.h>
#include <cur_contour.h>
#include <cur_offset_curve.h>
#include <cur_arc.h>
#include <curve3d.h>
#include <action_curve.h>
#include <alg_curve_equid.h>
#include <plane_instance.h>
#include <space_instance.h>
#include <mb_nurbs_function.h>
#include <last.h>

using namespace c3d;
using namespace TestVariables;


//------------------------------------------------------------------------------
// Прямоугольник
// ---
bool CreateRectangle() {
  MbCartPoint pnt[2];

  if ( GetPoint( IDS_SELECT_FIRST_POINT, pnt[0] ) &&
       GetPoint( IDS_SELECT_SECOND_POINT, pnt[1] ) &&
       ( pnt[0] != pnt[1] )  ) {
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    MbPlacement3D pl;
    if ( wind != nullptr )
      pl.Init( wind->GetOwnPlace() );

    MbCurve * poly( nullptr );
    ::RegularPolygon( pnt[0], pnt[1], 2, false, poly ); 
    viewManager->AddObject( LINE_SEG_Style, poly, &pl, true );
/*
    double a = ::fabs( pnt[0].x - pnt[1].x );
    double b = ::fabs( pnt[0].y - pnt[1].y );
    MbArc ellipse( a, b, pnt[0], 0.0 );
    MbArc * arc = new MbArc( ellipse, 0.0, M_PI_2, 1 );
    viewManager->AddObject( POLYLINE_Style, arc, &pl, true );

    MbCartPoint p0( pnt[0].x, pnt[1].y ), p1( pnt[1].x, pnt[0].y );
    ::RegularPolygon( pnt[0], pnt[1], 1, false, poly ); 
    viewManager->AddObject( LINE_SEG_Style, poly, &pl, true );
    ::RegularPolygon( p0, p1, 1, false, poly ); 
    viewManager->AddObject( LINE_SEG_Style, poly, &pl, true );

    arc->_PointOn( M_PI_4, p0 );
    viewManager->AddObject( ELLIPSE_Style, p0, &pl, true );
*/
    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Правильный многоугольник
// ---
bool CreateRegularPolygon() {
  MbCartPoint pnt[2];
  ptrdiff_t nVertex = 4;  //-V112
  int describe = 0;   

  if ( GetPoint( IDS_SELECT_CENTER, pnt[0] ) &&
       GetPoint( IDS_SELECT_PNT_ON_CIRCLE, pnt[1] ) &&
       ( pnt[0] != pnt[1] ) &&
       GetInt( IDS_ENTER_POINT_NUMBER, nVertex ) &&
       GetInt( IDS_ENTER_TRACING_MODE, describe ) )  {
    if (describe!=0) describe = 1;
    MbCurve * poly = 0;
    ::RegularPolygon( pnt[0], pnt[1], nVertex, describe != 0, poly ); 
    viewManager->AddObject( LINE_SEG_Style, poly );
    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Эллипс - Центр - радиусы A и B
// ---
bool CreateEllipseCentreAB() {
  double a = 20, b = 10, angle = 0;
  MbCartPoint centre;

  if (   GetDouble(IDS_ENTER_MAJOR_RADIUS, a) &&
         GetDouble(IDS_ENTER_MINOR_RADIUS, b) &&
         GetDouble(IDS_SELECT_SCALE_CENTER, angle) &&
         GetPoint(IDS_SELECT_CENTER, centre) ) {

    angle *= Math::DEGRAD;
    MbPlacement place( centre, MbDirection(cos(angle), sin(angle)) );

    MbArc *ell_tmp = new MbArc( a, b, place );

    viewManager->AddObject(ELLIPSE_Style, ell_tmp);

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Эллипс - Три точки
// ---
bool CreateEllipse3Points() {
  MbCartPoint points[3];

  if ( GetPoint(IDS_SELECT_POINT, points[0]) &&
       GetPoint(IDS_SELECT_FIRST_POINT, points[1]) &&
       GetPoint(IDS_SELECT_SECOND_POINT, points[2]) ) {

    double bb;
    MbArc *ell = new MbArc();
    ell->Init2( points[0], points[1], points[2], bb );

    viewManager->AddObject( ELLIPSE_Style, ell );

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Эллипс - Центр - 3 точки
// ---
bool CreateEllipseCentre3Points() {
  MbCartPoint centre, pnt;  
  std::vector<MbCartPoint> points; 

  points.reserve( 4 );
  if ( GetPoint(IDS_SELECT_CENTER, centre) ) {
//#ifdef _DRAWGI
    drawImp->DrawPoint( &centre, TRGB_GREEN, viewManager->GetCplaneMatrixFrom() );
//#endif // _DRAWGI
    while ( GetPoint(IDS_SELECT_POINT, pnt) ) {
      points.push_back( pnt );
//#ifdef _DRAWGI
      drawImp->DrawPoint( &pnt, TRGB_GREEN, viewManager->GetCplaneMatrixFrom() );
//#endif // _DRAWGI
    }

    MbCurve * result( nullptr );
    double angle( M_PI*2/3);
    double a( 0.0 ), b( 0.0 );
    int closed( 1 );
    GetInt( IDS_ENTER_CLOSED_BOOL, closed );
    GetDouble( IDS_ENTER_ANGLE, angle );

    //::Arc( centre, points, closed != 0, angle, a, b, result );
    ::Arc( (closed != 0) ? acw_EllipseByCenterAnd3Points : acw_ArcBy3Points, centre, points, a, b, angle, false, result );

    if ( result != nullptr )
      viewManager->AddObject( ELLIPSE_Style, result );

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Эллипс - Касание 2-х кривых
// ---
bool CreateEllipseTangent3Curves() {
  MbCartPoint p1, p2, p3;
  MbItem *obj[2];

  if (   GetPoint(IDS_SELECT_FIRST_CRVPNT, p1) &&
        (FindNearestObj( p1, pt_Curve, &obj[0] )) &&
         GetPoint(IDS_SELECT_SECOND_CRVPNT, p2) &&
        (FindNearestObj( p2, pt_Curve, &obj[1] )) &&
         GetPoint(IDS_SELECT_FIRST_POINT, p3)    ) {
    
    SPtr<const MbCurve> copy0( GetCurveCopyOnWindPlace(*viewManager, *obj[0]) );
    SPtr<const MbCurve> copy1( GetCurveCopyOnWindPlace(*viewManager, *obj[1]) );

    double t1 = copy0->PointProjection( p1 );
    double t2 = copy1->PointProjection( p2 );

    copy0->PointOn( t1, p1 );
    copy1->PointOn( t2, p2 );

    MbDirection dir1 = copy0->Tangent( t1 );
    MbDirection dir2 = copy1->Tangent( t2 );

    double angle;
    double aa, bb;
    angle = aa = bb = UNDEFINED_DBL;
    MbArc *ell = new MbArc();
    ell->Init8( p1, dir1, p2, dir2, p3, aa, bb, angle );

    // Проверка вырожденности эллипса
    if ( ell->IsDegenerate() ) {
//      WriteError( IDS_ERROR14 );
      delete ell;
    }
    else
      viewManager->AddObject(ELLIPSE_Style, ell );

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Построение дуги эллипса
// ---
bool CreateEllipseArc() {
  MbCartPoint points[4];   //-V112

  if (   GetPoint(IDS_SELECT_CENTER, points[0]) &&
         GetPoint(IDS_SELECT_POINT,  points[1]) &&
         GetPoint(IDS_SELECT_POINT,  points[2]) &&
         GetPoint(IDS_SELECT_POINT,  points[3]) ) {
    double angle;
    double aa, bb;
    angle = aa = bb = UNDEFINED_DBL;
    MbArc ell;
    ell.Init7( points[0], points[1], points[2], points[3], aa, bb, angle );

    // Проверка вырожденности эллипса
    if ( !ell.IsDegenerate() ) {
      double t1 = ell.PointProjection( points[1] );
      double t2 = ell.PointProjection( points[3] );
      MbArc * ellipse = new MbArc( ell, t1, t2, 1 );

      viewManager->AddObject( ELLIPSE_Style, ellipse );

      return true;
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Построение дуги коника
// ---
bool CreateConicArc() {
  MbCartPoint points[4];   //-V112

  if (  GetPoint(IDS_SELECT_POINT, points[0]) &&
        GetPoint(IDS_SELECT_POINT, points[1]) &&
        GetPoint(IDS_SELECT_POINT, points[2]) &&
        GetPoint(IDS_SELECT_POINT, points[3]) )
  {
    SArray<MbCartPoint> pointList;
    for ( ptrdiff_t i = 0; i < 4; i++ )     //-V112
      pointList.push_back(points[i]);

    SArray<double> knots;
    SArray<double> wList;
    MbCurve * crv = nullptr;
    ::NurbsCurve( pointList, wList, 3, knots, false, crv );
    viewManager->AddObject( ELLIPSE_Style, crv );
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// Кубический сплайн
// ---
bool CreateSpline( int curveType ) 
{
  SArray<MbCartPoint> pList;
  MbCartPoint pnt;

  while ( GetPoint( IDS_SELECT_POINT, pnt ) ) {
//#ifdef _DRAWGI
    drawImp->DrawPoint( &pnt, 0, 255, 0, viewManager->GetCplaneMatrixFrom() );
//#endif // _DRAWGI
    pList.push_back( pnt );
  }

  bool resu( pList.size() > 1 );
  if ( resu ) {
    int iCls = 0;
    GetInt( IDS_ENTER_CLOSED, iCls );
    MbCurve * result( nullptr );
    ::SplineCurve( pList, (iCls > 0), (MbePlaneType)curveType, result );
    viewManager->AddObject( BSPLINE_Style, result );
  }
  return resu;
}


//------------------------------------------------------------------------------
// В-сплайн
// ---
bool CreateNurbs() {
  MbCartPoint pnt;

  if ( GetInt( IDS_ENTER_SPLINE_ORDER, TestVariables::degree ) ) {
    if (  TestVariables::degree < 2 )   TestVariables::degree = 2;
//    if (  degree < MIN_NURBS_DEGREE )   degree = MIN_NURBS_DEGREE;
//    if (  degree > MAX_NURBS_DEGREE )   degree = MAX_NURBS_DEGREE;

    while ( GetPoint(IDS_SELECT_FIRST_POINT, pnt ) ) {
      SArray<MbCartPoint> pList;
//#ifdef _DRAWGI
      drawImp->DrawPoint( &pnt, 0, 255, 0, viewManager->GetCplaneMatrixFrom() );
//#endif // _DRAWGI
      pList.push_back( pnt );

      while ( GetPoint(IDS_SELECT_FIRST_POINT, pnt ) ) {
//#ifdef _DRAWGI
        drawImp->DrawPoint( &pnt, 0, 255, 0, viewManager->GetCplaneMatrixFrom() );
//#endif // _DRAWGI
        pList.push_back( pnt );
      }

      if ( ((ptrdiff_t)pList.size()) >= TestVariables::degree ) {
        int iCls = 0;
        GetInt( IDS_ENTER_CLOSED, iCls );
        SArray<double> knots;
        SArray<double> wList;

        ptrdiff_t iThrough = 0;
        GetInt( IDS_ENTER_THROUGH_ITEMS, iThrough );

        if ( iThrough <= 0 ) {
          MbCurve * crv = nullptr;
          ::NurbsCurve( pList, wList, TestVariables::degree, knots, iCls > 0, crv );
          viewManager->AddObject( BSPLINE_Style, crv );
        }
        else {
          MbCurve * seg = nullptr;
          ::SplineCurve( pList, (iCls > 0), pt_Nurbs, seg );
          viewManager->AddObject( CURVE_Style, seg );
        }
        return true;
      }
    }
    return false;
  }

  return false;
}


//------------------------------------------------------------------------------
// Построение усеченного сплайна
// ---
bool CreateTrimmSpline() {
  MbCartPoint pnt[3];
  MbItem *obj;

  if ( GetPoint( IDS_SELECT_FIRST_POINT, pnt[0] ) &&
       GetPoint( IDS_SELECT_SECOND_POINT, pnt[1] ) &&
       GetPoint( IDS_SELECT_CRV_ON_PLANE, pnt[2] ) &&
      (FindNearestObj( pnt[2], pt_Curve, &obj )) )
  {
    SPtr<const MbCurve> copy( GetCurveCopyOnWindPlace(*viewManager, *obj) );
    double t1 = copy->PointProjection( pnt[0] );
    double t2 = copy->PointProjection( pnt[1] );

    int sense = 1;

    if ( copy->IsClosed() )
      GetInt( IDS_ENTER_SENSE, sense );
    else
      sense = ( t1 < t2 ) ? 1 : - 1;

    viewManager->AddObject( LINE_SEG_Style, copy->Trimmed( t1, t2, sense ) );

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Эквидистанта к сплайну
// ---
void Spline_Equid() {
  MbItem *p_nearest;

  while ( GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &p_nearest ) ) {
    viewManager->ToggleObject(p_nearest);

    double radius = 5;
    int sav = 1;
    if ( (p_nearest->IsA() == st_PlaneInstance) &&
         GetDouble(IDS_ENTER_RADIUS, radius) &&
         GetInt(IDS_SAVE_TOPOLIGY, sav) ) {

      MbPlacement3D place( static_cast<const MbPlaneInstance *>(p_nearest)->GetPlacement() );
      const MbCurve  * curve = static_cast<const MbCurve *>( static_cast<const MbPlaneInstance *>(p_nearest)->GetPlaneItem() );

      PArray<MbCurve> equLeft( 1, 1, false );
      PArray<MbCurve> equRight( 1, 1, false );

      MbAccurateTimer ac;
      
      if ( sav == 0 ) {
        int side = (radius > 0 ) ? 0 : 1;
        bool arcMode( false ), degState( false );

        ac.Begin();
        ::Equid( curve, radius, -radius, side, arcMode, degState, equLeft, equRight );
        ac.End();
      }
      else {
        ac.Begin();
        MbCurve * equ = curve->Offset( radius );
        if (equ != nullptr) {
          if ( radius > 0 ) 
            equLeft.Add( equ );
          else
            equRight.Add( equ );
        }
        ac.End();
      }

      if ( TestVariables::checkOperationTime ) {
        const size_t bufLen = 1024;
        TCHAR buf[bufLen];
        size_t len = 0;
        len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
        ::MessageBoxEx( buf, IDS_RESULTS );
      }

      size_t i, iCount;
      for ( i = 0, iCount = equLeft.Count(); i < iCount; i++ )
        viewManager->AddObject( OFFSET_Style, equLeft[i], &place );
      for ( i = 0, iCount = equRight.Count(); i < iCount; i++ )
        viewManager->AddObject( OFFSET_Style, equRight[i], &place );
    }

    viewManager->ToggleObject(p_nearest);
  }
}


//------------------------------------------------------------------------------
// Продолженная эквидистанта
// ---
void OffsetExtendedCurve() {
  MbItem *p_nearest;

  while ( GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &p_nearest ) ) {
    viewManager->ToggleObject(p_nearest);

    const MbPlaneItem * pItem = ((MbPlaneInstance *)p_nearest)->GetPlaneItem();
    if ( (pItem != nullptr) && (pItem->Family() == pt_Curve) ) {

      if ( GetInt( IDS_ENTER_OFFSET_TYPE, TestVariables::offsetType ) ) {
        if ( GetDouble( IDS_ENTER_DISTANCE, TestVariables::offset0 ) ) {
          if ( TestVariables::offsetType > 0 ) 
            GetDouble( IDS_ENTER_DISTANCE, TestVariables::offset1 );

          SPtr<MbCurve> c( static_cast<MbCurve *>(&pItem->Duplicate()) );
          MbCurve * equ( nullptr );

          equ = ::OffsetCurve( *c, TestVariables::offset0, TestVariables::offset1, (MbeOffsetType)TestVariables::offsetType );

          viewManager->AddObject( OFFSET_Style, equ, &static_cast<MbPlaneInstance *>(p_nearest)->GetPlacement() );
        }
      }
    }

    viewManager->ToggleObject(p_nearest);
  }
}


//------------------------------------------------------------------------------
// Проекция пространственной кривой на конструктивную плоскость
// ---
void PlaneCurveBySpace()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) ) {
    MbCurve * curve = nullptr;

    if ( obj->IsA() == st_SpaceInstance ) {
      const MbSpaceItem * item = ((MbSpaceInstance*)obj)->GetSpaceItem();

      viewManager->ToggleObject(obj);
      if ( item != nullptr && (item->Family() == st_Curve3D) ) {
        MbPlacement3D place(viewManager->GetCplane());
        MbMatrix3D into;
        place.GetMatrixInto( into );
        curve = ((MbCurve3D *)item)->GetMap( into );
      }
      viewManager->ToggleObject(obj);
    }
    if ( curve!=nullptr )
      viewManager->AddObject( OFFSET_Style, curve );
  }
}


//------------------------------------------------------------------------------
// Касательная в точке
// ---
void Splinetangent() {
  MbItem *p_nearest;
  MbCartPoint points[2];

  while ( GetPoint(IDS_SELECT_SPLN_ON_PLANE, points[0]) &&
          ( FindNearestObj( points[0], pt_Nurbs, &p_nearest ) ) ) {
    viewManager->ToggleObject(p_nearest);
    SPtr<const MbCurve> copy( GetCurveCopyOnWindPlace(*viewManager, *p_nearest) );

    double delta = 1;
    if ( GetDouble(IDS_ENTER_PAR_ON_CURVE, delta)
//                 &&
//                 GetInt( &degree )
       ) {
//              delta = PMbNurbs(PSimpleGObj(p_nearest)->mathItem)->tMax * delta;
      copy->PointOn( delta, points[0] );
      MbVector fd;
        copy->FirstDer( delta, fd );
      MbVector sd;
        copy->SecondDer( delta, sd );
      MbDirection tanD = copy->Tangent( delta );
      MbLine *pl = new MbLine( points[0], tanD );

      viewManager->AddObject(LINE_Style, pl );
    }
    viewManager->ToggleObject(p_nearest);
  }
}


//------------------------------------------------------------------------------
// Преобразование в NURBS
// ---
void NurbsSpline() 
{
  MbItem * p_nearest = nullptr;
  while ( GetMObj ( IDS_SELECT_CNTR_ON_PLANE, pt_Curve, &p_nearest ) ) {
    const MbCurve * pmatobj = static_cast<const MbCurve *>( ((MbPlaneInstance*)p_nearest)->GetPlaneItem() );
    MbPlacement3D place( ((MbPlaneInstance *)p_nearest)->GetPlacement() );
    viewManager->ToggleObject(p_nearest);
    
    MbCurve * nurbs = nullptr;
    if ( GetInt ( IDS_ENTER_SPLINE_ORDER, TestVariables::degree ) ) {
      ptrdiff_t pCount = 0;
      GetInt ( IDS_ENTER_POINT_NUMBER, pCount );
      pCount = std_max( pCount, (ptrdiff_t)0 );
      double k1 = 0.0, k2 = 1.0;
      GetDouble ( IDS_ENTER_PAR_ON_CURVE, k1 );
      GetDouble ( IDS_ENTER_PAR_ON_CURVE, k2 );
      int useApprox = 1; //Использовать аппроксимацию при построении кривой
      GetInt ( IDS_ENTER_USE_APPROX, useApprox );

      MbNurbsParameters param ( TestVariables::degree, pCount, k1, k2, useApprox == 1 );
      nurbs = pmatobj->NurbsCurve ( param );
    }
    else { // Степень не введена - старый вариант вызываем
      ::NurbsCopy( *pmatobj, nurbs );
    }
    if ( nurbs != nullptr )
      viewManager->AddObject( OFFSET_Style, nurbs, &place );
    viewManager->ToggleObject(p_nearest);
  }
}

//------------------------------------------------------------------------------
// Создание срединной кривой
// ---
void CreateMedianCurve() 
{
  MbItem *p_nearest1, *p_nearest2;
  // Чтение первой кривой
  if ( GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &p_nearest1 ) ) {
    viewManager->ToggleObject(p_nearest1);
    const MbPlaneItem * pItem1 = ((MbPlaneInstance *)p_nearest1)->GetPlaneItem();
    if ( (pItem1 != nullptr) && (pItem1->Family() == pt_Curve) ) {
      
      // Чтение второй кривой
      if ( GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &p_nearest2 ) ) {
        viewManager->ToggleObject(p_nearest2);
        const MbPlaneItem * pItem2 = ((MbPlaneInstance *)p_nearest2)->GetPlaneItem();
        if ( (pItem2 != nullptr) && (pItem2->Family() == pt_Curve) ) {

          // Выбрать точку в квадранте, где строим срединную кривую
          MbCartPoint qPnt;
          //MbPlacement3D plc{ static_cast<MbPlaneInstance *>(p_nearest1)->GetPlacement() };
          if ( GetPoint(IDS_SELECT_FIRST_POINT, qPnt/*, &plc*/) ) {
            PlaneCurveSPtr curve1( static_cast<MbCurve *>(&pItem1->Duplicate()) );
            PlaneCurveSPtr curve2( static_cast<MbCurve *>(&pItem2->Duplicate()) );

            // Положение точки относительно нормали кривой
            auto pntPosition = [&qPnt]( const PlaneCurveSPtr & curve ) {
              double tproj = curve->PointProjection( qPnt );
              MbCartPoint proj;
              curve->PointOn( tproj, proj );
              MbVector n;
              curve->Normal( tproj, n );
              MbVector v( proj, qPnt );
              const double cos = n * v.GetNormalized();
              return ::signbit( cos );
            };

            const bool invCurve1 = pntPosition( curve1 );
            const bool invCurve2 = pntPosition( curve2 );
            MbMedianCurveParams params( {invCurve1, invCurve2}, true );
            MbMedianCurveResults results;
            MedianCurve( *curve1, *curve2, params, results );
            const PlaneCurveSPtr & medianCurve = results.GetMedianCurve();
            viewManager->AddObject( OFFSET_Style, medianCurve, &static_cast<MbPlaneInstance *>(p_nearest1)->GetPlacement() );

          }
        }
      }
      viewManager->ToggleObject(p_nearest2);
    }
    viewManager->ToggleObject(p_nearest1);
  }
}
