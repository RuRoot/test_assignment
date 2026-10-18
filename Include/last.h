////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Контроль утечек памяти.
         \en Control of memory leaks. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef _LAST_H_
#define _LAST_H_

#include "math_cfg.h" // There are modules without defined paths to C3D <math_cfg.h>

#ifdef ENABLE_VLD
  #include <wctype.h> // \ru KVA V14 16.1.2012 Для компиляции КОМПАС \en KVA V14 16.1.2012 To compile the COMPAS 
  #include <vld.h>
#else
  #ifdef C3D_WINDOWS //_MSC_VER
    #ifdef __AFX_H__
      #ifdef _DEBUG
        #define new DEBUG_NEW
        #undef THIS_FILE
        #define THIS_FILE __FILE__
      #endif
    #else // __AFX_H__
      #define _CRTDBG_MAP_ALLOC
      #include <stdlib.h>
      #include <crtdbg.h>

      #if defined(_DEBUG) && !defined(__DEBUG_MEMORY_ALLOCATE_FREE_)
        #define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
      #endif
    #endif // __AFX_H__
  #endif // C3D_WINDOWS
#endif // ENABLE_VLD

#endif // _LAST_H_
