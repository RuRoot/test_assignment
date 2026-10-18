////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Служебная функция. 
         \en Service function. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __FUNC_SERVE_FUNCTION_H
#define __FUNC_SERVE_FUNCTION_H


#include <function.h>


//------------------------------------------------------------------------------
/** \brief \ru Монотонно возрастающая служебная функция v(t) = a * t * t + b * t + c. 
           \en The monotonically increasing service function v(t) = a * t * t + b * t + c. \~
  \details \ru Функция служит для изменения параметризации кривых, обеспечивающей непрерывномть производных составных кривых. \n
           \en The function is used to change the parameterization of curves, which provides a continuous flow of derivatives of composite curves. \n \~
  \ingroup Functions
*/
// ---
class MATH_CLASS MbServeFunction : public MbFunction {
public :
  double a;    ///< \ru Коэффициент при квадратичном члене репараметризующего многочлена. \en The coefficient of the quadratic term of the reparametrizing polynomial.
  double b;    ///< \ru Коэффициент при линейном члене репараметризующего многочлена. \en The coefficient of the linear term of the reparametrizing polynomial.
  double c;    ///< \ru Свободный коэффициент репараметризующего многочлена. \en The free coefficient of the reparametrizing polynomial.
  double tmin; ///< \ru Начальный параметр. \en Start parameter. 
  double tmax; ///< \ru Конечный  параметр. \en End parameter.

public :
  ///< \ru Конструктор по умолчанию. \en Default constructor.
                  MbServeFunction();
private:
  ///< \ru Конструктор по параметрам. \en Constructor by parameters. 
                  MbServeFunction( double ka, double kb, double kc, double t1, double t2 );
  ///< \ru Конструктор копировния. \en Copy constructor. 
                  MbServeFunction( const MbServeFunction & );
public :
  virtual         ~MbServeFunction();
public:
  /** \brief \ru Инициализация переменных для линейной репараметризации.
             \en Initialization of variables for linear reparameterization. \~
    \param[in]  basisTMin, basisTMin - \ru Область определения базовой кривой.
                                       \en Parametric region of the base curve.. \~
    \param[in]  t1, t2 - \ru Область определения репараметризованной кривой
                         \en Parametric region of the reparameterized curve. \~
  */
  void    InitLinear( double basisTMin, double basisTMax, double t1, double t2 );
  
  /** \brief \ru Инициализация переменных для репараметризации с заданной производной в начале.
             \en Initialization of variables for reparameterization with a given derivative at the beginning. \~
    \param[in]  basisTMin, basisTMin - \ru Область определения базовой кривой.
                                       \en Parametric region of the base curve.. \~
    \param[in]  t1, t2 - \ru Область определения репараметризованной кривой
                         \en Parametric region of the reparameterized curve. \~
    \param[in]  begDer - \ru Производная параметра базовой кривой в начале кривой.
                         \en The derivative of the base curve parameter at the beginning of the curve. \~
    \return - \ru true - если репараметризация выполнена успешно, 
                  false - если репараметризация оказалась вырожденной и была сведена к линейной.
              \en true - if reparameterization is successful,
                  false - if the reparametrization is degenerate and reduced to linear. \~
  */
  bool    InitQuadratic( double basisTMin, double basisTMax, double t1, double t2, double begDer );

  /** \brief \ru Репараметризация, обеспечивающая на концах новой кривой указаные производные параметра.
             \en Reparametrization providing the indicated derivatives of the parameter at the ends of the new curve. \~
    \details \ru Параметрическая ширина будет подобрана автоматически, исходя из значений производных.
             \en The parametric width will be automatically selected based on the values of the derivatives. \~
    \param[in]  basisTMin, basisTMin - \ru Область определения базовой кривой.
                                       \en Parametric region of the base curve.. \~
    \param[in]  dt1, dt2 - \ru Производные параметра базовой кривой в начале и в конце кривой.
                           \en Derivatives of the base curve parameter at the beginning and end of the curve. \~
    \return - \ru true - репараметризация выполнена успешно, 
                  false - репараметризация оказалась вырожденной и была сведена к линейной.
              \en true - reparameterization is successful,
                  false - reparametrization is degenerate and reduced to linear. \~
  */
  bool    InitScaledEnds( double basisTMin, double basisTMax, double dt1, double dt2);
public:
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
  void    operator = ( const MbServeFunction & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbServeFunction )
};

IMPL_PERSISTENT_OPS( MbServeFunction )


#endif // __FUNC_SERVE_FUNCTION_H
