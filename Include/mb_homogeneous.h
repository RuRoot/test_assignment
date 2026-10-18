////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Расширенная точка с однородными координатами в двумерном пространстве.
         \en Extended point with homogeneous coordinates in the two-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_HOMOGENIUS_H
#define __MB_HOMOGENIUS_H


#include <mb_cart_point.h>


class   MbMatrix;


//------------------------------------------------------------------------------
/** \brief \ru Расширенная точка с однородными координатами в двумерном пространстве.
           \en Extended point with homogeneous coordinates in the two-dimensional space. \~
  \details \ru Расширенная точка с однородными координатами в двумерном пространстве. \n
    Дополнительная координата точки (вес) вводится для удобства работы с неоднородными рациональными сплайнами.\n
    Определены операции преобразования точки и вектора в однородные координаты. 
    Определены различные арифметические операции однородной точки с числом, декартовой точкой и однородной точкой.
           \en Extended point with homogeneous coordinates in the two-dimensional space. \n
    Additional coordinate of a point (weight) is introduced for the convenience of working with non-uniform rational splines. \n
    Operations of transformation of a point and a vector in homogeneous coordinates are defined. 
    Various arithmetic operations of a homogeneous point with a number, a Cartesian point and a homogeneous point are defined. \~
  \ingroup Mathematic_Base_2D
*/
// ---
class MATH_CLASS MbHomogeneous {
public :
  double x; ///< \ru Первая координата точки. \en A first point coordinate. 
  double y; ///< \ru Вторая координата точки. \en A second point coordinate. 
  double w; ///< \ru Вес точки. \en A point weight. 
  
  static const MbHomogeneous zero;   ///< \ru Нулевая точка. \en Zero point. 

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbHomogeneous() : x( 0.0 ), y( 0.0 ), w( 1.0 ) {}
  /// \ru Конструктор по точке и весу. \en The constructor by a point and a weight. 
  MbHomogeneous( const MbVector & v, double ww ) : x( v.x * ww ), y( v.y * ww ), w( ww ) {}
  /// \ru Конструктор копирования. \en The copy constructor. 
  MbHomogeneous( const MbHomogeneous & other ) : x( other.x ), y( other.y ), w( other.w ) {}
  /// \ru Конструктор по компонентам точки и весу. \en The constructor by point components and weight. 
  MbHomogeneous( double initX, double initY, double initW ) : x( initX ), y( initY ), w( initW ) {}

public:
  /// \ru Инициализация по компонентам точки. \en The initialization by point components. 
  void          Init( double initX, double initY );
  /// \ru Инициализация по компонентам точки и весу. \en The initialization by point components and weight. 
  void          Init( double initX, double initY, double initW );
  /// \ru Инициализация по компонентам точки и весу. \en The initialization by point components and weight. 
  void          Init( const MbCartPoint & pnt, double weight ) { x = pnt.x*weight; y = pnt.y*weight; w = weight; }
  /// \ru Установить вектор нулевой длины. \en Set a vector with null length. 
  void          SetZero() { x = 0; y = 0; w = 1; }

  /// \ru Преобразование по матрице. \en The transformation by a matrix. 
  void          Transform( const MbMatrix & matr );
  /// \ru Сдвинуть вдоль вектора. \en Translate along a vector. 
  void          Move( const MbVector & shift );
  /**
    \brief \ru Повернуть на угол.
           \en Rotate by an angle. \~
    \details \ru Угол определяет вектор вращения, а точка - центр.
             \en An angle defines a rotation vector and a point defines a center. \~
    \param[in] pnt - \ru Точка.
                     \en A point. \~
    \param[in] angle - \ru Угол вращения.
                       \en A rotation angle. \~
  */
  void          Rotate( const MbCartPoint & pnt, double angle );

  /// \ru Дать вес точки. \en Get a point weight. 
  double        GetWeight() const { return w; };
  /// \ru Вычислить декартовы координаты, как точки. \en Calculate cartesian coordinates as point. 
  void          GetCartPoint( MbCartPoint & pnt  ) const;
  /// \ru Вычислить декартовы координаты, как вектора. \en Calculate cartesian coordinates as vector. 
  void          GetVector   ( MbVector    & vect ) const;

  /// \ru Преобразовать точку в однородные координаты. \en Transform a point to homogeneous coordinates. 
  void          Set( const MbCartPoint & pnt );
  /// \ru Преобразовать точку в однородные координаты. \en Transform a point to homogeneous coordinates. 
  void          Set( const MbCartPoint & pnt,  double weight );
  /// \ru Преобразовать вектор в однородные координаты. \en Transform a vector to homogeneous coordinates. 
  void          Set( const MbVector    & pnt,  double weight );
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
  void          Set( const MbCartPoint & pnt1, double weight1,  
                     const MbCartPoint & pnt2, double weight2 );
  /**
    \brief   \ru Увеличить координаты на указанные значения.
             \en Increase coordinates by given values. \~
    \details \ru Увеличить координаты на указанные значения.
             \en Increase coordinates by given values. \~
    \param[in] initX -  \ru Исходное значение координаты x.
                        \en The initial value of x. \~
    \param[in] initY -  \ru Исходное значение координаты y.
                        \en The initial value of y. \~
    \param[in] weight - \ru Вес.
                        \en A weight. \~
  */
  void          Add( double initX, double initY,  double weight );
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
  void          Add( const MbCartPoint & pnt1, double weight1,  
                     const MbCartPoint & pnt2, double weight2 );
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
  void          Add( const MbCartPoint & pnt, double weight );

  /// \ru Разность p2 - p1 умножить на kk. \en Subtract p2 - p1 multiple by kk. 
  void          Dec( const MbHomogeneous & p1, const MbHomogeneous & p2, double kk ); 
  /// \ru Приравнять координаты вектора координатам точки v1, умноженных на t1. \en Equate coordinates of vector with coordinates of point v1 multiplied by t1. 
  void          Set( const MbHomogeneous & v1, double t1 ); 
  /// \ru Приравнять координаты вектора координатам суммы точек v1 и v2, умноженных на t1 и t2 соответственно. \en Equate vector coordinates with sum of points v1 and v2 multiplied with t1 and t2 correspondingly. 
  void          Set( const MbHomogeneous & v1, double t1, const MbHomogeneous & v2, double t2 );
  /// \ru Приравнять координаты вектора координатам суммы точек v1, v2 и v3, умноженных на t1, t2 и t3 соответственно. \en Equate vector coordinates with coordinates of sum of vectors v1, v2 and v3 multiplied with t1, t2 and t3 correspondingly. 
  void          Set( const MbHomogeneous & v1, double t1, const MbHomogeneous & v2, double t2, 
                     const MbHomogeneous & v3, double t3 );
  /// \ru Приравнять координаты вектора координатам суммы точек v1, v2, v3 и v4, умноженных на t1, t2, t3 и t4 соответственно. \en Equate vector coordinates with coordinates of sum of points v1, v2, v3 and v4 multiplied with t1, t2, t3 and t4 correspondingly. 
  void          Set( const MbHomogeneous & v1, double t1, const MbHomogeneous & v2, double t2, 
                     const MbHomogeneous & v3, double t3, const MbHomogeneous & v4, double t4 );
  /// \ru Приравнять координаты вектора координатам суммы точек v1, v2, v3, v4 и v5, умноженных на t1, t2, t3 t4 и t5 соответственно. \en Equate vector coordinates with coordinates of sum of points v1, v2, v3 and v4 multiplied with t1, t2, t3, t4, t5 correspondingly. 
  void          Set( const MbHomogeneous & v1, double t1, const MbHomogeneous & v2, double t2, 
                     const MbHomogeneous & v3, double t3, const MbHomogeneous & v4, double t4, const MbHomogeneous & v5, double t5 );
  /// \ru Добавить вектор p, умноженный на kk. \en Add vector p multiplied by kk.
  void          Add( const MbHomogeneous & p, double kk ); 

  /** 
    \ru \name Перегрузка арифметических операций.
    \en \name Overload of arithmetic operations.
    \{ */
  /// \ru Сложить две точки. \en Add two points. 
  MbHomogeneous operator + ( const MbHomogeneous & with ) const;
  /// \ru Вычесть из точки точку. \en Subtract a point from the point. 
  MbHomogeneous operator - ( const MbHomogeneous & with ) const;
  /// \ru Умножить координаты точки на число. \en Multiply point coordinates by a number. 
  MbHomogeneous operator * ( double factor ) const;
  /// \ru Разделить координаты точки на число. \en Divide point coordinates by a number. 
  MbHomogeneous operator / ( double factor ) const;
  /// \ru Скалярное умножение двух векторов. \en Scalar product of two vectors. 
  double        operator * ( const MbHomogeneous & vector ) const;
  /// \ru Векторное умножение двух векторов. \en Vector product of two vectors. 
  MbHomogeneous operator | ( const MbHomogeneous & vect2 ) const;
  /// \ru Присвоить значение. \en Assign a value. 
  void          operator = ( const MbHomogeneous & other ) { x = other.x; y = other.y; w = other.w; }
  /// \ru Умножить координаты точки на число. \en Multiply point coordinates by a number. 
  void          operator *= ( double factor );
  /// \ru Разделить координаты точки на число. \en Divide point coordinates by a number. 
  void          operator /= ( double factor );
  /// \ru Проверить на равенство. \en Check for equality. 
  bool          operator == ( const MbHomogeneous & ) const;
  /// \ru Проверить на неравенство. \en Check for inequality. 
  bool          operator != ( const MbHomogeneous & ) const;
  /// \ru Являются ли объекты равными? \en Are the objects equal?
  bool          IsSame( const MbHomogeneous & other, double accuracy ) const;
  /** \} */
}; // MbHomogeneous


//------------------------------------------------------------------------------
// \ru Инициализация \en The initialization.  
// ---
inline
void MbHomogeneous::Init( double initX, double initY )
{
  x = initX;
  y = initY;
}


//------------------------------------------------------------------------------
// \ru Инициализация \en The initialization.  
// ---
inline
void MbHomogeneous::Init( double initX, double initY, double initW )
{
  x = initX;
  y = initY;
  w = initW;
}


//------------------------------------------------------------------------------
// \ru Вычисление декартовых координат \en Calculation of the cartesian coordinates 
// ---
inline
void MbHomogeneous::GetCartPoint( MbCartPoint & pnt ) const
{
  pnt.x = x;
  pnt.y = y;
  if ( w != 0 ) {
    double k = 1.0 / w;
    pnt.x *= k;
    pnt.y *= k;
  }
}


//------------------------------------------------------------------------------
// \ru Вычисление декартовых координат как вектора \en Calculation of the cartesian coordinates as vector 
// ---
inline
void MbHomogeneous::GetVector( MbVector & v ) const
{
  v.x = x;
  v.y = y;
  if ( w != 0 ) {
    double k = 1.0 / w;
    v.x *= k;
    v.y *= k;
  }
}


//------------------------------------------------------------------------------
// \ru Преобразовать точку в однородные координаты \en Transform a point to homogeneous coordinates 
// ---
inline
void MbHomogeneous::Set( const MbCartPoint & pnt ) {
  Init( pnt.x * w, pnt.y * w );
}


//------------------------------------------------------------------------------
// \ru Преобразовать точку в однородные координаты \en Transform a point to homogeneous coordinates 
// ---
inline
void MbHomogeneous::Set( const MbCartPoint & pnt, double weight ) {
  Init( pnt.x * weight, pnt.y * weight, weight );
}


//------------------------------------------------------------------------------
// \ru Преобразовать вектор в однородные координаты \en Transform a vector to homogeneous coordinates 
// ---
inline
void MbHomogeneous::Set( const MbVector & vect, double weight ) {
  Init( vect.x * weight, vect.y * weight, weight );
}


//------------------------------------------------------------------------------
// \ru Увеличить координаты на указанные значения. \en Increase coordinates by given values.
// ---
inline
void MbHomogeneous::Add( double initX, double initY, double weight )
{
  x += initX;
  y += initY;
  w += weight;
}


//------------------------------------------------------------------------------
// \ru Преобразовать точки в однородные координаты \en Transform points to homogeneous coordinates 
// ---
inline
void MbHomogeneous::Set( const MbCartPoint & pnt1, double weight1,
                                const MbCartPoint & pnt2, double weight2 ) 
{
  Init( pnt1.x * weight1 + pnt2.x * weight2, 
        pnt1.y * weight1 + pnt2.y * weight2, 
        weight1 + weight2 );
}


//------------------------------------------------------------------------------
// \ru Добавить точку \en Add a point 
// ---
inline
void MbHomogeneous::Add( const MbCartPoint & pnt, double weight ) {
  Add( pnt.x * weight, pnt.y * weight, weight );
}


//------------------------------------------------------------------------------
// \ru Добавить точки \en Add points 
// ---
inline
void MbHomogeneous::Add( const MbCartPoint & pnt1, double weight1,
                         const MbCartPoint & pnt2, double weight2 ) 
{
  Add( pnt1.x * weight1 + pnt2.x * weight2,
       pnt1.y * weight1 + pnt2.y * weight2,
       weight1 + weight2 );
}


//------------------------------------------------------------------------------
// \ru Разность p2 - p1 умножить на kk. \en Subtract p2 - p1 multiple by kk. 
// ---
inline
void MbHomogeneous::Dec( const MbHomogeneous & p1, const MbHomogeneous & p2, double kk )
{
  Init( (p2.x - p1.x) * kk,
        (p2.y - p1.y) * kk,
        (p2.w - p1.w) * kk );
}


//------------------------------------------------------------------------------
// \ru Приравнять вектору вектор p, умноженному на kk. \en Equate vector with vector p multiplied by k.
// ---
inline
void MbHomogeneous::Set( const MbHomogeneous & v, double t ) {
  Init( v.x * t, v.y * t, v.w * t );
}


//------------------------------------------------------------------------------
// \ru Присвоить вектору значения. \en Assign values to vector. 
// ---
inline
void MbHomogeneous::Set( const MbHomogeneous & v1, double t1, const MbHomogeneous & v2, double t2 )
{
  Init( v1.x * t1 + v2.x * t2,
        v1.y * t1 + v2.y * t2,
        v1.w * t1 + v2.w * t2 );
}


//------------------------------------------------------------------------------
// \ru Присвоить вектору значения. \en Assign values to vector. 
// ---
inline
void MbHomogeneous::Set( const MbHomogeneous & v1, double t1, const MbHomogeneous & v2, double t2,
                         const MbHomogeneous & v3, double t3 )
{
  Init( v1.x * t1 + v2.x * t2 + v3.x * t3,
        v1.y * t1 + v2.y * t2 + v3.y * t3,
        v1.w * t1 + v2.w * t2 + v3.w * t3 );
}


//------------------------------------------------------------------------------
// \ru Присвоить вектору значения. \en Assign values to vector. 
// ---
inline
void MbHomogeneous::Set( const MbHomogeneous & v1, double t1, const MbHomogeneous & v2, double t2,
                         const MbHomogeneous & v3, double t3, const MbHomogeneous & v4, double t4 )
{
  Init( v1.x * t1 + v2.x * t2 + v3.x * t3 + v4.x * t4,
        v1.y * t1 + v2.y * t2 + v3.y * t3 + v4.y * t4,
        v1.w * t1 + v2.w * t2 + v3.w * t3 + v4.w * t4 );
}


//------------------------------------------------------------------------------
// \ru Приравнять координаты вектора координатам суммы точек v1, v2, v3, v4 и v5, умноженных на t1, t2, t3 t4, t4 и t5 соответственно. \en Equate vector coordinates with coordinates of sum of points v1, v2, v3 and v4 multiplied with t1, t2, t3, t4, t5 correspondingly. 
// ---
inline
void MbHomogeneous::Set( const MbHomogeneous & v1, double t1, const MbHomogeneous & v2, double t2,
                         const MbHomogeneous & v3, double t3, const MbHomogeneous & v4, double t4, const MbHomogeneous & v5, double t5 )
{
  Init( v1.x * t1 + v2.x * t2 + v3.x * t3 + v4.x * t4 + v5.x * t5, 
        v1.y * t1 + v2.y * t2 + v3.y * t3 + v4.y * t4 + v5.y * t5,
        v1.w * t1 + v2.w * t2 + v3.w * t3 + v4.w * t4 + v5.w * t5 );
}


//------------------------------------------------------------------------------
// \ru Добавить вектор p, умноженный на kk. \en Add vector p multiplied by kk.
// ---
inline
void MbHomogeneous::Add( const MbHomogeneous & p, double kk ) {
  Add( p.x * kk, p.y * kk, p.w * kk );
}


//------------------------------------------------------------------------------
// \ru Скалярное умножение двух векторов \en Scalar product of two vectors 
// ---
inline
double MbHomogeneous::operator * ( const MbHomogeneous & vector ) const {
  return ( x * vector.x + y * vector.y + w * vector.w );
}


//------------------------------------------------------------------------------
// \ru Векторное умножение двух векторов \en Vector product of two vectors 
// ---
inline
MbHomogeneous MbHomogeneous::operator | ( const MbHomogeneous & vect ) const {
  return MbHomogeneous( y * vect.w - w * vect.y,
                        w * vect.x - x * vect.w,
                        x * vect.y - y * vect.x );
}


//------------------------------------------------------------------------------
// \ru Умножение на число \en The multiplication by a number 
// ---
inline
void MbHomogeneous::operator *= ( double factor )
{
  x *= factor;
  y *= factor;
  w *= factor;
}


//------------------------------------------------------------------------------
// \ru Деление на число \en The division by a number 
// ---
inline
void MbHomogeneous::operator /= ( double factor )
{
  if ( factor != 0 ) {
    factor = 1.0 / factor;
    x *= factor;
    y *= factor;
    w *= factor;
  }
}


//------------------------------------------------------------------------------
// \ru Умножение на число \en The multiplication by a number 
// ---
inline
MbHomogeneous MbHomogeneous::operator * ( double factor ) const {
  return MbHomogeneous( x * factor, y * factor, w * factor );
}


//------------------------------------------------------------------------------
// \ru Деление на число \en The division by a number 
// ---
inline
MbHomogeneous MbHomogeneous::operator / ( double factor ) const
{
  if ( factor != 0 ) {
    factor = 1.0 / factor;
    return MbHomogeneous( x * factor, y * factor , w * factor );
  }
  else {
    return MbHomogeneous( x, y, w );
  }
}


//------------------------------------------------------------------------------
// \ru Проверить на равенство. \en Check for equality. 
// ---
inline
bool MbHomogeneous::operator == ( const MbHomogeneous & with ) const {
  return IsSame( with, Math::LengthEps );
}


//------------------------------------------------------------------------------
// \ru Проверить на неравенство. \en Check for inequality. 
// ---
inline
bool MbHomogeneous::operator != ( const MbHomogeneous & with ) const {
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
inline
bool MbHomogeneous::IsSame( const MbHomogeneous & other, double accuracy ) const
{
  return ( (::fabs(x - other.x) < accuracy) && 
           (::fabs(y - other.y) < accuracy) && 
           (::fabs(w - other.w) < accuracy) );
}


////////////////////////////////////////////////////////////////////////////////
//
// \ru Глобальные функции \en Global functions 
//
////////////////////////////////////////////////////////////////////////////////

namespace c3d // namespace C3D
{

//------------------------------------------------------------------------------
/** \brief \ru Разделение координат и весов.
           \en Separation of coordinates and weights. \~
  \details \ru Разделение координат и весов. \n
    Дополнительная координата точки (вес) вводится для удобства работы с неоднородными рациональными сплайнами.\n
           \en Separation of coordinates and weights. \n
    Additional coordinate of a point (weight) is introduced for the convenience of working with non-uniform rational splines. \n \~
  \ingroup Mathematic_Base_2D
*/
// ---
template <typename PointParamVector, typename PointVector>
void SplitHomoVector( const PointParamVector & hList, PointVector & uvList )
{
  const size_t sz = hList.size();
  uvList.clear();
  uvList.reserve( sz );
  for ( size_t n = 0; n < sz; ++n ) {
    uvList.push_back( MbCartPoint( hList[n].x, hList[n].y ) );
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Разделение координат и весов.
           \en Separation of coordinates and weights. \~
  \details \ru Разделение координат и весов. \n
    Дополнительная координата точки (вес) вводится для удобства работы с неоднородными рациональными сплайнами.\n
           \en Separation of coordinates and weights. \n
    Additional coordinate of a point (weight) is introduced for the convenience of working with non-uniform rational splines. \n \~
  \ingroup Mathematic_Base_2D
*/
// ---
template <typename PointParamVector, typename PointVector, typename ParamVector>
void SplitHomoVector( const PointParamVector & hList, PointVector & uvList, ParamVector & tList )
{
  const size_t sz = hList.size();
  uvList.clear();
  tList.clear();
  uvList.reserve( sz );
  tList.reserve( sz );
  for ( size_t n = 0; n < sz; ++n ) {
    uvList.push_back( MbCartPoint( hList[n].x, hList[n].y ) );
    tList.push_back( hList[n].w );
  }
}

} // namespace C3D


#endif // __MB_HOMOGENIUS_H
