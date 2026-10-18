////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Линейная функция.
         \en Linear function. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __FUNC_CONST_FUNCTION_H
#define __FUNC_CONST_FUNCTION_H


#include <function.h>


//------------------------------------------------------------------------------
/** \brief \ru Постоянная функция.
           \en Constant function. \~
  \details \ru Постоянная функция. \n
           \en Constant function. \n \~
  \ingroup Functions
*/
// ---
class MATH_CLASS MbConstFunction : public MbFunction {
public :
  double value; ///< \ru Значение функции. \en The value of function. 
  double tmin;   ///< \ru Начало области определения. \en Beginning of domain. 
  double tmax;   ///< \ru Конец области определения. \en Ending of domain. 

public :
  MbConstFunction( double v, double t1 = 0.0, double t2 = 1.0 ); ///< \ru Конструктор по значению. \en Constructor by the value. 
private:
  MbConstFunction( const MbConstFunction & );
public :
  virtual ~MbConstFunction();
public:
  void    Init   ( double v, double t1 = 0.0, double t2 = 1.0 ); ///< \ru Инициализация по значению. \en Initialization by the value. 
public:
  // \ru Общие функции математического объекта \en Common functions of mathematical object 
  MbeFunctionType  IsA()        const override;           // \ru Тип элемента \en A type of element
  MbFunction & Duplicate () const override;               // \ru Сделать копию элемента \en Create a copy of the element
  bool    IsSame   ( const MbFunction & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Являются ли объекты равными \en Determine whether objects are equal
  bool    SetEqual ( const MbFunction & ) override;       // \ru Сделать равным \en Make equal
  void    GetProperties( MbProperties & ) override;       // \ru Выдать свойства объекта \en Get properties of the object 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 

  double  GetTMax  ()   const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter
  double  GetTMin  ()   const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter
  bool    IsClosed ()   const override; // \ru Замкнутость кривой \en A curve closedness
  void    SetClosed( bool cl ) override; // \ru Замкнутость функции \en A function closedness

  double  Value    ( double & t ) const override; // \ru Значение функции для t \en The value of function for a given t
  double  FirstDer ( double & t ) const override; // \ru Первая производная по t \en The first derivative with respect to t
  double  SecondDer( double & t ) const override; // \ru Вторая производная по t \en The second derivative with respect to t
  double  ThirdDer ( double & t ) const override; // \ru Третья производная по t \en The third derivative with respect to t

  double _Value    ( double t ) const override; // \ru Значение функции для t \en The value of function for a given t
  double _FirstDer ( double t ) const override; // \ru Первая производная по t \en The first derivative with respect to t
  double _SecondDer( double t ) const override; // \ru Вторая производная по t \en The second derivative with respect to t
  double _ThirdDer ( double t ) const override; // \ru Третья производная по t \en The third derivative with respect to t
  // \ru Вычислить значение и производные. \en Calculate value and derivatives of object for given parameter. \~
  void    Explore( double & t, bool ext,
                   double & val, double & fir, double * sec, double * thr ) const override;

  void    Inverse  ( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction
  double  Step( double t, double sag ) const override;
  double  DeviationStep( double t, double angle ) const override;

  double  MinValue ( double & t ) const override; // \ru Минимальное  значение функции \en The minimum value of function
  double  MaxValue ( double & t ) const override; // \ru Максимальное значение функции \en The maximum value of function
  double  MidValue () const override; // \ru Среднее значение функции \en The middle value of function
  bool    IsGood   () const override; // \ru Корректность функции \en Correctness of function

  bool    IsConst() const override;
  bool    IsLine () const override;

  void    SetOffsetFunc( double off, double scale ) override; // \ru Сместить функцию \en Shift a function
  bool    SetLimitParam( double newTMin, double newTMax ) override; // \ru Установить область изменения параметра \en Set range of parameter
  void    SetLimitValue( size_t n, double newValue ) override; // \ru Установить значение на конце ( 1 - в начале, 2 - в конце) \en Set the value at the end (1 - at start point, 2 - at end point)
  double  GetLimitValue( size_t n ) const override; // \ru Дать значение на конце ( 1 - в начале, 2 - в конце) \en Get the value at the end (1 - at start point, 2 - at end point)

  // \ru Создать функцию из части функции между параметрами t1 и t2 c выбором направления sense. \en Create a function in part of the function between the parameters t1 and t2 choosing the direction. 
  MbFunction * Trimmed( double t1, double t2, int sense ) const override;
  // \ru Разбить функцию точкой с параметром t и вернуть отрезанную часть. \en Function break by the parameter t, and cut off part of the function: begs == true - save the initial half, beg == false - save the final half.
  MbFunction * BreakFunction( double t, bool beg ) override;

private:
  void    operator = ( const MbConstFunction & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbConstFunction )
};

IMPL_PERSISTENT_OPS( MbConstFunction )


#endif // __FUNC_CONST_FUNCTION_H
