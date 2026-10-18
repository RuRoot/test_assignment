////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Атрибуты для полигональной сетки в представлении half edge.
         \en Attributes for the polygonal mesh with halfedge representation.

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MESH_HALFEDGE_ATTR_BASE_H
#define __MESH_HALFEDGE_ATTR_BASE_H

#include <math_define.h>
#include <io_tape.h>
#include <type_traits>
#include <array>

class MbHalfedge;
class MbMatrix3D;
class HalfedgeVisitor;

#define HALFEDGE_VISITING()\
        public:\
          void Accept( HalfedgeVisitor & visitor ) override;\
          void Accept( HalfedgeVisitor & visitor ) const override\

//------------------------------------------------------------------------------
/** \brief \ru Тип атрибута.
           \en Attribute's type.
  \details \ru Тип атрибута (вершины, ребра или фасета).
           \en Attribute's type (associated with vertices, edges or facets).
  \ingroup Data_Structures
*/
// ---
enum class MbeTypeAttr
{
  ta_Undefined = 0,             ///< \ru Тип атрибута не определен. \en Attribute's type is not defined.
  ta_BoolValuesFacet,           ///< \ru Тип атрибута MbHalfedgeAttrBoolFacet. \en Attribute's type is MbHalfedgeAttrBoolFacet.
  ta_BoolValuesEdge,            ///< \ru Тип атрибута MbHalfedgeAttrBoolEdge. \en Attribute's type is MbHalfedgeAttrBoolEdge. 
  ta_BoolValuesVertex,          ///< \ru Тип атрибута MbHalfedgeAttrBoolVertex. \en Attribute's type is MbHalfedgeAttrBoolVertex.
  ta_UintValuesFacet,           ///< \ru Тип атрибута MbHalfedgeAttrUintFacet. \en Attribute's type is MbHalfedgeAttrUintFacet. 
  ta_UintValuesEdge,            ///< \ru Тип атрибута MbHalfedgeAttrUintEdge. \en Attribute's type is MbHalfedgeAttrUintEdge. 
  ta_UintValuesVertex,          ///< \ru Тип атрибута MbHalfedgeAttrUintVertex. \en Attribute's type is MbHalfedgeAttrUintVertex. 
  ta_IntValuesFacet,            ///< \ru Тип атрибута MbHalfedgeAttrIntFacet. \en Attribute's type is MbHalfedgeAttrIntFacet.
  ta_IntValuesEdge,             ///< \ru Тип атрибута MbHalfedgeAttrIntEdge. \en Attribute's type is MbHalfedgeAttrIntEdge.
  ta_IntValuesVertex,           ///< \ru Тип атрибута MbHalfedgeAttrIntVertex. \en Attribute's type is MbHalfedgeAttrIntVertex.
  ta_SizetValuesFacet,          ///< \ru Тип атрибута MbHalfedgeAttrSizetFacet. \en Attribute's type is MbHalfedgeAttrSizetFacet. 
  ta_SizetValuesEdge,           ///< \ru Тип атрибута MbHalfedgeAttrSizetEdge. \en Attribute's type is MbHalfedgeAttrSizetEdge. 
  ta_SizetValuesVertex,         ///< \ru Тип атрибута MbHalfedgeAttrSizetVertex. \en Attribute's type is MbHalfedgeAttrSizetVertex. 
  ta_DoubleValuesFacet,         ///< \ru Тип атрибута MbHalfedgeAttrDoubleFacet. \en Attribute's type is MbHalfedgeAttrDoubleFacet. 
  ta_DoubleValuesEdge,          ///< \ru Тип атрибута MbHalfedgeAttrDoubleEdge. \en Attribute's type is MbHalfedgeAttrDoubleEdge. 
  ta_DoubleValuesVertex,        ///< \ru Тип атрибута MbHalfedgeAttrDoubleVertex. \en Attribute's type is MbHalfedgeAttrDoubleVertex.
  ta_FloatValuesFacet,          ///< \ru Тип атрибута MbHalfedgeAttrFloatFacet. \en Attribute's type is MbHalfedgeAttrFloatFacet.
  ta_FloatValuesEdge,           ///< \ru Тип атрибута MbHalfedgeAttrFloatEdge. \en Attribute's type is MbHalfedgeAttrFloatEdge.
  ta_FloatValuesVertex,         ///< \ru Тип атрибута MbHalfedgeAttrFloatVertex. \en Attribute's type is MbHalfedgeAttrFloatVertex.
  ta_Points3D,                  ///< \ru Тип атрибута MbHalfedgeAttrPoint3D. \en Attribute's type is MbHalfedgeAttrPoint3D.
  ta_Parameters2D,              ///< \ru Тип атрибута MbHalfedgeAttrParameters. \en Attribute's type is MbHalfedgeAttrParameters.
  ta_NormalsFacet,              ///< \ru Тип атрибута MbHalfedgeAttrNormalsFacet. \en Attribute's type is MbHalfedgeAttrNormalsFacet. 
  ta_NormalsVertex,             ///< \ru Тип атрибута MbHalfedgeAttrNormalsVertex. \en Attribute's type is MbHalfedgeAttrNormalsVertex.
  ta_DihedralAngles,            ///< \ru Тип атрибута MbHalfedgeAttrDihedralAngles. \en Attribute's type is MbHalfedgeAttrDihedralAngles. 
  ta_Sharp,                     ///< \ru Тип атрибута MbHalfedgeAttrSharpEdge. \en Attribute's type is MbHalfedgeAttrSharpEdge. 
  ta_Curvatures,                ///< \ru Тип атрибута MbHalfedgeAttrCurvaturesVertex. \en Attribute's type is MbHalfedgeAttrCurvaturesVertex. 
  ta_Density,                   ///< \ru Тип атрибута MbHalfedgeAttrDensityVertex. \en Attribute's type is MbHalfedgeAttrDensityVertex. 
  ta_DirectionEdge3D,           ///< \ru Тип атрибута MbHalfedgeAttrDirectionEdge3D. \en Attribute's type is MbHalfedgeAttrDirectionEdge3D. 
  ta_DirectionVertex3D,         ///< \ru Тип атрибута MbHalfedgeAttrDirectionVertex3D. \en Attribute's type is MbHalfedgeAttrDirectionVertex3D. 
  ta_SegmentationSurface,       ///< \ru Тип атрибута MbHalfedgeAttrSurfaces. \en Attribute's type is MbHalfedgeAttrSurfaces. 
  ta_Parameters2DEdgeDependent, ///< \ru Тип атрибута MbHalfedgeAttrParamByEdge. \en Attribute's type is MbHalfedgeAttrParamByEdge. 
  ta_Strains,                   ///< \ru Тип атрибута MbHalfedgeAttrStrains. \en Attribute's type is MbHalfedgeAttrSizetVertex. 
  ta_PairValuesEdge,            ///< \ru Тип атрибута MbHalfedgeAttrIndPairEdge. \en Attribute's type is MbHalfedgeAttrIndPairEdge. 
  ta_UcharValuesFacet,          ///< \ru Тип атрибута MbHalfedgeAttrUcharFacet. \en Attribute's type is MbHalfedgeAttrUcharFacet. 
  ta_UcharValuesEdge,           ///< \ru Тип атрибута MbHalfedgeAttrUcharEdge. \en Attribute's type is MbHalfedgeAttrUcharEdge. 
  ta_UcharValuesVertex,         ///< \ru Тип атрибута MbHalfedgeAttrUcharVertex. \en Attribute's type is MbHalfedgeAttrUcharVertex. 
  ta_ContainerFacet = 1000,     ///< \ru Внутренний тип атрибута MbHalfedgeAttrContainerFacet. \en Attribute's inner type is MbHalfedgeAttrContainerFacet. 
  ta_ContainerEdge,             ///< \ru Внутренний тип атрибута MbHalfedgeAttrContainerEdge. \en Attribute's inner type is MbHalfedgeAttrContainerEdge. 
  ta_ContainerVertex,           ///< \ru Внутренний тип атрибута MbHalfedgeAttrContainerVertex. \en Attribute's inner type is MbHalfedgeAttrContainerVertex. 
  ta_BoolFlagsFacet,            ///< \ru Внутренний тип атрибута MbHalfedgeAttrSelectFacet. \en Attribute's inner type is MbHalfedgeAttrSelectFacet. 
  ta_BoolFlagsEdge,             ///< \ru Внутренний тип атрибута MbHalfedgeAttrSelectEdge. \en Attribute's inner type is MbHalfedgeAttrSelectEdge. 
  ta_BoolFlagsVertex,           ///< \ru Внутренний тип атрибута MbHalfedgeAttrSelectVertex. \en Attribute's inner type is MbHalfedgeAttrSelectVertex. 
  ta_Segmentation,              ///< \ru Внутренний тип атрибута MbHalfedgeAttrSegmentation. \en Attribute's inner type is MbHalfedgeAttrSegmentation. 
  ta_SegmentationEdge,          ///< \ru Внутренний тип атрибута MbHalfedgeAttrSegmentationEdge. \en Attribute's inner type is MbHalfedgeAttrSegmentationEdge. 
  ta_SegmentationRegions,       ///< \ru Внутренний тип атрибута MbHalfedgeAttrRegions. \en Attribute's inner type is MbHalfedgeAttrRegions. 
  ta_Silhouette,                ///< \ru Внутренний тип атрибута MbHalfedgeAttrSilhouette. \en Attribute's inner type is MbHalfedgeAttrSilhouette. 
  ta_IntersectEdge,             ///< \ru Внутренний тип атрибута MbHalfedgeAttrIntersect. \en Attribute's inner type is MbHalfedgeAttrIntersect. 
  ta_LocationFacet,             ///< \ru Внутренний тип атрибута MbHalfedgeAttrLocationFacet. \en Attribute's inner type is MbHalfedgeAttrLocationFacet. 
  ta_LocationVertex,            ///< \ru Внутренний тип атрибута MbHalfedgeAttrLocationVertex. \en Attribute's inner type is MbHalfedgeAttrLocationVertex. 
  ta_GridIds,                   ///< \ru Внутренний тип атрибута MbHalfedgeAttrGridId. \en Attribute's inner type is MbHalfedgeAttrGridId. 
  ta_SegmentValues,             ///< \ru Внутренний тип атрибута MbHalfedgeAttrSegmentValues. \en Attribute's inner type is MbHalfedgeAttrSegmentValues. 
  ta_FacetData,                 ///< \ru Внутренний тип атрибута MbHalfedgeAttrFacetData. \en Attribute's inner type is MbHalfedgeAttrFacetData. 
  ta_PoleSeamUvEdge,            ///< \ru Внутренний тип атрибута MbHalfedgeAttrPoleSeam. \en Attribute's inner type is MbHalfedgeAttrPoleSeam. 
  ta_Rosy,                      ///< \ru Внутренний тип атрибута MbHalfedgeAttrRosy. \en Attribute's inner type is MbHalfedgeAttrRosy. 
  ta_Moto,                      ///< \ru Внутренний тип атрибута MbHalfedgeAttrMoto. \en Attribute's inner type is MbHalfedgeAttrMoto. 
  ta_ComplFacet,                ///< \ru Внутренний тип атрибута MbHalfedgeAttrComplFacet. \en Attribute's inner type is MbHalfedgeAttrComplFacet. 
  ta_Fiber,                     ///< \ru Внутренний тип атрибута FiberNodeState. \en Attribute's inner type is FiberNodeState. 
  ta_SegmentationShellMap,      ///< \ru Внутренний тип атрибута MbHalfedgeAttrShellMap. \en Attribute's inner type is MbHalfedgeAttrShellMap. 
};


//------------------------------------------------------------------------------
/** \brief \ru Базовый интерфейс атрибутов для полигональной сетки для структуры half edge.
           \en Base attribute interface for the polygonal mesh with half edge representation.
  \details \ru Базовый интерфейс атрибутов для полигональной сетки для структуры half edge.
           \en Base attribute interface for the polygonal mesh with half edge representation.
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbHalfedgeAttrBase : public TapeBase
{
protected:
  const MbHalfedge * _topo;  ///< \ru Указатель на полигональную топологию в виде half edge. \en Pointer to the polygonal topology with half edge representation.
  MbeTypeAttr        _type;  ///< \ru Тип атрибута (для преобразования из базового типа). \en Attribute's type (used for attributes casting).
  size_t             _id;    ///< \ru Индекс в массиве атрибутов. \en Index in the attributes array as identifier.

public:
  /// \ru Конструктор. \en Constructor.
  MbHalfedgeAttrBase( const MbHalfedge & topo, MbeTypeAttr type ) : _topo( &topo ), _type( type ), _id( SYS_MAX_T ) {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbHalfedgeAttrBase() {}

  /// \ru Получить тип объекта. \en Get attribute's type.
  MbeTypeAttr TypeAttribute() const { return _type; }
  /// \ru Установить топологию. \en Set topology.
  void SetTopo( const MbHalfedge & topo ) { _topo = &topo; }
  /// \ru Установить тип объекта. \en Set attribute's type.
  void SetType( MbeTypeAttr type ) { _type = type; }
  /// \ru Установить индекс. \en Set identifier.
  void SetId( size_t id ) { _id = id; }
  /// \ru Получить индекс. \en Get identifier.
  size_t GetId() const { return _id; }
  /// \ru Получить топологию. \en GetTopology.
  const MbHalfedge & GetTopo() const { return *_topo; }

  /// \ru Принять посетителя. \en Accept Visitor
  virtual void Accept( HalfedgeVisitor & visitor ) = 0;
  /// \ru Принять посетителя. \en Accept Visitor
  virtual void Accept( HalfedgeVisitor & visitor ) const = 0;

  /// \ru Зарезервировать память под массив вершин, ребер и фасетов. \en Reserve memory to store attribute's data.
  virtual void               Reserve( size_t /*nVx*/, size_t /*nEd*/, size_t /*nF*/ ) {}
  /// \ru Добавить вершину с дефолтным значением атрибута. \en Add vertex with the default attribute.
  virtual void               AddVertex() {}
  /// \ru Добавить ребро с дефолтным значением атрибута. \en Add edge with the default attribute.
  virtual void               AddEdge() {}
  /// \ru Добавить фасет с дефолтным значением атрибута. \en Add facet with the default attribute.
  virtual void               AddFacet() {}
  /// \ru Преобразовать согласно матрице. \en Transform attribute with given transformation matrix.
  virtual void               Transform( const MbMatrix3D & /*matr*/ ) {}

  DECLARE_PERSISTENT_CLASS( MbHalfedgeAttrBase );
};


/** \brief \ru Преобразовать указатель на объект базового атрибута к указателю на объект атрибута заданного типа.
           \en Cast base attribute pointer to the selected attribute type.
  \param[in] pObj - \ru Указатель на объект базового типа. \en Pointer to the base object.
  \return - \ru Указатель на тип T, если тип объекта совпадает с заданным, иначе нулевой указатель.
            \en Pointer on the T object if casting possible and nullptr otherwise.
*/
template<class T>
T * CastHalfedgeAttribute( MbHalfedgeAttrBase * pObj )
{
  if ( T::IsA() == pObj->TypeAttribute() )
    return static_cast<T *>( pObj );
  return nullptr;
}


/** \brief \ru Преобразовать указатель на объект базового атрибута к указателю на объект атрибута заданного типа.
           \en Cast base attribute pointer to the selected attribute type.
  \param[in] pObj - \ru Указатель на объект базового типа. \en Pointer to the base object.
  \return - \ru Указатель на тип T, если тип объекта совпадает с заданным, иначе нулевой указатель.
            \en Pointer on the T object if casting possible and nullptr otherwise.
*/
template<class T>
const T * CastHalfedgeAttribute( const MbHalfedgeAttrBase * pObj )
{
  if ( T::IsA() == pObj->TypeAttribute() )
    return static_cast<const T *>( pObj );
  return nullptr;
}


//------------------------------------------------------------------------------
/** \brief \ru Базовый интерфейс атрибута простого типа для ассоциации
               значения с элементом half edge.
           \en Interface of the trivial type attribute to associate with half edge elements.
  \ingroup Data_Structures
*/
// ---
template<typename Value>
class MbHalfedgeAttrSimpleValueBase : public MbHalfedgeAttrBase
{
protected:
  std::vector<Value> _values; ///< \ru Массив величин, ассоциированных с топологическим элементом полигональной сетки. \en Array of the values associated with polygonal mesh topology element.

public:
  /// \ru Конструктор. \en Constructor.
  MbHalfedgeAttrSimpleValueBase( const MbHalfedge & topo, MbeTypeAttr type ) : MbHalfedgeAttrBase( topo, type ) {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbHalfedgeAttrSimpleValueBase() {}

  /// \ru Получить значение по умолчанию. \en Get default value.
  virtual const Value & DefaultValue() const = 0;
  /// \ru Установить размер массива величин, заполнить его значением по умолчанию. \en Set size of the values array filled with default values.
  void SetSize( size_t size ) { _values.assign( size, DefaultValue() ); }
  /// \ru Получить размер массива величин. \en Get size of the values array.
  size_t GetSize() const { return _values.size(); }
  /// \ru Определено ли значение элемента. \en Check if value is undefined (default).
  virtual bool IsUndefined( size_t i ) const { return _values[i] == DefaultValue(); }
  /// \ru Получить значение элемента. \en Get value by identifier.
  template<typename T = Value, typename = typename std::enable_if< std::is_fundamental<T>::value >::type>
  Value GetValue( size_t i ) const { return  _values[i]; }
  /// \ru Получить значение элемента. \en Get value by identifier.
  template<typename T = Value, typename = typename std::enable_if< !std::is_fundamental<T>::value >::type>
  const Value & GetValue( size_t i ) const { return  _values[i]; }
  /// \ru Установить значение элемента в указанную величину. \en Set value to the element given by identifier.
  void SetValue( size_t i, Value val ) { _values[i] = val; }
  /// \ru Установить значение набора элементов в указанную величину. \en Set value to the set of elements.
  void SetValue( const c3d::IndicesVector & aElem, Value val )
  {
    for ( auto i : aElem )
      _values[i] = val;
  }
  /// \ru Установить все значения набора элементов в указанную величину. \en Set all elements to given value.
  void Assign( Value val )
  {
    for ( size_t i = 0; i < _values.size(); ++i )
      _values[i] = val;
  } 
  /// \ru Получить все значения. \en Get all values.
  std::vector<Value> & GetValues() { return _values; }
  /// \ru Получить все значения. \en Get all values.
  const std::vector<Value> & GetValues() const { return _values; }

  DECLARE_PERSISTENT_CLASS( MbHalfedgeAttrSimpleValueBase );
};


/// \ru Конструктор поддержки сериализации \en Constructor for serialization support
template<class Value>
MbHalfedgeAttrSimpleValueBase<Value>::MbHalfedgeAttrSimpleValueBase( TapeInit tapeInit ) : MbHalfedgeAttrBase( tapeInit ) {}


/// \ru Чтение из потока. \en Read from input stream.
template<class Value>
void MbHalfedgeAttrSimpleValueBase<Value>::Read( reader & in, MbHalfedgeAttrSimpleValueBase<Value> * obj ) { ReadBase( in, static_cast<MbHalfedgeAttrBase *>(obj) ); }


/// \ru Запись в поток. \en Write into output stream.
template<class Value>
void MbHalfedgeAttrSimpleValueBase<Value>::Write( writer & out, const MbHalfedgeAttrSimpleValueBase<Value> * obj ) { WriteBase( out, static_cast<const MbHalfedgeAttrBase *>(obj) ); }


/// \ru Поддержка сериализации. \en Serialization support.
template<class Value>
ClassDescriptor MbHalfedgeAttrSimpleValueBase<Value>::GetClassDescriptor( const VersionContainer & v ) const { return ClassDescriptor( TapeBase::GetPureName( v ), Math::MathID() ); }


//------------------------------------------------------------------------------
/** \brief \ru Базовый атрибут простого типа для вершин.
           \en Simple type base attribute for vertices.
  \ingroup Data_Structures
*/
// ---
template<class Value>
class MbHalfedgeAttrSimpleValueVertexBase : public MbHalfedgeAttrSimpleValueBase<Value>
{
public:
  /// \ru Конструктор. \en Constructor.
  MbHalfedgeAttrSimpleValueVertexBase( const MbHalfedge & topo, MbeTypeAttr type ) : MbHalfedgeAttrSimpleValueBase<Value>( topo, type ) {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbHalfedgeAttrSimpleValueVertexBase() {}

  /// \ru Зарезервировать память под массив вершин, ребер и фасетов. \en Reserve memory to store attribute's data.
  void Reserve( size_t nVx, size_t /*nEd*/, size_t /*nF*/ ) override { MbHalfedgeAttrSimpleValueBase<Value>::_values.reserve( nVx ); }
  /// \ru Добавить вершину с дефолтным значением атрибута. \en Add Vertex with the default attribute.
  void AddVertex() override { MbHalfedgeAttrSimpleValueBase<Value>::_values.push_back( this->DefaultValue() ); }

  DECLARE_PERSISTENT_CLASS( MbHalfedgeAttrSimpleValueVertexBase );
};


/// \ru Конструктор поддержки сериализации \en Constructor for serialization support
template<class Value>
MbHalfedgeAttrSimpleValueVertexBase<Value>::MbHalfedgeAttrSimpleValueVertexBase( TapeInit tapeInit ) : MbHalfedgeAttrSimpleValueBase<Value>( tapeInit ) {}


/// \ru Чтение из потока. \en Read from input stream.
template<class Value>
void MbHalfedgeAttrSimpleValueVertexBase<Value>::Read( reader & in, MbHalfedgeAttrSimpleValueVertexBase<Value> * obj ) { ReadBase( in, ( MbHalfedgeAttrSimpleValueBase<Value> * )obj ); }


/// \ru Запись в поток. \en Write into output stream.
template<class Value>
void MbHalfedgeAttrSimpleValueVertexBase<Value>::Write( writer & out, const MbHalfedgeAttrSimpleValueVertexBase<Value> * obj ) { WriteBase( out, ( const MbHalfedgeAttrSimpleValueBase<Value> * )obj ); }


/// \ru Поддержка сериализации. \en Serialization support.
template<class Value>
ClassDescriptor MbHalfedgeAttrSimpleValueVertexBase<Value>::GetClassDescriptor( const VersionContainer & v ) const { return ClassDescriptor( TapeBase::GetPureName( v ), Math::MathID() ); }


//------------------------------------------------------------------------------
/** \brief \ru Базовый атрибут простого типа для ребер.
           \en Simple type base attribute for edges.
  \ingroup Data_Structures
*/
// ---
template<class Value>
class MbHalfedgeAttrSimpleValueEdgeBase : public MbHalfedgeAttrSimpleValueBase<Value>
{
public:
  /// \ru Конструктор. \en Constructor.
  MbHalfedgeAttrSimpleValueEdgeBase( const MbHalfedge & topo, MbeTypeAttr type ) : MbHalfedgeAttrSimpleValueBase<Value>( topo, type ) {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbHalfedgeAttrSimpleValueEdgeBase() {}

  /// \ru Зарезервировать память под массив вершин, ребер и фасетов. \en Reserve memory to store attribute's data.
  void Reserve( size_t /*nVx*/, size_t nEd, size_t /*nF*/ ) override { MbHalfedgeAttrSimpleValueBase<Value>::_values.reserve( nEd ); }
  /// \ru Добавить ребро с дефолтным значением атрибута. \en Add edge with the default attribute.
  void AddEdge() override { MbHalfedgeAttrSimpleValueBase<Value>::_values.push_back( this->DefaultValue() ); }

  DECLARE_PERSISTENT_CLASS( MbHalfedgeAttrSimpleValueEdgeBase );
};


/// \ru Конструктор поддержки сериализации \en Constructor for serialization support
template<class Value>
MbHalfedgeAttrSimpleValueEdgeBase<Value>::MbHalfedgeAttrSimpleValueEdgeBase( TapeInit tapeInit ) : MbHalfedgeAttrSimpleValueBase<Value>( tapeInit ) {}


/// \ru Чтение из потока. \en Read from input stream.
template<class Value>
void MbHalfedgeAttrSimpleValueEdgeBase<Value>::Read( reader & in, MbHalfedgeAttrSimpleValueEdgeBase<Value> * obj ) { ReadBase( in, ( MbHalfedgeAttrSimpleValueBase<Value> * )obj ); }


/// \ru Запись в поток. \en Write into output stream.
template<class Value>
void MbHalfedgeAttrSimpleValueEdgeBase<Value>::Write( writer & out, const MbHalfedgeAttrSimpleValueEdgeBase<Value> * obj ) { WriteBase( out, ( const MbHalfedgeAttrSimpleValueBase<Value> * )obj ); }


/// \ru Поддержка сериализации. \en Serialization support.
template<class Value>
ClassDescriptor MbHalfedgeAttrSimpleValueEdgeBase<Value>::GetClassDescriptor( const VersionContainer & v ) const { return ClassDescriptor( TapeBase::GetPureName( v ), Math::MathID() ); }


//------------------------------------------------------------------------------
/** \brief \ru Базовый атрибут простого типа для фасетов.
           \en Simple type base attribute for facets.
  \ingroup Data_Structures
*/
// ---
template<class Value>
class MbHalfedgeAttrSimpleValueFacetBase : public MbHalfedgeAttrSimpleValueBase<Value>
{
public:
  /// \ru Конструктор. \en Constructor.
  MbHalfedgeAttrSimpleValueFacetBase( const MbHalfedge & topo, MbeTypeAttr type ) : MbHalfedgeAttrSimpleValueBase<Value>( topo, type ) {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbHalfedgeAttrSimpleValueFacetBase() {}

  /// \ru Зарезервировать память под массив вершин, ребер и фасетов. \en Reserve memory to store attribute's data.
  void Reserve( size_t /*nVx*/, size_t /*nEd*/, size_t nF ) override { MbHalfedgeAttrSimpleValueBase<Value>::_values.reserve( nF ); }
  /// \ru Добавить фасет с дефолтным значением атрибута. \en Add facet with the default attribute.
  void AddFacet() override { MbHalfedgeAttrSimpleValueBase<Value>::_values.push_back( this->DefaultValue() ); }

  DECLARE_PERSISTENT_CLASS( MbHalfedgeAttrSimpleValueFacetBase );
};


/// \ru Конструктор поддержки сериализации \en Constructor for serialization support
template<class Value>
MbHalfedgeAttrSimpleValueFacetBase<Value>::MbHalfedgeAttrSimpleValueFacetBase( TapeInit tapeInit ) : MbHalfedgeAttrSimpleValueBase<Value>( tapeInit ) {}


/// \ru Чтение из потока. \en Read from input stream.
template<class Value>
void MbHalfedgeAttrSimpleValueFacetBase<Value>::Read( reader & in, MbHalfedgeAttrSimpleValueFacetBase<Value> * obj ) { ReadBase( in, ( MbHalfedgeAttrSimpleValueBase<Value> * )obj ); }


/// \ru Поддержка сериализации. \en Serialization support.
template<class Value>
void MbHalfedgeAttrSimpleValueFacetBase<Value>::Write( writer & out, const MbHalfedgeAttrSimpleValueFacetBase<Value> * obj ) { WriteBase( out, ( const MbHalfedgeAttrSimpleValueBase<Value> * )obj ); }


/// \ru Поддержка сериализации. \en Serialization support.
template<class Value>
ClassDescriptor MbHalfedgeAttrSimpleValueFacetBase<Value>::GetClassDescriptor( const VersionContainer & v ) const { return ClassDescriptor( TapeBase::GetPureName( v ), Math::MathID() ); }


//------------------------------------------------------------------------------
/** \brief \ru Базовый атрибут для ассоциированных с вершинами величин, зависящих от выбранного вокруг вершины ребра.
           \en Base attribute for edge-dependent data for vertices
  \details \ru Базовый атрибут для ассоциированных с вершинами величин, значение
               которых зависит от того, с какой стороны от вершины мы находимся.
               Например, нормаль в вершине, которая находится на остром сломе, разная
               с двух сторон слома. Та же ситуация с кривизнами и другими объектами.
           \en Base attribute for values related to the position around vertex.
               As example, a normal on a sharp fold can be different on different slopes.
               The same can be applied to curvatures and some other objects.
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbHalfedgeAttrEdgeDependentVertexBase : public MbHalfedgeAttrBase
{
protected:
  /// \ru Индексы для величин, ассоциированных с ребром. Должны храниться в отдельном массиве у наследников.
  /// \en Values' indices associated with edge. Data must be stored in separate array in a derived class.
  c3d::IndicesVector _ids;
  c3d::IndicesVector _idsFree; /// \ru Список свободных индексов. \en List of free indices.

public:
  /// \ru Конструктор. \en Constructor.
  MbHalfedgeAttrEdgeDependentVertexBase( const MbHalfedge & topo, MbeTypeAttr type );
  /// \ru Деструктор. \en Destructor.
  virtual ~MbHalfedgeAttrEdgeDependentVertexBase() {}

  /// \ru Добавить новое значение. \en Add new value.
  size_t AddValue();
  /// \ru Получить количество значений. \en Get number of associated values.
  virtual size_t ValuesCount() const = 0;
  /// \ru Добавить свободный индекс. \en Add the free index.
  void AddFreeIndex( size_t id ) { _idsFree.push_back( id ); }

  /// \ru Добавить ребро с дефолтным значением атрибута. \en Add edge with the default attribute value.
  void AddEdge() override { _ids.push_back( SYS_MAX_T ); }
  /// \ru Установить размер массива для ребер. \en Set size of the associated edges array.
  void SetSizeEdge( size_t nEd ) { _ids.assign( nEd, SYS_MAX_T ); }
  /// \ru Получить размер массива для ребер. \en Get size of associated edge array.
  size_t GetSizeEdge() const { return _ids.size(); }
  /// \ru Установить значение индекса для ребра. \en Set data index for the given edge.
  void SetEdgeId( size_t iEd, size_t iElem ) { _ids[iEd] = iElem; }
  /// \ru Получить значение индекса для ребра. \en Get data index for the given edge.
  size_t GetEdgeId( size_t iEd ) const { return _ids[iEd]; }
  /// \ru Получить все индексы. \en Get all edge indices.
  c3d::IndicesVector & GetIds() { return _ids; }
  /// \ru Получить все индексы. \en Get all edge indices.
  const c3d::IndicesVector & GetIds() const { return _ids; }
  /// \ru Получить свободные индексы. \en Get all free edge indices.
  c3d::IndicesVector & GetIdsFree() { return _idsFree; }
  /// \ru Получить свободные индексы. \en Get all free edge indices.
  const c3d::IndicesVector & GetIdsFree() const { return _idsFree; }

private:
  /// \ru Добавить новое значение. \en Add new value.
  virtual size_t AddValueInternal() = 0;

  DECLARE_PERSISTENT_CLASS( MbHalfedgeAttrEdgeDependentVertexBase );
};

#endif // __MESH_HALFEDGE_ATTR_BASE_H