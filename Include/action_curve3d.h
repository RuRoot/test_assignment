////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Методы построения трехмерных кривых.
         \en Functions for three-dimensional curves construction. \~
  \details \ru На базе кривых строятся рёбра. Рёбра используются в твёрдотельной и каркасной модели. 
    Кроме того, кривые используются для построения поверхностей, а также могут служить
    вспомогательными элементами модели.
           \en Edges are created on the basis of curves. Edges are used in solid and wireframe model. 
    In addition curves are used for construction of surfaces as well as can be used
    as auxiliary elements of a model. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ACTION_CURVE3D_H
#define __ACTION_CURVE3D_H


#include <hash32.h>
#include <templ_s_array.h>
#include <templ_sptr.h>
#include <mb_cart_point3d.h>
#include <space_item.h>
#include <curve3d.h>
#include <mb_operation_result.h>
#include <vector>


class    MbCurve;
class    MbContour3D;
class    MbWireFrame;
class    MbFace;
class    MbSurface;
class    MbItem;
class    MbName;
class    MbSweptData;
class    MbContourChamferParams;
struct   MbNurbsParameters;
class    MbNurbsCopyCurveResult;
class    MbCurvesWrappingParams;
class    MbCurvesWrappingResults;
class    MbCurveFitParameters3D;
class    MbCurveFitResults3D;
class    MbCurveExtensionParameters3D;

struct   EvolutionValues;

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
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) Line( const MbCartPoint3D & point1, 
                               const MbCartPoint3D & point2,
                                     MbCurve3D *&    result ); 


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
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) Segment( const MbCartPoint3D & point1, 
                                  const MbCartPoint3D & point2,
                                        MbCurve3D *&    result ); 


//------------------------------------------------------------------------------
/** \brief \ru Создать эллипс (окружность) или его дугу.
           \en Create an ellipse (circle) or an elliptical (circular) arc. \~
  \details \ru Создать эллипс (окружность) или его дугу. \n
    Контейнер points может содержать 0, 2 элементов. \n
           \en Create an ellipse (circle) or an elliptical (circular) arc. \n
    The container 'points' should contain 0 or 2 elements. \n \~
  \param[in] centre - \ru Центр эллипса (окружности)
                      \en The center of an ellipse (circle). \~
  \param[in] points - \ru Точки дуги.
                      \en Points on the arc. \~
  \param[in] curveClosed - \ru Замкнутость дуги.
                           \en The closedness of the arc. \~
  \param[in] angle - \ru Угол наклона.
                     \en The inclination angle. \~
  \param[in,out] a - \ru Длина большой полуоси.
                     \en The major axis length. \~
  \param[in,out] b - \ru Длина малой полуоси.
                     \en The minor axis length. \~
  \param[out] result - \ru Эллипс (окружность) или его дуга.
                       \en The ellipse (circle) or the elliptical (circular) arc. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC (MbResultType) Arc( const MbCartPoint3D &         centre, 
                              const SArray<MbCartPoint3D> & points, 
                                    bool                    curveClosed, 
                                    double                  angle, 
                                    double &                a, 
                                    double &                b,
                                    MbCurve3D *&            result );


//------------------------------------------------------------------------------
/** \brief \ru Создать кривую, проходящую по набору точек.
           \en Create a curve passing through a set of points. \~
  \details \ru Создать кривую, проходящую по набору точек, следующего типа: \n
    curveType == st_LineSegment3D - отрезок, \n
    curveType == st_Arc3D         - окружность или дуга, \n
    curveType == st_Polyline3D    - ломаная, \n
    curveType == st_Bezier3D      - кривая Безье, \n
    curveType == st_CubicSpline3D - кубический сплайн, \n
    curveType == st_Hermit3D      - составной кубический сплайн Эрмита, \n
    curveType == st_Nurbs3D       - неоднородный рациональный B-сплайн четвертого порядка (кубический). \n
           \en Create a curve passing through a set of points that has the following type: \n
    curveType == st_LineSegment3D - a line segment, \n
    curveType == st_Arc3D         - a circle or an arc, \n
    curveType == st_Polyline3D    - a polyline, \n
    curveType == st_Bezier3D      - a Bezier curve, \n
    curveType == st_CubicSpline3D - a cubic spline, \n
    curveType == st_Hermit3D      - a cubic Hermite spline, \n
    curveType == st_Nurbs3D       - a nonuniform rational B-spline of fourth order (cubic). \n \~
  \param[in]  pointList -   \ru Набор точек.
                            \en A point set. \~
  \param[in]  curveClosed - \ru Замкнутость кривой.
                            \en A curve closedness. \~
  \param[in]  curveType   - \ru Тип кривой.
                            \en A curve type. \~
  \param[out] result -      \ru Кривая.
                            \en The curve. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) SplineCurve( const SArray<MbCartPoint3D> & pointList, 
                                            bool                    curveClosed, 
                                            MbeSpaceType            curveType,
                                            MbCurve3D *&            result ); 


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
  \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC (MbResultType) NurbsCurve( const SArray<MbCartPoint3D> & pointList, 
                                     const SArray<double> &        weightList, 
                                           size_t                  degree, 
                                     const SArray<double> &        knotList, 
                                           bool                    curveClosed,
                                           MbCurve3D *&            result ); 


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
  \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC (MbResultType) NurbsCopy( const MbCurve3D &  curve, 
                                          MbCurve3D *& result ); 


//------------------------------------------------------------------------------
/** \brief \ru Создать NURBS копию кривой, по заданным параметрам.
           \en Create a NURBS curve copy by given parameters. \~
  \details \ru Создается NURBS копия с заданной точностью, присланной из параметров. Если точность <= LENGTH_EPSILON создается NURBS копия без учета точности. \n
           \en A copy is created with the specified accuracy sent from parameters. If accuracy <= LENGTH_EPSILON, a NURBS copy is created without regard to accuracy. \n \~
  \param[in] curve - \ru Исходная кривая.
                     \en The initial curve. \~
  \param[in] params - \ru Параметры построения NURBS копии объекта.
                      \en Parameters for the construction of a NURBS copy of the object. \~
  \param[out] result - \ru Выходные данные для операции создания NURBS копии кривой.
                       \en Output data for create NURBS copy of curve. \~
\result \ru Возвращает код результата операции.
        \en Returns operation result code. \~
\ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) NurbsCopy( const MbCurve3D &              curve, 
                                    const MbNurbsParameters &      params,
                                          MbNurbsCopyCurveResult & result ); 


//------------------------------------------------------------------------------
/** \brief \ru Создать правильный многоугольник, вписанный в окружность.
           \en Create a regular polygon inscribed in a circle. \~
  \details \ru Создать правильный многоугольник, вписанный в окружность \n
    (при describe == true описанного вокруг окружности) с центром centre и проходящей через point: \n
    - при vertexCount <= 1 строится окружность с центром centre и проходящая через point, \n
    - при vertexCount == 2 строится прямоугольник со сторонами, параллельными глобальным осям
    и противоположными вершинами в centre и point. \n
           \en Create a regular polygon inscribed in a circle \n
    (if 'describe' == true circumscribed around a circle) with the specified centre and passing through the given point: \n
    - if vertexCount <= 1, a circle with center 'center' passing through 'point' is created, \n
    - if vertexCount == 2, a rectangle aligned with the global axes
    with the opposite vertices at points 'centre' and 'point' is created. \n \~
  \param[in]  centre - \ru Центр.
                       \en The center. \~
  \param[in]  point - \ru Точка.
                      \en A point. \~
  \param[in]  axisZ - \ru Ось Z для создания ЛСК кривой.
                      \en Z-axis for curve LCS creation. \~
  \param[in]  vertexCount - \ru Количество вершин.
                            \en Number of vertices. \~
  \param[in]  describe - \ru Описанный вокруг окружности.
                         \en Circumscribing around the circle. \~
  \param[out] result - \ru Правильный многоугольник.
                       \en The regular polygon. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC (MbResultType) RegularPolygon( const MbCartPoint3D & centre, 
                                         const MbCartPoint3D & point,
                                         const MbVector3D &    axisZ,
                                               size_t          vertexCount, 
                                               bool            describe,
                                               MbCurve3D *&    result ); 


//------------------------------------------------------------------------------
/** \brief \ru Создать спираль.
           \en Create a spiral. \~
  \details \ru Создать спираль. \n
    Если spiralAxis == true,  то lawCurve - определяет плоскую ось спирали. \n
    Если spiralAxis == false, то lawCurve - определяет закон изменения радиуса спирали. \n
           \en Create a spiral. \n
    If 'spiralAxis' == true, 'lawCurve' determines the axis of a spiral. \n
    If spiralAxis == false, then 'lawCurve' - determines a radius law. \n \~
  \param[in] place - \ru Локальная система координат.
                     \en A local coordinate system. \~
  \param[in] radius - \ru Радиус спирали.
                      \en A spiral radius. \~
  \param[in] step - \ru Шаг спирали.
                    \en A pitch. \~
  \param[in] lawCurve - \ru Формообразующая кривая.
                        \en A guide curve. \~
  \param[in] spiralAxis - \ru Выбор режима формообразования
                          \en A spiral construction mode. \~
  \param[out] result - \ru Спиральная кривая.
                       \en The spiral curve. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) SpiralCurve( const MbPlacement3D & place, 
                                            double          radius, 
                                            double          step, 
                                            MbCurve &       lawCurve,
                                            bool            spiralAxis,
                                            MbCurve3D *&    result ); 


//------------------------------------------------------------------------------
/** \brief \ru Создать спираль.
           \en Create a spiral. \~
  \details \ru Создать спираль. \n
    Если spiralAxis == true,  то lawCurve - определяет плоскую ось спирали. \n
    Если spiralAxis == false, то lawCurve - определяет закон изменения радиуса спирали. \n
    Если lawCurve == nullptr, то строится коническая спираль с углом конусности angle. \n
           \en Create a spiral. \n
    If 'spiralAxis' == true, 'lawCurve' determines the axis of a spiral. \n
    If spiralAxis == false, then 'lawCurve' - determines a radius law. \n
    If lawCurve == nullptr, a conical spiral is created with the specified taper angle. \n \~
  \param[in] point0 - \ru Начало локальной системы координат (ЛСК).
                      \en The origin of local coordinate system (LCS). \~
  \param[in] point1 - \ru Точка на оси Z ЛСК.
                      \en A point on Z-axis of LCS. \~
  \param[in] point2 - \ru Точка на оси X ЛСК.
                      \en A point on the X-axis of LCS. \~
  \param[in] radius - \ru Радиус спирали.
                      \en A spiral radius. \~
  \param[in] step - \ru Шаг спирали.
                    \en A pitch. \~
  \param[in] angle - \ru Угол коничности спирали.
                     \en A taper angle. \~ 
  \param[in] lawCurve - \ru Формообразующая кривая.
                        \en A guide curve. \~
  \param[in] spiralAxis - \ru Выбор режима формообразования
                          \en A spiral construction mode. \~
  \param[out] result - \ru Спиральная кривая.
                       \en The spiral curve. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) SpiralCurve( const MbCartPoint3D & point0, 
                                      const MbCartPoint3D & point1, 
                                      const MbCartPoint3D & point2,
                                            double          radius, 
                                            double          step, 
                                            double          angle, 
                                            MbCurve *       lawCurve,
                                            bool            spiralAxis,
                                            MbCurve3D *&    result );


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
  \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC (MbResultType) CreateContour( MbCurve3D & curve, 
                                        MbContour3D *& result ); 


//------------------------------------------------------------------------------
/** \brief \ru Создать копию кривой.
           \en Create a copy of a curve. \~
  \details \ru Создать копию кривой с разбивкой ломаной линии и заменой некоторых кривых. \n
           \en Create a copy of a curve with splitting of a polyline and replacement of some curves. \n \~
  \param[in] curve - \ru Исходная кривая.
                     \en The initial curve. \~
  \param[in] version - \ru Версия исполнения.
                       \en The version. \~
  \return \ru Возвращает модифицированную копию кривой, если получилось ее создать.
          \en Returns a modified copy of the curve if it has been successfully created. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbCurve3D *) DuplicateCurve( const MbCurve3D & curve, 
                                        VERSION version = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Добавить кривую в составную кривую (контур).
           \en Add a curve to a composite curve (a contour). \~ 
  \details \ru Добавить кривую curve в составную кривую (контур) contour. \n
    Если toEnd == true, то добавить в конец. \n
    Если toEnd == false, то добавить в начало. \n
           \en Add a curve to a composite curve (a contour) 'contour'. \n
    If toEnd == true, the curve is to be added to the end. \n
    If toEnd == true, the curve is to be added to the beginning. \n \~
  \param[in]     curve - \ru Добавляемая кривая
                         \en A curve to be added. \~
  \param[in,out] contour - \ru Модифицируемый контур.
                           \en A contour to be modified. \~
  \param[in]     toEnd - \ru Флаг места добавления кривой.
                         \en The flag determines the place of the curve in the contour. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC (MbResultType) AddCurveToContour( MbCurve3D & curve, 
                                            MbCurve3D & contour, 
                                            bool        toEnd ); 


//------------------------------------------------------------------------------
/** \brief \ru Построить кривую в пространстве по двумерной кривой.
           \en Create a space curve from a two-dimensional curve. \~
  \details \ru Построить кривую в пространстве по двумерной кривой curve на плоскости place. \n
           Построение выполняется на оригинале кривой.
           \en Create a space curve from a two-dimensional curve 'curve' lying on plane 'place'. \n
           The construction is performed on the source curve. \~
  \param[in] place - \ru Система координат плоскости.
                     \en The plane coordinate system. \~
  \param[in] curve - \ru Двумерная кривая
                     \en A two-dimensional curve \~
  \param[out] result - \ru Плоская кривая.
                       \en The planar curve. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) PlaneCurve( const MbPlacement3D & place, 
                                     const MbCurve &       curve, 
                                           MbCurve3D *&    result );

//------------------------------------------------------------------------------
/** \brief \ru Построить кривую на поверхности по двумерной кривой.
           \en Create a curve on a surface given a two-dimensional curve. \~
  \details \ru Построить кривую на поверхности surface по двумерной кривой curve. \n
           Построение выполняется на оригиналах кривой и поверхности.
           \en Create a curve on a surface 'surface' given a two-dimensional curve 'curve'. \n
           The construction is performed on the original curve and surface. \~
  \param[in] surface - \ru Поверхность.
                       \en A surface. \~
  \param[in] curve - \ru Двумерная кривая
                     \en A two-dimensional curve \~
  \param[out] result - \ru Поверхностная кривая.
                       \en The curve on the specified surface. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) SurfaceCurve( const MbSurface &  surface, 
                                       const MbCurve &    curve, 
                                             MbCurve3D *& result );


//------------------------------------------------------------------------------
/** \brief \ru Создать поверхностную кривую, если пространственная кривая лежит на поверхности.
           \en Create a curve on a surface from a space curve lying on the surface. \~
  \details \ru Создать поверхностную кривую, если пространственная кривая лежит на поверхности. \n
    Разбираются частные случае точной принадлежности кривой выбранной поверхности.
    В общем случае пространственная кривая считается принадлежащей поверхности, если группа точек,
    полученная шаганием по параметру кривой по угловому отклонению, принадлежит поверхности.
    В этом случае создается двумерная проекционная кривая этой кривой на поверхности. \n
           \en Create a curve on a surface from a space curve lying on the surface. \n
    The special cases of curves exactly lying on the specified surfaces are treated.
    In the general case a space curve is considered to lie on the surface if a group of points
    obtained by sampling the curve with the given turning angle belongs to the surface.
    In this case a two-dimensional curve is created as the projection of the curve on the surface. \n \~
  \param[in] curve - \ru Пространственная кривая.
                     \en A space curve. \~
  \param[in] surf - \ru Поверхность.
                    \en A surface. \~
  \param[in] sameSurf - \ru Использовать оригинал поверхности.
                        \en Whether to use the source surface. \~
  \param[in] extSurf - \ru Искать на расширенной поверхности.
                       \en Whether to use the extended surface. \~
  \param[in] strictOnSurface - \ru Все точки кривой лежат на поверхности (true) или часть точек лежит на поверхности (false).
                               \en All the points of the curve belong to the surface (true) or a part of the points belong to the surface (false). \~
  \param[out] result - \ru Поверхностная кривая.
                       \en The curve on the specified surface. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC (MbResultType) CurveOnSurface( const MbCurve3D & curve, 
                                         const MbSurface & surf, 
                                         bool sameSurf, bool extSurf,
                                         MbCurve3D *& result, 
                                         bool strictOnSurface = false );


//------------------------------------------------------------------------------
/** \brief \ru Лежит ли кривая на поверхности.
           \en Determine whether the curve lies on the surface. \~
  \details \ru Проверить, лежит ли кривая полностью на поверхности. \n
           \en Determine whether the curve entirely lies on the surface. \n \~
  \param[in] curve - \ru Пространственная кривая.
                     \en A space curve. \~
  \param[in] surf - \ru Поверхность.
                    \en A surface. \~
  \param[in] ext - \ru Искать на расширенной поверхности.
                   \en Whether to use the extended surface. \~
  \param[in] strictOnSurface - \ru Все точки кривой лежат на поверхности (true) или часть точек лежит на поверхности (false).
                               \en All the points of the curve belong to the surface (true) or a part of the points belong to the surface (false). \~
  \return \ru Возвращает true, если кривая лежит на поверхности.
          \en Returns true if the curve lies on the surface. \~
  \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC (bool) IsCurveOnSurface( const MbCurve3D & curve, 
                                   const MbSurface & surf, bool ext,
                                   bool strictOnSurface, // = false
                                   VERSION version = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Создать массив контуров по массиву кривых.
           \en Create an array of contours given an array of curves. \~
  \details \ru Создать массив контуров по массиву кривых (на оригиналах кривых). Вырожденные кривые,
           кривые с нулевым указателем и дубликаты кривых удаляются из входного массива, если у них нет владельца. \n
           \en Create an array of contours given an array of curves (using the original curves). Degenerate curves,
           curves with a null pointer and duplicates of the curves are removed from the input array if they do not have an owner. \n \~
  \param[in] curves - \ru Множество кривых.
                      \en An array of curves. \~
  \param[in] metricEps - \ru Радиус захвата для стыковки кривых.
                         \en The radius for curves joining. \~
  \param[out] result - \ru Множество контуров.
                       \en The array of contours. \~
  \param[in] onlySmoothConnected - \ru Добавлять в контур только гладко стыкующиеся сегменты.
                                   \en Whether to add only smoothly connected segments. \~
  \param[in] version - \ru Версия исполнения.
                       \en Version. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) CreateContours( RPArray<MbCurve3D> & curves, 
                                         double metricEps,
                                         RPArray<MbContour3D> & result,
                                         bool onlySmoothConnected = false,
                                         VERSION version = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Создать контуры по набору кривых с удалением вырожденных.
           \en Create contours given a set of curves with elimination of degenerate ones. \~
  \details \ru Создать контуры по набору кривых с удалением вырожденных (на оригиналах кривых). Вырожденные кривые,
           кривые с нулевым указателем и дубликаты кривых удаляются из входного массива, если у них нет владельца. \n
           \en Create contours given a set of curves with elimination of degenerate ones (the original curves are used).
           Degenerate curves, curves with a null pointer and duplicates of the curves are removed from the input array
           if they do not have an owner. \n \~
  \param[in, out] curves - \ru Множество кривых.
                           \en An array of curves. \~
  \param[in] metricAcc - \ru Радиус захвата для стыковки кривых.
                         \en The radius for curves joining. \~
  \param[in] onlySmoothConnected - \ru Добавлять в контур только гладко стыкующиеся сегменты.
                                   \en Whether to add only smoothly connected segments. \~
  \param[in] version - \ru Версия исполнения.
                       \en Version. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC (MbResultType) CreateContours( RPArray<MbCurve3D> & curves, 
                                         double metricAcc,
                                         bool onlySmoothConnected = false,
                                         VERSION version = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Создать именованный трехмерный каркас.
           \en Create a named three-dimensional wireframe. \~
  \details \ru Создать именованный трехмерный каркас по кривой. \n
           \en Create a named three-dimensional wireframe from a curve. \n \~
  \param[in] curve - \ru Кривая.
                     \en The curve. \~
  \param[in] curveName - \ru Имя кривой.
                         \en The curve name. \~
  \param[in] mainName - \ru Главное имя операции.
                        \en The operation main name. \~
  \param[out] result - \ru Каркас.
                       \en The wireframe. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) WireFrame( const MbCurve3D &    curve, 
                                    const MbName &       curveName, 
                                          SimpleName     mainName, 
                                          MbWireFrame *& result );


//------------------------------------------------------------------------------
/** \brief \ru Создать именованный трехмерный каркас.
           \en Create a named three-dimensional wireframe. \~
  \details \ru Создать именованный трехмерный каркас по массиву кривых. \n
           \en Create a named three-dimensional wireframe from a given array of curves. \n \~
  \param[in] curves - \ru Множество кривых.
                      \en An array of curves. \~
  \param[in] curveNames - \ru Множество имен кривых.
                          \en An array of the curves names. \~
  \param[in] mainName - \ru Главное имя операции.
                        \en The operation main name. \~
  \param[out] result - \ru Каркас.
                       \en The wireframe. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) WireFrame( const RPArray<MbCurve3D> & curves, 
                                    const RPArray<MbName> &    curveNames,
                                          SimpleName           mainName, 
                                          MbWireFrame *&       result );


//------------------------------------------------------------------------------
/** \brief \ru Аппроксимировать контур дугами и отрезками.
           \en Approximate a contour with arcs and line segments. \~
  \details \ru Аппроксимировать контур дугами и отрезками. \n
           Производится аппроксимация каждого из сегментов контура. \n
           \en Approximate a contour with arcs and line segments. \n
           The approximation is performed for each segment of the contour. \n \~
  \param[in] curve - \ru Кривая или контур, которую надо аппроксимировать.
                     \en A curve or a contour to approximate. \~
  \param[out] result - \ru Результат аппроксимации.
                       \en The approximation result. \~
  \param[in] eps - \ru Ошибка аппроксимации.
                   \en The approximation precision. \~
  \param[in] minRad - \ru Минимально  допустимый радиус окружностей, используемых для аппроксимации.
                      \en The minimal acceptable radius of the circles used for the approximation. \~
  \param[in] maxRad - \ru Максимально допустимый радиус окружностей, используемых для аппроксимации.
                      \en The maximal acceptable radius of the circles used for the approximation. \~
  \return \ru - Возращает код результата операции.
          \en - Returns operation result code. \~
\ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) CreatePolyArcCurve3D( const MbCurve3D &  curve, 
                                                     MbCurve3D *& result, 
                                                     double &     eps, 
                                                     double       minRad = Math::minRadius, 
                                                     double       maxRad = Math::maxRadius );


//------------------------------------------------------------------------------
/** \brief \ru Получить или создать пространственную кривую.
           \en Get or create a space curve. \~
  \details \ru Получить или создать пространственную кривую из данных модельного объекта
           c опциональным сохранением несущей плоскости в случае плоского объекта. \n
           Двумерный контур на плоскости преобразуется не в контур на плоскости,
           а в пространственный контур. \n
           \en Get or create a space curve from a model object data.
           with optional keeping of supporting plane in the case of a planar object. \n
           A two-dimensional contour is to be converted not to a contour on a plane
           but to a three-dimensional contour. \n \~
  \param[in] item - \ru Модельный объект.
                    \en A model object. \~
  \param[in] keepPlacement - \ru Сохранять несущую плоскость.
                             \en Whether to keep the supporting plane. \~
  \param[out] curve0 - \ru Пространственная кривая.
                       \en A space curve. \~
  \param[out] curves - \ru Дополнительные пространственные кривые (могут быть в объекте MbPlaneInstance).
                       \en Additional space curve (It's possible if a object "MbItem" is the object "MbPlaneInstance"). \~
  \return \ru - Возвращает успешность результата операции.
          \en - Returns whether the result is successful. \~
\ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (bool) GetSpaceCurve( MbItem & item, 
                                bool keepPlacement, 
                                c3d::SpaceCurveSPtr & curve0,
                                c3d::SpaceCurvesSPtrVector * curves = nullptr );


//-------------------------------------------------------------------------------
/** \brief \ru Построить развертку кривой/контура на плоскость.
           \en Construct a unwrapping curve/contour. \~
  \details \ru Построение развертки кривой/контура на плоскость. Контур разворачивается посегментно, функцией UnwrapCurve.
          Первым разворачивается сегмент, который лежит в плоскости и находиться ближе всех к точке, 
          если лежащих в плоскости нет, то ближащий к точке из тех что пересекают контур, если и таких нет то просто ближайший к точке. 
          Первый сегмент разворачивается так, чтобы точка пересечения или ближайшей проекции была неподвижной. \n
           \en Construction unwrapping of the curve/contour on a plane. Each segment of the contour is unwrapped by the function UnwrapCurve.
          Unwrapping starting with a segment which lies in the plane or crosses the plane and which is closest to the given point.
          Unwrapping of  the first segment is constructed in the manner, that cross point or closest projection of the segment on the plane were stationary. \n \~
  \param[in] curve - \ru Разворачиваемая кривая/контур.
                     \en Original curve/contour. \~
  \param[in] placement - \ru Локальная система координат плоскости.
                         \en The placement of the plane. \~
  \param[in] point -     \ru Точка для определения первого сегмента.
                         \en The point for determine first segment. \~
  \param[in] deviationAngle - \ru Параметру точности.
                              \en The parameter of accuracy. \~
  \return \ru Возвращает указатель на построенную кривую с нулевум счетчиком ссылок \n
          или nullptr, если не удалось построить развертку для заданных параметров.
          \en The pointer to the constructed curve with zero counter of references\n
          return nullptr, if unwrap curve can't be construvted for this parameters
\ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbCurve3D *) UnwrapCurve( const MbCurve3D &     curve, 
                                     const MbPlacement3D & placement, 
                                     const MbCartPoint3D & point, 
                                           double          deviationAngle = DEVIATION_SAG);


//------------------------------------------------------------------------------
/** \brief \ru Создать сечение кинематического тела для заданного параметра на направляющей.
           \en Create a section of evolution solid for defined parameter on the guide curve. \~
  \details \ru Создать сечение кинематического тела для заданного параметра на направляющей. 
               Если направляющая является контуром, то стыки между сегментами контура должны быть гладкими. \n
           \en Create a section of evolution solid (sweep solid with guide curve) for defined parameter on guide curve. 
               If the guide curve is a contour, then this contour have to be smooth. \n \~
  \param[in] generCurves - \ru Множество плоских образующих.
                           \en An array of forming curves. \~
  \param[in] guideCurve - \ru Направляющая кривая (или контур).
                          \en An guide curve (or contour). \~
  \param[in] guideParam - \ru Параметр, заданный на направляющей кривой.
                          \en A parameter on the guide curve. \~
  \param[in] params     - \ru Параметры операции.
                          \en The operation parameters. \~
  \param[out] result -    \ru Кривые сечения кинематического тела.
                          \en Curves of evolution solid section. \~
  \param[in] version -    \ru Версия исполнения.
                          \en The version. \~
  \param[in] angleEpsilon - \ru Желаемая угловая точность параллельности касательных в точке стыка сегментов контура.
                            \en The desired angular precision parallel to the tangent at the point of junction path segments. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) EvolutionSection( const MbSweptData &     generCurves, 
                                           const MbCurve3D &       guideCurve, 
                                                 double            guideParam, 
                                           const EvolutionValues & params,
                                                 MbSweptData &     result, 
                                                 VERSION           version = Math::DefaultMathVersion(),
                                                 double            angleEpsilon = ANGLE_EPSILON );


//------------------------------------------------------------------------------
// Является ли кривая прямолинейной независимо от ее параметризации.
/** \brief \ru Является ли кривая прямолинейной независимо от ее параметризации.
           \en Whether the curve is like straight-line regardless of its parameterisation. \~
  \details \ru Является ли кривая прямолинейной независимо от ее параметризации.\n
           \en Whether the curve is like straight-line regardless of its parameterisation. \~
  \param[in] curve - \ru Кривая.
                     \en Curve. \~
  \param[in] eps   - \ru Точность.
                     \en Accuracy. \~
  \return \ru Возвращает true, если кривая геометрически прямолинейна.
          \en Returns true, if a curve is geometrically straight. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (bool) IsLikeStraightLine( const MbCurve3D & curve, double eps );


//------------------------------------------------------------------------------
/** \brief \ru Создать осевые (центральные) линии для грани оболочки.
           \en Create center lines of shell face. \~
  \details \ru Создать осевые (центральные) линии для грани оболочки. \n
           \en Create center lines of shell face. \n \~
  \param[in]  face     - \ru Исходная грань.
                         \en The initial face. \~
  \param[out] clCurves - \ru Набор построенных осевых линий.
                         \en The set of created center lines. \~
  \return \ru Возвращает true, если осевые кривые удалось создать.
          \en Returns true if the center lines was created. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC( bool ) CreateCenterLineCurves( const MbFace &                   face,
                                                std::vector<MbCurve3D *> & clCurves );


//------------------------------------------------------------------------------
  /** \brief \ru Создать плавную B-сплайновую кривую на опорной ломаной.
             \en Create a fair B-spline on base polyline. \~
    \details \ru Создать плавную V-кривую на опорной ломаной и аппроксимировать B-сплайновой кривой. 
             Степень сплайна m, (m = 3, 4, ... , 9, 10) устанавливается в переменной degreeBSpline. 
             Для гармоничного перераспределения точек значение переменной arrange == true. В противном случае == false. 
             Для уплотнения кривой значение переменной subdivision > 0 (1 - для однократного уплотнения, 2 - для двукратного). 
             Направление вектора в точках перегиба учитывается по значению переменной accountInflexVector (0 - как направление звена S-полигона, 
             1 - как направление касательного вектора). \n
             \en Create a fair V-curve on the base polyline and approximate by a B-spline curve. 
             The degree of spline m, (m = 3, 4, ... , 9, 10) is set by variable degreeBSpline. 
             For harmonious redistribution of points, the value of the variable arrange == true. Otherwise, == false. 
             To subdivide a curve, the value of the variable subdivision > 0 (1 for a single subdivision, 2 for a double subdivision).
             The direction of the vector at the inflection points is taken into account by the value of the variable InflexVector (0 - as the direction of the S-polygon segment, 
             1 - as the direction of the tangent vector). \n \~
    \attention  \ru Экспериментальный класс. \en Experimental class. \~
    \param[in]  polyline - \ru Исходная ломаная.
                           \en An initial polyline. \~
    \param[in]  data     - \ru Данные построения кривой.
                           \en The curve construction data. \~
    \param[out] result   - \ru Сплайновая кривая.
                           \en The spline curve. \~
    \return \ru Возвращает код результата операции.
            \en Returns operation result code. \~
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC (MbResultType) CreateFairBSplineCurveOnBasePolyline( MbCurve3D *       polyline,
                                                               MbFairCurveData & data,
                                                               MbCurve3D *&      result );


//------------------------------------------------------------------------------
  /** \brief \ru Создать плавную B-сплайновую кривую на касательной ломаной.
             \en Create a fair B-spline on tangent polyline. \~
    \details \ru Создать плавную V-кривую на касательной ломаной и аппроксимировать B-сплайновой кривой. 
             Степень сплайна m, (m = 3, 4, ... , 9, 10) устанавливается в переменной degreeBSpline. 
             Для уплотнения кривой устанавливается значение переменной subdivision > 0 (1 - для однократного уплотнения, 2 - для двукратного). 
             Направление вектора в точках перегиба кривой совпадает с направлением звена перегиба касательной ломаной. \n
             \en Create a fair V-curve on the base polyline and approximate by a B-spline curve. 
             The degree of spline m, (m = 3, 4, ... , 9, 10) is set by variable degreeBSpline. 
             To subdivide a curve, the value of the variable subdivision > 0 (1 for a single subdivision, 2 for a double subdivision). 
             The direction of the tangent vector at the inflection points coincides with the direction of the inflection segment of tangent polyline). \n \~
    \param[in]  polyline - \ru Исходная ломаная.
                           \en An initial polyline. \~
    \param[in]  data     - \ru Данные построения кривой.
                           \en The curve construction data. \~
    \param[out] result   - \ru Сплайновая кривая.
                           \en The spline curve. \~
    \return \ru Возвращает код результата операции.
            \en Returns operation result code. \~
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC (MbResultType) CreateFairBSplineCurveOnTangentPolyline( MbCurve3D *       polyline,
                                                                  MbFairCurveData & data,
                                                                  MbCurve3D *&      result );


//------------------------------------------------------------------------------
  /** \brief \ru Создать плавную кривую Безье на опорной ломаной.
             \en Create a fair Bezier curve on base polyline. \~
    \details \ru Создать плавную V-кривую на опорной ломаной и аппроксимировать рациональной кубической кривой Безье. 
             Для гармоничного перераспределения точек значение переменной arrange == true. В противном случае == false. 
             Для уплотнения кривой значение переменной subdivision > 0 (1 - для однократного уплотнения, 2 - для двукратного).\n 
             \en Create a smooth V-curve on the reference polyline and approximate a rational cubic Bezier curve. 
             For harmonious redistribution of points, the value of the variable arrange == true. Otherwise, == false. 
             To subdivide a curve, the value of the variable subdivision > 0 (1 for a single subdivision, 2 for a double subdivision). \n \~
    \param[in]  polyline - \ru Исходная ломаная.
                           \en An initial polyline. \~
    \param[in]  data     - \ru Данные построения кривой.
                           \en The curve construction data. \~
    \param[out] result   - \ru Сплайновая кривая.
                           \en The spline curve. \~
    \return \ru Возвращает код результата операции.
            \en Returns operation result code. \~
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC(MbResultType) CreateFairBezierCurveOnBasePolyline( MbCurve3D *       polyline,
                                                             MbFairCurveData & data,
                                                             MbCurve3D *&      result);

//------------------------------------------------------------------------------
/** \brief \ru Создать плавную кривую Безье сопряжения на опорной ломаной.
           \en Create a fillet fair Bezier curve on base polyline. \~
    \details \ru Создать плавную V-кривую на опорной ломаной и аппроксимировать рациональной кубической кривой Безье.
             \en Create a fillet fair Bezier curve on base polyline and approximate a rational cubic Bezier curve. \~

    \param[in]  polyline - \ru Исходная ломаная. \en An initial polyline. \~
    \param[in]  data     - \ru Данные построения кривой. \en The curve construction data. \~
    \param[out] resCurve - \ru Сплайновая кривая. \en The spline curve. \~
    \return                \ru Возвращает код результата операции. \en Returns operation result code. \~
    \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC( MbResultType ) CreateFairBezierFilletOnPolyline( MbCurve3D *       pllne,
                                                            MbFairCurveData & data,
                                                            MbCurve3D *&      resCurve );


//------------------------------------------------------------------------------
  /** \brief \ru Создать плавную кривую Безье на касательной ломаной.
             \en Create a fair Bezier curve on tangent polyline. \~
    \details \ru Создать плавную V-кривую на касательной ломаной и аппроксимировать рациональной кубической кривой Безье. 
             Для уплотнения кривой значение переменной subdivision > 0 (1 - для однократного уплотнения, 2 - для двукратного). \n
             \en Create a smooth V-curve on a tangent polyline and approximate a rational cubic Bezier curve. 
             To subdivide a curve, the value of the variable subdivision > 0 (1 for a single subdivision, 2 for a double subdivision). \n \~
    \param[in]  polyline - \ru Исходная ломаная.
                           \en An initial polyline. \~
    \param[in]  data     - \ru Данные построения кривой.
                           \en The curve construction data. \~
    \param[out] result   - \ru Сплайновая кривая.
                           \en The spline curve. \~
    \return \ru Возвращает значение результата операции. 0 - при успешном построении кривой. При > 0 значение равно номеру сообщения об ошибке из списка сообщений метода MessageError.
            \en Returns operation result value. 0 - upon successful creation of the curve. If > 0, the value is equal to the error message number.from the message list of the MessageError method. \~
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC (MbResultType) CreateFairBezierCurveOnTangentPolyline( MbCurve3D *       polyline,
                                                                 MbFairCurveData & data,
                                                                 MbCurve3D *&      result );


//------------------------------------------------------------------------------
  /** \brief \ru Создать ГО Эрмита на кривой Безье.
             \en Create Hermite GD on a Bezier curve. \~
    \details \ru Создать геометрический определитель (ГО) Эрмита в виде ломаной линии на рациональной сплайновой кривой Безье. \n
             Параметр type определяет тип создаваемой кривой: ломаная, касательная ломаная или ГО Эрмита.
             Если задан тип "ГО Эрмита", то на кривой определяется опорная ломаная, вершины которой принадлежат узловым точкам сплайна. 
             Определяются векторы касательных и векторы кривизны. Вектор касательной может иметь произвольную длину. 
             Вектор кривизны должет иметь длину, равную значению кривизны в данной вершине опорной ломаной. 
             Если значение кривизны равно нулю в точке перегиба, то вектор кривизны откладывается по направлению касательного вектора в данной вершине 
             и имеет произвольную ненулевую длину, не превышающую длину касательного вектора. 
             Направленная ломаная ГО Эрмита последовательно обходит вершины опорной ломаной, концы касательных векторов и концы векторов кривизны.
             Обход происходит следующим образом: от вершины опорной к концу касательного вектора, затем возврат к вершине опорной ломаной, 
             переход к концу вектора кривизны, возврат к вершине опорной ломаной, затем переход к следующей вершине опорной ломаной и т.д.
             Вершина с номером 1 и вершины с номерами через 5 (1, 6, 11, ... ) принадлежат концам касательных векторов.  
             Вершина с номером 3 и вершины с номерами через 5 (3, 8, 13, ... ) принадлежат концам векторов кривизны.  
             \en Create a Hermite geometric determinant (GD) in the form of a polyline on a rational Bezier spline curve.  \n
             The type parameter defines a type of created curve: polyline, tangent polyline, or Hermite GD.
             If the Hermite GD type is defined, then a base polyline is defined on the curve, the vertices of which belong to the nodal points of the spline.  
             The tangent vectors and the curvature vectors are determined. The tangent vector can be of arbitrary length.  
             The curvature vector must have a length equal to the value of curvature at a given vertex of the base polyline.  
             If the curvature value is zero at the inflection point, then the curvature vector is set in the direction of the tangent vector at the given vertex  
             and has an arbitrary nonzero length not exceeding the length of the tangent vector. \ n
             A directional polyline of Hermite GD sequentially bypasses the vertices of the  base polyline, the ends of the tangent vectors and the ends of the curvature vectors.  
             Bypass occurs as follows: from the vertex of the base polyline to the end of the tangent vector, then return to the vertex of the base polyline, 
             transition to the end of the curvature vector, return to the vertex of the base polyline, then transition to the next vertex of the base polyline, etc.
             The vertex with number 1 and the vertices with numbers in 5 (1, 6, 11, ...) belong to the ends of the tangent vectors.  
             The vertex with number 3 and the vertices with numbers in 5 (3, 8, 13, ...) belong to the ends of the curvature vectors. \~
    \param[in]  curve -    \ru Исходная кривая.
                           \en An initial curve. \~
    \param[in]  type -     \ru Тип создаваемой кривой. Допустимые значения: fairCrvType_Poly, fairCrvType_PolyTangent, fairCrvType_HermiteGD.
                           \en A type of a constructed curve. The allowed values: fairCrvType_Poly, fairCrvType_PolyTangent, fairCrvType_HermiteGD. \~
    \param[in]  data     - \ru Данные построения кривой.
                           \en The curve construction data. \~
    \param[out] polyline - \ru 3D ломаная ГО Эрмита.
                           \en 3D polyline of Hermite GD. \~
    \return \ru Возвращает значение результата операции. 0 - при успешном построении кривой. При > 0 значение равно номеру сообщения об ошибке из списка сообщений метода MessageError.
            \en Returns operation result value. 0 - upon successful creation of the curve. If > 0, the value is equal to the error message number from the message list of the MessageError method.\~
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC(MbResultType) KernelCreateHermiteGDOnBezierCurve( const MbNurbs3D *       curve,
                                                                  MbeFairCurveType  type,
                                                                  MbFairCurveData & data,
                                                                  MbCurve3D *&      polyline );
  
  
//------------------------------------------------------------------------------
  /** \brief \ru Создать кривую Безье на ГО Эрмита.
             \en Create a Bezier on Hermite GD. \~
    \details \ru Изогеометрически построить рациональную кубическую кривую Безье на ГО Эрмита второго порядка фиксации. 
             Используются все параметры ГО Эрмита второго порядка фиксации: точки, касательные векторы и значения кривизны. 
             \en Isogeometrically create a cubic rational Bezier curve on a Hermite GD of second-order  fixation. 
             Used all params of Hermite GD of second-order  fixation: points, tangent vectors and values of curvature. \~ 
    \param[in]  polyline - \ru Исходная ломаная.
                           \en An initial polyline. \~
    \param[in]  data     - \ru Данные построения кривой.
                           \en The curve construction data. \~
    \param[out] result -   \ru Сплайновая кривая.
                           \en The spline curve. \~
    \return \ru Возвращает значение результата операции. 0 - при успешном построении кривой. При > 0 значение равно номеру сообщения об ошибке из списка сообщений метода MessageError.
            \en Returns operation result value. 0 - upon successful creation of the curve. If > 0, the value is equal to the error message number from the message list of the MessageError method.\~
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC (MbResultType) CreateBezierCurveOnHermiteGD( MbCurve3D *       polyline,
                                                       MbFairCurveData & data,
                                                       MbCurve3D *&      result );


//------------------------------------------------------------------------------
  /** \brief \ru Создать B-сплайновую кривую на ГО Эрмита.
             \en Create a B-spline on Hermite GD. \~
    \details \ru Создать B-сплайную кривую на ГО Эрмита первого порядка фиксации. \n
                 Направления касательных векторов ГО Эрмита определяют направления звеньев S-полигона.
                 Направление вектора в точках перегиба учитывается по значению переменной accountInflexVector (0 - как направление звена S-полигона, \n
                 1 - как направление касательного вектора). \n
                 Кривизна в концевых точках учитывается по значению accountCurvature (0 - не учитывается,  1 - в начальной точке, 2 - в конечной точке, 3 - учитываются на обоих концах).
             \en Create a B-spline curve on Hermite's GD of first-order fixation. \ n
                The directions of the tangent vectors of the Hermite GD determine the directions of the S-polygon segmentss. \n
                The direction of the vector at the inflection points is taken into account by the value of the variable InflexVector (0 - as the direction of the S-polygon segment,
                1 - as the direction of the tangent vector). \n
                The curvature at the end points is taken into account by the value of accountCurvature (0 - not taken into account, 1 - at the start point, 2 - at the end point, 3 - are taken into account at both ends). \~
    \param[in]  polyline - \ru Исходная ломаная.
                           \en An initial polyline. \~
    \param[in]  data     - \ru Данные построения кривой.
                           \en The curve construction data. \~
    \param[out] result -   \ru Сплайновая кривая.
                           \en The spline curve. \~
    \return \ru Возвращает значение результата операции. 0 - при успешном построении кривой. При > 0 значение равно номеру сообщения об ошибке из списка сообщений метода MessageError.
            \en Returns operation result value. 0 - upon successful creation of the curve. If > 0, the value is equal to the error message number from the message list of the MessageError method.\~
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC(MbResultType) CreateBSplineCurveOnHermiteGD( MbCurve3D *       polyline,
                                                       MbFairCurveData & data,
                                                       MbCurve3D *&      result );


//------------------------------------------------------------------------------
  /** \brief \ru Создать плавную кривую Безье на опорной ломаной ГО Эрмита.
             \en Create a fair Bezier curve on base polyline of Hermite GD. \~
    \details \ru Создать плавную V-кривую на опорной ломаной ГО Эрмита и аппроксимировать рациональной кубической кривой Безье. \n
                 Учитываются концевые значения кривизны и направления касательных в точках перегиба ГО Эрмита.
                 Для гармоничного перераспределения точек значение переменной arrange == true. В противном случае == false.
                 Для уплотнения кривой значение переменной subdivision > 0 (1 - для однократного уплотнения, 2 - для двукратного).
             \en Create a fair V-curve on the base polyline and approximate by a rational cubic Bezier curve. \ n
                 The end values of the curvature and directions of the tangents at the inflection points of the Hermite GO are taken into account. 
                 For harmonious redistribution of points, the value of the variable arrange == true. Otherwise, == false.
                 To subdivide a curve, the value of the variable subdivision> 0 (1 for a single subdivision, 2 for a double subdivision).\~
    \param[in]  polyline - \ru Исходная ломаная.
                           \en An initial polyline. \~
    \param[in]  data     - \ru Данные построения кривой.
                           \en The curve construction data. \~
    \param[out] result -   \ru Сплайновая кривая.
                           \en The spline curve. \~
    \return \ru Возвращает значение результата операции. 0 - при успешном построении кривой. При > 0 значение равно номеру сообщения об ошибке из списка сообщений метода MessageError.
            \en Returns operation result value. 0 - upon successful creation of the curve. If > 0, the value is equal to the error message number from the message list of the MessageError method.\~
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC(MbResultType) CreateFairBezierCurveOnBasePolylineOfHermiteGD( MbCurve3D *       polyline,
                                                                        MbFairCurveData & data,
                                                                        MbCurve3D *&      result );


//------------------------------------------------------------------------------
  /** \brief \ru Создать плавную кривую Безье на касательных прямых ГО Эрмита.
             \en Create a fair Bezier curve on tangent lines of Hermite GD. \~
    \details \ru Создать плавную V-кривую на касательных прямых ГО Эрмита и аппроксимировать рациональной кубической кривой Безье. \n
                 Учитываются концевые значения кривизны и положения точек перегиба ГО Эрмита.
                 Для уплотнения кривой значение переменной subdivision > 0 (1 - для однократного уплотнения, 2 - для двукратного).
             \en Create a fair V-curve on the base polyline and approximate by a rational cubic Bezier curve. \ n
                 The end values of the curvature and directions of the tangents at the inflection points of the Hermite GO are taken into account. 
                 To subdivide a curve, set the value of the variable subdivision > 0 (1 for a single subdivision, 2 for a double subdivision).\~
    \param[in]  polyline - \ru Исходная ломаная.
                           \en An initial polyline. \~
    \param[in]  data     - \ru Данные построения кривой.
                           \en The curve construction data. \~
    \param[out] result -   \ru Сплайновая кривая.
                           \en The spline curve. \~
    \return \ru Возвращает значение результата операции. 0 - при успешном построении кривой. При > 0 значение равно номеру сообщения об ошибке \n
                из списка сообщений метода MessageError.
            \en Returns operation result value. 0 - upon successful creation of the curve. If > 0, the value is equal to the error message number \ n
                from the message list of the MessageError method.\~
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC (MbResultType) CreateFairBezierCurveOnTangentsOfHermiteGD( MbCurve3D *       polyline,
                                                                     MbFairCurveData & data,
                                                                     MbCurve3D *&      result );


//------------------------------------------------------------------------------
  /** \brief \ru Создать плавную B-сплайновую кривую на опорной ломаной ГО Эрмита.
             \en Create a fair B-spline curve on base polyline of Hermite GD. \~
    \details \ru Создать плавную V-кривую на опорной ломаной и аппроксимировать B-сплайновой кривой. \n
                 Степень сплайна m, (m = 3, 4, ... , 9, 10) устанавливается в переменной degreeBSpline. 
                 Для гармоничного перераспределения точек значение переменной arrange == true. В противном случае == false.
                 Для уплотнения кривой значение переменной subdivision > 0 (1 - для однократного уплотнения, 2 - для двукратного).
                 Направление вектора в точках перегиба учитывается по значению переменной accountInflexVector (0 - как направление звена S-полигона, 
                 1 - как направление касательного вектора). \n
                 Кривизна в концевых точках учитывается по значению accountCurvature (0 - не учитывается,  1 - в начальной точке, 2 - в конечной точке, 3 - учитываются на обоих концах).
             \en Create a fair V-curve on the base polyline and approximate by a B-spline curve. \ n
                 The degree of spline m, (m = 3, 4, ... , 9, 10) is set by variable degreeBSpline.
                 For harmonious redistribution of points, the value of the variable arrange == true. Otherwise, == false.
                 To subdivide a curve, the value of the variable subdivision > 0 (1 for a single subdivision, 2 for a double subdivision).
                 The directions of the tangent vectors of the Hermite GD determine the directions of the S-polygon segmentss. \n
                 The direction of the vector at the inflection points is taken into account by the value of the variable InflexVector (0 - as the direction of the S-polygon segment, \ n
                 1 - as the direction of the tangent vector). \n
                 The curvature at the end points is taken into account by the value of accountCurvature (0 - not taken into account, 1 - at the start point, 2 - at the end point, 3 - are taken into account at both ends). \~
    \attention  \ru Экспериментальный класс. \en Experimental class. \~
    \param[in]  polyline - \ru Исходная ломаная.
                           \en An initial polyline. \~
    \param[in]  data     - \ru Данные построения кривой.
                           \en The curve construction data. \~
    \param[out] result -   \ru Сплайновая кривая.
                           \en The spline curve. \~
    \return \ru Возвращает значение результата операции. 0 - при успешном построении кривой. При > 0 значение равно номеру сообщения об ошибке из списка сообщений метода MessageError.
            \en Returns operation result value. 0 - upon successful creation of the curve. If > 0, the value is equal to the error message number from the message list of the MessageError method.\~
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC (MbResultType) CreateFairBSplineCurveOnBasePolylineOfHermiteGD( MbCurve3D *       polyline,
                                                                          MbFairCurveData & data,
                                                                          MbCurve3D *&      result );


//------------------------------------------------------------------------------
  /** \brief \ru Создать плавную и сглаживающую B-сплайновую кривую на опорной ломаной ГО Эрмита.
             \en Create a fair B-spline curve on base polyline of Hermite GD. \~
    \details \ru Создать плавную V-кривую на опорной ломаной и аппроксимировать B-сплайновой кривой. \n
                 Степень сплайна m, (m = 3, 4, ... , 9, 10) устанавливается в переменной degreeBSpline.
                 Для гармоничного перераспределения точек значение переменной arrange == true. В противном случае == false.
                 Для уплотнения кривой значение переменной subdivision > 0 (1 - для однократного уплотнения, 2 - для двукратного).
                 Направление вектора в точках перегиба учитывается по значению переменной accountInflexVector (0 - как направление звена S-полигона,
                 1 - как направление касательного вектора). \n
                 Кривизна в концевых точках учитывается по значению accountCurvature (0 - не учитывается,  1 - в начальной точке, 2 - в конечной точке, 3 - учитываются на обоих концах).
             \en Create a fair V-curve on the base polyline and approximate by a B-spline curve. \ n
                 The degree of spline m, (m = 3, 4, ... , 9, 10) is set by variable degreeBSpline.
                 For harmonious redistribution of points, the value of the variable arrange == true. Otherwise, == false.
                 To subdivide a curve, the value of the variable subdivision > 0 (1 for a single subdivision, 2 for a double subdivision).
                 The directions of the tangent vectors of the Hermite GD determine the directions of the S-polygon segmentss. \n
                 The direction of the vector at the inflection points is taken into account by the value of the variable InflexVector (0 - as the direction of the S-polygon segment, \ n
                 1 - as the direction of the tangent vector). \n
                 The curvature at the end points is taken into account by the value of accountCurvature (0 - not taken into account, 1 - at the start point, 2 - at the end point, 3 - are taken into account at both ends). \~
    \attention  \ru Экспериментальный класс. \en Experimental class. \~
    \param[in]  polyline - \ru Исходная ломаная.
                           \en An initial polyline. \~
    \param[in]  data     - \ru Данные построения кривой.
                           \en The curve construction data. \~
    \param[out] result -   \ru Сплайновая кривая.
                           \en The spline curve. \~
    \return \ru Возвращает значение результата операции. 0 - при успешном построении кривой. При > 0 значение равно номеру сообщения об ошибке из списка сообщений метода MessageError.
            \en Returns operation result value. 0 - upon successful creation of the curve. If > 0, the value is equal to the error message number from the message list of the MessageError method.\~
\ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC( MbResultType ) CreateFairBSplineCurveOnBasePolylineOfHermiteGDInflex( const MbCurve3D *        pllne,
                                                                                       MbFairCurveData &  data,
                                                                                       MbCurve3D *&       resCurve );


// ------------------------------------------------------------------------------
// Создать плавную B-сплайновую кривую [со сглаживанием и репараметризацией] на зашумленных данных
// (опорной ломаной, ГО Эрмита)
// ---
//------------------------------------------------------------------------------
  /** \brief \ru Создать плавную B-сплайновую кривую (со сглаживанием и репараметризацией) на зашумленных данных.
             \en Create a fair B-spline curve with smoothing and reparameterization on noisy data. \~
    \details \ru Создать плавную B-сплайновую кривую [со сглаживанием и репараметризацией] на зашумленных данных
                 (опорной ломаной, ГО Эрмита).
             \en Create a fair B-spline curve with smoothing and reparametrization on noisy data (base polyline of Hermite GD). \n
    \attention  \ru Экспериментальный класс. \en Experimental class. \~
    \param[in]  polyline - \ru Исходная ломаная.
                           \en An initial polyline. \~
    \param[in]  plnNoisy - \ru Зашумленная ломаная (может быть nullptr).
                           \en The noisy polyline (can be nullptr). \~
    \param[in]  data     - \ru Данные построения кривой.
                           \en The curve construction data. \~
    \param[out] resCurve - \ru Сплайновая кривая.
                           \en The spline curve. \~
    \return \ru Возвращает значение результата операции. 0 - при успешном построении кривой. При > 0 значение равно номеру сообщения об ошибке из списка сообщений метода MessageError.
            \en Returns operation result value. 0 - upon successful creation of the curve. If > 0, the value is equal to the error message number from the message list of the MessageError method.\~
\ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC( MbResultType ) CreateFairBSplineCurveOnNoisyData( MbCurve3D* pllne,
                                                             MbCurve3D* plnNoisy,
                                                             MbFairCurveData & data,
                                                             MbCurve3D *& resCurve );


//------------------------------------------------------------------------------
  /** \brief \ru Создать плавную B-сплайновую кривую на касательных прямых ГО Эрмита.
             \en Create a fair B-spline curve on tangent lines of Hermite GD. \~
    \details \ru Создать плавную V-кривую на касательных прямых ГО Эрмита и аппроксимировать B-сплайновой кривой. \n
                 Степень сплайна m, (m = 3, 4, ... , 9, 10) устанавливается в переменной degreeBSpline. 
                 Для гармоничного перераспределения точек значение переменной arrange == true. В противном случае == false.
                 Для уплотнения кривой установите значение переменной subdivision > 0 (1 - для однократного уплотнения, 2 - для двукратного).
                 Кривизна в концевых точках учитывается по значению accountCurvature (0 - не учитывается,  1 - в начальной точке, 2 - в конечной точке, 3 - учитываются на обоих концах).
             \en Create a fair V-curve on the base polyline of Hermite GD and approximate by a B-spline curve. \ n
                 The degree of spline m, (m = 3, 4, ... , 9, 10) is set by variable degree.
                 For harmonious redistribution of points, the value of the variable arrange == true. Otherwise, == false.
                 To subdivide a curve, set the value of the variable subdivision > 0 (1 for a single subdivision, 2 for a double subdivision).
                 The curvature at the end points is taken into account by the value of accountCurvature (0 - not taken into account, 1 - at the start point, 2 - at the end point, 3 - are taken into account at both ends). \~
    \param[in]  polyline - \ru Исходная ломаная.
                           \en An initial polyline. \~
    \param[in]  data     - \ru Данные построения кривой.
                           \en The curve construction data. \~
    \param[out] result -   \ru Сплайновая кривая.
                           \en The spline curve. \~
    \return \ru Возвращает значение результата операции. 0 - при успешном построении кривой. При > 0 значение равно номеру сообщения об ошибке из списка сообщений метода MessageError.
            \en Returns operation result value. 0 - upon successful creation of the curve. If > 0, the value is equal to the error message number from the message list of the MessageError method.\~
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC(MbResultType) CreateFairBSplineCurveOnTangentsOfHermiteGD( MbCurve3D *       polyline,
                                                                     MbFairCurveData & data,
                                                                     MbCurve3D *&      result );


//------------------------------------------------------------------------------
  /** \brief \ru Создать клотоиду.
             \en Create a clothoid. \~
     \details \ru Создать начальный участок клотоиды. \n
              В переменных data задаются параметры построения клотоиды. Максимальная длина начального участка клотоиды в переменной 
              clothoidMax и минимальный радиус на конце участка в переменной clothoidRMin. В переменной clothoidSegms задается количество
              сегментов аппроксимирующей клотоиду сплайновой кривой Безье.
             \en Create the starting part of the clothoid. \ n
             The parameters to create the clothoid are set in the data. The maximum length of the initial section of the clothoid in the variable clothoidMax and the minimum radius at the end of the section in the variable clothoidRMin. 
             The  variable clothoidSegms sets the number of segments of the Bezier spline curve approximating the clothoid .\~
    \param[in]  data     - \ru Данные построения кривой.
                           \en The curve construction data. \~
    \param[out] result -   \ru Сплайновая кривая.
                           \en The spline curve. \~
    \return \ru Возвращает значение результата операции. 
            \en Returns operation result value.
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC(MbResultType) KernelCreateClothoid( MbFairCurveData & data,
                                              MbCurve3D *&      result );


//------------------------------------------------------------------------------
  /** \brief \ru Создать сектрису.
             \en Create a sectrix. \~
    \details \ru Создать сектрису Маклорена. \n
             На касательной ломаной из двух звеньев генерируются точки сектрисы Маклорена - кривой с монотонным изменением кривизны.
             Сектриса Маклорена изогеометрически с сохранением монотонности аппроксимируется сплайновой кривой Безье.
             \en Create a sectrix of Maclourin. 
             On a tangent polyline of two links, points of the Maclaurin sectrix — a curve with a monotonic change in curvature — are generated.
             The Maclaurin sectrix is isometrically approximated with the preservation of monotony by the Bezier spline curve.\~
           
    \param[in]  polyline - \ru Исходная ломаная.
                           \en An initial polyline. \~
    \param[in]  data     - \ru Данные построения кривой.
                           \en The curve construction data. \~
    \param[out] result -   \ru Сплайновая кривая.
                           \en The spline curve. \~
    \return \ru Возвращает значение результата операции. 
            \en Returns operation result value.
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC(MbResultType) KernelCreateSectrix( MbCurve3D *       polyline, 
                                             MbFairCurveData & data,
                                             MbCurve3D *&      result );


//------------------------------------------------------------------------------
  /** \brief \ru Увеличить степень NURBzS кривой.
             \en Elevate the degree of the NURBzS curve. \~
    \details \ru Увеличить степень NURBzS кривой. \n
             Степень кубической кривой увеличивается до шестой степени. 
             Степени больше 6 увеличиваются на единцу. Максимальная степень - 10.
             \en The increasing the degree of the NURBzS curve. \n
             The degree of the cubic curve increases to the sixth degree.
             Degrees greater than 6 increase by one. The maximum degree is 10.\~
    \param[in]  curve  - \ru Исходная кривая.
                         \en An initial curve. \~
    \param[in]  data   - \ru Данные построения кривой.
                         \en The curve construction data. \~
    \param[out] result - \ru Сплайновая кривая.
                         \en The spline curve. \~
    \return \ru Возвращает значение результата операции. 
            \en Returns operation result value.
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC(MbResultType) ElevateDegreeNurbzs( MbNurbs3D *       curve,
                                             MbFairCurveData & data,
                                             MbCurve3D *&      result );


//------------------------------------------------------------------------------
/** \brief \ru Репараметризировать NURBS кривую.
           \en Reparametrize the NURBS curve. \~
  \details \ru Репараметризировать NURBS кривую. \n
           \en Reparametrize the NURBS curve. \~
  \param[in]  curve    - \ru Исходная кривая.
                         \en An initial curve. \~
  \param[in]  data     - \ru Данные репараметризации кривой.
                         \en Curve reparametrization data. \~
  \param[out] resCurve - \ru Сплайновая кривая.
                         \en The spline curve. \~
  \return \ru Возвращает значение результата операции.
          \en Returns operation result value.
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC( MbResultType ) ReparamNurbs( MbNurbs3D * curve,
                                        MbFairCurveData & data,
                                        MbCurve3D *& resCurve );


//------------------------------------------------------------------------------
  /** \brief \ru Уплотнить NURBS кривую.
             \en Subdivide the NURBS curve. \~
    \details \ru Однократно уплотнить NURBS кривую. \n
             \en Single subdivision of NURBS curve. \~
    \param[in]  curve  - \ru Исходная кривая.
                         \en An initial curve. \~
    \param[in]  data   - \ru Данные построения и преобразования кривой.
                         \en Curve construction and transformation data. \~
    \param[out] result - \ru Сплайновая кривая.
                         \en The spline curve. \~
    \return \ru Возвращает значение результата операции. 
            \en Returns operation result value.
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC(MbResultType) SubdivideNurbs( MbNurbs3D *       curve,
                                        MbFairCurveData & data,
                                        MbCurve3D *&      result );


//------------------------------------------------------------------------------
  /** \brief \ru Выделить участок NURBS кривой / изменить формат NURBS кривой.
             \en To extract part of NURBS curve / change the format of the NURBS curve. \~
    \details \ru Выделить участок NURBS кривой / изменить формат NURBS кривой. \n
             В data устанавливаются переменные преобразований. Выделяемый участок определяется номером начального сегмента в переменной numSegment
             и количеством сегментов в переменной numSegments. Выходной формат сплайна определяется в переменной outFormat.
             При значении 2 - NURBS кривая с управляющим S-полигоном. При значении 3 - рациональная кривая Безье 
             с управляющим GB-полигоном. 
             \en To extract part of NURBS curve / change the format of the NURBS curve. \n
             Transformation variables are set in data. The extracting part is determined by the number of the starting segment in the variable numSegment and the number of segments in the variable numSegments. 
             The output spline format is defined in the outFormat variable.    If the value is 2 - NURBS curve with control S-polygon. If the value is 3 - rational Bezier curve with GB-control polygon.\~
    \param[in]  curve  - \ru Исходная кривая.
                         \en An initial curve. \~
    \param[in]  data   - \ru Данные построения и преобразования кривой.
                         \en Curve construction and transformation data. \~
    \param[out] result - \ru Сплайновая кривая.
                         \en The spline curve. \~
    \return \ru Возвращает значение результата операции. 
            \en Returns operation result value.
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC(MbResultType) ExtractChangeNurbs( MbNurbs3D *       curve,
                                            MbFairCurveData & data,
                                            MbCurve3D *&      result );


//------------------------------------------------------------------------------
  /** \brief \ru Выделить участок NURBS кривой / изменить формат NURBS кривой. Выдать параметры созданной кривой.
             \en To extract part of NURBS curve / change the format of the NURBS curve. Return parameters of created curve.\~
    \details \ru Выделить участок NURBS кривой / изменить формат NURBS кривой. Выдать параметры созданной кривой. \n
             В data устанавливаются переменные преобразований. Выделяемый участок определяется номером начального сегмента в переменной numSegment
             и количеством сегментов в переменной numSegments. Выходной формат сплайна определяется в переменной outFormat.
             При значении 2 - NURBS кривая с управляющим S-полигоном. При значении 3 - рациональная кривая Безье
             с управляющим GB-полигоном.
             \en To extract part of NURBS curve / change the format of the NURBS curve. Return parameters of created curve. \n
             Transformation variables are set in data. The extracting part is determined by the number of the starting segment in the variable numSegment and the number of segments in the variable numSegments.
             The output spline format is defined in the outFormat variable. If the value is 2 - NURBS curve with control S-polygon. If the value is 3 - rational Bezier curve with GB-control polygon.\~
    \param[in]  curve  -    \ru Исходная кривая. \en An initial curve. \~
    \param[in]  data   -    \ru Данные построения и преобразования кривой. \en Curve construction and transformation data. \~
    \param[out] resWts -    \ru Веса результирующей кривой. \en Weights of the result curve. \~
    \param[out] resKnots -  \ru Узлы результирующей кривой. \en Knots of the result curve. \~
    \param[out] result -    \ru Сплайновая кривая. \en The spline curve. \~
    \return \ru Возвращает значение результата операции.
            \en Returns operation result value.
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC( MbResultType ) ExtractChangeNurbsParams( MbNurbs3D *       curve,
                                                    MbFairCurveData & data,
                                                    SArray<double> &  resWts,
                                                    SArray<double> &  resKnots,
                                                    MbCurve3D *&      resCurve );

//------------------------------------------------------------------------------
  /** \brief \ru Вставить узел в NURBS.
             \en Insert node in NURBS. \~
    \details \ru Вставить узел в NURBS. \n
             В data устанавливаются переменные преобразований. Место вставки узла определяется переменными nSegment и tParam.
             В переменной nSegment устанавливается номер сегмента, в переменной tParam (0.1 < tParam < 0.9) задается значение внутреннего параметра на сегменте сплайна.
             \en Insert node in NURBS. Transformation variables are set in data. The insertion position of the node is determined by the variables nSegment and tParam.
             The nSegment variable sets the segment number, the tParam variable (0.1 <tParam <0.9) sets the value of the internal parameter on the spline segment.\~
    \param[in]  curve  - \ru Исходная кривая.
                         \en An initial curve. \~
    \param[in]  data   - \ru Данные построения кривой.
                         \en The curve construction data. \~
    \param[out] result - \ru Сплайновая кривая.
                         \en The spline curve. \~
    \return \ru Возвращает значение результата операции. 
            \en Returns operation result value.
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC( MbResultType ) AddKnotNurbs( MbNurbs3D *       curve,
                                        MbFairCurveData & data,
                                        MbCurve3D *&      result );


//-----------------------------------------------------------------------------
/** \brief \ru Выполнить вставку узла в односегментную NURBS кривую произвольного формата.
           \en Insert a knot into the onesegmented NURBS curve \~
    \details \ru Выполнить вставку узла в односегментную NURBS кривую произвольного формата.\n
            Выходным параметром метода является NURBS кривая первого или второго сегмента. \n
            \en Insert a knot into the onesegmented NURBS curve.\n
            The output parameter of the method is a NURBS curve of first segment. \~

    \param[in]  curve -  \ru NURBS кривая в произвольном формате. 
                         \en NURBS curve in any format. \~
    \param[in]  data - \ru Данные построения кривой. 
                       \en The curve construction data. \~
    \param[out] resCurve - \ru NURBS Кривая. 
                           \en NURBS curve. \~
    \return \ru Возвращает значение результата операции. 
            \en Returns operation result value.
    \ingroup Curve3D_Modeling
*/
MATH_FUNC( MbResultType ) ExtractExtrapolFromSegment( MbNurbs3D *       curve,
                                                      MbFairCurveData & data,
                                                      MbCurve3D *&      resCurve );


//------------------------------------------------------------------------------
  /** \brief \ru Преобразовать конический NURBzS в конический NURBS.
             \en Convert a conical NURBzS into a conical NURBS. \~
    \details \ru Преобразовать NURBzS кривую, геометрически точно представляющую коническую кривую, 
              в коническую рациональную B-сплайновую кривую 6-ой степени, геометрически точно представляющую коническую кривую. \n
             \en Convert a NURBzS curve geometrically representing a conic curve 
             into a rational B-spline curve of degree 6 geometrically representing a conic curve \~
    \param[in]  curve  - \ru Исходная кривая.
                         \en An initial curve. \~
    \param[in]  data   - \ru Данные построения кривой.
                         \en The curve construction data. \~
    \param[out] result - \ru Сплайновая кривая.
                         \en The spline curve. \~
    \return \ru Возвращает значение результата операции. 
            \en Returns operation result value.
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC(MbResultType) ConicNurbs( MbNurbs3D *       curve,
                                    MbFairCurveData & data,
                                    MbCurve3D *&      result );


//------------------------------------------------------------------------------
  /** \brief \ru Построение сплайна заданной длины, соединяющего заданные точки.
             \en Connecting the specified points by a curve of a specified length. \~
    \details \ru Построенная кривая должна выходить из заданной точки в заданном направлении и заканчиваться в заданной точке в заданном направлении.
              Кривизна кривой не должна превосходить заданного максимального значения. \n
             \en The resulting curve must start at a given point in a given direction and end at a given point in a given direction.
             The curvature of the curve must not exceed the specified maximum value. \~ \n
    \param[in]  begPoint       - \ru Начальная точка.
                                 \en The start point. \~
    \param[in]  begDirection   - \ru Направление кривой в начальной точке.
                                 \en The curve direction at the start point. \~
    \param[in]  endPoint       - \ru Конечная точка.
                                 \en The end point. \~
    \param[in]  endDirection   - \ru Направление кривой в конечной точке.
                                 \en The curve direction at the end point. \~
    \param[in]  length         - \ru Примерная длина кривой.
                                 \en The curve length evaluation. \~
    \param[in]  accuracy       - \ru Относительная точность рассчёта длины кривой.
                                 \en Relative accuracy calculate the length of a curve. \~
    \param[in,out] maxCurvatre - \ru Максимально допустимая кривизна кривой.
                                 \en The maximum curvature of the resulting curve. \~
    \param[out] result         - \ru Построенная кривая.
                                 \en The resulting curve. \~
    \return \ru Возвращает значение результата операции. 
            \en Returns operation result value.
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC( MbResultType ) Cabling( const MbCartPoint3D & begPoint, 
                                   const MbVector3D &    begDirection,
                                   const MbCartPoint3D & endPoint, 
                                   const MbVector3D &    endDirection,
                                         double          length, 
                                         double          accuracy,
                                         double &        maxCurvature,
                                         MbCurve3D *&    result );


//------------------------------------------------------------------------------
  /** \brief \ru Установить параметризацию кривой пропорционально длине кривой.
             \en Set curve parametrization proportional to the curve length. \~
    \details \ru Если параметризация кривой не пропорциональна длине кривой, то изменить параметризацию кривой, сделав её пропорциональной длине кривой. \n
              В общем случае кривая будет заменена на репараметризованную кривую MbReparamCurve3D с пропорциональной длине параметризацией.
              В частных случаях, когда исходная кривая имеет пропорциональную параметризацию с требуемыми границами, будет возвращена исходная кривая. \n
             \en If the curve parameterization is not proportional to the length of the curve, then change the curve parameterization to make it proportional to the length of the curve. \n
             In general, the curve will be replaced by a MbReparamCurve3D curve with length proportional parameterization. 
             In special cases, when the curve has a proportional parametrization with the required boundaries, the original curve will be returned. \~ \n
    \param[in]  curve - \ru Исходная кривая, подлежащая репараметризации.
                        \en The original curve to be reparametrized. \~
    \param[in]  tMin  - \ru Новый минимальный параметр кривой.
                        \en New minimal parameter of the curve. \~
    \param[in]  tMax  - \ru Новый максимальный параметр кривой.
                        \en New maximal parameter of the curve. \~
    \return \ru Возвращает построенную кривую с пропорциональной длине параметризацией.
            \en Returns created curve with proportional length  parameterization.
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC ( MbCurve3D * ) SetProportionalParametrization( MbCurve3D * curve,
                                                          double tMin, double tMax);


//------------------------------------------------------------------------------
  /** \brief \ru Продлить заданную кривую.
             \en Extend the given curve. \~
    \details \ru Продлить заданную кривую в соответствии с заданными параметрами. \n
             \en Extend the given curve according to the given parameters. \~ \n
    \param[in] sourceCurve  - \ru Исходная (продлеваемая) кривая.
                              \en Source (extended) curve. \~
    \param[in] parameters   - \ru Параметры продления.
                              \en Parameters of extension. \~
    \param[out] resFrame    - \ru Результирующая кривая внутри каркаса.
                              \en Result curve in the wire frame. \~
    \return \ru Возвращает код результата операции. В случае успеха всегда вернется ненулевая кривая (возможно, она будет совпадать с исходной кривой).
            \en Returns the code of the operation result. In case of success, the result curve cannot be nullptr (it will be possibly the same as the source one).
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC( MbResultType ) ExtendCurve( const MbCurve3D &                    sourceCurve,
                                       const MbCurveExtensionParameters3D & parameters,
                                             SPtr<MbWireFrame> &            resFrame );


//------------------------------------------------------------------------------
  /** \brief  \ru Сделать фаски на стыках кривых контура.
              \en Create chamfers on the contour joints \~
  \details \ru Сделать фаски на стыках кривых контура. \n
           \en Create chamfers on the contour joints \~ \n
  \param[in] paramList - \ru Набор входных параметров: исходная кривая и список стыков с параметрами.
            Исходная кривая может быть следующих типов: 3д-контур, 3д-полилиния, контур на поверхности, полилиния на поверхности.
            При прочих типах успешное создание фаски не гарантируется.\n
            \en Set of input parameters: the source curve and the list of joints with parameters.
            The source curve can be one of the following types: 3d contour, 3d polyline, contour on a surface, polyline on a surface.
            For other types the successful chamfer creation is not guaranteed.\~ \n 
  \return \ru Возвращает код результата операции. В случае успеха всегда вернется ненулевая кривая.
            В случае неудачи результат может быть nullptr. 
            Возможные коды ошибок: rt_ParameterError, rt_PartlyChamfer. rt_ParameterError означает ошибку во входных параметрах.
            Некоторые возможные причины: индекс для несуществующего узла, повторный индекс узла, неправильная входная кривая;
            результирующая кривая при этом не создаётся.
            rt_PartlyChamfer означает ошибку в создании фаски хотя бы на одном из сегментов; при этом результирующая кривая может не существовать.
            Некоторые возможные причины: попадание фаски на край сегмента, коллизия с другой фаской; при этом данная фаска не создаётся.
            Если ни одна фаска не была сделана, результирующая кривая не создаётся.
          \en Returns the code of the operation result. In case of success, the result curve cannot be nullptr.
            In case of failure the result may be nullptr.
            The possible error codes are: rt_ParameterError, rt_PartlyChamfer. rt_ParameterError means the error in the input parameters.
            Some of the possible reasons: index of the non-existing joint, repetitive jount index, wrong source curve; in this case the result curve is not created.
            rt_PartlyChamfer means the error during the chamfer creation on at least one of the segments; the result curve may not exist in this case.
            Some of the possible reasons: chamfer hits an edge of a segment, a collision with another chamfer; in this case the chamfer is not created.
            If no chamfers were created, the resulting curve is not created.\~ \n
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) CreateContourChamfer( const MbContourChamferParams & paramList,
                                                     MbCurve3D *&             resultChamferContour );


//-------------------------------------------------------------------------------
/** \brief \ru Построить развертку кривых и точек на плоскость.
           \en Construct unwrapped curves and points. \~
  \details \ru Построение развертки кривых и точек на плоскость.\n
           \en Construct unwrapped curves and points on a plane.\n \~
  \param[in] params  - \ru Параметры разворачивания.
                       \en Unwrapping parameters. \~
  \param[out] result - \ru Результат разворачивания.
                       \en Wrapping result. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
\ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) UnwrapCurve( const MbCurvesWrappingParams &  parameters,
                                            MbCurvesWrappingResults & result );


//-------------------------------------------------------------------------------
/** \brief \ru Выполнить операцию сворачивания кривых и точек на поверхность.
           \en Perform the operation of wrapping curves and points on a surface. \~
  \details \ru Выполнить операцию сворачивания кривых на поверхность.\n
           \en Perform the operation of wrapping curves on a surface.\n \~
  \param[in] params  - \ru Параметры сворачивания.
                       \en Wrapping parameters. \~
  \param[out] result - \ru Результат сворачивания.
                       \en Wrapping result. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
\ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC( MbResultType ) WrapCurve( const MbCurvesWrappingParams &  parameters,
                                           MbCurvesWrappingResults & result );


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
MATH_FUNC( MbResultType ) FitCurve( const MbCurveFitParameters3D & params, MbCurveFitResults3D & results );

#endif // __ACTION_CURVE3D_H
