////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Составной кубический сплайн Эрмита.
         \en Composite Hermite cubic spline. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_HERMIT3D_H
#define __CUR_HERMIT3D_H


#include <cur_polycurve3d.h>


class    MbHermit;


//------------------------------------------------------------------------------
/** \brief \ru Составной кубический сплайн Эрмита в трёхмерном пространстве.
           \en Composite Hermite cubic spline in three-dimensional space. \~
  \details \ru Составной кубический сплайн Эрмита определяется контрольными точками pointList, первыми производными 
    сплайна vectorList в контрольных точках и значениями параметра сплайна tList в контрольных точках. 
    Для не замкнутой кривой множества pointList, vectorList и tList должны содержать одинаковое количество элементов.
    Для замкнутой кривой количество элементов tList должно быть на единицу больше, чем количество элементов pointList и vectorList. 
    Сплайн Эрмита является оставной кубический кривой.
    На каждом участке между двумя соседними контрольными точками сплайн описывается кубическим полиномом 
    с заданными точками и производными на краях. 
    Сплайн Эрмита проходит через свои контрольные точки при значениях параметра из множества tList и имеет в них заданные производные.
    Кубические полиномы гладко стыкуются в контрольных точках и имеют в них непрерывные первые производные. 
    Если производные в контрольных точках не заданы, то они рассчитываются по данной контрольной точке и двух её соседним точкам.
    Для этого то трём точкам и значениям параметров в них строится парабола и вычисляется производная параболы в средней точке.
    Производные в краевых контрольных точках определяются по двум точкам и условию на краю для второй производной (ноль).
           \en Composite Hermite cubic spline is defined by control points pointList, the first derivatives 
    of spline vectorList in control points and values of the spline parameter in control point. 
    For unclosed curve the sets pointList, vectorList and tList must contain the same number of elements.
    For closed curve the number of elements tList must be one greater than the number of elements of pointList and vectorList. 
    Hermite spline is a composite cubic curve.
    On each region between two neighboring control points the spline is described by the cubic polynomial 
    with given points and derivatives at the edges. 
    Hermite spline passes through its control points for parameter values ??from the set tList and has given derivatives at these points.
    Cubic polynomials are connected smoothly at the control points and they have continuous first derivatives at these points. 
    If the derivatives at the control points are not specified then they are calculated by the given control point and its two neighboring points.
    For this purpose parabola is constructed by three points and the values ??of parameters in these points, and after this the derivative of parabola is calculated at the middle point.
    Derivatives at the boundary control points are defined by two points and the condition on the edge for the second derivative (zero). \~ 
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbHermit3D : public MbPolyCurve3D {
protected :
  SArray<MbVector3D> vectorList;   ///< \ru Множество производных в контрольных точках. \en Set of derivatives at the control points. 
  SArray<double>     tList;        ///< \ru Множество параметров в контрольных точках. \en Set of parameters at the control points. 
  ptrdiff_t          splinesCount; ///< \ru Количество сплайнов. \en The number of splines. 

protected :
  MbHermit3D(); ///< \ru Конструктор по умолчанию. \en Constructor by default.
  MbHermit3D( const MbHermit3D & ); ///< \ru Конструктор копирования. \en Copy constructor.
  MbHermit3D( const SArray<MbCartPoint3D> & initPoints, bool cls );
  MbHermit3D( const SArray<double> & initParams, const SArray<MbCartPoint3D> & initPoints, bool cls );
  MbHermit3D( const SArray<double> & initParams, const SArray<MbCartPoint3D> & initPoints, const SArray<MbVector3D> & initVectors, bool cls );
  MbHermit3D( const SArray<double> & initParams, const SArray<MbCartPoint3D> & initPoints, const SArray<int> & vLabels, bool cls );
  MbHermit3D( const MbHermit &, const MbPlacement3D & );
  MbHermit3D( const MbCartPoint3D & p1, const MbCartPoint3D & p2 ); // \ru Конструктор по двум точкам. \en Constructor by two points. 
  MbHermit3D( double t1, const MbCartPoint3D & p1, const MbVector3D & v1,
              double t2, const MbCartPoint3D & p2, const MbVector3D & v2 ); // \ru Конструктор по двум точкам и производным в этих точках. \en Constructor by two points and derivatives at this points. 

public:
  virtual ~MbHermit3D();

public:
  /** \brief \ru Создать копию сплайна.
             \en Create copy of spline. \~
    \details \ru Создать копию сплайна.\n
             \en Create copy of spline.\n \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbHermit3D * Create( const MbHermit3D & );
  /** \brief \ru Создать сплайн.
             \en Create spline. \~
    \details \ru Создать сплайн и установить параметры сплайна.\n
             \en Create spline and set parameters of spline.\n \~
    \param[in] initList - \ru Набор контрольных точек.
                          \en Set of control points. \~
    \param[in] cls      - \ru Признак замкнутости.
                          \en A closedness attribute. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbHermit3D * Create( const SArray<MbCartPoint3D> & initList, bool cls );
  /** \brief \ru Создать сплайн.
             \en Create spline. \~
    \details \ru Создать сплайн и установить параметры сплайна.\n
             \en Create spline and set parameters of spline.\n \~
    \param[in] initParams  - \ru Набор параметров в контрольных точках.
                             \en Set of parameters at the control points. \~
    \param[in] initPoints  - \ru Набор контрольных точек.
                             \en Set of control points. \~
    \param[in] cls         - \ru Признак замкнутости.
                             \en A closedness attribute. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbHermit3D * Create( const SArray<double> & initParams, const SArray<MbCartPoint3D> & initPoints,
                              bool cls );
  /** \brief \ru Создать сплайн.
             \en Create spline. \~
    \details \ru Создать сплайн и установить параметры сплайна.\n
             \en Create spline and set parameters of spline.\n \~
    \param[in] initParams  - \ru Набор параметров в контрольных точках.
                             \en Set of parameters at the control points. \~
    \param[in] initPoints  - \ru Набор контрольных точек.
                             \en Set of control points. \~
    \param[in] initVectors - \ru Набор производных в контрольных точках.
                             \en Set of derivatives at the control points. \~
    \param[in] cls         - \ru Признак замкнутости.
                             \en A closedness attribute. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbHermit3D * Create( const SArray<double> & initParams, const SArray<MbCartPoint3D> & initPoints,
                              const SArray<MbVector3D> & initVectors, bool cls );
  /** \brief \ru Создать сплайн, согласованный с LoftSurface.
             \en Create spline, agreed with LoftSurface. \~
    \details \ru Создать сплайн и установить параметры сплайна.\n
             \en Create spline and set parameters of spline.\n \~
    \param[in] initParams - \ru Набор параметров в контрольных точках.
                            \en Set of parameters at the control points. \~
    \param[in] initPoints - \ru Набор контрольных точек.
                            \en Set of control points. \~
    \param[in] vLabels    - \ru Массив, содержащий номера соседних точек с одинаковыми производными.
                            \en Array, containing indexes of points with same derivative. \~
    \param[in] cls        - \ru Признак замкнутости.
                            \en A closedness attribute. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbHermit3D * Create( const SArray<double> & initParams, const SArray<MbCartPoint3D> & initPoints,
                              const SArray<int> & vLabels, bool cls );
  /** \brief \ru Создать сплайн.
             \en Create spline. \~
    \details \ru Создать сплайн и установить параметры сплайна.\n
             \en Create spline and set parameters of spline.\n \~
    \param[in] bezier - \ru Двумерный сплайн.
                        \en The two-dimensional spline. \~ 
    \param[in] place  - \ru Локальная система координат сплайна.
                        \en Local coordinate system of spline. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbHermit3D * Create( const MbHermit & init, const MbPlacement3D & plane );
  /** \brief \ru Создать сплайн.
             \en Create spline. \~
    \details \ru Создать прямолинейный сплайн и установить параметры сплайна.\n
             \en Create a straight spline and set parameters of spline.\n \~
    \param[in] p1 - \ru Начальная точка кривой.
                    \en Start point of curve. \~
    \param[in] p2 - \ru Конечная точка кривой.
                    \en End point of curve. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbHermit3D * Create( const MbCartPoint3D & p1, const MbCartPoint3D & p2 );
  /** \brief \ru Создать сплайн.
             \en Create spline. \~
    \details \ru Создать сплайн и установить параметры сплайна.\n
             \en Create spline and set parameters of spline.\n \~
    \param[in] t1 - \ru Начальный параметр.
                    \en The initial parameter. \~
    \param[in] p1 - \ru Начальная точка кривой.
                    \en Start point of curve. \~
    \param[in] v1 - \ru Касательный вектор к кривой в начальной точке.
                    \en A tangent vector to the curve at the start point. \~
    \param[in] t2 - \ru  Конечный параметр.
                    \en The final parameter. \~
    \param[in] p2 - \ru Конечная точка кривой.
                    \en End point of curve. \~
    \param[in] v2 - \ru Касательный вектор к кривой в конечной точке.
                    \en A tangent vector to the curve at the end point. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbHermit3D * Create( double t1, const MbCartPoint3D & p1, const MbVector3D & v1,
                              double t2, const MbCartPoint3D & p2, const MbVector3D & v2 );

public :
  VISITING_CLASS( MbHermit3D );

  /// \ru Установить параметры сплайна по точкам и флагу замкнутости. \en Set parameters of spline by points and closeness flag.
  bool    Init( const SArray<MbCartPoint3D> & initPoints, bool cls );
  /// \ru Установить параметры сплайна по параметрам, точкам и флагу замкнутости. \en Set parameters of spline by parameters, points and closeness flag.
  bool    Init( const SArray<double> & initParams,
                const SArray<MbCartPoint3D> & initPoints, bool cls );
  /// \ru Установить параметры сплайна. \en Set parameters of spline.
  bool    Init( const SArray<double> & initParams,
                const SArray<MbCartPoint3D> & initPoints, 
                const SArray<MbVector3D> & initVectors, bool cls );
  /// \ru Установить параметры сплайна. \en Set parameters of spline.
  bool    Init( const SArray<double> & initParams,
                const SArray<MbCartPoint3D> & initPoints, 
                const SArray<int> & vLabels, bool cls );
  /// \ru Установить параметры сплайна по другому сплайну Эрмита. \en Set parameters of spline by another spline of Hermit.
  void    Init( const MbHermit3D & );
  /// \ru Установить параметры сплайна по двумерному сплайну Эрмита. \en Set parameters of spline by two-dimensional spline of Hermit.
  void    Init( const MbHermit &, const MbPlacement3D & );
  /// \ru Установить параметры сплайна. \en Set parameters of spline.
  void    Init( double t1, const MbCartPoint3D & p1, const MbVector3D & v1, 
                double t2, const MbCartPoint3D & p2, const MbVector3D & v2 ); 

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeSpaceType  IsA() const override; // \ru Тип элемента \en Type of element 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override;
  bool    SetEqual ( const MbSpaceItem & ) override;          // \ru Сделать равным \en Make equal 
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;             // \ru Преобразовать \en Transform.
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;             // \ru Сдвиг \en Translation
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Поворот \en Rotation

  void    GetProperties( MbProperties & ) override;   // \ru Выдать свойства объекта \en Get properties of the object 
  void    SetProperties( const MbProperties & ) override;   // \ru Записать свойства объекта \en Set properties of the object 

  // \ru Общие функции кривой \en Common functions of curve 

  double  GetTMax() const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter 
  double  GetTMin() const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter 
  // \ru Функции для работы внутри области определения кривой. \en Functions for working inside of the curve domain. \~
  void    PointOn  ( double & t, MbCartPoint3D & ) const override; // \ru Точка на кривой \en Point on curve 
  void    FirstDer ( double & t, MbVector3D & ) const override;  // \ru Первая производная \en First derivative 
  void    SecondDer( double & t, MbVector3D & ) const override;  // \ru Вторая производная \en Second derivative 
  void    ThirdDer ( double & t, MbVector3D & ) const override;  // \ru Третья производная \en Third derivative 
  // \ru Функции для работы внутри и вне области определения кривой. \en Functions for working inside and outside of the curve domain. \~
  void   _PointOn  ( double t,  MbCartPoint3D &p ) const override;
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void    Explore( double & t, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const override;

  void    Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction
  double  Step         ( double t, double sag ) const override; // \ru Вычисление шага аппроксимации \en Calculation of approximation step 
  double  DeviationStep( double t, double angle ) const override;

  MbNurbs3D * NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;
  MbCurve3D * NurbsCurve( const MbNurbsParameters & ) const override; // \ru Построить NURBS-копию кривой \en Create a NURBS-copy of the curve 

  // \ru Построить усеченную кривую c учетом заданной трехмерной точности. \en Construct a trimmed curve with the given three-dimensional accuracy.
  MbCurve3D * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyzEps, bool saveParamLenAndLaw ) const override;
  using MbCurve3D::Trimmed; // \ru Метод базового класса MbCurve3D без учета трехмерной точности. Точность по умолчанию - MbDimAccuracy::threeDimAcc.  \en Method of MbCurve3D base class without three-dimensional accuracy. The default accuracy is MbDimAccuracy::threeDimAcc.

  bool    IsPlanar  ( double accuracy = METRIC_EPSILON ) const override;                            // \ru Является ли кривая плоской \en Whether a curve is planar. 
  bool    IsStraight( bool ignoreParams = false ) const override; // \ru Признак прямолинейности кривой \en An attribute of curve straightness 

  bool    GetPlacement( MbPlacement3D & place, PlanarCheckParams params = PlanarCheckParams() ) const override; // \ru Заполнить плейсемент, ести кривая плоская \en Fill the placement if curve is planar 
  // \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская (после использования вызывать DeleteItem на двумерную кривую) \en Give a planar curve and placement, if the spatial curve is planar (after using the DeleteItem must be called for a two-dimensional curve) 
  bool    GetPlaneCurve( MbCurve *& curve2d, MbPlacement3D & place, bool saveParams, PlanarCheckParams params = PlanarCheckParams() ) const override;

  /// \ru Получить границы участков кривой, которые описываются одной аналитической функцией. 
  /// \en Get the boundaries of the curve sections that are described by one analytical function. \~
  void    GetAnalyticalFunctionsBounds( std::vector<double> & params ) const override;

  // \ru Общие функции полигональной кривой \en Common functions of polygonal curve 

  void    Rebuild() override; // \ru Пересчитать кривую \en Rebuild the curve 
  void    SetClosed( bool cls ) override; // \ru Установить признак замкнутости. \en Set attribute of closedness.
  void    AddPoint   ( const MbCartPoint3D & pnt ) override;         // \ru Добавить точку в конец массива \en Add a point to the end of array
  void    InsertPoint( ptrdiff_t index, const MbCartPoint3D & pnt                                         )  override; // \ru Добавить точку \en Add a point
  void    InsertPoint( double t, const MbCartPoint3D & pnt, double metrEps ) override; // \ru Добавить точку \en Add a point
  virtual void    InsertPoint( double t, const MbCartPoint3D & pnt, const MbVector3D & der, double metrEps ); // \ru Добавить точку и производную. \en Add a point and derivetive.
  virtual void    SetCurveValue( double t, const MbCartPoint3D & pnt, double tDelta, const MbVector3D & der, double metrEps ); // \ru Установить точку и производную на участке. \en Set a point and derivetive at region.
  void    RemovePoint( ptrdiff_t index ) override;                        // \ru Удалить точку \en Remove a point
  bool    ChangePoint( ptrdiff_t index, const MbCartPoint3D & pnt ) override; // \ru Заменить точку \en Replace a point
  void    GetVector  ( ptrdiff_t index, MbVector3D & vec ) const; 
  bool    SetTangentVectors( const SArray<MbVector3D> & tauVectors ); // \ru vectorList[i] сделать параллельными tauVectors[i] \en Make vectorList[i] parallel to tauVectors[i] 
  size_t  GetPointsCount() const override; // \ru Выдать количество точек \en Get the number of points
  void    GetRuleInterval( ptrdiff_t index, double & t1, double & t2 ) const override; // \ru Выдать интервал влияния точки \en Get the interval of point influence 
  ptrdiff_t GetNearPointIndex( const MbCartPoint3D & pnt ) const override; // \ru Выдать индекс точки, ближайшей к заданной \en Get the point index which is nearest to the given
  bool    CheckParam ( double & t, ptrdiff_t & i0, ptrdiff_t & i1, double & t0, double & t1 ) const override; // \ru Загнать параметр получить локальный индексы и параметры \en Move parameter into domain, get local indices and parameters
  double  GetParam( ptrdiff_t i ) const override; // \ru Выдать параметр для точки с номером \en Get parameter for point with index

  bool    NearPointProjection  ( const MbCartPoint3D &, double & t, bool ext, MbRect1D * tRange = nullptr ) const override; // \ru Ближайшая проекция точки на кривую \en The nearest point projection to the curve
  double  CalculateMetricLength() const override;     // \ru Посчитать метрическую длину \en Calculate the metric length 
  void    GetWeightCentre( MbCartPoint3D &wc ) const override; // \ru Посчитать центр тяжести кривой \en Calculate the gravity center of the curve
  void    CalculateGabarit( MbCube & gab ) const override; // \ru Вычислить габарит кривой \en Calculate the bounding box of curve
  bool    DistanceAlong( double & t, double len, int curveDir, double eps = Math::metricPrecision,
                         VERSION version = Math::DefaultMathVersion() ) const override; // \ru Сдвинуть параметр t на расстояние len по направлению \en Shift the parameter t by the distance 'len' in the direction 

  // \ru Функции только 3D кривой \en Function for 3D-curve 
  MbCurve * GetMap( const MbMatrix3D &, MbRect1D * pRgn = nullptr,
                    VERSION version = Math::DefaultMathVersion(), bool * coincParams = nullptr ) const override; // \ru Дать плоскую проекцию кривой \en Get a planar projection of curve

  size_t  GetCount() const override;
  // \ru Установить область изменения параметра. \en Set range of parameter.
  bool    SetLimitParam( double newTMin, double newTMax ); 
  void    CalculateDerivatives(); 
  void    SetLimitVector( ptrdiff_t n, const MbVector3D & v );
  
  size_t  GetVectorListCount() const { return vectorList.size(); }

          template <class VectorsVector>
  void    GetVectorList( VectorsVector & vectors ) const { vectors.assign( vectorList.begin(), vectorList.end() ); }

    const MbVector3D & _GetVectorList( size_t i ) const { return vectorList[i]; }
          MbVector3D & _SetVectorList( size_t i )       { MbPolyCurve3D::Refresh(); return vectorList[i]; }

  size_t  GetTListCount() const { return tList.size(); }
  void    GetTList( SArray<double> & params ) const { params = tList; }
          double _GetTList( size_t i ) const { return tList[i]; }

  void    LocalCoordinate( double & t,
                           ptrdiff_t & index1, ptrdiff_t & index2,
                           double    & param1, double    & param2,
                           double    & paramD, double    & paramW,
                           double    & quota1, double    & quota2 ) const;
  ptrdiff_t GetIndex( double t ) const;

          // \ru При линейном расположении нескольких точек согласовать производные на краях участка. \en Aligning the derivatives on the group if several points are located linearly.
  bool    DerivativesCorrection( double accuracy );

private:
  bool    Break( MbHermit3D & trimPart, double t1, double t2, double tRegion ) const; // \ru Разбить на две части \en Split into two parts 
  bool    SetCorrection( size_t ind, double tDelta ); // \ru Скорректировать кривую по индексу. \en Curve correction by index.
  void    CalculateValues( size_t i1, size_t i2 ); // \ru Скорректировать кривую на интервале i1-i2. \en Curve correction on the interval i1-i2.

  void    operator = ( const MbHermit3D & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbHermit3D )
};

IMPL_PERSISTENT_OPS( MbHermit3D )


//------------------------------------------------------------------------------
// \ru Определение местных координат области поверхности \en Definition of local coordinates in a surface region 
// ---
inline void MbHermit3D::LocalCoordinate( double & t,
                                 ptrdiff_t & index1, ptrdiff_t & index2,
                                 double    & param1, double    & param2,
                                 double    & paramD, double    & paramW,
                                 double    & quota1, double    & quota2 ) const
{
  double tmin = tList[0];
  double tmax = tList[splinesCount];
  bool bmin = t < tmin;

  if ( bmin || t > tmax ) { // \ru Параметр вне границ \en Parameter is out of bounds 
    if ( closed ) { 
      double tmp = tmax - tmin;
      t -= ::floor((t - tmin) / tmp) * tmp; 
    }
    else {
      if ( bmin ) { // \ru Начальный участок \en Starting piece  
        t  = tmin;
        index1 = 0;
        index2 = 1;
      }
      else {        // \ru Конечный участок \en Ending piece  
        t  = tmax;
        index1 = splinesCount-1;
        index2 = (index1 + 1) % (uppIndex + 1);
      }
      param1 = tList[index1];
      param2 = tList[index1+1];
      paramD = ( param2 - param1 );
      paramW = 1.0 / paramD;
      if ( ::fabs(t - param1) < DOUBLE_EPSILON ) {
        quota1 = 1.0;
        quota2 = 0.0;
      }
      else if ( ::fabs(t - param2) < DOUBLE_EPSILON ) {
        quota1 = 0.0;
        quota2 = 1.0;
      }
      else {
        quota1 = ( param2 - t) * paramW;
        quota2 = ( t - param1) * paramW;
      }
      return;
    }
  }

  // \ru Устанавливаем диапазон поиска \en Set the search range 
  if ( index1>=0 && index1<splinesCount ) {
    if ( tList[index1]<=t && t<tList[index1+1] ) { // \ru Предыдущее значение верно \en The previous value is true 
      index2 = (index1 + 1) % (uppIndex + 1);
      param1 = tList[index1];
      param2 = tList[index1+1];
      paramD = ( param2 - param1 );
      paramW = 1.0 / paramD;
      if ( ::fabs(t - param1) < DOUBLE_EPSILON ) {
        quota1 = 1.0;
        quota2 = 0.0;
      }
      else if ( ::fabs(t - param2) < DOUBLE_EPSILON ) {
        quota1 = 0.0;
        quota2 = 1.0;
      }
      else {
        quota1 = ( param2 - t ) * paramW;
        quota2 = ( t - param1 ) * paramW;
      }
      return;
    }
    index2 = index1 + 1;
    if ( index1>0 )
      index1--;
    if ( t < tList[index1] )
      index1 = 0;
    if ( index2 < splinesCount )
      index2++;
    if ( t >= tList[index2] )
      index2 = splinesCount;
  }
  else {
    index1 = 0;
    index2 = splinesCount;
  }

  ptrdiff_t ind, delta = index2 - index1; // \ru Диапазон \en A range 

  // \ru Поиск половинным делением \en Search by bisection 
  while ( delta>1 ) {
    ind = index1 + ( delta / (ptrdiff_t)2 ); // \ru Индекс в середине \en The index in the middle 
    if ( t < tList[ind] )  // \ru Если t больше серединного параметра \en If t is greater than the middle parameter 
      index2 = ind; // \ru Изменить правую границу \en Change the right bound 
    else 
      index1 = ind; // \ru Изменить левую границу \en Change the left bound 
    delta = index2 - index1; // \ru Диапазон \en A range 
  }
  index2 = index2 % (uppIndex+1);
  param1 = tList[index1];
  param2 = tList[index1+1];
  paramD = ( param2 - param1 );
  paramW = 1.0 / paramD;
  if ( ::fabs(t - param1) < DOUBLE_EPSILON ) {
    quota1 = 1.0;
    quota2 = 0.0;
  }
  else if ( ::fabs(t - param2) < DOUBLE_EPSILON ) {
    quota1 = 0.0;
    quota2 = 1.0;
  }
  else {
    quota1 = ( param2 - t ) * paramW;
    quota2 = ( t - param1 ) * paramW;
  }
}


//------------------------------------------------------------------------------
// \ru Вычисление вектора производной в средней точке по трём точкам параболы \en Calculation of derivative vector at the middle point by three points of parabola 
// pPrev = pointList[i-1]
// point = pointList[i]
// pNext = pointList[i+1]
// paramDeltaPrev = tList[i] - tList[i-1]
// paramDeltaNext = tList[i+1] - tList[i]
// derivative = vectorList[i]
// --- 
inline 
void HermitDerivative( const MbCartPoint3D & pPrev, const MbCartPoint3D & point, const MbCartPoint3D & pNext, 
                       double paramDeltaPrev, double paramDeltaNext,
                       MbVector3D & derivative )
{
  double dt  = paramDeltaNext + paramDeltaPrev;
  double dtPrev = -dt * paramDeltaPrev;
  double dtNext =  dt * paramDeltaNext;
  if ( ::fabs(dtPrev) > Math::paramRegion && ::fabs(dtNext) > Math::paramRegion ) { 
    // \ru Находим из равенства производной в средней точке параболы, построенной по трём точкам \en Find from equation of the derivative at the middle point of the parabola built by three points 
    double _dtPrev = 1.0 / dtPrev; 
    double _dtNext = 1.0 / dtNext; 
    derivative.Set( pPrev, paramDeltaNext * _dtPrev, point, -dt * _dtPrev,
                    pNext, paramDeltaPrev * _dtNext, point, -dt * _dtNext ); // \ru Не ломается 2)Часть дейдвуда.c3d \en Isn't crash 2)Part of deadwood.c3d 
//    derivative.Set( pPrev, paramDeltaNext / dtPrev, point, -dt / dtPrev,
//pNext, paramDeltaPrev / dtNext, point, -dt / dtNext ); // \ru Ломается 4)7-016.c3d \en // crashed 4)7-016.c3d 
  }
  else {
    if ( ::fabs(dt) > NULL_EPSILON )
      derivative = (pNext - pPrev) / dt; 
  }
}


#endif // __CUR_HERMIT3D_H
