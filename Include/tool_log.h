////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Логирование информации.
         \en Information logging. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TOOL_LOG_H
#define __TOOL_LOG_H

#include <io_define.h>
#include <tool_cstring.h>
#include <math_define.h>

namespace c3d //namespace c3d
{

//------------------------------------------------------------------------------
// \ru Потокобезопасные интерфейсы для ведения журнала сообщений и записи его в файл.
//     Для каждого потока ведется отдельный лог.
//     Затем накопленные сообщения, сортированные по потокам записываются в файл.
//     Интерфейсы доступны в дебаге.
// \en Thread-safe interfaces for logging messages and writing the log to the file.
//     A separate log is maintained for each thread.
//     The collected messages, sorted by threads, are then written to a file.
//     The interfaces are available in debug configuration.
//
/* \ru Пример использования.
     int k = 0;
     SetLogDir( _T("C:\\AnoherDir") ); // Меняем умолчательную директорию для лог-файлов.
     START_LOGGING;                           // Начинаем логирование.
     LOG_MSG( _T("My sample log") );          // Добавляем указанную строку в лог.
     ...
     // Форматируем строку лога и добавляем ее в лог с помощью оператора << Logger::Endl.
     c3d::Logger::Get() << _T("Value: ") << k << Logger::Endl;
     ...
     WRITE_LOG_FILE( _T("sample") ); // Записываем лог в файл sample.log.
     END_LOGGING;                    // Отключаем логирование.

   \en Usage sample. 
     int k = 0;
     SetLogDir( _T("C:\\AnoherDir") ); // Change default directory for log-files.
     START_LOGGING;                           // Start logging.
     LOG_MSG( _T("My sample log") );          // Put a specified string to the log.
     ...
     //  Format a log line and  puts it to the log using operator << Logger::Endl.
     c3d::Logger::Get() << _T("Value: ") << k << Logger::Endl;
     ...
     WRITE_LOG_FILE( _T("sample") ); // Write the log to the file sample.log.
     END_LOGGING;                    // Stop logging.
*/
// ---

//------------------------------------------------------------------------------
// \ru Класс для форматирования строки для лога и добавления ее в лог.
// \en Class for formatting a line to the log and writing it to the log.
// ---
class MATH_CLASS Logger
{
public:

  // \ru Тип сообщения. \en The message type.
  enum MsgType
  {
    ms_info,
    ms_warn,
    ms_err
  };

public:
  // \ru Получить логгер. \en Get the logger.
  static Logger & Get();

  // \ru Следующие методы позволяют форматировать строку для лога. Работают с текущей строкой лога.
  // \en Next methods can be used to format a log line. Work with the current line of the log.

#ifdef _UNICODE
  // \ru Добавить строку в текущую строку лога. \en Add a string to the current line of the log.
  virtual Logger & operator << ( const TCHAR * ) = 0;
#endif
  // \ru Добавить строку в текущую строку лога. \en Add a string to the current line of the log.
  virtual Logger & operator << ( const char * ) = 0;
  // \ru Добавить integer в текущую строку лога. \en Add integer to the current line of the log.
  virtual Logger & operator << ( const int & ) = 0;
#if defined(PLATFORM_64) // \ru x32 совпадение типов ptrdiff_t и int \en x32 coincidence of ptrdiff_t and int types
  // \ru Добавить ptrdiff_t в текущую строку лога. \en Add ptrdiff_t to the current line of the log.
  virtual Logger & operator << ( const ptrdiff_t & ) = 0;
#else
#ifdef __EMSCRIPTEN__
  // \ru Добавить long в текущую строку лога. \en Add integer to the current line of the log.
  virtual Logger & operator << ( const long & ) = 0;
#endif
#endif
  // \ru Добавить size_t в текущую строку лога. \en Add size_t to the current line of the log.
  virtual Logger & operator << ( const size_t & ) = 0;
  // \ru Добавить double в текущую строку лога. \en Add double to the current line of the log.
  virtual Logger & operator << ( const double & ) = 0;
  // \ru Добавить признак тип сообщения в текущую строку лога. \en Add message type to the current line of the log.
  virtual Logger & operator << ( MsgType type ) = 0;

  // \ru Завершить форматирование текущей строки и добавить ее в лог. Следующий вызов оператора << создаст новую текущую строку.
  // \en Finish formatting of the current line and add it to the log. Next call to the operator << will create new current line.
  virtual Logger & operator << ( Logger& (*man)( Logger& ) ) = 0;

  // \ru Манипулятор-признак завершения форматирования текущей строки лога.
  // \en Manipulator-indicator of finishing formatting of the current line of the log.
  static Logger & Endl( Logger& );
};


//------------------------------------------------------------------------------
/** \brief \ru Переключатель логирования в области видимости.
           \en Logging switch in a scope. \~
  \details \ru Класс для логирования в области видимости.
               В конструкторе логирование включается, в деструкторе - отключается.
               Если в конструкторе было указано имя файла, то перед отключением логирования
               деструктор записывает накопленный лог в файл.
           \en Helper class for the logging in the scope.
               The logging starts in the constructor and ends in the destructor.
               If a file name was defined in the constructor, then before ending the logging,
               the destructor writes the collected log to the file.
           \ru Пример использования:
             {
               // Начинаем логирование, лог будет записан в файл myLogFile.log.
               c3d::LogHelper log( true, _T("myLogFile") );

               // Добавляем указанную строку в лог.
               LOG_MSG( _T("My formatted message") );
               ...
               // Форматируем строку лога и добавляем ее в лог с помощью оператора << Logger::Endl.
               c3d::Logger::Get() << _T("Value ") << k << Logger::Endl;
               ...
             }  // При выходе из области кода логирование заканчивается и лог записывается в файл.

           \en Example of use:
             {
               // Start logging, the log will be written to the file myLogFile.log.
               c3d::LogHelper log( true, _T("myLogFile") );

               // Put a specified string to the log.
               LOG_MSG( _T("My formatted message") );
               ...
               // Format a log line and put it to the log using the operator << Logger::Endl.
               c3d::Logger::Get() << _T("Value ") << k << Logger::Endl;
               ...
             }  // When leaving the code scope the logging stops and the log is written to the file.
   \ingroup Base_Tools
*/
// ---
class MATH_CLASS LogHelper
{
  bool          _enabled;
  c3d::string_t _file;

public:

  // \ru Параметр enable определяет, стартовать ли логирование.
  // \en The enable parameter defines whether to start logging.
  LogHelper( bool enable = true, const TCHAR* file = nullptr );

  // \ru Если в конструкторе было указано имя файла, то деструктор записывает накопленный лог в файл.
  // \en If a file name was defined in the contructor, the destructor writes the collected log to the file. \~
  ~LogHelper();

private:
  void Init( bool enable, const TCHAR * file );
  void Reset();
};

//------------------------------------------------------------------------------
// \ru Функции логирования. \en Logging functions.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/** \brief \ru Установить директорию для логов. \en Set log directory. \~
  \details \ru По умолчанию лог-файлы сохраняются в директорию %USERPROFILE%/Logs.
               Функция позволяет изменить директорию для логов.
           \en By default log-files are saved to the directory %USERPROFILE%/Logs.
               The function allows changing the directory for logs. \~
  \param dirPath - \ru Полный путь к директории. \en Full path to log directory.
  \return \ru true, если директория создана. \en true if directory is created.
  \ingroup Base_Tools
*/
// ---
MATH_FUNC(bool) SetLogDir( const TCHAR * dirPath );

//------------------------------------------------------------------------------
/** \brief \ru Начать или закончить логирование. \en Start or stop logging. \~
  \details \ru Начать или закончить логирование. \en Start or stop logging. \~
  \param enable - \ru Полный путь к директории. \en Whether.
  \ingroup Base_Tools
*/
// ---
MATH_FUNC(void) SetLogging( bool enable );

//------------------------------------------------------------------------------
/** \brief \ru Записать лог в файл. \en Write the log to the file. \~
  \details \ru Записываются в файл сообщения, сортированные по потокам.
           \en Write messages, sorted by threads, to the file. \~
  \ingroup Base_Tools
*/
// ---
MATH_FUNC(void) WriteLog( const TCHAR *fileName );

//------------------------------------------------------------------------------
/** \brief \ru Записать указанную строку в лог. \en Write a specified string to the log. \~
  \details \ru Записать указанную строку в лог. \en Write a specified string to the log. \~
  \ingroup Base_Tools
*/
// ---
MATH_FUNC(void) LogMessage( const c3d::string_t &msg );

//------------------------------------------------------------------------------
/** \brief \ru Записать указанную строку и тип сообщения в лог.
           \en Write a specified string and the message type to the log. \~
  \details \ru Записать указанную строку и тип сообщения в лог.
           \en Write a specified string and the message type to the log. \~
  \ingroup Base_Tools
*/
// ---
MATH_FUNC(void) LogMessage( const c3d::string_t &msg, Logger::MsgType type );


//------------------------------------------------------------------------------
// \ru Макросы для операций логирования. \en Logging macros.
//------------------------------------------------------------------------------

// \ru Информация о месте. \en  The place information.
#ifdef C3D_WINDOWS
#define __LOCATION__  __FILE__ "(" __DEFTOSTR__(__LINE__) "): function " __FUNCTION__ "(): "
#else
inline std::string __location() {
  std::string s( __FILE__ "(" __DEFTOSTR__( __LINE__ ) "): function " );
  return s + std::string( __PRETTY_FUNCTION__ ) + "(): ";
}

#define __LOCATION__  __location().c_str()
#endif

// \ru Начать логирование. \en Start logging.
#define START_LOGGING(var)        c3d::SetLogging( var );
// \ru Закончить логирование. \en Stop logging.
#define END_LOGGING               c3d::SetLogging( false );
// \ru Записать лог в файл. Лог для каждого потока записывается в отдельный файл.
// \en Write the log to the file. Log of each thread writes to a separate file.
#define WRITE_LOG_FILE(fileName)  c3d::WriteLog( fileName );

// \ru Положить в лог форматированную строку. \en Put a formatted string to the log.
#define LOG_MSG(msg)                  c3d::LogMessage( msg );
// \ru Положить в лог форматированную строку с указанием места. \en Put a formatted string with the place info to the log.
#define LOG_MSG_PLACE(msg)            c3d::Logger::Get() << __LOCATION__ << msg << Logger::Endl;
// \ru Положить в лог форматированную строку с типом сообщения и указанием места. \en Put a formatted string with the message type and the place to the log.
#define LOG_MSG_PLACE_TYPE(msg,type)  c3d::Logger::Get() << __LOCATION__ << type << msg << Logger::Endl;

//------------------------------------------------------------------------------
/** \brief \ru Включить контроль утечек памяти.
           \en Enable memory leakage control. \~ 
  \details \ru Включить контроль утечек памяти.
           \en Enable memory leakage control. \~
  \ingroup Base_Tools
*/
// ---
MATH_FUNC(void) EnableMemoryLeakDump();

} //namespace c3d


#endif // __TOOL_LOG_H
