////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Макросы стандартов C и C++.
         \en C\C++ standards. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SYSTEM_CPP_STANDARD_H
#define __SYSTEM_CPP_STANDARD_H

#include <cstddef>
#include <math_cfg.h>

// Clang version (https://sourceforge.net/p/predef/wiki/Compilers/)
// LLVM versions of Apple Clang: https://gist.github.com/yamaya/2924292 + https://en.wikipedia.org/wiki/Xcode#Toolchain_versions
#if defined(__clang__)  // Clang compiler
  #define C3D_CLANG_COMPILER
  #if defined(__clang_major__)
    #if defined(__clang_minor__)
      #define C3D_CLANG_VERSION (__clang_major__ * 100 + __clang_minor__)
    #else
      #define C3D_CLANG_VERSION (__clang_major__ * 100)
    #endif
  #else
    #define C3D_CLANG_VERSION 0
  #endif
#else
  #define C3D_CLANG_VERSION 0
#endif  // Clang compiler

// Intel compiler version (https://sourceforge.net/p/predef/wiki/Compilers/)
#if defined(__INTEL_COMPILER)
  #define C3D_ICC_VERSION __INTEL_COMPILER
#else
  #define C3D_ICC_VERSION 0
#endif

// GCC version (https://sourceforge.net/p/predef/wiki/Compilers/)
#if !defined(C3D_WINDOWS) && C3D_CLANG_VERSION == 0 && C3D_ICC_VERSION == 0 // GCC compiler
  #if defined(__GNUC__)
    #if defined(__GNUC_MINOR__)
      #if defined(__GNUC_PATCHLEVEL__)
        #define C3D_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
      #else
        #define C3D_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100)
      #endif
    #else
      #define C3D_GCC_VERSION (__GNUC__ * 10000)
    #endif
  #else
    #define C3D_GCC_VERSION 0
  #endif
#else
  #define C3D_GCC_VERSION 0
#endif  // GCC compiler

// GNU glibc version.
#if defined(C3D_LINUX)  // glibc version.
  #include <features.h>
  #if defined(__GLIBC__) && defined(__GLIBC_MINOR__)
    #define C3D_GLIBC_VERSION (__GLIBC__ * 1000 + __GLIBC_MINOR__)
  #else
    #define C3D_GLIBC_VERSION 0
  #endif
#else
  #define C3D_GLIBC_VERSION 0
#endif  // glibc version.

// Available C++ standard to use.
// https://en.cppreference.com/w/cpp/compiler_support
// https://clang.llvm.org/cxx_status.html
// https://gcc.gnu.org/projects/cxx-status.html#cxx11
// https://software.intel.com/en-us/articles/c0x-features-supported-by-intel-c-compiler
// https://software.intel.com/en-us/articles/c14-features-supported-by-intel-c-compiler
// https://software.intel.com/en-us/articles/c17-features-supported-by-intel-c-compiler
#if (defined(_MSC_VER) && (_MSC_VER >= 1929)) || (C3D_GCC_VERSION >= 120000) || \
        (!defined(__apple_build_version__) && C3D_CLANG_VERSION >= 1600) || \
        (defined(__apple_build_version__) && __apple_build_version__ >= 1400)
  #define C3D_STANDARD_CXX 2020
#elif (defined(_MSC_VER) && (_MSC_VER >= 1914)) || (C3D_GCC_VERSION >= 70000) || (C3D_ICC_VERSION >= 1910) || \
        (!defined(__apple_build_version__) && C3D_CLANG_VERSION >= 500) || \
        (defined(__apple_build_version__) && __apple_build_version__ >= 901)
  #define C3D_STANDARD_CXX 2017
#elif (defined(_MSC_VER) && (_MSC_VER >= 1910)) || (C3D_GCC_VERSION >= 50000) || (C3D_ICC_VERSION >= 1700) || \
        (!defined(__apple_build_version__) && C3D_CLANG_VERSION >= 304) || \
        (defined(__apple_build_version__) && __apple_build_version__ >= 501)
  #define C3D_STANDARD_CXX 2014
#elif (defined(_MSC_VER) && (_MSC_VER >= 1900)) || (C3D_GCC_VERSION >= 40801) || (C3D_ICC_VERSION >= 1500) || \
        (!defined(__apple_build_version__) && C3D_CLANG_VERSION >= 303) || \
        (defined(__apple_build_version__) && __apple_build_version__ >= 500)
 #define C3D_STANDARD_CXX 2011
#else
  #define C3D_STANDARD_CXX 2003
#endif  // C++ version

#if !defined(C3D_STANDARD_CXX_LIMIT)
  #define C3D_STANDARD_CXX_LIMIT 99999
#endif

#if C3D_STANDARD_CXX_LIMIT >= 2014 && C3D_STANDARD_CXX >= 2014
  #define C3D_STANDARD_CXX_14
#endif

#if C3D_STANDARD_CXX_LIMIT >= 2017 && C3D_STANDARD_CXX >= 2017
  #define C3D_STANDARD_CXX_17
#endif

#if C3D_STANDARD_CXX_LIMIT >= 2020 && C3D_STANDARD_CXX >= 2020
  #define C3D_STANDARD_CXX_20
#endif

// Available C standard to use.
// https://en.wikipedia.org/wiki/C99 + https://gcc.gnu.org/c99status.html
// https://gcc.gnu.org/wiki/C11Status + https://en.wikipedia.org/wiki/C11_(C_standard_revision) + https://releases.llvm.org/3.6.0/tools/clang/docs/UsersManual.html#c-language-features
// https://en.wikipedia.org/wiki/C18_(C_standard_revision)
// https://software.intel.com/en-us/articles/c99-support-in-intel-c-compiler + https://software.intel.com/en-us/articles/c11-support-in-intel-c-compiler
#if (C3D_GCC_VERSION >= 80100) || (!defined(__apple_build_version__) && C3D_CLANG_VERSION >= 700) || \
        (defined(__apple_build_version__) && __apple_build_version__ >= 1000) || (C3D_ICC_VERSION >= 1910)
  #define C3D_STANDARD_C 2018
#elif (C3D_GCC_VERSION >= 40900) || (!defined(__apple_build_version__) && C3D_CLANG_VERSION >= 306) || \
        (defined(__apple_build_version__) && __apple_build_version__ >= 601) || (C3D_ICC_VERSION >= 1800)
  #define C3D_STANDARD_C 2011
#elif (defined(_MSC_VER) && (_MSC_VER >= 1900)) || (C3D_GCC_VERSION >= 40500) || \
        (!defined(__apple_build_version__) && C3D_CLANG_VERSION >= 301) || \
        (defined(__apple_build_version__) && __apple_build_version__ >= 301) || (C3D_ICC_VERSION >= 1200)
  #define C3D_STANDARD_C 1999
#else
  #define C3D_STANDARD_C 1989
#endif

#if C3D_STANDARD_C >= 1999
  #define C3D_STANDARD_C99
#endif

#if C3D_STANDARD_C >= 2011
  #define C3D_STANDARD_C11
#endif

#if C3D_STANDARD_C >= 2018
  #define C3D_STANDARD_C18
#endif


//////////////////////////////////////////////////////////////////////////////////////////
// C++ standard dependent syntax.
//////////////////////////////////////////////////////////////////////////////////////////

#endif // __SYSTEM_CPP_STANDARD_H
