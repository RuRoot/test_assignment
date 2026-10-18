////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Стек объектов.
         \en A stack of objects. \~

*/
////////////////////////////////////////////////////////////////////////////////


#ifndef __TEMPL_SSTACK_H
#define __TEMPL_SSTACK_H


#include <templ_s_array.h>


//------------------------------------------------------------------------------
/** \brief \ru Стек объектов.
           \en A stack of objects. \~
  \details \ru Стек объектов. \n
    Для организации стека используем в качестве строителя SArray,
    и отсекаем лишнее с помощью приватного наследования. \n
           \en A stack of objects. \n
    To organize stack SArray is used as the builder,
    and all redundant is cut by using a private inheritance. \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class SStack final: private SArray<Type>
{
public:
  /// \ru Конструктор. \en Constructor. 
  explicit SStack( size_t i_upper = 0, uint16 i_delta = 1 )
    : SArray<Type>( i_upper, i_delta )
  {}
public:
  void    Push( const Type & obj ); ///< \ru Добавить элемент в стек. \en Add an element to the stack. 
  Type &  Pop();                    ///< \ru Извлечь один элемент стека, если возвращаетя nullptr, значит достигнуто дно стека. \en Retrieve one element from the stack, if nullptr is returned then the bottom of stack is reached.
  Type &  Top() const;              ///< \ru Верхний элемент стека (последний внесенный). \en The top element of the stack (the last added). 

  // \ru Оставить доступными следующие методы: \en Leave an access to the next methods: 
  using SArray<Type>::Flush;      ///< \ru Очистить стек. \en Clear the stack. 
  using SArray<Type>::Count;      ///< \ru Количество элементов, содержащихся в стеке. \en The number of elements in stack.
  using SArray<Type>::IsExist;    ///< \ru Существует ли элемент. \en Whether an element exists.
  using SArray<Type>::operator[]; ///< \ru Оператор прямого доступа - работает, как для массива. \en An operator of a direct access - it works as for an array. 
  using SArray<Type>::size;       ///< \ru Количество элементов, содержащихся в стеке. \en The number of elements in stack.
  using SArray<Type>::back;       ///< \ru Количество элементов, содержащихся в стеке. \en The number of elements in stack.
  using SArray<Type>::push_back;  ///< \ru Количество элементов, содержащихся в стеке. \en The number of elements in stack.
  using SArray<Type>::pop_back;   ///< \ru Количество элементов, содержащихся в стеке. \en The number of elements in stack.
  using SArray<Type>::empty;      ///< \ru Количество элементов, содержащихся в стеке. \en The number of elements in stack.

private:
  OBVIOUS_PRIVATE_COPY( SStack ); ///< \ru (!) Без реализации \en (!) There is no implementation
};


//------------------------------------------------------------------------------
/// \ru Добавить элемент в стек \en Add an element to the stack 
//---
template <class Type>
void SStack<Type>::Push( const Type & obj )
{
  SArray<Type>::push_back( obj );
}


//------------------------------------------------------------------------------
/// \ru Извлечь один элемент стека \en Retrieve one element from the stack 
//---
template <class Type>
Type & SStack<Type>::Pop() {  
  if ( SArray<Type>::count > 0 ) {
    Type & ret = (*this)[SArray<Type>::count-1];
    SArray<Type>::count--;
    return ret;
  }
  return (*this)[0];
}


//------------------------------------------------------------------------------
/// \ru Верхний элемент стека \en The top element of the stack 
//---
template <class Type>
Type & SStack<Type>::Top() const {
  return (*this)[SArray<Type>::count-1];
}


#endif // __TEMPL_SSTACK_H
