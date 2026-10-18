////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Прямая в двумерном пространстве.
         \en Line in two-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_LINE_H
#define __CUR_LINE_H


#include <curve.h>


class MbRegDuplicate;
class MbRegTransform;


//------------------------------------------------------------------------------
/** \brief \ru Прямая в двумерном пространстве.
           \en Line in two-dimensional space. \~
  \details \ru Прямая линия ведёт себя как бесконечный объект, хотя в своих данных имеет граничные значения параметра tmin и tmax. 
    В отличие от других кривых в методах вычисления радиуса-вектора и его производных прямая не корректирует параметр t при его выходе за предельные значения tmin и tmax. \n
    Радиус-вектор прямой описывается векторной функцией \n
    r(t) = origin + (t direction).
           \en Straight line behaves as an infinite object although its data has boundary parameter values tmin and tmax. 
    In contrast to curves in the calculation methods of radius-vector and its derivatives the line doesn't correct parameter "t" if it is outside the values tmin and tmax. \n
    Radius-vector of line is described by the vector function \n
    r(t) = origin + (t direction). \~
  \ingroup Curves_2D
*/
// ---
class MATH_CLASS MbLine : public MbCurve {
private :
  MbCartPoint origin;    ///< \ru Начальная точка. \en Start point. 
  MbDirection direction; ///< \ru Вектор направления. \en A direction vector. 

public :
  MbLine();
  MbLine( const MbCartPoint & initP, const MbDirection & initDirect );
  MbLine( const MbCartPoint & initP, const MbVector & initV );
  MbLine( const MbCartPoint & initP, double angle ); // \ru Прямая по точке и углу \en Line by a point and angle 
  MbLine( const MbCartPoint & p1, const MbCartPoint & p2 ); // \ru Прямая по двум точкам (в случае совпадения точек принимается горизонтальное направление) \en Line by two points (in the case of points coincidence the horizontal direction is taken) 
  MbLine( double a, double b, double c ); // \ru Инициализация прямой по коэффициентам \en Initialization of a line by coefficients 
//protected :
  MbLine( const MbLine & );
public :
  virtual ~MbLine();

public :
  VISITING_CLASS( MbLine ); 

  /** \ru \name Функции инициализации прямой.
      \en \name Line initialization functions.
      \{ */
          // \ru Различные варианты инициализации прямой \en Different variants for the initialization of line 
  void        Init( const MbLine & other ) { origin = other.origin; direction = other.direction; }
  void        Init( const MbCartPoint & pnt, double angle )            { origin = pnt; direction = angle; }
  void        Init( const MbCartPoint & pnt, const MbDirection & dir ) { origin = pnt; direction = dir; }
  void        Init( const MbCartPoint & pnt, const MbVector & dir )    { origin = pnt; direction = dir; }
  void        Init( const MbCartPoint & p1, const MbCartPoint & p2 )   { origin = p1;  direction.Calculate( p1, p2 ); }
  void        Init( double a, double b, double c ); // \ru Инициализация прямой по коэффициентам \en Initialization of a line by coefficients 

  /** \} */
  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of geometric object.
      \{ */
  MbePlaneType  IsA()    const override; // \ru Тип элемента \en Type of element
  bool        IsSimilar( const MbPlaneItem & ) const override;    // \ru Являются ли элементы подобными \en Whether the elements are similar
  bool        SetEqual ( const MbPlaneItem & ) override;       // \ru Сделать элементы равными \en Make equal elements
  bool        IsSame   ( const MbPlaneItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Является ли кривая curve копией данной кривой ? \en Whether the 'curve' curve is duplicate of current curve.
  void        Transform( const MbMatrix &, MbRegTransform * ireg = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector &, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate( const MbCartPoint & pnt, const MbDirection & angle, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Поворот \en Rotation

  MbPlaneItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  void        AddYourGabaritTo ( MbRect & ) const override;; // \ru Добавь свой габарит в прямой прям-к \en Add bounding box into a straight box
  void        AddYourGabaritMtr( MbRect &, const MbMatrix & ) const override; // \ru Добавь в прям-к свой габарит с учетом матрицы \en Add bounding rectangle into a box with consideration of the matrix
  
  void        CalculateGabarit ( MbRect & ) const override;; // \ru Рассчитать габарит кривой.  \en Calculate bounding rectangle.
  void        CalculateLocalGabarit( const MbMatrix & into, MbRect & local ) const override; // \ru Рассчитать габарит в локальной системы координат.  \en Calculate bounding rectangle into local coordinate system.

  bool        IsVisibleInRect( const MbRect &, bool exact = false ) const override; // \ru Виден ли объект в заданном прям-ке \en Whether the object is visible in the given rectangle
  using       MbCurve::IsVisibleInRect;
  double      DistanceToPoint( const MbCartPoint & ) const override; // \ru Расстояние до точки \en Distance to a point
  bool        DistanceToPointIfLess( const MbCartPoint & toP, double & d ) const override; // \ru Вычислить расстояние до точки, если оно меньше d. \en Calculate the distance to the point if it is less than d.
  /** \} */

  /** \ru \name Функции описания области определения кривой.
      \en \name Functions describing the domain of a curve.
      \{ */
  double      GetTMin() const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter
  double      GetTMax() const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter
  /** \} */

  /** \ru \name Функции для работы в области определения кривой. 
            Функции _PointOn, _FirstDer, _SecondDer, _ThirdDer,... не корректируют параметр
            при выходе за пределы области определения. При выходе за область определения незамкнутая
            кривая, в общем случае, продолжается по касательной, которую она имеет на соответствующем конце.
      \en \name Functions for working in the domain of a curve. 
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
      \en \name Function for working inside and outside of the curve domain. 
            Function _PointOn, _FirstDer, _SecondDer, _ThirdDer,... do not correct a parameter
            when it is outside domain. If non-closed curve is outside of the domain
            in the general case it continues along a tangent, which it has at the respective end.
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

  /** \ru \name Общие функции кривой
      \en \name Common functions of curve
      \{ */
  MbCurve  *   Offset( double rad ) const override;  // \ru Смещение прямой \en Shift of line

  // \ru Удалить часть прямой между параметрами t1 и t2 \en Remove a part of the line between t1 and t2 parameters 
  MbeState    DeletePart( double t1, double t2, MbCurve *& part2 ) override;

  // \ru Оставить часть прямой между параметрами t1 и t2 \en Save a part of the line between t1 and t2 parameters 
  MbeState    TrimmPart( double t1, double t2, MbCurve *& part2 ) override;
  // \ru Построить усеченную кривую c учетом заданной двумерной точности. \en Construct a trimmed curve with the given two-dimensional accuracy.
  MbCurve *   Trimmed  ( double t1, double t2, int sense, const MbDimAccuracy & xyEps, bool saveParamLenAndLaw ) const override;
  using MbCurve::Trimmed; // \ru Метод базового класса MbCurve без учета двумерной точности. Точность по умолчанию - MbDimAccuracy::twoDimAcc.  \en Method of MbCurve base class without two-dimensional accuracy. The default accuracy is MbDimAccuracy::twoDimAcc.

  bool        HasLength( double & ) const override; // \ru Метрическая длина \en The metric length
  double      GetMetricLength() const override;     // \ru Метрическая длина кривой \en Metric length of a curve

  // \ru Вычисление минимальной длины кривой между двумя точками на ней \en Calculation of minimal length of a curve between two points on it 
  virtual double      LengthBetween2Points( MbCartPoint & p1, MbCartPoint & p2,
                                            MbCartPoint * pc = nullptr ) const override;

  bool        DistanceAlong( double & t, double len, int curveDir, double eps = Math::LengthEps,
                             VERSION version = Math::DefaultMathVersion() ) const override; // \ru Сдвинуть параметр t на расстояние len по направлению \en Shift the parameter t by the distance 'len' in the direction

  double      Step( double t, double sag ) const override; // \ru Вычисление шага аппроксимации \en Calculation of approximation step
  double      DeviationStep( double t, double angle ) const override; // \ru Вычисление шага аппроксимации с учетом угла отклонения \en Calculation of approximation step with consideration of deviation angle

  double      DistanceToPointSign( const MbCartPoint & to ) const; // \ru Расстояние от прямой до точки со знаком \en Signed distance from the line to the point 

  // \ru Положение точки относительно кривой. \en The point position relative to the curve. 
  // \ru iloc_InItem = 1 - точка находится слева от прямой, \en Iloc_InItem = 1 - point is located to the left of the line, 
  // \ru iloc_OnItem = 0 - точка находится на прямой, \en Iloc_OnItem = 0 - point is located on the line, 
  // \ru iloc_OutOfItem = -1 - точка находится справа от прямой. \en Iloc_OutOfItem = -1 - point is located to the right of the line. 
  MbeItemLocation PointRelative  ( const MbCartPoint & p, double eps = Math::LengthEps ) const override;
  double      PointProjection    ( const MbCartPoint & ) const override; // \ru Проекция точки на кривую \en Point projection on the curve
  bool        NearPointProjection( const MbCartPoint & pnt, double xEpsilon, double yEpsilon, 
                                   double & t, bool ext, MbRect1D * tRange = nullptr ) const override; // \ru Проекция точки на кривую или её продолжение в области поиска проекции \en Point projection on the curve or its extension in the projection search area
  // \ru Вычисление всех перпендикуляров к кривой из данной точки \en Calculation of all perpendiculars to the curve from a given point 
  void        PerpendicularPoint( const MbCartPoint & pnt, SArray<double> & tFind ) const override;

  bool        GetMiddlePoint ( MbCartPoint & ) const override;       // \ru Выдать среднюю точку кривой \en Calculate a middle point on a curve
  bool        GetWeightCentre( MbCartPoint & ) const override;       // \ru Выдать центр прямой \en Get the center of line

  bool        operator == ( const MbLine & ) const; // \ru Проверка на равенство \en Check for equality 
  bool        operator != ( const MbLine & ) const; // \ru Проверка на неравенство \en Check for inequality 

  bool        IsHorizontal( double eps = Math::AngleEps ) const { return ::fabs( direction.ay ) < eps; } // \ru Проверка горизонтальности \en Check for horizontality 
  bool        IsVertical  ( double eps = Math::AngleEps ) const { return ::fabs( direction.ax ) < eps; } // \ru Проверка вертикальности \en Check for verticality 

  bool        IsSimilar           ( const MbLine & ) const; // \ru Проверка одинаковости двух прямых \en Check for sameness of two lines 
  bool        IsParallel( const MbLine & other, double epsilon = Math::AngleEps ) const; // \ru Проверка параллельности двух прямых \en Check for parallelism of two lines 
  double      DistanceToParallel( const MbLine & ) const; // \ru Расстояние до параллельной прямой \en The distance to the parallel line 

  void        IntersectHorizontal( double y, SArray<double> & cross ) const override; // \ru Пересечение с горизонтальной прямой \en Intersection with the horizontal line
  void        IntersectVertical  ( double x, SArray<double> & cross ) const override; // \ru Пересечение с вертикальной прямой \en Intersection with the vertical line

  void        Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменение направления кривой на противоположное \en Change to the opposite direction of a curve
  bool        IsClosed()   const override; // \ru Проверка замкнутости \en Check for closedness
  bool        IsBounded()  const override; // \ru Определить, является ли кривая ограниченной. \en Define whether the curve is bounded.
  bool        IsStraight( bool ignoreParams = false ) const override; // \ru Признак прямолинейности кривой \en An attribute of curve straightness

  // \ru Дать приращение параметра, соответствующее единичной длине в пространстве \en Get increment of parameter, corresponding to the unit length in space 
  double      GetParamToUnit() const override;
  double      GetParamToUnit( double t ) const override;

  void        SetPoint( const MbCartPoint & pnt ) { origin = pnt; } // \ru Установить новую базовую точку \en Set the new base point 
  void        GetPoint( MbCartPoint & pnt ) const { pnt = origin; } // \ru Выдать базовую точку \en Get the base point 
  void        GetDirection( MbDirection & dir ) const { dir = direction; } // \ru Выдать вектор наклона прямой \en Get the vector of a line inclination 
  void        SetDirection( const MbDirection & dir ) { direction = dir; } // \ru Установить вектор наклона прямой \en Set the vector of a line inclination 
  void        SetDirection( const MbVector    & v   ) { direction = v; }

  void        SetAngle( double angle ) { direction = angle; } // \ru Установить новый угол \en Set the new angle 
  double      GetAngle() const { return direction.DirectionAngle(); } // \ru Выдать значение угла наклона \en Get the value of an angle inclination 

  // \ru Создать NURBS представление кривой \en Create a NURBS representation of the curve 
  MbNurbs *   NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;
  MbCurve *   NurbsCurve( const MbNurbsParameters & ) const override; // \ru Построить Nurbs-копию кривой \en Construct NURBS copy of the curve
  MbContour * NurbsContour() const override;
  size_t      GetCount() const override; // \ru Количество разбиений для прохода в операциях \en The number of partitions for passage in the operations

  ptrdiff_t   IntersectRect( MbRect & rect, MbCartPoint * cross ) const; // \ru Пересечение прямой с прямоугольником \en Intersection of a line with rectangle 
  void        Implicit( double & A, double & B, double & C ) const; // \ru Выдать коэффициенты неявного представления \en Get coefficients of implicit representation 

  const MbCartPoint & GetOrigin()    const { return origin; }
  const MbDirection & GetDirection() const { return direction; }
  MbCartPoint & SetOrigin()    { return origin; }
  MbDirection & SetDirection() { return direction; }

  MbCartPoint   Origin() const { MbCartPoint p( origin ); return p; }
  MbVector      Derive() const { MbVector v( direction.ax, direction.ay ); return v; }      

  void        GetProperties(       MbProperties & ) override;  // \ru Выдать свойства объекта \en Get properties of the object
  void        SetProperties( const MbProperties & ) override;  // \ru Записать свойства объекта \en Set properties of the object
  void        GetBasisPoints(       MbControlData & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \} */

private:

  void        operator = ( const MbLine & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbLine )
}; // MbLine

IMPL_PERSISTENT_OPS( MbLine )


//------------------------------------------------------------------------------
// \ru Расстояние от прямой до точки со знаком \en Signed distance from the line to the point 
// ---
inline double MbLine::DistanceToPointSign( const MbCartPoint & to ) const {
  return direction.ax * ( to.y - origin.y ) - direction.ay * ( to.x - origin.x );
}


//------------------------------------------------------------------------------
// \ru Проверка на равенство \en Check for equality 
// ---
inline bool MbLine::operator == ( const MbLine & with ) const {
  return (origin == with.origin) && ( direction.Colinear( with.direction ) );
}


//------------------------------------------------------------------------------
// \ru Проверка на неравенство \en Check for inequality 
// ---
inline bool MbLine::operator != ( const MbLine & with ) const {
  return !(*this == with);
}


//------------------------------------------------------------------------------
// \ru Проверка параллельности двух прямых \en Check for parallelism of two lines 
// ---
inline bool MbLine::IsParallel( const MbLine & other, double epsilon ) const {
  return fabs( direction.ay * other.direction.ax -
               direction.ax * other.direction.ay ) < epsilon;
}


//------------------------------------------------------------------------------
// \ru Проверка одинаковости двух прямых \en Check for sameness of two lines 
// ---
inline bool MbLine::IsSimilar( const MbLine & other ) const {
  return IsParallel( other ) && fabs( DistanceToPointSign( other.origin ) ) < Math::LengthEps;
}


//------------------------------------------------------------------------------
// \ru Расстояние до параллельной прямой \en The distance to the parallel line 
// ---
inline double MbLine::DistanceToParallel( const MbLine & to ) const {
  return DistanceToPoint( to.origin );
}


//------------------------------------------------------------------------------
// \ru Выдать коеффициенты неявного представления \en Get coefficients of implicit representation 
// ---
inline void MbLine::Implicit( double & A, double & B, double & C ) const {
  A = -direction.ay;
  B =  direction.ax;
  C = - (A*origin.x + B*origin.y);
}


//------------------------------------------------------------------------------
// \ru Определение параметров t1, t2 точки пересечения прямых, заданных точкой и вектором направления. 
// \en Definition of parameters t1, t2 of the intersection point of lines given a point and a direction vector. 
// ---
inline bool LineLineCrossParams( const MbCartPoint & origin1, const MbVector & direction1,
                                 const MbCartPoint & origin2, const MbVector & direction2,
                                 double & t1, double & t2 )
{
  double d = direction1.y * direction2.x - direction1.x * direction2.y;

  if ( ::fabs( d ) > EPSILON ) { // \ru Прямые не параллельны. \en Lines not parallel. 
    double dx = origin2.x - origin1.x;
    double dy = origin2.y - origin1.y;

    t2 = ( direction1.x * dy - direction1.y * dx ) / d;
    t1 = ( direction2.x * dy - direction2.y * dx ) / d;

    return true;
  }
  if ( origin1.IsSame(origin2, EPSILON) ) { // KOMPAS-41917
    t1 = 0.0;
    t2 = 0.0;
    return true;
  }
  // \ru Для параллельных прямых определяем параметры точки по середине между origin1 и origin2. \en Definition the parameters for the middle point between origin 1 and origin 2 for parallel lines. 
  MbCartPoint middle;
  middle.Set( origin1, 0.5, origin2, 0.5 );
  t1 = ( (middle.x - origin1.x) * direction1.x + (middle.y - origin1.y) * direction1.y );
  t2 = ( (middle.x - origin2.x) * direction2.x + (middle.y - origin2.y) * direction2.y );
  d = ( direction1.x * direction1.x ) + ( direction1.y * direction1.y );
  if ( d > NULL_EPSILON ) 
    t1 /= d;
  d = ( direction2.x * direction2.x ) + ( direction2.y * direction2.y );
  if ( d > NULL_EPSILON ) 
    t2 /= d;

  return false; // \ru Прямые совпадают или параллельны \en Lines coincide or are parallel 
}


#endif // __CUR_LINE_H
