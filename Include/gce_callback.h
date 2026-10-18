//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file     
  \brief \ru Функции и типы данных для обратных вызовов двухмерного геометрического решателя.
         \en Functions and data types for callbacks of the 2D-solver. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GCE_CALLBACK_H
#define  __GCE_CALLBACK_H

#include <gce_types.h>

/** 
  \addtogroup Constraints2D_API   
  \{
*/

/*
   Application data types for callback queries
*/
typedef   void* GCE_app_geom;   ///< Geometric object of the application using the solver
const GCE_app_geom GCE_NOGEOM = 0;  ///< \en Specifies an undefined object of the user's app. \ru Означает неопределенный объект пользовательского приложения.

/*
  Callback enquiries
*/
using GCE_geom_registered   = void (*)( GCE_app_geom ag); ///< Application geom was registered in the solver.
using GCE_geom_unregistered = void (*)( GCE_app_geom ag);
using GCE_allow_zero_radius = bool (*)( GCE_app_geom ag); ///< \ru Разрешает окружность нулевого радиуса. \en Allow zero radius of a circle.
using GCE_allow_zero_length = bool (*)( GCE_app_geom ag); ///< \ru Разрешает участок кривой или отрезок нулевой длины. \en Allow zero length of a bounded curve or line segments.
using GCE_get_lseg_direction = GCE_vec2d (*)(GCE_app_geom ag); ///< \ru Функция, которая выдает направляющий вектор отрезка. \en Function that returns the direction of a line segment.
using GCE_abort = bool (*)(); ///< Query to interrupt calculations

//----------------------------------------------------------------------------------------
/** \brief  \ru Структура, объединяющая обратные вызовы двухмерного решателя.
            \en The structure uniting 2D-solver callbacks.
    \details  \ru Таблица функций, определяемых на стороне пользовательского приложения 
                  для "тонкой настройки" решателя.
              \en Table of user-defined callbacks tuning the 2D-solver. \~
*/
//---
struct GCE_CLASS GCE_callback_table
{
  /*
    General system callbacks;
  */
  GCE_geom_registered   gRegister;
  GCE_geom_unregistered gUnregister;  
  GCE_abort             abortFunc;

  /* 
    Geometry properties 
  */
  GCE_allow_zero_radius allowZeroRadius;   ///< Permit circle to have zero radius.
  GCE_allow_zero_length allowZeroLength;   ///< Permit curve to have zero length.
  GCE_get_lseg_direction getLSegDirection; ///< Get direction of line segment
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Установить и вернуть структуру с функциями обратного вызова по умолчанию.
            \en Set and return default callback functions.  \~
    \details \ru GCE_callback_table - простая структура в стиле C, не имеющая конструктора.
                 Функция GCE_InitCallbacks позволяет придать структуре начальное значение
                 что бы избежать некорректных значений в памяти.
             \en GCE_callback_table is a plain old data structure with no constructor.  
                 The function is able to set an initial value of the structure to avoid
                 incorrect work with memory.
*/
//---
GCE_FUNC(GCE_callback_table&) GCE_InitCallbacks( GCE_callback_table & );

//----------------------------------------------------------------------------------------
/** \brief  \ru Зарегистрировать таблицу обратных вызовов для новой системы ограничений.
            \en Register callback table for new constraint system.  \~
  \param[in] gSys - \ru Система ограничений.
                    \en Constraint system. \~
  \param[in] cbTable - \ru Таблица обратных вызовов.
                       \en Table of callbacks. \~
  \return \ru Вернет GCE_RESULT_Ok, если регистрация выполнена.
          \en Returns GCE_RESULT_Ok if the registration fulfilled. \~

*/
//---
GCE_FUNC(GCE_result) GCE_Register( GCE_system gSys, const GCE_callback_table & cbTable );

//----------------------------------------------------------------------------------------
/// Associate an application geometry and a solver's descriptor.
//---
GCE_FUNC(void) GCE_Bind( GCE_system, geom_item, GCE_app_geom );

//----------------------------------------------------------------------------------------
/// Disassociate an application geometry and a solver's descriptor.
//---
GCE_FUNC(void) GCE_Unbind( GCE_system, geom_item );

/**
  \}
  Constraints2D_API
*/

#endif // __GCE_CALLBACK_H

// eof