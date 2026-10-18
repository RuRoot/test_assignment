////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Элементарная поверхность.
         \en An elementary surface. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_ELEMENTARY_SURFACE_H
#define __SURF_ELEMENTARY_SURFACE_H


#include <surface.h>
#include <mb_placement3d.h>


constexpr size_t CIRC_COUNT = 32;
constexpr size_t HIDE_COUNT = 16;
constexpr size_t LINE_COUNT = 10;


//------------------------------------------------------------------------------
/** \brief \ru Элементарная поверхность.
           \en An elementary surface. \~
  \details \ru Родительский класс поверхностей: MbConeSurface, MbCylinderSurface, MbPlane, MbSphereSurface, MbTorusSurface. 
    Элементарная поверхность описывается аналитическим выражением в локальной системе координат. 
    В локальной системе координат аналитическое выражение радиуса-вектора поверхности имеет канонический вид. \n
    Локальная система координат position может быть как правой, так и левой.\n
           \en Parent class for surfaces: MbConeSurface, MbCylinderSurface, MbPlane, MbSphereSurface, MbTorusSurface. 
    Elementary surface is described by analytical expression in local coordinate system. 
    Analytical expression of radius-vector of surface has canonical form in local coordinate system. \n
    Local coordinate system 'position' can be both right and left.\n \~
  \ingroup Surfaces
*/
// ---
class MATH_CLASS MbElementarySurface : public MbSurface {
protected:
  MbPlacement3D position; ///< \ru Локальная система координат. \en Local coordinate system. 
  double        umin;     ///< \ru Минимальное  значение первого параметра. \en Minimal value of first parameter. 
  double        vmin;     ///< \ru Минимальное  значение второго параметра. \en Minimal value of second parameter. 
  double        umax;     ///< \ru Максимальное значение первого параметра. \en Maximal value of first parameter. 
  double        vmax;     ///< \ru Максимальное значение второго параметра. \en Maximal value of second parameter. 

protected:
  MbElementarySurface();
  MbElementarySurface( const MbPlacement3D & );
  MbElementarySurface( const MbCartPoint3D & origin, const MbVector3D & axisZ, const MbVector3D & axisX );
  MbElementarySurface( const MbCartPoint3D & p0, const MbCartPoint3D & p1, const MbCartPoint3D & p2 );
  MbElementarySurface( const MbElementarySurface &other );
public:
  virtual ~MbElementarySurface();

public:
  VISITING_CLASS( MbElementarySurface );

  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */
  //virtual MbeSpaceType  IsA () const = 0; // \ru Тип элемента. \en A type of element.
  MbeSpaceType  Type() const override;     // \ru Тип элемента. \en A type of element. 
  //virtual MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const = 0;  // \ru Сделать копию элемента. \en Create a copy of the element.
  //virtual bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const = 0;     // \ru Являются ли объекты равными. \en Determine whether objects are equal.
  //virtual bool    SetEqual ( const MbSpaceItem & ) = 0;           // \ru Сделать равным. \en Make equal.
  void    Transform( const MbMatrix3D & matr, MbRegTransform * = nullptr ) override;              // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void    Move     ( const MbVector3D & to, MbRegTransform * = nullptr ) override;                // \ru Сдвиг. \en Move.
  void    Rotate   ( const MbAxis3D & axis, double angle, MbRegTransform * = nullptr ) override;  // \ru Повернуть вокруг оси. \en Rotate around an axis.
  double  DistanceToPoint( const MbCartPoint3D & to ) const override; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point.

  //virtual void    GetProperties( MbProperties & properties ) = 0; // \ru Выдать свойства объекта. \en Get properties of the object.
  //virtual void    SetProperties( const MbProperties & properties ) = 0; // \ru Записать свойства объекта. \en Set properties of the object.
  void    GetBasisItems ( RPArray<MbSpaceItem> & s ) override; ///< \ru Дать базовые объекты. \en Get the base objects.
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \} */
  /** \ru \name Функции описания области определения поверхности
      \en \name Functions for surface domain description
      \{ */
  //virtual double  GetUMin()   const = 0; // \ru Вернуть минимальное значение параметра u. \en Return the minimum value of parameter u.
  //virtual double  GetVMin()   const = 0; // \ru Вернуть минимальное значение параметра v. \en Return the minimum value of parameter v.
  //virtual double  GetUMax()   const = 0; // \ru Вернуть максимальное значение параметра u. \en Return the maximum value of parameter u.
  //virtual double  GetVMax()   const = 0; // \ru Вернуть максимальное значение параметра v. \en Return the maximum value of parameter v.
  //virtual bool    IsUClosed() const = 0; // \ru Проверка замкнутости по параметру u. \en Check of closedness by parameter u.
  //virtual bool    IsVClosed() const = 0; // \ru Проверка замкнутости по параметру v. \en Check of closedness by parameter v.
  /** \} */
  /** \ru \name Функции для работы в области определения поверхности
            Функции PointOn, Derive... поверхностей корректируют параметры
            при выходе их за пределы прямоугольной области определения параметров.\n
            Исключения составляют:\n
            1. MbPlane (плоскость)\n 
               Функции PointOn, Derive... плоскости не корректируют параметры
               при выходе их за пределы прямоугольной области определения параметров.\n
            2. MbSmoothSurface и её наследники (поверхности скругления или фаски)\n
               Функции PointOn и Derive... поверхностей сопряжения не корректируют
               первый параметр при его выходе за пределы определения параметров.
      \en \name Functions for working at surface domain
            Functions PointOn, Derive... of surfaces correct parameters
            when they are out of bounds of rectangular domain of parameters.\n
            Except:\n
            1. MbPlane (plane)\n 
               Functions PointOn, Derive... of plane don't correct parameters
               when they are out of bounds of rectangular domain of parameters.\n
            2. MbSmoothSurface and its inheritors (fillet or chamfer surfaces)\n
               Functions PointOn, Derive... of smooth surfaces don't correct
               first parameter when it is out of domain bounds
      \{ */
  //virtual void    PointOn  ( double & u, double & v, MbCartPoint3D & ) const = 0; // \ru Точка на поверхности. \en The point on the surface.
  //virtual void    DeriveU  ( double & u, double & v, MbVector3D & ) const = 0; // \ru Первая производная по u. \en The first derivative with respect to u.
  //virtual void    DeriveV  ( double & u, double & v, MbVector3D & ) const = 0; // \ru Первая производная по v. \en The first derivative with respect to v.
  //virtual void    DeriveUU ( double & u, double & v, MbVector3D & ) const = 0; // \ru Вторая производная по u. \en The second derivative with respect to u.
  //virtual void    DeriveVV ( double & u, double & v, MbVector3D & ) const = 0; // \ru Вторая производная по v. \en The second derivative with respect to v.
  //virtual void    DeriveUV ( double & u, double & v, MbVector3D & ) const = 0; // \ru Вторая производная по uv. \en The second derivative with respect to uv.
  //virtual void    DeriveUUU( double & u, double & v, MbVector3D & ) const = 0;
  //virtual void    DeriveUUV( double & u, double & v, MbVector3D & ) const = 0;
  //virtual void    DeriveUVV( double & u, double & v, MbVector3D & ) const = 0;
  //virtual void    DeriveVVV( double & u, double & v, MbVector3D & ) const = 0;
  /** \} */
  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметров поверхности.
      \en \name Functions for get of the group of data inside and outside the surface's domain of parameters.
      \{ */
  //virtual void    Explore( double & u, double & v, bool ext,
  //                         MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer,
  //                         MbVector3D * uuDer, MbVector3D * vvDer, MbVector3D * uvDer, MbVector3D * nor ) const = 0;
  /** \} */
  /** \ru \name Функции движения по поверхности
      \en \name Functions of moving along the surface
      \{ */
  // \ru Вычисление шага аппроксимации с учетом радиуса кривизны \en Calculation of step of approximation with consideration of curvature radius 
  //virtual double  StepU         ( double u, double v, double sag ) const = 0; // \ru Вычисление шага по u по заданной стрелке прогиба. \en Calculation of the parameter step in u direction by the sag.
  //virtual double  StepV         ( double u, double v, double sag ) const = 0; // \ru Вычисление шага по v по заданной стрелке прогиба. \en Calculation of the parameter step in v direction by the sag.
  //virtual double  DeviationStepU( double u, double v, double angle ) const = 0; // \ru Вычисление шага по u по заданному углу отклонения. \en Calculation of the parameter step in u direction by the deviation angle.
  //virtual double  DeviationStepV( double u, double v, double angle ) const = 0; // \ru Вычисление шага по v по заданному углу отклонения. \en Calculation of the parameter step in v direction by the deviation angle.
  //virtual double  MetricStepU   ( double u, double v, double length ) const = 0; // \ru Вычисление шага по u по заданной длине. \en Calculation of the parameter step in u direction by the given length.
  //virtual double  MetricStepV   ( double u, double v, double length ) const = 0; // \ru Вычисление шага по v по заданной длине. \en Calculation of the parameter step in v direction by the given length.
  /** \} */
  /** \ru \name Общие функции поверхности
      \en \name Common functions of surface
      \{ */
  // \ru Ближайшая проекция точки на поверхность \en Nearest point projection onto the surface 
  MbeNewtonResult PointProjectionNewton( const MbCartPoint3D & p, size_t iterLimit,
                                                 double & u, double & v, bool ext ) const override; // \ru Функция для нахождения проекции точки на поверхность. \en Function for searching the point projection onto the surface.
  bool    NearPointProjection( const MbCartPoint3D & p, double & u, double & v, bool ext, MbRect2D * uvRange = nullptr ) const override; // \ru Ближайшая проекция точки на поверхность. \en The nearest point projection onto the surface.

  bool    IsRectangular() const override; // \ru Если true производные по u и v ортогональны. \en If true, then derivatives by u and v are orthogonal.
  //virtual void    SetLimit( double u1, double v1, double u2, double v2 ) = 0;
  /** \} */  
  /** \ru \name Функции элементарных поверхностей
      \en \name Functions of elementary surfaces
      \{ */

  /** \brief \ru Ближайшая проекция точки на поверхность.
             \en The nearest point projection onto the surface. \~
    \details \ru Ближайшая проекция точки на поверхность.
             \en The nearest point projection onto the surface. \~
    \param[in] p - \ru Проецируемая точка
                   \en Projecting point \~
    \param[in] init - \ru Если true, то входные параметры u, v считаются начальными приближениями
                      \en If true, then input parameters u and v are considered to be initial approximations \~ 
    \param[in,out] u - \ru Параметр проекции на поверхности
                       \en Parameter of projection onto the surface \~ 
    \param[in,out] v - \ru Параметр проекции на поверхности
                       \en Parameter of projection onto the surface \~
    \param[in] ext - \ru Признак поиска на продолжении поверхности
                     \en Attribute of search at the extension of a surface \~
    \param[in] uvRange - \ru Область поиска проекции
                         \en Region of search the projection \~
    \return \ru true в случае успеха операции
            \en True if the operation succeeded \~
  */ 
  virtual bool    GetPointProjection( const MbCartPoint3D & p, bool init, double & u, double & v, bool ext, MbRect2D * uvRange = nullptr ) const = 0;

  // Дать приращение параметра u и параметра v, соответствующее единичной длине в пространстве. \en Get increment of parameters, corresponding to the unit length in space. 
  void    GetParamsToUnit( double u, double v, double & uParam, double & vParam ) const override; 

  /** \brief \ru Добавить параметры в массив с заданным шагом.
             \en Add parameters to the array with given step. \~
    \details \ru Добавить параметры от w1 до w2 в массив с заданным шагом.
             \en Add parameters from w1 to w2 to the array with given step. \~ 
    \param[in] step - \ru Шаг
                      \en Step \~
    \param[in] maxCount - \ru Максимальное количество ячеек. \en Maximum count of cell. \~
    \param[out] ww - \ru Контейнер с параметрами
                     \en Container with parameters \~
  */ 
  void    AddTesselation( double step, size_t maxCount, double w1, double w2, SArray<double> & ww ) const;

  /// \ru Локальная система координат. \en A local coordinate system. 
  const   MbPlacement3D & GetPlacement() const { return position; }                   
  MbPlacement3D & SetPlacement() { return position; }
  /// \ru Установить локальную систему координат. \en Set the local coordinate system. 
  void    InitPlacement( const MbPlacement3D & p ) { position.Init( p ); }          
  /// \ru Является ли система координат ортонормированной. \en Whether the coordinate system is orthonormalized. 
  bool    IsPositionNormal()    const { return ( !position.IsAffine()    ); } 
  /// \ru Является ли система координат ортогональной и изотропной по осям. \en Whether the coordinate system is orthogonal and isotropic by the axes. 
  bool    IsPositionIsotropic() const { return (  position.IsIsotropic() ); } 
  /// \ru Является ли система координат ортогональной с равными по длине осями X,Y. \en Whether the coordinate system is orthogonal with X and Y axes equal by length. 
  bool    IsPositionCircular()  const { return (  position.IsCircular()  ); } 
  /** \} */ 
protected:
  void    Init_( const MbElementarySurface & ); // \ru Габарит и position \en Bounding box and 'position' 

private:
  void    operator = ( const MbElementarySurface & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS( MbElementarySurface )
};

IMPL_PERSISTENT_OPS( MbElementarySurface )


//-------------------------------------------------------------------------------
// \ru Проверить и итерационно уточнить проекцию точки на поверхность по направлению \en Check and refine point projection onto the surface by direction iteratively 
// \ru (вспомогательная функция StraightIntersection у конуса и сферы) \en (an auxiliary function StraightIntersection of cone and sphere) 
// ---
bool AddStraightIntersect( const MbCartPoint3D & pnt, // \ru Точка проекции, полученная решением квадратного уравнения \en Point of projection obtained by solving square equation 
                           bool specifySolution, // \ru Необходимость уточнения решения \en Necessity of solution refinement 
                           const MbSurface & surf, bool surfExt,
                           const MbCurve3D & curv, bool curvExt,
                           double mEps,
                           SArray<MbCartPoint> & uvArr,
                           SArray<double>      & ttArr );


#endif // __SURF_ELEMENTARY_SURFACE_H
