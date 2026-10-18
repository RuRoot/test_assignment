////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Топология полигональной сетки и ее сегментации в виде half-edge.
         \en The topology of a polygonal mesh and it's segmentation in the half-edge representation. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __POLYMESH_TOPO_H
#define __POLYMESH_TOPO_H

#include <reference_item.h>
#include <halfedge.h>

class MbMesh;
class MbFaceShell;
class MbFace;
class MbCurveEdge;
class MbCube;
class MbAttributeContainer;
class IMeshProvider;
class MbRepairInconsistentMeshParams;

//------------------------------------------------------------------------------
/** \brief \ru Представление топологии полигональной модели в виде halfedge.
           \en Mesh topology representation as a halfedge. \~
  \details \ru Halfedge представление - это представление полигональной модели в виде графа, где ключевым элементом
               представления является направленное ребро, ассоциированное с фасетом, которому оно принадлежит, и вершиной,
               из которой ребро исходит. Таким образом, каждое неграничное ребро поверхности представлено в виде пары
               противоположно направленных ребер (реберной пары). \n
               Представление в виде графа позволяет иметь информацию о топологии поверхности, которую может быть сложно представить
               в виде обычного TriMesh либо полигонального представления, и открывает доступ к большой группе более эффективных алгоритмов,
               использующих операции на графах. \n
               Для перемещения по топологии ребра организуются в замкнутые циклы вокруг фасетов. Для ребра можно получить следующее ребро
               в цикле, предыдущее ребро в цикле, фасет, циклу которого принадлежит ребро, и обратное ребро, указывающее в противоположном направлении.
               Фасеты, организованные вокруг вершины, образуют так называемый вершинный веер. Для удобства существует еще один вариант обхода топологии,
               в котором осуществляется перебор фасетов либо реберных пар вокруг этой вершины в порядке расположения по вершинному вееру, для этого
               каждая вершина содержит ссылку на ребро, с которого возможен полный обход веера против часовой стрелки. \n
               Для обеспечения гибкости представления каждому элементу топологии возможно назначение атрибута, хранящего дополнительную информацию
               о данном элементе. Так, например, у вершины может быть атрибут, хранящий информацию о ее 3D координатах. Через атрибуты
               возможно назначение пользователем производных данных типов bool, float, double, unsigned int, int, и size_t. \n
               Как дополнительный набор информации объект дополнительно к топологии хранит информацию о сегментации объекта. \n
               Объект может быть разделен на регионы. Каждый регион содержит набор связных компонент, именующихся подрегионами.
               По аналогии с организацией half-edge каждый подрегион может иметь один или несколько сегментационных реберных циклов.
           \en Halfedge representation portrays a polygonal model as a directed graph. The key element is a directed edge associated with
               the facet to which it belongs and the vertex from which the edge originates. Each non-boundary edge of the surface in this model
               is represented as a pair of oppositely directed edges (the edge itself and its twin). \n
               With graph representation information about the topology can be stored and used in a wide range of more efficient graph-based algorithms
               as compared with the traditional representation, which suffers with inefficiency. \n
               For traversal edges are organized into closed loops around facets. For an edge, you can get the next edge in this loop,
               the previous edge in the loop, the facet to which the loop belongs, and the twin edge.
               Facets are grouped around the vertex form a vertex fan. An additional way to iterate through the topology is to cycle through
               facets or edge pairs in the vertex fan. For this purpose each vertex contains a reference to a beginning edge for a counterclockwise traversal. \n
               To provide a flexible way to manipulate data associated with the topology, each element can be assigned with an attribute that stores
               additional information. For example, a vertex can have an attribute that stores its 3D coordinats. With attributes it is possible to
               store user data with bool, float, double, unsigned int, int, and size_t information. \n
               This object, in addition to the topology, stores information about the object's segmentation. \n
               An object can be logically divided into regions. Each region can store a set of connected components, named subregions.
               Similar to the halfedge organization, each subregion can have one or more edge loops.
  \ingroup Topology_Items
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbPolymeshTopo : public MbRefItem
{
protected:
  /// \ru Конструктор. \en Constructor.
  MbPolymeshTopo();

public:
  /// \ru Создать пустой экземпляр класса. \en Create an empty object. 
  static MbPolymeshTopo * Create();
  /** \brief \ru Создать экземпляр класса на основе MbMesh.
             \en Create an object by MbMesh. \~
    \param[in] mesh         - \ru Полигональный геометрический объект.
                              \en A polygonal geometric object. \~
    \param[in] toleranceSew - \ru Метрическая погрешность совпадения точек.
                              \en Metric tolerance of points coincidence. \~.
    \return \ru Возвращает указатель на созданный объект.
            \en Returns pointer to created object. \~
  */
  static MbPolymeshTopo * Create( const MbMesh & mesh, double toleranceSew );
  /** \brief \ru Создать экземпляр класса на основе контейнера данных и параметров лечения.
             \en Create an object by mesh provider and repairing parameters. \~
    \param[in] provider         - \ru Контейнер данных полигонального объекта.
                                  \en A container for polygonal object data. \~
    \param[in] params - \ru Параметры лечения.
                        \en Repairing parameters. \~.
    \return \ru Возвращает указатель на созданный объект.
            \en Returns pointer to created object. \~
  */
  static MbPolymeshTopo * Create( const IMeshProvider & provider, const MbRepairInconsistentMeshParams & params );
  /// \ru Деструктор. \en Destructor.
  virtual ~MbPolymeshTopo() {}

  
  /** \brief \ru Создать полигональный геометрический объект MbMesh на основе сегментации.
             \en Create a polygonal geometric object MbMesh by segmentation. \~
    \param[out] mesh - \ru Полигональный геометрический объект.
                       \en A polygonal geometric object. \~
    \return \ru Возвращает true, если геометрический объект успешно создан, и false иначе.
            \en Returns true, if an object is successfully constructed, and false otherwise. \~
  */
  virtual bool GetMesh( MbMesh & mesh ) const = 0;
  /// \ru Клонировать. \en Clone. 
  virtual MbPolymeshTopo * Clone() const = 0;
  /** \brief \ru Рассчитать габарит в локальной системе координат. 
             \en Calculate a bounding box in the local coordinate system. \~
    \param[in] matrInto - \ru Матрица преобразования в локальную СК.
                          \en A matrix of transformation into a local coordinate system. \~
    \param[out] bb      - \ru Габарит.
                          \en A bounding box. \~
  */
  virtual void CalculateGabarit( const MbMatrix3D & matrInto, MbCube & bb ) const = 0;

  // \ru Функции для работы с топологией сетки. \en Functions for working with the topology of a polygonal mesh.

  /// \ru Получить топологию сетки. \en Get mesh topology.
  virtual const MbHalfedge & GetMeshTopo() const = 0;
  /** \brief \ru Зарезервировать память под вершины, ребра и фасеты. 
             \en Reserve memory for vertices, edges and facets. \~
    \param[in] nVx - \ru Количество вершин.
                     \en Vertex count. \~
    \param[in] nEd - \ru Количество ребер.
                     \en Edge count. \~
    \param[in] nF  - \ru Количество фасетов.
                     \en Facet count. \~
  */
  virtual void Reserve( size_t nVx, size_t nEd, size_t nF ) = 0;

  /** \brief \ru Клонировать вершину.
             \en Clone vertex. \~
    \param[in] iVx - \ru Индекс клонируемой вершины.
                     \en The index of a cloned vertex. \~
    \return \ru Возвращает индекс созданной вершины.
            \en Returns the index of a new vertex. \~
  */
  virtual size_t CloneVertex( size_t iVx ) = 0;
  /// \ru Добавить активную вершину. \en Add an active vertex.
  virtual size_t AddVertex() = 0;
  /// \ru Добавить активное ребро. \en Add an active edge.
  virtual size_t AddEdge() = 0;
  /// \ru Добавить активный фасет. \en Add an active facet.
  virtual size_t AddFacet() = 0;

  /** \brief \ru Сделать вершину неактивной.
             \en Make a vertex inactive. \~
    \param[in] iVx - \ru Индекс вершины.
                     \en The index of a vertex. \~
  */
  virtual void MakeVertexInactive( size_t iVx ) = 0;
  /** \brief \ru Сделать ребро неактивным.
             \en Make an edge inactive. \~
    \param[in] iEd - \ru Индекс ребра.
                     \en The index of an edge. \~
  */
  virtual void MakeEdgeInactive( size_t iEd ) = 0;
  /** \brief \ru Сделать фасет неактивным.
             \en Make an facet inactive. \~
    \param[in] iF - \ru Индекс фасета.
                    \en The index of a facet. \~
  */
  virtual void MakeFacetInactive( size_t iF ) = 0;
 
  /** \brief \ru Сделать ребра близнецами.
             \en Sew the edges. \~
    \param[in] iEd0 - \ru Индекс одного ребра.
                      \en The index of an edge. \~
    \param[in] iEd1 - \ru Индекс другого ребра.
                      \en The index of another edge. \~
  */
  virtual void SewEdges( size_t iEd0, size_t iEd1 ) = 0;
  /** \brief \ru Добавить фасет на основе существующих вершин.
             \en Add a facet by given vertices. \~
    \param[in] vertices - \ru Вершины.
                          \en Vertices. \~
    \return \ru Возвращает индекс созданного фасета.
            \en Returns the index of a new facet. \~
  */
  virtual size_t AddFacet( const c3d::IndicesVector & vertices ) = 0;
  /** \brief \ru Сшить сетку (установить близнецов) после добавления всех фасетов функцией AddFacet( const c3d::IndicesVector & vertices ).
             \en Sew mesh (set twins) after adding all facets by AddFacet( const c3d::IndicesVector & vertices ) function. \~
  */
  virtual void SewByVertices() = 0;

  /// \ru Удалить неактивные вершины, ребра и фасеты, регионы и циклы. \en Remove inactive vertices, edges, facets, regions and loops.
  virtual void DeleteInactive() = 0;
  /// \ru Инвертировать нормали (сменить на противоположные). \en Invert normals.
  virtual void Invert() = 0;
  /// \ru Преобразовать согласно матрице. \en Transform according to the matrix.
  virtual void Transform( const MbMatrix3D & matr ) = 0;
  /// \ru Сдвинуть вдоль вектора. \en Translate along a vector.
  virtual void Move( const MbVector3D & to ) = 0;
  /// \ru Повернуть вокруг оси. \en Rotate around an axis.
  virtual void Rotate( const MbAxis3D & axis, double angle ) = 0;

  /** \brief \ru Удалить указанные фасеты. 
             \en Delete facets. \~
    \param[in] facets - \ru Набор фасетов для удаления. 
                        \en Facets to be deleted. \~ 
  */
  virtual void DeleteFacets( const c3d::IndicesVector & facets ) = 0;
  /** \brief \ru Перебросить ребро между двумя треугольниками.
             \en Flip an edge. \~
    \param[in] iEd - \ru Ребро, которое требуется перебросить.
                     \en An edge to be flipped. \~
    \return \ru Возвращает true, если переброска выполнена успешно, и false иначе.
            \en Returns true, if an edge is successfully flipped, and false otherwise. \~
  */
  virtual bool FlipEdge( size_t iEd ) = 0;
  /** \brief \ru Разрезать ребро на два в заданном соотношении.
             \en Split an edge at a ratio. \~
    \param[in] iEd - \ru Ребро, которое требуется разрезать.
                     \en An edge to be split. \~
    \param[in] prm - \ru Параметр для разрезания ребра. Должен быть в промежутке от 0 до 1.
                     \en A ratio for the split. Has to be between 0 and 1. \~
    \return \ru Возвращает индекс вершины, созданной на месте разреза.
            \en Returns the index of a vertex created at the split point. \~
  */
  virtual size_t SplitEdge( size_t iEd, double prm ) = 0;
  /** \brief \ru Разбить фасет ребром, соединяющим вершины двух указанных ребер из цикла фасета.
             \en Split a facet by creating an edge connecting its two given edges. \~
    \param[in] eVx0  - \ru Индекс первого ребра.
                       \en The index of an edge. \~
    \param[in] eVx1  - \ru Индекс второго ребра.
                       \en The index of another edge. \~
    \param[out] eNew - \ru Индексы двух новых ребер.
                       \en The indices of two created edges. \~
  */
  virtual void SplitFacet( size_t eVx0, size_t eVx1, c3d::IndicesPair & eNew ) = 0;
  /** \brief \ru Объединить фасеты, соседние с указанным ребром.
             \en Join facets sharing a given edge. \~
    \details \ru Объединить фасеты, соседние с указанным ребром. Операция, обратная к SplitFacet.
             \en Join facets sharing a given edge. An inverse operation to SplitFacet. \~
    \param[in] iEd - \ru Индекс ребра.
                     \en The index of an edge. \~
    \return \ru Возвращает true, если объединение выполнено успешно, и false иначе.
            \en Returns true, if facets are successfully joined, and false otherwise. \~
  */
  virtual bool JoinEdgeFacets( size_t iEd ) = 0;
  /** \brief \ru Удалить вершину валентности два, объединить соотвествующие ребра в одно ребро.
             \en Remove the vertex of valence two and join the corresponding edges into one. \~
    \param[in] iVx - \ru Индекс вершины.
                     \en The index of a vertex. \~
    \return \ru Возвращает true, если удаление выполнено успешно, и false иначе.
            \en Returns true, if a vertex is successfully deleted, and false otherwise. \~
  */
  virtual bool RemoveVertexValence2( size_t iVx ) = 0;
  /** \brief \ru Внедрить в треугольник вершину, определяемую заданными барицентрическими координатами.
             \en Insert a vertex into a triangle by baricentric coordinates. \~
    \param[in] iTr - \ru Индекс треугольника.
                     \en The index of a triangle. \~
    \param[in] u   - \ru Первая барицентрическая координата.
                     \en The first baricentric coordinate. \~
    \param[in] v   - \ru Вторая барицентрическая координата.
                     \en The second baricentric coordinate. \~
    \return \ru Возвращает индекс созданной вершины.
            \en Returns the index of a created vertex. \~
  */
  virtual size_t InsertVertexTriangle( size_t iTr, double u, double v ) = 0;
  /** \brief \ru Внедрить в фасет вершину, определяемую заданными барицентрическими координатами.
             \en Insert a vertex into a facet by baricentric coordinates. \~
    \param[in] edgesSplit       - \ru Набор ребер, вершины которых будут соединяться с новой вершиной.
                                  \en Edges, whose vertices are connected with a new vertex. \~
    \param[in] edgesInterpolate - \ru Набор ребер, значения в вершинах которых используются для интерполяции.
                                  \en Edges, whose vertices are used for interpolation. \~
    \param[in] u                - \ru Первая барицентрическая координата.
                                  \en The first baricentric coordinate. \~
    \param[in] v                - \ru Вторая барицентрическая координата.
                                  \en The second baricentric coordinate. \~
    \return \ru Возвращает индекс созданной вершины.
            \en Returns the index of a created vertex. \~
  */
  virtual size_t InsertVertexFacet( const c3d::IndicesVector & edgesSplit, const std::array<size_t, 3> & edgesInterpolate, double u, double v ) = 0;

  /** \brief \ru Найти ближайшую точку и вернуть расстояние до нее.
             \en Find the nearest point and return the distance to it. \~
    \param[in] pnt      - \ru Точка.
                          \en A point. \~
    \param[out] pntProj - \ru Ближайшая точка.
                          \en The nearest point. \~
    \param[out] iFacet  - \ru Индекс фасета, содержащего ближайшую точку.
                          \en The index of a facet containing the nearest point. \~
    \return \ru Возвращает расстояние до ближайшей точки.
            \en Returns the distance to the nearest point. \~
  */
  virtual double FindNearest( const MbCartPoint3D & pnt, MbCartPoint3D & pntProj, size_t & iFacet ) const = 0;
  /** \brief \ru Найти ближайшую точку пересечения лучом.
             \en Find the nearest point of intersection with a ray. \~
    \param[in] ray       - \ru Луч.
                           \en A ray. \~
    \param[out] pntCross - \ru Ближайшая точка пересечения.
                           \en The nearest intersection point. \~
    \param[out] tRes     - \ru Параметр луча в точке пересечения.
                           \en The parameter of the ray in the intersection point. \~
    \param[out] iFacet   - \ru Индекс фасета, пересекающегося с лучом.
                           \en The index of a facet intersecting with a ray. \~
    \return \ru Возвращает true, если пересечение есть.
            \en Returns true, if the ray intersects a facet. \~
  */
  virtual bool FindLineIntersection( const MbAxis3D & ray, MbCartPoint3D & pntCross, double & tRes, size_t & iFacet ) const = 0;
  /** \brief \ru Триангулировать заданный фасет.
             \en Triangulate a given facet. \~
    \param[in] iFacet - \ru Индекс фасета, который нужно триангулировать.
                        \en The index of a facet being triangulated. \~
    \return \ru Возвращает true, если триангуляция прошла успешно.
            \en Returns true, if the triangulation is successful. \~
  */
  virtual bool TriangulateFacet( size_t iFacet ) = 0;
  /** \brief \ru Выделить набор фасетов в отдельный объект.
             \en Gather selected facets into a separate object. \~
    \param[in]  facets - \ru Набор фасетов.
                         \en Facets forming a part. \~
    \param[out] part   - \ru Отдельный объект.
                         \en A separate object. \~
  */
  virtual void GetPart( const c3d::IndicesVector & facets, MbPolymeshTopo & part ) const = 0;

  // \ru Функции для работы с топологией сегментации. \en Functions for working with the topology of a segmentation.

  /// \ru Установить, инициализирована ли сегментация. \en Check whether the segmentation is initialized.
  virtual bool IsSegmented() const = 0;
  /// \ru Инициализировать сегментацию критерием границы. \en Initialize a segmentation by a boundary criterion.
  virtual void Initialize( const MbHalfedgeBoundaryCriterionBase & crit ) = 0;
  /// \ru Инициализировать сегментацию из одного региона. \en Initialize segmentation with one region.
  virtual void Initialize() = 0;
  /// \ru Получить топологию сегментации. \en Get the topology of a segmentation.
  virtual const MbHalfedge & GetSegmTopo() const = 0;

  /// \ru Получить количество циклов. \en Get loop count.
  virtual size_t LoopsCount() const = 0;
  /// \ru Получить цикл. \en Get a loop.
  virtual const MbSegmentationLoop & Loop( size_t iLoop ) const = 0;
  /// \ru Установить, является ли цикл внутренним (все его ребра внутренние). \en Check whether the loop is inner.
  virtual bool IsInnerLoop( size_t iLoop ) const = 0;

  /// \ru Получить количество регионов в сегментации. \en Get region count.
  virtual size_t RegionCount() const = 0;
  /// \ru Установить, активен ли регион. \en Check whether the region is active.
  virtual bool IsRegionActive( size_t iRegion ) const = 0;
  /// \ru Получить количество подрегионов. \en Get subregions count.
  virtual size_t SubRegionsCount() const = 0;
  /// \ru Получить количество подрегионов для региона в сегментации. \en Get subregion count for a given region.
  virtual size_t SubRegionsCount( size_t iRegion ) const = 0;
  /// \ru Получить подрегион. \en Get a subregion.
  virtual const MbSegmentationSubRegion & SubRegion( size_t iSub ) const = 0;

  /// \ru Получить количество ребер в сегментации. \en Get segmentation edge count.
  virtual size_t EdgeSegmCount() const = 0;
  /// \ru Получить ребро сегментации по индексу. \en Get a segmentation edge.
  virtual const MbHalfedgeEdge & EdgeSegm( size_t iEdSegm ) const = 0;
  /// \ru Является ли ребро сегментации внутренним. \en Is a segmentation edge inner.
  virtual bool IsInnerEdge( size_t iEdSegm ) const = 0;
  /// \ru Получить индекс сегментационного ребра для ребра сетки. \en Get the index of a segmentation edge for a mesh edge.
  virtual size_t GetSegmEdgeId( size_t iEdTopo ) const = 0;
  /// \ru Получить идентификатор региона и подрегиона фасета. \en Get the region index and subregion index for the given facet.
  virtual void GetFacetRegionId( size_t iFacet, size_t & iRegion, size_t & iSubRegion ) const = 0;  

  /// \ru Получить количество вершин в сегментации. \en Get segmentation vertex count.
  virtual size_t VertexSegmCount() const = 0;
  /// \ru Получить вершину сегментации по индексу. \en Get a segmentation vertex.
  virtual const MbHalfedgeVertex & VertexSegm( size_t iVxSegm ) const = 0;

  /** \brief \ru Получить индексы фасетов, относящихся к региону.
             \en Get the indices of facets inside a region. \~
    \param[in] r       - \ru Индекс региона.
                         \en The index of a region. \~
    \param[out] facets - \ru Индексы фасетов.
                         \en The indices of facets. \~
  */
  virtual void GetRegion( size_t r, c3d::IndicesVector & facets ) const = 0;
  /** \brief \ru Получить индексы фасетов, относящихся к подрегиону региона.
             \en Get the indices of facets inside a subregion. \~
    \param[in] iSub    - \ru Индекс подрегиона.
                         \en The index of a subregion. \~
    \param[out] facets - \ru Индексы фасетов.
                         \en The indices of facets. \~
    \param[in] bAdd    - \ru Добавлять в конец списка фасетов.
                         \en Add to the end of the facet array. \~
  */
  virtual void GetSubRegion( size_t iSub, c3d::IndicesVector & facets, bool bAdd = false ) const = 0;
  /// \ru Удалить регион. \en Delete a region.
  virtual void DeleteRegion( size_t iRegion ) = 0;
  /** \brief \ru Разбить сегментацию выбранными ребрами.
             \en Split a segmentation by seleted edges. \~
    \param[in] splitSet     - \ru Ребра.
                              \en Edge set. \~
    \param[in] bKeepRegions - \ru Сохранять прежние регионы (то есть не выделять новые подрегионы в отдельные регионы).
                              \en Keep old regions (i.e. do not make separate regions from new subregions). \~
  */
  virtual void SplitByEdgesTopo( const c3d::IndicesVector & splitSet, bool bKeepRegions ) = 0;
  /** \brief \ru Удалить ребра сетки из ребер сегментации.
            \en Remove mesh edges from segmentation edges. \~
   \param[in] edgesToDel - \ru Удаляемые ребра сетки.
                            \en Mesh edges to remove. \~
 */
  virtual void RemoveTopoEdgesFromSegm( const c3d::IndicesVector & edgesToDel ) = 0;
  /** \brief \ru Удалить ребра сегментации.
  *              После удаления ребер общие регионы объединяются и 
  *              сохраняется регион, у которого наибольшая площадь.
             \en Segmentation edges to remove. 
  *              Merges all regions connected through removed edges into one. 
                 This region has the ID of the original region with the largest area. \~
    \param[in] edgesToDel - \ru Удаляемые ребра сегментации.
                            \en Remove segmentation edges. \~
  */
  virtual bool RemoveSegmEdges( const c3d::IndicesVector & edgesToDel ) = 0;
  /** \brief \ru Объединить несколько регионов в один.
             \en Merge several regions into one. \~
    \param[in] regions         - \ru Регионы.
                                 \en Regions. \~
    \param[in] bRemoveValence2 - \ru Удалить вершины сегментации валентности два.
                                 \en Remove vertices of valence two. \~
  */
  virtual void MergeRegions( const c3d::IndicesVector & regions, bool bRemoveValence2 ) = 0;
  /** \brief \ru Расшить ребра сегментации.
             \en Unsew segmentation edges. \~
    \param[in] edgesSegmToUnsew - \ru Ребра сегментации.
                                  \en Segmentation edges. \~
  */
  virtual void UnsewSegmEdges( const c3d::IndicesVector & edgesSegmToUnsew ) = 0;
  /** \brief \ru Расшить ребра сетки.
           \en Unsew topology edges. \~
  \param[in] edgesTopoToUnsew - \ru Ребра сетки.
                                \en Topology edges. \~
*/
  virtual void UnsewTopoEdges( const c3d::IndicesVector & edgesTopoToUnsew ) = 0;
  /** \brief \ru Разбить сегментацию выбранными фасетами.
             \en Split a segmentation by a given facet set. \~
    \param[in] splitSet       - \ru Фасеты.
                                \en Facet set. \~
    \param[in] bMakeOneRegion - \ru Объединять выбранные фасеты в один регион.
                                \en Unite all the given facets into a region. \~
    \param[in] bKeepRegions   - \ru Сохранять прежние регионы (то есть не выделять новые подрегионы в отдельные регионы).
                                \en Keep old regions (i.e. do not make separate regions from new subregions). \~
  */
  virtual void SplitByFacetsTopo( const c3d::IndicesVector & splitSet, bool bMakeOneRegion, bool bKeepRegions ) = 0;
  /// \ru Выделить несвязные подрегионы региона как отдельные регионы. \en Make new regions from the subregions of a region.
  virtual void SplitRegionBySubregions( size_t iReg ) = 0;
  /** \brief \ru Разрезать ребро сегментации на два по ребру сетки.
             \en Split a segmentation edge by a given mesh edge. \~
    \param[in] iEdSegm      - \ru Ребро сегментации.
                              \en A segmentation edge. \~
    \param[in] iEdTopo      - \ru Ребро сетки.
                              \en A mesh edge. \~
    \param[in] iVxSegmSplit - \ru Индекс вершины сегментации в точке разреза (если не задан, вершина будет добавлена).
                              \en The index of a segmentation vertex at the split point (will be added if is not given). \~
    \return \ru Новые ребра сегментации, созданные при разрезании.
            \en New segmentation edges, created during the split. \~
  */
  virtual c3d::IndicesPair SplitSegmentationEdge( size_t iEdSegm, size_t iEdTopo, size_t iVxSegmSplit = SYS_MAX_T ) = 0;
  /// \ru Выделить подрегион как отдельный регион. \en Make a new region from a given subregion.
  virtual bool ExtractSubRegionAsRegion( size_t iSub ) = 0;
  /// \ru Удалить вершину сегментации валентности два, объединить соотвествующие ребра в одно ребро. \en Remove the segmentation vertex of valence two and join the corresponding segmentation edges into one.
  virtual void RemoveSegmVertexValence2( size_t iVx ) = 0;
  /// \ru Получить индекс вершины сетки, соответствующей вершине сегментации. \en Get the index of a mesh vertex corresponding to a given segmentation vertex.
  virtual size_t GetTopoVertex( size_t iVxSegm ) const = 0;

  /** \brief \ru Найти следующее ребро в контуре.
             \en Get the next segmentation edge in a loop. \~
    \param[in] iEdSegm      - \ru Ребро сегментации.
                              \en A segmentation edge. \~
    \param[in] bExcudeInner - \ru Флаг исключения внутренних ребер сегментации.
                              \en The flag of inner edges exclusion. \~
    \return \ru Индекс следующего ребра сегментации в контуре.
            \en The index of the next segmentation edge in a loop. \~
  */
  virtual size_t NextLoopEdge( size_t iEdSegm, bool bExcudeInner ) const = 0;
  /** \brief \ru Найти предыдущее ребро в контуре.
             \en Get the previous segmentation edge in a loop. \~
    \param[in] iEdSegm      - \ru Ребро сегментации.
                              \en A segmentation edge. \~
    \param[in] bExcudeInner - \ru Флаг исключения внутренних ребер сегментации.
                              \en The flag of inner edges exclusion. \~
    \return \ru Индекс предыдущего ребра сегментации в контуре.
            \en The index of the previous segmentation edge in a loop. \~
  */
  virtual size_t PrevLoopEdge( size_t iEdSegm, bool bExcudeInner ) const = 0;

  /// \ru Получить идентификатор региона, которому принадлежит ребро. \en Get the index of a region containing a given segmentation edge.
  virtual size_t GetSegmEdgeRegionId( size_t iEdSegm ) const = 0;
  /// \ru Получить идентификатор подрегиона, которому принадлежит ребро. \en Get the index of a subregion containing a given segmentation edge.
  virtual size_t GetSegmEdgeSubRegionId( size_t iEdSegm ) const = 0;
  /// \ru Получить идентификатор региона, которому принадлежит цикл. \en Get the index of a region containing a given loop.
  virtual size_t GetLoopRegionId( size_t iLoop ) const = 0;
  /// \ru Получить идентификатор подрегиона, которому принадлежит цикл. \en Get the index of a subregion containing a given loop.
  virtual size_t GetLoopSubRegionId( size_t iLoop ) const = 0;
  /// \ru Получить идентификатор региона с другой стороны ребра сегментации. \en Get the index of a region on the other side of a segmentation edge.
  virtual size_t GetNeighborRegionId( size_t iEdSegm ) const = 0;
  /// \ru Получить идентификатор подрегиона с другой стороны ребра сегментации. \en Get the index of a subregion on the other side of a segmentation edge.
  virtual size_t GetNeighborSubRegionId( size_t iEdSegm ) const = 0;

  /// \ru Получить контейнер атрибутов для региона. \en Get the region attribute container.
  virtual MbAttributeContainer * GetSegmRegionModelAttributes( size_t iRegion, bool bCreate ) = 0;
  /// \ru Получить контейнер атрибутов для региона. \en Get the region attribute container.
  virtual const MbAttributeContainer * GetSegmRegionModelAttributes( size_t iRegion ) const = 0;
  /// \ru Получить контейнер атрибутов для ребра сегментации. \en Get the segmentation edge attribute container.
  virtual MbAttributeContainer * GetSegmEdgeModelAttributes( size_t iEdSegm, bool bCreate ) = 0;
  /// \ru Получить контейнер атрибутов для ребра сегментации. \en Get the segmentation edge attribute container.
  virtual const MbAttributeContainer * GetSegmEdgeModelAttributes( size_t iEdSegm ) const = 0;
  /// \ru Получить контейнер атрибутов для вершины сегментации. \en Get the segmentation vertex attribute container.
  virtual MbAttributeContainer * GetSegmVertexModelAttributes( size_t iVxSegm, bool bCreate ) = 0;
  /// \ru Получить контейнер атрибутов для вершины сегментации. \en Get the segmentation vertex attribute container.
  virtual const MbAttributeContainer * GetSegmVertexModelAttributes( size_t iVxSegm ) const = 0;

  /// \ru Получить количество атрибутов сетки. \en Get mesh attribute count.
  size_t MeshHalfedgeAttributeCount() const { return GetMeshTopo().AttrCount(); }
  /// \ru Получить количество атрибутов сегментации. \en Get segmentation attribute count.
  size_t SegmHalfedgeAttributeCount() const { return GetSegmTopo().AttrCount(); }

  /// \ru Добавить в сетку атрибут заданного типа. \en Add to the mesh an attribute of a given type.
  template<class T>
  T * AddMeshHalfedgeAttribute()
  {
    return MeshTopo().AddHalfedgeAttribute<T>();
  }

  /// \ru Добавить в сегментацию атрибут заданного типа. \en Add to the segmentation an attribute of a given type.
  template<class T>
  T * AddSegmHalfedgeAttribute()
  {
    return SegmTopo().AddHalfedgeAttribute<T>();
  }

  /// \ru Получить атрибут сетки заданного типа для изменения. \en Get the mesh attribute of a given type for editing.
  template<class T>
  T * GetMeshHalfedgeAttribute()
  {
    return MeshTopo().GetHalfedgeAttribute<T>();
  }

  /// \ru Получить атрибут сетки заданного типа. \en Get the mesh attribute of a given type.
  template<class T>
  const T * GetMeshHalfedgeAttribute() const
  {
    return GetMeshTopo().GetHalfedgeAttribute<T>();
  }

  /// \ru Получить атрибут сегментации заданного типа для изменения. \en Get the segmentation attribute of a given type for editing.
  template<class T>
  T * GetSegmHalfedgeAttribute()
  {
    return SegmTopo().GetHalfedgeAttribute<T>();
  }

  /// \ru Получить атрибут сегментации заданного типа. \en Get the segmentation attribute of a given type
  template<class T>
  const T * GetSegmHalfedgeAttribute() const
  {
    return GetSegmTopo().GetHalfedgeAttribute<T>();
  }

  /// \ru Удалить сеточный атрибут заданного типа. \en Delete the mesh attribute of a given type.
  template<class T>
  void DeleteMeshHalfedgeAttribute()
  {
    MeshTopo().DeleteHalfedgeAttribute<T>();
  }

  /// \ru Добавить сегментационный атрибут заданного типа. \en Delete the segmentation attribute of a given type.
  template<class T>
  void DeleteSegmHalfedgeAttribute()
  {
    SegmTopo().DeleteHalfedgeAttribute<T>();
  }

  /// \ru Получить сеточный атрибут по индексу для изменения. \en Get the mesh attribute by index for editing.
  MbHalfedgeAttrBase * GetMeshHalfedgeAttributeByIndex( size_t ind )
  {
    return MeshTopo().GetHalfedgeAttributeByIndex( ind );
  }

  /// \ru Получить сеточный атрибут по индексу. \en Get the mesh attribute by index.
  const MbHalfedgeAttrBase * GetMeshHalfedgeAttributeByIndex( size_t ind ) const
  {
    return GetMeshTopo().GetHalfedgeAttributeByIndex( ind );
  }

  /// \ru Получить сегментационный атрибут по индексу для изменения. \en Get the segmentation attribute by index for editing.
  MbHalfedgeAttrBase * GetSegmHalfedgeAttributeByIndex( size_t ind )
  {
    return SegmTopo().GetHalfedgeAttributeByIndex( ind );
  }

  /// \ru Получить сегментационный атрибут по индексу. \en Get the segmentation attribute by index.
  const MbHalfedgeAttrBase * GetSegmHalfedgeAttributeByIndex( size_t ind ) const
  {
    return GetSegmTopo().GetHalfedgeAttributeByIndex( ind );
  }

  /// \ru Удалить сеточный атрибут. \en Delete the given mesh attribute.
  void DeleteMeshHalfedgeAttribute( MbHalfedgeAttrBase * pAttr )
  {
    MeshTopo().DeleteHalfedgeAttribute( pAttr );
  }

  /// \ru Удалить сегментационный атрибут. \en Delete the given segmentation attribute.
  void DeleteSegmHalfedgeAttribute( MbHalfedgeAttrBase * pAttr )
  {
    SegmTopo().DeleteHalfedgeAttribute( pAttr );
  }

  /// \ru Получить оболочку-подложку. \en Get the substrate shell. 
  virtual const MbFaceShell * GetSubstrateShell() const = 0;
  /// \ru Удалить оболочку-подложку. \en Delete the substrate shell. 
  virtual void DeleteSubstrateShell() = 0;
  /// \ru Получить грань оболочки-подложки по заданному региону сегментации. \en Get the shell face corresponding to a given segmentation region.
  virtual const MbFace * GetSubstrateShellFace( size_t iRegion ) const = 0;
  /// \ru Получить ребро оболочки-подложки по заданному ребру сегментации. \en Get the shell edge corresponding to a given segmentation edge.
  virtual const MbCurveEdge * GetSubstrateShellEdge( size_t iEdSegm ) const = 0;

private:
  /// \ru Получить топологию сетки для изменения. \en Get the topology of a mesh for editing.
  virtual MbHalfedge & MeshTopo() = 0;
  /// \ru Получить топологию сегментации для изменения. \en Get the topology of a segmentation for editing.
  virtual MbHalfedge & SegmTopo() = 0;
    
};


//------------------------------------------------------------------------------
/** \brief \ru Маппинг между MbMesh и MbPolymesh.
           \en Mapping between MbMesh and MbPolymesh. \~
    \details \ru Маппинг между составляющими объекта MbMesh и объекта MbPolymesh, по которому был создан этот MbMesh.
             \en Mapping between parts of the MbPolymesh object and MbMesh object constructed by the polymesh. \n \~
*/
// ---
class MATH_CLASS MapPolymeshToMbMesh
{
private:
  c3d::IndicesVector _mapGridToSubRegion; ///< \ru Маппинг от гридов к подрегионам. \en Mapping from grids to subregions.
                                          ///< _mapGridToSubRegion[iGrid] = iSubregion.
  c3d::IndicesVector _mapSubRegionToGrid; ///< \ru Маппинг от подрегионов к гридам. \en Mapping from subregions to grids.
                                          ///< _mapSubRegionToGrid[iSubregion] = iGrid.

public:
  /// \ru Конструктор. \en Constructor.
  MapPolymeshToMbMesh( const c3d::IndicesVector & mapGridToSubRegion, size_t subRegionsCount );
  /// \ru Деструктор. \en Destructor.
  ~MapPolymeshToMbMesh();

  /// \ru Получить индекс подрегиона по индексу грида. \en Get subregion index by grid index.
  size_t GetSubRegionIdByGrid( size_t iGrid ) const;
  /// \ru Получить индекс грида по индексу подрегиона. \en Get grid index by subregion index.
  size_t GetGridIdBySubRegion( size_t iRegion ) const;
};

#endif // __POLYMESH_TOPO_H
