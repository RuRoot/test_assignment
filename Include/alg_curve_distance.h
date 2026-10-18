////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение кривых в двумерном пространстве.
         \en Construction of curves in two-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ALG_CURVE_DISTANCE_H
#define __ALG_CURVE_DISTANCE_H


#include <mb_cart_point.h>
#include <templ_p_array.h>
#include <templ_s_array.h>


class  MbCurve;
class  MbLineSegment;
class  MbArc;
class  MbLine;
class  MbTempCircle;


//------------------------------------------------------------------------------
/** \brief \ru Построить прямую, параллельную заданной.
           \en Construct a line parallel to a given line. \~ 
  \details \ru Построить прямую, параллельную заданной через точку.
           \en Construct a line parallel to a given line and passing through a given point. \~
  \param[in]  p      - \ru Точка на прямой.
                       \en The point on the line. \~
  \param[in]  pl     - \ru Параллельная прямая.
                       \en The parallel line. \~
  \param[out] pl_par - \ru Результат - прямая, параллельная pl, через точку p.
                       \en The result - the line parallel to the line pl and passing through the point p. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) LineParallelPoint( const MbCartPoint & p, const MbLine & pl, MbLine & pl_par );


//------------------------------------------------------------------------------      
/** \brief \ru Построить прямую, параллельную заданной.
           \en Construct a line parallel to a given line. \~ 
  \details \ru Построить прямую, параллельную заданной, на расстоянии.
           \en Construct a line parallel to a given line at a given distance from it. \~
  \param[in]  delta  - \ru Расстояние до параллельной прямой.
                       \en The distance to the parallel line. \~
  \param[in]  pl     - \ru Параллельная прямая.
                       \en The parallel line. \~
  \param[out] pl_par - \ru Результат - прямая, параллельная pl, на расстоянии delta.
                       \en The result - the line parallel to the line pl at a given distance delta. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) LineParallelDistance( double delta, const MbLine & pl, MbLine & pl_par );


//------------------------------------------------------------------------------
/** \brief \ru Построить прямую через точку.
           \en Construct a line passing through a given point. \~
  \details \ru Построить прямую, проходящую через точку,
    являющуюся биссектриссой угла между прямыми.
           \en Construct a line passing through a given point
    and being a bisector of angle between two given lines. \~
  \param[in]  p   - \ru Точка на прямой.
                    \en The point on the line. \~
  \param[in]  pl1 - \ru Прямая, задающая сторону угла.
                    \en The line defining the angle side. \~
  \param[in]  pl2 - \ru Прямая, задающая сторону угла.
                    \en The line defining the angle side. \~
  \param[out] pl3 - \ru Результат.
                    \en The result. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (int) LineBisector( const MbCartPoint & p, const MbLine & pl1, const MbLine & pl2, MbLine & pl3 );


//------------------------------------------------------------------------------
/** \brief \ru Построить прямую под углом.
           \en Construct a line passing at angle. \~
  \details \ru Построить прямую под углом angle к заданной pl через точку p
           \en Construct a line at an angle to a given line and passing through a given point. \~
  \param[in]  angle  - \ru Угол.
                       \en The angle. \~
  \param[in]  p      - \ru Точка на прямой.
                       \en The point on the line. \~
  \param[in]  pl     - \ru Прямая под углом angle к построенной.
                       \en The line at the given angle to the created line. \~
  \param[out] pl_new - \ru Результат - построенная прямая.
                       \en The result - the created line. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) LinePointAngle( double angle, const MbCartPoint & p, const MbLine & pl, MbLine & pl_new );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить центр окружности.
           \en Calculate a circle center. \~   
  \details \ru Вычислить центр окружности по двум точкам и радиусу.
           \en Calculate a circle center by two points and radius. \~
  \param[in]  p1     - \ru Первая точка.
                       \en The first point. \~
  \param[in]  p2     - \ru Вторая точка.
                       \en The second point. \~
  \param[in]  radius - \ru Радиус
                       \en Radius. \~
  \param[out] circle - \ru Результат - массив временных окружностей.
                       \en The result - the array of temporary circles. \~
  \return \ru Количество элементов в массиве circle.
          \en The count of elements in the array "circle". \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (int) Circle2PointsRadius( const MbCartPoint & p1, const MbCartPoint & p2,
                                     double radius, MbTempCircle * circle );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить центр и радиус окружности.
           \en Calculate center and radius of a circle. \~
  \details \ru Вычислить центр и радиус окружности по трем точкам.
           \en Calculate center and radius of a circle by three points. \~
  \param[in]  p1     - \ru Первая точка.
                       \en The first point. \~
  \param[in]  p2     - \ru Вторая точка.
                       \en The second point. \~
  \param[in]  p3     - \ru Третья точка.
                       \en The third point. \~
  \param[out] centre - \ru Результат - центр окружности.
                       \en The result - the circle center. \~
  \return \ru true в случае возможности построения.
          \en true if the construction is possible. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (bool) CircleCentre3Points( const MbCartPoint & p1,
                                      const MbCartPoint & p2,
                                      const MbCartPoint & p3, MbCartPoint & centre );

                               
//------------------------------------------------------------------------------
/** \brief \ru Построить эллипс
           \en Construct an ellipse. \~
  \details \ru Построить эллипс.\n
    Зафиксирована конечная точка и длина первой полуоси.
    Вводится конечная точка второй полуоси.
    Вычислить длину второй полуоси, центр эллипса и угол наклона первой полуоси.
           \en Construct an ellipse.\n
    The finite point and the length of the first semi-axis are fixed.
    The end point of the second semi-axis is put in.
    Calculate the length of the second semi-axis, ellipse center and inclination angle of the first semi-axis. \~     
  \param[in]  p1 - \ru Конечная точка первой полуоси.
                   \en The end point of the first semi-axis. \~
  \param[in]  l1 - \ru Длина первой полуоси.
                   \en The length of the first semi-axis. \~
  \param[in]  p2 - \ru Конечная точка второй полуоси.
                   \en The end point of the second semi-axis. \~
  \param[out] l2 - \ru Длина второй полуоси.
                   \en The length of the second semi-axis. \~
  \param[out] pc - \ru Центр эллипса.
                   \en The ellipse center. \~
  \param[out] angle - \ru Угол наклона первой полуоси.
                      \en The inclination angle of the first semi-axis. \~
  \ingroup Curve_Modeling
*/  
// ---
MATH_FUNC (bool) EllipsePntPntDist( const MbCartPoint & p1, const double & l1,
                                    const MbCartPoint & p2, double & l2,
                                    MbCartPoint & pc, double & angle );


//------------------------------------------------------------------------------
/** \brief \ru Построить прямую через точку.
           \en Construct a line passing through a given point. \~
  \details \ru Построить прямую через точку, перпендикулярную данной кривой.\n
    Базовая точка прямой сопадает с точкой пересечения.
           \en Construct a line passing through a point and perpendicular to a given curve.\n
    A line origin is coincident with intersection point. \~
  \param[in]  pnt    - \ru Точка на прямой.
                       \en The point on the line. \~
  \param[in]  pCurve - \ru Перпендикулярная прямая.
                       \en The perpendicular line. \~
  \param[out] pLine  - \ru Результат - массив прямых.
                       \en The result - the array of lines. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) LinePointPerpCurve( const MbCartPoint & pnt, const MbCurve & pCurve,
                                     PArray<MbLine> & pLine );


//------------------------------------------------------------------------------
/** \brief \ru Построить прямую через точку.
           \en Construct a line passing through a given point. \~
  \details \ru Построить прямую, проходящую через точку и касательную заданной окружности,
    заданной центром и радиусом.\n
           \en Construct a line passing through a point and tangent to a given circle
    with given center and radius.\n \~
  \param[in]  p      - \ru Точка на прямой.
                       \en The point on the line. \~
  \param[in]  centre - \ru Центр окружности.
                       \en The circle center. \~
  \param[in]  radius - \ru Радиус окружности.
                       \en The circle radius. \~
  \param[out] pl     - \ru Результат - массив прямых.
                       \en The result - the array of lines. \~
  \return \ru Количество прямых.
          \en The number of lines. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (int) LinePointTangentCircle( const MbCartPoint & p, const MbCartPoint & centre, double radius, 
                                        MbLine * pl );


//------------------------------------------------------------------------------
/** \brief \ru Построить окружность.
           \en Construct a circle. \~
  \details \ru Построить окружность по радиусу и точке на ней,
    центр окружности лежит на заданной кривой.
           \en Construct a circle by radius and coincident point,
    a circle center lies on a given curve. \~
  \param[in]  pCurve  - \ru Кривая, содержащая центр окружности.
                        \en The curve containing the circle center. \~
  \param[in]  radius  - \ru Радиус окружности.
                        \en The circle radius. \~
  \param[in]  on      - \ru Точка на окружности.
                        \en The point on the circle. \~
  \param[out] pCircle - \ru Результат - набор окружностей.
                        \en The result - the set of circles. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) CircleCentreOnCurveRadPointOn( const MbCurve & pCurve, double radius, const MbCartPoint & on,
                                                PArray<MbTempCircle> & pCircle );


//------------------------------------------------------------------------------
/** \brief \ru Построить окружность.
           \en Construct a circle. \~
  \details \ru Построить окружность по двум точкам,
    центр которой лежит на заданной кривой.
           \en Construct a circle by two points,
    with a center lying on a given curve. \~
  \param[in]  pCurve  - \ru Кривая, содержащая центр окружности.
                        \en The curve containing the circle center. \~
  \param[in]  on1     - \ru Точка на окружности.
                        \en The point on the circle. \~
  \param[in]  on2     - \ru Точка на окружности.
                        \en The point on the circle. \~
  \param[out] pCircle - \ru Результат - набор окружностей.
                        \en The result - the set of circles. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) CircleCentreOnCurveTwoPoints( const MbCurve & pCurve, const MbCartPoint & on1, const MbCartPoint & on2,
                                               PArray<MbTempCircle> & pCircle );


//------------------------------------------------------------------------------
/** \brief \ru Расстояние между кривыми.
           \en Distance between curves. \~
  \details \ru Расстояние между двумя кривыми.\n
           \en The distance between two curves.\n \~
  \ingroup Algorithms_2D
*/
// ---
struct MATH_CLASS MbDistance {
  double u; ///< \ru Параметр на первой кривой. \en Parameter on the first curve. 
  double v; ///< \ru Параметр на второй кривой. \en Parameter on the second curve. 
  double d; ///< \ru Минимальное расстояние. \en Minimal distance. 
}; // MbDistance


//------------------------------------------------------------------------------
/** \brief \ru Вычислить расстояние.
           \en Calculate distance. \~
  \details \ru Вычислить расстояние между двумя кривыми.
           \en Calculate distance between two curves. \~
  \param[in]  curve1 - \ru Первая кривая.
                       \en The first curve. \~
  \param[in]  curve2 - \ru Вторая кривая.
                       \en The second curve. \~
  \param[out] dmin   - \ru Результат - расстояние между кривыми.
                       \en The result - the distance between curves. \~ 
  \return \ru true  - кривые не пересекаются;
          false - кривые пересекаются.
          \en true if curves do not intersect.
          false if curves intersect. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (bool) DistanceCurveCurve( const MbCurve & curve1, const MbCurve & curve2,
                                     MbDistance & dmin );


//------------------------------------------------------------------------------
/** \brief \ru Построить дугу окружности.
           \en Construct a circle arc. \~
  \details \ru Построить дугу окружности по двум точкам, радиусу и направлению.
           \en Construct a circle arc by two points, radius and direction. \~
  \param[in]  p1        - \ru Точка на окружности.
                          \en The point on the circle. \~
  \param[in]  p2        - \ru Точка на окружности.
                          \en The point on the circle. \~
  \param[in]  rad       - \ru Радиус окружности.
                          \en The circle radius. \~
  \param[in]  clockwise - \ru Признак направления против часовой стенки.
                          \en The attribute of counterclockwise direction. \~
  \param[out] arc       - \ru Результат - массив окружностей.
                          \en The result - the array of circles. \~
  \return \ru Количество окружностей в массиве.
          \en The count of circles in array. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (int) Arc2PointsRadius( const MbCartPoint & p1,
                                  const MbCartPoint & p2,
                                  double rad, bool clockwise, 
                                  MbArc * arc );


//------------------------------------------------------------------------------
/** \brief \ru Рассчитать параметры кривых.
           \en Calculate parameters of curves. \~
  \details \ru Рассчитать параметры кривых для минимального расстояния.
           \en Calculate curves parameters for the minimal distance. \~
  \param[in]  curve1 - \ru Первая кривая.
                       \en The first curve. \~
  \param[in]  curve2 - \ru Вторая кривая.
                       \en The second curve. \~
  \param[out] u      - \ru Параметр на первой кривой.
                       \en Parameter on the first curve. \~
  \param[out] v      - \ru Параметр на второй кривой.
                       \en Parameter on the second curve. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (void) CalculateUV( const MbCurve & curve1, const MbCurve & curve2, double & u, double & v );


//------------------------------------------------------------------------------
/** \brief \ru Расставить точки на кривой.
           \en Put points on a curve. \~
  \details \ru Расставить заданное количество точек на кривой.\n
    Точки можно расставить только на ограниченную кривую.
           \en Put a given number of points on curve.\n
    Points may be put only on a bounded curve. \~
  \param[in]  count  - \ru Количество точек.
                       \en The number of points. \~
  \param[in]  on     - \ru Точка, проекция которой будет добавлена в результат
                       в случае замкнутой кривой.
                       \en Projection of this point will be added in the result
                       in a case of closed curve. \~
  \param[in]  curve  - \ru Кривая.
                       \en The curve. \~
  \param[out] points - \ru Точки на кривой.
                       \en Points on the curve. \~
  \param[out] pars   - \ru Параметры на кривой.
                       \en Parameters on the curve. \~
  \return \ru true, если точки были насчитаны.
          \en true if points were calculated. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (bool) PointsOnCurve( ptrdiff_t count, const MbCartPoint & on, const MbCurve & curve,
                                SArray<MbCartPoint> & points,
                                SArray<double>      & pars );

                       
//------------------------------------------------------------------------------
/** \brief \ru Построить кривые.
           \en Construct curves. \~
  \details \ru Построить кривые по коэффициентам конического сечения.
           \en Construct curves by conic section coefficients. \~      
  \param[in] A, B, C, D, E, F - \ru Коэффициенты уравнения конического сечения
                                A*x^2 + B*x*y + C*y^2 + D*x + E*y + F = 0.
                                \en The coefficients of the conic section equation
                                A*x^2 + B*x*y + C*y^2 + D*x + E*y + F = 0. \~
  \param[in] X1, Y1 - \ru Координаты первой граничной точки.
                      \en The first boundary point coordinates. \~     
  \param[in] X2, Y2 - \ru Координаты второй граничной точки.
                      \en The second boundary point coordinates. \~
  \return \ru Результаты построения:
  - дуга окружности -> дуга окружности;
  - дуга эллипса    -> дуга эллипса;
  - дуга параболы   -> NURBS-кривая;
  - дуга гиперболы  -> NURBS-кривая.
          \en The construction results:
  - circle arc -> circle arc;
  - ellipse arc -> ellipse arc;
  - parabola arc -> NURBS-curve;
  - hyperbola arc -> NURBS-curve. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (MbCurve *) CanonicToParametricConic( double A, double B, double C, double D, double E, double F,
                                                double X1, double Y1, double X2, double Y2 );


//------------------------------------------------------------------------------
/** \brief \ru Аппроксимация кривой дугами и отрезками.
           \en Approximation of a curve by arcs and segments. \~
  \details \ru Аппроксимация кривой дугами и отрезками.
           \en Approximation of a curve by arcs and segments. \~
  \param[in] curve     - \ru Кривая для аппроксимации.
                         \en The curve for approximation. \~
  \param[in] eps       - \ru Метрическая погрешность.
                         \en The metric tolerance. \~
  \param[in] maxRadius - \ru Максимальный радиус аппроксимации.
                         \en The maximal approximation radius. \~
  \param[in] mate      - \ru Флаг аппроксимации с учетом сопряжений.
                         \en The approximation flag with consideration of conjugations. \~
  \param[in] version   - \ru Версия построения.
                         \en The version of construction. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MbCurve *) FatArcContour( const MbCurve & curve, double eps, double maxRadius, bool mate,
                                     VERSION version = Math::DefaultMathVersion() );


#endif // __ALG_CURVE_DISTANCE_H
