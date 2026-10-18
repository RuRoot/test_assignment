////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Настройки импорта и экспорта.
         \en Settings of import and export procedure. \~
  \details  \ru Интерфейс настроек и предопределённая реализация ConvConvertorProperty3D.
            \en Interface of settings and pre-defined implementation ConvConvertorProperty3D. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CONV_EXCHANGE_SETTINGS_H
#define __CONV_EXCHANGE_SETTINGS_H

#include <math_define.h>
#include <mb_placement3d.h>
#include <mb_data.h>
#include <op_mesh_parameter.h>
#include <conv_predefined.h>
#include <reference_item.h>
#include <conv_requestor.h>
#include <attribute_item.h>
#include <tool_cstring.h>
#include <map>

class MbProductInfo;

//------------------------------------------------------------------------------
/** \brief  \ru Константы единиц измерения. 
\en Length units constants.\~   
\ingroup Data_Interface
*/
// ---
/// \ru Миллиметры. \en Millimeters.
#define LENGTH_UNIT_MM 1.0
/// \ru Сантиметры. \en Centimeters.
#define LENGTH_UNIT_CM 10.0
/// \ru Дециметры. \en Decimeters.
#define LENGTH_UNIT_DM 100.0
/// \ru Метры. \en Meters.
#define LENGTH_UNIT_METER 1000.0
/// \ru Дюймы. \en Inches.
#define LENGTH_UNIT_INCH 25.4


//------------------------------------------------------------------------------
/** \brief  \ru Константы единиц массы. 
\en Mass units constants.\~
\ingroup Data_Interface
*/
// ---
/// \ru Граммы. \en Grams.
#define MASS_UNIT_GM 1.0
/// \ru Килограммы. \en Kilograms.
#define MASS_UNIT_KG 1000.0
/// \ru Миллиграммы. \en Milligrams.
#define MASS_UNIT_MG 0.001
/// \ru Микрограммы. \en Micrograms.
#define MASS_UNIT_MKG 1e-6
/// \ru Унции. \en Ounces.
#define MASS_UNIT_OU 28.35
/// \ru Фунты. \en Pounds.
#define MASS_UNIT_PN 453.59


#define EXPORT_DEFAULT  -1  ///< \ru По умолчанию для заданного формата.                                           \en Default for specified format.
#define EXPORT_STEP_203 203 ///< \ru STEP прикладной протокол 203 ( Проектирование с управляемой конфигурацией ).  \en STEP applied protocol STEP 203 (Configuration controlled design).
#define EXPORT_STEP_214 214 ///< \ru STEP прикладной протокол 214 ( Проектирование автомобилей ).                  \en STEP applied protocol STEP 214 (Automotive design).
#define EXPORT_STEP_242 242 ///< \ru STEP прикладной протокол 242. (по умолчанию)                                  \en STEP applied protocol STEP 242. (default) 
#define EXPORT_ACIS_4   4   ///< \ru ACIS версия 4.0.                                                              \en ACIS version 4.0.  
#define EXPORT_ACIS_7   7   ///< \ru ACIS версия 7.0 (по умолчанию).                                               \en ACIS version 7.0 (default).  
#define EXPORT_ACIS_10  10  ///< \ru ACIS версия 10.0.                                                             \en ACIS version 10.0.  


//------------------------------------------------------------------------------
/** \brief  \ru Индексы строк, передаваемых через конвертер.
\en Indices of strings, transmitted through converter.\~   
\ingroup Data_Interface
*/
// ---
enum MbeConverterStrings {
  cvs_BEGIN = 0,        ///< \ru Для удобства перебора.           \en For lookup only.
  cvs_STEPAuthor,       ///< \ru Автор для конвертера STEP.       \en Author of the document, in STEP.
  cvs_STEPOrganization, ///< \ru Организация для конвертера STEP. \en The organization, the author is related with, in STEP.
  cvs_STEPComment,      ///< \ru Комментарий файла формата STEP.  \en Annotation, in STEP.
  cvs_CAD_NAME,         ///< \ru Название САПР при экспорте.      \en CAD Name for export.
  cvs_STEPPreprocessorVersion, ///< \ru Поле Preprocerssor version из формата STEP. \en Preprocerssor version, in STEP.
  cvs_STEPAuthorization, ///< \ru Поле Authorization из формата STEP. \en Authorization, in STEP.
  cvs_END               ///< \ru Для удобства перебора.           \en For lookup only.
};


//------------------------------------------------------------------------------
/** \brief  \ru Индексы, управляющие разрешением на чтение или запись объектов.
\en Indeces, which filter imported/exported objects or properties.\~   
\ingroup Data_Interface
*/
// ---
enum MbeIOPermiss {
  iop_rSolid = 0,       ///< \ru Разрешение на чтение твёрдых тел.  \en Import solid solids.
  iop_wSolid,           ///< \ru Разрешение на запись твёрдых тел.  \en Export solid solids.
  iop_rSurface,         ///< \ru Разрешение на чтение поверхностей. \en Import surfaces.
  iop_wSurface,         ///< \ru Разрешение на запись поверхностей. \en Export surfaces.
  iop_rCurve,           ///< \ru Разрешение на чтение кривых.       \en Import curves.
  iop_wCurve,           ///< \ru Разрешение на запись кривых.       \en Export curves.
  iop_rDrafts,          ///< \ru Разрешение на чтение эскизов (не применяется).       \en Import drafts (ignored).
  iop_wDrafts,          ///< \ru Разрешение на запись эскизов.       \en Export drafts.
  iop_rInvisible,       ///< \ru Разрешение на чтение невидимых объектов (не применяется).  \en Import invisible objects (not applied).
  iop_wInvisible,       ///< \ru Разрешение на запись невидимых объектов.  \en Export invisible objects.
  iop_rPoint,           ///< \ru Разрешение на чтение точек.        \en Import points.
  iop_wPoint,           ///< \ru Разрешение на запись точек.        \en Export points.
  iop_rDocInfo,         ///< \ru Разрешение на чтение информации о документе (автор, организация, комментарии).  \en Import components info ( author, organization, description ).
  iop_wDocInfo,         ///< \ru Разрешение на запись информации о документе (автор, организация, комментарии).  \en Export components info ( author, organization, description ).
  iop_rTextDescription, ///< \ru Разрешение на чтение технических требований.  \en Import technical requirements.
  iop_wTextDescription, ///< \ru Разрешение на запись технических требований.  \en Export technical requirements.
  iop_rDimensions,      ///< \ru Разрешение на чтение размеров.     \en Import dimensions.
  iop_wDimensions,      ///< \ru Разрешение на запись размеров.     \en Export dimensions.
  iop_rAttributes,      ///< \ru Разрешение на чтение атрибутов.     \en Import attributes.
  iop_wAttributes,      ///< \ru Разрешение на запись атрибутов.     \en Export attributes.
  iop_rBRep,            ///< \ru Разрешение на чтение форм изделий в граничном представлении (только в JT).  \en Import shapes in boundary representation (JT only).
  iop_wBRep,            ///< \ru Разрешение на запись форм изделий в граничном представлении (только в JT).  \en Export shapes in boundary representation (JT only).
  iop_rPolygonal,       ///< \ru Разрешение на чтение полигональных форм изделий.  \en Import polygonal shapes.
  iop_wPolygonal,       ///< \ru Разрешение на запись полигональных форм изделий.  \en Export polygonal shapes.
  iop_rLOD0,            ///< \ru Разрешение на чтение полигональных форм изделий уровня детализации 0.  \en Import polygonal shapes of the 0-th LOD.
  iop_wLOD0,            ///< \ru Разрешение на запись полигональных форм изделий уровня детализации 0.  \en Export polygonal shapes of the 0-th LOD.
  iop_rAssociated,      ///< \ru Разрешение на чтение ассоциированной геометрии (резьбы и др).  \en Import associated geometry (threads etc).
  iop_wAssociated,      ///< \ru Разрешение на запись ассоциированной геометрии (резьбы и др).  \en Export associated geometry (threads etc).
  iop_rDensity,         ///< \ru Разрешение на чтение единиц плотности.  \en Import density units.
  iop_wDensity,         ///< \ru Разрешение на запись единиц плотности.  \en Export density units.
  iop_rValidationProperties, ///< \ru Разрешение на чтение контрольных параметров - объёма, площади поверхности, центра масс.  \en Import validation properties - volume, surface area, centroid.
  iop_wValidationProperties, ///< \ru Разрешение на запись контрольных параметров - объёма, площади поверхности, центра масс.  \en Export validation properties - volume, surface area, centroid.
  iop_rStyle,           ///< \ru Разрешение на чтение элементов оформления (цвет, начертание, и т.п.).  \en Import appearance.
  iop_wStyle,            ///< \ru Разрешение на запись элементов оформления (цвет, начертание, и т.п.).  \en Export appearance
  iop_rCS,               ///< \ru Разрешение на чтение систем координат.  \en Import coordinate sistems.
  iop_wCS,               ///< \ru Разрешение на запись систем координат.  \en Export coordinate sistems.
  iop_rBounds,           ///< \ru Разрешение на чтение объектов, ограничивающих объем или площадь.  \en Import items that is the boundary of an area or volume.
  iop_wBounds,           ///< \ru Разрешение на запись объектов, ограничивающих объем или площадь.  \en Export items that is the boundary of an area or volume.
  iop_END
};


//------------------------------------------------------------------------------
/** \brief  \ru Тип сообщения об ошибке при выводе в лог.
\en Type of a log message.\~   
\ingroup Data_Exchange
*/
// ---
enum eMsgType { 
  emt_ErrorNoId,///< \ru Ошибка формата. Значение id игнорируется, выводится только текст.  \en Error not related with a certain record. The id field is ignored.
  emt_TextOnly, ///< \ru Значение id игнорируется, выводится только текст.                  \en Used to type message only. The id field is ignored.
  emt_Info,     ///< \ru Рабочая информация.                                                \en Info.
  emt_Warning,  ///< \ru Предупреждение.                                                    \en Warning.
  emt_Error     ///< \ru Ошибка формата или неустранимая ошибка преобразования.             \en Format mismatch or fatal converting error.
};


//------------------------------------------------------------------------------
/** \brief  \ru Код подробного сообщения об ошибке при выводе в лог.
\en The key of a detailed log message.\~   
\ingroup Data_Interface
*/
// ---
enum eMsgDetail { 
  emd_Title,                          ///< \ru Заголовок файла.                 \en File header.
  emd_HEADError,                      ///< \ru Тип сообщения - ошибка.          \en Error.
  emd_HEADWarinig,                    ///< \ru Тип сообщения - Предупреждение.  \en Warning.
  emd_HEADInfo,                       ///< \ru Тип сообщения - Информация.      \en Info.
  emd_HEADDefaultMsg,                 ///< \ru Тип сообщения - Сообщение.       \en Message.

  emd_STOPFileOpenError,              ///< \ru Ошибка открытия файла.                           \en Cannot open file.
  emd_STOPFileOpenErrorOrEmpty,       ///< \ru Ошибка открытия файла или файл пуст.             \en Cannot open file or file is empty.
  emd_STOPHeaderReadError,            ///< \ru Не удалось прочитать заголовок файла.            \en Cannot read file header.
  emd_STOPNoOrBadData,                ///< \ru Файл не содержит данных или их не удалось распознать. \en File body does not exist or incorrect.
  emd_STOPIncorrectStructure,         ///< \ru Неверная структура файла.                        \en Incorrect file structure.
  emd_STOPAddressConflict,            ///< \ru Данный адрес имеют два различных объекта.        \en Two or more entities have the same id.

  emd_ErrorNoRootObject,              ///< \ru Не найден корневой объект.                       \en Root object not found.
  emd_ErrorSyntaxIncorrectFormFloat,  ///< \ru Невозможно прочитать действительную константу.   \en Error reading floating-point number.
  emd_ErrorEmptyLoop,                 ///< \ru Цикл грани пуст.                                 \en Face has an empty loop.
  emd_ErrorEmptyQueriesList,          ///< \ru Список запросов пуст. \en 
  emd_ErrorEmptyObjectsList,          ///< \ru Список объектов пуст.                            \en List of objects is empty.
  emd_ErrorEmptyGeomObjectsList,      ///< \ru Список геометрических объектов пуст.             \en List of geometric objects is empty.
  emd_ErrorEmptyShellsList,           ///< \ru Список оболочек пуст.                            \en List of shells is empty.
  emd_ErrorEmptyListOfWrieframes,     ///< \ru Список каркасов пуст.                            \en List of frames is empty.
  emd_ErrorEmptyCurveCompositesList,  ///< \ru Список компонент составной кривой пуст.          \en Composite curve has an empty list of composites.
  emd_ErrorEmptyBoundCurvesList,      ///< \ru Список граничных кривых пуст.                    \en List of boundary curves is empty.
  emd_ErrorEmptyEdgeList,             ///< \ru Список рёбер пуст.                               \en List of edges is empty.
  emd_ErrorEmptyFacesList,            ///< \ru Список граней пуст.                              \en List of faces is empty.
  emd_ErrorEmptyReferencesList,       ///< \ru Список ссылок пуст.                              \en List of references is empty.
  emd_ErrorEmptyOrMore2ReferencesList,///< \ru Список ссылок пуст или содержит более 2 элементов. \en List of references is empty or contains more than 2 items.
  emd_ErrorUndefinedFaceSurfaceRef,   ///< \ru Ссылка на базовую поверхность грани не определена. \en Invalid reference to base surface.
  emd_ErrorUndefinedBaseCurveRef,     ///< \ru Ссылка на базовую кривую не определена.          \en Invalid reference to base curve.
  emd_ErrorRadiusTooCloseToZero,      ///< \ru Радиус слишком мал.                              \en Too small radius.
  emd_ErrorRadiusValueNegative,       ///< \ru Отрицательное значение радиуса.                  \en Negative value of radius.
  emd_ErrorEllipseAxisTooCloseToZero, ///< \ru Длина полуоси эллипса слишком мала.              \en Ellipse axis is too short.
  emd_ErrorEllipseAxisNegative,       ///< \ru Отрицательная длина полуоси эллипса.             \en Ellipse axis length is negative.
  emd_ErrorNegativeDegree,            ///< \ru Отрицательный порядок сплайна.                   \en Negative spline order.
  emd_ErrorNegativeUDegree,           ///< \ru Отрицательный порядок сплайновой поверхности по U. \en Spline surface order along U is negative.
  emd_ErrorNegativeVDegree,           ///< \ru Отрицательный порядок сплайновой поверхности по V. \en Spline surface order along V is negative.
  emd_ErrorDegreeFixImpossible,       ///< \ru Невозможно исправить порядок сплайна.            \en Cannot fix spline order.
  emd_ErrorPolylinePointListLess2,    ///< \ru Список точек ломаной содержит менее 2 элементов. \en Polyline contains less then 2 points.
  emd_ErrorPointListLess2,            ///< \ru Список точек содержит менее 2 элементов.         \en List of points contains less then 2 points.
  emd_ErrorKnotsListLess2,            ///< \ru Список узлов содержит менее 2 элементов.         \en List of knots contains less then 2 values.
  emd_ErrorWeightsListLess2,          ///< \ru Список весов содержит менее 2 элементов.         \en List of weights contains less then 2 values.
  emd_ErrorUPointListLess2,           ///< \ru Список точек по U содержит менее 2 элементов.    \en List of points along U contains less then 2 points.
  emd_ErrorUKnotsListLess2,           ///< \ru Список узлов по U содержит менее 2 элементов.    \en List of knots along U contains less then 2 values.
  emd_ErrorUWeightsListLess2,         ///< \ru Список весов по U содержит менее 2 элементов.    \en List of weights along U contains less then 2 values.
  emd_ErrorVPointListLess2,           ///< \ru Список точек по V содержит менее 2 элементов.    \en List of points along V contains less then 2 points.
  emd_ErrorVKnotsListLess2,           ///< \ru Список узлов по V содержит менее 2 элементов.    \en List of knots along V contains less then 2 values.
  emd_ErrorVWeightsListLess2,         ///< \ru Список весов по V содержит менее 2 элементов.    \en List of weights along V contains less then 2 values.
  emd_ErrorListsSizeMismatch,         ///< \ru Размеры списков не согласуются.                  \en Lists size mismatch.
  emd_ErrorKnotsWeightsListsOrderMismatch,                ///< \ru Размеры списков узлов и весов не согласуются с порядком сплайна. \en Sizes of knots and weights lists do not agree with the spline order.
  emd_ErrorKnotsWeightsListsSizeMismatch,                 ///< \ru Размеры списков узлов и весов не согласуются. \en Size of knots list does not agree with the size of the list of weights.
  emd_ErrorUKnotsWeightsListsSizeMismatch,                ///< \ru Размеры списков узлов и весов по U не согласуются. \en Sizes of knots and weights lists along U do not agree.
  emd_ErrorVKnotsWeightsListsSizeMismatch,                ///< \ru Размеры списков узлов и весов по V не согласуются. \en Sizes of knots and weights lists along V do not agree.
  emd_ErrorSplineCurveNotCreatedUndefinedKnotsVector,     ///< \ru Сплайновая кривая не создана - не определёны узлы. \en Cannot create spline, because knots are not defined.
  emd_ErrorSplineSurfaceNotCreatedUndefinedKnotsVectors,  ///< \ru Сплайновая поверхность не создана - не определёны узлы. \en Cannot create spline surface, because knots are not defined.
  emd_ErrorInCorrectSplineSurfaceData,                    ///< \ru Неверно заданы параметры NURBS поверхности. \en Spline surface parameters are not valid.

  emd_WarningNoSectionTerminator,             ///< \ru Маркер завершения раздела не обнаружен.  \en Section terminator not found.
  emd_WarningSyntaxMultipleDotInFloat,        ///< \ru Повторяющаяся точка в действительном числе. \en Too many dots in a floating-point number.
  emd_WarningSyntaxMultipleEInFloat,          ///< \ru Повторяющаяся E в действительном числе.  \en Too many E signs in a floating-point number.
  emd_WarningLoopNotClosed,                   ///< \ru Цикл не замкнут.                         \en Loop is not closed.
  emd_WarningContourNotClosed,                ///< \ru Контур не замкнут.                       \en contour is not closed.
  emd_WarningUndefinedRef,                    ///< \ru Ссылка не определена.                    \en Invalid reference.
  emd_WarningToroidalSurfaceDegenerated,      ///< \ru Тороидальная поверхность вырождена.      \en Toroidal surface is degenerate.
  emd_WarningUndefinedBasisCurve,             ///< \ru Не определена базовая кривая.            \en Base curve not defined.
  emd_WarningUndefinedSweptCurve,             ///< \ru Не определена образующая кривая.         \en Generatrix curve is not defined.
  emd_WarningUndefinedExtrusionDirection,     ///< \ru Не определено направление выдавливания.  \en Extrusion direction is not defined.
  emd_WarningUndefinedAxis,                   ///< \ru Не определена ось.                       \en Axis is not defined.
  emd_WarningUndefinedAxisOfRevolution,       ///< \ru Не определена ось вращения.              \en Rotation axis is not defined.
  emd_WarningUndefinedBasisSurface,           ///< \ru Не определена базовая поверхность.       \en Base surface is not defined.
  emd_WarningUndefinedRepresentation,         ///< \ru Не определено представление.             \en Representation is not defined.
  emd_WarningUndefinedTransformationOperator, ///< \ru Не определён оператор преобразования.    \en Transformation is not defined.
  emd_WarningUndefinedObjectTransformBy,      ///< \ru Не определён объект, по которому ведётся преобразование. \en Basic object of transformation is not defined.
  emd_WarningUndefinedObjectToTransform,      ///< \ru Не определён преобразуемый объект.       \en No object to transform is defined.
  emd_WarningUndefinedCurve,                  ///< \ru Не определена кривая.                    \en Curve is not defined.
  emd_WarningUndefinedCompositeSegment,       ///< \ru Не определён сегмент составной кривой.   \en Composite curve segment is not defined.
  emd_WarningUndefinedDirection,              ///< \ru Не определено направление.               \en Direction is not defined.
  emd_WarningUndefinedAxisDirection,          ///< \ru Не определено направление оси.           \en Axis direction is not defined.
  emd_WarningDegeneratedItemWasSkipped,       ///< \ru Проигнорирован (пропущен) вырожденный объект. \en Degenerate object was missed.
  emd_WarningFloatParceFailureDefaultUsed,    ///< \ru Ошибка разпознавания числа с плавающей точкой, подставлено значение по умолчанию. \en Floating point value couldn't be parced; default value was used.
  emd_WarningSameShapeEdgeTwiceInLoop,        ///< \ru В цикле дважды встречается одинаковое ребро. \en Edge based on same curves twice enters a loop.
  emd_WarningIncorrectFaceWasNotAddedToShell, ///< \ru Некорректная грань не была добавлена в оболочку. \en Incorrect face was not added to shell.
  emd_WarningBoundsNotConnectedWithSeams,     ///< \ru Границы замкнутой грани не стыкуются со швами. \en Bounds of periodic face not connected with seams.
  emd_WarningIntCurveWasReplacedBySegment,    ///< \ru Кривая пересечения была заменена отрезком. \en Intersection curve was replaced by segment.
  emd_WarningOpenSolidsNotSupported,          ///< \ru Формат не поддерживает запись незамкнутых тел. \en Format does not support export of open solids.
  emd_WarningOpenMeshesNotSupported,          ///< \ru Формат не поддерживает запись незамкнутых полигональных объектов. \en Format does not support export of open meshes.
  emd_WarningClosedSolidBecameOpenMesh,       ///< \ru В результате триангуляции замкнутого тела сформировался незамкнутый полигональный объект. \en Triangulation of closed solid created an open mesh.
  emd_WarningEmptyMeshSkipped,                ///< \ru Пропущен полигональный объект, не содержащий триангуляций. \en Mesh without grids is skipped.
  emd_WarningTriangulationFailed,             ///< \ru Не удалось выполнить триангуляцию. Объект пропущен. \en Triangulation failed. Object is skipped.

  emd_MessageWeightsFilled,                        ///< \ru Веса заданы. \en Weights are set.

  emd_ErrorEdgeCurveFlagTSingleRedefinition,   ///< \ru При создании ребра в конвертере дважды указана грань с флагом .T.. \en Double .T. face inclusion.
  emd_ErrorEdgeCurveFlagFSingleRedefinition,   ///< \ru При создании ребра в конвертере дважды указана грань с флагом .F.. \en Double .F. face inclusion.
  emd_ErrorSTEPEdgeCurveFlagTMultipleRedefinition, ///< \ru При создании ребра в конвертере STEP более чем дважды указана грань с флагом .T.. \en Multiple .T. face inclusion in STEP.
  emd_ErrorSTEPEdgeCurveFlagFMultipleRedefinition, ///< \ru При создании ребра в конвертере STEP более чем дважды указана грань с флагом .F.. \en Multiple .F. face inclusion in STEP.
  emd_ErrorSTEPUndefinedFaceGeometry,   ///< \ru Не определена геометрия грани в конвертере STEP. \en Face geometry is not defined in STEP.
  emd_ErrorSTEPSyntaxMultipleDotInEnum, ///< \ru Синтаксическая ошибка в файле формата STEP - в перечислении символ "." встречается более 1 раза подряд. \en Too many dots in a enumeration record in STEP.
  emd_WarningSTEPPointCorrection,       ///< \ru Скорректированы координаты точки. \en Point location corrected. ( by BUG_73871 )
  emd_WarningSTEPEdgeCurveByVertices,   ///< \ru Кривая ребра скорректирована с учётом координат вершин. \en Edge curve corrected in accordance with vertices. ( by BUG_73871 )
  emd_MessageSTEPFlagChangedToF,        ///< \ru Произведена замена флага на .F.. \en Flag was set as .F. in STEP.
  emd_MessageSTEPFlagChangedToT,        ///< \ru Произведена замена флага на .T.. \en Flag was set as .T. in STEP.
  emd_WarningBooleanUndefined,          ///< \ru Булево значение не определено. \en Boolean value not defined.

  emd_WarningACISUnsupportedInterpoleCurveType,  ///< \ru Данный подтип ACIS интерполяционной кривой не поддерживается. \en Interpolation curve type is not supported by SAT converter.
  emd_WarningACISUnsupportedParametricCurveType, ///< \ru Данный подтип ACIS параметрической кривой не поддерживается. \en Parametric curve type is not supported by SAT converter.
  emd_WarningACISUnsupportedSplineSurfaceType,   ///< \ru Данный подтип ACIS сплайновой поверхности не поддерживается. \en Spline surface type is not supported by SAT converter.
  emd_ErrorACISUnsupportedVersion,               ///< \ru Данная версия ACIS NT не поддерживается. \en Th version of file is not supported by SAT converter.
  emd_WarningACISCannotImportEntityId,           ///< \ru Не удалось импортировать объект с данным Id. \en Cannot import this object by SAT converter.
  emd_WarningACISIncorrectIntAttribute,          ///< \ru Некорректный целочисленный атрибут. \en Incorrect integer attribute.
  emd_WarningVRMLGridDuplicatesInMeshes,         ///< \ru Присутствуют дубликаты объектов в сетках. \en There are grid duplicates in meshes.
  emd_WarningACISLawIntCurveIsNotCreated,        ///< \ru Кривая по закону не создана. \en Law intersection curve is not created.

  emd_ErrorIGESIncorrectExternalReference,       ///< \ru Неверное имя внешней ссылки. \en Invalid external reference in IGES.

  emd_ErrorSTLTooManyTrianglesForBinary,         ///< \ru Триангуляция исходной модели содержит больше треугольников, чем допустимо стандартом ( не выражается 32-битным беззнаковым числом ) ( by BUG_71422 ). \en Too many triangles (not represented by unsigned 32-bit number) for export to binary STL.

  emd_ErrorXTUnsupportedVersion,                 ///< \ru Данная версия X_T не поддерживается. \en Th version of file is not supported by X_T converter.

  emd_ErrorJTUnsupportedVersion,                 ///< \ru Данная версия JT не поддерживается. \en Th version of file is not supported by JT converter.

  emd_ErrorStringEncoding,                       ///< \ru Ошибка кодировки строки. \en String encoding error.

  emd_WarningCurveParametrizationCorrected,      ///< \ru Исправлена параметризация кривой. \en Curve parameterization was corrected.

  emd_ErrorJTDuplicatedTriangles,                ///< \ru Сетка содержит дублирующиеся треугольники и не может быть записана. \en Mesh contains duplicate triangles and cannot be written.
  
  emd_WarningCreateEdgeBadInput,                 ///< \ru Некорректные исходные данные для кривой пересечения. \en Incorrect input data for intersection curve.

  emd_PluginGeneralProblem,                      ///< \ru Сбой общего характера в плагине. \en General plugin fault.
  emd_PluginLicenceProblem,                      ///< \ru Ошибка активации плагина. \en Plugin couldn't be activated.
  emd_PluginFunctionalityLimitation,             ///< \ru Плагин не обладает требуемой функциональностью. \en Plugin' funtionality doesn't cover the required case.
  emd_PluginImportPartialSuccess,                ///< \ru Модель частично импортирована через плагин. \en Model was partially imported via the plugin.
  emd_ErrorModelHasNoGeometry,                   ///< \ru Модель не содержит геометрии. \en Model does not contain geometry.
  emd_ErrorReferencedModelFileNotFound,          ///< \ru Файл, на который ссылается модель не найден. \en File which is referenced by the model not found.

  emd_InfoMetadataSuccessfullyImported,          ///< \ru Метаданные импортированы успешно. \en Metadata imported successfully.
  emd_WarningMetadataPartiallyImported,          ///< \ru Метаданные импортированы не полностью. \en Metadata imported partially.
  emd_InfoMetadataNotSupported,                  ///< \ru Импорт метаданных не поддерживается. \en Metadata import not supported.
  emd_InfoModelContainsNoMetadata,               ///< \ru Модель не содержит метаданных. \en Model contains no metadata.
  emd_ErrorInvalidTargetDirectoryForMetadata,    ///< \ru Не удалось записать метаданные в указанную директорию. \en Failed to write metadata into specified directory.
  emd_ErrorMetadataFileWriteFailure,             ///< \ru Не удалось записать файл с метаданными. \en Failed to write metadata file.
  emd_ErrorMetadataImportError                   ///< \ru Сбой импорта метаданных. \en Metadata import failure.
 };


 //------------------------------------------------------------------------------
/** \brief  \ru Режим логирования.
\en Logging mode.\~   
\ingroup Exchange_Interface
*/
// ---
 enum eLoggingMode {
   elm_LoggingOff,     ///< \ru Логирование отключено.                            \en Logging disabled.
   elm_LoggingAll,     ///< \ru Логирование включено для всех объектов.           \en Logging enabled for all objects.
   elm_LoggingForId    ///< \ru Логирование включено для объекта с заданным id.   \en Logging enabled for an object with the specified id.
 };


//------------------------------------------------------------------------------
/** \brief \ru Настройки отладочного функционала C3D Converter.
           \en Settings of the debug features of the C3D Converter. \~
    \details \ru Предназначен для для получения информации, облегчающей диагностику и отладку конвертера.
             \en Intended for getting information to make diagnostics and debugging of converter easier. \~
\ingroup Exchange_Interface
*/
struct C3DConverterDebugSettings {
  /// \ru Включить вывод отладочной информации в CERR. \en Enable debug info out into the CERR stream.
  bool enableCERRout { false };
  /// \ru Включить вывод обхода дерева модели, реализованной на стороне пользователя. \en Enable logging the traverse of the user-implemented model tree.
  bool cerrOutUserTreeTraverse { false };
  /// \ru Включить вывод обхода промежуточного дерева модели. \en Enable logging the traverse of the intermediate model tree.
  bool cerrOutIntermediateTreeTraverse { false };
  
  /** \brief \ru Сохранить двойник модели.
             \en Save the twin of the model. \~
      \details \ru При актавации этого флага при экспорте формируется дополнительно файл, предназначенный для приложения-двойника
      с точки зрения значений возвращаемых реализацией модельного документа.
                \en If this option active, an additional model for the twin application is written. It returns the same values as
                the original implementation of the model document.\~
  
  */
  bool saveModelTwin { false };

  /// \ru Включить вывод статистики импортируемых объектов. \en Enable logging the statistic of imported objects.
  bool cerrOutImportStatistic { false };

  /// \ru Добавлять целочисленный атрибут со значением id элемента из обменного файла. \en Attach int attribute which's value based on id from exchange file.
  bool attachThisIdAttribute { false };

  /// \ru Идентификатор элемента, для которого сделать вывод информации для тонкой отладки. \en Id of element for each save data for fine debugging.
  ptrdiff_t elementIdFineDebug { -1 };

  /// \ru Путь для сохранения информации для тонкой отладки. \en Fine for saving data for fine debugging.
  c3d::string_t pathFineDebug {};

  /// \ru Режим логирования. \en Logging mode.
  eLoggingMode loggingMode { elm_LoggingOff };

  /// \ru Идентификатор для логирования. \en Id for logging.
  ptrdiff_t idForLogging { -1 };

  /// \ru Конструктор. \en Conctuctor.
  C3DConverterDebugSettings() = default;
  ~C3DConverterDebugSettings() = default;
};


//------------------------------------------------------------------------------
/** \brief \ru Интерфейс генератора однострочного идентификатора компонента.
           \en Interface of component's identifier generator. \~
    \details \ru Предназначен для для экспорта в форматы, в которых для идентификации компонента предусмотрено одно строковое значение.
             \en Demanded for export to formats having one string field for product inetifier. \~
\ingroup Exchange_Interface
*/
struct CONV_CLASS IProductIdMaker : public MbRefItem
{
  virtual c3d::string_t operator()( const MbProductInfo& ) const = 0;
  virtual c3d::string_t operator()( const c3d::AttrSPtrVector& ) const = 0;
};

//------------------------------------------------------------------------------
/** \brief \ru Интерфейс для  преобразования строк.
           \en An interface for string encoding. \~
  \ingroup Base_Tools_String
*/
class CONV_CLASS IC3DCharEncodingTransformer : public MbRefItem
{
public:
  virtual ~IC3DCharEncodingTransformer() {}

  //------------------------------------------------------------------------------
  /** \brief \ru Преобразование строки из wchar_t* в char*.
             \en Transform string from wchar_t* to char*. \~
    \ingroup Base_Tools_String
  */
  virtual bool C3DToStd( const c3d::string_t& from, std::string& to ) = 0;

  //------------------------------------------------------------------------------
  /** \brief \ru Преобразование строки из char* в wchar_t*.
             \en Transform string from char* в wchar_t*. \~
    \ingroup  Base_Tools_String
  */
  virtual bool StdToC3D( const std::string& from, c3d::string_t& to ) = 0;
};

//------------------------------------------------------------------------------
/** \brief \ru Интерфейс свойств конвертера.
           \en Interface of converter's properties. \~
    \details \ru Интерфейс свойств конвертера реализует выдачу имени документа и других сведений о нём, таких как автор,
    и управление режимами работы - сшивкой поверхностей с возможностью создания твёрдых тел, фильтрацией объектов, 
    формирование журнала трансляции.
             \en Interface of converter's properties realizes getting the document's name and other information about it, 
             such as the author, and management of modes of operations - stitching of surfaces with possibility of solids 
             creation, objects filtration, generation of translation journal. \~
\ingroup Exchange_Interface
*/
class CONV_CLASS IConvertorProperty3D {
public :
  virtual ~IConvertorProperty3D() = default;

public:
  /// \ru Получить имя документа. \en Get document's name. 
  DEPRECATE_DECLARE
  virtual const std::string GetDocumentName   () const = 0; //{ return std::string( GetDocName().get_str() ); };
   /// \ru Получить имя файла для конвертирования. \en Get file name for converting. 
  //DEPRECATE_DECLARE
  virtual const c3d::path_string FullFilePath() const = 0;//{ return c3d::path_string( GetFileName().c_str() ); };
  /// \ru Является ли файл текстовым. \en Whether the file is a text file. 
  virtual bool           IsFileAscii      () const = 0; 
  /// \ru Получить версию формата при экспорте. \en Get the version of format for export.
  virtual long int       GetFormatVersion () const { return EXPORT_DEFAULT; };
  /// \ru Следует ли экспортировать только поверхности ( введено для работы конвертера IGES ). \en Whether to export only surfaces (introduced for work with converter IGES ). 
  virtual bool           IsOutOnlySurfaces() const = 0;
  /// \ru Является ли экспортируемый документ сборкой. \en Whether the document for export is an assembly. 
  DEPRECATE_DECLARE
  virtual bool           IsAssembling     () const = 0; 
  /// \ru Получить значение разрешения на импорт экспорт объектов определенного типа. \en Get the value of permission for import-export of objects of a certain type. 
  virtual bool           GetIoPermission( MbeIOPermiss nPermission ) const = 0; 
  /// \ru Получить значения разрешений на импорт экспорт объектов определенных типов. \en Get values of permission for import-export of objects of certain types. 
  virtual void           GetIoPermissions( std::vector<bool>& ioPermissions ) const = 0; 
  /// \ru Установить разрешение на импорт экспорт объектов определенного типа. \en Set permission for import-export of objects of a certain type. 
  virtual void           SetIoPermission( MbeIOPermiss nPermission, bool set ) = 0; 
  /// \ru Получить строку заголовка файла. \en Get file header string.
  virtual bool           GetPropertyString ( MbeConverterStrings nString, std::string &  propertyString ) const = 0;
  /// \ru Установить строку заголовка файла. \en Set file header string. 
  virtual void           SetPropertyString ( MbeConverterStrings nString, const std::string &  propertyString ) = 0;
  /// \ru Представление текста в аннотационных объектах. \en Text representation in annotation objects. 
  DEPRECATE_DECLARE
  virtual eTextForm      GetAnnotationTextRepresentation () const { return exf_TextOnly; }
  /**  \brief \ru Следует ли компоненты экспортировать в разные файлы (если позволяет формат).
              \en Export components into separate files ( if provided in format). \~
       \note  \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL \~.    
  */
  DEPRECATE_DECLARE
  virtual bool           ExportComponentsSeparately() const { return false; }
  /// \ru Получить ЛСК, относительно которой позиционирована модель. \en Get the location, the model is placed in.
  virtual MbPlacement3D  GetOriginLocation() const = 0;
  /// \ru Заменять ли принудительно СК компонент на правые. \en Replace components' placements to right-oriented.
  virtual bool           ReplaceLocationsToRight() const = 0;
  /**  \brief \ru Сшивать ли поверхности автоматически.
              \en If surfaces should be stitched automatically. \~
       \return \ru true - Сшивать поверхности автоматически, false - Спросить пользователя, сшивать ли поверхности.
               \en true - Stitch surfaces automatically, false - Ask user first time. \~
       \param[out] stitchPrecision - \ru Точность сшивки.
                                     \en Stitch precision. \~
  */
  virtual bool           EnableAutoStitch( double& /*stitchPrecision*/ ) const = 0;

  /**  \brief \ru Получить множитель единиц длины по отношению к миллиметру. 
              \en Get the factor of the length units to millimeters. \~
        \details \ru При импорте, если единицы измерения не заданы явно с помощью средств, предоставляемых обменным форматом,
  все размеры (координаты точек, радиусы) умножаются на возвращаемое значение. При экспорте либо с помощью 
  средств, предоставляемых обменным форматом, задаются единицы измерения, либо все размеры модели (координаты 
  точек, радиусы) умножаются на возвращаемое значение.
                \en During the import all spatial objects (coordinate values, radiuses) are multiplied by the returned value, 
  unless the scale factor comes from the exchange file. During the export the exchange format facilities are 
  used to specify the length units or all spatial objects (coordinate values, radiuses) are multiplied by the 
  returned value. \~
  */
  virtual double         LengthUnitsFactor() const { return LENGTH_UNIT_MM; } 


  /**  \brief \ru Получить дополнительный множитель единиц длины по отношению к миллиметру в модели приложения. 
              \en Get addifional factor of the length units to millimeters in the application model. \~
       \details \ru При импорте из всех форматов за исключением JT, если единицы измерения, в том числе и заданные 
  явно с помощью средств, предоставляемых обменным форматом, все размеры (координаты точек, радиусы) умножаются 
  на возвращаемое значение. При экспорте либо с помощью средств, предоставляемых обменным форматом, задаются 
  единицы измерения, либо все размеры модели (координаты точек, радиусы) умножаются на возвращаемое значение.
                \en During the import from all formats except for JT all spatial objects (coordinate values, radiuses) are 
  multiplied by the returned value, even if the scale factor comes from the exchange file. During the export the 
  exchange format facilities are used to specify the length units or all spatial objects (coordinate values, 
  radiuses) are multiplied by the returned value. \~
  */
  virtual double         AppLengthUnitsFactor() const { return LENGTH_UNIT_MM; } 

  /**  \brief \ru Сделать запись в журнал конвертирования.
              \en Make a record in the converter report. \~
  \param[in] id - \ru Идентификатор элемента внутри файла стороннего формата.
                  \en Identifier of an element inside the file of a foreign format. \~
  \param[in] msgType - \ru Тип сообщения.
                       \en Message type. \~
  \param[in] msgText - \ru Код сообщения.
                       \en Message code. \~
  */
  virtual void           LogReport( ptrdiff_t id, eMsgType msgType, eMsgDetail msgText ) = 0;

  //   /**  \brief \ru Следует ли показывать сообщения и диалоги пользователю. \en Whether to show messages and dialog to the user. \~
  //    \details \ru Обеспечивает работу через API. \en Provide possibility for work via API. \~
  //    \return \ru true - обычная работа, false - через API. \en true - ordinary work, false - via API. \~
  //   */
  virtual bool          CanShowMessages() const = 0;

  /// \ru Дать данные вычисления триангуляции (для конвертера JT, STL и VRML). \en Get data for step calculation during triangulation (for JT, STL, VRML only).
  virtual MbStepData    TesselationParameters() const { return MbStepData(); }

  /// \ru Дать данные вычисления триангуляции уровня детализации (для конвертера JT). \en Get data for step calculation during triangulation of LOD0 (for JTonly).
  virtual MbStepData    LOD0TesselationParameters() const { return TesselationParameters(); }

  /// \ru Флаг сохранения совпадающих точек швов. (для конвертера STL и VRML). \en Whether to keep coincident points of seams (for STL, VRML only).
  virtual bool          DualSeams() const { return true; }

  /// \ru Выполнять ли слияние подобных граней. \en Whether to join similar faces.
  virtual bool          JoinSimilarFaces() const { return true; }

  /// \ru Добавлять ли удаленные грани в качестве оболочек. \en Whether to add removed faces as shells.
  virtual bool          AddRemovedFacesAsShells() const { return false; }

  /// \ru Получить запросчик масштабного коэффициента единиц длины. \en Get scale requester.
  virtual SPtr<IScaleRequestor> GetScaleRequester() const { return SPtr<IScaleRequestor>{}; }

  /// \ru Получить запросчик сшивки. \en Get stitch requester.
  virtual SPtr<IStitchRequestor> GetStitchRequester() const { return SPtr<IStitchRequestor>{}; }

  /// \ru Получить генератор однострочного идентификтора изделия. \en Get generator of one-line product identifier.
  virtual SPtr<IProductIdMaker> ProductIdentifierGenerator() const { return SPtr<IProductIdMaker>(); }

  /// \ru Получить настройки для выдачи отладочной информации. \en Get the settings of debug info.
  virtual C3DConverterDebugSettings GetDebugSettings() const { return C3DConverterDebugSettings(); };

  /// \ru Получить пользовательский преобразователь строк. \en Get user string transformer.
  virtual SPtr<IC3DCharEncodingTransformer> GetUserCharEncodingTransformer() const { return SPtr<IC3DCharEncodingTransformer>( nullptr ); }

  /// \ru Создавать раскрашенные копии компонент при импорте. \en Create colored replicas of components on import.
  virtual bool           ImportComponentsWithColoredReplica() { return false; }

  /// \ru Определить способ формирования визуальных свойст при чтении из C3D. \en Specify the way how visual properties form while reading from C3D.
  virtual ColorProcessing GetC3DVisualPropertiesResolutionStrategy() const { return ColorProcessing::cr_Both; }

  /// \ru Импортировать полигональные объекты как объекты с топологической информацией. \en Import polygonal objects as objects with topological information.
  virtual bool           ImportMeshesWithTopology() const { return false; }

  /// \ru Лечить ли неправильные сетки, и параметры для лечения. \en Whether to repair inconsistent meshes, and parameters for repairing.
  virtual bool           RepairInconsistentMesh( MbRepairInconsistentMeshParams & ) const { return false; }


  /// \ru Импортировать атрибуты вставок (STEP). \en Import instances attributes (STEP).
  virtual bool           ImportInstanceAttributes() const { return true; }

}; // IConvertorProperty3D


//------------------------------------------------------------------------------
/** \brief \ru Реализация генератора однострочного идентификатора компонента.
           \en Implementation of component's identifier generator. \~
    \details \ru Реализация по умолчанию предполагает передачу наименования компонента.
             \en Default implementatio implies export of component's name. \~
\ingroup Exchange_Interface
*/
struct NameProductIdMaker : public IProductIdMaker
{
  virtual c3d::string_t operator()( const MbProductInfo& ) const;
  virtual c3d::string_t operator()( const c3d::AttrSPtrVector& ) const;
};


//------------------------------------------------------------------------------
/** \brief \ru Предопределённая реализация интерфейса свойств конвертера.
           \en Pre-defined implementation of converter's properties. \~
\ingroup Exchange_Interface
*/
class CONV_CLASS ConvConvertorProperty3D : public IConvertorProperty3D {
public:  
  std::string       docName;  ///< \ru Имя документа. \en Document name.  
  c3d::path_string  fileName; ///< \ru Имя файла. \en File name.
  bool              fileASCII; ///< \ru Экспортировать ли в текстовый файл (если формат поддерживает двоичный). \en Export to text file (if format supports binary one).
  long int          formatVersion; /// \ru Версия формата при экспорте. \en The version of format for export.
  bool              exportIGESTopology; ///< \ru Экспортировать ли топологию в IGES. \en Export topology items into IGES.
  std::vector<bool> ioPermissions; ///< \ru Фильтр объектов по типам. \en Type objects filter.
  SPtr<IC3DCharEncodingTransformer>          userEncodingTransformer; ///< \ru Пользовательский преобразователь строк. \en User string transformer. 
  std::map<MbeConverterStrings, std::string> propertyStrings; ///< \ru Особые значения сведений о документе. \en Specific values of documents properties.
  eTextForm         annotTextReprSTEP; ///< \ru Представление текста элементов аннотации. \en Text representation in annotation items.
  MbPlacement3D     originLocation; ///< \ru ЛСК документа. \en Own placement of the document.
  bool              replaceLocationsToRight; ///< \ru Следует ли принудительно преобразовывать ЛСК объектов к правым (для форматов, допускающих левые). \en Force replacement of locations to right ones.
  bool              enableAutostitch; ///< \ru Сшивать ли поверхности автоматически. \en Automatically stitch surfaces into shells.
  double            autostitchPrecision; ///< \ru Точность сшивки. \en Stitch precision.
  bool              showMessages; ///< \ru Отображать ли сообщения. \en Invoke messages show.
  MbStepData        tesseleationStepData; ///< \ru Параметры триангуляции при экспорте в STL и VRML. \en Tessellation parameters for export into STL and VRML.
  MbStepData        LOD0StepData; ///< \ru Параметры триангуляции при экспорте в JT. \en Tessellation parameters for export into JT.
  bool              dualSeams; ///< \ru Признак сдваивания швов при экспорте в STL и VRML. \en Make dual seams when export into STL and VRML.
  bool              joinSimilarFaces; ///< \ru Выполнять ли слияние подобных граней. \en Whether to join similar faces.
  bool              addRemovedFacesAsShells; ///< \ru Добавлять ли удаленные грани в качестве отдельных оболочек. \en Whether to add removed faces as shells.
  double            lengthUnitsFactor; ///< \ru Единицы длины модели. \en Length units of the model.
  double            appUnitsFactor; ///< \ru Единицы длины модели пользовательского приложения. \en Length units of the model used in user application.
  bool              importMeshesWithTopology; ///< \ru Сохранять сетки в MbMesh (false) или в MbPolymesh (true). \en Save meshes to MbMesh (false) or to MbPolymesh (true).
  bool              repairInconsistentMesh; /// \ru Лечить ли неправильные сетки. \en Whether to repair inconsistent meshes.
  bool              importInstanceAttributes; ///< \ru  Импортировать ли атрибуты вставок (STEP). \en Whether to import instances attributes (STEP).
  MbRepairInconsistentMeshParams repairInconsistentMeshParams; /// \ru Параметры для лечения неправильных сеток. \en Parameters for repairing inconsistent meshes.
  ColorProcessing   visualPropertiesFormation; ///< \ru Способ формирования визуальных свойст при чтении из C3D. \en The way how visual properties form while reading from C3D.
  SPtr<IProductIdMaker> pruductIdMaker; ///< \ru Генератор однострочного идентификатора по данным об изделии. \en Generator of a single-line id based on product information attribute.
  SPtr<IScaleRequestor> scaleRequester; /// \ru Запросчик масштабного коэффициента единиц длины. \en Scale requester.
  SPtr<IStitchRequestor> stitchRequester; /// \ru Запросчик сшивки. \en Stitch requester.
  C3DConverterDebugSettings debugSettings;
  
  /// \ru Сведения о сообщениях конвертера. \en Converter message data.
  struct LogRecord {
    ptrdiff_t id; ///< \ru Идентификатор записи. \en Record id.
    eMsgType msgType; ///< \ru Тип сообщения. \en Message type.
    eMsgDetail msgText; ///< \ru Код сообщения. \en Message code.
  };
  
  std::vector< LogRecord > logRecords; ///< \ru Сообщения конвертера. \en Converter messages.

public:

  ConvConvertorProperty3D(); ///< \ru Конструктор. \en Constructor.
  ~ConvConvertorProperty3D() override = default;///< \ru Деструктор. \en Destructor.

  /// \ru Получить имя документа. \en Get document's name. 
  const std::string GetDocumentName   () const override { return docName; };

  /// \ru Получить имя файла для конвертирования. \en Get file name for converting. 
  //DEPRECATE_DECLARE
  const c3d::path_string FullFilePath  () const override { return fileName; };

  /// \ru Является ли файл текстовым. \en Whether the file is a text file. 
  bool           IsFileAscii      () const override;

  /// \ru Получить версию формата при экспорте. \en Get the version of format for export.
  long int       GetFormatVersion () const override;

  /// \ru Следует ли экспортировать только поверхности ( введено для работы конвертера IGES ). \en Whether to export only surfaces (introduced for work with converter IGES ). 
  bool           IsOutOnlySurfaces() const override;

  /// \ru Является ли экспортируемый документ сборкой. \en Whether the document for export is an assembly. 
  DEPRECATE_DECLARE
  bool           IsAssembling     () const override { return true; };

  /// \ru Получить значение разрешения на импорт экспорт объектов определенного типа. \en Get the value of permission for import-export of objects of a certain type. 
  bool           GetIoPermission( MbeIOPermiss nPermission ) const override;

  /// \ru Получить значения разрешений на импорт экспорт объектов определенных типов. \en Get values of permission for import-export of objects of certain types. 
  void GetIoPermissions( std::vector<bool>& ioPermissions ) const override;

  /// \ru Установить разрешение на импорт экспорт объектов определенного типа. \en Set permission for import-export of objects of a certain type. 
  void           SetIoPermission( MbeIOPermiss nPermission, bool isSet ) override;

  /// \ru Получить строку заголовка файла. \en Get file header string.
  bool           GetPropertyString ( MbeConverterStrings nString, std::string &  propertyString ) const override;

  /// \ru Установить строку заголовка файла. \en Set file header string. 
  void           SetPropertyString ( MbeConverterStrings nString, const std::string &  propertyString ) override;

  /// \ru Представление текста в аннотационных объектах. \en Text representation in annotation objects. 
  DEPRECATE_DECLARE
  eTextForm      GetAnnotationTextRepresentation () const override;
  /**  \brief \ru Следует ли компоненты экспортировать в разные файлы (если позволяет формат).
              \en Export components into separate files ( if provided in format). \~
       \note  \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL \~.
  */
  DEPRECATE_DECLARE
  bool           ExportComponentsSeparately() const override;

  /// \ru Получить ЛСК, относительно которой позиционирована модель. \en Get the location, the model is placed in.
  MbPlacement3D  GetOriginLocation() const override;

  /// \ru Заменять ли принудительно СК компонент на правые. \en Replace components' placements to right-oriented.
  bool           ReplaceLocationsToRight() const override;

  /**  \brief \ru Сшивать ли поверхности автоматически.
              \en If surfaces should be stitched automatically. \~
    \return \ru true - Сшивать поверхности автоматически, false - Спросить пользователя, сшивать ли поверхности.
            \en true - Stitch surfaces automatically, false - Ask user first time. \~
    \param[out] stitchPrecision - \ru Точность сшивки.
                                  \en Stitch precision. \~
  */
  bool           EnableAutoStitch( double& /*stitchPrecision*/ ) const override;

  /// \ru Получить множитель единиц длины по отношению к миллиметру. \en Get the factor of the length units to millimeters.
  double         LengthUnitsFactor() const override;

  /**  \brief \ru Получить множитель единиц длины по отношению к миллиметру в модели приложения. 
  \en Get the factor of the length units to millimeters in the application model. \~  
  */
  double         AppLengthUnitsFactor() const override;

  /**  \brief \ru Сделать запись в журнал конвертирования.
              \en Make a record in the converter report. \~
    \param[in] id - \ru Идентификатор элемента внутри файла стороннего формата.
                    \en Identifier of an element inside the file of a foreign format. \~
    \param[in] msgType - \ru Тип сообщения.
                         \en Message type. \~
    \param[in] msgText - \ru Код сообщения.
                         \en Message code. \~
  */
  void           LogReport( ptrdiff_t id, eMsgType msgType, eMsgDetail msgText ) override;

//   /**  \brief \ru Следует ли показывать сообщения и диалоги пользователю. \en Whether to show messages and dialog to the user. \~
//    \details \ru Обеспечивает работу через API. \en Provide possibility for work via API. \~
//    \return \ru true - обычная работа, false - через API. \en true - ordinary work, false - via API. \~
//   */
   bool          CanShowMessages() const override;

  /// \ru Дать данные вычисления триангуляции (для конвертера STL и VRML). \en Get data for step calculation during triangulation (for STL, VRML only).
  MbStepData     TesselationParameters() const override;

  /// \ru Дать данные вычисления триангуляции уровня детализации (для конвертера JT). \en Get data for step calculation during triangulation of LOD0 (for JTonly).
  MbStepData    LOD0TesselationParameters() const override;

  /// \ru Получить флаг сохранения совпадающих точек швов. (для конвертера STL и VRML). \en Whether to keep coincident points of seams (for STL, VRML only).
  bool           DualSeams() const override;

  /// \ru Задать флаг сохранения совпадающих точек швов. (для конвертера STL и VRML). \en Whether to keep coincident points of seams (for STL, VRML only).
  void           DualSeams( bool );

  /// \ru Получить настройки для выдачи отладочной информации. \en Get the settings of debug info.
  C3DConverterDebugSettings GetDebugSettings() const override;

  /// \ru Выполнять ли слияние подобных граней. \en Whether to join similar faces.
  bool          JoinSimilarFaces() const override;

  /// \ru Добавлять ли удаленные грани в качестве оболочек. \en Whether to add removed faces as shells.
  bool          AddRemovedFacesAsShells() const override;

  /// \ru Получить генератор однострочного идентификтора изделия. \en Get generator of one-line product identifier.
  SPtr<IProductIdMaker> ProductIdentifierGenerator() const override;

  /// \ru Получить запросчик масштабного коэффициента единиц длины. \en Get scale requester.
  SPtr<IScaleRequestor> GetScaleRequester() const override;

  /// \ru Получить запросчик сшивки. \en Get stitch requester.
  SPtr<IStitchRequestor> GetStitchRequester() const override;

  /// \ru Получить пользовательский преобразователь строк. \en Get user string transformer. 
  SPtr<IC3DCharEncodingTransformer> GetUserCharEncodingTransformer() const override;

  /// \ru Определить способ формирования визуальных свойст при чтении из C3D. \en Specify the way how visual properties form while reading from C3D.
  ColorProcessing GetC3DVisualPropertiesResolutionStrategy() const override;

  /// \ru Импортировать полигональные объекты как объекты с топологической информацией. \en Import polygonal objects as objects with topological information.
  bool           ImportMeshesWithTopology() const override { return importMeshesWithTopology; }

  /// \ru Получить параметры для лечения неправильных сеток. \en Get parameters for repairing inconsistent meshes.
  bool           RepairInconsistentMesh( MbRepairInconsistentMeshParams &params ) const override;

  /// \ru Импортировать атрибуты вставок (STEP). \en Import instances attributes (STEP).
  bool           ImportInstanceAttributes() const override { return importInstanceAttributes; }

  /// \ru Обновить значения свойств, за исключением: GetPropertyString() и LogReport(). \en Update properties, exept : GetPropertyString() и LogReport()
  void Update( IConvertorProperty3D const & init );

  private:
    ConvConvertorProperty3D              ( ConvConvertorProperty3D const & ) = delete;
    ConvConvertorProperty3D & operator = ( ConvConvertorProperty3D const & ) = delete;
    ConvConvertorProperty3D              ( ConvConvertorProperty3D && )      = delete;

}; // IConvertorProperty3D



//------------------------------------------------------------------------------
/** \brief  \ru Преобразование строк с использованием установленной локали.              
            \en Transform strings using the set locale. 
   \details \ru Выполняется только для UNICODE-сборок, для остальных случаев производится копирование.
            \en It is performed only for UNICODE assemblies, for other cases copying is performed.\~
  \ingroup Base_Tools_String
*/
class CONV_CLASS C3DCharEncodingTransformerLocale : public IC3DCharEncodingTransformer
{
public:
  virtual ~C3DCharEncodingTransformerLocale() {}

  //------------------------------------------------------------------------------
  /** \brief \ru Преобразовать строку C3D в строку STD.
             \en Transform C3D string to the STD one. \~
    \ingroup Base_Tools_String
  */
  virtual bool C3DToStd( const c3d::string_t& from, std::string  & to );

  //------------------------------------------------------------------------------
  /** \brief \ru Преобразовать строку STD в строку C3D.
             \en Transform STD string to the C3D one. \~
    \ingroup Base_Tools_String
  */
  virtual bool StdToC3D( const std::string  & from, c3d::string_t& to );
};


//------------------------------------------------------------------------------
/** \brief  \ru Преобразование строк с использованием нотации STEP.
            \en Transform strings using the STEP wide characters encoding. 
  \ingroup Base_Tools_String
*/
class CONV_CLASS IC3DCharEncodingTransformerStep : public IC3DCharEncodingTransformer
{
public:
  ~IC3DCharEncodingTransformerStep() override = default;

  //------------------------------------------------------------------------------
  /** \brief \ru Преобразовать строку C3D в строку STD.
             \en Transform C3D string to the STD one. \~
    \ingroup Base_Tools_String
  */
  bool C3DToStd( const c3d::string_t& from, std::string  & to ) override;

  //------------------------------------------------------------------------------
  /** \brief \ru Преобразовать строку STD в строку C3D.
             \en Transform STD string to the C3D one. \~
    \ingroup Base_Tools_String
  */
  bool StdToC3D( const std::string  & from, c3d::string_t& to ) override;
};


//------------------------------------------------------------------------------
/** \brief  \ru Преобразование строк с использованием кодировки UTF-8.
            \en Transform strings using the utf-8 encoding.
  \note     \ru Реализовано только для C++ 20 или выше. 
            \en Implemented only for C++ 20 and above.

  \ingroup Base_Tools_String
*/
class CONV_CLASS IC3DCharEncodingTransformerUTF8 : public IC3DCharEncodingTransformer
{
public:
  ~IC3DCharEncodingTransformerUTF8() override = default;

  //------------------------------------------------------------------------------
  /** \brief \ru Преобразовать строку C3D в строку STD.
             \en Transform C3D string to the STD one. \~
    \ingroup Base_Tools_String
  */
  bool C3DToStd( const c3d::string_t& from, std::string  & to ) override;

  //------------------------------------------------------------------------------
  /** \brief \ru Преобразовать строку STD в строку C3D.
             \en Transform STD string to the C3D one. \~
    \ingroup Base_Tools_String
  */
  bool StdToC3D( const std::string  & from, c3d::string_t& to ) override;
};


//------------------------------------------------------------------------------
/** \brief  \ru Преобразование строк с использованием кодировки Windows-1251.
            \en Transform strings using the Windows-1251 encoding.
  \ingroup Base_Tools_String
*/
class CONV_CLASS IC3DCharEncodingTransformerWindows1251 : public IC3DCharEncodingTransformer
{
public:
  ~IC3DCharEncodingTransformerWindows1251() override = default;

  //------------------------------------------------------------------------------
  /** \brief \ru Преобразовать строку C3D в строку STD.
             \en Transform C3D string to the STD one. \~
    \ingroup Base_Tools_String
  */
  bool C3DToStd( const c3d::string_t& from, std::string  & to ) override;

  //------------------------------------------------------------------------------
  /** \brief \ru Преобразовать строку STD в строку C3D.
             \en Transform STD string to the C3D one. \~
    \ingroup Base_Tools_String
  */
  bool StdToC3D( const std::string  & from, c3d::string_t& to ) override;
};

#endif // __CONV_EXCHANGE_SETTINGS_H
