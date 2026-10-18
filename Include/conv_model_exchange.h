////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Общий интерфейс конвертера.
         \en Common API of the converter. \~
  \details  \ru Функции чтения и записи в буфер и файл с автоопределением формата по
  расширению файла, класс-конвертер с методами для каждого формата и возможностью
  подключения плагина, функции для работы с каждым форматом.
         \en Functions for export and import from buffer and file using file's extension
  for format detection, class of converter for format-specific methods and API for
  plugin, format-specific import and export functions. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CONV_MODEL_EXCHANGE_H
#define __CONV_MODEL_EXCHANGE_H

#include <map>
#include <tool_cstring.h>
#include <model_item.h>
#include <model_entity.h>
#include <solid.h>
#include "conv_res_type.h"
#include "conv_format_info.h"
#include <conv_plugin_version_info.h>

class  IProgressIndicator;
class  IScaleRequestor;
class  ItModelDocument;
class  IConvertorProperty3D;
class  IConfigurationSelector;
class  IAttributeNamesCollector;
class  IConverterEventLogger;
class  IConverterMetadataReceiver;

/**
  \addtogroup Exchange_Interface
  \{
*/

//------------------------------------------------------------------------------
/** \brief  \ru Обменный формат модели.
\en Model exchange format.\~
\ingroup Data_Interface
*/
// ---
enum MbeModelExchangeFormat {
  mxf_autodetect, ///< \ru Интерпретировать содержимое по расширению файла.                                                    \en File extension defines format.
  mxf_ACIS,       ///< \ru Интерпретировать содержимое как ACIS (.sat).                                                        \en Read data from buffer as ACIS (.sat).
  mxf_IGES,       ///< \ru Интерпретировать содержимое как IGES (.igs или .iges).                                              \en Read data from buffer as IGES (.igs or .iges).
  mxf_JT,         ///< \ru Интерпретировать содержимое как JT (.jt).                                                           \en Read data from buffer as JT (.jt).
  mxf_Parasolid,  ///< \ru Интерпретировать содержимое как Parasolid (.x_t, .xmt_txt или .xmp_txt).                            \en Read data from buffer as Parasolid (.x_t, .xmt_txt or .xmp_txt).
  mxf_STEP,       ///< \ru Интерпретировать содержимое как STEP (.stp или .step).                                              \en Read data from buffer as STEP (.stp or .step).
  mxf_STL,        ///< \ru Интерпретировать содержимое как STL (.stl).                                                         \en Read data from buffer as STL (.stl).
  mxf_VRML,       ///< \ru Интерпретировать содержимое как VRML (.wrl).                                                        \en Read data from buffer as VRML (.wrl).
  mxf_3MF,        ///< \ru Интерпретировать содержимое как 3MF (.3mf).                                                         \en Read data from buffer as 3MF (.3mf).
  mxf_OBJ,        ///< \ru Интерпретировать содержимое как OBJ (.obj).                                                         \en Read data from buffer as OBJ (.obj).
  mxf_GRDECL,     ///< \ru Интерпретировать содержимое как GRDECL (.grdecl).                                                   \en Read data from buffer as GRDECL (.grdecl).
  mxf_ASCIIPoint, ///< \ru Интерпретировать содержимое как облако точек в ASCII (.txt, .asc или .xyz).                         \en Read data from buffer as ASCII point cloud (.txt, .asc or .xyz).
  mxf_C3D,        ///< \ru Интерпретировать содержимое как C3D (.c3d).                                                         \en Read data from buffer as C3D (.c3d).
  mxf_ACISBinary, ///< \ru Интерпретировать содержимое как бинарный ACIS (.sab).                                               \en Read data from buffer as binary ACIS (.sab).
  mxf_ParasolidBinary ///< \ru Интерпретировать содержимое как бинарный Parasolid (.x_b, .xmt_bin или .xmp_bin).               \en Read data from buffer as binary Parasolid (.x_b, .xmt_bin or .xmp_bin).
};


namespace c3d {

  class CONV_CLASS C3DExchangeBuffer;

  typedef std::map<c3d::string_t, c3d::string_t> optionNameValuePairs_t; ///< \ru Набор имеованных значений опций. \en The container of named values of options.
                                          

  /** \brief \ru Прочитать файл обменного формата в модель.
             \en Read a file of an exchange format into model. \~
  \details \ru Если свойства конвертера заданы, аргумент fileName игнорируется, а имя файла берётся из свойств конвертера.
  В противном случае импорт идёт с умолчательными параметрами, соответствующими реализации ConvConvertorProperty3D. \~
  \en The fileName argument is not used if converter properties are defined obviously, file path comes from the FullFilePath
  method. Otherwise default parameters corresponding ConvConvertorProperty3D implementation are used for import.
  \param[out] model -    \ru Импортируемая модель.
                         \en The imported model. \~
  \param[in] filePath -  \ru Путь файла.
                         \en File path. \~
  \param[in] prop -      \ru Реализация интерфейса свойств конвертера.
                         \en Implementation of converter's properties interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup Exchange_Interface
  */
  CONV_FUNC( MbeConvResType ) ImportFromFile( MbModel& model,
                                              const path_string& fileName,
                                              IConvertorProperty3D* prop = nullptr,
                                              IProgressIndicator* indicator = nullptr );


  /** \brief \ru Прочитать файл обменного формата в элемент.
  \en Read a file of an exchange format into element. \~
  \details \ru Если свойства конвертера заданы, аргумент fileName игнорируется, а имя файла берётся из свойств конвертера.
  В противном случае импорт идёт с умолчательными параметрами, соответствующими реализации ConvConvertorProperty3D. \~
  \en The fileName argument is not used if converter properties are defined obviously, file path comes from the FullFilePath
  method. Otherwise default parameters corresponding ConvConvertorProperty3D implementation are used for import.
  \param[out] item -    \ru Корневой элемент импортированной модели.
                        \en The imported element. \~
  \param[in] filePath - \ru Путь файла.
                        \en File path. \~
  \param[in] prop -     \ru Реализация интерфейса свойств конвертера.
                        \en Implementation of converter's properties interface. \~
  \param[in] indicator -\ru Индикатор хода процесса.
                        \en The process progress indicator. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup Exchange_Interface
  */
  CONV_FUNC( MbeConvResType ) ImportFromFile( c3d::ItemSPtr& item,
                                              const path_string& filePath,
                                              IConvertorProperty3D* prop = nullptr,
                                              IProgressIndicator* indicator = nullptr );

  /** \brief \ru Прочитать файл обменного формата в модель.
  \en Read a file of an exchange format into model. \~
  \details \ru Если свойства конвертера заданы, аргумент fileName игнорируется, а имя файла берётся из свойств конвертера.
  В противном случае импорт идёт с умолчательными параметрами, соответствующими реализации ConvConvertorProperty3D. \~
  \en The fileName argument is not used if converter properties are defined obviously, file path comes from the FullFilePath
  method. Otherwise default parameters corresponding ConvConvertorProperty3D implementation are used for import.
  \param[out] mDoc -     \ru Импортированный модельный документ.
                         \en The imported model document. \~
  \param[in] filePath -  \ru Путь файла.
                         \en File path. \~
  \param[in] prop -      \ru Реализация интерфейса свойств конвертера.
                         \en Implementation of converter's properties interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup Exchange_Interface
  */
  CONV_FUNC( MbeConvResType ) ImportFromFile( ItModelDocument& mDoc,
                                              const path_string& filePath,
                                              IConvertorProperty3D* prop = nullptr,
                                              IProgressIndicator* indicator = nullptr );

  /** \brief \ru Записать модель в файл обменного формата.
             \en Write the model into an exchange format file. \~
  \details \ru Если свойства конвертера заданы, аргумент fileName игнорируется, а имя файла берётся из свойств конвертера.
  В противном случае экспорт идёт с умолчательными параметрами, соответствующими реализации ConvConvertorProperty3D. \~
           \en The fileName argument is not used if converter properties are defined obviously, file path comes from the FullFilePath
  method. Otherwise default parameters corresponding ConvConvertorProperty3D implementation are used for export.
  \param[out] model -    \ru Экспортируемая модель.
                         \en The exported model. \~
  \param[in] filePath -  \ru Путь файла.
                         \en File path. \~
  \param[in] prop -      \ru Реализация интерфейса свойств конвертера.
                         \en Implementation of converter's properties interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup Exchange_Interface
  */
  CONV_FUNC( MbeConvResType ) ExportIntoFile( MbModel& model,
                                              const path_string& filePath,
                                              IConvertorProperty3D* prop = nullptr,
                                              IProgressIndicator* indicator = nullptr );

  /** \brief \ru Записать модель в файл обменного формата.
  \en Write the model into an exchange format file. \~
  \details \ru Если свойства конвертера заданы, аргумент fileName игнорируется, а имя файла берётся из свойств конвертера.
  В противном случае экспорт идёт с умолчательными параметрами, соответствующими реализации ConvConvertorProperty3D. \~
  \en The fileName argument is not used if converter properties are defined obviously, file path comes from the FullFilePath
  method. Otherwise default parameters corresponding ConvConvertorProperty3D implementation are used for export.
  \param[out] item -    \ru Экспортируемый элемент.
                        \en The exported element. \~
  \param[in] filePath - \ru Путь файла.
                        \en File path. \~
  \param[in] prop -     \ru Реализация интерфейса свойств конвертера.
                        \en Implementation of converter's properties interface. \~
  \param[in] indicator -\ru Индикатор хода процесса.
                        \en The process progress indicator. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup Exchange_Interface
  */
  CONV_FUNC( MbeConvResType ) ExportIntoFile( MbItem& item,
                                              const path_string& filePath,
                                              IConvertorProperty3D* prop = nullptr,
                                              IProgressIndicator* indicator = nullptr );

  /** \brief \ru Записать модельный документ в файл обменного формата.
  \en Write the model into an exchange format file. \~
  \details \ru Если свойства конвертера заданы, аргумент fileName игнорируется, а имя файла берётся из свойств конвертера.
  В противном случае экспорт идёт с умолчательными параметрами, соответствующими реализации ConvConvertorProperty3D. \~
  \en The fileName argument is not used if converter properties are defined obviously, file path comes from the FullFilePath
  method. Otherwise default parameters corresponding ConvConvertorProperty3D implementation are used for export.
  \param[in] mDoc -      \ru Экспортируемый модельный документ.
                         \en The exported model document. \~
  \param[in] filePath -  \ru Путь файла.
                         \en File path. \~
  \param[in] prop -      \ru Реализация интерфейса свойств конвертера.
                         \en Implementation of converter's properties interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup Exchange_Interface
  */
  CONV_FUNC( MbeConvResType ) ExportIntoFile( ItModelDocument& mDoc,
                                              const path_string& filePath,
                                              IConvertorProperty3D* prop = nullptr,
                                              IProgressIndicator* indicator = nullptr );


  /** \brief \ru Импортировать данные из буфера в модель.
              \en Import data from buffer into model. \~
  \param[out] model -      \ru Импортируемая модель.
                           \en The imported model. \~
  \param[in] buffer -      \ru Буфер.
                           \en Buffer. \~
  \param[in] modelFormat - \ru Формат модели.
                           \en Model format. \~
  \param[in] prop -        \ru Реализация интерфейса свойств конвертера.
                           \en Implementation of converter's properties interface. \~
  \param[in] indicator -   \ru Индикатор хода процесса.
                           \en The process progress indicator. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup Exchange_Interface
  */
  CONV_FUNC( MbeConvResType ) ImportFromBuffer( MbModel& model,
                                                const  C3DExchangeBuffer& buffer,
                                                MbeModelExchangeFormat     modelFormat,
                                                IConvertorProperty3D* prop = nullptr,
                                                IProgressIndicator* indicator = nullptr );


  /** \brief \ru Импортировать данные из буфера в модель.
  \en Import data from buffer into model. \~
  \param[out] item -      \ru Импортируемый элемент.
                          \en The imported item. \~
  \param[in] buffer -     \ru Буфер.
                          \en Buffer. \~
  \param[in] modelFormat - \ru Формат модели.
                           \en Model format. \~
  \param[in] prop -        \ru Реализация интерфейса свойств конвертера.
                           \en Implementation of converter's properties interface. \~
  \param[in] indicator -   \ru Индикатор хода процесса.
                           \en The process progress indicator. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup Exchange_Interface
  */
  CONV_FUNC( MbeConvResType ) ImportFromBuffer( c3d::ItemSPtr& item,
                                                const C3DExchangeBuffer& buffer,
                                                MbeModelExchangeFormat    modelFormat,
                                                IConvertorProperty3D* prop = nullptr,
                                                IProgressIndicator* indicator = nullptr );


  /** \brief \ru Импортировать данные из буфера в модель.
  \en Import data from buffer into model. \~
  \param[out] mDoc -      \ru Импортируемый модельный документ.
                          \en The imported model document. \~
  \param[in] buffer -     \ru Буфер.
                          \en Buffer. \~
  \param[in] modelFormat - \ru Формат модели.
                           \en Model format. \~
  \param[in] prop -        \ru Реализация интерфейса свойств конвертера.
                           \en Implementation of converter's properties interface. \~
  \param[in] indicator -   \ru Индикатор хода процесса.
                           \en The process progress indicator. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup Exchange_Interface
  */
  CONV_FUNC( MbeConvResType ) ImportFromBuffer( ItModelDocument& mDoc,
                                                const C3DExchangeBuffer& buffer,
                                                MbeModelExchangeFormat    modelFormat,
                                                IConvertorProperty3D* prop = nullptr,
                                                IProgressIndicator* indicator = nullptr );


  /** \brief \ru Экспортировать модель в буфер.
             \en Export model into buffer. \~
  \param[in] model -       \ru Экспортируемая модель.
                           \en The exported model. \~
  \param[in] modelFormat - \ru Формат модели.
                           \en Model format. \~
  \param[out] buffer -     \ru Буфер.
                           \en Buffer. \~
  \param[in] prop -        \ru Реализация интерфейса свойств конвертера.
                           \en Implementation of converter's properties interface. \~
  \param[in] indicator -   \ru Индикатор хода процесса.
                           \en The process progress indicator. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup Exchange_Interface
  */
  CONV_FUNC( MbeConvResType ) ExportIntoBuffer( MbModel& model,
                                                MbeModelExchangeFormat modelFormat,
                                                C3DExchangeBuffer& buffer,
                                                IConvertorProperty3D* prop = nullptr,
                                                IProgressIndicator* indicator = nullptr );


  /** \brief \ru Экспортировать модель в буфер.
  \en Export model into buffer. \~
  \param[in] item -       \ru Экспортируемый элемент.
                          \en The exported item. \~
  \param[in] modelFormat - \ru Формат модели.
                           \en Model format. \~
  \param[out] buffer -     \ru Буфер.
                           \en Buffer. \~
  \param[in] prop -        \ru Реализация интерфейса свойств конвертера.
                           \en Implementation of converter's properties interface. \~
  \param[in] indicator -   \ru Индикатор хода процесса.
                           \en The process progress indicator. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup Exchange_Interface
  */
  CONV_FUNC( MbeConvResType ) ExportIntoBuffer( MbItem& item,
                                                MbeModelExchangeFormat  modelFormat,
                                                C3DExchangeBuffer& buffer,
                                                IConvertorProperty3D* prop = nullptr,
                                                IProgressIndicator* indicator = nullptr );

    /** \brief \ru Экспортировать модельный документ в буфер.
  \en Export model document into buffer. \~
  \param[in] mDoc -        \ru Экспортируемый модельный документ.
                           \en The exported model document. \~
  \param[in] modelFormat - \ru Формат модели.
                           \en Model format. \~
  \param[out] buffer -     \ru Буфер.
                           \en Buffer. \~
  \param[in] prop -        \ru Реализация интерфейса свойств конвертера.
                           \en Implementation of converter's properties interface. \~
  \param[in] indicator -   \ru Индикатор хода процесса.
                           \en The process progress indicator. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup Exchange_Interface
  */
  CONV_FUNC( MbeConvResType ) ExportIntoBuffer( ItModelDocument& item,
                                                MbeModelExchangeFormat modelFormat,
                                                C3DExchangeBuffer& buffer,
                                                IConvertorProperty3D* prop = nullptr,
                                                IProgressIndicator* indicator = nullptr );

/** \brief \ru Получить список поддерживаемых форматов.
  \en Get list of supported formats. \~
  \return \ru Список поддерживаемых форматов, с учётом лицензионного ключа и наличия формат-специфичных библиотек конвертера.
          \en A list of supported formats accounting the license key and presence of format-specific converter libraries. \~
  \details \ru Если получение списка форматов не поддерживается каким-либо плагином, то его форматы не будут включены в данный список.
           \en If some plugin does not support format listing, then its formats will not be included into the list. \~
  \ingroup Exchange_Interface
  */
CONV_FUNC( std::vector<FormatInfo> ) GetSupportedFormats();

/** \brief \ru Получить версии загруженных плагинов.
  \en Get versions of loaded plugins. \~
  \return \ru Версии загруженных плагинов.
          \en Versions of loaded plugins. \~
  \details \ru Значение версии зависит от реализации плагина. Если плагин не сообщает свою версию, то вместо неё проставляется "n/a".
         \en Version value is defined by plugin implementation. If plugin does not submit its version, "n/a" is used instead. \~
  \ingroup Exchange_Interface
  */
CONV_FUNC( std::vector<PluginVersionInfo> ) GetLoadedPluginsVersions();

  //------------------------------------------------------------------------------
  /** \brief \ru Буфер для обмена.
             \en Memory buffer for data exchange. \~
    \details \ru Обеспечивает обмен данными через оперативную память с контролем выделения и освобождения.
             \en Prvides data exchange with memory allocation and deallocation control. \~
  \ingroup Exchange_Interface
  */
  class CONV_CLASS C3DExchangeBuffer {
    char* data;   ///< \ru Адрес буфера. \en Buffer address.
    size_t count; ///< \ru Число байт. \en Bytes count.
  public:

    // \ru Конструктор. \en Constructor.
    C3DExchangeBuffer()
      : data( nullptr )
      , count( 0 ) {
    }

    // \ru Деструктор. \en Destructor.
    ~C3DExchangeBuffer() {
      Clear();
    }

    // \ru Очистить. \en Clear.
    inline void Clear() {
      if( data != nullptr )
        delete[] data;
      data = nullptr;
      count = 0;
    }

    // \ru Инициализировать буфер. \en Initialize buffer.
    inline void Init( const char* init, size_t size ) {
      Clear();
      data = new char[size];
      count = size;
      ::memcpy( data, init, count );
    }

    // \ru Инициализировать буфер. \en Initialize buffer.
    inline void Swap( char*& init, size_t& size ) {
      std::swap( init, data );
      std::swap( size, count );
    }

    // \ru Получить адрес буфера. \en Get buffer address.
    inline const char* Data() const {
      return data;
    }

    // \ru Получить число байт. \en Get count of bytes.
    inline size_t Count() const {
      return count;
    }
  };
};


//------------------------------------------------------------------------------
/** \brief \ru Интерфейс конвертера.
           \en Converter's interface. \~
  \details \ru Интерфейс конвертера реализует методы экспорта модели в файлы обменных форматов
    и импорта из них.
           \en Converter's interface implements methods of export of the model to files of exchange formats
    and import from them. \~
*/
class IConverter3D {
public:
  virtual ~IConverter3D() = default;

public:
  /** \brief \ru Установить обработчик для выбора конфигураций.
             \en Set handler for selecting configurations. \~
  \details \ru Если обработчик установлен и в импортируемом файле 
               есть более одной конфигурации (исполнения), то в 
               процессе чтения будет вызван установленный обработчик 
               для выбора необходимой конфигурации.
           \en If handler is set and imported file contains more than 
               one configuration (embodiment), then the handler will 
               be called for selection of needed configuration during 
               reading. \~
  \param[in] configuration_selector - \ru Указатель на устанавливаемый обработчик.
                                      \en Pointer to handler to be set. \~
  */
  virtual void SetConfgiurationSelector( SPtr<IConfigurationSelector> configuration_selector ) = 0;

  /** \brief \ru Установить обработчик для логирования.
             \en Set logging handler. \~
  \param[in] importEventLogger - \ru Указатель на устанавливаемый обработчик.
                                 \en Pointer to handler to be set. \~
  */
  virtual void SetDeveloperEventLogger( SPtr<IConverterEventLogger> importEventLogger ) = 0;

  /** \brief \ru Установить передачу метаданных.
             \en Set metadata transfer. \~
  \param[in] importEventLogger - \ru Указатель на устанавливаемый обработчик.
                                 \en Pointer to handler to be set. \~
  */
  virtual void SetMetadataTransferCallback( SPtr<IConverterMetadataReceiver> metadataTransferCallback ) = 0;

  /** \brief \ru Прочитать файл формата SAT.
             \en Read a file of SAT format. \~
  \details \ru Прочитать файл формата SAT или указанный поток.
    Если задан поток, то запись производится в присланный поток.
    Если поток не задан (нулевой), то открывается поток для файла, заданного в свойствах конвертера. \n
           \en Read a file of SAT format or a specified stream.
    If a stream is specified, then the record is performed to the given stream.
    If a stream is not specified (null), then a stream is being opened for file specified in the properties of the converter. \n \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] stream - \ru Поток, из которого производится чтение (может быть nullptr).
                      \en Stream from which reading is performed (can be nullptr). \~
  \param[in] indicator - \ru Индикатор хода процесса (может быть nullptr).
                         \en The process progress indicator (can be nullptr). \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей.
                           \en Dialog of request for stitching the surfaces. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup ACIS_Exchange
  */
  virtual MbeConvResType SATRead( IConvertorProperty3D& prop, ItModelDocument& idoc, std::iostream* stream, IProgressIndicator* indicator, MbRefItem* qeuryStitch ) = 0;

  /** \brief \ru Записать файл формата SAT.
             \en Write file of SAT format. \~
  \details \ru Записать файл формата SAT или указанный поток.
    Если задан поток, то запись производится в присланный поток.
    Если поток не задан (нулевой), то открывается поток для файла, заданного в свойствах конвертера. \n
           \en Write file of SAT format or the specified stream.
    If a stream is specified, then the record is performed to the given stream.
    If a stream is not specified (null), then a stream is being opened for file specified in the properties of the converter. \n \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] stream - \ru Поток, в который производится запись (может быть nullptr).
                      \en Stream in which the record is performed (can be nullptr). \~
  \param[in] indicator - \ru Индикатор хода процесса (может быть nullptr).
                         \en The process progress indicator (can be nullptr). \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup ACIS_Exchange
  */
  virtual MbeConvResType SATWrite( IConvertorProperty3D& prop, ItModelDocument& idoc, std::iostream* stream, IProgressIndicator* indicator ) = 0;

  /** \brief \ru Прочитать файл формата SAT.
             \en Read a file of SAT format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей.
                           \en Dialog of request for stitching the surfaces. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup ACIS_Exchange
  */
  virtual MbeConvResType SATRead( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;

  /** \brief \ru Записать файл формата SAT.
             \en Write file of SAT format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей (не используется).
                           \en Dialog of request for stitching the surfaces (not used). \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup ACIS_Exchange
  */
  virtual MbeConvResType SATWrite( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;

  /** \brief \ru Прочитать файл формата IGES.
             \en Read a file of IGES format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей.
                           \en Dialog of request for stitching the surfaces. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup IGES_Exchange
  */
  virtual MbeConvResType IGSRead( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;

  /** \brief \ru Записать файл формата IGES.
             \en Write a file of IGES format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей (не используется).
                           \en Dialog of request for stitching the surfaces (not used). \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup IGES_Exchange
  */
  virtual MbeConvResType IGSWrite( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;

  /** \brief \ru Прочитать файл формата JT.
             \en Read a file of JT format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей.
                           \en Dialog of request for stitching the surfaces. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup IGES_Exchange
  */
  virtual MbeConvResType JTRead( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;

  /** \brief \ru Записать файл формата JT.
             \en Write a file of JT format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей (не используется).
                           \en Dialog of request for stitching the surfaces (not used). \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup IGES_Exchange
  */
  virtual MbeConvResType JTWrite( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;

  /** \brief \ru Прочитать файл формата Parasolid.
             \en Read a file of Parasolid format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей.
                           \en Dialog of request for stitching the surfaces. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup Parasolid_Exchange
  */
  virtual MbeConvResType XTRead( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;

  /** \brief \ru Записать файл формата Parasolid.
             \en Write a file of Parasolid format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей.
                           \en Dialog of request for stitching the surfaces. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup Parasolid_Exchange
  */
  virtual MbeConvResType XTWrite( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;

  /** \brief \ru Прочитать файл формата STEP.
             \en Read a file of STEP format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей (не используется).
                           \en Dialog of request for stitching the surfaces (not used). \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup STEP_Exchange
  */
  virtual MbeConvResType STEPRead( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;

  /** \brief \ru Записать файл формата STEP.
             \en Write a file of STEP format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей (не используется).
                           \en Dialog of request for stitching the surfaces (not used). \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup STEP_Exchange
  */
  virtual MbeConvResType STEPWrite( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;

  /** \brief \ru Прочитать файл формата STL.
             \en Read a file of STL format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей (не используется).
                           \en Dialog of request for stitching the surfaces (not used). \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup STL_Exchange
  */
  virtual MbeConvResType STLRead( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;

  /** \brief \ru Записать файл формата STL.
             \en Write a file of STL format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей (не используется).
                           \en Dialog of request for stitching the surfaces (not used). \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup STL_Exchange
  */
  virtual MbeConvResType STLWrite( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;

 /** \brief \ru Прочитать файл формата OBJ.
            \en Read a file of OBJ format. \~
 \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                   \en Implementation of converter's properties interface. \~
 \param[in] idoc - \ru Реализация интерфейса документа.
                   \en Implementation of document interface. \~
 \param[in] indicator - \ru Индикатор хода процесса.
                        \en The process progress indicator. \~
 \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей (не используется).
                          \en Dialog of request for stitching the surfaces (not used). \~
 \return \ru Код завершения операции.
         \en Code of the operation termination. \~
 \ingroup VRML_Exchange
 */
  virtual MbeConvResType OBJRead( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0 ) = 0;

  /** \brief \ru Прочитать файл формата VRML.
             \en Read a file of VRML format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей (не используется).
                           \en Dialog of request for stitching the surfaces (not used). \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup VRML_Exchange
  */
  virtual MbeConvResType VRMLRead( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;

  /** \brief \ru Записать файл формата VRML.
             \en Write a file of VRML format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей (не используется).
                           \en Dialog of request for stitching the surfaces (not used). \~
  \param[in] devSag - \ru Угловой шаг для расчёта триангуляционной сетки.
                      \en Deviate sag requiref for grid calculateion. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup VRML_Exchange
  */
  virtual MbeConvResType VRMLWrite( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;


  /** \brief \ru Записать файл формата 3MF.
             \en Write a file of 3MF format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей (не используется).
                           \en Dialog of request for stitching the surfaces (not used). \~
  \param[in] devSag - \ru Угловой шаг для расчёта триангуляционной сетки.
                      \en Deviate sag requiref for grid calculateion. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup VRML_Exchange
  */
  virtual MbeConvResType ThreeMFWrite( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;



  /** \brief \ru Прочитать файл формата GRDECL.
             \en Read a file of GRDECL format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей (не используется).
                           \en Dialog of request for stitching the surfaces (not used). \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup VRML_Exchange
  */
  virtual MbeConvResType GRDECLRead( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;

  /** \brief \ru Записать файл формата GRDECL.
             \en Write a file of GRDECL format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей (не используется).
                           \en Dialog of request for stitching the surfaces (not used). \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup STL_Exchange
  */
  virtual MbeConvResType GRDECLWrite( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;

  /** \brief \ru Прочитать файл с облаком точек в формате ASCII.
              \en Read a file of ASCII Point Cloud format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей (не используется).
                           \en Dialog of request for stitching the surfaces (not used). \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup ASCII_Exchange
  */
  virtual MbeConvResType ASCIIPointCloudRead( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;

  /** \brief \ru Записать файл с облаком точек в формате ASCII..
             \en Write a point cloud file of ASCII format. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \param[in] qeuryStitch - \ru Диалог запроса на сшивку поверхностей (не используется).
                           \en Dialog of request for stitching the surfaces (not used). \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup ASCII_Exchange
  */
  virtual MbeConvResType ASCIIPointCloudWrite( IConvertorProperty3D& prop, ItModelDocument& idoc, IProgressIndicator* indicator = 0, MbRefItem* qeuryStitch = 0 ) = 0;

  /** \brief \ru Загрузить плагин получения данных для построения модели.
             \en Load plugin for getting information necessary to build model. \~
      \details  \ru Описание специфичных для плагина настроек следует получить у поставщика комопонента.
                \en The description of plugin-specific settings shoud be taken from the plugin's vendor. \~
      \note \ru Экспериментальное API. \en Expereimental API. \~
  \param[in] pluginName - \ru Имя подключаемого файла.
                    \en Name of the file to link. \~
  \param[in] pluginSpecificSettings - \ru Специфические для плагина настройки.
                                      \en Plugin-specific settings. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \note  \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
         \en EXPEREIMENTAL \~.
  \ingroup ASCII_Exchange
  */
  virtual MbeConvResType LoadForeignReader( const c3d::path_string& pluginName, const c3d::optionNameValuePairs_t& pluginSpecificSettings, IConfigurationSelector * configSelector ) = 0;


  /** \brief \ru Загрузить плагин получения данных для построения модели.
             \en Load plugin for getting information necessary to build model. \~
      \details  \ru Описание специфичных для плагина настроек следует получить у поставщика комопонента.
                \en The description of plugin-specific settings shoud be taken from the plugin's vendor. \~
      \note \ru Экспериментальное API. \en Expereimental API. \~
  \param[in] pluginName - \ru Имя подключаемого файла.
                    \en Name of the file to link. \~
  \param[in] pluginSpecificSettings - \ru Специфические для плагина настройки.
                                      \en Plugin-specific settings. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup ASCII_Exchange
  */
  virtual MbeConvResType LoadForeignReader( const c3d::path_string& pluginName, const c3d::optionNameValuePairs_t& pluginSpecificSettings ) = 0;


  /** \brief \ru Отключить загруженный плагин получения данных для построения модели.
             \en Release the loaded plugin for getting information necessary to build model. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup ASCII_Exchange
  */
  virtual MbeConvResType ReleaseForeignReader() = 0;

  /** \brief \ru Прочитать файл с использованием плагина.
              \en Read a file using plugin. \~
  \param[in] path - \ru ПУть к файлу, который нужно прочитать.
                    \en Path of the file to read. \~
  \param[in] idoc - \ru Реализация интерфейса документа.
                    \en Implementation of document interface. \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \note  \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
         \en EXPEREIMENTAL \~.
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup ASCII_Exchange
  */
  virtual MbeConvResType ImportForeign( const c3d::path_string& path, ItModelDocument& idoc, IConvertorProperty3D* prop = 0, IProgressIndicator* indicator = 0 ) = 0;

  /** \brief \ru Импортировать предварительные данные модели: список атрибутов и конфигураций.
             \en Import preliminary model data: list of attributes and configurations . \~
  \param[in] prop - \ru Реализация интерфейса свойств конвертера.
                    \en Implementation of converter's properties interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup Data_Exchange
  */
  virtual MbeConvResType PreliminaryImport( IConvertorProperty3D& prop, IProgressIndicator* indicator = 0) = 0;


  /** \brief \ru Задать параметры для сохранения данных на этапе предварительного импорта.
             \en Set parameters to save data at the stage of preliminary import. \~
  \param[in] attrCollector -  \ru Реализация интерфейса для сохранения имен атрибутов модели.
                              \en Implementation of the interface for collecting model attribute names.  . \~
  \param[in] configSelector - \ru Реализация интерфейса выбора конфигурации файла.
                              \en Implementation of the file configuration selection interface . \~
  \note  \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
         \en EXPEREIMENTAL \~.
  \ingroup Data_Exchange
  */
  virtual void SetPreliminaryCallbacks( IAttributeNamesCollector* attrCollector, IConfigurationSelector* configSelector) = 0;


  /** \brief \ru Прочитать файл обменного формата в модель.
  \en Read a file of an exchange format into model. \~
  \details \ru Если свойства конвертера заданы, аргумент fileName игнорируется, а имя файла берётся из свойств конвертера.
  В противном случае импорт идёт с умолчательными параметрами, соответствующими реализации ConvConvertorProperty3D. \~
  \en The fileName argument is not used if converter properties are defined obviously, file path comes from the FullFilePath
  method. Otherwise default parameters corresponding ConvConvertorProperty3D implementation are used for import.
  \param[out] mDoc -     \ru Модельный документ.
                         \en The model. \~
  \param[in] filePath -  \ru Путь файла.
                         \en File path. \~
  \param[in] prop -      \ru Реализация интерфейса свойств конвертера.
                         \en Implementation of converter's properties interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \ingroup Exchange_Interface
  */
  virtual MbeConvResType ImportFromFile( ItModelDocument& mDoc, const c3d::path_string& filePath, IConvertorProperty3D* prop = nullptr, IProgressIndicator* indicator = nullptr ) = 0;

  /** \brief \ru Получить список поддерживаемых форматов.
  \en Get list of supported formats. \~
  \return \ru Список поддерживаемых форматов, с учётом лицензионного ключа и наличия формат-специфичных библиотек конвертера.
          \en A list of supported formats accounting the license key and presence of format-specific converter libraries. \~
  \details \ru Если получение списка форматов не поддерживается каким-либо плагином, то его форматы не будут включены в данный список.
           \en If some plugin does not support format listing, then its formats will not be included into the list. \~
  \ingroup Exchange_Interface
  */
  virtual std::vector<c3d::FormatInfo> GetSupportedImportFormats() const = 0;

  /** \brief \ru Получить список поддерживаемых форматов для импорта через метод ImportForeign().
  \en Get list of supported formats for import via the ImportForeign() method. \~
  \return \ru Список поддерживаемых форматов. Список будет пустым если метод LoadForeignReader() 
              не был вызван или завершился неуспешно.
          \en A list of supported formats. The list is expected to be empty when the LoadForeignReader()
              has not been called or failed.  \~
  \details \ru Если получение списка форматов не поддерживается каким-либо плагином, то его форматы не будут включены в данный список.
           \en If some plugin does not support format listing, then its formats will not be included into the list. \~
  \ingroup Exchange_Interface
  */
  virtual std::vector<c3d::FormatInfo> GetSupportedImportForeignFormats() const = 0;
}; // IConverter3D

typedef IConverter3D IConvertor3D;


//------------------------------------------------------------------------------
/** \brief \ru Получить интерфейс конвертера.
           \en Get the converter interface. \~
\ingroup Exchange_Interface
*/
CONV_FUNC( IConverter3D* ) GetConverter3D();


//------------------------------------------------------------------------------
/** \brief \ru Освободить интерфейс конвертера.
           \en Release the converter interface. \~
\ingroup Exchange_Interface
*/
CONV_FUNC( void ) ReleaseConverter3D( IConverter3D* );



//------------------------------------------------------------------------------
/** \brief \ru Получить интерфейс конвертера.
           \en Get the converter interface. \~
\ingroup Exchange_Interface
*/
inline IConverter3D* GetConvertor3D() { return GetConverter3D(); }


//------------------------------------------------------------------------------
/** \brief \ru Освободить интерфейс конвертера.
           \en Release the converter interface. \~
\ingroup Exchange_Interface
*/
inline void ReleaseConvertor3D( IConverter3D* convInstance ) { ReleaseConverter3D( convInstance ); }


/** \} */


#endif // __CONV_MODEL_EXCHANGE_H
