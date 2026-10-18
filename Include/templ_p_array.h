
////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Одномерный массив указателей.
         \en One-dimensional array of pointers. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_P_ARRAY_H
#define __TEMPL_P_ARRAY_H


#include <io_define.h>
#include <templ_delete_define.h>
#include <templ_rp_array.h>



// \ru Реализация методов для чтения/записи массива лежит в templ_p_array_rw.h \en Implementation of methods for reading/writing of array is located in templ_p_array_rw.h 


FORVARD_DECL_TEMPLATE_TYPENAME( class PArray );
FORVARD_DECL_TEMPLATE_TYPENAME( bool set_Parray_size( PArray<Type> &, size_t newSize, bool clear ) );
FORVARD_DECL_TEMPLATE_TYPENAME( void destroy_array  ( PArray<Type> & ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader& CALL_DECLARATION operator >> ( reader& in, PArray<Type> & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer& CALL_DECLARATION operator << ( writer& out, const PArray<Type> & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader& CALL_DECLARATION operator >> ( reader& in, PArray<Type> *& ptr ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer& CALL_DECLARATION operator << ( writer& out, const PArray<Type> * ptr ) );


//------------------------------------------------------------------------------
/** \brief \ru Одномерный массив указателей.
           \en One-dimensional array of pointers. \~
  \details \ru Одномерный массив указателей на объекты. 
           Может владеть(удалять) или не владеть указателями. \n
           \en One-dimensional array of pointers to objects. 
           Can own (delete) or not own pointers. \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class PArray : public RPArray<Type>
{
protected :
  bool    owns;           ///< \ru Флаг владения элементами массива (элементы можно удалять) \en A flag of ownership of elements of an array (elements can be deleted) 
  Type *  nowDeletedElem; ///< \ru Удаляемый элемент \en Deleted element 

public :
  /// \ru Конструктор. \en Constructor. 
  PArray()
    : RPArray<Type>()
    , owns( true )
    , nowDeletedElem(nullptr)
  {}
  /// \ru Конструктор. \en Constructor. 
  PArray( size_t i_upper, uint16 i_delta = 1, bool shouldDelete = true )//, bool shouldNullSet = false )
    : RPArray<Type>( i_upper, i_delta )//, shouldNullSet )
    , owns( shouldDelete )
    , nowDeletedElem(nullptr)
  {}
  /// \ru Деструктор. \en Destructor. 
  virtual ~PArray();
  /// \ru Владеем ли элементами? \en Are the elements owned? 
          bool    OwnsElem() const        { return owns; }
  /// \ru Выставить состояние флага владения элементами. \en Set the flag of elements ownership 
          void    OwnsElem( bool ownsEl ) { owns = ownsEl; }

  /// \ru Функции, выделяющие потенциально большие участки памяти, возвращают результат операции (успех/ошибка).
  /// \en Functions that allocate potentially large memory, return an operation result (success/error). 
          bool    SetSize ( size_t newSize, bool clear ); ///< \ru Установить новый размер массива. \en Set the new size of an array. 

          void    Flush    ( DelType = defDelete );                        ///< \ru Удалить все элементы. \en Delete all elements. 
          void    HardFlush( DelType shdl = defDelete ) { Flush(shdl); RPArray<Type>::Adjust(); } ///< \ru Освободить всю память. \en Free the whole memory. 
          void    RemoveAll( DelType shdl = defDelete ) { Flush(shdl); }   ///< \ru Удалить все элементы обнулить количество элементов. \en Delete all elements and set the number of elements to null. 
          Type  * RemoveObj( Type * delObject, DelType = defDelete );      ///< \ru Удалить элемент из массива. \en Delete an element from array. 
  virtual Type  * RemoveInd( size_t delIndex, DelType del = defDelete );   ///< \ru Удалить элемент из массива. \en Delete an element from array. 

public: // \ru унификация с вектором STL \en unification with STL vector 
  virtual void    clear() { Flush(); }        ///< \ru Обнулить количество элементов. \en Set the number of elements to null. 

private:
  /// \ru Перераспределение памяти под новый размер массива. \en Reallocation of memory for the new size of array. 
  TEMPLATE_FRIEND bool  set_Parray_size TEMPLATE_SUFFIX ( PArray<Type> &, size_t newSize, bool clear );
  // \ru Удаление всех указателей, собранных в массиве. \en Deletion of all pointers from array. 
  TEMPLATE_FRIEND void  destroy_array   TEMPLATE_SUFFIX ( PArray<Type> & );

  /// \ru Оператор чтения. \en Read operator. 
  TEMPLATE_FRIEND reader & CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader & in, PArray<Type> & ref );
  /// \ru Оператор записи. \en Write operator. 
  TEMPLATE_FRIEND writer & CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer & out, const PArray<Type> & ref );
  /// \ru Оператор чтения. \en Read operator. 
  TEMPLATE_FRIEND reader & CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader & in, PArray<Type> *& ptr );
  /// \ru Оператор записи. \en Write operator. 
  TEMPLATE_FRIEND writer & CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer & out, const PArray<Type> * ptr );
  
#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
public:
  void * operator new    ( size_t );
  void   operator delete ( void *, size_t );
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_

private:
  PArray( const PArray<Type> & );
  PArray<Type> & operator = ( const PArray<Type> & );
};


#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
//------------------------------------------------------------------------------
// \ru Перегрузка оператора new. \en Overloading of the "new" operator. 
// ---
template <class Type>
inline void * PArray<Type>::operator new( size_t size ) {
  return ::Allocate( size, typeid(PArray<Type>).name() );
}

//------------------------------------------------------------------------------
// \ru Перегрузка оператора delete. \en Overloading of the "delete" operator. 
// ---
template <class Type>
inline void PArray<Type>::operator delete( void *ptr, size_t size ) {
  ::Free( ptr, size, typeid(PArray<Type>).name() );
}
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_


FORVARD_DECL_TEMPLATE_TYPENAME( class PIArray );
FORVARD_DECL_TEMPLATE_TYPENAME( void   for_each_in_array  ( const PIArray<Type> &, typename PIArray<Type>::IteratorFunc func ) );
FORVARD_DECL_TEMPLATE_TYPENAME( void   for_each_in_array  ( const PIArray<Type> &, typename PIArray<Type>::ParIteratorFunc func, void * pars ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t first_that_in_array( const PIArray<Type> &, typename PIArray<Type>::CompareFunc func, void * pars, size_t from ) ); 


//------------------------------------------------------------------------------
/** \brief \ru Одномерный массив указателей с итераторными функциями.
           \en One-dimensional array of pointers with iterator functions. \~
  \details \ru Одномерный массив указателей с итераторными функциями. \n
           \en One-dimensional array of pointers to objects with iterator functions. \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class PIArray : virtual public PArray<Type> {
public :
  /// \ru Конструктор. \en Constructor. 
  PIArray()
    : PArray<Type>()
  {}
  /// \ru Конструктор. \en Constructor. 
  PIArray( size_t i_upper, uint16 i_delta = 1, uint8 shouldDelete = 1 )
    : PArray<Type>( i_upper, i_delta, !!shouldDelete )
  {}

  typedef void    (*IteratorFunc) ( Type * );
  //      void    ForEachI( IteratorFunc func ) const;

  typedef void    (*ParIteratorFunc) ( Type *, void * );
          void    ForEachI( ParIteratorFunc func, void * ) const;

  typedef int     (*CompareFunc) ( Type *, void * );
          size_t  FirstThatI( CompareFunc func, void * pars, size_t from = 0 ) const;

  TEMPLATE_FRIEND void   for_each_in_array   TEMPLATE_SUFFIX ( const PIArray<Type> &, IteratorFunc func );
  TEMPLATE_FRIEND void   for_each_in_array   TEMPLATE_SUFFIX ( const PIArray<Type> &, ParIteratorFunc func, void * pars );
  TEMPLATE_FRIEND size_t first_that_in_array TEMPLATE_SUFFIX ( const PIArray<Type> &, CompareFunc func, void * pars, size_t from );

private:
  PIArray( const PIArray<Type> & );          // \ru запрещено !!! \en forbidden !!! 
  void operator = ( const PIArray<Type> & ); // \ru запрещено !!! \en forbidden !!! 
};


FORVARD_DECL_TEMPLATE_TYPENAME( class PMIArray );
FORVARD_DECL_TEMPLATE_TYPENAME( void    for_each_in_array  ( const PMIArray<Type> &, typename PMIArray<Type>::IteratorMemFunc func ) );
FORVARD_DECL_TEMPLATE_TYPENAME( void    for_each_in_array  ( const PMIArray<Type> &, typename PMIArray<Type>::ParIteratorMemFunc func, void * pars ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t  first_that_in_array( const PMIArray<Type> &, typename PMIArray<Type>::CompareMemFunc func, void * pars, size_t from ) );


//------------------------------------------------------------------------------
/** \brief \ru Одномерный массив указателей с итераторными функциями.
           \en One-dimensional array of pointers with iterator functions. \~
  \details \ru Одномерный массив указателей с итераторными функциями - членами классов. \n
           \en One-dimensional array of pointers to objects with iterator functions - class members. \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class PMIArray : virtual public PArray<Type> {
public :
  /// \ru Конструктор. \en Constructor. 
  PMIArray()
    : PArray<Type>()
  {}
  /// \ru Конструктор. \en Constructor. 
  PMIArray( size_t i_upper, uint16 i_delta = 1, uint8 shouldDelete = 1 )
    : PArray<Type>( i_upper, i_delta, !!shouldDelete )
  {}

  typedef void    (Type::*IteratorMemFunc) (void);
  //      void    ForEach( IteratorMemFunc func ) const;

  typedef void    (Type::*ParIteratorMemFunc) (void * pars);
          void    ForEach( ParIteratorMemFunc func, void * pars ) const;

  typedef bool    (Type::*CompareMemFunc) (void * pars);
          size_t  FirstThat( CompareMemFunc func, void * pars, size_t from = 0 ) const;

  TEMPLATE_FRIEND void    for_each_in_array   TEMPLATE_SUFFIX ( const PMIArray<Type> &, IteratorMemFunc func );
  TEMPLATE_FRIEND void    for_each_in_array   TEMPLATE_SUFFIX ( const PMIArray<Type> &, ParIteratorMemFunc func, void * pars );
  TEMPLATE_FRIEND size_t  first_that_in_array TEMPLATE_SUFFIX ( const PMIArray<Type> &, CompareMemFunc func, void * pars, size_t from );

private:
  PMIArray( const PMIArray<Type> & );        // \ru запрещено !!! \en forbidden !!! 
  void operator =( const PMIArray<Type> & ); // \ru запрещено !!! \en forbidden !!! 
};


//------------------------------------------------------------------------------
// \ru деструктор массива \en destructor of array 
// ---
template <class Type>
inline PArray<Type>::~PArray() {
  PRECONDITION( nowDeletedElem  == nullptr );
  if ( owns )
    destroy_array( *this );
}


//------------------------------------------------------------------------------
// \ru обнулить количество элементов \en set the number of elements to null 
// ---
template <class Type>
inline void PArray<Type>::Flush( DelType del ) {
  PRECONDITION( nowDeletedElem  == nullptr );
  
  if ( del==Delete || (del==defDelete && owns) )
    destroy_array( *this );
  else
    RPArray<Type>::count = 0;
}


//------------------------------------------------------------------------------
// \ru Указать новый размер массива, если clear = true, то массив очистится !!!
// \en Set the new size of an array, if 'clear' is true than the array will be cleared !!! 
// ---
template <class Type>
inline bool PArray<Type>::SetSize( size_t newSize, bool clear ) {
  return set_Parray_size( *this, newSize, clear );
}


//------------------------------------------------------------------------------
// \ru удалить элемент из массива (по индексу) \en delete an element from array (by the index) 
// ---
template <class Type>
inline Type * PArray<Type>::RemoveInd( size_t delIndex, DelType del ) {
  PRECONDITION( delIndex < RPArray<Type>::count );

  Type * r = 0;

  if ( delIndex < RPArray<Type>::count ) {
    const Type ** d = RPArray<Type>::GetAddr() + delIndex;
    r = (Type *)*d;

    // \ru сначала приведем в порядок массив ... \en put an array in order at first ... 
    memmove( d, d + 1, (RPArray<Type>::count - delIndex - 1) * SIZE_OF_POINTER );
    RPArray<Type>::count--;

    // \ru ... а теперь будем удалять \en ... and now we will delete 
    if ( del == Delete || ( del == defDelete && owns ) ) {
      PRECONDITION( !r || nowDeletedElem != r );
      nowDeletedElem = r;

      delete r;
      r = 0;

      nowDeletedElem = 0;
    }
  }

  return r;
}


//------------------------------------------------------------------------------
// \ru удалить элемент из массива (по указателю) \en delete an element from array (by the pointer) 
// ---
template <class Type>
inline Type * PArray<Type>::RemoveObj( Type * delObject, DelType del ) {
  PRECONDITION( nowDeletedElem  == nullptr ); // \ru временно, для отладки \en temporarily, for debugging

  size_t i = find_in_array( *this, delObject );
  return (i != SYS_MAX_T) ? RemoveInd(i, del) : nullptr;
}


//-------------------------------------------------------------------------------
// \ru удаление всех указателей, собранных в массиве \en deletion of all pointers from array 
// ---
template <class Type>
void destroy_array( PArray<Type> & arr ) {
  size_t i = 0, oldCount = arr.count;

  arr.count = 0; // \ru сразу приведем в порядок массив ... \en put an array in order ... 

  // \ru ... а теперь будем удалять \en ... and now we will delete 
  for( const Type **parr = arr.GetAddr(); i < oldCount; i++, parr++ ) {

    Type * del = (Type *)*parr;
    *parr = 0; // \ru сначала обнулим указатель ... \en set pointer to null at first ... 

    PRECONDITION( !del || arr.nowDeletedElem != del ); 
    arr.nowDeletedElem = del; // \ru временно \en temporarily 

    delete del; // \ru ... а потом будем удалять \en ... and then delete 

    arr.nowDeletedElem = 0;  // \ru временно \en temporarily 
  }
}


//------------------------------------------------------------------------------
// \ru Перераспределение памяти под новый размер массива. \en Reallocation of memory for the new size of array. 
// ---
template <class Type>
bool set_Parray_size( PArray<Type> & arr, size_t newSize, bool clear ) {
  PRECONDITION( arr.nowDeletedElem  == nullptr ); // \ru временно \en temporarily

  if ( clear && arr.count )
    arr.Flush(); // \ru будет arr.count = 0; \en arr.count will be equal 0; 

  if ( newSize < arr.count ) {
    if ( arr.owns ) {
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


//------------------------------------------------------------------------------
// \ru выполнить функцию для каждого элемента \en perform the function for every element 
// ---
//template <class Type>
//inline void PIArray<Type>::ForEachI( IteratorFunc func ) const {
// \ru C3D_ASSERT( PArray<Type>::nowDeletedElem  == nullptr ); // временно \en C3D_ASSERT( PArray<Type>::nowDeletedElem  == nullptr ); // temporarily
//#if !defined ( __INTEL_COMPILER ) /// for Intel C++ Compiler
//  for_each_in_array( *this, func );
//#endif // __INTEL_COMPILER
//}


//------------------------------------------------------------------------------
// \ru выполнить функцию с параметрами для каждого элемента \en perform the function with parameters for every element 
// ---
template <class Type>
inline void PIArray<Type>::ForEachI( ParIteratorFunc func, void * pars ) const {
  PRECONDITION( PArray<Type>::nowDeletedElem  == nullptr );
  for_each_in_array( *this, func, pars );
}


//------------------------------------------------------------------------------
// \ru найти элемент по условию \en find an element by condition 
// ---
template <class Type>
inline size_t PIArray<Type>::FirstThatI( CompareFunc func, void * pars, size_t from ) const {
  PRECONDITION( PArray<Type>::nowDeletedElem  == nullptr );
  return first_that_in_array( *this, func, pars, from );
}


//------------------------------------------------------------------------------
// \ru выполнить функцию для каждого элемента \en perform the function for every element 
// ---
//template <class Type>
//inline void PMIArray<Type>::ForEach( IteratorMemFunc func ) const {
// \ru C3D_ASSERT( PArray<Type>::nowDeletedElem  == nullptr );
//#if !defined ( __INTEL_COMPILER ) /// for Intel C++ Compiler
//  for_each_in_array( *this, func );
//#endif // __INTEL_COMPILER
//}


//------------------------------------------------------------------------------
// \ru выполнить функцию с параметрами для каждого элемента \en perform the function with parameters for every element 
// ---
template <class Type>
inline void PMIArray<Type>::ForEach( ParIteratorMemFunc func, void * pars ) const {
  PRECONDITION( PArray<Type>::nowDeletedElem  == nullptr );
  for_each_in_array( *this, func, pars );
}


//------------------------------------------------------------------------------
// \ru найти элемент по условию \en find an element by condition 
// ---
template <class Type>
inline size_t  PMIArray<Type>::FirstThat( CompareMemFunc func, void * pars, size_t from ) const {
  PRECONDITION( PArray<Type>::nowDeletedElem  == nullptr );
  return first_that_in_array( *this, func, pars, from );
}


//-------------------------------------------------------------------------------
// \ru вызвать итераторную функцию для каждого объекта массива \en call iterator function for every object of an array 
// ---
template <class Type>
void for_each_in_array( const PIArray<Type>& arr, void ( *func ) ( Type * ) ) {
  for( size_t  i = 0; i < arr.Count(); i++ )
    func( arr[i] );
}


//-------------------------------------------------------------------------------
// \ru вызвать итераторную функцию для каждого объекта массива \en call iterator function for every object of an array 
// ---
template <class Type>
void for_each_in_array( const PIArray<Type> & arr, void ( *func ) ( Type *, void * ), void * pars ) {
  for( size_t  i = 0; i < arr.Count(); i++ )
    func( arr[i], pars );
}


//-------------------------------------------------------------------------------
// \ru найти объект, удовлетворяющий условию, которое проверяется в присланной функции \en find an object satisfying the condition which is verified in the given function 
// ---
template <class Type>
size_t first_that_in_array( const PIArray<Type> & arr, int ( *func ) ( Type *, void * ), void * pars, size_t from ) {
  for( size_t i = from; i < arr.Count(); i++ )
    if ( func( arr[i], pars ) )
      return i;

  return SYS_MAX_T;
}


//-------------------------------------------------------------------------------
// \ru вызвать итераторную функцию-метод для каждого объекта массива \en call iterator function-method for every object of an array 
// ---
template <class Type>
void for_each_in_array( const PMIArray<Type> & arr, void ( Type::*func ) (void) ) {
  for( size_t i = 0; i < arr.Count(); i++ )
    (arr[i]->*func)();
}


//-------------------------------------------------------------------------------
// \ru вызвать итераторную функцию-метод для каждого объекта массива \en call iterator function-method for every object of an array 
// ---
template <class Type>
void for_each_in_array( const PMIArray<Type> & arr, void ( Type::*func ) (void *), void * pars ) {
  for( size_t i = 0; i < arr.Count(); i++ )
    (arr[i]->*func)( pars );
}


//-------------------------------------------------------------------------------
// \ru найти объект, удовлетворяющий условию, которое проверяется в присланной функции-методе \en find an object satisfying the condition which is verified in the sent function-method 
// ---
template <class Type>
size_t first_that_in_array( const PMIArray<Type> & arr, bool ( Type::*func ) (void *), void * pars, size_t from ) {
  for( size_t i = from; i < arr.Count(); i++ )
    if ( (arr[i]->*func)(pars) )
      return i;

  return SYS_MAX_T;
}


#endif  // __TEMPL_P_ARRAY_H
