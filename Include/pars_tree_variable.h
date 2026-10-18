////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Интерфейс переменной.
         \en Interface of variable.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ITTREEVARS_H
#define __ITTREEVARS_H

#include <io_base.h>
#include <reference_item.h>
#include <tool_cstring.h>
#include <templ_ss_array.h>

class  DefRange;
class  BTreeNode;
struct DerivesValues;


//------------------------------------------------------------------------------
/** \brief \ru Типы узлов бинарного дерева.
           \en Types nodes of binary tree. \~
  \details \ru Типы узлов бинарного дерева.\n
           \en Types nodes of binary tree.\n \~
  \ingroup Parser
*/
// ---
enum BteNodeType
{
  bt_Const,           ///< \ru Константа. \en Constant. 
  bt_Ident,           ///< \ru Идентификатор. \en Identifier. 
  bt_Function,        ///< \ru Функция. \en A function. 
  bt_Operation2Args,  ///< \ru Операция c двумя аргументами. \en Operation with two arguments. 
  bt_Operation1Arg,   ///< \ru Операция c одним аргументом. \en Operation with one argument. 
  bt_Operation3Args,  ///< \ru Операция c тремя аргументами. \en Operation with three arguments. 
};


//------------------------------------------------------------------------------
/** \brief \ru Типы операций.
           \en Operations types. \~
  \details \ru Типы операций.\n
           \en Operations types.\n \~
  \attention \ru Значения пишутся в файл. \en Values are written in file. \~
  \ingroup Parser
*/
// ---  
enum PceOperationType
{
  oprt_TernaryOperation = 0,                    ///< \ru Тернарная операция. \en Ternary operation. 
  oprt_BinaryOperation = 8,                     ///< \ru Бинарная операция. \en Binary operation. 
  oprt_Addition        = oprt_BinaryOperation,  ///< \ru Сложение. \en Addition. 
  oprt_Subtraction     = 9,                     ///< \ru Вычитание. \en Subtraction. 
  oprt_Division        = 10,                    ///< \ru Деление. \en Division. 
  oprt_Multiplication  = 11,                    ///< \ru Умножение. \en Multiplication 
  oprt_IntDivision     = 12,                    ///< \ru Целочисленное деление. \en Integer division. 
  oprt_OR              = 13,                    ///< \ru Или. \en Or. 
  oprt_AND             = 14,                    ///< \ru И. \en And. 
  oprt_NEQU            = 15,                    ///< \ru Не равно. \en Not equal. 
  oprt_EQU             = 16,                    ///< \ru Равно. \en Equal. 
  oprt_GT              = 17,                    ///< \ru Больше. \en More. 
  oprt_GE              = 18,                    ///< \ru Больше или равно. \en More or equal. 
  oprt_LT              = 19,                    ///< \ru Меньше. \en Less. 
  oprt_LE              = 20,                    ///< \ru Меньше или равно. \en Less or equal. 
  oprt_Involution      = 21,                    ///< \ru Возведение в степень. \en Involution. 
  oprt_UnaryOperation  = 22,                    ///< \ru Унарная операция. \en Unary operation. 
  oprt_NOT = oprt_UnaryOperation,               ///< \ru Не. \en Not. 
  oprt_UnaryMinus      = 23,                    ///< \ru Унарный минус. \en Unary minus. 
  oprt_UnaryPlus       = 24,                    ///< \ru Унарный плюс. \en Unary plus. 
  oprt_Parentheses     = 25                     ///< \ru Скобки. \en Brackets. 
};


//-----------------------------------------------------------------------------
/** \brief \ru Коды результата разбора строки.
           \en Result codes of string parsing. \~
  \details \ru Коды результата разбора строки.\n
           \en Result codes of string parsing.\n \~
  \ingroup Parser
*/
// ---
enum EquTreeResCode {
  // \ru Эта группа кодов ошибок полностью повторяет то, что было до V9. \en This group of result codes fully repeats all that was before V9. 
  equTreeResCode_Ok = 0,                              ///< \ru Все хорошо. \en Everything is OK. 
  equTreeResCode_First = 1,                           ///< \ru Начало диапазона ошибок. \en Start of errors range. 
  equTreeResCode_SyntaxError = equTreeResCode_First,  ///< \ru Ошибка: Синтаксическая ошибка в выражении. \en Error: Syntax error in expression. 
  equTreeResCode_TooComplex,                          ///< \ru Ошибка: Слишком сложное выражение. \en Error: Expression is too complex. 
  equTreeResCode_InvalidAssignment,                   ///< \ru Ошибка: Переменная присваивается самой себе. \en Error: Variable is assigned by itself. 
  equTreeResCode_NoVariables,                         ///< \ru Ошибка: В выражении должна быть хотя бы одна переменная. \en Error: There should be at least one variable in expression. 
  equTreeResCode_TooLargeIdent,                       ///< \ru Ошибка: Превышено количество символов в имени переменной. \en Error: The number of symbols in a name of variable is exceeded. 
  equTreeResCode_TangentsDomain ,                     ///< \ru Ошибка: Аргумент тангенса не в области определения. \en Error: An argument of tangent is out of domain. 
  equTreeResCode_SqrtDomain,                          ///< \ru Ошибка: Недопустимое значение аргумента для sqrt. \en Error: Invalid argument value for sqrt. 
  equTreeResCode_LogarithmDomain,                     ///< \ru Ошибка: Недопустимое значение аргумента для логарифмической функции. \en Error: Invalid argument value for logarithmic function. 
  equTreeResCode_ZeroDivide,                          ///< \ru Ошибка: Деление на ноль. \en Error: Division by zero. 
  equTreeResCode_TrigonometricDomain,                 ///< \ru Ошибка: Аргумент тригонометрической функции не в области определения. \en Error: An argument of trigonometric function is out of domain. 
  equTreeResCode_CyclicRelation,                      ///< \ru Ошибка: Найдена замкнутая зависимость. \en Error: There is found a closed dependence. 
  equTreeResCode_PowDomain,                           ///< \ru Ошибка: недопустимое значение аргумента для степенной функции. \en Error: Invalid argument value for power function. 
  equTreeResCode_WrongFuncFormat,                     ///< \ru Ошибка: Выражение содержит функцию, не соответствующую своему формату. \en Error: Expression contains a function which does not correspond to its format. 
  equTreeResCode_InvalidVariableType,                 ///< \ru Ошибка: Тип переменной не соответствует типу переменной в выражении. \en Error: The variable type does not match the variable type in the expression.

  // \ru ДОБАВЛЯТЬ НОВЫЕ СООБЩЕНИЯ ТОЛЬКО ПЕРЕД ЭТОЙ СТРОКОЙ; \en ADD NEW MESSAGES ONLY BEFORE THIS STRING; 
  equTreeResCode_Last                                 ///< \ru Конец диапазона ошибок. \en End of errors range. 
};


//------------------------------------------------------------------------------
/** \brief \ru Интерфейс координаты.
           \en Interface of coordinate. \~
  \details \ru Интерфейс координаты. \n
           \en Interface of coordinate. \n \~
  \ingroup Parser
*/
// ---
struct ItCoord
{
  virtual void   SetValue( double v ) = 0; ///< \ru Установить переменную. \en Set variable. 
  virtual double GetValue() const     = 0; ///< \ru Дать переменную. \en Get variable. 
};


//-----------------------------------------------------------------------------
/** \brief \ru Типы переменной дерева.
           \en Types of tree variable. \~
  \details \ru Типы переменной дерева.
           \en Types of tree variable. \~
  \ingroup Parser
*/
// ---
enum class VarType
{
  vr_double,  ///< \ru Вещественная переменная. \en Real variable.
  vr_string   ///< \ru Строковая переменная.    \en String variable.
};


//------------------------------------------------------------------------------
/** \brief \ru Интерфейс переменной.
           \en Interface of variable. \~
  \details \ru Интерфейс переменной. \n
           \en Interface of variable. \n \~
  \ingroup Parser
*/
// ---    
struct MATH_CLASS ItTreeVariable
{
  ItTreeVariable() {}
  virtual ~ItTreeVariable() {}

  /// \ru Дать имя. \en Get name.
  virtual const c3d::string_t & GetName()                         const = 0;
  /// \ru Установить имя. \en Set name.
  virtual       void            SetName( const c3d::string_t & )        = 0;
  /// \ru Дать переменную. \en Get variable. 
  virtual       double          GetValue()                        const = 0;
  /// \ru Установить переменную. \en Set variable. 
  virtual       void            SetValue( double )                      = 0;
  /// \ru Дать строковую переменную. \en Get string variable.
  virtual       void            GetStringValue( c3d::string_t & ) const = 0;
  /// \ru Установить строковую переменную. \en Set string variable.
  virtual       void            SetStringValue( const c3d::string_t & ) = 0;
  /// \ru Дать координату. \en Get coordinate. 
  virtual const ItCoord &       GetCoord()                        const = 0;
  /// \ru Вычислить размер в байтах. \en Get size in bytes. 
  virtual       size_t          SizeOf()                          const = 0;
  /// \ru Дать тип переменной. \en Get type of variable.
  virtual       VarType         IsA()                             const = 0;

  /// \ru Захватить \en Catch. 
  virtual refcount_t            AddRef ()                         const = 0;
  /// \ru Отпустить. \en Free. 
  virtual refcount_t            Release()                         const = 0;

  /// \ru Установить имя. Обработка нулевого указателя. \en Set name. Null pointer processing.
  virtual       void            SetName( const TCHAR * s )              = 0;

  /// \ru Операторы чтения, записи. \en Reading and writing operators. 
  DECLARE_PERSISTENT_OPS_BASE( ItTreeVariable, MATH_FUNC_EX )
};


//------------------------------------------------------------------------------
/** \brief \ru Интерфейс интервальной переменной.
           \en Interface of interval variable. \~
  \details \ru Интерфейс интервальной переменной. \n
           \en Interface of interval variable. \n \~
  \ingroup Parser
*/
// ---      
struct ItIntervalTreeVariable
{
  ItIntervalTreeVariable() {}

  /// \ru Дать имя. \en Get name. 
  virtual const c3d::string_t & GetName() const = 0;             
  /// \ru Установить имя. \en Set name. 
  virtual void                  SetValue( double f, double s ) = 0;  
  /// \ru Первая граница. \en The first boundary. 
  virtual double                First()   const = 0;             
  /// \ru Вторая граница. \en The second boundary. 
  virtual double                Second()  const = 0;             
 
  /// \ru Захватить. \en Catch. 
  virtual refcount_t            AddRef()  const = 0;             
  /// \ru Отпустить. \en Free. 
  virtual refcount_t            Release() const = 0;             
  
  /// \ru Операторы чтения, записи. \en Reading and writing operators. 
  DECLARE_PERSISTENT_OPS_BASE( ItIntervalTreeVariable, MATH_FUNC_EX )
};


class MbUserFunc;     

//------------------------------------------------------------------------------
/** \brief \ru Интерфейс функции.
           \en Interface of function. \~
  \details \ru Интерфейс функции. \n
           \en Interface of function. \n \~
  \ingroup Parser
*/
// ---   
struct MATH_CLASS ItUserFunc : public MbRefItem
{
  ItUserFunc() {};
  virtual ~ItUserFunc() {};

  /// \ru Дать копию объекта. \en Get a copy of the object. 
  virtual       ItUserFunc &    Duplicate()                                       const = 0; 
  /// \ru Дать имя. \en Get name. 
  virtual const c3d::string_t & GetName()                                         const = 0; 
  /// \ru Дать количество параметров. \en Get count of parameters.. 
  virtual       size_t          GetParsCount()                                     const = 0; 
  /// \ru Значение функции. \en Value of function. 
  virtual       EquTreeResCode  GetValue    ( const SArray<double> &, double & )   const = 0; 
  /// \ru Значение функции и производных. \en Value of function and derivatives. 
  virtual       EquTreeResCode  GetDerivates( const SArray<DerivesValues> &,
                                              double & v, double & f,
                                              double & s, double & t,
                                              size_t dIndex = 0  )                 const = 0; 
  /// \ru Внешние переменные. \en External variables. 
  virtual       void           GetExternalVars( SSArray<ItTreeVariable *> & vars,
                                                SSArray<ItUserFunc *> & funcs    ) const = 0; 
  /// \ru Только для внутреннего использования! Степень функции по индексу переменной. \en For internal use only! Function degree by the index of variable. 
  virtual       size_t         GetPseudoOrderByPar  ( size_t index )               const = 0;
  /// \ru Область определения. \en Domain. 
  virtual       bool           GetDefRange( DefRange &, ItTreeVariable & var,
                                      const std::vector<BTreeNode *> & )           const = 0; 
  /// \ru Равны ли функции. \en Whether functions are equal. 
  virtual       bool           IsEqual    ( const ItUserFunc & other )             const = 0; 
  /// \ru Равны ли функции. \en Whether functions are equal. 
  virtual       bool           IsEqual    ( const MbUserFunc & other )             const = 0; 

  /// \ru Операторы чтения, записи. \en Reading and writing operators. 
  DECLARE_PERSISTENT_OPS_BASE( ItUserFunc, MATH_FUNC_EX )
};

#endif //__ITTREEVARS_H