////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Расширенная точка с однородными координатами в трёхмерном пространстве.
         \en Extended point with homogeneous coordinates in the three-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_HOMOGENIUS3D_H
#define __MB_HOMOGENIUS3D_H


#include <mb_cart_point3d.h>


//------------------------------------------------------------------------------
/** \brief \ru Расширенная точка с однородными координатами в трёхмерном пространстве.
           \en Extended point with homogeneous coordinates in the three-dimensional space. \~
  \details \ru Расширенная точка с однородными координатами в трёхмерном пространстве. 
    Дополнительная координата точки (вес) вводится для удобства работы с неоднородными 
    рациональными сплайнами.\n
    Определены операции преобразования точки и вектора в однородные координаты. 
    Определены различные арифметические операции однородной точки с числом, декартовой точкой и однородной точкой.
           \en Extended point with homogeneous coordinates in the three-dimensional space. 
    Additional coordinate of a point (weight) is introduced for the convenience of working with non-uniform 
    rational splines. \n
    Operations of transformation of a point and a vector in homogeneous coordinates are defined. 
    Various arithmetic operations of a homogeneous point with a number, a cartesian point and a homogeneous point are defined. \~
  \ingroup Mathematic_Base_3D
*/
// ---
class MATH_CLASS MbHomogeneous3D {

public :
  double x; ///< \ru Первая координата точки. \en A first point coordinate. 
  double y; ///< \ru Вторая координата точки. \en A second point coordinate. 
  double z; ///< \ru Третья координата точки. \en A third point coordinate. 
  double w; ///< \ru Вес точки. \en A point weight. 
  
  static const MbHomogeneous3D zero; ///< \ru Нулевая точка. \en Zero point. 

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbHomogeneous3D() : x( 0.0 ), y( 0.0 ), z( 0.0 ), w( 1.0 ) {}
  /// \ru Конструктор по точке и весу. \en The constructor by a point and a weight. 
  MbHomogeneous3D( const MbVector3D & v, double ww ) : x( v.x * ww ), y( v.y * ww ), z( v.z * ww ), w( ww ) {}
  /// \ru Конструктор копирования. \en Copy constructor. 
  MbHomogeneous3D( const MbHomogeneous3D & v ) : x( v.x ), y( v.y ), z( v.z ), w( v.w ) {}
  /// \ru Конструктор по компонентам точки и весу. \en The constructor by point components and weight. 
  MbHomogeneous3D( double initX, double initY, double initZ, double initW ) : x( initX ), y( initY ), z( initZ ), w( initW ) {}

public:
  /// \ru Инициализация по компонентам точки и весу. \en The initialization by point components and weight. 
  void          Init( double xx, double yy, double zz, double ww ) { x = xx; y = yy; z = zz; w = ww; }
  /// \ru Инициализация по компонентам точки и весу. \en The initialization by point components and weight. 
  void          Init( const MbCartPoint3D & pnt, double weight ) { x = pnt.x*weight; y = pnt.y*weight; z = pnt.z*weight; w = weight; }
  /// \ru Установить вектор нулевой длины. \en Set a vector with null length. 
  void          SetZero() { x = y = z = 0.0; w = 1.0; }

  /// \ru Сделать копию элемента. \en Create a copy of the element. 
  MbHomogeneous3D & Duplicate() const;
  /// \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix. 
  void          Transform( const MbMatrix3D & matr );

  /// \ru Дать вес точки. \en Get a point weight. 
  double        GetWeight() const { return w; }
  /// \ru Вычислить декартовы координаты, как точки. \en Calculate cartesian coordinates as point. 
  void          GetCartPoint( MbCartPoint3D & pnt  ) const;
  /// \ru Вычислить декартовы координаты, как вектора. \en Calculate cartesian coordinates as vector. 
  void          GetVector   ( MbVector3D    & vect ) const;
  /// \ru Преобразовать точку в однородные координаты. \en Transform a point to homogeneous coordinates. 
  void          Set( const MbCartPoint3D & pnt );
  /// \ru Преобразовать точку в однородные координаты. \en Transform a point to homogeneous coordinates. 
  void          Set( const MbCartPoint3D & pnt,  double weight );
  /// \ru Преобразовать вектор в однородные координаты. \en Transform a vector to homogeneous coordinates. 
  void          Set( const MbVector3D    & vect, double weight );
  /**
    \brief \ru Установить по точкам.
           \en Set by points. \~
    \details \ru Координаты точки равны сумме однородных координат исходных точек, а вес - сумме весов.
             \en Coordinates of a point are equal to the sum of homogeneous coordinates of initial points, and weight is equal to the sum of the weights. \~
    \param[in] pnt1, pnt2 - \ru Исходные точки.
                            \en Initial points. \~
    \param[in] weight1, weight2 - \ru Весы точек.
                                  \en Weights of points. \~
  */
  void          Set( const MbCartPoint3D & pnt1, double weight1, 
                     const MbCartPoint3D & pnt2, double weight2 );
  /**
    \brief \ru Добавить точку.
           \en Add a point. \~
    \details \ru Увеличить координаты на значения однородных координат точки.
             \en Increase coordinates by values of homogeneous coordinates of a point. \~
    \param[in] pnt - \ru Исходная точка.
                     \en The initial point. \~
    \param[in] weight - \ru Вес точки.
                        \en A point weight. \~
  */
  void          Add( const MbCartPoint3D & pnt,  double weight );
  /**
    \brief \ru Добавить точки.
           \en Add points. \~
    \details \ru Увеличить координаты на значения однородных координат двух исходных точек.
             \en Increase coordinates by values of the homogeneous coordinates of two initial points. \~
    \param[in] pnt1, pnt2 - \ru Исходные точки.
                            \en Initial points. \~
    \param[in] weight1, weight2 - \ru Весы точек.
                                  \en Weights of points. \~
  */
  void          Add( const MbCartPoint3D & pnt1, double weight1,  
                     const MbCartPoint3D & pnt2, double weight2 ); ///< \ru Добавить точки. \en Add points. 
  /// \ru Разность p2 - p1 умножить на kk. \en Subtract p2 - p1 multiple by kk. 
  void          Dec( const MbHomogeneous3D & p1, const MbHomogeneous3D & p2, double kk ); 
  /// \ru Приравнять координаты вектора координатам точки v1, умноженных на t1. \en Equate coordinates of vector with coordinates of point v1 multiplied by t1. 
  void          Set( const MbHomogeneous3D & v1, double t1 ); 
  /// \ru Приравнять координаты вектора координатам суммы точек v1 и v2, умноженных на t1 и t2 соответственно. \en Equate vector coordinates with sum of points v1 and v2 multiplied with t1 and t2 correspondingly. 
  void          Set( const MbHomogeneous3D & v1, double t1, const MbHomogeneous3D & v2, double t2 );
  /// \ru Приравнять координаты вектора координатам суммы точек v1, v2 и v3, умноженных на t1, t2 и t3 соответственно. \en Equate vector coordinates with coordinates of sum of vectors v1, v2 and v3 multiplied with t1, t2 and t3 correspondingly. 
  void          Set( const MbHomogeneous3D & v1, double t1, const MbHomogeneous3D & v2, double t2, 
                     const MbHomogeneous3D & v3, double t3 );
  /// \ru Приравнять координаты вектора координатам суммы точек v1, v2, v3 и v4, умноженных на t1, t2, t3 и t4 соответственно. \en Equate vector coordinates with coordinates of sum of points v1, v2, v3 and v4 multiplied with t1, t2, t3 and t4 correspondingly. 
  void          Set( const MbHomogeneous3D & v1, double t1, const MbHomogeneous3D & v2, double t2, 
                     const MbHomogeneous3D & v3, double t3, const MbHomogeneous3D & v4, double t4 );
  /// \ru Приравнять координаты вектора координатам суммы точек v1, v2, v3, v4 и v5, умноженных на t1, t2, t3 t4 и t5 соответственно. \en Equate vector coordinates with coordinates of sum of points v1, v2, v3 and v4 multiplied with t1, t2, t3, t4, t5 correspondingly. 
  void          Set( const MbHomogeneous3D & v1, double t1, const MbHomogeneous3D & v2, double t2, 
                     const MbHomogeneous3D & v3, double t3, const MbHomogeneous3D & v4, double t4, const MbHomogeneous3D & v5, double t5 );
  /// \ru Добавить вектор p, умноженный на kk. \en Add vector p multiplied by kk.
  void          Add( const MbHomogeneous3D & v, double t ); 

  /** 
    \ru \name Перегрузка арифметических операций.
    \en \name Overload of arithmetic operations.
    \{ */
  /// \ru Сложить две точки. \en Add two points. 
  MbHomogeneous3D operator +  ( const MbHomogeneous3D & with ) const;
  /// \ru Вычесть из точки точку. \en Subtract a point from the point. 
  MbHomogeneous3D operator -  ( const MbHomogeneous3D & with ) const;
  /// \ru Разделить координаты точки на число. \en Divide point coordinates by a number. 
  MbHomogeneous3D operator /  ( double factor ) const;
  /// \ru Присвоить значение. \en Assign a value. 
  void          operator =  ( const MbHomogeneous3D & );
  /// \ru Умножить координаты точки на число. \en Multiply point coordinates by a number. 
  void          operator *= ( double factor );
  /// \ru Разделить координаты точки на число. \en Divide point coordinates by a number. 
  void          operator /= ( double factor );
  /// \ru Прибавить координаты точки. \en Add coordinates of the point. 
  void          operator += ( const MbHomogeneous3D & with );
  /// \ru Проверить на равенство. \en Check for equality. 
  bool          operator == ( const MbHomogeneous3D & ) const;
  /// \ru Проверить на неравенство. \en Check for inequality. 
  bool          operator != ( const MbHomogeneous3D & ) const;
  /// \ru Являются ли объекты равными? \en Are the objects equal?
  bool          IsSame( const MbHomogeneous3D & other, double accuracy ) const;
  /** \} */

  /// \ru Функция чтения по ссылке. \en Reading function by reference. 
  friend MATH_FUNC (reader &) operator >> ( reader & in,        MbHomogeneous3D & ref );
  /// \ru Функция записи по ссылке. \en Writing function by reference. 
  friend MATH_FUNC (writer &) operator << ( writer & out, const MbHomogeneous3D & ref );
  /// \ru Функция записи по ссылке. \en Writing function by reference. 
  friend MATH_FUNC (writer &) operator << ( writer & out,       MbHomogeneous3D & ref ) { return operator << ( out, (const MbHomogeneous3D &)ref ); }
}; // MbHomogeneous3D


//------------------------------------------------------------------------------
// \ru Вычисление декартовых координат как точки \en Calculation of the cartesian coordinates as point 
// ---
inline void MbHomogeneous3D::GetCartPoint( MbCartPoint3D & pnt ) const {
  if ( w != 0 ) { //-V550
    double r = 1.0 / w;
    pnt.x = x * r;
    pnt.y = y * r;
    pnt.z = z * r;
  }
  else {
    pnt.x = x;
    pnt.y = y;
    pnt.z = z;
  }
}


//------------------------------------------------------------------------------
// \ru Вычисление декартовых координат как точки \en Calculation of the cartesian coordinates as point 
// ---
inline void MbHomogeneous3D::GetVector( MbVector3D & v ) const {
  v.x = x;
  v.y = y;
  v.z = z;
  if ( w != 0 ) { //-V550
    double k = 1.0 / w;
    v.x *= k;
    v.y *= k;
    v.z *= k;
  }
}


//------------------------------------------------------------------------------
// \ru Преобразовать точку в однородные координаты \en Transform a point to homogeneous coordinates 
// ---
inline void MbHomogeneous3D::Set( const MbCartPoint3D & pnt ) {
  x = pnt.x * w;
  y = pnt.y * w;
  z = pnt.z * w;
}


//------------------------------------------------------------------------------
// \ru Преобразовать точку в однородные координаты \en Transform a point to homogeneous coordinates 
// ---
inline void MbHomogeneous3D::Set( const MbCartPoint3D & pnt, double weight ) {
  x = pnt.x * weight;
  y = pnt.y * weight;
  z = pnt.z * weight;
  w = weight;
}


//------------------------------------------------------------------------------
// \ru Преобразовать вектор в однородные координаты \en Transform a vector to homogeneous coordinates 
// ---
inline void MbHomogeneous3D::Set( const MbVector3D & vect, double weight ) {
  x = vect.x * weight;
  y = vect.y * weight;
  z = vect.z * weight;
  w = weight;
}


//------------------------------------------------------------------------------
// \ru Преобразовать точки в однородные координаты \en Transform points to homogeneous coordinates 
// ---
inline void MbHomogeneous3D::Set( const MbCartPoint3D & pnt1, double weight1,
                                  const MbCartPoint3D & pnt2, double weight2 ) {
  x = pnt1.x * weight1 + pnt2.x * weight2;
  y = pnt1.y * weight1 + pnt2.y * weight2;
  z = pnt1.z * weight1 + pnt2.z * weight2;
  w = weight1 + weight2;
}


//------------------------------------------------------------------------------
// \ru Добавить точку \en Add a point 
// ---
inline void MbHomogeneous3D::Add( const MbCartPoint3D & pnt, double weight ) {
  x += pnt.x * weight;
  y += pnt.y * weight;
  z += pnt.z * weight;
  w += weight;
}


//------------------------------------------------------------------------------
// \ru Добавить точки \en Add points 
// ---
inline void MbHomogeneous3D::Add( const MbCartPoint3D & pnt1, double weight1,
                                  const MbCartPoint3D & pnt2, double weight2 ) {
  x += pnt1.x * weight1 + pnt2.x * weight2;
  y += pnt1.y * weight1 + pnt2.y * weight2;
  z += pnt1.z * weight1 + pnt2.z * weight2;
  w += weight1 + weight2;
}


//------------------------------------------------------------------------------
/// \ru Разность p2 - p1 умножить на kk. \en Subtract p2 - p1 multiple by kk. 
// ---
inline void MbHomogeneous3D::Dec( const MbHomogeneous3D & p1, const MbHomogeneous3D & p2, 
                                  double kk ) {
  x = ( p2.x - p1.x ) * kk;
  y = ( p2.y - p1.y ) * kk;
  z = ( p2.z - p1.z ) * kk;
  w = ( p2.w - p1.w ) * kk;
}


//------------------------------------------------------------------------------
/// \ru Приравнять вектору вектор p, умноженному на kk. \en Equate vector with vector p multiplied by k.
// ---
inline void MbHomogeneous3D::Set( const MbHomogeneous3D & v1, double t1 ) 
{
  x = v1.x * t1;
  y = v1.y * t1;
  z = v1.z * t1;
  w = v1.w * t1;
}


//------------------------------------------------------------------------------
// \ru Присвоить вектору значения. \en Assign values to vector. 
// ---
inline void MbHomogeneous3D::Set( const MbHomogeneous3D & v1, double t1, const MbHomogeneous3D & v2, double t2 )
{
  x = v1.x * t1 + v2.x * t2;
  y = v1.y * t1 + v2.y * t2;
  z = v1.z * t1 + v2.z * t2;
  w = v1.w * t1 + v2.w * t2;
}


//------------------------------------------------------------------------------
// \ru Присвоить вектору значения. \en Assign values to vector. 
// ---
inline void MbHomogeneous3D::Set( const MbHomogeneous3D & v1, double t1, const MbHomogeneous3D & v2, double t2,
                                  const MbHomogeneous3D & v3, double t3 )
{
  x = v1.x * t1 + v2.x * t2 + v3.x * t3;
  y = v1.y * t1 + v2.y * t2 + v3.y * t3;
  z = v1.z * t1 + v2.z * t2 + v3.z * t3;
  w = v1.w * t1 + v2.w * t2 + v3.w * t3;
}


//------------------------------------------------------------------------------
// \ru Присвоить вектору значения. \en Assign values to vector. 
// ---
inline void MbHomogeneous3D::Set( const MbHomogeneous3D & v1, double t1, const MbHomogeneous3D & v2, double t2,
                                  const MbHomogeneous3D & v3, double t3, const MbHomogeneous3D & v4, double t4 )
{
  x = v1.x * t1 + v2.x * t2 + v3.x * t3 + v4.x * t4;
  y = v1.y * t1 + v2.y * t2 + v3.y * t3 + v4.y * t4;
  z = v1.z * t1 + v2.z * t2 + v3.z * t3 + v4.z * t4;
  w = v1.w * t1 + v2.w * t2 + v3.w * t3 + v4.w * t4;
}


//------------------------------------------------------------------------------
// \ru Приравнять координаты вектора координатам суммы точек v1, v2, v3, v4 и v5, умноженных на t1, t2, t3 t4, t4 и t5 соответственно. \en Equate vector coordinates with coordinates of sum of points v1, v2, v3 and v4 multiplied with t1, t2, t3, t4, t5 correspondingly. 
// ---
inline void MbHomogeneous3D::Set( const MbHomogeneous3D & v1, double t1, const MbHomogeneous3D & v2, double t2,
                                  const MbHomogeneous3D & v3, double t3, const MbHomogeneous3D & v4, double t4, const MbHomogeneous3D & v5, double t5 )
{
  x = v1.x * t1 + v2.x * t2 + v3.x * t3 + v4.x * t4 + v5.x * t5;
  y = v1.y * t1 + v2.y * t2 + v3.y * t3 + v4.y * t4 + v5.y * t5;
  z = v1.z * t1 + v2.z * t2 + v3.z * t3 + v4.z * t4 + v5.z * t5;
  w = v1.w * t1 + v2.w * t2 + v3.w * t3 + v4.w * t4 + v5.w * t5;
}


//------------------------------------------------------------------------------
// \ru Добавить вектор p, умноженный на kk. \en Add vector p multiplied by kk.
// ---
inline void MbHomogeneous3D::Add( const MbHomogeneous3D & p, double kk ) 
{
  x += p.x * kk;
  y += p.y * kk;
  z += p.z * kk;
  w += p.w * kk;
}


//------------------------------------------------------------------------------
// \ru Присвоение \en Assignment 
// ---
inline void MbHomogeneous3D::operator = ( const MbHomogeneous3D & obj ) {
  x = obj.x;
  y = obj.y;
  z = obj.z;
  w = obj.w;
}

//------------------------------------------------------------------------------
// \ru Сложение \en Addition 
// ---
inline MbHomogeneous3D MbHomogeneous3D::operator + ( const MbHomogeneous3D & with ) const {
  return MbHomogeneous3D( x + with.x, y + with.y, z + with.z, w + with.w );
}


//------------------------------------------------------------------------------
// \ru Вычитание \en Subtraction 
// ---
inline MbHomogeneous3D MbHomogeneous3D::operator - ( const MbHomogeneous3D & with ) const {
  return MbHomogeneous3D( x - with.x, y - with.y, z - with.z, w - with.w );
}


//------------------------------------------------------------------------------
// \ru Умножение на число \en Multiplication by a number 
// ---
inline MbHomogeneous3D operator * ( const MbHomogeneous3D & vector, double factor ) {
  return MbHomogeneous3D( vector.x*factor, vector.y*factor, vector.z*factor, vector.w*factor );
}


//------------------------------------------------------------------------------
// \ru Умножение на число \en Multiplication by a number 
// ---
inline MbHomogeneous3D operator * ( double factor, const MbHomogeneous3D & vector ) {
  return vector * factor;
}


//------------------------------------------------------------------------------
// \ru Деление на число \en Division by a number 
// ---
inline MbHomogeneous3D MbHomogeneous3D::operator / ( double factor ) const {
  if ( factor != 0 ) { //-V550
    double r = 1.0 / factor;
    return MbHomogeneous3D( x * r, y * r, z * r, w * r );
  }
  else {
    return MbHomogeneous3D( x, y, z, w );
  }
}


//------------------------------------------------------------------------------
// \ru Умножение на число \en Multiplication by a number 
// ---
inline void MbHomogeneous3D::operator *= ( double factor ) {
  x *= factor;
  y *= factor;
  z *= factor;
  w *= factor;
}


//------------------------------------------------------------------------------
// \ru Деление на число \en Division by a number 
// ---
inline void MbHomogeneous3D::operator /= ( double factor ) {
  if ( factor != 0 ) { //-V550
    x /= factor;
    y /= factor;
    z /= factor;
    w /= factor;
  }
}


//------------------------------------------------------------------------------
// \ru Сложение \en Addition 
// ---
inline void MbHomogeneous3D::operator += ( const MbHomogeneous3D & with ) {
  x += with.x;
  y += with.y;
  z += with.z;
  w += with.w;
}


//------------------------------------------------------------------------------
// \ru Проверить на равенство. \en Check for equality. 
// ---
inline bool MbHomogeneous3D::operator == ( const MbHomogeneous3D & with ) const
{
  return IsSame( with, Math::region );
}


//------------------------------------------------------------------------------
// \ru Проверить на неравенство. \en Check for inequality. 
// ---
inline bool MbHomogeneous3D::operator != ( const MbHomogeneous3D & with ) const
{
  return !( *this == with );
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
inline bool MbHomogeneous3D::IsSame( const MbHomogeneous3D & other, double accuracy ) const
{
  return ( (::fabs(x - other.x) < accuracy) && 
           (::fabs(y - other.y) < accuracy) && 
           (::fabs(z - other.z) < accuracy) &&
           (::fabs(w - other.w) < accuracy) );
}


#endif // __MB_HOMOGENIUS3D_H
