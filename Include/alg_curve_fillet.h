////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение скругления, фаски между двумя кривыми в двумерном пространстве.
         \en Construction of fillet or chamfer between two curves in two-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ALG_CURVE_FILLET_H 
#define __ALG_CURVE_FILLET_H 

#include <curve.h>


class  MbLineSegment;
class  MbArc;
class  MbContour;


//------------------------------------------------------------------------------
/** \brief \ru Построить скругление между кривыми.
           \en Construct fillet between curves. \~
  \details \ru Построить скругление постоянным радиусом между двумя кривыми.
           \en Construct fillet with a constant radius between two curves. \~
  \param[in]  curve1 - \ru Первая кривая.
                       \en The first curve. \~
  \param[in]  pnt1   - \ru Точка вблизи первой кривой.
                       \en The point near the first curve. \~
  \param[in]  trim1  - \ru Признак усечения первой кривой.
                       \en The attribute of trimming of the first curve. \~
  \param[in]  curve2 - \ru Вторая кривая.
                       \en The second curve. \~
  \param[in]  pnt2   - \ru Точка вблизи второй кривой.
                       \en The point near the second curve. \~
  \param[in]  trim2  - \ru Признак усечения второй кривой.
                       \en The attribute of trimming of the second curve. \~
  \param[in]  rad    - \ru Радиус скругления.
                       \en The radius of fillet. \~
  \param[out] state1 - \ru Состояние первой кривой.
                       \en The state of the first curve. \~
  \param[out] state2 - \ru Состояние второй кривой.
                       \en The state of the second curve. \~
  \param[out] arc    - \ru Дуга скругления.
                       \en The arc of fillet. \~  
  \return \ru true в случае успешной операции.
          \en true in case of successful operation. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (bool) Fillet( MbCurve *  curve1, const MbCartPoint & pnt1, bool trim1,
                         MbCurve *  curve2, const MbCartPoint & pnt2, bool trim2,
                         double     rad,
                         MbeState & state1, 
                         MbeState & state2, 
                         MbArc *&   arc );


//------------------------------------------------------------------------------
/** \brief \ru Построить фаску.
           \en Construct a chamfer. \~
  \details \ru Построить фаску между двумя кривыми.
           \en Construct a chamfer between two curves. \~
  \param[in]  curve1  - \ru Первая кривая.
                        \en The first curve. \~
  \param[in]  pnt1    - \ru Точка вблизи первой кривой.
                        \en The point near the first curve. \~
  \param[in]  trim1   - \ru Признак усечения первой кривой.
                        \en The attribute of trimming of the first curve. \~
  \param[in]  curve2  - \ru Вторая кривая.
                        \en The second curve. \~
  \param[in]  pnt2    - \ru Точка вблизи второй кривой.
                        \en The point near the second curve. \~
  \param[in]  trim2   - \ru Признак усечения второй кривой.
                        \en The attribute of trimming of the second curve. \~
  \param[in]  len     - \ru Размер фаски на первой кривой.
                        \en The size of the chamfer on the first curve. \~
  \param[in]  angle   - \ru Угол фаски или размер фаски на второй кривой в зависимости от типа построения.
                        \en The angle of the chamfer or the size of the chamfer on the second curve according to the type of construction. \~
  \param[in]  type    - \ru Тип построения фаски:\n
                        true - размер + угол,\n
                        false - размер + размер.
                        \en The type of chamfer construction:\n
                        true - size + angle,\n
                        false - size + size. \~
  \param[out] state1  - \ru Состояние первой кривой.
                        \en The state of the first curve. \~
  \param[out] state2  - \ru Состояние второй кривой.
                        \en The state of the second curve. \~
  \param[out] lineseg - \ru Отрезок фаски.
                        \en The segment of the chamfer. \~  
  \return \ru true в случае успешной операции.
          \en is true in case of successful operation. \~ 
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (bool) Chamfer( MbCurve *        curve1, const MbCartPoint & pnt1, bool trim1,
                          MbCurve *        curve2, const MbCartPoint & pnt2, bool trim2,
                          double           len, 
                          double           angle, 
                          bool             type,
                          MbeState &       state1, 
                          MbeState &       state2, 
                          MbLineSegment *& lineseg );


//------------------------------------------------------------------------------
/** \brief \ru Построить скругление.
           \en Construct a fillet. \~
  \details \ru Построить скругление полилинии или контура.\n
    Изменяемая кривая mc должна быть полилинией или контуром.
           \en Construct a fillet of polyline or contour.\n
    The curve "mc" being modified should be a polyline or a contour. \~
  \param[in]  mc       - \ru Изменяемая кривая.
                         \en The modified curve. \~
  \param[in]  rad      - \ru Радиус скругления.
                         \en The radius of fillet. \~
  \param[in]  nodeFlag - \ru Флаг выбора узлов скругления:\n
                         true - скругление во всех узлах,\n
                         false - скругление ближайшего узла.
                         \en The flag of selection of fillet nodes.\n
                         true - fillet at all nodes,\n
                         false - fillet of the nearest node. \~
  \param[in]  pnt      - \ru Точка для выбора ближайшего узла.
                         \en The point of choosing of the nearest node. \~
  \param[out] contour  - \ru Контур, построенный по полилинии.
                         \en Contour constructed by a polyline. \~
  \return \ru Состояние кривой после её модификации.
          \en The state of a curve after modification. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MbeState) FilletPolyContour( MbCurve * mc, double rad, bool nodeFlag,
                                        const MbCartPoint & pnt, MbContour *& contour );


//------------------------------------------------------------------------------
/** \brief \ru Построить фаску.
           \en Construct a chamfer. \~
  \details \ru Построить фаску полилинии или контура.\n
    Изменяемая кривая mc должна быть полилинией или контуром.
           \en Construct a chamfer of polyline or contour.\n
    The curve "mc" being modified should be a polyline or a contour. \~
  \param[in] mc              - \ru Изменяемая кривая.
                               \en The modified curve. \~
  \param[in] l1              - \ru Размер фаски.
                               \en The size of a chamfer. \~
  \param[in] par             - \ru Угол фаски или размер фаски в зависимости от типа построения.
                               \en The angle of a chamfer or the size of a chamfer on the second curve according to the type of construction. \~
  \param[in] chamferTypeFlag - \ru Тип построения фаски:\n
                               true - размер + угол,\n
                               false - размер + размер.
                               \en The type of chamfer construction:\n
                               true - size + angle,\n
                               false - size + size. \~
  \param[in] nodeFlag        - \ru Флаг выбора узлов скругления:\n
                               true - скругление во всех узлах,\n
                               false - скругление ближайшего узла.
                               \en The flag of selection of fillet nodes.\n
                               true - fillet at all nodes,\n
                               false - fillet of the nearest node. \~
  \param[in] pnt             - \ru Точка для выбора ближайшего узла.
                               \en The point of choosing of the nearest node. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (bool) ChamferPolyContour( MbCurve * mc, double l1, double par,
                                     bool chamferTypeFlag, bool nodeFlag,
                                     const MbCartPoint & pnt );


//------------------------------------------------------------------------------
/** \brief \ru Продлить кривые.
           \en Extend curves. \~
  \details \ru Продлить две кривые до точки пересечения.
           \en Extend two curves to the point of intersection. \~
  \param[in, out] crv1 - \ru Первая кривая
                         \en The first curve. \~
  \param[in, out] crv2 - \ru Вторая кривая.
                         \en The second curve. \~
  \param[in]      p1   - \ru Точка для выбора места пересечения.
                         \en The point for selection of intersection location. \~
  \param[in]      p2   - \ru Точка для выбора места пересечения.
                         \en The point for selection of intersection location. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (bool) Corner( MbCurve * crv1, MbCurve * crv2,
                         const MbCartPoint & p1, const MbCartPoint & p2 );


//------------------------------------------------------------------------------
// Скругление двух последовательных кривых curve1 и curve2 радиусом rad.
// Предполагается, что конец кривой curve1 совпадает с началом кривой curve2.
// ---
MbArc * FilletTwoCurves( const MbCurve & curve1,
                         const MbCurve & curve2,
                               double    rad,
                               double &  tCross1,   // параметр, соответствующий точке касания curve1 и дуги окружности
                               double &  tCross2 ); // параметр, соответствующий точке касания curve2 и дуги окружности


#endif // __ALG_CURVE_FILLET_H 
