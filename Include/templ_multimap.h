////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Мультимножество, реализующее основной функционал std::multimap.
         \en Multiset implementing the core functionality of std::multimap. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_MULTIMAP_H
#define __TEMPL_MULTIMAP_H


#include <templ_s_array.h>


//------------------------------------------------------------------------------
/** \brief \ru Ассоциативное множество c дубликатами (мультимножество).
           \en Associative set with duplicates (multiset). \~
  \details \ru Ассоциативное множество c дубликатами (мультимножество). \n
  Реализует основные функциии std::multimap. Требования к типам данных KeyType и ValType такие же, как в SArray<T>.
  Мультимножество задает соответствие (ассоциации) объекта-ключа подмножеству объектов-значений. 
  Для некоторого объекта типа KeyType задается соответствие подмножеству объектов ValType.
           \en Associative set with duplicates (multiset). \n
  Implements the core functions of is std::multimap. Requirements to data types KeyType and ValType are the same as in SArray<T>.
  Multiset sets the mapping (associations) between a key-object and a subset of value-objects. 
  For some object of the "KeyType" type there is set a mapping to a subset of objects of the "ValType" type. \~
  \ingroup Base_Tools_Containers
*/
// ---
template<class KeyType, class ValType>
class MultiMap {
public:

  struct Pair
  {
    const KeyType m_key;
    const ValType m_val;
    Pair( const KeyType & key, const ValType & val ) : m_key( key ), m_val( val ) {}    
    bool operator < ( const Pair & p ) const {
      return m_key < p.m_key;
    }  
    bool operator == ( const Pair & p ) const {
      return m_key == p.m_key && m_val == p.m_val;
    }
    Pair & operator = ( const Pair & ); // \ru Не реализовано. \en Not implemented.
  };  

public:
  MultiMap();
  virtual ~MultiMap() {}
  
  class Iterator;

public:
  /// \ru Оператор доступа по ключу. \en Access by key operator. 
  Iterator  operator[] ( const KeyType & key ) const;
  /// \ru Получить итератор, указывающий на первый элемент. \en Get an iterator pointing to the first element. 
  Iterator  First() const;
  /// \ru Добавить элемент с заданным ключом и значением. \en Add an element with specified key and value. 
  void      Associate( const KeyType & key, const ValType & val );
  /// \ru Удалить заданное значение с заданным ключом. \en Remove an element with specified key and value. 
  void      Dissociate( const KeyType & key, const ValType & val );
  /// \ru Удалить элементы в диапазоне [it1, it2). \en Remove elements in the range [it1, it2). 
  void      Dissociate( Iterator & it1, Iterator & it2 );
  /// \ru Удалить элемент, указанный итератором. \en Remove an element specified by an iterator. 
  Iterator  Dissociate( Iterator & it );
  /// \ru Удалить все элементы из контейнера. \en Removes all elements from the container. 
  void      Flush() { m_Pairs.Flush(); }
  /// \ru Существует ли элемент с заданными ключом и значением. \en Is there an element with the specified key and value. 
  bool      IsAssociated( const KeyType & key, const ValType & val ) const;
  /// \ru Найти элемент с заданными ключом и значением. \en Find an element with the specified key and value. 
  Iterator  Find( const KeyType & key ) const;
  // \ru Итератор, указывающий на первый элемент со значением ключа, не меньшим, чем заданный. \en An iterator pointing to the first element not less than the given key.
  Iterator  LowerBound( const KeyType & key ) const;
  // \ru Итератор, указывающий на первый элемент со значением ключа, большим, чем заданный. \en An iterator pointing to the first element greater than the given key.
  Iterator  UpperBound( const KeyType & key ) const;
  // \ru Диапазон, содержащий все элементы с данным ключом в контейнере. \en A range containing all elements with the given key in the container.
  std::pair<Iterator,Iterator>  EqualRange( const KeyType & key ) const;
  // \ru Количество элементов в контейнере. \en The number of elements in the container.
  size_t    Count() const { return m_Pairs.Count(); }
 
  static size_t UpperBoundEx( const SArray<Pair> & pairs, const KeyType & key );
  static size_t LowerBoundEx( const SArray<Pair> & pairs, const KeyType & key );
  static std::pair<size_t,size_t> EqualRangeEx( const SArray<Pair> & pairs, const KeyType & key );

private:  
  SArray<Pair> m_Pairs;
  
  private: // \ru Специфицируем нуль для разных типов. \en Specify null for different types. 
  template<class T>
    struct Null { // \ru Нуль тривиальных типов (int, float, double и т.д.). \en Null of trivial types (int, float, double etc.). 
    static inline T val() { return 0; }
  };
  template<class T>
    struct Null<T*> { // \ru Нуль указателей. \en Null of pointers.     
    static inline T* val() { return nullptr; }
  };
// \ru LF_Linux: 25.03.11 g++ выдает ошибку на этот код - не использованы KeyType, ValType в полной специализации шаблона.
//     Однако непонятно, зачем нужна эта полная специализация - общая частичная специализация для тривиальных типов вполне подойдет.
// \en LF_Linux: 25.03.11 g++ returns an error in this code - types KeyType and ValType are not used in the full specialization of template.
//     But it is not clear what for this specialization is required - the common partial specialization for the trivial types is sufficient. 
//  template<>
// \ru struct Null<double> { // Нуль вещественных чисел \en struct Null<double> { // Null of real numbers 
//    static inline double val() { return 0.0; }
//  };

public:
  // \ru Итератор по элементам контейнера. \en Iterator for the container elements.
  class Iterator {
  private:
    Pair  * m_Ptr;
    Pair  * m_MaxPtr;
    
  public:
    Iterator() : m_Ptr( nullptr ), m_MaxPtr( nullptr ) {}
    Iterator( const Iterator & iter ) : m_Ptr( iter.m_Ptr ), m_MaxPtr( iter.m_MaxPtr ) {}
    Iterator( const SArray<Pair> & m_Pairs, const Pair & pair ) : m_Ptr( nullptr ), m_MaxPtr( nullptr )
    {
      const size_t count = m_Pairs.Count();
      if ( count > 0 ) {
        size_t idx = MultiMap<KeyType,ValType>::LowerBoundEx( m_Pairs, pair.m_key );
        size_t temp = idx;
        while ( temp < m_Pairs.Count() && m_Pairs[temp].m_key == pair.m_key ) {
          if ( m_Pairs[temp] == pair ) {
            idx = temp;
            break;
          }
          temp++;
        }
        m_MaxPtr = &m_Pairs[count-1];
        if ( idx < count ) {
          m_Ptr = &m_Pairs[idx];          
        }
      }
    }
    Iterator( const SArray<Pair> & m_Pairs, const Iterator & iter1, const Iterator & iter2 ) // range
      : m_Ptr( nullptr ), m_MaxPtr( nullptr )
    {
      const size_t count = m_Pairs.Count();
      if ( count > 0 &&  iter1.m_Ptr != nullptr ) {
        size_t idx1 = MultiMap<KeyType,ValType>::LowerBoundEx( m_Pairs, iter1.m_Ptr->m_key );
        size_t temp = idx1;
        while ( temp < m_Pairs.Count() && m_Pairs[temp].m_key == iter1.m_Ptr->m_key ) {
          if ( m_Pairs[temp] == *iter1.m_Ptr ) {
            idx1 = temp;
            break;
          }
          temp++;
        }
        size_t idx2 = SYS_MAX_T;
        if ( iter2.m_Ptr != nullptr ) {
          idx2 = MultiMap<KeyType,ValType>::UpperBoundEx( m_Pairs, iter2.m_Ptr->m_key );
          if ( idx2 < m_Pairs.Count() ) {
            if ( idx2 > 0)
              idx2--;
            else
              idx2 = SYS_MAX_T;
          }
        }
        if ( idx2 == SYS_MAX_T )
          idx2 = count - 1;
        if ( idx1 < count && idx2 < count ) {
          m_Ptr = &m_Pairs[idx1];          
          m_MaxPtr = &m_Pairs[idx2];
        }
      }
    }

  public:
    /// \ru Получить текущий элемент и сдвинуть итератор на следующий. \en Get the current element and move the iterator to the next. 
    Iterator  operator ++( int ) 
    { 
      Iterator iter(*this); 
      if ( m_Ptr && m_Ptr < m_MaxPtr )
        m_Ptr++; 
      else
        m_Ptr = 0;
      return iter; 
    }
    /// \ru Оператор равенства. \en An equality operator. 
    bool operator == ( const Iterator & itr ) const
    {
      if ( Empty() && itr.Empty() )
        return true;
      if ( ( Empty() && !itr.Empty() ) || ( !Empty() && itr.Empty() ) )
        return false;
      return m_Ptr == itr.m_Ptr && m_MaxPtr == itr.m_MaxPtr;
    }
    /// \ru Оператор "!=". \en An "!=" operator. 
    bool operator != ( const Iterator & itr ) const
    {
      if ( Empty() && itr.Empty() )
        return false;
      if ( ( Empty() && !itr.Empty() ) || ( !Empty() && itr.Empty() ) )
        return true;
      return m_Ptr != itr.m_Ptr || m_MaxPtr != itr.m_MaxPtr;
    }
    /// \ru Оператор "меньше". \en Operator "less". 
    bool operator < ( const Iterator& itr ) const
    {
      return m_Ptr < itr.m_Ptr && m_MaxPtr <= itr.m_MaxPtr;
    }
    /// \ru Пустой ли итератор. \en Is the iterator empty. 
    bool Empty() const
    {
      return !( m_Ptr && m_Ptr <= m_MaxPtr );
    }
    // \ru Получить текущий ключ элемента. \en Get the current key of the element.
    KeyType Key() const
    {
      return !Empty() ? m_Ptr->m_key : (KeyType)0;
    }
    // \ru Получить текущее значение элемента. \en Get the current value of the element.
    ValType Value() const
    {
      return !Empty() ? m_Ptr->m_val : (ValType)0; // SKIP_SA
    }
    // \ru Получить пару с текущим ключом и текущим кзначением элемента. \en Get the pair with the current key and the current value of the element.
    Pair* GetPair() const
    {
      return m_Ptr;
    }
  };
};


//------------------------------------------------------------------------------
// 
// ---
template<class KeyType, class ValType >
size_t MultiMap<KeyType,ValType>::LowerBoundEx( const SArray<typename MultiMap<KeyType,ValType>::Pair> & pairs, const KeyType & key )
{
  if ( pairs.Count() > 11 ) {
    size_t end = pairs.Count() - 1;
    size_t last = end;
    size_t start = 0;
    size_t firstNotLess = SYS_MAX_T;

    while ( start + 1 < end ) { // \ru Ищем, пока не нашли. \en Seek until find.
      size_t middle = ( start + end ) / 2;
      Pair& mdE = pairs[middle];
      if ( mdE.m_key < key ) {
        start = middle;
      }
      else if ( key <= mdE.m_key ) {
        if ( middle < firstNotLess )
          firstNotLess = middle;
        end = middle;
      }
      // \ru Если попадаем сюда, значит некорректно написаны операторы "тождественно" и сравнения.
      // \en If we are here, then operators of identity check and comparison are not correct.
      else {
        PRECONDITION( 0 );
        return SYS_MAX_T;
      }
    }

    // \ru Проверяем ключ между start и end. \en Check a key between start and end.
    if ( start + 1 < pairs.Count() - 1 ) {
      size_t middle = start + 1;
      Pair& mdE = pairs[middle];
      if ( key <= mdE.m_key && middle < firstNotLess )
        firstNotLess = middle;
    }

    if ( key <= pairs[0].m_key )
      return 0;
    if ( firstNotLess <= last && key <= pairs[firstNotLess].m_key )
      return firstNotLess;
    if ( key == pairs[last].m_key )
      return last;
  }
  else {
    if ( pairs.Count() == 1 )
      return key <= pairs[0].m_key ? 0 : SYS_MAX_T;
    else if ( pairs.Count() == 2 ) {
      if ( key <= pairs[0].m_key )
        return 0;
      if ( key <= pairs[1].m_key )
        return 1;
    }
    else { // 2 < count <= 11
      for( size_t i = 0; i < pairs.Count(); ++i )
        if ( key <= pairs[i].m_key )
          return i;
    }
  }
  return SYS_MAX_T;
}

//------------------------------------------------------------------------------
// \ru Итератор, указывающий на первый элемент со значением ключа, не меньшим, чем заданный.
// \en An iterator pointing to the first element not less than the given key.
// ---
template<class KeyType, class ValType >
inline typename MultiMap<KeyType,ValType>::Iterator MultiMap<KeyType,ValType>::LowerBound( const KeyType & key ) const
{
  size_t idx = LowerBoundEx ( m_Pairs, key );
  return idx < m_Pairs.Count() ? Iterator ( m_Pairs, m_Pairs[idx] ) : Iterator();
}

template<class KeyType, class ValType >
size_t MultiMap<KeyType,ValType>::UpperBoundEx( const SArray<typename MultiMap<KeyType,ValType>::Pair> & pairs, const KeyType & key )
{
  if ( pairs.Count() > 11 ) {
    size_t end = pairs.Count() - 1;
    size_t last = end;
    size_t start = 0;
    size_t firstGreater = SYS_MAX_T;

    while ( start + 1 < end ) { // \ru Ищем, пока не нашли. \en Seek until find.
      size_t middle = ( start + end ) / 2;
      Pair& mdE = pairs[middle];
      if ( mdE.m_key <= key ) {
        start = middle;
      }
      else if ( key < mdE.m_key ) {
        if ( middle < firstGreater )
          firstGreater = middle;
        end = middle;
      }
      // \ru Если попадаем сюда, значит некорректно написаны операторы "тождественно" и сравнения.
      // \en If we are here, then operators of identity check and comparison are not correct.
      else {
        PRECONDITION( 0 );
        return SYS_MAX_T;
      }
    }

    if ( key < pairs[0].m_key )
      return 0;
    if ( firstGreater <= end && key < pairs[firstGreater].m_key )
      return firstGreater;
    if ( key < pairs[last].m_key )
      return last;
  }
  else {
    if ( pairs.Count() == 1 )
      return key < pairs[0].m_key ? 0 :SYS_MAX_T;
    else if ( pairs.Count() == 2 ) {
      if ( key < pairs[0].m_key )
        return 0;
      if ( key < pairs[1].m_key )
        return 1;
    }
    else { // 2 < count <= 11
      for( size_t i = 0; i < pairs.Count(); ++i )
        if ( key < pairs[i].m_key )
          return i;
    }
  }
  return SYS_MAX_T;
}


//------------------------------------------------------------------------------
// \ru Итератор, указывающий на первый элемент со значением ключа, большим, чем заданный.
// \en An iterator pointing to the first element greater than the given key.
// ---
template<class KeyType, class ValType >
inline typename MultiMap<KeyType,ValType>::Iterator MultiMap<KeyType,ValType>::UpperBound( const KeyType & key ) const
{
  size_t ind = UpperBoundEx ( m_Pairs, key );
  return ind != SYS_MAX_T ? Iterator ( m_Pairs, m_Pairs[ind] ) : Iterator();
}


//------------------------------------------------------------------------------
// 
// ---
template<class KeyType, class ValType >
std::pair<size_t,size_t> MultiMap<KeyType, ValType>::EqualRangeEx( const SArray<typename MultiMap<KeyType, ValType>::Pair> & pairs, const KeyType & key )
{
  if ( pairs.Count() > 11 ) {
    size_t end = pairs.Count() - 1;
    size_t lastRight = end;
    size_t start = 0;
    size_t firstNotLess = SYS_MAX_T;
    size_t firstGreater = SYS_MAX_T;

    // \ru Проверяем последний элемент. \en Check the last element.
    if ( key > pairs[pairs.Count() - 1].m_key )
      return std::pair<size_t,size_t> (SYS_MAX_T, SYS_MAX_T);

    // \ru Проверяем первый элемент. \en Check the first element.
    if ( key < pairs[0].m_key )
      return std::pair<size_t,size_t> (0, 0);

    if ( key != pairs[0].m_key ) {
      while ( start + 1 < end ) { // \ru Ищем, пока не нашли. \en Seek until find. 
        size_t middle = ( start + end ) / 2;
        Pair& mdE = pairs[middle];
        if ( mdE.m_key < key ) {
          start = middle;
        }
        else if ( key <= mdE.m_key ) {
          if ( key < mdE.m_key && lastRight > middle )
            lastRight = middle;
          if ( middle < firstNotLess )
            firstNotLess = middle;
          end = middle;
        }
        // \ru Если попадаем сюда, значит некорректно написаны операторы "тождественно" и сравнения.
        // \en If we are here, then operators of identity check and comparison are not correct.
        else {
          PRECONDITION( 0 );
          return std::pair<size_t,size_t> (SYS_MAX_T, SYS_MAX_T);
        }
      }
    }
    else
      firstNotLess = 0; // key == pairs[0].m_key

    // \ru Проверяем ключ между start и end. \en Check a key between start and end.
    if ( start + 1 == end ) {
      size_t middle = start + 1;
      Pair& mdE = pairs[middle];
      if ( key <= mdE.m_key && middle < firstNotLess )
        firstNotLess = middle;
    }

    // \ru Проверяем последний элемент. \en Check the last element.
    if ( firstNotLess == SYS_MAX_T && key == pairs[pairs.Count() - 1].m_key )
      return std::pair<size_t,size_t> (pairs.Count() - 1, SYS_MAX_T);

    if ( firstNotLess == SYS_MAX_T )
      return std::pair<size_t,size_t> (SYS_MAX_T, SYS_MAX_T);

    // \ru Теперь начинаем правый поиск. \en Now start right search.
    end = lastRight;
    start = firstNotLess;

    if ( start == end && pairs[firstNotLess].m_key != key )
      return std::pair<size_t,size_t> ( firstNotLess, firstNotLess );

    while ( start + 1 < end ) { // \ru Ищем, пока не нашли. \en Seek until find. 
      size_t middle = ( start + end ) / 2;
      Pair& mdE = pairs[middle];
      if ( mdE.m_key <= key ) {
        start = middle;
      }
      else if ( key < mdE.m_key ) {
        if ( middle < firstGreater )
          firstGreater = middle;
        end = middle;
      }
      // \ru Если попадаем сюда, значит некорректно написаны операторы "тождественно" и сравнения.
      // \en If we are here, then operators of identity check and comparison are not correct.
      else {
        PRECONDITION( 0 );
        return std::pair<size_t, size_t> ( SYS_MAX_T, SYS_MAX_T );
      }
    }

    if ( firstGreater == SYS_MAX_T )
    {
      if ( pairs[firstNotLess].m_key != key )
        firstGreater = firstNotLess;
      else if ( lastRight > 0 && pairs[lastRight - 1].m_key == key && pairs[lastRight].m_key != key )
        firstGreater = lastRight;
      else if ( firstNotLess + 1 < pairs.Count() && pairs[firstNotLess + 1].m_key != key )
        firstGreater = firstNotLess + 1;
    }
    return std::pair<size_t,size_t> ( firstNotLess, firstGreater );
  }
  else {
    if ( pairs.Count() == 0 )
      return std::pair<size_t, size_t> ( SYS_MAX_T, SYS_MAX_T );
    if ( pairs.Count() == 1 )
      return key == pairs[0].m_key ? std::pair<size_t, size_t> ( 0, SYS_MAX_T ) : 
        ( key < pairs[0].m_key ? std::pair<size_t, size_t> ( 0, 0 ) : std::pair<size_t, size_t> ( SYS_MAX_T, SYS_MAX_T ) ); 
    if ( pairs.Count() == 2 ) {
      size_t first = SYS_MAX_T;
      if ( key < pairs[0].m_key )
        return std::pair<size_t, size_t>(0, 0);
      if ( key > pairs[1].m_key )
        return std::pair<size_t, size_t>(SYS_MAX_T, SYS_MAX_T);
      if ( key == pairs[0].m_key )
        first = 0;
      else if ( key == pairs[1].m_key )
        first = 1;
      else // ( key > pairs[0].m_key && key < pairs[1].m_key )
        return std::pair<size_t, size_t> ( 1, 1 );
      if ( key == pairs[1].m_key )
        return std::pair<size_t ,size_t> ( first, SYS_MAX_T );
      return std::pair<size_t, size_t> (first, 1);
    }
    else { // 2 < count <= 11
      size_t first = SYS_MAX_T, second = SYS_MAX_T;
      if ( key < pairs[0].m_key )
        return std::pair<size_t, size_t> ( 0, 0 );
      if ( key > pairs[pairs.Count() - 1].m_key )
        return std::pair<size_t, size_t> ( first, second );
      for( size_t i = 0; i < pairs.Count(); ++i ) {
        if ( key == pairs[i].m_key ) {
          if ( first == SYS_MAX_T )
            first = i;
        }
        else {
          if ( key < pairs[i].m_key ) {
            second = i;
            break;
          }
        }
      }
      if ( first == SYS_MAX_T )
        first = second;
      return std::pair<size_t, size_t> ( first, second );
    }
  }
}


//------------------------------------------------------------------------------
// \ru Диапазон, содержащий все элементы с данным ключом в контейнере.
// \en A range containing all elements with the given key in the container.
// ---
template<class KeyType, class ValType >
inline std::pair<typename MultiMap<KeyType,ValType>::Iterator,typename MultiMap<KeyType,ValType>::Iterator>
  MultiMap<KeyType, ValType>::EqualRange ( const KeyType & key ) const
{
  std::pair<size_t, size_t> idx = EqualRangeEx ( m_Pairs, key );
  Iterator first, second;
  if ( idx.first < m_Pairs.Count() )
    first = Iterator ( m_Pairs, m_Pairs[idx.first]);
  if ( idx.second < m_Pairs.Count() )
    second = Iterator ( m_Pairs, m_Pairs[idx.second]);
  return std::pair<Iterator, Iterator> ( first, second );
}


//------------------------------------------------------------------------------
/// \ru Найти элемент с заданными ключом и значением.
// \en Find an element with the specified key and value. 
// ---
template<class KeyType, class ValType >
inline typename MultiMap<KeyType,ValType>::Iterator MultiMap<KeyType,ValType>::Find( const KeyType & key ) const
{
  if ( m_Pairs.Count() > 11 ) {
    size_t mx = m_Pairs.Count() - 1;
    size_t mxc = mx;
    size_t mn = 0;

    while ( mn + 1 < mx ) { // \ru Ищем, пока не нашли. \en Seek until find.
      size_t md = ( mn + mx ) / 2;
      Pair& mdE = m_Pairs[md];
      if ( mdE.m_key < key ) {
        mn = md;
      }
      else if ( key < mdE.m_key ) {
        mx = md;
      }
      else if ( mdE.m_key == key )
        return Iterator ( m_Pairs, mdE );
      // \ru Если попадаем сюда, значит некорректно написаны операторы "тождественно" и сравнения.
      // \en If we are here, then operators of identity check and comparison are not correct.
      else {
        PRECONDITION( 0 );
        return Iterator();
      }
    }

    if ( key == m_Pairs[0].m_key )
      return Iterator ( m_Pairs, m_Pairs[0] );
    if ( key == m_Pairs[mxc].m_key )
      return Iterator ( m_Pairs, m_Pairs[mxc] );
  }
  else {
    if ( m_Pairs.Count() == 1 )
      return key == m_Pairs[0].m_key ? Iterator ( m_Pairs, m_Pairs[0] ) : Iterator();
    else if ( m_Pairs.Count() == 2 ) {
      if ( key == m_Pairs[0].m_key )
        return Iterator ( m_Pairs, m_Pairs[0] );
      if ( key == m_Pairs[1].m_key )
        return Iterator ( m_Pairs, m_Pairs[1] );
      return Iterator();
    }
    else { // 2 < count <= 11
      for( size_t i = 0; i < m_Pairs.Count(); ++i )
        if ( key == m_Pairs[i].m_key )
          return Iterator ( m_Pairs, m_Pairs[i] );
    }
  }
  return Iterator();
}


//-------------------------------------------------------------------------------
// \ru Конструктор. \en Contructor.
// ---
template<class KeyType, class ValType >
MultiMap<KeyType,ValType>::MultiMap() 
  : m_Pairs( 0, 1 )
{}


//-------------------------------------------------------------------------------
/// \ru Добавить элемент с заданным ключом и значением.
// \en Add an element with specified key and value. 
// ---
template<class KeyType, class ValType >
inline void MultiMap<KeyType,ValType>::Associate( const KeyType & key, const ValType & val ) {
  Pair pair( key, val );
  std::pair<size_t, size_t> idx = EqualRangeEx ( m_Pairs, key );
  if ( idx.second < m_Pairs.Count() )
    m_Pairs.InsertInd ( idx.second, pair );
  else
    m_Pairs.Add ( pair );
}


//-------------------------------------------------------------------------------
// \ru Существует ли элемент с заданными ключом и значением.
// \en Is there an element with the specified key and value. 
// ---
template<class KeyType, class ValType >
inline bool MultiMap<KeyType,ValType>::IsAssociated( const KeyType & key, const ValType & val ) const {   
  return m_Pairs.FindIt( Pair(key, val) ) < m_Pairs.Count();
}


//-------------------------------------------------------------------------------
/// \ru Удалить заданное значение с заданным ключом. \en Remove an element with specified key and value. 
// ---
template<class KeyType, class ValType >
inline void MultiMap<KeyType,ValType>::Dissociate( const KeyType & key, const ValType & val ) {
  Pair pair( key, val );
  size_t idx = m_Pairs.FindIt( pair );
  if ( idx < m_Pairs.Count() ) {
    m_Pairs.RemoveInd( idx );
  }
}


//-------------------------------------------------------------------------------
/// \ru Удалить элемент, указанный итератором. \en Remove an element specified by an iterator. 
// ---
template<class KeyType, class ValType >
inline typename MultiMap<KeyType,ValType>::Iterator MultiMap<KeyType,ValType>::Dissociate( 
  typename MultiMap<KeyType,ValType>::Iterator & it )
{
  size_t idx = m_Pairs.FindIt( *it.GetPair() );
  if ( idx < m_Pairs.Count() ) {
    m_Pairs.RemoveInd( idx );
    if ( idx < m_Pairs.Count() )
      return Iterator ( m_Pairs, m_Pairs[idx]);
  }
  return Iterator();
}


//-------------------------------------------------------------------------------
/// \ru Удалить элементы в диапазоне [it1, it2). \en Remove elements in the range [it1, it2). 
// ---
template<class KeyType, class ValType >
inline void MultiMap<KeyType,ValType>::Dissociate( 
  typename MultiMap<KeyType,ValType>::Iterator & it1, 
  typename MultiMap<KeyType,ValType>::Iterator & it2 )
{
  if ( m_Pairs.Count() == 0 || it1.Empty() ) 
    return;
  if ( it2.Empty() ) {
    // \ru Удаляем с it1 до конца. \en Remove from it1 up to the end.
    it2 = Iterator ( m_Pairs, m_Pairs[m_Pairs.Count() - 1] );
    if ( it1 == it2 )
      Dissociate ( it1.Key(), it1.Value() ); // \ru Просто удаляем последний элемент. \en Just remove the last element.
    else if ( it1 < it2 ) {
      size_t idx = m_Pairs.FindIt( *it1.GetPair() );
      if ( idx != SYS_MAX_T )
        m_Pairs.RemoveInd ( idx, m_Pairs.Count () );
    }
  }
  else if ( it1 < it2 )
      m_Pairs.Remove ( it1.GetPair(), it2.GetPair() );
}


//-------------------------------------------------------------------------------
/// \ru Оператор доступа по ключу. \en Access by key operator. 
// ---
template<class KeyType, class ValType >
inline typename MultiMap<KeyType,ValType>::Iterator MultiMap<KeyType,ValType>::operator[] ( const KeyType & key ) const {
  return Iterator( m_Pairs, Pair(key, Null<ValType>::val()) );
}


//-------------------------------------------------------------------------------
/// \ru Получить итератор, указывающий на первый элемент. \en Get an iterator pointing to the first element. 
// ---
template<class KeyType, class ValType >
inline typename MultiMap<KeyType,ValType>::Iterator MultiMap<KeyType,ValType>::First() const {
  if ( m_Pairs.Count() )
    return Iterator ( m_Pairs, m_Pairs[0] );
  return Iterator();
}


#endif // __TEMPL_MULTIMAP_H
