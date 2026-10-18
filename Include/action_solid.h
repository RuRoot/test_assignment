////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Функции создания тел, операции с телами.
         \en Functions for creation of solids, operations on solids. \~
  \details \ru Процесс построения тел в геометрическом моделировании похож на процесс 
    изготовления моделируемого объекта. Сначала создаются тела простой формы, а далее 
    выполняется набор действий, позволяющих из тел простой формы получить более сложные тела.
    При необходимости создаются вспомогательные объекты. Редактировать и создавать подобные
    тела можно путём изменения параметров с последующим повторением процесса построения тел.\n 
    Все функции создания тел содержат в качестве входного параметр MbSNameMaker, 
    обеспечивающий именование граней, рёбер и вершин. 
    Первым параметром конструктора генератора имён MbSNameMaker служит главное имя операции. 
    По главному имени можно определить, в какой функции рождена та или иная грань, ребро, вершина.
    Главное имя выдаёт метод GetMainName().\n 
           \en The process of solids creation in geometric modeling is similar to the process 
    of the modeled object manufacturing. Firstly solids of a simple form are created, and then 
    a set of operations are performed to obtain a more complex solids from solids of a simple form.
    Auxiliary objects are created if necessary. The similar solids can be edited and created
    by modifying of the parameters and further repeating the process of the solids creation.\n  
    All of the function contain input parameter MbSNameMaker, 
    providing the naming of faces, edges and vertices.
    The first parameter to the constructor MbSNameMaker is the main name of the function.
    You can determine which function is born one or the other face, edge, vertex by main name.
    GetMainName() gives the main name of the function (face.GetMainName(), edge.GetMainName(), vertex.GetMainName()).\~ 
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ACTION_SOLID_H
#define __ACTION_SOLID_H


#include <templ_s_array.h>
#include <mb_cart_point3d.h>
#include <space_item.h>
#include <op_boolean_flags.h>
#include <op_shell_parameter.h>
#include <op_swept_parameter.h>
#include <topology_faceset.h>
#include <mb_operation_result.h>
#include <op_duplication_parameter.h>


class   MbCurve;
class   MbCurve3D;
class   MbSurface;
class   MbCurveEdge;
class   MbFace;
class   MbSolid;
class   MbItem;
class   MbSNameMaker;
class   MbPartSolidIndices;
class   MbSpine;
class   MbMesh;
class   MbGrid;
class   MbCollection;
class   IProgressIndicator;


//------------------------------------------------------------------------------
/** \brief \ru Создать элементарное тело.
           \en Create an elementary solid. \~
  \details \ru Создать одно из элементарных тел по заданным параметрам.\n
            1. Создать одно из элементарных тел по заданным точкам и типу: \n
               solidType = et_Sphere      - шар          (3 точки), \n
               solidType = et_Torus       - тор          (3 точки), \n
               solidType = et_Cylinder    - цилиндр      (3 точки), \n
               solidType = et_Cone        - конус        (3 точки, если конус не усеченный), 
                                                         (4 точки, если конус усеченный), \n
               solidType = et_Block       - блок         (4 точки), \n
               solidType = et_Wedge       - клин         (4 точки), \n
               solidType = et_Prism       - призма       (количество вершин основания+1 точка), \n
               solidType = et_Pyramid     - пирамида     (количество вершин основания+1 точка), \n
               solidType = et_Plate       - плита        (4 точки),
               solidType = et_Icosahedron - икосаэдр     (3 точки), \n
               solidType = et_Polyhedron  - многогранник (3 точки), \n
               solidType = et_Tetrapipe   - тетра-труба  (3 точки), \n
               solidType = et_Octapipe    - окта-труба   (3 точки). \n
            2. Создать тело по элементарной поверхности. \n
               Допускается только тип поверхности - цилиндр, конус, сфера, тор. \n
           \en Create one of the elementary solids according to the specified parameters.\n
            1. Create one of elementary solids from the specified points and type: \n
               solidType = et_Sphere      - a sphere       (3 points), \n
               solidType = et_Torus       - a torus        (3 points), \n
               solidType = et_Cylinder    - a cylinder     (3 points), \n
               solidType = et_Cone        - a cone         (3 points), in the case of non-frustum cone,
                                                           (4 points), in the case of frustum cone, \n
               solidType = et_Block       - a block        (4 points), \n
               solidType = et_Wedge       - a wedge        (4 points), \n
               solidType = et_Prism       - a prism        (points count is equal to the base vertices count + 1), \n
               solidType = et_Pyramid     - a pyramid      (points count is equal to the base vertices count + 1), \n
               solidType = et_Plate       - a plate        (4 points), \n
               solidType = et_Icosahedron - an icosahedron (3 points), \n
               solidType = et_Polyhedron  - a polyhedron   (3 points), \n
               solidType = et_Tetrapipe   - a tetra-pipe   (3 points), \n
               solidType = et_Octapipe    - an octa-pipe   (3 points). \n \~
            2. Create a solid from an elementary surface. \n
               The only acceptable surface types are cylinder, cone, sphere, torus. \~
  \param[in]  params - \ru Параметры операции.
                       \en Parameters of operation. ~
  \param[out] results - \ru Результаты операции.
                       \en The operation result. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) ElementarySolid( const MbElementarySolidParams &  params,
                                                MbElementarySolidResults & results );


//------------------------------------------------------------------------------
/** \brief \ru Создать тело на основе полигональной модели.
           \en Create a solid on the basis of a polygonal geometric object. \~
  \details \ru Создать тело #MbSolid на основе полигональной модели #MbMesh. \n
           \en Create a solid #MbSolid on the basis of a polygonal geometric object #MbMesh. \n \~
  \param[in] mesh -     \ru Полигональная модель.
                        \en The polygonal geometric object. \~
  \param[in] params -   \ru Параметры операции (в том числе индикатор прогресса).
                        \en The operation parameters (including a progress indicator). \~
  \param[out] result -  \ru Построенное тело.
                        \en The resultant solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC( MbResultType ) MeshSolid( const MbMesh &             mesh,
                                     const MbGridShellParams &  params,
                                           c3d::SolidSPtr &     result );


//------------------------------------------------------------------------------
/** \brief \ru Создать тело на основе триангуляции.
           \en Create a solid on the basis of a triangulation. \~
  \details \ru Создать тело #MbSolid на основе триангуляции #MbGrid. \n
           \en Create a solid #MbSolid on the basis of a triangulation #MbGrid. \n \~
  \param[in] grid   - \ru Полигональная модель.
                      \en The polygonal geometric object. \~
  \param[in] params - \ru Параметры операции (в том числе индикатор прогресса).
                      \en The operation parameters (including a progress indicator). \~
  \param[out] result - \ru Построенное тело.
                       \en The resultant solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC( MbResultType ) GridSolid( const MbGrid &             grid,
                                     const MbGridShellParams &  params,
                                           c3d::SolidSPtr &     result );


//------------------------------------------------------------------------------
/** \brief \ru Создать тело на основе коллекции элементов.
           \en Create a solid on the basis of collection of elements. \~
  \details \ru Создать тело #MbSolid на основе коллекции элементов #MbCollection. \n
           \en Create a solid #MbSolid on the basis of collection of elements #MbCollection. \n \~
  \param[in] collection - \ru Коллекция элементов.
                          \en The elements collection. \~
  \param[in] params - \ru Параметры операции.
                      \en Operation parameters. \~
  \param[out] result - \ru Построенное тело.
                       \en The resultant solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC( MbResultType ) CollectionSolid( const MbCollection &       collection,
                                           const MbGridShellParams &  params,
                                                 c3d::SolidSPtr &     result );


//------------------------------------------------------------------------------
/** \brief \ru Создать тело c заданной оболочкой.
           \en Create a solid with a given shell. \~
  \details \ru Создать тело без истории построения с заданной оболочкой. \n
           \en Create a solid with a given shell without a history. \n \~
  \param[in] shell - \ru Оболочка.
                     \en A shell. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \return \ru Возвращает тело без истории.
          \en Returns a solid without the history. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbSolid *) CreateSolid(       MbFaceShell &  shell, 
                                   const MbSNameMaker & names );


//------------------------------------------------------------------------------
/** \brief \ru Рассчитать глубину выдавливания или угол вращения.
           \en Compute the extrusion depth or the rotation angle. \~
  \details \ru Рассчитать value - глубину выдавливания или угол вращения (0.0 : M_PI2) 
    для последующего построения тела путем выдавливания или вращения образующей кривой. \n
           \en Compute 'value' - the extrusion depth or the rotation angle (0.0 : M_PI2) 
    for the further construction of a solid by extrusion or revolution of the generating curve. \n \~  
  \param[in] sweptData - \ru Данные об образующей.
                         \en The generating curve data. \~
  \param[in] axis - \ru Ось вращения.
                    \en Rotation axis. \~
  \param[in] direction - \ru Направление выдавливания.
                         \en An extrusion direction. \~
  \param[in] rotation - \ru Вращение или выдавливание.
                        \en Rotation or extrusion. \~
  \param[in] operationDirection - \ru Вперед\назад.
                                  \en Forward or backward direction. \~
  \param[in] point - \ru Точка, до которой требуется вращать или выдавливать поверхность.
                     \en The point to rotate or extrude the surface up to. \~
  \param[out] value - \ru Глубина выдавливания или угол вращения.
                      \en The extrusion depth or the rotation angle. \~
  \return \ru Возвращает true, если расчет выполнен успешно.
          \en Returns true if the value has been successfully calculated. \~
  \warning \ru Вспомогательная функция операций ExtrusionSolid, RevolutionSolid, ExtrusionResult и RevolutionResult.
           \en An auxiliary function of operations ExtrusionSolid, RevolutionSolid, ExtrusionResult and RevolutionResult. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (bool) GetSweptValue( const MbSweptData &   sweptData,
                                const MbAxis3D &      axis,
                                const MbVector3D &    direction,
                                const bool            rotation,
                                const bool            operationDirection,
                                const MbCartPoint3D & point,
                                      double &        value );


//------------------------------------------------------------------------------
/** \brief \ru Получить начальное приближение для нахождения образа при вращении/выдавливании.
           \en Get the initial approximation for image calculation while rotating/extruding. \~
  \details \ru Вычислить положение образа точки образующей кривой на поверхности для последующего 
    построения тела путем выдавливания или вращения образующей кривой до заданной поверхности. \n
           \en Compute the position of a generating curve point image on a surface for further 
    solid construction by extrusion or revolution of the generating curve up to the specified surface. \n \~
  \param[in] generatrix - \ru Кривая.
                          \en The curve. \~
  \param[in] surface - \ru Поверхность, до которой строим операцию.
                       \en The surface to construct up to. \~
  \param[in] direction - \ru Направление выдавливания.
                         \en An extrusion direction. \~
  \param[in] axis - \ru Ось вращения.
                    \en Rotation axis. \~
  \param[in] rotation - \ru Вращение (true) или выдавливание (false)
                        \en Rotation (true) or extrusion (false) \~
  \param[out] imagePosition - \ru Точка образа на поверхности.
                              \en The image point on the surface. \~
  \param[out] resType - \ru Код результата операции.
                        \en Operation result code. \~
  \warning \ru Вспомогательная функция операций ExtrusionSolid, RevolutionSolid, ExtrusionResult и RevolutionResult.
           \en An auxiliary function of operations ExtrusionSolid, RevolutionSolid, ExtrusionResult and RevolutionResult. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (void) GetSweptImagePosition( const MbCurve3D &    generatrix, 
                                        const MbSurface &    surface, 
                                        const MbVector3D &   direction,
                                        const MbAxis3D &     axis,
                                        const bool           rotation,
                                              MbCartPoint &  imagePosition,
                                              MbResultType & resType );


//------------------------------------------------------------------------------
/** \brief \ru Найти расстояния/углы от образующей до поверхности при вращении/выдавливании.
           \en Calculate the distances/angles from generating curve to the surface while rotating/extruding. \~
  \details \ru Вычислить глубины выдавливания в прямом и обратном направлениях или углы вращения 
    в прямом и обратном направлениях для последующего построения тела путем выдавливания или вращения 
    образующей кривой до заданной поверхности, а также и габарит образа кривой. \n
           \en Calculate the extrusion depths in forward and backward directions or the rotating angles 
    in forward and backward directions for further solid construction by extrusion or revolution 
    of the generating curve up to the specified surface; and also calculate the bounding box of the curve image. \n \~
  \param[in] surface - \ru Поверхность, до которой строим операцию.
                       \en The surface to construct up to. \~
  \param[in] curve - \ru Образующая кривая.
                     \en The generating curve. \~
  \param[in] direction - \ru Направление выдавливания.
                         \en An extrusion direction. \~
  \param[in] axis - \ru Ось вращения.
                    \en Rotation axis. \~
  \param[in] rotation - \ru Вращение (true) или выдавливание (false).
                        \en Revolution (true) or extrusion (false). \~
  \param[in] operationDirection - \ru Направление движения: вперед (true) или назад (false).
                                  \en The motion direction: forward (true) or backward (false). \~
  \param[out] imagePosition - \ru Точка на части поверхности, в которой лежит образ.
                              \en A point on a surface part that contains the image. \~
  \param[out] range - \ru Расстояния до поверхности в обратном и прямом направлениях.
                      \en The distance to surface in the backward and the forward directions. \~
  \param[out] rectOnSurface - \ru Габарит образа на поверхности.
                              \en The bounding box of image on the surface. \~
  \param[out] resType - \ru Код результата операции.
                        \en Operation result code. \~
  \warning \ru Вспомогательная функция операций ExtrusionSolid, RevolutionSolid, ExtrusionResult и RevolutionResult.
           \en An auxiliary function of operations ExtrusionSolid, RevolutionSolid, ExtrusionResult and RevolutionResult. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (void) GetRangeToSurface( const MbSurface &    surface,
                                    const MbCurve3D &    curve,
                                    const MbVector3D &   direction,
                                    const MbAxis3D &     axis,
                                    const bool           rotation,
                                    const bool           operationDirection,
                                    const MbCartPoint &  imagePosition,
                                          double         range[2],
                                          MbRect &       rectOnSurface,
                                          MbResultType & resType );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить площадь проекции кривой на виртуальную координатную плоскость.
           \en Compute the area of a curve projection onto a virtual coordinate plane. \~
  \details \ru Вычислить площадь проекции кривой на виртуальную координатную плоскость \n
    для определения ориентации образующей кривой в оболочке выдавливания и вращения. \n
    Параметры direction и axis определяют направление выдавливания и ось вращения,
    а параметр rotation определяет тип операции: \n
    - выдавливание, вычисляется площадь проекции на плоскость XOY, \n
    - вращение, вычисляется площадь "проекции" на "плоскость" ROZ.
    Для незамкнутой кривой в расчет добавляется "замыкание отрезком".
           \en Compute the area of a curve projection onto a virtual coordinate plane \n
    to determine the generating curve orientation in the shell of extrusion and revolution. \n
    Parameters 'direction' and 'axis' determine the extrusion direction and the rotation axis,
    and parameter 'rotation' determines the operation type: \n
    - extrusion, the area of projection onto the plane XOY is computed, \n
    - revolution, the area of "projection" to the "plane" ROZ is computed.
    For an open curve "enclosure by a segment" is considered. \~
  \param[in] curve - \ru Кривая.
                     \en The curve. \~
  \param[in] axis - \ru Ось вращения.
                    \en Rotation axis. \~
  \param[in] direction - \ru Направление выдавливания.
                         \en An extrusion direction. \~
  \param[in] rotation - \ru Вращение (true) или выдавливание (false).
                        \en Revolution (true) or extrusion (false). \~
  \return \ru Возвращает площадь проекции.
          \en Returns the projection area. \~
  \warning \ru Вспомогательная функция операций ExtrusionSolid, RevolutionSolid, ExtrusionResult и RevolutionResult.
           \en An auxiliary function of operations ExtrusionSolid, RevolutionSolid, ExtrusionResult and RevolutionResult. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (double) AreaSign( const MbCurve3D &  curve, 
                             const MbAxis3D &   axis, 
                             const MbVector3D & direction, 
                                   bool         rotation );


//------------------------------------------------------------------------------
/** \brief \ru Определить ориентацию секущей грани относительно тела выдавливания/вращения.
           \en Determine the orientation of a cutting surface relative to the extrusion/revolution solid. \~
  \details \ru Определить ориентацию секущей поверхности относительно тела, которое будет строиться 
    путём выдавливания или вращения образующей кривой до заданной поверхности. \n
           \en Determine the orientation of a cutting surface relative to the solid which is to be constructed 
    by extrusion or revolution the generation curve up to the specified surface. \n \~
  \param[in] cuttingSurface - \ru Поверхность для анализа.
                              \en A surface to analyze. \~
  \param[in] imagePosition - \ru Место образа на поверхности.
                             \en An image location on the surface. \~
  \param[in] curve - \ru Образующая кривая.
                     \en The generating curve. \~
  \param[in] direction - \ru Направление выдавливания.
                         \en An extrusion direction. \~
  \param[in] axis - \ru Ось вращения.
                    \en Rotation axis. \~
  \param[in] rotation - \ru Вращение (true) или выдавливание (false).
                        \en Revolution (true) or extrusion (false). \~
  \param[in] operationDirection - \ru Направление движения: вперед (true) или назад (false).
                                  \en The motion direction: forward (true) or backward (false). \~
  \param[out] relativeSense - \ru Ориентация поверхности по отношению к операции.
                              \en The surface orientation relative to the operation. \~
  \param[out] resType - \ru Код результата операции.
                        \en Operation result code. \~
  \warning \ru Вспомогательная функция операций ExtrusionSolid, RevolutionSolid, ExtrusionResult и RevolutionResult.
           \en An auxiliary function of operations ExtrusionSolid, RevolutionSolid, ExtrusionResult and RevolutionResult. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (void) AnalyzeSurfaceRelationToSweptOperation( const MbSurface &   cuttingSurface,
                                                         const MbCartPoint & imagePosition,
                                                         const MbCurve3D &   curve,
                                                         const MbVector3D &  direction,
                                                         const MbAxis3D &    axis,
                                                         const bool          rotation,
                                                               bool          operationDirection,
                                                               bool &        relativeSense,
                                                               MbResultType& resType );


//------------------------------------------------------------------------------
/** \brief \ru Найти сегменты контура на поверхности, соответствующие швам и полюсам.
           \en Find contour segments corresponding to the seams and poles. \~
  \details \ru Найти сегменты контура на поверхности, соответствующие швам и полюсам. \n
           \en Find contour segments corresponding to the seams and poles. \n \~
  \param[in] surface - \ru Поверхность.
                       \en Surface. \~
  \param[in] contour - \ru Контур.
                       \en Contour. \~
  \param[out] seamsAndPoles - \ru Номера сегментов, соответствующих швам и полюсам.
                              \en Segment numbers corresponding to the seams and poles. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (void) FindPolesAndSeamsInContour( const MbSurface &    surface, 
                                             const MbContour &    contour,
                                             c3d::IndicesVector & seamsAndPoles );


//------------------------------------------------------------------------------
/** \brief \ru Создать тело выдавливания.
           \en Create an extrusion solid. \~
  \details \ru Создать тело выдавливания. \n
    solid1 и solid2 используются с опцией "До ближайших граней" этих тел. \n
           \en Create an extrusion solid. \n
    solid1 and solid2 are used with option "Up to the closest faces" of these solids. \n \~
  \param[in]      sweptData         - \ru Данные об образующей кривой.
                                      \en The generating curve data. \~
  \param[in]      direction         - \ru Направление выдавливания.
                                      \en An extrusion direction. \~
  \param[in]      solid1            - \ru До ближайших граней этого тела в прямом направлении.
                                      \en Up to the closest faces of this solid in the forward direction. \~
  \param[in]      solid2            - \ru До ближайших граней этого тела в обратном направлении.
                                      \en Up to the closest faces of this solid in the backward direction. \~
  \param[in]      checkIntersection - \ru Объединять тела solid1 и solid2 с проверкой пересечения.
                                      \en Whether to union the solids solid1 and solid2 with the check for intersections. \~
  \param[in, out] params            - \ru Параметры выдавливания.
    Возвращают информацию для построения элементов массива операций до поверхности.
                                      \en The extrusion parameters.
    Returns the information for construction of the up-to-surface operation array elements. \~
  \param[in]      operNames         - \ru Именователь операции.
                                      \en An object defining names generation in the operation. \~
  \param[in]      contoursNames     - \ru Именователи сегментов образующего контура.
                                      \en An objects defining a names of the generating contour segments. \~
  \param[out]     result            - \ru Построенная оболочка (тело).
                                      \en The resultant shell (solid). \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) ExtrusionSolid( const MbSweptData &           sweptData, 
                                         const MbVector3D &            direction,
                                         const MbSolid *               solid1,
                                         const MbSolid *               solid2, 
                                               bool                    checkIntersection,
                                         const ExtrusionValues &       params, 
                                         const MbSNameMaker &          operNames, 
                                         const RPArray<MbSNameMaker> & contoursNames,
                                               MbSolid *&              result );


//------------------------------------------------------------------------------
/** \brief \ru Создать тело вращения.
           \en Create a solid of revolution. \~
  \details \ru Создать тело вращения по данным об образующей. \n
           \en Create a solid of revolution by the generating curve data. \n \~
  \param[in]      sweptData     - \ru Данные об образующей кривой.
                                  \en The generating curve data. \~
  \param[in]      axis          - \ru Ось вращения.
                                  \en Rotation axis. \~
  \param[in, out] params        - \ru Параметры вращения.
    Возвращают информацию для построения элементов массива операций до поверхности. \n
    При построении операции вращения до конструктивной плоскости с вырожденной областью определения
    оболочка будет построена до ближайшего пересечения с указанной плоскостью.
    В случае, если плоскость имеет не вырожденные границы, часть плоскости, до которой будет построено вращение, 
    определяется по положению центра тяжести контура, ограничивающего плоскость, относительно проекции оси вращения на плоскость.
                                  \en The revolution parameters.
    Returns the information for construction of the up-to-surface operation array elements. \n
    When constructing a rotation operation to a constructive plane with a degenerate domain of definition
    the shell will be constructed to the nearest intersection with the specified plane.
    If the plane has non-degenerate boundaries, the part of the plane up to which the rotation will be constructed is
    is selected according to the position of the center of gravity of the contour limiting the plane relative to the 
    projection of the axis of rotation onto the plane. \~
  \param[in]      operNames     - \ru Именователь операции.
                                  \en An object defining names generation in the operation. \~
  \param[in]      contoursNames - \ru Именователи сегментов образующего контура.
                                  \en An objects defining a names of the generating contour segments. \~
  \param[out]     result        - \ru Построенная оболочка (тело).
                                  \en The resultant shell (solid). \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) RevolutionSolid( const MbSweptData &           sweptData,
                                          const MbAxis3D &              axis,
                                          const RevolutionValues &      params, 
                                          const MbSNameMaker &          operNames, 
                                          const RPArray<MbSNameMaker> & contoursNames,
                                                MbSolid *&              result );


//------------------------------------------------------------------------------
/** \brief \ru Создать кинематическое тело.
           \en Create a sweeping solid. \~
  \details \ru Создать кинематическое тело путем движения образующей кривой вдоль направляющей кривой c дополнительной информацией. \n
           \en Create a sweeping solid by moving the generating curve along the guide curve with additional data. \n \~
  \param[in] evolShellParams  - \ru Исходные данные для выполнения операции.
                                \en The source data to make the operation. \~
  \param[out] result - \ru Построенная оболочка (тело).
                       \en The resultant shell (solid). \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) EvolutionShell( const MbEvolutionShellParams & evolShellParams,
                                               c3d::SolidSPtr &         result );


//------------------------------------------------------------------------------
/** \brief \ru Создать тело по пространственным сечениям.
           \en Create a solid from a space sections. \~
  \details \ru Создать тело по пространственным сечениям c направляющей линией. \n
           \en Create a solid from a space sections with a guide curve. \n \~
  \param[in] loftSolParams  - \ru Исходные данные для выполнения операции.
                              \en The source data to make the operation. \~
  \param[out] result - \ru Построенная оболочка (тело).
                       \en The resultant shell (solid). \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC( MbResultType ) LoftedSolid( const MbLoftedSolidParams & loftSolParams,
                                             c3d::SolidSPtr &      result );


//------------------------------------------------------------------------------
/** \brief \ru Создать тело выдавливания и выполнить булеву операцию.
           \en Create an extrusion solid and perform a boolean operation. \~
  \details \ru Создать тело выдавливания и выполнить булеву операцию типа oType с телом solid. 
    Принимаемые значения OperationType для тел: \n
    bo_Union      - объединение, \n
    bo_Intersect  - пересечение, \n
    bo_Difference - вычитание.
           \en Create an extrusion solid and perform a boolean operation of type 'oType' with solid 'solid'. 
    The possible values of 'OperationType' for solids: \n
    bo_Union      - union, \n
    bo_Intersect  - intersection, \n
    bo_Difference - subtraction. \~
  \param[in]       solid         - \ru Первое тело для булевой операции.
                                   \en The first solid for a boolean operation. \~
  \param[in]       sameShell     - \ru Режим копирования тела.
                                   \en Whether to copy the solid. \~
  \param[in]       sweptData     - \ru Данные об образующих кривых.
                                   \en The generating curve data. \~
  \param[in]       direction     - \ru Направление выдавливания.
                                   \en An extrusion direction. \~
  \param[in, out]  params        - \ru Параметры выдавливания.
    Возвращают информацию для построения элементов массива операций до поверхности.
                                   \en The extrusion parameters.
    Returns the information for construction of the up-to-surface operation array elements. \~
  \param[in]       oType         - \ru Тип булевой операции.
                                   \en A boolean operation type. \~
  \param[in]       operNames     - \ru Именователь операции.
                                   \en An object defining names generation in the operation. \~
  \param[in]       contoursNames - \ru Именователи образующих кривых.
                                   \en The objects defining the names of generating lines. \~
  \param[out]      result        - \ru Построенное тело.
                                   \en The resultant solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) ExtrusionResult(       MbSolid &               solid, 
                                                MbeCopyMode             sameShell, 
                                          const MbSweptData &           sweptData,
                                          const MbVector3D &            direction,
                                          const ExtrusionValues &       params, 
                                                OperationType           oType, 
                                          const MbSNameMaker &          operNames, 
                                          const RPArray<MbSNameMaker> & contoursNames,
                                                MbSolid *&              result );


//------------------------------------------------------------------------------
/** \brief \ru Создать тело вращения и выполнить булеву операцию.
           \en Create a revolution solid and perform a boolean operation. \~
  \details \ru Создать тело вращения и выполнить булеву операцию типа oType с телом solid. 
    Принимаемые значения OperationType для тел: \n
    bo_Union      - объединение, \n
    bo_Intersect  - пересечение, \n
    bo_Difference - вычитание.
           \en Create a revolution solid and perform a boolean operation of type oType with solid 'solid'. 
    The possible values of 'OperationType' for solids: \n
    bo_Union      - union, \n
    bo_Intersect  - intersection, \n
    bo_Difference - subtraction. \~
  \param[in]       solid         - \ru Первое тело для булевой операции.
                                   \en The first solid for a boolean operation. \~
  \param[in]       sameShell     - \ru Режим копирования тела.
                                   \en Whether to copy the solid. \~
  \param[in]       sweptData     - \ru Данные об образующих кривых.
                                   \en The generating curve data. \~
  \param[in]       axis          - \ru Ось вращения.
                                   \en Rotation axis. \~
  \param[in, out]  params        - \ru Параметры вращения.
    Возвращают информацию для построения элементов массива операций до поверхности.
                                   \en The revolution parameters.
    Returns the information for construction of the up-to-surface operation array elements. \~
  \param[in]       oType         - \ru Тип булевой операции.
                                   \en A boolean operation type. \~
  \param[in]       operNames     - \ru Именователь операции.
                                   \en An object defining names generation in the operation. \~
  \param[in]       contoursNames - \ru Именователи образующих кривых.
                                   \en The objects defining the names of generating lines. \~
  \param[out]      result        - \ru Построенное тело.
                                   \en The resultant solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) RevolutionResult(       MbSolid &               solid, 
                                                 MbeCopyMode             sameShell,
                                           const MbSweptData &           sweptData,
                                           const MbAxis3D &              axis,
                                           const RevolutionValues &      params, 
                                                 OperationType           oType, 
                                           const MbSNameMaker &          operNames, 
                                           const RPArray<MbSNameMaker> & contoursNames,
                                                 MbSolid *&              result );


//------------------------------------------------------------------------------
/** \brief \ru Сориентировать образующий контур и направляющую кинематики.
           \en Determine the orientation for a generating contour and for a guide curve of kinematics (evolution). \~
  \details \ru Выполнить ориентацию образующего контура и направляющей кривой для построения кинематического тела. \n
           \en Orientate the generating contour and the guide curve for a sweeping solid construction. \n \~
  \param[in] surface - \ru Поверхность.
                       \en The surface. \~
  \param[in] contours - \ru Образующие контуры.
                        \en Generating contours. \~
  \param[in] guide - \ru Направляющая кривая.
                     \en The spine curve. \~
  \param[in] parameters - \ru Параметры операции.
                          \en The operation parameters. \~
  \param[out] axis - \ru Ось доворота образующей.
                     \en The axis for the generating curve additional turn. \~
  \param[out] angle - \ru Угол доворота образующей.
                      \en The additional turn for generating line. \~
  \param[in] version - \ru Версия операции.
                       \en The version of the operation. \~
  \param[out] matrix - \ru Указатель на матрицу преобразование сечения к ближайшему концу направляющей кривой.
                       \en The pointer to matrix, which transform section to nearest end of spine curve. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \warning \ru Вспомогательная функция операций EvolutionSolid и EvolutionResult.
           \en An auxiliary function of operations EvolutionSolid and EvolutionResult. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbResultType) EvolutionNormalize( const MbSurface          & surface,
                                             const RPArray<MbContour> & contours,
                                             const MbCurve3D          & guide,
                                             const EvolutionValues    & parameters,
                                                   MbAxis3D           & axis,
                                                   double             & angle,
                                                   VERSION              version,
                                                   MbMatrix3D         * matrix = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Создать усеченную замкнутую кривую на копии кривой.
           \en Create a trimmed closed curve on a curve copy. \~
  \details \ru Выполнить построение копии замкнутой кривой с началом в точке, определяемой параметром t. \n
           \en Create a copy of a closed curve starting at a point with parameter t. \n \~
  \param[in] curve - \ru Направляющая кривая.
                     \en The spine curve. \~
  \param[in] t - \ru Параметр кривой.
                 \en A curve parameter. \~
  \return \ru При удачной работе функция возвращает построенную копию кривой 
    с началом в заданной точке, в противном случае функция возвращает ноль.
          \en Returns a constructed curve copy starting at the specified point if it has been successfully created, 
    otherwise it returns null. \~
  \warning \ru Вспомогательная функция операций EvolutionSolid и EvolutionResult.
           \en An auxiliary function of operations EvolutionSolid and EvolutionResult. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbCurve3D *) TrimClosedSpine( const MbCurve3D & curve, 
                                               double      t );


//------------------------------------------------------------------------------
/** \brief \ru Создать кинематическое тело и выполнить булеву операцию.
           \en Create an evolution solid and perform a boolean operation. \~
  \details \ru Создать кинематическое тело и выполнить булеву операцию с телом srcSolid.
    Поддерживаемые типы булевой операции: \n
      - bo_Union      - объединение, \n
      - bo_Intersect  - пересечение, \n
      - bo_Difference - вычитание.
           \en Create an evolution solid and perform a boolean operation with "srcSolid" solid.
    Following types of Boolean operation are supported: \n
      - bo_Union      - union, \n
      - bo_Intersect  - intersection, \n
      - bo_Difference - subtraction. \~
  \param[in] srcSolid - \ru Первое тело для булевой операции.
                        \en The first solid for a boolean operation. \~
  \param[in] copyMode - \ru Режим копирования тела.
                        \en Whether to copy the solid. \~
  \param[in] oType    - \ru Тип булевой операции.
                        \en A boolean operation type. \~
  \param[in] evShellParams - \ru Исходные данные для создания кинематического тела.
                             \en The source data for making evolution solid. \~
  \param[out] result  - \ru Построенное тело.
                        \en The resultant solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC( MbResultType ) EvolutionResult( const c3d::SolidSPtr &         srcSolid,
                                           const MbeCopyMode              copyMode,
                                                 OperationType            oType,
                                           const MbEvolutionShellParams & evShellParams,
                                                 c3d::SolidSPtr &         result );


//------------------------------------------------------------------------------
/** \brief \ru Создать тело по плоским сечениям и выполнить булеву операцию.
           \en Create a solid from the planar sections and perform a boolean operation. \~
  \details \ru Создать тело по плоским сечениям и выполнить булеву операцию с телом solid. 
    Поддерживаемые типы Булевой операции: \n
      - bo_Union      - объединение, \n
      - bo_Intersect  - пересечение, \n
      - bo_Difference - вычитание.
           \en The source data to create a solid from the planar sections and after that to perform a boolean operation.
    Following types of Boolean operation are supported: \n
      - bo_Union      - union, \n
      - bo_Intersect  - intersection, \n
      - bo_Difference - subtraction. \~
  \param[in] srcSolid - \ru Первое тело для булевой операции.
                        \en The first solid for a boolean operation. \~
  \param[in] copyMode - \ru Режим копирования тела.
                        \en Whether to copy the solid. \~
  \param[in] loftSolParams - \ru Исходные данные для выполнения операции.
                             \en The source data to make the operation. \~
  \param[out] result  - \ru Построенное тело.
                        \en The resultant solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC( MbResultType ) LoftedResult( const c3d::SolidSPtr &      srcSolid,
                                        const MbeCopyMode           copyMode,
                                        const MbLoftedSolidParams & loftSolParams,
                                              c3d::SolidSPtr &      result );


//------------------------------------------------------------------------------
/** \brief \ru Выполнить булеву операцию.
           \en Perform a Boolean operation. \~
  \details \ru Функция выполняет указанную булеву операцию над двумя телами с возможностью управления слиянием граней и рёбер.\n
           \en The function performs the specified Boolean operation on two solids with faces and edges merging control.\n \~
  \param[in]  solid1         - \ru Набор граней первого тела.
                               \en The set of faces of the first solid. \~
  \param[in]  sameShell1     - \ru Способ копирования граней первого тела.
                               \en Method of copying the faces of the first solid. \~
  \param[in]  solid2         - \ru Набор граней второго тела.
                               \en The second solid face set. \~
  \param[in]  sameShell2     - \ru Способ копирования граней второго тела.
                               \en Method of copying the faces of the second solid. \~
  \param[in]  params         - \ru Параметры булевой операции.
                               \en A Boolean operation parameters. \~
  \param[out] result         - \ru Построенный набор граней.
                               \en Constructed set of faces. \~  
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/ // ---
MATH_FUNC (MbResultType) BooleanResult( const c3d::SolidSPtr &           solid1,
                                              MbeCopyMode                sameShell1,
                                        const c3d::SolidSPtr &           solid2,
                                              MbeCopyMode                sameShell2,
                                        const MbBooleanOperationParams & params,
                                              c3d::SolidSPtr &           result );


//------------------------------------------------------------------------------
/** \brief \ru Разрезать тело на части.
           \en Cut a solid into parts. \~
  \details \ru Разрезать тело на части. \n
           \en Cut a solid into parts. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in]  sameShell - \ru Режим копирования исходного тела.
                          \en The mode of copying of the source solid. \~
  \param[in]  cuttingParams - \ru Параметры операции.
                              \en Operation parameters. \~
  \param[out] results - \ru Построенные тела.
                       \en The resultant solids. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/ // ---
MATH_FUNC (MbResultType) SolidCutting( const c3d::SolidSPtr &        solid, 
                                             MbeCopyMode             sameShell,
                                       const MbShellCuttingParams &  cuttingParams,
                                             c3d::SolidsSPtrVector & results );


//------------------------------------------------------------------------------
/** \brief \ru Создать симметричное тело относительно плоскости.
           \en Create a symmetric solid relative to a plane. \~
  \details \ru Создать симметричное тело относительно плоскости XY локальной системы координат. \n
    Если symParams._side = bo_Difference, то функция создаёт симметричное тело с заданной плоскостью симметрии следующим образом. 
    Исходное тело режется плоскостью XY локальной системы координат, берётся часть исходного тела, 
    расположенная сверху режущей плоскости, строится зеркальная копия выбранной части исходного тела
    и объединяется с выбранной частью исходного тела.
    Если symParams._side = bo_Unknown и sameShell = cm_Copy, то функция создает зеркальную копию тела относительно плоскости XY локальной системы координат \n
           \en Create a symmetric solid relative to XY-plane of a local coordinate system. \n
    If symParams._side = bo_Difference, then the function creates a symmetric solid with the specified plane of symmetry in the following way. 
    The source solid is cut off by the plane XY of the local coordinate system; a part of the source solid above the cutting plane 
    is retained. A mirror copy of the chosen part is created
    and then is united with the chosen part of the source solid.
    If symParams._side = bo_Unknown and sameShell = cm_Copy, then the function create a mirror copy of a solid relative to the XY-plane of a local coordinate system \n \~
  \param[in] solid - \ru Исходная оболочка.
                     \en The source shell. \~
  \param[in] sameShell - \ru Режим копирования оболочки.
                         \en Whether to copy the shell. \~
  \param[in] symParams - \ru Список параметров.
                         \en Parameters list. \~
  \param[out] result - \ru Построенная оболочка (тело).
                       \en The resultant shell (solid). \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/ // ---
MATH_FUNC (MbResultType) SymmetrySolid(       MbSolid &               solid, 
                                              MbeCopyMode             sameShell,
                                        const MbSymmetrySolidParams & symParams,
                                              c3d::SolidSPtr &        result );


//------------------------------------------------------------------------------
/** \brief \ru Создать тело с ребром жёсткости.
           \en Create a solid with a rib. \~
  \details \ru Создать тело с ребром жёсткости. \n
    По заданному контуру функция строит ребро жёсткости и объединяет его с исходным телом. 
    Сегмент контура с указанным номером устанавливает вектор уклона. \n
           \en Create a solid with a rib. \n
    The function creates a rib from a given contour and unites it with the source solid. 
    The segment of the contour with the given number determines the slope vector. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Режим копирования входного тела.
                         \en Whether to copy the input solid. \~
  \param[in] pars - \ru Параметры ребра жёсткости.
                    \en Parameters of a rib. \~
  \param[out] result - \ru Построенное тело.
                       \en The resultant solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/ // ---
MATH_FUNC (MbResultType) RibSolid(       MbSolid              & solid,
                                         MbeCopyMode            sameShell,
                                   const MbRibSolidParameters & pars,
                                         c3d::SolidSPtr       & result );


//------------------------------------------------------------------------------
/** \brief \ru Создать отдельное ребро жёсткости.
           \en Create a separate rib. \~
  \details \ru Создать отдельное ребро жёсткости для исходного тела без приклеивания. \n
           \en Create a separate rib for source solid without gluing. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] pars - \ru Параметры ребра жёсткости.
                    \en Parameters of a rib. \~
  \param[out] result - \ru Построенное тело.
                       \en The resultant solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/ // ---
MATH_FUNC (MbResultType) RibElement( const MbSolid              & solid, 
                                     const MbRibSolidParameters & pars,
                                           c3d::SolidSPtr       & result );


//------------------------------------------------------------------------------
/** \brief \ru Скруглить ребра тела.
           \en Fillet edges of a solid. \~
  \details \ru Скруглить указанные рёбра тела. \n
            Функция выполняет замену указанных рёбер исходного тела гранями,
            гладко сопрягающими смежные грани указанных рёбер. В поперечном сечении
            сопрягающие грани могут иметь форму дуги окружности, эллипса, гиперболы, параболы. \n
            Способы скругления ребер тела: \n
            - Скруглить указанные рёбра тела постоянным радиусом. \n
            - Скруглить вершины и примыкающие к ней рёбра тела постоянным радиусом. 
              В вершинах должно стыковаться три ребра. \n
            - Скруглить указанные ребра тела переменным радиусом, задаваемым MbEdgeFunction.function. 
              Параметры поперечного сечения могут изменяться по заданному закону. \n
            Способ построения скругления определяется набором параметров в MbShellChamferParams. \n
           \en Fillet the specified edges of the solid. \n
            The function performs the replacement of the specified edges of the source solid by faces
            smoothly connecting the adjacent faces of the specified edges. The cross-section
            of the connecting faces can be of the form of a circle, an ellipse, a hyperbola or a parabola. \n
            Ways to fillet edges of a solid: \n
            - Fillet the specified edges of the solid with a constant radius. \n
            - Create fillets on vertices and the edges of the solid adjacent to these vertices with a constant radius.
              Three edges must be incident to each vertex. \n
            - Fillet the given edges of the solid with a variable radius specified by MbEdgeFunction.function. 
              The parameters of the cross-section can vary by the specified law. \n
            The way of creating a fillet is determined by a set of parameters in MbShellChamferParams.\~
  \param[in] solid        - \ru Исходное тело.
                            \en The source solid. \~
  \param[in] sameShell    - \ru Режим копирования входного тела.
                            \en Whether to copy the input solid. \~
  \param[in] filletParams - \ru Параметры операции.
                            \en The operation parameters. \~
  \param[out] result      - \ru Построенная оболочка (тело).
                            \en The resultant shell (solid). \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/ // ---
MATH_FUNC( MbResultType ) FilletSolid(        MbSolid &             solid,
                                              MbeCopyMode           sameShell,
                                       const  MbShellFilletValues & filletParams,
                                              c3d::SolidSPtr &      result,
                                              IProgressIndicator *  progInd = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Скруглить цепочку граней тела.
           \en Create a fillet of faces of the solid. \~
  \details \ru Скруглить указанные грани тела. \n
    Функция выполняет замену указанных граней исходного тела гранями,
    гладко сопрягающими грани, связанные с указанными гранью.
    В поперечном сечении сопрягающие грани имеют форму
    дуги окружности, касающейся трёх граней исходного тела. \n
           \en Create a fillet on the specified faces of the solid. \n
    The function performs replacement of the specified faces of the source solid with faces
    smoothly connecting the faces adjacent to the specified faces.
    The cross-section of the connecting faces has a form of
    a circle arc tangent to three faces of the source solid. \n \~
  \param[in] solid          - \ru Исходное тело.
                              \en The source solid. \~
  \param[in] sameShell      - \ru Режим копирования входного тела.
                              \en Whether to copy the input solid. \~
  \param[in] filletParams   - \ru Параметры скругления.
                              \en Parameters of fillet. \~
  \param[out] result        - \ru Построенная оболочка (тело).
                              \en The resultant shell (solid). \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC( MbResultType ) FullFilletSolid(        MbSolid &                  solid,
                                                  MbeCopyMode                sameShell,
                                           const  MbShellFullFilletParams &  filletParams,
                                                  c3d::SolidSPtr &           result );


//------------------------------------------------------------------------------
/** \brief \ru Построить фаски ребер тела.
           \en Create chamfers for edges of the solid. \~
  \details \ru Построить фаски указанных ребер тела. \n
           Функция выполняет замену указанных рёбер исходного тела гранями фасок. \n
           \en Create chamfers for the specified edges of the solid. \n
           The function performs replacement of the specified edges of the source solid
           with faces of chamfers. \n \~
  \param[in] solid -          \ru Исходное тело.
                              \en The source solid. \~
  \param[in] sameShell      - \ru Режим копирования входного тела.
                              \en Whether to copy the input solid. \~
  \param[in] chamferParams  - \ru Параметры операции.
                              \en The operation parameters. \~
  \param[out] result        - \ru Построенная оболочка (тело).
                              \en The resultant shell (solid). \~
  \return                     \ru Возвращает код результата операции.
                              \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) ChamferSolid(       MbSolid &              solid, 
                                             MbeCopyMode            sameShell,
                                       const MbShellChamferParams & chamferParams,
                                             c3d::SolidSPtr &       result );


//------------------------------------------------------------------------------
/** \brief \ru Создать эквидистантное тело.
           \en Create а equidistant solid. \~
  \details \ru Создать эквидистантное тело или оболочку. \n
           \en Create an offset solid by equidistant faces or create an equidistant shell. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Режим копирования входного тела.
                         \en Whether to copy the input solid. \~
  \param[in] offset - \ru Расстояние смещения граней.
                      \en The equidistant parameter. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \param[out] result - \ru Построенная оболочка (тело).
                       \en The resultant shell (solid). \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) OffsetSolid(       MbSolid &      solid, 
                                            MbeCopyMode    sameShell, 
                                            double         offset, 
                                      const MbSNameMaker & names,
                                            MbSolid *&     result );

//------------------------------------------------------------------------------
/** \brief \ru Создать тонкостенное тело исключением граней.
           \en Create a thin-walled solid by exclusion of faces. \~
  \details \ru Создать тонкостенное тело исключением граней outFaces \n
    и приданием одинаковой толщины оставшимся граням \n
    или создание незамкнутой оболочки. \n
           \en Create a thin-walled solid by exclusion of faces outFaces \n
    and supplying the rest of faces with the same thickness \n
    or create an open shell. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Режим копирования входного тела.
                         \en Whether to copy the input solid. \~
  \param[in] outFaces - \ru Вскрываемые грани тела.
                        \en Faces of the solid to open. \~
  \param[in] params - \ru Параметры операции.
                      \en The operation parameters. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \param[in]  copyFaceAttrs - \ru Копировать атрибуты из исходных граней в эквидистантные.
                              \en Copy attributes of initial faces to offset faces. \~
  \param[out] result - \ru Построенная оболочка (тело).
                       \en The resultant shell (solid). \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) ThinSolid(       MbSolid &         solid, 
                                          MbeCopyMode       sameShell, 
                                          RPArray<MbFace> & outFaces, 
                                          SweptValues &     params, 
                                    const MbSNameMaker &    names,
                                          bool              copyFaceAttrs,
                                          MbSolid *&        result );


//------------------------------------------------------------------------------
/** \brief \ru Создать тонкостенное тело исключением граней.
           \en Create a thin-walled solid by exclusion of faces. \~
  \details \ru Создать тонкостенное тело исключением граней outFaces \n
    и приданием различной толщины оставшимся граням \n
    или создание незамкнутой оболочки. \n
           \en Create a thin-walled solid by exclusion of faces outFaces \n
    and supplying the rest of faces with different thickness \n
    or create an open shell. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Режим копирования входного тела.
                         \en Whether to copy the input solid. \~
  \param[in] outFaces - \ru Вскрываемые грани тела.
                        \en Faces of the solid to open. \~
  \param[in] offFaces - \ru Множество граней, для которых заданы индивидуальные значения толщин.
                        \en An array of faces for which the individual values of thickness are specified. \~
  \param[in] offDists - \ru Множество индивидуальных значений толщин (должен быть синхронизирован с массивом offFaces).
                        \en An array of individual values of thickness (must be synchronized with the array 'offFaces'). \~
  \param[in] params - \ru Параметры операции.
                      \en The operation parameters. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \param[in]  copyFaceAttrs - \ru Копировать атрибуты из исходных граней в эквидистантные.
                              \en Copy attributes of initial faces to offset faces. \~
  \param[out] result - \ru Построенная оболочка (тело).
                       \en The resultant shell (solid). \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) ThinSolid(       MbSolid &         solid, 
                                          MbeCopyMode       sameShell, 
                                          RPArray<MbFace> & outFaces, 
                                          RPArray<MbFace> & offFaces, 
                                          SArray<double> &  offDists,
                                          SweptValues &     params, 
                                    const MbSNameMaker &    names,
                                          bool              copyFaceAttrs,
                                          MbSolid *&        result );


//------------------------------------------------------------------------------
/** \brief \ru Выполнить разбиение граней оболочки.
           \en Perform splitting of a shell faces. \~
  \details \ru Выполнить разбиение граней оболочки поверхностями выдавливания контуров. \n
    Функция создаёт копию тела и разбивает указанные грани поверхностями выдавливания контуров. \n
           \en Perform splitting of a shell faces with surfaces of the contours extrusion. \n
    The function creates a copy of a solid and splits the specified faces with surfaces of the contours extrusion. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Режим копирования входного тела.
                         \en Whether to copy the input solid. \~
  \param[in] spPlace - \ru Система координат контуров.
                       \en The coordinate system of the contours. \~
  \param[in] spType - \ru Направление вытягивания.
                      \en The extrusion direction. \~
  \param[in] spContours - \ru Контура разбиения.
                          \en The contours of splitting. \~
  \param[in] spSame - \ru Использовать оригиналы или копии кривых.
                      \en Whether to use the originals or copies of curves. \~
  \param[in] selFaces - \ru Выбранные грани входного тела.
                        \en The chosen faces of the input solid. \~
  \param[in] flags - \ru Флаги слияния элементов оболочки.
                     \en Control flags of shell items merging. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \param[out] result - \ru Построенная оболочка (тело).
                       \en The resultant shell (solid). \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) SplitSolid(       MbSolid &            solid,
                                           MbeCopyMode          sameShell,
                                     const MbPlacement3D &      spPlace,
                                           MbeSenseValue        spType,
                                     const RPArray<MbContour> & spContours,
                                           bool                 spSame,
                                           RPArray<MbFace> &    selFaces,
                                     const MbMergingFlags &     flags,
                                     const MbSNameMaker &       names,
                                           MbSolid *&           result );


//------------------------------------------------------------------------------
/** \brief \ru Выполнить разбиение граней оболочки.
           \en Perform splitting of a shell faces. \~
  \details \ru Выполнить разбиение граней оболочки пространственными кривыми, поверхностями и оболочками. \n
    Функция создаёт копию тела и разбивает указанные грани пространственными кривыми, поверхностями и оболочками. \n
    Пространственные элементы разбиения не должны иметь полных или частичных наложений, а также - самопересечений. \n
           \en Perform splitting of the shell faces with space curves, surfaces and shells. \n
    The function creates a copy of the solid and splits the specified faces with space curves, surfaces and shells. \n
    The spatial elements of a splitting must not have complete or partial overlaps, and also self-intersections. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Режим копирования входного тела.
                         \en Whether to copy the input solid. \~
  \param[in] spItems - \ru Пространственные элементы разбиения.
                       \en A spatial elements of splitting. \~
  \param[in] spSame - \ru Использовать оригиналы или копии кривых.
                      \en Whether to use the originals or copies of curves. \~
  \param[in] selFaces - \ru Выбранные грани входного тела.
                        \en The chosen faces of the input solid. \~
  \param[in] flags - \ru Флаги слияния элементов оболочки.
                     \en Control flags of shell items merging. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \param[out] result - \ru Построенная оболочка (тело).
                       \en The resultant shell (solid). \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) SplitSolid(       MbSolid &              solid,
                                           MbeCopyMode            sameShell,
                                     const RPArray<MbSpaceItem> & spItems,
                                           bool                   spSame,
                                           RPArray<MbFace> &      selFaces,
                                     const MbMergingFlags &       flags,
                                     const MbSNameMaker &         names,
                                           MbSolid *&             result );


//------------------------------------------------------------------------------
/** \brief \ru Уклонить указанные грани тела.
           \en Slope the specified faces of the solid. \~
  \details \ru Уклонить указанные грани тела от нейтральной изоплоскости или нейтральных ребер на заданный угол. \n
           \en Slope the specified faces of the solid at the specified angle relative to the neutral isoplane or neutral edges. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Режим копирования входного тела.
                         \en Whether to copy the input solid. \~
  \param[in] draftParams - \ru Параметры уклона.
                          \en Draft parameters. \~
  \param[out] result - \ru Результаты построения тела уклона.
                       \en Results of creating a draft's solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) DraftSolid(       MbSolid &             solid, 
                                           MbeCopyMode           sameShell,
                                     const MbDraftSolidParams &  draftParams,
                                           MbDraftSolidResults & result );


//------------------------------------------------------------------------------
/** \brief \ru Выполнить объединение пересекающихся тел.
           \en Perform the union of intersecting solids. \~
  \details \ru Выполнить объединение пересекающихся тел и булеву операцию oType с телом solid, если оно не нулевое: \n
           bo_Union      - объединение, \n
           bo_Intersect  - пересечение, \n
           bo_Difference - вычитание. \n
    Если флаг проверки пересечения checkIntersect == true, то выполняется проверка на пересечение тел
    и булева операция объединения пересекающихся тел заданного множества в одно тело. В противном случае
    объединение тел заданного множества выполняется простым перекладыванием граней всех тел в одно новое тело. \n
    Если флаг регулярности множества тел isArray == true, то тела множества расположены в узлах 
    прямоугольной или круговой сетки и позиции тел заданы в именах граней. \n
           \en Perform the union of intersecting solids and a boolean operation oType with solid 'solid' if it is not null: \n
           bo_Union      - union, \n
           bo_Intersect  - intersection, \n
           bo_Difference - subtraction. \n
    If the flag of intersection check checkIntersect == true, check for solids intersection is performed
    and the boolean operation of union the intersection solids of the specified set into one solid is performed. Otherwise
    the union of solids from the given set is performed by simple moving the faces of all the solids into a new solid. \n
    If the flag of solid set regularity isArray == true, the solids are located at the nodes 
    of rectangular or circular grid and positions of solids are specified in the names of faces. \n \~
  \param[in] solid - \ru Тело.
                     \en A solid. \~
  \param[in] sameShell - \ru Режим копирования тела.
                         \en Whether to copy the solid. \~
  \param[in] solids - \ru Множество тел.
                      \en An array of solids. \~
  \param[in] sameShells - \ru Режим копирования тел.
                          \en Whether to copy the solids. \~
  \param[in] oType - \ru Тип булевой операции между телом и массивом тел.
                     \en The type of the boolean operation for the solid and the set of solids. \~
  \param[in] checkIntersect - \ru Проверять пересечение тел.
                              \en Whether to check the solids intersection. \~
  \param[in] mergeFaces     - \ru Управляющие флаги слияния элементов оболочки.
                              \en Control flags of shell items merging. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \param[in] isArray - \ru Флаг регулярности множества тел.
                       \en A flag of solid set regularity. \~
  \param[out] result - \ru Построенное тело.
                       \en The resultant solid. \~
  \param[out] notGluedSolids - \ru Множество тел, которые не получилось приклеить.
                               \en An array of solids which was not glued. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) UnionResult(       MbSolid *          solid, 
                                            MbeCopyMode        sameShell, 
                                            RPArray<MbSolid> & solids, 
                                            MbeCopyMode        sameShells, 
                                            OperationType      oType, 
                                            bool               checkIntersect,
                                      const MbMergingFlags &    mergeFlags,
                                      const MbSNameMaker &     names, 
                                            bool               isArray,
                                            MbSolid *&         result,
                                            RPArray<MbSolid> * notGluedSolids = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Создать одно тело из присланных тел.
           \en Create a solid from the specified solids. \~ 
  \details \ru Создать тело с объединением или без объединения пересекающихся тел. \n
    Если флаг проверки пересечения checkIntersect == true, то выполняется проверка на пересечение тел 
    и булева операция объединения пересекающихся тел заданного множества в одно тело. В противном случае
    объединение тел заданного множества выполняется простым перекладыванием граней всех тел в одно новое тело. \n
    Если флаг регулярности множества тел isArray == true, то тела множества расположены в узлах 
    прямоугольной или круговой сетки и позиции тел заданы в именах граней. \n
           \en Create a solid with or without union of the intersecting solids. \n
    If the flag of intersection check checkIntersect == true, check for solids intersection is performed 
    and the boolean operation of union the intersection solids of the specified set into one solid is performed. Otherwise
    the union of solids from the given set is performed by simple moving the faces of all the solids into a new solid. \n
    If the flag of solid set regularity isArray == true, the solids are located at the nodes 
    of rectangular or circular grid and positions of solids are specified in the names of faces. \n \~
  \param[in] solids - \ru Множество тел.
                      \en An array of solids. \~
  \param[in] sameShells - \ru Режим копирования тел.
                          \en Whether to copy the solids. \~
  \param[in] checkIntersect - \ru Проверять пересечение тел.
                              \en Whether to check the solids intersection. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \param[in] isArray - \ru Флаг регулярности множества тел.
                       \en A flag of solid set regularity. \~
  \param[out] result - \ru Построенное тело.
                       \en The resultant solid. \~
  \param[out] notGluedSolids - \ru Множество тел, которые не получилось приклеить.
                               \en An array of solids which was not glued. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) UnionSolid(       RPArray<MbSolid> & solids, 
                                           MbeCopyMode        sameShells,
                                           bool               checkIntersect,
                                     const MbSNameMaker &     names, 
                                           bool               isArray,
                                           MbSolid *&         result,
                                           RPArray<MbSolid> * notGluedSolids = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Создать одно тело из присланных тел.
           \en Create a solid from the specified solids. \~ 
  \details \ru Создать одно тело из присланных тел, не меняя их. \n
    Объединение тел заданного множества выполняется простым перекладыванием 
    граней всех тел в одно новое тело. \n
           \en Create a solid from the specified solids without the modification of the given solids. \n
    The union of solids from the specified set is performed by simple moving 
    the faces of all the solids into a new solid. \n \~
  \param[in] solids - \ru Множество тел.
                      \en An array of solids. \~
  \param[in] names  - \ru Именователь.
                      \en An object for naming the new objects. \~
  \param[out] result - \ru Построенное тело.
                       \en The resultant solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) UnionSolid( const RPArray<MbSolid> & solids, 
                                     const MbSNameMaker &     names,
                                           MbSolid *&         result );


//------------------------------------------------------------------------------
/** \brief \ru Создать одно тело из присланных тел.
           \en Create a solid from the specified solids. \~ 
  \details \ru Создать одно тело из присланных тел, не меняя их. \n
    Объединение тел заданного множества выполняется простым перекладыванием 
    граней всех тел в одно новое тело. \n
           \en Create a solid from the specified solids without the modification of the given solids. \n
    The union of solids from the specified set is performed by simple moving 
    the faces of all the solids into a new solid. \n \~
  \param[in] solids - \ru Множество тел.
                      \en An array of solids. \~
  \param[in] names  - \ru Именователь.
                      \en An object for naming the new objects. \~
  \param[out] result - \ru Построенное тело.
                       \en The resultant solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) UnionSolid( const c3d::SolidsVector & solids, 
                                     const MbSNameMaker &      names,
                                           MbSolid *&          result );


//------------------------------------------------------------------------------
/** \brief \ru Создать одно тело из присланных тел.
           \en Create a solid from the specified solids. \~ 
  \details \ru Создать одно тело из присланных тел, не меняя их. \n
    Объединение тел заданного множества выполняется простым перекладыванием 
    граней всех тел в одно новое тело. \n
           \en Create a solid from the specified solids without the modification of the given solids. \n
    The union of solids from the specified set is performed by simple moving 
    the faces of all the solids into a new solid. \n \~
  \param[in] solids - \ru Множество тел.
                      \en An array of solids. \~
  \param[in] names  - \ru Именователь.
                      \en An object for naming the new objects. \~
  \param[out] result - \ru Построенное тело.
                       \en The resultant solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) UnionSolid( const c3d::SolidsSPtrVector & solids, 
                                     const MbSNameMaker &          names,
                                           MbSolid *&              result );


//------------------------------------------------------------------------------
/** \brief \ru Разделить тело на отдельные части.
           \en Split the solid into separate parts. \~
  \details \ru Если исходное тело распадается на части, то наибольшая часть остаётся в исходном теле, 
    а остальные части части будут сложены в присланный контейнер тел. \n
    Если флаг сортировки sort == true, то в исходном теле останется часть с наибольшим габаритом, 
    а отделённые части будут сортированы по убыванию габарита. В противном случае в исходном теле 
    останется часть, топологически связанная с первой гранью, а отделённые части будут сортированы
    по номеру начальной грани в исходном теле. \n
           \en If the source solid is decomposed, the greatest part remains in the source solid, 
    and the other parts are put into the given array of solids. \n
    If 'sort' == 'true', the part with the greatest bounding box will remain in the source solid, 
    and separated parts will be sorted by bounding box size in descending order. Otherwise a part topologically connected with the first face will remain in the source solid 
    and the separated parts will be sorted
    by the number of the initial face in the source solid. \n \~
  \param[in,out] solid - \ru Исходное модифицируемое тело.
                         \en The source solid to be modified. \~
  \param[out] parts - \ru Отделённые части тела.
                      \en The separated parts of the solid. \~
  \param[in] sort - \ru Сортировать по убыванию габарита.
                    \en Whether to sort by the bounding box size in descending order. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \return \ru Возвращает количество отделенных частей.
          \en Returns the number of the separated parts. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (size_t) DetachParts(       MbSolid &          solid, 
                                      RPArray<MbSolid> & parts, 
                                      bool               sort,
                                const MbSNameMaker &     names );


//------------------------------------------------------------------------------
/** \brief \ru Разделить тело на отдельные части.
           \en Split the solid into separate parts. \~
  \details \ru Если исходное тело распадается на части, то наибольшая часть остаётся в исходном теле, 
    а остальные части части будут сложены в присланный контейнер тел. \n
    Если флаг сортировки sort == true, то в исходном теле останется часть с наибольшим габаритом, 
    а отделённые части будут сортированы по убыванию габарита. В противном случае в исходном теле 
    останется часть, топологически связанная с первой гранью, а отделённые части будут сортированы
    по номеру начальной грани в исходном теле. \n
           \en If the source solid is decomposed, the greatest part remains in the source solid, 
    and the other parts are put into the given array of solids. \n
    If 'sort' == 'true', the part with the greatest bounding box will remain in the source solid, 
    and separated parts will be sorted by bounding box size in descending order. Otherwise a part topologically connected with the first face will remain in the source solid 
    and the separated parts will be sorted
    by the number of the initial face in the source solid. \n \~
  \param[in,out] solid - \ru Исходное модифицируемое тело.
                         \en The source solid to be modified. \~
  \param[out] parts - \ru Отделённые части тела.
                      \en The separated parts of the solid. \~
  \param[in] sort - \ru Сортировать по убыванию габарита.
                    \en Whether to sort by the bounding box size in descending order. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \return \ru Возвращает количество отделенных частей.
          \en Returns the number of the separated parts. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (size_t) DetachParts(       MbSolid &           solid, 
                                      c3d::SolidsVector & parts, 
                                      bool                sort,
                                const MbSNameMaker &      names );


//------------------------------------------------------------------------------
/** \brief \ru Разделить тело на отдельные части.
           \en Split the solid into separate parts. \~
  \details \ru Если исходное тело распадается на части, то наибольшая часть остаётся в исходном теле, 
    а остальные части части будут сложены в присланный контейнер тел. \n
    Если флаг сортировки sort == true, то в исходном теле останется часть с наибольшим габаритом, 
    а отделённые части будут сортированы по убыванию габарита. В противном случае в исходном теле 
    останется часть, топологически связанная с первой гранью, а отделённые части будут сортированы
    по номеру начальной грани в исходном теле. \n
           \en If the source solid is decomposed, the greatest part remains in the source solid, 
    and the other parts are put into the given array of solids. \n
    If 'sort' == 'true', the part with the greatest bounding box will remain in the source solid, 
    and separated parts will be sorted by bounding box size in descending order. Otherwise a part topologically connected with the first face will remain in the source solid 
    and the separated parts will be sorted
    by the number of the initial face in the source solid. \n \~
  \param[in,out] solid - \ru Исходное модифицируемое тело.
                         \en The source solid to be modified. \~
  \param[out] parts - \ru Отделённые части тела.
                      \en The separated parts of the solid. \~
  \param[in] sort - \ru Сортировать по убыванию габарита.
                    \en Whether to sort by the bounding box size in descending order. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \return \ru Возвращает количество отделенных частей.
          \en Returns the number of the separated parts. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (size_t) DetachParts(       MbSolid &               solid, 
                                      c3d::SolidsSPtrVector & parts, 
                                      bool                    sort,
                                const MbSNameMaker &          names );


//------------------------------------------------------------------------------
/** \brief \ru Разделить тело на отдельные части.
           \en Split the solid into separate parts. \~
  \details \ru Если исходное тело распадается на части, то все его части будут сложены в присланный контейнер тел. \n
    Исходное тело остаётся неизменённым. \n
           \en If the source solid is decomposed, all the parts of the solid will be put into the given array of solids. \n
    The source solid remains unchanged. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[out] parts - \ru Части тела.
                      \en The parts of the solid. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \return \ru Возвращает количество созданных частей.
          \en Returns the number of created parts. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (size_t) CreateParts( const MbSolid &          solid, 
                                      RPArray<MbSolid> & parts, 
                                const MbSNameMaker &     names );


//------------------------------------------------------------------------------
/** \brief \ru Разделить тело на отдельные части.
           \en Split the solid into separate parts. \~
  \details \ru Если исходное тело распадается на части, то все его части будут сложены в присланный контейнер тел. \n
    Исходное тело остаётся неизменённым. \n
           \en If the source solid is decomposed, all the parts of the solid will be put into the given array of solids. \n
    The source solid remains unchanged. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[out] parts - \ru Части тела.
                      \en The parts of the solid. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \return \ru Возвращает количество созданных частей.
          \en Returns the number of created parts. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (size_t) CreateParts( const MbSolid &           solid, 
                                      c3d::SolidsVector & parts, 
                                const MbSNameMaker &      names );


//------------------------------------------------------------------------------
/** \brief \ru Разделить тело на отдельные части.
           \en Split the solid into separate parts. \~
  \details \ru Если исходное тело распадается на части, то все его части будут сложены в присланный контейнер тел. \n
    Исходное тело остаётся неизменённым. \n
           \en If the source solid is decomposed, all the parts of the solid will be put into the given array of solids. \n
    The source solid remains unchanged. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[out] parts - \ru Части тела.
                      \en The parts of the solid. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \return \ru Возвращает количество созданных частей.
          \en Returns the number of created parts. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (size_t) CreateParts( const MbSolid &               solid, 
                                      c3d::SolidsSPtrVector & parts, 
                                const MbSNameMaker &          names );


//------------------------------------------------------------------------------
/** \brief \ru Разделить тело на отдельные части.
           \en Split the solid into separate parts. \~
  \details \ru Если исходное тело распадается на части, то все его части будут сложены в присланный контейнер тел. \n
    Исходное тело остаётся неизменённым. \n
           \en If the source solid is decomposed, all the parts of the solid will be put into the given array of solids. \n
    The source solid remains unchanged. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[out] parts - \ru Части тела.
                      \en The parts of the solid. \~
  \param[in] sort - \ru Сортировать по убыванию габарита.
                    \en Whether to sort by the bounding box size in descending order. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \return \ru Возвращает количество созданных частей.
          \en Returns the number of created parts. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (size_t) CreateParts( const MbSolid &          solid, 
                                      RPArray<MbSolid> & parts, 
                                      bool               sort,
                                const MbSNameMaker &     names );


//------------------------------------------------------------------------------
/** \brief \ru Разделить тело на отдельные части.
           \en Split the solid into separate parts. \~
  \details \ru Если исходное тело распадается на части, то все его части будут сложены в присланный контейнер тел. \n
    Исходное тело остаётся неизменённым. \n
           \en If the source solid is decomposed, all the parts of the solid will be put into the given array of solids. \n
    The source solid remains unchanged. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[out] parts - \ru Части тела.
                      \en The parts of the solid. \~
  \param[in] sort - \ru Сортировать по убыванию габарита.
                    \en Whether to sort by the bounding box size in descending order. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \return \ru Возвращает количество созданных частей.
          \en Returns the number of created parts. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (size_t) CreateParts( const MbSolid &           solid, 
                                      c3d::SolidsVector & parts, 
                                      bool                sort,
                                const MbSNameMaker &      names );


//------------------------------------------------------------------------------
/** \brief \ru Разделить тело на отдельные части.
           \en Split the solid into separate parts. \~
  \details \ru Если исходное тело распадается на части, то все его части будут сложены в присланный контейнер тел. \n
    Исходное тело остаётся неизменённым. \n
           \en If the source solid is decomposed, all the parts of the solid will be put into the given array of solids. \n
    The source solid remains unchanged. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[out] parts - \ru Части тела.
                      \en The parts of the solid. \~
  \param[in] sort - \ru Сортировать по убыванию габарита.
                    \en Whether to sort by the bounding box size in descending order. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \return \ru Возвращает количество созданных частей.
          \en Returns the number of created parts. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (size_t) CreateParts( const MbSolid &               solid, 
                                      c3d::SolidsSPtrVector & parts, 
                                      bool                    sort,
                                const MbSNameMaker &          names );


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку тела по поверхности и толщине.
           \en Create a shell of the solid from a surface and a thickness. \~
  \details \ru Выполнить построение тела путём придания толщины заданной поверхности. \n
           \en Create a solid by supplying the surface with a thickness. \n \~
  \param[in] surface - \ru Поверхность.
                       \en The surface. \~
  \param[in] faceSense - \ru Ориентация нормали поверхности.
                         \en The surface normal orientation. \~
  \param[in] params - \ru Параметры операции.
                      \en The operation parameters. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \param[in] name - \ru Основное простое имя.
                    \en The main simple name. \~
  \param[out] result - \ru Построенное тело.
                       \en The resultant solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) ThinSolid( const MbSurface &    surface, 
                                          bool           faceSense, 
                                          SweptValues &  params, 
                                    const MbSNameMaker & names, 
                                          SimpleName     name, 
                                          MbSolid *&     result );


//------------------------------------------------------------------------------
/** \brief \ru Создать отверстие, карман, фигурный паз в теле.
           \en Create a hole, a pocket, a groove in the solid. \~
    \details \ru Создать отверстие, карман, фигурный паз в теле или создать сверло, бобышку, если solid==nullptr. \n
             \en Create a hole, a pocket, a groove in the solid or create a drill, a boss if 'solid' == nullptr. \n \~
    \param[in]  solid     - \ru Исходное тело.
                            \en The source solid. \~
    \param[in]  sameShell - \ru Режим копирования тела.
                            \en Whether to copy the solid. \~
    \param[in]  params    - \ru Параметры построения оболочки с отверстием.
                            \en Parameters for creating a shell with a hole. \~
    \param[out] result    - \ru Результаты операции.
                            \en The operation results. \~
    \return \ru Возвращает код результата операции.
            \en Returns operation result code. \~
    \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) HoleSolid( const c3d::SolidSPtr &    solid,
                                          MbeCopyMode         sameShell,
                                    const MbHoleSolidParams & params,
                                          c3d::SolidSPtr &    result );


//------------------------------------------------------------------------------
/** \brief \ru Выделить в отдельное тело указанную часть тела.
           \en Extract the specified part of a decomposing solid. \~
    \details \ru Создать тело, из указанной части тела, распадающегося на части. 
                 Исходное тело должно состоять из отдельных частей. \n
             \en Create a solid from the specified part of decomposing solid. 
                 The source solid should consist of separate parts. \n \~
    \param[in]  solid  - \ru Разделяемое на части тело.
                         \en A decomposing solid. \~
    \param[in]  params - \ru Параметры распадающегося на части тела.
                         \en Parameters for decomposing solid to a separate solid. \~
    \param[out] result - \ru Результаты операции.
                         \en The operation results. \~
    \return \ru Возвращает код результата операции.
            \en Returns operation result code. \~
    \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) ShellPart( const MbSolid &            solid,
                                    const MbShellPartParams &  params,
                                          MbShellPartResults & result );


//------------------------------------------------------------------------------
/** \brief \ru Размножить тело.
           \en Duplicate the solid. \~
  \details \ru Размножить тело согласно параметрам и объединить копии в одно тело.\n
           \en Duplicate the solid according to the parameters and unite copies into a single solid. \n \~
  \param[in] solid   - \ru Исходное тело.
                       \en The source solid. \~
  \param[in]  params - \ru Параметры размножения. 
                       \en The parameters of duplication. \~
  \param[out] result - \ru Результирующее тело.
                       \en The result solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns the resulting operation code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) DuplicationSolid( const MbSolid                  & solid, 
                                           const MbDuplicationSolidParams & params, 
                                                 c3d::SolidSPtr           & result );


//------------------------------------------------------------------------------
/** \brief \ru Создать тело-слиток из присланных объектов.
           \en Create an ingot solid from the specified objects. \~ 
  \details \ru Создать тело-слиток из присланных объектов. \n
               Среди присланных объектов могут присутствовать тела (#MbSolid), вставки тел (#MbInstance) и сборки тел (#MbAssembly).
               Из них строится одно тело, которое по внешности совпадает с присланными телами 
               и служит их упрощенным заменителем по внешним параметрам. \n
           \en Create an ingot solid from the specified objects without their modification. \n
               The input objects may include #MbSolid, #MbInstance and #MbAssembly.
               The function constructs the singular solid which appearence matches the input objects and which
               serves as a simplified substitute for their external parameters. \n \~
  \param[in] makeCopy  - \ru Опция копирования исходных тел.
                         \en Whether to copy the original solids.\~
  \param[in] params  - \ru Параметры операции.
                       \en The operation parameters.\~
  \param[out] result - \ru Построенное тело.
                       \en The resulting solid.\~
  \return \ru Возвращает код результата операции.
          \en Returns the operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) IngotSolid(       bool                 makeCopy,
                                     const MbIngotSolidParams & params, 
                                           c3d::SolidSPtr     & resIngot );

#endif // __ACTION_SOLID_H
