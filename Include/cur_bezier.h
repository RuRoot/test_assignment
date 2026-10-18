////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Сплайн Безье в двумерном пространстве.
         \en Bezier spline in two-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_BEZIER_H
#define __CUR_BEZIER_H


#include <cur_polycurve.h>
#include <templ_c_array.h>
#include <mb_dim_accuracy.h>


class   MbArc;
class             MbRegDuplicate;
class             MbRegTransform;


//------------------------------------------------------------------------------
/** \brief \ru Сплайн Безье в двумерном пространстве.
           \en Bezier spline in two-dimensional space. \~
  \details \ru Сплайн Безье в двумерном пространстве определяется контрольными точками pointList. \n
    Сплайн Безье является составной кривой, образованной стыкующимися между собой кривыми Безье третьей степени. 
    Каждая кривая Безье третьей степени построена по четырём соседним контрольным точкам множества pointList. 
    Для незамкнутого сплайна Безье первая и последняя контрольная точка множества pointList не используются.
    Таким образом, сплайн Безье проходит через каждую контрольную точку множества pointList с индексом 3n+1, где n - целое число.
    Значение параметра вдоль каждой кривой Безье третьей степени увеличивается на единицу. 
    Параметр сплайна Безье изменяется от нуля до k, где k - количество кривых Безье третьей степени, образующих сплайн. 
    В общем случае первая производная сплайна Безье может быть разрывной как по длине, так и по направлению.
           \en Bezier spline in two-dimensional space is defined by control points pointList. \n
    Bezier spline is composite curve formed by connected among themselves Bezier curves with third degree. 
    Each third-degree Bezier curve is constructed by four adjacent control points pointList. 
    First and last control point from pointList for unclosed Bezier spline are not used
    Thus Bezier spline passes through each control point with index 3n+1, where n is integer.
    The value of parameter along each third-degree Bezier curve is incremented. 
    The parameter of Bezier spline changes from zero to k, where k is the number of  tree-degree Bezier curves which form spline. 
    In general case the first derivative of Bezier spline can be discontinuous by length and direction. \~
  \ingroup Curves_2D
*/
// ---
class MATH_CLASS MbBezier : public MbPolyCurve {
private :
  ptrdiff_t splinesCount; ///< \ru Количество сплайнов. \en The number of splines. 
  int       form;         ///< \ru Форма сплайна. \en The shape of spline. 

public :
    /** \brief \ru Конструктор по массиву всех точек.
             \en Constructor by array of all points. \~
    \details \ru Конструктор по массиву всех точек(полюсов и коромысел),
      для создания из трехмерной кривой MbBezier3D.
             \en Constructor by array of all points(poles and rockers),
      for creation from three-dimensional curve MbBezier3D. \~
    \deprecated \ru Метод устарел. \en The method is deprecated. \~
    \param[in] closed - \ru Замкнута ли кривая.
                        \en Is curve closed? \~
    \param[in] points - \ru Массив точек. 
                        Должен быть получен из массива точек трехмерной кривой MbBezier3D, которая является
                        плоской, путем проецирования их на плоскость кривой.
                        Количество точек должно быть кратно трем. Минимальное количество точек - шесть.
                        \en An array of points. 
                        It must be obtained from point array of three-dimensional curve MbBezier3D which is 
                        planar, by projecting them onto the plane of the curve.
                        The number of points must be multiple of three. The minimal number of points is six. \~
  */
  DEPRECATE_DECLARE MbBezier( bool closed, const SArray<MbCartPoint> & points );
  /** \brief \ru Конструктор по полюсам.
             \en Constructor by poles. \~
    \details \ru Конструктор по полюсам. В массиве initList заданы только полюса.
             \en Constructor by poles. initList array contains only poles. \~
    \deprecated \ru Метод устарел. \en The method is deprecated. \~
    \param[in] initList - \ru Массив полюсов кривой. 
                          Минимальное количество точек в массиве равно двум.
                          \en An array of curve poles. 
                          Minimal number of points in array equals two. \~
    \param[in] cls - \ru Замкнутость кривой.
                     \en A curve closedness. \~
    \param[in] initForm - \ru Форма сплайна. Возможные значения:\n
      0 - Стандартная форма.
      1 - Более выпуклая форма кривой.
                          \en The shape of spline. Possible values:\n
      0 - The standard form.
      1 - More convex form of curve. \~
  */
  DEPRECATE_DECLARE MbBezier( const SArray<MbCartPoint> & initList, bool cls, int initForm = 0 );
  
protected :
  MbBezier( const MbBezier & pCurve ); ///< \ru Конструктор копирования. \en Copy-constructor. 
  /** \brief \ru Конструктор по четырем точкам.
             \en Constructor by four points. \~
    \details \ru Конструктор по четырем точкам.
             \en Constructor by four points. \~
    \param[in] initList - \ru Массив точек. Количество точек должно быть равно четырем.
                              Если точек больше четырех, то на каждых последовательных четырех
                              точках строится классический кубический сплайн Безье.
                              Гладкость полученной кривой в точках стыка сегментов может нарушаться.
                          \en An array of points. Number of points must be equal to four.
                              When number of points is more then four classical cubic bezier is
                              created for each sequential four points. The smoothness of result curve
                              can be broken. \~
  */
  MbBezier( const SArray<MbCartPoint> & initList );

  /** \brief \ru Конструктор по сегменту Bezier-кривой.
             \en Constructor by segment of Bezier curve. \~
    \details \ru Конструктор по сегменту заданной Bezier-кривой.
             \en Constructor by segment of a given Bezier curve. \~
    \param[in] pCurve - \ru Заданная кривая.
                        \en A given curve. \~
    \param[in] iseg - \ru Номер сегмента кривой.
                      \en A number of curve segment. \~
  */
  MbBezier( const MbBezier & pCurve, ptrdiff_t iseg );
  /** \brief \ru Конструктор по дуге окружности.
             \en Constructor by circle arc. \~
    \details \ru Построена кривая Безье, точно аппроксимирующая заданную дугу окружности.
             \en There was constructed a Bezier curve which approximates a given arc of a circle. \~
    \param[in] arc - \ru Дуга окружности.
                     \en Circle arc. \~
  */
  MbBezier( const MbArc & arc ); // \ru Инициализация по дуге окружности \en Initialization by a circle arc 
public :
  virtual ~MbBezier();  ///< \ru Деструктор. \en Destructor. 

public :
  /** \brief \ru Создать копию сплайна.
             \en Create copy of spline. \~
    \details \ru Создать копию сплайна.\n
             \en Create copy of spline.\n \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbBezier *   Create( const MbBezier & other );
  /** \brief \ru Создать сплайн по четырем точкам.
             \en Create spline by four points. \~
    \details \ru Создать сплайн по четырем точкам.
             \en Create spline by four points. \~
    \param[in] initList - \ru Массив точек. Количество точек должно быть равно четырем.
                              Если точек больше четырех, то на каждых последовательных четырех
                              точках строится классический кубический сплайн Безье.
                              Гладкость полученной кривой в точках стыка сегментов может нарушаться.
                          \en An array of points. Number of points must be equal to four.
                              When number of points is more then four classical cubic bezier is
                              created for each sequential four points. The smoothness of result curve
                              can be broken. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbBezier *   Create( const SArray<MbCartPoint> & initList );
  /** \brief \ru Создать сплайн по массиву всех точек.
             \en Create spline by array of all points. \~
    \details \ru Создать сплайн по массиву всех точек(полюсов и коромысел),
                 для создания из трехмерной кривой MbBezier3D.
             \en Create spline by array of all points(poles and rockers),
                 for creation from three-dimensional curve MbBezier3D. \~         
    \param[in] closed - \ru Замкнута ли кривая.
                        \en Is curve closed? \~
    \param[in] points - \ru Массив точек. 
                            Должен быть получен из массива точек трехмерной кривой MbBezier3D, которая является
                            плоской, путем проецирования их на плоскость кривой.
                            Количество точек должно быть кратно трем. Минимальное количество точек - шесть.
                        \en An array of points. 
                            It must be obtained from point array of three-dimensional curve MbBezier3D which is 
                            planar, by projecting them onto the plane of the curve.
                            The number of points must be multiple of three. The minimal number of points is six. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbBezier *   Create( bool closed, const SArray<MbCartPoint> & points );
  /** \brief \ru Создать сплайн по сегменту Bezier-кривой.
             \en Create spline by segment of Bezier curve. \~
    \details \ru Создать сплайн по сегменту заданной Bezier-кривой.
             \en Create spline by segment of a given Bezier curve. \~
    \param[in] pCurve - \ru Заданная кривая.
                        \en A given curve. \~
    \param[in] iseg - \ru Номер сегмента кривой.
                      \en A number of curve segment. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbBezier *   Create( const MbBezier & pCurve, ptrdiff_t iseg );
  /** \brief \ru Создать сплайн по полюсам.
             \en Create spline by poles. \~
    \details \ru Создать сплайн по полюсам. В массиве initList заданы только полюса.
             \en Create spline by poles. initList array contains only poles. \~
    \param[in] initList - \ru Массив полюсов кривой. 
                          Минимальное количество точек в массиве равно двум.
                          \en An array of curve poles. 
                          Minimal number of points in array equals two. \~
    \param[in] cls - \ru Замкнутость кривой.
                     \en A curve closedness. \~
    \param[in] initForm - \ru Форма сплайна. Возможные значения:\n
                              0 - Стандартная форма. \n
                              1 - Более выпуклая форма кривой.
                          \en The shape of spline. Possible values:\n
                              0 - The standard form. \n
                              1 - More convex form of curve. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbBezier *   Create( const SArray<MbCartPoint> & initList, bool cls, int initForm = 0 );
  /** \brief \ru Создать сплайн по дуге окружности.
             \en Create spline by circle arc. \~
    \details \ru Построена кривая Безье, точно аппроксимирующая заданную дугу окружности.
             \en There was constructed a Bezier curve which approximates a given arc of a circle. \~
    \param[in] arc - \ru Дуга окружности.
                     \en Circle arc. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbBezier *   Create( const MbArc & arc ); // \ru Инициализация по дуге окружности \en Initialization by a circle arc 

public :
  VISITING_CLASS( MbBezier ); 

  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */
  MbePlaneType  IsA() const override; // \ru Тип элемента \en A type of element 
  bool        SetEqual( const MbPlaneItem & ) override;        // \ru Сделать элементы равными \en Make the elements equal 
  bool        IsSame( const MbPlaneItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Является ли кривая curve копией данной кривой ? \en Whether the 'curve' curve is duplicate of current curve. 
  void        Transform( const MbMatrix & matr, MbRegTransform * ireg = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move( const MbVector & to, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate( const MbCartPoint & pnt, const MbDirection & angle, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Поворот \en Rotation
  MbPlaneItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
/** \} */
 /** \ru \name Функции инициализации сплайна.
     \en \name Spline initialization functions.
      \{ */  

  /** \brief \ru Инициировать кривую по полюсам и замкнутости.
             \en Initialize a curve by poles and closedness. \~
    \details \ru Инициировать кривую по полюсам и замкнутости. В массиве initList заданы только полюса.
             \en Initialize a curve by poles and closedness. initList array contains only poles. \~
    \param[in] initList - \ru Массив полюсов кривой.
                          \en An array of curve poles. \~
    \param[in] cls - \ru Замкнутость кривой.
                     \en A curve closedness. \~
  */
  void        Init( const SArray<MbCartPoint> & initList, bool cls );

  /** \brief \ru Инициировать кривую по заданной кривой Безье.
             \en Initialize a curve by a given Bezier curve. \~
    \details \ru Инициировать кривую по заданной кривой Безье. \n
             \en Initialize a curve by a given Bezier curve. \n \~
    \param[in] initCurve - \ru Заданная кривая.
                           \en A given curve. \~
  */
  void        Init( const MbBezier & initCurve );

  /** \brief \ru Инициировать кривую по дуге окружности.
             \en Initialize a curve by a circle arc. \~
    \details \ru Построена кривая Безье, точно аппроксимирующая заданную дугу окружности.
             \en There was constructed a Bezier curve which approximates a given arc of a circle. \~
    \param[in] arc - \ru Дуга окружности.
                     \en Circle arc. \~
  */
  void        Init( const MbArc & arc ); // \ru Инициализация по дуге окружности \en Initialization by a circle arc 

  /** \brief \ru Инициировать кривую по контрольным точкам.
             \en Initialize a curve by control points. \~
    \details \ru Инициировать кривую по контрольным точкам. 
      В массиве initList входят и полюса и коромысла.
             \en Initialize a curve by control points. 
      The array initList contains poles and "rocker arms". \~ 
    \param[in] initList - \ru Массив контрольных точек кривой.
                          \en An array of control points of curve. \~
  */
  void        InitCtrlPoints( const SArray<MbCartPoint> & initList );
/** \} */
 /** \ru \name Функции описания области определения кривой.
     \en \name Functions for curve domain description.
      \{ */
  double      GetTMin() const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter 
  double      GetTMax() const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter 
  /** \} */
  /** \ru \name Функции для работы в области определения кривой. 
            Функции PointOn, FirstDer, SecondDer, ThirdDer,... корректируют параметр
            при выходе его за пределы области определения.
      \en \name Functions for working in the domain of a curve. 
            Functions: PointOn, FirstDer, SecondDer, ThirdDer,... correct parameter
            when it is outside domain.
      \{ */
  void        PointOn  ( double & t, MbCartPoint & p  ) const override; // \ru Точка на кривой \en The point on the curve 
  void        FirstDer ( double & t, MbVector    & fd ) const override; // \ru Первая производная \en First derivative 
  void        SecondDer( double & t, MbVector    & sd ) const override; // \ru Вторая производная \en Second derivative 
  void        ThirdDer ( double & t, MbVector    & td ) const override; // \ru Третья производная \en Third derivative 
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
  double      Step( double t, double sag ) const override;  // \ru Вычисление шага аппроксимации с учетом радиуса кривизны \en Calculation of the approximation step with consideration of the curvature radius 
  double      DeviationStep( double t, double angle ) const override; // \ru Вычисление шага аппроксимации по угловой толерантности \en Calculation of the approximation step by angular tolerance 

  /** \} */
  /** \ru \name Общие функции кривой
      \en \name Common functions of curve
      \{ */

  MbNurbs *   NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;
  MbCurve *   NurbsCurve( const MbNurbsParameters & ) const override; // \ru Построить NURBS-копию кривой \en Create a NURBS-copy of the curve 

  // \ru Построить усеченную кривую c учетом заданной двумерной точности. \en Construct a trimmed curve with the given two-dimensional accuracy.  
  MbCurve *   Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyEps, bool saveParamLenAndLaw ) const override;
  using MbCurve::Trimmed; // \ru Метод базового класса MbCurve без учета двумерной точности. Точность по умолчанию - MbDimAccuracy::twoDimAcc.  \en Method of MbCurve base class without two-dimensional accuracy. The default accuracy is MbDimAccuracy::twoDimAcc.

  MbContour * NurbsContour() const override;

  void        Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменение направления кривой на противоположное \en Change to the opposite direction of a curve

  void        GetProperties( MbProperties & properties ) override;  // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & properties ) override;  // \ru Записать свойства объекта \en Set properties of the object 
  /// \ru Получить границы участков кривой, которые описываются одной аналитической функцией. \en Get the boundaries of the curve sections that are described by one analytical function. \~
  void        GetAnalyticalFunctionsBounds( std::vector<double> & params ) const override;


  /** \} */
  /** \ru \name Общие функции полигональной кривой
      \en \name Common functions of a polygonal curve
      \{ */
  size_t      GetPointsCount() const override; ///< \ru Вернуть количество несовпадающих контрольных точек. \en Return the number of non-coincedent control points.
  void        GetPoint( ptrdiff_t index, MbCartPoint & pnt ) const override; // \ru Выдать точку \en Get point

  ptrdiff_t   GetNearPointIndex( const MbCartPoint & pnt ) const override; ///< \ru Выдать индекс точки, ближайшей к заданной. \en Get the point index which is nearest to the given.

  void        GetRuleInterval( ptrdiff_t index, double & t1, double & t2 ) const override; // \ru Выдать интервал влияния точки \en Get the interval of point influence 

  void        Rebuild() override;      // \ru Пересчитать Безье кривую \en Recalculate Bezier curve 
  void        SetClosed( bool cls ) override;

  // \ru BEG: для библиотеки (хорошо бы избавиться) \en BEG: for the library (it would be good to get rid of this) 
  void        LtSetClosed( bool cls );  // \ru Установить признак замкнутости. \en Set the closedness attribute. 
  // \ru END: для библиотеки (хорошо бы избавиться) \en END: for the library (it would be good to get rid of this) 

  void        RemovePoint( ptrdiff_t index ) override; // \ru Удалить точку \en Remove the point. 
  void        RemovePoints() override;                 // \ru Удалить все точки \en Remove all points

  void        AddPoint( const MbCartPoint & pnt ) override;               // \ru Добавить точку в конец массива \en Add a point to the end of array
  void        InsertPoint( ptrdiff_t index, const MbCartPoint & pnt ) override; // \ru Вставить точку по индексу \en Insert a point by index
  void        InsertPoint( double t,  const MbCartPoint & pnt, double xEps, double yEps ) override; // \ru Вставить точку по индексу \en Insert a point by index

  /** \brief \ru Вставить полюс, заданный тремя точками, по индексу. Без пересчета производных в полюсах.
             \en Insert a pole, specified by three pointss, by index. No recalculation of derivatives at the poles. \~
    \details \ru Точка #basePnt задает характерную точку.
                 Вектор из точки #basePnt в точку слева #leftPnt задает производную слева от характерной точки,
                 вектор из точки #basePnt в точку справа #rightPnt задает производную справа от характерной точки.
                 Для сохранения непрерывности производной по направлению три заданных точки должны лежать на одной прямой.
                 Для сохранения непрерывности производной по величине расстояния от характерной точки до точек справа и слева должны быть равны.
             \en #basePnt is the characteristic point.
                 The vector from #basePnt to the left point #leftPnt specifies the derivative to the left of the characteristic point,
                 the vector from #basePnt to the right point #rightPnt specifies the derivative to the right of the characteristic point.
                 To maintain the continuity of the directional derivative, the three specified points must lie on one straight line.
                 To maintain the continuity of the derivative with respect to the magnitude, the distances from the characteristic point to the points on the right and left must be equal. \~
    \param[in] index - \ru Индекс полюса.
                       \en Pole index. \~
    \param[in] leftPnt - \ru Точка слева от характерной точки.
                         \en Point to the left of the characteristic point. \~
    \param[in] basePnt - \ru Характерная точка.
                         \en Characteristic point \~
    \param[in] rightPnt- \ru Точка справа от характерной точки.
                         \en Point to the right of the characteristic point. \~
  */
  void        InsertPolePoints( size_t index, const MbCartPoint & leftPnt, const MbCartPoint & basePnt, const MbCartPoint & rightPnt );

  /** \brief \ru Заменить полюс.
             \en Replace the pole. \~
    \details \ru Заменяет характерную точку с указанным индексом.
             \en Replaces characteristic point with a given index. \~
    \param[in] index - \ru Индекс изменяемой точки.
                       \en An index of changed point. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
  */
  void        ChangePole ( ptrdiff_t index, const MbCartPoint & pnt ) override;
  void        ChangePoint( ptrdiff_t index, const MbCartPoint & pnt ) override; // \ru Заменить точку или производную \en Replace a point or derivative 
  bool        CheckParam ( double & t, ptrdiff_t & i0, ptrdiff_t & i1, double & t0, double & t1 ) const override; // \ru Загнать параметр получить локальный индексы и параметры \en Move parameter, get local indices and parameters
  double      GetParam( ptrdiff_t i ) const override;
  size_t      GetParamsCount() const override;
  void        GetTList( SArray<double> & params ) const override;

  // \ru Функции только 2D кривой \en Functions for 2D-curve 

  void        CalculateGabarit ( MbRect & ) const override; // \ru Определить габариты кривой \en Determine the bounding box of the curve 

  MbeState    DeletePart( double t1, double t2, MbCurve *&part2 ) override; // \ru Удалить часть поликривой между параметрами t1 и t2 \en Remove a part of the polyline between t1 and t2 parameters 
  MbeState    TrimmPart( double t1, double t2, MbCurve *&part2 ) override; // \ru Оставить часть поликривой между параметрами t1 и t2 \en Save a part of the polyline between t1 and t2 parameters

  MbeState    Deformation( const MbRect &, const MbMatrix & ) override;  // \ru Деформация \en Deformation 
  bool        IsInRectForDeform( const MbRect & r ) const override;      // \ru Виден ли объект в заданном прямоугольнике для деформации \en Whether the object is visible in the specified rectangle for the deformation 
  void        TangentPoint( const MbCartPoint & pnt, SArray<double> & tFind ) const override; // \ru Вычисление всех касательных к кривой из данной точки \en Calculation of all tangents to the curve from a given point 

  /** \brief \ru Добавить точку и производные в конец массива.
             \en Add point and derivatives to the end of array. \~
    \details \ru Добавить полюс в конец кривой и перемещением соседних точек (не полюсов), обеспечить 
      направление касательной к кривой в точке полюса и длины производных в полюсе.
             \en Add pole to the end of curve, and the movement of neighboring points (no poles), to ensure 
      the direction of the tangent to the curve at the pole and derivatives lengths in the pole. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
    \param[in] dl - \ru Длина производной в полюсе слева.
                    \en Derivative length in pole on the left. \~
    \param[in] dr - \ru Длина производной в полюсе справа.
                    \en Derivative length in pole on the right. \~
    \param[in] angle - \ru Угол между направлением касательной и осью OX текущей системы координат.
                       \en The angle between the direction of the tangent and the OX-axis of the current coordinate system. \~
  */
  void        AddPoint( MbCartPoint & pnt, double dl, double dr, double angle );

  /** \brief \ru Определить выпуклую оболочку сегмента кривой.
             \en Determine the convex hull of the curve segment. \~
    \details \ru Определить выпуклую оболочку сегмента кривой.\n
             \en Determine the convex hull of the curve segment.\n \~
    \param[in] seg - \ru Номер сегмента кривой.
                     \en A number of curve segment. \~
    \param[out] poly - \ru Массив точек, составляющих выпуклую оболочку сегмента.
                       \en Array of points which constitute the convex hull of the segment. \~
  */
  void        ConvexHull( ptrdiff_t seg, SArray<MbCartPoint> & poly ) const;
  /// \ru Определение особых точек офсетной кривой \en Determination of singular points of the offset curve 
  void        OffsetCuspPoint( SArray<double> & tCusps, double dist ) const override;
  /// \ru Вернуть массив отдельных сегментов Bezier-кривой. \en Return an array of separate segments of the Bezier-curve. 
  void        GetSegments( RPArray<MbBezier> & segments ) const;
  /// \ru Удалить совпадающие точки. \en Delete coincident points. 
  void        ExeptEqualPoints(); 

  bool        IsDegenerate( double eps = Math::LengthEps ) const override;  // \ru Проверка вырожденности кривой \en Check for curve degeneracy 
  /// \ru Сделать контур из NURBS-кривой. \en Create a contour from the NURBS curve. 
  MbContour * CreateContour() const; 

  // \ru Функции только Bezier кривой \en Functions for Bezier curve           

  /** \brief \ru Выделить часть кривой Безье.
             \en Allocate a part of the Bezier curve. \~
    \details \ru Создается новая кривая - часть кривой Безье между параметрами t1 и t2.
             \en The new curve is created - a part of the Bezier curve between t1 and t2 parameters. \~
    \param[out] trimPart - \ru Созданная кривая.
                           \en Created curve. \~
    \param[in] t1    - \ru Параметр начала выделенной части.
                       \en A beginning parameter. \~
    \param[in] t2    - \ru Параметр конца выделенной части.
                       \en An end parameter. \~
    \param[in] xyEps - \ru Компонента xyEps.x соответствует точности по оси x, xyEps.y - по оси y.
                       \en The xyEps.x component corresponds to x-axis accuracy, xyEps.y corresponds to y-axis accuracy. \~
    \result \ru true - если построение прошло успешно.
            \en True - if construction has been successfully. \~ 
  */
  bool        Break( MbBezier & trimPart, double t1, double t2, const MbDimAccuracy & xyEps = MbDimAccuracy::twoDimAcc ) const; // \ru Выделить часть \en Break a part 
  void        SetBezierSplines();  ///< \ru Вычислить параметры кривой-Bezier. \en Calculate parameters of the Bezier curve. 
  int         GetFormType() const { return form; } ///< \ru Вернуть форму сплайна. \en Return the spline shape. 
  void        SetFormType( int newForm );          ///< \ru Установить форму сплайна. \en Set the spline shape. 
  ptrdiff_t   GetSplinesCount() const { return splinesCount; } ///< \ru Количество сплайнов \en The number of splines 

  /** \brief \ru Выделить часть кривой Безье.
             \en Break a part of the Bezier curve. \~
    \details \ru Создается новая кривая - часть кривой Безье между параметрами t1 и t2.
             \en The new curve is created - a part of the Bezier curve between t1 and t2 parameters. \~
    \param[out] trimm - \ru Созданная кривая.
                        \en Created curve. \~
    \param[in] t1 - \ru Параметр начала выделенной части.
                    \en A beginning parameter. \~
    \param[in] t2 - \ru Параметр конца выделенной части.
                    \en An end parameter. \~
    \param[in] sense - \ru Совпадает ли направление полученной кривой с направлением исходной кривой.
                       \en Whether the direction of the resulting curve coincides with the direction of the original curve. \~
  */
  void        Trimm( MbBezier & trimm, double t1, double t2, int sense ) const;

  bool        DistanceToPointIfLess( const MbCartPoint & to, double &d ) const override; // \ru Расстояние до точки, если оно меньше d \en Distance to the point if it is less than d

  /** \brief \ru Вычислить все базовые функции в точке.
             \en Calculate all base functions in the point. \~
    \details \ru При заданном параметре вычисляются все базовые функции сплайна и 
      индекс первой характерной точки, от которой зависит поведение сплайна 
      при заданном параметре.
             \en For a given parameter, are calculated all base functions of the spline and 
      the index of the first characteristic point which determines the behavior of the spline 
      at a given parameter. \~
    \param[in] t - \ru Параметр.
                   \en A parameter. \~
    \param[out] values - \ru Массив со значениями базовых функций в точке.
                         \en An array with values of base functions in the point. \~
    \param[in] left - \ru Индекс характерной точки - начала интервала влияния.
                      \en An index of characteristic point. \~
    \result \ru true - если операция прошла успешно.
            \en True - if operation has been successfully. \~ 
  */
  bool        BasicFunctions( double & t, CcArray<double> & values, ptrdiff_t & left ) const;

  // \ru Посчитать метрическую длину \en Calculate the metric length 
  double      CalculateMetricLength() const override; 
  // \ru Сдвинуть параметр t на расстояние len. \en Move parameter t on the metric distance len. 
  bool        DistanceAlong( double & t, double len, int curveDir, double eps = Math::LengthEps,
                             VERSION version = Math::DefaultMathVersion() ) const override;

  bool        IsContinuousDerivative( bool & contLength, bool & contDirect, c3d::DoubleVector * params = nullptr, double epsilon = EPSILON ) const override; // \ru Непрерывна ли первая производная? \en Have the first derivative the continuous?
  // \ru Устранить разрывы первых производных по длине. \en Eliminate the discontinuities of the first derivative at length.
  bool        SetContinuousDerivativeLength( VERSION version, double epsilon = EPSILON ) override; 

  /** \} */

protected:
  bool        CanChangeClosed() const override; // \ru Можно ли поменять признак замкнутости \en Whether it is possible to change the attribute of closedness
private :
  void        CheckData( double & t ) const;
  void        EvaluateSlope  ( ptrdiff_t index ); // \ru Рассчитать производные в полюсе index \en Calculate derivatives at the pole "index" 
  void        EvaluateSlope0 ( ptrdiff_t index ); // \ru Рассчитать производные в полюсе index для 0-ой формы \en Calculate derivatives at the pole "index" for 0-th form 
  void        EvaluateSlope1 ( ptrdiff_t index ); // \ru Рассчитать производные в полюсе index для 1-ой формы \en Calculate derivatives at the pole "index" for 1-th form 
  void        SetDerives(); // \ru Рассчитать все производные \en Calculate all derivatives 
  void        SetDerives ( ptrdiff_t index ); // \ru Рассчитать производные в полюсах при изменении полюса index. \en Calculate derivatives at poles when changing the pole "index". 

  void        operator = ( const MbBezier & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbBezier )
};

IMPL_PERSISTENT_OPS( MbBezier )


#endif // __CUR_BEZIER_H
