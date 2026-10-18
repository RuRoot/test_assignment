////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Спираль.
         \en Spiral. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_SPIRAL_H
#define __CUR_SPIRAL_H


#include <mb_placement3d.h>
#include <mb_cube.h>
#include <curve3d.h>


//------------------------------------------------------------------------------
/** \brief \ru Спираль.
           \en Spiral. \~
  \details \ru Родительский класс спиралей: MbConeSpiral, MbCrookedSpiral, MbCurveSpiral. \n
           \en Parent class for spirals: MbConeSpiral, MbCrookedSpiral, MbCurveSpiral. \n \~
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbSpiral: public MbCurve3D {
protected:
  MbPlacement3D   position; ///< \ru Локальная система координат (положение центра). \en Local coordinate system (location of center). 
  double          step;     ///< \ru Шаг спирали. \en A pitch of spiral. 
  double          tmin;     ///< \ru Минимальное значение  параметра спирали. \en Minimal value of parameter of spiral. 
  double          tmax;     ///< \ru Максимальное значение параметра спирали. \en Maximal value of parameter of spiral. 

  /** \brief \ru Метрическая длина кривой. 
             \en Metric length of a curve. \~
      \details \ru Метрическая длина кривой, рассчитывается только при запросе длины объекта. Метрическая длина кривой в конструкторе кривой и после модификации кривой принимает отрицательное значение.
               \en Metric length of a curve is calculated only at the request. Metric length of a curve is undefined (negative) after object constructor and after object modifications. \n \~
  */
  mutable atomic_double  metricLength; 
  /** \brief \ru Габаритный куб кривой. 
             \en Bounding box of a curve. \~
      \details \ru Габаритный куб кривой, рассчитывается только при запросе габарита объекта. Габаритный куб в конструкторе кривой и после модификации кривой принимает неопределенное значение.
               \en Bounding box of a curve is calculated only at the request. Bounding box of a curve is undefined after object constructor and after object modifications. \n \~
  */
  mutable MbCube  cube;

protected:
  /// \ru Конструктор по умолчанию. \en The default constructor.
  MbSpiral()                           : position(),     step( 1.0 ), tmin( 0.0 ), tmax( M_PI2 ), metricLength( -1.0 ), cube() {}
  /// \ru Конструктор по локальной системе координат. \en The constructor by local coordinate system.
  MbSpiral( const MbPlacement3D & pl ) : position( pl ), step( 1.0 ), tmin( 0.0 ), tmax( M_PI2 ), metricLength( -1.0 ), cube() {}
protected:
  /// \ru Конструктор по локальной системе координат, высоте и шагу между витками. \en The constructor by local coordinate system, height and step between coils of spiral.
  MbSpiral( const MbPlacement3D &, double height, double step );
  /** \brief \ru Конструктор по локальной системе координат, шагу между витками и параметрам. 
             \en The constructor by local coordinate system, step between coils of spiral and parameters. \~
      \details \ru Конструктор по локальной системе координат, шагу между витками, начальному и конечному параметрам.
               \en The constructor by local coordinate system, step between coils of spiral, starting and ending parameters. \n \~
  */
  MbSpiral( const MbPlacement3D &, double step, double t1, double t2 );
  /** \brief \ru Конструктор по трем разным точкам и ненулевому шагу.
             \en The constructor by three different points and non-zero step. \~
    \details \ru Конструктор по трем разным точкам и ненулевому шагу. \n
             \en The constructor by three different points and non-zero step. \n \~
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
  */
  MbSpiral( const MbCartPoint3D & p0, const MbCartPoint3D & p1, const MbCartPoint3D & p2, double step, bool left = false );
  /// \ru Конструктор по другой спирали. \en Constructor by another spiral.
  MbSpiral( const MbSpiral & );
public :
  virtual ~MbSpiral();

public :
  VISITING_CLASS( MbSpiral );

  /// \ru Установить параметры спирали по другой спирали. \en Set spiral parameters by another spiral.
  void    Init( const MbSpiral & );
  /// \ru Установить другую локальную систему координат. \en Replace local coordinate system.
  void    Init( const MbPlacement3D & );
  /// \ru Установить высоту и шаг. \en Set height and step between coils of spiral.
  bool    Init( double height, double st );
  /// \ru Установить локальную систему координат, высоту и шаг спирали. \en Set local coordinates system, height and step of spiral.
  bool    Init( const MbPlacement3D & place, double height, double st );

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  //virtual MbeSpaceType  IsA()  const = 0; // \ru Тип элемента \en Type of element
  MbeSpaceType  Type() const override;     // \ru Тип элемента \en Type of element 
  //virtual MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const = 0;                   // \ru Сделать копию элемента \en Create a copy of the element
  bool    IsSame( const MbSpaceItem &, double accuracy = LENGTH_EPSILON ) const override;
  bool    SetEqual( const MbSpaceItem & ) override;   // \ru Сделать равным \en Make equal
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;     // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void    Move( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void    Rotate( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis
  void    AddYourGabaritTo( MbCube & ) const override; // \ru Добавь свой габарит в куб \en Add your own bounding box into the cube 
  void    PrepareIntegralData( const bool forced ) const override; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.

  //virtual void    GetProperties( MbProperties & ) = 0;  // \ru Выдать свойства объекта \en Get properties of the object
  //virtual void    SetProperties( const MbProperties & ) = 0;  // \ru Записать свойства объекта \en Set properties of the object
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  void    Refresh() override; // \ru Сбросить все временные данные \en Reset all temporary data 

  // \ru Общие функции кривой \en Common functions of curve 

  double  GetTMin() const override;  // \ru Вернуть минимальное значение параметра \en Get the minimum value of the parameter 
  double  GetTMax() const override;  // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter 
  bool    IsClosed() const override; // \ru Проверка замкнутости кривой \en Check for curve closedness 
  // \ru Функции кривой для работы в области определения параметрической кривой \en Functions of curve for working at parametric curve domain 
  //virtual void    PointOn  ( double & t, MbCartPoint3D & ) const = 0; // \ru Точка на кривой \en Point on the curve
  //virtual void    FirstDer ( double & t, MbVector3D & ) const = 0;    // \ru Первая производная \en The first derivative
  //virtual void    SecondDer( double & t, MbVector3D & ) const = 0;    // \ru Вторая производная \en The second derivative
  //virtual void    ThirdDer ( double & t, MbVector3D & ) const = 0;    // \ru Третья производная по t \en The third derivative with respect to t
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  //virtual void    Explore( double & t, bool ext,
  //                         MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const = 0;

  //virtual void    Inverse( MbRegTransform * iReg = nullptr ) = 0; // \ru Изменить направление \en Change direction

  double  GetMetricLength() const override;      // \ru Выдать метрическую длину ограниченной кривой \en Get metric length of bounded curve 
  double  GetLengthEvaluation() const override;  // \ru Оценка метрической длины кривой \en Estimation of metric length of the curve 

  MbNurbs3D * NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;

  double  Curvature( double t ) const override;          // \ru Кривизна \en Curvature 
  double  Step( double t, double sag ) const override;   // \ru Вычисление шага аппроксимации \en Calculation of step of approximation 
  double  DeviationStep( double t, double angle ) const override;

  size_t  GetCount() const override;
  double  GetSpiralPeriod() const; // \ru Вернуть период \en Get period 

  // \ru Заполнить плейсемент, если кривая плоская \en Fill the placement if the curve is planar 
  bool    GetPlacement( MbPlacement3D & place, PlanarCheckParams params = PlanarCheckParams() ) const override;

  /// \ru Является ли объект смещением \en Whether the object is a shift 
  bool    IsShift( const MbSpaceItem &, MbVector3D &, bool & isSame, double accuracy = LENGTH_EPSILON ) const override;
  void    GetCentre( MbCartPoint3D & c ) const override; // \ru Выдать центр \en Get center  

  // \ru Функции спирали \en Functions of spiral 

  virtual bool    SetStep( double s ) = 0;                // \ru Изменить шаг \en Change step 
  virtual double  GetSpiralRadius ( double t ) const = 0; // \ru Выдать физический радиус спирали \en Get physical radius of spiral 

  void    CheckParam( double & t ) const;
  /// \ru Дать направление спирали. \en Get direction of spiral. 
  void    GetDirection ( MbVector3D & v ) const { v = position.GetAxisZ();  }
  /// \ru Дать ось спирали. \en Get axis of spiral. 
  bool    GetAxis( MbAxis3D & axis ) const;
  /// \ru Выдать шаг. \en Get step. 
  double  GetStep() const { return step;  }
  /// \ru Выдать физический шаг спирали. \en Get physical pitch of spiral. 
  double  GetSpiralStep() const;
  /// \ru Выдать полный угол спирали \en Get full angle of spiral 
  double  GetAngle() const { return tmax-tmin; }
  /// \ru Изменить граничный угол. \en Change boundary angle. 
  bool    SetTMin( double t )
  { 
    C3D_ASSERT( t < tmax );
    if ( t < tmax ) {
      tmin = t;
      Refresh();
      return true;
    }
    return false;
  }
  /// \ru Изменить граничный угол. \en Change boundary angle. 
  bool    SetTMax( double t )
  { 
    C3D_ASSERT( t > tmin );
    if ( t > tmin ) {
      tmax = t;
      Refresh();
      return true;
    }
    return false;
  }
  /// \ru Изменить граничные углы. \en Change boundary angles. 
  bool    SetLimit( double t1, double t2 ) 
  {
    if ( t1 > t2 )
      std::swap( t1, t2 );
    C3D_ASSERT( t1 < t2 );
    if ( t1 < t2 ) {
      tmin = t1;
      tmax = t2;
      Refresh();
      return true;
    }
    return false;
  }

  const   MbPlacement3D & GetPlacement() const { return position; }
  bool    IsPositionNormal() const { return ( !position.IsAffine() ); }

protected:
  static  bool    IsNonZeroStep( double s ) { return (::fabs( s ) >= METRIC_EPSILON); }
private:
  void    operator = ( const MbSpiral & ); // \ru Не реализовано. \en Not implemented. 

DECLARE_PERSISTENT_CLASS( MbSpiral )
};

IMPL_PERSISTENT_OPS( MbSpiral )


//------------------------------------------------------------------------------
// \ru Проверка параметра кривой \en Check parameter of curve 
//---
inline
void MbSpiral::CheckParam( double & t ) const
{
  if ( t < tmin )
    t = tmin;
  if ( t > tmax )
    t = tmax;
}


//------------------------------------------------------------------------------
// \ru Выдать физический шаг спирали \en Get physical pitch of spiral 
//---
inline
double MbSpiral::GetSpiralStep() const
{
  if ( position.IsNormal() )
    return step;
  else if ( position.IsOrthogonal() ) {
    return (step * position.GetAxisZ().Length());
  }
  return 0.0;
}


#endif // __CUR_SPIRAL_H
