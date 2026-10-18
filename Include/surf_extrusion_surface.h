////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Поверхность выдавливания.
         \en Extrusion surface. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_EXTRUSION_SURFACE_H
#define __SURF_EXTRUSION_SURFACE_H


#include <surf_swept_surface.h>


class   MbLine3D;
class   MbOffsetSurface;


//------------------------------------------------------------------------------
/** \brief \ru Поверхность выдавливания.
           \en Extrusion surface. \~
  \details \ru Поверхность выдавливания является кинематической поверхностью с прямолинейной направляющей. 
     Радиус-вектор поверхности описывается векторной функцией \n
    r(u,v) = curve(u) + (direction distance v). \n
    Первый параметр поверхности совпадает с параметром образующей кривой.
           \en Extrusion surface is swept surface with a rectilinear directrix. 
     Radius-vector of surface is described by the vector function \n
    r(u,v) = curve(u) + (direction distance v). \n
    First parameter of surface coincides with parameter of generating curve. \~ 
  \ingroup Surfaces
*/ 
// ---
class MATH_CLASS MbExtrusionSurface : public MbSweptSurface {
protected:
  MbVector3D direction; ///< \ru Направление выдавливания, единичный вектор. \en Direction of extrusion, vector of unit length. 
  double     distance;  ///< \ru Длина выдавливания. \en Length of extrusion. 

public:
  /** \brief \ru Конструктор по образующей и направлению выдавливания.
             \en Constructor by generating curve and direction of extrusion. \~
    \details \ru Конструктор по образующей и направлению выдавливания.
             \en Constructor by generating curve and direction of extrusion. \~
    \param[in] curve - \ru Образующая поверхности выдавливания
                       \en Generating curve of extrusion surface \~
    \param[in] vector - \ru Направление выдавливания
                        \en Direction of extrusion \~
    \param[in] same - \ru Использование оригинала кривой, а не ее копии
                      \en Usage of original of curve, not a copy \~
  */
  MbExtrusionSurface ( const MbCurve3D & curve, const MbVector3D & vector, bool same );
  
  /** \brief \ru Конструктор по прямой, точке и образующей.
             \en Constructor by line, point and generating curve. \~
    \details \ru Конструктор по прямой, точке и образующей.\n
      Используется только в конвертерах.
             \en Constructor by line, point and generating curve.\n
      Used only in converters. \~
  */
  MbExtrusionSurface( const MbLine3D &, const MbCartPoint3D &, MbCurve3D &, bool same );

public:
  virtual ~MbExtrusionSurface();

protected:
  MbExtrusionSurface( const MbExtrusionSurface &, MbRegDuplicate * );

private:
  MbExtrusionSurface( const MbExtrusionSurface & ); // \ru Не реализовано. \en Not implemented. 

public:
  VISITING_CLASS( MbExtrusionSurface );

  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */
  MbeSpaceType  IsA() const override; // \ru Тип элемента. \en A type of element. 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Равны ли объекты. \en Whether the objects are equal. 
  bool    SetEqual ( const MbSpaceItem & ) override;       // \ru Сделать равным. \en Make equal. 
  bool    IsSimilar( const MbSpaceItem & ) const override; // \ru Являются ли объекты подобными. \en Determine whether the objects are similar. 
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;          // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;          // \ru Сдвиг. \en Move.
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate around an axis.
  
  void    GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта. \en Set properties of the object. 
  /** \} */
  /** \ru \name Функции для работы в области определения поверхности
            Функции PointOn, Derive... поверхностей корректируют параметры
            при выходе их за пределы прямоугольной области определения параметров.\n 
      \en \name Functions for working at surface domain
            Functions PointOn, Derive... of surfaces correct parameters
            when they are out of bounds of rectangular domain of parameters.\n 
      \{ */
  void    PointOn  ( double & u, double & v, MbCartPoint3D & ) const override; // \ru Точка на поверхности. \en The point on the surface. 
  void    DeriveU  ( double & u, double & v, MbVector3D & ) const override; // \ru Первая производная по u. \en The first derivative with respect to u. 
  void    DeriveV  ( double & u, double & v, MbVector3D & ) const override; // \ru Первая производная по v. \en The first derivative with respect to v. 
  void    DeriveUU ( double & u, double & v, MbVector3D & ) const override; // \ru Вторая производная по u. \en The second derivative with respect to u. 
  void    DeriveVV ( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUV ( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUUU( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUUV( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUVV( double & u, double & v, MbVector3D & ) const override;
  void    DeriveVVV( double & u, double & v, MbVector3D & ) const override;
  void    NormalV  ( double & u, double & v, MbVector3D & ) const override; // \ru Производная нормали. \en The derivative of normal. 
  /** \} */
  /** \ru \name Функции для работы внутри и вне области определения поверхности
            функции _PointOn, _Derive... поверхностей не корректируют
            параметры при выходе их за пределы прямоугольной области определения параметров.
      \en \name Functions for working inside and outside the surface's domain
            functions _PointOn, _Derive... of surfaces don't correct
            parameters when they are out of bounds of rectangular domain of parameters.
      \{ */
  void   _PointOn  ( double u, double v, MbCartPoint3D & ) const override; // \ru Точка на расширенной поверхности. \en The point on the extended surface. 
  void   _DeriveU  ( double u, double v, MbVector3D & ) const override; // \ru Первая производная по u. \en The first derivative with respect to u. 
  void   _DeriveV  ( double u, double v, MbVector3D & ) const override; // \ru Первая производная по v. \en The first derivative with respect to v. 
  void   _DeriveUU ( double u, double v, MbVector3D & ) const override; // \ru Вторая производная по u. \en The second derivative with respect to u. 
  void   _DeriveVV ( double u, double v, MbVector3D & ) const override; // \ru Вторая производная по v. \en The second derivative with respect to v. 
  void   _DeriveUV ( double u, double v, MbVector3D & ) const override; // \ru Вторая производная по uv. \en The second derivative with respect to uv. 
  void   _DeriveUUU( double u, double v, MbVector3D & ) const override;
  void   _DeriveUUV( double u, double v, MbVector3D & ) const override;
  void   _DeriveUVV( double u, double v, MbVector3D & ) const override;
  void   _DeriveVVV( double u, double v, MbVector3D & ) const override;
  void   _NormalV  ( double u, double v, MbVector3D & ) const override; // \ru Производная нормали. \en The derivative of normal. 
  /** \} */
  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметров поверхности.
      \en \name Functions for get of the group of data inside and outside the surface's domain of parameters.
      \{ */
  void    Explore( double & u, double & v, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer,
                   MbVector3D * uuDer, MbVector3D * vvDer, MbVector3D * uvDer, MbVector3D * nor ) const override;
  /** \} */
  /** \ru \name Функции движения по поверхности
      \en \name Functions of moving along the surface
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
  double  CurvatureU    ( double u, double v ) const override; // \ru Кривизна линии по u. \en Curvature of line by u. 
  double  CurvatureV    ( double u, double v ) const override; // \ru Кривизна линии v. \en Curvature of v-line. 

  bool    IsPlanar( double accuracy = METRIC_EPSILON ) const override; // \ru Является ли поверхность плоской. \en Whether the surface is planar. 
  void    ChangeCarrier( const MbSpaceItem & item, MbSpaceItem & init ) override; // \ru Изменение носителя. \en Changing of carrier. 

  MbSplineSurface * NurbsSurface( double, double, double, double, bool bmatch = false ) const override; // \ru NURBS копия поверхности. \en NURBS copy of a surface. 
  MbSurface *       NurbsSurface( const MbNurbsParameters & uParam, const MbNurbsParameters & vParam ) const override; // \ru NURBS копия поверхности. \en NURBS copy of a surface. 
  MbSurface *       Offset( double d, bool same ) const override; // \ru Создание эквидистантной поверхности. \en Create an offset surface. 

  MbCurve3D * CurveU( double v, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии v = const. \en Spatial copy of 'v = const'-line. 
  MbCurve3D * CurveV( double u, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии u = const. \en Spatial copy of 'u = const'-line. 

  ThreeStates Salient() const override; // \ru Выпуклая ли поверхность. \en Whether a surface is convex. 

  // \ru Проекция точки на поверхность. \en The point projection onto the surface. 
  bool    NearPointProjection  ( const MbCartPoint3D & p, double & u, double & v, bool ext, MbRect2D * uvRange = nullptr ) const override; // \ru Ближайшая проекция точки на поверхность. \en The nearest point projection onto the surface.
  // \ru Пересечение с кривой. \en Intersection with curve. 
  void    CurveIntersection( const MbCurve3D & curv, SArray<MbCartPoint> & uv, SArray<double> & tt, 
                             bool ext0, bool ext, bool touchInclude = false ) const override;

  void    CalculateGabarit( MbCube & ) const override; // \ru Выдать габарит. \en Get the bounding box. 
  void    CalculateLocalGabarit( const MbMatrix3D &, MbCube & ) const override; // \ru Рассчитать габарит относительно л.с.к. \en Calculate bounding box relative to the local coordinate system. 

  // \ru Построить касательные и нормальные плейсменты конструктивных плоскостей. \en Construct tangent and normal placements of constructive planes. 
  bool    CreateNormalPlacements ( const MbVector3D & axisZ, double angle, SArray<MbPlacement3D> & places, VERSION version = Math::DefaultMathVersion() ) const override;
  bool    CreateTangentPlacements( const MbVector3D & axisZ, SArray<MbPlacement3D> & places ) const override;
  bool    GetCylinderAxis( MbAxis3D & axis ) const override; // \ru Дать ось вращения для поверхности. \en Get a rotation axis of a surface.
  bool    GetCenterLines( std::vector<MbCurve3D *> & clCurves ) const override; // \ru Дать осевые (центральные) линии для поверхности. \en Get center lines of a surface.
  // \ru Подобные ли поверхности для объединения (слива). \en Whether the surfaces to union (joining) are similar. 
  bool    IsSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override;
  // \ru Дать двумерную матрицу преобразования из своей параметрической области в параметрическую область surf. \en Get two-dimensional transformation matrix from own parametric region to parametric region of 'surf'. 
  bool    GetMatrixToSurface( const MbSurface & surf, MbMatrix & matr, VERSION version, double precision = METRIC_PRECISION ) const override;
  
  // \ru Подобные ли поверхности для объединения (слива) \en Whether the surfaces to union (joining) are similar 
  bool    IsSpecialSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override; // \ru Специальный случай \en Special case 

  double  GetRadius() const override; // \ru Дать физический радиус объекта или ноль, если это невозможно. \en Get the physical radius of the object or null if it impossible.
  double  GetFilletRadius( const MbCartPoint3D & p, double accuracy = METRIC_REGION ) const override; // \ru Является ли поверхность скруглением. \en Whether the surface is fillet. 
  MbeParamDir GetFilletDirection( double accuracy = METRIC_REGION ) const override; // \ru Направление поверхности скругления. \en Direction of fillet surface. 

  // \ru Определение разбивки параметрической области поверхности вертикалями и горизонталями. \en Determine splitting of parametric region of surface by vertical and horizontal lines. 
  void    GetTesselation( const MbStepData & stepData,
                          double u1, double u2, double v1, double v2,
                          SArray<double> & uu, SArray<double> & vv ) const override;  

  size_t  GetUMeshCount() const override; // \ru Выдать количество полигонов по u. \en Get the count of polygons by u. 
  size_t  GetVMeshCount() const override; // \ru Выдать количество полигонов по v. \en Get the count of polygons by v.    
  bool    SetLimit( double u1, double v1, double u2, double v2 ) override; 
  bool    IsLineU() const override; // \ru Если true все производные по U выше первой равны нулю. \en If true, then all the derivatives by U higher the first one are equal to zero. 
  bool    IsLineV() const override; // \ru Если true все производные по V выше первой равны нулю. \en If true, then all the derivatives by V higher the first one are equal to zero.    
  /** \} */
  /** \ru \name Функции поверхности выдавливания.
      \en \name Functions of extrusion surface.
      \{ */
    /// \ru Направление выдавливания. \en A direction of extrusion. 
    const MbVector3D & GetDirection() const { return direction; } 
          /// \ru Длина выдаливания. \en A length of extrusion.  
  double  GetDistance () const { return distance;  } 
          /// \ru Изменить направление выдавливания на противоположное. \en Change direction of extrusion to opposite. 
  void    InvertDirection() { direction.Invert(); SetDirtyGabarit(); } 

  /** \brief \ru Создание эквидистантной поверхности.
              \en Create an offset surface. \~
    \details \ru Создание поверхности типа st_OffsetSurface, совпадающей с данной поверхностью.\n
      Если образующая кривая является эквидистантной кривой на плоскости, 
      то, используя ее базовую кривую в качестве образующей, создается поверхность
      выдавливания и по ней эквидистантная поверхность.\n
      Поверхность строится в случае, если направление выдавливания
      перпендикулярно плоскости образующей кривой.\n
      Используется только в конвертерах.
              \en Create surface of st_OffsetSurface type coinciding with current surface.\n
      If generating curve is offset curve on plane, 
      then using its base curve as generating curve the extrusion surface
      and offset surface by it are created.\n
      Surface is created if direction of extrusion
      is perpendicular to plane of generating curve.\n
      Used only in converters. \~
  */
  MbOffsetSurface * GetSurfaceFromPlaneCurveOffset() const;
  /** \} */

private:
          // \ru Пересечение с прямолинейной кривой. \en Intersection with rectilinear curve. 
  bool    StraightIntersection( const MbCurve3D & curv, SArray<MbCartPoint> & uv, SArray<double> & tt, bool ext0, bool ext ) const;
  void    operator = ( const MbExtrusionSurface & ); // \ru Не реализовано. \en Not implemented. 
protected:
  inline  void    CheckParam( double &u, double &v ) const; // \ru Проверить параметры. \en Check parameters. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbExtrusionSurface )
};

IMPL_PERSISTENT_OPS( MbExtrusionSurface )


//------------------------------------------------------------------------------
// \ru Проверить параметры. \en Check parameters. 
// ---
inline
void MbExtrusionSurface::CheckParam( double & u, double & v ) const
{
  if ( u < umin ) {
    if ( uclosed ) {
      double uRgn = ( umax - umin );
      u -= ( ::floor((u - umin) / uRgn) * uRgn );
    }
    else
      u = umin;
  }
  if ( u > umax ) {
    if ( uclosed ) {
      double uRgn = ( umax - umin );
      u -= ( ::floor((u - umin) / uRgn) * uRgn );
    }
    else
      u = umax;
  }

  if ( v < vmin )
    v = vmin;
  if ( v > vmax )
    v = vmax;
}


#endif // __SURF_EXTRUSION_SURFACE_H
