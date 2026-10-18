////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Функции работы с кривыми, поверхностями, оболочками, телами.
         \en Functions for operating with curves, surfaces, shells and solids. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ACTION_GENERAL_H
#define __ACTION_GENERAL_H

#include <math_define.h>
#include <templ_rp_array.h>
#include <op_shell_parameter.h>
#include <op_swept_parameter.h>
#include <mb_operation_result.h>

class   MbPlanarGrid;
class   MbGrid;
struct  MbShellsIntersectionData;
class   MbShellProjectionParams;
class   IProgressIndicator;

//------------------------------------------------------------------------------
/** \brief \ru Проверить оболочку на предмет разделения на отдельные части.
           \en Check if the shell can be subdivided into separate parts. \~
  \details \ru Проверить замкнутую оболочку на предмет разделения на отдельные части с анализом вложенности. \n
           \en Check if the closed shell can be subdivided into separate parts with inclusion analysis. \n \~
  \param[in] shell - \ru Исходная оболочка.
                     \en The initial shell. \~
  \param[in] checkNesting - \ru Включить анализ вложенности оболочек.
                            \en Do nesting analysis of shells. \~
  \result \ru Возвращает true, если оболочка состоит из нескольких частей.
          \en Returns 'true' if the shell consists of several parts. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) IsMultiShell( const MbFaceShell * shell, bool checkNesting = true );


//------------------------------------------------------------------------------
/** \brief \ru Отделить части оболочки.
           \en Separate parts from a shell. \~
  \details \ru Отделить части оболочки без анализа вложенности.
    Если sort == true, то наибольшая часть оболочки останется в исходной оболочке, 
    а отделившиеся от неё части будут сложены в parts с сортировкой по убыванию габарита. \n
           \en Separate parts from a shell without inclusion analysis.
    If 'sort' == 'true', the greatest part of the shell will remain in the initial shell, 
    separated parts will be collected in array 'parts' sorted by bounding box size in descending order. \n \~
  \param[in]  shell - \ru Исходная оболочка.
                      \en The initial shell. \~
  \param[out] parts - \ru Оболочки, полученные из shell.
                      \en The shells separated from 'shell'. \~
  \param[in]  sort  - \ru Выполнять ли сортировку частей оболочки по убыванию габарита?
                      \en If 'sort' == true, the parts separated from the initial shell will be sorted by bounding box size in descending order. \~
  \param[out] partIndices - \ru Индексы позиций частей оболочки при разделении на части до сортировки по габариту (количество индексов - количество частей parts + 1). Последний индекс - индекс части shell.
                            \en Positions' indices of shell parts when dividing into parts before sorting by size (number of indices is number of parts + 1). The last index is the index of the "shell". \~
  \result \ru Возвращает количество оболочек в parts.
          \en Returns number of shells in 'parts'. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (size_t) DetachShells( MbFaceShell & shell, RPArray<MbFaceShell> & parts, bool sort, c3d::IndicesVector * partIndices = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Отделить части оболочки.
           \en Separate parts from a shell. \~
  \details \ru Отделить части оболочки без анализа вложенности.
    Если sort == true, то наибольшая часть оболочки останется в исходной оболочке, 
    а отделившиеся от неё части будут сложены в parts с сортировкой по убыванию габарита. \n
           \en Separate parts from a shell without inclusion analysis.
    If 'sort' == 'true', the greatest part of the shell will remain in the initial shell, 
    separated parts will be collected in array 'parts' sorted by bounding box size in descending order. \n \~
  \param[in]  shell - \ru Исходная оболочка.
                      \en The initial shell. \~
  \param[out] parts - \ru Оболочки, полученные из shell.
                      \en The shells separated from 'shell'. \~
  \param[in]  sort  - \ru Выполнять ли сортировку частей оболочки по убыванию габарита?
                      \en If 'sort' == true, the parts separated from the initial shell will be sorted by bounding box size in descending order. \~
  \param[out] partIndices - \ru Индексы позиций частей оболочки при разделении на части до сортировки по габариту (количество индексов - количество частей parts + 1). Последний индекс - индекс части shell.
                            \en Positions' indices of shell parts when dividing into parts before sorting by size (number of indices is number of parts + 1). The last index is the index of the "shell". \~
  \result \ru Возвращает количество оболочек в parts.
          \en Returns number of shells in 'parts'. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (size_t) DetachShells( MbFaceShell & shell, c3d::ShellsVector & parts, bool sort, c3d::IndicesVector * partIndices = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Отделить части оболочки.
           \en Separate parts from a shell. \~
  \details \ru Отделить части оболочки без анализа вложенности.
    Если sort == true, то наибольшая часть оболочки останется в исходной оболочке, 
    а отделившиеся от неё части будут сложены в parts с сортировкой по убыванию габарита. \n
           \en Separate parts from a shell without inclusion analysis.
    If 'sort' == 'true', the greatest part of the shell will remain in the initial shell, 
    separated parts will be collected in array 'parts' sorted by bounding box size in descending order. \n \~
  \param[in]  shell - \ru Исходная оболочка.
                      \en The initial shell. \~
  \param[out] parts - \ru Оболочки, полученные из shell.
                      \en The shells separated from 'shell'. \~
  \param[in]  sort  - \ru Выполнять ли сортировку частей оболочки по убыванию габарита?
                      \en If 'sort' == true, the parts separated from the initial shell will be sorted by bounding box size in descending order. \~
  \param[out] partIndices - \ru Индексы позиций частей оболочки при разделении на части до сортировки по габариту (количество индексов - количество частей parts + 1). Последний индекс - индекс части shell.
                            \en Positions' indices of shell parts when dividing into parts before sorting by size (number of indices is number of parts + 1). The last index is the index of the "shell". \~
  \result \ru Возвращает количество оболочек в parts.
          \en Returns number of shells in 'parts'. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (size_t) DetachShells( MbFaceShell & shell, c3d::ShellsSPtrVector & parts, bool sort, c3d::IndicesVector * partIndices = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Отделить части оболочки.
           \en Separate parts from a shell. \~
  \details \ru Отделить части оболочки без анализа вложенности.
    Исходная оболочка всегда остаётся неизменённой.
    Если исходная оболочка распадается на части, то все части складываются в parts. \n
           \en Separate parts from a shell without inclusion analysis.
    The initial shell always remains unchangeable.
    If the initial shell is decomposed, all the parts are put into array 'parts'. \n \~
  \param[in]  shell - \ru Исходная оболочка.
                      \en The initial shell. \~
  \param[out] parts - \ru Оболочки, полученные из shell.
                      \en The shells separated from 'shell'. \~
  \result \ru Возвращает количество оболочек в parts.
          \en Returns number of shells in 'parts'. \~
  \ingroup Algorithms_3D
*/
// ---
template <class ShellsVector>
size_t CreateShells( const MbFaceShell & shell, ShellsVector & parts, bool sort = true )
{
  c3d::ShellSPtr outer( new MbFaceShell( shell ) ); // new shell on the same faces (новая оболочка с теми же гранями)

  c3d::IndicesVector * partIndices = nullptr;

  if ( ::DetachShells( *outer, parts, sort, partIndices ) > 0 ) {
    parts.push_back( outer );
    ::DetachItem( outer );
  }

  return parts.size();
}


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку выдавливанием плоских контуров.
           \en Create a shell by extrusion of planar contours. \~
  \details \ru Построить оболочку выдавливанием плоских контуров. \n
           \en Create a shell by extrusion of planar contours. \n \~
  \param[in] surface - \ru Поверхность контуров.
                       \en A surface that contains the contours. \~
  \param[in] contours - \ru Набор двумерных контуров.
                        \en A set of planar contours. \~
  \param[in] direction - \ru Направление выдавливания.
                         \en An extrusion direction. \~
  \param[in] params - \ru Параметры построения.
                      \en Parameters of a shell creation. \~
  \param[in] operNames - \ru Именователь операции.
                         \en An object defining names generation in the operation. \~
  \param[in] cNames - \ru Набор именователей контуров.
                      \en A set of objects defining names of the contours. \~
  \param[out] result - \ru Результат операции - оболочка.
                       \en Result of the operation - a shell. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbResultType) ExtrusionShell( const MbSurface &             surface,
                                               RPArray<MbContour> &    contours,
                                         const MbVector3D &            direction,
                                         const ExtrusionValues &       params,
                                         const MbSNameMaker &          operNames,
                                               RPArray<MbSNameMaker> & cNames,
                                               MbFaceShell *&          result );


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку вращением плоских контуров.
           \en Create a shell by revolution of planar contours. \~
  \details \ru Построить оболочку вращением плоских контуров. \n
           \en Create a shell by revolution of planar contours. \n \~
  \param[in] surface - \ru Поверхность контуров.
                       \en A surface that contains the contours. \~
  \param[in] contours - \ru Набор двумерных контуров.
                        \en A set of planar contours. \~
  \param[in] axis - \ru Ось вращения.
                    \en Rotation axis. \~
  \param[in] params - \ru Параметры построения.
                      \en Parameters of a shell creation. \~
  \param[in] operNames - \ru Именователь операции.
                         \en An object defining names generation in the operation. \~
  \param[in] cNames - \ru Набор именователей контуров.
                      \en A set of objects defining names of the contours. \~
  \param[out] result - \ru Результат операции - оболочка.
                       \en Result of the operation - a shell. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbResultType) RevolutionShell( const MbSurface &        surface,
                                          RPArray<MbContour> &     contours,
                                          const MbAxis3D &         axis,
                                          const RevolutionValues & params,
                                          const MbSNameMaker &     operNames,
                                          RPArray<MbSNameMaker> &  cNames,
                                          MbFaceShell *&           result );


//------------------------------------------------------------------------------
/** \brief \ru Объединить компланарные грани.
           \en Unite complanar faces. \~
  \details \ru Объединить компланарные грани оболочки и проверить оболочку. \n
           \en Unite complanar faces of a shell and validate the shell. \n \~
  \param[in] shell - \ru Модифицируемая оболочка.
                     \en A shell to be modified. \~
  \param[in] nameMaker - \ru Именователь операции.
                         \en An object defining names generation in the operation. \~
  \param[in] checkBaseSurfaces - \ru Найти и устранить общие поверхности-подложки в гранях.
                                 \en Find and eliminate common underlying surfaces of faces \~
  \return \ru Возвращает true, если оболочка была успешно изменена.
          \en Returns 'true' if the shell has been successfully modified. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) UnifyOwnComplanarFaces(       MbFaceShell &  shell, 
                                         const MbSNameMaker & nameMaker,
                                               bool           checkBaseSurfaces );


//------------------------------------------------------------------------------
/** \brief \ru Захватить грани одним из способов.
           \en Capture the faces in one of proposed methods. \~
  \details \ru Захватить грани одним из способов распространения по связной оболочке. \n
           \en Capture the faces in one of methods of propagation in connected shell. \n \~
  \param[in]      fp - \ru Способ захвата граней.
                       \en A method of capturing the faces. \~
  \param[in,out]  face_set - \ru Набор граней.
                             \en A set of faces. \~
  \param[in]      dir - \ru Направление уклона.
                        \en A direction of inclination. \~
  \warning \ru Вспомогательная функция операции DraftSolid.
           \en An auxillary function of operation DraftSolid. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (void) FacePropagate( const MbeFacePropagation fp, 
                                      RPArray<MbFace> &  face_set, 
                                const MbVector3D &       dir );


//------------------------------------------------------------------------------
/** \brief \ru Масштабировать каверны литейной формы.
           \en Scale cavities of a mold. \~
  \details \ru Масштабировать каверны литейной формы относительно неподвижной точки. \n
           \en Scale cavities of a mold relative to a fixed point. \n \~
  \param[in,out] solids - \ru Модифицируемые тела.
                          \en The solids to be modified. \~
  \param[in]  fixedPoint - \ru Неподвижная точка масштабирования.
                           \en The fixed point of scaling. \~
  \param[in]  deltaX - \ru Относительное приращение размера по направлению X.
                       \en Relative increment of size in X-direction. \~ 
  \param[in]  deltaY - \ru Относительное приращение размера по направлению Y.
                       \en Relative increment of size in Y-direction. \~
  \param[in]  deltaZ - \ru Относительное приращение размера по направлению Z.
                       \en Relative increment of size in Z-direction. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (void) MouldCavitySolids( RPArray<MbSolid> & solids, 
                                    MbCartPoint3D *    fixedPoint,
                                    double             deltaX, 
                                    double             deltaY, 
                                    double             deltaZ );


//------------------------------------------------------------------------------
/** \brief \ru Проверить тела на пересечение.
           \en Check intersection of solids. \~
  \details \ru Проверить тела на пересечение без уточнения характера пересечения \n
           (проверяем до первого пересечения граней). \n
           \en Check if solids intersect each other without definition of intersection type \n
           (check until the first intersection is detected). \n \~
  \param[in] solid1 - \ru Первое тело.
                      \en The first solid. \~
  \param[in] solid2 - \ru Второе тело.
                      \en The second solid. \~
  \return \ru Возвращает true, если найдено хотя бы одно пересечение.
          \en Returns 'true' if at least one intersection is detected. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) IsSolidsIntersection( const MbSolid & solid1, const MbSolid & solid2, const MbSNameMaker & snMaker );


//------------------------------------------------------------------------------
/** \brief \ru Определить пересечение тел в сборке.
           \en Check intersection of solids in an assembly. \~
  \details \ru Определить пересечение тел в сборке. \n
           \en Check intersection of solids in an assembly. \n \~
  \param[in] solid1 - \ru Первое тело в локальной системе координат (ЛСК).
                      \en The first solid in local coordinate system (LCS). \~
  \param[in] matr1 - \ru Матрица преобразования в глобальную СК (ГСК).
                     \en Matrix of transformation to the global coordinate system (GCS). \~
  \param[in] solid2 - \ru Второе тело в ЛСК.
                      \en The second solid in LCS. \~
  \param[in] matr2 - \ru Матрица преобразования в ГСК.
                     \en Matrix of transformation to GCS. \~
  \param[in] checkTangent - \ru Считать касания пересечениями.
                            \en Consider tangencies as intersections. \~
  \param[in] getIntersectionSolids - \ru Получить не касательные пересечения в виде тел.
                                     \en Get non-tangent intersections in the form of bodies. \~
  \param[in] checkTouchPoints - \ru Искать точки касания.
                                \en Find touch points. \~
  \param[out] intData - \ru Информация о пересечении двух тел.
                            Если одно из пересекаемых тел не замкнуто, то
                            тела, полученные в результате, могут быть не валидны для операций.
                        \en Information about two solids intersection.
                            If one of the intersecting bodies is not closed, then
                            the resulting bodies may not be valid for operations. \~
  \return \ru Возвращает true, если найдено хотя бы одно пересечение.
          \en Returns 'true' if at least one intersection is detected. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) IsSolidsIntersection( const MbSolid & solid1, const MbMatrix3D & matr1,
                                       const MbSolid & solid2, const MbMatrix3D & matr2,
                                       bool checkTangent, // \ru Считать касания пересечениями \en Consider tangencies as intersections 
                                       bool getIntersectionSolids, // \ru Получить не касательные пересечения в виде тел \en Get non-tangency intersection as solids 
                                       bool checkTouchPoints, // \ru Искать точки касания \en Find touch points 
                                       RPArray<MbShellsIntersectionData> & intData );

//------------------------------------------------------------------------------
/** \brief \ru Определить минимальное расстояние между телами в сборке.
           \en Determine the minimum distance between solids in an assembly. \~
  \details \ru Определить минимальное расстояние между телами в сборке. В случае пересечения или касания тел возвращается нулевая дистанция.\n
    При многократном использовании первого тела следует установить isMultipleUseSolid1 = true, иначе false. Аналогично для второго тела.\n
           \en Determine the minimum distance between solids in an assembly. In case of intersection or tangent of the shells returns to zero distance.\n
    With multiple use of the first body should be set isMultipleUseSolid1 = true, else false. Similarly for the second body.\n \~
  \param[in] solid1 - \ru Первое тело в локальной системе координат (ЛСК).
                      \en The first solid in local coordinate system (LCS). \~
  \param[in] matr1  - \ru Матрица преобразования в глобальную СК (ГСК).
                      \en Matrix of transformation to the global coordinate system (GCS). \~
  \param[in] isMultipleUseSolid1 - \ru Множественное использование первого тела.
                                   \en Multiple use of the first body. \~
  \param[in] solid2 - \ru Второе тело в ЛСК.
                      \en The second solid in LCS. \~
  \param[in] matr2  - \ru Матрица преобразования в ГСК.
                      \en Matrix of transformation to GCS. \~
  \param[in] isMultipleUseSolid2 - \ru Множественное использование второго тела.
                                   \en Multiple use of the second body. \~
  \param[in] lowerLimitDistance  - \ru Минимальное допустимое расстояние.
                                   \en Minimum allowed distance. \~
  \param[in] tillFirstLowerLimit - \ru Искать до первого найденного удовлетворяющего минимально допустимому расстоянию.
                                   \en Search until the first found that satisfies the minimum acceptable distance. \~
  \param[out] shellsDistanceData - \ru Информация о расстоянии между телами.
                                   \en Information about the distance between solids. \~
  \return \ru Возвращает true, если определено хотя бы одно расстояние.
          \en Returns 'true' if at least one distance is obtained. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) MinimumSolidsDistance( const MbSolid & solid1, const MbMatrix3D & matr1, bool isMultipleUseSolid1,
                                        const MbSolid & solid2, const MbMatrix3D & matr2, bool isMultipleUseSolid2,
                                        double lowerLimitDistance, bool tillFirstLowerLimit,
                                        std::vector<MbShellsDistanceData> & shellsDistanceData );


//------------------------------------------------------------------------------
/** \brief \ru Классифицировать набор тел относительно другого набора тел.
           \en Classify a set of solids relative to another set of solids. \~
  \details \ru Классифицировать набор тел относительно другого набора тел. Тела находятся в одной системе координат. \n
    Каждое классифицируемое тело в результате имеет 3 состояния: 1) Находится вне относительного набора тел; \n
    2) Содержится только в одном теле, тогда есть информация об индексе относительного тела; \n
    3) Во всех остальных случаях тело пересекается с относительным набором тел. \n
           \en Classify a set of solids relative to another set of solids. The solids are in the same coordinate system. \n
    Each classified solids as result has 3 states: 1) It is outside relative set of bodies; \n
    2) Contained in only one solid, then there is information about the index of the relative body; \n
    3) In all other cases, solid intersects with relative set of solids. \n \~
  \param[in] solids - \ru Классифицируемый набор тел.
                      \en The classified set of solids. \~
  \param[in] relativeSolids - \ru Набор тел относительно которых осуществляется классификация.
                              \en A set of solids relative to which classification is performed. \~
  \param[out] info - \ru Информация о нахождении тел относительно другого набора тел.
                     \en Information about finding solids relative to another set of solids. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC( void ) MultipleSolidsClassification( const RPArray<MbSolid>        & solids,
                                                const RPArray<MbSolid>        & relativeSolids,
                                                      std::vector<MbUnitInfo> & info );


//------------------------------------------------------------------------------
/** \brief \ru Найти расстояния от контура на плоскости до поверхности.
           \en Find the distances from a contour on a plane to a surface. \~
  \details \ru Найти расстояния от контура на плоскости до поверхности. \n
    Прямое направление - это направление оси Z системы координат двумерной кривой. \n
    Расстояние в прямом направлении найдено, если значение не отрицательное. \n
    Расстояние в обратном направлении найдено, если значение не положительное. \n
           \en Find the distances from a contour on a plane to a surface. \n
    A forward direction is a direction of Z-axis of two-dimensional curve coordinate system. \n
    The distance in a forward direction is found if the value is non-negative. \n
    The distance in a backward direction is found if the value is non-positive. \n \~
  \param[in]  pl - \ru Система координат двумерной кривой.
                   \en A coordinate system of two-dimensional curve. \~
  \param[in]  curve - \ru Двумерная кривая.
                      \en A two-dimensional curve. \~
  \param[in]  surf - \ru Поверхность, до которой проводится поиск расстояний.
                     \en A surface to measure the distances up to. \~
  \param[out] lPlus - \ru Расстояние в прямом направлении.
                      \en The distance in a forward direction. \~
  \param[out] lMinus - \ru Расстояние в обратном направлении.
                       \en The distance in a backward direction. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (void) GetDistanceToSurface( const MbPlacement3D & pl, 
                                       const MbCurve *       curve, 
                                       const MbSurface *     surf,
                                             double &        lPlus, 
                                             double &        lMinus );


//------------------------------------------------------------------------------
/** \brief \ru Создание поверхностей сечения выдавливания плоского контура.
           \en Create cutter surfaces for extrusion of planar contours. \~
  \details \ru Создание поверхностей сечения выдавливания плоского контура и определение направлений выдавливаний. \n
           \en Create cutter surfaces for extrusion of planar contours and define directions of extrusions. \n \~
  \param[in] surface - \ru Поверхность контуров.
                       \en A surface that contains the contours. \~
  \param[in] contours - \ru Набор двумерных контуров.
                        \en A set of planar contours. \~
  \param[in] direction - \ru Направление выдавливания.
                         \en An extrusion direction. \~
  \param[in] params - \ru Параметры построения.
                      \en Parameters of a shell creation. \~
  \param[in] version   - \ru Версия построения.
                         \en The version of construction. \~
  \param[out] resType - \ru Код результата операции.
                        \en Operation result code. \~
  \param[out] surfAndDir- \ru Результат операции - поверхности и направление относительно direction.
                           \en Result of the operation - surfaces and directions relative to parameter direction. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (void) CreateExtrusionCutSurfaces( const MbSurface &                    surface, 
                                             const c3d::PlaneContoursSPtrVector & contours,
                                             const MbVector3D &                   direction,
                                                   ExtrusionValues &              params, 
                                                   VERSION                        version, 
                                                   MbResultType &                 resType, 
                                                   std::vector< std::pair<c3d::SurfaceSPtr,int>> & surfAndDir );


//------------------------------------------------------------------------------
/** \brief \ru Найти расстояния от контура на поверхности до габаритного куба оболочки.
           \en Find the distances from a contour on a surface to the bounding box of a shell. \~
  \details \ru Найти расстояния от контура на поверхности до габаритного куба оболочки. \n
    Расстояние в прямом направлении найдено, если значение не отрицательное. \n
    Расстояние в обратном направлении найдено, если значение не положительное. \n
           \en Find the distances from a contour on a surface to the bounding box of a shell. \n
    The distance in a forward direction is found if the value is nonnegative. \n
    The distance in a backward direction is found if the value is non-positive. \n \~
  \param[in] surface - \ru Поверхность, на которой лежит двумерная кривая.
                       \en A surface that contains the two-dimensional curve. \~
  \param[in] direction - \ru Направление поиска (выдавливания)
                         \en A direction of the distance calculation (an extrusion direction). \~
  \param[in] curve - \ru Двумерная кривая, лежащая на поверхности surface.
                     \en A two-dimensional curve on surface 'surface'. \~
  \param[in] cube - \ru Габаритный куб оболочки.
                    \en The bounding box of the shell. \~
  \param[out] lPlus - \ru Расстояние в прямом направлении.
                      \en The distance in a forward direction. \~
  \param[out] lMinus - \ru Расстояние в обратном направлении.
                       \en The distance in a backward direction. \~
  \param[out] resType - \ru Код результата операции.
                        \en Operation result code. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (void) GetDistanceToCube( const MbSurface &    surface, 
                                    const MbVector3D &   direction, 
                                    const MbCurve &      curve, 
                                    const MbCube &       cube, 
                                          double &       lPlus, 
                                          double &       lMinus,
                                          MbResultType & resType );


//------------------------------------------------------------------------------
/** \brief \ru Найти расстояния от набора кривых на поверхности до габаритного куба оболочки.
           \en Find the distances from curves on a surface to the bounding box of a shell. \~
  \details \ru Найти расстояния от набора кривых на поверхности до габаритного куба оболочки. \n
    Расстояние в прямом направлении найдено, если значение не отрицательное. \n
    Расстояние в обратном направлении найдено, если значение не положительное. \n
           \en Find the distances from curves on a surface to the bounding box of a shell. \n
    The distance in a forward direction is found if the value is nonnegative. \n
    The distance in a backward direction is found if the value is nonpositive. \n \~
  \param[in] surface - \ru Поверхность, на которой лежат двумерные кривые.
                       \en A surface that contains two-dimensional curves. \~
  \param[in] direction - \ru Направление поиска (выдавливания)
                         \en A direction of the distance calculation (an extrusion direction). \~
  \param[in] curves - \ru Набор двумерных кривых на поверхности surface.
                      \en A set of two-dimensional curves on the surface 'surface'. \~
  \param[in] cube - \ru Габаритный куб оболочки.
                    \en The bounding box of the shell. \~
  \param[out] lPlus - \ru Расстояние в прямом направлении.
                      \en The distance in a forward direction. \~
  \param[out] lMinus - \ru Расстояние в обратном направлении.
                       \en The distance in a backward direction. \~
  \param[out] resType - \ru Код результата операции.
                        \en Operation result code. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (void) GetDistanceToCube( const MbSurface        & surface, 
                                    const MbVector3D       & direction, 
                                    const RPArray<MbCurve> & curves, 
                                    const MbCube           & cube, 
                                          double           & lPlus, 
                                          double           & lMinus,
                                          MbResultType     & resType );


//------------------------------------------------------------------------------
/** \brief \ru Найти расстояния от плоскости до габаритного куба оболочки.
           \en Find the distances from a plane to the bounding box of a shell. \~
  \details \ru Найти расстояния от плоскости до габаритного куба оболочки. \n
    Расстояние в прямом направлении найдено, если значение не отрицательное. \n
    Расстояние в обратном направлении найдено, если значение не положительное. \n
    Использует расчет габарита относительно локальной системы координат. \n
    Если система координат плоскости лежит вне габаритного куба, \n
    то при взведенном флаге findMax ищется максимальное расстояние до куба. \n
           \en Find the distances from a plane to the bounding box of a shell. \n
    The distance in a forward direction is found if the value is non-negative. \n
    The distance in a backward direction is found if the value is non-positive. \n
    Calculation of the distance relative to the local coordinate system is used. \n
    If a plane coordinate system is out of the bounding box, \n
    then if the flag 'findMax' is set to 'true', the maximal distance to bounding box is calculated. \n \~
  \param[in] pl - \ru Система координат плоскости.
                  \en The plane coordinate system. \~
  \param[in] shell - \ru Целевая оболочка.
                     \en A target shell. \~
  \param[out] dPlus - \ru Расстояние в прямом направлении.
                      \en The distance in a forward direction. \~
  \param[out] dMinus - \ru Расстояние в обратном направлении.
                       \en The distance in a backward direction. \~
  \param[in] findMax - \ru Искать максимальное расстояние до габаритного куба.
                       \en The maximal distance to bounding box is to be calculated. \~
  \return \ru Возвращает true, если найдено хотя бы одно из расстояний.
          \en Returns 'true' if at least one of the distances has been found. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) GetDistanceToCube( const MbPlacement3D & pl, 
                                    const MbFaceShell *   shell,
                                          double &        dPlus, 
                                          double &        dMinus,
                                          bool            findMax = true ); // \ru Искать максимальное \en Find the maximal distance 


//------------------------------------------------------------------------------
/** \brief \ru Найти расстояния/углы от контура до куба или до поверхности.
           \en Find the distances/angles from a contour to a bounding box or to a surface. \~
  \details \ru Найти расстояния/углы от контура до куба или до поверхности. \n
    Нужно учесть уклон в двух направлениях. \n
           \en Find the distances/angles from a contour to a bounding box or to a surface. \n
    The inclination in two directions is to be considered. \n \~
  \param[in] curve - \ru Кривая.
                     \en A curve. \~
  \param[in] direction - \ru Направление выдавливания.
                         \en An extrusion direction. \~
  \param[in] axis - \ru Ось вращения.
                    \en Rotation axis. \~
  \param[in] rotation - \ru Вращение (true) или выдавливание (false)
                        \en Rotation (true) or extrusion (false) \~
  \param[in] operationDirection - \ru Вперед (true) или назад (false)
                                  \en Forward (true) or backward (false) \~
  \param[in] toCube - \ru До куба, если указатель ненулевой
                      \en Up to a cube if the pointer is not null. \~
  \param[in] toSurface - \ru До поверхности, если указатель ненулевой.
                         \en Up to a surface if the pointer is not null. \~
  \param[in,out] params:
\ru Должны быть заданы параметры: \n
      params.side1.rake - Уклон в направлении direction (для плоской образующей). \n
      params.side2.rake - Уклон в направлении direction, обратном direction (для плоской образующей). \n
      params.thikness1 - Толщина стенки в прямом направлении 
                         (в положительном направлении нормали объекта (грани, поверхности, плоскости кривой)). \n
      params.thikness2 - Толщина стенки в обратном направлении. \n
    Заполняются параметры: \n
      params.side1.scalarValue - Расстояние выдавливания в направлении direction (если operationDirection == true), 
                                 иначе обратном. \n
      params.side2.scalarValue - Расстояние выдавливания в направлении, обратном direction (если operationDirection == true), 
                                 иначе в прямом. \n
\en The following parameters should be defined: \n
      params.side1.rake - The inclination in direction 'direction' (for a planar generatrix). \n
      params.side2.rake - The inclination in direction opposite to 'direction' (for a planar generatrix). \n
      params.thikness1 - Wall thickness in forward direction. 
                         (in the positive direction of the normal of an object (a face, a surface, the plane of a curve)). \n
      params.thikness2 - The wall thickness in the backward direction. \n
    The output parameters: \n
      params.side1.scalarValue - The extrusion distance in the direction 'direction' (if 'operationDirection' == true), 
                                 else in the opposite direction. \n
      params.side2.scalarValue - The extrusion distance in the direction opposite to 'direction' (if 'operationDirection' == true), 
                                 else in the forward direction. \n \~
  \param[out] resType - \ru Код результата операции.
                        \en Operation result code. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (void) GetRangeToCubeOrSurface( const MbCurve3D &           curve,
                                          const MbVector3D &          direction,
                                          const MbAxis3D &            axis,
                                          const bool                  rotation,
                                                bool                  operationDirection,
                                          const MbCube *              toCube,
                                          const MbSurface *           toSurface,
                                                SweptValuesAndSides & params,
                                                MbResultType &        resType );


//------------------------------------------------------------------------------
/** \brief \ru Найти ближайшие тела при выдавливании с опцией "до ближайшего объекта".
           \en Find the nearest solids while extruding with option 'up to the nearest object'. \~
  \details \ru Найти ближайшие тела при выдавливании с опцией "до ближайшего объекта". \n
    возвращает номера ближайших тел ближайших тел в прямом и обратном направлении.
           \en Find the nearest solids while extruding with option 'up to the nearest object'. \n
    returns numbers of the nearest solids in the forward and the backward direction. \~
  \param[in]  params - \ru Параметры операции.
                       \en Operation parameters. \~
  \param[out] result - \ru Результат операции с номерами ближайших тел.
                       \en The result of the operation with the numbers of the nearest solids. \~
  \return \ru Возвращает true, если найдено тело хотя бы в одном из направлений.
          \en Returns 'true' if a solid is found in at least one of directions. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) GetNearestSolid( const MbNearestSolidParams & params,
                                        MbNearestSolidResult & result ); 


//------------------------------------------------------------------------------
/** \brief \ru Проверить оболочку или тело, состоящее из NURBS поверхностей.
           \en Check a shell or a solid that consists of NURBS surfaces. \~
  \details \ru Проверить корректность оболочки или тела, состоящего из NURBS поверхностей. \n
           \en Check the correctness of a shell or a solid that consists of NURBS surfaces. \n \~
  \param[in] params  - \ru Исходные параметры операции.
                       \en Initial parameters of the operation. \~
  \param[in] nsSolid - \ru Тело - результат операции.
                       \en A solid - the result of the operation. \~
  \param[in] progBar - \ru Индикатор прогресса выполнения операции.
                       \en A progress indicator of the operation. \~
  \return \ru Возвращает rt_Success, если тело успешно прошло проверку.
          \en Returns rt_Success if the solid has successfully passed the validation. \~
  \warning \ru Проверочная функция операции NurbsSurfacesShell.
           \en A checking function of the operation NurbsSurfacesShell. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (MbResultType) CheckNurbsShell( const NurbsSurfaceValues & params,
                                          const MbSolid &            nsSolid,
                                                IProgressIndicator * progBar );


//------------------------------------------------------------------------------
/** \brief \ru Положить эскиз в массив усекающих объектов.
           \en Add a sketch to the array of truncating objects. \~
  \details \ru Положить эскиз в массив усекающих объектов путем создания пространственных кривых. \n
           \en Add a sketch to the array of truncating objects by creation of spatial curves. \n \~
  \param[in] sketchPlace - \ru Локальная система координат двумерного эскиза.
                           \en A local coordinate system of two-dimensional sketch. \~
  \param[in] sketchCurves - \ru Двумерные кривые эскиза.
                            \en Two-dimensional curves of the sketch. \~
  \param[out] items - \ru Выходной массив пространственных объектов.
                      \en The output array of spatial objects. \~
  \return \ru - Возвращает true в случае добавления элементов в выходной массив.
          \en - Returns 'true' if elements are added into output array. \~
  \warning \ru Вспомогательная функция операции TruncateShell.
           \en Auxiliary function of the operation TruncateShell. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) AddTruncatingSketch( const MbPlacement3D &        sketchPlace, 
                                            RPArray<MbCurve> &     sketchCurves,
                                            RPArray<MbSpaceItem> & items );


//------------------------------------------------------------------------------
/** \brief \ru Положить кривую в массив усекающих объектов.
           \en Add a curve to the array of truncating objects. \~
  \details \ru Положить кривую в массив усекающих объектов
           (с разбором кривой на составляющие, в случае необходимости). \n
           \en Add a curve to the array of truncating objects
           (with decomposition of the curve if necessary). \n \~
  \param[in] curve - \ru Пространственная кривая.
                     \en A space curve. \~
  \param[out] items - \ru Выходной массив пространственных объектов.
                      \en The output array of spatial objects. \~
  \warning \ru Вспомогательная функция операции TruncateShell.
           \en Auxillary function of the operation TruncateShell. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (void) AddTruncatingCurve( const MbCurve3D &            curve, 
                                           RPArray<MbSpaceItem> & items );


//------------------------------------------------------------------------------
/** \brief \ru Проверить корректность вскрываемых граней для создания тонкостенного тела.
           \en Check the correctness of shelling faces for creation of a thin-walled solid. \~
  \details \ru Проверить корректность набора вскрываемых граней для создания тонкостенного тела. \n
           Удаляет из массива не подходящие для операции грани (гладко сопряженные с невыбранными гранями). \n
           \en Check the correctness of shelling faces set for creation of a thin-walled solid. \n
           Removes unsuitable for the operation faces from the array (Delete faces smoothly connected to unselected faces). \n \~
  \param[in] params - \ru Параметры тонкой стенки.
                      \en Parameters of a thin wall. \~
  \param[in,out] faces - \ru Множество вскрываемых граней тела.
                         \en An array of shelling faces of the solid. \~
  \warning \ru Вспомогательная функция операции построения тонкостенного тела.
           \en An auxiliary function of thin-walled solid construction operation. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (void) CheckShellingFaces( const SweptValues & params, RPArray<MbFace> & faces );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить компоненты проекции вектора, заданного в точке на поверхности.
           \en Calculate the components of projection of a vector defined at a point on the surface. \~
  \details \ru Вычислить компоненты x и y проекции пространственного вектора, заданного в точке на поверхности. \n
           \en Calculate x and y components of projection of a space vector defined at a point on a surface. \n \~
  \param[in] v3d - \ru Пространственный вектор.
                   \en A space vector. \~
  \param[in] surface - \ru Поверхность.
                       \en A surface. \~
  \param[in] p2d - \ru Параметрическая точка на поверхности.
                   \en A parametric point on the surface. \~
  \param[out] v2d - \ru Проекция пространственного вектора на поверхность.
                    \en The projection of the space vector on the surface. \~
  \return \ru - Возвращает true в случае успешного вычисления проекции вектора.
          \en - Returns 'true' if the vector projection has been successfully calculated. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) ProjectVectorOn( const MbVector3D & v3d, const MbSurface & surface, const MbCartPoint & p2d,
                                  MbVector & v2d );


//------------------------------------------------------------------------------
/** \brief \ru Расширить поверхность для резки тела.
           \en Extend a surface for cutting a solid. \~
  \details \ru Расширить поверхность до заданного габарита для резки тела. \n
           \en Extend a surface to a given bounding box for cutting a solid. \n \~
  \param[in,out] gabarit - \ru Желаемый габарит расширения.
                           \en A desirable bounding box of the extended surface. \~
  \param[in] surf - \ru Исходная поверхность.
                    \en The initial surface. \~
  \param[in] prolongState - \ru Состояние типа продления секущих поверхностей.
                            \en State of prolongation types of cutter surfaces. \~
  \param[in] version   - \ru Версия построения.
                         \en The version of construction. \~
  \return \ru - Возвращает расширенную поверхность, если получилось ее создать.
          \en - Returns the extended surface if it has been successfully created. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (MbSurface *) GetExtendedSurfaceCopy(       MbCube &                             gabarit, 
                                                const MbSurface &                          surf, 
                                                const MbShellCuttingParams::ProlongState & prolongState, 
                                                      VERSION                              version );// = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Построить набор граней с топологией призмы.
           \en Create a set of faces with topology of a prism. \~
  \details \ru Построить набор граней с топологией призмы. \n
           \en Create a set of faces with topology of a prism. \n \~
  \param[in] place - \ru Локальная система координат (ЛСК).
                     \en A local coordinate system (LCS). \~
  \param[in] contour - \ru Двумерный контур в ЛСК.
                       \en A two-dimensional curve in LCS. \~
  \param[in] der - \ru Направление выдавливания.
                   \en An extrusion direction. \~
  \param[in] sense - \ru Ориентация выходного массива граней как замкнутой оболочки.
                     \en An orientation of the output array of faces as a closed shell. \~
  \param[in] n - \ru Именователь.
                 \en An object for naming the new objects. \~
  \param[in,out] faces - \ru Множество созданных граней.
                             \en The array of created faces. \~
  \param[in] useAddCount - \ru Использовать количество граней initFaces на входе для именования новых граней.
                           \en The number of input faces initFaces is to be used for naming the new faces. \~
  \return \ru Возвращает true, если получилось построить грани.
          \en Returns true if new faces have been successfully created. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) CreateFaces( const MbPlacement3D &        place, 
                              const MbContour &            contour,
                              const MbVector3D &           der, 
                                    bool                   sense, 
                              const MbSNameMaker &         n,
                                    c3d::FacesSPtrVector & faces, 
                                    bool                   useAddCount = false );


//------------------------------------------------------------------------------
/** \brief \ru Оценить параметры выдавливания для ребра жёсткости.
           \en Estimate parameters of extrusion for a rib. \~
  \details \ru Оценить параметры выдавливания для построения ребра жёсткости. \n
           \en Estimate parameters of extrusion for creating a rib. \n \~
  \param[in] shell - \ru Целевая оболочка.
                     \en A target shell. \~
  \param[in] place - \ru Локальная система координат контура.
                     \en A local coordinate system of the contour. \~
  \param[in] contour - \ru Двумерный контур.
                       \en A two-dimensional contour. \~
  \param[in] index - \ru Номер сегмента контура.
                     \en A number of the contour segment. \~
  \param[out] side - \ru Сторона заполнения пространства телом ребра.
                     \en The side to place the rib on. \~
  \param[out] origin - \ru Точка.
                       \en A point. \~
  \param[out] dir3D - \ru Вектор.
                      \en A vector. \~
  \warning \ru Вспомогательная функция операции RibSolid.
           \en An auxillary function of the operation RibSolid. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (void) GetAutoReference(       MbFaceShell &            shell,
                                   const MbPlacement3D &          place,
                                   const MbContour &              contour,
                                         ptrdiff_t                index,
                                         RibValues::ExtrudeSide & side,
                                         MbCartPoint3D &          origin,
                                         MbVector3D &             dir3D );


//------------------------------------------------------------------------------
/** \brief \ru Создать кривую в параметрах поверхности.
           \en Create a curve in the parameter space of a surface. \~
  \details \ru Создать кривую в параметрах поверхности, если нужно - проекционную. \n
    Если проекционную кривую создавать не нужно, возвращает дубль двумерной кривой в параметрах поверхности. \n
    После использования кривую нужно удалить. \n
           \en Create a curve in the parameter space of a surface. The projection curve can be created if necessary. \n
    If it is not required to create the projection curve, returns a copy of two-dimensional curve in the parameter space of the surface. \n
    The curve is to be deleted after use. \n \~
  \param[in] intersectCurve - \ru Кривая пересечения.
                              \en The intersection curve \~
  \param[in] first - \ru true - Первая поверхность, false - вторая поверхность.
                     \en True - The first surface, false - the second surface. \~
  \return \ru Возвращает кривую, если ее получилось построить.
          \en Returns the curve if it has been successfully created. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (MbCurve *) GetProjCurveOnSurface( const MbSurfaceIntersectionCurve & intersectCurve, bool first );


//------------------------------------------------------------------------------
/** \brief \ru Проверить неизменность вектора кинематической направляющий в разных версиях.
           \en Check the invariance of the vector of spine direction in different versions. \~
  \details \ru Проверить, можно ли сохранить кинематическую направляющую из одной версии в другую без изменения формы. \n
           \en Check if the spine direction can be preserved between versions without any change of the shape. \n \~
  \param[in] curve - \ru Направляющая кривая.
                     \en The spine curve. \~
  \param[in] srcVersion - \ru Рабочая версия.
                          \en The current version. \~
  \param[in] dstVersion - \ru Целевая версия.
                          \en The target version. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) IsSameSpineDirection( const MbCurve3D & curve, 
                                             VERSION srcVersion, 
                                             VERSION dstVersion );// = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Классифицировать положения второго контура относительно первого.
           \en Classify the position of the second contour relative to the first one. \~
  \details \ru Классифицировать положения второго контура относительно первого: \n
    iloc_OutOfItem - снаружи, \n
    iloc_OnItem    - пересекается, \n
    iloc_InItem    - внутри. \n
           \en Classify the position of the second contour relative to the first one: \n
    iloc_OutOfItem - outside, \n
    iloc_OnItem    - intersects, \n
    iloc_InItem    - inside. \n \~
  \param[in] contour1 - \ru Первый контур.
                        \en The first contour. \~
  \param[in] contour2 - \ru Второй контур.
                        \en The second contour. \~
  \param[in] xEpsilon - \ru Погрешность по x.
                        \en Tolerance in x direction. \~
  \param[in] yEpsilon - \ru Погрешность по y.
                        \en Tolerance in y direction. \~
  \return \ru Возвращает результат классификации положения.
          \en Returns the result of classification of the relative position. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MbeItemLocation) SecondContourLocation( const MbContour & contour1, const MbContour & contour2, 
                                                   double xEpsilon, double yEpsilon );


//------------------------------------------------------------------------------
/** \brief \ru Определить, близка ли первая кривая ко второй кривой.
           \en Determine whether the first curve is close to the second curve. \~
  \details \ru Определить, близка ли кривая curve1 к кривой curve2 с заданной точностью. \n
           Близость определяется близостью точек первой кривой, полученных шаганием \n
           по кривой с заданным угловым отклонением, ко второй кривой. \n
           \en Determine whether curve 'curve1' is close to curve 'curve2' within the given precision. \n
           The proximity is defined by the closeness of points of the first curve obtained by sampling \n
           with the given turning angle to the second curve. \n \~
  \param[in] curve1 - \ru Первая кривая.
                      \en The first curve. \~
  \param[in] curve2 - \ru Вторая кривая.
                      \en The second curve. \~
  \param[in] xEpsilon - \ru Близость по x.
                        \en Proximity tolerance in x direction. \~
  \param[in] yEpsilon - \ru Близость по y.
                        \en Proximity tolerance in y direction. \~
  \param[in] devSag - \ru Максимальное угловое отклонение при шагании по кривой.
                      \en The maximal turning angle for sampling the curve. \~
  \return \ru Возвращает true, если кривые близки.
          \en Returns 'true' if the curves are close. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (bool) IsSpaceNear( const MbCurve & curve1, const MbCurve & curve2, double xEpsilon, double yEpsilon,
                              double devSag = 5.0 * Math::deviateSag );


//------------------------------------------------------------------------------
/** \brief \ru Определить, близка ли кривая к поверхности.
           \en Determine whether a curve is close to a surface. \~
  \details \ru Определить, близка ли кривая к поверхности с заданной точностью. \n
    Выполняется проверка по пробным точкам кривой, полученных шаганием по угловому отклонению. \n
           \en Determine whether a curve is close to a surface within the given tolerance. \n
    The check uses sample points of curves obtained by sampling with maximal turning angle. \n \~
    \param[in] curv - \ru Кривая.
                      \en A curve. \~
    \param[in] surf - \ru Поверхность.
                      \en A surface. \~
    \param[in] surfExt - \ru Проверять на расширенной поверхности.
                         \en Perform the check for the extended surface. \~
    \param[in] mEps - \ru Метрическая близость.
                      \en The metric proximity tolerance. \~
    \param[in] devSag - \ru Максимальное угловое отклонение при шагании по кривой.
                        \en The maximal turning angle for sampling the curve. \~
    \return \ru Возвращает true, если кривая близка к поверхности.
            \en Returns 'true' if the curve is close to the surface. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) IsSpaceNear( const MbCurve3D & curv, const MbSurface & surf, bool surfExt, 
                              double mEps, double devSag = 5.0 * Math::deviateSag );


//------------------------------------------------------------------------------
/** \brief \ru Создать грань по произвольной поверхности.
           \en Create the face on the base of arbitrary surface. \~
  \details \ru Создать грань по произвольной поверхности без самопересечений. \n
           \en Create the face on the base of arbitrary surface without selfintersections. \n \~
    \param[in] surface - \ru Поверхность.
                         \en A surface. \~
    \param[out] face - \ru Грань.
                       \en The face. \~
    \return \ru Возвращает true, если грань создана.
            \en Returns 'true' if the face was created. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) SurfaceFace( const MbSurface & surface, SPtr<MbFace> & face );


//------------------------------------------------------------------------------
/** \brief \ru Создание трёхмерной сетки по двумерной сетке.
           \en Creating a three-dimensional grid on a two-dimensional grid. \~
  \details \ru Создание трёхмерной сетки по двумерной сетке. \n
           \en Creating a three-dimensional grid on a two-dimensional grid. \n \~
    \param[in] place -       \ru Локальная система координат в трёхмерном пространстве.
                             \en Local coordinate system in three dimensional space. \~
    \param[out] planarGrid - \ru Триангуляция двумерной области.
                             \en Triangulation of a two-dimensional region. \~
    \return \ru Возвращает true, если грань создана.
            \en Returns 'true' if the face was created. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (MbGrid *) SpaceGrid( const MbPlacement3D & place, const MbPlanarGrid & planarGrid, bool exact = false );


//------------------------------------------------------------------------------
/** \brief \ru Заменить элемент на вставку, если расстояние от начала координат до центра
               его габарита, превышает размер габаритного куба в заданное число раз \~
           \en Replace an item by an instance if the length from it's bounding box to the
               world origin is greater than it's diagonal by specified factor.
    \details \ru Объект или его копия смещается на вектор из начала координат до центра габаритного куба объекта 
                 и размещается во вставке, обеспечивающей смещение на вектор противоположного направления. \~
                 Обрабатываются только объекты, не являющиеся вставками. \~
                 Нулевое или отрицательное значение параметра ratioThreashhold запрещает преобразование. \~
             \en The item or it's replica is moved bey the vector from the origin of the world to the center of the 
                 item's bounding box, then put in the instance, providing the displacement by the reversed vector. \~
                 Function processes objects of all types except for instances. \~
                 Null or negative value of the ratioThreashhold parameter blocks the transformation. \~
  \param[out] item -              \ru Обрабатываемый объект. \~
                                  \en Processable item. \~
  \param[in] ratioThreashhold -   \ru Пороговое значение отношения расстояния до центра габаритного куба и его диагонали, 
                                      при превышении которого происходит замена. \~
                                  \en The replacement threshold value of the ratio of the bounding box's center to the 
                                      origin of the world to it's diagonal length. \~
  \param[in] makeCopy -           \ru Производить ли трансформацию на копии объекта. \~
                                  \en Is the copy of the object must be transformed. \~
  \return \ru Возвращает вставку объекта.
          \en Returns instance of object. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (MbItem *) ReplaceByInstance( MbItem * item, double ratioThreashhold = -1.0, bool makeCopy = false );


//------------------------------------------------------------------------------
/** \brief \ru Построение «залитого» объема, расположенного между внутренней поверхностью сосуда и ограничивающей поверхностью или телом. \~
           \en The construction of a "flood fill" volume located between the inner surface of the vessel and the bounding surface or body. \~
    \details \ru На вход подаётся тело, дополнительная поверхность или дополнительное тело и координаты источника. 
                 На выходе получаем объём, построенный от источника и ограниченный со всех сторон оболочкой тела и дополнительными объектами. \~
             \en The body, an additional surface or an additional body and the coordinates of the source are fed to the input. 
                 On the output we get the volume, constructed from the source and bounded from all sides by the shell of the body and by additional objects. \~
  \param[in] vessel -    \ru Тело сосуда. \~
                         \en The vessel. \~
  \param[in] sameShell - \ru Режим копирования тела сосуда.
                         \en Whether to copy the vessel. \~
  \param[in] bungData -  \ru Поверхность уровня или тело пробки. \~
                         \en The surface of the level or body of the bung. \~
  \param[in] origin -    \ru Точка внутри сосуда. \~
                         \en The point inside the vessel. \~
  \param[in] names     - \ru Именователь операции.
                         \en An object defining names generation in the operation. \~
  \param[out] result   - \ru Построенное тело.
                         \en The resultant solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (MbResultType) FloodFillResult(       MbSolid &       vessel, 
                                                MbeCopyMode     sameShell, 
                                          const MbSweptData &   bungData,
                                          const MbCartPoint3D & origin,
                                          const MbSNameMaker &  names, 
                                                MbSolid *&      result );


//------------------------------------------------------------------------------
/** \brief \ru Создать крепеж по трехмерной точке. НЕ ИСПОЛЬЗОВАТЬ ВНЕ ТЕСТОВОГО ПРИЛОЖЕНИЯ!!! ФУНКЦИЯ НАХОДИТСЯ В РАЗРАБОТКЕ!!!
           \en Create fastener using 3D point. \~ 
    \details \ru Создать крепеж по трехмерной точке. \n
             \en Create fastener using 3D point. \n \~
  \param[in]  solids    - \ru Множество тел для скрепления. 
                          \en An array of bodies to fasten. 
  \param[in]  sameShell - \ru Режим копирования тел.
                          \en Whether to copy the solids. \~
  \param[in]  point     - \ru Трехмерная точка, на основе проецирования которой определяется положение крепежа.
                          \en 3d point. \~
  \param[in]  params    - \ru Параметры крепежа ( его тип, размеры и т.д. ).
                          \en Fastener parameters ( type, diameter,  etc. ). \~
  \param[in]  names     - \ru Именователь новых граней.
                          \en An object defining the name of a new faces. \~
  \param[out] results   - \ru Множество тел для скрепления с набором отверстий и набор тел крепежа в отверстиях.
                          \en Array of bodies with a holes and fastener body. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) CreateFastener ( const RPArray<MbSolid> & solids, 
                                                MbeCopyMode        sameShell,
                                          const MbCartPoint3D &    point, 
                                          const FastenersValues &  params,
                                          const MbSNameMaker &     names,
                                                RPArray<MbSolid> & results );
   

//------------------------------------------------------------------------------
/** \brief \ru Создать набор крепежных элементов по трехмерной кривой. НЕ ИСПОЛЬЗОВАТЬ ВНЕ ТЕСТОВОГО ПРИЛОЖЕНИЯ!!! ФУНКЦИЯ НАХОДИТСЯ В РАЗРАБОТКЕ!!!
           \en Create an array of fastener elements using 3d curve. \~ 
    \details \ru Создать набор крепежных элементов по трехмерной кривой.
             \en Create an array of fastener elements using 3d curve. \~
  \param[in]  solids    - \ru Множество тел для скрепления. 
                          \en Array of bodies with a hole and fastener body. \~
  \param[in]  sameShell - \ru Режим копирования тел.
                          \en Whether to copy the solids. \~
  \param[in]  curve     - \ru Трехмерная кривая, на основе проецирования точек которой определяются положения крепежных элементов.
                          \en 3D curve. \~
  \param[in]  number    - \ru Количество точек на кривой. Точки расположены равномерно по длине кривой.
                          \en Number of points on the curve. Points are uniformly located along the length of the curve. \~
  \param[in]  params    - \ru Параметры крепежа ( его тип, размеры и т.д. ).
                          \en Fastener parameters ( type, diameter, etc. ). \~
  \param[in]  names     - \ru Именователь новых граней.
                          \en An object defining the name of a new faces. \~
  \param[out] results   - \ru Множество тел для скрепления с набором отверстий и набор тел крепежа в отверстиях.
                          \en Array of bodies with a holes and fastener bodies. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) CreateFasteners( const RPArray<MbSolid> & solids, 
                                                MbeCopyMode        sameShell,
                                          const MbCurve3D &        curve, 
                                                size_t             number,
                                          const FastenersValues &  params,
                                          const MbSNameMaker &     names,
                                                RPArray<MbSolid> & results );


//------------------------------------------------------------------------------
/** \brief \ru Слить несколько граней тела в одну грань.
           \en Create a solid with one face instead selected faces. \~
  \details \ru Заменить указанные гладко стыкующиеся грани тела одной геометрически совпадающей гранью. \n
           \en To replace these smooth abutting faces to form a single geometrically matching face. \n
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Режим копирования исходного тела.
                         \en Whether to copy the source solid. \~
  \param[in] faces - \ru Объединяемые грани тела.
                        \en The faces of solid to be merged. \~
  \param[in] uParam - \ru Параметры u направления объединяющей поверхности.
                      \en The operation parameters for common surface in u direction. \~
  \param[in] vParam - \ru Параметры v направления объединяющей поверхности.
                      \en The operation parameters for common surface in v direction. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \param[in]  prolong - \ru Параметр добавления гладко стыкующихся граней с faces (prolong>0).
                        \en The parameter of adding prolong faces (prolong>0). \~
  \param[out] result - \ru Построенное тело.
                       \en The resultant solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) CreateMerging(       MbSolid &           solid, 
                                              MbeCopyMode         sameShell, 
                                              c3d::FacesVector &  faces, 
                                        const MbNurbsParameters & uParam, 
                                        const MbNurbsParameters & vParam,
                                              double              tolerance,
                                        const MbSNameMaker &      names,
                                              bool                prolong,
                                              MbSolid *&          result );


//------------------------------------------------------------------------------
/** \brief \ru Найти грани тел, имеющие контактные площадки. \~
           \en To find contacted faces of bodies. \~
    \details \ru Найти номера контактирующих граней тел, имеющих противоположно направленные нормали, у которых есть общие участки с конечной площадью перекрытия. \~
             \en To find contacted faces of bodies with oppositely directed normals which have a finite overlap area. \~
  \param[in] solid1        - \ru Первое тело.
                             \en The first solid. \~
  \param[in] solid2        - \ru Второе тело.
                             \en The second solid. \~
  \param[in] precision     - \ru Точность операции.
                             \en The precision of operation. \~
  \param[out] facesNumbers - \ru Пары номеров касающихся граней с противоположно направленными нормалями.
                             \en The couples of number of contacted faces with oppositely directed normals. \~
  \return \ru Возвращает true, если грани контакта найдены.
          \en Returns true, if contacted faces were found. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) FindTouchedFaces( const MbSolid & solid1, 
                                   const MbSolid & solid2,
                                   double precision, 
                                   c3d::IndicesPairsVector & facesNumbers );


//------------------------------------------------------------------------------
/** \brief \ru Разбить контактирующие грани тел. \~
           \en To find contacted faces of bodies. \~
    \details \ru Разбить контактирующие грани тел, выделив общие области с конечной площадью перекрытия в отдельные грани. \~
             \en To find contacted faces of bodies and build a finite overlap contacted area as faces. \~
  \param[in,out] solid1    - \ru Первое тело.
                             \en The first solid. \~
  \param[in,out] solid2    - \ru Второе тело.
                             \en The second solid. \~
  \param[in] precision     - \ru Точность операции.
                             \en The precision of operation. \~
  \param[in] facesNumbers  - \ru Множество пар номеров соприкасающихся граней, у которых требуется построить общие пятна контакта (может быть пустым).
                             \en The container with pairs of contact face numbers that need to have common contact spots (it can be empty). \~
  \return \ru Возвращает true, если грани контакта найдены.
          \en Returns true, if contacted faces were found. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) SplitTouchedFaces( MbSolid & solid1, 
                                    MbSolid & solid2,
                                    double precision, 
                                    c3d::IndicesPairsVector & facesNumbers );


//------------------------------------------------------------------------------
/** \brief \ru Объединить тела, имеющие контактирующие грани. \~
           \en The function performs unite the bodies with contacted faces. \~
    \details \ru Объединить тела, удалив контактирующие грани. \~
             \en The function performs unite the bodies and removing the contacting faces. \~
  \param[in] solid1      - \ru Первое тело.
                           \en The first solid. \~
  \param[in]  sameShell1 - \ru Способ копирования граней первого тела.
                           \en Method of copying the faces of the first solid. \~
  \param[in] solid2      - \ru Второе тело.
                           \en The second solid. \~
  \param[in]  sameShell2 - \ru Способ копирования граней второго тела.
                           \en Method of copying the faces of the second solid. \~
  \param[in] precision   - \ru Точность операции.
                           \en The precision of operation. \~
  \param[out] result     - \ru Построенное тело.
                           \en The resultant solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) TouchedSolidsMerging(       MbSolid &      solid1, 
                                                     MbeCopyMode    sameShell1,
                                                     MbSolid &      solid2,
                                                     MbeCopyMode    sameShell2,
                                               const MbSNameMaker & names,
                                                     double         precision, 
                                                     MbSolid *&     result );


//------------------------------------------------------------------------------
/** \brief \ru Починить тело. \~
           \en Solid repairing. \~
    \details \ru Найти дефекты и починить грани, рёбра, вершины тела. \~
             \en Find defects and repair faces, edges, vertices of the solid. \~
  \param[in] solid    - \ru Тело для ремонта.
                        \en The initial solid. \~
  \param[in] accuracy - \ru Точность для поиска дефектов тела.
                        \en The accuracy for finding defects of the solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbResultType) SolidRepairing( MbSolid & solid, double accuracy );


//------------------------------------------------------------------------------
/** \brief \ru Найти грани скругления и фаски. \~
           \en Find fillet and chamfer faces. \~
    \details \ru Найти грани скругления и фаски среди присланных граней и добавить в присланный контейнер. \~
             \en Find fillet and chamfer faces and add them into container. \~
  \param[in] faces    - \ru Грани для поиска.
                        \en Faces for check. \~
  \param[in] accuracy - \ru Точность для поиска.
                        \en The accuracy for finding. \~
  \param[in] filletFaces - \ru Найденные грани скругления и фаски.
                           \en Found fillet and chamfer faces. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (void) FindFilletFaces( const RPArray<MbFace> & faces, 
                                        double            accuracy,
                                        RPArray<MbFace> & filletFaces );


//------------------------------------------------------------------------------
/** \brief \ru Распознать грань скругления. \~
           \en Recognize fillet face. \~
    \details \ru Если грань является скруглением, то распознать грань скругления. \~
             \en If face is fillet, then recognize face. \~
  \param[in] face    - \ru Грань для распознавания.
                       \en Face for recognize. \~
  \param[in] accuracy - \ru Точность для распознавания.
                        \en The accuracy for recognize. \~
  \return \ru Возвращает true, если грань распознана как скругление, иначе возвращает false.
          \en Returns true, if face is recognized as a fillet, else returns false. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC(bool) IsFilletFace( const MbFace & face, 
                                    double   accuracy );


//------------------------------------------------------------------------------
/** \brief \ru Получить трансформированную копию тела. \~
           \en Get transformed copy of a solid. \~
    \details \ru Получить трансформированную копию тела, если матрица трансформации не единичная или оригинал, если единичная. \~
             \en Get transformed copy of a solid if a matrix is not identity matrix or original of the solid if the matrix is identity matrix. \~
  \param[in]     solid    - \ru Тело.
                            \en A solid. \~
  \param[in,out] copyMode - \ru Исходный режим копирования тела.
                            \en An initial copy mode. \~
  \param[in]     matr     - \ru Матрица преобразования.
                            \en Transformation matrix. \~
  \param[in]     transformedMainName - \ru Главное имя для операции трансформации.
                                       \en Main name of transformation operation. \~
  \return \ru Возвращает копию или оригинал тела.
          \en Returns copy or original of the solid. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC(c3d::SolidSPtr) GetTransformedSolid(       c3d::SolidSPtr & solid,
                                                     MbeCopyMode &    copyMode,
                                               const MbMatrix3D &     matr,
                                                     SimpleName       transformedMainName = ct_TransformedSolid );


//------------------------------------------------------------------------------
/** \brief \ru Получить трансформированную копию объекта. \~
           \en Get transformed copy of object. \~
    \details \ru Получить трансформированную копию объекта, если матрица трансформации не единичная или оригинал, если единичная. \~
             \en Get transformed copy of an object if a matrix is not identity matrix or original of the solid if the matrix is identity matrix. \~
  \param[in] item - \ru Объект.
                    \en An object. \~
  \param[in] matr - \ru Матрица преобразования.
                    \en Transformation matrix. \~
  \return \ru Возвращает копию или оригинал объекта.
          \en Returns copy or original of the object. \~
  \ingroup Algorithms_3D
*/
// ---
template <class Item>
SPtr<Item> GetTransformedItem( SPtr<Item> & item, const MbMatrix3D & matr, MbRegDuplicate * iDupReg = nullptr, MbRegTransform * iTransReg = nullptr )
{
  SPtr<Item> resItem( item );
  if ( (resItem != nullptr) && !matr.IsSingle() ) {
    resItem = static_cast<Item *>( &item->Duplicate( iDupReg ) );
    resItem->Transform( matr, iTransReg );
  }
  return resItem;
}


//------------------------------------------------------------------------------
/** \brief \ru Класс для проецирования на оболочку.
           \en Class for projection onto shell. \~
  \details \ru Класс для проецирования на оболочку. \n
           \en Class for projection onto shell. \n \~
  \ingroup Algorithms_3D
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbFaceShellProjector
{
protected:
  MbFaceShellProjector() {}

public:
  /// \ru Деструктор. \en Destructor.
  virtual ~MbFaceShellProjector() {}

  /** \brief \ru Создать экземпляр класса по оболочке и параметрам.
             \en Create class instance by shell and parameters. \~
    \details \ru Создать экземпляр класса по оболочке и параметрам. Пользователь должен сам удалить объект.
             \en Create class instance by shell and parameters. User must delete created object. \~
    \param[in] shell  - \ru Оболочка. \n
                        \en Shell. \~
    \param[in] params - \ru Параметры для многократного проецирования точек на оболочку. \n
                        \en Parameters for multiple projection of points onto the shell. \~
    \return \ru Возвращает указатель на созданный объект.
            \en Returns pointer to created object. \~
  */
  static MbFaceShellProjector * Create( const MbFaceShell & shell, const MbShellProjectionParams & params );

  /** \brief \ru Найти ближайшую проекцию точки на оболочку.
             \en Find nearest point projection to the shell. \~
    \details \ru Найти номер грани и параметры её поверхности для ближайшей проекции точки на оболочку. \n
             \en Find face index and surface parameters for the nearest point projection to the shell. \n \~
    \param[in] p - \ru Проецируемая точка.
                   \en A point to project. \~
    \param[out] faceIndex - \ru Номер грани в оболочке для ближайшей проекции.
                       \en The index of nearest face of the shell. \~
    \param[out] u - \ru Первый параметр поверхности грани проекций для ближайшей проекции точки на оболочку.
                    \en The first parameter of the face surface for the nearest projection. \~
    \param[out] v - \ru Второй параметр поверхности грани проекций для ближайшей проекции точки на оболочку.
                    \en The second parameter of the face surface for the nearest projection. \~
    \return \ru Удалось ли определить положение точки.
            \en Whether the point projection was successfully defined. \~
  */
  virtual bool NearPointProjection( const MbCartPoint3D & point, size_t & faceIndex, double & u, double & v ) const = 0;

  OBVIOUS_PRIVATE_COPY( MbFaceShellProjector );
};
#endif // __ACTION_GENERAL_H