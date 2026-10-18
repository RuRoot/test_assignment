////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Контроль выделения памяти под отладкой.
         \en Memory allocation control during the debugging process. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MEMORY_DEBUG_H
#define __MEMORY_DEBUG_H

#include <math_define.h>
#include <memory.h>

////////////////////////////////////////////////////////////////////////////////
// \ru Макросы для переключения контроля выделения памяти находятся в math_cfg.h
// \en Macros for switching control of memory allocation are in math_cfg.h
////////////////////////////////////////////////////////////////////////////////

#ifdef C3D_DEBUG
  #if defined(C3D_MacOS) // mac
  #include <malloc/malloc.h>
  #elif defined(C3D_FreeBSD)
  #include <stdlib.h>
  #else
  #include <malloc.h>
  #endif // mac

  #ifndef __DISABLE_MEMORY_CONTROL__  // \ru Чтобы можно было отключать в других проектах \en To allow to disable it in other projects 
    #define USE_REALLOC_IN_ARRAY
  #endif // __DISABLE_MEMORY_CONTROL__
#endif // C3D_DEBUG


#ifdef __REALLOC_ARRAYS_STATISTIC_
#include <string>
#include <fstream>

//------------------------------------------------------------------------------
/**
  \brief   \ru Класс для контроля выделения памяти под отладкой.
           \en Class for controling memory allocation during the debugging process.\~
  \ingroup Base_Tools
*/
//---
class MATH_CLASS MemoryChecker
{
public:

  /// \ru Получить указатель на синглтон класса. \en Get pointer to the class singleton.
  static MemoryChecker * Get();

  /// \ru Перезапуск сбора статистики. Очистить, чтобы в следующий раз цифры были новые, а не накопленные.
  /// \en Restart collecting statistics. Clear to renew numbers.
  virtual void RestartStatistic() = 0;

  /// \ru Статистика изменений размера массива. \en Statistics of array size changes.
  // arrayType : 
  // \ru 0 - SArray (или наследники), \en 0 -SAray (or inheritors) 
  // \ru 1 - RParray (или наследники), \en 1 -RPAray (or inheritors) 
  // 2 - Array2,
  // 3 - LiSArray,
  // 4 - CcArray,
  // \ru 5 - неопознанные (вообще-то, такого не должно быть) \en 5 - not defined (it should not happen) 
  virtual void ReallocArrayStatistic( void * oldParr, size_t oldSize, void * newParr, size_t newSize, uint arrayType ) = 0;

  /// \ru Отчет по статистике изменений размера массива. \en A report by the statistics of array size changes.  \~ \ingroup Base_Tools
  virtual void ReallocReport( bool clear, const char * title = nullptr ) = 0;

};

//------------------------------------------------------------------------------
/// \ru Дополнить статистику изменений размера массива. \en Add statistics of array size changes.
//---
#define REALLOC_ARRAY_STATISTICS(oldParr,oldSize,newParr,newSize,arrayType) \
MemoryChecker::Get()->ReallocArrayStatistic( oldParr, oldSize, newParr, newSize, arrayType );

#endif

#if defined(__DEBUG_MEMORY_ALLOCATE_FREE_) || defined(USE_REALLOC_IN_ARRAYS)
  #define C3D_INVALID_UINT32_1  SYS_MAX_UINT32
  #define C3D_INVALID_UINT32_2  0xEEEEEEEE
  #define C3D_INVALID_UINT16_1  SYS_MAX_UINT16
  #define C3D_INVALID_UINT16_2  0xEEEE
  #define C3D_MEMORY_FILL_VALUE 0xEE
  #ifdef PLATFORM_64
    #define C3D_INVALID_ADDR_1    0xFFFFFFFFFFFFFFFF
    #define C3D_INVALID_ADDR_2    0xEEEEEEEEEEEEEEEE
  #else // PLATFORM_64
    #define C3D_INVALID_ADDR_1    0xFFFFFFFF
    #define C3D_INVALID_ADDR_2    0xEEEEEEEE
  #endif // PLATFORM_64
#endif // (_DEBUG_MEMORY_ALLOCATE_FREE_ || USE_REALLOC_IN_ARRAYS)

#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_

//------------------------------------------------------------------------------
/// \ru Проверить указатель и значение. \en Check pointer and value.  \~ \ingroup Base_Tools
// ---
inline void CheckPointerAndValue( void * ptr, size_t size )
{
#if defined (C3D_WINDOWS) && !defined(ALL_WARNINGS)  // Set warnings level
#pragma warning(disable: 4312)
#endif
  PRECONDITION( ptr != (ptrdiff_t *)C3D_INVALID_ADDR_2 );
  PRECONDITION( ptr != (ptrdiff_t *)C3D_INVALID_ADDR_1 );
#if defined (C3D_WINDOWS) && !defined(ALL_WARNINGS)  // Set warnings level
#pragma warning(default: 4312)
#endif

  if ( ptr ) {
    if ( size > 11 ) {
      uint32 * _ptr = (uint32 *)ptr;
      uint32 value1 = *_ptr++;
      uint32 value2 = *_ptr++;
      uint32 value3 = *_ptr;
      PRECONDITION( value1 != C3D_INVALID_UINT32_2 || value2 != C3D_INVALID_UINT32_2 || value3 != C3D_INVALID_UINT32_2 );
      PRECONDITION( value1 != C3D_INVALID_UINT32_1 || value2 != C3D_INVALID_UINT32_1 || value3 != C3D_INVALID_UINT32_1 );
    }
    else if ( size > 7 ) {
      uint32 * _ptr = (uint32 *)ptr;
      uint32 value1 = *_ptr++;
      uint32 value2 = *_ptr;
      PRECONDITION( value1 != C3D_INVALID_UINT32_2 && value2 != C3D_INVALID_UINT32_2 );
      PRECONDITION( value1 != C3D_INVALID_UINT32_1 || value2 != C3D_INVALID_UINT32_1 );
    }
    else if ( size > 3 ) {
      uint32 value = *(uint32 *)ptr;
      PRECONDITION( value != C3D_INVALID_UINT32_2 );
      PRECONDITION( value != C3D_INVALID_ADDR_1 );
    }
    else if ( size > 1 ) {
      uint16 value = *(uint16 *)ptr;
      PRECONDITION( value != C3D_INVALID_UINT16_2 );
      PRECONDITION( value != C3D_INVALID_UINT16_1 );
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Выделить память указанного размера. \en Allocate memory of the given size.  \~ \ingroup Base_Tools
// ---
inline void * Allocate( size_t size, const char * )
{
  void * ptr = ::malloc( size );
  // \ru Дабы работал _msize: void * ptr = ::operator new( size ); \en For working of _msize: void * ptr = ::operator new( size ); 

  if ( ptr ) {
    ::memset( ptr, 0xFF, size );
  }
  return ptr;
}

//------------------------------------------------------------------------------
/// \ru Выделить память указанного размера под массив. \en Allocate memory of the given size for an array.  \~ \ingroup Base_Tools
// ---
inline void * AllocateArray( size_t size, const char * )
{
  void * ptr = ::malloc( size );
  // \ru Дабы работал _msize: void *ptr = ::operator new [] ( size ); \en For working of _msize: void *ptr = ::operator new [] ( size ); 

  if ( ptr ) {
#ifdef __MEMSET_USED_FREE_HEAP_HEAR__
    ::memset( ptr, C3D_INVALID_UINT32_1, size );
#endif // __MEMSET_USED_FREE_HEAP_HEAR__
  }
  return ptr;
}

//------------------------------------------------------------------------------
/// \ru Освободить память указанного размера. \en Free memory of the given size.  \~ \ingroup Base_Tools
// ---
inline void Free( void * ptr, size_t size, const char * )
{
  if ( ptr ) {
    ::CheckPointerAndValue( ptr, size );

#ifdef __MEMSET_USED_FREE_HEAP_HEAR__
    size_t ptr_size = ::_msize( ptr );
    PRECONDITION( ptr_size > 0 && ptr_size < SYS_MAX_T );
    if ( ptr_size ) {
      PRECONDITION( size <= ptr_size );
      ::memset( ptr, C3D_MEMORY_FILL_VALUE, ptr_size );
    }
#endif // __MEMSET_USED_FREE_HEAP_HEAR__

    ::free( ptr );
  }
}

//------------------------------------------------------------------------------
/// \ru Освободить память, выделенную под массив. \en Free the memory allocated for the array.  \~ \ingroup Base_Tools
// ---
// \ru ЯТ можно перегрузить в классах operator delete [] ( void *, size_t ) и передать \en ЯТ it is pertinent to overload the operator delete [] ( void *, size_t ) and pass 
// \ru в эту функцию size_t size, но это будет не размер массива, а размер Type, \en to this function size_t size, but this will be not the size of an array but the size of 'Type', 
// \ru массив которых распределялся. То есть эта информация здесь не нужна (делать \en an array of which was not distribute. I.e. this information is not needed here ( 
// \ru ::memset не нее НЕЛЬЗЯ!) \en it is forbidden to do ::memset here) 
// ---
inline void FreeArray( void * ptr, const char * )
{
  if ( ptr ) {
   ::CheckPointerAndValue( ptr, 0/*size*/ );

#ifdef __MEMSET_USED_FREE_HEAP_HEAR__
    size_t size = ::_msize( ptr );
    PRECONDITION( size > 0 && size < SYS_MAX_T );
    ::CheckPointerAndValue( ptr, size );
    if ( size )
      ::memset( ptr, C3D_MEMORY_FILL_VALUE, size );
#endif // __MEMSET_USED_FREE_HEAP_HEAR__

    ::free( ptr );
  }
}
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_


#ifdef USE_REALLOC_IN_ARRAYS
//------------------------------------------------------------------------------
/// \ru Функция перезахватов памяти в массивах. \en Function of memory reallocation in arrays.  \~ \ingroup Base_Tools
// ---
#ifdef __MEMSET_USED_FREE_HEAP_HEAR__
inline void * ReallocArraySize( void * arr_parr, size_t newBytesCount, bool clear )
#else // __MEMSET_USED_FREE_HEAP_HEAR__
inline void * ReallocArraySize( void * arr_parr, size_t newBytesCount, bool )
#endif // __MEMSET_USED_FREE_HEAP_HEAR__
{
#ifdef __MEMSET_USED_FREE_HEAP_HEAR__
  if ( newBytesCount == 0 || clear ) {
    size_t arr_parr_size = arr_parr ? ::_msize( arr_parr ) : 0;
    PRECONDITION( arr_parr ? ( arr_parr_size > 0 && arr_parr_size < SYS_MAX_T ) : true );
    if ( arr_parr_size )
      ::memset( arr_parr, C3D_MEMORY_FILL_VALUE, arr_parr_size );
  }
#endif // __MEMSET_USED_FREE_HEAP_HEAR__

#if defined( _AFXDLL ) && defined( C3D_DEBUG )
  void * tmp_parr = _realloc_dbg( arr_parr, newBytesCount, _NORMAL_BLOCK, __FILE__, __LINE__ );
#else
  void * tmp_parr = ::realloc( arr_parr, newBytesCount );
#endif

  PRECONDITION( newBytesCount == 0 || tmp_parr != nullptr ); // \ru проверка на нехватку памяти в массивах \en check the memory deficit in arrays

#ifdef __MEMSET_USED_FREE_HEAP_HEAR__
  if ( clear ) {
    size_t tmp_parr_size = tmp_parr ? ::_msize( tmp_parr ) : 0;
    PRECONDITION( tmp_parr ? ( tmp_parr_size > 0 && tmp_parr_size < SYS_MAX_T ) : true );
    if ( tmp_parr_size )
      ::memset( tmp_parr, C3D_MEMORY_FILL_VALUE, tmp_parr_size );
  }
#endif // __MEMSET_USED_FREE_HEAP_HEAR__

  return tmp_parr;
}

//------------------------------------------------------------------------------
// \ru Использовать realloc для изменения размера массивов \en Use realloc to change arrays sizes 
// \ru (если не определено, то по-старому, через new и delete) \en (if it is not defined then use new and delete operators) 
//---

#define REALLOC_ARRAY_SIZE(p,s,c) ::ReallocArraySize((p),(s),(c))
#else // USE_REALLOC_IN_ARRAYS
#define REALLOC_ARRAY_SIZE(p,s,c) ::realloc((p),(s))

#endif // USE_REALLOC_IN_ARRAYS


#endif  // __MEMORY_DEBUG_H
