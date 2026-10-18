////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Ломаная линия в двумерном пространстве.
         \en Polyline in two-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_POLYLINE_H 
#define __CUR_POLYLINE_H

#include <cur_polycurve.h>
#include <vector>


class   MbLineSegment;
class   MbCubicSpline;
class   MbContour;
class             MbRegDuplicate;
class             MbRegTransform;
class             MbPolylineSearchTree;


//------------------------------------------------------------------------------
/** \brief \ru Ломаная линия в двумерном пространстве.
           \en Polyline in two-dimensional space. \~
  \details \ru Ломаная линия в двумерном пространстве определяется контрольными точками pointList.
    Параметр ломаной в контрольных точках принимают целочисленные значения, начиная с нуля. 
    Ломаная проходит через свои контрольные точки при целочисленных значениях параметра. 
    Параметр ломаной изменяется от нуля до k, 
    где k - количество контрольных точек минус один для не замкнутой ломаной и k - количество контрольных для замкнутой ломаной. 
    Производная ломаной на каждом участке постоянна и равна вектору, построенному между двумя соседними контрольными точками.
           \en Polyline in two-dimensional space is defined by 'pointList' control points.
    Parameters of polyline at control points take on integer values starting from zero. 
    Polyline passes through its control points at integer values of parameter. 
    Parameter of a polyline changes from zero to 'k', 
    where 'k' - count of control points minus one for an open polyline and 'k' - count of control points for a closed polyline. 
    Derivative of polyline is constant at each piece and is equal to vector constructed between two neighboring control points. \~ 
  \ingroup Curves_2D
*/
// ---
class MATH_CLASS MbPolyline : public MbPolyCurve {
private :
  ptrdiff_t segmentsCount; ///< \ru Число сегментов ломаной линии. \en Count of segments of polyline. 
private:
  mutable MbPolylineSearchTree * searchTree; ///< \ru Дерево габаритов для ускорения поиска сегментов. \en A tree of bounding boxes for segment search acceleration. 

public :
  // \ru Конструктор отрезка. \en Constructor of a segment.
  MbPolyline( const MbCartPoint & p1, const MbCartPoint & p2 )
    : MbPolyCurve() 
    , segmentsCount( 1 )
    , searchTree( nullptr )
  {
    pointList.reserve( 2 );
    pointList.push_back( p1 );
    pointList.push_back( p2 );
    uppIndex = 1;
    closed = false;
  }
  // \ru Конструктор по набору точек и признаку замкнутости. \en Constructor by points and closedness state.
  template <class PointsVector>
  MbPolyline( const PointsVector & initList, bool cls )
    : MbPolyCurve()
    , segmentsCount( UNDEFINED_INT_T )
    , searchTree( nullptr )
  {
    Init( initList, cls );
  }
  /// \ru Конструктор по прямоугольнику. \en Constructor by a rectangle. 
  MbPolyline( MbRect & );
  /// \ru Конструктор наклонного прямоугольника. \en Constructor of inclined rectangle. 
  MbPolyline( const MbCartPoint & p1, double height, double weight, const MbDirection & angle );
  // \ru Конструктор копирования. \en Copy constructor.
  MbPolyline( const MbPolyline & );
public :
  virtual ~MbPolyline();

public :
  VISITING_CLASS( MbPolyline ); 

  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */

  MbePlaneType  IsA() const override; // \ru Тип элемента \en Type of element 
  bool        SetEqual( const MbPlaneItem & ) override;         // \ru Сделать элементы равными \en Make the elements equal 
  bool        IsSame( const MbPlaneItem &, double accuracy = LENGTH_EPSILON ) const override; // \ru Является ли кривая curve копией данной кривой ? \en Whether curve 'curve' is a duplicate of the current curve. 
  void        Transform( const MbMatrix &, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override;  // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move( const MbVector &, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate( const MbCartPoint &, const MbDirection & angle, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Поворот \en Rotation
  MbPlaneItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  void        Refresh() override; // \ru Сбросить все временные данные \en Reset all temporary data 

  /** \} */
  /** \ru \name Функции описания области определения кривой.
      \en \name Functions for description of a curve domain.
      \{ */
  double      GetTMin() const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of the parameter 
  double      GetTMax() const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter 

  /** \} */
  /** \ru \name Функции для работы в области определения кривой.
            Функции PointOn, FirstDer, SecondDer, ThirdDer,... корректируют параметр
            при выходе его за пределы области определения.
            Исключение составляет MbLine (прямая).
      \en \name Functions for working in the curve's domain. 
            Functions PointOn, FirstDer, SecondDer, ThirdDer,... correct parameter
            if it is out of domain bounds.
            Except MbLine (line). 
      \{ */
  void        PointOn  ( double & t, MbCartPoint & p  ) const override;  // \ru Точка на кривой \en Point on the curve 
  void        FirstDer ( double & t, MbVector    & fd ) const override;  // \ru Первая производная \en The first derivative 
  void        SecondDer( double & t, MbVector    & sd ) const override;  // \ru Вторая производная \en The second derivative 
  void        ThirdDer ( double & t, MbVector    & td ) const override;  // \ru Третья производная \en The third derivative 

  /** \} */
  /** \ru \name Функции для работы внутри и вне области определения кривой. 
            Функции _PointOn, _FirstDer, _SecondDer, _ThirdDer,... не корректируют параметр
            при выходе за пределы области определения. При выходе за область определения незамкнутая
            кривая, в общем случае, продолжается по касательной, которую она имеет на соответствующем конце.
            Исключение составляют дуги эллипса и окружности - они продолжаются в соответствии со своими уравнениями.
      \en \name Functions for working inside and outside the curve's domain. 
            Functions _PointOn, _FirstDer, _SecondDer, _ThirdDer,... don't correct parameter
            if it is out of domain bounds. If the parameter is out of domain bounds, an unclosed
            curve is extended by tangent vector at corresponding end point in general case.
            Except arcs of an ellipse or a circle - they are extended according to their equations.
      \{ */
  void       _SecondDer( double t, MbVector & v ) const override;
  void       _ThirdDer ( double t, MbVector & v ) const override;
  /** \} */

  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметра кривой.
      \en \name Functions for get of the group of data inside and outside the curve's domain of parameter.
      \{ */
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void        Explore( double & t, bool ext,
                       MbCartPoint & pnt, MbVector & fir, MbVector * sec, MbVector * thir ) const override;
  /** \} */

  /** \ru \name Функции инициализации кривой.
      \en \name Initialization functions of a curve.
      \{ */ 
  /// \ru Инициализация по другой ломаной. \en Initialization by another polyline.
  void        Init( const MbPolyline & ); 
  /// \ru Инициализация по точкам и признаку замкнутости. \en Initialization by points and an attribute of closedness.
  template <class PointsVector>
  bool        Init( const PointsVector & initList, bool cls )
  {
    if ( initList.size() > 1 ) {
      pointList.clear();
      pointList = initList;
      uppIndex  = (ptrdiff_t)pointList.size() - 1;
      closed    = cls;
      // if curve is closed then the start and the end points have to be different
      if ( (uppIndex > 1) && closed && c3d::EqualPoints( pointList.front(), pointList.back(), Math::LengthEps ) ) {
        closed = true;
        pointList.erase( pointList.begin() + uppIndex );
        uppIndex--;
      }
      segmentsCount  = (uppIndex > 0) ? ( uppIndex + !!closed ) : 0;
      Refresh();  // сбросить кривую
      return true;
    }
    return false;
  }
  /// \ru Построение прямоугольника. \en Construction of a rectangle. 
  void        Init( const MbCartPoint & p1, const MbCartPoint & p2 );
  /// \ru Построение правильного многоугольника. \en Construction of a regular polygon.
  void        Init( ptrdiff_t nVertex, const MbCartPoint & pc, double rad, const MbCartPoint & on, bool describe );
  /// \ru Построение наклонного прямоугольника. \en Constructor of an inclined rectangle.
  void        Init( const MbCartPoint & p1, double height, double weight, const MbDirection & angle );

  /** \} */
  /** \ru \name Общие функции кривой
      \en \name Common functions of curve
      \{ */
  MbCurve *   Offset( double rad ) const override;  // \ru Смещение полилинии \en Shift of polyline 

  MbNurbs *   NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;
  MbCurve *   NurbsCurve( const MbNurbsParameters & ) const override;
  MbContour * NurbsContour() const override;
  // \ru Построить усеченную кривую c учетом заданной двумерной точности. \en Construct a trimmed curve with the given two-dimensional accuracy.
  MbCurve *   Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyEps, bool saveParamLenAndLaw ) const override;
  using MbCurve::Trimmed; // \ru Метод базового класса MbCurve без учета двумерной точности. Точность по умолчанию - MbDimAccuracy::twoDimAcc.  \en Method of MbCurve base class without two-dimensional accuracy. The default accuracy is MbDimAccuracy::twoDimAcc.

  double      Step( double t, double sag ) const override; // \ru Вычисление шага аппроксимации \en Calculation of step of approximation 
  double      DeviationStep( double t, double angle ) const override; // \ru Вычисление шага аппроксимации по угловой толерантности \en Calculation of step of approximation by angular tolerance 

  bool        IsStraight( bool ignoreParams = false ) const override; // \ru Признак прямолинейности кривой \en An attribute of curve straightness. 

  void        Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменение направления кривой на противоположное \en Change curve direction to the opposite one

  double      CalculateMetricLength() const override; // \ru Посчитать метрическую длину \en Calculate the metric length 
  double      GetLengthEvaluation() const override; // \ru Оценка метрической длины кривой \en Estimation of metric length of the curve  
          // \ru Посчитать метрическую длину с заданной точностью \en Calculate metric length with given tolerance 
  double      CalculateLength( double t1, double t2 ) const override;
  // \ru Положение точки относительно полилинии. \en Point position relative to the polyline. 
  // \ru Возвращает результат : \en Returning result: 
  // \ru iloc_InItem = 1 - точка находится слева от полилинии, \en Iloc_InItem = 1 - point is to the left of the polyline, 
  // \ru iloc_OnItem = 0 - точка находится на полилинии, \en Iloc_OnItem = 0 - point is on the polyline, 
  // \ru iloc_OutOfItem = -1 - точка находится справа от полилинии. \en Iloc_OutOfItem = -1 - point is to the right of the polyline. 
  MbeItemLocation PointRelative( const MbCartPoint & pnt, double eps = Math::LengthEps ) const override;
  double      PointProjection( const MbCartPoint & ) const override;  // \ru Проекция точки на кривую \en Point projection on the curve 

  void        CalculateLocalGabarit( const MbMatrix & into, MbRect & local ) const override; // \ru Добавь в прям-к свой габарит с учетом матрицы \en Add your own gabarit taking the matrix into account 
  void        CalculateGabarit ( MbRect & ) const override; // \ru Определить габариты кривой \en Determine the bounding box of a curve 

  // \ru Сдвинуть параметр t на расстояние len по направлению \en Translate parameter 't' by distance 'len' along the direction 
  bool        DistanceAlong( double & t, double len, int curveDir, double eps = Math::LengthEps,
                             VERSION version = Math::DefaultMathVersion() ) const override;
  double      DistanceToPoint( const MbCartPoint & to ) const override; // \ru Расстояние до точки \en Distance to a point 
  bool        DistanceToPointIfLess( const MbCartPoint & toP, double & d ) const override; // \ru Расстояние до точки, если оно меньше d \en Distance to a point if it is less than 'd' 
  bool        GetMiddlePoint( MbCartPoint & midPoint ) const override;  // \ru Выдать среднюю точку кривой \en Get mid-point of a curve

  bool        GoThroughPoint( MbCartPoint & ) override; // \ru Пройти через точку \en Pass through point
  ptrdiff_t   GoThroughPoint( double t, MbCartPoint & p, double eps );

  /** \} */
  /** \ru \name Общие функции полигональной кривой
      \en \name Common functions of polygonal curve 
      \{ */

  void        GetRuleInterval( ptrdiff_t index, double & t1, double & t2 ) const override; // \ru Выдать интервал влияния точки кривой \en Get the interval of point influence 
  void        Rebuild() override; // \ru Перестроить кривую \en Rebuild curve 

  MbeState    DeletePart( double t1, double t2, MbCurve *& part2 ) override; // \ru Удалить часть кривой между параметрами t1 и t2 \en Remove piece of polyline between parameters t1 and t2 
  MbeState    TrimmPart ( double t1, double t2, MbCurve *& part2 ) override; // \ru Оставить часть кривой между параметрами t1 и t2 \en Keep piece of polyline between parameters t1 and t2 

  void        IntersectHorizontal( double y, SArray<double> & ) const override;  // \ru Пересечение с горизонтальной прямой \en Intersection with a horizontal line
  void        IntersectVertical  ( double x, SArray<double> & ) const override;  // \ru Пересечение с вертикальной прямой \en Intersection with a vertical line
  void        SelfIntersect( SArray<MbCrossPoint> &, double metricEps = Math::LengthEps ) const override; // \ru Самопересечение полилинии \en Self-intersection of a polyline 

  // \ru Прямые, проходящие под углом к оси 0X и касательные к кривой \en Lines passing angularly to the 0X axis and tangent to the curve 
  void        Isoclinal( const MbVector & angle, SArray<double> & tFind ) const override;

  bool        GetCentre( MbCartPoint & ) const override;  // \ru Выдать центр полилинии \en Get center of a polyline 
  bool        GetWeightCentre( MbCartPoint & ) const override; // \ru Выдать центр тяжести кривой \en Get the center of gravity of the curve 
  size_t      GetCount() const override; // \ru Количество разбиений для прохода в операциях \en Count of subdivisions for pass in operations  

  void        CheckParameter( double & ) const;  ///< \ru Проверка параметра. \en Check parameter. 
  ptrdiff_t   ChangeThroughPoint( const MbCartPoint & );

  void        InsertPoint( ptrdiff_t index, const MbCartPoint & pnt ) override;                 // \ru Вставить точку по индексу \en Insert point by index
  void        InsertPoint( double t,  const MbCartPoint & pnt, double, double ) override; // \ru Вставить точку \en Insert a point
  bool        CheckParam ( double & t, ptrdiff_t & i0, ptrdiff_t & i1, double & t0, double & t1 ) const override; // \ru Установить параметр \en Set parameter 
  double      GetParam( ptrdiff_t i ) const override;
  size_t      GetParamsCount() const override; 

  double      Area() const;         // \ru Площадь замкнутого многоугольника \en Area of closed a polygon 
  int         Orientation() const;  // \ru Ориентация замкнутого многоугольника \en Orientation of a closed polygon 

  bool        IsDegenerate( double eps = Math::LengthEps ) const override; // \ru Проверка вырожденности кривой \en Check for curve degeneracy 
  bool        IsSmoothConnected( double angleEps, bool ignoreLimits = false ) const override; // \ru Являются ли стыки контура\кривой гладкими? \en Whether the joints of a contour\curve are smooth. 

  bool        IsContinuousDerivative( bool & contLength, bool & contDirect, c3d::DoubleVector * params = nullptr, double epsilon = EPSILON ) const override; // \ru Непрерывна ли первая производная? \en Have the first derivative the continuous?
  // \ru Устранить разрывы первых производных по длине. \en Eliminate the discontinuities of the first derivative at length.
  bool        SetContinuousDerivativeLength( VERSION version, double epsilon = EPSILON ) override; 

  void        GetProperties( MbProperties & ) override;  // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override;  // \ru Записать свойства объекта \en Set properties of the object 

  /** \} */
  /** \ru \name Функции полилинии
      \en \name Functions of polyline
      \{ */

  void        Trimm( SArray<MbCartPoint> & point, double t1, double t2, double eps = Math::LengthEps ) const;

  // \ru Выдать среднюю точку сегмента полилинии \en Get mid-point of a segment of a polyline 
  bool        GetSegmentMiddlePoint( const MbCartPoint & from, MbCartPoint & midPoint ) const;
  // \ru Выдать линейный сегмент полилинии \en Get linear segment of a polyline 
  bool        GetLinearSegment( const MbCartPoint & from, MbCartPoint & p1, MbCartPoint & p2 ) const;
  bool        GetSegmentLength( const MbCartPoint & from, double & length ) const; // \ru Выдать длину сегмента полилинии \en Get length of segment of a polyline 
  ptrdiff_t   FindNearestSegment( const MbCartPoint & from ) const; // \ru Найти ближайший к точке сегмент полилинии \en Find the segment of polyline nearest to a point 
  MbContour * CreateContour() const; // \ru Сделать контур из полилинии \en Create a contour from a polyline 
  // \ru Вставка фаски между двумя соседними элементами \en Insert a chamfer between two neighboring elements 
  bool        ChamferTwoSegments( ptrdiff_t & index, double len, double angle,
                                  bool type, bool firstSeg = true );
  bool        Chamfer( double len, double angle, bool type ); // \ru Вставка фаски. \en Insertion of the chamfer. 
  MbeState    RemoveChamfer( const MbCartPoint & pnt );  // \ru Удалить фаску полилинии. \en Remove a chamfer of a polyline. 
  // \ru Построить точки и параметры для гладкого сплайна. \en Create points and parameters for a smooth spline. 
  bool        GetSplinePoints( SArray<MbCartPoint> & points, SArray<double> & arParams ) const;
  MbCubicSpline * CubicSpline() const; // \ru Построить гладкий сплайн из ломаной. \en Create a smooth spline from a polyline.

  ptrdiff_t   GetSegmentsCount() const { return segmentsCount; }

  double      Step         ( double t, double sag,   ThreeStates dir ) const;  ///< \ru Шаг параметра с учетом радиуса кривизны. \en Step of parameter with consideration of curvature. 
  double      DeviationStep( double t, double angle, ThreeStates dir ) const;  ///< \ru Шаг параметра по заданному углу отклонения касательной. \en Step of parameter by a given angle of deviation of tangent. 

  /** \brief \ru Определить точки пересечения с отрезком.
      \en Determine points of intersection with a line segment. \~
    \details \ru Определить точки пересечения ломаной и отрезка. \n
              \en Determine intersection points of the polyline and a line segment. \n \~
    \param[in] lineSegment - \ru Отрезок.
                              \en A line segment. \~
    \param[in] xEps -        \ru Погрешность по U.
                              \en U-accuracy. \~
    \param[out] ttPolyline - \ru Массив параметров на ломаной.
                              \en An array of parameters on the polyline. \~
    \param[out] ttSegment -  \ru Массив параметров на отрезке.
                              \en An array of parameters on the line segment. \~
    \return \ru Количество точек пересечения.
            \en The number of cross points. \~
  */
  template <class ParamsVector>
  MATH_FUNC( size_t ) SegmentIntersection( const MbLineSegment & lineSegment, double xEps, double yEps, ParamsVector & ttPolyline, ParamsVector & ttSegment ) const;
  
  /// \ru Определить положение точки относительно кривой при известном индексе ближайшего сегмента. \en Define the point position relative to the curve when the nearest segment index is known.
  bool        PointRelative( const MbCartPoint & pnt, ptrdiff_t nearestSegmentIndex, double eps, MbeItemLocation & iLoc ) const;
  /// \ru Определить номер сегмента (или пару номеров сегментов) по параметру на ломаной. \en Define segment index (or pair of segment indices) by parameter on polyline.
  bool        FindSegmentPair( double t, c3d::IndicesPair & ) const;
  /// \ru Определить расстояние от точки до сегмента ломаной как отрезка. \en Calculate distance from a point to segment of polyline.
  double      DistanceToPolylineSegment( size_t, const MbCartPoint & ) const;
  /// \ru Самопересечение ломаной. \en Self-intersection of a polyline. 
  bool        IsSelfIntersecting( double metricEps = Math::LengthEps ) const;

  /** \} */

protected:
  /// \ru Удалить дерево поиска сегментов. \en Delete segments search tree. 
  void        DeleteSearchTree() const;
  /// \ru Создать и заполнить дерево поиска сегментов. \en Create and fill segments search tree. 
  bool        CreateSearchTree() const;
  /// \ru Поиск ближайших к точке сегментов по дереву поиска. \en Nearest to point segments by search tree. 
  bool        FindNearestSegmentsByTree( const MbCartPoint &, c3d::IndicesVector & ) const;
  /// \ru Поиск пересекающихся с отрезком сегментов по дереву поиска. \en Intersecting of line segment and segments of polyline by search tree. 
  bool        FindIntersectingSegmentsByTree( const MbCartPoint & p1, const MbCartPoint & p2, double xEps, double yEps, c3d::IndicesVector & ) const;
  /// \ru Самопересечение ломаной. \en Self-intersection of a polyline. 
  template <class CrossPointsVector>
  bool        SelfIntersect( CrossPointsVector &, bool tillFirst, double metricEps ) const;

private:
  void        operator = ( const MbPolyline & ); // \ru Не реализовано. \en Not implemented. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbPolyline )
}; // MbPolyline

IMPL_PERSISTENT_OPS( MbPolyline )


#endif // __CUR_POLYLINE_H 
