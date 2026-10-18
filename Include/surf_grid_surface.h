////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Поверхность на базе триангуляции.
         \en Surface based on triangulation. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_GRID_SURFACE_H
#define __SURF_GRID_SURFACE_H


#include <templ_p_array.h>
#include <surface.h>
#include <cur_polyline.h>
#include <mesh_triangle.h>
#include <mb_operation_result.h>
#include <vector>


class    MbCurve3D;
class    MbMesh;
class    MbGrid;


#define   _C3D_3_   3
#define   _C3D_4_   4

  
//------------------------------------------------------------------------------
/// \ru Треугольная пластина поверхности на сетке точек. \en Triangular plate. 
// ---
class MbTrigon {
protected :
  size_t index[_C3D_3_];     ///< \ru Номера вершин треугольника в массиве точек. \en The numbers of vertices in points array.
  size_t neighbour[_C3D_3_]; ///< \ru Номера соседних треугольников. \en The numbers of neighbour triangles. SYS_MAX_T if it is absent.

/*                   points[index[2]]
//                          +
//                        /   \
//        neighbour[2]  /       \  neighbour[1]
//                    /           \
// points[index[0]] +---------------+ points[index[1]]
//                    neighbour[0]
*/

public :
  MbTrigon() { index[0] = SYS_MAX_T; index[1] = SYS_MAX_T; index[2] = SYS_MAX_T; 
    neighbour[0] = SYS_MAX_T; neighbour[1] = SYS_MAX_T; neighbour[2] = SYS_MAX_T; }
  MbTrigon( size_t i0, size_t i1, size_t i2 ) { index[0] = i0; index[1] = i1; index[2] = i2; 
    neighbour[0] = SYS_MAX_T; neighbour[1] = SYS_MAX_T; neighbour[2] = SYS_MAX_T; }
  MbTrigon( const MbTrigon  & init ) { index[0] = init.index[0]; index[1] = init.index[1]; index[2] = init.index[2];
    neighbour[0] = init.neighbour[0]; neighbour[1] = init.neighbour[1]; neighbour[2] = init.neighbour[2]; }
  MbTrigon( const MbTriangle & init ) { 
    uint i0, i1, i2;
    init.GetTriangle( i0, i1, i2 );
    index[0]     = i0;        index[1]     = i1;        index[2]     = i2;
    neighbour[0] = SYS_MAX_T; neighbour[1] = SYS_MAX_T; neighbour[2] = SYS_MAX_T; }
public :
  void        Init( size_t i0, size_t i1, size_t i2, bool orientation ) {
    if ( orientation ) { index[0] = i0; index[1] = i1; index[2] = i2; }
    else               { index[0] = i0; index[1] = i2; index[2] = i1; }
    neighbour[0] = SYS_MAX_T; neighbour[1] = SYS_MAX_T; neighbour[2] = SYS_MAX_T; 
  }
  void        Init( size_t i0, size_t i1, size_t i2, size_t n0, size_t n1, size_t n2 ) {
    index[0]     = i0; index[1]     = i1; index[2]     = i2;
    neighbour[0] = n0; neighbour[1] = n1; neighbour[2] = n2; 
  }
  void        GetTriangle( size_t & i0, size_t & i1, size_t & i2 ) const { i0 = index[0]; i1 = index[1]; i2 = index[2]; }
  void        GetTriangle( size_t & i0, size_t & i1, size_t & i2, size_t & n0, size_t & n1, size_t & n2 ) const {
    i0 = index[0];     i1 = index[1];     i2 = index[2];
    n0 = neighbour[0]; n1 = neighbour[1]; n2 = neighbour[2]; 
  }
  size_t      GetNunber( size_t & i ) const { return index[i % _C3D_3_]; }
  size_t      GetNeihbour( size_t & i ) const { return neighbour[i % _C3D_3_]; }
  // \ru Инициализация соседа. \en The neighbour initiation.
  void        SetNeihbour( size_t i, size_t n ) { neighbour[i % _C3D_3_] = n; }
  MbTrigon & operator = ( const MbTrigon & init ) { init.GetTriangle( index[0], index[1], index[2] );
    neighbour[0] = init.neighbour[0]; neighbour[1] = init.neighbour[1]; neighbour[2] = init.neighbour[2];
    return *this; }
  MbTrigon & operator = ( const MbTriangle & init ) { 
    uint i0, i1, i2;
    init.GetTriangle( i0, i1, i2 );
    index[0]     = i0;        index[1]     = i1;        index[2]     = i2;
    neighbour[0] = SYS_MAX_T; neighbour[1] = SYS_MAX_T; neighbour[2] = SYS_MAX_T; 
    return *this; }
}; // MbTrigon


//------------------------------------------------------------------------------
/** \brief \ru Поверхность на базе триангуляции.
           \en Surface based on triangulation. \~
  \details \ru Поверхность на базе триангуляции образована криволинейными треугольниками, 
    гладко стыкующимися между собой по общим сторонам.
    В общих вершинах стыкующиеся треугольники имеют общую нормаль.
    Сторону треугольников изменяются по кубическому закону. \n
           \en Surface based on triangulation is formed by curvilinear triangles 
    which are smoothly connected together through common edges.
    Connected triangles have common normal at common vertices.
    Edges of triangles are changed by cubic law. \n \~
  \ingroup Surfaces
*/
// ---
class MATH_CLASS MbGridSurface : public MbSurface {

private:
  // \ru Согласованные между собой множества данных в вершинах. \en Sets of data at vertices matched each other.  
  std::vector<MbCartPoint>      params;      ///< \ru Множество точек на параметрической области поверхности. \en Set of points in parametric space of surface. 
  std::vector<MbCartPoint3D>    points;      ///< \ru Множество точек поверхности. \en Set of points of surface. 
  std::vector<MbVector3D>       normals;     ///< \ru Множество нормалей поверхности. \en Set of normals of surface. 
  std::vector<MbTrigon>         triangles;   ///< \ru Множество треугольников. \en Set of triangles. 
  // \ru Описание области параметров поверхности. \en Description of surface parameters region.  
  size_t                        uCount;      ///< \ru Количество разбиений области по u. \en Count of splittings of region by u. 
  size_t                        vCount;      ///< \ru Количество разбиений области по v. \en Count of splittings of region by v. 
  PArray< std::vector<size_t> > cell;        ///< \ru Сетка области параметров поверхности. \en Grid of surface parameters region. 
  std::vector<MbPolyline *>     boundary;    ///< \ru Граничные кривые области параметров поверхности. \en Boundary curves of surface parameters region. 
  double                        umin;        ///< \ru Минимальное  значение параметра u. \en Minimal value of parameter u. 
  double                        vmin;        ///< \ru Минимальное  значение параметра v. \en Minimal value of parameter v. 
  double                        umax;        ///< \ru Максимальное значение параметра u. \en Maximal value of parameter u. 
  double                        vmax;        ///< \ru Максимальное значение параметра v. \en Maximal value of parameter v. 
  bool                          uclosed;     ///< \ru Признак замкнутости по параметру u. \en Attribute of closedness by parameter u. 
  bool                          vclosed;     ///< \ru Признак замкнутости по параметру v. \en Attribute of closedness by parameter v. 

private:
  /// \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbGridSurface( const MbGridSurface & init );
  MbGridSurface( const MbGridSurface & init, MbRegDuplicate * iReg );
protected:
  /// \ru Конструктор поверхности. \en Constructor of surface. 
  template <class Params, class Points, class Normals, class Triangles, class Bounds>
  MbGridSurface ( const Params      & _params 
                , const Points      & _points
                , const Normals     & _normals 
                , const Triangles   & _triangles
                , const Bounds      & _bounds)
    : MbSurface  ()
    , params     ()
    , points     ()
    , normals    ()
    , triangles  ()
    , uCount( 1 )
    , vCount( 1 )
    , cell( 0, 1, true)
    , boundary ()
    , umin     ( 0.0 )
    , vmin     ( 0.0 )
    , umax     ( 0.0 )
    , vmax     ( 0.0 )
    , uclosed  ( false )
    , vclosed  ( false )
  {
    size_t k, cnt;

    cnt = _params.size();
    params.reserve( cnt );
    for ( k = 0; k < cnt; k++ )
      params.push_back( _params[k] );
    cnt = _points.size();
    points.reserve( cnt );
    for ( k = 0; k < cnt; k++ )
      points.push_back( _points[k] );

    if ( _normals.size() > 0 ) {
      size_t normalsLast = _normals.size() - 1;
      normals.reserve( cnt );
      for ( k = 0; k < cnt; k++ ) {
        size_t ind = std_min( k, normalsLast );
        normals.push_back( _normals[ind] );
      }
    }
    cnt = _triangles.size();
    triangles.reserve( cnt );
    for ( k = 0; k < cnt; k++ )
      triangles.push_back( _triangles[k] );

    cnt = _bounds.size();
    boundary.reserve(cnt);
    for (k = 0; k < cnt; k++) {
      _bounds[k]->AddRef();
      boundary.push_back(_bounds[k]);
    }

    Init( boundary.size() == 0 );
  }

public:
  virtual ~MbGridSurface();

public:
  VISITING_CLASS( MbGridSurface );

  /** \name Общие функции геометрического объекта 
      \{ */
  // \ru Общие функции геометрического объекта \en Common functions of a geometric object  
  MbeSpaceType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Make a copy of element
  bool    IsSame( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override;
  bool    SetEqual( const MbSpaceItem &init ) override;        // \ru Сделать равным \en Make equal
  bool    IsSimilar( const MbSpaceItem &init ) const override;         // \ru Являются ли объекты подобными \en Whether the objects are similar
  void    Transform( const MbMatrix3D &matr, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void    Move( const MbVector3D &to, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void    Rotate( const MbAxis3D &axis, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis
  double  DistanceToPoint( const MbCartPoint3D & ) const override;   // \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 

  void    GetProperties( MbProperties &properties ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void    SetProperties( const MbProperties &properties ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  void    Refresh() override; // \ru Сбросить все временные данные. \en Reset all temporary data. 
  /** \} */

  /** \name Функции описания области определения поверхности
      \{ */
  // \ru Функции описания области определения поверхности. \en Functions for surface domain description. 
  double  GetUMin() const override;
  double  GetVMin() const override;
  double  GetUMax() const override;
  double  GetVMax() const override;
  bool    IsUClosed() const override; // \ru Замкнута ли поверхность по параметру u. \en Whether the surface is closed by parameter u. 
  bool    IsVClosed() const override; // \ru Замкнута ли поверхность по параметру v. \en Whether the surface is closed by parameter v. 
  /** \} */
  
  /** \name Функции для работы в области определения поверхности
            Функции PointOn, Derive... поверхностей корректируют параметры
            при выходе их за пределы прямоугольной области определения параметров.\n
      \{ */
  // \ru Функции для работы в области определения поверхности. \en Functions for working at surface domain. 
  void    PointOn  ( double & u, double & v, MbCartPoint3D & p ) const override; // \ru Точка на поверхности \en Point on the surface 
  void    DeriveU  ( double & u, double & v, MbVector3D & der ) const override; // \ru Первая производная по u \en First derivative with respect to u
  void    DeriveV  ( double & u, double & v, MbVector3D & der ) const override; // \ru Первая производная по v \en First derivative with respect to v
  void    DeriveUU ( double & u, double & v, MbVector3D & der ) const override; // \ru Вторая производная по u \en Second derivative with respect to u
  void    DeriveVV ( double & u, double & v, MbVector3D & der ) const override; // \ru Вторая производная по v \en Second derivative with respect to v
  void    DeriveUV ( double & u, double & v, MbVector3D & der ) const override; // \ru Вторая производная по uv \en Second derivative with respect to uv
  void    DeriveUUU( double & u, double & v, MbVector3D & der ) const override; // \ru Третья производная \en Third derivative
  void    DeriveUUV( double & u, double & v, MbVector3D & der ) const override; // \ru Третья производная \en Third derivative
  void    DeriveUVV( double & u, double & v, MbVector3D & der ) const override; // \ru Третья производная \en Third derivative
  void    DeriveVVV( double & u, double & v, MbVector3D & der ) const override; // \ru Третья производная \en Third derivative
  /** \} */
  
  /** \name Функции для работы внутри и вне области определения поверхности
            функции _PointOn, _Derive... поверхностей не корректируют
            параметры при выходе их за пределы прямоугольной области определения параметров.
            За пределами параметрической области поверхность продолжается по касательной.
      \{ */
  // \ru Функции для работы внутри и вне области определения поверхности. \en Functions for working inside and outside the surface's domain. 
  void   _PointOn  ( double  u, double  v, MbCartPoint3D & ) const override; // \ru Точка на расширенной поверхности. \en The point on the extended surface. 
  void   _DeriveU  ( double  u, double  v, MbVector3D & ) const override; // \ru Первая производная по u. \en The first derivative with respect to u. 
  void   _DeriveV  ( double  u, double  v, MbVector3D & ) const override; // \ru Первая производная по v. \en The first derivative with respect to v. 
  void   _DeriveUU ( double  u, double  v, MbVector3D & ) const override; // \ru Вторая производная по u. \en The second derivative with respect to u. 
  void   _DeriveVV ( double  u, double  v, MbVector3D & ) const override; // \ru Вторая производная по v. \en The second derivative with respect to v. 
  void   _DeriveUV ( double  u, double  v, MbVector3D & ) const override; // \ru Вторая производная. \en The second derivative. 
  void   _DeriveUUU( double  u, double  v, MbVector3D & ) const override; // \ru Третья производная. \en The third derivative. 
   void   _DeriveUUV( double  u, double  v, MbVector3D & ) const override; // \ru Третья производная. \en The third derivative. 
  void   _DeriveUVV( double  u, double  v, MbVector3D & ) const override; // \ru Третья производная. \en The third derivative. 
  void   _DeriveVVV( double  u, double  v, MbVector3D & ) const override; // \ru Третья производная. \en The third derivative. 
  /** \} */

  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметров поверхности.
      \en \name Functions for get of the group of data inside and outside the surface's domain of parameters.
      \{ */
  void    Explore( double & u, double & v, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer,
                   MbVector3D * uuDer, MbVector3D * vvDer, MbVector3D * uvDer, MbVector3D * nor ) const override;
  /** \} */
  // \ru Функции движения по поверхности \en Functions of moving along the surface 
  double  StepU( double u, double v, double sag ) const override; // \ru Вычисление шага аппроксимации с учетом радиуса кривизны по U \en Calculation of the approximation step with consideration of the curvature radius by U 
  double  StepV( double u, double v, double sag ) const override; // \ru Вычисление шага аппроксимации с учетом радиуса кривизны по V \en Calculation of the approximation step with consideration of the curvature radius by V 
  double  DeviationStepU( double u, double v, double ang ) const override; // \ru Вычисление шага параметра u по углу отклонения нормали \en Calculation of parameter u step by the angle of deviation of normal 
  double  DeviationStepV( double u, double v, double ang ) const override; // \ru Вычисление шага параметра v по углу отклонения нормали \en Calculation of parameter v step by the angle of deviation of normal 
  size_t  GetUCount() const override;
  size_t  GetVCount() const override;
  
  // \ru Выдать граничную точку \en Get the boundary point 
  void    GetLimitPoint( ptrdiff_t num, MbCartPoint3D & ) const override; // \ru Выдать граничную трехмерную точку. \en Get the three-dimensional boundary point.
  void    GetLimitPoint( ptrdiff_t num, MbCartPoint & )   const override; // \ru Выдать граничную двумерную точку (граничные параметры). \en Get the two-dimensional boundary point (boundary parameters).

  MbeItemLocation PointClassification( const MbCartPoint &, bool ignoreClosed = false ) const override; // \ru Находится ли точка в области, принадлежащей поверхности. \en Whether the point is in region belonging to the surface.
  double  DistanceToBorder   ( const MbCartPoint &, double & eps ) const override; // \ru Параметрическое расстояние до ближайшей границы. \en Parametric distance to the nearest boundary.
  // \ru Определение точек пересечения кривой с контурами поверхности. \en Determine intersection points of a curve with the contours on the surface. 
  size_t  CurveClassification( const MbCurve & curve, SArray<double> & tcurv, SArray<MbVector> & dir ) const override;

  // \ru Найти ближайшую проекцию точки на поверхность. \en Find the nearest projection of a point onto the surface. 
  bool    NearPointProjection( const MbCartPoint3D & p, double & u, double & v, bool ext, MbRect2D * uvRange = nullptr ) const override; // \ru Ближайшая проекция точки на поверхность. \en The nearest point projection onto the surface.
  // \ru Найти все проекции точки на поверхность вдоль вектора в любом из двух направлений. \en Find all a point projection onto the surface along a vector in either of two directions. 
  void    DirectPointProjection( const MbCartPoint3D & p, const MbVector3D & vect, SArray<MbCartPoint> & uv, bool ext, MbRect2D * uvRange = nullptr ) const override;
  // \ru Все точки пересечения поверхности и кривой. \en All the points of intersection of a surface and a curve. 
  void    CurveIntersection( const MbCurve3D & curv, SArray<MbCartPoint> & uv, SArray<double> & tt, 
                                     bool ext0, bool ext, bool touchInclude = false ) const override;
  // \ru Расчёт площади области определения параметров. \en Calculate area of parameter domain. 
  double  ParamArea() const override;
  size_t  GetUPairs( double v, SArray<double> & u ) const override; // \ru Вычислить U-пары от V. \en Calculate U-pairs by V.
  size_t  GetVPairs( double u, SArray<double> & v ) const override; // \ru Вычислить V-пары от U. \en Calculate V-pairs by U.
  void    CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.
  void    CalculateSurfaceWire( const MbStepData & stepData, size_t beg,  MbMesh & mesh,
                                        size_t uMeshCount = c3d::WIRE_MAX, size_t vMeshCount = c3d::WIRE_MAX ) const override;   // \ru Рассчитать сетку. \en Calculate mesh.
  // \ru Аппроксимация поверхности треугольными пластинами. \en Approximation of a surface by triangular plates. 
  void    CalculateSurfaceGrid( const MbStepData & stepData, bool sense, MbGrid & grid ) const override;
  // \ru Определение разбивки параметрической области поверхности вертикалями и горизонталями. \en Determine splitting of parametric region of surface by vertical and horizontal lines. 
  void    GetTesselation( const MbStepData & stepData,
                          double u1, double u2, double v1, double v2,
                          SArray<double> & uu, SArray<double> & vv ) const override;
  // \ru Пересчитать нормали в вершинах. \en Normals Calculation on vertex. 
  virtual void    Normalize();

  size_t  GetBoundariesCount() const { return boundary.size(); } // \ru Выдать количество граничных двумерных кривых. \en Get the two-dimensional boundary curves count. 
  MbContour & MakeContour( bool sense ) const override; // \ru Выдать граничных двумерный контур. \en Get the two-dimensional boundary contour.
  MbCurve & MakeSegment( size_t i, bool sense ) const override; // \ru Дать граничную двумерную кривую. \en Get the two-dimensional boundary curve. 

  /// \ru Инициализация объекта по другому такому же. \en Initialization of a object by same other object.
  void    Init( const MbGridSurface & init );

  /// \ru Выдать количество точек. \en Get the number of points. 
  size_t  PointsCount()  const { return points.size();  }
  /// \ru Выдать количество нормалей. \en Get the number of normals. 
  size_t  NormalsCount() const { return normals.size(); }
  /// \ru Выдать количество параметров. \en Get the number of parameters. 
  size_t  ParamsCount()  const { return params.size();  } 
  // \ru Выдать количество треугольников. \en Get the number of triangles. 
  size_t  TrianglesCount() const { return triangles.size(); }
  // \ru Выдать количество граничных кривых. \en Get the number of boundary curves. 
  size_t  BoundariesCount() const { return boundary.size(); }

  // \ru Добавить в контейнер параметры в опорных точках поверхности. \en Get the parameters to container. 
  void    GetParams( std::vector<MbCartPoint> & paramsVector )  const;
  // \ru Добавить в контейнер опорные точки. \en Get the points to container. 
  void    GetPoints( std::vector<MbCartPoint3D> & pointsVector )  const;
  // \ru Добавить в контейнер нормали в опорных точках. \en Add the normals to container. 
  void    GetNormals( std::vector<MbVector3D> & normalsVector ) const;
  // \ru Добавить в контейнер треугольники. \en Add the triangles to container. 
  void    GetTriangles( std::vector<MbTriangle> & tVector ) const;
  // \ru Добавить в контейнер треугольники. \en Add the triangles to container. 
  void    GetTriangles( std::vector<MbTrigon> & tVector ) const;
  // \ru Добавить в контейнер граничные кривые. \en Add the boundary curves of surface parameters region. 
  void    GetBoundaries( std::vector<const MbPolyline *> & bVector ) const;

  /// \ru Создание поверхности. \en Creatying of surface. 
  template <class Params, class Points, class Normals, class Triangles, class Bounds>
  static  MbGridSurface * Create( const Params    & _params 
                                , const Points    & _points
                                , const Normals   & _normals 
                                , const Triangles & _triangles
                                , const Bounds    & _bounds )
  {
    MbGridSurface * surface = nullptr;

    const size_t itemsCnt = _params.size();

    if ( (itemsCnt > 2) && (itemsCnt == _points.size()) && (_triangles.size() > 0) ) {
      if ( (itemsCnt == _normals.size()) || (_normals.size() == 1) )
        surface = new MbGridSurface( _params, _points, _normals, _triangles, _bounds );
    }
    return surface;
  }

private:
  /// \ru Инициализация. \en Initialization.
  void    Init( bool bound = true );
  // \ru Инициализация граничных кривых. \en Initialization of boundary curves. 
  void    MakeBoundary(); 
  // \ru Выдать треугольник. \en Get triangle. 
  MbTrigon & GetTriangle( size_t i ) { return triangles[i]; }
  void    PointOn  ( double & u, double & v, bool ext, MbCartPoint3D & p ) const; // \ru Точка на поверхности \en Point on the surface 
  void    DeriveU  ( double & u, double & v, bool ext, MbVector3D & der ) const; // \ru Первая производная по u \en First derivative with respect to u 
  void    DeriveV  ( double & u, double & v, bool ext, MbVector3D & der ) const; // \ru Первая производная по v \en First derivative with respect to v 
  void    DeriveUU ( double & u, double & v, bool ext, MbVector3D & der ) const; // \ru Вторая производная по u \en Second derivative with respect to u 
  void    DeriveVV ( double & u, double & v, bool ext, MbVector3D & der ) const; // \ru Вторая производная по v \en Second derivative with respect to v 
  void    DeriveUV ( double & u, double & v, bool ext, MbVector3D & der ) const; // \ru Вторая производная по uv \en Second derivative with respect to uv 
  void    DeriveUUU( double & u, double & v, bool ext, MbVector3D & der ) const; // \ru Третья производная \en Third derivative 
  void    DeriveUUV( double & u, double & v, bool ext, MbVector3D & der ) const; // \ru Третья производная \en Third derivative 
  void    DeriveUVV( double & u, double & v, bool ext, MbVector3D & der ) const; // \ru Третья производная \en Third derivative 
  void    DeriveVVV( double & u, double & v, bool ext, MbVector3D & der ) const; // \ru Третья производная \en Third derivative 
  // \ru Выставить взаимные связи триангуляции. \en Set mutual connections of triangulation. 
  bool    SetTrigonNeihbours();
  // \ru Поиск ближайшего треугольника для инициализации данных ячейки. \en Search nearest triangle for initialization of data of cell. 
  void    FindNearest( size_t i, size_t j, std::vector<size_t> & indecies, 
                       double uDelta, double vDelta, double u, double v );
  // \ru Добавить ближайший треугольник в ячейку. \en Add nearest triangle to cell. 
  bool    AddNearest( size_t i, size_t j, size_t ind );
  // \ru Вычислить индекс ближайшего треугольника и барицентрические координаты точки для него. \en Calculate barycentric coordinates of the nearest trianle.
  size_t  FindIndex( const double & u, const double & v, double & a, double & b, double & c, double & d ) const;
  // \ru Расстояние до треугольника. \en The distance to a triangle. 
  double  RangeToTriangle( size_t ind, const double & u, const double & v, double eps, 
                           double & a, double & b, double & c, double & d ) const;
  // \ru Расстояние до треугольника. \en The distance to a triangle. 
  double  DistanceToTriangle( size_t ind, const double & u, const double & v, double eps, 
                              MbCartPoint & p ) const;
  // \ru Проверка параметров. \en Check parameters. 
  void    CheckParam( double & u, double & v ) const;
  // \ru Выдать данные триангуляции. \en Get triangulation data.
  void    GetTriangleData( size_t tIndex,
                            size_t & index1, size_t & index2, size_t & index3,
                            size_t & neigh1, size_t & neigh2, size_t & neigh3,
                            MbCartPoint & param1, MbCartPoint & param2, MbCartPoint & param3, 
                            MbCartPoint3D & point1, MbCartPoint3D & point2, MbCartPoint3D & point3, 
                            MbVector3D & normal1, MbVector3D & normal2, MbVector3D & normal3 ) const;
  // \ru Выдать данные триангуляции соседнего треугольника. \en Get neighbour triangulation data.
  bool    GetNeighbourData( double u, double v, 
                            size_t neigh1, size_t neigh2, size_t neigh3,
                            double & aCalc, double & bCalc, double & cCalc, double & deter, double & portion, 
                            MbCartPoint & param1, MbCartPoint & param2, MbCartPoint & param3,
                            MbCartPoint3D & point1, MbCartPoint3D & point2, MbCartPoint3D & point3, 
                            MbVector3D & normal1, MbVector3D & normal2, MbVector3D & normal3 ) const;
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void    operator = ( const MbGridSurface & ); 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbGridSurface )
}; // MbGridSurface

IMPL_PERSISTENT_OPS( MbGridSurface )


#endif // __SURF_GRID_SURFACE_H
