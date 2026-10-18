//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file     
  \brief \ru Тестовый программный интерфейс геометрического решателя C3D Solver.
         \en Testing program interface of C3D Solver. \~

  \details \ru Данный файл содержит типы данных и вызовы, предназначенные для тестирования
            и отладки, поэтому могут быть изменены или удалены из API C3D Solver
            в будущих версиях. Для применения решателя двухмерных ограничений рекомендуется 
            использовать только интерфейс, объявленный в заголовочных файлах gce_api.h и gce_types.h.
           
           \en This file contains data types and calls for testing and debugging, so they 
           can be modified or removed from the C3D Solver API in future versions. To use 
           the 2D constraint solver, it is recommended to use only the interface declared 
           in the header files gce_api.h and gce_types.h. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GC_API_H
#define __GC_API_H

#include <math_version.h>
#include <gce_types.h>
#include <gce_equations.h>

template <class Type> class SArray;

//----------------------------------------------------------------------------------------
// \ru Дескрипция координаты геометрического примитива. \en Description of geometric primitive coordinate.
//---
template<class Geom = geom_item>
struct geom_coord
{
  Geom        geom;
  coord_name  crdName;

  geom_coord( Geom g, coord_name cName ) : geom(g), crdName(cName) {}
  geom_coord( const geom_coord & crd )
  {
    geom = crd.geom;
    crdName = crd.crdName;    
  }
  geom_coord & operator = ( const geom_coord & crd )
  {
    geom = crd.geom;
    crdName = crd.crdName;
    return *this;
  }

private:
  geom_coord();
};

/**
  \addtogroup Constraints2D_API   
  \{
*/

//----------------------------------------------------------------------------------------
/** \brief \ru Проверка: Останется ли система удовлетворенной, если изменить координаты точки.
           \en Check: Whether the system remains satisfied if the point coordinates are changed.
    \details
    \ru Функция работает корректно, если на момент её вызова система ограничений решена. При проверке 
  не происходит пробного перерешивания системы, а оценивается лишь удовлетворенность 
  смежных ограничений при новом параметре (px,py). Функция может быть применена для оценки области значений(окрестность 
  некоторой погрешности) точки, в которой система остается удовлетворенной.
    \en The function works correctly if the system is in resolved state, during the check 
  a test resolving of the system is not performed, only satisfaction of adjacent constraints 
  with a new parameter (px,py) is estimated.
    The function can be applied for estimation of domain (neighborhood of some tolerance) 
  of the point in which the system remains satisfied. \~
*/
//---
GCE_FUNC(bool) GCE_CheckPointSatisfaction( GCE_system gSys, geom_item pnt, point_type cp, double px, double py );

//----------------------------------------------------------------------------------------
/** \brief \ru Выдать состояние определенности системы ограничений.
           \en Get constraint system definition state.

    \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
    \details 
          \ru Функция вернет состояние #GCE_STATE_Underconstrained, если имеется хотя бы 
  один геометрический объект с ненулевой степенью свободы. 
  Состояние #GCE_STATE_WellConstrained означает, что геометрия полностью определена, а другое 
  состояние #GCE_STATE_UnresolvedRedundancy означает, что в модели имеются нерешенные
  избыточные ограничения.
          \en The function will return #GCE_STATE_Underconstrained if there is at least
  one geometric object with nonzero degree of freedom. State code #GCE_STATE_WellConstrained
  means that the geometry is fully-defined. And the other state is #GCE_STATE_UnresolvedRedundancy
  means there are unresolved redundant constraints (non solved overdefinition). \n

  \return \ru Одно из состояний, перечисленного набором: "Система недоопределена", 
              "Полностью определена" или "Есть не решенные избыточные ограничения".
          \en One of the states enumerated by a set: "Under-defined", "Well-defined" and 
              "There are unsolved redundant constraints (non solved overdefinition)". \~
  \note \ru Сложность выполнения функции сопоставима с GCE_Evaluate.
        \en Function complexity is comparable to GCE_Evaluate. 
*/
//---
GCE_FUNC(GCE_s_state) GCE_DefinitionState( GCE_system gSys );

//----------------------------------------------------------------------------------------
/// \ru Проверить, удовлетворена ли система ограничений. \en To check the satisfaction of constraints.
/**
  For internal use only!
  \param \ru gSys - Контекст решателя.
         \en gSys - Solver state. \~
  \param \ru c3dVer - Версия ядра c3d.
         \en c3dVer - Mathematical kernel version. \~
  \return \ru Код результата вычислений.
          \en Calculation result code. \~
*/
//---
GCE_FUNC(GCE_result) GCE_CheckSatisfaction( GCE_system gSys, VERSION c3dVer = GetCurrentMathFileVersion() );

//----------------------------------------------------------------------------------------
// The function for testing purposes.
/// \ru Выдать координаты переменных геометрической модели, значения которых не 
/// \ru зависят от изменения входных переменных in_coords.
/// \en Get coordinates of variables of geometric models which do not depend on 
/// \en changes of input variables in_coords.
// ---
GCE_FUNC(bool)  GCE_GetOutVarCoordinates( GCE_system gSys, const SArray<var_item> & inCoords,
                                          const SArray<constraint_item> & drvCons, SArray<var_item> & outCoords );

//----------------------------------------------------------------------------------------
/// \ru Задать ограничение "Радиальный размер" \en Specify "Radial dimension" constraint  
/**
  \param cir   - \ru окружность или дуга
                 \en a circle or an arc \~        
  \param diam  - \ru признак диаметрального размера
                 \en flag of diametral dimension \~
*/
//---
GCE_FUNC(constraint_item) GCE_FormCirDimension( GCE_system gcContext, geom_item cir, GCE_dim_pars dPars, bool diam );

//----------------------------------------------------------------------------------------
/// \ru Отменить режим драггинга \en Cancel dragging mode 
// ---
GCE_FUNC(void) GCE_ResetMovingMode( GCE_system );

//----------------------------------------------------------------------------------------
/**
  \details 
    \ru   По этому вызову солвер запоминает текущее состояние замороженных объектов,
      как обязательное к исполнению и перераспределяет их начальное приближение для 
      минимизации неудовлетворенных ограничений. После успешного вызова GCE_Evaluate
      замороженные объекты снова найдут свое обязательное положение. Незамороженные
      объекты подстроятся под замороженные в соотвествии с заданными ограничениями.

  \note 
    \ru Данный вызов обеспечивает поддержку старого поведения при переоценке координат замороженных объектов.
    \en The call is intended to replay an old-version evaluation for the start values of the frozen coordinates.
*/
//---
GCE_FUNC(bool) GCE_InitFrozenCoords( GCE_system gSys );

//----------------------------------------------------------------------------------------
/** \brief  \ru Собрать плохо-обусловленную часть системы ограничений. 
            \en Collect ill-conditioned part of the constraint system. \~
  \note \ru Данная функция может быть удалена из API в будущем. Для получения информации 
            о группе ограничений, образующих между собой избыточную или переопределенную
            часть ограничений, воспользуйтесь вызовом GCE_ConstraintStatus.
        \en The call may be removed from the API in the future. Use GCE_ConstraintStatus
            call to query which of constraints form a redundant or overdefined subsystem 
            with each other. \~

  \details 
  \ru Это вспомогательный вызов для диагностических целей. Функция собирает часть 
      ограничений, которые имеют один из следующих статусов:\n
        GCE_STATUS_IllConditioned,\n
        GCE_STATUS_WellConditioned,\n 
        GCE_STATUS_Redundant,\n
        GCE_STATUS_OverConstrained,\n
        GCE_STATUS_NotConsistent,\n
        GCE_STATUS_NotSolved.\n
   Такие ограничения обычно образуют избыточную или переопределенную подсистему.
   Для более детальной информации о статусе каждого из ограничений воспользуйтесь 
   вызовом GCE_ConstraintStatus.

  \en This is an auxiliary call for diagnostic purposes.The function collects  
  constraints that have one of the following statuses:\n
        GCE_STATUS_IllConditioned,\n
        GCE_STATUS_WellConditioned,\n 
        GCE_STATUS_Redundant,\n
        GCE_STATUS_OverConstrained,\n
        GCE_STATUS_NotConsistent,\n
        GCE_STATUS_NotSolved.\n
  These constraints usually form the redundant or overdifined part. For more detailed 
  information about the status of each constraint use the call GCE_ConstraintStatus.
*/
// ---
GCT_FUNC(bool) GCE_CollectLinearDependedConstrains( GCE_system, SArray<constraint_item> & );

/**
  \}
  Constraints2D_API
*/

/*
  Deprecated functions
*/

//----------------------------------------------------------------------------------------
// Deprecated call intended to maintaint previous versions.
//---
GCE_FUNC(constraint_item) GCE_FixCoordinate(GCE_system gSys, geom_item g, coord_name crd);

//----------------------------------------------------------------------------------------
// Deprecated call intended to maintaint previous versions.
// \ru Функция устарела. Вместо неё применять #GCE_FixCoordinate.
// \en The function is obsolete Use #GCE_FixCoordinate instead.
//---
GCE_FUNC(constraint_item) GCE_FormFixedCoordinate( GCE_system, geom_coord<> );

//----------------------------------------------------------------------------------------
// Deprecated value. It will be removed after 2023.
//---
const GCE_s_state GCE_STATE_OverConstrained = GCE_STATE_UnresolvedRedundancy; 

/*
  Deprecated typenames and constants (2019.06)
*/
typedef GCE_s_state GcConstraintStatus;

//----------------------------------------------------------------------------------------
/*
  Internal use only.
*/
//---
GCE_FUNC(GCE_system) GCE_RestoreFromJournal( const char * fName );

//----------------------------------------------------------------------------------------
/**
  \note Used only for testing.
*/
//---
GCE_FUNC(GCE_diagnostic_pars) GCE_DiagnosticPars( GCE_system gSys );

//----------------------------------------------------------------------------------------
/**
  \note Used only for testing.
*/
// ---
GCE_FUNC(size_t) GCT_InConstraintsFullCount( GCE_system gSys );

//----------------------------------------------------------------------------------------
// Measure a dimension value (it used for testing purposes only).
//---
GCE_FUNC(double) GCT_Measure( GCE_system gSys, constraint_type cType, geom_item g1, geom_item g2 );

#endif

// eof
