//////////////////////////////////////////////////////////////////////////////////////////
/** \file 
    \brief  \ru Геометрическая модель.
            \en Geometric model. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MODEL_ENTITY_H
#define __MODEL_ENTITY_H


#include <model_item.h>
#include <vector>

struct ItModelVisitor;

//----------------------------------------------------------------------------------------
/** \brief \ru Геометрическая модель.
           \en Geometric model. \~
  \details \ru Геометрическая модель - контейнер геометрических объектов. \n
    Модель состоит из массивов указателей на объекты геометрической модели MbItem.
    Модель может содержать вспомогательные объекты MbAssistingItem, 
    точки MbPointFrame, каркасы MbWireFrame,
    твердые тела MbSolid, полигональные объекты MbMesh,
    объекты MbSpaceInstance и MbPlaneInstance.\n
    Модель используется для описания геометрических свойств реальных и 
    воображаемых объектов, визуализации моделируемых объектов, 
    вычисления геометрических характеристик моделируемых объектов.\n
    Имя объекта геометрической модели представляет собой контейнер простых имён.
    В начале контейнера содержится простое имя SimpleName, 
    которое совпадает с первым полем std::multimap геометрической модели. \n
    Если объект не держит в себе других объектов, то контейнер содержит одно простое имя SimpleName.
    Ели объект держит в себе другие объекты (MbAssembly или MbInstance), 
    то имя внутренних объектов представляет собой контейнер, содержащий как минимум два простых имени.
    Количество элементов имени объекта отражают количество уровней вложенности объект относительно модели.
           \en Geometric model is a container of geometric objects. \n
    The model consists of arrays of pointers to geometric model objects MbItem.
    The model can contain MbAssistingItem assisting items, 
    MbPointFrame points, MbWireFrame frames,
    MbSolid solids, polygonal objects MbMesh,
    MbSpaceInstance and MbPlaneInstance objects.\n
    Model is used to describe geometric properties of real and 
    imaginary objects, to visualize modeled objects, 
    to calculate geometric properties of modeled objects.\n
    The name of an object of a geometric model is represented as a container of simple names.
    In the beginning of the container there is a SimpleName simple name 
    which coincides with the first field std::multimap of the geometric model. \n
    If the object doesn't contain other objects, then the container contains one SimpleName simple name.
    If the object contains other objects (MbAssembly or MbInstance), 
    then the internal objects name is represented as a container with at least two simple names.
    Number of the elements of an object's name corresponds to the number of levels of objects inclusion relative to the model. \~
  \ingroup Model
*/
//---
class MATH_CLASS MbModel : public TapeBase, 
                           public MbRefItem,
                           public MbTransactions, 
                           public MbAttributeContainer 
{
public:
  typedef std::map<SimpleName, MbItem *> NameItemArray;

private:
  NameItemArray modelItems; ///< \ru Множество объектов модели. \en Set of the model objects. 
  SimpleName    name;       ///< \ru Имя объекта. \en A name of an object. 

protected:
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator.
  explicit MbModel( const MbModel &, MbRegDuplicate * );

public:
  /// \ru Конструктор по имени объекта. \en Constructor by object's name. 
  MbModel( SimpleName n = 0 );
  /// \ru Деструктор \en Destructor 
  virtual ~MbModel();

public :

  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */
  /// \ru Тип контейнера атрибутов - классификатор наследников. \en Type of an attribute container is a classifier of inheritors. 
  MbeImplicationType ImplicationType() const override;

          /// \ru Создать копию. \en Create a copy. 
          MbModel & Duplicate( MbRegDuplicate * = nullptr ) const;
          /// \ru Преобразовать согласно матрице. \en Transform according to the matrix.  
          void    Transform( const MbMatrix3D &, MbRegTransform * iReg = nullptr );
          /// \ru Сдвинуть вдоль вектора. \en Translate along a vector. 
          void    Move     ( const MbVector3D &, MbRegTransform * iReg = nullptr );
          /// \ru Повернуть вокруг оси. \en Rotate about an axis. 
          void    Rotate   ( const MbAxis3D   &, double angle, MbRegTransform * iReg = nullptr );
          /// \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
          double  DistanceToPoint ( const MbCartPoint3D & ) const; 
          /// \ru Добавь свой габарит в габаритный куб. \en Include your own bounding box into bounding box. 
          void    AddYourGabaritTo( MbCube & ) const; 

          /// \ru Создать собственное свойство с заданием его имени. \en Create your own property with specified name. 
          MbProperty & CreateProperty( MbePrompt ) const;
          /// \ru Выдать свойства объекта. \en Get properties of the object. 
          void    GetProperties( MbProperties & ); 
          /// \ru Установить свойства объекта. \en Set properties of the object. 
          void    SetProperties( const MbProperties & ); 
  /** \} */

          /// \ru Выдать имя модели. \en Get name of a model. 
          SimpleName  GetModelName() const { return name; } 
          /// \ru Установить имя модели. \en Set name of a model. 
          void        SetModelName( SimpleName n ) { name = n; } 

          /** \brief \ru Добавить объект в модель.
                     \en Add object to the model. \~
            \details \ru Добавить объект в модель с указанным именем.
                     \en Add object to the model with a given name. \~
            \param[in] item - \ru Объект модели.
                              \en A model object. \~
            \param[in] n - \ru Имя объекта. Если указанное имя равно нулю, то модель именует объект своим уникальным именем.
                           \en A name of an object. If a given name is equal to zero, then the model names an object with its unique name. \~
            \return \ru Добавленный объект.
                    \en Added object. \~
          */
          MbItem *  AddItem( MbItem & item, SimpleName n = c3d::UNDEFINED_SNAME ); 
          /// \ru Добавить объекты модели item в модель. \en Add item objects of to the model. \~
          bool      AddModel( const MbModel & ); 

          // \ru Выдать объект модели по индексу. \en Get item of model by index.
  const   MbItem *  GetItem( size_t ind ) const;
          // \ru Выдать непосредственный объект модели по идентификатору. \en Get the immediate item of model by identifier.
  const   MbItem *  SubItem( SimpleName n ) const;

          /** \brief \ru Заменить объект.
                     \en Replace the object. \~
            \details \ru Заменить объект новым.
                     \en Replace the object by a new one. \~
            \param[in] item - \ru Заменяемый объект.
                              \en An object to replace. \~
            \param[in] newItem - \ru Новый объект.
                                 \en A new object. \~
            \return \ru Возвращает true, если замена была выполнена.
                    \en Returns true if the replacement has been done. \~
          */
          bool    ReplaceItem( const MbItem & item, MbItem & newItem, bool saveName = false ); 

          /// \ru Дать все объекты. \en Get all the objects.  
          template <class Items>
          void    GetItems( Items & ) const; 
          /// \ru Отцепить объект, если такой есть в модели (рекурсивно). \en Detach the item if it is in the model (recursively).
          /**
            /attention \ru Функция не удаляет объект item из памяти даже при обнулении счетчика ссылок!
                       \en The function does not delete the item object from memory even if the reference counter is zero!
          */
          bool    DetachItem ( MbItem * item, bool resetName = true ); 
          /// \ru Отцепить все объекты. \en Detach all the objects. 
          template <class Items>
          void    DetachItems( Items & ); 
          /// \ru Удалить объект, если он есть в модели. \en Delete an object if it is in the model. 
          bool    DeleteItem ( MbItem *, bool resetName = true ); 
          /// \ru Удалить все объекты модели. \en Delete all the model objects. 
          void    DeleteItems(); 

          /// \ru Разрушить сборки с подсборками на составляющие. \en Decompose assemblies with subassemblies into components. 
          bool    DecomposeAssemblies();

          /** \brief \ru Наполнить присланную модель полигональными копиями объектов модели.
                     \en Fill the given model with polygonal copies of the model objects. \~
            \details \ru Наполнить присланную модель полигональными копиями объектов оригинальной модели.
              Присланная модель опустошается и наполняется полигональными копиями объектов оригинальной модели.
              Присланная модель заполняется аналогично оригинальной модели с той разницей, что вместо 
              тел, проволочных каркасов, точечных каркасов и других конечных объектов модели 
              присланную модель заполняют соответствующие полигональные копии объектов (MbMesh).
              Сборки и вставки в присланной модели сохраняются аналогичными оригинальной модели.
              Присланная модель может использоваться для визуализации модели и расчетов.
                     \en Fill the given model with polygonal copies of the original model objects.
              Given model is being cleared and filled with polygonal copies of objects of the original model.
              Given model is to be filled similarly to original model, but instead of 
              solids, wire-frames, point-frames and other finite objects of model 
              the given model is filled by the corresponding polygonal copies of objects (MbMesh).
              Assemblies and instances in the given model remains similar to the original model.
              Given model can be used for calculations and visualization of the model. \~
            \note \ru В многопоточном режиме выполняется параллельно. \en In multithreaded mode runs in parallel. \~

            \param[in] stepData - \ru Данные для вычисления шага при триангуляции.
                                  \en Data for step calculation during  triangulation. \~
            \param[in] note     - \ru Способ построения полигонального объекта.
                                  \en Way for polygonal object constructing. \~
            \param[out] meshModel - \ru Присланная модель для наполнения.
                                    \en Given model to be filled. \~
            \return \ru Не было ошибок во время построения - true, были ошибки - false.
                    \en If there were no errors during construction, then true, otherwise false. \~
          */
          bool    FillMeshModel( const MbStepData & stepData, const MbFormNote & note, MbModel & meshModel ) const; 

          /** \brief \ru Добавить полигональный объект.
                     \en Add polygonal object. \~
            \details \ru Добавить полигональную копию модели в присланный полигональный объект (MbMesh).
              Все объекты модели, её сборки и вставки помещаются в единый плоскогранный полигональный объект.
              Один и тот же объект, вставленный несколько раз в сборки и вставки модели, получает несколько копий,
              так как каждая копия трансформируется по матрице локальной системы координат cjjndtncnde.otq сборки и вставки.
                     \en Add polygonal copy of the model to the given polygonal object (MbMesh).
              All the objects, assemblies and instances of the model are placed in a unified planar polygonal object.
              The same object inserted several times in assemblies and instances of the model gets several copies
              because each copy is transformed by the matrix of local coordinate system of the corresponding assembly and instance. \~
            \param[in] stepData - \ru Данные для вычисления шага при триангуляции.
                                  \en Data for step calculation during triangulation. \~
            \param[in] note     - \ru Способ построения полигонального объекта.
                                  \en Way for polygonal object constructing. \~
            \param[out] mesh - \ru Присланный полигональный объект.
                               \en Given polygonal object. \~
            \return \ru Добавлен ли объект.
                    \en Whether the object is added. \~
          */
          bool    AddYourMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const; 

          /** \brief \ru Разрезать модель полигональных объектов одной или двумя параллельными плоскостями.
                     \en Cut model of polygonal objects by one or two parallel planes. \~
            \details \ru Создать новую модель полигональных объектов и наполнить её частями исходной модели, 
              лежащими под плоскостью XY локальной системы координат на заданном расстоянии.\n
              Функция "режет" только модель полигональных объектов MbMesh.
              Функция "режет" модель двумя плоскостями: 
              плоскостью XY локальной системы координат place и плоскостью, параллельной ей и 
              расположенной на расстоянии distance ниже неё.
              Если distance<=0, то функция "режет" объект только одной плоскостью XY локальной системы.\n
              Содержимое исходных полигональных объектов, 
              необходимое для построения разрезанного объекта и не затронутое режущими плоскостями,
              добавляется в возвращаемый разрезанный объект без копирования.\n
                     \en Create new model of polygonal objects and fill it by the source model parts 
              lying under XY plane of the local coordinate system at the given distance.\n
              Function "cuts" only MbMesh model of polygonal objects.
              Function "cuts" the model by two planes: 
              XY plane of 'place' local coordinate system and plane parallel to it and 
              located at 'distance' distance below it.
              If 'distance' is less than or equal to zero, then the function "cuts" an object only by one XY plane of local coordinate system.\n
              Contents of the source polygonal objects 
              that are necessary for creation of cut object and not affected by cutting planes
              are added to returned cut object without copying.\n \~
            \param[in] place - \ru Локальная система координат, плоскость XY которой задаёт режущую плоскость.
                               \en A local coordinate system which XY plane defines a cutting plane. \~
            \param[in] distance - \ru Расстояние до параллельной режущей плоскости откладывается в отрицательную сторону оси Z локальной системы.
                                  \en Distance to a parallel cutting plane is measured in negative direction of Z-axis of local coordinate system. \~
            \param[in] names    - \ru Именователь.
                                  \en An object defining names generation in the operation. \~
            \result \ru Возвращает новую модель полигональных объектов, лежащую под плоскость XY локальной системы координат на заданном расстоянии.
                    \en Returns a new model of polygonal objects that lies under XY plane of local coordinate system at given distance. \~
          */
          MbModel * CutMeshModel( const MbPlacement3D & cutPlace, double distance, const MbSNameMaker * names = nullptr ) const;

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
            \note \ru В многопоточном режиме выполняется параллельно. \en In multithreaded mode runs in parallel. \~
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
            \param[out] find - \ru Найденный объект.
                               \en Found object. \~
            \param[out] findName - \ru Имя найденного объекта.
                                   \en Name of the found object. \~
            \param[out] element - \ru Найденный составляющий элемент объекта.
                                  \en Found component of the object. \~
            \param[out] elementName - \ru Имя найденного составляющего элемента объекта.
                                      \en Name of found component of the object. \~
            \param[out] path - \ru Путь к объекту в модели.
                               \en Path to the object in model. \~
            \param[out] from - \ru Матрица преобразования найденного объекта в глобальную систему координат.
                               \en Transformation matrix of the found object to the global coordinate system. \~
            \return \ru Найден ли объект или его имя.
                    \en Whether the object or its name is found. \~
          */
          bool    NearestMesh( MbeSpaceType sType, MbeTopologyType tType, MbePlaneType pType,
                               const MbAxis3D & axis, double maxDistance, bool gridPriority,
                               MbItem *& find, SimpleName & findName,
                               MbRefItem *& element, SimpleName & elementName,
                               MbPath & path, MbMatrix3D & from ) const; 

  /** \brief \ru Дать все объекты указанного типа.
             \en Get all objects by the type. \~
    \details \ru Дать все объекты указанного типа, а также матрицы преобразования их в глобальную систему координат. \n
             \en Get all objects by the type along with transformation matrices to the global coordinate system. \n \~
    \param[in] type - \ru Тип объекта.
                      \en Object's type. \~
    \param[out] items - \ru Множество найденных объектов.
                        \en Found objects. \~
    \param[out] matrs - \ru Матрицы преобразования найденных объектов в глобальную систему координат.
                        \en Transformation matrices of found objects to the global coordinate system. \~
    \ingroup Model_Items
  */
   virtual void GetItems( MbeSpaceType type, std::vector<SPtr<MbItem>> & items, std::vector<MbMatrix3D> & matrs );

  /** \brief \ru Дать все объекты указанного типа.
             \en Get all objects by type. \~
    \details \ru Дать все объекты указанного типа, 
      а также матрицы преобразования их в глобальную систему координат. \n
             \en Get all objects by type 
      and get transformation matrix to the global coordinate system. \n \~
    \param[in] type - \ru Тип объекта.
                      \en Object's type. \~
    \param[out] items - \ru Множество найденных объектов.
                        \en Found objects. \~
    \param[out] matrs - \ru Матрицы преобразования найденных объектов в глобальную систему координат.
                        \en Transformation matrix of found objects to the global coordinate system. \~
    \ingroup Model_Items
  */
  virtual void    GetItems( MbeSpaceType type, RPArray<MbItem> & items, SArray<MbMatrix3D> & matrs ); 

  /** \brief \ru Дать все объекты указанного типа.
             \en Get all objects by type. \~
    \details \ru Дать все объекты указанного типа, 
      а также матрицы преобразования их в глобальную систему координат. \n
             \en Get all objects by type 
      and get transformation matrix to the global coordinate system. \n \~
    \param[in] type - \ru Тип объекта.
                      \en Object's type. \~
    \param[out] items - \ru Множество найденных объектов.
                        \en Found objects. \~
    \param[out] matrs - \ru Матрицы преобразования найденных объектов в глобальную систему координат.
                        \en Transformation matrix of found objects to the global coordinate system. \~
    \ingroup Model_Items
  */
  virtual void    GetItems( MbeSpaceType type, RPArray<const MbItem> & items, SArray<MbMatrix3D> & matrs ) const; 

  /** \brief \ru Дать все уникальные объекты указанного типа.
             \en Get all unique objects by type. \~
    \details \ru Дать все уникальные объекты указанного типа. \n
             \en Get all unique objects by type. \n \~
    \param[in] type - \ru Тип объекта.
                      \en Object's type. \~
    \param[out] items - \ru Множество найденных объектов.
                        \en Found objects. \~
    \ingroup Model_Items
  */
  virtual void    GetUniqItems( MbeSpaceType type, CSSArray<const MbItem *> & items ) const;  

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
            \param[out] path - \ru Путь к объекту в модели.
                               \en Path to the object in model. \~
            \param[out] from - \ru Матрица преобразования объекта в глобальную систему координат.
                               \en Transformation matrix of object to the global coordinate system. \~
            \return \ru Найден ли путь и матрица объекта.
                    \en Whether the path and the matrix of object are found. \~
          */
          bool    MakePath( const MbItem & obj, MbPath & path, MbMatrix3D & from ) const;

          /** \brief \ru Дать объект по его пути.
                     \en Get the object by its path. \~
            \details \ru Дать объект по его пути положения в модели и 
              дать матрицу преобразования объекта в глобальную систему координат.
              Объект может содержаться в другом объекте (в сборке или вставке).
                     \en Get the object by path of its position in the model and 
              get transformation matrix of the object to the global coordinate system.
              Object can be contained in other object (in assembly or in instance). \~
            \param[in] path - \ru Путь к объекту в модели.
                              \en Path to object in the model. \~
            \param[out] from - \ru Матрица преобразования объекта в глобальную систему координат.
                               \en Transformation matrix of object to the global coordinate system. \~
            \return \ru Найден ли путь и матрица объекта.
                    \en Whether the path and the matrix of object are found. \~
          */
          const MbItem * GetItemByPath( const MbPath & path, MbMatrix3D & from ) const; 

          /** \brief \ru Найти объект по геометрическому объекту.
                     \en Find object by geometric object. \~
            \details \ru Найти объект по геометрическому объекту, а также получить путь к 
                         объекту в модели и матрицу преобразования в глобальную систему координат.
                     \en Find object by geometric object and also get the path to the 
                         object in model and get transformation matrix to the global coordinate system. \~
            \param[in] s - \ru Геометрический объект.
                           \en Geometric object. \~
            \param[out] path - \ru Путь к объекту в модели.
                               \en Path to object in the model. \~
            \param[out] from - \ru Матрица преобразования объекта в глобальную систему координат.
                               \en Transformation matrix of object to the global coordinate system. \~
            \return \ru Найден ли путь и матрица объекта.
                    \en Whether the path and the matrix of object are found. \~
          */
  const   MbItem * FindItem( const MbSpaceItem * s, MbPath & path, MbMatrix3D & from ) const; 

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
          */
  const   MbItem * FindItem( const MbPlaneItem * s, MbPath & path, MbMatrix3D & from ) const; 

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
          */
  const   MbItem * FindItem( const MbItem * s, MbPath & path, MbMatrix3D & from ) const; 

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
          */
  const   MbItem * GetItemByName( SimpleName n, MbPath & path, MbMatrix3D & from ) const; 

          /** \brief \ru Найти объект по имени для редактирования.
                     \en Find object by name for editing. \~
            \details \ru Найти объект по имени для редактирования, а также получить путь к объекту в модели 
              и матрицу преобразования в глобальную систему координат. \n
                     \en Find object by name for editing and also get path to object in model 
              and get transformation matrix to the global coordinate system. \n \~
            \param[in] n - \ru Имя объекта.
                           \en A name of an object. \~
            \param[out] path - \ru Путь к объекту в модели.
                               \en Path to object in the model. \~
            \param[out] from - \ru Матрица преобразования объекта в глобальную систему координат.
                               \en Transformation matrix of object to the global coordinate system. \~
            \return \ru Найден ли путь и матрица объекта.
                    \en Whether the path and the matrix of object are found. \~
          */
          MbItem * SetItemByName( SimpleName n, MbPath & path, MbMatrix3D & from );       
          /** \brief \ru Алгоритм общего назначения для обхода дерева модели в глубину.
                     \en General-purpose algorithm traversing the model graph in depth. */
          void    Traverse( ItModelVisitor & ) const;
          /// \ru Преобразовать селектирование объекты по матрице. \en Transform selected objects by matrix. 
          void    TransformSelected( const MbMatrix3D &, MbRegTransform * = nullptr );
          /// \ru Сдвинуть выбранные объекты. \en Move selected objects. 
          void    MoveSelected( const MbVector3D &, MbRegTransform * = nullptr );
          /// \ru Повернуть выбранные объекты вокруг оси. \en Rotate selected objects around an axis. 
          void    RotateSelected( const MbAxis3D &, double angle, MbRegTransform * = nullptr );

          /** \brief \ru Отцепить все выбранные объекты.
                     \en Detach all selected objects. \~
            \details \ru Отцепить все выбранные объекты модели, в том числе и 
              содержащиеся в сложных составных объектах, таких как сборка. \n
                     \en Detach all selected objects of model including ones 
              contained in complex composite objects such as assembly. \n \~
            \param[out] items - \ru Отцепленные объекты.
                                \en Detach the objects. \~
            \param[out] matrs - \ru Матрицы преобразования отцепленных объектов в глобальную систему координат.
                                \en Transformation matrices of detached objects to global coordinate system. \~
          */
          void    DetachSelected( RPArray<MbItem> & , SArray<MbMatrix3D> & , bool selected, bool resetName = true ); 
          /// \ru Отцепить все видимые или невидимые объекты. \en Detach all visible or invisible objects. \~
          void    DetachInvisible( RPArray<MbItem> & , SArray<MbMatrix3D> & , bool invisible, bool resetName = true );

          /// \ru Выдать количество объектов модели. \en Get the count of objects of model. 
          size_t  ItemsCount()    const { return modelItems.size(); } 

          /// \ru Содержится ли объект в модели? \en Whether the object is contained in model. 
          bool    ContainsItem( const MbItem * ) const;
          /// \ru Добавить в модель объекты другой модели. \en Add objects of other model to the model.  
          bool    AddModelItems( const MbModel & );
          /// \ru Добавить в массив выбранные объекты модели без поиска в сложных составных объектах. \en Add selected objects of model to array without search in complex composite objects. 
          size_t  GetSelected( RPArray<MbItem> & ) const; 

          /// \ru Вычислить габарит по всем объектам модели. \en Calculate bounding box for all the objects of model. 
          void    CalculateGabarit( MbCube & ) const; 

  private:
          // Отдать все объекты с указанным свойством. 
          void    DetachByAttribute( RPArray<MbItem> & items, SArray<MbMatrix3D> & matrs, int attribute, bool resetName );

public:
  /// \ru Простой итератор по объектам модели. \en Simple iterator on objects of model. 
  class ItemIterator {
  private:
    NameItemArray::iterator currIter;

  private:
    ItemIterator( const NameItemArray::iterator & iter ) : currIter( iter ) {}

  public:
    MbItem * operator *  () { return currIter->second; }
    MbItem * operator -> () { return currIter->second; }
    ItemIterator & operator ++ () { ++currIter; return *this; }
    ItemIterator   operator ++ ( int ) { return ItemIterator(currIter++); }

    bool operator == ( const ItemIterator & other) const { return currIter == other.currIter; }
    bool operator != ( const ItemIterator & other) const { return currIter != other.currIter; }  

    friend class MbModel;
  }; // ItemIterator


  /// \ru Константный итератор по объектам модели. \en Constant iterator on objects of the model. 
  class ItemConstIterator {
  private:
    NameItemArray::const_iterator currIter;

  private:
    ItemConstIterator( const NameItemArray::const_iterator & iter ) : currIter( iter ) {}

//   public:
//     ItemConstIterator( const ItemIterator& iIter ) : currIter( iIter.currIter ) {}

  public:
    const MbItem * operator *  () const  { return currIter->second; }
    const MbItem * operator -> () const { return currIter->second; }
    ItemConstIterator & operator ++ () { ++currIter; return *this; }
    ItemConstIterator   operator ++ ( int ) { return ItemConstIterator(currIter++); }

    bool operator == ( const ItemConstIterator & other) const { return currIter == other.currIter; }
    bool operator != ( const ItemConstIterator & other) const { return currIter != other.currIter; }  

    friend class MbModel;
  }; // ItemConstIterator

public:
  /// \ru Выдать константный итератор по всем объектам с указанием на начало. \en Get constant iterator on all objects pointing to the first element.  
  ItemConstIterator  CBegin() const { return ItemConstIterator( modelItems.begin() ); }
  /// \ru Выдать константный итератор по всем объектам с указанием за конец. \en Get constant iterator on all objects pointing to the past-the-end element.  
  ItemConstIterator  CEnd()   const { return ItemConstIterator( modelItems.end() ); }
  /// \ru Выдать константный итератор для указанного имени с указанием на начало. \en Get constant iterator by the given name pointing to the first element.  
  ItemConstIterator  CBegin( SimpleName n ) const { return ItemConstIterator( modelItems.lower_bound(n) ); }
  /// \ru Выдать константный итератор для указанного имени с указанием на конец. \en Get constant iterator by the given name pointing to the past-the-end element.  
  ItemConstIterator  CEnd( SimpleName n )   const { return ItemConstIterator( modelItems.upper_bound(n) ); }
  /// \ru Выдать не константный итератор по всем объектам с указанием на начало. \en Get non-constant iterator on all the objects pointing to the first element.  
  ItemIterator       Begin() { return ItemIterator( modelItems.begin() ); }
  /// \ru Выдать не константный итератор по всем объектам с указанием за конец. \en Get non-constant iterator on all the objects pointing to the past-the-end element.  
  ItemIterator       End() { return ItemIterator( modelItems.end() ); }
  /// \ru Выдать не константный итератор для указанного имени с указанием на начало. \en Get non-constant iterator by the given name pointing to the first element.  
  ItemIterator       Begin( SimpleName n ) { return ItemIterator( modelItems.lower_bound(n) ); }
  /// \ru Выдать не константный итератор для указанного имени с указанием на конец. \en Get non-constant iterator by the given name pointing to the past-the-end element.  
  ItemIterator       End( SimpleName n ) { return ItemIterator( modelItems.upper_bound(n) ); }

private:
  void  CreateItemsMeshes( const MbStepData & stepData, const MbFormNote & note, MbRegDuplicate * ) const; 

private: // \ru Закрытые методы. // \en Internal use methods.
  /// \ru Выдать объект модели по индексу. \en Get an object of model by an index. 
  const   MbItem *  GetModelItem( size_t i ) const;
  // \ru Выдать имя последнего объекта в контейнере. \en Get name of the last object in container. 
  SimpleName _LastItemName() const { return modelItems.empty() ? 0 : modelItems.rbegin()->first; }
  // \ru Выдать имя для следующего за последним объекта. \en Get name of past-the-end object (next object to the last one). 
  SimpleName _NextItemName() const;
  // \ru Генерация имени для нового элемента. \en Generate identifier for a new item.
  SimpleName _NewItemName( SimpleName & startName ) const;  

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbModel )
OBVIOUS_PRIVATE_COPY( MbModel )
}; // MbModel

IMPL_PERSISTENT_OPS( MbModel )


//----------------------------------------------------------------------------------------
// \ru Дать все объекты. \en Get all the objects.  
// ---
template <class Items>
void MbModel::GetItems( Items & items ) const
{
  items.reserve( modelItems.size() );
  for ( NameItemArray::const_iterator iter = modelItems.begin(); iter != modelItems.end(); ++iter ) {
    if ( iter->second != nullptr )
      items.push_back( iter->second );
  }
}


//----------------------------------------------------------------------------------------
// \ru Отцепить все объекты. \en Detach all the objects. 
// ---
template <class Items>
void MbModel::DetachItems( Items & items )
{
  items.reserve( modelItems.size() );
  NameItemArray::iterator iter = modelItems.begin();
  NameItemArray::const_iterator endItem = modelItems.end();
  for ( ; iter != endItem; ++iter ) {
    MbItem * item = iter->second;
    if ( item != nullptr ) {
      item->DecRef();
      items.push_back( item );
    }
  }
  modelItems.clear();
  AttributesChange();
}


//----------------------------------------------------------------------------------------
/** \brief \ru Чтение модели MbModel из потока #reader.
           \en Read MbModel model from #reader stream. \~
  \details \ru Чтение модели MbModel из потока #reader. \n
           \en Read MbModel model from #reader stream. \n \~
  \ingroup Model
*/
// ---
MATH_FUNC (bool) ReadModelItems( reader &, MbModel & );


//----------------------------------------------------------------------------------------
/** \brief \ru Чтение из потока объектов, определенных в данном дереве модели.
           \en Read items, defined in the given model tree, from a stream.
  \details \ru Чтение из потока всех объектов, определенных в данном дереве модели, включая его корень (корни).
           \en Read items, defined in the given model tree including its root(s), from a stream.
  \param[in]  in    - \ru Поток для чтения. \en Stream to read from. \~
  \param[in]  tree  - \ru Дерево модели. \en Model tree. \~
  \param[out] items - \ru Прочитанные объекты. \en Read objects. \~
*/ 
// ---
MATH_FUNC (void) ReadModelItemsFromTree( reader & in, const c3d::IModelTree * tree, std::vector< SPtr<MbItem> > & items );


//----------------------------------------------------------------------------------------
/** \brief \ru Чтение из потока объектов, определенных в поддереве с корнем в данном узле.
           \en Read items, defined in a subtree with a root at the given node, from a stream.
  \details \ru Чтение из потока объектов, определенных в поддереве с корнем в данном узле, исключая сам узел.
           Если определено флагом (addAttr == true), то из объекта, определенного заданным узлом, читаются атрибуты и добавляются в модель.
           \en Read items, defined in a subtree with a root at the given node, from a stream, excluding the node itself.
           If defined by the flag (addAttr == true), attributes are read from an object defined by the given node and added to the model.
  \param[in]  in      - \ru Поток для чтения. \en Stream to read from. \~
  \param[in]  node    - \ru Узел дерева модели. \en Node of Model tree. \~
  \param[out] model   - \ru Модель, куда добавлять прочитанные объекты. \en Model where to add read objects. \~
  \param[in]  addAttr - \ru Флаг чтения атрибутов. \en Attributes read flag. \~
*/
// ---
MATH_FUNC (void) ReadModelItemsFromTree( reader & in, const c3d::IModelTreeNode * node, MbModel & model, bool addAttr = false );


//----------------------------------------------------------------------------------------
/** \brief \ru Запись модели MbModel в поток #writer.
           \en Write MbModel model to #writer stream. \~
  \details \ru Запись модели MbModel в поток #writer. \n
           \en Write MbModel model to #writer stream. \n \~
  \ingroup Model
*/
// ---
MATH_FUNC (void) WriteModelItems( writer &, const MbModel & );


#endif // __MODEL_ENTITY_H
