////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Операции с кривой в двумерном пространстве. Удаление части кривой.
         \en Operations with a curve in two-dimensional space. Deletion of a curve piece. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ALG_CURVE_DELETE_PART_H
#define __ALG_CURVE_DELETE_PART_H

#include <curve.h>
#include <templ_s_list.h>


//------------------------------------------------------------------------------
/** \brief \ru Удалить часть кривой.
           \en Delete the piece of a curve. \~
  \details \ru Удалить часть кривой по отношению к точке.\n
    У кривой удаляется часть, 
    ограниченная двумя последовательными параметрами пересечения ее с кривыми из заданного списка,
    ближайшая к проекции заданной точки.
           \en Delete a piece of a curve by the point.\n
    Delete the piece 
    bounded by two successive parameters of intersection of the curve with the curves from the given list
    and the nearest to the projection of the given point. \~
  \param[in]      curveList - \ru Список кривых для пересечения.
                              \en The list of curves for intersection. \~
  \param[in]      pnt       - \ru Точка, показывающая удаляемую часть кривой.
                              \en The point indicating the piece of a curve to be deleted. \~
  \param[in, out] curve     - \ru Изменяемая кривая.
                              \en The curve to be modified. \~
  \param[out]     part2     - \ru Конечный участок измененной кривой, если кривая распалась на две части.
                              \en The finite piece of the modified curve if the curve is split into two pieces. \~
  \return \ru Состояние кривой после ее модификации.
          \en The state of a curve after modification. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MbeState) DeleteCurvePart(       List<MbCurve> & curveList,
                                      const MbCartPoint &   pnt,
                                            MbCurve *       curve, 
                                            MbCurve *&      part2 );


//------------------------------------------------------------------------------
/** \brief \ru Удалить часть кривой.
           \en Delete the piece of a curve. \~
  \details \ru Удалить часть кривой по двум точкам.\n
    Для замкнутых кривых дополнительно задается третья точка,
    которая показывает удаляемую часть.
           \en Delete the piece of a curve by two points.\n
    The third point is additionally specified for closed curves,
    it indicates the piece of a curve to be deleted. \~
  \param[in]      p1 - \ru Точка, показывающая первую границу удаляемого участка.
                       \en The point indicating the first boundary of the piece to be deleted. \~
  \param[in]      p2 - \ru Точка, показывающая вторую границу удаляемого участка.
                       \en The point indicating the second boundary of the piece to be deleted. \~
  \param[in]      p3 - \ru Точка, показывающая удаляемую часть замкнутой кривой.
                       \en The point indicating the piece of a closed curve to be deleted. \~
  \param[in, out] curve - \ru Изменяемая кривая.
                          \en The curve to be modified. \~
  \param[out]     part2 - \ru Конечный участок измененной кривой, если кривая распалась на две части.
                          \en The finite piece of the modified curve if the curve is split into two pieces. \~
  \return \ru Состояние кривой после ее модификации.
          \en The state of a curve after modification. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MbeState) DeleteCurvePart( const MbCartPoint & p1,
                                      const MbCartPoint & p2,
                                      const MbCartPoint & p3,
                                            MbCurve *     curve, 
                                            MbCurve *&    part2 );


//------------------------------------------------------------------------------
/** \brief \ru Оставить часть кривой.
           \en Keep the piece of a curve. \~
  \details \ru Оставить часть кривой по отношению к точке.
    У кривой оставляется часть, 
    ограниченная двумя последовательными параметрами пересечения ее с кривыми из заданного списка,
    ближайшая к проекции заданной точки.
           \en Keep the piece of a curve by the point.
    Keep the curve piece 
    bounded by two successive parameters of intersection of the initial curve with the curves from the given list
    and the nearest to the projection of the given point. \~
  \param[in]      curveList - \ru Список кривых для пересечения.
                              \en The list of curves for intersection. \~
  \param[in]      pnt       - \ru Точка, показывающая оставляемую часть кривой.
                              \en The point indicating the piece of a curve to be kept. \~
  \param[in, out] curve     - \ru Изменяемая кривая.
                              \en The curve to be modified. \~
  \param[in, out] part2     - \ru Всегда nullptr.
                              \en This value is always nullptr. \~
  \return \ru Состояние кривой после ее модификации.
          \en The state of a curve after its modification. \~
  \warning \ru Для внутреннего использования.
           \en For internal use only. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MbeState) TrimmCurvePart(       List<MbCurve> & curveList,
                                     const MbCartPoint &   pnt,
                                           MbCurve *       curve, 
                                           MbCurve *&      part2 );


//------------------------------------------------------------------------------
/** \brief \ru Оставить часть кривой.
           \en Keep the piece of a curve. \~
  \details \ru Оставить часть кривой по двум точкам.\n
    Для замкнутых кривых дополнительно задается третья точка,
    которая показывает оставляемую часть.
           \en Keep the piece of a curve by two points.\n
    The third point is additionally specified for closed curves,
    it indicates the piece of a curve to be kept. \~
  \param[in]      p1    - \ru Точка, показывающая первую границу удаляемого участка.
                          \en The point indicating the first boundary of the piece to be deleted. \~
  \param[in]      p2    - \ru Точка, показывающая вторую границу удаляемого участка.
                          \en The point indicating the second boundary of the piece to be deleted. \~
  \param[in]      p3    - \ru Точка, показывающая оставляемую часть замкнутой кривой.
                          \en The point indicating the piece of a closed curve to be kept \~
  \param[in, out] curve - \ru Изменяемая кривая.
                          \en The curve to be modified. \~
  \param[in, out] part2 - \ru Всегда nullptr.
                          \en This value is always nullptr. \~
  \return \ru Состояние кривой после ее модификации.
          \en The state of a curve after its modification. \~
  \warning \ru Для внутреннего использования.
           \en For internal use only. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MbeState) TrimmCurvePart( const MbCartPoint & p1,
                                     const MbCartPoint & p2,
                                     const MbCartPoint & p3,
                                           MbCurve *     curve, 
                                           MbCurve *&    part2 );


//------------------------------------------------------------------------------
/** \brief \ru Выровнять кривую.
           \en Justify the curve. \~
  \details \ru Выровнять кривую по отношению к заданной кривой и точке на кривой.\n
    Кривая усекается точкой пересечения ее с граничной кривой, ближайшей к заданной
    точке. Остается часть кривой со стороны указанной точки.
           \en Justify the curve relative to the given curve and a point on the curve.\n
    The curve is truncated by the point of its intersection with the boundary curve, which is the nearest to the given
    point. Only the piece of a curve at the side of the given point is kept. \~
  \param[in, out] curve      - \ru Изменяемая кривая.
                               \en The modified curve. \~
  \param[in]      limitCurve - \ru Граничная кривая для выравнивания.
                               \en Boundary curve for justification. \~
  \param[in]      pnt        - \ru Точка для выбора нужной части кривой.
                               \en The point for selecting the piece of a curve. \~
  \param[in, out] part2      - \ru Всегда nullptr.
                               \en This value is always nullptr. \~
  \return \ru Состояние кривой после ее модификации.
          \en The state of a curve after modification. \~
  \warning \ru Для внутреннего использования.
           \en For internal use only. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MbeState) JustifyCurve(       MbCurve *     curve, 
                                   const MbCurve *     limitCurve,
                                   const MbCartPoint & pnt, 
                                         MbCurve *&    part2 );


//------------------------------------------------------------------------------
/** \brief \ru Положение точки.
           \en The position of the point. \~
  \details \ru Положение точки относительно замкнутых границ.
           \en The position of the point relative to closed borders. \~
  \param[in] limits - \ru Набор кривых, задающий границы.
                      В совокупности должен представлять собой замкнутые границы.
                      \en The set of curves that defines boundaries.
                      These curves should be closed boundaries in the aggregate. \~
  \param[in] pnt    - \ru Точка для определения положения.
                      \en The point for the position definition. \~
  \return \ru Положение точки относительно кривой.
          \en The point position relative to the curve. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MbeLocation) PointLocation( const RPArray<const MbCurve> & limits, 
                                       const MbCartPoint & pnt );


//------------------------------------------------------------------------------
/** \brief \ru Выкинуть части кривой.
           \en Exclude the piece of a curve. \~
  \details \ru Выкинуть части кривой, попадающие в замкнутые границы.
           \en Exclude curve pieces from closed boundaries. \~
  \param[in]  curve        - \ru Кривая, на которую накладываются границы.
                             \en The bounded curve. \~
  \param[in]  limits       - \ru Множество замкнутых кривых-границ.
                             \en The array of closed curves-boundaries. \~
  \param[in]  inside       - \ru Признак удаления внутри границ.
                             \en The attribute of deletion inside boundaries. \~
  \param[out] part2        - \ru Множество оставшихся участков кривой.
                             \en The array of remaining curve pieces. \~
  \param[out] cross        - \ru Точки пересечения кривой с границами.
                             \en The curve and boundaries intersection point. \~
  \param[out] isEqualCurve - \ru Признак совпадения разбиваемой кривой с какой-то из
                             присланного массива границ. Имеет смысл при результате dp_NoChanged.
                             \en The attribute of coincidence between the broken curve with one of
                             the given array of boundaries. It is worthwhile if the result is dp_NoChanged. \~
  \param[in]  cutOnCurve   - \ru Если false, не удаляются части кривой,
                             совпадающие с участками границы.
                             \en If it equals to false, then the pieces of a curve
                             coincident with pieces of the boundary are not to be deleted. \~
  \return \ru Состояние кривой после ее модификации.
          \en The state of a curve after modification. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MbeState) BreakByClosedCurves(       MbCurve &                curve,
                                          const RPArray<const MbCurve> & limits,
                                                bool                     inside,
                                                PArray<MbCurve> &        part2,
                                                SArray<MbCrossPoint> *   cross = nullptr,
                                                bool *                   isEqualCurve = nullptr,
                                                bool                     cutOnCurve = false );


//------------------------------------------------------------------------------
/** \brief \ru Выкинуть части кривой.
           \en Exclude the piece of a curve. \~
  \details \ru Выкинуть части кривой, совпадающие с набором кривых.
           \en Exclude pieces of a curve by coincidence with curves from a set. \~
  \param[in]  curve  - \ru Кривая, на которую накладываются границы.
                       \en The bounded curve. \~
  \param[in]  limits - \ru Множество кривых для тестирования попадания.
                       \en The array of curves for the hit testing. \~
  \param[out] part2  - \ru Множество оставшихся кусков кривой.
                       \en The array of remaining curve pieces. \~
  \return \ru Состояние кривой после ее модификации.
          \en The state of a curve after modification. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MbeState) BreakByCurvesArr(       MbCurve &                curve,
                                       const RPArray<const MbCurve> & limits,
                                             PArray<MbCurve> &        part2 );


//------------------------------------------------------------------------------
/** \brief \ru Разбить кривую.
           \en Split the curve. \~
  \details \ru Разбить кривую на две части.\n
    В результате кривая разбивается на части, первая часть которой остается в curve,
    остальные части складываются в массив part2.
           \en Split the curve into two pieces.
    In result the curve is split into pieces. The first piece remains,
    the other pieces are added to the array part2. \~
  \return \ru Состояние кривой после ее модификации.
          \en The state of a curve after modification. \~
  \param[in, out] curve - \ru Разбиваемая кривая.
                          \en The curve for splitting. \~
  \param[in]      p1    - \ru Первая точка разбиения.
                          \en The first point of splitting. \~
  \param[in]      p2    - \ru Вторая точка разбиения.
                          \en The second point of splitting. \~
  \param[out]     part2 - \ru Множество частей кривой.
                          \en The array of curve pieces. \~
  \return \ru Состояние кривой после ее модификации.
          \en The state of a curve after modification. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MbeState) BreakCurve(       MbCurve &         curve, 
                                 const MbCartPoint &     p1, 
                                 const MbCartPoint &     p2,
                                       PArray<MbCurve> & part2 );


//------------------------------------------------------------------------------
/** \brief \ru Разбить кривую..
           \en Split the curve. \~
  \details \ru Разбить кривую на несколько равных частей.
           \en Split the curve by several equal pieces. \~
  \param[in, out] curve - \ru Разбиваемая кривая.
                          \en The curve for splitting. \~
  \param[in]      partsCount - \ru Количество частей.
                               \en The count of pieces. \~
  \param[in]      p1         - \ru Одна из точек разбиения.
                               \en One of splitting points. \~ 
  \param[out]     part2      - \ru Множество частей кривой.
                               \en The array of curve pieces. \~
  \return \ru Состояние кривой после ее модификации.
          \en The state of a curve after modification. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MbeState) BreakCurveNParts( MbCurve & curve, ptrdiff_t partsCount, const MbCartPoint & p1,
                                       PArray<MbCurve> & part2 );


//------------------------------------------------------------------------------
/** \brief \ru Удлинить кривую.
           \en Extend the curve. \~
  \details \ru Удлинить кривую curve до кривой-границы limitCurve с конца ближайшего к точке pnt
           \en Extend the curve to the given curve-boundary limitCurve from the end nearest to the given point pnt. \~
  \param[in, out] curve      - \ru Изменяемая кривая.
                               \en The modified curve. \~
  \param[in]      limitCurve - \ru Кривая-граница.
                               \en The curve-boundary \~
  \param[in]      pnt        - \ru Точка, показывающая удлиняемый конец кривой.
                               \en The point indicating the extended end of a curve. \~   
  \return \ru Состояние кривой после ее модификации.
          \en The state of a curve after modification. \~ 
  \ingroup Algorithms_2D
*/ 
// ---
MATH_FUNC (MbeState) ExtendCurveToCurve(       MbCurve *     curve, 
                                         const MbCurve *     limitCurve, 
                                         const MbCartPoint & pnt );


#endif // __ALG_CURVE_DELETE_PART_H
