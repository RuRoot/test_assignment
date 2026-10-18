////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Посетитель классов.
         \en Visitor of classes. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_VISITOR_H
#define __TEMPL_VISITOR_H

#include <math_define.h>


//------------------------------------------------------------------------------
/** \brief \ru Абстрактный базовый класс посетителя.
           \en Abstract base class of the visitor. \~
  \details \ru Абстрактный базовый класс паттерна Visitor. \n
    Служит для ссылки на конкретного посетителя (ConcreteVisitor). \n
           \en Abstract base class of the pattern 'Visitor'. \n
    It serves for the reference of a concrete visitor. \n \~
  \ingroup Base_Tools
*/
// ---
class MATH_CLASS Visitor
{
public:
  virtual ~Visitor(){}; ///< \ru Деструктор. \en Destructor. 
};


//------------------------------------------------------------------------------
/** \brief \ru Обобщенное объявление поддержки посещения объекта конкретным посетителем.
           \en A generallized declaration of support of the object visit by a concrete visitor. \~
  \details \ru Обобщенное объявление поддержки посещения объекта конкретным посетителем. \n
    Конкретный посетитель обязан наследовать от этой обобщенной реализации 
    для каждого типа посещаемых объектов. \n
           \en A generalized declaration of support of the object visit by a concrete visitor. \n
    A concrete visitor should inherit from this generalized implementation 
    for each type of visited objects. \n \~
  \ingroup Base_Tools
*/
// ---
template<class T> class
#if !defined ( C3D_WINDOWS )
MATH_CLASS 
#endif
VisitorImpl
{
public:
  virtual void Visit( T & ) = 0; ///< \ru Функция, обрабатывающая посещение объекта. \en A function processing a visit of an object. 
};


//------------------------------------------------------------------------------
/** \brief \ru Реализация функции, инициирующей посещение объекта.
           \en Implementation of a function initializing a visit of an object. \~
  \details \ru Реализация функции, инициирующей посещение объекта. \n
    Сделана через динамическое приведение типов ради реализации асимметричного посетителя 
    (см. Александреску "Modern C++ Design").
    Необходимо объявлять в любом классе, поддерживающем посещение.
           \en Implementation of a function initializing a visit of an object. \n
    It is made by the dynamic cast in order to implement an asymmetric visitor 
    (see Alexandrescu "Modern C++ Design").
    There is necessary to declare this in every class which supports a visitor. \~
  \ingroup Base_Tools
*/
// ---
#define VISITING_CLASS( Class )                                                       \
  public:                                                                             \
    void Accept( Visitor & visitor ) override                                 \
    {                                                                                 \
      VisitorImpl<Class> * impl = dynamic_cast<VisitorImpl<Class> *>(&visitor);       \
      if( impl )                                                                      \
        impl->Visit( *this );                                                         \
    }


#endif // __TEMPL_VISITOR_H
