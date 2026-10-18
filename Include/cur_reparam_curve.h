////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Репараметризованная кривая в двумерном пространстве.
         \en Reparametrized curve in two-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_REPARAM_CURVE_H
#define __CUR_REPARAM_CURVE_H


#include <curve.h>


class MbRegDuplicate;
class MbRegTransform;
class MbFunction;


//------------------------------------------------------------------------------
/** \brief \ru Репараметризованная кривая в двумерном пространстве.
           \en Reparametrized curve in two-dimensional space. \~
  \details \ru Репараметризованная кривая служит для согласования областей определения кривых. \n
    Геометрически репараметризованная кривая полностью совпадает с базовой кривой basisCurve.
    Репараметризованная кривая имеет другую область определения и, как следствие, другую длину своих производных.
    Параметры базовой кривой и репараметризованной кривой связаны равенством: \n
    dt (t - tmin) = t_basisCurve - tmin_basisCurve, где t - параметр репараметризованной кривой.
    Базовой кривой для репараметризованной кривой не может служить другая репараметризованная кривая.
    В подобной ситуации выполняется переход к первичной базовой кривой.
           \en Reparametrized curve is used for matching domains of curves. \n
    Geometrically reparametrized curve completely coincides with base curve 'basisCurve'.
    Reparametrized curve has another definition domain and as a result another length of its derivatives.
    Parameters of base curve and reparametrized curve are related by the equality: \n
    dt (t - tmin) = t_basisCurve - tmin_basisCurve, where 't' - parameter of reparametrized curve.
    Another reparametrized curve can't be the base curve for a reparametrized curve.
    In this situation it changes to the initial base curve. \~
  \ingroup Curves_2D
*/
// ---
class MATH_CLASS MbReparamCurve : public MbCurve {
public:
  enum MbeReparamType {
    rt_Linear,        ///< \ru Линейная репараметризация. \en Linear reparametrization.
    rt_Quadratic,     ///< \ru Квадратичная репараметризация. \en Quadratic reparametrization. 
    rt_ScaledEndDers, ///< \ru Репараметризация с заданными масштабами производной на концах. \en Reparametrization with a given scale of the derivative at the ends.
    rt_Proportional,  ///< \ru Пропорциональная длине кривой репараметризация. \en The poportional to the length of the curve parametrization.
    rt_User           ///< \ru Функция, заданная пользователем. \en User defined function.
  };

protected :
  MbCurve *        basisCurve;  ///< \ru Базовая кривая. \en The base curve.
  MbeReparamType   reparamType; ///< \ru Способ репараметризации. \en Way of repatametrization.
  SPtr<MbFunction> repFunction; ///< \ru Репараметризующая функция вычисления параметра базовой кривой. \en Reparametric function for calculation the base curve parameter.

public :
  /// \ru Конструктор по кривой и новым параметрам. \en Constructor by curve and by new parametric limits.
  MbReparamCurve( const MbCurve &, double t1, double t2 );
  /// \ru Конструктор по кривой и новым параметрам. \en Constructor by curve and by new parametric limits.
  MbReparamCurve( const MbCurve &, const double t1, const double t2, const double begFirstDerValue );
  /// \ru Конструктор по кривой и производным параметра на ее концах. \en Constructor for the curve and derivatives of the parameter at its ends
  MbReparamCurve( double dt1, double dt2, const MbCurve & curve );
protected:
  MbReparamCurve( const MbReparamCurve &, MbRegDuplicate * );
private :
  MbReparamCurve( const MbReparamCurve & ); // \ru Не реализовано. \en Not implemented. 
public :
  virtual ~MbReparamCurve();

public :
  VISITING_CLASS( MbReparamCurve ); 

  /// \ru Установить параметрическую область кривой. \en Set curve parametric range.
  void        Init( double t1, double t2 );
  /// \ru Установить параметрическую область кривой и длину производной в начале кривой. \en Set curve parametric range and first derive length.
  void        Init( double t1, double t2, double begFirstDerValue );
  /// \ru Установить параметрическую область кривой. \en Set curve parametric range.
  void        InitScaledEnds( double scaleDer1, double scaleDer2 );
  /// \ru Установить параметрическую область кривой пропорциональную метрической длине кривой. Ожидается, что первая производная базовой кривой r'(t) является непрерывной функцией параметра t по всей ее длине \en Set the parametric area of the curve proportional to the metric length of the curve. The first derivative of the base curve r'(t) is expected to be a continuous function of the parameter t over its entire length
  bool        InitProportional( double t1, double t2 );
  /// \ru Установить пользовательскую функцию репараметризации. \en Set users reparameterization function.
  bool        InitByUsersFunction( MbFunction & repFunc, MbeReparamType tp = rt_User );
  /// \ru Установить параметрическую область кривой пропорциональную метрической длине кривой. \en Set the parametric area of the curve proportional to the metric length of the curve.
  static  MbReparamCurve * CreateProportional( const MbCurve & curve, double t1, double t2 );
  /// \ru Установить пользовательскую функцию репараметризации. \en Set users reparameterization function.
  static  MbReparamCurve * CreateByFunction( const MbCurve & curve, MbFunction & repFunc, MbeReparamType tp = rt_User );

  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */

  MbePlaneType  IsA() const override; // \ru Тип элемента \en Type of element
  bool        IsSimilar( const MbPlaneItem & ) const override; // \ru Являются ли элементы подобными \en Whether the elements are similar
  bool        SetEqual ( const MbPlaneItem & ) override;       // \ru Сделать элементы равными \en Make the elements equal
  void        Transform( const MbMatrix &, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move( const MbVector &, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate( const MbCartPoint &, const MbDirection & angle, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Поворот \en Rotation
   bool        IsSame( const MbPlaneItem &, double accuracy = LENGTH_EPSILON ) const override;
  MbCurve *   Offset( double rad ) const override;  // \ru Смещение усеченной кривой \en Shift of a trimmed curve
  MbPlaneItem & Duplicate( MbRegDuplicate * = nullptr ) const override;
  // \ru Построить усеченную кривую c учетом заданной двумерной точности. \en Construct a trimmed curve with the given two-dimensional accuracy.
  MbCurve *   Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyEps, bool saveParamLenAndLaw ) const override;
  using MbCurve::Trimmed; // \ru Метод базового класса MbCurve без учета двумерной точности. Точность по умолчанию - MbDimAccuracy::twoDimAcc.  \en Method of MbCurve base class without two-dimensional accuracy. The default accuracy is MbDimAccuracy::twoDimAcc.

  MbContour * NurbsContour() const override; // \ru Построить контур \en Create a contour
  void        AddYourGabaritTo( MbRect & ) const override; // \ru Добавь свой габарит в прямой прям-к \en Add your own gabarit into the given bounding rectangle
  void        CalculateGabarit( MbRect & ) const override; // \ru Определить габариты кривой \en Determine the bounding box of a curve
  bool        IsVisibleInRect( const MbRect &, bool exact = false ) const override; // \ru Виден ли объект в заданном прямоугольнике \en Whether the object is visible in the given rectangle
  using      MbCurve::IsVisibleInRect;   
  const   MbFunction * GetFunction() const { return repFunction;  } // \ru Получить функцию. \en Get function 
  /** \} */

  /** \ru \name Функции описания области определения кривой.
      \en \name Functions for description of a curve domain.
      \{ */
  double      GetTMax () const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter
  double      GetTMin () const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of the parameter
  bool        IsClosed() const override; // \ru Проверка замкнутости кривой \en Check for curve closedness
  double      GetPeriod() const override; // \ru Вернуть период \en Get period
  /** \} */

  /** \ru \name Функции для работы в области определения кривой. 
            Функции PointOn, FirstDer, SecondDer, ThirdDer,... корректируют параметр
            при выходе его за пределы области определения.
            Исключение составляет MbLine (прямая).
      \en \name Functions for working in the curve's domain. 
            Functions PointOn, FirstDer, SecondDer, ThirdDer,... correct parameter
            if it is out of domain bounds.
            Except MbLine (line).
      \{ */
  void        PointOn  ( double & t, MbCartPoint & p  ) const override; // \ru Точка на кривой \en Point on the curve
  void        FirstDer ( double & t, MbVector    & fd ) const override; // \ru Первая производная \en The first derivative
  void        SecondDer( double & t, MbVector    & sd ) const override; // \ru Вторая производная \en The second derivative
  void        ThirdDer ( double & t, MbVector    & td ) const override; // \ru Третья производная \en The third derivative
  /** \} */

  /** \ru \name Функции для работы внутри и вне области определения кривой. 
            Функции _PointOn, _FirstDer, _SecondDer, _ThirdDer,... не корректируют параметр
            при выходе за пределы области определения. При выходе за область определения незамкнутая
            кривая, в общем случае, продолжается по касательной, которую она имеет на соответствующем конце.
            Исключение составляют дуги эллипса и окружности - они продолжаются в соответствии со своими уравнениями.
      \en \name Functions for working inside and outside the curve's domain. 
            Functions _PointOn, _FirstDer, _SecondDer, _ThirdDer,... don't correct parameter
            if it is out of domain bounds. If the parameter is out of domain bounds, an unclosed
            curve is extended by tangent vector at corresponding end point in general case.
            Except arcs of an ellipse or a circle - they are extended according to their equations.
      \{ */
  void       _PointOn  ( double t, MbCartPoint & p ) const override;
  void       _FirstDer ( double t, MbVector    & v ) const override;
  void       _SecondDer( double t, MbVector    & v ) const override;
  void       _ThirdDer ( double t, MbVector    & v ) const override;
  /** \} */

  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметра кривой.
      \en \name Functions for get of the group of data inside and outside the curve's domain of parameter.
      \{ */
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void        Explore( double & t, bool ext,
                       MbCartPoint & pnt, MbVector & fir, MbVector * sec, MbVector * thir ) const override;
  /** \} */

  /** \ru \name Функции движения по кривой
      \en \name Functions of moving along the curve
    \{ */
  double      Step( double t, double sag ) const override; // \ru Вычисление шага аппроксимации с учетом радиуса кривизны \en Calculation of step of approximation with consideration of curvature radius
  double      DeviationStep( double t, double _atol ) const override; // \ru Вычисление шага аппроксимации с учетом радиуса кривизны \en Calculation of step of approximation with consideration of curvature radius
  /** \} */

  /** \ru \name Общие функции кривой
      \en \name Common functions of curve
      \{ */
  void        Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменение направления кривой на противоположное \en Change curve direction to the opposite one
  double      DistanceToPoint( const MbCartPoint & toP ) const override; // \ru Расстояние до точки \en Distance to a point
  bool        DistanceToPointIfLess( const MbCartPoint & toP, double & d ) const override; // \ru Расстояние до точки, если оно меньше d \en Distance to a point if it is less than 'd'
  MbeState    Deformation( const MbRect &, const MbMatrix & ) override;  // \ru Деформация \en Deformation
  // \ru Удалить часть кривой между параметрами t1 и t2 \en Delete a part of a curve between parameters t1 and t2 
  MbeState    DeletePart( double t1, double t2, MbCurve *& part2 ) override;
  // \ru Оставить часть кривой между параметрами t1 и t2 \en Keep a piece of a curve between parameters t1 and t2  
  MbeState    TrimmPart( double t1, double t2, MbCurve *& part2 ) override;

  MbNurbs *   NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;
  MbCurve *   NurbsCurve( const MbNurbsParameters & ) const override; // \ru Построить NURBS-копию кривой \en Create a NURBS-copy of the curve

  bool        IsBounded()    const override; // \ru Признак ограниченной кривой \en Attribute of a bounded curve
  bool        IsDegenerate( double eps = Math::LengthEps ) const override; // \ru Проверка вырожденности кривой \en Check for curve degeneracy
  bool        IsStraight( bool ignoreParams = false ) const override; // \ru Является ли линия прямолинейной \en Whether the line is straight
  bool        IsSmoothConnected( double angleEps, bool ignoreLimits = false ) const override; // \ru Являются ли стыки контура\кривой гладкими? \en Whether the joints of a contour\curve are smooth.
  bool        IsCompleteInRect( const MbRect & r ) const override; // \ru Виден ли объект полностью в в заданном прямоугольнике \en Whether the object is completely visible in the given rectangle
  double      CalculateMetricLength() const override; // \ru Посчитать метрическую длину \en Calculate the metric length
  bool        HasLength( double & length ) const override; // \ru Метрическая длина кривой \en Metric length of a curve
  double      GetMetricLength() const override; // \ru Метрическая длина кривой \en Metric length of a curve
  double      GetLengthEvaluation() const override; // \ru Оценка метрической длины кривой \en Estimation of metric length of the curve
  double      Curvature      ( double t ) const override; // \ru Кривизна кривой \en Curvature of the curve

  double      CalculateLength( double t1, double t2 ) const override;
  // \ru Положение точки относительно кривой. \en The point position relative to the curve. 
  // \ru Возвращает результат : \en Returning result: 
  // \ru iloc_InItem = 1 - точка находится слева от кривой, \en Iloc_InItem = 1 - point is to the left of the curve, 
  // \ru iloc_OnItem = 0 - точка находится на кривой, \en Iloc_OnItem = 0 - point is on the curve, 
  // \ru iloc_OutOfItem = -1 - точка находится справа от кривой. \en Iloc_OutOfItem = -1 - point is to the right of the curve. 
  MbeItemLocation PointRelative( const MbCartPoint & pnt, double eps = Math::LengthEps ) const override;
  void        Refresh() override; // \ru Сбросить все временные данные \en Reset all temporary data 
  void        PrepareIntegralData( const bool forced ) const override; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.

  double      PointProjection( const MbCartPoint & ) const override; // \ru Проекция точки на кривую \en Point projection on the curve
  bool        NearPointProjection( const MbCartPoint & pnt, double xEpsilon, double yEpsilon, 
                                           double & t, bool ext, MbRect1D * tRange = nullptr ) const override; // \ru Проекция точки на кривую или её продолжение в области поиска проекции \en Projection of a point onto the curve or its extension in the projection region
  // \ru Вычисление всех перпендикуляров к кривой из данной точки \en Calculation of all the perpendiculars to the curve from a given point 
  void        PerpendicularPoint( const MbCartPoint & pnt, SArray<double> & tFind ) const override;
  bool        SmallestPerpendicular( const MbCartPoint & pnt, double & tProj ) const override; // \ru Нахождение ближайшего перпендикуляра к кривой из данной точки \en Calculation of the closest perpendicular to the curve from the given point
  void        TangentPoint( const MbCartPoint & pnt, SArray<double> & tFind ) const override; // \ru Вычисление всех касательных к кривой из данной точки \en Calculation of all the tangents to the curve from a given point
  void        IntersectHorizontal( double y, SArray<double> & ) const override; // \ru Пересечение кривой с горизонтальной прямой \en Intersection of a curve with a horizontal line
  void        IntersectVertical  ( double x, SArray<double> & ) const override; // \ru Пересечение с вертикальной прямой \en Intersection with a vertical line
  void        SelfIntersect( SArray<MbCrossPoint> &, double metricEps = Math::LengthEps ) const override; // \ru Самопересечение произвольной кривой \en Self-intersection of an arbitrary curve

  // \ru Определение особых точек офсетной кривой \en Determination of special points of an offset curve 
  void        OffsetCuspPoint( SArray<double> & tCusps, double dist ) const override;
  bool        GetMiddlePoint( MbCartPoint & ) const override; // \ru Выдать среднюю точку кривой \en Get mid-point of a curve
  bool        GoThroughPoint( MbCartPoint & p0 ) override;
  // \ru Вычисление минимальной длины кривой между двумя точками на ней \en Calculate the minimal curve length between two points on it 
  double      LengthBetween2Points( MbCartPoint & p1, MbCartPoint & p2, MbCartPoint * pc = nullptr ) const override;
  bool        GetSpecificPoint( const MbCartPoint & from, double & dmax, MbCartPoint & pnt ) const override; // \ru Выдать характерную точку кривой если она ближе чем dmax \en Get control point of curve if it is closer than 'dmax'
  bool        GetWeightCentre( MbCartPoint & c ) const override; // \ru Выдать центр тяжести кривой \en Get the center of gravity of the curve
  bool        GetCentre( MbCartPoint & c ) const override; // \ru Выдать центр кривой \en Get center of curve
  double      GetRadius( double accuracy = PARAM_REGION ) const override; // \ru Дать физический радиус объекта или ноль, если это невозможно. \en Get the physical radius of the object or null if it impossible.
  bool        DistanceAlong( double & t, double len, int curveDir, double eps = Math::LengthEps,
                                     VERSION version = Math::DefaultMathVersion() ) const override; // \ru Сдвинуть параметр t на расстояние len по направлению \en Translate parameter 't' by distance 'len' along the direction
  bool        GetAxisPoint( MbCartPoint & ) const override; // \ru Точка для построения оси \en Point for the axis construction 
  bool        IsSimilarToCurve( const MbCurve & curve, double precision = PARAM_PRECISION ) const override; // \ru Подобные ли кривые для объединения (слива) \en Whether the curves for union (joining) are similar
  size_t      GetCount() const override; // \ru Количество разбиений для прохода в операциях \en Count of subdivisions for pass in operations
  /// \ru Получить границы участков кривой, которые описываются одной аналитической функцией. \en Get the boundaries of the curve sections that are described by one analytical function. \~
  void        GetAnalyticalFunctionsBounds( std::vector<double> & params ) const override;
  void        GetPointsByEvenLengthDelta( size_t n,  std::vector<MbCartPoint> & pnts ) const override; // \ru Выдать n точек кривой с равными интервалами по длине дуги \en Get n points of curve equally spaced by the arc length

  void        ParameterInto( double & ) const; // \ru Перевод параметра базовой кривой в локальный параметр \en Transformation of the base curve parameter to a local parameter 
  void        ParameterFrom( double & ) const; // \ru Перевод локального параметра в параметр базовой кривой \en Transformation of a local parameter to the base curve parameter 
  double      EpsilonInto( double eps ) const; // \ru Перевод точности параметра базовой кривой в точность локального параметра \en Transformation of the base curve parameter tolerance to a local parameter tolerance 
  double      EpsilonFrom( double eps ) const; // \ru Перевод точности локального параметра в точность параметра базовой кривой \en Transformation of a local parameter tolerance to the base curve parameter tolerance 

  const MbCurve & GetBasisCurve() const override;
        MbCurve & SetBasisCurve() override;

  bool        SetBasisCurve( const MbCurve &, const MbRect1D * tRange = nullptr ); ///< \ru Заменить плоскую кривую \en Replace the planar curve
  double      Tmin() const; ///< \ru Начальный параметр. \en Start parameter. 
  double      Tmax() const; ///< \ru Конечный  параметр. \en End parameter. 
  double      Dt()   const; ///< \ru Производная параметра кривой basisCurve по параметру. \en Derivative of parameter of 'basisCurve' curve by parameter. 
  bool        SetTmin( double t );
  bool        SetTmax( double t );
  bool        SetDt  ( double d );

  MbeReparamType GetReparamType() const { return reparamType; } // \ru Тип параметризации. \en Parameterization type.

  // \ru !!! геометрия подложки тождественна геометрии кривой, отлична параметризация !!! \en !!! geometry of substrate is identical to geometry of curve, parameterization is different !!! 
  const MbCurve & GetSubstrate() const override; // \ru Выдать подложку или себя \en Get substrate or itself
        MbCurve & SetSubstrate() override;       // \ru Выдать подложку или себя \en Get substrate or itself
  int         SubstrateCurveDirection() const override;    // \ru Направление подложки относительно кривой или наоборот \en Direction of substrate relative to the curve or vice versa
  void        SubstrateToCurve( double & ) const override; // \ru Преобразовать параметр подложки в параметр кривой \en Transform a substrate parameter to the curve parameter
  void        CurveToSubstrate( double & ) const override; // \ru Преобразовать параметр кривой в параметр подложки \en Transform a curve parameter to the substrate parameter

  void        GetProperties( MbProperties & ) override;         // \ru Выдать свойства объекта \en Get properties of the object
  void        SetProperties( const MbProperties & ) override;   // \ru Записать свойства объекта \en Set properties of the object
  void        GetBasisPoints( MbControlData & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  bool        IsContinuousDerivative( bool & contLength, bool & contDirect, c3d::DoubleVector * params = nullptr, double epsilon = EPSILON ) const override; // \ru Непрерывна ли первая производная? \en Have the first derivative the continuous?
  // \ru Устранить разрывы первых производных по длине. \en Eliminate the discontinuities of the first derivative at length.
  bool        SetContinuousDerivativeLength( VERSION version, double epsilon = EPSILON ) override;

  bool        IsProportional() const { return (reparamType == rt_Proportional); } ///< \ru Является ли репараметризация пропорциональной? \en Is the re-parametrization proportional?
  bool        IsLinear() const { return (reparamType == rt_Linear); } // \ru Является ли репараметризация линейной? \en Is the re-parametrization linear?
  bool        SetLimitParam( double newTMin, double newTMax ); // \ru Установить область изменения параметра \en Set range of parameter  

  /** \} */

private:
  void        operator = ( const MbReparamCurve & ); // \ru Не реализовано. \en Not implemented. 
  // \ru Параметр базовой кривой и его производные. \en The base curve parameter and its derivatives.
  void        Explore( double & t, bool ext, double & par, double & dpar, double * ddpar, double * dddpar ) const; 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbReparamCurve )
}; // MbReparamCurve

IMPL_PERSISTENT_OPS( MbReparamCurve )


#endif // __CUR_REPARAM_CURVE_H
