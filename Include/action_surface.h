////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Методы построения поверхностей.
         \en Functions for surfaces creation. \~
  \details \ru Поверхности являются основным элементом описания формы моделируемых объектов.
    На базе поверхностей строятся грани, которые используются в твёрдых телах.
           \en Surfaces is a basic element of the modeled objects shape description.
    Faces are constructed on the basis of surfaces and then are used in solid solids. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ACTION_SURFACE_H
#define __ACTION_SURFACE_H


#include <templ_s_array.h>
#include <mb_cart_point3d.h>
#include <surface.h>
#include <topology.h>
#include <mb_operation_result.h>
#include <vector>


class   MbCurve;
class   MbCurve3D;
class   MbSolid;
class   MbSurfaceCurve;
class   MbFunction;
class   MbGrid;
class   MbRegion;


//------------------------------------------------------------------------------
/** \brief \ru Создать элементарную поверхность.
           \en Create an elementary surface. \~
  \details \ru Создать одну из элементарных поверхностей по трем управляющим точкам и типу: \n
    surfaceType == st_Plane           - плоскость \n
    surfaceType == st_ConeSurface     - коническая поверхность \n
    surfaceType == st_CylinderSurface - цилиндрическая поверхность \n
    surfaceType == st_SphereSurface   - сферическая поверхность \n
    surfaceType == st_TorusSurface    - поверхность тора \n
           \en Create one of elementary surfaces from three points and a type: \n
    surfaceType == st_Plane           - a plane \n
    surfaceType == st_ConeSurface     - a conical surface \n
    surfaceType == st_CylinderSurface - a cylindrical surface \n
    surfaceType == st_SphereSurface   - a spherical surface \n
    surfaceType == st_TorusSurface    - a torus surface \n \~
  \param[in] point0 - \ru Точка, определяющая начало локальной системы координат поверхности.
                      \en The origin of the surface local coordinate system. \~
  \param[in] point1 - \ru Точка, определяющая направление оси X локальной системы и радиус поверхности.
                      \en A point specifying the direction of X-axis of the local system and the surface radius. \~
  \param[in] point2 - \ru Точка, определяющая направление оси Y локальной системы.
                      \en A point specifying the direction of Y-axis of the local system. \~
  \param[in] surfaceType - \ru Тип поверхности.
                           \en The surface type. \~
  \param[out] result - \ru Построенная поверхность.
                       \en The constructed surface. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) ElementarySurface( const MbCartPoint3D & point0, 
                                            const MbCartPoint3D & point1, 
                                            const MbCartPoint3D & point2, 
                                                  MbeSpaceType    surfaceType,
                                                  MbSurface *&    result ); 


//------------------------------------------------------------------------------
/** \brief \ru Создать плоскую NURBS - поверхность.
           \en Create a planar NURBS - surface. \~
  \details \ru Создать плоскую NURBS - поверхность по угловым точкам. \n
           \en Create a planar NURBS - surface given the corner points. \n \~
  \param[in] pUMinVMin - \ru Угловая точка поверхности.
                         \en A corner point of a surface. \~
  \param[in] pUMaxVMin - \ru Угловая точка поверхности.
                         \en A corner point of a surface. \~
  \param[in] pUMaxVMax - \ru Угловая точка поверхности.
                         \en A corner point of a surface. \~
  \param[in] pUMinVMax - \ru Угловая точка поверхности.
                         \en A corner point of a surface. \~
  \param[in] uCount - \ru Количество точек по U.
                      \en A number of points by U direction. \~
  \param[in] vCount - \ru Количество точек по V.
                      \en A number of points by V direction. \~
  \param[in] uDegree - \ru Порядок сплайнов по U.
                       \en Splines degree by U. \~
  \param[in] vDegree - \ru Порядок сплайнов по V.
                       \en Splines degree by V. \~
  \param[out] result - \ru Сплайновая поверхность.
                       \en The spline surface. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) SplineSurface( const MbCartPoint3D & pUMinVMin, 
                                        const MbCartPoint3D & pUMaxVMin,
                                        const MbCartPoint3D & pUMaxVMax, 
                                        const MbCartPoint3D & pUMinVMax,
                                              size_t          uCount, 
                                              size_t          vCount,
                                              size_t          uDegree, 
                                              size_t          vDegree, 
                                              MbSurface *&    result );


//------------------------------------------------------------------------------
/** \brief \ru Создать NURBS - поверхность.
           \en Create a NURBS - surface. \~ 
  \details \ru Создать NURBS - поверхность по массивам точек и весов. \n
    контейнер weightList может быть пустым. \n
    контейнер uKnotList может быть пустым. \n
    контейнер vKnotList может быть пустым. \n
           \en Create a NURBS - surface given arrays of points and weights. \n
    container 'weightList' can be empty. \n
    container 'uKnotList' can be empty. \n
    container 'vKnotList' can be empty. \n \~
  \param[in] pointList - \ru Множество точек.
                         \en An array of points. \~
  \param[in] weightList - \ru Множество весов
                          \en An array of weights. \~
  \param[in] uCount - \ru Размерность массива точек по U.
                      \en The size of point array by U. \~
  \param[in] vCount - \ru Размерность массива точек по V.
                      \en The size of point array by V. \~
  \param[in] uDegree - \ru Порядок сплайнов по U.
                       \en Splines degree by U. \~
  \param[in] uKnotList - \ru Узловой вектор по U.
                         \en A knot vector by U. \~
  \param[in] uClosed - \ru Замкнутость по U.
                       \en Closedness by U. \~
  \param[in] vDegree - \ru Порядок сплайнов по V.
                       \en Splines degree by V. \~
  \param[in] vKnotList - \ru Узловой вектор по V.
                         \en A knot vector by V. \~
  \param[in] vClosed - \ru Замкнутость по V.
                       \en Closedness by V. \~
  \param[out] result - \ru Сплайновая поверхность.
                       \en The spline surface. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) SplineSurface( const SArray<MbCartPoint3D> & pointList,
                                        const SArray<double> &        weightList,
                                              size_t                  uCount, 
                                              size_t                  vCount,
                                              size_t                  uDegree, 
                                        const SArray<double> &        uKnotList,
                                              bool                    uClosed, 
                                              size_t                  vDegree, 
                                        const SArray<double> &        vKnotList,
                                              bool                    vClosed, 
                                              MbSurface *&            result );


//------------------------------------------------------------------------------
/** \brief \ru Создать поверхность выдавливания.
           \en Create an extrusion surface. \~
  \details \ru Создать поверхность выдавливания кривой. \n
           \en Create a surface of a curve extrusion. \n \~
  \param[in] curve - \ru Образующая кривая.
                     \en The generating curve. \~
  \param[in] direction - \ru Вектор выдавливания.
                         \en An extrusion vector. \~
  \param[in] simplify - \ru Упрощать поверхность, если возможно.
                        \en Simplify a surface if it's possible. \~
  \param[out] result - \ru Поверхность выдавливания.
                       \en An extrusion surface. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) ExtrusionSurface( const MbCurve3D &  curve, 
                                           const MbVector3D & direction, 
                                                 bool         simplify, 
                                                 MbSurface *& result );


//------------------------------------------------------------------------------
/** \brief \ru Создать поверхность вращения.
           \en Create a revolution surface. \~
  \details \ru Создать поверхность вращения кривой. \n
           \en Create a curve revolution surface. \n \~
  \param[in] curve - \ru Образующая кривая.
                     \en The generating curve. \~
  \param[in] origin - \ru Точка положения оси вращения.
                      \en The rotation axis origin. \~
  \param[in] axis - \ru Направление оси вращения.
                    \en The rotation axis direction. \~
  \param[in] angle - \ru Угол вращения.
                     \en A rotation angle. \~
  \param[in] simplify - \ru Упрощать поверхность, если возможно.
                        \en Simplify a surface if it's possible. \~
  \param[out] result - \ru Поверхность вращения.
                       \en The revolution surface. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) RevolutionSurface( const MbCurve3D &     curve, 
                                            const MbCartPoint3D & origin, 
                                            const MbVector3D &    axis, 
                                                  double          angle,
                                                  bool            simplify, 
                                                  MbSurface *&    result );


//------------------------------------------------------------------------------
/** \brief \ru Создать поверхность движения.
           \en Create an expansion surface. \~
  \details \ru Создать поверхность движения кривой. \n
           \en Create a surface of a curve sweeping. \n \~
  \param[in] curve - \ru Образующая кривая.
                     \en The generating curve. \~
  \param[in] spine - \ru Направляющая кривая.
                     \en The spine curve. \~
  \param[out] result - \ru Поверхность движения с доворотами.
                       \en The expansion surface. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) ExpansionSurface( const MbCurve3D &  curve, 
                                           const MbCurve3D &  spine, 
                                           const MbCurve3D *  curve1, 
                                                 MbSurface *& result );


//------------------------------------------------------------------------------
/** \brief \ru Создать кинематическую поверхность.
           \en Create an evolution surface. \~
  \details \ru Создать кинематическую поверхность по образующей и направляющей. \n
    В случае, если spine имеет тип st_ConeSpiral, результатом построения 
    является спиральная поверхность. \n
           \en Create an evolution surface from the generating curve and the guide curve. \n
    If 'spine' has type st_ConeSpiral, the result of the construction 
    is a spiral surface. \n \~
  \param[in] curve - \ru Образующая кривая.
                     \en The generating curve. \~
  \param[in] spine - \ru Направляющая кривая.
                     \en The spine curve. \~
  \param[out] result - \ru Кинематическая поверхность или спиральная поверхность.
                       \en The evolution surface or a spiral surface. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) EvolutionSurface( const MbCurve3D &  curve, 
                                           const MbCurve3D &  spine, 
                                                 MbSurface *& result );


//------------------------------------------------------------------------------
/** \brief \ru Создать спиральную поверхность.
           \en Create a spiral surface. \~
  \details \ru Создать спиральную поверхность по образующей и 3 точкам. \n
           \en Create a spiral surface from a generating line and three points. \n \~
  \param[in] curve - \ru Образующая кривая спирали.
                     \en The generating curve of a spiral. \~
  \param[in] sameCurve - \ru Использовать копию образующей кривой, если sameCurve равен false.
                         \en Use a copy of the generating curve if the flag "sameCurve" is false. \~
  \param[in] p0 - \ru Начало локальной системы координат (ЛСК).
                  \en The origin of local coordinate system (LCS). \~
  \param[in] p1 - \ru Точка для формирования оси Z ЛСК.
                  \en A point specifying Z-axis of LCS. \~
  \param[in] p2 - \ru Точка для формирования оси X ЛСК.
                  \en A point specifying X-axis of LCS. \~
  \param[in] step - \ru Шаг спирали.
                    \en A pitch. \~
  \param[out] result - \ru Спиральная поверхность.
                       \en A spiral surface. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) SpiralSurface( const MbCurve3D &     curve, 
                                              bool            sameCurve,
                                        const MbCartPoint3D & p0, 
                                        const MbCartPoint3D & p1, 
                                        const MbCartPoint3D & p2, 
                                              double          step, 
                                              MbSurface *&    result );


//------------------------------------------------------------------------------
/** \brief \ru Создать секториальную поверхность.
           \en Create a sectorial surface. \~ 
  \details \ru Создать секториальную поверхность по кривой и точке. \n
           \en Create a sectorial surface from a curve and a point. \n \~
  \param[in] curve - \ru Образующая кривая.
                     \en The generating curve. \~
  \param[in] point - \ru Точка.
                     \en A point. \~
  \param[out] result - \ru Линейчатая поверхность в виде сектора.
                       \en The ruled surface in a form of a sector. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) SectorSurface( const MbCurve3D &     curve, 
                                        const MbCartPoint3D & point, 
                                              MbSurface *&    result );


//------------------------------------------------------------------------------
/** \brief \ru Создать линейчатую поверхность.
           \en Create a ruled surface. \~
  \details \ru Создать линейчатую поверхность по двум кривым. \n
           \en Create a ruled surface from two curves. \n \~
  \param[in] curve1 - \ru Первая образующая кривая.
                      \en The first generating curve. \~
  \param[in] curve2 - \ru Вторая образующая кривая.
                      \en The second generating curve. \~
  \param[in] simplify - \ru Упрощать поверхность, если возможно.
                        \en Simplify a surface if it's possible. \~
  \param[out] result - \ru Линейчатая поверхность.
                       \en The ruled surface. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) RuledSurface( MbCurve3D &  curve1, 
                                       MbCurve3D &  curve2, 
                                       bool         simplify, 
                                       MbSurface *& result );


//------------------------------------------------------------------------------
/** \brief \ru Создать треугольную поверхность.
           \en Create a triangular surface. \~
  \details \ru Создать треугольную поверхность по трем кривым. \n
           \en Create a triangular surface from three curves. \n \~
  \param[in] curve1 - \ru Первая кривая.
                      \en The first curve. \~
  \param[in] curve2 - \ru Вторая кривая.
                      \en The second curve. \~
  \param[in] curve3 - \ru Третья кривая.
                      \en The third curve. \~
  \param[out] result - \ru Треугольная поверхность по трём кривым.
                       \en The triangular surface by three curves. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) CornerSurface( MbCurve3D &  curve1, 
                                        MbCurve3D &  curve2, 
                                        MbCurve3D &  curve3,
                                        MbSurface *& result );


//------------------------------------------------------------------------------
/** \brief \ru Создать билинейную поверхность.
           \en Create a bilinear surface. \~
  \details \ru Создать билинейную поверхность по четырем кривым. \n
           \en Create a bilinear surface from four curves. \n \~
  \param[in] curve1 - \ru Первая кривая.
                      \en The first curve. \~
  \param[in] curve2 - \ru Вторая кривая.
                      \en The second curve. \~
  \param[in] curve3 - \ru Третья кривая.
                      \en The third curve. \~
  \param[in] curve4 - \ru Четвертая кривая.
                      \en The fourth curve. \~
  \param[out] result - \ru Билинейная поверхность по четырём кривым.
                       \en The bilinear surface from four curves. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) CoverSurface( MbCurve3D &  curve1, 
                                       MbCurve3D &  curve2,
                                       MbCurve3D &  curve3, 
                                       MbCurve3D &  curve4,
                                       MbSurface *& result );


//------------------------------------------------------------------------------
/** \brief \ru Создать поверхность по семейству кривых.
           \en Create a surface by a set of curves. \~
  \details \ru Создать поверхность по семейству кривых. \n
    begDirection направление в начале поверхности может быть нулевой длины. \n
    endDirection направление в конце  поверхности может быть нулевой длины. \n
           \en Create a surface by a set of curves. \n
    begDirection direction at the begining of the surface can be of zero length. \n
    endDirection direction at the end of the surface can be of zero length. \n \~
  \param[in] curveList - \ru Семейство образующих кривых вдоль U-направления.
                         \en A set of generating curves along U direction. \~
  \param[in] closed - \ru Замкнутость вдоль V-направления.
                      \en Closedness by V direction. \~
  \param[in] begDirection - \ru Вектор направления в начале поверхности.
                            \en The vector of direction at the beginning of the surface. \~
  \param[in] endDirection - \ru Вектор направления в конце поверхности.
                            \en The vector of direction at the end of the surface. \~
  \param[out] result - \ru Поверхность по семейству кривых.
                       \en The surface from the set of curves. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) LoftedSurface( const RPArray<MbCurve3D> & curveList, 
                                              bool                 closed, 
                                        const MbVector3D &         begDirection, 
                                        const MbVector3D &         endDirection, 
                                              MbSurface *&         result );


//------------------------------------------------------------------------------
/** \brief \ru Создать поверхность по семейству кривых и направляющей.
           \en Create a surface from a set of curves and a spine curve. \~
  \details \ru Создать поверхность по семейству кривых и направляющей. \n
           \en Create a surface from a set of curves and a spine curve. \n \~
  \param[in] curveList - \ru Семейство образующих кривых вдоль U-направления.
                         \en A set of generating curves along U direction. \~
  \param[in] spine - \ru Направляющая кривая.
                     \en The spine curve. \~
  \param[out] result - \ru Поверхность по семейству кривых и направляющей.
                       \en The surface from a set of curves and a spine curve. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) LoftedSurface( const RPArray<MbCurve3D> & curveList, 
                                              MbCurve3D &          spine,
                                              MbSurface *&         result,
                                              bool                 isSimToEvol = true );


//------------------------------------------------------------------------------
/** \brief \ru Создать поверхность на сетке кривых.
           \en Create a surface constructed by the grid curves. \~
  \details \ru Создать поверхность на сетке кривых по двум семействам кривых. \n
           \en Create a surface constructed by the grid curves given two sets of curves. \n \~
  \param[in] uCurveList - \ru Семейство кривых вдоль U-направления.
                          \en A curve set along U direction. \~
  \param[in] vCurveList - \ru Семейство кривых вдоль V-направления.
                          \en A curve set along V direction. \~
  \param[out] result - \ru Поверхность на сетке кривых.
                       \en The surface constructed by the grid curves. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) MeshSurface( const RPArray<MbCurve3D> & uCurveList, 
                                      const RPArray<MbCurve3D> & vCurveList,
                                            MbSurface *&         result );


//------------------------------------------------------------------------------
/** \brief \ru Создать эквидистантную поверхность.
           \en Create an offset surface. \~
  \details \ru Создать эквидистантную поверхность к исходной поверхности. \n
           \en Create an offset surface to a given surface. \n \~
  \param[in] surface - \ru Исходная поверхность.
                       \en The initial surface. \~
  \param[in] distance - \ru Величина эквидистанты (знаковая).
                        \en The offset distance (signed). \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \param[out] result - \ru Эквидистантная поверхность.
                       \en The offset surface. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) OffsetSurface( const MbSurface &  surface, 
                                              double       distance, 
                                              MbSurface *& result );


//------------------------------------------------------------------------------
/** \brief \ru Создать эквидистантную поверхность.
           \en Create an offset surface. \~
  \details \ru Создать эквидистантную поверхность по исходной поверхности. \n
           \en Create an offset surface from the initial surface. \n \~
  \param[in] surface -        \ru Базовая поверхность.
                              \en The base surface. \~
  \param[in] offsetUminVmin - \ru Смещение в точке Umin Vmin базовой поверхности.
                              \en Offset distance on point Umin Vmin of base surface. \~
  \param[in] offsetUmaxVmin - \ru Смещение в точке Umax Vmin базовой поверхности.
                              \en Offset distance on point Umax Vmin of base surface. \~
  \param[in] offsetUminVmax - \ru Смещение в точке Umin Vmax базовой поверхности.
                              \en Offset distance on point Umin Vmax of base surface. \~
  \param[in] offsetUmaxVmax - \ru Смещение в точке Umax Vmax базовой поверхности.
                              \en Offset distance on point Umax Vmax of base surface. \~
  \param[in] type -           \ru Тип смещения точек: константный, линейный или кубический.
                              \en The offset type: constant, or linear, or cubic. \~
  \param[out] result -        \ru Эквидистантная поверхность.
                              \en The offset surface. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) OffsetSurface( const MbSurface &   surface, 
                                              double        offsetUminVmin,
                                              double        offsetUmaxVmin,
                                              double        offsetUminVmax,
                                              double        offsetUmaxVmax,
                                              MbeOffsetType type,
                                              MbSurface *&  result );


//------------------------------------------------------------------------------
/** \brief \ru Создать продленную поверхность.
           \en Create an extended surface. \~
  \details \ru Создать продленную поверхность по исходной поверхности. \n
           \en Create an extended surface from the initial surface. \n \~
  \param[in] surface - \ru Исходная поверхность.
                       \en The initial surface. \~
  \param[in] uMin - \ru Минимальное значение по U.
                    \en The minimal parameter value by U. \~
  \param[in] uMax - \ru Максимальное значение по U.
                    \en The maximal parameter value by U. \~
  \param[in] vMin - \ru Минимальное значение по V.
                    \en The minimal parameter value by V. \~
  \param[in] vMax - \ru Максимальное значение по V.
                    \en The maximal parameter value by V. \~
  \param[out] result - \ru Продлённая поверхность.
                       \en The extended surface. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) ExtendedSurface( MbSurface &  surface, 
                                          double       uMin, 
                                          double       uMax, 
                                          double       vMin,
                                          double       vMax,
                                          MbSurface *& result );


//------------------------------------------------------------------------------
/** \brief \ru Создать деформированную поверхность.
           \en Create a deformed surface. \~
  \details \ru Создать деформированную поверхность по исходной поверхности. \n
           \en Create a deformed surface from the initial surface. \n \~
  \param[in] surface - \ru Исходная поверхность.
                       \en The initial surface. \~
  \param[in] uCount - \ru Количество точек по U.
                      \en A number of points by U direction. \~
  \param[in] vCount - \ru Количество точек по V.
                      \en A number of points by V direction. \~
  \param[in] uDegree - \ru Порядок сплайнов по U.
                       \en Splines degree by U. \~
  \param[in] vDegree - \ru Порядок сплайнов по V.
                       \en Splines degree by V. \~
  \param[in] dist - \ru Величина сдвига вдоль нормали.
                    \en Shift along the normal. \~
  \param[out] result - \ru Деформированная поверхность.
                       \en The deformed surface. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) DeformedSurface( const MbSurface &  surface, 
                                                size_t       uCount,
                                                size_t       vCount,
                                                size_t       uDegree,
                                                size_t       vDegree, 
                                                double       dist, 
                                                MbSurface *& result );


//------------------------------------------------------------------------------
/** \brief \ru Создать поверхность с заданной границей.
           \en Create a surface with the given boundary. \~
  \details \ru Создать поверхность с заданной границей по массиву двумерных кривых. \n 
    Контейнер boundList может быть пустым. \n
           \en Create a surface with the given boundary from an array of two-dimensional curves. \n 
    Container 'boundList' can be empty. \n \~
  \param[in] surface - \ru Исходная поверхность.
                       \en The initial surface. \~
  \param[in] boundList - \ru Множество двумерных границ в виде кривых (первая кривая - внешний контур).
                         \en An array of two-dimensional boundaries in the form of curves (the first curve is an outer contour). \~
  \param[out] result - \ru Поверхность, ограниченная кривыми.
                       \en The surface bounded by the curves. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) BoundedSurface(       MbSurface &        surface, 
                                         const RPArray<MbCurve> & boundList, 
                                               MbSurface *&       result );

//------------------------------------------------------------------------------
/** \brief \ru Создать поверхность с заданной границей.
           \en Create a surface with the given boundary. \~
  \details \ru Создать поверхность с заданной границей по массиву двумерных контуров. \n 
           \en Create a surface with the given boundary from an array of two-dimensional curves. \n \~
  \param[in] place - \ru Локальная система координат плоскости.
                     \en The local coordinate system of a plane. \~
  \param[in] region - \ru Множество двумерных границ в виде региона (первая контур - внешний).
                         \en An array of two-dimensional boundaries in the form of region (the first contour is outer). \~
  \param[out] result - \ru Поверхность, ограниченная кривыми.
                       \en The surface bounded by the curves. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) BoundedSurface( const MbPlacement3D & place, 
                                         const MbRegion &      region, 
                                               MbSurface *&    result );


//------------------------------------------------------------------------------
/** \brief \ru Создать NURBS копию поверхности, ограниченную двумерными границами.
           \en Create a NURBS surface copy with two-dimensional boundaries. \~
  \details \ru Создать NURBS копию поверхности, ограниченную двумерными границами проецированием пространственных границ \n
    (предполагается, что пространственные граничные кривые лежат на поверхности). \n
           \en Create a NURBS surface copy with two-dimensional boundaries by projecting of the spatial boundaries \n
    (the boundary space curves are considered to belong to the surface) \n \~
  \param[in] surf - \ru Исходная поверхность.
                    \en The initial surface. \~
  \param[in] version - \ru Версия исполнения.
                       \en The version. \~
  \param[out] result - \ru Сплайновая поверхность (ограниченная кривыми).
                       \en The spline surface (bounded by the curves). \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) NurbsSurface( const MbSurface &  surf, 
                                             VERSION      version, 
                                             MbSurface *& result );


//------------------------------------------------------------------------------
/** \brief \ru Создать NURBS копию поверхности, по заданным параметрам.
           \en Create a NURBS surface copy by given parameters. \~
  \details \ru Создать NURBS копию поверхности, по заданным параметрам. \n
           \en Create a NURBS surface copy by given parameters. \n \~
  \param[in] surf - \ru Исходная поверхность.
                    \en The initial surface. \~
  \param[in] uParam - \ru Параметры построения NURBS копии объекта.
                      \en Parameters for the construction of a NURBS copy of the object. \~
  \param[in] vParam - \ru Параметры построения NURBS копии объекта.
                      \en Parameters for the construction of a NURBS copy of the object. \~
  \param[out] result - \ru Сплайновая поверхность.
                       \en The spline surface. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) NurbsSurface( const MbSurface &         surf, 
                                       const MbNurbsParameters & uParam, 
                                       const MbNurbsParameters & vParam, 
                                             c3d::SurfaceSPtr &  result );


//------------------------------------------------------------------------------
/** \brief \ru Создать поверхность симплексного сплайна.
           \en Create a simplex spline surface. \~
  \details \ru Создать поверхность симплексного сплайна по массиву вершин. \n
           \en Create a simplex spline surface from a point array. \n \~
  \param[in] pList - \ru Множество вершин.
                     \en An array of points. \~
  \param[out] resSurface - \ru Поверхность симплексного сплайна.
                           \en The simplex spline surface. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) SimplexSplineSurface( const SArray<MbCartPoint3D> & pList, MbSurface *& resSurface );


//------------------------------------------------------------------------------
/** \brief \ru Создать треугольную поверхность Безье.
           \en Create a triangular Bezier surface. \~
  \details \ru Создать треугольную поверхность Безье по 3 точкам. \n
           \en Create a triangular Bezier surface from three points. \n \~
  \param[in] k - \ru Порядок поверхности.
                 \en The surface order. \~
  \param[in] p1 - \ru Первая точка.
                  \en The first point. \~
  \param[in] p2 - \ru Вторая точка.
                  \en The second point. \~
  \param[in] p3 - \ru Третья точка.
                  \en The third point. \~
  \param[out] resSurface - \ru Треугольная поверхность Безье.
                           \en The triangular Bezier surface. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) TriBezierSurface(       ptrdiff_t       k, 
                                           const MbCartPoint3D & p1,
                                           const MbCartPoint3D & p2,
                                           const MbCartPoint3D & p3, 
                                                 MbSurface *&    resSurface );


//------------------------------------------------------------------------------
/** \brief \ru Создать треугольную В-сплайн поверхность.
           \en Create a triangular B-spline surface. \~
  \details \ru Создать треугольную В-сплайн поверхность по 3 точкам. \n
           \en Create a triangular B-spline surface from three points. \n \~
  \param[in] surface - \ru Исходная поверхность.
                       \en The initial surface. \~
  \param[in] degree  - \ru Порядок поверхности.
                       \en The surface order. \~
  \param[in] uCount  - \ru Число элементов вдоль первого направления.
                       \en The elements count along first direction. \~
  \param[in] vCount  - \ru Число элементов вдоль второго направления.
                       \en The elements count along second direction. \~
  \param[out] result - \ru Симплексная поверхность.
                       \en The simplex surface. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) SimplexSurface( const MbSurface &  surface,
                                               size_t       degree, 
                                               size_t       uCount, 
                                               size_t       vCount, 
                                               MbSurface *& result );


//------------------------------------------------------------------------------
/** \brief \ru Построить характеристическую ломаную сплайновой поверхности.
           \en Create a characteristic polyline of a spline surface. \~
  \details \ru Построить характеристическую ломаную сплайновой поверхности. \n
    Функция работает с поверхностями типа st_SplineSurface, st_HermitSurface,
    st_TriBezierSurface, st_SimplexSurface. \n
           \en Create a characteristic polyline of a spline surface. \n
    The function accepts the surfaces of types st_SplineSurface, st_HermitSurface,
    st_TriBezierSurface, st_SimplexSurface. \n \~
  \param[in] surf - \ru Поверхность.
                    \en The surface. \~
  \param[out] segments - \ru Сегменты характеристической ломаной.
                         \en The characteristic polyline. \~
  \result \ru Возвращает true - если характеристическая ломаная получена.
          \en Returns true - if the characteristic polyline is obtained. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) GetLineSegmentNURBSSurface( const MbSurface & surf, RPArray<MbCurve3D> & segments );


//------------------------------------------------------------------------------
/** \brief \ru Создание поверхности на сетке точек.
           \en Create a surface from a points grid. \~ 
  \details \ru Создание поверхности на сетке точек и триангуляции. \n
    Множество треугольников должен представлять собой правильную триангуляцию.
           \en Create a surface from a points grid and triangulation. \n
    The triangles array should form a regular triangulation. \~
  \param[in] grid    - \ru Триангуляция.
                       \en A triangulation. \~
  \param[out] result - \ru Поверхность на сетке точек.
                       \en The surface on a point set. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) GridSurface( const MbGrid & grid, MbSurface *& result );


//------------------------------------------------------------------------------
/** \brief \ru Создать средние плоскости.
           \en Create median planes. \~
  \details \ru Создать средние плоскости по двум кривым.\n
           \en Create median planes from two curves.\n \~
  \param[in] curve1   - \ru Первая кривая.
                        \en The first curve. \~
  \param[in] curve2   - \ru Вторая кривая.
                        \en The second curve. \~
  \param[out] places  - \ru Набор систем координат, задающих плоскости.
                        \en The set of coordinate systems which determine the planes. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) MiddlePlaces( const MbCurve3D                  & curve1, 
                                       const MbCurve3D                  & curve2, 
                                             std::vector<MbPlacement3D> & places );


//------------------------------------------------------------------------------
/** \brief \ru Построение поверхности переменного сечения.
           \en Construction of a swept section surface. \~
  \details \ru Поверхность переменного (конического) сечения образуется путем движения плоской кривой, являющейся коническим сечением, вдоль опорной кривой.
    В процессе движения форма плоской кривой меняется в соответствии с дискриминантом конического сечения.
    Начало плоской кривой располагается на начальной направляющей кривой, а конец - на конечной направляющей кривой.
    Плоскость переменного сечения сохраняет ортогональность опорной кривой в процессе движения.
           \en The swept (conic) section surface is formed by moving the flat conic section curve along the reference curve.
    In the process of movement, the shape of the flat curve changes in accordance with the discriminant of the conic section.
    The beginning of the flat curve is located on the first guide curve and the end is located on the second guide curve.
    The plane of the swept section preserves the orthogonality to the reference curve during movement. \~ 
  \param[in] rc -       \ru Опорная кривая.
                        \en The reference curve (spine). \~
  \param[in] g1 -       \ru Первая направляющая кривая.
                        \en The first guide curve. \~
  \param[in] g2 -       \ru Вторая направляющая кривая (g1==g2 совпадает с первой при cs_Round).
                        \en The second guide curve (g1==g2 the same first guide for cs_Round). \~
  \param[in] c0 -       \ru Дополнительная направляющая кривая (может быть nullptr).
                        \en The additional guide curve (may be nullptr). \~
  \param[in] form -     \ru Форма сечения поверхности (0, 1, 2, 3).
                        \en The form of the surface section (0, 1, 2, 3). \~
  \param[in] sense -    \ru Направление нормали поверхности направляющей кривой (для guide1==guide2).
                        \en The normal direction of guide curve surface (for guide1==guide2). \~
  \param[in] uBeg -     \ru Минимальное значение первого параметра поверхности.
                        \en The minimum value of the first surface parameter. \~
  \param[in] uEnd -     \ru Максимальное значение первого параметра поверхности.
                        \en The maximum value of the first surface parameter. \~
  \param[in] func -     \ru Функция управления сечением поверхности.
                        \en Section control function. \~
  \param[in] patt -     \ru Образующая кривая при form==cs_Shape. 
                        \en Forming curve for form==cs_Shape. \~
  \param[in] accuracy - \ru Точность построения. 
                        \en The accuracy of building. \~
  \param[in] vers -     \ru Версия поверхности.
                        \en The surface version. \~
  \param[out] result -  \ru Построенная поверхность.
                        \en The constructed surface. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) SectionSurface( const MbCurve3D &  rc,
                                         const MbCurve3D &  g1, 
                                         const MbCurve3D &  g2,
                                         const MbCurve3D *  c0, 
                                               size_t       form,
                                               bool         sense,
                                               double       uBeg, 
                                               double       uEnd, 
                                               MbFunction * func,
                                               MbCurve *    patt,
                                               double       accuracy, 
                                               VERSION      vers,
                                               MbSurface *& result );


//------------------------------------------------------------------------------
/** \brief \ru Построение поверхности Кунса.
           \en Construction of a Coons surface. \~
  \details \ru Построение бикубической поверхности Кунса на четырех кривых и их поперечных производных, касательной к четырём кривым на прверхностях. \n
           \en The construction of Coons surface, which will be tangent to four surfaces and coincide with four curves on this surfaces on it sides.\n \~
  \param[in] surfaceCurve0 - \ru Кривая на поверхности 0.
                             \en The curve on surface0. \~
  \param[in] surfaceCurve1 - \ru Кривая на поверхности 1.
                             \en The curve on surface1. \~
  \param[in] surfaceCurve2 - \ru Кривая на поверхности 2.
                             \en The curve on surface2. \~
  \param[in] surfaceCurve3 - \ru Кривая на поверхности 3.
                             \en The curve on surface3. \~
  \param[out] result - \ru Построенная поверхность.
                       \en The constructed surface. \~
  \result \ru Возвращает код результата построения.
          \en Returns operation result code. \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) CreateCoonsSurface( const MbSurfaceCurve & surfaceCurve0, 
                                             const MbSurfaceCurve & surfaceCurve1, 
                                             const MbSurfaceCurve & surfaceCurve2, 
                                             const MbSurfaceCurve & surfaceCurve3, 
                                                   MbSurface *& result );


//------------------------------------------------------------------------------
/** \brief \ru Построение поверхности-заплатки для заданных рёбер.
           \en Construction of a surface-patch by the edges. \~
  \details \ru Построение поверхности-заплатки, гладко стыкующейся с поверхностями ребер. \n
           \en Construction of a surface-patch, smoothly joining with the surfaces of the edges. \n \~
  \param[in] edges   - \ru Ребра, с которыми требуется стыковать новую поверхность.
                       \en Edges to join the new surface. \~
  \param[out] result - \ru Построенные поверхности.
                       \en The constructed surfaces. \~
  \result \ru Возвращает код результата построения.
          \en Returns operation result code. \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) CreateSplinePatch( const c3d::ConstEdgesVector & edges, 
                                                  c3d::SurfacesVector &   result );


#endif // __ACTION_SURFACE_H
