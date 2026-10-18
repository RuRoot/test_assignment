////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Подключение математических функций в LINUX и WINDOWS, а также для компилятора Intel.
         \en Connection of mathematical functions in LINUX and WINDOWS, and for Intel compiler. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MATHX_H
#define __MATHX_H

#include <system_cpp_standard.h>

#if !defined ( C3D_WINDOWS )  //_MSC_VER

  // Only for LINUX 
  #if defined ( __INTEL_COMPILER ) 
    // Only INTEL COMPILER for LINUX
    #include <mathimf.h>
    #define _CMATH_
  #else // __INTEL_COMPILER
    // Other COMPILER for LINUX
    #include <math.h>
  #endif //__INTEL_COMPILER
  #include <stdint.h>
  #include <cstddef>

  #define _hypot hypot

  #ifndef __SIZEOF_WCHAR_T__ 
    #define __SIZEOF_WCHAR_T__  4
  #endif

#else // C3D_WINDOWS

  // Only for WINDOWS
  #ifndef __SIZEOF_WCHAR_T__ 
    #define __SIZEOF_WCHAR_T__  2
  #endif

  #if defined ( __INTEL_COMPILER ) 
    // Only INTEL COMPILER for WINDOWS 
    #include <mathimf.h>

    #define _hypot hypot
    #define _CMATH_
  #else // __INTEL_COMPILER
    // Only MS COMPILER for WINDOWS  
    #ifndef _USE_MATH_DEFINES 
      #define _USE_MATH_DEFINES  // \ru для подключения #define математических констант из math.h \en for connection #define of mathematical constants from math.h 
    #endif
    #include <math.h>

//-----------------------------------------------------------------------------
namespace c3d {
    # define _HYPOT_EPSILON   1.0E-20
    template <class T>
    T _hypot(T x, T y)
    {
       // Normalize x and y, so that both are positive and x >= y:
       x = fabs(x);
       y = fabs(y);

       if ( y > x ) {
         if(y * _HYPOT_EPSILON >= x)
            return y;

         T rat = x / y;
         return y * sqrt(1 + rat*rat);
       }
       else {
         if(x * _HYPOT_EPSILON >= y)
            return x;

         T rat = y / x;
         return x * sqrt(1 + rat*rat);
       }
    } // template <class T> T _hypot(T x, T y)
}

//-----------------------------------------------------------------------------
// ---
#define _hypot c3d::_hypot
  #endif //__INTEL_COMPILER


#endif //C3D_WINDOWS   

//------------------------------------------------------------------------------


#define CONCAT_TKNS(A, B, C, D)   A B C D        // Concatenate tokens without expanding macro (if invoked from macro, arguments are expanded).
#define CONCAT_TOKENS(A, B, C, D) CONCAT_TKNS(A, B, C, D) // Concatenate tokens with expanding macro.

#define DEPR_MSG          "This is a deprecated API that will be removed in version 2026!" // Message about a deprecated API.
#define DEPR_MSG_EX(msg)  CONCAT_TOKENS(DEPR_MSG, " Use ", #msg, " instead it.")           // Message about a deprecated API indicating a replacement API.


//------------------------------------------------------------------------------
// \ru Макросы для объявления устаревшего интерфейса.
// \en Macros for deprecated API declaration.
// ---
#ifdef C3D_STANDARD_CXX_14
#define DEPRECATE_DECLARE               [[deprecated( DEPR_MSG )]]
#define DEPRECATE_DECLARE_REPLACE(func) [[deprecated( DEPR_MSG_EX(func) )]]
#else
#if !defined ( C3D_WINDOWS )  //_MSC_VER
#define  DEPRECATE_DECLARE __attribute__((deprecated))
#define  DEPRECATE_DECLARE_REPLACE(func) DEPRECATE_DECLARE
#else
#define DEPRECATE_DECLARE               __declspec( deprecated( DEPR_MSG ) )
#define DEPRECATE_DECLARE_REPLACE(func) __declspec( deprecated( DEPR_MSG_EX(func) ) )
#endif //_MSC_VER
#endif // C3D_STANDARD_CXX_14


namespace c3d {
  //------------------------------------------------------------------------------
  /** \brief \ru Безопасная функция acos.
             \en Secure acos function. \~
    \details \ru Безопасная функция acos. Загоняет параметр x в диапазон [-1, +1]. \n
             \en Secure acos function. The function puts the parameter x into the range [-1, +1]. \n \~
    \ingroup Base_Tools
  */
  // ---
  inline double ACos( double x )
  {
    if ( x > 1.0 )
      x = 1.0;
    if ( x < -1.0 )
      x = -1.0;
    return ::acos( x );
  }


  //------------------------------------------------------------------------------
  /** \brief \ru Функция проверки является ли заданное число конечным.
             \en Function to check whether the given number is finite. \~
    \details \ru Функция проверки является ли заданное число конечным. \n
                 Если число имеет значение MB_INFINITY или MB_QNAN, то оно не является конечным. \n
             \en Function to check whether the given number is finite. \n
                 If given number is MB_INFINITY or MB_QNAN then it is not finite. \n \~
    \ingroup Base_Tools
  */
  inline bool IsFinite( double val )
  {
    return isfinite( val );
  }
}

//-----------------------------------------------------------------------------
// \ru Безопасная функция acos. \en Secure acos function.
// ---
#define _acos c3d::ACos

#endif // __MATHX_H
