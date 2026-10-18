////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Одномерный массив указателей с подсчетом ссылок.
         \en One-dimensional array of pointers with counting of references. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_IFC_ARRAY_H
#define __TEMPL_IFC_ARRAY_H


#include <templ_fdp_array_.h>
#include <templ_rp_array.h>
#include <templ_sptr.h>
#include <algorithm>
//#include <xutility>


FORVARD_DECL_TEMPLATE_TYPENAME( class IFC_Array );
FORVARD_DECL_TEMPLATE_TYPENAME( reader & CALL_DECLARATION operator >> ( reader & in, IFC_Array<Type> & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer & CALL_DECLARATION operator << ( writer & out, const IFC_Array<Type> & ref ) );


//------------------------------------------------------------------------------
/** \brief \ru Одномерный массив указателей с подсчетом ссылок.
           \en One-dimensional array of pointers with counting of references. \~
  \details \ru Одномерный массив указателей с подсчетом ссылок. \n
    У объектов должны быть функции AddRef и Release.
           \en One-dimensional array of pointers with counting of references. \n
    Objects should have functions AddRef and Release. \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class IFC_Array : private RPArray<Type> 
{
public:
  typedef Type *               stored_type; ///< \ru Имя для указателя на объект. \en A name of the pointer to the object.
  typedef Type * &             reference;
  typedef Type * const & const_reference;

  /// \ru Константный итератор (новые функции можно добавлять по мере необходимости). \en A constant iterator (new functions can be added as necessary) 
  class iterator
  {    
  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef const Type*               value_type;
    typedef ptrdiff_t                 difference_type;
    typedef const value_type *        pointer;

  public:
    iterator() { m_curr = nullptr; }
    iterator( const stored_type * ptr ) { m_curr = ptr; }    
    iterator( const iterator & iter ) { m_curr = iter.m_curr; }
    stored_type  operator*() const { return *m_curr; }
    // \ru Префиксный инкремент \en A prefix increment 
    iterator &  operator++()
    {
      ++m_curr;
      return *this;
    }
    bool operator == ( const iterator & iter )
    {
      return iter.m_curr == m_curr;
    }
    bool operator != ( const iterator & iter )
    {
      return iter.m_curr != m_curr;
    }
    iterator & operator = ( const iterator& iter )
    {
      m_curr = iter.m_curr;
      return *this;
    }    

  private:
    const stored_type * m_curr; ///< \ru Указатель на указатель на объект \en A pointer to the pointer to the object 
  };

public:
  /// \ru Конструктор. \en Constructor. 
  IFC_Array()
    : RPArray<Type>( 0, 1 )
  {}
  /// \ru Конструктор копирования. \en Copy constructor. 
  IFC_Array( const IFC_Array & init ) 
    : RPArray<Type>( 0, 1 ) 
  {
    RPArray<Type>::AddArray( init );
    for ( size_t idx = Count(); idx != 0; ) {
      RPArray<Type>::operator[](--idx)->AddRef();
    }    
  }
  /// \ru Конструктор. \en Constructor. 
  IFC_Array( size_t i_upper, uint16 i_delta )
    : RPArray<Type>( i_upper, i_delta )
  {}
  /// \ru Деструктор. \en Destructor. 
 ~IFC_Array();

public:
  /// \ru Добавить элемент с повышением счетчика ссылок. \en Add an element with increase of the reference counter. 
  void        Add( stored_type );
  /// \ru Итератор начала. \en An iterator of the beginning. 
  iterator    Begin() const { return iterator(RPArray<Type>::begin()); }
  /// \ru Итератор конца. \en An iterator of the end. 
  iterator    End() const { return iterator(RPArray<Type>::end()); }
  /// \ru Вставить элемент перед указанным. \en Insert an element before the specified one. 
  void        AddAt( stored_type, size_t );
  /// \ru Вставить элемент после указанного. \en Insert an element after the specified one. 
  void        AddAfter( stored_type, size_t );
  /// \ru Задать i-му элементу новое значение. \en Set a new value for the i-th element. 
  void        SetAt( stored_type, size_t );
  /// \ru Поменять местами значения двух элементов массива. \en Swap values of two elements of an array. 
  void        Exchange( size_t, size_t );
  /// \ru Удалить первый найденый элемент из массива. \en Remove a first founded element from an array. 
  stored_type RemoveObj ( stored_type );  
  /// \ru Удалить элемент из массива по индексу. \en Delete an element from array by the index. 
  stored_type RemoveInd ( size_t );
  /// \ru Очистить массив. \en Clear the array. 
  void        Flush(); 
  /// \ru Индексированный доступ (принципиально выдается элемент массива, а не ссылка на него). \en indexed access (returns an element of an array but not the reference to it) 
  const_reference operator []( size_t idx ) const { return RPArray<Type>::at(idx); }

public: // \ru Стандартные функции контейнерного типа. \en Standard functions of container type.
  using RPArray<Type>::empty;
  using RPArray<Type>::size;
  using RPArray<Type>::capacity;
  using RPArray<Type>::reserve;
  /// \ru Получить указатель на первый элемент массива. \en Get the pointer to the first array element. 
  using RPArray<Type>::begin; //const stored_type * begin() const { return RPArray<Type>::begin(); }
  ///< \ru Получить указатель на участок памяти после массива. \en Get the pointer to the piece of memory after the array. 
  using RPArray<Type>::end; //const stored_type *  end() const { return RPArray<Type>::end(); }
  using RPArray<Type>::front;
  using RPArray<Type>::back;

public: // \ru Функции для упрощения перехода на std::vector<SPtr<Type>>. \en Functions to replace this class to std::vector<SPtr<T>>.  
  void push_back( const SPtr<Type> & elem ) { Add(elem.get()); }
  void clear() { Flush(); }
  void erase( typename RPArray<Type>::TPtr * ptr ) { RemoveInd( ptr - RPArray<Type>::begin()); }

public: // \ru Доступные методы от RPArray<Type> \en Available methods from RPArray<Type> 
  using RPArray<Type>::Adjust;
  using RPArray<Type>::Count;
  using RPArray<Type>::Upper;
  using RPArray<Type>::MaxIndex;
  using RPArray<Type>::FindIt;     
  using RPArray<Type>::IsExist;    
  using RPArray<Type>::Reserve;    
  using RPArray<Type>::SetMaxDelta;
  using RPArray<Type>::Sort;
  using RPArray<Type>::GetLast;  

private:
  IFC_Array<Type> & operator = ( const IFC_Array<Type> & ); // \ru Реализовать по необходимости \en Implement if necessary 
  
  TEMPLATE_FRIEND reader & CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader & in, IFC_Array<Type> & ref );
  TEMPLATE_FRIEND writer & CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer & out, const IFC_Array<Type> & ref );

public:
  IFC_Array( IFC_Array<Type> && );                     ///< \ru Конструктор перемещения массива. \en Constructor of an array moving.
  IFC_Array<Type> & operator = ( IFC_Array<Type> && ); ///< \ru Оператор перемещения массива. \en Operator of an array moving.
};

//------------------------------------------------------------------------------
// \ru функция освобождения одного элемента (true означает, что el деструктурирован) \en function of one element release (returns true if 'el' has been destructured)  
// ---
template <class Type>
inline void IFCArray_Release( Type * & el ) 
{
  if ( el != nullptr && el->Release() == 0 )
  {
    // AS K11 27.05.2008 Обнулять, только если объект действительно удален.
    el = nullptr;
  }
}

//------------------------------------------------------------------------------
// \ru Конструктор перемещения массива. \en Constructor of an array moving.
// ---
template <class Type>
inline IFC_Array<Type>::IFC_Array(IFC_Array<Type> && _Right)
  : RPArray<Type>( std::move(_Right) )
{
}

//------------------------------------------------------------------------------
// \ru Оператор перемещения массива. \en Operator of an array moving.
// ---
template <class Type>
IFC_Array<Type> & IFC_Array<Type>::operator = ( IFC_Array<Type> && _Right )
{
  if (this != &_Right)
  {
    std::for_each( RPArray<Type>::begin(), RPArray<Type>::end(), IFCArray_Release<Type> );
    RPArray<Type>::operator = ( std::move(_Right) );
  }
  return (*this);
}

//------------------------------------------------------------------------------
// \ru Деструктор массива. \en Destructor of array 
// ---
template <class Type>
inline IFC_Array<Type>::~IFC_Array() 
{  
  std::for_each( RPArray<Type>::begin(), RPArray<Type>::end(), IFCArray_Release<Type> );
}

//------------------------------------------------------------------------------
// \ru добавить 1 элемент в конец массива \en add 1 element at the end of array 
// ---
template <class Type>
inline void IFC_Array<Type>::Add( Type* ent ) {
  if ( ent != nullptr )
    ent->AddRef();
  RPArray<Type>::Add( ent );
}


//------------------------------------------------------------------------------
// \ru вставить элемент перед указанным \en insert element before the specified one 
// ---
template <class Type>
inline void IFC_Array<Type>::AddAt( stored_type ent, size_t ind ) {
  if ( ent != nullptr )
    ent->AddRef();
  RPArray<Type>::AddAt( ent, ind );
}


//------------------------------------------------------------------------------
// \ru вставить элемент после указанного \en insert element after the specified one 
// ---
template <class Type>
inline void IFC_Array<Type>::AddAfter( stored_type ent, size_t ind ) {
  if ( ent != nullptr )
    ent->AddRef();
  RPArray<Type>::AddAfter( ent, ind );
}


//------------------------------------------------------------------------------
// \ru вставить элемент перед указанным \en insert element before the specified one 
// ---
template <class Type>
inline void IFC_Array<Type>::SetAt( stored_type ent, size_t ind ) {  
  if ( ent != nullptr )
    ent->AddRef();
  Type * & el = RPArray<Type>::operator[](ind);
  if ( el != nullptr )
    el->Release();
  el = ent;
}


//------------------------------------------------------------------------------
// \ru Поменять значения двух элементов массива местами \en Swap values of two elements of an array. 
//---
template <class Type>
inline void IFC_Array<Type>::Exchange( size_t ind1, size_t ind2 ) {
  if ( ind1 != ind2 && ind1 < RPArray<Type>::count && ind2 < RPArray<Type>::count ) {
    Type * value = RPArray<Type>::operator[](ind2);
    RPArray<Type>::operator[](ind2) = RPArray<Type>::operator[](ind1);
    RPArray<Type>::operator[](ind1) = value;
  }
}


//------------------------------------------------------------------------------
/// \ru Удалить элемент из массива по индексу \en Delete an element from array by the index 
// ---
template <class Type>
inline Type * IFC_Array<Type>::RemoveInd( size_t delIndex )
{
  PRECONDITION( delIndex < RPArray<Type>::count );
  stored_type r = 0;
  if ( delIndex < RPArray<Type>::count ) {
    stored_type * d = RPArray<Type>::begin() + delIndex;

    r = *d;

    memmove( d, d + 1, (RPArray<Type>::count - delIndex - 1) * SIZE_OF_POINTER );
    RPArray<Type>::count--;
    IFCArray_Release( r );
  }
  return r;
}


//------------------------------------------------------------------------------
// \ru удалить элемент из массива. \en delete an element from array.
// ---
template <class Type>
inline Type * IFC_Array<Type>::RemoveObj( stored_type delObject )
{
  size_t i = find_in_array( *(RPArray<Type>*)this, delObject );
  return (i != SYS_MAX_T) ? RemoveInd(i) : 0;
}


//------------------------------------------------------------------------------
// \ru обнулить количество элементов \en set the number of elements to null. 
// ---
template <class Type>
inline void IFC_Array<Type>::Flush() 
{
  std::for_each( RPArray<Type>::begin(), RPArray<Type>::end(), IFCArray_Release<Type> );
  RPArray<Type>::DetachAll();
}


#endif  // __TEMPL_IFC_ARRAY_H
