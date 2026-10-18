////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Сплайн Безье в трёхмерном пространстве.
         \en Bezier spline in three-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_BEZIER3D_H
#define __CUR_BEZIER3D_H


#include <cur_polycurve3d.h>


class   MbBezier;
class   MbArc3D;


//------------------------------------------------------------------------------
/** \brief \ru Сплайн Безье в трёхмерном пространстве.
           \en Bezier spline in three-dimensional space. \~
  \details \ru Сплайн Безье в трёхмерном пространстве определяется контрольными точками pointList. \n
    Сплайн Безье является составной кривой, образованной стыкующимися между собой кривыми Безье третьей степени. 
    Каждая кривая Безье третьей степени построена по четырём соседним контрольным точкам множества pointList. 
    Для незамкнутого сплайна Безье первая и последняя контрольная точка множества pointList не используются.
    Таким образом, сплайн Безье проходит через каждую контрольную точку множества pointList с индексом 3n+1, где n - целое число.
    Значение параметра вдоль каждой кривой Безье третьей степени увеличивается на единицу. 
    Параметр сплайна Безье изменяется от нуля до k, где k - количество кривых Безье третьей степени, образующих сплайн. 
    В общем случае первая производная сплайна Безье может быть разрывной как по длине, так и по направлению.
           \en Bezier spline in three-dimensional space is defined by control points pointList. \n
    Bezier spline is composite curve formed by connected among themselves Bezier curves with third degree. 
    Each third-degree Bezier curve is constructed by four adjacent control points pointList. 
    First and last control point from pointList for unclosed Bezier spline are not used
    Thus Bezier spline passes through each control point with index 3n+1, where n is integer.
    The value of parameter along each third-degree Bezier curve is incremented. 
    The parameter of Bezier spline changes from zero to k, where k is the number of  tree-degree Bezier curves which form spline. 
    In general case the first derivative of Bezier spline can be discontinuous by length and direction. \~
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbBezier3D : public MbPolyCurve3D {
private :
  ptrdiff_t splinesCount; ///< \ru Количество кривых Безье третьей степени. \en The number of third-degree Bezier curves. 
  int       form;         ///< \ru Форма кривой. \en Form of curve. 

protected :
  MbBezier3D( const SArray<MbCartPoint3D> & initList, bool cls, int initForm = 0 );
  MbBezier3D( const MbBezier &, const MbPlacement3D & );
  MbBezier3D( const MbCartPoint3D & p1, const MbCartPoint3D & p2 );
  MbBezier3D( const MbBezier3D & ); ///< \ru Конструктор копирования. \en Copy constructor.
public :
  virtual ~MbBezier3D();

public :
  /** \brief \ru Создать копию сплайна.
             \en Create copy of spline. \~
    \details \ru Создать копию сплайна.\n
             \en Create copy of spline.\n \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbBezier3D * Create( const MbBezier3D & other );
  /** \brief \ru Создать сплайн.
             \en Create spline. \~
    \details \ru Создать сплайн и установить параметры сплайна.\n
             \en Create spline and set parameters of spline.\n \~
    \param[in] initList - \ru Набор контрольных точек.
                          \en Set of control points. \~
    \param[in] cls      - \ru Признак замкнутости.
                          \en A closedness attribute. \~
    \param[in] initForm - \ru Форма кривой. 
                          \en Form of curve. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbBezier3D * Create( const SArray<MbCartPoint3D> & initList, bool cls, int initForm = 0 );
  /** \brief \ru Создать сплайн.
             \en Create spline. \~
    \details \ru Создать сплайн и установить параметры сплайна.\n
             \en Create spline and set parameters of spline.\n \~
    \param[in] bezier - \ru Двумерный сплайн.
                        \en The two-dimensional spline. \~ 
    \param[in] place  - \ru Локальная система координат сплайна.
                        \en Local coordinate system of spline. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbBezier3D * Create( const MbBezier & bezier, const MbPlacement3D & place );
  /** \brief \ru Создать сплайн.
             \en Create spline. \~
    \details \ru Создать сплайн и установить параметры сплайна.\n
             \en Create a spline and set parameters of spline.\n \~
    \param[in] p1 - \ru Начальная точка кривой.
                    \en Start point of curve. \~
    \param[in] p2 - \ru Конечная точка кривой.
                    \en End point of curve. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbBezier3D * Create( const MbCartPoint3D & p1, const MbCartPoint3D & p2 );

public :
  VISITING_CLASS( MbBezier3D );
  
  void    Init( const SArray<MbCartPoint3D> & initList, bool cls );
  void    Init( const MbBezier3D & );
  void    Init( const MbBezier &, const MbPlacement3D & );
  void    Init( MbArc3D & );

  // \ru Общие функции математического объекта \en The common functions of the mathematical object 

  MbeSpaceType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override;
  bool    SetEqual ( const MbSpaceItem & ) override;  // \ru Сделать равным \en Make equal 
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;     // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;     // \ru Сдвиг \en Translation
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override;  // \ru Поворот \en Rotation

  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 

  // \ru Общие функции кривой \en Common functions of curve 

  double  GetTMax() const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter 
  double  GetTMin() const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter 
  // \ru Функции для работы внутри области определения кривой. \en Functions for working inside of the curve domain. \~
  void    PointOn  ( double & t, MbCartPoint3D & ) const override; // \ru Точка на кривой \en The point on the curve 
  void    FirstDer ( double & t, MbVector3D & ) const override;  // \ru Первая производная \en First derivative 
  void    SecondDer( double & t, MbVector3D & ) const override;  // \ru Вторая производная \en Second derivative 
  void    ThirdDer ( double & t, MbVector3D & ) const override;  // \ru Третья производная \en Third derivative 
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void    Explore( double & t, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const override;

  double  Step( double t, double sag ) const override; // \ru Вычисление шага аппроксимации \en Calculate step of approximation 
  double  DeviationStep( double t, double angle ) const override;
  void    Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction

  virtual bool        Break( MbBezier3D &, double t1, double t2 ) const; // \ru Разбить на две части \en Split into two parts 

  MbNurbs3D * NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;
  MbCurve3D * NurbsCurve( const MbNurbsParameters & ) const override;
  
  // \ru Построить усеченную кривую c учетом заданной трехмерной точности. \en Construct a trimmed curve with the given three-dimensional accuracy.
  MbCurve3D * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyzEps, bool saveParamLenAndLaw ) const override;
  using MbCurve3D::Trimmed; // \ru Метод базового класса MbCurve3D без учета трехмерной точности. Точность по умолчанию - MbDimAccuracy::threeDimAcc.  \en Method of MbCurve3D base class without three-dimensional accuracy. The default accuracy is MbDimAccuracy::threeDimAcc.

  // \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская (после использования вызывать DeleteItem на двумерную кривую) 
  // \en Give a planar curve and placement, if the spatial curve is planar (after using the DeleteItem must be called for a two-dimensional curve) 
  bool    GetPlaneCurve( MbCurve *& curve2d, MbPlacement3D & place, bool saveParams, PlanarCheckParams params = PlanarCheckParams() ) const override;

  /// \ru Получить границы участков кривой, которые описываются одной аналитической функцией. 
  /// \en Get the boundaries of the curve sections that are described by one analytical function. \~
  void    GetAnalyticalFunctionsBounds( std::vector<double> & params ) const override;

  // \ru Общие функции полигональной кривой \en Common functions of a polygonal curve 

  void    Rebuild() override;                                       // \ru Пересчитать Безье кривую \en Recalculate Bezier curve 
  void    SetClosed  ( bool cls ) override;                         // \ru Установить признак замкнутости \en Set the closedness attribute.
  void    AddPoint   ( const MbCartPoint3D & ) override;            // \ru Добавить точку в конец массива \en Add a point to the end of array 
  void    InsertPoint( ptrdiff_t index, const MbCartPoint3D &         ) override; // \ru Добавить точку \en Add a point
  void    InsertPoint( double t,  const MbCartPoint3D &, double ) override; // \ru Добавить точку \en Add a point
  void    RemovePoint( ptrdiff_t index ) override;                        // \ru Удалить точку \en Remove a point 
  bool    ChangePoint( ptrdiff_t index, const MbCartPoint3D & ) override; // \ru Заменить точку \en Replace a point
  void    ChangePole ( ptrdiff_t index, const MbCartPoint3D &  override); // \ru Заменить полюс \en Replace a pole
  bool    CheckParam ( double & t, ptrdiff_t & i0, ptrdiff_t & i1, double & t0, double & t1 ) const override; // \ru Загнать параметр получить локальный индексы и параметры \en Move parameter, get local indices and parameters 
  double  GetParam( ptrdiff_t i ) const override; // \ru Выдать параметр для точки с номером \en Get a parameter for point with number 
  size_t  GetPointsCount() const override; // \ru Выдать количество точек \en Get the number of points 
  void    GetPoint            ( ptrdiff_t index, MbCartPoint3D & ) const override; // \ru Выдать точку \en Get a point
  ptrdiff_t GetNearPointIndex   ( const MbCartPoint3D & ) const override; // \ru Выдать индекс точки, ближайшей к заданной \en Get the point index which is nearest to the given
  void    GetRuleInterval     ( ptrdiff_t index, double & t1, double & t2 ) const override; // \ru Выдать интервал влияния точки \en Get the interval of point influence

  MbNurbs3D * Trimm( double t1, double t2, int sense ) const;
  void        Trimm( MbBezier3D &, double t1, double t2, int sense ) const;

  void    InitCtrlPoints( const SArray<MbCartPoint3D> & );
  void    SetBezierSplines(); // \ru Вычислить параметры кривой-Bezier \en Calculate parameters of the Bezier curve 
  int     GetFormType() const { return form; } // \ru Форма сплайна \en The spline form 
  void    SetFormType( int newForm );
  ptrdiff_t GetSplinesCount() const { return splinesCount; } // \ru Количество сплайнов \en The number of splines 

  // \ru Функции только 3D кривой \en Function for 3D-curve 
  MbCurve * GetMap( const MbMatrix3D &, MbRect1D * pRgn = nullptr,
                    VERSION version = Math::DefaultMathVersion(), bool * coincParams = nullptr ) const override; // \ru Дать плоскую проекцию кривой \en Get a planar projection of curve

  size_t  GetCount() const override;

  // \ru Посчитать метрическую длину \en Calculate the metric length 
  double  CalculateMetricLength() const override;
  // \ru Сдвинуть параметр t на расстояние len. \en Move parameter t on the metric distance len. 
  bool    DistanceAlong( double & t, double len, int curveDir, double eps = Math::metricPrecision,
                         VERSION version = Math::DefaultMathVersion() ) const override; 

  bool    IsContinuousDerivative( bool & contLength, bool & contDirect, c3d::DoubleVector * params = nullptr, double epsilon = EPSILON ) const override; // \ru Непрерывна ли первая производная? \en Have the first derivative the continuous?
  // \ru Устранить разрывы первых производных по длине. \en Eliminate the discontinuities of the first derivative at length.
  bool    SetContinuousDerivativeLength( VERSION version, double epsilon = EPSILON ) override;

private :
  void    CheckBezierClosed();               // \ru Проверка признака замкнутости. \en Check closed.
  void    EvaluateSlope ( ptrdiff_t index ); // \ru Рассчитать производные в полюсе index \en Calculate derivatives at the pole "index" 
  void    EvaluateSlope0( ptrdiff_t index ); // \ru Рассчитать производные в полюсе index для 0-ой формы \en Calculate derivatives at the pole "index" for 0-th form 
  void    EvaluateSlope1( ptrdiff_t index ); // \ru Рассчитать производные в полюсе index для 1-ой формы \en Calculate derivatives at the pole "index" for 1-th form 
  void    SetDerives    ();            // \ru Рассчитать все производные \en Calculate all derivatives 
  void    SetDerives    ( ptrdiff_t index ); // \ru Рассчитать производные в полюсах при изменении полюса index \en Calculate derivatives at poles when changing the pole "index" 

private:
  void    operator = ( const MbBezier3D & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbBezier3D )
};

IMPL_PERSISTENT_OPS( MbBezier3D )


#endif // __CUR_BEZIER3D_H
