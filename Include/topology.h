////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Топологические объекты: вершина, ребра, цикл, грань.
         \en Topological objects: vertices, edges, loop, face. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TOPOLOGY_H
#define __TOPOLOGY_H


#include <templ_rp_array.h>
#include <templ_css_array.h>
#include <templ_three_states.h>
#include <tool_multithreading.h>
#include <mb_rect.h>
#include <mb_rect1d.h>
#include <mb_data.h>
#include <mb_enum.h>
#include <topology_item.h>
#include <list>
#include <set>
#include <map>
#include <vector>
#include <surface.h>
#include <op_binding_data.h>

class    MbCurve;
class    MbContour;
class    MbMesh;
class    MbCurve3D;
class    MbContour3D;
class    MbPlane;
class    MbContourOnSurface;
class    MbContourOnPlane;
class    MbSurfaceIntersectionCurve;
class    MbVertex;
class    MbEdge;
class    MbCurveEdge;
class    MbOrientedEdge;
class    MbLoop;
class    MbFace;
class    MbFunction;
class              MbFaceTemp;


namespace c3d // namespace C3D
{
// vertices typedefs
typedef SPtr<MbVertex>                                VertexSPtr;
typedef SPtr<const MbVertex>                          ConstVertexSPtr;

typedef std::pair<MbVertex *, MbVertex *>             VerticesPair;
typedef std::pair<const MbVertex *, const MbVertex *> ConstVerticesPair;
typedef std::pair<VertexSPtr, VertexSPtr>             VerticesSPtrPair;
typedef std::pair<ConstVertexSPtr, ConstVertexSPtr>   ConstVerticesSPtrPair;

typedef std::vector<MbVertex *>                       VerticesVector;
typedef std::vector<const MbVertex *>                 ConstVerticesVector;
typedef std::vector<VertexSPtr>                       VerticesSPtrVector;
typedef std::vector<ConstVertexSPtr>                  ConstVerticesSPtrVector;

typedef std::set<MbVertex *>                          VerticesSet;
typedef VerticesSet::iterator                         VerticesSetIt;
typedef VerticesSet::const_iterator                   VerticesSetConstIt;
typedef std::pair<VerticesSetConstIt, bool>           VerticesSetRet;

typedef std::set<VertexSPtr>                          VerticesSPtrSet;
typedef VerticesSPtrSet::iterator                     VerticesSPtrSetIt;
typedef VerticesSPtrSet::const_iterator               VerticesSPtrSetConstIt;
typedef std::pair<VerticesSPtrSetConstIt, bool>       VerticesSPtrSetRet;

typedef std::set<const MbVertex *>                    ConstVerticesSet;
typedef ConstVerticesSet::iterator                    ConstVerticesSetIt;
typedef ConstVerticesSet::const_iterator              ConstVerticesSetConstIt;
typedef std::pair<ConstVerticesSetConstIt, bool>      ConstVerticesSetRet;

typedef std::set<ConstVertexSPtr>                     ConstVerticesSPtrSet;
typedef ConstVerticesSPtrSet::iterator                ConstVerticesSPtrSetIt;
typedef ConstVerticesSPtrSet::const_iterator          ConstVerticesSPtrSetConstIt;
typedef std::pair<ConstVerticesSPtrSetConstIt, bool>  ConstVerticesSPtrSetRet;

typedef std::map<MbVertex *, MbVertex *>              VerticesPairMap;
typedef std::map<const MbVertex *, const MbVertex *>  ConstVerticesPairMap;
typedef std::map<VertexSPtr, VertexSPtr>              VerticesSPtrPairMap;
typedef std::map<ConstVertexSPtr, ConstVertexSPtr>    ConstVerticesSPtrPairMap;


// edges typedefs
typedef SPtr<MbEdge>                                    WireEdgeSPtr;
typedef SPtr<const MbEdge>                              ConstWireEdgeSPtr;

typedef std::pair<MbEdge *, MbEdge *>                   WireEdgesPair;
typedef std::pair<const MbEdge *, const MbEdge *>       ConstWireEdgesPair;
typedef std::pair<WireEdgeSPtr, WireEdgeSPtr>           WireEdgesSPtrPair;
typedef std::pair<ConstWireEdgeSPtr, ConstWireEdgeSPtr> ConstWireEdgesSPtrPair;

typedef std::vector<MbEdge *>                           WireEdgesVector;
typedef std::vector<const MbEdge *>                     ConstWireEdgesVector;
typedef std::vector<WireEdgeSPtr>                       WireEdgesSPtrVector;
typedef std::vector<ConstWireEdgeSPtr>                  ConstWireEdgesSPtrVector;


// edges typedefs
typedef SPtr<MbCurveEdge>                                   EdgeSPtr;
typedef SPtr<const MbCurveEdge>                             ConstEdgeSPtr;

typedef std::pair<MbEdge *, size_t>                         WireEdgeIndex;
typedef std::pair<const MbEdge *, size_t>                   ConstWireEdgeIndex;

typedef std::pair<MbCurveEdge *, size_t>                    EdgeIndex;
typedef std::pair<const MbCurveEdge *, size_t>              ConstEdgeIndex;

typedef std::pair<size_t, MbCurveEdge *>                    IndexEdge;
typedef std::pair<size_t, const MbCurveEdge *>              IndexConstEdge;

typedef std::pair<MbCurveEdge *, MbCurveEdge *>             EdgesPair;
typedef std::pair<const MbCurveEdge *, const MbCurveEdge *> ConstEdgesPair;
typedef std::pair<EdgeSPtr, EdgeSPtr>                       EdgesSPtrPair;
typedef std::pair<ConstEdgeSPtr, ConstEdgeSPtr>             ConstEdgesSPtrPair;

typedef std::vector<MbCurveEdge *>                          EdgesVector;
typedef std::vector<const MbCurveEdge *>                    ConstEdgesVector;
typedef std::vector<EdgeSPtr>                               EdgesSPtrVector;
typedef std::vector<ConstEdgeSPtr>                          ConstEdgesSPtrVector;

typedef std::list<MbCurveEdge *>                            EdgesList;
typedef std::list<const MbCurveEdge *>                      ConstEdgesList;

typedef std::set<MbCurveEdge *>                             EdgesSet;
typedef EdgesSet::iterator                                  EdgesSetIt;
typedef EdgesSet::const_iterator                            EdgesSetConstIt;
typedef std::pair<EdgesSetConstIt, bool>                    EdgesSetRet;

typedef std::set<EdgeSPtr>                                  EdgesSPtrSet;
typedef EdgesSPtrSet::iterator                              EdgesSPtrSetIt;
typedef EdgesSPtrSet::const_iterator                        EdgesSPtrSetConstIt;
typedef std::pair<EdgesSPtrSetConstIt, bool>                EdgesSPtrSetRet;

typedef std::set<const MbCurveEdge *>                       ConstEdgesSet;
typedef ConstEdgesSet::iterator                             ConstEdgesSetIt;
typedef ConstEdgesSet::const_iterator                       ConstEdgesSetConstIt;
typedef std::pair<ConstEdgesSetConstIt, bool>               ConstEdgesSetRet;

typedef std::set<ConstEdgeSPtr>                             ConstEdgesSPtrSet;
typedef ConstEdgesSPtrSet::iterator                         ConstEdgesSPtrSetIt;
typedef ConstEdgesSPtrSet::const_iterator                   ConstEdgesSPtrSetConstIt;
typedef std::pair<ConstEdgesSPtrSetConstIt, bool>           ConstEdgesSPtrSetRet;


// oriented edges typedefs
typedef MbOrientedEdge *                          OrientEdge;
typedef const MbOrientedEdge *                    ConstOrientEdge;

typedef SPtr<MbOrientedEdge>                      OrientEdgeSPtr;
typedef SPtr<const MbOrientedEdge>                ConstOrientEdgeSPtr;

typedef std::pair<MbCurveEdge *,bool>             EdgeOrient;
typedef std::pair<const MbCurveEdge *,bool>       ConstEdgeOrient;

typedef std::pair<EdgeSPtr, bool>                 EdgeSPtrOrient;
typedef std::pair<ConstEdgeSPtr, bool>            ConstEdgeSPtrOrient;

typedef std::vector<OrientEdge>                   OrientEdgesVector;
typedef std::vector<ConstOrientEdge>              ConstOrientEdgesVector;
typedef std::vector<OrientEdgeSPtr>               OrientEdgesSPtrVector;
typedef std::vector<ConstOrientEdgeSPtr>          ConstOrientEdgesSPtrVector;

typedef std::vector<EdgeOrient>                   EdgeOrientVector;
typedef std::vector<ConstEdgeOrient>              ConstEdgeOrientVector;
typedef std::vector<EdgeSPtrOrient>               EdgeSPtrOrientVector;
typedef std::vector<ConstEdgeSPtrOrient>          ConstEdgeSPtrOrientVector;


// loops typedefs
typedef SPtr<MbLoop>                              LoopSPtr;
typedef SPtr<const MbLoop>                        ConstLoopSPtr;

typedef std::pair<MbLoop *, size_t>               LoopIndex;
typedef std::pair<const MbLoop *, size_t>         ConstLoopIndex;

typedef std::pair<MbLoop *,ptrdiff_t>             LoopNumber;
typedef std::vector<LoopNumber>                   LoopNumberVector;

typedef std::vector<MbLoop *>                     LoopsVector;
typedef std::vector<const MbLoop *>               ConstLoopsVector;
typedef std::vector<LoopSPtr>                     LoopsSPtrVector;
typedef std::vector<ConstLoopSPtr>                ConstLoopsSPtrVector;


// faces typedefs
typedef SPtr<MbFace>                                  FaceSPtr;
typedef SPtr<const MbFace>                            ConstFaceSPtr;

typedef std::pair<MbFace *, size_t>                   FaceIndex;
typedef std::pair<const MbFace *, size_t>             ConstFaceIndex;
typedef std::pair<const MbFace*,const MbFace*>        ConstFaceFacePair;

typedef std::vector<MbFace *>                         FacesVector;
typedef std::vector<const MbFace *>                   ConstFacesVector;
typedef std::vector<FaceSPtr>                         FacesSPtrVector;
typedef std::vector<ConstFaceSPtr>                    ConstFacesSPtrVector;
typedef std::vector<ConstFaceIndex>                   ConstFaceIndicesVector;

typedef std::pair<ConstFacesVector, ConstFacesVector> ConstFacesVectorPair;

typedef std::set<MbFace *>                            FacesSet;
typedef FacesSet::iterator                            FacesSetIt;
typedef FacesSet::const_iterator                      FacesSetConstIt;
typedef std::pair<FacesSetConstIt, bool>              FacesSetRet;

typedef std::set<FaceSPtr>                            FacesSPtrSet;
typedef FacesSPtrSet::iterator                        FacesSPtrSetIt;
typedef FacesSPtrSet::const_iterator                  FacesSPtrSetConstIt;
typedef std::pair<FacesSPtrSetConstIt, bool>          FacesSPtrSetRet;

typedef std::set<const MbFace *>                      ConstFacesSet;
typedef ConstFacesSet::iterator                       ConstFacesSetIt;
typedef ConstFacesSet::const_iterator                 ConstFacesSetConstIt;
typedef std::pair<ConstFacesSetConstIt, bool>         ConstFacesSetRet;

typedef std::set<ConstFaceSPtr>                       ConstFacesSPtrSet;
typedef ConstFacesSPtrSet::iterator                   ConstFacesSPtrSetIt;
typedef ConstFacesSPtrSet::const_iterator             ConstFacesSPtrSetConstIt;
typedef std::pair<ConstFacesSPtrSetConstIt, bool>     ConstFacesSPtrSetRet;

typedef std::map<MbFace *, size_t>                    FaceIndexMap;
typedef std::map<const MbFace *, size_t>              ConstFaceIndexMap;
typedef std::map<size_t, MbFace *>                    IndexFaceMap;
typedef std::map<size_t, const MbFace *>              IndexConstFaceMap;

typedef std::map<FaceSPtr *, size_t>                  FaceSPtrIndexMap;
typedef std::map<ConstFaceSPtr, size_t>               ConstFaceSPtrIndexMap;
typedef std::map<size_t, FaceSPtr>                    IndexFaceSPtrMap;
typedef std::map<size_t, ConstFaceSPtr>               IndexConstFaceSPtrMap;

} // namespace C3D


//------------------------------------------------------------------------------
/** \brief \ru Вершина.
           \en Vertex. \~
  \details \ru Вершина служит для описания стыковки рёбер и
    представляет собой точку, в которой стыкуются рёбра.
    В вершине может стыковаться любое конечное число рёбер. 
    Стыкующиеся рёбра указывают на одну и ту же общую для них вершину. 
    Для вершины известна максимальная погрешностью стыковки рёбер. 
    При неточной стыковке рёбер погрешность расположения вершины 
    равна расстоянию от точки вершины до края наиболее удалённого ребра.
           \en Vertex is used to describe connections of edges and
    it is a point where edges are connected.
    Any number of edges may be connected by one vertex. 
    Adjacent edges point to the same common vertex. 
    The maximum tolerance of edges connection is known for a vertex. 
    When the connection is not accurate, the tolerance of the vertex location 
    equals the distance between the point of the vertex and the end of the farthest edge. \~
  \ingroup Topology_Items
*/
// ---
class MATH_CLASS MbVertex : public MbTopologyItem {
protected :
  MbCartPoint3D point;     ///< \ru Точка. \en A point. 
  double        tolerance; ///< \ru Максимальное расстояние от точки до примыкающих ребер. \en Maximum distance from a point to incident edges. 

protected :
  /// \ru Конструктор копирования. \en Copy constructor. 
  MbVertex( const MbVertex &, MbRegDuplicate * );
public :
  /// \ru Конструктор по точке. \en Constructor by point. 
  MbVertex( const MbCartPoint3D & );
  /// \ru Конструктор по точке и расстоянию. \en Constructor by point and distance. 
  MbVertex( const MbCartPoint3D &, double s );
  /// \ru Конструктор-копия. \en Copy constructor. 
  MbVertex( const MbVertex & );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbVertex();

public :
  VISITING_CLASS( MbVertex ); 

  // \ru Функции топологического объекта. \en Functions of topological object 

  MbeTopologyType IsA() const override; // \ru Тип элемента. \en A type of element.
  /// \ru Создать новую вершину копированием всех данных исходной вершины. \en Create new vertex by copying all data of the initial vertex. 
  virtual MbVertex *  DataDuplicate( MbRegDuplicate * = nullptr ) const;
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать согласно матрице. \en Transform according to the matrix.
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;   // \ru Сдвинуть вдоль вектора. \en Move along a vector.
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate around an axis.
  double      DistanceToPoint( const MbCartPoint3D & ) const override; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  void        AddYourGabaritTo( MbCube & ) const override; // \ru Добавить свой габарит в присланный габарит. \en Add your own bounding box into the sent bounding box. 
  void        CalculateLocalGabarit( const MbMatrix3D & into, MbCube & cube ) const override; // \ru Рассчитать габарит в локальной системы координат. \en Calculate bounding box in the local coordinate system. 
  /// \ru Являются ли объекты равными? \en Determine whether objects are equal. 
  bool        IsSame( const MbTopologyItem & other, double accuracy ) const override;
  /// \ru Построить полигональную копию объекта mesh. \en Construct a polygonal copy of an object mesh). 
  void        CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; 

          /// \ru Выдать декартову точку вершины. \en Get the Cartesian point of a vertex. 
          const MbCartPoint3D & GetCartPoint() const { return point; }
          /// \ru Выдать декартову точку вершины. \en Get the Cartesian point of a vertex. 
                MbCartPoint3D & SetCartPoint()       { return point; }

          /// \ru Выдать декартову точку вершины. \en Get the Cartesian point of a vertex. 
          void        GetCartPoint( MbCartPoint3D & cp ) const { cp = point; } 
          /// \ru Установить декартову точку вершины. \en Set the Cartesian point of a vertex. 
          void        SetCartPoint( const MbCartPoint3D & cp ) { point = cp; SetOwnChanged( tct_Replaced ); }
          /// \ru Вычислить ближайшее расстояние до ребра. \en Calculate the nearest distance to an edge. 
          double      DistanceToEdge( const MbCurveEdge & edge, MbCartPoint3D & p0, MbCartPoint3D & p1 ) const;
          /// \ru Вычислить ближайшее расстояние до грани. \en Calculate the nearest distance to a face. 
          double      DistanceToFace( const MbFace & face, MbCartPoint3D & p0, MbCartPoint3D & p1 ) const;

          /// \ru Получить погрешность стыковки рёбер. \en Get a tolerance of edges connection. 
          double      GetTolerance() const { return std_max( METRIC_REGION, tolerance ); } 
          /// \ru Установить погрешность стыковки рёбер. \en Set a tolerance of edges connection. 
          void        SetTolerance( double a ) { tolerance = a; } 
          /// \ru Установить погрешность стыковки рёбер. \en Set a tolerance of edges connection. 
          double &    SetTolerance() { return tolerance; }        
          
          /// \ru Выдать свойства объекта. \en Get properties of the object. 
          void        GetProperties( MbProperties & ); 
          /// \ru Установить свойства объекта. \en Set properties of the object. 
          void        SetProperties( const MbProperties & ); 

private:
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
          void        operator = ( const MbVertex & ); 
  
DECLARE_PERSISTENT_CLASS_NEW_DEL( MbVertex )
};

IMPL_PERSISTENT_OPS( MbVertex )


//------------------------------------------------------------------------------
/** \brief \ru Ребро.
           \en Edge. \~
  \details \ru Ребро представляет собой кривую, которой приписано направление. \n
    Направление кривой MbCurve3D жёстко связано с направлением возрастания её параметра. 
    В отличие от кривой ребро может быть направлено как в сторону возрастания параметра, 
    так и в сторону уменьшения параметра кривой. 
    Ребро всегда начинается и оканчивается в некоторой вершине MbVertex. \n
           \en An edge is a curve with direction. \n
    Direction of a curve MbCurve3D is rigidly connected with direction of increasing of its parameter. 
    In contrast to a curve an edge may be directed either in the direction of increase of parameter 
    or in the direction of decrease of parameter. 
    An edge always starts and ends at some vertex MbVertex. \n \~
  \ingroup Topology_Items
*/
// ---
class MATH_CLASS MbEdge : public MbTopologyItem {
protected :
  MbCurve3D * curve;     ///< \ru Кривая, по которой проходит ребро (всегда не nullptr). \en A curve, an edge passes by (it is always not nullptr).
  bool        sameSense; ///< \ru Признак совпадения направления ребра с направлением кривой. \en An attribute of coincidence between direction of curve and direction of edge. 
  MbVertex  * begVertex; ///< \ru Вершина-начало (всегда не nullptr). \en Start vertex (always not nullptr).
  MbVertex  * endVertex; ///< \ru Вершина-конец  (всегда не nullptr). \en End vertex (always not nullptr).

protected :
  /// \ru Конструктор копирования. \en Copy constructor. 
  MbEdge( const MbEdge & init, MbRegDuplicate * iReg );

public :
  /// \ru Конструктор по кривой, ее ориентации и вершинам. \en Constructor by curve, its orientation and vertices. 
  MbEdge( const MbCurve3D & initCurve, bool sense, const MbVertex & beg, const MbVertex & end );

  /** \brief \ru Конструктор по кривой, ее ориентации и вершинам.
             \en Constructor by curve, its orientation and vertices. \~
    \details \ru Конструктор ребра по кривой, ее ориентации и вершинам.
             Проводится проверка существования и правильности положения точек-вершин ребра. \n
             \en Constructor of edge by curve, its orientation and vertices.
             There is performed a check of existence and correctness of location of edge vertices points. \n \~
  */
  MbEdge( const MbCurve3D & initCurve, bool sense, const MbVertex * beg, const MbVertex * end );

  /** \brief \ru Конструктор по кривой и ее ориентации.
             \en Constructor by curve and its orientation. \~
    \details \ru Конструктор ребра по кривой и ее ориентации.
             Вершины формируются на основе граничных точек кривой. \n
             \en Constructor of edge by curve and its orientation.
             Vertices are constructed by boundary points of curve. \n \~
  */
  MbEdge( const MbCurve3D & initCurve, bool sense );

  /// \ru Конструктор копирования с использованием другой кривой. \en Copy constructor using other curve 
  MbEdge( const MbEdge & other, const MbCurve3D & newCurve );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbEdge();

public :
  VISITING_CLASS( MbEdge ); 

  // \ru Функции топологического объекта \en Functions of topological object 

  MbeTopologyType IsA() const override; // \ru Тип элемента. \en A type of element.
  /// \ru Создать новое ребро копированием всех данных исходного ребра. \en Create new edge by copying all data of the initial edge. 
  virtual MbEdge *    DataDuplicate( MbRegDuplicate * = nullptr ) const;
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Трансформация. \en Transformation.
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;   // \ru Перемещение. \en Moving.
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Вращение. \en Rotation.
  double      DistanceToPoint( const MbCartPoint3D & ) const override; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  void        AddYourGabaritTo( MbCube & ) const override; // \ru Добавить свой габарит в присланный габарит. \en Add your own bounding box into the sent bounding box. 
  void        CalculateLocalGabarit( const MbMatrix3D & into, MbCube & cube ) const override; // \ru Рассчитать габарит в локальной системы координат. \en Calculate bounding box in the local coordinate system. 

  /// \ru Установить флаг изменения в положение измененного объекта. \en Set the flag that the object has been changed. 
  virtual void        SetOwnChangedThrough( MbeChangedType );
  /// \ru Изменить направление ребра на противоположной, не изменяя кривую. \en Change direction of edge without changing a curve. 
  virtual void        Reverse(); 
  /// \ru Являются ли объекты равными? \en Determine whether objects are equal. 
  bool        IsSame( const MbTopologyItem & other, double accuracy ) const override;
  /// \ru Построить полигональную копию объекта mesh. \en Construct a polygonal copy of an object mesh). 
  void        CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; 

  /// \ru Замена кривой ребра на кривую crv. \en Replacement of a curve by the curve 'crv'. 
  virtual bool        ChangeCurve( MbCurve3D & crv );
  /** \brief \ru Объединение двух стыкующихся ребер.
             \en Merging of two connected edges. \~
    \details \ru Объединение двух стыкующихся ребер. \n
             \en Merging of two connected edges. \n \~
    \param[in,out] edge2 - \ru Присоединяемое ребро.
                           \en Merging edge. \~
    \param[in] begin1 - \ru К началу (true) или к концу (false) ребра this стыкуется присоединяемое ребро.
                        \en This edge is joined by the beginning (true) or by the ending (false). \~
    \param[in] begin2 - \ru Началом (true) или концом (false) стыкуется присоединяемое ребро к ребру this.
                        \en The edge2 is joined by the beginning (true) or by the ending (false). \~
    \param[in] snMaker - \ru Именователь с версией операции.
                         \en Names maker with a version of operation. \~
    \param[in] precision - \ru Точность стыковки ребер.
                           \en Accuracy of edges joining. \~
    \return \ru Возвращает поглощенное ребро edge2, которое можно удалять.
            \en Returns absorbed edge (edge2), which can be removed. \~
  */
  virtual MbEdge *    MergeEdges( MbEdge & edge2, bool begin1, bool begin2, const MbSNameMaker & snMaker, double precision );

          /// \ru Выдать кривую, по которой проходит ребро. \en Get a curve where an edge passes. 
  const   MbCurve3D & GetCurve() const { return *curve; } 
          /// \ru Выдать кривую, по которой проходит ребро. \en Get a curve where an edge passes. 
          MbCurve3D & SetCurve()       { return *curve; } 
          /// \ru Выдать направление по отношению к кривой. \en Get the direction relative to the curve. 
          bool        IsSameSense() const { return sameSense;  } 
          /// \ru Установить направление по отношению к кривой. \en Set the direction relative to the curve. 
          void        SetSameSense( bool s ); 

          /// \ru Выдать вершину-начало. \en Get the start vertex. 
  const   MbVertex  * GetBegVertexPointer() const { return begVertex; } 
          /// \ru Выдать вершину-конец. \en Get the end vertex. 
  const   MbVertex  * GetEndVertexPointer() const { return endVertex; } 
          /// \ru Выдать вершину-начало. \en Get the start vertex. 
          MbVertex  * SetBegVertexPointer() { return begVertex; }
          /// \ru Выдать вершину-конец. \en Get the end vertex. 
          MbVertex  * SetEndVertexPointer() { return endVertex; }

          /// \ru Выдать вершину-начало. \en Get the start vertex. 
  const   MbVertex  & GetBegVertex() const { return *begVertex; } 
          /// \ru Выдать вершину-конец. \en Get the end vertex. 
  const   MbVertex  & GetEndVertex() const { return *endVertex; } 
          /// \ru Выдать вершину по номеру (0 - вершина-начало, 1 - вершина-конец). \en Get vertex by number (0 - start vertex, 1 - end vertex). 
  const   MbVertex  & GetVertex( size_t i ) const { return i ? *endVertex : *begVertex; }
          /// \ru Выдать вершину-начало. \en Get the start vertex. 
          MbVertex  & SetBegVertex() { return *begVertex; } 
          /// \ru Выдать вершину-конец. \en Get the end vertex. 
          MbVertex  & SetEndVertex() { return *endVertex; } 
          /// \ru Выдать вершину по номеру (0 - вершина-начало, 1 - вершина-конец). \en Get vertex by number (0 - start vertex, 1 - end vertex). 
          MbVertex  & SetVertex( size_t i ) { return i ? *endVertex : *begVertex; }
          /// \ru Установить вершину-начало. \en Set the start vertex. 
          void        SetBegVertex( const MbVertex & ver ); 
          /// \ru Установить вершину-конец. \en Set the end vertex. 
          void        SetEndVertex( const MbVertex & ver ); 

          /// \ru Выдать вершину, соответствующую начальной точке кривой. \en Get a vertex corresponding to the start point of a curve. 
  const   MbVertex  & GetTMinVertex() const { return sameSense ? *begVertex : *endVertex; }
          /// \ru Выдать вершину, соответствующую конечной точке кривой. \en Get a vertex corresponding to the end point of a curve. 
  const   MbVertex  & GetTMaxVertex() const { return sameSense ? *endVertex : *begVertex; }
          /// \ru Выдать вершину, соответствующую начальной точке кривой. \en Get a vertex corresponding to the start point of a curve. 
          MbVertex  & SetTMinVertex() { return sameSense ? *begVertex : *endVertex; }
         /// \ru Выдать вершину, соответствующую конечной точке кривой. \en Get a vertex corresponding to the end point of a curve. 
          MbVertex  & SetTMaxVertex() { return sameSense ? *endVertex : *begVertex; }
          /// \ru Установить вершину, соответствующую начальной точке кривой. \en Set a vertex corresponding to the start point of a curve. 
          void        SetTMinVertex( const MbVertex & ver ); 
          /// \ru Установить вершину, соответствующую конечной точке кривой. \en Set a vertex corresponding to the end point of a curve. 
          void        SetTMaxVertex( const MbVertex & ver ); 

          /// \ru Выдать параметр кривой, соответствующий начальной вершине. \en Get curve parameter corresponding to the start vertex. 
          double      GetTBegVertex() const; 
          /// \ru Выдать параметр кривой, соответствующий конечной вершине. \en Get curve parameter corresponding to the end vertex. 
          double      GetTEndVertex() const; 

          /// \ru Является ли ребро прямолинейным? \en Is an edge rectilinear? 
          bool        IsStraight( bool ignoreParams = false ) const; 
          /// \ru Выдать декартову точку начальной вершины. \en Get Cartesian point of start vertex. 
          void        GetBegVertexPoint( MbCartPoint3D & cp ) const { begVertex->GetCartPoint(cp); }
          /// \ru Выдать декартову точку конечной вершины. \en Get Cartesian point of end vertex. 
          void        GetEndVertexPoint( MbCartPoint3D & cp ) const { endVertex->GetCartPoint(cp); }
          /// \ru Параллельно ли ребро плейсменту? \en Is an edge parallel to the placement? 
          bool        IsColinear( const MbPlacement3D & p, double epsilon = Math::angleRegion ) const; 
          /// \ru Является ли ребро циклически замкнутым? \en Is an edge cyclic closed? 
          bool        IsClosed() const;

          /// \ru Установить метку себе и вершинам. \en Set a label for self and vertices. 
          void        SetLabelThrough( MbeLabelState l, void * key = nullptr ) const;
          /// \ru Установить метку себе и вершинам. \en Set a label for self and vertices. 
          void        SetLabelThrough( MbeLabelState l, void * key, bool setLock ) const;
          /// \ru Удалить частную метку себе и вершинам. \en Remove private label for self and vertices. 
          void        RemovePrivateLabelThrough( void * key ) const;
          /// \ru Проверка того, что вершина принадлежит ребру. \en Check that vertices belong to an edge. 
          bool        IsVertexOn( const MbVertex * vertex ) const { return vertex == begVertex || vertex == endVertex; }
          /// \ru Нахождение общей вершины ребер. \en A search of common vertex between edges. 
  const   MbVertex  * IsConnectedWith( const MbEdge & edge ) const;

          /// \ru Добавить вершины ребра в множество вершин (если их там нет). \en Add vertices in a set of vertices (if they do not exist). 
          void        GetVerticesArray ( RPArray<MbVertex>       & );
          /// \ru Добавить вершины ребра в множество вершин (если их там нет). \en Add vertices in a set of vertices (if they do not exist). 
          void        GetVerticesArray ( RPArray<const MbVertex> & ) const;

          /// \ru Дать параметр для кривой по параметру ребра (0 <= w <= 1). \en Get parameter on a curve by the parameter on an edge (0 <= w <= 1). 
          double      GetCurveParam( double w ) const; 
          /// \ru Дать параметр от 0(начало) до 1(конец) для ребра по параметру кривой. \en Get parameter from 0 (start) to 1 (end) for an edge by the parameter of a curve. 
          double      GetEdgeParam( double t ) const;
          /// \ru Вычислить точка на ребре (0 <= t <= 1). \en Calculate point on the edge (0 <= t <= 1). 
          void        Point( double t, MbCartPoint3D & ) const; 
          /// \ru Получить точку в начальной вершине. \en Get point at start vertex. 
          void        GetBegPoint( MbCartPoint3D & p ) const { Point( 0, p ); }
          /// \ru Получить точку в конечной вершине. \en Get point at end vertex. 
          void        GetEndPoint( MbCartPoint3D & p ) const { Point( 1, p ); }
          /// \ru Выдать касательный вектор к ребру ( 0 <= t <= 1 ). \en Get tangent vector to the edge (0 <= t <= 1). 
          void        Tangent( double t, MbVector3D & ) const;  
          /// \ru Выдать касательный вектор в начальной вершине. \en Get tangent vector at start vertex. 
          void        GetBegTangent( MbVector3D & v ) const { Tangent( 0, v ); }
          /// \ru Выдать касательный вектор в конечной вершине. \en Get the tangent vector at the end vertex. 
          void        GetEndTangent( MbVector3D & v ) const { Tangent( 1, v ); }
          /// \ru Вычислить производную в середине ребра. \en Calculate derivative in the middle of an edge. 
          void        GetMiddleDerive( MbVector3D & ) const;

          /// \ru Выдать метрическую длину ребра. \en Get the metric length of an edge. 
          double      GetMetricLength() const;
          /// \ru Выдать оценку метрической длины ребра. \en Get the estimate of metric length of an edge. 
          double      GetLengthEvaluation() const;
          /// \ru Вычислить ближайшее расстояние до ребра. \en Calculate the nearest distance to an edge. 
          double      DistanceToEdge( const MbEdge & edge, MbCartPoint3D & p0, MbCartPoint3D & p1 ) const;
          /// \ru Вычислить ближайшее расстояние до грани. \en Calculate the nearest distance to a face. 
          double      DistanceToFace( const MbFace & face, MbCartPoint3D & p0, MbCartPoint3D & p1 ) const;
          /// \ru Вычислить ближайшее расстояние до поверхности. \en Calculate the nearest distance to a surface. 
          double      DistanceToSurface( const MbSurface & surf, MbCartPoint3D & p0, MbCartPoint3D & p1 ) const;
          /// \ru Вычислить проекцию точки на ребро. \en Calculate the point projection on the edge. 
          double      PointProjection( const MbCartPoint3D & ) const; 
          /// \ru Вычислить проекцию точки на продолжение прямого ребра. \en Calculate the point projection on extension of a straight edge. 
          bool        PointProjection( const MbCartPoint3D & p0, MbCartPoint3D & pOnEdge, double & distance ) const;
          /// \ru Вычислить угол между прямыми ребрами. \en Calculate an angle between straight edges. 
          bool        AngleWithEdge( const MbEdge &, double & angle ) const;
          /// \ru Создать проекцию ребра на плоскость. \en Create projection of an edge to the plane. 
          MbCurve *   GetProjection( const MbPlacement3D &, VERSION version ) const;

          /// \ru Выдать свойства объекта. \en Get properties of the object. 
          void        GetProperties( MbProperties & ); 
          /// \ru Установить свойства объекта. \en Set properties of the object. 
          void        SetProperties( const MbProperties & ); 

OBVIOUS_PRIVATE_COPY( MbEdge )
DECLARE_PERSISTENT_CLASS_NEW_DEL( MbEdge )
};

IMPL_PERSISTENT_OPS( MbEdge )


//------------------------------------------------------------------------------
/** \brief \ru Ребро грани.
           \en Face edge. \~
  \details \ru Ребро грани представляет собой кривую пересечения поверхностей 
    MbSurfaceIntersectionCurve, которой приписано направление. 
    В отличие от ребра MbEdge ребро MbCurveEdge описывает не просто кривую, 
    а гладкий участок стыковки двух граней или гладкий участок края грани. \n
    Ребро грани служит для описания участка стыковки двух граней или для описания участка края грани. \n
      Если ребро описывает участок стыковки двух граней, то указатели на грань слева и грань справа ненулевые. \n
    Ребро, описывающее участок стыковки циклически замкнутой грани самой с собой, называется швом. \n
    В последнем случае указатели на грань слева и грань справа одинаковые. \n
      Если ребро описывает участок края грани, то указатель на грань слева или грань справа равен нулю. \n
    Ребро, описывающее участок края грани, стянутый в точку, называется полюсным ребром. \n
    Если ребро описывает участок края грани, то кривая пересечения поверхностей также является граничной,
    то есть состоит из двух одинаковых кривых на поверхности. \n
      Ребро начинается и заканчивается в вершинах MbVertex.
    Если кривая ребра циклически замкнута, то ребро начинается и заканчивается в одной и той же вершине. \n
           \en A face edge is a curve of surfaces intersection 
    MbSurfaceIntersectionCurve with direction. 
    In contrast to an edge MbEdge an edge MbCurveEdge describes not just a curve, 
    but a smooth piece of connection between two faces or a smooth piece of face boundary. \n
    A face edge is used to describe a piece of connection between two faces or a piece of face boundary. \n
      If an edge describes a piece of connection between two faces then the both pointers to faces are not null. \n
    An edge describing a piece of connection of a cyclic closed faces with itself is called seam. \n
    In the last case pointers to the left face and to the right face are the same. \n
      If an edge describes a piece of face boundary then the pointer to the left face or the pointer to the right face equals null. \n
    An edge describing a contracted to a point piece of face boundary is called a pole edge. \n
    If an edge describes a piece of face boundary then a curve of surfaces intersection is boundary too,
    i.e. it consists of two equal curves on surface. \n
      An edge starts and ends in vertices MbVertex.
    If a curve of an edge is cyclic closed, then an edge starts and ends at the same vertex. \n \~
  \ingroup Topology_Items
*/
// ---
class MATH_CLASS MbCurveEdge : public MbEdge {
protected :
  MbFace    * facePlus;  ///< \ru Грань слева,  в которой направление ребра    совпадает с направлением цикла. \en A face on the left, where the direction of edge coincides with the direction of loop. 
  MbFace    * faceMinus; ///< \ru Грань справа, в которой направление ребра не совпадает с направлением цикла. \en A face on the right, where the direction of edge does not coincide with the direction of loop. 

protected :
  /// \ru Конструктор копирования. \en Copy constructor. 
  MbCurveEdge( const MbCurveEdge &, MbRegDuplicate * iReg );

public :
  /// \ru Конструктор по вершинам, кривой пересечения и ее ориентации в ребре. \en Constructor by vertices, intersection curve and its orientation on edge. 
  MbCurveEdge( const MbVertex & beg, const MbVertex & end, const MbSurfaceIntersectionCurve & initCurve, bool sense );

  /** \brief \ru Конструктор по вершинам, кривой пересечения и ее ориентации в ребре.
             \en Constructor by vertices, intersection curve and its orientation on edge. \~
    \details \ru Конструктор ребра по вершинам, кривой пересечения и ее ориентации в ребре.
             Проводится проверка существования и правильности положения точек-вершин ребра. \n
             \en Constructor of edge by vertices, intersection curve and its orientation on edge.
             There is performed a check of existence and correctness of location of edge vertices points. \n \~
  */
  MbCurveEdge( const MbVertex * beg, const MbVertex * end, const MbSurfaceIntersectionCurve & initCurve, bool sense );

  /** \brief \ru Конструктор по кривой пересечения.
             \en Constructor by intersection curve. \~
    \details \ru Конструктор ребра по кривой пересечения.
             Вершины формируются на основе граничных точек кривой. \n
             \en Constructor of edge by intersection curve.
             Vertices are constructed by boundary points of curve. \n \~
  */
  MbCurveEdge( const MbSurfaceIntersectionCurve &, bool sense );

  /// \ru Конструктор копирования с использованием другой кривой. \en Copy constructor using other curve 
  MbCurveEdge( const MbCurveEdge & other, const MbSurfaceIntersectionCurve & newCurve );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbCurveEdge();

public :
  VISITING_CLASS( MbCurveEdge ); 

  // \ru Функции топологического объекта \en Functions of topological object 

  MbeTopologyType  IsA() const override; // \ru Тип элемента. \en A type of element.
  /// \ru Создать новое ребро копированием всех данных исходного ребра. \en Create new edge by copying all data of the initial edge. 
  MbCurveEdge *    DataDuplicate( MbRegDuplicate * = nullptr ) const override;
  void        SetOwnChangedThrough( MbeChangedType ) override; // \ru Установить флаг изменения в положение измененного объекта. \en Set the flag that the object has been changed.
  void        Reverse() override; // \ru Изменить направление ребра на противоположной, не изменяя кривую. \en Change direction of edge without changing a curve.
  /// \ru Являются ли объекты равными? \en Determine whether objects are equal. 
  bool        IsSame( const MbTopologyItem &, double accuracy ) const override;

  /// \ru Замена кривой ребра на кривую crv. \en Replacement of a curve by the curve 'crv'. 
  bool        ChangeCurve( MbCurve3D & crv ) override;
  /// \ru Замена кривой ребра на кривую crv. \en Replacement of a curve by the curve 'crv'. 
  void        ChangeIntersectionCurve( MbSurfaceIntersectionCurve & crv, bool sense );

  /// \ru Выдать кривую пересечения поверхностей. \en Get surfaces intersection curve. 
  const   MbSurfaceIntersectionCurve & GetIntersectionCurve() const { return (const MbSurfaceIntersectionCurve &)*curve; } 
  /// \ru Выдать кривую пересечения поверхностей для модификации. \en Get surfaces intersection curve for modification. 
          MbSurfaceIntersectionCurve & SetIntersectionCurve()       { return (      MbSurfaceIntersectionCurve &)*curve; } 
          /// \ru Дать пространственную копию кривой пересечения поверхностей. \en Get a spatial copy of surfaces intersection curve. 
  const   MbCurve3D * GetSpaceCurve() const; 
          /// \ru Построить пространственную копию кривой пересечение поверхностей. \en Construct a spatial copy of surfaces intersection curve. 
          MbCurve3D * MakeCurve() const;

          /// \ru Выдать грань слева. \en Get a face on the left. 
          MbFace    * GetFacePlus () const { return facePlus;   } 
          /// \ru Выдать грань справа. \en Get a face on the right. 
          MbFace    * GetFaceMinus() const { return faceMinus;  } 
          /// \ru Выдать грань по индексу (0 - справа от ребра, 1 - слева от ребра. \en Get a face by index (0 - on the right from an edge, 1 - on the left from an edge). 
          MbFace    * GetFace( size_t i ) const { return i ? facePlus : faceMinus; }
          /// \ru Установить грань слева. \en Set a face on the left. 
          void        SetFacePlus ( MbFace * f ) { facePlus  = f; } 
          /// \ru Установить грань справа. \en Set a face on the right. 
          void        SetFaceMinus( MbFace * f ) { faceMinus = f; } 
          /// \ru Является ли ребро гладким? \en Is an edge smooth? 
          bool        IsSmooth( double epsilon = Math::paramPrecision ) const; 
          /// \ru Является ли ребро швом? \en Is an edge a seam? 
          bool        IsSeam () const; 
          /// \ru Является ли ребро разбиением грани? \en Is an edge a face splitting? 
          bool        IsSplit( bool strict = false ) const; 
          /// \ru Является ли ребро полюсным? \en Is an edge pole? 
          bool        IsPole ( VERSION version = Math::DefaultMathVersion() ) const;
          /// \ru Является ли ребро обычным ребром пересечения (толерантное по флагу)? \en Is an edge a usual edge of intersection (tolerant by the flag)? 
          bool        IsUsual( bool tolerantIsUsual ) const;

          /// \ru Установить метки ориентированных ребер. \en Set labels of oriented edges. 
          void        SetOrientedEdgesLabel( MbeLabelState, void * key = nullptr );
          /// \ru Найти ориентированное ребро. \en Find an oriented edge. 
          bool        FindOrientedEdge( bool orient, const MbFace * face, MbLoop *& findLoop, size_t & index ) const;
          /// \ru Найти ориентированное ребро. \en Find an oriented edge. 
          bool        FindOrientedEdgePlus ( size_t & loopIndex, MbLoop *& findLoop, size_t & index ) const;
          /// \ru Найти ориентированное ребро. \en Find an oriented edge. 
          bool        FindOrientedEdgeMinus( size_t & loopIndex, MbLoop *& findLoop, size_t & index ) const;
          /// \ru Удалить ориентированные ребра на данном ребре. \en Delete oriented edges of the given edge. 
          void        DeleteOrientedEdges();

          /** \brief \ru Замена поверхности.
                     \en Replacement of a surface. \~
            \details \ru Замена в кривой поверхности oldSurf на поверхность newSurf.
                     \en Replacement of the surface oldSurf to the surface newSurf in a curve. \~ 
            \param[in] oldSurf - \ru Заменяемая поверхность грани.
                                 \en Replaced surface of a face. \~
            \param[in] newSurf - \ru Заменяющая (новая) поверхность грани.
                                 \en Replacing (new) surface of a face. \~
            \param[in] faceSense - \ru Совпадение направления нормали грани и нормали поверхности.
                                   \en Coincidence between the face normal direction and the surface normal direction. \~
            \param[in] orient - \ru Ориентация ребра в цикле грани.
                                \en Orientation of an edge in the face loop. \~
          */
          void        ChangeSurface( const MbSurface & oldSurf, MbSurface & newSurf, bool faceSense, bool orient );

          /** \brief \ru Замена поверхности.
                     \en Replacement of a surface. \~
            \details \ru Замена в кривой поверхности item на поверхность init при объединении подобных граней.
                     \en Replacement of the surface 'item' to the surface 'init' when combining the similar faces. \~ 
            \param[in] item - \ru Заменяемая поверхность грани.
                              \en Replaced surface of a face. \~
            \param[in] init - \ru Заменяющая (новая) поверхность грани.
                              \en Replacing (new) surface of a face. \~
            \param[in] matr - \ru Матрица преобразования двумерных кривых кривой пересечения curve при замене поверхностей.
                              \en A transformation matrix of two-dimensional curves of intersection curve when replacing surfaces. \~
            \return \ru Выполнена ли замена и преобразование?
                    \en Are replacement and transformation performed? \~
          */
          bool        ChangeCarrierBorne( const MbSurface & item, MbSurface & init, const MbMatrix & matr );
          /// \ru Построение нормалей грани face и векторов от ребра в обе стороны. \en Construction of normals of a face and vectors from an edge to both sides. 
          bool        GetTraverses( const MbFace * face, const MbFace * other, bool plus, double t,
                                    double paramStep, double metricStep, MbCartPoint & p0, MbTwoTraverses & traverses,
                                    VERSION version = Math::DefaultMathVersion() ) const;
          /// \ru Построить вектор от ребра вне/внутрь грани (out==true/false). \en Construct a vector from edge to the outside/inside of a face(out==true/false). 
          bool        GetOutTraverse( const MbFace & face, bool plus, double t, double metricStep, MbCartPoint3D & q0, MbVector3D & outv,
                                      MbCartPoint & p0, MbVector & tv, bool & out ) const;
          /// \ru Построить перпендикуляр к ребру (0 <= t <= 1) внутрь грани facePlus/faceMinus (plus==true/false). \en Construct perpendicular to an edge (0 <= t <= 1) to the inside of a facePlus/faceMinus (plus==true/false). 
          bool        Transversal( double t, MbVector3D & f, bool plus ) const;  
          /// \ru Построить перпендикуляр к ребру (0 <= t <= 1) внутрь грани facePlus/faceMinus (plus==true/false). \en Construct perpendicular to an edge (0 <= t <= 1) to the inside of a facePlus/faceMinus (plus==true/false).  
          bool        Transversal( double t, MbVector & f, MbCartPoint & p, bool plus ) const;
          /// \ru Построить перпендикуляр к ребру (0 <= t <= 1) внутрь грани facePlus/faceMinus (plus==true/false). \en Construct perpendicular to an edge (0 <= t <= 1) to the inside of a facePlus/faceMinus (plus==true/false).  
          bool        TransversalReper( double t, MbPlacement3D & pl, bool plus ) const;
          /// \ru Вычислить на ребре (0 <= t <= 1) двумерную точку p в области параметров поверхности surf. \en Calculate on an edge (0 <= t <= 1) a two-dimensional point p in the region of parameters of the surface surf. 
          bool        PointBy( const MbSurface & surf, bool faceSense, bool orient, double t, MbCartPoint & p ) const; 
          /// \ru Вычислить на ребре двумерную точку p в области параметров поверхности surf по значению параметра t кривой ребра. \en Calculate two-dimensional point p on the edge in the region of parameters of the surface surf by the value of parameter t on the edge curve. 
          bool        PointOn( const MbSurface & surf, bool faceSense, bool orient, double & t, MbCartPoint & p ) const;
          /// \ru Вычислить на ребре двумерную точку p в области параметров поверхности грани face по значению параметра t кривой ребра. \en Calculate two-dimensional point p on the edge in the region of parameters of the ace 'face' by the value of parameter t on the edge curve. 
          bool        PointOn( const MbFace * face, bool orient, double & t, MbCartPoint & p ) const;
          /// \ru Нормаль к грани facePlus или faceMinus на ребре (0 <= t <= 1). \en A normal to the face facePlus or faceMinus on the edge (0 <= t <= 1). 
          bool        FaceNormal( double t, MbVector3D & n, bool plus ) const;
          /// \ru Нормаль к грани поверхности surf на ребре (0 <= t <= 1) поверхности surf. \en A normal to the surface surf on the edge (0 <= t <= 1) of the surface surf. 
          bool        FaceNormal( const MbSurface & surf, bool faceSense, double t, MbVector3D & p ) const;
          /// \ru Средняя нормаль на ребре (0 <= t <= 1) наружу оболочки. \en Calculate a normal on the edge (0 <= t <= 1) outside of the shell. 
          bool        EdgeNormal( double t, MbVector3D & p ) const;
          /// \ru Вычислить среднюю нормаль в вершине begin ребра edge. \en Calculate the middle normal at the vertex 'begin' of the edge 'edge'. 
          bool        VertexNormal( bool begin, MbVector3D & normal ) const;
          /// \ru Выбор двумерной кривой на поверхности surf в направлении цикла. \en Selection of two-dimensional curve on the surface 'surf' by direction of the loop. 
          MbCurve *   ChooseCurve( const MbSurface & surf, bool faceSense, bool orient ) const;
          /// \ru Выбор двумерной кривой на поверхности surf в направлении цикла. \en Selection of two-dimensional curve on the surface 'surf' by direction of the loop. 
          MbCurve *   ChooseCurve( const MbFace * face, bool orient ) const;
          /// \ru Дать параметр t крайней точки ребра и соответствующие ему точки на поверхностях кривой пересечения. \en Get parameter t of edge boundary point and the corresponding points on surfaces of the intersection curve. 
          bool        GetLimitParam( bool beg, MbCartPoint & pPlus, MbCartPoint & pMinus, double & t ) const;
          /// \ru Вычислить угол ребра (0 <= t <= 1): для выпуклого ребра угол больше нуля, для вогнутого ребра угол меньше нуля. \en Calculate an angle of the edge (0 <= t <= 1). an angle is more than null for a convex edge and less than null for a concave edge. 
          double      FacesAngle( double t ) const; 

          /** \brief \ru Выпуклое ли ребро?
                     \en Is an edge convex? \~ 
            \details \ru Выпуклое ли ребро по среднему параметру ребра (или среднему параметру указанного диапазона)?
              Расчет верен для не меняющего выпуклость ребра.
              Для гладких рёбер возвращает ts_neutral.
                     \en Is an edge convex by its middle parameter (or middle parameter of input range)?
              The calculation is correct for edges which do not change a convexity.
              Returns ts_neutral for smooth edges. \~
          */
          ThreeStates IsConvex( double angleEps = EXTENT_EPSILON, const MbRect1D * tRange = nullptr ) const;

          /**\ru Скопировать из копии готовые метрические оценки, которые в оригинале не были рассчитаны.
             \en Copy from the copy ready estimates which were not calculated in the original. \~
              \warning \ru Внимание: для скорости проверка идентичности оригинала и копии не выполняется!
                       \en Attention: a check of identity between a copy and an original is not performed for the time saving! \~
          */
          bool        CopyReadyMutable( const MbCurveEdge & e );

          /** \brief \ru Вычислить двумерный вектор сдвига двумерной кривой.
                     \en Calculate two-dimensional vector of two-dimensional curve shift. \~
            \details \ru Вычислить двумерный вектор сдвига двумерной кривой. \n
                     \en Calculate two-dimensional vector of two-dimensional curve shift. \n \~ 
          */
          bool        GetMoveVector( const MbSurface & surf, bool faceSense, bool orient, MbVector & to ) const;

          /**\ru Где лежат кривые пересечения поверхностей (граней): \n
              curveOne на facePlus, curveTwo на faceMinus => +1 \n
              curveOne на faceMinus, curveTwo на facePlus => -1 \n
              иначе => 0
             \en Where surfaces (faces) intersection curves lie: \n
              curveOne on facePlus, curveTwo on faceMinus => +1 \n
              curveOne on faceMinus, curveTwo on facePlus => -1 \n
              otherwise => 0 \~
          */
          int         IsCurveOneOnFacePlus() const;

          /// \ru Сдвиг одной двумерной кривой. \en A shift of two-dimensional curve. 
          bool        MoveBy( const MbSurface & surf, bool faceSense, bool orient, const MbVector & to );

          /** \brief \ru Разбить ребро на два ребра по параметру его кривой.
                     \en Split an edge by the parameter of its curve. \~
            \details \ru Если beginSafe == true  - ребро сохранит начальный участок, 
              разбиваемое ребро сохранит начальную вершину и будет кончаться в вершине breakVertex,
              новое ребро newEdge будет начинаться в вершине breakVertex и кончаться в бывшей конечной вершине разбиваемого ребра
              Если beginSafe == false - ребро сохранит конечный  участок, 
              разбиваемое ребро сохранит конечную вершину и будет начинаться в вершине breakVertex,
              новое ребро newEdge будет начинаться в начальной вершине разбиваемого ребра и кончаться в вершине breakVertex \n
              Параметр 'surface' необходим только для толерантной кривой пересечения.
              Новое ребро встраиваются в циклы смежных граней. Вернет новое ребро.
                     \en If beginSafe == true then the edge saves its starting piece, 
              the spit edge will save its start vertex and will end at the vertex breakVertex,
              the new edge newEdge will start at the vertex breakVertex and will end at the ex-end vertex of the spit edge
              If beginSafe == false then the edge saves its ending piece, 
              the spit edge will save its end vertex and will start at the vertex breakVertex,
              the new edge newEdge will start at the start vertex of the spit edge and will end at the vertex breakVertex.\n
              The parameter 'surface' plays a role only for tolerant edge.
              The new edge will be embedded in the loops of adjacent faces.
            \param[in] t   - \ru Параметр кривой пересечения для разбиения ребра,
                              \en Parameter of intersection curve of edge to split, \~
            \param[in] beginSafe - \ru Ребро сохранит начальную половину (true) или ребро сохранит конечную половину (false),
                                    \en The edge will keep a beginning piece (true) or the edge will keep an end piece (false) \~
            \param[in] surface - \ru Для толерантной кривой требуется указать поверхность, к кривой которой относится параметр.
                                  \en For tolerant curve it is required to specify a surface which contain a curve a parameter belongs to. \~
            \return \ru Возвращает отрезанную часть ребра.
                    \en Returns a cut edge. \~
          */
          MbCurveEdge * CuttingEdge( double t, bool beginSafe, const MbSurface * surface );

          /** \brief \ru Усечь ребро по параметру его кривой.
                     \en Truncate an edge by the parameter of its curve. \~
            \details \ru Если beginCutting == true  - отрезается начало ребра, 
                         Если beginCutting == false - отрезается конец ребра.
                     \en If beginCutting == true - then the starting part of an edge is cut. 
                         If beginCutting == false - then the ending part of an edge is cut. \~
            \param[in] t   - \ru Параметр кривой пересечения для усечения ребра.
                              \en Parameter of intersection curve of edge to truncate. \~
            \param[in] beginCutting - \ru Ребро сохранит начальную половину (false) или ребро сохранит конечную половину (true).
                                      \en The edge will keep a beginning piece (false) or the edge will keep an end piece (true). \~
            \param[in] surface - \ru Для толерантной кривой требуется указать поверхность, к кривой которой относится параметр.
                                  \en For tolerant curve it is required to specify a surface which contain a curve a parameter belongs to. \~
            \return \ru Возвращает true, если усечение выполнено, false - в противном случае.
                    \en Returns true if the edge was truncated, otherwise returns false. \~
          */
          bool        TruncateEdge ( double & t, bool beginCutting, const MbSurface * surface );

          /** \brief \ru Разбить ребро по параметрам его кривой на несколько его частей.
                     \en Split the edge by the curve parameters into several pieces. \~
            \details \ru . Если beginSafe == true  - ребро сохранит начальный участок, 
              Если beginSafe == false - ребро сохранит конечный  участок. 
              По параметру 'eps' отсеиваются значения в контейнере 'params', совпадающие друг с другом и с начальным и конечным параметрами кривой пересечения.
              Параметр 'surface' необходим только для толерантной кривой пересечения.
              Контейнер 'edges' содержит отрезанные части. Отрезанные части встраиваются в циклы смежных граней.
                     \en . If beginSafe == true then the edge saves its starting piece, 
              If beginSafe == false then the edge saves its ending piece. 
              According to the parameter 'eps' drop out value in the container 'params', coinciding with each other and with the initial and final parameters of the intersection curve.
              The parameter 'surface' plays a role only for tolerant edge.
              The container 'edges' contains cut parts. The cut parts will be embedded in the loops of adjacent faces. \~ 
            \param[in] params - \ru Параметры кривой пересечения для разбиения ребра,
                                \en Parameters of intersection curve of edge to split, \~
            \param[in] beginSafe - \ru Ребро сохранит начальную половину (true) или ребро сохранит конечную половину (false),
                                    \en The edge will keep a beginning piece (true) or the edge will keep an end piece (false) \~
            \param[in] eps - \ru Точность совпадения параметров разбиения,
                              \en Precision matching options of parameters to split, \~
            \param[in] surface - \ru Для толерантной кривой требуется указать поверхность грани, к кривой которой относятся параметры резки. 
                                  \en For tolerant curve it is required to specify a surface of face which contain a curve parameters belongs to. \~
            \param[out] edges - \ru Отрезанные части ребра.
                                \en The container of cut parts. \~
            \return \ru Возвращает true, если ребро было порезано.
                    \en Returns true, if the edge was cut. \~
          */
          bool        CuttingEdge( SArray<double> & params, bool beginSafe, double eps, const MbSurface * surface, 
                                   RPArray<MbCurveEdge> & edges );
          /** \brief \ru Разбить ребро по параметрам его кривой на несколько его частей.
                     \en Split the edge by the curve parameters into several pieces. \~
            \details \ru . Если beginSafe == true  - ребро сохранит начальный участок, 
              Если beginSafe == false - ребро сохранит конечный  участок. 
              По параметру 'eps' отсеиваются значения в контейнере 'params', совпадающие друг с другом и с начальным и конечным параметрами кривой пересечения.
              Параметр 'surface' необходим только для толерантной кривой пересечения.
              Контейнер 'edges' содержит отрезанные части. Отрезанные части встраиваются в циклы смежных граней.
                     \en . If beginSafe == true then the edge saves its starting piece, 
              If beginSafe == false then the edge saves its ending piece. 
              According to the parameter 'eps' drop out value in the container 'params', coinciding with each other and with the initial and final parameters of the intersection curve.
              The parameter 'surface' plays a role only for tolerant edge.
              The container 'edges' contains cut parts. The cut parts will be embedded in the loops of adjacent faces. \~ 
            \param[in] params - \ru Параметры кривой пересечения для разбиения ребра,
                                \en Parameters of intersection curve of edge to split, \~
            \param[in] beginSafe - \ru Ребро сохранит начальную половину (true) или ребро сохранит конечную половину (false),
                                    \en The edge will keep a beginning piece (true) or the edge will keep an end piece (false) \~
            \param[in] eps - \ru Точность совпадения параметров разбиения,
                              \en Precision matching options of parameters to split, \~
            \param[in] surface - \ru Для толерантной кривой требуется указать поверхность грани, к кривой которой относятся параметры резки. 
                                  \en For tolerant curve it is required to specify a surface of face which contain a curve parameters belongs to. \~
            \param[out] edges - \ru Отрезанные части ребра.
                                \en The container of cut parts. \~
            \return \ru Возвращает true, если ребро было порезано.
                    \en Returns true, if the edge was cut. \~
          */
          bool        CuttingEdge( SArray<double> & params, bool beginSafe, double eps, const MbSurface * surface, 
                                   c3d::EdgesSPtrVector & edges );

          /** \brief \ru Разбить ребро по точкам изменения выпуклости-вогнутости.
                     \en Split the edge by points where the convexity changes. \~
            \details \ru Разбить ребро по точкам изменения выпуклости-вогнутости и сложить отрезанные части в контейнер 'edges'.
                         Отрезанные части встраиваются в циклы смежных граней.
                     \en Split the edge by points where the convexity changes. 
                         The container 'edges' contains cut parts of edge. The cut parts will be embedded in the loops of adjacent faces. \~
            \param[out] edges - \ru Отрезанные части ребра.
                                \en The container of cut parts. \~
            \return \ru Возвращает true, если ребро было порезано.
            \en Returns true, if the edge was cut. \~
          */
          bool        ConvexoConcaveCutting( RPArray<MbCurveEdge> & edges, VERSION version );
          bool        ConvexoConcaveCutting( RPArray<MbCurveEdge> & edges, VERSION version, 
                                             MbFunction & function, RPArray<MbFunction> & functions );

          /** \brief \ru Продолжить ребро.
                     \en Prolong an edge. \~
            \details \ru Продолжить кривую пересечения ребра до параметра t, лежащего за пределами области определения. \n
                     \en Continue the intersection curve of edge by the parameter t, lying outside of the curve. \n \~
            \param[in,out] t - \ru Параметра на продолжении кривой ребра.
                                \en Parameter outside of the intersection curve. \~
            \param[in] begin - \ru Начало (true) или конец (false) ребра продолжить.
                                \en The edge should be prolonged by the beginning (true) or by the ending (false). \~
            \param[in] deviateAngle - \ru Угловое отклонение для шага при движении вдоль кривой в общем случае.
                                      \en The angular deviation step of the motion along the curve in the general case. \~
            \param[in] version - \ru Версия операции.
                                  \en Version of operation. \~
            \param[in] eps - \ru Точность построений.
                           - \en Build precision. \~
            \return \ru Возвращает true, если продление выполнено, false - в противном случае.
                    \en Returns true if the edge was prolonged, otherwise returns false. \~
          */
          bool        ProlongEdge ( double & t, bool begin, double deviateAngle, const VERSION version, double eps = Math::paramNear );
          // \ru Объединение двух гладко-стыкующихся ребер. \en Union of two smoothly joining edges. 
          MbEdge * MergeEdges( MbEdge & edge2, bool begin1, bool begin2, const MbSNameMaker & snMaker, double precision ) override;
          /** \brief \ru Объединение двух стыкующихся ребер.
                     \en Merging of two connected edges. \~
            \details \ru Объединение двух стыкующихся ребер: \n
              перед вызовом на ребра надо сделать AddRef, т.к. одно из них может быть удалено,
              а после вызова и использования ребер на них надо сделать Release. \n
                     \en Merging of two connected edges: \n
              Before the call AddRef should be done on the edges, since one of the edges may be deleted,
              and after the call and using the edges Release should be done on them. \n \~
            \param[in,out] edge2 - \ru Присоединяемое ребро.
                                    \en Merging edge. \~
            \param[in] begin1 - \ru К началу (true) или к концу (false) ребра this стыкуется присоединяемое ребро.
                                \en This edge is joined by the beginning (true) or by the ending (false). \~
            \param[in] begin2 - \ru Началом (true) или концом (false) стыкуется присоединяемое ребро к ребру this.
                                \en The edge2 is joined by the beginning (true) or by the ending (false). \~
            \param[in] snMaker - \ru Именователь с версией операции.
                                 \en Names maker with a version of operation. \~
            \param[in] precision - \ru Точность стыковки ребер.
                                   \en Accuracy of edges joining. \~
            \return \ru Возвращает поглощенное ребро edge2, которое можно удалять.
                    \en Returns absorbed edge (edge2), which can be removed. \~
          */
          MbCurveEdge * MergeEdges( MbCurveEdge & edge2, bool begin1, bool begin2, 
                                    const MbSNameMaker & snMaker, double precision = Math::paramNear );

          /// \ru Собрать все ребра, стыкующиеся с заданным ребром в его начале begin==true (конце begin==false). \en Collect all edges which are connected with the given edge at its start vertex (begin==true) or at its end vertex (begin==false). 
          void        GetConnectedEdges( bool begin, RPArray<MbCurveEdge> & edges, SArray<bool> & orients, VERSION version = Math::DefaultMathVersion() ) const;
          /// \ru Собрать все ребра, являющиеся продолжением данного ребра, и имеющие те же грани слева и справа. \en Collect all edges that are a continuation of this edge, and have the same faces on the left and right.  
          bool        GetProlongEdges( RPArray<MbCurveEdge> & edges ) const;

          /// \ru Выдать свойства объекта. \en Get properties of the object. 
          void        GetProperties( MbProperties & ); 
          /// \ru Установить свойства объекта. \en Set properties of the object. 
          void        SetProperties( const MbProperties & ); 

          /// \ru Является ли ребро граничным, по которому происходит разрыв оболочки? \en Is an edge a boundary edge where occurs a discontinuity of the shell? 
          bool        IsBoundaryFace( double mEps = Math::metricEpsilon ) const; 
          /// \ru Получить тип кривой по построению. \en Get a type of a curve by the construction. 
          MbeCurveBuildType GetBuildType() const; 

          /// \ru Вычислить и выдать толерантность кривой ребра. \en Calculate and get the tolerance of the edge curve. 
          double      GetTolerance() const;
          /// \ru Изменить точность построения кривой пересечения. \en Change the tolerance of construction of intersection curve. 
          void        SetTolerance( double eps ); 
          /// \ru Является ли кривая толерантной? \en Is a curve tolerant? 
          bool        IsTolerant() const;

          /// \ru Сделать ребро граничным (изменить кривую ребра). \en Make the edge boundary (change its curve). 
          bool        MakeBoundaryCurve();

          /// \ru Пересечение ребра с плоскостью, результат - множество параметров на плейсменте или множество двумерных кривых на плейсменте. \en An intersection between an edge and a plane, in result a set of parameters on the placement or a set of two-dimensional curves on the placement. 
          void        CurveSection( const MbPlacement3D & place, SArray<MbCartPoint> & points, RPArray<MbCurve> & pCurve ) const; 
          /// \ru Получить параметры разрезки для периодического ребра. \en Get the cutting parameters  for a periodic edge. 
          bool        CutPeriodicEdge( const MbVector3D & eye, SSArray<double> & trimParams, 
                                       double & delT1, double & delT2 ) const;

// \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
OBVIOUS_PRIVATE_COPY( MbCurveEdge )
DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCurveEdge )
};

IMPL_PERSISTENT_OPS( MbCurveEdge )


//------------------------------------------------------------------------------
/** \brief \ru Ориентированное ребро.
           \en Oriented edge. \~
  \details \ru Ориентированное ребро описывает гладкий участок границы грани.
    Последовательность ориентированных ребер описывает границу грани и образует цикл MbLoop.
    Ориентированное ребро базируется на ребре MbCurveEdge и всегда ориентировано вдоль цикла грани. \n
    При движении вдоль ориентированного ребра c внешней стороны грани грань всегда располагается слева.
           \en An oriented edge describes a smooth piece of a face boundary.
    A sequence of oriented edges describes a boundary and forms a loop MbLoop.
    An oriented edge is based on the edge MbCurveEdge and always oriented along the loop of a face. \n
    When moving along the oriented edge from the face external side the face always lies on the left. \~
  \ingroup Topology_Items
*/
// ---
class MATH_CLASS MbOrientedEdge : public MbTopItem {
protected:
  MbCurveEdge *   curveEdge;   ///< \ru Ребро грани (всегда не nullptr). \en Face edge (always not nullptr).
  bool            orientation; ///< \ru Направление ребра грани в цикле. \en Direction of a face edge in the loop. 
  mutable MbLabel label;       ///< \ru Временная метка для выполнения операций. \en Temporary label for performing of operations. 
public :
  /// \ru Конструктор ориентированного ребра. \en Constructor of oriented edge. 
  MbOrientedEdge( const MbCurveEdge & edge, bool orient );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbOrientedEdge ();

public :
  virtual void Accept( Visitor & visitor )
  {
    VisitorImpl<MbOrientedEdge> * impl = dynamic_cast<VisitorImpl<MbOrientedEdge> *>(&visitor);
    if( impl )
      impl->Visit( *this );
  }

  // \ru Функции топологического объекта. \en Functions of topological object 

  MbeTopologyType  IsA() const override; // \ru Тип элемента. \en A type of element.

          /// \ru Инициализация по ребру и направлению. \en Initialization by edge and direction. 
          void        InitEdge( MbCurveEdge & initEdge, bool orient );

          /// \ru Выдать кривую пересечения поверхностей. \en Get surfaces intersection curve. 
          const   MbSurfaceIntersectionCurve & GetIntersectionCurve() const { return curveEdge->GetIntersectionCurve(); } 
          /// \ru Выдать кривую пересечения поверхностей для модификации. \en Get surfaces intersection curve for modification. 
          MbSurfaceIntersectionCurve & SetIntersectionCurve()       { return curveEdge->SetIntersectionCurve(); } 
          /// \ru Выдать кривую ребра. \en Get a curve of an edge. 
  const   MbCurve3D & GetCurve() const { return curveEdge->GetCurve(); } 
          /// \ru Выдать кривую ребра для модификации. \en Get a curve of an edge for modification. 
          MbCurve3D & SetCurve()       { return curveEdge->SetCurve(); } 

          /// \ru Выдать грань, в которой лежит ребро. \en Get the face where an edge lies. 
          MbFace    * GetFacePlus()  const { return orientation ? curveEdge->GetFacePlus() : curveEdge->GetFaceMinus(); } 
          /// \ru Выдать соседнюю грань. \en Get adjacent face. 
          MbFace    * GetFaceMinus() const { return orientation ? curveEdge->GetFaceMinus() : curveEdge->GetFacePlus(); }

          /// \ru Установить грань слева. \en Set a face on the left. 
          void        SetFacePlus ( MbFace * f ) { orientation ? curveEdge->SetFacePlus( f ) : curveEdge->SetFaceMinus( f ); } 
          /// \ru Установить грань справа. \en Set a face on the right. 
          void        SetFaceMinus( MbFace * f ) { orientation ? curveEdge->SetFaceMinus( f ) : curveEdge->SetFacePlus( f ); } 

          /// \ru Выдать вершину-начало. \en Get the start vertex. 
  const   MbVertex  & GetBegVertex() const { return orientation ? curveEdge->GetBegVertex() : curveEdge->GetEndVertex(); }
          /// \ru Выдать вершину-конец. \en Get the end vertex. 
  const   MbVertex  & GetEndVertex() const { return orientation ? curveEdge->GetEndVertex() : curveEdge->GetBegVertex(); } 
          /// \ru Выдать вершину-начало. \en Get the start vertex. 
          MbVertex  & SetBegVertex() { return orientation ? curveEdge->SetBegVertex() : curveEdge->SetEndVertex(); }
          /// \ru Выдать вершину-конец. \en Get the end vertex. 
          MbVertex  & SetEndVertex() { return orientation ? curveEdge->SetEndVertex() : curveEdge->SetBegVertex(); } 
          /// \ru Установить вершину-начало. \en Set the start vertex. 
          void        SetBegVertex( const MbVertex & ver ); 
          /// \ru Установить вершину-конец. \en Set the end vertex. 
          void        SetEndVertex( const MbVertex & ver );

          /// \ru Выдать ребро грани MbCurveEdge. \en Get a face edge MbCurveEdge. 
          MbCurveEdge & GetCurveEdge() const { return *curveEdge; } 
          /// \ru Выдать ребро грани MbCurveEdge. \en Get a face edge MbCurveEdge. 
          MbCurveEdge & SetCurveEdge()       { return *curveEdge; }

          /// \ru Выдать направление по отношению к кривой. \en Get the direction relative to the curve. 
          bool        IsSameSense() const;
          /// \ru Является ли ребро прямолинейным? \en Is an edge rectilinear? 
          bool        IsStraight( bool ignoreParams = false ) const;
          /// \ru Является ли ребро швом? \en Is an edge a seam? 
          bool        IsSeam() const;
          /// \ru Параллельно ли ребро плейсменту. \en Is an edge parallel to the placement? 
          bool        IsColinear( const MbPlacement3D & ) const;
          
          /// \ru Принадлежит ли вершина ребру? \en Does a vertex belong an edge? 
          bool        IsVertexOn( const MbVertex * vertex ) const { return vertex == &GetBegVertex() || vertex == &GetEndVertex(); }
          /// \ru Выдать декартову точку начальной вершины. \en Get Cartesian point of start vertex. 
          void        GetBegVertexPoint( MbCartPoint3D & cp ) const { GetBegVertex().GetCartPoint(cp); }
          /// \ru Выдать декартову точку конечной вершины. \en Get Cartesian point of end vertex. 
          void        GetEndVertexPoint( MbCartPoint3D & cp ) const { GetEndVertex().GetCartPoint(cp); }

          /// \ru Выдать множество вершин. \en Get a set of vertices. 
          template <class VerticesVector>
          void        GetVerticesArray( VerticesVector & vertices, bool findSame = true ) const
          {
            const MbVertex * lastVertex = nullptr;
            if ( vertices.size() > 0 )
              lastVertex = static_cast<const MbVertex *>( vertices.back() );

            MbVertex * begVertex = const_cast<MbVertex *>( &GetBegVertex() );
            if ( begVertex != lastVertex ) {
              if ( !findSame || (std::find( vertices.begin(), vertices.end(), begVertex ) == vertices.end()) )
                vertices.emplace_back( begVertex );
            }
            MbVertex * endVertex = const_cast<MbVertex *>( &GetEndVertex() );
            if ( endVertex != begVertex && endVertex != lastVertex ) {
              if ( !findSame || (std::find( vertices.begin(), vertices.end(), endVertex ) == vertices.end()) )
                vertices.emplace_back( endVertex );
            }
          }

          /// \ru Выдать множество вершин. \en Get a set of vertices. 
          template <class VerticesSet>
          void        GetVerticesSet( VerticesSet & vertices ) const
          {
            MbVertex * begVertex = const_cast<MbVertex *>( &GetBegVertex() );
            vertices.emplace( begVertex );
            MbVertex * endVertex = const_cast<MbVertex *>( &GetEndVertex() );
            if ( begVertex != endVertex )
              vertices.emplace( endVertex );
          }

          /// \ru Выдать ориентацию ребра грани. \en Get orientation of face edge. 
          bool        GetOrientation() const { return orientation; } 
          /// \ru Установить ориентацию ребра грани. \en Set orientation of face edge. 
          void        SetOrientation( bool o ); 

          /// \ru Получить метку. \en Get label. 
          MbeLabelState GetLabel( void * key = nullptr ) const { return (MbeLabelState)label.GetLabel(key);}
          /// \ru Установить свою метку. \en Set label. 
          void        SetOwnLabel( MbeLabelState l, void * key = nullptr ) const { label.SetLabel( l, key ); }
          /// \ru Установить метку ориентированному ребру, ребру грани и вершинам ребра. \en Set label for oriented edge, face edge and vertices of edge. 
          void        SetLabelThrough( MbeLabelState l, void * key = nullptr ) const;
          /// \ru Установить метку ориентированному ребру, ребру грани и вершинам ребра. \en Set label for oriented edge, face edge and vertices of edge. 
          void        SetLabelThrough( MbeLabelState l, void * key, bool setLock ) const;
          /// \ru Получить частную метку, соответствующую ключу. Выдаст false первым аргументом пары, если ключа нет. \en Get private label according to the key. If there is no such key, returns false as 1st pair argument.
          std::pair<bool, MbeLabelState> GetPrivateLabel( void * key ) const { return label.GetPrivateLabel( key ); }
          /// \ru Удалить частную метку. \en Remove private label.
          void        RemovePrivateLabel ( void * key = nullptr ) const { label.DeletePrivate(key); }
          /// \ru Удалить частную метку ориентированному ребру, ребру грани и вершинам ребра. \en Remove private label for oriented edge, face edge and vertices of edge. 
          void        RemovePrivateLabelThrough( void * key ) const; 
          
          /// \ru Выдать точку на ребре (0 <= t <= 1). \en Get point on the edge (0 <= t <= 1). 
          void        Point( double t, MbCartPoint3D & p ) const; 
          /// \ru Выдать точку в начальной вершине. \en Get point at start vertex. 
          void        GetBegPoint( MbCartPoint3D & p ) const; 
          /// \ru Выдать точку в конечной вершине. \en Get point at start vertex. 
          void        GetEndPoint( MbCartPoint3D & p ) const; 
          /// \ru Выдать касательную к ребру  (0 <= t <= 1). \en Get tangent to the edge (0 <= t <= 1). 
          void        Tangent( double t, MbVector3D & p ) const; 
          /// \ru Выдать касательный вектор в начальной вершине. \en Get tangent vector at start vertex. 
          void        GetBegTangent( MbVector3D & p ) const; 
          /// \ru Выдать касательный вектор в конечной вершине. \en Get the tangent vector at the end vertex. 
          void        GetEndTangent( MbVector3D & p ) const; 

          /// \ru Выдать свойства объекта. \en Get properties of the object. 
          void        GetProperties( MbProperties & ); 
          /// \ru Установить свойства объекта. \en Set properties of the object. 
          void        SetProperties( const MbProperties & ); 

          // \ru Дать информацию для конвертеров. \en Get information for converters.
          void        GetFinCurve( const MbSurface &, SPtr<MbCurve> &, bool & );

          // \ru Дать информацию для конвертеров. \en Get information for converters. 
          void        GetFin( const MbSurface &, bool, SPtr<MbCurve> &, bool & ) const;
          // \ru Нормализовать fin по ChooseCurve() и установить правило выбора для конвертеров. \en Normalize  'fin' by ChooseCurve() and set the rule for selection of convertors. 
          void        NormalizeFin(); 
          // \ru Установить правило выбора для конвертеров. \en Set the rule for selection of converters. 
          void        SetChooseRule(); 

          // \ru Функции унификации объекта и вектора объектов в шаблонных функциях. \en Functions for compatibility of a object and a vector of objects in template functions.
          size_t                 size() const { return 1; } ///< \ru Количество объектов при трактовке объекта как  вектора объектов. \en Number of objects if object is interpreted as vector of objects. 
          const MbOrientedEdge * operator [] ( size_t ) const { return this; }  ///< \ru Оператор доступа. \en An access operator.

// \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
OBVIOUS_PRIVATE_COPY( MbOrientedEdge )
DECLARE_PERSISTENT_CLASS_NEW_DEL( MbOrientedEdge )
}; // MbOrientedEdge

IMPL_PERSISTENT_OPS( MbOrientedEdge )


//------------------------------------------------------------------------------
/** \brief \ru Цикл грани.
           \en Face loop. \~
  \details \ru Цикл грани представляет собой замкнутую составную кривую, проходящую вдоль границы грани. \n
    Цикл образован последовательностью ориентированных рёбер MbOrientedEdge. Цикл всегда замкнут. \n
    Цикл направлен так, чтобы грань всегда для нас находилась бы слева, 
    если мы движемся вдоль цикла c внешней стороны грани. \n
           \en A face loop represents a closed composite curve passing along the face boundary. \n
    A loop is formed by a sequence of oriented edges MbOrientedEdge. A loop is always closed. \n
    A loop is directed in such way that the face is always located on the left side, 
    if the moving along the loop is performed on the face external side. \n \~
  \ingroup Topology_Items
*/
// ---
class MATH_CLASS MbLoop : public MbTopItem, public MbSyncItem {
protected:
  RPArray<MbOrientedEdge> edgeList; ///< \ru Массив ориентированных ребер, составляющих цикл. \en An array of edges forming a loop. 
  mutable MbRect          rect;     ///< \ru Габаритный прямоугольник в пространстве параметров поверхности. \en A bounding box in a surface parameter space. 
  mutable MbLabel         label;    ///< \ru Временная метка для выполнения операций. \en Temporary label for performing of operations. 

public :
  /// \ru Пустой конструктор. \en Empty constructor. 
  MbLoop(); 
  /// \ru Конструктор по ребру или массиву ребер. \en Constructor by oriented edge or array of oriented edges. 
  template <class OrientEdges>
  MbLoop( const OrientEdges & initList )
    : MbTopItem()
    , edgeList()
    , rect()
    , label( ls_Null )
  {
    edgeList.reserve( initList.size() );
    for ( size_t i = 0, cnt = initList.size(); i < cnt; ++i ) {
      MbOrientedEdge * initEdge = &const_cast<MbOrientedEdge &>( *initList[i] );
      ::AddRefItem( initEdge );
      edgeList.push_back( initEdge );
    }
  }
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbLoop(); 

public :
  virtual void Accept( Visitor & visitor )
  {
    VisitorImpl<MbLoop> * impl = dynamic_cast<VisitorImpl<MbLoop> *>(&visitor);
    if( impl )
      impl->Visit( *this );
  }

  // \ru Функции топологического объекта. \en Functions of topological object 

  MbeTopologyType  IsA() const override; // \ru Тип элемента. \en A type of element.

          /// \ru Проверить и согласовать вершины цикла. \en Check and reconcile vertices of the loop. 
          void        CheckVertices(); 
          /// \ru Выдать количество ребер цикла. \en Get the number of edges of the loop. 
          size_t      GetEdgesCount() const { return edgeList.size(); } 

          /// \ru Получить метку цикла. \en Get a label of the loop. 
          MbeLabelState GetLabel( void * key = nullptr ) const { return (MbeLabelState)label.GetLabel(key); }
          /// \ru Установить метку. \en Set a label of the loop. 
          void        SetOwnLabel( MbeLabelState l, void * key = nullptr ) const { label.SetLabel( l, key ); }
          /// \ru Установить метку себе и ребрам цикла. \en Set a label for self and loop vertices. 
          void        SetLabelThrough( MbeLabelState l, void * key = nullptr ) const;
          /// \ru Установить метку себе и ребрам цикла. \en Set a label for self and loop vertices. 
          void        SetLabelThrough( MbeLabelState l, void * key, bool setLock ) const; 
          /// \ru Получить частную метку цикла, соответствующую ключу. Выдаст false первым аргументом пары, если ключа нет. \en Get loop private label according to the key. If there is no such key, returns false as 1st pair argument.
          std::pair<bool, MbeLabelState> GetPrivateLabel( void * key ) const { return label.GetPrivateLabel( key ); }
          /// \ru Удалить частную метку себе и ребрам цикла. \en Remove private label for self and loop edges. 
          void        RemovePrivateLabelThrough( void * key ) const; 
          /// \ru Установить метку ребрам. \en Set a label for edges. 
          void        SetCurveEdgesLabel( MbeLabelState, void * key = nullptr ) const;
          /// \ru Проверить метки рёбер и установить свою метку. \en Check edges labels and set own label. 
          void        CheckEdgesLabel( void * key = nullptr ) const;
          /// \ru Удалить частную метку. \en Remove private label. 
          void        RemovePrivateLabel ( void * key = nullptr ) const { label.DeletePrivate(key); }

          /// \ru Выдать множество вершин цикла. \en Get a set of loop vertices.
          template <class VerticesVector>
          void        GetVertices( VerticesVector & vertices ) const
          {
            size_t edgesCnt = edgeList.size();
            vertices.reserve( vertices.size() + edgesCnt * 2 );
            for ( size_t i = 0; i < edgesCnt; ++i )
              edgeList[i]->GetVerticesArray( vertices );
          }
          /// \ru Выдать множество вершин цикла. \en Get a set of loop vertices.
          template <class VerticesSet>
          void        GetVerticesSet( VerticesSet & vertices ) const
          {
            for ( const auto & edge : edgeList )
              edge->GetVerticesSet( vertices );
          }
          /// \ru Выдать множество ребер грани. \en Get a set of face edges. 
          template <class EdgesVector>
          void        GetEdges( EdgesVector & edges, bool findSame = true ) const
          {
            const size_t edgesCnt = edgeList.size();
            edges.reserve( edges.size() + edgesCnt );
            MbCurveEdge * edge = nullptr;
            for ( size_t i = 0; i < edgesCnt; ++i ) {
              edge = const_cast<MbCurveEdge *>( &edgeList[i]->GetCurveEdge() );
              if ( !findSame || (std::find( edges.begin(), edges.end(), edge ) == edges.end()) )
                edges.emplace_back( edge );
            }
          }
          /// \ru Выдать множество ребер грани. \en Get a set of face edges. 
          template <class EdgesSet>
          void        GetEdgesSet( EdgesSet & edges ) const
          {
            size_t edgesCnt = edgeList.size();
            MbCurveEdge * edge = nullptr;
            for ( size_t i = 0; i < edgesCnt; ++i ) {
              edge = const_cast<MbCurveEdge *>( &edgeList[i]->GetCurveEdge() );
              edges.emplace( edge );
            }
          }
          /// \ru Выдать множество уникальных ребер грани. \en Get a set of face unique edges. 
          template <class EdgesVector, class EdgesSet>
          void        GetEdges( EdgesVector & edges, EdgesSet & edgesCache ) const
          {
            size_t edgesCnt = edgeList.size();
            MbCurveEdge * edge = nullptr;
            for ( size_t i = 0; i < edgesCnt; ++i ) {
              edge = const_cast<MbCurveEdge *>( &edgeList[i]->GetCurveEdge() );
              if( edgesCache.insert( edge ).second )
                edges.emplace_back( edge );
            }
          }
          /// \ru Выдать множество ориентированных ребер. \en Get a set of oriented edges. 
          template <class OrientedEdgesVector>
          void        GetOrientedEdges( OrientedEdgesVector & edges, bool findSame = true ) const
          {
            size_t edgesCnt = edgeList.size();
            edges.reserve( edges.size() + edgesCnt );
            MbOrientedEdge * edge = nullptr;
            for ( size_t i = 0; i < edgesCnt; ++i ) {
              edge = const_cast<MbOrientedEdge *>( edgeList[i] );
              if ( !findSame || (std::find( edges.begin(), edges.end(), edge ) == edges.end()) )
                edges.emplace_back( edge );
            }
          }
          
          /// \ru Замена базового ребра. \en Replacement of the basis edge. 
          void        InitOrientedEdge( size_t edgeIndex, MbCurveEdge & initEdge, bool initOrientation, bool replaceVertices );
          /// \ru Выдать ориентированное ребро по номеру. \en Get an oriented edge by the number. 
          MbOrientedEdge * GetOrientedEdge( size_t index ) const { return (index < edgeList.size()) ? edgeList[index] : nullptr; }
          /// \ru Выдать ориентированное ребро по номеру. \en Get an oriented edge by the number. 
          MbOrientedEdge * SetOrientedEdge( size_t index )       { return (index < edgeList.size()) ? edgeList[index] : nullptr; }
          /// \ru Выдать ориентированное ребро по номеру без проверки корректности индекса. \en Get an oriented edge by the number without check of correctness of the index. 
          MbOrientedEdge *_GetOrientedEdge( size_t index ) const { return edgeList[index]; }
          /// \ru Выдать ориентированное ребро по номеру без проверки корректности индекса. \en Get an oriented edge by the number without check of correctness of the index. 
          MbOrientedEdge *_SetOrientedEdge( size_t index )       { return edgeList[index]; }

          /// \ru Добавить ребро без проверки. \en Add an edge without check. 
          void       _AddEdge     ( const MbOrientedEdge & ); 
          /// \ru Вставить ребро перед ребром с указанным индексом без проверки корректности индекса. \en Insert an edge at the given index without check of index correctness. 
          void       _InsertEdge  ( size_t k, const MbOrientedEdge & ); 
          /// \ru Добавить ребро после ребра с указанным индексом без проверки корректности индекса. \en Add an edge after an edge at the given index without check of index correctness. 
          void       _AddEdgeAfter( const MbOrientedEdge &, size_t k ); 
          /// \ru Добавить ребро. \en Add an edge. 
          void        AddEdge     ( const MbOrientedEdge & ); 
          /// \ru Вставить ребро перед ребром с указанным индексом. \en Insert an edge before an edge at the given index 
          void        InsertEdge  ( size_t k, MbOrientedEdge & ); 
          /// \ru Добавить ребро после ребра с указанным индексом. \en Add an edge after an edge at the given index 
          void        AddEdgeAfter( MbOrientedEdge &, size_t k ); 
          
          /// \ru Отсоединить ребро с заданным индексом. \en Detach an edge at the given index. 
          MbOrientedEdge * DetachEdge( size_t index );
          /// \ru Удалить ребро с заданным индексом. \en Delete an edge at the given index. 
          void        DeleteEdge ( size_t index );
          /// \ru Отцепить все ребра от цикла. \en Detach all edges from the loop. 
          void        DetachEdges( bool setNullToFace = false );
          /// \ru Удалить все ребра из цикла. \en Delete all edges of the loop. 
          void        DeleteEdges( bool setNullToFace = false );

          /// \ru Дать номер ребра грани в цикле. \en Get the number of a face edge in the loop. 
          size_t      GetEdgeIndex( const MbCurveEdge & edge, bool orient ) const; 
          /// \ru Найти следующее ребро за данным (next == true) или перед данным (next == false). \en Find the next edge after the given one (next==true) or before the given one (next==false). 
          bool        FindNeighbourEdge( const MbCurveEdge & edge, bool orient, bool next, MbCurveEdge *& findEdge, bool & findOrient ) const;

          /// \ru Изменить ориентацию цикла (порядок следования ориентированных рёбер и их ориентацию). \en Change orientation of the loop (the order of oriented edges and their orientation). 
          void        PartialReverse(); 
          /// \ru Изменить порядок следования ориентированных рёбер без изменения их ориентации. \en Change the order of oriented edges without changing of their orientation. 
          void        Inverse();

          /// \ru Принадлежит ли  вершина пути. \en Does a vertex belong a path? 
          bool        IsVertexOn( const MbVertex * vertex, size_t * index = nullptr ) const;
          /// \ru Замена указателей на поверхность. \en Replacement of the pointers to a surface. 
          void        ChangeSurface( MbSurface & oldSurf, MbSurface & newSurf, bool orient );

          /// \ru Найти вершину цикла по имени. \en Find loop vertex by name. 
  const   MbVertex *    FindVertexByName( const MbName & ) const;  
          /// \ru Найти ребро цикла по имени. \en Find loop edge by name. 
  const   MbCurveEdge * FindEdgeByName( const MbName & ) const;  

          /// \ru Найти вершину цикла по хешу имени. \en Find loop vertex by hash of a name. 
  const   MbVertex *    FindVertexByHash( const SimpleName h ) const;  
          /// \ru Найти ребро цикла по хешу имени. \en Find loop edge by hash of a name. 
  const   MbCurveEdge * FindEdgeByHash( const SimpleName h ) const;  

          /// \ru Создать двумерный контур по циклу. \en Create two-dimensional contour by loop. 
          MbContour & MakeContour( const MbSurface & surf, bool faceSense, bool doExact,
                                   MbRegDuplicate * iReg, bool calculateMetricLength = true ) const; 
          /// \ru Создать двумерный контур по циклу. \en Create two-dimensional contour by loop. 
          MbContour & MakeContour( const MbSurface & surf, bool faceSense, bool calculateMetricLength = true ) const;
          /// \ru Создать контур на поверхности по циклу. \en Create contour on surface by loop. 
          MbContourOnSurface & MakeContourOnSurface( const MbSurface & surf, bool faceSense, bool doExact = false ) const;
          /// \ru Создать пространственный контур по циклу. \en Create spatial contour by loop. 
          MbContour3D & MakeContour3D( bool doExact = false ) const;

          /// \ru Вычислить габаритный прямоугольник цикла. \en Calculate bounding box of loop. 
          bool        CalculateGabarit( const MbSurface &, bool faceSense ) const;
          /// \ru Выдать габаритный прямоугольник цикла. \en Calculate bounding rectangle of loop. 
          bool        GetGabarit( const MbSurface &, bool faceSense, MbRect & r ) const; 
          /// \ru Прямой доступ к переменной габарита с вызовом расчета. \en Direct access to the variable of bounding box with a call of calculation. 
  const   MbRect    & GetGabarit( const MbSurface & surf, bool faceSense ) const { 
                        if ( rect.IsEmpty() )
                          CalculateGabarit( surf, faceSense );
                        return rect;
                      }
          /// \ru Прямой доступ к переменной габарита без вызова расчета. \en Direct access to the variable of bounding box without a call of calculation. 
  const   MbRect    & GetGabarit() const { return rect; } 
          /// \ru Скопировать габарит с контура. \en Copy bounding box from contour. 
          void        CopyGabarit( const MbContour & c ); 
          /// \ru Скопировать габарит цикла. \en Copy bounding box of loop. 
          void        CopyGabarit( const MbLoop & c ) const { rect = c.rect; } 
          /// \ru Сбросить габарит - сделать его не определённым. \en Reset bounding box - make it undefined. 
          void        SetEmptyGabarit() const { rect.SetEmpty(); } 

          /// \ru Выдать свойства объекта. \en Get properties of the object. 
          void        GetProperties( MbProperties & ); 
          /// \ru Установить свойства объекта. \en Set properties of the object. 
          void        SetProperties( const MbProperties & ); 

          /// \ru Преобразовать объект согласно матрице. \en Transform an object according to the matrix. 
          void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr );
          /// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector. 
          void        Move     ( const MbVector3D &, MbRegTransform * = nullptr );
          /// \ru Повернуть объект вокруг оси на заданный угол. \en Rotate an object at a given angle around an axis.  
          void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr );
          /// \ru Сдвинуть двумерные кривые вдоль вектора в области параметров поверхности (все сразу). \en Move two-dimensional curves along the vector in the surface parameter region (all at once). 
          void        Move( MbVector &, const MbSurface &, bool ); 
          /// \ru Является ли контур граничным? \en Is a contour boundary? 
          bool        IsBoundaryFace( double mEps = Math::metricEpsilon ) const; 

          /// \ru Зарезервировать место под столько элементов. \en Reserve space for a given number of elements. 
          void        EdgesReserve( size_t additionalSpace ) { edgeList.Reserve( additionalSpace ); } 
          /// \ru Удалить лишнюю память. \en Free the unnecessary memory. 
          void        EdgesAdjust ()                         { edgeList.Adjust();                   } 

// \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
OBVIOUS_PRIVATE_COPY( MbLoop )
DECLARE_PERSISTENT_CLASS_NEW_DEL( MbLoop )
}; // MbLoop

IMPL_PERSISTENT_OPS( MbLoop )


//------------------------------------------------------------------------------
/** \brief \ru Параметр самопересечения циклов грани.
           \en Parameter of loops self-intersection. \~
  \details \ru Параметр самопересечения циклов грани.
           \en Parameter of loops self-intersection. \~
  \ingroup Topology_Items
*/
// ---
struct LoopCrossParam {
public:
  size_t              loopIndex;  ///< \ru Номер цикла грани. \en Loop index of the face. 
  const MbCurveEdge * edge;       ///< \ru Ребро цикла. \en Loop edge. 
  double              curveParam; ///< \ru Параметр двумерной кривой ребра, лежащей на поверхности грани. \en The parameter of two-dimensional curve that lies on the surface of a face and is contained in the edge. 
public:
  LoopCrossParam() : loopIndex( SYS_MAX_T), edge( nullptr ), curveParam( UNDEFINED_DBL ) {}
  LoopCrossParam( size_t li, const MbCurveEdge * e, double t ) : loopIndex( li ), edge( e ), curveParam( t ) {}
  LoopCrossParam( const LoopCrossParam & obj ) : loopIndex( obj.loopIndex ), edge( obj.edge ), curveParam( obj.curveParam ) {}

  const LoopCrossParam & operator = ( const LoopCrossParam & obj ) 
  {
    loopIndex  = obj.loopIndex;
    edge       = obj.edge;
    curveParam = obj.curveParam;
    return *this;
  }
};


//------------------------------------------------------------------------------
// Сортировка MbItemIndex
// ---
inline int ItemIndexCompare( const MbItemIndex * first, const MbItemIndex * second ) {
  return second->GetIndex() < first->GetIndex() ? 1 : -1;
}


//------------------------------------------------------------------------------
/** \brief \ru Грань.
           \en Face. \~
  \details \ru Грань представляет собой связный конечный кусок поверхности, 
    которому приписано направление нормали. \n
    Сторону грани, при взгляде на которую мы смотрим навстречу нормали, 
    будем называть внешней, другую сторону грани будем называть внутренней. \n
    Стороны поверхности MbSurface не обладают равноправием относительно нормали, 
    так как у поверхности одна сторона всегда внешняя, а другая сторона всегда внутренняя. 
    В отличие от поверхности для грани мы имеем возможность назначить направление нормали
    и тем самым назначить внешнюю и внутреннюю стороны. \n
    Границы грани описываются циклами MbLoop.
    Количество циклов грани равно количеству границ грани. 
    Один из циклов грани будем называть внешним, а остальные - внутренними. 
    Внутренние циклы целиком лежат внутри внешнего цикла. 
    Внешний цикл грани ориентирован против часовой стрелки, 
    а внутренние циклы ориентированы по часовой стрелке, 
    если смотреть навстречу выбранной нормали грани. \n 
    Циклы грани не могут пересекать друг друга и сами себя. \n
           \en A face is connected finite piece of a surface 
    with a normal direction. \n
    A side of a face which is seen when looking towards a normal 
     is called external side, other side is called internal side. \n
    Sides of a surface MbSurface do not have equal rights with respect to normal direction, 
    because one side of surface is always external and other is internal. 
    In contrast to surface, it is possible to assign a normal direction for a face
    and thereby to assign an external and internal sides. \n
    Face boundaries are described by loops MbLoop.
    A number of face loops is equal to a number of face boundaries. 
    One loop is external, other loops are internal. 
    Internal loops entirely lie inside the external loop. 
    External loop of a face is oriented counterclockwise, 
    but internal loops are oriented clockwise, 
    when looking towards the chosen normal of a face. \n 
    Loops do not intersect each other and themselves. \n \~
  \internal BUG_69306: \n \endinternal
\ru Если два цикла грани имеют общую точку, то эта точка в обоих
    циклах должна быть вершиной ребра.\n
\en If two loops of a face have a common point then this point in both
    loops should be a vertex of an edge.\n \~
  \ingroup Topology_Items
*/
// ---
class MATH_CLASS MbFace : public MbTopologyItem, public MbSyncItem {
public:
    //------------------------------------------------------------------------------
/** \brief \ru Вспомогательные данные для грани.
           \en Auxiliary data for a face. \~
    \details \ru Вспомогательные данные служат для ускорения работы объекта.
             \en Auxiliary data are used for fast calculations. \n \~
  */
  // ---
  struct MATH_CLASS MbFaceAuxiliaryData : public AuxiliaryData
  {
    MbFaceTemp * _temporal; ///< \ru Объект сопровождения грани (для скорости вычислений). \en An object for maintenance of a face (to improve calculations speed). 
    MbFaceAuxiliaryData() : AuxiliaryData(), _temporal( nullptr ) {}
    MbFaceAuxiliaryData( const MbFaceAuxiliaryData & ) : AuxiliaryData {}, _temporal( nullptr ) {}
    virtual ~MbFaceAuxiliaryData();
  };

protected:
  MbSurface *     surface;   ///< \ru Поверхность грани (всегда не nullptr). \en Face surface (always not nullptr).
  bool            sameSense; ///< \ru Признак совпадения направления нормали грани с нормалью поверхности. \en An attribute of coincidence between the face normal direction and the surface normal direction. 
  RPArray<MbLoop> loops;     ///< \ru Границы грани (первая граница должна быть внешней). \en Face boundaries (the first boundary should be external). 

  mutable CacheManager<MbFaceAuxiliaryData> * cache; ///< \ru Вспомогательные данные для грани. \en Auxiliary data for the face.

public:
  /// \ru Конструктор по поверхности и ориентации нормали грани относительно нормали поверхности. \en Constructor by surface and orientation of face normal in relation to surface normal. 
  MbFace( const MbSurface &, bool sense );
  /// \ru Конструктор по циклу, поверхности и ориентации нормали грани относительно нормали поверхности. \en Constructor by loop, surface and orientation of face normal in relation to surface normal. 
  explicit MbFace( MbLoop & bnd, const MbSurface & surf, bool sense );
  /// \ru Конструктор по циклам, поверхности и ориентации нормали грани относительно нормали поверхности. \en Constructor by loops, surface and orientation of face normal in relation to surface normal. 
  MbFace( MbLoop & bnd0, MbLoop & bnd1, const MbSurface & surf, bool sense );
  /// \ru Конструктор по циклам, поверхности и ориентации нормали грани относительно нормали поверхности. \en Constructor by loops, surface and orientation of face normal in relation to surface normal. 
  template <class Loops>
  MbFace( const Loops & bnds, const MbSurface & surf, bool sense )
    : MbTopologyItem()
    , surface   ( const_cast<MbSurface *>(&surf) )
    , sameSense ( sense ) // признак совпадения нормали
    , loops     ( bnds.size(), 1 )
    , cache     ( nullptr )
  {
    surface->AddRef();
    for ( size_t i = 0, cnt = bnds.size(); i < cnt; ++i ) {
      if ( bnds[i] != nullptr )
        AddLoop( *bnds[i] );
    }
  }

  /// \ru Конструктор по другой грани, новой поверхности и ориентации нормали грани относительно нормали этой поверхности. \en Constructor by other face, new surface and orientation of face normal in relation to  normal of this surface. 
  MbFace( const MbFace & other, const MbSurface & newSurface, bool surfaceSense );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbFace();

public:
  VISITING_CLASS( MbFace ); 

  // \ru Функции топологического объекта. \en Functions of topological object 

  MbeTopologyType IsA() const override; // \ru Тип элемента. \en A type of element.
  /// \ru Создать новую грань копированием всех данных исходной грани. \en Create new face by copying all data of the initial face. 
  virtual MbFace *    DataDuplicate( MbRegDuplicate * = nullptr ) const;
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Трансформация. \en Transformation.
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;   // \ru Перемещение. \en Moving.
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Вращение. \en Rotation.
  double      DistanceToPoint( const MbCartPoint3D & ) const override; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  void        AddYourGabaritTo( MbCube & ) const override; // \ru Добавить свой габарит в присланный габарит. \en Add your own bounding box into the sent bounding box. 
  void        CalculateLocalGabarit( const MbMatrix3D & into, MbCube & cube ) const override; // \ru Рассчитать габарит в локальной системы координат. \en Calculate bounding box in the local coordinate system. 

  /// \ru Выдать поверхность грани. \en Get a surface of a face. 
  virtual const MbSurface & GetSurface() const; 
  /// \ru Выдать поверхность грани для модификации. \en Get a surface of a face for modifications. 
  virtual       MbSurface & SetSurface();
  /// \ru Является ли грань плоской? \en Is a face planar? 
  virtual bool        IsPlanar( double accuracy = METRIC_EPSILON ) const;
  /// \ru Дать плоскость (или только возможность ее выдачи). \en Get a plane (or only a possibility of getting a plane) 
  virtual bool        GetPlacement( MbPlacement3D * ) const;
          /// \ru Выдать направление нормали грани по отношению к нормали поверхности. \en Get direction of face normal in relation to the direction of surface normal. 
          bool        IsSameSense() const { return sameSense; }
          /// \ru Установить направление нормали грани по отношению к нормали поверхности. \en Set direction of face normal in relation to the direction of surface normal. 
          void        SetSameSense( bool s ); 

          /// \ru Дать ось вращения для поверхности, если это возможно. \en Get a rotation axis of a surface if it is possible. 
          bool        GetCylinderAxis( MbAxis3D & axis ) const; 
          /// \ru Дать локальную систему координат плоскости грани, если это возможно. \en Get a local coordinate system of a face plane, if it is possible. 
          bool        GetPlanePlacement( MbPlacement3D & ) const;
          /// \ru Дать локальную систему координат плоскости поверхности, если это возможно. \en Get a local coordinate system of a surface plane, if it is possible. 
          bool        GetSurfacePlacement( MbPlacement3D & ) const; 
          /// \ru Дать локальную систему координат грани в средней точке, если это возможно. \en Get a local coordinate system in a face middle point, if it is possible. 
          bool        GetControlPlacement( MbPlacement3D & ) const; 
          /// \ru Сориентировать ось Х плейсмента вдоль линии его пересечения с поверхностью грани, ось Y - с нормалью. \en Orient the axis X of a placement along the line of its intersection with a surface of a face,  the axis Y - with a normal. 
          bool        OrientPlacement( MbPlacement3D & ) const; 

          /** \brief   \ru Дать нормаль грани. \en Get the face normal. \~
              \param[in] surfaceU -   \ru Координата U на поверхности грани, в uv-области поверхности.
                                      \en U-coordinate on the face's surface, in uv-area of the surface.\~
              \param[in] surfaceV -   \ru Координата V на поверхности грани, в uv-области поверхности.
                                      \en V-coordinate on the face's surface, in uv-area of the surface.\~
              \param[out] normal -    \ru Рассчитанная нормаль к грани в заданной точке.
                                      \en Calculated face's normal at the given point.\~
          */
          void        Normal( double surfaceU, double surfaceV, MbVector3D & normal ) const; 
          /// \ru Параллельно ли ребро грани? \en Is an edge parallel to a face? 
          bool        IsColinear( const MbCurveEdge & ) const; 

          /// \ru Выдать количество циклов (границ) грани . \en Get the number of loops (boundaries) of face. 
          size_t      GetLoopsCount() const { return loops.size(); } 
          /// \ru Установить метку грани, циклам, рёбрам и вершинам. \en Set a label of face to its loops, edges and vertices. 
          void        SetLabelThrough( MbeLabelState l, void * key = nullptr ) const;
          /// \ru Установить метку грани, циклам, рёбрам и вершинам. \en Set a label of face to its loops, edges and vertices. 
          void        SetLabelThrough( MbeLabelState l, void * key, bool setLock ) const;
          /// \ru Удалить частную метку грани, циклам, рёбрам и вершинам. \en Remove a private label of face to its loops, edges and vertices.
          void        RemovePrivateLabelThrough( void * key ) const; 

          /// \ru Модифицирована ли грань или ее ребро? \en Has been face or it edges modified? 
          bool        IsOwnChangedItem( bool checkVertices = false ) const; 

          /// \ru Выдать множество вершин грани. \en Get a set of face vertices.
          template <class VerticesVector>
          void        GetVertices( VerticesVector & vertices ) const
          {
            size_t loopsCnt = loops.size();
            vertices.reserve( vertices.size() + loopsCnt * 4 );
            for ( size_t i = 0; i < loopsCnt; ++i ) {
              if ( loops[i] != nullptr )
                loops[i]->GetVertices( vertices );
            }
          }
          /// \ru Выдать множество вершин грани. \en Get a set of face vertices.
          template <class VerticesSet>
          void        GetVerticesSet( VerticesSet & vertices ) const
          {
            for ( const auto & loop : loops )
              if ( loop != nullptr )
                loop->GetVerticesSet( vertices );
          }
          /// \ru Выдать множество ребер грани. \en Get a set of face edges. 
          template <class EdgesVector>
          void        GetEdges( EdgesVector &, size_t mapThreshold = 50 ) const;
          /// \ru Выдать множество ребер грани. \en Get a set of face edges. 
          template <class EdgesSet>
          void        GetEdgesSet( EdgesSet & edges ) const {
            size_t loopsCnt = loops.size();
            for ( size_t i = 0; i < loopsCnt; ++i ) {
              if ( loops[i] != nullptr )
                loops[i]->GetEdgesSet<EdgesSet>( edges );
            }
          }
          /// \ru Выдать множество уникальных ребер грани. \en Get a set of face unique edges. 
          template <class EdgesVector, class EdgesSet>
          void        GetEdges( EdgesVector & edges, EdgesSet & edgesCache ) const {
            size_t loopsCnt = loops.size();
            for ( size_t i = 0; i < loopsCnt; ++i ) {
              if ( loops[i] != nullptr )
                loops[i]->GetEdges<EdgesVector, EdgesSet>( edges , edgesCache );
            }
          }
          /// \ru Выдать множество ребер внешнего цикла грани. \en Get a set of edges in outer loop of face. 
          template <class EdgesVector>
          void        GetOuterEdges( EdgesVector &, size_t mapThreshold = 50 ) const;
          /// \ru Выдать множество смежных граней. \en Get a set of adjacent faces. 
          template <class FacesVector>
          void        GetNeighborFaces( FacesVector & ) const;
          /// \ru Есть ли смежные грани? \en Is there any neighbor face?
          bool        HasNeighborFace() const; 

          /// \ru Выдать границу (цикл) с проверкой корректности индекса. \en Get a boundary (a loop) with a check of index correctness. 
          MbLoop    * GetLoop( size_t index ) const { size_t cnt = loops.size(); return cnt ? loops[index % cnt] : nullptr; }
          /// \ru Выдать границу (цикл) без проверки корректности индекса. \en Get a boundary (a loop) without a check of index correctness. 
          MbLoop    *_GetLoop( size_t index ) const { return loops[index]; }
          /// \ru Обнулить количество ребер в цикле с указанным индексом. \en Set to null the number of edges in loop with the given index. 
          void        SetNullLoop( size_t index );
          /// \ru Поменять местами циклы. \en Swap loops. 
          void        ExchangeLoops( size_t i1, size_t i2 );
          /// \ru Добавить новый цикл грани. \en Add a new loop to a face. 
          void        AddLoop   ( MbLoop & l );
          /// \ru Вставить цикл по индексу. \en Insert a loop by an index. 
          void        InsertLoop( size_t index, MbLoop & l );
          /// \ru Заменить цикл другим. \en Replace a loop by other loop. 
          void        ChangeLoop( MbLoop & oldLoop, MbLoop & newLoop );
          /// \ru Отцепить цикл с указанным индексом. \en Detach a loop with the given index. 
          MbLoop    * DetachLoop( size_t index );
          /// \ru Удалить цикл с указанным индексом. \en Delete a loop with the given index. 
          void        DeleteLoop( size_t index );
          /// \ru Удалить все циклы грани. \en Delete all loops of a face. 
          void        DeleteLoops();
          /// \ru Скопировать границы поверхности с циклов грани. \en Copy face boundaries from face loops. 
          void        AdjustContours();

          /// \ru Установить указатели ребер цикла на грань. \en Set the pointers of loop edges to the face. 
          void        SetFaceToLoopEdges( MbLoop & );
          /// \ru Установить указатели на грань слева или грань справа в ребрах цикла на nullptr. \en Set to null the pointers to the face on the left or to the face on the right in edges of loop.
          void        SetNullToLoopEdges( MbLoop & );
          /// \ru Установить указатели на грань слева или грань справа в ребрах циклов на nullptr. \en Set to null the pointers to the face on the left or to the face on the right in edges of loops.
          void        SetNullToLoopsEdges();
          /// \ru Обнулить указатели на грань слева или грань справа, указывающие на смежную грань delFace, в ребрах циклов. \en Set to null pointers to the face on the left or to the face on the right which point to the adjacent face delFace in edges of loops. 
          bool        SetNullToFace( const MbFace * delFace );
          /// \ru Обнулить указатели на грань слева или грань справа, указывающие на смежную грань delFace, в ребрах циклов. \en Set to null pointers to the face on the left or to the face on the right which point to the adjacent face delFace in edges of loops. 
          bool        SetNullToFace( const MbFace * delFace, bool setEdgeChanged );
          /// \ru Установить указатели на грань слева или грань справа в ребрах циклов на данную грань и параметры поверхности по данным циклов грани (setBounds = true). \en Set the pointers to the face on the left or to the face on the right to the given face in edges of loops and parameters of surface by loops of face (setBounds = true). 
          void        MakeRight( bool setBounds = false );

          /// \ru Принадлежит ли вершина грани? \en Does a vertex belong an edge? 
          bool        IsVertexOn( const MbVertex * vertex, size_t * indLoop = nullptr, size_t * indEdge = nullptr ) const;

          /** \brief \ru Изменить ориентацию грани.
                     \en Change an orientation of a face. \~
            \details \ru Инвертировать ориентацию ребер в цикле и инвертировать флаг ориентации грани.
                     \en Invert an orientation of edges in loop and invert a flag of face orientation. \~
          */
          void        PartialReverse();

          /** \brief \ru Изменить ориентацию грани.
                     \en Change an orientation of a face. \~
            \details \ru Инвертировать ориентацию ребер в цикле, 
              поменять местами указатели на грани справа и слева в ребрах циклов,
              инвертировать флаг ориентации грани. \n
                     \en Invert an orientation of edges in loop, 
              swap pointers to a face on the left and to a face on the right in edges of loops,
              invert a flag of face orientation. \n \~
          */
          void        TotalReverse();

          /** \brief \ru Изменить ориентацию цикла с указанным индексом.
                     \en Invert an orientation of a loop with the given index. \~
            \details \ru Инвертировать ориентацию ребер в цикле с указанным индексом,
              инвертировать ориентацию соответствующих ребер в циклах соседних граней,
              установить указатели ребер цикла на нужные грани. \n
                     \en Invert orientation of edges in loop with the given index.
              invert an orientation of corresponding edges in loops of adjacent faces,
              set the pointers of loop edges to the required faces. \n \~
          */
          void        PartialReverseLoopWithNeighbours( size_t index );

  /// \ru Являются ли объекты равными? \en Determine whether objects are equal. 
  bool        IsSame( const MbTopologyItem & other, double accuracy ) const override;
  /// \ru Построить полигональную копию объекта mesh. \en Construct a polygonal copy of an object mesh). 
  void        CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; 
          void        CalculateWire( const MbStepData & stepData, MbMesh & mesh ) const // The method deprecated. It will be removed at 2019. Use CalculateMesh( stepData, MbFormNote(true, false), mesh ); \~  
                        { CalculateMesh( stepData, MbFormNote(true, false), mesh ); } 
          /// \ru Связаны ли грани? \en Are faces connected? 
          bool        IsConnectedWith( const MbFace * face, RPArray<const MbCurveEdge> * commonEdges = nullptr ) const;
          /// \ru Подобны ли поверхности для объединения трансформацией по матрице (первичная проверка)? \en Are surfaces similar for merge by transformation by the matrix (a primary check)? 
          bool        IsSimilarToFace( const MbFace & face, bool & normal, bool & planeType, VERSION version, double precision = METRIC_PRECISION ) const;
          /// \ru Подобны ли поверхности для объединения путем замены (первичная проверка)? \en Are surfaces similar for merge by replacement (a primary check)? 
          bool        IsSpecialSimilarToFace( const MbFace & face, bool & normal, bool & swap, VERSION version, double precision = METRIC_PRECISION ) const;

          /// \ru Заменить поверхность item на подобную поверхность init в грани и во всех её рёбрах. \en Replace the surface 'item' to the similar surface 'init' in the face and all edges. 
          bool        ChangeCarrier     ( const MbSurface & item, MbSurface & init ); 
          /// \ru Заменить поверхность item на подобную поверхность init в грани и во всех её рёбрах и преобразовать по матрице все двумерные кривые из области определения item. \en Replace the surface 'item' to the similar surface 'init' in the face and all edges and transform by the matrix all two-dimensional curves from the domain of 'item'. 
          bool        ChangeCarrierBorne( const MbSurface & item, MbSurface & init, const MbMatrix & matr ); 
          /// \ru Заменить поверхность грани поверхностью присланной грани. \en Replace the face surface by the sent face surface. 
          bool        ChangeCarrierBorneSpecial( const MbFace & face ); 
          /// \ru Подобны ли поверхности для замены с преобразованием по матрице двумерных кривых? \en Are surfaces similar for replacement with transformation of two-dimensional curves by the matrix? 
          bool        IsSimilarExactly( const MbFace & face, MbMatrix & matr, VERSION version, double precision = METRIC_PRECISION ) const;
          /// \ru Подобны ли поверхности граней для замены с преобразованием двумерных кривых? \en Are surfaces similar for replacement with transformation of two-dimensional curves? 
          bool        IsSpecialSimilarExactly( const MbFace & face, bool doSwap, double precision = METRIC_PRECISION ) const;

          /// \ru Найти следующее ребро за данным (next == true) или перед данным (next == false). \en Find the next edge after the given one (next==true) or before the given one (next==false). 
          bool        FindNeighbourEdge( const MbCurveEdge & edge, bool orient, bool next, MbCurveEdge *& findEdge, bool & findOrient ) const;
          /// \ru Найти индекс цикла в грани и индекс ребра в найденном цикле. \en Find the index of a loop in a face and the index of an edge in the found loop. 
          bool        FindEdgeIndex( const MbCurveEdge & edge, bool orient, size_t & loopIndex, size_t & edgeIndex ) const;
          /// \ru Найти индекс цикла в грани и индекс ребра в найденном цикле. \en Find the index of a loop in a face and the index of an edge in the found loop. 
          bool        FindEdgeIndex( const MbCurveEdge & edge, ThreeStates orient, size_t & loopIndex, size_t & edgeIndex ) const;
          /// \ru Дать ребро по индексам цикла грани и ребра в цикле. \en Get an edge by the indices of a face loop and an edge in the loop. 
          MbCurveEdge * GetEdgeByIndex( size_t loopIndex, size_t edgeIndex ) const;
          /// \ru Найти ориентированное ребро по индексам цикла грани и ребра в цикле. \en Find an oriented edge by the indices of a face loop and an edge in the loop.
  const   MbOrientedEdge * GetOrientedEdge( size_t loopIndex, size_t edgeIndex ) const; 
          MbOrientedEdge * SetOrientedEdge( size_t loopIndex, size_t edgeIndex ); 
          /// \ru Найти ориентированное ребро по ребру грани. Не ищет для швов. \en Find an oriented edge by the edge of a face. Don't seek if an edge is a seam.
  const   MbOrientedEdge * GetOrientedEdge( const MbCurveEdge & curveEdge ) const; 
          /// \ru Найти ориентированное ребро по ребру грани. Не ищет для швов. \en Find an oriented edge by the edge of a face. Don't seek if an edge is a seam.
          MbOrientedEdge * SetOrientedEdge( const MbCurveEdge & curveEdge );
          /// \ru Удалить ориентированное ребро по ребру грани. \en Remove an oriented edge by the edge of a face.
          bool        DeleteOrientedEdge( MbCurveEdge * curveEdge ); 
          /// \ru Найти номера для рёбер. \en Find numbers for the edges. 
          template<class EdgeVector>
          bool FindIndexByEdges( const EdgeVector &          initEdges, 
                                       SArray<MbItemIndex> & indexes    ) const {
            MbItemIndex ind;

            for ( size_t n = 0, nCount = initEdges.size(); n < nCount; n++ ) {
              MbCurveEdge * initEdge = initEdges[n];
              size_t number = 0;

              for ( size_t i = 0, iCount = loops.Count(); i < iCount; i++ ) {
                MbLoop * loop = loops[i];
                for( size_t j = 0, jCount = loop->GetEdgesCount(); j < jCount; j++ ) {
                  MbOrientedEdge * oe = loop->_GetOrientedEdge( j );
                  if ( &oe->GetCurveEdge() == initEdge ) {
                    ind.Init( oe->GetCurveEdge(), number );
                    indexes.Add( ind );
                    i = iCount;
                    j = jCount; // выход из циклов
                  }
                  number++;
                }
              }
            }

            // сортировка по возрастанию номера
            size_t indCount = indexes.Count();
            if ( indCount > 1 )
              QuickSort<MbItemIndex, MbItemIndex, MbItemIndex>( indexes.begin(), indCount, (KsQSortCompFunc)ItemIndexCompare );
          
            return indCount > 0;
          }
          /// \ru Найти рёбра по номерам. \en Find edges by numbers. 
          bool        FindEdgesByIndex( SArray<MbItemIndex> & indexes, RPArray<MbCurveEdge> & initEdges ) const;

          /// \ru Найти вершину по имени. \en Find vertex by name. 
  const   MbVertex  * FindVertexByName( const MbName & ) const; 
          /// \ru Найти ребро по имени. \en Find edge by name. 
  const   MbCurveEdge * FindEdgeByName( const MbName & ) const; 

          /// \ru Найти вершину по хешу имени. \en Find vertex by hash of a name. 
  const   MbVertex  * FindVertexByHash( const SimpleName h ) const; 
          /// \ru Найти ребро по хешу имени. \en Find edge by hash of a name. 
  const   MbCurveEdge * FindEdgeByHash( const SimpleName h ) const; 

          /// \ru Установить метку ориентированного ребра. \en Set a label for an oriented edge. 
          void        SetOrientedLabel ( const MbCurveEdge & edge, MbeLabelState n, void * key = nullptr );
          /// \ru Вычислить ближайшее расстояние до ребра и ближайшие точки грани и ребра. \en Calculate the nearest distance to an edge and the nearest points of an edge. 
          double      DistanceToEdge  ( const MbCurveEdge & edge, MbCartPoint3D & p, MbCartPoint3D & edgeP ) const;
          /// \ru Вычислить ближайшее расстояние до грани и ближайшие точки граней. \en Calculate the nearest distance to a face and the nearest points of faces. 
          double      DistanceToFace  ( const MbFace & face, MbCartPoint3D & p, MbCartPoint3D & faceP ) const;
          /// \ru Найти угол между прямым ребром и плоской гранью. \en Find an angle between straight edge and planar face. 
          bool        AngleWithEdge( const MbEdge &, double & angle ) const; 
          /// \ru Найти угол между плоскими гранями. \en Find an edge between planar faces. 
          bool        AngleWithFace( const MbFace &, double & angle ) const; 
          /** \brief \ru Найти ближайшее в 2д ребро грани к заданной точке через процирование.
                     \en Find the closest edge in 2d to a specific point via projection.\~
            \details \ru Точка должна содержать координаты на UV-области поверхности, принадлежащей грани.
                         Поиск осуществляется именно в 2d на поверхности через проецирование на граничные кривые.
                         В 3d результат может быть другим.
                     \en The point must contain coordinates on the UV-area of the surface corresponding to the face.
                         The search is perfrormed namely in 2d on the surface by projecting on the bounding curves.
                         In 3d result may be different.\~
            \param[in] pOnSurface -   \ru Исследуемая точка на поверхности грани, в uv-области поверхности.
                                      \en A point on the face's surface, in uv-area of the surface.\~
            \param[out] edgeIndex -   \ru Номер найденного ребра, где edgeIndex.first - номер цикла грани, edgeIndex.second - номер ребра в цикле.
                                      \en An index of found edge, where edgeIndex.first - face loop number, edgeIndex.second - edge number in loop.\~
            \param[out] tEdgeCurve -  \ru Параметр t точки 2д кривой ребра, принадлежащей этой грани, ближайшей к pOnSurface.
                                      \en A t-parameter of the edge's 2d-curve closest to the pOnSurface. Curve corresponds to this face.\~
            \param[out] orientation - \ru Ориентация найденного ребра.
                                      \en Found edge orientation. \~
            \param[out] distance -    \ru Расстояние от pOnSurface до ребра на UV-пространстве поверхности в 2д.
                                      \en Distance between pOnSurface and edge in 2d UV-area of the face's surface.\~
            \param[in] paramEpsilon - \ru Параметрическая погрешность (нормируется на dU и dV).
                                      \en Parameteric precision (is normalised on dU and dV).\~
            \return \ru Возвращает true, если ребро было найдено.
                    \en Returns true if the edge is found.\~
            \ingroup Topology_Items
          */
          bool        GetNearestEdge( const MbCartPoint &      pOnSurface,
                                            c3d::IndicesPair & edgeIndex,
                                            double &           tEdgeCurve,
                                            bool &             orientation,
                                            double &           distance,
                                            double             paramEpsilon = Math::paramEpsilon ) const;

          /// \ru Найти ребра, пересекающиеся с габаритом своим габаритами. \en Find edges by intersections of two-dimensional bounding boxes. 
          bool        GetRectIntersectingEdges( const MbRect & rect, std::vector<c3d::IndicesPair> & edgeLocs, double eps ) const;

          /** \brief \ru Найти параметрическое расстояние до ближайшей границы.
                     \en Find a parametric distance to the nearest boundary. \~
            \details \ru Найденное расстояние до ближайшей границы имеет положительное значение, если точка находится внутри, и отрицательное - если снаружи.
                     \en The calculated distance is positive if the point is inside, and is negative if it is outside. \~
            \param[in] point  - \ru Исследуемая точка.
                                \en A point. \~
            \param[out] precision  - \ru Погрешность вычислений.
                                     \en Precision of calculation. \~
            \return \ru Возвращает расстояние до ближайшей границы в пространстве параметров поверхности. \n
                    \en Returns the distance to the nearest boundary in 2D space of the surface parameters. \~
            \ingroup Topology_Items
          */
          double      DistanceToBorder( const MbCartPoint & point, double & precision ) const;
          /// \ru Найти параметрическое расстояние до ближайшей границы. \en Find a parametric distance to the nearest boundary. 
          double      DistanceToBorder( const MbCartPoint & point, MbVector & normal, double & precision ) const;
          /// \ru Найти параметрическое расстояние до ближайшей границы. \en Find a parametric distance to the nearest boundary. 
          double      DistanceToBorder( const MbCartPoint & point,
                                        size_t & loopNumber, 
                                        size_t & edgeNumber, 
                                        double & precision ) const;
          /** \brief \ru Найти параметрическое расстояние до ближайшей границы.
                     \en Find a parametric distance to the nearest boundary. \~
            \details \ru Найденное расстояние до ближайшей границы имеет положительное значение, если точка находится внутри, и отрицательное - если снаружи.
                     \en The calculated distance is positive if the point is inside, and is negative if it is outside. \~
            \param[in] point  - \ru Проецируемая точка.
                                \en A point. \~
            \param[out] normal  - \ru Двумерная нормаль границы границы в ближайшей точке.
                                  \en Two-dimensional normal of border at its closest point. \~
            \param[out] loopNumber - \ru Индекс ближайшего цикла.
                                     \en Index of nearest loop. \~
            \param[out] edgeNumber - \ru Индекс ближайшего ребра в цикле.
                                     \en Index of nearest edge. \~
            \param[out] corner  - \ru 0, если проекция не располагается на стыке ребер, 
                                      1, если проекция располагается в конце ориентированного ребра с индексом edgeLoc, 
                                     -1, если проекция располагается в начале ориентированного ребра с индексом edgeLoc.
                                  \en 0, if the projection is not located on the edge,
                                      1, if the projection is located at the end of the oriented edge with index edgeLoc,
                                     -1, if the projection is located at the begining of the oriented edge with index edgeLoc. \~
            \param[out] tEdgeCurve - \ru Параметр кривой ближайшего ребра.
                                     \en Curve parameter of the nearest edge. \~
            \param[out] precision  - \ru Погрешность вычислений.
                                     \en Precision of calculation. \~
            \return \ru Возвращает расстояние до ближайшей границы в пространстве параметров поверхности. \n
                    \en Returns the distance to the nearest boundary in 2D space of the surface parameters. \~
            \ingroup Topology_Items
          */
          double      DistanceToBorder( const MbCartPoint & point, 
                                              MbVector & normal,
                                              size_t & loopNumber, 
                                              size_t & edgeNumber, 
                                              ptrdiff_t & corner,
                                              double & tEdgeCurve,
                                              double & precision ) const;

          /** \brief \ru Найти ближайшую проекцию точки на грань.
                     \en Find the nearest projection of the point on face.  \~
            \details \ru Найти ближайшую проекцию точки p на поверхность грани или ее границу: ребро или вершину. \n
                     \en Find the nearest projection of the point on face or it border. \n \~
            \param[in] point - \ru Проецируемая точка.
                               \en A point. \~
            \param[out] u    - \ru Найденный первый параметр поверхности.
                               \en Found u parameter of surface. \~
            \param[out] v    - \ru Найденный второй параметр поверхности.
                               \en Found v parameter of surface. \~
            \param[out] normal  - \ru Нормаль поверхности или ее границы (ребра или вершины) в точке проекции.
                                  \en Normal of surface or boundaries (edges or vertices) at the point of the projection. \~
            \param[out] edgeLoc - \ru Если проекция не попала на ребро, то равен SYS_MAX_T, SYS_MAX_T.
                                      Если проекция попала на ребро, то индексы цикла и ребра в цикле.
                                  \en If the projection is not no the edge, then SYS_MAX_T, SYS_MAX_T. 
                                      If the projection is on the edge, then the loop index and edge index in the loop. \~
            \param[out] corner  - \ru 0, если проекция не располагается на стыке ребер, 
                                      1, если проекция располагается в конце ориентированного ребра с индексом edgeLoc, 
                                     -1, если проекция располагается в начале ориентированного ребра с индексом edgeLoc.
                                  \en 0, if the projection is not located on the edge,
                                      1, if the projection is located at the end of the oriented edge with index edgeLoc,
                                     -1, if the projection is located at the begining of the oriented edge with index edgeLoc. \~
            \return \ru Возвращает положение проекции относительно границ поверхности.
                    \en Returns the location of the projection point relative to the surface boundaries. \~
            \ingroup Topology_Items
          */
          MbeItemLocation NearPointProjection( const MbCartPoint3D & point, 
                                                     double & u, 
                                                     double & v,
                                                     MbVector3D & normal, 
                                                     c3d::IndicesPair & edgeLoc, 
                                                     ptrdiff_t & corner ) const;

          /// \ru Разбить рёбра грани точкой на поверхности. \en Split edges of a face by a point on surface. 
          bool        CuttingEdges( const MbCartPoint & p, double xEpsilon, double yEpsilon, 
                                    double paramPrecision );
          /// \ru Найти самопересечения циклов. \en Find self-intersections of loops. 
          bool        LoopSelfIntersection( std::vector<LoopCrossParam> & siParams1,
                                            std::vector<LoopCrossParam> & siParams2,
                                            std::vector<bool> * crossCrossings, 
                                            bool checkInsideEdges,
                                            double metricNear, 
                                            VERSION version ) const;
          /// \ru Построить нормальные ЛСК конструктивных плоскостей. \en Construct normal placements of constructive planes. 
          bool        CreateNormalPlacements ( const MbVector3D & axisZ, double angle, SArray<MbPlacement3D> & places, VERSION version = Math::DefaultMathVersion() ) const;
          /// \ru Построить касательные ЛСК конструктивных плоскостей. \en Construct tangent placements of constructive planes. 
          bool        CreateTangentPlacements( const MbVector3D & axisZ, SArray<MbPlacement3D> & places ) const;
          /// \ru Дать некоторую точку и нормаль на грани. \en Get some point and normal on face. 
          bool        GetAnyPointOn( MbCartPoint3D & pnt, MbVector3D & nor ) const;

          /// \ru Выдать свойства объекта. \en Get properties of the object. 
          void        GetProperties( MbProperties & ); 
          /// \ru Установить свойства объекта. \en Set properties of the object. 
          void        SetProperties( const MbProperties & ); 

          /// \ru Заменить поверхность на заданную. \en Replace the surface by the given one. 
          void        ChangeSurface( const MbSurface & newSurf ); 

          /// \ru Дать параметры грани (0 <= faceU <= 1, 0 <= faceV <= 1) для параметров поверхности surfaceU и surfaceV. \en Get the face parameters (0 <= faceU <= 1, 0 <= faceV <= 1) for surface parameters surfaceU and surfaceV. 
          void        GetFaceParam   ( const double   surfaceU,
                                       const double   surfaceV,
                                             double & faceU,
                                             double & faceV ) const;
          /// \ru Дать параметры поверхности surfaceU и surfaceV по параметрам грани (0 <= faceU <= 1, 0 <= faceV <= 1). \en Get surface parameters surfaceU and surfaceV for the face parameters (0 <= faceU <= 1, 0 <= faceV <= 1). 
          void        GetSurfaceParam( const double   faceU,
                                       const double   faceV,
                                             double & surfaceU,
                                             double & surfaceV ) const;
          /// \ru Дать точку point грани по абстрактным параметрам (0 <= faceU <= 1, 0 <= faceV <= 1). \en Get a point on a face by abstract parameters (0 <= faceU <= 1, 0 <= faceV <= 1). 
          void        Point( double faceU, double faceV, MbCartPoint3D & point ) const;  
          /// \ru Дать точку point грани по параметрам её поверхности. \en Get a point on a face by surface parameters. 
          void        PointOn( double surfaceU, double surfaceV, MbCartPoint3D & point ) const;  

          /// \ru Является ли грань граничной (имеет ли ребра, которые не стыкуются с другими гранями)? \en Is a face boundary? (Does it have edges where it is not connected with other faces? ) 
          bool        IsBoundaryFace( double mEps = Math::metricEpsilon ) const; 
          /// \ru Выдать множество граничных ребер грани. \en Get a set of boundary face edges. 
          template <class ConstEdgesVector>
          void        GetBoundaryEdges( ConstEdgesVector & ) const;
          /// \ru Выдать множество граничных ребер грани. \en Get a set of boundary face edges. 
          template <class ConstEdgesSet>
          void        GetBoundaryEdgesSet( ConstEdgesSet & ) const;

          /// \ru Дать топологическое состояние. \en Get topological state. 
          bool        GetTopologyState( ptrdiff_t & cntAdjacentFaces, RPArray<const MbFace> *& adjacentFaces,
                                        bool & boundaryFace, ptrdiff_t & cntLoops ) const; 

          /// \ru Зарезервировать место под циклы. \en Reserve memory for loops. 
          void        LoopsReserve( size_t additionalSpace ) { loops.Reserve( additionalSpace ); } 
          /// \ru Удалить лишнюю память. \en Free the unnecessary memory. 
          void        LoopsAdjust()                          { loops.Adjust();                   } 
          /// \ru Пересечение поверхности грани с плоскостью, результат - множество кривых на поверхности и двумерных кривых на плоскости . \en Intersection between face and plane, the result is a set of curves on surface and two-dimensional curves on plane. 
          void        SurfaceSection( const MbPlacement3D & place, PArray<MbCurve> & pCurve );

          /// \ru Построить копию поверхности и двумерные контуры по циклам для операций выдавливания и вращения кривых. \en Construct copy of surface and two-dimensional contours by loops for operations of extrusion and rotation of curves. 
          MbSurface * GetSurfaceCurvesData( RPArray<MbContour> & contours ) const;
          /// \ru Построить копию поверхности и двумерные контуры по циклам для операций выдавливания и вращения кривых. \en Construct copy of surface and two-dimensional contours by loops for operations of extrusion and rotation of curves. 
          MbSurface * GetSurfaceCurvesData( std::vector<SPtr<MbContour> > & contours ) const;

          /// \ru Обновить границы поверхности, ограниченной кривыми, по циклам грани. \en Update boundaries of a face bounded by curves by face loops. 
          bool        UpdateSurfaceBounds( bool curveBoundedOnly = true );
          /// \ru Выдать габарит грани. \en Get bounding box of face. \~
          MbCube      GetCube() const;
          /// \ru Выдать габарит области параметров. \en Get bounding box of the space of parameters. \~
          MbRect      GetRect() const;
          /// \ru Обновить габарит цикла с указанным индексом. \en Update rectangle bound of loop by index.
          bool        UpdateLoopRect( size_t loopIndex );
          /// \ru Сбросить габариты циклов. \en Reset rectangle bounds of loops.
          void        ResetLoopsRects() const;
          /** \brief \ru Определить, выпуклая ли грань.
                     \en Determine whether the face is convex.  \~
            \details \ru Определить, выпуклая ли грань.
                     \en Determine whether the face is convex. 
            \result \ru ts_positive - грань выпуклая, ts_negative - грань вогнутая, ts_neutral - невозможно опеределить.
                    \en ts_positive - the face is convex, ts_negative - the face is concave, ts_neutral - impossible to determine. \~
          */
          ThreeStates Salient() const;

public:
          /// \ru Создан ли временный объект сопровождения грани? \en Is a temporary object for the maintenance of a face created? 
          bool        IsTemporal() const { return cache != nullptr ? (*cache)()->_temporal != nullptr : false; }
          /// \ru Удалить временный объект сопровождения. \en Delete a temporary maintenance object. 
          void        RemoveTemporal() const; 
          /// \ru Создать новый временный объект сопровождения. \en Create new temporary maintenance object. 
    const MbFaceTemp * CreateTemporal( bool keepExisting ) const;
          /// \ru Обновить временный объект сопровождения, если он уже создан. \en Update temporary maintenance object if it is already created. 
          bool        UpdateTemporal() const;


// \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
OBVIOUS_PRIVATE_COPY( MbFace )
DECLARE_PERSISTENT_CLASS_NEW_DEL( MbFace )
};

IMPL_PERSISTENT_OPS( MbFace )


//------------------------------------------------------------------------------
// \ru Выдать множество ребер грани. \en Get a set of face edges. 
// ---
template <class EdgesVector>
void MbFace::GetEdges( EdgesVector & edges, size_t mapThreshold ) const
{
  size_t loopsCnt = loops.size();
  edges.reserve( edges.size() + loopsCnt * 4 );

  bool useMap = false;

  if ( edges.size() < 1 ) {
    size_t checkCnt = 0;
    for ( size_t i = 0; i < loopsCnt; ++i ) {
      if ( loops[i] != nullptr ) {
        checkCnt += loops[i]->GetEdgesCount();
        if ( checkCnt > mapThreshold ) {
          useMap = true;
          break;
        }
      }
    }
    if ( useMap ) { // performance
      std::map<MbCurveEdge *, size_t> mapEdges;
      std::map<MbCurveEdge *, size_t>::iterator mapIt;

      size_t edgeIndex = 0;
      MbCurveEdge * edge = nullptr;
      for ( size_t i = 0; i < loopsCnt; ++i ) {
        MbLoop * loop = loops[i];
        if ( loop != nullptr ) {
          for ( size_t j = 0, edgesCnt = loop->GetEdgesCount(); j < edgesCnt; ++j ) {
            if ( loop->_GetOrientedEdge( j ) != nullptr ) {
              edge = &loop->_GetOrientedEdge( j )->GetCurveEdge();
              mapIt = mapEdges.find( edge );
              if ( mapIt == mapEdges.end() ) {
                mapEdges.insert( std::make_pair( edge, edgeIndex ) );
                edges.emplace_back( edge );
                ++edgeIndex;
              }
            }
          }
        }
      }
    }
  }
  if ( !useMap ) {
    for ( size_t i = 0; i < loopsCnt; ++i ) {
      if ( loops[i] != nullptr )
        loops[i]->GetEdges( edges );
    }
  }
}


//------------------------------------------------------------------------------
// \ru Выдать множество ребер внешнего цикла грани. \en Get a set of edges in outer loop of face. 
// ---
template <class EdgesVector>
void MbFace::GetOuterEdges( EdgesVector & edges, size_t mapThreshold ) const
{
  size_t loopsCnt = loops.size();
  edges.reserve( edges.size() + loopsCnt * 4 );

  bool useMap = false;

  if ( edges.size() < 1 ) {
    size_t checkCnt = 0;
    if ( loops.front() != nullptr ) {
      checkCnt += loops.front()->GetEdgesCount();
      if ( checkCnt > mapThreshold )
        useMap = true;
    }
    if ( useMap ) { // performance
      std::map<MbCurveEdge *, size_t> mapEdges;
      std::map<MbCurveEdge *, size_t>::iterator mapIt;

      size_t edgeIndex = 0;
      MbCurveEdge * edge = nullptr;
      MbLoop * loop = loops.front();
      for ( size_t j = 0, edgesCnt = loop->GetEdgesCount(); j < edgesCnt; ++j ) {
        if ( loop->_GetOrientedEdge( j ) != nullptr ) {
          edge = &loop->_GetOrientedEdge( j )->GetCurveEdge();
          mapIt = mapEdges.find( edge );
          if ( mapIt == mapEdges.end() ) {
            mapEdges.insert( std::make_pair( edge, edgeIndex ) );
            edges.emplace_back( edge );
            ++edgeIndex;
          }
        }
      }
    }
  }
  if ( !useMap ) {
    if ( loops.front() != nullptr )
      loops.front()->GetEdges( edges );
  }
}


//------------------------------------------------------------------------------
// \ru Выдать множество граничных ребер грани. \en Get a set of boundary face edges. 
// ---
template <class ConstEdgesVector>
void MbFace::GetBoundaryEdges( ConstEdgesVector & boundaryEdges ) const
{
  for ( const auto & loop : loops ) {
    if ( loop == nullptr )
      continue;
    MbCurveEdge * edge = nullptr;
    for ( size_t j = 0, edgesCnt = loop->GetEdgesCount(); j < edgesCnt; ++j ) {
      const MbOrientedEdge * orientEdge = loop->_GetOrientedEdge( j );
      if ( orientEdge == nullptr )
        continue;
      edge = const_cast<MbCurveEdge *>( &orientEdge->GetCurveEdge() );
      if ( edge->IsBoundaryFace() ) {
        boundaryEdges.emplace_back( edge );
      }
    }
  }
}

//------------------------------------------------------------------------------
// \ru Выдать множество граничных ребер грани. \en Get a set of boundary face edges. 
// ---
template <class ConstEdgesSet>
void MbFace::GetBoundaryEdgesSet( ConstEdgesSet & boundaryEdges ) const
{
  for ( const auto & loop : loops ) {
    if ( loop == nullptr )
      continue;
    MbCurveEdge * edge = nullptr;
    for ( size_t j = 0, edgesCnt = loop->GetEdgesCount(); j < edgesCnt; ++j ) {
      const MbOrientedEdge * orientEdge = loop->_GetOrientedEdge( j );
      if ( orientEdge == nullptr )
        continue;
      edge = const_cast<MbCurveEdge *>(&orientEdge->GetCurveEdge());
      if ( edge->IsBoundaryFace() ) {
        boundaryEdges.emplace( edge );
      }
    }
  }
}


//------------------------------------------------------------------------------
// \ru Выдать множество смежных граней. \en Get a set of adjacent faces. 
// ---
template <class FacesVector>
void MbFace::GetNeighborFaces( FacesVector & neighborFaces ) const
{
  const size_t loopsCnt = GetLoopsCount();

  if ( loopsCnt > 0 ) {
    std::vector< std::pair<const MbFace *, MbeLabelState> > facesLabels;
    facesLabels.reserve( neighborFaces.size() + 5 );
    size_t k;

    // save labels for faces added before
    size_t neighborsCnt0 = neighborFaces.size();
    for ( k = 0; k < neighborsCnt0; k++ ) {
      const MbFace * neighborFace = neighborFaces[k];
      if ( neighborFace != nullptr )
        facesLabels.push_back( std::make_pair( neighborFace, neighborFace->GetLabel() ) );
    }
    neighborsCnt0 = facesLabels.size();

    // mark neighbour faces by the first label
    for ( k = 0; k < loopsCnt; ++k ) {
      const MbLoop * loop = loops[k];
      if ( loop == nullptr )
        continue;
      for ( size_t edgeInd = 0, edgesCnt = loop->GetEdgesCount(); edgeInd < edgesCnt; ++edgeInd ) {
        const MbOrientedEdge * edge = loop->_GetOrientedEdge( edgeInd );
        if ( edge == nullptr )
          continue;
        const MbFace * neighborFace = edge->GetFaceMinus();
        if ( neighborFace != nullptr && neighborFace != this ) {
          facesLabels.push_back( std::make_pair( neighborFace, neighborFace->GetLabel() ) ); // save initial label
          neighborFace->SetOwnLabel( ls_Used );
        }
      }
    }
    size_t neighborsCnt1 = facesLabels.size();

    // mark faces added before by the second label (label of initial state)
    for ( k = 0; k < neighborsCnt0; ++k ) {
      facesLabels[k].first->SetOwnLabel( ls_Null );
    }
    // add faces with the first label
    neighborFaces.reserve( neighborsCnt1 );
    MbFace * neighborFace = nullptr;
    for ( k = neighborsCnt0; k < neighborsCnt1; ++k ) {
      neighborFace = const_cast<MbFace *>(facesLabels[k].first);
      if ( neighborFace->GetLabel() == ls_Used ) {
        neighborFace->SetOwnLabel( facesLabels[k].second ); // restore initial label
        neighborFaces.emplace_back( neighborFace );
      }
    }
    // restore initial labels for faces added before
    for ( k = 0; k < neighborsCnt0; ++k ) {
      facesLabels[k].first->SetOwnLabel( facesLabels[k].second ); // restore initial label
    }
  }
}

#endif // __TOPOLOGY_H
