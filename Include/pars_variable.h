////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Переменная.
         \en Variable. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MBVARIABLE_H
#define __MBVARIABLE_H

#include <pars_tree_variable.h>
#include <math_define.h>
#include <mb_variables.h>
#include <io_tape_define.h>
#include <io_define.h>


//-----------------------------------------------------------------------------
/** \brief \ru Координата.
           \en Coordinate. \~
  \details \ru Координата. \n
           \en Coordinate. \n \~
  \ingroup Parser
*/
// ---
class MATH_CLASS MbCoord : public ItCoord
{
private:
  double m_coord; // \ru Значение координаты. \en A value of coordinate. 
public:
  /// \ru Конструктор по значению. \en Constructor by the value. 
  MbCoord( double v ) : m_coord(v) {}
  /// \ru Установить значение. \en Set value. 
  virtual void   SetValue( double v ){ m_coord = v; }
  /// \ru Получить значение. \en Get the value. 
  virtual double GetValue() const    { return m_coord; }
  /// \ru Вычислить размер координаты в байтах. \en Get size of coordinate in bytes. 
  size_t SizeOf() const      { return sizeof(double);}

  /// \ru Операторы чтения, записи. \en Reading and writing operators. 
  KNOWN_OBJECTS_RW_REF_OPERATORS( MbCoord );
};


//-----------------------------------------------------------------------------
/** \brief \ru Вещественная переменная.
           \en Real variable. \~
  \details \ru Вещественная переменная. \n
           \en Real variable. \n \~
  \ingroup Parser
*/
// ---
class MATH_CLASS MbTreeVariable : public TapeBase, public ItTreeVariable
{
private:
  MbCoord         m_coord;   ///< \ru Координата. \en Coordinate.
  c3d::string_t   m_name;    ///< \ru Имя переменной. \en A name of variable. 
  mutable size_t  useCount;  ///< \ru Количество использований. \en The number of uses. 

public:
  /// \ru Конструктор по имени и значению. \en Constructor by the name and the value.
  MbTreeVariable( const c3d::string_t & name, double v );
  /// \ru Деструктор. \en Destructor.
  virtual ~MbTreeVariable();

public:
  /// \ru Получить имя. \en Get name. 
  const  c3d::string_t &   GetName  () const override                { return m_name; }
  /// \ru Установить имя. \en Set name. 
         void       SetName  ( const c3d::string_t & name ) override { m_name = name; }
  /// \ru Установить имя. Обработка нулевого указателя. \en Set name. Null pointer processing.
         void        SetName( const TCHAR* s ) override { m_name.assign( s ? s : _T("") ); };
  /// \ru Получение значение. \en Get value.
          double     GetValue () const override     { return m_coord.GetValue(); }
  /// \ru Установить значение. \en Set value.
          void       SetValue ( double v ) override { m_coord.SetValue( v ); }
  /// \ru Дать строковую переменную. \en Get string variable.
          void       GetStringValue( c3d::string_t & ) const override {}
  /// \ru Установить строковую переменную. \en Set string variable.
          void       SetStringValue( const c3d::string_t & ) override {}
  /// \ru Получить координату. \en Get coordinate.
  const  MbCoord &  GetCoord () const override { return m_coord; }
  /// \ru Вычислить размер переменной в байтах. \en Get size of variable in bytes.
          size_t     SizeOf() const override { return m_coord.SizeOf() + sizeof(m_name) + sizeof(size_t); }
  /// \ru Дать тип переменной. \en Get type of variable.
          VarType    IsA() const override { return VarType::vr_double; }
  /// \ru Создать копию переменной. \en Create a copy of variable. 
          MbTreeVariable &  Duplicate() const { return *new MbTreeVariable( GetName(), GetValue() ); }
  /// \ru Увеличить счетчик использований. \en Increase a counter of uses. 
  refcount_t            AddRef () const override { return ++useCount; }
  /// \ru Уменьшить счетчик использований и удалить объект, если он никому уже не нужен. \en Decrease a counter of uses and delete an object if it is not used any more. 
  refcount_t            Release() const override;

/// \ru Операторы чтения, записи. \en Reading and writing operators. 
DECLARE_PERSISTENT_CLASS_NEW_DEL( MbTreeVariable )
};

IMPL_PERSISTENT_OPS( MbTreeVariable )


//-----------------------------------------------------------------------------
/** \brief \ru Строковая переменная дерева разбора выражения.
           \en String variable for an expression parse tree. \~
  \details \ru Строковая переменная дерева разбора выражения.
           \en String variable for an expression parse tree. \~
  \ingroup Parser
*/
// ---
class MATH_CLASS MbTreeStringVariable : public TapeBase, public ItTreeVariable
{
private:
  c3d::string_t   m_name;     ///< \ru Имя переменной. \en A name of variable. 
  c3d::string_t   m_value;    ///< \ru Строковое значение переменной. \en A string value of variable.
  MbCoord         m_coord;    ///< \ru Вспомогательный объект, используется для совместимости интерфейса. \en Auxiliary object, used for interface compatibility.
  mutable size_t  m_useCount; ///< \ru Количество использований. \en The number of uses. 


public:
  /// \ru Конструктор по имени и значению. \en Constructor by the name and the value.
  MbTreeStringVariable( const c3d::string_t & name, const c3d::string_t & value )
    : m_name    ( name          )
    , m_value   ( value         )
    , m_coord   ( -MB_MAXDOUBLE )
    , m_useCount( 0             )
  {}
  
  /// \ru Деструктов. \en Destructor.
  virtual ~MbTreeStringVariable() {}

public:
  /// \ru Получить имя. \en Get name.
  const  c3d::string_t & GetName () const override { return m_name; }
  /// \ru Установить имя. \en Set name. 
  void                   SetName ( const c3d::string_t & name ) override { m_name = name; }
  /// \ru Установить имя. Обработка нулевого указателя. \en Set name. Null pointer processing.
  void                   SetName( const TCHAR* s ) override { m_name.assign(s ? s : _T("")); };

  /// \ru Дать строковую переменную. \en Get string variable.
  void                   GetStringValue( c3d::string_t & value ) const override { value = m_value; }
  /// \ru Установить строковую переменную. \en Set string variable.
  void                   SetStringValue( const c3d::string_t & value ) override { m_value = value; }
  /// \ru Вычислить размер переменной в байтах. \en Get size of variable in bytes.
  size_t                 SizeOf() const override { return sizeof(m_name) + sizeof(m_value) + sizeof(MbCoord) + sizeof(size_t); }
  /// \ru Дать тип переменной. \en Get type of variable.
  VarType                IsA() const override { return VarType::vr_string; }
  /// \ru Создать копию переменной. \en Create a copy of variable.
  MbTreeStringVariable & Duplicate() const { return *new MbTreeStringVariable(m_name, m_value); }

  /// \ru Дать переменную. \en Get variable.
  double                 GetValue() const override { return -MB_MAXDOUBLE; }
  /// \ru Установить переменную. \en Set variable.
  void                   SetValue( double ) override {}
  /// \ru Дать координату. \en Get coordinate.
  const ItCoord &        GetCoord() const override { return m_coord; }

  /// \ru Увеличить счетчик использований. \en Increase a counter of uses. 
  refcount_t             AddRef () const override { return ++m_useCount; }
  /// \ru Уменьшить счетчик использований и удалить объект, если он никому уже не нужен. \en Decrease a counter of uses and delete an object if it is not used any more. 
  refcount_t             Release() const override;

/// \ru Операторы чтения, записи. \en Reading and writing operators. 
DECLARE_PERSISTENT_CLASS_NEW_DEL( MbTreeStringVariable )
};


IMPL_PERSISTENT_OPS( MbTreeStringVariable )


#endif // __MBVARIABLE_H
