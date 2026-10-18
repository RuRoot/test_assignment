////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru NURBS функция.
         \en NURBS function. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __FUNC_NURBS_FUNCTION_H
#define __FUNC_NURBS_FUNCTION_H


#include <function.h>
#include <templ_s_array.h>
#include <mb_nurbs_function.h>
#include <tool_multithreading.h>

//------------------------------------------------------------------------------
/** \brief \ru NURBS функция.
           \en NURBS function. \~
  \details \ru NURBS функция определяется контрольными точками pointList,
    весами контрольных точек weights, узловым вектором knots и порядком сплайна degree. \n
           \en NURBS function is defined by 'pointList' control points, 
    weights of control points ('weights'), knot vector ('knots') and order of spline ('degree').\n. \n \~
  \ingroup Functions
*/
// ---
class MATH_CLASS MbNurbsFunction : public MbFunction {

protected:
  size_t         degree;        ///< \ru Порядок В-сплайна. \en Order of B-spline. 
  SArray<double> pointList;     ///< \ru Множество контрольных точек. \en Set of control points. 
  SArray<double> weights;       ///< \ru Множество весов контрольных точек. \en Set of weights of the control points. 
  SArray<double> knots;         ///< \ru Узловой вектор. \en Knot vector. 
  ptrdiff_t      uppIndex;      ///< \ru Количество участков кривой (равно количество контрольных точек минус единица). \en Count of curve pieces (is equal to count of control points minus one). 
  ptrdiff_t      uppKnotsIndex; ///< \ru Последний индекс узлового вектора. \en Last index of knot vector. 
  bool           closed;        ///< \ru Признак замкнутости кривой. \en An attribute of curve closedness. 


  //------------------------------------------------------------------------------
/** \brief \ru Вспомогательные данные.
           \en Auxiliary data. \~
  \details \ru Вспомогательные данные служат для ускорения работы объекта.
           \en Auxiliary data are used for fast calculations. \n \~
*/
// ---
  class MbNurbsFunctionAuxiliaryData : public AuxiliaryData {
  public:
    Array2<double>  values;
    SArray<double>  pointsM;
    SArray<double>  weightM;

  public:
    MbNurbsFunctionAuxiliaryData() : values(), pointsM(), weightM() {}
    MbNurbsFunctionAuxiliaryData( const MbNurbsFunctionAuxiliaryData & ) : AuxiliaryData {}, values(), pointsM(), weightM() {}
    virtual ~MbNurbsFunctionAuxiliaryData() {}
  };

  mutable CacheManager<MbNurbsFunctionAuxiliaryData> cache;


public :
  /// \ru Конструктор по точкам и признаку замкнутости. \en Constructor by points and an attribute of closedness. 
  MbNurbsFunction( size_t d, const SArray<double> & values, bool cls );
  /// \ru Конструктор по точкам, параметрам и признаку замкнутости. \en Constructor by points, parameters and an attribute of closedness. 
  MbNurbsFunction( size_t d, const SArray<double> & values, const SArray<double> & knots, bool cls );
private:
  /// \ru Конструктор. \en Constructor. 
  MbNurbsFunction();
  MbNurbsFunction( const MbNurbsFunction & );
public :
  virtual ~MbNurbsFunction();

public:

  /** \brief \ru Создать сплайн.
             \en Create spline. \~
      \details \ru Сплайн, проходящий через заданные точки при заданных параметрах.\n
        В случае замкнутости нужно передавать массив с совпадением первой и последней точек.\n
               \en Spline passing through given points at given parameters.\n
        In case of closedness it is necessary to pass the array with coincidence of the first and the last points.\n \~
      \param[in] degree  - \ru Порядок сплайна.
                           \en A spline order. \~
      \param[in] cls     - \ru Признак замкнутости.
                           \en A closedness attribute. \~
      \param[in] points  - \ru Набор точек, через которые проходит сплайн.
                           \en Set of points which the spline passes through. \~
      \param[in] params  - \ru Последовательность узловых параметров.
                           \en Sequence of knot parameters. \~
      \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
              \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static  MbNurbsFunction * CreateThrough( size_t deg, bool cls, const SArray<double> & points,
                                           const SArray<double> & params, const SArray<double> * aKnots = nullptr );
  /// \ru Инициализация сплайна, проходящего через заданные точки при заданных параметрах. В случае замкнутости нужно передавать массив с совпадением первой и последней точек.\n \~
  /// \en Initialization of spline passing through given points at given parameters. In case of closedness it is necessary to pass the array with coincidence of the first and the last points.\n \~
  bool    InitThrough( size_t deg, bool cls, const SArray<double> & points, const SArray<double> & params, const SArray<double> * aKnots = nullptr );

private:
  /// \ru Инициализация по точкам и признаку замкнутости. \en Initialization by values and an attribute of closedness. 
  template <class DoubleVector>
  bool    Init( size_t initDegree, const DoubleVector & initPoints, bool initClosed )
  {
    if ( ::IsValidNurbsParams( initDegree, initClosed, initPoints.size() ) ) {
      degree    = initDegree;
      closed    = initClosed;
      uppIndex  = (ptrdiff_t)initPoints.size() - 1;
      pointList.assign( initPoints.begin(), initPoints.end() );
      weights.assign( initPoints.size(), 1.0 );
      uppKnotsIndex = ::DefineKnotsVector( degree, closed, uppIndex, knots );
      return true;
    }
    return false;
  }

  /// \ru Инициализация по точкам, параметрам и признаку замкнутости. \en Initialization by values, knots and an attribute of closedness. 
  template <class DoubleVector>
  bool    Init( size_t initDegree, const DoubleVector & initPoints, const DoubleVector & initKnots, bool initClosed )
  {
    if ( ::IsValidNurbsParams( initDegree, initClosed, initPoints.size(), initPoints.size(), initKnots.size() ) ) {
      degree = initDegree;
      closed = initClosed;
      pointList = initPoints;
      weights.assign( initPoints.size(), 1.0 );
      knots = initKnots;
      uppIndex = (ptrdiff_t)pointList.size() - 1;
      uppKnotsIndex = (ptrdiff_t)knots.size() - 1;
      SetClamped();
      return true;
    }
    return false;
  }

public:

  // \ru Общие функции математического объекта \en Common functions of mathematical object 
  MbeFunctionType  IsA  () const override; // \ru Тип элемента \en A type of element
  MbFunction & Duplicate() const override; // \ru Сделать копию элемента \en Create a copy of the element 
  bool    IsSame   ( const MbFunction & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Являются ли объекты равными \en Determine whether objects are equal 
  bool    SetEqual ( const MbFunction & ) override; // \ru Сделать равным \en Make equal 
  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 

  double  GetTMax  () const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter 
  double  GetTMin  () const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter 
  bool    IsClosed () const override; // \ru Замкнутость кривой \en A curve closedness 
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
  /// \ru Сместить функцию. \en Shift a function. 
  void    SetOffsetFunc( double off, double scale ) override; 

  bool    SetLimitParam( double newTMin, double newTMax ) override; // \ru Установить область изменения параметра \en Set range of parameter 
  void    SetLimitValue( size_t n, double newValue ) override; // \ru Установить значение на конце ( 1 - в начале, 2 - в конце) \en Set the value at the end (1 - at start point, 2 - at end point) 
  double  GetLimitValue( size_t n ) const override; // \ru Дать значение на конце ( 1 - в начале, 2 - в конце) \en Get the value at the end (1 - at start point, 2 - at end point) 
  void    SetLimitDerive( size_t n, double newValue, double dt ) override; // \ru Установить значение на конце ( 1 - в начале, 2 - в конце) \en Set the value at the end (1 - at start point, 2 - at end point)
  double  GetLimitDerive( size_t n ) const override; // \ru Дать значение на конце ( 1 - в начале, 2 - в конце) \en Get the value at the end (1 - at start point, 2 - at end point)
  bool    InsertValue( double t, double newValue ) override; // \ru Установить значение для параметра t. \en Set the value for the pdrdmeter t. 

  // \ru Создать функцию из части функции между параметрами t1 и t2 c выбором направления sense. \en Create a function in part of the function between the parameters t1 and t2 choosing the direction. 
  MbFunction * Trimmed( double t1, double t2, int sense ) const override; 
  // \ru Разбить функцию точкой с параметром t и вернуть отрезанную часть. \en Function break by the parameter t, and cut off part of the function: begs == true - save the initial half, beg == false - save the final half.
  MbFunction * BreakFunction( double t, bool beg ) override;
  MbFunction * Break( double t1, double t2 ) const; ///< \ru Выделить часть функции. \en Select a part of a function. 

  /// \ru Добавление нового узла; возвращает количество узлов, которые удалось вставить. \en Addition of a new knots; returns the number of knots which have been inserted. 
  size_t  InsertKnots( double & newKnot, size_t multiplicity, double relEps );
  /// \ru Удалить кратный внутренний узел id, num раз; вернуть количество удалений, которое удалось сделать. \en Remove multiple internal 'id' knot 'num' times, return count of removals was successfully made. 
  ptrdiff_t RemoveKnot( ptrdiff_t id, ptrdiff_t num, double relEps = Math::paramEpsilon, double absEps = Math::lengthEpsilon );
  /// \ru Преобразовать узловой вектор в зажатый (если кривая замкнута и clm = false) или разжатый (если кривая не замкнута и clm = true). \en Transform knot vector to clamped (if curve is closed and clm = false) or unclamped (if curve is open and clm = true). 
  bool    UnClamped( bool clm );
  void    SetClamped(); // \ru Делаем зажатый узловой вектор. \en Set clamped knots vector. 

  size_t  GetValuesCount() const; // \ru Выдать количество опорных точек \en Get the number of control points 
  double  GetValue( size_t index ) const; // \ru Дать значение точки по номеру \en Get the value of point by its number  
  bool    SetValue( size_t index, double v ); // \ru Установить значение точки по номеру \en Set the value of point by its number  
  void    GetValueList( SArray<double> & vals ) const; ///< \ru Вернуть массив контрольных значений. \en Get array of control values.

  /// \ru Получить размер весового вектора. \en Get a size of weights vector. 
  size_t  GetWeightsCount() const { return weights.size(); }
  /// \ru Получить весовой вектор. \en Get a weights vector. 
  template <class WeightsVector>
  void    GetWeights( WeightsVector & wts, bool justSet = true ) const { if ( justSet ) { wts.clear(); }; std::copy( weights.begin(), weights.end(), std::back_inserter( wts ) ); }
  /// \ru Получить значение элемента весового вектора по индексу. \en Get a weights vector element value by index. 
  double  GetWeight( size_t ind ) const { return weights[ind]; }
  /// \ru Получить значение элемента весового вектора по индексу. \en Get a weights vector element value by index. 
  double & SetWeight( size_t ind ) { return weights[ind]; }

  /// \ru Получить размер узлового вектора. \en Get a size of knots vector. 
  size_t  GetKnotsCount() const { return knots.size(); }
  /// \ru Получить узловой вектор. \en Get a knots vector. 
  template <class KnotsVector>
  void    GetKnots( KnotsVector & kts, bool justSet = true ) const { if ( justSet ) { kts.clear();  }; std::copy( knots.begin(), knots.end(), std::back_inserter( kts ) ); }
  /// \ru Получить значение элемента узлового вектора по индексу. \en Get a knots vector element value by index. 
  double  GetKnot( size_t ind ) const { return knots[ind]; }
  /// \ru Получить значение элемента узлового вектора по индексу. \en Get a knots vector element value by index. 
  double & SetKnot( size_t ind ) { return knots[ind]; }
  /// \ru Вернуть максимальный индекс узлового вектора. \en Get the maximal index of knots vector.  
  ptrdiff_t  GetUppKnotsIndex() const { return uppKnotsIndex; }

  bool    CheckParam( double & t ) const; // \ru Проверить параметр. \en Parameter check.

private:

  // \ru Вычисление функции и веса. \en Calculating the function and weight.
  bool    CalculateValue( double & t, size_t deriveN, Array2<double> & values, 
                          ptrdiff_t & left, double & valw, double & weig ) const;
  // \ru Вычисление производной функции и производной веса. \en Calculating the function and weight derivatives.
  void    CalculateFirst( const Array2<double> & values, const ptrdiff_t & left, 
                          SArray<double> & pointsM, SArray<double> & weightM,
                          double & firw, double & weig_ ) const;
  void    CalculateDerive( const Array2<double> & values, const ptrdiff_t & left, size_t deriveN,
                           SArray<double> & pointsM, SArray<double> & weightM,
                           double & derw, double & weig_ ) const;


  void    operator = ( const MbNurbsFunction & );   // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbNurbsFunction )
};

IMPL_PERSISTENT_OPS( MbNurbsFunction )


#endif // __FUNC_NURBS_FUNCTION_H
