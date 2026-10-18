////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Одномерный массив указателей.
         \en One-dimensional array of pointers. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_FDP_ARRAY_H
#define __TEMPL_FDP_ARRAY_H


#include <stdlib.h>
#include <system_types.h>
#include <templ_delete_define.h>
#include <templ_rp_array.h>
#include <io_define.h>



FORVARD_DECL_TEMPLATE_TYPENAME( class FDPArray );
FORVARD_DECL_TEMPLATE_TYPENAME( void  destroy_array  ( FDPArray<Type> & ) );
FORVARD_DECL_TEMPLATE_TYPENAME( bool  set_Farray_size( FDPArray<Type> &, size_t newSize, bool clear ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader & CALL_DECLARATION operator >> ( reader & in, FDPArray<Type> & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer & CALL_DECLARATION operator << ( writer & out, const FDPArray<Type> & ref ) );


//------------------------------------------------------------------------------
/** \brief \ru Одномерный массив указателей.
           \en One-dimensional array of pointers. \~
  \details \ru Одномерный массив указателей на объекты. \n
    Можно использовать для классов с указателями. Удаление объектов производится через функцию удаления. \n
    Внимание! Удаление объектов в больших массивах выполняется медленнее, чем в PArray. \n
    Чтобы избежать потери времени, надо использовать Clear( TotalDestroyFunc ).
           \en One-dimensional array of pointers to objects. \n
    It may be used for the classes with pointers. Deletion of objects is performed by the function of deletion. \n
    Attention! Destructing of large arrays is performed slower than in PArray. \n
    In order to avoid of losing time Clear( TotalDestroyFunc ) should be used. \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class FDPArray : public RPArray<Type> {
public :
  typedef void (*TotalDestroyFunc)( const Type**, size_t ); // \ru функция удаления ВСЕГО массива, работает значительно быстрее \en the function of deletion of THE WHOLE array, it works significantly faster. 
  typedef bool (*DestroyFunc)( Type * ); // \ru функция удаления вместо флага fDestroy \en the function of deletion instead of the "tDestroy" flag 

protected :
  DestroyFunc   fDestroy; // \ru функция удаления вместо флага owns \en the function of deletion instead of the flag 'owns' 
  
  Type  *       nowDeletedElem;  // \ru Временно \en Temporarily 

public :
  /// \ru Конструктор. \en Constructor. 
  FDPArray()
    : RPArray<Type>()
    , fDestroy( nullptr )
    , nowDeletedElem(nullptr)
  {}
  /// \ru Конструктор. \en Constructor. 
  FDPArray( size_t i_upper, uint16 i_delta, DestroyFunc fd )
    : RPArray<Type>( i_upper, i_delta)
    , fDestroy( fd )
    , nowDeletedElem(nullptr)
  {}
  /// \ru Деструктор. \en Destructor. 
  virtual ~FDPArray();

  /// \ru Установлена ли функция удаления элементов? \en Whether a function of deletion of elements is set? 
  bool        OwnsElem() const { return !!fDestroy; }
  /// \ru Установить функцию удаления элементов. \en Set a function of deletion of elements. 
  void        OwnsElem( DestroyFunc fd ) { fDestroy = fd; }

  /// \ru Функции, выделяющие потенциально большие участки памяти, возвращают результат операции (успех/ошибка).
  /// \en Functions that allocate potentially large memory, return an operation result (success/error). 
  bool        SetSize ( size_t newSize, bool clear ); ///< \ru Указать новый размер массива. \en Set the new size of an array. 

  void        Flush( DelType = defDelete ); ///< \ru Удалить все элементы. \en Delete all elements. 
  void        Clear( TotalDestroyFunc );    ///< \ru Удалить все элементы. \en Delete all elements. 

          Type * RemoveObj( Type * delObject, DelType del = defDelete ); ///< \ru Удалить элемент из массива по указателю. \en Delete an element from array by the pointer. 
  virtual Type * RemoveInd( size_t delIndex,  DelType del = defDelete ); ///< \ru Удалить элемент из массива по индексу. \en Delete an element from array by the index. 

  Type *      DestroyInd( size_t delIndex, DestroyFunc ); ///< \ru Удалить элемент из массива. \en Delete an element from array. 
  Type *      DestroyObj( Type *delObject, DestroyFunc ); ///< \ru Удалить элемент из массива. \en Delete an element from array. 

public: // \ru унификация с вектором STL \en unification with STL vector 
  virtual void  clear() { Flush(); }                  ///< \ru Обнулить количество элементов. \en Set the number of elements to null. 

private:
  FDPArray( const FDPArray<Type> & );                     // \ru запрещено !!! \en forbidden !!! 
  FDPArray<Type> & operator = ( const FDPArray<Type> & ); // \ru запрещено !!! \en forbidden !!! 

  TEMPLATE_FRIEND void  destroy_array   TEMPLATE_SUFFIX ( FDPArray<Type> & );
  TEMPLATE_FRIEND bool  set_Farray_size TEMPLATE_SUFFIX ( FDPArray<Type> &, size_t newSize, bool clear );

  TEMPLATE_FRIEND reader & CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader & in, FDPArray<Type> & ref );
  TEMPLATE_FRIEND writer & CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer & out, const FDPArray<Type> & ref );
  // Intel Compiler 12 // KNOWN_OBJECTS_RW_PTR_OPERATORS( FDPArray<Type> )

#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
public:
  void * operator new    ( size_t );
  void   operator delete ( void *, size_t );
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_

public:
  FDPArray( FDPArray<Type> && );                     ///< \ru Конструктор перемещения массива. \en Constructor of an array moving.
  FDPArray<Type> & operator = ( FDPArray<Type> && ); ///< \ru Оператор перемещения массива. \en Operator of an array moving.
};


#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
//------------------------------------------------------------------------------
// \ru Перегрузка оператора new. \en Overloading of the "new" operator. 
// ---
template <class Type>
inline void * FDPArray<Type>::operator new( size_t size ) {
  return ::Allocate( size, typeid(FDPArray<Type>).name() );
}

//------------------------------------------------------------------------------
// \ru Перегрузка оператора delete. \en Overloading of the "delete" operator. 
// ---
template <class Type>
inline void FDPArray<Type>::operator delete( void * ptr, size_t size ) {
  ::Free( ptr, size, typeid(FDPArray<Type>).name() );
}
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_

//------------------------------------------------------------------------------
// \ru Конструктор перемещения массива. \en Constructor of an array moving.
// ---
template <class Type>
FDPArray<Type>::FDPArray( FDPArray<Type> && _Right )
  : RPArray<Type>  (std::move(_Right) )
  , fDestroy      ( std::move(_Right.fDestroy) )
  , nowDeletedElem( std::move(_Right.nowDeletedElem) )
{
  _Right.fDestroy       = nullptr;
  _Right.nowDeletedElem = nullptr;
}

//------------------------------------------------------------------------------
// \ru Оператор перемещения массива. \en Operator of an array moving.
// ---
template <class Type>
FDPArray<Type> & FDPArray<Type>::operator = ( FDPArray<Type> && _Right )
{
  if ( this != &_Right )
  {
    destroy_array( *this );
    std::swap ( fDestroy, _Right.fDestroy );
    std::swap ( nowDeletedElem, _Right.nowDeletedElem );
    RPArray<Type>::operator = ( std::move(_Right) );
  }
  return (*this);
}

//------------------------------------------------------------------------------
// \ru деструктор массива \en destructor of array 
// ---
template <class Type>
inline FDPArray<Type>::~FDPArray() {
  PRECONDITION( nowDeletedElem  == nullptr );

  destroy_array( *this );
}


//------------------------------------------------------------------------------
// \ru обнулить количество элементов \en set the number of elements to null. 
// ---
template <class Type>
inline void FDPArray<Type>::Flush( DelType del ) {
  PRECONDITION( nowDeletedElem  == nullptr );
  
  if ( del==Delete || (del==defDelete && fDestroy) )
    destroy_array( *this );
  else
    RPArray<Type>::count = 0;
}


//------------------------------------------------------------------------------
// \ru обнулить количество элементов \en set the number of elements to null. 
// ---
template <class Type>
inline void FDPArray<Type>::Clear( typename FDPArray<Type>::TotalDestroyFunc fd ) {
  PRECONDITION( nowDeletedElem  == nullptr );

  size_t oldCount = RPArray<Type>::count;
  RPArray<Type>::count = 0;  // \ru сначала приведем в порядок массив ... \en put an array in order at first ... 

  if ( fd )
    (*fd)( RPArray<Type>::GetAddr()/*parr*/, oldCount ); // \ru ...а потом будем удалять \en ... and then delete 
}


//------------------------------------------------------------------------------
// \ru указать новый размер массива \en set the new size of an array. 
// \ru если clear = true, то массив очистится !!! \en if 'clear' is true than the array will be cleared !!! 
// ---
template <class Type>
inline bool FDPArray<Type>::SetSize( size_t newSize, bool clear ) {
  return set_Farray_size( *this, newSize, clear );
}


//------------------------------------------------------------------------------
// \ru удалить элемент из массива \en delete an element from array 
// ---
template <class Type>
inline Type* FDPArray<Type>::RemoveInd( size_t delIndex, DelType del ) {
  PRECONDITION( delIndex < RPArray<Type>::count );

  Type * r = 0;
  if ( delIndex < RPArray<Type>::count ) {
    const Type ** d = RPArray<Type>::GetAddr() + delIndex;

    r = const_cast<Type *>(*d);

    // \ru сначала приведем в порядок массив ... \en put an array in order at first ... 
    memmove( d, d + 1, (RPArray<Type>::count - delIndex - 1) * SIZE_OF_POINTER );
    RPArray<Type>::count--;

    // \ru ...а потом будем удалять \en ... and then delete 
    if ( fDestroy && ( del == Delete || del == defDelete ) ) {
      PRECONDITION( !r || nowDeletedElem != r );
      nowDeletedElem = r;

      ( *fDestroy )( r );
      r = 0;

      nowDeletedElem = 0;
    }
  }

  return r;
}


//------------------------------------------------------------------------------
// \ru удалить элемент из массива \en delete an element from array 
// ---
template <class Type>
inline Type * FDPArray<Type>::RemoveObj( Type * delObject, DelType del )
{
  PRECONDITION( nowDeletedElem  == nullptr );
  size_t i = find_in_array( *this, delObject );
  return (i != SYS_MAX_T) ? RemoveInd(i, del) : nullptr;
}


//------------------------------------------------------------------------------
// \ru удалить элемент из массива \en delete an element from array 
// ---
template <class Type>
inline Type* FDPArray<Type>::DestroyInd( size_t delIndex, typename FDPArray<Type>::DestroyFunc fd ) {
  PRECONDITION( delIndex < RPArray<Type>::count );

  Type * r = 0;
  if ( delIndex < RPArray<Type>::count ) {
    const Type ** d = RPArray<Type>::GetAddr() + delIndex;

    r = const_cast<Type *>(*d);

    // \ru сначала приведем в порядок массив ... \en put an array in order at first ... 
    memmove( d, d + 1, (RPArray<Type>::count - delIndex - 1) * SIZE_OF_POINTER );
    RPArray<Type>::count--;

    // \ru ...а потом будем удалять \en ... and then delete 
    if ( fd ) {
      PRECONDITION( !r || nowDeletedElem != r );  // \ru Временно, для отладки \en Temporarily, for debugging. 
      nowDeletedElem = r;

      ( *fd )( r );
      r = 0;

      nowDeletedElem = 0;
    }
  }

  return r;
}


//------------------------------------------------------------------------------
// \ru удалить элемент из массива \en delete an element from array 
// ---
template <class Type>
inline Type * FDPArray<Type>::DestroyObj( Type * delObject, typename FDPArray<Type>::DestroyFunc fd )
{
  PRECONDITION( nowDeletedElem  == nullptr );
  size_t i = find_in_array( *this, delObject );
  return ( i != SYS_MAX_T ) ? DestroyInd( i, fd ) : nullptr;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
void destroy_array( FDPArray<Type> & arr )
{
  size_t oldCount = arr.count;
  arr.count = 0;

  if ( arr.fDestroy ) {
    size_t i = 0;
    for ( const Type **parr = arr.GetAddr(); i < oldCount; i++, parr++ ) {

      Type *del = const_cast<Type*>(*parr);
      *parr = 0; // \ru сначала обнулить... \en set to null at first... 

      PRECONDITION( !del || arr.nowDeletedElem != del );  // \ru Временно, для отладки \en Temporarily, for debugging. 
      arr.nowDeletedElem = del;

      (*arr.fDestroy)( del ); // \ru ...потом удалить \en ... then delete 

      arr.nowDeletedElem = 0;
    }
  }
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
bool set_Farray_size( FDPArray<Type> & arr, size_t newSize, bool clear )
{
  PRECONDITION( arr.nowDeletedElem  == nullptr );
  
  if ( clear && arr.count )
    arr.Flush(); // \ru будет arr.count = 0; \en arr.count will be equal 0; 

  if ( newSize < arr.count ) { // \ru нужно удалить лишние элементы массива \en it is necessary to delete extra elements from the array 
    if ( arr.fDestroy ) {
      if ( newSize == 0 ) {
        if ( arr.count )
          arr.Flush(); // \ru будет arr.count = 0; \en arr.count will be equal 0; 
      }
      else
        while ( arr.count > newSize )
          arr.RemoveInd( arr.count - 1 ); // \ru удалить элемент из массива (по индексу), count-- \en delete an element from array (by the index), count-- 
    }

    arr.count = newSize;
  }

  return set_Rarray_size( arr, newSize );
}


#endif  // __TEMPL_FDP_ARRAY_H
