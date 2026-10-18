////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Итераторы для использования range-based-for в Halfedge
         \en Iterators with range-based-for support to use in a HalfEdge class \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __HALFEDGE_RANGE_ITERATORS_H
#define __HALFEDGE_RANGE_ITERATORS_H

#include <halfedge.h>


/** \brief \ru Итератор по ребрам топологии для поддержки range-based for.
           \en Edge iterator for haldedge range based for support
*/
struct MATH_CLASS ActiveEdgeConstIterator
{
  const MbHalfedge & _topo;             //< \ru Ссылка на топологию. \en Topology reference.
  size_t             _edge = SYS_MAX_T; //< \ru Индекс текущего ребра. \en An actual edge index.

  /** \brief \ru Конструктор. \en Constructor.
    \param[in] topo - \ru Топология. \en Halfedge topology.
    \param[in] begin - \ru Если true создает начальный итератор, иначе - конечный.
                       \en If true creates begin iterator, creates end iterator otherwise.
  */
  ActiveEdgeConstIterator( const MbHalfedge & topo, bool begin = true ) noexcept;

  /// \ru Оператор инкремента. \en An increment operator.
  ActiveEdgeConstIterator & operator ++() noexcept;
  /// \ru Оператор сравнения. \en A comparison operator.
  bool operator != ( const ActiveEdgeConstIterator & it ) const noexcept;
  /// \ru Доступ к данным. \en Data access.
  const MbHalfedgeEdge & operator*() const noexcept;
};


/** \brief \ru Объект хранящий begin() и end() для итерации по вершинам для поддержки range-based for.
           \en Storage for begin-end pair to support halfedge range-based for iteration
*/
struct MATH_CLASS ActiveEdgesBegEndPair
{
  const MbHalfedge & _topo; //< \ru Ссылка на топологию. \en Topology reference.

  /// \ru Констуктор. \en Constructor.
  ActiveEdgesBegEndPair( const MbHalfedge & topo ) noexcept
    : _topo( topo )
  {}

  /// \ru Начальный итератор. \en Begin iterator.
  ActiveEdgeConstIterator begin() const noexcept
  {
    return ActiveEdgeConstIterator( _topo );
  }

  /// \ru Конечный итератор. \en End Iterator.
  ActiveEdgeConstIterator end() const noexcept
  {
    return ActiveEdgeConstIterator( _topo, false );
  }
};


/** \brief \ru Итератор по вершинам топологии для поддержки range-based for.
           \en Vertex iterator for haldedge range based for support
*/
struct MATH_CLASS ActiveVertexConstIterator
{
  const MbHalfedge & _topo;               //< \ru Ссылка на топологию. \en Topology reference.
  size_t             _vertex = SYS_MAX_T; //< \ru Индекс текущей вершины. \en An actual vertex index.


  /** \brief \ru Конструктор. \en Constructor.
    \param[in] topo - \ru Топология. \en Halfedge topology.
    \param[in] begin - \ru Если true создает начальный итератор, иначе - конечный.
                       \en If true creates begin iterator, creates end iterator otherwise.
  */
  ActiveVertexConstIterator( const MbHalfedge & topo, bool begin = true ) noexcept;

  /// \ru Оператор инкремента. \en An increment operator.
  ActiveVertexConstIterator & operator ++() noexcept;
  /// \ru Оператор сравнения. \en A comparison operator.
  bool operator != ( const ActiveVertexConstIterator & it ) const noexcept;
  /// \ru Доступ к данным. \en Data access.
  const MbHalfedgeVertex & operator*() const noexcept;
};


/** \brief \ru Объект хранящий begin() и end() для итерации по вершинам для поддержки range-based for.
           \en Storage for begin-end pair to support halfedge range-based for iteration
*/
struct MATH_CLASS ActiveVerticesBegEndPair
{
  const MbHalfedge & _topo; //< \ru Ссылка на топологию. \en Topology reference.

  /// \ru Констуктор. \en Constructor.
  ActiveVerticesBegEndPair( const MbHalfedge & topo) noexcept
    : _topo( topo )
  {}

  /// \ru Начальный итератор. \en Begin iterator.
  ActiveVertexConstIterator begin() const noexcept
  {
    return ActiveVertexConstIterator( _topo );
  }

  /// \ru Конечный итератор. \en End Iterator.
  ActiveVertexConstIterator end() const noexcept
  {
    return ActiveVertexConstIterator( _topo, false );
  }
};


/** \brief \ru Итератор по фасетам топологии для поддержки range-based for.
           \en Facet iterator for haldedge range based for support.
*/
struct MATH_CLASS ActiveFacetConstIterator
{
  const MbHalfedge & _topo;              //< \ru Ссылка на топологию. \en Topology reference.
  size_t             _facet = SYS_MAX_T; //< \ru Индекс текущего фасета. \en An actual facet index.

  /** \brief \ru Конструктор. \en Constructor.
    \param[in] topo - \ru Топология. \en Halfedge topology.
    \param[in] begin - \ru Если true создает начальный итератор, иначе - конечный.
                       \en If true creates begin iterator, creates end iterator otherwise.
  */
  ActiveFacetConstIterator( const MbHalfedge & topo, bool begin = true ) noexcept;

  /// \ru Оператор инкремента. \en An increment operator.
  ActiveFacetConstIterator & operator ++() noexcept;
  /// \ru Оператор сравнения. \en A comparison operator.
  bool operator != ( const ActiveFacetConstIterator & it ) const noexcept;
  /// \ru Доступ к данным. \en Data access.
  const MbHalfedgeFacet & operator*() const noexcept;
};


/** \brief \ru Объект хранящий begin() и end() для итерации по вершинам для поддержки range-based for.
           \en Storage for begin-end pair to support halfedge range-based for iteration
*/
struct MATH_CLASS ActiveFacetsBegEndPair
{
  const MbHalfedge & _topo; //< \ru Ссылка на топологию. \en Topology reference.

  /// \ru Констуктор. \en Constructor.
  ActiveFacetsBegEndPair( const MbHalfedge & topo ) noexcept
    : _topo( topo )
  {}

  /// \ru Начальный итератор. \en Begin iterator.
  ActiveFacetConstIterator begin() const noexcept
  {
    return ActiveFacetConstIterator( _topo );
  }

  /// \ru Конечный итератор. \en End Iterator.
  ActiveFacetConstIterator end() const noexcept
  {
    return ActiveFacetConstIterator( _topo, false );
  }
};


/** \brief \ru Базовый реберный итератор для поддержки range-based for.
           \en Base edge iterator for haldedge range based for support.
*/
struct MATH_CLASS EdgeConstIterator
{
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = MbHalfedgeEdge;
  using pointer = MbHalfedgeEdge *;
  using reference = MbHalfedgeEdge &;

  const MbHalfedge & _topo;            //< \ru Ссылка на топологию. \en Topology reference.
  bool              _bFirst = true;    //< \ru Признак того, что не нужно проверять окончание итерации. \en If true there is no need to check the end of the iteration.
  size_t            _edge = SYS_MAX_T; //< \ru Индекс текущего ребра. \en An actual edge index.

  /** \brief \ru Конструктор из ребра. \en Construct from edge.
    \param[in] topo - \ru Топология. \en Halfedge topology.
    \param[in] iVx - \ru Индекс вершины. \en Vertex id.
    \param[in] begin - \ru Если true создает начальный итератор, иначе - конечный.
                       \en If true creates begin iterator, creates end iterator otherwise.
  */
  EdgeConstIterator( const MbHalfedge & topo, size_t iEd, bool begin = true ) noexcept;

  /// \ru Оператор сравнения. Для поддержки итерации на границе пустое ребро считается равным любому другому.
  /// \en Comparison operator. Empty edge is identical to any other edge to support iteration on open boundaries.
  bool operator != ( const EdgeConstIterator & it ) const noexcept;

  /// \ru Доступ к данным. \en Data access.
  const MbHalfedgeEdge & operator*() const noexcept;
  const MbHalfedgeEdge * operator->() const noexcept;
};


/** \brief \ru Итератор по ребрам вокруг фасета для поддержки range-based for.
           \en Edge around facet iterator for haldedge range based for support.
*/
struct MATH_CLASS EdgeFacetConstIterator : EdgeConstIterator
{
  /// \ru Конструктор. \en Constructor.
  EdgeFacetConstIterator( const MbHalfedge & topo, size_t iF, bool begin = true ) noexcept;
  /// \ru Оператор инкремента. \en An increment operator.
  EdgeFacetConstIterator & operator ++() noexcept;
};


/** \brief \ru Объект хранящий begin() и end() для итерации по ребрам вокруг фасета для поддержки range-based for.
           \en Storage for begin-end pair to support halfedge range-based for iteration .
*/
struct MATH_CLASS EdgesAroundFacetBegEndPair
{
  const MbHalfedge & _topo;  //< \ru Ссылка на топологию. \en Topology reference.
  size_t             _facet; //< \ru Индекс фасета. \en A facet index.

  /// \ru Конструктор. \en Constructor.
  EdgesAroundFacetBegEndPair( const MbHalfedge & topo, size_t iF ) noexcept
    : _topo( topo ), _facet( iF )
  {}

  /// \ru Начальный итератор. \en Begin iterator.
  EdgeFacetConstIterator begin() const noexcept
  {
    return EdgeFacetConstIterator( _topo, _facet );
  }

  /// \ru Конечный итератор. \en End Iterator.
  EdgeFacetConstIterator end() const noexcept
  {
    return EdgeFacetConstIterator( _topo, _facet, false );
  }
};


/** \brief \ru Итератор по вершинам вокруг фасета для поддержки range-based for.
           \en Vertex around facet iterator for haldedge range based for support.
*/
struct MATH_CLASS VertexFacetConstIterator : EdgeFacetConstIterator
{
  using value_type = MbHalfedgeEdge;
  using pointer = MbHalfedgeEdge *;
  using reference = MbHalfedgeEdge &;

  /// \ru Конструктор. \en Constructor.
  VertexFacetConstIterator( const MbHalfedge & topo, size_t iF, bool begin = true ) noexcept;
  /// \ru Доступ к данным. \en Data access.
  const MbHalfedgeVertex & operator*() const noexcept;
  const MbHalfedgeVertex * operator->() const noexcept;
};


/** \brief \ru Объект хранящий begin() и end() для итерации по вершинам вокруг фасета для поддержки range-based for.
           \en Storage for begin-end pair to support halfedge range-based for iteration .
*/
struct MATH_CLASS VerticesAroundFacetBegEndPair
{
  const MbHalfedge & _topo;  //< \ru Ссылка на топологию. \en Topology reference.
  size_t             _facet; //< \ru Индекс фасета. \en A facet index.

  /// \ru Конструктор. \en Constructor.
  VerticesAroundFacetBegEndPair( const MbHalfedge & topo, size_t iF ) noexcept
    : _topo( topo ), _facet( iF )
  {}

  /// \ru Начальный итератор. \en Begin iterator.
  VertexFacetConstIterator begin() const noexcept
  {
    return VertexFacetConstIterator( _topo, _facet );
  }

  /// \ru Конечный итератор. \en End Iterator.
  VertexFacetConstIterator end() const noexcept
  {
    return VertexFacetConstIterator( _topo, _facet, false );
  }
};


/** \brief \ru Итератор по исходящим ребрам фана вершины для поддержки range-based for.
           \en Outgoing edges in the vertex fan iterator for haldedge range based for support.
*/
struct MATH_CLASS OutgoingEdgeConstIterator : EdgeConstIterator
{
  /// \ru Конструктор. \en Constructor.
  OutgoingEdgeConstIterator( const MbHalfedge & topo, size_t iEd, bool begin = true ) noexcept;
  /// \ru Оператор инкремента. \en An increment operator.
  OutgoingEdgeConstIterator & operator ++() noexcept;
};


/** \brief \ru Итератор по фасетам вокруг вершины для поддержки range-based for.
           \en Facets around the vertex iterator for haldedge range based for support.
*/
struct MATH_CLASS FacetVertexConstIterator : OutgoingEdgeConstIterator
{
  /// \ru Конструктор. \en Constructor.
  FacetVertexConstIterator( const MbHalfedge & topo, size_t iVx, bool begin = true ) noexcept;
  /// \ru Доступ к данным. \en Data access.
  const MbHalfedgeFacet & operator*() const noexcept;
};


/** \brief \ru Объект хранящий begin() и end() для итерации по фасетам вокруг вершины для поддержки range-based for.
           \en Storage for begin-end pair to support halfedge range-based for iteration .
*/
struct MATH_CLASS FacetsAroundVertexBegEndPair
{
  const MbHalfedge & _topo;   //< \ru Ссылка на топологию. \en Topology reference.
  size_t             _vertex; //< \ru Индекс вершины. \en A vertex index.

  /// \ru Конструктор. \en Constructor.
  FacetsAroundVertexBegEndPair( const MbHalfedge & topo, size_t iVx ) noexcept
    : _topo( topo ), _vertex( iVx )
  {
  }

  /// \ru Начальный итератор. \en Begin iterator.
  FacetVertexConstIterator begin() const noexcept
  {
    return FacetVertexConstIterator( _topo, _vertex );
  }

  /// \ru Конечный итератор. \en End Iterator.
  FacetVertexConstIterator end() const noexcept
  {
    return FacetVertexConstIterator{ _topo, _vertex, false };
  }
};


/** \brief \ru Итератор по всем ребрам фана вершины для поддержки range-based for.
           \en All edges in the vertex fan iterator for haldedge range based for support.
*/
struct MATH_CLASS EdgeVertexConstIterator : EdgeConstIterator
{
  bool _outgoing = true; //< \ru Признак исходящего ребра.

  /// \ru Конструктор. \en Constructor.
  EdgeVertexConstIterator( const MbHalfedge & topo, size_t iVx, bool begin = true ) noexcept;
  /// \ru Оператор инкремента. \en An increment operator.
  EdgeVertexConstIterator & operator ++() noexcept;
};


/** \brief \ru Объект хранящий begin() и end() для итерации по ребрам вокруг вершины для поддержки range-based for.
           \en Storage for begin-end pair to support halfedge range-based for iteration .
*/
struct MATH_CLASS EdgesAroundVertexBegEndPair
{
  const MbHalfedge & _topo;   //< \ru Ссылка на топологию. \en Topology reference.
  size_t             _vertex; //< \ru Индекс вершины. \en A vertex index.

  /// \ru Конструктор. \en Constructor.
  EdgesAroundVertexBegEndPair( const MbHalfedge & topo, size_t iVx ) noexcept
    : _topo( topo ), _vertex( iVx )
  {
  }

  /// \ru Начальный итератор. \en Begin iterator.
  EdgeVertexConstIterator begin() const noexcept
  {
    return EdgeVertexConstIterator( _topo, _vertex );
  }

  /// \ru Конечный итератор. \en End Iterator.
  EdgeVertexConstIterator end() const noexcept
  {
    return EdgeVertexConstIterator{ _topo, _vertex, false };
  }
};


/** \brief \ru Итерация по всем смещным вершинам вершины для поддержки range-based for.
           \en All vertex adjacent vertices iterator for haldedge range based for support.
*/
struct MATH_CLASS VertexVertexConstIterator : EdgeVertexConstIterator
{
  /// \ru Конструктор. \en Constructor.
  VertexVertexConstIterator( const MbHalfedge & topo, size_t iVx, bool begin = true ) noexcept;
  /// \ru Оператор инкремента. \en An increment operator.
  VertexVertexConstIterator & operator ++() noexcept;
  /// \ru Доступ к данным. \en Data access.
  const MbHalfedgeVertex & operator*() const noexcept;
};


/** \brief \ru Объект хранящий begin() и end() для итерации по вершинам вокруг вершины для поддержки range-based for.
           \en Storage for begin-end pair to support halfedge range-based for iteration .
*/
struct MATH_CLASS VerticesAroundVertexBegEndPair
{
  const MbHalfedge & _topo;   //< \ru Ссылка на топологию. \en Topology reference.
  size_t             _vertex; //< \ru Индекс вершины. \en A vertex index.

  /// \ru Конструктор. \en Constructor.
  VerticesAroundVertexBegEndPair( const MbHalfedge & topo, size_t iVx ) noexcept
    : _topo( topo ), _vertex( iVx )
  {
  }

  /// \ru Начальный итератор. \en Begin iterator.
  VertexVertexConstIterator begin() const noexcept
  {
    return VertexVertexConstIterator( _topo, _vertex );
  }

  /// \ru Конечный итератор. \en End Iterator.
  VertexVertexConstIterator end() const noexcept
  {
    return VertexVertexConstIterator{ _topo, _vertex, false };
  }
};

#endif // __HALFEDGE_RANGE_ITERATORS_H
