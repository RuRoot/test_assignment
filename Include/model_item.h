////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Объект геометрической модели.
         \en A model geometric object. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MODEL_ITEM_H
#define __MODEL_ITEM_H


#include <space_item.h>
#include <plane_item.h>
#include <topology_item.h>
#include <creator_transaction.h>
#include <attribute_container.h>
#include <mb_placement3d.h>
#include <name_item.h>
#include <math_define.h>
#include <tool_mutex.h>
#include <templ_visitor.h>


class  MbItem;
namespace c3d // namespace C3D
{
typedef SPtr<MbItem>                          ItemSPtr;
typedef SPtr<const MbItem>                    ConstItemSPtr;

typedef std::vector<MbItem *>                 ItemsVector;
typedef std::vector<const MbItem *>           ConstItemsVector;

typedef std::vector<ItemSPtr>                 ItemsSPtrVector;
typedef std::vector<ConstItemSPtr>            ConstItemsSPtrVector;

typedef std::set<MbItem *>                    ItemsSet;
typedef ItemsSet::iterator                    ItemsSetIt;
typedef ItemsSet::const_iterator              ItemsSetConstIt;
typedef std::pair<ItemsSetConstIt, bool>      ItemsSetRet;

typedef std::set<const MbItem *>              ConstItemsSet;
typedef ConstItemsSet::iterator               ConstItemsSetIt;
typedef ConstItemsSet::const_iterator         ConstItemsSetConstIt;
typedef std::pair<ConstItemsSetConstIt, bool> ConstItemsSetRet;
}


//------------------------------------------------------------------------------
/** \brief \ru Объект геометрической модели.
           \en A model geometric object. \~
  \details \ru Родительский класс объектов геометрической модели. \n
    Наследниками являются: \n
    локальная система координат MbAssistingItem,\n 
    точечный каркас MbPointFrame,\n 
    проволочный каркас MbWireFrame,\n 
    твёрдое тело MbSolid,\n
    полигональный объект MbMesh,\n
    вставка объекта в локальной системе координат MbInstance,\n
    сборка объектов в локальной системе координат MbAssembly,\n
    вставка трехмерного объекта MbSpaceInstance,\n
    вставка двумерного  объекта MbPlaneInstance в плоскости XY локальной системы координат.\n
    Объект содержит последовательность и способы своего построения MbTransactions.\n
    Объект содержит не геометрические свойства в виде контейнера атрибутов MbAttributeContainer.\n
    Имя объекта геометрической модели представляет собой контейнер простых имён.
    В начале контейнера содержится простое имя SimpleName, присвоенное объекту геометрической моделью MbModel. \n
    Если объект не держит в себе других объектов, то контейнер содержит одно простое имя SimpleName.
    Ели объект держит в себе другие объекты (MbAssembly или MbInstance), 
    то имя внутренних объектов представляет собой контейнер, содержащий как минимум два простых имени.
    Количество элементов имени объекта отражают количество уровней вложенности объект относительно модели.
           \en Parent class of model geometric objects. \n
    Inheritors are: \n
    local coordinate system of MbAssistingItem,\n 
    MbPointFrame point-frame,\n 
    MbWireFrame wireframe,\n 
    MbSolid solid,\n
    MbMesh polygonal planar object,\n
    MbInstance instance of object in the local coordinate system,\n
    MbAssembly assembly of objects in the local coordinate system,\n
    MbSpaceInstance instance of three-dimensional object,\n
    MbPlaneInstance instance of a two-dimensional object in the XY-plane of a local coordinate system.\n
    Object contains MbTransactions sequence and ways to construct itself.\n
    Object contains non-geometric properties as MbAttributeContainer attribute container.\n
    The name of an object of a geometric model is represented as a container of simple names.
    In the beginning of the container there is a SimpleName simple name assigned to object by MbModel geometric model. \n
    If the object doesn't contain other objects, then the container contains one SimpleName simple name.
    If the object contains other objects (MbAssembly or MbInstance), 
    then the internal objects name is represented as a container with at least two simple names.
    Number of the elements of an object's name corresponds to the number of levels of objects inclusion relative to the model. \~
  \ingroup Model_Items
*/
// ---
class MATH_CLASS MbItem : public MbSpaceItem,
                          public MbTransactions, 
                          public MbAttributeContainer,
                          public MbSyncItem {

private:
  SimpleName name; ///< \ru Имя объекта геометрической модели. \en Name of a geometric model object.

protected:
  /// \ru Конструктор копирования с регистратором дублирования. \en Copy-constructor with duplication registrator. 
  explicit MbItem( const MbItem &, MbRegDuplicate * ); 
public:
  /// \ru Конструктор. \en Constructor. 
  MbItem();
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbItem();

public :
  virtual void Accept( Visitor & visitor )
  {
    VisitorImpl<MbItem> * impl = dynamic_cast<VisitorImpl<MbItem> *>(&visitor);
    if( impl )
      impl->Visit( *this );
  }

  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */
  //virtual MbeSpaceType IsA()    const = 0; // \ru Тип объекта. \en A type of an object.
  MbeSpaceType Type()   const override;     // \ru Групповой тип объекта. \en Group type of object.
  MbeSpaceType Family() const override;     // \ru Семейство объекта. \en Family of object. 
  MbeImplicationType ImplicationType() const override; // \ru Тип контейнера атрибутов - классификатор наследников. \en Type of an attribute container is a classifier of inheritors. 
  //virtual MbSpaceItem & Duplicate ( MbRegDuplicate * = nullptr ) const = 0; // \ru Создать копию. \en Create a copy.
  //virtual void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) = 0; // \ru Преобразовать согласно матрице. \en Transform according to the matrix.
  //virtual void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) = 0; // \ru Сдвинуть вдоль вектора. \en Translate along a vector.
  //virtual void    Rotate   ( const MbAxis3D   &, double angle, MbRegTransform * = nullptr ) = 0; // \ru Повернуть вокруг оси. \en Rotate about an axis.
  //virtual bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const = 0; // \ru Являются ли объекты равными? \en Are the objects equal?
  //virtual bool    SetEqual ( const MbSpaceItem & init ) = 0;   // \ru Сделать объекты равными. \en Make the objects equal.
  //virtual double  DistanceToPoint ( const MbCartPoint3D & ) const = 0; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point.
  //virtual void    AddYourGabaritTo( MbCube & r ) const = 0; // \ru Добавь свой габарит в куб. \en Add bounding box into a cube.
  //virtual void    CalculateLocalGabarit( const MbMatrix3D & into, MbCube & cube ) const = 0; // \ru Рассчитать габарит в локальной системы координат. \en Calculate bounding box in the local coordinate system.

  MbProperty & CreateProperty( MbePrompt n ) const override;     // \ru Создать собственное свойство. \en Create a custom property. 
  void    GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & properties ) override; // \ru Установить свойства объекта. \en Set properties of the object. 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  virtual void    GetBasisPoints( MbControlData & ) const; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  virtual void    SetBasisPoints( const MbControlData & ); // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  /**
    \brief  \ru Получить систему координат объекта, если она есть. 
            \en Get the coordinate system of an item if it is exist.
    \return \ru Функция вернет true, если объект имеет собственную подсистему координат, 
                иначе считается, что ЛСК объекта всегда "стандартная" (MbPlacement3D::global).
            \en The function returns true, if the object have its own local coordinate system,
                otherwise it is considered that the object LCS is always "standard" (MbPlacement3D :: global).
    \param[out] place - \ru Система координат объекта.
                        \en The coordinate system of object. \~
  */
  virtual bool    GetPlacement( MbPlacement3D & place, PlanarCheckParams = PlanarCheckParams() ) const { place = MbPlacement3D::global; return false; }
  /// \ru Установить систему координат объекта, если возможно. \en Set the coordinate system of an item if it is possible.
  virtual bool    SetPlacement( const MbPlacement3D & ) { return false; }

  /** \brief \ru Построить полигональную копию mesh.
             \en Build polygonal copy mesh. \~
    \details \ru Построить полигональную копию данного объекта, представленную полигонами, или/и плоскими пластинами.
             \en Build a polygonal copy of the object that is represented by polygons or/and fasets. \~
    \param[in] stepData -  \ru Данные для вычисления шага при построении полигонального.
                           \en Data for еру step calculation for polygonal object. \~
    \param[in] note     -  \ru Способ построения полигонального объекта.
                           \en Way for polygonal object constructing. \~
    \param[in, out] mesh - \ru Построенный полигональный объект.
                           \en The builded polygonal object.
  */
  //virtual void    CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const = 0;
  /** \} */

  /** \ru \name Общие функции объекта геометрической модели
      \en \name Common functions of object of geometric model.
      \{ */

  /** \brief \ru Перестроить объект по журналу построения.
             \en Reconstruct object according to the history tree. \~
    \details \ru Создать заново объект по журналу построения.
             \en Create object by the history tree. \~
    \param[in] sameShell - \ru Полнота копирования элементов.
                           \en Whether to perform complete copying of elements while constructing. \~
    \param[out] items - \ru Контейнер для складывания элементов невыполненных построений (может быть nullptr).
                        \en Container for the elements of not performed constructions (can be nullptr). \~
    \return \ru Перестроен ли объект.
            \en Whether an object is constructed. \~
  \ingroup Model_Items
  */
  bool    RebuildItem( MbeCopyMode sameShell, RPArray<MbSpaceItem> * items, IProgressIndicator * progInd ) override;

  /** \brief \ru Создать полигональный объект.
             \en Create polygonal object. \~
    \details \ru Создать полигональный объект - упрощенную копию данного объекта.
             \en Create a polygonal object - a polygonal copy of the given object. \~
    \param[in] stepData - \ru Данные для вычисления шага при триангуляции.
                          \en Data for step calculation during triangulation. \~
    \param[in] note     - \ru Способ построения полигонального объекта.
                          \en Way for polygonal object constructing. \~
    \return \ru Построенный полигональный объект.
            \en Created polygonal object. \~
    \ingroup Model_Items
  */
  virtual MbItem * CreateMesh( const MbStepData & stepData, const MbFormNote & note, MbRegDuplicate * iReg ) const = 0; 

  /** \brief \ru Добавить полигональный объект.
             \en Add polygonal object. \~
    \details \ru Добавить свою полигональную копию в присланный полигональный объект.
             \en Add your own polygonal copy to the given polygonal object. \~
    \param[in] stepData - \ru Данные для вычисления шага при триангуляции.
                          \en Data for step calculation during triangulation. \~
    \param[in] note     - \ru Способ построения полигонального объекта.
                          \en Way for polygonal object constructing. \~
    \param[out] mesh - \ru Присланный полигональный объект.
                       \en Given polygonal object. \~
    \return \ru Добавлен ли объект.
            \en Whether the object is added. \~
    \ingroup Model_Items
  */
  virtual bool    AddYourMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const; 

  /** \brief \ru Разрезать полигональный объект одной или двумя параллельными плоскостями.
             \en Cut the polygonal object by one or two parallel planes. \~
    \details \ru Построить полигональный объект из части исходного полигонального объекта, 
      лежащей под плоскостью XY локальной системы координат на заданном расстоянии.\n
      Функция "режет" только полигональный объект MbMesh.
      Функция "режет" объект двумя плоскостями: 
      плоскостью XY локальной системы координат place и плоскостью, параллельной ей и 
      расположенной на расстоянии distance ниже неё.
      Если distance<=0, то функция "режет" объект только одной плоскостью XY локальной системы.\n
      Содержимое объекта, необходимое для построения разрезанного объекта и не затронутое режущими плоскостями,
      добавляется в возвращаемый разрезанный объект без копирования.\n
             \en Create polygonal object from a part of source polygonal object 
      which located under XY-plane of local coordinate system at given distance.\n
      Function 'cuts' only MbMesh polygonal object.
      Function 'cuts' the object by two planes: 
      XY plane of 'place' local coordinate system and plane parallel to it and 
      located at 'distance' distance below it.
      If 'distance' is less than or equal to zero, then the function "cuts" an object only by one XY plane of local coordinate system.\n
      Contents of an object that are necessary for creation of cut object and not affected by cutting planes
      are added to returned cut object without copying.\n \~
    \param[in] cutPlace - \ru Локальная система координат, плоскость XY которой задаёт режущую плоскость.
                          \en A local coordinate system which XY plane defines a cutting plane. \~
    \param[in] distance - \ru Расстояние до параллельной режущей плоскости откладывается в отрицательную сторону оси Z локальной системы.
                          \en Distance to a parallel cutting plane is measured in negative direction of Z-axis of local coordinate system. \~
    \param[in] names    - \ru Именователь.
                          \en An object defining names generation in the operation. \~
    \result \ru Возвращает новый полигональный объект, лежащий под плоскость XY локальной системы координат на заданном расстоянии.
            \en Returns new polygonal object that located under XY-plane of local coordinate system at given distance. \~
    \ingroup Model_Items
  */
  virtual MbItem * CutMesh( const MbPlacement3D & cutPlace, double distance, const MbSNameMaker * names = nullptr ) const;

  /** \brief \ru Найти ближайший объект или имя ближайшего объекта.
             \en Find the nearest object or name of the nearest object. \~
    \details \ru Найти ближайший трехмерный объект или его имя по типу объекта и 
      составляющий элемент искомого объекта или его имя по топологическому или двумерному типу элемента (по требованию)
      на расстоянии от прямой, не превышающем заданной величины.
      Функция предназначена для идентификации геометрического объекта, породившего полигональный объект.
      Реальный поиск выполняется для элементов MbPrimitive полигонального объекта MbMesh, 
      у которых берётся информация о породившем примитив геометрическом объекте.
             \en Find the nearest three-dimensional object or its name by type of object and 
      component of the required object or its name by topological or two-dimensional type of the element (on demand)
      at distance from line less than or equal to the given value.
      Function is intended for identification of a geometric object which is begetter of a polygonal object.
      The real search is performed for MbMesh polygonal object's MbPrimitive elements 
      from which the information is taken about geometric object which is begetter of the primitive. \~
    \param[in] sType - \ru Тип искомого объекта.
                       \en Type of required object. \~
    \param[in] tType - \ru Топологический тип составляющего элемента искомого объекта.
                       \en Topological type of the required object's component. \~
    \param[in] pType - \ru Двумерный тип составляющего элемента искомого объекта.
                       \en Two-dimensional type of the required object's component. \~
    \param[in] axis  - \ru Прямая поиска.
                       \en Line of search. \~
    \param[in] maxDistance - \ru Расстояние от прямой, на котором ищется объект.
                             \en Distance from the line on which the object is looked for. \~
    \param[in] gridPriority - \ru Повышенный приоритет триангуляционной сетки при поиске.
                              \en Increased priority triangulation grid when searching. \~
    \param[out] t    - \ru Параметр прямой для найденной точки.
                       \en Parameter of found point on line. \~
    \param[out] dMin - \ru Найденное расстояние объекта от прямой.
                       \en Found distance from line to an object. \~
    \param[out] find - \ru Найденный объект.
                       \en Found object. \~
    \param[out] findName - \ru Имя найденного объекта.
                           \en Name of the found object. \~
    \param[out] element - \ru Найденный составляющий элемент объекта.
                          \en Found component of the object. \~
    \param[out] elementName - \ru Имя найденного составляющего элемента объекта.
                              \en Name of found component of the object. \~
    \param[out] path - \ru Путь положения объекта в модели.
                       \en Object's path in the model. \~
    \param[out] from - \ru Матрица преобразования найденного объекта в глобальную систему координат.
                       \en Transformation matrix of the found object to the global coordinate system. \~
    \return \ru Найден ли объект или его имя.
            \en Whether the object or its name is found. \~
    \ingroup Model_Items
  */
  virtual bool    NearestMesh( MbeSpaceType sType, MbeTopologyType tType, MbePlaneType pType, 
                               const MbAxis3D & axis, double maxDistance, bool gridPriority, double & t, double & dMin, 
                               MbItem *& find, SimpleName & findName,
                               MbRefItem *& element, SimpleName & elementName,
                               MbPath & path, MbMatrix3D & from ) const; 

  /** \brief \ru Дать все объекты указанного типа.
             \en Get all objects by the type. \~
    \details \ru Дать все объекты указанного типа, а также матрицы преобразования их в глобальную систему координат. \n
             \en Get all objects by the type along with transformation matrices to the global coordinate system. \n \~
    \param[in] type - \ru Тип объекта.
                      \en Object's type. \~
    \param[in] from - \ru Исходная матрица преобразования в глобальную систему координат.
                      \en Initial transformation matrix to the global coordinate system. \~
    \param[out] items - \ru Множество найденных объектов.
                        \en Found objects. \~
    \param[out] matrs - \ru Матрицы преобразования найденных объектов в глобальную систему координат.
                        \en Transformation matrices of found objects to the global coordinate system. \~
    \return \ru Добавлен ли данный объект.
            \en Whether to add this object. \~
    \ingroup Model_Items
  */
  virtual bool GetItems(       MbeSpaceType                type, 
                         const MbMatrix3D                & from, 
                               std::vector<SPtr<MbItem>> & items, 
                               std::vector<MbMatrix3D>   & matrs );

  /** \brief \ru Дать все объекты указанного типа.
             \en Get all objects by type. \~
    \details \ru Дать все объекты указанного типа, 
      а также матрицы преобразования их в глобальную систему координат. \n
             \en Get all objects by type 
      and get transformation matrix to the global coordinate system. \n \~
    \param[in] type - \ru Тип объекта.
                      \en Object's type. \~
    \param[in] from - \ru Исходная матрица преобразования в глобальную систему координат.
                      \en Initial transformation matrix to the global coordinate system. \~
    \param[out] items - \ru Множество найденных объектов.
                        \en Found objects. \~
    \param[out] matrs - \ru Матрицы преобразования найденных объектов в глобальную систему координат.
                        \en Transformation matrix of found objects to the global coordinate system. \~
    \return \ru Добавлен ли данный объект.
            \en Whether add this object. \~
    \ingroup Model_Items
  */
  virtual bool    GetItems( MbeSpaceType type, const MbMatrix3D & from, 
                            RPArray<MbItem> & items, SArray<MbMatrix3D> & matrs ); 

  /** \brief \ru Дать все уникальные объекты указанного типа.
             \en Get all unique objects by type. \~
    \details \ru Дать все уникальные объекты указанного типа. \n
             \en Get all unique objects by type. \n \~
    \param[in] type - \ru Тип объекта.
                      \en Object's type. \~
    \param[out] items - \ru Множество найденных объектов.
                        \en Found objects. \~
    \return \ru Добавлен ли данный объект.
            \en Whether add this object. \~
    \ingroup Model_Items
  */
  virtual bool    GetUniqItems( MbeSpaceType type, CSSArray<const MbItem *> & items ) const; 

  /** \brief \ru Дать объект по его пути.
             \en Get the object by its path. \~
    \details \ru Дать объект по его пути положения в модели и 
      дать матрицу преобразования объекта в глобальную систему координат.
      Объект может содержаться в другом объекте (в сборке или вставке).
             \en Get the object by path of its position in the model and 
      get transformation matrix of the object to the global coordinate system.
      Object can be contained in other object (in assembly or in instance). \~
    \param[in] path - \ru Путь объекта.
                      \en Path of object. \~
    \param[in] ind  - \ru Индекс требуемого объекта в path.
                      \en Index of the desired object in 'path'. \~
    \param[out] from - \ru Матрица преобразования объекта в глобальную систему координат.
                       \en Transformation matrix of object to the global coordinate system. \~
    \param[in] currInd - \ru Индекс текущего объекта в path.
                         \en Index of current object in path. \~
    \return \ru Найден ли путь и матрица объекта.
            \en Whether the path and the matrix of object are found. \~
    \ingroup Model_Items
    */
  virtual const MbItem * GetItemByPath( const MbPath & path, size_t ind, MbMatrix3D & from, size_t currInd = 0 ) const; 

  /** \brief \ru Найти объект по геометрическому объекту.
             \en Find object by geometric object. \~
    \details \ru Найти объект по геометрическому объекту, 
      а также получить путь к объекту в модели 
      и матрицу преобразования в глобальную систему координат. \n
             \en Find object by geometric object 
      and also get the path to the object in model 
      and get transformation matrix to the global coordinate system. \n \~
    \param[in] s - \ru Геометрический объект.
                   \en Geometric object. \~
    \param[out] path - \ru Путь к объекту в модели.
                       \en Path to object in the model. \~
    \param[out] from - \ru Матрица преобразования объекта в глобальную систему координат.
                       \en Transformation matrix of object to the global coordinate system. \~
    \return \ru Найден ли путь и матрица объекта.
            \en Whether the path and the matrix of object are found. \~
    \ingroup Model_Items
  */
  virtual const MbItem * FindItem( const MbSpaceItem * s, MbPath & path, MbMatrix3D & from ) const;

  /** \brief \ru Найти объект по геометрическому объекту.
             \en Find object by geometric object. \~
    \details \ru Найти объект по геометрическому объекту, 
      а также получить путь к объекту в модели 
      и матрицу преобразования в глобальную систему координат. \n
             \en Find object by geometric object 
      and also get the path to the object in model 
      and get transformation matrix to the global coordinate system. \n \~
    \param[in] s - \ru Геометрический объект.
                   \en Geometric object. \~
    \param[out] path - \ru Путь к объекту в модели.
                       \en Path to object in the model. \~
    \param[out] from - \ru Матрица преобразования объекта в глобальную систему координат.
                       \en Transformation matrix of object to the global coordinate system. \~
    \return \ru Найден ли путь и матрица объекта.
            \en Whether the path and the matrix of object are found. \~
    \ingroup Model_Items
  */
  virtual const MbItem * FindItem( const MbPlaneItem * s, MbPath & path, MbMatrix3D & from ) const;

  /** \brief \ru Найти объект по объекту геометрической модели.
             \en Find object by object of geometric model \~
    \details \ru Найти объект по объекту геометрической модели.
      а также получить путь к объекту в модели 
      и матрицу преобразования в глобальную систему координат. \n
             \en Find object by object of geometric model
      and also get the path to the object in model 
      and get transformation matrix to the global coordinate system. \n \~
    \param[in] s - \ru Геометрический объект.
                   \en Geometric object. \~
    \param[out] path - \ru Путь к объекту в модели.
                       \en Path to object in the model. \~
    \param[out] from - \ru Матрица преобразования объекта в глобальную систему координат.
                       \en Transformation matrix of object to the global coordinate system. \~
    \return \ru Найден ли путь и матрица объекта.
            \en Whether the path and the matrix of object are found. \~
    \ingroup Model_Items
  */
  virtual const MbItem * FindItem( const MbItem * s, MbPath & path, MbMatrix3D & from ) const; 

  /** \brief \ru Найти объект по имени.
             \en Find object by name. \~
    \details \ru Найти объект по имени, а также получить путь к объекту в модели 
      и матрицу преобразования в глобальную систему координат. \n
             \en Find object by name and also get path to object in model 
      and get transformation matrix to the global coordinate system. \n \~
    \param[in] n - \ru Имя объекта.
                   \en A name of an object. \~
    \param[out] path - \ru Путь к объекту в модели.
                       \en Path to object in the model. \~
    \param[out] from - \ru Матрица преобразования объекта в глобальную систему координат.
                       \en Transformation matrix of object to the global coordinate system. \~
    \return \ru Найден ли путь и матрица объекта.
            \en Whether the path and the matrix of object are found. \~
    \ingroup Model_Items
  */
  virtual const MbItem * GetItemByName( SimpleName n, MbPath & path, MbMatrix3D & from ) const; 

  /** \brief \ru Преобразовать выбранный объект согласно матрице.
             \en Transform selected object according to the matrix. \~
    \details \ru Преобразовать выбранный простой объект согласно матрице c использованием регистратора.
      Если объект содержит другие объекты геометрической модели, то преобразуется выбранное содержимое.
             \en Transform selected simple object according to the matrix using the registrator.
      If object contains other objects of geometric model then selected contents will be transformed. \~ 
    \param[in] matr - \ru Матрица преобразования.
                      \en A transformation matrix. \~
    \param[in] iReg - \ru Регистратор.
                      \en Registrator. \~
    \ingroup Model_Items
  */
  virtual void    TransformSelected( const MbMatrix3D & matr, MbRegTransform * iReg = nullptr );

  /** \brief \ru Сдвинуть выбранный объект вдоль вектора.
             \en Move selected object along a vector. \~
    \details \ru Сдвинуть вдоль вектора с использованием регистратора выбранный простой объект. 
      Если объект содержит другие объекты геометрической модели, то преобразуется выбранное содержимое.
             \en Move selected simple object along the vector using the registrator. 
      If object contains other objects of geometric model then selected contents will be transformed. \~ 
    \param[in] to   - \ru Вектор сдвига.
                      \en Translation vector. \~
    \param[in] iReg - \ru Регистратор.
                      \en Registrator. \~
    \ingroup Model_Items
  */
  virtual void    MoveSelected( const MbVector3D & to, MbRegTransform * iReg = nullptr );

  /** \brief \ru Повернуть выбранный объект вокруг оси на заданный угол.
             \en Rotate selected object by a given angle about an axis. \~
    \details \ru Повернуть вокруг оси на заданный угол с использованием регистратора выбранный простой объект. 
      Если объект содержит другие объекты геометрической модели, то преобразуется выбранное содержимое.
             \en Rotate selected simple object about the axis by the given angle using the registrator. 
      If object contains other objects of geometric model then selected contents will be transformed. \~ 
    \param[in] axis - \ru Ось поворота.
                      \en The rotation axis. \~
    \param[in] angle - \ru Угол поворота.
                       \en The rotation angle. \~
    \param[in] iReg - \ru Регистратор.
                      \en Registrator. \~
    \ingroup Model_Items
  */
  virtual void    RotateSelected( const MbAxis3D & axis, double angle, MbRegTransform * iReg = nullptr );

  /// \ru Дать матрицу преобразования из локальной системы объекта. \en Get transform matrix from local coordinate system of object.
  virtual  bool   GetMatrixFrom( MbMatrix3D & from ) const;
  /// \ru Дать матрицу преобразования в  локальную систему объекта. \en Get transform matrix into local coordinate system of object.
  virtual bool    GetMatrixInto( MbMatrix3D & into ) const;

          /// \ru Копировать строители и атрибуты. \en Copy creators and attributes. 
          void    Assign( const MbItem & other );  
          /// \ru Копировать имя объекта. \en Copy the name of an object. 
          void    CopyItemName( const MbItem & other ) { name = other.GetItemName(); }
          /// \ru Выдать имя объекта. \en Get name of object. 
          SimpleName GetItemName() const { return name; }
          /// \ru Установить имя объекта. \en Set name of the object. 
          void    SetItemName( SimpleName n ) { name = n; }
          /// \ru Соответствует ли знаковый атрибут объекту? \en Whether a sign attribute matches an object?
          bool    IsAttributeEqual( int attribute );

  /** \} */

protected:
          /// \ru Захватить объект, если ядро работает в многопоточном режиме. \en Catch object if multithreading mode is on. 
          void    LockItem() const;
          /// \ru Освободить объект, если ядро работает в многопоточном режиме. \en Release object if multithreading mode is on. 
          void    UnlockItem() const;

private:
  /** \brief \ru Построить путь положения объекта.
             \en Create path of object's position. \~
    \details \ru Построить путь положения объекта в модели и 
      дать матрицу преобразования объекта в глобальную систему координат.
      Объект может содержаться в другом объекте (в сборке или вставке).
             \en Create path of object's position in the model and 
      get transformation matrix of the object to the global coordinate system.
      Object can be contained in other object (in assembly or in instance). \~
    \param[in]  obj - \ru Объект.
                      \en Object. \~
    \param[out] path - \ru Путь объекта.
                       \en Path of object. \~
    \param[out] from - \ru Матрица преобразования объекта в глобальную систему координат.
                       \en Transformation matrix of object to the global coordinate system. \~
    \return \ru Найден ли путь и матрица объекта.
            \en Whether the path and the matrix of object are found. \~
    \ingroup Model_Items
    */
  virtual bool    MakePath( const MbItem & obj, MbPath & path, MbMatrix3D & from ) const;

DECLARE_PERSISTENT_CLASS( MbItem )
OBVIOUS_PRIVATE_COPY( MbItem )
}; // MbItem

IMPL_PERSISTENT_OPS( MbItem )


//----------------------------------------------------------------------------------------
// The functor implementing less operator of two model objects.
//---
struct LessName
{
  bool operator()( const MbItem * _Left, const MbItem * _Right ) const
  {
    return (_Left->GetItemName() < _Right->GetItemName());
  }
  bool operator()( const MbItem * _Left, SimpleName _Right ) const
  {
    return _Left->GetItemName() < _Right;
  }
  bool operator()( SimpleName _Left, const MbItem * _Right ) const
  {
    return _Left < _Right->GetItemName();
  }
};


namespace c3d // namespace C3D
{

//------------------------------------------------------------------------------
/// \ru Удалить копии построителей в объектах. \en Delete creators' copies. 
// ---
template <class ItemsVector>
bool DeleteCreatorsCopies( ItemsVector & items, double eps = LENGTH_EPSILON )
{
  bool changed = false;

  size_t itemsCnt = items.size();
  c3d::CreatorsSPtrVector creators1, creators2;

  if ( itemsCnt > 1 ) {
    for ( size_t i = 0; i < itemsCnt; ++i ) {
      MbItem * item1 = items[i];
      if ( item1 == nullptr )
        continue;
      creators1.clear();
      item1->GetCreators( creators1 );
      size_t creatorsCnt1 = creators1.size();

      if ( creatorsCnt1 > 0 ) {
        for ( size_t j = i + 1; j < itemsCnt; ++j ) {
          MbItem * item2 = items[j];
          if ( item2 == nullptr )
            continue;
          creators2.clear();
          item2->GetCreators( creators2 );
          size_t creatorsCnt2 = creators2.size();

          if ( creatorsCnt2 > 0 ) {
            bool replace = false;
            for ( size_t k1 = 0; k1 < creatorsCnt1; ++k1 ) {
              MbCreator * creator1 = creators1[k1];
              if ( creator1 == nullptr )
                continue;
              for ( size_t k2 = k1 + 1; k2 < creatorsCnt2; ++k2 ) {
                MbCreator * creator2 = creators2[k2];
                if ( creator2 == nullptr )
                  continue;
                if ( (creator1 != creator2) && creator1->IsSame( *creator2, eps ) ) {
                  creators2[k2] = creators1[k1];
                  replace = true;
                }
              }
            }
            if ( replace ) {
              item2->DeleteCreators();
              item2->AddCreators( creators2 );
              changed = true;
            }
          }
        }
      }
    }
  }
  return changed;
}

} // namespace C3D


#endif // __MODEL_ITEM_H
