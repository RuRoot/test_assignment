////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Упорядоченный массив объектов.
         \en Ordered array of objects. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_SSARRAY_H
#define __TEMPL_SSARRAY_H


#include <templ_s_array.h>


FORVARD_DECL_TEMPLATE_TYPENAME( class SSArray );
FORVARD_DECL_TEMPLATE_TYPENAME( Type * add_to_array        (       SSArray<Type> &, const Type & el, size_t & indexEl ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t find_in_array       ( const SSArray<Type> &, const Type & el ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t find_from_array_spec( const SSArray<Type> &, const Type & el, bool & isPresent ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader & CALL_DECLARATION operator >> ( reader & in,        SSArray<Type> & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer & CALL_DECLARATION operator << ( writer & out, const SSArray<Type> & ref ) );

//------------------------------------------------------------------------------
/** \brief \ru Упорядоченный массив.
           \en Ordered array. \~
  \details \ru Упорядоченный массив объектов. \n
    У объектов массива должны быть операторы "==" и "<".
    Одинаковые объекты не добавляются. \n
           \en Ordered array of objects. \n
    Elements of the array should have operators "==" and "<".
    The similar objects are not added. \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class SSArray : protected SArray<Type> {
public :
  SSArray( size_t maxCnt = 0, uint16 delt = 1 ) : SArray<Type>( maxCnt, delt ) {}
  SSArray( const SSArray<Type> & other )        : SArray<Type>( other ) {}
protected:
  SSArray( const SArray<Type> & other )         : SArray<Type>( other ) {}
public:
  using SArray<Type>::operator[];

  using SArray<Type>::Flush;
  using SArray<Type>::HardFlush;
  using SArray<Type>::Adjust;
  using SArray<Type>::Remove;
  using SArray<Type>::RemoveInd;  
  using SArray<Type>::Count;
  using SArray<Type>::MaxIndex;
  using SArray<Type>::GetAddr;
  using SArray<Type>::GetEndAddr;
  using SArray<Type>::Reserve;
  using SArray<Type>::SetSize;
  using SArray<Type>::SetMaxDelta;

  using SArray<Type>::empty;
  using SArray<Type>::size;
  using SArray<Type>::reserve;
  using SArray<Type>::capacity;
  using SArray<Type>::begin;
  using SArray<Type>::end;
  using SArray<Type>::cbegin;
  using SArray<Type>::cend;
  using SArray<Type>::front;
  using SArray<Type>::back;
  using SArray<Type>::clear;
  using SArray<Type>::erase;
  using SArray<Type>::shrink_to_fit;

  Type  * Add ( const Type & );                  // \ru добавить элемент с упорядочиванием по массиву \en add element with sorting 
  Type  * Add ( const Type &, size_t & indexEnt ); // \ru добавить элемент с упорядочиванием по массиву, возвращает индекс \en add element with sorting, returns index of the element 
  // \ru используя эту функцию, пользователь несет всю ответственность за дальнейшую работу сортировки \en When using this function the user is fully responsible for the further work of the sorting process 
  // \ru - сортировка может работать неправильно. \en - sorting may work incorrectly. 
  void    AddSimple( const Type & ent ) { SArray<Type>::Add( ent ); } // \ru Доступ к функции базового класса - добавить элемент в конец массива \en An access to the function of the base class - add an element to the end of the array 

  size_t  Find( const Type & ) const;      // \ru найти элемент в упорядоченном массиве \en find an element in ordered array 
  size_t  RemoveObj( const Type & delObject );

  bool    operator == ( const SSArray<Type> & ) const; // \ru сравнить два массива \en compare two arrays 
  bool    operator != ( const SSArray<Type> & ) const; // \ru сравнить два массива \en compare two arrays 
  bool    operator <  ( const SSArray<Type> & ) const; // \ru сравнить два массива \en compare two arrays 
  void    operator =  ( const SSArray<Type> & arr ) { SArray<Type>::operator = ( arr ); }
  
  // \ru преобразование к базовому классу \en Convert to the base class 
  const SArray<Type>  & BaseClass() const { return *this; }

  size_t  PossibleIndex( const Type & ent, bool & isPresent ) const; // \ru найти место в массиве, куда будет добавлен элемент ( без добавления ) \en find a place in the array for adding ann element (adding is not performed) 
                                                                     // \ru на выходе : isPresent == true - элемент уже в массиве \en in output : isPresent == true - the element is already in the array 

  TEMPLATE_FRIEND Type * add_to_array         TEMPLATE_SUFFIX (       SSArray<Type> &, const Type & el, size_t & indexEl );
  TEMPLATE_FRIEND size_t find_in_array        TEMPLATE_SUFFIX ( const SSArray<Type> &, const Type & el );
  TEMPLATE_FRIEND size_t find_from_array_spec TEMPLATE_SUFFIX ( const SSArray<Type> &, const Type & el, bool & isPresent );

  TEMPLATE_FRIEND reader & CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader & in,        SSArray<Type> & ref );
  TEMPLATE_FRIEND writer & CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer & out, const SSArray<Type> & ref );
  
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
inline void * SSArray<Type>::operator new( size_t size ) {
  return ::Allocate( size, typeid(SSArray<Type>).name() );
}

//------------------------------------------------------------------------------
// \ru Перегрузка оператора delete. \en Overloading of the "delete" operator. 
// ---
template <class Type>
inline void SSArray<Type>::operator delete ( void * ptr, size_t size) {
  ::Free( ptr, size, typeid(SSArray<Type>).name() );
}
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_


//-------------------------------------------------------------------------------
// \ru оператор сравнения двух массивов \en an operator of two arrays comparison 
// ---
template <class Type>
inline bool SSArray<Type>::operator == ( const SSArray<Type> & w ) const {
  if ( SArray<Type>::count != w.count )
    return false;

  // \ru OV K6 При размещении в памяти с выравниванием не равным 1, между элементами массива \en OV K6 While the memory allocation with alignment which is not equal 1 between elements of the array  
  // \ru возможно появление "дырок" заполненного случайным мусором, т.к. сравнивать этот мусор \en may appear "holes" filled by random trash, since there is not reason to compare this trash 
  // \ru нам никчему, будем сравнивать содержимое массивов поэлементно (через оператор == объекта) \en we will compare the content of arrays element by element (using the operator == of an object) 
  for ( size_t i = 0; i < SArray<Type>::count; ++i ) {
    if ( !((*this)[i] == w[i]) )
      return false;
  }
  return true;
}


//-------------------------------------------------------------------------------
// \ru оператор сравнения двух массивов \en an operator of two arrays comparison 
// ---
template <class Type>
inline bool SSArray<Type>::operator < ( const SSArray<Type> & w ) const {
  // \ru OV K6 При размещении в памяти с выравниванием не равным 1, между элементами массива \en OV K6 While the memory allocation with alignment which is not equal 1 between elements of the array  
  // \ru возможно появление "дырок" заполненного случайным мусором, т.к. сравнивать этот мусор \en may appear "holes" filled by random trash, since there is not reason to compare this trash 
  // \ru нам никчему, будем сравнивать содержимое массивов поэлементно (через оператор < объекта) \en we will compare the content of arrays element by element (using the operator < of an object) 
  for ( size_t i = 0, c = std_min(SArray<Type>::count, w.count); i < c; i++ ) {
    if ( !((*this)[i] == w[i]) )
      return static_cast<bool>((*this)[i] < w[i]);
  }

  if ( SArray<Type>::count != w.count )
    return SArray<Type>::count < w.count;

  return true;
}


//-------------------------------------------------------------------------------
// \ru оператор сравнения двух массивов \en an operator of two arrays comparison 
// ---
template <class Type>
inline bool SSArray<Type>::operator != ( const SSArray<Type> & w ) const {
  return ! ( *this == w );
}


//-------------------------------------------------------------------------------
// \ru добавление объекта в массив \en adding an object to array 
// ---
template <class Type>
inline Type * SSArray<Type>::Add( const Type & el ) {
  size_t indexEl = SYS_MAX_T;
  return add_to_array( *this, el, indexEl );
}


//-------------------------------------------------------------------------------
// \ru добавление объекта в массив \en adding an object to array 
// ---
template <class Type>
inline Type * SSArray<Type>::Add( const Type & el, size_t & indexEl ) {
  return add_to_array( *this, el, indexEl );
}


//------------------------------------------------------------------------------
// \ru удалить элемент из массива \en delete an element from array 
// ---
template <class Type>
inline size_t SSArray<Type>::RemoveObj( const Type & delObject )
{
  size_t ind = Find( delObject );
  if ( ind != SYS_MAX_T )
    RemoveInd( ind );
  return ind;
}


//-------------------------------------------------------------------------------
// \ru поиск объека в массиве \en search of an element in array 
// ---
template <class Type>
inline size_t SSArray<Type>::Find( const Type & el ) const {
  return find_in_array( *this, el );
}


//-------------------------------------------------------------------------------
// \ru найти место в массиве, куда будет добавлен элемент ( без добавления ) \en find a place in the array for adding ann element (adding is not performed) 
// ---
template <class Type>
inline size_t SSArray<Type>::PossibleIndex( const Type & el, bool & isPresent ) const {
  return find_from_array_spec( *this, el, isPresent );
}


//-------------------------------------------------------------------------------
// \ru добавить объект в массив с упорядочиванием \en add an object to the array with ordering 
// ---
template <class Type>
Type * add_to_array( SSArray<Type> & arr, const Type & el, size_t & indexEl )
{
  size_t mx = arr.count - 1;
  size_t mxc = mx;
  size_t mn = 0;

  if ( arr.count > 2 ) {
    while ( mn + 1 < mx ) {  // \ru пока не нашли - ищем \en seek until do not find 
      size_t md = ( mn + mx ) / 2;
      Type & mdE = arr[md];
      if ( mdE < el ) {
        if ( el == arr[mx] ) {
          indexEl = mx;
          return 0;
        }
        mn = md;
      }
      // \ru по логике правильно было бы проверять сначала меньше, потом тождественно, а затем, уже \en it would be better to check at first whether it is less, then whether it is equal and only after this 
      // \ru без сравнения - делать вывод что больше. \en conclude which is greater without comparison. 
      // \ru НО! оператор сравнения, как правило более быстрый, чем оператор тождественности, \en BUT! the comparison operator is generally faster than identity operator, 
      // \ru и заведомо более часто используется( тождественно - финишная операция в поиске ) \en and it is used more often (identity check is the last operation in search) 
      // \ru и если поставить проверку тождественности впереди "больше" - можно получить торможение \en and if the identity check will be placed before the "greater" check then there may occur an inhibition 
      // \ru на тяжелых операторах \en on heavy operators 
      // \ru Кроме того, все три проверки делаются, дабы не отказывать программистам в их праве делать \en - 
      // \ru ошибки при написании операторов сравнения и тождественности и потом их с комфортом исправлять. \en - 
      else if ( el < mdE ) {
        if ( el == arr[mn] ) {
          indexEl = mn;
          return 0;
        }
        mx = md;
      }
      else if ( mdE == const_cast<Type&>(el) ) {
        indexEl = md;
        return 0;
      }
      // \ru если попадаем сюда - значит некорректно написаны операторы "тождественно" и сравнения \en if we are here then operators of identity check and comparison are not correct 
      else {
        PRECONDITION( 0 );
        return 0; // \ru но это не повод устраивать зависание. \en we have to exit to avoid hang.
      }
    }

    // \ru здесь дублирование кода проверки границ \en here the code of boundaries check is duplicated 
    // \ru объем кода принесен в жертву скорости \en the code is huge but optimized 
    if ( el < arr[0] ) {
      Type * res = arr.InsertInd( 0, el );
      indexEl = 0;
      return res; //arr.parr;
    }
    if ( arr[mxc] < el ) {
      Type * res = arr.SArray<Type>::Add( el );
      indexEl = mxc + 1;
      return res; 
    }

    Type * res = arr.InsertInd( mx, el );
    indexEl = mx;
    return res;
  }

  // \ru проверку на пустоту массива нельзя переносить за проверку границ - \en a check for array emptyness should not be moved outside a boundary check -  
  // \ru можно вылететь (из-за mxc = -1) \en the error may occur (because of mxc = -1) 
  if ( !arr.count ) {
    Type * res = arr.SArray<Type>::Add( el );
    indexEl = 0;
    return res; //arr.parr;
  }

  // \ru здесь дублирование кода проверки границ \en here the code of boundaries check is duplicated 
  // \ru объем кода принесен в жертву скорости \en the code is huge but optimized 
  if ( el < arr[0] ) {
    Type * res = arr.InsertInd( 0, el );
    indexEl = 0;
    return res; 
  }
  if ( arr[mxc] < el ) {
    Type * res = arr.SArray<Type>::Add( el );
    indexEl = mxc + 1;
    return res; 
  }

  if ( arr.count == 2 ) {
    if ( el == arr[0] ) {
      indexEl = 0;
      return 0;
    }
    else
      if ( el == arr[1] ) {
        indexEl = 1;
        return 0;
      }
      else {
        Type * res = arr.InsertInd( 1, el );
        indexEl = 1;
        return res; 
      }
  }

  indexEl = 0;
  return 0;
}


//-------------------------------------------------------------------------------
// \ru найти объект в массиве \en find an object in the array 
// \ru поиск ведется методом половинных делений \en a search is performed by the bisection method 
// ---
template <class Type>
size_t find_in_array( const SSArray<Type> & arr, const Type & el )
{
  if ( arr.count > 11 ) {
    size_t mx = arr.count - 1;
    size_t mxc = mx;
    size_t mn = 0;

    while ( mn + 1 < mx ) {  // \ru пока не нашли - ищем \en seek until do not find 
      size_t md = ( mn + mx ) / 2;
      Type & mdE = arr[md];
      if ( mdE < el ) {
        if ( el == arr[mx] )
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
      else if ( el < mdE ) {
        if ( el == arr[mn] )
          return mn;
        mx = md;
      }
      else if ( mdE == el )
        return md;
      // \ru если попадаем сюда - значит некорректно написаны операторы "тождественно" и сравнения \en if we are here then operators of identity check and comparison are not correct 
      else {
        PRECONDITION( 0 );
        return SYS_MAX_T;
      }
    }

    if ( el < arr[0] )
      return SYS_MAX_T;
    if ( arr[mxc] < el )
      return SYS_MAX_T;
  }
  else {
    if ( arr.count == 1 )
      return el == arr[0] ? 0 : SYS_MAX_T;
    else { 
      if ( arr.count == 2 )
        return el == arr[0] ? 0 : ((el == arr[1]) ? 1 : SYS_MAX_T);
      else {
        // 2 < count <= 11
        for( size_t i = 0; i < arr.count; i++ )
          if ( el == arr[i] )
            return i;
      }
    }
  }

  return SYS_MAX_T;
}


//-------------------------------------------------------------------------------
// \ru найти объект в массиве \en find an object in the array 
// \ru поиск ведется методом половинных делений \en a search is performed by the bisection method 
// ---
template <class Type>
size_t find_from_array_spec( const SSArray<Type> & arr, const Type & el, bool & isPresent )
{
  isPresent = false;

  if ( !arr.count  || el < arr/*.parr*/[0] )
    return 0;

  size_t mx = arr.count - 1;

  if ( arr/*.parr*/[mx] < el )
    return mx + 1;

  if ( arr.count == 1 ) { // \ru значит *el == *parr[0] \en it means *el == *parr[0] 
    isPresent = true;
    return 0;
  }

  if ( arr.count == 2 ) { // \ru значит между 0  и 1 \en between 0 and 1 
    if ( el == arr/*.parr*/[0] ) {
      isPresent = true;
      return 0;
    }
    else {
      if (el == arr/*.parr*/[1]) isPresent = true;
      return 1;
    }
  }

  size_t mn = 0;

  while ( mn + 1 < mx ) {  // \ru пока не нашли - ищем \en seek until do not find 
    if ( el == arr/*.parr*/[mn] ) {
      isPresent = true;
      return mn;
    }
    else
      if (el == arr/*.parr*/[mx]) {
        isPresent = true;
        return mx;
      }
      else {
        size_t md = ( mn + mx ) / 2;
        if ( arr/*.parr*/[md] < el )
          mn = md;
        else if ( el < arr/*.parr*/[md] )
          mx = md;
        else if ( arr/*.parr*/[md] == const_cast<Type&>(el) ) {
          isPresent = true;
          return md;
        }
      }
  }

  return mx;
}


#endif // __TEMPL_SSARRAY_H
