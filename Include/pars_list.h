////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Список переменных.
         \en List of variables. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __PARS_LIST_H
#define __PARS_LIST_H


#include <tool_cstring.h>
#include <templ_p_array.h>
#include <pars_var.h>
#include <math_define.h>


//------------------------------------------------------------------------------
/** \brief \ru Список переменных.
           \en List of variables. \~
  \details \ru Список переменных символьной записи выражения. \n
           \en A list of variables with symbol writing of expression. \n \~
  \ingroup Parser
*/
// ---
class MATH_CLASS MbListVars {
private:
  RPArray<MbVar> _vars; ///< \ru Список переменных. \en List of variables. 

public:
  MbListVars();           ///< \ru Пустой конструктор. \en Empty constructor. 
  virtual ~MbListVars();  ///< \ru Деструктор. \en Destructor. 

public:
  /// \ru Найти переменную по имени. \en Find variable by name. 
  MbVar * FindVariable  ( const c3d::string_t & name ) const; 
  /// \ru Получить переменную по индексу. \en Get variable by index. 
  MbVar * GetVariable   ( size_t index ) const;
  /// \ru Количество переменных. \en The number of variables. 
  size_t  CountVariables() const;
  /// \ru Добавить переменную. \en Add a variable. 
  void    AddVariable   ( MbVar * );
  /// \ru Убрать переменную. \en Remove a variable. 
  void    RemoveVariable( MbVar * );

OBVIOUS_PRIVATE_COPY( MbListVars )
};


#endif  // __PARS_LIST_H