////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Методы построения полигональных геометрических объектов.
         \en Functions for construction of the polygonal geometric object. \~
  \details \ru Полигональные геометрические объекты могут быть построены по набору точек или на базе других объектов.
           \en Polygonal geometric objects can be constructed using a set of point or on the basis of other objects. \~ 

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ACTION_MESH_H
#define __ACTION_MESH_H

#include <mb_cart_point.h>
#include <mb_cart_point3d.h>
#include <mesh.h>
#include <mb_enum.h>
#include <mb_operation_result.h>
#include <curve3d.h>
#include <vector>

class    MbPlacement3D;
class    MbMesh;
class    MbSurface;
class    MbSolid;
class    MbPlaneItem;
class    MbSNameMaker;
class    MbFace;
class    MbCollection;
class    MbMeshSimplificationParams;
class    MbConvexHullParams;
struct   MbConvexHullResults;
class    MbConvexHullParams3D;
class    MbRepairInconsistentMeshParams;
class    MbMeshToInstanceParams;
class    MbMeshToInstanceResults;
class    MbOrientedBox;
class    MbMeshUnwrapParams;
class    MbMeshUnwrapResult;
class    MbObjectAlignmentParams;
class    MbObjectAlignmentResult;


//------------------------------------------------------------------------------
/** \brief \ru Построить полигональную копию объекта.
           \en Build polygonal copy of an object. \~
  \details \ru Построить полигональную копию данного объекта, представленную полигонами, или/и плоскими пластинами.
           \en Build a polygonal copy of the object that is represented by polygons or/and fasets. \~
  \param[in] item -      \ru Объект геометрической модели.
                         \en The model geometric object. \~
  \param[in] stepData -  \ru Данные для вычисления шага при построении полигонального объекта.
                         \en Data for еру step calculation for polygonal object. \~
  \param[in] note     -  \ru Способ построения полигонального объекта.
                         \en Way for polygonal object constructing. \~
  \param[in, out] mesh - \ru Построенный полигональный объект.
                         \en The builded polygonal object.
  \return                \ru Возвращает код результата операции.
                         \en Returns operation result code. \~
  \ingroup Polygonal_Objects
*/
// ---
MATH_FUNC( MbResultType ) CalculateMesh( const MbItem & item, const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh );



//------------------------------------------------------------------------------
/** \brief \ru Данные диагностики полигонального объекта.
           \en Diagnostic data of a polygonal object. \~
  \ingroup Polygonal_Objects
*/
// ---
struct MATH_CLASS MeshInfo
{
/**
  \ru \name Типы данных. \en \name Data types.
\{ */
  enum ErrorState // \ru Состояние полигонального объекта. \en State of a polygonal object.
  {
    es_Unknown, // \ru Состояние не известно. \en Unknown state.
    es_Closed,  // \ru Замкнутый полигональный объект. \en Closed polygonal object.
    es_Open,    // \ru Незамкнутый полигональный объект. \en Open polygonal object.
    es_InconsistPolys, // \ru Несогласованная ориентация полигонов. \en Inconsistent orientation of polygons.
    es_InconsistGrids, // \ru Несогласованная ориентация триангуляций. \en Inconsistent orientation of grids.
    es_DegeneratePoly  // \ru Вырожденные полигоны. \en Degenerated polygons.
  };

  struct Vertex { // \ru Вершина. \en Vertex.
    uint  _data; // \ru Идентификатор вершины в триангуляции. \en Vertex ID in the triangulation.
    Vertex( uint v ) :  _data( v ) {}
  private:
    Vertex(); // \ru Запрет на создание значения по умолчанию. \en Prohibiting the creation of a default value.
  };

  struct Edge { // \ru Ребро. \en Edge.
    Vertex  _vert1; // \ru Идентификатор вершины в триангуляции. \en Vertex ID in the triangulation.
    Vertex  _vert2; // \ru Идентификатор вершины в триангуляции. \en Vertex ID in the triangulation.
    Edge( uint v1, uint v2 ) :  _vert1( v1 ), _vert2( v2 ) {}
    Edge( c3d::UintPair v ) :  _vert1( v.first ), _vert2( v.second ) {}
  private:
    Edge(); // \ru Запрет на создание значения по умолчанию. \en Prohibiting the creation of a default value.
  };

  struct Facet : public c3d::UintVector { // \ru Полигон (массив индексов вершин в триангуляции). \en Polygon (an array of vertex indices in the triangulation).
    Facet() {}
    Facet( const c3d::UintVector & v ) : c3d::UintVector( v ) {}
  private:
    void resize( size_type ); // \ru Запрет на добавление значений по умолчанию. \en Prohibiting the addition of default values.
  };

  typedef std::pair<size_t, std::vector<Facet>>  Facets;   // \ru Набор полигонов в триангуляции. \en A a set of polygons in the triangulations.
  typedef std::pair<size_t, std::vector<Edge>>   Edges;    // \ru Набор ребер в триангуляции. \en A a set of edges in the triangulations.
  typedef std::pair<size_t, std::vector<Vertex>> Vertices; // \ru Набор вершин в триангуляции. \en A a set of vertices in the triangulation.
/** \} */

  // \ru Массивы данных. \en Arrays of data.
  std::vector<Edges>      _edges;     // \ru Набор ребер триангуляций. \en A a set of edges of triangulations.
  std::vector<Facets>     _facets;    // \ru Набор полигонов триангуляций. \en A a set of polygons of triangulations.
  std::vector<Vertices>   _vertices;  // \ru Набор вершин триангуляций. \en A a set of vertices of triangulations.
  std::vector<ErrorState> _states;    // \ru Диагностики. \en Diagnostics.

  MeshInfo() {}

private:
  bool operator == ( const MeshInfo & ); // \ru Не реализован. \en Not implemented. 
  bool operator != ( const MeshInfo & ); // \ru Не реализован. \en Not implemented. 

  OBVIOUS_PRIVATE_COPY( MeshInfo )

};


//------------------------------------------------------------------------------
  /** \brief \ru Расчет полигона кривой.
             \en Calculation of polygon of curve. \~
  \details \ru Расчет трехмерного полигона двумерной кривой в плоскости XOY локальная системы координат.
           \en Calculation of three-dimensional polygon of two-dimensional curve located in the XOY-plane of a local coordinate system. \~
  \param[in] curve - \ru Двумерная кривая.
                     \en A two-dimensional curve. \~
  \param[in] plane - \ru Локальная система координат.
                     \en Local coordinate system. \~
  \param[in] sag - \ru Максимальное допустимое отклонение полигона от оригинала по прогибу или по углу между соседними элементами.
                   \en Maximum allowable deviation of polygon from the original by sag or by angle between neighboring elements. \~
  \param[out] polygon - \ru Рассчитанный полигон.
                        \en Calculated polygon. \~
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC (void) CalculatePolygon( const MbCurve &       curve, 
                                   const MbPlacement3D & plane,
                                         double          sag, 
                                         MbPolygon3D &   polygon );


//------------------------------------------------------------------------------
/** \brief \ru Построить полигональный двухмерный объект.
           \en Create a polygonal two-dimensional object. \~
  \details \ru Построить полигональный объект для двумерного объекта в плоскости XOY 
           локальной системы координат.
           \en Create a polygonal object for two-dimensional object in the XOY-plane 
           of the local coordinate system. \~
  \param[in] obj - \ru Двумерный объект (если nullptr, то объект не создаётся).
                   \en Two-dimensional object (if nullptr, object isn't created). \~
  \param[in] plane - \ru Локальная система координат.
                     \en A local coordinate system. \~
  \param[in] sag - \ru Максимальное отклонение полигонального объекта от оригинала по прогибу.
                   \en The maximum deviation of polygonal object from the original object by sag. \~
  \param[out] mesh - \ru Полигональный объект.
                     \en Polygonal object. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC (void) CalculateWire( const MbPlaneItem &   obj, 
                                const MbPlacement3D & plane, 
                                      double          sag, 
                                      MbMesh &        mesh );

//------------------------------------------------------------------------------
/** \brief \ru Построить икосаэдр в виде полигональной модели.
           \en Construct an icosahedron mesh. \~
  \details \ru Построить икосаэдр в виде полигональной модели. \n
           \en Construct an icosahedron mesh. \n \~
  \param[in] place - \ru Местная система координат.
                     \en Local placement. \~
  \param[in] radius - \ru Радиус описанной сферы.
                      \en The radius of the sphere. \~
  \param[in] fn - \ru Способ построения полигонального объекта.
                  \en Way for polygonal object constructing. \~
  \param[out] result - \ru Результат построения.
                       \en The resulting mesh. \~
  \return  \ru Возвращает код результата операции.
           \en Returns operation result code. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC (MbResultType) CreateIcosahedron( const MbPlacement3D & place, 
                                                  double          radius, 
                                            const MbFormNote &    fn,
                                                  MbMesh *&       result );


//------------------------------------------------------------------------------
/** \brief \ru Построить параллелепипед в виде полигональной модели.
           \en Construct a parallepiped mesh. \~
  \details \ru Построить ориентированный параллелепипед в виде полигональной модели.
               Если матрица 'trans' единичная, то результатом вызова будет кубик 
               единичного объема. В общем случае матрица содержит смещение и ротацию,
               позиционирующую куб относительно начала координат. Размеры параллелепипеда 
               определяются коэффициентами масштабирования по собственным осям ЛСК 
               параллелепипеда, содержащимися в матрице.
           \en Construct an oriented box in the mesh representation. If the matrix 'trans' 
               is identity then the result of the call will be an unit cube. In general, 
               the matrix contains a translation and rotation that positions the cube 
               relative to the origin. The sizes of the parallelepiped are specified
               by the scaling factors of the matrix along the eigen axes of the LCS of 
               the parallelepiped. \~
  \note \ru У матрицы должна быть ротационная часть ортогональной и не вырожденной. 
            Масштабирующая часть определяет размеры параллелепипеда.
        \en The rotational componet of the matrix should be nondegenerate and ortogonal. 
            The scaling component specifies the box sizes.
  \param[in] trans - \ru Матрица, задающая позицию, ориентацию и размеры сторон параллелепипеда.
                     \en The matrix specifying a postion, orientation and sizes of the box sides. \~
  \param[out] result - \ru Результат построения.
                       \en The resulting mesh. \~
  \return  \ru Возвращает код результата операции.
           \en Returns operation result code. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC (MbResultType) CreateBoxMesh( const MbMatrix3D &   trans, 
                                              SPtr<MbMesh> & result );


//------------------------------------------------------------------------------
/** \brief \ru Построить параллелепипед в виде проволочной модели.
           \en Construct an parallepiped wireframe.
    \details \ru Функция конструирует проволочный каркас параллелепипеда по тем же правилам, что и #CreateBoxMesh.
             \en The function makes the wireframe of the oriented box by the same rules as #CreateBoxMesh.
  \ingroup Polygonal_Objects
*/ //---
MATH_FUNC (MbResultType) CreateBoxWire( const MbMatrix3D &   trans, 
                                              SPtr<MbMesh> & result );


//------------------------------------------------------------------------------
/** \brief \ru Построить пространственный проволочный каркас по полигональному объекту.
           \en Create a spatial wireframe by a mesh \~
  \details \ru Построить набор пространственных кривых, характеризующих полигональных объект. \n
           \en Create a spatial wireframe by a mesh. \n \~
  \param[in] item - \ru Полигональный объект.
                    \en The mesh. \~
  \param[out] wire - \ru Набор пространственных кривых, характеризующих полигональных объект.
                     \en A spatial wireframe by a mesh. \~
  \param[in] isVisibleOnly - \ru В случае если флаг true, то обрабатываются только видимые полигоны сетки.
                             \en If true only visible mesh polygons are processed. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC( void ) MakeSpaceWireFrame( const MbItem &             item, 
                                            RPArray<MbCurve3D> & wire, 
                                            bool                 isVisibleOnly = false );


//------------------------------------------------------------------------------
/** \brief \ru Построить плоский проволочный каркас по полигональному объекту.
           \en Create a planar wireframe by a mesh \~
  \details \ru Построить пространственных набор кривых, являющихся проекциями кривых, характеризующих полигональных объект, на плоскость XY локальной системы координат. \n
           \en Create a planar wireframe by a mesh. \n \~
  \param[in] item - \ru Полигональный объект.
                    \en The mesh. \~
  \param[in] place - \ru Локальная система координат.
                     \en The local coordinate system. \~
  \param[out] wire - \ru Набор пространственных кривых, характеризующих полигональных объект.
                     \en A spatial wireframe by a mesh. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC( void ) MakePlaneWireFrame( const MbItem &             item,
                                      const MbPlacement3D &      place,
                                            RPArray<MbCurve3D> & wire );


//------------------------------------------------------------------------------
/** \brief \ru Построить плоский проволочный каркас по полигональному объекту в перспективе.
           \en Create a planar wireframe by a mesh in perspective \~
  \details \ru Построить пространственных набор кривых, являющихся проекциями кривых в перспективе, характеризующих полигональных объект, на плоскость XY локальной системы координат. \n
           \en Create a planar wireframe by a mesh in perspective. \n \~
  \param[in] item - \ru Полигональный объект.
                    \en The mesh. \~
  \param[in] place - \ru Локальная система координат.
                     \en The local coordinate system. \~
  \param[in] vista - \ru Точка наблюдения.
                     \en The observation point . \~
  \param[out] wire - \ru Набор пространственных кривых, характеризующих полигональных объект.
                     \en A spatial wireframe by a mesh. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC( void ) MakePlaneVistaWireFrame( const MbItem &             item, 
                                           const MbPlacement3D &      place,
                                           const MbCartPoint3D &      vista,
                                                 RPArray<MbCurve3D> & wire );


//------------------------------------------------------------------------------
/** \brief \ru Построить плоский проволочный каркас по полигональному объекту.
           \en Create a planar wireframe by a mesh \~
  \details \ru Построить набор двумерных кривых, являющихся проекциями пространственных кривых, характеризующих полигональных объект, на плоскость XY локальной системы координат. \n
           \en Create a planar wireframe by a mesh. \n \~
  \param[in] item - \ru Полигональный объект.
                    \en The mesh. \~
  \param[in] place - \ru Локальная система координат.
                     \en The local coordinate system. \~
  \param[out] wire - \ru Набор двумерных кривых, характеризующих полигональных объект.
                     \en A spatial wireframe by a mesh. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC( void ) MakePlaneWireFrame( const MbItem &           item, 
                                      const MbPlacement3D &    place,
                                            RPArray<MbCurve> & wire );


//------------------------------------------------------------------------------
/** \brief \ru Построить плоский проволочный каркас по полигональному объекту в перспективе.
           \en Create a planar wireframe by a mesh in perspective \~
  \details \ru Построить набор двумерных кривых, являющихся проекциями в перспективе пространственных кривых, характеризующих полигональных объект, на плоскость XY локальной системы координат. \n
           \en Create a planar wireframe by a mesh in perspective. \n \~
  \param[in] item - \ru Полигональный объект.
                    \en The mesh. \~
  \param[in] place - \ru Локальная система координат.
                     \en The local coordinate system. \~
  \param[in] vista - \ru Точка наблюдения.
                     \en The observation point . \~
  \param[out] wire - \ru Набор двумерных кривых, характеризующих полигональных объект.
                     \en A spatial wireframe by a mesh. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC( void ) MakePlaneVistaWireFrame( const MbItem &           item,
                                           const MbPlacement3D &    place,
                                           const MbCartPoint3D &    vista, 
                                                 RPArray<MbCurve> & wire );


//------------------------------------------------------------------------------
// .
/** \brief \ru Построить полигональную сферу.
           \en Construct an spherical mesh. \~
  \details \ru Построить аппроксимацию сферы выпуклым многогранником. \n
           \en Construct an approximation of the sphere by a convex polyhedron. \n \~
  \param[in] place - \ru Местная система координат.
                     \en Local placement. \~
  \param[in] radius - \ru Радиус сферы.
                      \en The radius of the sphere. \~
  \param[in] epsilon - \ru Параметр аппроксимации сферы.
                       \en The approximation parameter. \~
  \param[out] result - \ru Результат построения.
                       \en The resulting mesh. \~
  \return  \ru Возвращает код результата операции.
           \en Returns operation result code. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC (MbResultType) CreateSpherePolyhedron( const MbPlacement3D & place, 
                                                       double          radius, 
                                                       double &        epsilon, 
                                                       MbMesh *&       result );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить выпуклую оболочку для множества точек.
           \en Calculate a convex hull of a point set. \~
  \details \ru Вычислить сетку, представляющую выпуклой оболочку для множества точек.
           \en Calculate mesh being a convex hull of a point set. \~
  \param[in] points - \ru Набор точек.
                      \en Point array. \~
  \param[out] result - \ru Результирующая выпуклая триангуляционная сетка.
                       \en The resulting triangulation convex grid. \~
  \return  \ru Возвращает код результата операции.
           \en Returns operation result code. \~
  \ingroup Polygonal_Objects
*/ // ---
// DEPRECATE_DECLARE_REPLACE( CreateConvexPolyhedron with MbConvexHullParams )
MATH_FUNC (MbResultType) CreateConvexPolyhedron( const SArray<MbFloatPoint3D> & points,
                                                       MbMesh *& result );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить выпуклую оболочку для множества точек.
           \en Calculate a convex hull of a point set. \~
  \details \ru Вычислить сетку, представляющую выпуклой оболочку для множества точек.
           \en Calculate mesh being a convex hull of a point set. \~
  \param[in] points - \ru Набор точек.
                      \en Point array. \~
  \param[out] result - \ru Результирующая выпуклая триангуляционная сетка.
                       \en The resulting triangulation convex grid. \~
  \return  \ru Возвращает код результата операции.
           \en Returns operation result code. \~
  \ingroup Polygonal_Objects
*/ // ---
// DEPRECATE_DECLARE_REPLACE( CreateConvexPolyhedron with MbConvexHullParams )
MATH_FUNC( MbResultType ) CreateConvexPolyhedron( const std::vector<MbFloatPoint3D> & points,
                                                  MbMesh *& result );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить выпуклую оболочку для множества точек.
           \en Calculate a convex hull of a point set. \~
  \details \ru Вычислить сетку, представляющую выпуклой оболочку для множества точек.
           \en Calculate mesh being a convex hull of a point set. \~
  \param[in] points - \ru Набор точек.
                      \en Point array. \~
  \param[out] result - \ru Результирующая выпуклая триангуляционная сетка.
                       \en The resulting triangulation convex grid. \~
  \return  \ru Возвращает код результата операции.
           \en Returns operation result code. \~
  \ingroup Polygonal_Objects
*/ // ---
// DEPRECATE_DECLARE_REPLACE( CreateConvexPolyhedron with MbConvexHullParams )
MATH_FUNC( MbResultType ) CreateConvexPolyhedron( const SArray<MbCartPoint3D> & points,
                                                        MbMesh *& result );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить выпуклую оболочку для множества точек.
           \en Calculate a convex hull of a point set. \~
  \details \ru Вычислить сетку, представляющую выпуклой оболочку для множества точек.
           \en Calculate mesh being a convex hull of a point set. \~
  \param[in] points - \ru Набор точек.
                      \en Point array. \~
  \param[out] result - \ru Результирующая выпуклая триангуляционная сетка.
                       \en The resulting triangulation convex grid. \~
  \return  \ru Возвращает код результата операции.
           \en Returns operation result code. \~
  \ingroup Polygonal_Objects
*/ // ---
// DEPRECATE_DECLARE_REPLACE( CreateConvexPolyhedron with MbConvexHullParams )
MATH_FUNC( MbResultType ) CreateConvexPolyhedron( const std::vector<MbCartPoint3D> & points,
                                                        MbMesh *& result );


//------------------------------------------------------------------------------
/** \brief \ru Построить выпуклую оболочку для триангуляционной сетки.
           \en Construct the convex hull of triangulation grid. \~
  \details \ru Построить сетку, представляющую собой выпуклую оболочку для тела,
    заданного его триангуляционной сеткой. По заданному объекту MbMesh
    строится охватывающая его вершины выпуклая триангуляционная сетка.
    Расстояние offset задает смещение точек результирующей сетки относительно
    заданной вдоль нормалей к её граням. Если offset = 0, то результирующая сетка
    будет в точности охватывать все вершины заданной. Смещение по нормали может
    быть как положительным, так и отрицательным (внутрь сетки). Используется для
    определения пересечения с некоторым допуском (offset). \n
           \en Construct the convex hull of triangulation grid. \n \~
  \param[in] mesh - \ru Исходная триангуляционная сетка.
                    \en Initial triangulated mesh. \~
  \param[in] offset - \ru Отступ по нормали для результирующей сетки.
                      \en The offset along a normal for the resulting grid. \~
  \param[out] resMesh - \ru Результирующая выпуклая триангуляционная сетка.
                        \en The resulting triangulation convex grid. \~
  \return  \ru Возвращает код результата операции.
           \en Returns operation result code. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC (MbResultType) CreateConvexPolyhedron( const MbMesh &  mesh, 
                                                       double    offset,
                                                       MbMesh *& result );


//------------------------------------------------------------------------------
/** \brief \ru Построить выпуклую оболочку для набора двумерных точек.
           \en Construct the convex hull of 2D points array. \~
  \details \ru Построить выпуклую оболочку для набора двумерных точек. \n
           \en Construct the convex hull of 2D points array. \n \~
  \param[in] params - \ru Параметры операции.
                      \en Operation parameters. \~
  \param[out] result - \ru Результат операции.
                       \en Operation result. \~
  \return  \ru Возвращает код результата операции.
           \en Returns operation result code. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC( MbResultType ) CreateConvexPolygon( const MbConvexHullParams &  params,
                                                     MbConvexHullResults & result );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить выпуклую оболочку для множества точек.
           \en Calculate a convex hull of a point set. \~
  \details \ru Вычислить сетку, представляющую выпуклой оболочку для множества точек.
           \en Calculate mesh being a convex hull of a point set. \~
  \param[in] params - \ru Параметры операции.
                      \en Operation parameters. \~
  \param[out] result - \ru Результирующая выпуклая триангуляционная сетка.
                       \en The resulting triangulation convex grid. \~
  \return  \ru Возвращает код результата операции.
           \en Returns operation result code. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC( MbResultType ) CreateConvexPolyhedron( const MbConvexHullParams3D & params,
                                                        SPtr<MbMesh> &         result );


//------------------------------------------------------------------------------
/** \brief \ru Определить, пересекаются ли данные выпуклые сетки.
           \en Whether there is intersection of convex grids. \~
  \details \ru Определить, пересекаются ли данные выпуклые оболочки, заданные
    триангуляционными сетками. Пересечение определяется по алгоритму
    Гильберта-Джонсона-Керти (Gilbert-Johnson-Keerthi). Заданные сетки
    равноправны, их последовательность в алгоритме не важна. Сложность
    алгоритма линейная, зависит от количества вершин сеток. \n
           \en Whether there is intersection of convex grids. \n \~
  \param[in] mesh1 - \ru Первая выпуклая триангуляционная сетка.
                     \en The first convex grid. \~
  \param[in] mesh2 - \ru Вторая выпуклая триангуляционная сетка.
                     \en The second convex grid. \~
  \return \ru true - Выпуклые триангуляционные сетки пересекаются.
              false - Выпуклые триангуляционные сетки не пересекаются.
          \en true - true - there is an intersection, 
              false - there are no intersections. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC (bool) AreIntersectConvexPolyhedrons( const MbMesh & mesh1, 
                                                const MbMesh & mesh2 );


//------------------------------------------------------------------------------
/** \brief \ru Отрезать часть полигонального объекта плоскостью.
           \en Cut a part of a polygonal object by a plane. \~
  \details \ru Отрезать часть полигонального объекта плоскостью XY локальной системы координат. \n
    part =  1 - оставляем часть объекта, расположенную сверху плоскости XY локальной системы координат, \n
    part = -1 - оставляем часть объекта, расположенную снизу  плоскости XY локальной системы координат. \n
           \en Cut a part of a polygonal object off by a plane XY of local coordinate system. \n
    part =  1 - a part of polygonal object above the XY plane is to be retained. \n
    part = -1 - a part of polygonal object below the XY plane is to be retained. \n \~
  \param[in] mesh  - \ru Исходный полигональный объект.
                     \en The source polygonal object. \~
  \param[in] sameShell - \ru Режим копирования исходного объекта.
                         \en The mode of copying of the source polygonal object. \~
  \param[in] place - \ru Секущая плоскость.
                     \en A cutting plane. \~
  \param[in] part - \ru Направление отсечения.
                    \en The direction of cutting off. \~
  \param[in] names - \ru Именователь.
                     \en An object for naming the new objects. \~
  \param[in] onlySection - \ru Флаг режима отсечения: false - сечем как тело, true - сечем как оболочку.
                           \en The flag of the cutting off mode: false - cut as a solid, true - cut as a shell. \~
  \param[out] result - \ru Построенный полигональный объект.
                       \en The resultant polygonal object. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC (MbResultType) MeshCutting(       MbMesh &        mesh, 
                                            MbeCopyMode     sameShell,
                                      const MbPlacement3D & place, 
                                            int             part, 
                                      const MbSNameMaker &  names,
                                            bool            onlySection, 
                                            MbMesh *&       result );


//------------------------------------------------------------------------------
/** \brief \ru Построить контур сечения полигонального объекта плоскостью.
           \en Create a section contour of a polygon figure. \~
  \details \ru  Построить контур сечения присланного объекта плоскостью XY локальной системы координат. \n
           \en Construct curves of the section of the mesh object lying on the XY plane of the local coordinate system. \n
  \param[in] mesh  - \ru Исходный полигональный объект.
                     \en The source polygonal object. \~
  \param[in] place - \ru Секущая плоскость.
                     \en A cutting plane. \~
  \param[out] polylines - \ru Построенные ломаные контура сечения объекта.
                          \en The resultant contours. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC( MbResultType ) MeshSection( const MbMesh &                     mesh,
                                       const MbPlacement3D &              place,
                                             c3d::SpaceCurvesSPtrVector & polylines );


//------------------------------------------------------------------------------
/** \brief \ru Построить контур пересечения двух полигональных объектов.
           \en Create an intersection contour of two polygon objects. \~
  \details \ru  Построить контур пересечения двух полигональных объектов. \n
           \en Create an intersection contour of two polygon objects. \n
  \param[in] mesh1  - \ru Исходный полигональный объект.
                      \en The source polygonal object. \~
  \param[in] mesh1 -  \ru Исходный полигональный объект.
                      \en The source polygonal object. \~
  \param[out] polylines - \ru Построенные ломаные контура пересечения полигональных объектов.
                          \en The result contours. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC( MbResultType ) MeshMeshIntersection( const MbMesh &                     mesh1,
                                                const MbMesh &                     mesh2,
                                                      c3d::SpaceCurvesSPtrVector & polylines );


//------------------------------------------------------------------------------
/** \brief \ru Создать полигональный объект булевой операции.
           \en Create a polygon object of Boolean operation. \~
  \details \ru Построить полигональный объект как результат булевой операции над полигональными объектами.
           \en Create a polygon object as a result of Boolean operation on the given polygon objects.
  \param[in]  mesh1         - \ru Первый полигональный объект.
                              \en The first polygon object. \~
  \param[in]  mesh1         - \ru Второй полигональный объект.
                              \en The second polygon object. \~
  \param[in]  operation     - \ru Тип булевой операции.
                              \en A Boolean operation type. \~
  \param[out] newMesh       - \ru Построенный полигональный объект.
                              \en Constructed polygon object. \~
  \result \ru Возвращает код результата операции.
          \en Returns the operation result code. \~
  \ingroup Model_Creators
*/ // ---
MATH_FUNC( MbResultType ) CreateBoolean( const MbMesh &        mesh1, 
                                         const MbMesh &        mesh2,
                                               OperationType   operation,
                                               c3d::MeshSPtr & newMesh );


//------------------------------------------------------------------------------
/** \brief \ru Построить триангуляцию по облаку точек на основе алгоритма поворотного шара.
           \en Build a triangulation by point cloud with Ball Pivoting algorithm. \~
  \param[in]  collection - \ru Коллекция трехмерных элементов.
                           \en Collection of 3d elements. \~
  \param[in]  radius     - \ru Радиус поворотного шара, если radius==0 будет предпринята попытка его автоопределения.
                           \en Radius of the pivoting ball, if radius==0 an autoguess for the ball pivoting radius is attempted \~
  \param[in]  radiusMin  - \ru Радиус кластеризации ( в % от радиуса поворотного шара ).
                           \en Clusterization radius ( % from radius value). \~
  \param[in]  angle      - \ru Максимальный угол между двумя соседними элементами сетки.
                           \en Max angle between two mesh faces \~
  \param[out] result -     \ru Построенный полигональный объект.
                           \en The resultant polygonal object. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC (MbResultType) CalculateBallPivotingGrid( const MbCollection & collection, 
                                                          double         radius,
                                                          double         radiusMin,
                                                          double         angle,
                                                          MbMesh *&      result );

//------------------------------------------------------------------------------
/** \brief  \ru Рассчитать минимизированный по объему параллелепипед (OBB), включающий полигональный объект.
            \en Calculate a volume-minimized oriented bounding box (OBB) containing a polygonal object. \~
            \param[in] mesh \ru Полигональная сетка, для которой рассчитывается ограничивающий объем.
                            \en A polygonal mesh for witch the bounding volume is calculated. \~            
            \return \ru Ограничивающий параллелепипед с ориентацией, выбранной в пользу минимального объема. \~
                    \en Bounding box with orientation chosen for minimum bounding volume.
            \ingroup Polygonal_Objects
*/
// ---
MATH_FUNC(MbOrientedBox) CalculateBoundingBox( const MbMesh & mesh );

//------------------------------------------------------------------------------
/** \brief \ru Исправить несовместную ориентацию полигонов сетки по отношению друг к другу.
           \en Repair inconsistent orientation of mesh polygons with respect to each other. \~
  \param[in] params - \ru Параметры операции.
                      \en Parameters of the operation \~
  \param[in] mesh   - \ru Полигональный объект.
                      \en The polygonal object. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/ // ---
MATH_FUNC (MbResultType) RepairInconsistentMesh( const MbRepairInconsistentMeshParams & params, 
                                                       MbMesh & mesh );


//------------------------------------------------------------------------------
/** \brief \ru Провести поиск по набору объектов и заменить повторяющиеся полигональные объекты (#MbMesh)
               на #MbInstance, которые ссылаются на первый из набора одинаковых.
               Для больших объектов размером более ~1e6 рекомендуется использовать относительную точность.
           \en Search an array of objects to replace repeating polygonal objects (#MbMesh)
               with #MbInstance, which are linked to the first in the set.
               It is recommended to use relative accuracy for large objects which size exceeds ~1e6. \~
  \param[in] meshContainer - \ru Набор объектов.
                             \en The array of objects. \~
  \param[in] params -        \ru Параметры операции.
                             \en Parameters of the operation \~
  \param[out] results -      \ru Результаты операции. Содержат присланный массив, где на месте повторяющихся полигональных объектов расположены вставки.
                             \en Results of the operation. Contain the initial array, where instances instances replace repeatative meshes.\~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC ( MbResultType ) ConvertMeshToInstance( const c3d::ItemsSPtrVector & meshContainer,
                                                  const MbMeshToInstanceParams  & params,
                                                        MbMeshToInstanceResults & results );


//------------------------------------------------------------------------------
/** \brief \ru Определить замкнутость объекта и выдать информацию о граничных ребрах.
           \en Check the object closeness and provide information about the boundary edges. \~
  \param[in] mesh   - \ru Полигональный объект.
                      \en The polygonal object. \~
  \param[out] info  - \ru Структура, содержащая информацию о граничных ребрах (если объект незамкнутый).
                      \en A struct containing information about boundary edges (if the object is not closed). \~
  \return \ru Возвращает true, если объект замкнут.
          \en Returns true the object is closed. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/ // ---
MATH_FUNC( bool ) CheckMeshClosure( const MbMesh & mesh, MeshInfo & info );


//------------------------------------------------------------------------------
/** \brief \ru Определить замкнутость объекта и выдать подробную информацию о границах.
           \en Check the object closeness and provide detailed information about boundaries. \~
  \details \ru Определить замкнутость объекта и выдать подробную информацию о границах. Расширенная версия #CheckMeshClosure.
           \en Check the object closeness and provide detailed information about boundaries. An extended version of #CheckMeshClosure. \~
  \param[in] mesh   - \ru Полигональный объект.
                      \en The polygonal object. \~
  \param[out] info  - \ru Структура, содержащая информацию о границах, если объект незамкнутый.
                      \en A struct containing information about boundaries., if the object is not closed. \~
  \return \ru Возвращает true, если объект замкнут.
          \en Returns true the object is closed. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/ // ---
MATH_FUNC( bool ) InspectMeshClosure( const MbMesh & mesh, MeshInfo & info );


//------------------------------------------------------------------------------
/** \brief \ru Упростить триангуляцию.
           \en Simplify triangulation. \~
  \details \ru Упростить триангуляцию.
           \en Simplify triangulation. \~
  \param[in] mesh   - \ru Полигональный объект.
                      \en The polygonal object. \~
  \param[in] params   - \ru Параметры симплификации.
                        \en Simplifiation parameters. \~
  \param[out] simplifiedMeshes  - \ru Массив упрощенных триангуляций.
                                  \en Vector of simplified triangulations. \~
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC( MbResultType ) SimplifyTriangularMesh( const MbMesh &                     mesh, 
                                                  const MbMeshSimplificationParams & params,
                                                        c3d::MeshesSPtrVector &      simplifiedMeshes );


//------------------------------------------------------------------------------
/** \brief \ru Упростить триангуляцию.
           \en Simplify triangulation. \~
  \details \ru Упростить триангуляцию. Делает упрощение до первого уровня, указанного в параметрах.
           \en Simplify triangulation up to the first level from parameters. \~
  \param[in] params   - \ru Параметры симплификации.
                        \en Simplifiation parameters. \~
  \param[in-out] mesh   - \ru Полигональный объект.
                          \en The polygonal object. \~
 
  \ingroup Polygonal_Objects
*/ // ---
MATH_FUNC( MbResultType ) SimplifyTriangularMesh( const MbMeshSimplificationParams & params,
                                                        MbMesh &                     mesh );


//------------------------------------------------------------------------------
/** \brief \ru Построение развёртки сетки на плоскость.
           \en Construct a unwrapping the mesh on a plane. \~
  \details \ru Построение развёртки сетки на плоскость.
           \en Construct a unwrapping the mesh on a plane. \~
  \param[in] params - \ru Параметры алгоритма. Перед вызовом необходимо у параметров вызвать метод InitBaseMesh и (опционально) InitRemeshedMesh. Если есть ремешированная сетка,
                          работает с ней, иначе с базовой.
                      \en The parameters. It is necessary to call method InitBaseMesh and (optionally) InitRemeshedMesh. If remeshed mesh exists,
                          we will work on it, otherwise on base mesh. \~
  \param[out] results - \ru Результат операции. Содержит в себе копию сетки из параметров, содержащую 2Д параметры, соответствующие развертке.
                        \en The operation result. Contains the copy of the mesh from parameters with filled 2D coordinates in according to unwrapped result.  \~
  \return \ru Код результата операции.
          \en Returns the operation result code. \n
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
MATH_FUNC( MbResultType ) UnwrapMesh( const MbMeshUnwrapParams & params, MbMeshUnwrapResult & results );


//------------------------------------------------------------------------------
/** \brief \ru Совместить объекты.
           \en Align objects. \~
  \details \ru Совместить два объекта: moving к fixed.
           \en Align two objects: moving to fixed. \~
  \param[in] fixed   - \ru Объект, к которому выполняем совмещение.
                       \en Object to which another object is aligned. \~
  \param[in] moving  - \ru Объект, для которого находим трансформацию.
                       \en Object which is being aligned. \~
  \param[in] params  - \ru Параметры совмещения объектов.
                       \en Parameters of object alignment. \~
  \param[out] result - \ru Результат совмещения объектов.
                       \en Result of object alignment. \~
  \return \ru Код результата операции.
          \en Returns the operation result code. \~
  \warning \ru В разработке.
           \en Under development. \~
*/ // ---
MATH_FUNC( MbResultType ) AlignObjects( const MbItem &                  fixed,
                                        const MbItem &                  moving,
                                        const MbObjectAlignmentParams & params,
                                              MbObjectAlignmentResult & result );


//------------------------------------------------------------------------------
/** \brief \ru Класс для драпировки оболочки тканью.
           \en Class for shell draping. \~
  \details \ru Класс для драпировки оболочки тканью. \n
           \en Class for shell draping. \n \~
  \ingroup Polygonal_Objects
  \deprecated
*/
// ---
class MATH_CLASS MbDrapMaker
{
protected:
  MbDrapMaker() {}

public:
  /// \ru Деструктор. \en Destructor.
  virtual ~MbDrapMaker() {}

  /** \brief \ru Получить грань-развертку.
             \en Get unwrapped face. \~
    \details \ru Получить грань-развертку.
                 Ее контур получается путем аппроксимации границы сеточной развертки.
             \en Get unwrapped face.
                 It's contour is calculated by approximation of the unwrapped mesh border.
    \param[in] tolApprox - \ru Точность аппроксимации.
                           \en Approximation tolerance. \~
    \param[out] unwrapped - \ru Грань-развертка.
                            \en Unwrapped face. \~
  */
  virtual void GetUnwrapped( double tolApprox, c3d::FaceSPtr & unwrapped ) const = 0;
  /** \brief \ru Получить полилинии для главной нити основы.
             \en Get polyline for main warp thread. \~
    \details \ru Получить полилинии для главной нити основы.
                 Эти полилинии идут от точки приложения в положительном и отрицательном направлении
                 оси Х локальной системы координат.
             \en Get polyline for main warp thread.
                 Polylines go in the positive and negative direction of the local coordinate system X-axis.
    \param[out] warpPlus - \ru Полилиния в положительном направлении.
                           \en Polyline in the positive direction. \~
    \param[out] warpMinus - \ru Полилиния в отрицательном направлении.
                            \en Polyline in the negative direction. \~
  */
  virtual void GetWarp( c3d::SpacePointsVector & warpPlus, c3d::SpacePointsVector & warpMinus ) const = 0;
  /** \brief \ru Получить полилинии для главной нити утка.
             \en Get polyline for main weft thread. \~
    \details \ru Получить полилинии для главной нити утка.
                 Эти полилинии идут от точки приложения в положительном и отрицательном направлении
                 оси Y локальной системы координат.
             \en Get polyline for main weft thread.
                 Polylines go in the positive and negative direction of the local coordinate system Y-axis.
    \param[out] weftPlus - \ru Полилиния в положительном направлении.
                           \en Polyline in the positive direction. \~
    \param[out] weftMinus - \ru Полилиния в отрицательном направлении.
                            \en Polyline in the negative direction. \~
  */
  virtual void GetWeft( c3d::SpacePointsVector & weftPlus, c3d::SpacePointsVector & weftMinus ) const = 0;
  /// \ru Получить количество сторон ячеек. \en Get cell sides count.
  virtual size_t GetCellSidesCount() const = 0;
  /// \ru Получить полилинию для стороны ячейки. \en Get cell side polyline.
  virtual void GetCellSidePolyline( size_t iSide, c3d::SpacePointsVector & poly ) const = 0;
  /// \ru Получить угол для стороны ячейки. \en Get cell side angle.
  virtual double GetCellSideAngle( size_t iSide ) const = 0;
  /** \brief \ru Получить полилинию разреза ткани на поверхности оболочки.
             \en Get fiber cut polyline on the shell surface. \~
    \details \ru Получить полилинию разреза ткани на поверхности оболочки.
             \en Get fiber cut polyline on the shell surface.
    \param[in] uv - \ru Начало прямой - линии разреза.
                    \en Beginning of the cut line. \~
    \param[in] dir - \ru Направление прямой - линии разреза.
                     \en Direction of the cut line. \~
    \param[out] polys - \ru Полилинии - отображение линии разреза на оболочку.
                        \en Polylines - line mapping to the shell. \~
  */
  virtual void GetSplitPolylines( const MbCartPoint & uv, const MbVector & dir, std::vector<c3d::SpacePointsVector> & polys ) const = 0;

  OBVIOUS_PRIVATE_COPY( MbDrapMaker );
};
#endif // __ACTION_MESH_H
