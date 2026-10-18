////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Секториальная поверхность.
         \en Sectorial surface. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_SECTOR_SURFACE_H
#define __SURF_SECTOR_SURFACE_H


#include <surf_swept_surface.h>


#define SECT_NUMB 3


//------------------------------------------------------------------------------
/** \brief \ru Секториальная поверхность.
           \en Sectorial surface. \~
  \details \ru Секториальная поверхность построена по кривой и точке. 
    Секториальная поверхность является частным случаем линейчатой поверхности с вырожденной в точку второй кривой. 
     Радиус-вектор поверхности описывается векторной функцией \n
    r(u,v) = (1 - v) curve(u) + v origin. \n
    Первый параметр поверхности совпадает с параметром кривой curve. 
    Вдоль второго параметра поверхность прямолинейна.
           \en Sectorial surface is created by curve and point. 
    Sectorial surface is special case of ruled surface with a second curve degenerated to a point. 
     Radius-vector of line surface is described by the vector function \n
    r(u,v) = (1 - v) curve(u) + v origin. \n
    The first surface parameter coincides with the parameter of curve 'curve'. 
    A surface is rectilinear along the second parameter. \~ 
  \ingroup Surfaces
*/ // ---
class MATH_CLASS MbSectorSurface : public MbSweptSurface {
private:
  MbCartPoint3D origin; ///< \ru Точка вместо второй кривой. \en Point instead of the second curve. 

public:

  /** \brief \ru Конструктор по точке и кривой.
             \en Constructor by point and curve. \~
    \details \ru Конструктор по точке и кривой.
             \en Constructor by point and curve. \~
    \param[in] initCurve - \ru Кривая
                           \en Curve \~
    \param[in] p - \ru Точка
                   \en Point \~
    \param[in] same - \ru Признак использования оригинала кривой, а не копии
                      \en Attribute of using the original of a curve instead of the copy. \~
  */
  MbSectorSurface( const MbCurve3D & initCurve, const MbCartPoint3D & p, bool same = false );
protected:
  MbSectorSurface( const MbSectorSurface &, MbRegDuplicate * );
private:
  MbSectorSurface( const MbSectorSurface & ); // \ru Не реализовано. \en Not implemented. 
public:
  virtual ~MbSectorSurface();

public:
  VISITING_CLASS( MbSectorSurface );

  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */
  MbeSpaceType  IsA() const override; // \ru Тип элемента. \en A type of element. 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Make a copy of an element.
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override;    // \ru Являются ли объекты равными. \en Determine whether objects are equal. 
  bool    SetEqual ( const MbSpaceItem & ) override;          // \ru Сделать равным. \en Make equal. 
  bool    IsSimilar( const MbSpaceItem & ) const override;    // \ru Являются ли объекты подобными. \en Determine whether the objects are similar. 
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;             // \ru Преобразовать. \en Transform.
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;             // \ru Сдвиг. \en Translation.
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate around an axis.

  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта. \en Set properties of the object. 
  void    GetBasisItems ( RPArray<MbSpaceItem>   & ) override; // \ru Дать базовые объекты. \en Get the base objects. 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

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
  void    DeriveVV ( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUV ( double & u, double & v, MbVector3D & ) const override; // \ru Вторая производная по uv. \en Second derivative with respect to u and v. 
  void    DeriveUUU( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUUV( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUVV( double & u, double & v, MbVector3D & ) const override;
  void    DeriveVVV( double & u, double & v, MbVector3D & ) const override;
  void    TangentU ( double & u, double & v, MbVector3D & ) const override;
  void    TangentV ( double & u, double & v, MbVector3D & ) const override;
  void    Normal   ( double & u, double & v, MbVector3D & ) const override;
  void    NormalV  ( double & u, double & v, MbVector3D & ) const override;
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
  void   _TangentU ( double u, double v, MbVector3D & ) const override;
  void   _TangentV ( double u, double v, MbVector3D & ) const override;
  void   _Normal   ( double u, double v, MbVector3D & ) const override;
  void   _NormalV  ( double u, double v, MbVector3D & ) const override;
  /** \} */
  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметров поверхности.
      \en \name Functions for get of the group of data inside and outside the surface's domain of parameters.
      \{ */
  void    Explore( double & u, double & v, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer,
                   MbVector3D * uuDer, MbVector3D * vvDer, MbVector3D * uvDer, MbVector3D * nor ) const override;
  /** \} */
  /** \ru \name Функции движения по поверхности
      \en \name Functions of moving over the surface
      \{ */
  double  StepU         ( double u, double v, double sag ) const override; // \ru Вычисление шага по u по заданной стрелке прогиба. \en Calculation of the parameter step in u direction by the sag. 
  double  StepV         ( double u, double v, double sag ) const override; // \ru Вычисление шага по v по заданной стрелке прогиба. \en Calculation of the parameter step in v direction by the sag. 
  double  DeviationStepU( double u, double v, double angle ) const override; // \ru Вычисление шага по u по заданному углу отклонения. \en Calculation of the parameter step in u direction by the deviation angle. 
  double  DeviationStepV( double u, double v, double angle ) const override; // \ru Вычисление шага по v по заданному углу отклонения. \en Calculation of the parameter step in v direction by the deviation angle. 
  double  MetricStepV   ( double u, double v, double length ) const override; // \ru Вычисление шага по v по заданной длине. \en Calculation of the parameter step in v direction by the given length. 
  size_t  GetUCount() const override;
  size_t  GetVCount() const override;
  /** \} */
  /** \ru \name Общие функции поверхности
      \en \name Common functions of surface.
      \{ */
  double  CurvatureV    ( double u, double v ) const override; // \ru Кривизна линии v. \en Curvature of v line. 
  bool    IsPlanar( double accuracy = METRIC_EPSILON ) const override; // \ru Является ли поверхность плоской. \en Whether a surface is planar. 
  void    ChangeCarrier( const MbSpaceItem & item, MbSpaceItem & init ) override; // \ru Изменение носителя. \en Changing of carrier. 

  MbSplineSurface * NurbsSurface( double, double, double, double, bool bmatch = false ) const override; // \ru NURBS копия поверхности. \en NURBS copy of surface. 
  MbSurface *       NurbsSurface( const MbNurbsParameters & uParam, const MbNurbsParameters & vParam ) const override; // \ru NURBS копия поверхности. \en NURBS copy of surface. 

  MbCurve3D * CurveU( double v, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии v = const. \en A spatial copy of the line v = const. 
  MbCurve3D * CurveV( double u, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии u = const. \en A spatial copy of the line u = const. 
    
  size_t  GetUMeshCount() const override; // \ru Выдать количество полигонов по u. \en Get the number of polygons in u-direction. 
  size_t  GetVMeshCount() const override; // \ru Выдать количество полигонов по v. \en Get the number of polygons in v-direction. 

  void    CalculateGabarit( MbCube & ) const override; // \ru Вычислить габарит кривой. \en Calculate bounding box of a curve. 
  void    CalculateLocalGabarit( const MbMatrix3D &, MbCube & ) const override; // \ru Рассчитать габарит относительно л.с.к. \en Calculate bounding box relative to the local coordinate system. 

  // \ru Построить касательные и нормальные плейсменты конструктивных плоскостей. \en Construct tangent and normal placements of constructive planes. 
  bool    CreateNormalPlacements ( const MbVector3D & axisZ, double angle, SArray<MbPlacement3D> & places, VERSION version = Math::DefaultMathVersion() ) const override;
  bool    CreateTangentPlacements( const MbVector3D & axisZ, SArray<MbPlacement3D> & places ) const override;
  // \ru Определение разбивки параметрической области поверхности вертикалями и горизонталями. \en Determine a splitting of parametric region of a surface by verticals and horizontals. 
  void    GetTesselation( const MbStepData & stepData,
                          double u1, double u2, double v1, double v2,
                          SArray<double> & uu, SArray<double> & vv ) const override;
  // \ru Существует ли полюс на границе параметрической области. \en Whether a pole exists on parametric region boundary. 
  bool    GetPoleVMax() const override;
  bool    IsPole( double u, double v, double paramPrecision = PARAM_PRECISION ) const override;

  bool    IsLineV() const override; // \ru Если true все производные по V выше первой равны нулю. \en If it equals true then all derivatives with respect to v which have more than first order are equal to null. 
  /** \} */
  /** \ru \name Функции секториальной поверхности
      \en \name Functions of sectorial surface.
      \{ */
  /// \ru Изменить точку. \en Change point. 
  void    SetOrigin( MbCartPoint3D & p )       { origin = p; }  
  /// \ru Дать точку. \en Get point. 
  void    GetOrigin( MbCartPoint3D & p ) const { p = origin; }  
  /// \ru Дать точку. \en Get point. 
  const MbCartPoint3D & GetOrigin() const { return origin; }          
  /** \} */
private:
  inline  void    CheckParam( double & u, double & v ) const; // \ru Проверить параметры. \en Check parameters. 
  void    operator = ( const MbSectorSurface & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSectorSurface )
};

IMPL_PERSISTENT_OPS( MbSectorSurface )


//------------------------------------------------------------------------------
// \ru Проверить параметры \en Check parameters 
// ---
inline void MbSectorSurface::CheckParam( double & u, double & v ) const
{
  if ( u < umin ) {
    if ( uclosed ) {
      double pRgn = ( umax - umin );
      u -= ( ::floor((u - umin) / pRgn) * pRgn );      
    }
    else
      u = umin;
  }
  else if ( u > umax ) {
    if ( uclosed ) {
      double pRgn = ( umax - umin );
      u -= ( ::floor((u - umin) / pRgn) * pRgn );      
    }
    else
      u = umax;
  }

  if ( v < vmin )
    v = vmin;
  else if ( v > vmax )
    v = vmax;
}


#endif // __SURF_SECTOR_SURFACE_H
