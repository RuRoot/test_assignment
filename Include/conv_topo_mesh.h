////////////////////////////////////////////////////////////////////////////////
/** 
\file
\brief Преобразователь сетки к форме, сохраняющей связи граней и полигонов.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CONV_TOPO_MESH_H
#define __CONV_TOPO_MESH_H

#include <mb_cart_point3d.h>

#include <reference_item.h>
#include <templ_sptr.h>

#include <vector>
#include <list>
#include <map>

class MbGrid;
class MbFloatGrid;
class MbItem;

namespace JTC {

  class TopoMesh;
  class TopoGrid;
  class TopoLoop;
  class TopoVertex;
  class MeshVertex;
  class MeshPolygon;

  typedef SPtr<TopoMesh> TopoMeshPtr;
  typedef SPtr<TopoGrid> TopoGridPtr;
  typedef SPtr<TopoLoop> TopoLoopPtr;
  typedef SPtr<TopoVertex> TopoVertexPtr;
  typedef SPtr<MeshVertex> MeshVertexPtr;
  typedef SPtr<MeshPolygon> MeshPolygonPtr;

  typedef std::vector<TopoGrid*> RawTopoGridVector;
  typedef std::vector<TopoGridPtr> TopoGridVector;
  typedef std::vector<TopoLoopPtr> TopoLoopVector;
  typedef std::vector<TopoVertexPtr> TopoVertexVector;
  typedef std::vector<MeshVertexPtr> MeshVertexVector;
  typedef std::vector<MeshPolygonPtr> MeshPolygonVector;

  //------------------------------------------------------------------------------
  // Сетка с топологической информацией
  // ---
  class CONV_CLASS TopoMesh : public MbRefItem {
  protected:
    SPtr<const MbItem> item;
    TopoGridVector grids;
    MeshVertexVector ownPoints;
    MeshPolygonVector ownFacePolygons;
    std::map< size_t, std::vector<size_t> > degeneratedTriangles;
    std::vector<MbCartPoint3D> boundaryPoints;
    double const metricTolerance;
  public:
    /// Конструктор
    TopoMesh();

    /// Деструктор
    virtual ~TopoMesh();

    /// Инициализировать
    bool Init( const MbItem& mesh, bool enableDiagnostics = false );

    /// Получить сетку
    SPtr<const MbItem> GetMesh() const;

    /// Число полигонов
    size_t MeshPolygonsCount() const;

    /// Получить полигон
    MeshPolygonPtr Polygon( size_t index ) const;

    /// Число вершин
    size_t MeshVerticisCount() const;

    /// Получить вершину
    MeshVertexPtr Vertex( size_t index ) const;

    /// Добавить вершину
    void AddVertex( MeshVertexPtr vert );

    /// Получить вырожденные треуголники
    std::map< size_t, std::vector<size_t> > GetDegeneratedTriangles() const;

    /// Получить граничные точки сетки
    std::vector<MbCartPoint3D> GetBoundaryPoints() const;

    /// Сбросить все данные
    void Reset();

    /// Получить следующую в цепочке граничную вершину.
    //unused:size_t NextBoundaryVertex( size_t indexBoundaryVertex, const std::vector<size_t>& allBoundary ) const;

    /// Сформировать внешнюю границу начиная с указанной вершины.
    bool InitVoidBoundFrom( std::vector<size_t>& freeBoundaryVerticis );

    /// Получить точность задания расстояния.
    double MetricTolerance() const;

    /// Получить нормаль на сетке
    void GetNormal( size_t indexGrid, size_t indexNormal, MbFloatVector3D& vNormal ) const;

    OBVIOUS_PRIVATE_COPY( TopoMesh )
  };

};

//------------------------------------------------------------------------------
// Построить сетку по полигонам
// ---
CONV_FUNC( SPtr<MbGrid> ) CreateGridByPolygonPoints( const std::vector<std::vector<MbCartPoint3D>>& polygonsAsPoints );

//------------------------------------------------------------------------------
// Создать нормали сетки по умолчанию
// ---
void CreateDefaultNormals( MbGrid & grid );


#endif // !__CONV_TOPO_MESH_H
