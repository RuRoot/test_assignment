////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Двумерный вектор.
         \en Two-dimensional vector. \~
  \details \ru Определены классы: двумерный вектор и нормализованный двумерный вектор.
    Также определены функции, находящие различные соотношения между двумя векторами.
           \en Defined classes: two-dimensional vector and normalized two-dimensional vector.
    Also defined functions finding various relations between two vectors. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_VECTOR_H
#define __MB_VECTOR_H


#include <io_base.h>
#include <mb_enum.h>


class  MbDirection;
class  MbCartPoint;
class  MbPlacement;
class  MbHomogeneous;
class  MbMatrix;
class  MbProperties;


//------------------------------------------------------------------------------
/** \brief \ru Двумерный вектор.
           \en Two-dimensional vector. \~
  \details \ru Двумерный вектор. Определены алгебраические и геометрические операции
    для вектора с числом, точкой и другим вектором.
           \en Two-dimensional vector. Defined algebraic and geometric operations
    for vector and number, point or another vector. \~
  \ingroup Mathematic_Base_2D
*/
// ---
class MATH_CLASS MbVector {
public :
  double x; ///< \ru Первая компонента вектора. \en First component of vector. 
  double y; ///< \ru Вторая компонента вектора. \en Second component of vector. 

  static const MbVector zero;   ///< \ru Нулевой вектор. \en Zero vector. 
  static const MbVector xAxis;  ///< \ru Вектор "X" стандартного базиса. \en "X" vector of standard basis. 
  static const MbVector yAxis;  ///< \ru Вектор "Y" стандартного базиса. \en "Y" vector of standard basis. 

  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbVector ()                        : x( 0.0 ),      y( 0.0 )      {}
  /// \ru Конструктор по координатам. \en The constructor by coordinates. 
  MbVector ( double xx, double yy  ) : x( xx  ),      y( yy  )      {}
  /// \ru Конструктор копирования. \en Copy constructor. 
  MbVector ( const MbVector & dir )  : x( dir.x ),    y( dir.y )    {}
  /// \ru Конструктор по двум точкам. \en The constructor by two points. 
  MbVector ( const MbCartPoint & p1, const MbCartPoint & p2 ) { Init( p1, p2 ); }  
  /// \ru Конструктор по точке. \en Constructor by point. 
  MbVector ( const MbCartPoint & p );
  /// \ru Конструктор по углу. \en Constructor by angle. 
  explicit MbVector ( double a )     : x( ::cos(a) ), y( ::sin(a) ) {}
  /// \ru Конструктор по единичному вектору направления. \en Constructor by unit vector of direction. 
  explicit MbVector ( const MbDirection & dir );  

  /// \ru Инициализировать по заданным точкам. \en Initialize by given points. 
  MbVector &  Init( const MbCartPoint & p1, const MbCartPoint & p2 );
  /// \ru Инициализировать по заданным координатам. \en Initialize by given coordinates. 
  MbVector &  Init( double xx, double yy ) { x = xx;  y = yy;  return *this; }
  /// \ru Инициализировать по заданному вектору. \en Initialize by given vector. 
  template <class Vector>
  MbVector &  Init( const Vector & v )   { x = v.x; y = v.y; return *this; }

  /// \ru Обнулить вектор. \en Set vector coordinates to zero. 
  MbVector &  SetZero() { x = y = 0.0; return *this; }
  /// \ru Проверить на равенство. \en Check for equality. 
  bool        operator == ( const MbVector & with ) const;
  /// \ru Проверить на равенство. \en Check for equality. 
  bool        Equal( const MbVector & with ) const;
  /// \ru Проверить на неравенство. \en Check for inequality. 
  bool        operator != ( const MbVector & with ) const;

  /// \ru Вычислить длину вектора. \en Calculate vector length. 
  double      Length() const;
  /// \ru Рассчитать квадрат длины вектора. \en Calculate vector length square 
  double      Length2() const;
  /// \ru Нормализовать вектор. \en Normalize a vector. 
  bool        Normalize();
  /// \ru Вернуть нормализованную копию вектора. \en Return normalized copy of vector. 
  MbVector    GetNormalized() const;
  /// \ru Повернуть вектор на угол angle. \en Rotate vector by an angle 'angle'. 
  MbVector &  Rotate( double angle );
  /// \ru Повернуть вектор на угол, заданный направлением. \en Rotate vector by an angle that defined by direction. 
  MbVector &  Rotate( const MbDirection & angle );
  /// \ru Преобразовать в соответствии с матрицей matr. \en Transform according to matrix 'matr'. 
  MbVector &  Transform( const MbMatrix & matr);
  /// \ru Вычислить угол по нормализованному вектору. \en Calculate an angle by normalized vector. 
  double      DirectionAngle() const;

  /// \ru Сложить два вектора. \en Sum up two vectors. 
  MbVector    operator + ( const MbVector    & ) const;
  /// \ru Сложить вектор и точку. \en Sum up vector and point. 
  MbVector    operator + ( const MbCartPoint & ) const;
  /// \ru Вычесть из вектора вектор. \en Subtract vector from vector. 
  MbVector    operator - ( const MbVector    & ) const;
  /// \ru Вычесть из вектора точку. \en Subtract point from vector. 
  MbVector    operator - ( const MbCartPoint & ) const;
  /// \ru Унарный минус. \en Unary minus. 
  MbVector    operator - () const;
  /// \ru Умножить вектор на число. \en Multiply vector by number. 
  MbVector    operator * ( double factor ) const;
  /// \ru Разделить вектор на число. \en Divide vector by number. 
  MbVector    operator / ( double factor ) const;

  /// \ru Сложить два вектора. \en Sum up two vectors. 
  MbVector &  operator += ( const MbVector & );
  /// \ru Вычесть из вектора вектор. \en Subtract vector from vector. 
  MbVector &  operator -= ( const MbVector & );
  /// \ru Умножить вектор на число. \en Multiply vector by number. 
  MbVector &  operator *= ( double );
  /// \ru Разделить вектор на число. \en Divide vector by number. 
  MbVector &  operator /= ( double );

  /// \ru Скалярное умножение двух векторов. \en Scalar product of two vectors. 
  double      operator * ( const MbVector & ) const;
  /// \ru Скалярное умножение двух векторов. \en Scalar product of two vectors. 
  double      operator * ( const MbDirection & ) const;
  /// \ru Векторное умножение двух векторов. \en Vector product of two vectors. 
  double      operator | ( const MbVector & ) const;
  /// \ru Вычислить вектор как копию данного вектора, преобразованную матрицей. \en Calculate the vector as this copy transformed by the matrix. 
  MbVector    operator * ( const MbMatrix & ) const;

  /// \ru Делает вектор перпендикулярным самому себе, а именно задает вектор (-y, x). \en Makes a vector orthogonal to itself, namely sets the vector (-y, x).
  MbVector &  Perpendicular();
  /// \ru Выдать перпендикуляр к вектору, а именно вектор (-y, x). \en Returns a vector orthogonal to this vector, namely the vector (-y, x).
  MbVector    operator ~ () const;
  /// \ru Присвоить вектору значения другого вектора. \en Assign the value of another vector to the vector. 
  MbVector & operator = ( const MbVector & );
  /// \ru Присвоить вектору значения координат точки. \en Assign point coordinate values to vector. 
  MbVector &  operator = ( const MbCartPoint & );
  /// \ru Присвоить вектору значения однородных координат точки. \en Assign uniform point coordinate values to vector. 
  MbVector &  operator = ( const MbHomogeneous & );
  /// \ru Присвоить вектору значения нормализованного вектора. \en Assign normalized vector values to vector. 
  MbVector &  operator = ( const MbDirection & );
  /// \ru Доступ к координате по индексу. \en Access to a coordinate by an index. 
  double &    operator [] ( size_t i )       { return i ? y : x; };
  /// \ru Значение координаты по индексу. \en The value of a coordinate by an index. 
  double      operator [] ( size_t i ) const { return i ? y : x; };

  /// \ru Количество координат точки. \en The number of point coordinates. 
  static size_t GetDimension() { return 2; }

  /// \ru Дать положение вектора относительно текущего вектора. \en Give vector location relative to current vector. 
  int         Relative( const MbVector & ) const;
  /// \ru Является ли вектор нулевым? \en Check if vector is zero.
  bool        IsZero ( double eps ) const;
  /// \ru Проверить на вырожденность. \en Check for degeneracy. 
  bool        IsDegenerate( double lenEps = Math::LengthEps ) const { return IsZero(lenEps); }
  /// \ru Проверить нормализован ли вектор. \en Check if vector is normalized. 
  bool        IsNormalized( double eps = LENGTH_EPSILON ) const;
  /// \ru Проверить коллинеарность по скалярному произведению. \en Check colinearity by dot product. 
  bool        RoundColinear( const MbVector & with, double eps = Math::paramNear ) const;
  /// \ru Проверить коллинеарность. \en Check colinearity. 
  bool        Colinear     ( const MbVector & with, double eps = Math::AngleEps ) const;
  /// \ru Проверить ортогональность. \en Check orthogonality. 
  bool        Orthogonal    ( const MbVector & with, double eps = Math::AngleEps ) const;
  /// \ru Проверить коллинеарность. \en Check colinearity. 
  bool        operator ||  ( const MbVector & with ) const { return Colinear(with); }

  /// \ru Сменить направление вектора на противоположное. \en Change vector direction to opposite. 
  void        Invert();
  /// \ru Масштабировать компоненты вектора. \en Scale components of vector. 
  void        Scale( double sx, double sy ) { x *= sx, y *= sy; }

  // \ru Присвоение вектору значений \en Assign values to vector 
  /// \ru Приравнять вектору вектор v1, умноженный на t1. \en Equate vector with vector v1 multiplied by t1. 
  void        Set( const MbVector & v1, double t1 );
  /// \ru Приравнять вектору сумму векторов v1 и v2, умноженных на t1 и t2 соответственно. \en Equate vector with sum of vectors v1 and v2 multiplied with t1 and t2 correspondingly. 
  void        Set( const MbVector & v1, double t1, const MbVector & v2, double t2 );
  /// \ru Приравнять вектору сумму векторов v1, v2 и v3, умноженных на t1, t2 и t3 соответственно. \en Equate vector with sum of vectors v1, v2 and v3 multiplied with t1, t2 and t3 correspondingly. 
  void        Set( const MbVector & v1, double t1, const MbVector & v2, double t2, 
                   const MbVector & v3, double t3 );
  /// \ru Приравнять вектору сумму векторов v1, v2, v3 и v4, умноженных на t1, t2, t3 и t4 соответственно. \en Equate vector with sum of vectors v1, v2, v3 and v4 multiplied with t1, t2, t3 and t4 correspondingly. 
  void        Set( const MbVector & v1, double t1, const MbVector & v2, double t2, 
                   const MbVector & v3, double t3, const MbVector & v4, double t4 );
  /// \ru Приравнять вектору сумму нормализованных векторов v1 и v2, умноженных на t1 и t2 соответственно. \en Equate vector with sum of normalized vectors v1 and v2 multiplied with t1 and t2 correspondingly. 
  void        Set( const MbDirection & v1, double t1, const MbDirection & v2, double t2 );
  /// \ru Приравнять координаты вектора координатам суммы точек v1 и v2, умноженных на t1 и t2 соответственно. \en Equate vector coordinates with sum of points v1 and v2 multiplied with t1 and t2 correspondingly. 
  void        Set( const MbCartPoint & v1, double t1, const MbCartPoint & v2, double t2 );
  /// \ru Приравнять координаты вектора координатам суммы точек v1, v2 и v3, умноженных на t1, t2 и t3 соответственно. \en Equate vector coordinates with coordinates of sum of vectors v1, v2 and v3 multiplied with t1, t2 and t3 correspondingly. 
  void        Set( const MbCartPoint & v1, double t1, const MbCartPoint & v2, double t2, 
                   const MbCartPoint & v3, double t3 );
  /// \ru Приравнять координаты вектора координатам суммы точек v1, v2, v3 и v4, умноженных на t1, t2, t3 и t4 соответственно. \en Equate vector coordinates with coordinates of sum of points v1, v2, v3 and v4 multiplied with t1, t2, t3 and t4 correspondingly. 
  void        Set( const MbCartPoint & v1, double t1, const MbCartPoint & v2, double t2, 
                   const MbCartPoint & v3, double t3, const MbCartPoint & v4, double t4 );
  // \ru Добавление вектору значений \en Addition values to a vector 
  /// \ru Прибавить к вектору вектор v1, умноженный на t1. \en Add vector v1 multiplied by t1 to a vector. 
  void        Add( const MbVector & v1, double t1 );
  /// \ru Прибавить к вектору сумму векторов v1 и v2, умноженных на t1 и t2 соответственно. \en Add sum of vectors v1 and v2 multiplied with t1 and t2 correspondingly to a vector. 
  void        Add( const MbVector & v1, double t1, const MbVector & v2, double t2 );
  /// \ru Прибавить к вектору сумму векторов v1, v2 и v3, умноженных на t1, t2 и t3 соответственно. \en Add sum of vectors v1, v2 and v3 multiplied with t1, t2 and t3 correspondingly to a vector. 
  void        Add( const MbVector & v1, double t1, const MbVector & v2, double t2, 
                   const MbVector & v3, double t3 );
  /// \ru Прибавить к вектору сумму векторов v1, v2, v3 и v4, умноженных на t1, t2, t3 и t4 соответственно. \en Add sum of vectors v1, v2, v3 and v4 multiplied with t1, t2, t3 and t4 correspondingly to a vector. 
  void        Add( const MbVector & v1, double t1, const MbVector & v2, double t2, 
                   const MbVector & v3, double t3, const MbVector & v4, double t4 );
  /// \ru Прибавить к вектору сумму единичных векторов v1 и v2, умноженные на t1 и t2 соответственно. \en Add sum of unit vectors v1 and v2 multiplied with t1 and t2 correspondingly to a vector. 
  void        Add( const MbDirection & v1, double t1, const MbDirection & v2, double t2 );

  /// \ru Дать максимальную по модулю компоненту вектора. \en Give the largest absolute value of a vector. 
  double      MaxFactor() const;

  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  void        GetProperties( MbProperties &properties );
  /// \ru Записать свойства объекта. \en Set properties of the object. 
  void        SetProperties( const MbProperties &properties );
  /// \ru Являются ли объекты равными? \en Are the objects equal? 
  bool        IsSame( const MbVector & other, double accuracy ) const;

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbVector, MATH_FUNC_EX )
  DECLARE_NEW_DELETE_CLASS( MbVector )
  DECLARE_NEW_DELETE_CLASS_EX( MbVector )
}; // MbVector


//------------------------------------------------------------------------------
// \ru Длина вектора. \en Vector length. 
// ---
inline double MbVector::Length() const
{
  return ::_hypot( x, y );
}


//------------------------------------------------------------------------------
// \ru Квадрат длины вектора. \en Vector length square.
// ---
inline double MbVector::Length2() const
{
  return ( x * x + y * y );
}


//------------------------------------------------------------------------------
// \ru Нормализация вектора \en Normalize a vector 
// ---
inline bool MbVector::Normalize()
{
  double len = ::_hypot( x, y );
//  C3D_ASSERT( len > LENGTH_EPSILON || len < NULL_EPSILON );
  bool res = ( len >= NULL_EPSILON );
  if ( res && ::fabs(len - 1.0) > NULL_EPSILON ) {
    double one_len = 1.0 / len;
    x *= one_len;
    y *= one_len;
    C3D_ASSERT( ::fabs(1.0 - ::_hypot(x, y)) < LENGTH_EPSILON );
  }
  return res;
}


//------------------------------------------------------------------------------
// \ru Вернуть нормализованную копию вектора. \en Return normalized copy of vector. 
// ---
inline MbVector MbVector::GetNormalized() const
{
  double len = ::_hypot( x, y );
  if ( len >= NULL_EPSILON && ::fabs(len - 1.0) > NULL_EPSILON ) 
    len = 1.0 / len;
  MbVector res( x * len, y * len );
  return res;
}


//------------------------------------------------------------------------------
// \ru Сложение двух векторов; результат - вектор \en Addition of two vectors; result is vector 
// ---
inline MbVector MbVector::operator + ( const MbVector & vector ) const
{
  return MbVector(x + vector.x, y + vector.y);
}


//------------------------------------------------------------------------------
// \ru Вычитание двух векторов; результат - вектор \en Subtraction of two vectors; result is vector 
// ---
inline MbVector MbVector::operator - ( const MbVector & v1 ) const
{
  return MbVector( x - v1.x, y - v1.y );
}


//------------------------------------------------------------------------------
// \ru Унарный минус \en Unary minus 
// ---
inline MbVector MbVector::operator - () const
{
  return MbVector( -x, -y );
}


//------------------------------------------------------------------------------
// \ru Умножение вектора на число \en Multiply vector by number 
// ---
inline MbVector MbVector::operator * ( double factor ) const
{
  return MbVector( x * factor, y * factor );
}


//------------------------------------------------------------------------------
// \ru Умножение скаляра на вектор \en Multiply scalar by vector 
// ---
inline MbVector operator * ( double factor, const MbVector & v )
{
  return v*factor;
}


//------------------------------------------------------------------------------
// \ru Деление вектора на число \en Division of vector by number 
// ---
inline MbVector MbVector::operator / ( double factor ) const
{
  if ( ::fabs(factor) > NULL_EPSILON ) 
    return MbVector( x / factor, y / factor );
  return MbVector( x, y );
}


//------------------------------------------------------------------------------
// \ru Сложение двух векторов \en Addition of two vectors 
// ---
inline MbVector & MbVector::operator += ( const MbVector & vector )
{
  x += vector.x;
  y += vector.y;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Вычитание двух векторов \en Subtraction of two vectors 
// ---
inline MbVector & MbVector::operator -= ( const MbVector & vector )
{
  x -= vector.x;
  y -= vector.y;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Умножение вектора на число \en Multiplication of vector by number 
// ---
inline MbVector & MbVector::operator *= ( double factor )
{
  x *= factor;
  y *= factor;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Деление вектора на число \en Division of vector by number 
// ---
inline MbVector & MbVector::operator /= ( double factor )
{
  if ( ::fabs(factor) > NULL_EPSILON ) {
    x /= factor;
    y /= factor;
  }
  return *this;
}


//------------------------------------------------------------------------------
// \ru Скалярное умножение двух векторов \en Scalar product of two vectors 
// ---
inline double MbVector::operator * ( const MbVector & vector ) const
{
  return x * vector.x + y * vector.y;
}


//------------------------------------------------------------------------------
// \ru Векторное умножение двух векторов \en Vector product of two vectors 
// ---
inline double MbVector::operator | ( const MbVector & vector ) const
{
  return x * vector.y - y * vector.x;
}


//------------------------------------------------------------------------------
// \ru Перпендикуляр к вектору; результат - вектор \en Perpendicular to a vector; result is vector 
// ---
inline MbVector MbVector::operator ~ () const
{
  return MbVector( -y, x );
}


//------------------------------------------------------------------------------
// \ru Перпендикуляр к вектору; результат - вектор \en Perpendicular to a vector; result is vector 
// \ru Поворот влево на 90 градусов \en Rotation to the left by 90 degrees 
// ---
inline MbVector & MbVector::Perpendicular()
{
  double temp = x;
  x = - y;
  y = temp;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Положение вектора относительно текущего вектора. \en Vector location relative to current vector. 
// \ru Возвращает результат : \en Returning result: 
// \ru +1 - слева по направлению; \en +1 - on the left by direction; 
// \ru 0 - вектора коллинеарны; \en 0 - vectors are collinear; 
// \ru -1 - справа по направлению. \en -1 - on the right by direction. 
// ---
inline int MbVector::Relative( const MbVector & rel ) const
{
  double l1 = Length();
  double l2 = rel.Length();

  if ( l1 < Math::LengthEps() || l2 < Math::LengthEps() )
    return 0;

  double yy = (- rel.x * y + rel.y * x) / (l1*l2);
  return ( ::fabs(yy) < Math::LengthEps() ) ? 0 : ( yy > 0.0 ) ? + 1 : - 1;
}

//------------------------------------------------------------------------------
// \ru Проверка на равенство \en Check for equality 
// ---
inline bool MbVector::operator == (const MbVector & with) const
{
  return ( ::fabs( x - with.x ) < Math::LengthEps ) &&
         ( ::fabs( y - with.y ) < Math::LengthEps );
}

//------------------------------------------------------------------------------
// \ru Проверка на равенство \en Check for equality 
// ---
inline bool MbVector::Equal(const MbVector & with ) const
{
  return ( x == with.x && y == with.y ); //-V550
}

//------------------------------------------------------------------------------
// \ru Проверка на неравенство \en Check for inequality 
// ---
inline bool MbVector::operator != (const MbVector & with) const
{
  return !(*this == with);
}

//------------------------------------------------------------------------------
/// \ru Является ли вектор нулевым? \en Check if vector is zero.
// ---
inline bool MbVector::IsZero( double lenEps ) const
{
  return ( fabs(x) < lenEps ) && ( fabs(y) < lenEps );
}

//------------------------------------------------------------------------------
// \ru Является ли вектор нормированным \en Check if  vector is normalized 
// ---
inline bool MbVector::IsNormalized( double eps ) const
{  
  return fabs(Length() - 1.0) < eps;
}

//------------------------------------------------------------------------------
// \ru Сменить направление вектора на противоположное \en Change vector direction to opposite 
// ---
inline void MbVector::Invert()
{
  x = -x;
  y = -y;
}

//------------------------------------------------------------------------------
// \ru Присвоение вектору значений \en Assign values to vector 
// ---
inline void MbVector::Set( const MbVector & v1, double t1 )
{
  x = v1.x * t1;
  y = v1.y * t1;
}

//------------------------------------------------------------------------------
// \ru Присвоение вектору значений \en Assign values to vector 
// ---
inline void MbVector::Set( const MbVector & v1, double t1, const MbVector & v2, double t2 )
{
  x = v1.x * t1 + v2.x * t2;
  y = v1.y * t1 + v2.y * t2;
}

//------------------------------------------------------------------------------
// \ru Присвоение вектору значений \en Assign values to vector 
// ---
inline void MbVector::Set( const MbVector & v1, double t1, const MbVector & v2, double t2,
                           const MbVector & v3, double t3 )
{
  x = v1.x * t1 + v2.x * t2 + v3.x * t3;
  y = v1.y * t1 + v2.y * t2 + v3.y * t3;
}

//------------------------------------------------------------------------------
// \ru Присвоение вектору значений \en Assign values to vector 
// ---
inline void MbVector::Set( const MbVector & v1, double t1, const MbVector & v2, double t2,
                           const MbVector & v3, double t3, const MbVector & v4, double t4 )
{
  x = v1.x * t1 + v2.x * t2 + v3.x * t3 + v4.x * t4;
  y = v1.y * t1 + v2.y * t2 + v3.y * t3 + v4.y * t4;
}

//------------------------------------------------------------------------------
// \ru Добавление вектору значений \en Addition values to a vector 
// ---
inline void MbVector::Add( const MbVector & v1, double t1 )
{
  x += v1.x * t1;
  y += v1.y * t1;
}

//------------------------------------------------------------------------------
// \ru Добавление вектору значений \en Addition values to a vector 
// ---
inline void MbVector::Add( const MbVector & v1, double t1, const MbVector & v2, double t2 )
{
  x += v1.x * t1 + v2.x * t2;
  y += v1.y * t1 + v2.y * t2;
}

//------------------------------------------------------------------------------
// \ru Добавление вектору значений \en Addition values to a vector 
// ---
inline void MbVector::Add( const MbVector & v1, double t1, const MbVector & v2, double t2,
                           const MbVector & v3, double t3 )
{
  x += v1.x * t1 + v2.x * t2 + v3.x * t3;
  y += v1.y * t1 + v2.y * t2 + v3.y * t3;
}

//------------------------------------------------------------------------------
// \ru Добавление вектору значений \en Addition values to a vector 
// ---
inline void MbVector::Add( const MbVector & v1, double t1, const MbVector & v2, double t2,
                           const MbVector & v3, double t3, const MbVector & v4, double t4 )
{
  x += v1.x * t1 + v2.x * t2 + v3.x * t3 + v4.x * t4;
  y += v1.y * t1 + v2.y * t2 + v3.y * t3 + v4.y * t4;
}

//-------------------------------------------------------------------------------
// \ru Максимальная по модулю компонента вектора \en The largest absolute value of a vector 
// ---
inline double MbVector::MaxFactor() const
{
  return std_max( ::fabs( x ), ::fabs( y ) );
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
inline bool MbVector::IsSame( const MbVector & other, double accuracy ) const
{
  return ( (::fabs(x - other.x) < accuracy) && 
           (::fabs(y - other.y) < accuracy) );
}

////////////////////////////////////////////////////////////////////////////////
//
// \ru Двумерный нормализованный вектор. \en Two-dimensional normalized vector. 
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
/** \brief \ru Двумерный нормализованный вектор.
           \en Two-dimensional normalized vector. \~
  \details \ru Двумерный нормализованный вектор. Определены алгебраические и геометрические операции
    для нормализованного вектора с числом и другим вектором.
           \en Two-dimensional normalized vector. Defined algebraic and geometric operations
    for normalized vector and number or another vector. \~
  \ingroup Mathematic_Base_2D
*/
// ---
class MATH_CLASS MbDirection {
public :
  double ax; ///< \ru Первая компонента вектора. \en First component of vector. 
  double ay; ///< \ru Вторая компонента вектора. \en Second component of vector. 

  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbDirection ()                          : ax( 1.0 ), ay( 0.0 ) {}
  /// \ru Конструктор по координатам. \en The constructor by coordinates. 
  MbDirection ( double _ax, double _ay )  : ax( _ax ), ay( _ay ) { Normalize(); }
  /// \ru Конструктор по углу. \en Constructor by angle. 
  explicit MbDirection ( double a )       : ax( ::cos(a) ), ay( ::sin(a) ) {}
  /// \ru Конструктор копирования. \en Copy constructor. 
  MbDirection ( const MbDirection & dir ) : ax( dir.ax ), ay( dir.ay ) {}
  /// \ru Конструктор по двум точкам. \en Constructor by two points. 
  MbDirection ( const MbCartPoint & p1, const MbCartPoint & p2 ) { Calculate( p1, p2 ); }  

  /// \ru Повернуть нормализованный вектор на угол angle. \en Rotate normalized vector by an angle 'angle'. 
  void        Rotate( double angle );
  /// \ru Повернуть вектор на угол, заданный направлением. \en Rotate vector by an angle that defined by direction. 
  void        Rotate( const MbDirection & );
  /// \ru Преобразовать в соответствии с матрицей matr. \en Transform according to matrix 'matr'. 
  void        Transform( const MbMatrix & );
  /// \ru Вычислить угол по нормализованному вектору. \en Calculate an angle by normalized vector. 
  double      DirectionAngle() const;
  /// \ru Вычислить направление по 2 точкам. \en Calculate direction by two points. 
  void        Calculate( const MbCartPoint & from, const MbCartPoint & to);
  /// \ru Нормализовать вектор. \en Normalize a vector. 
  void        Normalize();

  /// \ru Унарный минус. \en Unary minus. 
  MbDirection operator - () const;
  /// \ru Сложить два нормализованных вектора. \en Sum up two normalized vectors. 
  MbDirection operator + ( const MbDirection & ) const;
  /// \ru Найти перпендикуляр к нормализованному вектору. \en Find perpendicular to a normalized vector. 
  MbDirection operator ~ () const;

  /// \ru Найти перпендикуляр к нормализованному вектору. \en Find perpendicular to a normalized vector. 
  void        Perpendicular();

  /// \ru Проверить на равенство. \en Check for equality. 
  bool        operator == ( const MbDirection & ) const;
  /// \ru Проверить на равенство. \en Check for equality. 
  bool        Equal( const MbDirection & ) const;
  /// \ru Проверить на неравенство. \en Check for inequality. 
  bool        operator != ( const MbDirection & ) const;

  /// \ru Проверить коллинеарность. \en Check colinearity. 
  bool        Colinear ( const MbDirection &, double eps = Math::AngleEps ) const;
  /// \ru Проверить ортогональность. \en Check orthogonality. 
  bool        Orthogonal( const MbDirection &, double eps = Math::AngleEps ) const;
  /// \ru Проверить коллинеарность. \en Check colinearity. 
  bool        Colinear ( const MbVector & with, double eps = Math::AngleEps ) const;
  /// \ru Проверить ортогональность. \en Check orthogonality. 
  bool        Orthogonal( const MbVector & with, double eps = Math::AngleEps ) const;
  /// \ru Сменить направление вектора на противоположное. \en Change vector direction to opposite. 
  void        Invert();

  /// \ru Присвоить значение вектору по заданному углу. \en Assign value to vector by given angle. 
  void        operator = ( double angle ) { Set( angle ); }
  /// \ru Присвоить значение вектору по заданному вектору. \en Assign value to vector by given vector. 
  void        operator = ( const MbVector & );
  /// \ru Присвоить значение вектору по заданной точке. \en Assign value to vector by given point. 
  void        operator = ( const MbCartPoint & );
  /// \ru Умножить вектор на число. \en Multiply vector by number. 
  MbVector    operator * ( double ) const;
  /// \ru Сложить два вектора. \en Sum up two vectors. 
  void        operator += ( const MbDirection & );
  /// \ru Найти разность векторов. \en Find the difference of vectors. 
  void        operator -= ( const MbDirection & );
//  void        operator *= ( double );

  /// \ru Скалярное умножение двух векторов. \en Scalar product of two vectors. 
  double      operator * ( const MbDirection & ) const;
  /// \ru Векторное умножение двух векторов. \en Vector product of two vectors. 
  double      operator | ( const MbDirection & ) const;

  /// \ru Инициализировать по координатам. \en Initialize by coordinates. 
  void        Init( double xx, double yy ) { ax = xx; ay = yy; Normalize(); }
  /// \ru Инициализировать по координатам другого вектора. \en Initialize by coordinates of a vector. 
  void        Init( const MbDirection & dir ) { ax = dir.ax; ay = dir.ay; }
  /// \ru Инициализировать по заданному углу. \en Initialize by given angle. 
  void        Set( double angle ) { ax = ::cos( angle ); ay = ::sin( angle ); }

  /// \ru Найти положение вектора относительно текущего вектора. \en Find vector location relative to current vector. 
  int         Relative( const MbDirection & ) const;
  /// \ru Проверить на вырожденность. \en Check for degeneracy. 
  bool        IsDegenerate( double lenEps = Math::LengthEps ) const;

  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  void        GetProperties( MbProperties &properties );
  /// \ru Записать свойства объекта. \en Set properties of the object. 
  void        SetProperties( const MbProperties &properties );
  /// \ru Являются ли объекты равными? \en Are the objects equal? 
  bool        IsSame( const MbDirection & other, double accuracy ) const;

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbDirection, MATH_FUNC_EX )
  DECLARE_NEW_DELETE_CLASS( MbDirection )
  DECLARE_NEW_DELETE_CLASS_EX( MbDirection )
}; // MbDirection


/** \} */


//------------------------------------------------------------------------------
// \ru Нормализация вектора \en Normalize a vector 
// ---
inline void MbDirection::Normalize()
{
  double len = ::_hypot( ax, ay );

  if ( len >= NULL_EPSILON ) {
    double k = 1.0 / len;
    ax *= k;
    ay *= k;
  }
}


//------------------------------------------------------------------------------
// \ru Конструктор по нормализованному вектору \en Constructor by a normalized vector 
// ---
inline MbVector::MbVector( const MbDirection & dir )
{
  x = dir.ax;
  y = dir.ay;
}


//------------------------------------------------------------------------------
// \ru Присвоить вектору значения другого вектора. \en Assign the value of another vector to the vector. 
// ---
inline MbVector & MbVector::operator = ( const MbVector & v )
{
  x = v.x;
  y = v.y;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Присвоение вектору значений нормализованного вектора \en Assign normalized vector values to vector 
// ---
inline MbVector & MbVector::operator = ( const MbDirection & dir )
{
  x = dir.ax;
  y = dir.ay;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Скалярное умножение двух векторов \en Scalar product of two vectors 
// ---
inline double MbVector::operator * ( const MbDirection & vector ) const
{
  return x * vector.ax + y * vector.ay;
}


//------------------------------------------------------------------------------
// \ru Присвоение вектору значений \en Assign values to vector 
// ---
inline void MbVector::Set( const MbDirection & v1, double t1, const MbDirection & v2, double t2 )
{
  x = v1.ax * t1 + v2.ax * t2;
  y = v1.ay * t1 + v2.ay * t2;
}


//------------------------------------------------------------------------------
// \ru Добавление вектору значений \en Addition values to a vector 
// ---
inline void MbVector::Add( const MbDirection & v1, double t1, const MbDirection & v2, double t2 )
{
  x += v1.ax * t1 + v2.ax * t2;
  y += v1.ay * t1 + v2.ay * t2;
}


//------------------------------------------------------------------------------
// \ru Присвоение вектора \en Assignment of vector 
// ---
inline void MbDirection::operator = ( const MbVector & vect )
{
  double d = vect.Length();

  if ( d >= NULL_EPSILON ) {
    ax = vect.x / d;
    ay = vect.y / d;
  }
  else {
    ax = ay = 0.0;
  }
}


//------------------------------------------------------------------------------
// \ru Унарный минус \en Unary minus 
// ---
inline MbDirection MbDirection::operator - () const
{
  return MbDirection( - ax, - ay );
}


//------------------------------------------------------------------------------
// \ru Сложение двух нормализованных векторов \en Addition of two normalized vectors 
// \ru Результат - нормализованный вектор \en Result is normalized vector 
// ---
inline MbDirection MbDirection::operator + ( const MbDirection & dir ) const
{
  double xx = ax + dir.ax;
  double yy = ay + dir.ay;
  double d = ::_hypot( xx, yy );

  if ( d <= NULL_EPSILON ) 
    return MbDirection( ax, ay );

  double k = 1.0 / d;

  return MbDirection( xx*k, yy*k );
}


//------------------------------------------------------------------------------
// \ru Перпендикуляр к нормализованному вектору \en Perpendicular to a normalized vector 
// ---
inline MbDirection MbDirection::operator ~ () const
{
  return MbDirection( - ay, ax );
}


//------------------------------------------------------------------------------
// \ru Перпендикуляр к нормализованному вектору \en Perpendicular to a normalized vector 
// ---
inline void MbDirection::Perpendicular()
{
  double temp = ax;
  ax = - ay;
  ay = temp;
}


//------------------------------------------------------------------------------
// \ru Умножение вектора на число \en Multiplication of vector by number 
// ---
inline MbVector MbDirection::operator * ( double factor ) const
{
  return MbVector( ax * factor, ay * factor );
}


//------------------------------------------------------------------------------
// \ru Сложение двух векторов \en Addition of two vectors 
// ---
inline void MbDirection::operator += ( const MbDirection & dir )
{
  double xx = ax + dir.ax;
  double yy = ay + dir.ay;
  double d = ::_hypot( xx, yy );
  if ( d > NULL_EPSILON ) {
    ax = xx / d;
    ay = yy / d;
  }
}


//------------------------------------------------------------------------------
// \ru Вычитание двух векторов \en Subtraction of two vectors 
// ---
inline void MbDirection::operator -= ( const MbDirection & dir )
{
  double xx = ax - dir.ax;
  double yy = ay - dir.ay;
  double d = ::_hypot( xx, yy );
  if ( d > NULL_EPSILON ) {
    ax = xx/d;
    ay = yy/d;
  }
}


//------------------------------------------------------------------------------
// \ru Оператор \en Operator 
// ---
//inline void MbDirection::operator *= ( double factor )
//{
//  ax *= factor;
//  ay *= factor;
//}


//------------------------------------------------------------------------------
// \ru Скалярное умножение двух векторов \en Scalar product of two vectors 
// ---
inline double MbDirection::operator * ( const MbDirection & dir2 ) const
{
  return ax * dir2.ax + ay * dir2.ay;
}


//------------------------------------------------------------------------------
// \ru Векторное умножение двух векторов \en Vector product of two vectors 
// ---
inline double MbDirection::operator | ( const MbDirection & dir2 ) const
{
  return ax * dir2.ay - ay * dir2.ax;
}


//------------------------------------------------------------------------------
// \ru Проверка на равенство \en Check for equality 
// ---
inline bool MbDirection::operator == ( const MbDirection & with) const
{
  return ( ::fabs( ax - with.ax ) < Math::LengthEps ) &&
         ( ::fabs( ay - with.ay ) < Math::LengthEps );
}


//------------------------------------------------------------------------------
// \ru Проверка на равенство \en Check for equality 
// ---
inline bool MbDirection::Equal( const MbDirection & with ) const
{
  return ( ax == with.ax && ay == with.ay ); //-V550
}


//------------------------------------------------------------------------------
// \ru Проверка на неравенство \en Check for inequality 
// ---
inline bool MbDirection::operator != ( const MbDirection & with) const
{
  return !(*this == with);
}


//------------------------------------------------------------------------------
// \ru Проверка коллинеарности \en Check for colinearity 
// ---
inline bool MbDirection::Colinear( const MbDirection & with, double eps ) const
{
  return (::fabs( ax * with.ay - ay * with.ax ) < eps);
}


//------------------------------------------------------------------------------
// \ru Проверка ортогональности \en Check for orthogonality 
// ---
inline bool MbDirection::Orthogonal( const MbDirection & with, double eps ) const
{
  return (::fabs(*this * with) <= eps);
}


//------------------------------------------------------------------------------
// \ru Проверка коллинеарности \en Check for colinearity 
// ---
inline bool MbDirection::Colinear( const MbVector & with, double eps ) const
{
  MbVector vect( *this );
  return vect.Colinear( with, eps );
}


//------------------------------------------------------------------------------
// \ru Проверка ортогональности \en Check for orthogonality 
// ---
inline bool MbDirection::Orthogonal( const MbVector & with, double eps ) const
{
  MbVector vect( *this );
  return vect.Orthogonal( with, eps );
}


//------------------------------------------------------------------------------
// \ru Проверка на вырожденность \en Check for degeneracy 
// ---
inline bool MbDirection::IsDegenerate( double lenEps ) const
{
  return ( ::fabs( ax ) < lenEps ) && 
         ( ::fabs( ay ) < lenEps );
}


//------------------------------------------------------------------------------
// \ru Сменить направление вектора на противоположное \en Change vector direction to opposite 
// ---
inline void MbDirection::Invert()
{
  ax = -ax;
  ay = -ay;
}


//------------------------------------------------------------------------------
// \ru Положение вектора относительно текущего вектора \en Vector location relative to current vector 
// \ru Возвращает результат : \en Returning result: 
// \ru +1 - слева по направлению; \en +1 - on the left by direction; 
// \ru 0 - вектора коллинеарны; \en 0 - vectors are collinear; 
// \ru -1 - справа по направлению. \en -1 - on the right by direction. 
// ---
inline int MbDirection::Relative( const MbDirection & rel ) const
{
  double y = - rel.ax * ay + rel.ay * ax;
  return ( ::fabs( y ) < Math::LengthEps() ) ? 0 : ( y > 0 ) ? + 1 : - 1;
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
inline bool MbDirection::IsSame( const MbDirection & other, double accuracy ) const
{
  return ( (::fabs(ax - other.ax) < accuracy) && 
           (::fabs(ay - other.ay) < accuracy) );
}


////////////////////////////////////////////////////////////////////////////////
//
// \ru Глобальные функции \en Global functions 
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
/** \brief \ru Вычисление минимального угла между двумя векторами
           \en Calculate minimal angle between two vectors \~
  \details \ru Вычисление угла между двумя векторами (-пи ... пи)
    Возвращает результат со знаком:\n
    <0 находится слева от вектора v1\n
    >0 находится справа от вектора v2
           \en Calculate angle between two vectors (-pi ... pi)
    Returns signed result:\n
    <0 to the left of vector v1\n
    >0 to the right of vector v2 \~
  \note \ru Нормализация векторов не требуется
        \en Normalization of vectors is not required \~
  \ingroup Mathematic_Base_2D
*/
inline
double Angle2Vectors( const MbVector & v1, const MbVector & v2 )
{
  if ( v1.MaxFactor() < NULL_REGION || v2.MaxFactor() < NULL_REGION )
    return 0;  
  return ::atan2( v1|v2/*y*/, v1*v2/*x*/ );
}

//------------------------------------------------------------------------------
/// \ru Вычислить "векторное" произведение двух векторов. \en Calculate "vector" product of two vectors. 
// ---
inline
double SetVecM( const MbVector & vF, const MbVector & vS ) {
  return (vF.x * vS.y) - (vF.y * vS.x);
}


namespace c3d // namespace C3D
{

//------------------------------------------------------------------------------
/// \ru Вектор, повернутый на pi/2 радиан против часовой стрелки. \en Vector rotated by pi/2 radians counterclockwise.
//---
inline
MbVector Perpendicular( MbVector vec ) {
  return vec.Perpendicular();  
}

//------------------------------------------------------------------------------
/// \ru Проверить на равенство нулю длину вектора с заданной точностью. \en Check vector length to be equaled to zero with given tolerance. 
// ---
inline
bool IsNull( const MbVector & vec, double eps ) {
  return ::fabs(vec.x) < eps && ::fabs(vec.y) < eps;
}

} // namespace C3D

#endif // __MB_VECTOR_H
