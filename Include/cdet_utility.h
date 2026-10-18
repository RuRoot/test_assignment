//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Утилита оценки столкновений и параметров близости тел.
         \en Utility of collision detection and proximity queries.
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CDET_UTILITY_H
#define __CDET_UTILITY_H

#include <cdm_types.h>
#include <conv_res_type.h>
#include <tool_cstring.h>

class  MtRefItem;
class  MbItem;
class  MbSolid;
class  MbMesh;
class  MbAssembly;
struct MbLumpAndFaces;
class  CdCollisionManager; // Internal implementation of the collision detection module.

/**
  \addtogroup Collision_Detection   
  \{
*/

//----------------------------------------------------------------------------------------
/** \brief \ru Утилита для поиска столкновений и запросов на измерения расстояний.
           \en Collision search and distance measurement query utility. \~
  \details \ru Предоставляет функциональность Collision Detection для взаимодействия 
               с приложением САПР.
           \en Provides facilities of The Collision Detection to interact with the CAD 
               application. \~
  \attention  \ru Для гарантированно правильной работы детектора необходимо, чтобы объект
    типа MbLumpAndFaces, добавляемый в рассмотрение посредством функции AddSolid, имел 
    правильную матрицу преобразования в мир в настоящем его положении, т.е. с самого начала.
              \en For the ensure proper functionality of the detector it is necessary that 
    an object of type MbLumpAndFaces to be added in consideration by function AddSolid will 
    have a correct matrix of transformation to the world coordinate system in its current 
    state, i.e. from the beginning. \~
*/
// ---
class MATH_CLASS CdCollisionDetection
{
  CdCollisionManager * m_manager;

public:
    CdCollisionDetection();
   ~CdCollisionDetection();

public:
    /** \brief \ru Выдать сцену геометрического поиска, которую обслуживает модуль столкновений.
               \en Get a geometric search scene, which is served by the collision detection utility. \~
    */    
  cdet_scene Scene() const;

public:
  /**
    \brief \ru Добавить твердое тело с заданным положением в набор для контроля столкновений.
           \en Add a solid with given placement to the collision detection set. \~
    \return \ru Дескриптор объекта для контроля столкновений. \en Descriptor of object for collision detection. \~
  */  
  cdet_item AddItem( const MbSolid & solid, const MbPlacement3D & place, cdet_app_item appItem = CDET_APP_NULL );
  /**
    \brief \ru Добавить луч по вершине и направлению с заданным положением в набор для контроля столкновений.
           \en Add a ray with given vertex, direction and placement to the collision detection set. \~
    \return \ru Дескриптор объекта для контроля столкновений. \en Descriptor of object for collision detection. \~
  */
  cdet_item AddRay( const MbCartPoint3D & vertex, const MbVector3D & direction, 
                    const MbPlacement3D & place, cdet_app_item appItem = CDET_APP_NULL );
  /**
    \brief \ru Добавить новый компонент контроля столкновений и параметров близости.
           \en Add a new component to track collisions and proximity parameters. \~
    \details \ru Компонент позволяет объединять тела в геометрически-жесткие множества.
             \en Component is able to unite solids into the rigid geometric sets.
  */
  cdet_item AddComponent( cdet_app_item );
  /**
    \brief \ru Добавить новый экземпляр тела в компонент контроля столкновений.
           \en Add a new instance of a reused solid into the component. \~
    \param[in] compItem - \ru Компонент, которому будет принадлежать экземпляр.
                          \en A component to witch the instance will belong.
    \param[in] solidItem - \ru Оригинальное тело, добавленное методом #AddItem, по которому изготавливается экземпляр.
                           \en An original solid added by the method #AddItem by which the instance is made.
    \param[in] place - \ru Положение, которое занимает тело экземпляра в глобальной СК.
                       \en The placement that the instance solid takes in global space.
    \return \ru Новый экземпляр тела, зарегистрированный в аппарате контроля столкновений.
            \en The new solid instance registered in the detector.
    \note   \ru Значение compItem может быть нулевым. Значит просто вставка не будет 
                принадлежать ни одному компоненту.
            \en The value compItem can be CDET_NULL. This just means that the 
                instance does not belong to any component.
  */
  cdet_item AddInstance( cdet_item compItem, cdet_item solidItem, const MbPlacement3D & place );
  /**
    \brief \ru Добавить новый экземпляр тела по образцу в компонент контроля столкновений.
           \en Add a new instance of a reused solid into the component. \~
    \param[in] compItem - \ru Компонент, которому будет принадлежать экземпляр.
                          \en A component to witch the instance will belong.
    \param[in] mbItem - \ru Оригинальное тело, по которому изготавливается экземпляр.
                           \en An original solid by which the instance is made.
    \param[in] place - \ru Положение, которое занимает тело экземпляра в глобальной СК.
                       \en The placement that the instance solid takes in global space.
    \param[in] appItem - \ru Геометрический объект пользователя. \en User geometric item.
    \return \ru Новый экземпляр тела, зарегистрированный в сцене контроля столкновений.
            \en The new solid instance registered in the detector.
    \note   \ru Значение compItem может быть нулевым. Значит просто вставка не будет 
                принадлежать ни одному компоненту.
            \en The value compItem can be CDET_NULL. This just means that the 
                instance does not belong to any component.
  */
  cdet_item AddInstance( cdet_item compItem, const MbItem* mbItem
                       , const MbPlacement3D & place, cdet_app_item appItem = CDET_APP_NULL );
  /**
    \brief \ru Удалить геометрический объект из набора для контроля столкновений.
           \en Remove a geometric object from the set of collision detection. \~
  */
  void      RemoveItem( cdet_item cdItem );
  /**
    \brief  \ru Поменять текущее положение геометрического объекта в наборе.
            \en Change current position of a geometric object. \~
  */
  void      Reposition( cdet_item, const MbPlacement3D & );
  /**
    \brief  \ru Проверить столкновения между геометрическими объектами набора.
            \en Check collisions between geometric objects of the set. \~
    \return \ru Функция вернет CDM_RESULT_CollisionDetected при обранужении хотя бы одной коллизии.
            \en The function will return CDM_RESULT_CollisionDetected if it detects at least one collision.
  */
  cdet_result CheckCollisions( cdet_query & );
  /**
    \brief  \ru Проверить столкновения между геометрическими объектами набора.
            \en Check collisions between geometric objects of the set. \~
    \return \ru Функция вернет CDM_RESULT_CollisionDetected при обранужении хотя бы одной коллизии.
            \en The function will return CDM_RESULT_CollisionDetected if it detects at least one collision.    
  */
  cdet_result CheckCollisions();
  /**
    \brief  \ru Выдать дескриптор клиентского приложения по дескриптору контрольного набора столкновений.  
            \en Get an application pointer by descriptor of the collision detection set.
  */
  cdet_app_item AppItem( cdet_item cdItem ) const;
  /**
  \brief \ru Выдать диапазон итераторов объекта/компонента сцены детектора столкновений.
         \en Return the iterator range of the scene object/component in the collision detector. \~
  */
  CdSceneIteratorRange SubItemsRange(cdet_item scnItem) const noexcept;
  /**
  \brief \ru Выдать диапазон итераторов с обходом в глубину для объекта/компонента сцены дектектора столкновений.
         \en Return the iterator range with in depth traversal for the scene object/component in the collision detector. \~
  */
  CdSceneIteratorRange InDepthSubItems(cdet_item scnItem) const noexcept;
  /**
  \brief \ru Выдать диапазон итераторов всех объектов сцены детектора столкновений.
         \en Return the iterator range of all objects in the collision detector scene. \~
  */
  CdSceneIteratorRange SceneItems() const noexcept;
  /**
    \brief  \ru Задать отклонение размеров тела наружу либо внутрь оболочки на величину допуска.
            \en Set the allowed deviation of the solid size inside or outside the shell. \~

    \param[in] cqItem - \ru Экземпляр сцены, как объект проверки на столкновение с другими.
                        \en An scene instance as an object to check collisions with others.
    \param[in] delta - \ru Предельное отклонение размеров геометрического объекта от своего номинального размера.
                       \en The maximum deviation of a geometric object size from its nominal size.
    \details \ru Величина отклонения может быть положительной или отрицательной, и определяет допуск,
                 влияющий на результат метода #CheckCollisions. Отрицательное отклонение (delta < 0) 
                 определяет максимальную глубину пересечения с телом, чтобы не засчитывать столкновение.
                 Положительное отклонение (delta>0) определяет минимальное расстояние до тела, 
                 с которым не засчитывается столкновение.
             \en The delta value can be positive or negative, and defines the tolerance that affects 
                 the #CheckCollisions result. A negative delta (delta < 0) defines the maximum interference depth 
                 with a solid to count a status "no collision".
                 A positive delta (delta>0) defines the minimum distance to an object to count a status "no collision".
    \note \ru По умолчанию каждое тело имеет нулевое отклонение размеров (delta = 0).
          \en By default, each body has a zero size deviation (delta = 0).
    \return \ru Функция вернет CDM_PARAMETER_Changed при успешной смене параметра настройки.
            \en The function returns CDM_PARAMETER_Changed if the tolerance parameter was successfully changed.
  */
  cdet_setting_result SetToleranceDelta( cdet_item cqItem, double delta );

public: // the functions below can be deprecated in future version.
  /*
    \brief \ru Добавить модель тела, как набор граней и решеток. 
           \en Add a solid data as a set of faces and the grids. \~
    \return \ru Внутренняя структура данных представляющая добавленную модель. \en Internal data structure representing added solid data. \~
  */
  cdet_item     AddSolid( const MbLumpAndFaces & );
  /// \note \ru Вызов устарел. Вместо него используйте AddInstance. \en The call is deprecated. Use AddInstance instead. \~
  cdet_item     AddMesh( const MbMesh & mesh, const MbPlacement3D & place, cdet_app_item appItem = CDET_APP_NULL, double delta = 0. );
  /// \ru Удалить твердотельную модель или луч из детектора столкновений. \en Remove a solid model or ray from a collision detector.
  void          RemoveSolid( cdet_item );
  /// \ru Выдать количество добавленных твердотельных моделей или лучей. \en Get number of added solid models or rays.
  size_t        Count() const;
  /// \ru Номер твердотельной модели или луча, зарегистрированной в детекторе. \en An index of solid model or ray registered in the detector.
  size_t        SolidIndex( cdet_item cItem ) const;
  /// \ru Вычисление минимального расстояния между объектами (см.функцию #SetDistanceTracking) \en Calculation of minimal distance between objects (see the function #SetDistanceTracking)
  cdet_result   DistanceQuery( CdProximityParameters & ) const;
  /// \ru Вычисление минимального расстояния между объектами \en Calculation of minimal distance between objects. \~
  cdet_result   DistanceQuery( cdet_item, cdet_item, CdProximityParameters & ) const;
  /// \ru Выключить из рассмотрения все модели. \en Exclude all models from consideration.
  void          FlushSolids();
  /// \ru Выдать иерархическое представление тела (CDET_NULL = отсутствие такового в списке). \en Get the hierarchical representation of the solid (CDET_NULL means that the solid is not in the list).
  cdet_item     GetHRepSolid( const MbLumpAndFaces & ) const;
  /// \ru Задать барьер для отличия касания от пересечения. \en Set the barrier for the difference between the touch and the intersection.
  void          SetTouchTolerance( double lTol );
  /// \ru Вкл./выкл. приближенного вычисления пересечений тел \en On/off approximated calculation of intersections of solids 
  void          SetApproxCollisionQuery( bool ff );
  /// \ru Вкл./выкл. приближенного вычисления параметров близости - по триангуляции \en On/off approximated calculation of proximity parameters - by triangulation 
  void          SetApproxDistanceComputation ( bool ff );
  /// \ru Назначить объекты для отслеживания между ними расстояния. \en Assign the pair to track the distance between them.
  void          SetDistanceTracking( const MbLumpAndFaces &, const MbLumpAndFaces & );
  /// \ru Обновить текущее положение тела или луча с индексом solIdx. \en Update current placement of solid or ray with index solIdx.
  void          SetPlacement( size_t solIdx, const MbPlacement3D & );
  /// \ru Экспортировать сцену поиска столкновений в модель C3D. \en Export collision detection scene into C3D model.
  MbeConvResType ExportToC3D(const TCHAR * c3dFile);

public:  /*
            Deprecated and testing functions
         */
  void    SetCallback( CDM_exam_func );

  // Use AppItem() insead this
  /*DEPRECATE_DECLARE*/ cdet_app_item Component(size_t solIdx) const; // It in use yet.
  // The func is deprecated. Use SetDistanceTracking instead.
  void    SetDistanceComputationObjects( const MbLumpAndFaces &, const MbLumpAndFaces & );
  // The func is deprecated. Use AddSolid/AddItem instead.
  size_t  AddLump( const MbLumpAndFaces & );
  // For testing purposes
  bool    IsEmpty( cdet_item ) const;
  // For testing purposes
  const MtRefItem * _ComputeBVTree( cdet_item );

private:
  
  OBVIOUS_PRIVATE_COPY( CdCollisionDetection );
};

/// \ru Узел дерева объемов. \en A node of the bounding volume tree.
typedef const MtRefItem * cdet_bvt_node;
/// \ru Пустое дерево объемов. \en An empty bounding volume tree.
const cdet_bvt_node CDET_BVT_NULL = nullptr;
/// \ru Пара ветвей поддерева объемов. \en A pair of branches of the bounding volume subtree.
typedef std::pair<cdet_bvt_node,cdet_bvt_node> cdet_bvt_pair;  

//---------------------------------------------------------------------------------------
/** 
  \brief  \ru Получить левую и правую ветви поддерева объемов.
          \en Get the left and the right branches of the bounding volume subtree.
*/
//---
MATH_FUNC(cdet_bvt_pair) BvtSubNodes( cdet_item, cdet_bvt_node );

//---------------------------------------------------------------------------------------
/** \brief  \ru Матрица получения ограничивающего параллелепипеда из единичного куба. 
            \en Transformation matrix yielding bounding parallelepiped from the unit cube.
*/
//---
MATH_FUNC(void) GetOrientedBox( cdet_item, cdet_bvt_node, MbMatrix3D & );

// deprecated typename since 2025.05. Use CdCollisionDetection instead.
using MbCollisionDetectionUtility = struct {};

#endif // __CDET_UTILITY_H

/** \} */

// eof