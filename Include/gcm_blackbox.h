//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file     
  \brief  Абстрактный интерфейс для чёрного ящика.
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GCM_BLACKBOX_H
#define __GCM_BLACKBOX_H

#include <mb_placement3d.h>
#include <templ_ifc_array.h>
#include <templ_s_array.h>
#include <gcm_geom.h>
#include <gcm_manager.h>


//----------------------------------------------------------------------------------------
/** \brief  \ru Чёрный ящик.
            \en Blackbox. \~
  \details  
    \ru Черный ящик реализует закон позиционирования геометрических объектов, зависящих 
  от позиции других объектов. Интерфейс #ItGCBlackbox реализуется обычно на стороне 
  клиентского приложения и передается геометрическому решателю для исполнения через метод 
  #MtGeomSolver::AddBlackbox. Например, в сапрах абстракция черного ящика применяется для
  моделирования массивов тел (паттерны), которые родились путем тиражирования экземпляров 
  некоторой детали некоторому геометрическому закону паттерна. 
    Черный ящик может применяться не только для организации паттернов, но и для задания на 
  стороне клиентского приложения различных однонаправленных зависимостей,  с определенным 
  законом позиционирования тел. Объекты, которые рассматриваются, как входящие для черного 
  ящика, называются независимыми. Объекты, которые рассматриваются, как исходящие для 
  черного ящика, называются зависимыми.
            \en Blackbox implements a law of positioning of geometric objects which are 
  dependent on positions of other objects. Interface #ItGCBlackbox is usually implemented 
  on the side of application and it is transferred from the application to C3D Solver 
  by method #MtGeomSolver::AddBlackbox. The blackbox abstraction can be used to organize
  patterns in assembly structures, when the elements of the pattern are copies (instancies) 
  of the same part (sample) replicated according to a geometric law specified by the application. \~
    \ingroup  GCM_3D_ObjectAPI
*/
//---
struct GCM_CLASS ItGCBlackbox
{
  using GeomPlace = std::pair<const ItGeom*, MbPlacement3D>;  

  /// \ru Выдать независимые геометрические объекты. \en The function collects in the array independent geoms of a blackbox.
  virtual void    CollectMyInGeoms( IFC_Array<ItGeom> & ) const = 0;
  /// \ru Выдать зависимые геометрические объекты. \en The function collects in the array dependent geoms of a blackbox.
  virtual void    CollectMyOutGeoms( IFC_Array<ItGeom> & ) const = 0;

  /** \brief \ru Рассчитать положение зависимого объекта.
             \en Calculate position of a dependent geometric object. \~
    \param[in]  inGeomPlaces  - \ru Позиции независимых объектов, получаемых методом #ItGCBlackbox::CollectMyInGeoms.
                                \en Positions of independed geoms, which are got by #ItGCBlackbox::CollectMyInGeoms.\~
    \param[in]  depPlace   - \ru Зависимый геометрический объект.
                             \en Depended geometric object.\~
    \param[out] depPlace  - \ru Вычисленное положение для объекта outGeom.
                            \en Calculated position for a dependent geom 'outGeom'\~
    \return                 \ru GCM_DEP_RESULT_Ok, если функция корректно исполнена, иначе диагностический код ошибки.    
                            \en GCM_DEP_RESULT_Ok if the function performed succeeded otherwise one of diagnostic error codes. \~
  */
  virtual GCM_dependent_result CalculateDependent( const std::vector<GeomPlace> & inGeomPlaces
                                                 , GeomPlace & depPlace ) const
  {
    /*
      This code will be removed tougether deprecated call ItGCBlackbox::Calculate.
      Данный код будет удален после того, как выйдет из применения устаревший вызов ItGCBlackbox::Calculate.
    */
    SArray<MbPlacement3D> inPlaces( inGeomPlaces.size() );
    for ( const GeomPlace & inPlace : inGeomPlaces )
    {
      inPlaces.push_back( inPlace.second );
    }
    if ( depPlace.first != nullptr )
      return Calculate( inPlaces, *depPlace.first, depPlace.second ) ? GCM_DEP_RESULT_Ok : GCM_DEP_RESULT_None;
    return GCM_DEP_RESULT_InternalError;
  }

  /// \ru Является ли данный объект зависимым для черного ящика? \en Check if the given geometric item is dependent
  virtual bool    IsMyOutGeom( const ItGeom & ) const = 0;

  /** 
    \brief \ru Сформулировать ограничения для зависимого геометрического объекта. 
           \en Formulate constraints for the dependent geometric object. \~
    \details \ru Функция позволяет задать положение зависимого объекта относительно управляющих в явном виде с помощью 
                 интерфейса #MtGeomSolver (#MtGeomSolver::AddConstraint, #MtGeomSolver::AddConstraintItem, 
                 #MtGeomSolver::AddPattren). Данный механизм является альтернативой вызову #ItGCBlackbox::Calculate.
                 Его использование сообщает решателю не только о наличии зависимости (как в случае с 
                 #ItGCBlackbox::Calculate), но и о ее характере. Данное знание позволяет расширить класс разрешимых 
                 задач, но в некоторых случаях может привести к ухудшению производительности.
             \en The function allows to set the position of the dependent object relative to it's governing objects 
                 explicitly using the interface #MtGeomSolver (#MtGeomSolver::AddConstraint, 
                 #MtGeomSolver::AddConstraintItem, #MtGeomSolver::AddPattren). This mechanism is an alternative to calling 
                 #ItGCBlackbox::Calculate. Using this mechanism provides an information about the dependency character.
                 This information allows to extend the class of solvable problems but in some cases can lead to 
                 performance degradation.
    \param[in, out] solver  - \ru Система ограничений. 
                              \en System of constraints. \~
    \param[in]      outGeom - \ru Зависимый геометрический объект. 
                              \en Depended geometric object.\~
    \return \ru Должна возвращать true, если положение зависимого объекта было задано в явном виде через задание в 
                решателе нужных для этого ограничений; если же положение зависимого объекта должно вычисляться с помощью 
                метода #ItGCBlackbox::Calculate, функция должна возвращать false.
            \en The function should return true, if the position of the dependent object was formulated in the 
                constraints solver explicitly using #MtGeomSolver::AddConstraint, #MtGeomSolver::AddConstraintItem or 
                #MtGeomSolver::AddPattren methods.
                If the position of the dependent object must be calculated using the method #ItGCBlackbox::Calculate, 
                the function should return false.
  */
  virtual bool    FormulateOutGeom( MtGeomSolver & solver, ItGeomPtr outGeom );

  /**
    \brief \ru Завершить работу с черным ящиком. \en To finish work with the black box. \~
    \details \ru Функция предоставляет возможность пользователю данного интерфейса корректно завершить работу с черным 
                 ящиком в тот момент, когда он удаляется в решателе.
             \en The function allows the user of this interface correctly to complete work with the black box when 
                 it is removing from the solver. \~
  */ 
  virtual void    FinishBlackBox() {}

  /** \brief \ru Рассчитать положение зависимого объекта.
             \en Calculate position of a dependent geometric object. \~
      \attention 
      \ru Данная функция выбывает из API C3D Solver, ее поддержка будет прекращена. 
          Поэтому приложению заказчика следует перегружать новую виртуальную функцию CalculateDependent вместо старой.
      \en This function is deprecated and supporting it will be stopped. 
          So the customer application should be override new virtual function CalculateDependent instead.
  */
  virtual bool  Calculate( const SArray<MbPlacement3D> &, const ItGeom &, MbPlacement3D &) const { return false; }

public:
  virtual refcount_t AddRef() const = 0;
  virtual refcount_t Release() const = 0;
};

//----------------------------------------------------------------------------------------
// \ru Сформулировать ограничения для зависимого геометрического объекта. \en Formulate constraints for the dependent geometric object. \~
// ---
inline bool ItGCBlackbox::FormulateOutGeom( MtGeomSolver & /*solver*/, ItGeomPtr /*outGeom*/ )
{
  return false;
}

#endif // __GCM_BLACKBOX_H

// eof
