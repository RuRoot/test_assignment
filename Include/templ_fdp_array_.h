////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Стандартные реализации функций удаления элементов для FDPArray'а.
         \en Standard implementations of FDPArray elements deletion functions. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_FDP_ARRAY__H
#define __TEMPL_FDP_ARRAY__H

#include <templ_fdp_array.h>


//------------------------------------------------------------------------------
// \ru функция удаления одного элемента \en function of one element deletion 
// ---
template <class Type>
inline bool FDPArray_Destroy( Type *el ) {
  delete el;
  return true;
}


//------------------------------------------------------------------------------
// \ru функция удаления всех элементов \en function of all elements deletion 
// ---
template <class Type>
void FDPArray_TotalDestroy( const Type **arr, size_t count ) {
  if ( arr ) {
    size_t i = 0;
    for( const Type** parr = arr; i < count; i++, parr++ ) {
      Type *del = (Type*)*parr;
      *parr = 0;  // \ru Сначала обнулить ... \en Set to null at first... 
      delete del; // \ru ... потом удалить \en ... then delete 
    }
  }
}


//------------------------------------------------------------------------------
// \ru функция освобождения одного элемента (true означает, что el деструктурирован) \en function of one element release (returns true if 'el' has been destructured)  
// ---
template <class Type>
inline bool FDPArray_Release( Type *el ) {
  if ( el )
    return (el->Release() == 0);

  return true;
}


//------------------------------------------------------------------------------
// \ru функция освобождения всех элементов \en function of all elements release 
// ---
template <class Type>
void FDPArray_TotalRelease( const Type **arr, size_t count ) {
  if ( arr ) {
    for ( const Type** parr = arr, **last = arr + count; parr < last; ++parr ) 
    {
      Type *del = (Type*)*parr;
      *parr = 0;        // \ru Сначала обнулить ... \en Set to null at first... 
      if ( del )
        del->Release(); // \ru ... потом удалить \en ... then delete 
    }
  }
}


#endif // __TEMPL_FDP_ARRAY__H
