#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <test_draw.h>
#include <cur_line_segment.h>
#include <cur_polyline.h>
#include <cur_bezier.h>
#include <cur_contour.h>
#include <cur_arc.h>
#include <cur_nurbs.h>
#include <alg_curve_delete_part.h>
#include <alg_curve_envelope.h>
#include <alg_curve_fillet.h>
#include <plane_instance.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Скруглить кривые.
// Fillet curves.
// ---
bool FilletCurves() {
  MbCartPoint pnt2, pnt1;
  MbItem *obj1, *obj2;

  if (  GetPoint( IDS_SELECT_CRV_ON_PLANE, pnt1 ) &&
       (FindNearestObj( pnt1, pt_Curve, &obj1 )) ) {
    if (  GetPoint( IDS_SELECT_CRV_ON_PLANE, pnt2 ) &&
         (FindNearestObj( pnt2, pt_Curve, &obj2 )) ) {

       MbCurve *curve1 = static_cast<MbCurve *>( ((MbPlaneInstance *)obj1)->SetPlaneItem() );
       MbCurve *curve2 = static_cast<MbCurve *>( ((MbPlaneInstance *)obj2)->SetPlaneItem() );
      MbPlacement3D cp1( ((MbPlaneInstance *)obj1)->GetPlacement() ); // Дать плоскость. // Give a plane.
      MbPlacement3D cp2( ((MbPlaneInstance *)obj2)->GetPlacement() ); // Дать плоскость. // Give a plane.

      if ( GetDouble( IDS_ENTER_RADIUS, TestVariables::radius1 ) && cp1==cp2 ) {
        TestVariables::radius2 = TestVariables::radius1;
        bool trim = true;
        MbeState state1, state2;
        MbArc * seg = nullptr;

        if ( Fillet( curve1, pnt1, trim, curve2, pnt2, trim, TestVariables::radius1,
                     state1, state2, seg ) ) {
          TestVariables::viewManager->RefreshObjectModel( obj1 );
          TestVariables::viewManager->RefreshObjectModel( obj2 );

          if ( seg != nullptr ) // Появилась еще кривая.  // Appeared another curve.
            TestVariables::viewManager->AddObject( TestVariables::LINE_SEG_Style, seg, &cp1 );

          return true;
        }
      }
    }
  }
  return false;
}


//------------------------------------------------------------------------------
// Фаска на кривых.
// Chamfer on the curves.
// ---
bool ChamferCurves() {
  MbCartPoint pnt2, pnt1;
  MbItem *obj1, *obj2;

  if (  GetPoint( IDS_SELECT_CRV_ON_PLANE, pnt1 ) &&
       (FindNearestObj( pnt1, pt_Curve, &obj1 )) ) {
    if (  GetPoint( IDS_SELECT_CRV_ON_PLANE, pnt2 ) &&
         (FindNearestObj( pnt2, pt_Curve, &obj2 )) ) {

       MbCurve *curve1 = static_cast<MbCurve *>( ((MbPlaneInstance *)obj1)->SetPlaneItem() );
       MbCurve *curve2 = static_cast<MbCurve *>( ((MbPlaneInstance *)obj2)->SetPlaneItem() );
      MbPlacement3D cp1( ((MbPlaneInstance *)obj1)->GetPlacement() ); // Дать плоскость. // Give a plane.
      MbPlacement3D cp2( ((MbPlaneInstance *)obj2)->GetPlacement() ); // Дать плоскость. // Give a plane.

      if ( GetDouble( IDS_ENTER_FIRST_SRF_DST, TestVariables::distance1 ) && 
           cp1==cp2 ) {
        TestVariables::distance2 = TestVariables::distance1;
        if ( GetDouble( IDS_ENTER_FIRST_SRF_DST, TestVariables::distance2 ) ) {
          bool trim = true;
          MbeState state1, state2;
          MbLineSegment * seg = nullptr;
          bool type = false;

          if ( Chamfer( curve1, pnt1, trim, curve2, pnt2, trim, TestVariables::distance1, TestVariables::distance2, type,
                        state1, state2, seg ) ) {
            TestVariables::viewManager->RefreshObjectModel( obj1 );
            TestVariables::viewManager->RefreshObjectModel( obj2 );

            if ( seg != nullptr ) // Появилась еще кривая. // Appeared another curve.
              TestVariables::viewManager->AddObject( TestVariables::LINE_SEG_Style, seg, &cp1 );

            return true;
          }
        }
      }
    }
  }
  return false;
}


//------------------------------------------------------------------------------
// Удалить часть.
// Delete a part.
// ---
bool DelCrvPart() {
  MbCartPoint pnt;
  MbItem *obj;

  if ( GetPoint( IDS_SELECT_CRV_DEL_PART, pnt ) &&
      (FindNearestObj( pnt, pt_PlaneItem, &obj )) ) {
    List <MbCurve> curveList( false );
    RPArray <MbCurve> curveArray( 10,10 );
    MbPlacement3D place;
    obj->GetPlacement( place ); // Запоминаем ЛСК.
    TestVariables::viewManager->CreateCurveArray( obj, curveArray, false );
    for ( ptrdiff_t j = 0; j < (ptrdiff_t)curveArray.Count(); j++ )
      curveList.Add(curveArray[j]);

    // Нахождение ближайшей к точке кривой
    MbCurve * selectCurve = FindNearestCurve( curveList, pnt );

    if ( selectCurve != nullptr ) {
      MbCurve *part2;

      // Функция удаления части кривой по отношению к точке
      MbeState dp = DeleteCurvePart( curveList, pnt, selectCurve, part2 );

      obj = (MbItem *)TestVariables::viewManager->FindObject( selectCurve );

      if ( dp == dp_Degenerated ) // Кривая выродилась
        TestVariables::viewManager->DeleteObject( obj );  // Удалить элемент
      else
        TestVariables::viewManager->RefreshObjectModel( obj );

      if ( part2 != nullptr ) // Появилась еще кривая
        TestVariables::viewManager->AddObject( TestVariables::LINE_SEG_Style, part2, &place );

      return true;
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Удалить часть от точки p1 до точки p2.
// Delete part from the point p1 to the point p2.
// ---
bool DelCrvPartP1P2() {
  MbCartPoint3D pnt3D[3];
  MbItem *obj;

  if ( GetPoint( IDS_SELECT_FIRST_POINT, pnt3D[0] ) &&
       GetPoint( IDS_SELECT_SECOND_POINT, pnt3D[1] ) &&
       GetPoint( IDS_SELECT_CRV_ON_PLANE, pnt3D[2] ) &&
       (FindNearestObj( pnt3D[2], pt_Curve, &obj )) ) {

    MbPlaneInstance * planeInstance = static_cast<MbPlaneInstance *>( obj );
    Style style = planeInstance->GetStyle();
    MbCurve * curve = static_cast<MbCurve *>( planeInstance->SetPlaneItem() );
    
    if( curve )
    {          
      // Спроецировать на плоскость кривой в направлении взгляда
      MbCartPoint pnt[3];
      MbPlacement3D place;
      planeInstance->GetPlacement( place );

      WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();
      

      for ( size_t i = 0; i < 3; ++i )
        place.DirectPointProjection( pnt3D[i], currentView3D->GetAxisZ(), pnt[i].x, pnt[i].y );       
           
      // Функция удаления части кривой от точки p1 до точки p2
      MbCurve *part2; 
      MbeState dp = DeleteCurvePart( pnt[0], pnt[1], pnt[2], curve, part2 );

      if ( dp == dp_Degenerated ) // Кривая выродилась
        TestVariables::viewManager->DeleteObject( obj );  // Удалить элемент
      else
        TestVariables::viewManager->RefreshObjectModel( obj );

      if ( part2 != nullptr ) // Появилась еще кривая
        TestVariables::viewManager->AddObject( style, part2, &place );

      return true;
    }      
  }

  return false;
}


//------------------------------------------------------------------------------
// Вырезать часть кривой.
// Cut part of the curve.
// ---
bool CreateTrimmCurve() {
  MbCartPoint pnt;
  MbItem *obj;

  if ( GetPoint( IDS_SELECT_CRV_DEL_PART, pnt ) &&
      (FindNearestObj( pnt, pt_PlaneItem, &obj )) ) {
    List <MbCurve> curveList( false );
    RPArray <MbCurve> curveArray( 10,10 );
    MbPlacement3D place;
    obj->GetPlacement( place ); // Запоминаем ЛСК.
    TestVariables::viewManager->CreateCurveArray( obj, curveArray, false );
    for ( ptrdiff_t j = 0; j < (ptrdiff_t)curveArray.Count(); j++ )
      curveList.Add(curveArray[j]);

    // Нахождение ближайшей к точке кривой
    MbCurve * selectCurve = FindNearestCurve( curveList, pnt );

    if ( selectCurve != nullptr ) {
      MbCurve *part2;

      // Функция оставления части кривой по отношению к точке
      MbeState dp = TrimmCurvePart( curveList, pnt, selectCurve, part2 );

      obj = (MbItem *)TestVariables::viewManager->FindObject( selectCurve );

      if ( dp == dp_Degenerated ) // Кривая выродилась
        TestVariables::viewManager->DeleteObject( obj );  // Удалить элемент
      else
        TestVariables::viewManager->RefreshObjectModel( obj );

      if ( part2 != nullptr ) // Появилась еще кривая
        TestVariables::viewManager->AddObject( TestVariables::LINE_SEG_Style, part2, &place );

      return true;
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Разбить на две кривые.
// Break into two curves.
// ---
bool CreateBreakCurve() {
  MbCartPoint3D p;
  MbItem * obj;

  if ( GetMObj( IDS_SELECT_LINE_ON_PLANE, pt_Curve, &obj ) &&
       GetPoint( IDS_SELECT_POINT, p ) && (obj->IsA() == st_PlaneInstance) ) {
    MbCurve *curve = static_cast<MbCurve *>( static_cast<MbPlaneInstance *>(obj)->SetPlaneItem() );
    if ( curve!=nullptr ) {
      MbPlacement3D place( ((MbPlaneInstance *)obj)->GetPlacement() );
      MbCartPoint pnt;
      place.DirectPointProjection( p, TestVariables::viewManager->GetActiveWindow()->GetAxisZ(), pnt.x, pnt.y );
//      place.PointProjection( p, pnt.x, pnt.y );
      double t = curve->PointProjection( pnt );
      MbCurve *c1 = curve->Trimmed( curve->GetTMin(), t, 1 );
      MbCurve *c2 = curve->Trimmed( t, curve->GetTMax(), 1 );

      if ( c1 && c2 ) {
        Style st( obj->GetWidth(), obj->GetColor() );
        TestVariables::viewManager->DeleteObject( obj );

        TestVariables::viewManager->AddObject( st, c1, &place );
        TestVariables::viewManager->AddObject( st, c2, &place );
        return true;
      }
    }
  }
  return false;
}


//------------------------------------------------------------------------------
// Вставить узел.
// Insert a knot.
// ---
bool InsertSplineKnot() {

  MbCartPoint pnt;
  MbItem * obj;

  if (  GetPoint( IDS_SELECT_POINT, pnt ) &&
        GetMObj( IDS_SELECT_SPLN_ON_PLANE, pt_Nurbs, &obj ) && (obj->IsA() == st_PlaneInstance) ) {

    MbNurbs *curve = (MbNurbs *)((MbPlaneInstance *)obj)->GetPlaneItem();
    double t1 = curve->PointProjection( pnt );

    ptrdiff_t k = ((MbNurbs *)curve)->GetDegree();
    ptrdiff_t n = ((MbNurbs *)curve)->GetPointListCount();
    for ( ptrdiff_t j = k-1; j < n+k-1; j++ ) {
      double t = ((MbNurbs *)curve)->GetKnot(j);
      if ( fabs(t-t1) < .05 )   t1 = t;
    }

    curve->InsertKnots( t1, 1, PARAM_REGION );
    TestVariables::viewManager->RefreshObjectModel(obj);

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Построение усеченного сплайна.
// Construction of a truncated spline.
// ---
bool CreateInverseSpline()
{
  MbCartPoint pnt;
  MbItem * obj;

  if (  GetPoint( IDS_SELECT_CRV_ON_PLANE, pnt ) &&
       (FindNearestObj( pnt, pt_Curve, &obj )) && (obj->IsA() == st_PlaneInstance) ) {

    MbCurve * curve = static_cast<MbCurve *>( static_cast<MbPlaneInstance *>(obj)->SetPlaneItem() );

    curve->Inverse();
    TestVariables::viewManager->RefreshObjectModel(obj);

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Установить вес вершины.
// Set weight of the vertex.
// ---
void Splineset_Weights() {

  MbItem *p_nearest;
  MbCartPoint point;
  double weight = 1;

  while (  GetPoint(IDS_SELECT_SPLN_ON_PLANE, point ) &&
          ( FindNearestObj( point, pt_Nurbs, &p_nearest ) && (p_nearest->IsA() == st_PlaneInstance) ) ) {

    TestVariables::viewManager->ToggleObject(p_nearest);
    if ( GetPoint(IDS_SELECT_POINT, point) &&
         GetDouble(IDS_ENTER_POINT_WEIGHT, weight) ) {

      MbNurbs *nurbs = (MbNurbs *)((MbPlaneInstance *)p_nearest)->GetPlaneItem();
      ptrdiff_t index = nurbs->GetNearPointIndex( point );

      if ( index != -1 ) {
        nurbs->SetWeight( index, weight );
        TestVariables::viewManager->RefreshObjectModel(p_nearest);
      }
    }
  }

}


//------------------------------------------------------------------------------
// Новая вершина.
// A new vertex.
// ---
void Splinenew_Point() {
  MbItem * obj;
  MbCartPoint point;
  ptrdiff_t index = 1;

  while ( GetPoint(IDS_SELECT_POINT, point) &&
          GetMObj(IDS_SELECT_SPLN_ON_PLANE, pt_Curve, &obj ) &&
          GetInt(IDS_ENTER_POINT_INDEX, index) && (obj->IsA() == st_PlaneInstance) ) {

    MbPolyCurve *curve = ( MbPolyCurve *)((MbPlaneInstance *)obj)->GetPlaneItem();

    curve->InsertPoint( index, point ); // Добавить точку по индексу
    TestVariables::viewManager->RefreshObjectModel(obj);
  }
}



//------------------------------------------------------------------------------
// Замена точки.
// Change point.
// ---
void Splinechangepoint() {
  MbItem *p_nearest;
  MbCartPoint point;

  while (  GetPoint(IDS_SELECT_SPLN_ON_PLANE, point) &&
          ( FindNearestObj( point, pt_Curve, &p_nearest ) ) &&
           GetPoint( IDS_SELECT_NEAREST_POINT, point ) && (p_nearest->IsA() == st_PlaneInstance) ) {

    MbPolyCurve *curve = ( MbPolyCurve *)((MbPlaneInstance *)p_nearest)->GetPlaneItem();

    // Выдать индекс точки, ближайшей к заданной
    ptrdiff_t index = curve->GetNearPointIndex( point );

    if ( index != -1 && GetPoint( IDS_SELECT_NEW_PNT_PLC, point ) ) {
/*
      double t1, t2;
      curve->GetRuleInterval( index, t1, t2 ); // Выдать интервал влияния точки
*/
//              HidePhantom( curve, t1, t2 ); // Стереть фантом кривой
      curve->ChangePoint( index, point ); // Заменить точку
//              ShowPhantom( curve, t1, t2 ); // Нарисовать фантом кривой

      TestVariables::viewManager->RefreshObjectModel(p_nearest);
    }
  }
}


//------------------------------------------------------------------------------
// Замкнуть / разомкнуть кривую.
// Close / unclose a curve.
// ---
void SplineSetClosed() {
  MbItem * obj( nullptr );

  while ( GetMObj(IDS_SELECT_CRV_ON_PLANE, pt_Curve, &obj ) && (obj->IsA() == st_PlaneInstance) ) {
    MbCurve * curve = static_cast<MbCurve *>( static_cast<MbPlaneInstance *>(obj)->SetPlaneItem() );

    if ( curve->IsA() == pt_Nurbs  ||
         curve->IsA() == pt_Bezier ||
         curve->IsA() == pt_Hermit ||
         curve->IsA() == pt_CubicSpline ||
         curve->IsA() == pt_Polyline ) {
      bool cl = curve->IsClosed();
      // Установить признак замкнутости. // Set a sign of closure.
      static_cast<MbPolyCurve *>(curve)->SetClosed( !cl ); // Замкнуть / разомкнуть кривую.  // Close / unclose a curve.
      TestVariables::viewManager->RefreshObjectModel(obj);
    }  
  }
}


//------------------------------------------------------------------------------
// Изменить порядок двумерной кривой NURBS.
// Change degree of two-dimensional curve NURBS.
// ---
void SetNurbsDegree()
{
  MbItem * obj = nullptr;
  ptrdiff_t degree = 4;     //-V112

  while ( GetMObj(IDS_SELECT_SPLN_ON_PLANE, pt_Nurbs, &obj ) && GetInt( IDS_ENTER_SPLINE_ORDER, degree ) && 
          (obj->IsA() == st_PlaneInstance) ) {
    MbNurbs * curve = static_cast<MbNurbs *>((static_cast<MbPlaneInstance *>(obj))->SetPlaneItem());
    curve->SetDegree( degree ); // Установить порядок NURBS. // Set NURBS degree.
    TestVariables::viewManager->RefreshObjectModel(obj);
  }
}


//------------------------------------------------------------------------------
// Аппроксимация кривой дугами окружности или отрезком.
// Approximation of the curve by arcs or a segment.
// ---
void PolyArc( MbCurve *curve, double eps, PArray <MbCurve> &poly ) {

  ptrdiff_t n;
  double tmin = curve->GetTMin();
  double tmax = curve->GetTMax();
  double dt, dmax = tmax-tmin;
  double delta = 100*Math::NewtonReg;  // Минимальный параметрический участок построения уги
  double alpha = 2*Math::paramEpsilon; // Минимальный угол уги
  double angle = 0.1*Math::deviateSag;       // Максимальное отклонение от касательной к кривой
  double t, w2, t1, t2;
  MbArc * arc = nullptr;
  MbCartPoint p1, p2, r2;
  MbVector v1, v2, u1, u2;
  MbVector    vec;
  double a, b;
  ptrdiff_t i0, j0, l0, k0 = 5; // Число точек рпроверки на близость

  n = 2;
  switch (curve->IsA()) {
    case pt_Nurbs :
      n = ((MbNurbs *)curve)->GetPointListCount();
      break;
    case pt_Bezier :
      n = 1+(((MbBezier *)curve)->GetSplinesCount())/3;
      break;
    case pt_Arc :
      n = (ptrdiff_t)( 1+2*(M_PI/(tmax-tmin)) );
      break;
    case pt_LineSegment :
    case pt_Line :
      poly.Add( static_cast<MbCurve *>( &curve->Duplicate() ) );
      return;
    case pt_ContourWithBreaks : 
    case pt_Contour : {
      MbContour *cntr = (MbContour *)curve;
      for ( size_t i = 0; i < cntr->GetSegmentsCount(); i++ )
        PolyArc( cntr->SetSegment(i), eps, poly );
      return;
    }
    default: break;
  }

  dmax /= (double)n;
  eps = fabs(eps);
  dt = dmax;
  t1 = tmin;
  curve->PointOn( t1, p1 );
  curve->FirstDer( t1, v1 );
  v1.Normalize();
  i0 = 1;
  while (i0) {
    if (t1+dt>tmax-Math::paramEpsilon) {
      dt = tmax - t1;
      t2 = tmax;
    }
    else
      t2 = t1 + dt;
    arc = new MbArc();
    u1 = ~v1;
    j0 = 1;
    while (j0>0) {
      curve->PointOn( t2, p2 );
      vec.Init(p1,p2);
      a = vec * v1;
      b = vec * u1;
      if (a!=0 || b!=0) {
        a = atan2( b, a );
        if (fabs(a) < M_PI_2) {
          if (fabs(a) < alpha) {
            a = (a>0) ? alpha : -alpha;
          }
          arc->Init( a, p1, p2 );
          curve->FirstDer( t2, v2 );
          w2 = arc->GetTMax();
          arc->FirstDer(w2,u2);
          v2.Normalize();
          u2.Normalize();
          if (fabs(v2|u2)<angle) {
            w2 = (w2-arc->GetTMin()) / (double)(k0+1);
            l0 = 0;
            for ( ptrdiff_t i = 0; i < k0; i++ ) {
              t = (double)(i + 1) * w2;
              arc->PointOn( t, r2 );
              if ( curve->DistanceToPoint( r2 )>eps ) {
                l0++;
                break;
              }
            }
            if (l0==0)
              j0 = 0;
          }
        }
      }
      if (j0>0) {
        if (dt>delta) {
          dt /= 2;
          t2 = t1 + dt;
        }
        else {
          a = (b>0) ? alpha : -alpha;
          arc->Init( a, p1, p2 );
          t = arc->GetTMax();
          arc->FirstDer(t,u2);
          u2.Normalize();
          j0 = 0;
        }
      }
    }
    poly.Add(arc);
    if (t2>tmax-Math::paramEpsilon)
      i0 = 0;
    else {
      t1 = t2;
      p1 = p2;
      v1 = u2;
      dt += dt;
      if (dt>dmax)
        dt = dmax;
    }
  }

}


//------------------------------------------------------------------------------
// Аппроксимация кривой дугами окружности.
// Approximation of the curve by arcs.
// ---
void CreateArcsByCurve()
{
  MbItem * obj = nullptr;

  while ( GetMObj( IDS_SELECT_CRV_ON_PLANE, pt_Curve, &obj ) ) {
    const MbCurve * curve = static_cast<const MbCurve *>( static_cast<const MbPlaneInstance *>(obj)->GetPlaneItem() );
    double eps = 0.1; // KYA K12 как умолчательный в библиотеке // 1.0;
    if ( GetDouble(IDS_ENTER_PRECISION, eps) ) {
      // Аппроксимация кривой дугами и отрезками
      double maxRadius = Math::maxRadius;
      bool mate = true;
      MbCurve * poly = ::FatArcContour( *curve, eps, maxRadius, mate );

      if ( poly != nullptr ) {
        MbPlacement3D polyPlace;

        if ( ((MbPlaneInstance *)obj)->GetPlacement( polyPlace ) )
          TestVariables::viewManager->AddObject( TestVariables::LINE_SEG_Style, poly, &polyPlace );
        else
          TestVariables::viewManager->AddObject( TestVariables::LINE_SEG_Style, poly );
      }
    }
  }

}


//------------------------------------------------------------------------------
// Переопределение базисных точек NURBS из Close в Open при замкнутом узловом векторе.
// Overriding NURBS basis points from Close to Open with a closed nodal vector.
// ---
bool ConvertPlaneNURBSIntoOpen() {

  MbItem *obj;

  if ( GetMObj( IDS_SELECT_CURVE, pt_Nurbs, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    const MbPlaneItem * planeItem = ((MbPlaneInstance *)obj)->GetPlaneItem();
    if (planeItem != nullptr && planeItem->IsA()==pt_Nurbs) {
      MbNurbs *nurbs = (MbNurbs *)planeItem;

      if ( nurbs->UnClamped( false ) ) {
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
bool ConvertPlaneNURBSIntoClose() {

  MbItem *obj;

  if ( GetMObj( IDS_SELECT_CURVE, pt_Nurbs, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    const MbPlaneItem * planeItem = ((MbPlaneInstance *)obj)->GetPlaneItem();
    if (planeItem != nullptr && planeItem->IsA()==pt_Nurbs) {
      MbNurbs *nurbs = (MbNurbs *)planeItem;

      if ( nurbs->UnClamped( true ) ) {
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
// Выделить элементы конструктивной плоскости.
// Select elements of the constructive plane.
// ---
void SelectCPlane() {
  MbItem *obj;

  while ( GetMObj( IDS_SELECT_AREA_ON_PLANE, pt_PlaneItem, &obj ) ) {
    TestVariables::viewManager->ModelSelectByObject( obj );
  }
}


//------------------------------------------------------------------------------
// Переместить в своей плоскости. 
// Move in the own plane.
// ---
void MoveCPlane() 
{
  MbItem * obj( nullptr );
  MbCartPoint3D points[2];
  MbVector3D to;

  while ( GetMObj( IDS_SELECT_AREA_ON_PLANE, pt_PlaneItem, &obj ) ) {
    if ( GetPoint( IDS_SELECT_START_POINT, points[0] ) ) {
      while ( GetPoint( IDS_SELECT_FINAL_POINT, points[1] ) ) {
        to.Init( points[0], points[1] );
        TestVariables::viewManager->MoveObjectInPlacement( obj, to );
        points[0] = points[1];
      }
    }
  }
}


//------------------------------------------------------------------------------
// Повернуть в своей плоскости. 
// Rotate in the own plane.
// ---
void RotateCPlane() 
{
  MbItem * obj( nullptr );
  MbCartPoint3D point;
  double angle( 90 );

  if ( GetPoint( IDS_SELECT_START_POINT, point ) ) {
    if ( GetDouble( IDS_ENTER_ANGLE, angle ) ) {
      while ( GetMObj( IDS_SELECT_AREA_ON_PLANE, pt_PlaneItem, &obj ) ) {
        angle *= M_DEGRAD;
        TestVariables::viewManager->RotateObjectInPlacement( obj, point, angle );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Совместить конструктивную плоскость с плоскостью вида.
// Combine the constructive plane with the view plane.
// ---
void ComplanarCPlane()
{
  MbItem * obj = nullptr;
  while ( GetMObj( IDS_SELECT_AREA_ON_PLANE, pt_PlaneItem, &obj ) ) {
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    TestVariables::viewManager->SetObjectPlacement( obj, wind->GetNullPlacement() );
  }
}


//------------------------------------------------------------------------------
// Перевести в локальную плоскость. Transform by local plane.
// ---
void TransformPlaneObject()
{
  MbCartPoint3D p0, p;
  if ( GetPoint( IDS_SELECT_CENTER, p0 ) ) {
    MbVector3D axisX( 1.0, 0.0, 0.0 ), axisY( 0.0, 1.0, 0.0 ), axisZ( 0.0, 0.0, 1.0 );
    p = axisX;
    if ( GetPoint( IDS_SELECT_VECTOR_X, p ) ) {
      axisX = p;
      p = axisY;
      if ( GetPoint( IDS_SELECT_VECTOR_Y, p ) ) {
        axisY = p;
        p = axisZ;

        MbPlacement place;
        place.SetOrigin().x = p0.x;
        place.SetOrigin().y = p0.y;
        place.SetAxisX().x = axisX.x;
        place.SetAxisX().y = axisX.y;
        place.SetAxisY().x = axisY.x;
        place.SetAxisY().y = axisY.y;
        MbMatrix matr;
        place.GetMatrixInto( matr );

        if ( !matr.IsSingle() ) {
          MbItem * obj = nullptr;

          if ( GetMObj( IDS_SELECT_OBJ_ON_PLANE, pt_PlaneItem, &obj ) ) {
            SetWaitCursor( true );

            TestVariables::viewManager->TransformObjectInPlacement( obj, matr );

            SetWaitCursor( false );
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Удаление всех объектов плоскости.
// Delete all objects of plane.
// ---
void DeletePlaneObjects()
{
  TestVariables::viewManager->DeletePlaneItems();
}


//------------------------------------------------------------------------------
// Нормализовать двумерный объект.
// Normalize a two-dimensional object.
// ---
void Normalize() {
  MbItem * obj = nullptr;

  while ( GetMObj( IDS_SELECT_CURVE, pt_Curve, &obj ) ) {
    MbPlaneItem * planeItem = ((MbPlaneInstance *)obj)->SetPlaneItem(); // Дать двумерный геометрический объект. // Give a two-dimensional geometric object.
    if ( planeItem != nullptr ) {
      if ( planeItem->IsA() == pt_Arc ) {
        (static_cast<MbArc *>(planeItem))->Normalize();
        TestVariables::viewManager->RefreshObjectModel(obj);
      }
    }
  }

}

