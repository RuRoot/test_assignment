////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Дерево габаритных кубов.
         \en Bounding boxes tree. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_CUBE_TREE_H
#define __MB_CUBE_TREE_H

#include <system_cpp_standard.h>
#include <vector>
#include <utility>
#include <iterator>
#include <algorithm>
#include <limits>
#include <queue>

//------------------------------------------------------------------------------
/** \brief \ru Базовый интерфейс для вычисления квадрата расстояния до объекта, которому
               принадлежит габаритный куб дерева по указанному индексу.
           \en The base interface for calculating of squared distance to the object from 
               the tree's bounding box with specified index. \~
  \ingroup Base_Items
*/
// ---
template <class Point>
class MbTreeDistanceToElementBase
{
public:
  /// \ru Конструктор. \en Constructor.
  MbTreeDistanceToElementBase() {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbTreeDistanceToElementBase() {}

  /// \ru Вычислить квадрат расстояния от точки до объекта из габаритного куба. \en Find the squared distance from the point to tree object from bounding box.
  virtual double DistanceToElement2( const Point & pnt, size_t iElem ) const = 0;

  /// \ru Вернуть флаг объекта из габаритного куба. \en Return the flag of a tree object from bounding box.
  virtual bool IsAllowed( size_t /*iElem*/ ) const { return true; }
};


//------------------------------------------------------------------------------
/** \brief \ru Дерево объектов с габаритными кубами.
           \en Tree of objects with bounding boxes. \~
  \details \ru Дерево объектов с габаритными кубами.\n
               Объекты дерева должны иметь функции "const Cube & Type::GetCube() const", "double Type::DistanceToCube( const Cube & cube, double eps )".
           \en Tree of objects with bounding boxes.\n 
               Tree objects must have functions : "const Cube & Type::GetCube() const", "double Type::DistanceToCube( const Cube & cube, double eps )". \~
  \ingroup Base_Items
*/
// ---
template <class Type, class Cube, class Point, class Vector>
class MbCubeTree {
public:
  typedef std::pair<const Type *,size_t>        ItemIndex;
  typedef std::pair<const Type *,const Type *>  ItemItem;
  typedef std::vector<ItemIndex>                ItemIndexVector;
  typedef std::pair<const Type *,double>        ItemDistance;
  typedef std::pair<size_t,double>              IndexDistance;
  typedef MbCubeTree<Type,Cube,Point,Vector>    TypeTree;

  /// \ru Тип алгоритма вычисления расстояния от точки до габарита. \en Algorithm type that is used to measure distance between point and bounding box.
  enum DistanceMeasure {
    edm_MaxCoord = 0, ///< \ru Максимальное расстояние вдоль одной из координатных осей. \en Distance maximum along one of coordinate axes.
    edm_CubeCenter,   ///< \ru Расстояние до центра габарита объекта. \en Distance to central point of bounding box.
  };
  /// \ru Битовые флаги направлений ветвления. \en Bit flags of branching directions.
  enum BranchingDirections {
    ebd_WasDirX = 0x01, ///< \ru Была попытка ветвления по оси Ox. \en An attempt was made to branch along the Ox axis.
    ebd_WasDirY = 0x02, ///< \ru Была попытка ветвления по оси Oy. \en An attempt was made to branch along the Oy axis.
    ebd_WasDirZ = 0x04, ///< \ru Была попытка ветвления по оси Oz. \en An attempt was made to branch along the Oz axis.
  };
private:
  /// \ru Координатная ось деления на ветви. \en Selected coordinate axis for branching.
  enum DirectionAxis {
    eda_anyDir = 0, ///< \ru Произвольная координатная ось. \en Arbitrarily coordinate axis.
    eda_axisX  = 1, ///< \ru Координатная ось X. \en Coordinate axis X.
    eda_axisY  = 2, ///< \ru Координатная ось Y. \en Coordinate axis Y.
    eda_axisZ  = 3, ///< \ru Координатная ось Z. \en Coordinate axis Z.
    eda_newDir = 4, ///< \ru Нужен выбор оси в зависимости от битового флага. \en Have to select axis by bit flags of branching.
  };

  // Selected coordinate axis for branching
  //     ^
  //     | maximum     +-------------+
  //     |             | upperBranch | 
  //     |             |             |
  //     | upper   +---|-------------|---+
  //     |         |   |             |   |
  //     | midst --------midstBranch-------- or leafObjects for terminal branch content
  //     |         |   |             |   |
  //     | lower   +---|-------------|---+
  //     |             |             |
  //     |             | lowerBranch | 
  //     | minimum     +-------------+
  //     |

private:
  double              midst;          ///< \ru Центральное  значение по оси direction. \en Central value along "direction" axis.
  double              minimum;        ///< \ru Минимальное  значение по оси direction. \en Minimal value along "direction" axis.
  double              maximum;        ///< \ru Максимальное значение по оси direction. \en Maximal value along "direction" axis.
  double              lower;          ///< \ru Размер центральной ветви в сторону увеличения от midst. \en Size of the central branch upward relative to the value of midst.
  double              upper;          ///< \ru Размер центральной ветви в сторону уменьшения от midst. \en Size of the central branch downward relative to the value of midst.
  DirectionAxis       direction;      ///< \ru Направление деления на ветви. \en Branching direction.
  DistanceMeasure     dmType;         ///< \ru Тип расчета расстояния от точки до габарита объекта. \en Distance measurement type between point and bounding box.
  Cube                branchCube;     ///< \ru Габарит ветви. \en Branch bounding box.
  ItemIndexVector     leafObjects;    ///< \ru Объекты c габаритами в конечной ветви (листе). \en Objects (with indices in initial array) in terminal branch of tree.

  TypeTree *          midstBranch;    ///< \ru Ветвь, элементы которой пересекают центральное значение. \en A branch, whose elements cross the central value.
  TypeTree *          lowerBranch;    ///< \ru Ветвь, элементы которой меньше центрального значения. \en A branch whose elements are smaller than the central value.
  TypeTree *          upperBranch;    ///< \ru Ветвь, элементы которой больше центрального значения. \en A branch whose elements are larger than the central value.

  static size_t       treeDepth;      ///< \ru Глубина дерева. \en Tree depth.
  static size_t       leafMinCount;   ///< \ru Минимальное количество элементов в конечной ветви. \en The minimum number of elements in the leaf branch.
  static const double minDoubleValue; ///< \ru Минимальное значение double. \en Min double value.
  static const double maxDoubleValue; ///< \ru Максимальное значение double. \en Max double value.
  static const double defaultEpsilon; ///< \ru Точность по умолчанию. \en Default accuracy.

public:
  /**
    \brief \ru Конструктор дерева по объектам.
           \en Tree constructor by objects. \~
    \details \ru Конструктор дерева по объектам. \n
             \en Tree constructor by objects. \n \~
    \param[in] objects - \ru Объекты дерева.
                         \en Tree objects. \~
    \param[in] dm - \ru Тип вычисления расстояния от точки до габарита.
                    \en Distance measure type between point and bounding box. \~
  */
  MbCubeTree( std::vector<ItemIndex> & objects, DistanceMeasure dm = edm_MaxCoord );
  /**
    \brief \ru Конструктор дерева по объектам.
           \en Tree constructor by objects. \~
    \details \ru Конструктор дерева по объектам. \n
             \en Tree constructor by objects. \n \~
    \param[in] objects - \ru Объекты дерева.
                         \en Tree objects. \~
    \param[in] gabarit - \ru Общий габарит всех объектов дерева, должен быть не пустым.
                         \en The total bounding box of all objects in the tree, it must not be empty. \~
    \param[in] dm - \ru Тип вычисления расстояния от точки до габарита.
                    \en Distance measure type between point and bounding box. \~
  */
  MbCubeTree( std::vector<ItemIndex> & objects, const Cube & gabarit, DistanceMeasure dm = edm_MaxCoord );
  /**
    \brief \ru Конструктор пустого дерева.
           \en Constructor of empty tree. \~
    \details \ru Конструктор пустого дерева. \n
             \en Constructor of empty tree. \n \~
    \param[in] dm - \ru Тип вычисления расстояния от точки до габарита.
                    \en Distance measure type between point and bounding box. \~
  */
  MbCubeTree( DistanceMeasure dm = edm_MaxCoord );
private:
  MbCubeTree( const MbCubeTree & ); // не реализовано / not implemented
  MbCubeTree & operator = ( const MbCubeTree & ); // не реализовано / not implemented
public:
  /// \ru Деструктор. \en Destructor.
  ~MbCubeTree();

public:
  /**
    \brief \ru Инициализация дерева по объектам.
           \en Initialize tree by objects. \~
    \details \ru Инициализация дерева по объектам. \n
             \en Initialize tree by objects. \n \~
    \param[in] objects - \ru Объекты дерева.
                         \en Tree objects. \~
    \param[in] gabarit - \ru Общий габарит всех объектов дерева, должен быть не пустым.
                         \en The total bounding box of all objects in the tree, it must not be empty. \~
    \param[in] dm - \ru Тип вычисления расстояния от точки до габарита.
                    \en Distance measure type between point and bounding box. \~
  */
  bool            InitTree( std::vector<ItemIndex> & objects, const Cube & gabarit, DistanceMeasure dm = edm_MaxCoord );
  /**
    \brief \ru Копировать дерево.
           \en Copy the tree. \~
    \details \ru Копировать дерево. \n
             \en Copy the tree. \n \~
    \param[in] srcTree - \ru Исходное дерево.
                         \en Initial tree. \~
    \param[in] copiedObjects - \ru Копии объектов дерева.
                               \en Copies of tree objects. \~
  */
  bool            InitTree( const MbCubeTree & srcTree, 
                            const std::vector<ItemIndex> & srcObjects,
                            const std::vector<ItemIndex> & dstObjects );
  /// \ru Очистить дерево. \en Clear the tree.
  void            Clear();
  /// \ru Сдвинуть дерево. \en Move the tree.
  void            Move( const Vector & );

public:
  // функции настроек и состояния дерева / settings and tree state functions 

  /// \ru Получить тип алгоритма вычисления расстояния от точки до габарита. \en Get algorithm type that is used to measure distance between point and bounding box.
  DistanceMeasure GetDistanceMeasure() const { return dmType; }

  /// \ru Количество объектов в дереве (не хранит, считает обходом дерева). \en Number of objects in the tree (does not store, tree calculates it by traversing tree nodes).
  size_t          Count() const;

  /// \ru Количество узлов в дереве. \en Number of branches in the tree.
  size_t          NodeCount() const;

  /// \ru Готово ли дерево. \en Whether tree is ready. 
  bool            IsReady() const;

  /// \ru Получить глубину дерева (общая переменная). \en Get tree depth (shared variable).
  size_t          GetTreeDepth() const { return treeDepth; };
  /// \ru Установить глубину дерева (общая переменная). \en Set tree depth (shared variable).
  void            SetTreeDepth( size_t k ) { treeDepth = (std::min)( k, (size_t)64 ); }
  /// \ru Сбросить глубину дерева (общая переменная). \en Reset tree depth (shared variable).
  void            ResetTreeDepth() { treeDepth = 64; }

  /// \ru Получить минимальное количество элементов в конечной ветви (общая переменная). \en Get a minimum number of elements in the leaf branch (shared variable).
  size_t          GetLeafMinElementsCount() const { return leafMinCount; }
  /// \ru Установить минимальное количество элементов в конечной ветви (общая переменная). \en Set a minimum number of elements in the leaf branch (shared variable).
  void            SetLeafMinElementsCount( size_t k ) { leafMinCount = (std::max)( k, (size_t)1 ); }
  /// \ru Сбросить минимальное количество элементов в конечной ветви (общая переменная). \en Reset the minimum number of elements in the leaf branch (shared variable).
  void            ResetLeafMinElementsCount() { leafMinCount = 1; }

public:
  // функции работы с деревом объектов / public functions for work with objects tree

  /// \ru Выдать содержащие точку объекты дерева. \en Get tree objects containing a point. 
  void    GetContainsObjects ( const Point & pnt, double epsilon, 
                               std::vector<const Type *> & objects ) const;
  /// \ru Выдать содержащие точку объекты дерева. \en Get tree objects containing a point. 
  void    GetContainsObjects( const Point & pnt, double epsilon,
                              std::vector<size_t> & indices ) const;

  /// \ru Выдать пересекающиеся с габаритным кубом объекты дерева. \en Get objects of the tree intersecting with the bounding box. 
  void    GetIntersectObjects( const Cube & gabarit, double epsilon, 
                               std::vector<const Type *> & objects,
                               bool skipOwnself = false ) const;
  /// \ru Выдать пересекающиеся с габаритным кубом объекты дерева. \en Get objects of the tree intersecting with the bounding box. 
  void    GetIntersectObjects( const Cube & gabarit, double epsilon, 
                               std::vector<size_t> & indices,
                               bool skipOwnself = false ) const;

  /// \ru Выдать пересекающиеся с отрезком луча кубом объекты дерева. \en Get objects of the tree intersecting with the ray segment. 
  void    GetIntersectObjects( const Point & rayPnt1, const Point & rayPnt2, double epsilon,
                               std::vector<size_t> & indices ) const;

  /// \ru Найти ближайший к кубу объект дерева. \en Find the nearest tree object to a bounding box.
  void    FindNearestObject  ( const Cube & gabarit, double & distance, 
                               const Type *& object, 
                               double eps = MbCubeTree::defaultEpsilon ) const;
  /// \ru Найти ближайший к кубу объект дерева в виде индекса в исходном массиве. \en Find the nearest tree object to a bounding box (as an index in initial array of objects).
  void    FindNearestObject  ( const Cube & gabarit, double & distance, 
                               size_t & index, 
                               double eps = MbCubeTree::defaultEpsilon ) const;
  /// \ru Найти ближайший к точке объект дерева. \en Find the nearest tree object to a point.
  void    FindNearestObject  ( const Point & pnt, double & distance, 
                               const Type *& object, 
                               double eps = MbCubeTree::defaultEpsilon ) const;
  /// \ru Найти ближайший к точке объект дерева в виде индекса в исходном массиве. \en Find the nearest tree object to a point (as an index in initial array of objects).
  void    FindNearestObject  ( const Point & pnt, double & distance, 
                               size_t & index, 
                               double eps = MbCubeTree::defaultEpsilon ) const;
  
  /// \ru Выдать объекты дерева, расположенные к кубу ближе заданного расстояния. \en Get tree objects that are closer to the cube than the specified distance.
  void    GetNearestObjects  ( const Cube & gabarit, const double distance, 
                               std::vector<IndexDistance> & itemDistances,
                               double eps = MbCubeTree::defaultEpsilon ) const;
  /// \ru Выдать объекты дерева, расположенные к точке ближе заданного расстояния. \en Get tree objects that are closer to the point than the specified distance.
  void    GetNearestObjects  ( const Point & pnt, const double distance, 
                               std::vector<ItemDistance> & itemDistances,
                               double eps = MbCubeTree::defaultEpsilon ) const;
  /// \ru Выдать индексы объектов дерева, расположенные к точке ближе заданного расстояния. \en Get tree objects indices that are closer to the point than the specified distance.
  void    GetNearestObjects  ( const Point & pnt, const double distance, 
                               std::vector<IndexDistance> & indexDistances,
                               double eps = MbCubeTree::defaultEpsilon ) const;
  /// \ru Выдать индексы объектов дерева, расположенные к точке ближе заданного расстояния. \en Get tree objects indices that are closer to the point than the specified distance.
  void    GetNearestObjects  ( const Point & pnt, const double distance, 
                               std::vector<size_t> & indices,
                               double eps = MbCubeTree::defaultEpsilon ) const;
  /// \ru Выдать индекс объекта дерева, ближайшего к точке, и квадрат расстояния до него. \en Get tree objects index that are closest to the point, and squared distance to object.
  double  FindNearestObject  ( const Point & pnt, const MbTreeDistanceToElementBase<Point> & calc, size_t & index ) const;
  /// \ru Выдать набор индексов объектов дерева, которые хотя бы частично лежат внутри заданной сферы. \en Get an array of tree object indices at least partially lying inside a given sphere.
  void    FindObjectsInsideSphere( const Point & pnt, double radius, const MbTreeDistanceToElementBase<Point> & calc, c3d::IndicesVector & indices ) const;
  
private:
  // функции инициализация и заполнение ветвей дерева / internal functions for initialization, filling branches

  /// \ru Копировать дерево. \en Copy the tree. 
  bool    InitTree( const MbCubeTree & srcTree, const std::vector<ItemItem> & objPair );

  /// \ru Инициализация дерева по объектам. \en Initialize tree by objects. 
  bool    InitTree   ( std::vector<ItemIndex> & objects, const Cube & gabarit,
                       DirectionAxis a, int tier, unsigned char flag );
  /// \ru Заполнение ветвей дерева. \en Fill (set) tree branches. 
  void    SetBranches( std::vector<ItemIndex> & objects, const Cube & gabarit, 
                       DirectionAxis a, int tier, unsigned char & flag );
  /// \ru Заполнение конечной ветви дерева. \en Fill terminal tree branch. 
  void    FillLeaf( std::vector<ItemIndex> & objects, const Cube & gabarit,
                    DirectionAxis a );

  /// \ru Установка битового флага flag в зависимости от направления direction. \en Set direction bit flag. 
  void    SetFlag( unsigned char & flag ); 
  /// \ru Получить компонент точки в зависимости от направления direction. \en Get point component by branching direction. 
  double  GetPointComponent( const Point & pnt ) const;
  /// \ru Получить компонент точки в зависимости от направления direction. \en Get point component by branching direction. 
  void    GetCubeMinMaxComponents( const Cube & cube, double epsExt, double & wMin, double & wMax ) const;
  /// \ru Измерить расстояние по выбранной в мере. \en Measure distance using selected measure type. 
  double  GetDistance( const Type & object, const Point & pnt, bool signedDistance ) const;

  /// \ru Функция сравнения - меньше. \en Compare function "Less". 
  static bool LessPairByFirstItem( const ItemItem & e1, const ItemItem & e2 ) { return (e1.first < e2.first) ? true : false; }
};


//-------------------------------------------------------------------------------
// \ru Глубина дерева. \en Tree depth.
// ---
template <class Type, class Cube, class Point, class Vector>
size_t MbCubeTree<Type, Cube, Point, Vector>::treeDepth( 64 );


//-------------------------------------------------------------------------------
// \ru Минимальное количество элементов в конечной ветви. \en The minimum number of elements in the leaf branch.
// ---
template <class Type, class Cube, class Point, class Vector>
size_t MbCubeTree<Type, Cube, Point, Vector>::leafMinCount( 1 );


//-------------------------------------------------------------------------------
// \ru Минимальное значение double. \en Min double value.
// ---
template <class Type, class Cube, class Point, class Vector>
const double MbCubeTree<Type, Cube, Point, Vector>::minDoubleValue( (std::numeric_limits<double>::lowest)() );

//-------------------------------------------------------------------------------
// \ru Максимальное значение double. \en Max double value.
// ---
template <class Type, class Cube, class Point, class Vector>
const double MbCubeTree<Type, Cube, Point, Vector>::maxDoubleValue( (std::numeric_limits<double>::max)() );


//-------------------------------------------------------------------------------
// \ru Точность по умолчанию. \en Default accuracy.
// ---
template <class Type, class Cube, class Point, class Vector>
const double MbCubeTree<Type, Cube, Point, Vector>::defaultEpsilon( 1E-7 );


//-------------------------------------------------------------------------------
// \ru Конструктор дерева по объектам. \en Tree constructor by objects.
// ---
template <class Type, class Cube, class Point, class Vector>
inline MbCubeTree<Type, Cube, Point, Vector>::MbCubeTree( std::vector<ItemIndex> & objects, DistanceMeasure dm )
  : midst      ( 0.0  )
  , minimum    ( 0.0  )
  , maximum    ( 0.0  )
  , lower      ( 0.0  )
  , upper      ( 0.0  )
  , direction  ( eda_anyDir )
  , dmType     ( dm   )
  , leafObjects(      )
  , branchCube (      )
  , midstBranch( nullptr )
  , lowerBranch( nullptr )
  , upperBranch( nullptr )
{ 
  size_t cnt = objects.size();
  C3D_ASSERT( Cube::GetDimension() == Point::GetDimension() );

  if ( cnt > 0 ) { // Инициализация дерева
    Cube gabarit;
    for ( size_t i = 0; i < cnt; ++i ) {
      if ( objects[i].first != nullptr )
        gabarit |= objects[i].first->GetCube();
    }
    branchCube = gabarit;
    InitTree( objects, gabarit, eda_anyDir, 0, 0 );
    ResetLeafMinElementsCount();
    ResetTreeDepth();
  }
}


//-------------------------------------------------------------------------------
// \ru Конструктор дерева по объектам. \en Tree constructor by objects.
// ---
template <class Type, class Cube, class Point, class Vector>
inline MbCubeTree<Type, Cube, Point, Vector>::MbCubeTree( std::vector<ItemIndex> & objects, const Cube & gabarit, DistanceMeasure dm )
  : midst      ( 0.0  )
  , minimum    ( 0.0  )
  , maximum    ( 0.0  )
  , lower      ( 0.0  )
  , upper      ( 0.0  )
  , direction  ( eda_anyDir )
  , dmType     ( dm   )
  , leafObjects(      )
  , branchCube ( gabarit  )
  , midstBranch( nullptr )
  , lowerBranch( nullptr )
  , upperBranch( nullptr )
{
  C3D_ASSERT( Cube::GetDimension() == Point::GetDimension() );
  InitTree( objects, gabarit, eda_anyDir, 0, 0 );
  ResetLeafMinElementsCount();
  ResetTreeDepth();
}


//-------------------------------------------------------------------------------
// \ru Конструктор дерева. \en Tree constructor.
// ---
template <class Type, class Cube, class Point, class Vector>
inline MbCubeTree<Type, Cube, Point, Vector>::MbCubeTree( DistanceMeasure dm )
  : midst      ( 0.0  )
  , minimum    ( 0.0  )
  , maximum    ( 0.0  )
  , lower      ( 0.0  )
  , upper      ( 0.0  )
  , direction  ( eda_anyDir )
  , dmType     ( dm   )
  , leafObjects(      )
  , branchCube (      )
  , midstBranch( nullptr )
  , lowerBranch( nullptr )
  , upperBranch( nullptr )
{
}


//-------------------------------------------------------------------------------
// \ru Очистить дерево. \en Clear the tree.
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::Clear()
{
  delete midstBranch; 
  delete lowerBranch; 
  delete upperBranch; 
  midstBranch = lowerBranch = upperBranch = nullptr;
  midst = minimum = maximum = lower = upper = 0.0;
  direction = eda_anyDir;
  branchCube.SetEmpty();
  leafObjects.clear();
}


//-------------------------------------------------------------------------------
// \ru Деструктор. \en Destructor.
// ---
template <class Type, class Cube, class Point, class Vector>
inline MbCubeTree<Type, Cube, Point, Vector>::~MbCubeTree()
{
  delete midstBranch; 
  delete lowerBranch; 
  delete upperBranch; 
}


//-------------------------------------------------------------------------------
// \ru Сдвинуть дерево. \en Move the tree.
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::Move( const Vector & to )
{
  if ( to.MaxFactor() > 0.0 ) {
    if ( midstBranch != nullptr )
      midstBranch->Move( to );
    if ( lowerBranch != nullptr )
      lowerBranch->Move( to );
    if ( upperBranch != nullptr )
      upperBranch->Move( to );

    Point pto;
    pto.Move( to );
    double toValue = GetPointComponent( pto );

    midst += toValue;
    minimum += toValue;
    maximum += toValue;
    lower += toValue;
    upper += toValue;

    branchCube.Move( to );
  }
}


//-------------------------------------------------------------------------------
// \ru Копировать дерево. \en Copy the tree. 
// ---
template <class Type, class Cube, class Point, class Vector>
inline bool MbCubeTree<Type, Cube, Point, Vector>::InitTree( const MbCubeTree<Type, Cube, Point, Vector> & srcTree, const std::vector<ItemItem> & itemPairs )
{
  bool res = true;

  midst = srcTree.midst;
  minimum = srcTree.minimum;
  maximum = srcTree.maximum;
  lower = srcTree.lower;
  upper = srcTree.upper;

  direction = srcTree.direction;
  dmType = srcTree.dmType;

  branchCube = srcTree.branchCube;

  size_t cnt = srcTree.leafObjects.size();
  leafObjects.resize( cnt );

  typename std::vector<ItemItem>::const_iterator it;
  ItemItem itemPair( nullptr, nullptr );

  for ( size_t k = 0; k < cnt && res; ++k ) {
    res = false;

    leafObjects[k].second = srcTree.leafObjects[k].second;

    itemPair.first = srcTree.leafObjects[k].first;
    it = std::lower_bound( itemPairs.begin(), itemPairs.end(), itemPair, MbCubeTree<Type, Cube, Point, Vector>::LessPairByFirstItem );
    
    if ( it != itemPairs.end() ) {
      if ( !MbCubeTree<Type, Cube, Point, Vector>::LessPairByFirstItem( itemPair, *it ) ) {
        leafObjects[k].first = it->second;
        res = true;
      }
    }
  }

  if ( res && (srcTree.midstBranch != nullptr) ) {
    C3D_ASSERT( midstBranch == nullptr );
    if ( midstBranch == nullptr )
      midstBranch = new MbCubeTree<Type, Cube, Point, Vector>( srcTree.midstBranch->dmType );
    else
      midstBranch->Clear();

    res &= midstBranch->InitTree( *srcTree.midstBranch, itemPairs );
  }

  if ( res && (srcTree.lowerBranch != nullptr) ) {
    C3D_ASSERT( lowerBranch == nullptr );
    if ( lowerBranch == nullptr )
      lowerBranch = new MbCubeTree<Type, Cube, Point, Vector>( srcTree.lowerBranch->dmType );
    else
      lowerBranch->Clear();

    res &= lowerBranch->InitTree( *srcTree.lowerBranch, itemPairs );
  }

  if ( res && (srcTree.upperBranch != nullptr) ) {
    C3D_ASSERT( upperBranch == nullptr );
    if ( upperBranch == nullptr )
      upperBranch = new MbCubeTree<Type, Cube, Point, Vector>( srcTree.upperBranch->dmType );
    else
      upperBranch->Clear();

    res &= upperBranch->InitTree( *srcTree.upperBranch, itemPairs );
  }

  return res;
}


//-------------------------------------------------------------------------------
// \ru Копировать дерево. \en Copy the tree. 
// ---
template <class Type, class Cube, class Point, class Vector>
inline bool MbCubeTree<Type, Cube, Point, Vector>::InitTree( const MbCubeTree<Type, Cube, Point, Vector> & srcTree, 
                                                             const std::vector<ItemIndex> &                srcItems,
                                                             const std::vector<ItemIndex> &                dstItems )
{
  bool res = false;

  Clear();

  const size_t itemCnt = srcItems.size();

  if ( (itemCnt > 0) && (itemCnt == dstItems.size()) && srcTree.IsReady() && (srcTree.Count() == itemCnt) ) {
    res = true;

    std::vector< std::pair<const Type *, const Type *> > itemPairs;
    itemPairs.resize( itemCnt );

    double eps = defaultEpsilon;

    size_t k;
    for ( k = 0; k < itemCnt; ++k ) {
      res = false;
      if ( srcItems[k].second == dstItems[k].second ) {
        const Type * srcItem = srcItems[k].first;
        const Type * dstItem = dstItems[k].first;
        if ( (srcItem != dstItem) && (srcItem != nullptr) && (dstItem != nullptr) ) {
          if ( srcItem->IsSame( *dstItem, eps ) ) {
            itemPairs[k].first = srcItem;
            itemPairs[k].second = dstItem;
            res = true;
          }
        }
      }
    }
    if ( res ) {
      res = true;
      if ( itemPairs.size() > 1 ) {
        std::sort( itemPairs.begin(), itemPairs.end() );
        itemPairs.erase( std::unique( itemPairs.begin(), itemPairs.end() ), itemPairs.end() );
        if ( itemPairs.size() != itemCnt )
          res = false;
      }
    }
    if ( res ) {
      res = InitTree( srcTree, itemPairs );
    }
  }

  return res;
}


//-------------------------------------------------------------------------------
// \ru Инициализация дерева по объектам. \en Initialize tree by objects. 
// ---
template <class Type, class Cube, class Point, class Vector>
inline bool MbCubeTree<Type, Cube, Point, Vector>::InitTree( std::vector<ItemIndex> & objects, const Cube & gabarit, DistanceMeasure dm )
{ 
  bool res = false;

  Clear();
  dmType = dm;

  size_t cnt = objects.size();
  C3D_ASSERT( Cube::GetDimension() == Point::GetDimension() );

  if ( cnt > 0 ) { // Инициализация дерева
    res = InitTree( objects, gabarit, eda_anyDir, 0, 0 );
    ResetLeafMinElementsCount();
    ResetTreeDepth();
  }

  return res;
}


//-------------------------------------------------------------------------------
// \ru Инициализация дерева по объектам. \en Initialize tree by objects. 
// ---
template <class Type, class Cube, class Point, class Vector>
inline bool MbCubeTree<Type, Cube, Point, Vector>::InitTree( std::vector<ItemIndex> & objects, const Cube & gabarit, 
                                                             DirectionAxis a, int tier, unsigned char flag )
{ 
  bool res = false;

  size_t iCount = objects.size();

  if ( iCount > 0 ) { // Инициализация дерева / Initialize tree
    const double eps = defaultEpsilon;

    const size_t gabDim = gabarit.GetDimension();

    // slow : memory reallocation
    //std::vector< std::pair<double,double> > gabLimits;
    //std::vector<double> gabLens;
    //gabLimits.reserve( gabDim );
    //gabLens.reserve( gabDim );

    double gabLens[3];
    C3D_ASSERT( gabDim < 4 );

    bool validLimits = true;
    {
      double gabMin, gabMax, gabLen;
      for ( size_t i = 0; i < gabDim; ++i ) {
        gabMin = gabarit.GetMin(i);
        gabMax = gabarit.GetMax(i);
        gabLen = gabMax - gabMin;

        // slow : memory reallocation
        //gabLimits.push_back( std::make_pair( gabMin, gabMax ) );
        //gabLens.push_back( gabLen );
        //gabLimits[i] = std::make_pair( gabMin, gabMax );
        gabLens[i] = gabLen;

        validLimits &= (gabLen > -eps);
      }
    }

    if ( validLimits ) {
      res = true;

      branchCube = gabarit;

      if ( (tier < (int)treeDepth) && (iCount > leafMinCount) ) {
        // если ось указана, то следующее разделение должно выполниться по другой оси 
        switch ( a ) {

          case eda_axisX : { // разделение на ветви по по наиболее вытянутой оси Y или Z 
            if ( gabDim == 3 ) {
              if ( gabLens[1] - eps > gabLens[2] ) 
                SetBranches( objects, gabarit, eda_axisY, tier, flag );
              else 
                SetBranches( objects, gabarit, eda_axisZ, tier, flag );
            }
            else if ( gabDim == 2 ) {
              SetBranches( objects, gabarit, eda_axisY, tier, flag );
            }
          } break;

          case eda_axisY : { // разделение на ветви по по наиболее вытянутой оси Z или X
            if ( gabDim == 3 ) {
              if ( gabLens[2] - eps > gabLens[0] ) 
                SetBranches( objects, gabarit, eda_axisZ, tier, flag );
              else 
                SetBranches( objects, gabarit, eda_axisX, tier, flag );
            }
            else if ( gabDim == 2 ) {
              SetBranches( objects, gabarit, eda_axisX, tier, flag );
            }
          } break;

          case eda_axisZ : { // разделение на ветви по по наиболее вытянутой оси X или Y
            if ( gabDim == 3 ) {
              if ( gabLens[0] - eps > gabLens[1] ) 
                SetBranches( objects, gabarit, eda_axisX, tier, flag );
              else 
                SetBranches( objects, gabarit, eda_axisY, tier, flag );
            }
            else {
              C3D_ASSERT_UNCONDITIONAL( false );
            }
          } break;

          case eda_newDir : { // разделение на ветви по ещё не использованному направлению
            if ( gabDim == 3 ) {
              if ( flag & ebd_WasDirX ) { // попытка разделения по оси X не удалась
                if ( gabLens[1] + eps > gabLens[2] && !(flag & ebd_WasDirY) ) 
                  SetBranches( objects, gabarit, eda_axisY, tier, flag );
                else { 
                  if ( !(flag & ebd_WasDirZ) ) 
                    SetBranches( objects, gabarit, eda_axisZ, tier, flag );
                  else { // исчерпаны все возможности разделения
                    FillLeaf( objects, gabarit, eda_axisZ ); 
                  }
                }
              }
              else if ( flag & ebd_WasDirY ) { // попытка разделения по оси Y не удалась
                if ( gabLens[2] - eps > gabLens[0] && !(flag & ebd_WasDirZ) ) 
                  SetBranches( objects, gabarit, eda_axisZ, tier, flag );
                else 
                  SetBranches( objects, gabarit, eda_axisX, tier, flag );
              }
              else { // попытка разделения по оси Z не удалась
                if ( gabLens[0] + eps > gabLens[1] ) 
                  SetBranches( objects, gabarit, eda_axisX, tier, flag );
                else 
                  SetBranches( objects, gabarit, eda_axisY, tier, flag );
              }
            }
            else if ( gabDim == 2 ) {
              if ( flag & ebd_WasDirX ) { // попытка разделения по оси X не удалась
                if ( !(flag & ebd_WasDirY) ) 
                  SetBranches( objects, gabarit, eda_axisY, tier, flag );
                else { // исчерпаны все возможности разделения
                  FillLeaf( objects, gabarit, eda_axisY ); 
                }
              }
              else if ( flag & ebd_WasDirY ) { // попытка разделения по оси Y не удалась
                if ( !(flag & ebd_WasDirX) ) 
                  SetBranches( objects, gabarit, eda_axisX, tier, flag );
                else { // исчерпаны все возможности разделения
                  FillLeaf( objects, gabarit, eda_axisX ); 
                }
              }
            }
            else {
              C3D_ASSERT_UNCONDITIONAL( false );
            }
          } break;

          default : { // разделение на ветви по наиболее вытянутой оси 
            if ( gabDim == 3 ) {
              if ( (gabLens[0] + eps > gabLens[1]) && (gabLens[0] + eps > gabLens[2]) )
                SetBranches( objects, gabarit, eda_axisX, tier, flag );
              else if ( gabLens[1] + eps > gabLens[2] )
                SetBranches( objects, gabarit, eda_axisY, tier, flag );
              else
                SetBranches( objects, gabarit, eda_axisZ, tier, flag );
            }
            else if ( gabDim == 2 ) {
              if ( gabLens[0] + eps > gabLens[1] )
                SetBranches( objects, gabarit, eda_axisX, tier, flag );
              else
                SetBranches( objects, gabarit, eda_axisY, tier, flag );
            }
          } break;
        }
      }
      else { // один объект или достигнут максимальный уровень вложенности  
        if ( (a == eda_anyDir) || (a == eda_newDir) ) {
          if ( gabDim == 3 ) {
            if ( (gabLens[0] + eps > gabLens[1]) && (gabLens[0] + eps > gabLens[2]) ) 
              a = eda_axisX;
            else if ( gabLens[1] + eps > gabLens[2] ) 
              a = eda_axisY;
            else 
              a = eda_axisZ;
          }
          else if ( gabDim == 2 ) {
            if ( gabLens[0] + eps > gabLens[1] ) 
              a = eda_axisX;
            else
              a = eda_axisY;
          }
        }
        FillLeaf( objects, gabarit, a ); 
      }
    }
  }

  return res;
}


//-------------------------------------------------------------------------------
// установка ветвей
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::SetBranches( std::vector<ItemIndex> & objects, const Cube & gabarit, 
                                                                DirectionAxis a, int tier, unsigned char & flag ) 
{
  direction = a;
  const size_t iCount = objects.size();
  double eps = defaultEpsilon;

  GetCubeMinMaxComponents( gabarit, eps, minimum, maximum );

  midst = 0.5 * (minimum + maximum);   // центральное значение
  lower = midst; 
  upper = midst; 

  size_t lowerCount = 0;
  size_t upperCount = 0;
  size_t midstCount = 0;

  Cube lowerGabarit;
  Cube upperGabarit;
  Cube midstGabarit;

  for ( size_t i = 0; i < iCount; ++i ) {
    const ItemIndex & obj = objects[i];
    if ( obj.first != nullptr ) {
      const Cube & cube = obj.first->GetCube();

      double pMin, pMax;
      GetCubeMinMaxComponents( cube, 0.0, pMin, pMax );

      if ( pMax < midst - eps ) {
        lowerCount++;
        lowerGabarit |= cube;
      }
      else if ( pMin > midst + eps ) {
        upperCount++;
        upperGabarit |= cube;
      }
      else {
        lower = std_min( lower, pMin );
        upper = std_max( upper, pMax );
        midstCount++;
        midstGabarit |= cube;
      }
    }
  }

  std::vector<ItemIndex> lowerArray;
  std::vector<ItemIndex> upperArray;
  std::vector<ItemIndex> midstArray;

  lowerArray.reserve( lowerCount );
  upperArray.reserve( upperCount );
  midstArray.reserve( midstCount );

  // KOMPAS-46058
  //size_t iDelta = std_max( (size_t)1, iCount / 8 );
  //lowerArray.reserve( iDelta );
  //upperArray.reserve( iDelta );
  //midstArray.reserve( iDelta );

  for ( size_t i = 0; i < iCount; ++i ) {
    const ItemIndex & obj = objects[i];
    if ( obj.first != nullptr ) {
      const Cube & cube = obj.first->GetCube();

      double pMin, pMax;
      GetCubeMinMaxComponents( cube, 0.0, pMin, pMax );

      if ( pMax < midst - eps ) { // объект расположен ниже центрального значения
        lowerArray.emplace_back( obj );
      }
      else if ( pMin > midst + eps ) { // объект расположен выше центрального значения
        upperArray.emplace_back( obj );
      }
      else { // объект содержит центральное значение
        lower = std_min( lower, pMin ); 
        upper = std_max( upper, pMax ); 
        midstArray.emplace_back( obj );
      }
    }
  }

  lower -= eps; // верхний размер центральной ветви, расширенный на погрешность
  upper += eps; // нижний  размер центральной ветви, расширенный на погрешность

  lowerCount = lowerArray.size();
  upperCount = upperArray.size();
  midstCount = midstArray.size();

  if ( (lowerCount > 0) || (upperCount > 0) ) {
    if ( lowerCount > 0 ) { // нижняя ветвь / lower branch
      C3D_ASSERT ( lowerBranch == nullptr );
      if ( lowerBranch == nullptr )
        lowerBranch = new MbCubeTree<Type,Cube,Point,Vector>( dmType );
      lowerBranch->InitTree( lowerArray, lowerGabarit, eda_anyDir, tier+1, 0 );
      lowerArray.clear();
    }
    if ( upperCount > 0 ) { // верхняя ветвь / upper branch
      C3D_ASSERT( upperBranch == nullptr );
      if ( upperBranch == nullptr )
        upperBranch = new MbCubeTree<Type,Cube,Point,Vector>( dmType );
      upperBranch->InitTree( upperArray, upperGabarit, eda_anyDir, tier+1, 0 );
      upperArray.clear();
    }
    if ( midstCount > 0 ) { // центральная ветвь / central branch
      C3D_ASSERT( midstBranch == nullptr );
      if ( midstBranch == nullptr )
        midstBranch = new MbCubeTree<Type,Cube,Point,Vector>( dmType );
      midstBranch->InitTree( midstArray, midstGabarit, direction, tier+1, 0 );
      midstArray.clear();
    }
  }
  else { // попытка разделить на ветви по-другому, ещё не использованному направлению
    bool freeDir = false;

    const size_t gabDim = gabarit.GetDimension();
    if ( gabDim == 3 ) {
      freeDir = !((flag & ebd_WasDirX) && 
                  (flag & ebd_WasDirY) &&
                  (flag & ebd_WasDirZ));
    }
    else if ( gabDim == 2 ) {
      freeDir = !((flag & ebd_WasDirX) && 
                  (flag & ebd_WasDirY));
    }

    if ( midstCount > leafMinCount && freeDir ) {
      // установка битового флага flag в зависимости от направления direction
      SetFlag( flag ); 
      InitTree( midstArray, midstGabarit, eda_newDir, tier, flag );
      midstArray.clear();
    }
    else { // разделение на ветви не произошло ни по одному направлению
      branchCube = gabarit;
      leafObjects = std::move( objects );
    }
  }
}


//-------------------------------------------------------------------------------
// установка битового флага flag в зависимости от направления direction
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::SetFlag( unsigned char & flag ) 
{
  if ( direction == eda_axisX )
    flag |= ebd_WasDirX;
  else if ( direction == eda_axisY )
    flag |= ebd_WasDirY;
  else if ( direction == eda_axisZ )
    flag |= ebd_WasDirZ;
}


//-------------------------------------------------------------------------------
// \ru Получить компонент точки в зависимости от направления direction. \en Get point component by branching direction. 
// ---
template <class Type, class Cube, class Point, class Vector>
inline double MbCubeTree<Type, Cube, Point, Vector>::GetPointComponent( const Point & pnt ) const
{
  double w = minDoubleValue;

  switch ( direction ) {
    case eda_axisX  : { w = pnt[0]; } break;
    case eda_axisY  : { w = pnt[1]; } break;
    case eda_axisZ  : { w = pnt[2]; } break;
    case eda_anyDir : { C3D_ASSERT_UNCONDITIONAL( false ); } break;
    case eda_newDir : { C3D_ASSERT_UNCONDITIONAL( false ); } break;
    default: break;
  }
  return w;
}


//-------------------------------------------------------------------------------
// \ru Получить компонент точки в зависимости от направления direction. \en Get point component by branching direction. 
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::GetCubeMinMaxComponents( const Cube & cube, double eps, double & wMin, double & wMax ) const
{
  wMin = maxDoubleValue;
  wMax = minDoubleValue;

  switch ( direction ) {
    case eda_axisX: { wMin = cube.GetMin( 0 ) - eps; wMax = cube.GetMax( 0 ) + eps; } break;
    case eda_axisY: { wMin = cube.GetMin( 1 ) - eps; wMax = cube.GetMax( 1 ) + eps; } break;
    case eda_axisZ: { wMin = cube.GetMin( 2 ) - eps; wMax = cube.GetMax( 2 ) + eps; } break;
    case eda_anyDir: { C3D_ASSERT_UNCONDITIONAL( false ); } break;
    case eda_newDir: { C3D_ASSERT_UNCONDITIONAL( false ); } break;
    default: break;
  }
}


//-------------------------------------------------------------------------------
// \ru Заполнение конечной ветви дерева (установка конечной ветви). \en Fill (set) terminal tree branch. 
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::FillLeaf( std::vector<ItemIndex> & objects, const Cube & gabarit, DirectionAxis a ) 
{
  direction = a;
  const double eps = defaultEpsilon;
  GetCubeMinMaxComponents( gabarit, eps, minimum, maximum );

  midst = 0.5 * (minimum + maximum); // центральное значение
  lower = minimum; 
  upper = maximum; 

  branchCube = gabarit;

  leafObjects = std::move( objects );
}


//-------------------------------------------------------------------------------
// \ru Готово ли дерево. \en Whether tree is ready. 
// ---
template <class Type, class Cube, class Point, class Vector>
inline bool MbCubeTree<Type, Cube, Point, Vector>::IsReady() const
{
  return ( (midstBranch != nullptr) ||
           (lowerBranch != nullptr) ||
           (upperBranch != nullptr) ||
           (leafObjects.size() > 0) );
}


//-------------------------------------------------------------------------------
// \ru Количество объектов в дереве. \en Number of objects in the tree.
// ---
template <class Type, class Cube, class Point, class Vector>
inline size_t MbCubeTree<Type, Cube, Point, Vector>::Count() const
{
  size_t cnt = leafObjects.size();

  if ( midstBranch != nullptr )
    cnt += midstBranch->Count();
  if ( lowerBranch != nullptr )
    cnt += lowerBranch->Count();
  if ( upperBranch != nullptr )
    cnt += upperBranch->Count();

  return cnt;
}


//-------------------------------------------------------------------------------
// \ru Количество узлов в дереве. \en Number of branches in the tree.
// ---
template <class Type, class Cube, class Point, class Vector>
inline size_t MbCubeTree<Type, Cube, Point, Vector>::NodeCount() const
{
  size_t count = 0;

  std::vector<const TypeTree *> stack;
  stack.push_back( this );
  
  while ( !stack.empty() ) {
    const TypeTree * ptr = stack.back();
    stack.pop_back();
    ++count;
    
    if ( ptr->lowerBranch != nullptr )
      stack.push_back( ptr->lowerBranch );

    if ( ptr->upperBranch != nullptr )
      stack.push_back( ptr->upperBranch );

    if ( ptr->midstBranch != nullptr )
      stack.push_back( ptr->midstBranch );
  }

  return count;
}


//-------------------------------------------------------------------------------
// \ru Выдать содержащие точку объекты дерева. \en Get tree objects containing a point. 
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::GetContainsObjects( const Point & pnt, double epsilon, 
                                                                       std::vector<const Type *> & items ) const
{
  if ( (lowerBranch != nullptr) || (upperBranch != nullptr) || (midstBranch != nullptr) ) {
    double w = GetPointComponent( pnt );

    if ( (w < (minimum - epsilon)) || ((maximum + epsilon) < w) )
      return; // вне области / out of region

    if ( (w < (midst + epsilon)) && (lowerBranch != nullptr) ) {
      lowerBranch->GetContainsObjects( pnt, epsilon, items );
    }
    if ( ((midst - epsilon) < w) && (upperBranch != nullptr) ) {
      upperBranch->GetContainsObjects( pnt, epsilon, items );
    }
    if ( ((lower - epsilon) < w) && (w < (upper + epsilon)) && (midstBranch != nullptr) ) {
      midstBranch->GetContainsObjects( pnt, epsilon, items );
    }
  }
  else { // содержимое конечной ветви // terminal branch content
    size_t iCount = leafObjects.size();
    items.reserve( items.size() + iCount );
    for ( size_t i = 0; i < iCount; ++i ) {
      const ItemIndex & obj = leafObjects[i];
      if ( obj.first != nullptr ) {
        if ( obj.first->GetCube().Contains( pnt, epsilon ) )
          items.push_back( obj.first );
      }
    }
  }
}


//-------------------------------------------------------------------------------
// \ru Выдать содержащие точку объекты дерева. \en Get tree objects containing a point. 
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::GetContainsObjects( const Point & pnt, double epsilon,
                                                                       std::vector<size_t> & indices ) const
{
  if ( (lowerBranch != nullptr) || (upperBranch != nullptr) || (midstBranch != nullptr) ) {
    double w = GetPointComponent( pnt );

    if ( (w < (minimum - epsilon)) || ((maximum + epsilon) < w) )
      return; // вне области / out of region

    if ( (w < (midst + epsilon)) && (lowerBranch != nullptr) ) {
      lowerBranch->GetContainsObjects( pnt, epsilon, indices );
    }
    if ( ((midst - epsilon) < w) && (upperBranch != nullptr) ) {
      upperBranch->GetContainsObjects( pnt, epsilon, indices );
    }
    if ( ((lower - epsilon) < w) && (w < (upper + epsilon)) && (midstBranch != nullptr) ) {
      midstBranch->GetContainsObjects( pnt, epsilon, indices );
    }
  }
  else { // содержимое конечной ветви // terminal branch content
    size_t iCount = leafObjects.size();
    indices.reserve( indices.size() + iCount );
    for ( size_t i = 0; i < iCount; ++i ) {
      const ItemIndex & obj = leafObjects[i];
      if ( obj.first != nullptr ) {
        if ( obj.first->GetCube().Contains( pnt, epsilon ) )
          indices.push_back( obj.second );
      }
    }
  }
}


//-------------------------------------------------------------------------------
// \ru Выдать пересекающиеся с габаритным кубом объекты дерева. \en Get objects of the tree intersecting with the bounding box. 
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::GetIntersectObjects( const Cube & gabarit, double epsilon, 
                                                                        std::vector<const Type *> & items,
                                                                        bool skipOwnself ) const
{
  if ( (lowerBranch != nullptr) || (upperBranch != nullptr) || (midstBranch != nullptr) ) {
    double wMin, wMax;
    GetCubeMinMaxComponents( gabarit, 0.0, wMin, wMax );

    if ( (wMax < minimum) || (maximum < wMin) )
      return; // вне области / out of region

    if ( (wMin < midst) && (lowerBranch != nullptr) ) {
      lowerBranch->GetIntersectObjects( gabarit, epsilon, items, skipOwnself );
    }
    if ( (midst < wMax) && (upperBranch != nullptr) ) {
      upperBranch->GetIntersectObjects( gabarit, epsilon, items, skipOwnself );
    }
    if ( (lower < wMax) && (wMin < upper) && (midstBranch != nullptr) ) {
      midstBranch->GetIntersectObjects( gabarit, epsilon, items, skipOwnself );
    }
  }
  else { // содержимое конечной ветви // terminal branch content
    size_t iCount = leafObjects.size();
    items.reserve( items.size() + iCount );
    for ( size_t i = 0; i < iCount; ++i ) {
      const ItemIndex & obj = leafObjects[i];
      if ( obj.first != nullptr ) {
        if ( !skipOwnself || (&gabarit != &static_cast<const Cube &>(obj.first->GetCube()) ) ) { // KOMPAS-20871
          if ( obj.first->GetCube().Intersect( gabarit, epsilon ) )
            items.push_back( obj.first );
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// \ru Выдать пересекающиеся с габаритным кубом объекты дерева. \en Get objects of the tree intersecting with the bounding box. 
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::GetIntersectObjects( const Cube & gabarit, double epsilon, 
                                                                        std::vector<size_t> & items,
                                                                        bool skipOwnself ) const
{
  if ( (lowerBranch != nullptr) || (upperBranch != nullptr) || (midstBranch != nullptr) ) {
    double wMin, wMax;
    GetCubeMinMaxComponents( gabarit, 0.0, wMin, wMax );

    if ( (wMax < minimum) || (maximum < wMin) )
      return; // вне области / out of region

    if ( (wMin < midst) && (lowerBranch != nullptr) ) {
      lowerBranch->GetIntersectObjects( gabarit, epsilon, items, skipOwnself );
    }
    if ( (midst < wMax) && (upperBranch != nullptr) ) {
      upperBranch->GetIntersectObjects( gabarit, epsilon, items, skipOwnself );
    }
    if ( (lower < wMax) && (wMin < upper) && (midstBranch != nullptr) ) {
      midstBranch->GetIntersectObjects( gabarit, epsilon, items, skipOwnself );
    }
  }
  else { // содержимое конечной ветви // terminal branch content
    size_t iCount = leafObjects.size();
    items.reserve( items.size() + iCount );
    for ( size_t i = 0; i < iCount; ++i ) {
      const ItemIndex & obj = leafObjects[i];
      if ( obj.first != nullptr ) {
        if ( !skipOwnself || (&gabarit != &static_cast<const Cube &>(obj.first->GetCube()) ) ) { // KOMPAS-20871
          if ( obj.first->GetCube().Intersect( gabarit, epsilon ) )
            items.push_back( obj.second );
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// \ru Выдать пересекающиеся с отрезком луча кубом объекты дерева. \en Get objects of the tree intersecting with the ray segment. 
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::GetIntersectObjects( const Point & rayPnt1, const Point & rayPnt2, double eps,
                                                                        std::vector<size_t> & itemIndices ) const
{
  bool isBranch = (lowerBranch != nullptr) || (upperBranch != nullptr) || (midstBranch != nullptr);

  bool isLeaf = !leafObjects.empty();

  Point segmPnt1( rayPnt1 ), segmPnt2( rayPnt2 );
  double segmLen = segmPnt1.DistanceToPoint( segmPnt2 );

  double sameEps = std_min( eps, LENGTH_EPSILON );
  sameEps = std_max( sameEps, EXTENT_EPSILON );

  C3D_ASSERT( !branchCube.IsEmpty() );
 // C3D_ASSERT( !rayPnt1.IsSame( rayPnt2, sameEps ) );

  if ( isBranch || isLeaf ) {
    if ( !branchCube.IsEmpty() && !rayPnt1.IsSame( rayPnt2, sameEps ) ) {
      Vector rayVect( rayPnt1, rayPnt2 );
      {
        double rayLen = rayVect.Length();
        rayVect /= rayLen;
      }
      double minParam = maxDoubleValue;
      double maxParam = minDoubleValue;

      Point vertPnt;
      Vector vertVect;
      size_t vertCnt = branchCube.GetVerticesCount();
      for ( size_t vertInd = 0; vertInd < vertCnt; ++vertInd ) {
        branchCube.GetVertex( vertInd, vertPnt );
        vertVect.Init( rayPnt1, vertPnt );
        double vt = (rayVect * vertVect);
        if ( vt < minParam )
          minParam = vt;
        if ( vt > maxParam )
          maxParam = vt;
      }
      segmPnt1.Set( rayPnt1, rayVect, minParam );
      segmPnt2.Set( rayPnt1, rayVect, maxParam );
      segmLen = segmPnt1.DistanceToPoint( segmPnt2 );
    }
  }

  bool diffPnt = !segmPnt1.IsSame( segmPnt2, sameEps );

  if ( isBranch ) {
    double w1 = minDoubleValue;
    double w2 = minDoubleValue;

    switch ( direction ) {
      case eda_axisX  : { w1 = segmPnt1[0]; w2 = segmPnt2[0]; } break;
      case eda_axisY  : { w1 = segmPnt1[1]; w2 = segmPnt2[1]; } break;
      case eda_axisZ  : { w1 = segmPnt1[2]; w2 = segmPnt2[2]; } break;
      case eda_anyDir : { C3D_ASSERT_UNCONDITIONAL( false );  } break;
      case eda_newDir : { C3D_ASSERT_UNCONDITIONAL( false );  } break;
      default: break;
    }

    double wMin = std_min( w1, w2 );
    double wMax = std_max( w1, w2 );

    if ( (wMax < (minimum - eps)) || ((maximum + eps) < wMin) )
      return; // вне области / out of region

    if ( (wMin < (midst + eps)) && (lowerBranch != nullptr) ) {
      if ( diffPnt ) {
        lowerBranch->GetIntersectObjects( segmPnt1, segmPnt2, eps, itemIndices );
      }else{
        lowerBranch->GetContainsObjects( segmPnt1, eps, itemIndices );
      }
    }
    if ( (wMax > (midst - eps)) && (upperBranch != nullptr) ) {
      if ( diffPnt ) {
        upperBranch->GetIntersectObjects( segmPnt1, segmPnt2, eps, itemIndices );
      }
      else {
        upperBranch->GetContainsObjects( segmPnt1, eps, itemIndices );
      }
    }
    if ( (wMax > (lower - eps)) && (wMin < (upper + eps)) && (midstBranch != nullptr) ) {
      if ( diffPnt ) {
        midstBranch->GetIntersectObjects( segmPnt1, segmPnt2, eps, itemIndices );
      }
      else {
        midstBranch->GetContainsObjects( segmPnt1, eps, itemIndices );
      }
    }
  }
  else if ( isLeaf ) { // содержимое конечной ветви // terminal branch content

    if ( !diffPnt ) {
      GetContainsObjects( segmPnt1, eps, itemIndices );
      return;
    }

    const Cube segmCube( segmPnt1, segmPnt2, true );

    Point objPnt, segmPnt;
    segmPnt.Set( segmPnt1, 0.5, segmPnt2, 0.5 );

    Vector vect1, vect2;

    size_t iCount = leafObjects.size();
    itemIndices.reserve( itemIndices.size() + iCount );
    for ( size_t i = 0; i < iCount; ++i ) {
      const ItemIndex & obj = leafObjects[i];
      if ( obj.first != nullptr ) {
        bool add = false;

        const Cube & objCube = obj.first->GetCube();

        if ( objCube.Contains( segmPnt, eps ) )
          add = true;
        else if ( objCube.Intersect( segmCube, eps ) ) {
          add = true;

          if ( segmLen > eps ) {
            objCube.GetCenter( objPnt );
            double objDist = 0.5 * objCube.GetDiagonal();

            vect1.Init( segmPnt1, objPnt );
            vect2.Init( segmPnt1, segmPnt2 );
            vect2 /= segmLen;
            vect2 = (vect1 * vect2) * vect2;
            vect1 -= vect2;
            double d = ::sqrt( vect1 * vect1 );

            if ( d > objDist + eps )
              add = false;
          }
        }
        if ( add )
          itemIndices.push_back( obj.second );
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Измерить расстояние по выбранной в мере
// ---
template <class Type, class Cube, class Point, class Vector>
inline double MbCubeTree<Type, Cube, Point, Vector>::GetDistance( const Type & object, const Point & pnt, bool signedDistance ) const
{
  double d = maxDoubleValue;

  if ( dmType == edm_MaxCoord )
    d = object.GetCube().DistanceToPoint( pnt );
  else if ( dmType == edm_CubeCenter ) {
    Point pnt0;
    object.GetCube().GetCenter( pnt0 );
    d = pnt0.DistanceToPoint( pnt );
  }

  if ( !signedDistance )
    d = ::fabs(d);

  return d;
}


//-------------------------------------------------------------------------------
// \ru Найти ближайший к кубу объект дерева. \en Find the nearest tree object to a bounding box.
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::FindNearestObject( const Cube & gabarit, double & distance, const Type *& item, double eps ) const
{
  if ( (lowerBranch != nullptr) || (upperBranch != nullptr) || (midstBranch != nullptr) ) {
    double wMin, wMax;
    GetCubeMinMaxComponents( gabarit, 0.0, wMin, wMax );

    if ( (wMax < (minimum-distance)) || ((maximum+distance) < wMin) )
      return; // вне области / out of region
    
    if ( (wMin < (midst+distance)) && (lowerBranch != nullptr) ) {
      lowerBranch->FindNearestObject( gabarit, distance, item, eps );
    }
    if ( ((midst-distance) < wMax) && (upperBranch != nullptr) ) {
      upperBranch->FindNearestObject( gabarit, distance, item, eps );
    }
    if ( ((lower-distance) < wMax) && (wMin < (upper+distance)) && (midstBranch != nullptr) ) {
      midstBranch->FindNearestObject( gabarit, distance, item, eps );
    }
  }
  else { // содержимое конечной ветви // terminal branch content
    for ( size_t i = 0, iCount = leafObjects.size(); i < iCount; ++i ) {
      const ItemIndex & obj = leafObjects[i];
      if ( obj.first != nullptr ) {
        double d = obj.first->GetCube().DistanceToCube( gabarit, eps );
        if ( distance > d - eps ) {
          item = obj.first;
          distance = d;
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// \ru Найти ближайший к кубу объект дерева в виде индекса в исходном массиве. \en Find the nearest tree object to a bounding box (as an index in initial array of objects).
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::FindNearestObject( const Cube & gabarit, double & distance, size_t & index, double eps ) const
{
  if ( (lowerBranch != nullptr) || (upperBranch != nullptr) || (midstBranch != nullptr) ) {
    double wMin, wMax;
    GetCubeMinMaxComponents( gabarit, 0.0, wMin, wMax );

    if ( (wMax < (minimum-distance)) || ((maximum+distance) < wMin) )
      return; // вне области / out of region
    
    if ( (wMin < (midst+distance)) && (lowerBranch != nullptr) ) {
      lowerBranch->FindNearestObject( gabarit, distance, index, eps );
    }
    if ( ((midst-distance) < wMax) && (upperBranch != nullptr) ) {
      upperBranch->FindNearestObject( gabarit, distance, index, eps );
    }
    if ( ((lower-distance) < wMax) && (wMin < (upper+distance)) && (midstBranch != nullptr) ) {
      midstBranch->FindNearestObject( gabarit, distance, index, eps );
    }
  }
  else { // содержимое конечной ветви // terminal branch content
    for ( size_t i = 0, iCount = leafObjects.size(); i < iCount; ++i ) {
      const ItemIndex & obj = leafObjects[i];
      if ( obj.first != nullptr ) {
        double d = obj.first->GetCube().DistanceToCube( gabarit, eps );
        if ( distance > d - eps ) {
          index = obj.second;
          distance = d;
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// \ru Выдать объекты дерева, расположенные к кубу ближе заданного расстояния. \en Get tree objects that are closer to the cube than the specified distance.
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::GetNearestObjects( const Cube & gabarit, const double distance, 
                                                                      std::vector<IndexDistance> & itemDistances,
                                                                      double eps ) const
{
  if ( (lowerBranch != nullptr) || (upperBranch != nullptr) || (midstBranch != nullptr) ) {
    double wMin, wMax;
    GetCubeMinMaxComponents( gabarit, 0.0, wMin, wMax );

    if ( (wMax < (minimum-distance)) || ((maximum+distance) < wMin) )
      return; // вне области / out of region
    
    if ( (wMin < (midst+distance)) && (lowerBranch != nullptr) ) {
      lowerBranch->GetNearestObjects( gabarit, distance, itemDistances, eps );
    }
    if ( ((midst-distance) < wMax) && (upperBranch != nullptr) ) {
      upperBranch->GetNearestObjects( gabarit, distance, itemDistances, eps );
    }
    if ( ((lower-distance) < wMax) && (wMin < (upper+distance)) && (midstBranch != nullptr) ) {
      midstBranch->GetNearestObjects( gabarit, distance, itemDistances, eps );
    }
  }
  else { // содержимое конечной ветви // terminal branch content
    for ( size_t i = 0, iCount = leafObjects.size(); i < iCount; ++i ) {
      const ItemIndex & obj = leafObjects[i];
      if ( obj.first != nullptr ) {
        double d = obj.first->GetCube().DistanceToCube( gabarit, eps );
        if ( distance > d - eps ) {
          IndexDistance itemDistance( obj.second, d );
          itemDistances.push_back( itemDistance );
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// \ru Найти ближайший к точке объект дерева. \en Find the nearest tree object to a point.
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::FindNearestObject( const Point & pnt, double & distance, const Type *& item, double eps ) const
{
  if ( (lowerBranch != nullptr) || (upperBranch != nullptr) || (midstBranch != nullptr) ) {
    double w = GetPointComponent( pnt );

    if ( (w < (minimum-distance)) || ((maximum+distance) < w) )
      return; // вне области / out of region

    if ( (w < (midst+distance)) && (lowerBranch != nullptr) ) {
      lowerBranch->FindNearestObject( pnt, distance, item, eps );
    }
    if ( ((midst-distance) < w) && (upperBranch != nullptr) ) {
      upperBranch->FindNearestObject( pnt, distance, item, eps );
    }
    if ( ((lower-distance) < w) && (w < (upper+distance)) && (midstBranch != nullptr) ) {
      midstBranch->FindNearestObject( pnt, distance, item, eps );
    }
  }
  else { // содержимое конечной ветви // terminal branch content
    for ( size_t i = 0, iCount = leafObjects.size(); i < iCount; ++i ) {
      const ItemIndex & obj = leafObjects[i];
      if ( obj.first != nullptr ) {
        double d = GetDistance( *obj.first, pnt, false );
        if ( distance > d - eps ) {
          item = obj.first;
          distance = d;
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Выдать ближайший к точке объект дерева
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::FindNearestObject( const Point & pnt, double & distance, size_t & index, double eps ) const
{
  if ( (lowerBranch != nullptr) || (upperBranch != nullptr) || (midstBranch != nullptr) ) {
    double w = GetPointComponent( pnt );

    if ( (w < (minimum-distance)) || ((maximum+distance) < w) )
      return; // вне области / out of region

    if ( (w < (midst+distance)) && (lowerBranch != nullptr) ) {
      lowerBranch->FindNearestObject( pnt, distance, index, eps );
    }
    if ( ((midst-distance) < w) && (upperBranch != nullptr) ) {
      upperBranch->FindNearestObject( pnt, distance, index, eps );
    }
    if ( ((lower-distance) < w) && (w < (upper+distance)) && (midstBranch != nullptr) ) {
      midstBranch->FindNearestObject( pnt, distance, index, eps );
    }
  }
  else { // содержимое конечной ветви // terminal branch content
    for ( size_t i = 0, iCount = leafObjects.size(); i < iCount; ++i ) {
      const ItemIndex & obj = leafObjects[i];
      if ( obj.first != nullptr ) {
        double d = GetDistance( *obj.first, pnt, false );
        if ( distance > d - eps ) {
          index = obj.second;
          distance = d;
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// \ru Выдать объекты дерева, расположенные к точке ближе заданного расстояния. \en Get tree objects that are closer to the point than the specified distance.
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::GetNearestObjects( const Point & pnt, const double distance, 
                                                                      std::vector<ItemDistance> & itemDistances,
                                                                      double eps ) const
{
  if ( (lowerBranch != nullptr) || (upperBranch != nullptr) || (midstBranch != nullptr) ) {
    double w = GetPointComponent( pnt );

    if ( (w < (minimum-distance)) || ((maximum+distance) < w) )
      return; // вне области / out of region

    if ( (w < (midst+distance)) && (lowerBranch != nullptr) ) {
      lowerBranch->GetNearestObjects( pnt, distance, itemDistances, eps );
    }
    if ( ((midst-distance) < w) && (upperBranch != nullptr) ) {
      upperBranch->GetNearestObjects( pnt, distance, itemDistances, eps );
    }
    if ( ((lower-distance) < w) && (w < (upper+distance)) && (midstBranch != nullptr) ) {
      midstBranch->GetNearestObjects( pnt, distance, itemDistances, eps );
    }
  }
  else { // содержимое конечной ветви // terminal branch content
    size_t iCount = leafObjects.size();
    itemDistances.reserve( itemDistances.size() + iCount );
    for ( size_t i = 0; i < iCount; ++i ) {
      const ItemIndex & obj = leafObjects[i];
      if ( obj.first != nullptr ) {
        double d = GetDistance( *obj.first, pnt, true );
        if ( distance > d - eps ) {
          ItemDistance itemDistance( obj.first, d );
          itemDistances.push_back( itemDistance );
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// \ru Выдать индексы объектов дерева, расположенные к точке ближе заданного расстояния. \en Get tree objects indices that are closer to the point than the specified distance.
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::GetNearestObjects( const Point & pnt, const double distance, 
                                                                      std::vector<IndexDistance> & itemDistances,
                                                                      double eps ) const
{
  if ( (lowerBranch != nullptr) || (upperBranch != nullptr) || (midstBranch != nullptr) ) {
    double w = GetPointComponent( pnt );

    if ( (w < (minimum-distance)) || ((maximum+distance) < w) )
      return; // вне области / out of region

    if ( (w < (midst+distance)) && (lowerBranch != nullptr) ) {
      lowerBranch->GetNearestObjects( pnt, distance, itemDistances, eps );
    }
    if ( ((midst-distance) < w) && (upperBranch != nullptr) ) {
      upperBranch->GetNearestObjects( pnt, distance, itemDistances, eps );
    }
    if ( ((lower-distance) < w) && (w < (upper+distance)) && (midstBranch != nullptr) ) {
      midstBranch->GetNearestObjects( pnt, distance, itemDistances, eps );
    }
  }
  else { // содержимое конечной ветви // terminal branch content
    size_t iCount = leafObjects.size();
    itemDistances.reserve( itemDistances.size() + iCount );
    for ( size_t i = 0; i < iCount; ++i ) {
      const ItemIndex & obj = leafObjects[i];
      if ( obj.first != nullptr ) {
        double d = GetDistance( *obj.first, pnt, true );
        if ( distance > d - eps ) {
          IndexDistance itemDistance( obj.second, d );
          itemDistances.push_back( itemDistance );
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// \ru Выдать индексы объектов дерева, расположенные к точке ближе заданного расстояния. \en Get tree objects indices that are closer to the point than the specified distance.
// ---
template <class Type, class Cube, class Point, class Vector>
inline void MbCubeTree<Type, Cube, Point, Vector>::GetNearestObjects( const Point & pnt, const double distance, 
                                                                      std::vector<size_t> & itemIndices,
                                                                      double eps ) const
{
  if ( (lowerBranch != nullptr) || (upperBranch != nullptr) || (midstBranch != nullptr) ) {
    double w = GetPointComponent( pnt );

    if ( (w < (minimum-distance)) || ((maximum+distance) < w) )
      return; // вне области / out of region

    if ( (w < (midst+distance)) && (lowerBranch != nullptr) ) {
      lowerBranch->GetNearestObjects( pnt, distance, itemIndices, eps );
    }
    if ( ((midst-distance) < w) && (upperBranch != nullptr) ) {
      upperBranch->GetNearestObjects( pnt, distance, itemIndices, eps );
    }
    if ( ((lower-distance) < w) && (w < (upper+distance)) && (midstBranch != nullptr) ) {
      midstBranch->GetNearestObjects( pnt, distance, itemIndices, eps );
    }
  }
  else { // содержимое конечной ветви // terminal branch content
    size_t iCount = leafObjects.size();
    itemIndices.reserve( itemIndices.size() + iCount );
    for ( size_t i = 0; i < iCount; ++i ) {
      const ItemIndex & obj = leafObjects[i];
      if ( obj.first != nullptr ) {
        double d = GetDistance( *obj.first, pnt, true );
        if ( distance > d - eps ) {
          itemIndices.push_back( obj.second );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Вспомогательная структура для элемента приоритетной очереди.
           \en Auxilliary structure for the priority queue element. \~
  \ingroup Base_Items
*/
// ---
template <class Type>
struct NodeElem
{
  const Type * _ptr; ///< \ru Указатель на узел дерева. \en Pointer to the tree node.
  double _weight;    ///< \ru Расстояние от куба узла до точки. \en Distance from the node bounding box to the point.
   
  /// \ru Конструктор. \en Constructor.
  NodeElem() : _ptr( nullptr ), _weight( MB_MAXDOUBLE ) {}
  /// \ru Конструктор. \en Constructor.
  NodeElem( const Type * ptr, double weigth ) : _ptr( ptr ), _weight( weigth ) {}
  /// \ru Конструктор копирования. \en Copy constructor.
  NodeElem( const NodeElem<Type> & that ) : _ptr( that._ptr ), _weight( that._weight ) {}
  /// \ru Деструктор. \en Destructor.
  ~NodeElem() {}
  /// \ru Оператор присваивания. \en An assignment operator. 
  NodeElem<Type> & operator = ( const NodeElem<Type> & that )
  {
    _ptr = that._ptr;
    _weight = that._weight;
    return *this;
  }
  /// \ru Оператор сравнения. \en Comparison operator. 
  bool operator < ( const NodeElem<Type> & that ) const
  {
    return _weight > that._weight;
  }
};


//-------------------------------------------------------------------------------
// \ru Выдать индекс объекта дерева, ближайшего к точке. \en Get tree objects index that are closest to the point.
// ---
template <class Type, class Cube, class Point, class Vector>
double  MbCubeTree<Type, Cube, Point, Vector>::FindNearestObject( const Point & pnt, const MbTreeDistanceToElementBase<Point> & calc, size_t & index ) const
{
  double dist2( MB_MAXDOUBLE );
  index = SYS_MAX_T;

  std::priority_queue< NodeElem<TypeTree> > queue;
  queue.push( NodeElem<TypeTree>( this, 0. ) );
  while ( !queue.empty() ) {
    const NodeElem<TypeTree> node = queue.top(); // Первым рассматриваем узлы, наиболее близкие к точке.
    queue.pop();

    double d2 = node._ptr->branchCube.DistanceToPoint2( pnt );
    if ( d2 > dist2 )
      break; // Все оставшиеся узлы дальше уже найденного расстояния.

    if ( ( node._ptr->lowerBranch != nullptr ) || ( node._ptr->upperBranch != nullptr ) || ( node._ptr->midstBranch != nullptr ) ) {
      if ( node._ptr->lowerBranch != nullptr ) {
        d2 = node._ptr->lowerBranch->branchCube.DistanceToPoint2( pnt );
        if ( d2 < dist2 )
          queue.push( NodeElem<TypeTree>( node._ptr->lowerBranch, d2 ) );
      }

      if ( node._ptr->upperBranch != nullptr ) {
        d2 = node._ptr->upperBranch->branchCube.DistanceToPoint2( pnt );
        if ( d2 < dist2 )
          queue.push( NodeElem<TypeTree>( node._ptr->upperBranch, d2 ) );
      }

      if ( node._ptr->midstBranch != nullptr ) {
        d2 = node._ptr->midstBranch->branchCube.DistanceToPoint2( pnt );
        if ( d2 < dist2 )
          queue.push( NodeElem<TypeTree>( node._ptr->midstBranch, d2 ) );
      }
    }
    else { // содержимое конечной ветви // terminal branch content
      for ( size_t i = 0, iCount = node._ptr->leafObjects.size(); i < iCount; ++i ) {
        const ItemIndex & obj = node._ptr->leafObjects[i];
        d2 = obj.first->DistanceToPoint2( pnt );
        if ( d2 < dist2 ) {
          d2 = calc.DistanceToElement2( pnt, obj.second );
          if ( d2 < dist2 ) {
            dist2 = d2;
            index = obj.second;
          }
        }
      }
    }
  }

  return dist2;
}


//-------------------------------------------------------------------------------
// \ru Выдать набор индексов объектов дерева, которые хотя бы частично лежат внутри заданной сферы. \en Get an array of tree object indices at least partially lying inside a given sphere.
// ---
template <class Type, class Cube, class Point, class Vector>
void MbCubeTree<Type, Cube, Point, Vector>::FindObjectsInsideSphere( const Point & pnt, double radius, const MbTreeDistanceToElementBase<Point> & calc, c3d::IndicesVector & indices ) const
{
  const double dist2( radius * radius );
  indices.clear();

  std::priority_queue< NodeElem<TypeTree> > queue;
  queue.push( NodeElem<TypeTree>( this, 0. ) );
  while ( !queue.empty() ) {
    const NodeElem<TypeTree> node = queue.top(); // Первым рассматриваем узлы, наиболее близкие к точке.
    queue.pop();

    double d2 = node._ptr->branchCube.DistanceToPoint2( pnt );
    if ( d2 > dist2 )
      break; // Все оставшиеся узлы дальше уже найденного расстояния.

    if ( ( node._ptr->lowerBranch != nullptr ) || ( node._ptr->upperBranch != nullptr ) || ( node._ptr->midstBranch != nullptr ) ) {
      if ( node._ptr->lowerBranch != nullptr ) {
        d2 = node._ptr->lowerBranch->branchCube.DistanceToPoint2( pnt );
        if ( d2 < dist2 )
          queue.push( NodeElem<TypeTree>( node._ptr->lowerBranch, d2 ) );
      }

      if ( node._ptr->upperBranch != nullptr ) {
        d2 = node._ptr->upperBranch->branchCube.DistanceToPoint2( pnt );
        if ( d2 < dist2 )
          queue.push( NodeElem<TypeTree>( node._ptr->upperBranch, d2 ) );
      }

      if ( node._ptr->midstBranch != nullptr ) {
        d2 = node._ptr->midstBranch->branchCube.DistanceToPoint2( pnt );
        if ( d2 < dist2 )
          queue.push( NodeElem<TypeTree>( node._ptr->midstBranch, d2 ) );
      }
    }
    else { // содержимое конечной ветви // terminal branch content
      for ( size_t i = 0, iCount = node._ptr->leafObjects.size(); i < iCount; ++i ) {
        const ItemIndex & obj = node._ptr->leafObjects[i];
        if ( calc.IsAllowed( obj.second ) ) {
          d2 = obj.first->DistanceToPoint2( pnt );
          if ( d2 < dist2 ) {
            d2 = calc.DistanceToElement2( pnt, obj.second );
            if ( d2 < dist2 )
              indices.push_back( obj.second );
          }
        }
      }
    }
  }
}

#endif
