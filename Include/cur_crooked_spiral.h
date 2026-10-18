////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Спираль постоянного радиуса и осью, заданной произвольной кривой на плоскости XZ position.
         \en Spiral with constant radius and axis defined by an arbitrary curve on the XZ plane "position". \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_CROOKET_SPIRAL_H
#define __CUR_CROOKET_SPIRAL_H


#include <cur_spiral.h>


//------------------------------------------------------------------------------
/** \brief \ru Спираль с криволинейной осью.
           \en Spiral with a curvilinear axis. \~
  \details \ru Спираль постоянного радиуса и осью, заданной произвольной плоской кривой. 
    Ось спирали определяется кривой curve, располагающейся в плоскости ZX локальной системы координат спирали.
    При этом ось Z локальной системы координат спирали служит осью X системы координат двумерной кривой curve,
    а ось X локальной системы координат спирали служит осью Y системы координат двумерной кривой curve, 
    что приведено на рис. 1 ниже. \n
    Радиус-вектор кривой в методе PointOn(double&t,MbCartPoint3D&r) описывается векторной функцией: \n
    r(t) = position.origin + 
           (position.axisX (point.y + (radius cos(t) normal.ay)) + 
           (position.axisY radius sin(t)) +
           (position.axisZ (point.x + (radius cos(t) normal.ax)),
    где point - точка кривой curve, normal - нормаль кривой curve.
    Рис. 1.
  ^ Ось X локальной системы координат спирали является осью Y системы координат curve.
  | 
  |        curve(w)
  |  
  +----> Ось Z локальной системы координат спирали является осью X системы координат curve.
           \en Spiral with a constant radius and axis defined by an arbitrary plane curve. 
    Spiral axis is determined by the curve "curve" based in the ZX plane of the local coordinate system of spiral.
    The Z-axis of the local coordinate system is the X-axis of coordinate system of two-dimensional uv-curve "curve",
    and the X-axis of the local coordinate system is the Y-axis of the coordinate system of two-dimensional uv-curve "curve", 
    that is shown in fig. 1 below. \n
    The radius-vector of curve in the method PointOn(double&t,MbCartPoint3D&r) is described by a vector function: \n
    r(t) = position.origin + 
           (position.axisX (point.y + (radius cos(t) normal.ay)) + 
           (position.axisY radius sin(t)) +
           (position.axisZ (point.x + (radius cos(t) normal.ax)),
    where "point" is point of the curve "curve", "normal" is normal of the curve "curve".
    Fig. 1 1.
  ^ X-axis of the local coordinate system of the spiral is Y-axis of the coordinate system of the curve "curve".
  | 
  |        curve(w)
  |  
  +----> Z-axis of the local coordinate system of the spiral is X-axis of the coordinate system of the curve "curve". \~
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbCrookedSpiral : public MbSpiral {
  typedef std::vector<c3d::DoublePair> CurveParams;
protected:
  MbCurve *   curve;       ///< \ru Кривая, задающая ось спирали, (не может быть nullptr). \en The curve which determines the axis of the spiral, (can not be nullptr).
  double      radius;      ///< \ru Радиус спирали. \en A spiral radius. 
  double      wMin;        ///< \ru Минимальное значение  параметра curve. \en Minimal value of parameter "curve". 
  double      wMax;        ///< \ru Максимальное значение параметра curve. \en Maximal value of parameter "curve". 
  double      t0;          ///< \ru Начальный угол спирали. \en The initial angle of the spiral. 
  bool        curveSense;  ///< \ru Совпадение направления оси спирали с направлением кривой curve. \en The coincidence of the direction of the spiral axis with the direction of the curve "curve". 
  CurveParams curveParams; ///< \ru Параметры спирали (параметрические сдвиги от начала кривой) и параметры двумерной кривой. \en Parameters of spiral (parametric shifts from the beginning of the curve) and parameters of "curve".

protected:
  MbCrookedSpiral( const MbCrookedSpiral &, MbRegDuplicate * );
public :
  /// \ru Конструктор спирали с криволинейной осью. \en Spiral with a curvilinear axis.
  MbCrookedSpiral( const MbPlacement3D & pos, MbCurve & axisCurve, double radius, double step, bool same );
public:
  virtual ~MbCrookedSpiral();

public :
  VISITING_CLASS( MbCrookedSpiral );

  /// \ru Инициализация спирали по спирали. \en Spiral initialization by spiral.
  void    Init( const MbCrookedSpiral & );
  /// \ru Инициализация спирали по основанию (локальной системе координат). \en Spiral initialization by base (local coordinate system). 
  void    Init( const MbPlacement3D & );

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeSpaceType  IsA() const override; // \ru Тип элемента \en Type of element 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  bool    IsSame( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override;
  bool    SetEqual( const MbSpaceItem & init ) override;   // \ru Сделать равным \en Make equal 

  void    GetProperties( MbProperties & ) override;  // \ru Выдать свойства объекта \en Get properties of the object 
  void    SetProperties( const MbProperties & ) override;  // \ru Записать свойства объекта \en Set properties of the object 

  // \ru Общие функции кривой \en Common functions of curve 

  bool    IsClosed() const override; // \ru Проверка замкнутости кривой \en Check for curve closedness 
  // \ru Функции для работы внутри области определения кривой. \en Functions for working inside of the curve domain. \~
  void    PointOn  ( double & t, MbCartPoint3D & pnt ) const override; // \ru Точка на кривой \en Point on the curve 
  void    FirstDer ( double & t, MbVector3D & fd ) const override; // \ru Первая производная \en First derivative 
  void    SecondDer( double & t, MbVector3D & sd ) const override; // \ru Вторая производная \en Second derivative 
  void    ThirdDer ( double & t, MbVector3D & td ) const override; // \ru Третья производная по t \en Third derivative with respect to t 
  // \ru Функции для работы внутри и вне области определения кривой. \en Functions for working inside and outside of the curve domain. \~
  void   _PointOn  ( double t,  MbCartPoint3D & pnt ) const override; // \ru Точка на кривой \en Point on the curve
  void   _FirstDer ( double t,  MbVector3D & fd ) const override; // \ru Первая производная \en First derivative 
  void   _SecondDer( double t,  MbVector3D & sd ) const override; // \ru Вторая производная \en Second derivative 
  void   _ThirdDer ( double t,  MbVector3D & td ) const override; // \ru Третья производная по t \en Third derivative with respect to t 
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void    Explore( double & t, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const override;

  void    Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction

  // \ru Построить усеченную кривую c учетом заданной трехмерной точности. \en Construct a trimmed curve with the given three-dimensional accuracy.
  MbCurve3D * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyzEps, bool saveParamLenAndLaw ) const override;
  using MbCurve3D::Trimmed; // \ru Метод базового класса MbCurve3D без учета трехмерной точности. Точность по умолчанию - MbDimAccuracy::threeDimAcc.  \en Method of MbCurve3D base class without three-dimensional accuracy. The default accuracy is MbDimAccuracy::threeDimAcc.

  double  CalculateLength( double t1, double t2 ) const override;
  void    GetBasisItems( RPArray<MbSpaceItem> & ) override;

  // \ru Функции спирали \en Functions of spiral 

  bool    SetStep( double s ) override;                      // \ru Изменить шаг \en Change step 
  double  GetSpiralRadius( double t ) const override;        // \ru Выдать физический радиус спирали \en Get physical radius of spiral

  const MbCurve & GetAxisCurve() const  { return *curve; }        ///< \ru Выдать осевую кривую. \en Get axial curve. 
  double  GetSpiralRadius() const     { return radius; }  ///< \ru Выдать радиус. \en Get radius. 
  void    SetSpiralRadius( double r ) { radius = r; };    ///< \ru Изменить радиус. \en Change radius. 
  bool    GetCurveSense () const { return curveSense; }   ///< \ru Выдать признак совпадения направления на спирали и оси (кривой). \en Get attribute of coincidence of the direction on the spiral and axis (curve).

private:
  void    GetFirstDerNormW  ( const MbVector & fDerW, const MbVector & sDerW, MbVector & fdNormW ) const; // \ru Выдать первую производную нормали по параметру кривой оси \en Get the first derivative of normal vector with respect to parameter of axis curve 
  void    GetSecondDerNormW ( const MbVector & fDerW, const MbVector & sDerW, const MbVector & tDerW, MbVector & sdNormW ) const; // \ru Выдать вторую производную нормали по параметру кривой оси \en Get the second derivative of normal vector with respect to parameter of axis curve 
  void    GetThirdDerNormW  ( const MbVector & fDerW, const MbVector & sDerW, const MbVector & tDerW, MbVector & tdNormW ) const; // \ru Выдать третью производную нормали по параметру кривой оси \en Get the third derivative of normal vector with respect to parameter of axis curve 
  void    GetFirstDerNormT  ( const MbVector & fDerW, const MbVector & sDerW, MbVector & fdNorm ) const; // \ru Выдать первую производную нормали по параметру спирали \en Get the first derivative of normal vector with respect to parameter of the spiral 
  void    GetSecondDerNormT ( const MbVector & fDerW, const MbVector & sDerW, const MbVector & tDerW, MbVector & sdNorm ) const; // \ru Выдать вторую производную нормали по параметру спирали \en Get the second derivative of normal vector with respect to parameter of the spiral 
  void    GetThirdDerNormT  ( const MbVector & fDerW, const MbVector & sDerW, const MbVector & tDerW, MbVector & tdNorm ) const; // \ru Выдать третью производную нормали по параметру спирали \en Get the third derivative of normal vector with respect to parameter of the spiral 
  void    GetFirstDerT      ( const MbVector & fDerW, MbVector & fd ) const; // \ru Выдать первую производную кривой оси по параметру спирали \en Get the first derivative of axis curve with respect to parameter of the spiral 
  void    GetSecondDerT     ( const MbVector & fDerW, const MbVector & sDerW, MbVector & sd ) const; // \ru Выдать вторую производную кривой оси по параметру спирали \en Get the second derivative of axis curve with respect to parameter of the spiral 
  void    GetThirdDerT      ( const MbVector & fDerW, const MbVector & sDerW, const MbVector & tDerW, MbVector & td ) const; // \ru Выдать третью производную кривой оси по параметру спирали \en Get the third derivative of axis curve with respect to parameter of the spiral 
  double  GetFirstDerParamT ( const MbVector & fDerW ) const; // \ru Выдать первую производную параметра кривой оси по параметру спирали \en Get the first derivative of axis curve parameter with respect to parameter of the spiral 
  double  GetSecondDerParamT( const MbVector & fDerW, const MbVector & sDerW ) const; // \ru Выдать вторую производную параметра кривой оси по параметру спирали \en Get the second derivative of axis curve parameter with respect to parameter of the spiral 
  double  GetThirdDerParamT ( const MbVector & fDerW, const MbVector & sDerW, const MbVector & tDerW ) const; // \ru Выдать третью производную параметра кривой оси по параметру спирали \en Get the third derivative of axis curve parameter with respect to parameter of the spiral 
  void    GetCurveParams    ( double tSense, MbCartPoint & point, MbDirection & normal, 
                              MbVector & fDerW, MbVector & sDerW, MbVector & tDerW ) const; // \ru Параметры кривой оси, соответствующие параметру спирали t \en Parameters of axis curve corresponding to the parameter t of the spiral 
  void    CalculateParams   (); // \ru Посчитать параметры спирали (параметрические сдвиги от начала кривой) и параметры кривой. \en Calculate parameters of spiral (parametric shifts from the beginning of the curve) and parameters of "curve".
  bool    NearestLeftParams ( double tSense, c3d::DoublePair & paramPair ) const; // \ru Ближайшая слева пара параметров спирали и кривой. \en Nearest left parameters pair of spiral and "curve".

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCrookedSpiral )
  OBVIOUS_PRIVATE_COPY( MbCrookedSpiral )

}; // MbCrookedSpiral

IMPL_PERSISTENT_OPS( MbCrookedSpiral )


#endif // __CUR_CROOKET_SPIRAL_H
