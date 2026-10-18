////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Модуль геометрических построений.
         \en The module of geometric constructions. \~
  \details \ru Структуры данных и функции для аппроксимации точек NURBS кривой
               с возможностью сглаживания и учета констрейнов на точки и производные.
           \en Data structures and functions for points approximation by NURBS curve
               with smoothing and points and derivatives constraints. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_SMOOTH_NURBS_FIT_CURVE_H
#define __MB_SMOOTH_NURBS_FIT_CURVE_H


#include <mb_cart_point3d.h>
#include <mb_cart_point.h>
#include <templ_sptr.h>
#include <mb_operation_result.h>
#include <vector>


class  MbNurbs3D;
class  MbNurbs;


//-------------------------------------------------------------------------------
/** \brief \ru Параметризация точек по длине полилинии.
           \en Parameterization by length. \~
  \details \ru Параметризация точек по длине полилинии.
           \en Parameterization by length. \~
  \ingroup Data_Structures
*/
// ---
template <class Point>
double ParameterizeByLength( const std::vector<Point> & aPt, c3d::DoubleVector & aPrm, double epsDegenerated = PARAM_EPSILON )
{
  const auto nPt = aPt.size();
  aPrm.resize( nPt );
  double len = 0.;
  aPrm[0] = 0.;
  for ( size_t i = 1; i < nPt; ++i )
  {
    len += ( aPt[i] - aPt[i - 1] ).Length();
    aPrm[i] = len;
  }

  if ( len > epsDegenerated )
  {
    len = 1. / len;
    for ( size_t i = 1; i < nPt; ++i )
      aPrm[i] *= len;
    aPrm.back() = 1.;
  }

  return len;
}


//-------------------------------------------------------------------------------
/** \brief \ru Параметры точек для аппроксимации NURBS кривой.
           \en Parameters for the approximation by a NURBS curve. \~
  \details \ru Параметры точек для аппроксимации NURBS кривой.
               Констрейны на точки и производные могут быть интерполяционные и аппроксимационные.
               Признаком интерполяционного констрейна является любой отрицательный вес.
           \en Parameters for the approximation by a NURBS curve.
               There are two constraint types - interpolational and approximational.
               A sign of an interpolation constrain is any negative weight. \~
  \ingroup Data_Structures
*/
// ---
template<class Vector>
struct MbApproxWeightConstraint
{
public:
  double _weight;      ///< \ru Вес точки. Если отрицательный - точка зафиксирована. \en Point's weight. Negative weight means that point is fixed. 
  double _weightDer;   ///< \ru Вес производной. Если отрицательный - производная зафиксирована. 
                       ///      Если равен нулю - констрейн отсутствует.
                       ///  \en Derivative's weight.
                       ///      Negative weight means that point is fixed. Zero weight means there is no constraint.
  Vector _deriv;       ///< \ru Значение производной. \en Derivative value. 

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbApproxWeightConstraint()
    : _weight   ( 1. )
    , _weightDer( 0. )
    , _deriv    (    )
  {}

  /// \ru Конструктор копирования. \en The copy constructor.  
  MbApproxWeightConstraint(const MbApproxWeightConstraint & that )
    : _weight   ( that._weight    )
    , _weightDer( that._weightDer )
    , _deriv    ( that._deriv     )
  {}

  /// \ru Конструктор по параметрам. \en The constructor by parameters. 
  MbApproxWeightConstraint( double weight, double weightDer, const Vector & deriv )
    : _weight   ( weight    )
    , _weightDer( weightDer )
    , _deriv    ( deriv     )
  {}

  /// \ru Конструктор по параметрам. \en The constructor by parameters. 
  MbApproxWeightConstraint( double weight )
    : _weight   ( weight )
    , _weightDer( 0.     )
    , _deriv    (        )
  {}

  /// \ru Оператор присваивания. \en The assignment operator. 
  MbApproxWeightConstraint & operator = ( const MbApproxWeightConstraint & that ) 
  {
    _weight = that._weight;
    _weightDer = that._weightDer;
    _deriv = that._deriv;
    return ( *this );
  }

public:
  /// \ru Получить вес точки. \en Get point weight. 
        double       GetWeightPoint() const                { return _weight; }
  /// \ru Получить вес производной. \en Get derivative weight. 
        double       GetWeightDer()   const                { return _weightDer; }
  /// \ru Получить производную. \en Get derivative. 
  const Vector &     GetDerivative()  const                { return _deriv; }
  /// \ru Определен ли констрейн для производной. \en Whether derivative constraint defined. 
        bool         IsDerivativeConstraintDefined() const { return ::fabs( _weightDer ) > EXTENT_EPSILON; }
  /// \ru Зафиксирована ли точка. \en Whether point fixed. 
        bool         IsPointFixed() const                  { return _weight < 0; }
  /// \ru Зафиксирована ли производная. \en Whether derivative fixed. 
        bool         IsDerivativeFixed() const             { return _weightDer < 0; }
  /// \ru Установить вес точки. \en Set point weight. 
        void SetWeightPoint( double wt ) { _weight = wt; }
  /// \ru Установить вес производной. \en Set derivative weight.
        void SetWeightDerivative( double wt, const Vector & der ) { _weightDer = wt; _deriv = der; }
}; // MbApproxWeightConstraint


//------------------------------------------------------------------------------
/** \brief \ru Метод сглаживания NURBS кривой.
           \en NURBS smoothing method. \~
  \details \ru Метод сглаживания NURBS кривой.
           \en NURBS smoothing method. \~
  \ingroup Data_Structures
*/
// ---
enum MbeSmoothingMethod
{
  sm_Curvature,        ///< \ru Минимизация кривизны. \en Curvature minimization. 
  sm_CurvatureVariance ///< \ru Минимизация вариации кривизны. \en Curvature variance minimization. 
};


//-------------------------------------------------------------------------------
/** \brief \ru Параметры аппроксимации полилинии NURBS кривой.
           \en Parameters for the approximation polyline by a NURBS curve. \~
  \details \ru Параметры аппроксимации полилинии NURBS кривой.
           \en Parameters for the approximation polyline by a NURBS curve. \~
  \ingroup Data_Structures
*/
// ---
template<class Point, class Vector, class Nurbs>
class MbApproxNurbsParameters
{
private:
  bool                                          _bClosed;        ///< \ru Флаг замкнутости. \en Closedness.
  size_t                                        _order;          ///< \ru Порядок NURBS кривой. \en Order of NURBS curve.
  MbeSmoothingMethod                            _methodSmoothing;///< \ru Метод сглаживания. \en Smoothing method.
  double                                        _coefSmoothing;  ///< \ru Коэффициент сглаживания. \en Smoothing coefficient.
  double                                        _tolerance;      ///< \ru Желаемая точность аппроксимации. \en Desired approximation accuracy.
  c3d::DoubleVector                             _approxKnots;    ///< \ru Узловой вектор. \en Knot vector.
  std::vector<Point>                            _approxPoints;   ///< \ru Аппроксимируемая полилиния. \en Polyline to be approximated.
  c3d::DoubleVector                             _approxParams;   ///< \ru Параметры точек полилинии. \en Points parameters.
  std::vector<MbApproxWeightConstraint<Vector>> _approxWeights;  ///< \ru Веса точек полилинии. \en Points weights.
  const Nurbs *                                 _pReference;     ///< \ru Кривая для сравнения с результатом аппроксимации. \en Curve for result comparing.
  VERSION                                       _version;        ///< \ru Версия операции. \en The operation version.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbApproxNurbsParameters( VERSION version = Math::DefaultMathVersion() )
    : _bClosed        ( false                )
    , _order          ( c3d::NURBS_DEGREE    )
    , _methodSmoothing( sm_CurvatureVariance )
    , _coefSmoothing  ( -1.                  )
    , _tolerance      ( c3d::DELTA_MIN       )
    , _pReference     ( nullptr              )
    , _version        ( version              )
  {}

  /// \ru Конструктор копирования. \en The copy constructor. 
  MbApproxNurbsParameters( const MbApproxNurbsParameters & other ) { Init( other ); }

  /// \ru Деструктор. \en Destructor. 
  ~MbApproxNurbsParameters() {}

  /// \ru Инициализировать по другим параметрам. \en Initialize by another parameters. 
  void        Init( const MbApproxNurbsParameters & other )
  {
    _bClosed = other._bClosed;
    _order = other._order;
    _methodSmoothing = other._methodSmoothing;
    _coefSmoothing = other._coefSmoothing;
    _tolerance = other._tolerance;
    _approxKnots = other._approxKnots;
    _approxPoints = other._approxPoints;
    _approxParams = other._approxParams;
    _approxWeights = other._approxWeights;
    _pReference = other._pReference;
    _version = other._version;
  }

  /** \brief \ru Инициализировать по точкам, создать равномерный узловой вектор.
             \en Initialize by points. Create uniform knot vector. \~
    \details \ru Инициализировать по точкам, создать равномерный узловой вектор по заданному пользователем количеству сегментов.
                 Параметры аппроксимируемых точек будут рассчитаны по длине полилинии. Начальное и конечное значение узлов будут равны
                 первому и последнему параметру точек, либо расширены на указанное значение.
                 Веса всех точек будут единичные. Констрейны для точек и производных отсутствуют.
                 Кривая будет незамкнута.
             \en Initialize by points. Create uniform knot vector with user defined number of spans.
                 Points parameters will be calculated by chord length method. 
                 First and last knots will be equal to first and last paramemters values or will be extended by specified value. All point weights are unit.
                 There are no constraints for points and derivatives. The curve will be unclosed. \~
    \param[in] aPt - \ru Точки для аппроксимации.
                     \en Points for approximation. \~
    \param[in] order - \ru Порядок кривой.
                       \en Curve order. \~
    \param[in] nSpan - \ru Количество сегментов в узловом векторе.
                       \en Number of spans. \~
    \param[in] smooth - \ru Коэффициент сглаживания.
                        \en Smoothing coefficient. \~
    \param[in] orderSmoothing - \ru Порядок производных для сглаживания.
                                \en Derivative order for smoothing. \~
    \param[in] tolerance - \ru Точность аппроксимации.
                           \en Approximation tolerance. \~
    \param[in] bClosed - \ru Флаг замкнутости.
                         \en Closure flag. \~
    \param[in] extendBeg - \ru Величина расширения параметрической длины в начале кривой в процентах относительно
                               разности начального и конечного параметра точек (опционально).
                           \en Parametric length extention value for the curve beginning as percent w.r.t. difference
                               between last and first point parameter (optional). \~
    \param[in] extendEnd - \ru Величина расширения параметрической длины в конце кривой в процентах относительно
                               разности начального и конечного параметра точек (опционально).
                           \en Parametric length extention value for the curve end as percent w.r.t. difference
                                between last and first point parameter (optional). \~
  */
  void        InitUniform( const std::vector<Point> & aPt,
                                 size_t               order,
                                 size_t               nSpan,
                                 double               smooth,
                                 MbeSmoothingMethod   typeSmoothing,
                                 double               tolerance,
                                 bool                 bClosed,
                                 double               extendBeg = 0.,
                                 double               extendEnd = 0. )
  {
    InitPoints( aPt );
    if ( _approxPoints.empty() )
      return;

    _bClosed = bClosed;
    _order = order;
    _methodSmoothing = typeSmoothing;
    _methodSmoothing = order > 3 ? typeSmoothing : MbeSmoothingMethod::sm_Curvature;
    _coefSmoothing = smooth;
    _tolerance = tolerance;
    if ( bClosed )
    {
      if ( aPt.back().DistanceToPoint( aPt.front() ) > PARAM_EPSILON )
        _approxPoints.push_back( aPt.front() );
    }

    ParameterizeByLength( _approxPoints, _approxParams );
    GetUniformKnots( bClosed ? 0. : -extendBeg, bClosed ? 1. : 1. + extendEnd, nSpan, order, bClosed, _approxKnots );
    _approxWeights.resize( _approxPoints.size() );
  }

  /** \brief \ru Инициализировать по точкам и параметрам, создать равномерный узловой вектор.
             \en Initialize by points and parameters. Create uniform knot vector. \~
    \details \ru Инициализировать по точкам и параметрам, создать равномерный узловой вектор по заданному пользователем количеству сегментов.
                 Начальное и конечное значение узлов будут равны первому и последнему параметру точек, либо расширены на указанное значение.
                 Веса всех точек будут единичные. Констрейны для точек и производных отсутствуют.
                 Кривая будет незамкнута.
             \en Initialize by points and parameters. Create uniform knot vector with user defined number of spans.
                 First and last knots will be equal to first and last paramemters values or will be extended by specified value. All point weights are unit.
                 There are no constraints for points and derivatives. The curve will be unclosed. \~
    \param[in] aPt - \ru Точки для аппроксимации.
                     \en Points for approximation. \~
    \param[in] aPrm - \ru Параметры точек.
                      \en Point parameters. \~
    \param[in] order - \ru Порядок кривой.
                       \en Curve order. \~
    \param[in] nSpan - \ru Количество сегментов в узловом векторе.
                       \en Number of spans. \~
    \param[in] smooth - \ru Коэффициент сглаживания.
                        \en Smoothing coefficient. \~
    \param[in] orderSmoothing - \ru Порядок производных для сглаживания.
                                \en Derivative order for smoothing. \~
    \param[in] tolerance - \ru Точность аппроксимации.
                           \en Approximation tolerance. \~
    \param[in] bClosed - \ru Флаг замкнутости.
                         \en Closure flag. \~
    \param[in] extendBeg - \ru Величина расширения параметрической длины в начале кривой в процентах относительно
                               разности начального и конечного параметра точек (опционально).
                           \en Parametric length extention value for the curve beginning as percent w.r.t. difference
                               between last and first point parameter (optional). \~
    \param[in] extendEnd - \ru Величина расширения параметрической длины в конце кривой в процентах относительно
                               разности начального и конечного параметра точек (опционально).
                           \en Parametric length extention value for the curve end as percent w.r.t. difference
                                between last and first point parameter (optional). \~
  */
  void        InitUniformByPointsAndParams( const std::vector<Point> &  aPt,
                                            const std::vector<double> & aPrm,
                                                  size_t                order,
                                                  size_t                nSpan,
                                                  double                smooth,
                                                  MbeSmoothingMethod    typeSmoothing,
                                                  double                tolerance,
                                                  bool                  bClosed,
                                                  double                extendBeg = 0.,
                                                  double                extendEnd = 0. )
  {
    if ( aPt.empty() )
      return;

    _approxPoints = aPt;
    _approxParams = aPrm;
    _bClosed = bClosed;
    _order = order;
    _methodSmoothing = typeSmoothing;
    _methodSmoothing = order > 3 ? typeSmoothing : MbeSmoothingMethod::sm_Curvature;
    _coefSmoothing = smooth;
    _tolerance = tolerance;
    GetUniformKnots( bClosed ? 0. : -extendBeg, bClosed ? 1. : 1. + extendEnd, nSpan, order, bClosed, _approxKnots );
    _approxWeights.resize( _approxPoints.size() );
  }

  /** \brief \ru Инициализировать по точкам.
             \en Initialize by points. \~
    \details \ru Инициализировать по точкам. Параметры аппроксимируемых точек будут рассчитаны по длине полилинии.
                 Узловой вектор будет пустой, он должен быть подобран в алгоритме аппроксимации.
                 Веса всех точек будут единичные. Констрейны для точек и производных отсутствуют.
                 Порядок кривой, коэффициент сглаживания, порядок производной для сглаживания задаются пользователем.
                 Кривая будет незамкнута.
             \en Initialize by points. Knot vector will be empty and should be defined in the approximation algorithm.
                 Points parameters will be calculated by chord length method. 
                 All point weights are unit.
                 There are no constraints for points and derivatives. The curve will be unclosed. \~
    \param[in] aPt - \ru Точки для аппроксимации.
                     \en Points for approximation. \~
    \param[in] order - \ru Порядок кривой.
                       \en Curve order. \~
    \param[in] smooth - \ru Коэффициент сглаживания.
                        \en Smoothing coefficient. \~
    \param[in] orderSmoothing - \ru Порядок производных для сглаживания.
                                \en Derivative order for smoothing. \~
    \param[in] tolerance - \ru Точность аппроксимации.
                            \en Approximation tolerance. \~
  */
  void        InitPoints( const std::vector<Point> & aPt,
                                size_t               order,
                                double               smooth,
                                MbeSmoothingMethod   typeSmoothing,
                                double               tolerance )
  {
    InitPoints( aPt );
    if ( _approxPoints.empty() )
      return;

    _bClosed = false;
    _order = order;
    _methodSmoothing = typeSmoothing;
    _coefSmoothing = smooth;
    _tolerance = tolerance;

    ParameterizeByLength( aPt, _approxParams );
    _approxWeights.resize( aPt.size() );
  }
 
  /** \brief \ru Инициализировать по точкам с заданием констрейнов на концах.
             \en Initialize by points with end constraints. \~
    \details \ru Инициализировать по точкам.
                 Узловой вектор и коэффициент сглаживания должны быть подобраны в алгоритме аппроксимации.
                 Констрейны для внутренних точек и производных отсутствуют. Порядок кривой задается пользователем.
                 Кривая будет незамкнута.
             \en Initialize by points. Knot vector and smoothing coefficient should be defined in the approximation algorithm.
                 There are no constraints for inner points and derivatives. The curve will be unclosed. \~
    \deprecated \ru Метод устарел. \en The method is deprecated. \~
    \param[in] aPt - \ru Точки для аппроксимации.
                     \en Points for approximation. \~
    \param[in] order - \ru Порядок кривой.
                       \en Curve order. \~
    \param[in] tolerance - \ru Точность аппроксимации.
                           \en Approximation tolerance. \~
    \param[in] bFixBeginEnd - \ru Флаги закрепления начала и конца кривой на первой и последней аппроксимационной точке.
                              \en Whether curve should coincide with first and last point. \~
    \param[in] bClosed - \ru Флаг замкнутости.
                         \en Closure flag. \~
    \param[in] pDerBeg - \ru Констрейн на производную в начале кривой (если нулевой указатель - значит констрейн отсутствует).
                         \en Curve beginnig derivative constraint (optional). \~
    \param[in] pDerEnd - \ru Констрейн на производную в конце кривой (если нулевой указатель - значит констрейн отсутствует).
                         \en Curve end derivative constraint (optional). \~
  */
  void        InitSimpleConstraints( const std::vector<Point> & aPt,
                                           size_t               order,
                                           double               tolerance,
                                           c3d::BoolPair        bFixBeginEnd,
                                           bool                 bClosed,
                                           Vector *             pDerBeg = nullptr,
                                           Vector *             pDerEnd = nullptr )
  {
    InitPoints( aPt );
    if ( _approxPoints.empty() )
      return;

    _bClosed = bClosed;
    _order = order;
    _methodSmoothing = order > 3 ? MbeSmoothingMethod::sm_CurvatureVariance : MbeSmoothingMethod::sm_Curvature;
    _coefSmoothing = 0.;
    _tolerance = tolerance;
    
    if ( bClosed )
    {
      if ( aPt.back().DistanceToPoint( aPt.front() ) > PARAM_EPSILON )
        _approxPoints.push_back( aPt.front() );
    }

    ParameterizeByLength( _approxPoints, _approxParams );
    _approxWeights.resize( _approxPoints.size() );

    if ( bClosed )
    {
      if ( bFixBeginEnd.first || bFixBeginEnd.second )
        _approxWeights[0].SetWeightPoint( -1. );

      if ( pDerBeg != nullptr || pDerEnd != nullptr )
        _approxWeights[0].SetWeightDerivative( -1., pDerBeg != nullptr ? *pDerBeg : *pDerEnd );
    }
    else
    {
      if ( bFixBeginEnd.first )
        _approxWeights[0].SetWeightPoint( -1. );

      if ( bFixBeginEnd.second )
        _approxWeights.back().SetWeightPoint( -1. );

      if ( pDerBeg != nullptr )
        _approxWeights[0].SetWeightDerivative( -1., *pDerBeg );

      if ( pDerEnd != nullptr )
        _approxWeights.back().SetWeightDerivative( -1., *pDerEnd );
    }
  }


  /** \brief \ru Инициализировать по точкам с заданием констрейнов для точек и производных.
             \en Initialize by points with points and derivatives constraints. \~
  \details \ru Инициализировать с заданием констрейнов для точек и производных.
               Узловой вектор и коэффициент сглаживания должны быть подобраны в алгоритме аппроксимации.
           \en Initialize by points with points and derivatives constraints. Knot vector and smoothing coefficient should be defined in the approximation algorithm. \~
  \param[in] aPt - \ru Точки для аппроксимации.
                   \en Points for approximation. \~
  \param[in] order - \ru Порядок кривой.
                     \en Curve order. \~
  \param[in] tolerance - \ru Точность аппроксимации.
                         \en Approximation tolerance. \~
  \param[in] bClosed - \ru Флаг замкнутости.
                       \en Closure flag. \~
  \param[in] constraints - \ru Массив констрейнов на точки и производные. First - индекс точки для фиксации. Second - вектор для фиксации производной,
                               если nullptr - констрейн на производную отсутствует.
                           \en Array of constraints for points and derivatives. First - index of fixed point. Second - derivative vector constraint,
                               if nullptr - derivative is unconstrained for this point. \~
*/
  void        InitConstraints( const std::vector<Point> &                             aPt,
                                     size_t                                           order,
                                     double                                           tolerance,
                                     bool                                             bClosed,
                               const std::vector<std::pair<size_t, const Vector *>> & constraints )
  {
    _bClosed = bClosed;
    _order = order;
    _methodSmoothing = MbeSmoothingMethod::sm_CurvatureVariance;
    _coefSmoothing = 0.;
    _tolerance = tolerance;
    _approxPoints = aPt;

    if ( bClosed )
    {
      if ( aPt.back().DistanceToPoint( aPt.front() ) > PARAM_EPSILON )
        _approxPoints.push_back( aPt.front() );
    }

    ParameterizeByLength( _approxPoints, _approxParams );

    _approxWeights.resize( _approxPoints.size() );
    for ( const auto & constr : constraints )
    {
      const auto id = constr.first;
      _approxWeights[id].SetWeightPoint( -1. );
      if ( constr.second != nullptr )
        _approxWeights[id].SetWeightDerivative( -1., *constr.second );
    }
  }

public:
  /// \ru Должна ли кривая быть замкнута. \en Whether curve closed. 
        bool                                            IsClosed() const { return _bClosed; }
  /// \ru Получить порядок NURBS кривой. \en Get NURBS order. 
        size_t                                          GetOrder() const { return _order; }
  /// \ru Получить способ сглаживания. \en Get smoothing method. 
        MbeSmoothingMethod                              GetMethodOrderSmoothing() const { return _methodSmoothing; }
  /// \ru Получить коэффициент сглаживания. \en Get smoothing coefficient. 
        double                                          GetSmoothCoefficient() const { return _coefSmoothing; }
  /// \ru Получить точность аппроксимации. \en Get approximation tolerance. 
        double                                          GetTolerance() const { return _tolerance; }
  /// \ru Получить узловой вектор. \en Get knots. 
  const c3d::DoubleVector &                             GetKnots() const { return _approxKnots; }
  /// \ru Получить точки. \en Get points. 
  const std::vector<Point> &                            GetPoints() const { return _approxPoints; }
  /// \ru Получить параметры точек. \en Get point's parameters. 
  const c3d::DoubleVector &                             GetPointsParameters()       const { return _approxParams; }
  /// \ru Получить веса точек. \en Get point's weights. 
  const std::vector<MbApproxWeightConstraint<Vector>> & GetPointsWeights() const { return _approxWeights; }
  ///< \ru Получить кривую для сравнения. \en Get curve for result comparing.
  const Nurbs * GetReferenceCurve() const { return _pReference; }
  /// \ru Определен ли коэффициент сглаживания. \en Whether smoothing coefficient defined. 
  bool  IsSmoothDefined() const { return ::fabs( _coefSmoothing ) > EXTENT_EPSILON; }
  /// \ru Установить вес точки с заданным индексом. \en Set weight for point with specified index. 
  void SetPointWeight( size_t idx, double weight ) { _approxWeights[idx].SetWeightPoint( weight ); }
  /// \ru Установить вес производной и саму производную для точки с заданным индексом. \en Set derivative weight and derivative itself for point with specified index. 
  void SetWeightDerivative( size_t idx, double weight, const Vector & der ) { _approxWeights[idx].SetWeightDerivative( weight, der ); }
  ///< \ru Установить кривую для сравнения. \en Set curve for result comparing.
  void SetReferenceCurve( const Nurbs * pCurve ) { _pReference = pCurve; }
  /// \ru Установить коэффициент сглаживания. \en Set smoothing coefficient. 
  void SetSmoothCoefficient( double smooth ) { _coefSmoothing = smooth; }
  /// \ru Установить узловой вектор. \en Set knot vector. 
  void SetKnots( const c3d::DoubleVector & knots ) { _approxKnots = knots; }
  ///< \ru Получить версию математики. \en Get math version.
  VERSION GetVersion() const { return _version; }

  private:
    /** \brief \ru Создать равномерный узловой вектор.
      \details \ru Создать равномерный узловой вектор по заданному пользователем домену и количеству сегментов.
               \en Create uniform knot vector. \~
      \param[in] beg - \ru Значение первого узла.
                       \en First knot value. \~
      \param[in] end - \ru Значение последнего узла.
                       \en Last knot value. \~
      \param[in] nSpan - \ru Количество сегментов.
                         \en Number of segments. \~
      \param[in] order - \ru Порядок кривой.
                         \en NURBS order. \~
      \param[in] bClosed - \ru Признак замкнутости кривой.
                           \en Closure flag. \~
      \param[out] aKt - \ru Равномерный узловой вектор.
                        \en Uniform knot vector. \~
    */
    void GetUniformKnots( double beg, double end, size_t nSpan, size_t order, bool bClosed, c3d::DoubleVector & aKt )
    {
      aKt.clear();

      if ( bClosed )
      {
        const size_t count = order * 2 + nSpan - 1;
        const double step = ( end - beg ) / nSpan;
        aKt.resize( count );
        const double val0 = beg - step * ( order - 1 );
        for ( size_t i = 0; i < count; ++i )
          aKt[i] = val0 + step * i;
      }
      else
      {
        for ( size_t i = 0; i < order; ++i )
          aKt.push_back( beg );

        if ( nSpan > 1 )
        {
          const double step = ( end - beg ) / nSpan;
          for ( size_t i = 1; i < nSpan; ++i )
            aKt.push_back( beg + step * i );
        }

        for ( size_t i = 0; i < order; ++i )
          aKt.push_back( end );
      }
    }
    /// \ru Выбросить дублирующиеся точки из исходных данных. \en Remove duplicated points from the initial data. 
    void InitPoints( const std::vector<Point> & aPt )
    {
      _approxPoints.clear();
      if ( !aPt.empty() )
      {
        _approxPoints.reserve( aPt.size() );
        _approxPoints.push_back( aPt.front() );
        for ( size_t i = 1, n = aPt.size(); i < n; ++i )
        {
          const auto & pnt = aPt[i];
          if ( pnt.DistanceToPoint( _approxPoints.back() ) > PARAM_EPSILON )
               _approxPoints.push_back( pnt );
        }
      }
    }
}; // MbApproxNurbsParameters


//-------------------------------------------------------------------------------
/** \brief \ru Сведения об ошибке аппроксимации.
           \en Approximation error information. \~
  \details \ru Сведения об ошибке аппроксимации.
           \en Approximation error information. \~
  \ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbApproxError
{
public:
  double         _errMax;      ///< \ru Максимальная ошибка. \en Maximal error. 
  double         _errMean;     ///< \ru Средняя ошибка. \en Mean error. 
  MbCartPoint3D  _ptMax;       ///  \ru Точка, для которой ошибка максимальна. \en Point with maximal error.
  MbCartPoint3D  _ptMaxProj;   ///< \ru Проекция этой точки. \en Projection of this point. 

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbApproxError()
    : _errMax   ( MB_MAXDOUBLE )
    , _errMean  ( MB_MAXDOUBLE )
    , _ptMax    (              )
    , _ptMaxProj(              )
  {}

  /// \ru Конструктор копирования. \en The copy constructor. 
  MbApproxError( const MbApproxError & that )
    : _errMax   ( that._errMax    )
    , _errMean  ( that._errMean   )
    , _ptMax    ( that._ptMax     )
    , _ptMaxProj( that._ptMaxProj )
  {
  }

  /// \ru Оператор присваивания. \en The assignment operator. 
  MbApproxError & operator = ( const MbApproxError & that )
  {
    _errMax = that._errMax;
    _errMean = that._errMean;
    _ptMax = that._ptMax;
    _ptMaxProj = that._ptMaxProj;
    return ( *this );
  }

public:
  /// \ru Инициализировать по ошибке и 3D точкам. \en Initialize by error and 3D points.
  void Init( double errMax, double errMean, const MbCartPoint3D & ptMax, const MbCartPoint3D & ptMaxProj )
  {
    _errMax = errMax;
    _errMean = errMean;
    _ptMax = ptMax;
    _ptMaxProj = ptMaxProj;
  }

  /// \ru Инициализировать по ошибке и 2D точкам. \en Initialize by error and 2D points.
  void Init( double errMax, double errMean, const MbCartPoint & ptMax, const MbCartPoint & ptMaxProj )
  {
    _errMax = errMax;
    _errMean = errMean;
    _ptMax = MbCartPoint3D( ptMax.x, ptMax.y, 0. );
    _ptMaxProj = MbCartPoint3D( ptMaxProj.x, ptMaxProj.y, 0. );
  }

  /// \ru Инициализировать по другому инстансу. \en Initialize by another instance.
  void Init( const MbApproxError & that )
  {
    _errMax = that._errMax;
    _errMean = that._errMean;
    _ptMax = that._ptMax;
    _ptMaxProj = that._ptMaxProj;
  }

  /// \ru Сбросить данные к значениям по умолчанию. \en Reset to default values. 
  void    Reset()
  {
    _errMax = MB_MAXDOUBLE;
    _errMean = MB_MAXDOUBLE;
    _ptMax = MbCartPoint3D();
    _ptMaxProj = MbCartPoint3D();
  }

  /// \ru Получить максимальную ошибку. \en Get maximal error. 
       double           GetMaxError() const { return _errMax; }
  /// \ru Получить среднюю ошибку. \en Get mean error. 
       double           GetMeanError()   const { return _errMean; }
  /// \ru Получить точку с максимальной ошибкой. \en Get point with maximal error. 
  const MbCartPoint3D & GetPointMaxError() const { return _ptMax; }
  /// \ru Получить проекцию на кривую точки с максимальной ошибкой. \en Get point with maximal error projection. 
  const MbCartPoint3D & GetPointProjectionMaxError() const { return _ptMaxProj; }

}; // MbApproxError


//------------------------------------------------------------------------------
/** \brief \ru Выходные данные для аппроксимации полилинии NURBS кривой.
           \en Output data for the approximation polyline by a NURBS curve. \~
   \details \ru Класс содержит достигнутую ошибку аппроксимации и NURBS кривую. \n
           \en The output data contains Reached approximation tolerance and NURBS curve. \n \~
   \ingroup Shell_Building_Parameters
*/
// ---
template< class Nurbs >
struct MbApproxNurbsCurveResult
{
public:
  MbApproxError _error; ///< \ru Достигнутая точность аппроксимации. \en Reached approximation tolerance. 
  SPtr<Nurbs>   _pNurbs;///< \ru Результирующая кривая. \en Resulting curve. 

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbApproxNurbsCurveResult() : _error(), _pNurbs() {}

  /// \ru Конструктор копирования. \en Copy constructor. \~
  MbApproxNurbsCurveResult( const MbApproxNurbsCurveResult & other ) : _error( other._error ), _pNurbs( other._pNurbs ) {}

  /// \ru Инициализировать по ошибке и кривой. \en Initialize by error and curve.
  void Init( const MbApproxError & err, const SPtr<Nurbs> & pNurbs )
  {
    _error = err;
    _pNurbs = pNurbs;
  }

  /// \ru Получить ошибку. \en Get error. 
  const MbApproxError & GetError() const { return _error; }

  /// \ru Получить ошибку. \en Get error. 
  MbApproxError & GetError() { return _error; }

  /// \ru Получить NURBS кривую. \en Get Nurbs curve. 
  const SPtr<Nurbs> & GetNurbsCurve() const { return _pNurbs; }

  /// \ru Получить NURBS кривую. \en Get Nurbs curve. 
  SPtr<Nurbs> & GetNurbsCurve() { return _pNurbs; }

  /// \ru Оператор копирования. \en Copy operator. \~
  void operator = ( const MbApproxNurbsCurveResult & other )
  {
    _error = other._error;
    _pNurbs = other._pNurbs;
  }
};


//-------------------------------------------------------------------------------
/** \brief \ru Аппроксимировать 3D точки NURBS кривой.
           \en Approximate 3D points by NURBS curve. \~
  \details \ru Аппроксимировать точки NURBS кривой.
           \en IApproximate points by NURBS curve. \~
  \param[in] param - \ru Параметры аппроксимации.
                     \en Parameters for approximation. \~
  \param[out] result - \ru Результат операции.
                       \en Operation result. \~
  \return \ru Код ошибки. \en Error code. \~
  \ingroup Curve3D_Modeling
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
MATH_FUNC( MbResultType ) ApproximatePolylineByNurbs( const MbApproxNurbsParameters<MbCartPoint3D, MbVector3D, MbNurbs3D> & param,
                                                            MbApproxNurbsCurveResult<MbNurbs3D> &                           result );


//-------------------------------------------------------------------------------
/** \brief \ru Аппроксимировать 2D точки NURBS кривой.
           \en Approximate 2D points by NURBS curve. \~
  \details \ru Аппроксимировать точки NURBS кривой.
           \en Approximate points by NURBS curve. \~
  \param[in] param - \ru Параметры аппроксимации.
                     \en Parameters for approximation. \~
  \param[out] result - \ru Результат операции.
                       \en Operation result. \~
  \return \ru Код ошибки. \en Error code. \~
  \ingroup Curve_Modeling
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
MATH_FUNC( MbResultType ) ApproximatePolylineByNurbs( const MbApproxNurbsParameters<MbCartPoint, MbVector, MbNurbs> & param,
                                                            MbApproxNurbsCurveResult<MbNurbs> &                       result );

#endif // __MB_SMOOTH_NURBS_FIT_CURVE_H
