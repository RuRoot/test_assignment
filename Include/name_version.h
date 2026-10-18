////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Версия топологического имени объекта.
         \en Version of an object topological name. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __NAME_VERSION_H
#define __NAME_VERSION_H


#include <math_define.h>
#include <io_version_container.h>
#include <templ_s_array_rw.h>

////////////////////////////////////////////////////////////////////////////////
//
/** \brief \ru Версия имени.
           \en Version of a name. \~
  \details \ru Версия имени. \n
           \en Version of a name. \n \~
  \ingroup Names
*/
class MATH_CLASS MbNameVersion {
  VersionContainer m_ver; /// \ru Контейнер версий. \en Container of versions. 

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbNameVersion() : m_ver() {}
  /// \ru Конструктор копирования. \en Copy-constructor. 
  explicit MbNameVersion( const VersionContainer & ver ) : m_ver( ver ) {}
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbNameVersion( const MbNameVersion & o ) : m_ver( o.m_ver ) {}
  /// \ru Установить версию имени по умолчанию. \en Set default version of a name. 
  void  SetDefault();
  /// \ru Установить версию в контейнере версий по индексу. \en Set the version in container of versions by an index. 
  void  SetVersion ( size_t index, VERSION ver ) { m_ver.SetVersion( index, ver ); }
  /// \ru Инициализировать версию имени другим контейнером версий. \en Initialize version of a name by other container of versions. 
  void  Init       ( const VersionContainer & vers ) { m_ver = vers; }

  /// \ru Получить контейнер версий. \en Get the container of versions. 
  const VersionContainer & GetVersionContainer() const { return m_ver; }
  /// \ru Оператор получения математической версии. \en Operator for obtaining a mathematical version. 
  operator VERSION () const { return m_ver.GetMathVersion(); }

  /// \ru Оператор равенства. \en An equality operator. 
  bool operator == ( const MbNameVersion & v ) const { return (v.GetVersionContainer().GetMathVersion() == m_ver.GetMathVersion()); }
  /// \ru Оператор неравенства. \en Inequality operator. 
  bool operator != ( const MbNameVersion & v ) const { return (v.GetVersionContainer().GetMathVersion() != m_ver.GetMathVersion()); }
  /// \ru Оператор больше.const MbNameVersion & \en "Greater than" operator. 
  bool operator >  ( const MbNameVersion & v ) const { return (v.GetVersionContainer().GetMathVersion() <  m_ver.GetMathVersion()); }
  /// \ru Оператор больше или равно. \en "Greater than or equal to" operator. 
  bool operator >= ( const MbNameVersion & v ) const { return (v.GetVersionContainer().GetMathVersion() <= m_ver.GetMathVersion()); }
  /// \ru Оператор меньше. \en "Less than" operator. 
  bool operator <  ( const MbNameVersion & v ) const { return (v.GetVersionContainer().GetMathVersion() >  m_ver.GetMathVersion()); }
  /// \ru Оператор меньше или равно. \en "Less than or equal to" operator. 
  bool operator <= ( const MbNameVersion & v ) const { return (v.GetVersionContainer().GetMathVersion() >= m_ver.GetMathVersion()); }

  /// \ru Оператор равенства. \en An equality operator. 
  bool operator == ( VERSION v )         const { return (v == m_ver.GetMathVersion()); }
  /// \ru Оператор неравенства. \en Inequality operator. 
  bool operator != ( VERSION v )         const { return (v != m_ver.GetMathVersion()); }
  /// \ru Оператор больше. \en "Greater than" operator. 
  bool operator >  ( VERSION v )         const { return (v <  m_ver.GetMathVersion()); }
  /// \ru Оператор больше или равно. \en "Greater than or equal to" operator. 
  bool operator >= ( VERSION v )         const { return (v <= m_ver.GetMathVersion()); }
  /// \ru Оператор меньше. \en "Less than" operator. 
  bool operator <  ( VERSION v )         const { return (v >  m_ver.GetMathVersion()); }
  /// \ru Оператор меньше или равно. \en "Less than or equal to" operator. 
  bool operator <= ( VERSION v )         const { return (v >= m_ver.GetMathVersion()); }
  
  /// \ru Оператор равенства. \en An equality operator. 
  bool operator == ( int32 v )         const { return ((VERSION)v == m_ver.GetMathVersion()); }
  /// \ru Оператор неравенства. \en Inequality operator. 
  bool operator != ( int32 v )         const { return ((VERSION)v != m_ver.GetMathVersion()); }
  /// \ru Оператор больше. \en "Greater than" operator. 
  bool operator >  ( int32 v )         const { return ((VERSION)v <  m_ver.GetMathVersion()); }
  /// \ru Оператор больше или равно. \en "Greater than or equal to" operator. 
  bool operator >= ( int32 v )         const { return ((VERSION)v <= m_ver.GetMathVersion()); }
  /// \ru Оператор меньше. \en "Less than" operator. 
  bool operator <  ( int32 v )         const { return ((VERSION)v >  m_ver.GetMathVersion()); }
  /// \ru Оператор меньше или равно. \en "Less than or equal to" operator. 
  bool operator <= ( int32 v )         const { return ((VERSION)v >= m_ver.GetMathVersion()); }
  
  /// \ru Оператор присваивания. \en An assignment operator. 
  void operator =  ( const MbNameVersion & o )       { m_ver = o.m_ver; }
private:
  static VERSION GetIOVersion( uint8 v, VERSION ver );
  static uint8   GetVersion  ( VERSION iov );

KNOWN_OBJECTS_RW_REF_OPERATORS( MbNameVersion )
};


//------------------------------------------------------------------------------
//
// ---
inline
reader & CALL_DECLARATION operator >> ( reader & in, MbNameVersion & ref )
{
  VERSION version = in.MathVersion();

  if ( version < 0x0590004FL  ) {
    ref.m_ver.Flush();
    ref.m_ver.SetVersion( 0/*System*/, version );
  }
  else if ( version < 0x07000104L ) {
    uint8 v;
    in >> v;
    ref.m_ver.Flush();
    ref.m_ver.SetVersion( 0/*System*/, MbNameVersion::GetIOVersion( v, version ) );
  }
  else {
    in >> ref.m_ver;
  }

  return in;
}


//------------------------------------------------------------------------------
//
// ---
inline
writer & CALL_DECLARATION operator << ( writer & out, const MbNameVersion & ref )
{
  VERSION version = out.MathVersion();
  if ( version < 0x07000104L ) {
    out << MbNameVersion::GetVersion( version );
  }
  else {
    out << ref.m_ver; 
  }

  return out;
}


#endif // __NAME_VERSION_H
