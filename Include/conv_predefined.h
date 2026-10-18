////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief  \ru Перечисления, используемые при импорте и экспорте.
          \en Enumerations for import/export operations.\~
  \details  \ru Определены перечисления, определяющие результат конвертирования,
 разрешение на чтение и запись различных объектов и передаваемых черезх конвертер строк.
            \en Converting result, objects and properties filters, special strings 
 of enumerations are defined.\~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CONV_PREDEFINED_H
#define __CONV_PREDEFINED_H


#include <mb_enum.h>


//------------------------------------------------------------------------------
/** \brief  \ru Представление текста при экспорте.
\en Representation of exported text.\~   
\ingroup Data_Exchange
*/
// ---
enum eTextForm { 
  exf_TextOnly,     ///< \ru Только текст.      \en Text only.
  exf_GeometryOnly, ///< \ru Только геометрия.  \en Geometry only.
};

//------------------------------------------------------------------------------
/** \brief  \ru Предопределённые ключи атрибутов для передачи контрольных параметров.
\en Predefined key of attributes used for validation properties' exchange.\~   
\ingroup Data_Interface
*/
// ---
/// \ru Объём. \en Volume.
#define C3D_CAD_VALIDATION_PROPERTY_VOLUME    c3d::c3dStr_ValidationPropertyVolumeExchange
/// \ru Площать поверхости. \en Surface area.
#define C3D_CAD_VALIDATION_PROPERTY_AREA      c3d::c3dStr_ValidationPropertySurfaceAreaExchange
/// \ru Масса. \en Mass.
#define C3D_CAD_VALIDATION_PROPERTY_MASS      c3d::c3dStr_ValidationPropertyMassExchange
/// \ru Идентификатор элемента. \en Item Identifier.
#define C3D_CAD_ITEM_IDENTIFIER               c3d::c3dStr_ItemIdentifierExchange


//------------------------------------------------------------------------------
/** \brief \ru Типы линий, передаваемых через конвертер.
\en Types of lines passed via converter. \~
\ingroup Data_Interface
*/
// ---
enum MbeLineFontPattern {
  lfp_BEGIN = 0,            ///< \ru Для удобства перебора. \en For the convenient search. 
  lfp_STEPcontinuous,       ///< \ru Непрерывная в конвертерах STEP и IGES. \en Continuous line in STEP and IGES (Solid) converters. 
  lfp_STEPchain,            ///< \ru Штрих-пунктирная в конвертерах STEP и IGES. \en Chain line( dash-dotted) in STEP and IGES converters.
  lfp_STEPchainDoubleDash,  ///< \ru Штриховая с двумя пунктирами в конвертерах STEP и IGES. \en Dash-double-dot line in STEP and IGES (Phantom) converter. 
  lfp_STEPdashed,           ///< \ru Штриховая в конвертерах STEP и IGES. \en Dash line in STEP and IGES converters.
  lfp_STEPdotted,           ///< \ru Пунктирная в конвертерах STEP и IGES. \en Dotted line in STEP and IGES converters. 
  lfp_END                   ///< \ru Для удобства перебора. \en For search 
};


//------------------------------------------------------------------------------
/** \brief \ru Отображение точек, передаваемых через конвертер.
\en Representation of points passed via converter. \~
\ingroup Data_Interface
*/
// ---
enum MbeDotMarkerSymbol {
  dms_BEGIN = 0,          ///< \ru Для удобства перебора. \en For the convenient search. 
  dms_STEPdot,            ///< \ru Точка. \en A point. 
  dms_STEPx,              ///< \ru Косой крест. \en x - cross. 
  dms_STEPplus,           ///< \ru Прямой крест. \en Plus. 
  dms_STEPasterisk,       ///< \ru Звёздочка. \en Asterisk. 
  dms_STEPring,           ///< \ru Кольцо. \en Ring. 
  dms_STEPsquare,         ///< \ru Квадрат. \en Square. 
  dms_STEPtriangle,       ///< \ru Треугольник. \en Triangle. 
  dms_END                 ///< \ru Для удобства перебора. \en For the convenient search. 
};


//------------------------------------------------------------------------------
/** \brief  \ru Ключи строк, соответствующих названию специальных атрибутов.
            \en Keys of the strings, which mark special attributes.\~   
\ingroup Data_Interface
*/
// ---
enum ePromtAttributeKey {
  pac_GConverterInternalIsDummy, ///< \ru Является ли элемент пустышкой.\~   
  pac_GeneralIsAssembly,         ///< \ru Является ли элемент сборкой. \en Is item assembly.\~   
  pac_GeneralFileName,           ///< \ru Имя файла. \en File name.\~   
  pac_STEPHeader,                ///< \ru Заголовок STEP. \en STEP header.\~   
  pac_STEPProduct,               ///< \ru Изделие STEP. \en STEP product.\~   
  pac_STEPPersonOrganization,    ///< \ru Лицо и организация STEP. \en STEP person and organization.\~   
  pac_STEPAssignedRole           ///< \ru Назначенная роль STEP. \en The role, assigned to the person.\~   
};


//------------------------------------------------------------------------------
/** \brief \ru Идентификаторы сообщений индикатора прогресса выполнения конвертации данных.
           \en Identifiers of the execution progress indicator messages converters data exchange \~
\ingroup Data_Exchange
*/
//---
enum MbeProgBarId_Converters {
  pbarId_Cnv_Beg = pbarId_PointsSurface_End + 1,

  pbarId_Cnv_Parse_Data,        // \ru Синтаксический анализ... \en Syntactic analysis... 
  pbarId_Cnv_Create_Objects,    // \ru Создание объектов... \en Creation of objects... 
  pbarId_Cnv_Process_Surfaces,  // \ru Обработка поверхностей... \en Surfaces processing... 
  pbarId_Cnv_Process_Annotation,// \ru Обработка аннотации... \en Annotation processing... 
  pbarId_Cnv_Create_Model,      // \ru Создание модели... \en Creation of model... 
  pbarId_Cnv_Write_Model,       // \ru Запись модели... \en Writing of model... 

  pbarId_Cnv_End,
};


//------------------------------------------------------------------------------
/** \brief \ru Идентификаторы сообщений индикатора прогресса выполнения триангуляции при выполнении конвертации данных.
           \en Identifiers of the execution progress indicator messages triangulation. \~
\ingroup Data_Exchange
*/
//---
enum MbeProgBarId_Triangulation {
  pbarId_Triangulation_Beg = pbarId_Cnv_End + 1,

  pbarId_Calc_Triangulation,  // \ru Расчет триангуляции \en Calculating of triangulation 

  pbarId_Triangulation_End,
};


//------------------------------------------------------------------------------
/** \brief \ru Идентификаторы сообщений индикатора прогресса выполнения расчёта 
               масс-инерционные характеристики детали или сборки при выполнении конвертации данных.
           \en Identifiers of the execution progress indicator messages of mass-inertial properties of assembly or a detail. \~
\ingroup Data_Exchange
*/
//---
enum MbeProgBarId_MassInertiaProperties {
  pbarId_MassInertiaProperties_Beg = pbarId_Triangulation_End + 1,

  pbarId_Calc_MassInertiaProperties,  // \ru Расчет масс-инерционных характеристик \en Mass-inertial properties calculation 

  pbarId_MassInertiaProperties_End,
};


#endif // __CONV_PREDEFINED_H