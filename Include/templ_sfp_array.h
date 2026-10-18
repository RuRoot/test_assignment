////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Упорядоченный массив указателей.
         \en Ordered array of pointers. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_SFP_ARRAY_H
#define __TEMPL_SFP_ARRAY_H


#include <templ_p_array.h>
#include <templ_s_array.h>


////////////////////////////////////////////////////////////////////////////////
//
// \ru упорядоченный массив указателей \en ordered array of pointers 
// \ru одинаковые объекты не добавляются \en the similar objects are not added 
//
////////////////////////////////////////////////////////////////////////////////
FORVARD_DECL_TEMPLATE_TYPENAME( class SFPArray );
FORVARD_DECL_TEMPLATE_TYPENAME( Type  *              add_to_array          (       SFPArray<Type> &,       Type * ent, size_t & indexEnt ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t               find_from_array       ( const SFPArray<Type> &, const Type * ent ) );
FORVARD_DECL_TEMPLATE_TYPENAME( Type *               find_from_array_by_key( const SFPArray<Type> &,       void * key, size_t & index ) );
FORVARD_DECL_TEMPLATE_TYPENAME( void                 qp_sort               (       SFPArray<Type> &, bool always ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader & CALL_DECLARATION operator >> ( reader & in,        SFPArray<Type> & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer & CALL_DECLARATION operator << ( writer & out, const SFPArray<Type> & ref ) );


template <class Type>
class  SFPArray : private PArray<Type> {
public:
typedef  int   (*CompareFunc)( const Type * left, const Type * right );  // \ru функция сравнения для поиска и наполнения \en a comparison function for search and filling 
typedef  int   (*SearchFunc) ( const Type * d1, void * key );              // \ru функция сравнения для поиска \en a comparison function for search 
typedef  void  (*AssignFunc) ( Type *& oldObj, const Type * newObj );     // \ru функция присвоения \en an assignment function 

  CompareFunc   fCompare_m; // \ru функция сравнения \en a comparison function 
  SearchFunc    fSearch_m;  // \ru функция поиска \en a search function 

protected:
  // \ru признак сортированности массива \en attribute of an array being sorted 
  bool m_sort;
  // \ru при сортировке выкидывать одинаковые \en throw out the same elements while sorting 
  bool m_keepEq;


public :
  SFPArray( size_t      maxCnt
          , uint16      delt
          , CompareFunc fc
          , SearchFunc  fs
          , bool        shouldDelete 
          , bool        keepEq       
          )
    : PArray<Type>( maxCnt, delt, shouldDelete ) 
    , fCompare_m  ( fc ) // \ru функция сравнения \en a comparison function 
    , fSearch_m   ( fs ) // \ru функция поиска \en a search function 
    , m_sort      ( true )
    , m_keepEq    ( keepEq )
  {}

  using PArray<Type>::OwnsElem;
  using PArray<Type>::Delta;
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

  using PArray<Type>::empty;
  using PArray<Type>::size;
  using PArray<Type>::reserve;
  using PArray<Type>::capacity;
  using PArray<Type>::begin;
  using PArray<Type>::end;
  using PArray<Type>::front;
  using PArray<Type>::back;
  using PArray<Type>::clear;

  Type  *                Add( Type * );                 // \ru добавить элемент с упорядочиванием по массиву \en add element with sorting 
  Type  *                Add( Type *, size_t & indexEnt );// \ru добавить элемент с упорядочиванием по массиву, возвращает индекс \en add element with sorting, returns index of the element. 
  void                   AddSimple( Type * ent ) { m_sort = false; PArray<Type>::Add( ent ); } // \ru Доступ к функции базового класса - добавить элемент в конец массива \en An access to the function of the base class - add an element to the end of the array 
  // \ru найти элемент в упорядоченном массиве \en find an element in ordered array 
  size_t                 Find( const Type * );
  // \ru найти элемент по ключевым полям \en find an element by the key fields 
  Type *                 FindByKey( void * key, size_t & index );    
  // \ru удалить элемент из массива \en delete an element from array 
  Type  *                RemoveObj( Type * delObject, DelType=defDelete );   
  // \ru true если элемент найден \en true if the element was found 
  bool                   IsExist( const Type * );                
  // \ru сортировать массив, если не сортирован \en sort array if it is not sorted 
  void                   Sort();                          
  // \ru проверить на соответствие сортировке элемент массива \en check that an element of the array corresponds to the sorting 
  // \ru если меняли значение полей Type - условие сортировки может быть не выполнено \en if the values of fields Type have been changed then the comparison condition may be not performed 
  // \ru если это так - флаг сортировки у массива снимается \en if it is so then the flag of sorting is switched off 
  void                   SortCheckByIndex( size_t index ); 

  TEMPLATE_FRIEND Type  *              add_to_array           TEMPLATE_SUFFIX (       SFPArray<Type> &,       Type * ent, size_t & indexEnt );
  TEMPLATE_FRIEND size_t               find_from_array        TEMPLATE_SUFFIX ( const SFPArray<Type> &, const Type * ent );
  TEMPLATE_FRIEND Type *               find_from_array_by_key TEMPLATE_SUFFIX ( const SFPArray<Type> &,       void * key, size_t & index );
  TEMPLATE_FRIEND void                 qp_sort                TEMPLATE_SUFFIX (       SFPArray<Type> &, bool always );

private:
  OBVIOUS_PRIVATE_COPY( SFPArray );

  // \ru Т.к. наследование private, то сделаем здесь операторы чтения-записи \en Since there is a private inheritance, then make here operators of reading/writing 
//ID K8  KNOWN_OBJECTS_RW_REF_OPERATORS( SFPArray<Type> )
  // \ru OV_x64 но коду не нашел реализации операторов чтения/записи \en OV_x64 implementation of reading/writing operators was not found 
  TEMPLATE_FRIEND reader & CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader & in,        SFPArray<Type> & ref );
  TEMPLATE_FRIEND writer & CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer & out, const SFPArray<Type> & ref );

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
inline void * SFPArray<Type>::operator new( size_t size ) {
  return ::Allocate( size, typeid(SFPArray<Type>).name() );
}

//------------------------------------------------------------------------------
// \ru Перегрузка оператора delete. \en Overloading of the "delete" operator. 
// ---
template <class Type>
inline void SFPArray<Type>::operator delete( void * ptr, size_t size ) {
  ::Free( ptr, size, typeid(SFPArray<Type>).name() );
}
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_


//-------------------------------------------------------------------------------
// \ru добавить элемент с упорядочиванием по массиву \en add element with sorting 
// ---
template <class Type>
inline Type* SFPArray<Type>::Add( Type* el ) {
  size_t index;
  qp_sort( *this, false );
  return add_to_array( *this, el, index );
}


//-------------------------------------------------------------------------------
// \ru добавить элемент с упорядочиванием по массиву, возвращает индекс \en add element with sorting, returns index of the element 
// ---
template <class Type>
inline Type* SFPArray<Type>::Add( Type* el, size_t & indexEl ) {
  qp_sort( *this, false );
  return add_to_array( *this, el, indexEl );
}


//-------------------------------------------------------------------------------
// \ru поиск объекта в массиве \en search of an element in array 
// ---
template <class Type>
inline size_t SFPArray<Type>::Find( const Type * el ) {
  qp_sort( *this, false );
  return find_from_array( *this, el );
}
  

//-------------------------------------------------------------------------------
// \ru найти элемент по ключевым полям \en find an element by the key fields 
// ---
template <class Type>
inline Type * SFPArray<Type>::FindByKey( void * key, size_t & index ) {
  qp_sort( *this, false );
  return find_from_array_by_key( *this, key, index );
}


//------------------------------------------------------------------------------
// \ru удалить элемент из массива \en delete an element from array 
// ---
template <class Type>
inline Type * SFPArray<Type>::RemoveObj( Type *delObject, DelType del ) {
  PRECONDITION( PArray<Type>::nowDeletedElem  == nullptr ); // \ru Временно, для отладки \en Temporarily, for debugging
  qp_sort( *this, false );
  size_t i = find_in_array( *this, delObject );
  return ( i != SYS_MAX_T ) ? RemoveInd(i, del) : nullptr;
}


//------------------------------------------------------------------------------
// \ru true если элемент найден \en true if the element was found 
// ---
template <class Type>
inline bool SFPArray<Type>::IsExist( const Type * el ) {
  qp_sort( *this, false );
  return find_in_array( *this, el ) != SYS_MAX_T;
}


//-------------------------------------------------------------------------------
// \ru сортировать массив, если не сортирован \en sort array if it is not sorted 
// ---
template <class Type>
inline void SFPArray<Type>::Sort() {
  qp_sort( *this, true );
}


//------------------------------------------------------------------------------
// \ru проверить на соответствие сортировке элемент массива \en check that an element of the array corresponds to the sorting 
// \ru если меняли значение полей Type - условие сортировки может быть не выполнено \en if the values of fields Type have been changed then the comparison condition may be not performed 
// \ru если это так - флаг сортировки у массива снимается \en if it is so then the flag of sorting is switched off 
// ---
template <class Type>
inline void SFPArray<Type>::SortCheckByIndex( size_t /*OV_x64 int*/ index ) {
  if ( Count() )
  {
    size_t /*OV_x64 int*/ maxIndex = Count() - 1; // \ru проверено count > 0 \en chacked that count > 0 
    //if ( 0 <= index && index <= maxIndex ) OV - Expression '0 <= index' is always true. Unsigned type value is always >= 0.
    if ( index <= maxIndex ) 
    {
      Type * el = (*this)[index];
      // \ru проверяем середину интервала \en check the middle of the interval 
      if ( 0 < index && index < maxIndex ) {
        if ( (*fCompare_m)( (*this)[index-1], el ) >= 0 
             ||   
             (*fCompare_m)( el, (*this)[index+1] ) >= 0 
           )
          m_sort = false;
      }
      else 
        // \ru левая граница \en the left boundary 
        if ( index == 0 ) {
          if ( (*fCompare_m)( el, (*this)[1] ) >= 0 )
            m_sort = false;

        }
        else //index == maxIndex
          // \ru правая граница \en the right boundary 
          if ( (*fCompare_m)( (*this)[index-1], el ) >= 0 )
            m_sort = false;
    }
  }
}

  
//-------------------------------------------------------------------------------
// \ru добавление объекта в массив с упорядочиванием \en adding an object to the array with ordering 
// ---
template <class Type>
Type * add_to_array( SFPArray<Type> & arr, Type * el, size_t & indexEl ) {
  size_t mr = arr.count - 1;
  size_t mxc = mr; // \ru правое \en right 
  size_t ml = 0;   // \ru левое \en left 
  int resCmp = 0;
  size_t mc = 0;

  // \ru специальные случаи \en special cases 
  // \ru массив пустой - просто добавляем \en the array is empty - simply add 
  // \ru проверку на пустоту массива нельзя переносить за проверку границ - \en a check for array emptyness should not be moved outside a boundary check -  
  // \ru можно вылететь (из-за mxc = -1) \en the error may occur (because of mxc = -1) 
  if ( !arr.count ) {
    arr.PArray<Type>::Add( el );
    indexEl = 0;
    return arr[0]; //arr.parr;
  }

  // \ru SA K6 элемент за границами массива - добавляем \en SA K6 an element is out of bounds - add it 
  resCmp = (*arr.fCompare_m)( el, arr[0] );
  if ( resCmp < 0 ) {
    arr.Insert( 0, el );
    indexEl = 0;
    return arr[0]; //arr.parr;
  }
  else 
    if ( 0 == resCmp ) {
      indexEl = 0;
      return 0;
    }

  // \ru есть всего один \en only one exists 
  if ( arr.count == 1 ) { 
   arr.PArray<Type>::Add( el );
   indexEl = 0;
   return arr[1];
  }

  // \ru проверка правой границы \en a check of the right bound 
  resCmp = (*arr.fCompare_m)( arr[mxc], el );
  if ( resCmp < 0 ) {
    arr.PArray<Type>::Add( el );
    indexEl = mxc + 1;
    return arr[indexEl]; //arr.parr + indexEl;
  }
  else 
    if ( 0 == resCmp ) {
      indexEl = mxc;
      return 0;
    }

  // \ru общий случай - элементов больше двух \en the common case - the number of elements is more than two 
  if ( arr.count > 2 ) {
    // \ru результаты сравнения элементов с левой и правой границей \en the results of comparison between the elements and bounds 
    // \ru сделаны для минимизации вызовов функций сравнений - они могут \en it is done in order to minimize the number of comparison functions calls - they may 
    // \ru быть дорогими по времени выполнения \en be too time-consuming 
    // \ru пока не нашли - ищем \en seek until do not find 
    while ( ml + 1 < mr ) {
      // \ru ищем СРЕДНИЙ индекс \en seek for the MIDDLE index 
      mc = ( ml + mr ) / 2;
      // \ru сравниваем СРЕДНЕЕ с присланным \en compare the MIDDLE element and the sent element 
      resCmp = (*arr.fCompare_m)( arr[mc], el );
      if ( resCmp > 0 ) { // \ru mc БОЛЬШЕ пришедшего, но mn МЕНЬШЕ пришедшего - поиск слева \en 'mc' is GREATER than the sent one and 'mn' is LESS than the sent one - search on the left 
        mr = mc;
      }
      else if ( resCmp < 0 ) { // \ru mc МЕНЬШЕ пришедшего - поиск справа \en 'mc' is LESS than the sent one - search on the right 
        ml = mc;
      }
      else {
        // \ru такой элемент уже есть - добавления не происходит \en such element already exists - nothing to add 
        indexEl = mc;
        return 0;
      }

    } // end while

    arr.Insert( mr, el );
    indexEl = mr;
    return arr[mr];//arr.parr + mr;
  }

  // \ru SA K6 элемент не за границами массива и массив содержит 2 элемента \en SA K6 an element is not outside the array's bounds and the array consists of 2 elements 
  if ( arr.count == 2 ) {
    arr.Insert( 1, el );
    indexEl = 1;
    return arr[1]; // arr.parr + 1;
  }

  indexEl = 0;
  return 0;
}


//-------------------------------------------------------------------------------
// \ru поиск объекта в массиве \en search of an element in array 
// \ru поиск ведется методом половинных делений \en a search is performed by the bisection method 
// ---
template <class Type>
size_t find_from_array( const SFPArray<Type> & arr, const Type * el ) 
{
  C3D_ASSERT( arr.fCompare_m ); // \ru без функции сравнения массив бессмысленнен \en the array is useless without comparison function 
  // \ru общий случай - элементов больше двух \en the common case - the number of elements is more than two 
  int res = 0;

  if ( arr.count > 11 ) 
  {
    size_t mr = arr.count - 1; // \ru проверено count >= 1 \en count >= 1 validated 
    size_t mxc = mr;
    size_t ml = 0;

    while ( ml + 1 < mr ) {  // \ru пока не нашли - ищем \en seek until do not find 
      size_t mc = ( ml + mr ) / 2;
      res = (*arr.fCompare_m)( arr[mc], el );
      switch ( res ) {
        case 1  : mr = mc; break;
        case -1 : ml = mc; break;
        case 0  : return mc;
      }
    }

    // \ru проверка по границам \en check by bounds 
    res = (*arr.fCompare_m)( arr[0], el );
    switch ( res ) {
      case 0 : return 0;
      case 1 : return SYS_MAX_T;
      case -1 : break;
    }

    res = (*arr.fCompare_m)( arr[mxc], el );
    switch ( res ) {
      case 0  : return mxc;
      default : return SYS_MAX_T;
    }
  }
  else {
    // \ru специальные случаи count ==   >0 & < 11 \en special cases  count ==   >0 & < 11 
    for( size_t i = 0, count = (size_t)arr.count; i < count; i++ ) {
      switch ( (*arr.fCompare_m)( arr[i], el ) ) {
        case 0  : return i;
        case 1  : return SYS_MAX_T;
        case -1 : break;
      }
    }
  }

  return SYS_MAX_T;
}


//-------------------------------------------------------------------------------
//
// ---
template <class Type>
Type * find_from_array_by_key( const SFPArray<Type> & arr, void * key, size_t & index )
{
  PRECONDITION( arr.fSearch_m && key ); // \ru без функции сравнения массив бессмысленнен \en the array is useless without comparison function 
  // \ru общий случай - элементов больше двух \en the common case - the number of elements is more than two 
  int res = 0;
  if ( arr.count > 11 ) {
    size_t mr = arr.count - 1; // \ru проверено count >= 1 \en count >= 1 validated 

    size_t mxc = mr;
    size_t ml = 0;

    while ( ml + 1 < mr ) {  // \ru пока не нашли - ищем \en seek until do not find 
      size_t mc = ( ml + mr ) / 2;
      res = (*arr.fSearch_m)( arr[mc], key );
      switch ( res ) {
        case 1  : mr = mc; break;
        case -1 : ml = mc; break;
        case 0  : index = mc; return arr[mc];
      }
    }

    // \ru проверка по границам \en check by bounds 
    res = (*arr.fSearch_m)( arr[0], key );
    switch ( res ) {
      case 0 : index = 0; return arr[0];
      case 1 : index = SYS_MAX_T; return nullptr;
      case -1 : break;
    }

    res = (*arr.fSearch_m)( arr[mxc], key );
    switch ( res ) {
      case 0  : index = mxc; return arr[mxc];
      default : index = SYS_MAX_T; return nullptr;
    }
  }
  else {
    // \ru специальные случаи count ==   >0 & < 11 \en special cases  count ==   >0 & < 11 
    for( size_t i = 0, count = (size_t)arr.count; i < count; i++ ) {
      switch ( (*arr.fSearch_m)( arr[i], key ) ) {
        case 0  : index = i; return arr[i];
        case 1  : index = SYS_MAX_T; return nullptr;
        case -1 : break;
      }
    }
  }

  index = SYS_MAX_T;
  return nullptr;
}


//-----------------------------------------------------------------------------
// \ru Н.Вирт "Алгоритмы и структуры данных" 2е издание, Санкт-Петербург, 2001г., стр.111 \en see N.Wirth "Algorithms and Data Structures" 
// ---
template <class Type>
void qp_sort_r( SFPArray<Type> & arr, ptrdiff_t minInd, ptrdiff_t maxInd )
{
//OV_x64 =====================
  if ( arr.Count() > 1 )
  {
    if ( minInd == -1 ) minInd = 0;
    if ( maxInd == -1 || maxInd > arr.MaxIndex() ) maxInd = arr.MaxIndex(); // \ru проверено count > 0 \en chacked that count > 0 
//OV_x64 =====================

    ptrdiff_t i = minInd, j = maxInd;
    ptrdiff_t im = (i + j)/2;

    Type * middle = arr[im];

    do {
      while( (*arr.fCompare_m)( arr[i], middle ) < 0 ) i++;
      while( (*arr.fCompare_m)( middle, arr[j] ) < 0 ) j--;
      if ( i <= j ) {
        if ( i != j ) {
          Type * wi = arr[i];
          arr[i] = arr[j];
          arr[j] = wi;
        }
        i++;
        j--;
      }
    } while( !(i > j) );


    if ( minInd < j )
      qp_sort_r( arr, minInd, j );
    if ( i < maxInd )
      qp_sort_r( arr, i, maxInd );
  }
}


//-------------------------------------------------------------------------------
//
// ---
template <class Type>
void qp_sort( SFPArray<Type> & arr, bool always )
{
  if ( !arr.m_sort || always ) {
//OV_x64 =====================
    if ( arr.Count() > 1 )
    {
//OV_x64 =====================
      size_t maxIndex = arr.MaxIndex(); // \ru проверено count > 1 \en chacked that count > 1 
      qp_sort_r( arr, 0, maxIndex );
      // \ru удаление одинаковых \en deletion of similar objects 
      if ( !arr.m_keepEq )
        for ( size_t /*OV_x64 int*/ i = maxIndex; i >= 1; i-- ) { // maxIndex > 0
          int resCmp = (*arr.fCompare_m)( arr[i], arr[i-1] );
          switch ( resCmp ) {
            case 1  : break;
            case 0  : arr.RemoveInd( i ); break;
            case -1 : PRECONDITION( false ); break;
          }
        }
    }

    arr.m_sort = true;
  }
}


#endif // __TEMPL_SFP_ARRAY_H
