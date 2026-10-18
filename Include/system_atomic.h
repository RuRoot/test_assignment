////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Системозависимые атомарные операции.
         Если требуются атомарные операции, должен использоваться этот файл (<b>atomic</b> не использовать!!!).
         \en System-dependent atomic operations.
         If atomic operations are required, this file should be used (<b>atomic</b> must not be used!!!).\~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SYSTEM_ATOMIC_H
#define __SYSTEM_ATOMIC_H

#include <system_types.h>
#include <system_cpp_standard.h>
#include <math_define.h>


//------------------------------------------------------------------------------
// \ru Использование атомарных операций согласно стандарту C++11.
// \en Using atomic operations according to C++11 standard.
//     
//---

#include <atomic>

typedef std::atomic_ptrdiff_t use_count_type; ///< \ru Потокобезопасный тип счётчика ссылок. \en Thread-safe references count type.
typedef std::atomic_size_t    serial_type;    ///< \ru Потокобезопасный тип счётчика ссылок. \en Thread-safe references count type.
typedef std::atomic_schar     flag_type;      ///< \ru Потокобезопасный тип флага. \en Thread-safe flag type.
typedef std::atomic<bool>     atomic_bool;    ///< \ru Потокобезопасный логический тип. \en Thread-safe boolean type.
typedef std::atomic<double>   atomic_double;  ///< \ru Потокобезопасный double тип. \en Thread-safe double type.
typedef std::atomic<uint8>    atomic_uint8;  ///< \ru Потокобезопасный uint8 тип. \en Thread-safe uint8 type.

//------------------------------------------------------------------------------
/** \ru Получить значение. \en Get value.
*/
//---
template <class AtomicType, class Type>
Type LoadTypeValue( const AtomicType & v ) {
  return v.load();
}

//------------------------------------------------------------------------------
/** \ru Установить значение. \en Get value.
*/
//---
template <class AtomicType>
void StoreTypeValue( const AtomicType & src, AtomicType & dst ) {
  dst.store( src.load() );
}

//------------------------------------------------------------------------------
/** \ru Установить значение. \en Get value.
*/
//---
template <class AtomicType, class Type>
void StoreTypeValue( const Type src, AtomicType & dst ) {
  dst.store( src );
}

//------------------------------------------------------------------------------
/** \ru Получить значение. \en Get value.
*/
//---
inline size_t SerialTypeValue( const serial_type & v ) {
  return v.load();
}
inline ptrdiff_t UseCountValue( const use_count_type & v ) {
  return v.load();
}
inline double DoubleTypeValue( const atomic_double & v ) {
  return v.load();
}
inline double Uint8TypeValue( const atomic_uint8 & v ) {
  return v.load();
}

//------------------------------------------------------------------------------
// \ru Шаблонный класс для перечислений (Type - тип перечисления). 
// \en Template class for enums (Type - a type of enum).
//--
template< class Type >
class atomic_enum_type
{
  use_count_type  m_value;

public:
  atomic_enum_type() :m_value (0) {}
  atomic_enum_type( const Type & val ) : m_value (val) {}
  atomic_enum_type( const atomic_enum_type<Type> & val ) : m_value( val.m_value.load() ) {}

  // \ru Операторы присваивания. \en Assignment operators.
  atomic_enum_type & operator = ( const atomic_enum_type & t ) { m_value.store( t.m_value.load() ); return *this; }
  atomic_enum_type & operator = ( const Type & t ) { m_value.store( t ); return *this; }

  // \ru Операторы сравнения. \en Comparison operators.
  bool operator == ( const atomic_enum_type & t ) const { return m_value.load() == t.m_value.load(); }
  bool operator == ( const Type & t ) const { return m_value.load() == t; }
  bool operator != ( const atomic_enum_type & t ) const { return m_value.load() != t.m_value.load(); }
  bool operator != ( const Type & t ) const { return m_value.load() != t; }
  bool operator < ( const atomic_enum_type & t ) const { return m_value.load() < t.m_value.load(); }
  bool operator < ( Type t )  const { return m_value.load() < t; }
  bool operator > ( const atomic_enum_type & t ) const { return m_value.load() > t.m_value; }
  bool operator > ( const Type & t ) const { return m_value.load() > t; }
  bool operator <= ( const atomic_enum_type & t ) const { return m_value.load() <= t.m_value.load(); }
  bool operator <= ( const Type & t ) const { return m_value.load() <= t; }
  bool operator >= ( const atomic_enum_type & t ) const { return m_value.load() >= t.m_value.load(); }
  bool operator >= ( const Type & t ) const { return m_value.load() >= t; }

  // \ru Доступ к данным. \en Data access.
  Type operator()() const { return static_cast<Type>( m_value.load() ); }
};

namespace c3d
{
  typedef std::pair<atomic_double, atomic_double> AtomicDoublePair; ///< \ru Пара потокобезопасных действительных чисел двойной точности с плавающей запятой. \en Pair of thread-safe doubles. 
}

#endif  // __SYSTEM_ATOMIC_H
