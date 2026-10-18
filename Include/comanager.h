//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Модуль: COMANAGER
         \en Module: COMANAGER. \~
  \details \ru Цель: Менеджер геометрических ограничений для MbModel
           \en Target: Geometric constraints manager for MbModel  \~

*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __COMANAGER_H
#define __COMANAGER_H
//
#include <templ_sptr.h>
#include <reference_item.h>
// constraints
#include "gce_api.h"



class GcFormerImpl;
class MbConstraint;


//////////////////////////////////////////////////////////////////////////////////////////
//
/// \ru Менеджер для взаимодействия с решателем \en Manager of interactions with the solver 
//
//////////////////////////////////////////////////////////////////////////////////////////

class MATH_CLASS ConstraintManager2D
{
  GCE_system      m_gcSolver;
  GcFormerImpl &  m_gcFormer;
  
public:
  ConstraintManager2D();
 ~ConstraintManager2D();

public:
  /// \ru Добавить ограничение в решатель \en Add a constraint to the solver 
  bool  AddConstraint( const MbConstraint & );
  /// \ru Рассчитать систему ограничений \en Compute a system of constraints 
  bool  Evaluate();
  /// \ru Применить решение \en Apply the solution 
  void  ApplySolution();
  /// \ru Очистить весь контекст решателя \en Clear the whole context of the solver 
  void  Clear();

private:
  ConstraintManager2D( const ConstraintManager2D & );
  ConstraintManager2D & operator = ( const ConstraintManager2D & );
};

#endif // __COMANAGER_H


// eof