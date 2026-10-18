////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Итераторы обхода модели: контейнеров, сборок, 
             граней, рёбер и врешин тел, 
             циклов граней, рёбер циклов, 
             граней и рёбер, примыкающих к вершине.
         \en Iterators for model traverse: containers, assemblies,
             faces, edges, verticies of solids, 
             loops of faces, edges of loops, 
             faces and edjes adjacent to verticies. \~

    \details  \ru Итераторы обеспечивают обход с защитой от нулевых указателей.
              \en Iterators provide traverse of only non-null elements.

*/
////////////////////////////////////////////////////////////////////////////////


#ifndef __TOPOLOGY_ITERATOR_H
#define __TOPOLOGY_ITERATOR_H

class MbItem;
class MbAssembly;
class MbPlaneInstance;
class MbSolid;
class MbFaceShell;
class MbFace;
class MbEdge;
class MbLoop;
class MbOrientedEdge;
class MbVertex;
class MbWireFrame;
class MbCurveEdge;
class MbPlaneItem;
class MbMesh;
class MbPolygon3D;

#include <topology.h>
#include <templ_sptr.h>
#include <system_cpp_standard.h>
#include <vector>
#include <map>


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода контейнера указателей.
           \en Iterator for traverse container of pointers. \~
    \details  \ru ContainerType - контейнер указателей на объекты типа ItemType.
              \en ContainerType - is a container of of pointers to items of ItemType type.
*/
// ---
template<typename ContainerType, typename ItemType>
class ItemPtrVectorIterator {
  const ContainerType& container; ///< \ru Контейнер. \en A container.
  size_t index;                   ///< Индекс текущего элемента. \en Index of current element.
public:
  /// \ru Конструктор. \en Constructor.
  ItemPtrVectorIterator( const ContainerType& cont )
    : container( cont )
    , index( 0 )
  {
    if ( (!container.empty()) && container[index] == nullptr )
      next();
  }

  /// \ru Перейти к следующему элементу. \en Move to the next element.
  inline void next() {
    do { index++; } while ( (index < container.size()) && (nullptr == container[index]) );
  }

  /// \ru Перейти к следующему элементу. \en Move to the next element.
  inline void operator++() { next(); }

  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  inline bool done() const { return index >= container.size(); }

  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  inline bool undone() const { return !done(); }

  /// \ru Получить элемент в контейнере. \en Get element in container.
  inline ItemType& operator*() { return *(container.at(index)); }

  /// \ru Получить индекс. \en Get current index.
  inline size_t Index() { return index; }
private:
  ItemPtrVectorIterator( const ItemPtrVectorIterator& );
  ItemPtrVectorIterator& operator = ( const ItemPtrVectorIterator& );
};


typedef ItemPtrVectorIterator<std::vector<SPtr<MbItem> >, MbItem>     ItemVectorSptrIterator;
typedef ItemPtrVectorIterator<std::vector<SPtr<MbSolid> >, MbSolid>   SolidVectorSptrIterator;


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода сборки.
           \en Iterator for traverse of assembly. \~
*/
// ---
class MATH_CLASS AssemblyItemsIterator {
  MbAssembly& assm; ///< \ru Сборка. \en An assembly.
  size_t index;     ///< \ru Индекс текущего элемента. \en Index of current element.
public:
  /// \ru Конструктор. \en Constructor.
  AssemblyItemsIterator( MbAssembly& assebmly );

  /// \ru Перейти к следующему элементу. \en Move to the next element.
  void next();

  /// \ru Перейти к следующему элементу. \en Move to the next element.
  void operator++();

  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done() const;

  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone() const;

  /// \ru Получить элемент сборки. \en Get assembly element.
  MbItem& operator*();

  /// \ru Получить индекс. \en Get current index.
  size_t Index();
private:
  AssemblyItemsIterator( const AssemblyItemsIterator& );
  AssemblyItemsIterator& operator =( const AssemblyItemsIterator& );
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода константной сборки.
           \en Iterator for traverse of immutable assembly. \~
*/
// ---
class MATH_CLASS AssemblyItemsConstIterator {
  const MbAssembly& assm; ///< \ru Сборка. \en An assembly.
  size_t index;           ///< \ru Индекс текущего элемента. \en Index of current element.
public:
  /// \ru Конструктор. \en Constructor.
  AssemblyItemsConstIterator( const MbAssembly& assebmly );

  /// \ru Перейти к следующему элементу. \en Move to the next element.
  void next();

  /// \ru Перейти к следующему элементу. \en Move to the next element.
  void operator++();

  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done() const;

  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone() const;

  /// \ru Получить элемент сборки. \en Get assembly element.
  const MbItem& operator*();

  /// \ru Получить индекс. \en Get current index.
  size_t Index();
private:
  AssemblyItemsConstIterator( const AssemblyItemsConstIterator& );
  AssemblyItemsConstIterator& operator =( const AssemblyItemsConstIterator& );
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода элементов плоской вставки.
           \en Iterator for traverse of elements of planar instance. \~
*/
// ---
class MATH_CLASS PlaneInstanceItemsIterator {
  MbPlaneInstance& plInst;  ///< \ru Плоская вставка.           \en A planar instance.
  size_t index;             ///< \ru Индекс текущего элемента.  \en Index of current element.
public:
  /// \ru Конструктор. \en Constructor.
  PlaneInstanceItemsIterator( MbPlaneInstance& inst );

  /// \ru Перейти к следующему элементу. \en Move to the next element.
  void next();

  /// \ru Перейти к следующему элементу. \en Move to the next element.
  void operator++();

  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done() const;

  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone() const;

  /// \ru Получить элемент плоской вставки. \en Get element of planar instance.
  MbPlaneItem& operator*();

  /// \ru Получить индекс. \en Get current index.
  size_t Index();
private:
  PlaneInstanceItemsIterator( const PlaneInstanceItemsIterator& );
  PlaneInstanceItemsIterator& operator =( const PlaneInstanceItemsIterator& );
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода элементов константной плоской вставки.
           \en Iterator for traverse of elements of immutable planar instance. \~
*/
// ---
class MATH_CLASS PlaneInstanceItemsConstIterator {
  const MbPlaneInstance& plInst;  ///< \ru Плоская вставка.           \en A planar instance.
  size_t index;                   ///< \ru Индекс текущего элемента.  \en Index of current element.
public:
  /// \ru Конструктор. \en Constructor.
  PlaneInstanceItemsConstIterator( const MbPlaneInstance& inst );

  /// \ru Перейти к следующему элементу. \en Move to the next element.
  void next();

  /// \ru Перейти к следующему элементу. \en Move to the next element.
  void operator++();

  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done() const;

  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone() const;

  /// \ru Получить элемент плоской вставки. \en Get element of planar instance.
  const MbPlaneItem& operator*();

  /// \ru Получить индекс. \en Get current index.
  size_t Index();
private:
  PlaneInstanceItemsConstIterator( const PlaneInstanceItemsConstIterator& );
  PlaneInstanceItemsConstIterator& operator =( const PlaneInstanceItemsConstIterator& );
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода рёбер константного тела.
           \en Iterator for traverse edged of immutable solid. \~
*/
// ---
class MATH_CLASS SolidEdgesConstIterator {
  std::vector<const MbCurveEdge*> edges;  ///< \ru Кэш рёбер. \en Edges cache.

  /// \ru Итератор кэша рёбер. \en Iterator of edges cache.
  ItemPtrVectorIterator< std::vector<const MbCurveEdge*>, const MbCurveEdge > edgesItr; 
public:
  /// \ru Конструктор. \en Constructor.
  SolidEdgesConstIterator( const MbSolid& solid, bool skipSeamsPoles = false );

  /// \ru Перейти к следующему ребру. \en Move to the next edge.
  void next();

  /// \ru Перейти к следующему ребру. \en Move to the next edge.
  void operator++();

  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done() const;

  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone() const;

  /// \ru Получить ребро. \en Get edge.
  const MbCurveEdge& operator*();

  /// \ru Получить индекс. \en Get current index.
  size_t Index();
private:
  SolidEdgesConstIterator( const SolidEdgesConstIterator& );
  SolidEdgesConstIterator& operator =( const SolidEdgesConstIterator& );
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода рёбер тела.
           \en Iterator for traverse edged of solid. \~
*/
// ---
class MATH_CLASS SolidEdgesIterator {
  std::vector<MbCurveEdge*> edges;  ///< \ru Кэш рёбер. \en Edges cache.

  /// \ru Итератор кэша рёбер. \en Iterator of edges cache.
  ItemPtrVectorIterator< std::vector<MbCurveEdge*>, MbCurveEdge > edgesItr;
public:
  /// \ru Конструктор. \en Constructor.
    /** \brief  \ru Конструктор.
              \en Contructor. \~
      \param[in] solid  \ru Тело, чьи рёбра обходятся. \en Solid which's edges are traversed.
      \param[in] skipSeamsPoles \ru Пропускать ли при обходе полюсные и шовные рёбра.
                                \en Weather ignore seam and pole edges.
  */
  SolidEdgesIterator( MbSolid& solid, bool skipSeamsPoles = false );

  /// \ru Перейти к следующему ребру. \en Move to the next edge.
  void next();

  /// \ru Перейти к следующему ребру. \en Move to the next edge.
  void operator++();

  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done() const;

  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone() const;

  /// \ru Получить ребро. \en Get edge.
  MbCurveEdge& operator*();

  /// \ru Получить индекс. \en Get current index.
  size_t Index();
private:
  SolidEdgesIterator( const SolidEdgesIterator& );
  SolidEdgesIterator& operator =( const SolidEdgesIterator& );
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода вершин оболочки.
           \en Iterator for traverse verticies of a shell. \~
*/
// ---
class MATH_CLASS ShellVerticiesIterator
{
  c3d::VerticesVector vertices; ///< \ru Кэш вершин. \en Verticies cache.
  size_t              index;    ///< \ru Индекс текущей вершины. \en Index of current vertex.
public:
  /// \ru Конструктор. \en Constructor.
  ShellVerticiesIterator( MbFaceShell& shell );

  /// \ru Перейти к следующей вершине. \en Move to the next vertex.
  void next();

  /// \ru Перейти к следующей вершине. \en Move to the next vertex.
  void operator++();

  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done() const;

  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone() const;

  /// \ru Получить вершину. \en Get vertex.
  MbVertex& operator*();

  /// \ru Получить индекс. \en Get current index.
  size_t Index();
private:
  ShellVerticiesIterator( const ShellVerticiesIterator& );
  ShellVerticiesIterator& operator =( const ShellVerticiesIterator& );
};


//------------------------------------------------------------------------------
/** \brief \ru Кэш связей вершины и ребер, примыкающих к ней.
           \en Cache for edges adjacent to a vertex. \~
*/
// ---
class MATH_CLASS CacheEdgesForVertex
{
  /// \ru Кэш рёбер, примыкающих к каждой вершине. \en Cacne of edges adjacent to each vertex.
  std::map<const MbVertex*, c3d::ConstEdgesVector > mapVertexEdges;
public:
  /** \brief  \ru Конструктор.
              \en Contructor. \~
      \param[in] shell  \ru Оболочка, для вершин которой строится кэш. \en Shell for which's verticies cache is built.
      \param[in] vertex \ru Вершина, для которой строится кэш. Если нуль, то кэш строится для всех вершин.
                        \en Vertex for which's cache is built. If null, cache is built for every vertex.
  */
  CacheEdgesForVertex( const MbFaceShell& shell, const MbVertex* vertex = nullptr);

  /// \ru Деструктор. \en Destructor.
  ~CacheEdgesForVertex() {};
  
  /// \ru Получить рёбра, примыкающие к вершине. \en Get edges adjacent to a vertex.
  bool GetEdgesForVertex( const MbVertex* vrt, c3d::ConstEdgesVector& edges );
    
  /// \ru Получить число рёбер, примыкающих к вершине. \en Get count of edges adjacent to a vertex.
  size_t EdgesCount( const MbVertex* vrt );

private:
  /// \ru Заполнить кэш. \en Fill cache.
  void FillMap( const MbFaceShell& shell, const MbVertex* vrt );
};


//------------------------------------------------------------------------------
/** \brief \ru Кэш связей вершины и граней, примыкающих к ней.
           \en Cache for faces adjacent to a vertex. \~
*/
// ---
class MATH_CLASS CacheFacesForVertex
{
  /// \ru Кэш граней, примыкающих к каждой вершине. \en Cacne of faces adjacent to each vertex.
  std::map<const MbVertex*, c3d::ConstFacesVector > mapVertexFaces;
public:
  /** \brief  \ru Конструктор.
              \en Contructor. \~
      \param[in] shell  \ru Оболочка, для вершин которой строится кэш. \en Shell for which's verticies cache is built.
      \param[in] vertex \ru Вершина, для которой строится кэш. Если нуль, то кэш строится для всех врешин.
                        \en Vertex for which's cache is built. If null, cache is built for every vertex.
  */
  CacheFacesForVertex( const MbFaceShell& shell, const MbVertex* vertex = nullptr );

  /// \ru Деструктор. \en Destructor.
  ~CacheFacesForVertex();
  
  /// \ru Получить грани, примыкающие к вершине. \en Get faces adjacent to a vertex.
  bool GetFacesForVertex( const MbVertex* vrt, c3d::ConstFacesVector& faces );
  
  /// \ru Получить число граней, примыкающих к вершине. \en Get count of faces adjacent to a vertex.
  size_t FacesCount( const MbVertex* vrt );
private:
  /// \ru Заполнить кэш. \en Fill cache.
  void FillMap( const MbFaceShell& shell, const MbVertex* vrt );
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода рёбер, примыкающих к вершине.
           \en Iterator for traverse edges adjacent to a vertex. \~
*/
// ---
class MATH_CLASS VertexEdgesIterator
{
  const MbVertex* vrt;        /// \ru Вершина. \en Vertex.
  CacheEdgesForVertex* cache; /// \ru Кэш. \en Cache.
  size_t edgeIndex;           /// \ru Индекс текущего ребра. \en Index of current edge.
  bool bLocalCache;           /// \ru Кэш создаётся и удаляется итератором. \en Weather the cache is created and destroied in the iterator.
public:
  
  /** \brief  \ru Конструктор.
              \en Contructor. \~      
      \param[in] vertex \ru Вершина, для которой строится кэш. Если нуль, то кэш строится для всех врешин.
                        \en Vertex for which's cache is built. If null, cache is built for every vertex.
      \param[in] shell  \ru Оболочка, для вершин которой строится кэш. \en Shell for which's verticies cache is built.
  */
  VertexEdgesIterator( const MbVertex& vertex, MbFaceShell& shell );
    
  
  /** \brief  \ru Конструктор.
              \en Contructor. \~
      \details  \ru Ожидается, что существующий кэш содержит данные для вершины.
                \en Expected that existing cache contains data for the vertex.
      \param[in] vertex \ru Вершина, для которой строится кэш. Если нуль, то кэш строится для всех врешин.
                        \en Vertex for which's cache is built. If null, cache is built for every vertex.
      \param[in] cacheEdges  \ru Существующий кэш. \en Existing cache.
  */
  VertexEdgesIterator( const MbVertex& vertex, CacheEdgesForVertex& cacheEdges );

  /// \ru Деструктор. \en Destructor.
  ~VertexEdgesIterator();

  /// \ru Перейти к следующему ребру. \en Move to the next edge.
  void next();

  /// \ru Перейти к следующему ребру. \en Move to the next edge.
  void operator++();

  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done();

  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone();

  /// \ru Получить ребро. \en Get edge.
  const MbCurveEdge& operator*();
  
  /// \ru Получить кэш. \en Get cache.
  CacheEdgesForVertex* GetCache() const;
private:
  VertexEdgesIterator( const VertexEdgesIterator& );
  VertexEdgesIterator& operator =( const VertexEdgesIterator& );
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода граней, примыкающих к вершине.
           \en Iterator for traverse faces adjacent to a vertex. \~
*/
// ---
class MATH_CLASS VertexFacesIterator
{
  const MbVertex* vrt;        /// \ru Вершина. \en Vertex.
  CacheFacesForVertex* cache; /// \ru Кэш. \en Cache.
  size_t faceIndex;           /// \ru Индекс текущей грани. \en Index of current edge.
  bool bLocalCache;           /// \ru Кэш создаётся и удаляется итератором. \en Weather the cache is created and destroied in the iterator.
public:  
  /** \brief  \ru Конструктор.
              \en Contructor. \~
      \param[in] vertex \ru Вершина, для которой строится кэш. Если нуль, то кэш строится для всех врешин.
                        \en Vertex for which's cache is built. If null, cache is built for every vertex.
      \param[in] shell  \ru Оболочка, для вершин которой строится кэш. \en Shell for which's verticies cache is built.
  */
  VertexFacesIterator( const MbVertex& vertex, MbFaceShell& shell );  
  
  /** \brief  \ru Конструктор.
              \en Contructor. \~
      \details  \ru Ожидается, что существующий кэш содержит данные для вершины.
                \en Expected that existing cache contains data for the vertex.
      \param[in] vertex \ru Вершина, для которой строится кэш. Если нуль, то кэш строится для всех врешин.
                        \en Vertex for which's cache is built. If null, cache is built for every vertex.
      \param[in] cacheEdges  \ru Существующий кэш. \en Existing cache.
  */
  VertexFacesIterator( const MbVertex& vertex, CacheFacesForVertex& cacheEdges );
  
  /// \ru Деструктор. \en Destructor.
  ~VertexFacesIterator();

  /// \ru Перейти к следующей грани. \en Move to the next face.
  void next();

  /// \ru Перейти к следующей грани. \en Move to the next face.
  void operator++();

  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done();

  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone();

  /// Получить ребро.
  const MbFace& operator*();

  /// \ru Получить кэш. \en Get cache.
  CacheFacesForVertex* GetCache() const;
private:
  VertexFacesIterator( const VertexFacesIterator& );
  VertexFacesIterator& operator =( const VertexFacesIterator& );
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода граней тела.
           \en Iterator for traverse faces of a solid. \~
*/
// ---
class MATH_CLASS SolidFacesIterator {
  MbSolid& sol;   ///< \ru Тело. \en Solid.
  size_t index;   ///< \ru Индекс текущей грани. \en Index of current face.
public:
  /// \ru Конструктор. \en Constructor.
  SolidFacesIterator( MbSolid& solid );

  /// \ru Перейти к следующей грани. \en Move to the next face.
  void next();

  /// \ru Перейти к следующей грани. \en Move to the next face.
  void operator++();

  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done() const;

  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone() const;

  /// Получить грань.
  MbFace& operator*();

  /// \ru Получить индекс. \en Get current index.
  size_t Index();
private:
  SolidFacesIterator( const SolidFacesIterator& );
  SolidFacesIterator& operator =( const SolidFacesIterator& );
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода граней константного тела.
           \en Iterator for traverse faces of a constant solid. \~
*/
// ---
class MATH_CLASS SolidFacesConstIterator {
  const MbSolid& sol;   ///< \ru Тело. \en Solid.
  size_t index;         ///< \ru Индекс текущей грани. \en Index of current face.
public:
  /// \ru Конструктор. \en Constructor.
  SolidFacesConstIterator( const MbSolid& solid );

  /// \ru Перейти к следующей грани. \en Move to the next face.
  void next();

  /// \ru Перейти к следующей грани. \en Move to the next face.
  void operator++();

  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done() const;

  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone() const;

  /// Получить грань.
  const MbFace& operator*();

  /// \ru Получить индекс. \en Get current index.
  size_t Index();
private:
  SolidFacesConstIterator( const SolidFacesConstIterator& );
  SolidFacesConstIterator& operator =( const SolidFacesConstIterator& );
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода граней оболочки.
           \en Iterator for traverse faces of a shell. \~
*/
// ---
class MATH_CLASS ShellFacesIterator {
  MbFaceShell& fsh; ///< \ru Оболочка. \en Shell.
  size_t index;     ///< \ru Индекс текущей грани. \en Index of current face.
public:
  /// \ru Конструктор. \en Constructor.
  ShellFacesIterator( MbFaceShell& shell );

  /// \ru Перейти к следующей грани. \en Move to the next face.
  void next();

  /// \ru Перейти к следующей грани. \en Move to the next face.
  void operator++();

  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done() const;

  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone() const;

  /// Получить грань.
  MbFace& operator*();

  /// \ru Получить индекс. \en Get current index.
  size_t Index();
private:
  ShellFacesIterator( const ShellFacesIterator& );
  ShellFacesIterator& operator =( const ShellFacesIterator& );
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода граней константной оболочки.
           \en Iterator for traverse faces of a immutable shell. \~
*/
// ---
class MATH_CLASS ShellFacesConstIterator {
  const MbFaceShell& fsh; ///< \ru Оболочка. \en Shell.
  size_t index;            ///< \ru Индекс текущей грани. \en Index of current face.
public:
  /// \ru Конструктор. \en Constructor.
  ShellFacesConstIterator( const MbFaceShell& shell );

  /// \ru Перейти к следующей грани. \en Move to the next face.
  void next();

  /// \ru Перейти к следующей грани. \en Move to the next face.
  void operator++();

  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done() const;

  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone() const;

  /// Получить грань.
  MbFace& operator*();

  /// \ru Получить индекс. \en Get current index.
  size_t Index();
private:
  ShellFacesConstIterator( const ShellFacesConstIterator& );
  ShellFacesConstIterator& operator =( const ShellFacesConstIterator& );
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода циклоа грани.
           \en Iterator for traverse loops of a face. \~
*/
// ---
class MATH_CLASS FaceLoopsIterator {
  MbFace& fs;   ///< \ru Грань. \en Face.
  size_t index; ///< \ru Индекс текущего цикла. \en Index of current loop.
public:
  /// \ru Конструктор. \en Constructor.
  FaceLoopsIterator( MbFace& face );

  /// \ru Перейти к следующему циклу. \en Move to the next loop.
  void next();

  /// \ru Перейти к следующему циклу. \en Move to the next loop.
  void operator++();

  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done() const;

  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone() const;

  /// \ru Получить текущий цикл. \en Get a current loop.
  MbLoop& operator*();
private:
  FaceLoopsIterator( const FaceLoopsIterator& );
  FaceLoopsIterator& operator =( const FaceLoopsIterator& );
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода циклоа константной грани.
           \en Iterator for traverse loops of a immutable face. \~
*/
// ---
class MATH_CLASS FaceLoopsConstIterator {
  const MbFace& fs;   ///< \ru Грань. \en Face.
  size_t index;       ///< \ru Индекс текущего цикла. \en Index of current loop.
public:
  /// \ru Конструктор. \en Constructor.
  FaceLoopsConstIterator( const MbFace& face );
  
  /// \ru Перейти к следующему циклу. \en Move to the next loop.
  void next();
  
  /// \ru Перейти к следующему циклу. \en Move to the next loop.
  void operator++();
  
  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done() const;
  
  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone() const;
  
  /// \ru Получить текущий цикл. \en Get a current loop.
  const MbLoop& operator*();
private:
  FaceLoopsConstIterator( const FaceLoopsConstIterator& );
  FaceLoopsConstIterator& operator =( const FaceLoopsConstIterator& );
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода ориентированных рёбер цикла.
           \en Iterator for traverse oriented edges of a loop. \~
*/
// ---
class MATH_CLASS LoopOrientedEdgesIterator {
  MbLoop& lp;   ///< \ru Цикл. \en Loop.
  size_t index; ///< \ru Индекс текущего ребра. \en Index of current edge.
public:
  /// \ru Конструктор. \en Constructor.
  LoopOrientedEdgesIterator( MbLoop& loop );

  /// \ru Перейти к следующему ориентированному ребру. \en Move to the next oriented edge.
  void next();

  /// \ru Перейти к следующему ориентированному ребру. \en Move to the next oriented edge.
  void operator++();

  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done() const;

  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone() const;

  /// \ru Получить текущее ориентированное ребро. \en Get a current oriented edge.
  MbOrientedEdge& operator*();

  /// \ru Получить текущее ребро. \en Get a current edge.
  MbCurveEdge& CurveEdge();
private:
  LoopOrientedEdgesIterator( const LoopOrientedEdgesIterator& );
  LoopOrientedEdgesIterator& operator =( const LoopOrientedEdgesIterator& );
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода ориентированных рёбер константного цикла.
           \en Iterator for traverse oriented edges of a immutable loop. \~
*/
// ---
class MATH_CLASS LoopOrientedEdgesConstIterator {
  const MbLoop& lp;   ///< \ru Цикл. \en Loop.
  size_t index;       ///< \ru Индекс текущего ребра. \en Index of current edge.
public:
  /// \ru Конструктор. \en Constructor.
  LoopOrientedEdgesConstIterator( const MbLoop& loop );

  /// \ru Перейти к следующему ориентированному ребру. \en Move to the next oriented edge.
  void next();

  /// \ru Перейти к следующему ориентированному ребру. \en Move to the next oriented edge.
  void operator++();

  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done() const;

  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone() const;

  /// \ru Получить текущее ориентированное ребро. \en Get a current oriented edge.
  const MbOrientedEdge& operator*();

  /// \ru Получить текущее ребро. \en Get a current edge.
  const MbCurveEdge& CurveEdge();

  /// \ru Получить индекс. \en Get current index.
  size_t Index() const;
private:
  LoopOrientedEdgesConstIterator( const LoopOrientedEdgesConstIterator& );
  LoopOrientedEdgesConstIterator& operator =( const LoopOrientedEdgesConstIterator& );
};


//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода рёбер каркаса.
           \en Iterator for traverse edges of wire frame. \~
*/
// ---
class MATH_CLASS WireFrameEdgesIterator {
  MbWireFrame& wf; ///< \ru Каркас. \en Wire frame.
  size_t index;    ///< \ru Индекс текущего ребра. \en Index of current edge.
public:
  /// \ru Конструктор. \en Constructor.
  WireFrameEdgesIterator( MbWireFrame& wire );

  /// \ru Перейти к следующему ребру. \en Move to the next edge.
  void next();

  /// \ru Перейти к следующему ребру. \en Move to the next edge.
  void operator++();

  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done() const;

  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone() const;

  /// \ru Получить текущее ребро. \en Get a current edge.
  MbEdge& operator*();
private:
  WireFrameEdgesIterator( const WireFrameEdgesIterator& );
  WireFrameEdgesIterator& operator =( const WireFrameEdgesIterator& );
};



//------------------------------------------------------------------------------
/** \brief \ru Итератор обхода рёбер полигональого объекта.
           \en Iterator for traverse edges of a mesh. \~
*/
// ---
class MATH_CLASS MeshPolygonsConstIterator {
  const MbMesh& mesh; ///< \ru Полигональный объект. \en Mesh.
  size_t index;       ///< \ru Индекс текущего полигона. \en Index of current polygon.
public:
  /// \ru Конструктор. \en Constructor.
  MeshPolygonsConstIterator( const MbMesh& msh );
  /// \ru Перейти к следующему полигону. \en Move to the next polygon.
  void next();
  /// \ru Перейти к следующему полигону. \en Move to the next polygon.
  void operator++();
  /// \ru Закончен ли обход. \en Test whether the traverse is over.
  bool done() const;
  /// \ru Можно ли продолжать обход. \en Test whether the traverse is not over.
  bool undone() const;
  /// Получить ребро.
  const MbPolygon3D& operator*();
private:
  MeshPolygonsConstIterator( const MeshPolygonsConstIterator& );
  MeshPolygonsConstIterator& operator =( const MeshPolygonsConstIterator& );
};


#endif // !__TOPOLOGY_ITERATOR_H
