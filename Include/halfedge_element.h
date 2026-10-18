////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Элементарные структуры для построения топологии полигональной сетки в виде half-edge.
         \en Elementary units to construct the topology of a polygonal mesh as a half-edge. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __HALFEDGE_ELEMENT_H
#define __HALFEDGE_ELEMENT_H

#include <math_define.h>
#include <io_tape.h>


//-------------------------------------------------------------------------------
/** \brief \ru Базовый класс элемента топологии полигональной сетки.
           \en Base class for a polygon mesh topology elementary unit.
*/
// ---
class MATH_CLASS MbHalfedgeElement
{
protected:
  size_t _id; ///< \ru Идентификатор (индекс в массиве). \en Identifier (index in the array).

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbHalfedgeElement() : _id( SYS_MAX_T ) {}
  /// \ru Конструктор копирования. \en Copy constructor.
  MbHalfedgeElement( const MbHalfedgeElement & that ) : _id( that._id ) {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbHalfedgeElement() {}

  /// \ru Оператор присваивания. \en Assignment operator.
  MbHalfedgeElement & operator = ( const MbHalfedgeElement & that )
  {
    _id = that._id;
    return ( *this );
  }

  /// \ru Получить идентификатор. \\ Get an identifier.
  size_t Id() const { return _id; }
  /// \ru Установить идентификатор. \\ Set an identifier.
  void SetId( size_t id ) { _id = id; }
  /// \ru Является ли объект активным. \\ Check whether an object is in the active state.
  bool IsActive() const { return _id < SYS_MAX_T; }
};


//-------------------------------------------------------------------------------
/** \brief \ru Вершина полигональной сетки.
           \en Polygonal mesh vertex.
*/
// ---
class MATH_CLASS MbHalfedgeVertex : public MbHalfedgeElement
{
private:
  size_t _ed; ///< \ru Идентификатор ребра, выходящего из вершины (любого из выходящих). \en Any outgoing edge identifier.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbHalfedgeVertex() : MbHalfedgeElement(), _ed( SYS_MAX_T ) {}
  /// \ru Конструктор копирования. \en Copy constructor.
  MbHalfedgeVertex( const MbHalfedgeVertex & that ) : MbHalfedgeElement( that ), _ed( that._ed ) {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbHalfedgeVertex() {}

  /// \ru Оператор присваивания. \en Assingment operator.
  MbHalfedgeVertex & operator = ( const MbHalfedgeVertex & that )
  {
    MbHalfedgeElement::operator=( that );
    _ed = that._ed;
    return ( *this );
  }

  /// \ru Обнулить вершину. \en Clear the vertex.
  void Clear();
  /// \ru Получить ребро вершины. \en Get an outgoing edge identifier.
  size_t Edge() const { return _ed; }
  /// \ru Установить ребро вершины. \en Set an outgoing edge identifier.
  void SetEdge( size_t ed ) { _ed = ed; }
  /// \ru Определено ли ребро вершины. \en Check if an outgoing edge is defined.
  bool IsEdgeDefined() const { return _ed < SYS_MAX_T; }

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbHalfedgeVertex, MATH_FUNC_EX );
  DECLARE_NEW_DELETE_CLASS( MbHalfedgeVertex );
  DECLARE_NEW_DELETE_CLASS_EX( MbHalfedgeVertex );
};


//-------------------------------------------------------------------------------
/** \brief \ru Ребро полигональной сетки.
           \en Polygonal mesh edge.
*/
// ---
class MATH_CLASS MbHalfedgeEdge : public MbHalfedgeElement
{
private:
  size_t _prev;   ///< \ru Идентификатор предыдущего ребра в цикле. \en Identifier of the previous edge in the cycle.
  size_t _next;   ///< \ru Идентификатор следующего ребра в цикле. \en Identifier of the next edge in the cycle.
  size_t _twin;   ///< \ru Идентификатор "близнеца" ребра. \en Identifier of the twin edge.
  size_t _vertex; ///< \ru Идентификатор вершины, из которой выходит ребро. \en Identifier of the vertex for which the edge is outgoing.
  size_t _facet;  ///< \ru Идентификатор фасета, к циклу которого принадлежит ребро. \en Identifier of the facet whose cycle the edge belongs to.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbHalfedgeEdge()
    : MbHalfedgeElement()
    , _prev( SYS_MAX_T )
    , _next( SYS_MAX_T )
    , _twin( SYS_MAX_T )
    , _vertex( SYS_MAX_T )
    , _facet( SYS_MAX_T )
  {}
  /// \ru Конструктор копирования. \en Copy constructor.
  MbHalfedgeEdge( const MbHalfedgeEdge & that )
    : MbHalfedgeElement( that )
    , _prev( that._prev )
    , _next( that._next )
    , _twin( that._twin )
    , _vertex( that._vertex )
    , _facet( that._facet )
  {}
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbHalfedgeEdge() {}

  /// \ru Оператор присваивания. \en Assingnment operator.
  MbHalfedgeEdge & operator = ( const MbHalfedgeEdge & that )
  {
    MbHalfedgeElement::operator=( that );
    _prev = that._prev;
    _next = that._next;
    _twin = that._twin;
    _vertex = that._vertex;
    _facet = that._facet;
    return ( *this );
  }

  /// \ru Обнулить ребро. \en Clear the edge.
  void Clear();
  /// \ru Получить предыдущее ребро цикла. \en Get a previous edge in the cycle.
  size_t Prev() const { return _prev; }
  /// \ru Получить следующее ребро цикла. \en Get a next edge in the cycle.
  size_t Next() const { return _next; }
  /// \ru Получить ребро-близнеца. \en Get a twin edge.
  size_t Twin() const { return _twin; }
  /// \ru Получить вершину. \en Get an edge vertex.
  size_t Vertex() const { return _vertex; }
  /// \ru Получить фасет. \en Get an edge facet.
  size_t Facet() const { return _facet; }

  /// \ru Определено ли предыдущее ребро. \en Check if the previous edge is defined.
  bool IsPrevDefined() const { return _prev < SYS_MAX_T; }
  /// \ru Определено ли следующее ребро. \en Check if the next edge is defined.
  bool IsNextDefined() const { return _next < SYS_MAX_T; }
  /// \ru Определено ли ребро-близнец. \en Check if the twin edge is defined.
  bool IsTwinDefined() const { return _twin < SYS_MAX_T; }
  /// \ru Определена ли вершина ребра. \en Check if the edge vertex is defined.
  bool IsVertexDefined() const { return _vertex < SYS_MAX_T; }
  /// \ru Определен ли фасет ребра. \en Check if the edge facet is defined.
  bool IsFacetDefined() const { return _facet < SYS_MAX_T; }

  /// \ru Установить предыдущее ребро. \en Set the previous edge in the cycle.
  void SetPrev( size_t ed ) { _prev = ed; }
  /// \ru Установить следующее ребро. \en Set the next edge in the cycle.
  void SetNext( size_t ed ) { _next = ed; }
  /// \ru Установить ребро-близнеца. \en Set the twin edge.
  void SetTwin( size_t ed ) { _twin = ed; }
  /// \ru Установить вершину. \en Set the edge vertex.
  void SetVertex( size_t vx ) { _vertex = vx; }
  /// \ru Установить фасет. \en Set the edge facet.
  void SetFacet( size_t f ) { _facet = f; }
  /// \ru Убрать ссылку на близнеца. \en Remove the twin edge.
  void ClearTwin() { _twin = SYS_MAX_T; }

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbHalfedgeEdge, MATH_FUNC_EX );
  DECLARE_NEW_DELETE_CLASS( MbHalfedgeEdge );
  DECLARE_NEW_DELETE_CLASS_EX( MbHalfedgeEdge );
};


//-------------------------------------------------------------------------------
/** \brief \ru Фасет полигональной сетки.
           \en Polygonal mesh facet.
*/
// ---
class MATH_CLASS MbHalfedgeFacet : public MbHalfedgeElement
{
private:
  size_t _ed; ///< \ru Идентификатор ребра, входящего в цикл фасета (любого из входящих). \en Identifier of any edge in the facet cycle.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbHalfedgeFacet() : MbHalfedgeElement(), _ed( SYS_MAX_T ) {}
  /// \ru Конструктор копирования. \en Copy constructor.
  MbHalfedgeFacet( const MbHalfedgeFacet & that ) : MbHalfedgeElement( that ), _ed( that._ed ) {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbHalfedgeFacet() {}

  /// \ru Оператор присваивания. \en Assingment operator.
  MbHalfedgeFacet & operator = ( const MbHalfedgeFacet & that )
  {
    MbHalfedgeElement::operator=( that );
    _ed = that._ed;
    return ( *this );
  }

  /// \ru Обнулить фасет. \en Clear the facet.
  void Clear();
  /// \ru Получить ребро фасета. \en Get a facet edge.
  size_t Edge() const { return _ed; }
  /// \ru Установить ребро фасета. \en Set the facet edge.
  void SetEdge( size_t ed ) { _ed = ed; }
  /// \ru Определено ли ребро фасета. \en Check if the facet edge is defined.
  bool IsEdgeDefined() const { return _ed < SYS_MAX_T; }

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbHalfedgeFacet, MATH_FUNC_EX );
  DECLARE_NEW_DELETE_CLASS( MbHalfedgeFacet );
  DECLARE_NEW_DELETE_CLASS_EX( MbHalfedgeFacet );
};


//-------------------------------------------------------------------------------
/** \brief \ru Цикл в сегментации полигональной сетки.
           \en Polygonal mesh segmentation cycle.
*/
// ---
class MATH_CLASS MbSegmentationLoop : public MbHalfedgeElement
{
private:
  size_t _prev;          ///< \ru Идентификатор предыдущего цикла. \en Previous cycle identifier.
  size_t _next;          ///< \ru Идентификатор следующего цикла. \en Next cycle identifier
  size_t _subRegion;     ///< \ru Идентификатор подрегиона, в который цикл входит. \en Identifier of the subregion whose cycle the edge belongs to.
  size_t _ed;            ///< \ru Идентификатор первого ребра цикла. \en First edge of the cycle.
  bool   _hasInnerEdges; ///< \ru Признак наличия в цикле внутренних ребер. \en An indicator of inner edges presence in the cycle.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbSegmentationLoop()
    : MbHalfedgeElement()
    , _prev( SYS_MAX_T )
    , _next( SYS_MAX_T )
    , _subRegion( SYS_MAX_T )
    , _ed( SYS_MAX_T )
    , _hasInnerEdges( false )
  {}
  /// \ru Конструктор копирования. \en Copy constructor.
  MbSegmentationLoop( const MbSegmentationLoop & that )
    : MbHalfedgeElement( that )
    , _prev( that._prev )
    , _next( that._next )
    , _subRegion( that._subRegion )
    , _ed( that._ed )
    , _hasInnerEdges( that._hasInnerEdges )
  {}
  /// \ru Деструктор. \en Destructor
  virtual ~MbSegmentationLoop() {}

  /// \ru Оператор присваивания. \en Assignment operator.
  MbSegmentationLoop & operator = ( const MbSegmentationLoop & that )
  {
    MbHalfedgeElement::operator=( that );
    _prev = that._prev;
    _next = that._next;
    _subRegion = that._subRegion;
    _ed = that._ed;
    _hasInnerEdges = that._hasInnerEdges;
    return ( *this );
  }

  /// \ru Обнулить цикл. \en Clear the cycle.
  void Clear();
  /// \ru Получить предыдущий цикл. \en Get a previous cycle.
  size_t Prev() const { return _prev; }
  /// \ru Получить следующий цикл. \en Get a next cycle.
  size_t Next() const { return _next; }
  /// \ru Получить первое ребро цикла. \en Get a cycle's first edge.
  size_t Edge() const { return _ed; }
  /// \ru Получить идентификатор подрегиона, к которому принадлежит цикл. \en Get parent subregion.
  size_t SubRegion() const { return _subRegion; }
  /// \ru Есть ли в цикле внутренние ребра. \en Check if inner edges present in the cycle.
  bool HasInnerEdges() const { return _hasInnerEdges; }

  /// \ru Определен ли предыдущий цикл. \en Check if a previous cycle is defined.
  bool IsPrevDefined() const { return _prev < SYS_MAX_T; }
  /// \ru Определен ли следующий цикл. \en Check if a next cycle is defined.
  bool IsNextDefined() const { return _next < SYS_MAX_T; }

  /// \ru Установить предыдущий цикл. \en Set the previous cycle.
  void SetPrev( size_t ed ) { _prev = ed; }
  /// \ru Установить следующий цикл. \en Set the next cycle.
  void SetNext( size_t ed ) { _next = ed; }
  /// \ru Установить первое ребро цикла. \en Set the first edge of the cycle.
  void SetEdge( size_t ed ) { _ed = ed; }
  /// \ru Установить идентификатор подрегиона, к которому принадлежит цикл. \en Set the parent subregion.
  void SetSubRegion( size_t iReg ) { _subRegion = iReg; }
  /// \ru Установить признак наличия в цикле внутренних ребер. \en Set whether a cycle uses inner edges of not.
  void SetInnerEdges( bool bVal ) { _hasInnerEdges = bVal; }

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbSegmentationLoop, MATH_FUNC_EX );
  DECLARE_NEW_DELETE_CLASS( MbSegmentationLoop );
  DECLARE_NEW_DELETE_CLASS_EX( MbSegmentationLoop );
};


//-------------------------------------------------------------------------------
/** \brief \ru Подрегион в сегментации полигональной сетки.
           \en Polygonal mesh segmentation subregion.
*/
// ---
class MATH_CLASS MbSegmentationSubRegion : public MbHalfedgeElement
{
private:
  size_t _prev;          ///< \ru Идентификатор предыдущего подрегиона. \en Previous subregion identifier.
  size_t _next;          ///< \ru Идентификатор следующего подрегиона. \en Next subregion identifier.
  size_t _firstFacet; ///< \ru Идентификатор первого фасета подрегиона. \en First subregion facet identifier.
  size_t _firstLoop;     ///< \ru Идентификатор первого цикла подрегиона. \en First subregion cycle identifier.
  size_t _region;        ///< \ru Идентификатор региона, которому принадлежит подрегион. \en Parent region identifier.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbSegmentationSubRegion()
    : MbHalfedgeElement()
    , _prev( SYS_MAX_T )
    , _next( SYS_MAX_T )
    , _firstFacet( SYS_MAX_T )
    , _firstLoop( SYS_MAX_T )
    , _region( SYS_MAX_T )
  {}
  /// \ru Конструктор копирования. \en Copy constructor.
  MbSegmentationSubRegion( const MbSegmentationSubRegion & that )
    : MbHalfedgeElement( that )
    , _prev( that._prev )
    , _next( that._next )
    , _firstFacet( that._firstFacet )
    , _firstLoop( that._firstLoop )
    , _region( that._region )
  {}
  /// \ru Деструктор. \Destructor.
  virtual ~MbSegmentationSubRegion() {}

  /// \ru Оператор присваивания. \en Assingnment operator.
  MbSegmentationSubRegion & operator = ( const MbSegmentationSubRegion & that )
  {
    MbHalfedgeElement::operator=( that );
    _prev = that._prev;
    _next = that._next;
    _firstFacet = that._firstFacet;
    _firstLoop = that._firstLoop;
    _region = that._region;
    return ( *this );
  }

  /// \ru Обнулить подрегион. \en Clear the subregion.
  void Clear();
  /// \ru Получить предыдущий подрегион. \en Get a previous subregion.
  size_t Prev() const { return _prev; }
  /// \ru Получить следующий подрегион. \en Get a next subregion.
  size_t Next() const { return _next; }
  /// \ru Получить идентификатор первого фасета подрегиона. \en Get a subregion's first facet.
  size_t FirstFacet() const { return _firstFacet; }
  /// \ru Получить идентификатор первого цикла подрегиона. \en Get the first subregion cycle identifier.
  size_t FirstLoop() const { return _firstLoop; }
  /// \ru Получить идентификатор региона. \en Get the parent subregion.
  size_t Region() const { return _region; }

  /// \ru Определен ли предыдущий подрегион. \en Check whether a previous region is defined.
  bool IsPrevDefined() const { return _prev < SYS_MAX_T; }
  /// \ru Определен ли следующий подрегион. \en Check whether a next region is defined.
  bool IsNextDefined() const { return _next < SYS_MAX_T; }

  /// \ru Установить предыдущий подрегион. \en Set the previous subregion.
  void SetPrev( size_t ed ) { _prev = ed; }
  /// \ru Установить следующий подрегион. \en Set the next subregion.
  void SetNext( size_t ed ) { _next = ed; }
  /// \ru Установить идентификатор первого треугольника подрегиона. \en Set the subregion's first facet.
  void SetFirstFacet( size_t iF ) { _firstFacet = iF; }
  /// \ru Установить идентификатор первого цикла подрегиона. \en Set the subregion's first cycle
  void SetFirstLoop( size_t iLoop ) { _firstLoop = iLoop; }
  /// \ru Установить идентификатор региона. \en Set parenting region
  void SetRegion( size_t iRegion ) { _region = iRegion; }

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbSegmentationSubRegion, MATH_FUNC_EX );
  DECLARE_NEW_DELETE_CLASS( MbSegmentationSubRegion );
  DECLARE_NEW_DELETE_CLASS_EX( MbSegmentationSubRegion );
};
#endif // __HALFEDGE_ELEMENT_H
