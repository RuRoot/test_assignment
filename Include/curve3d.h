////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Кривая в трехмерном пространстве.
         \en Curve in three-dimensional space.\~
  \details \ru Кривые являются представителями семейства трёхмерных геометрических объектов.
    Кривые используются для построения поверхностей, а также вспомогательных элементов геометрической модели. 
    В геометрическом моделировании используются кривые, которыми легко управлять. 
    Управление осуществляется через данные, по которым построены кривые. 
    Кривые строятся с помощью аналитических функций, по набору точек, на базе кривых и на базе поверхностей.
           \en Curves are members of a family of three-dimensional geometric objects.
    Curves are used for construction of surfaces and auxiliary elements of geometric model. 
    In geometric modeling there are used curves which are easy to control. 
    Control is performed by the data which curves constructed by. 
    Curves are constructed using analytical functions by a set of points on the basis of curves and on the basis of surfaces. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CURVE3D_H
#define __CURVE3D_H


#include <space_item.h>
#include <mb_data.h>
#include <templ_sptr.h>
#include <templ_visitor.h>
#include <tool_mutex.h>
#include <hash32.h>


class  MbCurve;
class  MbRect1D;
class  MbSurface;
class  MbPolygon3D;
class  MbNurbs3D;
class  MbPlacement3D;
class  MbCurveExtensionParameters3D;
class  MbDimAccuracy;
class  MbCurveIntoNurbsInfo;
struct MbNurbsParameters;


class   MbCurve3D;
namespace c3d // namespace C3D
{
typedef SPtr<MbCurve3D>                             SpaceCurveSPtr;
typedef SPtr<const MbCurve3D>                       ConstSpaceCurveSPtr;

typedef std::vector<MbCurve3D *>                    SpaceCurvesVector;
typedef std::vector<const MbCurve3D *>              ConstSpaceCurvesVector;

typedef std::vector<SpaceCurveSPtr>                 SpaceCurvesSPtrVector;
typedef std::vector<ConstSpaceCurveSPtr>            ConstSpaceCurvesSPtrVector;

typedef std::set<MbCurve3D *>                       SpaceCurvesSet;
typedef SpaceCurvesSet::iterator                    SpaceCurvesSetIt;
typedef SpaceCurvesSet::const_iterator              SpaceCurvesSetConstIt;
typedef std::pair<SpaceCurvesSetConstIt, bool>      SpaceCurvesSetRet;

typedef std::set<const MbCurve3D *>                 ConstSpaceCurvesSet;
typedef ConstSpaceCurvesSet::iterator               ConstSpaceCurvesSetIt;
typedef ConstSpaceCurvesSet::const_iterator         ConstSpaceCurvesSetConstIt;
typedef std::pair<ConstSpaceCurvesSetConstIt, bool> ConstSpaceCurvesSetRet;
}


//------------------------------------------------------------------------------
/** \brief \ru Кривая в трехмерном пространстве.
           \en Curve in three-dimensional space. \~ 
  \details \ru Кривая в пространстве представляет собой векторную функцию скалярного параметра,
    принимающего значения на конечной одномерной области. 
    Координаты точки кривой являются однозначными непрерывными функциями параметра кривой.
    Кривая представляет собой непрерывное отображение некоторого участка числовой оси в трёхмерное пространство.\n
    Кривые используются для построения поверхностей. \n
    Нормаль к кривой вычисляется с учетом кривизны (второй производной). \n
    Для прямолинейных кривых это вектор нулевой за исключением плоских прямолинейных кривых,
    где он перпендикулярен нормали плоскости этой кривой.
           \en A curve in space is a vector function of a scalar parameter,
    which is set on a finite one-dimensional space. 
    Coordinates of the point are single-valued continuous functions of curve parameter.
    A curve is continuous mapping from a piece of numeric axis to the three-dimensional space.\n
    Curves are used to construct surfaces. \n
    Normal to a curve is calculated taking into account curvature (second derivative). \n
    For rectilinear curves it's a zero vector except for flat rectilinear curves, 
    where it's perpendicular to a normal of curve plane. \~ 
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbCurve3D : public MbSpaceItem, public MbNestSyncItem {
protected:
  SimpleName name; ///< \ru Имя кривой. Данные объекта являются временными и используются для внутренних целей. \en A curve name. The object data is temporary and used internally. 

protected :
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbCurve3D();
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbCurve3D( const MbCurve3D & other ) : MbSpaceItem(), name( other.name ) {}
public :
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbCurve3D();

public :
  /// \ru Реализация функции, инициирующей посещение объекта. \en Implementation of a function initializing a visit of an object. 
  virtual void Accept( Visitor & visitor )
  {
    VisitorImpl<MbCurve3D> * impl = dynamic_cast<VisitorImpl<MbCurve3D> *>(&visitor);
    if( impl )
      impl->Visit( *this );
  }

  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */
  //virtual MbeSpaceType IsA()  const = 0; // \ru Тип элемента. \en A type of element.
  MbeSpaceType Type() const override;     // \ru Групповой тип элемента. \en Group element type. 
  MbeSpaceType Family() const override;   // \ru Семейство объекта. \en Family of object.
  /// \ru Сделать копию с измененным направлением. \en Create a copy with changed direction. 
  virtual MbCurve3D   & InverseDuplicate() const;
  double      DistanceToPoint( const MbCartPoint3D & ) const override; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point.
  void        AddYourGabaritTo( MbCube & ) const override; // \ru Добавить габарит кривой в куб. \en Add a bounding box of a curve to a cube.
  /// \ru Перевести все временные (mutable) данные объекта в неопределённое (исходное) состояние. \en Translate all the time (mutable) data objects in an inconsistent (initial) state.
  void        Refresh() override; 
#ifdef C3D_SIGNAL_ENABLED
  /// \ru Перевести временные данные объекта в неопределённое состояние, учитывая тип произошедших изменений. \en Translate temporal data of the object in an inconsistent state, taking into account type of changes.
  virtual void        Refresh( MbeObjectChangeType );
#endif

  /** \brief \ru Рассчитать временные (mutable) данные объекта.
             \en Calculate temporary (mutable) data of an object. \~
    \details \ru Рассчитать временные данные объекта в зависимости от параметра forced. 
      Если параметр forced равен false, рассчитываются только ещё не насчитанные данные.
      Если параметр forced равен true, пересчитываются все временные данные объекта.
             \en Calculate the temporary data of an object depending of the "forced" parameter. 
      Calculate only data that was not calculated earlier if parameter "forced" is equal false.
      Recalculate all temporary data of an object if parameter "forced" is equal true.
    \param[in] forced - \ru Принудительный перерасчёт.
                        \en Forced recalculation. \~
  */
  virtual void        PrepareIntegralData( const bool forced ) const;

  /// \ru Являются ли объекты идентичными в пространстве. \en Are the objects identical in space? 
  virtual bool        IsSpaceSame( const MbSpaceItem & item, double eps = METRIC_REGION ) const;
  /** \} */
  /** \ru \name Функции описания области определения кривой
      \en \name Functions for curve domain description
      \{ */
  /// \ru Вернуть максимальное значение параметра. \en Get the maximum value of parameter. 
  virtual double      GetTMax()    const = 0;
  /// \ru Вернуть минимальное значение параметра. \en Get the minimum value of parameter. 
  virtual double      GetTMin()    const = 0;

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
      - MbLine3D,\n
      - MbLineSegment3D,\n
      - MbSpiral и производные классы,\n
      - MbBridgeCurve3D,\n
      - MbContour3D (если он содержит только один сегмент одного из указанных типов)\n
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
      - MbLine3D,\n
      - MbLineSegment3D,\n
      - MbSpiral and inherited classes,\n
      - MbBridgeCurve3D,\n
      - MbContour3D (if it contains only a single segment of one of the listed types)\n
      are not considered periodic when the start and end points coincide.
      The IsClosed() method always returns false for them.\~
    \return \ru true, если кривая замкнута.
            \en True if a curve is closed. \~
    \ingroup Curves_3D
  */
  virtual bool        IsClosed()   const = 0;

  /// \ru Вернуть период. Если кривая непериодическая, то 0. \en Return period. If a curve is not periodic then 0. 
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
      - MbLine3D,\n
      - MbLineSegment3D,\n
      - MbSpiral и производные классы,\n
      - MbBridgeCurve3D,\n
      - MbContour3D (если он содержит только один сегмент одного из указанных типов)\n
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
      - MbLine3D,\n
      - MbLineSegment3D,\n
      - MbSpiral and inherited classes,\n
      - MbBridgeCurve3D,\n
      - MbContour3D (if it contains only a single segment of one of the listed types)\n
      are not considered periodic when the start and end points coincide.
      The IsPeriodic() method always returns false for them.\~
    \return \ru true, если кривая замкнута.
            \en True if a curve is closed. \~
    \ingroup Curves_3D
  */
  virtual bool        IsPeriodic() const;

  /** \brief \ru Определить, замкнута ли кривая фактически независимо от гладкости замыкания.
             \en Determine whether a curve is closed regardless of the smoothness of the closure. \~
    \details \ru Определить, замкнута ли кривая фактически независимо от гладкости замыкания.
             \en Determine whether a curve is actually closed regardless of the smoothness of the closure. \~
  */
  bool        IsTouch( double eps = Math::metricPrecision )  const;

  /** \} */

  /** \ru \name Функции для работы в области определения кривой\n
            функции PointOn, FirstDer, SecondDer, ThirdDer,... корректируют параметр
            при выходе его за пределы области определения параметра.
            Исключение составляет MbLine3D (прямая).
      \en \name Functions for working in the curve's domain.\n
            functions PointOn, FirstDer, SecondDer, ThirdDer,... correct parameter
            when it is out of domain bounds.
            The exception is MbLine3D (line).
      \{ */
  
  /** \brief \ru Вычислить точку на кривой.
             \en Calculate a point on the curve. \~
    \details \ru Скорректировать параметры при выходе их за пределы области определения и вычислить точку на кривой.
             \en Correct parameter when getting out of domain bounds and calculate a point on the curve. \~
    \param[in] t - \ru Параметр curve.
                   \en Curve parameter. \~ 
    \param[out] p - \ru Вычисленная точка на кривой.
                    \en A point on the curve. \~
    \ingroup Curves_3D
  */
  virtual void        PointOn  ( double & t, MbCartPoint3D & p ) const = 0;
  /// \ru Вычислить первую производную. \en Calculate first derivative. 
  virtual void        FirstDer ( double & t, MbVector3D & ) const = 0;
  /// \ru Вычислить вторую производную. \en Calculate second derivative. 
  virtual void        SecondDer( double & t, MbVector3D & ) const = 0;
  /// \ru Вычислить третью производную. \en Calculate third derivative. 
  virtual void        ThirdDer ( double & t, MbVector3D & ) const = 0;
  /// \ru Вычислить касательный вектор (нормализованный). \en Calculate tangent vector (normalized). 
  virtual void        Tangent  ( double & t, MbVector3D & ) const;
  /// \ru Вычислить вектор главной нормали. \en Calculate main normal vector. 
  virtual void        Normal   ( double & t, MbVector3D & ) const;
  /// \ru Вычислить вектор бинормали. \en Calculate binormal vector. 
  virtual void        BNormal  ( double & t, MbVector3D & ) const;
  /** \} */

  /** \ru \name Функции для работы внутри и вне области определения кривой\n
            функции _PointOn, _FirstDer, _SecondDer, _ThirdDer,... не корректируют параметр
            при выходе за пределы области определения.
      \en \name Functions for working inside and outside the curve's domain\n
            functions _PointOn, _FirstDer, _SecondDer, _ThirdDer,... do not correct parameter
            when it is out of domain bounds.
      \{ */
  
  /** \brief \ru Вычислить точку на кривой и её продолжении. 
             \en Calculate point at curve and its extension. \~
    \details \ru Вычислить точку на кривой в том числе и за пределами области определения параметра.
             \en Calculate a point on the curve including the outside area determination parameter. \~
    \param[in] t - \ru Параметр curve.
                   \en Curve parameter. \~ 
    \param[out] p - \ru Вычисленная точка на кривой.
                    \en A point on the curve. \~
    \ingroup Curves_3D
  */
  virtual void       _PointOn  ( double  t, MbCartPoint3D & p ) const;
  /// \ru Вычислить первую производную на кривой и её продолжении. \en Calculate first derivative at curve and its extension. 
  virtual void       _FirstDer ( double  t, MbVector3D & ) const;
  /// \ru Вычислить вторую производную на кривой и её продолжении. \en Calculate second derivative at curve and its extension. 
  virtual void       _SecondDer( double  t, MbVector3D & ) const;
  /// \ru Вычислить третью производную на кривой и её продолжении. \en Calculate third derivative at curve and its extension. 
  virtual void       _ThirdDer ( double  t, MbVector3D & ) const;
  /// \ru Вычислить касательный вектор (нормализованный) на кривой и её продолжении. \en Calculate tangent vector (normalized) at curve and its extension. 
  virtual void       _Tangent  ( double  t, MbVector3D & ) const;
  /// \ru Вычислить вектор главной нормали (нормализованный) на кривой и её продолжении. \en Calculate main normal vector (normalized) at curve and its extension. 
  virtual void       _Normal   ( double  t, MbVector3D & ) const;
  /// \ru Вычислить вектор бинормали (нормализованный) на кривой и её продолжении. \en Calculate binormal vector (normalized) at curve and its extension. 
  virtual void       _BNormal  ( double  t, MbVector3D & ) const;

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
                               MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const;

  /** \brief \ru Вычислить точку и производные на кривой.
             \en Calculate point and derivatives on the curve. \~
    \details \ru Функция перегружена у MbSurfaceIntersectionCurve и MbSilhouetteCurve для приближённого быстрого вычисления точки и производных. 
      В остальных поверхностях эквивалентна функции Explore(t,false,pnt,fir,sec,nullptr).
             \en The function is overloaded in MbSurfaceIntersectionCurve and MbSilhouetteCurve for the fast approximated calculation of a point and derivatives. 
      In other surfaces it is equivalent to the function Explore(t,false,pnt,fir,sec,nullptr). \~
    \param[in] t - \ru Параметр.
                   \en Parameter. \~
    \param[out] pnt - \ru Вычисленная точка.
                      \en A calculated point. \~
    \param[out] fir - \ru Производная.
                      \en Derivative with respect to t. \~
    \param[out] sec - \ru Вторая производная по t, если не ноль.
                      \en Second derivative with respect to t, if not nullptr. \~
    \ingroup Curves_3D
  */
  virtual void        FastApproxExplore( double & t, MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec ) const;
  /** \} */

  /** \ru \name Функции движения по кривой
      \en \name Function of moving by curve
    \{ */

  /** \brief \ru Вычислить шаг параметра.
             \en Calculate parameter step. \~
    \details \ru Вычислить шаг параметра для аппроксимации кривой по величине прогиба кривой. 
      Вычисление шага проходит с учетом радиуса кривизны.
      Шаг аппроксимации кривой выбирается таким образом, 
      чтобы отклонение кривой от ее полигона не превышало заданную величину прогиба.
             \en Calculate parameter step for the curve's approximation by its sag value. 
      Calculation of the step is performed with consideration of curvature radius.
      A step of curve's approximation is chosen in such way, 
      that the deviation from its polygon does not exceed the given value of sag. \~
    \param[in] t   - \ru Параметр, определяющий точку на кривой, в которой надо вычислить шаг.
                     \en A parameter defining the point on a curve, at which a step should be calculated. \~
    \param[in] sag - \ru Максимально допустимая величина прогиба.
                     \en Maximum feasible sag value. \~
    \return \ru Величина шага по параметру в заданной точке.
            \en A sag value by parameter at given point. \~
    \ingroup Curves_3D
  */
  virtual double      Step( double t, double sag ) const;

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
    \ingroup Curves_3D
  */
  virtual double      DeviationStep( double t, double ang ) const;

  /** \brief \ru Вычислить шаг параметра.
             \en Calculate parameter step. \~
    \details \ru Вычислить шаг параметра для аппроксимации кривой по заданной метрической длине шага вдоль кривой.
             \en Calculate the parameter step for approximation of a curve by the given metric length of a step along a curve. \~ 
    \param[in] t   - \ru Параметр, определяющий точку на кривой, в которой надо вычислить шаг.
                     \en A parameter defining the point on a curve, at which a step should be calculated. \~
    \param[in] len - \ru Максимальная метрическая длина шага вдоль кривой.
                     \en Maximum metric length of a step along a curve. \~
    \return \ru Величина шага по параметру в заданной точке.
            \en A sag value by parameter at given point. \~
    \ingroup Curves_3D
  */
  virtual double      MetricStep   ( double t, double length ) const;

  /** \brief \ru Вычислить шаг параметра.
             \en Calculate parameter step. \~
    \details \ru Вычислить шаг параметра для аппроксимации кривой или по угловому отклонению касательной, или по величине прогиба, или по метрической длине. 
             \en Calculate parameter step for the curve approximation: by diviation sngle; or by its sag value; or by the metric length. \~
    \param[in] t   - \ru Параметр, определяющий точку на кривой, в которой надо вычислить шаг.
                     \en A parameter defining the point on a curve, at which a step should be calculated. \~
    \param[in] stepData - \ru Данные для вычисления шага.
                          \en Data for step calculation. \~
    \return \ru Величина шага по параметру в заданной точке.
            \en A sag value by parameter at given point. \~
    \ingroup Curves_3D
  */
  double      CurveStep( const double & t, const MbStepData & stepData ) const;

  /** \} */
  /** \ru \name Общие функции кривой
      \en \name Common function of curve.
      \{ */
  /// \ru Сбросить текущее значение параметра. \en Reset the current value of parameter. 
  virtual void        ResetTCalc() const;
  /// \ru Изменить направление кривой. \en Change direction of a curve. 
  virtual void        Inverse( MbRegTransform * iReg = nullptr ) = 0;
  /// \ru Вернуть базовую кривую, если есть, или себя \en Returns the base curve if exists or itself 
  virtual const MbCurve3D & GetBasisCurve() const;
  /// \ru Вернуть базовую кривую, если есть, или себя \en Returns the base curve if exists or itself 
  virtual       MbCurve3D & SetBasisCurve();
  /// \ru Вычислить кривизну кривой. \en Calculate curvature of curve. 
  virtual double      Curvature( double t ) const;
  /// \ru Вычислить вторую производную касательной. \en Calculate second derivative of tangent. 
  virtual void        ThirdMetricDer( double t, MbVector3D & vect ) const;
  
  // \ru Построить NURBS копию кривой. \en Construct a NURBS copy of a curve. 

  /** \brief \ru Построить NURBS копию кривой.
             \en Construct a NURBS copy of a curve. \~
    \details \ru Строит NURBS кривую, аппроксимирующую заданную. По возможности, строит точную кривую, возможно с кратными узлами.
      Количество узлов для NURBS определяется в зависимости от кривой.
             \en Constructs a NURBS copy which approximates a given curve. If it is possible, constructs the accurate curve, perhaps with multiple knots.
      The number of knots for NURBS is defined depending on the curve. \~
    \param[in] nInfo - \ru Параметры преобразования кривой в NURBS.
                       \en Parameters of conversion of a curve to NURBS. \~
    \result \ru Построенная NURBS кривая или nullptr при  неуспешном построении.
            \en The constructed NURBS curve or nullptr in a case of failure. \~
  */
          MbNurbs3D * NurbsCurve( const MbCurveIntoNurbsInfo * nInfo = nullptr ) const;

  /** \brief \ru Построить NURBS копию кривой.
             \en Construct a NURBS copy of a curve. \~
    \details \ru Строит NURBS кривую, аппроксимирующую заданную в диапазоне параметров [t1, t2] с заданным направлением. 
      По возможности, строит точную кривую, возможно с кратными узлами.
      Количеством узлов для NURBS определяется в зависимости от кривой.
             \en Constructs a NURBS curve which approximates a given curve inside the range [t1, t2]. with a given direction. 
      If it is possible, constructs the accurate curve, perhaps with multiple knots.
      The number of knots for NURBS is defined depending on the curve. \~
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
    \result \ru Построенная NURBS кривая или nullptr при  неуспешном построении.
            \en The constructed NURBS curve or nullptr in a case of failure. \~
  */
  virtual MbNurbs3D * NurbsCurve( const MbCurveIntoNurbsInfo & nInfo ) const;

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
    \result \ru Построенная NURBS кривая или nullptr при  неуспешном построении.
            \en The constructed NURBS curve or nullptr in a case of failure. \~
    \ingroup Curves_3D
  */
  virtual MbCurve3D * NurbsCurve( const MbNurbsParameters & tParameters ) const;

  /** \brief \ru Определить число узлов NURBS кривой, нужное для аппроксимации кривой с заданной точностью.
             \en Define the number of knots of a NURBS curve which is required to approximate the curve with the given tolerance. \~
    \details \ru Определить число узлов NURBS кривой, нужное для аппроксимации кривой с заданной точностью. \n
             \en Define the number of knots of a NURBS curve which is required to approximate the curve with the given tolerance. \n \~
    \param[in] tParameters - \ru Параметры построения NURBS копии кривой.
                             \en Parameters for the construction of a NURBS copy of the curve. \~
    \param[in] epsilon - \ru Точность аппроксимации.
                         \en The tolerance of approximation. \~
    \result \ru Построенная NURBS кривая или nullptr при  неуспешном построении.
            \en The constructed NURBS curve or nullptr in a case of failure. \~
  */
  virtual size_t      NurbsCurveMinPoints( const MbNurbsParameters & tParameters, double epsilon = c3d::METRIC_DELTA ) const; 

  /** \brief \ru Построить усеченную кривую.
             \en Construct a trimmed curve. \~
    \details \ru Строит усеченную кривую, начало которой соответствует точке с параметром t1 и
      конец - точке с параметром t2. 
      Можно изменить направление полученной кривой относительно исходной с помощью параметра sense.
      Если кривая замкнута, можно получить усеченную кривую, проходящую через
      начало кривой.\n
      В случае замкнутой или периодической кривой три параметра sense, t1 и t2 однозначно
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
      In a case of closed or periodic curve three parameters 'sense', t1 and t2 clearly
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
    \result \ru Построенная усеченная кривая.
            \en A constructed trimmed curve. \~
    \ingroup Curves_3D
  */
  virtual MbCurve3D * Trimmed( double t1, double t2, int sense, bool saveParamLenAndLaw = false  ) const; 

  /** \brief \ru Построить усеченную кривую c учетом заданной трехмерной точности.
             \en Construct a trimmed curve with the given two-dimensional accuracy. \~
    \details \ru Строит усеченную кривую c учетом заданной трехмерной точности, 
      начало которой соответствует точке с параметром t1 и конец - точке с параметром t2. 
      Можно изменить направление полученной кривой относительно исходной с помощью параметра sense.
      Если кривая замкнута, можно получить усеченную кривую, проходящую через
      начало кривой.\n
      В случае замкнутой или периодической кривой три параметра sense, t1 и t2 однозначно
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
      In a case of closed or periodic curve three parameters 'sense', t1 and t2 clearly
      define the result. 
      In a case of unclosed curve the parameter 'sense' and parameter of trimming should correspond each other:\n
      1) if sense ==  1, then t1 < t2,\n 
      2) if sense ==  -1, then t1 > t2,\n
      If there is a discrepancy between 'sense' and parameters of trimming, then 
      'sense' parameter has higher priority.
      If parameters t1 and t2 are equal and the curve is closed, then in result a closed curve should be obtained. \~
    \param[in] t1    -  \ru Параметр, соответствующий началу усеченной кривой.
                        \en Parameter corresponding to start of a trimmed curve. \~
    \param[in] t2    -  \ru Параметр, соответствующий концу усеченной кривой.
                        \en Parameter corresponding to end of a trimmed curve. \~
    \param[in] sense -  \ru Направление усеченной кривой относительно исходной.\n
                        sense =  1 - направление кривой сохраняется.
                        sense = -1 - направление кривой меняется на обратное.
                        \en Direction of a trimmed curve in relation to an initial curve.
                        sense =  1 - direction does not change.
                        sense = -1 - direction changes to the opposite value. \~
    \param[in] xyzEps - \ru Трехмерная точность.
                        \en Three-dimensional accuracy. \~
    \param[in] saveParLenAndLaw - \ru Сохранять параметрическую длину и закон.
                                  \en Save parametric length and law. \~
    \result \ru Построенная усеченная кривая.
            \en A constructed trimmed curve. \~
    \ingroup Curves_3D
    \warning \ru В разработке. \en Under development.
  */
  virtual MbCurve3D * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyzEps, bool saveParamLenAndLaw ) const; 

  /// \ru Вернуть параметрическую длину кривой. \en Return the parametric length of a curve. 
  double      GetParamLength () const { return GetTMax() - GetTMin(); }

  // \ru Функции с расчетом метрической длины перегружать все сразу, чтобы не было рассогласования \en Functions with calculation of metric length, they should be overloaded simultaneously to avoid  mismatches 
  /// \ru Вычислить метрическую длину кривой. \en Calculate the metric length of a curve. 
  virtual double      GetMetricLength() const;
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
    \ingroup Curves_3D
  */
  virtual double      CalculateLength( double t1, double t2 ) const;

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
    \return \ru true - если операция выполнена успешно, иначе false.
            \en True - if the operation is performed successfully, otherwise false. \~ 
    \ingroup Curves_3D
  */
  virtual bool        DistanceAlong( double & t, double len, int curveDir, double eps = Math::metricPrecision,
                                     VERSION version = Math::DefaultMathVersion() ) const; 

  /** \brief \ru Вычислить метрическую длину кривой.
             \en Calculate the metric length of a curve. \~
    \details \ru Длина кривой вычисляется неточно, на основе аппроксимации ломаной. 
      Если нужна более точно вычисленная длина кривой, надо пользоваться функцией CalculateMetricLength().
             \en The length of a curve is inaccurately calculated, by approximation of polyline. 
      If the more accurate curve's length is required, then use the function CalculateMetricLength(). \~
  */
  virtual double      GetLengthEvaluation() const; // \ru Оценка метрической длины кривой. \en Estimation of the metric length of a curve.

  /// \ru Вычислить габарит кривой. \en Calculate bounding box of a curve. 
  virtual void        CalculateGabarit( MbCube & cube ) const;
  // \ru Вычислить габарит в локальной системе координат. \en Calculate bounding box in the local coordinate system. 
  void        CalculateLocalGabarit( const MbMatrix3D & into, MbCube & cube ) const override;

  /// \ru Проверить вырожденная ли кривая. \en Check whether the curve is degenerated.calculate. 
  virtual bool        IsDegenerate( double eps = METRIC_PRECISION ) const;
  /// \ru Является ли линия прямолинейной? \en Whether the line is straight? 
  virtual bool        IsStraight( bool ignoreParams = false ) const;
  /// \ru Является ли кривая плоской? \en Is a curve planar? 
  virtual bool        IsPlanar    ( double accuracy = METRIC_EPSILON ) const;

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

  /// \ru Изменить носитель. Для поверхностных кривых. \en Change the carrier. For surface curves. 
  virtual void        ChangeCarrier( const MbSpaceItem & item, MbSpaceItem & init );

  /** \brief \ru Изменить носитель.
             \en Change the carrier. \~
    \details \ru Для поверхностных кривых. Заменяет текущий носитель item на новый, если возможно.
      Трансформирует носимый элемент по заданной матрице.
             \en For surface curves. Replaces the current carrier 'item' by a new one, if this is possible.
      Transforms a carried element by the given matrix. \~
    \param[in] item - \ru Исходный носитель.
                      \en An initial carrier. \~
    \param[in] init - \ru Новый носитель.
                      \en A new carrier. \~
    \param[in] matr - \ru Матрица для трансформации носимого элемента.
                      \en A matrix for transformation of a carried element. \~
    \return \ru true - если операция выполнена успешно, иначе false.
            \en True - if the operation is performed successfully, otherwise false. \~ 
    \ingroup Curves_3D
  */
  virtual bool        ChangeCarrierBorne( const MbSpaceItem & item, MbSpaceItem & init, const MbMatrix & matr ); // \ru Изменение носителя. \en Changing of carrier. 

  MbProperty & CreateProperty( MbePrompt name ) const override; // \ru Создать собственное свойство. \en Create a custom property.
  //virtual void        GetProperties( MbProperties & properties ) = 0; // \ru Выдать свойства объекта. \en Get properties of the object.
  //virtual void        SetProperties( const MbProperties & properties ) = 0; // \ru Записать свойства объекта. \en Set properties of the object.

  void        CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.

  /** \brief \ru Рассчитать массив точек для отрисовки.
             \en Calculate an array of points for drawing. \~
    \details \ru Выдать массив отрисовочных точек с заданной стрелкой прогиба. 
      Если кривая представляет собой контур, то узловые точки контура дублируются.
             \en Get an array of drawn points with a given sag. 
      If the cure is a contour then knots of a contour are duplicated. \~
    \param[in] stepData - \ru Данные для вычисления шага.
                          \en Data for step calculation. \~
    \param[in, out] polygon - \ru Полигон рассчитанных точек на кривой.
                              \en A polygon of calculated points on a curve. \~
    \ingroup Curves_3D
  */
  virtual void        CalculatePolygon( const MbStepData & stepData, MbPolygon3D & polygon ) const; // \ru Рассчитать полигон. \en Calculate a polygon. 

  /// \ru Выдать центр кривой. \en Give the curve center. 
  virtual void        GetCentre      ( MbCartPoint3D & ) const;
  /// \ru Выдать центр тяжести кривой. \en Give the gravity center of a curve. 
  virtual void        GetWeightCentre( MbCartPoint3D & ) const;

  // \ru Проекция точки на кривую (метод Ньютона). \en Point projection on a curve (the Newton method). 
  /** \brief \ru Найти проекцию точки на кривую.
             \en Find the point projection to the curve. \~
    \details \ru Найти проекцию точки на кривую или ее продолжение методом Ньютона по заданному начальному приближению.
             \en Find the point projection to the curve or its extension by the Newton method with the given initial approximation. \~
    \param[in] p - \ru Заданная точка.
                   \en A given point. \~
    \param[in] iterLimit - \ru Максимальное количество итераций.
                           \en The maximum number of iterations. \~
    \param[out] t - \ru На входе - начальное приближение, на выходе - параметр кривой, соответствующий ближайшей проекции.
                    \en Input - initial approximation, output - parameter of a curve corresponding to the nearest projection. \~
    \param[in] ext - \ru Флаг, определяющий, искать ли проекцию на продолжении кривой (если true, то искать).
                     \en A flag defining whether to seek projection on the extension of the curve. \~ 
    \result \ru Результат выполнения итерационного метода.
            \en The result of the iterative method. \~
    \ingroup Curves_3D
  */
  virtual MbeNewtonResult PointProjectionNewton( const MbCartPoint3D & p, size_t iterLimit, double & t, bool ext ) const;

  // \ru Ближайшая проекция точки на кривую. \en The nearest projection of a point onto the curve. 
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
    \param[in] pnt -    \ru Заданная точка.
                        \en A given point. \~
    \param[in,out] t -  \ru На входе - начальное приближение, на выходе - параметр кривой, соответствующий ближайшей проекции.
                        \en Input - initial approximation, output - parameter of a curve corresponding to the nearest projection. \~
    \param[in] ext -    \ru Флаг, определяющий, искать ли проекцию на продолжении кривой (если true, то искать).
                        \en A flag defining whether to seek projection on the extension of the curve. \~ 
    \param[in] tRange - \ru Диапазон изменения параметра, в котором надо найти решение.
                        \en A range of parameter changing in which the solution should be found. \~
    \return \ru Возвращает true, если найденный параметр находится в допустимом диапазоне (в соответствии с заданными параметрами ext, tRange),
            или false - в противном случае.
            \en Returns true if the found parameter is in a valid range (according to the given ext, tRange parameters),
            or false - otherwise. \~
    \ingroup Curves_3D
  */
  virtual bool        NearPointProjection  ( const MbCartPoint3D & pnt, double & t, bool ext, MbRect1D * tRange = nullptr ) const;

  // \ru Изоклины кривой (метод Ньютона). \en Isoclines of a curve (Newton method). 
  /** \brief \ru Найти изоклины кривой.
             \en Find isoclines of a curve. \~
    \details \ru Найти точку на кривой, в которой касательная параллельна некоторой плоскости,
      имеющей нормаль dir.
             \en Find the point on a curve where the tangent is parallel to a plane
      having a normal dir. \~
    \param[in] dir - \ru Вектор, задающий плоскость.
                     \en A vector which defines a plane. \~
    \param[in] iterLimit - \ru Максимальное количество итераций.
                           \en The maximum number of iterations. \~
    \param[in,out] t - \ru На входе - начальное приближение, на выходе - параметр точки с искомой касательной.
                       \en Input - initial approximation, output - parameter of a point of the required tangent. \~
    \result \ru Результат выполнения итерационного метода.
            \en The result of the iterative method. \~
    \ingroup Curves_3D
  */
  virtual MbeNewtonResult IsoclinalNewton( const MbVector3D & dir, size_t iterLimit, double & t ) const;

  // \ru Определение точек касания изоклины. \en Defining the tangent points of isocline. 
  /** \brief \ru Найти все изоклины кривой.
             \en Find all isoclines of a curve. \~
    \details \ru Найти точки на кривой, в которых касательная параллельна некоторой плоскости,
      имеющей нормаль nor.
             \en Find the points on a curve where the tangent is parallel to a plane
      having a normal nor. \~
    \param[in] nor - \ru Вектор, задающий плоскость.
                     \en A vector which defines a plane. \~
    \param[out] tIso - \ru Массив параметров точек с искомой касательной.
                       \en An array of  parameters of points for the required tangent. \~
    \ingroup Curves_3D
  */
  virtual void        GetIsoclinal   ( const MbVector3D & nor, SArray<double> & tIso ) const;

  /// \ru Вычислить ближайшее расстояние до кривой. \en Calculate the nearest distance to a curve. 
  virtual double      DistanceToCurve( const MbCurve3D & curve2, double & t1, double & t2 ) const;
  /// \ru Ближайшая точка кривой к плейсменту. \en The nearest point of a curve by the placement. 
  virtual double      DistanceToPlace( const MbPlacement3D & place, double & t0, double & angle ) const;

  /** \brief   \ru Дать физический радиус кривой или ноль, если это невозможно. 
               \en Get the physical radius of the curve or zero if it impossible. \~
      \details \ru В общем случае на запрос радиуса возвращается 0. Число, отличное от 0, можно получить лишь в том случае,
                   если кривая является дугой окружности или эквивалентна дуге окружности с заданной точностью
                   (по-умолчанию METRIC_REGION).
               \en Generally returns 0. A non-zero value may be obtained only when the curve 
                   is an arc or is equal to an arc with the set precision (METRIC_REGION by default). \~ 
      \params[in] accuracy - \ru Максимальное отклонение кривой от дуги окружности (по-умолчанию METRIC_REGION).
                             \en The maximum curve deviation from an arc (METRIC_REGION by default). \~
      \return  \ru Значение радиуса, если есть, или 0.0.
               \en Radius value if it can be obtained or 0.0. \~
  */
  virtual double      GetRadius( double accuracy = METRIC_REGION ) const;

  /** \brief \ru Дать ось окружности, геометрически совпадающей с данной кривой
             \en Get an axis of a circle which is geometrically coincident to the given curve. \~
      \details \ru Дать ось окружности, геометрически совпадающей с данной кривой
               \en Get an axis of a circle which is geometrically coincident to the given curve. \~
      \param[out] axis - \ru Ось с началом в центре окружности и направлением вдоль нормали плоскости окружности
                         \en An axis with the origin at circle's center and direction along the normal of circle's plane. \~
  */
  virtual bool        GetCircleAxis( MbAxis3D & axis ) const;

  /** \brief \ru Построить плоскую проекцию некоторой части пространственной кривой.
             \en Construct a planar projection of a piece of a space curve. \~
    \details \ru Построить плоскую проекцию некоторой части пространственной кривой.
             \en Construct a planar projection of a piece of a space curve. \~
    \param[in]      into        - \ru Матрица преобразования из глобальной системы координат в видовую плоскость.
                                  \en The transformation matrix from the global coordinate system into a plane of view. \~
    \param[in]      pRegion     - \ru Отображаемая часть кривой (paramRegion.x = t1, paramRegion.y = t2), по умолчанию - вся кривая.
                                  \en A mapped piece of a curve (paramRegion.x = t1, paramRegion.y = t2), by default - the whole curve.. \~
    \param[in]      version     - \ru Версия, по умолчанию - последняя.
                                  \en Version, last by default. \~
    \param[in, out] coincParams - \ru Флаг совпадения параметризации исходной кривой и ее проекции \n  
                                  если coincParams != nullptr, функция попытается сделать проекцию с совпадающей параметризацией \n
                                  если в результате *coincParams = true, у проекции параметризация совпадает с параметрицацией исходной кривой.
                                  \en A flag of coincidence between parameterization of initial curve and its projection \n  
                                  if coincParams != nullptr then the function tries to create a projection with coincident parameterization \n
                                  if *coincParams = true then parameterization of projection coincides with parameterization of initial curve. \~
    \return \ru Двумерная проекция кривой.
            \en Two-dimensional projection of a curve \~
    \ingroup Curves_3D
  */
  virtual MbCurve *   GetMap( const MbMatrix3D & into, MbRect1D * pRegion = nullptr,
                              VERSION version = Math::DefaultMathVersion(), bool * coincParams = nullptr ) const;

  /** \brief \ru Построить плоскую проекцию некоторой части пространственной кривой.
             \en Construct a planar projection of a piece of a space curve. \~
    \details \ru Построить плоскую проекцию некоторой части пространственной кривой для перспективного отображения.
             \en Construct a planar projection of a piece of a space curve for the perspective visualization. \~
    \param[in] into    - \ru Матрица преобразования из глобальной системы координат в видовую плоскость.
                         \en The transformation matrix from the global coordinate system into a plane of view. \~
    \param[in] zNear   - \ru Параметр перспективного отображения, равный расстоянию точки наблюдения от видовой плоскости (отрицательный)
                         \en The parameter of the perspective visualization which is equal to the distance between the observation point and the plane of view (negative). \~
    \param[in] pRegion - \ru Отображаемая часть кривой (paramRegion.x = t1, paramRegion.y = t2), по умолчанию - вся кривая.
                         \en A mapped piece of a curve (paramRegion.x = t1, paramRegion.y = t2), by default - the whole curve.. \~
    \return \ru Двумерная проекция кривой.
            \en Two-dimensional projection of a curve \~
    \ingroup Curves_3D
  */
  virtual MbCurve *   GetMapPsp( const MbMatrix3D & into, double zNear, 
                                 MbRect1D * pRegion = nullptr ) const;

  /** \brief \ru Построить плоскую проекцию пространственной кривой на плоскость.
             \en Construct a planar projection of a space curve to a plane. \~
    \details \ru Построить плоскую проекцию пространственной кривой на плоскость. \n
             \en Construct a planar projection of a space curve to a plane. \n \~
    \param[in] place    - \ru Плоскость.
                          \en A plane. \~
    \param[in] version   - \ru Версия математики.
                           \en The version of mathematics. \~
    \return \ru Двумерная проекция кривой.
            \en Two-dimensional projection of a curve \~
    \ingroup Curves_3D
  */
  virtual MbCurve *   GetProjection( const MbPlacement3D & place, VERSION version ) const; 

  /// \ru Определить количество разбиений для прохода в операциях. \en Define the number of splittings for one passage in operations. 
  virtual size_t      GetCount() const;

  /// \ru Выдать n точек кривой с равными интервалами по параметру. \en Get n points of a curve with equal intervals by parameter. 
  void        GetPointsByEvenParamDelta ( size_t n, std::vector<MbCartPoint3D> & pnts ) const;
  void        GetPointsByEvenParamDelta ( size_t n, SArray<MbCartPoint3D> & pnts ) const; // Deprecated.
  /// \ru Выдать n точек кривой с равными интервалами по длине дуги. \en Get n points of a curve with equal intervals by arc length. 
  virtual void        GetPointsByEvenLengthDelta( size_t n, std::vector<MbCartPoint3D> & pnts ) const;
  void        GetPointsByEvenLengthDelta( size_t n, SArray<MbCartPoint3D> & pnts ) const; // Deprecated.

  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \brief \ru Непрерывна ли первая производная кривой по длине и направлению?
              \en Have the first derivative of the curve the continuous length and direction?
    \details \ru Отсутствуют ли разрывы первой производной кривой по длине и направлению? \n
              \en Are absent any discontinuities at length or at direction of first derivative of the curve? \n \~
    \param[out] contLength - \ru Непрерывность длины (да/нет).
                              \en The length is continuous (true/false). \~
    \param[out] contDirect - \ru Непрерывность направления (да/нет).
                              \en The direction of the first derivative is continuous (true/false). \~
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
    \return \ru true - если кривые метрически близки.
            \en True - if curves are metrically close. \~
    \ingroup Curves_3D
  */
  bool        IsSpaceNear( const MbCurve3D & curve, double eps, bool ext, double devSag = 5.0*Math::deviateSag ) const;

  /// \ru Проверить, лежит ли точка на кривой. \en Check whether a point is on a curve or not. 
  bool        IsPointOn( const MbCartPoint3D &, double eps = METRIC_PRECISION ) const;
  /// \ru Вернуть середину параметрического диапазона кривой. \en Return the middle of parametric range of a curve. 
  double      GetTMid() const { return ((GetTMin() + GetTMax()) * 0.5); }
  /// \ru Вернуть параметрическую длину кривой. \en Return the parametric length of a curve. 
  double      GetTRange() const { return (GetTMax() - GetTMin()); }

  /// \ru Вычислить точку на кривой. \en Calculate point on the curve. 
          MbCartPoint3D PointOn   ( double & t ) const;
  /// \ru Вычислить первую производную. \en Calculate first derivative. 
          MbVector3D  FirstDer  ( double & t ) const;
  /// \ru Вычислить вторую производную. \en Calculate second derivative. 
          MbVector3D  SecondDer ( double & t ) const;
  /// \ru Вычислить третью производную. \en Calculate third derivative. 
          MbVector3D  ThirdDer  ( double & t ) const;

  /** \brief \ru Найти все особые точки функции кривизны кривой. 
             \en Find all the special points of the curvature function of the curve. \~
    \details \ru Найти все экстремумы, точки разрыва и точки перегиба функции кривизны кривой. \n
             \en Find all extrema, discontinuity points and inflection points of the curvature function of the curve. \n \~
    \param[out] points - \ru Массив найденных особых точек функции кривизны.\n
                             Первое поле каждого элемента содержит параметр найденной точки.\n
                             Второе поле каждого элемента содержит значения кривизн в найденных точках.\n
                             Данные значения могут быть следующих видов: \n 
                             1) = 0.0 - точка перегиба;\n 
                             2) < 0.0 - значение кривизны в точке минимума;\n 
                             3) > 0.0 - значение кривизны в точке максимума;\n 
                             На разрыве кривизны вставляются две точки, слева и справа от разрыва. Точка с большей кривизной \n
                             вставляется со знаком плюс, точка с меньшей кривизной вставляется со знаком минус.\n
                         \en The array of the found special points of the curvature function. \n
                             The first field of each element contains the parameter of the found point. \n
                             The second field of each element contains the curvature values at the found points.
                             These values can be of the following types:
                             1) = 0.0 - inflection point; \ n 
                             2) < 0.0 - curvature value at the minimum point; \ n
                             3) > 0.0 - the curvature value at the maximum point; \ n 
                             Two points are inserted at the curvature discontinuity, to the left and to the right of the discontinuity. \n
    \param[out] energy - \ru Значение потенциальной энергии кривой.\n
                             Энергия вычисляется как интеграл от квадрата кривизны по длине кривой. \n
                         \en Value of curve potential energy.\n
                             The energy is calculated as the integral of the square curvature along the length of the curve.\n \~
    \ingroup Curves_3D
  */
  virtual void      GetCurvatureSpecialPoints( std::vector<c3d::DoublePair> & points, double * energy = nullptr) const;

  /** \brief \ru Получить границы участков кривой, которые описываются одной аналитической функцией.
             \en Get the boundaries of the curve sections that are described by one analytical function. \~
    \details \ru Получить границы участков кривой, которые описываются одной аналитической функцией. \n
                 Функция введена для оптимизации реализации функции MbCurve3D::GetCurvatureSpecialPoints, чтобы не насчитывать точки разрыва. \n
             \en Get the boundaries of the curve sections that are described by one analytical function. \n 
                 The function was introduced to optimize the implementation of the function MbCurve3D::GetCurvatureSpecialPoints, so as not to calculate the break points.\n \~
    \param[out] params - \ru Параметры кривой, в которых меняется аналитическая функция.
                         \en Curve parameters in which the analytical function changes. \~
    \ingroup Curves_3D
  */
  virtual void        GetAnalyticalFunctionsBounds( std::vector<double> & params ) const;
  /// \ ru Определение точек излома кривой. \en The determination of curve smoothness break points.
  virtual void        BreakPoints( std::vector<double> & vBreaks, double precision = ANGLE_REGION ) const;

  /** \brief \ru Вычислить граничную точку.
             \en Calculate the boundary point. \~
    \details \ru Вычислить граничную точку. \n
             \en Calculate the boundary point. \n \~
    \param[in] number - \ru Номер граничной точки. Значение 1 соответствует начальной точке кривой, 2 - конечной.
                        \en A number of a boundary point. The value 1 corresponds to the start point of a curve, 2 - to the end point. \~
    \return \ru Вычисленная точка.
            \en A calculated point. \~
    \ingroup Curves_3D
  */
  MbCartPoint3D GetLimitPoint( ptrdiff_t number ) const; // \ru number <= 1 : в начале, иначе - в конце \en Number <= 1 : at start, otherwise - at end 

  /** \brief \ru Вычислить граничную точку.
             \en Calculate the boundary point. \~
    \details \ru Вычислить граничную точку. \n
             \en Calculate the boundary point. \n \~
    \param[in] number - \ru Номер граничной точки. Значение 1 соответствует начальной точке кривой, 2 - конечной.
                        \en A number of a boundary point. The value 1 corresponds to the start point of a curve, 2 - to the end point. \~
    \param[in, out] pnt - \ru Вычисленная точка.
                          \en A calculated point. \~
    \ingroup Curves_3D
  */
  void        GetLimitPoint( ptrdiff_t number, MbCartPoint3D & pnt ) const;

  /** \brief \ru Вычислить касательный вектор в граничной точке.
             \en Calculate a tangent vector to the boundary point. \~
    \details \ru Вычислить нормализованный касательный вектор в граничной точке.
             \en Calculate a normalized tangent vector to the boundary point. \~
    \param[in] number - \ru Номер граничной точки. Значение 1 соответствует начальной точке кривой, 2 - конечной.
                        \en A number of a boundary point. The value 1 corresponds to the start point of a curve, 2 - to the end point. \~
    \return \ru Касательный вектор.
            \en Tangent vector. \~
  */
  MbVector3D  GetLimitTangent( ptrdiff_t number ) const;

  /** \brief \ru Вычислить касательный вектор в граничной точке.
             \en Calculate a tangent vector to the boundary point. \~
    \details \ru Вычислить нормализованный касательный вектор в граничной точке.
             \en Calculate a normalized tangent vector to the boundary point. \~
    \param[in] number - \ru Номер граничной точки. Значение 1 соответствует начальной точке кривой, 2 - конечной.
                        \en A number of a boundary point. The value 1 corresponds to the start point of a curve, 2 - to the end point. \~
    \param[in, out] v - \ru Касательный вектор.
                        \en Tangent vector. \~
  */
  void        GetLimitTangent( ptrdiff_t number, MbVector3D & v ) const;

  /** \brief \ru Равны ли граничные точки.
             \en Are boundary points equal? \~
    \details \ru Равны ли граничные точки кривой.
             \en Are curve boundary points equal? \~
    \return \ru true, если точки равны.
            \en Returns true if points are equal. \~
  */
  bool        AreLimitPointsEqual() const { return GetLimitPoint( 1 ) == GetLimitPoint( 2 ); } 

  /// \ru Загнать в параметрическую область. \en Move to the parametric region. 
  bool        SetInParamRegion( double & t ) const;
  /// \ru Проверить, что параметр в диапазоне кривой. \en Check whether a parameter is in the range of the curve. 
  bool        IsParamOn( double t, double eps ) const { return ( GetTMin()-eps<=t && t<=GetTMax()+eps ); }
  /// \ru Являются ли кривая инверсно такой же? \en Whether an inversed curve is the same. 
  bool        IsInverseSame( const MbCurve3D & curve, double accuracy = LENGTH_EPSILON ) const;

  /** \brief \ru Определить, является ли кривая репараметризованно такой же.
             \en Define whether a reparameterized curve is the same. \~
    \details \ru Определить, является ли кривая репараметризованно такой же.
             \en Define whether a reparameterized curve is the same. \~
    \param[in]  curve  - \ru Кривая для сравнения.
                         \en A curve for comparison. \~
    \param[out] factor - \ru Коэффициент сжатия параметрической области при переходе
      к указанной кривой.
                         \en Coefficient of compression of parametric region at the time of transition
      to the pointed curve. \~
  */
  virtual bool        IsReparamSame( const MbCurve3D & curve, double & factor ) const;

  /// \ru Дать приращение параметра, осреднённо соответствующее единичной длине в пространстве. \en Get increment of parameter, corresponding to the unit length in space. 
  virtual double      GetParamToUnit() const;
  /// \ru Дать приращение параметра, соответствующее единичной длине в пространстве. \en Get increment of parameter, corresponding to the unit length in space. 
  virtual double      GetParamToUnit( double t ) const;
  /// \ru Дать минимально различимую величину параметра. \en Get the minimum distinguishable value of parameter. 
  double      GetTEpsilon() const;
  /// \ru Дать минимально различимую величину параметра. \en Get the minimum distinguishable value of parameter. 
  double      GetTEpsilon( double t ) const;
  /// \ru Дать минимально различимую величину параметра. \en Get the minimum distinguishable value of parameter. 
  double      GetTRegion() const;
  /// \ru Дать минимально различимую величину параметра. \en Get the minimum distinguishable value of parameter. 
  double      GetTRegion( double t ) const;
  /// \ru Дать минимально различимую величину параметра с заданной трехмерной точностью в зависимости от параметра. \en Get the minimum distinguishable value of parameter with the given three-dimensional accuracy according to parameter. 
  double      GetTRegion( double t, const MbDimAccuracy & xyzEps ) const;


  // \ru Геометрия подложки тождественна геометрии кривой, но отлична параметризация. \en The geometry of the a substrate is identical to the geometry of a curve, but parameterization differs.  
  /// \ru Выдать подложку или себя. \en Get a substrate or itself. 
  virtual const MbCurve3D & GetSubstrate() const;
  /// \ru Выдать подложку или себя. \en Get a substrate or itself. 
  virtual       MbCurve3D & SetSubstrate();
  /// \ru Направление подложки относительно кривой или наоборот. \en Direction of a substrate relative to a curve or vice versa. 
  virtual int         SubstrateCurveDirection() const;
  /// \ru Преобразовать параметр подложки в параметр кривой. \en Transform a substrate parameter to the curve parameter. 
  virtual void        SubstrateToCurve( double & ) const;
  /// \ru Преобразовать параметр кривой в параметр подложки. \en Transform a curve parameter to the substrate parameter. 
  virtual void        CurveToSubstrate( double & ) const;

  /** \brief \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская.
             \en Get planar curve and placement if the space curve is planar. \~
    \details \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская (после использования вызывать DeleteItem на двумерную кривую).
             \en Get planar curve and placement if the space curve is planar (after the using call DeleteItem for two-dimensional curves). \~
    \param[out] curve2d    - \ru Полученная плоская кривая.
                             \en The resulting flat curve. \~
    \param[out] place      - \ru Система координат полученной двумерной кривой. 
                             \en The coordinate system of the resulting 2D curve. \~
    \param[in]  saveParams - \ru Параметр, задающий сохранение соответствия параметризации у двумерной кривой.
                                 Если true - параметризация кривой curve2d должна соответствовать параметризациии исходной кривой this.
                                 Если false - параметризации кривых могут не соответствовать. Кривая curve2d может быть найдена с больший вероятностью, чем если бы saveParams = true.
                             \en The parameter specifying the preservation of the correspondence of the parameterization for the two-dimensional curve.
                                 If true - parameterization of curve2d curve must match the parameterization of the original curve this.
                                 If false - curve parameterizations may not correspond. The curve2d is more likely to be detected than with the true flag. \~
    \param[in]  params     - \ru Параметры проверки.
                             \en Validation parameters. \~
    \return \ru true, если создана плоская кривая.
            \en true if a flat curve was created. \~
  */  
  virtual bool        GetPlaneCurve( MbCurve *& curve2d, MbPlacement3D & place, bool saveParams, PlanarCheckParams params = PlanarCheckParams() ) const;
  
  /** \brief \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская.
             \en Get planar curve and placement if the space curve is planar. \~
    \details \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская (после использования вызывать DeleteItem на двумерную кривую).
             \en Get planar curve and placement if the space curve is planar (after the using call DeleteItem for two-dimensional curves). \~
    \param[out] curve2d    - \ru Полученная плоская кривая.
                             \en The resulting flat curve. \~
    \param[out] place      - \ru Система координат полученной двумерной кривой.
                             \en The coordinate system of the resulting 2D curve. \~
    \param[in]  saveParams - \ru Параметр, задающий сохранение соответствия параметризации у двумерной кривой.
                                 Если true - параметризация кривой curve2d должна соответствовать параметризациии исходной кривой this.
                                 Если false - параметризации кривых могут не соответствовать. Кривая curve2d может быть найдена с больший вероятностью, чем если бы saveParams = true.
                             \en The parameter specifying the preservation of the correspondence of the parameterization for the two-dimensional curve.
                                 If true - parameterization of curve2d curve must match the parameterization of the original curve this.
                                 If false - curve parameterizations may not correspond. The curve2d is more likely to be detected than with the true flag. \~
    \param[in]  params     - \ru Параметры проверки.
                             \en Validation parameters. \~
    \return \ru true, если создана плоская кривая.
            \en true if a flat curve was created. \~
  */
  bool        GetPlaneCurve( SPtr<MbCurve> & curve2d, MbPlacement3D & place, bool saveParams, PlanarCheckParams params = PlanarCheckParams() ) const;
  
  /** \brief \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская.
             \en Get planar curve and placement if the space curve is planar. \~
    \details \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская (после использования вызывать DeleteItem на двумерную кривую).
             \en Get planar curve and placement if the space curve is planar (after the using call DeleteItem for two-dimensional curves). \~
    \param[out] curve2d    - \ru Полученная плоская кривая.
                             \en The resulting flat curve. \~
    \param[out] place      - \ru Система координат полученной двумерной кривой. 
                             \en The coordinate system of the resulting 2D curve. \~
    \param[in]  saveParams - \ru Параметр, задающий сохранение соответствия параметризации у двумерной кривой.
                                 Если true - параметризация кривой curve2d должна соответствовать параметризациии исходной кривой this.
                                 Если false - параметризации кривых могут не соответствовать. Кривая curve2d может быть найдена с больший вероятностью, чем если бы saveParams = true.
                             \en The parameter specifying the preservation of the correspondence of the parameterization for the two-dimensional curve.
                                 If true - parameterization of curve2d curve must match the parameterization of the original curve this.
                                 If false - curve parameterizations may not correspond. The curve2d is more likely to be detected than with the true flag. \~
    \param[in]  params     - \ru Параметры проверки.
                             \en Validation parameters. \~
    \return \ru true, если создана плоская кривая.
            \en true if a flat curve was created. \~
  */  
  bool        GetPlaneCurve( SPtr<const MbCurve> & curve2d, MbPlacement3D & place, bool saveParams, PlanarCheckParams params = PlanarCheckParams() ) const;
  
  /// \ru Дать поверхностную кривую, если пространственная кривая поверхностная (после использования вызывать DeleteItem на аргументы). \en Get surface curve if the space curve is surface (after the using call DeleteItem for arguments) 
  virtual bool        GetSurfaceCurve( MbCurve *& curve2d, MbSurface *& surface, VERSION version = Math::DefaultMathVersion() ) const;
  /// \ru Дать поверхностную кривую, если пространственная кривая поверхностная (после использования вызывать DeleteItem на аргументы). \en Get surface curve if the space curve is surface (after the using call DeleteItem for arguments) 
  bool        GetSurfaceCurve( SPtr<MbCurve> & curve2d, SPtr<MbSurface> & surface, VERSION version = Math::DefaultMathVersion() ) const;
  /// \ru Дать поверхностную кривую, если пространственная кривая поверхностная (после использования вызывать DeleteItem на аргументы). \en Get surface curve if the space curve is surface (after the using call DeleteItem for arguments) 
  bool        GetSurfaceCurve( SPtr<const MbCurve> & curve2d, SPtr<const MbSurface> & surface, VERSION version = Math::DefaultMathVersion() ) const;
  /// \ru Заполнить плейсемент, если кривая плоская. \en Fill the placement if a curve is planar. 
  virtual bool        GetPlacement( MbPlacement3D & place, PlanarCheckParams params = PlanarCheckParams() ) const;
  /// \ru Является ли объект смещением. \en Is the object is a shift? 
  virtual bool        IsShift( const MbSpaceItem &, MbVector3D &, bool & isSame, double accuracy = LENGTH_EPSILON ) const;
  /// \ru Подобные ли кривые для объединения (слива). \en Whether the curves to union (joining) are similar. 
  virtual bool        IsSimilarToCurve( const MbCurve3D & other, double precision = METRIC_PRECISION ) const;
  /// \ru Аппроксимация кривой плоскогранной трубкой радиуса radius. \en Approximation of a curve by the flat tube with the given radius. 
  void        CalculateGrid( double radius, const MbStepData & stepData, MbMesh & mesh ) const;   
          
  SimpleName  GetCurveName() const { return name; } ///< \ru Имя кривой. \en A curve name. 
  void        SetCurveName( SimpleName newName ) { name = newName; } ///< \ru Установить имя кривой. \en Set a curve name. 
  /** \} */

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
  virtual MbResultType Extend( const MbCurveExtensionParameters3D & parameters, c3d::SpaceCurveSPtr & resCurve ) const;

  // \ru Функции унификации кривой и вектора кривых в шаблонных функциях. \en Functions for compatibility of a curve and a vector of curves in template functions.
  size_t      size() const { return 1; } ///< \ru Размер кривой трактуемой как в виде вектора кривых. \en Size of curve interpreted as vector of curves. 
  const   MbCurve3D * operator [] ( size_t ) const { return this; }  ///< \ru Оператор доступа. \en An access operator.

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию \en Declaration without implementation of the assignment operator to prevent an assignment by default 
  MbCurve3D & operator = ( const MbCurve3D & );

  DECLARE_PERSISTENT_CLASS( MbCurve3D )
};

IMPL_PERSISTENT_OPS( MbCurve3D )


//------------------------------------------------------------------------------
// \ru Определить, замкнута ли кривая фактически независимо от гладкости замыкания. \en Determine whether a curve is closed regardless of the smoothness of the closure.
// ---
inline bool MbCurve3D::IsTouch( double eps ) const
{
  MbCartPoint3D p1, p2;
  _PointOn( GetTMin(), p1 );
  _PointOn( GetTMax(), p2 );
  if ( c3d::EqualPoints( p1, p2, eps ) ) {
    MbCartPoint3D p;
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
  \details \ru Вычислить параметры ближайших точек двух кривых и расстояние между этими точками. \n
           \en Calculate parameters of the nearest points of two curves and the distance between these points. \n \~
  \param[in] curve1 - \ru Кривая 1.
                      \en Curve 1. \~
  \param[in] ext1   - \ru Признак поиска на продолжении кривой 1.
                      \en An attribute of search on the extension of the curve 1. \~
  \param[in] curve2 - \ru Кривая 2.
                      \en Curve 2. \~
  \param[in] ext2   - \ru Признак поиска на продолжении кривой 2.
                      \en An attribute of search on the extension of the curve 2. \~
  \param[in] t1     - \ru Параметр точки кривой 1.
                      \en A point parameter of curve 1. \~
  \param[in] t2     - \ru Параметр точки кривой 2.
                      \en A point parameter of curve 2. \~
  \param[in] dmin   - \ru Расстояние между точками кривых.
                      \en The distance between points of curves. \~
  \return \ru Возвращает nr_Success (+1) или nr_Special(0) в случае успешного определения, в случае неудачи возвращает nr_Failure(-1).
          \en Return nr_Success (+1) or nr_Special(0) in a case of successful defining, return nr_Failure(-1) in a case of failure. \~
  \ingroup Curves_3D
*/
// ---
MATH_FUNC (MbeNewtonResult) NearestPoints( const MbCurve3D & curve1, bool ext1, // \ru Признак поиска на продолжении объекта \en An attribute of search at the extension of an object. 
                                           const MbCurve3D & curve2, bool ext2, // \ru Признак поиска на продолжении объекта \en An attribute of search at the extension of an object. 
                                           double & t1, double & t2, double & dmin );


//------------------------------------------------------------------------------
/** \brief \ru Определение параметров ближайших точек кривых.
           \en Definition of parameters of the nearest points of curves. \~
  \details \ru Итерационное определение параметров ближайших точек кривых 
    путём решения уравнений методом Ньютона при заданных начальных приближениях.
    Если кривые пересекаются и начальные приближения близки к точке пересечения, 
    то будут найдены параметры точки пересечения. \n
    Если в области начальных приближений параметров кривые не пересекаются, 
    то будут найдены параметры точек кривых, касательные в которых ортогональны отрезку, 
    соединяющему найденные точки.  \n
           \en Iterative definition of parameters of the nearest points of curves 
    by solving an equation by the Newton method with given initial approximations.
    If curves intersect and initial approximations are close to the intersection point, 
    then parameters of the intersection point will be found. \n
    If curves do not intersect in the region of initial approximations, 
    then there will be found parameters of curves points where tangents to the curves are orthogonal to the segment 
    which connects the found points.  \n \~
  \param[in] curve1      - \ru Кривая 1.
                           \en Curve 1. \~
  \param[in] ext1        - \ru Признак поиска на продолжении кривой 1.
                           \en An attribute of search on the extension of the curve 1. \~
  \param[in] curve2      - \ru Кривая 2.
                           \en Curve 2. \~
  \param[in] ext2        - \ru Признак поиска на продолжении кривой 2.
                           \en An attribute of search on the extension of the curve 2. \~
  \param[in] funcEpsilon - \ru Максимальная погрешность расстояния между точками пересечения кривых.
                           \en The minimal tolerance of the distance between curves intersection points. \~
  \param[in] iterLimit   - \ru Максимальное число итераций.
                           \en The maximum number of iterations. \~
  \param[in, out] t1     - \ru Параметр кривой 1 для точки пересечения (начальное приближение на входе).
                           \en Parameter of the curve 1 for the intersection point (the initial approximation at input). \~
  \param[in, out] t2     - \ru Параметр кривой 2 для точки пересечения (начальное приближение на входе).
                           \en Parameter of the curve 2 for the intersection point (the initial approximation at input). \~
  \param[in] correctNewtonParam - \ru Выполнять корректировку приращения параметра по предыщушему приращению параметра.
                                  \en Modify current parameter changes by previous parameter changes. \~
  \return \ru Код ошибки: случае успешного определения nr_Success (+1), nr_Special(0) или nr_Failure(-1) - в случае неудачи.
          \en Error code: in a case of successful defining nr_Success (+1), nr_Special(0) or nr_Failure(-1) - in a case of failure. \~
  \ingroup Curves_3D
*/
// ---
MATH_FUNC (MbeNewtonResult) CurveCrossNewton( const MbCurve3D & curve1, bool ext1,
                                              const MbCurve3D & curve2, bool ext2,
                                              double funcEpsilon, size_t iterLimit,
                                              double & t1, double & t2,
                                              bool correctNewtonParam = true );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить точки полигона кривой в общем случае.
           \en Calculate polygon points of curve. \~
  \details \ru Вычислить точки полигона кривой в общем случае. \n
           \en Calculate polygon points of curve. \n \~
  \param[in] curve - \ru Кривая.
                     \en Curve. \~
  \param[in] stepData - \ru Данные для вычисления шага.
                        \en Data for step calculation. \~
  \param[out] paramPoints - \ru Массив параметров и точек.
                            \en Array of parameters and points. \~
  \ingroup Curves_3D
*/
// ---
MATH_FUNC (void) CalculatePolygon( const MbCurve3D & curve, const MbStepData & stepData, std::vector< std::pair<double,MbCartPoint3D> > & paramPoints );


#endif // __CURVE3D_H
