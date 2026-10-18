//////////////////////////////////////////////////////////////////////////////////
/** 
\file
\brief \ru К-мерное дерево.
       \en K-d tree. \~

*/
// \ru К-мерное дерево -  это структура данных с разбиением пространства для упорядочивания точек из K-мерного пространства,
//     используемая для поиска k ближайших соседей.
// \en K-d tree is a space-partitioning data structure for organizing points in a k-dimensional space, 
//     using for the k-nearest neighbors (kNN) search 
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __MB_KDTREE_H
#define __MB_KDTREE_H

#include <mb_vector3d.h>
#include <mb_cube.h>
#include <tool_err_handling.h>
#include <queue>
#include <vector>

#if defined (C3D_WINDOWS) && !defined(ALL_WARNINGS)  // _MSC_VER // Set warnings level
#pragma warning(push)          // Preserve current state of warning settings
#pragma warning(disable: 4201) // warning C4201: nonstandard extension used: nameless struct/union
#endif


//-----------------------------------------------------------------------------
/** \brief \ru Очередь с приоритетом с использованием кучи.
           \en Priority queue using a heap. \~
  \details \ru Очередь с приоритетом с использованием кучи. Размер очереди фиксирован. 
               Производительность этой реализации выше чем у std::priority_queue. \n
           \en Priority queue using a heap. Size of queue is fixed. 
               This implementation perfomance is better in comparsion with std::priority_queue. \n \~
  \ingroup Base_Tools
*/
// ---
template <class Index, class Weight>
class PriorityQueue
{
protected:
  struct Element               ///< \ru Элемент очереди. \en Element of queue. 
  {
    Weight weight;             ///< \ru Вес элемента. \en Weight of element. 
    Index  index;              ///< \ru Индекс элемента. \en Index of element. 
  };
  Element * elements;          ///< \ru Элементы очереди. \en Elements of queue. 
  Element * offsetedElements;  ///< \ru Смещенные элементы очереди. \en Shifted elements of queue. 
  size_t    count;             ///< \ru Актуальное число элементов в очереди. \en Actual count of elements in the queue.
  size_t    maxSize;           ///< \ru Максимальное число элементов в очереди. \en Maximal count of elements in the queue.

protected:
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en The copy constructor without implementation prevents from copying by default. 
  PriorityQueue( const PriorityQueue & );

public:
  /// \ru Конструктор. \en Constructor. 
  PriorityQueue( );
  /// \ru Деструктор. \en Destructor. 
  ~PriorityQueue();

public:
  /** \ru \name Функции очереди с приоритетом.
      \en \name Functions of priority queue.
      \{ */
         /// \ru Инициализировать очередь максимальным количеством элементов в очереди. \en Initialize the queue by number of elements. 
         inline bool   Initialize( size_t _maxSize );
         /// \ru Получить количество элементов в очереди. \en Get elements count in the queue. 
         inline size_t ElementsCount() const { return count; }
         /// \ru Получить вес элемента очереди. \en Get weight of element in the queue. 
         inline Weight GetWeight( size_t i ) const { if ( i < count ) return elements[i].weight; return Weight(); }
         /// \ru Получить индекс элемента очереди. \en Get index of element in the queue. 
         inline Index  GetIndex( size_t i ) const { if ( i < count ) return elements[i].index; return Index(); }
         /// \ru Получить вес верхнего элемента очереди. \en Get weight of the top element in the queue. 
         inline Weight GetTopWeight() const { if ( count > 0 ) return elements[0].weight; return Weight(); }
         /// \ru Вставить элемент с заданным индексом и весом в очередь. \en Insert element with given index and weight in the queue. 
         inline void   Insert( Index index, Weight weight );
  /** \} */

private:
         // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation, to prevent an assignment by default. 
         void          operator = ( const PriorityQueue & );
};


//-----------------------------------------------------------------------------
/** \brief \ru К-d дерево.
           \en K-d tree. \~
  \details \ru К-мерное бинарное дерево. \n
           \en K-d binary tree. \n \~
  \ingroup Base_Tools
*/
// ---
template<class Scalar>
class KdTree {
public:
  struct Node {                   ///< \ru Структура узла дерева. \en Structure of node of the tree. 
    union {
      struct {                    ///< \ru Узел дерева. \en Node of tree. 
        Scalar splitValue;        ///< \ru Разделяющее значение. \en Split value. 
        size_t firstChildId : 24; ///< \ru Индекс первого потомка. \en Index of first child. 
        size_t dim : 2;           ///< \ru Измерение, вдоль которого происходит сравнение [0 = x, 1 = y, 2 = z]. \en The dimension along which node is splitted [0 = x, 1 = y, 2 = z]. 
        size_t type : 1;          ///< \ru Тип узла дерева (0 - узел, 1 - лист). \en Type of node (0 - node, 1 - leaf). 
      };
      struct {                    ///< \ru Лист дерева. \en Leaf of tree. 
        unsigned int start;       ///< \ru Индекс первого элемента листа. \en Index of first element of leaf. 
        unsigned short size;      ///< \ru Количество элементов листа. \en Number of elements in the leaf. 
      };
    };
  };

  typedef std::vector<Node>             NodeList;
  typedef std::vector<MbCartPoint3D>    PointList;
  typedef std::vector<size_t>           IndexList;
  typedef PriorityQueue<size_t, Scalar> ScalarPriorityQueue;

protected:
  MbCube     box;            ///< \ru Ограничивающий куб. \en Bounding box. 
  NodeList   nodes;          ///< \ru Узлы дерева. \en Nodes of tree. 
  PointList  points;         ///< \ru Множество точек. \en Set of points.
  IndexList  indices;        ///< \ru Индексы точек. \en Indices of points.
  size_t     targetCellSize; ///< \ru Минимальное количество точек в листе дерева. \en Minimal number of point in a tree leaf.
  size_t     targetMaxDepth; ///< \ru Максимальная глубина дерева. \en Maximal tree depth.
  size_t     numLevel;       ///< \ru Глубина дерева. \en  Tree depth.
  bool       isBalanced;     ///< \ru Сбалансированное дерево или нет. \en Three  is balanced or unbalanced.
                                          
  struct QueryNode {         ///< \ru Структура узла для запроса. \en Structure of query node.
    QueryNode() {}
    QueryNode( size_t id ) : nodeId( id ) {}
    size_t nodeId;           ///< \ru Id следующего узла. \en Id of next node.
    Scalar sq;               ///< \ru Квадрат расстояния до следующего узла. \en Squared distance to next node.
  };

public:
  /** 
  \brief \ru Конструктор.
         \en Constructor. \~
  \details \ru Конструктор k-мерного дерева.
           \en Constructor of k-d tree. \~
  \param[in] points - \ru Множество точек.
                      \en Set of points. \~
  \param[in] minLeafSize - \ru Минимальное количество точек в листе дерева (по умолчанию 16).
                           \en Minimal number of point in a tree leaf (16 by default). \~
  \param[in] maxDepth - \ru Максимальная глубина дерева (по умолчанию 64).
                        \en Maximal tree depth (64 by default). \~
  \param[in] balanced - \ru Создать сбалансированное дерево или нет (по умолчанию несбалансированное).
                        \en Create three balanced or unbalanced (unbalanced by default). \~
  */
  KdTree( const PointList & points, size_t minLeafSize = 16, size_t maxDepth = 64, bool balanced = false );
  /// \ru Деструктор. \en Destructor. 
  ~KdTree();

protected:
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en The copy constructor without implementation prevents from copying by default. 
  KdTree( const KdTree & );

public:
  /** \ru \name Функции очереди с приоритетом.
      \en \name Functions of priority queue.
  \{ */
         /** 
         \brief \ru Найти k ближайших соседей для данной точки.
                \en Performs the k-nearest neighbors (kNN) query. \~
         \details \ru Найти k ближайших соседей для данной точки. 
                  \en Performs the k-nearest neighbors (kNN) query. \~
         \param[in] queryPoint - \ru Точка, для которой ищутся соседи.
                                 \en The point for which the neighbors are being searched for . \~
         \param[in] neighborCount - \ru Запрашиваемое число соседей .
                                    \en Number of neighbors requested. \~
         \param[in] neighborQueue - \ru Очередь с результатами поиска, в котором верхний элемент является наиболее удаленным от заданной точки
                                    \en Queue with k-nearest neighbors (kNN) query results, where the topmost element [0] is NOT the nearest but the farthest \~
         */
         void                     GetKNearestNeighbors( const MbCartPoint3D & queryPoint, size_t neighborCount, ScalarPriorityQueue & neighborQueue );
         /**
         \brief \ru Найти ближайших соседей внутри сферы заданного радиуса для данной точки.
                \en Performs the query for neighbors inside given sphere. \~
         \details \ru Найти ближайших соседей внутри сферы заданного радиуса для данной точки.
                  \en Performs the query for neighbors inside given sphere. \~
         \param[in] queryPoint - \ru Точка, для которой ищутся соседи.
                                 \en The point for which the neighbors are being searched for . \~
         \param[in] radius - \ru Радиус сферы.
                             \en Sphere radius. \~
         \param[in] used - \ru Фильтр для точек. Значение true подразумевает, что точка исключается из рассмотрения.
                           \en Points filter. Value true implies that point is excluded from consideration. \~
         \param[out] neighbors - \ru Массив найденных соседей.
                                 \en Neigbors array. \~
         */
         void                     GetRadiusNeighbors( const MbCartPoint3D & queryPoint, double radius, const c3d::BoolVector & used, c3d::IndicesVector & neighbors );
         /// \ru Получить узлы дерева. \en Get tree nodes. 
         inline const NodeList &  GetNodes() { return nodes; }
         /// \ru Получить множество точек. \en Get points set. 
         inline const PointList & GetPoints() { return points; }
         /// \ru Получить индексы точек. \en Get indices of points. 
         inline const IndexList & GetIndices() { return indices; }
         /// \ru Получить глубину дерева. \en Get depth of tree. 
         inline size_t            GetNumLevel() { return numLevel; }
         /// \ru Получить ограничивающий куб. \en Get axis aligned bounding box. 
         inline const MbCube &    GetAxisAlignedBox() { return box; }
  /** \} */

private:
         // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation, to prevent an assignment by default. 
         void                     operator = ( const KdTree & );
         // Используется для построения дерева: разделить множество subset [start..end] согласно dim и splitValue,
         // и вернуть индекс первого элемента второго множества.
         size_t                   split( size_t start, size_t end, size_t dim, double splitValue );
         // Построение дерева.
         size_t                   createTree( size_t nodeId, size_t start, size_t end, size_t level );

};


////////////////////////////////////////////////////////////////////////////////
//
// \ru PriorityQueue - очередь с приоритетом. \en PriorityQueue - priority queue. 
//
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------
// \ru Конструктор очереди с приоритетом. \en Constructor of priority queue.
// ---
template <class Index, class Weight>
PriorityQueue<Index, Weight>::PriorityQueue()
  : elements( 0 )
  , maxSize ( 0 )
{
}


//-------------------------------------------------------------------------------
// \ru Деструктор очереди с приоритетом. \en Destructor of priority queue.
// ---
template <class Index, class Weight>
PriorityQueue<Index, Weight>::~PriorityQueue()
{
  if ( elements )
    delete[] elements;
}


//-------------------------------------------------------------------------------
// \ru Инициализировать очередь максимальным количеством элементов в очереди. \en Initialize the queue by number of elements.
// ---
template <class Index, class Weight>
inline bool PriorityQueue<Index, Weight>::Initialize( size_t _maxSize )
{
  if ( maxSize != _maxSize ) {
    maxSize = _maxSize;
    delete[] elements;
    try {
      elements = new Element[maxSize];
    }
    catch ( ... ) {
      elements = nullptr;
      maxSize = count = 0;
      C3D_CONTROLED_THROW;
      return false;
    }
    offsetedElements = ( elements - 1 );
  }
  count = 0;
  return true;
}


//-------------------------------------------------------------------------------
// \ru Вставить элемент с заданным индексом и весом в очередь. \en Insert element with given index and weight in the queue.
// ---
template <class Index, class Weight>
inline void PriorityQueue<Index, Weight>::Insert( Index index, Weight weight )
{
  if ( count == maxSize ) {
    if ( weight < elements[0].weight ) {
      size_t j, k;
      j = 1;
      k = 2;
      while ( k <= maxSize ) {
        Element* z = &( offsetedElements[k] );
        if ( (k < maxSize) && (z->weight < offsetedElements[k+1].weight) )
          z = &( offsetedElements[++k] );

        if( weight >= z->weight )
          break;
        offsetedElements[j] = *z;
        j = k;
        k = 2 * j;
      }
      offsetedElements[j].weight = weight;
      offsetedElements[j].index = index;
    }
  }
  else {
    size_t i, j;
    i = ++count;
    while ( i >= 2 ) {
      j = i >> 1;
      Element& y = offsetedElements[j];
      if( weight <= y.weight )
        break;
      offsetedElements[i] = y;
      i = j;
    }
    offsetedElements[i].index = index;
    offsetedElements[i].weight = weight;
  }
}


////////////////////////////////////////////////////////////////////////////////
//
// \ru К-d дерево. \en PriorityQueue - priority queue. 
//
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------
// \ru Конструктор дерева. \en K-d tree.
// ---
template<class Scalar>
KdTree<Scalar>::KdTree( const PointList & _points, size_t nofPointsPerCell, size_t maxDepth, bool balanced )
  : points ( _points.size() )
  , indices( _points.size() ) 
{
  points[0] = _points[0];
  box.SetEmpty();
  for( size_t i = 1; i < points.size(); ++i ) {
    points[i] = _points[i];
    indices[i] = i;
    box |= ( points[i] );
  }

  targetMaxDepth = maxDepth;
  targetCellSize = nofPointsPerCell;
  isBalanced = balanced;

  // Добавление первого узла. Остальные добавляются при вызове createTree (рекурсия).
  nodes.resize( 1 );
  nodes.back().type = 0;
  numLevel = createTree( 0, 0, points.size(), 1 );
}


//-------------------------------------------------------------------------------
// \ru Деструктор дерева. \en Destructor of tree. 
// ---
template<class Scalar>
KdTree<Scalar>::~KdTree()
{
}


//-------------------------------------------------------------------------------
// \ru Найти k ближайших соседей для данной точки. Результат операции хранится 
//     в виде стека neighborQueue, в котором верхний элемент является наиболее
//     удаленным от заданной точки.
//     (его содержимое не сортировано, но упорядоченно расположено в куче)
// \en Performs the k-nearest neighbors (kNN) query. The result of the query, 
//     the k-nearest neighbors, are stored into the stack neighborQueue, where the 
//     topmost element [0] is NOT the nearest but the farthest!
//     (they are not sorted but arranged into a heap)
// ---
template<class Scalar>
void KdTree<Scalar>::GetKNearestNeighbors( const MbCartPoint3D & queryPoint, size_t k, ScalarPriorityQueue & neighborQueue ) 
{
  if ( !neighborQueue.Initialize( k ) )
    return;

  std::vector<QueryNode> nodeStack( numLevel + 1 );
  nodeStack[0].nodeId = 0;
  nodeStack[0].sq = 0.0;
  size_t count = 1;

  while( count ) {
    QueryNode & qnode = nodeStack[count - 1]; // Последний вставленный в стек узел.

    // При проходе вниз по дереву qnode.nodeId является ближайшим поддеревом, т.е.
    // qnode.nodeId является другим поддеревом, которое будет обрабатываться,
    // если фактический ближайший узел будет больше, чем делящее расстояние (split distance).
    Node & node = nodes[qnode.nodeId]; // 

    // Если расстояние меньше чем верхний элемент neighborQueue, то это может быть один из k ближайших соседей.
    if( neighborQueue.ElementsCount() < k ||  neighborQueue.GetTopWeight() ) {
      if( node.type ) { // Достигли листа дерева.
        --count;

        // Индекс последнего элемента листа в points.
        size_t end = node.start + node.size;
        // Добавление элемента листа в очередь.
        for( size_t i = node.start; i < end; ++i )
          neighborQueue.Insert( indices[i], ( queryPoint - points[i] )*( queryPoint - points[i] )  );
      }
      else { // Не лист дерева.
        //  Расстояние между найденной точкой и фактической координатой деления.
        double new_off = queryPoint[node.dim] - node.splitValue;

        // Левое поддерево.
        if( new_off < 0. ) {
          nodeStack[count].nodeId = node.firstChildId;
          // В родительском nodeId хранится индекс другого поддерева (для прохода в обратном направлении).
          qnode.nodeId = node.firstChildId + 1;
        }
        // Правое поддерево.
        else {
          nodeStack[count].nodeId = node.firstChildId + 1;
          qnode.nodeId = node.firstChildId;
        }
        // Расстояние наследуется от родителя(при спуске по дереву оно равно 0).
        nodeStack[count].sq = qnode.sq;
        // Расстояние от родителя - это квадрат расстояния от плоскости деления.
        qnode.sq = new_off * new_off;
        ++count;
      }
    }
    else {
      --count;
    }
  }
}


//-------------------------------------------------------------------------------
// \ru Найти ближайших соседей внутри сферы заданного радиуса для данной точки.
//     в виде неупорядоченного массива neighbors
// \en Performs the query for neighbors inside given sphere.
//     The result of the query is an unsorted array neighbors.
// ---
template<class Scalar>
void KdTree<Scalar>::GetRadiusNeighbors( const MbCartPoint3D & queryPoint, double radius, const c3d::BoolVector & used, c3d::IndicesVector & neighbors )
{
  neighbors.clear();
  const auto r2 = radius * radius;

  c3d::IndicesVector nodeStack;
  nodeStack.reserve( numLevel + 1 );
  nodeStack.push_back( 0 );

  while ( !nodeStack.empty() ) {
    Node & node = nodes[nodeStack.back()];
    nodeStack.pop_back();
    if ( node.type ) { // Достигли листа дерева.
      // Индекс последнего элемента листа в points.
      size_t end = node.start + node.size;
      // Добавление элемента листа в очередь.
      for ( size_t i = node.start; i < end; ++i ) {
        if ( !used[indices[i]] ) {
          const auto d2 = ( queryPoint - points[i] ).Length2();
          if ( d2 < r2 )
            neighbors.push_back( indices[i] );
        }
      }
    }
    else { // Не лист дерева.
        //  Расстояние между найденной точкой и фактической координатой деления.
      const double new_off = queryPoint[node.dim] - node.splitValue;
      const double new_off2 = new_off * new_off;

      // Левое поддерево.
      if ( new_off < 0. ) {
        nodeStack.push_back( node.firstChildId );
        if( new_off2 <= r2 )
          nodeStack.push_back( node.firstChildId + 1);
      }
      // Правое поддерево.
      else {
        nodeStack.push_back( node.firstChildId + 1 );
        if ( new_off2 <= r2 )
          nodeStack.push_back( node.firstChildId );
      }
    }
  }
}


//-------------------------------------------------------------------------------
// \ru Разделить часть массива между индексами start и end на две части, одна из которых меньше  
//     чем splitValue, другое с элементами больше или равными чем splitValue. Сравнение элементов
//     производится с помощью координаты dim [0 = x, 1 = y, 2 = z].
// \en Split the array part between start and end in two part, one with the elements less than splitValue,
//     the other with the elements greater or equal than splitValue. The elements are compared
//     using the "dim" coordinate [0 = x, 1 = y, 2 = z].
// ---
template<class Scalar>
size_t KdTree<Scalar>::split( size_t start, size_t end, size_t dim, double splitValue ) 
{
  size_t l( start ), r( end - 1 );
  for( ; l < r; ++l, --r ) {
    while( l < end && points[l][dim] < splitValue )
      l++;
    while( r < SYS_MAX_T && r >= start && points[r][dim] >= splitValue )
      r--;
    if( l > r || r == SYS_MAX_T )
      break;
    std::swap( points[l], points[r] );
    std::swap( indices[l], indices[r] );
  }
  return ( points[l][dim] < splitValue ? l + 1 : l ); // Вернуть индекс первого элемента во второй части.
}


//-------------------------------------------------------------------------------
// \ru Построить k-мерное дерево (рекурсивно). Если количество точек узла меньше чем
//     targetCellsize, то делаем этот узел листом, иначе рассчитываем ограничивающий 
//     куб для точек узла и делим его по среднему значению наибольшего габарита куба.
// \en Build the kdtree recursively. If the number of points in the node is lower than
//     targetCellsize then mark this node as leaf, else compute the bounding box of the points 
//     of the node and split it at the middle of the largest bounding box dimension.
// ---
template<class Scalar>
size_t KdTree<Scalar>::createTree( size_t nodeId, size_t start, size_t end, size_t level ) 
{
  Node & node = nodes[nodeId]; // Первый узел
  MbCube cube;

  for( size_t i = start + 1; i < end; ++i )
    cube |= points[i] ;


  MbVector3D diag = cube.pmax - cube.pmin; // Диагональ габаритного куба.

  size_t dim;
  if( diag.x > diag.y )
    dim = diag.x > diag.z ? 0 : 2;
  else
    dim = diag.y > diag.z ? 1 : 2;

  node.dim = dim;
  if( isBalanced ) // Разделить точки используя среднее значение вдоль направления dim.
  {
    std::vector<Scalar> tempVector;
    for( size_t i = start + 1; i < end; ++i )
      tempVector.push_back( (points[i])[dim] );
    std::sort( tempVector.begin(), tempVector.end() );
    node.splitValue = ( tempVector[int(tempVector.size() / 2.0)] + tempVector[int(tempVector.size() / 2.0) + 1] ) / 2.0;
  }
  else // Разделить ограничивающий куб на две части на основе среднего значение вдоль направления dim.
    node.splitValue = Scalar( 0.5*( cube.pmax[dim] + cube.pmin[dim] ) );

  size_t midId = split( start, end, dim, node.splitValue ); // Индекс первого элемента во второй части.

  node.firstChildId = nodes.size();
  nodes.resize( nodes.size() + 2 );
  bool flag = ( midId == start ) || ( midId == end );
  size_t leftLevel, rightLevel;
  {
    // Левый потомок.
    size_t childId = nodes[nodeId].firstChildId;
    Node & child = nodes[childId];
    if( flag || ( midId - start ) <= targetCellSize || level >= targetMaxDepth ) {
      child.type = 1;
      child.start = (unsigned int)start;
      child.size = (unsigned short)(midId - start);
      leftLevel = level;
    }
    else {
      child.type = 0;
      leftLevel = createTree( childId, start, midId, level + 1 );
    }
  }

  {
    // Правый потомок.
    size_t childId = nodes[nodeId].firstChildId + 1;
    Node & child = nodes[childId];
    if( flag || ( end - midId ) <= targetCellSize || level >= targetMaxDepth ) {
      child.type = 1;
      child.start = (unsigned int)midId;
      child.size = (unsigned short)(end - midId);
      rightLevel = level;
    }
    else {
      child.type = 0;
      rightLevel = createTree( childId, midId, end, level + 1 );
    }
  }
  if( leftLevel > rightLevel )
    return leftLevel;
  return rightLevel;
}

#if defined (C3D_WINDOWS) && !defined(ALL_WARNINGS) // _MSC_VER // Set warnings level
#pragma warning(pop) // Restore state of warning settings
#endif

#endif //__MB_KDTREE_H