////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Линейчатая поверхность.
         \en Ruled surface. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_RULED_SURFACE_H
#define __SURF_RULED_SURFACE_H


#include <surf_swept_surface.h>
#include <math_define.h>


class   MbMatrix;
class   MbConeSurface;
class   MbExtrusionSurface;


//------------------------------------------------------------------------------
/** \brief \ru Линейчатая поверхность.
           \en Ruled surface. \~
  \details \ru Линейчатая поверхность построена по двум кривым путём соединения их соответствующих точек отрезками прямой. 
     Радиус-вектор поверхности описывается векторной функцией \n
    r(u,v) = (1 - v) curve(u) + v sline(w(u)). \n
    Первый параметр поверхности совпадает с параметром кривой curve. 
    Параметр w кривой sline пропорционален первому параметру поверхности. 
    Вдоль второго параметра поверхность прямолинейна.
           \en A ruled surface is constructed on two curves by connection of its corresponding points by linear segments. 
     Radius-vector of surface is described by the vector function \n
    r(u,v) = (1 - v) curve(u) + v sline(w(u)). \n
    First parameter of surface coincides with parameter of 'curve' curve. 
    Parameter w of 'sline' curve is proportional to first parameter of surface. 
    Surface is linear along the second parameter. \~ 
  \ingroup Surfaces
*/
// ---
class MATH_CLASS MbRuledSurface : public MbSweptSurface {
public:

  /** \brief \ru Типы линейчатой поверхности.
             \en Types of ruled surface. \~
  */
  enum RuledSurfaceType {
    rld_Unset = 0, ///< \ru Тип поверхности не установлен. \en Surface type isn't set. 
    rld_Planar,    ///< \ru Геометрия плоскости. \en Planar. 
    rld_Line,      ///< \ru Образующие кривые прямолинейны. \en Generating curves are straight lines. 
    rld_Cone,      ///< \ru Геометрия конической поверхности. \en Conical surface. 
    rld_PoleMin,   ///< \ru Кривая curve вырождена в точку. \en 'curve' curve is degenerated to a point. 
    rld_PoleMax,   ///< \ru Кривая sline вырождена в точку. \en 'sline' curve is degenerated to a point. 
    rld_Offset,    ///< \ru Геометрия поверхности выдавливания с уклоном. \en Extrusion surface with taper. 
    rld_Swept,     ///< \ru Геометрия поверхности выдавливания. \en Extrusion surface. 
    rld_Arbitrary, ///< \ru Произвольная линейчатая поверхность. \en Arbitrary ruled surface. 
  };

private:
  RuledSurfaceType    type;          ///< \ru Тип линейчатой поверхности. \en Type of ruled surface. 
  MbCurve3D *         sline;         ///< \ru Вторая образующая кривая (первой является curve). \en The second generating curve ('curve' is first one). 
  double              tmin;          ///< \ru Начальный параметр sline. \en Start parameter of 'sline'. 
  double              dt;            ///< \ru Производная параметра кривой sline по параметру u (dt * (u - umin) = t_sline - tmin_sline). \en Derivative of parameter of 'sline' curve by u parameter (dt * (u - umin) = t_sline - tmin_sline). 
  bool                poleMin;       ///< \ru Полюс при umin. \en Pole at umin. 
  bool                poleMax;       ///< \ru Полюс при umax. \en Pole at umax. 
  double              uMinNormDelta; ///< \ru Отступ при наличии касательного полюса umin. \en Indent in the presence of umin tangent pole. 
  double              uMaxNormDelta; ///< \ru Отступ при наличии касательного полюса umax. \en Indent in the presence of umax tangent pole. 
  double              uminExt;       ///< \ru Минимальное разрешенное значение по u. \en Minimal allowed value by u. 
  double              umaxExt;       ///< \ru Максимальное разрешенное значение по u. \en Maximal allowed value by u. 
  double              vminExt;       ///< \ru Минимальное разрешенное значение по v. \en Minimal allowed value by v. 
  double              vmaxExt;       ///< \ru Максимальное разрешенное значение по u. \en Maximal allowed value by v. 
  std::vector<double> uBreaks;       ///< \ru Параметры кривой curve, для которых curve и sline терпят излом. \en Parameters of the curve for which the curve and sline have a break. 

public:

  /** \brief \ru Конструктор по двум кривым.
             \en Constructor by two curves. \~
    \details \ru Конструктор по двум кривым.
             \en Constructor by two curves. \~
    \param[in] c1 - \ru Первая образующая кривая
                    \en First generating curve \~
    \param[in] c2 - \ru Вторая образующая кривая.
                    \en Second generating curve. \~
    \param[in] same - \ru Признак использования оригинала образующих кривых, а не их копий.
                      \en Attribute of usage of original of generating curves, not a copies. \~
  */
  MbRuledSurface( const MbCurve3D & c1, const MbCurve3D & c2, bool same );
  
  /** \brief \ru Конструктор по двум кривым и параметрам по V.
             \en Constructor by two curves and parameters by V. \~
    \details \ru Конструктор по двум кривым и параметрам по V.\n
      Используется только в конвертерах.
             \en Constructor by two curves and parameters by V.\n
      Used only in converters. \~
    \param[in] vin - \ru Минимальный параметр по V
                     \en Minimal parameter by V \~
    \param[in] vax - \ru Максимальный параметр по V
                     \en Maximal parameter by V \~
    \param[in] c1 - \ru Первая образующая кривая
                    \en First generating curve \~
    \param[in] c2 - \ru Вторая образующая кривая
                    \en Second generating curve \~        
  */
  // \ru Используется только в конверторах для эллипса и цилиндра в orthosur \en Used only in converters for ellipse and cylinder in 'orthosur' 
  MbRuledSurface( double vin, double vax, const MbCurve3D & c1, const MbCurve3D & c2 ); 

protected:
  MbRuledSurface( const MbRuledSurface &, MbRegDuplicate * );
public:
  virtual ~MbRuledSurface ();

public:
  VISITING_CLASS( MbRuledSurface );

  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */
  MbeSpaceType  IsA() const override; // \ru Тип элемента. \en A type of element. 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Равны ли объекты. \en Whether the objects are equal. 
  bool    SetEqual ( const MbSpaceItem & ) override; // \ru Сделать равным. \en Make equal. 
  bool    IsSimilar( const MbSpaceItem & ) const override; // \ru Являются ли объекты подобными. \en Determine whether the objects are similar. 
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг. \en Move.
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate around an axis.
  
  void    GetProperties( MbProperties &properties ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties &properties ) override; // \ru Записать свойства объекта. \en Set properties of the object. 
  void    GetBasisItems ( RPArray <MbSpaceItem> & ) override; // \ru Дать базовые объекты. \en Get the base objects.
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  void    Refresh() override; // \ru Сбросить все временные данные. \en Reset all temporary data. 

  /** \} */
  /** \ru \name Функции описания области определения поверхности
      \en \name Functions for surface domain description
      \{ */
  double  GetUPeriod() const override; // \ru Вернуть период. \en Return period. 
  /** \} */
  /** \ru \name Функции для работы в области определения поверхности
            Функции PointOn, Derive... поверхностей корректируют параметры
            при выходе их за пределы прямоугольной области определения параметров.   
      \en \name Functions for working at surface domain
            Functions PointOn, Derive... of surfaces correct parameters
            when they are out of bounds of rectangular domain of parameters.
      \{ */
  void    PointOn  ( double & u, double & v, MbCartPoint3D & ) const override; // \ru Точка на поверхности. \en The point on the surface. 
  void    DeriveU  ( double & u, double & v, MbVector3D & ) const override; // \ru Первая производная по u. \en The first derivative with respect to u. 
  void    DeriveV  ( double & u, double & v, MbVector3D & ) const override; // \ru Первая производная по v. \en The first derivative with respect to v. 
  void    DeriveUU ( double & u, double & v, MbVector3D & ) const override; // \ru Вторая производная по u. \en The second derivative with respect to u. 
  void    DeriveVV ( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUV ( double & u, double & v, MbVector3D & ) const override; // \ru Вторая производная по uv. \en The second derivative with respect to uv. 
  void    DeriveUUU( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUUV( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUVV( double & u, double & v, MbVector3D & ) const override;
  void    DeriveVVV( double & u, double & v, MbVector3D & ) const override;
  void    Normal   ( double & u, double & v, MbVector3D & ) const override; // \ru Нормаль. \en Normal. 
  void    NormalU  ( double & u, double & v, MbVector3D & ) const override; // \ru Производная нормали. \en The derivative of normal. 
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
  void   _Normal   ( double u, double v, MbVector3D & ) const override; // \ru Нормаль. \en Normal. 
  void   _NormalU  ( double u, double v, MbVector3D & ) const override; // \ru Производная нормали. \en The derivative of normal. 
  void   _NormalV  ( double u, double v, MbVector3D & ) const override; // \ru Производная нормали. \en The derivative of normal. 
  void   _NormalUU ( double u, double v, MbVector3D & ) const override; // \ru Производная нормали. \en The derivative of normal. 
  void   _NormalUV ( double u, double v, MbVector3D & ) const override; // \ru Производная нормали. \en The derivative of normal. 
  void   _NormalVV ( double u, double v, MbVector3D & ) const override; // \ru Производная нормали. \en The derivative of normal. 
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

  size_t  GetUMeshCount() const override; // \ru Выдать количество полигонов по u. \en Get the count of polygons by u. 
  size_t  GetVMeshCount() const override; // \ru Выдать количество полигонов по v. \en Get the count of polygons by v. 

  void    ChangeCarrier( const MbSpaceItem & item, MbSpaceItem & init ) override; // \ru Изменение носителя. \en Changing of carrier. 

  // \ru Ближайшая проекция точки на поверхность. \en The nearest point projection onto the surface.
  bool    NearPointProjection( const MbCartPoint3D & p, double & u, double & v, bool ext, MbRect2D * uvRange = nullptr ) const override;

  // \ru Пересечение с кривой. \en Intersection with curve. 
  void    CurveIntersection( const MbCurve3D & curv, SArray<MbCartPoint> & uv, SArray<double> & tt, 
                             bool ext0, bool ext, bool touchInclude = false ) const override;

  void    CalculateGabarit( MbCube & ) const override; // \ru Выдать габарит. \en Get the bounding box. 
  void    CalculateLocalGabarit( const MbMatrix3D &, MbCube & ) const override; // \ru Рассчитать габарит относительно л.с.к.. \en Calculate bounding box relative to the local coordinate system. 

  MbSplineSurface * NurbsSurface( double, double, double, double, bool bmatch = false ) const override; // \ru NURBS копия поверхности. \en NURBS copy of a surface. 
  MbSurface *       NurbsSurface( const MbNurbsParameters & uParam, const MbNurbsParameters & vParam ) const override;
  MbSurface *       Offset( double d, bool same ) const override;  // \ru Построить смещенную поверхность. \en Create a shifted surface. 

  MbCurve3D * CurveU( double v, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии v = const. \en Spatial copy of 'v = const'-line. 
  MbCurve3D * CurveV( double u, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии u = const. \en Spatial copy of 'u = const'-line. 
  MbCurve3D * CurveUV( const MbLineSegment &, bool bApprox = true ) const override; // \ru Пространственная копия линии по параметрической линии. \en Spatial copy of line by parametric line.

  // \ru Построить касательные и нормальные плейсменты конструктивных плоскостей. \en Construct tangent and normal placements of constructive planes. 
  bool    CreateNormalPlacements ( const MbVector3D & axisZ, double angle, SArray<MbPlacement3D> & places, VERSION version = Math::DefaultMathVersion() ) const override;
  bool    CreateTangentPlacements( const MbVector3D & axisZ, SArray<MbPlacement3D> & places ) const override;
  bool    GetCylinderAxis( MbAxis3D &axis ) const override; // \ru Дать ось вращения для поверхности \en Get a rotation axis of a surface 
  // \ru Подобные ли поверхности для объединения (слива). \en Whether the surfaces to union (joining) are similar. 
  bool    IsSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override;
  // \ru Подобные ли поверхности для объединения (слива). Специальный случай. Для внутреннего использования. \en Whether the surfaces are similar to merge. Special case. For internal use only. 
  bool    IsSpecialSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override;
  // \ru Дать двумерную матрицу преобразования из своей параметрической области в параметрическую область surf. \en Get two-dimensional transformation matrix from own parametric region to parametric region of 'surf'. 
  bool    GetMatrixToSurface( const MbSurface & surf, MbMatrix & matr, VERSION version, double precision = METRIC_PRECISION ) const override;

  // \ru Существует ли полюс на границе параметрической области. \en Whether there is pole on boundary of parametric region. 
  bool    GetPoleUMin() const override;
  bool    GetPoleUMax() const override;
  bool    GetPoleVMin() const override;
  bool    GetPoleVMax() const override;
  bool    IsPole( double u, double v, double paramPrecision = PARAM_PRECISION ) const override; // \ru Является ли точка особенной. \en Whether the point is special. 
  // \ru Определение разбивки параметрической области поверхности вертикалями и горизонталями. \en Determine splitting of parametric region of surface by vertical and horizontal lines. 
  void    GetTesselation( const MbStepData & stepData,
                          double u1, double u2, double v1, double v2,
                          SArray<double> & uu, SArray<double> & vv ) const override;

  bool    IsLineU() const override; // \ru Если true все производные по U выше первой равны нулю. \en If true, then all the derivatives by U higher the first one are equal to zero. 
  bool    IsLineV() const override; // \ru Если true все производные по V выше первой равны нулю. \en If true, then all the derivatives by V higher the first one are equal to zero. 
  /** \} */
  /** \ru \name Функции движения по поверхности
      \en \name Functions of moving along the surface
      \{ */
  /// \ru Дать вторую образующую кривую. \en Get second generating curve. 
  const   MbCurve3D & GetSline() const { return *sline; } 
  /// \ru Дать вторую образующую кривую для изменения. \en Get second generating curve for changing. 
          MbCurve3D & SetSline()       { return *sline; } 

  /// \ru Тип линейчатой поверхности. \en Type of ruled surface.    
  inline  RuledSurfaceType  GetType() const { C3D_ASSERT( type != rld_Unset ); return type; } 
  /// \ru Обновить тип линейчатой поверхности. \en Update type of ruled surface.
  inline  void              UpdateType();
  /** \} */

  inline  double  SlineParameterFrom( const double & u ) const;
  inline  double  SlineParameterInto( const double & t ) const;

private:  
  inline  void    CheckParam( double & u, double & v ) const; // \ru Проверить параметры. \en Check parameters. 
  inline  void    CheckPoleParam( double & u, double & v ) const; 

  void    InitNormDeltaU(); // \ru Посчитать величины отступа от uMin и uMax при подсчете нормали \en Calculate indent values from uMin and uMax in calculation of normal 
  void    InitTabooUV();    // \ru Вычислить ограничения по u и v \en Calculate constraints by u and v 
          // \ru Пересечение с прямолинейной кривой \en Intersection with rectilinear curve 
  bool    StraightIntersection( const MbCurve3D & curv, SArray<MbCartPoint> & uv, SArray<double> & tt, 
                                        bool ext0, bool ext ) const;
  RuledSurfaceType CheckType();
  MbConeSurface * GetConeSurface( bool checkParams ) const;
  bool    IsPlane() const; // \ru НЕ ИСПОЛЬЗОВАТЬ СНАРУЖИ !!! \en NOT USE OUTSIDE !!! 
  void    BreakPoints( bool forCurve, double precision = ANGLE_REGION ); // \ru Определение точек излома направляющих. \en Determination of the break points of the guides.

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbRuledSurface )
OBVIOUS_PRIVATE_COPY( MbRuledSurface )
};

IMPL_PERSISTENT_OPS( MbRuledSurface )


//------------------------------------------------------------------------------
// \ru Проверить параметры \en Check parameters 
// ---
inline void MbRuledSurface::CheckParam( double & u, double & v ) const
{
  if (u < umin) {
    if ( uclosed ) {
      double pRgn = ( umax - umin );
      u -= ( ::floor((u - umin) / pRgn) * pRgn );      
    }
    else
      u = umin;
  }
  else if ( u > umax) {
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


//------------------------------------------------------------------------------
// \ru Корректировка параметров \en Correct parameters 
// ---
inline void MbRuledSurface::CheckPoleParam( double & u, double & v ) const
{
  if ( (type == rld_PoleMin) && (v < vmin) )
    v = vmin;
  else if ( v < vminExt )
    v = vminExt;

  if ( (type == rld_PoleMax) && (v > vmax) )
    v = vmax;
  else if ( v > vmaxExt )
    v = vmaxExt;

  if ( u < umin ) {
    if ( poleMin )
      u = umin;
    else if ( u < uminExt )
      u = uminExt;
  }
  if ( u > umax ) {
    if ( poleMax )
      u = umax;
    else if ( u > umaxExt )
      u = umaxExt;
  }
}


//------------------------------------------------------------------------------
// \ru Обновить тип линейчатой поверхности. \en Update type of ruled surface.
// ---
inline void MbRuledSurface::UpdateType()
{
  type = rld_Unset;
  CheckType();
}


//------------------------------------------------------------------------------
// \ru Перевод параметра curve в параметр sline \en Convert 'curve' parameter to 'sline' parameter 
// ---
inline double MbRuledSurface::SlineParameterFrom( const double & u ) const {
  return tmin + (u - umin) * dt;
}


//------------------------------------------------------------------------------
// \ru Перевод параметра sline в параметр curve \en Convert 'sline' parameter to 'curve' parameter 
// ---
inline double MbRuledSurface::SlineParameterInto( const double & t ) const {
  double du = (::fabs(dt) > EXTENT_EQUAL) ? 1.0 / dt : 1.0;
  return umin + (t - tmin) * du;
}


//------------------------------------------------------------------------------
// \ru Являются ли поверхости геометрически одинаковыми \en Whether surfaces are similar 
// ---
bool IsSameRuledExtrusion( const MbRuledSurface & ruled, const MbExtrusionSurface & extrusion,
                           MbMatrix * matr );


#endif // __SURF_RULED_SURFACE_H
