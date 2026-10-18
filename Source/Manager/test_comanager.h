////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Модуль: COMANAGER
         \en Module: COMANAGER. \~
  \details \ru Цель: Менеджер геометрических ограничений для MbModel
           \en Target: Geometric constraints manager for MbModel  \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEST_COMANAGER_H
#define __TEST_COMANAGER_H
//
#include <templ_sptr.h>
#include <reference_item.h>
// constraints
#include <gcm_api.h>
#include <assembly.h>



/*
class GcFormerImpl;
class MbConstraint;
*/
class MbAssembly;

//////////////////////////////////////////////////////////////////////////////////////////
//
/** \brief \ru Менеджер, обслуживающий систему ограничений сборки
           \en Manager of constraint system of assembly unit
*/
//////////////////////////////////////////////////////////////////////////////////////////

class ConstraintManager
{
public:
  ConstraintManager();
 ~ConstraintManager();

  /// \ru Добавить ограничение в решатель \en Add a constraint to the solver 
  // Задать сопряжение для пары объектов
  bool            AddConstraint( MtMateType, const MtGeomArgument &, const MtGeomArgument &, MtParVariant );      
  /// \ru Рассчитать систему ограничений \en Compute a system of constraints 
  MtResultCode3D  Evaluate();
  /// \ru Применить решение \en Apply the solution 
  void            Apply();
  /// \ru Очистить весь контекст решателя \en Clear the whole context of the solver 
  void            Clear();

private:
  ConstraintManager( const ConstraintManager & );
  ConstraintManager & operator = ( const ConstraintManager & );
};

#endif // __TEST_COMANAGER_H

// eof
