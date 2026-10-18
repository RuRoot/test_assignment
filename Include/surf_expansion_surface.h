////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Поверхность плоскопараллельного движения.
         \en Motion surface (plane-parallel swept surface). \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_EXPANSION_SURFACE_H
#define __SURF_EXPANSION_SURFACE_H


#include <surf_swept_surface.h>
#include <surf_spine.h>


//------------------------------------------------------------------------------
/** \brief \ru Поверхность плоскопараллельного движения.
           \en Motion surface (plane-parallel swept surface). \~
  \details \ru Поверхность плоскопараллельного движения получается путем движения образующей кривой curve 
    по направляющей кривой spine->curve параллельно самой себе:
     Радиус-вектор поверхности описывается векторной функцией \n
    r(u,v) = curve(u) + spine(v) - origin. \n
    Направляющая плоскопараллельной поверхности должна быть незамкнутой монотонной кривой.
    Первый параметр поверхности совпадает с параметром образующей кривой. 
    Второй параметр поверхности совпадает с параметром направляющей кривой.
           \en Expansion surface is obtained by moving 'curve' generating curve 
    along 'spine->curve" guide curve parallel to itself:
     Radius-vector of surface is described by the vector function \n
    r(u,v) = curve(u) + spine(v) - origin. \n
    Guide curve of plane-parallel surface should be open monotonous curve.
    First parameter of surface coincides with parameter of generating curve. 
    Second parameter of surface coincides with parameter of guide curve. \~ 
  \ingroup Surfaces
*/ 
// ---
class MATH_CLASS MbExpansionSurface : public MbSweptSurface {
private:
  MbCurve3D *   spine;  ///< \ru Направляющая кривая. \en Spine (guide) curve. 
  MbCurve3D *   brink;  ///< \ru Вторая образующая кривая (первой является curve, может быть nullptr). \en The second generating curve ('curve' is first one, may be nullptr).
  double        tmin;   ///< \ru Начальный параметр brink. \en Start parameter of 'brink'. 
  double        dt;     ///< \ru Производная параметра кривой brink по параметру u (dt * (u - umin) = t_brink - tmin_brink). \en Derivative of parameter of 'brink' curve by u parameter (dt * (u - umin) = t_brink - tmin_brink). 
  MbCartPoint3D origin; ///< \ru Начало образующей. \en Begin of gravity of generating curve. 
  MbCartPoint3D ending; ///< \ru Конец образующей. \en End of gravity of generating curve. 

public:

  /** \brief \ru Конструктор по образующей и направляющей.
             \en Constructor by generating curve and guide curve. \~
    \details \ru Конструктор по образующей и направляющей.
             \en Constructor by generating curve and guide curve. \~
    \param[in] cr - \ru Образующая кривая.
                    \en Generating curve \~ 
    \param[in] sp - \ru Направляющая кривая.
                    \en Guide curve \~
    \param[in] sameCurve - \ru Признак использования оригинала образующей, а не копии.
                           \en Attribute of usage of original of generating curve, not a copy. \~
    \param[in] sameSpine - \ru Признак использования оригинала направляющей, а не копии
                           \en Attribute of usage of original of guide curve, not a copy. \~
    \param[in] sl - \ru Вторая образующая кривая.
                    \en Second generating curve \~
  */
  MbExpansionSurface( const MbCurve3D & cr, const MbCurve3D & sp, bool sameCurve, bool sameSpine,
                      const MbCurve3D * sl = nullptr );
  
  /** \brief \ru Конструктор по точке, образующей и направляющей.
             \en Constructor by point, generating curve and guide curve. \~
    \details \ru Конструктор по точке, образующей и направляющей.\n
      Создается поверхность, привязанная к образующей.
             \en Constructor by point, generating curve and guide curve.\n
      Created a surface is binded to generating curve. \~      
    \param[in] point - \ru Точка, определяющая вектор сдвига центра тяжести образующей от направляющей,
                           он направлен из точки point в точку origin.
                       \en Point determining translation vector of center of gravity of generating curve from guide curve
                           which is directed from 'point' point to 'origin' point. \~
    \param[in] curve - \ru Образующая
                       \en Generating curve \~ 
    \param[in] spine - \ru Направляющая
                       \en Guide curve \~         
    \param[in] sameCurve - \ru Признак использования оригинала образующей, а не копии
                           \en Attribute of usage of original of generating curve, not a copy \~
    \param[in] sameSpine - \ru Признак использования оригинала направляющей, а не копии
                           \en Attribute of usage of original of guide curve, not a copy \~
  */
  MbExpansionSurface( const MbCartPoint3D & point, const MbCurve3D & curve, const MbCurve3D & spine,
                      bool sameCurve, bool sameSpine );
public:
  virtual ~MbExpansionSurface();

protected:
  MbExpansionSurface( const MbExpansionSurface &, MbRegDuplicate * );

private:
  MbExpansionSurface( const MbExpansionSurface & ); // \ru Не реализовано. \en Not implemented. 

public:
  VISITING_CLASS( MbExpansionSurface );

  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */
  MbeSpaceType  IsA() const override; // \ru Тип элемента. \en A type of element. 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Равны ли объекты. \en Whether the objects are equal. 
  bool    SetEqual ( const MbSpaceItem & ) override; // \ru Сделать равным. \en Make equal. 
  bool    IsSimilar( const MbSpaceItem & ) const override; // \ru Являются ли объекты подобными. \en Determine whether the objects are similar. 
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;    // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;    // \ru Сдвиг. \en Move.
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate around an axis.
  
  void    GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта. \en Set properties of the object. 
  void    GetBasisItems ( RPArray<MbSpaceItem> &   ) override;  // \ru Дать базовые объекты. \en Get the base objects. 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

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
  void    DeriveVV ( double & u, double & v, MbVector3D & ) const override; // \ru Вторая производная по v. \en The second derivative with respect to v. 
  void    DeriveUV ( double & u, double & v, MbVector3D & ) const override; // \ru Вторая производная по uv. \en The second derivative with respect to uv. 
  void    DeriveUUU( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUUV( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUVV( double & u, double & v, MbVector3D & ) const override;
  void    DeriveVVV( double & u, double & v, MbVector3D & ) const override;
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
  double  StepU         ( double u, double v, double sag ) const override; // \ru Вычисление шага по u при пересечении поверхностей. \en Calculation of step by u while intersecting surfaces. 
  double  StepV         ( double u, double v, double sag ) const override; // \ru Вычисление шага по v при пересечении поверхностей. \en Calculation of step by v while intersecting surfaces. 
  double  DeviationStepU( double u, double v, double sag ) const override; // \ru Вычисление шага по u при пересечении поверхностей. \en Calculation of step by u while intersecting surfaces. 
  double  DeviationStepV( double u, double v, double sag ) const override; // \ru Вычисление шага по v при пересечении поверхностей. \en Calculation of step by v while intersecting surfaces. 
  size_t  GetUCount() const override;
  size_t  GetVCount() const override;
  /** \} */
  /** \ru \name Общие функции поверхности
      \en \name Common functions of surface
      \{ */
  double  CurvatureU    ( double u, double v ) const override; // \ru Кривизна линии по u. \en Curvature of line by u. 
  double  CurvatureV    ( double u, double v ) const override; // \ru Кривизна линии по v. \en Curvature of line by v. 
  bool    IsPlanar( double accuracy = METRIC_EPSILON ) const override; // \ru Является ли поверхность плоской. \en Whether the surface is planar. 
    
  size_t  GetUMeshCount() const override; // \ru Выдать количество полигонов по u. \en Get the count of polygons by u. 
  size_t  GetVMeshCount() const override; // \ru Выдать количество полигонов по v. \en Get the count of polygons by v. 

  void    ChangeCarrier( const MbSpaceItem & item, MbSpaceItem & init ) override; // \ru Изменение носителя. \en Changing of carrier. 

  MbSplineSurface * NurbsSurface( double, double, double, double, bool bmatch = false ) const override; // \ru NURBS копия поверхности. \en NURBS copy of a surface. 
  MbSurface *       NurbsSurface( const MbNurbsParameters & uParam, const MbNurbsParameters & vParam ) const override; // \ru NURBS копия поверхности. \en NURBS copy of a surface. 

  MbCurve3D * CurveU( double v, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии v = const. \en Spatial copy of 'v = const'-line. 
  MbCurve3D * CurveV( double u, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии u = const. \en Spatial copy of 'u = const'-line. 

  // \ru Построить касательные и нормальные плейсменты конструктивных плоскостей. \en Construct tangent and normal placements of constructive planes. 
  bool    CreateNormalPlacements ( const MbVector3D & axisZ, double angle, SArray<MbPlacement3D> & places, VERSION version = Math::DefaultMathVersion() ) const override;
  bool    CreateTangentPlacements( const MbVector3D & axisZ, SArray<MbPlacement3D> & places ) const override;
  // \ru Определение разбивки параметрической области поверхности вертикалями и горизонталями. \en Determine splitting of parametric region of surface by vertical and horizontal lines. 
  void    GetTesselation( const MbStepData & stepData,
                          double u1, double u2, double v1, double v2,
                          SArray<double> & uu, SArray<double> & vv ) const override;

  // \ru Подобные ли поверхности для объединения (слива) (геометрическое совпадение). \en Whether the surfaces to union (joining) are similar (geometric coincidence). 
  bool    IsSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override;
  // \ru Подобные ли поверхности для объединения (слива) \en Whether the surfaces to union (joining) are similar 
  bool    IsSpecialSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override; // \ru Специальный случай \en Special case 
  // \ru Дать двумерную матрицу преобразования из своей параметрической области в параметрическую область surf. \en Get two-dimensional transformation matrix from own parametric region to parametric region of 'surf'. 
  bool    GetMatrixToSurface( const MbSurface & surf, MbMatrix & matr, VERSION version, double precision = METRIC_PRECISION ) const override;
  /** \} */
  /** \ru \name Функции плоскопараллельной поверхности
      \en \name Functions of plane-parallel surface
      \{ */         

  /** \brief \ru Определение вектора переноса образующей.
              \en Determination of translation vector for generating curve. \~
    \details \ru Определение вектора переноса образующей по параметру на направляющей.
              \en Determination of translation vector for generating curve by parameter of guide curve. \~
    \param[in] v - \ru Параметр на направляющей
                    \en Parameter on the guide curve \~
    \param[in] vect - \ru Вектор-результат
                      \en Vector-result \~
  */
  void    TransformVector( double & v, MbVector3D & vect ) const; 

  /// \ru Направляющая кривая. \en The spine (guide) curve. 
  const MbCurve3D     & GetSpineCurve() const { return *spine; } 
  /// \ru Центр тяжести образующей. \en Center of gravity of generating curve. 
  const MbCartPoint3D & GetOrigin()     const { return origin; } 
  /// \ru Вторая образующая кривая. \en The second generating curve. 
  const MbCurve3D     * GetBrink() const { return brink; } 

  inline  double  BrinkParameterFrom( const double & u ) const;
  inline  double  BrinkParameterInto( const double & t ) const;
          
  /// \ru Дать направляющую кривую для изменения. \en Get guide curve for editing. 
  MbCurve3D     & SetSpineCurve() { return *spine; } 
  /// \ru Изменение центра тяжести образующей. \en Change center of gravity of generating curve. 
  void            SetOrigin( const MbCartPoint3D & p ) { origin = p; SetDirtyGabarit(); } 
   /** \} */
private:
  void    Init();
  void    operator = ( const MbExpansionSurface & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbExpansionSurface )
};

IMPL_PERSISTENT_OPS( MbExpansionSurface )


//------------------------------------------------------------------------------
// \ru Перевод параметра curve в параметр brink \en Convert 'curve' parameter to 'brink' parameter 
// ---
inline double MbExpansionSurface::BrinkParameterFrom( const double & u ) const {
  double t = u;
  if ( brink != nullptr )
    t = tmin + (u - umin) * dt;
  return t;
}


//------------------------------------------------------------------------------
// \ru Перевод параметра brink в параметр curve \en Convert 'brink' parameter to 'curve' parameter 
// ---
inline double MbExpansionSurface::BrinkParameterInto( const double & t ) const {
  double u = t;
  if ( brink != nullptr ) {
    double du = (::fabs(dt) > EXTENT_EQUAL) ? 1.0 / dt : 1.0;
    u = umin + (t - tmin) * du;
  }
  return u;
}


#endif // __SURF_EXPANSION_SURFACE_H
