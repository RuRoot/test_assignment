#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <cur_arc3d.h>
#include <cur_line_segment3d.h>
#include <cur_nurbs3d.h>
#include <cur_hermit3d.h>
#include <cur_cubic_spline3d.h>
#include <cur_plane_curve.h>
#include <cur_surface_curve.h>
#include <cur_contour3d.h>
#include <cur_hermit.h>
#include <point3d.h>
#include <op_curve_parameter.h>
#include <space_instance.h>
#include <plane_instance.h>
#include <action_curve3d.h>
#include <test_draw.h>
#include <wire_frame.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Сдвиг точки вдоль направления кривой.
// Shift of the point along the curve direction.
// ---
bool MovePointAlongLine() {
  MbItem *nearest;
  MbItem *nearest0;

  if ( GetGObj( IDS_SELECT_POINT, st_Point3D, &nearest0 ) && (nearest0->IsA() == st_SpaceInstance) ) {
    if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest ) && (nearest->IsA() == st_SpaceInstance) ) {
      TestVariables::viewManager->ToggleObject(nearest);

      double d = 10;
      if ( GetDouble( IDS_ENTER_DISTANCE, d ) ) {
        MbPoint3D *r = (MbPoint3D *)((MbSpaceInstance *)nearest0)->GetSpaceItem();
        MbCurve3D *c = (MbCurve3D *)((MbSpaceInstance *)nearest)->GetSpaceItem();
        double t;
        MbVector3D vect;
        MbCartPoint3D p;
        r->GetCartPoint( p );
        c->NearPointProjection( p, t, false );
        c->FirstDer( t, vect );
        vect.Normalize();
        vect *= d;
        nearest0->Move( vect );
        ::DeleteItem( c );
        TestVariables::viewManager->ToggleObject(nearest);
        return true;
      }
      TestVariables::viewManager->ToggleObject(nearest);
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Задание новой точки кривой.
// Specify a new point on the curve.
// ---
bool AddSplineCurvePoint() {
  ptrdiff_t index = 0;
  MbItem * obj;

  while ( GetAnyCurve( IDS_SELECT_CURVE, &obj ) ) {
    if ( obj->IsA() == st_SpaceInstance ) {
      const MbSpaceItem * spaceItem = ((MbSpaceInstance *)obj)->GetSpaceItem();
      if ( (spaceItem != nullptr) && (spaceItem->Family() == st_Curve3D) ) {
        MbCartPoint3D pnt;
        if ( GetPoint( IDS_SELECT_POINT, pnt ) &&
             GetInt( IDS_ENTER_POINT_INDEX, index ) ) {
          MbCurve3D *curve = (MbCurve3D *)spaceItem;
          if ( curve->IsA() == st_Nurbs3D ||
               curve->IsA() == st_Bezier3D ) {
            MbPolyCurve3D *pcurve = (MbPolyCurve3D *)curve;
            pcurve->InsertPoint( index, pnt ); // Добавить точку
            TestVariables::viewManager->RefreshObjectModel( obj );
            return true;
          }
        }
      }
    }
    if ( obj->IsA() == st_PlaneInstance ) {
      const MbPlaneItem * planeItem = ((MbPlaneInstance *)obj)->GetPlaneItem();
      if ( (planeItem != nullptr) && (planeItem->Family() == pt_Curve) ) {
        MbCartPoint pnt;
        if ( GetPoint( IDS_SELECT_POINT, pnt ) &&  // Нужна проекция точки на плоскость кривой (а сейчас получим точку на видовой плоскости)
             GetInt( IDS_ENTER_POINT_INDEX, index ) ) {
          MbCurve *curve = ( MbCurve *)planeItem;
          if ( curve->IsA() == pt_Nurbs ||
               curve->IsA() == pt_Bezier ) {
            MbPolyCurve *pcurve = ( MbPolyCurve *)curve;
            pcurve->InsertPoint( index, pnt ); // Добавить точку
            TestVariables::viewManager->RefreshObjectModel( obj );
            return true;
          }
        }
      }
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Добавить новый узел в кривую.
// Add a new node to the curve.
// ---
bool AddNurbsKnot()
{  
  MbItem * obj = nullptr;

  while ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) && (obj->IsA() == st_SpaceInstance) ) {
    const MbSpaceItem * spaceItem = ((MbSpaceInstance *)obj)->GetSpaceItem();
    if ( (spaceItem != nullptr) && (spaceItem->Family() == st_Curve3D) ) {
      MbCartPoint3D pnt;
      ptrdiff_t index1 = 0;
      if ( GetInt( IDS_ENTER_POINT_INDEX, index1 ) ) {
        //ptrdiff_t index2 = index1 + 1;
        MbCurve3D *curve = (MbCurve3D *)spaceItem;
        if ( curve->IsA() == st_Nurbs3D ) {
          MbNurbs3D *pcurve = (MbNurbs3D *)curve;
          pcurve->InsertKnotsInRegion( index1 ); 
          TestVariables::viewManager->RefreshObjectModel( obj );
          return true;
        }
      }
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Удаление точки кривой.
// Delete a point of the curve.
// ---
bool RemoveNurbsKnote()
{
  ptrdiff_t index1 = 0;
  MbItem * obj = nullptr;

  while ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) && (obj->IsA() == st_SpaceInstance) ) {
    const MbSpaceItem * spaceItem = ((MbSpaceInstance *)obj)->GetSpaceItem();
    if ( (spaceItem != nullptr) && (spaceItem->Family() == st_Curve3D) ) {
      MbCartPoint3D pnt;
      if ( GetInt( IDS_ENTER_POINT_INDEX, index1 ) ) {
        //ptrdiff_t index2 = index1 + 1;
        MbCurve3D * curve = (MbCurve3D *)spaceItem;
        if ( curve->IsA() == st_Nurbs3D ) {
          MbNurbs3D * pcurve = (MbNurbs3D *)curve;
          pcurve->RemoveKnotAlways( index1 ); 
          //ptrdiff_t num = pcurve->RemoveKnot( index1, 1 );

          TestVariables::viewManager->RefreshObjectModel( obj );
          return true;
        }
      }
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Уменьшить\увеличить порядок nurbs кривой на 1.
// Reduce\raise the order of the nurbs curve at 1.
// ---
bool ChangeSpaceNurbsDegree( bool degreeDown )
{
  MbItem * obj = nullptr;

  while ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) && (obj->IsA() == st_SpaceInstance) ) {
    MbSpaceItem * spaceItem = static_cast<MbSpaceInstance *>(obj)->SetSpaceItem();
    if ( (spaceItem != nullptr) && (spaceItem->Family() == st_Curve3D) ) {
      MbCartPoint3D pnt;
      MbCurve3D * curve = static_cast<MbCurve3D *>( spaceItem );
      if ( curve->IsA() == st_Nurbs3D ) {
        MbNurbs3D * pcurve = static_cast<MbNurbs3D *>(curve);

        if ( degreeDown ) {
          if ( pcurve->ReductionDegree() ) {
            TestVariables::viewManager->RefreshObjectModel( obj );
            return true;
          }
          else {
            return false;
          }
        }
        else {
          size_t degree = pcurve->GetDegree();
          ++degree;
          if ( pcurve->RaiseDegree( degree ) ) {
            TestVariables::viewManager->RefreshObjectModel( obj );
            return true;
          }
          else {
            return false;
          }
        }
      }
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Замена точки кривой.
// Change point of the curve.
// ---
bool ChangeSpaceSplinePoint()
{
  MbCartPoint3D pnt;
  MbItem *obj;

  while ( GetGObj( IDS_SELECT_CURVE, st_PolyCurve3D, &obj ) &&
          GetPoint( IDS_SELECT_POINT, pnt ) ) {
    MbPolyCurve3D *curve = ( MbPolyCurve3D *)((MbSpaceInstance *)obj)->GetSpaceItem();

    if ( curve->IsA() == st_Nurbs3D ||
         curve->IsA() == st_Bezier3D ) {

      // Выдать индекс точки, ближайшей к заданной. // Give index of the point closest to a specified.
      ptrdiff_t index = curve->GetNearPointIndex( pnt );

      if ( index != -1 && GetPoint( IDS_SELECT_POINT, pnt ) ) {
        curve->ChangePoint( index, pnt ); // Заменить точку. // Change a point.
        TestVariables::viewManager->RefreshObjectModel( obj ); // Погасить кривую.  // Hide a curve.
        return true;
      }
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Замкнуть / разомкнуть кривую.
// Close / unclose a curve.
// ---
void SpaceSplineSetClosed()
{
  MbItem * obj( nullptr );

  while ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) && (obj->IsA() == st_SpaceInstance) ) {
    MbCurve3D * curve = ( MbCurve3D *)((MbSpaceInstance *)obj)->GetSpaceItem();

    if ( curve->IsA() == st_Nurbs3D   ||
         curve->IsA() == st_Bezier3D ||
         curve->IsA() == st_Hermit3D ||
         curve->IsA() == st_CubicSpline3D ||
         curve->IsA() == st_Polyline3D ) {
      bool cl = curve->IsClosed();
      // Установить признак замкнутости.  // Set a sign of closure.
      ((MbPolyCurve3D *)curve)->SetClosed( !cl ); // Замкнуть / разомкнуть кривую. // Close / unclose a curve.
      TestVariables::viewManager->RefreshObjectModel( obj ); // Погасить кривую. // Hide a curve.
    }
  }
}


//------------------------------------------------------------------------------
// Изменить направление кривой на противоположное.
// Reverse curve direction.
// ---
bool CurveInverse() {
  MbItem *obj;

  if ( GetAnyCurve( IDS_SELECT_CURVE, &obj ) ) {
    if ( obj->IsA() == st_SpaceInstance ) {
      const MbSpaceItem * spaceItem = ((MbSpaceInstance *)obj)->GetSpaceItem();
      if ( (spaceItem != nullptr) && (spaceItem->Family() == st_Curve3D) ) {
        MbCurve3D *curve = (MbCurve3D *)spaceItem;
        curve->Inverse();
        TestVariables::viewManager->RefreshObjectModel( obj );
        return true;
      }
    }
    if ( obj->IsA() == st_PlaneInstance ) {
      MbPlaneItem * planeItem = ((MbPlaneInstance *)obj)->SetPlaneItem();
      if ( (planeItem != nullptr) && (planeItem->Family() == pt_Curve) ) {
        MbCurve * curve = static_cast<MbCurve *>(planeItem);
        curve->Inverse();
        TestVariables::viewManager->RefreshObjectModel( obj );
      }
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Дать часть кривой.
// Give a part of the curve.
// ---
bool CreateMidlePart() {
  MbItem *obj;
  MbCartPoint3D points[2];

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) && (obj->IsA() == st_SpaceInstance) ) {
    TestVariables::viewManager->ToggleObject(obj);
    const MbSpaceItem * sItem = ((MbSpaceInstance *)obj)->GetSpaceItem(); // Дать трёхмерный геометрический объект. // Give a three-dimensional geometric object.
    if ( (sItem != nullptr) && (sItem->Family() == st_Curve3D) ) {
      MbCurve3D * curve = (MbCurve3D *)sItem;

      if ( GetPoint( IDS_SELECT_START_POINT, points[0] ) &&
           GetPoint( IDS_SELECT_FINAL_POINT, points[1] ) ) {
        MbCurve3D * curv = nullptr;
        double t1 = UNDEFINED_DBL, t2 = UNDEFINED_DBL;
        curve->NearPointProjection( points[0], t1, false );
        curve->NearPointProjection( points[1], t2, false );
        if (t1<t2)
          curv = curve->Trimmed( t1, t2, 1);
        else
          curv = curve->Trimmed( t2, t1, 1);
        TestVariables::viewManager->ToggleObject(obj);
        if ( TestVariables::part != 0 ) {
          ((MbSpaceInstance*)obj)->SetSpaceItem(curv);
          TestVariables::viewManager->RefreshModel();
        }
        return true;
      }
    }
    else
      TestVariables::viewManager->ToggleObject(obj);
  }

  return false;
}


//------------------------------------------------------------------------------
// Дать концевые части кривой.
// Give end parts of the curve.
// ---
bool DeleteMidlePart() {
  MbItem *obj;
  MbCartPoint3D points[2];

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) && (obj->IsA() == st_SpaceInstance) ) {
    TestVariables::viewManager->ToggleObject(obj);
    MbCurve3D *curve = ( MbCurve3D *)((MbSpaceInstance *)obj)->GetSpaceItem();

    if ( GetPoint( IDS_SELECT_START_POINT, points[0] ) &&
         GetPoint( IDS_SELECT_FINAL_POINT, points[1] ) ) {
      MbCurve3D *part1 = nullptr;
      MbCurve3D *part2 = nullptr;
      double t1 = UNDEFINED_DBL, t2 = UNDEFINED_DBL;
      curve->NearPointProjection( points[0], t1, false );
      curve->NearPointProjection( points[1], t2, false );
      double tmin = curve->GetTMin();
      double tmax = curve->GetTMax();

      if ( t1 < t2 ) {
        if ( fabs(t1-tmin) >= Math::paramRegion )
          part1 = curve->Trimmed( tmin, t1, 1);
        if ( fabs(t2-tmax) >= Math::paramRegion )
          part2 = curve->Trimmed( t2, tmax, 1);
      }
      else {
        if ( fabs(t2-tmin) >= Math::paramRegion )
          part1 = curve->Trimmed( tmin, t2, 1);
        if ( fabs(t1-tmax) >= Math::paramRegion )
          part2 = curve->Trimmed( t1, tmax, 1);
      }

      TestVariables::viewManager->ToggleObject(obj);
      if (part1!=nullptr) {
        ((MbSpaceInstance*)obj)->SetSpaceItem( part1 );
        TestVariables::viewManager->RefreshModel();
      }
      if (part2!=nullptr)
        TestVariables::viewManager->AddObject( Style( obj->GetWidth(), obj->GetColor() ), part2 );

      return true;
    }
    else
      TestVariables::viewManager->ToggleObject(obj);
  }

  return false;
}


//------------------------------------------------------------------------------
// Разбить кривую.
// Break a curve.
// ---
bool CreateBreakParts()
{
  MbItem * obj = nullptr;
  MbCartPoint3D point;
  Style ns = TestVariables::CURVE_Style;
  Style st;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) && (obj->IsA() == st_SpaceInstance) ) {
    TestVariables::viewManager->ToggleObject(obj);
    MbCurve3D * curve = static_cast<MbCurve3D *>(static_cast<MbSpaceInstance *>(obj)->SetSpaceItem());

    if ( GetPoint( IDS_SELECT_POINT, point ) ) {
      MbCurve3D * part1 = nullptr;
      MbCurve3D * part2 = nullptr;
      double t = UNDEFINED_DBL;
      curve->NearPointProjection( point, t, false );
      double tmin = curve->GetTMin();
      double tmax = curve->GetTMax();

      if ( ::fabs(t - tmin) < Math::paramRegion )
        part1 = nullptr;
      else
        part1 = curve->Trimmed( tmin, t, 1 );

      if ( ::fabs(t - tmax) < Math::paramRegion )
        part2 = nullptr;
      else
        part2 = curve->Trimmed( t, tmax, 1 );

      TestVariables::viewManager->ToggleObject(obj);
      if ( part1 != nullptr )
        TestVariables::viewManager->AddObject( Style( obj->GetWidth(), obj->GetColor() ), part1, false );
      if ( part2 != nullptr )
        TestVariables::viewManager->AddObject( Style( obj->GetWidth(), obj->GetColor() ), part2, false );

      TestVariables::viewManager->DeleteObject( obj );
      TestVariables::viewManager->RefreshModel();
      ::MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );
      return true;
    }
    else {
      TestVariables::viewManager->ToggleObject( obj );
    }
  }
  else {
    MbTopItem * item = nullptr;

    if ( ::GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &item ) && item->IsAnEdge() ) {
      SPtr<MbCurveEdge> edge0( static_cast<MbCurveEdge *>(item) );

      double t = 0.5;
      MbRect1D tRange( 0.0, 1.0 );

      if ( ::GetDouble( IDS_ENTER_PARAMETER, t, &tRange ) ) {
        t = edge0->GetCurveParam( t );
        SPtr<MbCurveEdge> edge( edge0->CuttingEdge( t, true, nullptr ) );
        if ( edge != nullptr ) {
          TestVariables::viewManager->RefreshModel();
          ::MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );
          return true;
        }
      }
      else {
        // write new variant here if need split by point
      }
    }
  }

  ::MessageBoxEx( RT_DO_NOT_DO, IDS_TITLE_MSG );
  return false;
}


//------------------------------------------------------------------------------
// Удалить кривую, оставив базовые объекты.
// Remove the curve, leaving the base objects.
// ---
bool RemoveCurveBasis()
{
  MbItem * nearest = nullptr;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest ) ) {
    TestVariables::viewManager->ToggleObject(nearest);

    RPArray<MbSpaceItem> s( 0, 1 );
    SPtr<MbCurve3D> curve( ::GetCurve3D( nearest, true ) );

    if (curve != nullptr) {
      MbAxis3D axis;

      bool b = curve->GetCircleAxis(axis);

      if ( b ) {
        TestVariables::drawImp->DrawPoint( &axis.GetOrigin(), TRGB_LIGHTBLUE );
      }

      curve->GetBasisItems(s);
    }
    else {
      nearest->GetBasisItems( s );
      if ( s.Count() == 1 && s[0] != nullptr && s[0]->Family() == st_Curve3D ) {
        curve = static_cast<MbCurve3D *>( s[0] );
        s.DetachAll();
        curve->GetBasisItems( s );
      }
    }

    ::AddRefItems( s );

    size_t n = s.Count();
    TestVariables::viewManager->ToggleObject( nearest );

    if ( n > 0 ) {
      TestVariables::viewManager->DeleteObject( nearest ); // Удаление элемента.  // Delete an element.
      for ( size_t i = 0; i < n; i++ )
        TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, &(s[i]->Duplicate()) );
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
bool ConvertSpaceNurbsIntoOpen()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_CURVE, st_Nurbs3D, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    const MbSpaceItem * spaceItem = ((MbSpaceInstance *)obj)->GetSpaceItem();
    if (spaceItem != nullptr && spaceItem->IsA() == st_Nurbs3D) {
      MbNurbs3D * nurbs = (MbNurbs3D *)spaceItem;

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
bool ConvertSpaceNurbsIntoClose()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_CURVE, st_Nurbs3D, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    const MbSpaceItem * spaceItem = ((MbSpaceInstance *)obj)->GetSpaceItem();
    if (spaceItem != nullptr && spaceItem->IsA() == st_Nurbs3D) {
      MbNurbs3D * nurbs = (MbNurbs3D *)spaceItem;

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
// Состыковать сплайн с объектом.
// Attach NURBS to Object.
// ---
bool AttachSpaceNurbsToObject()
{
  MbItem * obj = nullptr;
  
  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) && (obj->IsA() == st_SpaceInstance) ) { // Выбираем NURBS
    TestVariables::viewManager->ToggleObject(obj);
    MbNurbs3D       * nurbs       = nullptr;
    MbCubicSpline3D * cubicSpline = nullptr;
    
    const MbSpaceItem * spaceItem = ((MbSpaceInstance *)obj)->GetSpaceItem();
    if (spaceItem != nullptr && spaceItem->IsA() == st_Nurbs3D)
      nurbs = ( MbNurbs3D * )spaceItem;
    else if ( spaceItem != nullptr && spaceItem->IsA() == st_CubicSpline3D )
      cubicSpline = ( MbCubicSpline3D * )spaceItem;
    
    if ( nurbs || cubicSpline) {
      MbPolyCurve3D * spline = nullptr;
      if ( nurbs!= nullptr )
        spline = nurbs;
      else
        spline = cubicSpline;
      // Выбираем сам объект, с которым производится стыковка
      if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) && (obj->IsA() == st_SpaceInstance) ) { 
        MbCurve3D * object = (MbCurve3D *)((MbSpaceInstance *)obj)->GetSpaceItem();  
        double tSpline = spline->GetTMin(), 
               tObject = object->GetTMin(), 
               dmin = 0.0;
        ::NearestPoints( *spline, true, *object, true, tSpline, tObject, dmin );
       
        bool beg = ::fabs( tSpline - spline->GetTMin() ) < ::fabs( tSpline - spline->GetTMax() );
        MbeMatingType type = trt_Tangent; // Вид сопряжения
        int itype = type;
        bool modify = false; // Модифицировать ли исходные точки
        int  imod = modify;
        GetInt( IDS_ENTER_USE_OR_NOT, imod );
        modify = imod > 0 ? true : false;
        GetInt( IDS_ENTER_CONN_TYPE, itype ); 
        PntMatingData3D begData, otherData;
        if ( itype >= (int)trt_Position && itype <= (int)trt_SmoothG3 )
        {
          type = (MbeMatingType)( itype ); // Выбрали тип сопряжения для первой кривой
          MbVector3D tang;
          MbVector3D * tangDiff  = nullptr;
          MbVector3D * tang2Diff = nullptr;
          SArray<MbCartPoint3D> points( 0, 1 );
          spline->GetPointList( points );
          SArray<intptr_t> * changedInds = nullptr;

          switch ( type ) {
            case trt_Tangent:
            object->_Tangent( tObject, tang ); 
            begData.Init( trt_Tangent, &tang, nullptr, nullptr, changedInds, true, true );
            break;
            case trt_Normal :
            object->Normal( tObject, tang );
            begData.Init( trt_Tangent, &tang, nullptr, nullptr, changedInds, true, true );
            break;
            case trt_SmoothG2 :
            {
              object->_Tangent( tObject, tang ); 
              tangDiff = new MbVector3D();
              object->Normal( tObject, *tangDiff );
              *tangDiff *= object->Curvature( tObject );
              begData.Init( trt_SmoothG2, &tang, tangDiff, nullptr, changedInds, true, true );
            }
            break;
            case trt_SmoothG3 :
            {
              object->Tangent( tObject, tang );
              tangDiff = new MbVector3D();
              object->Normal ( tObject, *tangDiff );
              *tangDiff *= object->Curvature( tObject );              
              tang2Diff = new MbVector3D();
              object->ThirdMetricDer( tObject, *tang2Diff );
              begData.Init( trt_SmoothG3, &tang, tangDiff, tang2Diff, changedInds, true, true );
            }
            default: break;
          }
          if ( nurbs != nullptr )
            nurbs->AttachG( begData, beg );
          else {
            MbeSplineParamType spType = spt_ChordLength;
            nurbs = nullptr;

            MbeSplineCreateType useInitThrough = sct_Version2; // BUG_54449
//           otherData.Init( trt_SmoothG2, &MbVector3D(1, 0, 0), &MbVector3D(0, 1, 0), nullptr, false );
            if ( beg )
              nurbs = MbNurbs3D::CreateNURBS4( points, spType, begData, otherData, useInitThrough );
            else
              nurbs = MbNurbs3D::CreateNURBS4( points, spType, otherData, begData, useInitThrough );
            
            if ( nurbs != nullptr )
              TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, nurbs );
          }
        }
      }
    }

    else { // Состыковать две кривые на поверхностях. // C3D-3005
      if ( spaceItem != nullptr && spaceItem->IsA() == st_SurfaceCurve ) {
        MbSurfaceCurve * sCurve1 = ( MbSurfaceCurve * )spaceItem;
        if ( sCurve1 != nullptr && sCurve1->GetCurve().IsA() == pt_Hermit ) {
          MbHermit * spline1 = static_cast<MbHermit *>( &sCurve1->SetCurve() );
          MbItem * obj2 = nullptr;
          if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj2 ) && (obj2->IsA() == st_SpaceInstance) ) { 
            const MbSpaceItem * spaceItem2 = ((MbSpaceInstance *)obj2)->GetSpaceItem();
            if ( spaceItem2 != nullptr && spaceItem2->IsA() == st_SurfaceCurve ) {
              MbSurfaceCurve * sCurve2 = ( MbSurfaceCurve * )spaceItem2;
              if ( sCurve2 != nullptr && sCurve2->GetCurve().IsA() == pt_Hermit ) { // Выбраны две кривые на поверхности.
                MbHermit * spline2 = static_cast<MbHermit *>( &sCurve2->SetCurve() );
                double t1, t2, dMin;

                MbeNewtonResult nr = ::NearestPoints( *sCurve1, false, *sCurve2, false, t1, t2, dMin );

                if ( nr != nr_Failure && dMin < PARAM_DELTA_MAX ) {
                  MbCartPoint3D pnt, pnt1, pnt2;
                  MbVector3D der, der1, der2;
                  sCurve1->Explore( t1, false, pnt1, der1, nullptr, nullptr );
                  sCurve2->Explore( t2, false, pnt2, der2, nullptr, nullptr );
                  pnt.Set( pnt1, ONE_HALF, pnt2, ONE_HALF );
                  const MbSurface & surface1 = sCurve1->GetSurface();
                  const MbSurface & surface2 = sCurve2->GetSurface();
                  MbCartPoint r1, r2;
                  if ( surface1.NearPointProjection( pnt, r1.x, r1.y, true, nullptr) &&
                       surface2.NearPointProjection( pnt, r2.x, r2.y, true, nullptr) ) {
                    double t1min = spline1->GetTMin(), t1max = spline1->GetTMax();
                    double t2min = spline2->GetTMin(), t2max = spline2->GetTMax();
                    size_t count1 = spline1->GetPointsCount();
                    size_t count2 = spline2->GetPointsCount();
                    if ( count1 > 2 && count2 > 2 ) {
                      double dt1 = ( t1max - t1min ) / count1;
                      double dt2 = ( t2max - t2min ) / count2;

                      if ( ::fabs(t1 - t1min) < dt1 )
                        spline1->ChangePoint( 0, r1 );
                      else
                      if ( ::fabs(t1 - t1max) < dt1 )
                        spline1->ChangePoint( count1 - 1, r1 );

                      if ( ::fabs(t2 - t2min) < dt2 )
                        spline2->ChangePoint( 0, r2 );
                      else
                      if ( ::fabs(t2 - t2max) < dt2 )
                        spline2->ChangePoint( count2 - 1, r2 );

                      sCurve1->Refresh();
                      sCurve2->Refresh();
                      TestVariables::viewManager->RefreshObjectModel( obj );
                      TestVariables::viewManager->RefreshObjectModel( obj2 );
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  
  TestVariables::viewManager->ToggleObject(obj);
  
  return false;

}


//------------------------------------------------------------------------------
// Объединить два сплайна. 
// Add NURBS to NURBS.
// ---
bool AddSpaceNurbsToNurbs()
{
  MbItem * obj1 = nullptr;
  bool result = false;
  // Выбираем NURBS (кривую).
  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj1 ) && (obj1->IsA() == st_SpaceInstance) ) {
    TestVariables::viewManager->ToggleObject(obj1);
    MbSpaceItem * spaceItem1 = ((MbSpaceInstance *)obj1)->SetSpaceItem();
    if ( spaceItem1 != nullptr && spaceItem1->Family() == st_Curve3D ) {
      MbItem * obj2 = nullptr;
      // Выбираем NURBS (кривую).
      if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj2 ) && (obj2->IsA() == st_SpaceInstance) ) { 
        TestVariables::viewManager->ToggleObject(obj2);
        MbSpaceItem * spaceItem2 = ((MbSpaceInstance *)obj2)->SetSpaceItem();
        if ( spaceItem2 != nullptr && spaceItem2->Family() == st_Curve3D ) {

          if ( spaceItem1->IsA() == st_Nurbs3D && spaceItem2->IsA() == st_Nurbs3D ) { // Выбраны два NURBS.
            MbNurbs3D * nurbs1 = static_cast<MbNurbs3D *> ( spaceItem1 );
            MbNurbs3D * nurbs2 = static_cast<MbNurbs3D *> ( spaceItem2 );
            if ( nurbs1 != nullptr && nurbs2 != nullptr ) {

              bool bmerge = true;
              nurbs1->AddCurve( *nurbs2, bmerge );

              TestVariables::viewManager->ToggleObject( obj2 );
              TestVariables::viewManager->DeleteObject( obj2 );
              TestVariables::viewManager->RefreshObjectModel( obj1 );
              TestVariables::viewManager->ToggleObject( obj1 );
              result = true;
              return result;
            }
          }

          if ( spaceItem1 != nullptr && spaceItem1->IsA() == st_SurfaceCurve &&
               spaceItem2 != nullptr && spaceItem2->IsA() == st_SurfaceCurve ) { // Объединение двух кривых на поверхностях. // C3D-3005
            MbSurfaceCurve * sCurve1 = static_cast<MbSurfaceCurve *>( spaceItem1 );
            MbSurfaceCurve * sCurve2 = static_cast<MbSurfaceCurve *>( spaceItem2 );
            const MbSurface & surface1 = sCurve1->GetSurface();
            const MbSurface & surface2 = sCurve2->GetSurface();
            if ( (&surface1 == &surface2) && // Выбраны две кривые на общей поверхности.
                 (sCurve1 != nullptr && sCurve1->GetCurve().IsA() == pt_Hermit) &&
                 (sCurve2 != nullptr && sCurve2->GetCurve().IsA() == pt_Hermit) ) {
              MbHermit * spline1 = static_cast<MbHermit *>( &sCurve1->SetCurve() );
              MbHermit * spline2 = static_cast<MbHermit *>( &sCurve2->SetCurve() );
              double t1, t2, dMin;

              MbeNewtonResult nr = ::NearestPoints( *sCurve1, false, *sCurve2, false, t1, t2, dMin );

              if ( nr != nr_Failure && dMin < PARAM_DELTA_MIN ) {
                MbCartPoint3D pnt, pnt1, pnt2;
                MbVector3D der, der1, der2;

                sCurve1->Explore( t1, false, pnt1, der1, nullptr, nullptr );
                sCurve2->Explore( t2, false, pnt2, der2, nullptr, nullptr );
                pnt.Set( pnt1, ONE_HALF, pnt2, ONE_HALF );
                MbCartPoint r1, r2;
                if ( surface1.NearPointProjection( pnt, r1.x, r1.y, true, nullptr) &&
                     surface2.NearPointProjection( pnt, r2.x, r2.y, true, nullptr) ) {
                  double t1min = spline1->GetTMin(), t1max = spline1->GetTMax();
                  double t2min = spline2->GetTMin(), t2max = spline2->GetTMax();
                  size_t count1 = spline1->GetPointsCount();
                  size_t count2 = spline2->GetPointsCount();
                  if ( count1 > 2 && count2 > 2 ) {
                    double dt1 = ( t1max - t1min ) / count1;
                    double dt2 = ( t2max - t2min ) / count2;
                    MbHermit * spline( nullptr );

                    if ( ::fabs(t1 - t1min) < dt1 ) {
                      if ( ::fabs(t2 - t2min) < dt2 ) {
                        spline2->Inverse();
                        spline = spline1->CurvesCombine( t1min, t1max, false, *spline2, spline2->GetTMax(), spline2->GetTMin(), 1.0, true );
                      }
                      else
                      if ( ::fabs(t2 - t2max) < dt2 ) {
                        spline = spline1->CurvesCombine( t1min, t1max, false, *spline2, t2max, t2min, 1.0, true );
                      }
                    }
                    else
                    if ( ::fabs(t1 - t1max) < dt1 ) {
                      if ( ::fabs(t2 - t2min) < dt2 ) {
                        spline = spline1->CurvesCombine( t1min, t1max, true, *spline2, t2min, t2max, 1.0, true );
                      }
                      else
                      if ( ::fabs(t2 - t2max) < dt2 ) {
                        spline2->Inverse();
                        spline = spline1->CurvesCombine( t1min, t1max, true, *spline2, spline2->GetTMin(), spline2->GetTMax(), 1.0, true );
                      }
                    }

                    if ( spline != nullptr ) {
                      sCurve1->ChangeCurve( *spline );
                      sCurve1->Refresh();
                      TestVariables::viewManager->ToggleObject( obj2 );
                      TestVariables::viewManager->DeleteObject( obj2 );
                      TestVariables::viewManager->RefreshObjectModel( obj1 );
                      TestVariables::viewManager->ToggleObject( obj1 );
                      result = true;
                      return result;
                    }
                  }
                }
              }
            }
          }

        }
      }
    }
    TestVariables::viewManager->ToggleObject(obj1);
  }

  return result;
}


//------------------------------------------------------------------------------
// Гладко сопрячь сегменты контура. 
// Conjugate the contour segments smoothly.
// ---
bool ConjugateContourSegments()
{
  MbItem * obj = nullptr;
  bool res = false;
  // Выбираем контур. Choose the contour.
  if ( GetGObj( IDS_SELECT_CONTOUR, st_Curve3D, &obj ) && (obj->IsA() == st_SpaceInstance) ) {
    TestVariables::viewManager->ToggleObject( obj );
    const MbCartPoint3D p = TestVariables::cursorAxis.GetOrigin();
    const MbVector3D v = TestVariables::cursorAxis.GetAxisZ();
    MbSpaceItem * spaceItem = ((MbSpaceInstance *)obj)->SetSpaceItem();
    if ( spaceItem != nullptr && spaceItem->IsA() == st_Contour3D ) {
      MbContour3D * contour = (MbContour3D *)spaceItem;
      MbLine3D line( p, v );
      double t, w, dmin;

      ::NearestPoints( *contour, false, line, true, t, w, dmin );

      double tSeg;
      ptrdiff_t index = contour->FindSegment( t, tSeg );

      if ( index >= 0 ) {
        MbCurve3D * segment = contour->SetSegment( index );
        if ( segment != nullptr ) {
          double tMin = segment->GetTMin(), tMax = segment->GetTMax();
          //double tEps = ( tMax - tMin ) * PARAM_REGION;
          bool beg = ::fabs( tSeg - tMin ) < ::fabs( tSeg - tMax );
          MbCurve3D * neighbo( nullptr );
          if ( beg && index > 0 )
            neighbo = contour->SetSegment( index - 1 );
          if ( !beg && index < (ptrdiff_t)contour->GetSegmentsCount() )
            neighbo = contour->SetSegment( index + 1 );
          if ( neighbo != nullptr ) {
            MbCurve3D * segment1, * segment2;
            MbCartPoint3D p0, p1, p2;
            MbVector3D der0, der1, der2;
            double t1, t2;
            if ( beg ) {
              segment1 = neighbo;
              segment2 = segment;
            }
            else {
              segment1 = segment;
              segment2 = neighbo;
            }
            t1 = segment1->GetTMax();
            t2 = segment2->GetTMin();
            segment1->Explore( t1, false, p1, der1, nullptr, nullptr );
            segment2->Explore( t2, false, p2, der2, nullptr, nullptr );
            if ( der1.Angle(der2) < M_PI_4 ) {
              MbeSpaceType type1 = segment1->IsA();
              MbeSpaceType type2 = segment2->IsA();
              bool res0 = false;

              if ( !res0 && type1 == st_LineSegment3D && type2 == st_LineSegment3D ) {
                MbLineSegment3D * lineSeg1 = static_cast<MbLineSegment3D *>( segment1 );
                MbLineSegment3D * lineSeg2 = static_cast<MbLineSegment3D *>( segment2 );
                MbLineSegment3D lineSeg( lineSeg1->GetPoint1(), lineSeg2->GetPoint2() );
                lineSeg.NearPointProjection( p1, w, false, nullptr );
                lineSeg.PointOn( w, p0 );
                lineSeg1->SetPoint1( p0 );
                lineSeg2->SetPoint2( p0 );
                res0 = true;
              }

              if ( !res0 && type1 == st_Arc3D && type2 != st_Nurbs3D ) {
                MbArc3D * arc = static_cast<MbArc3D *>( segment1 );
                if ( arc->GetParamLength() < M_PI ) {
                  arc->_PointOn( arc->GetTMin(), p1 );
                  der2.Invert();
                  MbVector3D vec( p2, p1 );
                  double d = vec.Length() * ONE_HALF;
                  double angl = vec.Angle( der2 );
                  double sa = ::sin( angl );
                  if ( angl < M_PI_2 && sa > EPSILON && d > METRIC_EPSILON ) {
                    double r = d / sa;
                    double h = r * ( 1.0 - ::cos( angl ) );
                    der0 = der2;
                    der0 = der0.NormalComponent( vec );
                    der0.Normalize();
                    p0.Set( p1, ONE_HALF, p2, ONE_HALF );
                    p0.Add( der0, h );
                    arc->Init( p1, p0, p2, 1, false );
                    arc->Refresh();
                    res0 = true;
                  }
                }
              }

              if ( !res0 && type2 == st_Arc3D && type1 != st_Nurbs3D ) {
                MbArc3D * arc = static_cast<MbArc3D *>( segment2 );
                if ( arc->GetParamLength() < M_PI ) {
                  arc->_PointOn( arc->GetTMax(), p2 );
                  MbVector3D vec( p1, p2 );
                  double d = vec.Length() * ONE_HALF;
                  double angl = vec.Angle( der1 );
                  double sa = ::sin( angl );
                  if ( angl < M_PI_2 && sa > EPSILON && d > METRIC_EPSILON ) {
                    double r = d / sa;
                    double h = r * ( 1.0 - ::cos( angl ) );
                    der0 = der1;
                    der0 = der0.NormalComponent( vec );
                    der0.Normalize();
                    p0.Set( p1, ONE_HALF, p2, ONE_HALF );
                    p0.Add( der0, h );
                    arc->Init( p1, p0, p2, 1, false );
                    arc->Refresh();
                    res0 = true;
                  }
                }
              }

              if ( !res0 && type1 == st_Nurbs3D ) {
                MbNurbs3D * nurbs = static_cast<MbNurbs3D *>( segment1 );
                size_t pCount = nurbs->GetPointsCount();
                if ( pCount > 2 ) {
                  nurbs->GetPoint( pCount - 2, p0 );
                  double d = p0.DistanceToPoint( p2 );
                  der2.Invert();
                  der2.Normalize();
                  p0.Set( p2, der2, d );
                  nurbs->ChangePoint( pCount - 2, p0 );
                  nurbs->Refresh();
                  res0 = true;
                }
              }

              if ( !res0 && type2 == st_Nurbs3D ) {
                MbNurbs3D * nurbs = static_cast<MbNurbs3D *>( segment2 );
                size_t pCount = nurbs->GetPointsCount();
                if ( pCount > 2 ) {
                  nurbs->GetPoint( 1, p0 );
                  double d = p0.DistanceToPoint( p1 );
                  der1.Normalize();
                  p0.Set( p1, der1, d );
                  nurbs->ChangePoint( 1, p0 );
                  nurbs->Refresh();
                  res0 = true;
                }
              }

              if ( res0 )
                contour->Refresh();
            }
          }
        }
      }
      res = true;
    }
  }
  return res;
}


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

namespace {

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
}


//------------------------------------------------------------------------------
// Продлить кривую с одного из концов.
// Extend the curve from one of its ends.
// ---
bool CurveExtension() {
  const size_t maxStringBufSize = 1000; // Максимальная длина строки в сообщениях.  // Maximal string length in messages.
  MbItem *obj;

  if ( ::GetAnyCurve(IDS_SELECT_CURVE, &obj) ) {
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
    ::GetExtensionParamAtEnd( extendWayStart, extendLengthStart, paramStart );
    ::GetExtensionParamAtEnd( extendWayEnd, extendLengthEnd, paramEnd );

    const bool allowClosed( ::GetBoolean(IDS_ENTER_CLOSED_BOOL) );

    // Продлеваемая кривая. // Extended curve.
    ConstSpaceCurveSPtr crv3D( nullptr );

    if ( obj->IsA() == st_SpaceInstance ) {
      const MbSpaceItem * spaceItem = (static_cast<const MbSpaceInstance *>(obj))->GetSpaceItem();
      if ( (spaceItem != nullptr) && (spaceItem->Family() == st_Curve3D) ) {
        crv3D = static_cast<const MbCurve3D *>(spaceItem);
      }
    }
    else if ( obj->IsA() == st_PlaneInstance ) {
      const MbPlacement3D & curvPlacement = static_cast<MbPlaneInstance *>(obj)->GetPlacement(); // Дать плоскость. // Give a plane.
      const MbPlaneItem * planeItem = (static_cast<const MbPlaneInstance *>(obj))->GetPlaneItem();
      if ( (planeItem != nullptr) && (planeItem->Family() == pt_Curve) ) {
        SPtr<const MbCurve> curve = SPtr <const MbCurve>( static_cast<const MbCurve *>(planeItem) );
        crv3D = new MbPlaneCurve( curvPlacement, *curve, true );
      }
    }

    if ( crv3D != nullptr ) {
      const double l0 = crv3D->GetMetricLength(); // Длина исходной кривой. // Length of the source curve.
      const MbSNameMaker snMaker( SimpleName(-1), MbSNameMaker::i_SideNone, 0 );
      WireFrameSPtr resCurve( nullptr );
      MbResultType res( rt_Error );

      bool alongSurface( false ); // Удлинять вдоль поверхности. // Extend along surface.
      if ( crv3D->GetSubstrate().Type() == st_SurfaceCurve ) {
        alongSurface = ::GetBoolean( IDS_BOOL_USE_CNTR_ON_SURFACE );
      }

      MbCurveExtensionParameters3D opParameters( paramStart, paramEnd, allowClosed, alongSurface, snMaker );  // Параметры продления. // Extension parameters.
      res = ::ExtendCurve( *crv3D, opParameters, resCurve );

      if ( !resCurve.is_null() ) {
        TestVariables::viewManager->AddItem( *resCurve );

        double sumLength( 0.0 ); // Общая длина результирующей кривой. // Total length of the result curve.
        for ( size_t i = resCurve->GetEdgesCount(); i-- > 0; ) {
          sumLength += resCurve->GetEdge( i )->GetMetricLength();
        }
        
        TCHAR format[maxStringBufSize];
        _sntprintf( format, maxStringBufSize, _T("%s (%d). %s. %s"), IDS_DONE, static_cast<unsigned int>( res ), RT_CURVE_LENGTH_THE_ARC, _T("d = %22.15e") );
        _sntprintf( buff, maxStringBufSize, format, sumLength, sumLength - l0 );
        
        MessageBoxEx( buff, IDS_DONE );
        return true;
      }
      else {
        if ( res == rt_ParameterError ) { // Ошибка входных данных. // Error in input data.
          // Делаем диагностику для каждого из концов.  // Check parameter of every end.
          _sntprintf( buff, maxStringBufSize, _T("%s\n1: %d"), IDS_ERROR, static_cast<unsigned int>(::CheckExtParametersValid(paramStart)) );
          MessageBoxEx( buff, IDS_ERROR );
          _sntprintf( buff, maxStringBufSize, _T("%s\n2: %d"), IDS_ERROR, static_cast<unsigned int>(::CheckExtParametersValid(paramEnd)) );
          MessageBoxEx( buff, IDS_ERROR );
        }
        else {  // Ошибка выполнения. // Execution error.
          _sntprintf( buff, maxStringBufSize, _T( "%s: %d" ), RT_ERROR_, static_cast<unsigned int>(res) );
          MessageBoxEx( buff, IDS_ERROR );
        }
      }
    }
  }

  return false;
}
