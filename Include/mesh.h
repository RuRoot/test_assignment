////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Полигональный геометрический объект (фасетный объект).
         \en The polygonal geometric object - Mesh. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MESH_H
#define __MESH_H


#include <mesh_grid.h>
#include <mesh_polygon.h>
#include <model_item.h>


class  MbMesh;
class  MapPolymeshToMbMesh;
class            MbMeshTopology;

namespace c3d // namespace C3D
{
typedef SPtr<MbMesh>                          MeshSPtr;
typedef SPtr<const MbMesh>                    ConstMeshSPtr;

typedef std::vector<MbMesh *>                 MeshesVector;
typedef std::vector<const MbMesh *>           ConstMeshesVector;

typedef std::vector<MeshSPtr>                 MeshesSPtrVector;
typedef std::vector<ConstMeshSPtr>            ConstMeshesSPtrVector;
typedef DPtr<MbMeshTopology>                  MeshTopologyPtr;
} // namespace C3D


//------------------------------------------------------------------------------
/** \brief \ru Полигональный объект модели - фасетный объект.
           \en The polygonal geometric object - Mesh. \~
  \details \ru Фасетный объект - это объект геометрической модели, наследник MbItem, являющийся
  множеством примитивов #MbPrimitive, аппроксимирующих некоторый 
  геометрический объект для ускорения визуализации, вычисления инерционных 
  характеристик, определения столкновений и других расчетов.\n

    Например, сетку можно построить на основе множества точек, полученных замерами 
  реального физического тела. Полигональный объект содержит множество точек, являющимися
  узлами (вершинами) в таких структурах данных:
    множество указателей на триангуляции MbGrid (наборы стыкующихся треугольных и четырёхугольных пластин),
    множество указателей на полигоны MbPolygon3D (наборы точек, описывающих ломаные линии), 
    множество указателей на апексы MbApex3D (точки, описывающие положение вершин или объектов-точек).
           \en Mesh is an object of geometric model (subclass MbItem) which is
  the set of primitives #MbPrimitive which approximate some 
  geometric object for speed up rendering, calculation of inertial 
  characteristics, collision detection and other calculations.\n

    For example, the mesh can be create on the basis of a point set obtained measurements 
  of the real physical solid. Polygonal object contains a set of points which are
  nodes (vertices) in the data structures:
    a set of pointers to triangulations MbGrid (sets of mating triangular and quadrangular plates),
    a set of pointers to polygons MbPolygon3D (sets of points which describe the polylines), 
    a set of pointers to apexes MbApex3D (points wich describe the position of vertices or objects-points). \~ 

  \par \ru Применение
    Полигональный объект используется для представления геометрических объектов в упрощенном виде, \n
    для визуализации геометрических объектов, \n 
    для расчетов столкновений геометрических объектов, \n 
    для вычисления масс-инерционных характеристик. \n 
    Если фасетный объект аппроксимирует тело #MbSolid, то фасетный объект, как тело, может быть
    замкнутым или незамкнутым.\n
    Граничные точки разных триангуляций для замкнутого объекта совпадают, но имеют в них разные нормали. 

       \en Usage
    The mesh is used to represent geometric objects in a polygonal form, \n
    for visualization of geometric objects, \n 
    for calculations of geometry objects collisions, \n 
    for calculation of the mass-inertial properties. \n 
    If the mesh approximates the solid #MbSolid then polygonal object as a solid can be
    closed or unclosed. \n \~ 
  \ingroup Model_Items
*/ 
// ---
class MATH_CLASS MbMesh : public MbItem 
{
private:
  RPArray<MbGrid>      _grids;  ///< \ru Множество указателей на триангуляции. \en A set of pointers to triangulations. 
  RPArray<MbPolygon3D> _wires;  ///< \ru Множество указателей на полигоны. \en A set of pointers to polygons. 
  RPArray<MbApex3D>    _peaks;  ///< \ru Множество указателей на апексы. \en A set of pointers to apexes. 
  const MbRefItem *    _item;   ///< \ru Источник сетки. \en Source of mesh. 
  MbeSpaceType         _type;   ///< \ru Тип сетки отражает характер, но не связан напрямую с item. \en A mesh type describes the character but it is not related to "item". 
  bool                 _exact;  ///< \ru Объекты построены на числах double. \en Objects builded on double data. 

  /** \brief \ru Маппинг между MbMesh и MbPolymesh.
             \en Mapping between MbMesh and MbPolymesh. \~
    \details \ru Маппинг между составляющими объекта MbMesh и объекта MbPolymesh, по которому был создан этот MbMesh.
             \en Mapping between parts of the MbPolymesh object and MbMesh object constructed by the polymesh. \n \~
  */
  std::unique_ptr<MapPolymeshToMbMesh> _mapPolymesh;

  /** \brief \ru Замкнутость объекта. 
             \en Closeness of object. \~
    \details \ru Замкнутость указывает на отсутствие края в триангуляции. Значение флага может быть пересчитано по запросу.
             \en Closeness indicates the absence of edge in the triangulation. The flag value can be recalculated by request. \n \~
  */
  mutable bool         _closed;

  /** \brief \ru Габаритный куб объекта. 
             \en Bounding box of object. \~
      \details \ru Габаритный куб объекта расчитывается только при запросе габарита объекта. Габаритный куб в конструкторе объекта и после модификации объекта принимает неопределенное значение.
               \en Bounding box of object is calculated only at the request. Bounding box of object is undefined after object constructor and after object modifications \n \~
  */
  mutable MbCube       _cube; 
  mutable c3d::MeshTopologyPtr _topo; ///< \ru Дополнительная информация о топологии полигонального объекта. \en Additional information about a topology of the polygonal object.
private:
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en The copy constructor without implementation prevents from copying by default. 
  MbMesh( const MbMesh & );
  /// \ru Конструктор копирования с регистратором. \en Copy constructor with registrator. 
  explicit MbMesh( const MbMesh & other, MbRegDuplicate * iReg );

public:
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  explicit MbMesh( bool _exact = false, const MbCreator * = nullptr );
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  explicit MbMesh( bool _exact, const  RPArray<MbCreator> & );
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  explicit MbMesh( bool _exact, const  c3d::CreatorsSPtrVector & );
  /// \ru Деструктор \en Destructor 
  virtual ~MbMesh();

public: 
  VISITING_CLASS( MbMesh ); 
  
  // \ru Общие функции геометрического объекта. \en Common functions of a geometric object. 

  MbeSpaceType  IsA() const override; // \ru Тип объекта. \en Type of the object. 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Создать копию. \en Create a copy.
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать согласно матрице. \en Transform according to the matrix.
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвинуть вдоль вектора. \en Translate along a vector.
  void    Rotate   ( const MbAxis3D   &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate around an axis.
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Являются ли объекты равными? \en Determine whether objects are equal. 
  bool    SetEqual ( const MbSpaceItem & init ) override;   // \ru Сделать объекты равным. \en Make equal objects. 
  double  DistanceToPoint ( const MbCartPoint3D & ) const override; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  void    AddYourGabaritTo( MbCube & r ) const override; // \ru Добавить свой габарит в куб. \en Add bounding box into a cube. 
  void    CalculateLocalGabarit( const MbMatrix3D & into, MbCube & cube ) const override; // \ru Рассчитать габарит в локальной системы координат. \en Calculate bounding box in the local coordinate system. 
  void    CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.

  MbProperty & CreateProperty( MbePrompt n ) const override;     // \ru Создать собственное свойство. \en Create own property. 
  void    GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & properties ) override; // \ru Установить свойства объекта. \en Set properties of object. 
  void    GetBasisItems( RPArray<MbSpaceItem> & ) override;   // \ru Выдать базовые объекты. \en Get the base objects. 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  bool    GetCreators( RPArray<MbCreator> & creators ) const override;       // \ru Добавить свои строители в присланный массив. \en Add your own creators to the given array.
  bool    GetCreators( c3d::CreatorsSPtrVector & creators ) const override;  // \ru Добавить свои строители в присланный массив. \en Add your own creators to the given array.

  // \ru Перестроить объект по журналу построения. \en Reconstruct object according to the history tree. 
  bool    RebuildItem( MbeCopyMode sameShell, RPArray<MbSpaceItem> * items, IProgressIndicator * progInd ) override;
  // \ru Создать полигональный объект - упрощенную копию данного объекта. \en Create a polygonal object - a polygonal copy of the given object. 
  MbItem* CreateMesh( const MbStepData & stepData, const MbFormNote & note, MbRegDuplicate * iReg ) const override; 
  // \ru Добавить себя в присланный полигональный объект mesh без копирования. \en Add itself to the given polygonal object "mesh" without copying. 
  bool    AddYourMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; 
  // \ru Разрезать полигональный объект одной или двумя параллельными плоскостями. \en Cut polygonal form of an object by one or two parallel planes. 
  MbItem* CutMesh( const MbPlacement3D & cutPlace, double distance, const MbSNameMaker * = nullptr ) const override;
  // \ru Найти ближайший объект или имя ближайшего объекта. \en Find the nearest object or name of nearest object.
  // \note \ru В многопоточном режиме выполняется параллельно. \en In multithreaded mode runs in parallel. \~
  bool    NearestMesh( MbeSpaceType sType, MbeTopologyType tType, MbePlaneType pType,
                               const MbAxis3D & axis, double maxDistance, bool gridPriority, double & t, double & dMin, 
                               MbItem *& find, SimpleName & findName,
                               MbRefItem *& element, SimpleName & elementName,
                               MbPath & path, MbMatrix3D & from ) const override;

  /** \ru \name Функции полигонального объекта.
      \en \name Function of polygonal object.
      \{ */
          /// \ru Является ли полигональный объект аппроксимацией точки? \en Whether the polygonal object is approximation of point. 
          bool    IsAPointMesh  () const; 
          /// \ru Является ли полигональный объект аппроксимацией кривой? \en Whether the polygonal object is approximation of curve. 
          bool    IsACurveMesh  () const;
          /// \ru Является ли полигональный объект аппроксимацией поверхности? \en Whether the polygonal object is approximation of surface. 
          bool    IsASurfaceMesh() const;
          /// \ru Является ли полигональный объект аппроксимацией твёрдого тела? \en Whether the polygonal object is approximation of solid. 
          bool    IsASolidMesh  () const;
          /// \ru Содержат ли контейнеры сетки данные? \en Whether containers of mesh contain data. 
          bool    IsComplete() const { return (_grids.size() > 0) || (_wires.size() > 0) || (_peaks.size() > 0); }
          /// \ru Зарезервировать место для пластин. \en Reserve space for the plates. 
          void    GridsReserve(size_t cnt, bool fill = false );
          /// \ru Зарезервировать место для полигонов. \en Reserve space for the polygons. 
          void    PolygonsReserve( size_t cnt ) { _wires.Reserve( cnt ); }
          /// \ru Зарезервировать место для апексов. \en Reserve space for the apexes. 
          void    ApexReserve( size_t cnt )     { _peaks.Reserve( cnt ); }
          /// \ru Обнулить данные объекта. \en Set object data to null. 
          void    Flush(); 
          /// \ru Освободить лишнюю память. \en Free the unnecessary memory. 
          void    Adjust() { _grids.Adjust(); _wires.Adjust(); _peaks.Adjust(); }

          /// \ru Выдать количество триангуляций. \en Get the number of triangulations. 
          size_t  GridsCount() const { return _grids.size(); }
          /// \ru Добавить триангуляцию. \en Add triangulation. 
          void    AddGrid( MbGrid & gr ); 
          /// \ru Добавить новую пустую триангуляцию и выдать её для заполнения. \en Add new empty triangulation and give it to fill. 
          MbGrid * AddGrid(); 
          /// \ru Отсоединить триангуляцию с заданным номером. \en Detach triangulation with a given number. 
          MbGrid * DetachGrid( size_t i ); 
          /// \ru Отсоединить все триангуляции. \en Detach all triangulations. 
          template <class GridsVector>
          void    DetachAllGrids( GridsVector & gridsVector )
          {
            gridsVector.reserve( gridsVector.size() + _grids.size() );
            for( size_t i = 0, iCount = _grids.size(); i < iCount; ++i ) {
              MbGrid * gr = _grids[i];
              if ( gr != nullptr ) {
                gr->DecRef();
                gridsVector.push_back( gr );
              }
            }
            _grids.clear();
            _grids.shrink_to_fit();
            ClearAuxData();
          } 
          /// \ru Вернуть указатель на триангуляцию по её номеру. \en Return pointer to triangulation by it number. 
  const   MbGrid * GetGrid( size_t i ) const { return ( (i < _grids.size()) ? _grids[i]: nullptr ); }
          /// \ru Вернуть указатель на триангуляцию по её номеру для модификации. \en Return the pointer to triangulation by its number to be modified. 
          MbGrid * SetGrid( size_t i )       { return ( (i < _grids.size()) ? _grids[i]: nullptr ); }
          /// \ru Получить указатели на триангуляции. \en Get pointers to triangulations.
          template <class GridsVector>
          void    GetGrids( GridsVector & gridsVector ) const
          {
            size_t gridsCnt = _grids.size();
            gridsVector.reserve( gridsVector.size() + gridsCnt );
            for ( size_t k = 0; k < gridsCnt; ++k ) {
              const MbGrid * grid = _grids[k];
              gridsVector.push_back( grid );
            }
          }
          /// \ru Получить все точки триангуляции. \en Get all triangulations points.
          template <class Point, class PointsVector>
          void GetGridsPoints( PointsVector & points ) const
          {
            size_t addPointsCnt = 0;

            size_t k, gridsCnt = _grids.size();
            
            for ( k = 0; k < gridsCnt; ++k ) {
              const MbGrid * grid = _grids[k];
              if ( grid != nullptr )
                addPointsCnt += grid->PointsCount();
            }
            points.reserve( points.size() + addPointsCnt );

            for ( k = 0; k < gridsCnt; ++k ) {
              const MbGrid * grid = _grids[k];
              if ( grid != nullptr )
                grid->GetPoints<Point>( points );
            }
          }
          /// \ru Выдать количество полигонов. \en Get the number of polygons. 
          size_t  PolygonsCount() const { return _wires.size(); }
          /// \ru Добавить полигон. \en Add polygon. 
          void    AddPolygon( MbPolygon3D & );
          /// \ru Добавить новый пустой полигон и выдать его для заполнения. \en Add new empty polygon and give it to fill. 
          MbPolygon3D * AddPolygon();
          /// \ru Отсоединить полигон с заданным номером. \en Detach polygon with a given number. 
          MbPolygon3D * DetachPolygon( size_t i ); 
          /// \ru Отсоединить все полигоны. \en Detach all polygons. 
          template <class PolygonsVector>
          void    DetachAllPolygons( PolygonsVector & polyVector ) {
            polyVector.reserve( polyVector.size() + _wires.size() );
            for( size_t i = 0, iCount = _wires.size(); i < iCount; ++i ) {
              MbPolygon3D * pl = _wires[i];
              if ( pl != nullptr ) {
                pl->DecRef();
                polyVector.push_back( pl );
              }
            }
            _wires.clear();
            _wires.shrink_to_fit();
            _cube.SetEmpty();
          }
          /// \ru Вернуть указатель на полигон по его номеру. \en Return the pointer to polygon by its number. 
  const   MbPolygon3D * GetPolygon( size_t i ) const { return ( (i < _wires.size()) ? _wires[i]: nullptr ); }
          /// \ru Вернуть указатель на полигон по его номеру. \en Return the pointer to polygon by its number. 
          MbPolygon3D * SetPolygon( size_t i )       { return ( (i < _wires.size()) ? _wires[i]: nullptr ); }
          /// \ru Получить указатели на полигоны. \en Get pointers to polygons.
          template <class PolygonsVector>
          void    GetPolygons( PolygonsVector & polyVector ) const {
            size_t polyCnt = _wires.size();
            polyVector.reserve( polyVector.size() + polyCnt );
            for ( size_t k = 0; k < polyCnt; ++k ) {
              const MbPolygon3D * poly = _wires[k];
              polyVector.push_back( poly );
            }
          }
          /// \ru Выдать количество апексов. \en Get the number of apexes. 
          size_t  ApexesCount() const { return _peaks.size(); }
          /// \ru Добавить новый апекс. \en Add new apex. 
          void    AddApex( MbApex3D & ap ); 
          /// \ru Добавить новый пустой апекс и выдать его для заполнения. \en Add new empty apex and give it to fill. 
          MbApex3D * AddApex();
          /// \ru Отсоединить апекс с заданным номером. \en Detach apex with a given number. 
          MbApex3D * DetachApex( size_t i ); 
          /// \ru Отсоединить все вершины. \en Detach all apexes. 
          template <class ApexesVector>
          void    DetachAllApexes( ApexesVector & peakVector ) {
            peakVector.reserve( peakVector.size() + _peaks.size() );
            for( size_t i = 0, iCount = _peaks.size(); i < iCount; ++i ) {
              MbApex3D * peak = _peaks[i];
              if ( peak != nullptr ) {
                peak->DecRef();
                peakVector.push_back( peak );
              }
            }
            _peaks.clear();
            _peaks.shrink_to_fit();
            _cube.SetEmpty();
          } 
          /// \ru Вернуть указатель на апекс по его номеру. \en Return the pointer to apex by its number. 
  const   MbApex3D * GetApex( size_t i ) const { return ( (i < _peaks.size()) ? _peaks[i]: nullptr ); }
          /// \ru Вернуть указатель на апекс по его номеру для модификации. \en Return the pointer to apex by its number to be modified. 
          MbApex3D * SetApex( size_t i )       { return ( (i < _peaks.size()) ? _peaks[i]: nullptr ); }
          /// \ru Получить указатели на вершины. \en Get pointers to apexes.
          template <class ApexesVector>
          void    GetApexes( ApexesVector & peakVector ) const {
            size_t peaksCnt = _peaks.size();
            peakVector.reserve( peakVector.size() + peaksCnt );
            for ( size_t k = 0; k < peaksCnt; ++k ) {
              const MbApex3D * peak = _peaks[k];
              peakVector.push_back( peak );
            }
          }

          // \ru Создать полигональный объект - упрощенную копию данного объекта. \en Create a polygonal object - a polygonal copy of the given object. 
          MbMesh *  CreateMeshByExistingItem( const MbRefItem & ) const; 

          /// \ru Инициализировать объект. \en Initialize an object. 
          void    InitMesh( const MbMesh & );
          /// \ru Добавить объекты сетки из присланной сетки. \en Add objects of mesh from a given mesh. 
          bool    AddMesh( const MbMesh &, bool checkSamePointers );

          /// \ru Вывернуть объект наизнанку - переориентировать все полигоны и триангуляции. \en Revert the object - reorient all polygons and grids. 
          bool    Reverse();
          /// \ru Вывернуть объект наизнанку - переориентировать заданную триангуляцию. \en Revert the object - reorient given grids. 
          bool    ReverseGrid( size_t gIndex );
          /// \ru Получить пространственный объект, для которого построен полигональный объект. \en Get a spatial object for which a polygonal object is constructed. 
  const   MbSpaceItem * SpaceItem() const { return ((_item != nullptr && _item->RefType() == rt_SpaceItem) ? (const MbSpaceItem *)_item : nullptr); }
          /// \ru Получить двумерный объект, для которого построен полигональный объект. \en Get a two-dimensional object for which a polygonal object is constructed. 
  const   MbPlaneItem * PlaneItem() const { return (( _item != nullptr && _item->RefType() == rt_PlaneItem) ? (const MbPlaneItem *)_item : nullptr); }
          /// \ru Получить объект геометрической модели, для которого построен полигональный объект. \en Get a model geometric object for which a polygonal object is constructed. 
  const   MbItem * Item() const
          {
            const MbItem * modelItem = nullptr;
            if ( _item != nullptr ) {
              MbeRefType refType = _item->RefType();
              if ( refType == rt_SpaceItem ) {
                if ( static_cast<const MbSpaceItem *>( _item)->Family() == st_Item )
                  modelItem = static_cast<const MbItem *>( _item);
              }
            }
            return modelItem;
          }
          /// \ru Получить объект, для которого построен полигональный объект. \en Get an object for which a polygonal object is constructed. 
  const   MbRefItem * GetRefItem() const { return _item; }
          /// \ru Запомнить объект, для которого построен полигональный объект. \en Remember an object for which a polygonal object is constructed. 
          void    SetRefItem( const MbRefItem * g ) { _item = g; }
          /// \ru Сбросить все запомненные объекты. \en Reset all reference objects. 
          void    ResetRefItems();
          /// \ru Установить тип полигонального объекта. \en Set a type of polygonal object. 
          void    SetMeshType( MbeSpaceType t ) { _type = t; }
          /// \ru Дать тип полигонального объекта. \en Get a type of polygonal object. 
          MbeSpaceType GetMeshType() const { return _type; }

          /// \ru Установить имя всем апексам. \en Set the name of all apexes. 
          void    SetApexName( SimpleName n );
          /// \ru Установить имя всем полигонам. \en Set the name of all polygons. 
          void    SetPolygonName( SimpleName n );
          /// \ru Установить имя всем триангуляциям. \en Set the name of all triangulations. 
          void    SetGridName( SimpleName n ); 

          /// \ru Установить имя всем апексам. \en Set the name of all apexes. 
          void    SetApexName( const MbName & n );
          /// \ru Установить имя всем полигонам. \en Set the name of all polygons. 
          void    SetPolygonName( const MbName & n );
          /// \ru Установить имя всем триангуляциям. \en Set the name of all triangulations. 
          void    SetGridName( const MbName & n );

          /// \ru Найти арекс по хешу имени. \en Find apex by name. 
  const   MbApex3D    * FindApexByName( const SimpleName h ) const; 
          /// \ru Найти полигон по имени. \en Find polygon by name. 
  const   MbPolygon3D * FindPolygonByName( const SimpleName h ) const; 
          /// \ru Найти триангуляцию по имени. \en Find grid by name. 
  const   MbGrid      * FindGridByName( const SimpleName h ) const;

          /// \ru Найти арекс по имени. \en Find apex by name. 
  const   MbApex3D    * FindApexByName( const MbName & n ) const;
          /// \ru Найти полигон по имени. \en Find polygon by name. 
  const   MbPolygon3D * FindPolygonByName( const MbName & n ) const;
          /// \ru Найти триангуляцию по имени. \en Find grid by name. 
  const   MbGrid      * FindGridByName( const MbName & n ) const;

          /// \ru Замкнутость объекта. \en Object closeness. 
          bool    IsClosed()          const { return _closed; }
          /// \ru Установить (не)замкнутость объекта. \en Set object (un-) closeness. 
          void    SetClosed( bool c ) const { _closed = c;    }
          /// \ru Определить (не)замкнутость объекта и установить флаг. \en Check object (un-) closeness and set the flag. 
          void    CheckClosed()       const;

          /// \ru Объекты на числах double. \en Objects on double data. 
          bool    IsExact() const { return _exact; }

          /** 
            \brief \ru Определить положение объекта относительно плоскости.
                   \en Define the object position relative to the plane. \~
            \details \ru Определить положение объекта относительно плоскости XY локальной системы координат.
                     \en Define the object position relative to the plane XY of a local coordinate system. \~
            \param[in] pl - \ru Локальная система координат, задающая плоскость.
                            \en A local coordinate system which defines a plane. \~
            \param[in] eps - \ru Метрическая точность.
                             \en A metric tolerance. \~
            \param[in] onlyInItem - \ru Интересует только положение объекта над плоскостью XY локальной системы координат.
                                    \en Whether the object position relative to the XY-plane of a local coordinate system is interested only. \~
            \return \ru iloc_OnItem - объект пересекает плоскость XY локальной системы координат,\n
                    iloc_InItem - объект расположен над плоскостью XY локальной системы координат,\n
                    iloc_OutOfItem - объект расположен под плоскостью XY локальной системы координат.
                    \en Iloc_OnItem - object intersects the XY-plane of a local coordinate system,\n
                    iloc_InItem - object is located over the XY plane of a local coordinate system,\n
                    iloc_OutOfItem - object is located under the XY plane of a local coordinate system. \~
          */
          MbeItemLocation GetLocation( const MbPlacement3D & pl, double eps, bool onlyInItem = false ) const; 

          /** 
            \brief \ru Определить положение объекта относительно трубы.
                   \en Define the object position relative to the tube. \~
            \details \ru Определить, расположен ли объект внутри трубы прямоугольного сечения, 
              заданного прямоугольником в плоскости XY локальной системы координат.
                     \en Define whether the object is inside the tube of rectangular section, 
              given by a rectangle in the XY plane of a local coordinate system. \~
            \param[in] place - \ru Локальная система координат, в в плоскости XY которой лежит сечение трубы.
                               \en A local coordinate system in the XY plane of which a tube section is located. \~
            \param[in] rect - \ru Прямоугольник, задающая сечение трубы.
                              \en A rectangle which defines a tube section. \~
            \param[in] eps - \ru Метрическая точность.
                             \en A metric tolerance. \~
            \param[in] onlyInItem - \ru Интересует только положение объекта внутри трубы.
                                    \en Whether the object position relative to the tube is interested only. \~
            \return \ru true, если объект расположен внутри трубы.
                    \en Returns true if the object is inside the tube. \~
          */
          bool    InsideLocation( const MbPlacement3D & place, MbRect & rect, double eps ) const; 

          /// \ru Перевести все объекты в треугольники и уравнять число точек и нормалей. \en Convert all objects to triangles and equalize the number of points and normals. 
          void    ConvertAllToTriangles();  
          /// \ru Общее количество всех треугольников. \en The total number of all (adjacent and nonadjacent) triangles. 
          size_t  AllTrianglesCount() const;   
          /// \ru Общее количество всех четырёхугольников. \en The total number of all (adjacent and nonadjacent) quadrangles. 
          size_t  AllQuadranglesCount() const; 
          /// \ru Общее количество всех точек триангуляций. \en The total number of all points of triangulations. 
          size_t  AllPointsCount() const; 
          /// \ru Общее количество всех нормалей триангуляций. \en The total number of all normals of triangulations. 
          size_t  AllNormalsCount() const; 
          /// \ru Общее количество всех параметров триангуляций. \en The total number of all parameters of triangulations. 
          size_t  AllParamsCount() const; 
          /// \ru Общее количество всех точек полигонов. \en The total number of all points of poligons. 
          size_t  AllPolyPointsCount() const; 
  /** \} */

          /** \brief \ru Найти пересечение прямой линии и полигонального объекта.
                     \en Find the intersection of a straight line with the polygonal object. \~
            \details \ru Для всех треугольников определяется пересечение с прямой линии и вычисляется минимальное значение 
              параметра точки пересечения на секущей прямой линии. \n
                     \en For all the triangles the intersection with the straight line is determined and the minimum value of 
              the intersection point parameter on the secant straight line is calculated. \n \~
            \param[in] line - \ru Прямая линия, для которой вычисляется пересечение с  и полигонального объекта.
                              \en Straight line to calculate the intersection of triangulation with. \~
            \param[out] crossPnt - \ru Точка пересечения.
                                   \en The intersection point. \~
            \param[out] tRes - \ru Параметр точки пересечения на линии.
                               \en Parameter of the intersection point on the line. \~
            \return \ru Найдено ли пересечение (true - В случае успеха).
                    \en Whether the intersection is found (true if success). \~
          */
          bool    LineIntersection( const MbFloatAxis3D &  line,
                                          MbFloatPoint3D & crossPnt,
                                          float         &  tRes ) const;

          /** \brief \ru Найти индекс треугольника или четырехугольника, ближайших к точке.
                     \en Find index of triangle or quadrangle which is closest to a point. \~
            \param[in] pnt          - \ru Точка.
                                      \en Point. \~
            \param[out] pntProj  - \ru Ближайшая точка на сетке.
                                   \en Nearest point from mesh. \~
            \param[out] iGrid  - \ru Индекс ближайшего грида.
                                 \en Nearest grid index. \~
            \param[out] tqInd - \ru Индекс ближайшего треугольника или четырехугольника.
                                \en Index of nearest triangle or quadrangle. \~
            \return \ru Расстояние до ближайшей точки.
                    \en Distance to the nearest point. \~
           */
          double    FindNearest( const MbCartPoint3D & pnt, MbCartPoint3D & pntProj, size_t & iGrid, c3d::IndicesPair & tqInd ) const;

          /** \brief \ru Создать дерево поиска элементов.
                     \en Create elements search tree. \~
            \details \ru Создать дерево поиска элементов. \n
                     \en Create elements search tree. \n \~
            \param[in] forcedNew - \ru Принудительно создать новые.
                                   \en Create forced new ones. \~
          */
          void    CreateGridSearchTrees( bool forcedNew = false )
          {
            for ( size_t k = _grids.size(); k--; ) {
              const MbGrid * grid = _grids[k];
              if ( nullptr != grid ) {
                if ( forcedNew || !grid->IsSearchTreeReady() )
                  grid->CreateSearchTree();
              }
            }
          }
          /** \brief \ru Удалить дерево поиска элементов.
                     \en Delete elements search tree. \~
            \details \ru Удалить дерево поиска элементов. \n
                     \en Delete elements search tree. \n \~
          */
          void    DeleteGridSearchTrees()
          {
            for ( size_t k = _grids.size(); k--; ) {
              const MbGrid * grid = _grids[k];
              if ( nullptr != grid )
                grid->DeleteSearchTree();
            }
          }
          /// \ru Создать новый временный объект информации о топологии полигонального объекта. \en Create new temporary maintenance object information about polygonal object topology. 
          MbMeshTopology * CreateMeshTopology( bool keepExisting, bool orientNormalsOutward = false ) const;
          /// \ru Получить временный объект информации о топологии полигонального объекта. \en Get temporary maintenance object information about polygonal object topology. 
          const MbMeshTopology * GetMeshTopology() const;
          /// \ru Удалить информацию о топологии полигонального объекта. \en Delete a information about polygonal object topology. 
          void             ResetMeshTopology() const;
          /// \ru Инициализировать по топологии полигонального объекта. \en Initialize by polygonal object topology. 
          void             UpdateByMeshTopology();
          /// \ru Установить маппинг. \en Set mapping. 
          void SetPolymeshMap( MapPolymeshToMbMesh * pMap );
          /// \ru Получить маппинг. \en Get mapping. 
          const MapPolymeshToMbMesh * GetPolymeshMap() const;

private:
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
          MbMesh & operator = ( const MbMesh & );
          /// \ru Удалить вспомогательные данные. \en Clear auxillary data. 
          void ClearAuxData();
  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbMesh ) 
}; // MbMesh

IMPL_PERSISTENT_OPS( MbMesh )

#endif // __MESH_H
