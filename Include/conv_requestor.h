////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Интерфейс запроса масштаба. Интерфейс запроса сшивки.
         \en Interface of scale request. Interface of stitching request. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CONV_REQUESTOR_H
#define __CONV_REQUESTOR_H


#include <reference_item.h>
#include <math_define.h>
#include <tool_cstring.h>


//------------------------------------------------------------------------------
/** 
\brief \ru Интерфейс выбора конфигурации. 
       \en Interface of configuration selection. \~
\details  \ru Вызывается при импорте однократно, если импортируемоя модель содержит более одной конфигурации.
          \en Called on import once if the model contains contains more than one configurations. \~
*/
// ---
class CONV_CLASS IConfigurationSelector : public MbRefItem
{
public:
  // \ru Конструктор по умолчанию. \en Default constructor.
  IConfigurationSelector() = default;

  // \ru Деструктор. \en Destructor.
  virtual ~IConfigurationSelector() = default;

  // \ru Добавить конфигурацию для выбора пользователем. \en Add configuration for selection by user.
  virtual void    AddConfiguration        ( const c3d::string_t& configurationName )  = 0;

  // \ru Добавить конфигурацию для выбора пользователем, не содержащую модель. \en Add configuration without model for selection by user.
  virtual void    AddEmptyConfiguration( const c3d::string_t & configurationName ) { AddConfiguration( configurationName ); };

  // \ru Указать индекс активной конфигурации. \en Specify the index of active configuration.
  virtual void    SetActiveConfiguration  ( const size_t index )                      = 0;

  // \ru Получить индекс конфигурации выбранной пользователем. \en Get index of configuration chosen by user.
  virtual size_t  GetConfiguration        () const                                    = 0;

  // \ru Индекс конфигурации для прекращения чтения по запросу пользователя. \en Index which should be returned if user canceled import.
  static const size_t configurationIdToCancel = SYS_MAX_T;
};


//------------------------------------------------------------------------------
/**
\brief \ru Интерфейс для сохранения списка имен атрибутов модели.
       \en Interface for collecting model attribute names. \~
\details  \ru Вызывается во время импорта предварительных данных модели
        для задания списка имен атрибутов модели.
          \en Called during import of preliminary model data to specify
        a list of model attribute names .  \~
 \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
       \en EXPEREIMENTAL. \~
*/
// ---
class CONV_CLASS IAttributeNamesCollector : public MbRefItem
{
public:
  virtual void AddAttributeName( const c3d::string_t& configurationName ) = 0;
};



//------------------------------------------------------------------------------
///  
/** 
\brief \ru Интерфейс запроса масштаба.
       \en Interface of scale request. \~
\details  \ru Рекомендуется использовать методы интерфейса IConvertorProperty3D.
          \en Using methods of the IConvertorProperty3D interface recommended. \~
 \note \ru Рекомендуется использовать методы интерфейса IConvertorProperty3D.
       \en Using methods of the IConvertorProperty3D interface recommended. \~
*/
// ---
class CONV_CLASS IScaleRequestor : public MbRefItem
{
public:
  virtual double ScaleRequest() = 0;
};


//------------------------------------------------------------------------------
/** 
\brief \ru Интерфейс запроса сшивки. 
      \en Interface of stitching request \~
\details  \ru Рекомендуется использовать методы интерфейса IConvertorProperty3D.
          \en Using methods of the IConvertorProperty3D interface recommended. \~
 \note \ru Рекомендуется использовать методы интерфейса IConvertorProperty3D.
       \en Using methods of the IConvertorProperty3D interface recommended. \~
*/
// ---
class CONV_CLASS IStitchRequestor : public MbRefItem
{
public:
  virtual bool StitchRequest() = 0;
};


//------------------------------------------------------------------------------
/**
\brief \ru Интерфейс запроса отладочного вывода.
      \en Debug output request interface \~
*/
// ---
// TODO: depracate
class IConverterEventLogger : public MbRefItem
{
public:
  virtual bool WriteToLog( const char * ) = 0;
};


//------------------------------------------------------------------------------
/** \brief \ru Интерфейс передачи метаданных.
       \en Metadata transfer. \~

    \details  \ru Метаданные используются в тех случаях, когда исходные данные от 
конвертеров плохо вписываются в геометрическую и атрибутивную модель. Пример метаданных - 
альтернативные представления дерева модели.
          \en Metadata are used in cases when geometric or attribute model doesn't
match structure of information from converters. Alternate tree sutrucure is a sample of
metadata.  \~
*/
// ---
class CONV_CLASS IConverterMetadataReceiver : public MbRefItem 
{
public:

/** \brief \ru Инициализировать передачу метаданных для элемента.
             \en Initialize metadata transfer for item. \~
  \details   \ru Вызывается перед началом передачи метаданных, относящихся к элементу негеометрических данных, с целью подготовки приемника.
             \en Should be called before metadata related to non-geometric element to prepare the reciever. \~
  \param[in] category - \ru Категория, к которой относится элемент.
                        \en Category element belongs to.\~
  \param[in] nameInCategory - \ru Название элемента внутри категории.
                              \en Element's name in category.\~  
  \param[in] contentType - \ru Представление передаваемых метаданных.
                           \en Representation of metadata.\~  

  \return    \ru Признак, возможна ли передача метаданных.
             \en Wether the metadata transfer is possible. \~
  \note \ru Ожидается, что вызывается однократно для каждой пары категория - название.
        \en Expected it is called once for each category - name pair. \~
  */
  virtual bool InitMetadataForItem(const c3d::string_t& category, const c3d::string_t& nameInCategory, const c3d::string_t& contentType) = 0;

  /** \brief \ru Завершить передачу метаданных для элемента.
             \en Finalize metadata transfer for item. \~
  \details   \ru Вызывается после передачи метаданных, относящихся к элементу негеометрических данных, с целью освободить ресурсы.
             \en Should be called after metadata related to non-geometric element to free related resources. \~
  \param[in] category - \ru Категория, к которой относится элемент.
                        \en Category element belongs to.\~
  \param[in] nameInCategory - \ru Название элемента внутри категории.
                              \en Element's name in category. 
  \note \ru Ожидается, что вызывается не более одного раза для каждой пары категория - название и только в случае, если передача была инициализирована успешно.
        \en Expected it is called once for each category - name pair in case transfer is possible or not called otherwise. \~
  */
  virtual void CloseMetadataForItem(const c3d::string_t& category, const c3d::string_t& nameInCategory) = 0;

  /** \brief \ru Передать блок метаданных для элемента.
             \en Transfer metadata block for item. \~
  \param[in] category - \ru Категория, к которой относится элемент.
                        \en Category element belongs to.\~
  \param[in] nameInCategory - \ru Название элемента внутри категории.
                              \en Element's name in category.\~  
  \param[in] metadataBuffer - \ru Адрес бинарного буфера, содержащего данные блока.
                              \en Binary buffer containing block data.\~  
  \param[in] bufferCapacity - \ru Длина бинарного буфера, содержащего данные блока.
                              \en Lenght of binary buffer containing block data.\~  
  \note \ru Возможен многократный вызов для каждой пары категория - название.
        \en Many calls for each category - name are possible. \~
  */
  virtual void TransferMetadataForItem(const c3d::string_t& category, const c3d::string_t& nameInCategory, const char* metadataBuffer, unsigned long int bufferCapacity) = 0;
};


#endif // __CONV_REQUESTOR_H
