////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Габаритные объекты. Одномерный куб.
         \en Bounding box objects. One-dimensional box. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_RECT1D_H
#define __MB_RECT1D_H

#include <mb_enum.h>


//------------------------------------------------------------------------------
/// \ru Одномерный куб. \en One-dimensional box.
/** 
  \ingroup Mathematic_Base_3D
*/ // ---
class MATH_CLASS MbRect1D {
public:
  double zmin; ///< \ru Начало диапазона. \en Start of range. 
  double zmax; ///< \ru Конец диапазона. \en End of range. 

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbRect1D          (                  ) : zmin( MB_MAXDOUBLE ), zmax( -MB_MAXDOUBLE ) {}
  /// \ru Конструктор копирования. \en Copy constructor. 
  MbRect1D          ( const MbRect1D & );
  /// \ru Конструктор по заданным значениям границ. \en Constructor by given bounds. 
  MbRect1D          ( double pmin, double pmax, bool equalize = true );

  /// \ru Инициализировать неустановленным. \en Initialize unspecified. 
  void   Init       (                  );
  /// \ru Инициализировать другим кубом. \en Initialize by another box. 
  void   Init       ( const MbRect1D & );
  /// \ru Инициализировать заданными значениями границ. \en Initialize by given bounds. 
  void   Init       ( double pmin, double pmax, bool equalize = true );

  /// \ru Создать вывернутый одномерный куб. \en Create reverted one-dimensional box. 
  void   Invert     ();
  /// \ru Сократить одномерный куб на заданный коэффициент и расширить на дельта. \en Decrease one-dimensional box by a given factor and increase by delta. 
  void   Short      ( double,         bool bis = true, double delta = LENGTH_EPSILON );
  /// \ru Сократить одномерный куб на заданный коэффициент относительно точки и расширить на дельта. \en Decrease one-dimensional box by a given factor relative to point and increase by delta. 
  void   Short      ( double, double, bool bis = true, double delta = LENGTH_EPSILON );

  /// \ru Включить точку. \en Include point. 
  void   Include    ( double,            bool bis = true, double delta = LENGTH_EPSILON );
  /// \ru Установить куб. \en Set box. 
  void   Include    ( const MbRect1D & , bool bis = true, double delta = LENGTH_EPSILON );

  /// \ru Включить точку. \en Include point. 
  void   IncludeEx  ( double           );
  /// \ru Установить куб. \en Set box. 
  void   IncludeEx  ( const MbRect1D & );

  ///< \ru Выровнять диапазон. \en Justify range. 
  void   Equalize   ( double &, double & ) const;
  /// \ru Выровнять диапазон (zmin, zmax). \en Justify range (zmin, zmax).  
  void   Equalize   (                    );
  /// \ru Является ли область вывернутой. \en Check if region is reverted. 
  bool   IsEmpty    (                    ) const;
  /// \ru Является ли область вырожденной. \en Check if region is degenerate. 
  bool   IsDegenert (                    ) const;

  /// \ru Есть ли пересечение с другим прямоугольником. \en Is there intersection with another rectangle. 
  bool   IsIntersect( const MbRect1D & ) const;
  /// \ru Есть ли пересечение с точкой. \en Is there intersection with point. 
  bool   IsIntersect( double           ) const;
  /// \ru Есть ли пересечение с другим прямоугольником. \en Is there intersection with another rectangle. 
  bool   IsIntersect( double,  double  ) const;
  /// \ru Есть ли пересечение с пустым кубом. \en Is there intersection with empty box. 
  bool   IsEmptyInt ( double           ) const;

  /// \ru Загнать одномерную точку в куб. \en Drive one-dimensional point to box. 
  void   SetInR     ( double   & ) const;
  /// \ru Загнать одномерную точку в куб. \en Drive one-dimensional point to box. 
  void   SetInRect  ( double   & ) const;
  /// \ru Загнать другую область в куб. \en Drive another region to box. 
  void   SetInRect  ( MbRect1D & ) const;

  /// \ru Получить минимум. \en Get minimum. 
  double GetMin     () const { return zmin; }
  /// \ru Получить максимум. \en Get maximum. 
  double GetMax     () const { return zmax; }
  /// \ru Задать минимум. \en Set minimum. 
  void   SetMin( double v ) { zmin = v; }
  /// \ru Задать максимум. \en Set maximum. 
  void   SetMax( double v ) { zmax = v; }

  /// \ru Получить характерный масштаб одномерного куба. \en Get characteristic scale of one-dimensional box. 
  double GetScale   () const;

  /// \ru Увеличить куб. \en Increase box. 
  void   Increase   ( double );

  /// \ru Проверить принадлежность границе первого параметра с точностью, заданной вторым. \en Check if first parameter belongs to bound with tolerance given by second parameter. 
  bool   IsBound    ( double, double ) const;
  /// \ru Сдвинуть куб. \en Move box. 
  void   Move       ( double );
  /// \ru Проверить два куба на равенство с заданной точностью. \en Check equality of two boxes with given tolerance. 
  bool   IsEqual    ( const MbRect1D &, double eps ) const;
  /// \ru Найти габарит пересечения двух габаритов. \en Find bounding box of two bounding boxes intersection. 
  bool   Intersection( const MbRect1D &, const MbRect1D &, double eps = LENGTH_EPSILON );
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool   IsSame( const MbRect1D & other, double accuracy ) const { 
    return ( (::fabs(zmin - other.zmin) < accuracy) && (::fabs(zmax - other.zmax) < accuracy) );
  }
  /// \ru Проверить на равенство (точность PARAM_REGION). \en Check for equality (tolerance PARAM_REGION). 
  bool   operator ==( const MbRect1D & ) const;
  /// \ru Проверка на меньше (точность PARAM_REGION). \en Check for lesser (tolerance PARAM_REGION). 
  bool   operator < ( const MbRect1D & ) const;
};


//------------------------------------------------------------------------------
// \ru Выровнять диапазон. \en Justify range. 
// ---
inline
void MbRect1D::Equalize( double & ozmin, double & ozmax ) const
{
  if ( ozmin > ozmax ) {
    double maxvalue = ozmin;
           ozmin    = ozmax;
           ozmax    = maxvalue;
  }
}


//------------------------------------------------------------------------------
// \ru Выровнять диапазон. \en Justify range. 
// ---
inline
void MbRect1D::Equalize()
{
  Equalize( zmin, zmax );
}


//------------------------------------------------------------------------------
// \ru Конструктор. \en Constructor. 
// ---
inline
MbRect1D::MbRect1D( double ozmin, double ozmax, bool equalize )
  : zmin( ozmin )
  , zmax( ozmax )
{
  if ( equalize )
    Equalize(); // \ru Выровнять куб. \en Justify box. 
}


//------------------------------------------------------------------------------
// \ru Конструктор. \en Constructor. 
// ---
inline
MbRect1D::MbRect1D( const MbRect1D & other )
  : zmin( other.zmin )
  , zmax( other.zmax )
{
}


//------------------------------------------------------------------------------
// \ru Инициализировать. \en Initialize. 
// ---
inline
void MbRect1D::Init()
{
  zmin =  MB_MAXDOUBLE;
  zmax = -MB_MAXDOUBLE;
}


//------------------------------------------------------------------------------
// \ru Инициализировать. \en Initialize. 
// ---
inline
void MbRect1D::Init( double ozmin, double ozmax, bool equalize )
{
  zmin = ozmin; // \ru Присвоить значения. \en Assign values. 
  zmax = ozmax;
  if ( equalize )
    Equalize(); // \ru Выровнять куб. \en Justify box. 
}


//------------------------------------------------------------------------------
// \ru Инициализировать. \en Initialize. 
// ---
inline
void MbRect1D::Init( const MbRect1D & other )
{
  zmin = other.zmin;
  zmax = other.zmax;
}


//------------------------------------------------------------------------------
// \ru Сократить одномерный куб на заданный коэфициент. \en Decrease one-dimensional box by given factor. 
// ---
inline
void MbRect1D::Short( double ks, bool bis, double d )
{
  double delta = bis ? d : 0;
  double l  = ( zmax - zmin ) * 0.5;
  double cz = zmin + l;
  l *= ks;
  l += delta;
  zmin = cz - l;
  zmax = cz + l;
}


//------------------------------------------------------------------------------
// \ru Сократить одномерный куб на заданный коэффициент относительно точки. \en Decrease one-dimensional box by given factor relative to point. 
// ---
inline
void MbRect1D::Short( double ks, double cz, bool bis, double d )
{
  double delta = bis ? d : 0;
  zmax = cz > zmax ? cz + delta : ( zmax - cz ) * ks + cz + delta;
  zmin = cz < zmin ? cz - delta : ( zmin - cz ) * ks + cz - delta;
}


//------------------------------------------------------------------------------
// \ru Включить одномерную точку. \en Include one-dimensional point. 
// ---
inline
void MbRect1D::Include( double other, bool bis, double delta )
{
  if ( other > zmax )
    zmax = bis ? other + delta : other; // \ru Расширить. \en Increase. 
  if ( other < zmin )
    zmin = bis ? other - delta : other; // \ru Расширить. \en Increase. 
}


//------------------------------------------------------------------------------
// \ru Добавить не пустой габарит. \en Add non-empty bounding box. 
// ---
inline
void MbRect1D::Include( const MbRect1D & other, bool bis, double delta )
{
  if ( !other.IsEmpty() ) {
    Include( other.zmin, bis, delta );
    Include( other.zmax, bis, delta );
  }
}


//-------------------------------------------------------------------------------
// \ru Включить точку. \en Include point. 
// ---
inline
void MbRect1D::IncludeEx( double other )
{
  if ( other > zmax ) 
    zmax = other;
  if ( other < zmin ) 
    zmin = other;
}


//-------------------------------------------------------------------------------
// \ru Установить куб. \en Set box. 
// ---
inline
void MbRect1D::IncludeEx( const MbRect1D & other )
{
  if ( !other.IsEmpty() ) {
    IncludeEx( other.zmin );
    IncludeEx( other.zmax );
  }
}


//------------------------------------------------------------------------------
// \ru Является ли область пустой. \en Check if region is empty. 
// ---
inline
bool MbRect1D::IsEmpty() const
{
  return zmin > zmax;
}


//------------------------------------------------------------------------------
// \ru Является ли область вырожденной. \en Check if region is degenerate. 
// ---
inline
bool MbRect1D::IsDegenert() const
{
  return zmax - zmin < LENGTH_REGION;
}


//------------------------------------------------------------------------------
// \ru Включение точки в прямоугольник. \en Point inclusion in rectangle. 
// ---
inline
bool MbRect1D::IsIntersect( double other ) const
{
  return other <= zmax && other >= zmin;
}


//------------------------------------------------------------------------------
// \ru Включение точки в прямоугольник. \en Point inclusion in rectangle. 
// ---
inline
bool MbRect1D::IsIntersect( double z1, double z2 ) const
{
  Equalize( z1, z2 );
  return std_max( zmin, z1 ) <= std_min( zmax, z2 );
}


//------------------------------------------------------------------------------
// \ru Пересечение с другим прямоугольником. \en Check intersection with another rectangle. 
// ---
inline
bool MbRect1D::IsIntersect( const MbRect1D & other ) const
{
  return std_max( zmin, other.zmin ) <= std_min( zmax, other.zmax );
}


//------------------------------------------------------------------------------
// \ru Пересечение с пустым кубом. \en Is there intersection with empty box. 
// ---
inline
bool MbRect1D::IsEmptyInt( double other ) const
{
  return other <= zmax || other >= zmin;
}


//------------------------------------------------------------------------------
// \ru Загнать одномерную точку в куб. \en Drive one-dimensional point to box. 
// ---
inline
void MbRect1D::SetInR( double & z ) const
{
  if ( z < zmin )
    z = zmin;
  else
  if ( z > zmax )
    z = zmax;
}


//------------------------------------------------------------------------------
// \ru Загнать одномерную точку в куб. \en Drive one-dimensional point to box. 
// ---
inline
void MbRect1D::SetInRect( double & z ) const
{
  if ( IsEmpty() ) {
    if( !IsEmptyInt(z) )
      z = zmin;
  }
  else {
    SetInR( z );
  }
}


//------------------------------------------------------------------------------
// \ru Загнать другую область в куб. \en Drive another region to box. 
// ---
inline
void MbRect1D::SetInRect( MbRect1D & other ) const
{
  SetInRect( other.zmin );
  SetInRect( other.zmax );
}


//------------------------------------------------------------------------------
// \ru Создать вывернутый одномерный куб. \en Create reverted one-dimensional box. 
// ---
inline
void MbRect1D::Invert()
{
  double oldzmax = zmax;
  zmax = -zmin;
  zmin = -oldzmax;
}


//------------------------------------------------------------------------------
// \ru Получить характерный масштаб одномерного куба. \en Get characteristic scale of one-dimensional box. 
// ---
inline
double MbRect1D::GetScale() const
{
  return (zmax - zmin);
}


//------------------------------------------------------------------------------
// \ru Увеличить куб. \en Increase box. 
// ---
inline
void MbRect1D::Increase( double delta )
{
  zmax += delta;
  zmin -= delta;
}


//------------------------------------------------------------------------------
// \ru Принадлежность границе первого параметра с точность заданной вторым. \en Check if the first parameter belongs to bound with tolerance given by second parameter. 
// ---
inline
bool MbRect1D::IsBound( double z, double delta ) const
{
  return ::fabs( z - zmin ) < delta || ::fabs( z - zmax ) < delta;
}


//-------------------------------------------------------------------------------
// \ru Сдвиг. \en Move. 
// ---
inline
void MbRect1D::Move( double shift )
{
  zmin += shift;
  zmax += shift;
}


//-------------------------------------------------------------------------------
// \ru Равны ли двы куба. \en Check if two boxes are equal. 
// ---
inline
bool MbRect1D::IsEqual( const MbRect1D & other, double eps ) const
{
  return ( ::fabs(zmin - other.zmin) < eps && ::fabs(zmax - other.zmax) < eps );
}


//------------------------------------------------------------------------------
// \ru Проверка на равенство. \en Check for equality. 
// ---
inline
bool MbRect1D::operator == ( const MbRect1D & other ) const 
{
  return IsEqual( other, PARAM_REGION );
}


//------------------------------------------------------------------------------
// \ru Проверка на меньше. \en Check for lesser.
// ---
inline
bool MbRect1D::operator < ( const MbRect1D & other ) const 
{
  return ( zmin < other.zmin - PARAM_REGION ) ||
    ( ( ::fabs( zmin - other.zmin ) < PARAM_REGION ) && ( zmax < other.zmax - PARAM_REGION ) );
}


//------------------------------------------------------------------------------
// \ru Габарит пересечения двух габаритов. \en Bounding box of two bounding boxes intersection. 
// ---
inline
bool MbRect1D::Intersection( const MbRect1D & r1, const MbRect1D & r2, double eps )
{
  bool isInt = false;

  if ( !r1.IsEmpty() && !r2.IsEmpty() ) {
    eps = ::fabs( eps );
    zmin = std_max( r1.zmin, r2.zmin  );
    zmax = std_min( r1.zmax, r2.zmax );
    if ( zmax > zmin + eps )
      isInt = true;
  }
  if ( !isInt )
    Init();

  return isInt;
}


#endif // __MB_RECT1D_H