////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Линейная функция.
         \en Linear function. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __FUNC_LINE_FUNCTION_H
#define __FUNC_LINE_FUNCTION_H


#include <function.h>


//------------------------------------------------------------------------------
/** \brief \ru Линейная функция.
           \en Linear function. \~
  \details \ru Линейная функция. \n
           \en Linear function. \n \~
  \ingroup Functions
*/
// ---
class MATH_CLASS MbLineFunction : public MbFunction {
public :
  double value1; ///< \ru Значение функции в начале области определения. \en A value of function in the beginning of its domain. 
  double value2; ///< \ru Значение функции в конце области определения. \en A value of function in the ending of its domain. 
  double tmin;   ///< \ru Начало области определения. \en Beginning of domain. 
  double tmax;   ///< \ru Конец области определения. \en Ending of domain. 

public :
  ///< \ru Конструктор по значениям и параметрам. \en Constructor by values and parameters. 
  MbLineFunction( double v1, double v2, double t1, double t2 );
private:
  MbLineFunction( const MbLineFunction & );
public :
  virtual ~MbLineFunction();
public:
  void    Init( double v1, double v2, double t1, double t2 ); ///< \ru Инициализация по значениям и параметрам. \en Initialization by values and parameters. 
public:
  // \ru Общие функции математического объекта \en Common functions of mathematical object 
  MbeFunctionType  IsA() const override;                  // \ru Тип элемента \en A type of element
  MbFunction & Duplicate() const override;                // \ru Сделать копию элемента \en Create a copy of the element 
  bool    IsSame   ( const MbFunction & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Являются ли объекты равными \en Determine whether objects are equal 
  bool    SetEqual ( const MbFunction & ) override;       // \ru Сделать равным \en Make equal 
  void    GetProperties( MbProperties & ) override;       // \ru Выдать свойства объекта \en Get properties of the object 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 

  double  GetTMax()    const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter
  double  GetTMin()    const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter
  bool    IsClosed()   const override; // \ru Замкнутость кривой \en A curve closedness
  void    SetClosed( bool cl ) override; // \ru Замкнутость функции \en A function closedness 

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

private:
  void    operator = ( const MbLineFunction & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbLineFunction )
};

IMPL_PERSISTENT_OPS( MbLineFunction )


#endif // __FUNC_LINE_FUNCTION_H
