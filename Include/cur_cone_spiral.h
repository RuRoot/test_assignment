////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Коническая спираль.
         \en Conical spiral. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_CONE_SPIRAL_H
#define __CUR_CONE_SPIRAL_H


#include <cur_spiral.h>


//------------------------------------------------------------------------------
/** \brief \ru Коническая спираль.
           \en Conical spiral. \~
  \details \ru Плоская, коническая или цилиндрическая спираль.
    Ось спирали направлена вдоль оси Z локальной системы координат. \n
    Параметр кривой отсчитывается от оси position.axisX локальной системы координат. 
    Радиус-вектор кривой в методе PointOn(double&t,MbCartPoint3D&r) описывается векторной функцией: \n
    для плоской спирали: \n
    r(t) = position.origin + (position.axisX rad cos(t)) + (position.axisY rad sin(t)),
    где rad = radius + (stepd2pi t), если tgAlpha = pi/2 или rad = radius - (stepd2pi t), если tgAlpha = -pi/2;
    для цилиндрической спирали: \n
    r(t) = position.origin + (position.axisZ stepd2pi t) + (position.axisX radius cos(t)) + (position.axisY radius sin(t));
    для конической спирали: \n
    r(t) = position.origin + (position.axisZ stepd2pi t) + (position.axisX rad cos(t)) + (position.axisY rad sin(t)),
    где rad = radius + (tgAlpha stepd2pi t). \n
           \en Planar, Conical or cylindrical spiral.
    A spiral axis is directed along the Z-axis of the local coordinate system. \n
    The curve parameter is measured from the axis "position.axisX" of the local coordinate system. 
    The radius-vector of curve in the method PointOn(double&t,MbCartPoint3D&r) is described by a vector function: \n
    for planar spiral: \n
    r(t) = position.origin + (position.axisX rad cos(t)) + (position.axisY rad sin(t)),
    where rad = radius + (stepd2pi t), if tgAlpha = pi/2 or rad = radius - (stepd2pi t), if tgAlpha = -pi/2;
    for cylindrical spiral: \n
    r(t) = position.origin + (position.axisZ stepd2pi t) + (position.axisX radius cos(t)) + (position.axisY radius sin(t));
    for conical spiral: \n
    r(t) = position.origin + (position.axisZ stepd2pi t) + (position.axisX radius cos(t)) + (position.axisY radius sin(t));
    where rad = radius + (tgAlpha stepd2pi t). \n \~
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbConeSpiral : public MbSpiral
{
public:
  enum ConeSpiralType {
    cst_Plane,         ///< \ru Плоская спираль. \en Planar spiral. 
    cst_Cylindrical,   ///< \ru Цилиндрическая спираль. \en Cylindrical spiral. 
    cst_Conical,       ///< \ru Коническая спираль. \en Conical spiral. 
  };
private:
  ConeSpiralType type; ///< \ru Тип спирали. \en A spiral type. 
protected:
  double  radius;      ///< \ru Радиус основания. \en Bottom radius. 
  double  tgAlpha;     ///< \ru Тангенс угла полуконуса. \en Tangent of the semicone angle. 
  double  stepd2pi;    ///< \ru Шаг спирали, деленный на 2*pi. \en Spiral step divided by 2*pi. 

protected:
  /// \ru Цилиндрическая спираль по трем разным точкам и ненулевому шагу. \en Cylindrical spiral by three different points and non-zero step. 
  MbConeSpiral( const MbCartPoint3D & p0, const MbCartPoint3D & p1, const MbCartPoint3D & p2, double st, bool left = false );
  /// \ru Коническая спираль по основанию, радиусам двух оснований и высоте с шагом. \en Conical spiral by the base, radii of the two bases and height with step. 
  MbConeSpiral( const MbPlacement3D & pl, double radius1, double radius2, double height, double st );
  /// \ru Коническая спираль по радиусу, углу уклона, высоте с шагом, и основанию. \en Conical spiral by radius, inclination angle, height with step and base. 
  MbConeSpiral( double radius, double angle, double height, double st, const MbPlacement3D & pl );
  /// \ru Цилиндрическая спираль по радиусу основания, шагу, основанию, и двум параметрам. \en Cylindrical spiral by bottom radius, step, base and two parameters. 
  MbConeSpiral( double radius, double st, const MbPlacement3D & pl, double t1, double t2 );
  /// \ru Коническая спираль по радиусу основания, ненулевой высоте, тангенсу угла наклона, основанию, и параметрам. \en Conical spiral by bottom radius, non-zero height, tangent of inclination angle, base and parameters. 
  MbConeSpiral( double r0, double h, double tgAH, const MbPlacement3D & pl, double u1, double v1, double u2, double v2 );

protected:
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbConeSpiral( const MbConeSpiral & );
public:
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbConeSpiral();

public:
  /** \brief \ru Создать цилиндрическую спираль по трем разным точкам и ненулевому шагу.
             \en Create cylindrical spiral by three different points and non-zero step. \~
    \details \ru Создать цилиндрическую спираль по трем разным точкам и ненулевому шагу. \n
             \en Create cylindrical spiral by three different points and non-zero step. \n \~
    \param[in] p0  - \ru Начало локальной системы координат.
                     \en Origin point of the local coordinate system. \~
    \param[in] p1  - \ru Точка в направлении оси Z, определяющая высоту спирали.
                     \en Point in the direction of the Z axis determining the height of the spiral. \~
    \param[in] p2  - \ru Точка в направлении оси X.
                     \en Point in the direction of the X axis. \~
    \param[in] st  - \ru Шаг между витками спирали.
                     \en Step between coils of spiral. \~
    \param[in] left - \ru Признак левой системы координат.
                      \en Flag of the left coordinate system. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbConeSpiral * Create( const MbCartPoint3D & p0, const MbCartPoint3D & p1, const MbCartPoint3D & p2, double st, bool left = false );
  /** \brief \ru Создать коническую спираль по основанию, радиусам двух оснований и высоте с шагом.
             \en Create conical spiral by the base, radii of the two bases and height with step. \~
    \details \ru Создать коническую спираль по основанию, радиусам двух оснований и высоте с ненулевым шагом. \n
             \en Create conical spiral by the base, radii of the two bases and height with non-zero step. \n \~
    \param[in] pl - \ru Локальная система координат.
                    \en A local coordinate system. \~
    \param[in] radius1 - \ru Начальный радиус основания спирали.
                         \en Starting radius of spiral. \~
    \param[in] radius2 - \ru Конечный радиус основания спирали.
                         \en Ending radius of spiral. \~
    \param[in] height  - \ru Высота спирали.
                         \en Height of spiral. \~
    \param[in] st - \ru Шаг между витками спирали.
                    \en Step between coils of spiral. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbConeSpiral * Create( const MbPlacement3D & pl, double radius1, double radius2, double height, double st );

  /** \brief \ru Создать коническую спираль по радиусу, углу уклона, высоте с шагом, и основанию.
             \en Create conical spiral by radius, inclination angle, height, step and base. \~
    \details \ru Создать коническую спираль по радиусу, углу уклона, высоте, шагу между витками и основанию (локальной системе координат). \n
             \en Create conical spiral by radius, inclination angle, height, step between coils of spiral and base (locale coordinate system). \n \~
    \param[in] radius - \ru Радиус основания спирали.
                        \en Radius of spiral. \~
    \param[in] angle - \ru Угол уклона.
                       \en Inclination angle. \~
    \param[in] height - \ru Высота спирали.
                        \en Height of spiral. \~
    \param[in] st - \ru Шаг между витками спирали.
                    \en Step between coils of spiral. \~
    \param[in] pl - \ru Локальная система координат.
                    \en A local coordinate system. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbConeSpiral * Create( double radius, double angle, double height, double st, const MbPlacement3D & pl );
  /** \brief \ru Создать цилиндрическую спираль по радиусу основания, шагу, основанию, и двум параметрам.
             \en Create cylindrical spiral by bottom radius, step, base and two parameters. \~
    \details \ru Создать цилиндрическую спираль по радиусу основания, шагу, основанию, и двум параметрам. \n
             \en Create cylindrical spiral by bottom radius, step, base and starting and ending parameters. \n \~
    \param[in] radius - \ru Радиус основания спирали.
                        \en Radius of spiral. \~
    \param[in] st - \ru Шаг между витками спирали.
                    \en Step between coils of spiral. \~
    \param[in] pl - \ru Локальная система координат.
                    \en A local coordinate system. \~
    \param[in] t1 - \ru Начальный параметр.
                    \en Starting parameter. \~
    \param[in] t2 - \ru Конечный параметр.
                    \en Ending parameter. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbConeSpiral * Create( double radius, double st, const MbPlacement3D & pl, double t1, double t2 );

  /** \brief \ru Создать коническую спираль по радиусу основания, ненулевой высоте, тангенсу угла наклона, основанию, и параметрам.
             \en Create conical spiral by bottom radius, non-zero height, tangent of inclination angle, base and parameters. \~
    \details \ru Создать коническую спираль по отрезку на конусе : радиусу основания, ненулевой высоте, тангенсу угла наклона, основанию, и параметрам.
                 Конструктор предназначен для создания конической спирали по отрезку на конической поверхности. \n
             \en Create conical spiral by bottom radius, non-zero height, tangent of inclination angle, base and parameters. 
                 The constructor is used internally to create a conical spiral representing a segment on a conical surface. \n \~
    \param[in] r0 - \ru Радиус основания конуса.
                    \en Radius of spiral. \~
    \param[in] h - \ru Высота спирали.
                   \en Height of spiral. \~
    \param[in] tgAH - \ru Вспомогательная величина h*tan(angle) где angle - угол между осью Z и боковой образующей.
                      \en Auxiliary value h*tan(angle) where angle is angle between Z-axis and lateral generatrix. \~
    \param[in] pl - \ru Локальная система координат.
                    \en A local coordinate system. \~
    \param[in] u1 - \ru Начальный параметр по U.
                    \en Starting parameter by U. \~
    \param[in] v1 - \ru Начальный параметр по V.
                    \en Starting parameter by V. \~
    \param[in] u2 - \ru Конечный параметр по U.
                    \en Ending parameter by U. \~
    \param[in] v2 - \ru Конечный параметр по V.
                    \en Ending parameter by V. \~
    \warning \ru Для внутреннего использования.
             \en For internal use only. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbConeSpiral * Create( double r0, double h, double tgAH, const MbPlacement3D & pl, double u1, double v1, double u2, double v2 );

public:
  VISITING_CLASS( MbConeSpiral );

  /// \ru Инициализация конической спирали по конической спирали. \en Initialization of conical spiral by conical spiral. 
  void    Init( const MbConeSpiral & );
  /// \ru Инициализация цилиндрической спирали по основанию. \en Initialization of cylindrical spiral by base. 
  bool    Init( const MbPlacement3D &, bool resetToCylindrical );
  /// \ru Инициализация конической спирали по радиусам оснований, высоте и шагу. \en Initialization of conical spiral by bottom radii, height and step. 
  bool    Init( double radius1, double radius2, double height, double st );
  /// \ru Инициализация конической спирали по основанию, радиусам оснований, и высоте с шагом. \en Initialization of conical spiral by the base, radii of bases and height with step. 
  bool    Init( const MbPlacement3D & place, double radius1, double radius2, double height, double st );

public:
  // \ru Общие функции математического объекта. \en The common functions of the mathematical object. 
  MbeSpaceType IsA() const override; // \ru Получить тип. \en Get a type. 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Создать копию. \en Create a copy.
  bool    IsSame( const MbSpaceItem &, double accuracy = LENGTH_EPSILON ) const override;
    
  void    GetProperties( MbProperties & ) override;  // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & ) override;  // \ru Записать свойства объекта. \en Set properties of the object. 

  // \ru Общие функции кривой. \en Common functions of curve. 

  // \ru Функции для работы внутри области определения кривой. \en Functions for working inside of the curve domain. \~
  void    PointOn  ( double & t, MbCartPoint3D & pnt ) const override; // \ru Точка на кривой. \en Point on the curve.
  void    FirstDer ( double & t, MbVector3D & fd ) const override;    // \ru Первая производная. \en First derivative. 
  void    SecondDer( double & t, MbVector3D & sd ) const override;   // \ru Вторая производная. \en Second derivative. 
  void    ThirdDer ( double & t, MbVector3D & td ) const override;    // \ru Третья производная по t. \en The third derivative with respect to t. 
  // \ru Функции для работы внутри и вне области определения кривой. \en Functions for working inside and outside of the curve domain. \~
  void   _PointOn  ( double t, MbCartPoint3D & pnt ) const override;  // \ru Точка на кривой. \en Point on the curve.
  void   _FirstDer ( double t, MbVector3D & fd ) const override;     // \ru Первая производная. \en First derivative.
  void   _SecondDer( double t, MbVector3D & sd ) const override;    // \ru Вторая производная. \en Second derivative.
  void   _ThirdDer ( double t, MbVector3D & td ) const override;     // \ru Третья производная по t. \en The third derivative with respect to t.
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void    Explore( double & t, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const override;

  // \ru Продлить кривую. \en Extend the curve. \~
  MbResultType Extend( const MbCurveExtensionParameters3D & parameters, c3d::SpaceCurveSPtr & resCurve ) const override;

  double  Step         ( double t, double sag ) const override; // \ru Вычисление шага по стрелке прогиба. \en Calculation of parameter step by the sag.
  double  DeviationStep( double t, double angle ) const override; // \ru Вычисление шага по углу отклонения нормали. \en Calculation of parameter step by the deviation angle.   
  double  MetricStep   ( double t, double length ) const override; // \ru Вычисление шага параметра по длине. \en Calculation of parameter step by the given length. 

  MbCurve * GetMap( const MbMatrix3D &, MbRect1D * pRgn = nullptr,
                    VERSION version = Math::DefaultMathVersion(), bool * coincParams = nullptr ) const override;

  // \ru Построить усеченную кривую c учетом заданной трехмерной точности. \en Construct a trimmed curve with the given three-dimensional accuracy.
  MbCurve3D * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyzEps, bool saveParamLenAndLaw ) const override;
  using MbCurve3D::Trimmed; // \ru Метод базового класса MbCurve3D без учета трехмерной точности. Точность по умолчанию - MbDimAccuracy::threeDimAcc.  \en Method of MbCurve3D base class without three-dimensional accuracy. The default accuracy is MbDimAccuracy::threeDimAcc.

  void    Inverse( MbRegTransform * = nullptr ) override; // \ru Изменить направление. \en Change the direction.
  MbNurbs3D * NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;

  double  CalculateLength( double t1, double t2 ) const override;

  // \ru Ближайшая проекция точки на спираль. \en The nearest point projection on the spiral. 
  bool    NearPointProjection( const MbCartPoint3D & p, double & t, bool ext, MbRect1D * tRange = nullptr ) const override;

  // \ru Частные функции спирали. \en Special functions for spiral. 
  bool    SetStep( double s ) override;                // \ru Изменить шаг. \en Change the step. 
  double  GetSpiralRadius( double t ) const override;  // \ru Выдать радиус физический спирали. \en Get a radius of the physical spiral.
  void    GetPointsByEvenLengthDelta( size_t n,  std::vector<MbCartPoint3D> & pnts ) const override;  // \ru Выдать n точек кривой с равными интервалами по длине дуги. \en Get n points of curve with equal intervals along the length of the arc. 

  // \ru Дать поверхностную кривую, если пространственная кривая поверхностная (после использования вызывать DeleteItem на аргументы) \en Get a surface curve if spatial curve is lying on the surface (after the using call DeleteItem for arguments) 
  bool    GetSurfaceCurve( MbCurve *& curve2d, MbSurface *& surface, VERSION version = Math::DefaultMathVersion() ) const override;

  double  GetAlpha() const { return ::atan( tgAlpha ); }
  double  GetTgAlpha() const { return tgAlpha; }
  void    SetAlpha( double a ) { tgAlpha = ::tan( a ); Refresh(); }

  double  GetR() const { return radius; }
  void    FastInverse();
  double  GetStepD2PI() const { return stepd2pi; }
  void    GetSpiralDir( MbVector3D & dir ) const;

  /// \ru  Узнать тип конической спирали \en Learn the type of conical spiral.
  ConeSpiralType GetType() const { return type; }
private:
  /// \ru Усеченное значение tmax для случая закручивающихся в точку плоской или конической спиралей. \en Trimmed value tmax for the case of planar or conical spirals trailing to the point. 
  void    CalConeTMax();

  /// \ru Вернуть значение параметра, при котором плоская или коническая спираль "закручивается в точку" (радиус равен 0). Вернуть DBL_MAX, если такого значения не существует. \en Returns the parameter value, which the planar or conical spiral trails to the point (the radius is equal to 0) for. Return DBL_MAX if such value does not exist.
  double    CalConeT() const;

  /// \ru Текущий радиус. \en The current radius. 
  double  GetR( double t ) const;
  /// \ru Производная текущего радиуса. \en The derivative of the current radius. 
  double  GetRDerive( double t ) const;
  /// \ru Ближайшая проекция точки на плоскую спираль. \en The nearest point projection on the planar spiral. 
  bool    PlaneProjection( const MbCartPoint3D & pSpace, double & tProj, bool ext, MbRect1D * tRange ) const;
  /// \ru Ближайшая проекция точки на цилиндрическую спираль. \en The nearest point projection on the cylindrical spiral. 
  bool    CylindricalProjection( const MbCartPoint3D & pSpace, double & tProj, bool ext, MbRect1D * tRange ) const;

private:
  // \ru Объявление (перегрузка) оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en Declaration (overload) of the assignment operator without its implementation, to prevent the default assignment. 
  MbConeSpiral & operator = ( const MbConeSpiral & );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbConeSpiral )

};

IMPL_PERSISTENT_OPS( MbConeSpiral )


//------------------------------------------------------------------------------
// \ru Дать направление движения по спирали \en Give the direction of motion on a spiral 
// ---
inline
void MbConeSpiral::GetSpiralDir( MbVector3D & dir ) const
{
  dir.Init( position.GetAxisZ() );

  MbCartPoint3D pnt1, pnt2;
  double t = GetTMin();
  _PointOn( t, pnt1 );
  t += M_PI2;
  _PointOn( t, pnt2 );
  MbVector3D spiralV( pnt1, pnt2 );

  if ( spiralV * dir < -ANGLE_EPSILON )
    dir.Invert();
}


//------------------------------------------------------------------------------
// \ru Внутренний радиус \en Inner radius 
// ---
inline
double MbConeSpiral::GetR( double t ) const
{
  //C3D_ASSERT( position.IsIsotropic() || ::fabs(tgAlpha) < LENGTH_EPSILON ); // \ru Проверить корректность работы \en Check the correctness of working 
  switch ( type ) { // t = [0; 2*pi]
    case cst_Plane : return tgAlpha > 0.0 ? ( radius + stepd2pi * t ) : ( radius - stepd2pi * t );
    case cst_Cylindrical : return radius;
    default : return radius + tgAlpha * stepd2pi * t;
  }
}


//------------------------------------------------------------------------------
// \ru Производная внутреннего радиуса \en Derivative of inner radius 
// ---
inline
double MbConeSpiral::GetRDerive( double /*t */) const
{
  switch ( type ) { // t = [0; 2*pi]
    case cst_Plane : return tgAlpha > 0.0 ? stepd2pi : -stepd2pi;
    case cst_Cylindrical : return 0.0;
    default : return tgAlpha * stepd2pi;
  }
}


#endif // __CUR_CONE_SPIRAL_H
