////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Методы построения двумерных кривых.
         \en Functions for two-dimensional curves construction. \~
  \details \ru Двумерные кривые могут быть построены с помощью аналитических функций,
    по набору точек, на базе других двумерных кривых.
           \en Two-dimensional curves can be constructed using analytical functions,
    for a point set or on the basis of other two-dimensional curves. \~ 

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ACTION_CURVE_H
#define __ACTION_CURVE_H


#include <templ_s_array.h>
#include <mb_operation_result.h>
#include <op_curve_parameter.h>

class    MbPlacement3D;
class    MbCurve;
class    MbContour;
class    MbCurve3D;
class    MbSurface;
class    MbSNameMaker;
class    MbFace;
class    MbCurveFitParameters;
class    MbCurveFitResults;

//------------------------------------------------------------------------------
/** \brief \ru Перечисление способов создания эллипса (окружности) или их дуг в двумерном пространстве.
           \en Enumeration of ways to create an ellipse (circle) or their arcs in two-dimensional space. \~
\ingroup Curve_Modeling
*/
// ---
enum MbeArcCreateWay
{
  /**
  \ru Окружность по центру и радиусу, задается 'center' и радиус в 'c'.
  \en Circle by center and radius, set the 'center' and radius in 'c'.
  */
  acw_CircleByCenterAndRadius,

  /**
  \ru Дуга окружности по центру и двум точкам.
  Задается: 'center', две точки в 'points', направление в 'option' (true - по часовой стрелке).
  Возвращается: начальный угол дуги в 'а', конечный угол в 'b', радиус в 'c'. 
  \en Circular arc by center and two points.
  Set: 'center', two points in 'points', direction in 'option' (true - clockwise direction).
  Return: start angle in 'a', end in 'b', radius in 'c'.
  */
  acw_ArcByCenterAnd2Points,

  /**
  \ru Дуга окружности по центру и двум углам.
  Задается: 'center', начальный угол в 'a', конечный в 'b', радиус в 'c', 
  направление в 'option' (true - по часовой стрелке). Углы задаются в радианах.
  \en Circular arc by center and two angles, 
  Set: 'center', start angle in 'a', end in 'b', radius in 'c', 
  direction in 'option' (true - clockwise direction).   The angles are given in radians.
  */
  acw_ArcByCenterAnd2Angles,
  /**
  \ru Дуга окружности по трем точкам, заданным в 'points', точки points[0] и points[2] конечные.
   Возвращается: начальный угол дуги в 'а', конечный угол в 'b', радиус в 'c'. 
  \en Circular arc by three points specified in 'points', points[0] and points[2] are the end point.
   Return: start angle in 'a', end in 'b', radius in 'c'.
  */
  acw_ArcBy3Points,

  /**
  \ru Эллипс с заданными полуосями и углом наклона.
  Задается 'center', X полуось в 'a', Y полуось в 'b', угол наклона в 'с'. Угол задается в радианах.
  \en Ellipse by semiaxes and angle.
  Set : 'center', X semiaxis in 'a', Y semiaxis in 'b', angle in 'c'. The angle are given in radians.
  */
  acw_EllipseByCenterAndSemiaxis,

  /**
  \ru Эллипс по центру и трем точкам на нем. 
  Задается 'center' и 3 точки в 'points'. 
  Возвращаются: X полуось в 'a', Y полуось в 'b', угол наклона в 'с'. Угол задается в радианах.
  \en Ellipse by centre and three points on ellipse.
  Set: 'center', 3 points in 'points'. 
  Return:  X semiaxis in 'a', Y semiaxis in 'b', angle in 'c'. The angle are given in radians.
  */
  acw_EllipseByCenterAnd3Points,

  /**
  \ru Дуга эллипса, обрезанная двумя лучами из центра к заданным точкам.
  Задается 'center' и 2 точки в 'points', X полуось в 'a', Y полуось в 'b', угол наклона в 'с', 
  направление в 'option' (true - по часовой стрелке). Угол задается в радианах.
  \en Elliptical arc is trimmed by two rays, starting from the center and passing through points.
  Set: 'center', 2 points in 'points', X semiaxis in 'a', Y semiaxis in 'b', angle in 'c', 
  direction in 'option' (true - clockwise direction). The angle are given in radians.
  */
  acw_EArcByCenterAnd2Points
};

//------------------------------------------------------------------------------
/** \brief \ru Создать прямую.
           \en Create a line. \~
  \details \ru Создать прямую по двум точкам. \n
           \en Create a line given two points. \n \~
  \param[in] point1 - \ru Первая точка.
                      \en The first point. \~
  \param[in] point2 - \ru Вторая точка.
                      \en The second point. \~
  \param[out] result - \ru Прямая.
                       \en The line. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (MbResultType) Line( const MbCartPoint & point1, 
                               const MbCartPoint & point2,
                                     MbCurve *&    result ); 


//------------------------------------------------------------------------------
/** \brief \ru Создать отрезок прямой.
           \en Create a line segment. \~
  \details \ru Создать отрезок прямой по двум точкам. \n
           \en Create a line segment given two points. \n \~
  \param[in] point1 - \ru Первая точка.
                      \en The first point. \~
  \param[in] point2 - \ru Вторая точка.
                      \en The second point. \~
  \param[out] result - \ru Отрезок.
                       \en The segment. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (MbResultType) Segment( const MbCartPoint & point1, 
                                  const MbCartPoint & point2,
                                        MbCurve *&    result ); 


//------------------------------------------------------------------------------
/** \brief  \ru Создать эллипс (окружность) или его дугу указанным способом. 
            \en Create an ellipse (circle) or an elliptical (circular) arc in the specified way. \~
\details  \ru Создать эллипс (окружность) или его дугу указанным способом.\n Входные параметры интерпретируются в соответствии с выбранным путем создания.
          \en Create an ellipse (circle) or an elliptical (circular) arc in the specified way. \n The input parameters are interpreted according to the selected create way.
\param[in] createWay - \ru Способ создания. Определяет как интерпретировать входные параметры.
                       \en Create way. Defines how to interpret the input parameters.\~
\param[in] center - \ru Центр
                    \en Center. \~
\param[in] points - \ru Конечные точки или точки через которые проходит кривая.
                    \en Endpoints or points through which the curve passes. \~
\param[in,out] a - \ru Интерпретация параметра зависит от способа создания дуги, см. enum #ArcCreateWay
                    \en Interpretation of parameter depends on a way of creation of an arc, see enum #ArcCreateWay. \~
\param[in,out] b - \ru Интерпретация параметра зависит от способа создания дуги, см. enum #ArcCreateWay
                    \en Interpretation of parameter depends on a way of creation of an arc, see enum #ArcCreateWay. \~
\param[in,out] с - \ru Интерпретация параметра зависит от способа создания дуги, см. enum #ArcCreateWay
                   \en Interpretation of parameter depends on a way of creation of an arc, see enum #ArcCreateWay. \~
\param[in] option - \ru Интерпретация параметра зависит от способа создания дуги, см. enum #ArcCreateWay
                    \en Interpretation of parameter depends on a way of creation of an arc, see enum #ArcCreateWay. \~
\param[out] result - \ru Эллипс (окружность) или его дуга.
                     \en The ellipse (circle) or the elliptical (circular) arc. \~
\return \ru Возвращает код результата операции.
        \en Returns operation result code. \~
\ingroup Curve_Modeling
*/
//---
MATH_FUNC( MbResultType ) Arc(       MbeArcCreateWay          createWay,
                               const MbCartPoint &            center,
                               const c3d::ParamPointsVector & points,
                                     double &                 a, 
                                     double &                 b, 
                                     double &                 c,
                                     bool                     option,
                                     MbCurve *&               result );

//------------------------------------------------------------------------------
/** \brief \ru Создать кривую, проходящую по набору точек.
           \en Create a curve passing through a set of points. \~
  \details \ru Создать кривую, проходящую по набору точек, следующего типа: \n
    - curveType == pt_LineSegment - отрезок, \n
    - curveType == pt_Arc         - окружность или дуга, \n
    - curveType == pt_Polyline    - ломаная, \n
    - curveType == pt_Bezier      - кривая Безье, \n
    - curveType == pt_CubicSpline - кубический сплайн, \n
    - curveType == pt_Hermit      - составной кубический сплайн Эрмита, \n
    - curveType == pt_Nurbs       - неоднородный рациональный B-сплайн четвертого порядка (кубический). \n
           \en Create a curve passing through a set of points that has the following type: \n
    - curveType == pt_LineSegment - a line segment, \n
    - curveType == pt_Arc         - a circle or an arc, \n
    - curveType == pt_Polyline    - a polyline, \n
    - curveType == pt_Bezier      - a Bezier curve, \n
    - curveType == pt_CubicSpline - a cubic spline, \n
    - curveType == pt_Hermit      - a cubic Hermite spline, \n
    - curveType == pt_Nurbs       - a nonuniform rational B-spline of fourth order (cubic). \n \~
  \param[in]  pointList -   \ru Набор точек.
                            \en A point set. \~
  \param[in]  curveClosed - \ru Замкнутость кривой.
                            \en A curve closedness. \~
  \param[in]  curveType -   \ru Тип кривой.
                            \en A curve type. \~
  \param[out] result -      \ru Кривая.
                            \en The curve. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (MbResultType) SplineCurve( const SArray<MbCartPoint> & pointList,
                                            bool                  curveClosed, 
                                            MbePlaneType          curveType,
                                            MbCurve *&            result ); 


//------------------------------------------------------------------------------
/** \brief \ru Создать NURBS-кривую.
           \en Create a NURBS-curve. \~
  \details \ru Создать NURBS-кривую, построенную по набору контрольных точек. \n
    Контейнер weightList может быть пустым. \n
    Контейнер knotList может быть пустым. \n
           \en Create a NURBS-curve given a sequence of control points. \n
    Container 'weightList' can be empty. \n
    Container 'knotList' can be empty. \n \~
  \param[in]  pointList - \ru Множество точек.
                          \en An array of points. \~
  \param[in]  weightList - \ru Множество весов.
                           \en An array of weights. \~
  \param[in]  degree - \ru Порядок сплайна.
                       \en A spline degree. \~
  \param[in]  knotList - \ru Множество параметрических узлов (Узловой вектор).
                         \en An array of parametric knots (A knot vector). \~
  \param[in]  curveClosed - \ru Замкнутость кривой.
                            \en A curve closedness. \~
  \param[out] result - \ru Сплайновая кривая.
                       \en The spline curve. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve_Modeling
*/
//---
MATH_FUNC (MbResultType) NurbsCurve( const SArray<MbCartPoint> & pointList, 
                                     const SArray<double> &      weightList, 
                                           size_t                degree, 
                                     const SArray<double> &      knotList, 
                                           bool                  curveClosed,
                                           MbCurve *&            result ); 


//------------------------------------------------------------------------------
/** \brief \ru Создать Nurbs кривую с сортировкой точек по расстоянию, проходящую по набору точек.
           \en Create a Nurbs curve with sorting points by distance passing through a set of points. \~
  \details \ru Создать Nurbs кривую с сортировкой точек по расстоянию, проходящую по набору точек, следующего типа: \n
           \en Create a Nurbs curve with sorting points by distance passing through a set of points that has the following type: \n
  \param[in]  pointList   - \ru Исходные точки.
                            \en The initial curve. \~
  \param[in]  curveClosed - \ru Замкнутость кривой.
                            \en Closure of the curve. \~
  \param[in]  degree      - \ru Степень кривой (для Nurbs).
                            \en A curve degree (for Nurbs). \~
  \param[in]  mode        - \ru Вариант построения кривой.
                            \en A Curve construction option. \~
  \param[in/out] accuracy - \ru Требуемая точность / максимальное отклонение построенной кривой.
                            \en Thye desired accuracy / max deviation of constructed curve. \~
  \param[out] result -      \ru Построенная кривая.
                            \en The constructed curve. \~
  \return \ru Возвращает среднее отклонение построенной кривой.
          \en Returns average deviation of constructed curve. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (double) FittingCurve( const std::vector<MbCartPoint> & pointList, 
                                       bool                    curveClosed, 
                                       size_t                  degree,
                                       size_t                  mode,
                                       double &                accuracy,
                                       MbCurve *&              result );


//------------------------------------------------------------------------------
/** \brief \ru Создать копию кривой в виде NURBS.
           \en Create a copy of a curve as a NURBS-curve. \~
  \details \ru Создать копию кривой в виде NURBS. \n
           \en Create a copy of a curve as a NURBS-curve. \n \~
  \param[in] curve - \ru Исходная кривая.
                     \en The initial curve. \~
  \param[out] result - \ru Сплайновая копия кривой.
                       \en The spline copy of the curve. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve_Modeling
*/
//---
MATH_FUNC (MbResultType) NurbsCopy( const MbCurve &  curve, 
                                          MbCurve *& result ); 


//------------------------------------------------------------------------------
/** \brief \ru Создать правильный многоугольник, вписанный в окружность или описанный вокруг окружности.
           \en Create a regular polygon inscribed in a circle or circumscribed around a circle. \~
 \details \ru Создать правильный многоугольник, вписанный в окружность (describe == false) или
    описанный вокруг окружности (describe == true) с центром centre, проходящей через point: \n
    - при vertexCount == 0 строится окружность с центром centre, проходящая через point, \n
    - при vertexCount == 1 строится отрезок c крайними точками centre и point, \n
    - при vertexCount == 2 строится прямоугольник со сторонами, параллельными глобальным осям и противоположными вершинами в centre и point, \n
    - при vertexCount >= 3 строится правильный многоугольник с заданным числом сторон, 
    вписанный в окружность (describe == false) или описанный вокруг окружности (describe == true) с центром centre, проходящей через point: \n
           \en Create a regular polygon inscribed in a circle (describe == false) or
    circumscribed around a circle (describe == true) with the specified centre and passing through the given point: \n
    - if vertexCount == 0, a circle with center 'center' passing through 'point' is created, \n
    - if vertexCount == 1, a line segment with end points at 'centre' and 'point' is created, \n
    - if vertexCount, == 2 a rectangle aligned with the global axes with the opposite vertices at points 'centre' and 'point' is created, \n
    - if vertexCount >= 3, a regular polygon is created with a given number of sides, 
    inscribed in a circle (describe == false) or circumscribed around a circle (describe == true) with the specified centre and passing through the given point: \n \~
  \param[in]  centre - \ru Центр фигуры.
                       \en A figure centre. \~
  \param[in]  point - \ru Точка для построения.
                      \en A point for the curve construction. \~
  \param[in]  vertexCount - \ru Количество вершин правильного многоугольника.
                            \en The number of vertices of a regular polygon. \~
  \param[in]  describe - \ru Флаг построения многоугольника: описать вокруг окружности (true), вписать в окружность (false).
                         \en A polygon construction flag: circumscribe the polygon around the circle, inscribe the polygon in the circle (false). \~
  \param[out] result - \ru Результат построения.
                       \en The curve creation result. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve_Modeling
*/
//---
MATH_FUNC (MbResultType) RegularPolygon( const MbCartPoint & centre, 
                                         const MbCartPoint & point,
                                               size_t        vertexCount, 
                                               bool          describe,
                                               MbCurve *&    result ); 


//------------------------------------------------------------------------------
/** \brief \ru Создать косинусоиду.
           \en Create a cosine curve. \~
  \details \ru Создать косинусоиду по точкам, фазе и длине волны. \n
           \en Create a cosine curve given the points, phase and wave length. \n \~
  \param[in]  point0 - \ru Начало локальной системы координат (ЛСК).
                       \en The origin of local coordinate system (LCS). \~
  \param[in]  point1 - \ru Точка на оси X ЛСК.
                       \en A point on the X-axis of LCS. \~
  \param[in]  point2 - \ru Точка на оси Y ЛСК.
                       \en A point on the Y-axis of LCS. \~
  \param[in]  phase - \ru Фаза.
                      \en The phase. \~
  \param[in]  waveLength - \ru Длина Волны.
                           \en The wave length. \~
  \param[out] result - \ru Косинусоида.
                       \en The cosine curve. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve_Modeling
*/
//---
MATH_FUNC (MbResultType) Cosinusoid( const MbCartPoint & point0, 
                                     const MbCartPoint & point1, 
                                     const MbCartPoint & point2,
                                           double        phase, 
                                           double        waveLength,
                                           MbCurve *&    result );

//------------------------------------------------------------------------------
/** \brief \ru Создать косинусоиду.
           \en Create a cosine curve. \~
  \details \ru Создать косинусоиду по точкам, фазе и длине волны. \n
           \en Create a cosine curve given the points, phase and wave length. \n \~
  \param[in]  origin - \ru Начало локальной системы координат (ЛСК).
                       \en The origin of local coordinate system (LCS). \~
  \param[in]  amplitude - \ru Амплитуда волны.
                          \en The amplitude of the wave. \~
  \param[in]  waveLength - \ru Длина Волны.
                           \en The wave length. \~
  \param[in]  wavesCount - \ru Количество волн.
                           \en The number of waves. \~
  \param[in]  phase - \ru Фаза.
                      \en The phase. \~
  \param[out] result - \ru Косинусоида.
                       \en The cosine curve. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve_Modeling
*/
//---
MATH_FUNC (MbResultType) Cosinusoid( const MbCartPoint & origin, 
                                           double        amplitude, 
                                           double        waveLength, 
                                           double        wavesCount,
                                           double        phase, 
                                           MbCurve *&    result );


//------------------------------------------------------------------------------
/** \brief \ru Создать составную кривую (контур).
           \en Create a composite curve (contour). \~
  \details \ru Создать составную кривую (контур) на базе исходной кривой. \n
           \en Create a composite curve (contour) on the basis of the given curve. \n \~
  \param[in]  curve - \ru Исходная кривая.
                      \en The initial curve. \~
  \param[out] result - \ru Контур на основе кривой.
                       \en The contour created on the basis of the curve. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve_Modeling
*/
//---
MATH_FUNC (MbResultType) CreateContour( MbCurve &    curve, 
                                        MbContour *& result ); 


//------------------------------------------------------------------------------
/** \brief \ru Создать копию кривой.
           \en Create a copy of a curve. \~ 
  \details \ru Создать копию кривой с заменой некоторых кривых. \n
           \en Create a copy of a curve with substitution of some curves. \n \~
  \param[in] curve   - \ru Исходная кривая.
                       \en The initial curve. \~
  \param[in] version - \ru Версия исполнения.
                       \en The version. \~
  \return \ru Возвращает модифицированную копию кривой, если получилось ее создать.
          \en Returns a modified copy of the curve if it has been successfully created. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (MbCurve *) DuplicateCurve( const MbCurve & curve, VERSION version = Math::DefaultMathVersion() ); 


//------------------------------------------------------------------------------
/** \brief \ru Создать копию контура.
           \en Create a copy of a contour. \~
  \details \ru Создать копию контура с заменой некоторых кривых и его модификацией по флагу. 
           Модификация - слияние подобных кривых и удаление вырожденных. \n
           \en Create a copy of a contour with substitution of some curves and its modification according to the flag. 
           Modification is a merging of similar curves and deleting of degenerate ones. \n \~
  \param[in] cntr - \ru Исходный контур.
                    \en The initial contour. \~
  \param[in] modifySegments - \ru Флаг разрешения замены и слияния сегментов.
                              \en The flag determines whether segments can be replaced or merged. \~
  \param[in] version - \ru Версия исполнения.
                       \en The version. \~
  \param[in] names - \ru Именователь, синхронизированный с контуром.
                     \en An object defining the names synchronized with contour. \~
  \return \ru Возвращает модифицированнную копию контура, если получилось его создать.
          \en Returns a modified copy of the contour if it has been successfully created. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (MbContour *) DuplicateContour( const MbContour &    cntr, 
                                                bool           modifySegments, 
                                                VERSION        version,
                                                MbSNameMaker * names = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Создать эквидистантную кривую.
           \en Create an offset curve. \~
  \details \ru Создать эквидистантную кривую по базовой кривой и смещению в крайних точках. \n
           \en Create the offset curve for a given curve with offset in the begin and the end points. \n \~
  \param[in] curve -   \ru Базовая кривая. Не может быть контуром или ломаной.
                       \en Base curve. Cannot be a contour or polyline. \~
  \param[in] offset1 - \ru Смещение в точке Tmin базовой кривой.
                       \en Offset distance on point Tmin of base curve. \~
  \param[in] offset2 - \ru Смещение в точке Tmax базовой кривой.
                       \en Offset distance on point Tmax of base curve. \~
  \param[in] type -    \ru Тип смещения точек: константный, линейный или кубический.
                       \en The offset type: constant, or linear, or cubic. \~
  \return \ru Возвращает эквидистантную кривую.
          \en Returns the offset curve. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (MbCurve *) OffsetCurve( const MbCurve &     curve, 
                                         double        offset1, 
                                         double        offset2, 
                                         MbeOffsetType type );


//------------------------------------------------------------------------------
/** \brief \ru Создать эквидистантный контур.
           \en Create an offset contour. \~
  \details \ru Создать эквидистантный контур к исходному контуру. \n
           \en Create the offset contour for a given contour. \n \~
  \param[in] cntr - \ru Исходный контур.
                    \en The initial contour. \~
  \param[in] rad - \ru Величина эквидистантного смещения.
                   \en The offset value. \~
  \param[in] xEpsilon - \ru Точность по x.
                        \en Tolerance in x direction. \~
  \param[in] yEpsilon - \ru Точность по y.
                        \en Tolerance in y direction. \~
  \param[in] modifySegments - \ru Флаг разрешения замены и слияния сегментов.
                              \en The flag determines whether segments can be replaced or merged. \~
  \param[in] version - \ru Версия исполнения.
                       \en The version. \~
  \return \ru Возвращает эквидистантный контур, если получилось его создать.
          \en Returns the offset contour if it has been successfully created. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (MbContour *) OffsetContour( const MbContour & cntr, 
                                             double      rad, 
                                             double      xEpsilon, 
                                             double      yEpsilon, 
                                             bool        modifySegments, 
                                             VERSION     version = Math::DefaultMathVersion() ); // BUG_61694


//------------------------------------------------------------------------------
/** \brief \ru Создать эквидистантный контур, начинающийся и оканчивающийся на оси вращения.
           \en Create an offset contour with start and end points on the rotation axis. \~
  \details \ru Создать незамкнутый эквидистантный контур, начинающийся и оканчивающийся на оси вращения. \n
    Считается, что, если контур замкнуть, то он будет ориентирован против движения часовой стрелки. \n
           \en Create an open offset contour with start and end points on the rotation axis. \n
    It is considered that if one closes the contour, it will be oriented counterclockwise. \n \~
  \param[in] cntr - \ru Исходный контур.
                    \en The initial contour. \~
  \param[in] q1 - \ru Начальная точка оси вращения.
                  \en The start point of the rotation axis. \~
  \param[in] q2 - \ru Конечная точка оси вращения.
                  \en The end point of the rotation axis. \~
  \param[in] rad - \ru Величина эквидистантного смещения.
                   \en The offset value. \~
  \param[in] xEpsilon - \ru Точность по x.
                        \en Tolerance in x direction. \~
  \param[in] yEpsilon - \ru Точность по y.
                        \en Tolerance in y direction. \~
  \param[in] version - \ru Версия исполнения.
                       \en The version. \~
  \return \ru Возвращает эквидистантный контур, если получилось его создать.
          \en Returns the offset contour if it has been successfully created. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (MbContour *) AxisOffsetOpenContour( const MbContour &   cntr, 
                                               const MbCartPoint & q1, 
                                               const MbCartPoint & q2,
                                                     double        rad, 
                                                     double        xEpsilon, 
                                                     double        yEpsilon,
                                                     VERSION       version );// = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Инициализировать кривую по новым параметрам.
           \en Initialize a curve with new parameters. \~
  \details \ru Инициализировать кривую по новым параметрам. \n
           \en Initialize a curve with new parameters. \n \~
  \param[in,out] curve - \ru Изменяемая кривая.
                         \en The curve to be modified. \~
  \param[in] t1 - \ru Новый начальный параметр.
                  \en A new start parameter. \~
  \param[in] t2 - \ru Новый конечный параметр.
                  \en A new end parameter. \~
  \param[in] eps - \ru Точность.
                   \en Tolerance. \~
  \return \ru Возвращает true, если получилось модифицировать кривую.
          \en Returns 'true' if the curve has been successfully modified. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (bool) CurveTrim( MbCurve & curve, 
                            double    t1, 
                            double    t2, 
                            double    eps = METRIC_PRECISION );


//------------------------------------------------------------------------------
/** \brief \ru Добавить кривую в составную кривую (контур).
           \en Add a curve to a composite curve (a contour). \~ 
  \details \ru Добавить кривую curve в составную кривую (контур) contour. \n
    Если toEnd == true, то добавить в конец. \n
    Если toEnd == false, то добавить в начало. \n
           \en Add a curve to a composite curve (a contour) 'contour'. \n
    If toEnd == true, the curve is to be added to the end. \n
    If toEnd == false, the curve is to be added to the beginning. \n \~
  \param[in]     curve - \ru Добавляемая кривая
                         \en A curve to be added. \~
  \param[in,out] contour - \ru Модифицируемый контур.
                           \en A contour to be modified. \~
  \param[in]     toEnd - \ru Флаг места добавления кривой.
                         \en The flag determines the place of the curve in the contour. \~
  \param[in]     epsilon - \ru Близость точек стыка при добавлении кривой.
                           \en The Proximity of the junction points. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve_Modeling
*/
//---
MATH_FUNC (MbResultType) AddCurveToContour( MbCurve &   curve, 
                                            MbContour & contour, 
                                            bool        toEnd,
                                            double      epsilon = METRIC_REGION );


//------------------------------------------------------------------------------
/** \brief \ru Создать контур из кривых.
           \en Create a contour by curves. \~ 
  \details \ru В контур будут добавлены кривые curves, возможно инвертирование направления некоторых кривых. Неиспользованные кривые будут удалены. \n
           \en Curves will be added to the contour, it is possible to invert the direction of some curves. Unused curves will be deleted. \n
  \param[in] curve - \ru Кривые для построения контура.
                     \en Curves of the contour. \~
  \param[in] accuracy - \ru Максимальное расстояние точек сегментов в стыке.
                        \en The maximum distance of segment points at the junction. \~
  \return \ru Возвращает контур или nullptr.
          \en Returns contour or nullptr. \~
  \ingroup Curve_Modeling
*/
//---
MATH_FUNC (MbContour *) MakeContour( std::vector<MbCurve *> & curves, double accuracy, VERSION version = Math::DefaultMathVersion() ); 


//------------------------------------------------------------------------------
/** \brief \ru Объединить две подобные кривые в одну.
           \en Combine two similar curves into one. \~ 
  \param[in] curveOne - \ru Первая кривая.
                        \en First curve. \~
  \param[in] curveTwo - \ru Вторая кривая.
                        \en Second curve. \~
  \param[in] precision - \ru Точность.
                         \en Tolerance. \~
  \param[in] version - \ru Версия исполнения.
                       \en The version. \~
  \param[out] result - \ru Кривая - результат объединения.
                       \en Curve - result of merge. \~
  \return \ru Возвращает true, если получилось слить кривые.
          \en Returns 'true' if the curve has been successfully merged. \~
  \ingroup Curve_Modeling
*/
//---
MATH_FUNC (bool) MergeCurves( const MbCurve & curveOne, const MbCurve & curveTwo, double precision, VERSION version, c3d::PlaneCurveSPtr & result );


//------------------------------------------------------------------------------
/** \brief \ru Найти пересечения кривой с плоскостью.
           \en Calculate the intersections of a curve and a surface. \~
  \details \ru Найти пересечения кривой с плоскостью. \n
    Результат - массив точек или массив двумерных кривых на плоскости.
           \en Calculate the intersections of a curve and a surface. \n
    The result is an array of points or an array of two-dimensional curves on the plane. \~
  \param[in] curve - \ru Кривая.
                     \en The curve. \~
  \param[in] place - \ru Система координат плоскости.
                     \en The plane coordinate system. \~
  \param[out] result - \ru Множество точек на плоскости.
                       \en The array of points on the plane. \~
  \param[out] resultCurve - \ru Множество кривых на плоскости.
                            \en The array of curves on the plane. \~
  \param[in] version - \ru Версия исполнения.
                       \en The version. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) CurveSection( const MbCurve3D &           curve, 
                               const MbPlacement3D &       place,
                                     SArray<MbCartPoint> & result, 
                                     RPArray<MbCurve> &    resultCurve,
                                     VERSION               version = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Найти пересечения поверхности с плоскостью.
           \en Calculate the intersections of a surface and a plane. \~
  \details \ru Найти пересечения поверхности с плоскостью. \n
    Результат - массив кривых на поверхности и двумерных кривых на плоскости.
           \en Calculate the intersections of a surface and a plane. \n
    The result is an array of curves on the surface and two-dimensional curves on the plane. \~
  \param[in] surface - \ru Поверхность.
                       \en A surface. \~
  \param[in] place - \ru Система координат плоскости.
                     \en The plane coordinate system. \~
  \param[out] result - \ru Множество кривых на плоскости.
                       \en The array of curves on the plane. \~
  \param[in] version - \ru Версия исполнения.
                       \en The version. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) SurfaceSection( const MbSurface &     surface, 
                                 const MbPlacement3D & place,
                                 RPArray<MbCurve> &    result,
                                 VERSION               version = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Создать двумерный сегмент поверхности проецированием ориентированного ребра.
           \en Create a two-dimensional segment on a surface by projection of an oriented edge. \~
  \details \ru Создать двумерный сегмент поверхности проецированием ориентированного ребра. \n
    Результатом является двумерная кривая в параметрической области поверхности surface.
           \en Create a two-dimensional segment on a surface by projection of an oriented edge. \n
    The result is a two-dimensional curve in the parametric domain of the given surface. \~
  \param[in] face - \ru Грань поверхности.
                    \en A face defined on the surface. \~
  \param[in] loopInd - \ru Номер цикла в грани.
                       \en The number of a loop in the face. \~
  \param[in] edgeInd - \ru Номер проецируемого ребра в цикле.
                       \en Index of the edge in the loop to be projected. \~
  \param[in] surface - \ru Поверхность проецирования.
                       \en A surface to project on. \~
  \param[in] version - \ru Версия изготовления.
                       \en Version. \~
  \param[out] result - \ru Двумерная кривая.
                       \en A two-dimensional curve. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve_Modeling
*/
//---
MATH_FUNC (MbResultType) FaceBoundSegment( const MbFace &    face, 
                                                 size_t      loopInd, 
                                                 size_t      edgeInd, // \ru Проецируемое ребро грани \en The edge of face to be pojected. 
                                           const MbSurface & surface, // \ru На поверхность \en On the surface 
                                                 VERSION     version,
                                                 MbCurve *&  result );

//------------------------------------------------------------------------------
/** \brief \ru Создать двумерную границу поверхности проецированием пространственной кривой.
           \en Create a two-dimension boundary of a surface by projection of a space curve. \~
  \details \ru Создать двумерную границу поверхности проецированием пространственной кривой \n
           (предполагается, что пространственные граничные кривые лежат на поверхности). \n
           \en Create a two-dimension boundary of a surface by projection of a space curve \n
           (the boundary space curves are considered to belong to the surface) \n \~
  \param[in] surface - \ru Поверхность.
                       \en A surface. \~
  \param[in] spaceCurve - \ru Пространственная кривая.
                          \en A space curve. \~
  \param[in] version - \ru Версия изготовления.
                       \en Version. \~
  \param[out] result - \ru Двумерный контур на поверхности.
                       \en The two-dimensional contour on the surface. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (MbResultType) SurfaceBoundContour( const MbSurface &  surface, 
                                              const MbCurve3D &  spaceCurve,
                                                    VERSION      version,
                                                    MbContour *& result );


//------------------------------------------------------------------------------
/** \brief \ru Скорректировать начальную точку.
           \en Correct the start point. \~
  \details \ru Изменить начальную точку кривой на новую.\n
    Меняет начальную точку у кривых типа:\n 
    pt_Nurbs, pt_Hermit, pt_Polyline, pt_Bezier, 
    pt_CubicSpline, pt_LineSegment, pt_ReparamCurve,\n
    или у контура pt_Contour, если первый его сегмент одного из перечисленных типов.
           \en Change the start point of curve with a new one.\n
    Changes the start point for curves of types:\n 
    pt_Nurbs, pt_Hermit, pt_Polyline, pt_Bezier, 
    pt_CubicSpline, pt_LineSegment, pt_ReparamCurve,\n
    or for contour pt_Contour if its first segment is of one of the listed types. \~
  \param[in] segment - \ru Изменяемая кривая.
                       \en The modified curve. \~
  \param[in] p      - \ru Новая начальная точка.
                      \en A new start point. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (bool) ChangeFirstPoint( MbCurve * segment, const MbCartPoint & p );


//------------------------------------------------------------------------------
/** \brief \ru Скорректировать конечную точку.
           \en Correct the last point. \~
  \details \ru Изменить конечную точку кривой на новую.\n
    Меняет начальную точку у кривых типа:\n 
    pt_Nurbs, pt_Hermit, pt_Polyline, pt_Bezier, 
    pt_CubicSpline, pt_LineSegment, pt_ReparamCurve,\n
    или у контура pt_Contour, если последний его сегмент одного из перечисленных типов.
           \en Change the end point of curve with a new one.\n
    Changes the end point for curves of types:\n 
    pt_Nurbs, pt_Hermit, pt_Polyline, pt_Bezier, 
    pt_CubicSpline, pt_LineSegment, pt_ReparamCurve,\n
    or for contour pt_Contour if its last segment is of one of the listed types. \~
  \param[in] segment - \ru Изменяемая кривая.
                       \en The modified curve. \~
  \param[in] p      - \ru Новая начальная точка.
                      \en A new start point. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (bool) ChangeLastPoint( MbCurve * segment, const MbCartPoint & p ); 


//------------------------------------------------------------------------------
/** \brief \ru Является ли кривая прямолинейной независимо от ее параметризации.
           \en Whether the curve is like straight-line regardless of its parameterization. \~
  \details \ru Является ли кривая прямолинейной независимо от ее параметризации.\n
           \en Whether the curve is like straight-line regardless of its parameterization. \~
  \param[in] curve - \ru Кривая.
                     \en Curve. \~
  \param[in] eps   - \ru Точность.
                     \en Accuracy. \~
  \return \ru Возвращает true, если кривая геометрически прямолинейна.
          \en Returns true, if a curve is geometrically straight. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (bool) IsLikeStraightLine( const MbCurve & curve, double eps );


//------------------------------------------------------------------------------
/** \brief \ru Удалить вырожденные сегменты из контура.
           \en Delete degenerate segments from contour. \~
  \details \ru Удалить вырожденные сегменты из контура с заменой некоторых кривых и модификацией по флагу. \n
           \en Delete degenerate segments from contour with substitution of some curves and its modification according to the flag. \n \~
  \param[in] cntr           - \ru Исходный контур.
                              \en The initial contour. \~
  \param[in] modifySegments - \ru Флаг разрешения замены сегментов.
                              \en The flag determines whether segments can be replaced. \~
  \param[in] version        - \ru Версия исполнения.
                              \en The version. \~
  \param[in] names          - \ru Именователь, синхронизированный с контуром.
                              \en An object defining the names synchronized with contour. \~
  \return \ru Возвращает модифицированнную копию контура, если получилось его создать.
          \en Returns a modified copy of the contour if it has been successfully created. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC( MbContour * ) DeleteDegenerateSegments( const MbContour &    cntr,
                                                         bool           modifySegments,
                                                         VERSION        verison,
                                                         MbSNameMaker * names = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Удалить самопересечения из контура.
           \en Remove self-intersections from contour. \~
  \details \ru Если контур самопересекается, удаляет части контура, чтобы в результате не осталось самопересечений. \n
           \en If the contour self-intersects, removes parts of the contour so that the result is free of self-intersections. \n \~
  \param[in, out] contour - \ru Исходный контур для изменения.
                            \en The initial contour to be modified. \~
  \param[in] params       - \ru Параметры операции.
                            \en Operation parameters. \~
  \param[out] parts       - \ru Набор несамопересекающихся замкнутых контуров, оставшихся после удаления самопересечений.
                            \en The set of non-self-intersecting closed contours remaining after removing self-intersections. \~
  \return \ru Возвращает код результата операции:
                MbResultType::rt_NoIntersect, если самопересечений не найдено;
                MbResultType::rt_Success, если самопересечения успешно удалены;
                MbResultType::rt_Error в случае ошибки выполнения операции.
          \en Returns operation result code:
                MbResultType::rt_NoIntersect if no self-intersections were found;
                MbResultType::rt_Success if self-intersections were successfully removed;
                MbResultType::rt_Error if the operation failed. \~
  \ingroup Curve_Modeling
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
MATH_FUNC( MbResultType ) RemoveSelfIntersections(       MbContour &                    contour,
                                                   const MbRemoveSelfIntParams &        params,
                                                         std::vector<SPtr<MbContour>> * parts = nullptr );


//------------------------------------------------------------------------------
  /** \brief \ru Установить параметризацию кривой пропорционально длине кривой.
             \en Set curve parametrization proportional to the curve length. \~
    \details \ru Если параметризация кривой не пропорциональна длине кривой, то изменить параметризацию кривой, сделав её пропорциональной длдине кривой. \n
              В общем случае кривая будет заменена на репараметризованную кривую MbReparamCurve с пропорциональной длине параметризацией.
              В частных случаях, когда исходная кривая имеет пропорциональную параметризацию с требуемыми границами, будет возвращена исходная кривая. \n
             \en If the curve parameterization is not proportional to the length of the curve, then change the curve parameterization to make it proportional to the length of the curve. \n
             In general, the curve will be replaced by a MbReparamCurve3D curve with length proportional parameterization.
             In special cases, when the curve has a proportional parametrization with the required boundaries, the original curve will be returned. \~ \n
    \param[in]  curve - \ru Исходная кривая, подлежащая репараметризации.
                        \en The original curve to be reparametrized. \~
    \param[in]  tMin  - \ru Новый минимальнай параметр кривой.
                        \en New minimal parameter of the curve. \~
    \param[in]  tMax  - \ru Новый максимальнай параметр кривой.
                        \en New maximal parameter of the curve. \~
    \return \ru Возвращает построенную кривую с пропорциональной длине параметризацией.
            \en Returns created curve with proportional length  parameterization.
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC ( MbCurve * ) SetProportionalParametrization( MbCurve * curve,
                                                        double tMin, double tMax );


//------------------------------------------------------------------------------
/** \brief \ru Вписать кривую в множество точек.
           \en Fit the curve into point set. \~
  \details \ru Вписать кривую заданного типа в множество точек.
           \en Fit a curve of a given type into the point set. \~
  \param[in]  params - \ru Параметры вписывания.
                       \en The fitting parameters. \~
  \param[out] results - \ru Результаты вписывания.
                        \en Results of curve fitting. \~
  \return  \ru Возвращает код результата операции.
           \en Returns operation result code. \~
  \ingroup Polygonal_Objects
*/
// ---
MATH_FUNC( MbResultType ) FitCurve( const MbCurveFitParameters & params, MbCurveFitResults & results );


//------------------------------------------------------------------------------
/** \brief \ru Построить срединную кривую.
           \en Create median curve. \~
  \details \ru Построить срединную кривую.
           \en Create median curve. \~
  \param[in]  baseCurve - \ru Базовая кривая.
                          \en The base curve. \~
  \param[in]  responseCurve - \ru Ответная кривая.
                              \en The response curve. \~
  \param[in]  params - \ru Параметры построения.
                       \en The parameters for creating median curve. \~
  \param[out] results - \ru Результаты построения.
                        \en The results of creating median curve. \~
  \return  \ru Возвращает код результата операции.
           \en Returns operation result code. \~
  \ingroup Curve_Modeling
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
MATH_FUNC( MbResultType ) MedianCurve( const MbCurve &              baseCurve,
                                       const MbCurve &              responseCurve,
                                       const MbMedianCurveParams &  params,
                                             MbMedianCurveResults & results );
#endif // __ACTION_CURVE_H
