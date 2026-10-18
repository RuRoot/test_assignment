////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Кривая на поверхности.
         \en Curve on surface. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_SURFACE_CURVE_H
#define __CUR_SURFACE_CURVE_H


#include <templ_three_states.h>
#include <curve3d.h>
#include <mb_cube.h>
#include <plane_item.h>
#include <tool_multithreading.h>
#include <surf_tessellation.h>


class   MbRect;
class   MbPlacement3D;
class   MbCurve;
class   MbSurface;
class   MbSurfaceIntersectionCurve;
class   MbContourOnSurface;
class   MbCurveTessellation;
class             MbCurveIntoNurbsInfo;


//------------------------------------------------------------------------------
/** \brief \ru Кривая на поверхности.
           \en Curve on surface. \~
  \details \ru Кривая на поверхности строится путём введения зависимости параметров поверхности u и v 
    от некоторого общего для них параметра t: u=u(t), v=v(t). \n 
    Параметры поверхности u и v являются координатами двумерной точки в пространстве параметров поверхности. 
    Кривая на поверхности описывается поверхностью surface и двумерной кривой в пространстве параметров curve. 
    Поверхностью surface может быть любая поверхность, кроме MbCurveBoundedSurface. \n 
    Для заданного параметра t кривой curve вычисляется двумерная точка w=[u v] области параметров поверхности, 
    далее для параметров u и v поверхностью surface вычисляется точка кривой на поверхности. 
    Параметры u и v поверхности могут выходить за пределы её области определения. \n 
    Кривая на поверхности может быть периодической, 
    если периодической является двумерная кривая curve или 
    если кривая curve имеет совпадающие производные на краях и крайние точки кривой 
    смещены на соответствующий период периодической по первому или второму параметру поверхности surface.
           \en Curve on surface is constructed by introduction of dependence of u and v surface parameters 
    from some parameter t common for them: u=u(t), v=v(t). \n 
    u and v surface parameters are coordinates of two-dimensional point in space of surface parameters. 
    Curve on surface is described by 'surface' surface and two-dimensional curve 'curve' in space of parameters. 
    Any surface except MbCurveBoundedSurface can be surface 'surface'. \n 
    two-dimensional point w=[u v] of region of surface parameters is calculated for a given parameter t of curve 'curve', 
    further, a point of curve on surface is calculated for u and v parameters of 'surface' surface. 
    u and v surface parameters can exceed the bounds of its domain. \n 
    Curve on surface can be periodic, 
    if two-dimensional curve 'curve' is periodic or 
    if curve 'curve' has coinciding derivatives at the end points and the curve end points 
    is shifted by corresponding period of 'surface' surface which is periodic by first or second parameter. \~
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbSurfaceCurve : public MbCurve3D {
protected :
  MbCurve           * curve;    ///< \ru Плоская кривая в uv-пространстве (всегда не nullptr). \en Planar curve in uv-space (always not nullptr).
  MbSurface         * surface;  ///< \ru Указатель на поверхность         (всегда не nullptr). \en Pointer to the surface (always not nullptr).
  bool                closed;   ///< \ru Флаг замкнутости поверхностной кривой. \en An attribute of closedness of surface of curve. 

#ifdef C3D_SIGNAL_ENABLED
  MbObjectChangeSignalOne<MbCurve3D> _signal;  ///< \ru Сигнал об изменениях в кривой. \en Signal about the curve changes. 
#endif

  /** \brief \ru Вспомогательные данные. 
             \en Auxiliary data. \~
      \details \ru Вспомогательные данные служат для ускорения работы объекта.
               \en Auxiliary data are used for fast calculations. \n \~
  */
  mutable MbCube            cube;             ///< \ru Габаритный куб. \en Bounding box. 
  mutable atomic_double     metricLength;     ///< \ru Метрическая длина кривой. \en Metric length of the curve. \~
  mutable atomic_double     lengthEvaluation; ///< \ru Оценочная длина кривой. \en Estimated length of the curve. 
  mutable MbCartPoint3D     weightCenter;     ///< \ru Центр тяжести кривой. \en Center of mass of the curve. 
  mutable atomic_double     curveRadius;      ///< \ru Радиус кривой, если она является дугой окружности в пространстве. \en The radius of the curve, if the curve is a spatial arc. 
  mutable c3d::AtomicDoublePair         radiusAccuracy; ///< \ru Точности вычисления curveRadius. \en The precisions for the calculation of curveRadius.
  mutable atomic_enum_type<ThreeStates> isStraight;     ///< \ru Флаг прямолинейности. \en A straightness flag. 
  SPtr<MbCurveTessellation> tessellation;     ///< \ru Разбивка кривой. \en Curve tessellation. 

  //------------------------------------------------------------------------------
  /** \brief \ru Вспомогательные данные. 
             \en Auxiliary data. \~
    \details \ru Вспомогательные данные служат для ускорения работы объекта.
             \en Auxiliary data are used for fast calculations. \n \~
  */
  // ---
  class MbSurfaceCurveAuxiliaryData : public AuxiliaryData {
  public:
    double        t;    ///< \ru Модифицированный параметр. \en Modified parameter.

    MbVector      pDers[cdt_CountDer]; ///< \ru Точка и производные двумерной кривой. \en Curve point and derivatives.
    MbVector3D    sDers[sdt_CountDer]; ///< \ru Точка и производные поверхности. \en Surface point and derivatives.
    MbVector3D    sNorm;               ///< \ru Нормаль поверхности. \en Surface normal.

    MbSurfaceCurveAuxiliaryData();
    MbSurfaceCurveAuxiliaryData( const MbSurfaceCurveAuxiliaryData & );
    virtual ~MbSurfaceCurveAuxiliaryData();

    void Init();
    void Init( const MbSurfaceCurveAuxiliaryData & );
    void Move( const MbVector3D & );
  };
  
  mutable CacheManager<MbSurfaceCurveAuxiliaryData> cache;

public :
  /// \ru Конструктор кривой на поверхности. \en Constructor of curve on surface. 
  MbSurfaceCurve( const MbSurface &, const MbCurve &, bool sameCurve, MbRegDuplicate * iReg = nullptr );
  /// \ru Конструктор отрезка прямой на поверхности. \en Constructor of a line segment on surface. 
  MbSurfaceCurve( const MbSurface &, const MbCartPoint & p0, const MbCartPoint & p1, MbePlaneType type = pt_Curve );
  /// \ru Конструктор граничной кривой поверхности. \en Constructor of boundary curve of surface. 
  MbSurfaceCurve( const MbSurface &, ptrdiff_t pnt1, ptrdiff_t pnt2, MbePlaneType type = pt_Curve );
protected:
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbSurfaceCurve( const MbSurfaceCurve &, MbRegDuplicate * );
  /// \ru Конструктор копирования кривой с той же поверхностью для CurvesDuplicate(). \en Copy-constructor of a curve with the same surface for CurvesDuplicate(). 
  explicit MbSurfaceCurve( const MbSurfaceCurve * );
private:
  MbSurfaceCurve( const MbSurfaceCurve & ); // \ru Не реализовано!!! \en Not implemented!!! 

public :
  virtual ~MbSurfaceCurve();

public:
  /// \ru Реализация функции, инициирующей посещение объекта. \en Implementation of a function initializing a visit of an object. 
  VISITING_CLASS( MbSurfaceCurve );

  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */

  MbeSpaceType IsA() const override;  // \ru Дать тип элемента. \en Get element type.
  MbeSpaceType Type() const override; // \ru Дать тип элемента. \en Get element type.
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  /// \ru Копия кривой с той же поверхностью. \en Copy of curve with the same surface. 
          MbSurfaceCurve & CurvesDuplicate() const;
  bool        IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Определить, являются ли объекты одинаковыми. \en Determine whether objects are equal.
  bool        SetEqual ( const MbSpaceItem & ) override; // \ru Сделать равным. \en Make equal. 
  bool        IsSimilar( const MbSpaceItem & ) const override; // \ru Определить, являются ли объекты подобными. \en Determine whether the objects are similar. 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвинуть вдоль вектора. \en Move along a vector.
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate about an axis.
  void        AddYourGabaritTo( MbCube & ) const override; // \ru Добавить свой габарит в куб. \en Add your own bounding box into a cube. 
  void        Refresh() override; // \ru Сбросить все временные данные. \en Reset all temporary data. 
  void        PrepareIntegralData( const bool forced ) const override; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.

  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта. \en Set properties of the object. 
  void        GetBasisItems( RPArray<MbSpaceItem> & ) override; // \ru Дать базовые объекты. \en Get the basis objects. 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \} */
  /** \ru \name Общие функции кривой.
      \en \name Common functions of curve.
      \{ */

  double      GetTMin() const override;   // \ru Вернуть минимальное значение параметра. \en Get the minimum value of parameter. 
  double      GetTMax() const override;   // \ru Вернуть максимальное значение параметра. \en Get the maximum value of parameter. 
  bool        IsClosed() const override;  // \ru Проверить замкнутость кривой. \en Check for curve closedness. 
  double      GetPeriod() const override; // \ru Вернуть период периодической кривой. \en Get period of a periodic curve. 

  // \ru Функции для работы в области определения. \en Functions for working in the definition domain. 
  void        PointOn  ( double & t, MbCartPoint3D & ) const override; // \ru Вычислить точку на кривой. \en Calculate a point on the curve.
  void        FirstDer ( double & t, MbVector3D & )    const override; // \ru Вычислить первую производную. \en Calculate the first derivative.
  void        SecondDer( double & t, MbVector3D & )    const override; // \ru Вычислить вторую производную. \en Calculate the second derivative.
  void        ThirdDer ( double & t, MbVector3D & )    const override; // \ru Вычислить третью производную по t. \en Calculate the third derivative by t.
  // \ru Функции для работы вне области определения. \en Functions for working outside of definition domain. 
  void       _PointOn  ( double t, MbCartPoint3D & )  const override; // \ru Вычислить точку на расширенной кривой. \en Calculate a point on the extended curve. 
  void       _FirstDer ( double t, MbVector3D & )     const override; // \ru Вычислить первую производную. \en Calculate the first derivative. 
  void       _SecondDer( double t, MbVector3D & )     const override; // \ru Вычислить вторую производную. \en Calculate the second derivative. 
  void       _ThirdDer ( double t, MbVector3D & )     const override; // \ru Вычислить третью производную по t. \en Calculate the third derivative by t. 
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void        Explore  ( double & t, bool ext,
                                 MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const override;

  // \ru Установить параметры NURBS. \en Set parameters of NURBS. 
  MbNurbs3D * NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;

  // \ru Построить усеченную кривую c учетом заданной трехмерной точности. \en Construct a trimmed curve with the given three-dimensional accuracy.
  MbCurve3D * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyzEps, bool saveParamLenAndLaw ) const override;
  using MbCurve3D::Trimmed; // \ru Метод базового класса MbCurve3D без учета трехмерной точности. Точность по умолчанию - MbDimAccuracy::threeDimAcc.  \en Method of MbCurve3D base class without three-dimensional accuracy. The default accuracy is MbDimAccuracy::threeDimAcc.

  // \ru Вычислить ближайшую проекцию точки на кривую. \en Calculate the nearest projection of a point onto the curve. 
  bool        NearPointProjection( const MbCartPoint3D &, double & t, bool ext, MbRect1D * tRange = nullptr ) const override;
  void        Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление. \en Change the direction.

  double      Step         ( double t, double sag ) const override; // \ru Вычисление шага по стрелке прогиба. \en Calculation of parameter step by the sag.
  double      DeviationStep( double t, double angle ) const override; // \ru Вычисление шага по углу отклонения нормали. \en Calculation of parameter step by the deviation angle.   
  void        SetTesselation( const MbContourOnSurface & contour, size_t indSegment ); // \ru Установить разбиение из контура. \en Set tessellation from contour.
  double      MetricStep   ( double t, double length ) const override; // \ru Вычисление шага параметра по длине. \en Calculation of parameter step by the given length.

  MbCurve *   GetMap( const MbMatrix3D &, MbRect1D * pRgn = nullptr,
                              VERSION version = Math::DefaultMathVersion(), bool * coincParams = nullptr ) const override; // \ru Дать плоскую проекцию кривой. \en Get a planar projection of a curve.
          /// \ru Вычислить плоскую проекцию кривой в частных случаях. \en Calculate planar projection of a curve in special cases. 
          MbCurve *   GetParticularMap( const MbMatrix3D & into, MbRect1D * pRgn,
                                        VERSION version ) const;

  bool        IsStraight( bool ignoreParams = false ) const override; // \ru Определить, является ли линия прямолинейной. \en Determine whether the line is straight. 
  void        ChangeCarrier     ( const MbSpaceItem & item, MbSpaceItem & init ) override; // \ru Изменить носитель. \en Change the carrier.
  bool        ChangeCarrierBorne( const MbSpaceItem &, MbSpaceItem &, const MbMatrix & matr ) override; // \ru Изменить носимые элементы. \en Change a carrier elements.
  bool        IsPlanar( double accuracy = METRIC_EPSILON ) const override; // \ru Определить, является ли кривая плоской. Прямолинейные кривые являются плоскими, но без определённой ЛСК. \en Determine whether the curve is planar. Straight lines is planar but without certain placement.
  bool        IsSmoothConnected( double angleEps, bool ignoreLimits = false ) const override; // \ru Определить, являются ли стыки контура\кривой гладкими. \en Determine whether the joints of contour\curve are smooth. 
  void        CalculateGabarit( MbCube & ) const override; // \ru Вычислить габарит кривой. \en Calculate bounding box of a curve. 
  double      GetMetricLength() const override; // \ru Вычислить метрическую длину кривой. \en Calculate the metric length of a curve. 
  double      GetLengthEvaluation() const override; // \ru Оценить метрическую длину кривой. \en Estimate the metric length of a curve. 
  double      GetRadius( double accuracy = METRIC_REGION ) const override; // \ru Дать физический радиус объекта или ноль, если это невозможно. \en Get the physical radius of the object or null if it impossible.
  bool        GetCircleAxis  ( MbAxis3D & ) const override; // \ru Дать ось кривой. \en Get the curve axis. 
  void        GetCentre      ( MbCartPoint3D &  c ) const override; // \ru Выдать центр кривой. \en Get center of curve.
  void        GetWeightCentre( MbCartPoint3D & wc ) const override; // \ru Выдать центр тяжести кривой. \en Get center of mass of curve. 

  // \ru Вычислить ближайшую точку кривой к плейсменту. \en Calculate the curve point nearest to a placement. 
  double      DistanceToPlace( const MbPlacement3D & place, double & t0, double & angle ) const override;
  // \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская (после использования вызывать DeleteItem на двумерную кривую). \en Get the planar curve and placement if the spatial curve is planar (call DeleteItem for two-dimensional curve after using ). 
  bool        GetPlaneCurve( MbCurve *& curve2d, MbPlacement3D & place3d, bool saveParams, PlanarCheckParams params = PlanarCheckParams() ) const override;
  // \ru Дать поверхностную кривую, если пространственная кривая поверхностная (после использования вызывать DeleteItem на аргументы). \en Get a surface curve if a spatial curve is on a surface (call DeleteItem for arguments after use). 
  bool        GetSurfaceCurve( MbCurve *& curve2d, MbSurface *& surface, VERSION version = Math::DefaultMathVersion() ) const override;
  // \ru Заполнить плейсмент, если кривая плоская. \en Fill the placement if the curve is planar. 
  bool        GetPlacement( MbPlacement3D & place, PlanarCheckParams params = PlanarCheckParams() ) const override;

  /// \ru Дать тип кривой. \en Get type of curve. 
  virtual MbeCurveBuildType GetBuildType() const;
  /** \brief \ru Вставить точку по параметру.
             \en Insert point by parameter. \~
    \details \ru Вставить точку по желаемому параметру и выдать фактический параметр. \n
             \en Insert point by desirable parameter and get the actual parameter. \n \~ 
    \param[in] t - \ru Параметр на кривой, куда надо вставить точку.
                   \en Parameter on the curve where it is necessary to insert a point. \~
    \return \ru Возвращает true, если произошла вставка точки.
            \en Returns true if a point was inserted. \~
  */
  virtual bool        InsertPoint( double & t );

  /// \ru Определить, является ли объект смещением. \en Determine whether the object is a translation. 
  bool        IsShift( const MbSpaceItem &, MbVector3D &, bool & isSame, double accuracy = LENGTH_EPSILON ) const override;
  /// \ru Определить, подобные ли кривые для объединения (слива). \en Determine whether the curves for union (joining) are similar. 
  bool        IsSimilarToCurve( const MbCurve3D & other, double precision = METRIC_PRECISION ) const override;
  void        GetPointsByEvenLengthDelta( size_t n, std::vector<MbCartPoint3D> & pnts ) const override; // \ru Выдать n точек кривой с равными интервалами по длине дуги. \en Get n points of a curve with equally spaced by the arc length. 

  // \ru Определить количество разбиений для прохода в операциях. \en Define the number of splittings for one passage in operations. 
  size_t      GetCount() const override;

  /// \ru Получить границы участков кривой, которые описываются одной аналитической функцией. 
  /// \en Get the boundaries of the curve sections that are described by one analytical function. \~
  void        GetAnalyticalFunctionsBounds( std::vector<double> & params ) const override;

  bool        IsContinuousDerivative( bool & contLength, bool & contDirect, c3d::DoubleVector * params = nullptr, double epsilon = EPSILON ) const override; // \ru Непрерывна ли первая производная? \en Have the first derivative the continuous?
  // \ru Устранить разрывы первых производных по длине. \en Eliminate the discontinuities of the first derivative at length.
  bool        SetContinuousDerivativeLength( VERSION version, double epsilon = EPSILON ) override; 
  /// \ ru Определение точек излома кривой. \en The determination of curve smoothness break points.
  void        BreakPoints( std::vector<double> & vBreaks, double precision = ANGLE_REGION ) const override;

  /// \ru Продлить кривую. \en Extend the curve. \~
  MbResultType Extend( const MbCurveExtensionParameters3D & parameters, c3d::SpaceCurveSPtr & resCurve ) const override;

  /** \} */

  /// \ru Определить, является ли кривая curve копией этой кривой. \en Determine whether the 'curve' curve is a duplicate of the current curve. 
          bool        IsSameCurvePoints( const MbSurfaceCurve * scurve, double accuracy, bool sameSense ) const;
  /// \ru Вычислить нормаль к поверхности. \en Calculate surface normal. 
          void        SurfaceNormal( double & t, MbVector3D &, bool ext = false ) const;
  /// \ru Нормаль к поверхности, первая и вторая производная нормали поверхности по t. \en Calculation of the normal, first, and second derivative of the surface normal.
          void        SurfaceNormal( double & t, MbVector3D & n, MbVector3D & fd, MbVector3D & sd, bool ext = false ) const;
  /// \ru Кривизна поверхности в поперечном направлении к вектору tau. \en The surface curvature in the transverse direction to the vector tau.
          double      SurfaceTransversalCurvature( double t, const MbVector3D & tau ) const;
  /// \ru Получить параметры поверхности по параметру на кривой. \en Get surface parameters by a parameter on the curve. 
          void        SurfaceParams( double & t, double & u, double & v, bool ext = false ) const;
  /// \ru Вычислить параметрический габарит кривой. \en Calculate parametric bounding box of the curve. 
          void        CalculateUVLimits( MbRect & uvRect ) const;
  /// \ru Вычислить U-пары от V. \en Calculate U-pairs from V. 
          void        GetUPairs( double v, SArray<double> & u, SArray<double> & t ) const;
  /// \ru Вычислить V-пары от U. \en Calculate V-pairs from U. 
          void        GetVPairs( double u, SArray<double> & v, SArray<double> & t ) const;

  /// \ru Построить участок пространственной копии кривой. \en Construct a piece of a spatial curve copy. 
          MbCurve3D * MakeCurve( double t1, double t2, VERSION version ) const;
  /// \ru Построить пространственную копию кривой. \en Construct a spatial curve copy. 
          MbCurve3D * MakeCurve( VERSION version ) const;
  /// \ru Построить точную пространственную копию кривой. \en Construct the exact spatial curve copy. 
          MbCurve3D * CreateCurve() const;
  /// \ru Создать пространственную кривую по линии u, v. \en Create a spatial curve by u, v lines.
          MbCurve3D * CreateUV() const;

  /// \ru Вычислить левый перпендикуляр к кривой в плоскости поверхности. \en Calculate the left perpendicular to a curve at level of surface. 
          void        Transversal( double & t, MbVector3D & f ) const;

  /// \ru Определить, являются ли объекты подобными. \en Determine whether the objects are similar. 
          bool        IsCurveEqual ( const MbSpaceItem & ) const;
  /// \ru Сделать равной кривую. \en Make curve equal. 
          bool        SetCurveEqual( const MbSpaceItem & );

  /// \ru Дать кривую. \en Get curve. 
  const   MbCurve &   GetCurve()   const { return *curve; }
  /// \ru Дать поверхность. \en Get surface. 
  const   MbSurface & GetSurface() const { return *surface; }
  /// \ru Дать кривую. \en Get curve. 
          MbCurve &   SetCurve()
          {
            metricLength = -1.0;
            cache.Reset( true );
            return *curve;
          }
  /// \ru Дать поверхность. \en Get surface. 
          MbSurface & SetSurface()
          {
            metricLength = -1.0;
            cache.Reset( true );
            return *surface;
          }

  /// \ru Заменить кривую. \en Replace curve. 
          bool        ChangeCurve  ( const MbCurve & );
  /// \ru Заменить поверхность. \en Replace surface. 
          bool        ChangeSurface( const MbSurface & );
  /// \ru Заменить кривую и поверхность. \en Replace curve and surface. 
          bool        ChangeSurfaceCurve( const MbSurfaceCurve & );
                    
  /// \ru Установить область изменения параметра. \en Set range of parameter.
          bool        SetLimitParam( double newTMin, double newTMax );

  /// \ru Получить текущую точку на кривой по параметру. \en Get current point on a curve by a parameter. 
  virtual bool        GetCurvePoint( double & t, MbCartPoint & cPoint ) const;

#ifdef C3D_SIGNAL_ENABLED
  /// \ru Подписать указанную кривую на сигнал об изменениях текущего объекта. \en Subscribe the specified curve to a signal about changes in the current object.
          void SubscribeOnChangeSignal( MbCurve3D * c ) { _signal.Subscribe( c ); }
  /// \ru Отписать указанную кривую от сигнала об изменениях текущего объекта. \en Unsubscribe the specified curve from a signal about changes in the current object.
          void UnsubscribeOnChangeSignal( MbCurve3D * c ) { _signal.Unsubscribe( c ); }
#endif

protected:
          void        CheckParam ( double & t, bool ext ) const; // \ru Проверить и изменить при необходимости параметр. \en Check and correct parameter. 
          void        CalculatePoint ( double & t, bool ext, MbCartPoint3D & ) const; // \ru Вычислить точку на расширенной кривой. \en Calculate a point on the extended curve. 
          void        CalculateFirst ( double & t, bool ext, MbVector3D & )    const; // \ru Вычислить первую производную. \en Calculate the first derivative. 
          void        CalculateSecond( double & t, bool ext, MbVector3D & )    const; // \ru Вычислить вторую производную. \en Calculate the second derivative. 
          void        CalculateThird ( double & t, bool ext, MbVector3D & )    const; // \ru Вычислить третью производную по t. \en Calculate the third derivative by t. 
  /// \ru Попытаться создать пространственную кривую по проекционной кривой. \en Try to create a spatial curve from a projection curve. 
          MbCurve3D * TryProjection() const;
  /// \ru Вычислить точку двумерной и пространственной кривой. \en Calculate a point on two-dimensional curve and on the curve. 
          void        Explorer( double & t, bool ext, MbCartPoint & cPnt, MbCartPoint3D & pnt ) const; 
          void        Explorer( double & t, bool ext, 
                                MbCartPoint & cPnt, MbVector & cFir, MbVector & cSec, MbVector * cThird,
                                MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer, 
                                MbVector3D & uuDer, MbVector3D & vvDer, MbVector3D & uvDer, MbVector3D & nor ) const; 

  /** \brief \ru Проверить и установить замкнутость кривой.
             \en Check and set the curve closedness. \~
    \details \ru Проверить и установить признак замкнутости кривой. \n 
      Вызывать после самостоятельного изменения двумерной кривой.
             \en Check and set attribute of curve closedness. \n 
      To be called after independent changing of two-dimensional curve. \~
  */
          void        CheckClosed();

  /** \brief \ru Удалить кэши.
             \en Delete caches. \~
    \details \ru Удалить кэши. \n 
             \en Delete caches. \n 
  */
          void        CacheReset();

          double      CalcMetricLength( bool force = false ) const; // \ru Вычислить метрическую длину кривой. \en Calculate the metric length of a curve. 
          double      CalcLengthEvaluation( bool force = false ) const; // \ru Оценить метрическую длину кривой. \en Estimate the metric length of a curve. 
          void        CalcWeightCentre( MbCartPoint3D & wc, bool force = false  ) const; // \ru Выдать центр тяжести кривой. \en Get center of mass of curve. 

          /// \ru Создать приближенную пространственную кривую. \en Create an approximated spatial curve.
          MbCurve3D * CreateUVSpecial( VERSION ) const;

private:
          // \ru Объявить оператор приравнивания по ссылке, \en Declare operator of assignment by reference 
          // \ru чтобы не был вызван по умолчанию оператор приравнивания по значению \en To prevent default calling of the assignment operator by value 
          void        operator = ( const MbSurfaceCurve & ); // \ru Не реализовано!!! \en Not implemented!!! 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSurfaceCurve )

  friend class MbSurfaceIntersectionCurve;
};

IMPL_PERSISTENT_OPS( MbSurfaceCurve )

#endif // __CUR_SURFACE_CURVE_H
