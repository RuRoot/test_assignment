////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Контур в трёхмерном пространстве.
         \en Contour in three-dimensional space. \~ 

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_CONTOUR3D_H
#define __CUR_CONTOUR3D_H


#include <templ_rp_array.h>
#include <templ_sptr.h>
#include <curve3d.h>
#include <cur_line_segment3d.h>
#include <mb_cube.h>


class  SimpleNameArray;


class   MbContour3D;
namespace c3d // namespace C3D
{
typedef SPtr<MbContour3D>                         SpaceContourSPtr;
typedef SPtr<const MbContour3D>                   ConstSpaceContourSPtr;

typedef std::vector<MbContour3D *>                SpaceContoursVector;
typedef std::vector<const MbContour3D *>          ConstSpaceContoursVector;

typedef std::vector<SpaceContourSPtr>             SpaceContoursSPtrVector;
typedef std::vector<ConstSpaceContourSPtr>        ConstSpaceContoursSPtrVector;
}


//------------------------------------------------------------------------------
/** \brief \ru Контур в трёхмерном пространстве.
           \en Contour in three-dimensional space. \~ 
  \details \ru Контур представляет собой составную кривую, в которой начало каждого последующего сегмента стыкуется с концом предыдущего.
    Контур является замкнутым, если конец последнего сегмента стыкуется с началом первого сегмента.\n
    Если сегменты составной кривой стыкуются не гладко, то составная кривая будет иметь изломы. 
    В общем случае в местах стыковки сегментов производные составной кривой терпят разрыв по длине и направлению. \n
      Начальное значение параметра составной кривой равно нулю. 
    Параметрическая длина составной кривой равна сумме параметрических длин составляющих её сегментов. \n
    При вычислении радиуса-вектора составной кривой сначала определяется сегмент, 
    которому соответствует значение параметра составной кривой, и соответствующее значение собственного параметра этого сегмента. 
    Далее вычисляется радиус-вектор сегмента, который и будет радиусом-вектором составной кривой. \n
      В качестве сегментов составной кривой не используются другие составные кривые. 
    Если составную кривую нужно построить на основе других составных кривых, 
    то последние должны рассматриваться как совокупность составляющих их кривых, а не как единые кривые.\n 
    Трёхмерный контур используется для пространственного моделирования, например, для описания траекторий движения.\n
           \en Contour is a composite curve in which the beginning of each subsequent segment is joined to the end of the previous one.
    Contour is closed if the end of last segment is joined to the beginning of the first segment.\n
    If the segments of a composite curve are not smoothly joined then the composite curve will have breaks. 
    In general case in places of joining segments derivatives of a composite curve have discontinuity along the length and direction. \n
      The initial value of the composite curve is equal to zero. 
    The parametric length of a composite curve is equal to the sum of the parametric lengths of components of its segments. \n
    When the calculation of the radius-vector of a composite curve segment is determined at first, 
    the value of composite curve parameter and the corresponding value of the own parameters of this segment corresponds to this segment. 
    Then computes the radius-vector of the segment which will be the radius-vector of the composite curve. \n
      Other composite curves are not used as segments of the composite curve. 
    If it is required to create a composite curve based on other composite curves, 
    then the latter must be regarded as a set of their curves, and not as single curves. \n 
    The three-dimensional contour is used for space modeling e.g. for describing trajectories. \n \~
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbContour3D : public MbCurve3D {
protected :
  RPArray<MbCurve3D> segments;    ///< \ru Множество сегментов контура. \en A set of contour segments. 
  bool               closed;      ///< \ru Признак замкнутости кривой. \en An attribute of curve closedness. 
  double             paramLength; ///< \ru Параметрическая длина контура. \en Parametric length of a contour. 

public :
  MbContour3D(); ///< \ru Пустой контур. \en Empty contour. 

  /** \brief \ru Конструктор по набору кривых.
              \en Constructor by curves vector. \~
    \details \ru Конструктор по набору кривых. Кривые добавляются в контур без проверки, что начало каждого последующего сегмента стыкуется с концом предыдущего, а также без проверки на самопересечение контура. Выполнение данных условий должно гарантироваться вызывающим кодом. \n
              \en Constructor by curves vector. Curves are added to the contour without checking that the beginning of each subsequent segment joins the end of the previous one, and without checking for self-intersection of the contour. The conditions must be guaranteed by the calling code. \n \~
    \param[in] initSegments - \ru Кривые.
                              \en Curves. \~
    \param[in] sameCurves   - \ru Использовать оригиналы кривых (true) или их копии (false).
                              \en Use original curves (true) or copies thereof (false). \~
    \param[in]  version     - \ru Версия.
                              \en Version. \~
  */
  template <class CurvesVector>
  MbContour3D( const CurvesVector & initSegments, bool sameCurves, VERSION version = Math::DefaultMathVersion() );
protected:
  MbContour3D( const MbContour3D &, MbRegDuplicate * ); ///< \ru Конструктор копирования. \en Copy constructor.
public :
  virtual ~MbContour3D();

public:
  VISITING_CLASS( MbContour3D );

  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */

  MbeSpaceType  IsA()  const override; // \ru Тип элемента \en Type of element 
  MbeSpaceType  Type() const override; // \ru Групповой тип элемента \en Group element type 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  bool        IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Является ли объект копией \en Whether the object is a copy
  bool        SetEqual ( const MbSpaceItem & ) override;       // \ru Сделать равным \en Make equal 
  bool        IsSimilar( const MbSpaceItem & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;   // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;   // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override;   // \ru Повернуть вокруг оси \en Rotate about an axis
  double      DistanceToPoint( const MbCartPoint3D & ) const override;             // \ru Расстояние до точки \en Distance to a point 
  void        PrepareIntegralData( const bool forced ) const override; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.

  bool        IsSpaceSame( const MbSpaceItem & item, double eps = METRIC_REGION ) const override; // \ru Являются ли объекты идентичными в пространстве \en Are the objects identical in space? 

  // \ru Общие функции кривой \en Common functions of curve 

  /** \} */
  /** \ru \name Функции описания области определения кривой.
      \en \name Functions describing the domain of a curve.
      \{ */
  double      GetTMin () const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter 
  double      GetTMax () const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter 
  bool        IsClosed() const override; // \ru Проверка замкнутости кривой \en Check for curve closedness 
  bool        IsSmoothConnected( double angleEps, bool ignoreLimits = false ) const override; // \ru Являются ли стыки контура\кривой гладкими? \en Whether the joints of contour\curve are smooth. 
  bool        IsStraight( bool ignoreParams = false ) const override; // \ru Признак прямолинейности кривой \en An attribute of curve straightness 
  /** \} */

  /** \ru \name Функции для работы в области определения кривой. 
            Функции PointOn, FirstDer, SecondDer, ThirdDer,... корректируют параметр
            при выходе его за пределы области определения.
      \en \name Functions for working in the domain of a curve. 
            Functions: PointOn, FirstDer, SecondDer, ThirdDer,... correct the parameter
            when it is outside domain.
      \{ */
  void        PointOn  ( double & t, MbCartPoint3D & ) const override; // \ru Точка на кривой \en Point on the curve 
  void        FirstDer ( double & t, MbVector3D & ) const override;    // \ru Первая производная \en First derivative
  void        SecondDer( double & t, MbVector3D & ) const override;    // \ru Вторая производная \en Second derivative
  void        ThirdDer ( double & t, MbVector3D & ) const override;    // \ru Третья производная по t \en Third derivative with respect to t
  void        Normal( double  & t, MbVector3D & ) const override;      // \ru Вычислить вектор главной нормали. \en Calculate main normal vector.
  /** \} */

  /** \ru \name Функции для работы внутри и вне области определения кривой. 
            Функции _PointOn, _FirstDer, _SecondDer, _ThirdDer,... не корректируют параметр
            при выходе за пределы области определения. При выходе за область определения незамкнутая
            кривая, в общем случае, продолжается по касательной, которую она имеет на соответствующем конце.
      \en \name Function for working inside and outside of the curve domain. 
            Function _PointOn, _FirstDer, _SecondDer, _ThirdDer,... do not correct a parameter
            when it is outside domain. When parameter is out of domain bounds, an unclosed
            curve is extended by tangent vector at corresponding end point in general case.
      \{ */
  void       _PointOn  ( double t, MbCartPoint3D & ) const override; // \ru Точка на расширенной кривой \en Point on the extended curve
  void       _FirstDer ( double t, MbVector3D & ) const override;    // \ru Первая производная \en First derivative
  void       _SecondDer( double t, MbVector3D & ) const override;    // \ru Вторая производная \en Second derivative
  void       _ThirdDer ( double t, MbVector3D & ) const override;    // \ru Третья производная по t \en Third derivative with respect to t
  void       _Normal( double  t, MbVector3D & ) const override;      // \ru Вычислить вектор главной нормали. \en Calculate main normal vector.
  /** \} */

  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметра кривой.
      \en \name Functions for get of the group of data inside and outside the curve's domain of parameter.
      \{ */
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void        Explore( double & t, bool ext,
                       MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const override;
  /** \} */

  /** \ru \name Функции движения по кривой
      \en \name Functions of the motion along the curve
    \{ */
  double      Step         ( double t, double sag ) const override; // \ru Вычисление шага по стрелке прогиба. \en Calculation of parameter step by the sag.
  double      DeviationStep( double t, double angle ) const override; // \ru Вычисление шага по углу отклонения нормали. \en Calculation of parameter step by the deviation angle.   
  double      MetricStep   ( double t, double length ) const override; // \ru Вычисление шага параметра по длине. \en Calculation of parameter step by the given length.
  /** \} */
  /** \ru \name Общие функции кривой
      \en \name Common functions of curve
      \{ */

  /// \ru Вычислить кривизну кривой. \en Calculate curvature of curve. 
  double      Curvature( double t ) const override;
  // \ru Преобразование в NURBS кривую \en Transform to NURBS-curve 
  MbNurbs3D * NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;

  // \ru Построить усеченную кривую c учетом заданной трехмерной точности. \en Construct a trimmed curve with the given three-dimensional accuracy.
  MbCurve3D * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyzEps, bool saveParamLenAndLaw ) const override;
  using MbCurve3D::Trimmed; // \ru Метод базового класса MbCurve3D без учета трехмерной точности. Точность по умолчанию - MbDimAccuracy::threeDimAcc.  \en Method of MbCurve3D base class without three-dimensional accuracy. The default accuracy is MbDimAccuracy::threeDimAcc.

  // \ru Продлить кривую. \en Extend the curve. \~
  MbResultType Extend( const MbCurveExtensionParameters3D & parameters, c3d::SpaceCurveSPtr & resCurve ) const override;

  // \ru Изменить направление \en Change direction 
  void        Inverse( MbRegTransform * iReg = nullptr ) override;
          // \ru Согласовать параметризацию сегментов, если до инвертирования она была согласованной. \en Agree on segment parameterization, if it was consistent before inversion.
  bool        NormalizeReparametrization();
  /// \ru Подобные ли кривые для объединения (слива). \en Whether the curves to union (joining) are similar. 
  bool        IsSimilarToCurve( const MbCurve3D & other, double precision = METRIC_PRECISION ) const override;

/** \brief  \ru Найти проекцию точки на контур.
            \en Find the point projection to the contour. \~
    \details \ru Найти ближайшую проекцию точки на контур в диапазоне изменения параметра или на его продолжении.
              По умолчанию ('tRange' = nullptr), диапазон изменения параметра совпадает с областью определения контура.
              Если же 'tRange' задан, то диапазон изменения параметра совпадает с 'tRange' (заданный диапазон может 
              выходить за пределы области определения контура).
              Режим работы  метода зависит от 'ext'.
              При 'ext' = true, параметру 't' присваивается значение, соответствующее ближайшей проекции 
              в рамках диапазона изменения параметра или на его продолжении. Результат выполнения метода - true.
              При 'ext' = false, метод производит поиск ближайшей проекции только в рамках диапазона изменения параметра. 
              Если проекция находится, параметру 't' присваивается соответствующее значение. Результат выполнения метода  - true. 
              Если ближайшая проекция не находится, проекция "загоняется" в диапазон и 
              параметру 't' присваивается  значение ближайшей границы диапазона. Результат выполнения метода - false.
              Если имеют место несколько равноудаленных проекций с минимальным расстоянием, 
              выбор производится следующим образом:
              - при 'ext' = true, всегда возвращается точка, лежащая в диапазоне поиска;
              - из проекций, лежащих в диапазоне выбирается проекция с минимальным значением параметра;
              - если все ближайшие проекции лежат вне диапазона ('ext' = true), выбирается ближайшая к области определения проекция.
              Используется метод Ньютона.
             \en Find the nearest projection of a point to the contour within the parameter range or its extension. 
             By default ('tRange' = nullptr), the parameter range coincides with the contour's domain. 
             If the 'tRange' is defined, the parameter range aligns with the 'tRange' (range may not belong to the contour's domain). 
             The method's results depend on the 'ext' flag. 
             When 'ext' = true, the parameter 't' is assigned the value corresponding to the nearest projection within 
             the parameter range or its extension. The method's result is true. 
             When 'ext' = false, the method searches for the nearest projection only within the parameter range. 
             If a projection is found, the parameter 't' is assigned the corresponding value. The method's result is true. 
             If the nearest projection is not found, the projection is confined within the range, 
             and the parameter 't' is assigned the value corresponding  to the nearest boundary of the range. The method's result is false. 
             If there are multiple equidistant projections with the minimum distance, 
             the selection is made according to the following rules: 
             - when 'ext' = true, a point within the parameter range is always returned; 
             - among the projections within the parameter range, the projection with the minimum parameter value is chosen; 
             - if all nearest projections are located outside the parameter range ('ext' = true), 
               the projection closest to the contour's domain is chosen. 
             Newton's method is used. \~
    \param[in] pnt -      \ru Заданная точка.
                          \en A given point. \~
    \param[in] xEpsilon - \ru Точность определения проекции по оси x.
                          \en A tolerance of detection of the projection by x axis. \~
    \param[in] yEpsilon - \ru Точность определения проекции по оси y.
                          \en A tolerance of detection of the projection by y axis. \~
    \param[in,out] t -    \ru На входе - начальное приближение, на выходе - параметр кривой, соответствующий ближайшей проекции.
                          \en Input - initial approximation, output - parameter of a curve corresponding to the nearest projection. \~
    \param[in] ext -      \ru Флаг, определяющий, искать ли проекцию на продолжении диапазона изменения параметра (если true, то искать).
                          \en A flag defining whether to seek projection on the extension of the curve. \~ 
    \param[in] tRange -   \ru Диапазон изменения параметра, в котором надо найти решение.
                          \en A range of parameter changing in which the solution should be found. \~
    \return \ru Возвращает true, если найденный параметр находится в допустимом диапазоне (в соответствии с заданными параметрами ext, tRange),
            или false - в противном случае.
            \en Returns true if the found parameter is in a valid range (according to the given ext, tRange parameters),
            or false - otherwise. \~
  */
  bool        NearPointProjection( const MbCartPoint3D &, double & t, bool ext, MbRect1D * tRange = nullptr ) const override;

  double      CalculateMetricLength() const override;      // \ru Посчитать метрическую длину \en Calculate the metric length 
  double      CalculateLength( double t1, double t2 ) const override;
  bool        DistanceAlong( double & t, double len, int curveDir, double eps = Math::metricPrecision,
                             VERSION version = Math::DefaultMathVersion() ) const override; 

  void        CalculateLocalGabarit( const MbMatrix3D &, MbCube & ) const override; // \ru Рассчитать габарит относительно л.с.к. \en Calculate bounding box relative to the local coordinate system.
  void        CalculateGabarit( MbCube & ) const override; // \ru Вычислить габарит кривой \en Calculate the bounding box of curve 

  MbCurve *   GetMap( const MbMatrix3D &, MbRect1D * pRgn = nullptr,
                      VERSION version = Math::DefaultMathVersion(), bool * coincParams = nullptr ) const override; // \ru Дать плоскую проекцию кривой \en Get a planar projection of curve
  MbCurve *   GetProjection( const MbPlacement3D & place, VERSION version ) const override; // \ru Дать проекцию ребра на плоскость. \en Get the edge projection onto plane.

  size_t      GetCount() const override;
  void        ChangeCarrier( const MbSpaceItem & item, MbSpaceItem & init ) override; // \ru Изменение носителя \en Changing of carrier

  double      GetRadius( double accuracy = METRIC_REGION ) const override; // \ru Дать физический радиус объекта или ноль, если это невозможно. \en Get the physical radius of the object or null if it impossible.
  bool        GetCircleAxis( MbAxis3D & ) const override; // \ru Дать ось кривой. \en Get the curve axis.
  void        GetCentre( MbCartPoint3D & ) const override;
  void        GetWeightCentre( MbCartPoint3D & ) const override;

  bool        IsPlanar  ( double accuracy = METRIC_EPSILON ) const override; // \ru Является ли кривая плоской \en Whether the curve is planar?
  bool        GetPlacement( MbPlacement3D & place, PlanarCheckParams params = PlanarCheckParams() ) const override; // \ru Заполнить плейсемент, ести кривая плоская \en Fill the placement if curve is planar
  // \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская (после использования вызывать DeleteItem на двумерную кривую) \en Give a planar curve and placement, if the spatial curve is planar (after using the DeleteItem must be called on a three-dimensional curve) 
  bool        GetPlaneCurve( MbCurve *& curve2d, MbPlacement3D & place, bool saveParams, PlanarCheckParams params = PlanarCheckParams() ) const override;
  /// \ru Дать поверхностную кривую, если пространственная кривая поверхностная (после использования вызывать DeleteItem на аргументы). \en Get surface curve if the space curve is surface (after the using call DeleteItem for arguments) 
  bool        GetSurfaceCurve( MbCurve *& curve2d, MbSurface *& surface, VERSION version = Math::DefaultMathVersion() ) const override;

  void        CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.
  void        Refresh() override; // \ru Сбросить все временные данные \en Reset all temporary data 

  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisItems ( RPArray<MbSpaceItem> & ) override; // \ru Дать базовые объекты \en Get the basis objects
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \brief \ru Непрерывна ли первая производная кривой по длине и направлению?
              \en Have the first derivative of the curve the continuous length and direction?
    \details \ru Отсутствуют ли разрывы производной по длине и направлению в стыках сегментов контура? \n
              \en Are absent any discontinuities of the derivative at length or at direction in the junction of path segments? \n \~
    \param[out] contLength - \ru Непрерывность длины (да/нет).
                              \en The length is continuous (true/false). \~
    \param[out] contDirect - \ru Непрерывность направления (да/нет).
                              \en The direction of the first derivative is continuous (true/false). \~
    \param[out] params    - \ru Параметры точек, в которых происходит разрыв направления.
                              \en The parameters of the points at which the direction break occurs. \~
    \param[in]  epsilon    - \ru Погрешность вычисления.
                              \en The accuracy of the calculation. \~
  */
  bool        IsContinuousDerivative( bool & contLength, bool & contDirect, c3d::DoubleVector * params = nullptr, double epsilon = EPSILON ) const override;

  /** \brief \ru Устранить разрывы производных по длине в стыках сегментов.
              \en Eliminate the discontinuities of the derivatives of the length of the joints of the segments.
    \details \ru Устранить разрывы производных по длине в стыках сегментов. \n
              \en Eliminate the discontinuities of the derivatives of the length of the joints of the segments. \n \~
    \param[in]  epsilon    - \ru Погрешность вычисления.
                              \en The accuracy of the calculation. \~
    \param[in]  version    - \ru Версия математики.
                              \en Math version. \~
  */
  bool        SetContinuousDerivativeLength( VERSION version, double epsilon = EPSILON ) override;

  /// \ru Получить границы участков кривой, которые описываются одной аналитической функцией. \en Get the boundaries of the curve sections that are described by one analytical function. \~
  void        GetAnalyticalFunctionsBounds( std::vector<double> & params ) const override;
  /// \ru Найти все особые точки функции кривизны кривой и вычислить значение потенциальной энергии кривой. \en Find all the special points of the curvature function of the curve and calculate potential energy of the curve.
  void        GetCurvatureSpecialPoints( std::vector<c3d::DoublePair> & points, double * energy = nullptr ) const override;
  /// \ ru Определение точек излома контура. \en The determination of contour smoothness break points.
  void        BreakPoints( std::vector<double> & vBreaks, double precision = ANGLE_REGION ) const override;

  /** \} */
  /** \ru \name Функции работы с сегментами контура
      \en \name Function for working with segments of contour
      \{ */

  /** \brief \ru Инициализация по массиву кривых.
              \en Initialization by array of curves. \~
    \details \ru Инициализация по массиву кривых. Кривые добавляются в контур без проверки, что начало каждого последующего сегмента стыкуется с концом предыдущего. Выполнение данного условия должно гарантироваться вызывающим кодом. \n
              \en Initialization by array of curves. Curves are added to the contour without checking that the beginning of each subsequent segment joins the end of the previous one. The condition must be guaranteed by the calling code. \n \~
    \param[in] initSegments - \ru Кривые.
                              \en Curves. \~
    \param[in] sameCurves   - \ru Использовать оригиналы кривых (true) или их копии (false).
                              \en Use original curves (true) or copies thereof (false). \~
    \param[in] cls          - \ru Признак замкнутости кривой.
                              \en An Attribute of curve closedness. \~
    \param[in]  version     - \ru Версия.
                              \en Version. \~
    \return \ru Возвращает true, если кривые были добавлены.
            \en Returns true if curves were added. \~
  */
  template <class CurvesVector>
  bool        Init( const CurvesVector & initSegments, bool sameCurves, bool cls, VERSION version = Math::DefaultMathVersion() );
  /// \ru Инициализация по набору точек. \en Initialize by points. 
  template <class PointsVector>
  bool        Init( const PointsVector & points, bool doClosed = true );

  /** \brief \ru Найти сегмент контура.
              \en Find a contour segment. \~
    \details \ru Найти сегмент контура по параметру контура. \n
              \en Find a contour segment by parameter on contour. \n \~
    \param[in,out] t - \ru Параметр контура.
                        \en Contour parameter. \~
    \param[out] tSeg - \ru Параметр сегмента контура.
                        \en Contour segment parameter. \~
    \return \ru Возвращает номер сегмента в случае успешного выполнения или -1.
            \en Returns the segment number in case of successful execution or -1. \~
  */
  ptrdiff_t   FindSegment( double & t, double & tSeg ) const;

  /** \brief \ru Найти параметр контура.
              \en Find a contour segment. \~
    \details \ru Найти параметр контура по номеру сегмента и параметру сегмента. \n
              \en Find a contour parameter by segment number and segment parameter. \n \~
    \param[in] iSeg - \ru Номер сегмента (индекс).
                      \en Segment number (index). \~
    \param[in] tSeg - \ru Параметр сегмента контура.
                      \en Segment parameter. \~
    \return \ru Возвращает параметр контура или UNDEFINED_DBL в случае неудачи.
            \en Returns the contour parameter or UNDEFINED_DBL if failure. \~
  */
  double      FindParameter( size_t iSeg, double tSeg ) const;

  size_t      GetSegmentsCount() const { return segments.size(); } ///< \ru Выдать количество сегментов контура. \en Get the number of contour segments. 
  template <class CurvesVector>
  void        GetSegments( CurvesVector & curves ) const; ///< \ru Получить кривые контура. \en Get contour segments. 

  void        DetachSegments(); ///< \ru Отцепить все сегменты контура. \en Detach all segments of contour. 
  void        DeleteSegments(); ///< \ru Отсоединить используемые сегменты и удалить остальные. \en Delete used segments and remove other segments. 

  void        DeleteSegment( size_t ind ); ///< \ru Удалить сегмент контура. \en Delete the segment of contour. 
  MbCurve3D * DetachSegment( size_t ind ); ///< \ru Отцепить сегмент контура. \en Detach the segment of contour. 

  const   MbCurve3D * GetSegment( size_t ind ) const { return segments[ind]; } ///< \ru Выдать сегмент контура по индексу. \en Get contour segment by the index.
          MbCurve3D * SetSegment( size_t ind )       { return segments[ind]; } ///< \ru Выдать сегмент контура по индексу. \en Get contour segment by the index. 

  void        SetSegment     ( MbCurve3D & newSegment, size_t ind, bool same ); ///< \ru Заменить сегмент в контуре. \en Replace a segment in the contour. 
  void        AddSegment     ( MbCurve3D & newSegment, bool same, VERSION version = Math::DefaultMathVersion() ); ///< \ru Добавить сегмент в контур. \en Add a segment to the contour. 
  void        AddAtSegment   ( MbCurve3D & newSegment, size_t ind, bool same ); ///< \ru Добавить сегмент в контур перед сегментом с индексом ind. \en Add a segment to the contour before the segment with index ind. 
  void        AddAfterSegment( MbCurve3D & newSegment, size_t ind, bool same ); ///< \ru Добавить сегмент в контур после сегмента с индексом ind. \en Add a segment to the contour after the segment with index ind. 

  /** \brief \ru Добавить (усеченную) копию сегмента в конец контура.
              \en Add a (truncated) segment copy to the end of the contour. \~
    \details \ru Добавить (усеченную) копию сегмента в конец контура. \n
              \en Add a (truncated) segment copy to the end of the contour. \n \~
    \param[in] pBasis- \ru Исходная кривая.
                        \en Initial curve. \~
    \param[in] t1 - \ru Начальный параметр усечения.
                    \en Truncation starting parameter. \~
    \param[in] t2 - \ru Конечный параметр усечения.
                    \en Truncation ending parameter. \~
    \param[in] sense - \ru Направление усеченной кривой относительно исходной. \n
                sense =  1 - направление кривой сохраняется.
                sense = -1 - направление кривой меняется на обратное.
                \en Direction of a trimmed curve in relation to an initial curve.
                sense =  1 - direction does not change.
                sense = -1 - direction changes to the opposite value. \~
    \return \ru Возвращает в случае успешного выполнения ненулевой указатель на добавленную кривую.
            \en Returns, if successful, a non-zero pointer to the added curve. \~
  */
  MbCurve3D * AddSegment( MbCurve3D & pBasis, double t1, double t2, int sense );

  void        SegmentsAdd( MbCurve3D & newSegment, bool calculateParamLength = true ); ///< \ru Добавить сегмент в контур без проверки. \en Add a segment to the contour without checking. 
  bool        GetCornerAngle( size_t index, MbCartPoint3D & origin, MbVector3D & axis, MbVector3D & tau, double & angle,
                              double angleEps ) const;
  /// \ru Сбросить переменные кэширования. \en Reset variables caching.
  void        Clear() {
    CalculateParamLengthAndClosed(); // \ru Параметрическая длина контура. \en Parametric length of a contour. 
  }
  bool        IsSimple() const; ///< \ru Состоит ли контур из отрезков и дуг? \en Whether the contour consists of the segments and arcs? 
  /// \ru Управление распределением памяти в массиве segments. \en Control of memory allocation in the array "segments". 
  void        SegmentsReserve( size_t additionalSpace ) { segments.Reserve( additionalSpace ); } ///< \ru Зарезервировать место. \en Reserve space.  
  void        SegmentsAdjust ()                         { segments.Adjust();                   } ///< \ru Удалить лишнюю память. \en Free the unnecessary memory. 

  /** \brief \ru Добавить новый элемент в начало или конец контура.
              \en Add the new element to the beginning or end of contour. \~
    \details \ru Добавить новый элемент в начало или конец контура. \n
              \en Add the new element to the beginning or end of contour. \n \~
    \param[in] curve - \ru Добавляемая кривая.
                        \en Added curve. \~
    \param[in] absEps - \ru Точность проверки совпадения концов кривых (1e-8 - 1e-4).
                        \en Accuracy of verification of curve end coincidence (1e-8 - 1e-4). \~
    \param[in] toEndOnly - \ru Добавлять кривую только в конец контура.
                            \en Add the curve only at the end of the contour. \~
    \param[in] checkSame - \ru Проверять наличие такой же (добавляемой) кривой в контуре.
                            \en Check a presence of the same curve in the contour. \~
    \param[in] checkSame - \ru Версия.
                            \en Version. \~
    \return \ru Возвращает true, если кривая была добавлена.
            \en Returns true if the curve was added. \~
  */
  bool        AddCurveWithRuledCheck( MbCurve3D & curve, double absEps, bool toEndOnly = false, bool checkSame = true,
                                      VERSION version = Math::DefaultMathVersion() );

  /// \ru Проверка непрерывности контура. \en Check for contour continuity. 
  bool        CheckConnection( double eps = METRIC_PRECISION ) const;

  void        CalculateParamLength(); ///< \ru Рассчитать параметрическую длину. \en Calculate parametric length. 
  void        CheckClosed( double closedEps = Math::LengthEps ); ///< \ru Установить признак замкнутости контура. \en Set the closedness attribute of contour. 
  /// \ru Содержат ли контура идентичные сегменты. \en Whether contours contains identical segments. 
  bool        IsSameSegments( const MbContour3D &, double accuracy = METRIC_PRECISION ) const;
  /// \ru Нахождение точки сегмента контура по индексу сегмента. \en Finding the point of a contour segment by segment index. 
  void        FindCorner( size_t index, MbCartPoint3D & ) const;
  /// \ru Установить начальную (конечную) точку для замкнутого контура. \en Set the start (end) point for closed contour. 
  bool        SetBegEndPoint( double t );

  /** \} */
  /** \ru \name Функции работы с именами контура.
      \en \name Functions for working with names of contours.
      \{ */

          /** \brief \ru Дать имена сегментов.
                     \en Get names of segments. \~
            \details \ru Дать имена сегментов контура.\n
                     \en Get names of contour segments. \n \~
            \param[out] names - \ru Имена сегментов.
                                \en Names of segments \~
          */
  void        GetSegmentsNames(       SimpleNameArray & names ) const;

          /** \brief \ru Установить имена сегментов.
                     \en Set names of segments. \~
            \details \ru Установить имена сегментов контура по массиву имен.\n
                     \en Set names of contour segments by array of names. \n \~
            \param[in] names - \ru Набор имен.
                               \en A set of names. \~
          */
  void        SetSegmentsNames( const SimpleNameArray & names );

  /** \} */

private:
  void        SetClosed(); // \ru Проверить и установить признак замкнутости контура. \en Check and set closedness attribute of contour. 
  void        CalculateParamLengthAndClosed();  // \ru Посчитать параметрическую длину и признак замкнутости \en Calculate parametric length and closedness attribute 
  ptrdiff_t  _FindSegment( double & t, double & tSeg ) const; // \ru Нахождение сегмента контура \en Finding of a contour segment 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbContour3D )
  OBVIOUS_PRIVATE_COPY( MbContour3D )

}; // MbContour3D

IMPL_PERSISTENT_OPS( MbContour3D )


//------------------------------------------------------------------------------
// \ru Конструктор по набору кривых. \en Constructor by curves. 
// ---
template <class CurvesVector>
MbContour3D::MbContour3D( const CurvesVector & initSegments, bool sameCurves, VERSION version )
  : MbCurve3D  (                        )
  , segments   ( initSegments.size(), 1 )
  , closed     ( false                  )
  , paramLength( 0.0                    ) // параметрическая длина контура не рассчитана 
{
  const size_t count = initSegments.size();

  if ( count > 0 ) {
    MbRegDuplicate * ireg = nullptr;
    MbAutoRegDuplicate autoreg( ireg );
    for ( size_t i = 0; i < count; ++i ) {
      const MbCurve3D * initSegment = initSegments[i];
      if ( initSegment != nullptr ) {
        C3D_ASSERT( initSegment->GetSubstrate().Type() != st_Contour3D ); // \ru Использование контура не по назначению. \en Wrong contour use as contours container. 
        if ( version >= 0x1500100AL && // \ru Контур в контуре возможен в ранних версиях. \ en A contour within a contour is possible in early versions.
             initSegment->Type() == st_Contour3D ) { // \ru Присланный контур удаляет владелец. \en contour should be deleted by owner.
          const MbContour3D * cntr = static_cast<const MbContour3D *>( initSegment );
          MbContour3D * contour = const_cast<MbContour3D *>( cntr );
          size_t cnt = contour->segments.size();
          for ( size_t j = 0; j < cnt; j++ ) {
            MbCurve3D * seg = contour->segments[j];
            if ( seg != nullptr ) {
              MbCurve3D * segment = sameCurves ? seg : static_cast<MbCurve3D *>(&seg->Duplicate(ireg));
              segments.push_back( segment );
              segment->AddRef();
            }
          }
        }
        else {
          MbCurve3D * segment = sameCurves ? const_cast<MbCurve3D *>(initSegment) : static_cast<MbCurve3D *>(&initSegment->Duplicate(ireg));
          segments.push_back( segment );
          segment->AddRef();
        }
      }
    }
    CalculateParamLengthAndClosed();
  }
}


//------------------------------------------------------------------------------
// \ru Инициализация по набору кривых. \en Initialize by curves. 
// ---
template <class CurvesVector>
bool MbContour3D::Init( const CurvesVector & initSegments, bool sameCurves, bool cls, VERSION version )
{
  size_t count = initSegments.size();

  if ( count > 0 ) {
    ::AddRefItems( initSegments );
    DeleteSegments();
    MbRegDuplicate * ireg = nullptr;
    MbAutoRegDuplicate autoreg( ireg );
    segments.reserve( count );
    for ( size_t i = 0; i < count; ++i ) {
      const MbCurve3D * initSegment = initSegments[i];
      if ( initSegment != nullptr ) {
        C3D_ASSERT( initSegment->GetSubstrate().Type() != st_Contour3D ); // \ru Использование контура не по назначению. \en Wrong contour use as contours container. 
        if ( version >= 0x1500100AL && // \ru Контур в контуре возможен в ранних версиях. \ en A contour within a contour is possible in early versions.
             initSegment->Type() == st_Contour3D ) { // \ru Присланный контур удаляет владелец. \en contour should be deleted by owner.
          const MbContour3D * cntr = static_cast<const MbContour3D *>( initSegment );
          MbContour3D * contour = const_cast<MbContour3D *>( cntr );
          size_t cnt = contour->segments.size();
          for ( size_t j = 0; j < cnt; j++ ) {
            MbCurve3D * seg = contour->segments[j];
            if ( seg != nullptr ) {
              MbCurve3D * segment = sameCurves ? seg : static_cast<MbCurve3D *>(&seg->Duplicate(ireg));
              segments.push_back( segment );
              segment->AddRef();
            }
          }
        }
        else {
          MbCurve3D * segment = sameCurves ? const_cast<MbCurve3D *>( initSegment ) : static_cast<MbCurve3D *>(&initSegment->Duplicate(ireg));
          segments.push_back( segment );
          segment->AddRef();
        }
      }
    }
    ::DecRefItems( initSegments );
    CalculateParamLength();
    closed = cls;
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru Инициализация по набору точек. \en Initialize by points. 
// ---
template <class PointsVector>
bool MbContour3D::Init( const PointsVector & points, bool doClosed )
{
  bool isDone = false;

  size_t count = points.size();

  if ( count > 1 ) {
    if ( doClosed ) {
      if ( count > 2 ) {
        DeleteSegments();
        segments.reserve( count );
        for ( size_t i = 0; i < count; ++i ) {
          MbLineSegment3D * seg = new MbLineSegment3D( points[i], points[(i + 1) % count] );
          segments.push_back( seg );
          seg->AddRef();
        }
        CalculateParamLength();
        closed = true;
        isDone = true;
      }
    }
    else {
      size_t cnt = count - 1;
      DeleteSegments();
      segments.reserve( cnt );
      for ( size_t i = 0; i < cnt; ++i ) {
        MbLineSegment3D * seg = new MbLineSegment3D( points[i], points[i + 1] );
        segments.push_back( seg );
        seg->AddRef();
      }
      CalculateParamLength();
      closed = false;
      isDone = true;
    }
  }

  return isDone;
}


//------------------------------------------------------------------------------
// \ru Получить кривые контура. \en Get contour segments. 
// ---
template <class CurvesVector>
void MbContour3D::GetSegments( CurvesVector & curves ) const
{
  size_t segmentsCnt = segments.size();
  curves.reserve( curves.size() + segmentsCnt );
  SPtr<MbCurve3D> curve;
  for ( size_t k = 0; k < segmentsCnt; ++k ) {
    curve = const_cast<MbCurve3D *>(segments[k]);
    if ( curve != nullptr ) {
      curves.push_back( curve );
      ::DetachItem( curve );
    }
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Усечь контур.
           \en Trim a contour. \~
    \details \ru Усечь контур. Расширенная версия функции контура Trimmed
             \en Trim a contour. This function is extended version of contour's function Trimmed. \~
    \param[in] t1    - \ru Параметр, соответствующий началу усеченной кривой.
                       \en Parameter corresponding to start of a trimmed curve. \~
    \param[in] t2    - \ru Параметр, соответствующий концу усеченной кривой.
                       \en Parameter corresponding to end of a trimmed curve. \~
    \param[in] sense - \ru Направление усеченной кривой относительно исходной.\n
                       sense =  1 - направление кривой сохраняется.
                       sense = -1 - направление кривой меняется на обратное.
                       \en Direction of a trimmed curve in relation to an initial curve.
                       sense =  1 - direction does not change.
                       sense = -1 - direction changes to the opposite value. \~
    \param[in] useTrimmedOnly - \ru При усечении создавать кривые MbTrimmedCurve3D.
                                \en A truncated segment is always curve MbTrimmedCurve3D. \~
    \result \ru Построенная усеченная кривая.
            \en A constructed trimmed curve. \~
    \ingroup Curves_3D
  */
// ---
MATH_FUNC (MbCurve3D *) TrimContour( const MbContour3D & cntr, double t1, double t2, int sense,
                                     bool useTrimmedOnly, bool saveParamLenAndLaw = false  );


#endif // __CUR_CONTOUR3D_H
