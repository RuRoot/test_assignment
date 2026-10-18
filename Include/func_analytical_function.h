////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Символьные (пользовательские) функции.
         \en Symbolic (user) functions. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __FUNC_ANLYTICAL_FUNCTION_H
#define __FUNC_ANLYTICAL_FUNCTION_H


#include <templ_sptr.h>
#include <function.h>
#include <mb_enum.h>
#include <tool_cstring.h>
#include <tool_multithreading.h>


class MbMathematicalNode; // \ru Математический узел \en The mathematical node 
class MbListVars;         // \ru Список переменных \en The list of variables 
class MbUserFunc;         // \ru Пользовательская функция \en The user function 
class DefRange;           // \ru Область определения функции \en The function domain


//------------------------------------------------------------------------------
/** \brief \ru Скалярная функция, заданная символьной строкой.
           \en The symbolic function. \~
  \details \ru Скалярная функция, заданная символьной строкой. \n
           \en The symbolic function. \n \~
  \ingroup Functions
*/
// ---
class MATH_CLASS MdCharacterFunction : public MbFunction, public MbSyncItem {
private :
  const MbMathematicalNode * expression; ///< \ru Выражение функции в виде дерева. \en An expression of a function as tree. 
  const MbListVars *         variables;  ///< \ru Аргументы функции в виде списка. \en Arguments of a function as list 
        c3d::string_t        data;       ///< \ru Выражение функции в виде строки. \en An expression of a function as string. 
        c3d::string_t        argument;   ///< \ru Аргументы функции в виде строки. \en Arguments of a function as string. 
        double               tmin;       ///< \ru Начальный параметр. \en Start parameter. 
        double               tmax;       ///< \ru Конечный параметр. \en End parameter. 
        bool                 sense;      ///< \ru Направление. \en Direction. 

public :
  /// \ru Конструктор. \en Constructor. 
  MdCharacterFunction( const MbMathematicalNode & expression, 
                       const MbListVars &         vars, 
                       const c3d::string_t &      data, 
                       const c3d::string_t &      argument,
                     double               tmin, 
                     double               tmax, 
                     bool                 sense );
  virtual ~MdCharacterFunction();

private:
  MdCharacterFunction( const MdCharacterFunction & ); 

public :
  // \ru Общие функции математического объекта \en Common functions of mathematical object 
  MbeFunctionType IsA() const override;                   // \ru Тип элемента \en A type of element
  MbFunction & Duplicate() const override;                // \ru Сделать копию элемента \en Create a copy of the element 
  bool    IsSame   ( const MbFunction & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Являются ли объекты равными \en Determine whether objects are equal 
  bool    SetEqual ( const MbFunction & ) override;       // \ru Сделать равным \en Make equal 
  void    GetProperties( MbProperties & ) override;       // \ru Выдать свойства объекта \en Get properties of object 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of object 

  double  GetTMin      () const override;    // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter
  double  GetTMax      () const override;    // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter
  bool    IsClosed     () const override;    // \ru Замкнутость кривой \en A curve closeness
  void    SetClosed    ( bool cl ) override; // \ru Замкнутость функции \en A function closeness

  double  Value        ( double & t ) const override; // \ru Значение функции для t \en The value of function for a given t
  double  FirstDer     ( double & t ) const override; // \ru Первая производная по t \en The first derivative with respect to t
  double  SecondDer    ( double & t ) const override; // \ru Вторая производная по t \en The second derivative with respect to t
  double  ThirdDer     ( double & t ) const override; // \ru Третья производная по t \en The third derivative with respect to t
  // \ru Вычислить значение и производные. \en Calculate value and derivatives of object for given parameter. \~
  void    Explore( double & t, bool ext,
                   double & val, double & fir, double * sec, double * thr ) const override;
                                
  void    Inverse      ( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction
  double  Step         ( double t, double sag )    const override;
  double  DeviationStep( double t, double angle )  const override;

  double  MinValue     ( double & t ) const override; // \ru Минимальное  значение функции \en The minimum value of function
  double  MaxValue     ( double & t ) const override; // \ru Максимальное значение функции \en The maximum value of function
  double  MidValue     () const override; // \ru Среднее значение функции \en The middle value of function
  bool    IsGood       () const override; // \ru Корректность функции \en Correctness of function
  bool    IsConst      () const override;
  bool    IsLine       () const override;

  void    SetOffsetFunc( double off, double scale ) override; // \ru Сместить функцию \en Shift a function 
  bool    SetLimitParam( double newTMin, double newTMax ) override; // \ru Установить область изменения параметра \en Set range of parameter 
  void    SetLimitValue( size_t n, double newValue ) override;      // \ru Установить значение на конце ( 1 - в начале, 2 - в конце) \en Set the value at the end (1 - at start point, 2 - at end point) 
  double  GetLimitValue( size_t n ) const override;                 // \ru Дать значение на конце ( 1 - в начале, 2 - в конце) \en Get the value at the end (1 - at start point, 2 - at end point) 

  // \ru Создать функцию из части функции между параметрами t1 и t2 c выбором направления sense. \en Create a function in part of the function between the parameters t1 and t2 choosing the direction. 
  MbFunction * Trimmed( double t1, double t2, int sense ) const override; 
  // \ru Разбить функцию точкой с параметром t и вернуть отрезанную часть. \en Function break by the parameter t, and cut off part of the function: begs == true - save the initial half, beg == false - save the final half.
  MbFunction * BreakFunction( double t, bool beg ) override;

private:  
  void    Translate  ();
  void    CheckParam ( double & t ) const;
private:
  void    operator = ( const MdCharacterFunction & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MdCharacterFunction )
};

IMPL_PERSISTENT_OPS( MdCharacterFunction )


//------------------------------------------------------------------------------
/** \brief \ru Скалярная функция, заданная аналитическим выражением.
           \en The analytical function. \~
  \details \ru Скалярная функция, заданная аналитическим выражением. \n
           \en The analytical function. \n \~
  \ingroup Functions
*/
// ---
class MATH_CLASS MdAnalyticalFunction : public MbFunction
{
private:
  MbUserFunc *   expression; // \ru Пользовательская функция. \en The user function. 
  double         tmin;       // \ru Начальный параметр. \en Start parameter. 
  double         tmax;       // \ru Конечный параметр. \en End parameter. 
  bool           sense;      // \ru Направление. \en Direction. 
private:
  struct DerivateData : public AuxiliaryData {
    double tCur;                      // \ru Параметр для которого рассчитаны производные \en. The parameter where the derivatives have been calculated. 
    double derivatives[cdt_CountDer]; // \ru Рассчитанные значения производных. \en The calculated values of derivatives. 

    DerivateData();
    DerivateData( const DerivateData & data );
  };
  mutable CacheManager<DerivateData> cache;

public :
  // \ru Конструктор. \en Constructor. 
  MdAnalyticalFunction( const MbUserFunc & ufunc, bool sameFunc, double tmin, double tmax, bool sense );
public:
  virtual ~MdAnalyticalFunction();

private:
  MdAnalyticalFunction( const MdAnalyticalFunction & ); 

public :
  // \ru Общие функции математического объекта \en Common functions of mathematical object 
  MbeFunctionType IsA() const override;                   // \ru Тип элемента \en A type of element
  MbFunction & Duplicate() const override;                // \ru Сделать копию элемента \en Create a copy of the element 
  bool    IsSame   ( const MbFunction & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Являются ли объекты равными \en Determine whether objects are equal 
  bool    SetEqual ( const MbFunction & ) override;       // \ru Сделать равным \en Make equal 
  void    GetProperties( MbProperties & ) override;       // \ru Выдать свойства объекта \en Get properties of the object 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 

  double  GetTMin       () const override;    // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter
  double  GetTMax       () const override;    // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter
  bool    IsClosed      () const override;    // \ru Замкнутость кривой \en A curve closeness
  void    SetClosed     ( bool cl ) override; // \ru Замкнутость функции \en A function closeness

  double  Value         ( double & t ) const override; // \ru Значение функции для t \en The value of function for a given t
  double  FirstDer      ( double & t ) const override; // \ru Первая производная по t \en The first derivative with respect to t
  double  SecondDer     ( double & t ) const override; // \ru Вторая производная по t \en The second derivative with respect to t
  double  ThirdDer      ( double & t ) const override; // \ru Третья производная по t \en The third derivative with respect to t
  // \ru Вычислить значение и производные. \en Calculate value and derivatives of object for given parameter. \~
  void    Explore       ( double & t, bool ext,
                          double & val, double & fir, double * sec, double * thr ) const override;

  void    Inverse       ( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction
  double  Step          ( double t, double sag )    const override;
  double  DeviationStep ( double t, double angle )  const override;

  double  MinValue      ( double & t ) const override; // \ru Минимальное  значение функции \en The minimum value of function
  double  MaxValue      ( double & t ) const override; // \ru Максимальное значение функции \en The maximum value of function
  double  MidValue      () const override; // \ru Среднее значение функции \en The middle value of function
  bool    IsGood        () const override; // \ru Корректность функции \en Correctness of function
  bool    IsConst       () const override;
  bool    IsLine        () const override;

  void    SetOffsetFunc ( double off, double scale) override; // \ru Сместить функцию \en Shift a function
  bool    SetLimitParam ( double newTMin, double newTMax ) override; // \ru Установить область изменения параметра \en Set range of parameter
  void    SetLimitValue ( size_t n, double newValue ) override;      // \ru Установить значение на конце ( 1 - в начале, 2 - в конце) \en Set the value at the end (1 - at start point, 2 - at end point)
  double  GetLimitValue ( size_t n ) const override;                 // \ru Дать значение на конце ( 1 - в начале, 2 - в конце) \en Get the value at the end (1 - at start point, 2 - at end point)

  virtual void    GetCharacteristicParams( std::vector<double> & tVec, double t1, double t2 ) const; // \ru Дать параметры особого поведения(для cos это Pi*n) \en Get parameters of special behavior (for cos it is Pi*n)  

  // \ru Создать функцию из части функции между параметрами t1 и t2 c выбором направления sense. \en Create a function in part of the function between the parameters t1 and t2 choosing the direction. 
  MbFunction * Trimmed( double t1, double t2, int sense ) const override; 
  // \ru Разбить функцию точкой с параметром t и вернуть отрезанную часть. \en Function break by the parameter t, and cut off part of the function: begs == true - save the initial half, beg == false - save the final half.
  MbFunction * BreakFunction( double t, bool beg ) override;

  virtual bool IsCos ( double & a, double & b ) const;  ///< \ru Имеет ли функция вид a * cos() + b. \en Function looks like a * cos() + b.

  /** \brief \ru Получить область определения функции. \en Get function domain. \~
    \details \ru Получить область определения функции для параметра с индексом ind. Метод вырезает из заданной области определения участки, где функция не определена. \n
             \en Get the function domain for the parameter with index ind. The method cuts from the initial domain some parts where the function is not defined. \n \~
    \param[in, out] dr     - \ru Инициализируется некоторым начальным диапазоном. На выходе - пересечение этого диапазона с реальной областью определения функции.
                             \en It is initialized by some start range. The output value will be the intersection between this range and the true function domain. \~
    \param[in] ind         - \ru Индекс переменной, для которой ищется область определения.
                             \en An index of the variable, which the domain is searched for. \~
    \param[in] stopOnBreak - \ru Остановить поиск если найден разрыв.
                             \en Searching will be stopped if the first break is found. \~
    \return \ru Возвращает false если область определения найти не удалось.
            \en Returns false if domain cannot be found. \~
  */
  bool GetDefRange( DefRange & dr, size_t ind, bool stopOnBreak ) const;

private: 
  /// \ru Производные по параметру: параметр, значение, первая, вторая и третья производные \en Derivatives with respect to the parameter: parameter, value, first, second and third derivatives 
  void    Derivates ( double & t, DerivateData * data ) const;
  void    CheckParam( double & t ) const;
  void    ResetTCalc() const; // \ru Сбросить временные данные \en Reset temporary data 
  void    operator = ( const MdAnalyticalFunction & ); // \ru Не реализовано. \en NOT ALLOWED !!! 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MdAnalyticalFunction )
};

IMPL_PERSISTENT_OPS( MdAnalyticalFunction )


#endif // __FUNC_ANLYTICAL_FUNCTION_H
