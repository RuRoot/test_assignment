////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Габаритный куб геометрического объекта.
         \en The bounding box (cube) of a geometric object. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_CUBE_H
#define __MB_CUBE_H


#include <mb_cart_point3d.h>
#include <io_base.h>
#include <mb_cube_tree.h>
#include <templ_s_array.h>
#include <utility>
#include <vector>


constexpr size_t CUBE_CONTROL_POINTS_COUNT = 26; ///< \ru Количество характерных точек куба. \en The number of control points of the bounding box.
constexpr size_t CUBE_VERTEX_COUNT         = 8;  ///< \ru Количество вершин куба. \en The number of the bounding box vertices.
constexpr size_t CUBE_EDGES_COUNT          = 12; ///< \ru Количество рёбер куба. \en The number of the bounding box edges.
constexpr size_t CUBE_FACES_COUNT          = 6;  ///< \ru Количество граней куба. \en The number of the bounding box faces.


class   MbRect;
class   MbRect1D;
class   MbCube;
class   MbPlacement3D;
class   MbCurve3D;
class   MbSurface;

namespace c3d // namespace C3D
{
typedef std::pair<MbCube, size_t>                             CubeIndex;            ///< \ru Габаритный куб и индекс. \en Bounding box and index.
typedef std::pair<size_t, MbCube>                             IndexCube;            ///< \ru Индекс и габаритный куб. \en Index and bounding box.
typedef std::pair<MbCube *, size_t>                           CubePtrIndex;         ///< \ru Габаритный куб и индекс. \en Bounding box and index.
typedef std::pair<size_t, MbCube *>                           IndexCubePtr;         ///< \ru Индекс и габаритный куб. \en Index and bounding box.
typedef std::pair<const MbCube *, size_t>                     ConstCubePtrIndex;    ///< \ru Габаритный куб и индекс. \en Bounding box and index.
typedef std::pair<size_t, const MbCube *>                     IndexConstCubePtr;    ///< \ru Индекс и габаритный куб. \en Index and bounding box.

typedef std::vector<CubePtrIndex>                             CubesPtrIndices;      ///< \ru Вектор габаритных кубов и индексов. \en Vector of bounding boxes and indices.
typedef std::vector<ConstCubePtrIndex>                        ConstCubesPtrIndices; ///< \ru Вектор габаритных кубов и индексов. \en Vector of bounding boxes and indices.
typedef std::vector<MbCube>                                   CubesVector;          ///< \ru Вектор габаритных кубов. \en Vector of bounding boxes.
typedef std::vector<IndexCube>                                IndexCubeVector;      ///< \ru Вектор индексов и габаритных кубов. \en Vector of indices and bounding boxes.
typedef std::vector<CubeIndex>                                CubeIndexVector;      ///< \ru Вектор индексов и габаритных кубов. \en Vector of indices and bounding boxes.
typedef MbCubeTree<MbCube, MbCube, MbCartPoint3D, MbVector3D> CubesTree;            ///< \ru Дерево габаритных кубов. \en Tree of bounding boxes.
} // namespace C3D


//------------------------------------------------------------------------------
/** \brief \ru Габаритный параллелепипед.
           \en The bounding box. \~
  \details \ru Габаритный параллелепипед - это область 3D-пространства, ограниченная
    прямым параллелепипедом, грани которого параллельным плоскостям системы координат.\n
    Используется для быстрой оценки близости или не пересечения трёхмерных объектов,
    содержащихся в параллелепипеде. Габаритный параллелепипед описывается парой точек, 
    расположенных на главной диагонали куба.
           \en The bounding box is a domain (block) of 3D-space bounded by parallelepiped 
    with edges parallel to the axes of coordinate system.\n
    It is used for quick estimation of the proximity or non-intersection of three-dimensional objects, 
    which are contained in the "box". Faces of "box" are parallel to planes of coordinate system.
    The bounding box is described by a pair of points which are located on the main diagonal of a box. \~
  \ingroup Mathematic_Base_3D
*/
// ---
class MATH_CLASS MbCube {
public :
  MbCartPoint3D pmin; ///< \ru Точка диагонали параллелепипеда с минимальными  координатами. \en A point of a box diagonal with minimal coordinates. 
  MbCartPoint3D pmax; ///< \ru Точка диагонали параллелепипеда с максимальными координатами. \en A point of a box diagonal with maximal coordinates. 

public :
  /// \ru Пустой конструктор. \en The empty constructor. 
  MbCube() { SetEmpty(); }
  /// \ru Конструктор копирования. \en Copy constructor. 
  MbCube( const MbCube & init ) : pmin( init.pmin ), pmax( init.pmax ) {}
  /// \ru Конструктор по координатам \en The constructor by coordinates 
  /**
    \param[in] xmin, ymin, zmin - \ru Координаты точки угла куба с минимальными  координатами.
                                  \en Coordinates of a box corner point with minimal coordinates. \~
    \param[in] xmax, ymax, zmax - \ru Координаты точки угла куба с максимальными  координатами.
                                  \en Coordinates of a box corner point with maximal coordinates. \~
    \param[in] normalize - \ru Нормализовать себя.
                           \en Normalize itself. \~
  */
  MbCube( double xmin, double ymin, double zmin, double xmax, double ymax, double zmax, bool normalize = false ) : pmin( xmin, ymin, zmin ), pmax( xmax, ymax, zmax ) { if ( normalize ) Normalize(); }
  /// \ru Конструктор по двум точкам. \en The constructor by two points. 
  /**
    \param[in] p0 - \ru Точка угла куба с минимальными  координатами.
                    \en A point of a box corner with minimal coordinates. \~
    \param[in] p1 - \ru Точка угла куба с максимальными  координатами.
                    \en A point of a box corner with maximal coordinates. \~
    \param[in] normalize - \ru Нормализовать себя.
                           \en Normalize itself. \~
  */
  MbCube( const MbCartPoint3D & p0, const MbCartPoint3D & p1, bool normalize = false ) : pmin( p0 ), pmax( p1 ) { if ( normalize ) Normalize(); }
  
  /**
    \brief \ru Проверка на пустоту. \en The check for emptiness.
    \details \ru Габаритный параллелепипед считается пустым, если он не содержит ни одной точки 3D-пространства.
             \en A bounding box is empty if it doesn't contain any points of 3D-space \~
  */
  bool      IsEmpty() const;
  /// \ru Установить пустым ("вывернутым"). \en Set empty ("reverted"). 
  void      SetEmpty();

  /// \ru Инициализировать по координатам. \en Initialize by coordinates. 
  /**
    \param[in] xmin, ymin, zmin - \ru Координаты точки угла куба с минимальными  координатами.
                                  \en Coordinates of a box corner point with minimal coordinates. \~
    \param[in] xmax, ymax, zmax - \ru Координаты точки угла куба с максимальными  координатами.
                                  \en Coordinates of a box corner point with maximal coordinates. \~
    \param[in] normalize - \ru Нормализовать себя.
                           \en Normalize itself. \~
  */
  void      Set( double xmin, double ymin, double zmin,
                 double xmax, double ymax, double zmax,
                 bool normalize = false );
  /// \ru Инициализировать по двум точкам. \en Initialize by two points. 
  /**
    \param[in] p0 - \ru Точка угла куба с минимальными  координатами.
                    \en A point of a box corner with minimal coordinates. \~
    \param[in] p1 - \ru Точка угла куба с максимальными  координатами.
                    \en A point of a box corner with maximal coordinates. \~
    \param[in] normalize - \ru Нормализовать себя.
                           \en Normalize itself. \~
  */
  void      Set( const MbCartPoint3D & p0, const MbCartPoint3D & p1, bool normalize = false );

  /** 
    \ru \name Функции доступа к полям
    \en \name Functions for access to fields.
    \{ */
  /// \ru Установить минимальную координату по X. \en Set the minimal coordinate by X. 
  void      SetXMin( double s ) { pmin.x = s; }
  /// \ru Установить минимальную координату по Y. \en Set the minimal coordinate by Y. 
  void      SetYMin( double s ) { pmin.y = s; }
  /// \ru Установить минимальную координату по Z. \en Set the minimal coordinate by Z. 
  void      SetZMin( double s ) { pmin.z = s; }
  /// \ru Установить максимальную координату по X. \en Set the maximal coordinate by X. 
  void      SetXMax( double s ) { pmax.x = s; }
  /// \ru Установить максимальную координату по Y. \en Set the maximal coordinate by Y. 
  void      SetYMax( double s ) { pmax.y = s; }
  /// \ru Установить максимальную координату по Z. \en Set the maximal coordinate by Z. 
  void      SetZMax( double s ) { pmax.z = s; }

  /// \ru Дать минимальную координату по X. \en Give the minimal coordinate by X. 
  double    GetXMin() const { return pmin.x; }
  /// \ru Дать минимальную координату по Y. \en Give the minimal coordinate by Y. 
  double    GetYMin() const { return pmin.y; }
  /// \ru Дать минимальную координату по Z. \en Give the minimal coordinate by Z. 
  double    GetZMin() const { return pmin.z; }
  /// \ru Дать максимальную координату по X. \en Give the maximal coordinate by X. 
  double    GetXMax() const { return pmax.x; }
  /// \ru Дать максимальную координату по Y. \en Give the maximal coordinate by Y. 
  double    GetYMax() const { return pmax.y; }
  /// \ru Дать максимальную координату по Z. \en Give the maximal coordinate by Z. 
  double    GetZMax() const { return pmax.z; }
  /** \} */

  /// \ru Инициализировать по другому габариту. \en Initialize by another bounding box. 
  void      Init( const MbCube & init ) { Set( init.pmin, init.pmax, false ); }
  /**
    \brief \ru Инициализировать по двум точкам.
           \en Initialize by two points. \~
    \details \ru Инициализированный куб нормализуется.
             \en Initialized box is normalized. \~
    \param[in] p0 - \ru Точка угла куба с минимальными  координатами.
                    \en A point of a box corner with minimal coordinates. \~
    \param[in] p1 - \ru Точка угла куба с максимальными  координатами.
                    \en A point of a box corner with maximal coordinates. \~
  */
  void      Init( const MbCartPoint3D & p0, const MbCartPoint3D & p1 ) { Set( p0, p1, true ); }
  /**
    \brief \ru Добавить габарит.
           \en Add bounding box. \~
    \details \ru Габарит добавляется в локальной системе координат исходного куба.
             \en A bounding box is added in the local coordinate system of the initial box. \~
    \param[in] r - \ru Габарит.
                   \en A bounding box. \~
    \param[in] place - \ru Локальная система координат.
                       \en A local coordinate system. \~
  */
  void      AddRect( const MbRect & r, const MbPlacement3D & place );
  /**
    \brief \ru Проекция на плейсмент.
           \en A projection onto the placement. \~
    \details \ru Вычисляет прямоугольник, охватывающий проекцию куба на плейсмент.
             \en Calculates a rectangle covering a projection of the box onto the placement. \~
    \param[in] place - \ru Локальная система координат.
                       \en A local coordinate system. \~
    \param[out] rect - \ru Прямоугольник, охватывающий искомую проекцию.
                       \en A rectangle covering a required projection. \~
  */
  void      ProjectionRect( const MbPlacement3D & place, MbRect & rect ) const;
  /**
  \brief \ru Проекция на плейсмент вдоль вектора в любом из двух направлений.
         \en A projection onto the placement along a vector in either of two directions. \~
   \details \ru Вычисляет прямоугольник, охватывающий проекцию куба на плейсмент вдоль вектора в любом из двух направлений.
           \en Calculates a rectangle covering a projection of the box onto the placement along a vector in either of two directions. \~
   \param[in] place - \ru Локальная система координат.
                      \en A local coordinate system. \~
   \param[in] dir   - \ru Вектор направления.
                      \en A direction vector. \~
   \param[out] rect - \ru Прямоугольник, охватывающий искомую проекцию.
                      \en A rectangle covering a required projection. \~
  */
  void      ProjectionRect( const MbPlacement3D & place, const MbVector3D & dir, MbRect & rect ) const;

  /// \ru Вычислить параметрический интервал кривой, охватывающий проекцию куба на кривую. \en Calculate a parametric interval of the curve covering the projection of the box on the curve.
  void      ProjectionRect( const MbCurve3D & curve, bool ext, MbRect1D & rect ) const;

  /** 
    \ru \name Перегрузка логических операций.
    \en \name Overload of logical operations.
    \{ */
  /// \ru Присвоить значение. \en Assign a value. 
  void      operator  = ( const MbCube & );
  /// \ru Включить в себя точку. \en Enclose a point. 
  template <class Point>
  MbCube  & operator |= ( const Point & );
  /// \ru Включить в себя габаритный куб. \en Enclose a bounding box. 
  MbCube  & operator |= ( const MbCube & );
  /// \ru Оператор равенства. \en The equality operator. 
  bool      operator == ( const MbCube & ) const;
  /// \ru Оператор неравенства. \en The inequality operator. 
  bool      operator != ( const MbCube & ) const;
  /** \} */

  /// \ru Нормализовать себя. \en Normalize itself. 
  void      Normalize ();

  /** 
    \brief \ru Определить положение куба относительно плоскости.
           \en Define the box position relative to the plane. \~
    \details \ru Определить положение куба относительно плоскости XY локальной системы координат, направление оси Z локальной системы координат при этом не учитывается.
             \en Define the box position relative to the plane XY of a local coordinate system, the Z axis of the local coordinate system is not taken into account here. \~
    \param[in] pl - \ru Локальная система координат, задающая плоскость.
                    \en A local coordinate system which defines a plane. \~
    \param[in] eps - \ru Метрическая точность.
                     \en A metric tolerance. \~
    \return \ru iloc_OnItem - куб пересекает плоскость XY локальной системы координат,\n
            iloc_InItem - куб расположен над плоскостью XY локальной системы координат,\n
            iloc_OutOfItem - куб расположен под плоскостью XY локальной системы координат.
            \en Iloc_OnItem - box intersects the XY plane of a local coordinate system,\n
            iloc_InItem - box is located over the XY plane of a local coordinate system,\n
            iloc_OutOfItem - box is located under the XY plane of a local coordinate system. \~
  */
  MbeItemLocation GetLocation( const MbPlacement3D & pl, double eps ) const; 

  /** 
    \brief \ru Определить положение куба относительно трубы.
           \en Define the box position relative to the tube. \~
    \details \ru Определить, расположен ли куб внутри трубы прямоугольного сечения, 
      заданного прямоугольником в плоскости XY локальной системы координат.
             \en Determine whether the box is inside the tube of rectangular section, 
      given by a rectangle in the XY plane of a local coordinate system. \~
    \param[in] place - \ru Локальная система координат, в в плоскости XY которой лежит сечение трубы.
                       \en A local coordinate system in the XY plane of which a tube section is located. \~
    \param[in] rect - \ru Прямоугольник, задающая сечение трубы.
                      \en A rectangle which defines a tube section. \~
    \param[in] eps - \ru Метрическая точность.
                     \en A metric tolerance. \~
    \return \ru true, если куб расположен внутри трубы.
            \en Returns true if the box is inside the tube. \~
  */
  bool      InsideLocation( const MbPlacement3D & place, MbRect & rect, double eps ) const; 

  /// \ru Включить в себя точку, заданную как XYZ. \en Enclose a point specified as XYZ. 
  /**
    \param[in] x, y, z - \ru Координаты точки, которую требуется включить в габарит.
                         \en Coordinates of a point which has to be included in the box. \~
  */
  void      Include( double x, double y, double z );

  /// \ru Включить в себя точку. \en Enclose a point.
  template <class Point>
  void      Include( const Point & );

  /** 
    \ru \name Булевы операции куба с точкой, линией, плоскостью и другим кубом.
    \en \name The boolean operations of a box with a point, line, plane and another box.
    \{ */


  /// \ru Проверить, лежит ли точка внутри габаритного параллелепипеда. \en Check whether a point is inside the box or not. 
  /** 
    \return \ru true, если лежит.
            \en Returns true if it is inside. \~
  */
  template <class Point>
  bool      Contains( const Point & ) const;

  /// \ru Проверить, лежит ли точка внутри габаритного параллелепипеда. \en Check whether a point is inside the box or not. 
  /** 
    \param[in] eps - \ru Метрическая точность.
                     \en A metric tolerance. \~
    \return \ru true, если лежит.
            \en Returns true if it is inside. \~
  */
  template <class Point>
  bool      Contains( const Point &, double eps ) const;

  /// \ru Проверить, содержит ли один параллелепипед другой. \en Check whether a box is inside another box or not.
  /** 
    \return \ru true, если данный габаритный параллелепипед содержит другой.
            \en Returns true if a box contains another. \~
  */
  bool      Contains( const MbCube & ) const;

  /// \ru Проверить, содержит ли один параллелепипед другой. \en Check whether a box is inside another box or not.
  /**
    \return \ru true, если данный габаритный параллелепипед содержит другой.
            \en Returns true if a box contains another. \~
  */
  bool      Contains( const MbCube &, double eps ) const;

  /// \ru Пересекается ли габаритный параллелепипед с другим параллелепипедом. \en Whether the box intersects another box or not. 
  /** 
    \param[in] other - \ru Другой параллелепипед.
                       \en Another box. \~
    \param[in] eps - \ru Метрическая точность.
                     \en A metric tolerance. \~
    \return \ru true, если пересекается.
            \en Returns true if it intersects. \~
  */
  bool      Intersect( const MbCube & other, double eps = (c3d::MIN_RADIUS + c3d::MIN_RADIUS) ) const; 

  /// \ru Пересекается ли куб с плоскостью XY локальной системы координат. \en Whether the box intersects the XY plane of a local coordinate system or not. 
  /** 
    \param[in] pl - \ru Плейсмент, задающий плоскость.
                    \en A placement which defines a plane. \~
    \param[in] eps - \ru Метрическая точность.
                     \en A metric tolerance. \~
    \return \ru true, если пересекается.
            \en Returns true if it intersects. \~
  */
  bool      Intersect( const MbPlacement3D & pl, double eps = c3d::MIN_RADIUS ) const;

  /** 
    \brief \ru Пересекается ли куб с линией.
           \en Whether the box intersects the line or not. \~
    \details \ru Линия задается точкой и вектором.
             \en A line is given by a point and vector. \~
    \param[in] eps - \ru Метрическая точность.
                     \en A metric tolerance. \~
    \return \ru true, если пересекается.
            \en Returns true if it intersects. \~
  */
  bool      Intersect( const MbCartPoint3D &, const MbVector3D &, double eps = Math::metricRegion ) const;

  /// \ru Найти пересечение прямой с "поверхностью" куба. \en Find an intersection of a line with the box "surface". 
  /**
    \param[in] p - \ru Точка на прямой.
                   \en The point on the line. \~
    \param[in] axis - \ru Вектор, задающий направление прямой.
                      \en A vector which defines the direction of the line. \~
    \param[out] param - \ru Точки пересечения.
                        \en Intersection points. \~
    \param[in] delta - \ru Метрическая точность.
                       \en A metric tolerance. \~
    \return \ru true, если пересечение есть, \n false в противном случае.
            \en Returns true if intersection exists, \n false otherwise. \~
  */
  bool      Intersect( const MbCartPoint3D &  p, 
                       const MbVector3D &     axis,
                             SArray<double> & param, 
                             double           delta = Math::metricRegion ) const; 

  /// \ru Пересечение куба и окружности. \en The intersection of the box with a circle. 
  /**
    \param[in] placement - \ru Плейсмент окружности.
                           \en A circle placement. \~
    \param[in] radius - \ru Радиус окружности.
                        \en The circle radius. \~
    \param[out] param - \ru Точки пересечения.
                        \en Intersection points. \~
    \param[in] delta - \ru Метрическая точность.
                       \en A metric tolerance. \~
    \return \ru true, если пересечение есть, \n false в противном случае.
            \en Returns true if intersection exists, \n false otherwise. \~
  */
  bool      Intersect( const MbPlacement3D &  placement, 
                             double           radius, 
                             SArray<double> & param, 
                             double           delta = Math::metricRegion ) const;

  /// \ru Куб пересечения двух кубов. \en A box of intersection of two boxes. 
  /**
    \param[in] cube1, cube2 - \ru Исходные кубы.
                              \en Input boxes. \~
    \param[in] eps - \ru Метрическая точность.
                     \en A metric tolerance. \~
    \return \ru true, если пересечение есть, \n false в противном случае.
            \en Returns true if intersection exists, \n false otherwise. \~
  */
  bool      Intersection( const MbCube & cube1, const MbCube & cube2, double eps = c3d::MIN_RADIUS );

  /// \ru Куб объединения двух кубов. \en A box of union of two boxes. 
  /**
    \param[in] cube1, cube2 - \ru Исходные кубы.
                              \en Input boxes. \~
    \return \ru false, если итоговый куб пуст, \n true в противном случае.
            \en Returns false if the result box is empty, \n true otherwise. \~
  */
  bool      Union       ( const MbCube & cube1, const MbCube & cube2 ); 
  /** \} */

  /// \ru Дать объем куба. \en Give the volume of a box. 
  double    GetVolume ( double eps = Math::metricRegion ) const;
  /// \ru Дать половину площади граней куба. \en Give half of the area of the box faces. 
  double    GetSquare ( double eps = Math::metricRegion ) const;
  /// \ru Дать размер стороны X куба. \en Give the size of the X side of a box. 
  double    GetLengthX() const { return pmax.x - pmin.x; }
  /// \ru Дать размер стороны Y куба. \en Give the size of the Y side of a box. 
  double    GetLengthY() const { return pmax.y - pmin.y; }
  /// \ru Дать размер стороны Z куба. \en Give the size of the Z side of a box. 
  double    GetLengthZ() const { return pmax.z - pmin.z; }
  /// \ru Дать размер диагонали куба. \en Give the size of box diagonal. 
  double    GetDiagonal() const;

  /** \brief \ru Вычислить расстояние до ближайшей грани габаритного куба.
             \en Calculate the distance to the nearest boundary of the bounding box. \~
    \details \ru Найденное расстояние до ближайшей границы имеет отрицательное значение, если точка находится внутри, и положительное - если снаружи.
             \en The calculated distance is negative if the point is inside, and is positive if it is outside. \~
    \param[in] point - \ru Исследуемая точка.
                       \en The investigated point. \~
    \return \ru Возвращает расстояние до границы.
            \en Returns the distance to the boundary. \~
  */
  double    DistanceToPoint( const MbCartPoint3D & point ) const;
  /// \ru Вычислить квадрат расстояния до точки. \en Calculate the square of distance to a point.
  double    DistanceToPoint2( const MbCartPoint3D & point ) const;

  /** \brief \ru Вычислить расстояние до куба. 
             \en Calculate the distance to the box. \~
    \details \ru Возвращается ноль, если кубы пересекаются или один содержится в другом.
             \en It returns zero if the boxes intersect or one is contained in the other. \~
    \param[in] cube - \ru Другой куб.
                      \en Other cube. \~
    \param[in] eps - \ru Метрическая точность.
                     \en A metric tolerance. \~
    \return \ru Возвращает расстояние до границы.
            \en Returns the distance to the boundary. \~
  */
  double    DistanceToCube( const MbCube & cube, double eps = Math::metricRegion ) const;

  /// \ru Расширить куб во все стороны на величину delta. \en Expand the box in all directions on a "delta" amount. 
  void      Enlarge  ( double delta );
  /// \ru Расширить куб во все стороны на соответствующую величину. \en Expand the box in all directions on a corresponding amount. 
  void      Enlarge  ( double dx, double dy, double dz );
  /// \ru Сдвинуть вдоль вектора. \en Translate along a vector. 
  void      Move     ( const MbVector3D & to );
  /// \ru Преобразовать согласно матрице. \en Transform according to the matrix. 
  void      Transform( const MbMatrix3D & matrix );
  /// \ru Масштабировать. \en Scale. 
  void      Scale    ( double sx, double sy, double sz );
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool      IsSame( const MbCube & other, double accuracy ) const; 

  /// \ru Количество координат точки. \en The number of point coordinates. 
  static size_t GetDimension() { return 3; }
  /// \ru Доступ к минимальной координате по индексу. \en Access to a coordinate by an index. 
  double    GetMin( size_t k ) const { return k ? (--k ? pmin.z : pmin.y) : pmin.x; }
  /// \ru Доступ к максимальной координате по индексу. \en Access to a coordinate by an index. 
  double    GetMax( size_t k ) const { return k ? (--k ? pmax.z : pmax.y) : pmax.x; }
  /// \ru Дать длина по стороне. \en Get side length. 
  double    GetSideLength( size_t k ) const { return k ? (--k ? ::fabs(GetLengthZ()) : ::fabs(GetLengthY())) : ::fabs(GetLengthX()); }

  /// \ru Дать себя. \en Give itself. 
  const MbCube & GetCube() const { return *this; }

  // \ru Общая нумерация характерных точек куба: \en General numeration of box control points:  
  // \ru 0-7 вершины, 8-19 середины рёбер, 20-25 центры граней \en 0-7 vertices, 8-19 middles of edges, 20-25 centers of faces 
  //         Z
  //         |
  //         4------15------7   
  //        /|             /|
  //       12|    25      14|
  //      /  16     20   /  19
  //     5---+--13------6   |
  //     | 22|          | 23|
  //     |   0------11--+---3 - Y
  //     17 /   21      18 /
  //     | 8      24    | 10
  //     |/             |/ 
  //     1-------9------2 
  //    /
  //   X
  // \ru Выдать характерные точки куба \en Give control points of the box 
  /// \ru Количество вершин. \en Number of vertices. 
  size_t    GetVerticesCount() const { return 8; }
  /**
    \brief \ru Выдать все вершины куба.
           \en Give all vertices of the box. \~
    \details \ru Выдать все вершины куба.
             \en Give all vertices of the box. \~
    \param[out] points - \ru Вершины.
                         \en Vertices. \~
    \return \ru Число вершин. Ноль, если куб пуст.
            \en The number of vertices. Null if the box is empty. \~
  */
  size_t    GetVertices( std::vector<MbCartPoint3D> & points ) const;
  /// \ru Выдать вершину куба по индексу. \en Get a box vertex by an index. 
  /**
    \param[in] index - \ru Исходный индекс. 0 <= index <= 7.
                       \en An initial index. 0 <= index <= 7. \~
    \param[out] p - \ru Искомая вершина.
                    \en Required vertex. \~
  */
  void      GetVertex( ptrdiff_t index, MbCartPoint3D & p ) const;
  /// \ru Выдать центр ребра по индексу. \en Give the center of an edge by an index. 
  /**
    \param[in] index - \ru Исходный индекс. 0 <= index <= 12 (общий номер минус CUBE_VERTEX_COUNT).
                       \en An initial index. 0 <= index <= 12 ("general number" minus CUBE_VERTEX_COUNT). \~
    \param[out] p - \ru Координаты центра ребра.
                    \en Coordinates of an edge center. \~
  */
  void      GetEdgeCentre( ptrdiff_t index, MbCartPoint3D & p ) const; 
  /// \ru Выдать центр грани  по индексу. \en Give the center of a face by an index. 
  /**
    \param[in] index - \ru Исходный индекс. 0 <= index <= 5 (общий номер минус CUBE_VERTEX_COUNT минус CUBE_EDGES_COUNT).
                       \en An initial index. 0 <= index <= 5 ("general number" minus CUBE_VERTEX_COUNT minus CUBE_EDGES_COUNT). \~
    \param[out] p - \ru Координаты центра ребра.
                    \en Coordinates of an edge center. \~
  */
  void      GetFaceCentre( ptrdiff_t index, MbCartPoint3D & p ) const;
  /// \ru Выдать центр куба. \en Give the box center. 
  void      GetCentre( MbCartPoint3D & p ) const { p.Set( pmax, 0.5, pmin, 0.5 ); }
  /// \ru Выдать центр куба. \en Give the box center. 
  void      GetCenter( MbCartPoint3D & p ) const { p.Set( pmax, 0.5, pmin, 0.5 ); }
  /// \ru Центр габарита. \en The center of bounding box. 
  MbCartPoint3D Centre() const { return MbCartPoint3D().Set( pmax, 0.5, pmin, 0.5 ); }

  /**
    \brief \ru Дать характерную точку куба по ее номеру.
           \en Give a box control point by its number. \~
    \details \ru Общая нумерация характерных точек куба: 0-7 вершины, 8-19 середины рёбер, 20-25 центры граней.
             \en General numeration of box control points: 0-7 vertices, 8-19 middles of edges, 20-25 centers of faces. \~
    \param[in] pIndex - \ru Номер характерных точек.
                        \en A number of control points. \~
    \param[out] p - \ru Координаты характерной точки.
                    \en Coordinates of control point. \~
    \return \ru false, если куб пуст или индекс принимает недопустимое значение, \n true в противном случае.
            \en Returns false if the box is empty or the index has an invalid value, \n true otherwise. \~
  */
  bool      GetControlPoint( size_t pIndex, MbCartPoint3D & p ) const; 
  /**
    \brief \ru Выдать все характерные точки куба.
           \en Give all control points of the box. \~
    \details \ru Все характерные точки куба: 8 вершин, 12 середин рёбер, 6 центров граней.
             \en All control points of the box: 8 vertices, 12 middles of edges, 6 centers of faces. \~
    \param[out] points - \ru Характерные точки.
                         \en Control points. \~
    \return \ru Число характерных точек. Ноль, если куб пуст.
            \en The number of control points. Null if the box is empty. \~
  */
  size_t    GetControlPoints( SArray<MbCartPoint3D> & points ) const;
  /// \ru Выдать номер ближайшей характерной точки куба. \en Give the number of the nearest control point of the box. 
  /**
    \param[in] p - \ru Исходная точка, к которой ищется ближайшая характерная точка куба.
                   \en An initial point for which the nearest control point of the box is searched. \~
    \return \ru Номер точки.
            \en A number of a point. \~
  */
  size_t    GetNearestControlIndex( const MbCartPoint3D & p ) const;
  /**
    \brief \ru Дать номер противолежащей точки.
           \en Give the number of the opposite point. \~
    \details \ru Для точки куба с номером index дать номер противолежащей точки, 
      которая может использоваться в качестве фиксированной.
             \en For a box point with the "index" number give the number of the opposite point, 
      which can be used as fixed. \~
    \param[in] index - \ru Номер исходной точки.
                       \en The number of the initial point. \~
    \return \ru Номер противолежащей точки.
            \en A number of the opposite point. \~
  */
  size_t    GetFixedControlIndex  ( size_t index ) const; 
  /**
    \brief \ru Рассчитать матрицу деформации.
           \en Calculate a deformation matrix. \~
    \details \ru Матрица деформации рассчитывается по-новому положению point характерной точки куба с индексом pIndex.
             \en A deformation matrix is calculated according to the new position of box control point "point" with the index "pIndex". \~
    \param[in] pIndex - \ru Номер смещаемой точки (0-7 вершины, 8-19 середины рёбер, 20-25 центры граней).
                        \en A number of a moved point (0-7 vertices, 8-19 middles of edges, 20-25 centers of faces). \~
    \param[in] point - \ru Точка, с которой нужно совместить точку куба с номером pIndex.
                       \en A point with which the box point with the "pIndex" number has to be joined. \~
    \param[in] fixedPoint - \ru Неподвижная точка преобразования, используется, если useFixed = true.
                            \en A fixed point. It is used if useFixed = true. \~
    \param[in] useFixed - \ru Использовать неподвижную точку (true), если useFixed = false, то неподвижной будет противолежащая точка куба.
                          \en Use a fixed point (true); if useFixed = false, then the opposite point of the box will be fixed. \~
    \param[in] isotropy - \ru Одинаковые масштабы по осям (true), масштабы, пропорциональны проекциям смещения рассматриваемой точки на стороны куба (false)
                          \en The same scales of the axes (true); the scales are proportional to the shift projections of the considered point on the sides of the box (false) \~
    \param[out] matrix - \ru Рассчитанная матрица преобразования.
                         \en Calculated transformation matrix. \~
    \return \ru true, если искомая матрица была найдена.
            \en Returns true if the matrix was found. \~
  */
  bool      CalculateMatrix( size_t pIndex, const MbCartPoint3D & point, const MbCartPoint3D & fixedPoint,
                             bool useFixed, bool isotropy, MbMatrix3D & matrix ) const;
  public:

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbCube, MATH_FUNC_EX )
  DECLARE_NEW_DELETE_CLASS( MbCube )
  DECLARE_NEW_DELETE_CLASS_EX( MbCube )
};


//------------------------------------------------------------------------------
// \ru Установить пустым ("вывернутым") \en Set empty ("reverted") 
// ---
inline
void MbCube::SetEmpty() 
{
  pmin.x = pmin.y = pmin.z =  MB_MAXDOUBLE;
  pmax.x = pmax.y = pmax.z = -MB_MAXDOUBLE;
}


//------------------------------------------------------------------------------
// \ru Проверка на пустоту \en The check for emptiness. 
// ---
inline
bool MbCube::IsEmpty() const {
  return ( pmin.x > pmax.x ) || ( pmin.y > pmax.y ) || ( pmin.z > pmax.z );
}


//------------------------------------------------------------------------------
// \ru Установить значения куба \en Set a box values. 
// ---
inline
void MbCube::Set( double xmin, double ymin, double zmin,
                  double xmax, double ymax, double zmax,
                  bool normalize )
{
  if ( normalize ) {
    pmin.x = std_min( xmin, xmax );
    pmin.y = std_min( ymin, ymax );
    pmin.z = std_min( zmin, zmax );
    pmax.x = std_max( xmin, xmax );
    pmax.y = std_max( ymin, ymax );
    pmax.z = std_max( zmin, zmax );
  }
  else {
    pmin.x = xmin;
    pmin.y = ymin;
    pmin.z = zmin;
    pmax.x = xmax;
    pmax.y = ymax;
    pmax.z = zmax;
  }
}


//------------------------------------------------------------------------------
// \ru Установить значения куба \en Set a box values. 
// ---
inline
void MbCube::Set( const MbCartPoint3D & p0, const MbCartPoint3D & p1, bool normalize )
{
  if ( normalize ) {
    pmin.x = std_min( p0.x, p1.x );
    pmin.y = std_min( p0.y, p1.y );
    pmin.z = std_min( p0.z, p1.z );
    pmax.x = std_max( p0.x, p1.x );
    pmax.y = std_max( p0.y, p1.y );
    pmax.z = std_max( p0.z, p1.z );
  }
  else {
    pmin = p0;
    pmax = p1;
  }
}


//------------------------------------------------------------------------------
// \ru Присвоение \en Assignment 
// ---
inline
void MbCube::operator = ( const MbCube & other ) {
  pmin = other.pmin;
  pmax = other.pmax;
}


//------------------------------------------------------------------------------
// \ru Включить в себя точку \en Enclose a point. 
// ---
template <class Point>
inline MbCube & MbCube::operator |= ( const Point & p )
{
  pmin.x = std_min( pmin.x, (double)p.x );
  pmin.y = std_min( pmin.y, (double)p.y );
  pmin.z = std_min( pmin.z, (double)p.z );
  pmax.x = std_max( pmax.x, (double)p.x );
  pmax.y = std_max( pmax.y, (double)p.y );
  pmax.z = std_max( pmax.z, (double)p.z );
  return *this;
}


//------------------------------------------------------------------------------
// \ru Включить в себя точку,заданную как XYZ \en Enclose a point specified as XYZ. 
// ---
inline
void MbCube::Include( double x, double y, double z )
{
  pmin.x = std_min( pmin.x, x );
  pmin.y = std_min( pmin.y, y );
  pmin.z = std_min( pmin.z, z );
  pmax.x = std_max( pmax.x, x );
  pmax.y = std_max( pmax.y, y );
  pmax.z = std_max( pmax.z, z );
}


//------------------------------------------------------------------------------
// \ru Включить в себя точку \en Enclose a point 
//---
template <class Point>
inline void MbCube::Include( const Point & pnt ) { 
  Include( pnt.x, pnt.y, pnt.z );
}


//------------------------------------------------------------------------------
// \ru Включить в себя габаритный куб \en Enclose a bounding box. 
// ---
inline
MbCube & MbCube::operator |= ( const MbCube & other )
{
  pmin.x = std_min( pmin.x, other.pmin.x );
  pmin.y = std_min( pmin.y, other.pmin.y );
  pmin.z = std_min( pmin.z, other.pmin.z );
  pmax.x = std_max( pmax.x, other.pmax.x );
  pmax.y = std_max( pmax.y, other.pmax.y );
  pmax.z = std_max( pmax.z, other.pmax.z );
  return *this;
}


//------------------------------------------------------------------------------
// \ru Проверка равенства с другим габаритом. \en The check for equality with another box.
// ---
inline
bool MbCube::operator == ( const MbCube & other ) const 
{
  return c3d::EqualPoints( pmin, other.pmin, Math::lengthEpsilon ) &&
         c3d::EqualPoints( pmax, other.pmax, Math::lengthEpsilon );
}


//------------------------------------------------------------------------------
// \ru Проверка неравенства с другим кубом. \en The check for inequality with another box.
// ---
inline
bool MbCube::operator != ( const MbCube & other) const {
  return !( other == *this );
}


//------------------------------------------------------------------------------
// \ru Проверка на то, что заданная точка лежит внутри куба \en Check whether a given point is inside the box or not 
// ---
template <class Point>
inline bool MbCube::Contains( const Point & p ) const 
{
  return ( ( (double)p.x >= pmin.x ) && ( (double)p.x <= pmax.x ) &&
           ( (double)p.y >= pmin.y ) && ( (double)p.y <= pmax.y ) &&
           ( (double)p.z >= pmin.z ) && ( (double)p.z <= pmax.z ) );
}

//-------------------------------------------------------------------------------
// \ru Проверка на то, что заданная точка лежит внутри габарита. \en Check whether a given point is inside the box or not.
// ---
template <class Point>
inline bool MbCube::Contains( const Point & p, double eps ) const 
{
  return ( ((double)p.x > (pmin.x - eps)) && ((double)p.x < (pmax.x + eps)) &&
           ((double)p.y > (pmin.y - eps)) && ((double)p.y < (pmax.y + eps)) &&
           ((double)p.z > (pmin.z - eps)) && ((double)p.z < (pmax.z + eps)) );
}


//------------------------------------------------------------------------------
/// \ru Проверить, содержит ли один параллелепипед другой. \en Check whether a box is inside another box or not.
//---
inline
bool MbCube::Contains( const MbCube & box ) const
{
  return Contains( box.pmin ) && Contains( box.pmax );
}


//------------------------------------------------------------------------------
/// \ru Проверить, содержит ли один параллелепипед другой. \en Check whether a box is inside another box or not.
//---
inline
bool MbCube::Contains( const MbCube & box, double eps ) const
{
  return Contains( box.pmin, eps ) && Contains( box.pmax, eps );
}


//------------------------------------------------------------------------------
// \ru Нормализовать себя \en Normalize itself 
// ---
inline
void MbCube::Normalize()
{
  double tmp;
  if ( pmin.x > pmax.x )
    { tmp = pmin.x; pmin.x = pmax.x; pmax.x = tmp; }
  if ( pmin.y > pmax.y )
    { tmp = pmin.y; pmin.y = pmax.y; pmax.y = tmp; }
  if ( pmin.z > pmax.z )
    { tmp = pmin.z; pmin.z = pmax.z; pmax.z = tmp; }
}


//------------------------------------------------------------------------------
// \ru Расширить куб во все стороны \en Expand the box in all directions 
// ---
inline
void MbCube::Enlarge( double delta )
{
  pmin.x -= delta;
  pmin.y -= delta;
  pmin.z -= delta;

  pmax.x += delta;
  pmax.y += delta;
  pmax.z += delta;
}


//------------------------------------------------------------------------------
// \ru Расширить куб во все стороны. \en Expand the box in all directions 
// ---
inline
void MbCube::Enlarge( double dx, double dy, double dz )
{
  pmin.x -= dx;
  pmin.y -= dy;
  pmin.z -= dz;

  pmax.x += dx;
  pmax.y += dy;
  pmax.z += dz;
}


//------------------------------------------------------------------------------
// \ru Сдвиг \en Translation 
// ---
inline
void MbCube::Move( const MbVector3D & to )
{
  if ( !IsEmpty() ) {
    pmin.Move( to );
    pmax.Move( to );
  }
}


//------------------------------------------------------------------------------
// \ru Расстояние до точки \en The distance to a point 
// ---
inline
double MbCube::DistanceToPoint( const MbCartPoint3D & pnt ) const
{
  double dx = std_max( pmin.x - pnt.x, pnt.x - pmax.x );
  double dy = std_max( pmin.y - pnt.y, pnt.y - pmax.y );
  double dz = std_max( pmin.z - pnt.z, pnt.z - pmax.z );
  double dd = std_max( dx, dy );
  return std_max( dd, dz );
}


//------------------------------------------------------------------------------
// \ru Квадрат расстояния до точки \en The square of distance to a point 
// ---
inline
double MbCube::DistanceToPoint2( const MbCartPoint3D & pnt ) const
{
  double dx = std_min( 0., std_min( pnt.x - pmin.x, pmax.x - pnt.x ) );
  double dy = std_min( 0., std_min( pnt.y - pmin.y, pmax.y - pnt.y ) );
  double dz = std_min( 0., std_min( pnt.z - pmin.z, pmax.z - pnt.z ) );
  return dx * dx + dy * dy + dz * dz;
}


#endif // __MB_CUBE_H
