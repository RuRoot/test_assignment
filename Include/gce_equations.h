//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file     
  \brief \ru Нелинейные уравнения и неравенства, определяемые на стороне Приложения.
         \en Nonlinear equations and inequations defined by the application. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GCE_EQUATIONS_H
#define __GCE_EQUATIONS_H

#include <mt_ref_item.h>
#include "gce_geom.h"

class MtVectorN;
template <class Type> class RPArray;

//----------------------------------------------------------------------------------------
/** \brief \ru Интерфейс численного уравнения, выраженного через набор координат.
           \en Interface of the numeric equation expressed via a set of coordinates.
  \details \ru Как правило, это алгебраические уравнение общего вида f(x1,x2,..,xn) = g(x1,x2,..,xn)
               явно-выраженной форме: x1 = g(x2,x3,..,xn).
           \en As a rule, it is an algebraic equation of a general form  f(x1,x2,..,xn) = g(x1,x2,..,xn)
               or, as a special case, it is an equation in explicit form: x1 = g(x2,x3,..,xn). \~
*/
//---
struct GCE_CLASS ItNumericEquation
{  
  enum eval_result_code: char
  { 
    EVAL_RESULT_Undefined = 0,
    EVAL_RESULT_Ok, 
    EVAL_RESULT_OutOfDomaint 
  };
  struct eval_result
  { 
    eval_result_code  resCode = EVAL_RESULT_Undefined;
    double funDer = MB_MAXDOUBLE; // Derivative value.
    double funVal = MB_MAXDOUBLE; // Function value.
  };
  /// \ru Выдать координату с индексом crdIdx. \en Get coordinate with crdIdx index.
  virtual ItCoord*  Coord( size_t crdIdx ) const = 0;
  /// \ru Количество координат, связанных с уравнением. \en Count of coordinates connected with the equation.
  virtual size_t    NumCoords() const = 0;
  /// \ru Вычисление первой производной по координате и значений функции. \en The first derivative by coordinate and the function values calculation.
  virtual eval_result Evaluate(const ItCoord* crd, const std::vector<double>& crdVals) const;
  /// \ru Выдать координату зависимой переменной (для уравнений заданных в явно-выраженной форме). \en Get the coordinate of dependent variable (for explicit equations).
  virtual const ItCoord*  DependedCoord() const = 0;

  /** \brief \ru Признак уравнения, заданного в форме присвоения (начиная с Компас V12).
             \en Flag of equation specified in form of assignment. 
    \details
    \ru Уравнения, заданные в явно выраженной форме, считающиеся присвоением выражения зависимой переменной: x1 = g(x2,x3,..,xn).
        Такие уравнения стремимся вычислять иерархическим способом, сверху-вниз.
    \en Equations specified explicitly, considered to be the assignment of dependent variable: x1 = g(x2,x3,..,xn).
        It is preferred to compute such equations by hierarchical top-down method. \~
  */
  virtual bool IsExplicit() const = 0;

public:  
  virtual refcount_t  AddRef() const = 0;
  virtual refcount_t  Release() const = 0;

private:
  // It will be removed
  virtual bool CalcDerive( ItGeomCoord &, const MtVectorN &, double &, double & ) const { return false; }
  // It will be removed.
  virtual ItGeomCoord * GetCoord( ptrdiff_t ) const { return nullptr; }
  // It will be removed.
  virtual bool  CalcDerive( ItGeomCoord &, const std::vector<double> & /*argLine*/, double & /*fd*/, double & /*f*/ ) const { return false; }
  // It will be removed.
  virtual ptrdiff_t GetCoordCount() const { return 0; }
  // It will be removed.
  virtual ptrdiff_t GetDependedCoordIdx() const { return -1; }

protected:
  ~ItNumericEquation() {}
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Вычисление первой производной по координате и значений функции.
            \en The first derivative by coordinate and the function values calculation.
*/
//---
inline ItNumericEquation::eval_result ItNumericEquation::Evaluate(const ItCoord* crd, const std::vector<double>& crdVals) const
{
  eval_result res;
  ItGeomCoord * gCrd = dynamic_cast<ItGeomCoord*>(const_cast<ItCoord*>(crd));
  if ( gCrd!=nullptr && CalcDerive(*gCrd, crdVals, res.funDer, res.funVal))
  {
    res.resCode = EVAL_RESULT_Ok;
  }
  return res;
}


using ItAlgebraicConstraint = ItNumericEquation;

/** 
  \addtogroup Constraints2D_API   
  \{
*/

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение, реализуемое на стороне клиента.
           \en Specify a constraint implemented by the user. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] iEqu - \ru Интерфейс уравнения, заданного пользователем.
                    \en Interface of the equation specified by the user. \~
  \param[in] varsCount - \ru Количество переменных.
                         \en Count of variables. \~
  \param[in] varsVector - \ru Вектор переменных.
                          \en Vector of variables. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
*/
// ---
GCE_FUNC(constraint_item) GCE_AddEquation(  GCE_system gSys
                                          , ItAlgebraicConstraint & iEqu
                                          , size_t    varsCount
                                          , const var_item * varsVector );

/**
  \}
  Constraints2D_API
*/

#endif // __GCE_EQUATIONS_H

// eof