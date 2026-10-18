////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Сферическая поверхность.
         \en Spherical surface. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_SPHERE_SURFACE_H
#define __SURF_SPHERE_SURFACE_H


#include <surf_elementary_surface.h>


//------------------------------------------------------------------------------
/** \brief \ru Сферическая поверхность.
           \en Spherical surface. \~
  \details \ru Сфера описывается радиусом radius, заданными в локальной системе координат position.\n
    Первый параметр поверхности отсчитывается по дуге от оси position.axisX в направлении оси position.axisY. 
    Первый параметр поверхности u принимает значения на отрезке: umin<=u<=umax. 
    Значения u=0 и u=2pi соответствуют точке на плоскости XZ локальной системы координат. 
    Поверхность может быть замкнутой по первому параметру. 
    У замкнутой поверхности umax-umin=2pi, у не замкнутой поверхности umax-umin<2pi. \n
    Второй параметр поверхности отсчитывается по дуге от плоскости XY локальной системы координат поверхности в направлении оси position.axisZ. 
    Второй параметр поверхности v принимает значения на отрезке: vmin<=v<=vmax. 
    Значение v=0 соответствует точке на плоскости XY локальной системы координат поверхности. 
    Поверхность не замкнута по второму параметру. \n
    Радиус-вектор поверхности описывается векторной функцией \n
    r(u,v) = position.origin + (radius cos(v) (cos(u) position.axisX + sin(u) position.axisY)) + (radius sin(v) position.axisZ). \n
    Радиус сферы должен быть больше нуля: radius>0. 
    Сфера имеет полюсы для параметра v=pi/2 и v=–pi/2. \n
    Для граничных параметров поверхности должны соблюдаться неравенства: umin<umax, vmin<vmax, vmax<=pi/2, vmin>=–pi/2.\n
    Локальная система координат position может быть как правой, так и левой. 
    Если локальная система координат правая, то нормаль направлена наружу сферы, 
    если локальная система координат левая, то нормаль направлена внутрь сферы. \n
           \en A sphere is described by the radius 'radius' given in a local coordinate system 'position'.\n
    The first parameter of a surface is measured by an arc  from the axis 'position.axisX' in direction of the axis 'position.axisY'. 
    The first parameter of a surface u takes values on the segment: umin<=u<=umax. 
    Values u=0 and u=2pi correspond to the point on the plane XZ of a local coordinate system. 
    A surface may be closed in direction of the first parameter. 
    If a surface is closed, then umax-umin=2pi, otherwise umax-umin<2pi. \n
    The second parameter of a surface is measured by an arc from the plane XY of the surface local coordinate system in direction of the axis 'position.axisZ'. 
    The second parameter of a surface v takes values on the segment: vmin<=v<=vmax. 
    The value v=0 corresponds to a point on the plane XY of the surface local coordinate system. 
    A surface is not closed in direction of the second parameter. \n
    Radius-vector of line surface is described by the vector function \n
    r(u,v) = position.origin + (radius cos(v) (cos(u) position.axisX + sin(u) position.axisY)) + (radius sin(v) position.axisZ). \n
    Radius of sphere must be positive: radius>0. 
    A sphere has the poles for the parameters v=pi/2 and v=-pi/2 \n
    The following inequalities must be satisfied for the parameters of surface boundary: umin<umax, vmin<vmax, vmax<=pi/2, vmin>=-pi/2.\n
    Local coordinate system 'position' can be both right and left. 
    If the local coordinate system is right then the normal is directed outside the sphere, 
    if the local coordinate system is left then the normal is directed inside the sphere. \n \~
  \ingroup Surfaces
*/
// ---
class MATH_CLASS MbSphereSurface : public MbElementarySurface {
private:
  double radius;  ///< \ru Радиус сферы. \en Radius of sphere. 
  bool   uclosed; ///< \ru Признак замкнутости по первому параметру u. \en An attribute of closedness in u-parameter direction. 

public:
  /// \ru Конструктор по локальной системе координат и радиусу. \en Constructor by local coordinate system and radius. 
  MbSphereSurface ( const MbPlacement3D & pl, double r );
  
  /** \brief \ru Конструктор по радиусу и локальной системе координат.
             \en Constructor by local coordinate system and radius. \~
    \details \ru Конструктор по радиусу и локальной системе координат.
             \en Constructor by local coordinate system and radius. \~
    \warning \ru Используется только в конвертерах.
             \en This is used only in converters. \~
  */
  MbSphereSurface ( double r, const MbPlacement3D & pl ); // \ru Используется только в конверторах \en This is used only in converters 
  
  /** \brief \ru Конструктор по трем точкам.
             \en Constructor by three points. \~
    \details \ru Конструктор по трем точкам.\n
      Первая точка определяет центр сферической поверхности.\n
      Длина вектора, направленного из первой точки во вторую, равна радиусу сферы,\n
      его направление показывает направление оси Z.
             \en Constructor by three points.\n
      The first point defines the center of spherical surface.\n
      Length of the vector directed from the first point to the second one is equal to the sphere radius,\n
      its direction shows the direction of the axis Z. \~
  */
  MbSphereSurface ( const MbCartPoint3D & c0, const MbCartPoint3D & c1, const MbCartPoint3D & c2 );
  
  /// \ru Конструктор по центру и радиусу. \en Constructor by center and radius. 
  MbSphereSurface ( const MbCartPoint3D & centre, double r );

protected:
  explicit MbSphereSurface ( const MbSphereSurface & );

public:
  virtual ~MbSphereSurface();

public:
  VISITING_CLASS( MbSphereSurface );

  /** \ru \name Функции инициализации
      \en \name Initialization functions
  \{ */
          /// \ru Инициализация по сферической поверхности. \en Initialization by spherical surface. 
  void    Init( const MbSphereSurface & ); 
  /** \} */   
  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */
  MbeSpaceType  IsA() const override; // \ru Тип элемента. \en A type of element. 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Равны ли объекты. \en Whether the objects are equal. 
  bool    SetEqual ( const MbSpaceItem & ) override; // \ru Сделать равным. \en Make equal. 
  void    Transform( const MbMatrix3D & matr, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  
  void    GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта. \en Set properties of the object. 
  /** \} */
  /** \ru \name Функции описания области определения поверхности
      \en \name Functions for surface domain description
      \{ */
  double  GetUMin  () const override;
  double  GetVMin  () const override;
  double  GetUMax  () const override;
  double  GetVMax  () const override;
  bool    IsUClosed() const override;
  bool    IsVClosed() const override;        
  double  GetUPeriod() const override; // \ru Вернуть период для замкнутой функции. \en Return period for a closed function. 
  double  GetVPeriod() const override; // \ru Вернуть период для замкнутой функции. \en Return period for a closed function. 
  /** \} */
  /** \ru \name Функции для работы в области определения поверхности
                Функции PointOn, Derive... поверхностей корректируют параметры
                при выходе их за пределы прямоугольной области определения параметров.
      \en \name Functions for working at surface domain
                Functions PointOn, Derive... correct parameters
                when getting out of rectangular domain bounds.
      \{ */
  void    PointOn  ( double & u, double & v, MbCartPoint3D & ) const override; // \ru Точка на поверхности. \en A point on surface. 
  void    DeriveU  ( double & u, double & v, MbVector3D & ) const override; // \ru Первая производная по u. \en First derivative with respect to u. 
  void    DeriveV  ( double & u, double & v, MbVector3D & ) const override; // \ru Первая производная по v. \en First derivative with respect to v. 
  void    DeriveUU ( double & u, double & v, MbVector3D & ) const override; // \ru Вторая производная по u. \en Second derivative with respect to u. 
  void    DeriveVV ( double & u, double & v, MbVector3D & ) const override; // \ru Вторая производная по v. \en Second derivative with respect to v. 
  void    DeriveUV ( double & u, double & v, MbVector3D & ) const override; // \ru Вторая производная по uv. \en Second derivative with respect to u and v. 
  void    DeriveUUU( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUUV( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUVV( double & u, double & v, MbVector3D & ) const override;
  void    DeriveVVV( double & u, double & v, MbVector3D & ) const override;
  void    Normal   ( double & u, double & v, MbVector3D & ) const override; // \ru Нормаль. \en Normal. 
  void    NormalU  ( double & u, double & v, MbVector3D & ) const override; // \ru Нормаль. \en Normal. 
  void    NormalV  ( double & u, double & v, MbVector3D & ) const override; // \ru Нормаль. \en Normal. 
  void    TangentU ( double & u, double & v, MbVector3D & ) const override;
  /** \} */
  /** \ru \name Функции для работы внутри и вне области определения поверхности
                функции _PointOn, _Derive... поверхностей не корректируют
                параметры при выходе их за пределы прямоугольной области определения параметров.
      \en \name Functions for working inside and outside the surface domain.
                functions _PointOn, _Derive... of surfaces don't correct
                parameters when getting out of rectangular domain bounds.
      \{ */
  void   _PointOn  ( double u, double v, MbCartPoint3D & ) const override; // \ru Точка на расширенной поверхности. \en A point on extended surface. 
  void   _DeriveU  ( double u, double v, MbVector3D & ) const override; // \ru Первая производная по u. \en First derivative with respect to u. 
  void   _DeriveV  ( double u, double v, MbVector3D & ) const override; // \ru Первая производная по v. \en First derivative with respect to v. 
  void   _DeriveUU ( double u, double v, MbVector3D & ) const override; // \ru Вторая производная по u. \en Second derivative with respect to u. 
  void   _DeriveVV ( double u, double v, MbVector3D & ) const override; // \ru Вторая производная по v. \en Second derivative with respect to v. 
  void   _DeriveUV ( double u, double v, MbVector3D & ) const override; // \ru Вторая производная по uv. \en Second derivative with respect to u and v. 
  void   _DeriveUUU( double u, double v, MbVector3D & ) const override;
  void   _DeriveUUV( double u, double v, MbVector3D & ) const override;
  void   _DeriveUVV( double u, double v, MbVector3D & ) const override;
  void   _DeriveVVV( double u, double v, MbVector3D & ) const override;
  void   _Normal   ( double u, double v, MbVector3D & ) const override; // \ru Нормаль. \en Normal. 
  void   _NormalU  ( double u, double v, MbVector3D & ) const override; // \ru Нормаль. \en Normal. 
  void   _NormalV  ( double u, double v, MbVector3D & ) const override; // \ru Нормаль. \en Normal. 
  void   _NormalUU ( double u, double v, MbVector3D & ) const override;
  void   _NormalUV ( double u, double v, MbVector3D & ) const override;
  void   _NormalVV ( double u, double v, MbVector3D & ) const override;
  /** \} */
  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметров поверхности.
      \en \name Functions for get of the group of data inside and outside the surface's domain of parameters.
      \{ */
  void    Explore( double & u, double & v, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer,
                   MbVector3D * uuDer, MbVector3D * vvDer, MbVector3D * uvDer, MbVector3D * nor ) const override;
  void   _PointNormal( double u, double v,
                       MbCartPoint3D & pnt, MbVector3D & deru, MbVector3D & derv,
                       MbVector3D & norm, MbVector3D & noru, MbVector3D & norv,
                       MbVector3D & deruu, MbVector3D & dervv, MbVector3D & deruv ) const override; // \ru Значения производных в точке. \en Values of derivatives at point. 
  /** \} */
  /** \ru \name Функции движения по поверхности
      \en \name Functions of moving on surface
      \{ */
  double  StepU         ( double u, double v, double sag ) const override; // \ru Вычисление шага по u по заданной стрелке прогиба. \en Calculation of the parameter step in u direction by the sag. 
  double  StepV         ( double u, double v, double sag ) const override; // \ru Вычисление шага по v по заданной стрелке прогиба. \en Calculation of the parameter step in v direction by the sag. 
  double  DeviationStepU( double u, double v, double angle ) const override; // \ru Вычисление шага по u по заданному углу отклонения. \en Calculation of the parameter step in u direction by the deviation angle. 
  double  DeviationStepV( double u, double v, double angle ) const override; // \ru Вычисление шага по v по заданному углу отклонения. \en Calculation of the parameter step in v direction by the deviation angle. 
  double  MetricStepU   ( double u, double v, double length ) const override; // \ru Вычисление шага по u по заданной длине. \en Calculation of the parameter step in u direction by the given length. 
  double  MetricStepV   ( double u, double v, double length ) const override; // \ru Вычисление шага по v по заданной длине. \en Calculation of the parameter step in v direction by the given length. 
  size_t  GetUCount() const override;
  size_t  GetVCount() const override;
  /** \} */
  /** \ru \name Общие функции поверхности
      \en \name Common functions of surface
      \{ */
  double  CurvatureU    ( double u, double v ) const override; // \ru Кривизна вдоль u. \en Curvature in u direction. 
  double  CurvatureV    ( double u, double v ) const override; // \ru Кривизна вдоль v. \en Curvature in v direction. 
  // \ru Определение точки касания поверхностей с одним неподвижным параметром. \en Determination of tangency point of surfaces with one fixed parameter. 
  MbeNewtonResult SurfaceTangentNewton( const MbSurface & surf1, MbeParamDir switchPar, double funcEpsilon, size_t iterLimit,
                                        double & u0, double & v0, double & u1, double & v1, bool ext0, bool ext1, VERSION version = Math::DefaultMathVersion() ) const override;
  // \ru Определение точки касания поверхности и кривой. \en Determination of tangency point between a surface and a curve. 
  MbeNewtonResult CurveTangentNewton( const MbCurve3D & curv, double funcEpsilon, size_t iterLimit,
                                      double & u, double & v, double & t, bool ext0, bool ext1 ) const override;

  MbSplineSurface * NurbsSurface( double, double, double, double, bool bmatch = false ) const override; // \ru NURBS копия поверхности. \en NURBS copy of surface. 
  MbSurface *       NurbsSurface( const MbNurbsParameters & uParam, const MbNurbsParameters & vParam ) const override;
  MbSurface *       Offset( double d, bool same ) const override; // \ru Создание эквидистантной поверхности. \en Creation of an offset surface. 

  MbCurve3D * CurveU( double v, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии v = const. \en A spatial copy of the line v = const. 
  MbCurve3D * CurveV( double u, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии u = const. \en A spatial copy of the line u = const. 
  // \ru Найти все проекции точки на поверхность вдоль вектора в любом из двух направлений. \en Find all a point projection onto the surface along a vector in either of two directions. 
  void    DirectPointProjection( const MbCartPoint3D & p, const MbVector3D & vect, SArray<MbCartPoint> & uv, bool ext, MbRect2D * uvRange = nullptr ) const override;
  // \ru Пересечение с кривой. \en Intersection with a curve. 
  void    CurveIntersection( const MbCurve3D & curv, SArray<MbCartPoint> & uv, SArray<double> & tt, 
                                     bool ext0, bool ext, bool touchInclude = false ) const override;

  bool    GetCylinderAxis( MbAxis3D & axis ) const override; // \ru Дать ось вращения для поверхности. \en Get rotation axis of a surface.

  // \ru Подобные ли поверхности для объединения (слива). \en Whether the surfaces are similar to merge. 
  bool    IsSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override;
  bool    IsSpecialSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override;
  // \ru Изменение носимых двумерных кривых (точек) поверхности путем проецирования на совпадающую поверхность. \en Changing of two-dimensional curves (points) of a surface by projection to coincident surface. 
  bool    ProjectCurveOnSimilarSurface( const MbCurve3D & spaceCurve, const MbCurve & curve, const MbSurface & surfNew, MbCurve *& curveNew ) const override;

  // \ru Дать двумерную матрицу преобразования из своей параметрической области в параметрическую область surf. \en Get two-dimensional matrix of transformation from its parametric region to the parametric region of 'surf'. 
  bool    GetMatrixToSurface( const MbSurface & surf, MbMatrix & matr, VERSION version, double precision = METRIC_PRECISION ) const override;
  double  GetFilletRadius( const MbCartPoint3D & p, double accuracy = METRIC_REGION ) const override; // \ru Является ли поверхность скруглением. \en Whether a surface is fillet. 
  MbeParamDir GetFilletDirection( double accuracy = METRIC_REGION ) const override; // \ru Направление поверхности скругления. \en Direction of fillet surface. 
  ThreeStates Salient() const override; // \ru Выпуклая ли поверхность. \en Whether a surface is convex. 
  // \ru Определение разбивки параметрической области поверхности вертикалями и горизонталями. \en Determine a splitting of parametric region of a surface by verticals and horizontals. 
  void    GetTesselation( const MbStepData & stepData,
                          double u1, double u2, double v1, double v2,
                          SArray<double> & uu, SArray<double> & vv ) const override;
  void    CalculateGabarit( MbCube & ) const override; // \ru Выдать габарит. \en Get bounding box. 
  void    CalculateLocalGabarit( const MbMatrix3D &, MbCube & ) const override; // \ru Рассчитать габарит относительно л.с.к. \en Calculate bounding box relative to the local coordinate system. 

  bool    SetLimit( double u1, double v1, double u2, double v2 ) override;
  void    SetExtendedParamRegion( double u1, double v1, double u2, double v2 ) override;
  void    IncludePoint( double u, double v ) override; // \ru Включить точку в область определения. \en Include a point into domain. 
  // \ru Существует ли полюс на границе параметрической области. \en Whether a pole exists on parametric region boundary. 
  bool    GetPoleVMin() const override;
  bool    GetPoleVMax() const override;
  bool    IsPole( double u, double v, double paramPrecision = PARAM_PRECISION ) const override; // \ru Является ли точка особенной. \en Whether the point is singular. 

  size_t  GetUMeshCount() const override; // \ru Выдать количество полигонов по u. \en Get the number of polygons in u-direction. 
  size_t  GetVMeshCount() const override; // \ru Выдать количество полигонов по v. \en Get the number of polygons in v-direction. 

  double  GetUParamToUnit() const override; // \ru Дать приращение параметра u, соответствующее единичной длине в пространстве. \en Get increment of u-parameter, corresponding to the unit length in space. 
  double  GetVParamToUnit() const override;  // \ru Дать приращение параметра v, соответствующее единичной длине в пространстве. \en Get increment of v-parameter, corresponding to the unit length in space. 
  double  GetUParamToUnit( double u, double v ) const override; // \ru Дать приращение параметра u, соответствующее единичной длине в пространстве. \en Get increment of u-parameter, corresponding to the unit length in space. 
  double  GetVParamToUnit( double u, double v ) const override; // \ru Дать приращение параметра v, соответствующее единичной длине в пространстве. \en Get increment of v-parameter, corresponding to the unit length in space. 
  void    GetParamsToUnit( double u, double v, double & uParam, double & vParam ) const override; // \ru Дать приращение параметра u и параметра v, соответствующее единичной длине в пространстве. \en Get increment of parameters, corresponding to the unit length in space.  

  double  GetRadius() const override; // \ru Дать физический радиус объекта или ноль, если это невозможно. \en Get the physical radius of the object or null if it impossible.
  /// \ru Выдать центр сферической поверхности. \en Give the center of sphere surface. 
  bool    GetCentre( MbCartPoint3D & c ) const override;
          
  // \ru Является ли объект смещением. \en Is the object a shift? 
  bool    IsShift  ( const MbSpaceItem &, MbVector3D &, bool & isSame, double accuracy = LENGTH_EPSILON ) const override;
  /** \} */  
  /** \ru \name Функции элементарных поверхностей
      \en \name Functions of elementary surfaces.
      \{ */
  bool    GetPointProjection( const MbCartPoint3D & p, bool init, double & u, double & v, bool ext, MbRect2D * uvRange = nullptr ) const override;
  /** \} */  
  /** \ru \name Функции конической поверхности
      \en \name Functions of conical surface
      \{ */

  /// \ru Дать внутренний радиус. \en Get inner radius. 
  double  GetR() const { return radius; } 
  /// \ru Выдать радиус параллели, соответствующей 'V'. \en Get the radius of parallel corresponding to 'V'. 
  double  GetR( double v ) const { return radius * ::cos(v); }
  /// \ru Изменение внутреннего радиуса. \en Changing of inner radius. 
  void    SetR( double r ) { radius = r; SetDirtyGabarit(); } 
  /// \ru Являются ли сферы пространственно идентичными. \en Whether surfaces are spatially identical. 
  bool    IsSpaceSame( const MbSpaceItem &, double eps ) const; 
  /** \} */ 

private:
  inline  void    CheckParam( double &u, double &v ) const; // \ru Проверка параметров \en Check parameters 
  inline  void    CheckParam( double &v ) const; 
          // \ru Пересечение с прямолинейной кривой \en Intersection with rectilinear curve 
  bool    StraightIntersection( const MbCurve3D & curv, SArray<MbCartPoint> & uv, SArray<double> & tt, bool ext0, bool ext ) const;
  void    operator = ( const MbSphereSurface & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSphereSurface )
};

IMPL_PERSISTENT_OPS( MbSphereSurface )


//------------------------------------------------------------------------------
// \ru Проверить параметры \en Check parameters 
// ---
inline void MbSphereSurface::CheckParam( double & u, double & v ) const
{
  if ( (u < umin) || (u > umax) ) {
    if ( uclosed ) 
      u -= ::floor( (u - umin) * Math::invPI2 ) * M_PI2;
    else if ( u < umin )
      u = umin;
    else if ( u > umax )
      u = umax;
  }
  if ( v < vmin )
    v = vmin;
  else if ( v > vmax )
    v = vmax;
}


//------------------------------------------------------------------------------
// \ru Проверить параметр \en Check parameter 
// ---
inline void MbSphereSurface::CheckParam( double &v ) const
{
  if ( v < -M_PI_2 )
    v = -M_PI_2;
  else if ( v > M_PI_2 )
    v = M_PI_2;
}


#endif // __SURF_SPHERE_SURFACE_H
