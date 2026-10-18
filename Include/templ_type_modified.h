////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Объект с флагом модификации.
         \en An object with the modifications flag. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_TYPE_MODIFIED_H
#define __TEMPL_TYPE_MODIFIED_H


#include <io_define.h>


////////////////////////////////////////////////////////////////////////////////
//
// \ru Класс рассчитан на Type только bool,bool,int,uint,int16,uint16,float,double, теперь работает и с более сложными объектами, но осторожно с деструкторами Type!
// \en The class is intended only for types bool,bool,int,uint,int16,uint16,float,double now it works with more complex objects too, but be careful with the destructors of the class 'Type'!
//
////////////////////////////////////////////////////////////////////////////////

FORVARD_DECL_TEMPLATE_TYPENAME( class TypeModified );
FORVARD_DECL_TEMPLATE_TYPENAME( reader& CALL_DECLARATION operator >> ( reader& in, TypeModified<Type> & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer& CALL_DECLARATION operator << ( writer& out, const TypeModified<Type> & ref ) );


template <class Type>
class TypeModified {
private:
  bool modified_m;
  Type value_m;

  typedef int WriteType;
  enum ValInit { valInit = 0 };

public :
  TypeModified( ValInit val = valInit, bool modified = false ); // \ru ИР K7 для TRect тут компилятор сгенерирует ошибку \en ИР K7 here the compiler will generate an error for TRect 
  TypeModified( const Type & val, bool modified = false );
  TypeModified( const TypeModified & );
  virtual ~TypeModified();

  void  SetValue   ( Type val, bool modified = true );
  void  SetValueRef( const Type & val, bool modified = true );
  const Type & GetValue () const;
  operator const Type & () const { return value_m; }
  
  void  Assign     ( const TypeModified & );

  bool  IsModified () const;
  void  SetModified( bool modified );

private:
  void operator = ( const TypeModified & ); // \ru запрещено \en forbidden  

// ID K8 KNOWN_OBJECTS_RW_REF_OPERATORS(TypeModified<Type> )
  TEMPLATE_FRIEND reader& CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader& in, TypeModified<Type> & ref );
  TEMPLATE_FRIEND writer& CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer& out, const TypeModified<Type> & ref );
};


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline TypeModified<Type>::TypeModified( /*TypeModified::*/ValInit val, bool modified )
  : modified_m( modified )
  , value_m( val )  
{
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline TypeModified<Type>::TypeModified( const Type & val, bool modified )
  : modified_m( modified )
  , value_m( val )
{
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline TypeModified<Type>::TypeModified( const TypeModified & other )
  : modified_m( other.modified_m )
  , value_m( other.value_m )
{
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline TypeModified<Type>::~TypeModified() {
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline void TypeModified<Type>::SetValue( Type val, bool modified ) {
  value_m = val;
//MA K9   modified_m = modified_m ? true : modified;
  modified_m |= modified;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline void TypeModified<Type>::SetValueRef( const Type & val, bool modified ) {
  value_m = val;  
//MA K9   modified_m = modified_m ? true : modified;
  modified_m |= modified;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline void TypeModified<Type>::Assign( const TypeModified & other ) {
  value_m = other.value_m;
  modified_m = other.modified_m;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline const Type & TypeModified<Type>::GetValue() const {
  return value_m;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline bool TypeModified<Type>::IsModified() const {
  return modified_m;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline void TypeModified<Type>::SetModified( bool modified ) {
  modified_m = modified;
}


//------------------------------------------------------------------------------
// \ru В Visual Studio 2017 и более поздних версиях с включенной опцией /permissive- компилятор анализирует
// определения шаблонных функций и классов, идентифицируя используемые в них зависимые и независимые имена,
// как требует двухфазный поиск имен.
// В частности, независимые имена, которые не объявляются в контексте определения шаблона,
// вызывают диагностическое сообщение в соответствии с требованиями стандартов ISO C++.
// Поэтому в шаблонных функциях int заменен на зависимый тип WriteType, объявленный в шаблонном классе TypeModified.
// \en When the /permissive- option is set, the MSVC compiler (in Visual Studio 2017 and later)
// parses function and class template definitions, identifying dependent and non-dependent names
// used in templates as required for two-phase name look-up. 
// In particular, non-dependent names that aren't declared in the context of a template definition
// cause a diagnostic message as required by the ISO C++ standards.
// Therefore, in the template functions int is replaced with the dependent type WriteType, declared in the TypeModified template class.
// See https://docs.microsoft.com/ru-ru/cpp/build/reference/permissive-standards-conformance?view=msvc-160#two-phase-name-look-up// ---
// ---
template <class Type>
reader& operator >> ( reader& in, TypeModified<Type>& ref ) {
  in >> ref.value_m; // \ru ИР K7 >> ref.modified_m; \en ИР K7 >> ref.modified_m; 
  typename TypeModified<Type>::WriteType m;
  in >> m;
  ref.modified_m = !!m;
  return in;
}


//------------------------------------------------------------------------------
//
//---
template <class Type>
writer& operator << ( writer& out, const TypeModified<Type>& ref ) {
  return out << ref.value_m << (typename TypeModified<Type>::WriteType)ref.modified_m;
}


#endif // __TEMPL_TYPE_MODIFIED_H
