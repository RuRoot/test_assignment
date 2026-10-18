////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Кривая в двумерном пространстве, заданная точками.
         \en Curve in two-dimensional space, defined by points. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_POLYCURVE_H
#define __CUR_POLYCURVE_H


#include <curve.h>
#include <cur_line_segment.h>
#include <mb_rect.h>
#include <vector>


class             MbRegDuplicate;
class             MbRegTransform;


//------------------------------------------------------------------------------
/** \brief \ru Кривая в двумерном пространстве, заданная точками.
           \en Curve in two-dimensional space, defined by points. \~
  \details \ru Родительский класс кривых в двумерном пространстве, заданных контрольными точками: 
    MbBezier, MbCubicSpline, MbHermit, MbNurbs, MbPolyline. \n
           \en Parent class of curves in two-dimensional space, defined by control points: 
    MbBezier, MbCubicSpline, MbHermit, MbNurbs, MbPolyline. \n \~
  \ingroup Curves_2D
*/
// ---
class MATH_CLASS MbPolyCurve : public MbCurve {
protected :
  SArray<MbCartPoint>         pointList;    ///< \ru Множество контрольных точек. \en Set of control points. 
  ptrdiff_t                   uppIndex;     ///< \ru Количество участков кривой (равно количество контрольных точек минус единица). \en Count of curve pieces (is equal to count of control points minus one). 
  bool                        closed;       ///< \ru Признак замкнутости кривой. \en An attribute of curve closedness. 
  
  mutable MbRect              rect;         ///< \ru Габаритный прямоугольник. \en Bounding rectangle. 
  mutable atomic_double       metricLength; ///< \ru Метрическая длина сплайна. \en Metric length of a spline. 

protected:
  MbPolyCurve();  ///< \ru Конструктор по умолчанию. \en Default constructor. 
  MbPolyCurve( const MbPolyCurve & pCurve ); ///< \ru Конструктор копирования. \en Copy-constructor. 
public :
  virtual ~MbPolyCurve(); ///< \ru Деструктор. \en Destructor. 

public :
  VISITING_CLASS( MbPolyCurve ); 

  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */
  //virtual MbePlaneType  IsA() const = 0; // \ru Тип элемента \en Type of element
  MbePlaneType  Type() const override;    // \ru Тип элемента \en Type of element 
  //virtual bool        SetEqual( const MbPlaneItem & ) = 0;         // \ru Сделать элементы равными \en Make the elements equal
  //virtual bool        IsSame( const MbPlaneItem & other, double accuracy = LENGTH_EPSILON ) const = 0; // \ru Является ли кривая curve копией данной кривой ? \en Whether curve 'curve' is a duplicate of the current curve.
  //virtual void        Transform( const MbMatrix & matr, MbRegTransform * ireg = nullptr, const MbSurface * newSurface = nullptr ) = 0; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  //virtual void        Move( const MbVector & to, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) = 0;        // \ru Сдвиг \en Translation
  //virtual void        Rotate( const MbCartPoint & pnt, const MbDirection & angle, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) = 0; // \ru Поворот \en Rotation
  //virtual MbPlaneItem & Duplicate( MbRegDuplicate * = nullptr ) const = 0; // \ru Сделать копию элемента \en Create a copy of the element
  void        AddYourGabaritTo( MbRect & r ) const override;           // \ru Добавь свой габарит в прямой прям-к \en Add your own gabarit into the given bounding rectangle 
  void        Refresh() override;          // \ru Сбросить все временные данные \en Reset all temporary data 
  void        PrepareIntegralData( const bool forced ) const override; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.
 /** \} */
 /** \ru \name Функции описания области определения кривой.
     \en \name Functions for description of a curve domain.
      \{ */
  //virtual double      GetTMax()    const = 0; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter
  //virtual double      GetTMin()    const = 0; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter
  bool        IsClosed()   const override; // \ru Проверка замкнутости сплайна \en Check the spline closedness 
  /** \} */
  /** \ru \name Функции для работы в области определения кривой. 
            Функции PointOn, FirstDer, SecondDer, ThirdDer,... корректируют параметр
            при выходе его за пределы области определения.
      \en \name Functions for working in the curve's domain. 
            Functions PointOn, FirstDer, SecondDer, ThirdDer,... correct parameter
            if it is out of domain bounds.
      \{ */
  //virtual void        PointOn  ( double & t, MbCartPoint & pnt ) const = 0; // \ru Точка на кривой \en Point on the curve
  //virtual void        FirstDer ( double & t, MbVector    & fd  ) const = 0;   // \ru Первая производная \en The first derivative
  //virtual void        SecondDer( double & t, MbVector    & sd  ) const = 0;   // \ru Вторая производная \en The second derivative
  //virtual void        ThirdDer ( double & t, MbVector    & td  ) const = 0;   // \ru Третья производная \en The third derivative
  /** \} */
  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметра кривой.
      \en \name Functions for get of the group of data inside and outside the curve's domain of parameter.
      \{ */
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  //virtual void        Explore( double & t, bool ext,
  //                             MbCartPoint & pnt, MbVector & fir, MbVector * sec, MbVector * thir ) const = 0;
  /** \} */
  /** \ru \name Общие функции кривой
      \en \name Common functions of curve
      \{ */
  bool        IsStraight( bool ignoreParams = false ) const override; // \ru Признак прямолинейности кривой \en An attribute of curve straightness. 
  bool        HasLength( double & length ) const override;

  //virtual void        Inverse( MbRegTransform * iReg = nullptr ) = 0;      // \ru Изменение направления кривой на противоположное \en Change curve direction to the opposite one

  MbeState    Deformation( const MbRect &, const MbMatrix & ) override; // \ru Деформация \en Deformation 
  bool        IsInRectForDeform( const MbRect & r ) const override;     // \ru Виден ли объект в заданном прямоугольнике для деформации \en Whether the object is visible in the given rectangle for deformation 
  size_t      GetCount() const override; // \ru Количество разбиений для прохода в операциях \en Count of subdivisions for pass in operations  

  // \ru Дать метрическую длину кривой. \en Get metric length of curve. 
  double      GetMetricLength() const override;
  // \ru Дать оценочную длину кривой. \en Get evaluation length of curve. 
  double      GetLengthEvaluation() const override;
  // \ru Выдать характерную точку полилинии если она ближе чем dmax \en Get control point of polyline if it is closer than 'dmax' 
  bool        GetSpecificPoint( const MbCartPoint & from, double & dmax, MbCartPoint & pnt ) const override;

  //virtual void        GetProperties( MbProperties & ) = 0;  // \ru Выдать свойства объекта \en Get properties of the object
  //virtual void        SetProperties( const MbProperties & ) = 0;  // \ru Записать свойства объекта \en Set properties of the object
  void        GetBasisPoints( MbControlData & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \} */
  /** \ru \name Общие функции полигональной кривой
      \en \name Common functions of polygonal curve
      \{ */
  virtual size_t      GetPointsCount() const;     ///< \ru Выдать количество контрольных точек. \en Get count of control points.

  /** \brief \ru Выдать характерную точку.
             \en Get control point. \~
    \details \ru Возвращает характерную точку кривой по ее индексу. 
      Если индекс отрицательный - возвращает первую точку.
      Если индекс больше максимального доступного, то возвращается последняя точка.
             \en Returns control point of the curve by its index. 
      If the index is negative - returns the first point.
      If the index is greater than the maximum available, then the last point is returned. \~ 
    \param[in] index - \ru Номер характерной точки.
                       \en Index of control point. \~
    \param[out] pnt - \ru Характерная точка.
                      \en Control point. \~
  */
  virtual void        GetPoint( ptrdiff_t index, MbCartPoint & pnt ) const;    // \ru Выдать точку \en Get point

  virtual  ptrdiff_t  GetNearPointIndex( const MbCartPoint & pnt ) const; ///< \ru Выдать индекс точки, ближайшей к заданной. \en Get index of the point nearest to the given one. 

  /** \brief \ru Вернуть интервал влияния точки кривой.
             \en Get the range of influence of point of the curve. \~
    \details \ru Определяет, на каком интервале параметра кривой скажется 
      изменение характерной точки с индексом index.
             \en Determines which range of curve parameter will be affected by 
      changing of the control point with 'index' index. \~
    \param[in] index - \ru Номер характерной точки.
                       \en Index of control point. \~
    \param[out] t1 - \ru Минимальный параметр интервала влияния.
                     \en Minimal parameter of the range of influence. \~
    \param[out] t2 - \ru Максимальный параметр интервала влияния.
                     \en Maximal parameter of the range of influence. \~
  */
  virtual void        GetRuleInterval( ptrdiff_t index, double & t1, double & t2 ) const = 0;

  virtual void        Rebuild() = 0; ///< \ru Перестроить кривую. \en Rebuild curve. 
  virtual void        SetClosed( bool cls );   ///< \ru Установить признак замкнутости. \en Set attribute of closedness.

  virtual void        SetBegEndDerivesEqual(); ///< \ru Установить равные производные на краях. \en Set equal derivatives at the ends. 
  virtual void        ClosedBreak(); ///< \ru Сделать незамкнутой, оставив совпадающими начало и конец. \en Make curve open, keeping coincidence of the beginning and the end. 

  virtual void        RemovePoint( ptrdiff_t index ); ///< \ru Удалить характерную точку с заданным индексом. \en Remove control point with given index.
  virtual void        RemovePoints();           ///< \ru Удалить все точки. \en Remove all points. 

  virtual void        AddPoint( const MbCartPoint & pnt ); ///< \ru Добавить точку в конец массива контрольных точек. \en Add point to the end of the array of control points. 

  /** \brief \ru Изменить характерные точки.
             \en Change control points. \~
    \details \ru Если количество точек в заданном массиве совпадает с текущим количеством точев,
      то массив точек заменяется на новый. Если не совпадает, то характерные точки не меняются,
      функция возвращает false.
             \en If count of points in the given array is equal to the current count of points,
      then the array of points is replaced by the new one. If not equal, then the control points are not changed,
      the function returns false. \~
    \param[in] pntList - \ru Заданный массив контрольных точек.
                         \en The given array of control points. \~
    \return \ru true - если точки были изменены.
            \en True if points have been changed. \~
  */
  virtual bool        ChangePointsValue( const SArray<MbCartPoint> & pntList ); // \ru Поменять точки \en Swap points 

  /** \brief \ru Вставить точку в массив контрольных точек.
             \en Insert a point to the array of control points. \~
    \details \ru Вставить заданную точку после точки с индексом index.
      В функции не проверяется корректность индекса. 
      Надо заранее удостоверится, что заданный индекс меньше количества точек в массиве.
             \en Insert the given point after the point with 'index' index.
      Correctness of the index isn't checked in the function. 
      It is necessary to make sure in advance that the given index is less than the count of points in array. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
    \param[in] index - \ru Индекс, после которого надо вставить точку.
                       \en Index of point to insert a point after. \~
  */
  virtual void        AddAfter( const MbCartPoint & pnt, ptrdiff_t index );

  /** \brief \ru Вставить точку в массив контрольных точек.
             \en Insert a point to the array of control points. \~
    \details \ru Вставить заданную точку по индексу.
             \en Insert the given point by index. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
    \param[in] index - \ru Индекс, по которому надо вставить точку.
                       \en Index to insert a point by. \~
  */
  virtual void        InsertPoint( ptrdiff_t index, const MbCartPoint & pnt ) = 0; // \ru Вставить точку по индексу \en Insert point by index 

  /** \brief \ru Вставить точку в массив контрольных точек.
             \en Insert a point to the array of control points. \~
    \details \ru Вставить точку, которая будет соответствовать параметру t кривой.
      Если параметр t отличается от параметра некоторой точки меньше, чем на заданную погрешность,
      то новая точка не вставляется, заменяется уже существующая близкая по параметру точка.
             \en Insert a point which corresponds to parameter 't' of the curve.
      If parameter 't' differs from the parameter of some point less than by the given tolerance,
      then the new point isn't inserted, already existent point close by parameter is replaced. \~
    \param[in] t - \ru Параметр новой точки.
                   \en Parameter of the new point. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
    \param[in] xEps - \ru Точность по x.
                      \en Tolerance in x direction. \~
    \param[in] yEps - \ru Точность по y.
                      \en Tolerance in y direction. \~
  */
  virtual void        InsertPoint( double t,  const MbCartPoint & pnt,                     double xEps, double yEps ) = 0; // \ru Вставить точку \en Insert a point 

  /** \brief \ru Вставить точку в массив контрольных точек.
             \en Insert a point to the array of control points. \~
    \details \ru Вставить точку и соответствующий ей вектор производной, 
      которая будет соответствовать параметру t кривой.
      Если параметр t отличается от параметра некоторой точки меньше, чем на заданную погрешность,
      то новая точка не вставляется, заменяется уже существующая близкая по параметру точка.
             \en Insert a point and derivative vector corresponding to it, 
      which will correspond to parameter 't' of the curve.
      If parameter 't' differs from the parameter of some point less than by the given tolerance,
      then the new point isn't inserted, already existent point close by parameter is replaced. \~
    \param[in] t - \ru Параметр новой точки.
                   \en Parameter of the new point. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
    \param[in] v - \ru Производная в заданной точке.
                   \en Derivative at the given point. \~
    \param[in] xEps - \ru Точность по x.
                      \en Tolerance in x direction. \~
    \param[in] yEps - \ru Точность по y.
                      \en Tolerance in y direction. \~
  */
  virtual void        InsertPoint( double t,  const MbCartPoint & pnt, const MbVector & v, double xEps, double yEps ); // \ru Вставить точку по индексу \en Insert point by index 

  /** \brief \ru Заменить полюс.
             \en Replace a pole. \~
    \details \ru В общем случае функция эквивалентна ChangePoint() и заменяет характерную точку с указанным индексом.
             \en Generally the function is equivalent to ChangePoint() and replaces a control point with the specified index. \~
    \param[in] index - \ru Индекс изменяемой точки.
                       \en Index of a point to be changed. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
  */
  virtual void        ChangePole ( ptrdiff_t index, const MbCartPoint & pnt ); // \ru Заменить полюс \en Replace a pole 

  /** \brief \ru Заменить точку.
             \en Replace a point. \~
    \details \ru Заменить точку. \n
             \en Replace a point. \n \~
    \param[in] index - \ru Индекс изменяемой точки.
                       \en Index of a point to be changed. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
  */
  virtual void        ChangePoint( ptrdiff_t index, const MbCartPoint & pnt ); // \ru Заменить точку \en Replace a point 

  /** \brief \ru Переместить параметр в область определения кривой.
             \en Drive a parameter into the curve domain. \~
    \details \ru Проверить параметр. Если он выходит за диапазон изменения параметров кривой, то 
      в случае замкнутой кривой привести его в область определения, изменяя на период.
      В случае незамкнутой кривой - сделать равным ближайшему граничному параметру.
      Определить индексы характерный точек, между которыми находится заданный параметр, 
      и их параметрические значения.
             \en Check parameter. If it is out of the range of the curve parameters, then 
      in case of closed curve drive it into the definition domain with changing by period.
      In case of open curve - make equal to the nearest boundary parameter.
      Determine indices of control points the given parameter is between, 
      and also determine their parametric values. \~
    \param[in, out] t - \ru На входе - заданный параметр. На выходе - параметр в области определения кривой.
                        \en On input - the given parameter. On output - parameter in the curve definition domain. \~
    \param[out] i0 - \ru Индекс характерной точки слева от заданного параметра.
                     \en Index of control point to the left of the given parameter. \~
    \param[out] i1 - \ru Индекс характерной точки справа от заданного параметра.
                     \en Index of control point to the right of the given parameter. \~
    \param[out] t0 - \ru Параметр характерной точки слева от заданного параметра.
                     \en Parameter of control point to the left of the given parameter. \~
    \param[out] t1 - \ru Параметр характерной точки справа от заданного параметра.
                     \en Parameter of control point to the right of the given parameter. \~
    \return \ru true - если операция выполнена успешно.
            \en True if the operation succeeded. \~
  */
  virtual bool        CheckParam ( double & t, ptrdiff_t & i0, ptrdiff_t & i1, double & t0, double & t1 ) const = 0; // \ru Загнать параметр получить локальный индексы и параметры \en Drive parameter into domain, get local indices and parameters 
  virtual double      GetParam( ptrdiff_t i ) const = 0;///< \ru Вернуть параметр, соответствующий точке с указанным индексом. \en Get parameter corresponding to the point with specified index. 
  virtual size_t      GetParamsCount() const = 0; ///< \ru Выдать количество параметров. \en Get count of parameters. 
  virtual void        GetTList( SArray<double> & params ) const;
  
  size_t      GetPointListCount() const { return pointList.Count(); } ///< \ru Выдать количество характерный точек. \en Get count of control points. 
  ptrdiff_t   GetPointListMaxIndex() const { return pointList.MaxIndex(); } ///< \ru Выдать максимальный индекс массива контрольных точек. \en Get maximal index of array of control points. 

  template <class PointsVector>
  void        GetPoints( PointsVector & pnts ) const { std::copy( pointList.begin(), pointList.end(), std::back_inserter( pnts ) ); } ///< \ru Вернуть массив контрольных точек. \en Get array of control points.  
  void        GetPointList( SArray<MbCartPoint>    & pnts ) const { pnts.assign( pointList.begin(), pointList.end() ); } ///< \ru Вернуть массив контрольных точек. \en Get array of control points.
  void        GetPointList( c3d::ParamPointsVector & pnts ) const { pnts.assign( pointList.begin(), pointList.end() ); } ///< \ru Вернуть массив контрольных точек. \en Get array of control points.

  bool        ReplacePoints( const SArray<MbCartPoint> & pnts ); ///< \ru Заменить набор контрольных точек. \en Replace the set of control points. 
  bool        ReplacePoints( const std::vector<MbCartPoint> & pnts ); ///< \ru Заменить набор контрольных точек. \en Replace the set of control points. 

  const   MbCartPoint & GetPointList( size_t i ) const { return pointList[i]; } ///< \ru Вернуть характерную точку с заданным индексом. \en Get control point with the given index. 
          MbCartPoint & SetPointList( size_t i )       { Refresh(); return pointList[i]; } ///< \ru Вернуть характерную точку с заданным индексом. \en Get control point with the given index. 

  ptrdiff_t   GetUppIndex() const { return uppIndex; } ///< \ru Вернуть максимальный индекс массива контрольных точек. \en Get the maximal index of array of control points.  
  size_t      GetSegmentsCount() const { return (uppIndex > 0) ? (uppIndex + (!!closed)) : 0; }  ///< \ru Вернуть количество интервалов. \en Get count of ranges. 

  template <class SegmentsVector>
  void        GetLineSegments( SegmentsVector & segments, double eps = PARAM_PRECISION ) const; ///< \ru Выдать массив отрезков. \en Get the array of segments.

  /** \brief \ru Дать информацию для функции NurbsCurve.
             \en Get information for NurbsCurve function. \~
    \details \ru Дать информацию для функции NurbsCurve. 
      Если отрезок кривой проходит через pmin у замкнутой кривой (например, [0.2, 1.1] для замкнутой кривой с параметризацией [0, 1])
      то i1, i2 учитывают это и могут нумероваться не с 0, а с GetPointsCount(), т.е. через период.
             \en Get information for NurbsCurve function. 
      If curve segment passes through 'pmin' of a closed curve (for example, [0.2, 1.1] for closed curve with parameterization [0, 1]),
      then i1, i2 consider it and can be enumerated from GetPointsCount(), i.e. by period, instead of 0. \~
    \param[in] epsilon - \ru Заданная точность.
                         \en Given accuracy. \~
    \param[in] pmin - \ru Параметр начала аппроксимируемой части кривой.
                      \en Parameter of the beginning of the curve piece being approximated . \~
    \param[in] pmax - \ru Параметр конца аппроксимируемой части кривой.
                      \en Parameter of the end of a curve piece being approximated. \~
    \param[in] i1 - \ru Индекс следующего  за  pmin значения параметра.
                    \en Index of the parameter value next 'pmin' . \~
    \param[in] t1 - \ru Индекс предшествующего pmax значения параметра.
                    \en Index of the parameter value preceding 'pmax'. \~
    \param[in] i2 - \ru Параметр следующей  за  pmin характерной точки.
                    \en Parameter of the control point next 'pmin' . \~
    \param[in] t2 - \ru Параметр предшествующей pmax характерной точки.
                    \en Parameter of the control point preceding 'pmax' . \~
  */
  bool        NurbsParam( double epsilon, double & pmin, double & pmax, 
                          ptrdiff_t & i1, double & t1, ptrdiff_t & i2, double & t2 ) const;  
  /** \} */

protected:

  virtual bool        CanChangeClosed() const; ///< \ru Определить, можно ли поменять признак замкнутости. \en Determine whether it is possible to change an attribute of closedness. 
  // \ru Сдвинуть параметр t на расстояние len. \en Move parameter t on the metric distance len. 
  bool        CompositeDistanceAlong( double & t, double len, int curveDir, double eps, const SArray<double> & tList ) const;
  // \ru Рассчитать метрическую длину сегмента кривой. \en Calculate metric length of curve segment.
  double      SegmentCalculateLength( double w1, double w2, size_t n, double * x, double * w ) const;
  // \ru Сдвинуть параметр t на расстояние len. \en Move parameter t on the metric distance len. 
  bool        SegmentDistanceAlong( double & t1, double ln, int curveDir, double eps, double stepMax, size_t n, double * x, double * w ) const;

private:
  void        operator = ( const MbPolyCurve & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS( MbPolyCurve )
}; // MbPolyCurve

IMPL_PERSISTENT_OPS( MbPolyCurve )


//------------------------------------------------------------------------------
// \ru Выдать массив отрезков. \en Get the array of segments.
// ---
template <class SegmentsVector>
void MbPolyCurve::GetLineSegments( SegmentsVector & segments, double eps ) const
{
  size_t count = pointList.size();

  ptrdiff_t cnt = count;
  if ( !IsClosed() )
    cnt--;

  if ( cnt > 0 )
    segments.reserve( segments.size() + (size_t)cnt );

  c3d::PlaneCurveSPtr segment;

  for ( ptrdiff_t i = 0; i < cnt; i++ ) {
    ptrdiff_t inext = (i + 1) % count;

    if ( !pointList[i].IsSame( pointList[inext], eps ) ) {
      segment = new MbLineSegment( pointList[i], pointList[inext] );
      segments.push_back( static_cast<MbLineSegment *>( segment.get() ) );
      ::DetachItem( segment );
    }
  }
}


#endif // __CUR_POLYCURVE_H
