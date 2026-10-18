////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Упорядоченный одномерный массив указателей.
         \en Ordered one-dimensional array of pointers. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_CSP_ARRAY_H
#define __TEMPL_CSP_ARRAY_H


#include <templ_sp_array.h>


FORVARD_DECL_TEMPLATE_TYPENAME( class CSPArray );
FORVARD_DECL_TEMPLATE_TYPENAME( void qp_sort( CSPArray<Type> & arr ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader & CALL_DECLARATION operator >> ( reader & in,        CSPArray<Type> & ref ) );         
FORVARD_DECL_TEMPLATE_TYPENAME( writer & CALL_DECLARATION operator << ( writer & out, const CSPArray<Type> & ref ) );  


//------------------------------------------------------------------------------
/** \brief \ru Упорядоченный одномерный массив.
           \en Ordered one-dimensional array. \~
  \details \ru Упорядоченный одномерный массив указателей. \n
    У объектов массива должны быть операторы "==" и "<".
    Имеется возможность добавлять несортированные данные через функцию AddNoSort,
    но при первом обращении к функциям Add и Find произойдет сортировка
    Одинаковые объекты не добавляются.
           \en Ordered one-dimensional array of pointers. \n
    Elements of the array should have operators "==" and "<".
    The unsorted data can be added by the function 'AddNoSort',
    but the sorting starts after the first call of functions Add and Find.
    The similar objects are not added. \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class CSPArray : protected SPArray<Type> {
public:
  typedef bool (*LessFuncPtr)( const Type &, const Type & ); ///< \ru Тип указателя на функцию выбора удаляемого элемента из двух одинаковых. \en The type of pointer to selection function of the item to remove from the two identical.

private:
  bool        m_sort;     ///< \ru Признак сортированности массива. \en Attribute of an array being sorted. 
  bool        m_keepEq;   ///< \ru Признак запрет удаления одинаковых элементов. \en Attribute of forbiddance to delete similar elements. 
  LessFuncPtr m_lessFunc; ///< \ru Функция выбора удаляемого элемента из двух одинаковых. \en The function of selecting the item to remove from the two identical.

public:  

  /// \ru Конструктор. \en Constructor. 
  CSPArray( size_t maxCnt = 0, uint16 delt = 1, bool shouldDelete = true, bool _keepEq = false, LessFuncPtr func = nullptr )
    : SPArray<Type>( maxCnt, delt, shouldDelete )
    , m_sort( true )
    , m_keepEq( _keepEq )
    , m_lessFunc( func ) 
  {}

  using SPArray<Type>::OwnsElem;
  using SPArray<Type>::Delta;
  using SPArray<Type>::Flush;
  using SPArray<Type>::Upper;
  using SPArray<Type>::HardFlush;
  using SPArray<Type>::Adjust;
  using SPArray<Type>::RemoveInd;
  using SPArray<Type>::Count;
  using SPArray<Type>::MaxIndex;
  using SPArray<Type>::operator[];
  using SPArray<Type>::Reserve;
  using SPArray<Type>::SetSize;
  using SPArray<Type>::GetLast;

  using SPArray<Type>::empty;
  using SPArray<Type>::size;
  using SPArray<Type>::reserve;
  using SPArray<Type>::capacity;
  using SPArray<Type>::begin;
  using SPArray<Type>::end;
  using SPArray<Type>::front;
  using SPArray<Type>::back;
  using SPArray<Type>::clear;
  using SPArray<Type>::shrink_to_fit;
  /// \ru Задать метод выбора удаляемого элемента из двух одинаковых. \en Set the selection method of the item to remove from the two identical.
  void    SetLessFunc( LessFuncPtr func ) { m_lessFunc = func; }
  /// \ru Добавить массив без сортировки. \en Add array without sorting. 
  bool    AddArray( const RPArray<Type> & arr ) { m_sort = false; return SPArray<Type>::AddArraySimple( arr ); }
  /// \ru Добавить элемент без сортировки \en Add element without sorting. 
  void    AddNoSort( Type  * ent ) { SPArray<Type>::AddSimple( ent ); m_sort = false; } 
  Type  * Add ( Type * );                                    ///< \ru Добавить элемент с упорядочиванием по массиву \en Add element with sorting. 
  Type  * Add ( Type *, size_t & indexEnt );                 ///< \ru Добавить элемент с упорядочиванием по массиву, возвращает индекс \en Add element with sorting, returns index of the element. 
  size_t  Find( const Type * );                              ///< \ru Найти элемент в упорядоченном массиве \en Find an element in ordered array. 
  void    Sort();                                            ///< \ru Сортировать массив, если не сортирован \en Sort array if it is not sorted 
  Type *  RemoveObj( Type * delObject, DelType del );        ///< \ru Удалить элемент. \en Delete element. 

  TEMPLATE_FRIEND void qp_sort TEMPLATE_SUFFIX ( CSPArray<Type> & arr );
  TEMPLATE_FRIEND reader & CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader & in,        CSPArray<Type> & ref );
  TEMPLATE_FRIEND writer & CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer & out, const CSPArray<Type> & ref );

private:
  /// \ru Сортировка с выбором удаляемого элемента из двух одинаковых. \en Sort with the choice of the element to remove from the two identical.
  void Sort( LessFuncPtr isLess );
  void operator = ( const CSPArray<Type> & ); // \ru запрещено !!! \en forbidden !!! 
  CSPArray( const CSPArray & other );         // \ru запрещено !!! \en forbidden !!! 
};


//-------------------------------------------------------------------------------
// \ru добавление объекта в массив \en adding an object to array 
// ---
template <class Type>
inline Type * CSPArray<Type>::Add( Type * el ) 
{
  //::qp_sort( *this );
  Sort();
  return SPArray<Type>::Add( el );
}


//-------------------------------------------------------------------------------
// \ru добавление объекта в массив \en adding an object to array 
// ---
template <class Type>
inline Type * CSPArray<Type>::Add( Type * el, size_t & indexEl ) 
{
  //::qp_sort( *this );
  Sort();
  return SPArray<Type>::Add( el, indexEl );
}


//-------------------------------------------------------------------------------
// \ru поиск объекта в массиве \en search of an element in array 
// ---
template <class Type>
inline size_t CSPArray<Type>::Find( const Type * el )
{
  // ::qp_sort( *this );
  Sort();
  return SPArray<Type>::Find( el );
}


//-------------------------------------------------------------------------------
// \ru сортировать массив, если не сортирован \en sort array if it is not sorted 
// ---
template <class Type>
inline void CSPArray<Type>::Sort() 
{
  if ( m_lessFunc )
    Sort( m_lessFunc );
  else
    qp_sort( *this );
}

//-------------------------------------------------------------------------------
// \ru Сортировать массив с проверкой, какой из двух одинаковых элементов удалять. Для проверки используется сравнительная функция isLess.
// \en Sort an array with checking which of the two identical items to remove. Comparative function isLess used for the checking.
// ---
template <class Type>
inline void CSPArray<Type>::Sort( LessFuncPtr lessFunc )
{
  if ( !m_sort ) 
  {
    if ( Count() > 1 ) 
    {
      qp_sort_r( *this );

      // \ru удаление одинаковых \en deletion of similar objects 
      if ( !m_keepEq ) {
        for ( ptrdiff_t i = MaxIndex(); i >= 1; i-- ) {
          if ( *operator[](i) == *operator[](i-1) ) {
            if ( !lessFunc || lessFunc(*operator[](i), *operator[](i-1)) )
              RemoveInd( i );
            else
              RemoveInd( i - 1 );
          }
        }
      }
    }

    m_sort = true;
  }
}


//------------------------------------------------------------------------------
// \ru удалить элемент из массива (по указателю) \en delete an element from array (by the pointer) 
// ---
template <class Type>
inline Type * CSPArray<Type>::RemoveObj( Type * delObject, DelType del )
{
  C3D_ASSERT( SPArray<Type>::nowDeletedElem  == nullptr ); // \ru Временно, для отладки \en Temporarily, for debugging.
  size_t i = Find( delObject );
  return ( i != SYS_MAX_T ) ? RemoveInd(i, del) : nullptr;
}


//-----------------------------------------------------------------------------
// \ru Н.Вирт "Алгоритмы и структуры данных" 2е издание, Санкт-Петербург, 2001г., стр.111 \en see N.Wirth "Algorithms and Data Structures" 
// ---
template <class Type>
//OV_x64 void qp_sort_r( CSPArray<Type> & arr, ptrdiff_t /*OV_x64 int*/ minInd, ptrdiff_t /*OV_x64 int*/ maxInd ) 
void qp_sort_r( CSPArray<Type> & arr, size_t minInd = SYS_MAX_T, size_t maxInd = SYS_MAX_T ) 
{
//OV_x64 =====================
  if ( arr.Count() > 1 )
  {
    if ( minInd == SYS_MAX_T ) minInd = 0;
    if ( maxInd == SYS_MAX_T || maxInd > (size_t)arr.MaxIndex() ) maxInd = (size_t)arr.MaxIndex(); // \ru проверено count > 0 \en checked that count > 0 
//OV_x64 =====================
    size_t i = minInd, j = maxInd; // \ru OV_x64 приводить к знаковому значению будем только в операторах > и < \en OV_x64 cast to signed value only in operators > and <  
    size_t im = (i + j)/2;         // \ru OV_x64 приводить к знаковому значению будем только в операторах > и < \en OV_x64 cast to signed value only in operators > and <  

    Type * middle = arr[im];

    do {
      while( *arr[i] < *middle )
        i++;
      while( *middle < *arr[j] )
        j--;

      if ( (ptrdiff_t)i <= (ptrdiff_t)j ) {
        if ( i != j ) {
          Type * wi = arr[i];
          arr[i] = arr[j];
          arr[j] = wi;
        }
        i++;
        j--;
      }
    } while( !((ptrdiff_t)i > (ptrdiff_t)j) );


    if ( (ptrdiff_t)minInd < (ptrdiff_t)j )
      qp_sort_r( arr, minInd, j );
    if ( (ptrdiff_t)i < (ptrdiff_t)maxInd )
      qp_sort_r( arr, i, maxInd );
  }
}

template <class Type>
void qp_sort_r2( Type ** arr, size_t minIndex, size_t maxIndex )
{
  ptrdiff_t rangeSize = (ptrdiff_t)maxIndex - (ptrdiff_t)minIndex;
  if ( rangeSize == 1 ) {
    if ( arr[maxIndex] < arr[minIndex] ) {
      Type *wi = arr[maxIndex];
      arr[maxIndex] = arr[minIndex];
      arr[minIndex] = wi;
    }
  }
  else if ( rangeSize > 1 ) {
    c3d::NumbersPair iterStack[30];
    int stackCount = -1;

    ptrdiff_t minInd = minIndex, maxInd = maxIndex;
    ptrdiff_t i = minInd, j = maxInd;
    ptrdiff_t im = 0;
    Type *middle = nullptr;

    for ( ;; ) {
      i = minInd, j = maxInd;
      im = ( i + j ) / 2;
      middle = arr[im];

      do {
        while ( *arr[i] < *middle ) i++;
        while ( *middle < *arr[j] ) j--;
        if ( i <= j ) {
          if ( i != j ) {
            Type * wi = arr[i];
            arr[i] = arr[j];
            arr[j] = wi;
          }
          i++;
          j--;
        }
      } while ( !( i > j ) );

      if ( j - minInd > maxInd - i ) {
        if ( minInd < j ) {
          iterStack[++stackCount].first = minInd;
          iterStack[stackCount].second = j;
        }

        if ( i < maxInd ) {
          minInd = i;
          continue;
        }
      }
      else {
        if ( i < maxInd ) {
          iterStack[++stackCount].first = i;
          iterStack[stackCount].second = maxInd;
        }

        if ( minInd < j ) {
          maxInd = j;
          continue;
        }
      }

      if ( stackCount < 0 )
        break; // \ru Все подмассивы обработаны. \en All subarrays are done.
      minInd = iterStack[stackCount].first;
      maxInd = iterStack[stackCount--].second;
    }
  }
}


template <class Type>
void qp_sort( CSPArray<Type> & arr )
{
  if ( !arr.m_sort ) {
    if ( arr.Count() > 1 ) {
      // ptrdiff_t maxIndex = arr.MaxIndex(); // проверено count > 1 \en // verified that count > 1 
      //OV_x64qp_sort_r( arr, 0, maxIndex );
      qp_sort_r2<Type>( arr.begin(), 0, arr.Count() - 1 ); // C3D-1211
      //qp_sort_r( arr );

      // \ru удаление одинаковых \en deletion of similar objects 
      if ( !arr.m_keepEq ) {
        for ( ptrdiff_t i = arr.MaxIndex(); i >= 1; i-- ) { // \ru OV_x64 проверено что maxIndex > 0 \en OV_x64 verified that maxIndex > 0 
          if ( *arr[i] == *arr[i-1] )
            arr.RemoveInd( i );
        }
      }
    }

    arr.m_sort = true;
  }
}


#endif // __TEMPL_CSP_ARRAY_H
