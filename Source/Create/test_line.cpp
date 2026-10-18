#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <alg_curve_distance.h>
#include <action_curve.h>
#include <alg_curve_tangent.h>
#include <plane_instance.h>
#include <last.h>

using namespace c3d;
using namespace TestVariables;


//------------------------------------------------------------------------------
// Горизонтальная прямая.
// Construct horizontal line.
// ---
bool CreateGorisontalLine() {
  MbCartPoint pnt;

  if ( GetPoint( IDS_SELECT_POINT, pnt ) ) {
    MbCurve * crv = nullptr;
    ::Line( pnt, MbCartPoint( pnt.x + 1, pnt.y ), crv );
    viewManager->AddObject( LINE_Style, crv );

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Вертикальная прямая.
// Construct vertical line.
// ---
bool CreateVerticalLine() {
  MbCartPoint pnt;

  if ( GetPoint( IDS_SELECT_POINT, pnt ) ) {
    MbCurve * crv = nullptr;
    ::Line( pnt, MbCartPoint( pnt.x, pnt.y + 1 ), crv );
    viewManager->AddObject( LINE_Style, crv );

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Прямая точка-точка.
// Construct line on two points.
// ---
void CreateLinePointPoint() {
  MbCartPoint pnt[2];

  while ( GetPoint(IDS_SELECT_FIRST_POINT, pnt[0]) &&
          GetPoint(IDS_SELECT_SECOND_POINT, pnt[1]) )  {
    MbCurve * crv = nullptr;
    ::Line( pnt[0], pnt[1], crv );
    viewManager->AddObject( LINE_Style, crv );
  }
}


//------------------------------------------------------------------------------
// Прямая точка-угол с осью 0X.
// Construct line by a point and angle with the axis OX.
// ---
void CreateLinePoint0X() {
  MbCartPoint pnt;
  double angle = 30;

  while ( GetPoint( IDS_SELECT_POINT, pnt ) &&
          GetDouble( IDS_ENTER_ANGLE, angle ) ) {
    angle *= Math::DEGRAD;
    MbCartPoint p2( pnt );
    double delta = 10;
    p2.MoveAlongLine( angle, delta );
    MbCurve * crv = nullptr;
    ::Line( pnt, p2, crv );
    viewManager->AddObject( LINE_Style, crv );
  }
}


//------------------------------------------------------------------------------
// Прямая через точку под углом к прямой.
// Construct line by a point and angle with the given line.
// ---
void CreateLinePointAngle() {
  MbCartPoint pnt;
  MbLine lTmp;
  double angle = 0;
  MbItem *curve;

  while ( GetPoint(IDS_SELECT_POINT, pnt) &&
          GetMObj(IDS_SELECT_CURVE, pt_Curve, &curve ) &&
          GetDouble(IDS_ENTER_ANGLE, angle )  )  {

    SPtr<const MbCurve> copy0( GetCurveCopyOnWindPlace(*viewManager, *curve) );
    angle = angle * Math::DEGRAD;
    ::LinePointAngle( angle, pnt, (MbLine &)*copy0, lTmp );

    //viewManager->AddObject(LINE_Style, new MbLine(lTmp) );

    const MbPlacement3D & curvePlace = static_cast<const MbPlaneInstance *>(curve)->GetPlacement();
    {
      const WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
      const MbPlacement3D & windPlace = wind->GetOwnPlacement();
      const MbCurve * curve0 = static_cast<const MbCurve *>( static_cast<const MbPlaneInstance *>(curve)->GetPlaneItem() );
      MbCartPoint p;
      windPlace.DirectPointProjection( TestVariables::cursorPoint, wind->GetAxisZ(), p.x, p.y );
      MbCartPoint3D point;
      windPlace.PointOn( p, point );
      curvePlace.PointProjection( point, p.x, p.y );
      double t;
      MbVector der;
      curve0->NearPointProjection( p, PARAM_EPSILON, PARAM_EPSILON, t, false, nullptr );
      curve0->Explore( t, true, p, der, nullptr, nullptr );
      if ( t > curve0->GetTMid() )
        der.Invert();
      lTmp.Init( p, der );
    }

    viewManager->AddObject(LINE_Style, new MbLine(lTmp), &curvePlace, true );
  }
}


//------------------------------------------------------------------------------
// Прямая параллельная прямой через точку.
// Construct a line through the point and parallel to a given line.
// ---
void CreateLineParallelPoint() {
  MbCartPoint pnt;
  MbItem *curve;

  while (  GetPoint(IDS_SELECT_POINT, pnt) &&
          GetMObj(IDS_SELECT_LINE, pt_Line, &curve ) ) {

    SPtr<const MbCurve> copy( GetCurveCopyOnWindPlace(*viewManager, *curve) );
    MbLine pl;
    LineParallelPoint( pnt, (MbLine &)*copy, pl );

    viewManager->AddObject(LINE_Style, new MbLine(pl));

  }
}


//------------------------------------------------------------------------------
// Прямая параллельная на расстоянии.
// Construct a line parallel to a given line at a given distance from it.
// ---
void CreateLineParallelDistance() {
  MbItem *curve;
  double delta = 10;

  while ( GetMObj(IDS_SELECT_LINE, pt_Line, &curve ) &&
          GetDouble(IDS_ENTER_DISTANCE, delta ) ) {

    SPtr<const MbCurve> copy( GetCurveCopyOnWindPlace(*viewManager, *curve) );
    MbLine pl;
    LineParallelDistance( delta, (MbLine &)*copy, pl );

    viewManager->AddObject(LINE_Style, new MbLine(pl));
  }
}


//------------------------------------------------------------------------------
// Прямая перпендикулярная кривой через точку.
// Construct a line through the point and perpendicular to a given line.
// ---
bool CreateLinePointPerpCurve() {
  MbCartPoint pnt;
  MbItem *obj;

  if (   GetPoint(IDS_SELECT_POINT, pnt) &&
         GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &obj ) ) {

    SPtr<const MbCurve> copy( GetCurveCopyOnWindPlace(*viewManager, *obj) );

    PArray <MbLine> pLine( 2, 2, false );
    LinePointPerpCurve( pnt, *copy, pLine );

    double tMin;
    MbCartPoint pon;
    tMin = copy->PointProjection( pnt );
    copy->PointOn(tMin, pon);
    MbCurve * seg = nullptr;
    ::Segment( pnt, pon, seg );

    for ( ptrdiff_t i = 0; i < (ptrdiff_t)pLine.Count(); i++ )
      viewManager->AddObject( LINE_Style, pLine[i] );

    viewManager->AddObject( LINE_SEG_Style, seg );

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Прямая точка-касательная к кривой.
// Construct a line passing through a point and tangent to a given curve.
// ---
void CreateLinePointTangentCurve() {
  MbCartPoint pnt;
  MbItem *curve;

  while (  GetPoint(IDS_SELECT_POINT, pnt) &&
          GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &curve ) ) {

    SPtr<const MbCurve> copy( GetCurveCopyOnWindPlace(*viewManager, *curve) );

    PArray <MbLine> pLine( 0, 2, false );
    LinePointTangentCurve( pnt, *copy, pLine );

    for ( size_t i = 0; i < pLine.Count(); i++ )
      viewManager->AddObject( LINE_Style, pLine[i] );
  }
}


//------------------------------------------------------------------------------
// Прямая касательная к двум кривым.
// Construct a line tangent to two curves.
// ---
void CreateLineTangentTwoCurves() {
  MbCartPoint pnt[2];
  MbItem *curve[2];

  while ( GetPoint(IDS_SELECT_FIRST_CURVE, pnt[0]) &&
         (FindNearestObj( pnt[0], pt_Curve, &curve[0] )) &&
          GetPoint(IDS_SELECT_SECOND_CURVE, pnt[1]) &&
         (FindNearestObj( pnt[1], pt_Curve, &curve[1] )) ) {

    SPtr<const MbCurve> copy0( GetCurveCopyOnWindPlace(*viewManager, *curve[0]) );
    SPtr<const MbCurve> copy1( GetCurveCopyOnWindPlace(*viewManager, *curve[1]) );

    PArray <MbLine> pLine( 0, 2, false );
    SArray <MbCartPoint> pTmp;

    LineTangentTwoCurves( copy0, copy1, &pLine, &pTmp );

    for ( size_t i = 0; i < pLine.Count(); i++ )
      viewManager->AddObject( LINE_Style, pLine[i] );
  }
}


//------------------------------------------------------------------------------
// Прямые, проходящие под углом к оси 0X и касательные к кривой
// Construct lines at a given angle to the axis OX and tangent to a given curve.
// ---
void CreateLineAngleTangentCurve() {
  MbItem *curve;
  double angle = 0;

  while ( GetDouble(IDS_ENTER_ANGLE, angle) &&
          GetMObj(IDS_SELECT_CRCL_ON_PLANE, pt_Curve, &curve ) ) {

    SPtr<const MbCurve> copy( GetCurveCopyOnWindPlace(*viewManager, *curve) );
    angle = angle * Math::DEGRAD;
    PArray <MbLine> pLine( 0, 2, false );

    LineAngleTangentCurve( angle, *copy, pLine );

    for ( size_t i = 0; i < pLine.Count(); i++ )
      viewManager->AddObject( LINE_Style, pLine[i] );
  }
}


//------------------------------------------------------------------------------
// Биссектриса.
// Construct a bisector.
// ---
void CreateLineBisector() {
  MbCartPoint pnt;
  MbItem *curve[2];
  MbLine lTmp;

  while ( GetPoint(IDS_SELECT_POINT, pnt) &&
          GetMObj(IDS_SELECT_LINE, pt_Line, &curve[0] ) &&
          GetMObj(IDS_SELECT_LINE, pt_Line, &curve[1] ) )  {

    SPtr<const MbCurve> copy0( GetCurveCopyOnWindPlace(*viewManager, *curve[0]) );
    SPtr<const MbCurve> copy1( GetCurveCopyOnWindPlace(*viewManager, *curve[1]) );
    LineBisector( pnt, static_cast<const MbLine &>(*copy0), static_cast<const MbLine &>(*copy1), lTmp );

    viewManager->AddObject(LINE_Style, new MbLine(lTmp));
  }
}


//------------------------------------------------------------------------------
// Отрезок - минимальное расстояние.
// Construct a line segment as the shortest distance between two curves.
// ---
bool CreateLineSegMinDistance()
{
  MbItem *gobj[2];
  MbCartPoint pnt[2];

  if ( GetPoint(IDS_SELECT_LINE, pnt[0] ) &&
       GetPoint(IDS_SELECT_LINE, pnt[1] ) ) {
    const MbCurve *curve0 = nullptr;
    const MbCurve *curve1 = nullptr;

    if ( FindNearestObj( pnt[0], pt_Curve, &gobj[0] ) )
      curve0 = static_cast<const MbCurve *>( ((MbPlaneInstance*)gobj[0])->GetPlaneItem() );
    if ( FindNearestObj( pnt[1], pt_Curve, &gobj[1] ) )
      curve1 = static_cast<const MbCurve *>( ((MbPlaneInstance*)gobj[1])->GetPlaneItem() );

    const MbPlacement3D & windPlace = viewManager->GetActiveWindow()->GetOwnPlacement();

    MbCurve * crv = nullptr;
    if ( ::Segment( pnt[0], pnt[1], crv ) != rt_Success ) {
      return false;
    }
    viewManager->AddObject( LINE_SEG_Style, crv, &windPlace );

    MbDistance dmin;

    if ( curve0 != nullptr && curve1 != nullptr && curve0 != curve1 ) {
      // У кривых могут быть разные плейсменты.
      // Curves could have different placements.
      SPtr<const MbCurve> copy0( GetCurveCopyOnWindPlace(*viewManager, *gobj[0]) );
      SPtr<const MbCurve> copy1( GetCurveCopyOnWindPlace(*viewManager, *gobj[1]) );

      if ( ::DistanceCurveCurve( *copy0, *copy1, dmin ) ) {
        copy0->PointOn( dmin.u, pnt[0] );
        copy1->PointOn( dmin.v, pnt[1] );
        MbResultType const resType = ::Segment( pnt[0], pnt[1], crv );
        if (resType == rt_Success ) {
          viewManager->AddObject( LINE_SEG_Style, crv, &windPlace );
        }

        const size_t bufLen = 1024;
        TCHAR buf[bufLen];
        size_t len = 0;
        len += (size_t)_sntprintf( buf + len, bufLen - len, RT_MIN_DISTANCE, dmin.d );
        ::MessageBoxEx( buf, IDS_RESULTS );
      }
    }
    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Отрезок точка-точка.
// Construct a line segment by two given points.
// ---
bool CreateSegmentPointPoint() {
  MbCartPoint pnt[2];

  if ( GetPoint( IDS_SELECT_FIRST_POINT, pnt[0] ) &&
       GetPoint( IDS_SELECT_SECOND_POINT, pnt[1] ) ) {

    if ( pnt[0] != pnt[1] ) {
      MbCurve * crv = 0;
      Segment( pnt[0], pnt[1], crv );
      viewManager->AddObject( LINE_SEG_Style, crv );

      return true;
    }
  }

  return false;
}


