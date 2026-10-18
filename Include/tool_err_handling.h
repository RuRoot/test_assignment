////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Сервис для обработки ошибок.
         \en Error-handling services. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TOOL_ERR_HANDLING_H
#define __TOOL_ERR_HANDLING_H

#include <math_define.h>
#include <mb_operation_result.h>
#include <system_atomic.h>
#include <stdexcept>

#include <tool_memory_debug.h>// \ru Внимание! Читаем внимательно! Если вытереть отсюда, то подключить везде, где есть функции из этого файла! \en Attention! Read carefully! If remove this from here, then it should be included anywhere where the functions from this file exist! 


//------------------------------------------------------------------------------
/**
  \brief   \ru Определение режима обработки исключений.
           \en Definition of mode for exception handling. \~
  \details \ru Определяет, пробрасывать ли исключение дальше после его обработки. По умолчанию исключения подавляются. \n
           \en Defines, whether to throw exception further after its processing. By default, exceptions are suppressed. \~
  \ingroup Base_Tools
*/
//---
class MATH_CLASS ExceptionMode
{
  static bool sExptEnabled;
public:
  /** \brief \ru  Установить режим обработки исключений: true, чтобы пробрасывать исключения дальше; false, чтобы подавлять исключения.
                  Возвращает предыдущий режим.
             \en  Set mode for exception handling: true - to throw exceptions further; false - to suppress exceptions.
                  Return the previous mode.
  */
  static bool Enable( bool enabled = true );

  /** \brief \ru Получить текущий режим обработки исключений (true - пробрасывать исключение дальше; false - нет).
             \en Get current exception handling mode (true - to throw exception further; false - to not throw).
  */
  static bool IsEnabled();
};

//------------------------------------------------------------------------------
/**
  \brief \ru Меняет режим обработки исключений в области видимости.
         \en Alter mode for exception handling in a scope. \~
  \ingroup Base_Tools
*/
//---
class MATH_CLASS ScopedExceptionMode
{
  bool m_oldMode;
public:
  ScopedExceptionMode( bool mode = true ) { m_oldMode = ExceptionMode::Enable( mode ); }
  ~ScopedExceptionMode() { ExceptionMode::Enable( m_oldMode ); }
};


//------------------------------------------------------------------------------
/**
  \brief \ru Бросить указанное исключение, если режим позволяет.
         \en Throw the specified exception if allowed by the exception mode. \~
  \ingroup Base_Tools
*/
//---
#define C3D_CONTROLED_THROW_EX(expt) if( ExceptionMode::IsEnabled() ) throw expt;

//------------------------------------------------------------------------------
/**
  \brief \ru Бросить исключение, если режим позволяет.
         \en Throw exception if allowed by the mode for exception handling. \~
  \ingroup Base_Tools
*/
//---
#define C3D_CONTROLED_THROW if( ExceptionMode::IsEnabled() ) throw;

//------------------------------------------------------------------------------
/**
  \brief   \ru Обработчик фатальных ошибок.
           \en Handler of fatal errors. \~
  \details \ru Обработчик фатальных ошибок.
           Перед началом мониторинга должна вызываться функция Init().
           При выходе из проблемной области должна вызываться функция Recover(). \n
           \en Handler of fatal errors.
           Before starting error monitoring the Init() function should be called.
           On exit a faulty region, the Recover() function should be called. \~
  \ingroup Base_Tools
*/
//---
class MATH_CLASS FatalErrorHandler
{
public:

  /** \brief \ru  Получить последнюю ошибку. \en  Get the last error.
  */
  static MbResultType GetError();

  /** \brief \ru  Есть ли фатальная ошибка. \en  Is there a fatal error.
  */
  static bool         HasError();

  /** \brief \ru  Зарегистрировать фатальную ошибку. Возвращает эту ошибку.
             \en  Register a fatal error. Return this error.
  */
  static MbResultType SetError( MbResultType );

  /** \brief \ru  Достаточно ли памяти для работы. \en  Whether is enough memory to work.
      \param[in]  memRatio - \ru Максимальный процент использования памяти (при memRatio == -1 используются значения по умолчанию).
                             \en The maximum percentage of memory usage (with memRatio == -1, the default values are used). \~
  */
  static bool         IsEnoughMemory( double memRatio = -1 );

  /** \brief \ru  Сбросить все накопленные данные.
             \en  Reset all accumulated data.
  */
  static void         Reset();

  /** \brief \ru  Активирован ли обработчик фатальных ошибок (по умолчанию - активирован).
             \en  Whether the handler of fatal errors is enabled (by default, enabled).
  */
  static bool         IsEnabled();

  /** \brief \ru  Активировать / деактивировать обработчик фатальных ошибок. Возвращает предыдущее состояние.
             \en  Enable / disable the handler of fatal errors. Return the previous state.
  */
  static bool         Enable( bool enabled = true );
};

#endif // __TOOL_ERR_HANDLING_H
