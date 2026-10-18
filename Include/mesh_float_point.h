////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Двумерная точка полигона или триангуляции.
         \en Two-dimensional point of polygon or triangulation. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MESH_FLOAT_POINT_H
#define __MESH_FLOAT_POINT_H


#include <mb_matrix.h>

namespace c3d // namespace C3D
{

//------------------------------------------------------------------------------
/// \ru Конвертация числа из double в float с проверкой перед присваиванием. \en Conversion of number from double to float with check before assignment.
// ---
inline float D2F( double v )
{
  float fv = 0;

  if ( v > FLT_MAX )
    fv = FLT_MAX;
  else if ( v < -FLT_MAX )
    fv = -FLT_MAX;
  else if ( ::fabs(v) > FLT_EPSILON )
    fv = (float)v;

  return fv;
}

} // namespace C3D


//------------------------------------------------------------------------------
/** \brief \ru Двумерная точка.
           \en A two-dimensional point. \~
    \details \ru Класс MbFloatPoint служит для представления точки на плоскости, 
  также как аналогичный класс #MbCartPoint, который отличается более высокой 
  точностью представления. MbFloatPoint имеет структуру данных, состоящей из 
  пары чисел с плавающей точкой одинарной точности (float). Применяется в
  полигоне (MbPolygon) для аппроксимации двухмерных кривых.
             \en MbFloatPoint class is used to present a point on the plane, 
  as a similar #MbCartPoint class which has a higher 
  tolerance of representation. MbFloatPoint has a data structure which consists of 
   a pair of numbers with a single precision floating point (float). It is used in
  the polygon (MbPolygon) to approximate two-dimensional curves. \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbFloatPoint {
public:
  float x; ///< \ru Первая координата точки. \en A first point coordinate. 
  float y; ///< \ru Вторая координата точки. \en A second point coordinate. 

  /// \ru Конструктор. \en Constructor. 
  MbFloatPoint() { x = y = 0; }
  /// \ru Конструктор. \en Constructor. 
  MbFloatPoint( double xx, double yy  ) : x( c3d::D2F(xx ) ), y( c3d::D2F(yy ) ) {}
  /// \ru Конструктор. \en Constructor. 
  explicit MbFloatPoint( const MbCartPoint & p ) : x( c3d::D2F(p.x) ), y( c3d::D2F(p.y) ) {}
  /// \ru Конструктор. \en Constructor. 
  explicit MbFloatPoint( const MbVector &    p ) : x( c3d::D2F(p.x) ), y( c3d::D2F(p.y) ) {}
  /// \ru Конструктор. \en Constructor. 
  MbFloatPoint( const MbFloatPoint & p ) : x( p.x ), y( p.y ) {}

public:

  // \ru Общие функции объекта. \en Common functions of object. 
  void    Move  ( const MbVector & v );                       ///< \ru Сдвиг. \en Translation. 
  void    Rotate( const MbCartPoint &, const MbDirection & ); ///< \ru Вращение. \en Rotation. 
  void    Transform( const MbMatrix & matr );                 ///< \ru Преобразовать согласно матрице. \en Transform according to the matrix. 

  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  void    GetProperties( MbProperties &properties );
  /// \ru Записать свойства объекта. \en Set properties of the object. 
  void    SetProperties( const MbProperties &properties );

  void    GetCartPoint( MbCartPoint & p ) const { p.x = x; p.y = y; } ///< \ru Выдать декартову точку. \en Get the Cartesian point. 
  void    GetVector   ( MbVector    & p ) const { p.x = x; p.y = y; } ///< \ru Выдать вектор. \en Get the vector. 

  void    operator =  ( const MbCartPoint  &  );   ///< \ru Присвоение точки значений. \en The assignment of values to the point. 
  void    operator =  ( const MbVector     &  );   ///< \ru Присвоение точки значений. \en The assignment of values to the point. 
  void    operator =  ( const MbFloatPoint &  );   ///< \ru Присвоение точки значений. \en The assignment of values to the point. 

  bool    operator == ( const MbFloatPoint & ) const; ///< \ru Проверка на равенство. \en The check for equality. 
  bool    operator <  ( const MbFloatPoint & ) const; ///< \ru Проверка на меньше. \en The check for "less". 
  bool    operator >  ( const MbFloatPoint & ) const; ///< \ru Проверка на больше. \en The check for "greater". 

  /// \ru Являются ли объекты равными? \en Are the objects equal?
  bool    IsSame( const MbFloatPoint & other, double accuracy ) const;

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE(MbFloatPoint, MATH_FUNC_EX);
  KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE(MbFloatPoint, MATH_FUNC_EX);
}; // MbFloatPoint


//------------------------------------------------------------------------------
// \ru Сдвиг \en Translation 
// ---
inline void MbFloatPoint::Move( const MbVector & v ) {
  x += (float)v.x;
  y += (float)v.y;
}


//------------------------------------------------------------------------------
// \ru Вращение \en Rotation 
// ---
inline void MbFloatPoint::Rotate( const MbCartPoint & c, const MbDirection & angle ) {
  x -= (float)c.x;
  y -= (float)c.y;

  double xx = (x * angle.ax) - (y * angle.ay);
  double yy = (x * angle.ay) + (y * angle.ax);

  x = (float)( xx + c.x );
  y = (float)( yy + c.y );
}


//------------------------------------------------------------------------------
// \ru Преобразовать согласно матрице \en Transform according to the matrix 
// ---
inline void MbFloatPoint::Transform( const MbMatrix & matr ) {
  double xx = x * matr.El(0, 0) + y * matr.El(1, 0) + matr.El(2, 0);
  double yy = x * matr.El(0, 1) + y * matr.El(1, 1) + matr.El(2, 1);

  x = (float)xx;
  y = (float)yy;
}


//------------------------------------------------------------------------------
// \ru Присвоение точке значений \en Assigning of values to point 
// ---
inline void MbFloatPoint::operator = ( const MbCartPoint & v ) {
  x = c3d::D2F(v.x);
  y = c3d::D2F(v.y);
}


//------------------------------------------------------------------------------
// \ru Присвоение точке значений \en Assigning of values to point 
// ---
inline void MbFloatPoint::operator = ( const MbVector & v ) {
  x = c3d::D2F(v.x);
  y = c3d::D2F(v.y);
}


//------------------------------------------------------------------------------
// \ru Присвоение точке значений вектора \en Assigning of vector values to point 
// ---
inline void MbFloatPoint::operator = ( const MbFloatPoint & v ) {
  x = v.x;
  y = v.y;
}


//------------------------------------------------------------------------------
// \ru Проверка на равенство \en Check for equality 
// ---
inline bool MbFloatPoint::operator == ( const MbFloatPoint & with ) const {
  return ( ::fabs( x - with.x ) < Math::LengthEps ) &&
         ( ::fabs( y - with.y ) < Math::LengthEps );
}


//------------------------------------------------------------------------------
// \ru Проверка на меньше \en Check for "less" 
// ---
inline bool MbFloatPoint::operator < ( const MbFloatPoint & with ) const {
  return ( x < with.x - Math::LengthEps ) ||
         ( ( fabs( x - with.x ) < Math::LengthEps ) && ( y < with.y - Math::LengthEps ) );
}


//------------------------------------------------------------------------------
// \ru Проверка на больше \en The check for "greater" 
// ---
inline bool MbFloatPoint::operator > ( const MbFloatPoint & with ) const {
  return ( x > with.x + Math::LengthEps ) ||
         ( ( fabs( x - with.x ) < Math::LengthEps ) && ( y > with.y + Math::LengthEps ) );
}


//------------------------------------------------------------------------------
// \ru Конструктор присвоения точки значений float-точки \en Constructor of assignment of float-point values to point 
// ---
inline MbCartPoint::MbCartPoint( const MbFloatPoint & fp ) {
  x = fp.x;
  y = fp.y;
}


//------------------------------------------------------------------------------
// \ru Присвоение точки значений float-точки \en Assigning of float-point values to point 
// ---
inline void MbCartPoint::operator = ( const MbFloatPoint & fp ) {
  x = fp.x;
  y = fp.y;
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
inline bool MbFloatPoint::IsSame( const MbFloatPoint & other, double accuracy ) const
{
  return ( (::fabs(x - other.x) < accuracy) && 
           (::fabs(y - other.y) < accuracy) );
}

#endif // __MESH_FLOAT_POINT_H
