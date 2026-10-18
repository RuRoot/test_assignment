////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Репараметризованная кривая в трехмерном пространстве.
         \en Reparametrized curve in three-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_REPARAM_CURVE3D_H
#define __CUR_REPARAM_CURVE3D_H


#include <curve3d.h>

class MbFunction;

//------------------------------------------------------------------------------
/** \brief \ru Репараметризованная кривая в трехмерном пространстве.
           \en Reparametrized curve in three-dimensional space. \~
  \details \ru Репараметризованная кривая служит для согласования областей определения кривых. \n
    Геометрически репараметризованная кривая полностью совпадает с базовой кривой basisCurve.
    Репараметризованная кривая имеет другую область определения и, как следствие, другую длину своих производных.
    Параметры базовой кривой и репараметризованной кривой связаны равенствами: \n
    Для линейной репараметризации b * (t - tmin) = t_basisCurve - tmin_basisCurve, где t - параметр репараметризованной кривой.
    Для квадратичной репараметризации t_basisCurve = a * t * t + b * t + c,  где t - параметр репараметризованной кривой.
    При квадратичной репараметризации можно выбирать не только новый диапазон параметров, но и величину первой производной в начале кривой.
    Базовой кривой для репараметризованной кривой не может служить другая репараметризованная кривая.
    В подобной ситуации выполняется переход к первичной базовой кривой.
           \en Reparametrized curve is used for matching domains of curves. \n
    Geometrically reparametrized curve completely coincides with base curve 'basisCurve'.
    Reparametrized curve has another definition domain and as a result another length of its derivatives.
    Parameters of base curve and reparametrized curve are related by the equality: \n
    For linear reparametrization b * (t - tmin) = t_basisCurve - tmin_basisCurve, where 't' - parameter of reparametrized curve.
    For quadratic reparametrization t_basisCurve = a * t * t + b * t + c, where 't' - parameter of reparametrized curve.
    When the quadratic reparametrization is used it is possible to define the first derivative value at the beginning of the curve.
    Another reparametrized curve can't be the base curve for a reparametrized curve.
    In this situation it changes to the initial base curve. \~
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbReparamCurve3D : public MbCurve3D {
public:
  enum MbeReparamType {
    rt_Linear,        ///< \ru Линейная репараметризация. \en Linear reparametrization.
    rt_Quadratic,     ///< \ru Квадратичная репараметризация. \en Quadratic reparametrization. 
    rt_ScaledEndDers, ///< \ru Репараметризация с заданными масштабами производной на концах. \en Reparametrization with a given scale of the derivative at the ends.
    rt_Proportional,  ///< \ru Пропорциональная длине кривой репараметризация. \en The poportional to the length of the curve parametrization.
    rt_User           ///< \ru Функция, заданная пользователем. \en User defined function.
  };

protected:
  MbCurve3D *      basisCurve;  ///< \ru Базовая кривая. \en The base curve.
  MbeReparamType   reparamType; ///< \ru Способ репараметризации. \en Way of repatametrization.
  SPtr<MbFunction> repFunction; ///< \ru Репараметризующая функция вычисления параметра базовой кривой. \en Reparametric function for calculation the base curve parameter.
public :
  /// \ru Конструктор по кривой и новым параметрам. \en Constructor by curve and by new parametric limits.
  MbReparamCurve3D( const MbCurve3D &, double t1, double t2 );
  /// \ru Конструктор по кривой и новым параметрам. \en Constructor by curve and by new parametric limits.
  MbReparamCurve3D( const MbCurve3D &, double t1, double t2, double begFirstDerValue );
  /// \ru Конструктор по кривой и производным параметра на ее концах. \en Constructor for the curve and derivatives of the parameter at its ends
  MbReparamCurve3D( double dt1, double dt2, const MbCurve3D & curve );
protected:
  MbReparamCurve3D( const MbReparamCurve3D &, MbRegDuplicate * );
private :
  MbReparamCurve3D( const MbReparamCurve3D & ); // \ru Не реализовано. \en Not implemented. 
public :
  virtual ~MbReparamCurve3D();

public :
  VISITING_CLASS( MbReparamCurve3D );
  
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
  static  MbReparamCurve3D *  CreateProportional( const MbCurve3D & curve, double t1, double t2 );
  /// \ru Установить пользовательскую функцию репараметризации. \en Set users reparameterization function.
  static  MbReparamCurve3D *  CreateByFunction( const MbCurve3D & curve, MbFunction & repFunc, MbeReparamType tp = rt_User );

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeSpaceType  IsA()              const override; // \ru Тип элемента \en Type of element
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  bool        IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Является ли объект копией \en Whether the object is a copy 
  bool        SetEqual ( const MbSpaceItem & ) override;       // \ru Сделать равным \en Make equal 
  bool        IsSimilar( const MbSpaceItem & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis
  double      DistanceToPoint( const MbCartPoint3D & ) const override; // \ru Расстояние до точки \en Distance to a point 
  void        PrepareIntegralData( const bool forced ) const override; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.

  bool        IsSpaceSame( const MbSpaceItem & item, double eps = METRIC_REGION ) const override; // \ru Являются ли объекты идентичными в пространстве \en Whether the objects are equal in space 

  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisItems ( RPArray<MbSpaceItem> & ) override;  // \ru Дать базовые объекты \en Get the basis objects 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  // \ru Общие функции кривой \en Common functions of curve 

  double      GetTMin()  const override;
  double      GetTMax()  const override;
  bool        IsClosed() const override; // \ru Проверка замкнутости кривой \en Check for curve closedness 
  double      GetPeriod() const override; // \ru Вернуть период \en Get period  
  // \ru Функции кривой для работы в области определения параметрической кривой \en Functions of curve for working at parametric curve domain 
  void        PointOn  ( double & t, MbCartPoint3D & ) const override; // \ru Точка на кривой \en Point on the curve 
  void        FirstDer ( double & t, MbVector3D & ) const override;  // \ru Первая производная \en The first derivative 
  void        SecondDer( double & t, MbVector3D & ) const override;  // \ru Вторая производная \en The second derivative 
  void        ThirdDer ( double & t, MbVector3D & ) const override;  // \ru Третья производная по t \en The third derivative with respect to t 
  void        Normal   ( double & t, MbVector3D & ) const override;// \ru Вектор главной нормали \en Vector of the principal normal
  // \ru Функции кривой для работы вне области определения параметрической кривой \en Functions of curve for working outside the domain of parametric curve 
  void       _PointOn  ( double t,  MbCartPoint3D & ) const override; // \ru Точка на расширенной кривой \en Point on the extended curve
  void       _FirstDer ( double t,  MbVector3D & ) const override;    // \ru Первая производная \en The first derivative
  void       _SecondDer( double t,  MbVector3D & ) const override;    // \ru Вторая производная \en The second derivative
  void       _ThirdDer ( double t,  MbVector3D & ) const override;    // \ru Третья производная по t \en The third derivative with respect to t
  void       _Normal   ( double t,  MbVector3D & ) const override;// \ru Вектор главной нормали \en Vector of the principal normal
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void        Explore( double & t, bool ext,
                       MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const override;

  MbNurbs3D * NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;
  MbCurve3D * NurbsCurve( const MbNurbsParameters & ) const override; // \ru Построить NURBS-копию кривой \en Create a NURBS-copy of the curve 

  const   MbFunction * GetFunction() const { return repFunction; } // \ru Получить функцию. \en Get function 
  const MbCurve3D & GetBasisCurve() const override;
         MbCurve3D & SetBasisCurve() override;

  // \ru Построить усеченную кривую c учетом заданной трехмерной точности. \en Construct a trimmed curve with the given three-dimensional accuracy.
  MbCurve3D * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyzEps, bool saveParamLenAndLaw ) const override;
  using MbCurve3D::Trimmed; // \ru Метод базового класса MbCurve3D без учета трехмерной точности. Точность по умолчанию - MbDimAccuracy::threeDimAcc.  \en Method of MbCurve3D base class without three-dimensional accuracy. The default accuracy is MbDimAccuracy::threeDimAcc.

  void        Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction

  double      GetMetricLength() const override; // \ru Метрическая длина кривой \en Metric length of a curve 
  double      CalculateMetricLength() const override; // \ru Метрическая длина кривой \en Metric length of a curve 
  double      CalculateLength( double t1, double t2 ) const override;
  bool        DistanceAlong( double & t, double len, int curveDir, double eps = Math::metricPrecision,
                                     VERSION version = Math::DefaultMathVersion() ) const override; 
  double      GetLengthEvaluation() const override;

  void        CalculateLocalGabarit( const MbMatrix3D &, MbCube & ) const override; // \ru Рассчитать габарит относительно л.с.к. \en Calculate bounding box relative to the local coordinate system 
  bool        IsDegenerate( double eps = METRIC_PRECISION ) const override; // \ru Проверка вырожденности кривой \en Check for curve degeneracy
  bool        IsSmoothConnected( double angleEps, bool ignoreLimits = false ) const override; // \ru Являются ли стыки контура\кривой гладкими? \en Whether the joints of a contour\curve are smooth. 
  double      Curvature    ( double  ) const override; // \ru Кривизна кривой \en Curvature of the curve 
                      
  double      Step         ( double t, double sag ) const override; // \ru Вычисление шага по стрелке прогиба. \en Calculation of parameter step by the sag.
  double      DeviationStep( double t, double angle ) const override; // \ru Вычисление шага по углу отклонения нормали. \en Calculation of parameter step by the deviation angle.   
  double      MetricStep   ( double t, double length ) const override; // \ru Вычисление шага параметра по длине. \en Calculation of parameter step by the given length.

  void        CalculatePolygon( const MbStepData & stepData, MbPolygon3D &polygon ) const override; // \ru рассчитать полигон \en Calculate a polygon 
  void        CalculateGabarit( MbCube & ) const override; // \ru Вычислить габарит кривой \en Calculate the bounding box of a curve 
  void        GetCentre( MbCartPoint3D & ) const override; // \ru Посчитать центр кривой \en Calculate the center of a curve 
  void        GetWeightCentre( MbCartPoint3D & ) const override; // \ru Посчитать центр тяжести кривой \en Calculate the gravity center of the curve 

  void        GetPointsByEvenLengthDelta( size_t n, std::vector<MbCartPoint3D> & pnts ) const override; // \ru Выдать n точек кривой с равными интервалами по длине дуги \en Get n points of curve equally spaced by the arc length

  // \ru Ближайшая проекция точки на кривую \en The nearest projection of a point onto the curve 
  bool        NearPointProjection( const MbCartPoint3D &, double & t, bool ext, MbRect1D * tRange = nullptr ) const override;
  // \ru Определение точек касания изоклины \en Determination of tangent points of isocline 
  void        GetIsoclinal( const MbVector3D & nor, SArray<double> & tIso ) const override;
  /// \ru Найти все особые точки функции кривизны кривой и вычислить значение потенциальной энергии кривой.
  /// \en Find all the special points of the curvature function of the curve and calculate potential energy of the curve. \~
  void        GetCurvatureSpecialPoints( std::vector<c3d::DoublePair> & points, double * energy = nullptr ) const override;
  /// \ ru Определение точек излома кривой. \en The determination of curve smoothness break points.
  void        BreakPoints( std::vector<double> & vBreaks, double precision = ANGLE_REGION ) const override;

  // \ru Прохождение кривой через точку \en Passing of curve through a point 
  //virtual bool        GoThroughPoint( double t, MbCartPoint3D & p0 );
  // \ru Касание кривой через точку с заданной производной \en Tangent of curve through point with the given derivative 
  //virtual bool        GoThroughPointWithDerive( double t, MbCartPoint3D & p0, MbVector3D & v0 );

  MbCurve *   GetMap( const MbMatrix3D &, MbRect1D * pRgn = nullptr,
                      VERSION version = Math::DefaultMathVersion(), bool * coincParams = nullptr ) const override; // \ru Дать плоскую проекцию кривой \en Get a planar projection of a curve

  size_t      GetCount()   const override;
  void        ChangeCarrier( const MbSpaceItem & item, MbSpaceItem & init ) override; // \ru Изменение носителя \en Change a carrier 
  bool        IsPlanar  ( double accuracy = METRIC_EPSILON ) const override; // \ru Является ли кривая плоской \en Whether the curve is planar 
  bool        IsStraight( bool ignoreParams = false ) const override; // \ru Является ли линия прямолинейной \en Whether the line is straight 
  double      GetRadius( double accuracy = METRIC_REGION ) const override; // \ru Дать физический радиус объекта или ноль, если это невозможно. \en Get the physical radius of the object or null if it impossible.
  bool        GetCircleAxis  ( MbAxis3D & ) const override; // \ru Дать ось кривой \en Get axis of curve 

  void        ParameterInto( double & ) const; // \ru Перевод параметра базовой кривой в локальный параметр \en Transformation of the base curve parameter to a local parameter 
  void        ParameterFrom( double & ) const; // \ru Перевод локального параметра в параметр базовой кривой \en Transformation of a local parameter to the base curve parameter 

  void        SetBasisCurve( MbCurve3D & );      // \ru Заменить плоскую кривую \en Replace the planar curve 
  double      Tmin() const; ///< \ru Начальный параметр. \en Start parameter. 
  double      Tmax() const; ///< \ru Конечный  параметр. \en End parameter. 
  double      Dt()   const; ///< \ru Производная параметра кривой basisCurve по параметру. \en Derivative of parameter of 'basisCurve' curve by parameter. 
  void        SetTmin( double t );
  void        SetTmax( double t );
  void        SetDt  ( double d );

  MbeReparamType GetReparamType() const { return reparamType; } // \ru Тип параметризации. \en Parameterization type.

  // \ru !!! геометрия подложки тождественна геометрии кривой, отлична параметризация !!! \en !!! geometry of substrate is identical to geometry of curve, parameterization is different !!! 
  const MbCurve3D & GetSubstrate() const override; // \ru Выдать подложку или себя \en Get substrate or itself
  MbCurve3D & SetSubstrate() override;       // \ru Выдать подложку или себя \en Get substrate or itself
  int         SubstrateCurveDirection() const override;    // \ru Направление подложки относительно кривой или наоборот \en Direction of substrate relative to the curve or vice versa 
  void        SubstrateToCurve( double & ) const override; // \ru Преобразовать параметр подложки в параметр кривой \en Transform a substrate parameter to the curve parameter 
  void        CurveToSubstrate( double & ) const override; // \ru Преобразовать параметр кривой в параметр подложки \en Transform a curve parameter to the substrate parameter 
  // \ru Заполнить плейсемент, ести кривая плоская \en Fill the placement if the curve is planar 
  bool        GetPlacement( MbPlacement3D & place, PlanarCheckParams params = PlanarCheckParams() ) const override;
  // \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская (после использования вызывать DeleteItem на двумерную кривую) \en Get the planar curve and placement if the spatial curve is planar (call DeleteItem for two-dimensional curve after using) 
  bool        GetPlaneCurve( MbCurve *& curve2d, MbPlacement3D & place, bool saveParams, PlanarCheckParams params = PlanarCheckParams() ) const override;
  // \ru Дать поверхностную кривую, если пространственная кривая поверхностная (после использования вызывать DeleteItem на аргументы) \en Get a surface curve if a spatial curve is on a surface (call DeleteItem for arguments after using ) 
  bool        GetSurfaceCurve( MbCurve *& curve2d, MbSurface *& surface, VERSION version = Math::DefaultMathVersion() ) const override;

  /// \ru Является ли объект смещением \en Whether the object is a shift 
  bool        IsShift( const MbSpaceItem &, MbVector3D &, bool & isSame, double accuracy = LENGTH_EPSILON ) const override;
  bool        IsSimilarToCurve( const MbCurve3D & curve, double precision = METRIC_PRECISION ) const override; // \ru Подобные ли кривые для объединения (слива) \en Whether the curves for union (joining) are similar 

  bool        IsContinuousDerivative( bool & contLength, bool & contDirect, c3d::DoubleVector * params = nullptr, double epsilon = EPSILON ) const override; // \ru Непрерывна ли первая производная? \en Have the first derivative the continuous?
  // \ru Устранить разрывы первых производных по длине. \en Eliminate the discontinuities of the first derivative at length.
  bool        SetContinuousDerivativeLength( VERSION version, double epsilon = EPSILON ) override; 

  bool        IsProportional() const { return (reparamType == rt_Proportional); } ///< \ru Является ли репараметризация пропорциональной? \en Is the re-parametrization proportional?
  bool        IsLinear() const { return (reparamType == rt_Linear); } // \ru Является ли репараметризация линейной? \en Is the re-parametrization linear?
  bool        SetLimitParam( double newTMin, double newTMax ); // \ru Установить область изменения параметра \en Set range of parameter  
  /// \ru Получить границы участков кривой, которые описываются одной аналитической функцией. 
  /// \en Get the boundaries of the curve sections that are described by one analytical function. \~
  void        GetAnalyticalFunctionsBounds( std::vector<double> & params ) const override;

private:
  void        operator = ( const MbReparamCurve3D & ); // \ru Не реализовано. \en Not implemented. 
  // \ru Параметр базовой кривой и его производные. \en The base curve parameter and its derivatives.
  void        Explore( double & t, bool ext, double & par, double & dpar, double * ddpar, double * dddpar ) const; 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbReparamCurve3D )
}; // MbReparamCurve3D

IMPL_PERSISTENT_OPS( MbReparamCurve3D )


#endif // __CUR_REPARAM_CURVE3D_H
