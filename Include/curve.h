////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Кривая в двумерном пространстве.
         \en Curve in two-dimensional space. \~
  \details \ru Двумерные кривые используются для описания области определения параметров поверхностей,
    построения трёхмерных кривых на поверхностях, кривых пересечения поверхностей, проекций
    трёхмерных кривых на поверхности и плоскости локальных систем координат. 
    Двумерные кривые устроены аналогично трёхмерным кривым с той разницей, что вместо трёхмерных точек и векторов
    в двумерных кривых используются двумерные точки и векторы.
           \en Two-dimensional curves are used for description of surfaces parameters domain,
    calculation of three-dimensional curves on surfaces, surfaces intersection curves, projections,
    three-dimensional curves on surface and plane of local coordinate systems. 
    Two-dimensional curves are organizes in the same way as three-dimensional curves with a difference that instead of three-dimensional points and vectors
    two-dimensional points and vectors are used in two-dimensional curves. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CURVE_H
#define __CURVE_H


#include <plane_item.h>
#include <mb_cart_point.h>
#include <mb_rect1d.h>
#include <mb_data.h>
#include <templ_visitor.h>
#include <hash32.h>


class  MbPlacement;
class  MbNurbs;
class  MbLine;
class  MbContour;
class  MbPolygon;
class  MbCrossPoint;
class  MbCurveExtensionParameters;
class  MbDimAccuracy;
class  MbCurveIntoNurbsInfo;
struct MbNurbsParameters;


class   MbCurve;
namespace c3d // namespace C3D
{
typedef SPtr<MbCurve>                    PlaneCurveSPtr;
typedef SPtr<const MbCurve>              ConstPlaneCurveSPtr;

typedef std::vector<MbCurve *>           PlaneCurvesVector;
typedef std::vector<const MbCurve *>     ConstPlaneCurvesVector;

typedef std::vector<PlaneCurveSPtr>      PlaneCurvesSPtrVector;
typedef std::vector<ConstPlaneCurveSPtr> ConstPlaneCurvesSPtrVector;
}


//------------------------------------------------------------------------------
/** \brief \ru Кривая в двумерном пространстве.
           \en Curve in two-dimensional space. \~
  \details \ru Кривая в двумерном пространстве представляет собой векторную функцию скалярного параметра,
    заданную на конечной одномерной области. Кривая представляет собой непрерывное
    отображение некоторого участка числовой оси в двумерное пространство.\n
    Двумерная кривая используется:\n 
    для плоского моделирования,\n
    для описания области определения параметров поверхности,\n
    для построения кривых на поверхностях,\n
    для построения кривых пересечения поверхностей.\n
    Нормаль к кривой - это перпендикуляр к касательной. \n
    Для вычисления направления нормали с учётом кривизны нужно умножить нормаль на знак кривизны.
           \en A curve in two-dimensional space is a vector function of a scalar parameter,
    given on a finite one-dimensional space. A curve is continuous
    mapping of some piece of numeric axis to two-dimensional space.\n
    Two-dimensional curve is used: \n 
    for planar modeling, \n
    for description of surface parameters domain, \n
    for construction of curves on surfaces, \n
    for constructing of surfaces intersection curves. \n
    Normal vector to a curve is perpendicular to tangent. \n
    To calculate the direction of normal according curvature, multiply this normal vector by the curvature sign. \~
  \ingroup Curves_2D
*/
// ---
class MATH_CLASS MbCurve : public MbPlaneItem, public MbNestSyncItem {
protected:
  SimpleName name; ///< \ru Имя кривой. Данные объекта являются временными и используются для внутренних целей. \en A curve name. The object data is temporary and used internally.

protected :
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbCurve();  
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbCurve( const MbCurve & other ) : MbPlaneItem(), name( other.name ) {}
public :
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbCurve();

public :
  virtual void Accept( Visitor & visitor )
  {
    VisitorImpl<MbCurve> * impl = dynamic_cast<VisitorImpl<MbCurve> *>(&visitor);
    if( impl )
      impl->Visit( *this );
  }

  // \ru Общие функции геометрического объекта. \en Common functions of a geometric object. 

  MbePlaneType Type() const override;     // \ru Групповой тип элемента. \en Group element type.
  MbePlaneType Family() const override;   // \ru Семейство объекта. \en Family of object. 
  void        Refresh() override; // \ru Сбросить все временные данные. \en Reset all temporary data. 
#ifdef C3D_SIGNAL_ENABLED
  virtual void        Refresh( MbeObjectChangeType ); // \ru Перевести временные данные объекта в неопределённое состояние, учитывая тип произошедших изменений. \en Translate temporal data of the object in an inconsistent state, taking into account type of changes.
#endif
  /** \brief \ru Рассчитать временные (mutable) данные объекта.
             \en Calculate temporary (mutable) data of an object. \~
    \details \ru Рассчитать временные данные объекта в зависимости от параметра forced. 
      Если параметр forced равен false, рассчитываются только ещё не насчитанные данные.
      Если параметр forced равен true, перерасчитываются все временные данные объекта.
             \en Calculate the temporary data of an object depending of the "forced" parameter. 
      Calculate only data that was not calculated earlier if parameter "forced" is equal false.
      Recalculate all temporary data of an object if parameter "forced" is equal true.
    \param[in] forced - \ru Принудительный перерасчёт.
                        \en Forced recalculation. \~
  */
  virtual void        PrepareIntegralData( const bool forced ) const;

  //virtual void        AddYourGabaritTo ( MbRect & ) const = 0; // \ru Добавить в прямоугольник свой габарит. \en Add a bounding box to rectangle.

  /** \ru \name Общие функции двумерного объекта.
      \en \name Common functions of two-dimensional object.
      \{ */
  /** \brief \ru Добавить габарит в прямоугольник.
             \en Add a bounding box to rectangle. \~
    \details \ru Добавить в прямоугольник свой габарит с учетом матрицы трансформации. 
      Если матрица не единичная, то происходит трансформация копии объекта по матрице и 
      затем к прямоугольнику добавляется габарит трансформированного объекта. 
      После использования копия уничтожается.
             \en Add a bounding box to rectangle with taking into account of transformation matrix. 
      If the transformation matrix is not an identity matrix then there is performed a transformation of object's copy by the matrix and 
      after that a bounding box of the transformed object is added to rectangle. 
      A copy is destroyed after using. \~
    \param[out] rect - \ru Прямоугольник с информацией по габаритам.
                       \en A rectangle with information about bounds. \~
    \param[in] matr - \ru Матрица трансформации.
                      \en Transformation matrix. \~
  */
  virtual void        AddYourGabaritMtr( MbRect & rect, const MbMatrix & matr ) const;  
  /** \brief \ru Определить габаритный прямоугольник кривой.
             \en Detect the bounding box of a curve. \~
    \details \ru Для получения габарита объекта присланный прямоугольник делается пустым.
      Затем вычисляются габариты объекта и сохраняются в прямоугольнике rect.
             \en The sent rectangle becomes empty for getting a bounding box.
      Then bounding boxes of an object are calculated and saved into a rectangle 'rect'. \~
  */
  virtual void        CalculateGabarit ( MbRect & ) const;

  /** \brief \ru Рассчитать габарит в локальной системы координат.
             \en Calculate bounding box in the local coordinate system. \~
    \details \ru Для получения габарита объекта относительно локальной системы координат, 
      присланный прямоугольник делается пустым. Затем вычисляются габариты объекта в локальной системе координат
      и сохраняются в прямоугольнике local.
             \en For getting a bounding box of an object relatively to the local coordinate system, 
      a sent rectangle becomes empty. After that bounding boxes of an object in the local coordinate system are calculated
      and saved in a rectangle 'local'. \~ 
    \param[in] into - \ru Матрица перехода от текущей для объекта системы координат к локальной системе координат.
                      \en A transition matrix from the current coordinate system of the object to the local coordinate system. \~
    \param[out] local - \ru Прямоугольник с информацией по габаритам.
                       \en A rectangle with information about bounds. \~
  */
  virtual void        CalculateLocalGabarit( const MbMatrix & into, MbRect & local ) const; 

  /** \brief \ru Определить видимость объекта в прямоугольнике.
             \en Determine visibility of an object in rectangle. \~
    \details \ru Определить, виден ли объект в заданном прямоугольнике. Есть возможность выполнить быструю проверку
      или более тщательную при соответствующем значении второго параметра.
             \en Determine whether an object is visible in the given rectangle. There is a possibility to perform a fast check
      or more thorough check when the second parameter has a corresponding value. \~
    \param[in] rect - \ru Заданный двумерный прямоугольник.
                      \en A given two-dimensional rectangle. \~
    \param[in] exact - \ru Выполнять ли более тщательную проверку?
                       \en Whether to perform a more thorough check. \~
    \return \ru true - объект полностью или частично содержится в прямоугольнике, иначе - false.
            \en true, if the object is fully or partially contained in the rectangle, otherwise false. \~
  */
  bool        IsVisibleInRect ( const MbRect & rect, bool exact = false ) const override;
  
  /** \brief \ru Определить, виден ли объект полностью в прямоугольнике.
             \en Determine whether an object is fully visible in rectangle. \~
    \details \ru Объект полностью содержится в заданном прямоугольнике, если его габаритный прямоугольник вложен в заданный.
             \en An object is fully contained in the given rectangle if its bounding rectangle is included in the given rectangle. \~
    \param[in] rect - \ru Прямоугольник, вложенность в который проверяется.
                      \en Rectangle to check inclusion to. \~
    \return \ru true - объект полностью содержится в прямоугольнике, иначе - false.
            \en true, if the object is fully contained in the rectangle, otherwise false. \~
  */
  virtual bool        IsCompleteInRect( const MbRect & rect ) const; 

  // \ru Определить расстояние до точки. \en Determine the distance to a point. 
  double      DistanceToPoint( const MbCartPoint & toP ) const override;
  // \ru Вычислить расстояние до точки, если оно меньше d. \en Calculate the distance to the point if it is less than d. 
  bool        DistanceToPointIfLess( const MbCartPoint & toP, double & d ) const override; 

  /** \} */
  /** \ru \name Функции описания области определения кривой.
      \en \name Functions for curve domain description.
      \{ */
  /// \ru Получить максимальное значение параметра. \en Get the maximum value of parameter. 
  virtual double      GetTMax () const = 0;
  /// \ru Получить минимальное значение параметра. \en Get the minimum value of parameter. 
  virtual double      GetTMin () const = 0;

  /** \brief \ru Определить, является ли кривая периодической.
             \en Define whether the curve is periodic. \~
    \details \ru Определить, является ли кривая периодической.\n
      Периодическая кривая является замкнутой.
      Кривая считается периодической, если:\n
      - начальная и конечная точка кривой совпадают;\n
      - производные в начальной и конечной точке совпадают; \n
        если по своей природе кривая допускает изломы (контур, ломаная), 
          то допускается не совпадение производных;
        у кривой Безье производные должны совпадать по направлению,
          но могут не совпадать по модулю.\n
      Кривые:\n
      - MbLine,\n
      - MbLineSegment,\n
      - MbCosinusoid,\n
      - MbContour (если он содержит только один сегмент одного из указанных типов)\n
      не считаются периодическими при совпадении начальной и конечной точек.
      Метод IsClosed() для них всегда возвращает false.\~
             \en Define whether the curve is periodic.\n
      A periodic curve is closed.
      A curve is considered as periodic if:\n
      - start point is coincident with end point;\n
      - derivatives in start point and end point coincide; \n
        if there are breaks at curve (in cases when a curve is contour or polyline), 
          then derivatives may not coincide;
        in Bezier curve derivatives should coincide by direction,
          but they may differ by absolute value.\n
      Curves:\n
      - MbLine,\n
      - MbLineSegment,\n
      - MbCosinusoid,\n
      - MbContour (if it contains only a single segment of one of the listed types)\n
      are not considered periodic when the start and end points coincide.
      The IsClosed() method always returns false for them.\~
    \return \ru true, если кривая замкнута.
            \en True if a curve is closed. \~
  */
  virtual bool        IsClosed() const = 0;

  /** \brief \ru Вернуть период.
             \en Return period. \~
    \details \ru Вернуть значение периода, если может быть кривая замкнута. Для незамкнутой кривой вернуть нуль.
             \en Return the period value if a curve can be closed. Let unclosed curve return null. \~
    \return \ru Значение периода для замкнутой кривой или нуль - для незамкнутой.
            \en The value of period for a closed curve or null - for unclosed curve. \~
  */
  virtual double      GetPeriod()  const;
  
  /** \brief \ru Определить, является ли кривая периодической.
             \en Define whether the curve is periodic. \~
    \details \ru Определить, является ли кривая периодической.\n
      Кривая считается периодической, если:\n
      - начальная и конечная точка кривой совпадают;\n
      - производные в начальной и конечной точке совпадают; \n
        если по своей природе кривая допускает изломы (контур, ломаная),
          то допускается не совпадение производных;
        у кривой Безье производные должны совпадать по направлению,
          но могут не совпадать по модулю.\n
      Кривые:\n
      - MbLine,\n
      - MbLineSegment,\n
      - MbCosinusoid,\n
      - MbContour (если он содержит только один сегмент одного из указанных типов)\n
      при совпадении начальной и конечной точек не считаются периодическими.
      Метод IsPeriodic() для них всегда возвращает false.\~
             \en Define whether the curve is periodic.\n
      A curve is considered as periodic if:\n
      - start point is coincident with end point;\n
      - derivatives in start point and end point coincide; \n
        if there are breaks at curve (in cases when a curve is contour or polyline),
          then derivatives may not coincide;
        in Bezier curve derivatives should coincide by direction,
          but they may differ by absolute value.\n
      Curves:\n
      - MbLine,\n
      - MbLineSegment,\n
      - MbCosinusoid,\n
      - MbContour (if it contains only a single segment of one of the listed types)\n
      are not considered periodic when the start and end points coincide.
      The IsPeriodic() method always returns false for them.\~
    \return \ru true, если кривая замкнута.
            \en True if a curve is closed. \~
  */
  virtual bool        IsPeriodic() const;

  /** \brief \ru Определить, замкнута ли кривая фактически независимо от гладкости замыкания.
             \en Determine whether a curve is closed regardless of the smoothness of the closure. \~
    \details \ru Определить, замкнута ли кривая фактически независимо от гладкости замыкания.
             \en Determine whether a curve is actually closed regardless of the smoothness of the closure. \~
  */
  bool        IsTouch( double eps = Math::LengthEps )  const;
  
  /** \} */
  /** \ru \name Функции для работы в области определения кривой. 
            Функции PointOn, FirstDer, SecondDer, ThirdDer,... корректируют параметр
            при выходе его за пределы области определения.
            Исключение составляет MbLine (прямая).
      \en \name Functions for working in the curve's domain. 
            Functions PointOn, FirstDer, SecondDer, ThirdDer,... correct parameter
            when it is out of domain bounds.
            The exception is MbLine (line).
      \{ */
  
  /** \brief \ru Вычислить точку на кривой.
             \en Calculate a point on the curve. \~
    \details \ru Скорректировать параметры при выходе их за пределы области определения и вычислить точку на кривой.
             \en Correct parameter when getting out of domain bounds and calculate a point on the curve. \~
    \param[in] t - \ru Параметр curve.
                   \en Curve parameter. \~ 
    \param[out] p - \ru Вычисленная точка на кривой.
                    \en A point on the curve. \~
    \ingroup Curves_2D
  */
  virtual void        PointOn  ( double & t, MbCartPoint & p ) const = 0;
  /// \ru Вычислить первую производную. \en Calculate first derivative. 
  virtual void        FirstDer ( double & t, MbVector    & v ) const = 0;
  /// \ru Вычислить вторую производную. \en Calculate second derivative. 
  virtual void        SecondDer( double & t, MbVector    & v ) const = 0;
  /// \ru Вычислить третью производную. \en Calculate third derivative. 
  virtual void        ThirdDer ( double & t, MbVector    & v ) const = 0;
  /// \ru Вычислить касательный вектор (нормализованный). \en Calculate tangent vector (normalized). 
  void        Tangent  ( double & t, MbVector    & v ) const;
  /// \ru Вычислить касательный вектор (нормализованный). \en Calculate tangent vector (normalized). 
  void        Tangent  ( double & t, MbDirection & d ) const;
  /// \ru Вычислить вектор главной нормали (нормализованный). \en Calculate main normal vector (normalized). 
  void        Normal   ( double & t, MbVector    & v ) const;
  /// \ru Вычислить вектор главной нормали (нормализованный). \en Calculate main normal vector (normalized). 
  void        Normal   ( double & t, MbDirection & d ) const;
  /** \} */

  /** \ru \name Функции для работы внутри и вне области определения кривой.
            Функции _PointOn, _FirstDer, _SecondDer, _ThirdDer,... не корректируют параметр
            при выходе за пределы области определения. При выходе за область определения незамкнутая
            кривая, в общем случае, продолжается по касательной, которую она имеет на соответствующем конце.
            Исключение составляют дуги эллипса и окружности - они продолжаются в соответствии со своими уравнениями. 
      \en \name Functions for working inside and outside the curve's domain. 
            Functions _PointOn, _FirstDer, _SecondDer, _ThirdDer,... do not correct parameter
            when it is out of domain bounds. When parameter is out of domain bounds, an unclosed
            curve is extended by tangent vector at corresponding end point in general case.
            The exceptions are arcs of ellipse and arcs of circle - they are extended according to their equations.
      \{ */
  
  /** \brief \ru Вычислить точку на кривой и её продолжении. 
             \en Calculate point at curve and its extension. \~
    \details \ru Вычислить точку на кривой в том числе и за пределами области определения параметра.
             \en Calculate a point on the curve including the outside area determination parameter. \~
    \param[in] t - \ru Параметр curve.
                   \en Curve parameter. \~ 
    \param[out] p - \ru Вычисленная точка на кривой.
                    \en A point on the curve. \~
    \ingroup Curves_2D
  */
  virtual void       _PointOn  ( double t, MbCartPoint & p ) const;
  /// \ru Вычислить первую производную на кривой и её продолжении. \en Calculate first derivative at curve and its extension. 
  virtual void       _FirstDer ( double t, MbVector    & v ) const;
  /// \ru Вычислить вторую производную на кривой и её продолжении. \en Calculate second derivative at curve and its extension. 
  virtual void       _SecondDer( double t, MbVector    & v ) const;
  /// \ru Вычислить третью производную на кривой и её продолжении. \en Calculate third derivative at curve and its extension. 
  virtual void       _ThirdDer ( double t, MbVector    & v ) const;
  /// \ru Вычислить касательный вектор (нормализованный). \en Calculate tangent vector (normalized). 
  void       _Tangent  ( double t, MbVector    & v ) const;
  /// \ru Вычислить касательный вектор (нормализованный). \en Calculate tangent vector (normalized). 
  void       _Tangent  ( double t, MbDirection & d ) const;
  /// \ru Вычислить вектор главной нормали (нормализованный) на кривой и её продолжении. \en Calculate main normal vector (normalized) at curve and its extension. 
  void       _Normal   ( double t, MbVector    & v ) const;
  /// \ru Вычислить вектор главной нормали (нормализованный) на кривой и её продолжении. \en Calculate main normal vector (normalized) at curve and its extension. 
  void       _Normal   ( double t, MbDirection & d ) const;

  /** \brief \ru Вычислить значения точки и производных для заданного параметра.
             \en Calculate point and derivatives of object for given parameter. \~
    \details \ru Значения точки и производных вычисляются в пределах области определения и на расширенной кривой.
             \en Values of point and derivatives are calculated on parameter area and on extended curve. \~
    \param[in] t   - \ru Параметр.
                     \en Parameter. \~
    \param[in] ext - \ru В пределах области определения (false), на расширенной кривой (true).
                     \en On parameters area (false), on extended curve (true). \~
    \param[out] pnt  - \ru Точка.
                       \en Point. \~
    \param[out] fir  - \ru Производная.
                       \en Derivative with respect to t. \~
    \param[out] sec  - \ru Вторая производная по t, если не ноль.
                       \en Second derivative with respect to t, if not nullptr. \~
    \param[out] thir - \ru Третья производная по t, если не ноль.
                       \en Third derivative with respect to t, if not nullptr. \~
    \ingroup Curves_3D
  */
  virtual void        Explore( double & t, bool ext,
                               MbCartPoint & pnt, MbVector & fir, MbVector * sec, MbVector * thir ) const;

  /** \} */
  /** \ru \name Функции движения по кривой
      \en \name Function of moving by curve
    \{ */

  /** \brief \ru Вычислить шаг параметра.
             \en Calculate parameter step. \~
    \details \ru Вычислить шаг параметра для аппроксимации кривой по величине прогиба кривой. 
      Вычисление шага проходит с учетом радиуса кривизны.
      Шаг аппроксимации кривой выбирается таким образом, чтобы отклонение кривой от
      её полигона не превышало заданную величину прогиба.
             \en Calculate parameter step for the curve's approximation by its sag value. 
      Calculation of the step is performed with consideration of curvature radius.
      A step of curve's approximation is chosen in such way, that the deviation of a curve from
      its polygon does not exceed the given sag value. \~
    \param[in] t   - \ru Параметр, определяющий точку на кривой, в которой надо вычислить шаг.
                     \en A parameter defining the point on a curve, at which a step should be calculated. \~
    \param[in] sag - \ru Максимально допустимая величина прогиба.
                     \en Maximum feasible sag value. \~
    \return \ru Величина шага по параметру в заданной точке.
            \en A sag value by parameter at given point. \~
  */
  virtual double      Step         ( double t, double sag ) const; 

  /** \brief \ru Вычислить шаг параметра.
             \en Calculate parameter step. \~
    \details \ru Вычислить шаг параметра для аппроксимации кривой по углу отклонения касательной. 
    Шаг аппроксимации кривой выбирается таким образом, 
    чтобы угловое отклонение касательной кривой в следующей точке 
    не превышало заданную величину ang.
             \en Calculate parameter step for the curve's approximation by the deviation angle of the tangent vector. 
    A step of curve's approximation is chosen in such way, 
    that angular deviation of the tangent curve at the next point 
    does not exceed the given value ang. \~
    \param[in] t   - \ru Параметр, определяющий точку на кривой, в которой надо вычислить шаг.
                     \en A parameter defining the point on a curve, at which a step should be calculated. \~
    \param[in] ang - \ru Максимально допустимый угол отклонения касательной.
                     \en The maximum feasible deviation angle of tangent. \~
    \return \ru Величина шага по параметру в заданной точке.
            \en A sag value by parameter at given point. \~
  */
  virtual double      DeviationStep( double t, double ang ) const; 
  /** \} */
  /** \ru \name Общие функции кривой
      \en \name Common function of curve.
      \{ */
  /// \ru Вычислить кривизну кривой. \en Calculate curvature of curve. 
  virtual double      Curvature      ( double t ) const;
  /// \ru Вычислить производную кривизны по параметру. \en Calculate derivative of curvature by parameter. 
  double      CurvatureDerive( double t ) const;
  /// \ru Вычислить радиус кривизны кривой со знаком. \en Calculate radius of curve with a sign. 
  double      CurvatureRadius( double t ) const;

  /** \brief \ru Вычислить метрическую длину кривой.
             \en Calculate the metric length of a curve. \~
    \details \ru Вычислить метрическую длину кривой и записать ее в переменную length.
             \en Calculate the metric length of a curve and save the result in the variable 'length'. \~ 
    \param[in, out] length - \ru Вычисленная длина кривой.
                             \en Calculated length of a curve. \~
    \return \ru true - если длина кривой отлична от нуля. Иначе возвращает false.
            \en True - if the length of a curve differs from null. Otherwise returns false. \~
  */
  virtual bool        HasLength( double & length ) const = 0;
  /// \ru Определить, является ли кривая ограниченной. \en Define whether the curve is bounded. 
  virtual bool        IsBounded()    const;
  /// \ru Определить, является ли кривая прямолинейной. \en Define whether the curve is rectilinear.. 
  virtual bool        IsStraight( bool ignoreParams = false ) const;
  /// \ru Определить, является ли кривая вырожденной. \en Define whether the curve is degenerate.. 
  virtual bool        IsDegenerate( double eps = Math::LengthEps ) const;

  /** \brief \ru Определить, являются ли стыки контура/кривой гладкими.
             \en Define whether joints of contour/curve are smooth. \~
    \details \ru Определить, являются ли стыки контура/кривой гладкими.\n
             \en Define whether joints of contour/curve are smooth. \n \~
    \param[in] angleEps - \ru Угловая точность для проверки гладкости.
                          \en Angular accuracy for checking smoothness. \~
    \param[in] ignoreLimits - \ru Не проводить проверку на концах замкнутой кривой.
                              \en Do not check at the ends of a closed curve. \~
    \return \ru true, если кривая гладкая.
            \en true if the curve is smooth. \~
  */
  virtual bool        IsSmoothConnected( double angleEps, bool ignoreLimits = false ) const;

  /// \ru Вычислить параметрическую длину кривой. \en Calculate the parametric length of a curve. 
  double      GetParamLength() const { return GetTMax() - GetTMin(); }
  // \ru Функции с расчетом метрической длины перегружать все сразу, чтобы не было рассогласования. \en Functions with calculation of metric length, they should be overloaded simultaneously to avoid  mismatches. 
  /// \ru Вычислить метрическую длину кривой. \en Calculate the metric length of a curve. 
  virtual double      CalculateMetricLength() const;

  /** \brief \ru Вычислить метрическую длину кривой.
             \en Calculate the metric length of a curve. \~
    \details \ru Вычислить метрическую длину разомкнутой кривой от параметра t1 до t2. 
      Должно выполнятся условие t1 < t2.
             \en Calculate the metric length of unclosed curve from parameter t1 to parameter t2. 
      The condition t1 < t2 should satisfied. \~
    \param[in] t1 - \ru Начальный параметр отрезка кривой.
                    \en Start parameter of a curve section. \~
    \param[in] t2 - \ru Конечный параметр отрезка кривой.
                    \en End parameter of a curve section. \~
    \return \ru Длина кривой.
            \en Length of a curve. \~
  */
  virtual double      CalculateLength( double t1, double t2 ) const; 

  /** \brief \ru Вычислить метрическую длину кривой.
             \en Calculate the metric length of a curve. \~
    \details \ru Если длина кривой уже была вычислена и запомнена в объекте, эта функция возвращает готовый результат,
      не выполняя повторных вычислений. Иначе длина вычисляется с помощью функции CalculateMetricLength().
             \en If a length of a curve is already calculated and saved in the object then this function returns the existing result,
      without repeating of calculations. Otherwise the length is calculated by the function CalculateMetricLength(). \~
    \return \ru Длина кривой.
            \en Length of a curve. \~
  */
  virtual double      GetMetricLength() const = 0; 
  
  /** \brief \ru Сдвинуть параметр вдоль кривой.
             \en Translate parameter along the curve. \~
    \details \ru Сдвинуть параметр вдоль кривой на заданное расстояние в заданном направлении. 
      Новое значение параметра сохраняется в переменной t. Если кривая не замкнута и длина ее части от точки с параметром t до конца в заданном направлении
      меньше, чем нужное смещение, то вычисления происходят на продолжении кривой, если можно построить продолжение.
             \en Translate parameter along the curve by the given distance at the given direction. 
      The new value of parameter is saved in the variable t. If the curve is not closed and the length of its part from the point with parameter t to the end at the given direction
      is less than the required shift, then calculations are performed on extension of the curve, if it possible to construct such extension. \~
    \param[in, out] t - \ru На входе - исходное значение параметра. На выходе - новое значение параметра.
                        \en Input - the initial value of parameter. Output - the new value of parameter. \~
    \param[in] len - \ru Величина смещения вдоль кривой.
                     \en The value of shift along the curve. \~
    \param[in] curveDir - \ru Направление смещения. Если curveDir - неотрицательно, то смещение направлено в сторону увеличения параметра. 
      Иначе - в сторону уменьшения параметра.
                          \en The offset direction. If curveDir is non-negative, then the shift is directed to the side of increasing of parameter. 
      Otherwise - to the side of decreasing of parameter. \~
    \param[in] eps - \ru Точность вычислений.
                     \en Computational tolerance. \~
    \param[in] version - \ru Версия.
                         \en Version. \~
    \return \ru true - если операция выполнена успешно, иначе false.
            \en True - if the operation is performed successfully, otherwise false. \~ 
  */
  virtual bool        DistanceAlong( double & t, double len, int curveDir, double eps = Math::LengthEps,
                                     VERSION version = Math::DefaultMathVersion() ) const;

  /// \ru Сбросить текущее значение параметра. \en Reset the current value of parameter. 
  virtual void        ResetTCalc() const;
  /// \ru Изменить направления кривой на противоположное. \en Set the opposite direction of curve. 
  virtual void        Inverse( MbRegTransform * iReg = nullptr ) = 0;
  /// \ru Построить эквидистантную кривую, смещённую на заданное расстояние. \en Construct the equidistant curve which is shifted by the given value. 
  virtual MbCurve *   Offset( double rad ) const;

  /** \brief \ru Начать аппроксимацию для отрисовки.
             \en Start approximation for the drawing. \~
    \details \ru В функции проверяются и при необходимости корректируются параметры начала и конца аппроксимируемой части кривой.
      Вычисляется первая точка, соответствующая начальному параметру. Определяется, можно ли вычислить следующую точку. 
      Следующие точки вычисляются функцией GetNextPoint.
             \en Parameters of start and end of approximated part of a curve are checked and corrected this is necessary.
      There is calculated a first point corresponding to start parameter. There is defined whether it is possible to calculate the next point. 
      The next points are calculated by the function GetNextPoint. \~
    \param[in] sag - \ru Максимальная величина прогиба.
                     \en Maximal value of sag. \~
    \param[in, out] tbeg - \ru Параметр, соответствующий началу аппроксимируемой части кривой.
                           \en Parameter corresponding to start of approximated part of a curve. \~
    \param[in, out] tend - \ru Параметр, соответствующий концу аппроксимируемой части кривой.
                           \en Parameter corresponding to end of approximated part of a curve. \~
    \param[in, out] pnt - \ru Вычисленная точка.
                          \en A calculated point. \~
    \param[in, out] existNextPoint - \ru Флаг, показывающий, надо ли вычислять следующую точку (true) 
      или вычисленная точка соответствует концу аппроксимируемой кривой (false).
                                     \en Flag showing whether the next point should be calculated (true by default) 
      or calculated point corresponds to the end of approximated curve. \~
    \return \ru true - если операция выполнена успешно, иначе false.
            \en True - if the operation is performed successfully, otherwise false. \~ 
  */
  virtual bool        BeginApprox     ( double sag, double & tbeg, double & tend, MbCartPoint & pnt, bool & existNextPoint ) const; 

  /** \brief \ru Вычислить очередную точку.
             \en Calculate the next point. \~
    \details \ru Функция используется для расчета аппроксимации кривой, после вызова функции BeginApprox.
      В ней определяется параметр для вычисления следующей точки полигона, вычисляется точка и определяется, является ли она конечной.
             \en This function is used for the calculation of curve's approximation after call of the function BeginApprox.
      In this function a parameter for calculation of the next point of the polygon is defined, a point is calculated and there is defined whether it is an end point. \~
    \param[in] sag - \ru Максимальная величина прогиба.
                     \en Maximal value of sag. \~
    \param[in] tend - \ru Параметр, соответствующий концу аппроксимируемой части кривой.
                      \en Parameter corresponding to end of approximated part of a curve. \~
    \param[in, out] tcur - \ru На входе - значение параметра в последней вычисленной точке. На выходе - параметр, соответствующий новой
      вычисленной точке.
                           \en Input - the value of parameter at the last calculated point. Output - parameter corresponding to the new
      calculated point. \~
    \param[in, out] pnt - \ru Вычисленная точка.
                          \en A calculated point. \~
    \return \ru true - если необходимы дальнейшие вычисления. false - если вычисленная точка соответствует концу аппроксимируемой кривой.
            \en True - if the further calculations are required. false - if the calculated point corresponds to the end of approximated curve. \~ 
  */
  virtual bool        GetNextPoint    ( double sag, double tend, double & tcur, MbCartPoint & pnt ) const; 
  
  /** \brief \ru Рассчитать массив точек для отрисовки.
             \en Calculate an array of points for drawing. \~
    \details \ru Выдать массив отрисовочных точек с заданной стрелкой прогиба. 
      Если кривая представляет собой контур, то узловые точки контура дублируются.
             \en Get an array of drawn points with a given sag. 
      If the cure is a contour then knots of a contour are duplicated. \~
    \param[in] sag - \ru Максимальная величина прогиба.
                     \en Maximal value of sag. \~
    \param[in, out] poligon - \ru Полигон рассчитанных точек на кривой.
                              \en A polygon of calculated points on a curve. \~
  */
  virtual void        CalculatePolygon( double sag, MbPolygon & poligon ) const;

  /** \brief \ru Построить NURBS копию кривой.
             \en Construct a NURBS copy of a curve. \~
    \details \ru Строит NURBS кривую, аппроксимирующую заданную. По возможности, строит точную кривую, возможно с кратными узлами.
      Количество узлов для NURBS определяется в зависимости от кривой.
             \en Constructs a NURBS copy which approximates a given curve. If it is possible, constructs the accurate curve, perhaps with multiple knots.
      The number of knots for NURBS is defined depending on the curve. \~
    \param[in, out] nurbs - \ru Построенная NURBS кривая.
                            \en A constructed NURBS-curve. \~
    \param[in] nInfo - \ru Параметры преобразования кривой в NURBS.
                       \en Parameters of conversion of a curve to NURBS. \~
    \result \ru Построенная NURBS кривая или nullptr при неуспешном построении.
            \en The constructed NURBS curve or nullptr in a case of failure. \~
  */
          MbNurbs *   NurbsCurve( const MbCurveIntoNurbsInfo * nInfo = nullptr ) const;
  
  /** \brief \ru Построить NURBS копию кривой.
             \en Construct a NURBS copy of a curve. \~
    \details \ru Строит NURBS кривую, аппроксимирующую заданную в диапазоне параметров [t1, t2] с заданным направлением. 
      По возможности, строит точную кривую, возможно с кратными узлами.
      Количество узлов для NURBS определяется в зависимости от кривой.
             \en Constructs a NURBS curve which approximates a given curve inside the range [t1, t2]. with a given direction. 
      If it is possible, constructs the accurate curve, perhaps with multiple knots.
      The number of knots for NURBS is defined depending on the curve. \~
    \param[in, out] nurbs - \ru Построенная NURBS кривая.
                            \en A constructed NURBS-curve. \~
    \param[in] t1 - \ru Параметр, соответствующий началу аппроксимируемой части кривой.
                    \en Parameter corresponding to start of approximated part of a curve. \~
    \param[in] t2 - \ru Параметр, соответствующий концу аппроксимируемой части кривой.
                    \en Parameter corresponding to end of approximated part of a curve. \~
    \param[in] sense - \ru Совпадает ли направление возрастания параметра вдоль NURBS кривой с направлением на исходной кривой.
      sense > 0 - направление совпадает.
                       \en Does the direction of parameter increasing along the NURBS curve coincide with direction of the initial curve.
      'sense' > 0 - direction coincide. \~ 
    \param[in] nInfo - \ru Параметры преобразования кривой в NURBS.
                       \en Parameters of conversion of a curve to NURBS. \~
    \result \ru Построенная NURBS кривая или nullptr при неуспешном построении.
            \en The constructed NURBS curve or nullptr in a case of failure. \~
  */
  virtual MbNurbs *   NurbsCurve( const MbCurveIntoNurbsInfo & nInfo ) const = 0;

  /** \brief \ru Построить NURBS копию кривой.
             \en Construct a NURBS copy of a curve. \~
    \details \ru Строит NURBS кривую, аппроксимирующую исходную с заданными параметрами. 
      В параметрах можно задать степень и количество узлов сплайна, диапазон изменения параметра кривой.
      Если в параметрах не задан флаг точной аппроксимации, то строит NURBS без кратных узлов.
             \en Constructs a NURBS curve which approximates a given curve with the given parameters. 
      In parameters the degree and the number of knots of a spline and the range of curve's parameters changing may be set.
      If the flag of accurate approximation is not set in parameters then NURBS without multiple knots is constructed. \~
    \param[in] tParameters - \ru Параметры построения NURBS копии кривой.
                             \en Parameters for the construction of a NURBS copy of the curve. \~
    \result \ru Построенная NURBS кривая или nullptr при неуспешном построении.
            \en The constructed NURBS curve or nullptr in a case of failure. \~
  */
  virtual MbCurve *   NurbsCurve( const MbNurbsParameters & tParameters ) const;

  /** \brief \ru Построить усеченную кривую.
             \en Construct a trimmed curve. \~
    \details \ru Строит усеченную кривую, начало которой соответствует точке с параметром t1 и
      конец - точке с параметром t2. 
      Можно изменить направление полученной кривой относительно исходной с помощью параметра sense. 
      Если кривая замкнута, можно получить усеченную кривую, проходящую через
      начало кривой.\n
      В случае замкнутой кривой (или для дуги - исключение) три параметра sense, t1 и t2 однозначно
      определяют результат. 
      В случае разомкнутой кривой параметр sense и параметрами усечения должны соответствовать друг другу:\n
      1) если sense ==  1, то t1 < t2,\n 
      2) если sense == -1, то t1 > t2.\n
      Если есть несоответствие между sense и параметрами усечения, то 
      приоритетным параметром считается sense.
      Если параметры t1 и t2 равны и кривая замкнута, в результате должны получить замкнутую кривую.
             \en Constructs a trimmed curve, a start point of which corresponds to a point with parameter t1 and
      an end point corresponds to a point with parameter t2. 
      Direction of the constructed curve relative to the initial curve may be changed by the parameter 'sense'. 
      If the curve is closed, then there may be obtained a trimmed curve, passing through
      the start of a curve.\n
      In a case of closed curve (or for an arc - exception) three parameters 'sense', t1 and t2 clearly
      define the result. 
      In a case of unclosed curve the parameter 'sense' and parameter of trimming should correspond each other:\n
      1) if sense ==  1, then t1 < t2,\n 
      2) if sense ==  -1, then t1 > t2,\n
      If there is a discrepancy between 'sense' and parameters of trimming, then 
      'sense' parameter has higher priority.
      If parameters t1 and t2 are equal and the curve is closed, then in result a closed curve should be obtained. \~
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
    \param[in] saveParLenAndLaw - \ru Сохранять параметрическую длину и закон.
                                  \en Save parametric length and law. \~
    \internal \ru При изменении поведения или документации метода переделать юнит-тестирование.
              \en When changing of the behavior or the documentation of the method being performed, the unit-testing should be redone. \~ \endinternal
    \result \ru Построенная усеченная кривая.
            \en A constructed trimmed curve. \~
  */
  virtual MbCurve * Trimmed( double t1, double t2, int sense, bool saveParamLenAndLaw = false ) const;

  /** \brief \ru Построить усеченную кривую c учетом заданной двумерной точности.
             \en Construct a trimmed curve with the given two-dimensional accuracy. \~
    \details \ru Строит усеченную кривую c учетом заданной двумерной точности на плоскости,
      начало которой соответствует точке с параметром t1 и конец - точке с параметром t2. 
      Можно изменить направление полученной кривой относительно исходной с помощью параметра sense. 
      Если кривая замкнута, можно получить усеченную кривую, проходящую через
      начало кривой.\n
      В случае замкнутой кривой (или для дуги - исключение) три параметра sense, t1 и t2 однозначно
      определяют результат. 
      В случае разомкнутой кривой параметр sense и параметрами усечения должны соответствовать друг другу:\n
      1) если sense ==  1, то t1 < t2,\n 
      2) если sense == -1, то t1 > t2.\n
      Если есть несоответствие между sense и параметрами усечения, то 
      приоритетным параметром считается sense.
      Если параметры t1 и t2 равны и кривая замкнута, в результате должны получить замкнутую кривую.
             \en Constructs a trimmed curve, a start point of which corresponds to a point with parameter t1 and
      an end point corresponds to a point with parameter t2. 
      Direction of the constructed curve relative to the initial curve may be changed by the parameter 'sense'. 
      If the curve is closed, then there may be obtained a trimmed curve, passing through
      the start of a curve.\n
      In a case of closed curve (or for an arc - exception) three parameters 'sense', t1 and t2 clearly
      define the result. 
      In a case of unclosed curve the parameter 'sense' and parameter of trimming should correspond each other:\n
      1) if sense ==  1, then t1 < t2,\n 
      2) if sense ==  -1, then t1 > t2,\n
      If there is a discrepancy between 'sense' and parameters of trimming, then 
      'sense' parameter has higher priority.
      If parameters t1 and t2 are equal and the curve is closed, then in result a closed curve should be obtained. \~
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
    \param[in] xyEps - \ru Двумерная точность. Используется для оценок вблизи точек, соответствующих параметрам t1 и t2.
                       \en Two-dimensional accuracy. It is used for estimations near the points corresponding to the parameters t1 and t2. \~
    \param[in] saveParLenAndLaw - \ru Сохранять параметрическую длину и закон.
                                  \en Save parametric length and law. \~
    \internal \ru При изменении поведения или документации метода переделать юнит-тестирование.
              \en When changing of the behavior or the documentation of the method being performed, the unit-testing should be redone. \~ \endinternal
    \result \ru Построенная усеченная кривая.
            \en A constructed trimmed curve. \~
    \warning \ru В разработке. \en Under development.
  */
  virtual MbCurve * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyEps, bool saveParamLenAndLaw ) const = 0;

  /// \ru Аппроксимировать кривую контуром из NURBS-кривых. \en Approximate of a curve by the contour from NURBS curves. 
  virtual MbContour * NurbsContour() const;

  /** \brief \ru Деформировать кривую.
             \en Deform the curve. \~
    \details \ru Если габаритный прямоугольник кривой пересекаться с заданным, 
      то кривая трансформируется в соответствии с матрицей с помощью функции Transform.
             \en If the bounding rectangle of a curve intersects the given one, 
      then the curve is transformed according to the matrix with a help of 'Transform' function. \~
    \param[in] rect - \ru Прямоугольник, в котором проверяется видимость кривой.
                      \en A rectangle, in which the visibility of a curve is checked. \~
    \param[in] matr - \ru Матрица деформации.
                      \en A deformation matrix. \~
    \result \ru Состояние кривой после деформации.
            \en A state of a curve after deformation. \~
  */
  virtual MbeState    Deformation( const MbRect & rect, const MbMatrix & matr ); 

  /// \ru Определить видимость кривой в прямоугольнике. \en Determine visibility of a curve in rectangle. 
  virtual bool        IsInRectForDeform( const MbRect & ) const; 

  /** \brief \ru Удалить часть кривой.
             \en Delete the piece of a curve. \~
    \details \ru Удалить часть кривой между параметрами t1 и t2. Если после удаления кривая распалась на две части,
      то исходный объект соответствует начальной части кривой, а в параметре part2 содержится конечная часть кривой. 
      Если осталась односвязной, то изменяется только исходный объект.
             \en Delete a part of a curve between parameters t1 and t2. If the curve is split into two parts after deletion,
      then the initial object corresponds to the start part of a curve, and parameter 'part2' contains the end part of a curve. 
      If the curve remained simply connected, then only the initial object changes. \~
    \param[in] t1 - \ru Начальный параметр усечения.
                    \en Start parameter of trimming. \~
    \param[in] t2 - \ru Конечный параметр усечения.
                    \en End parameter of trimming. \~
    \param[in, out] part2 - \ru Конечная часть кривой после удаления, если исходная кривая распалась на части.
                            Может являться единственной частью кривой после удаления, \
                            если не смогли изменить саму кривую (например, для прямой MbLine),
                            в этом случае возвращаемый результат dp_Degenerated.
                            \en The end part of a curve after deletion, if an initial curve is split into parts.
                            It may be the only part after deletions, \
                            if the curve did not change (e. g. for a curve of MbLine type),
                            in this case the returned value is dp_Degenerated. \~
    \result \ru Состояние кривой после модификации.
            \en A state of a curve after modification. \~
  */
  virtual MbeState    DeletePart( double t1, double t2, MbCurve *& part2 ) = 0; 

  /** \brief \ru Оставить часть кривой.
             \en Keep the piece of a curve. \~
    \details \ru Оставить часть кривой между параметрами t1 и t2.\n
      В случае успеха операции возвращаемое значение равно dp_Changed и
      кривая удовлетворяет следующим условиям:\n
      - если исходная кривая замкнута, то начальная точка усеченной кривой должна 
      соответствовать параметру t1, конечная - параметру t2,
      - если исходная кривая не замкнута, то начальная точка усеченной кривой должна
      соответствовать минимальному параметру из t1 и t2, конечная - максимальному.
             \en Leave a part of a curve between parameters t1 and t2.\n
      In a case of success the returned value equals dp_Changed and
      a curve satisfies to the next conditions:\n
      - if an initial curve is closed then the start point of a trimmed curve should 
      correspond to the parameter t1, the end point - to the parameter t2,
      - if an initial curve is not closed then the start point of a trimmed curve should
      correspond to the minimum parameter from t1 and t2, the end point - to the maximum one. \~ 
    \param[in] t1 - \ru Начальный параметр усечения.
                    \en Start parameter of trimming. \~
    \param[in] t2 - \ru Конечный параметр усечения.
                    \en End parameter of trimming. \~
    \param[in, out] part2 - \ru Может заполниться результатом усечения, если не смогли изменить саму кривую.
                            В этом случае возвращаемый результат dp_Degenerated.
                            Иначе = nullptr.
                            \en This may be filled by a result of trimming if the curve was not changed.
                            In this case the returned value is dp_Degenerated.
                            Otherwise nullptr is returned. \~
    \result \ru Состояние кривой после модификации:\n
            dp_Degenerated - кривая выродилась, может быть три варианта: 
                             кривая не была изменена, так как в результате преобразования она бы выродилась,
                             или не была изменена, а результат усечения - part2,\n
            dp_NoChanged   - кривая не изменилась,\n
            dp_Changed     - кривая изменилась.
            \en A state of a curve after modification:\n
            dp_Degenerated - the curve is degenerated and there are possible three cases: 
                             the curve was not changed, because it would degenerate in a result of transformation,
                             or it it was not changed and the result of trimming is 'part2',\n
            dp_NoChanged   - the curve was not changes, \n
            dp_Changed     - the curve is changed. \~
    \warning \ru Функция предназначена для внутреннего использования.
             \en The function is designed for internal use only. \~
  */
  virtual MbeState    TrimmPart ( double t1, double t2, MbCurve *& part2 ) = 0; 

  /** \brief \ru Определить положение точки относительно кривой.
             \en Define the point position relative to the curve. \~
    \details \ru Определяется, как расположена точка относительно кривой, если двигаться по кривой в положительном направлении.
             \en There is defined on which side from a curve the point is located, by the positive direction of a curve. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
    \param[in] eps - \ru Точность определения.
                     \en A tolerance of detection. \~
    \result \ru iloc_InItem = 1 - если точка находится слева от кривой, \n
      iloc_OnItem = 0 - если точка находится на кривой, \n
      iloc_OutOfItem = -1 - если точка находится справа от кривой.
            \en Iloc_InItem = 1 - if the point is on the left from a curve, \n
      iloc_OnItem = 0 - if the point is on a curve, \n
      iloc_OutOfItem = 1 - if the point is on the right from a curve. \~
  */
  virtual MbeItemLocation PointRelative( const MbCartPoint & pnt, double eps = Math::LengthEps ) const;

  /// \ru Положение точки относительно кривой. \en The point position relative to the curve. 
  virtual MbeLocation PointLocation( const MbCartPoint & pnt, double eps = Math::LengthEps ) const;

  /** \brief \ru Вычислить проекцию точки на кривую.
             \en Calculate the point projection to the curve. \~
    \details \ru Вычислить параметр кривой, соответствующий проекции заданной точки на эту кривую. \n
                 Эта функция не допускает выхода параметра за параметрическую область определения незамкнутой кривой в отличие от функции #NearPointProjection.
             \en Calculate the parameter of the curve corresponding to the projection of the given point on this curve. \n
                 This function does not allow the parameter to go beyond the parametric domain of the definition of an unclosed curve, unlike the #NearPointProjection function. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
    \result \ru Найденный параметр проекции точки на кривую.
            \en The found parameter for the projection of a point onto the curve. \~
  */
  virtual double      PointProjection( const MbCartPoint & pnt ) const;

  /** \brief \ru Найти проекцию точки на кривую.
             \en Find the point projection to the curve. \~
    \details \ru Найти проекцию точки на кривую или ее продолжение методом Ньютона по заданному начальному приближению.
             \en Find the point projection to the curve or its extension by the Newton method with the given initial approximation. \~
    \param[in] p - \ru Заданная точка.
                   \en A given point. \~
    \param[in] xEpsilon - \ru Точность определения проекции по оси x.
                          \en A tolerance of detection of the projection by x axis. \~
    \param[in] yEpsilon - \ru Точность определения проекции по оси y.
                          \en A tolerance of detection of the projection by y axis. \~
    \param[in] iterLimit - \ru Максимальное количество итераций.
                           \en The maximum number of iterations. \~
    \param[in] t - \ru На входе - начальное приближение, на выходе - параметр кривой, соответствующий ближайшей проекции.
                   \en Input - initial approximation, output - parameter of a curve, corresponding to the nearest projection. \~
    \param[in] ext - \ru Флаг, определяющий, искать ли проекцию на продолжении кривой (если true, то искать).
                     \en A flag defining whether to seek projection on the extension of the curve. \~ 
    \result \ru Результат выполнения итерационного метода.
            \en The result of the iterative method. \~
  */
          MbeNewtonResult PointProjectionNewton( const MbCartPoint & p, double xEpsilon, double yEpsilon, 
                                                 size_t iterLimit, double & t, bool ext ) const;

  /** \brief \ru Найти проекцию точки на кривую.
             \en Find the point projection to the curve. \~
    \details \ru Найти ближайшую проекцию точки на кривую (в диапазоне кривой) или на ее продолжение по заданному начальному приближению.
              Если параметр ext = true, то также выполняется поиск проекции на продолжение кривой.
              Если задан диапазон изменения параметра 'tRange', то нужно найти проекцию в заданном диапазоне.
              Диапазон параметра может выходить за область определения параметра кривой.
              Используется метод Ньютона.
             \en Find the nearest projection of a point on a curve (in the range of the curve) or on its continuation
              by the given initial approximation.
              If the parameter ext = true, then also search for a projection on the continuation of the curve.
              If the range of change of the 'tRange' parameter is specified, then find the projection in the specified range.
              A range of parameter may not belong to the domain of a curve.
              The Newton method is used. \~
    \note    \ru Математическое ядро обеспечивает потокобезопасную реализацию функции для своих объектов. 
             \en Mathematical kernel provides a thread-safe function implementation for its objects. \~
    \param[in] pnt -      \ru Заданная точка.
                          \en A given point. \~
    \param[in] xEpsilon - \ru Точность определения проекции по оси x.
                          \en A tolerance of detection of the projection by x axis. \~
    \param[in] yEpsilon - \ru Точность определения проекции по оси y.
                          \en A tolerance of detection of the projection by y axis. \~
    \param[in,out] t -    \ru На входе - начальное приближение, на выходе - параметр кривой, соответствующий ближайшей проекции.
                          \en Input - initial approximation, output - parameter of a curve corresponding to the nearest projection. \~
    \param[in] ext -      \ru Флаг, определяющий, искать ли проекцию на продолжении кривой (если true, то искать).
                          \en A flag defining whether to seek projection on the extension of the curve. \~ 
    \param[in] tRange -   \ru Диапазон изменения параметра, в котором надо найти решение.
                          \en A range of parameter changing in which the solution should be found. \~
    \return \ru Возвращает true, если найденный параметр находится в допустимом диапазоне (в соответствии с заданными параметрами ext, tRange),
            или false - в противном случае.
            \en Returns true if the found parameter is in a valid range (according to the given ext, tRange parameters),
            or false - otherwise. \~
  */
  virtual bool        NearPointProjection( const MbCartPoint & pnt, double xEpsilon, double yEpsilon, 
                                           double & t, bool ext, MbRect1D * tRange = nullptr ) const;

  /** \brief \ru Вычислить проекцию точки на кривую.
             \en Calculate the point projection to the curve. \~
    \details \ru Вычислить точку на кривой, соответствующую проекции заданной точки на эту кривую.
             \en Calculate the point on the curve corresponding to the projection of the given point on this curve. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
    \param[in, out] on - \ru Искомая точка - проекция.
                         \en The required point - projection. \~
  */
  void        PointProjection( const MbCartPoint & pnt, MbCartPoint & on ) const;

  /** \brief \ru Вычислить проекцию точки на кривую.
             \en Calculate the point projection to the curve. \~
    \details \ru Вычислить точку на кривой, соответствующую проекции заданной точки на эту кривую.
      Если кривая - усеченная, то вычисляется проекция на базовую кривую.
             \en Calculate the point on the curve corresponding to the projection of the given point on this curve.
      If a curve is trimmed then a projection to the base curve is calculated. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
    \param[in, out] on - \ru Искомая точка - проекция.
                         \en The required point - projection. \~
  */
  void        BasePointProjection( const MbCartPoint & pnt, MbCartPoint & on ) const;

  /** \brief \ru Вычислить проекцию точки на кривую.
             \en Calculate the point projection to the curve. \~
    \details \ru Вычислить точку на кривой, соответствующую проекции заданной точки на эту кривую.
      Дополнительно возвращает угол наклона касательной к оси 0X в точке проекции.
             \en Calculate the point on the curve corresponding to the projection of the given point on this curve.
      Additionally returns an inclination angle of a tangent to the axis OX at the point of projection. \~
    \param[in, out] on - \ru На входе - исходная точка. На выходе - точка-проекция на кривой.
                         \en Input - an initial point. Output - a projection point on a curve. \~
    \param[in, out] angle - \ru Вычисленный угол наклона касательной к оси 0X.
                            \en A calculated inclination angle of a curve to the axis OX. \~
  */
  void        PointProjectionAndAngle( MbCartPoint & on, double & angle ) const;

  /** \brief \ru Вычислить проекцию точки на кривую.
             \en Calculate the point projection to the curve. \~
    \details \ru Вычислить ближайшую точку пересечения кривой с лучом, выходящим из заданной точки pnt по направлению dir.
                 Рассматриваются точки, лежащие за начальной точкой луча pnt на расстоянии, превосходящем Math::paramEpsilon.
             \en Calculate the nearest point of intersection between a curve and a ray from the given point 'pntp by the direction 'dir'. 
                 We consider the points lying over the starting point pnt beam at a distance exceeding Math :: paramEpsilon. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
    \param[in] dir - \ru Заданное направление.
                     \en A given direction. \~
    \param[in, out] pp - \ru Искомая точка на кривой.
                         \en Required point on the curve. \~
  */
  bool        DirectPointProjection( const MbCartPoint & pnt,
                                     const MbDirection & dir, MbCartPoint & pp ) const;

  /** \brief \ru Найти ближайший перпендикуляр к кривой.
             \en Find the nearest perpendicular to the curve. \~
    \details \ru Найти ближайший перпендикуляр к кривой, опущенный из заданной точки.
      В этой функции не рассматриваются перпендикуляры, опущенные на продолжение кривой.
             \en Find the nearest perpendicular to the curve from the given point.
      In this function perpendiculars to an extension of a curve are not considered. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
    \param[in, out] tProj - \ru Параметр на кривой, соответствующий точке на кривой, через которую проходит перпендикуляр.
                            \en Parameter on a curve, corresponding to the point on a curve, which the perpendicular is passed through. \~
    \return \ru true, если искомый перпендикуляр построен.
            \en True if the required perpendicular is constructed. \~
  */
  virtual bool        SmallestPerpendicular( const MbCartPoint & pnt, double & tProj ) const; 

  /** \brief \ru Найти касательные к кривой.
             \en Find tangents to a curve. \~
    \details \ru Найти все касательные к кривой, которые можно провести из заданной точки.
      Точка может лежать на кривой. В данной функции рассматривается кривая без продолжений.
             \en Find all tangents to a curve from the given point.
      A point may belong to a curve. In this function a curve without extensions is considered. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
    \param[in, out] tFind - \ru Массив параметров кривой, соответствующих точкам касания.
                            \en An array of parameters of a curve, corresponding to the tangent points. \~
  */
  virtual void        TangentPoint( const MbCartPoint & pnt, SArray<double> & tFind ) const; 

  /** \brief \ru Найти перпендикуляры к кривой.
             \en Find perpendiculars to a curve. \~
    \details \ru Найти все перпендикуляры к кривой, которые можно провести из заданной точки.
      В данной функции рассматривается кривая без продолжений.
             \en Find all perpendiculars to a curve from the given point.
      In this function a curve without extensions is considered. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
    \param[in, out] tFind - \ru Массив параметров кривой, соответствующих точкам на кривой, через которые проходят перпендикуляры.
                            \en An array of parameter on a curve, corresponding to the points on a curve, which the perpendiculars are passed through. \~
  */
  virtual void        PerpendicularPoint( const MbCartPoint & pnt, SArray<double> & tFind ) const;

  /** \brief \ru Найти пересечения кривой с горизонтальной прямой.
             \en Find intersections of a curve with horizontal line. \~
    \details \ru Найти пересечения кривой с горизонтальной прямой.\n
             \en Find intersections of a curve with horizontal line.\n \~
    \param[in] y - \ru Ордината точек горизонтальной прямой.
                   \en An ordinate of points of a horizontal line. \~
    \param[in, out] cross - \ru Массив параметров кривой, соответствующих точкам пересечения.
                            \en An array of parameters of a curve corresponding to the intersection points. \~
  */
  virtual void        IntersectHorizontal( double y, SArray<double> & cross ) const;

  /** \brief \ru Найти пересечения кривой с вертикальной прямой.
             \en Find intersections of a curve with vertical line. \~
    \details \ru Найти пересечения кривой с вертикальной прямой.\n
             \en Find intersections of a curve with vertical line.\n \~
    \param[in] x - \ru Абсцисса точек вертикальной прямой.
                   \en An abscissa of points of a vertical line. \~
    \param[in, out] cross - \ru Массив параметров кривой, соответствующих точкам пересечения.
                            \en An array of parameters of a curve corresponding to the intersection points. \~
  */
  virtual void        IntersectVertical  ( double x, SArray<double> & cross ) const;

  /** \brief \ru Построить изоклины.
             \en Construct isoclines. \~
    \details \ru Построить прямые, проходящие под углом к оси 0X и касательные к кривой.
             \en Construct lines at an angle to the axis OX and tangent to the curve. \~
    \param[in] angle - \ru Вектор, определяющий угол наклона прямой к оси OX.
                       \en A vector defining an inclination angle of line to the axis OX. \~
    \param[in, out] tFind - \ru Массив параметров кривой, соответствующих точкам касания.
                            \en An array of parameters of a curve, corresponding to the tangent points. \~
  */
  virtual void        Isoclinal( const MbVector & angle, SArray<double> & tFind ) const; 

  /** \brief \ru Построить горизонтальные изоклины.
             \en Construct horizontal isoclines. \~
    \details \ru Построить горизонтальные прямые, касательные к кривой.
             \en Construct horizontal lines tangent to the curve. \~
    \param[in, out] tFind - \ru Массив параметров кривой, соответствующих точкам касания.
                            \en An array of parameters of a curve, corresponding to the tangent points. \~
  */
  void        HorzIsoclinal( SArray<double> & tFind ) const; 

  /** \brief \ru Построить вертикальные изоклины.
             \en Construct vertical isoclines. \~
    \details \ru Построить вертикальные прямые, касательные к кривой.
             \en Construct vertical lines tangent to the curve. \~
    \param[in, out] tFind - \ru Массив параметров кривой, соответствующих точкам касания.
                            \en An array of parameters of a curve, corresponding to the tangent points. \~
  */
  void        VertIsoclinal( SArray<double> & tFind ) const; 

  /// \ru Найти нижнюю точку кривой и соответствующий ей параметр. \en Find the lowest point of a curve and the corresponding parameter. 
  void        LowestPoint( MbCartPoint & lowestPoint, double & tLowest ) const; 

  /** \brief \ru Найти самопересечения кривой.
             \en Find self-intersections of curve. \~
    \details \ru Найти точки самопересечения кривой и соответствующие им параметры.
             \en Find the points of self-intersection of a curve and the corresponding parameters. \~
    \param[in, out] crossPnt - \ru Массив точек самопересечения.
                               \en An array of points of self-intersection. \~
  */
  virtual void        SelfIntersect( SArray<MbCrossPoint> &, double metricEps = Math::LengthEps ) const;

  /** \brief \ru Найти особые точки эквидистантной кривой.
             \en Find the special points of an offset curve. \~
    \details \ru Особыми точками эквидистантной кривой будем считать точки, в которых радиус кривизны исходной кривой 
      равен величине смещения эквидистантной кривой.
             \en Special points of an offset curve are the points where the curvature radius of the initial curve 
      equals to the value of shift of an offset curve. \~
    \param[in, out] tCusps - \ru Массив параметров особых точек.
                             \en An array of parameters of special points. \~
    \param[in] dist - \ru Смещение эквидистантной кривой.
                      \en Shift of the offset curve. \~
  */
  virtual void        OffsetCuspPoint( SArray<double> & tCusps, double dist ) const; 

  /** \brief \ru Провести кривую через точку.
             \en Create a curve through a point. \~
    \details \ru Изменить кривую так, чтобы она проходила через заданную точку. Изменения не должны затрагивать 
      всю кривую. Если у кривой присутствуют какие-либо базовые объекты, связь с ними не должна потеряться при модификации.
      Если построить искомую кривую невозможно, исходная кривая не изменяется, возвращается false.
             \en Change a curve such that it passes through the given point. Changes should not affect 
      the whole curve. If the curve has any base objects, then the connection with them should not be modified.
      If the curve cannot be constructed, then the initial curve will not change, false is returned. \~ 
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
    \return \ru true, если модификация выполнена, иначе - false.
            \en True - if the modification is performed, otherwise - false. \~
  */
  virtual bool        GoThroughPoint( MbCartPoint & pnt ); 

  /// \ru Вычислить среднюю точку кривой. \en Calculate a middle point of a curve. 
  virtual bool        GetMiddlePoint( MbCartPoint & ) const;
  /// \ru Вычислить начальную точку кривой. \en Calculate a start point of a curve. 
  virtual void        GetStartPoint ( MbCartPoint & ) const;
  /// \ru Вычислить конечную точку кривой. \en Calculate an end point of a curve. 
  virtual void        GetEndPoint   ( MbCartPoint & ) const;
  /// \ru Вычислить центр кривой. \en Calculate a center of curve. 
  virtual bool        GetCentre     ( MbCartPoint & ) const;

  /** \brief   \ru Дать физический радиус кривой или ноль, если это невозможно. 
               \en Get the physical radius of the curve or zero if it impossible. \~
      \details \ru В общем случае на запрос радиуса возвращается 0. Число, отличное от 0, можно получить лишь в том случае,
                   если кривая является дугой окружности или эквивалентна дуге окружности с заданной точностью
                   (по-умолчанию PARAM_REGION).
               \en Generally returns 0. A non-zero value may be obtained only when the curve 
                   is an arc or is equal to an arc with the set precision (PARAM_REGION by default). \~ 
      \params[in] accuracy - \ru Максимальное отклонение кривой от дуги окружности (по-умолчанию PARAM_REGION).
                             \en The maximum curve deviation from an arc (PARAM_REGION by default). \~
      \return  \ru Значение радиуса, если есть, или 0.0.
               \en Radius value if it can be obtained or 0.0. \~
  */
  virtual double      GetRadius( double accuracy = PARAM_REGION ) const;

  /** \brief \ru Вычислить точку для построения оси.
             \en Calculate a point to construct an axis. \~
    \details \ru Вычисляет точку для построения оси, если кривая может быть построена вращением точки вокруг некоторой оси.
             \en Calculates a point to construct an axis, if a curve may be constructed by rotation of a point around an axis. \~
    \return \ru true, если такая ось существует.
            \en true, if such axis exists. \~
  */
  virtual bool        GetAxisPoint( MbCartPoint & p ) const; 

  /// \ru Определить, подобны ли кривые для объединения (слива). \en Define whether the curves are similar for the merge. 
  virtual bool        IsSimilarToCurve( const MbCurve & curve, double precision = PARAM_PRECISION ) const;
  /// \ru Определить количество разбиений для прохода в операциях. \en Define the number of splittings for one passage in operations. 
  virtual size_t      GetCount() const;

  /// \ru Выдать n точек кривой с равными интервалами по параметру. \en Get n points of a curve with equal intervals by parameter. 
  void        GetPointsByEvenParamDelta ( size_t n, std::vector<MbCartPoint> & pnts ) const;
  void        GetPointsByEvenParamDelta ( size_t n, SArray<MbCartPoint> & pnts ) const; // Deprecated.
  /// \ru Выдать n точек кривой с равными интервалами по длине дуги. \en Get n points of a curve with equal intervals by arc length. 
  virtual void        GetPointsByEvenLengthDelta( size_t n, std::vector<MbCartPoint> & pnts ) const;
  void        GetPointsByEvenLengthDelta( size_t n, SArray<MbCartPoint> & pnts ) const; // Deprecated.

  /** \brief \ru Вычислить минимальную длину кривой между двумя точками на ней.
             \en Calculate minimal length of a curve between two points on it. \~
    \details \ru Если кривая не замкнутая, то длина кривой между точками определяется однозначно.
      Если кривая замкнута, то из двух возможных путей выбирается самый короткий.
      Для замкнутой кривой можно определить желаемую часть с помощью задания контрольной точки pc. 
      Тогда выбирается та часть кривой, к которой ближе находится контрольная точка.
             \en If a curve is not closed, then the length between points is clearly defined.
      If a curve is closed, then there is chosen the shortest path from the two possible paths.
      For a closed curve the desired part may be defined by the control points pc. 
      In this case the such part of a curve is chosen, which is closer to a control point. \~
    \param[in] p1 - \ru Первая точка.
                    \en The first point. \~
    \param[in] p2 - \ru Вторая точка.
                    \en The second point \~
    \param[in] pc - \ru Контрольная точка.
                    \en A control point \~
    \return \ru Длина части кривой между точками.
            \en A length of a curve between points. \~
  */
  virtual double      LengthBetween2Points( MbCartPoint & p1, MbCartPoint & p2,
                                            MbCartPoint * pc = nullptr ) const;

  /// \ru Вычислить центр тяжести кривой. \en Calculate the center of gravity of a curve. 
  virtual bool        GetWeightCentre ( MbCartPoint & ) const;

  /// \ru Проверить лежит ли точка на кривой с точностью eps. \en Check whether the point is on a curve with the tolerance eps. 
  bool        IsPointOn( const MbCartPoint &, double eps = Math::LengthEps ) const;
  /// \ru Проверить лежит ли параметр в диапазоне кривой с точностью eps. \en Check whether the parameter is inside a range with the tolerance eps. 
  bool        IsParamOn( double t, double eps = Math::paramEpsilon ) const;

  /** \brief \ru Корректировать параметр для замкнутых кривых.
             \en Correct parameter for closed curves. \~
    \details \ru Если кривая замкнута, то функция загоняет параметр t в диапазон параметров кривой. 
      Кроме того, если параметр t отличается от одного из граничных параметров меньше, чем на eps,
      он делается равным граничному параметру.
             \en If the curve is closed, then the function sets the parameter t to the range of the curve. 
      Besides, if t differs from one of bounding parameters by a value which is less than eps,
      then it becomes equal to the bounding parameter. \~
    \param[in, out] t - \ru На входе - заданное значение параметра, на выходе - скорректированное.
                        \en Input - given value of parameter, output - corrected value of parameter. \~
    \param[in] eps - \ru Точность попадания на край диапазона.
                     \en A tolerance of getting to the bound of the range. \~
  */
  void        CorrectCyclicParameter( double & t, double eps = Math::paramRegion ) const;

  /** \brief \ru Корректировать параметр.
             \en Correct parameter. \~
    \details \ru Функция загоняет параметр t в диапазон параметров кривой.
             \en The function sets the parameter t to the range of the curve. \~ 
    \param[in, out] t - \ru На входе - заданное значение параметра, на выходе - скорректированное.
                        \en Input - given value of parameter, output - corrected value of parameter. \~
  */
  void        CorrectParameter      ( double & t ) const;

  /// \ru Сделать копию с измененным направлением. \en Create a copy with changed direction. 
  MbCurve   * InverseDuplicate() const;

  /// \ru Определить, являются ли кривая инверсно такой же. \en Define whether an inversed curve is the same. 
  virtual bool        IsInverseSame( const MbCurve &, double accuracy = LENGTH_EPSILON ) const;

  /** \brief \ru Определить, является ли кривая репараметризованно такой же.
             \en Define whether a reparameterized curve is the same. \~
    \details \ru Определить, является ли кривая репараметризованно такой же.
             \en Define whether a reparameterized curve is the same. \~
    \param[in]  curve  - \ru Кривая для сравнения.
                         \en A curve for comparison. \~
    \param[out] factor - \ru Коэффициент сжатия параметрической области при переходе к указанной кривой.
                         \en Coefficient of compression of parametric region at the time of transition to the pointed curve. \~
  */
  virtual bool        IsReparamSame( const MbCurve & curve, double & factor ) const;

  /** \brief \ru Вычислить граничную точку.
             \en Calculate the boundary point. \~
    \details \ru Вычислить граничную точку. \n
             \en Calculate the boundary point. \n \~
    \param[in] number - \ru Номер граничной точки. Значение 1 соответствует начальной точке кривой, 2 - конечной.
                        \en A number of a boundary point. The value 1 corresponds to the start point of a curve, 2 - to the end point. \~
    \return \ru Вычисленная точка.
            \en A calculated point. \~
  */
  MbCartPoint GetLimitPoint( ptrdiff_t number ) const; 

  /** \brief \ru Вычислить граничную точку.
             \en Calculate the boundary point. \~
    \details \ru Вычислить граничную точку. \n
             \en Calculate the boundary point. \n \~
    \param[in] number - \ru Номер граничной точки. Значение 1 соответствует начальной точке кривой, 2 - конечной.
                        \en A number of a boundary point. The value 1 corresponds to the start point of a curve, 2 - to the end point. \~
    \param[in, out] pnt - \ru Вычисленная точка.
                          \en A calculated point. \~
  */
  void        GetLimitPoint( ptrdiff_t number, MbCartPoint & pnt ) const;

  /** \brief \ru Вычислить касательный вектор в граничной точке.
             \en Calculate a tangent vector to the boundary point. \~
    \details \ru Вычислить нормализованный касательный вектор в граничной точке.
             \en Calculate a normalized tangent vector to the boundary point. \~
    \param[in] number - \ru Номер граничной точки. Значение 1 соответствует начальной точке кривой, 2 - конечной.
                        \en A number of a boundary point. The value 1 corresponds to the start point of a curve, 2 - to the end point. \~
    \param[in, out] v - \ru Касательный вектор.
                        \en Tangent vector \~
  */
  void        GetLimitTangent( ptrdiff_t number, MbVector & v ) const;

  /** \brief \ru Вычислить касательный вектор и точку на конце кривой.
             \en Calculate a tangent vector and point at the end of a curve. \~
    \details \ru Вычислить нормализованный касательный вектор и точку на конце кривой.
             \en Calculate a normalized tangent vector and point at the end of a curve. \~
    \param[in] number - \ru Номер граничной точки. Значение 1 соответствует начальной точке кривой, 2 - конечной.
                        \en A number of a boundary point. The value 1 corresponds to the start point of a curve, 2 - to the end point. \~
    \param[in, out] pnt - \ru Вычисленная точка.
                          \en A calculated point. \~
    \param[in, out] v - \ru Касательный вектор.
                        \en Tangent vector \~
  */
  void        GetLimitPointAndTangent( ptrdiff_t number, MbCartPoint & pnt, MbVector & v ) const;

  /** \brief \ru Равны ли граничные точки?
             \en Are boundary points equal? \~
    \details \ru Равны ли граничные точки кривой?
             \en Are curve boundary points equal? \~
    \return \ru true, если точки равны.
            \en Returns true if points are equal. \~
  */
  bool        AreLimitPointsEqual() const { return GetLimitPoint( 1 ) == GetLimitPoint( 2 ); } 

  /** \brief \ru Вернуть характерную точку кривой.
             \en Return a specific point of a curve. \~ 
    \details \ru Вернуть характерную точку кривой, если расстояние от нее до заданной точки from меньше, чем dmax. 
      Характерными точками ограниченной кривой являются начальная точка и конечная точка.
             \en Return a specific point of a curve if the distance from it to the given point is less than dmax. 
      Specific points of a bounded curve are its start and end points. \~
    \param[in] from - \ru Контрольная точка.
                      \en A control point \~
    \param[in, out] dmax - \ru На входе - максимальное расстояние для поиска характерной точки. На выходе - расстояние от точки from до найденной характерной точки.
                           \en Input - maximum distance for search of specific point. Output - a distance from the point 'from' to the found specific point. \~
    \param[in, out] pnt - \ru Касательный вектор.
                          \en Tangent vector. \~
    \result \ru true - если характерная точка найдена.
            \en True - if the specific point is found. \~
  */
  virtual bool        GetSpecificPoint( const MbCartPoint & from, double & dmax, MbCartPoint & pnt ) const; 

  /// \ru Вернуть базовую кривую, если есть, или себя. \en Returns the base curve if exists or itself. 
  virtual const MbCurve & GetBasisCurve() const;
  /// \ru Вернуть базовую кривую, если есть, или себя. \en Returns the base curve if exists or itself. 
  virtual       MbCurve & SetBasisCurve();
  /// \ru Вернуть отступ по параметру кривой. \en Return an indent by parameter of a curve. 
  virtual double      GetParamDelta() const;

  // \ru Геометрия подложки тождественна геометрии кривой, отлична параметризация. \en The geometry of the a substrate is identical to the geometry of a curve, a parameterization differs. 

  /// \ru Выдать подложку или себя. \en Get a substrate or itself. 
  virtual const MbCurve & GetSubstrate() const;
  /// \ru Выдать подложку или себя. \en Get a substrate or itself. 
  virtual       MbCurve & SetSubstrate();
  /// \ru Вернуть направление подложки относительно кривой или наоборот. \en Return direction of a substrate relative to a curve or vice versa. 
  virtual int         SubstrateCurveDirection() const;
  /// \ru Преобразовать параметр подложки в параметр кривой. \en Transform a substrate parameter to the curve parameter. 
  virtual void        SubstrateToCurve( double & ) const;
  /// \ru Преобразовать параметр кривой в параметр подложки. \en Transform a curve parameter to the substrate parameter. 
  virtual void        CurveToSubstrate( double & ) const;

  /** \brief \ru Вычислить метрическую длину кривой.
             \en Calculate the metric length of a curve. \~
    \details \ru Длина кривой вычисляется неточно, на основе аппроксимации ломаной. 
      Если нужна более точно вычисленная длина кривой, надо пользоваться функцией CalculateMetricLength().
             \en The length of a curve is inaccurately calculated, by approximation of polyline. 
      If the more accurate curve's length is required, then use the function CalculateMetricLength(). \~
  */
  virtual double      GetLengthEvaluation() const;

  /// \ru Вернуть приращение параметра, соответствующее единичной длине в пространстве. \en Return increment of parameter, corresponding to the unit length in space. 
  virtual double      GetParamToUnit() const; 
  /// \ru Вернуть приращение параметра, соответствующее единичной длине в пространстве в зависимости от параметра. \en Return increment of parameter, corresponding to the unit length in space according to parameter. 
  virtual double      GetParamToUnit( double t ) const; 
  /// \ru Вернуть минимально различимую величину параметра с заданной точностью. \en Return the minimal discernible value of parameter with the given tolerance. 
  virtual double      GetTEpsilon( double epsilon ) const;
  /// \ru Вернуть минимально различимую величину параметра с заданной точностью в зависимости от параметра. \en Return the minimal discernible value of parameter with the given tolerance according to parameter. 
  virtual double      GetTEpsilon( double t, double epsilon ) const; 
  /// \ru Вернуть минимально различимую величину параметра с заданной точностью. \en Return the minimal discernible value of parameter with the given tolerance. 
  virtual double      GetTRegion ( double epsilon ) const; 
  /// \ru Вернуть минимально различимую величину параметра с заданной точностью в зависимости от параметра. \en Return the minimal discernible value of parameter with the given tolerance according to parameter. 
  virtual double      GetTRegion ( double t, double epsilon ) const;
  /// \ru Вернуть минимально различимую величину параметра с заданной двумерной точностью в зависимости от параметра. Метод учитывает направление кривой в точке. \en Return the minimal discernible value of parameter with the given two-dimensinal accuracy according to parameter. The method takes into account the direction of the curve at a point.
  virtual double      GetTRegion( double t, const MbDimAccuracy & xyEps ) const;

  /// \ru Вернуть середину параметрического диапазона кривой. \en Return the middle of parametric range of a curve. 
  double      GetTMid() const { return ((GetTMin() + GetTMax()) * 0.5); }
  /// \ru Вернуть параметрическую длину кривой. \en Return the parametric length of a curve. 
  double      GetTRange() const { return (GetTMax() - GetTMin()); }
  /// \ru Вычислить точку на кривой. \en Calculate point on the curve. 
          MbCartPoint PointOn  ( double & t ) const;
  /// \ru Вычислить первую производную. \en Calculate first derivative. 
          MbVector    FirstDer ( double & t ) const;
  /// \ru Вычислить касательный вектор (нормализованный). \en Calculate tangent vector (normalized). 
          MbDirection Tangent  ( double & t ) const;
  /// \ru Вычислить нормальный вектор. \en Calculate the normal vector. 
          MbDirection Normal   ( double & t ) const;
  /// \ru Вычислить длину вектора производной. \en Calculate the length of derivative vector. 
  double      DerLength( double & t ) const;

  /** \brief \ru Получить границы участков кривой, которые описываются одной аналитической функцией.
             \en Get the boundaries of the curve sections that are described by one analytical function. \~
    \details \ru Получить границы участков кривой, которые описываются одной аналитической функцией. \n
                 Функция введена для оптимизации реализации функции MbCurve3D::GetCurvatureSpecialPoints, чтобы не насчитывать точки разрыва. \n
             \en Get the boundaries of the curve sections that are described by one analytical function. \n 
                 The function was introduced to optimize the implementation of the function MbCurve3D :: GetCurvatureSpecialPoints, so as not to calculate the break points.\n \~
    \param[out] params - \ru Параметры кривой, в которых меняется аналитическая функция..
                         \en Curve parameters in which the analytical function changes. \~
  */
  virtual void        GetAnalyticalFunctionsBounds( std::vector<double> & params ) const;
  /// \ ru Определение точек излома кривой. \en The determination of curve smoothness break points.
  virtual void        BreakPoints( std::vector<double> & vBreaks, double precision = ANGLE_REGION ) const;

  /// \ru Создать собственное свойство. \en Create a custom property. 
  MbProperty& CreateProperty( MbePrompt name ) const override;
  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  //virtual void        GetProperties( MbProperties & properties ) = 0;
  /// \ru Записать свойства объекта. \en Set properties of the object. 
  //virtual void        SetProperties( const MbProperties & properties ) = 0;
  /// \ru Выдать базовые точки кривой. \en Get the basis points of the curve. 
  //virtual void        GetBasisPoints( MbControlData & ) const = 0; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  //virtual void        SetBasisPoints( const MbControlData & ) = 0; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \brief \ru Непрерывна ли первая производная кривой по длине и направлению?
              \en Have the first derivative of the curve the continuous length and direction?
    \details \ru Отсутствуют ли разрывы первой производной кривой по длине и направлению? \n
              \en Are absent any discontinuities at length or at direction of first derivative of the curve? \n \~
    \param[out] contLength - \ru Непрерывность длины (да/нет).
                              \en The length is continuous (true/false). \~
    \param[out] contDirect - \ru Непрерывность направления (да/нет).
                              \en The direction of the first derivative is continuous (true/false). \~
    \param[out] params    - \ru Параметры точек, в которых происходит разрыв направления.
                              \en The parameters of the points at which the direction break occurs. \~
    \param[in]  epsilon    - \ru Погрешность вычисления.
                              \en The accuracy of the calculation. \~
  */
  virtual bool        IsContinuousDerivative( bool & contLength, bool & contDirect, c3d::DoubleVector * params = nullptr, double epsilon = EPSILON ) const;

  /** \brief \ru Устранить разрывы первых производных по длине.
              \en Eliminate the discontinuities of the first derivative at length.
    \details \ru Устранить разрывы производных по длине. \n
              \en Eliminate the discontinuities of the first derivatives of the length. \n \~
    \param[in]  epsilon    - \ru Погрешность вычисления.
                              \en The accuracy of the calculation. \~
    \param[in]  version    - \ru Версия математики.
                              \en Math version. \~
  */
  virtual bool        SetContinuousDerivativeLength( VERSION version, double epsilon = EPSILON );
                
  /** \brief \ru Определить, близки ли две кривые метрически.
             \en Check whether the two curves are metrically close. \~
    \details \ru Близость кривых определяется, исходя из равенства их конечных точек 
      и расстояния произвольной точки одной кривой от другой кривой.
      Параметрически кривые могут отличаться.
             \en The proximity of curves is defined by equality of their ends 
      and the distance of an arbitrary point of one curve to another curve.
      Curves may differ parametrically. \~
    \param[in] curve - \ru Кривая, с которой производится сравнение.
                       \en A curve to compare with. \~
    \param[in] eps - \ru Максимально допустимое расстояние между ближайшими точками двух кривых.
                     \en The maximum allowed distance between the nearest points of two curves. \~
    \param[in] ext - \ru Флаг определяет, будет ли при необходимости продолжена кривая curve. 
      Если ext = true, то кривая может быть продолжена.
                     \en A flag defines whether the curve 'curve' may be extended when necessary. 
      If ext = true then the curve may be extended. \~
    \param[in] devSag - \ru Максимальная величина прогиба.
                        \en Maximal value of sag. \~ 
  */
  bool        IsSpaceNear( const MbCurve & curve, double eps, bool ext, double devSag = 5.0*Math::deviateSag ) const; 

  /** \brief \ru Определить, близки ли две кривые метрически.
             \en Check whether the two curves are metrically close. \~
    \details \ru Близость кривых определяется, исходя из равенства их конечных точек и расстояния произвольной точки одной кривой от другой кривой.
      Параметрически кривые могут отличаться.
             \en The proximity of curves is defined by equality of their ends and the distance of an arbitrary point of one curve to another curve.
      Curves may differ parametrically. \~
    \param[in] curve - \ru Кривая, с которой производится сравнение.
                       \en A curve to compare with. \~
    \param[in] xEps - \ru Точность определения проекции по оси x.
                      \en A tolerance of detection of the projection by x axis. \~
    \param[in] yEps - \ru Точность определения проекции по оси y.
                      \en A tolerance of detection of the projection by y axis. \~
    \param[in] ext - \ru Флаг определяет, будет ли при необходимости продолжена кривая curve. 
      Если ext = true, то кривая может быть продолжена.
                     \en A flag defines whether the curve 'curve' may be extended when necessary. 
      If ext = true then the curve may be extended. \~
    \param[in] xNear - \ru Максимально допустимое расстояние между ближайшими точками двух кривых по X.
                       \en The maximum allowed distance along X between the nearest points of two curves. \~
    \param[in] yNear - \ru Максимально допустимое расстояние между ближайшими точками двух кривых по Y.
                       \en The maximum allowed distance along Y between the nearest points of two curves. \~
    \param[in] devSag - \ru Максимальная величина прогиба.
                        \en Maximal value of sag. \~ 
  */
  bool        IsSpaceNear( const MbCurve & curve, double xEps, double yEps, bool ext, 
                           double xNear, double yNear,
                           double devSag = 5.0*Math::deviateSag ) const; 

  SimpleName  GetCurveName() const { return name; } ///< \ru Имя кривой. \en A curve name. 
  void        SetCurveName( SimpleName newName ) { name = newName; } ///< \ru Установить имя кривой. \en Set a curve name. 
  /** \} */

          // \ru Функции унификации объекта и вектора объектов в шаблонных функциях. \en Functions for compatibility of a object and a vector of objects in template functions.
  size_t      size() const { return 1; } ///< \ru Количество объектов при трактовке объекта как вектора объектов. \en Number of objects if object is interpreted as vector of objects. 
  const   MbCurve *   operator [] ( size_t ) const { return this; }  ///< \ru Оператор доступа. \en An access operator.

  /** \brief \ru Продлить кривую.
             \en Extend the curve. \~
    \details \ru Продлить кривую в соответствии с заданными параметрами. \n
             \en Extend the curve according to the given parameters. \~ \n
    \param[in]  parameters - \ru Параметры продления.
                             \en Parameters of extension. \~
    \param[out] resCurve   - \ru Продленная кривая или нулевой указатель, если продление невозможно.
                             \en The extended curve or nullptr if extension is impossible. \~
    \return  \ru Возвращает статус ошибки или rt_Success, если все хорошо.
             \en Returns error status or rt_Success if all is OK.
  */
// ---
  virtual MbResultType Extend( const MbCurveExtensionParameters & parameters, c3d::PlaneCurveSPtr & resCurve ) const;

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию \en Declaration without implementation of the assignment operator to prevent an assignment by default 
  void        operator = ( const MbCurve & ); 

  DECLARE_PERSISTENT_CLASS( MbCurve )

}; // MbCurve

IMPL_PERSISTENT_OPS( MbCurve )


//------------------------------------------------------------------------------
// \ru Вычислить точку на кривой. \en Calculate point on the curve. 
// ---
inline MbCartPoint MbCurve::PointOn( double & t ) const
{
  MbCartPoint pOn;
  PointOn( t, pOn );
  return pOn;
}


//------------------------------------------------------------------------------
// \ru Вычислить первую производную. \en Calculate first derivative. 
// ---
inline MbVector MbCurve::FirstDer( double & t ) const
{
  MbVector der;
  FirstDer( t, der );
  return der;
}


//------------------------------------------------------------------------------
// \ru Вычислить длину вектора производной. \en Calculate the length of derivative vector. 
// ---
inline double MbCurve::DerLength( double & t ) const
{
  MbVector der;
  FirstDer( t, der );
  return der.Length();
}


//------------------------------------------------------------------------------
// \ru Вычислить касательный вектор (нормализованный). \en Calculate tangent vector (normalized). 
// ---
inline MbDirection MbCurve::Tangent( double & t ) const
{
  MbDirection tang;
  Tangent( t, tang );
  return tang;
}


//------------------------------------------------------------------------------
// \ru Вычислить касательный вектор (нормализованный). \en Calculate tangent vector (normalized). 
// ---
inline void MbCurve::Tangent( double & t, MbVector & v ) const
{
  FirstDer( t, v );
  v.Normalize();
}


//------------------------------------------------------------------------------
// \ru Вычислить касательный вектор (нормализованный). \en Calculate tangent vector (normalized). 
// ---
inline void MbCurve::Tangent( double & t, MbDirection & tang ) const
{
  MbVector v;
  FirstDer( t, v );
  tang = v;
}


//------------------------------------------------------------------------------
// \ru Вычислить нормальный вектор. \en Calculate the normal vector. 
// ---
inline MbDirection MbCurve::Normal( double & t ) const {
  return ~Tangent( t );
}


//------------------------------------------------------------------------------
// \ru Вычислить нормальный вектор. \en Calculate the normal vector. 
// ---
inline void MbCurve::Normal( double & t, MbVector & v ) const
{
  Tangent( t, v );
  v.Perpendicular();
}


//------------------------------------------------------------------------------
// \ru Вычислить нормальный вектор. \en Calculate the normal vector. 
// ---
inline void MbCurve::Normal( double & t, MbDirection & norm ) const {
  Tangent( t, norm );
  norm.Perpendicular();
}


//------------------------------------------------------------------------------
// \ru Вычислить касательный вектор (нормализованный). \en Calculate tangent vector (normalized). 
// ---
inline void MbCurve::_Tangent( double t, MbVector & v ) const
{
  _FirstDer( t, v );
  v.Normalize();
}


//------------------------------------------------------------------------------
// \ru Вычислить касательный вектор (нормализованный). \en Calculate tangent vector (normalized). 
// ---
inline void MbCurve::_Tangent( double t, MbDirection & tang ) const
{
  MbVector v;
  _FirstDer( t, v );
  tang = v;
}


//------------------------------------------------------------------------------
// \ru Вычислить вектор главной нормали (нормализованный) на кривой и её продолжении. \en Calculate main normal vector (normalized) at curve and its extension. 
// ---
inline void MbCurve::_Normal( double t, MbVector & v ) const
{
  _FirstDer( t, v );
  v.Perpendicular();
  v.Normalize();
}


//------------------------------------------------------------------------------
// \ru Вычислить вектор главной нормали (нормализованный) на кривой и её продолжении. \en Calculate main normal vector (normalized) at curve and its extension. 
// ---
inline void MbCurve::_Normal( double t, MbDirection & norm ) const
{
  MbVector v;
  _FirstDer( t, v );
  v.Perpendicular();
  norm = v;
}


//------------------------------------------------------------------------------
// \ru Проверить лежит ли точка на кривой с точностью eps. \en Check whether the point is on a curve with the tolerance eps. 
// ---
inline bool MbCurve::IsPointOn( const MbCartPoint & pOn, double eps ) const {
  return DistanceToPoint( pOn ) < eps;
}


//------------------------------------------------------------------------------
// \ru Проверить лежит ли параметр в диапазоне кривой с точностью eps. \en Check whether the parameter is inside a range with the tolerance eps. 
// ---
inline bool MbCurve::IsParamOn( double t, double eps ) const {
  return ( GetTMin() - t < eps ) && ( t - GetTMax() < eps );
}


//------------------------------------------------------------------------------
// \ru Вычислить проекцию точки на кривую. \en Calculate the point projection to the curve. 
// ---
inline void MbCurve::PointProjection( const MbCartPoint & pnt, MbCartPoint & on ) const
{
  double t = PointProjection( pnt );
  PointOn( t, on );
}


//------------------------------------------------------------------------------
// \ru Вычислить точку на кривой, соответствующую проекции заданной точки на эту кривую. \en Calculate the point on the curve corresponding to the projection of the given point on this curve. 
// \ru Дополнительно возвращает угол наклона касательной к оси 0X в точке проекции. \en Additionally returns an inclination angle of a tangent to the axis OX at the point of projection. 
// ---
inline void MbCurve::PointProjectionAndAngle( MbCartPoint & on, double & angle ) const
{
  double t = PointProjection( on );
  PointOn( t, on );
  MbVector fd;
  FirstDer( t, fd );
  angle = fd.DirectionAngle(); // \ru Угол касательной к оси 0X \en An angle of tangent to the axis OX. 
}


//------------------------------------------------------------------------------
// \ru Вычислить граничную точку. \en Calculate the boundary point. 
// ---
inline MbCartPoint MbCurve::GetLimitPoint( ptrdiff_t number ) const {
  double t = ( number == 1 ) ? GetTMin() : GetTMax();

  MbCartPoint pOn;
  PointOn( t, pOn );
  return pOn;
}


//------------------------------------------------------------------------------
// \ru Вычислить граничную точку. \en Calculate the boundary point. 
// ---
inline void MbCurve::GetLimitPoint( ptrdiff_t number, MbCartPoint & pnt ) const {
  double t = ( number == 1 ) ? GetTMin() : GetTMax();
  PointOn( t, pnt );
}


//------------------------------------------------------------------------------
// \ru Вычислить касательный вектор в граничной точке. \en Calculate a tangent vector to the boundary point. 
// ---
inline void MbCurve::GetLimitTangent( ptrdiff_t number, MbVector & tang ) const {
  double t = ( number == 1 ) ? GetTMin() : GetTMax();
  Tangent( t, tang );
}


//------------------------------------------------------------------------------
// \ru Вычислить касательный вектор и точку на конце кривой. \en Calculate a tangent vector and point at the end of a curve. 
// ---
inline void MbCurve::GetLimitPointAndTangent( ptrdiff_t number, MbCartPoint & pnt, MbVector & tang ) const {
  double t = ( number == 1 ) ? GetTMin() : GetTMax();
  PointOn( t, pnt );
  Tangent( t, tang );
}


//------------------------------------------------------------------------------
// \ru Определить, замкнута ли кривая фактически независимо от гладкости замыкания. \en Determine whether a curve is closed regardless of the smoothness of the closure.
// ---
inline bool MbCurve::IsTouch( double eps ) const
{
  MbCartPoint p1, p2;
  _PointOn( GetTMin(), p1 );
  _PointOn( GetTMax(), p2 );
  if ( c3d::EqualPoints( p1, p2, eps ) ) {
    MbCartPoint p;
    _PointOn( GetTMid(), p );
    double d = p1.DistanceToPoint( p2 );
    double d1 = p.DistanceToPoint( p1 );
    double d2 = p.DistanceToPoint( p2 );
    if ( d <= d1 + d2 - EXTENT_EQUAL )
      return true;
  }
  return false;
}


//------------------------------------------------------------------------------
/** \brief \ru Вычислить параметры ближайших точек двух кривых.
           \en Calculate parameters of the nearest points of two curves. \~
  \details \ru Вычислить параметры ближайших точек двух кривых и расстояние между этими точками.
           \en Calculate parameters of the nearest points of two curves and the distance between these points. \~
  \param[in] curve1 - \ru Кривая 1.
                      \en Curve 1. \~
  \param[in] ext1 - \ru Признак поиска на продолжении кривой 1.
                    \en An attribute of search on the extension of the curve 1. \~
  \param[in] curve2 - \ru Кривая 2.
                      \en Curve 2. \~
  \param[in] ext2 - \ru Признак поиска на продолжении кривой 2.
                    \en An attribute of search on the extension of the curve 2. \~
  \param[in] xEpsilon - \ru Погрешность по X.
                        \en Tolerance in X direction. \~
  \param[in] yEpsilon - \ru Погрешность по Y.
                        \en Tolerance in Y direction. \~
  \param[out] t1 - \ru Параметр точки кривой 1.
                   \en A point parameter of curve 1. \~
  \param[out] t2 - \ru Параметр точки кривой 2.
                   \en A point parameter of curve 2. \~
  \param[out] dmin - \ru Расстояние между точками кривых.
                     \en The distance between points of curves. \~
  \param[in] version - \ru Версия.
                       \en Version. \~
  \return \ru Возвращает nr_Success (+1) или nr_Special(0) в случае успешного определения, в случае неудачи возвращает nr_Failure(-1).
          \en Return nr_Success (+1) or nr_Special(0) in a case of successful defining, return nr_Failure(-1) in a case of failure. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MbeNewtonResult) NearestPoints( const MbCurve & curve1, bool ext1, 
                                           const MbCurve & curve2, bool ext2, 
                                           double xEpsilon, double yEpsilon,
                                           double & t1, double & t2, double & dmin,
                                           VERSION version = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Площадь и ориентация замкнутой кривой.
           \en An area and orientation of a closed curve. \~
  \details \ru Вычислить площадь замкнутой кривой. 
    По знаку площади определяется ориентация замкнутой кривой.
    Если площадь положительна, то замкнутая кривая направлена против движения часовой стрелки. \n
    Если кривая не замкнута, но установлен флаг замкнутости, 
    то площадь будет вычислена для кривой, замкнутой отрезком, соединяющим края.
           \en Calculate an area of a closed curve. 
    Orientation of a closed curve is defined by the sign of an area.
    If the area is positive then the closed curve is directed counterclockwise. \n
    If the curve is not closed, but the flag of closedness is set, 
    then an area will be calculated for the curve which is closed by a section connecting bounds. \~
  \param[in] curve - \ru Исходная кривая.
                     \en The initial curve. \~
  \param[in] sag - \ru Угловое отклонение касательной кривой в соседних точках интегрирования, используется для расчета шага по кривой.
                   \en An angular deviation of a tangent curve at the neighbor integration points, is it used for the calculation of the step by a curve. \~
  \param[in] close - \ru Флаг замкнутости.
                     \en A flag of closedness. \~
  \return \ru Площадь замкнутой кривой со знаком (ориентацией).
          \en An area of closed curve with a sign (orientation). \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (double) AreaSign( const MbCurve & curve, double sag, bool close );


#endif // __CURVE_H
