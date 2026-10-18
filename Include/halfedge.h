////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Топология полигональной сетки в виде half edge.
         \en Polygonal mesh topology represented as half edge. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __HALFEDGE_H
#define __HALFEDGE_H

#include <halfedge_element.h>
#include <mesh_halfedge_attr.h>
#include <mesh_halfedge_segm_attr.h>

struct ActiveEdgesBegEndPair;
struct ActiveVerticesBegEndPair;
struct ActiveFacetsBegEndPair;
struct EdgesAroundFacetBegEndPair;
struct VerticesAroundFacetBegEndPair;
struct EdgesAroundVertexBegEndPair;
struct VerticesAroundVertexBegEndPair;
struct FacetsAroundVertexBegEndPair;

//-------------------------------------------------------------------------------
/** \brief \ru Топология полигональной сетки в виде half edge.
           \en Polygonal mesh topology represented as half edge.
*/
// ---
class MATH_CLASS MbHalfedge
{
protected:
  std::vector<std::unique_ptr<MbHalfedgeAttrBase>> _attributes; ///< \ru Атрибуты вершин, ребер и фасетов, ассоциированные с сеткой. \en Vertex, edge, and facet attributes associated with a mesh.
  std::vector<MbHalfedgeVertex>                    _vertices;   ///< \ru Массив вершин. \en Vertex array.
  std::vector<MbHalfedgeEdge>                      _edges;      ///< \ru Массив ребер. \en Edge array.
  std::vector<MbHalfedgeFacet>                     _facets;     ///< \ru Массив фасетов. \en Facet array.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbHalfedge();
  /// \ru Деструктор. \en Destructor.
  virtual ~MbHalfedge();

  /// \ru Получить количество ребер в сетке (включая неактивные). \en Get the number of edges in the mesh (including inactive ones).
  size_t EdgeCount() const { return _edges.size(); }
  /// \ru Получить ребро по индексу. \en Get edge by index.
  MbHalfedgeEdge & Edge( size_t iEd ) { return _edges[iEd]; }
  /// \ru Получить ребро по индексу. \en Get edge by index.
  const MbHalfedgeEdge & Edge( size_t iEd ) const { return _edges[iEd]; }

  /// \ru Получить количество вершин в сетке (включая неактивные). \en Get the number of vertices in the mesh (including inactive ones).
  size_t VertexCount() const { return _vertices.size(); }
  /// \ru Получить вершину по индексу. \en Get vertex by index.
  MbHalfedgeVertex & Vertex( size_t iVx ) { return _vertices[iVx]; }
  /// \ru Получить вершину по индексу. \en Get vertex by index.
  const MbHalfedgeVertex & Vertex( size_t iVx ) const { return _vertices[iVx]; }

  /// \ru Получить количество фасетов в сетке (включая неактивные). \en Get the number of facets in the mesh (including inactive ones).
  size_t FacetCount() const { return _facets.size(); }
  /// \ru Получить фасет по индексу. \en Get facet by index.
  MbHalfedgeFacet & Facet( size_t iF ) { return _facets[iF]; }
  /// \ru Получить фасет по индексу. \en Get facet by index.
  const MbHalfedgeFacet & Facet( size_t iF ) const { return _facets[iF]; }

  /// \ru Пара range-based for итераторов для перебора всех активных ребер. \en Range based for pair for all alive edges.
  ActiveEdgesBegEndPair AllActiveEdges() const;
  /// \ru Пара range-based for итераторов для перебора всех активных вершин. \en Range based for pair for all alive vertices.
  ActiveVerticesBegEndPair AllActiveVertices() const;
  /// \ru Пара range-based for итераторов для перебора всех активных вершин. \en Range based for pair for all alive vertices.
  ActiveFacetsBegEndPair AllActiveFacets() const;

  /// \ru Пара range-based for итераторов для перебора всех ребер вокруг фасета. \en Range based for pair for all edges around the facet.
  EdgesAroundFacetBegEndPair AllEdgesAroundFacet( size_t iF ) const;
  /// \ru Пара range-based for итераторов для перебора всех вершин вокруг фасета. \en Range based for pair for all vertices around the facet.
  VerticesAroundFacetBegEndPair AllVerticesAroundFacet( size_t iF ) const;
  /// \ru Пара range-based for итераторов для перебора всех ребер в фане вершины. \en Range based for pair for all edges in the vertex fan.
  EdgesAroundVertexBegEndPair AllEdgesAroundVertex( size_t iV ) const;
  /// \ru Пара range-based for итераторов для перебора всех вершин вокруг вершины. \en Range based for pair for all vertices around the vertex.
  VerticesAroundVertexBegEndPair AllVerticesAroundVertex( size_t iV ) const;
  /// \ru Пара range-based for итераторов для перебора всех фасетов вокруг вершины. \en Range based for pair for all facets around the vertex.
  FacetsAroundVertexBegEndPair AllFacetsAroundVertex( size_t iV ) const;

  /// \ru Получить следующее ребро в фане вершины по или против часовой стрелки. \en Get the next edge in the vertex fan clockwise or counterclockwise.
  size_t NextFanEdge( size_t iEd, bool bClockwise ) const;
  /// \ru Получить следующую пару ребер (исходящее из вершины и его близнец при наличии) в фане вершины по или против часовой стрелки. 
  /// \en Get the next edge pair (ingoing and outgoing twin if present) in the vertex fan clockwise or counterclockwise.
  void NextFanEdgePair( size_t iEd, bool bClockwise, c3d::IndicesPair & pair ) const;
  /// \ru Получить последнее ребро в фане вершины по часовой стрелки. \en Get the last edge in the vertex fan in a clockwise direction.
  size_t FindMostClockwiseFanEdge( size_t iEd ) const;
  /// \ru Получить индексы вершин в фасете, если он является треугольным. \en Get the indices of facet vertices if triangular one.
  bool GetTriangleVertices( size_t f, std::array<size_t, 3> & vertices ) const;
  /// \ru Получить индексы ребер в фасете, если он является треугольным. \en Get the indices of facet edges if triangular one.
  bool GetTriangleEdges( size_t f, std::array<size_t, 3> & edges ) const;
  /// \ru Получить последовательность ребер в фасете. \en Get the sequence of facet edges.
  void GetFacetEdges( size_t iF, c3d::IndicesVector & edges ) const;
  /// \ru Получить последовательность вершин в фасете. \en Get the sequence of facet vertices.
  void GetFacetVertices( size_t f, c3d::IndicesVector & vertices ) const;

  /// \ru Получить количество ассоциированных атрибутов. \en Get the number of associated attributes.
  size_t AttrCount() const { return _attributes.size(); }

  /// \ru Добавить атрибут заданного типа. \en Add an attribute of the specified type.
  template<class T>
  T * AddHalfedgeAttribute()
  {
    _attributes.emplace_back( new T( *this ) );
    _attributes.back()->SetId( _attributes.size() - 1 );
    return CastHalfedgeAttribute<T>( _attributes.back().get() );
  }

  /// \ru Получить атрибут заданного типа. \en Get an attribute of the specified type.
  template<class T>
  T * GetHalfedgeAttribute()
  {
    for ( size_t i = 0, n = _attributes.size(); i < n; ++i )
    {
      if ( _attributes[i] )
      {
        T * pAttr = CastHalfedgeAttribute<T>( _attributes[i].get() );
        if ( pAttr != nullptr )
          return pAttr;
      }
    }
    return nullptr;
  }

  /// \ru Получить атрибут заданного типа. \en Get an attribute of the specified type.
  template<class T>
  const T * GetHalfedgeAttribute() const
  {
    for ( size_t i = 0, n = _attributes.size(); i < n; ++i )
    {
      if ( _attributes[i] )
      {
        const T * pAttr = CastHalfedgeAttribute<T>( _attributes[i].get() );
        if ( pAttr != nullptr )
          return pAttr;
      }
    }
    return nullptr;
  }

  /// \ru Получить атрибут по индексу. \en Get an attribute by index.
  MbHalfedgeAttrBase * GetHalfedgeAttributeByIndex( size_t ind )
  {
    MbHalfedgeAttrBase * pAttr( nullptr );
    if ( ind < _attributes.size() )
      pAttr = _attributes[ind].get();
    return pAttr;
  }

  /// \ru Получить атрибут по индексу. \en Get an attribute by index.
  const MbHalfedgeAttrBase * GetHalfedgeAttributeByIndex( size_t ind ) const
  {
    MbHalfedgeAttrBase * pAttr( nullptr );
    if ( ind < _attributes.size() )
      pAttr = _attributes[ind].get();
    return pAttr;
  }

  /// \ru Удалить атрибут заданного типа. \en Remove an attribute of the specified type.
  template<class T>
  void DeleteHalfedgeAttribute()
  {
    std::vector<T *> toDel;
    for ( size_t i = 0, n = _attributes.size(); i < n; ++i )
    {
      if ( _attributes[i] )
      {
        T * pAttr = CastHalfedgeAttribute<T>( _attributes[i].get() );
        if ( pAttr != nullptr )
          toDel.push_back( pAttr );
      }
    }

    for ( auto * pAttr : toDel )
      DeleteHalfedgeAttribute( pAttr );
  }

  /// \ru Удалить атрибут. \en Remove the attribute.
  void DeleteHalfedgeAttribute( MbHalfedgeAttrBase * pAttr );

  /// \ru Удалить атрибут с заданным индексом. \en Remove an attribute with the given index.
  void DeleteHalfedgeAttribute( size_t ind );

  OBVIOUS_PRIVATE_COPY( MbHalfedge );
};


//-------------------------------------------------------------------------------
/** \brief \ru Базовый интерфейс критерия границы региона фасетов.
           \en Basic facet region boundary criterion interface.
*/
// ---
class MATH_CLASS MbHalfedgeBoundaryCriterionBase
{
public:
  /// \ru Конструктор. \en Constructor.
  MbHalfedgeBoundaryCriterionBase() {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbHalfedgeBoundaryCriterionBase() {}

  /// \ru Проверить, является ли ребро граничным. \en Check whether an edge is a boundary edge.
  virtual bool IsBoundaryEdge( size_t e ) const = 0;
  OBVIOUS_PRIVATE_COPY( MbHalfedgeBoundaryCriterionBase );
};


//-------------------------------------------------------------------------------
/** \brief \ru Критерий границы помеченной с помощью реберного атрибута булевых пометок.
           \en A facet region boundary criterion defined through edge boolean marks attribute.
*/
// ---
class MbHalfedgeBoundaryCriterionMarked : public MbHalfedgeBoundaryCriterionBase
{
private:
  const MbHalfedge                              & _topo; /// \ru Топология. \en Topology.
  const MbHalfedgeAttrSimpleValueEdgeBase<bool> & _mark; /// \ru Атрибут булевых пометок. \en Boolean marks attribute.

public:
  /// \ru Конструктор. \en Constructor.
  MbHalfedgeBoundaryCriterionMarked( const MbHalfedgeAttrSimpleValueEdgeBase<bool> & mark, const MbHalfedge & topo )
    : MbHalfedgeBoundaryCriterionBase()
    , _topo( topo )
    , _mark( mark ) {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbHalfedgeBoundaryCriterionMarked() {}

  /// \ru Проверить, является ли ребро граничным. \en Check whether an edge is a boundary edge.
  bool IsBoundaryEdge( size_t e ) const override
  {
    const auto & ed = _topo.Edge( e );
    return ed.IsActive() ? ( !ed.IsTwinDefined() || _mark.GetValue( e ) ) : false;
  }

  OBVIOUS_PRIVATE_COPY( MbHalfedgeBoundaryCriterionMarked );
};


#endif // __HALFEDGE_H
