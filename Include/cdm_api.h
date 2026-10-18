//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Программный интерфейс модуля обнаружения столкновений.
         \en Program interface of collision detection module. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CDM_API_H
#define __CDM_API_H

#include <cdm_types.h>
#include <solid.h>
#include <conv_res_type.h>

class MtRefItem;

/**
  \addtogroup Collision_Detection
  \{
*/

/*
  Creation and removing a scene of collision detection.
*/

//----------------------------------------------------------------------------------------
/**
  \brief  \ru Создать пустую сцену детектора столкновений.
          \en Create an empty collision detection scene. \~
  \return \ru Дескриптор сцены детектора столкновений и запросов на близость тел.
          \en Descriptor of collision detection scene and proximity queries. \~
*/
//---
MATH_FUNC(cdet_scene) CDM_CreateScene();

//----------------------------------------------------------------------------------------
/** 
  \brief  \ru Удалить сцену детектора столкновений.
          \en Delete collision detection scene. \~
  \details  \ru Данный метод освобождает память от внутренних структур данных, обслуживающих
                детектор столкновений. Удаляемая сцена становится недействительной после
                данного вызова.
            \en This method releases memory from internal data structures maintaining
                the collision detector. The removed collision scene is invalidated after this call. \~
  \param[in] scn - \ru Сцена запросов на столкновения и близость тел.
                   \en Scene of the collision and proximity queries. \~
*/
//--- 
MATH_FUNC(void) CDM_RemoveScene( cdet_scene scn );

//---------------------------------------------------------------------------------------
/**
  \brief  \ru Добавить новый компонент в сцену детектора столкновений и задать параметры близости.
          \en Add a new component to the collision detector scene and set proximity parameters. \~
  \param[in] comp - \ru Компонент, являющийся геометрическим объединением тел или
                        полигональных объектов, наделенный собственной локальной системой координат.
                    \en A component that is a geometric union of bodies or polygonal objects. It have
                        its own local coordinate system. \~
  \return \ru Новый компонет, зарегистрированный в сцене детектора столкновений.
          \en New component registered in the collision detector scene. \~
  \details \ru Компонент позволяет объединять тела в геометрически-жесткие множества.
           \en Component is able to unite solids into the rigid geometric sets. \~
*/
//---
MATH_FUNC(cdet_item) CDM_AddComponent( cdet_scene scn, const CDM_component & comp );

//---------------------------------------------------------------------------------------
/**
  \brief \ru Добавить экземпляр тела с заданным положением в сцену для детектора столкновений.
         \en Add a solid with given placement to the collision detection scene. \~
  \param[in] inst - \ru Запись экземпляра геометрического объекта (полигональное или граничное представление).
                    \en The instance record of geometric object (polygonal or boundary representation). \~   
  \return \ru Дескриптор объекта для детектора столкновений.
          \en Descriptor of object for collision detection. \~
  \details
  \par \ru Повторно-используемые элементы сцены
       \en Reused scene elements. \~
  \ru Один и тот же экземпляр класса 'MbItem' может быть добавлен в сцену сколько угодно раз с помощью данного вызова.
      При многократном добавлении одного и того же геометрического объекта детектор столкновений создает только одну
      поисковую структуру данных (BVT). Ресурсы памяти и времени вычислений, потраченные на создание такой структуры,
      повторно используется для всех экземпляров сцены, основанные на одном и том же объекте 'MbItem'. \~
  \en The same instance of the 'MbItem' class can be added to the scene as many times as necessary using this call.
      When the same geometric object is added multiple times, the collision detector creates only one
      search data structure (BVT). The memory and computation time resources spent to create this structure,
      are reused for all scene instances based on the same 'MbItem' object. \~
*/
//---
MATH_FUNC(cdet_item) CDM_AddInstance( cdet_scene scn, const CDM_instance & inst );

//---------------------------------------------------------------------------------------
/**
  \brief \ru Добавить новый экземпляр тела в компонент детектора столкновений.
         \en Add a new instance of a reused solid into the component of the collision detector. \~
  \param[in] compItem - \ru Компонент, которому будет принадлежать экземпляр.
                        \en A component to witch the instance will belong. \~
  \param[in] inst - \ru Запись экземпляра геометрического объекта (полигональное или граничное представление).
                    \en The instance record of geometric object (polygonal or boundary representation). \~
  \return \ru Дескриптор экземпляра тела, зарегистрированный в сцене детектора столкновений.
          \en Descriptor of solid instance registered in the detector. \~
  \note   \ru Значение compItem может быть нулевым. Значит просто вставка не будет
              принадлежать ни одному компоненту.
          \en The value compItem can be CDM_NULL. This just means that the
              instance doesn't belong to any component. \~
*/
//---
MATH_FUNC(cdet_item) CDM_AddInstance( cdet_scene scn, cdet_item compItem, const CDM_instance & inst );

//---------------------------------------------------------------------------------------
/**
  \brief \ru Добавить луч в сцену детектора столкновений.
         \en Add a ray to the collision detector scene. \~
  \param[in] ray - \ru Геометрическое представление луча
                   \en Geometric representation of ray. \~
  \return \ru Дескриптор объекта в сцене детектора столкновений.
          \en Descriptor of object for collision detector scene. \~
*/
//---
MATH_FUNC(cdet_item) CDM_AddRay( cdet_scene scn, const CDM_ray & ray );

//---------------------------------------------------------------------------------------
/**
  \brief \ru Удалить геометрический объект из сцены детектора столкновений.
         \en Remove a geometric object from the scene of collision detector. \~
  \param[in] scnItem - \ru Дескриптор объекта в сцене детектора столкновений.
                       \en Decriptor of object in collision detector scene. \~
*/
//---
MATH_FUNC(void) CDM_RemoveItem( cdet_scene scn, cdet_item scnItem );

//---------------------------------------------------------------------------------------
/**
 \brief \ru Поменять текущее положение геометрического объекта в сцене.
        \en Change current position of a geometric object. \~
  \param[in] scnItem - \ru Дескриптор объекта в сцене детектора столкновений.
                       \en Decriptor of object in collision detector scene. \~
  \param[in] place - \ru Положение, которое занимает объект в глобальной СК.
                     \en The placement that the object takes in global space. \~
*/
//---
MATH_FUNC(void) CDM_Reposition( cdet_scene scn, cdet_item scnItem, const MbPlacement3D & place );

//---------------------------------------------------------------------------------------
/**
  \brief \ru Запросить поиск столкновений между элементами сцены.
         \en Request a collision search between scene elements. \~
  \return \ru Коды результата детектора столкновений и измерение расстояния.
          \en Collision control result codes and distance measurement. \~
  \details
  \ru Осуществляется поиск столкновения между элементами сцены согласно запроса `cdQuery`,
      а именно проверяются пары, как на уровне сборки, включая компоненты целиком, так и
      на уровне отдельных компонент.  Клиентское приложение имеет возможность настроить запрос
      так, чтобы искать столкновения не всех со всеми, а только между группами объектов.
  \en The search for collisions between scene elements is performed according to the `cdQuery`
      structure, namely, pairs are checked both at the root level, including entire components,
      and at the level within a component. The client application has the ability to configure
      the query so as to search not for collisisons "all with all", but only between groups of objects. \~
*/
//---
MATH_FUNC(cdet_result) CDM_CheckCollisions( cdet_scene scn, cdet_query & cdQuery );

//---------------------------------------------------------------------------------------
/**
  \brief  \ru Проверить столкновения между геометрическими объектами сцены.
          \en Check collisions between geometric objects of the scene. \~
  \return \ru Коды результата детектора столкновений и измерение расстояния.
          \en Collision control result codes and distance measurement. \~
*/
//---
MATH_FUNC(cdet_result) CDM_CheckCollisions( cdet_scene scn );

//---------------------------------------------------------------------------------------
/**
  \brief \ru Выдать дескриптор клиентского приложения по дескриптору объекта сцены детектора столкновений.
          \en Get an application pointer by object descriptor of the collision detector scene. \~
  \param[in] scnItem - \ru Дескриптор объекта в сцене детектора столкновений.
                       \en Object descriptor in the collision detector scene. \~
  \return \ru Дескриптор клиентского приложения.
          \en Descriptor of client application. \~
*/
//---
MATH_FUNC(cdet_app_item) CDM_AppItem( cdet_scene scn, cdet_item scnItem );

//---------------------------------------------------------------------------------------
/**
  \brief \ru Экспортировать сцену детектора столкновений в модель C3D.
         \en Export collision detector scene into C3D model. \~
  \param[in] c3dFile - \ru Имя файла.
                       \en The name of the file. \~
  \return \ru Результат конвертирования.
          \en Result of converting operation. \~
*/
//---
MATH_FUNC(MbeConvResType) CDM_ExportToC3D( cdet_scene scn, const TCHAR * c3dFile );

//----------------------------------------------------------------------------------------
/**
  \brief \ru Выдать диапазон итераторов для обхода элементов, принадлежащих компоненту сцены
            (только на уровне этого компонента).
         \en Returns an iterator range over the elements of the given scene component (one level only). \~
  \param[in] scnItem - \ru Объект сцены детектора столкновений.
                       \en Object of the collision detector scene. \~
*/
//---
MATH_FUNC(CdSceneIteratorRange) CDM_SubItemsRange( cdet_scene scn, cdet_item scnItem );

//----------------------------------------------------------------------------------------
/**
  \brief \ru Выдать диапазон итераторов с обходом в глубину для компонента сцены.
         \en Returns an iterator range with depth‑first traversal of the given scene component. \~
  \param[in] scnItem - \ru Объект сцены детектора столкновений.
                       \en Object of the collision detector scene. \~
*/
//---
MATH_FUNC(CdSceneIteratorRange) CDM_InDepthSubItems( cdet_scene scn, cdet_item scnItem );

//----------------------------------------------------------------------------------------
/**
  \brief \ru Выдать диапазон итераторов всех объектов сцены дектектора столкновений.
         \en Return the iterator range of all objects in the collision detector scene. \~
*/
//---
MATH_FUNC(CdSceneIteratorRange) CDM_SceneItems( cdet_scene scn );

//---------------------------------------------------------------------------------------
/**
  \brief \ru Исключить из рассмотрения все модели. 
         \en Exclude all models from consideration. \~
  */
//---
MATH_FUNC(void) CDM_ClearScene( cdet_scene scn );

/** \} */ // Collision_Detection

#endif // __CDM_API_H

// eof