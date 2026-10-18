////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Упорядоченный массив указателей.
         \en Ordered array of pointers. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_SP_ARRAY_H
#define __TEMPL_SP_ARRAY_H


#include <templ_p_array.h>


////////////////////////////////////////////////////////////////////////////////
//
// \ru упорядоченный массив указателей \en ordered array of pointers 
// \ru у объектов массива должны быть операторы "==" и "<" \en objects of the array should have operators "==" and "<"  
// \ru одинаковые объекты не добавляются \en the similar objects are not added 
//
////////////////////////////////////////////////////////////////////////////////

FORVARD_DECL_TEMPLATE_TYPENAME( class SPArray );
FORVARD_DECL_TEMPLATE_TYPENAME( Type *  add_to_array        (       SPArray<Type> &,       Type * ent, size_t & indexEnt ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t  find_from_array     ( const SPArray<Type> &, const Type * ent ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t  find_from_array_spec( const SPArray<Type> &, const Type * ent, bool & isPresent ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader & CALL_DECLARATION operator >> ( reader & in,        SPArray<Type> & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer & CALL_DECLARATION operator << ( writer & out, const SPArray<Type> & ref ) );

template <class Type>
class  SPArray : protected PArray<Type> {
public :
  SPArray( size_t maxCnt = 0, uint16 delt = 1, bool shouldDelete = true  )
    : PArray<Type>( maxCnt, delt, shouldDelete ) {}

  using PArray<Type>::OwnsElem;
  using PArray<Type>::Delta;
  using PArray<Type>::Upper;
  using PArray<Type>::Flush;
  using PArray<Type>::HardFlush;
  using PArray<Type>::Adjust;
  using PArray<Type>::RemoveInd;
  using PArray<Type>::Count;
  using PArray<Type>::MaxIndex;
  using PArray<Type>::operator[];
  using PArray<Type>::Reserve;
  using PArray<Type>::SetSize;
  using PArray<Type>::GetLast;
  using PArray<Type>::FindIt;

  using PArray<Type>::empty;
  using PArray<Type>::size;
  using PArray<Type>::reserve;
  using PArray<Type>::capacity;
  using PArray<Type>::begin;
  using PArray<Type>::end;
  using PArray<Type>::front;
  using PArray<Type>::back;
  using PArray<Type>::clear;

  Type  *                 Add( Type * );                   // \ru добавить элемент с упорядочиванием по массиву \en add element with sorting 
  Type  *                 Add( Type *, size_t & indexEnt );// \ru добавить элемент с упорядочиванием по массиву, возвращает индекс \en add element with sorting, returns index of the element 

  // \ru После "простого" добавления нет возможности восстановить сортированность массива \en After a "simple" adding there is no possibility to recover the sorting of the array  
  void                    AddSimple     ( Type * ent ) { PArray<Type>::Add( ent ); } // \ru Доступ к функции базового класса - добавить элемент в конец массива \en An access to the function of the base class - add an element to the end of the array 
          // \ru Доступ к функции базового класса - добавить массив в конец массива \en An access to the function of the base class - add an array to the end of the array 
  bool                    AddArraySimple( const RPArray<Type> & arr ) { return PArray<Type>::AddArray( arr ); } 
  Type  *                 RemoveObj( Type * delObject, DelType = defDelete );   // \ru удалить элемент из массива \en delete an element from array 
          // \ru Доступ к функции базового класса - удалить элемент из массива \en An access to the function of the base class - delete an element from the array 
  Type  *                 RemoveObjSimple( Type * delObject, DelType delType = defDelete ) { return PArray<Type>::RemoveObj( delObject, delType ); } ;   // \ru удалить элемент из массива \en delete an element from array 

  size_t                  Find( const Type * ) const;                            // \ru найти элемент в упорядоченном массиве \en find an element in ordered array 
  size_t                  PossibleIndex( const Type *, bool & isPresent ) const; // \ru найти место в массиве, куда будет добавлен элемент ( без добавления ) \en find a place in the array for adding an element (adding is not performed) 
                                                                                 // \ru на выходе : isPresent == true - элемент уже в массиве \en in output : isPresent == true - the element is already in the array 
  bool                    IsExist( const Type * ) const; // \ru true если элемент найден \en true if the element was found 

  TEMPLATE_FRIEND Type *  add_to_array         TEMPLATE_SUFFIX (       SPArray<Type> &,       Type * ent, size_t & indexEnt );
  TEMPLATE_FRIEND size_t  find_from_array      TEMPLATE_SUFFIX ( const SPArray<Type> &, const Type * ent );
  TEMPLATE_FRIEND size_t  find_from_array_spec TEMPLATE_SUFFIX ( const SPArray<Type> &, const Type * ent, bool & isPresent );

private:
  SPArray( const SPArray<Type> & );           // \ru запрещено !!! \en forbidden !!! 
  void operator = ( const SPArray<Type> & );  // \ru запрещено !!! \en forbidden !!! 

  // \ru Т.к. наследование private, то сделаем здесь операторы чтения-записи \en Since there is a private inheritance, then make here operators of reading/writing 
//ID K8  KNOWN_OBJECTS_RW_REF_OPERATORS( SPArray<Type> )
  TEMPLATE_FRIEND reader & CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader & in,        SPArray<Type> & ref );
  TEMPLATE_FRIEND writer & CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer & out, const SPArray<Type> & ref );
  
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
inline void * SPArray<Type>::operator new( size_t size ) {
  return ::Allocate( size, typeid(SPArray<Type>).name() );
}

//------------------------------------------------------------------------------
// \ru Перегрузка оператора delete. \en Overloading of the "delete" operator. 
// ---
template <class Type>
inline void SPArray<Type>::operator delete( void * ptr, size_t size ) {
  ::Free( ptr, size, typeid(SPArray<Type>).name() );
}
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_


//-------------------------------------------------------------------------------
// \ru добавить элемент с упорядочиванием по массиву \en add element with sorting 
// ---
template <class Type>
inline Type * SPArray<Type>::Add( Type * el )
{
  size_t index;
  return add_to_array( *this, el, index );
}


//-------------------------------------------------------------------------------
// \ru добавить элемент с упорядочиванием по массиву, возвращает индекс \en add element with sorting, returns index of the element 
// ---
template <class Type>
inline Type* SPArray<Type>::Add( Type * el, size_t & indexEl ) {
  return add_to_array( *this, el, indexEl );
}


//-------------------------------------------------------------------------------
// \ru поиск объекта в массиве \en search of an element in array 
// ---
template <class Type>
inline size_t SPArray<Type>::Find( const Type * el ) const {
  return find_from_array( *this, el );
}


//------------------------------------------------------------------------------
// \ru Есть ли в массиве такой указатель \en Whether such pointer exists in the array 
// ---
template <class Type>
inline bool SPArray<Type>::IsExist( const Type * el ) const {
  return find_from_array( *this, el ) != SYS_MAX_T;
}


//------------------------------------------------------------------------------
// \ru удалить элемент из массива (по указателю) \en delete an element from array (by the pointer) 
// ---
template <class Type>
inline Type * SPArray<Type>::RemoveObj( Type * delObject, DelType del )
{
  PRECONDITION( PArray<Type>::nowDeletedElem  == nullptr ); // \ru Временно, для отладки \en Temporarily, for debugging
  size_t i = find_from_array( *this, delObject );
  return ( i != SYS_MAX_T ) ? RemoveInd( i, del ) : nullptr;
}


//-------------------------------------------------------------------------------
// \ru найти место в массиве, куда будет добавлен элемент ( без добавления ) \en find a place in the array for adding ann element (adding is not performed) 
// \ru на выходе : isPresent == true - элемент уже в массиве \en in output : isPresent == true - the element is already in the array 
// ---
template <class Type>
inline size_t SPArray<Type>::PossibleIndex( const Type * el, bool & isPresent ) const {
  return find_from_array_spec( *this, el, isPresent );
}


//-------------------------------------------------------------------------------
// \ru добавление объекта в массив с упорядочиванием \en adding an object to the array with ordering 
// ---
template <class Type>
Type * add_to_array( SPArray<Type> & arr, Type * el, size_t & indexEl )
{
  if ( el == nullptr ) // \ru LF_Linux: добавил проверку на nullptr \en LF_Linux: added a check for nullptr
    return nullptr;
  size_t mx = arr.count - 1;
  size_t mxc = mx;
  size_t mn = 0;

  // \ru общий случай - элементов больше двух \en the common case - the number of elements is more than two 
  if ( arr.count > 2 ) {
    while ( mn + 1 < mx ) {  // \ru пока не нашли - ищем \en seek until do not find 
      size_t md = ( mn + mx ) / 2;
      if ( *arr[md] < *el ) {
        if ( *el == *arr[mx] ) {
          indexEl = mx;
          return 0;
        }
        mn = md;
      }
      // \ru по логике правильнее было бы проверять сначала меньше, потом тождественно, а затем, уже \en it would be better to check at first whether it is less, then whether it is equal and only after this 
      // \ru без сравнения - делать вывод что больше. \en conclude which is greater without comparison. 
      // \ru НО! оператор сравнения, как правило более быстрый, чем оператор тождественности, \en BUT! the comparison operator is generally faster than identity operator, 
      // \ru и заведомо более часто используется( тождественно - финишная операция в поиске ) \en and it is used more often (identity check is the last operation in search) 
      // \ru и если поставить проверку тождественности впереди "больше" - можно получить торможение \en and if the identity check will be placed before the "greater" check then there may occur an inhibition 
      // \ru на тяжелых операторах \en on heavy operators 
      // \ru Кроме того, все три проверки делаются, дабы не отказывать программистам в их праве делать \en - 
      // \ru ошибки при написании операторов сравнения и тождественности и потом их с комфортом исправлять. \en - 
      else if ( *el < *arr[md] ) {
        if ( *el == *arr[mn] ) {
          indexEl = mn;
          return 0;
        }
        mx = md;
      }
      else if ( *arr[md] == (Type&)*el ) {
        indexEl = md;
        return 0;
      }
      // \ru если попадаем сюда - значит некорректно написаны операторы "тождественно" и сравнения \en if we are here then operators of identity check and comparison are not correct 
      else {
        PRECONDITION( 0 );
        indexEl = SYS_MAX_T;
        return 0;
      }
    }

    if ( *el < *arr[0] ) {
      arr.Insert( 0, el );
      indexEl = 0;
      return arr[0]; //arr.parr;
    }
    if ( *arr[mxc] < *el ) {
      arr.PArray<Type>::Add( el );
      indexEl = mxc + 1;
      return arr[indexEl]; //arr.parr + indexEl;
    }

    arr.Insert( mx, el );
    indexEl = mx;
    return arr[mx];//arr.parr + mx;
  }

  // \ru специальные случаи \en special cases 
  // \ru массив пустой - просто добавляем \en the array is empty - simply add 
  // \ru проверку на пустоту массива нельзя переносить за проверку границ - \en a check for array emptiness should not be moved outside a boundary check -  
  // \ru можно вылететь (из-за mxc = -1) \en the error may occur (because of mxc = -1) 
  if ( !arr.count ) {
    arr.PArray<Type>::Add( el );
    indexEl = 0;
    return arr[0]; //arr.parr;
  }

  // \ru SA K6 элемент за границами массива - добавляем \en SA K6 an element is out of bounds - add it 
  if ( *el < *arr[0] ) {
    arr.Insert( 0, el );
    indexEl = 0;
    return arr[0]; //arr.parr;
  }
  if ( *arr[mxc] < *el ) {
    arr.PArray<Type>::Add( el );
    indexEl = mxc + 1;
    return arr[indexEl]; //arr.parr + indexEl;
  }

  // \ru SA K6 элемент не за границами массива и массив содержит 2 элемента \en SA K6 an element is not outside the array's bounds and the array consists of 2 elements 
  if ( arr.count == 2 ) {
    if ( *el == *arr[0]/*arr.parr*/ ) {
      indexEl = 0;
      return 0;
    }
    else
      if ( *el == *arr[1]/*arr.parr[1]*/ ) {
        indexEl = 1;
        return 0;
      }
      else {
        arr.Insert( 1, el );
        indexEl = 1;
        return arr[1]; // arr.parr + 1;
      }
  }

  indexEl = 0;
  return 0;
}


//-------------------------------------------------------------------------------
// \ru поиск объекта в массиве \en search of an element in array  
// \ru поиск ведется методом половинных делений \en a search is performed by the bisection method 
// ---
template <class Type>
size_t find_from_array_spec( const SPArray<Type> & arr, const Type * el, bool & isPresent )
{
  isPresent = false;
  
  if ( el == nullptr ) // \ru LF_Linux: добавил проверку на nullptr \en LF_Linux: added a check for nullptr
    return SYS_MAX_T;
  
  if ( !arr.count  || *el < *arr/*.parr*/[0] )
    return 0;

  size_t mx = arr.count - 1;

  if ( *arr/*.parr*/[mx] < *el )
    return mx + 1;

  if ( arr.count == 1 ) { // \ru значит *el == *parr[0] \en it means *el == *parr[0] 
    isPresent = true;
    return 0;
  }

  if ( arr.count == 2 ) { // \ru значит между 0  и 1 \en between 0 and 1 
    if (*el == *arr/*.parr*/[0]) {
      isPresent = true;
      return 0;
    }
    else {
      if (*el == *arr/*.parr*/[1]) isPresent = true;
      return 1;
    }
  }

  size_t mn = 0;

  while ( mn + 1 < mx ) {  // \ru пока не нашли - ищем \en seek until do not find 
    if (*el == *arr/*.parr*/[mn]) {
      isPresent = true;
      return mn;
    }
    else {
      if ( *el == *arr/*.parr*/[mx] ) {
        isPresent = true;
        return mx;
      }
      else {
        size_t md = (mn + mx) / 2;
        if ( *arr/*.parr*/[md] < *el )
          mn = md;
        else if ( *el < *arr/*.parr*/[md] )
          mx = md;
        else if ( *arr/*.parr*/[md] == (Type&)*el ) {
          isPresent = true;
          return md;
        }
      }
    }
  }

  return mx;
}


//-------------------------------------------------------------------------------
// \ru поиск объекта в массиве \en search of an element in array 
// \ru поиск ведется методом половинных делений \en a search is performed by the bisection method 
// \ru Входные параметры: \en Input parameters: 
// \ru arr - массив элементов \en arr - the array of elements 
// \ru el  - объект для поиска \en el - the object for the search 
// \ru Возвращаемое значение: \en The returned value: 
// \ru Индекс объекта в массиве. Если объект не был найден, то возращается SYS_MAX_T. \en index of an object in array If the object was not found then SYS_MAX_T is returned. 
// ---
template <class Type>
size_t find_from_array( const SPArray<Type> & arr, const Type * el )
{
  if ( el == nullptr ) // \ru LF_Linux: добавил проверку на nullptr \en LF_Linux: added a check for nullptr
    return SYS_MAX_T;
  // \ru общий случай - элементов больше 11 //LF_Linux: откуда 11??? \en the common case - the number of elements is more than 11 //LF_Linux: why 11?? 
  if ( arr.count > 11 ) {
    size_t mx = arr.count - 1;
    size_t mxc = mx;
    size_t mn = 0;

    while ( mn + 1 < mx ) {  // \ru пока не нашли - ищем \en seek until do not find 
      size_t md = ( mn + mx ) / 2;
      if ( *arr[md] < *el ) {
        if ( *el == *arr[mx] )
          return mx;
        mn = md;
      }
      // \ru по логике правильнее было бы проверять сначала меньше, потом тождественно, а затем, уже \en it would be better to check at first whether it is less, then whether it is equal and only after this 
      // \ru без сравнения - делать вывод что больше. \en conclude which is greater without comparison. 
      // \ru НО! оператор сравнения, как правило более быстрый, чем оператор тождественности, \en BUT! the comparison operator is generally faster than identity operator, 
      // \ru и заведомо более часто используется( тождественно - финишная операция в поиске ) \en and it is used more often (identity check is the last operation in search) 
      // \ru и если поставить проверку тождественности впереди "больше" - можно получить торможение \en and if the identity check will be placed before the "greater" check then there may occur an inhibition 
      // \ru на тяжелых операторах \en on heavy operators 
      // \ru Кроме того, все три проверки делаются, дабы не отказывать программистам в их праве делать \en - 
      // \ru ошибки при написании операторов сравнения и тождественности и потом их с комфортом исправлять. \en - 
      else if ( *el < *arr[md] ) {
        if ( *el == *arr[mn] )
          return mn;
        mx = md;
      }
      else if ( *arr[md] == *el )
        return md;
      // \ru если попадаем сюда - значит некорректно написаны операторы "тождественно" и сравнения \en if we are here then operators of identity check and comparison are not correct 
      else {
        PRECONDITION( 0 );
        return SYS_MAX_T;
      }
    }

    // \ru проверка по границам \en check by bounds 
    if ( *el < *arr[0] )
      return SYS_MAX_T;
    if ( *arr[mxc] < *el )
      return SYS_MAX_T/*-1*/;
  }
  else {
    // \ru специальные случаи \en special cases 
    if ( arr.count == 1 )
      return *el == *arr[0] ? 0 : SYS_MAX_T;
    else {
      if ( arr.count == 2 )
        return *el == *arr[0] ? 0 : ((*el == *arr[1]) ? 1 : SYS_MAX_T);
      else {
        // 2 < count <= 11
        for( size_t i = 0, count = arr.count; i < count; i++ )
          if ( *el == *arr[i] )
            return i;
      }
    }
  }

  return SYS_MAX_T;
}


#endif // __TEMPL_SP_ARRAY_H
