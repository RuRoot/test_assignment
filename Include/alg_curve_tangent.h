////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение прямой.
         \en Construction of line. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ALG_CURVE_TANGENT_H
#define __ALG_CURVE_TANGENT_H

#include <cur_line.h>


//------------------------------------------------------------------------------
/** \brief \ru Построить касательные прямые.
           \en Construct a line. \~
  \details \ru Построить все возможные прямые через точку касательно данной кривой.\n
    Базовая точка прямой совпадает с точкой касания.
           \en Construct a line passing through a point and tangent to a given curve.\n
    A line origin is coincident with a tangency point. \~
  \param[in]  pnt    - \ru Точка, через которую проходит прямая.
                       \en The point which the line passing through. \~
  \param[in]  pCurve - \ru Кривая, которой должна касаться построенная прямая.
                       \en The curve which the constructed line should be tangent to. \~ 
  \param[out] pLine  - \ru Набор прямых.
                       \en The set of lines. \~
  \param[in]  lineAsCurve - \ru Обрабатывать прямую, ломаную и отрезок как кривую в общем случае.
                            \en Work with MbLline, MbPolyline, MbLineSegment as with MbCurve. \~ 
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) LinePointTangentCurve( const MbCartPoint & pnt, const MbCurve & pCurve,
                                        PArray<MbLine> & pLine, bool lineAsCurve = false );


//------------------------------------------------------------------------------
/** \brief \ru Построить прямые под углом.
           \en Construct lines passing at angle. \~
  \details \ru Построить прямые, проходящие под углом angle к оси 0X и касательные к кривой.\n
    Базовая точка прямой совпадает с точкой касания.
           \en Construct lines at angle "angle" to the axis OX and tangent to the curve.\n
    A line origin is coincident with a tangency point. \~
  \param[in]  angle  - \ru Угол к оси абсцисс.
                       \en The angle to the abscissa axis. \~
  \param[in]  pCurve - \ru Кривая, которой должна касаться построенная прямая.
                       \en The curve which the constructed line should be tangent to. \~ 
  \param[out] pLine  - \ru Набор прямых.
                       \en The set of lines. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) LineAngleTangentCurve( double angle, const MbCurve & pCurve,
                                        PArray<MbLine> & pLine );


//------------------------------------------------------------------------------
/** \brief \ru Построить прямые, касательные к окружностям.
           \en Construct lines tangent to circles. \~
  \details \ru Построить прямые, касательные к двум окружностям,
    заданным центрами и радиусами.\n
    Базовая точка прямой совпадает с точкой касания первой окружности.
    Функция строит от 0 до 4 прямых.
           \en Construct lines tangent to two circles.
    with given centers and radii.\n
    A line origin is coincident with a point of tangency with the first circle.
    Function constructs from 0 to 4 variables. \~
  \param[in]  centre1 - \ru Центр первой окружности.
                        \en The center of the first circle. \~
  \param[in]  radius1 - \ru Радиус первой окружности.
                        \en The radius of the first circle. \~
  \param[in]  centre2 - \ru Центр второй окружности.
                        \en The center of the second circle. \~
  \param[in]  radius2 - \ru Радиус второй окружности.
                        \en The radius of the second circle. \~
  \param[out] pl      - \ru Результат - массив прямых.
                        \en The result - the array of lines. \~
  \param[out] sp      - \ru Множество точек касания на второй кривой.
                        \en The array of tangency points on the second curve. \~
  \return \ru Количество прямых в массиве pl, 
    равное количеству базовых точек в массиве sp.
          \en The number of lines in array "pl" 
    that is equal to the number of the base points in array "sp". \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (ptrdiff_t) LineTan2Circles( const MbCartPoint & centre1, double radius1,
                                       const MbCartPoint & centre2, double radius2,
                                       MbLine * pl, MbCartPoint * sp ); 


//------------------------------------------------------------------------------
/** \brief \ru Построить касательную прямую.
           \en Construct a tangent line. \~
  \details \ru Построить прямую, касательную двум кривым.\n
    Базовая точка прямой совпадает с точкой касания первой кривой.
           \en Construct a line tangent to two curves.\n
    A line origin is coincident with a point of tangency on the first curve. \~
  \param[in]  pCurve1   - \ru Первая кривая, которой должна касаться построенная прямая.
                          \en The first curve which the constructed line should be tangent to. \~ 
  \param[in]  pCurve2   - \ru Вторая кривая, которой должна касаться построенная прямая.
                          \en The second curve the constructed line should be tangent to. \~ 
  \param[out] pLine     - \ru Результат - массив прямых.
                          \en The result - the array of lines. \~
  \param[out] secodnPnt - \ru Множество точек касания на второй кривой.
                          \en The array of tangency points on the second curve. \~
  \return \ru Количество прямых в массиве pLine, 
    равное количеству точек в массиве secodnPnt.
          \en The number of lines in array "pline" 
    that is equal to the number of points in array "secondPnt". \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) LineTangentTwoCurves( const MbCurve * pCurve1, const MbCurve * pCurve2,
                                       PArray<MbLine> * pLine,
                                       SArray<MbCartPoint> * secodnPnt );


//------------------------------------------------------------------------------
/** \brief \ru Построить прямые под углом.
           \en Construct lines passing at angle. \~
  \details \ru Построить прямые, проходящие под углом angle к оси 0X,
    касательные к окружности, заданной центром и радиусом.
           \en Construct lines at angle "angle" to the axis OX,
    tangent to a circle with the given center and radius. \~
  \param[in]  angle  - \ru Угол.
                       \en The angle. \~
  \param[in]  centre - \ru Центр окружности.
                       \en The circle center. \~
  \param[in]  radius - \ru Радиус окружности.
                       \en The circle radius. \~
  \param[out] pLine  - \ru Результат - массив прямых.
                       \en The result - the array of lines. \~
  \return \ru Количество прямых в массиве pLine.
          \en The number of lines in array "pLine", \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (ptrdiff_t) LineAngleTanCircle( double angle, const MbCartPoint & centre, double radius, MbLine * pLine ); 


//------------------------------------------------------------------------------
/** \brief \ru Переставить прямые.
           \en Swap lines. \~
  \details \ru Переставить прямые местами.
           \en Swap lines. \~
  \param[in] l1 - \ru Первая прямая.
                  \en The first line. \~
  \param[in] l2 - \ru Вторая прямая.
                  \en The second line. \~
  \ingroup Curve_Modeling
*/
// ---
inline
void SwapLines( MbLine & l1, MbLine & l2 )
{
  std::swap( l1.SetOrigin(), l2.SetOrigin() );
  std::swap( l1.SetDirection(), l2.SetDirection() );
}


#endif // __ALG_CURVE_TANGENT_H
