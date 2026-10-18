////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Определение отображения резьбы.
         \en The thread mapping definition. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MAP_THREAD_H
#define __MAP_THREAD_H


#include <map_lump.h>
#include <mb_thread.h>


//------------------------------------------------------------------------------
/** \brief \ru Структура для строителя отображения резьбы.
           \en A structure for the thread mapping creator. \~
  \details \ru Структура для строителя отображения резьбы.
           \en A structure for the thread mapping creator. \~
  \ingroup Mapping
*/
// --- 
struct ThreadMapperStruct {     
public:
  const MbThread      & thread;     ///< \ru Резьба в мировой системе координат. \en A thread in the world coordinate system. 
  const MbSolid       & solid;      ///< \ru Тело для резьбы (в системе координат тела). \en A solid for thread (in a solid coordinate system). 
  const MbMatrix3D    & matrFrom;   ///< \ru Матрица преобразования из системы координат тела в систему координат мира. \en A matrix of transformation from a solid coordinate system to the world coordinate system. 
        MbPlacement3D   placeView;  ///< \ru Система координат вида в мировой системе координат. \en A view coordinate system in the world coordinate system. 
        MbeThrMapType   thrMapType; ///< \ru Тип отображения резьбы. \en A type of thread mapping. 
  const VERSION         version;    ///< \ru Версия построения (математическая). \en A version of construction (mathematical). 

public:     

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.\n
             \en Constructor.\n \~   
    \param[in] thr   - \ru Резьба
                       \en A thread \~
    \param[in] sol   - \ru Тело.
                       \en A solid. \~
    \param[in] mFrom - \ru Матрица преобразования тела в мировую систему координат.
                       \en A matrix of solid transformation to the world coordinate system. \~
  */
  ThreadMapperStruct( const MbThread & thr, const MbSolid & sol, const MbMatrix3D & mFrom,
                      const VERSION ver = Math::DefaultMathVersion())
    : thread    ( thr              )
    , solid     ( sol              )
    , matrFrom  ( mFrom            )
    , placeView (                  )
    , thrMapType( tmt_CompleteView )
    , version   ( ver              )
  {}  

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.\n
             \en Constructor.\n \~   
    \param[in] thr    - \ru Резьба
                        \en A thread \~
    \param[in] lump   - \ru Тело с матрицей преобразования.
                        \en A solid with a matrix of transformation. \~
    \param[in] plView - \ru Система координат вида.
                        \en A view coordinate system. \~
    \param[in] tmType - \ru Тип отображения резьбы.
                        \en A type of thread mapping. \~
  */
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.\n
             \en Constructor.\n \~   
    \param[in] thr    - \ru Резьба
                        \en A thread \~
    \param[in] sol    - \ru Тело.
                        \en A solid. \~
    \param[in] mFrom  - \ru Матрица преобразования тела в мировую систему координат.
                        \en A matrix of solid transformation to the world coordinate system. \~
    \param[in] plView - \ru Система координат вида.
                        \en A view coordinate system. \~
    \param[in] tmType - \ru Тип отображения резьбы.
                        \en A type of thread mapping. \~
  */
  ThreadMapperStruct( const MbThread & thr, const MbSolid & sol, const MbMatrix3D & mFrom,
                      const MbPlacement3D & plView, MbeThrMapType tmType,
                      const VERSION ver = Math::DefaultMathVersion() )
    : thread    ( thr    )
    , solid     ( sol    )
    , matrFrom  ( mFrom  )
    , placeView ( plView )
    , thrMapType( tmType )
    , version   ( ver    )
  {}

  // \ru Объявление конструктора копирования и оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration without implementation of the copy-constructor and assignment operator to prevent an assignment by default. 
  OBVIOUS_PRIVATE_COPY( ThreadMapperStruct );
};


//------------------------------------------------------------------------------
/** \brief \ru Вычислить линии отображения резьбы.
           \en Calculate thread mapping lines. \~
  \details \ru Вычислить линии отображения резьбы тела (с интерфейсом).\n    
    \n
    Условия получения набора кривых для отображения:\n
    1. Ось резьбы должна быть перпендикулярна или колинеарна оси Z вида.\n
    2. Массивы кривых в _symbView должны быть пустыми.
           \en Calculate solid thread mapping lines (with interface).\n    
    \n
    Conditions for obtaining a set of curves for mapping:\n
    1. A thread axis must be perpendicular or colinear to the Z-axis of a view.\n
    2. Arrays of curves in the _symbView must be empty. \~  
  \param[in]  _thrStruct - \ru Данные для построения.
                           \en Data for construction. \~
  \param[out] _symbView  - \ru Указатель на интерфейс вида.\n
                           Содержит набор кривых отображения резьбы в системе координат тела, 
                           видимость которых определяется при проецированнии.
                           \en A pointer to a view interface.\n
                           Contains a set of curves of thread mapping in a solid coordinate system 
                           with visibility defining in projection. \~
  \return \ru true в случае успеха операции.
          \en returns true if the operation succeeded. \~
  \ingroup Mapping
*/
//---
MATH_FUNC (bool) PerformThreadMapping( const ThreadMapperStruct & _thrStruct,
                                             MbSimbolthThreadView & _symbView );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить линии отображения резьбы.
           \en Calculate thread mapping lines. \~
  \details \ru Вычислить линии отображения резьбы тела (без интерфейса).\n
           \en Calculate solid thread mapping lines (without interface).\n \~
  \param[in]  _thrStruct - \ru Данные для построения.\n
                           \en Data for construction.\n \~
  \param[out] annCurves  - \ru Множество аннотационных кривых.\n
                           \en The array of annotative curves.\n \~
  \return \ru true в случае успеха операции.
          \en returns true if the operation succeeded. \~
  \ingroup Mapping
*/
//---
MATH_FUNC ( bool ) PerformThreadMapping( const ThreadMapperStruct & _thrStruct, MbAnnCurves & annCurves );


#endif // __MAP_THREAD_H