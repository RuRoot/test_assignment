////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Кубический сплайн функция.
         \en Cubic spline function. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __FUNC_CUBIC_SPLINE_FUNCTION_H
#define __FUNC_CUBIC_SPLINE_FUNCTION_H


#include <function.h>
#include <templ_s_array.h>


//------------------------------------------------------------------------------
/** \brief \ru Кубический сплайн функция.
           \en Cubic spline function. \~
  \details \ru Кубический сплайн функция. \n
           \en Cubic spline function. \n \~
  \ingroup Functions
*/
// ---
class MATH_CLASS MbCubicSplineFunction : public MbFunction {
protected:
  SArray<double> valueList;  ///< \ru Характерные точки. \en The control points. 
  SArray<double> secondList; ///< \ru Вторые производные в контрольных точках. \en Second derivatives in control points. 
  SArray<double> tList;      ///< \ru Значение параметров на кривой, которую моделирует кубический сплайн. \en The values of parameters on a curve which is modeled by a cubic spline. 
  bool           closed;     ///< \ru Признак замкнутости кривой. \en An attribute of curve closeness. 
  ptrdiff_t      uppIndex;   ///< \ru Количество интервалов (число точек - 1). \en The number of intervals (a number of points - 1). 

public :
  /// \ru Конструктор по точкам и признаку замкнутости. \en Constructor by points and an attribute of closeness. 
  MbCubicSplineFunction( const SArray<double> & values, bool cls );
  /// \ru Конструктор по точкам, параметрам и признаку замкнутости. \en Constructor by points, parameters and an attribute of closeness. 
  MbCubicSplineFunction( const SArray<double> & values, const SArray<double> & params, bool cls );
private:
  MbCubicSplineFunction( const MbCubicSplineFunction & );
public :
  virtual ~MbCubicSplineFunction();

public:
  /// \ru Инициализация по точкам, параметрам и признаку замкнутости. \en Initialization by points, parameters and an attribute of closeness. 
  void    Init( const SArray<double> & values,   // \ru Инициализация переменных \en Initialization of variables 
                        const SArray<double> & params, bool cls );
public:
  // \ru Общие функции математического объекта \en Common functions of mathematical object 
  MbeFunctionType  IsA      () const override;                // \ru Тип элемента \en A type of element
  MbFunction & Duplicate() const override;                // \ru Сделать копию элемента \en Create a copy of the element
  bool    IsSame   ( const MbFunction & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Являются ли объекты равными \en Determine whether objects are equal 
  bool    SetEqual ( const MbFunction & ) override;       // \ru Сделать равным \en Make equal 
  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of object 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of object 

  double  GetTMax  () const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter
  double  GetTMin  () const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter
  bool    IsClosed () const override; // \ru Замкнутость кривой \en A curve closeness
  void    SetClosed( bool cl ) override; // \ru Замкнутость функции \en A function closeness

  double  Value     ( double & t ) const override; // \ru Значение функции для t \en The value of function for a given t
  double  FirstDer  ( double & t ) const override; // \ru Первая производная по t \en The first derivative with respect to t
  double  SecondDer ( double & t ) const override; // \ru Вторая производная по t \en The second derivative with respect to t
  double  ThirdDer  ( double & t ) const override; // \ru Третья производная по t \en The third derivative with respect to t

  double _Value     ( double t ) const override; // \ru Значение функции для t \en The value of function for a given t
  double _FirstDer  ( double t ) const override; // \ru Первая производная по t \en The first derivative with respect to t
  double _SecondDer ( double t ) const override; // \ru Вторая производная по t \en The second derivative with respect to t
  double _ThirdDer  ( double t ) const override; // \ru Третья производная по t \en The third derivative with respect to t
  // \ru Вычислить значение и производные. \en Calculate value and derivatives of object for given parameter. \~
  void    Explore( double & t, bool ext,
                   double & val, double & fir, double * sec, double * thr ) const override;

  void    Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction
  double  Step( double t, double sag ) const override;
  double  DeviationStep( double t, double angle ) const override;

  double  MinValue  ( double & t ) const override; // \ru Минимальное  значение функции \en The minimum value of function
  double  MaxValue  ( double & t ) const override; // \ru Максимальное значение функции \en The maximum value of function
  double  MidValue  () const override; // \ru Среднее значение функции \en The middle value of function
  bool    IsGood    () const override; // \ru Корректность функции \en Correctness of function

  bool    IsConst() const override;
  bool    IsLine () const override;

  void    SetOffsetFunc( double off, double scale ) override; // \ru Сместить функцию \en Shift a function 
  bool    SetLimitParam( double newTMin, double newTMax ) override; // \ru Установить область изменения параметра \en Set range of parameter 
  void    SetLimitValue( size_t n, double newValue ) override; // \ru Установить значение на конце ( 1 - в начале, 2 - в конце) \en Set the value at the end (1 - at start point, 2 - at end point) 
  double  GetLimitValue( size_t n ) const override; // \ru Дать значение на конце ( 1 - в начале, 2 - в конце) \en Get the value at the end (1 - at start point, 2 - at end point) 
  bool    InsertValue( double t, double newValue ) override; // \ru Установить значение для параметра t. \en Set the value for the pdrdmeter t. 

  // \ru Создать функцию из части функции между параметрами t1 и t2 c выбором направления sense. \en Create a function in part of the function between the parameters t1 and t2 choosing the direction. 
  MbFunction * Trimmed( double t1, double t2, int sense ) const override; 
  // \ru Разбить функцию точкой с параметром t и вернуть отрезанную часть. \en Function break by the parameter t, and cut off part of the function: begs == true - save the initial half, beg == false - save the final half.
  MbFunction * BreakFunction( double t, bool beg ) override;
  MbFunction * Break( double t1, double t2 ) const; ///< \ru Выделить часть функции. \en Select a part of a function. 

  size_t  GetParamsCount() const; ///< \ru Выдать количество параметров. \en Get count of parameters. 
  double  GetParam( size_t index ) const; // \ru Дать значение параметра точки по номеру \en Get the value of point parameter by its number  
  void    GetTList( SArray<double> & params ) const; ///< \ru Выдать параметры. \en Get parameters tList. 
  size_t  GetValuesCount() const; // \ru Выдать количество опорных точек \en Get the number of control points 
  double  GetValue( size_t index ) const; // \ru Дать значение точки по номеру \en Get the value of point by its number  
  bool    SetValue( size_t index, double v ); // \ru Установить значение точки по номеру \en Set the value of point by its number  
  void    GetValueList( SArray<double> & vals ) const; ///< \ru Вернуть массив контрольных значений. \en Get array of control values.
  double  GetDerive( size_t index ) const; // \ru Дать значение производной по номеру \en Get the value of derivative by its number  
  bool    CalcSecondDerives(); // \ru Расчет вторых производных \en Calculation of second derivatives 

private:
  double  Value   ( double t, size_t num ) const;  // \ru Точка на кривой \en The point on the curve 
  double  FirstDer( double t, size_t num ) const;  // \ru Первая производная \en First derivative 
  ptrdiff_t GetIndex( double t ) const; 
  bool    CalcClosedSpline  (); // \ru Расчет вторых производных в узлах для замкнутой кривой \en Calculation of second derivatives in nodes of closed curve 
  bool    CalcUnClosedSpline(); // \ru Расчет вторых производных в узлах для разомкнутой кривой \en Calculation of second derivatives in nodes of unclosed curve 
  bool    DefineIntervalPar( double & t, size_t & num ) const; // \ru Определение принадлежности интервалу параметров \en Check belonging to interval of parameters 
private:
  void    operator = ( const MbCubicSplineFunction & );   // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCubicSplineFunction )
};

IMPL_PERSISTENT_OPS( MbCubicSplineFunction )


#endif // __FUNC_CUBIC_SPLINE_FUNCTION_H
