////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Отрезок прямой в двумерном пространстве.
         \en Line segment in two-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_LINE_SEGMENT_H
#define __CUR_LINE_SEGMENT_H


#include <cur_line.h>
#include <mb_rect1d.h>


class   MbLine;
class             DiskreteLengthData;


//------------------------------------------------------------------------------
/** \brief \ru Отрезок прямой в двумерном пространстве.
           \en Line segment in two-dimensional space. \~
  \details \ru Отрезок прямой описывается начальной точкой point1 и конечной точкой point2.\n
    Область определения параметра отрезка располагается в пределах от нуля до единицы. 
    Начальной точке отрезка point1 соответствует параметр tmin=0, конечной точке отрезка point2 соответствует параметр tmax=1.\n
    Радиус-вектор отрезка описывается векторной функцией\n
    r(t) = ((1 - t) point1) + (t point2).\n
           \en Line segment is described by the start point "point1" and the end point "point2". \n
    Domain of a line segment is the range [0, 1]. 
    The start point of line segment corresponds to parameter tmin=0, the end point of line segment corresponds to parameter tmax=1.\n
    Radius-vector of line segment is described by the vector function \n
    r(t) = ((1 - t) point1) + (t point2).\n \~
  \ingroup Curves_2D
*/
// ---
class MATH_CLASS MbLineSegment : public MbCurve {
private :
  MbCartPoint point1; ///< \ru Начальная точка. \en Start point. 
  MbCartPoint point2; ///< \ru Конечная  точка. \en End point. 

public :
  MbLineSegment();
  MbLineSegment( double u1, double v1, double u2, double v2 ) : MbCurve(), point1( u1, v1 ), point2( u2, v2 ) {}
  MbLineSegment( const MbCartPoint &p1, const MbCartPoint &p2 );
  MbLineSegment( const MbCartPoint &p,  const MbVector &dir, double t1, double t2 );
  MbLineSegment( const MbCartPoint &initP, double x1, double x2 );
  MbLineSegment( const MbLine & line, double t1, double t2, int sense );
  MbLineSegment( const MbLineSegment & lseg, double t1, double t2 );
  MbLineSegment( double t1, double t2, double s, bool bTIsX );
//protected:
  MbLineSegment( const MbLineSegment & );
public :
  virtual ~MbLineSegment();

public :
  VISITING_CLASS( MbLineSegment ); 

  /** \ru \name Функции инициализации отрезка.
      \en \name Line segment initialization functions.
      \{ */
  // \ru Установить параметры отрезка \en Set the parameters of line segment 
  void        Init( const MbLineSegment & );
  void        Init( const MbCartPoint &p1, const MbCartPoint &p2 );
  void        Init( const MbCartPoint &pnt, double x1, double x2 );
  void        Init( double t1, double t2 );
  void        Init1( const MbCartPoint &p1, const MbCartPoint &p2, double &len, double &angle );
  void        Init2( const MbCartPoint &p1, MbCartPoint &p2, const double &len, double &angle );
  void        Init3( const MbCartPoint &p1, MbCartPoint &p2, double &len, const double &angle,
                     const DiskreteLengthData * diskrData = nullptr );
  void        Init4( MbCartPoint &p1, const MbCartPoint &p2, const double &len, double &angle );
  void        Init5( MbCartPoint &p1, const MbCartPoint &p2, double &len, const double &angle,
                     const DiskreteLengthData * diskrData = nullptr );
  void        Init6( const MbCartPoint &p1, MbCartPoint &p2, const double &len, const double &angle );
  void        Init7( MbCartPoint &p1, const MbCartPoint &p2, const double &len, const double &angle );
  void        Init8( MbCartPoint &p1, MbCartPoint &p2, double &len, double &angle,
                     const DiskreteLengthData & diskrData, bool correctP1 );
  void        Init9( const MbCartPoint & p1, MbCartPoint & p2, double & len, double & angle,
                     const DiskreteLengthData & diskrData, bool keepX );

  /** \} */
  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */
  MbePlaneType  IsA() const override; // \ru Тип элемента \en Type of element
  bool        IsSame ( const MbPlaneItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Является ли кривая curve копией данной кривой ? \en Whether the 'curve' curve is duplicate of current curve. 
  bool        SetEqual( const MbPlaneItem & ) override;     // \ru Сделать элементы равными \en Make equal elements
  MbPlaneItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  void        Transform( const MbMatrix &, MbRegTransform * ireg = nullptr, const MbSurface * newSurface = nullptr ) override;  // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move( const MbVector &, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate( const MbCartPoint & pnt, const MbDirection & angle, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Поворот \en Rotation
  void        AddYourGabaritTo ( MbRect & ) const override;    // \ru Добавь свой габарит в прямой прям-к \en Add bounding box into a straight box 
  void        CalculateGabarit ( MbRect & ) const override;              // \ru Определить габариты кривой \en Determine the bounding box of the curve
  void        CalculateLocalGabarit( const MbMatrix & into, MbRect & local ) const override; // \ru Добавь в прям-к свой габарит с учетом матрицы \en Add bounding box into a box with consideration of the matrix
  double      DistanceToPoint( const MbCartPoint & to ) const override; // \ru Расстояние до точки \en Distance to a point
  bool        IsVisibleInRect( const MbRect & r, bool exact = false ) const override; // \ru Виден ли объект в заданном прямоугольнике \en Whether the object is visible in the given rectangle
  using      MbCurve::IsVisibleInRect;      
  bool        IsCompleteInRect( const MbRect & r ) const override;  // \ru Виден ли объект полностью в в заданном прямоугольнике \en Whether the object is entirely visible in the given rectangle
  /** \} */

  /** \ru \name Функции описания области определения кривой.
      \en \name Functions for curve domain description.
      \{ */
  double      GetTMin ()     const override; // \ru Вернуть минимальное  значение параметра \en Get the minimum value of parameter
  double      GetTMax ()     const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter
  bool        IsClosed()     const override; // \ru Проверка замкнутости \en Check for closedness
  bool        IsDegenerate( double eps = Math::LengthEps ) const override; // \ru Проверка вырожденности \en Check for degeneracy
  /** \} */

  /** \ru \name Функции для работы в области определения кривой. 
            Функции PointOn, FirstDer, SecondDer, ThirdDer,... корректируют параметр
            при выходе его за пределы области определения.
      \en \name Functions for working in the curve's domain. 
            Functions: PointOn, FirstDer, SecondDer, ThirdDer,... correct the parameter
            when it is outside domain.
      \{ */
  void        PointOn  ( double & t, MbCartPoint & p  ) const override; // \ru Точка на кривой \en Point on curve 
  void        FirstDer ( double & t, MbVector    & fd ) const override; // \ru Первая производная \en First derivative 
  void        SecondDer( double & t, MbVector    & sd ) const override; // \ru Вторая производная \en Second derivative 
  void        ThirdDer ( double & t, MbVector    & td ) const override; // \ru Третья производная \en Third derivative 
  /** \} */

  /** \ru \name Функции для работы внутри и вне области определения кривой. 
            Функции _PointOn, _FirstDer, _SecondDer, _ThirdDer,... не корректируют параметр
            при выходе за пределы области определения. При выходе за область определения незамкнутая
            кривая, в общем случае, продолжается по касательной, которую она имеет на соответствующем конце.
      \en \name Functions for working inside and outside the curve's domain. 
            Functions _PointOn, _FirstDer, _SecondDer, _ThirdDer,... do not correct parameter
            when it is out of domain bounds. When parameter is out of domain bounds, an unclosed
            curve is extended by tangent vector at corresponding end point in general case.
      \{ */
  void       _PointOn  ( double t, MbCartPoint & p ) const override;  // \ru Точка на кривой или на её продолжении \en Point on the curve or on its extension
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
      \en \name Function of moving by curve
    \{ */
  double      Step( double t, double sag ) const override; // \ru Вычисление шага аппроксимации с учетом радиуса кривизны \en Calculation of approximation step with consideration of curvature radius
  double      DeviationStep( double t, double angle ) const override; // \ru Вычисление шага аппроксимации по угловой толерантности \en Calculation of approximation step by angular tolerance

  /** \} */
  /** \ru \name Общие функции кривой
      \en \name Common function of curve.
      \{ */
  double      Curvature( double t ) const override; // \ru Кривизна усеченной кривой \en Curvature of a trimmed curve
  void        Inverse( MbRegTransform * iReg = nullptr ) override;  // \ru Изменить направление кривой \en Change direction of a curve

  MbCurve *   Offset( double rad ) const override; // \ru Смещение отрезка \en Shift of a line segment
  // \ru Построить усеченную кривую c учетом заданной двумерной точности. \en Construct a trimmed curve with the given two-dimensional accuracy.
  MbCurve *   Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyEps, bool saveParamLenAndLaw ) const override;
  using MbCurve::Trimmed; // \ru Метод базового класса MbCurve без учета двумерной точности. Точность по умолчанию - MbDimAccuracy::twoDimAcc.  \en Method of MbCurve base class without two-dimensional accuracy. The default accuracy is MbDimAccuracy::twoDimAcc.

  MbNurbs *   NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;
  MbCurve *   NurbsCurve( const MbNurbsParameters & ) const override; // \ru Построить Nurbs-копию кривой \en Construct NURBS copy of the curve 

  MbeState    DeletePart( double t1, double t2, MbCurve *& part2 ) override; // \ru Удалить часть усеченной кривой между параметрами t1 и t2 \en Delete a part of a trimmed curve between parameters t1 and t2
  MbeState    TrimmPart( double t1, double t2, MbCurve *& part2 ) override;  // \ru Оставить часть усеченной кривой между параметрами t1 и t2 \en Keep a part of the trimmed curve between parameters t1 and t2
  // \ru Выдать характерную точку усеченной кривой если она ближе чем dmax \en Get characteristic point of trimmed curve if it is closer than dmax 
  bool        GetSpecificPoint( const MbCartPoint & from, double & dmax, MbCartPoint & pnt ) const override;
  MbeState    Deformation( const MbRect &, const MbMatrix & ) override;  // \ru Деформация \en Deformation
  bool        IsInRectForDeform( const MbRect & r ) const override;      // \ru Виден ли объект в заданном прямоугольнике для деформации \en Whether the object is visible in the specified rectangle for the deformation

  bool        IsStraight( bool ignoreParams = false ) const override;    // \ru Является ли линия прямолинейной \en Whether the line is straight
  // \ru Определить, являются ли кривая инверсно такой же. \en Define whether an inversed curve is the same. 
  bool        IsInverseSame( const MbCurve & curve, double accuracy = LENGTH_EPSILON ) const override;

  // \ru Положение точки относительно кривой. \en The point position relative to the curve. 
  // \ru iloc_InItem = 1 - точка находится слева по направлению, \en Iloc_InItem = 1 - the point is on the left, 
  // \ru iloc_OnItem = 0 - точка находится по направлению, \en Iloc_OnItem = 0 - the point is on the direction, 
  // \ru iloc_OutOfItem = -1 - точка находится справа по направлению. \en Iloc_OutOfItem = -1 - the point is on the right. 
  MbeItemLocation PointRelative( const MbCartPoint & pnt, double eps = Math::LengthEps ) const override;
  double      PointProjection( const MbCartPoint & pnt ) const override;  // \ru Проекция точки на отрезок \en Point projection on the line segment
  bool        NearPointProjection( const MbCartPoint & pnt, double xEpsilon, double yEpsilon,
                                   double & t, bool ext, MbRect1D * tRange = nullptr ) const override; // \ru Проекция точки на кривую или её продолжение в области поиска проекции \en Point projection on the curve or its extension in the projection search area
  void        PerpendicularPoint( const MbCartPoint & pnt, SArray<double> & tFind ) const override; // \ru Вычисление всех перпендикуляров к кривой из данной точки \en Calculation of all perpendiculars to the curve from a given point
  void        IntersectHorizontal( double y, SArray<double> & cross ) const override; // \ru Пересечение с горизонтальной прямой \en Intersection with the horizontal line
  void        IntersectVertical  ( double x, SArray<double> & cross ) const override; // \ru Пересечение с вертикальной прямой \en Intersection with the vertical line

  bool        HasLength( double & length ) const override;
  double      GetMetricLength() const override;  // \ru Метрическая длина \en The metric length 
  double      GetLengthEvaluation() const override; // \ru Оценка метрической длины кривой \en Evaluation of the metric length of the curve
  // \ru Вычисление минимальной длины кривой между двумя точками на ней \en Calculation of minimal length of a curve between two points on it 
  double      LengthBetween2Points( MbCartPoint & p1, MbCartPoint & p2, MbCartPoint * pc = nullptr ) const override;

  bool        DistanceAlong( double & t, double len, int curveDir, double eps = Math::LengthEps,
                             VERSION version = Math::DefaultMathVersion() ) const override;
  double      CalculateLength( double t1, double t2 ) const override; // \ru Посчитать метрическую длину отрезка от параметра t1 до t2 с заданной точностью \en Coclculate the metric length of the line segment from parameter 't1' to 't2' with the given tolerance
  bool        GetMiddlePoint ( MbCartPoint & ) const override; // \ru Выдать среднюю точку отрезка \en Get the middle point on a line segment
  bool        GetCentre      ( MbCartPoint & ) const override; // \ru Выдать центр отрезка \en Get the center of a line segment
  bool        GetWeightCentre( MbCartPoint & ) const override; // \ru Выдать центр тяжести кривой \en Get the center of gravity of the curve
  bool        IsSimilarToCurve( const MbCurve & curve, double precision = PARAM_PRECISION ) const override; // \ru Подобные ли кривые для объединения (слива) \en Whether the curves are similar for merge (joining)
  size_t      GetCount() const override; // \ru Количество разбиений для прохода в операциях \en The number of partitions for passage in the operations
  void        GetPointsByEvenLengthDelta( size_t n,  std::vector<MbCartPoint> & pnts ) const override; // \ru Выдать n точек кривой с равными интервалами по длине дуги \en Get n points of curves equally spaced by the arc length

  void        GetProperties( MbProperties & properties ) override;  // \ru Выдать свойства объекта \en Get properties of the object
  void        SetProperties( const MbProperties & properties ) override;  // \ru Записать свойства объекта \en Set properties of the object
  void        GetBasisPoints( MbControlData & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  void        ThroughPoint( double t, const MbCartPoint & pnt ); // \ru Пройти через точку при данном параметре \en Pass through the point in the given parameter 
  void        InsertPoint( double t,  const MbCartPoint & pnt ); // \ru Вставить точку \en Insert a point 
  double      GetAngle() const; // \ru Выдать значение угла наклона отрезка \en Get the value of an angle inclination of a line segment 
  
  MbDirection GetDirection() const; // \ru Выдать вектор наклона отрезка \en Get the vector of a line segment inclination 
  bool        IsParallel( const MbLineSegment & seg, double eps = Math::AngleEps ) const; // \ru Проверка параллельности двух прямых \en Check for parallelism of two lines 
  const MbCartPoint & GetPoint1() const { return point1; }
  const MbCartPoint & GetPoint2() const { return point2; }
        MbCartPoint & SetPoint1()       { return point1; }
        MbCartPoint & SetPoint2()       { return point2; }
  void        GetPoint1( MbCartPoint & p ) const { p = point1; }
  void        GetPoint2( MbCartPoint & p ) const { p = point2; }
  void        SetPoint1( const MbCartPoint & p ) { point1 = p; }
  void        SetPoint2( const MbCartPoint & p ) { point2 = p; }
  void        SetLimitPoint( ptrdiff_t number, const MbCartPoint & pnt ); // \ru Заменить точку отрезка \en Replace the point of a line segment 
  void        CheckParameter( double & t ) const; // \ru Проверка и коррекция параметра \en Check and correction of parameter 

  // \ru Работа с базовой прямой \en Work with the base line 
  MbCartPoint Origin() const { MbCartPoint p( point1 ); return p; }
  MbVector    Derive() const { MbVector v( point1, point2 ); return v; }
  MbDirection Direction() const { MbDirection v( point1, point2 ); return v; }
  bool        Extend( const MbCartPoint & point ); // \ru Удлинить отрезок до проекции точки point \en Extend line segment to projection of point "point" 
  double      PointProjectionOnBaseLine( const MbCartPoint & pnt ) const; // \ru Проекция на прямую \en Projection on the line 
  double      PointProjectionOnBaseLine( const MbCartPoint & pnt, MbCartPoint & proj ) const; // \ru Проекция на прямую \en Projection on the line 
  double      DistanceToPointOnBaseLine( const MbCartPoint & pnt ) const; // \ru Расстояние от точки до проекции на прямую \en Distance from a point to a projection on the line 
  bool        IsHorizontal( double eps = Math::paramEpsilon ) const { return ::fabs(point1.y - point2.y) < eps; } // \ru Проверка горизонтальности \en Check for horizontality 
  bool        IsVertical  ( double eps = Math::paramEpsilon ) const { return ::fabs(point1.x - point2.x) < eps; } // \ru Проверка вертикальности \en Check for verticality 

  // \ru Продлить кривую. \en Extend the curve. \~
  MbResultType Extend( const MbCurveExtensionParameters & parameters, c3d::PlaneCurveSPtr & resCurve ) const override;

//private:
  const   MbLineSegment & operator = ( const MbLineSegment & ); // \ru Реализовано \en Implemented 

  /** \} */

  void        ReadAsLineSeg( reader & in ); // \ru Чтение. 
  void        WriteAsLineSeg( writer & out ) const; // \ru Запись. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbLineSegment )
}; // MbLineSegment

IMPL_PERSISTENT_OPS( MbLineSegment )


//------------------------------------------------------------------------------
// \ru Инициализировать по отрезку \en Initialize by a line segment 
// ---
inline void MbLineSegment::Init( const MbLineSegment & ls ) {
  point1 = ls.point1;
  point2 = ls.point2;
}


//------------------------------------------------------------------------------
// \ru Пересчитать параметры отрезка \en Recalculate the parameters of the line segment 
// ---
inline void MbLineSegment::Init( const MbCartPoint & p1, const MbCartPoint & p2 ) {
  point1 = p1;
  point2 = p2;
}


//------------------------------------------------------------------------------
// \ru Инициализация горизонтального отрезка для штриховки \en Initialization of a horizontal segment for hatching 
// ---
inline void MbLineSegment::Init( const MbCartPoint & pnt, double x1, double x2 ) {
  point1 = pnt;
  point2 = pnt;
  point1.x += x1;
  point2.x += x2;
}


//------------------------------------------------------------------------------
// \ru Заменить точку отрезка \en Replace the point of a line segment 
// ---
inline void MbLineSegment::SetLimitPoint( ptrdiff_t number, const MbCartPoint & pnt )
{
  if ( number == 1 )  // \ru Меняем 1-ую точку \en Change the first point 
    point1 = pnt;
  else
    point2 = pnt;
}


//------------------------------------------------------------------------------
// \ru Выдать значение угла наклона отрезка \en Get the value of an angle inclination of a line segment 
// ---
inline double MbLineSegment::GetAngle() const {
  MbDirection d0( point1, point2 );
  return d0.DirectionAngle();
}


//------------------------------------------------------------------------------
// \ru Выдать вектор наклона отрезка \en Get the vector of a line segment inclination 
// ---
inline MbDirection MbLineSegment::GetDirection() const {
  MbDirection d0( point1, point2 );
  return d0;
}


//------------------------------------------------------------------------------
// \ru Проверка параллельности двух прямых \en Check for parallelism of two lines 
// ---
inline bool MbLineSegment::IsParallel( const MbLineSegment & seg, double eps ) const {
  if ( &seg == this )
    return true;
  MbDirection d0( point1, point2 );
  MbDirection d1( seg.point1, seg.point2 );
  return ( ::fabs( d0.ay * d1.ax - d0.ax * d1.ay ) < eps );
}


//------------------------------------------------------------------------------
// \ru Проверка и коррекция параметра \en Check and correction of parameter 
// ---
inline void MbLineSegment::CheckParameter( double & t ) const {
  if ( t < 0 ) t = 0;
  if ( t > 1 ) t = 1;
}


//------------------------------------------------------------------------------
// \ru Инициализация по другому отрезку \en Initialization by another segment 
// ---
inline const MbLineSegment & MbLineSegment::operator = ( const MbLineSegment & other ) {
  point1 = other.point1;
  point2 = other.point2;

  return *this;
}


#endif // __CUR_LINE_SEGMENT_H
