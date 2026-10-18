////////////////////////////////////////////////////////////////////////////////
//
// Примеры создания двумерных точек.
// Examples of creation of points on a plane.
//
////////////////////////////////////////////////////////////////////////////////
#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <mb_cross_point.h>
#include <action_point.h>
#include <plane_instance.h>
#include <plane_item.h>
#include <region.h>
#include <last.h>

using namespace c3d;
using namespace TestVariables;


//------------------------------------------------------------------------------
// Новая точка по координатам
// New point by coordinates
// ---
bool CreateSimplePoint()
{
  MbCartPoint pnt;
  if ( GetPoint( IDS_SELECT_POINT, pnt ) ) {
    viewManager->AddObject( PPOINT_Style, /*new MbPoint( */pnt  );
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// Точка как пересечение двух элементов
// A point as an intersection of two elements
// ---
bool CreateCrossPoint()
{
  MbItem *obj[2];
  MbCartPoint pnt;

  if ( GetPoint( IDS_SELECT_CRV_ON_PLANE, pnt ) &&
      (FindNearestObj( pnt, pt_Curve, &obj[0] )) &&
        GetPoint( IDS_SELECT_CRV_ON_PLANE, pnt ) &&
      (FindNearestObj( pnt, pt_Curve, &obj[1] )) ) {

    SArray <MbCrossPoint> crossPnt;
    const MbCurve *curves[2];

    curves[0] = static_cast<const MbCurve *>( ((MbPlaneInstance*)obj[0])->GetPlaneItem() );
    curves[1] = static_cast<const MbCurve *>( ((MbPlaneInstance*)obj[1])->GetPlaneItem() );

    viewManager->ToggleObject(obj[0]);
    viewManager->ToggleObject(obj[1]);

    if ( curves[0] != nullptr && curves[1] != nullptr )
      ::IntersectTwoCurves( *curves[0], *curves[1], crossPnt, true );

    viewManager->ToggleObject(obj[0]);
    viewManager->ToggleObject(obj[1]);

    double dmin = MB_MAXDOUBLE;
    size_t imin = SYS_MAX_T/*OV_x64 -1*/;
    size_t result = crossPnt.Count();

    for (size_t i = 0; i < result; i++) {
      double d = crossPnt[i].p.DistanceToPoint( pnt );

      if ( d < dmin ) {
        dmin = d;
        imin = i;
      }
    }

    MbPlacement3D place( ((MbPlaneInstance *)obj[0])->GetPlacement() );
    if ( imin != SYS_MAX_T )
      viewManager->AddObject( PPOINT_Style, /*new MbPoint( */crossPnt[imin].p , &place );

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Точка на кривой
// A point on a curve
// ---
bool CreatePointOnCurve()
{
  MbItem * obj( nullptr );
  MbCartPoint3D pnt3d;

  if ( GetPoint( IDS_SELECT_CRV_ON_PLANE, pnt3d ) &&
       (FindNearestObj( pnt3d, pt_Curve, &obj )) &&
       (obj->IsA() ==  st_PlaneInstance) )  {

    const MbCurve * curve = static_cast<const MbCurve *>( ((MbPlaneInstance*)obj)->GetPlaneItem() );
    MbPlacement3D place( ((MbPlaneInstance *)obj)->GetPlacement() );

    WorkWindow * currentView3D = viewManager->GetActiveWindow();

    MbCartPoint pnt; // Проекция на плоскость кривой в направлении взгляда
    place.DirectPointProjection( pnt3d, currentView3D->GetAxisZ(), pnt.x, pnt.y );

    double t = curve->PointProjection( pnt );  
    MbCartPoint pp, pq;
    curve->PointOn( t, pp );
    MbVector norm;
    curve->Normal( t, norm );
    MbDirection dir (norm.x, norm.y );
    bool res = curve->DirectPointProjection( pp, dir, pq );
    if ( res )
      viewManager->AddObject(PPOINT_Style, pq, &place );
    else
      viewManager->AddObject(PPOINT_Style, pp, &place );

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Много точек на кривой
// A set of points on a curve with equal distance along the curve between
// ---
bool CreateNPointsOnCurve() 
{
  MbItem *nearest;
  MbCartPoint pnt;
  std::vector<MbCartPoint> pnts;
  MbPlacement3D place;

  if ( FindNearestObj( pnt, pt_Curve, &nearest ) ) {
    viewManager->ToggleObject(nearest);

    if ( st_PlaneInstance == nearest->IsA() &&
         GetInt( IDS_ENTER_QUANTITY, TestVariables::count ) ) {
      place.Init( ((const MbPlaneInstance *)nearest)->GetPlacement() );
      const MbCurve *curve = static_cast<const MbCurve *>( ((MbPlaneInstance*)nearest)->GetPlaneItem() );
      curve->GetPointsByEvenLengthDelta( TestVariables::count, pnts );
    }
    viewManager->ToggleObject(nearest);
  }

  for( size_t i = 0; i < pnts.size(); i++ ) {
    viewManager->AddObject( POINT_Style, pnts[i], &place );
  }
  return pnts.size() > 0;
/*
  MbItem *obj;
  MbCartPoint pnt;
  ptrdiff_t count = 5; 
  int dir = 1;
  double length = 10;

  if (  GetPoint( IDS_SELECT_CRV_ON_PLANE, pnt ) &&
       (FindNearestObj( pnt, pt_Curve, &obj )) &&
        GetDouble( IDS_ENTER_DISTANCE, length ) &&
        GetInt( IDS_ENTER_POINT_NUMBER, count ) &&
        GetInt( IDS_ENTER_SENSE, dir ) ) {

    const MbCurve *curve = static_cast<const MbCurve *>( ((const MbPlaneInstance*)obj)->GetPlaneItem() );
    MbPlacement3D place( ((MbPlaneInstance *)obj)->GetPlacement() );

    double t = curve->PointProjection( pnt );
    curve->PointOn( t, pnt );
    viewManager->AddObject( PPOINT_Style, pnt , &place );

    for ( size_t i = 0; i < count; i++ ) {
      if ( !curve->DistanceAlong( t, length, dir ) )
        break;

      curve->PointOn( t, pnt );
      viewManager->AddObject( PPOINT_Style, pnt , &place );

    }
    return true;
  }
  return false;
*/
}


//------------------------------------------------------------------------------
// Много точек на кривой по параметру
// A set of points on a curve with the equal curve parameter intervals between
// ---
bool CreatePointsOnParamCurve() 
{
  MbItem *nearest;
  MbCartPoint pnt;
  std::vector<MbCartPoint> pnts;
  MbPlacement3D place;

  if ( GetPoint( IDS_SELECT_CRV_ON_PLANE, pnt ) &&
       FindNearestObj( pnt, pt_Curve, &nearest ) ) {
    viewManager->ToggleObject(nearest);

    if ( st_PlaneInstance == nearest->IsA() &&
         GetInt( IDS_ENTER_QUANTITY, TestVariables::count ) ) {
      place.Init( ((MbPlaneInstance *)nearest)->GetPlacement() );
      const MbCurve *curve = static_cast<const MbCurve *>( ((MbPlaneInstance*)nearest)->GetPlaneItem() );
      curve->GetPointsByEvenParamDelta( TestVariables::count, pnts );
    }
    viewManager->ToggleObject(nearest);
  }

  for( size_t i = 0; i < pnts.size(); i++ ) {
    viewManager->AddObject( POINT_Style, pnts[i], &place );
  }
  return pnts.size() > 0;
/*
  MbItem *obj;
  MbCartPoint pnt;
  ptrdiff_t count = 5;

  if (  GetPoint( IDS_SELECT_CRV_ON_PLANE, pnt ) &&
        (FindNearestObj( pnt, pt_Curve, &obj )) &&
        GetInt( IDS_ENTER_POINT_NUMBER, count ) ) {

    MbCurve * curve = static_cast<MbCurve *>( ((MbPlaneInstance*)obj)->SetPlaneItem() );
    MbPlacement3D place( ((MbPlaneInstance *)obj)->GetPlacement() );

    ptrdiff_t n = curve->IsClosed()? count : count - 1;
    double step = (curve->GetTMax()-curve->GetTMin())/ (double)n;
    for ( size_t i = 0; i < count; i++ ) {
      double t = (double)i * step + curve->GetTMin();
      curve->PointOn( t, pnt );
      viewManager->AddObject( PPOINT_Style, pnt , &place );
    }
    return true;
  }

  return false;
*/
}


//------------------------------------------------------------------------------
// Выдать центр тяжести кривой
// Calculate a center of a curve
// ---
bool CreateCentrePoint() {
  MbItem *gobj;
  MbCartPoint pnt;

  if (  GetPoint( IDS_SELECT_CRV_ON_PLANE, pnt ) &&
       (FindNearestObj( pnt, pt_Curve, &gobj )) )  {

    MbCurve *curve = static_cast<MbCurve *>( ((MbPlaneInstance*)gobj)->SetPlaneItem() );
    MbPlacement3D place( ((MbPlaneInstance *)gobj)->GetPlacement() );

    if ( curve->GetCentre( pnt ) ) {
      viewManager->AddObject( PPOINT_Style, /*new MbPoint( */pnt , &place );

      return true;
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Пересечение двух выбранных элементов
// Intersection points of two selected elements
// ---
bool IntersectCurves() {
  MbCartPoint pnt, p1, p2 ;
  MbItem *gcrv1, *gcrv2;

  if ( GetMObj( IDS_SELECT_FIRST_CURVE, pt_Curve, &gcrv1 ) &&
       GetMObj( IDS_SELECT_SECOND_CURVE, pt_Curve, &gcrv2 ) ) {
//  if (  GetPoint( IDS_SELECT_FIRST_CURVE, pnt ) &&
//        FindNearestObj( pnt, pt_Curve, &gcrv1 ) &&
//        GetPoint( IDS_SELECT_SECOND_CURVE, pnt ) &&
//        FindNearestObj( pnt, pt_Curve, &gcrv2 ) ) {

    SArray <MbCrossPoint> crossPnt;

    viewManager->ToggleObject(gcrv1);
    viewManager->ToggleObject(gcrv2);

    SPtr<const MbCurve> crv1, crv2;

    if ( gcrv1->IsA() == st_PlaneInstance ) {
      const MbPlaneItem * pItem = (static_cast<const MbPlaneInstance *>(gcrv1))->GetPlaneItem();
      if ( pItem->Family() == pt_Curve )
        crv1 = static_cast<const MbCurve *>( pItem );
      else if ( pItem->Family() == pt_Region )
        crv1 = (static_cast<const MbRegion *>( pItem ))->GetOutContour();
    }
    if ( gcrv2->IsA() == st_PlaneInstance ) {
      const MbPlaneItem * pItem = (static_cast<const MbPlaneInstance *>(gcrv2))->GetPlaneItem();
      if ( pItem->Family() == pt_Curve )
        crv2 = static_cast<const MbCurve *>( pItem );
      else if ( pItem->Family() == pt_Region )
        crv2 = (static_cast<const MbRegion *>( pItem ))->GetOutContour();
    }

    if ( crv1 != nullptr && crv2 != nullptr ) {
      MbPlacement3D place1( ((MbPlaneInstance *)gcrv1)->GetPlacement() );
      MbPlacement3D place2( ((MbPlaneInstance *)gcrv2)->GetPlacement() );
      MbMatrix matr;
      place2.GetMatrixToPlace( place1, matr );

      if ( !matr.IsSingle() ) {
        crv2 = static_cast<MbCurve *>(&crv2->Duplicate());
        (const_cast<MbCurve &>(*crv2)).Transform( matr );
      }

      bool touchInclude = true; // false;
      ::IntersectTwoCurves( *crv1, *crv2, crossPnt, touchInclude );

      if ( !touchInclude ) // KOMPAS-15672
        ::RemoveAllTouchParams( *crv1, *crv2, crossPnt );
    }
    

    viewManager->ToggleObject(gcrv1);
    viewManager->ToggleObject(gcrv2);

    ptrdiff_t result = crossPnt.Count();
    MbPlacement3D place( ((MbPlaneInstance *)gcrv1)->GetPlacement() );

    for ( ptrdiff_t i = 0; i < result; i++ ) {
      crossPnt[i].on1.curve->PointOn( crossPnt[i].on1.t, p1 );
      crossPnt[i].on2.curve->PointOn( crossPnt[i].on2.t, p2 );
      pnt = (p1+p2)*0.5;

      viewManager->AddObject( crossPnt[i].IsTangent() ? TANGENT_Style : PPOINT_Style, pnt , &place );
    }

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Самопересечение произвольной кривой
// Selfintersection points of a curve
// ---
bool SelfIntersectCurve()
{
  bool res = false;
  MbCartPoint pnt;
  MbItem * item = nullptr;

  if ( GetPoint( IDS_SELECT_CRV_ON_PLANE, pnt ) && FindNearestObj( pnt, pt_Curve, &item ) )
  {
    const MbPlaneInstance * pInst = static_cast<const MbPlaneInstance *>( item );
    MbPlacement3D itemPlace( pInst->GetPlacement() );

    const MbPlaneItem * pItem = pInst->GetPlaneItem();

    std::vector<const MbCurve *> pCurves;
    pCurves.reserve( 2 );

    if ( pItem->Family() == pt_Curve ) {
      const MbCurve * pCurve = static_cast<const MbCurve *>(pItem);
      pCurves.push_back( pCurve );
    }
    else if ( pItem->Family() == pt_Region ) {
      const MbRegion * pRegion = static_cast<const MbRegion *>(pItem);
      size_t boundsCnt = pRegion->GetContoursCount();
      for ( size_t k = 0; k < boundsCnt; ++k ) {
        pCurves.push_back( pRegion->GetContour( k ) );
      }
    }

    SArray<MbCrossPoint> crossPnts;
    size_t curvesCnt = pCurves.size();
    for ( size_t k = 0; k < curvesCnt; ++k ) {
      crossPnts.clear();
      pCurves[k]->SelfIntersect( crossPnts );
      size_t crossCnt = crossPnts.size();
      if ( crossCnt > 0 )
        res = true;
      for ( size_t i = 0; i < crossCnt; ++i ) {
        crossPnts[i].on1.curve->PointOn( crossPnts[i].on1.t, pnt );
        bool showItem = (i == crossCnt-1);
        Style st( crossPnts[i].IsTangent() ? TestVariables::TANGENT_Style : TestVariables::PPOINT_Style );
        TestVariables::viewManager->AddObject( st, pnt, &itemPlace, showItem );
      }
    }
  }
  return res;
}


//------------------------------------------------------------------------------
// Разбить кривые точками пересечения.
// Truncate curves by intersection points.
// ---
bool TruncateCurvesByIntersection()
{
  MbCartPoint pnt, p1, p2 ;
  MbItem *gcrv1, *gcrv2;

  if ( GetMObj( IDS_SELECT_FIRST_CURVE, pt_Curve, &gcrv1 ) &&
       GetMObj( IDS_SELECT_SECOND_CURVE, pt_Curve, &gcrv2 ) ) {
//  if (  GetPoint( IDS_SELECT_FIRST_CURVE, pnt ) &&
//        FindNearestObj( pnt, pt_Curve, &gcrv1 ) &&
//        GetPoint( IDS_SELECT_SECOND_CURVE, pnt ) &&
//        FindNearestObj( pnt, pt_Curve, &gcrv2 ) ) {

    SArray <MbCrossPoint> crossPnt;

    viewManager->ToggleObject(gcrv1);
    viewManager->ToggleObject(gcrv2);

    SPtr<const MbCurve> crv1, crv2;

    if ( gcrv1->IsA() == st_PlaneInstance ) {
      const MbPlaneItem * pItem = (static_cast<const MbPlaneInstance *>(gcrv1))->GetPlaneItem();
      if ( pItem->Family() == pt_Curve )
        crv1 = static_cast<const MbCurve *>( pItem );
      else if ( pItem->Family() == pt_Region )
        crv1 = (static_cast<const MbRegion *>( pItem ))->GetOutContour();
    }
    if ( gcrv2->IsA() == st_PlaneInstance ) {
      const MbPlaneItem * pItem = (static_cast<const MbPlaneInstance *>(gcrv2))->GetPlaneItem();
      if ( pItem->Family() == pt_Curve )
        crv2 = static_cast<const MbCurve *>( pItem );
      else if ( pItem->Family() == pt_Region )
        crv2 = (static_cast<const MbRegion *>( pItem ))->GetOutContour();
    }

    if ( crv1 != nullptr && crv2 != nullptr ) {
      MbPlacement3D place1( ((MbPlaneInstance *)gcrv1)->GetPlacement() );
      MbPlacement3D place2( ((MbPlaneInstance *)gcrv2)->GetPlacement() );
      MbMatrix matr;
      place2.GetMatrixToPlace( place1, matr );

      if ( !matr.IsSingle() ) {
        crv2 = static_cast<MbCurve *>(&crv2->Duplicate());
        (const_cast<MbCurve &>(*crv2)).Transform( matr );
      }

      bool touchInclude = true; // false;
      ::IntersectTwoCurves( *crv1, *crv2, crossPnt, touchInclude );

      if ( !touchInclude ) // KOMPAS-15672
        ::RemoveAllTouchParams( *crv1, *crv2, crossPnt );
    }

    viewManager->ToggleObject(gcrv1);
    viewManager->ToggleObject(gcrv2);

    ptrdiff_t result = crossPnt.Count();
    MbPlacement3D place( ((MbPlaneInstance *)gcrv1)->GetPlacement() );
    double t1_beg = crv1->GetTMin();
    double t2_beg = crv2->GetTMin();
    Style st1( gcrv1->GetWidth(), gcrv1->GetColor() );
    Style st2( gcrv2->GetWidth(), gcrv2->GetColor() );
    bool del1 = false, del2 = false;

    for ( ptrdiff_t i = 0; i < result; i++ ) {
      double t1_end = crossPnt[i].on1.t;
      double t2_end = crossPnt[i].on2.t;
      MbCurve * c1 = crv1->Trimmed( t1_beg, t1_end, 1 );
      MbCurve * c2 = crv2->Trimmed( t2_beg, t2_end, 1 );

      if ( c1 != nullptr ) {
        viewManager->AddObject( st1, c1, &place );
        del1 = true;
      }
      if ( c2 != nullptr ) {
        viewManager->AddObject( st2, c2, &place );
        del2 = true;
      }

      t1_beg = t1_end;
      t2_beg = t2_end;
    }
    double t1_end = crv1->GetTMax();
    double t2_end = crv2->GetTMax();
    MbCurve * c1 = crv1->Trimmed( t1_beg, t1_end, 1 );
    MbCurve * c2 = crv2->Trimmed( t2_beg, t2_end, 1 );

    if ( c1 != nullptr ) {
      viewManager->AddObject( st1, c1, &place );
      del1 = true;
    }
    if ( c2 != nullptr ) {
      viewManager->AddObject( st2, c2, &place );
      del2 = true;
    }

    if ( del1 )
      viewManager->DeleteObject( gcrv1 );
    if ( del2 )
      viewManager->DeleteObject( gcrv2 );

    return ( del1 || del2 );
  }

  return false;
}
