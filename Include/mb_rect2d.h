////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Габаритные объекты. Двумерный и трехмерный кубы.
         \en Bounding box objects. Two-dimensional and three-dimensional boxes. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_RECT2D_H
#define __MB_RECT2D_H

#include <mb_rect1d.h>
#include <mb_rect.h>
#include <mb_cart_point.h>
#include <mb_cart_point3d.h>


////////////////////////////////////////////////////////////////////////////////
//
// \ru Двумерный куб. \en A two-dimensional box.
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
/// \ru Двумерный куб. \en A two-dimensional box. 
/**
  \ingroup Mathematic_Base_3D
*/ // ---
class MATH_CLASS MbRect2D {
public:
  MbRect1D rx; ///< \ru Диапазон по x. \en Range for x. 
  MbRect1D ry; ///< \ru Диапазон по y. \en Range for y. 

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbRect2D() : rx(), ry() {}
  /// \ru Конструктор по заданным значениям границ. \en Constructor by given bounds. 
  MbRect2D( double xmin, double ymin, double xmax, double ymax ) : rx( xmin, xmax ), ry( ymin, ymax ) {}
  /// \ru Конструктор по двум диагональным точкам. \en The constructor by two diagonal points. 
  MbRect2D( const MbCartPoint &   pmin, const MbCartPoint &   pmax ) : rx( pmin.x, pmax.x ), ry( pmin.y, pmax.y ) {}
  /// \ru Конструктор по двум диагональным трехмерным точкам. \en The constructor by two diagonal three-dimensional points. 
  MbRect2D( const MbCartPoint3D & pmin, const MbCartPoint3D & pmax ) : rx( pmin.x, pmax.x ), ry( pmin.y, pmax.y ) {}
  /// \ru Конструктор копирования. \en Copy constructor. 
  MbRect2D( const MbRect2D & other ) : rx( other.rx ), ry( other.ry ) {}

  /// \ru Инициализировать неустановленным. \en Initialize unspecified. 
  void   Init       ();
  /// \ru Инизиализировать заданными значениями границ. \en Initialize by given bounds. 
  void   Init       ( double xmin, double ymin, double xmax, double ymax );
  /// \ru Инизиализировать двумя диагональными точками. \en Initialize by two diagonal points. 
  void   Init       ( const MbCartPoint   &, const MbCartPoint   & );
  /// \ru Инизиализировать двумя диагональными трехмерными точками. \en Initialize by two diagonal three-dimensional points. 
  void   Init       ( const MbCartPoint3D &, const MbCartPoint3D & );
  /// \ru Инициализировать другим кубом. \en Initialize by another bounding box. 
  void   Init       ( const MbRect2D      & );
  /// \ru Инициализировать другим кубом. \en Initialize by another bounding box. 
  void   Init       ( const MbRect        & );

  /// \ru Создать вывернутый двумерный куб. \en Create reverted two-dimensional box. 
  void   Invert     ();

  /// \ru Сократить двумерный куб на заданный коэффициент. \en Decrease two-dimensional box by given factor. 
  void   Short      ( double,                        bool bis = true, double delta = Math::lengthEpsilon );
  /// \ru Сократить двумерный куб на заданный коэффициент. \en Decrease two-dimensional box by given factor. 
  void   Short      ( double, double, double,        bool bis = true, double delta = Math::lengthEpsilon );
  /// \ru Сократить двумерный куб на заданный коэффициент. \en Decrease two-dimensional box by given factor. 
  void   Short      ( double, const MbCartPoint   &, bool bis = true, double delta = Math::lengthEpsilon );
  /// \ru Сократить двумерный куб на заданный коэффициент. \en Decrease two-dimensional box by given factor. 
  void   Short      ( double, const MbVector      &, bool bis = true, double delta = Math::lengthEpsilon );
  /// \ru Сократить двумерный куб на заданный коэффициент. \en Decrease two-dimensional box by given factor. 
  void   Short      ( double, const MbCartPoint3D &, bool bis = true, double delta = Math::lengthEpsilon );
  /// \ru Сократить двумерный куб на заданный коэффициент. \en Decrease two-dimensional box by given factor. 
  void   Short      ( double, const MbVector3D    &, bool bis = true, double delta = Math::lengthEpsilon );

  /// \ru Включить точку. \en Include point. 
  void   Include    ( double, double,        bool bis = true, double delta = Math::lengthEpsilon );
  /// \ru Включить точку. \en Include point. 
  void   Include    ( const MbCartPoint   &, bool bis = true, double delta = Math::lengthEpsilon );
  /// \ru Включить точку. \en Include point. 
  void   Include    ( const MbVector      &, bool bis = true, double delta = Math::lengthEpsilon );
  /// \ru Включить точку. \en Include point. 
  void   Include    ( const MbCartPoint3D &, bool bis = true, double delta = Math::lengthEpsilon );
  /// \ru Включить точку. \en Include point. 
  void   Include    ( const MbVector3D    &, bool bis = true, double delta = Math::lengthEpsilon );
  /// \ru Включить другой двумерный куб. \en Include another two-dimensional box. 
  void   Include    ( const MbRect2D      &, bool bis = true, double delta = Math::lengthEpsilon );
  /// \ru Установить двумерный куб. \en Set two-dimensional box. 
  void   Include    ( const MbCartPoint   &, const MbCartPoint &, bool bis = true, double delta = Math::lengthEpsilon );

  /// \ru Включить точку. \en Include point. 
  void   IncludeEx  ( double ,  double      );
  /// \ru Включить точку. \en Include point. 
  void   IncludeEx  ( const MbCartPoint   & );
  /// \ru Включить точку. \en Include point. 
  void   IncludeEx  ( const MbVector      & );
  /// \ru Включить точку. \en Include point. 
  void   IncludeEx  ( const MbCartPoint3D & );
  /// \ru Включить точку. \en Include point. 
  void   IncludeEx  ( const MbVector3D    & );
  /// \ru Включить другой двумерный куб. \en Include another two-dimensional box. 
  void   IncludeEx  ( const MbRect2D      & );
  /// \ru Установить двумерный куб. \en Set two-dimensional box. 
  void   IncludeEx  ( const MbCartPoint   &, const MbCartPoint & );

  /// \ru Выровнять область. \en Justify the region. 
  void   Equalize   ();
  /// \ru Выровнять область. \en Justify the region. 
  void   Equalize   ( MbCartPoint   &,       MbCartPoint   & ) const;
  /// \ru Выровнять область. \en Justify the region. 
  void   Equalize   ( double &, double &, double &, double & ) const;
  /// \ru Проверить, является ли область пустой. \en Check if region is empty. 
  bool   IsEmpty    () const;
  /// \ru Проверить, является ли область вырожденной. \en Check if region is degenerate. 
  bool   IsDegenert () const;

  /// \ru Пересекается ли точка с прямоугольником. \en Check if point intersects with rectangle. 
  bool   IsIntersect( const MbCartPoint3D & ) const;
  /// \ru Пересекается ли точка с прямоугольником. \en Check if point intersects with rectangle. 
  bool   IsIntersect( const MbCartPoint   & ) const;
  /// \ru Пересекается ли точка с прямоугольником. \en Check if point intersects with rectangle. 
  bool   IsIntersect( double, double        ) const;
  /// \ru Пересекаются ли прямоугольники. \en Check if rectangles intersect. 
  bool   IsIntersect( const MbRect2D      & ) const;
  /// \ru Пересекаются ли прямоугольники. \en Check if rectangles intersect. 
  bool   IsIntersect( const MbCartPoint   &, const MbCartPoint   & ) const;
  /// \ru Пересекаются ли прямоугольники. \en Check if rectangles intersect. 
  bool   IsIntersect( const MbCartPoint3D &, const MbCartPoint3D & ) const;
  /// \ru Пересекаются ли прямоугольники. \en Check if rectangles intersect. 
  bool   IsIntersect( double, double, double, double ) const;
  /// \ru Есть ли пересечение с пустым кубом. \en Is there intersection with empty box. 
  bool   IsEmptyInt ( double, double ) const;
  /// \ru Есть ли пересечение с пустым кубом. \en Is there intersection with empty box. 
  bool   IsEmptyInt ( const MbVector      & ) const;
  ///< \ru Есть ли пересечение с пустым кубом. \en Is there intersection with empty box. 
  bool   IsEmptyInt ( const MbCartPoint   & ) const;
  /// \ru Есть ли пересечение с пустым кубом. \en Is there intersection with empty box. 
  bool   IsEmptyInt ( const MbVector3D    & ) const;
  /// \ru Есть ли пересечение с пустым кубом. \en Is there intersection with empty box. 
  bool   IsEmptyInt ( const MbCartPoint3D & ) const;

  /// \ru Загнать трехмерную точку в куб. \en Drive three-dimensional point to box. 
  void   SetInRect  ( MbCartPoint3D &    ) const;
  /// \ru Загнать двумерную точку в куб. \en Drive two-dimensional point to box. 
  void   SetInRect  ( MbCartPoint   &    ) const;
  /// \ru Загнать другой куб в куб. \en Drive another box to box. 
  void   SetInRect  ( MbRect2D      &    ) const;
  /// \ru Загнать двумерную точку в куб. \en Drive two-dimensional point to box. 
  void   SetInRect  ( double &, double & ) const;
  /// \ru Загнать в куб. \en Drive to box. 
  void   SetInRectX ( double & ) const;
  /// \ru Загнать в куб. \en Drive to box. 
  void   SetInRectY ( double & ) const;

  /// \ru Вернуть минимальное значение параметра u. \en Get the minimum value of u. 
  double GetXMin() const { return rx.GetMin(); }
  /// \ru Вернуть максимальное значение параметра u. \en Get the maximum value of u. 
  double GetXMax() const { return rx.GetMax(); }
  /// \ru Вернуть минимальное значение параметра v. \en Get the minimum value of v. 
  double GetYMin() const { return ry.GetMin(); }
  /// \ru Вернуть максимальное значение параметра v. \en Get the maximum value of v. 
  double GetYMax() const { return ry.GetMax(); }

  /// \ru Получить характерный масштаб двумерного куба по x. \en Get characteristic scale of two-dimensional box by x. 
  double GetScaleX  () const;
  /// \ru Получить характерный масштаб двумерного куба по y. \en Get characteristic scale of two-dimensional box by y. 
  double GetScaleY  () const;
  /// \ru Получить характерный масштаб двумерного куба. \en Get characteristic scale of two-dimensional box. 
  double GetScale   () const;

  /// \ru Увеличить куб по x. \en Increase box at x. 
  void   IncreaseX  ( double );
  /// \ru Увеличить куб по y. \en Increase box at y. 
  void   IncreaseY  ( double );
  ///< \ru Увеличить куб. \en Increase box. 
  void   Increase   ( double );
  
  /// \ru Принадлежит ли границе X первый параметр с точностью заданной вторым. \en Check if value given by the first parameter belongs to X bound with tolerance given by the second parameter. 
  bool   IsBoundX   ( double, double ) const;
  /// \ru Принадлежит ли границе Y первый параметр с точностью заданной вторым. \en Check if value given by the first parameter belongs to Y bound with tolerance given by the second parameter. 
  bool   IsBoundY   ( double, double ) const;
  /// \ru Принадлежит ли границе первый параметр с точностью заданной вторым. \en Check if value given by the first parameter belongs to bound with tolerance given by the second parameter. 
  bool   IsBound    ( double, double, double        ) const;
  /// \ru Принадлежит ли границе первый параметр с точностью заданной вторым. \en Check if value given by the first parameter belongs to bound with tolerance given by the second parameter. 
  bool   IsBound    ( const MbCartPoint   &, double ) const;
  /// \ru Принадлежит ли границе первый параметр с точностью заданной вторым. \en Check if value given by the first parameter belongs to bound with tolerance given by the second parameter. 
  bool   IsBound    ( const MbVector      &, double ) const;
  /// \ru Принадлежит ли границе первый параметр с точностью заданной вторым. \en Check if value given by the first parameter belongs to bound with tolerance given by the second parameter. 
  bool   IsBound    ( const MbCartPoint3D &, double ) const;
  /// \ru Принадлежит ли границе первый параметр с точностью заданной вторым. \en Check if value given by the first parameter belongs to bound with tolerance given by the second parameter. 
  bool   IsBound    ( const MbVector3D    &, double ) const;
  /// \ru Сдвинуть куб. \en Move box. 
  void   Move       ( const MbVector & );
  /// \ru Выдать вершину габаритного прямоугольника по индексу от 0 до 3. \en Get vertex of bounding rectangle by index in range from 0 to 3. 
  void   GetVertex( size_t index, MbCartPoint & p ) const;

};


//------------------------------------------------------------------------------
// \ru Выровнять область. \en Justify region. 
// ---
inline
void MbRect2D::Equalize( double & fx, double & fy, double & sx, double & sy ) const
{
  rx.Equalize( fx, sx );
  ry.Equalize( fy, sy );
}


//------------------------------------------------------------------------------
// \ru Выровнять диапазоны. \en Justify ranges. 
// ---
inline
void MbRect2D::Equalize( MbCartPoint & opmin, MbCartPoint & opmax ) const {
  Equalize( opmin.x, opmin.y, opmax.x, opmax.y );
}


//------------------------------------------------------------------------------
// \ru Выровнять диапазоны. \en Justify ranges. 
// ---
inline
void MbRect2D::Equalize()
{
  rx.Equalize();
  ry.Equalize();
}


//------------------------------------------------------------------------------
// \ru Инициализировать пустой. \en Initialize as empty. 
// ---
inline
void MbRect2D::Init()
{
  rx.Init();
  ry.Init();
}


//------------------------------------------------------------------------------
// \ru Инициализировать. \en Initialize. 
// ---
inline
void MbRect2D::Init( double pminx, double pminy, double pmaxx, double pmaxy )
{
  rx.Init( pminx, pmaxx );
  ry.Init( pminy, pmaxy );
}


//------------------------------------------------------------------------------
// \ru Инициализировать. \en Initialize. 
// ---
inline
void MbRect2D::Init( const MbCartPoint & opmin, const MbCartPoint & opmax )
{
  rx.Init( opmin.x, opmax.x );
  ry.Init( opmin.y, opmax.y );
}


//------------------------------------------------------------------------------
// \ru Инициализировать. \en Initialize. 
// ---
inline
void MbRect2D::Init( const MbCartPoint3D & opmin, const MbCartPoint3D & opmax )
{
  rx.Init( opmin.x, opmax.x );
  ry.Init( opmin.y, opmax.y );
}


//------------------------------------------------------------------------------
// \ru Инициализировать другим кубом. \en Initialize by another bounding box. 
// ---
inline
void MbRect2D::Init( const MbRect2D & other )
{
  rx.Init( other.rx );
  ry.Init( other.ry );
}


//------------------------------------------------------------------------------
// \ru Инициализировать другим кубом. \en Initialize by another bounding box. 
// ---
inline
void MbRect2D::Init( const MbRect & other )
{
  rx.Init( other.GetXMin(), other.GetXMax(), false );
  ry.Init( other.GetYMin(), other.GetYMax(), false );
}


//------------------------------------------------------------------------------
// \ru Сократить двумерный куб на заданный коэффициент. \en Decrease two-dimensional box by given factor. 
// ---
inline
void MbRect2D::Short( double ks, bool bis, double d )
{
  rx.Short( ks, bis, d );
  ry.Short( ks, bis, d );
}


//------------------------------------------------------------------------------
// \ru Сократить двумерный куб на заданный коэффициент. \en Decrease two-dimensional box by given factor. 
// ---
inline
void MbRect2D::Short( double ks, double cx, double cy, bool bis, double d )
{
  rx.Short( ks, cx, bis, d );
  ry.Short( ks, cy, bis, d );
}


//------------------------------------------------------------------------------
// \ru Сократить двумерный куб на заданный коэффициент. \en Decrease two-dimensional box by given factor. 
// ---
inline
void MbRect2D::Short( double ks, const MbCartPoint & cp, bool bis, double d ) {
  Short( ks, cp.x, cp.y, bis, d );
}


//------------------------------------------------------------------------------
// \ru Сократить двумерный куб на заданный коэффициент. \en Decrease two-dimensional box by given factor. 
// ---
inline
void MbRect2D::Short( double ks, const MbVector & cp, bool bis, double d ) {
  Short( ks, cp.x, cp.y, bis, d );
}


//------------------------------------------------------------------------------
// \ru Сократить двумерный куб на заданный коэффициент. \en Decrease two-dimensional box by given factor. 
// ---
inline
void MbRect2D::Short( double ks, const MbCartPoint3D & cp, bool bis, double d ) {
  Short( ks, cp.x, cp.y, bis, d );
}


//------------------------------------------------------------------------------
// \ru Сократить двумерный куб на заданный коэффициент. \en Decrease two-dimensional box by given factor. 
// ---
inline
void MbRect2D::Short( double ks, const MbVector3D & cp, bool bis, double d ) {
  Short( ks, cp.x, cp.y, bis, d );
}


//------------------------------------------------------------------------------
// \ru Включить двумерную точку. \en Include two-dimensional point. 
// ---
inline
void MbRect2D::Include( double x, double y, bool bis, double delta )
{
  rx.Include( x, bis, delta );
  ry.Include( y, bis, delta );
}


//------------------------------------------------------------------------------
// \ru Включить двумерную точку. \en Include two-dimensional point. 
// ---
inline
void MbRect2D::Include( const MbCartPoint & other, bool bis, double delta ) {
  Include( other.x, other.y, bis, delta );
}


//------------------------------------------------------------------------------
// \ru Включить двумерную точку. \en Include two-dimensional point. 
// ---
inline
void MbRect2D::Include( const MbVector & other, bool bis, double delta ) {
  Include( other.x, other.y, bis, delta );
}


//------------------------------------------------------------------------------
// \ru Включить двумерную точку. \en Include two-dimensional point. 
// ---
inline
void MbRect2D::Include( const MbVector3D & other, bool bis, double delta ) {
  Include( other.x, other.y, bis, delta );
}


//------------------------------------------------------------------------------
// \ru Включить двумерную точку. \en Include two-dimensional point. 
// ---
inline
void MbRect2D::Include( const MbCartPoint3D & other, bool bis, double delta ) {
  Include( other.x, other.y, bis, delta );
}


//------------------------------------------------------------------------------
// \ru Установить двумерный куб \en Set two-dimensional box 
// ---
inline
void MbRect2D::Include( const MbCartPoint  &opmin, const MbCartPoint & opmax, bool bis, double delta )
{
  Include( opmin, bis, delta );
  Include( opmax, bis, delta );
}


//------------------------------------------------------------------------------
// \ru Включить другой двумерный куб \en Include another two-dimensional box 
// ---
inline
void MbRect2D::Include( const MbRect2D & other, bool bis, double delta )
{
  rx.Include( other.rx, bis, delta );
  ry.Include( other.ry, bis, delta );
}


//-------------------------------------------------------------------------------
// \ru Включить точку. \en Include point.
// ---
inline
void MbRect2D::IncludeEx( double x, double y )
{
  rx.IncludeEx( x );
  ry.IncludeEx( y );
}


//-------------------------------------------------------------------------------
// \ru Включить точку. \en Include point. 
// ---
inline
void MbRect2D::IncludeEx( const MbCartPoint & other ) {
  IncludeEx( other.x, other.y );
}


//-------------------------------------------------------------------------------
// \ru Включить вектор. \en Include vector. 
// ---
inline
void MbRect2D::IncludeEx( const MbVector & other ) {
  IncludeEx( other.x, other.y );  
}


//-------------------------------------------------------------------------------
// \ru Включить точку. \en Include point. 
// ---
inline
void MbRect2D::IncludeEx( const MbCartPoint3D & other ) {
  IncludeEx( other.x, other.y );
}


//-------------------------------------------------------------------------------
// \ru Включить вектор. \en Include vector. 
// ---
inline
void MbRect2D::IncludeEx( const MbVector3D & other ) {
  IncludeEx( other.x, other.y );
}


//-------------------------------------------------------------------------------
// \ru Установить двумерный куб. \en Set two-dimensional box. 
// ---
inline
void MbRect2D::IncludeEx( const MbCartPoint & opmin, const MbCartPoint & opmax )
{
  IncludeEx( opmin );
  IncludeEx( opmax );  
}


//-------------------------------------------------------------------------------
// \ru Включить другой двумерный куб. \en Include another two-dimensional box. 
// ---
inline
void MbRect2D::IncludeEx( const MbRect2D & other )
{
  rx.IncludeEx( other.rx );
  ry.IncludeEx( other.ry );
}


//------------------------------------------------------------------------------
// \ru Является ли двумерный куб пустым. \en Check if two-dimensional box is empty. 
// ---
inline
bool MbRect2D::IsEmpty() const {
  return rx.IsEmpty() || ry.IsEmpty();
}


//------------------------------------------------------------------------------
// \ru Является ли область вырожденной. \en Check if region is degenerate. 
// ---
inline
bool MbRect2D::IsDegenert() const {
  return rx.IsDegenert() || ry.IsDegenert();
}


//------------------------------------------------------------------------------
// \ru Пересечение с другим прямоугольником. \en Check intersection with another rectangle. 
// ---
inline
bool MbRect2D::IsIntersect( double fx, double fy, double sx, double sy ) const {
  return rx.IsIntersect( fx, sx ) && ry.IsIntersect( fy, sy );
}


//------------------------------------------------------------------------------
// \ru Пересечение с другим прямоугольником. \en Check intersection with another rectangle. 
// ---
inline
bool MbRect2D::IsIntersect( const MbCartPoint & omin, const MbCartPoint & omax ) const {
  return IsIntersect( omin.x, omin.y, omax.x, omax.y );
}


//------------------------------------------------------------------------------
// \ru Пересечение с другим прямоугольником. \en Check intersection with another rectangle. 
// ---
inline
bool MbRect2D::IsIntersect( const MbCartPoint3D & omin, const MbCartPoint3D & omax ) const {
  return IsIntersect( omin.x, omin.y, omax.x, omax.y );
}


//------------------------------------------------------------------------------
// \ru Пересечение с другим габаритом. \en Check intersection with another bounding box. 
// ---
inline
bool MbRect2D::IsIntersect( const MbRect2D & other ) const {
  return rx.IsIntersect( other.rx ) && ry.IsIntersect( other.ry );
}


//------------------------------------------------------------------------------
// \ru Лежит ли точка внутри области. \en Is point inside region. 
// ---
inline
bool MbRect2D::IsIntersect( double x, double y ) const {
  return rx.IsIntersect( x ) && ry.IsIntersect( y );
}


//------------------------------------------------------------------------------
// \ru Лежит ли точка внутри области. \en Is point inside region. 
// ---
inline
bool MbRect2D::IsIntersect( const MbCartPoint & other ) const {
  return rx.IsIntersect( other.x ) && ry.IsIntersect( other.y );
}


//------------------------------------------------------------------------------
// \ru Лежит ли точка внутри области. \en Is point inside region. 
// ---
inline
bool MbRect2D::IsIntersect( const MbCartPoint3D & other ) const {
  return rx.IsIntersect( other.x ) && ry.IsIntersect( other.y );
}


//------------------------------------------------------------------------------
// \ru Пересечение с пустым кубом. \en Is there intersection with empty box. 
// ---
inline
bool MbRect2D::IsEmptyInt( double x, double y ) const {
  return rx.IsEmptyInt( x ) || ry.IsEmptyInt( y );
}


//------------------------------------------------------------------------------
// \ru Пересечение с пустым кубом. \en Is there intersection with empty box. 
// ---
inline
bool MbRect2D::IsEmptyInt( const MbVector & p ) const {
  return IsEmptyInt( p.x, p.y );
}


//------------------------------------------------------------------------------
// \ru Пересечение с пустым кубом. \en Is there intersection with empty box. 
// ---
inline
bool MbRect2D::IsEmptyInt( const MbCartPoint & p ) const {
  return IsEmptyInt( p.x, p.y );
}


//------------------------------------------------------------------------------
// \ru Пересечение с пустым кубом. \en Is there intersection with empty box. 
// ---
inline
bool MbRect2D::IsEmptyInt( const MbVector3D & p ) const {
  return IsEmptyInt( p.x, p.y );
}


//------------------------------------------------------------------------------
// \ru Пересечение с пустым кубом. \en Is there intersection with empty box. 
// ---
inline
bool MbRect2D::IsEmptyInt( const MbCartPoint3D & p ) const {
  return IsEmptyInt( p.x, p.y );
}


//------------------------------------------------------------------------------
// \ru Загнать двумерную точку в куб. \en Drive two-dimensional point to box. 
// ---
inline
void MbRect2D::SetInRectX( double & value ) const {
  rx.SetInRect( value );
}


//------------------------------------------------------------------------------
// \ru Загнать двумерную точку в куб. \en Drive two-dimensional point to box. 
// ---
inline
void MbRect2D::SetInRectY( double & value ) const {
  ry.SetInRect( value );
}


//------------------------------------------------------------------------------
// \ru Загнать двумерную точку в куб. \en Drive two-dimensional point to box. 
// ---
inline
void MbRect2D::SetInRect( double & x, double & y ) const
{
  rx.SetInRect( x );
  ry.SetInRect( y );
}


//------------------------------------------------------------------------------
// \ru Загнать двумерную точку в куб. \en Drive two-dimensional point to box. 
// ---
inline
void MbRect2D::SetInRect( MbCartPoint & p ) const
{
  rx.SetInRect( p.x );
  ry.SetInRect( p.y );
}


//------------------------------------------------------------------------------
// \ru Загнать трехмерную точку в куб. \en Drive three-dimensional point to box. 
// ---
inline
void MbRect2D::SetInRect( MbCartPoint3D & p ) const
{
  rx.SetInRect( p.x );
  ry.SetInRect( p.y );
}


//------------------------------------------------------------------------------
// \ru Загнать другой куб в куб. \en Drive another box to box.
// ---
inline
void MbRect2D::SetInRect( MbRect2D & other ) const
{
  rx.SetInRect( other.rx );
  ry.SetInRect( other.ry );  
}


//------------------------------------------------------------------------------
// \ru Создать вывернутый двумерный куб. \en Create reverted two-dimensional box. 
// ---
inline
void MbRect2D::Invert()
{
  rx.Invert();
  ry.Invert();
}


//------------------------------------------------------------------------------
// \ru Получить характерный масштаб двумерного куба. \en Get characteristic scale of two-dimensional box.
// ---
inline
double MbRect2D::GetScaleX() const {
  return rx.GetScale();
}


//------------------------------------------------------------------------------
// \ru Получить характерный масштаб двумерного куба. \en Get characteristic scale of two-dimensional box. 
// ---
inline
double MbRect2D::GetScaleY() const {
  return ry.GetScale();
}


//------------------------------------------------------------------------------
// \ru Получить характерный масштаб двумерного куба. \en Get characteristic scale of two-dimensional box. 
// ---
inline
double MbRect2D::GetScale() const {
  return rx.GetScale() + ry.GetScale();
}


//------------------------------------------------------------------------------
// \ru Увеличить куб. \en Increase box. 
// ---
inline
void MbRect2D::IncreaseX( double delta ) {
  rx.Increase( delta );
}


//------------------------------------------------------------------------------
// \ru Увеличить куб. \en Increase box. 
// ---
inline
void MbRect2D::IncreaseY( double delta ) {
  ry.Increase( delta );
}


//------------------------------------------------------------------------------
// \ru Увеличить куб \en Increase box 
// ---
inline
void MbRect2D::Increase( double delta )
{
  rx.Increase( delta );
  ry.Increase( delta );
}


//------------------------------------------------------------------------------
// \ru Принадлежность границе. \en Belonging to bound. 
// ---
inline
bool MbRect2D::IsBoundX( double x, double delta ) const {
  return rx.IsBound( x, delta );
}


//------------------------------------------------------------------------------
// \ru Принадлежность границе. \en Belonging to bound. 
// ---
inline
bool MbRect2D::IsBoundY( double y, double delta ) const {
  return ry.IsBound( y, delta );
}


//------------------------------------------------------------------------------
// \ru Принадлежность границе. \en Belonging to bound.
// ---
inline
bool MbRect2D::IsBound ( double x, double y, double delta ) const {
  return rx.IsBound( x, delta ) || ry.IsBound( y, delta );
}


//------------------------------------------------------------------------------
// \ru Принадлежность границе. \en Belonging to bound. 
// ---
inline
bool MbRect2D::IsBound ( const MbCartPoint & p, double delta ) const {
  return rx.IsBound( p.x, delta ) || ry.IsBound( p.y, delta );
}


//------------------------------------------------------------------------------
// \ru Принадлежность границе. \en Belonging to bound.
// ---
inline
bool MbRect2D::IsBound ( const MbVector & p, double delta ) const {
  return rx.IsBound( p.x, delta ) || ry.IsBound( p.y, delta );
}


//------------------------------------------------------------------------------
// \ru Принадлежность границе. \en Belonging to bound. 
// ---
inline
bool MbRect2D::IsBound ( const MbCartPoint3D & p, double delta ) const {
  return rx.IsBound( p.x, delta ) || ry.IsBound( p.y, delta );
}


//------------------------------------------------------------------------------
// \ru Принадлежность границе. \en Belonging to bound. 
// ---
inline
bool MbRect2D::IsBound ( const MbVector3D & p, double delta ) const {
  return rx.IsBound( p.x, delta ) || ry.IsBound( p.y, delta );
}


//-------------------------------------------------------------------------------
// \ru Сдвиг. \en Move. 
// ---
inline
void MbRect2D::Move( const MbVector & vShift )
{
  if ( ::fabs(vShift.x) > EXTENT_EQUAL )
    rx.Move( vShift.x );
  if ( ::fabs(vShift.y) > EXTENT_EQUAL )
    ry.Move( vShift.y );
}


//------------------------------------------------------------------------------
// выдать вершину габаритного прямоугольника по индексу от 0 до 3
//
//  Y
//  |
//  3 - - - 2
//  |       |
//  |       |
//  0 - - - 1 - X
//
// ---
inline
void MbRect2D::GetVertex( size_t index, MbCartPoint & p ) const
{
  index = std_max( (ptrdiff_t)0, (ptrdiff_t)index );
  index = std_min( (ptrdiff_t)3, (ptrdiff_t)index );

  switch ( index ) {
  case 0: { p.x = rx.zmin;  p.y = ry.zmin; break; }
  case 1: { p.x = rx.zmax;  p.y = ry.zmin; break; }
  case 2: { p.x = rx.zmax;  p.y = ry.zmax; break; }
  case 3: { p.x = rx.zmin;  p.y = ry.zmax; break; }
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// \ru Трехмерный куб \en Three-dimensional box 
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
/// \ru Трехмерный куб. \en Three-dimensional box. 
/**
  \ingroup Mathematic_Base_3D
*/ // ---
class MbRect3D {
public:
  MbRect1D rx; ///< \ru Диапазон по x. \en Range for x. 
  MbRect1D ry; ///< \ru Диапазон по y. \en Range for y. 
  MbRect1D rz; ///< \ru Диапазон по z. \en Range for z. 

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbRect3D() : rx(), ry(), rz() {}

private:
  MbRect3D ( const MbRect3D & ); // \ru Не реализован \en Not implemented 
  void   operator = ( const MbRect3D & ); // \ru Не реализован \en Not implemented 
};


/** \} */


#endif

