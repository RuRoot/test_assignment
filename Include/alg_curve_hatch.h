////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Пересечение кривых в двумерном пространстве для штриховки.
         \en Intersection of curves in two-dimensional space for hatching. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ALG_CURVE_HATCH_H 
#define __ALG_CURVE_HATCH_H 

#include <curve.h>


//------------------------------------------------------------------------------
/** \brief \ru Найти пересечение с горизонтальной прямой.
           \en Find intersection with horizontal line. \~
  \details \ru Найти пересечение кривой с горизонтальной прямой.
    Для штриховки.
           \en Find intersection of a curve with horizontal line.
    For hatching. \~
  \param[in]  y              - \ru Координата у горизонтальной прямой.
                               \en The coordinate of a horizontal line. \~
  \param[in]  curve          - \ru Кривая.
                               \en The curve. \~
  \param[out] crossPnt       - \ru Точки пересечения.
                               \en Intersection points. \~
  \param[in] skipTouchPoints - \ru Пропускать точки касания. true - пропускаем точки касания, false - старое поведение функции.
                               \en Skip touch points. true - skip touch points, false - old function behavior. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (void) HatchIntersectLine( double y, const MbCurve * curve, SArray<MbCartPoint> & crossPnt, bool skipTouchPoints = false );


//------------------------------------------------------------------------------
/** \brief \ru Найти пересечение с окружностью.
           \en Find intersection with a circle. \~
  \details \ru Найти пересечение с окружностью.\n
    Для штриховки.
           \en Find intersection with a circle.\n
    For hatching. \~
  \param[in]  circle   - \ru Окружность.
                         \en The circle. \~
  \param[in]  curve    - \ru Кривая.
                         \en The curve. \~
  \param[out] crossPnt - \ru Точки пересечения.
                         \en Intersection points. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (void) HatchIntersectCircle( const MbCurve * circle, const MbCurve * curve, SArray<MbCartPoint> & crossPnt );


#endif // __ALG_CURVE_HATCH_H 
