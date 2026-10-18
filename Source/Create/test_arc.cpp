//////////////////////////////////////////////////////////////////////////////////////////
// Различные способы построения окружностей и дуг
// Various methods for constructing circles and arcs
//////////////////////////////////////////////////////////////////////////////////////////

#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <cur_arc.h>
#include <alg_circle_curve.h>
#include <plane_instance.h>
#include <last.h>

using namespace c3d;
using namespace TestVariables;


//----------------------------------------------------------------------------------------
// Создать окружность с заданными центром и радиусом
// Create a circle with specified center and radius
// ---
void CreateCircleCentreRadius() 
{
  MbCartPoint pnt;
  double radius = 20;
  while ( GetPoint(IDS_SELECT_CENTER, pnt) &&
          GetDouble(IDS_ENTER_RADIUS, radius) )  
  {
    viewManager->AddObject( CIRCLE_Style, new MbArc(pnt, radius) );
  }
}

//----------------------------------------------------------------------------------------
// Создать окружность с указанными центром и произвольной точкой окружности
// Create a circle with pointed center and arbitrary point of the circle
// ---
void CreateCircleCentrePoint() 
{
  MbCartPoint pnt[2];
  while ( GetPoint( IDS_SELECT_CENTER, pnt[0] ) &&
          GetPoint( IDS_SELECT_PNT_ON_CIRCLE, pnt[1] ) ) 
  {
    if ( pnt[0] != pnt[1] ) 
    {
      viewManager->AddObject( CIRCLE_Style, new MbArc( pnt[0], pnt[1] ) );
    }
  }
}

//----------------------------------------------------------------------------------------
// Построить окружность по радиусу и точке на ней, центр окружности лежит на заданной кривой.
// Create a circle by radius and coincident point, a circle center lies on a given curve.
// ---
void CreateCircleCentreOnCurveRadPointOn() 
{
  MbCartPoint pnt;
  MbItem * curve = nullptr;
  double radius = 20;

  while ( GetPoint(IDS_SELECT_PNT_ON_CIRCLE, pnt) &&
          GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &curve ) &&
          GetDouble(IDS_ENTER_RADIUS, radius) ) 
  {
    // Точка и кривая в разных системах координат
    SPtr<const MbCurve> copy( GetCurveCopyOnWindPlace(*viewManager, *curve) );

    PArray<MbTempCircle> tempCircle( 0, 1, true );
    CircleCentreOnCurveRadPointOn( *copy, radius, pnt, tempCircle );

    PArray<MbArc> pCircle( 0, 2, false );
    CreateNewCircles( tempCircle, pCircle ); 

    for ( size_t i = 0; i < pCircle.Count(); i++ )
      viewManager->AddObject(CIRCLE_Style, pCircle[i] );
  }
}

//----------------------------------------------------------------------------------------
// Построить окружность по двум точкам, центр которой лежит на заданной кривой.
// Construct a circle by two points, with a center lying on a given curve.
// ---
void CreateCircleCentreOnCurveTwoPoints() 
{
  MbCartPoint pnt[2];
  MbItem *curve;

  while ( GetPoint(IDS_SELECT_PNT_ON_CIRCLE, pnt[0]) &&
          GetPoint(IDS_SELECT_PNT_ON_CIRCLE, pnt[1]) &&
          GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &curve ) ) 
  {
    // Точки и кривая в разных системах координат
    SPtr<const MbCurve> copy( GetCurveCopyOnWindPlace(*viewManager, *curve) );
    PArray<MbTempCircle> tempCircle( 0, 1, true );

    CircleCentreOnCurveTwoPoints( *copy,  pnt[0], pnt[1], tempCircle );
    PArray <MbArc> pCircle( 0, 2, false );
    CreateNewCircles( tempCircle, pCircle ); 
    for ( size_t i = 0; i < pCircle.Count(); ++i )
    {
      viewManager->AddObject(CIRCLE_Style, pCircle[i] );
    }
  }
}


//----------------------------------------------------------------------------------------
// Построить окружности с заданным центром, касающиеся заданной кривой.
// Create circles with given center,that touches given curve.
// ---
void CreateCircleCentreTanCurve() 
{
  MbCartPoint pnt;
  MbItem * curve = nullptr;

  while ( GetPoint(IDS_SELECT_CENTER, pnt) &&
          GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &curve ) ) 
  {
    // Точка и кривая в разных системах координат
    SPtr<const MbCurve> copy( GetCurveCopyOnWindPlace(*viewManager, *curve) );

    PArray<MbTempCircle> tempCircle( 0, 1, true );
    CircleTanCurveCentre( *copy, pnt, tempCircle );
    
    PArray <MbArc> pCircle( 0, 2, false );
    CreateNewCircles( tempCircle, pCircle ); 

    for ( size_t i = 0; i < pCircle.Count(); i++ )
      viewManager->AddObject(CIRCLE_Style, pCircle[i] );
  }
}


//----------------------------------------------------------------------------------------
// Построить окружности, касающиеся заданной кривой, с заданным радиусом, проходящие через заданную точку.
// Create circles that touch given curve and with a given radius, passing through a given point.
// ---
void CreateCircleTangentCurveTwoPoints() 
{
  MbCartPoint pnt[2];
  MbItem * curve = nullptr;

  while ( GetPoint(IDS_SELECT_PNT_ON_CIRCLE, pnt[0]) &&
          GetPoint(IDS_SELECT_PNT_ON_CIRCLE, pnt[1]) &&
          GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &curve ) ) 
  {
    // Точки и кривая в разных системах координат
    SPtr<const MbCurve> copy( GetCurveCopyOnWindPlace(*viewManager, *curve) );

    PArray<MbTempCircle> tempCircle( 0, 1, true );
    CircleTangentCurveTwoPoints( *copy, pnt[0], pnt[1], tempCircle );

    PArray <MbArc> pCircle( 0, 2, false );
    CreateNewCircles( tempCircle, pCircle ); 

    for ( size_t i = 0; i < pCircle.Count(); i++ )
      viewManager->AddObject(CIRCLE_Style, pCircle[i] );
  }
}


//----------------------------------------------------------------------------------------
// Построить окружности с заданным радиусом, касающиеся двух кривых.
// Creates circles with a given radius that touch two curves.
// ---
void CreateCircleTangentCurveRPointOn() 
{
  MbCartPoint pnt;
  MbItem * curve = nullptr;
  double radius = 20;

  while ( GetDouble(IDS_ENTER_RADIUS, radius) &&
          GetPoint(IDS_SELECT_PNT_ON_CIRCLE, pnt) &&
          GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &curve ) ) 
  {
    // Точка и кривая в разных системах координат
    SPtr<const MbCurve> copy( GetCurveCopyOnWindPlace(*viewManager, *curve) );

    PArray<MbTempCircle> tempCircle( 0, 1, true );
    CircleTangentCurveRPointOn( *copy, radius, pnt, tempCircle );

    PArray <MbArc> pCircle( 0, 2, false );
    CreateNewCircles( tempCircle, pCircle ); 

    for ( size_t i = 0; i < pCircle.Count(); ++i )
    {
      viewManager->AddObject(CIRCLE_Style, pCircle[i] );
    }
  }
}

//----------------------------------------------------------------------------------------
// Построить окружности с заданным радиусом, касающиеся двух кривых.
// Create circles with a given radius that touch two curves.
// ---
void CreateCircleTanTwoCurvesRadius() 
{
  MbItem * curve[2] = { nullptr, nullptr };
  double radius = 20;

  while ( GetDouble(IDS_ENTER_RADIUS, radius) &&
          GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &curve[0] ) &&
          GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &curve[1] ) ) 
  {
    // Кривые могут быть в разных системах координат.
    SPtr<const MbCurve> copy0( GetCurveCopyOnWindPlace(*viewManager, *curve[0]) );
    SPtr<const MbCurve> copy1( GetCurveCopyOnWindPlace(*viewManager, *curve[1]) );

    PArray<MbTempCircle> tempCircle( 0, 1, true );
    CircleTanTwoCurvesRadius( *copy0, *copy1, radius, tempCircle );

    PArray <MbArc> pCircle( 0, 2, false );
    CreateNewCircles( tempCircle, pCircle ); 

    for ( size_t i = 0; i < pCircle.Count(); i++ )
      viewManager->AddObject(CIRCLE_Style, pCircle[i] );
  }
}

//----------------------------------------------------------------------------------------
// Построить окружности с центром на первой кривой, касательные ко второй кривой, проходящие через заданную точку.
// Create circles with center on the first curve that touches second curve and passes through the given point.
// ---
void CreateCircleTanTwoCurvesPointOn() 
{
  MbCartPoint pnt;
  MbItem * curve[2] = { nullptr, nullptr };

  while ( GetPoint(IDS_SELECT_PNT_ON_CIRCLE, pnt) &&
          GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &curve[0] ) &&
          GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &curve[1] ) ) 
  {
    // Кривые и точка могут быть в разных системах координат.
    // Curves and a point can be in different coordinate systems.
    SPtr<const MbCurve> copy0( GetCurveCopyOnWindPlace(*viewManager, *curve[0]) );
    SPtr<const MbCurve> copy1( GetCurveCopyOnWindPlace(*viewManager, *curve[1]) );

    PArray<MbTempCircle> tempCircle( 0, 1, true );
    CircleTanTwoCurvesPointOn( *copy0, *copy1, pnt, tempCircle );

    PArray <MbArc> pCircle( 0, 2, false );
    CreateNewCircles( tempCircle, pCircle ); 

    for ( size_t i = 0; i < pCircle.Count(); i++ )
      viewManager->AddObject(CIRCLE_Style, pCircle[i] );
  }
}

//----------------------------------------------------------------------------------------
// Построить окружности, касающиеся трех кривых.
// Create circles that touch three curves.
// ---
void CreateCircleTanThreeCurves() 
{
  static_array<MbItem*,3> curve( (MbItem*)nullptr );

  while ( GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &curve[0] ) &&
          GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &curve[1] ) &&
          GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &curve[2] ) ) 
  {
    const MbPlaneItem * pItem0 = ((MbPlaneInstance*)curve[0])->GetPlaneItem();
    const MbPlaneItem * pItem1 = ((MbPlaneInstance*)curve[1])->GetPlaneItem();
    const MbPlaneItem * pItem2 = ((MbPlaneInstance*)curve[2])->GetPlaneItem();
    if ( (pItem0 != nullptr) && (pItem1 != nullptr) && (pItem2 != nullptr) &&
         (pItem0->Family() == pt_Curve) && (pItem1->Family() == pt_Curve) && (pItem2->Family() == pt_Curve) ) 
    {
      MbCartPoint pnt;
      if ( !GetPoint(IDS_SELECT_POINT, pnt) ) break;

      // Кривые и точка могут быть в разных системах координат.
      // Curves and a point can be in different coordinate systems.
      SPtr<const MbCurve> copy0( GetCurveCopyOnWindPlace(*viewManager, *curve[0]) );
      SPtr<const MbCurve> copy1( GetCurveCopyOnWindPlace(*viewManager, *curve[1]) );
      SPtr<const MbCurve> copy2( GetCurveCopyOnWindPlace(*viewManager, *curve[2]) );

      PArray<MbTempCircle> tempCircle( 0, 1, true );
      CircleTanThreeCurves( copy0, copy1, copy2, pnt, tempCircle );

      PArray <MbArc> pCircle( 0, 2, false );
      CreateNewCircles( tempCircle, pCircle ); 

      for ( size_t i = 0; i < pCircle.Count(); i++ )
        viewManager->AddObject(CIRCLE_Style, pCircle[i] );
    }
  }
}

//----------------------------------------------------------------------------------------
// Построить две окружности по радиусу и двум точкам
// Create two circles by radius and two points
// ---
void CreateCircleRad2Points() 
{
  MbCartPoint pnt[2];
  double radius = 20;

  while ( GetPoint(IDS_SELECT_FIRST_POINT, pnt[0]) &&
          GetPoint(IDS_SELECT_SECOND_POINT, pnt[1]) &&
          GetDouble( IDS_ENTER_RADIUS, radius ) ) 
  {
    viewManager->AddObject( CIRCLE_Style, new MbArc( pnt[0], radius ) );
    viewManager->AddObject( CIRCLE_Style, new MbArc( pnt[1], radius ) );
  }
}

//----------------------------------------------------------------------------------------
// Построить окружность по трем точкам.
// Create a circle by three points.
// ---
void CreateCircle3Points() 
{
  MbCartPoint pnt[3];
  MbCartPoint centre;

  while ( GetPoint(IDS_SELECT_FIRST_POINT, pnt[0]) &&
          GetPoint(IDS_SELECT_SECOND_POINT, pnt[1]) &&
          GetPoint(IDS_SELECT_THIRD_POINT, pnt[2]) ) {

    if ( ::CircleCentre3Points( pnt[0], pnt[1], pnt[2], centre ) ) {
      viewManager->AddObject( CIRCLE_Style, new MbArc( centre, centre.DistanceToPoint( pnt[0] ) ) );
    }
  }
}

//----------------------------------------------------------------------------------------
// Построить дугу окружности по двум точкам, радиусу и направлению.
// Construct a circle arc by two points, radius and direction.
// ---
void CreateArc2PointsRadius() 
{
  MbCartPoint pnt[2];
  MbArc  arcTmp[2];
  double radius = 20;

  while ( GetPoint(IDS_SELECT_FIRST_POINT, pnt[0]) &&
          GetPoint(IDS_SELECT_SECOND_POINT, pnt[1]) &&
          GetDouble( IDS_ENTER_RADIUS, radius ) ) {

    int result = Arc2PointsRadius( pnt[0], pnt[1], radius, 1, arcTmp );

    for ( ptrdiff_t i = 0; i < (ptrdiff_t)result; i++ )
      viewManager->AddObject( ARC_Style, new MbArc( arcTmp[i] ) );
  }
}


//----------------------------------------------------------------------------------------
// Построить дугу окружности по трем точкам.
// Create a circular arc by three points.
// ---
void CreateArc3Points() 
{
  MbCartPoint pnt[3];

  while ( GetPoint(IDS_SELECT_FIRST_POINT, pnt[0]) &&
          GetPoint(IDS_SELECT_SECOND_POINT, pnt[1]) &&
          GetPoint(IDS_SELECT_THIRD_POINT, pnt[2]) ) 
  {
    // Первая и третья точки - концы дуги.
    // The first and third points are ens of the arc.
    MbArc * arc = MbArc::Create( pnt[0], pnt[1], pnt[2] );
    if ( arc != nullptr )
      viewManager->AddObject( ARC_Style, arc );
  }
}

//----------------------------------------------------------------------------------------
// Построить дуги окружностей по двум точкам, касающиеся заданной кривой.
// Create arcs of circles by two points that touches given curve.
// ---
void CreateArcTangentCurveTwoPoints()
{
  MbCartPoint pnt[3];
  MbItem * curve = nullptr;

  while ( GetPoint(IDS_SELECT_PNT_ON_CIRCLE, pnt[0]) &&
          GetPoint(IDS_SELECT_PNT_ON_CIRCLE, pnt[1]) &&
          GetPoint(IDS_SELECT_OBJ_ON_PLANE, pnt[2]) &&
         (FindNearestObj( pnt[2], pt_Curve, &curve )) ) 
  {
    SPtr<const MbCurve> copy( GetCurveCopyOnWindPlace(*viewManager, *curve) );
    PArray<MbArc> pArc( 0, 2, false );
    ArcTangentCurveTwoPoints( *copy, pnt[0], pnt[1], pArc );
    for ( size_t i = 0; i < pArc.Count(); i++ )
      viewManager->AddObject(ARC_Style, pArc[i] );
  }
}


//----------------------------------------------------------------------------------------
// Построить дуги окружностей по радиусу и точке, касающиеся заданной кривой.
// Create arcs of circles by radius and point that touch given curve.
// ---
void CreateArcTangentCurveRPointOn() 
{
  MbCartPoint pnt[2];
  MbItem *curve = nullptr;
  double radius = 20;

  while ( GetDouble(IDS_ENTER_RADIUS, radius) &&
          GetPoint(IDS_SELECT_PNT_ON_CIRCLE, pnt[0]) &&
          GetPoint(IDS_SELECT_OBJ_ON_PLANE, pnt[1]) &&
         (FindNearestObj( pnt[1], pt_Curve, &curve ))  )  
  {
    SPtr<const MbCurve> copy( GetCurveCopyOnWindPlace(*viewManager, *curve) );
    PArray <MbArc> pArc( 0, 2, false );
    ArcTangentCurveRPointOn( *copy, radius, pnt[0], pArc );

    for ( size_t i = 0; i < pArc.Count(); i++ )
      viewManager->AddObject(ARC_Style, pArc[i] );
  }
}

//----------------------------------------------------------------------------------------
// Создать дугу окружности по центру и паре точек, задающих концы.
// Create a circular arc by the center point and pair of points specifying the bounds.
/*
  Создается дуга окружности с указанным центром. Другая пара точек определяют границы дуги.
  Начальная точка дуги лежит на луче, выходящем из центра окружности и проходящем через первую точку.
  Конечная точка - на луче проходит через вторую точку. Целочисленный параметр определяет направление дуги.

  A circular arc is created with pointed center. Other points specify the bounds of arc.
  The start point of the arc lies on the ray starting from the circle center and passing through the first point.
  The end point is on the ray passing through the second point. The integer parameter specifies the arc orientation.
*/
// ---
bool CreateArcCentre2Points() 
{
  MbCartPoint pnt[3];
  int sense = 1;

  if ( GetPoint( IDS_SELECT_ARC_CENTER, pnt[0] ) &&
       GetPoint( IDS_SELECT_FIRST_POINT, pnt[1] ) &&
       GetPoint( IDS_SELECT_SECOND_POINT, pnt[2] ) &&
       GetInt( IDS_ENTER_SENSE, sense ) )  
  {
    viewManager->AddObject( ARC_Style, new MbArc( pnt[0], pnt[1], pnt[2], sense ) );
    return true;
  }

  return false;
}

//----------------------------------------------------------------------------------------
// Создать дугу на основе данной окружности и данных граничных точек.
// Create an arc based on given circle with specified bounding points.
// ---
bool CreateArcCircle2Points() 
{
  MbItem * obj = nullptr;
  MbCartPoint pnt[3];
  int sense = 1;

  if (  GetMObj( IDS_SELECT_CRCL_ON_PLANE, pt_Arc, &obj ) &&
        GetPoint( IDS_SELECT_FIRST_POINT, pnt[1] ) &&
        GetPoint( IDS_SELECT_SECOND_POINT, pnt[2] ) &&
        GetInt( IDS_SELECT_EDIT_ELEMENT, sense ) )  
  {
    sense = sense > 0 ? 1 : -1;
    SPtr<const MbCurve> copy( GetCurveCopyOnWindPlace(*viewManager, *obj) );
    viewManager->AddObject(PPOINT_Style, new MbArc(static_cast<const MbArc &>(*copy), pnt[0], pnt[1], sense) );

    return true;
  }

  return false;
}

// eof