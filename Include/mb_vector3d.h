////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Вектор в трехмерном пространстве.
         \en Vector in three-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_VECTOR3D_H
#define __MB_VECTOR3D_H


#include <io_base.h>
#include <mb_enum.h>


class MbVector;
class MbCartPoint3D;
class MbHomogeneous3D;
class MbFloatVector3D;
class MbAxis3D;
class MbPlacement3D;
class MbMatrix3D;
class MbProperties;


//------------------------------------------------------------------------------
/** \brief \ru Вектор в трехмерном пространстве.
           \en Vector in three-dimensional space. \~
  \details \ru Вектор описывает перемещение или направление в трёхмерном пространстве
    и определяется тремя компонентами x, y, z в декартовой системе координат.  \n
    Вектор не привязан к точкам пространства и поэтому не имеет метода,
    перемещающего его в пространстве. \n
           \en Vector describes translation or direction in three-dimensional space
    and is defined by three coordinates x, y, z in the Cartesian coordinate system.  \n
    Vector is not binded to space points and therefore has not method
    for translating it in space. \n \~
  \ingroup Mathematic_Base_3D
*/
// ---
class MATH_CLASS MbVector3D {
public :
  double x; ///< \ru Первая компонента вектора. \en First component of vector. 
  double y; ///< \ru Вторая компонента вектора. \en Second component of vector. 
  double z; ///< \ru Третья компонента вектора. \en Third component of vector.   
  
  static const MbVector3D zero;   ///< \ru Нулевой вектор. \en Zero vector. 
  static const MbVector3D xAxis;  ///< \ru Вектор "X" стандартного базиса. \en "X" vector of standard basis. 
  static const MbVector3D yAxis;  ///< \ru Вектор "Y" стандартного базиса. \en "Y" vector of standard basis. 
  static const MbVector3D zAxis;  ///< \ru Вектор "Z" стандартного базиса. \en "Z" vector of standard basis. 

public :
  /// \ru Конструктор без параметров, вектор нулевой. \en Constructor without parameters, vector is zero. 
  MbVector3D ()                                : x( 0.0 ), y( 0.0 ), z( 0.0 ) {}
  /// \ru Конструктор по координатам. \en The constructor by coordinates. 
  MbVector3D ( double a, double b, double c )  : x( a   ), y( b   ), z( c   ) {}
  /// \ru Конструктор-копия. \en Copy constructor. 
  MbVector3D ( const MbVector3D & v )          : x( v.x ), y( v.y ), z( v.z ) {}
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbVector3D ( const MbFloatVector3D & );
  /// \ru Конструктор по точке. \en Constructor by point. 
  MbVector3D ( const MbCartPoint3D & p );
  /// \ru Конструктор по двум точкам. \en Constructor by two points. 
  MbVector3D ( const MbCartPoint3D & p1, const MbCartPoint3D & p2 ) { Init( p1, p2 );  }  
  /// \ru Конструктор по двумерному вектору в плоскости XOY локальной системы координат place. \en Constructor by two-dimensional vector in XOY plane of local coordinate system 'place'. 
  MbVector3D ( const MbVector & v2d, const MbPlacement3D & place )  { Init( v2d, place ); }
  /// \ru Конструктор по двумерному вектору в плоскости XOY локальной системы координат place. \en Constructor by two-dimensional vector in XOY plane of local coordinate system 'place'. 
  MbVector3D( const MbVector & v2d ) { InitXY( v2d );  }

  /// \ru Инициализировать по двумерному вектору в плоскости XOY локальной системы координат place. \en Initialize by two-dimensional vector in XOY plane of local coordinate system 'place'. 
  void        Init( const MbVector &, const MbPlacement3D & );
  /// \ru Инициализировать по двумерному вектору. \en Initialize by two-dimensional vector. 
  void        InitXY( const MbVector & );
  /// \ru Инициализировать по координатам. \en Initialize by coordinates. 
  void        Init( double a, double b, double c ) { x = a;   y = b;   z = c;   }
  /// \ru Инициализировать по другому вектору. \en Initialize by another vector. 
  template <class Vector>
  void        Init( const Vector & v ) { x = v.x; y = v.y; z = v.z; }  
  /// \ru Инициализировать по двум точкам. \en Initialize by two points. 
  void        Init( const MbCartPoint3D & p1, const MbCartPoint3D & p2 );
  /// \ru Увеличить координаты на заданные величины. \en Increase coordinates by given values. 
  void        Add ( double a, double b, double c ) { x += a;   y += b;   z += c;   }
  /// \ru Увеличить координаты на значение координат заданного вектора. \en Increase coordinates by values of given vector. 
  void        Add ( const MbVector3D & v )         { x += v.x; y += v.y; z += v.z; }

  /// \ru Обнулить координаты вектора. \en Set coordinates of vector to zero. 
  void        SetZero() { x = y = z = 0.0; }
  /// \ru Является ли вектор нулевым? \en Check if  vector is zero. 
  bool        IsZero ( double eps = Math::lengthEpsilon ) const;

  /// \ru Преобразовать согласно матрице. Матрица действует на вектор справа. \en Transform according to the matrix. A matrix acts on a vector from the right.
  MbVector3D & Transform( const MbMatrix3D & matr );
  /// \ru Повернуть вокруг оси на заданный угол. \en Rotate at given angle around axis. 
  MbVector3D & Rotate( const MbVector3D & axis, double angle );
  /// \ru Повернуть вокруг оси на заданный угол. \en Rotate at given angle around axis. 
  MbVector3D & Rotate( const MbAxis3D & axis, double angle );
  /// \ru Повернуть вокруг оси (по её номеру) на заданный угол ( 0 - ось X, 1 - ось Y, 2 - ось Z ). \en Rotate around the axis (by its number) at a given angle ( 0 - X-axis, 1 - Y-axis, 2 - Z-axis ). 
  MbVector3D & RotateXYZ( int number, double angle );

  /// \ru Проверить вектор на вырожденность. \en Check vector for degeneracy. 
  bool        IsDegenerate( double comEps = Math::region ) const;
  /// \ru Проверить вектор на нормированность. \en Check if vector is normalized. 
  bool        IsNormalized( double eps = Math::lengthEpsilon ) const;
  /// \ru Проверить коллинеарность векторов с заданной точностью (по косинусу угла между векторами). \en Check if vectors are colinear with given tolerance (by cosine of angle between vectors). 
  bool        RoundColinear( const MbVector3D & with, double eps = Math::paramNear ) const; 
  /// \ru Проверить коллинеарность векторов с заданной точностью (по синусу угла между векторами). \en Check if vectors are colinear with given tolerance (by sine of angle between vectors). 
  bool        Colinear ( const MbVector3D & with, double eps = Math::angleRegion ) const; 
  /// \ru Проверить ортогональность векторов с заданной точностью. \en Check if vectors are orthogonal with given tolerance. 
  bool        Orthogonal( const MbVector3D & with, double eps = Math::angleRegion ) const;
  /// \ru Рассчитать длину вектора. \en Calculate vector length. 
  double      Length() const;
  /// \ru Рассчитать квадрат длины вектора. \en Calculate vector length square 
  double      Length2() const;
  /// \ru Рассчитать угол между векторами. \en Calculate angle between vectors. 
  double      Angle( const MbVector3D & with ) const;
  /// \ru Нормализовать вектор. \en Normalize a vector. 
  bool        Normalize();
  /// \ru Вернуть нормализованную копию вектора. \en Return normalized copy of vector. 
  MbVector3D  GetNormalized() const;
  /// \ru Сменить направление вектора на противоположное. \en Change vector direction to opposite. 
  MbVector3D& Invert();
  /// \ru Вернуть составляющую часть вектора, параллельную вектору v. \en Get parallel to vector v part of vector. 
  MbVector3D  TangentComponent( const MbVector3D & v ) const;  
  /// \ru Вернуть составляющую часть вектора, ортогональную вектору v. \en Get orthogonal to vector v part of vector. 
  MbVector3D  NormalComponent ( const MbVector3D & v ) const;  

  /// \ru Учесть перспективу для первой производной. \en Perspective for first derivative is to be considered. 
  void        PspDerivative      ( double, const MbCartPoint3D & );
  /// \ru Учесть перспективу для производной второго порядка. \en Perspective for second derivative is to be considered. 
  void        PspDerivative2     ( double, const MbCartPoint3D &, const MbVector3D & );
  /// \ru Учесть перспективу для смешанной производной второго порядка. \en Perspective for second order mixed derivative is to be considered. 
  void        PspDerivative2Mix  ( double, const MbCartPoint3D &, const MbVector3D &, const MbVector3D & );
  /// \ru Учесть перспективу для производной третьего порядка. \en Perspective for third derivative is to be considered. 
  void        PspDerivative3     ( double, const MbCartPoint3D &, const MbVector3D &, const MbVector3D & );
  /// \ru Учесть перспективу для смешанной производной третьего порядка. \en Perspective for third order mixed derivative is to be considered. 
  void        PspDerivative3Mix  ( double, const MbCartPoint3D &, const MbVector3D &, const MbVector3D &, 
                                   const MbVector3D &, const MbVector3D & );
  /// \ru Учесть перспективу для нормали. \en Perspective for normal is to be considered. 
  void        PspNormal          ( double, const MbCartPoint3D & );
  /// \ru Учесть перспективу для производной нормали. \en Perspective for derivative of normal is to be considered. 
  void        PspNormalDerivative( double, const MbCartPoint3D &, const MbVector3D &, const MbVector3D & );

  /// \ru Проверить на равенство. \en Check for equality. 
  bool        operator == ( const MbVector3D & ) const;
  /// \ru Проверить на неравенство. \en Check for inequality. 
  bool        operator != ( const MbVector3D & ) const;
  /// \ru Сложить векторы. \en Sum up two vectors. 
  MbVector3D  operator +  ( const MbVector3D & ) const;
  /// \ru Найти разность векторов. \en Find the difference of vectors. 
  MbVector3D  operator -  ( const MbVector3D & ) const;
  /// \ru Сложить вектор с точкой. \en Sum up vector and point. 
  MbVector3D  operator +  ( const MbCartPoint3D & ) const;
  /// \ru Вычесть из вектора точку. \en Subtract point from vector. 
  MbVector3D  operator -  ( const MbCartPoint3D & ) const;
  /// \ru Унарный минус. \en Unary minus. 
  MbVector3D  operator -  () const;

  /// \ru Сложить векторы. \en Sum up two vectors. 
  MbVector3D & operator += ( const MbVector3D & );
  /// \ru Вычесть из вектора точку. \en Subtract point from vector. 
  MbVector3D & operator -= ( const MbVector3D & );
  /// \ru Умножить вектор на число. \en Multiply vector by number. 
  MbVector3D & operator *= ( double );
  /// \ru Разделить вектор на число. \en Divide vector by number. 
  MbVector3D & operator /= ( double );

  /// \ru Вычислить скалярное произведение двух векторов. \en Calculate dot product of two vectors. 
  double      operator * ( const MbVector3D & ) const;
  /// \ru Вычислить векторное произведение двух векторов. \en Calculate vector product of two vectors. 
  MbVector3D  operator | ( const MbVector3D & ) const;
  /// \ru Вычислить прямое произведение двух векторов. \en Calculate direct product of two vectors. 
  MbMatrix3D  operator & ( const MbVector3D & ) const;
  /// \ru Вычислить вектор как копию данного вектора, преобразованную матрицей. \en Calculate the vector as this copy transformed by the matrix. 
  MbVector3D  operator * ( const MbMatrix3D & ) const;

  /// \ru Присвоить вектору значения координат точки. \en Assign point coordinate values to vector. 
  MbVector3D & operator = ( const MbCartPoint3D   & );
  /// \ru Присвоить вектору значения однородных координат точки. \en Assign uniform point coordinate values to vector. 
  MbVector3D & operator = ( const MbHomogeneous3D & );
  /// \ru Присвоить вектору координаты другого вектора. \en Assign coordinates of another vector to the vector. 
  MbVector3D & operator = ( const MbFloatVector3D & );
  /// \ru Присвоить вектору значения другого вектора. \en Assign the value of another vector to the vector. 
  MbVector3D & operator = ( const MbVector3D & );

  /// \ru Получить доступ к координате по индексу. \en Access to coordinate by an index. 
  double &    operator [] ( size_t i )       { return i ? (--i ? z : y) : x; };
  /// \ru Получить значение координаты по индексу. \en Get coordinate value by an index. 
  double      operator [] ( size_t i ) const { return i ? (--i ? z : y) : x; };
  /// \ru Получить количество координат точки. \en Get point coordinates count. 
  static size_t GetDimension() { return 3; }

  /// \ru Приравнять вектору вектор v1, умноженный на t1. \en Equate vector with vector v1 multiplied by t1. 
  void        Set( const MbVector3D & v1, double t1 );
  /// \ru Приравнять вектору сумму векторов v1 и v2, умноженных на t1 и t2 соответственно. \en Equate vector with sum of vectors v1 and v2 multiplied with t1 and t2 correspondingly. 
  void        Set( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2 );
  /// \ru Приравнять вектору сумму векторов v1, v2 и v3, умноженных на t1, t2 и t3 соответственно. \en Equate vector with sum of vectors v1, v2 and v3 multiplied with t1, t2 and t3 correspondingly. 
  void        Set( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2, 
                   const MbVector3D & v3, double t3 );
  /// \ru Приравнять вектору сумму векторов v1, v2, v3 и v4, умноженных на t1, t2, t3 и t4 соответственно. \en Equate vector with sum of vectors v1, v2, v3 and v4 multiplied with t1, t2, t3 and t4 correspondingly. 
  void        Set( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2, 
                   const MbVector3D & v3, double t3, const MbVector3D & v4, double t4 );

  /// \ru Приравнять координаты вектора координатам точки v1, умноженных на t1. \en Equate coordinates of vector with coordinates of point v1 multiplied by t1. 
  void        Set( const MbCartPoint3D & v1, double t1 );
  /// \ru Приравнять координаты вектора координатам суммы точек v1 и v2, умноженных на t1 и t2 соответственно. \en Equate vector coordinates with sum of points v1 and v2 multiplied with t1 and t2 correspondingly. 
  void        Set( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2 );
  /// \ru Приравнять координаты вектора координатам суммы точек v1, v2 и v3, умноженных на t1, t2 и t3 соответственно. \en Equate vector coordinates with coordinates of sum of vectors v1, v2 and v3 multiplied with t1, t2 and t3 correspondingly. 
  void        Set( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2, 
                   const MbCartPoint3D & v3, double t3 );
  /// \ru Приравнять координаты вектора координатам суммы точек v1, v2, v3 и v4, умноженных на t1, t2, t3 и t4 соответственно. \en Equate vector coordinates with coordinates of sum of points v1, v2, v3 and v4 multiplied with t1, t2, t3 and t4 correspondingly. 
  void        Set( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2, 
                   const MbCartPoint3D & v3, double t3, const MbCartPoint3D & v4, double t4 );

  /// \ru Прибавить к вектору вектор v1, умноженный на t1. \en Add vector v1 multiplied by t1 to a vector. 
  void        Add( const MbVector3D & v1, double t1 );
  /// \ru Прибавить к вектору сумму векторов v1 и v2, умноженных на t1 и t2 соответственно. \en Add sum of vectors v1 and v2 multiplied with t1 and t2 correspondingly to a vector. 
  void        Add( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2 );
  /// \ru Прибавить к вектору сумму векторов v1, v2 и v3, умноженных на t1, t2 и t3 соответственно. \en Add sum of vectors v1, v2 and v3 multiplied with t1, t2 and t3 correspondingly to a vector. 
  void        Add( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2, 
                   const MbVector3D & v3, double t3 );
  /// \ru Прибавить к вектору сумму векторов v1, v2, v3 и v4, умноженных на t1, t2, t3 и t4 соответственно. \en Add sum of vectors v1, v2, v3 and v4 multiplied with t1, t2, t3 and t4 correspondingly to a vector. 
  void        Add( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2, 
                   const MbVector3D & v3, double t3, const MbVector3D & v4, double t4 );

  /// \ru Прибавить к координатам вектора координаты точки v1, умноженных на t1. \en Add coordinates of point v1 multiplied by t1 to coordinates of vector. 
  void        Add( const MbCartPoint3D & v1, double t1 );
  /// \ru Прибавить к координатам вектора координаты точек v1 и v2, умноженных на числа t1 и t2, соответственно. \en Add sum of coordinates of points v1 and v2 multiplied with t1 and t2 correspondingly to coordinates of vector. 
  void        Add( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2 );
  /// \ru Прибавить к координатам вектора координаты точек v1, v2 и v3, умноженных на числа t1, t2 и t3, соответственно. \en Add sum of coordinates of points v1, v2 and v3 multiplied with t1, t2 and t3 correspondingly to coordinates of vector. 
  void        Add( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2, 
                   const MbCartPoint3D & v3, double t3 );
  /// \ru Прибавить к координатам вектора координаты точек v1, v2, v3 и v4, умноженных на числа t1, t2, t3 и t4, соответственно. \en Add sum of coordinates of points v1, v2, v3 and v4 multiplied with t1, t2, t3 and t4 correspondingly to coordinates of vector. 
  void        Add( const MbCartPoint3D & v1, double t1, const MbCartPoint3D & v2, double t2, 
                   const MbCartPoint3D & v3, double t3, const MbCartPoint3D & v4, double t4 );

  /// \ru Задать векторное произведение двух заданных векторов. \en Set vector product of two given vectors. 
  void        SetVecM( const MbVector3D & vF, const MbVector3D & vS ); 
  /// \ru Задать векторное произведение двух заданных векторов, умноженное на mulKoef. \en Set vector product of two given vectors multiplied by mulKoef. 
  void        SetVecM( const MbVector3D & vF, const MbVector3D & vS, double mulKoef ); 
  /// \ru Добавить к вектору векторное произведение двух заданных векторов. \en Add vector product of two given vectors to vector. 
  void        AddVecM( const MbVector3D & vF, const MbVector3D & vS ); 
  /// \ru Добавить к вектору векторное произведение двух заданных векторов, умноженное на mulKoef. \en Add vector product of two given vectors multiplied by mulKoef to vector. 
  void        AddVecM( const MbVector3D & vF, const MbVector3D & vS, double mulKoef ); 

  /// \ru Найти максимальную по модулю компоненту вектора. \en Find the largest absolute value of a vector. 
  double      MaxFactor() const;
  /// \ru Масштабировать компоненты. \en Scale components. 
  void        Scale( double sx, double sy, double sz ) { x *= sx, y *= sy, z *= sz; }
  /// \ru Масштабировать компоненты. \en Scale components. 
  void        Scale( double s ) { x *= s, y *= s, z *= s; }

  /// \ru Округлить компоненты вектора. \en Round components of vector.   
  bool        SetRoundedValue( bool total, double eps );

  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  void        GetProperties( MbProperties & );
  /// \ru Записать свойства объекта. \en Set properties of the object. 
  void        SetProperties( const MbProperties & );
  /// \ru Являются ли объекты равными? \en Are the objects equal?
  bool        IsSame( const MbVector3D & other, double accuracy ) const;

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbVector3D, MATH_FUNC_EX ) // \ru Для работы со ссылками и объектами класса \en For working with references and objects of the class. 
  DECLARE_NEW_DELETE_CLASS( MbVector3D )
  DECLARE_NEW_DELETE_CLASS_EX( MbVector3D )
}; // MbVector3D


//------------------------------------------------------------------------------
// \ru Сложить два вектора; результат - вектор. \en Sum up two vectors; result is vector. 
// ---
inline MbVector3D MbVector3D::operator + ( const MbVector3D & vector ) const
{
  return MbVector3D( x + vector.x, y + vector.y, z + vector.z );
}


//------------------------------------------------------------------------------
// \ru Вычесть из вектора вектор; результат - вектор. \en Subtract vector from vector; result is vector. 
// ---
inline MbVector3D MbVector3D::operator - ( const MbVector3D & v2 ) const
{
  return MbVector3D ( x - v2.x, y - v2.y, z - v2.z );
}


//------------------------------------------------------------------------------
// \ru Унарный минус. \en Unary minus. 
// ---
inline MbVector3D MbVector3D::operator - () const
{
  return MbVector3D ( - x, - y, - z );
}


//------------------------------------------------------------------------------
// \ru Сложить два вектора. \en Sum up two vectors. 
// ---
inline MbVector3D & MbVector3D::operator += ( const MbVector3D & vector )
{
  x += vector.x;
  y += vector.y;
  z += vector.z;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Вычесть из вектора вектор. \en Subtract vector from vector. 
// ---
inline MbVector3D & MbVector3D::operator -= ( const MbVector3D & vector )
{
  x -= vector.x;
  y -= vector.y;
  z -= vector.z;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Умножить вектор на число. \en Multiply vector by number. 
// ---
inline MbVector3D & MbVector3D::operator *= ( double factor )
{
  x *= factor;
  y *= factor;
  z *= factor;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Разделить вектор на число. \en Divide vector by number. 
// ---
inline MbVector3D & MbVector3D::operator /= ( double factor )
{
  // \ru Операция деления занимает 40 циклов процессора, а умножения 7, т.е. (/) 5.7 раза медленней (*) \en Division operation takes 40 CPU cycles and multiplication takes only 7, i.e. division 5.7 times slower than multiplication 
  C3D_ASSERT( factor != 0.0 ); //-V550
  double invFactor = ( 1.0 / factor );
  x *= invFactor;
  y *= invFactor;
  z *= invFactor;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Вычислить скалярное произведение двух векторов. \en Calculate dot product of two vectors. 
// ---
inline double MbVector3D::operator * ( const MbVector3D & vector ) const
{
  return ( x * vector.x + y * vector.y + z * vector.z );
}


//------------------------------------------------------------------------------
// \ru Вычислить векторное произведение двух векторов. \en Calculate vector product of two vectors. 
// ---
inline MbVector3D MbVector3D::operator | ( const MbVector3D & vect2 ) const
{
  return MbVector3D( y * vect2.z - z * vect2.y,
                     z * vect2.x - x * vect2.z,
                     x * vect2.y - y * vect2.x );
}


//------------------------------------------------------------------------------
// \ru Проверить на равенство. \en Check for equality. 
// ---
inline bool MbVector3D::operator == ( const MbVector3D & with ) const
{
  return ( ::fabs( x - with.x ) < Math::region &&
           ::fabs( y - with.y ) < Math::region &&
           ::fabs( z - with.z ) < Math::region );
}


//------------------------------------------------------------------------------
// \ru Присвоить вектору значения другого вектора. \en Assign the value of another vector to the vector. 
// ---
inline MbVector3D & MbVector3D::operator = ( const MbVector3D & v )
{
  x = v.x; // SKIP_SA
  y = v.y;
  z = v.z;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Проверить на неравенство. \en Check for inequality. 
// ---
inline bool MbVector3D::operator != ( const MbVector3D & with ) const
{
  return !( *this == with );
}


//------------------------------------------------------------------------------
// \ru Проверить на вырожденность. \en Check for degeneracy. 
// ---
inline bool MbVector3D::IsDegenerate( double comEps ) const
{
  return ( ::fabs( x ) < comEps && 
           ::fabs( y ) < comEps && 
           ::fabs( z ) < comEps );
}


//------------------------------------------------------------------------------
// \ru Проверить, является ли вектор нормированным. \en Check if  vector is normalized. 
// ---
inline bool MbVector3D::IsNormalized( double eps ) const
{  
  return ( ::fabs( Length() - 1.0) < eps );
}


//------------------------------------------------------------------------------
// \ru Проверить, является ли вектор нулевым. \en Check if  vector is zero. 
// ---
inline bool MbVector3D::IsZero( double eps ) const 
{ 
  return ( ::fabs(x) < eps && 
           ::fabs(y) < eps && 
           ::fabs(z) < eps );
}


//------------------------------------------------------------------------------
// \ru Сменить направление вектора на противоположное. \en Change vector direction to opposite. 
// ---
inline MbVector3D & MbVector3D::Invert()
{
  x = - x;
  y = - y;
  z = - z;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Найти длину вектора. \en Find vector length. 
// ---
inline double MbVector3D::Length() const
{
  return ::sqrt( x * x + y * y + z * z );
}


//------------------------------------------------------------------------------
// \ru Найти квадрат длины вектора. \en Find vector length square. 
// ---
inline double MbVector3D::Length2() const
{
  return ( x * x + y * y + z * z );
}


//------------------------------------------------------------------------------
// \ru Нормализовать вектор. \en Normalize a vector. 
// ---
inline bool MbVector3D::Normalize()
{
  double len = Length();
//  C3D_ASSERT( len > LENGTH_EPSILON || len < NULL_EPSILON );
  bool res = ( len >= NULL_EPSILON );
  if ( res && ::fabs(len - 1.0) > NULL_EPSILON ) {
    double one_len = 1.0 / len;
    x *= one_len;
    y *= one_len;
    z *= one_len;
    C3D_ASSERT( ::fabs(Length() - 1.0) < LENGTH_EPSILON );
  }
  return res;
}


//------------------------------------------------------------------------------
// \ru Вернуть нормализованную копию вектора. \en Return normalized copy of vector. 
// ---
inline MbVector3D MbVector3D::GetNormalized() const
{
  double len = Length();
  if ( len >= NULL_EPSILON && ::fabs(len - 1.0) > NULL_EPSILON ) 
    len = 1.0 / len;
  MbVector3D res( x * len, y * len, z * len );
  return res;
}


//------------------------------------------------------------------------------
// \ru Выдать компоненту вектора в направлении vector. \en Give part of vector coincident to direction 'vector'. 
// ---
inline MbVector3D MbVector3D::TangentComponent( const MbVector3D & vector ) const
{
  MbVector3D res( vector );
  res.Normalize();
  res *= res * (*this);
  return res;
}


//------------------------------------------------------------------------------
// \ru Выдать компоненту вектора в направлении ортогональном vector. \en Give part of vector in direction orthogonal to 'vector'. 
// ---
inline MbVector3D MbVector3D::NormalComponent( const MbVector3D & vector ) const
{
  MbVector3D res( vector );
  res.Normalize();
  res *= res * (*this);
  res.Init( x-res.x, y-res.y, z-res.z );
  return res;
}


//------------------------------------------------------------------------------
// \ru Присвоить вектору значения. \en Assign values to vector. 
// ---
inline void MbVector3D::Set( const MbVector3D & v1, double t1 )
{
  x = v1.x * t1;
  y = v1.y * t1;
  z = v1.z * t1;
}


//------------------------------------------------------------------------------
// \ru Присвоить вектору значения. \en Assign values to vector. 
// ---
inline void MbVector3D::Set( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2 )
{
  x = v1.x * t1 + v2.x * t2;
  y = v1.y * t1 + v2.y * t2;
  z = v1.z * t1 + v2.z * t2;
}


//------------------------------------------------------------------------------
// \ru Присвоить вектору значения. \en Assign values to vector. 
// ---
inline void MbVector3D::Set( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2,
                             const MbVector3D & v3, double t3 )
{
  x = v1.x * t1 + v2.x * t2 + v3.x * t3;
  y = v1.y * t1 + v2.y * t2 + v3.y * t3;
  z = v1.z * t1 + v2.z * t2 + v3.z * t3;
}


//------------------------------------------------------------------------------
// \ru Присвоить вектору значения. \en Assign values to vector. 
// ---
inline void MbVector3D::Set( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2,
                             const MbVector3D & v3, double t3, const MbVector3D & v4, double t4 )
{
  x = v1.x * t1 + v2.x * t2 + v3.x * t3 + v4.x * t4;
  y = v1.y * t1 + v2.y * t2 + v3.y * t3 + v4.y * t4;
  z = v1.z * t1 + v2.z * t2 + v3.z * t3 + v4.z * t4;
}


//------------------------------------------------------------------------------
// \ru Добавить к вектору значения. \en Add values to vector. 
// ---
inline void MbVector3D::Add( const MbVector3D & v1, double t1 )
{
  x += v1.x * t1;
  y += v1.y * t1;
  z += v1.z * t1;
}


//------------------------------------------------------------------------------
// \ru Добавить к вектору значения. \en Add values to vector. 
// ---
inline void MbVector3D::Add( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2 )
{
  x += v1.x * t1 + v2.x * t2;
  y += v1.y * t1 + v2.y * t2;
  z += v1.z * t1 + v2.z * t2;
}


//------------------------------------------------------------------------------
// \ru Добавить к вектору значения. \en Add values to vector. 
// ---
inline void MbVector3D::Add( const MbVector3D & v1, double t1,
                             const MbVector3D & v2, double t2,
                             const MbVector3D & v3, double t3 )
{
  x += v1.x * t1 + v2.x * t2 + v3.x * t3;
  y += v1.y * t1 + v2.y * t2 + v3.y * t3;
  z += v1.z * t1 + v2.z * t2 + v3.z * t3;
}


//------------------------------------------------------------------------------
// \ru Добавить к вектору значения. \en Add values to vector. 
// ---
inline void MbVector3D::Add( const MbVector3D & v1, double t1, const MbVector3D & v2, double t2,
                             const MbVector3D & v3, double t3, const MbVector3D & v4, double t4 )
{
  x += v1.x * t1 + v2.x * t2 + v3.x * t3 + v4.x * t4;
  y += v1.y * t1 + v2.y * t2 + v3.y * t3 + v4.y * t4;
  z += v1.z * t1 + v2.z * t2 + v3.z * t3 + v4.z * t4;
}


//------------------------------------------------------------------------------
/// \ru Умножить вектор на число. \en Multiply vector by number. 
// ---
inline MbVector3D operator * ( const MbVector3D & vector, double factor )
{
  return MbVector3D( vector.x * factor, vector.y * factor, vector.z * factor );
}


//------------------------------------------------------------------------------
/// \ru Разделить вектор на число. \en Divide vector by number. 
// ---
inline MbVector3D operator / ( const MbVector3D & vector, double factor )
{
  // \ru Операция деления занимает 40 циклов процессора, а умножения 7, т.е. (/) 5.7 раза медленней (*) \en Division operation takes 40 CPU cycles and multiplication takes only 7, i.e. division 5.7 times slower than multiplication 
  C3D_ASSERT( factor != 0.0 ); //-V550
  double invFactor = ( 1.0 / factor );
  return MbVector3D( vector.x * invFactor, vector.y * invFactor, vector.z * invFactor );
}


//------------------------------------------------------------------------------
/// \ru Умножить вектор на число. \en Multiply vector by number. 
// ---
inline MbVector3D operator * ( double factor, const MbVector3D & vector )
{
  return vector * factor;
}


//------------------------------------------------------------------------------
// \ru Вычислить векторное произведение векторов. \en Calculate vector product of vectors. 
// ---
inline void MbVector3D::SetVecM( const MbVector3D & vF, const MbVector3D & vS )
{
  Init( vF.y * vS.z - vF.z * vS.y,
        vF.z * vS.x - vF.x * vS.z,
        vF.x * vS.y - vF.y * vS.x );
}


//------------------------------------------------------------------------------
// \ru Вычислить векторное произведение векторов, умноженное на число. \en Calculate vector product of vectors multiplied by number. 
// ---
inline void MbVector3D::SetVecM( const MbVector3D & vF, const MbVector3D & vS, double mulKoef )
{
  Init( ( vF.y * vS.z - vF.z * vS.y ) * mulKoef,
        ( vF.z * vS.x - vF.x * vS.z ) * mulKoef,
        ( vF.x * vS.y - vF.y * vS.x ) * mulKoef );
}


//------------------------------------------------------------------------------
// \ru Добавить к вектору векторное произведение векторов. \en Add vector product of vectors to vector. 
// ---
inline void MbVector3D::AddVecM( const MbVector3D & vF, const MbVector3D & vS )
{
  Init ( x + vF.y * vS.z - vF.z * vS.y,
         y + vF.z * vS.x - vF.x * vS.z,
         z + vF.x * vS.y - vF.y * vS.x);
}


//------------------------------------------------------------------------------
// \ru Добавить к вектору векторное произведение векторов, умноженное на число. \en Add vector product of vectors multiplied by number to vector. 
// ---
inline void MbVector3D::AddVecM( const MbVector3D & vF, const MbVector3D & vS, double mulKoef )
{
  Init ( x + (vF.y * vS.z - vF.z * vS.y) * mulKoef,
         y + (vF.z * vS.x - vF.x * vS.z) * mulKoef,
         z + (vF.x * vS.y - vF.y * vS.x) * mulKoef );
}


//-------------------------------------------------------------------------------
// \ru Найти максимальную по модулю компоненту вектора. \en Find the largest absolute value of a vector. 
// ---
inline double MbVector3D::MaxFactor() const
{
  double ax = ::fabs( x );
  double ay = ::fabs( y );
  double az = ::fabs( z );
  return ( ((ax > ay) && (ax > az)) ? ax : ((ay > az) ? ay : az) );
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
inline bool MbVector3D::IsSame( const MbVector3D & other, double accuracy ) const
{
  return ( (::fabs(x - other.x) < accuracy) && 
           (::fabs(y - other.y) < accuracy) && 
           (::fabs(z - other.z) < accuracy) );
}


////////////////////////////////////////////////////////////////////////////////
//
// \ru Глобальные функции \en Global functions 
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
/// \ru Вычислить векторное произведение векторов. \en Calculate vector product of vectors. 
// ---
inline
void SetVecM( MbVector3D & vVec, const MbVector3D & vF, const MbVector3D & vS )
{
  vVec.Init( vF.y * vS.z - vF.z * vS.y,
             vF.z * vS.x - vF.x * vS.z,
             vF.x * vS.y - vF.y * vS.x );
}

//------------------------------------------------------------------------------
/// \ru Вычислить векторное произведение векторов, умноженное на число. \en Calculate vector product of vectors multiplied by number. 
// ---
inline
void SetVecM( MbVector3D & vVec, const MbVector3D & vF, const MbVector3D & vS, double mulKoef )
{
  vVec.Init( ( vF.y * vS.z - vF.z * vS.y ) * mulKoef,
             ( vF.z * vS.x - vF.x * vS.z ) * mulKoef,
             ( vF.x * vS.y - vF.y * vS.x ) * mulKoef );
}


namespace c3d // namespace C3D
{

//------------------------------------------------------------------------------
/** \brief \ru Проверить, что вектор ненулевой с заданной точностью.
           \en Check equality of vector to zero with given tolerance. \~
  \details \ru Проверка ненулевого вектора с заданной точностью.
    Вектор считается ненулевым, если его координаты превосходят заданную погрешность.
           \en Check equality of vector to zero with given tolerance.
    Vector is nonzero if its coordinates are greater than given tolerance. \~
  \param[in] vec - \ru Вектор.
                   \en A vector. \~ 
  \param[in] eps - \ru Погрешность координат.
                   \en Coordinate tolerance. \~
  \return \ru Возвращает true, если вектор ненулевой.
          \en Returns true if the vector is nonzero. \~
  \ingroup Mathematic_Base_3D
*/
// ---
inline
bool Nonzero( const MbVector3D & vec, double eps ) {
  return ::fabs(vec.x) > eps || ::fabs(vec.y) > eps || ::fabs(vec.z) > eps;
}

} // namespace C3D


#endif // __MB_VECTOR3D_H
