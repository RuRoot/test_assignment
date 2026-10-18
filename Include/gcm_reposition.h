//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Абстракция для управления положением моделей в сборке.
         \en Abstract for control of model position in the assembly. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GCM_REPOSITION_H
#define __GCM_REPOSITION_H

#include <math_define.h>
#include <gcm_types.h>

//----------------------------------------------------------------------------------------
// Deprecated typename
//---
typedef GCM_dependency MtJointStatus;

//----------------------------------------------------------------------------------------
//
// ---
inline GCM_FUNC(MtJointStatus) InversionOf( MtJointStatus jointDir )
{
  if ( jointDir == GCM_2ND_DEPENDENT )
    return GCM_1ST_DEPENDENT;
  if ( jointDir == GCM_1ST_DEPENDENT )
    return GCM_2ND_DEPENDENT;
  return jointDir;
}


struct ItGeom;

/**
  \addtogroup GCM_3D_ObjectAPI
  \{
*/

//----------------------------------------------------------------------------------------
/**
  \brief \ru Абстракция для управления положением геометрических объектов в сборке. 
         \en Abstract for control of geom position in the assembly \~
  \details
      \ru Объект ItPositionManager это класс, с помощью которого менеджер сопряжений
  MtGeomSolver возвращает свое решение, т.е. положение тел ItGeom (см.функцию
  ItPositionManager::Reposition( ItGeom & geom, const MbPlacement3D & pos )).
  Экземпляр решателя MtGeomSolver располагает только одним объектом ItPositionManager.
  Фактически в интерфейсе ItPositionManager будем собирать виртуальные функции для 
  организации обратной связи с клиентским приложением.
      
      \en ItPositionManager object is the class with which the mates manager
  MtGeomSolver returns its decision, i.e. position of solids ItGeom (see function
  ItPositionManager::Reposition( ItGeom & geom, const MbPlacement3D & pos )).
  Instance of the solver MtGeomSolver has only one object ItPositionManager.
  Actually in the interface of #ItPositionManager we will collect virtual functions
  for organizing feedback of the solver client. \~
  
      \par  
      \ru Объект, возвращаемый функцией ItGeom & ItPositionManager::GetGround() const
  обычно соответствует сборке, система сопряжений которой обслуживается.
      \en Object returned by function ItGeom & ItPositionManager::GetGround() const
  usually corresponds to the assembly of which the mates system is solved.    
*/
//---
struct GCM_CLASS ItPositionManager
{
  /// \ru Установить новое положение объекта. \en Set new position of the object.
  virtual void            Reposition( ItGeom & geom, const MbPlacement3D & pos ) = 0;
  /** \brief  \ru Выдать "геометрическую землю", имеющую систему координат всегда совпадающую с мировой.
              \en Get "geometric ground" having a coordinate system which always coincident with World Coord System.
      \details  \ru Главное свойство "Земли" это нулевая степень свободы, т.е. неподвижность в контексте всей системы ограничений.
                \en The main property of "Ground" is zero degree of freedom, i.e. absolute fixity in context of the constraint system.
  */
  virtual ItGeom &        GetGround() const = 0;
  /// \ru Выдать характер связи для пары сопрягаемых тел (направленность соединения) \en Get the link character for pair of mating solids (direct connection) 
  virtual GCM_dependency  GetJointStatus( const ItGeom & geomOne, const ItGeom & geomTwo ) const = 0;  
  /// \ru Добавить ссылку \en Add a reference 
  virtual refcount_t      AddRef() const = 0;
  /// \ru Удалить ссылку \en Release reference 
  virtual refcount_t      Release() const = 0;

protected:
  ~ItPositionManager() {}
};

/** \} */ // GCM_3D_ObjectAPI

#endif

// eof