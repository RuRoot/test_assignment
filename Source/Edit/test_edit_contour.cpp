#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <test_draw.h>
#include <cur_line_segment.h>
#include <cur_point_curve.h>
#include <cur_polyline.h>
#include <alg_curve_envelope.h>
#include <plane_instance.h>
#include <mb_cross_point.h>
#include <region.h>
#include <contour_graph.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Установить направление обхода контура.
// Set the direction of the contour bypass.
// ---
bool ContourSense()
{
  MbItem * obj = nullptr;
  MbCartPoint points[1];

  if ( GetPoint(IDS_SELECT_CNTR_ON_PLANE, points[0]) &&
       FindNearestObj(points[0], pt_Contour, &obj) ) {
    int sense = 1;
    if ( (obj != nullptr) && (obj->IsA() == st_PlaneInstance) ) {
      if ( GetInt( IDS_ENTER_SENSE, sense ) ) {
        MbContour *contour = (MbContour *)((MbPlaneInstance *)obj)->GetPlaneItem();
        contour->SetSense(sense);
      }
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Устранить разрывы в контуре
// ---
bool EliminateContourGaps()
{
  MbItem * obj = nullptr;
  MbCartPoint p;

  if ( GetPoint( IDS_SELECT_CNTR_ON_PLANE, p ) ) {
    if ( FindNearestObj( p, pt_Contour, &obj ) || FindNearestObj( p, pt_PlaneItem, &obj ) ) {
      if ( (obj != nullptr) && (obj->IsA() == st_PlaneInstance) ) {
        MbPlaneInstance * pInst = static_cast<MbPlaneInstance *>(obj);
        MbPlaneItem * pItem = pInst->SetPlaneItem();

        if ( pItem->Type() == pt_Contour ) {
          bool removeContourGaps = false;

          MbContour * contour = static_cast<MbContour *>(pItem);
          double eps = METRIC_PRECISION;

          if ( !contour->IsClosedContinuousC0( eps ) ) {
            size_t segmentsCnt = contour->GetSegmentsCount();
            size_t cnt = contour->IsClosed() ? (segmentsCnt+1) : segmentsCnt;

            MbCartPoint p1, p2;
            for ( size_t k = 0; k < cnt; k++ ) {
              const MbCurve * segm1 = contour->GetSegment( k % segmentsCnt );
              const MbCurve * segm2 = contour->GetSegment( (k+1)%segmentsCnt );
              segm1->GetLimitPoint( 2, p1 );
              segm2->GetLimitPoint( 1, p2 );
              double d = p1.DistanceToPoint( p2 );
              eps = std_max( d, eps );
            }

            removeContourGaps = true;
          }
          if ( removeContourGaps ) {
            bool canInsert = false;
            bool canReplace = false;
            if ( ::RemoveContourGaps( *contour, eps, canInsert, canReplace ) ) {
              if ( contour->IsClosedContinuousC0( eps ) )
                ::MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );
              else
                ::MessageBoxEx( RT_DONE_IN_PART, IDS_TITLE_MSG );
            }
            else {
              ::MessageBoxEx( IDS_FAILED, IDS_TITLE_MSG );
            }
          }

          return true;
        }
      }
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Разрушить контур на сегменты.
// Break contour into segments.
// ---
bool DisassemblyContour()
{
  MbCartPoint pnt;
  MbItem * obj = nullptr;

  if ( GetMObj( IDS_SELECT_FIRST_POINT, pt_Contour, &obj ) ) {
    if ( (obj != nullptr) && (obj->IsA() == st_PlaneInstance) ) {
      MbPlaneInstance * pi = static_cast<MbPlaneInstance *>(obj);
      if ( pi != nullptr ) {

        const MbPlacement3D & pl = pi->GetPlacement();
        for ( size_t j = 0, jCount = pi->PlaneItemsCount(); j < jCount; j++ ) {

          MbPlaneItem * pItem = pi->SetPlaneItem( j ); 
          if ( pItem->Type() == pt_Contour ) {

            MbContour * contour = static_cast<MbContour *>(pItem);
            size_t i, icount;
            /*
            for ( i = 0, icount = contour->GetSegmentsCount(); i < icount; i++ ) {
              const MbCurve * segm = contour->GetSegment( i );
              const MbCurve * next = contour->GetSegment( (i+1)%count );
              MbVector der, dir;
              double t = segm->GetTMax();
              double w = next->GetTMin();
              segm->FirstDer( t, der );
              next->FirstDer( w, dir );
              double len = der.Length();
              double lin = dir.Length();
              if ( ::fabs(len - lin) > EPSILON ) {
                // Проверка для BUG_60989.
              }
            }
            */
            for ( i = 0, icount = contour->GetSegmentsCount(); i < icount; i++ )
              TestVariables::viewManager->AddObject( TestVariables::LINE_Style, contour->SetSegment( i ), &pl );

            TestVariables::viewManager->DeleteObject(obj);
            return true;
          }
          else if ( pItem->Family() == pt_Region ) {
            MbRegion * region = static_cast<MbRegion *>(pItem);

            size_t cntrCnt = region->GetContoursCount();

            for ( size_t k = 0; k < cntrCnt; k++ ) {
              MbContour * contour = region->SetContour( k );
              if ( contour != nullptr ) {
                for ( size_t i = 0, icount = contour->GetSegmentsCount(); i < icount; i++ )
                  TestVariables::viewManager->AddObject( TestVariables::CONTOUR_Style, contour->SetSegment( i ), &pl );
              }
            }

            TestVariables::viewManager->DeleteObject(obj);
            return true;
          }
        }
      }
    }
  }
  return false;
}


//------------------------------------------------------------------------------
// Вставить точечную кривую в контур на плоскости.
// Insert a point curve in the contour on the plane.
// ---
bool InsertPointInContour()
{
  MbItem * obj = nullptr;
  MbCartPoint points[1];

  if ( GetPoint(IDS_SELECT_CNTR_ON_PLANE, points[0]) &&
       FindNearestObj(points[0], pt_Contour, &obj) ) {

    if ( (obj != nullptr) && (obj->IsA() == st_PlaneInstance) ) {
      MbPlaneInstance * pInst = static_cast<MbPlaneInstance *>(obj);
      MbPlaneItem * pItem = pInst->SetPlaneItem();
      MbPlacement3D pl( pInst->GetPlacement() );

      if ( pItem->Type() == pt_Contour ) {
        MbContour * contour = static_cast<MbContour *>(pItem);
        MbCartPoint p, pmin;
        double dmin( MB_MAXDOUBLE );
        size_t ind( SYS_MAX_T );
        size_t icount = contour->GetSegmentsCount();

        for ( size_t i = 0; i < icount; i++ ) {
          MbCurve * c = contour->SetSegment( i );
          double t = c->GetTMin();
          c->PointOn( t, p );
          double d = points[0].DistanceToPoint( p );
          if ( dmin > d ) {
            pmin = p;
            dmin = d;
            ind = i;
          }
        }
        if ( !contour->IsClosed() && (icount > 0) ) {
          MbCurve * c = contour->SetSegment( icount-1 );
          double t = c->GetTMax();
          c->PointOn( t, p );
          double d = points[0].DistanceToPoint( p );
          if ( dmin > d ) {
            pmin = p;
            dmin = d;
            ind = icount;
          }
        }
        if ( ind != SYS_MAX_T ) {
          MbCurve * c = new MbPointCurve( pmin, 0, 1, false );
          if ( ind < icount )
            contour->AddAtSegment( c, ind );
          else
            contour->AddSegment( c );

          ::DeleteItem( c );
          return true;
        }
      }
    }
  }
  return false;
}


//------------------------------------------------------------------------------
//
// ---
struct TangentDir {
  MbDirection dir;    // Вектор касательного направления. // The tangent direction vector.
  ptrdiff_t    idx;    // Индекс кривой. // Curve index.
  int          sense;  // Признак совпадения направления. // Sign of matching direction.
};


//------------------------------------------------------------------------------
// Выбрать подходящую кривую из узла.
// Choose the appropriate curve from the node.
// ---
ptrdiff_t SelectCurveFromNode( const MbCurve * pSegment, 
                               const MbCurve * selectCurve,
                               SArray<MbCrossPoint> & crossRight,
                               int & sense )
{
  MbCrossPoint cross;

  double t2 = pSegment->GetTMax();
  pSegment->PointOn( t2, cross.p );
  cross.on2.curve = selectCurve;
  cross.on2.t     = selectCurve->PointProjection( cross.p );

  cross.on1.curve = cross.on2.curve;
  cross.on1.t     = cross.on2.t;

  crossRight.Add( cross ); // Добавим кривую, по которой проходит сегмент

  ptrdiff_t count = crossRight.Count(); // Количество элементов в узле
  ptrdiff_t i;

  TangentDir tdTmp;
  SArray <TangentDir> tda;  // Множество касательных направлений

  for ( i = 0; i < count; i++ ) {
    const MbCurve * curve = crossRight[i].on2.curve;
    double tNode          = crossRight[i].on2.t;

    tdTmp.dir = curve->Tangent( tNode );
    tdTmp.idx = i;
    tdTmp.sense = 1;

    if ( curve->IsBounded() ) {
      // Ограниченная ( возможно усеченная ) кривая
      // Узел может попасть на граничную точку поэтому надо проверить
      // Это попадание

      if ( fabs( tNode - curve->GetTMin() ) < Math::paramRegion )  // Узел совпадает с начальной точкой кривой
        tda.Add( tdTmp );
      else
        if ( fabs( tNode - curve->GetTMax() ) < Math::paramRegion ) {
          // Узел совпадает с конечной точкой кривой
          tdTmp.dir.Invert();  // Меняем направление
          tdTmp.sense = -1;
          tda.Add( tdTmp );
        }
        else {
          // Узел находится "посередине" кривой
          // Необходимо учитывать два направления касательной в узле

          tda.Add( tdTmp );

          tdTmp.dir.Invert();  // Меняем направление
          tdTmp.sense = -1;
          tda.Add( tdTmp );
        }
    }
    else {
      tda.Add( tdTmp );

      tdTmp.dir.Invert();  // Меняем направление
      tdTmp.sense = -1;
      tda.Add( tdTmp );
    }
  }

  count = tda.Count();  // Количество направлений

  MbLineSegment ls;

  COLORREF col = GetSystemWindowColor();
  int R = GetRValue(col);
  int G = GetGValue(col);
  int B = GetBValue(col);
  
  i = 0;
  do {
    MbCartPoint p2( cross.p.x + tda[i].dir.ax * 10, cross.p.y + tda[i].dir.ay * 10 );
    ls.Init( cross.p, p2 );

    TestVariables::drawImp->DrawItem( &ls, TRGB_LIGHTRED, TestVariables::viewManager->GetCplaneMatrixFrom(), 10 );

    ptrdiff_t result = ExecuteChooseDirectionDlg(); // Выбор направления

    TestVariables::drawImp->DrawItem( &ls, R, G, B, TestVariables::viewManager->GetCplaneMatrixFrom(), 10 );

    switch ( result ) {
      case  0 : // Направление подходит
        sense = tda[i].sense;
        return tda[i].idx;

      case -1 : // Шаг назад
      case -2 : // Отказ
        return result;
    }

    i++;
    if ( i >= count )
      i = 0;

  }  while ( true );

  return false;
}


//------------------------------------------------------------------------------
// Перестановка узлов.
// The permutation of nodes.
// ---
void SwapCrossPoints( SArray<MbCrossPoint> & crossLeft,
                      SArray<MbCrossPoint> & crossRight )
{
  SArray<MbCrossPoint> tmp;
  tmp = crossLeft;
  crossLeft = crossRight;
  crossRight = tmp;
} // SwapCrossPoints


//------------------------------------------------------------------------------
// Структура для запоминания шагов выбора контура для отката назад.
// Structure for storing the contour selection steps for rollback.
// ---
struct SaveStep {
  const MbCurve * curve;  // Селектированная кривая. // Selected curve.
  double          tProj;  // Параметр в узле. // Parameter in the node.
  int              sense;  // Признак совпадения направления. // // Sign of matching direction.
};


//------------------------------------------------------------------------------
// Построение контура по стрелке.
// Building a contour on the arrow.
// ---
MbContour * HandEnvelopeContour( List <MbCurve> &curveList, MbCartPoint &insidePnt ) {

  // Нахождение ближайшей к точке кривой
  const MbCurve * selectCurve = FindNearestCurve( curveList, insidePnt );

  if ( selectCurve == nullptr )
    return nullptr;

  MbContour *contour = new MbContour();
  SArray <MbCrossPoint> crossRight; // Точки пересечения справа от точки проекции
  SArray <MbCrossPoint> cross;      // Точки пересечения

  // Ищем точки пересечения ближайшей кривой с остальными кривыми списка
  LIterator <MbCurve> firstCurve = curveList;
  IntersectWithAll( selectCurve, firstCurve, cross, true );

  // Функция нахождения первого сегмента контура и узла
  if ( !BeginEnvelopeContour( insidePnt, selectCurve, cross, *contour, crossRight ) ) {
    ::DeleteItem( contour );
    return nullptr;
  }

  const MbCurve * pSegment = contour->GetSegment( 0 );  // Выдать начальный сегмент контура
  TestVariables::drawImp->DrawItem( pSegment, 0, 0, 0, TestVariables::viewManager->GetCplaneMatrixFrom() );  // Рисуем начальный сегмент

  MbCartPoint startPnt( pSegment->GetLimitPoint( 1 ) );
  MbCartPoint stopPnt( pSegment->GetLimitPoint( 2 ) );

  SArray <MbCrossPoint> crossLeft(4,4);  // Точки пересечения слева от точки проекции     //-V112
  MbPointOnCurve<MbCurve> pOnAdd;
  MbCrossPoint   pAdd;
  double tProj = selectCurve->PointProjection( startPnt );

  int sense = 1;
  int newSense;

  SaveStep stepTmp;
  SArray <SaveStep> steps;  // Шаги для отката назад

  while ( startPnt != stopPnt ) {
    bool ok       = true;
    bool forward  = true;  // Признак шага вперед

    // Выбрать подходящую кривую из узла
    ptrdiff_t indexCurve = SelectCurveFromNode( pSegment, selectCurve, crossRight, newSense );

    if ( indexCurve >= 0 || indexCurve == - 1 ) { // Нашли подходящую кривую
      if ( indexCurve == - 1 ) { // Шаг назад
        ptrdiff_t index = steps.MaxIndex();

        if ( index >= 0 ) {  // Сегментов контура больше одного
          selectCurve = steps[index].curve;
          tProj       = steps[index].tProj;
          sense       = steps[index].sense;

          ptrdiff_t countSeg = contour->GetSegmentsCount();  // Выдать количество сегментов контура

          pSegment = contour->GetSegment( countSeg - 1 );  // Выдать сегмент контура по индексу
          COLORREF col = GetSystemWindowColor();
          int R = GetRValue(col);
          int G = GetGValue(col);
          int B = GetBValue(col);
          TestVariables::drawImp->DrawItem( pSegment, R, G, B, TestVariables::viewManager->GetCplaneMatrixFrom() );  // Стираем последний сегмент

          contour->DeleteSegment( countSeg - 1 );  // Удалить сегмент контура по индексу
          pSegment = contour->GetSegment( countSeg - 2 );  // Выдать сегмент контура по индексу

          steps.RemoveInd( index );  // Удаляем последний шаг

          forward = false;
        }
        else
          ok = false;
      }
      else {
        stepTmp.curve = selectCurve;
        stepTmp.tProj = tProj;
        stepTmp.sense  = sense;

        steps.Add( stepTmp );  // Запоминаем шаг

        selectCurve  = crossRight[indexCurve].on2.curve;
        tProj = crossRight[indexCurve].on2.t;
        sense = newSense;
      }

      if ( ok ) {
        cross.Flush(); // Очистим множество точек пересечения

        // Ищем точки пересечения селектированной кривой с остальными кривыми списка
        IntersectWithAll( selectCurve, firstCurve, cross, true );

        if ( selectCurve->IsBounded() && ( !selectCurve->IsClosed() ) ) {
          // Ограниченная незамкнутая кривая
          // Добавляем в множество граничные точки ограниченной кривой
          pOnAdd.Init( selectCurve->GetTMin(), selectCurve );
          pAdd.Init( selectCurve->GetLimitPoint( 1 ), pOnAdd, pOnAdd );
          cross.Add( pAdd );

          pOnAdd.Init( selectCurve->GetTMax(), selectCurve );
          pAdd.Init( selectCurve->GetLimitPoint( 2 ), pOnAdd, pOnAdd );
          cross.Add( pAdd );
        }

        // Удаляем из множества точки совпадающие с точкой проекции
        RemoveEquPoints( tProj, cross );

        if ( cross.Count() > 0 ) {
          // Сортировка точек пересечения по отношению точки проекции
          SortCrossPoints( tProj, selectCurve, cross, crossLeft, crossRight );

          if ( sense != 1 ) {
            if ( crossRight.Count() > 0 ) {
              if ( fabs( crossRight[0].on1.t - tProj ) > FLT_EPSILON )
                SwapCrossPoints( crossLeft, crossRight ); // Меняем местами узлы
            }
            else {
              SwapCrossPoints( crossLeft, crossRight ); // Меняем местами узлы
            }
          }

          if ( crossRight.Count() > 0 ) {
            if ( forward )
              pSegment = contour->AddSegment( selectCurve,
                                              tProj,
                                              crossRight[0].on1.t, sense );
            TestVariables::drawImp->DrawItem( pSegment, 0, 0, 0, TestVariables::viewManager->GetCplaneMatrixFrom() );  // Рисуем очередной сегмент

            if ( pSegment != nullptr ) // Новый сегмент может и не создаться
              stopPnt = pSegment->GetLimitPoint( 2 );
            else
              stopPnt = startPnt; // ?
          }
          else
            stopPnt = startPnt;
        }
        else
          stopPnt = startPnt;
      }
      else
        stopPnt = startPnt;
    }
    else
      stopPnt = startPnt;
  }

  return contour;
}


//------------------------------------------------------------------------------
// Построение охватывающего контура или выбор по стрелке.
// Building a covering loop or selection of the arrow.
// ---
bool CreateEnvelopeContour( int/*WPARAM*/ wParam ) {
  MbCartPoint point;
  MbItem * obj( nullptr );
  MbContour * HandEnvelopeContour( List <MbCurve> &curveList, MbCartPoint &insidePnt );

  if ( GetPoint(IDS_SELECT_OBJ_ON_PLANE, point ) &&
      (FindNearestObj( point, pt_PlaneItem, &obj )) ) {
    List<MbCurve> curveList( false );
    RPArray<MbCurve> curveArray( 10,10 );

    // Набираем все кривые из модели, которые находятся на той же плоскости, что и obj.
    // Переводим в систему координат вида.
    MbPlacement3D place;
    obj->GetPlacement( place ); // Запоминаем ЛСК.
    TestVariables::viewManager->CreateCurveArray( obj, curveArray, true );
    ::AddRefItems( curveArray );
    for ( ptrdiff_t j = 0; j < (ptrdiff_t)curveArray.Count(); j++ )
      curveList.Add(curveArray[j]);

    MbContour * contour = nullptr;

    switch ( wParam ) {
      case CM_NEWENVELOPCONTOUR : // Построение охватывающего контура
//        contour = EnvelopeContour( curveList, points[0] );
//        break;

      case CM_HANDENVELOPE  : // Выбор по стрелке
        contour = HandEnvelopeContour( curveList, point );
        break;
    }

    if ( contour != nullptr )
      TestVariables::viewManager->AddObject( TestVariables::CONTOUR_Style, contour, &place );

    ::ReleaseItems( curveArray );
    return true;
  }

  return false;
}

