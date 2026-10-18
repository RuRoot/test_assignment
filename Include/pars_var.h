////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Переменная.
         \en Variable. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __VAR_H
#define __VAR_H

#include <math_define.h>
#include <tool_cstring.h>
#include <reference_item.h>

//-----------------------------------------------------------------------------
/** \brief \ru Тип переменной.
           \en Type of a variable. \~
  \details \ru Тип переменной.
           \en Type of a variable. \~
  \ingroup Parser
*/
// ---
enum class MbVarType
{
  vr_double,  ///< \ru Вещественная переменная. \en Real variable.
  vr_string   ///< \ru Строковая переменная.    \en String variable.
};


//------------------------------------------------------------------------------
/** \brief \ru Переменная.
           \en Variable. \~
  \details \ru Переменная. \n
           \en Variable. \n \~
  \ingroup Parser
*/
// ---
class MATH_CLASS MbVar : public MbRefItem {
private:
  const c3d::string_t _name;  ///< \ru Имя переменной. \en A name of variable. 
  double              _value; ///< \ru Значение переменной. \en A value of variable. 

public:
  /// \ru Конструктор. \en Constructor. 
  MbVar( const c3d::string_t & name ); 
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbVar();             

public:   
          /// \ru Получить имя переменной. \en Get variable name. 
    const c3d::string_t & Name () const;              
          /// \ru Значение переменной. \en A value of variable. 
          double          Value() const;              
          /// \ru Присвоить значение переменной. \en Assign a value to a variable. 
          void            Assignment( double value ); 
          /// \ru Тип переменной. \en Value type.
  virtual MbVarType       IsA() const;

OBVIOUS_PRIVATE_COPY( MbVar )
};


//------------------------------------------------------------------------------
/** \brief \ru Строковая переменная.
           \en String variable. \~
  \details \ru Строковая переменная.
           \en String variable. \~
  \ingroup Parser
*/
// ---
class MATH_CLASS MbStringVar : public MbVar {
private:
  c3d::string_t _stringValue; ///< \ru Строковое значение переменной. \en A string value of variable.

public:
  /// \ru Конструктор. \en Constructor.
  MbStringVar( const c3d::string_t & name );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbStringVar();
  
  /// \ru Строковое значение переменной. \en A value of variable.
  const c3d::string_t & GetStringValue() const;
  /// \ru Присвоить значение переменной. \en Assign a value to a variable.
        void            SetStringValue( const c3d::string_t & stringValue );
  /// \ru Тип переменной. \en Value type.
        MbVarType       IsA() const override;

OBVIOUS_PRIVATE_COPY( MbStringVar )
};


#endif  // __VAR_H