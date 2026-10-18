////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Отрезок прямой в трёхмерном пространстве.
         \en Line segment in three-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_LINE_SEGMENT3D_H
#define __CUR_LINE_SEGMENT3D_H


#include <curve3d.h>


class   MbLineSegment;
class   MbLine3D;
class   MbCurveExtensionParameters;


//------------------------------------------------------------------------------
/** \brief \ru Отрезок прямой в трёхмерном пространстве.
           \en Line segment in three-dimensional space. \~
  \details \ru Отрезок прямой описывается начальной точкой point1 и конечной точкой point2.\n
    Область определения параметра отрезка располагается в пределах от нуля до единицы. 
    Начальной точке отрезка point1 соответствует параметр tmin=0, конечной точке отрезка point2 соответствует параметр tmax=1.\n
    Радиус-вектор кривой в методе PointOn(double&t,MbCartPoint3D&r) описывается векторной функцией\n
    r(t) = ((1 - t) point1) + (t point2).\n
           \en Line segment is described by the start point "point1" and the end point "point2". \n
    Domain of a line segment is the range [0, 1]. 
    The start point of line segment corresponds to parameter tmin=0, the end point of line segment corresponds to parameter tmax=1.\n
    Radius-vector of the curve in the method PointOn(double&t,MbCartPoint3D&r) is described by the vector function\n
    r(t) = ((1 - t) point1) + (t point2).\n \~
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbLineSegment3D : public MbCurve3D {
private :
  MbCartPoint3D point1; ///< \ru Начальная точка. \en Start point. 
  MbCartPoint3D point2; ///< \ru Конечная  точка. \en End point. 

public :
  MbLineSegment3D() : MbCurve3D(), point1(), point2() {}
  explicit MbLineSegment3D( const MbCartPoint3D & p1, const MbCartPoint3D & p2 );
  explicit MbLineSegment3D( const MbCartPoint3D & p, const MbVector3D & v );
  explicit MbLineSegment3D( const MbCartPoint3D & p, const MbVector3D & v, double t );
  MbLineSegment3D( const MbLine3D & initLine, double t1, double t2 );
  MbLineSegment3D( const MbLineSegment &, const MbPlacement3D & plane );
protected:
  MbLineSegment3D( const MbLineSegment3D & );
public :
  virtual ~MbLineSegment3D();

public :
  VISITING_CLASS( MbLineSegment3D );

          // \ru Установить параметры отрезка. \en Set the parameters of line segment.
  void    Init( const MbLineSegment3D & );
  void    Init( const MbCartPoint3D & p1, const MbCartPoint3D & p2 );
  void    Init( const MbCartPoint3D & p0, const MbVector3D & v0 );
  void    Init( const MbPlacement3D &,   const MbLineSegment & );
  void    Init( double t1, double t2 );

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeSpaceType  IsA() const override; // \ru Тип элемента \en Type of element 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override;
  bool    SetEqual ( const MbSpaceItem & ) override;           // \ru Сделать равным \en Make equal 
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void    GetBasisItems ( RPArray<MbSpaceItem> & ) override;  // \ru Дать базовые объекты \en Get the basis objects
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  // \ru Общие функции кривой \en Common functions of curve 

  double  GetTMin()  const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter 
  double  GetTMax()  const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter 
  bool    IsClosed() const override; // \ru Проверка замкнутости кривой \en Check for curve closedness 
  // \ru Функции кривой для работы в области определения параметрической кривой \en Functions of curve for working at parametric curve domain 
  void    PointOn  ( double & t, MbCartPoint3D & ) const override; // \ru Точка на кривой \en Point on the curve 
  void    FirstDer ( double & t, MbVector3D & )    const override; // \ru Первая производная \en First derivative 
  void    SecondDer( double & t, MbVector3D & )    const override; // \ru Вторая производная \en Second derivative 
  void    ThirdDer ( double & t, MbVector3D & )    const override; // \ru Третья производная по t \en Third derivative with respect to t 
  // \ru Функции кривой для работы вне области определения параметрической кривой \en Functions of curve for working outside the domain of parametric curve 
  void   _PointOn  ( double t, MbCartPoint3D & )  const override; // \ru Точка на расширенной кривой \en Point on the extended curve 
  void   _FirstDer ( double t, MbVector3D & )     const override; // \ru Первая производная \en First derivative 
  void   _SecondDer( double t, MbVector3D & )     const override; // \ru Вторая производная \en Second derivative 
  void   _ThirdDer ( double t, MbVector3D & )     const override; // \ru Третья производная по t \en Third derivative with respect to t 
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void    Explore( double & t, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const override;

  MbNurbs3D * NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;
  MbCurve3D * NurbsCurve( const MbNurbsParameters & ) const override; // \ru Построить Nurbs-копию кривой \en Construct NURBS copy of the curve 
  
  // \ru Построить усеченную кривую c учетом заданной трехмерной точности. \en Construct a trimmed curve with the given three-dimensional accuracy.
  MbCurve3D * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyzEps, bool saveParamLenAndLaw ) const override;
  using MbCurve3D::Trimmed; // \ru Метод базового класса MbCurve3D без учета трехмерной точности. Точность по умолчанию - MbDimAccuracy::threeDimAcc.  \en Method of MbCurve3D base class without three-dimensional accuracy. The default accuracy is MbDimAccuracy::threeDimAcc.

  void    Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction
  double  CalculateMetricLength() const override;           // \ru Метрическая длина кривой \en Metric length of a curve 
  double  GetLengthEvaluation() const override;
  double  CalculateLength( double t1, double t2 ) const override;

  void    CalculateGabarit     ( MbCube & ) const override;          // \ru Выдать габарит кривой \en Get the bounding box of curve
  void    CalculateLocalGabarit( const MbMatrix3D &, MbCube & ) const override; // \ru Рассчитать габарит относительно л.с.к. \en Calculate bounding box relative to the local coordinate system. 

  void    GetCentre      ( MbCartPoint3D & wc ) const override; // \ru Посчитать центр кривой \en Calculate a center of curve
  void    GetWeightCentre( MbCartPoint3D & wc ) const override; // \ru Посчитать центр тяжести кривой \en Calculate the gravity center of the curve
  double  Curvature( double t ) const override;               // \ru Кривизна усеченной кривой \en Curvature of a trimmed curve 
  bool    NearPointProjection  ( const MbCartPoint3D &, double & t, bool ext, MbRect1D * tRange = nullptr ) const override; // \ru Ближайшая проекция точки на кривую \en The nearest point projection to the curve

  double  Step         ( double t, double sag ) const override; // \ru Вычисление шага по стрелке прогиба. \en Calculation of parameter step by the sag.
  double  DeviationStep( double t, double angle ) const override; // \ru Вычисление шага по углу отклонения нормали. \en Calculation of parameter step by the deviation angle.   
  double  MetricStep   ( double t, double length ) const override; // \ru Вычисление шага параметра по длине. \en Calculation of parameter step by the given length. 

  MbCurve * GetMap( const MbMatrix3D &, MbRect1D * pRgn = nullptr,
                            VERSION version = Math::DefaultMathVersion(), bool * coincParams = nullptr ) const override; // \ru Дать плоскую проекцию кривой. \en Get a planar projection of curve.
  MbCurve * GetMapPsp( const MbMatrix3D &, double zNear, MbRect1D * pRgn = nullptr ) const override; // \ru Дать перспективную плоскую проекцию кривой. \en Get a planar geometric projection of curve.

  bool    IsStraight( bool ignoreParams = false ) const override; // \ru Является ли линия прямолинейной \en Whether the line is straight 
  size_t  GetCount  () const override;
  bool    IsPlanar  ( double accuracy = METRIC_EPSILON ) const override; // \ru Является ли кривая плоской \en Whether a curve is planar 
  bool    DistanceAlong( double & t, double len, int curveDir, double eps = Math::metricPrecision,
                                 VERSION version = Math::DefaultMathVersion() ) const override; // \ru Сдвинуть параметр t на расстояние len по направлению \en Shift the parameter t by the distance 'len' in the direction 
  void    CalculatePolygon( const MbStepData & stepData, MbPolygon3D & polygon ) const override; // \ru Рассчитать полигон \en Calculate a polygon
  void    GetPointsByEvenLengthDelta( size_t n,  std::vector<MbCartPoint3D> & pnts ) const override; // \ru Выдать n точек кривой с равными интервалами по длине дуги \en Get n points of curves equally spaced by the arc length 
  bool    IsSimilarToCurve( const MbCurve3D & curve, double precision = METRIC_PRECISION ) const override; // \ru Подобные ли кривые для объединения (слива) \en Whether the curves are similar for merge (joining) 

  // \ru Построить двумерный отрезок, если пространственный отрезок параллелен плоской поверхности. \en Construct a two-dimensional segment if the spatial segment is parallel to a planar surface
  bool    GetSurfaceCurve( MbCurve *& curve, MbSurface *& surface, VERSION version ) const override;

  const MbCartPoint3D & GetPoint1() const { return point1; }
  const MbCartPoint3D & GetPoint2() const { return point2; }
        MbCartPoint3D & SetPoint1()       { return point1; }
        MbCartPoint3D & SetPoint2()       { return point2; }
  void    GetPoint1( MbCartPoint3D & p ) const { p = point1; }
  void    GetPoint2( MbCartPoint3D & p ) const { p = point2; }
  void    SetPoint1( const MbCartPoint3D & p ) { point1 = p; }
  void    SetPoint2( const MbCartPoint3D & p ) { point2 = p; }
  void    SetLimitPoint( ptrdiff_t number, const MbCartPoint3D &pnt ); // \ru Заменить точку отрезка \en Replace the point of a line segment 

  // \ru Продлить кривую. \en Extend the curve. \~
  MbResultType Extend( const MbCurveExtensionParameters3D & parameters, c3d::SpaceCurveSPtr & resCurve ) const override;

  /// \ru Является ли объект смещением \en Whether the object is a shift 
  bool    IsShift ( const MbSpaceItem &, MbVector3D &, bool & isSame, double accuracy = LENGTH_EPSILON ) const override;

private:
  void    operator = ( const MbLineSegment3D & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbLineSegment3D )
};

IMPL_PERSISTENT_OPS( MbLineSegment3D )


#endif // __CUR_LINE_SEGMENT3D_H
