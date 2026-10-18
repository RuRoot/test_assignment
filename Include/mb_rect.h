////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Габаритный прямоугольник.
         \en Bounding rectangle. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_RECT_H
#define __MB_RECT_H

#include <mb_cart_point.h>
#include <templ_s_array.h>
#include <mb_homogeneous.h>
#include <mb_cube_tree.h>
#include <utility>
#include <vector>


class   MbMatrix;
class   MbRect;
class   MbRect2D;

namespace c3d // namespace C3D
{
typedef std::pair<MbRect *, size_t>                       RectPtrIndex;         ///< \ru Габаритный куб и индекс. \en Bounding box and index.
typedef std::pair<const MbRect *, size_t>                 ConstRectPtrIndex;    ///< \ru Габаритный куб и индекс. \en Bounding box and index.
typedef std::vector<RectPtrIndex>                         RectsPtrIndices;      ///< \ru Вектор габаритных кубов и индексов. \en Vector of bounding boxes and indices.
typedef std::vector<ConstRectPtrIndex>                    ConstRectsPtrIndices; ///< \ru Вектор габаритных кубов и индексов. \en Vector of bounding boxes and indices.
typedef std::vector<MbRect>                               RectsVector;          ///< \ru Вектор габаритных кубов. \en Vector of bounding boxes.
typedef MbCubeTree<MbRect, MbRect, MbCartPoint, MbVector> RectsTree;            ///< \ru Дерево габаритных кубов. \en Tree of bounding boxes.
} // namespace C3D


//------------------------------------------------------------------------------
/** \brief \ru Возможные положения двух габаритов относительно друг друга.
           \en Possible locations of two bounding boxes relative to each other. \~
  \details \ru Возможные положения двух габаритов относительно друг друга.
           \en Possible locations of two bounding boxes relative to each other. \~
  \ingroup Mathematic_Base_2D
*/
// ---
enum TaeTwoRectPos {
  rp_FirstInside,    ///< \ru Первый включает в себя второй габарит. \en The first bounding box includes the second one. 
  rp_SecondInside,   ///< \ru Второй включает в себя первый габарит. \en The second bounding box includes the first one. 
  rp_Intersect,      ///< \ru Габариты пересекаются. \en Bounding boxes intersect. 
  rp_NoIntersect     ///< \ru Габариты не пересекаются. \en Bounding boxes do not intersect. 
};


//------------------------------------------------------------------------------
/** \brief \ru Габаритный прямоугольник.
           \en Bounding rectangle. \~
  \details \ru Габаритный прямоугольник в двумерном пространстве. \n
    Используется для быстрого оценочного определения близости двумерных объектов.
           \en Bounding box in two-dimensional space. \n
    Used for fast estimation of two-dimensional objects proximity. \~
  \ingroup Mathematic_Base_2D
*/
// ---
class MATH_CLASS MbRect {
public :
  double left;   ///< \ru Левая   граница габаритного прямоугольника. \en Left bound of bounding box. 
  double bottom; ///< \ru Нижняя  граница габаритного прямоугольника. \en Bottom bound of bounding box. 
  double right;  ///< \ru Правая  граница габаритного прямоугольника. \en Right bound of bounding box. 
  double top;    ///< \ru Верхняя граница габаритного прямоугольника. \en Top bound of bounding box. 

public:
  /// \ru Конструктор пустого габарита. \en Constructor of an empty bounding box. 
  MbRect() { SetEmpty(); }
  /// \ru Конструктор по заданным значениям границ. \en Constructor by given bounds. 
  MbRect( double _left, double _bottom, double _right, double _top ) : left(_left), bottom(_bottom), right(_right), top(_top) {}
  /// \ru Конструктор по другому габариту. \en Constructor by another bounding box. 
  MbRect( const MbRect & r ) : left(r.left), bottom(r.bottom), right(r.right), top(r.top) {}
  /// \ru Конструктор по двум диагональным точкам. \en The constructor by two diagonal points. 
  MbRect( const MbCartPoint & p1, const MbCartPoint & p2, bool normalize = true )
  {
    Set(p1.x, p1.y, p2.x, p2.y);
    if ( normalize )
      Normalize();
  }
  /// \ru Конструктор по габариту с последующей трансформацией по матрице. \en Constructor by bounding box with subsequent transformation by the matrix. 
  MbRect( const MbRect & r, const MbMatrix & m ) : left(r.left), bottom(r.bottom), right(r.right), top(r.top) {
    Transform( m );
  }
  /// \ru Конструктор по габариту #MbRect2D. \en Constructor by bounding box #MbRect2D.
  MbRect( const MbRect2D & r );

#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
  ~MbRect(); ///< \ru Деструктор. \en Destructor. 
#endif

  /// \ru Установить заданные значения границ. \en Set given values of bounds. 
  void        Set( double _left, double _bottom, double _right, double _top );
  /// \ru Установить заданные значения границ. \en Set given values of bounds. 
  void        Set( const MbRect   & );
  /// \ru Установить заданные значения границ. \en Set given values of bounds. 
  void        Set( const MbRect2D & );
  /// \ru Установить заданные значения границ. \en Set given values of bounds. 
  void        Set( const MbCartPoint & p1, const MbCartPoint & p2 );
  /// \ru Установить значения границ как окрестность точки. \en Set values of bounds as point neighborhood. 
  void        Set( const MbCartPoint & p, double eps );
  /// \ru Установить значения границ как окрестность точки. \en Set values of bounds as point neighborhood. 
  void        Set( const MbCartPoint & p, double dx, double dy );
  /// \ru Установить нулевым. \en Set to zero. 
  void        SetNull();
  /// \ru Установить пустым ("вывернутым"). \en Set empty ("everted"). 
  void        SetEmpty();
  /// \ru Проверить на пустоту. \en Check for emptiness. 
  bool        IsEmpty() const;
  /// \ru Проверить габарит на вырожденность по оси X. \en Check bounding box for degeneracy by X-axis. 
  bool        IsDegenerateX() const;
  /// \ru Проверить габарит на вырожденность по оси Y. \en Check bounding box for degeneracy by Y-axis. 
  bool        IsDegenerateY() const;
  /// \ru Проверить габариты на равенство. \en Check bounding boxes for equality. 
  bool        IsSame( const MbRect &, double eps ) const;
  /// \ru Проверить габариты на равенство. \en Check bounding boxes for equality. 
  bool        IsSame( const MbRect &, double xeps, double yeps ) const;

  /// \ru Проверить габариты на равенство. \en Check bounding boxes for equality. 
  bool        operator == ( const MbRect & other ) const;
  /// \ru Проверить габариты на неравенство. \en Check bounding boxes for inequality. 
  bool        operator != ( const MbRect & other ) const;
  /// \ru Присвоить значение другого габарита. \en Assign a value of another bounding box. 
  void        operator  = ( const MbRect & other ) { Set( other ); }

  /// \ru Получить верхнюю границу. \en Get top bound. 
  double      GetTop   () const { return top;    }
  /// \ru Получить нижнюю границу. \en Get bottom bound. 
  double      GetBottom() const { return bottom; }
  /// \ru Получить левую границу. \en Get left bound. 
  double      GetLeft  () const { return left;   }
  /// \ru Получить правую границу. \en Get right bound. 
  double      GetRight () const { return right;  }

  /// \ru Установить верхнюю границу. \en Set top bound. 
  void        SetTop   ( double _top    ) { top    = _top;    }
  /// \ru Установить нижнюю границу. \en Set bottom bound. 
  void        SetBottom( double _bottom ) { bottom = _bottom; }
  /// \ru Установить левую границу. \en Set left bound. 
  void        SetLeft  ( double _left   ) { left   = _left;   }
  /// \ru Установить правую границу. \en Set right bound. 
  void        SetRight ( double _right  ) { right  = _right;  }

  /// \ru Получить минимум по X. \en Get minimum by X. 
  double      GetXMin() const { return left;   }
  /// \ru Получить минимум по Y. \en Get minimum by Y. 
  double      GetYMin() const { return bottom; }
  /// \ru Получить максимум по X. \en Get maximum by X. 
  double      GetXMax() const { return right;  }
  /// \ru Получить максимум по Y. \en Get maximum by Y. 
  double      GetYMax() const { return top;    }

  /// \ru Получить середину по X. \en Get middle by X. 
  double      GetXMid() const { return 0.5 * ( left + right ); }
  /// \ru Получить середину по Y. \en Get middle by Y. 
  double      GetYMid() const { return 0.5 * ( bottom + top ); }

  /// \ru Установить минимум по X. \en Set minimum by X. 
  void        SetXMin( double s ) { left   = s; }
  /// \ru Установить максимум по X. \en Set maximum by X. 
  void        SetYMin( double s ) { bottom = s; }
  /// \ru Установить минимум по Y. \en Set minimum by Y. 
  void        SetXMax( double s ) { right  = s; }
  ///< \ru Установить максимум по Y. \en Set maximum by Y. 
  void        SetYMax( double s ) { top    = s; }

  /// \ru Найти ширину габарита. \en Find width of bounding box. 
  double      Width()      const { return right - left; }
  /// \ru Найти высоту габарита. \en Find height of bounding box. 
  double      Height()     const { return top - bottom; }
  /// \ru Дать длину по X. \en Get length by X. 
  double      GetLengthX() const { return right - left; } //-V524
  /// \ru Дать длину по Y. \en Get length by Y. 
  double      GetLengthY() const { return top - bottom; } //-V524
  /// \ru Дать половину периметра. \en Get half of perimeter. 
  double      GetLength( double eps )  const;
  /// \ru Дать площадь. \en Get area. 
  double      GetSquare( double eps ) const;
  /// \ru Дать длину диагонали. \en Get the diagonal length. 
  double      GetDiagonal() const { return ::_hypot( left - right, top - bottom ); }

  /// \ru Вычислить габарит пересечения двух габаритов. \en Calculate bounding box of two bounding boxes intersection. 
  bool        Intersection( const MbRect & rect1, const MbRect & rect2, double eps = Math::LengthEps ); 
  /// \ru Вычислить суммарный габарит двух габаритов. \en Calculate bounding box enclosing two bounding boxes. 
  bool        Union       ( const MbRect & rect1, const MbRect & rect2 ); 

  /// \ru Проверить принадлежность габариту заданной точки. \en Check if the bounding box contains the given point. 
  bool        Contains( const MbCartPoint & p, double eps = Math::LengthEps ) const; 
  /// \ru Проверить принадлежность габариту заданной точки. \en Check if the bounding box contains the given point. 
  bool        Contains( double x, double y, double eps = Math::LengthEps ) const; 
  /// \ru Проверить принадлежность габариту заданной точки. \en Check if the bounding box contains the given point. 
  bool        Contains( const MbCartPoint & p, double xeps, double yeps ) const; 
  /// \ru Проверить принадлежность габариту заданной точки. \en Check if the bounding box contains the given point. 
  bool        Contains( double x, double y, double xeps, double yeps ) const; 
  /// \ru Проверить принадлежность габариту заданной координаты по X. \en Check if the bounding box contains the given X coordinate. 
  bool        ContainsX( double x, double eps = Math::LengthEps ) const ; 
  /// \ru Проверить принадлежность габариту заданной координаты по Y. \en Check if the bounding box contains the given Y coordinate. 
  bool        ContainsY( double y, double eps = Math::LengthEps ) const ;
  /// \ru Вычислить коды расположения точки относительно прямоугольника. \en Calculate the codes of a point location relative to the rectangle. 
  void        OutCodes( const MbCartPoint & p, unsigned int & outcodes, double eps = METRIC_PRECISION ) const;

  /** \brief \ru Вычислить расстояние до ближайшей границы габаритного прямоугольника.
             \en Calculate the distance to the nearest boundary of the bounding box. \~
    \details \ru Найденное расстояние до ближайшей границы имеет отрицательное значение, если точка находится внутри, и положительное - если снаружи.
             \en The calculated distance is negative if the point is inside, and is positive if it is outside. \~
    \param[in] point - \ru Исследуемая точка.
                       \en The investigated point. \~
    \return \ru Возвращает расстояние до границы.
            \en Returns the distance to the boundary. \~
  */
  double      DistanceToPoint( const MbCartPoint & point ) const;
  /// \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  double      DistanceToPoint( const MbCartPoint & to,
                               uint & outcodes ) const;
  /// \ru Вычислить квадрат расстояния до точки. \en Calculate the square of distance to a point.
  double      DistanceToPoint2( const MbCartPoint & pnt ) const;

  /** \brief \ru Вычислить расстояние до куба. 
             \en Calculate the distance to the cube. \~
    \details \ru Возвращается ноль, если кубы пересекаются или один содержится в другом.
             \en It returns zero if the cubes intersect or one is contained in the other. \~
    \param[in] cube - \ru Другой куб.
                      \en Other cube. \~
    \param[in] eps - \ru Метрическая точность.
                     \en A metric tolerance. \~
    \return \ru Возвращает расстояние до границы.
            \en Returns the distance to the boundary. \~
  */
  double      DistanceToCube( const MbRect & cube, double eps = Math::metricRegion ) const;

  /// \ru Вычислить минимальное и максимальное расстояния до точки. \en Calculate minimal and maximal distances to a point. 
  void        CalcDistances( const MbCartPoint & to,
                             double & dmin, double & dmax,
                             uint & outcodes ) const;
  // \ru Пересекается ли габарит с другим габаритом. \en Whether the bounding box intersects with another bounding box. 
  /// \ru Проверить, пересекается ли габарит с другим габаритом с признаком пересечения. \en Whether the bounding box intersects with another bounding box with intersection attribute. 
  bool        Intersect( const MbRect &, TaeTwoRectPos & ) const; 
  /// \ru Проверить пересекается ли габарит с другим габаритом. \en Whether the bounding box intersects with another bounding box. 
  bool        Intersect( const MbRect & other, double eps = Math::LengthEps ) const;
  /// \ru Проверить пересекается ли габарит с другим габаритом. \en Whether the bounding box intersects with another bounding box. 
  bool        Intersect( const MbRect & other, double xeps, double yeps ) const;
  /// \ru Сделать другой прямоугольник из этого, сжав его. \en Create new rectangle from the current one by shrinking. 
  MbRect      CompressedBy( double dLeft, double dBottom,
                            double dRight, double dTop) const ;

  /// \ru Нормализовать себя. \en Normalize oneself. 
  MbRect &    Normalize();
  /// \ru Преобразовать по матрице. \en Transform by matrix. 
  void        Transform( const MbMatrix & );
  /// \ru Масштабировать. \en Scale. 
  void        Scale( double sx, double sy );

  /// \ru Включить в себя прямоугольник. \en Enclose a rectangle. 
  MbRect &    operator |= ( const MbRect & );
  /// \ru Включить в себя точку. \en Enclose a point. 
  MbRect &    operator |= ( const MbCartPoint & );
  /// \ru Включить в себя точку. \en Enclose a point. 
  MbRect &    operator |= ( const MbHomogeneous & );
  /// \ru Включить в себя массив точек. \en Enclose an array of points. 
  MbRect &    operator |= ( const SArray<MbCartPoint> & );
  /// \ru Включить в себя массив точек. \en Enclose an array of points. 
  MbRect &    operator |= ( const SArray<MbHomogeneous> & );

  /// \ru Включить в себя точку,заданную как XY. \en Enclose a point specified as XY. 
  void        Include( double x, double y );
  /// \ru Включить в себя координату X. \en Enclose an X-coordinate. 
  void        IncludeX( double x );
  /// \ru Включить в себя координату Y. \en Enclose an Y-coordinate. 
  void        IncludeY( double y );
  /// \ru Включить в себя интервал от X - dx до X + dx. \en Enclose a range from X - dx to X + dx. 
  void        IncludeXInterval( double x, double dx );
  /// \ru Включить в себя интервал от Y - dy до Y + dy. \en Enclose a range from Y - dy to Y + dy. 
  void        IncludeYInterval( double y, double dy );
  // \ru Сдвинуть прямоугольник \en Move rectangle 
  /// \ru Сдвинуть прямоугольник. \en Move rectangle. 
  void        Move( const MbVector & to );
  /// \ru Сдвинуть прямоугольник. \en Move rectangle. 
  void        Move( double dx, double dy );
  /// \ru Масштабировать относительно 0. \en Scale relative to 0. 
  void        Scale( double scale );
  /// \ru Масштабировать относительно точки. \en Scale relative to point. 
  void        Scale( const MbCartPoint & point, double scale );

  /// \ru Расширить прямоугольник. \en Extend rectangle. 
  void        Enlarge( double x, double y );
  /// \ru Расширить прямоугольник во все стороны. \en Extend rectangle in all directions. 
  void        Enlarge( double delta );

  /// \ru Поличить охватывающий прямоугольник. \en Get the covering rectangle. 
  void        GetOusideRect( MbRect & r ) const { r = *this; }
  /// \ru Вернуть точку центра габарита. \en Get center of the bounding box. 
  void        GetCenter( MbCartPoint & p ) const
              {
                p.x = (left + right)  * 0.5; 
                p.y = (top  + bottom) * 0.5; 
              }

  // \ru Вершины габаритного прямоугольника \en Vertices of bounding box 
  //  Y
  //  |
  //  3 - - - 2
  //  |       |
  //  |       |
  //  0 - - - 1 - X
  //

  /// \ru Дать количество вершин. \en Get count of vertices. 
  size_t      GetVerticesCount() const { return 4; } //-V112
  /// \ru Выдать вершину габаритного прямоугольника по индексу от 0 до 3. \en Get vertex of bounding rectangle by index in range from 0 to 3. 
  void        GetVertex( size_t index, MbCartPoint & p ) const;

  /// \ru Получить ссылку на себя. \en Get reference to itself. 
  const MbRect & GetRect() const { return *this; }

  /// \ru Количество координат точки. \en The number of point coordinates. 
  static size_t GetDimension() { return 2; }
  /// \ru Доступ к координате по индексу. \en Access to a coordinate by an index. 
  double        GetMin( size_t k ) const { return k ? bottom : left; }
  /// \ru Доступ к координате по индексу. \en Access to a coordinate by an index. 
  double        GetMax( size_t k ) const { return k ? top : right; }
  /// \ru Дать длина по стороне. \en Get side length. 
  double        GetSideLength( size_t k ) const { return k ? ::fabs( GetLengthY() ) : ::fabs( GetLengthX() ); }

  /// \ru Получить ссылку на себя. \en Get reference to itself. 
  const MbRect & GetCube() const { return *this; }

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbRect, MATH_FUNC_EX )
  DECLARE_NEW_DELETE_CLASS( MbRect )
  DECLARE_NEW_DELETE_CLASS_EX( MbRect )
}; // MbRect


//------------------------------------------------------------------------------
// \ru Установить заданные значения границ \en Set given values of bounds 
// ---
inline
void MbRect::Set( double _left, double _bottom, double _right, double _top )
{
  left   = _left;
  top    = _top;
  right  = _right;
  bottom = _bottom;
}


//------------------------------------------------------------------------------
// \ru Установить заданные значения границ \en Set given values of bounds 
// ---
inline
void MbRect::Set( const MbRect & r )
{
  left   = r.left;
  top    = r.top;
  right  = r.right;
  bottom = r.bottom;
}


//------------------------------------------------------------------------------
// \ru Установить значения границ как окрестность точки. \en Set values of bounds as point neighborhood. 
// ---
inline
void MbRect::Set( const MbCartPoint & p1, const MbCartPoint & p2  )
{
  left   = std_min( p1.x, p2.x );
  right  = std_max( p1.x, p2.x );
  bottom = std_min( p1.y, p2.y );
  top    = std_max( p1.y, p2.y );
}


//------------------------------------------------------------------------------
// \ru Установить значения границ как окрестность точки. \en Set values of bounds as point neighborhood. 
// ---
inline
void MbRect::Set( const MbCartPoint & p, double eps )
{
  eps = ::fabs( eps );
  left   = p.x - eps;
  right  = p.x + eps;
  bottom = p.y - eps;
  top    = p.y + eps;
}


//------------------------------------------------------------------------------
// \ru Установить значения границ как окрестность точки. \en Set values of bounds as point neighborhood. 
// ---
inline
void MbRect::Set( const MbCartPoint & p, double dx, double dy )
{
  dx = ::fabs( dx );
  dy = ::fabs( dy );

  left   = p.x - dx;
  right  = p.x + dx;
  bottom = p.y - dy;
  top    = p.y + dy;
}


//------------------------------------------------------------------------------
// \ru Установить нулевым \en Set to zero 
// ---
inline
void MbRect::SetNull() {
  left = top = right = bottom = 0;
}


//------------------------------------------------------------------------------
// \ru Установить пустым ("вывернутым") \en Set empty ("everted") 
// ---
inline
void MbRect::SetEmpty() {
  Set( MB_MAXDOUBLE, MB_MAXDOUBLE, -MB_MAXDOUBLE, -MB_MAXDOUBLE );
}


//------------------------------------------------------------------------------
// \ru Проверка на пустоту \en Check for emptiness 
// ---
inline
bool MbRect::IsEmpty() const {
  return ( left > right ) || ( bottom > top );
}


//------------------------------------------------------------------------------
// \ru Проверка габарита по оси 0X \en Check bounding box by 0X-axis 
// ---
inline
bool MbRect::IsDegenerateX() const {
  return fabs( right - left ) < Math::LengthEps;
}


//------------------------------------------------------------------------------
// \ru Проверка габарита по оси 0Y \en Check bounding box by 0Y-axis 
// ---
inline
bool MbRect::IsDegenerateY() const {
  return ::fabs( top - bottom ) < Math::LengthEps;
}


//------------------------------------------------------------------------------
// \ru Проверка равенства с другим прямоугольником \en Check for equality with another rectangle 
// ---
inline
bool MbRect::IsSame( const MbRect & other, double eps ) const
{
  return ::fabs( other.left   - left   ) < eps &&
         ::fabs( other.right  - right  ) < eps &&
         ::fabs( other.top    - top    ) < eps &&
         ::fabs( other.bottom - bottom ) < eps;
}


//------------------------------------------------------------------------------
// \ru Проверка равенства с другим прямоугольником \en Check for equality with another rectangle 
// ---
inline
bool MbRect::IsSame( const MbRect & other, double xeps, double yeps ) const
{
  return ::fabs( other.left   - left   ) < xeps &&
         ::fabs( other.right  - right  ) < xeps &&
         ::fabs( other.top    - top    ) < yeps &&
         ::fabs( other.bottom - bottom ) < yeps;
}


//------------------------------------------------------------------------------
// \ru Проверка равенства с другим прямоугольником \en Check for equality with another rectangle 
// ---
inline
bool MbRect::operator == ( const MbRect & other ) const
{
  return ::fabs( other.left   - left   ) < Math::LengthEps &&
         ::fabs( other.right  - right  ) < Math::LengthEps &&
         ::fabs( other.top    - top    ) < Math::LengthEps &&
         ::fabs( other.bottom - bottom ) < Math::LengthEps;
}


//------------------------------------------------------------------------------
// \ru Проверка неравенства с другим прямоугольником \en Check for inequality with another rectangle 
// ---
inline
bool MbRect::operator != ( const MbRect & other ) const {
  return !( other == *this );
}


//------------------------------------------------------------------------------
// \ru Проверка на то, что заданная точка лежит внутри прямоугольника \en Check that a given point is inside the rectangle 
// ---
inline
bool MbRect::Contains( const MbCartPoint & p, double eps ) const
{
  return ( p.x >= (left   - eps) ) && ( p.x <= (right  + eps) ) &&
         ( p.y >= (bottom - eps) ) && ( p.y <= (top    + eps) );
}

//------------------------------------------------------------------------------
// \ru Проверка на то, что заданная точка лежит внутри прямоугольника \en Check that a given point is inside the rectangle 
// ---
inline
bool MbRect::Contains( double x, double y, double eps ) const
{
  return ( x >= (left   - eps) ) && ( x <= (right  + eps) ) &&
         ( y >= (bottom - eps) ) && ( y <= (top    + eps) );
}


//------------------------------------------------------------------------------
// \ru Проверка на то, что заданная точка лежит внутри прямоугольника \en Check that a given point is inside the rectangle 
// ---
inline
bool MbRect::Contains( const MbCartPoint & p, double xeps, double yeps ) const
{
  return ( p.x >= (left   - xeps) ) && ( p.x <= (right  + xeps) ) &&
         ( p.y >= (bottom - yeps) ) && ( p.y <= (top    + yeps) );
}


//------------------------------------------------------------------------------
// \ru Проверка на то, что заданная точка лежит внутри прямоугольника \en Check that a given point is inside the rectangle 
// ---
inline
bool MbRect::Contains( double x, double y, double xeps, double yeps ) const
{
  return ( x >= (left   - xeps) ) && ( x <= (right  + xeps) ) &&
         ( y >= (bottom - yeps) ) && ( y <= (top    + yeps) );
}


//------------------------------------------------------------------------------
// \ru Проверка на то, что заданная координата X лежит внутри прямоугольника \en Check that given X coordinate is inside the rectangle 
// ---
inline
bool MbRect::ContainsX( double x, double eps ) const {
  return ( x > left   - eps ) && ( x < right  + eps );
}


//------------------------------------------------------------------------------
// \ru Проверка на то, что заданная координата Y лежит внутри прямоугольника \en Check that given Y coordinate is inside the rectangle 
// ---
inline
bool MbRect::ContainsY( double y, double eps ) const {
  return ( y > bottom - eps ) && ( y < top    + eps );
}


//------------------------------------------------------------------------------
// \ru Вычисление кодов расположения точки относительно прямоугольника \en Calculate codes of a point location relative to the rectangle 
// \ru Бит 0 - точка слева от окна \en Bit 0 - point to the left of a window 
// \ru Бит 1 - точка справа от окна \en Bit 1 - point to the right of a window 
// \ru Бит 2 - точка ниже окна \en Bit 2 - the point is lower than a window 
// \ru Бит 3 - точка выше окна \en Bit 3 - the point is higher than a window 
// ---
inline
void MbRect::OutCodes( const MbCartPoint & p, unsigned int & outcodes, double eps ) const
{
  outcodes =      ( p.x < left   - eps ) |
              ( ( ( p.x > right  + eps ) << 1 ) & 0x2 ) |
              ( ( ( p.y < bottom - eps ) << 2 ) & 0x4 ) | //-V112
              ( ( ( p.y > top    + eps ) << 3 ) & 0x8 );

}


//------------------------------------------------------------------------------
// \ru Расстояние до точки: если расстояние < 0, то точка лежит внутри \en Distance to a point: if distance is less than 0 then point is inside  
// ---
inline
double MbRect::DistanceToPoint( const MbCartPoint & pnt ) const
{
  double dx = std_max( left-pnt.x, pnt.x-right );
  double dy = std_max( bottom-pnt.y, pnt.y-top );
  return std_max(dx,dy);
}


//------------------------------------------------------------------------------
// \ru Квадрат расстояния до точки \en square of distance to a point
// ---
inline
double MbRect::DistanceToPoint2( const MbCartPoint & pnt ) const
{
  double dx = std_min( 0., std_min( pnt.x - left, right - pnt.x ) );
  double dy = std_min( 0., std_min( pnt.y - bottom, top - pnt.y ) );
  return dx * dx + dy * dy;
}


//------------------------------------------------------------------------------
// \ru Расстояние до точки \en Distance to a point 
// ---
inline
double MbRect::DistanceToPoint( const MbCartPoint & to,
                                uint & outcodes ) const
{
  if ( IsEmpty() ) {
    outcodes = 0xFFFF;
    return 1E+6;
  }

  OutCodes( to, outcodes ); // \ru Вычисление кодов расположения точки \en Calculate codes of a point location 

  double dx = std_min( fabs( to.x - left), fabs( to.x - right) );
  double dy = std_min( fabs( to.y - top),  fabs( to.y - bottom) );

  return ( outcodes == 0x0 ) ? std_min( dx, dy ) : // \ru Точка внутри прямоугольника \en Point is inside the rectangle 
           ( outcodes == 0x1 || outcodes == 0x2 ) ? dx : // \ru Точка по Y - внутри, по X - вне прямоугольника \en Point is inside rectangle by Y, outside by X 
             ( outcodes == 0x4 || outcodes == 0x8 ) ? dy : // \ru Точка по X - внутри, по Y - вне прямоугольника //-V112 \en Point is inside rectangle by X, outside by Y //-V112 
               ::_hypot( dx, dy ); // \ru Точка по X и по Y - вне прямоугольника \en Point is outside the rectangle by X and Y 
}


//------------------------------------------------------------------------------
// \ru Вычисление минимального и максимального расстояний до точки \en Calculate minimal and maximal distances to point 
// ---
inline
void MbRect::CalcDistances( const MbCartPoint & to,
                            double & dmin, double & dmax,
                            uint & outcodes ) const
{
  dmin = DistanceToPoint( to, outcodes );

  if ( outcodes != 0xFFFF ) { // \ru Прямоугольник не вырожден \en Rectangle is not degenerate 
    dmax   = ::_hypot( to.x - left, to.y - bottom );

    double d = ::_hypot( to.x - left, to.y - top );
    if ( d > dmax ) dmax = d;

    d = ::_hypot( to.x - right, to.y - bottom );
    if ( d > dmax ) dmax = d;

    d = ::_hypot( to.x - right, to.y - top );
    if ( d > dmax ) dmax = d;
  }
}


//------------------------------------------------------------------------------
// \ru Сделать другой прям-к из этого, сжав его \en Create a new rectangle from the current one by shrinking 
// ---
inline
MbRect MbRect::CompressedBy( double dLeft, double dBottom, double dRight, double dTop) const
{
  if ( IsEmpty() )
    return MbRect();
  return MbRect(left+dLeft, bottom+dBottom, right-dRight, top-dTop);
}


//------------------------------------------------------------------------------
// \ru Нормализовать себя \en Normalize oneself 
// ---
inline
MbRect & MbRect::Normalize()
{
  double c = UNDEFINED_DBL;
  if ( left > right ) { c = left; left = right; right = c;  }
  if ( top < bottom ) { c = top;  top = bottom; bottom = c; }
  return *this;
}


//------------------------------------------------------------------------------
// \ru Включить в себя точку \en Enclose a point 
// ---
inline
MbRect & MbRect::operator |=( const MbCartPoint & p )
{
  Include( p.x, p.y );
  return *this;
}


//------------------------------------------------------------------------------
// \ru Включить в себя точку \en Enclose a point 
// ---
inline
MbRect & MbRect::operator |=( const MbHomogeneous & p )
{
  Include( p.x, p.y );
  return *this;
}


//------------------------------------------------------------------------------
// \ru Включить в себя точку,заданную как XY \en Enclose a point secified as XY 
// ---
inline
void MbRect::Include( double x, double y )
{
  left   = std_min(left,   x);
  bottom = std_min(bottom, y);
  right  = std_max(right,  x);
  top    = std_max(top,    y);
}


//------------------------------------------------------------------------------
// \ru Включить в себя координату X \en Enclose an X-coordinate 
// ---
inline
void MbRect::IncludeX( double x )
{
  left   = std_min(left,   x);
  right  = std_max(right,  x);
}


//------------------------------------------------------------------------------
// \ru Включить в себя координату Y \en Enclose an Y-coordinate 
// ---
inline
void MbRect::IncludeY( double y )
{
  bottom = std_min(bottom, y);
  top    = std_max(top,    y);
}


//------------------------------------------------------------------------------
// \ru Включить в себя интервал от X - dx до X + dx \en Enclose a range from X - dx to X + dx 
// ---
inline
void MbRect::IncludeXInterval( double x, double dx )
{
  left   = std_min( left,  x - dx );
  right  = std_max( right, x + dx );
}


//------------------------------------------------------------------------------
// \ru Включить в себя интервал от Y - dy до Y + dy \en Enclose a range from Y - dy to Y + dy 
// ---
inline
void MbRect::IncludeYInterval( double y, double dy ) {
  bottom = std_min( bottom, y - dy );
  top    = std_max( top,    y + dy );
}


//------------------------------------------------------------------------------
// \ru Включить в себя прямоугольник \en Enclose a rectangle 
// ---
inline
MbRect& MbRect::operator |= ( const MbRect & other )
{
  left   = std_min(left,   other.left);
  bottom = std_min(bottom, other.bottom);
  right  = std_max(right,  other.right);
  top    = std_max(top,    other.top);

  return *this;
}


//------------------------------------------------------------------------------
// \ru Сдвинуть прямоугольник \en Move rectangle 
// ---
inline
void MbRect::Move( const MbVector & to ) {
  Move( to.x, to.y );
}


//------------------------------------------------------------------------------
// \ru Сдвинуть прямоугольник \en Move rectangle 
// ---
inline
void MbRect::Move( double dx, double dy )
{
  if ( !IsEmpty() ) {
    left   += dx;
    right  += dx;
    top    += dy;
    bottom += dy;
  }
}


//------------------------------------------------------------------------------
// \ru Промасштабировать относительно 0 \en Scale relative to 0 
// ---
inline
void MbRect::Scale( double scale )
{
  if ( !IsEmpty() ) {
    left   *= scale;
    right  *= scale;
    top    *= scale;
    bottom *= scale;
  }
}


//------------------------------------------------------------------------------
// \ru Расширить прямоугольник \en Extend rectangle 
// ---
inline
void MbRect::Enlarge( double x, double y )
{
  if ( !IsEmpty() ) {
    left   -= x;
    right  += x;
    bottom -= y;
    top    += y;
  }
}


//------------------------------------------------------------------------------
// \ru Расширить прямоугольник во все стороны \en Extend the rectangle in all directions 
// \ru НЕ ПРОВЕРЯЕТСЯ вырожденность прямоугольников !!! \en Rectangles degeneracy IS NOT CHECKED !!! 
// ---
inline
void MbRect::Enlarge( double delta )
{
  left   -= delta;
  right  += delta;
  bottom -= delta;
  top    += delta;
}


//------------------------------------------------------------------------------
// \ru Проверка, пересекается ли прям-к с другим прям-ком \en Check if rectangle intersect another rectangle 
// \ru НЕ ПРОВЕРЯЕТСЯ вырожденность прямоугольников !!! \en Rectangles degeneracy IS NOT CHECKED !!! 
// ---
inline
bool MbRect::Intersect( const MbRect & other, double eps ) const
{
  return std_max( left,   other.left   ) < std_min( right, other.right ) + eps &&
         std_max( bottom, other.bottom ) < std_min( top,   other.top   ) + eps;
}


//------------------------------------------------------------------------------
// \ru Проверка, пересекается ли прям-к с другим прям-ком \en Check if the rectangle intersect another rectangle 
// \ru НЕ ПРОВЕРЯЕТСЯ вырожденность прямоугольников !!! \en Rectangles degeneracy IS NOT CHECKED !!! 
// ---
inline
bool MbRect::Intersect( const MbRect & other, double xeps, double yeps ) const
{
  return std_max( left,   other.left   ) < std_min( right, other.right ) + xeps &&
         std_max( bottom, other.bottom ) < std_min( top,   other.top   ) + yeps;
}


/** \} */





#endif

