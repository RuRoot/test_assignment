////////////////////////////////////////////////////////////////////////////////
/// Слежение за утечками
/**
  \file
  \brief  Содержит класс MemoryLeaksVerifiable - базовый для контролируемых классов
*/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math_define.h>

#if ( (defined(_MSC_VER) && (_MSC_VER > 1800)) || !defined(C3D_WINDOWS) )
#define ENABLE_MEMORY_LEAKS_CHECK
#endif


namespace c3d // namespace C3D
{

#ifdef ENABLE_MEMORY_LEAKS_CHECK

//------------------------------------------------------------------------------
/** \brief \ru Базовый класс для контролируемых по утечкам классов.
           \en Base class for leak-controlled classes. \~
  \details \ru Базовый класс для контролируемых по утечкам классов.
               В конструкторе объект регистрируется в менеджере утечек, в деструкторе удаляется из списка зарегистрированных.
               Информация о всех объектах, которые остались в регистраторе, будет выведена. \n
           \en Base class for leak-controlled classes. \n \~ 
  \ingroup Base_Tools
*/
// ---
class MATH_CLASS MemoryLeaksVerifiable {
  friend class MemoryLeaksCatcher;

private:
  mutable size_t m_index; ///< \ru Индекс данного объекта в контейнере зарегистрированных объектов. \en The index of this object in the container of registered objects.

protected:
  MemoryLeaksVerifiable();
  virtual ~MemoryLeaksVerifiable();
  MemoryLeaksVerifiable( const MemoryLeaksVerifiable & ) : MemoryLeaksVerifiable() {}
  MemoryLeaksVerifiable( MemoryLeaksVerifiable && ) : MemoryLeaksVerifiable() {}
  MemoryLeaksVerifiable & operator = ( const MemoryLeaksVerifiable & ) { return *this; }
  MemoryLeaksVerifiable & operator = ( MemoryLeaksVerifiable && ) { return *this; }

private:
  /// \ru Задать индекс данного объекта в контейнере зарегистрированных объектов. \en Set the index of this object in the container of registered objects. 
  void    SetIndex( size_t index ) const { m_index = index; }
  /// \ru Получить индекс данного объекта в контейнере зарегистрированных объектов. \en Get the index of this object in the container of registered objects.
  size_t  GetIndex() const { return m_index; }
};

#else

class MATH_CLASS MemoryLeaksVerifiable {};

#endif // ENABLE_MEMORY_LEAKS_CHECK

} // namespace C3D
