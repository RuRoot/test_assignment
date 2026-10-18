////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Сортированный одномерный массив указателей на обьекты.
         \en Sorted one-dimensional array of pointers to objects. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_SFDP_ARRAY_H
#define __TEMPL_SFDP_ARRAY_H


#include <templ_fdp_array.h>


FORVARD_DECL_TEMPLATE_TYPENAME( class SFDPArray );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t add_to_array ( SFDPArray<Type> & arr, Type& el, Type *& found, bool & added ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t find_in_array( const SFDPArray<Type>&, const Type&, Type *&found ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader & CALL_DECLARATION operator >> ( reader& in, SFDPArray<Type> & ref ) );         
FORVARD_DECL_TEMPLATE_TYPENAME( writer & CALL_DECLARATION operator << ( writer& out, const SFDPArray<Type> & ref ) );


//------------------------------------------------------------------------------
/** \brief \ru Сортированный одномерный массив указателей на обьекты.
           \en Sorted one-dimensional array of pointers to objects. \~
  \details \ru Сортированный одномерный массив указателей на обьекты. \n
    Нет повторного добавления. Без функции сравнения массив бесполезен.
           \en Sorted one-dimensional array of pointers to objects. \n
    There is no repeat adding. The array is useless without comparison function. \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class  SFDPArray : private FDPArray<Type> {

protected :
  typedef int   (*CompareFunc)( const Type &d1, const Type &d2 ); // \ru Функция сравнения для поиска и наполнения. \en A comparison function for search and filling.
  typedef int   (*SearchFunc) ( const Type &d1, size_t d );       // \ru Функция сравнения для поиска. \en A comparison function for search.

  CompareFunc   fCompare; // \ru Функция сравнения. \en A comparison function.

public :
  SFDPArray( size_t i_upper, uint16 i_delta, CompareFunc fc, typename FDPArray<Type>::DestroyFunc fd );
  virtual ~SFDPArray();

using  FDPArray<Type>::AddCArray;
using  FDPArray<Type>::OwnsElem;
using  FDPArray<Type>::Delta;
using  FDPArray<Type>::Upper;
using  FDPArray<Type>::SetMaxDelta;
using  FDPArray<Type>::Flush;
using  FDPArray<Type>::Clear;
using  FDPArray<Type>::Adjust;
using  FDPArray<Type>::Reserve;
using  FDPArray<Type>::RemoveInd;
using  FDPArray<Type>::DetachInd;
using  FDPArray<Type>::DestroyInd;
using  FDPArray<Type>::DestroyObj;
using  FDPArray<Type>::Count;
using  FDPArray<Type>::MaxIndex;
using  FDPArray<Type>::operator[];
using  FDPArray<Type>::clear;

using  RPArray<Type>::empty;
using  RPArray<Type>::size;
using  RPArray<Type>::reserve;
using  RPArray<Type>::capacity;
using  RPArray<Type>::begin;
using  RPArray<Type>::end;
using  RPArray<Type>::cbegin;
using  RPArray<Type>::cend;
using  RPArray<Type>::front;
using  RPArray<Type>::back;


  /**  \brief \ru Сбросить себя и скопировать other. \en Reset itself and copy 'other'.
  */
  bool                   Init    ( const SFDPArray<Type> & other );

  /**
    \brief   \ru Попробовать добавить элемент с сортировкой.
             \en Try to add an element with sorting. \~
    \details \ru Попробовать добавить элемент с сортировкой. Если объект уже существует, то не добавлять.
             \en Try to add an element with sorting. If an object already exists, it is not added. \~
    \param[in] ent    - \ru Элемент для добавления.
                        \en An element to add. \~
    \param[out] found - \ru Добавленный или найденный элемент.
                        \en Added or found element. \~
    \return \ru Если элемент добавлен, то возвращает индекс добавленного объекта, в противном случае возвращает индекс найденного объекта.
            \en If the element has been added, then returns the index of added element, otherwise, returns the index of the found element.\~
  */
  size_t                 AddTry  ( Type &ent, Type *&found );
                                                                                                                           
  /**
    \brief   \ru Добавить элемент с сортировкой.
             \en Add an element with sorting. \~
    \details \ru Добавить элемент с сортировкой. Нельзя повторно добавить существующий объект.
             \en Add an element with sorting. An existed object cannot be added repeatedly.  \~
    \return  \ru Возвращает true, если элемент добавлен.
              Если элемент не добавлен:
               - если он эквивалентен найденному в соответствии с функцией сравнения, 
                 но не тот же самый (указатели данного элемента и существующего не равны), то возвращается false.
               - если элемент тот же самый (указатели данного элемента и существующего равны), то возвращается true.
             \en Returns true, if the element has been added.
              If the element was not added:
                 - if the element is equivalent to the existing element according to the comparison function,
                   but not the same (the pointers of the given element and the existing element are not equal), false is returned.
                 - if the element is the same (the pointers of the given element and the existing element are equal), true is returned. \~
  */
  bool                   AddExact( Type & );                   

  /**
    \brief   \ru Добавить элемент с сортировкой.
             \en Add an element with sorting.
    \details \ru Добавить элемент с сортировкой. Нельзя добавить повторно существующий объект.
             \en Add an element with sorting. An existed object cannot be added repeatedly. 
    \return  \ru Возвращает true, если элемент добавлен, или false, если не добавлен.
             \en Returns true, if the element has been added, false - otherwise.
  */
  bool                   AddIfNotExist( Type & );              

  /**
    \brief   \ru Доступ к функции базового класса - добавить элемент в конец массива.
             \en An access to the function of the base class - add an element to the end of the array.
    \details \ru Доступ к функции базового класса - добавить элемент в конец массива.
             \en An access to the function of the base class - add an element to the end of the array.
  */
  void                   AddSimple  ( Type &ent ) { FDPArray<Type>::Add( &ent ); }
  
  /**
    \brief   \ru Найти индекс элемента, используя функцию сравнения.
             \en Find the index of the element using the comparison function.
    \details \ru Найти индекс элемента, используя функцию сравнения.
             \en Find the index of the element using the comparison function.
    \param[in] el     - \ru элемент, который ищется.
                        \en An element to find. \~
    \param[out] found - \ru В 'found' будет лежать найденный элемент, или ближайший к искомому.
                        \en The 'found' will contain the found element or the nearest to the required element.\~
    \return \ru Вернет -1, если ближайший элемент, или индекс найденного элемента.
            \en Returns -1, if this is the nearest element, or the index of the same element.\~         
  */
  size_t                 FindNearest( const Type & el, Type *& found ) const;

  /**
    \brief   \ru Найти индекс элемента, используя функцию сравнения.
             \en Find the index of the element using the comparison function.
    \details \ru Найти индекс элемента, используя функцию сравнения.
             \en Find the index of the element using the comparison function.
    \return  \ru Вернет точно найденный элемент или nullptr, если элемент не найден.
             \en Returns the found element or nullptr, if element not found.
  */
  Type  *                FindExact  ( const Type & el ) const;

  /**
    \brief   \ru Вернет true, если элемент найден, или false в противном случае.
             \en Returns true, if the element was found, or false otherwise.
    \details \ru Вернет true, если элемент найден, или false в противном случае.
             \en Returns true, if the element was found, or false otherwise.
  */
  bool                   IsExist ( const Type & el ) const;
  Type*                  RemoveObj( Type * delObject, DelType=defDelete );
  bool                   DetachObj( const Type * delObject );

  /**
    \brief   \ru Найти индекс элемента, используя функцию поиска.
             \en Find the index of the element using the search function.
    \details \ru Найти индекс элемента, используя функцию поиска.
             \en Find the index of the element using the search function.
    \param[out] found - \ru В 'found' будет лежать найденный элемент, или ближайший к искомому.
                        \en The 'found' will contain the found element or the nearest to the required element.
    \return  \ru Вернет -1, если ближайший элемент, или индекс найденного элемента.
             \en Returns -1, if this is the nearest, or the index of the same element.
  */
  size_t                 SearchIt( size_t , SearchFunc, Type *& found ) const;

  /**
    \brief   \ru Сортировать массив, используя функцию сравнения.
             \en Sort an array using the comparison function.
  */
  void                   Sort( size_t minInd = SYS_MAX_T, size_t maxInd = SYS_MAX_T );

private:
  SFDPArray( const SFDPArray<Type> & );                        // \ru запрещено !!! \en forbidden !!! 
  SFDPArray<Type>& operator = ( const SFDPArray<Type> & );     // \ru запрещено !!! \en forbidden !!! 

  TEMPLATE_FRIEND size_t add_to_array              TEMPLATE_SUFFIX (       SFDPArray<Type> &,       Type &, Type *& found, bool & added );
  // \ru в found будет лежать найденный, или ближайший к искомому \en in the 'found' object the found element or the nearest to the required element will be stored 
  TEMPLATE_FRIEND size_t find_in_array             TEMPLATE_SUFFIX ( const SFDPArray<Type> &, const Type &, Type *& found );

  // \ru Т.к. наследование от базового класса сделано private, то делаю доступ к операторам базового класса.
  // \en Since there is a private inheritance from the base class, I give an access to the operators of the base class.
  TEMPLATE_FRIEND reader& CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader& in, SFDPArray<Type> & ref );         
  TEMPLATE_FRIEND writer& CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer& out, const SFDPArray<Type> & ref );  

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
inline void * SFDPArray<Type>::operator new( size_t size ) {
  return ::Allocate( size, typeid(SFDPArray<Type>).name() );
}

//------------------------------------------------------------------------------
// \ru Перегрузка оператора delete. \en Overloading of the "delete" operator. 
// ---
template <class Type>
inline void SFDPArray<Type>::operator delete( void * ptr, size_t size ) {
  ::Free( ptr, size, typeid(SFDPArray<Type>).name() );
}
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_


//------------------------------------------------------------------------------
// \ru конструктор массива \en constructor of an array 
// ---
template <class Type>
inline SFDPArray<Type>::SFDPArray( size_t i_upper, uint16 i_delta, CompareFunc fc, typename FDPArray<Type>::DestroyFunc fd )
  : FDPArray<Type>( i_upper, i_delta, fd ),
    fCompare ( fc ) // \ru функция сравнения \en a comparison function 
{}


//------------------------------------------------------------------------------
// \ru деструктор массива \en destructor of array 
// ---
template <class Type>
inline SFDPArray<Type>::~SFDPArray() {}


//------------------------------------------------------------------------------
// \ru сбросить себя и скопировать other \en reset itself and copy 'other' 
// ---
template <class Type>
bool SFDPArray<Type>::Init( const SFDPArray<Type> & other ) {
  PRECONDITION( FDPArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily
  
  Flush(); // \ru сбросить себя \en reset itself 

  return AddCArray( other.GetAddr(), other.count );
}


//------------------------------------------------------------------------------
// \ru добавить элемент \en add element 
// ---
template <class Type>
inline size_t  SFDPArray<Type>::AddTry( Type& ent, Type *&found ) {
  PRECONDITION( FDPArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily
// \ru CatchMemory(); ЯТ \en CatchMemory(); ЯТ 

  bool added = true;
  return add_to_array( *this, ent, found, added );
}


//------------------------------------------------------------------------------
// \ru добавить элемент \en add element 
// \ru вернет true - добавлен, false - не добавлен \en if returns true then the element has been added, it has not been added otherwise 
// ---
template <class Type>
inline bool SFDPArray<Type>::AddExact( Type& ent ) {
  PRECONDITION( FDPArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily
// \ru CatchMemory(); ЯТ \en CatchMemory(); ЯТ 

  Type * found = nullptr;
  bool added = true;
  add_to_array( *this, ent, found, added );
  return ( &ent == found ); // \ru вернет true - добавлен, false - не добавлен \en if returns true then the element has been added, it has not been added otherwise 
}


//------------------------------------------------------------------------------
// \ru добавить элемент \en add element 
// \ru вернет true - добавлен, false - не добавлен \en if returns true then the element has been added, it has not been added otherwise 
// ---
template <class Type>
inline bool SFDPArray<Type>::AddIfNotExist( Type& ent ) {
  PRECONDITION( FDPArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily
// \ru CatchMemory(); ЯТ \en CatchMemory(); ЯТ 

  Type * found = nullptr;
  bool added = true;
  add_to_array( *this, ent, found, added );
  return ( added ); // \ru вернет true - добавлен, false - не добавлен \en if returns true then the element has been added, it has not been added otherwise 
}


//------------------------------------------------------------------------------
// \ru вернуть индекс элемента в массиве \en return an index of the element in the array 
// ---
template <class Type>
inline size_t SFDPArray<Type>::FindNearest( const Type &el, Type *&found ) const {
  PRECONDITION( FDPArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily
  return find_in_array( *this, el, found );
}


//------------------------------------------------------------------------------
// \ru найти индекс элемента, используя функцию сравнения \en find the index of the element using the comparison function 
// \ru вернет точно найденный \en returns the found one 
// ---
template <class Type>
inline Type * SFDPArray<Type>::FindExact( const Type &el ) const {
  PRECONDITION( FDPArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily
  Type * found = nullptr;
  size_t  foundInd = find_in_array( *this, el, found );
  return ( foundInd != SYS_MAX_T ) ? found : nullptr;
}


//------------------------------------------------------------------------------
// \ru удалить элемент из массива \en delete an element from array 
// ---
template <class Type>
inline Type* SFDPArray<Type>::RemoveObj( Type *delObject, DelType del ) {
  PRECONDITION( FDPArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily
  if ( !delObject )
    return nullptr;
  
  Type * found = nullptr;
  size_t  i = find_in_array( *this, *delObject, found );
  return ( i != SYS_MAX_T ) ? RemoveInd(i, del) : nullptr;
}


//------------------------------------------------------------------------------
// \ru отсоединить элемент от массива (по указателю) \en detach an element from the array (by the pointer) 
// ---
template <class Type>
inline bool SFDPArray<Type>::DetachObj( const Type *delObject ) {
  if ( !delObject )
    return false;
  Type * found = nullptr;
  size_t i = find_in_array( *this, *delObject, found );
  
  if ( i != SYS_MAX_T ) {
    DetachInd( i );
    return true;
  }
  
  return false;
}


//------------------------------------------------------------------------------
// \ru вернуть индекс элемента в массиве \en return an index of the element in the array 
// ---
template <class Type>
inline bool SFDPArray<Type>::IsExist( const Type &el ) const {
  PRECONDITION( FDPArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily
  Type * found = nullptr;
  return find_in_array( *this, el, found ) != SYS_MAX_T;
}


//------------------------------------------------------------------------------
// \ru повторно добавить нельзя!!! \en it cannot be added repeatedly!!! 
// \ru снаружи отвести памяти \en allocate memory from the outside 
// ---
template <class Type>
size_t add_to_array ( SFDPArray<Type>& arr, Type& el, Type *&found, bool & added ) {
  PRECONDITION( arr.fCompare ); // \ru без функции сравнения массив бессмысленен \en the array is useless without comparison function 

  added = true;
  if ( !arr.count ) {
    found = &el;
//arr.parr[arr.count++] = &el; // \ru count вырос на 1 \en 'count' increased by 1 
    arr.Insert( 0, &el );
    return 0;
  }

  found = arr/*.parr*/[0];
  int res = (*arr.fCompare)( *found, el ); // SKIP_SA
  if ( res > 0 ) { // \ru нулевой БОЛЬШЕ пришедшего - вставим ПЕРЕД нулевым \en the first element is GREATER than the sent one - set it BEFORE the first element 
    found = &el;
    arr.Insert( 0, &el );
    return 0;
  }
  else if ( res == 0 ) {
    added = false;
    return 0; // \ru повторно добавить нельзя!!! \en it cannot be added repeatedly!!! 
  }

  if ( arr.count == 1 ) { // \ru есть всего один \en only one exists 
    if ( res < 0 ) { // \ru нулевой МЕНЬШЕ пришедшего - вставим ПОСЛЕ нулевого \en the first element is LESS than the sent one - set it AFTER the first element 
      found = &el;
      arr.Insert( 1, &el );
      return 1;
    }
  }

  // \ru к этому моменту пришедший БОЛЬШЕ нулевого \en by this time the sent element is GREATER than the first element 
  size_t mx = arr.count - 1;

  found = arr/*.parr*/[mx];
  res = (*arr.fCompare)( *found, el ); // SKIP_SA
  if ( res < 0 ) { // \ru последний МЕНЬШЕ пришедшего - вставим ПОСЛЕ последнего \en the last element is LESS than the sent one - set it AFTER the last element 
    found = &el;
    arr.Insert( mx+1, &el );
    return mx+1;
  }
  else if ( res == 0 ) {
    added = false;
    return mx; // \ru повторно добавить нельзя!!! \en it cannot be added repeatedly!!! 
  }

  if ( arr.count == 2 ) { // \ru значит между 0  и 1 \en between 0 and 1 
    if ( res > 0 ) { // \ru последний БОЛЬШЕ пришедшего - вставим ПЕРЕД последним \en the last element is GREATER than the sent one - set it BEFORE the last element 
      found = &el;
      arr.Insert( 1, &el );
      return 1;
    }
  }

  // \ru к этому моменту пришедший БОЛЬШЕ нулевого, но МЕНЬШЕ последнего \en by the moment the sent element is GREATER than the first element and LESS than the last element 
  // \ru вставим где-то между \en put it somewhere between 
  size_t  mn = 0;

  while ( mn + 1 < mx ) {  // \ru пока не нашли - ищем \en seek until do not find 
    size_t md = ( mn + mx ) / 2;

    found = arr/*.parr*/[md];
    res = (*arr.fCompare)( *found, el ); // SKIP_SA
    if ( res > 0 ) { // \ru md БОЛЬШЕ пришедшего, но mn МЕНЬШЕ пришедшего - поиск слева \en 'md' is GREATER than the sent one and 'mn' is LESS than the sent one - search on the left 
      mx = md;
    }
    else if ( res < 0 ) { // \ru md МЕНЬШЕ пришедшего - поиск справа \en 'md' is LESS than the sent one - search on the right 
      mn = md;
    }
    else {
      added = false;
      return md; // \ru повторно добавить нельзя!!! \en it can not be added repeatedly!!! 
    }

  } // end while

  found = &el;
  arr.Insert( mx, &el );
  return mx;
}

//------------------------------------------------------------------------------
// \ru бинарный поиск \en binary search 
// ---
template <class Type>
size_t find_in_array( const SFDPArray<Type>& arr, const Type& el, Type *&found ) {
  PRECONDITION( arr.fCompare ); // \ru без функции сравнения массив бессмысленен \en the array is useless without comparison function 
  // \ru общий случай - элементов больше двух \en the common case - the number of elements is more than two 
  int res = 0;
  if ( arr.count > 11 ) {
    size_t mx = arr.count - 1;

    size_t mxc = mx;
    size_t mn = 0;

    while ( mn + 1 < mx ) {  // \ru пока не нашли - ищем \en seek until do not find 
      size_t md = ( mn + mx ) / 2;
      found = arr[md];
      res = (*arr.fCompare)( *found, el ); // SKIP_SA
      switch ( res ) {
        case 1  : mx = md; break;
        case -1 : mn = md; break;
        case 0  : return md;
        default : PRECONDITION( false );
      }
    }

    // \ru проверка по границам \en check by bounds 
    found = arr[0];
    res = (*arr.fCompare)( *found, el ); // SKIP_SA
    switch ( res ) {
      case 0 : return 0;
      case 1 : return SYS_MAX_T;
      case -1 : break;
    }

    found = arr[mxc];
    res = (*arr.fCompare)( *found, el ); // SKIP_SA
    switch ( res ) {
      case 0  : return mxc;
      default : return SYS_MAX_T;
    }
  }
  else {
    // \ru специальные случаи count ==   >0 & < 11 \en special cases  count ==   >0 & < 11 
    for ( size_t i = 0, count = arr.count; i < count; i++ ) {
      found = arr[i];
      switch ( (*arr.fCompare)( *found, el ) ) { // SKIP_SA
        case 0  : return i;
        case 1  : return SYS_MAX_T;
        case -1 : break;
      }
    }
  }

  return SYS_MAX_T;
}


//------------------------------------------------------------------------------
// \ru бинарный поиск \en binary search 
// \ru найти индекс элемента, используя функцию поиска \en find the index of the element using the search function 
// \ru вернуть индекс элемента в массиве \en return an index of the element in the array 
// \ru в found будет лежать найденный, или ближайший к искомому \en in the 'found' object the found element or the nearest to the required element will be stored 
// ---
template <class Type>
size_t SFDPArray<Type>::SearchIt ( size_t data, typename SFDPArray<Type>::SearchFunc fSearch, Type *&found ) const {
  PRECONDITION( FDPArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily
  PRECONDITION( fSearch ); // \ru без функции сравнения массив бессмысленен \en the array is useless without comparison function 

  if ( !FDPArray<Type>::count )
    return SYS_MAX_T;

  found = (*this)/*parr*/[0]; // \ru или найденный, или ближайший к искомому \en either found or the nearest to the required one 
  int res = (*fSearch)( *found, data );  // SKIP_SA
  if ( res == 0 )
    return 0;

  if ( FDPArray<Type>::count == 1 )
    return SYS_MAX_T;

  if ( res > 0 ) // \ru пришедший МЕНЬШЕ нулевого \en the sent element is LESS than the first element 
    return SYS_MAX_T;

  // \ru к этому моменту пришедший БОЛЬШЕ нулевого \en by this time the sent element is GREATER than the first element 
  size_t mx = FDPArray<Type>::count - 1;

  found = (*this)/*parr*/[mx]; // \ru или найденный, или ближайший к искомому \en either found or the nearest to the required one 
  res = (*fSearch)( *found, data );
  if ( res < 0 ) { // \ru последний МЕНЬШЕ пришедшего \en the last element is LESS than the sent element 
    return SYS_MAX_T;
  }
  else if ( res == 0 ) {
    return mx; // \ru повторно добавить нельзя!!! \en it can not be added repeatedly!!! 
  }

  if ( FDPArray<Type>::count == 2 ) { // \ru значит между 0  и 1 \en between 0 and 1 
    return SYS_MAX_T;
  }

  // \ru к этому моменту пришедший БОЛЬШЕ нулевого, но МЕНЬШЕ последнего \en by the moment the sent element is GREATER than the first element and LESS than the last element 
  // \ru вставим где-то между \en put it somewhere between 
  size_t mn = 0;

  while ( mn + 1 < mx ) {  // \ru пока не нашли - ищем \en seek until do not find 
    size_t md = ( mn + mx ) / 2;

    found = (*this)/*parr*/[md]; // \ru или найденный, или ближайший к искомому \en either found or the nearest to the required one 
    res = (*fSearch)( *found, data );
    if ( res > 0 ) { // \ru md БОЛЬШЕ пришедшего, но mn МЕНЬШЕ пришедшего - поиск слева \en 'md' is GREATER than the sent one and 'mn' is LESS than the sent one - search on the left 
      mx = md;
    }
    else if ( res < 0 ) { // \ru md МЕНЬШЕ пришедшего - поиск справа \en 'md' is LESS than the sent one - search on the right 
      mn = md;
    }
    else {
      return md; // \ru повторно добавить нельзя!!! \en it can not be added repeatedly!!! 
    }

  } // end while

  return SYS_MAX_T;
}


//-----------------------------------------------------------------------------
// \ru Сортировка массива \en Array sorting 
// \ru Н.Вирт "Алгоритмы и структуры данных" 2е издание, Санкт-Петербург, 2001г., стр.111 \en see N.Wirth "Algorithms and Data Structures" 
// ---
template <class Type>
inline void SFDPArray<Type>::Sort( size_t /*OV_x64 int*/ minInd /*= -1*/, size_t /*OV_x64 int*/ maxInd /*= -1*/ )
{
  // K10 SP2 33872
  if ( Count() == 0 )
    return;

  if ( minInd == SYS_MAX_T )
    minInd = 0;
  if ( maxInd == SYS_MAX_T )
    maxInd = MaxIndex();  // \ru OV_x64 проверено - count > 0 \en OV_x64 validated - count > 0 

  size_t i = minInd, j = maxInd; // \ru OV_x64 приводить к знаковому значению будем только в операторах > и < \en OV_x64 cast to signed value only in operators > and <  
  size_t im = (i + j)/2;         // \ru OV_x64 приводить к знаковому значению будем только в операторах > и < \en OV_x64 cast to signed value only in operators > and <  

  Type * middle = (*this)[im];

  do {
    while( (*fCompare)(*((*this)[i]), *middle    ) == -1 ) i++;
    while( (*fCompare)(      *middle, *(*this)[j]) == -1 ) j--;
    if ( static_cast<ptrdiff_t>(i) <= static_cast<ptrdiff_t>(j) ) {
      if ( i != j ) {
        Type * wi = (*this)[i];
        (*this)[i] = (*this)[j];
        (*this)[j] = wi;
      }
      i++;
      j--;
    }
  } while( !(static_cast<ptrdiff_t>(i) > static_cast<ptrdiff_t>(j)) );


  if ( static_cast<ptrdiff_t>(minInd) < static_cast<ptrdiff_t>(j) )
    Sort( minInd, j );
  if ( static_cast<ptrdiff_t>(i) < static_cast<ptrdiff_t>(maxInd) )
    Sort( i, maxInd );
}


#endif  // __TEMPL_SFDP_ARRAY_H
