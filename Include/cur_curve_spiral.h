////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Спираль переменного радиуса, именяющегося в соответствии с образующей кривой.
         \en Spiral with a variable radius which changes according to the generating curve. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUC_CURVE_SPIRAL_H
#define __CUC_CURVE_SPIRAL_H


#include <cur_spiral.h>


//------------------------------------------------------------------------------
/** \brief \ru Спираль переменного радиуса.
           \en Spiral with a variable radius. \~
  \details \ru Спираль переменного радиуса, именяющегося в соответствии с образующей кривой curve. \n
    Образующая кривая является двумерной и располагается в плоскости ZX локальной системы координат спирали.
    Радиус спирали равен второй координате точки образующей кривой.
    Ось спирали направлена вдоль оси Z локальной системы координат. \n
    Радиус-вектор кривой в методе PointOn(double&t,MbCartPoint3D&r) описывается векторной функцией: \n
    r(t) = position.origin + (position.axisZ step t / 2pi) + (position.axisX radius cos(t)) + (position.axisY radius sin(t)),
    где radius = curve(w).y;
           \en Spiral with a variable radius which are changed according to the generating curve "curve". \n
    Generating curve is two-dimensional and located in the ZX plane of the local coordinate system.
    A spiral radius is equal to the second point coordinate of generating curve.
    A spiral axis is directed along the Z-axis of the local coordinate system. \n
    The radius-vector of curve in the method PointOn(double&t,MbCartPoint3D&r) is described by a vector function: \n
    r(t) = position.origin + (position.axisZ step t / 2pi) + (position.axisX radius cos(t)) + (position.axisY radius sin(t)),
    where radius = curve(w).y; \~
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbCurveSpiral : public MbSpiral {
protected:
  MbCurve * curve;      ///< \ru Кривая, задающая закон изменения радиуса. \en Curve which determines the rule of radius changing. 
  double    wMin;       ///< \ru Минимальное значение  параметра кривой curve. \en Minimal parameter value of curve "curve". 
  double    wMax;       ///< \ru Максимальное значение параметра кривой curve. \en Maximal parameter value of curve "curve". 
  bool      curveSense; ///< \ru Совпадение направления оси спирали с направлением кривой curve. \en The coincidence of the direction of the spiral axis with the direction of the curve "curve". 

public:
  /** \brief \ru Создать цилиндрическую спираль по радиусу основания, шагу, основанию, и двум параметрам.
             \en Create cylindrical spiral by bottom radius, step, base and two parameters. \~
    \details \ru Создать цилиндрическую спираль по радиусу основания, шагу, основанию, и двум параметрам. \n
             \en Create cylindrical spiral by bottom radius, step, base and starting and ending parameters. \n \~
    \param[in] place - \ru Локальная система координат.
                       \en A local coordinate system. \~
    \param[in] radius - \ru Радиус основания спирали.
                        \en Radius of spiral. \~
    \param[in] st - \ru Шаг между витками спирали.
                    \en Step between coils of spiral. \~
    \param[in] t1 - \ru Начальный параметр.
                    \en Starting parameter. \~
    \param[in] t2 - \ru Конечный параметр.
                    \en Ending parameter. \~
  */
  MbCurveSpiral( const MbPlacement3D & place, double radius, double st, double t1, double t2 );
  /** \brief \ru Конструктор спирали по её основанию, закону изменения радиуса и шагу.
             \en The constructor of spiral by its base, the law of radius change and step. \~
    \details \ru Конструктор спирали по её основанию (локальной системе координат), закону изменения радиуса в виде образующей кривой и шагу между витками спирали. \n
             \en The constructor of spiral by its base (local coordinate system), the law of radius change (in the form of the 2D-curve) and step between coils of spiral. \n \~
    \param[in] place - \ru Локальная система координат.
                       \en A local coordinate system. \~
    \param[in] lawCurve  - \ru Закон изменения радиуса.
                           \en The law of radius change. \~
    \param[in] st - \ru Шаг между витками спирали.
                    \en Step between coils of spiral. \~
    \param[in] sameLawCurve - \ru Флаг использования оригинала кривой.
                              \en Flag of original curve use. \~
  */
  MbCurveSpiral( const MbPlacement3D & place, MbCurve & lawCurve, double st, bool sameLawCurve );
protected:
  MbCurveSpiral( const MbCurveSpiral & );
public :
  virtual ~MbCurveSpiral();

public:
  VISITING_CLASS( MbCurveSpiral );

          /// \ru Инициализация спирали по спирали. \en Spiral initialization by spiral.
  void    Init( const MbCurveSpiral & );
          /// \ru Инициализация спирали по основанию (локальной системе координат). \en Spiral initialization by base (local coordinate system). 
  void    Init( const MbPlacement3D & );

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeSpaceType  IsA() const override; // \ru Тип элемента \en Type of element 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  bool    IsSame( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override;
  bool    SetEqual( const MbSpaceItem & init ) override;   // \ru Сделать равным \en Make equal 

  void    GetProperties( MbProperties & ) override;  // \ru Выдать свойства объекта \en Get properties of the object 
  void    SetProperties( const MbProperties & ) override;  // \ru Записать свойства объекта \en Set properties of the object 
  void    GetBasisItems( RPArray<MbSpaceItem> & ) override;

  // \ru Общие функции кривой \en Common functions of curve 

  // \ru Функции для работы внутри области определения кривой. \en Functions for working inside of the curve domain. \~
  void    PointOn  ( double & t, MbCartPoint3D & pnt ) const override; // \ru Точка на кривой \en Point on curve
  void    FirstDer ( double & t, MbVector3D & fd ) const override; // \ru Первая производная \en First derivative
  void    SecondDer( double & t, MbVector3D & sd ) const override; // \ru Вторая производная \en Second derivative
  void    ThirdDer ( double & t, MbVector3D & td ) const override; // \ru Третья производная по t \en Third derivative with respect to t
  // \ru Функции для работы внутри и вне области определения кривой. \en Functions for working inside and outside of the curve domain. \~
  void   _PointOn  ( double t,  MbCartPoint3D & pnt ) const override; // \ru Точка на кривой \en Point on curve
  void   _FirstDer ( double t,  MbVector3D & fd ) const override; // \ru Первая производная \en First derivative
  void   _SecondDer( double t,  MbVector3D & sd ) const override; // \ru Вторая производная \en Second derivative
  void   _ThirdDer ( double t,  MbVector3D & td ) const override; // \ru Третья производная по t \en Third derivative with respect to t
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void    Explore( double & t, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const override;

  void    Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction

  // \ru Построить усеченную кривую c учетом заданной трехмерной точности. \en Construct a trimmed curve with the given three-dimensional accuracy.
  MbCurve3D * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyzEps, bool saveParamLenAndLaw ) const override;

  // \ru Функции спирали \en Functions of spiral 

  bool    SetStep( double s ) override;                // \ru Изменить шаг \en Change step
  double  GetSpiralRadius ( double t ) const override; // \ru Выдать физический радиус спирали \en Get physical radius of spiral

protected:
  void    Init( bool setLimits );
  double  GetRadiusValue( double t, double & r0, MbVector & derive ) const; // \ru Выдать радиус спирали \en Get the spiral radius 
  void    GetRadiusDerivative( MbVector & derive, double & r1 ) const;  // \ru Выдать первую производную радиуса \en Get the first derivative of the radius 
  void    GetRadiusDerivatives( double wPar, MbVector & derive, double & r1, double & r2, double & r3 ) const; // \ru Выдать первую, вторую и третью производные радиуса \en Get the first, second and third derivatives of the radius 

private:
  void    operator = ( const MbCurveSpiral & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCurveSpiral )
}; // MbCurveSpiral

IMPL_PERSISTENT_OPS( MbCurveSpiral )


#endif // __CUC_CURVE_SPIRAL_H
