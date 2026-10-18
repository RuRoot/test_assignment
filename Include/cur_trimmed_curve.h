////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Усеченная кривая в двумерном пространстве.
         \en Trimmed curve in two-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_TRMMED_CURVE_H
#define __CUR_TRMMED_CURVE_H


#include <curve.h>
#include <mb_rect.h>


class             MbRegDuplicate;
class             MbRegTransform;


//------------------------------------------------------------------------------
/** \brief \ru Усеченная кривая в двумерном пространстве.
           \en Trimmed curve in two-dimensional space. \~
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
  \ingroup Curves_2D
*/
// ---
class MATH_CLASS MbTrimmedCurve : public MbCurve {
// \ru Усечение может быть на продолжении кривой (внесенные изменения помечены как E13865) \en Trimming can be on curve extension (made changes are marked as E13865) 
protected :
  MbCurve * basisCurve; ///< \ru Базовая кривая (не может быть nullptr). \en Base curve (can't be nullptr).
  double    trim1;      ///< \ru Параметры начальной точки \en Parameters of start point 
  double    trim2;      ///< \ru Параметры конечной  точки \en Parameters of end point 
  int       sense;      ///< \ru Флаг совпадения направления с направлением базовой кривой (sense==0 не допускается) \en Flag of coincidence of the direction with the direction of the base curve (sense==0 isn't allowed) 
                        ///< \ru Если (sense > 0), то (trim2 > trim1) \en If (sense > 0), then (trim2 > trim1) 
                        ///< \ru Если (sense < 0), то (trim2 < trim1) \en If (sense < 0), then (trim2 < trim1) 
                        ///< \ru Равенство trim1 и trim2 не допускается \en Equality of 'trim1' and 'trim2' isn't allowed 

  mutable MbRect        rect;         ///< \ru Габаритный прямоугольник \en Bounding rectangle 
  mutable atomic_double metricLength; ///< \ru Метрическая длина усеченной кривой \en Metric length of a trimmed curve 

protected :
  MbTrimmedCurve( const MbTrimmedCurve & initCurve );
  MbTrimmedCurve( MbTrimmedCurve * initCurve ); // \ru Сохранение той же базовой кривой \en Preservation of the same base curve 
public :
  MbTrimmedCurve( const MbCurve & initCurve, double t1, double t2, int initSense, 
                  bool same, double eps = Math::paramEpsilon );
  // \ru Для периодической базовой кривой прии t1==t2 получим периодическую кривую со смещённым началом. \en For a periodic base curve if 't1' is equal to 't2', then obtain periodic curve with shifted beginning. 
public :
  virtual ~MbTrimmedCurve();

public :
  VISITING_CLASS( MbTrimmedCurve ); 

  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */

  MbePlaneType  IsA() const override;  // \ru Тип элемента \en Type of element 
  MbePlaneType  Type() const override; // \ru Вернуть тип кривой \en Get type of curve 
  bool        IsSimilar( const MbPlaneItem & ) const override; // \ru Являются ли элементы подобными \en Whether the elements are similar 
  bool        SetEqual ( const MbPlaneItem & ) override;       // \ru Сделать элементы равными \en Make the elements equal 
   bool        IsSame   ( const MbPlaneItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Является ли кривая curve копией данной кривой ? \en Whether curve 'curve' is a duplicate of the current curve. 
  void        Transform( const MbMatrix &, MbRegTransform * ireg = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector &, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbCartPoint & pnt, const MbDirection & angle, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Поворот \en Rotation
  MbPlaneItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  void        AddYourGabaritTo( MbRect & ) const override; // \ru Добавь свой габарит в прямой прям-к \en Add your own gabarit into the given bounding rectangle 
  void        CalculateGabarit( MbRect & ) const override; // \ru Определить габаритный прямоугольник кривой. \en Detect the bounding box of a curve.
  bool        IsInRectForDeform( const MbRect & r ) const override; // \ru Виден ли объект в заданном прямоугольнике для деформации \en Whether the object is visible in the given rectangle for deformation 
  void        Refresh() override; // \ru Сбросить все временные данные \en Reset all temporary data
  void        PrepareIntegralData( const bool forced ) const override; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.
  /** \} */

  /** \ru \name Функции описания области определения кривой.
      \en \name Functions for description of a curve domain.
      \{ */
  double      GetTMin()      const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter
  double      GetTMax()      const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter
  bool        IsClosed()     const override;  // \ru Проверка замкнутости кривой \en Check for curve closedness
  double      GetPeriod()    const override; // \ru Вернуть период \en Get period
  bool        IsDegenerate( double eps = Math::LengthEps ) const override; // \ru Проверка вырожденности кривой \en Check for curve degeneracy 
  /** \} */

  /** \ru \name Функции для работы в области определения кривой. 
            Функции PointOn, FirstDer, SecondDer, ThirdDer,... корректируют параметр
            при выходе его за пределы области определения.
      \en \name Functions for working in a curve domain. 
            Functions PointOn, FirstDer, SecondDer, ThirdDer,... correct parameter
            if it is out of domain bounds.
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
      \en \name Functions for working inside and outside the curve's domain. 
            Functions _PointOn, _FirstDer, _SecondDer, _ThirdDer,... don't correct parameter
            if it is out of domain bounds. If the parameter is out of domain bounds, an unclosed
            curve is extended by tangent vector at corresponding end point in general case.
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
  double      Step( double t, double sag ) const override; // \ru Вычисление шага аппроксимации \en Calculation of step of approximation 
  double      DeviationStep( double t, double angle ) const override; // \ru Вычисление шага аппроксимации с учетом угла отклонения \en Calculation of step of approximation with consideration of angle of deviation 

  /** \} */
  /** \ru \name Общие функции кривой
      \en \name Common functions of curve
      \{ */

  double      Curvature( double t ) const override; // \ru Кривизна усеченной кривой \en Curvature of a trimmed curve 
  bool        HasLength( double & length ) const override;
  double      GetLengthEvaluation() const override; // \ru Оценка метрической длины кривой \en Estimation of metric length of the curve  
  double      GetMetricLength() const override; // \ru Метрическая длина \en The metric length 

  bool        GetMiddlePoint( MbCartPoint & ) const override; // \ru Вычислить среднюю точку кривой. \en Calculate mid-point of curve. 
  double      PointProjection( const MbCartPoint & pnt ) const override; // \ru Проекция точки на кривую \en Point projection on the curve 

  bool        IsStraight( bool ignoreParams = false ) const override; // \ru Признак прямолинейности кривой \en An attribute of curve straightness. 
  // \ru Положение точки относительно кривой. \en The point position relative to the curve. 
  // \ru Возвращает результат : \en Returning result: 
  // \ru iloc_InItem = 1 - точка находится слева от кривой, \en Iloc_InItem = 1 - point is to the left of the curve, 
  // \ru iloc_OnItem = 0 - точка находится на кривой, \en Iloc_OnItem = 0 - point is on the curve, 
  // \ru iloc_OutOfItem = -1 - точка находится справа от кривой. \en Iloc_OutOfItem = -1 - point is to the right of the curve. 
  MbeItemLocation PointRelative( const MbCartPoint & pnt, double eps = Math::LengthEps ) const override;
  /// \ru Получить границы участков кривой, которые описываются одной аналитической функцией. 
  /// \en Get the boundaries of the curve sections that are described by one analytical function. \~
  void        GetAnalyticalFunctionsBounds( std::vector<double> & params ) const override;

  const MbCurve & GetBasisCurve() const override; // \ru Вернуть базовую кривую \en Get the base curve 
        MbCurve & SetBasisCurve() override;       // \ru Вернуть базовую кривую \en Get the base curve 

  void        Isoclinal( const MbVector & angle, SArray<double> & tFind ) const override; // \ru Прямые, проходящие под углом к оси 0X и касательные к кривой \en Lines passing angularly to the 0X axis and tangent to the curve
  // \ru Выдать характерную точку усеченной кривой если она ближе чем dmax \en Get control point of trimmed curve if it is closer than 'dmax' 
  bool        GetSpecificPoint( const MbCartPoint & from, double & dmax, MbCartPoint & pnt ) const override;
  MbeState    DeletePart( double t1, double t2, MbCurve *& part2 ) override; // \ru Удалить часть усеченной кривой между параметрами t1 и t2 \en Delete a part of a trimmed curve between parameters t1 and t2 
  MbeState    TrimmPart ( double t1, double t2, MbCurve *& part2 ) override; // \ru Оставить часть усеченной кривой между параметрами t1 и t2 \en Keep a part of the trimmed curve between parameters t1 and t2 
  // \ru Построить усеченную кривую c учетом заданной двумерной точности. \en Construct a trimmed curve with the given two-dimensional accuracy.
  MbCurve *   Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyEps, bool saveParamLenAndLaw ) const override;
  using MbCurve::Trimmed; // \ru Метод базового класса MbCurve без учета двумерной точности. Точность по умолчанию - MbDimAccuracy::twoDimAcc.  \en Method of MbCurve base class without two-dimensional accuracy. The default accuracy is MbDimAccuracy::twoDimAcc.

  MbCurve *   Offset( double rad ) const override;    // \ru Смещение усеченной кривой \en Shift of a trimmed curve 

  MbNurbs *   NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;
  MbCurve *   NurbsCurve( const MbNurbsParameters & ) const override; // \ru Построить NURBS-копию кривой \en Create a NURBS-copy of the curve 

  void        ParameterInto( double & t ) const; // \ru Перевод параметра базовой кривой в локальный параметр \en Transformation of the base curve parameter to a local parameter 
  void        ParameterFrom( double & t ) const; // \ru Перевод локального параметра в параметр базовой кривой \en Transformation of a local parameter to the base curve parameter 
  double      GetBasisParameter( double & t ) const; // \ru Установить параметр в область допустимых значений \en Set the parameter into the region of the legal values 
  bool        IsBaseParamOn( double t, double eps = Math::paramEpsilon ) const; // \ru Находится ли параметр базовой кривой в диапазоне усеченной кривой \en Whether the parameter of base curve is in range of a trimmed curve 

  double      GetTrim1() const { return trim1; }
  double      GetTrim2() const { return trim2; }
  int         GetSense() const { return trim2 > trim1 ? 1 : -1; } // \ru Флаг совпадения направления с направлением базовой кривой \en Flag of coincidence of the direction with the direction of base curve 
  void        SetTrim1( double t ) { trim1 = t; InitParam( trim1, trim2, sense ); }
  void        SetTrim2( double t ) { trim2 = t; InitParam( trim1, trim2, sense ); }

  void        Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление кривой \en Change direction of a curve
  bool        GetAxisPoint( MbCartPoint & p ) const override; // \ru Точка для построения оси \en Point for the axis construction  
  bool        IsSimilarToCurve( const MbCurve & curve, double precision = PARAM_PRECISION ) const override; // \ru Подобные ли кривые для объединения (слива) \en Whether the curves for union (joining) are similar 
  double      GetRadius( double accuracy = PARAM_REGION ) const override; // \ru Дать физический радиус объекта или ноль, если это невозможно. \en Get the physical radius of the object or null if it impossible.
  bool        GetCentre     ( MbCartPoint & ) const override; ///< \ru Вычислить центр кривой. \en Calculate center of curve.
  size_t      GetCount() const override; // \ru Количество разбиений для прохода в операциях \en Count of subdivisions for pass in operations  

  // \ru Геометрия подложки тождественна геометрии кривой, отлична параметризация \en Geometry of substrate is identical to geometry of curve, parameterization is different  
  const MbCurve & GetSubstrate() const override; // \ru Выдать подложку или себя \en Get substrate or itself
        MbCurve & SetSubstrate() override;       // \ru Выдать подложку или себя \en Get substrate or itself
  int         SubstrateCurveDirection() const override;    // \ru Направление подложки относительно кривой или наоборот \en Direction of substrate relative to the curve or vice versa 
  void        SubstrateToCurve( double & ) const override; // \ru Преобразовать параметр подложки в параметр кривой \en Transform a substrate parameter to the curve parameter
  void        CurveToSubstrate( double & ) const override; // \ru Преобразовать параметр кривой в параметр подложки \en Transform a curve parameter to the substrate parameter

  void        GetProperties( MbProperties & properties ) override;  // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & properties ) override;  // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisPoints( MbControlData & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  // \ru Продлить кривую. \en Extend the curve. \~
  MbResultType Extend( const MbCurveExtensionParameters & parameters, c3d::PlaneCurveSPtr & resCurve ) const override;

  /** \} */

  void        SetBasisCurve( MbCurve & newCurve );
  void        InitParam( double t1, double t2, int s, double eps = Math::paramEpsilon );
  void        Init( double t1, double t2, int initSense ) {
                InitParam( t1, t2, initSense );
                Refresh();
              }

  const   MbTrimmedCurve & operator = ( const MbTrimmedCurve & source ); // \ru Присвоение параметров усеченной кривой \en Assignment of parameters of trimmed curve 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbTrimmedCurve )
}; // MbTrimmedCurve

IMPL_PERSISTENT_OPS( MbTrimmedCurve )


//------------------------------------------------------------------------------
// \ru Находится ли параметр базовой кривой в диапазоне \en Whether the parameter of the base curve is in the range 
// \ru Усеченной кривой \en Of the trimmed curve 
// ---
inline bool MbTrimmedCurve::IsBaseParamOn( double t, double eps ) const {
  ParameterInto( t );
  return IsParamOn( t, eps );
}


#endif // __CUR_TRMMED_CURVE_H
