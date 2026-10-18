////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Функции сериализации.
         \en Functions of serialization. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_RW_OPERATOR_H
#define __TEMPL_RW_OPERATOR_H


#include <io_tape.h>


//------------------------------------------------------------------------------
// \ru Функция чтения указателя на основе оператора чтения ссылки \en Function of pointer reading on the basis of reference reading operator. 
// \ru для объектов имеющих конструктор по умолчанию \en for objects which have constructor by default 
//---
template <class Type>
inline reader & ReadPtrByRefDCtor ( reader & in, Type *& ptr )
{
  ptr = nullptr;

  char exist;
  in >> exist;

  if ( exist ) {
    ptr = new Type; // \ru не прислали объект - сделаем новый \en if the object has not been sent then make the new one 
    in >> (*ptr);
  }

  return in;
}


//------------------------------------------------------------------------------
// \ru Функция чтения указателя на основе оператора чтения ссылки \en Function of pointer reading on the basis of reference reading operator 
// \ru для объектов имеющих конструктор для чтения/записи \en for objects which have constructor for reading/writing 
//---
template <class Type>
inline reader & ReadPtrByRefRWCtor ( reader & in, Type *& ptr )
{
  ptr = nullptr;

  char exist;
  in >> exist;

  if ( exist ) {
    ptr = new Type( tapeInit ); // \ru не прислали объект - сделаем новый \en if the object has not been sent then make the new one 
    in >> (*ptr);
  }

  return in;
}


//------------------------------------------------------------------------------
// \ru Функция записи указателя на основе оператора записи ссылки \en Function of pointer writing on the basis of reference writing operator 
//---
template <class Type>
inline writer & WritePtrByRef ( writer & out, const Type * ptr )
{
  char exist = (ptr != nullptr);
  out << exist;

  if ( exist )
    out <<(*ptr);
  
  return out;
}


//------------------------------------------------------------------------------
// \ru Реализация чтения/записи указателей в поток \en Implementation of reading/writing of pointers to stream 
// \ru на основе оператора записи ссылки \en on the basis of reference writing operator. 
// \ru для объектов имеющих конструктор для чтения/записи \en for objects which have constructor for reading/writing 
//---
#define KNOWN_OBJECTS_RW_PTR_OPERATORS_IMP_BY_REF(Class)    \
  reader & operator >> ( reader & in, Class *& ptr )        \
    { return ReadPtrByRefRWCtor<Class>(in, ptr); }          \
  writer & operator << ( writer & out, const Class * ptr )  \
    { return WritePtrByRef<Class>(out, ptr); }


//------------------------------------------------------------------------------
// \ru Реализация чтения/записи указателей в поток \en Implementation of reading/writing of pointers to stream 
// \ru на основе оператора записи ссылки \en on the basis of reference writing operator 
// \ru для объектов имеющих конструктор по умолчанию \en for objects which have constructor by default 
//---
#define KNOWN_OBJECTS_DEF_CTOR_RW_PTR_OPERATORS_IMP_BY_REF(Class) \
  reader & operator >> ( reader & in, Class *& ptr )              \
    { return ReadPtrByRefDCtor<Class>(in, ptr); }                 \
  writer & operator << ( writer & out, const Class * ptr )        \
    { return WritePtrByRef<Class>(out, ptr); }


#endif // __TEMPL_RW_OPERATOR_H
