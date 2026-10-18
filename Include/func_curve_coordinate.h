////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Функция координаты кривой.
         \en The function by curve coordinate. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __FUNC_CURVE_COORDINATE_H
#define __FUNC_CURVE_COORDINATE_H


#include <function.h>
#include <curve3d.h>
#include <mb_matrix3d.h>


//------------------------------------------------------------------------------
/** \brief \ru Функция координаты кривой.
           \en The function by curve coordinate. \~
  \details \ru В качестве функции используется одна из координат кривой. \n
           \en One of the coordinates of the curve is used as a function. \n \~
  \ingroup Functions
*/
// ---
class MATH_CLASS MbCurveCoordinate : public MbFunction {
private :
  c3d::SpaceCurveSPtr curve;      ///< \ru Кривая. \en The curve.
  size_t              coordinate; ///< \ru Координата кривой: X(0), Y(1), Z(2). \en The curve coordinate: X(0), Y(1), Z(2).
  MbCartPoint3D       origin;     ///< \ru Точка отсчёта параметра функции. \en The origin point for parameter.
  MbVector3D          derive;     ///< \ru Вектор перемещения по параметру. \en The vector of movement by parameter.
  c3d::DoubleVector   tBreaks;    ///< \ru Параметры функции, в которых она терпит излом. \en The function parameters for which the function have a break.

private:
  ///< \ru Конструктор по кривой и индексу координаты. \en Constructor by curve and index of coordinate.
  MbCurveCoordinate( MbCurve3D & cur, size_t coord );
  MbCurveCoordinate( const MbCurveCoordinate & ); // \ru Не реализовано \en Not implemented 
  MbCurveCoordinate( const MbCurveCoordinate &, MbRegDuplicate * iReg );

public :
  virtual ~MbCurveCoordinate();

public:
  /// \ru Создание объекта. \en Object creation.
  static MbFunction * Create( const MbCurve3D & cur, size_t coord, const MbPlacement3D * place, MbResultType & res );

  // \ru Общие функции математического объекта \en Common functions of mathematical object 
  MbeFunctionType  IsA() const override;                  // \ru Тип элемента \en A type of element
  MbFunction & Duplicate() const override;                // \ru Сделать копию элемента \en Create a copy of the element 
  bool    IsSame   ( const MbFunction & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Являются ли объекты равными \en Determine whether objects are equal 
  bool    SetEqual ( const MbFunction & ) override;       // \ru Сделать равным \en Make equal 
  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 

  double  GetTMax()    const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter
  double  GetTMin()    const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter
  bool    IsClosed()   const override; // \ru Замкнутость кривой \en A curve closedness
  void    SetClosed( bool cl ) override; // \ru Замкнутость функции \en A function closedness 

  double  Value     ( double & t ) const override; // \ru Значение функции для t \en The value of function for a given t 
  double  FirstDer  ( double & t ) const override; // \ru Первая производная по t. Может вернуть значение MB_INFINITY. \en The first derivative with respect to t. May return MB_INFINITY.
  double  SecondDer ( double & t ) const override; // \ru Вторая производная по t. Может вернуть значение MB_QNAN. \en The second derivative with respect to t. May return MB_QNAN.
  double  ThirdDer  ( double & t ) const override; // \ru Третья производная по t. Может вернуть значение MB_QNAN. \en The third derivative with respect to t. May return MB_QNAN.

  double _Value     ( double t ) const override; // \ru Значение функции для t \en The value of function for a given t 
  double _FirstDer  ( double t ) const override; // \ru Первая производная по t. Может вернуть значение MB_INFINITY. \en The first derivative with respect to t. May return MB_INFINITY.
  double _SecondDer ( double t ) const override; // \ru Вторая производная по t. Может вернуть значение MB_QNAN. \en The second derivative with respect to t. May return MB_QNAN.
  double _ThirdDer  ( double t ) const override; // \ru Третья производная по t. Может вернуть значение MB_QNAN. \en The third derivative with respect to t. May return MB_QNAN.
  // \ru Вычислить значение и производные. \en Calculate value and derivatives of object for given parameter. \~
  void    Explore( double & t, bool ext,
                   double & val, double & fir, double * sec, double * thr ) const override;
  // \ru Вычислить аргумент t по значению функции. \en Calculate the argument t by the function value. 
  double  Argument( double & val ) const override;

  void    Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction
  double  Step( double t, double sag ) const override;
  double  DeviationStep( double t, double angle ) const override;

  double  MinValue  ( double & t ) const override; // \ru Минимальное  значение функции \en The minimum value of function 
  double  MaxValue  ( double & t ) const override; // \ru Максимальное значение функции \en The maximum value of function 
  double  MidValue  () const override; // \ru Среднее значение функции \en The middle value of function 
  bool    IsGood    () const override; // \ru Корректность функции \en Correctness of function 

  bool    IsConst() const override;
  bool    IsLine () const override;

  // \ru Создать функцию из части функции между параметрами t1 и t2 c выбором направления sense. \en Create a function in part of the function between the parameters t1 and t2 choosing the direction. 
  MbFunction * Trimmed( double t1, double t2, int sense ) const override; 
  // \ru Разбить функцию точкой с параметром t и вернуть отрезанную часть. \en Function break by the parameter t, and cut off part of the function: begs == true - save the initial half, beg == false - save the final half.
  MbFunction * BreakFunction( double t, bool beg ) override;

  void    SetOffsetFunc( double off, double scale ) override; // \ru Сместить функцию \en Shift a function 
  bool    SetLimitParam( double newTMin, double newTMax ) override; // \ru Установить область изменения параметра \en Set range of parameter 
  void    SetLimitValue( size_t n, double newValue ) override; // \ru Установить значение на конце ( 1 - в начале, 2 - в конце) \en Set the value at the end (1 - at beginning, 2 - at ending) 
  double  GetLimitValue( size_t n ) const override; // \ru Дать значение на конце ( 1 - в начале, 2 - в конце) \en Get the value at the end (1 - at beginning, 2 - at ending) 
  void    BreakPoints( std::vector<double> & vBreaks, double precision = ANGLE_REGION ) const override; // \ ru Определение точек излома функции. \en The determination of function smoothness break points.

private:
  static MbResultType CheckCurve( const MbCurve3D & curve, size_t coord );
  void SetOriginAndDerive();
  double GetCurveParam( double & t, bool ext ) const;
  double GetFuncParam( double w ) const;
  void ExploreFunctionDerivatives( double & t, bool ext, double * fir, double * sec, double * thr ) const;
  void    operator = ( const MbCurveCoordinate & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCurveCoordinate )
};

IMPL_PERSISTENT_OPS( MbCurveCoordinate )


#endif // __FUNC_CURVE_COORDINATE_H
