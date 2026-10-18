////////////////////////////////////////////////////////////////////////////////
/** 
  \file     
  \brief \ru Итератор массива.
         \en Iterator of array. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_ITERATOR_H
#define __TEMPL_ITERATOR_H

#include <templ_s_list.h>

//------------------------------------------------------------------------------
/** \brief \ru Интерфейс итератора.
           \en Interface of iterator. \~
  \details \ru Итератор итератора. \n
           \en Iterator of iterator. \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class PointersIterator {
public:
  /// \ru Деструктор. \en Destructor. 
  virtual ~PointersIterator() {};
  /// \ru Сброс итератора. \en Reset iterator. 
  virtual void    Restart() = 0;
  /// \ru Получить текущий элемент и сдвинуть итератор на следующий. \en Get the current element and move an iterator to the next. 
  virtual Type *  operator ++(int) = 0;
  /// \ru Получить текущий элемент \en Get the current element 
  virtual Type *  operator() () const = 0;
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор массива.
           \en Iterator of array. \~
  \details \ru Итератор массива указателей. \n
           \en Iterator of pointers array. \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Array, class Type>
class PointersArrayIterator : public PointersIterator<Type> {
private:
  const Array & items; ///< \ru Ссылка на массив. \en A reference to the array. 
  size_t        index; ///< \ru Текущее положение итератора. \en The current position of iterator. 

public:
  /// \ru Конструктор итератора. \en Constructor of iterator. 
  PointersArrayIterator( const Array & arr ) : items( arr ), index( 0 ) {}
  /// \ru Сброс итератора. \en Reset iterator. 
  virtual void    Restart() { index = 0; }
  /// \ru Получить текущий элемент и сдвинуть итератор на следующий. \en Get the current element and move an iterator to the next. 
  virtual Type *  operator ++(int)    { return (index < items.Count()) ? items[index++] : nullptr; }
  /// \ru Получить текущий элемент \en Get the current element 
  virtual Type *  operator() () const { return (index < items.Count()) ? items[index]   : nullptr; }

private: // \ru не реализовано \en not implemented 
  PointersArrayIterator & operator = ( const PointersArrayIterator & );
  PointersArrayIterator ( const PointersArrayIterator & );
  PointersArrayIterator ();
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор списка.
           \en Iterator of list. \~
  \details \ru Итератор списка указателей. \n
           \en Iterator of pointers list. \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
struct PointersListIterator : public LIterator<Type>, public PointersIterator<Type>
{
  /// \ru Конструктор итератора. \en Constructor of iterator. 
  PointersListIterator( const List<Type> & l ) : LIterator<Type>( l ) {}
  /// \ru Конструктор итератора. \en Constructor of iterator. 
  PointersListIterator()                       : LIterator<Type>() {}

  /// \ru Сброс итератора. \en Reset iterator. 
  virtual void    Restart() { LIterator<Type>::Restart(); }
  /// \ru Получить текущий элемент и сдвинуть итератор на следующий. \en Get the current element and move an iterator to the next. 
  virtual Type *  operator ++(int)    { return LIterator<Type>::operator++(int()); }
  /// \ru Получить текущий элемент \en Get the current element 
  virtual Type *  operator() () const { return LIterator<Type>::operator()(); }

private: // \ru не реализовано \en not implemented 
  PointersListIterator & operator = ( const PointersListIterator & );
  PointersListIterator ( const PointersListIterator & );
};


#endif  //__TEMPL_ITERATOR_H
