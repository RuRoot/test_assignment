////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Алгоритм синтаксического разбора алгебраического выражения.
         \en Algorithm of syntax parsing of algebraic expression. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __YACC_H
#define __YACC_H

#include <math_define.h>
#include <pars_tree_variable.h>
#include <vector>

class   BTreeNode;
class   TreeIntervalNode;
struct            ItTreeVariable;
struct            ItUserFunc;
struct            ItIntervalTreeVariable;


/// \ru Максимальная длина переменной. \en Maximum length of variable. 
constexpr size_t MAX_VARIABLE_NAME_LENGTH = 512;
/// \ru Максимальная длина выражения. \en Maximum length of expression. 
constexpr size_t MAX_EQU_LENGTH = 8192; // C3D-5977.
/// \ru Максимальная высота дерева. \en Maximum height of tree.
constexpr size_t MAX_TREE_HEIGHT = 2047; // C3D-8352.


//-------------------------------------------------------------------------------
//
// ---
struct ItEquVarCreator
{
  virtual ~ItEquVarCreator()
  {
  }

  virtual ItTreeVariable * GetVariable( const c3d::string_t & )                                              = 0;
  virtual ItIntervalTreeVariable * GetIntervalVariable( const c3d::string_t & )                              = 0;
  virtual ItUserFunc *     GetFunc    ( const c3d::string_t &, size_t parCount )                             = 0;
  virtual bool             CreateFunc ( const c3d::string_t &, const std::vector<c3d::string_t> & parNames ) = 0;
  virtual bool             CreateInterval( const c3d::string_t & )                                           = 0;
  virtual bool             CreateVariable( const c3d::string_t &)                                            = 0;
  virtual ItTreeVariable * CreateUnnamedVariable( const c3d::string_t & )                                    = 0;

  virtual ItTreeVariable * GetVariable( const TCHAR * )                                                      = 0;
  virtual ItIntervalTreeVariable * GetIntervalVariable( const TCHAR * )                                      = 0;
  virtual ItUserFunc *     GetFunc    ( const TCHAR *, size_t parCount )                                     = 0;
  virtual bool             CreateInterval( const TCHAR * )                                                   = 0;
  virtual bool             CreateVariable( const TCHAR * )                                                   = 0;
  virtual unsigned int     GetUsedCoordsCount() const                                                        = 0;
  virtual ItTreeVariable & CreateAuxVar()                                                                    = 0;

  // \ru Использовались ли какие-либо переменные? \en Were any variables used? 
  virtual bool             WereVarsUsed() const                                                              = 0;
  // \ru Использовались ли несуществующие (новые) переменные? \en Were any not existed (new) variables used? 
  virtual bool             WereNewVarsUsed() const                                                           = 0;
};


//-------------------------------------------------------------------------------
/** \brief \ru Создать дерево уравнения (a = b + c) через параметрический калькулятор.
           \en Create a tree of equations (a = b + c) by parametric calculator. \~
  \details \ru Создать дерево уравнения  a = b + c) через параметрический калькулятор. \n
           \en Create a tree of equations (a = b + c) by parametric calculator. \n \~
  \return \ru Код результата операции.
          \en Operation result code. \~
  \ingroup Parser
*/
// ---
MATH_FUNC(EquTreeResCode) CreateBTreeForEquation( const c3d::string_t & equstr
                                                , ItEquVarCreator &     varsCreator
                                                , SPtr<BTreeNode> &     dRoot
                                                );


//-------------------------------------------------------------------------------
/** \brief \ru Создать дерево выражения (b + c + d) через параметрический калькулятор.
           \en Create a tree of equations (b + c + d) by parametric calculator. \~
  \details \ru Создать дерево выражения (b + c + d) через параметрический калькулятор. \n
           \en Create a tree of equations (b + c + d) by parametric calculator. \n \~
  \return \ru Код результата операции.
          \en Operation result code. \~
  \ingroup Parser
*/
// ---
MATH_FUNC(EquTreeResCode) CreateBTreeForExpression( const c3d::string_t &               equstr
                                                  , ItEquVarCreator *                   varsCreator
                                                  , SPtr<BTreeNode> &                   root
                                                  , std::unique_ptr<TreeIntervalNode> & iRoot
                                                  );


//-------------------------------------------------------------------------------
/** \brief \ru Создать переменную.
           \en Create variable. \~
  \details \ru Создать переменную по строке. \n
           \en Create variable by string. \n \~
  \return \ru Код результата операции.
          \en Operation result code. \~
  \ingroup Parser
*/
// ---
MATH_FUNC(EquTreeResCode) CreateVariable( const c3d::string_t & expression, ItEquVarCreator & );


//-------------------------------------------------------------------------------
/** \brief \ru Является ли выражение неравенством.
           \en Whether expression is inequation. \~
  \details \ru Является ли выражение неравенством. \n
           \en Whether expression is inequation. \n \~
  \ingroup Parser
*/
// ---
MATH_FUNC(bool) IsInequality( const c3d::string_t & );


//-------------------------------------------------------------------------------
/** \brief \ru Создать дерево уравнения (a = b + c) через параметрический калькулятор.
           \en Create a tree of equations (a = b + c) by parametric calculator. \~
  \details \ru Создать дерево уравнения  a = b + c) через параметрический калькулятор. \n
           \en Create a tree of equations (a = b + c) by parametric calculator. \n \~
  \return \ru Код результата операции.
          \en Operation result code. \~
  \ingroup Parser
*/
// ---
MATH_FUNC(EquTreeResCode) CreateBTreeForEquation( const TCHAR *     equstr
                                                , ItEquVarCreator & varsCreator
                                                , SPtr<BTreeNode> & dRoot
                                                );


//-------------------------------------------------------------------------------
/** \brief \ru Создать дерево выражения (b + c + d) через параметрический калькулятор.
           \en Create a tree of equations (b + c + d) by parametric calculator. \~
  \details \ru Создать дерево выражения (b + c + d) через параметрический калькулятор. \n
           \en Create a tree of equations (b + c + d) by parametric calculator. \n \~
  \return \ru Код результата операции.
          \en Operation result code. \~
  \ingroup Parser
*/
// ---
MATH_FUNC(EquTreeResCode) CreateBTreeForExpression( const TCHAR *                       equstr
                                                  , ItEquVarCreator *                   varsCreator
                                                  , SPtr<BTreeNode> &                   root
                                                  , std::unique_ptr<TreeIntervalNode> & iRoot
                                                  );


//-------------------------------------------------------------------------------
/** \brief \ru Создать переменную.
           \en Create variable. \~
  \details \ru Создать переменную по строке. \n
           \en Create variable by string. \n \~
  \return \ru Код результата операции.
          \en Operation result code. \~
  \ingroup Parser
*/
// ---
MATH_FUNC(EquTreeResCode) CreateVariable( const TCHAR * expression, ItEquVarCreator & );


//-------------------------------------------------------------------------------
/** \brief \ru Является ли выражение неравенством.
           \en Whether expression is inequation. \~
  \details \ru Является ли выражение неравенством. \n
           \en Whether expression is inequation. \n \~
  \ingroup Parser
*/
// ---
MATH_FUNC(bool) IsInequality( const TCHAR * );


#endif