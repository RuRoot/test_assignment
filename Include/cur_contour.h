////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Контур в двумерном пространстве.
         \en Contour in two-dimensional space. \~ 
 
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_CONTOUR_H
#define __CUR_CONTOUR_H


#include <curve.h>
#include <cur_line_segment.h>
#include <templ_s_list.h>
#include <templ_sptr.h>
#include <alg_base.h>
#include <mb_cube_tree.h>


class   MbArc;
class             SimpleNameArray;
class             MbCurveIntoNurbsInfo;


class   MbContour;
namespace c3d // namespace C3D
{
typedef SPtr<MbContour>                                       PlaneContourSPtr;
typedef SPtr<const MbContour>                                 ConstPlaneContourSPtr;

typedef std::vector<MbContour *>                              PlaneContoursVector;
typedef std::vector<const MbContour *>                        ConstPlaneContoursVector;

typedef std::vector<PlaneContourSPtr>                         PlaneContoursSPtrVector;
typedef std::vector<ConstPlaneContourSPtr>                    ConstPlaneContoursSPtrVector;

typedef MbCubeTree<MbContour, MbRect, MbCartPoint, MbVector>  ContourRectsTree;
}


//------------------------------------------------------------------------------
/** \brief \ru Контур в двумерном пространстве.
           \en Contour in two-dimensional space. \~ 
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
    Двумерный контур используется для плоского моделирования, а также для описания двумерных связных областей, например, для описания области определения параметров поверхности.\n
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
    The two-dimensional contour is used for planar modeling and also for describing of two-dimensional areas, for example for determining of the domain of surface.\n \~
  \ingroup Curves_2D
*/
// ---
class MATH_CLASS MbContour : public MbCurve {
protected :
  RPArray<MbCurve>        segments;     ///< \ru Множество сегментов контура. \en An array of contour segments. 
  bool                    closed;       ///< \ru Признак замкнутости кривой. \en An Attribute of curve closedness. 
  double                  paramLength;  ///< \ru Параметрическая длина контура. \en Parametric length of a contour. 
protected:
  mutable atomic_double         metricLength; ///< \ru Метрическая длина контура. \en Metric length of a contour. 
  mutable MbRect                rect;         ///< \ru Габаритный прямоугольник. \en Bounding box. 
  mutable c3d::AtomicDoublePair areaSign;     ///< \ru Площадь контура со знаком. \en Contour area with a sign. 

public :
  /// \ru Пустой контур. \en Empty contour. 
  MbContour();

  /** \brief \ru Конструктор по набору кривых.
              \en Constructor by curves vector. \~
    \details \ru Конструктор по набору кривых. Кривые добавляются в контур без проверки, что начало каждого последующего сегмента стыкуется с концом предыдущего. Выполнение данного условия должно гарантироваться вызывающим кодом. \n
              \en Constructor by curves vector. Curves are added to the contour without checking that the beginning of each subsequent segment joins the end of the previous one. The condition must be guaranteed by the calling code. \n \~
    \param[in] initCurves - \ru Кривые.
                            \en Curves. \~
    \param[in] sameCurves - \ru Использовать оригиналы кривых (true) или их копии (false).
                            \en Use original curves (true) or copies thereof (false). \~
  */
  template <class Curves>
  MbContour( const Curves & initCurves, bool sameCurves );
protected :
  explicit MbContour( const MbContour *, MbRegDuplicate * ); ///< \ru Конструктор копирования. \en Copy constructor.
public :
  virtual ~MbContour();

public:
  VISITING_CLASS( MbContour ); 

  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */

  MbePlaneType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbePlaneType  Type() const override; // \ru Тип элемента \en A type of element 
  MbPlaneItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element.
  bool        IsSimilar ( const MbPlaneItem & ) const override;   // \ru Являются ли элементы подобными \en Whether the elements are similar.
  bool        SetEqual( const MbPlaneItem & ) override;           // \ru Сделать элементы равными \en Make equal elements. 
  bool        IsSame( const MbPlaneItem &, double accuracy = LENGTH_EPSILON ) const override; // \ru Является ли кривая curve копией данной кривой ? \en Whether the curve "curve" is a copy of a given curve?
  void        Transform( const MbMatrix &, MbRegTransform * ireg = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix.
  void        Move( const MbVector &, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Сдвиг \en Translation.
  void        Rotate( const MbCartPoint &, const MbDirection & angle, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Поворот \en Rotation.

  /** \brief \ru Построить эквидистантную кривую, смещённую на заданное расстояние.
             \en Construct the equidistant curve which is shifted by the given value. \~
    \details \ru Построить эквидистантную кривую, смещённую на заданное расстояние. 
      Функция вставляет в копии контура дуги нулевого радиуса между стыками сегментов.
      Это нужно для обеспечения непрерывности контура для обоих направлений смещения.
      Но это может приводить к появлению самопересечений контура.
      Затем выполняет эквидистантное смещение всех сегментов.
      Данная функция имеет ограниченную область применения.
      В качестве альтернативы можно использовать функцию OffsetContour.
             \en Construct the equidistant curve which is shifted by the given value. 
      The function inserts (in a contour's copy) the arcs of zero radius between neighbor segments.
      This is necessary to provide continuity of the contour for both directions of displacement.
      But it can lead to the appearance of self-intersections of an contour.
      Then the function performs an equidistant offset of all segments.
      This function has a limited scope of using.
      You can use the OffsetContour function instead this function. \~
    \param[in] rad - \ru Величина эквидистантного смещения.
                     \en Equidistant offset. \~
    \return \ru Возвращает эквидистантный контур, если получилось его построить, иначе - nullptr.
            \en Returns the equidistant curve if it's possible to build it, otherwise - nullptr. \~
  */
  MbCurve *   Offset( double rad ) const override; // \ru Смещение контура. \en Shift of a contour 

  void        AddYourGabaritTo ( MbRect & ) const override; // \ru Добавь свой габарит в прямой прям-к. \en Add bounding box into a straight box. 
  void        CalculateGabarit ( MbRect & ) const override; // \ru Определить габариты кривой. \en Determine the bounding box of the curve. 
  void        CalculateLocalGabarit( const MbMatrix & into, MbRect & local ) const override; // \ru Добавь в прям-к свой габарит с учетом матрицы \en Add bounding box into a box with consideration of the matrix. 

  const MbRect & GetGabarit() const { if ( rect.IsEmpty() ) { MbRect tmp;  CalculateGabarit( tmp ); } return rect; }
  const MbRect & GetCube()    const { if ( rect.IsEmpty() ) { MbRect tmp;  CalculateGabarit( tmp ); } return rect; }

  /// \ru Сбросить рассчитанный габарит контура. \en Reset the calculated contour bounding box.  
  void        SetDirtyGabarit() const { ScopedRecursiveLock ll( GetLock() ); rect.SetEmpty(); }
  /// \ru Копировать габарит контура в контур (использовать только для передачи габарита в копию контура). \en Copy contour bounding box to contour copy (use only to transfer into contour copy).
  void        CopyGabarit( const MbContour & c ) { rect = c.rect; }
  /// \ru Пуст ли габарит контура? \en Is the contour bounding box empty?  
  bool        IsGabaritEmpty() const { return rect.IsEmpty(); }

  double      DistanceToPoint( const MbCartPoint & ) const override; // \ru Расстояние до точки \en Distance to a point.

  MbeState    Deformation( const MbRect &, const MbMatrix & ) override;  // \ru Деформация \en Deformation. 
  bool        IsInRectForDeform( const MbRect & ) const override;      // \ru Виден ли объект в заданном прямоугольнике для деформации \en Whether the object is visible in the specified rectangle for the deformation. 

  void        Refresh() override; // \ru Сбросить все временные данные \en Reset all temporary data. 
  void        PrepareIntegralData( const bool forced ) const override; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.
  // \ru Удалить часть контура между параметрами t1 и t2 \en Remove a part of the contour between t1 and t2 parameters. 
  MbeState    DeletePart( double t1, double t2, MbCurve *& part2 ) override;
  // \ru Оставить часть контура между параметрами t1 и t2 \en Save a part of the contour between t1 and t2 parameters. 
  MbeState    TrimmPart( double t1, double t2, MbCurve *& part2 ) override;
  // \ru Построить усеченную кривую c учетом заданной двумерной точности. \en Construct a trimmed curve with the given two-dimensional accuracy.
  MbCurve *   Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyEps, bool saveParamLenAndLaw ) const override; 
  using MbCurve::Trimmed; // \ru Метод базового класса MbCurve без учета двумерной точности. Точность по умолчанию - MbDimAccuracy::twoDimAcc.  \en Method of MbCurve base class without two-dimensional accuracy. The default accuracy is MbDimAccuracy::twoDimAcc.

  /** \} */
  /** \ru \name Функции описания области определения кривой.
      \en \name Functions for curve domain description.
      \{ */

  double      GetTMin()    const override; // \ru Вернуть минимальное значение параметра. \en Get the minimum value of parameter.
  double      GetTMax()    const override; // \ru Вернуть максимальное значение параметра. \en Get the maximum value of parameter.
  bool        IsClosed()   const override; // \ru Проверка замкнутости кривой. \en Check for curve closedness.
  bool        IsStraight( bool ignoreParams = false ) const override; // \ru Признак прямолинейности кривой. \en An attribute of curve straightness. 
  bool        IsSmoothConnected( double angleEps, bool ignoreLimits = false ) const override; // \ru Являются ли стыки контура\кривой гладкими? \en Whether the joints of contour\curve are smooth. 
  /** \} */

  /** \ru \name Функции для работы в области определения кривой. 
            Функции PointOn, FirstDer, SecondDer, ThirdDer,... корректируют параметр
            при выходе его за пределы области определения.
      \en \name Functions for working in the domain of a curve. 
            Functions: PointOn, FirstDer, SecondDer, ThirdDer,... correct parameter
            when it is outside domain.
      \{ */
  void        PointOn  ( double &, MbCartPoint & ) const override;  // \ru Точка на кривой \en The point on the curve
  void        FirstDer ( double &, MbVector    & ) const override;  // \ru Первая производная \en First derivative
  void        SecondDer( double &, MbVector    & ) const override;  // \ru Вторая производная \en Second derivative
  void        ThirdDer ( double &, MbVector    & ) const override;  // \ru Третья производная \en Third derivative
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
  void       _PointOn  ( double, MbCartPoint & ) const override;  // \ru Точка на кривой. \en The point on the curve.
  void       _FirstDer ( double, MbVector    & ) const override;  // \ru Первая производная. \en First derivative.
  void       _SecondDer( double, MbVector    & ) const override;  // \ru Вторая производная. \en Second derivative.
  void       _ThirdDer ( double, MbVector    & ) const override;  // \ru Третья производная. \en Third derivative.
  /** \} */

  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметра кривой.
      \en \name Functions for get of the group of data inside and outside the curve's domain of parameter.
      \{ */
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void        Explore( double & t, bool ext,
                       MbCartPoint & pnt, MbVector & fir, MbVector * sec, MbVector * thir ) const override;
  /** \} */

  /** \ru \name Функции движения по кривой
      \en \name Functions of the motion along the curve
    \{ */
  double      Step( double t, double sag ) const override; // \ru Вычисление шага аппроксимации \en Calculate step of approximation. 
  double      DeviationStep( double t, double angle ) const override; // \ru Вычисление шага аппроксимации с учетом угла отклонения \en Calculation of step approximation with consideration of the deviation angle. 
  /** \} */

  /** \ru \name Общие функции кривой
      \en \name Common functions of curve
      \{ */
  bool        HasLength( double & ) const override;
  double      GetMetricLength() const override;   // \ru Метрическая длина контура \en Metric length of a contour. 
  double      GetLengthEvaluation() const override; // \ru Оценка метрической длины кривой \en Evaluation of the metric length of the curve.

  double      CalculateMetricLength() const override; // \ru Посчитать метрическую длину \en Calculate the metric length 
  double      GetParamLength() const { return paramLength; }
  double      CalculateParamLength(); // \ru Посчитать параметрическую длину \en Calculate the parametric length 

  /// \ru Вычисление площади контура, если контур замкнут. \en Calculation of contour area if contour is closed.  
  double      GetArea( double sag = Math::deviateSag ) const
              { 
                sag = ::fabs(sag);
                if ( ::fabs( areaSign.first - sag ) > EXTENT_EPSILON )
                  return CalculateArea( sag );
                return areaSign.second;
              }

  MbNurbs *   NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;
  MbContour * NurbsContour() const override;

  void        CheckClosed( double closedEps = Math::LengthEps );  ///< \ru Установить признак замкнутости контура. \en Set the closedness attribute of contour. 
  void        InitClosed( bool c ) { closed = c; } ///< \ru Установить признак замкнутости контура. \en Set the closedness attribute of contour. 

  /** \brief \ru Проверить замкнутость и непрерывность точек контура.
              \en Check for closedness and continuity of contour points. \~
    \details \ru Проверить замкнутость и непрерывность точек контура.
      Проверяется совпадение первой и последней точки контура,
      совпадение последней точки каждого сегмента с первой точкой следующего сегмента.
      Равенство точек проверяется по умолчанию грубо - с точностью, равной 5 * PARAM_NEAR.
              \en Check for closedness and continuity of contour points.
      Checking for coincidence of first and last points of the contour,
      coincidence of the last point of each segment with the first point of the next segment.
      Equality of points is checked roughly by default - with tolerance is equal to 5* PARAM_NEAR. \~
    \return \ru true, если контур замкнутый и непрерывный.
            \en true, if contour is closed and continuous. \~
  */
  bool        IsClosedContinuousC0( double eps = 5.0 * PARAM_NEAR ) const;

  void        CloseByLineSeg( bool calcInternalData ); ///< \ru Замкнуть контур отрезком. \en Close the contour by segment. 

  // \ru Посчитать метрическую длину разомкнутой кривой с заданной точностью \en Calculate the metric length of unclosed curve within the given tolerance 
  double      CalculateLength( double t1, double t2 ) const override;
  // \ru Сдвинуть параметр t на расстояние len \en Move parameter t on the distance len 
  bool        DistanceAlong( double & t, double len, int curveDir, double eps = Math::LengthEps,
                             VERSION version = Math::DefaultMathVersion() ) const override;
  /// \ru Сбросить переменные кэширования. \en Reset variables caching.
  void        Clear( bool calculateParamLength = true )
              {
                if ( calculateParamLength )
                  CalculateParamLength(); // \ru Параметрическая длина контура \en Parametric length of a contour 
                metricLength = -1; // \ru Метрическая длина кривой \en Metric length of a curve 
                rect.SetEmpty();
                areaSign.first = -1.0;
              }
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

  size_t      GetSegmentsCount() const { return segments.size(); }     ///< \ru Выдать количество сегментов контура. \en Get the number of contour segments. 
  const   MbCurve *   GetSegment( size_t ind ) const { return segments[ind]; } ///< \ru Выдать сегмент контура по индексу. \en Get contour segment by the index. 
          MbCurve *   SetSegment( size_t ind )       { return segments[ind]; } ///< \ru Выдать сегмент контура по индексу. \en Get contour segment by the index. 

  // \ru Положение точки относительно кривой. \en The point position relative to the curve. 
  // \ru iloc_InItem = 1 - точка находится слева от контура, \en Iloc_InItem = 1 - point is located to the left of the contour,  
  // \ru iloc_OnItem = 0 - точка находится на контуре, \en Iloc_OnItem = 0 - point is located on the contour, 
  // \ru iloc_OutOfItem = -1 - точка находится справа от контура. \en Iloc_OutOfItem = -1 - point is located to the right of the contour. 
  MbeItemLocation PointRelative( const MbCartPoint & pnt, double eps = Math::LengthEps ) const override; 
  // \ru Положение точки относительно кривой. \en The point position relative to the curve. 
  MbeLocation     PointLocation( const MbCartPoint & pnt, double eps = Math::LengthEps ) const override;

  double      PointProjection( const MbCartPoint & ) const override;  // \ru Проекция точки на кривую \en Point projection on the curve

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
  bool        NearPointProjection( const MbCartPoint &, double xEpsilon, double yEpsilon,
                                   double & t, bool ext, MbRect1D * tRange = nullptr ) const override;

  /** \brief \ru Параметрическое расстояние до ближайшей границы.
              \en Parametric distance to the nearest boundary.
    \details \ru Параметрическое расстояние до ближайшей границы. \n
              \en Parametric distance to the nearest boundary. \n \~
    \param[in] pnt - \ru Тестируемая точка.
                      \en A testing point. \~
    \param[in] eps - \ru Точность.
                      \en Accuracy. \~
      \return \ru Расстояние до ближайшей границы.
              \en Distance to the nearest boundary. \~
  */
  double      DistanceToBorder( const MbCartPoint & pnt, double eps = Math::paramRegion ) const;

  void        Trimm( double t1, double t2, bool saveParamLenAndLaw = false ); ///< \ru Выделить часть контура. \en Trim a part of the contour. 
  void        Trimm( double t1, double t2, const MbDimAccuracy & xyEps, bool saveParamLenAndLaw ); ///< \ru Выделить часть контура c учетом двумерной точности. \en Trim a part of the contour with the given two-dimensional accuracy.

  // \ru Продлить кривую. \en Extend the curve. \~
  MbResultType Extend( const MbCurveExtensionParameters & parameters, c3d::PlaneCurveSPtr & resCurve ) const override;

  // \ru Вычисление всех перпендикуляров к кривой из данной точки \en Calculation of all perpendiculars to the curve from a given point 
  void        PerpendicularPoint( const MbCartPoint &, SArray<double> & tFind ) const override;
  // \ru Вычисление всех касательных к кривой из данной точки \en Calculation of all tangents to the curve from a given point 

  void        TangentPoint( const MbCartPoint &, SArray<double> & tFind ) const override;

  void        IntersectHorizontal( double y, SArray<double> & ) const override; // \ru Пересечение с горизонтальной прямой \en Intersection with the horizontal line
  void        IntersectVertical  ( double x, SArray<double> & ) const override; // \ru Пересечение с вертикальной прямой \en Intersection with the vertical line
  void        SelfIntersect( SArray<MbCrossPoint> &, double metricEps = Math::LengthEps ) const override; // \ru Самопересечение контура \en Self-intersection of the contour

  /** \brief \ru Есть ли самопересечения контура?
              \en Is it a contour with self-intersections? \~
    \details \ru Есть ли самопересечения контура?
              \en Is it a contour with self-intersections? \~
    \param[in] metricEps - \ru Точность (по умолчанию рекомендуется использовать Math::LengthEps).
                            \en Accuracy (it's recommended to use Math::LengthEps). \~
    \param[in] considerPartialCoincidence - \ru Считать частичное совпадение соседних сегментов самопересечением (true - по умолчанию).
                                            \en Consider partial coincidence of neighboring segments as self-intersection (true - by default). \~
  */
  bool        IsSelfIntersect( double metricEps, bool considerPartialCoincidence ) const;

  // \ru Функции находятся в файле equcntr.cpp \en Functions are in the file equcntr.cpp 

  /// \ru Скругление двух соседних элементов с информацией об удалении \en Fillet of two neighboring elements with information about removal 
  bool        FilletTwoSegments( ptrdiff_t & index, double rad, bool & del1, bool & del2 );
  /// \ru Скругление двух соседних элементов \en Fillet of two neighboring elements 
  bool        FilletTwoSegments( ptrdiff_t & index, double rad );
  /// \ru Вставка фаски между двумя соседними элементами с информацией об удалении \en Insertion of chamfer between two neighboring elements with information about removal 
  bool        ChamferTwoSegments( ptrdiff_t & index, double len, double angle,
                                  bool type, bool firstSeg, bool & del1, bool & del2 );
  /// \ru Вставка фаски между двумя соседними элементами \en Insertion of chamfer between two neighboring elements 
  bool        ChamferTwoSegments( ptrdiff_t & index, double len, double angle,
                                  bool type, bool firstSeg = true );
  bool        Fillet( double rad ); ///< \ru Скругление контура \en Fillet of contour 
  bool        Chamfer( double len, double angle, bool type ); ///< \ru Вставка фаски \en Insertion of the chamfer 
  MbeState    RemoveFilletOrChamfer( const MbCartPoint & pnt );  ///< \ru Удалить скругление или фаску контура \en Remove fillet or contour chamfer 
  /// \ru Разбить контур на непересекающиеся сегменты. \en Split contour into non-overlapping segments. 
  bool        InsertCrossPoints();
  /// \ru Разбиение сегментов контура в точках пересечения. \en Splitting of contour segments at the points of intersection. 
  void        BreakSegment( ptrdiff_t & index, ptrdiff_t firtsIdx,
                            SArray<MbCrossPoint> & cross, bool firstCurve = true );

  bool        CheckConnection( double eps = Math::LengthEps ) const;  ///< \ru Проверка непрерывности контура \en Check for contour continuity. 
  bool        CheckConnection( double xEps, double yEps ) const;      ///< \ru Проверка непрерывности контура \en Check for contour continuity. 

  /// \ru Скругление контура дугой нулевого радиуса. \en Rounding contour by arc of zero radius. 
  void        FilletZero( int defaultSense, bool fullInsert = false );
  /// \ru Вставка фаски для построения эквидистанты. \en Insertion of chamfer for construction of the offset. 
  void        ChamferZero( double rad );
  /// \ru Удаление вырожденных сегментов контура. \en Removal of degenerate contour segments. 
  void        DeleteDegenerateSegments( double radius, MbCurve * curve, bool mode );

  /** \brief \ru Построение эквидистанты к контуру.
              \en Construction of offset to contour. \~
    \details \ru Построение эквидистанты к контуру справа и слева.
      Имя каждого эквидистантного контура совпадает с именем исходного.
              \en Construction of offset to contour of the left and right.
      A name of every offset contour matches with the name of the initial one. \~
    \param[in]  radLeft  - \ru Радиус эквидистанты слева по направлению.
                            \en The equidistance radius on the left by direction. \~
    \param[in]  radRight - \ru Радиус эквидистанты справа по направлению.
                            \en The equidistance radius on the right by direction. \~
    \param[in]  side     - \ru Признак, с какой стороны строить:\n
                            0 - слева по направлению,\n
                            1 - справа по направлению,\n
                            2 - с двух сторон.
                            \en Attribute defining the side to construct:\n
                            0 - on the left by direction,\n
                            1 - on the right by direction,\n
                            2 - on the both sides. \~
    \param[in]  mode     - \ru Способ обхода углов:\n
                            true - дугой,
                            false - срезом.
                            \en The way of traverse of angles:\n
                            true - by arc,
                            false - by section. \~
    \param[out] equLeft  - \ru Массив контуров слева.
                            \en The array of contours on the left side. \~
    \param[out] equRight - \ru Массив контуров справа.
                            \en The array of contours on the right side. \~
  */
  void        Equid( double radLeft, double radRight, int side, bool mode,
                     PArray<MbCurve> & equLeft, PArray<MbCurve> & equRight );

  /// \ru Построение новых контуров из эквидистанты. \en Construction of new contours from equidistance. 
  void        CreateNewContours( RPArray<MbCurve> & );

  /// \ru Вычисление площади контура, если контур замкнут. \en Calculation of contour area if contour is closed.  
  double      CalculateArea( double sag = Math::deviateSag ) const;
  /// \ru Определение направления обхода контура, если контур замкнут. \en Determination of traverse direction if contour is closed. 
  int         GetSense() const;
  /// \ru Установить направление обхода контура. \en Set the traverse direction of the contour.
  void        SetSense( int sense );

  // \ru Изменить направление обхода контура \en Change the traverse direction of the contour 
  void        Inverse( MbRegTransform * = nullptr ) override;
  // \ru Согласовать параметризацию сегментов, если до инвертации она была согласованной. \en Agree on segment parameterization, if it was consistent before inversion.
  bool        NormalizeReparametrization();
  size_t      GetCount() const override; // \ru Количество разбиений для прохода в операциях \en The number of partitions for passage in the operations  
  // \ru Выдать характерную точку ограниченной кривой если она ближе чем dmax \en Get characteristic point of bounded curve if it is closer than dmax 
  bool        DistanceToPointIfLess( const MbCartPoint & toP, double & d ) const override; // \ru Расстояние до точки, если оно меньше d \en Distance to the point if it is less than d 
  bool        GetSpecificPoint( const MbCartPoint & from, double & dmax, MbCartPoint & pnt ) const override;

  /// \ru Выдать среднюю точку сегмента контура. \en Get a mid-point of the contour segment. 
  bool        GetSegmentMiddlePoint( const MbCartPoint & from, MbCartPoint & midPoint ) const;
  /// \ru Выдать линейный сегмент контура. \en Get the linear segment of contour.  
  bool        GetLinearSegment( const MbCartPoint & from, double maxDist, MbCartPoint & p1, MbCartPoint & p2, double & d ) const;
  /// \ru Выдать дуговой сегмент контура. \en Get the arc segment of contour. 
  MbArc *     GetArcSegment( const MbCartPoint & from, double maxDist, double & d ) const;
  /// \ru Выдать длину сегмента контура. \en Get the contour segment length. 
  bool        GetSegmentLength( const MbCartPoint & from, double & length ) const;

  bool        GetWeightCentre( MbCartPoint & ) const override; // \ru Выдать центр тяжести контура \en Get gravity center of contour 
  bool        GetCentre      ( MbCartPoint & ) const override; // \ru Выдать центр кривой \en Get the center of curve 

  /// \ru Найти ближайший к точке узел контура. \en Find the nearest node of contour to point.
  ptrdiff_t   FindNearestNode( const MbCartPoint & to ) const;
  /// \ru Найти ближайший к точке сегмент контура. \en Find the nearest segment of contour to point.
  ptrdiff_t   FindNearestSegment( const MbCartPoint & to ) const;

  // \ru Определение особых точек офсетной кривой \en Determination of singular points of the offset curve 
  void        OffsetCuspPoint( SArray<double> & tCusps, double dist ) const override;
  double      GetRadius( double accuracy = PARAM_REGION ) const override; // \ru Дать физический радиус объекта или ноль, если это невозможно. \en Get the physical radius of the object or null if it impossible.
  bool        GetAxisPoint( MbCartPoint & ) const override; // \ru Выдать центр оси кривой. \en Give the curve axis center.

  void        CombineNurbsSegments(); ///< \ru Объединить NURBS кривые в контуре. \en Unite NURBS curves into the contour.

  void        GetProperties( MbProperties & ) override;  // \ru Выдать свойства объекта \en Get properties of the object
  void        SetProperties( const MbProperties & ) override;  // \ru Записать свойства объекта \en Set properties of the object
  void        GetBasisPoints( MbControlData & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \brief \ru Непрерывна ли первая производная кривой по длине и направлению?
             \en Have the first derivative of the curve the continuous length and direction?
    \details \ru Отсутствуют ли разрывы производной по длине и направлению в стыках сегментов контура? \n
             \en Are absent any discontinuities of the derivative at length or at direction in the junction of path segments? \n \~
    \param[out] contLength - \ru Непрерывность длины (да/нет).
                              \en The length is continuous (true/false). \~
    \param[out] contDirect - \ru Непрерывность направления (да/нет).
                              \en The direction of the first derivative is continuous (true/false). \~
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
  /// \ ru Определение точек излома контура. \en The determination of contour smoothness break points.
  void        BreakPoints( std::vector<double> & vBreaks, double precision = ANGLE_REGION ) const override;

  /** \} */
  /** \ru \name Функции работы с сегментами контура
      \en \name Function for working with segments of contour
      \{ */

  bool        Init( List<MbCurve> & );   ///< \ru Инициализация по списку кривых. \en Initialization by list of curves.
  void        Init( const MbContour & ); ///< \ru Инициализация по контуру. \en Initialization by a contour.

  /** \brief \ru Инициализация по массиву кривых.
              \en Initialization by array of curves. \~
    \details \ru Инициализация по массиву кривых. Кривые добавляются в контур без проверки, что начало каждого последующего сегмента стыкуется с концом предыдущего. Выполнение данного условия должно гарантироваться вызывающим кодом. \n
              \en Initialization by array of curves. Curves are added to the contour without checking that the beginning of each subsequent segment joins the end of the previous one. The condition must be guaranteed by the calling code. \n \~
    \param[in] curves - \ru Кривые.
                        \en Curves. \~
    \param[in] sameCurves - \ru Использовать оригиналы кривых (true) или их копии (false).
                            \en Use original curves (true) or copies thereof (false). \~
    \return \ru Возвращает true, если кривые были добавлены.
            \en Returns true if curves were added. \~
  */
  template <class Curves>
  bool        Init( Curves & curves, bool sameCurves );
  /// \ru Инициализация по массиву точек (замкнутый контур). \en Initialization by array of points (closed contour).
  template <class Points>
  bool        InitByPoints( const Points & );

  bool        InitAsRectangle( const MbCartPoint * ); ///< \ru Инициализация как прямоугольника ( приходит 4 точки ) \en Initialization as rectangle (4 points are given).
  bool        InitByRectangle( const MbRect & );      ///< \ru Инициализация по прямоугольнику габарита. \en Initialization by rectangle of bounding box.

  bool        AddSegment             ( MbCurve * ); ///< \ru Добавить сегмент в контур. \en Add a segment to the contour. 
  bool        AddSegmentOrDeleteCurve( MbCurve * ); ///< \ru Добавить кривую как сегмент или удалить ее. \en Add a curve as segment or remove its. 

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
  MbCurve *   AddSegment( const MbCurve * pBasis, double t1, double t2, int sense = 1 );

  bool        AddAtSegment   ( MbCurve * newSegment, size_t index ); ///< \ru Вставить сегмент перед сегментом контура с индексом index. \en Insert a segment before the contour segment with the index "index". 
  bool        AddAfterSegment( MbCurve * newSegment, size_t index ); ///< \ru Вставить сегмент после сегмента контура с индексом index. \en Insert a segment after the contour segment with the index "index". 

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
    \param[in] version - \ru Версия.
                          \en Version. \~
    \return \ru Возвращает true, если кривая была добавлена.
            \en Returns true if the curve was added. \~
  */
  bool        AddCurveWithRuledCheck( MbCurve & newCur, double absEps, bool toEndOnly = false, bool checkSame = true,
                                      VERSION version = Math::DefaultMathVersion() );

  void        DeleteSegments();            ///< \ru Удалить все сегменты в контуре. \en Remove all segments from contour. 
  void        DeleteSegment( size_t ind ); ///< \ru Удалить сегмент в контуре. \en Remove a segment from contour. 
  void        DetachSegments();            ///< \ru Отцепить все сегменты от контура без удаления. \en Detach all segments from the contour without removing. 
  MbCurve *   DetachSegment( size_t ind ); ///< \ru Отцепить сегмент от контура и вернуть его. \en Detach a segment from contour return it. 

  template <class CurvesVector>
  void        DetachSegments( CurvesVector & segms ); ///< \ru Отцепить все сегменты от контура без удаления. \en Detach all segments from the contour without removing. 

  void        SetSegment( MbCurve & newSegment, size_t ind ); ///< \ru Заменить сегмент в контуре. \en Replace a segment in the contour.
  void        SegmentsAdd( MbCurve & newSegment, bool calculateParamLength = true ); ///< \ru Добавить сегмент в контур без проверки. \en Add a segment to the contour without checking. 
  void        SegmentsInsert( size_t ind, MbCurve & newSegment ); ///< \ru Вставить сегмент в контур перед индексом без проверки. \en Insert a segment into contour before an index without checking. 
  void        SegmentsRemove( size_t ind ); ///< \ru Удалить сегмент без проверки. \en Remove a segment without checking. 
  void        SegmentsDetach( size_t ind ); ///< \ru Отцепить сегмент без проверки. \en Detach a segment without checking. 

  void        Calculate( bool calcArea = false ); ///< \ru Рассчитать параметры: rect, paramLength, metricLength, closed. \en Calculate parameters:  rect, paramLength, metricLength, closed. 

  // \ru Управление распределением памяти в массиве segments \en Control of memory allocation in the array "segments" 
  void        SegmentsReserve( size_t additionalSpace ) { segments.Reserve( additionalSpace ); } ///< \ru Зарезервировать место под столько элементов. \en Reserve memory for this number of elements. 
  void        SegmentsAdjust ()                         { segments.Adjust();                   } ///< \ru Удалить лишнюю память. \en Free the unnecessary memory. 

  template <class CurvesVector>
  bool        GetSegments( CurvesVector & segms ) const; ///< \ru Получить сегменты контура. \en Get contour segments.

  void        SetMetricLength( double len ) const { ScopedRecursiveLock ll( GetLock() ); metricLength = len; }
  /// \ru Установить начальную (конечную) точку для замкнутого контура. \en Set the start (end) point for closed contour. 
  bool        SetBegEndPoint( double t );
  /// \ru Заменить сегменты контуры и сегменты полилинии. \en Replace segments of contour and segments of polyline. 
  void        ReplaceContoursAndPolylines();
  void        GetPolygon( double sag, SArray<MbCartPoint> & poly, double eps ) const; ///< \ru Дать точки полигона. \en Get points of polygon. 

  bool        IsAnyCurvilinear() const; ///< \ru Есть ли в контуре криволинейный сегмент. \en Whether the contour has a curved segment. 
  bool        IsSameSegments( const MbContour &, double accuracy = PARAM_PRECISION ) const; ///< \ru Содержат ли контура идентичные сегменты. \en Whether contours contains identical segments. 
  bool        GetBegSegmentPoint( size_t i, MbCartPoint & ) const; ///< \ru Дать начальную точку i-го сегмента. \en Get the start point of i-th segment. 
  bool        GetEndSegmentPoint( size_t i, MbCartPoint & ) const; ///< \ru Дать конечную  точку i-го сегмента. \en Get the end point of i-th segment. 

  /** \brief \ru Нормаль по параметру, учитывая стыки сегментов.
              \en Normal by parameter with consideration of segments joints \~
    \details \ru Нормаль по параметру, учитывая стыки сегментов.\n
              \en Normal by parameter with consideration of segments joints \n \~
    \param[in] t - \ru Параметр на контуре
                    \en Parameter on the contour \~
    \param[out] norm - \ru Единичный вектор нормали, если не попали на стык сегментов\n
      если попали на стык сегментов - вектор, направленный, 
      как сумма двух нормалей на сегментах в точке стыка, 
      с длиной, равной 1, деленной на синус половинного угла между сегментами
                        \en Unit vector of normal if not hit on the joint of segments \n
      if we got on the joint of segments then the vector is directed, 
      as the sum of two normals on segments in joint, 
      with length which is equal to 1 divided by the sine of half angle between the segments \~
    \return \ru true, если попали на стык сегментов
            \en true, if got on the joint of segments \~
  */
  bool        CornerNormal( double t, MbVector & norm ) const;

  /** \brief \ru Параметры стыков сегментов.
              \en Parameters of segments joints. \~
    \details \ru Параметры стыков сегментов кроме минимального
      и максимального параметров контура.
              \en Parameters of segments joints without minimal
      and maximal contour parameter. \~
    \param[out] params - \ru Набор параметров.
                          \en Set of parameters. \~
  */
  template <class Params>
  void        GetCornerParams( Params & params ) const;
    // \ru Вычисление всех касательных к кривой из данной точки \en Calculation of all tangents to the curve from a given point 

  /** \brief \ru Вычисление двух касательных (для параметров стыков).
              \en Calculation of two tangents (for parameters of joints). \~
    \details \ru Вычисление двух касательных для параметра стыка по соответствующим сегментам.
                  Если параметр не стыковой, то касательные равны.
              \en Calculation of two tangents for parameter of segments joint corresponding to the segments.
                  If parameter is not one of parameters of segments joints tangents are equal.
      and maximal contour parameter. \~
    \param[in] t      - \ru Параметр.
                        \en A parameter. \~
    \param[out] tan1  - \ru Первая касательная.
                        \en First tangent. \~
    \param[out] tan2  - \ru Вторая касательная.
                        \en Second tangent. \~
  */
  bool        GetTwoTangents( double t, MbVector & tan1, MbVector & tan2 ) const;

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
  ptrdiff_t   _FindSegment( double & t, double & tSeg ) const; // \ru Нахождение сегмента контура \en Finding of a contour segment     
  void        FilletTwoSegmentsZeroRadius( ptrdiff_t & index, int defaultSense, bool fullInsert );  // \ru Скругление двух соседних элементов дугой нулевого радиуса. \en Rounding two neighboring elements by arc of zero radius.    
  void        ChamferTwoSegmentsZeroRadius( ptrdiff_t & index, double rad ); // \ru Вставка фаски между двумя соседними элементами для построения эквидистанты. \en Insertion of chamfer between two neighboring elements for construction of the offset.

  void        SetClosed(); ///< \ru Установить признак замкнутости контура. \en Set the closedness attribute of contour. 
  void        CreateNoDegenerateContours( PArray<MbCurve> & curves, SimpleName contourName, PArray<MbCurve> & contours );///< \ru Создать невырожденные контуры. \en Create nondegenerate contours.

  MbContour & operator = ( const MbContour & initContour );

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbContour )
}; // MbContour


IMPL_PERSISTENT_OPS( MbContour )


//------------------------------------------------------------------------------
// \ru Конструктор по набору кривых. \en Constructor by curves vector.
// ---
template <class Curves>
MbContour::MbContour( const Curves & initCurves, bool same )
  : MbCurve     (       )
  , segments    ( 0, 1  )
  , closed      ( false )
  , paramLength (  0.0  )
  , metricLength( -1.0  )
  , rect        (       )
  , areaSign    ( -1.0, 0.0 )
{
  size_t count = initCurves.size();
  segments.reserve( count );
  SPtr<MbCurve> segment;
  for ( size_t i = 0; i < count; ++i ) {
    if ( initCurves[i] == nullptr )
      continue;
    segment = same ? &const_cast<MbCurve &>( *initCurves[i] ) : &static_cast<MbCurve &>( initCurves[i]->Duplicate() );
    SegmentsAdd( *segment, false );
  }

  CalculateGabarit( rect ); // посчитать габарит
  CalculateParamLength();
  CalculateMetricLength();
  SetClosed(); // установить признак замкнутости контура
}


//------------------------------------------------------------------------------
// \ru Инициализация по массиву точек (замкнутый контур). \en Initialization by array of points (closed contour).
// ---
template <class Points>
bool MbContour::InitByPoints( const Points & points )
{
  size_t count = points.size();

  if ( count > 1 ) {
    DeleteSegments();
    segments.reserve( count );
    for ( size_t i = 0; i < count; ++i ) {
      MbLineSegment * seg = new MbLineSegment( points[i], points[ (i + 1) % count ] );
      segments.push_back( seg );
      seg->AddRef();
    }
    closed = true;
    Clear();
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru Инициализация по массиву кривых. \en Initialization by array of curves.
// ---
template <class Curves>
bool MbContour::Init( Curves & curves, bool same )
{
  bool res = false;

  if ( !curves.empty() ) {
    DeleteSegments();

    size_t count = curves.size();
    segments.reserve( count );
    for ( size_t i = 0; i < count; i++ ) {
      if ( curves[i] == nullptr )
        continue;
      MbCurve * segm = same ? &const_cast<MbCurve &>(*curves[i]) : static_cast<MbCurve *>( &curves[i]->Duplicate() );
      SegmentsAdd( *segm );
    }
    CalculateGabarit( rect ); // посчитать габарит
    CalculateParamLength();
    CalculateMetricLength();
    SetClosed(); // установить признак замкнутости контура
    res = segments.size() > 0;
  }

  return res;
}


//------------------------------------------------------------------------------
// \ru Получить сегменты контура. \en Get contour segments.
// ---
template <class CurvesVector>
bool MbContour::GetSegments( CurvesVector & segms ) const
{
  bool res = false;
  SPtr<MbCurve> segm;
  size_t addCnt = segments.size();
  segms.reserve( segms.size() + addCnt );
  for ( size_t k = 0; k < addCnt; ++k ) {
    segm = segments[k];
    segms.push_back( segm );
    res = true;
  }
  return res;
}


//------------------------------------------------------------------------------
// \ru Отцепить все сегменты от контура без удаления. \en Detach all segments from the contour without removing. 
// ---
template <class CurvesVector>
void MbContour::DetachSegments( CurvesVector & segms )
{
  SPtr<MbCurve> segm;
  size_t addCnt = segments.size(); 
  segms.reserve( segms.size() + addCnt );
  for ( size_t k = 0; k < addCnt; ++k ) {
    ::DecRefItem( segments[k] );
    segm = segments[k];
    segms.push_back( segm );
    ::DetachItem( segm );
  }
  segments.clear();
  SetClosed(); // установить признак замкнутости контура
  Clear();  // сбросить контур
}


//------------------------------------------------------------------------------
// \ru Параметры стыков сегментов. \en Parameters of segments joints.
// ---
template <class Params>
void MbContour::GetCornerParams( Params & params ) const
{
  size_t segCount = GetSegmentsCount();
  if ( segCount > 1 ) {
    double pLength = 0.0;

    const MbCurve * segment = GetSegment( 0 );
    if ( segment != nullptr )
      pLength = segment->GetParamLength();

    for ( size_t segInd = 1; segInd < segCount; ++segInd ) {
      params.push_back( pLength );
      segment = GetSegment( segInd );
      if ( segment != nullptr )
        pLength += segment->GetParamLength();
    }
  }
}


#endif // __CUR_CONTOUR_H
