////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Плоская кривая в трехмерном пространстве.
         \en Plane curve in three-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_PLANE_CURVE_H
#define __CUR_PLANE_CURVE_H


#include <cur_offset_curve.h>
#include <curve3d.h>
#include <mb_placement3d.h>
#include <mb_cube.h>


class    MbContour;


//------------------------------------------------------------------------------
/** \brief \ru Плоская кривая в трехмерном пространстве.
           \en Plane curve in three-dimensional space. \~
  \details \ru Плоская кривая описывается двумерной кривой curve в плоскости XY локальной системы координат position. \n
    Радиус-вектор кривой в методе PointOn(double&t,MbCartPoint3D&r) описывается векторной функцией: \n
    r(t) = position.origin + (position.axisX point.x) + (position.axisY point.y),
    где point = curve(t);
           \en Plane curve is described by two-dimensional uv-curve in the XY-plane of the local coordinate system "position". \n
    The radius-vector of curve in the method PointOn(double&t,MbCartPoint3D&r) is described by a vector function: \n
    r(t) = position.origin + (position.axisX point.x) + (position.axisY point.y),
    where point = curve(t); \~
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbPlaneCurve : public MbCurve3D {
protected :
  MbPlacement3D position; ///< \ru Локальная система координат, в плоскости XY которой расположена кривая. \en The local coordinate system in XY plane of which the curve is located. 
  MbCurve   *   curve;    ///< \ru Двумерная кривая (не может быть nullptr). \en A two-dimensional uv-curve (can not be nullptr).

  mutable MbCube  cube;   ///< \ru Габаритный куб. \en Bounding box. 

public :
  /// \ru same = false - копировать кривую init. \en Same = false - copy the curve "init". 
  MbPlaneCurve( const MbPlacement3D &, const MbCurve & init, bool same );
protected:
  MbPlaneCurve( const MbPlaneCurve & );
public :
  virtual ~MbPlaneCurve();

public :
  VISITING_CLASS( MbPlaneCurve );
  
  void        Init( const MbPlaneCurve &init );
  void        Init( const MbPlacement3D &pl, MbCurve &initCurve );

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeSpaceType  IsA() const override; // \ru Тип элемента \en Type of element 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  bool        IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override;    // \ru Является ли объект копией \en Whether the object is a copy 
  bool        SetEqual ( const MbSpaceItem & ) override;          // \ru Сделать равным \en Make equal 
  bool        IsSimilar( const MbSpaceItem & ) const override;    // \ru Являются ли объекты подобными \en Whether the objects are similar 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;             // \ru Преобразовать \en Transform.
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;             // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis
  double      DistanceToPoint( const MbCartPoint3D & ) const override; // \ru Расстояние до точки \en Distance to a point 
  void        PrepareIntegralData( const bool forced ) const override; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.

  bool        IsSpaceSame( const MbSpaceItem & item, double eps = METRIC_REGION ) const override; // \ru Являются ли объекты идентичными в пространстве \en Are the objects identical in space? 

  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisItems ( RPArray<MbSpaceItem> & ) override; // \ru Дать базовые объекты \en Get the basis objects 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  // \ru Общие функции кривой \en Common functions of curve 

  double      GetTMax()   const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter 
  double      GetTMin()   const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter 
  bool        IsClosed()  const override; // \ru Проверка замкнутости кривой \en Check for curve closedness 
  double      GetPeriod() const override; // \ru Вернуть период \en Get period  
  // \ru Функции кривой для работы в области определения параметрической кривой \en Functions of curve for working at parametric curve domain 
  void        PointOn  ( double & t, MbCartPoint3D & ) const override; // \ru Точка на кривой \en Point on curve 
  void        FirstDer ( double & t, MbVector3D & )    const override; // \ru Первая производная \en First derivative 
  void        SecondDer( double & t, MbVector3D & )    const override; // \ru Вторая производная \en Second derivative 
  void        ThirdDer ( double & t, MbVector3D & )    const override; // \ru Третья производная по t \en Third derivative with respect to t 
  void        Normal   ( double & t, MbVector3D & )    const override; // \ru Вектор главной нормали \en Vector of the principal normal
  // \ru Функции кривой для работы вне области определения параметрической кривой \en Functions of curve for working outside the domain of parametric curve 
  void       _PointOn  ( double t, MbCartPoint3D & )   const override; // \ru Точка на расширенной кривой \en Point on the extended curve
  void       _FirstDer ( double t, MbVector3D & )      const override; // \ru Первая производная \en First derivative
  void       _SecondDer( double t, MbVector3D & )      const override; // \ru Вторая производная \en Second derivative
  void       _ThirdDer ( double t, MbVector3D & )      const override; // \ru Третья производная по t \en Third derivative with respect to t
  void       _Normal   ( double t, MbVector3D & )      const override; // \ru Вектор главной нормали \en Vector of the principal normal
    // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void        Explore( double & t, bool ext,
                       MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const override;

  MbNurbs3D * NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;
  MbCurve3D * NurbsCurve( const MbNurbsParameters & ) const override;

  // \ru Построить усеченную кривую c учетом заданной трехмерной точности. \en Construct a trimmed curve with the given three-dimensional accuracy.
  MbCurve3D * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyzEps, bool saveParamLenAndLaw ) const override;
  using MbCurve3D::Trimmed; // \ru Метод базового класса MbCurve3D без учета трехмерной точности. Точность по умолчанию - MbDimAccuracy::threeDimAcc.  \en Method of MbCurve3D base class without three-dimensional accuracy. The default accuracy is MbDimAccuracy::threeDimAcc.

  void        Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction
  bool        NearPointProjection( const MbCartPoint3D &, double & t, bool ext, MbRect1D * tRange = nullptr ) const override; // \ru Ближайшая проекция точки на кривую \en The nearest point projection to the curve
  double      Curvature( double  ) const override; // \ru Кривизна кривой \en Curvature of the curve
  double      Step         ( double t, double sag ) const override; // \ru Вычисление шага по стрелке прогиба. \en Calculation of parameter step by the sag.
  double      DeviationStep( double t, double angle ) const override; // \ru Вычисление шага по углу отклонения нормали. \en Calculation of parameter step by the deviation angle.   
  double      MetricStep   ( double t, double length ) const override; // \ru Вычисление шага параметра по длине. \en Calculation of parameter step by the given length.

  double      GetMetricLength() const override; // \ru Метрическая длина кривой \en Metric length of a curve 
  double      GetLengthEvaluation() const override; // \ru Оценка метрической длины кривой \en Evaluation of the metric length of the curve 
  double      CalculateMetricLength() const override;      // \ru Посчитать метрическую длину \en Calculate the metric length 
  double      CalculateLength( double t1, double t2 ) const override;
  // \ru Сдвинуть параметр t на расстояние len по направлению \en Shift the parameter t by the distance 'len' in the direction 
  bool        DistanceAlong( double & t, double len, int curveDir, double eps = Math::metricPrecision,
                                     VERSION version = Math::DefaultMathVersion() ) const override;   
  /// \ru Получить границы участков кривой, которые описываются одной аналитической функцией. 
  /// \en Get the boundaries of the curve sections that are described by one analytical function. \~
  void        GetAnalyticalFunctionsBounds( std::vector<double> & params ) const override;

  bool        IsPlanar( double accuracy = METRIC_EPSILON ) const override;   // \ru Является ли кривая плоской \en Whether a curve is planar
  bool        IsSmoothConnected( double angleEps, bool ignoreLimits = false ) const override; // \ru Являются ли стыки контура\кривой гладкими? \en Whether the joints of contour\curve are smooth. 
  // \ru Ближайшая точка кривой к плейсменту \en The nearest point of a curve by the placement 
  double      DistanceToPlace( const MbPlacement3D & place, double & t0, double & angle ) const override;

  MbCurve *   GetMap( const MbMatrix3D &, MbRect1D * pRgn = nullptr,
                      VERSION version = Math::DefaultMathVersion(), bool * coincParams = nullptr ) const override; // \ru Дать плоскую проекцию кривой \en Get a planar projection of curve
  MbCurve *   GetMapPsp( const MbMatrix3D &, double zNear,
                         MbRect1D * pRgn = nullptr ) const override;

  double      GetRadius( double accuracy = METRIC_REGION ) const override; // \ru Дать физический радиус объекта или ноль, если это невозможно. \en Get the physical radius of the object or null if it impossible.
  bool        GetCircleAxis  ( MbAxis3D & ) const override; // \ru Дать ось кривой \en Get the curve axis
  void        GetCentre      ( MbCartPoint3D & wc ) const override;
  void        GetWeightCentre( MbCartPoint3D & wc ) const override;
  bool        IsStraight( bool ignoreParams = false ) const override; // \ru Является ли линия прямолинейной \en Whether the line is straight 
  size_t      GetCount  () const override;
  void        GetPointsByEvenLengthDelta( size_t n,  std::vector<MbCartPoint3D> & pnts ) const override; // \ru Выдать n точек кривой с равными интервалами по длине дуги \en Get n points of curves equally spaced by the arc length

  void        AddYourGabaritTo( MbCube & ) const override; // \ru Добавить габарит кривой в куб. \en Add a bounding box of a curve to a cube. 
  void        CalculateGabarit( MbCube & ) const override; // \ru Вычислить габарит кривой \en Calculate the bounding box of curve 
  void        CalculateLocalGabarit( const MbMatrix3D &, MbCube & ) const override; // \ru Рассчитать габарит относительно л.с.к. \en Calculate bounding box relative to the local coordinate system. 

  // \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская (после использования вызывать DeleteItem на двумерную кривую) \en Give a planar curve and placement, if the spatial curve is planar (after using the DeleteItem must be called for a two-dimensional curve) 
  bool        GetPlaneCurve( MbCurve *& curve2d, MbPlacement3D & place, bool saveParams, PlanarCheckParams params = PlanarCheckParams() ) const override;
  // \ru Дать поверхностную кривую, если пространственная кривая поверхностная (после использования вызывать DeleteItem на аргументы) \en Get a surface curve if spatial curve is lying on the surface (after the using call DeleteItem for arguments) 
  bool        GetSurfaceCurve( MbCurve *& curve2d, MbSurface *& surface, VERSION version = Math::DefaultMathVersion() ) const override;
  // \ru Заполнить плейсемент, если кривая плоская \en Fill the placement if curve is planar 
  bool        GetPlacement( MbPlacement3D & place, PlanarCheckParams params = PlanarCheckParams() ) const override;

  MbCurve *   GetCurve ( const MbPlacement3D & , MbMatrix & ) const; // \ru Дать плоскую кривую \en Get the plane curve 
  MbCurve *   MakeCurve( const MbPlacement3D & ) const;
  MbCurve3D * MakeCurve() const; // \ru Дать пространственную кривую \en Get the spatial curve 

  void        SetCurve( const MbCurve & ); // \ru Заменить плоскую кривую \en Replace the plane curve 
  bool        SetLimitParam( double newTMin, double newTMax ); // \ru Установить область изменения параметра. \en Set range of parameter.
  void        SetOrigin( const MbCartPoint3D & org ) { position.SetOrigin(org); }

  const MbPlacement3D & GetPlacement() const { return position; }
  const MbCurve &   GetCurve() const { return *curve;   } // \ru Дать плоскую кривую \en Get the plane curve 
        MbCurve &   SetCurve()       { return *curve;   } // \ru Дать плоскую кривую \en Get the plane curve 

  // \ru Является ли объект смещением? \en Is the object a shift? 
  bool        IsShift( const MbSpaceItem &, MbVector3D &, bool & isSame, double accuracy = LENGTH_EPSILON ) const override;
  bool        IsSimilarToCurve( const MbCurve3D & curve, double precision = METRIC_PRECISION ) const override; // \ru Подобные ли кривые для объединения (слива) \en Whether the curves are similar for merge (joining)

  bool        IsContinuousDerivative( bool & contLength, bool & contDirect, c3d::DoubleVector * params = nullptr, double epsilon = EPSILON ) const override; // \ru Непрерывна ли первая производная? \en Have the first derivative the continuous?
  // \ru Устранить разрывы первых производных по длине. \en Eliminate the discontinuities of the first derivative at length.
  bool        SetContinuousDerivativeLength( VERSION version, double epsilon = EPSILON ) override; 
  /// \ ru Определение точек излома кривой. \en The determination of curve smoothness break points.
  void        BreakPoints( std::vector<double> & vBreaks, double precision = ANGLE_REGION ) const override;

  // \ru Продлить кривую. \en Extend the curve. \~
  MbResultType Extend( const MbCurveExtensionParameters3D & parameters, c3d::SpaceCurveSPtr & resCurve ) const override;

private:
  void        operator = ( const MbPlaneCurve & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbPlaneCurve )
};

IMPL_PERSISTENT_OPS( MbPlaneCurve )


//------------------------------------------------------------------------------
/** \brief \ru Создать пространственную кривую.
           \en Create a spatial curve. \~
  \details \ru Создать пространственную кривую как точное представление двумерной кривой на плоскости.
           \en Create a spatial curve as an accurate representation of the two-dimensional uv-curve on the plane. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbCurve3D *) MakeCurve3D( const MbCurve & curve, const MbPlacement3D & );


//-------------------------------------------------------------------------------
// \ru Создать по плоской кривой подложке пространственную кривую \en Create a spatial curve from plane curve of substrate 
// \ru (кривой должен кто-то владеть иначе она может быть уничтожена) \en (someone must own the curve otherwise it can be destroyed) 
// ---
MbCurve3D & GetCurve3DWithCheckingPlaneCur( MbCurve3D & initCur );


#endif // __CUR_PLANE_CURVE_H
