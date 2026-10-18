////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Методы построения полигональных геометрических объектов с топологией.
         \en Functions for construction of the polygonal geometric object with topology. \~
  \details \ru Полигональные геометрические объекты могут быть построены по набору точек или на базе других объектов.
           \en Polygonal geometric objects can be constructed using a set of points or on the basis of other objects. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ACTION_POLYMESH_H
#define __ACTION_POLYMESH_H

#include <mb_cart_point.h>
#include <mb_cart_point3d.h>
#include <mb_operation_result.h>
#include <curve3d.h>

class MbPolymesh;
class MbFaceShell;
class MbPolymeshTopo;
class MbHalfedgeBoundaryCriterionBase;
class MbMeshCurvatureParams;
class MbDrapeShellParams;
class MbDrapeShellResults;
class MbRemeshParams;
class MbShellLinearizationParams;
class MbShellLinearizationResults;
class MbMergePolymeshParams;
class MbMeshSmoothingParams;
class MbPolyline3D;
class MbPolylinePolymeshProjectParams;
class MbPolylinePolymeshProjectResults;
class MbFillClosedCurveParams;
class MbFillPolymeshHoleParams;
class MbPolymeshNormalsParams;
class MbCurvePolymeshProjectParams;
class MbPolymeshProjectResults;
class MbPathPolymeshOffsetParams;
class MbPolymeshBooleanParams;
class MbPolymeshBooleanResults;
class MbPolymeshImprintParams;
class MbPolymeshImprintResults;


//------------------------------------------------------------------------------
/** \brief \ru Создать полигональный объект по оболочке.
           \en Create the polymesh by the shell. \~
  \details \ru Создать полигональный объект по оболочке.
               В нем будет сохранен маппинг от регионов и ребер сегментации к граням и ребрам оболочки.
               Любые операции, изменяющие топологию сегментации, приведут к невалидности маппинга.
           \en Create the polymesh by the shell.
               Creates mapping from segmentation edges to shell edges and from segmentation regions to shell faces.
               Any operations that change segmentation topology will lead to invalid mapping.
  \param[in] params - \ru Параметры алгоритма.
                      \en The parameters. \~
  \param[in] shell - \ru Оболочка.
                      \en The shell. \~
  \param[in,out] mesh - \ru Полигональная сетка.
                        \en The polymesh.  \~
  \return \ru Код результата операции, rt_Success в случае успеха.
          \en Returns the operation result code, rt_Success if polymesh has been created. \n
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
DEPRECATE_DECLARE_REPLACE( CreatePolymeshByShell with MbShellLinearizationResults )
MATH_FUNC( MbResultType ) CreatePolymeshByShell( const MbShellLinearizationParams & params, const MbFaceShell & shell, MbPolymesh & mesh );


//------------------------------------------------------------------------------
/** \brief \ru Создать полигональный объект по оболочке.
           \en Create the polymesh by the shell. \~
  \details \ru Создать полигональный объект по оболочке.
               В нем будет сохранен маппинг от регионов и ребер сегментации к граням и ребрам оболочки.
               Любые операции, изменяющие топологию сегментации, приведут к невалидности маппинга.
               Результаты алгоритма содержат набор граней, которые не удалось линеаризовать.
           \en Create the polymesh by the shell.
               Creates mapping from segmentation edges to shell edges and from segmentation regions to shell faces.
               Any operations that change segmentation topology will lead to invalid mapping.
               The results contain the indices of unlinearized faces.
  \param[in] params   - \ru Параметры алгоритма.
                        \en The parameters. \~
  \param[in] shell    - \ru Оболочка.
                        \en The shell. \~
  \param[in,out] mesh - \ru Полигональная сетка.
                        \en The polymesh.  \~
  \param[out] results - \ru Результаты алгоритма.
                        \en The results. \~
  \return \ru Код результата операции, rt_Success в случае успеха.
          \en Returns the operation result code, rt_Success if polymesh has been created. \n
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
MATH_FUNC( MbResultType ) CreatePolymeshByShell( const MbShellLinearizationParams &  params,
                                                 const MbFaceShell &                 shell,
                                                       MbPolymesh &                  mesh,
                                                       MbShellLinearizationResults & results );


//------------------------------------------------------------------------------
/** \brief \ru Выделить связные группы из набора фасетов.
           \en Get connected groups from facets. \~
  \param[in] mesh    - \ru Полигональный объект с топологией.
                       \en A polygonal object with topology. \~
  \param[in] facets  - \ru Набор фасетов.
                       \en Facet array. \~
  \param[out] groups - \ru Связные группы фасетов.
                       \en Groups of connected facets. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/ // ---
MATH_FUNC( void ) GetConnectedFacetGroups( const MbPolymesh &                      mesh, 
                                           const c3d::IndicesVector &              facets, 
                                                 std::vector<c3d::IndicesVector> & groups );


//------------------------------------------------------------------------------
/** \brief \ru Выделить связные группы из набора регионов.
           \en Get connected groups from regions. \~
  \param[in] mesh    - \ru Полигональный объект с топологией.
                       \en A polygonal object with topology. \~
  \param[in] regions - \ru Набор регионов.
                       \en Region array. \~
  \param[out] groups - \ru Связные группы регионов.
                       \en Groups of connected regions. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/ // ---
MATH_FUNC( void ) GetConnectedRegionGroups( const MbPolymesh &                      mesh,
                                            const c3d::IndicesVector &              regions,
                                                  std::vector<c3d::IndicesVector> & groups );


//------------------------------------------------------------------------------
/** \brief \ru Разбить сетку на регионы согласно критерию границы.
           \en Divide a polymesh into regions according to a given boundary criterion. \~
  \param[in] mesh    - \ru Полигональный объект с топологией.
                       \en A polygonal object with topology. \~
  \param[in] crit    - \ru Критерий границы.
                       \en Boundary criterion. \~
  \param[out] facets - \ru Группы фасетов, разделенные по регионам.
                       \en Groups of facets divided into regions. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/ // ---
MATH_FUNC( void ) GetPolymeshRegions( const MbPolymesh &                      mesh, 
                                      const MbHalfedgeBoundaryCriterionBase & crit,
                                            std::vector<c3d::IndicesVector> & facets );


//------------------------------------------------------------------------------
/** \brief \ru Получить регион сетки согласно критерию границы по указанному фасету-затравке.
           \en Get the region containing a given facet according to a given boundary criterion. \~
  \param[in] mesh    - \ru Полигональный объект с топологией.
                       \en A polygonal object with topology. \~
  \param[in] crit    - \ru Критерий границы.
                       \en Boundary criterion. \~
  \param[in] seed    - \ru Фасет-затравка.
                       \en A seed facet. \~
  \param[out] facets - \ru Все фасеты региона.
                       \en All the facets of a region. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/ // ---
MATH_FUNC( void ) GetPolymeshRegion( const MbPolymesh &                      mesh, 
                                     const MbHalfedgeBoundaryCriterionBase & crit,
                                           size_t                            seed, 
                                           c3d::IndicesVector &              facets );


//------------------------------------------------------------------------------
/** \brief \ru Получить границы региона фасетов.
           \en Get the boundary edges of a given region. \~
  \param[in] mesh        - \ru Полигональный объект с топологией.
                           \en A polygonal object with topology. \~
  \param[in] region      - \ru Набор фасетов, составляющих регион.
                           \en Facet array which forms a region. \~
  \param[out] boundaries - \ru Границы региона (набор граничных ребер).
                           \en Boundary edges of a region. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/ // ---
MATH_FUNC( void ) GetPolymeshRegionBoundaries( const MbPolymesh &                      mesh,
                                               const c3d::IndicesVector &              region, 
                                                     std::vector<c3d::IndicesVector> & boundaries );


//------------------------------------------------------------------------------
/** \brief \ru Получить открытые границы сетки.
           \en Get the open boundary edges of a polymesh. \~
  \param[in] mesh        - \ru Полигональный объект с топологией.
                           \en A polygonal object with topology. \~
  \param[out] boundaries - \ru Открытые границы (наборы граничных ребер).
                           \en Arrays of open boundary edges. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/ // ---
MATH_FUNC( void ) GetPolymeshOpenBoundaries( const MbPolymesh & mesh, std::vector<c3d::IndicesVector> & boundaries );


//------------------------------------------------------------------------------
/** \brief \ru Получить открытые границы сегментации.
           \en Get open segmentation boundaries. \~
  \param[in] mesh        - \ru Полигональный объект с топологией.
                           \en A polygonal object with topology. \~
  \param[out] boundaries - \ru Открытые границы (наборы граничных ребер сегментации).
                           \en Arrays of open boundary edges of a segmentation. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/ // ---
MATH_FUNC( void ) GetSegmOpenBoundaries( const MbPolymesh & mesh, std::vector<c3d::IndicesVector> & boundaries );


/// \ru Получить полилинию для ребра сегментации.
MATH_FUNC( bool ) GetPolymeshEdgePolyline( const MbPolymesh & mesh, size_t iEdSegm, c3d::SpacePointsVector & polyline );


/// \ru Получить полилинию для цикла сегментации.
MATH_FUNC( void ) GetPolymeshLoopPolyline( const MbPolymesh & mesh, size_t iLoop, c3d::SpacePointsVector & polyline );


//------------------------------------------------------------------------------
/** \brief \ru Построить контур сечения полигонального объекта плоскостью.
           \en Create a section contour for a polygonal object. \~
  \param[in] mesh       - \ru Полигональный объект с топологией.
                          \en A polygonal object with topology. \~
  \param[in] place      - \ru Секущая плоскость.
                          \en A cutting plane. \~
  \param[out] polylines - \ru Построенные ломаные контура сечения объекта.
                          \en The resultant contours. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/ // ---
MATH_FUNC( MbResultType ) PolymeshSection( const MbPolymesh &                 mesh,
                                           const MbPlacement3D &              place,
                                                 c3d::SpaceCurvesSPtrVector & polylines );


//------------------------------------------------------------------------------
/** \brief \ru Отрезать часть полигонального объекта плоскостью.
           \en Cut the part of a polygonal object by a plane. \~
  \param[in] place    - \ru Секущая плоскость.
                        \en A cutting plane. \~
  \param[in] bTriangulate    - \ru Триангулировать ли разрезанные фасеты.
                               \en Should cut facets be triangulated. \~
  \param[in,out] mesh - \ru Полигональный объект с топологией.
                        \en A polygonal object with topology. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/ // ---
MATH_FUNC( bool ) CutPolymeshByPlane( const MbPlacement3D & place,
                                            bool            bTriangulate,
                                            MbPolymesh    & mesh );


//------------------------------------------------------------------------------
/** \brief \ru Создать оттиск 2D прямой или плоскости на полигональном объекте.
           \en Imprint a 2D line or a plane on a polygonal object. \~
  \param[in] params   - \ru Параметры создания оттиска.
                        \en Imprinting parameters. \~
  \param[in,out] mesh - \ru Полигональный объект с топологией.
                        \en A polygonal object with topology. \~
  \param[out] results - \ru Результаты создания оттиска.
                        \en Imprinting results. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/ // ---
MATH_FUNC( MbResultType ) ImprintOnPolymesh( const MbPolymeshImprintParams &  params,
                                                   MbPolymesh &               mesh,
                                                   MbPolymeshImprintResults & results );


//------------------------------------------------------------------------------
/** \brief \ru Скопировать часть полигонального объекта в другой полигональный объект.
           \en Copy the part of a polygonal object into another polygonal object. \~
  \param[in] mesh        - \ru Оригинальный полигональный объект.
                           \en An original polygonal object. \~
  \param[in] facets      - \ru Набор копируемых фасетов.
                           \en A polygonal object with topology. \~
  \param[in,out] meshSub - \ru Полигональный объект, в который производится копирование.
                           \en A polygonal object in which data should be copied to. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development.\~
*/ // ---
MATH_FUNC( void ) CopyPolymeshPart( const MbPolymesh & mesh, const c3d::IndicesVector & facets, MbPolymesh & meshSub );


//------------------------------------------------------------------------------
/** \brief \ru Проверить, является ли полигональный объект триангуляцией.
           \en Verify if polymesh is triangulation. \~
  \param[in] mesh        - \ru Полигональный объект.
                           \en Polygonal object. \~
  \return \ru True если полигональный объект является триангуляцией, иначе false.
          \en True if polymesh is triangulation, otherwise false. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC( bool ) IsPolymeshTriangulation( const MbPolymesh & mesh );


//------------------------------------------------------------------------------
/** \brief \ru Ремешировать полигональный объект.
           \en Remesh a polygonal object. \~
  \details \ru Ремешировать полигональный объект.
               Он должен быть триангуляцией. Если метод ремешинга по кривизне, в сетке должен
               присутствовать атрибут кривизн.
               Атрибуты нормалей и кривизн будут удалены.
               Если в сетке есть оболочка как параметрическая подложка, она будет использована для проецирования
               вершин триангуляции.
           \en Remesh a polygonal object.
               The polymesh must contains only triangle facets.
               For remeshing by curvature method a curvature attribute must be defined.
               Deletes curvature and normal attributes.
               If the polymesh has a shell as a parametric substrate, it will be used for a polymesh vertices projection. \~
  \param[in] params - \ru Параметры алгоритма.
                      \en The parameters. \~
  \param[in,out] mesh - \ru Полигональная сетка.
                        \en The polymesh.  \~
  \return \ru Код результата операции, rt_Success в случае успеха.
          \en Returns the operation result code, rt_Success if remeshing is successful. \n
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
MATH_FUNC( MbResultType ) Remesh( const MbRemeshParams & params, MbPolymesh & mesh );


//------------------------------------------------------------------------------
/** \brief \ru Рассчитать кривизны полигональной сетки.
           \en Calculates mesh curvatures. \~
  \details \ru Рассчитать кривизны полигональной сетки.
               Результатом операции является появление у полигональной сетки атрибута кривизн.
               Если атрибут уже был, он будет удален и создан заново.
           \en Calculates mesh curvatures.
               Operation result is a curvature half edge attribute inserted into the original polymesh.
               If mesh has one it will be removed before caclulation. \~
  \param[in] params - \ru Параметры алгоритма.
                      \en The parameters. \~
  \param[in,out] mesh - \ru Полигональная сетка.
                        \en Polymesh.  \~
  \return \ru Код результата операции, rt_Success если кривизны были рассчитаны.
          \en Returns the operation result code, rt_Success if curvatures were calculated. \n
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
MATH_FUNC( MbResultType ) CalculatePolymeshCurvatures( const MbMeshCurvatureParams & params, MbPolymesh & mesh );


//------------------------------------------------------------------------------
/** \brief \ru Драпировка оболочки тканью.
           \en Drap a shell with fabric. \~
  \details \ru Драпировка оболочки тканью.
           \en Drap a shell with fabric.
  \param[in] params - \ru Параметры алгоритма.
                      \en The parameters. \~
  \param[out] results - \ru Результаты алгоритма.
                        \en Results.  \~
  \return \ru Код результата операции, rt_Success если драпировка получилась.
          \en Returns the operation result code, rt_Success if drape is successful. \n
  \ingroup Shell_Modeling
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
MATH_FUNC( MbResultType ) DrapeShell( const MbDrapeShellParams & params, MbDrapeShellResults & results );


//------------------------------------------------------------------------------
/** \brief \ru Объединить полигональный объект с другим полигональным объектом.
           \en Merge a polygonal object with another polygonal object. \~
  \param[in] meshFrom   - \ru Полигональный объект, который копируется в meshTo.
                          \en A polygonal object, being copied into meshTo. \~
  \param[in] params     - \ru Параметры объединения.
                          \en Merging parameters. \~
  \param[in-out] meshTo - \ru Полигональный объект, который аккумулирует результат объединения.
                          \en A polygonal object, accumulating merged objects. \~
  \return \ru Код результата операции, rt_Success, если объекты успешно объединены.
          \en Returns the operation result code, rt_Success if the objects are successfully merged. \n
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development.\~
*/ // ---
MATH_FUNC( MbResultType ) MergePolymeshes( const MbPolymesh & meshFrom, const MbMergePolymeshParams & params, MbPolymesh & meshTo );


//------------------------------------------------------------------------------
/** \brief \ru Сгладить полигональный объект по набору входных параметров сглаживания.
           \en Smooth a polygonal object using a set of input smoothing parameters. \~
  \param[in] params   - \ru Параметры сглаживания.
                        \en Smoothing parameters. \~
  \param[in-out] mesh - \ru Полигональный объект.
                         \en A polygonal object. \~
  \return \ru Код результата операции, rt_Success если объект успешно сглажен.
          \en Returns the operation result code, rt_Success if the object is successfully smoothed. \n
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development.\~
*/ // ---
MATH_FUNC( MbResultType ) SmoothPolymesh( const MbMeshSmoothingParams & params, MbPolymesh & mesh );


//------------------------------------------------------------------------------
/** \brief \ru Спроецировать полилинию на полигональный объект.
           \en Project a polyline on a polygonal object. \~
  \details \ru Спроецировать полилинию на полигональный объект.
               Если на некотором участке полилиния проецируется внутрь фасета, он разделяется на несколько фасетов.
               При этом создаются новые рёбра полигонального объекта.
           \en Project a polyline on a polygonal object.
               In case of projecting a polyline's fragment inside a facet, it splits into new ones, so new edges are created. \~
  \param[in-out] mesh - \ru Полигональный объект.
                        \en A polygonal object being projected on. \~
  \param[in] params   - \ru Параметры проецирования полилинии на полигональный объект.
                        \en Parameters for projecting a polyline on a polymesh. \~
  \param[out] results - \ru Результат проецирования полилинии на полигональный объект.
                        \en Result of projecting a polyline on a polymesh. \~
  \return \ru Код результата операции, rt_Success, если проекция успешно построена.
          \en Returns the operation result code, rt_Success if a polyline is successfully projected. \n
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development.\~
*/ // ---
MATH_FUNC( MbResultType ) ProjectPolylineOnPolymesh( MbPolymesh & mesh, const MbPolylinePolymeshProjectParams & params, MbPolylinePolymeshProjectResults & results );


//------------------------------------------------------------------------------
/** \brief \ru Заполнить отверстие, заданное замкнутой кривой.
           \en Fill a hole defined by a closed curve. \~
  \details \ru Заполнить отверстие, заданное замкнутой кривой.
           \en Fill a hole defined by a closed curve. \~
  \param[in] params - \ru Параметры заполнения.
                      \en Filling parameters. \~
  \param[out] patch - \ru Заплатка.
                      \en A patch. \~
  \return \ru Код результата операции.
          \en Returns the operation result code. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/ // ---
MATH_FUNC( MbResultType ) FillClosedCurve( const MbFillClosedCurveParams & params, MbPolymesh & patch );


//------------------------------------------------------------------------------
/** \brief \ru Заполнить отверстие полигонального объекта.
           \en Fill a polygonal object hole. \~
  \details \ru Заполнить отверстие полигонального объекта.
               В результате операции в полигональном объекте могут появиться неактивные элементы.
               Впоследствии их можно удалить вручную функцией DeleteInactive.
           \en Fill a polygonal object hole.
               Some inactive elements can appear during the operation.
               They can be explicitly removed by the DeleteInactive function afterwards. \~
  \param[in] params   - \ru Параметры заполнения.
                        \en Filling parameters. \~
  \param[in-out] mesh - \ru Полигональный объект.
                        \en A polygonal object. \~
  \return \ru Код результата операции.
          \en Returns the operation result code. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/ // ---
MATH_FUNC( MbResultType ) FillPolymeshHole( const MbFillPolymeshHoleParams & params, MbPolymesh & mesh );


//------------------------------------------------------------------------------
/** \brief \ru Рассчитать нормали в вершинах полигонального объекта.
           \en Calculate normal vectors at the vertices of a polygonal object. \~
  \details \ru Рассчитать нормали в вершинах полигонального объекта.
               В результате операции у полигонального объекта появится атрибут нормалей, если его не было,
               и обновится, если был.
           \en Calculate normal vectors at the vertices of a polygonal object.
               As a result of the operation, the polygonal object will have a attribute with normals if it did not exist, 
               and will be updated if it did. \~
  \param[in] params   - \ru Параметры расчета нормалей.
                        \en Parameters for calculation of normal vectors. \~
  \param[in-out] mesh - \ru Полигональный объект.
                        \en A polygonal object. \~
  \return \ru Код результата операции.
          \en Returns the operation result code. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/ // ---
MATH_FUNC( MbResultType ) CalculatePolymeshNormals( const MbPolymeshNormalsParams & params, MbPolymesh & mesh );


//------------------------------------------------------------------------------
/** \brief \ru Обрезать полигональный объект набором кривых.
           \en Cut a polygonal object by a set of curves. \~
  \details \ru Обрезать полигональный объект набором кривых.
               В результате операции в полигональном объекте могут появиться неактивные элементы.
               Впоследствии их можно удалить вручную функцией DeleteInactive.
           \en Cut a polygonal object by a set of curves.
               Some inactive elements can appear during the operation.
               They can be explicitly removed by the DeleteInactive function afterwards. \~
  \param[in] params   - \ru Параметры обрезки полигонального объекта набором кривых.
                        \en Parameters for cutting a polymesh with a set of curves. \~
  \param[out] results - \ru Результаты обрезки полигонального объекта набором кривых.
                        \en Results of cutting a polymesh with a set of curves. \~
  \param[in-out] mesh - \ru Полигональный объект.
                        \en A polygonal object being projected on. \~
  \return \ru Код результата операции, rt_Success в случае успеха операции.
          \en Returns the operation result code, rt_Success if an operation is successful. \n
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development.\~
*/ // ---
MATH_FUNC( MbResultType ) CutPolymeshByCurves( const MbCurvePolymeshProjectParams & params, MbPolymeshProjectResults & results, MbPolymesh & mesh );


//------------------------------------------------------------------------------
/** \brief \ru Выполнить эквидистантное смещение цепочек ребер по поверхности полигонального объекта.
           \en Offset polymesh edge paths. \~
  \details \ru Выполнить эквидистантное смещение цепочек ребер по поверхности полигонального объекта.
               В результате операции в полигональном объекте могут появиться неактивные элементы.
               Впоследствии их можно удалить вручную функцией DeleteInactive.
           \en Offset polymesh edge paths.
               Some inactive elements can appear during the operation.
               They can be explicitly removed by the DeleteInactive function afterwards. \~
  \param[in] params   - \ru Параметры оффсета цепочки ребер полигонального объекта.
                        \en Parameters for polymesh edge paths offsetting. \~
  \param[out] results - \ru Результаты оффсета цепочки ребер полигонального объекта
                        \en Results of polymesh edge paths offsetting. \~
  \param[in-out] mesh - \ru Полигональный объект.
                        \en A polygonal object. \~
  \return \ru Код результата операции, rt_Success в случае успеха операции.
          \en Returns the operation result code, rt_Success if an operation is successful. \n
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development.\~
*/ // ---
MATH_FUNC( MbResultType ) OffsetPolymeshPath( const MbPathPolymeshOffsetParams & params, MbPolymeshProjectResults & results, MbPolymesh & mesh );


//------------------------------------------------------------------------------
/** \brief \ru Булева операция на двух полигональных объектах.
           \en Boolean operation for two polygonal objects. \~
  \details \ru Булева операция на двух полигональных объектах.
               Поддерживаются сетки, содержащие только треугольные фасеты.
               Первый полигональный объект будет содержать сетку - результат операции.
               Второй полигональный объект будет невалиден после операции.
           \en Boolean operation for two polygonal objects. \~
               Supports meshes with triangular facets only.
               The first polygonal object will contain the polymesh with boolean operation result.
               The second polygonal object will be invalid after operation.
  \param[in] params - \ru Параметры операции.
                      \en The parameters. \~
  \param[out] results - \ru Результаты операции.
                        \en The results. \~
  \param[in-out] mesh0 - \ru Первый полигональный объект.
                         \en The first polygonal object. \~
  \param[in-out] mesh1 - \ru Второй полигональный объект.
                         \en The second polygonal object. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \warning \ru В разработке.
           \en Under development.\~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC( MbResultType ) PolymeshBoolean( const MbPolymeshBooleanParams & params, MbPolymeshBooleanResults & results, MbPolymesh & mesh0, MbPolymesh & mesh1 );

#endif // __ACTION_POLYMESH_H
