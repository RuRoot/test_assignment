////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief Атрибуты для полигональной сетки в представлении half edge.

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MESH_HALFEDGE_ATTR_H
#define __MESH_HALFEDGE_ATTR_H

#include <mesh_halfedge_attr_base.h>
#include <mb_cart_point3d.h>
#include <mb_cart_point.h>
#include <m2b_mesh_curvature.h>


class  MbCube;
class  MbRect;
class MbHalfedge;

#define SIMPLE_FACET_ATTRIBUTE(type, value, nameAttr, typeAttr) \
          class MATH_CLASS nameAttr : public MbHalfedgeAttrSimpleValueFacetBase<type>\
          {\
            private:\
            const type _def = value;\
            public:\
            nameAttr( const MbHalfedge & topo ) : MbHalfedgeAttrSimpleValueFacetBase<type>( topo, typeAttr ) {}\
            virtual ~nameAttr() {}\
            static MbeTypeAttr IsA() { return typeAttr; }\
            const type& DefaultValue() const override { return _def; }\
            HALFEDGE_VISITING();\
            DECLARE_PERSISTENT_CLASS( nameAttr );\
          };


#define SIMPLE_EDGE_ATTRIBUTE(type, value, nameAttr, typeAttr) \
          class MATH_CLASS nameAttr : public MbHalfedgeAttrSimpleValueEdgeBase<type>\
          {\
            private:\
            const type _def = value;\
            public:\
            nameAttr( const MbHalfedge & topo ) : MbHalfedgeAttrSimpleValueEdgeBase<type>( topo, typeAttr ) {}\
            virtual ~nameAttr() {}\
            static MbeTypeAttr IsA() { return typeAttr; }\
            const type& DefaultValue() const override { return _def; }\
            HALFEDGE_VISITING();\
            DECLARE_PERSISTENT_CLASS( nameAttr );\
          };


#define SIMPLE_VERTEX_ATTRIBUTE(type, value, nameAttr, typeAttr) \
          class MATH_CLASS nameAttr : public MbHalfedgeAttrSimpleValueVertexBase<type>\
          {\
            private:\
            const type _def = value;\
            public:\
            nameAttr( const MbHalfedge & topo ) : MbHalfedgeAttrSimpleValueVertexBase<type>( topo, typeAttr ) {}\
            virtual ~nameAttr() {}\
            static MbeTypeAttr IsA() { return typeAttr; }\
            const type& DefaultValue() const override { return _def; }\
            HALFEDGE_VISITING();\
            DECLARE_PERSISTENT_CLASS( nameAttr );\
          };


//------------------------------------------------------------------------------
/** \brief \ru Атрибут 3Д вершин полигональной сетки.
           \en Attribute for 3D vertices of polygonal mesh.
  \ingroup DaMbeTypeAttr::ta_Structures
*/
// ---
class MATH_CLASS MbHalfedgeAttrPoint3D : public MbHalfedgeAttrBase
{
private:
  c3d::SpacePointsVector _points; ///< \ru Массив 3Д точек для вершин полигональной сетки. \en An array of 3D points for the polygonal mesh's vertices.

public:
  /// \ru Конструктор. \en Constructor
  MbHalfedgeAttrPoint3D( const MbHalfedge & topo ) : MbHalfedgeAttrBase( topo, MbeTypeAttr::ta_Points3D ) {}
  /// \ru Деструктор. \en Destructor
  virtual ~MbHalfedgeAttrPoint3D() {}

  /// \ru Тип атрибута. \en Attribute type.
  static MbeTypeAttr         IsA() { return MbeTypeAttr::ta_Points3D; }
  /// \ru Зарезервировать память под массив вершин, ребер и фасетов. \en Reserve memory to store attribute's data.
  void Reserve( size_t nVx, size_t /*nEd*/, size_t /*nF*/ ) override { _points.reserve( nVx ); }
  /// \ru Добавить вершину с дефолтным значением атрибута. \en Add Vertex with the  default attribute.
  void AddVertex() override { _points.push_back( MbCartPoint3D() ); }
  /// \ru Задать размер массива для вершин. \en Set size of the vertex array.
  void SetSize( size_t size ) { _points.resize( size ); }
  /// \ru Получить 3Д точку для вершины с указанным индексом. \en Get 3D point for the given vertex identifier.
  const MbCartPoint3D & GetPoint( size_t iPt ) const { return _points[iPt]; }
  /// \ru Получить 3Д вершины полигональной сетки. \en Get all 3D points for the polygonal mesh.
  const c3d::SpacePointsVector & GetPoints() const { return _points; }
  /// \ru Получить 3Д вершины полигональной сетки. \en Get all 3D points for the polygonal mesh.
  c3d::SpacePointsVector & GetPoints() { return _points; }
  /// \ru Установить 3Д точку для вершины с указанным индексом. \en Set 3D point for the vertex with given identifier.
  void SetPoint( size_t iPt, const MbCartPoint3D & pt) { _points[iPt] = pt; }
  /// \ru Получить площадь треугольного фасета. \en Find the area of the triangle facet.
  bool GetTriangleArea( size_t iTr, double & area ) const;
  /// \ru Оценить площадь фасета. \en Estimate the area of the facet.
  bool EstimateFacetArea( size_t iF, double & area ) const;
  /// \ru Получить 3Д вершины треугольного фасета. \ru Get 3D points for the triangle facet vertices.
  bool GetTrianglePoints( size_t iTr, std::array<MbCartPoint3D, 3> & points ) const;
  /// \ru Получить центр треугольного фасета. \en Get center of the triangle facet.
  bool GetTriangleCenter( size_t iTr, MbCartPoint3D & center) const;
  /// \ru Получить 3Д вершины начала и конца ребра. \en Get 3D points for the edge's ends.
  bool GetPointsEdge( size_t iEd, MbCartPoint3D & p0, MbCartPoint3D & p1 ) const;
  /// \ru Рассчитать точку на ребре с заданным параметром [0,1]. \en Calculate a point on the edge with the given position [0, 1]
  bool CalculateEdgePoint( size_t iEd, double prm, MbCartPoint3D & pt ) const;
  /// \ru Получить 3Д вершины для последовательности ребер. \en Get 3D points for all vertices in the edges chain.
  bool GetEdgePathVertices( const c3d::IndicesVector & edges, c3d::SpacePointsVector & points ) const;
  /// \ru Получить 3Д вершины для последовательности ребер. \en Get 3D points for all vertices in the edges chain.
  bool GetEdgePathVertices( const c3d::IndicesPairsVector & edges, c3d::SpacePointsVector & points ) const;
  /// \ru Посчитать нормаль треугольного фасета. \en Calculate the normal for the triangle facet.
  bool CalculateTriangleNormal( size_t iTr, MbVector3D & normal ) const;
  /// \ru Посчитать aspect ratio треугольного фасета. \en Calculate aspect ratio for the triangle facet.
  bool CalculateTriangleAspectRatio( size_t iTr, double & ratio ) const;
  /// \ru Оценить нормаль произвольного фасета. \en Estimate the normal of the arbitrary facet.
  bool CalculateFacetNormal( size_t iEd, MbVector3D & normal, bool & bDegenerated ) const;
  /// \ru Посчитать нормаль и площадь треугольного фасета. \en Calculate the normal and area of the triangle facet.
  bool CalculateTriangleNormalAndArea( size_t iTr, bool & bDegenerated, MbVector3D & normal, double & area ) const;
  /// \ru Посчитать двуграный угол между фасетами справа и слева от ребра. \en Calculate the dihedral angle between the facets on both sides of the edge.
  bool CalculateDihedralAngle( size_t iEd, double & ang) const;
  /// \ru Посчитать направление ребра. \en Calculate the edge's direction.
  bool CalculateEdgeDirection( size_t iEd, MbVector3D & dir ) const;
  /// \ru Посчитать угол между указанным ребром и его следующим либо предыдущим ребром. \en Calculate the angle between given edge and it's next or prev edge in the loop.
  bool CalculateEdgeAngle( size_t iEd, bool bEnd, double & ang ) const;
  /// \ru Посчитать длину ребра. \en Calculate the edge's length.
  bool CalculateLengthEdge( size_t iEd, double & len ) const;
  /// \ru Посчитать квадрат длины ребра. \en Calculate the square of the edge's length.
  bool CalculateLengthEdge2( size_t iEd, double & len2 ) const;
  // \ru Рассчитать габарит в локальной системе координат. \en Calculate the bounding box int the given coordinate system. 
  void CalculateLocalGabarit( const MbMatrix3D & matrInto, MbCube & localCube ) const;
  // \ru Рассчитать габарит фасета в локальной системе координат. \en Calculate the facet's bounding box int the given coordinate system. 
  void CalculateFacetLocalGabarit( const MbMatrix3D & matrInto, size_t iF, MbCube & localCube ) const;
  // \ru Рассчитать уравнение плоскости треугольного фасета. \en Calculate the plane where the given triangle facet is located.
  bool GetTrianglePlaneEquation( size_t iTr, bool bNormalize, std::array<double, 4> & plane ) const;
  // \ru Рассчитать нормаль в вершине усреднением нормалей соседних треугольных фасетов. \en Calculate the normal at a vertex as the average normal of all triangle facets around it.
  bool CalculateVertexNormal( size_t iVx, MbVector3D & normal ) const;
  // \ru Оценить нормаль в вершине усреднением нормалей соседних фасетов. \en Estimate the normal at a vertex as the average normal of all facets around it.
  bool EstimateVertexNormal( size_t iVx, MbVector3D & normal ) const;
  /// \ru Преобразовать согласно матрице. \en Transform 3D point by given matrix.
  void Transform( const MbMatrix3D & matr ) override;

  HALFEDGE_VISITING();
  DECLARE_PERSISTENT_CLASS( MbHalfedgeAttrPoint3D );
};


//------------------------------------------------------------------------------
/** \brief \ru Атрибут 2Д параметров для вершин полигональной сетки.
           \en Attribute for vertexes 2D parameters of polygonal mesh.
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbHalfedgeAttrParameters : public MbHalfedgeAttrBase
{
private:
  c3d::ParamPointsVector _params; //< \ru Массив 2Д точек (параметров) для вершин полигональной сетки. \en An array of 2D points (parameters) for the polygonal mesh's vertices.

public:
  /// \ru Конструктор. \en Constructor.
  MbHalfedgeAttrParameters( const MbHalfedge & topo ) : MbHalfedgeAttrBase( topo, MbeTypeAttr::ta_Parameters2D ) {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbHalfedgeAttrParameters() {}

  /// \ru Тип атрибута. \en Attribute type.
  static MbeTypeAttr         IsA() { return MbeTypeAttr::ta_Parameters2D; }
  /// \ru Зарезервировать память под массив вершин, ребер и фасетов. \en Reserve memory to store attribute's data.
  void Reserve( size_t nVx, size_t /*nEd*/, size_t /*nF*/ ) override { _params.reserve( nVx ); }
  /// \ru Добавить вершину с дефолтным значением атрибута. \en Add Vertex with the default attribute.
  void AddVertex() override { _params.push_back( MbCartPoint() ); }
  /// \ru Задать размер массива для вершин. \en Set size of the parameters array.
  void SetSize( size_t size ) { _params.resize( size ); }
  /// \ru Получить 2Д параметр для вершины с указанным индексом. \en Get 2D parameter for the given vertex identifier.
  const MbCartPoint & GetParameter( size_t iPt ) const { return _params[iPt]; } 
  /// \ru Получить 2Д параметры полигональной сетки. \en Get all 2D parameters for the polygonal mesh.
  const c3d::ParamPointsVector & GetParameters() const { return _params; }
  /// \ru Получить 2Д параметры полигональной сетки. \en Get all 2D parameters for the polygonal mesh.
  c3d::ParamPointsVector & GetParameters() { return _params; }
  /// \ru Получить 2Д параметры треугольного фасета. \en Get 2D parameters for the triangular facet.
  bool GetTrianglePoints( size_t iTr, std::array<MbCartPoint, 3> & points ) const;
  /// \ru Получить 2Д параметры треугольного фасета в виде 3Д точек с нулевой z координатой. \en Get 2D parameters for the triangular facet as 3D points with the zero z coord.
  bool GetTrianglePoints( size_t iTr, std::array<MbCartPoint3D, 3> & points, const MbPlacement * placeUv = nullptr ) const;
  /// \ru Получить 2Д параметр в центре треугольного фасета. \en Get the 2D parameter in the center of the triangular facet.
  bool GetTriangleCenter( size_t iTr, MbCartPoint & center ) const;
  /// \ru Получить площадь треугольного фасета. \en Get the area of the triangle facet.
  bool GetTriangleArea( size_t iTr, double & area ) const;
  /// \ru Установить 2Д параметр для вершины с указанным индексом. \en Set the 2D parameter for the given 
  void SetParameter( size_t iPt, const MbCartPoint & pt ) { _params[iPt] = pt; }
  /// \ru Получить 2Д параметры начала и конца ребра. \en Get 2D parameters for the edge's ends.
  bool GetPointsEdge( size_t iEd, MbCartPoint & p0, MbCartPoint & p1 ) const;
  /// \ru Рассчитать точку на ребре с заданным параметром [0,1]. \en Calculate a point on the edge with the given position [0, 1]
  bool CalculateEdgePoint( size_t iEd, double prm, MbCartPoint & pt ) const;
  /// \ru Посчитать направление ребра. \en Calculate the edge's direction.
  bool CalculateEdgeDirection( size_t iEd, MbVector & dir ) const;
  /// \ru Посчитать угол между указанным ребром и его следующим либо предыдущим ребром. \en Calculate the angle between given edge and it's next or prev edge in the loop.
  bool CalculateEdgeAngle( size_t iEd, bool bEnd, double & ang ) const;
  /// \ru Посчитать длину ребра. \en Calculate the square of the edge's length.
  bool CalculateLengthEdge( size_t iEd, double & len ) const;
  /// \ru Посчитать квадрат длины ребра. \en Calculate the square of the edge's length.
  bool CalculateLengthEdge2( size_t iEd, double & len2 ) const;
  /// \ru Получить параметры для последовательности ребер. \en Get 2D parameters for all vertices in the edges chain.
  bool GetEdgePathVertices( const c3d::IndicesVector & edges, c3d::ParamPointsVector & points ) const;
  /// \ru Получить параметры для последовательности ребер. \en Get 2D parameters for all vertices in the edges chain.
  bool GetEdgePathVertices( const c3d::IndicesPairsVector & edges, c3d::ParamPointsVector & points ) const;
  /// \ru Рассчитать габаритный прямоугольник. \en Calculate the bounding rect.
  void CalculateRect( MbRect & rect ) const;

  HALFEDGE_VISITING();
  DECLARE_PERSISTENT_CLASS( MbHalfedgeAttrParameters );
};


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для идентификаторов гридов.
           \en The attribute for the grid's identification.
  \ingroup Data_Structures
*/
// ---
SIMPLE_FACET_ATTRIBUTE( uint, SYS_MAX_UINT, MbHalfedgeAttrGridId, MbeTypeAttr::ta_GridIds );


//------------------------------------------------------------------------------
/** \brief \ru Вещественный атрибут для ребер.
*          \en The double attribute associated with edges.
  \ingroup Data_Structures
*/
// ---
SIMPLE_EDGE_ATTRIBUTE( double, 0., MbHalfedgeAttrDoubleEdge, MbeTypeAttr::ta_DoubleValuesEdge );


//------------------------------------------------------------------------------
/** \brief \ru Вещественный атрибут для фасетов.
           \en The double attribute associated with facets.
  \ingroup Data_Structures
*/
// ---
SIMPLE_FACET_ATTRIBUTE( double, 0., MbHalfedgeAttrDoubleFacet, MbeTypeAttr::ta_DoubleValuesFacet );


//------------------------------------------------------------------------------
/** \brief \ru Вещественный атрибут для вершин.
           \en The double attribute associated with vertices.
  \ingroup Data_Structures
*/
// ---
SIMPLE_VERTEX_ATTRIBUTE( double, 0., MbHalfedgeAttrDoubleVertex, MbeTypeAttr::ta_DoubleValuesVertex );


//------------------------------------------------------------------------------
/** \brief \ru Вещественный атрибут для ребер.
*          \en The float attribute associated with edges.
  \ingroup Data_Structures
*/
// ---
SIMPLE_EDGE_ATTRIBUTE( float, 0.f, MbHalfedgeAttrFloatEdge, MbeTypeAttr::ta_FloatValuesEdge );


//------------------------------------------------------------------------------
/** \brief \ru Вещественный атрибут для фасетов.
           \en The float attribute associated with facets.
  \ingroup Data_Structures
*/
// ---
SIMPLE_FACET_ATTRIBUTE( float, 0.f, MbHalfedgeAttrFloatFacet, MbeTypeAttr::ta_FloatValuesFacet );


//------------------------------------------------------------------------------
/** \brief \ru Вещественный атрибут для вершин.
           \en The float attribute associated with vertices.
  \ingroup Data_Structures
*/
// ---
SIMPLE_VERTEX_ATTRIBUTE( float, 0.f, MbHalfedgeAttrFloatVertex, MbeTypeAttr::ta_FloatValuesVertex );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для плотности в вершине.
           \en The density attribute associated with vertices.
  \ingroup Data_Structures
*/
// ---
SIMPLE_VERTEX_ATTRIBUTE( double, 0., MbHalfedgeAttrDensityVertex, MbeTypeAttr::ta_Density );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для беззнаковых целых чисел, ассоциированных с вершинами.
           \en The unsigned int attribute associated with vertices.
  \ingroup Data_Structures
*/
// ---
SIMPLE_VERTEX_ATTRIBUTE( uint, SYS_MAX_UINT, MbHalfedgeAttrUintVertex, MbeTypeAttr::ta_UintValuesVertex );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для беззнаковых целых чисел, ассоциированных с ребрами.
           \en The unsigned int attribute associated with edges.
  \ingroup Data_Structures
*/
// ---
SIMPLE_EDGE_ATTRIBUTE( uint, SYS_MAX_UINT, MbHalfedgeAttrUintEdge, MbeTypeAttr::ta_UintValuesEdge );


/** \brief \ru Атрибут для беззнаковых целых чисел, ассоциированных с фасетами.
           \en The unsigned int attribute associated with facets.
  \ingroup Data_Structures
*/
// ---
SIMPLE_FACET_ATTRIBUTE( uint, SYS_MAX_UINT, MbHalfedgeAttrUintFacet, MbeTypeAttr::ta_UintValuesFacet );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для целых чисел, ассоциированных с вершинами.
           \en The int attribute associated with vertices.
  \ingroup Data_Structures
*/
// ---
SIMPLE_VERTEX_ATTRIBUTE( int, SYS_MAX_UINT, MbHalfedgeAttrIntVertex, MbeTypeAttr::ta_IntValuesVertex );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для целых чисел, ассоциированных с ребрами.
           \en The int attribute associated with edges.
  \ingroup Data_Structures
*/
// ---
SIMPLE_EDGE_ATTRIBUTE( int, SYS_MAX_UINT, MbHalfedgeAttrIntEdge, MbeTypeAttr::ta_IntValuesEdge );


/** \brief \ru Атрибут для целых чисел, ассоциированных с фасетами.
           \en The int attribute associated with facets.
  \ingroup Data_Structures
*/
// ---
SIMPLE_FACET_ATTRIBUTE( int, SYS_MAX_UINT, MbHalfedgeAttrIntFacet, MbeTypeAttr::ta_IntValuesFacet );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для типа size_t, ассоциированных с ребрами.
           \en The size_t attribute associated with edges.
  \ingroup Data_Structures
*/
// ---
SIMPLE_EDGE_ATTRIBUTE( size_t, SYS_MAX_T, MbHalfedgeAttrSizetEdge, MbeTypeAttr::ta_SizetValuesEdge );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для пар, ассоциированных с ребрами.
           \en The c3d::IndicesPair attribute associated with edges.
  \ingroup Data_Structures
*/
// ---
SIMPLE_EDGE_ATTRIBUTE( c3d::IndicesPair, c3d::IndicesPair( SYS_MAX_T, SYS_MAX_T), MbHalfedgeAttrIndPairEdge, MbeTypeAttr::ta_PairValuesEdge );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для типа size_t, ассоциированных с фасетами.
           \en The size_t attribute associated with facets.
  \ingroup Data_Structures
*/
// ---
SIMPLE_FACET_ATTRIBUTE( size_t, SYS_MAX_T, MbHalfedgeAttrSizetFacet, MbeTypeAttr::ta_SizetValuesFacet );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для типа size_t, ассоциированных с вершинами.
           \en The size_t attribute associated with vertices.
  \ingroup Data_Structures
*/
// ---
SIMPLE_VERTEX_ATTRIBUTE( size_t, SYS_MAX_T, MbHalfedgeAttrSizetVertex, MbeTypeAttr::ta_SizetValuesVertex );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для типа unsigned char, ассоциированных с ребрами.
           \en The unsigned char attribute associated with edges.
  \ingroup Data_Structures
*/
// ---
SIMPLE_EDGE_ATTRIBUTE( uint8, SYS_MAX_UINT8, MbHalfedgeAttrUcharEdge, MbeTypeAttr::ta_UcharValuesEdge );


/** \brief \ru Атрибут для типа unsigned char, ассоциированных с фасетами.
           \en The unsigned char attribute associated with facets.
  \ingroup Data_Structures
*/
// ---
SIMPLE_FACET_ATTRIBUTE( uint8, SYS_MAX_UINT8, MbHalfedgeAttrUcharFacet, MbeTypeAttr::ta_UcharValuesFacet );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для типа unsigned char, ассоциированных с вершинами.
           \en The unsigned char attribute associated with vertices.
  \ingroup Data_Structures
*/
// ---
SIMPLE_VERTEX_ATTRIBUTE( uint8, SYS_MAX_UINT8, MbHalfedgeAttrUcharVertex, MbeTypeAttr::ta_UcharValuesVertex );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для 3D векторов, ассоциированных с ребрами.
           \en The 3D vector attribute associated with edges.
  \ingroup Data_Structures
*/
// ---
SIMPLE_EDGE_ATTRIBUTE( MbVector3D, MbVector3D::zero, MbHalfedgeAttrDirectionEdge3D, MbeTypeAttr::ta_DirectionEdge3D );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для 3D векторов, ассоциированных с вершинами.
           \en The 3D vector attribute associated with vertices.
  \ingroup Data_Structures
*/
// ---
SIMPLE_VERTEX_ATTRIBUTE( MbVector3D, MbVector3D::zero, MbHalfedgeAttrDirectionVertex3D, MbeTypeAttr::ta_DirectionVertex3D );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут нормалей для вершин.
           \en The normals attribute associated with vertices
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbHalfedgeAttrNormalsVertex : public MbHalfedgeAttrEdgeDependentVertexBase
{
private:
  c3d::SpaceVectorsVector _normals; //< \ru Нормали. \en The normals array.

public:
  /// \ru Конструктор. \en Constructor.
  MbHalfedgeAttrNormalsVertex( const MbHalfedge & topo );
  /// \ru Деструктор. \en Destructor.
  virtual ~MbHalfedgeAttrNormalsVertex() {}

  /// \ru Тип атрибута. \en Attribute type.
  static MbeTypeAttr         IsA() { return MbeTypeAttr::ta_NormalsVertex; }
  /// \ru Зарезервировать память под массив вершин, ребер и фасетов. \en Reserve memory to store attribute's data.
  void Reserve( size_t /*nVx*/, size_t nEd, size_t /*nF*/ ) override { _ids.reserve( nEd ); _normals.reserve( nEd ); }
  /// \ru Получить размер массива нормалей. \en Get size of the normals array.
  size_t ValuesCount() const override { return _normals.size(); }
  /// \ru Установить размер массива нормалей. \en Set size of the normals array.
  void SetSizeNormals( size_t nNm ) { _normals.assign( nNm, MbVector3D() ); }
  /// \ru Получить нормаль для вершины ребра. \en Get normal for the edge's origin vertex.
  const MbVector3D & GetNormalEdge( size_t iEd ) const { return _normals[_ids[iEd]]; }
  /// \ru Установить нормаль по заданному индексу. \en Set the normal at the given index.
  void SetNormal( size_t iNm, const MbVector3D & nm ) { _normals[iNm] = nm; }
  /// \ru Получить нормаль по заданному индексу. \en Get the normal at the given index.
  const MbVector3D & GetNormal( size_t iNm ) const { return _normals[iNm]; }
  /// \ru Получить все нормали. \en Get all normals.
  c3d::SpaceVectorsVector & GetNormals() { return _normals; }
  /// \ru Получить все нормали. \en Get all normals.
  const c3d::SpaceVectorsVector & GetNormals() const { return _normals; }
  /// \ru Преобразовать согласно матрице. \en Transform by given matrix.
  void Transform( const MbMatrix3D & matr ) override;

private:
  /// \ru Добавить новое значение. \en Add new value.
  size_t AddValueInternal() override { _normals.emplace_back(); return _normals.size() - 1; }

  HALFEDGE_VISITING();
  DECLARE_PERSISTENT_CLASS( MbHalfedgeAttrNormalsVertex );
};


//------------------------------------------------------------------------------
/** \brief \ru Атрибут 2Д параметров для вершин.
           \en The 2D parameter attribute for vertices.
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbHalfedgeAttrParamByEdge : public MbHalfedgeAttrEdgeDependentVertexBase
{
private:
  c3d::ParamPointsVector _uvs; //< \ru Параметры. \en The 2D Parameters array.

public:
  /// \ru Конструктор. \en Constructor.
  MbHalfedgeAttrParamByEdge( const MbHalfedge & topo );
  /// \ru Деструктор. \en Destructor.
  virtual ~MbHalfedgeAttrParamByEdge() {}

  /// \ru Тип атрибута. \en Attribute type.
  static MbeTypeAttr         IsA() { return MbeTypeAttr::ta_Parameters2DEdgeDependent; }
  /// \ru Зарезервировать память под массив вершин, ребер и фасетов. \en Reserve memory to store attribute's data.
  void Reserve( size_t /*nVx*/, size_t nEd, size_t /*nF*/ ) override { _ids.reserve( nEd ); _uvs.reserve( nEd ); }
  /// \ru Получить размер массива параметров. \en Get size of the parameters array.
  size_t ValuesCount() const override { return _uvs.size(); }
  /// \ru Установить размер массива параметров. \en Set size of the parameters array.
  void SetSizeParams( size_t nNm ) { _uvs.assign( nNm, MbCartPoint() ); }
  /// \ru Получить параметр для вершины ребра. \en Get the 2D parameter for the edge's origin vertex.
  const MbCartPoint & GetParamEdge( size_t iEd ) const { return _uvs[_ids[iEd]]; }
  /// \ru Получить параметр для вершины ребра. \en Get the 2D parameter for the edge's origin vertex.
  MbCartPoint & SetParamEdge( size_t iEd ) { return _uvs[_ids[iEd]]; }
  /// \ru Добавить параметр. \en Add the parameter.
  size_t AddParam( const MbCartPoint & prm ) { _uvs.push_back( prm ); return _uvs.size() - 1; }
  /// \ru Установить параметр по заданному индексу. \en Set the parameter by the given index.
  void SetParam( size_t iParam, const MbCartPoint & prm ) { _uvs[iParam] = prm; }
  /// \ru Получить параметр по заданному индексу. \en Get the parameter by the given index.
  const MbCartPoint & GetParam( size_t iParam ) const { return _uvs[iParam]; }
  /// \ru Получить все параметры. \en Get all parameters.
  c3d::ParamPointsVector & GetParams() { return _uvs; }
  /// \ru Получить все параметры. \en Get all parameters.
  const c3d::ParamPointsVector & GetParams() const { return _uvs; }
  /// \ru Получить центр треугольного фасета. \en Get 2D parameter at the center of the triangular facet.
  bool GetTriangleCenter( size_t iTr, MbCartPoint & center ) const;
  /// \ru Рассчитать точку на ребре с заданным параметром [0,1]. \en Calculate a point on the edge with the given position [0, 1]
  bool CalculateEdgePoint( size_t iEd, double prm, MbCartPoint & pt ) const;
  /// \ru Посчитать направление ребра. \en Calculate the edge's direction.
  bool CalculateEdgeDirection( size_t iEd, MbVector & dir ) const;
  /// \ru Получить 2Д параметры треугольного фасета. \en Get 2D parameters for the triangular facet.
  bool GetTrianglePoints( size_t iTr, std::array<MbCartPoint, 3> & points ) const;

private:
  /// \ru Добавить новое значение. \en Add new value. 
  size_t AddValueInternal() override { _uvs.emplace_back(); return _uvs.size() - 1; }

  HALFEDGE_VISITING();
  DECLARE_PERSISTENT_CLASS( MbHalfedgeAttrParamByEdge );
};


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для двугранных углов ребер.
           \en The dihedral angle attribute for edges.
  \ingroup Data_Structures
*/
// ---
SIMPLE_EDGE_ATTRIBUTE( double, 0., MbHalfedgeAttrDihedralAngles, MbeTypeAttr::ta_DihedralAngles );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для нормалей фасетов.
           \en The normal attribute for facets.
  \ingroup Data_Structures
*/
// ---
SIMPLE_FACET_ATTRIBUTE( MbVector3D, MbVector3D::zero, MbHalfedgeAttrNormalsFacet, MbeTypeAttr::ta_NormalsFacet );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для деформаций треугольников.
           \en The deformation attribute for triangular facets.
  \ingroup Data_Structures
*/
// ---
SIMPLE_FACET_ATTRIBUTE( MbVector3D, MbVector3D::zero, MbHalfedgeAttrStrains, MbeTypeAttr::ta_Strains );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для острых ребер.
           \en The sharp edge attribute for edges.
  \ingroup Data_Structures
*/
// ---
SIMPLE_EDGE_ATTRIBUTE( bool, false, MbHalfedgeAttrSharpEdge, MbeTypeAttr::ta_Sharp );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для помеченных фасетов.
           \en The mark attribute for facets.
  \ingroup Data_Structures
*/
// ---
SIMPLE_FACET_ATTRIBUTE( bool, false, MbHalfedgeAttrBoolFacet, MbeTypeAttr::ta_BoolValuesFacet );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для помеченных ребер.
           \en The mark attribute for edges.
  \ingroup Data_Structures
*/
// ---
SIMPLE_EDGE_ATTRIBUTE( bool, false, MbHalfedgeAttrBoolEdge, MbeTypeAttr::ta_BoolValuesEdge );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для помеченных вершин.
           \en The mark attribute for vertices.
  \ingroup Data_Structures
*/
// ---
SIMPLE_VERTEX_ATTRIBUTE( bool, false, MbHalfedgeAttrBoolVertex, MbeTypeAttr::ta_BoolValuesVertex );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для линий очерка.
           \en The outline line attribute for edges.
   
  \ingroup DaMbeTypeAttr::ta_Structures
*/
// ---
SIMPLE_EDGE_ATTRIBUTE( bool, false, MbHalfedgeAttrSilhouette, MbeTypeAttr::ta_Silhouette );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для линии пересечения.
           \en The intersect line attribute for edges.
  \ingroup DaMbeTypeAttr::ta_Structures
*/
// ---
SIMPLE_EDGE_ATTRIBUTE( bool, false, MbHalfedgeAttrIntersect, MbeTypeAttr::ta_IntersectEdge );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для флага положения вершины.
           \en The location flag attribute for vertices.
  \ingroup DaMbeTypeAttr::ta_Structures
*/
// ---
SIMPLE_VERTEX_ATTRIBUTE( int, SYS_MAX_INT32, MbHalfedgeAttrLocationVertex, MbeTypeAttr::ta_LocationVertex );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут для флага положения фасета.
           \en The location flag attribute for facets.
  \ingroup DaMbeTypeAttr::ta_Structures
*/
// ---
SIMPLE_FACET_ATTRIBUTE( int, SYS_MAX_INT32, MbHalfedgeAttrLocationFacet, MbeTypeAttr::ta_LocationFacet );


//------------------------------------------------------------------------------
/** \brief \ru Целочисленный атрибут для фасетов.
           \en The internal integer attribute for facets.
  \ingroup DaMbeTypeAttr::ta_Structures
*/
// ---
SIMPLE_FACET_ATTRIBUTE( uint, SYS_MAX_UINT, MbHalfedgeAttrComplFacet, MbeTypeAttr::ta_ComplFacet );


//------------------------------------------------------------------------------
/** \brief \ru Атрибут кривизн для вершин.
           \en The curvature attribute for vertices.
  \ingroup DaMbeTypeAttr::ta_Structures
*/
// ---
class MATH_CLASS MbHalfedgeAttrCurvaturesVertex : public MbHalfedgeAttrEdgeDependentVertexBase
{
private:
  std::vector<MbCurvature> _curvatures; //< \ru Кривизны. \en Curvatures array.

public:
  /// \ru Конструктор. \en Constructor.
  MbHalfedgeAttrCurvaturesVertex( const MbHalfedge & topo ) : MbHalfedgeAttrEdgeDependentVertexBase( topo, MbeTypeAttr::ta_Curvatures ) {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbHalfedgeAttrCurvaturesVertex() {}

  /// \ru Тип атрибута. \en Attribute type.
  static MbeTypeAttr         IsA() { return MbeTypeAttr::ta_Curvatures; }
  /// \ru Получить размер массива кривизн. \en Get the size of curvatures array.
  size_t ValuesCount() const override { return _curvatures.size(); }
  /// \ru Зарезервировать память под массив вершин, ребер и фасетов. \en Reserve memory to store attribute's data.
  void Reserve( size_t /*nVx*/, size_t nEd, size_t /*nF*/ ) override { _ids.reserve( nEd ); _curvatures.reserve( nEd ); }

  /// \ru Установить размер массива кривизн. \en Set the size of curvatures array.
  void SetSizeCurvatures( size_t nElem ) { _curvatures.assign( nElem, MbCurvature() ); }
  /// \ru Получить кривизну для вершины ребра. \en Get the curvature for edge's origin vertex.
  const MbCurvature & GetCurvatureEdge( size_t iEd ) const { return _curvatures[_ids[iEd]]; }
  /// \ru Получить кривизну по индексу. \en Get the curvature by index.
  const MbCurvature & GetCurvature( size_t iCurv ) const { return _curvatures[iCurv]; }
  /// \ru Получить кривизну по индексу. \en Get the curvature by index.
  MbCurvature & SetCurvature( size_t iCurv ) { return _curvatures[iCurv]; }
  /// \ru Установить кривизну по заданному индексу. \en Set the curvature by index.
  void SetCurvature( size_t iNm, const MbCurvature & curv ) { _curvatures[iNm] = curv; }
  /// \ru Получить все кривизны. \en Get all curvatures.
  const std::vector<MbCurvature> & GetCurvatures() const { return _curvatures; }
  /// \ru Получить все кривизны. \en Get all curvatures.
  std::vector<MbCurvature> & GetCurvatures() { return _curvatures; }
  /// \ru Преобразовать согласно матрице. \en Transform by given matrix.
  void Transform( const MbMatrix3D & matr ) override;

private:
  /// \ru Добавить новое значение. \en Add new value.
  size_t AddValueInternal() override { _curvatures.emplace_back(); return _curvatures.size() - 1; }

  HALFEDGE_VISITING();
  DECLARE_PERSISTENT_CLASS( MbHalfedgeAttrCurvaturesVertex );
};

#endif // __MESH_HALFEDGE_ATTR_H