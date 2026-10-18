////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Операции с кривыми в двумерном пространстве.
         \en Operations with curves in two-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ALG_CURVE_ENVELOPE_H
#define __ALG_CURVE_ENVELOPE_H

#include <cur_contour.h>
#include <templ_s_list.h>


//------------------------------------------------------------------------------
/** \brief \ru Найти первый сегмент контура.
           \en Find the first segment of a contour. \~
  \details \ru Найти первый сегмент контура.\n
    В контур добавляется часть кривой selectCurve между параметрами пересечение,
    ближайшими к проекции точки insidePoint.
           \en Find the first segment of a contour.\n
    A piece of the curve "selectCurve" between intersection parameters is added in contour,
    the parameters are the nearest to the projection of the point "insidePoint". \~
  \param[in]  insidePnt   - \ru Точка, вокруг которой надо построить контур.
                            \en The point around which to create the contour. \~
  \param[in]  selectCurve - \ru Ближайшая кривая.
                            \en The nearest curve. \~
  \param[in]  cross       - \ru Множество точек пересечения ближайшей кривой.
                            \en The array of points of intersection with the nearest curve. \~
  \param[out] contour     - \ru Контур для добавления сегмента.
                            \en The contour for adding the segment to. \~
  \param[out] crossRight  - \ru Узел - массив точек пересечения кривой в сторону продолжения контура.
                            \en The node - the array of intersection points in the side of contour extension. \~
  \return \ru true, если сегмент добавлен.
          \en true if a segment has been added. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (bool) BeginEnvelopeContour( const MbCartPoint &          insidePnt, 
                                       const MbCurve *              selectCurve,
                                             SArray<MbCrossPoint> & cross,
                                             MbContour &            contour,
                                             SArray<MbCrossPoint> & crossRight );


//------------------------------------------------------------------------------
/** \brief \ru Найти ближайшую кривую.
           \en Find the nearest curve. \~
  \details \ru Найти ближайшую к точке кривую.
           \en Find the curve nearest to a point. \~
  \param[in] curveList - \ru Список кривых.
                         \en The list of curves. \~
  \param[in] pnt       - \ru Точка.
                         \en The point. \~
  \return \ru Ближайшую кривую.
          \en The nearest curve. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MbCurve *) FindNearestCurve( List<MbCurve> & curveList, const MbCartPoint & pnt );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки пересечения.
           \en Find intersection points. \~
  \details \ru Найти точки пересечения выбранной кривой с
    остальными кривыми списка от кривой включительно.
           \en Find intersection points of the chosen curve with
    the other curves from the list. \~
  \param[in]  selectCurve - \ru Кривая.
                            \en The curve. \~
  \param[in]  fromCurve   - \ru Итератор списка кривых.
                            \en The iterator of the curves list. \~
  \param[out] cross       - \ru Точки пересечения.
                            \en Intersection points. \~
  \param[in]  self        - \ru Флаг поиска самопересечений.
                            \en Flag of self-intersections search. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (void) IntersectWithAll( const MbCurve *              selectCurve,
                                         LIterator<MbCurve> &   fromCurve,
                                         SArray<MbCrossPoint> & cross, 
                                         bool                   self );


//------------------------------------------------------------------------------
/** \brief \ru Сортировать точки пересечения.
           \en Sort intersection points. \~
  \details \ru Сортировать точки пересечения по отношению к точки проекции
    выбранной кривой.
           \en Sort intersection points relative to the projection point
    of the chosen curve. \~
  \param[in]      tProj       - \ru Параметр проекции на кривую.
                                \en Parameter of the projection on the curve. \~
  \param[in]      selectCurve - \ru Кривая.
                                \en The curve. \~
  \param[in, out] cross       - \ru Множество точек пересечения для сортировки.
                                \en The array of intersection points for sorting. \~
  \param[out]     crossLeft   - \ru Узел точек пересечения слева от проекции.
                                \en The node of intersection points on the left of the projection. \~
  \param[out]     crossRight  - \ru Узел точек пересечения справа от проекции.
                                \en The node of intersection points on the right of the projection. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (void) SortCrossPoints( double tProj, const MbCurve * selectCurve,
                                  SArray<MbCrossPoint> & cross,
                                  SArray<MbCrossPoint> & crossLeft,
                                  SArray<MbCrossPoint> & crossRight );


//------------------------------------------------------------------------------
/** \brief \ru Удалить совпадающие точки.
           \en Delete coincident points. \~
  \details \ru Удалить из массива точки совпадающие с точкой проекции, заданной параметром.\n
    Если все точки совпадают с точкой проекции, то они не будут удалены.
           \en Delete points from the array which are coincident with the projection point specified by the parameter.\n
    If all the points are coincident with the projection point, then they will not be deleted. \~
  \param[in]      tProj - \ru Параметр проекции.
                          \en The projection parameter. \~
  \param[in, out] cross - \ru Множество точек пересечения.
                          \en The array of intersection points. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (void) RemoveEquPoints( double tProj, SArray<MbCrossPoint> & cross );


#endif // __ALG_CURVE_ENVELOPE_H
