////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Методы прямого редактирования тел.
         \en Functions for direct editing of solids. \~
  \details
         \ru Прямое моделирование позволяет редактировать и создавать подобные тела 
             путём непосредственной модификации элементов уже построенных тел.\n
         \en The direct modeling allows to edit and to create similar solids 
             by direct modification of elements of already constructed solids.\n\~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ACTION_DIRECT_H
#define __ACTION_DIRECT_H

#include <op_direct_mod_parameter.h>

class   MbCartPoint3D;
class   MbCurve;
class   MbCurve3D;
class   MbCurveEdge;
class   MbFace;
class   MbFaceShell;
class   MbModifiedSolidParams;
class   MbModifiedSolidResults;
class   MbPlacement3D;
class   MbSNameMaker;
class   MbSolid;
class   MbSplineSurface;
class   MbSurface;
class   MbSurfaceIntersectionCurve;

struct  ModifyValues;
struct  NurbsBlockValues;
struct  NurbsValues;
struct  TransformValues;

template <class Type> class Array2;
template <class Type> class SArray;
template <class Type> class RPArray;

//------------------------------------------------------------------------------
/** \brief \ru Модифицировать тело по матрице.
           \en Modify a solid by the matrix. \~
  \details \ru Выполнить трансформацию копии исходного тела по матрице, рассчитанной по габаритному кубу. \n
           \en Transform a copy of the solid using the matrix calculated by bounding box of solid. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The initial solid. \~
  \param[in] sameShell - \ru Режим копирования исходного тела.
                         \en The mode of copying of the initial solid. \~
  \param[in] p - \ru Параметры трансформации.
                 \en The transformation parameters. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \param[out] result - \ru Модифицированное тело.
                       \en The modified solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Direct_Modeling
*/
// ---
MATH_FUNC (MbResultType) TransformedSolid(       MbSolid &         solid, 
                                                 MbeCopyMode       sameShell,
                                           const TransformValues & p,
                                           const MbSNameMaker &    names,
                                                 MbSolid *&        result );


//------------------------------------------------------------------------------
/** \brief \ru Собрать грани оболочки для методов прямого моделирования.
           \en Collect faces for direct modeling methods. \~
  \details
    \ru Основное назначение функции - подсветка модифицируемых граней.
    Функция собирает грани оболочки для методов прямого моделирования: \n
    dmm_RemoveBelowRadius, dmm_RemoveBelowRadiusWithAdjacentFillets, dmm_PurifyFilletsBelowRadius,
    dmm_FilletsRadiusSet, dmm_FilletsRadiusSetSingle, dmm_FilletsRadiusChange. \n
    Для типа dmm_RemoveBelowRadius собираются замкнутые цилиндрические, конические, тороидальные, сферические грани тела,
    а также грани вращения, радиус которых не превосходит указанный радиус.
    Для типа dmm_PurifyFilletsBelowRadius собираются незамкнутые цилиндрические, тороидальные, сферические грани,
    а также грани скругления, радиус которых не превосходит указанный радиус. \n
    \en The main purpose of this function is to highlight faces that will be modified.
    The method collects the faces of the shell for direct modeling methods: \n
    dmm_RemoveBelowRadius, dmm_RemoveBelowRadiusWithAdjacentFillets, dmm_PurifyFilletsBelowRadius,
    dmm_FilletsRadiusSet, dmm_FilletsRadiusSetSingle, dmm_FilletsRadiusChange. \n
    The cylindrical, conical, toroidal, spherical, and revolution periodic faces are collected for the dmm_RemoveBelowRadius type.
    The cylindrical, toroidal, spherical non-periodic, and fillet faces are collected for the dmm_PurifyFilletsBelowRadius type. \n \~
  \param[in] shell  - \ru Исходная оболочка тела.
                      \en The initial shell. \~
  \param[in] parameters - \ru Параметры операции.
                          \en The operation parameters. \~
  \param[out] outputFaces - \ru Найденные грани для дальнейшей модификации.
                            \en Found faces to be modified. \~
  \return \ru Возвращает код результата действий.
          \en Returns action result code. \~
  \ingroup Direct_Modeling
*/
// ---
MATH_FUNC( bool ) CollectFacesForModification( const MbFaceShell &          shell,
                                               const MbCollectFacesParams & parameters,
                                                     c3d::FacesVector &     outputFaces );


//------------------------------------------------------------------------------
/** \brief \ru Собрать грани оболочки для методов прямого моделирования.
           \en Modify a shell by the methods of direct modeling. \~
  \details \ru Функция собирает грани оболочки для методов прямого моделирования: \n
    удаление из тела выбранных граней с окружением (way==dmt_Remove), \n
    удаление выбранных граней скругления тела (way==dmt_Purify). \n
    Для удаления граней собираются замкнутые цилиндрические, конические, тороидальные, сферические грани тела,
    а также грани вращения, радиус которых не превосходит указанный радиус.
    Для удаления граней скругления собираются незамкнутые цилиндрические, тороидальные, сферические грани,
    а также грани скругления, радиус которых не превосходит указанный радиус. \n
           \en The method collects the faces of the shell for direct modeling methods: \n
    removal of the faces from a shell (way==dmt_Remove), \n
    removal of the fillet faces from a shell (way==dmt_Purify). \n 
    The cylindrical, conical, toroidal, spherical, and revolution periodic faces are collect to remove way.
    The cylindrical, toroidal, spherical non-periodic, and fillet faces are collect to purify way. \n \~
  \deprecated \ru Метод устарел. Взамен использовать #CollectFacesForModification с набором параметров #MbCollectFacesParams.
              \en The method is deprecated. Use instead #CollectFacesForModification with the parameters #MbCollectFacesParams. \~
  \param[in] shell  - \ru Исходная оболочка тела.
                      \en The initial faces set. \~
  \param[in] way    - \ru Способ модификации.
                      \en Way of the modification. \~
  \param[in] radius - \ru Радиус собираемых граней.
                      \en Radius of collected faces. \~
  \param[in] faces  - \ru Найденные грани для дальнейшей модификации.
                      \en Found faces to be modified. \~
  \return \ru Возвращает код результата действий.
          \en Returns action result code. \~
  \ingroup Direct_Modeling
*/
// ---
//DEPRECATE_DECLARE_REPLACE( CollectFacesForModification with 'MbCollectFacesParams' argument )
MATH_FUNC (MbResultType) CollectFacesForModification( MbFaceShell *     shell, 
                                                      MbeModifyingType  way,
                                                      double            radius, 
                                                      RPArray<MbFace> & faces );


//------------------------------------------------------------------------------
/** \brief \ru Модифицировать или построить тело методами прямого моделирования.
           \en Modify a solid by the methods of direct modeling. \~
  \details \ru В зависимости от параметров модификации #MbModifiedSolidParams метод выполняет одно из следующих действий: \n
    1.  Удаление из тела выбранных граней без прилегающих скруглений (dmm_RemoveFaces).\n
        Удаление грани вместе с прилегающими скруглениями (dmm_RemoveFacesWithAdjacentFillets).\n
        Удаление замкнутых цилиндрических, сферических граней, граней вращения и выдавливания радиуса меньше заданного
        без прилегающих скруглений (dmm_RemoveBelowRadius).\n
        Удаление замкнутых цилиндрических, сферических граней, граней вращения и выдавливания радиуса меньше заданного
        вместе с прилегающими скруглениями (dmm_RemoveBelowRadiusWithAdjacentFillets). \n
    2.  Создание тела из выбранных граней с окружением (dmm_Create). \n
    3.  Перемещение выбранных граней с окружением относительно оставшихся граней тела (dmm_MoveFacesByVector).
        Направление и величину перемещения определяет вектор direction. \n
    4.  Замена выбранных граней тела эквидистантными гранями (перемещение по нормали, изменение радиуса)
        (dmm_OffsetFaces). Расстояние смещения определяется параметром value.\n
    5.  Изменение радиуса выбранных граней скругления на заданную величину (dmm_FilletsRadiusChange).
        Величина изменения радиуса определяется параметром value.\n
    6.  Замена выбранных граней тела деформируемыми гранями (превращение в NURBS) для редактирования (dmm_ConvertFacesToNurbs). \n
    7.  Удаление выбранных граней скругления тела (dmm_PurifyFillets).
        Удаление также всех гладко прилегающих скруглений (dmm_PurifyFilletsChains).
        Удаления всех скруглений тела, меньших определённого радиуса (dmm_PurifyFilletsBelowRadius).\n
    8.  Слияние вершин ребёр и удаление рёбер (dmm_DeleteEdges). \n
    9.  Замена гладко стыкующихся граней одной гранью (dmm_UniteFaces). \n
    10. Поворот выбранных граней с адаптацией прилегающих скруглений (dmm_RotateFaces) или без (dmm_RotateFacesNoFilletAdaptation).
        Угол поворота определяется параметром angle, ось поворота определяется осью axis.\n
    11. Замена выбранных граней тела (dmm_ReplaceFaces).\n
    Одновременно с построением оболочки функция создаёт её строитель.\n
        \en The method performs one of the following actions depending on the parameter #MbModifiedSolidParams: \n
    1.  Removal of the specified faces without the adjacent fillets (dmm_RemoveFaces).\n
        Removal of the specified faces with the adjacent fillets (dmm_RemoveFacesWithAdjacentFillets).
        Removal of the close cylindric, spherical faces, extrusion and rotation faces below specified radius
        with the adjacent fillets (dmm_RemoveBelowRadiusWithAdjacentFillets).\n
        Removal of the close cylindric, spherical faces, extrusion and rotation faces below specified radius
        without the adjacent fillets (dmm_RemoveBelowRadius).\n
    2.  Creation of a solid from the specified faces with the neighborhood (dmm_Create). \n
    3.  Translation of the specified faces with neighborhood relative to the other faces of the solid (dmm_MoveFacesByVector).
        The translation direction is defined by the 'direction' vector.\n
    4.  Replacement of the specified faces by offset faces (dmm_OffsetFaces).
        The distance of the offset (or radius change) is defined by 'value'. \n
    5.  Radius change of the specified fillet faces (dmm_FilletChange).
        The value of radius change is defined by 'value'.\n
    6.  Replacement of the specified faces of a solid with a deformable NURBS faces for editing (dmm_ConvertFacesToNurbs). \n 
    7.  Remove of the specified fillet faces from a solid (dmm_PurifyFillets).
        Remove of the specified fillet faces with all smoothly connected fillets (dmm_PurifyFilletsChains).
        Remove of all fillets below specified radius from a solid (dmm_PurifyFilletsBelowRadius).\n
    8.  Edge vertices merging and edges removal (dmm_DeleteEdges). \n
    9.  Replacement of the smoothly joined faces with a single face (dmm_UniteFaces). \n
    10. Rotation of the chosen faces with the adjacent fillets adaptation (dmm_RotateFaces)
        or without (dmm_RotateFacesNoFilletAdaptation ). The parameters are the rotation angle and the rotation axis.\n
    11. Replacement of the specified faces of a solid (dmm_ReplaceFaces).\n
    The function simultaneously creates the shell and its constructor.\n \~
  \param[in] solid     - \ru Исходное тело.
                         \en The initial solid. \~
  \param[in] sameShell - \ru Режим копирования исходного тела.
                         \en The mode of copying of the initial solid. \~
  \param[in] params    - \ru Параметры модификации.
                         \en Parameters of the modification. \~
  \param[out] results  - \ru Результаты модификации.
                         \en Results of the modifcation. \~
  \return \ru Возвращает код результата операции.
  Возможные коды ошибок:
  Для всех типов:
    rt_Success -- Ошибок нет.
    rt_Error -- Неизвестная ошибка.
    rt_IncorrectData -- Некорректные данные.
    rt_ParameterError -- Ошибочный параметр.
  Для типа dmm_ReplaceGroupFaces:
    rt_NoFacesForModification -- Ошибка в параметрах. Отсутствуют заменяемые грани.
    rt_NoReplacementFaces -- Ошибка в параметрах. Отсутствуют заменяющие грани.
    rt_ConnectionError -- Ошибка в параметрах. Нарушена связность группы граней.
    rt_FacesGroupsOverlapError -- Ошибка в параметрах. Среди заменяющих граней есть заменяемые, массивы не должны пересекаться.
    rt_NoEdgesForBuild -- Отсутствуют граничные рёбра в заменяемой оболочке.
    rt_OffsetError -- Невозможно эквидистантно сдвинуть оболочку.
    rt_ExtensionOutOfRange -- Невозможно продлить оболочку.
    rt_SolidError -- Невозможно собрать оболочку из заменяющих граней.
    rt_CuttingError -- Невозможно усечь оболочку новой группой граней.
    rt_StitchShellsError -- Невозможно сшить итоговую оболочку.
          \en Returns operation result code.
  Possible error codes:
  For all types:
    rt_Success -- No errors.
    rt_Error -- Unknown error.
    rt_IncorrectData -- Incorrect data.
    rt_ParameterError -- Wrong parameter.
  For the type dmm_ReplaceGroupFaces:
    rt_NoFacesForModification -- Parameter error. There are no replaced faces.
    rt_NoReplacementFaces -- Parameter error. There are no replacement faces.
    rt_ConnectionError -- Parameter error. Broken connectivity of the face group.
    rt_FacesGroupsOverlapError -- Parameter error. There are replacement faces among the replaced faces. Those arrays must not overlap.
    rt_NoEdgesForBuild -- There are no boundary edges in the replaced shell.
    rt_OffsetError -- Cannot move the shell equidistantly.
    rt_ExtensionOutOfRange -- The shell cannot be extended.
    rt_SolidError -- Cannot make the shell from replacement faces.
    rt_CuttingError -- Impossible to cut the shell with the new faces group.
    rt_StitchShellsError -- Impossible to stitch the final shell. \~
  \ingroup Direct_Modeling
*/
// ---
MATH_FUNC (MbResultType) ModifiedSolid(       MbSolid &                solid,
                                              MbeCopyMode              sameShell,
                                        const MbModifiedSolidParams &  params,
                                              MbModifiedSolidResults & results );


//------------------------------------------------------------------------------
/** \brief \ru Модифицировать или построить тело методами прямого моделирования.
           \en Modify a solid by the methods of direct modeling. \~
  \deprecated \ru Метод устарел и будет удален в версии 2024.
              \en The method is deprecated and will be removed in version 2024. \~
  \details \ru В зависимости от параметров модификации #ModifyValues метод выполняет одно из следующих действий: \n
    1. Удаление из тела выбранных граней с окружением (param.way==dmt_Remove). \n
    2. Создание тела из выбранных граней с окружением (param.way==dmt_Create). \n
    3. Перемещение выбранных граней с окружением относительно оставшихся граней тела (param.way==dmt_Action). \n
    4. Замена выбранных граней тела эквидистантными гранями (param.way==dmt_Offset). \n
    5. Изменение радиуса выбранных граней скругления (param.way==dmt_Fillet). \n
    6. Замена выбранных граней тела деформируемыми  гранями для редактирования (param.way==dmt_Supple). \n
    7. Удаление выбранных граней скругления тела (param.way==dmt_Purify). \n
    8. Замена гладко стыкующихся граней одной гранью (param.way==dmt_United). \n
    9. Поворот выбранных граней (param.way==dmt_Rotate).
           \en The method performs one of the following actions depending on the parameter #ModifyValues: \n
    1. Removal of the specified faces with the neighborhood from a solid (param.way==dmt_Remove). \n
    2. Creation of a solid from the specified faces with the neighborhood (param.way==dmt_Create). \n
    3. Translation of the specified faces with neighborhood relative to the other faces of the solid (param.way==dmt_Action). \n
    4. Replacement of the specified faces by offset faces (param.way==dmt_Offset). \n
    5. Radius change of the specified fillet faces (param.way==dmt_Fillet). \n
    6. Replacement of the specified faces of a solid with a deformable faces for editing (param.way==dmt_Supple). \n 
    7. Removal of the specified fillet faces from a solid (param.way==dmt_Purify). \n
    8. Replacement of the smoothly joined faces with a single face (param.way==dmt_United). \n\~
    9. Rotation of the chosen faces (param.way==dmt_Rotate). \n \~
  \param[in] solid     - \ru Исходное тело.
                         \en The initial solid. \~
  \param[in] sameShell - \ru Режим копирования исходного тела.
                         \en The mode of copying of the initial solid. \~
  \param[in] params    - \ru Параметры модификации.
                         \en Parameters of the modification. \~
  \param[in] faces    -  \ru Изменяемые грани тела.
                         \en Faces to be modified. \~
  \param[in] names     - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[out] result   - \ru Модифицированное тело.
                         \en The modified solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Direct_Modeling
*/
// ---
//DEPRECATE_DECLARE_REPLACE( ModifiedSolid with MbModifiedSolidParams )
MATH_FUNC (MbResultType) FaceModifiedSolid(       MbSolid &         solid, 
                                                  MbeCopyMode       sameShell,
                                            const ModifyValues &    params,
                                            const RPArray<MbFace> & faces, 
                                            const MbSNameMaker &    names,
                                                  MbSolid *&        result );

//------------------------------------------------------------------------------
/** \brief \ru Модифицировать или построить тело методами прямого моделирования.
           \en Modify a solid by the methods of direct modeling. \~
  \deprecated \ru Метод устарел и будет удален в версии 2024.
              \en The method is deprecated and will be removed in version 2024. \~
  \details \ru Метод выполняет удаление указанных рёбер, слияние их вершин и модификацию окружающих граней (param.way==dmt_Merger). 
               По направлению вектора "param.direction" определяется: начальная ли вершина ребра будет слита с конечной вершиной, или конечная вершина ребра будет слита с начальной вершиной. \n
           \en The method performs the deletion of selectsd edges, merging their vertices and modification of surrounding faces (param.way==dmt_Merger).
               The direction of the vector "params.direction" determines whether the start vertex of an edge is merged with the end vertex, or whether the end vertex of an edge is merged with the start vertex. \n
  \param[in] solid     - \ru Исходное тело.
                         \en The initial solid. \~
  \param[in] sameShell - \ru Режим копирования исходного тела.
                         \en The mode of copying of the initial solid. \~
  \param[in] params    - \ru Параметры модификации, способ должен быть равен param.way==dmt_Merger.
                         \en Parameters of the modification, the way must be equal to param.way==dmt_Merger. \~
  \param[in] edges     - \ru Удаляемые рёьра тела.
                         \en Edges to be removed. \~
  \param[in] names     - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[out] result   - \ru Модифицированное тело.
                         \en The modified solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Direct_Modeling
*/
// ---
//DEPRECATE_DECLARE_REPLACE( ModifiedSolid with MbModifiedSolidParams )
MATH_FUNC (MbResultType) EdgeModifiedSolid(       MbSolid &         solid, 
                                                  MbeCopyMode       sameShell,
                                            const ModifyValues &    params,
                                            const RPArray<MbCurveEdge> & edges,
                                            const MbSNameMaker &    names,
                                                  MbSolid *&        result );


//------------------------------------------------------------------------------
/** \brief \ru Заменить выбранные грани тела деформируемыми гранями.
           \en Replace the specified faces of solid with deformable faces. \~
  \details \ru Заменить выбранные грани тела деформируемыми гранями (превращение в NURBS для редактирования). \n
           \en Replace the specified faces of the solid with deformable faces (conversion to NURBS for editing). \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The initial solid. \~
  \param[in] sameShell - \ru Режим копирования исходного тела.
                         \en The mode of copying of the initial solid. \~
  \param[in] p - \ru Параметры преобразования.
                 \en The transformation parameters. \~
  \param[in] faces - \ru Заменяемые грани тела.
                     \en Faces to replace. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \param[out] result - \ru Модифицированное тело.
                       \en The modified solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Direct_Modeling
*/
// ---
MATH_FUNC (MbResultType) ModifiedNurbsItem(       MbSolid &         solid, 
                                                  MbeCopyMode       sameShell,
                                            const NurbsValues &     p,
                                            const RPArray<MbFace> & faces,
                                            const MbSNameMaker &    names,
                                                  MbSolid *&        result );


//------------------------------------------------------------------------------
/** \brief \ru Заменить выбранную грань тела деформируемой гранью.
           \en Replace the specified face of the solid by a deformable face. \~
  \details \ru Заменить выбранную грань тела деформируемой гранью (превращение в NURBS для редактирования). \n
           \en Replace the specified face of the solid by a deformable face (conversion to NURBS for editing). \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The initial solid. \~
  \param[in] sameShell - \ru Режим копирования исходного тела.
                         \en The mode of copying of the initial solid. \~
  \param[in] p - \ru Параметры преобразования.
                 \en The transformation parameters. \~
  \param[in] face - \ru Заменяемая грань тела.
                    \en A face to replace. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \param[out] result - \ru Модифицированное тело.
                       \en The modified solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Direct_Modeling
*/
// ---
MATH_FUNC (MbResultType) ModifiedNurbsItem(       MbSolid &      solid, 
                                                  MbeCopyMode    sameShell,
                                            const NurbsValues &  p,
                                            const MbFace &       face,
                                            const MbSNameMaker & names,
                                                  MbSolid *&     result );


//------------------------------------------------------------------------------
/** \brief \ru Получить NURBS-поверхности грани.
           \en Get the NURBS-surfaces of a face. \~
  \details \ru Выполнить построение деформируемой поверхности для исходной грани. \n
           \en Create a deformable surface for the initial face. \n \~
  \param[in] face - \ru Исходная грань.
                    \en The initial face. \~
  \return \ru Возвращает NURBS-поверхности грани.
          \en Returns NURBS-surfaces of the face. \~
  \ingroup Direct_Modeling
*/
// ---
MATH_FUNC (MbSurface *) GetControlSurface( const MbFace & face ); 


//------------------------------------------------------------------------------
/** \brief \ru Получить контрольные точки NURBS-поверхности грани.
           \en Get the control points of the NURBS-surface of a face. \~
  \details \ru Получить множество контрольных точек NURBS-поверхности грани и множества их весов. \n
           \en Get a set of the control points of a NURBS-surface of a face and a set of their weights. \n \~
  \param[in] face - \ru Исходная грань.
                    \en The initial face. \~
  \param[out] controlPoints - \ru Контрольные точки NURBS-поверхности грани.
                              \en The control points of the NURBS-surface of the face. \~
  \param[out] result - \ru Веса контрольных точек.
                       \en The weights of the control points. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Direct_Modeling
*/
// ---
MATH_FUNC (MbResultType) FaceControlPoints( const MbFace &                face,
                                                  Array2<MbCartPoint3D> & controlPoints,
                                                  Array2<double> &        result );


//------------------------------------------------------------------------------
/** \brief \ru Деформировать грань тела.
           \en Deform a face of a solid. \~
  \details \ru Деформировать грань тела путём подстановки присланных контрольных точек NURBS-поверхности грани. \n
           \en Deform a face of a solid by substitution the control points of NURBS-surface of the face with the given control points. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The initial solid. \~
  \param[in] sameShell - \ru Режим копирования исходного тела.
                         \en The mode of copying of the initial solid. \~
  \param[in] face - \ru Изменяемая грань тела.
                    \en A face of a solid to be modified. \~
  \param[in] faceSurface - \ru Новая NURBS-поверхность для грани.
                           \en The new NURBS-surface of the face. \~
  \param[in] fixedPoints - \ru Неподвижные узлы.
                           \en The fixed points. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \param[out] result - \ru Модифицированное тело.
                       \en The modified solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Direct_Modeling
*/
// ---
MATH_FUNC (MbResultType) NurbsModification(       MbSolid &      solid, 
                                                  MbeCopyMode    sameShell,
                                                  MbFace *       face,
                                                  MbSurface &    faceSurface,
                                                  Array2<bool> & fixedPoints,
                                            const MbSNameMaker & names,
                                                  MbSolid *&     result );


//------------------------------------------------------------------------------
/** \brief \ru Деформировать грань тела.
           \en Deform a face of a solid. \~
  \details \ru Деформировать грань тела путём подстановки присланных контрольных точек NURBS-поверхности грани. \n
           \en Deform a face of a solid by substitution the control points of NURBS-surface of the face with the given control points. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The initial solid. \~
  \param[in] sameShell - \ru Режим копирования исходного тела.
                         \en The mode of copying of the initial solid. \~
  \param[in] face - \ru Изменяемая грань тела.
                    \en A face of a solid to be modified. \~
  \param[in] controlPoints - \ru Контрольные точки NURBS-поверхности грани.
                             \en The control points of the NURBS-surface of the face. \~
  \param[in] weights - \ru Веса контрольных точек.
                       \en The weights of the control points. \~
  \param[in] fixedPoints - \ru Неподвижные узлы.
                           \en The fixed points. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \param[out] result - \ru Модифицированное тело.
                       \en The modified solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Direct_Modeling
*/
// ---
MATH_FUNC (MbResultType) NurbsModification(       MbSolid &               solid, 
                                                  MbeCopyMode             sameShell,
                                                  MbFace *                face,
                                            const Array2<MbCartPoint3D> & controlPoints,
                                            const Array2<double> &        weights,
                                                  Array2<bool> *          fixedPoints,
                                            const MbSNameMaker &          names,
                                                  MbSolid *&              result );


//------------------------------------------------------------------------------
/** \brief \ru Построить деформируемую призму.
           \en Create a deformable prism. \~
  \details \ru Построить тело в форме прямого параллелепипеда с деформируемыми гранями. \n
           \en Create a solid as a right parallelepiped with deformable faces. \n \~
  \param[in] place - \ru Локальная система координат.
                     \en A local coordinate system. \~
  \param[in] ax - \ru Размер по X.
                  \en The size in X-direction. \~
  \param[in] ay - \ru Размер по Y.
                  \en The size in Y-direction. \~
  \param[in] az - \ru Размер по Z.
                  \en The size in Z-direction. \~
  \param[in] outDir - \ru Ориентация нормалей наружу.
                      \en An outer orientation of the normals. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \param[in] name - \ru Главное имя.
                    \en The main name. \~
  \param[in] param - \ru Параметры NURBS-поверхностей граней параллелепипеда.
                     \en The parameters of NURBS-surfaces of the parallelepiped faces. \~
  \param[out] result - \ru Тело из NURBS-поверхностей.
                       \en The solid constructed from the NU|RBS-surfaces. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Direct_Modeling
*/
// ---
MATH_FUNC (MbResultType) NurbsBlockSolid( const MbPlacement3D &    place,
                                                double             ax, 
                                                double             ay, 
                                                double             az, 
                                                bool               outDir,
                                          const MbSNameMaker &     names, 
                                                SimpleName         name,
                                                NurbsBlockValues & param,
                                                MbSolid *&         result );


//------------------------------------------------------------------------------
/** \brief \ru Построить согласованную поверхность.
           \en Create a matched surface. \~
  \details \ru Для исходной поверхности выполнить построение изменённой поверхности
    путем выставления сопряжения вдоль кривой. \n
           \en Create a modified surface for the initial surface
    by specifying the conjugation along the curve. \n \~
  \param[in] curve - \ru Кривая пересечения поверхностей ребра.
                     \en The intersection curve of the edge surfaces. \~
  \param[in] sences - \ru Ориентация кривой ребра в цикле.
                      \en The edge curve sense in the loop. \~
  \param[in] faceSences - \ru Ориентация нормали на смежной грани.
                          \en The adjacent face normal orientation. \~
  \param[in] surface - \ru Исходная сплайновая поверхность для изменяемой грани.
                       \en The initial spline surface of the face to be modified. \~
  \param[in] tension - \ru Натяжение.
                       \en The tension. \~
  \param[in] conType - \ru Тип сопряжения.
                       \en The conjugation type. \~
  \param[in] insertNum - \ru Вставляемый ряд.
                         \en The row number. \~
  \param[out] result - \ru NURBS-поверхность, полученная в результате преобразований.
                       \en The NURBS-surface obtained as a result of the modifications. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Direct_Modeling
*/
// ---
MATH_FUNC (MbResultType) NurbsFaceConjugation( const MbSurfaceIntersectionCurve & curve,
                                                     bool                         sences,
                                                     bool                         faceSences,
                                               const MbSplineSurface &            surface,
                                                     double                       tension, 
                                                     MbeConjugationType           conType,
                                                     size_t                       insertNum,
                                                     MbSplineSurface *&           result );


//------------------------------------------------------------------------------
/** \brief \ru Построить подобную поверхность.
           \en Create a similar surface. \~
  \details \ru Для исходной поверхности выполнить построение подобной поверхности
    по указанной поверхности-образцу. \n
           \en Create a surface similar to the initial one
    given the pattern surface. \n \~
  \param[in] originSurface - \ru Поверхность-образец.
                          \en A pattern surface. \~
  \param[in] surface - \ru Исходная сплайновая поверхность для изменяемой грани.
                        \en The initial spline surface of the face to be modified. \~
  \param[in] uToU - \ru Флаг сохранения параметрического направления как у поверхности-образца.
                    \en Whether to keep the parametric direction of the pattern surface. \~
  \param[in] normSence - \ru Флаг сохранения направления нормали поверхности-образца.
                         \en Whether to keep the normal direction of the pattern surface. \~
  \param[out] result - \ru NURBS-поверхность, полученная в результате преобразований.
                       \en The NURBS-surface obtained as a result of the modifications. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Direct_Modeling
*/
// ---
MATH_FUNC (MbResultType) NurbsFaceSimilarity( const MbSurface &        originSurface, 
                                              const MbSplineSurface &  surface, 
                                                    bool               uToU, 
                                                    bool               normSence, 
                                                    MbSplineSurface *& result );


//------------------------------------------------------------------------------
/** \brief \ru Построить сглаженную поверхность.
           \en Create a smoothed surface. \~
  \details \ru Выполнить сглаживание копии исходной поверхности не изменяя ее порядок и количество контрольных точек. \n
           \en Perform smoothing of a copy of the initial surface without changing its order and the number of control points. \n \~
  \param[in] surface - \ru Исходная сплайновая поверхность для изменяемой грани.
                       \en The initial spline surface of the face to be modified. \~
  \param[in] udegree - \ru Параметр сглаживания по первому параметру поверхности.
                       \en The smoothing surface degree for direction of first parameter of surface. \~
  \param[in] vdegree - \ru Параметр сглаживания по второму параметру поверхности.
                       \en The smoothing surface degree for direction of second parameter of surface. \~
  \param[out] result - \ru NURBS-поверхность, полученная в результате преобразований.
                       \en The NURBS-surface obtained as a result of the modifications. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Direct_Modeling
*/
// ---
MATH_FUNC (MbResultType) SplineSurfaceSmoothing( const MbSplineSurface &  surface, 
                                                       size_t             udegree, 
                                                       size_t             vdegree, 
                                                       MbSplineSurface *& result );


#endif // __ACTION_DIRECT_H
