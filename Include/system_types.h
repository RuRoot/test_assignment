////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Базовые типы данных.
         \en Base types of data. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SYSTEM_TYPES_H
#define __SYSTEM_TYPES_H


#include <math_x.h> // \ru СМВ для компиляции ICC \en СМВ for compilation by ICC 
#include <system_cpp_standard.h>


/**
  \addtogroup Base_Tools   
  \{
*/

#if defined(_DEBUG) || !defined(NDEBUG)
  #define C3D_DEBUG
#endif

//------------------------------------------------------------------------------
// \ru Системные типы \en System types 
//---
#ifdef C3D_WINDOWS //_MSC_VER 

typedef   signed char      int8; ///< \ru int со знаком (1 байт). \en int with sign (1 byte).  \~ 
typedef unsigned char     uint8; ///< \ru int без знака (1 байт). \en int without sign (1 byte).  \~

typedef   signed short    int16; ///< \ru int со знаком (2 байта). \en int with sign (2 bytes).  \~
typedef unsigned short   uint16; ///< \ru int без знака (2 байта). \en int without sign (2 bytes).  \~

typedef   signed long     int32; ///< \ru int со знаком (4 байта). \en int with sign (4 bytes).  \~
typedef unsigned long    uint32; ///< \ru int без знака (4 байта). \en int without sign (4 bytes).  \~

typedef   signed __int64  int64; ///< \ru int со знаком (8 байта). \en int with sign (8 bytes).  \~
typedef unsigned __int64 uint64; ///< \ru int без знака (8 байта). \en int without sign (8 bytes).  \~

typedef unsigned int       uint; ///< \ru int без знака. \en int without sign.  \~ \ingroup Base_Tools

typedef unsigned long   VERSION; ///< \ru Версия. \en Version.  \~


#else // C3D_WINDOWS

#include <stdint.h>
#include <cstddef>

typedef int8_t             int8;
typedef uint8_t           uint8;

typedef int16_t           int16;
typedef uint16_t         uint16;

typedef int32_t           int32;
typedef uint32_t         uint32;

typedef int64_t           int64;
typedef uint64_t         uint64;

typedef unsigned int       uint;

typedef uint32          VERSION; ///< \ru Версия. \en Version.  \~


#endif // _MSC_VER

/** \} */ //addtogroup Base_Tools   

#include <memory>
#include <float.h>
//------------------------------------------------------------------------------
// \ru Системные лимиты \en System limits 
//---
#include <limits>  // \ru для использования std_min() и std_max() \en for using in std_min() and std_max() 


typedef ptrdiff_t       refcount_t; // Возвращаемый тип счетчика ссылок. / The return type of the reference count.

//#if defined(NOMINMAX) || !defined (_MSC_VER)
//
//const uint      SYS_MAX_UINT   = std::numeric_limits<uint>::max();
//const size_t    SYS_MAX_T      = std::numeric_limits<size_t>::max();
//const ptrdiff_t SYS_MAX_ST     = std::numeric_limits<ptrdiff_t>::max();
//const ptrdiff_t SYS_MIN_ST     = std::numeric_limits<ptrdiff_t>::min();
//
//const uint8     SYS_MAX_UINT8  = std::numeric_limits<uint8>::max();
//const uint16    SYS_MAX_UINT16 = std::numeric_limits<uint16>::max();
//const uint32    SYS_MAX_UINT32 = std::numeric_limits<uint32>::max();
//const uint64    SYS_MAX_UINT64 = std::numeric_limits<uint64>::max();
//
//const int16    SYS_MAX_INT16   = std::numeric_limits<int16>::max();
//const int32    SYS_MAX_INT32   = std::numeric_limits<int32>::max();
//const int64    SYS_MAX_INT64   = std::numeric_limits<int64>::max();
//
//const int16    SYS_MIN_INT16   = std::numeric_limits<int16>::min();
//const int32    SYS_MIN_INT32   = std::numeric_limits<int32>::min();
//const int64    SYS_MIN_INT64   = std::numeric_limits<int64>::min();
//
//#else // NOMINMAX 

#if defined(PLATFORM_64)
/// \ru Максимальное значение uint. \en Maximum value of uint.  \~ \ingroup Base_Tools
constexpr uint      SYS_MAX_UINT   = 0xffffffff;         // ((uint)-1)      //-V112
/// \ru Максимальное значение size_t. \en Maximum value of size_t.  \~ \ingroup Base_Tools
constexpr size_t    SYS_MAX_T      = 0xffffffffffffffff; // ((size_t)-1)    //-V112
/// \ru Максимальное значение ptrdiff_t. \en Maximum value of ptrdiff_t.  \~ \ingroup Base_Tools
constexpr ptrdiff_t SYS_MAX_ST     = 0x7fffffffffffffff;                    //-V112
/// \ru Минимальное значение ptrdiff_t. \en Minimum value of ptrdiff_t.  \~ \ingroup Base_Tools
constexpr ptrdiff_t SYS_MIN_ST     = 0x8000000000000000;                    //-V112
#else  // PLATFORM_64 
/// \ru Максимальное значение uint. \en Maximum value of uint.  \~ \ingroup Base_Tools
constexpr uint      SYS_MAX_UINT   = 0xffffffff; // ((uint)-1)              //-V112
/// \ru Максимальное значение size_t. \en Maximum value of size_t.  \~ \ingroup Base_Tools
constexpr size_t    SYS_MAX_T      = 0xffffffff; // ((size_t)-1)            //-V112
/// \ru Максимальное значение ptrdiff_t. \en Maximum value of ptrdiff_t.  \~ \ingroup Base_Tools
constexpr ptrdiff_t SYS_MAX_ST     = 0x7fffffff;                            //-V112
/// \ru Минимальное значение ptrdiff_t. \en Minimum value of ptrdiff_t.  \~ \ingroup Base_Tools
constexpr ptrdiff_t SYS_MIN_ST     = 0x80000000;                            //-V112
#endif // PLATFORM_64 

/// \ru Максимальное значение uint8. \en Maximum value of uint8.  \~ \ingroup Base_Tools
constexpr uint8     SYS_MAX_UINT8  = 0xFF;                                  //-V112
/// \ru Максимальное значение uint16. \en Maximum value of uint16.  \~ \ingroup Base_Tools
constexpr uint16    SYS_MAX_UINT16 = 0xFFFF;                                //-V112
/// \ru Максимальное значение uint32. \en Maximum value of uint32.  \~ \ingroup Base_Tools
constexpr uint32    SYS_MAX_UINT32 = 0xFFFFFFFF;                            //-V112
/// \ru Максимальное значение uint64. \en Maximum value of uint64.  \~ \ingroup Base_Tools
constexpr uint64    SYS_MAX_UINT64 = 0xFFFFFFFFFFFFFFFF;                    //-V112

/// \ru Максимальное значение int16. \en Maximum value of int16.  \~ \ingroup Base_Tools
constexpr int16     SYS_MAX_INT16 = 0x7FFF;                                 //-V112
/// \ru Максимальное значение int32. \en Maximum value of int32.  \~ \ingroup Base_Tools
constexpr int32     SYS_MAX_INT32 = 0x7FFFFFFF;                             //-V112
/// \ru Максимальное значение int64. \en Maximum value of int64.  \~ \ingroup Base_Tools
constexpr int64     SYS_MAX_INT64 = 0x7FFFFFFFFFFFFFFF;                     //-V112

/// \ru Минимальное значение int16. \en Minimum value of int16.  \~ \ingroup Base_Tools
constexpr int16     SYS_MIN_INT16 = static_cast<int16>( static_cast<uint16>(0x8000) );
/// \ru Минимальное значение int32. \en Minimum value of int32.  \~ \ingroup Base_Tools
constexpr int32     SYS_MIN_INT32 = static_cast<int32>( static_cast<uint32>(0x80000000) );
/// \ru Минимальное значение int64. \en Minimum value of int64.  \~ \ingroup Base_Tools
constexpr int64     SYS_MIN_INT64 = static_cast<int64>( static_cast<uint64>(0x8000000000000000) );

//#endif // NOMINMAX


//------------------------------------------------------------------------------
// Integer byte, word and long word manipulation
//---

/// \ru Создать uint16 на основе двух uint8. \en Create uint16 by two uint8.  \~ \ingroup Base_Tools
inline uint16 MkUint16( uint8  lo, uint8  hi ) { return uint16(lo | (uint16(hi) << 8)); }                   //-V112
/// \ru Создать uint32 на основе двух uint16. \en Create uint32 by two uint16.  \~ \ingroup Base_Tools
inline uint32 MkUint32( uint16 lo, uint16 hi ) { return        lo | (uint32(hi) << 16); }                   //-V112
/// \ru Создать uint64 на основе двух uint32. \en Create uint64 by two uint32.  \~ \ingroup Base_Tools
inline uint64 MkUint64( uint32 lo, uint32 hi ) { return uint64(static_cast<uint64>(lo) | (uint64(hi) << 32)); } //OV_x64 //-V112

/// \ru Выделить младшее слово uint32 из uint64. \en Get lower uint32 word from uint64.  \~ \ingroup Base_Tools
inline uint32 LoUint32( uint64 u64 ) { return uint32(u64); }       //OV_x64
/// \ru Выделить младшее слово int32 из int64. \en Get lower int32 word from int64.  \~ \ingroup Base_Tools
inline  int32 LoInt32 ( uint64 u64 ) { return  int32(u64); }       //OV_x64
/// \ru Выделить старшее слово uint32 из uint64. \en Get upper uint32 word from uint64.  \~ \ingroup Base_Tools
inline uint32 HiUint32( uint64 u64 ) { return uint32(u64 >> 32); } //OV_x64 //-V112
/// \ru Выделить старшее слово int32 из int64. \en Get upper int32 word from int64.  \~ \ingroup Base_Tools
inline  int32 HiInt32 ( uint64 u64 ) { return  int32(u64 >> 32); } //OV_x64 //-V112

/// \ru Выделить младшее слово uint16 из uint32. \en Get lower int16 word from int32.  \~ \ingroup Base_Tools
inline uint16 LoUint16( uint32 u32 ) { return uint16(u32); }
/// \ru Выделить младшее слово int16 из uint32. \en Get lower int16 word from uint32.  \~ \ingroup Base_Tools
inline  int16 LoInt16 ( uint32 u32 ) { return  int16(u32); }
/// \ru Выделить старшее слово uint16 из uint32. \en Get upper uint16 word from uint32.  \~ \ingroup Base_Tools
inline uint16 HiUint16( uint32 u32 ) { return uint16(u32 >> 16); }          //-V112
/// \ru Выделить старшее слово int16 из uint32. \en Get upper int16 word from uint32.  \~ \ingroup Base_Tools
inline  int16 HiInt16 ( uint32 u32 ) { return  int16(u32 >> 16); }          //-V112

/// \ru Выделить младшее слово uint8 из uint16. \en Get lower uint8 word from uint16.  \~ \ingroup Base_Tools
inline  uint8 LoUint8 ( uint16 u16 ) { return uint8(u16); }
/// \ru Выделить младшее слово int8 из uint16. \en Get lower int8 word from uint16.  \~ \ingroup Base_Tools
inline   int8 LoInt8  ( uint16 u16 ) { return  int8(u16); }
/// \ru Выделить старшее слово uint8 из uint16. \en Get upper uint8 word from uint16.  \~ \ingroup Base_Tools
inline  uint8 HiUint8 ( uint16 u16 ) { return uint8(u16 >> 8); }            //-V112
/// \ru Выделить младшее слово int8 из uint16. \en Get upper int8 word from uint16.  \~ \ingroup Base_Tools
inline   int8 HiInt8  ( uint16 u16 ) { return  int8(u16 >> 8); }            //-V112


//------------------------------------------------------------------------------
/// \ru Неопределенный размер. \en Undefined size.  \~ \ingroup Base_Tools
//---
constexpr size_t NSIZE = SYS_MAX_T; //OV_x64 (size_t)-1;

//------------------------------------------------------------------------------
/// \ru Неопределенная позиция (для 32 битных данных). \en Undefined position (for 32-bit data).  \~ \ingroup Base_Tools
//---
constexpr uint NPOS32 = static_cast<uint>(SYS_MAX_UINT32); // \ru КВН x64 -1 для работы с таблицами \en КВН x64 -1 for dealing with tables


//------------------------------------------------------------------------------
/** \brief \ru Размер указателя в байтах.
           \en Size of pointer in bytes. \~
  \details \ru Размер указателя в байтах. \n
           \en Size of pointer in bytes. \n \~
  \ingroup Base_Tools
*/
//---
constexpr size_t SIZE_OF_POINTER = sizeof(char *);


//------------------------------------------------------------------------------
/** \brief \ru Модуль числа.
           \en Absolute value. \~
  \details \ru Модуль числа. \n
           \en Absolute value. \n \~
  \ingroup Base_Tools
*/
//---
template <class SignedType>
SignedType abs_t( const SignedType x ) { return ((x >= 0) ? x : -x); }


//------------------------------------------------------------------------------
// \ru Подавить предупреждения \en Suppress warnings 
//---
#if defined (C3D_WINDOWS ) && !defined(ALL_WARNINGS)  //_MSC_VER // Set warnings level

// http://support.microsoft.com/kb/134980/ru
//#pragma warning(disable: 4275)  //AP non dll-interface class '1' used as base for dll-interface class '2' (deriving exported from non-exported)
#pragma warning(disable: 4251)  //AP class '1' needs to have dll-interface to be used by clients of class '2' (using non-exported in exported)
#pragma warning(disable: 4505)  //'SomeFunctionName': unreferenced local function has been removed 

// have to be disabled
//#pragma warning(disable: 4018)  // W3: comparison : signed/unsigned mismatch
//#pragma warning(disable: 4201)  // W4: nonstandard extension used : nameless struct/union

#pragma warning(disable: 4245)  // W4: const conversion, signed/unsigned mismatch
//#pragma warning(disable: 4365)  // W4: value conversion, signed/unsigned mismatch

//#pragma warning(disable: 4516)  // W4: access-declarations are deprecated; member using-declarations provide a better alternative
//#pragma warning(disable: 4786)  // \ru Иначе map сыпет такое!!! _ANN_PARAM_MAP_ Чтобы не ругалась на урезание отладочной информации \en Otherwise map output such a things!!! _ANN_PARAM_MAP_ For not swearing on reduction of debugging information 

// The keyword -D_CRT_SECURE_NO_WARNINGS added. Pragma below were disable deprecated warnings.
//#pragma warning(disable: 4996)  // This function or variable may be unsafe. Consider using strcpy_s instead.

#endif // !ALL_WARNINGS


#endif // __SYSTEM_TYPES_H
