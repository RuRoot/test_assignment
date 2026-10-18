////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Динамический одномерный массив без счетчика количества элементов.
         \en Dynamic one-dimensional array without counter of elements number. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_C_ARRAY_H
#define __TEMPL_C_ARRAY_H


#include <string.h>
#ifndef _INC_STDLIB
  #include <stdlib.h>
#endif
#include "tool_quick_sort.h"
#include "io_define.h"
#include <tool_err_handling.h>



FORVARD_DECL_TEMPLATE_TYPENAME( class CcArray );
FORVARD_DECL_TEMPLATE_TYPENAME( void fill_array( CcArray<Type> &, const Type & fillData, size_t cnt ) );


//-----------------------------------------------------------------------------
/** \brief \ru Динамический одномерный массив без счетчика количества элементов.
           \en Dynamic one-dimensional array without counter of elements number. \~
  \details \ru Динамический одномерный массив без счетчика количества элементов. \n
    Применяется для выделения памяти под массив, когда не требуется знать размер
    массива. Под отладкой контролируется некорректное обращение по индексу за
    пределы массива.
           \en Dynamic one-dimensional array without counter of elements number. \n
    It is used to allocate memory for array when there is not required to know a size
    of array. In debug mode there is performed a control of incorrect reference by index out of
    array bounds. \~
  \warning \ru Класс остался для поддержки старых кодов. Вместо него можно использовать массивы из STL.
           \en The class is left to support old codes. Arrays from STL can be used instead of it. \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class CcArray
{
  Type  * parr; ///< \ru Указатель на первый элемент массива. \en A pointer to the first array element.
#if defined(C3D_DEBUG)
  size_t count; ///< \ru Количество элементов массива (для отладки). \en A number of elements in array (for debugging).
#endif

public :
  /// \ru Конструктор. \en Constructor.
  CcArray( size_t /*count*/ );
  /// \ru Деструктор. \en Destructor.
 ~CcArray() { SetArraySize( 0 ); }

  /// \ru Заполнить массив значениями. \en Fill an array.
  void          Fill( const Type &data, size_t cnt ) { fill_array( *this, data, cnt ); }
  /// \ru Копировать в себя (со смещением offset) cnt значений из from. \en Copy to itself (with the shift 'offset') 'cnt' values from 'from'.
  void          Copy( const void * from, size_t cnt, size_t offset = 0 );
  /// \ru Перераспределить память. \en Reallocate memory.
  bool          SetArraySize( size_t newCount );
  /// \ru Освободить память. \en Free memory.
  void          FreeMemory() { SetArraySize( 0 ); }
  /// \ru Оператор доступа. \en An access operator.
  Type        & operator []( size_t idx ) const { PRECONDITION( idx < count ); return parr[idx]; } // SKIP_SA
  /// \ru Выделена ли память? \en Is memory allocated?
  bool          IsNull () const { return parr == nullptr; }
  /// \ru Выдать адрес начала массива. \en Get address of the beginning of an array.
  const Type  * GetAddr() const { return parr;         }

  /// \ru Заполнить cnt элементов массива значением fillData. \en Fill 'cnt' elements of an array by the values of 'fillData'.
  TEMPLATE_FRIEND void fill_array TEMPLATE_SUFFIX ( CcArray<Type> &, const Type & fillData, size_t cnt );

private:
  CcArray( const CcArray & );           // \ru запрещено \en forbidden
  void operator = ( const CcArray & );  // \ru запрещено \en forbidden

#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
public:
  void * operator new    ( size_t );
  void   operator delete ( void *, size_t );
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_
};


#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
//------------------------------------------------------------------------------
// \ru Перегрузка оператора new. \en Overloading of the "new" operator. 
// ---
template <class Type>
inline void * CcArray<Type>::operator new( size_t size ) {
  return ::Allocate( size, typeid(CcArray<Type>).name() );
}

//------------------------------------------------------------------------------
// \ru Перегрузка оператора delete. \en Overloading of the "delete" operator. 
// ---
template <class Type>
inline void CcArray<Type>::operator delete( void * ptr, size_t size ) {
  ::Free( ptr, size, typeid(CcArray<Type>).name() );
}
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_


//------------------------------------------------------------------------------
// \ru Конструктор массива \en Constructor of an array.
// ---
template <class Type>
inline CcArray<Type>::CcArray( size_t _count )
  : parr( 0 )
#if defined(C3D_DEBUG)
  , count( 0 )
#endif
{
  if ( !SetArraySize( _count ) && !ExceptionMode::IsEnabled() )
    throw std::bad_alloc(); // \ru Бросить исключение при любом режиме. \en Throw exception in case of any mode.
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline void CcArray<Type>::Copy( const void * from, size_t cnt, size_t offset )
{
  PRECONDITION( (offset + cnt <= count) && (cnt ? from != nullptr : true) );
  memcpy( parr + offset, from, cnt * sizeof(Type) ); // SKIP_SA
}


//------------------------------------------------------------------------------
// \ru перераспределить память \en reallocate memory.
// ---
template <class Type>
inline bool CcArray<Type>::SetArraySize( size_t newCount )
{
  if ( ::TestNewSize(sizeof(Type), newCount) )
  {
    try {
#ifdef C3D_DEBUG
#ifdef __REALLOC_ARRAYS_STATISTIC_
      void * oldParr = parr;
      size_t oldSize = count;
#endif // __REALLOC_ARRAYS_STATISTIC_
#endif

#ifdef USE_REALLOC_IN_ARRAYS
      if ( parr != nullptr || newCount != 0 ) {
        // \ru показывает утечки памяти, если parr==nullptr и newCount==0 \en Memory leaks happen if parr==nullptr and newCount==0
        parr = static_cast<Type*>( REALLOC_ARRAY_SIZE(parr, newCount * sizeof(Type), true/*clear*/) );
      }
#else
      Type * p_tmp = newCount ? new Type[newCount] : nullptr;

      delete[] parr; // \ru Удалять parr, если оператор new выполнился успешно. \en Delete parr if operator new is succeeded.
      parr = p_tmp;
#endif // USE_REALLOC_IN_ARRAYS

#if defined(C3D_DEBUG)
      count = newCount;
#ifdef __REALLOC_ARRAYS_STATISTIC_
      REALLOC_ARRAY_STATISTICS( oldParr, oldSize * sizeof(Type), parr, newCount * sizeof(Type), 4/*CcArray*/ );
#endif // __REALLOC_ARRAYS_STATISTIC_
#endif // C3D_DEBUG
    }
    catch ( const std::bad_alloc & ) {
      C3D_CONTROLED_THROW;
      return false;
    }
    catch ( ... ) {
      if ( newCount == 0 )// \ru Не смогли удалить parr. \en Failed to delete parr.
        parr = nullptr;
      C3D_CONTROLED_THROW;
      return false;
    }
  }
  else {
    PRECONDITION( false ); // \ru не бывает столько памяти \en incorrect size of memory
    C3D_CONTROLED_THROW_EX( std::bad_alloc() );
    return false;
  }
  return true;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
void fill_array( CcArray<Type> & arr, const Type & fillData, size_t cnt )
{
  PRECONDITION( cnt <= arr.count );
  for ( size_t i = 0; i < cnt; i++ )
    memcpy( &arr[i], &fillData, sizeof(Type) );
}


//------------------------------------------------------------------------------
// \ru Инициализировать массив символов с присланной строки \en Initialize an array of symbols from a given string
// ---
inline void InitCharArray( CcArray<char> & array, const char * text )
{
  if ( text ) {
    size_t len = ::strlen( text ) + 1;
    if ( array.SetArraySize( len ) )
      array.Copy( text, len );
  }
  else {
    if ( array.SetArraySize( 1 ) )
      array[0] = 0;
  }
}


//-- __PRECOMPILED_HEADER_OPTIMIZE -------------------------------------------------
#ifdef _PCH_OPT
#pragma message( "----" __FILE__ )
#endif


#endif // __TEMPL_C_ARRAY_H
