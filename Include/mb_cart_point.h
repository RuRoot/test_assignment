////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Декартова двумерная точка.
         \en The cartesian two-dimensional point. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_CART_POINT_H
#define __MB_CART_POINT_H


#include <mb_vector.h>


class  MbFloatPoint;
class  MbCartPoint;


namespace c3d // namespace C3D
{
typedef std::vector<MbCartPoint>            ParamPointsVector;  ///< \ru Вектор точек. \en Points vector.
typedef std::vector<MbVector>               ParamVectorsVector; ///< \ru Вектор векторов. \en Vectors vector.

typedef std::pair<MbCartPoint, size_t>      ParamPointIndex;    ///< \ru Пара точка-индекс. \en Pair point-index.
typedef std::pair<MbCartPoint, size_t>      Point2DIndex;       ///< \ru Пара точка-индекс. \en Pair point-index.

typedef std::pair<size_t, MbCartPoint>      IndexParamPoint;    ///< \ru Пара индекс-точка. \en Pair index-point.
typedef std::pair<size_t, MbCartPoint>      IndexPoint2D;       ///< \ru Пара индекс-точка. \en Pair index-point.

typedef std::pair<MbCartPoint, MbCartPoint> ParamPointsPair;    ///< \ru Пара точка-точка. \en Pair point-point.

typedef std::pair<MbCartPoint, MbVector>    ParamPointVector;   ///< \ru Пара точка-вектор. \en Pair point-vector.
typedef std::pair<MbCartPoint, MbVector>    PointVector2D;      ///< \ru Пара точка-вектор. \en Pair point-vector.
}


//------------------------------------------------------------------------------
/** \brief \ru Декартова двумерная точка.
           \en The cartesian two-dimensional point. \~
  \details \ru Двумерная точка описывается двумя координатами. Определены различные
    логические, арифметические и геометрические операции точек с точками и векторами.
    Точка названа в честь французского учёного геометра Рене Декарта (Rene Descartes, по латыни Renatus Cartesius). \n
           \en The two-dimensional point is defined by two coordinates. Different
    logical, arithmetical and geometrical operations of points with points and vectors are defined.
    The point is named in honor of French scientist Rene Descartes (lat. Renatus Cartesius). \n \~
  \ingroup Mathematic_Base_2D
*/
// ---
class MATH_CLASS MbCartPoint 
{
public :
  double x; ///< \ru Первая координата точки. \en A first coordinate of point. 
  double y; ///< \ru Вторая координата точки. \en A second coordinate of point. 

  /// \ru Начало координат или { 0, 0 }. \en The origin or { 0, 0 }. 
  static const MbCartPoint origin;  

  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
    \details \ru Создает точку с нулевыми координатами.
             \en Creates a point with zero coordinates. \~
  */
  MbCartPoint () : x(0), y(0) {}

  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbCartPoint ( const MbCartPoint & p ) : x( p.x ), y( p.y ) {}
  /// \ru Конструктор по вектору. \en Constructor by a vector. 
  MbCartPoint ( const MbVector & p ) : x( p.x ), y( p.y ) {}
  /// \ru Конструктор по координатам. \en Constructor by coordinates. 
  MbCartPoint ( double initX, double initY ) : x(initX), y(initY) {}

  /** \brief \ru Конструктор по точке в локальной системе координат.
             \en Constructor by a point in a local coordinate system. \~
    \details \ru Для перевода точки в глобальную систему координат задаётся матрица перехода 
      из локальной системы.
             \en A transition matrix is given for transforming of a point to the global coordinate system 
      from a local coordinate system. \~
    \param[in] p - \ru Точка в локальной системе координат.
                   \en A point in the local coordinate system. \~
    \param[in] matr - \ru Матрица перехода из локальной системы координат в глобальную.
                      \en A transition matrix from the local coordinate system to the global coordinate system. \~
  */
  MbCartPoint ( const MbCartPoint & p, const MbMatrix & matr ) : x(p.x), y(p.y) { Transform(matr); }

  /// \ru Конструктор по float-точке. \en Constructor by float point. 
  MbCartPoint ( const MbFloatPoint & );

  /// \ru Обнулить координаты. \en Set coordinates to zero. 
  void        SetZero() { x = y = 0; }
  /// \ru Инициализировать по другой точке. \en Initialize by another point. 
  template <class Point>
  MbCartPoint & Init( const Point & p ) { x = p.x; y = p.y; return *this; }
  /// \ru Инициализировать по координатам. \en Initialize by coordinates. 
  MbCartPoint & Init( double initX, double initY ) { x = initX; y = initY; return *this; }

  /// \ru Найти расстояние от точки до точки. \en Find the distance between points. 
  double      DistanceToPoint( const MbCartPoint & to ) const;

  /// \ru Преобразовать согласно матрице. \en Transform according to the matrix. 
  MbCartPoint & Transform( const MbMatrix & matr );
  /// \ru Сдвинуть вдоль вектора. \en Translate along a vector. 
  MbCartPoint & Move( const MbVector & to ) { x += to.x; y += to.y; return *this; }
  /// \ru Сдвинуть на заданные приращения. \en Translate by given increments. 
  MbCartPoint & Move( double dx, double dy ) { x += dx; y += dy; return *this; }

  /// \ru Найти квадрат расстояния от точки до точки. \en Find the squared distance between points. 
  double      DistanceToPoint2( const MbCartPoint & to ) const;

  /** 
    \ru \name Сдвиги, вращения, отражения.
    \en \name Translations, rotations, reflections.
    \{ */
 
  /** \brief \ru Сдвинуть по направлению, заданному второй точкой.
             \en Translate along the direction which is defined by a second point. \~
    \param[in] p2 - \ru Исходная точка, определяющая направления сдвига.
                    \en An initial point, which defines the direction of the translation. \~
    \param[in] delta - \ru Величина сдвига.
                       \en Magnitude of the translation. \~
  */
  void        MoveAlongLine( const MbCartPoint & p2, double delta );
 
  /** \brief \ru Сдвинуть по направлению, заданному углом.
             \en Translate along the direction which is defined by an angle. \~
    \param[in] angle - \ru Угол, определяющий направление сдвига.
                       \en An angle which defines the direction of the translation. \~
    \param[in] delta - \ru Величина сдвига.
                       \en Magnitude of the translation. \~
  */
  void        MoveAlongLine( double angle, double delta );
 
  /** \brief \ru Сдвинуть по направлению, заданному вектором.
             \en Translate along the direction which is defined by a vector. \~
    \param[in] angle - \ru Вектор, определяющий направление сдвига.
                       \en A vector which defines the direction of the translation. \~
    \param[in] delta - \ru Величина сдвига.
                       \en Magnitude of the translation. \~
  */
  void        MoveAlongLine( const MbDirection & angle, double delta );
 
  /** \brief \ru Сдвинуть по направлению, заданному вектором.
             \en Translate along the direction which is defined by a vector. \~
    \param[in] angle - \ru Вектор, определяющий направление сдвига.
                       \en A vector which defines the direction of the translation. \~
    \param[in] delta - \ru Величина сдвига.
                       \en Magnitude of the translation. \~
  */
  void        MoveAlongLine( const MbVector & vect, double delta );
            
  /** \brief \ru Сдвинуть точку по направлению к заданной на расстояние dist.
             \en Translate the point towards a given point to the distance "dist". \~
    \param[in] p1 - \ru Исходная точка, определяющая направления сдвига.
                    \en An initial point, which defines the direction of the translation. \~
    \param[in] dist - \ru Величина сдвига.
                      \en Magnitude of the translation. \~
  */
  void        MoveUntilDist( const MbCartPoint & p1, double dist );
  
  /** \brief \ru Сдвинуть по направлению.
             \en Translate along the direction. \~
    \details \ru Сдвиг по направлению точки to на расстояние, определяемое 
      отношением величины ratio к расстоянию между точками.
             \en Translation along the direction of  the "to" point to the distance which is defined 
      by the ratio of "ratio" and distance between points. \~
    \param[in] to - \ru Точка.
                    \en A point. \~
    \param[in] ratio - \ru Доля расстояния.
                       \en A distance part. \~
  */
  void        GoNearToPoint( const MbCartPoint & to, double ratio ); // \ru Сдвиг в направлении точки to \en Translation along the direction of point "to" 

  /** \brief \ru Повернуть на угол.
             \en Rotate by an angle. \~
    \details \ru Угол определяет вектор вращения, а точка - центр.
             \en An angle defines a rotation vector and a point defines a center. \~
    \param[in] pnt - \ru Точка.
                     \en A point. \~
    \param[in] angle - \ru Угол вращения.
                       \en A rotation angle. \~
  */
  void        Rotate( const MbCartPoint & pnt, double angle );

  /** \brief \ru Повернуть на угол.
             \en Rotate by an angle. \~
    \details \ru Угол определяется вектором вращения.
             \en An angle is defined by a rotation vector. \~
    \param[in] pnt - \ru Точка - центр вращения.
                     \en A point is a rotation center. \~
    \param[in] angle - \ru Вектор вращения.
                       \en A rotation vector. \~
  */
  void        Rotate( const MbCartPoint & pnt, const MbDirection & angle );

  /** \brief \ru Повернуть на угол.
             \en Rotate by an angle. \~
    \details \ru Угол определяет вектор вращения.
             \en An angle defines a rotation vector. \~
    \param[in] angle - \ru Угол вращения.
                       \en A rotation angle. \~
  */
  void        Rotate( double angle );

  /** \brief \ru Повернуть на угол.
             \en Rotate by an angle. \~
    \details \ru Угол определяется вектором вращения.
             \en An angle is defined by a rotation vector. \~
    \param[in] angle - \ru Вектор вращения.
                       \en A rotation vector. \~
  */
  void        Rotate( const MbDirection & angle );
         
  /** \brief \ru Зеркально отразить точку от заданной.
             \en Reflect a point specularly from a given point. \~
    \param[in] from - \ru Точка, от которой требуется отразить исходную.
                      \en A point from which it is necessary to reflect the initial point. \~
    \return \ru Отраженную точку.
            \en Returns a reflected point. \~
  */
  MbCartPoint Mirror( const MbCartPoint & from ) const;
 
  /** \brief \ru Зеркально отразить точку от прямой.
             \en Reflect a point specularly from a line. \~
    \param[in] p1, p2 - \ru Точки, задающие прямую.
                        \en Points which define a line. \~
    \return \ru Отраженную точку.
            \en Returns a reflected point. \~
  */
  MbCartPoint Mirror( const MbCartPoint & p1, const MbCartPoint & p2  ) const;

  /** \brief \ru Зеркально отразить точку от прямой.
             \en Reflect a point specularly from a line. \~
    \details \ru Отражение происходит от прямой в направлении, задаваемом базовой точкой.
             \en The reflection from a line in the direction which is defined by the base point. \~
    \param[in] p1 - \ru Точка, задающая направление отражения.
                    \en A point which defines the direction of the reflection. \~
    \param[in] dir - \ru Вектор, задающий прямую, от которой делается отражение.
                     \en A vector defines a line from which the reflection is made. \~
  */
  void        Mirror( const MbCartPoint & p1, const MbDirection & dir );
  /** \} */

  /// \ru Количество координат точки. \en The number of point coordinates. 
  static size_t GetDimension() { return 2; }
  /** 
    \ru \name Логические и арифметические операции.
    \en \name Logical and arithmetical operations.
    \{ */
  /// \ru Доступ к координате по индексу. \en Access to a coordinate by an index. 
  double &    operator [] ( size_t i )       { return i ? y : x; }
  /// \ru Значение координаты по индексу. \en The value of a coordinate by an index. 
  double      operator [] ( size_t i ) const { return i ? y : x; }

  /// \ru Проверить на равенство в рамках точности. \en Check for equality with tolerance. 
  bool        operator == ( const MbCartPoint & with ) const;
  /// \ru Проверить на неравенство. \en Check for inequality. 
  bool        operator != ( const MbCartPoint & with ) const;
  /// \ru Проверить на точное равенство. \en Check for an accurate equality. 
  bool        Equal( const MbCartPoint & with )        const;

  /// \ru Проверить на меньше. \en Check for "less". 
  bool        operator < ( const MbCartPoint & ) const;
  /// \ru Проверить на больше. \en Check for "greater".  
  bool        operator > ( const MbCartPoint & ) const;

  /// \ru Сложить две точки. \en Sum two points. 
  MbVector    operator + ( const MbCartPoint & pnt ) const;
  /// \ru Вычесть из точки точку. \en Subtract a point from the point. 
  MbVector    operator - ( const MbCartPoint & pnt ) const;
  /// \ru Сложить точку с вектором. \en Add a point to a vector. 
  MbCartPoint operator + ( const MbDirection & d )   const;
  /// \ru Вычесть вектор из точки. \en Subtract a vector from a point. 
  MbCartPoint operator - ( const MbDirection & d )   const;

  /// \ru Унарный минус. \en The unary minus. 
  MbCartPoint operator - () const;
  /// \ru Умножить точку на число. \en Multiply a point by a number. 
  MbCartPoint operator * ( double factor ) const;
  /// \ru Разделить точку на число. \en Divide a point by a number. 
  MbCartPoint operator / ( double factor ) const;
  /// \ru Вычислить точку как копию данной точки, преобразованную матрицей. \en Calculate the point as this copy transformed by the matrix. 
  MbCartPoint operator * ( const MbMatrix & ) const;

  /// \ru Сложить две точки. \en Sum two points. 
  void        operator += ( const MbCartPoint & pnt );
  /// \ru Вычесть из точки точку. \en Subtract a point from the point. 
  void        operator -= ( const MbCartPoint & pnt );
  /// \ru Умножить точку на число. \en Multiply a point by a number. 
  void        operator *= ( double factor );
  /// \ru Разделить точку на число. \en Divide a point by a number. 
  void        operator /= ( double factor );

  // \ru Тела функций находится в файле "vector.h" \en Implementations of functions are located in the "vector.h" file 
  /// \ru Сложить точку с вектором. \en Add a point to a vector. 
  void        operator += ( MbVector & v );
  /// \ru Вычесть вектор из точки. \en Subtract a vector from a point. 
  void        operator -= ( MbVector & v );

  /// \ru Сложить точку с вектором. \en Add a point to a vector. 
  MbCartPoint operator + ( const MbVector & vector ) const;
  /// \ru Вычесть вектор из точки. \en Subtract a vector from a point. 
  MbCartPoint operator - ( const MbVector & vector ) const;
  /// \ru Присвоить точке значения другой точки. \en Assign the value of another point to the point. 
  MbCartPoint & operator = ( const MbCartPoint & );
  /// \ru Присвоить точке значения компонент вектора. \en Assign values of vector components to the point. 
  void        operator = ( const MbVector     & );
  /// \ru Присвоить точке значения компонент вектора. \en Assign values of vector components to the point. 
  void        operator = ( const MbDirection  & );
  /// \ru Присвоить точке значений float-точки. \en Assign the float-point value to the point. 
  void        operator = ( const MbFloatPoint & );
  /** \} */
  
  /** \brief \ru Расстояние от точки до отрезка.
             \en The distance from a point to a segment. \~
    \details \ru Отрезок задается по двум входным точкам.
             \en A segment is defined by two input points, \~
    \param[in] p1, p2 - \ru Начальная и конечная точки отрезка.
                        \en Start and end points of a segment. \~
    \return \ru Искомое расстояние.
            \en The required distance. \~
  */
  double      DistanceToLineSeg( const MbCartPoint & p1, const MbCartPoint & p2, double eps = Math::LengthEps ) const;
  /// \ru Длина вектора ( 0, p(x,y) ). \en The vector length ( 0, p(x,y) ). 
  double      Length() const;

  /** \brief \ru Масштабировать координаты.
             \en Scale the coordinates. \~
    \param[in] sx, sy - \ru Масштабирующие коэффициенты для компонент x и y соответственно.
                        \en Scaling coefficients for components x & y respectively. \~
  */
  void        Scale( double sx, double sy ) { x *= sx, y *= sy; }

  /** 
    \ru \name Функции сложения и умножения точек с точками и с векторами.
    \en \name The functions of addition and multiplication of points with points and vectors.
    \{ */
  // \ru Присвоение значений \en Values assignment 

  /** \brief \ru Приравнять координаты сумме координат точки и вектора.
             \en Equate coordinates to sum of point coordinates and vector coordinates. \~
    \details \ru Приравнять координаты сумме координат точки v1 и вектора v2, умноженного на число t2.
             \en Equate coordinates to sum of v1 point coordinates and v2 vector coordinates multiplied by t2. \~
    \param[in] v1 - \ru Исходная точка.
                    \en The initial point. \~
    \param[in] v2 - \ru Исходный вектор.
                    \en The initial vector. \~
    \param[in] t2 - \ru Число, на которое умножаются координаты исходного вектора v2.
                    \en Coordinates of the initial vector v2 are multiplied by this number. \~
  */
  void        Set( const MbCartPoint & v1, const MbVector & v2, double t2 );

  /** \brief \ru Приравнять координаты сумме координат точки и двух векторов.
             \en Equate coordinates to sum of point coordinates and two vectors coordinates. \~
    \details \ru Приравнять координаты сумме координат точки v1 и векторов v2 и v3, 
      умноженных на числа t2 и t3, соответственно.
             \en Equate coordinates to sum of v1 point coordinates and v2 and v3 vectors coordinates 
      multiplied by the numbers t2 and t3 respectively. \~
    \param[in] v1 - \ru Исходная точка.
                    \en The initial point. \~
    \param[in] v2, v3 - \ru Исходные векторы.
                        \en The initial vectors. \~
    \param[in] t2, t3 - \ru Числа, на которые умножаются координаты векторов v2 и v3 соответственно.
                        \en Coordinates of v2 and v3 vectors are multiplied by these numbers respectively. \~
  */
  void        Set( const MbCartPoint & v1, const MbVector & v2, double t2, 
                   const MbVector & v3, double t3 );

  /** \brief \ru Приравнять координаты сумме координат двух точек.
             \en Equate coordinates to sum of two points coordinates. \~
    \details \ru Приравнять координаты сумме координат точек v1 и v2, умноженных на числа t1 и t2, соответственно.
             \en Equate coordinates to sum of v1 and v2 points coordinates multiplied by the numbers t1 and t2 respectively. \~
    \param[in] v1, v2 - \ru Исходные точки.
                        \en Initial points. \~
    \param[in] t1, t2 - \ru Числа, на которые умножаются координаты точек v1 и v2 соответственно.
                        \en Coordinates of v1 and v2 points are multiplied by these numbers respectively. \~
  */
  void        Set( const MbCartPoint & v1, double t1, const MbCartPoint & v2, double t2 );
  /**
    \brief \ru Приравнять координаты сумме координат трех точек.
           \en Equate coordinates to sum of three points coordinates. \~
    \details \ru Приравнять координаты сумме координат точек v1, v2 и v3, 
      умноженных на числа t1, t2 и t3, соответственно.
             \en Equate coordinates to sum of v1, v2 and v3 points coordinates 
      multiplied by the numbers t1, t2 and t3 respectively. \~
    \param[in] v1, v2, v3 - \ru Исходные точки.
                            \en Initial points. \~
    \param[in] t1, t2, t3 - \ru Числа, на которые умножаются координаты точек v1, v2 и v3 соответственно.
                            \en Coordinates of v1, v2 and v3 points are multiplied by these numbers respectively. \~
  */
  void        Set( const MbCartPoint & v1, double t1, const MbCartPoint & v2, double t2, 
                   const MbCartPoint & v3, double t3 );

  /** \brief \ru Приравнять координаты сумме координат четырех точек.
             \en Equate coordinates to sum of four points coordinates. \~
    \details \ru Приравнять координаты сумме координат точек v1, v2, v3 и v4, 
      умноженных на числа t1, t2, t3 и t4, соответственно.
             \en Equate coordinates to sum of  v1, v2, v3 and v4 points coordinates 
      multiplied by the numbers t1, t2, t3 and t4 respectively. \~
    \param[in] v1, v2, v3, v4 - \ru Исходные точки.
                                \en Initial points. \~
    \param[in] t1, t2, t3, t4 - \ru Числа, на которые умножаются координаты точек v1, v2, v3 и v4 соответственно.
                                \en Coordinates of v1, v2, v3 and v4 points are multiplied by these numbers respectively. \~
  */
  void        Set( const MbCartPoint & v1, double t1, const MbCartPoint & v2, double t2, 
                   const MbCartPoint & v3, double t3, const MbCartPoint & v4, double t4 );

  /** \brief \ru Приравнять координаты сумме координат точки и двух векторов.
             \en Equate coordinates to sum of point coordinates and two vectors coordinates. \~
    \details \ru Приравнять координаты сумме координат точки v1 и векторов v2 и v3, 
      умноженных на числа t2 и t3, соответственно.
             \en Equate coordinates to sum of v1 point coordinates and v2 and v3 vectors coordinates 
      multiplied by the numbers t2 and t3 respectively. \~
    \param[in] v1 - \ru Исходная точка.
                    \en The initial point. \~
    \param[in] v2, v3 - \ru Исходные векторы.
                        \en The initial vectors. \~
    \param[in] t2, t3 - \ru Числа, на которые умножаются координаты векторов v2 и v3 соответственно.
                        \en Coordinates of v2 and v3 vectors are multiplied by these numbers respectively. \~
  */
  void        Set( const MbCartPoint & v1, const MbDirection & v2, double t2, 
                   const MbDirection & v3, double t3 );

  // \ru Добавление значений \en Values addition      
  /** \brief \ru Увеличить координаты на значения компонент вектора.
             \en Increase coordinates by values of vector components. \~
    \details \ru Увеличить координаты на значения компонент вектора v1, умноженных на число t1.
             \en Increase coordinates by values of v1 vector components multiplied by t1. \~
    \param[in] v1 - \ru Исходный вектор.
                    \en The initial vector. \~
    \param[in] t1 - \ru Число, на которое умножаются координаты исходного вектора v1.
                    \en Coordinates of the initial vector v1 are multiplied by this number. \~
  */
  void        Add( const MbVector & v1, double t1 );

  /** \brief \ru Увеличить координаты на значения компонент двух векторов.
             \en Increase coordinates by values of two vectors components. \~
    \details \ru Увеличить координаты на значения компонент векторов v1 и v2, 
      умноженных на числа t1 и t2, соответственно.
             \en Increase coordinates by values of v1 and v2 vectors components 
      multiplied by the numbers t1 and t2  respectively. \~
    \param[in] v1, v2 - \ru Исходные векторы.
                        \en The initial vectors. \~
    \param[in] t1, t2 - \ru Числа, на которые умножаются координаты векторов v1 и v2 соответственно.
                        \en Coordinates of v1 and v2 vectors are multiplied by these numbers respectively. \~
  */
  void        Add( const MbVector & v1, double t1, const MbVector & v2, double t2 );

  /** \brief \ru Увеличить координаты на значения компонент векторов v1, v2 и v3, умноженных на числа t1, t2 и t3, соответственно.
             \en Increase coordinates by values of v1, v2 and v3 vectors components multiplied by t1, t2 and t3 respectively. \~
    \param[in] v1, v2, v3 - \ru Исходные векторы.
                            \en The initial vectors. \~
    \param[in] t1, t2, t3 - \ru Числа, на которые умножаются координаты векторов v1, v2 и v3 соответственно.
                            \en Coordinates of v1, v2 and v3 vectors are multiplied by these numbers respectively. \~
  */
  void        Add( const MbVector & v1, double t1, const MbVector & v2, double t2, 
                   const MbVector & v3, double t3 );

  /** \brief \ru Увеличить координаты на значения компонент трех векторов.
             \en Increase coordinates by values of three vectors components. \~
    \details \ru Увеличить координаты на значения компонент векторов v1, v2 и v3, 
      умноженных на числа t1, t2 и t3, соответственно.
             \en Increase coordinates by values of v1, v2 and v3 vectors components 
      multiplied by the numbers t1, t2 and t3 respectively. \~
    \param[in] v1, v2, v3 - \ru Исходные векторы.
                            \en The initial vectors. \~
    \param[in] t1, t2, t3 - \ru Числа, на которые умножаются координаты векторов v1, v2 и v3 соответственно.
                            \en Coordinates of v1, v2 and v3 vectors are multiplied by these numbers respectively. \~
  */
  void        Add( const MbVector & v1, double t1, const MbVector & v2, double t2, 
                   const MbVector & v3, double t3, const MbVector & v4, double t4 );

  /** \brief \ru Увеличить координаты на значения компонент двух векторов.
             \en Increase coordinates by values of two vectors components. \~
    \details \ru Увеличить координаты на значения компонент векторов v1 и v2, 
      умноженных на числа t1 и t2, соответственно.
             \en Increase coordinates by values of v1 and v2 vectors components 
      multiplied by the numbers t1 and t2  respectively. \~
    \param[in] v1, v2 - \ru Исходные векторы.
                        \en The initial vectors. \~
    \param[in] t1, t2 - \ru Числа, на которые умножаются координаты векторов v1 и v2 соответственно.
                        \en Coordinates of v1 and v2 vectors are multiplied by these numbers respectively. \~
  */
  void        Add( const MbDirection & v1, double t1, const MbDirection & v2, double t2 );

  /** \brief \ru Увеличить координаты на значения координат точки.
             \en Increase coordinates by values of point coordinates. \~
    \details \ru Увеличить координаты на значения координат точки v1, умноженных на число t1.
             \en Increase coordinates by values of v1 point coordinates multiplied by t1. \~
    \param[in] v1 - \ru Исходная точка.
                    \en The initial point. \~
    \param[in] t1 - \ru Число, на которое умножаются координаты исходной точки v1.
                    \en Coordinates of the initial point v1 are multiplied by this number. \~
  */
  void        Add( const MbCartPoint & v1, double t1 );

  /** \brief \ru Увеличить координаты на значения координат трех точек.
             \en Increase coordinates by values of three points coordinates. \~
    \details \ru Увеличить координаты на значения координат точек v1, v2 и v3, 
      умноженных на числа t1, t2 и t3, соответственно.
             \en Increase coordinates by values of v1, v2 and v3 points coordinates 
      multiplied by the numbers t1, t2 and t3 respectively. \~
    \param[in] v1, v2, v3 - \ru Исходные точки.
                            \en Initial points. \~
    \param[in] t1, t2, t3 - \ru Числа, на которые умножаются координаты точек v1, v2 и v3 соответственно.
                            \en Coordinates of v1, v2 and v3 points are multiplied by these numbers respectively. \~
  */
  void        Add( const MbCartPoint & v1, double t1, const MbCartPoint & v2, double t2, 
                   const MbCartPoint & v3, double t3 );

  /** \brief \ru Увеличить координаты на значения координат четырех точек.
             \en Increase coordinates by values of four points coordinates. \~
    \details \ru Увеличить координаты на значения координат точек v1, v2, v3 и v4, 
      умноженных на числа t1, t2, t3 и t4, соответственно.
             \en Increase coordinates by values of v1, v2, v3 and v4 points coordinates 
      multiplied by the numbers t1, t2, t3 and t4 respectively. \~
    \param[in] v1, v2, v3, v4 - \ru Исходные точки.
                                \en Initial points. \~
    \param[in] t1, t2, t3, t4 - \ru Числа, на которые умножаются координаты точек v1, v2, v3 и v4 соответственно.
                                \en Coordinates of v1, v2, v3 and v4 points are multiplied by these numbers respectively. \~
  */
  void        Add( const MbCartPoint & v1, double t1, const MbCartPoint & v2, double t2, 
                   const MbCartPoint & v3, double t3, const MbCartPoint & v4, double t4 );
  /** \} */

  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  void        GetProperties( MbProperties &properties );
  /// \ru Записать свойства объекта. \en Set properties of the object. 
  void        SetProperties( const MbProperties &properties );
  /// \ru Являются ли объекты равными? \en Are the objects equal?
  bool        IsSame( const MbCartPoint & other, double accuracy ) const;
  /// \ru Является ли точка неопределенной? \en Is the point undefined?
  bool        IsUndefined() const { return (x == UNDEFINED_DBL || y == UNDEFINED_DBL); }

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbCartPoint, MATH_FUNC_EX )
  DECLARE_NEW_DELETE_CLASS( MbCartPoint )
  DECLARE_NEW_DELETE_CLASS_EX( MbCartPoint )
}; // MbCartPoint


//------------------------------------------------------------------------------
// \ru Расстояние от точки до точки. \en The distance between two points. 
// ---
inline double MbCartPoint::DistanceToPoint( const MbCartPoint & to ) const {
  return ::_hypot( x - to.x, y - to.y );
}


//------------------------------------------------------------------------------
// \ru Квадрат расстояния от точки до точки. \en The squared distance between two points. 
// ---
inline double MbCartPoint::DistanceToPoint2( const MbCartPoint & to ) const {
  double dx = ( x - to.x );
  double dy = ( y - to.y );
  return ( dx * dx + dy * dy );
}


//------------------------------------------------------------------------------
// \ru Сложение двух точек. \en The addition of two points. 
// ---
inline void MbCartPoint::operator += ( const MbCartPoint & pnt ) {
  x += pnt.x;
  y += pnt.y;
}


//------------------------------------------------------------------------------
// \ru Вычитание двух точек. \en The subtraction of two points. 
// ---
inline void MbCartPoint::operator -= ( const MbCartPoint & pnt ) {
  x -= pnt.x;
  y -= pnt.y;
}


//------------------------------------------------------------------------------
// \ru Умножение точки на число. \en The multiplication of a point by a number. 
// ---
inline void MbCartPoint::operator *= ( double factor ) {
  x *= factor; // SKIP_SA
  y *= factor;
}


//------------------------------------------------------------------------------
// \ru Деление точки на число. \en The division of a point by a number. 
// ---
inline void MbCartPoint::operator /= ( double factor ) {
  x /= factor;
  y /= factor;
}


//------------------------------------------------------------------------------
// \ru Унарный минус. \en The unary minus. 
// ---
inline MbCartPoint MbCartPoint::operator - () const {
  return MbCartPoint ( - x, - y );
}


//------------------------------------------------------------------------------
// \ru Умножение точки на число. \en The multiplication of a point by a number. 
// ---
inline MbCartPoint MbCartPoint::operator * ( double factor ) const {
  return MbCartPoint( x * factor, y * factor ); // SKIP_SA
}


//------------------------------------------------------------------------------
// \ru Деление точки на число. \en The division of a point by a number. 
// ---
inline MbCartPoint MbCartPoint::operator / ( double factor ) const {
  return MbCartPoint( x / factor, y / factor );
}


//------------------------------------------------------------------------------
// \ru Проверить на точное равенство. \en Check for an accurate equality. 
// ---
inline bool MbCartPoint::Equal( const MbCartPoint & with ) const {
  return ( x == with.x  && y == with.y ); //-V550
}


//------------------------------------------------------------------------------
// \ru Проверка на равенство. \en The check for equality. 
// ---
inline bool MbCartPoint::operator == ( const MbCartPoint & with ) const
{
  double eps = Math::LengthEps;
  bool res = ( ::fabs( x - with.x ) <= eps ) &&
             ( ::fabs( y - with.y ) <= eps );
  return res;
}


//------------------------------------------------------------------------------
// \ru Проверка на неравенство. \en The check for inequality. 
// ---
inline bool MbCartPoint::operator != ( const MbCartPoint & with ) const {
  return !(*this == with);
}


//------------------------------------------------------------------------------
// \ru Проверка на меньше. \en The check for "less". 
// ---
inline bool MbCartPoint::operator < ( const MbCartPoint & with ) const {
  return ( (x - with.x) < -Math::LengthEps ) ||
         ( (::fabs(x - with.x) <= Math::LengthEps) && ((y - with.y) < -Math::LengthEps) );
  // double inaccuracy
  // return ( x < with.x - Math::LengthEps ) ||
  //       ( (::fabs(x - with.x) <= Math::LengthEps) && (y < with.y - Math::LengthEps) );
}


//------------------------------------------------------------------------------
// \ru Проверка на больше. \en The check for "greater". 
// ---
inline bool MbCartPoint::operator > ( const MbCartPoint & with ) const {
  return ( (x - with.x) > Math::LengthEps ) ||
         ( (::fabs(x - with.x) <= Math::LengthEps) && ((y - with.y) > Math::LengthEps) );
  // double inaccuracy
  // return ( x > with.x + Math::LengthEps ) ||
  //        ( (::fabs(x - with.x) <= Math::LengthEps) && (y > with.y + Math::LengthEps) );
}


//------------------------------------------------------------------------------
// \ru Длина вектора. \en The vector length. 
// ---
inline double MbCartPoint::Length() const {
  return ::_hypot( x, y );
}


//------------------------------------------------------------------------------
// \ru Сложение двух точек. \en The addition of two points. 
// ---
inline MbVector MbCartPoint::operator + ( const MbCartPoint & pnt ) const {
  return MbVector( x + pnt.x, y + pnt.y );
}


//------------------------------------------------------------------------------
// \ru Вычитание двух точек. \en The subtraction of two points. 
// ---
inline MbVector MbCartPoint::operator - ( const MbCartPoint & pnt ) const {
  return MbVector ( x - pnt.x, y - pnt.y );
}


//------------------------------------------------------------------------------
// \ru Сложение точки и вектора. \en The addition of a point and a vector. 
// ---
inline MbCartPoint MbCartPoint::operator + ( const MbVector & vector ) const
{
  return MbCartPoint( x + vector.x, y + vector.y );
}


//------------------------------------------------------------------------------
// \ru Оператор \en Operator 
// ---
inline MbCartPoint MbCartPoint::operator + ( const MbDirection & d ) const
{
  return MbCartPoint( x + d.ax, y + d.ay );
}


//------------------------------------------------------------------------------
// \ru Оператор \en Operator 
// ---
inline void MbCartPoint::operator += ( MbVector & v )
{
  x += v.x;
  y += v.y;
}


//------------------------------------------------------------------------------
// \ru Вычитание вектора из точки. \en The subtraction of a vector from a point. 
// ---
inline MbCartPoint MbCartPoint::operator - ( const MbVector & vector ) const
{
  return MbCartPoint( x - vector.x, y - vector.y );
}


//------------------------------------------------------------------------------
// \ru Оператор \en Operator 
// ---
inline MbCartPoint MbCartPoint::operator - ( const MbDirection & d ) const
{
  return MbCartPoint( x - d.ax, y - d.ay );
}


//------------------------------------------------------------------------------
// \ru Оператор \en Operator 
// ---
inline void MbCartPoint::operator -= ( MbVector & v )
{
  x -= v.x;
  y -= v.y;
}


//------------------------------------------------------------------------------
// \ru Присвоить точке значения другой точки. \en Assign the value of another point to the point. 
// ---
inline MbCartPoint & MbCartPoint::operator = ( const MbCartPoint & fp ) {
  x = fp.x; // SKIP_SA
  y = fp.y;
  return *this;
}

//------------------------------------------------------------------------------
// \ru Присвоение точки значений вектора. \en The assignment of vector values to the point. 
// ---
inline void MbCartPoint::operator = ( const MbVector & v )
{
  x = v.x;
  y = v.y;
}


//------------------------------------------------------------------------------
// \ru Присвоение точки значений. \en The assignment of values to the point. 
// ---
inline void MbCartPoint::operator = ( const MbDirection & d )
{
  x = d.ax;
  y = d.ay;
}


//------------------------------------------------------------------------------
// \ru Сдвиг точки по направлению заданному углом. \en The translation of a point along the direction which is given by an angle. 
// ---
inline void MbCartPoint::MoveAlongLine( double angle, double delta )
{
  MbDirection d( angle ); // \ru Вектор направления \en Direction vector 
  x += d.ax * delta;
  y += d.ay * delta;
}


//------------------------------------------------------------------------------
// \ru Сдвиг в направлении на расстояние. \en The translation along the direction by offset. 
// ---
inline void MbCartPoint::MoveAlongLine( const MbDirection & angle, double delta )
{
  x += angle.ax * delta;
  y += angle.ay * delta;
}


//------------------------------------------------------------------------------
// \ru Сдвиг в направлении на расстояние. \en The translation along the direction by offset. 
// ---
inline void MbCartPoint::MoveAlongLine( const MbVector & vect, double delta )
{
  MbDirection angle;
  angle = vect;
  x += angle.ax * delta;
  y += angle.ay * delta;
}


//------------------------------------------------------------------------------
// \ru Сместиться в направлении точки to на расстояние, \en Displace along the direction of "to" point to a distance 
// \ru Определяемое его отношением к расстоянию между точками \en Which is defined by its ratio to the distance between the points 
// ---
inline void MbCartPoint::GoNearToPoint( const MbCartPoint & to, double ratio )
{
  x += ( to.x - x ) * ratio;
  y += ( to.y - y ) * ratio;
}


//------------------------------------------------------------------------------
// \ru Поворот \en Rotation 
// ---
inline void MbCartPoint::Rotate( const MbCartPoint & c, double angle ) {
  Rotate( c, MbDirection( ::cos( angle ), ::sin( angle ) ) );
}


//------------------------------------------------------------------------------
// \ru Поворот \en Rotation 
// ---
inline void MbCartPoint::Rotate( double angle ) {
  Rotate( MbDirection( ::cos( angle ), ::sin( angle ) ) );
}


//------------------------------------------------------------------------------
// \ru Зеркальное отражение точки от заданной \en Specular reflection of a point from a given point 
// ---
inline MbCartPoint MbCartPoint::Mirror( const MbCartPoint & from) const {
  return MbCartPoint ( from.x - ( x - from.x ), from.y - ( y - from.y ) );
}


//------------------------------------------------------------------------------
// \ru Зеркальное отражение точки от прямой - базовая точка-направление \en Specular reflection of a point from a line (base point and direction) 
// ---
inline void MbCartPoint::Mirror( const MbCartPoint & p1, const MbDirection & dir )
{
  double d2 = ( dir.ax * ( y - p1.y ) - dir.ay * ( x - p1.x ) ) * 2;

  x += dir.ay * d2;
  y -= dir.ax * d2;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline void MbCartPoint::Set( const MbCartPoint & v1, const MbVector & v2, double t2 )
{
  x = v1.x + v2.x * t2;
  y = v1.y + v2.y * t2;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline void MbCartPoint::Set( const MbCartPoint &v1, 
                              const MbVector & v2, double t2, const MbVector & v3, double t3 )
{
  x = v1.x + v2.x * t2 + v3.x * t3;
  y = v1.y + v2.y * t2 + v3.y * t3;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline void MbCartPoint::Set( const MbCartPoint & v1, 
                              const MbDirection & v2, double t2, const MbDirection & v3, double t3 )
{
  x = v1.x + v2.ax * t2 + v3.ax * t3;
  y = v1.y + v2.ay * t2 + v3.ay * t3;
}


//------------------------------------------------------------------------------
// \ru Добавление значений \en Values addition 
// ---
inline void MbCartPoint::Add( const MbVector & v1, double t1 )
{
  x += v1.x * t1;
  y += v1.y * t1;
}


//------------------------------------------------------------------------------
// \ru Добавление значений \en Values addition 
// ---
inline void MbCartPoint::Add( const MbVector & v1, double t1, const MbVector & v2, double t2 )
{
  x += v1.x * t1 + v2.x * t2;
  y += v1.y * t1 + v2.y * t2;
}


//------------------------------------------------------------------------------
// \ru Добавление значений \en Values addition 
// ---
inline void MbCartPoint::Add( const MbVector & v1, double t1, const MbVector & v2, double t2,
                              const MbVector & v3, double t3 )
{
  x += v1.x * t1 + v2.x * t2 + v3.x * t3;
  y += v1.y * t1 + v2.y * t2 + v3.y * t3;
}


//------------------------------------------------------------------------------
// \ru Добавление значений \en Values addition 
// ---
inline void MbCartPoint::Add( const MbVector & v1, double t1, const MbVector & v2, double t2,
                              const MbVector & v3, double t3, const MbVector & v4, double t4 )
{
  x += v1.x * t1 + v2.x * t2 + v3.x * t3 + v4.x * t4;
  y += v1.y * t1 + v2.y * t2 + v3.y * t3 + v4.y * t4;
}


//------------------------------------------------------------------------------
// \ru Добавление значений \en Values addition 
// ---
inline void MbCartPoint::Add( const MbDirection & v1, double t1, const MbDirection & v2, double t2 )
{
  x += v1.ax * t1 + v2.ax * t2;
  y += v1.ay * t1 + v2.ay * t2;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline void MbCartPoint::Set( const MbCartPoint & v1, double t1, const MbCartPoint & v2, double t2 ) {
  x = v1.x * t1 + v2.x * t2;
  y = v1.y * t1 + v2.y * t2;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline void MbCartPoint::Set( const MbCartPoint & v1, double t1, const MbCartPoint & v2, double t2,
                              const MbCartPoint & v3, double t3 ) {
  x = v1.x * t1 + v2.x * t2 + v3.x * t3;
  y = v1.y * t1 + v2.y * t2 + v3.y * t3;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline void MbCartPoint::Set( const MbCartPoint & v1, double t1, const MbCartPoint & v2, double t2,
                              const MbCartPoint & v3, double t3, const MbCartPoint & v4, double t4 ) {
  x = v1.x * t1 + v2.x * t2 + v3.x * t3 + v4.x * t4;
  y = v1.y * t1 + v2.y * t2 + v3.y * t3 + v4.y * t4;
}


//------------------------------------------------------------------------------
// \ru Добавление значений \en Values addition 
// ---
inline void MbCartPoint::Add( const MbCartPoint & v1, double t1 ) {
  x += v1.x * t1;
  y += v1.y * t1;
}


//------------------------------------------------------------------------------
// \ru Добавление значений \en Values addition 
// ---
inline void MbCartPoint::Add( const MbCartPoint & v1, double t1, const MbCartPoint & v2, double t2,
                              const MbCartPoint & v3, double t3 ) {
  x += v1.x * t1 + v2.x * t2 + v3.x * t3;
  y += v1.y * t1 + v2.y * t2 + v3.y * t3;
}


//------------------------------------------------------------------------------
// \ru Добавление значений \en Values addition 
// ---
inline void MbCartPoint::Add( const MbCartPoint & v1, double t1, const MbCartPoint & v2, double t2,
                              const MbCartPoint & v3, double t3, const MbCartPoint & v4, double t4 ) {
  x += v1.x * t1 + v2.x * t2 + v3.x * t3 + v4.x * t4;
  y += v1.y * t1 + v2.y * t2 + v3.y * t3 + v4.y * t4;
}


//------------------------------------------------------------------------------
/** \brief \ru Являются ли объекты равными?
           \en Are the objects equal? \~
  \details \ru Равными считаются объекты, данные которых равны с заданной точностью.
           \en The objects are equal if their data are equal with a given accuracy. \~
  \param[in] other -    \ru Объект для сравнения.
                        \en The object to compare. \~
  \param[in] accuracy - \ru Точность сравнения.
                        \en The accuracy to compare. \~
  \return \ru Равны ли объекты.
          \en Whether the objects are equal.
*/
inline bool MbCartPoint::IsSame( const MbCartPoint & other, double accuracy ) const
{
  return ( (::fabs(x - other.x) < accuracy) && 
           (::fabs(y - other.y) < accuracy) );
}


////////////////////////////////////////////////////////////////////////////////
//
// \ru Двумерный вектор. \en The two-dimensional vector. 
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// \ru Конструктор точки по вектору \en Constructor of a point by a vector 
// ---
inline MbVector::MbVector( const MbCartPoint & p )
  : x( p.x )
  , y( p.y )
{
}

//------------------------------------------------------------------------------
// \ru Инициализировать по заданным точкам. \en Initialize by given points. 
// ---
inline MbVector & MbVector::Init( const MbCartPoint & p1, const MbCartPoint & p2 ) 
{
  x = p2.x - p1.x; 
  y = p2.y - p1.y; 
  return *this;
}


//------------------------------------------------------------------------------
// \ru Сложение вектора и точки \en The addition of a vector and a point 
// ---
inline MbVector MbVector::operator + ( const MbCartPoint & pnt ) const
{
  return MbVector(x + pnt.x, y + pnt.y);
}


//------------------------------------------------------------------------------
// \ru Вычитание из вектора точки \en The subtraction of a point from a vector 
// ---
inline MbVector MbVector::operator - ( const MbCartPoint & pnt ) const
{
  return MbVector( x - pnt.x, y - pnt.y );
}


//------------------------------------------------------------------------------
// \ru Присвоение вектору значений точки \en Assignment of point values to a vector 
// ---
inline MbVector & MbVector::operator = ( const MbCartPoint & pnt )
{
  x = pnt.x;
  y = pnt.y;
  return *this;
}

//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline void MbVector::Set( const MbCartPoint & v1, double t1, const MbCartPoint & v2, double t2 )
{
  x = v1.x * t1 + v2.x * t2;
  y = v1.y * t1 + v2.y * t2;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline void MbVector::Set( const MbCartPoint & v1, double t1, const MbCartPoint & v2, double t2,
                           const MbCartPoint & v3, double t3 )
{
  x = v1.x * t1 + v2.x * t2 + v3.x * t3;
  y = v1.y * t1 + v2.y * t2 + v3.y * t3;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline void MbVector::Set( const MbCartPoint & v1, double t1, const MbCartPoint & v2, double t2,
                           const MbCartPoint & v3, double t3, const MbCartPoint & v4, double t4 )
{
  x = v1.x * t1 + v2.x * t2 + v3.x * t3 + v4.x * t4;
  y = v1.y * t1 + v2.y * t2 + v3.y * t3 + v4.y * t4;
}


////////////////////////////////////////////////////////////////////////////////
//
// \ru Двумерный нормализованный вектор. \en The two-dimensional normalized vector. 
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// \ru Присвоение нормализованному вектору значений точки \en Assignment of point values to a normalized vector 
// ---
inline void MbDirection::operator = ( const MbCartPoint & pnt )
{
//  ax = pnt.x;
//  ay = pnt.y;
  double d  = ::_hypot( pnt.x, pnt.y );

  if ( d > NULL_EPSILON ) {
    ax = pnt.x / d;
    ay = pnt.y / d;
  }
  else {
    ax = ay = 0.0;
  }
}


////////////////////////////////////////////////////////////////////////////////
//
// \ru Глобальные функции \en Global functions 
//
////////////////////////////////////////////////////////////////////////////////

namespace c3d // namespace C3D
{

//------------------------------------------------------------------------------
/**
  \brief \ru Проверить длины на равенство.
         \en Check lengths for equality. \~
  \param[in] lx, ly - \ru Сравниваемые длины.
                      \en Compared lengths. \~
  \param[in] minLen - \ru Минимально возможная длина.
                      \en A minimum possible length. \~
  \param[in] minDev - \ru Метрическая погрешность равенства длин.
                      \en A metric accuracy of lengths equality. \~
  \return \ru true, если длины равны, \n иначе false.
          \en True if lengths are equal, \n false otherwise. \~
  \ingroup Algorithms_2D
*/
//---
inline
bool EqualLengths( double lx, double ly, double minLen = METRIC_EPSILON, double minDev = LENGTH_EPSILON ) {
  return ( ::fabs(lx - ly) < minDev && lx > minLen && ly > minLen );
}

//------------------------------------------------------------------------------
// \ru Проверка корректности точки по величине компонент \en The check of point correctness by a component magnitude 
//---
#if defined(C3D_WINDOWS) && !defined(C3D_CLANG_COMPILER) // clang warning on WINDOWS: '#pragma optimize' is not supported [-Wignored-pragma-optimize]
#pragma optimize( "", off )
#endif // C3D_WINDOWS
template <class Point>
bool IsValidPoint( const Point & p )
{
  bool isValid = true;
  size_t cnt = p.GetDimension();
  for ( size_t k = 0; isValid && k < cnt; k++ ) {
    const double & t = p[k];
    if ( t <= -MB_MAXDOUBLE || t >= MB_MAXDOUBLE )
      isValid = false;
    else if ( t != t ) // реакция на бесконечность
      isValid = false;
  }
  return isValid;
}
#if defined(C3D_WINDOWS)  && !defined(C3D_CLANG_COMPILER) // clang warning on WINDOWS: '#pragma optimize' is not supported [-Wignored-pragma-optimize]
#pragma optimize( "", on )
#endif // C3D_WINDOWS

} // namespace C3D


#endif // __MB_CART_POINT_H
