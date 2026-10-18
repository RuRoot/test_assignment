////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Упорядоченный одномерный массив объектов.
         \en Ordered one-dimensional array of objects. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_CSS_ARRAY_H
#define __TEMPL_CSS_ARRAY_H


#include <templ_ss_array.h>
#include <utility>


FORVARD_DECL_TEMPLATE_TYPENAME( class CSSArray );
FORVARD_DECL_TEMPLATE_TYPENAME( void q_sort( CSSArray<Type> & arr, SArray<Type> * del ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader & CALL_DECLARATION operator >> ( reader & in, CSSArray<Type> & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer & CALL_DECLARATION operator << ( writer & out, const CSSArray<Type> & ref ) );


//------------------------------------------------------------------------------
/** \brief \ru Упорядоченный одномерный массив.
           \en Ordered one-dimensional array. \~
  \details \ru Упорядоченный одномерный массив объектов. \n
    У объектов массива должны быть операторы "==" и "<".
    Имеется возможность добавлять несортированные данные через функцию AddNoSort,
    но при первом обращении к функциям Add и Find произойдет сортировка
    Одинаковые объекты не добавляются.
           \en Ordered one-dimensional array of objects. \n
    Elements of the array should have operators "==" and "<".
    The unsorted data can be added by the function 'AddNoSort',
    but the sorting starts after the first call of functions Add and Find.
    The similar objects are not added. \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class CSSArray : protected SSArray<Type> {  
  bool m_sort; ///< \ru Признак сортированности массива. \en Attribute of an array being sorted. 

public:
  /// \ru Конструктор. \en Constructor. 
  CSSArray( size_t maxCnt = 0, uint16 delt = 1 )  
    : SSArray<Type>( maxCnt, delt )
    , m_sort( true )   
  {}
  /// \ru Конструктор копирования. \en Copy constructor. 
  CSSArray( const CSSArray<Type> & other )    
    : SSArray<Type>( other )
    , m_sort( other.m_sort )  
  {}
  /// \ru Конструктор копирования. \en Copy constructor. 
  CSSArray( const SArray<Type> & other, SArray<Type> * del = nullptr )
    : SSArray<Type>( other )
    , m_sort( false )  
  {
    q_sort( *this, del );
  }
  /// \ru Конструктор копирования. \en Copy constructor. 
  CSSArray( const SArray< std::pair<Type,Type> > & other, bool addFirst, SArray<Type> * del = nullptr )
    : SSArray<Type>( other.Count(), 1 )
    , m_sort( false )  
  {
    if ( addFirst ) {
      for ( size_t k = 0, cnt = other.Count(); k < cnt; k++ )
        AddNoSort( other[k].first );
    }
    else {
      for ( size_t k = 0, cnt = other.Count(); k < cnt; k++ )
        AddNoSort( other[k].second );
    }
    q_sort( *this, del );
  }

  using SSArray<Type>::Flush;
  using SSArray<Type>::HardFlush;
  using SSArray<Type>::Adjust;
  using SSArray<Type>::operator[];
  using SSArray<Type>::Remove;
  using SSArray<Type>::RemoveInd;
  using SSArray<Type>::Count;
  using SSArray<Type>::MaxIndex;
  using SSArray<Type>::GetAddr;
  using SSArray<Type>::GetEndAddr;
  using SSArray<Type>::Reserve;
  using SSArray<Type>::SetSize;
  using SSArray<Type>::SetMaxDelta;

  using SSArray<Type>::empty;
  using SSArray<Type>::size;
  using SSArray<Type>::reserve;
  using SSArray<Type>::capacity;
  using SSArray<Type>::front;
  using SSArray<Type>::back;
  using SSArray<Type>::clear;

  using SSArray<Type>::begin;
  using SSArray<Type>::end;

  void                    AddNoSort( const Type & ent ) { SSArray<Type>::AddSimple( ent ); m_sort = false; } ///< \ru Добавить элемент без сортировки. \en Add element without sorting. 
  Type  *                 Add ( const Type & );                    ///< \ru Добавить элемент с упорядочиванием по массиву. \en Add element with sorting. 
  Type  *                 Add ( const Type &, size_t & indexEnt ); ///< \ru Добавить элемент с упорядочиванием по массиву, возвращает индекс. \en Add element with sorting by array, returns index of the element. 
  size_t                  Find( const Type & );                    ///< \ru Найти элемент в упорядоченном массиве. \en Find an element in ordered array. 
  void                    Sort( SArray<Type> * del = nullptr );       ///< \ru Выполнить сортировку элементов массива. \en Sort elements of array.
  size_t                  RemoveObj( const Type & delObject );     ///< \ru Удалить элемент из массива. \en Delete an element from array. 
  void                    SetNoSort() { m_sort = false; }          ///< \ru Сбросить флаг сортированности. \en Reset the flag of being sorted. 

  void                    AddArray( const CSSArray<Type> & arr, bool doSort ); ///< \ru Добавить массив. \en Add array 
  void                    AddArray( const SArray<Type>   & arr, bool doSort ); ///< \ru Добавить массив. \en Add array 
  bool                    IsSorted() const {return m_sort; } ///< \ru Вернуть вризнак сортированности массива. \en Return an attribute of an array being sorted. 

  TEMPLATE_FRIEND void    q_sort TEMPLATE_SUFFIX ( CSSArray<Type> & arr, SArray<Type> * del );

  TEMPLATE_FRIEND reader & CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader & in, CSSArray<Type> & ref );
  TEMPLATE_FRIEND writer & CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer & out, const CSSArray<Type> & ref );
  // Intel Compiler 12 // KNOWN_OBJECTS_RW_REF_OPERATORS( CSSArray<Type> )
};


//-------------------------------------------------------------------------------
// \ru добавление объекта в массив \en adding an object to array 
// ---
template <class Type>
inline Type * CSSArray<Type>::Add( const Type & el ) {
  q_sort( *this, (SArray<Type> *)nullptr );
  return SSArray<Type>::Add( el );
}


//-------------------------------------------------------------------------------
// \ru добавление объекта в массив \en adding an object to array 
// ---
template <class Type>
inline Type * CSSArray<Type>::Add( const Type & el, size_t & indexEl ) {
  q_sort( *this, (SArray<Type> *)nullptr );
  return SSArray<Type>::Add( el, indexEl );
}


//-------------------------------------------------------------------------------
// \ru добавление массива \en adding of an array 
// ---
template <class Type>
inline void CSSArray<Type>::AddArray( const CSSArray<Type> & arr, bool doSort )
{
  if ( this != &arr ) {
    m_sort = false;
    (*this) += arr;
    if ( doSort )
      q_sort( *this, (SArray<Type> *)nullptr );
  }
}


//-------------------------------------------------------------------------------
// \ru добавление массива \en adding of an array 
// ---
template <class Type>
inline void CSSArray<Type>::AddArray( const SArray<Type> & arr, bool doSort )
{
  if ( this != &arr ) {
    m_sort = false;
    (*this) += arr;
    if ( doSort )
      q_sort( *this, (SArray<Type> *)nullptr );
  }
}


//-------------------------------------------------------------------------------
// \ru поиск объекта в массиве \en search of an element in array 
// ---
template <class Type>
inline size_t CSSArray<Type>::Find( const Type & el ) {
  q_sort( *this, (SArray<Type> *)nullptr );
  return SSArray<Type>::Find( el );
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline void CSSArray<Type>::Sort( SArray<Type> * del )  {
  q_sort( *this, del );
}


//------------------------------------------------------------------------------
// \ru удалить элемент из массива \en delete an element from array 
// ---
template <class Type>
inline size_t CSSArray<Type>::RemoveObj( const Type & delObject ) {
  size_t ind = Find( delObject );
  if ( ind != SYS_MAX_T )
    RemoveInd( ind );
  return ind;
}


//-----------------------------------------------------------------------------
// \ru Н.Вирт "Алгоритмы и структуры данных" 2е издание, Санкт-Петербург, 2001г., стр.111 \en see N.Wirth "Algorithms and Data Structures" 
// ---
template <class Type>
void q_sort_r( CSSArray<Type> & arr, size_t minInd = SYS_MAX_T, size_t maxInd = SYS_MAX_T )
{
  if ( arr.Count() > 1 ) {
    if ( minInd == SYS_MAX_T ) minInd = 0;
    if ( maxInd == SYS_MAX_T || maxInd > (size_t)arr.MaxIndex() ) 
      maxInd = (size_t)arr.MaxIndex(); // \ru проверено count > 0 \en chacked that count > 0 

    size_t i = minInd, j = maxInd; // \ru OV_x64 приводить к знаковому значению будем только в операторах > и < \en OV_x64 cast to signed value only in operators > and <  
    size_t im = (i + j)/2;         // \ru OV_x64 приводить к знаковому значению будем только в операторах > и < \en OV_x64 cast to signed value only in operators > and <  

    Type * middle = (Type *)new char[sizeof(Type)];
    ::memcpy( middle, &arr[im], sizeof( Type ) );

    Type * buff   = (Type *)new char[sizeof(Type)]; 
    do {
      while( arr[i] < *middle ) i++;
      while( *middle < arr[j] ) j--;
      if ( (ptrdiff_t)i <= (ptrdiff_t)j ) {
        if ( i != j ) {
          Type * wi = &arr[i];
          Type * wj = &arr[j];
          ::memcpy( buff,  wi,    sizeof( Type ) ); 
          ::memcpy( wi,    wj,    sizeof( Type ) ); 
          ::memcpy( wj,    buff, sizeof( Type ) ); 
        }
        i++;
        j--;
      }
    } while( !((ptrdiff_t)i > (ptrdiff_t)j) );

    delete [] (char*)buff;
    delete [] (char*)middle;

    if ( (ptrdiff_t)minInd < (ptrdiff_t)j )
      q_sort_r( arr, minInd, j );
    if ( (ptrdiff_t)i < (ptrdiff_t)maxInd )
      q_sort_r( arr, i, maxInd );
  }
}

//-----------------------------------------------------------------------------
// \ru Аналог q_sort_r без рекурсии. Не требует выделения/освобождения памяти на каждой итерации.
// \en Analog of q_sort_r without recursion. Not require memory allocation/deallocation at each iteration.
// \param arr[out] - \ru Указатель на начало сортируемого участка массива. \en Pointer to the beginning of the array part being sorted.
// \param minIndex[in] - \ru Индекс первого элемента в сортируемом участке массива. \en Index of the first element in the array part being sorted.
// \param maxIndex[in] - \ru Индекс последнего элемента в сортируемом участке массива. \en Index of the last element in the array part being sorted.
// ---
template <class Type>
void q_sort_r2( Type * arr, size_t minIndex, size_t maxIndex )
{
  ptrdiff_t rangeSize = (ptrdiff_t)maxIndex - (ptrdiff_t)minIndex;
  if ( rangeSize == 1 ) {
    if ( arr[maxIndex] < arr[minIndex] ) {
      Type * buff = ( Type * )new char[sizeof( Type )];
      Type * w0 = &arr[minIndex];
      Type * w1 = &arr[maxIndex];
      ::memcpy( buff, w1, sizeof( Type ) );
      ::memcpy( w1, w0, sizeof( Type ) );
      ::memcpy( w0, buff, sizeof( Type ) );
      delete[]( char* )buff;
    }
  }
  else if ( rangeSize > 1 ) {
    c3d::NumbersPair iterStack[30];
    int stackCount = -1;

    ptrdiff_t minInd = minIndex, maxInd = maxIndex;
    ptrdiff_t i = minInd, j = maxInd; 
    ptrdiff_t im = 0;

    Type * middle = ( Type * )new char[sizeof( Type )];
    Type * buff = ( Type * )new char[sizeof( Type )];

    for ( ;; ) {
      i = minInd, j = maxInd;
      im = ( i + j ) / 2;
      ::memcpy( middle, &arr[im], sizeof( Type ) );

      do {
        while ( arr[i] < *middle ) i++;
        while ( *middle < arr[j] ) j--;
        if ( i <= j ) {
          if ( i != j ) {
            Type * wi = &arr[i];
            Type * wj = &arr[j];
            ::memcpy( buff, wi, sizeof( Type ) );
            ::memcpy( wi, wj, sizeof( Type ) );
            ::memcpy( wj, buff, sizeof( Type ) );
          }
          i++;
          j--;
        }
      } while ( !( i > j ) );

      if ( j - minInd > maxInd - i ) {
        if ( minInd < j ) {
          iterStack[++stackCount].first = minInd;
          iterStack[stackCount].second  = j;
        }

        if ( i < maxInd ) {
          minInd = i;
          continue;
        }
      }
      else {
        if ( i < maxInd ) {
          iterStack[++stackCount].first = i;
          iterStack[stackCount].second  = maxInd;
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

    delete[]( char* )buff;
    delete[]( char* )middle;
  }
}

//-----------------------------------------------------------------------------
// 
// ---
template <class Type>
void q_sort( CSSArray<Type> & arr, SArray<Type> * del )
{
  if ( !arr.m_sort ) {
    if ( arr.Count() ) {
      q_sort_r2<Type>( arr.begin(), 0, arr.Count() - 1 ); // C3D-1211
      //q_sort_r( arr );
      
      // \ru удаление одинаковых \en deletion of similar objects 
      for ( ptrdiff_t i = arr.MaxIndex(); i >= 1; i-- ) { // OV_x64 maxIndex >= 0
        if ( arr[i] == arr[i-1] ) {
          if ( del )
            del->Add( arr[i] );
          arr.RemoveInd( i );
        }
      }
    }

    arr.m_sort = true;
  }
}


#endif // __TEMPL_CSS_ARRAY_H
