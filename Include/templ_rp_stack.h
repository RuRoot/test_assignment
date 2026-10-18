////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Не владеющий стек указателей.
         \en Not owning stack of pointers. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_RP_STACK_H
#define __TEMPL_RP_STACK_H


#include <templ_rp_array.h>


//------------------------------------------------------------------------------
/** \brief \ru Стек указателей.
           \en Stack of pointers. \~
  \details \ru Стек указателей без владения. \n
    Для организации стека используем в качестве базы RPArray, и отсекаем лишнее с помощью приватного наследования.
           \en Stack of pointers without ownership. \n
    For the organization of the stack use RPArray as the base and cut all unnecessary by the private inheritance. \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class RPStack: private RPArray<Type> {
public:
  RPStack( size_t i_upper, uint16 i_delta = 1 ): RPArray<Type>( i_upper, i_delta ) {}  
  
public:
  void    Push( Type & obj ); ///< \ru Добавить элемент в стек. \en Add an element to the stack. 
  Type *  Pop();              ///< \ru Извлечь один элемент стека, если возвращаетя nullptr, значит достигнуто дно стека. \en Retrieve one element from the stack, if nullptr is returned then the bottom of stack is reached.
  Type *  Top() const;        ///< \ru Верхний элемент стека. \en The top element of the stack. 

  // \ru Оставить доступными следующие методы: \en Leave an access to the next methods: 
  using RPArray<Type>::DetachAll; ///< \ru Отцепить все элементы (очистить стек). \en Detach all elements (clear the stack). 
  using RPArray<Type>::Count;
  using RPArray<Type>::IsExist;
  using RPArray<Type>::operator[];  ///< \ru Оператор доступа по индексу; \en Access by index operator; 

private:
  RPStack( const RPStack<Type> & );         // \ru запрещено !!! \en forbidden !!! 
  void operator =( const RPStack<Type> & ); // \ru запрещено !!! \en forbidden !!! 
};


//------------------------------------------------------------------------------
// \ru Добавить элемент в стек \en Add an element to the stack. 
//---
template <class Type>
void RPStack<Type>::Push( Type & obj ) {
  RPArray<Type>::Add( &obj );
}


//------------------------------------------------------------------------------
// \ru Извлечь один элемент стека \en Retrieve one element from the stack 
//---
template <class Type>
Type * RPStack<Type>::Pop() {  
  if ( RPArray<Type>::count > 0 ) {
    Type * ret = (*this)[RPArray<Type>::count-1];
    RPArray<Type>::count--;
    return ret;
  }
  return nullptr;
}


//------------------------------------------------------------------------------
// \ru Верхний элемент стека; \en The top element of the stack; 
//---
template <class Type>
Type * RPStack<Type>::Top() const {  
  if ( RPArray<Type>::count > 0 ) {
    return (*this)[RPArray<Type>::count-1];
  }
  return nullptr;
}


#endif // __TEMPL_RP_STACK_H
