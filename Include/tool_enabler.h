////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Включатель модулей ядра.
         \en Kernel modules enabler \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef _TOOL_ENABLER_H_
#define _TOOL_ENABLER_H_

#include <math_define.h>


//------------------------------------------------------------------------------
/** \brief \ru Активировать модули ядра.
           \en Enable kernel modules. \~ 
  \details \ru Активировать соответствующие модули ядра.
           \en Enable the corresponding kernel modules. \~
  \ingroup Base_Tools
*/
// ---
extern "C" MATH_FUNC (void) EnableMathModules( const char * name, int nameLength, const char * key, int keyLength );


//------------------------------------------------------------------------------
/** \brief \ru Деактивировать все модули ядра.
           \en Disable all kernel modules. \~ 
  \details \ru Деактивировать все модули ядра.
           \en Disable all kernel modules. \~
  \ingroup Base_Tools
*/
// ---
extern "C" MATH_FUNC (void) DisableMathModules();


//------------------------------------------------------------------------------
/** \brief \ru Проверить ключ активации на валидность.
           \en Verify key. \~ 
  \details \ru Проверить ключ активации на валидность.
           \en Verify key. \~
  \ingroup Base_Tools
*/
// ---
extern "C" MATH_FUNC (bool) VerifyLicenseKey( const char * name, const char * key, const char * pub_key = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Проверить контроллер защиты моделировщика.
           \en Check the controller of the Modeler. \~ 
  \details \ru Проверить контроллер защиты моделировщика.
           \en Check the controller of the Modeler. \~ 
  \ingroup Base_Tools
*/
// ---
extern "C" MATH_FUNC (bool) IsMathModelerEnable();


//------------------------------------------------------------------------------
/** \brief \ru Проверить контроллер защиты конвертеров.
           \en Check the controller of the Converter. \~ 
  \details \ru Проверить контроллер защиты конвертеров.
           \en Check the controller of the Converter. \~ 
  \ingroup Base_Tools
*/
// ---
extern "C" MATH_FUNC (bool) IsMathConverterEnable();


//------------------------------------------------------------------------------
/** \brief \ru Проверить контроллер защиты решателя.
           \en Check the controller of the Solver. \~ 
  \details \ru Проверить контроллер защиты решателя.
           \en Check the controller of the Solver. \~ 
  \ingroup Base_Tools
*/
// ---
extern "C" MATH_FUNC (bool) IsMathSolverEnable();


//------------------------------------------------------------------------------
/** \brief \ru Проверить контроллер защиты визуализатора.
           \en Check the controller of the Vision. \~ 
  \details \ru Проверить контроллер защиты визуализатора.
           \en Check the controller of the Vision. \~ 
  \ingroup Base_Tools
*/
// ---
extern "C" MATH_FUNC (bool) IsMathVisionEnable();


//------------------------------------------------------------------------------
/** \brief \ru Проверить контроллер защиты преобразователя сеток в BRep.
           \en Check the controller of the BShaper. \~ 
  \details \ru Проверить контроллер защиты преобразователя сеток в BRep.
           \en Check the controller of the BShaper. \~ 
  \ingroup Base_Tools
*/
// ---
extern "C" MATH_FUNC (bool) IsMathBShaperEnable();


//------------------------------------------------------------------------------
/** \brief \ru Проверить контроллер защиты полигонального ядра.
           \en Check the controller of the PolyShaper. \~
  \details \ru Проверить контроллер защиты полигонального ядра.
           \en Check the controller of the PolyShaper. \~
  \ingroup Base_Tools
*/
// ---
extern "C" MATH_FUNC( bool ) IsMathPolyShaperEnable();


//------------------------------------------------------------------------------
/** \brief \ru Проверить контроллер защиты детектора столкновений.
           \en Check the controller of the Collision Detection. \~
  \details \ru Проверить контроллер защиты детектора столкновений.
           \en Check the controller of the Collision Detection. \~
  \ingroup Base_Tools
*/
// ---
extern "C" MATH_FUNC (bool) IsMathCollisionEnable();


//------------------------------------------------------------------------------
/** \brief \ru Проверить контроллер защиты FairCurve моделировщика.
           \en Check the controller of the FairCurve modeler. \~
  \details \ru Проверить контроллер защиты FairCurve моделировщика.
           \en Check the controller of the FairCurve modeler. \~
  \ingroup Base_Tools
*/
// ---
extern "C" MATH_FUNC (bool) IsMathFairCurveEnable();


//------------------------------------------------------------------------------
/** \brief \ru Проверить контроллер защиты модуля проецирования.
           \en Check the controller of the Mapping. \~
  \details \ru Проверить контроллер защиты модуля проецирования.
           \en Check the controller of the Mapping. \~
  \ingroup Base_Tools
*/
// ---
extern "C" MATH_FUNC (bool) IsMathMappingEnable();


//------------------------------------------------------------------------------
/** \brief \ru Отпустить контролера работы модулей ядра.
           \en Free the controller of the kernel modules work. \~ 
  \details \ru Отпустить контролера работы модулей ядра.
           \en Free the controller of the kernel modules work. \~
  \deprecated \ru Функция устарела и будет удалена. Для отключения модулей ядра использовать #DisableMathModules.
              \en The function is deprecated and will be removed. To disable the kernel modules use #DisableMathModules. \~
  \ingroup Base_Tools
*/
// ---
extern "C" MATH_FUNC (void) FreeMathModulesChecker();


#endif // _TOOL_ENABLER_H_
