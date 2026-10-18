////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Усеченная кривая в трехмерном пространстве.
         \en Trimmed curve in three-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_TRMMED_CURVE3D_H
#define __CUR_TRMMED_CURVE3D_H


#include <curve3d.h>
#include <mb_dim_accuracy.h>


//------------------------------------------------------------------------------
/** \brief \ru Усеченная кривая в трехмерном пространстве.
           \en Trimmed curve in three-dimensional space. \~
  \details \ru Усеченная кривая описывает участок базовой кривой basisCurve, 
    который начинается в точке с параметром trim1 и оканчивается в точке с параметром trim2. \n
    Описываемый участок может иметь направление, совпадающее с направлением базовой кривой (sense == +1),
    а также может иметь направление, противоположное направлению базовой кривой (sense == -1).
    Для замкнутых периодических кривых описываемый участок может содержать внутри начальную точку базовой кривой. 
    Базовой кривой для усеченной кривой не может служить другая усеченная кривая.
    В подобной ситуации выполняется переход к первичной базовой кривой.
           \en Trimmed curve describes a piece of base curve 'basisCurve' 
    which starts at point with 'trim1' parameter and ends at point with 'trim2' parameter. \n
    Described piece can have the direction coinciding with the direction of a base curve (sense == +1),
    and also can have the direction opposite to the direction of a base curve (sense == -1).
    For closed periodic curves the described piece can contain the start point of base curve inside. 
    Another trimmed curve can't be the base curve for a trimmed curve.
    In this situation it changes to the initial base curve. \~
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbTrimmedCurve3D : public MbCurve3D {
private :
  MbCurve3D *     basisCurve;   ///< \ru Базовая кривая. \en The base curve. 
  double          trim1;        ///< \ru Параметр начальной точки базовой кривой. \en Parameter of the start point of the base curve. 
  double          trim2;        ///< \ru Параметр конечной  точки базовой кривой. \en Parameter of the end point of base curve. 
  int             sense;        ///< \ru Флаг совпадения направления с направлением базовой кривой. \en Flag of coincidence of the direction with the direction of base curve. 
private:
  mutable atomic_double  metricLength; ///< \ru Метрическая длина кривой. \en Metric length of a curve. 

public :
  MbTrimmedCurve3D( const MbCurve3D & initCurve, double t1, double t2, int initSense, bool same, const MbDimAccuracy & xyzEps = MbDimAccuracy::threeDimAcc );
  MbTrimmedCurve3D( const MbCurve3D & initCurve, const MbCartPoint3D & p0, const MbCartPoint3D & p1 );
protected :
  MbTrimmedCurve3D( const MbTrimmedCurve3D &, MbRegDuplicate * );
private :
  MbTrimmedCurve3D( const MbTrimmedCurve3D & ); // \ru Не реализовано. \en Not implemented. 
public :
  virtual ~MbTrimmedCurve3D();

public : 
  VISITING_CLASS( MbTrimmedCurve3D );

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 
  
  MbeSpaceType  IsA() const override; // \ru Тип элемента \en Type of element 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Является ли объект копией \en Whether the object is a copy 
  bool    SetEqual ( const MbSpaceItem & ) override; // \ru Сделать равным \en Make equal 
  bool    IsSimilar( const MbSpaceItem & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;    // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;    // \ru Сдвиг \en Translation
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis
  void    Refresh() override; // \ru Сбросить все временные данные \en Reset all temporary data 
  void    PrepareIntegralData( const bool forced ) const override; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.

  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void    GetBasisItems ( RPArray<MbSpaceItem> & ) override;  // \ru Дать базовые объекты \en Get the basis objects 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  // \ru Общие функции кривой \en Common functions of curve 

  double  GetTMin()  const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter 
  double  GetTMax()  const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter 
  bool    IsClosed() const override; // \ru Проверка замкнутости кривой \en Check for curve closedness 
  double  GetPeriod() const override; // \ru Вернуть период \en Get period  
  // \ru Функции кривой для работы в области определения параметрической кривой \en Functions of curve for working at parametric curve domain 
  void    PointOn  ( double &t, MbCartPoint3D & ) const override; // \ru Точка на кривой \en Point on the curve
  void    FirstDer ( double &t, MbVector3D & ) const override;     // \ru Первая производная \en The first derivative
  void    SecondDer( double &t, MbVector3D & ) const override;     // \ru Вторая производная \en The second derivative
  void    ThirdDer ( double &t, MbVector3D & ) const override;     // \ru Третья производная по t \en The third derivative with respect to t
  void    Normal   ( double &t, MbVector3D & ) const override;   // \ru Вектор главной нормали \en Vector of the principal normal
  // \ru Функции кривой для работы вне области определения параметрической кривой \en Functions of curve for working outside the domain of parametric curve 
  void   _PointOn  ( double t,  MbCartPoint3D & ) const override; // \ru Точка на расширенной кривой \en Point on the extended curve
  void   _FirstDer ( double t,  MbVector3D & ) const override;    // \ru Первая производная \en The first derivative
  void   _SecondDer( double t,  MbVector3D & ) const override;    // \ru Вторая производная \en The second derivative
  void   _ThirdDer ( double t,  MbVector3D & ) const override;    // \ru Третья производная по t \en The third derivative with respect to t
  void   _Normal   ( double t,  MbVector3D & ) const override;   // \ru Вектор главной нормали \en Vector of the principal normal
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void    Explore( double & t, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const override;

  MbNurbs3D * NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;
  MbCurve3D * NurbsCurve( const MbNurbsParameters & ) const override; // \ru Построить NURBS-копию кривой \en Create a NURBS-copy of the curve 

  const MbCurve3D & GetBasisCurve() const override;
         MbCurve3D & SetBasisCurve() override;

  // \ru Построить усеченную кривую c учетом заданной трехмерной точности. \en Construct a trimmed curve with the given three-dimensional accuracy.
  MbCurve3D * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyzEps, bool saveParamLenAndLaw ) const override; 
  using MbCurve3D::Trimmed; // \ru Метод базового класса MbCurve3D без учета трехмерной точности. Точность по умолчанию - MbDimAccuracy::threeDimAcc.  \en Method of MbCurve3D base class without three-dimensional accuracy. The default accuracy is MbDimAccuracy::threeDimAcc.

  void    Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction
  double  Curvature    ( double  )                const override; // \ru Кривизна усеченной кривой \en Curvature of a trimmed curve
  double  Step         ( double t, double sag ) const override; // \ru Вычисление шага по стрелке прогиба. \en Calculation of parameter step by the sag.
  double  DeviationStep( double t, double angle ) const override; // \ru Вычисление шага по углу отклонения нормали. \en Calculation of parameter step by the deviation angle.   
  double  MetricStep   ( double t, double length ) const override; // \ru Вычисление шага параметра по длине. \en Calculation of parameter step by the given length. 

  bool    IsDegenerate( double eps = METRIC_PRECISION ) const override; // \ru Проверка вырожденности кривой \en Check for curve degeneracy
  MbCurve * GetMap( const MbMatrix3D &, MbRect1D * pRgn = nullptr,
                            VERSION version = Math::DefaultMathVersion(), bool * coincParams = nullptr ) const override; // \ru Дать плоскую проекцию кривой \en Get a planar projection of a curve
  size_t  GetCount() const override;
  void    ChangeCarrier( const MbSpaceItem & item, MbSpaceItem & init ) override; // \ru Изменение носителя \en Change a carrier 
  bool    IsPlanar  ( double accuracy = METRIC_EPSILON ) const override; // \ru Является ли кривая плоской \en Whether the curve is planar 
  bool    IsStraight( bool ignoreParams = false ) const override; // \ru Является ли линия прямолинейной \en Whether the line is straight 
  double  GetRadius( double accuracy = METRIC_REGION ) const override; // \ru Дать физический радиус объекта или ноль, если это невозможно. \en Get the physical radius of the object or null if it impossible.
  bool    GetCircleAxis  ( MbAxis3D & ) const override; // \ru Дать ось кривой \en Get axis of curve 
  void    GetCentre( MbCartPoint3D &wc ) const override; // \ru Посчитать центр кривой \en Calculate the center of a curve

  double  GetLengthEvaluation() const override; // \ru Оценка метрической длины кривой \en Estimation of metric length of the curve  
  double  GetMetricLength() const override; // \ru Метрическая длина \en Metric length 

  void    SetBasisCurve( MbCurve3D & );      // \ru Заменить плоскую кривую \en Replace the planar curve 
  void    InitParam( double t1, double t2, int initSense, const MbDimAccuracy & xyzEps = MbDimAccuracy::threeDimAcc );
  double  GetTrim1() const { return trim1; }
  double  GetTrim2() const { return trim2; }
  int     GetSense() const { return sense; } // \ru Флаг совпадения направления с направлением базовой кривой \en Flag of coincidence of the direction with the direction of base curve 

  void    ParameterInto( double &t ) const; // \ru Перевод параметра базовой кривой в параметр усеченной кривой \en Transformation of parameter of base curve to a parameter of trimmed curve 
  void    ParameterFrom( double &t ) const; // \ru Перевод локального параметра в параметр базовой кривой \en Transformation of a local parameter to the base curve parameter 
  double  GetBasisParameter( double &t ) const; // \ru Установить параметр в область допустимых значений \en Set the parameter into the region of the legal values 

  bool    IsBaseParamOn( double t ) const; // \ru Находится ли параметр базовой кривой в диапазоне усеченной кривой \en Whether the parameter of base curve is in range of a trimmed curve 

  // \ru Ближайшая проекция точки на кривую. \en The nearest projection of a point onto the curve. 
  bool    NearPointProjection( const MbCartPoint3D &, double & t, bool ext, MbRect1D * tRange = nullptr ) const override; // \ru Ближайшая проекция точки на кривую \en The nearest projection of a point onto the curve
  /// \ru Найти все особые точки функции кривизны кривой и вычислить значение потенциальной энергии кривой.
  /// \en Find all the special points of the curvature function of the curve and calculate potential energy of the curve. \~
  void    GetCurvatureSpecialPoints( std::vector<c3d::DoublePair> & points, double * energy = nullptr ) const override;
  /// \ru Получить границы участков кривой, которые описываются одной аналитической функцией. 
  /// \en Get the boundaries of the curve sections that are described by one analytical function. \~
  void    GetAnalyticalFunctionsBounds( std::vector<double> & params ) const override;

  // \ru Геометрия базовой кривой тождественна геометрии кривой, отлична параметризация. \en Geometry of base curve is identical to geometry of curve, parameterization is different. 
  const MbCurve3D & GetSubstrate() const override; // \ru Выдать подложку или себя \en Get substrate or itself 
  MbCurve3D & SetSubstrate() override;       // \ru Выдать подложку или себя \en Get substrate or itself 
  int     SubstrateCurveDirection() const override;    // \ru Направление подложки относительно кривой или наоборот \en Direction of substrate relative to the curve or vice versa
  void    SubstrateToCurve( double & ) const override; // \ru Преобразовать параметр подложки в параметр кривой \en Transform a substrate parameter to the curve parameter
  void    CurveToSubstrate( double & ) const override; // \ru Преобразовать параметр кривой в параметр подложки \en Transform a curve parameter to the substrate parameter

  // \ru Заполнить плейсемент, ести кривая плоская \en Fill the placement if the curve is planar 
  bool    GetPlacement( MbPlacement3D & place, PlanarCheckParams params = PlanarCheckParams() ) const override;
  // \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская (после использования вызывать DeleteItem на двумерную кривую) \en Get the planar curve and placement if the spatial curve is planar (call DeleteItem for two-dimensional curve after using) 
  bool    GetPlaneCurve( MbCurve *& curve2d, MbPlacement3D & place, bool saveParams, PlanarCheckParams params = PlanarCheckParams() ) const override;
  // \ru Дать поверхностную кривую, если пространственная кривая поверхностная (после использования вызывать DeleteItem на аргументы) \en Get a surface curve if a spatial curve is on a surface (call DeleteItem for arguments after using ) 
  bool    GetSurfaceCurve( MbCurve *& curve2d, MbSurface *& surface, VERSION version = Math::DefaultMathVersion() ) const override;

  /// \ru Является ли объект смещением \en Whether the object is a shift 
  bool    IsShift( const MbSpaceItem &, MbVector3D &, bool & isSame, double accuracy = LENGTH_EPSILON ) const override;
  bool    IsSimilarToCurve( const MbCurve3D & curve, double precision = METRIC_PRECISION ) const override; // \ru Подобные ли кривые для объединения (слива) \en Whether the curves for union (joining) are similar 

  // \ru Продлить кривую. \en Extend the curve. \~
  MbResultType Extend( const MbCurveExtensionParameters3D & parameters, c3d::SpaceCurveSPtr & resCurve ) const override;

private:
  void    operator = ( const MbTrimmedCurve3D & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbTrimmedCurve3D )
};

IMPL_PERSISTENT_OPS( MbTrimmedCurve3D )


//------------------------------------------------------------------------------
// \ru Находится ли параметр базовой кривой в диапазоне усеченной кривой \en Whether the parameter of base curve is in range of a trimmed curve 
// ---
inline bool MbTrimmedCurve3D::IsBaseParamOn( double t ) const {
  ParameterInto( t );
  return IsParamOn( t, PARAM_REGION );
}


#endif // __CUR_TRMMED_CURVE3D_H
