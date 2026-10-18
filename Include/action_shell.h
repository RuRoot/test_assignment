////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Методы построения незамкнутых тел.
         \en Functions for open solids construction. \~
  \details \ru Геометрическое ядро C3D поддерживает поверхностное моделирование. 
    Результатом поверхностного моделирования являются элементы геометрической модели, 
    которые будем называть незамкнутыми телами. Незамкнутые тела характерны тем,
    что они описывают не всю поверхность моделируемого объекта, а только часть её.
    Часто незамкнутое тело состоит из одной грани. В незамкнутом теле всегда присутствуют 
    краевые рёбра. Незамкнутое тело описывает множество точек, принадлежащих только граням
    этого тела, тогда как замкнутое тело описывает множество точек, располагающихся
    на поверхности моделируемого объекта и внутри него.
           \en The geometric kernel C3D supports the surface modeling. 
    The result of surface modeling are elements of geometric model 
    which are called open solids here. Open solids
    describe not the whole surface of an object of modeling but only a part of it.
    An open solid often consists of one face. An open solid always contains 
    boundary edges. An open solid describes a point set that belong to faces of the solid only,
    whereas a closed solid describes a point set
    on the surface of the modeled object and inside it. \~ 
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ACTION_SHELL_H
#define __ACTION_SHELL_H


#include <space_item.h>
#include <op_boolean_flags.h>
#include <op_shell_parameter.h>
#include <mb_operation_result.h>


class MbCurve;
class MbCurve3D;
class MbSurface;
class MbCurveEdge;
class MbFace;
class MbSolid;
class MbSNameMaker;
class MbPatchCurve;
class IProgressIndicator;


//------------------------------------------------------------------------------
/** \brief \ru Построить заплатку.
           \en Create a patch. \~
  \details \ru Построить заплатку по указанным параметрам. \n
           \en Create a patch from the specified parameters. \n \~
  \param[in] params - \ru Параметры операции.
                      \en Operation parameters. \~
  \param[out] result - \ru Построенная заплатка.
                       \en The required patch. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC( MbResultType ) PatchShell( const MbPatchShellParams & params, 
                                            c3d::SolidSPtr &     result );


//------------------------------------------------------------------------------
/** \brief \ru Построить незамкнутое тело по множеству групп точек.
           \en Create an open solid given a set of point groups. \~
  \details \ru Построить незамкнутое тело по сечениям, образованным сплайнами, построенными по группе контрольных точек. \n
           \en Create an open lofted solid whose profiles are defined by splines created from the specified groups of points. \n \~
  \param[in] params - \ru Параметры операции.
                          Содержат информацию о наборе точек, именователе, идентификаторе.
                      \en Operation parameters.
                          Contains information about points, object defining names generation in the operation, identifier. \~
  \param[out] result - \ru Результирующая оболочка.
                       \en The required shell. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbResultType) LoftedShell( const MbLoftedPointsShellParams & params,
                                            c3d::SolidSPtr &            result );


//------------------------------------------------------------------------------
/** \brief \ru Построить незамкнутое тело по множеству кривых.
           \en Create an open solid from a set of curves. \~
  \details \ru Построить незамкнутое тело по сечениям, образованным кривыми. \n
           \en Create an open lofted solids whose profiles are defined by the curves. \n \~
  \param[in] params - \ru Параметры операции.
                          Содержат информацию о наборе кривых, именователе, идентификаторе.
                      \en Operation parameters.
                          Contains information about curves, object defining names generation in the operation, identifier. \~
  \param[out] result - \ru Результирующая оболочка.
                       \en The required shell. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbResultType) LoftedShell( const MbLoftedCurvesShellParams & params,
                                            c3d::SolidSPtr &            result );


//------------------------------------------------------------------------------
/** \brief \ru Построить незамкнутое эквидистантное тело.
           \en Create an open offset solid. \~
  \details \ru Построить незамкнутое эквидистантное тело на базе указанных в initFaces граней. \n
           \en Create an open offset solid on the basis of the faces 'initFaces'. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The initial solid. \~
  \param[in] sameShell - \ru Режим копирования тела.
                         \en Whether to copy the solid. \~
  \param[in] initFaces - \ru Грани исходного тела для построения.
                         \en Faces of the initial solid for construction. \~
  \param[in] checkFacesConnection - \ru Необходимость проверки связности выбранных граней.
                                    \en Whether to check connectivity of the specified faces. \~
  \param[in] p - \ru Параметры операции.
                 \en The operation parameters. \~
  \param[in] operNames - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[in]  copyFaceAttrs - \ru Копировать атрибуты из исходных граней в эквидистантные.
                              \en Copy attributes of initial faces to offset faces. \~
  \param[out] result - \ru Эквидистантная оболочка.
                       \en The offset shell. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbResultType) OffsetShell(       MbSolid &         solid, 
                                            MbeCopyMode       sameShell, 
                                            RPArray<MbFace> & initFaces, 
                                            bool              checkFacesConnection, 
                                            SweptValues &     p, 
                                      const MbSNameMaker &    operNames,
                                            bool              copyFaceAttrs,
                                            MbSolid *&        result );


//------------------------------------------------------------------------------
/** \brief \ru Построить незамкнутое тело по множеству точек.
           \en Create an open solid from a point set. \~
  \details \ru Построить незамкнутое тело по множеству точек, заданных в параметрах построения. \n
           \en Create an open solid from a point set specified in parameters. \n \~
  \param[in, out] params - \ru Параметры операции.
                           \en The operation parameters. \~
  \param[out] result - \ru Результирующая оболочка.
                       \en The required shell. \~
  \param[in, out] progBar - \ru Индикатор прогресса выполнения операции.
                            \en A progress indicator of the operation. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbResultType) NurbsSurfacesShell( MbNurbsSurfacesShellParams & params,
                                             c3d::SolidSPtr &             result,
                                             IProgressIndicator *         progBar );


//------------------------------------------------------------------------------
/** \brief \ru Построить тело по сети кривых.
           \en Create a solid from a set of curves. \~
  \details \ru Построить тело по сети кривых, заданных в параметрах построения. \n
           \en Create a solid from a set of curves specified in the parameters. \n \~
  \param[in] params - \ru Параметры операции.
                      \en The operation parameters. \~
  \param[out] result  - \ru Результаты построения.
                          \en Operation results. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbResultType) MeshShell( const MbMeshShellParameters & params, 
                                          MbMeshShellResults    & result );


//------------------------------------------------------------------------------
/** \brief \ru Усечь (обрезать) незамкнутое тело.
           \en Truncate an open solid. \~
  \details \ru Выполнить построение незамкнутого тела путём усечения исходного тела. \n
           \en Create an open solid by truncation the initial solid. \n \~
  \param[in] initSolid - \ru Исходная оболочка.
                         \en The initial shell. \~
  \param[in] copyMode -  \ru Режим копирования исходной оболочки.
                         \en Whether to copy the initial shells. \~
  \param[in]  params -   \ru Параметры операции.
                         \en The operation parameters. \~
  \param[out] results -  \ru Результаты операции.
                         \en The operation results. \~
  \return                \ru Возвращает код результата операции.
                         \en Returns operation result code. \~
  \ingroup Shell_Modeling
  \warning               \ru В разработке.
                         \en Under development. \~
*/
// ---
MATH_FUNC( MbResultType ) TruncateShell(       MbSolid &                initSolid,
                                               MbeCopyMode              copyMode,
                                         const MbTruncateShellParams &  params,
                                               MbTruncateShellResults & results );


//------------------------------------------------------------------------------
/** \brief \ru Построить продолжение незамкнутого тела выдавливанием.
           \en Create an extension of an open solid by extrusion. \~
  \details \ru Построить продолжение незамкнутого тела путём продления указанных краевых рёбер тела. \n
               Продление может быть выполнено следующими способами.
               Может быть удлинена на заданное расстояние указанная грань.
               К указанной грани может быть добавлена гладко стыкующаяся с ней грань.
               К указанной грани может быть добавлена грань, полученная выдавливанием крайнего ребра в заданном направлении.
           \en Create an extension of an open solid by extension of specified boundary edges of the solid. \n \~
               An extension can be performed in the following ways:
               The specified faces can be extended on the given distance.
               A smoothly connected face can be added to the given face.
               A face obtained by extrusion of a boundary edge in the given direction can be added to the specified face.
  \param[in] solid -     \ru Исходная оболочка.
                         \en The initial shell. \~
  \param[in] sameShell - \ru Режим копирования оболочки.
                         \en Whether to copy the shell. \~
  \param[in] params -    \ru Параметры продления оболочки (#MbExtensionShellParams).
                         \en Parameters of extension shell. \~
  \param[out] result -   \ru Результирующая оболочка.
                         \en The required shell. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
\ingroup Shell_Modeling
*/
//---
MATH_FUNC (MbResultType) ExtensionShell(       MbSolid &                solid,   
                                               MbeCopyMode              sameShell,
                                         const MbExtensionShellParams & params, 
                                               c3d::SolidSPtr &         result );


//------------------------------------------------------------------------------
/** \brief \ru Построить линейчатое незамкнутое тело.
           \en Create an open ruled solid. \~
  \details \ru Построить линейчатое незамкнутое тело по двум кривым, заданным в параметрах. \n
           \en Create an open ruled solid from two curves specified in parameters. \n \~
  \param[in] ruledParams - \ru Параметры для построения линейчатого незамкнутого тела.
                           \en The operation parameters for the creation an open ruled solid. \~
  \param[out] outParams - \ru Выходные параметры операции.
                          \en Output operation parameters. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbResultType) RuledShell( const MbRuledShellParams & ruledParams,
                                           MbRuledShellResult & outParams );


//------------------------------------------------------------------------------
/** \brief \ru Проверить кривую для построения линейчатого тела.
           \en Check the curve for a ruled solid creation. \~
  \details \ru Проверить вторую кривую на согласованность с первой кривой для построения
    линейчатого незамкнутого тела и выполнить необходимую модификацию второй кривой. \n
           \en Check the second curve for consistency with the first curve for creation
    of the open ruled solid and make the necessary modification of the second curve. \n \~
  \param[in] curve0 - \ru Первая кривая.
                      \en The first curve. \~
  \param[in] curve1 - \ru Вторая кривая.
                      \en The second curve. \~
  \param[out] isInverted1 - \ru Была ли вторая кривая инвертирована.
                            \en Whether the second curve was inverted. \~
  \param[out] isShifted1 - \ru Было ли смещено начало второй кривой.
                           \en Whether the beginning of the first curve was shifted. \~
  \param[in] version - \ru Версия операции.
                       \en The version of the operation. \~
  \warning \ru Вспомогательная функция операции RuledShell.
           \en An auxiliary function of operation 'RuledShell'. \~
  \ingroup Shell_Modeling
*/
//---
MATH_FUNC (void) CheckRuledCurve( const MbCurve3D & curve0,
                                  const MbCurve3D & curve1,
                                        bool &      isInverted1,
                                        bool &      isShifted1,
                                        VERSION     version );


//------------------------------------------------------------------------------
/** \brief \ru Проверить параметры кривой для построения линейчатого тела.
           \en Check the curve parameters for creation of a ruled solid. \~
  \details \ru Проверить параметры кривой и выполнить нормализацию параметров замкнутой кривой. \n
           \en Check the curve parameters and perform the normalization of a closed curve parameters. \n \~
  \param[in] curve - \ru Кривая.
                     \en The curve. \~
  \param[in,out] params - \ru Множество параметров кривой.
                          \en An array of the curve parameters. \~
  \param[in] isAscending - \ru Будет ли порядок параметров возрастающим.
                           \en Whether the parameters are specified in the ascending order. \~
  \return \ru Возвращает true, если удалось нормализовать массив параметров.
          \en Returns true if the parameter array has been successfully normalized. \~
  \warning \ru Вспомогательная функция операции RuledShell.
           \en An auxiliary function of operation 'RuledShell'. \~
  \ingroup Shell_Modeling
*/
//---
MATH_FUNC (bool) CheckRuledParams( const MbCurve3D &      curve, 
                                         SArray<double> & params, 
                                         bool             isAscending );


//------------------------------------------------------------------------------
/** \brief \ru Построить тело соединения по параметрам.
           \en Create a joint solid from parameters. \~
    \details \ru Построить незамкнутое тело соединения по параметрам. \n
             \en Create an open joint solid from parameters. \n \~
    \param[in]  parameters - \ru Параметры операции.
                             \en The operation parameters. \~
    \param[out] result     - \ru Результаты операции.
                             \en The operation results. \~
    \return \ru Возвращает код результата операции.
            \en Returns operation result code. \~
    \ingroup Shell_Modeling
*/
//---
MATH_FUNC (MbResultType) JoinShell( const MbJoinShellParams &  parameters,
                                          MbJoinShellResults & result );


//------------------------------------------------------------------------------
/** \brief \ru Разделить оболочку на части по заданному набору ребер.
           \en Divide a shell into parts using a given set of edges. \~
  \details \ru Разделить оболочку на части по заданному набору ребер. \n
               Разделение тела\оболочки выполняется путем удаления в ребрах топологических связей между смежными гранями.
               После выполнения операции весь результат будет в находится в результирующей оболочке.
               Для разделения оболочки на части, далее используйте функции CreateParts или DetachParts
           \en Divide shell into parts using a given set of edges. \n 
               Dividing of the body\shell is performed by removing topological connections in the edges between adjacent faces.
               When the operation is completed, the entire result will be in the resulting shell.
               Use the CreateParts or DetachParts functions to separate the shell into parts. \~
  \param[in] solid -     \ru Оболочка.
                         \en A shell. \~
  \param[in] params -    \ru Параметры деления оболочки.
                         \en Parameters for the shell division. \~
  \param[out] resSolid - \ru Результирующая оболочка.
                         \en The resulting shell. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
\ingroup Shell_Modeling
*/
//---
MATH_FUNC (MbResultType) DivideShell(       MbSolid             & solid,
                                            MbeCopyMode           sameShell,
                                      const MbDivideShellParams & params, 
                                            c3d::SolidSPtr      & resSolid );


//------------------------------------------------------------------------------
/** \brief \ru Проверить кривую для построения тела соединения.
           \en Check a curve for creation a joint solid. \~
  \details \ru Проверить вторую кривую на согласованность с первой кривой для построения
    незамкнутого тела соединения и выполнить необходимую модификацию второй кривой. \n
           \en Check the second curve for consistency with the first curve for creation
    of the open joint solid and make the necessary modification of the second curve. \n \~
  \param[in] curve1 - \ru Первая кривая.
                      \en The first curve. \~
  \param[in] curve2 - \ru Вторая кривая.
                      \en The second curve. \~
  \param[out] isInverted1 - \ru Была ли вторая кривая инвертирована.
                            \en Whether the second curve was inverted. \~
  \param[out] isShifted1 - \ru Было ли смещено начало второй кривой.
                           \en Whether the beginning of the first curve was shifted. \~
  \param[in] version - \ru Версия операции.
                       \en The version of the operation. \~
  \warning \ru Вспомогательная функция операции JoinShell.
           \en An auxiliary function of operation JoinShell. \~
  \ingroup Shell_Modeling
*/
//---
MATH_FUNC (void) CheckJoinedCurve( const MbCurve3D & curve1,
                                   const MbCurve3D & curve2,
                                         bool &      isInverted1,
                                         bool &      isShifted1,
                                         VERSION     version );


//------------------------------------------------------------------------------
/** \brief \ru Проверить параметры кривой для построения тела соединения.
           \en Check the curve parameters for creation of a joint solid. \~
  \details \ru Проверить параметры кривой и нормализовать параметры замкнутой кривой. \n
           \en Check the curve parameters and normalize a closed curve parameters. \n \~
  \param[in] curve - \ru Кривая.
                     \en The curve. \~
  \param[in,out] params - \ru Множество параметров кривой.
                          \en An array of the curve parameters. \~
  \param[in] isAscending - \ru Будет ли порядок параметров возрастающим.
                           \en Whether the parameters are specified in the ascending order. \~
  \return \ru Возвращает true, если удалось нормализовать массив параметров.
          \en Returns true if the parameter array has been successfully normalized. \~
  \warning \ru Вспомогательная функция операции JoinShell.
           \en An auxiliary function of operation JoinShell. \~
  \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC (bool) CheckJoinedParams( const MbCurve3D &      curve, 
                                          SArray<double> & params, 
                                          bool             isAscending );


//------------------------------------------------------------------------------
/** \brief \ru Построить кривую по множеству рёбер.
           \en Create a curve from a set of edges. \~
  \details \ru Создать кривую для поверхности соединения по списку ребер. \n
           \en Create a curve for a surface of the joint from a list of edges. \n \~
  \param[in] edges - \ru Набор ребер (контейнер константных указателей на ребра).
                     \en A set of edges (container of constant pointers to edges). \~
  \param[in] orients - \ru Ориентации ребер (контейнер булевых флагов).
                       \en Edges senses (boolean flags container). \~
  \param[in] matr - \ru Матрица преобразования ребер.
                    \en Edges transformation matrix. \~
  \param[out] res - \ru Результат операции.
                    \en The operation result. \~
  \return \ru Возвращает указатель на кривую, если ее получилось создать,
    иначе возвращает ноль.
          \en Returns a pointer to the curve if it has been successfully created,
    otherwise it returns null. \~
  \warning \ru Вспомогательная функция операции JoinShell.
           \en An auxiliary function of operation JoinShell. \~
  \ingroup Curve3D_Modeling
*/
//---
template <typename ConstEdgeVector, typename OrientsVector>
MATH_FUNC (MbCurve3D *) CreateJoinedCurve( const ConstEdgeVector & edges,
                                           const OrientsVector &   orients,
                                           const MbMatrix3D &      matr,
                                                 MbResultType &    res );


//------------------------------------------------------------------------------
/** \brief \ru Построить тело сопряжения несвязанных граней.
           \en Create a solid of two non-connected faces. \~
  \details \ru Построить незамкнутое тело, состоящее из грани скругления между двумя несвязанными гранями. \n
           \en Create an open solid that consists of a fillet face between two non-connected faces. \n \~
  \param[in] params   - \ru Параметры операции.
                        \en Operation parameters. \~
  \param[out] result  - \ru Построенная оболочка (тело).
                        \en The resultant shell (solid). \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbResultType) FacesFillet( const MbSectionData &   params,
                                            c3d::SolidSPtr & result );


//------------------------------------------------------------------------------
/** \brief \ru Построить тело на базе элементарной поверхности.
           \en Create a solid given an elementary surface. \~
  \details \ru Построить тело, состоящее из одной грани, на базе исходной элементарной поверхности. \n
           \en Create a solid which consists of a face with the specified underlying elementary surface. \n \~
  \param[in] surface - \ru Поверхность.
                       \en The surface. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \param[out] result - \ru Построенная оболочка.
                       \en The resultant shell. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbResultType) ElementaryShell( const MbSurface &    surface,
                                          const MbSNameMaker & names, 
                                                MbSolid *&     result );


//------------------------------------------------------------------------------
/** \brief \ru Построить тело на базе поверхности.
           \en Create a solid given a surface. \~
  \details \ru Построить тело, состоящее из одной грани, на базе исходной поверхности.
  Создаваемая грань использует переданную поверхность, не создавая ее копии.
  Поверхность должна быть без самопересечений, с корректной ориентацией
  ограничивающих кривых в случае поверхности MbCurveBoundedSurface. \n
           \en Create a solid which consists of a face with the specified underlying surface.
  The face uses the sent surface without creating a copy of it.
  The surface should have no self-intersections,
  the bounding curves should be correctly oriented in case of surface MbCurveBoundedSurface. \n \~
  \param[in] surface - \ru Поверхность.
                       \en The surface. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \param[out] result - \ru Построенная оболочка.
                       \en The resultant shell. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
// DEPRECATE_DECLARE_REPLACE( SurfaceShell with MbSurfaceShellParams )
MATH_FUNC (MbResultType) SurfaceShell( const MbSurface &    surface,
                                       const MbSNameMaker & names, 
                                             MbSolid *&     result );


//------------------------------------------------------------------------------
/** \brief \ru Построить тело на базе поверхности.
           \en Create a solid given a surface. \~
  \details \ru Построить тело, состоящее из одной грани, на базе исходной поверхности.
  Поверхность должна быть без самопересечений, с корректной ориентацией
  ограничивающих кривых в случае поверхности MbCurveBoundedSurface. \n
           \en Create a solid which consists of a face with the specified underlying surface.
  The surface should have no self-intersections,
  the bounding curves should be correctly oriented in case of surface MbCurveBoundedSurface. \n \~
  \param[in] params -  \ru Параметры построения оболочки по поверхности.
                       \en Parameters for creating the shell by a surface. \~
  \param[out] result - \ru Построенная оболочка.
                       \en The resultant shell. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbResultType) SurfaceShell( const MbSurfaceShellParams &    params,
                                             c3d::SolidSPtr &          result );



//------------------------------------------------------------------------------
/** \brief \ru Разрезать тело силуэтным контуром. 
           \en Cut a solid by a silhouette contour. \~
  \details \ru Построить тела, полученные в результате разрезания тела его силуэтным контуром. \n 
           \en Create solids as a result of cutting a solids by its silhouette contour. \n \~
  \param[in] params  - \ru Исходное тело, способ копирования оболочки, направление взгляда, именователь с версией.
                       \en The solid, method of shell copying, the eye's direction, an object defining the names and the version.
  \param[out] result - \ru Кривые, входящие в силуэтный контур и тела, полученные в результате применения операции. 
                       \en Curves of the silhouette contour and resultant solids. \~
  \return \ru Возвращает код результата операции.\~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbResultType) CutShellSilhouetteContour( const MbCutShellSilhouetteParams & params, 
                                                          MbCutShellSilhouetteResult & result );


//------------------------------------------------------------------------------
/** \brief \ru Сшить грани нескольких тел в одно тело.
           \en Stitch faces of several solids into single solid. \~
  \details \ru Сшить стыкующиеся друг с другом грани нескольких тел в одно тело. Ориентация граней может быть изменена. \n
           \en Stitch faces of several solids with coincident edges into single solid. The faces orientation can be changed. \n \~
  \param[in] initialSolids - \ru Множество тел для сшивки.
                             \en An array of solids for stitching. \~
  \param[in] stitchParams - \ru Параметры сшивки оболочек.
                            \en Shells stitch parameters. \~
  \param[out] resultSolid - \ru Результирующая оболочка или тело (в зависимости от флага).
                            \en The resultant shell or solid (depends on the flag). \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbeStitchResType) StitchShells( const c3d::SolidsSPtrVector & initialSolids, 
                                           const MbShellStitchParams &   stitchParams,
                                                 c3d::SolidSPtr &        resultSolid );


//------------------------------------------------------------------------------
/** \brief \ru Определение оси токарного сечения и построение кривых сечения для тела.
           \en Search for lathe axis and construction of lathe elements for the solid. \~
  \details \ru Функция выполняет поиск токарной оси граней вращения и строит токарное сечение в некоторой плоскости. \n 
           \en The function searches for lathe axis of rotation faces and builds the curves of lathe-section in a plane. \n \~
  \param[in]  params - \ru Параметры для определения оси токарного сечения и построения кривых сечения для тела.
                       \en Parameters for looking the lathe axis and construction of lathe elements for the solid. \~ 
  \param[out] result - \ru Построенные двумерные кривые сечения, расположенные в плоскости XY локальной системы координат. \n 
                       \en The two-dimensional curves located in the XY plane of the local coordinate system. \n \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbResultType) LatheCurves( const MbLatheCurvesParams & params, 
                                            MbLatheCurvesResult & result );


//------------------------------------------------------------------------------
/** \brief \ru Построение следа кривой при её вращении вокруг оси токарного сечения.
           \en Building of curves for lathe section for given curve. \~
  \details \ru Функция выполняет построение следа ребра в плоскости XY локальной системы координат при его вращении вокруг оси X. \n 
           \en The function builds the generatrix track in the XY plane of the local coordinate system as it rotates around the axis X. \n \~
  \param[in]  generatrix - \ru Кривая. \en Curve \~ 
  \param[in]  position   - \ru Плоскость, ось X которой является осью токарного сечения. \en Plane position of section, axis X is a axis of section. \~  
  \param[out] curves     - \ru Контейнер кривых, в который будет добавлен след в плоскости XY position от вращения кривой generatrix вокруг оси X. \en The curve on plane XY of position will be added to container curves. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbResultType) LatheCurve( const MbCurve3D &        generatrix, 
                                     const MbPlacement3D &    position,
                                           RPArray<MbCurve> & curves ); 


//------------------------------------------------------------------------------
/** \brief \ru Построить срединную оболочку по граням тела, основанным на эквидистантных поверхностях.
           \en Create a median shell by solid faces, based on equidistant surfaces. \~
  \details \ru Построить срединную оболочку по парам граней тела, основанным на
               эквидистантных поверхностях. Пары граней либо выбираются пользователем,
               либо находятся автоматически по заданному расстоянию между гранями.
               Грани должны принадлежать одному и тому же телу.\n
           \en Construct a median shell between pair of faces, based on equidistant
               surfaces. Pair of faces are selected by user or are found by given distance
               between faces. The faces must belong to the same body. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The initial solid. \~
  \param[in] sameShell - \ru Режим копирования тела.
                         \en Whether to copy the solid. \~
  \param[in] params - \ru Параметры построения срединной оболочки.
                      \en The median shell parameters. \~
  \param[out] result - \ru Результаты операции.
                       \en The operation results. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC( MbResultType ) MedianShell(       MbSolid &              solid,
                                             MbeCopyMode            sameShell,
                                       const MbMedianShellParams &  params,
                                             MbMedianShellResults & result );


//------------------------------------------------------------------------------
/** \brief \ru Построение развёртки грани на плоскость.
           \en Construction of a face sweep on a plane. \~
  \details \ru Построение развёртки грани на плоскость.\n
           \en Construction of a face sweep on a plane.\n \~
  \param[in] face -     \ru Исходная грань.
                        \en The initial face. \~
  \param[in] values -   \ru Параметры построения: локальная система координат развернутой поверхности грани, данные для вычисления шага при триангуляции, коэффициент Пуассона материала грани.
                        \en The parameters: Local coordinate system for result surface, Data for step calculation during triangulation, the Poisson's ratio of face material. \~
  \param[out] result -  \ru Тело - плоская развертка исходной грани.
                        \en The built solid unbend face on plane. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbResultType) RectifyFace( const MbFace &      face, 
                                      const RectifyValues values,
                                            MbSolid *&    result );


//------------------------------------------------------------------------------
/** \brief \ru Создать решетчатую оболочку.
           \en Create a lattice shell. \~
  \details \ru Создать решетчатую оболочку по трем управляющим точкам, параметрам решетки и количеству элементов. \n
           \en Create a lattice shell on the three control points of the lattice parameters and the number of elements. \~
  \param[in] params  - \ru Параметры операции.
                       \en Operation parameters. \~
  \param[out] result - \ru Построенное тело.
                       \en The constructed solid. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbResultType) OctaLattice( const MbOctaLatticeParams & params,
                                            c3d::SolidSPtr      & result );


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку на поверхности переменного сечения.
           \en Create a shell on swept mutable section surface. \~
  \details \ru Построить грань тела путём движения образующей кривой по направляющей кривой
    и выполнить булеву операцию с оболочкой, если последняя задана. \n
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Create a face of shell by moving the generating curve along the spine curve
    and perform the Boolean operation with the shell if it is specified. \n
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  solid     - \ru Оболочка, к которой дополняется построение.
                          \en The shell the construction is complemented with respect to. \~
  \param[in]  sameShell - \ru Способ копирования граней оболочки.
                          \en The method of copying faces of shell. \~
  \param[in]  data      - \ru Данные о поверхности переменного сечения. 
                          \en Data about swept mutable section surface.  \~
  \param[out] opResult  - \ru Результаты построения.
                          \en Operation results. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
*/
// ---
//DEPRECATE_DECLARE_REPLACE ( class MbFaceFilletMaker )
MATH_FUNC( MbResultType ) SectionShell( const c3d::SolidSPtr &   solid,
                                              MbeCopyMode        sameShell,
                                        const MbSectionData &    data,
                                              MbSectionResults & opResult );


//------------------------------------------------------------------------------
/** \brief \ru Построить балочную модель с постоянным поперечным сечением на основе оболочки.
           \en Create a shell-based beam model with constant cross section. \~
  \details \ru Построить балочную модель с постоянным поперечным сечением на основе оболочки.
           \en Create a shell-based beam model with constant cross section. \~
  \param[in]  solid  - \ru Тело, в котором ищутся балочные элементы.
                       \en The solid, which beam elements are searched in. \~
  \param[in]  params - \ru Входные параметры.
                       \en Input parameters. \~
  \param[out] result - \ru Результат операции.
                       \en The operation result. \~
  \result \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Shell_Modeling
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
MATH_FUNC( MbResultType ) ExtractBeamElement( const MbSolid &              solid,
                                              const MbBeamElementParams &  params,
                                                    MbBeamElementResults & results );

#endif // __ACTION_SHELL_H
