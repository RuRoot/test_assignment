//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file     
  \brief \ru Обобщенные алгоритмы на графах.
         \en Generic graph algorithms. \~
*/
/////////////////////////////////////////////////////// MA 25.10.2010 ///////// C3D API///

#ifndef __GRAPH_ALGORITHMS_H
#define __GRAPH_ALGORITHMS_H
//
#include <generic_utility.h>
#include <vector>

/**
  \addtogroup Base_GraphLib
  \{
*/

//----------------------------------------------------------------------------------------
/// Пустой посетитель алгоритма обхода графа в глубину.
/**  
  \attention  Класс не предназначен для того, что бы применять статический или динамический 
              полиморфизм, т.е. не обязывает своих наследников перегружать методы.
*/
//---
template<class Graph>
struct DefaultDFSVisitor
{   
  using vertex = typename Graph::vertex;

  /// Встретили "обратное" ребро (дуга, если орграф) dfs-дерева.
  /**
    Вызывается когда при посещении вершины v найдено исх.ребро, направленное к 
    ранее посещенной вершине. Другими словами, вершина u является предком 
    вершине v в dfs-дереве.
  */
  void  BackEdge( vertex /*v*/, vertex /*u*/, const Graph & /*g*/ ) {}
  /// Вызывается, когда впервые проходим через исходящую дугу v->u, вершину u еще не посещали
  void  ExamineEdge( vertex /*v*/, vertex /*u*/, const Graph & /*g*/ ) {}
  /// Посещение вершины: Вызывается один раз для каждой вершины, когда она впервые начинает просматриваться
  void  DiscoverNode( vertex /*v*/, const Graph & /*g*/ ) {}
  /// Вершина рассмотрена: Означает, что все исходящие ребра вершины рассмотрены
  void  FinishNode( vertex /*v*/, const Graph & /*g*/ ) {}
  /// Встретили "поперечное" или "прямое" ребро 
  /** 
    Вызывается, когда находим дугу, идущую к другому dfs-дереву, либо прямую дугу, 
    идущую к потомку того же дерева, имеющему два и более  отцов.
    Для поперечного ребра вызывается только для ориентированных графов.
  */
  void  ForwardOrCrossEdge( vertex /*v*/, vertex /*u*/, const Graph & /*g*/ ) {}
  /// Отвечает, что вершина исключена из рассмотрения
  bool  Ignored( vertex /*v*/, const Graph & /*g*/ ) const { return false; }
  /// Означает, что начато рассмотрение корневой вершины будущего дерева обхода
  void  StartNode( vertex /*v*/, const Graph & /*g*/ ) {}
  /// Ребро стало "древесным" (принадлежит dfs-дереву). Вызывается перед переходом от посещенной вершины v к еще не посещенной вершине u
  void  TreeEdge( vertex /*v*/, vertex /*u*/, const Graph & /*g*/ ) {}
};


//////////////////////////////////////////////////////////////////////////////////////////
//
/// Посетитель алгоритма поиска блоков и точек сочленения в неориентированном графе.
/**
  Позволяет настроить алгоритм поиска блоков и точек сочленения под конкретные реализации.
*/
//////////////////////////////////////////////////////////////////////////////////////////

template< class Graph >
struct DefaultBicompVisitor
{
  /// Найден блок, как последовательность ребер
  template<class EdgeIterator>
  void BlockFounded(EdgeIterator, EdgeIterator, const Graph&) {}

  /// Обнаружена точка сочленения (articulation vertex)
  template<class Vertex>
  void CutNode(Vertex, const Graph&) {}

  /// Функция обратного вызова: Фильтрация для точек сочленения
  /**
    С момощью этой функции пользователь настраивает поведение алгорита поиска блоков.
    Если визитер отвечает true, то алгоритм не учитывает данную вершину, 
    как вершину разреза, отделяющую блоки. Таким образом в результате 
    отфильтрованная  точка сочленения всегда будет принадлежать одному блоку.
  */
  template<class Vertex>
  bool IsFilteredCut(Vertex, const Graph&) const { return false; }
};


//////////////////////////////////////////////////////////////////////////////////////////
//
/// Посетитель обхода в глубину для поиска блоков и точек сочленения.
/**
  Класс является автономным и не нуждается в уточнении наследованием от него.
  Graph - предполагается, что это неориентированный граф.
  BicompVisitor - надстроенный визитер, посетитель этого визитера, который реализует 
                  события обнаружения блока, точки сочленения и фильтрацию вершин, которые 
                  принудительно запрещается быть точками сочленения.
*/
//////////////////////////////////////////////////////////////////////////////////////////

template< class Graph, class BicompVisitor = DefaultBicompVisitor<Graph> >
class BicompDFSVisitor final: public DefaultDFSVisitor<Graph>
{
public:
  using adjacency_iterator= typename Graph::adjacency_iterator;
  using vertex = typename Graph::vertex;
  using edge = typename Graph::edge;

public:
  BicompDFSVisitor(BicompVisitor& vis)
    : m_graph( nullptr )
    , m_bicompVis( vis )
    , m_dfsCounter( 1 )
    , num()
    , father()
    , lval()
    , m_stackEdges()
  {}  
  
  /// Встретили поперечное или прямое ребро.
  void  ForwardOrCrossEdge(vertex v, vertex u, const Graph&) 
  { 
    C3D_UNUSED_PARAMETER( u );
    C3D_UNUSED_PARAMETER( v );
    PRECONDITION( num[v] < num[u] ); 
  }

  /// Найдено обратное ребро dfs-дерева, вызывается когда при посещении вершины v найдено исх.ребро к ранее посещенной вершине
  /**
    Вершина u является предком вершине v в dfs-дереве.
  */
  void  BackEdge(vertex v, vertex u, const Graph& g)
  {
    C3D_UNUSED_PARAMETER( g );
    PRECONDITION( m_graph == &g );    
    PRECONDITION( num[u] < num[v] );
    PRECONDITION( father[v] != g.NullVertex() );
    if ( u != father[v] )
    {
      // Здесь vu - есть обратное ребро входящее в вершину u, которая выше, чем v в d-дереве.      
      m_stackEdges.push_back( edge(v,u) );  // вставить ребро vu.
      lval[v] = min_of( lval[v], num[u] );  // см.лемму 6.
    }
  }
  
  /// Посещение вершины: Вызывается один раз для каждой вершины, когда она впервые начинает просматриваться.
  void  DiscoverNode(vertex v, const Graph& g) 
  {
    C3D_UNUSED_PARAMETER( g );
    C3D_ASSERT( m_graph == &g );
    PRECONDITION( num[v] == 0 );
    PRECONDITION( lval[v] == 0 );    
    num[v] = lval[v] = m_dfsCounter++;
  }

  /// Вершина рассмотрена: Означает, что все исходящие ребра вершины рассмотрены.
  void  FinishNode(vertex u, const Graph& g)
  {
    PRECONDITION( m_graph == &g );
    vertex v = father[u];

    // СЛУЧАЙ 1: Вершина v - корневая, завершен обход fds-дерева.
    if ( v == g.NullVertex() ) 
    {
      // Оценить является ли u - точкой сочленения
      // Сколько раз стартовая вершина стала папой (столько же в ней стыкуется блоков)
      if ( NumChildren(u, g) > 1)
      {
        // В корневой вершине стыкуются 2 или более блоков - значит она же является и точкой сочленения.
        m_bicompVis.CutNode( u, g );
      }
      // Извещение о найденном блоке
      if ( !m_stackEdges.empty() ) // Все что есть в m_stackEdges - следует считать последним найденным блоком.
      {
        m_bicompVis.BlockFounded(m_stackEdges.begin(), m_stackEdges.end(), g);
        // После извещения визитера - вычищаем стек
        m_stackEdges.clear();
      }
      return;
    }

    // СЛУЧАЙ 2: u - не корневая вершина.    
    {      
      lval[v] = min_of( lval[v], lval[u] ); // см. лемму 6;
      if ( lval[u] >= num[v] )
      {
        //  Здесь можно получить новый блок, для чего достаточно вытолкнуть из 
        //  стека все ребра, включая ребро vu.
        
        // (!) Если вершина v не корень d-дерева, то можно утверждать, что она - есть точка сочленения;
        //     См. теорему 8.2. [М.О.Асанов]
        if ( father[v] != g.NullVertex() ) // если v корневая вершина, то оценки для неё делаются в конце обхода дерева
        {
          m_bicompVis.CutNode( v, *m_graph );
        }
        
        // Извещение о найденном блоке
        if ( !m_bicompVis.IsFilteredCut(v,*m_graph) ) // Запрет на отфильтрованные точки сочленения - они не могут "вырезать" блоки.
        {
          // Тут мы запретили собирать блок, т.к. вершина фильтрованная, однако это не принесет ущерба, 
          // если окажется что v - не точка сочленения. Вот почему:
          /*
            Если v - есть корень DFS-дерева, то возможны 2 варианта: 
              i) v принадлежит одному блоку, тогда v не точка сочленения; 
              ii) v принадлежит двум и более блокам, тогда v - есть точка сочленения.
            В первом случае единственный блок, куда включена v, будет собран в конце текущего обхода dfs-дерева, 
            массив m_stackEdges полностью будет содержать этот блок. Во втором случае, если блок не 
            единственный, то v - есть точка сочленения, тогда очевидно запрет правомерен - в конце обхода дерева все, 
            что осталось в стеке ребер есть один блок.
          */
          
          PRECONDITION( !m_stackEdges.empty() );          
          const edge seek( v, u );
          typename std::vector<edge>::iterator first = m_stackEdges.begin();
          typename std::vector<edge>::iterator iter, last;          
          for ( iter = last = m_stackEdges.end(); iter != first; )          
          { 
            --iter;
            if ( *iter == seek )
            {
              break;
            }
          }
          
          PRECONDITION( *iter == seek ); // Это ребро обязано быть в стеке
          m_bicompVis.BlockFounded( iter, last, *m_graph );
          // После извещения визитера - вычищаем блок из стека конца
          m_stackEdges.erase( iter, last );
        }         
      }
    }
  }

  /// Означает, что начато рассмотрение корневой вершины будущего дерева обхода
  void StartNode(vertex v, const Graph& g)
  {
    C3D_UNUSED_PARAMETER( v );
    InitVisitor( g );
    PRECONDITION( father[v] == g.NullVertex() );
    PRECONDITION( num[v] == 0 &&  lval[v] == 0 );
  }

  /// Заход в ребро dfs-дерева, вызывается перед переходом от посещенной вершины v к еще не посещенной вершине u
  void TreeEdge(vertex v, vertex u, const Graph& g)
  {
    C3D_UNUSED_PARAMETER( g );
    PRECONDITION( m_graph == &g );
    PRECONDITION( father[u] == g.NullVertex() );
    m_stackEdges.push_back( edge(v,u) );  // Вставить ребро vu;    
    father[u] = v;  // зафиксируем отца для вершины u;
  }

private:
  // Количество сыновей вершины.
  size_t NumChildren(vertex u, const Graph& g) const
  {
    PRECONDITION( m_graph == &g );
    // Оценить является ли u - точкой сочленения
    size_t fatherNb = 0; // Сколько раз  вершина u стала папой    
    for ( vertex adjNode: g.AdjacentVertices(u) )
    {
      if ( father[adjNode] == u )
      {
        ++fatherNb;
      }
    }
    return fatherNb;
  }
  void InitVisitor(const Graph& graph)
  {
    m_graph = &graph;    
    m_dfsCounter = 1;
    InitPropertyMap(num, graph, 0);
    InitPropertyMap(father, graph, graph.NullVertex());
    InitPropertyMap(lval, graph, 0);
    m_stackEdges.clear();
  }

private:
  using NumVertexMap = typename property_map<Graph,size_t>::type;
  using FatherVertexMap = typename property_map<Graph,vertex>::type;

  const Graph*    m_graph;      ///< Рассматриваемый граф, для которого ищутся точки сочленения.
  BicompVisitor&  m_bicompVis;  ///< Посетитель алгоритмов этого класса.
  ptrdiff_t       m_dfsCounter; ///< Счетчик вершин dfs-дерева.
  NumVertexMap    num;    ///< Нумерация порядка обхода вершин d-дерева.
  NumVertexMap    lval;   ///< Массив значений функции L[v] на каждую вершину - см.теорию стр.166, [Asan], Лемма 6.
  FatherVertexMap father; ///< Отец вершины в dfs-дереве.
  std::vector<edge> m_stackEdges; ///< Стек ребер для обслуживания нахождения блоков.

private:
  BicompDFSVisitor & operator= ( const BicompDFSVisitor& );
};

//----------------------------------------------------------------------------------------
// Стековый элемент для алгоритма обхода в глубину.
// ---
template<class Graph>
struct DFSVertexInfo
{
private:
  using vertex = typename Graph::vertex;
  using adjacency_iterator = typename Graph::adjacency_iterator;

public:
  vertex m_node;
  adjacency_iterator m_iter, m_last;

  DFSVertexInfo( vertex v, adjacency_iterator iter, adjacency_iterator last )
    : m_node( v )
    , m_iter( iter )
    , m_last( last )
  {}

  DFSVertexInfo( vertex v, const Graph & graph )
    : m_node( v )
    , m_iter()
    , m_last()
  {
    c3d::tie(m_iter,m_last) = graph.AdjacentVertices( v ); 
  }

  DFSVertexInfo( const DFSVertexInfo & vi )
    : m_node( vi.m_node )
    , m_iter( vi.m_iter )
    , m_last( vi.m_last )
  {}
  
  DFSVertexInfo & operator = ( const DFSVertexInfo & vi )
  {      
    m_node = vi.m_node;
    m_iter = vi.m_iter;
    m_last = vi.m_last;
    return *this;
  }  
};

//----------------------------------------------------------------------------------------
/// Алгоритм обхода в глубину графа смежности.
/**
  Вычислительная сложность алгоритма практически линейная, если считать что 
  методы визитера выполняются за константное время.

  \param graph Граф смежных вершин.
  \param vis Посетитель алгоритма.
*/
//---
template<class Graph, class Visitor, class ColourMap>
void DepthFirstSearch(const Graph& graph, Visitor& vis, ColourMap& colourMap)
{  
  using vertices_size_t = typename Graph::vertices_size_t;
  using vertex_t = typename Graph::vertex;
  using adjacency_iterator = typename Graph::adjacency_iterator;

  std::vector<DFSVertexInfo<Graph>> stack;

  // Preliminary marking of graph vertices.
  for (vertex_t v: graph.Vertices())
  {
    colourMap[v] = vis.Ignored(v,graph) ? black_color : white_color;
  }

  adjacency_iterator vIter, vLast;  

  for (vertex_t startNode: graph.Vertices())
  {
    if ( colourMap[startNode] == white_color ) // Начинаем с x-вершины.
    {      
      colourMap[startNode] = gray_color;
      vis.StartNode( startNode, graph );

      vis.DiscoverNode( startNode, graph );
      stack.push_back( DFSVertexInfo<Graph>(startNode,graph) );

      while ( !stack.empty() )
      {
        vertex_t srcNode;
        {
          DFSVertexInfo<Graph> & curr = stack.back();
          vIter = curr.m_iter;
          vLast = curr.m_last;
          srcNode = curr.m_node;
          stack.pop_back();
        }
        
        while (vIter != vLast)
        {
          const vertex_t trgNode = *vIter;
          ++vIter;

          vis.ExamineEdge(srcNode, trgNode, graph);

          switch (colourMap[trgNode]) // Переход по дереву к следующей вершине.
          {
          case white_color:
            {
              vis.TreeEdge( srcNode, trgNode, graph ); // "древесное" ребро.
              colourMap[trgNode] = gray_color;
              stack.push_back( DFSVertexInfo<Graph>(srcNode, vIter, vLast) );
              vis.DiscoverNode( srcNode = trgNode, graph );
              c3d::tie( vIter, vLast ) = graph.AdjacentVertices(srcNode);
              break;
            }
          case gray_color: // Встетили обратное ребро.
            {
              vis.BackEdge(srcNode, trgNode, graph);
              break;
            }
          default: // Встретили "прямое" или "кросс-ребро" в ориентированном графе.
            {
              vis.ForwardOrCrossEdge(srcNode, trgNode, graph);
              break;
            }
          }
        }
        
        // Событие завершения обхода текущей вершины.
        colourMap[srcNode] = black_color;
        vis.FinishNode(srcNode, graph);
      }
    }
  }
}

//----------------------------------------------------------------------------------------
// Обход на фиксированную в глубину от стартовой вершины root с посещением вершин не однократно.
/*
  DFS-функция изначально создавалась для поиска циклов, включающих до N вершин. Т.к. максимальная 
  глубина поиска известна, рекурсия раскрывается на этапе компиляции.
  Если FinColor = white_color, то обход в глубину можно применять для выявления всех циклов 
                               длиной до N, однако с неоднократным посещением каждой вершины.
  Если FinColor = black_color, тогда получим классический обход на глубину не более N узлов 
                               с однократным посещением узлов графа..

*/
//---
template<size_t N, color_code FinColor>
struct dfs_fixed_from_anode
{
  template<class Graph, class Node, class ColorMap, class Visitor >
  dfs_fixed_from_anode( const Graph & graph, Node root, ColorMap & colorMap, Visitor & vis )
  {
    typename Graph::adjacency_iterator vIter, vLast;
    colorMap[root] = gray_color;
    vis.DiscoverNode( root, graph );
    c3d::tie( vIter, vLast ) = graph.AdjacentVertices( root );
    for ( ; vIter!=vLast; ++vIter )
    {      
      const color_code cVal = colorMap[*vIter];
      switch ( cVal )
      {
        case white_color:
          vis.ExamineEdge( root, *vIter, graph );
          dfs_fixed_from_anode<N-1,FinColor>( graph, *vIter, colorMap, vis );
          break;
        case gray_color:
          vis.BackEdge( root, *vIter, graph ); // The cycle is found.
          break;
        default:
          break;
      }
    }
    // black_color: set the color to avoid the visiting again.
    // white_color: reset the color label to visit it again.
    colorMap[root] = FinColor;  
    vis.FinishNode( root, graph );
  }
};

//----------------------------------------------------------------------------------------
// Stop recursion
//---
template<color_code FinColor>
struct dfs_fixed_from_anode<0,FinColor>
{
  template<class Graph, class Node, class ColorMap, class Visitor >
  dfs_fixed_from_anode( const Graph &, Node, ColorMap &, Visitor & ) {}
};

//----------------------------------------------------------------------------------------
// Обход всех маршрутов в графе длинной не более N (применяется для выявления циклов и не только..)
/*
  В отличие от классического алгоритма DFS каждая вершина посещается не единожды. Однако мы 
  не ожидаем сильного замедления благодаря ограничению на глубину ветки обхода.
*/
//---
template<size_t N, class Graph, class ColorMap, class Visitor >
void dfs_fixed_depth( const Graph & graph, ColorMap & colorMap, Visitor & vis )
{
  typedef typename Graph::vertex_iterator vertex_iterator;
  typedef typename Graph::vertex          vertex;
 
  vertex_iterator vIter, vLast;
  // Пометить пропускаемые вершины
  for ( c3d::tie(vIter,vLast) = graph.Vertices(); vIter!=vLast; ++vIter )
  {
    if ( vis.Ignored(*vIter,graph) )
    {
      colorMap[*vIter] = black_color;
    }
  }  
  // Обход всех циклов длиной N
  for ( c3d::tie(vIter,vLast) = graph.Vertices(); vIter!=vLast; ++vIter )
  {
    if ( colorMap[*vIter] == white_color )
    {      
      vis.StartNode( *vIter, graph );
      dfs_fixed_from_anode<N,white_color>( graph, *vIter, colorMap, vis );
      colorMap[*vIter] = black_color; // the node is labeled as visited and will not be visited again.
    }
  }  
}

//////////////////////////////////////////////////////////////////////////////////////////
//
/// Отображение реберных свойств для графов, поддерживающих концепцию смежности вершин (без явных ребер)
/**
  Для графов с инцидентными ребрами лучше использовать другие типы отображений
*/
//////////////////////////////////////////////////////////////////////////////////////////
/*
template<class Graph, class Property>
class EdgePropertyMap
{
  typedef Graph::vertex_descriptor        vertex_descriptor;
  typedef Graph::edge_descriptor          edge_descriptor;
  typedef std::pair<edge_descriptor,Property> pair;
  class node
  {    
  public:
    node( const node & );
    node & operator = ( const node & );

  private:
    vertex_descriptor vertex;
    std::vector<pair> props;
  };

  std::vector<node> nodes;

public:
  const Property & operator[]( edge_descriptor ) const;
        Property & operator[]( edge_descriptor );
};
*/

//////////////////////////////////////////////////////////////////////////////////////////
//
/// Инкапсуляция алгоритма поиска 2-связных компонентов и/или точек сочленения.
/**
  ПЛАНИРУЕТСЯ ЗАМЕНИТЬ ЭТОТ АЛГОРИТМ НА БОЛЕЕ ОБЩИЙ НО НЕ МЕНЕЕ ЭФФЕКТИВНЫЙ:
  DepthFirstSearch + BicompDFSVisitor

  \par Определение
  d-деревом называем ациклический подграф рассматриваеморго графа, состоящего
  из вершин и ребер, которые обходит поиск в глубину, на основе которого построен 
  данный адгоритм.
  Graph - тип, отвечающий требованиям обычного графа смежности по вершинам

  \par РЕФАКТОРИНГ
  1) Нужно обобщить это алгоритм с библиотекой MtGraph
  2) Возможно снабдить это класс-алгоритм посетителем поиска компонентов. 
     Это, например, позволит генерировать два варианта алгоритма поиска блоков:
     Вариант, когда нужно найти только вершины сочленения (без блоков) вариант, 
     когда нужно искать шарниры и/или блоки;
  2.1.) Возможны другие рецепты, как генерить шаблоном два похожих алгоритма.
  3) Алгоритм можно упростить, если переложить его на еще более общный 
     алгоритм обхода в глубину.
*/
//////////////////////////////////////////////////////////////////////////////////////////

template<class Graph>
class MtBicompSearch
{
  // Ассоциативные типы
  typedef typename  Graph::vertex_index  vertex_index;
  typedef typename  Graph::vertex_size_t vertex_size_t;
  typedef typename  Graph::adjacency_iterator  adjacency_iterator;

private:
  const Graph &             m_graph;
  ptrdiff_t                 m_dfsCounter;  
  std::vector<ptrdiff_t>    num;            ///< Нумерация порядка обхода вершин d-дерева
  std::vector<ptrdiff_t>    father;         ///< Отец вершины в d-дереве
  std::vector<ptrdiff_t>    lval;           ///< Массив значений функции L[v] на каждую вершину - см.теорию стр.166, [Asan], Лемма 6;
  std::vector<vertex_index> m_cutnodes;     ///< Обнаруженные точки сочленения
  std::vector<bool>         m_cutnodeProp;  ///< Признак точки сочленения для вершин

public:
  MtBicompSearch( const Graph & );
  /// Найти все точки сочленения
  const std::vector<vertex_index> & SearchCutnodes();

private:
  /// Алгоритм реккурсивного вызова поиска блоков и точек сочленения в графе
  void BiComp( vertex_index );
  /// Инициализировать все рабочие данные для нового поиска
  void Init();
  /// Запустить алгоритм
  void Perform();
};

//----------------------------------------------------------------------------------------
//
//---
template<class Graph>
MtBicompSearch<Graph>::MtBicompSearch( const Graph & g )
  : m_graph( g )
  , m_dfsCounter(1)
  , num()
  , father()
  , lval()
  , m_cutnodes()
  , m_cutnodeProp()
{}

//----------------------------------------------------------------------------------------
/// Найти все точки сочленения
//---
template<class Graph>
const std::vector<typename Graph::vertex_index> & MtBicompSearch<Graph>::SearchCutnodes()
{
  Init();
  Perform();
  return m_cutnodes;
}

//----------------------------------------------------------------------------------------
/// Инициализировать все рабочие данные для нового поиска
//---
template<class Graph>
void MtBicompSearch<Graph>::Init()
{
  const vertex_size_t vertNb = m_graph.NumVertices();
  m_dfsCounter = 1;
  num.assign( vertNb, 0 );  
  father.assign( vertNb, -1 );  
  lval.assign( vertNb, -1 );  
  m_cutnodeProp.assign( vertNb, false );
  m_cutnodes.clear();
}

//----------------------------------------------------------------------------------------
// Запустить алгоритм.
//---
template<class Graph>
void MtBicompSearch<Graph>::Perform()
{
  PRECONDITION( m_cutnodes.empty() );
  
  const vertex_size_t vertNb = m_graph.NumVertices();
  for ( vertex_index startIdx = 0; startIdx<vertNb; ++startIdx ) // startIdx - корень d-дерева
  {
    if ( num[startIdx] == 0 )
    {
      BiComp( startIdx );
      PRECONDITION( father[startIdx] == -1 );

      // Оценить является ли startIdx - точкой сочленения
      size_t fatherNb = 0; // Сколько раз стартовая вершина стала папой
      std::pair<adjacency_iterator,adjacency_iterator> adjIterPair = m_graph.AdjacentVertices( startIdx );
      for ( ; adjIterPair.first!=adjIterPair.second; ++adjIterPair.first )
      {
        if ( father[*adjIterPair.first] == startIdx )
        {
          ++fatherNb;
        }
      }
      if ( fatherNb > 1 )
      {
        // Корневая вершина - есть точка сочленения
        PRECONDITION( !m_cutnodeProp[startIdx] );
        m_cutnodes.push_back( startIdx );
        m_cutnodeProp[startIdx] = true;
      }
    }
  }
}

//----------------------------------------------------------------------------------------
/// Алгоритм реккурентного вызова поиска блоков и точек сочленения в графе
/**
  Теорию см.главе 8, стр.166, Графы, матроиды, алгоритмы [Asan];
  \param vIdx   -  вершина (индекс), с которой начинаем поиск, которая ещё не рассмотрена, т.е. 
                   num[vIdx] = 0;

  \par Определения
  d-дерево - ациклический подграф основного подграфа, образуемого при обходе 
            вершин во время поиска в глубину;  

  \par Вычислительная сложность
  Вычислительная сложность: O(n+m), где n-кол-во вершин, m-кол-во ребер. Это следует из 
  того факта, что каждая вершина посещается не более одного раза.
*/
//---
template<class Graph>
void MtBicompSearch<Graph>::BiComp( const vertex_index vIdx )
{  
  PRECONDITION( num[vIdx] == 0 );
  num[vIdx] = lval[vIdx] = m_dfsCounter;  
  ++m_dfsCounter;

  // Цикл по всем смежным вершинам vert;
  std::pair<adjacency_iterator,adjacency_iterator> adjIterPair = m_graph.AdjacentVertices( vIdx );
  for ( ; adjIterPair.first!=adjIterPair.second; ++adjIterPair.first )
  {
    const vertex_index uIdx = *adjIterPair.first; // Вершина - сын в d-дереве;
    // const edge_descriptor vuEdg = m_graph.GetEdge( vIdx, uIdx );
    if ( num[uIdx] == 0 ) // uIdx - сын вершины vIdx
    {
      // stackE.push_back( vuEdg );  // Вставить ребро vu;
      PRECONDITION( father[uIdx] == -1 );
      father[uIdx] = vIdx;  // зафиксируем отца для данной вершины uIdx;
      BiComp( uIdx );
      
      // При выходе из рекурсии значение функции L[u] уже вычислено;
      lval[vIdx] = min_of( lval[vIdx], lval[uIdx] ); // см.лемму 6;
      if ( lval[uIdx] >= num[vIdx] )
      {
        //  Здесь можно получить новый блок, для чего достаточно вытолкнуть из 
        //  стека все ребра, включая ребро vu.
        
        // (!) Если вершина vIdx не корень d-дерева, то можно утверждать, что она - есть точка сочленения;
        //     См. теорему 8.2.
        if ( father[vIdx] != -1 ) // Первородитель
        {
          if ( !m_cutnodeProp[vIdx] )
          {
            m_cutnodes.push_back( vIdx );
            m_cutnodeProp[vIdx] = true;
          }
        }
      }
    }
    else if ( num[uIdx] < num[vIdx] && uIdx != father[vIdx] )
    {
      // Здесь vu - есть обратное ребро входящее в вершину u, которая выше, чем v в d-дереве;       
      // stackE.push_back( vuEdg );  // вставить ребро vu;
      lval[vIdx] = min_of( lval[vIdx], num[uIdx] ); // см.лемму 6;
    }
  }
}

//---------------------------------------------------------------------------------------
// Посетитель алгоритма поиска компонент сильной связности.
//---
struct DefaultSCVisitor
{
  // Вызывается алгоритмом перед началом обхода всего графа.
  template<class Graph>
  inline void Start( const Graph & ) {}
  // Вызывается, когда найден очередной компонент сильной связности в орграфе.
  /*
    Аргументы: граф и пара вершинных итераторов, пробегающих подмножество компонента.
  */
  template<class Graph, class VertexIter>
  inline void Component( const Graph &, VertexIter, VertexIter ) {}
  // Если IsFiltered = true, вершина считается исключенной из графа
  template<class Graph, class Vertex>
  inline bool IsFiltered( const Graph &, Vertex ) { return false; }
};

//---------------------------------------------------------------------------------------
/** \brief \ru Алгоритм поиска компонентов сильной связности в орграфе.
           \en Algorithm for searching strongly connected components in a digraph.
  \details
     \ru Напомним, что две вершины орграфа считаются сильно связанными, если 
  существует маршрут из первой вершины ко второй и обратный маршрут из второй 
  к первой. Подграф называется <b>сильно связным</b>, если любая пары его 
  вершин сильно связаны. <b>Компонент сильной связности</b> графа - это один из 
  его сильно сзязный подграфов G', для которого не существует сильно связной пары 
  вершин u и v, таких, что u-принадлежит G', а v не принадлежит G'. Другими словами, 
  вершины компонента сильной связости принадлежат классу взаимной достижимости вершин. \~
    \note \ru Алгоритм #MtStrongComponents имеет линейную сложность вычислений.  
*/
//---
template <class Graph, class SCVisitor, class NumVertexMap>
class MtStrongComponents
{
public:  // Ассоциативные типы.
  typedef typename graph_traits<Graph>::vertex           vertex;
  typedef typename graph_traits<Graph>::edge             edge;
  typedef typename graph_traits<Graph>::edge_iterator    edge_iterator;
  typedef typename graph_traits<Graph>::vertex_iterator  vertex_iterator;    
  typedef void (*InitNumVertexMap)(NumVertexMap&, const Graph&, const size_t& val);  

public:  
  MtStrongComponents(const Graph& g, SCVisitor& vis, InitNumVertexMap initFunc = DefaultInitFunc)
  : m_diGraph( g )
  , m_vis( vis )
  , num()
  , lval()
  , m_counter( 1 )
  , initNumVertexMap(initFunc)
  {}
  MtStrongComponents( const MtStrongComponents & ) = delete;
  MtStrongComponents & operator = ( const MtStrongComponents & ) = delete;

public:
  void operator() ()  ///< Исполнить алгоритм поиска сильных компонентов.
  {
    Perform( m_diGraph.Vertices() );
  }

  // Главный алгоритм поиска компонентов сильной связносити для указанных вершин.
  template<class VertexRange>
  void Perform(const VertexRange & nodesRng)
  {
    initNumVertexMap(num, m_diGraph, 0);
    initNumVertexMap(lval, m_diGraph, 0);
   
    m_vis.Start(m_diGraph);

    std::vector<vertex> stack;
    stack.reserve(m_diGraph.NumVertices());
    m_counter = 1;    
  
    for (const vertex & node: m_diGraph.Vertices())
    {
      num[node] = 0;
    }

    for (const vertex & node: nodesRng)
    {
      if (num[node] == 0 && !m_vis.IsFiltered(m_diGraph,node))
        StrongSearch(node, stack);
    }    
  }

private:
  // DFS-алгоритм для поиска компонентов сильной связности в графе ограничений.
  void StrongSearch( vertex, std::vector<vertex> & );
  static void DefaultInitFunc(NumVertexMap& pMap, const Graph& g, const size_t& val) 
  {
    pMap.assign(g.NumVertices(), val);
  }

private:
  const Graph &      m_diGraph;  ///< Ориентированный граф.
        SCVisitor &  m_vis;      ///< Посетитель алгоритма поиска компонентов сильной связности.
        size_t       m_counter;  ///< Порядок DFS-обхода.
        NumVertexMap num;        ///< Вспомогательный массив порядковых номеров обхода в глубину.
        NumVertexMap lval;       ///< Массив для промежуточных целочисленных вычислений.
    InitNumVertexMap initNumVertexMap; ///< Function initializing the vertex property map.
};

//----------------------------------------------------------------------------------------
/// Стековый элемент для алгоритма обхода в глубину.
//---
template<class Graph>
struct DFS_element
{
  typedef typename Graph::vertices_size_t vertices_size_t;
  typedef typename Graph::vertex          vertex;
  typedef typename Graph::edge_iterator   edge_iterator;

  vertex        node;
  edge_iterator iter; // Current incident edge.
  edge_iterator last;

  DFS_element( vertex v, const std::pair<edge_iterator,edge_iterator> & pair )
    : node( v )
    , iter( pair.first )
    , last( pair.second )
  {}
  
  DFS_element( vertex v, const Graph & graph )
    : node( v )
    , iter()
    , last()
  {
    c3d::tie( iter, last ) = graph.OutArcs( v ); 
  }  

  DFS_element( const DFS_element & vi )
    : node( vi.node )
    , iter( vi.iter )
    , last( vi.last )
  {}

  DFS_element & operator = ( const DFS_element & vi )
  {      
    node = vi.node;
    iter = vi.iter;
    last = vi.last;
    return *this;
  }  
};

//----------------------------------------------------------------------------------------
/// Алгоритм поиска компонентов сильной связности в орграфе.
/**    
    Алгоритм применяется для разбиения графа ограничений на независимо-решаемые
  подсистемы (сегменты). Описание алгоритма приведено в книжке Асанова по 
  теории графов, стр.171.
    MA 2013-02-22: Алгоритм переделан под нерекурсивный вариант.
  \param rootVert - корневая вершина поддерева DFS
  \param comStack - стек рассмотренных вершин, для которых не установлена компонентная принадлежность
*/
//---
template <class Graph, class Vis, class VPropMap>
void MtStrongComponents<Graph,Vis,VPropMap>::StrongSearch( vertex rootVert, std::vector<vertex> & comStack )
{  
  using DfsStackElem = DFS_element<Graph>;  

  PRECONDITION( !m_vis.IsFiltered(m_diGraph,rootVert) );
  std::vector<DfsStackElem> dfsStack;  
  
  num[rootVert] = lval[rootVert] = m_counter;
  ++m_counter;
  comStack.push_back( rootVert );  
  dfsStack.push_back( DfsStackElem( rootVert, m_diGraph ) );
  DfsStackElem * topElem = &dfsStack.back();    

  while( !dfsStack.empty() ) // Цикл возвратов из стека (одна итерация - одно возвращение против древесного ребра).
  {    
    while ( topElem->iter != topElem->last )
    {
      vertex w = m_diGraph.Target( *topElem->iter ); // Выходящая вершина прямого ребра <v,w>.
      PRECONDITION( w != topElem->node ); // Граф не ориентированный !!!
      if ( (w != topElem->node) && !m_vis.IsFiltered(m_diGraph,w) )  // игнорируем обратное ребро из w в vx, а также отфильтрованные узлы;
      {  
        if ( num[w] == 0 ) // <v,w> - "древесная" дуга
        {          
          // Вместо рекурсии: StrongSearch( w, stack );          
          num[w] = lval[w] = m_counter;
          ++m_counter;
          comStack.push_back( w );
          dfsStack.push_back( DfsStackElem(w, m_diGraph) );
          topElem = &dfsStack.back();
          continue;
        }
        else
        {          
          const size_t wNum = num[w];
          if ( wNum < num[topElem->node] && wNum < lval[topElem->node] ) // <v,w> - "поперечная" или "обратная" дуга.
          {
            // Предположение: В стеке лежат вершины, из которых вершина vx достижима.
            if ( std::find(comStack.rbegin(), comStack.rend(), w) != comStack.rend() )
            {              
              lval[topElem->node] = wNum; 
            }
          }
        }
      }
      ++topElem->iter;
    }

    PRECONDITION( dfsStack.back().iter == dfsStack.back().last );
    PRECONDITION( topElem == &dfsStack.back() );
    
    // Завершено посещение узла sElem->m_node.
    const size_t vNum = num[topElem->node/*vx*/];
    if ( lval[topElem->node/*vx*/] == vNum ) // vx - корневая вершина очередной компоненты сильной связности.
    {
      // Обнаружен очередной сильный компонент
      if ( !comStack.empty() && num[comStack.back()] >= vNum )
      {
        // Посчитать размер компонента
        typename std::vector<vertex>::reverse_iterator vIter, vLast;
        vIter = comStack.rbegin();
        vLast = comStack.rend();
        ptrdiff_t compSize = 0;
        for ( ; vIter != vLast && num[*vIter] >= vNum; ++vIter, ++compSize );
          
        // Передать диапазон компонента визитеру
        typename std::vector<vertex>::iterator cIter, cLast;
        cIter = cLast = comStack.end();
        std::advance( cIter, -compSize );
        m_vis.Component( m_diGraph, cIter, cLast );
        comStack.erase( cIter, cLast ); // очистить верхушку стека
      }
    }
    // Возвращение против древесного ребра <v,w>, где w-просмотренная вершина, v-вершина из которой пришли в w.
    {      
      const vertex w = topElem->node;
      dfsStack.pop_back();
      
      if ( !dfsStack.empty() )
      {
        topElem = &dfsStack.back();
        const vertex vxPrev = dfsStack.back().node;
        // При выходе из рекурсии значение lVal(w) должно быть уже насчитано;
        if ( lval[w] < lval[vxPrev] ) 
        {
          lval[vxPrev] = lval[w];
        }
      }
    }
  }
}

#endif // __GRAPH_ALGORITHMS_H

/** \} */ // Base_GraphLib

// eof
