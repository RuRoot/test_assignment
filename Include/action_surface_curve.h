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

#ifndef __ACTION_SURFACE_CURVE_H
#define __ACTION_SURFACE_CURVE_H


#include <templ_s_array.h>
#include <mb_cart_point3d.h>
#include <mb_vector3d.h>
#include <space_item.h>
#include <wire_frame.h>
#include <mb_point_mating.h>
#include <mb_operation_result.h>
#include <op_curve_parameter.h>


class    MbAxis3D;
class    MbPlacement3D;
class    MbCurve;
class    MbCurve3D;
class    MbContour3D;
class    MbSurfaceCurve;
class    MbSurface;
class    MbElementarySurface;
class    MbFace;
class    MbSolid;
class    MbWireFrame;
class    MbSNameMaker;


//------------------------------------------------------------------------------
/** \brief \ru Рассчитать вершины ломаной.
           \en Compute the vertices of a polyline. \~
  \details \ru Рассчитать вершины ломаной point1 и point2, соединяющей точки origin1 и origin2, 
           сдвинутые в направлениях direction1 и direction2 на расстояния length1 и length2, 
           которую можно скруглить радиусами radius1 и radius2. \n
           \en Compute the vertices 'point1' and 'point2' of a polyline connecting points 'origin1' and 'origin2' 
           translated in the directions 'direction1' and 'direction2' by distances 'length1' and 'length2' 
           which can be rounded with radius 'radius1' and 'radius2'. \n \~
  \param[in] origin1 - \ru Первая точка.
                       \en The first point. \~
  \param[in] direction1 - \ru Направление сдвига первой точки.
                          \en The direction of the first point translation. \~
  \param[in] length1 - \ru Величина сдвига первой точки.
                       \en The distance of the first point translation. \~
  \param[in] radius1 - \ru Радиус скругления для первой точки.
                       \en The rounding radius for the first point. \~
  \param[in] origin2 - \ru Вторая точка.
                       \en The second point. \~
  \param[in] direction2 - \ru Направления сдвига для второй точки.
                          \en The direction of the second point translation. \~
  \param[in] length2 - \ru Величина сдвига для второй точки.
                       \en The distance of the second point translation. \~
  \param[in] radius2 - \ru Радиус скругления для второй точки.
                       \en The rounding radius for the second point. \~
  \param[out] result1 - \ru Первая точка ломаной.
                        \en The first point of the polyline. \~
  \param[out] result2 - \ru Вторая точка ломаной.
                        \en The second point of the polyline. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) CalculatePipePoints( const MbCartPoint3D & origin1, 
                                              const MbVector3D &    direction1,
                                                    double          length1, 
                                                    double          radius1,
                                              const MbCartPoint3D & origin2, 
                                              const MbVector3D &    direction2,
                                                    double          length2, 
                                                    double          radius2,
                                                    MbCartPoint3D & result1, 
                                                    MbCartPoint3D & result2 ); 


//------------------------------------------------------------------------------
/** \brief \ru Создать эквидистантную кривую.
           \en Create an offset curve. \~
  \details \ru Создать эквидистантную кривую по плоской кривой. \n
           \en Create an offset curve from a planar curve. \n \~
  \param[in] curve - \ru Исходная кривая.
                     \en The initial curve. \~
  \param[in] dist - \ru Величина эквидистанты.
                    \en The offset distance. \~
  \param[out] result - \ru Эквидистантная кривая.
                       \en The offset curve. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) OffsetPlaneCurve( const MbCurve3D &  curve, 
                                                 double       dist,
                                                 MbCurve3D *& result );


//------------------------------------------------------------------------------
/** \brief \ru Создать эквидистантную кривую в пространстве.
           \en Create an offset curve in space. \~
  \details \ru Создать эквидистантную кривую в пространстве по трехмерной кривой и вектору направления. \n
           \en Create an offset curve in space from a three-dimensional curve and a direction vector. \n \~
  \param[in] initCurve - \ru Пространственная кривая, к которой строится эквидистантная.
                         \en A space curve for which to construct the offset curve. \~
  \param[in] params    - \ru Параметры.
                         \en Parameters. \~
  \param[out] result   - \ru Каркас с построенными кривыми.
                         \en The frame with the constructed curves. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
//---
//DEPRECATE_DECLARE_REPLACE ( CreateOffsetWireFrame with MbOffsetWireFrameParams )
MATH_FUNC (MbResultType) OffsetCurve( const MbCurve3D &                  initCurve,
                                      const MbSpatialOffsetCurveParams & params,
                                            c3d::WireFrameSPtr &         result );


//------------------------------------------------------------------------------
/** \brief \ru Создать эквидистантную кривую на поверхности.
           \en Create an offset curve on a surface. \~
  \details \ru Создать эквидистантную кривую на поверхности по поверхностной кривой и значению смещения. \n
           \en Create an offset curve on a surface from a curve on the surface and a shift value. \n \~
  \param[in] curve   - \ru Кривая на поверхности грани face.
                       \en A curve on face 'face' surface. \~
  \param[in] params  - \ru Параметры.
                       \en Parameters. \~
  \param[out] result - \ru Каркас с построенными кривыми.
                       \en The frame with the constructed curves. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
//---
//DEPRECATE_DECLARE_REPLACE ( CreateOffsetWireFrame with MbOffsetWireFrameParams )
MATH_FUNC (MbResultType) OffsetCurve( const MbCurve3D &                  curve,
                                      const MbSurfaceOffsetCurveParams & params,
                                            c3d::WireFrameSPtr &         result );


//------------------------------------------------------------------------------
/** \brief \ru Создать проекцию кривой на поверхность.
           \en Create a curve projection onto the surface. \~
  \details \ru Создать проекцию кривой curve на поверхность surface (направление проецирования direction может быть nullptr). \n
           \en Create the projection of a curve onto surface 'surface' (the projection direction 'direction' can be nullptr). \n \~
  \param[in] surface - \ru Поверхность для проецирования.
                       \en The surface to project onto. \~
  \param[in] curve - \ru Проецируемая кривая.
                     \en The curve to project. \~
  \param[in] direction - \ru Направление проецирования (если не указано то проецирование по нормали).
                         \en The projection direction (if not specified, the projection along the normal). \~
  \param[in] createExact - \ru Создавать проекционную кривую при необходимости.
                           \en Create a projection curve if necessary. \~
  \param[in] truncateByBounds - \ru Усекать границами поверхности.
                                \en Truncate by the surface bounds. \~
  \param[in] version - \ru Версия исполнения.
                       \en The version. \~
  \param[out] result - \ru Множество кривых на поверхности.
                       \en An array of curves on the surface. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
//DEPRECATE_DECLARE_REPLACE ( ProjectionWireFrame with MbProjectionWireFrameParams and MbProjectionWireFrameResults )
MATH_FUNC (MbResultType) CurveProjection( const MbSurface &          surface, 
                                          const MbCurve3D &          curve, 
                                                MbVector3D *         direction, 
                                                bool                 createExact, 
                                                bool                 truncateByBounds,
                                                RPArray<MbCurve3D> & result,
                                                VERSION              version = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Создать проекцию кривой на поверхность.
           \en Create a curve projection onto the surface. \~
  \details \ru Создать проекцию кривой curve на поверхность surface (направление проецирования direction может быть nullptr). \n
           \en Create the projection of a curve onto surface 'surface' (the projection direction 'direction' can be nullptr). \n \~
  \param[in] surface - \ru Поверхность для проецирования.
                       \en The surface to project onto. \~
  \param[in] curve - \ru Проецируемая кривая.
                     \en The curve to project. \~
  \param[in] direction - \ru Направление проецирования (если не указано то проецирование по нормали).
                         \en The projection direction (if not specified, the projection along the normal). \~
  \param[in] createExact - \ru Создавать проекционную кривую при необходимости.
                           \en Create a projection curve if necessary. \~
  \param[in] truncateByBounds - \ru Усекать границами поверхности.
                                \en Truncate by the surface bounds. \~
  \param[in] version - \ru Версия исполнения.
                       \en The version. \~
  \param[out] result - \ru Множество кривых на поверхности.
                       \en An array of curves on the surface. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
//DEPRECATE_DECLARE_REPLACE ( ProjectionWireFrame with MbProjectionWireFrameParams and MbProjectionWireFrameResults )
MATH_FUNC (MbResultType) CurveProjection( const MbSurface &                  surface, 
                                          const MbCurve3D &                  curve, 
                                                MbVector3D *                 direction, 
                                                bool                         createExact, 
                                                bool                         truncateByBounds,
                                                c3d::SpaceCurvesSPtrVector & result,
                                                VERSION                      version = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Создать пространственную кривую по двум плоским проекциям.
           \en Create a space curve from two planar projections. \~
  \details \ru Создать пространственную кривую по двум плоским проекциям. \n
           \en Create a space curve from two planar projections. \n \~
  \param[in] place1 - \ru Локальная система координат 1.
                      \en A local coordinate system 1. \~
  \param[in] curve1 - \ru Двумерная кривая 1.
                      \en A two-dimensional curve 1. \~
  \param[in] place2 - \ru Локальная система координат 2.
                      \en A local coordinate system 2. \~
  \param[in] curve2 - \ru Двумерная кривая 2.
                      \en A two-dimensional curve 2. \~
  \param[out] result - \ru Множество трехмерных кривых.
                       \en The array of three-dimensional curves. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC (MbResultType) CurveByTwoProjections( const MbPlacement3D &      place1, 
                                                const MbCurve &            curve1,
                                                const MbPlacement3D &      place2, 
                                                const MbCurve &            curve2,
                                                      RPArray<MbCurve3D> & result,
                                                      VERSION              version = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Создать проекционную кривую по нормали или по направлению.
           \en Create a projection curve from a normal or from a direction. \~
  \details \ru Создать проекционную кривую по нормали или по направлению. \n
    Если проекция на конструктивную плоскость (плоскость без границ), 
    то создать на ее основе грань и прислать, а за ее удалением следит приславший. \n
           \en Create a projection curve from a normal or from a direction. \n
    If the projection is onto the constructive plane (a plane without bounds), 
    create a face on the basis of this projection. \n \~
  \param[in] curve - \ru Проецируемая кривая.
                     \en The curve to project. \~
  \param[in] faces - \ru Связный набор граней.
                     \en A connected set of faces. \~
  \param[in] dir - \ru Вектор направления (если его нет, проекция по нормали).
                   \en The direction vector (if it is absent, the normal projection). \~
  \param[in] createExact - \ru Создавать проекционную кривую при необходимости.
                           \en Create a projection curve if necessary. \~
  \param[in] truncateByBounds - \ru Усечь границами.
                                \en Truncate by bounds. \~
  \param[in] snMaker - \ru Именователь с версией.
                       \en An object defining the names with the version. \~
  \param[out] result - \ru Проекционные кривые.
                       \en The projection curves. \~
  \param[out] resultIndices - \ru Индексы соответствия (номера граней в исходном массиве).
                              \en The indices of faces in the initial array. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
//---
//DEPRECATE_DECLARE_REPLACE ( ProjectionWireFrame with MbProjectionWireFrameParams and MbProjectionWireFrameResults )
MATH_FUNC (MbResultType) ProjectionCurve( const MbCurve3D &          curve,
                                          const RPArray<MbFace> &    faces,
                                          const MbVector3D *         dir,
                                          const bool                 createExact,
                                          const bool                 truncateByBounds,
                                          const MbSNameMaker &       snMaker,
                                                RPArray<MbCurve3D> & result,
                                                SArray<uintptr_t> *  resultIndices );


//------------------------------------------------------------------------------
/** \brief \ru Создать проекционный проволочный каркас по нормали или по направлению.
           \en Create a projection wireframe from a normal or from a direction. \~
  \details \ru Создать проекционный проволочный каркас по нормали или по направлению. \n
    Если проекция на конструктивную плоскость (плоскость без границ), 
    то создать на ее основе грань и прислать, а за ее удалением следит приславший. \n
           \en Create a projection curve from a normal or from a direction. \n
    If the projection is onto the constructive plane (a plane without bounds), 
    create a face on the basis of this projection. \n \~
  \param[in] wireFrame - \ru Проецируемый проволочный каркас.
                         \en The wireframe to project. \~
  \param[in] sameWireFrame - \ru Использовать тот же экземпляр проволочного каркаса, или создать копию.
                             \en Flag whether to use the same wireframe or make a copy of it. \~
  \param[in] solid - \ru Тело.
                     \en Solid. \~
  \param[in] same - \ru Использовать ли тот же экземпляр журнала тела или создать копию.
                    \en Flag whether to use the same creators of the body or make a copy. \~
  \param[in] faceIndices - \ru Номера граней в первой оболочке.
                           \en The numbers of faces in the first shell. \~
  \param[in] dir - \ru Вектор направления (если его нет, проекция по нормали).
                   \en The direction vector (if it is absent, the normal projection). \~
  \param[in] createExact - \ru Создавать проекционную кривую при необходимости.
                           \en Create a projection curve if necessary. \~
  \param[in] truncateByBounds - \ru Усечь границами.
                                \en Truncate by bounds. \~
  \param[in] snMaker - \ru Именователь с версией.
                       \en An object defining the names with the version. \~
  \param[out] resFrame - \ru Результирующий проволочный каркас, в котором в атрибутах ребер лежат имена соответствующих граней.
                         \en The resulting wireframe, the attributes of the edges contain the names of corresponding faces. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
//---
//DEPRECATE_DECLARE_REPLACE ( ProjectionWireFrame with MbProjectionWireFrameParams and MbProjectionWireFrameResults )
MATH_FUNC (MbResultType) ProjectionCurve( const MbWireFrame &    wireFrame,
                                          const bool             sameWireFrame,
                                          const MbSolid &        solid,
                                          const bool             same,
                                          const SArray<size_t> & faceIndices,
                                          const MbVector3D *     dir,
                                          const bool             createExact,
                                          const bool             truncateByBounds,
                                          const MbSNameMaker &   snMaker,
                                                MbWireFrame *&   resFrame );


//------------------------------------------------------------------------------
/** \brief \ru Устранить наложение сегментов проекционной кривой.
           \en Eliminate the projection curve segments overlay. \~
  \details \ru Устранить наложение сегментов проекционной кривой (вспомогательная функция для функции ProjectionCurve). \n
           \en Eliminate the projection curve segments overlay (an auxiliary function for function ProjectionCurve). \n \~
  \param[in,out] curves - \ru Множество кривых.
                          \en An array of curves. \~
  \param[in,out] indices - \ru Множество индексов, синхронный с массивом кривых.
                           \en An array of indices synchronized with the array of curves. \~
  \return \ru Возвращает true, если что-то изменилось в наборе кривых.
          \en Returns true if something has modified in the curve set. \~
  \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC (bool) EliminateProjectionCurveOverlay( RPArray<MbCurve3D> & curves, SArray<uintptr_t> * indices );


//------------------------------------------------------------------------------
/** \brief \ru Создать массив линий очерка поверхности.
           \en Create an array of isocline curves of the surface. \~
  \details \ru Создать массив линий очерка поверхности с обрезкой по области определения. \n
           \en Create an array of isocline curves of the surface with truncation by the definition domain. \n \~
  \param[in] surface - \ru Поверхность.
                       \en The surface. \~
  \param[in] eye - \ru Вектор взгляда.
                   \en The direction of view. \~
  \param[in] perspective - \ru Является ли проекция перспективной.
                           \en Whether the projection is perspective. \~
  \param[in] removeOnSurfaceBounds - \ru Удалить линии очерка, совпадающие с границами поверхности.
                                     \en Remove the isocline curves coincident with the surface bounds. \~
  \param[out] result - \ru Выходной массив линий очерка.
                       \en The output array of isocline curves. \~
  \param[in] version - \ru Версия построения.
                       \en The version. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Mapping
*/
// ---
MATH_FUNC (MbResultType) SilhouetteCurve( const MbSurface &          surface, 
                                          const MbVector3D &         eye, 
                                                bool                 perspective,
                                                bool                 removeOnSurfaceBounds,
                                                RPArray<MbCurve3D> & result, 
                                                VERSION              version = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Создать массив линий очерка грани.
           \en Create an array of isocline curves of the face. \~
  \details \ru Создать массив линий очерка грани с обрезкой по области определения. \n
           \en Create an array of isocline curves of the face with truncation by the definition domain. \n \~
  \param[in] face - \ru Грани.
                    \en The face. \~
  \param[in] eye - \ru Вектор взгляда.
                   \en The direction of view. \~
  \param[in] perspective - \ru Является ли проекция перспективной.
                           \en Whether the projection is perspective. \~
  \param[out] result - \ru Выходной массив линий очерка.
                       \en The output array of isocline curves. \~
  \param[in] version - \ru Версия построения.
                       \en The version. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Mapping
*/
// ---
MATH_FUNC (MbResultType) SilhouetteCurve( const MbFace &             face, 
                                          const MbVector3D &         eye, 
                                                bool                 perspective,
                                                RPArray<MbCurve3D> & result, 
                                                VERSION              version = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Создать массив линий очерка поверхности при вращательном движении вокруг оси.
           \en Create an array of isocline curves of the rotated surface. \~
  \details \ru Создать массив линий очерка поверхности с обрезкой по области определения. \n
           \en Create an array of isocline curves of the surface with truncation by the definition domain. \n \~
  \param[in] surface - \ru Поверхность.
                       \en The surface. \~
  \param[in] axis - \ru Ось кругового взгляда (ось токарного сечения).
                    \en The axis of lathe section. \~
  \param[in] removeOnSurfaceBounds - \ru Удалить линии очерка, совпадающие с границами поверхности.
                                     \en Remove the isocline curves coincident with the surface bounds. \~
  \param[out] curves - \ru Выходной массив линий очерка.
                       \en The output array of isocline curves. \~
  \param[in] version - \ru Версия построения.
                       \en The version. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Mapping
*/
// ---
MATH_FUNC (MbResultType) SilhouetteCurve( const MbSurface &          surface, 
                                          const MbAxis3D &           axis, 
                                                bool                 removeOnSurfaceBounds, 
                                                RPArray<MbCurve3D> & curves, 
                                                VERSION              version = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Создать массив линий очерка грани при вращательном движении вокруг оси.
           \en Create an array of isocline curves of the rotated face. \~
  \details \ru Создать массив линий очерка грани с обрезкой по области определения. \n
           \en Create an array of isocline curves of the face with truncation by the definition domain. \n \~
  \param[in] face - \ru Грани.
                    \en The face. \~
  \param[in] axis - \ru Ось кругового взгляда (ось токарного сечения).
                    \en The axis of lathe section. \~
  \param[out] result - \ru Выходной массив линий очерка.
                       \en The output array of isocline curves. \~
  \param[in] version - \ru Версия построения.
                       \en The version. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Mapping
*/
// ---
MATH_FUNC (MbResultType) SilhouetteCurve( const MbFace &             face, 
                                          const MbAxis3D &           axis, 
                                                RPArray<MbCurve3D> & curves, 
                                                VERSION              version = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Создать кривые пересечения двух поверхностей.
           \en Create the intersection curves of two surfaces. \~
  \details \ru Создать кривые пересечения двух поверхностей. Результат - массив кривых пересечения поверхностей. \n
           \en Create the intersection curves of two surfaces. The result is an array of intersection curves of surfaces. \n \~
  \deprecated \ru Функция устарела, взамен использовать #IntersectionCurve с набором параметров #MbIntCurveParams и #MbIntCurveResults.
              \en The function is deprecated, instead use #IntersectionCurve with the parameter list #MbIntCurveParams and #MbIntCurveResults. \~
  \param[in] surface1 - \ru Первая поверхность.
                        \en The first surface. \~
  \param[in] surface2 - \ru Вторая поверхность.
                        \en The second surface. \~
  \param[in] params   - \ru Параметры.
                        \en Parameters. \~
  \param[out] result  - \ru Каркас с построенными кривыми.
                        \en The frame with the constructed curves. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \warning \ru Лучше использовать IntersectionCurve на гранях, т.к. границы поверхностей могут бы неточные, \n
           что приведет к неточному положению концов кривых пересечения в результате операции. \n
           В гранях же границы поверхности точные, т.к. хранятся в виде кривых пересечения,
           а не виде двумерных кривых. \n
           \en It is better to use IntersectionCurve on faces since the surfaces bounds can be inexact, \n
           and it will result in inexact position of intersection curves ends. \n
           But the surface bounds in faces are exact since they are stored in the form of intersection curves,
           not in the form of two-dimensional curves. \n \~
  \ingroup Curve3D_Modeling
*/ // ---
//DEPRECATE_DECLARE_REPLACE ( IntersectionCurve with MbIntCurveParams and MbIntCurveResults )
MATH_FUNC (MbResultType) IntersectionCurve( const MbSurface &        surface1, 
                                            const MbSurface &        surface2, 
                                            const MbIntCurveParams & params,
                                                  MbWireFrame *&     result );


//------------------------------------------------------------------------------
/** \brief \ru Создать кривые пересечения двух граней.
           \en Create intersection curves of two faces. \~
  \details \ru Создать кривые пересечения двух граней. Результат - массив кривых пересечения поверхностей. \n
           \en Create intersection curves of two faces. The result is an array of intersection curves of surfaces. \n \~
  \deprecated \ru Функция устарела, взамен использовать #IntersectionCurve с набором параметров #MbIntCurveParams и #MbIntCurveResults.
              \en The function is deprecated, instead use #IntersectionCurve with the parameter list #MbIntCurveParams and #MbIntCurveResults. \~
  \param[in] face1    - \ru Первая грань оболочки.
                        \en The first face of the shell. \~
  \param[in] face2    - \ru Вторая грани оболочки.
                        \en The second face of the shell. \~
  \param[in] params   - \ru Параметры.
                        \en Parameters. \~
  \param[out] result  - \ru Каркас с построенными кривыми.
                        \en The frame with the constructed curves. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/ // ---
//DEPRECATE_DECLARE_REPLACE ( IntersectionCurve with MbFace and MbIntCurveResults )
MATH_FUNC (MbResultType) IntersectionCurve(       MbFace &           face1,
                                                  MbFace &           face2, 
                                            const MbIntCurveParams & params,
                                                  MbWireFrame *&     result );


//------------------------------------------------------------------------------
/** \brief \ru Создать кривые пересечения двух граней.
           \en Create intersection curves of two faces. \~
  \details \ru Создать кривые пересечения двух граней. Результат - массив кривых пересечения поверхностей. \n
           \en Create intersection curves of two faces. The result is an array of intersection curves of surfaces. \n \~
  \param[in] face1    - \ru Первая грань оболочки.
                        \en The first face of the shell. \~
  \param[in] face2    - \ru Вторая грани оболочки.
                        \en The second face of the shell. \~
  \param[in] params   - \ru Параметры.
                        \en Parameters. \~
  \param[out] results - \ru Выходные параметры.
                        \en Output parameters. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/ // ---
MATH_FUNC (MbResultType) IntersectionCurve(       MbFace &           face1, 
                                                  MbFace &           face2, 
                                            const MbIntCurveParams & params,
                                                  MbIntCurveResults & results );


//------------------------------------------------------------------------------
/** \brief \ru Создать кривые пересечения граней двух оболочек.
           \en Create intersection curves of two shells faces. \~
  \details \ru Создать кривые пересечения граней двух оболочек. Результат - массив кривых пересечения поверхностей. \n
           \en Create intersection curves of two shells faces. The result is an array of intersection curves of surfaces. \n \~
  \deprecated \ru Функция устарела, взамен использовать #IntersectionCurve с набором параметров #MbIntCurveParams и #MbIntCurveResults.
              \en The function is deprecated, instead use #IntersectionCurve with the parameter list #MbIntCurveShellParams and #MbIntCurveResults. \~
  \param[in] solid1       - \ru Первая оболочка.
                            \en The first shell. \~
  \param[in] faceIndices1 - \ru Номера граней в первой оболочке.
                            \en The numbers of faces in the first shell. \~
  \param[in] solid2       - \ru Вторая оболочка.
                            \en The second shell. \~
  \param[in] faceIndices2 - \ru Номера граней во второй оболочке.
                            \en The numbers of faces in the second shell. \~
  \param[in] params       - \ru Параметры.
                            \en Parameters. \~
  \param[out] result      - \ru Каркас с построенными кривыми.
                            \en The frame with the constructed curves. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/ // ---
//DEPRECATE_DECLARE_REPLACE ( IntersectionCurve with MbIntCurveShellParams and MbIntCurveResults )
MATH_FUNC (MbResultType) IntersectionCurve( const MbSolid &            solid1,
                                            const c3d::IndicesVector & faceIndices1,
                                            const MbSolid &            solid2, 
                                            const c3d::IndicesVector & faceIndices2,
                                            const MbIntCurveParams &   params,
                                                  MbWireFrame *&       result );


//------------------------------------------------------------------------------
/** \brief \ru Создать кривые пересечения граней двух оболочек.
           \en Create intersection curves of two shells faces. \~
  \details \ru Создать кривые пересечения граней двух оболочек. Результат - массив кривых пересечения поверхностей. \n
           \en Create intersection curves of two shells faces. The result is an array of intersection curves of surfaces. \n \~
  \deprecated \ru Функция устарела, взамен использовать #IntersectionCurve с набором параметров #MbIntCurveParams и #MbIntCurveResults.
              \en The function is deprecated, instead use #IntersectionCurve with the parameter list #MbIntCurveShellParams and #MbIntCurveResults. \~
  \param[in] solid1       - \ru Первая оболочка.
                            \en The first shell. \~
  \param[in] faceIndices1 - \ru Номера граней в первой оболочке.
                            \en The numbers of faces in the first shell. \~
  \param[in] same1        - \ru Использовать ли тот же журнал построителей первого тела или сделать копию.
                            \en Flag whether to use the same creators of the first body or make a copy. \~
  \param[in] solid2       - \ru Вторая оболочка.
                            \en The second shell. \~
  \param[in] faceIndices2 - \ru Номера граней во второй оболочке.
                            \en The numbers of faces in the second shell. \~
  \param[in] same2        - \ru Использовать ли тот же самый журнал построителей второго тела или сделать копию.
                            \en Flag whether to use the same creators of the second body or make a copy. \~
  \param[in] params       - \ru Параметры.
                            \en Parameters. \~
  \param[out] result      - \ru Каркас с построенными кривыми.
                            \en The frame with the constructed curves. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/ // ---
//DEPRECATE_DECLARE_REPLACE ( IntersectionCurve with MbIntCurveShellParams and MbIntCurveResults )
MATH_FUNC (MbResultType) IntersectionCurve( const MbSolid &            solid1,
                                            const c3d::IndicesVector & faceIndices1, 
                                                  bool                 same1,
                                            const MbSolid &            solid2, 
                                            const c3d::IndicesVector & faceIndices2, 
                                                  bool                 same2,
                                            const MbIntCurveParams &   params, 
                                                  MbWireFrame *&       result );


//------------------------------------------------------------------------------
/** \brief \ru Создать кривые пересечения граней двух оболочек.
           \en Create intersection curves of two shells faces. \~
  \details \ru Создать кривые пересечения граней двух оболочек. Результат - массив кривых пересечения поверхностей. \n
           \en Create intersection curves of two shells faces. The result is an array of intersection curves of surfaces. \n \~
  \param[in] solid1   - \ru Первая оболочка.
                        \en The first shell. \~
  \param[in] solid2   - \ru Вторая оболочка.
                        \en The second shell. \~
  \param[in] params   - \ru Входные параметры.
                        \en Inpit parameters. \~
  \param[out] results - \ru Выходные параметры.
                        \en Output parameters. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/ // ---
MATH_FUNC (MbResultType) IntersectionCurve( const MbSolid &               solid1,
                                            const MbSolid &               solid2,
                                            const MbIntCurveShellParams & params,
                                                  MbIntCurveResults &     results );


//------------------------------------------------------------------------------
/** \brief \ru Создать линию пересечения поверхностей.
           \en Create an intersection curve of surfaces. \~
  \details \ru Создать линию пересечения двух поверхностей. Возможные варианты создания:\n
               *) В общем случае с каркасом #MbWireFrame на выходе.
                  Warning:
                    В общем случае лучше использовать IntersectionCurve на гранях, т.к. границы поверхностей могут бы неточные,
                    что приведет к неточному положению концов кривых пересечения в результате операции.
                    В гранях же границы поверхности точные, т.к. хранятся в виде кривых пересечения, а не виде двумерных кривых. \n
               *) По известному началу линии пересечения в заданном направлении. \n
               *) По известным началу и концу линии пересечения и начальному направлению. \n
               *) По известным началу и концу линии пересечения и вспомогательной кривой. \n
            \en Create an intersection curve of the two surfaces. Possible options of creation are: \n
               *) General case resulting with #MbWireFrame.
                   Warning:
                    In general case it is better to use IntersectionCurve on faces since the surfaces bounds can be inexact,
                    and it will result in inexact position of intersection curve edges.
                    But the surface bounds in faces are exact since they are stored in the form of intersection curves,
                    not in the form of two-dimensional curves. \n
               *) From the specified starting point and the initial direction of the intersection curve. \n
               *) From the specified starting and ending points of the intersection curve and the initial direction. \n
               *) From the specified starting and ending points of the intersection curve and the guide curve that approximates the desired curve. \n \~
           
  \param[in] surface1 - \ru Первая поверхность.
                        \en The first surface. \~
  \param[in] surface2 - \ru Вторая поверхность.
                        \en The second surface. \~
  \param[in] params   - \ru Входные параметры.
                        \en Inpit parameters. \~
  \param[out] results - \ru Выходные параметры.
                        \en Output parameters. \~
  \return \ru Возвращает код результата операции.
          \en Returns the operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) IntersectionCurve( const MbSurface         & surface1, 
                                            const MbSurface         & surface2, 
                                            const MbIntCurveParams  & params,
                                                  MbIntCurveResults & results );


//------------------------------------------------------------------------------
/** \brief \ru Создать линию пересечения поверхностей.
           \en Create an intersection curve of surfaces. \~
  \details \ru Создать линию пересечения поверхностей surf1 и surf2 по известным началу и концу линии пересечения. \n
           \en Create an intersection curve of surfaces 'surf1' and 'surf2' from the specified start point and end point of the intersection curve. \n \~
  \deprecated \ru Функция устарела, взамен использовать #IntersectionCurve с набором параметров #MbIntCurveParams и #MbIntCurveResults.
              \en The function is deprecated, instead use #IntersectionCurve with the parameter list #MbIntCurveParams and #MbIntCurveResults. \~
  \param[in] surface1 - \ru Первая поверхность.
                        \en The first surface. \~
  \param[in] ext1 - \ru На расширенной первой поверхности.
                    \en Whether to create on the extended surface. \~
  \param[in] uv1beg - \ru Начальная точка на первой поверхности.
                      \en The start point on the first surface. \~
  \param[in] uv1end - \ru Конечная точка на первой поверхности.
                      \en The end point on the first surface. \~
  \param[in] surface2 - \ru Вторая поверхность.
                        \en The second surface. \~
  \param[in] ext2 - \ru На расширенной второй поверхности.
                    \en Whether to create on the extended second surface. \~
  \param[in] uv2beg - \ru Начальная точка на второй поверхности.
                      \en The start point on the second surface. \~
  \param[in] uv2end - \ru Конечная точка на второй поверхности.
                      \en The end point on the second surface. \~
  \param[in] dir - \ru Начальное направление создания линии пересечения.
                   \en The start direction for intersection curve creation. \~
  \param[out] result1 - \ru Двумерная кривая на первой поверхности.
                        \en The two-dimensional curve on the first surface. \~
  \param[out] result2 - \ru Двумерная кривая на второй поверхности.
                        \en The two-dimensional curve on the second surface. \~
  \param[out] label - \ru Тип полученной кривой пересечения.
                      \en The resultant intersection curve type. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
//DEPRECATE_DECLARE_REPLACE ( IntersectionCurve with MbIntCurveParams and MbIntCurveResults )
MATH_FUNC (MbResultType) IntersectionCurve( const MbSurface &         surface1, 
                                                  bool                ext1, 
                                            const MbCartPoint &       uv1beg, 
                                            const MbCartPoint &       uv1end,
                                            const MbSurface &         surface2, 
                                                  bool                ext2, 
                                            const MbCartPoint &       uv2beg,
                                            const MbCartPoint &       uv2end,
                                            const MbVector3D &        dir, 
                                                  MbCurve *&          result1, 
                                                  MbCurve *&          result2, 
                                                  MbeCurveBuildType & label );


//------------------------------------------------------------------------------
/** \brief \ru Создать линию пересечения поверхностей.
           \en Create an intersection curve of surfaces. \~
  \details \ru Создать линию пересечения поверхностей surf1 и surf2 по известным началу и концу линии пересечения и вспомогательной кривой. \n
           \en Create an intersection curve of surfaces 'surf1' and 'surf2' from the specified start point and end point of the intersection curve
               and guide curve that approximates the desired curve. \n \~
  \deprecated \ru Функция устарела, взамен использовать #IntersectionCurve с набором параметров #MbIntCurveParams и #MbIntCurveResults.
              \en The function is deprecated, instead use #IntersectionCurve with the parameter list #MbIntCurveParams and #MbIntCurveResults. \~
  \param[in] surface1 - \ru Первая поверхность.
                        \en The first surface. \~
  \param[in] ext1 - \ru На расширенной первой поверхности.
                    \en Whether to create on the extended surface. \~
  \param[in] uv1beg - \ru Начальная точка на первой поверхности.
                      \en The start point on the first surface. \~
  \param[in] uv1end - \ru Конечная точка на первой поверхности.
                      \en The end point on the first surface. \~
  \param[in] surface2 - \ru Вторая поверхность.
                        \en The second surface. \~
  \param[in] ext2 - \ru На расширенной второй поверхности.
                    \en Whether to create on the extended second surface. \~
  \param[in] uv2beg - \ru Начальная точка на второй поверхности.
                      \en The start point on the second surface. \~
  \param[in] uv2end - \ru Конечная точка на второй поверхности.
                      \en The end point on the second surface. \~
  \param[in] guideCurve - \ru Направляющая кривая, приближенно описывающая искомую кривую.
                          \en The guide curve that approximates the desired curve. \~
  \param[in] useRedetermination - \ru Флаг, определяющий нужно ли уточнять шаг построения следующей точки по сравнению с функцией DeviationStep.
                                  \en The flag that determines whether it is necessary to specify the next point build step as compared to the DeviationStep function. \~
  \param[in] checkPoles - \ru Флаг необходимости проверки и корректировки полюсных точек.
                          \en The flag that determines whether it is necessary to check and correct pole points. \~ 
  \param[out] result1 - \ru Двумерная кривая на первой поверхности.
                        \en The two-dimensional curve on the first surface. \~
  \param[out] result2 - \ru Двумерная кривая на второй поверхности.
                        \en The two-dimensional curve on the second surface. \~
  \param[out] label - \ru Тип полученной кривой пересечения.
                      \en The resultant intersection curve type. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
//DEPRECATE_DECLARE_REPLACE ( IntersectionCurve with MbIntCurveParams and MbIntCurveResults )
MATH_FUNC( MbResultType ) IntersectionCurve( const MbSurface &         surf1, 
                                                   bool                ext1,
                                             const MbCartPoint &       uv1beg,
                                             const MbCartPoint &       uv1end,
                                             const MbSurface &         surf2, 
                                                   bool                ext2,
                                             const MbCartPoint &       uv2beg,
                                             const MbCartPoint &       uv2end,
                                             const MbCurve3D *         guideCurve,
                                                   bool                useRedetermination,
                                                   bool                checkPoles,
                                                   MbCurve *&          pCurve1,
                                                   MbCurve *&          pCurve2,
                                                   MbeCurveBuildType & label );


//------------------------------------------------------------------------------
/** \brief \ru Создать пространственный сплайн через точки и с сопряжениями.
           \en Create a spatial spline through points and with the given derivatives. \~
  \details \ru Создать пространственный сплайн через точки и с сопряжениями. \n
    Примечания: \n
    Если есть сопряжения, то количество сопряжений должно быть равно количеству точек. \n
    Отсутствующие сопряжения должны быть представлены нулевыми указателями в массиве \n
           \en Create a spatial spline through points and with the given derivatives. \n
    Notes: \n
    If derivatives are specified, the number of derivatives should be equal to the number of points. \n
    Missing derivatives should be represented by null pointers in the array \n \~
  \param[in] points - \ru Точки.
                      \en Points. \~
  \param[in] paramType - \ru Тип параметризации.
                         \en The parametrization type. \~
  \param[in] degree - \ru Порядок сплайна.
                      \en A spline degree. \~
  \param[in] closed - \ru Замкнутость сплайна.
                      \en The spline closedness. \~  
  \param[in] transitions - \ru Заданные сопряжения.
                           \en The specified derivatives. \~
  \param[in] snMaker - \ru Именователь кривых каркаса.
                       \en An object defining the frame curves names. \~
  \param[out] result - \ru Каркас с построенными кривыми.
                       \en The frame with the constructed curves. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC (MbResultType) SpaceSplineThrough( const SArray<MbCartPoint3D> &         points,
                                                   MbeSplineParamType              paramType,
                                                   size_t                          degree,
                                                   bool                            closed,
                                                   RPArray<c3d::PntMatingData3D> & transitions,
                                             const MbSNameMaker &                  snMaker,
                                                   MbWireFrame *&                  result );


//------------------------------------------------------------------------------
/** \brief \ru Создать пространственный сплайн по точкам и с сопряжениями.
           \en Create a spatial spline from points and derivatives. \~
  \details \ru Создать пространственный сплайн по точкам и с сопряжениями. \n
           \en Create a spatial spline from points and derivatives. \n \~
  \param[in] points   - \ru Множество точек.
                        \en An array of points. \~
  \param[in] degree   - \ru Порядок сплайна.
                        \en A spline degree. \~
  \param[in] closed   - \ru Строить замкнутый сплайн.
                        \en Create a closed spline. \~
  \param[in] weights  - \ru Множество весов точек.
                        \en An array of points weights. \~
  \param[in] knots    - \ru Узловой вектор сплайна.
                        \en A knot vector of the spline. \~
  \param[in] begData  - \ru Сопряжение в начале.
                        \en The start derivative. \~
  \param[in] endData  - \ru Сопряжение в конце.
                        \en The end derivative. \~
  \param[in] snMaker - \ru Именователь кривых каркаса.
                       \en An object defining the frame curves names. \~
  \param[out] result - \ru Каркас с построенными кривыми.
                       \en The frame with the constructed curves. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC (MbResultType) SpaceSplineBy( const SArray<MbCartPoint3D> & points,
                                              size_t                  degree,
                                              bool                    closed,
                                        const SArray<double> *        weights,
                                        const SArray<double> *        knots,
                                              c3d::PntMatingData3D *  begData,
                                              c3d::PntMatingData3D *  endData,
                                        const MbSNameMaker &          snMaker,
                                              MbWireFrame *&          result );


//------------------------------------------------------------------------------
/** \brief \ru Создать кривую на поверхности.
           \en Create a curve on a surface. \~
  \details \ru Создать кривую на поверхности. \n
    Примечания: \n
    1. Если есть сопряжения, то количество сопряжений должно быть равно количеству точек. \n
       Отсутствующие сопряжения должны быть представлены нулевыми указателями в массиве \n
    2. Если сплайн строится через точки, то сопряжения могуть быть заданы произвольно. \n
    2. Если сплайн строится по полюсам и он незамкнут, то сопряжения могут быть только на концах. \n
    3. Если сплайн строится по полюсам и он замкнут, то сопряжения должны отсутствовать. \n
    4. Множество весов должен быть пуст или синхронизирован с массивом точек по количеству 
       (с опцией throughPoints веса игнорируются). \n
           \en Create a curve on a surface. \n
    Notes: \n
    1. If derivatives are specified, the number of derivatives should be equal to the number of points. \n
       Missing derivatives should be represented by null pointers in the array \n
    2. If the spline is created from points, arbitrary derivatives can be defined. \n
    2. If the spline is created from poles and it is open, only the end derivatives can be specified. \n
    3. If the spline is constructed from poles and it is closed, the derivatives cannot be specified. \n
    4. An array of weights should be empty or synchronized with the point array by size 
       (with option throughPoints weights are ignored). \n \~
  \param[in] surface        - \ru Поверхность.
                              \en The surface. \~
  \param[in] throughPoints  - \ru Провести сплайн через точки.
                              \en Create a spline through points. \~
  \param[in] paramPnts      - \ru Множество параметрических точек.
                              \en An array of parametric points. \~
  \param[in] paramWts       - \ru Множество весов параметрических точек.
                              \en An array of parametric point weights. \~
  \param[in] paramClosed    - \ru Строить замкнутый параметрический сплайн.
                              \en Create a closed parametric spline. \~
  \param[in] spaceTransitions - \ru Сопряжения в точках.
                                \en Derivatives in the points. \~
  \param[in] snMaker      - \ru Именователь кривых каркаса.
                            \en An object defining the frame curves names. \~
  \param[out] result      - \ru Каркас с построенными кривыми.
                            \en The frame with the constructed curves. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC (MbResultType) SurfaceSpline( const MbSurface &                     surface,
                                              bool                            throughPoints,
                                              SArray<MbCartPoint> &           paramPnts,
                                              SArray<double> &                paramWts,
                                              bool                            paramClosed,
                                              RPArray<c3d::PntMatingData3D> & spaceTransitions,
                                        const MbSNameMaker &                  snMaker,
                                              MbWireFrame *&                  result );


//------------------------------------------------------------------------------
/** \brief \ru Создать изопараметрическую кривую.
           \en Create an isoparametric curve. \~
  \details \ru Создать изопараметрическую кривую на поверхности surface. \n
           \en Create an isoparametric curve on surface 'surface'. \n \~
  \param[in] surface - \ru Поверхность.
                       \en The surface. \~
  \param[in] x - \ru Значение по первому параметру.
                 \en A value of the first parameter. \~
  \param[in] isU - \ru Первый параметр есть U.
                   \en Whether the first parameter is U. \~
  \param[in] yRange - \ru Диапазон по второму параметру (если не задан, используются параметрические границы поверхности).
                      \en A range of the second parameter (if not defined, the parametric bounds of the surface are used). \~
  \param[out] result - \ru Изопараметрическая кривая.
                       \en The isoparametric curve. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC (MbResultType) IsoparametricCurve( const MbSurface &  surface, 
                                                   double       x, 
                                                   bool         isU, 
                                             const MbRect1D *   yRange,
                                                   MbCurve3D *& result );


//------------------------------------------------------------------------------
/** \brief \ru Создать кривую - мостик, соединяющую кривые curve1 и curve2.
           \en Create a transition curve connecting curves 'curve1' and 'curve2'. \~
  \details \ru Создать кривую - мостик, соединяющую кривые curve1 и curve2 кубическим сплайном Эрмита. \n
           \en Create a transition curve connecting curves 'curve1' and 'curve2' by a cubic Hermite spline. \n \~
  \param[in] curve1   - \ru Сопрягаемая кривая 1.
                        \en A curve 1 to be connected. \~
  \param[in] t1       - \ru Параметр точки на сопрягаемой кривой 1.
                        \en A point parameter on the curve 1. \~
  \param[in] sense1   - \ru Начало мостика совпадает с направлением кривой curve1 (true).
                        \en The beginning of the transition curve is equal to the direction of 'curve1' (true). \~
  \param[in] curve2   - \ru Сопрягаемая кривая 2.
                        \en A curve 2 to be connected. \~
  \param[in] t2       - \ru Параметр точки на сопрягаемой кривой 2.
                        \en A point parameter on the curve 2. \~
  \param[in] sense2   - \ru Конец  мостика совпадает с направлением кривой curve2 (true).
                        \en The end of the transition curve is equal to the direction of 'curve2' (true). \~
  \param[in] names    - \ru Именователь кривых каркаса.
                        \en An object defining the frame curves names. \~
  \param[out] result  - \ru Каркас с построенными кривыми.
                        \en The frame with the constructed curves. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) BridgeCurve( const MbCurve3D & curve1, double t1, bool sense1, 
                                      const MbCurve3D & curve2, double t2, bool sense2, 
                                      const MbSNameMaker & names,  
                                      MbWireFrame *& result ); 


//------------------------------------------------------------------------------
/** \brief \ru Создать составную кривую плавного соединения концов двух кривых.
           \en Create a composite curve smoothly connecting two curves ends. \~
  \details \ru Создать составную кривую плавного соединения концов двух кривых. \n
    Полученная кривая состоит из трёхмерной дуги радиуса radius1, 
    отрезка (в определенных случаях отрезок отсутствует), 
    трёхмерной дуги радиуса radius2.
           \en Create a composite curve smoothly connecting two curves ends. \n
    The constructed curve consists of a three-dimensional arc of radius 'radius1', 
    a segment (in specific cases a segment is absent), 
    a three-dimensional arc of radius 'radius2'. \~
  \param[in] curve1   - \ru Соединяемая кривая 1.
                        \en A curve 1 to be connected. \~ 
  \param[in] isBegin1 - \ru Начало соединяемой кривой 1 (true).
                        \en The beginning of the curve 1 (true). \~
  \param[in] radius1  - \ru Радиус сопряжения у соединяемой кривой 1.
                        \en The conjugation raidus of curve 1. \~
  \param[in] curve2   - \ru Соединяемая кривая 2.
                        \en A curve 2 to be connected. \~
  \param[in] isBegin2 - \ru Начало соединяемой кривой 2 (true).
                        \en The beginning of the curve 2 (true). \~
  \param[in] radius2  - \ru Радиус сопряжения у соединяемой кривой 2.
                        \en The conjugation raidus of curve 2. \~
  \param[in] names    - \ru Именователь кривых каркаса.
                        \en An object defining the frame curves names. \~
  \param[out] result  - \ru Каркас с построенными кривыми.
                        \en The frame with the constructed curves. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) ConnectingCurve( const MbCurve3D & curve1, bool isBegin1, double radius1,
                                          const MbCurve3D & curve2, bool isBegin2, double radius2,
                                          const MbSNameMaker & names,  
                                          MbWireFrame *& result );


//------------------------------------------------------------------------------
/** \brief \ru Создать соединительную NURBS кривую для кривых curve1 и curve2.
           \en Create a connecting NURBS curve for curves 'curve1' and 'curve2'. \~
  \details \ru Создать соединительную NURBS кривую для кривых curve1 и curve2. \n
    t1 и t2 - параметры кривых curve1 и curve2, в точках которых начинается и заканчивается соединение.\n
           \en Create a connecting NURBS curve for curves 'curve1' and 'curve2'. \n
    t1 and t2 are parameters of curves 'curve1' and 'curve2' which correspond to the start point and the end point of the connecting curve.\n \~
  \param[in] curve1   - \ru Соединяемая кривая 1.
                        \en A curve 1 to be connected. \~ 
  \param[in] t1       - \ru Параметр точки на кривой 1.
                        \en A point parameter on curve 1. \~
  \param[in] mating1  - \ru Тип соединения кривой 1.
                        \en The connection type for curve 1. \~
  \param[in] curve2   - \ru Соединяемая кривая 2.
                        \en A curve 2 to be connected. \~
  \param[in] t2       - \ru Параметр точки на кривой 2.
                        \en A point parameter on curve 2. \~
  \param[in] mating2  - \ru Тип соединения кривой 2.
                        \en The connection type for curve 2. \~
  \param[in] tension1 - \ru Параметр "натяжение" соединительной кривой на стыке с кривой 1 ( 0<= tension1 <=1).
                        \en The "tension" parameter of the connecting curve at the intersection with the curve 1 (0 <= tension1 <=1). \~
  \param[in] tension2 - \ru Параметр "натяжение" соединительной кривой на стыке с кривой 2( 0<= tension2 <=1).
                        \en The "tension" parameter of the connecting curve at the intersection with the curve 2 (0 <= tension2 <=1). \~
  \param[in] names    - \ru Именователь кривых каркаса.
                        \en An object defining the frame curves names. \~
  \param[out] result  - \ru Каркас с построенными кривыми.
                        \en The frame with the constructed curves. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC (MbResultType) ConnectingSpline( const MbCurve3D & curve1, double t1, MbeMatingType mating1,
                                           const MbCurve3D & curve2, double t2, MbeMatingType mating2,
                                           double tension1, double tension2,
                                           const MbSNameMaker & names,  
                                           MbWireFrame *& result ); 


//------------------------------------------------------------------------------
/** \brief \ru Создать кривую для плавного соединения (скругления) кривых.
           \en Create a fillet curve for curves. \~
  \details \ru Создать кривую для плавного соединения (скругления) кривых. \n
    Для плавного сопряжения кривых curve1 и curve2 строится кривая filletCurve. \n
    При входе t1 и t2 - начальные приближения, определяющие сектор построения скругления, 
              w1 и w2 - не используются. \n
    При входе type - тип скругления (обычное или на поверхности). \n
    На выходе t1 и t2 - будут равны параметрам касания кривых curve1 и curve2 с кривой filletCurve. \n
    На выходе t1 и w1 - определяют параметры сохраняемого участка при обрезке кривой curve1. \n
    На выходе t2 и w2 - определяют параметры сохраняемого участка при обрезке кривой curve2. \n
    Параметр radius - радиус дуги или цилиндра. \n
    Если радиус radius не задан (равен нулю), то он вычисляется из условия,
    что начало кривой сопряжения будет находится в точке с параметором t1,
    t1 и t2 - параметры кривых curve1 и curve2, в соответствующих точках которых начинается и заканчивается скругление. \n
    Параметр sense - прямое или обратное направление кривой скругления. \n
    Кривая filletCurve - это кривая сопряжения, дуга (когда surface == nullptr) или кривая на поверхности цилиндра surface. \n
    Поверхность surface - это цилиндрическая поверхность, на которой строится кривая сопряжения в общем случае.
    Для управления жизненным циклом поверхномти следует миспользовать методы ::AddRefItem(surface) и ::ReleaseItem(surface). \n
           \en Create a fillet curve for curves. \n
    Curve 'filletCurve' is created for smooth connection of curves 'curve1' and 'curve2'. \n
    On input t1 and t2 are the initial estimations which determine a sector for fillet construction, 
              w1 and w2 are not used. \n
    On input 'type' is a fillet type (ordinary or on a surface). \n
    On output t1 and t2 are the parameters of touching of curves 'curve1' and 'curve2' with curve 'filletCurve'. \n
    On output t1 and w1 determines parameters of a part to be kept while trimming curve1. \n
    On output t2 and w2 determines parameters of a part to be kept while trimming curve2. \n
    Parameter 'radius' is a radius of an arc or a cylinder. \n
    If radius 'radius' is not defined (equal to zero), it is computed from the condition
    that the fillet curve start is at the point with parameter t1,
    t1 and t2 are parameters of curves 'curve1' and 'curve2' which correspond to the start point and the end point of the fillet. \n
    Parameter 'sense' determines forward or backward orientation of the fillet curve. \n
    Curve filletCurve is a fillet curve, an arc (when 'surface' == nullptr) or a curve on a cylindric surface 'surface'. \n
    Surface 'surface' is a cylindric surface on which the fillet curve is constructed in general case.
    Use ::AddRefItem(surface) and ::ReleaseItem(surface) methods to manage the surface lifecycle. \n \~
  \param[in] curve1   - \ru Соединяемая кривая 1.
                        \en A curve 1 to be connected. \~
  \param[in,out] t1   - \ru Параметр точки на кривой 1 соединения с кривой соединения.
                        \en A point parameter on curve 1 of connection with fillet curve. \~
  \param[out] w1      - \ru Параметр края на кривой 1.
                        \en The parameter of curve 1 end point. \~
  \param[in] curve2   - \ru Соединяемая кривая 2.
                        \en A curve 2 to be connected. \~
  \param[in,out] t2   - \ru Параметр точки на кривой 2 соединения с кривой соединения.
                        \en A point parameter on curve 2 of connection with fillet curve. \~
  \param[out] w2      - \ru Параметр края на кривой 2.
                        \en The parameter of curve 2 end point. \~
  \param[in,out] radius - \ru Радиус дуги или цилиндра.
                        \en The radius of an arc or a cylinder. \~
  \param[in] sense    - \ru Прямое (true) или обратное (false) направление кривой скругления.
                        \en The forward (true) or the backward (false) direction of the fillet curve. \~
  \param[out] unchanged - \ru Не изменился радиус соединения (true) или изменился (false).
                          \en The fillet radius has not changed (true) or has changed (false). \~
  \param[in] type     - \ru Тип скругления.
                        \en The fillet type. \~
  \param[in] names    - \ru Именователь кривых каркаса.
                        \en An object defining the frame curves names. \~
  \param[out] surface - \ru Поверхность, которая будет создана и на которой базируется соединительная кривая, (может быть возращён nullptr).
                        \en A surface on which the fillet curve is based on, it will be created by the method (can be nullptr). \~
  \param[out] result  - \ru Каркас с построенными кривыми.
                        \en The frame with the constructed curves. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) FilletCurve( const MbCurve3D & curve1, double & t1, double & w1, 
                                      const MbCurve3D & curve2, double & t2, double & w2, 
                                      double & radius, bool sense, bool & unchanged,
                                      const MbeConnectingType type, 
                                      const MbSNameMaker & names,  
                                      MbElementarySurface *& surface, 
                                      MbWireFrame *& result ); 


//------------------------------------------------------------------------------
/** \brief \ru Определить изменение радиуса при перемещении средней точки кривой скругления.
           \en Determine the radius variation while translating the middle point of the fillet curve. \~
  \details \ru Определить изменение радиуса при перемещении средней точки кривой скругления \n
    от центра на расстояние len (с учётом знака len). \n
           \en Determine the radius variation while translating the middle point of the fillet curve \n
    from the centre on distance 'len' (signed). \n \~
  \param[in] filletCurve - \ru Кривая скругления.
                           \en The fillet curve. \~
  \param[in] radius - \ru Радиус скругления.
                      \en The radius of fillet. \~
  \param[in] sense - \ru Направления смещения средней точки кривой скругления.
                     \en A direction of the middle point of the fillet curve translation. \~
  \param[in] len - \ru Величина смещения средней точки кривой скругления.
                   \en A value of translation of the fillet curve middle point. \~
  \param[in] curve1 - \ru Первая сопрягаемая кривая.
                      \en The first curve to fillet. \~
  \param[in] t1 - \ru Параметр начала кривой скругления на первой сопрягаемой кривой.
                  \en The parameter of fillet curve start point on the first curve. \~
  \param[in] curve2 - \ru Вторая сопрягаемая кривая.
                      \en The second curve to fillet. \~
  \param[in] t2 - \ru Параметр конца кривой скругления на второй сопрягаемой кривой.
                  \en The parameter of fillet curve end point on the second curve. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (double) GetFilletRadiusDelta( const MbCurve3D & filletCurve, 
                                         double radius, bool sense, double len,
                                         const MbCurve3D & curve1, double t1,
                                         const MbCurve3D & curve2, double t2 );


//------------------------------------------------------------------------------
/** \brief \ru Определить изменение радиуса при перемещении средней точки кривой скругления.
           \en Determine the radius variation while translating the middle point of the fillet curve. \~
  \details \ru Определить изменение радиуса при перемещении средней точки кривой скругления \n
    от центра на расстояние len (с учётом знака len). \n
           \en Determine the radius variation while translating the middle point of the fillet curve \n
    from the centre on distance 'len' (signed). \n \~
  \param[in] filletCurve - \ru Кривая скругления.
                           \en The fillet curve. \~
  \param[in] radius - \ru Радиус скругления.
                      \en The radius of fillet. \~
  \param[in] sense - \ru Направления смещения средней точки кривой скругления.
                     \en A direction of the middle point of the fillet curve translation. \~
  \param[in] len - \ru Величина смещения средней точки кривой скругления.
                   \en A value of translation of the fillet curve middle point. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (double) GetFilletRadiusDelta( const MbCurve3D & filletCurve, 
                                         double radius, bool sense, double len );


//------------------------------------------------------------------------------
/** \brief \ru Создать кривую для плавного соединения (скругления) всех кривых контура.
           \en Create a curve for fillet of all the curves of a contour. \~
  \details \ru Создать кривую для плавного соединения (скругления) всех кривых контура contour. \n
    type - тип скругления (обычное или на поверхности). \n
    radiuses - радиусы скругления, i-й радиус соответствует стыку i-го и i+1-го сегмента. \n
    Если две кривых в контуре гладко стыкуются, в этом стыке скругление не делается, радиус игнорируется. \n
           \en Create a curve for fillet of all the curves of a contour 'contour'. \n
    'type' is a fillet type (ordinary or on a surface). \n
    'radiuses' are the fillet radii, the i-th radius corresponds to the joint of the i-th and the i+1-th segments. \n
    If two curves in contours are smoothly connected, the fillet is not created at this joint, the radius is ignored. \n \~
  \param[in] contour - \ru Исходный контур.
                       \en The initial contour. \~
  \param[in] radiuses - \ru Множество радиусов скругления.
                        \en An array of fillet radii. \~
  \param[out] result - \ru Контур со скруглениями. Имя сегмента скругления - Hash32SN() имен исходных сегменов.
                       \en The contour with the fillets. The name of the fillet segment - Hash32SN() of initial segments names. \~
  \param[in] type - \ru Тип выполняемых скруглений.
                    \en The type of fillets. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbResultType) CreateContourFillets( const MbContour3D &     contour, 
                                               SArray<double> &        radiuses, 
                                               MbCurve3D *&            result, 
                                               const MbeConnectingType type );


//------------------------------------------------------------------------------
/** \brief \ru Создать кривую или контур на поверхности грани по заданным рёбрам.
           \en Construct a curve or contour on the surface of a face along the specified edges. \~
  \details \ru Строится кривая или контур на поверхности грани по копиям соответствующих двумерных кривых, лежащих в кривых пересечения рёбер. \n
           \en A curve or contour is constructed on the surface of a face based on copies of the corresponding two-dimensional curves lying in the intersection curves of edges. \n \~
  \param[in] face     - \ru Общая для рёбер грань.
                        \en The face common to the edges. \~
  \param[in] edges    - \ru Рёбра.
                        \en The edges. \~
  \param[in] accuracy - \ru Максимальное расстояние точек сегментов в стыке.
                        \en The maximum distance of segment points at the junction. \~
  \return \ru Возвращает кривую или контур на поверхности (nullptr - в случае неудачи).
          \en Returns a curve or contour on a surface (nullptr - in case of failure). \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbCurve3D *) MakeCurveOnSurface( const MbFace & face, 
                                            const std::vector<const MbCurveEdge *> & edges, 
                                                  double accuracy );


//------------------------------------------------------------------------------
/** \brief \ru Создать плавную кривую заданного типа по заданным параметрам.
           \en Construct a fair curve of the specified type according to the specified parameters. \~
  \details \ru Создать плавную кривую заданного типа по заданным параметрам. \n
           \en Construct a fair curve of the specified type according to the specified parameters. \n \~
  \param[in]  initParams - \ru Данные исходной кривой.
                            \en An initial curve data. \~
  \param[in] method       - \ru Метод построения кривой.
                            \en Method of a curve construction. \~
  \param[in] curveData    - \ru Параметры построения кривой.
                            \en Parameters of a curve construction. \~
  \param[out] result      - \ru Результирующая кривая с данными об ошибке аппроксимации.
                            \en The result curve with approximation error data. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC( MbResultType ) FairCurve( const MbFairCurveParams & initParams,
                                     const MbFairCurveMethod & method,
                                     const MbFairCreateData &  curveData,
                                           MbFairCurveResult & result );


//------------------------------------------------------------------------------
/** \brief \ru Создать плавную кривую сопряжения.
           \en Construct a fair fillet curve. \~
  \details \ru Создать плавную кривую сопряжения по заданным параметрам. \n
           \en Construct a fair fillet curve according to the specified parameters. \n \~
  \param[in]  initParams -  \ru Данные исходной кривой.
                            \en An initial curve data. \~
  \param[in] curveData    - \ru Параметры построения кривой.
                            \en Parameters of a curve construction. \~
  \param[out] result      - \ru Результирующая кривая с данными об ошибке аппроксимации.
                            \en The result curve with approximation error data. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC( MbResultType ) FairCurve( const MbFairCurveParams & initParams,
                                     const MbFairFilletData & curveData,
                                           MbFairCurveResult & result );


//------------------------------------------------------------------------------
/** \brief \ru Изменить плавную кривую.
           \en Change a fair curve. \~
  \details \ru Изменить плавную кривую заданным способом. \n
           \en Change a fair curve by the specified method. \n \~
  \param[in]  initParams -  \ru Данные исходной кривой.
                            \en An initial curve data. \~
  \param[in] method       - \ru Метод изменения кривой.
                            \en Method of a curve change. \~
  \param[in] changeData   - \ru Параметры изменения.
                            \en Changing parameters. \~
  \param[out] result      - \ru Результирующая кривая.
                            \en The result curve with approximation error data. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
   \warning \ru В разработке.
           \en Under development. \~
 \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC( MbResultType ) FairCurve( const MbFairCurveParams & initParams,
                                     const MbFairCurveMethod & method,
                                     const MbFairChangeData &  changeData,
                                           c3d::WireFrameSPtr & result );


//------------------------------------------------------------------------------
/** \brief \ru Создание клотоиды.
           \en Creating a clothoid. \~
  \details \ru Создание клотоиды.\n
           \en Creating a clothoid.\n \~
  \param[in] parameters   - \ru Параметры построения кривой.
                            \en Parameters of a curve construction. \~
  \param[in] snMaker      - \ru Именователь кривых каркаса.
                            \en An object defining the frame curves names. \~
  \param[out] result      - \ru Каркас с построенной кривой.
                            \en The frame with the constructed curve. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC( MbResultType ) FairCurve( const MbClothoidParams & parameters,
                                     const MbSNameMaker & snMaker,
                                           c3d::WireFrameSPtr & result );


#endif // __ACTION_SURFACE_CURVE_H
