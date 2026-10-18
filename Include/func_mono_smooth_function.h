////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Монотонная сплайн интерполяция класса c2 на основе 
               однопараметрических групп диффеоморфизмов (Н.В.Осадченко).
         \en Monotone spline interpolation class c2 based
               one-parameter groups of diffeomorphisms (N.V. Osadchenko). \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __FUNC_MONO_SMOOTH_FUNCTION_H
#define __FUNC_MONO_SMOOTH_FUNCTION_H


#include <function.h>
#include <templ_s_array.h>


//------------------------------------------------------------------------------
/** \brief \ru Монотонная сплайн интерполяция класса c2 на основе 
               однопараметрических групп диффеоморфизмов (Н.В.Осадченко).
           \en Monotone spline interpolation class c2 based
               one-parameter groups of diffeomorphisms (N.V. Osadchenko). \~
  \details \ru Класс описывает сплайн-функцию, которая строится по возрастающему \n
               набору параметров и монотонно возрастающему/убывающему набору значений. \n
               Функция гарантирует однозначное соответствие параметр-значение. При этом \n
               аналичиские функции, описывающие сплайн на каждом участке, стыкуются  \n
               между собой с сохранением непрерывности первой и второй производной.  \n
               (Если от функции не требуется второй порядок непрерывности, то монотонную функцию  \n
               можно построить с помощью кубической функции MbCubicFunction через инициализацию  \n
               InitMonotonic). Данная функция может быть использована, как функция репараметризации. \n
               При этом, если репараметризуемый объект обладает свойством замкнутости, то \n
               функция может учесть это свойство, обеспечив также второй порядок непрерывности \n
               через шов. Функция принимает на вход при инициализации массив параметров и \n
               значений, имеющих одинаковую размерность вне зависимости от параметра \n
               замкнутости. Репараметризующая функция построена в виде 3х вложенных друг в друга \n
               рациональных функций y(x)=y1+(y2-y1)*Fb(Fg(Fb((x-x1)/(x2-x1)))). Подробно ознакомится \n
               с данным типом сплайна можно по ссылке.
               http://www.stfi.ru/journal/STFI_2017_03/STFI_2017_03_Osadchenko.pdf
           \en The class describes a spline function, which is built from an increasing set  \n
               of parameters and a monotonically increasing / decreasing set of values. The \n
               function guarantees a one-to-one parameter-value match. In this case, the  \n
               analytical functions describing the spline in each section are joined together \n
               while maintaining the continuity of the first and second derivatives. (If the \n
               function does not require the second order of continuity, then a monotonic  \n
               function can be built using the MbCubicFunction cubic function through the  \n
               InitMonotonic initialization). This function can be used as a reparameterization \n
               function. Moreover, if the reparameterizable object has the property of being  \n
               closed, then the function can take this property into account, providing also \n
               the second order of continuity through the seam. During initialization, the  \n
               function accepts an array of parameters and values ​​that have the same dimension, \n
               regardless of the closure parameter. The reparametrizing function is built in \n
               the form of 3 nested rational functions  \n
               y (x) = y1 + (y2-y1) * Fb (Fg (Fb ((x-x1) / (x2-x1)))).  \n
               You can learn more about this type of spline here. \n
           http://www.stfi.ru/journal/STFI_2017_03/STFI_2017_03_Osadchenko.pdf \~
  \ingroup Functions
*/
// ---
class MATH_CLASS MbMonoSmoothFunction : public MbFunction {
protected:
  c3d::DoubleVector x;   ///< \ru Возрастающий набор параметров. \en Increasing set of parameters.
  c3d::DoubleVector y;   ///< \ru Монотонный набор значений. \en Monotonic set of values.
  c3d::DoubleVector bet; ///< \ru Параметры beta-функций. \en Parameters of beta functions.
  c3d::DoubleVector gam; ///< \ru Параметры gamma-функций. \en Parameters of gamma functions.
  bool              cls; ///< \ru Периодичность набора значений.\en Periodicity of the set of values.
  double            tBegin; ///< \ru Начальный параметр.\en Begin parameter.
  double            tEnd;   ///< \ru Конечный параметр.\en End parameter.

private:
  /// \ru Конструктор. \en Constructor. 
  MbMonoSmoothFunction();
  /// \ru Конструктор копии. \en Copy constructor. 
  MbMonoSmoothFunction( const MbMonoSmoothFunction & );
public :
  virtual ~MbMonoSmoothFunction();

public:
    /// \ru Создание объекта. \en Object creation.
    static MbFunction * Create( const c3d::DoubleVector & pars, const c3d::DoubleVector & vals, bool yCls );
  /** \brief \ru Инициализация сплайна.
             \en Spline initialization. \~
      \param[in] pars - \ru Возрастающий набор параметров. 
                        \en Increasing set of parameters. \~
      \param[in] vals - \ru Монотонный набор значений. 
                        \en Monotonic set of values. \~
      \param[in] periodic - \ru Периодичность набора значений.
                          \en Periodicity of the set of values. \~
      \return \ru Статус операции.
              \en Operation status. \~
  */
  bool    Init ( const c3d::DoubleVector & pars, const c3d::DoubleVector & vals, bool periodic );
public:
  // \ru Общие функции математического объекта \en Common functions of mathematical object 
  MbeFunctionType  IsA      () const override;            // \ru Тип элемента \en A type of element 
  MbFunction & Duplicate() const override;                // \ru Сделать копию элемента \en Create a copy of the element 
  bool    IsSame   ( const MbFunction & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Являются ли объекты равными \en Determine whether objects are equal 
  bool    SetEqual ( const MbFunction & ) override;       // \ru Сделать равным \en Make equal 
  void    GetProperties( MbProperties & ) override;       // \ru Выдать свойства объекта \en Get properties of object 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of object 

  double  GetTMax  () const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter 
  double  GetTMin  () const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter 
  bool    IsClosed () const override; // \ru Замкнутость кривой \en A curve closeness 
  void    SetClosed( bool ) override; // \ru Замкнутость функции \en A function closeness

  double  Value     ( double & t ) const override; // \ru Значение функции для t \en The value of function for a given t 
  double  FirstDer  ( double & t ) const override; // \ru Первая производная по t \en The first derivative with respect to t 
  double  SecondDer ( double & t ) const override; // \ru Вторая производная по t \en The second derivative with respect to t 
  double  ThirdDer  ( double & t ) const override; // \ru Третья производная по t \en The third derivative with respect to t 

  double _Value     ( double t ) const override; // \ru Значение функции для t \en The value of function for a given t 
  double _FirstDer  ( double t ) const override; // \ru Первая производная по t \en The first derivative with respect to t 
  double _SecondDer ( double t ) const override; // \ru Вторая производная по t \en The second derivative with respect to t 
  double _ThirdDer  ( double t ) const override; // \ru Третья производная по t \en The third derivative with respect to t 
  // \ru Вычислить значение и производные. \en Calculate value and derivatives of object for given parameter. \~
  void    Explore( double & t, bool ext, double & val, double & fir, double * sec, double * thr ) const override;
  // \ru Вычислить аргумент t по значению функции. \en Calculate the argument t by the function value. 
  double  Argument( double & val ) const override;

  size_t  GetListCount() const { return x.size(); }      // \ru Количество точек в наборе \en Number of points in a set.
  double  GetValue(size_t ind ) const { return y[ind]; } // \ru Получить значение по индексу. \en Get value by index.
  double  GetParam(size_t ind ) const { return x[ind]; } // \ru Получить параметр по индексу. \en Get parameter by index.

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
  bool    SetLimitParam( double, double ) override; // \ru Установить область изменения параметра \en Set range of parameter
  void    SetLimitValue( size_t n, double) override; // \ru Установить значение на конце ( 1 - в начале, 2 - в конце) \en Set the value at the end (1 - at start point, 2 - at end point)
  double  GetLimitValue( size_t n ) const override; // \ru Дать значение на конце ( 1 - в начале, 2 - в конце) \en Get the value at the end (1 - at start point, 2 - at end point) 
  bool    InsertValue( double x, double y) override; // \ru Установить значение для параметра t. \en Set the value for the pdrdmeter t. 

  // \ru Создать функцию из части функции между параметрами t1 и t2 c выбором направления sense. \en Create a function in part of the function between the parameters t1 and t2 choosing the direction. 
  MbFunction * Trimmed( double t1, double t2, int sense ) const override; 
  // \ru Разбить функцию точкой с параметром t и вернуть отрезанную часть. \en Function break by the parameter t, and cut off part of the function: begs == true - save the initial half, beg == false - save the final half.
  MbFunction * BreakFunction( double t, bool beg ) override;
  MbFunction * Break( double t1, double t2 ) const; ///< \ru Выделить часть функции. \en Select a part of a function. 

private:
  void    CheckParam( double & t ) const; // \ru Установить параметр в область определения. \en Set the parameter to the domain.
  void    DivExplore( size_t ord, double( &P )[4], double( &T )[4], double( &res )[4] ) const; // \ru Найти производные частного P/T. \en Find the derivatives of the quotient P / T.
  void    ExploreBet( double b, size_t ord, double( &res )[4] ) const; // \ru Расчитать производные beta функции. \en Calculate the derivatives of the beta function.
  void    ExploreGam( double b, size_t ord, double( &res )[4] ) const;// \ru Расчитать производные gamma функции. \en Calculate the derivatives of the gamma function.
  void    Explore( double x, size_t ord, double( &res )[4] ) const;// \ru Расчитать производные сплайна.  \en Calculate the derivatives of the spline.
  void    operator = ( const MbMonoSmoothFunction & );   // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbMonoSmoothFunction )
};  

IMPL_PERSISTENT_OPS( MbCubicSplineFunction )


#endif // __FUNC_MONO_SMOOTH_FUNCTION_H
