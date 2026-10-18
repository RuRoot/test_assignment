////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Итераторы доступа к полигональному геометрическому объекту.
         \en Access iterators to the polygonal geometric object. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __HALFEDGE_ITERATORS_H
#define __HALFEDGE_ITERATORS_H

#include <halfedge.h>

/** \brief \ru Итератор ребер фана вершины против часовой стрелки. Не проверяет идентификатор вершины на валидность.
           \en Counterclockwise iterator of vertex fan edges. Does not verify given vertex validity.
*/
class MATH_CLASS EdgeFanIterator
{
private:
  const MbHalfedge &         _topo;   //< \ru Ссылка на топологию. \en The topology reference.
  bool                       _bFirst; //< \ru Признак того, что не нужно проверять окончание итерации. \en If true there is no need to check the end of the iteration.
  c3d::IndicesPair           _edge;   //< \ru Индекс текущего ребра и его обратное ребро. \en The current edge and it's twin.
  size_t                     _edge0;  //< \ru Индекс начального ребра для проверки окончания итерации для внутренних вершин. \en Start edge for the end of iteration check.

public:
  /** \brief \ru Конструктор по вершине. \en Construct from vertex.
    \param[in] topo - \ru Топология. \en Halfedge topology.
    \param[in] iVx - \ru Индекс вершины. \en Vertex id.
    \param[in] bSure - \ru Признак упрощенной инициализации. True предполагает, что при
                           создании и модификации топологии ребра граничных вершин установлены в открытые ребра их фана,
                           и проверять это при инициализации не нужно.
                       \en True if simplified iteration is allowed. For simplified iteration a boundary vertex always stores a boundary edge.
  */
  EdgeFanIterator( const MbHalfedge & topo, size_t iVx, bool bSure );
  /** \brief \ru Конструктор по ребру. \en Construct from edge.
    \param[in] topo - \ru Топология. \en Halfedge topology.
    \param[in] iEd - \ru Индекс ребра. \en Edge id.
  */
  EdgeFanIterator( const MbHalfedge & topo, size_t iEd );
  /// \ru Деструктор. \en Destructor.
  ~EdgeFanIterator() {}

  /// \ru Сдвинуть итератор на следующее ребро фана. \en Increment operator moves iterator to the next edge in the vertex fan.
  void  operator ++()
  {
    _bFirst = false;
    if ( _edge.first == SYS_MAX_T )
      _edge.second = SYS_MAX_T;
    else
      _topo.NextFanEdgePair( _edge.first, false, _edge );
  }
  /// \ru Получить индекс текущей пары ребер. \en Get indices of edges for the current iterator position.
  const c3d::IndicesPair & EdgePair() const { return _edge; }
  /// \ru Получить индекс исходящего ребра. \en Get an index of the outgoing edge.
  size_t Edge() const
  {
    return _edge.first;
  }
  /// \ru Получить индекс входящего ребра. \en Get an index of the incoming edge.
  size_t Twin() const
  {
    return _edge.second;
  }
  /// \ru Получить индекс исходной вершины. \en Get an index of the origin vertex.
  size_t OriginVertex() const
  {
    size_t vx = SYS_MAX_T;
    if ( _edge.first != SYS_MAX_T )
      vx = _topo.Edge( _edge.first ).Vertex();
    else if ( _edge.second != SYS_MAX_T )
      vx = _topo.Edge( _topo.Edge( _edge.second ).Next() ).Vertex();
    return vx;
  }
  /// \ru Получить индекс противолежащей по ребру вершины. \en Get an index of the destination vertex.
  size_t DestinationVertex() const
  {
    size_t vx = SYS_MAX_T;
    if ( _edge.second != SYS_MAX_T )
      vx = _topo.Edge( _edge.second ).Vertex();
    else if ( _edge.first != SYS_MAX_T )
      vx = _topo.Edge( _topo.Edge( _edge.first ).Next() ).Vertex();
    return vx;
  }
  /// \ru Закончен ли обход. \en Check if all edges in the fan were visited during iteration.
  bool Done() const
  {
    return _bFirst ? false
      : _edge.second == SYS_MAX_T || _edge.first == _edge0;
  }

  OBVIOUS_PRIVATE_COPY( EdgeFanIterator );
};


/** \brief \ru Итератор по ребрам фасета. Не проверяет идентификатор фасета на валидность.
           \en Facet's edge loop iterator. Does not verify given facet validity.
*/
class MATH_CLASS EdgeFacetIterator
{
private:
  const MbHalfedge &         _topo;   //< \ru Ссылка на топологию. \en The topology reference.
  bool                       _bFirst; //< \ru Признак того, что не нужно проверять окончание итерации. \en If true there is no need to check the end of the iteration.
  size_t                     _edge;   //< \ru Индекс текущего ребра. \en The current edge id.
  size_t                     _edge0;  //< \ru Индекс начального ребра для проверки окончания итерации. \en Start edge for the end of iteration check.

public:
  /** \brief \ru Конструктор. \en Constructor.
    \param[in] topo - \ru Топология. \en Halfedge topology.
    \param[in] iFacet - \ru Индекс фасета. \en Facet id.
  */
  EdgeFacetIterator( const MbHalfedge & topo, size_t iFacet );
  /// \ru Деструктор. \en Destructor.
  ~EdgeFacetIterator() {}

  /// \ru Сдвинуть итератор на следующее ребро фасета. \en Increment operator moves iterator to the next edge in the facet loop.
  void  operator ++()
  {
    _bFirst = false;
    _edge = _topo.Edge( _edge ).Next();
  }
  /// \ru Получить индекс текущего ребра. \en Get the current edge id.
  size_t Edge() const
  {
    return _edge;
  }
  /// \ru Закончен ли обход. \en Check if all edges in the loop were visited during iteration.
  bool Done() const
  {
    return _bFirst ? false
      : _edge == _edge0;
  }

  OBVIOUS_PRIVATE_COPY( EdgeFacetIterator );
};

#endif // __HALFEDGE_ITERATORS_H
