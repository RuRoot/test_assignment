////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Индикатор прогресса выполнения.
         \en A run progress indicator. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TOOL_PROGRESS_INDICATOR_H
#define __TOOL_PROGRESS_INDICATOR_H

#include <tool_cstring.h>

//------------------------------------------------------------------------------
/** \brief \ru Индикатор прогресса выполнения.
           \en A run progress indicator. \~
  \details \ru Интерфейс индикатора прогресса выполнения. \n
           \en Interface of the execution progress indicator. \n \~
  \note    \ru Геометрическое ядро обеспечивает потокобезопасность использования индикатора прогресса.
           Пользователь должен обеспечить потокобезопасность внутренней реализации IfProgressIndicator.
           \en The geometric kernel provides the thread-safe use of the progress indicator.
               The user is responsible for thread-safety of IfProgressIndicator internal implementation. \~
  \ingroup Base_Tools
*/
// ---
struct MATH_CLASS IfProgressIndicator
{
  /// \ru Инициализация. \en Initialization. 
  virtual void          StartProgress  ( ptrdiff_t minValue, ptrdiff_t maxValue, const TCHAR * lpszNewText, bool resetTxt, bool aPIcall = false ) = 0;
  /// \ru Установка текущего значения. \en Setting of the current value. 
  virtual void          SetProgress    ( ptrdiff_t nCurr, const TCHAR * lpszNewText, bool resetTxt, bool aPIcall = false ) = 0;
  /// \ru Остановить индикатор. \en Stop the indicator. 
  virtual void          StopProgress   ( const TCHAR * lpszNewText, bool resetTxt, bool aPIcall = false ) = 0;
  /// \ru Получить строку из индикатора. \en Get the string from the indicator. 
  virtual const TCHAR * GetStrBuild    () const = 0;
  /// \ru Установить строку в индикатор. \en Set the string to the indicator. 
  virtual void          StBarSetMessageText( const TCHAR * msg, bool aPIcall = false ) = 0; // \ru Переименовано для отличия от аналогичных функций в окне, процессе и приложении \en Renamed to make different the similar functions in window, in process and in application 
  /// \ru Запущен ли какой-нибудь процесс с индикатором. \en Whether any process with indicator is run. 
  virtual bool          IsProgressStarted() const = 0;
};


#endif // __TOOL_PROGRESS_INDICATOR_H
