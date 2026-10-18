////////////////////////////////////////////////////////////////////////////////
/// Утилиты Слежения за утечками
/**
  \file
  \brief Содержит интерфейс MemoryLeaksController - контроллер утечек памяти
*/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <tool_memory_leaks_check.h>
#include <unordered_map>


namespace c3d // namespace C3D
{
#ifdef ENABLE_MEMORY_LEAKS_CHECK

typedef std::vector< std::pair<const MemoryLeaksVerifiable *, std::wstring> > MemoryLeaksRegisteredData;
typedef std::unique_ptr<struct MemoryLeaksController>                         MemoryLeaksControllerPtr;

//----------------------------------------------------------------------------------------
/** \brief \ru Контроллер утечек памяти.
           \en Memory Leaks Controller. \~
  \details \ru Контроллер утечек памяти. При выходе из приложения, вызывается метод OnLeakDetect, в который передается информация об утечках.
           \en Memory Leaks Controller. When you exit the application, the OnLeakDetect method is called, to which information about leaks is transmitted. \~
  \ingroup Base_Tools
*/
//---
struct MemoryLeaksController
{
  MemoryLeaksController() {}
  virtual ~MemoryLeaksController() {}
  virtual void OnLeakDetect( const MemoryLeaksRegisteredData & ) const = 0;
};


//------------------------------------------------------------------------------
/** \brief \ru Добавить контроллер утечек памяти.
           \en Add memory leaks controller. \~
\ingroup Base_Tools
*/
// ---
MATH_FUNC( void ) AddController( MemoryLeaksControllerPtr );


#ifdef _UNICODE
//------------------------------------------------------------------------------
/** \brief \ru Добавить внешнюю библиотеку контроллером утечек памяти.
           \en Add memory leaks controller. \~
\ingroup Base_Tools
*/
// ---
MATH_FUNC(void) AddExternalController( const std::wstring & dllName, const std::string & funcName, const std::wstring & args );
#endif //_UNICODE


//------------------------------------------------------------------------------
/** \brief \ru Собрать утечки. Необходимо вызвать перед выходом из приложения, чтобы выполнить именование утекающих объектов.
           \en Collect leaks. The function should be called before exiting the application to names the leaking objects. \~
\ingroup Base_Tools
*/
// ---
MATH_FUNC( void ) CollectLeaks();

#else

MATH_FUNC( void ) CollectLeaks() {}

#endif // ENABLE_CHECK_MEMLEAK

} // namespace c3d

