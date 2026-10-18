////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Триангуляция.
         \en Triangulation. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MESH_GRID_H
#define __MESH_GRID_H


#include <mesh_primitive.h>
#include <templ_s_array.h>
#include <mesh_float_point3d.h>
#include <mesh_float_point.h>
#include <mesh_triangle.h>
#include <mb_data.h>


class   MbRect;
class   MbPlane;


////////////////////////////////////////////////////////////////////////////////
/** \brief \ru Триангуляция на числах double.
           \en Triangulation on double data. \~
  \details \ru Триангуляция представляет собой набор треугольных и четырёхугольных пластин, стыкующихся друг с другом по общим сторонам.\n
      Триангуляция состоит из согласованных наборов точек, нормалей, параметров триангулируемой поверхности и наборов треугольников и четырехугольников.
    Каждый треугольник - это три номера из набора точек, определяющих вершины треугольника, каждый четырехугольник - это четыре номера из набора точек, определяющих вершины четырехугольника. \n
           \en Triangulation represents a set of triangular and quadrangular plates which are joined to each other by their common sides.\n
      The triangulation consists of a sets of points, normals, surface parameters and a sets of triangles and quadrangles.
    The triangle is represented as three indices from the set of points defining vertices of triangle, the quadrangle is represented as four indices from the set of points defining vertices of quadrangle.\n \~
  \ingroup Polygonal_Objects
*/
////////////////////////////////////////////////////////////////////////////////
class MATH_CLASS MbExactGrid : public MbGrid {
private:
  std::vector<MbCartPoint3D>   points;      ///< \ru Множество контрольных точек триангуляции (согласовано с множеством параметров, если последнее не пустое, или пусто, если не пусто множество параметров). \en Set of control points of triangulation (synchronized with set of parameters if the last is not empty, or empty if the set of parameters isn't empty). 
  std::vector<MbVector3D>      normals;     ///< \ru Множество нормалей в контрольных точках согласовано с множеством контрольных точек. \en Set of normals at control points is synchronized with the set of control points. 
  std::vector<MbCartPoint>     params;      ///< \ru Множество параметров - двумерных точек на параметрической области триангулируемой поверхности (может быть пустым). \en Set of parameters of two-dimensional points in parametric domain of surface being triangulated(can be empty). 
  std::vector<uint32>          escorts;     ///< \ru Множество значений для дополнительной информации в точках. \en The set of values for additional information of points. 

protected:
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en The copy constructor without implementation prevents from copying by default. 
  MbExactGrid( const MbExactGrid & init );
  // \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbExactGrid( const MbExactGrid & init, MbRegDuplicate * iReg ); 
public:
  // \ru Конструктор без параметров. \en Constructor without parameters. 
  MbExactGrid();
  // \ru Деструктор. \en Destructor. 
  virtual ~MbExactGrid();

public:

  // \ru \name Общие функции примитива. \en \name Common functions of primitive.

  MbePrimitiveType IsA() const override; // \ru Тип объекта. \en A type of an object.
  SPtr<MbPrimitive> Clone( MbRegDuplicate * iReg = nullptr ) const override; // \ru Создать копию объекта \en Create a copy of the object
  void    Transform( const MbMatrix3D & ) override;                // \ru Преобразовать сетку согласно матрице. \en Transform mesh according to the matrix.
  void    Move     ( const MbVector3D & ) override;                // \ru Сдвиг сетки. \en Move mesh.
  void    Rotate   ( const MbAxis3D &, double angle ) override;    // \ru Поворот сетки вокруг оси. \en Rotation of mesh about an axis.
  void    AddYourGabaritTo( MbCube & ) const override;             // \ru Расширить присланный габаритный куб так, чтобы он включал в себя данный объект. \en Extend given bounding box so that it encloses the given object.  
  //double  DistanceToPoint( const MbCartPoint3D & ) const override; // \ru Вычислить расстояние до точки. \en Calculate distance to point.  
  //double  DistanceToLine( const MbAxis3D &, double extDistance, double & t ) const override;  // \ru Вычислить расстояние до оси. \en Calculate the distance to an axis.
  void    GetProperties( MbProperties & ) override;        // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & ) override;  // \ru Записать свойства объекта. \en Set properties of the object. 

  // \ru Выдать количество точек. \en Get the number of points. 
  size_t  PointsCount()  const override { return points.size();  }
  // \ru Выдать количество нормалей. \en Get the number of normals. 
  size_t  NormalsCount() const override { return normals.size(); }
  // \ru Выдать количество параметров. \en Get the number of parameters. 
  size_t  ParamsCount()  const override { return params.size();  }
  // \ru Выдать количество значений. \en Get count of values. 
  size_t  EscortsCount() const override { return escorts.size(); }
  // \ru Выдать количество точек минус 1 (максимальный индекс). \en Get the number of points minus one (maximal index). 
  ptrdiff_t PointsMaxIndex()  const override { ptrdiff_t n = points.size(); return ( n - 1 ); }
  // \ru Выдать количество нормалей минус 1 (максимальный индекс). \en Get the number of normals minus one (maximal index). 
  ptrdiff_t NormalsMaxIndex() const override { ptrdiff_t n = normals.size(); return ( n - 1 ); }
  // \ru Выдать количество параметров минус 1 (максимальный индекс). \en Get the number of parameters minus one (maximal index). 
  ptrdiff_t ParamsMaxIndex()  const override { ptrdiff_t n = params.size(); return ( n - 1 ); }

  // \ru Добавить в триангуляцию параметры, точку и нормаль триангулируемой поверхности в точке. \en Add parameters, point and normal of triangulated surface at point to triangulation. 
  void    AddPoint ( const MbCartPoint   & p2D, const MbCartPoint3D & p3D, const MbVector3D & n3D ) override;
  // \ru Добавить в триангуляцию параметры и точку. \en Add parameters and a point to triangulation. 
  void    AddPoint ( const MbCartPoint   & p2D, const MbCartPoint3D & p3D ) override;
  // \ru Добавить в триангуляцию точку и нормаль в точке. \en Add a point and normal at the point to triangulation. 
  void    AddPoint ( const MbCartPoint3D & p3D, const MbVector3D & n3D ) override;
  // \ru Добавить в триангуляцию точку. \en Add a point to triangulation. 
  void    AddPoint ( const MbCartPoint3D & p3D ) override;
  // \ru Добавить в триангуляцию нормаль. \en Add a normal to triangulation. 
  void    AddNormal( const MbVector3D    & n3D ) override;
  // \ru Добавить в триангуляцию параметры триангулируемой поверхности. \en Add parameters of triangulated surface to triangulation. 
  void    AddParam ( const MbCartPoint   & p2D ) override;

  // \ru Добавить в триангуляцию параметры, точку и нормаль триангулируемой поверхности в точке. \en Add parameters, point and normal of triangulated surface at point to triangulation. 
  void    AddPoint ( const MbFloatPoint  & p2D, const MbFloatPoint3D & p3D, const MbFloatVector3D & n3D ) override;
  // \ru Добавить в триангуляцию параметры и точку. \en Add parameters and a point to triangulation. 
  void    AddPoint ( const MbFloatPoint  & p2D, const MbFloatPoint3D & p3D ) override;
  // \ru Добавить в триангуляцию точку и нормаль в точке. \en Add a point and normal at the point to triangulation. 
  void    AddPoint ( const MbFloatPoint3D & p3D, const MbFloatVector3D & n3D ) override;
  // \ru Добавить в триангуляцию точку. \en Add a point to triangulation. 
  void    AddPoint( const MbFloatPoint3D & p3D ) override;
  // \ru Добавить в триангуляцию нормаль. \en Add a normal to triangulation. 
  void    AddNormal( const MbFloatVector3D & n3D ) override;
  // \ru Добавить в триангуляцию параметры триангулируемой поверхности. \en Add parameters of triangulated surface to triangulation. 
  void    AddParam( const MbFloatPoint   & p2D ) override;

  // \ru Добавить в коллекцию данных. \en Add scores to collection. 
  void    AddEscorts( const std::vector<uint32> & scores ) override { escorts.insert(escorts.end(), scores.begin(), scores.end()); }
  // \ru Добавить в коллекцию данных цвет точки (0 <= r,g,b,a <= 1). \en Add color to collection (0 <= r,g,b,a <= 1). 
  void    AddColor( float r, float g, float b, float a ) override;

  // \ru Выдать точку по её номеру. \en Get point by its index. 
  void    GetPoint ( size_t i, MbCartPoint3D  & p ) const override;
  // \ru Выдать нормаль по её номеру. \en Get normal by its index. 
  void    GetNormal( size_t i, MbVector3D     & n ) const override;
  // \ru Выдать параметр по его номеру. \en Get parameter by its index. 
  void    GetParam ( size_t i, MbCartPoint    & p ) const override;
  // \ru Выдать дополнительную информацию по её номеру. \en Get additional information by its index. 
  const   uint32 & GetEscort( size_t i ) const override;

  /// \ru Выдать точку по её номеру. \en Get point by its index. 
  void    GetPoint ( size_t i, MbFloatPoint3D  & p ) const override;
  /// \ru Выдать нормаль по её номеру. \en Get normal by its index. 
  void    GetNormal( size_t i, MbFloatVector3D & n ) const override;
  /// \ru Выдать параметр по его номеру. \en Get parameter by its index. 
  void    GetParam ( size_t i, MbFloatPoint    & p ) const override;

          // \ru Выдать точку с заданным номером. \en Get point by the given index. 
          const   MbCartPoint3D  & GetPoint ( size_t i ) const;
          // \ru Выдать нормаль с заданным номером. \en Get normal by the given index. 
          const   MbVector3D & GetNormal( size_t i ) const;
          // \ru Выдать параметр с заданным номером. \en Get parameter by the given index. 
          const   MbCartPoint    & GetParam ( size_t i ) const;

  // \ru Установить точку с заданным номером. \en Set point by the given index. 
  void    SetPoint ( size_t i, const MbCartPoint3D & p ) override;
  // \ru Установить нормаль с заданным номером. \en Set normal by the given index. 
  void    SetNormal( size_t i, const MbVector3D    & n ) override;
  // \ru Установить параметр с заданным номером. \en Set parameter by the given index. 
  void    SetParam ( size_t i, const MbCartPoint   & p ) override;
  // \ru Установить дополнительную информацию по её номеру. \en Set additional information by its index. 
  void    SetEscort( size_t i, const uint32 & e ) override;

  // \ru Удалить точку с заданным номером. \en Delete point by the given index. 
  void    PointRemove ( size_t i ) override;
  // \ru Удалить нормаль с заданным номером. \en Delete normal by the given index. 
  void    NormalRemove( size_t i ) override;
  // \ru Удалить параметры поверхности с заданным номером. \en Delete parameters of surface by the given index. 
  void    ParamRemove ( size_t i ) override;

  // \ru Удалить точки. \en Delete points. 
  void    PointsDelete() override;
  // \ru Удалить нормали. \en Delete normal. 
  void    NormalsDelete() override;
  // \ru Удалить параметры. \en Delete papams. 
  void    ParamsDelete() override;
  // \ru Удалить дополнительную информацию. \en Delete additional information. 
  void    EscortsDelete() override;

  /// \ru Инвертировать нормали. \en Invert normals. 
  void    NormalsInvert() override;

  // \ru Выдать для треугольника с номером i в общей нумерации (с полосами) параметры поверхности. \en Get parameters of surface for i-th triangle in general numbering (with strips). 
  bool    GetTriangleParams   ( size_t i, MbCartPoint & r0, MbCartPoint & r1, MbCartPoint & r2 ) const override;
  // \ru Выдать для треугольника с номером i в общей нумерации (с полосами) точки вершин. \en Get points of vertices for i-th triangle in general numbering (with strips). 
  bool    GetTrianglePoints   ( size_t i, MbCartPoint3D & p0, MbCartPoint3D & p1, MbCartPoint3D & p2 ) const override;
  // \ru Выдать для треугольника с номером i в общей нумерации (с полосами) точки вершин. \en Get points of vertices for i-th triangle in general numbering (with strips). 
  bool    GetTrianglePoints   ( size_t i, MbFloatPoint3D & p0, MbFloatPoint3D & p1, MbFloatPoint3D & p2 ) const override;
  // \ru Выдать для треугольника с номером i в общей нумерации (с полосами) нормали в вершинах. \en Get normals at vertices for i-th triangle in general numbering (with strips). 
  bool    GetTriangleNormals  ( size_t i, MbVector3D & n0, MbVector3D & n1, MbVector3D & n2 ) const override;
  // \ru Выдать для треугольника с номером i в общей нумерации (с полосами) нормали в вершинах. \en Get normals at vertices for i-th triangle in general numbering (with strips). 
  bool    GetTriangleNormals  ( size_t i, MbFloatVector3D & n0, MbFloatVector3D & n1, MbFloatVector3D & n2 ) const override;

  // \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) параметры поверхности. \en Get parameters of surface for i-th quadrangle in general numbering (with strips). 
  bool    GetQuadrangleParams ( size_t i, MbCartPoint & r0, MbCartPoint & r1, MbCartPoint & r2, MbCartPoint & r3 ) const override;
  // \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) точки вершин. \en Get points of vertices for i-th quadrangle in general numbering (with strips). 
  bool    GetQuadranglePoints ( size_t i, MbCartPoint3D & p0, MbCartPoint3D & p1, MbCartPoint3D & p2, MbCartPoint3D & p3 ) const override;
  // \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) точки вершин. \en Get points of vertices for i-th quadrangle in general numbering (with strips). 
  bool    GetQuadranglePoints ( size_t i, MbFloatPoint3D & p0, MbFloatPoint3D & p1, MbFloatPoint3D & p2, MbFloatPoint3D & n3 ) const override;
  // \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) нормали в вершинах. \en Get normals at vertices for i-th quadrangle in general numbering (with strips). 
  bool    GetQuadrangleNormals( size_t i, MbVector3D & n0, MbVector3D & n1, MbVector3D &n2, MbVector3D & n3 ) const override;
  // \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) нормали в вершинах. \en Get normals at vertices for i-th quadrangle in general numbering (with strips). 
  bool    GetQuadrangleNormals( size_t i, MbFloatVector3D & n0, MbFloatVector3D & n1, MbFloatVector3D & n2, MbFloatVector3D & n3 ) const override;

  // \ru Выдать первую нормаль для плоской триангуляции, если количество точек больше количества нормалей (только для плоской триангуляции). \en Get first normal for flat triangulation if count of points is greater than count of normals (only for planar triangulation). 
  bool    GetSingleNormal   ( MbVector3D &      ) const override;
  // \ru Выдать первую нормаль для плоской триангуляции, если количество точек больше количества нормалей (только для плоской триангуляции). \en Get first normal for flat triangulation if count of points is greater than count of normals (only for planar triangulation). 
  bool    GetSingleNormal   ( MbFloatVector3D & ) const override;
  // \ru Если количество точек больше количества нормалей, то добавить недостающие нормали (только для плоской триангуляции). \en If count of points is greater than count of normals, then add missing normals (only for planar triangulation). 
  void    SynchronizeNormals () override;
  // \ru Нормализовать нормали. \en Normalize normals. 
  void    NormalizeNormals   () override;

  // \ru Расширить присланный габаритный прямоугольник так, чтобы он включал в себя проекцию данного объекта на глобальную плоскость XY. \en Extend given bounding box so that it enclose projection of this object to the global XY-plane. 
  void    AddRect( MbRect & rect ) const override;
  // \ru Расширить присланный габаритный куб так, чтобы он включал в себя данный объект. \en Extend given bounding box so that it encloses the given object. 
  void    AddCube( MbCube & r ) const override;

          // \ru Определить, пересекается ли проекция на глобальную плоскость XY треугольника с заданным номером с присланным прямоугольником. \en Determine whether the projection of triangle with a given index to the global XY-plane intersects the given rectangle. 
  bool    TriangleIntersectRect( size_t i, MbRect & rect ) const override { return (i < triangles.size()) ? triangles[i].IntersectRect( rect, points ) : false; }
          // \ru Рассчитать габаритный прямоугольник проекции на глобальную плоскость XY треугольника с заданным номером. \en Determine bounding box of the projection of triangle with given index to the global XY-plane. 
  void    TriangleGetGabRect   ( size_t i, MbRect & rect ) const override { if (i < triangles.size()) triangles[i].GetGabRect( rect, points ); }

          // \ru Определить, пересекается ли проекция на глобальную плоскость XY четырёхугольника с заданным номером с присланным прямоугольником. \en Determine whether the projection of quadrangle with given index to the global XY-plane intersects the given rectangle. 
  bool    QuadrangleIntersectRect( size_t i, MbRect & rect ) const override { return (i < quadrangles.size()) ? quadrangles[i].IntersectRect( rect, points ) : false; }
          // \ru Рассчитать габаритный прямоугольник проекции на глобальную плоскость XY четырёхугольника с заданным номером. \en Determine bounding box of the projection of quadrangle with given index to the global XY-plane. 
  void    QuadrangleGetGabRect   ( size_t i, MbRect & rect ) const override { if (i < quadrangles.size()) quadrangles[i].GetGabRect( rect, points ); }

  // \ru Определить положение объекта относительно плоскости XY локальной системы координат. \en Determine the object position relative to the XY-plane of a local coordinate system. \~
  MbeItemLocation GetLocation( const MbPlacement3D & pl, double eps, bool onlyInItem = false ) const override;
  // \ru Определить, расположен ли объект внутри трубы прямоугольного сечения, заданного прямоугольником в плоскости XY локальной системы координат. \en Determine whether the object is inside the tube of rectangular section given by a rectangle in the XY plane of a local coordinate system. \~
  bool    InsideLocation( const MbPlacement3D & place, MbRect & rect, double eps ) const override;

  // \ru Преобразовать триангуляцию так, чтобы её параллельная проекция выглядела как центральная проекция, наблюдаемая из заданной точки vista. \en Transform triangulation so that its parallel projection looks as the central projection observed from the given 'vista' point. 
  void    SetVista   ( const MbCartPoint3D & vista ) override;
  // \ru Отменить преобразование триангуляцию для центральной проекции, наблюдаемой из заданной точки vista. \en Undo the transformation of triangulation for central projection observed from given 'vista' point. 
  void    DeleteVista( const MbCartPoint3D & vista ) override;

  // \ru Зарезервировать память для контейнера параметров. \en Reserve memory for container of elements. 
  void    ParamsReserve     ( size_t n ) override { params.reserve( params.size() + n ); }
  // \ru Зарезервировать память для контейнера точек. \en Reserve memory for container of points. 
  void    PointsReserve     ( size_t n ) override { points.reserve( points.size() + n ); }
  // \ru Зарезервировать память для контейнера нормалей. \en Reserve memory for container of normals. 
  void    NormalsReserve    ( size_t n ) override { normals.reserve( normals.size() + n ); }
  // \ru Зарезервировать память для контейнера параметров. \en Reserve memory for container of elements. 
  void    EscordsReserve    ( size_t n ) override { escorts.reserve( escorts.size() + n ); }

  // \ru Зарезервировать память для контейнеров. \en Reserve memory for some containers. 
  void    ReserveParamsPoints( size_t n ) override { params.reserve( params.size() + n ); points.reserve( points.size() + n ); }
  // \ru Зарезервировать память для контейнеров. \en Reserve memory for some containers. 
  void    ReservePointsNormals( size_t n ) override { points.reserve( points.size() + n ); normals.reserve( normals.size() + n ); }
  // \ru Зарезервировать память для контейнеров. \en Reserve memory for some containers. 
  void    ReserveParamsPointsNormals( size_t n ) override { params.reserve( params.size() + n ); points.reserve( points.size() + n ); normals.reserve( normals.size() + n ); }

  // \ru Удалить дублирующие с заданной точностью друг друга точки. \en Remove redundant points with a given tolerance (duplicates).
  bool    RemoveRedundantPoints( bool deleteNormals, double epsilon = LENGTH_EPSILON ) override;

  // \ru Удалить всю триангуляцию без освобождения памяти, занятую контейнерами. \en Delete all triangulation without freeing the memory occupied by containers. 
  void    Flush() override    { params.clear(); points.clear(); normals.clear(); escorts.clear();
                                triangles.clear(); quadrangles.clear(); LoopsDelete();
                                cube.SetEmpty(); DeleteSearchTree(); }
  // \ru Удалить всю триангуляцию и освободить память. \en Delete all triangulation and free the memory. 
  void    HardFlush() override { params.clear(); points.clear(); normals.clear(); escorts.clear();
                                triangles.clear(); quadrangles.clear(); LoopsDelete();
                                points.shrink_to_fit(); normals.shrink_to_fit(); params.shrink_to_fit(); escorts.shrink_to_fit();
                                triangles.shrink_to_fit(); quadrangles.shrink_to_fit(); loops.shrink_to_fit();
                                cube.SetEmpty(); DeleteSearchTree(); }
  // \ru Освободить лишнюю память. \en Free the unnecessary memory. 
  void    Adjust() override   {
                                points.shrink_to_fit(); normals.shrink_to_fit(); params.shrink_to_fit(); escorts.shrink_to_fit();
                                triangles.shrink_to_fit(); quadrangles.shrink_to_fit(); loops.shrink_to_fit(); 
                              }

  // \ru Выдать размер триангуляции в байтах. \en Get size of triangulation in bytes. 
  size_t  SizeOf() const override;
  // \ru Инвертировать последовательность вершин треугольников и четырехугольников. \en Reverse the sequence of vertices of triangles and quadrilaterals. 
  void    Reverse() override;
  // \ru Являются ли объекты равными? \en Determine whether objects are equal. 
  bool    IsSame( const MbGrid & init, double eps ) const override;

  // \ru Инициировать по другой триангуляции. \en Init by other triangulation. 
  void    Init( const MbGrid & grid ) override;

  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  const   MbCartPoint3D *   GetExactPointsAddr() const override { return points.data(); }
  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  const   MbVector3D *      GetExactNormalsAddr() const override { return normals.data(); }
  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  const   MbCartPoint *     GetExactParamsAddr() const override { return params.data(); }
  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  const   MbFloatPoint3D *  GetFloatPointsAddr() const override { return nullptr; }
  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  const   MbFloatVector3D * GetFloatNormalsAddr() const override { return nullptr; }
  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  const   MbFloatPoint *    GetFloatParamsAddr() const override { return nullptr; }

private :
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation, to prevent an assignment by default. 
          void    operator = ( const MbExactGrid & ); 

KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbExactGrid, MATH_FUNC_EX );
KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE( MbExactGrid, MATH_FUNC_EX );
}; // MbExactGrid


////////////////////////////////////////////////////////////////////////////////
/** \brief \ru Триангуляция на числах float.
           \en Triangulation on float data. \~
  \details \ru Триангуляция представляет собой набор треугольных и четырёхугольных пластин, стыкующихся друг с другом по общим сторонам.\n
      Триангуляция состоит из согласованных наборов точек, нормалей, параметров триангулируемой поверхности и наборов треугольников и четырехугольников.
    Каждый треугольник - это три номера из набора точек, определяющих вершины треугольника, каждый четырехугольник - это четыре номера из набора точек, определяющих вершины четырехугольника. \n
           \en Triangulation represents a set of triangular and quadrangular plates which are joined to each other by their common sides.\n
      The triangulation consists of a sets of points, normals, surface parameters and a sets of triangles and quadrangles.
    The triangle is represented as three indices from the set of points defining vertices of triangle, the quadrangle is represented as four indices from the set of points defining vertices of quadrangle.\n \~
  \ingroup Polygonal_Objects
*/
////////////////////////////////////////////////////////////////////////////////
class MATH_CLASS MbFloatGrid : public MbGrid {
private:
  std::vector<MbFloatPoint3D>  points;      ///< \ru Множество контрольных точек триангуляции (согласовано с множеством параметров, если последнее не пустое, или пусто, если не пусто множество параметров). \en Set of control points of triangulation (synchronized with set of parameters if the last is not empty, or empty if the set of parameters isn't empty). 
  std::vector<MbFloatVector3D> normals;     ///< \ru Множество нормалей в контрольных точках согласовано с множеством контрольных точек. \en Set of normals at control points is synchronized with the set of control points. 
  std::vector<MbFloatPoint>    params;      ///< \ru Множество параметров - двумерных точек на параметрической области триангулируемой поверхности (может быть пустым). \en Set of parameters of two-dimensional points in parametric domain of surface being triangulated(can be empty). 
  std::vector<uint32>          escorts;     ///< \ru Множество значений для дополнительной информации в точках. \en The set of values for additional information of points. 

protected:
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en The copy constructor without implementation prevents from copying by default. 
  MbFloatGrid( const MbFloatGrid & init );
  // \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbFloatGrid( const MbFloatGrid & init, MbRegDuplicate * iReg ); 
public:
  // \ru Конструктор без параметров. \en Constructor without parameters. 
  MbFloatGrid();
  // \ru Деструктор. \en Destructor. 
  virtual ~MbFloatGrid();

public:

  // \ru \name Общие функции примитива. \en \name Common functions of primitive.

  MbePrimitiveType IsA() const override; // \ru Тип объекта. \en A type of an object.
  SPtr<MbPrimitive> Clone( MbRegDuplicate * iReg = nullptr ) const override; // \ru Создать копию объекта \en Create a copy of the object
  void    Transform( const MbMatrix3D & ) override;                // \ru Преобразовать сетку согласно матрице. \en Transform mesh according to the matrix.
  void    Move     ( const MbVector3D & ) override;                // \ru Сдвиг сетки. \en Move mesh.
  void    Rotate   ( const MbAxis3D &, double angle ) override;    // \ru Поворот сетки вокруг оси. \en Rotation of mesh about an axis.
  void    AddYourGabaritTo( MbCube & ) const override;             // \ru Расширить присланный габаритный куб так, чтобы он включал в себя данный объект. \en Extend given bounding box so that it encloses the given object.  
  //double  DistanceToPoint( const MbCartPoint3D & ) const override; // \ru Вычислить расстояние до точки. \en Calculate distance to point.  
  //double  DistanceToLine( const MbAxis3D &, double extDistance, double & t ) const override;  // \ru Вычислить расстояние до оси. \en Calculate the distance to an axis.
  void    GetProperties( MbProperties & ) override;        // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & ) override;  // \ru Записать свойства объекта. \en Set properties of the object. 

  // \ru Выдать количество точек. \en Get the number of points. 
  size_t  PointsCount()  const override { return points.size();  }
  // \ru Выдать количество нормалей. \en Get the number of normals. 
  size_t  NormalsCount() const override { return normals.size(); }
  // \ru Выдать количество параметров. \en Get the number of parameters. 
  size_t  ParamsCount()  const override { return params.size();  }
  // \ru Выдать количество значений. \en Get count of values. 
  size_t  EscortsCount() const override { return escorts.size(); }
  // \ru Выдать количество точек минус 1 (максимальный индекс). \en Get the number of points minus one (maximal index). 
  ptrdiff_t PointsMaxIndex()  const override { ptrdiff_t n = points.size(); return ( n - 1 ); }
  // \ru Выдать количество нормалей минус 1 (максимальный индекс). \en Get the number of normals minus one (maximal index). 
  ptrdiff_t NormalsMaxIndex() const override { ptrdiff_t n = normals.size(); return ( n - 1 ); }
  // \ru Выдать количество параметров минус 1 (максимальный индекс). \en Get the number of parameters minus one (maximal index). 
  ptrdiff_t ParamsMaxIndex()  const override { ptrdiff_t n = params.size(); return ( n - 1 ); }

  // \ru Добавить в триангуляцию параметры, точку и нормаль триангулируемой поверхности в точке. \en Add parameters, point and normal of triangulated surface at point to triangulation. 
  void    AddPoint ( const MbCartPoint   & p2D, const MbCartPoint3D & p3D, const MbVector3D & n3D ) override;
  // \ru Добавить в триангуляцию параметры и точку. \en Add parameters and a point to triangulation. 
  void    AddPoint ( const MbCartPoint   & p2D, const MbCartPoint3D & p3D ) override;
  // \ru Добавить в триангуляцию точку и нормаль в точке. \en Add a point and normal at the point to triangulation. 
  void    AddPoint ( const MbCartPoint3D & p3D, const MbVector3D & n3D ) override;
  // \ru Добавить в триангуляцию точку. \en Add a point to triangulation. 
  void    AddPoint ( const MbCartPoint3D & p3D ) override;
  // \ru Добавить в триангуляцию нормаль. \en Add a normal to triangulation. 
  void    AddNormal( const MbVector3D    & n3D ) override;
  // \ru Добавить в триангуляцию параметры триангулируемой поверхности. \en Add parameters of triangulated surface to triangulation. 
  void    AddParam ( const MbCartPoint   & p2D ) override;

  // \ru Добавить в триангуляцию параметры, точку и нормаль триангулируемой поверхности в точке. \en Add parameters, point and normal of triangulated surface at point to triangulation. 
  void    AddPoint ( const MbFloatPoint  & p2D, const MbFloatPoint3D & p3D, const MbFloatVector3D & n3D ) override;
  // \ru Добавить в триангуляцию параметры и точку. \en Add parameters and a point to triangulation. 
  void    AddPoint ( const MbFloatPoint  & p2D, const MbFloatPoint3D & p3D ) override;
  // \ru Добавить в триангуляцию точку и нормаль в точке. \en Add a point and normal at the point to triangulation. 
  void    AddPoint ( const MbFloatPoint3D & p3D, const MbFloatVector3D & n3D ) override;
  // \ru Добавить в триангуляцию точку. \en Add a point to triangulation. 
  void    AddPoint( const MbFloatPoint3D & p3D ) override;
  // \ru Добавить в триангуляцию нормаль. \en Add a normal to triangulation. 
  void    AddNormal( const MbFloatVector3D & n3D ) override;
  // \ru Добавить в триангуляцию параметры триангулируемой поверхности. \en Add parameters of triangulated surface to triangulation. 
  void    AddParam( const MbFloatPoint   & p2D ) override;

  // \ru Добавить в коллекцию данных. \en Add scores to collection. 
  void    AddEscorts( const std::vector<uint32> & scores ) override { escorts.insert(escorts.end(), scores.begin(), scores.end()); }
  // \ru Добавить в коллекцию данных цвет точки (0 <= r,g,b,a <= 1). \en Add color to collection (0 <= r,g,b,a <= 1). 
  void    AddColor( float r, float g, float b, float a ) override;

  // \ru Выдать точку по её номеру. \en Get point by its index. 
  void    GetPoint ( size_t i, MbCartPoint3D  & p ) const override;
  // \ru Выдать нормаль по её номеру. \en Get normal by its index. 
  void    GetNormal( size_t i, MbVector3D     & n ) const override;
  // \ru Выдать параметр по его номеру. \en Get parameter by its index. 
  void    GetParam ( size_t i, MbCartPoint    & p ) const override;
  // \ru Выдать дополнительную информацию по её номеру. \en Get additional information by its index. 
  const   uint32 & GetEscort( size_t i ) const override;

  /// \ru Выдать точку по её номеру. \en Get point by its index. 
  void    GetPoint ( size_t i, MbFloatPoint3D  & p ) const override;
  /// \ru Выдать нормаль по её номеру. \en Get normal by its index. 
  void    GetNormal( size_t i, MbFloatVector3D & n ) const override;
  /// \ru Выдать параметр по его номеру. \en Get parameter by its index. 
  void    GetParam ( size_t i, MbFloatPoint    & p ) const override;

          // \ru Выдать точку с заданным номером. \en Get point by the given index. 
          const   MbFloatPoint3D  & GetPoint ( size_t i ) const;
          // \ru Выдать нормаль с заданным номером. \en Get normal by the given index. 
          const   MbFloatVector3D & GetNormal( size_t i ) const;
          // \ru Выдать параметр с заданным номером. \en Get parameter by the given index. 
          const   MbFloatPoint    & GetParam ( size_t i ) const;

  // \ru Установить точку с заданным номером. \en Set point by the given index. 
  void    SetPoint ( size_t i, const MbCartPoint3D & p ) override;
  // \ru Установить нормаль с заданным номером. \en Set normal by the given index. 
  void    SetNormal( size_t i, const MbVector3D    & n ) override;
  // \ru Установить параметр с заданным номером. \en Set parameter by the given index. 
  void    SetParam ( size_t i, const MbCartPoint   & p ) override;
  // \ru Установить дополнительную информацию по её номеру. \en Set additional information by its index. 
  void    SetEscort( size_t i, const uint32 & e ) override;

  // \ru Удалить точку с заданным номером. \en Delete point by the given index. 
  void    PointRemove ( size_t i ) override;
  // \ru Удалить нормаль с заданным номером. \en Delete normal by the given index. 
  void    NormalRemove( size_t i ) override;
  // \ru Удалить параметры поверхности с заданным номером. \en Delete parameters of surface by the given index. 
  void    ParamRemove ( size_t i ) override;

  // \ru Удалить точки. \en Delete points. 
  void    PointsDelete() override;
  // \ru Удалить нормали. \en Delete normal. 
  void    NormalsDelete() override;
  // \ru Удалить параметры. \en Delete papams. 
  void    ParamsDelete() override;
  // \ru Удалить дополнительную информацию. \en Delete additional information. 
  void    EscortsDelete() override;

  /// \ru Инвертировать нормали. \en Invert normals. 
  void    NormalsInvert() override;

  // \ru Выдать для треугольника с номером i в общей нумерации (с полосами) параметры поверхности. \en Get parameters of surface for i-th triangle in general numbering (with strips). 
  bool    GetTriangleParams   ( size_t i, MbCartPoint & r0, MbCartPoint & r1, MbCartPoint & r2 ) const override;
  // \ru Выдать для треугольника с номером i в общей нумерации (с полосами) точки вершин. \en Get points of vertices for i-th triangle in general numbering (with strips). 
  bool    GetTrianglePoints   ( size_t i, MbCartPoint3D & p0, MbCartPoint3D & p1, MbCartPoint3D & p2 ) const override;
  // \ru Выдать для треугольника с номером i в общей нумерации (с полосами) точки вершин. \en Get points of vertices for i-th triangle in general numbering (with strips). 
  bool    GetTrianglePoints   ( size_t i, MbFloatPoint3D & p0, MbFloatPoint3D & p1, MbFloatPoint3D & p2 ) const override;
  // \ru Выдать для треугольника с номером i в общей нумерации (с полосами) нормали в вершинах. \en Get normals at vertices for i-th triangle in general numbering (with strips). 
  bool    GetTriangleNormals  ( size_t i, MbVector3D & n0, MbVector3D & n1, MbVector3D & n2 ) const override;
  // \ru Выдать для треугольника с номером i в общей нумерации (с полосами) нормали в вершинах. \en Get normals at vertices for i-th triangle in general numbering (with strips). 
  bool    GetTriangleNormals  ( size_t i, MbFloatVector3D & n0, MbFloatVector3D & n1, MbFloatVector3D & n2 ) const override;

  // \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) параметры поверхности. \en Get parameters of surface for i-th quadrangle in general numbering (with strips). 
  bool    GetQuadrangleParams ( size_t i, MbCartPoint & r0, MbCartPoint & r1, MbCartPoint & r2, MbCartPoint & r3 ) const override;
  // \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) точки вершин. \en Get points of vertices for i-th quadrangle in general numbering (with strips). 
  bool    GetQuadranglePoints ( size_t i, MbCartPoint3D & p0, MbCartPoint3D & p1, MbCartPoint3D & p2, MbCartPoint3D & p3 ) const override;
  // \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) точки вершин. \en Get points of vertices for i-th quadrangle in general numbering (with strips). 
  bool    GetQuadranglePoints ( size_t i, MbFloatPoint3D & p0, MbFloatPoint3D & p1, MbFloatPoint3D & p2, MbFloatPoint3D & n3 ) const override;
  // \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) нормали в вершинах. \en Get normals at vertices for i-th quadrangle in general numbering (with strips). 
  bool    GetQuadrangleNormals( size_t i, MbVector3D & n0, MbVector3D & n1, MbVector3D &n2, MbVector3D & n3 ) const override;
  // \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) нормали в вершинах. \en Get normals at vertices for i-th quadrangle in general numbering (with strips). 
  bool    GetQuadrangleNormals( size_t i, MbFloatVector3D & n0, MbFloatVector3D & n1, MbFloatVector3D & n2, MbFloatVector3D & n3 ) const override;

  // \ru Выдать первую нормаль для плоской триангуляции, если количество точек больше количества нормалей (только для плоской триангуляции). \en Get first normal for flat triangulation if count of points is greater than count of normals (only for planar triangulation). 
  bool    GetSingleNormal   ( MbVector3D &      ) const override;
  // \ru Выдать первую нормаль для плоской триангуляции, если количество точек больше количества нормалей (только для плоской триангуляции). \en Get first normal for flat triangulation if count of points is greater than count of normals (only for planar triangulation). 
  bool    GetSingleNormal   ( MbFloatVector3D & ) const override;
  // \ru Если количество точек больше количества нормалей, то добавить недостающие нормали (только для плоской триангуляции). \en If count of points is greater than count of normals, then add missing normals (only for planar triangulation). 
  void    SynchronizeNormals() override;
  // \ru Нормализовать нормали. \en Normalize normals. 
  void    NormalizeNormals  () override;

  // \ru Расширить присланный габаритный прямоугольник так, чтобы он включал в себя проекцию данного объекта на глобальную плоскость XY. \en Extend given bounding box so that it enclose projection of this object to the global XY-plane. 
  void    AddRect( MbRect & rect ) const override;
  // \ru Расширить присланный габаритный куб так, чтобы он включал в себя данный объект. \en Extend given bounding box so that it encloses the given object. 
  void    AddCube( MbCube & r ) const override;

  // \ru Определить, пересекается ли проекция на глобальную плоскость XY треугольника с заданным номером с присланным прямоугольником. \en Determine whether the projection of triangle with a given index to the global XY-plane intersects the given rectangle. 
  bool    TriangleIntersectRect( size_t i, MbRect & rect ) const override { return (i<triangles.size()) ? triangles[i].IntersectRect( rect, points ) : false; }
  // \ru Рассчитать габаритный прямоугольник проекции на глобальную плоскость XY треугольника с заданным номером. \en Determine bounding box of the projection of triangle with given index to the global XY-plane. 
  void    TriangleGetGabRect   ( size_t i, MbRect & rect ) const override { if (i<triangles.size()) triangles[i].GetGabRect( rect, points ); }

  // \ru Определить, пересекается ли проекция на глобальную плоскость XY четырёхугольника с заданным номером с присланным прямоугольником. \en Determine whether the projection of quadrangle with given index to the global XY-plane intersects the given rectangle. 
  bool    QuadrangleIntersectRect( size_t i, MbRect & rect ) const override { return (i<quadrangles.size()) ? quadrangles[i].IntersectRect( rect, points ) : false; }
  // \ru Рассчитать габаритный прямоугольник проекции на глобальную плоскость XY четырёхугольника с заданным номером. \en Determine bounding box of the projection of quadrangle with given index to the global XY-plane. 
  void    QuadrangleGetGabRect   ( size_t i, MbRect & rect ) const override { if (i<quadrangles.size()) quadrangles[i].GetGabRect( rect, points ); }

  // \ru Определить положение объекта относительно плоскости XY локальной системы координат. \en Determine the object position relative to the XY-plane of a local coordinate system. \~
  MbeItemLocation GetLocation( const MbPlacement3D & pl, double eps, bool onlyInItem = false ) const override;
  // \ru Определить, расположен ли объект внутри трубы прямоугольного сечения, заданного прямоугольником в плоскости XY локальной системы координат. \en Determine whether the object is inside the tube of rectangular section given by a rectangle in the XY plane of a local coordinate system. \~
  bool    InsideLocation( const MbPlacement3D & place, MbRect & rect, double eps ) const override;

  // \ru Преобразовать триангуляцию так, чтобы её параллельная проекция выглядела как центральная проекция, наблюдаемая из заданной точки vista. \en Transform triangulation so that its parallel projection looks as the central projection observed from the given 'vista' point. 
  void    SetVista   ( const MbCartPoint3D & vista ) override;
  // \ru Отменить преобразование триангуляцию для центральной проекции, наблюдаемой из заданной точки vista. \en Undo the transformation of triangulation for central projection observed from given 'vista' point. 
  void    DeleteVista( const MbCartPoint3D & vista ) override;

  // \ru Зарезервировать память для контейнера параметров. \en Reserve memory for container of elements. 
  void    ParamsReserve     ( size_t n ) override { params.reserve( params.size() + n ); }
  // \ru Зарезервировать память для контейнера точек. \en Reserve memory for container of points. 
  void    PointsReserve     ( size_t n ) override { points.reserve( points.size() + n ); }
  // \ru Зарезервировать память для контейнера нормалей. \en Reserve memory for container of normals. 
  void    NormalsReserve    ( size_t n ) override { normals.reserve( normals.size() + n ); }
  // \ru Зарезервировать память для контейнера параметров. \en Reserve memory for container of elements. 
  void    EscordsReserve    ( size_t n ) override { escorts.reserve( escorts.size() + n ); }

  // \ru Зарезервировать память для контейнеров. \en Reserve memory for some containers. 
  void    ReserveParamsPoints( size_t n ) override { params.reserve( params.size() + n ); points.reserve( points.size() + n ); }
  // \ru Зарезервировать память для контейнеров. \en Reserve memory for some containers. 
  void    ReservePointsNormals( size_t n ) override { points.reserve( points.size() + n ); normals.reserve( normals.size() + n ); }
  // \ru Зарезервировать память для контейнеров. \en Reserve memory for some containers. 
  void    ReserveParamsPointsNormals( size_t n ) override { params.reserve( params.size() + n ); points.reserve( points.size() + n ); normals.reserve( normals.size() + n ); }

  // \ru Удалить дублирующие с заданной точностью друг друга точки. \en Remove redundant points with a given tolerance (duplicates).
  bool    RemoveRedundantPoints( bool deleteNormals, double epsilon = LENGTH_EPSILON ) override;

  // \ru Удалить всю триангуляцию без освобождения памяти, занятую контейнерами. \en Delete all triangulation without freeing the memory occupied by containers. 
  void    Flush() override    { params.clear(); points.clear(); normals.clear(); escorts.clear();
                                triangles.clear(); quadrangles.clear(); LoopsDelete();
                                cube.SetEmpty(); DeleteSearchTree(); }
  // \ru Удалить всю триангуляцию и освободить память. \en Delete all triangulation and free the memory. 
  void    HardFlush() override { params.clear(); points.clear(); normals.clear(); escorts.clear();
                                triangles.clear(); quadrangles.clear(); LoopsDelete();
                                points.shrink_to_fit(); normals.shrink_to_fit(); params.shrink_to_fit(); escorts.shrink_to_fit();
                                triangles.shrink_to_fit(); quadrangles.shrink_to_fit(); loops.shrink_to_fit();
                                cube.SetEmpty(); DeleteSearchTree(); }
  // \ru Освободить лишнюю память. \en Free the unnecessary memory. 
  void    Adjust() override   {
                                points.shrink_to_fit(); normals.shrink_to_fit(); params.shrink_to_fit(); escorts.shrink_to_fit();
                                triangles.shrink_to_fit(); quadrangles.shrink_to_fit(); loops.shrink_to_fit(); 
                              }
  
  // \ru Выдать размер триангуляции в байтах. \en Get size of triangulation in bytes. 
  size_t  SizeOf() const override;
  // \ru Инвертировать последовательность вершин треугольников и четырехугольников. \en Reverse the sequence of vertices of triangles and quadrilaterals. 
  void    Reverse() override;
  // \ru Являются ли объекты равными? \en Determine whether objects are equal. 
  bool    IsSame( const MbGrid & init, double eps ) const override;

  // \ru Инициировать по другой триангуляции. \en Init by other triangulation. 
  void    Init( const MbGrid & grid ) override;

  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  const   MbCartPoint3D *   GetExactPointsAddr() const override { return nullptr; }
  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  const   MbVector3D *      GetExactNormalsAddr() const override { return nullptr; }
  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  const   MbCartPoint *     GetExactParamsAddr() const override { return nullptr; }
  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  const   MbFloatPoint3D *  GetFloatPointsAddr() const override { return points.data(); }
  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  const   MbFloatVector3D * GetFloatNormalsAddr() const override { return normals.data(); }
  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  const   MbFloatPoint *    GetFloatParamsAddr() const override { return params.data(); }

private :
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation, to prevent an assignment by default. 
          void    operator = ( const MbFloatGrid & ); 

KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbFloatGrid, MATH_FUNC_EX );
KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE( MbFloatGrid, MATH_FUNC_EX );
}; // MbFloatGrid


////////////////////////////////////////////////////////////////////////////////
//
//         \ru Неклассные функции.   \en Other Functions.
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
/** \brief \ru Построить триангуляции в форме трубы заданного радиуса вокруг полигона.
           \en Construct triangulations in the form of a tube of given radius around the polygon. \~
  \details \ru Построить триангуляции в форме трубы заданного радиуса вокруг полигона. \n
           \en Construct triangulations in the form of a tube of given radius around the polygon. \n \~
  \param[in] poly - \ru Полигон.
                    \en A polygon. \~
  \param[in] radius - \ru Радиус трубы.
                      \en Radius of a tube. \~
  \param[in] sag - \ru Максимальное отклонение триангуляции от окружности (стрелка прогиба).
                   \en Maximum deviation of triangulation from a circle (sag). \~
  \param[out] grid - \ru Триангуляция - результат построения.
                     \en Triangulation - the result. \~
  \ingroup Triangulation
*/
// ---
MATH_FUNC (bool) PolygonTubeGrid( const MbPolygon3D & poly, 
                                        double        radius, 
                                        double        sag, 
                                        MbGrid &      grid );


//------------------------------------------------------------------------------
/** \brief \ru Преобразовать двумерный полигон в триангуляцию.
           \en Convert two-dimensional polygon to triangulation. \~
  \details \ru Преобразовать двумерный полигон в триангуляцию. \n
           \en Convert two-dimensional polygon to triangulation. \n \~
  \param[in] poly - \ru Двумерный полигон.
                    \en A two-dimensional polygon. \~
  \param[out] grid - \ru Построенная триангуляция.
                     \en Constructed triangulation. \~
  \ingroup Triangulation
*/
// ---
MATH_FUNC (bool) PolygonToGrid( const MbPolygon & poly, MbGrid & grid );


//------------------------------------------------------------------------------
/** \brief \ru Найти пересечение прямой линии и треугольника.
           \en Find the intersection of a line with a triangle. \~
  \details \ru Найти пересечение прямой линии и треугольника, заданного своими вершинами. 
    Если прямая линия пересекает треугольник, то определяется значение параметра прямой линии для точки пересечения. \n
           \en Find the intersection of a line with a triangle defined by its vertices. 
    If a straight line crosses the triangle, then the value of parameter of a straight line at the intersection point is determined. \n \~
  \param[in] tri_v0 - \ru Первая вершина треугольника.
                      \en The first vertex of the triangle. \~
  \param[in] tri_v1 - \ru Вторая вершина треугольника.
                      \en The second vertex of the triangle. \~
  \param[in] tri_v2 - \ru Третья вершина треугольника.
                      \en The third vertex of the triangle. \~
  \param[in] line - \ru Прямая линия, для которой вычисляется пересечение с треугольником.
                    \en A straight line to calculate the intersection of triangle with. \~
  \param[in] epsilon - \ru Пространственная погрешность.
                       \en A spatial tolerance. \~
  \param[out] crossPnt - \ru Точка пересечения.
                         \en The intersection point. \~
  \param[out] tRes - \ru Параметр точки пересечения на линии.
                     \en Parameter of the intersection point on the line. \~
  \return \ru Пересекает ли прямая треугольник (true - В случае пересечения линии и треугольника).
          \en Whether the line intersects the triangle (true in case of line and triangle intersection). \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC(bool) LineTriangleIntersect( const MbFloatPoint3D & tri_v0,
                                       const MbFloatPoint3D & tri_v1, 
                                       const MbFloatPoint3D & tri_v2,
                                       const MbFloatAxis3D &  line,
                                             float            epsilon,
                                             MbFloatPoint3D & crossPnt,
                                             float &          tRes );


//------------------------------------------------------------------------------
/** \brief \ru Найти пересечение прямой линии и триангуляции.
           \en Find the intersection of a straight line with the triangulation. \~
  \details \ru Для всех треугольников определяется пересечение с прямой линии и вычисляется минимальное значение 
    параметра точки пересечения на секущей прямой линии. \n
           \en For all the triangles the intersection with the straight line is determined and the minimum value of 
    the intersection point parameter on the secant straight line is calculated. \n \~
  \param[in] grid - \ru Триангуляция.
                    \en Triangulation. \~
  \param[in] line - \ru Прямая линия, для которой вычисляется пересечение с триангуляцией.
                    \en Straight line to calculate the intersection of triangulation with. \~
  \param[out] crossPnt - \ru Точка пересечения.
                         \en The intersection point. \~
  \param[out] tRes - \ru Параметр точки пересечения линии.
                     \en Parameter of the intersection point on the line. \~
  \return \ru Найдено ли пересечение (true - В случае успеха).
          \en Whether the intersection is found (true if success). \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) LineGridIntersect( const MbGrid &         grid, 
                                    const MbFloatAxis3D &  line,
                                          MbFloatPoint3D & crossPnt,
                                          float &          tRes );

//------------------------------------------------------------------------------
/** \brief \ru Найти пересечение прямой линии и триангуляции.
           \en Find the intersection of a straight line with the triangulation. \~
  \details \ru Для всех треугольников определяется пересечение с прямой линии и вычисляется минимальное значение 
    параметра точки пересечения на секущей прямой линии. \n
           \en For all the triangles the intersection with the straight line is determined and the minimum value of 
    the intersection point parameter on the secant straight line is calculated. \n \~
  \param[in] grid - \ru Триангуляция.
                    \en Triangulation. \~
  \param[in] line - \ru Прямая линия, для которой вычисляется пересечение с триангуляцией.
                    \en Straight line to calculate the intersection of triangulation with. \~
  \param[out] crossPnt - \ru Точка пересечения.
                         \en The intersection point. \~
  \param[out] tRes - \ru Параметр точки пересечения линии.
                     \en Parameter of the intersection point on the line. \~
  \param[out] tqInd - \ru Индекс найденного треугольника (tqInd.first) или четырехугольника (tqInd.second).
                      \en Index of the found triangle (tqInd.first) or quadrangle (tqInd.second). \~
  \return \ru Найдено ли пересечение (true - В случае успеха).
          \en Whether the intersection is found (true if success). \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) LineGridIntersect( const MbGrid &           grid, 
                                    const MbFloatAxis3D &    line,
                                          MbFloatPoint3D &   crossPnt,
                                          float &            tRes,
                                          c3d::IndicesPair & tqInd );


//------------------------------------------------------------------------------
/** \brief \ru Найти пересечение прямой линии и триангуляции.
           \en Find the intersection of a straight line with the triangulation. \~
  \details \ru Для всех треугольников определяется пересечение с прямой линии и вычисляется минимальное значение 
    параметра точки пересечения на секущей прямой линии. \n
           \en For all the triangles the intersection with the straight line is determined and the minimum value of 
    the intersection point parameter on the secant straight line is calculated. \n \~
  \param[in] grid - \ru Триангуляция.
                    \en Triangulation. \~
  \param[in] line - \ru Прямая линия, для которой вычисляется пересечение с триангуляцией.
                    \en Straight line to calculate the intersection of triangulation with. \~
  \param[in] cutPlace - \ru Отсекающая плоскость.
                        \en Cutting plane. \~
  \param[out] crossPnt - \ru Точка пересечения.
                         \en The intersection point. \~
  \param[out] tRes - \ru Параметр точки пересечения линии.
                     \en Parameter of the intersection point on the line. \~
  \return \ru Найдено ли пересечение (true - В случае успеха).
          \en Whether the intersection is found (true if success). \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) LineGridIntersect( const MbGrid &         grid, 
                                    const MbFloatAxis3D &  line,
                                    const MbPlacement3D &  cutPlace,
                                          MbFloatPoint3D & crossPnt,
                                          float &          tRes );


//------------------------------------------------------------------------------
/** \brief \ru Найти пересечение прямой линии и триангуляции.
           \en Find the intersection of a straight line with the triangulation. \~
  \details \ru Для всех треугольников определяется пересечение с прямой линии и вычисляется минимальное значение 
    параметра точки пересечения на секущей прямой линии. \n
           \en For all the triangles the intersection with the straight line is determined and the minimum value of 
    the intersection point parameter on the secant straight line is calculated. \n \~
  \param[in] grid - \ru Триангуляция.
                    \en Triangulation. \~
  \param[in] line - \ru Прямая линия, для которой вычисляется пересечение с триангуляцией.
                    \en Straight line to calculate the intersection of triangulation with. \~
  \param[in] cutPlaces - \ru Отсекающая плоскости.
                         \en Cutting planes. \~
  \param[out] crossPnt - \ru Точка пересечения.
                         \en The intersection point. \~
  \param[out] tRes - \ru Параметр точки пересечения линии.
                     \en Parameter of the intersection point on the line. \~
  \return \ru Найдено ли пересечение (true - В случае успеха).
          \en Whether the intersection is found (true if success). \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) LineGridIntersect( const MbGrid &                     grid, 
                                    const MbFloatAxis3D &              line,
                                    const std::vector<MbPlacement3D> & cutPlaces,
                                          MbFloatPoint3D &             crossPnt,
                                          float &                      tRes );


//------------------------------------------------------------------------------
/** \brief \ru Найти пересечение прямой линии и триангуляции.
           \en Find the intersection of a straight line with the triangulation. \~
  \details \ru Для всех треугольников определяется пересечение с прямой линии и вычисляется минимальное значение 
    параметра точки пересечения на секущей прямой линии. \n
           \en For all the triangles the intersection with the straight line is determined and the minimum value of 
    the intersection point parameter on the secant straight line is calculated. \n \~
  \param[in] grid - \ru Триангуляция.
                    \en Triangulation. \~
  \param[in] line - \ru Прямая линия, для которой вычисляется пересечение с триангуляцией.
                    \en Straight line to calculate the intersection of triangulation with. \~
  \param[out] tRes - \ru Параметр точки пересечения линии.
                     \en Parameter of the intersection point on the line. \~
  \return \ru Найдено ли пересечение (true - В случае успеха).
          \en Whether the intersection is found (true if success). \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) LineGridIntersect( const MbGrid &        grid, 
                                    const MbFloatAxis3D & line,
                                          float &         tRes );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить квадрат расстояния от линии до полигона.
           \en Calculate squared distance from a line to a polygon. \~
  \details \ru При вычислении квадрата расстояния от линии до полигона проверяется расстояние от каждого 
    сегмента полигона до первого попадания в окрестность delta.
    Возвращается значение параметра ближайшей точки на линии tRes и квадрат расстояния 
    от этой точки до сегмента полигона. \n
           \en During calculation of squared distance from a line to a polygon the distance from each 
    segment of the polygon is checked until the first getting to 'delta' neighborhood.
    Returns the value of the nearest point parameter on tRes line and the squared distance 
    from this point to a segment of the polygon. \n \~
  \param[in] grid - \ru Триангуляция.
                    \en Triangulation. \~
  \param[in] edgeInd - \ru Индекс тестируемого полигона.
                       \en Index of polygon to check. \~
  \param[in] line - \ru Линия, до которой вычисляется расстояние.
                    \en Line to calculate the distance to. \~
  \param[in] delta - \ru Радиус окрестности вокруг линии.
                     \en Neighborhood radius around the line. \~
  \param[out] tRes - \ru Значение параметра ближайшей точки линии.
                     \en The value of parameter of the nearest point on the line. \~
  \return \ru Квадрат расстояния ближайшей точки до линии.
          \en Squared distance between the nearest point and the line. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC(float) LineToGridEdgeDistanceSquared( const MbGrid &        grid,
                                                      size_t          edgeInd,
                                                const MbFloatAxis3D & line,
                                                      float           delta,
                                                      float &         tRes );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить квадрат расстояния от линии до полигона.
           \en Calculate squared distance from a line to a polygon. \~
  \details \ru При вычислении квадрата расстояния от линии до полигона проверяется расстояние от каждого 
    сегмента полигона до первого попадания в окрестность delta.
    Возвращается значение параметра ближайшей точки на линии tRes, вектор между ближайшими точками и квадрат 
    расстояния от этой точки до сегмента полигона. \n
           \en During calculation of squared distance from a line to a polygon the distance from each 
    segment of the polygon is checked until the first getting to 'delta' neighborhood.
    Returns the value of the nearest point parameter on the line, the vector between the nearest points 
    and the squared distance from this point to a segment of the polygon. \n \~
  \param[in] grid - \ru Триангуляция.
                    \en Triangulation. \~
  \param[in] edgeInd - \ru Индекс тестируемого полигона.
                       \en Index of polygon to check. \~
  \param[in] line - \ru Линия, до которой вычисляется расстояние.
                    \en Line to calculate the distance to. \~
  \param[in] vDelta - \ru Габарит окрестности вокруг линии.
                     \en The dimensions of the area around the line. \~
  \param[out] vRes - \ru Вектор от ближайшей точки на линии до ближайшей точки на полигоне.
                     \en Vector from the nearest point on the line to the nearest point on the polygon. \~
  \param[out] tRes - \ru Значение параметра ближайшей точки линии.
                     \en The value of parameter of the nearest point on the line. \~
  \return \ru Квадрат расстояния ближайшей точки до линии.
          \en Squared distance between the nearest point and the line. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC(float) LineToGridEdgeDistanceSquared( const MbGrid &          grid,
                                                      size_t            edgeInd,
                                                const MbFloatAxis3D &   line,
                                                const MbFloatVector3D & vDelta,
                                                      MbFloatVector3D & vRes,
                                                      float &           tRes );


//------------------------------------------------------------------------------
/** \brief \ru Создать плоскость на основе моментов инерции облака точек.
           \en Create a plane based on the moments of inertia of the point cloud. \~
  \details \ru Создать плоскость на основе моментов инерции облака точек. \n
           \en Create a plane based on the moments of inertia of the point cloud . \n \~
  \param[in] points - \ru Набор точек.
                    \en Points vector. \~
  \return \ru Плоскость, если получилось построить.
          \en Plane, if it turned out to be built. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC( MbPlane * ) CreateCloudPointsPlane( const c3d::SpacePointsVector & points );


#endif // __MESH_GRID_H
