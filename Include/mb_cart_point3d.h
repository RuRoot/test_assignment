////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Трехмерная точка.
         \en The three-dimensional point. \~

*/
////////////////////////////////////////////////////////////////////////////////


#ifndef __MB_CART_POINT3D_H
#define __MB_CART_POINT3D_H

#include <mb_vector3d.h>
#include <io_base.h>


class  MbCartPoint;
class  MbCartPoint3D;
class  MbFloatPoint3D;


namespace c3d // namespace C3D
{
typedef std::vector<MbCartPoint3D>              SpacePointsVector;  ///< \ru Вектор точек. \en Points vector.
typedef std::vector<MbVector3D>                 SpaceVectorsVector; ///< \ru Вектор векторов. \en Vectors vector.

typedef std::pair<MbCartPoint3D, size_t>        SpacePointIndex;    ///< \ru Пара точка-индекс. \en Pair point-index.
typedef std::pair<MbCartPoint3D, size_t>        Point3DIndex;       ///< \ru Пара точка-индекс. \en Pair point-index.

typedef std::pair<size_t, MbCartPoint3D>        IndexSpacePoint;    ///< \ru Пара индекс-точка. \en Pair index-point.
typedef std::pair<size_t, MbCartPoint3D>        IndexPoint3D;       ///< \ru Пара индекс-точка. \en Pair index-point.

typedef std::pair<MbCartPoint3D, MbCartPoint3D> SpacePointsPair;    ///< \ru Пара точка-точка. \en Pair point-point.

typedef std::pair<MbCartPoint3D, MbVector3D>    SpacePointVector;   ///< \ru Пара точка-вектор. \en Pair point-vector.
typedef std::pair<MbCartPoint3D, MbVector3D>    PointVector3D;      ///< \ru Пара точка-вектор. \en Pair point-vector.

typedef std::pair<double, MbCartPoint3D>        ParamSpacePointPair;   ///< \ru Пара double-точка. \en Pair double-point.
typedef std::vector<ParamSpacePointPair>        ParamSpacePointVector; ///< \ru Вектор пар double-точка. \en Vector of double-point pairs.
}


//------------------------------------------------------------------------------
/** \brief \ru Трехмерная точка.
           \en The three-dimensional point. \~
  \details \ru Трехмерная точка (картезианская точка) описывается тремя координатами в декартовой системе координат.
    Точка названа картезианской в честь французского учёного геометра Рене Декарта (Rene Descartes, по латыни Renatus Cartesius). \n
    Точку можно описать радиусом-вектором. Радиус-вектор описывает преобразование, 
    переводящее начальную точку декартовой системы координат в точку пространства с заданными координатами в этой декартовой системе координат. \n
           \en The three-dimensional point (cartesian point) is defined by three coordinates in a cartesian coordinate system.
    The point is named cartesian in honor of French scientist Rene Descartes (lat. Renatus Cartesius). \n
    A point can be defined by radius-vector. A radius-vector describes transform 
    which translates a start point of a cartesian coordinate system to a space point with the given coordinates in this system. \n \~
  \ingroup Mathematic_Base_3D
*/
// ---
class MATH_CLASS MbCartPoint3D 
{
public :  
  double x; ///< \ru Первая координата точки. \en A first coordinate of point. 
  double y; ///< \ru Вторая координата точки. \en A second coordinate of point. 
  double z; ///< \ru Третья координата точки. \en A third coordinate of point.   

  /// \ru Начало координат или { 0, 0, 0 }. \en The origin or { 0, 0, 0 }. 
  static const MbCartPoint3D origin;  

public :
  /// \ru Конструктор без параметров, точка расположена в начале глобальных координат. \en Constructor without parameters. A point is located at the origin of global coordinates. 
  MbCartPoint3D () : x(0.0), y(0.0), z(0.0) {}
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbCartPoint3D ( const MbCartPoint3D & p ) : x(p.x), y(p.y), z(p.z) {}
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbCartPoint3D ( const MbFloatPoint3D & );
  /// \ru Конструктор по координатам. \en Constructor by coordinates. 
  MbCartPoint3D ( double xx, double yy, double zz ) : x(xx), y(yy), z(zz) {}
  /**
    \brief \ru Конструктор по точке в локальной системе координат.
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
  MbCartPoint3D ( const MbCartPoint3D & p, const MbMatrix3D & matr );
  /**
    \brief \ru Конструктор по двумерной точке.
           \en Constructor by a two-dimensional point. \~
    \details \ru Двумерная точка лежит в плоскости XOY заданной локальной системы координат.
             \en A two-dimensional point is located in the XOY plane defined by a local coordinate system. \~
    \param[in] p - \ru Двумерная точка.
                   \en A two-dimensional point. \~
    \param[in] place - \ru Исходная локальная система координат.
                       \en The initial local coordinate system. \~
  */
  MbCartPoint3D ( const MbCartPoint & p, const MbPlacement3D & place );

  /**
    \brief \ru Инициализация по двумерной точке.
           \en The initialization by two-dimensional point. \~
    \details \ru Двумерная точка лежит в плоскости XOY заданной локальной системы координат.
             \en A two-dimensional point is located in the XOY plane defined by a local coordinate system. \~
    \param[in] p - \ru Двумерная точка.
                   \en A two-dimensional point. \~
    \param[in] place - \ru Исходная локальная система координат.
                       \en The initial local coordinate system. \~
  */
  void        Init( const MbCartPoint & p, const MbPlacement3D & place );
  /// \ru Инициализация по двумерной точке. \en The initialization by two-dimensional point. 
  void        Init( const MbCartPoint & p );
  /// \ru Инициализировать по другой точке. \en Initialize by another point. 
  template <class Point>
  MbCartPoint3D & Init( const Point & p ) { x = p.x; y = p.y; z = p.z; return *this; }
  /// \ru Инициализировать по координатам. \en Initialize by coordinates.
  MbCartPoint3D & Init( double xx, double yy, double zz ) { x = xx; y = yy; z = zz; return *this; }

  /// \ru Преобразовать согласно матрице. \en Transform according to the matrix. 
  MbCartPoint3D & Transform( const MbMatrix3D & );
  /// \ru Сдвинуть вдоль вектора. \en Translate along a vector. 
  MbCartPoint3D & Move( double dx, double dy, double dz ) { x += dx; y += dy; z += dz; return *this; }
  /// \ru Сдвинуть вдоль вектора. \en Translate along a vector. 
  MbCartPoint3D & Move( const MbVector3D & to ) { x += to.x; y += to.y; z += to.z; return *this; }
  /// \ru Повернуть вокруг оси. \en Rotate around an axis. 
  MbCartPoint3D & Rotate( const MbAxis3D   &, double angle );
  /// \ru Расстояние до точки. \en The distance to a point. 
  double          DistanceToPoint( const MbCartPoint3D & ) const;

  /// \ru Обнулить координаты. \en Set coordinates to zero. 
  void        SetZero() { x = y = z = 0; }
  /// \ru Квадрат расстояния от точки до точки. \en The squared distance between two points. 
  double      DistanceToPoint2( const MbCartPoint3D & to ) const;
  /// \ru Сдвинуть по направлению точки to на расстояние delta. \en Translate in the direction of  the "to" point on the "delta" distance.  
  void        MoveAlongLine   ( const MbCartPoint3D & to, double delta );
  /// \ru Сдвинуть по направлению. \en Translate along the direction. 
  void        GoNearToPoint   ( const MbCartPoint3D & to, double ratio );
  /// \ru Инициализация максимальными координатами. \en The initialization by the maximum coordinates. 
  void        Maximum         ( const MbCartPoint3D & p );
  /// \ru Инициализация минимальными координатами. \en The initialization by the minimal coordinates. 
  void        Minimum         ( const MbCartPoint3D & p ); 

  /**
    \brief \ru Увеличить координаты x и y в ( znear / z ) раз.
           \en Increase coordinates x and y in ( znear / z ) times. \~
    \details \ru Увеличение координат производится для учета перспективного преобразования. 
      Точка должна находится в локальной или видовой системе координат. z - координата точки 
      не может быть равна нулю.
             \en The increase of coordinates is produced for a perspective transformation. 
      A point must be located in a local or view coordinate system. Z-coordinate of a point 
      can't be equal to zero. \~
    \param[in] znear - \ru Величина znear/z определяет масштаб увеличения.
                       \en The value znear/z defines the scale factor. \~ 
  */
  void        Perspective( double znear );
  /// \ru Зеркальное отражение точки относительно плоскости. \en The specular reflection of a point relative to a plane. 
  void        Mirror( const MbCartPoint3D & p0, const MbVector3D & dir ); 

  /** 
    \ru \name Логические и арифметические операции.
    \en \name Logical and arithmetical operations.
    \{ */
  /// \ru Добавить вектор. \en Add a vector. 
  void        operator += ( const MbVector3D & );
  /// \ru Вычесть вектор. \en Subtract a vector. 
  void        operator -= ( const MbVector3D & );
  /// \ru Добавить координаты точки. \en Add point coordinates. 
  void        operator += ( const MbCartPoint3D & );
  /// \ru Вычесть координаты точки. \en Subtract point coordinates. 
  void        operator -= ( const MbCartPoint3D & );
  /// \ru Умножить координаты на число. \en Multiply coordinates by a factor. 
  void        operator *= ( double factor );
  /// \ru Разделить координаты на число. \en Divide coordinates by a factor. 
  void        operator /= ( double factor );
  /// \ru Проверить на равенство. \en Check for equality. 
  bool        operator == ( const MbCartPoint3D & ) const;
  /// \ru Проверить на неравенство. \en Check for inequality. 
  bool        operator != ( const MbCartPoint3D & ) const;
  /// \ru Проверить на меньше. \en Check for "less". 
  bool        operator <  ( const MbCartPoint3D & ) const;
  /// \ru Проверить на больше. \en Check for "greater". 
  bool        operator >  ( const MbCartPoint3D & ) const;

  /// \ru Присвоить точке координаты другой точки. \en Assign coordinates of another point to the point. 
  MbCartPoint3D & operator = ( const MbCartPoint3D   & );
  /// \ru Присвоить точке координаты другой точки. \en Assign coordinates of another point to the point. 
  MbCartPoint3D & operator = ( const MbFloatPoint3D  & );
  /// \ru Присвоить точке компоненты вектора. \en Assign vector components to the point. 
  MbCartPoint3D & operator = ( const MbVector3D      & );
  /// \ru Присвоить точке значения однородных координат. \en Assign values of uniform coordinates to the point. 
  MbCartPoint3D & operator = ( const MbHomogeneous3D & );

  /// \ru Сложить точку и вектор. \en Add a point and vector. 
  MbCartPoint3D operator + ( const MbVector3D & ) const;
  /// \ru Вычесть из точки вектор. \en Subtract a vector from the point. 
  MbCartPoint3D operator - ( const MbVector3D & ) const;
  /// \ru Сложить две точки. \en Add two points. 
  MbVector3D    operator + ( const MbCartPoint3D & ) const;
  /// \ru Вычесть из точки точку. \en Subtract a point from the point. 
  MbVector3D    operator - ( const MbCartPoint3D & ) const;
  /// \ru Унарный минус. \en The unary minus. 
  MbCartPoint3D operator - () const;
  /// \ru Вычислить точку как копию данной точки, преобразованную матрицей. \en Calculate the point as this copy transformed by the matrix. 
  MbCartPoint3D operator * ( const MbMatrix3D & ) const;

  /// \ru Доступ к координате по индексу. \en Access to a coordinate by an index. 
  double &      operator [] ( size_t i )       { return i ? (--i ? z : y) : x; }
  /// \ru Значение координаты по индексу. \en The value of a coordinate by an index. 
  double        operator [] ( size_t i ) const { return i ? (--i ? z : y) : x; }
  /** \} */
  /// \ru Количество координат точки. \en The number of point coordinates. 
  static size_t GetDimension() { return 3; }
  
  /** 
    \ru \name Функции сложения и умножения точек с точками и с векторами.
    \en \name The functions of addition and multiplication of points with points and vectors.
    \{ */
  /// \ru Приравнять координаты сумме координат точки и вектора. \en Equate coordinates to sum of point coordinates and vector coordinates. 
  /**
    \param[in] v1 - \ru Исходная точка.
                    \en The initial point. \~
    \param[in] v2 - \ru Исходный вектор.
                    \en The initial vector. \~
  */
  MbCartPoint3D & SetAdd( const MbCartPoint3D & v1, const MbVector3D & v2 );
  /// \ru Приравнять координаты разности координат точки и вектора. \en Equate difference between the coordinates of point and vector to coordinates. 
  /**
    \param[in] v1 - \ru Исходная точка.
                    \en The initial point. \~
    \param[in] v2 - \ru Исходный вектор.
                    \en The initial vector. \~
  */
  MbCartPoint3D & SetDec( const MbCartPoint3D & v1, const MbVector3D & v2 );
  /**
    \brief \ru Приравнять координаты сумме координат точки и вектора.
           \en Equate coordinates to sum of point coordinates and vector coordinates. \~
    \details \ru Приравнять координаты сумме координат точки v1 и вектора v2, умноженного на число t2.
             \en Equate coordinates to sum of v1 point coordinates and v2 vector coordinates multiplied by t2. \~
    \param[in] v1 - \ru Исходная точка.
                    \en The initial point. \~
    \param[in] v2 - \ru Исходный вектор.
                    \en The initial vector. \~
    \param[in] t2 - \ru Число, на которое умножаются координаты исходного вектора v2.
                    \en Factor the coordinates of the initial vector v2 are multiplied by. \~
  */
  MbCartPoint3D & Set( const MbCartPoint3D & v1, const MbVector3D & v2, double t2 );
  /**
    \brief \ru Приравнять координаты сумме координат точки и двух векторов.
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
  MbCartPoint3D & Set( const MbCartPoint3D & v1, const MbVector3D & v2, double t2, 
                       const MbVector3D & v3, double t3 );

  /**
    \brief \ru Приравнять координаты сумме компонент двух векторов.
           \en Equate sum of the components of two vectors to coordinates. \~
    \details \ru Приравнять координаты сумме компонент векторов v1 и v2, умноженных на числа t1 и t2, соответственно.
             \en Equate coordinates to sum of v1 and v2 vectors components multiplied by the numbers t1 and t2 respectively. \~
    \param[in] v1, v2 - \ru Исходные векторы.
                        \en The initial vectors. \~
    \param[in] t1, t2 - \ru Числа, на которые умножаются координаты векторов v1 и v2 соответственно.
                        \en Coordinates of v1 and v2 vectors are multiplied by these numbers respectively. \~
  */
  MbCartPoint3D & Set( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2 );
  /**
    \brief \ru Приравнять координаты сумме компонент четырех векторов.
           \en Equate coordinates to sum of the components of four vectors. \~
    \details \ru Приравнять координаты сумме компонент векторов v1, v2, v3 и v4, 
      умноженных на числа t1, t2, t3 и t4, соответственно.
             \en Equate coordinates to sum of v1, v2, v3 and v4 vectors components 
      multiplied by the numbers t1, t2, t3 and t4 respectively. \~
    \param[in] v1, v2, v3, v4 - \ru Исходные векторы.
                                \en The initial vectors. \~
    \param[in] t1, t2, t3, t4 - \ru Числа, на которые умножаются координаты векторов v1, v2, v3 и v4 соответственно.
                                \en Coordinates of  v1, v2, v3 and v4 vectors are multiplied by these numbers respectively. \~
  */
  MbCartPoint3D & Set( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2, 
                       const MbVector3D & v3, double t3, const MbVector3D & v4, double t4 );

  /**
    \brief \ru Приравнять координаты сумме координат двух точек.
           \en Equate coordinates to sum of two points coordinates. \~
    \details \ru Приравнять координаты сумме координат точек v1 и v2, умноженных на числа t1 и t2, соответственно.
             \en Equate coordinates to sum of v1 and v2 points coordinates multiplied by the numbers t1 and t2 respectively. \~
    \param[in] v1, v2 - \ru Исходные точки.
                        \en Initial points. \~
    \param[in] t1, t2 - \ru Числа, на которые умножаются координаты точек v1 и v2 соответственно.
                        \en Coordinates of v1 and v2 points are multiplied by these numbers respectively. \~
  */
  MbCartPoint3D & Set( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2 );
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
  MbCartPoint3D & Set( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2, 
                       const MbCartPoint3D & v3, double t3 );
  /**
    \brief \ru Приравнять координаты сумме координат четырех точек.
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
  MbCartPoint3D & Set( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2, 
                       const MbCartPoint3D & v3, double t3, const MbCartPoint3D & v4, double t4 );

  /**
    \brief \ru Увеличить координаты на значения компонент вектора.
           \en Increase coordinates by values of vector components. \~
    \details \ru Увеличить координаты на значения компонент вектора v1, умноженных на число t1.
             \en Increase coordinates by values of v1 vector components multiplied by t1. \~
    \param[in] v1 - \ru Исходный вектор.
                    \en The initial vector. \~
    \param[in] t1 - \ru Число, на которое умножаются координаты исходного вектора v1.
                    \en Coordinates of the initial vector v1 are multiplied by this number. \~
  */
  void        Add( const MbVector3D & v1, double t1 );
  /**
    \brief \ru Увеличить координаты на значения компонент двух векторов.
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
  void        Add( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2 );
  /**
    \brief \ru Увеличить координаты на значения компонент трех векторов.
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
  void        Add( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2, 
                   const MbVector3D & v3, double t3 );
  /**
    \brief \ru Увеличить координаты на значения компонент четырех векторов.
           \en Increase coordinates by components values of four vectors. \~
    \details \ru Увеличить координаты на значения компонент векторов v1, v2, v3 и v4, 
      умноженных на числа t1, t2, t3 и t4, соответственно.
             \en Increase coordinates by components values of vectors v1, v2, v3 and v4 
      multiplied by the numbers t1, t2, t3 and t4 respectively. \~
    \param[in] v1, v2, v3, v4 - \ru Исходные векторы.
                                \en The initial vectors. \~
    \param[in] t1, t2, t3, t4 - \ru Числа, на которые умножаются координаты векторов v1, v2, v3 и v4 соответственно.
                                \en Coordinates of  v1, v2, v3 and v4 vectors are multiplied by these numbers respectively. \~
  */
  void        Add( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2, 
                   const MbVector3D & v3, double t3, const MbVector3D & v4, double t4 );

  /**
    \brief \ru Увеличить координаты на значения координат точки.
           \en Increase coordinates by values of point coordinates. \~
    \details \ru Увеличить координаты на значения координат точки v1, умноженных на число t1.
             \en Increase coordinates by values of v1 point coordinates multiplied by t1. \~
    \param[in] v1 - \ru Исходная точка.
                    \en The initial point. \~
    \param[in] t1 - \ru Число, на которое умножаются координаты исходной точки v1.
                    \en Coordinates of the initial point v1 are multiplied by this number. \~
  */
  void        Add( const MbCartPoint3D & v1, double t1 );
  /**
    \brief \ru Увеличить координаты на значения координат трех точек.
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
  void        Add( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2, 
                   const MbCartPoint3D & v3, double t3 );
  /**
    \brief \ru Увеличить координаты на значения координат четырех точек.
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
  void        Add( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2, 
                   const MbCartPoint3D & v3, double t3, const MbCartPoint3D & v4, double t4 );
  /** \} */

  /// \ru Масштабировать координаты. \en Scale the coordinates. 
  /**
    \param[in] sx, sy, sz - \ru Масштабирующие коэффициенты для компонент x, y и z соответственно.
                            \en Scale factors for x,y and z components respectively. \~
  */
  void        Scale( double sx, double sy, double sz ) { x *= sx, y *= sy, z *= sz; }
  /// \ru Масштабировать координаты. \en Scale the coordinates. 
  /**
    \param[in] s - \ru Масштабирующий коэффициент.
                   \en A scale factor. \~
  */
  void        Scale( double s ) { x *= s, y *= s, z *= s; }
  double      MaxFactor() const;                          ///< \ru Дать максимальную по модулю координату \en Get the maximum absolute coordinate  
  /**
    \brief \ru Округлить с точностью до eps.
           \en Rounded with eps tolerance. \~
    \details \ru Округляются координаты точки с заданное точностью.
             \en Point coordinates are rounded with a given tolerance. \~
    \param[in] total - \ru Если true, то округлять в любом случае.
                       \en If true round anyway. \~
    \param[in] eps - \ru Точность округления.
                     \en A round-off tolerance. \~
    \return \ru true, если округление было выполнено.
            \en Returns true if round-off has been done. \~
  */
  bool        SetRoundedValue( bool total, double eps ); 

  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  void        GetProperties( MbProperties & );
  /// \ru Записать свойства объекта. \en Set properties of the object. 
  void        SetProperties( const MbProperties & );
  /// \ru Являются ли объекты равными? \en Are the objects equal?
  bool        IsSame( const MbCartPoint3D & other, double accuracy ) const;
  /// \ru Является ли точка неопределенной? \en Is the point undefined?
  bool        IsUndefined() const { return (x == UNDEFINED_DBL || y == UNDEFINED_DBL || z == UNDEFINED_DBL); }

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbCartPoint3D, MATH_FUNC_EX ) // \ru Для работы со ссылками и объектами класса \en For working with references and objects of the class 
  DECLARE_NEW_DELETE_CLASS( MbCartPoint3D )
  DECLARE_NEW_DELETE_CLASS_EX( MbCartPoint3D )
}; // MbCartPoint3D


//------------------------------------------------------------------------------
/**
  \details \ru Вычислить квадрат расстояния от точки до точки.
           \en Calculate the squared distance from point to point. \~
  \param[in] to - \ru Точка.
                  \en A point. \~
  \return \ru Квадрат расстояния от точки до точки.
          \en The squared distance between two points. \~
*/
// ---
inline double MbCartPoint3D::DistanceToPoint2( const MbCartPoint3D & to ) const {
  double coordDiff[3] = { ( x - to.x ), ( y - to.y ), ( z - to.z ) }; // SKIP_SA
  coordDiff[0] *= coordDiff[0];
  coordDiff[1] *= coordDiff[1];
  coordDiff[2] *= coordDiff[2];
  return coordDiff[0] + coordDiff[1] + coordDiff[2];
}


//------------------------------------------------------------------------------
// расстояние от точки до точки
// ---
inline double MbCartPoint3D::DistanceToPoint( const MbCartPoint3D & to ) const {
  return ::sqrt( DistanceToPoint2(to) );
}


//------------------------------------------------------------------------------
/** 
  \details \ru Сдвиг по направлению точки to на расстояние, определяемое отношением величины ratio 
    к расстоянию между точками.
           \en The translation in the direction of point "to" to the distance which is defined by the ratio of "ratio" and 
    the distance between points. \~
  \param[in] to - \ru Точка.
                  \en A point. \~
  \param[in] ratio - \ru Доля расстояния.
                     \en A distance part. \~
*/
// ---
inline void MbCartPoint3D::GoNearToPoint( const MbCartPoint3D & to, double ratio ) {
  x += ( to.x - x ) * ratio;
  y += ( to.y - y ) * ratio;
  z += ( to.z - z ) * ratio;
}


//------------------------------------------------------------------------------
/**
  \details \ru Зеркальное отражение точки относительно плоскости с началом p0 и нормалью dir.
           \en The specular reflection of a point relative to a plane with origin "p0" and normal "dir". \~
  \param[in] p0 - \ru Точка плоскости симметрии.
                  \en A point of plane of symmetry. \~
  \param[in] dir - \ru Направление нормали плоскости симметрии.
                   \en A direction of normal of plane of symmetry. \~
*/
// ---
inline void MbCartPoint3D::Mirror( const MbCartPoint3D & p0,  const MbVector3D & dir ) {
  MbVector3D ndir( dir );
  MbVector3D vect( p0, *this );
  ndir.Normalize();
  ndir *= 2*( vect * ndir );
  *this -= ndir;
//  *this += 2 * ndir *( ( *this - p0 ) * ndir ) - *this;
}


//------------------------------------------------------------------------------
/**
  \details \ru Координата x (или y,z) точки инициализируется максимальным значением получаемым из сравнения
  соответствующей координаты исходной точки и точки заданной в аргументе функции.
           \en A coordinate x (or y, z) of a point is initialized by maximum value which is got from comparison of
  corresponding coordinate of initial point and point given in function argument. \~
  \param[in] p - \ru Заданная точка.
                 \en A given point. \~
*/
// ---
inline void MbCartPoint3D::Maximum( const MbCartPoint3D & p ) {
  x = std_max( x, p.x );
  y = std_max( y, p.y );
  z = std_max( z, p.z );
}


//------------------------------------------------------------------------------
/**
  \details \ru Координата x (или y,z) точки инициализируется минимальным значением получаемым из сравнения
    соответствующей координаты исходной точки и точки заданной в аргументе функции.
           \en A coordinate x (or y, z) of a point is initialized by minimal value which is got from comparison of
    corresponding coordinate of initial point and point given in function argument. \~
  \param[in] p - \ru Заданная точка.
                 \en A given point. \~
*/
// ---
inline void MbCartPoint3D::Minimum( const MbCartPoint3D & p ) {
  x = std_min( x, p.x );
  y = std_min( y, p.y );
  z = std_min( z, p.z );
}


//------------------------------------------------------------------------------
// \ru Унарный минус \en The unary minus 
// ---
inline MbCartPoint3D MbCartPoint3D::operator - () const {
  return MbCartPoint3D( -x, -y, -z );
}


//------------------------------------------------------------------------------
// \ru Сложение точки и вектора \en Sum of a vector and a point 
// ---
inline MbCartPoint3D MbCartPoint3D::operator + ( const MbVector3D & vector ) const {
  return MbCartPoint3D( x + vector.x, y + vector.y, z + vector.z );
}


//------------------------------------------------------------------------------
// \ru Вычитание вектора из точки \en The subtraction of a vector from a point 
// ---
inline MbCartPoint3D MbCartPoint3D::operator - ( const MbVector3D & vector ) const {
  return MbCartPoint3D( x - vector.x, y - vector.y, z - vector.z );
}


//------------------------------------------------------------------------------
// \ru Сложение двух точек \en Sum of two points 
// ---
inline MbVector3D MbCartPoint3D::operator + ( const MbCartPoint3D & pnt ) const {
  return MbVector3D( x + pnt.x, y + pnt.y, z + pnt.z );
}


//------------------------------------------------------------------------------
// \ru Вычитание двух точек \en The subtraction of two points 
// ---
inline MbVector3D MbCartPoint3D::operator - ( const MbCartPoint3D & pnt ) const {
  return MbVector3D ( x - pnt.x, y - pnt.y, z - pnt.z );
}


//------------------------------------------------------------------------------
// \ru Сложение двух точек \en Sum of two points 
// ---
inline void MbCartPoint3D::operator += ( const MbVector3D & with ) {
  x += with.x;
  y += with.y;
  z += with.z;
}


//------------------------------------------------------------------------------
// \ru Вычитание двух точек \en The subtraction of two points 
// ---
inline void MbCartPoint3D::operator -= ( const MbVector3D & with ) {
  x -= with.x;
  y -= with.y;
  z -= with.z;
}


//------------------------------------------------------------------------------
// \ru Сложение двух точек \en Sum of two points 
// ---
inline void MbCartPoint3D::operator += ( const MbCartPoint3D & with ) {
  x += with.x;
  y += with.y;
  z += with.z;
}


//------------------------------------------------------------------------------
// \ru Вычитание двух точек \en The subtraction of two points 
// ---
inline void MbCartPoint3D::operator -= ( const MbCartPoint3D & with ) {
  x -= with.x;
  y -= with.y;
  z -= with.z;
}


//------------------------------------------------------------------------------
// \ru Умножение точки на число \en The multiplication of a point by a number 
// ---
inline void MbCartPoint3D::operator *= ( double factor ) {
  x *= factor;
  y *= factor;
  z *= factor;
}


//------------------------------------------------------------------------------
// \ru Деление точки на число \en The division of a point by a number 
// ---
inline void MbCartPoint3D::operator /= ( double factor ) {
  // \ru Операция деления занимает 40 циклов процессора, а умножения 7, т.е. (/) 5.7 раза медленней (*) \en Division operation takes 40 CPU cycles and multiplication takes only 7, i.e. division 5.7 times slower than multiplication 
  C3D_ASSERT( factor != 0.0 ); //-V550
  double invFactor = ( 1.0 / factor );
  x *= invFactor;
  y *= invFactor;
  z *= invFactor;
}


//------------------------------------------------------------------------------
// \ru Проверка на равенство. \en The check for equality. 
// ---
inline bool MbCartPoint3D::operator == ( const MbCartPoint3D & with ) const
{
  double eps = Math::region;
  bool res = ( ::fabs( x - with.x ) <= eps ) &&
             ( ::fabs( y - with.y ) <= eps ) &&
             ( ::fabs( z - with.z ) <= eps );
  return res;
}


//------------------------------------------------------------------------------
// \ru Проверка на неравенство. \en The check for inequality. 
// ---
inline bool MbCartPoint3D::operator != ( const MbCartPoint3D & with ) const {
  return !( *this == with );
}


//------------------------------------------------------------------------------
// \ru Проверка на меньше. \en The check for "less". 
// ---
inline bool MbCartPoint3D::operator < ( const MbCartPoint3D & with ) const {
  double eps = Math::region;
  if ( (x - with.x) < -eps )
    return true;
  double dx_abs = ::fabs(x - with.x); // \ru Исключаем повторные вызовы ::fabs (производительность). \en Avoid repeated calls of ::fabs (performance).
  return  ( (dx_abs <= eps) && ((y - with.y) < -eps) ) ||
          ( (dx_abs <= eps) && (::fabs(y - with.y) <= eps) && ((z - with.z) < -eps) );
  // double inaccuracy
  // return  ( x < with.x - eps ) ||
  //         ( (::fabs(x - with.x) <= eps) && (y < with.y - eps) ) ||
  //         ( (::fabs(x - with.x) <= eps) && (::fabs(y - with.y) <= eps) && (z < with.z - eps) );
  // //return ( x < with.x && y < with.y && z < with.z );
}


//------------------------------------------------------------------------------
// \ru Проверка на больше. \en The check for "greater". 
// ---
inline bool MbCartPoint3D::operator > ( const MbCartPoint3D & with ) const {
  double eps = Math::region;
  if ( (x - with.x) > eps )
    return true;
  double dx_abs = ::fabs(x - with.x); // \ru Исключаем повторные вызовы ::fabs (производительность). \en Avoid repeated calls of ::fabs (performance).
  return ( (dx_abs <= eps) && ((y - with.y) > eps) ) ||
         ( (dx_abs <= eps) && (::fabs(y - with.y) <= eps) && ((z - with.z) > eps) );
  // double inaccuracy
  // return ( x > with.x + eps ) ||
  //        ( (::fabs(x - with.x) <= eps) && (y > with.y + eps) ) ||
  //        ( (::fabs(x - with.x) <= eps) && (::fabs(y - with.y) <= eps) && (z > with.z + eps) );
  // //return ( x > with.x && y > with.y && z > with.z );
}


//------------------------------------------------------------------------------
// \ru Присвоение точке значений. \en Assignment of values to point. 
// ---
inline MbCartPoint3D & MbCartPoint3D::operator = ( const MbCartPoint3D & v ) {
  x = v.x;
  y = v.y;
  z = v.z;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Присвоение точке значений вектора. \en Assignment of vector values to point. 
// ---
inline MbCartPoint3D & MbCartPoint3D::operator = ( const MbVector3D & v ) 
{
  x = v.x;
  y = v.y;
  z = v.z;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Приравнять координаты сумме координат точки v1 и вектора v2. \en Equate coordinates to sum of point v1 and vector v2 coordinates. 
// ---
inline MbCartPoint3D & MbCartPoint3D::SetAdd( const MbCartPoint3D & v1, const MbVector3D & v2 ) {
  x = ( v1.x + v2.x );
  y = ( v1.y + v2.y );
  z = ( v1.z + v2.z );
  return *this;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline MbCartPoint3D & MbCartPoint3D::SetDec( const MbCartPoint3D & v1, const MbVector3D & v2 ) 
{
  x = ( v1.x - v2.x );
  y = ( v1.y - v2.y );
  z = ( v1.z - v2.z );
  return *this;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline MbCartPoint3D & MbCartPoint3D::Set( const MbCartPoint3D & v1, const MbVector3D & v2, double t2 ) {
  x = v1.x + v2.x * t2;
  y = v1.y + v2.y * t2;
  z = v1.z + v2.z * t2;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline MbCartPoint3D & MbCartPoint3D::Set( const MbCartPoint3D & v1, const MbVector3D & v2, double t2,
                                           const MbVector3D & v3, double t3 ) {
  x = v1.x + v2.x * t2 + v3.x * t3;
  y = v1.y + v2.y * t2 + v3.y * t3;
  z = v1.z + v2.z * t2 + v3.z * t3;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline MbCartPoint3D & MbCartPoint3D::Set( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2 ) {
  x = v1.x * t1 + v2.x * t2;
  y = v1.y * t1 + v2.y * t2;
  z = v1.z * t1 + v2.z * t2;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline MbCartPoint3D & MbCartPoint3D::Set( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2,
                                           const MbVector3D & v3, double t3, const MbVector3D & v4, double t4 ) {
  x = v1.x * t1 + v2.x * t2 + v3.x * t3 + v4.x * t4;
  y = v1.y * t1 + v2.y * t2 + v3.y * t3 + v4.y * t4;
  z = v1.z * t1 + v2.z * t2 + v3.z * t3 + v4.z * t4;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline MbCartPoint3D & MbCartPoint3D::Set( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2 ) {
  x = v1.x * t1 + v2.x * t2;
  y = v1.y * t1 + v2.y * t2;
  z = v1.z * t1 + v2.z * t2;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline MbCartPoint3D & MbCartPoint3D::Set( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2,
                                           const MbCartPoint3D & v3, double t3 ) {
  x = v1.x * t1 + v2.x * t2 + v3.x * t3;
  y = v1.y * t1 + v2.y * t2 + v3.y * t3;
  z = v1.z * t1 + v2.z * t2 + v3.z * t3;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline MbCartPoint3D & MbCartPoint3D::Set( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2,
                                           const MbCartPoint3D & v3, double t3, const MbCartPoint3D & v4, double t4 ) {
  x = v1.x * t1 + v2.x * t2 + v3.x * t3 + v4.x * t4;
  y = v1.y * t1 + v2.y * t2 + v3.y * t3 + v4.y * t4;
  z = v1.z * t1 + v2.z * t2 + v3.z * t3 + v4.z * t4;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Добавление значений \en Values addition 
// ---
inline void MbCartPoint3D::Add( const MbVector3D &v1, double t1 ) {
  x += v1.x * t1;
  y += v1.y * t1;
  z += v1.z * t1;
}


//------------------------------------------------------------------------------
// \ru Добавление значений \en Values addition 
// ---
inline void MbCartPoint3D::Add( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2 ) {
  x += v1.x * t1 + v2.x * t2;
  y += v1.y * t1 + v2.y * t2;
  z += v1.z * t1 + v2.z * t2;
}


//------------------------------------------------------------------------------
// \ru Добавление значений \en Values addition 
// ---
inline void MbCartPoint3D::Add( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2,
                                const MbVector3D & v3, double t3 ) {
  x += v1.x * t1 + v2.x * t2 + v3.x * t3;
  y += v1.y * t1 + v2.y * t2 + v3.y * t3;
  z += v1.z * t1 + v2.z * t2 + v3.z * t3;
}


//------------------------------------------------------------------------------
// \ru Добавление значений \en Values addition 
// ---
inline void MbCartPoint3D::Add( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2,
                                const MbVector3D & v3, double t3, const MbVector3D & v4, double t4 ) {
  x += v1.x * t1 + v2.x * t2 + v3.x * t3 + v4.x * t4;
  y += v1.y * t1 + v2.y * t2 + v3.y * t3 + v4.y * t4;
  z += v1.z * t1 + v2.z * t2 + v3.z * t3 + v4.z * t4;
}


//------------------------------------------------------------------------------
// \ru Добавление значений \en Values addition 
// ---
inline void MbCartPoint3D::Add( const MbCartPoint3D & v1, double t1 ) {
  x += v1.x * t1;
  y += v1.y * t1;
  z += v1.z * t1;
}


//------------------------------------------------------------------------------
// \ru Добавление значений \en Values addition 
// ---
inline void MbCartPoint3D::Add( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2,
                                const MbCartPoint3D & v3, double t3 ) {
  x += v1.x * t1 + v2.x * t2 + v3.x * t3;
  y += v1.y * t1 + v2.y * t2 + v3.y * t3;
  z += v1.z * t1 + v2.z * t2 + v3.z * t3;
}


//------------------------------------------------------------------------------
// \ru Добавление значений \en Values addition 
// ---
inline void MbCartPoint3D::Add( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2,
                                const MbCartPoint3D & v3, double t3, const MbCartPoint3D & v4, double t4 ) {
  x += v1.x * t1 + v2.x * t2 + v3.x * t3 + v4.x * t4;
  y += v1.y * t1 + v2.y * t2 + v3.y * t3 + v4.y * t4;
  z += v1.z * t1 + v2.z * t2 + v3.z * t3 + v4.z * t4;
}


//-------------------------------------------------------------------------------
// \ru Максимальная по модулю координата \en Maximum absolute coordinate 
// ---
inline double MbCartPoint3D::MaxFactor() const {
  double ax = ::fabs( x );
  double ay = ::fabs( y );
  double az = ::fabs( z );
  return ( ((ax > ay) && (ax > az)) ? ax : ((ay > az) ? ay : az) );
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline void MbVector3D::Set( const MbCartPoint3D & v1, double t1 ) {
  x = v1.x * t1;
  y = v1.y * t1;
  z = v1.z * t1;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline void MbVector3D::Set( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2 ) {
  x = v1.x * t1 + v2.x * t2;
  y = v1.y * t1 + v2.y * t2;
  z = v1.z * t1 + v2.z * t2;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline void MbVector3D::Set( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2,
                             const MbCartPoint3D & v3, double t3 ) {
  x = v1.x * t1 + v2.x * t2 + v3.x * t3;
  y = v1.y * t1 + v2.y * t2 + v3.y * t3;
  z = v1.z * t1 + v2.z * t2 + v3.z * t3;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline void MbVector3D::Set( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2,
                             const MbCartPoint3D & v3, double t3, const MbCartPoint3D & v4, double t4 ) {
  x = v1.x * t1 + v2.x * t2 + v3.x * t3 + v4.x * t4;
  y = v1.y * t1 + v2.y * t2 + v3.y * t3 + v4.y * t4;
  z = v1.z * t1 + v2.z * t2 + v3.z * t3 + v4.z * t4;
}


//------------------------------------------------------------------------------
// \ru Увеличить координаты на значения координат точки v1, умноженных на число t1. \en Increase coordinates by values of v1 point coordinates multiplied by t1. 
// ---
inline void MbVector3D::Add( const MbCartPoint3D & v1, double t1 ) {
  x += v1.x * t1;
  y += v1.y * t1;
  z += v1.z * t1;
}


//------------------------------------------------------------------------------
// \ru Увеличить координаты на значения координат точек v1 и v2, умноженных на числа t1 и t2, соответственно. \en Increase coordinates by values of v1 and v2 points coordinates multiplied by t1 and t2 respectively. 
// ---
inline void MbVector3D::Add( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2 ) {
  x += v1.x * t1 + v2.x * t2;
  y += v1.y * t1 + v2.y * t2;
  z += v1.z * t1 + v2.z * t2;
}


//------------------------------------------------------------------------------
// \ru Увеличить координаты на значения координат точек v1, v2 и v3, умноженных на числа t1, t2 и t3, соответственно. \en Increase coordinates by values of v1, v2 and v3 points coordinates multiplied by t1, t2 and t3 respectively. 
// ---
inline void MbVector3D::Add( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2,
                             const MbCartPoint3D & v3, double t3 ) {
  x += v1.x * t1 + v2.x * t2 + v3.x * t3;
  y += v1.y * t1 + v2.y * t2 + v3.y * t3;
  z += v1.z * t1 + v2.z * t2 + v3.z * t3;
}


//------------------------------------------------------------------------------
// \ru Увеличить координаты на значения координат точек v1, v2, v3 и v4, умноженных на числа t1, t2, t3 и t4, соответственно. \en Increase coordinates by values of v1, v2, v3 and v4 points coordinates multiplied by t1, t2, t3 and t4 respectively. 
// ---
inline void MbVector3D::Add( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2,
                             const MbCartPoint3D & v3, double t3, const MbCartPoint3D & v4, double t4 ) {
  x += v1.x * t1 + v2.x * t2 + v3.x * t3 + v4.x * t4;
  y += v1.y * t1 + v2.y * t2 + v3.y * t3 + v4.y * t4;
  z += v1.z * t1 + v2.z * t2 + v3.z * t3 + v4.z * t4;
}


//------------------------------------------------------------------------------
// \ru Инициализация вектора по двум точкам. \en The initialization of a vector by two points. 
// ---
inline void MbVector3D::Init( const MbCartPoint3D & p1, const MbCartPoint3D & p2 ) {
  x = p2.x - p1.x;
  y = p2.y - p1.y;
  z = p2.z - p1.z;
}


//------------------------------------------------------------------------------
/// \ru Умножение точки на число. \en The multiplication of a point by a number. 
/** 
  \param[in] pnt    - \ru Точка.
                      \en A point. \~
  \param[in] factor - \ru Число.
                      \en A number. \~
  \return \ru Точку, умноженную на число.
          \en Returns a point multiplied by a number. \~
  \ingroup Mathematic_Base_3D
*/
// ---
inline MbCartPoint3D operator * ( const MbCartPoint3D & pnt, double factor ) {
  return MbCartPoint3D( pnt.x * factor, pnt.y * factor, pnt.z * factor );
}


//------------------------------------------------------------------------------
/// \ru Деление точки на число. \en The division of a point by a number. 
/** 
  \param[in] pnt    - \ru Точка.
                      \en A point. \~
  \param[in] factor - \ru Число.
                      \en A number. \~
  \return \ru Точку, разделенную на число.
          \en Returns a point divided by a number. \~
  \ingroup Mathematic_Base_3D
*/
// ---
inline MbCartPoint3D operator / ( const MbCartPoint3D & pnt, double factor ) {
  // \ru Операция деления занимает 40 циклов процессора, а умножения 7, т.е. (/) 5.7 раза медленней (*) \en Division operation takes 40 CPU cycles and multiplication takes only 7, i.e. division 5.7 times slower than multiplication 
  C3D_ASSERT( factor != 0.0 ); //-V550
  double invFactor = ( 1.0 / factor );
  return MbCartPoint3D( pnt.x * invFactor, pnt.y * invFactor, pnt.z * invFactor );
}


//------------------------------------------------------------------------------
/// \ru Умножение координат точки на число. \en The multiplication of point coordinates by a number. 
/** 
  \param[in] factor - \ru Число.
                      \en A number. \~
  \param[in] pnt    - \ru Точка.
                      \en A point. \~
  \return \ru Точка с увеличенными в число раз координатами.
          \en A point with coordinates multiplied by factor. \~
  \ingroup Mathematic_Base_3D
*/
// ---
inline MbCartPoint3D operator * ( double factor, const MbCartPoint3D & pnt ) {
  return MbCartPoint3D( pnt.x * factor, pnt.y * factor, pnt.z * factor );
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
inline bool MbCartPoint3D::IsSame( const MbCartPoint3D & other, double accuracy ) const
{
  return ( (::fabs(x - other.x) < accuracy) && 
           (::fabs(y - other.y) < accuracy) && 
           (::fabs(z - other.z) < accuracy) );
}


#endif // __MB_CART_POINT3D_H
