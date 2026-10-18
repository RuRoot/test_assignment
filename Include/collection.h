////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Коллекция элементов.
         \en Collection of elements . \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __COLLECTION_H
#define __COLLECTION_H


#include <mb_cart_point3d.h>
#include <mb_cube.h>
#include <mesh_triangle.h>
#include <model_item.h>
#include <templ_dptr.h>
#include <vector>


class  MbMesh;
class  MbGrid;
class            MbGridTopology;


//------------------------------------------------------------------------------
/** \brief \ru Коллекция элементов.
           \en Collection of elements. \~
  \details \ru Коллекция элементов - это объект геометрической модели, наследник MbItem, являющийся
  множеством элементов в трехмерном пространстве. \n
           \en The collection of 3D elements is an object of geometric model (subclass MbItem) which is
  the set of elements in 3D space. \n \~
  \ingroup Model_Items
*/
// ---
class MATH_CLASS MbCollection : public MbItem {
public:
  /** \brief \ru Типы коллекций 3D объектов.
             \en Types of 3D object collection. \~
*/
enum CollectionType {
  coll_PointCloud   = 0,  ///< \ru Облако точек. \en The point cloud.
  coll_Tessellation = 1,  ///< \ru Триангуляция. \en The tessellation.
  coll_Elements     = 2,  ///< \ru Набор элементов. \en Set of elements.
  coll_Segmentation = 3,  ///< \ru Сегментированная полигональная сетка. \en Segmented polygonal mesh.
};

private:
  CollectionType                  type;        ///< \ru Тип коллекции 3D объектов. \en Type of 3D object collection. 
  uint32                          xSize;       ///< \ru Количество объектов вдоль первой  координаты. \en The number of objects along the first  coordinate. 
  uint32                          ySize;       ///< \ru Количество объектов вдоль второй  координаты. \en The number of objects along the second coordinate. 
  uint32                          zSize;       ///< \ru Количество объектов вдоль третьей координаты. \en The number of objects along the third  coordinate. 
  std::vector<MbCartPoint3D>      points;      ///< \ru Множество точек. \en Set of points. 
  std::vector<MbVector3D>         normals;     ///< \ru Множество нормалей в точках согласовано с множеством точек. \en Set of normals at control points is synchronized with the set of points. 
  std::vector<double>             escorts;     ///< \ru Множество значений для дополнительной информации в точках. \en The set of values for additional information of points. 
  std::vector<MbTriangle>         triangles;   ///< \ru Индексное множество треугольных пластин содержит номера элементов множества points и normals. \en Set of triangular plates contains numbers of elements of 'points' and 'normals' sets. 
  std::vector<MbQuadrangle>       quadrangles; ///< \ru Индексное множество четырёхугольных пластин содержит номера элементов множества params и/или множеств points и normals. \en Set of quadrangular plates contains numbers of elements of 'params' set and/or of 'points' and 'normals' sets. 
  std::vector<MbElement>          elements;    ///< \ru Индексное множество объемных элементов содержит номера элементов множества points. \en Set of volume elements contains numbers of vertices of 'points' sets. 
  std::vector<MbGridSegment>      segments;    ///< \ru Множество сегментов полигональной сетки. \en Set of segments of mesh. 

  /** \brief \ru Габаритный куб объекта. 
  \en Bounding box of object. \~
  \details \ru Габаритный куб объекта рассчитывается только при запросе габарита объекта. Габаритный куб в конструкторе объекта и после модификации объекта принимает неопределенное значение.
  \en Bounding box of object is calculated only at the request. Bounding box of object is undefined after object constructor and after object modifications \n \~
  */
  mutable MbCube               cube;
private:  
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en The copy constructor without implementation prevents from copying by default. 
  MbCollection( const MbCollection & init );

  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbCollection( const MbCollection &, MbRegDuplicate * );
public:
  /// \ru Конструктор. \en Constructor. 
  MbCollection();
  /// \ru Конструктор. \en Constructor. 
  MbCollection( const MbMesh & mesh );

  /// \ru Деструктор. \en Destructor. 
  virtual ~MbCollection();

public:
  VISITING_CLASS( MbCollection ); 

  // \ru Общие функции геометрического объекта \en Common functions of a geometric object  
  MbeSpaceType  IsA()  const override; // \ru Тип объекта. \en A type of an object. 
  MbeSpaceType  Type() const override; // \ru Групповой тип объекта. \en Group type of object. 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Создать копию. \en Create a copy.
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать согласно матрице. \en Transform according to the matrix.
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвинуть вдоль вектора. \en Translate along a vector.
  void    Rotate   ( const MbAxis3D   &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate about an axis.
  bool    IsSame   ( const MbSpaceItem & init, double accuracy = LENGTH_EPSILON ) const override; // \ru Являются ли объекты равными? \en Are the objects equal? 
  bool    SetEqual ( const MbSpaceItem & init ) override;   // \ru Сделать объекты равным. \en Make the objects equal. 
  double  DistanceToPoint ( const MbCartPoint3D & ) const override; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  void    AddYourGabaritTo( MbCube & r ) const override; // \ru Добавь свой габарит в куб. \en Add bounding box into a cube. 
  void    CalculateLocalGabarit( const MbMatrix3D & into, MbCube & cube ) const override; // \ru Рассчитать габарит в локальной системы координат. \en Calculate bounding box in the local coordinate system. 
  void    CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.

  MbProperty & CreateProperty( MbePrompt n ) const override; // \ru Создать собственное свойство. \en Create a custom property. 
  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of the object. 

  // \ru Создать полигональный объект - упрощенную копию данного объекта. \en Create a polygonal object - a simplified copy of the given object. 
  MbItem * CreateMesh( const MbStepData & stepData, const MbFormNote & note, MbRegDuplicate * iReg ) const override;
  // \ru Создать полигональный объект - упрощенную копию данного объекта. \en Create a polygonal object - a simplified copy of the given object. 
          MbGrid * CreateGrid() const;

          // \ru Создать сетки из четырехугольных пластин наружных стенок элементов. \en Create grids by quadrangular plates of the outside walls of elements. 
          void    CreateGridsByElements( RPArray<MbGrid> & grids_ ) const;

          // \ru Создать угловые точки и элементы. \en Create corner points and elements.
          void    CreateCornerPointsAndElements( SArray<MbFloatPoint3D> & points0, SArray<MbElement> & elements0 ) const;

          // \ru Создать сетки из по результатам сегментации. \en Create grids by segmentation results. 
          void    CreateGridsBySegments( RPArray<MbGrid> & grids_ ) const;

  /** \ru \name Общие функции коллекции.
      \en \name Common functions of a collection.
      \{ */

          /// \ru Выдать количество точек. \en Get count of points. 
          size_t  PointsCount() const { return points.size(); } 
          /// \ru Выдать количество нормалей. \en Get the number of normals. 
          size_t  NormalsCount() const { return normals.size(); }
          /// \ru Выдать количество значений. \en Get count of values. 
          size_t  EscortsCount() const { return escorts.size(); } 
          /// \ru Выдать количество треугольников. \en Get the number of triangles. 
          size_t  TrianglesCount() const { return triangles.size(); }
          /// \ru Выдать количество четырехугольников. \en Get the number of quadrangles. 
          size_t  QuadranglesCount() const { return quadrangles.size(); }
          /// \ru Выдать количество объемных элементов. \en Get the number of elements of volume. 
          size_t  ElementsCount() const { return elements.size(); }
          /// \ru Выдать количество сегментов. \en Get the number of segments of mesh. 
          size_t  SegmentsCount() const { return segments.size(); }
          /// \ru Выдать количество триангуляций. \en Get the number of triangulations. 
          //size_t  GridsCount() const { return grids.size(); } 
          ptrdiff_t PointsMaxIndex()  const { ptrdiff_t c = points.size(); return ( c - 1 ); }
          /// \ru Выдать количество нормалей минус 1 (максимальный индекс). \en Get the number of normals minus one (maximal index). 
          ptrdiff_t NormalsMaxIndex() const { ptrdiff_t c = normals.size(); return ( c - 1 ); }

          /// \ru Добавить в коллекцию точку и нормаль в точке. \en Add a point and normal at the point to collection. 
          void    AddPoint ( const MbCartPoint3D & p3D, const MbVector3D & n3D ) { points.push_back(p3D); normals.push_back(n3D); cube.SetEmpty(); }
          /// \ru Добавить в коллекцию точку. \en Add a point to collection. 
          void    AddPoint ( const MbCartPoint3D & p3D ) { points.push_back(p3D); cube.SetEmpty(); }
          /// \ru Добавить в коллекцию нормаль. \en Add a normal to collection. 
          void    AddNormal( const MbVector3D    & n3D ) { normals.push_back(n3D) ; }
          /// \ru Добавить в коллекцию точки. \en Add points to collection. 
          void    AddPoints ( const std::vector<MbCartPoint3D> & pnts )  { points.insert(points.end(), pnts.begin(), pnts.end()); cube.SetEmpty(); }
          /// \ru Добавить в коллекцию нормали. \en Add normals to collection. 
          void    AddNormals( const SArray<MbFloatVector3D> & nrms ) { normals.insert(normals.end(), nrms.begin(), nrms.end()); cube.SetEmpty(); }
          /// \ru Добавить в коллекцию данных. \en Add scores to collection. 
          void    AddEscorts( const std::vector<double> & scores )  { escorts.insert(escorts.end(), scores.begin(), scores.end()); }

          /// \ru Добавить треугольник. \en Add a triangle. 
          void    AddTriangle  ( const MbTriangle & triangle )   { triangles.push_back( triangle ); }
          /// \ru Добавить треугольник с заданными номерами вершин. \en Add a triangle by the given indices of vertices 
          void    AddTriangle  ( uint j0, uint j1, uint j2, bool o ) { MbTriangle t(j0,j1,j2,o); triangles.push_back( t ); }
          /// \ru Добавить четырёхугольник. \en Add a quadrangle. 
          void    AddQuadrangle( const MbQuadrangle & quadrangle ) { quadrangles.push_back( quadrangle ); }
          /// \ru Добавить четырёхугольник с заданными номерами вершин. \en Add a quadrangle by the given indices of vertices. 
          void    AddQuadrangle( uint j0, uint j1, uint j2, uint j3, bool o ) { MbQuadrangle t(j0,j1,j2,j3,o); quadrangles.push_back( t ); }
          /// \ru Добавить объемный элемент. \en Add an element. 
          void    AddElement( const MbElement & element ) { elements.push_back(element); }
          /// \ru Добавить объемный элемент. \en Add an element. 
          void    AddElement( uint j0, uint j1, uint j2, uint j3, uint j4, uint j5, uint j6, uint j7 ) { 
                              MbElement t( j0,j1,j2,j3,j4,j5,j6,j7 ); elements.push_back( t ); }
          void    AddSegment( const MbGridSegment & segment ) { segments.push_back( segment ); }
          void    AddSegment( const std::vector<size_t> & initFaces ) { MbGridSegment seg( initFaces ); segments.push_back( seg ); }
          /// \ru Добавить полигон. \en Add a polygon. 
          //void    AddGrid( MbExactGrid & grd ) { grids.push_back( &grd ); }

          /// \ru Выдать индексы точек в массиве points для i-го треугольника (связанного или несвязанного). \en Get indices of points in 'points' array for i-th triangle (adjacent or non-adjacent). 
          bool    GetTrianglePointIndex  ( size_t i, uint & ind0, uint & ind1, uint & ind2 ) const; 
          /// \ru Выдать индексы точек в массиве points для i-го четырехугольника (связанного или несвязанного). \en Get indices of points in 'points' array for i-th quadrangle (adjacent or non-adjacent). 
          bool    GetQuadranglePointIndex( size_t i, uint & ind0, uint & ind1, uint & ind2, uint & ind3 ) const; 
          /// \ru Выдать для треугольника с номером i в общей нумерации (с полосами) точки вершин. \en Get points of vertices for i-th triangle in general numbering (with strips). 
          bool    GetTrianglePoints   ( size_t i, MbCartPoint3D &p0, MbCartPoint3D &p1, MbCartPoint3D &p2 ) const;
          /// \ru Выдать для треугольника с номером i в общей нумерации (с полосами) нормали в вершинах. \en Get normals at vertices for i-th triangle in general numbering (with strips). 
          bool    GetTriangleNormals  ( size_t i, MbVector3D &n0, MbVector3D &n1, MbVector3D &n2 ) const;

          /// \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) точки вершин. \en Get points of vertices for i-th quadrangle in general numbering (with strips). 
          bool    GetQuadranglePoints ( size_t i, MbCartPoint3D &p0, MbCartPoint3D &p1, MbCartPoint3D &p2, MbCartPoint3D &p3 ) const;
          /// \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) нормали в вершинах. \en Get normals at vertices for i-th quadrangle in general numbering (with strips). 
          bool    GetQuadrangleNormals( size_t i, MbVector3D &n0, MbVector3D &n1, MbVector3D &n2, MbVector3D &n3 ) const;

          /// \ru Удалить точки. \en Delete points. 
          void    PointsRemove() {  points.clear(); 
                                    points.shrink_to_fit();
                                    cube.SetEmpty(); }
          /// \ru Удалить точку с заданным номером. \en Delete point by the given index. 
          void    PointRemove ( size_t i ) { if ( i < points.size() )  points.erase( points.begin() + i ); cube.SetEmpty(); }
          /// \ru Удалить нормаль с заданным номером. \en Delete normal by the given index. 
          void    NormalRemove( size_t i ) { if ( i < normals.size() ) normals.erase( normals.begin() + i ); }

          /// \ru Установить тип объекта. \en Set type.
          void    SetType( CollectionType t ) { type = t; }
          /// \ru Выдать тип объекта. \en Get type.
          CollectionType GetType() const { return type; }
          /// \ru Установить количество объектов вдоль первой  координаты. \en Set the number of objects along the first  coordinate.
          void    SetXSize( uint32 n ) { xSize = n; }
          /// \ru Установить количество объектов вдоль второй  координаты. \en Set the number of objects along the cecond coordinate.
          void    SetYSize( uint32 n ) { ySize = n; }
          /// \ru Установить количество объектов вдоль третьей координаты. \en Set the number of objects along the third  coordinate.
          void    SetZSize( uint32 n ) { zSize = n; }
          /// \ru Выдать количество объектов вдоль первой  координаты. \en Get the number of objects along the first  coordinate.
          uint32  GetXSize() const { return xSize; }
          /// \ru Выдать количество объектов вдоль второй  координаты. \en Get the number of objects along the cecond coordinate.
          uint32  GetYSize() const { return ySize; }
          /// \ru Выдать количество объектов вдоль третьей координаты. \en Get the number of objects along the third  coordinate.
          uint32  GetZSize() const { return zSize; }

          /// \ru Выдать точку по её номеру. \en Get point by its index. 
          void    GetPoint ( size_t i, MbCartPoint3D  & p ) const { p = points[i]; }
          /// \ru Выдать множество точек. \en Get set of points. 
          const std::vector<MbCartPoint3D> & GetPoints ( ) const { return points; }
          /// \ru Выдать нормаль по её номеру. \en Get normal by its index. 
          void    GetNormal( size_t i, MbVector3D     & n ) const { n = normals[i]; }
          /// \ru Выдать множество нормалей. \en Get set of normals. 
          const std::vector<MbVector3D> &  GetNormals( ) const { return normals; }
          /// \ru Выдать точку по её номеру. \en Get point by its index. 
          double  GetEscort( size_t i ) const { return escorts[i]; }
          /// \ru Выдать элемент по его номеру. \en Get element by its index. 
          void    GetElement( size_t i, MbElement & elem ) const { elem = elements[i]; }
          /// \ru Выдать индексы точек в массиве points для i-го объемного элемента. \en Get indices of points in 'points' array for i-th element. 
          bool    GetElementIndex( size_t i, uint & ind0, uint & ind1, uint & ind2, uint & ind3, uint & ind4, uint & ind5, uint & ind6, uint & ind7 ) const; 
          /// \ru Выдать для элемента с номером i точки вершин. \en Get points of vertices for i-th element. 
          bool    GetElementPoints ( size_t i, MbCartPoint3D &p0, MbCartPoint3D &p1, MbCartPoint3D &p2, MbCartPoint3D &p3, 
                                               MbCartPoint3D &p4, MbCartPoint3D &p5, MbCartPoint3D &p6, MbCartPoint3D &p7 ) const;
          /// \ru Выдать сегмент по его номеру. \en Get segment by its index. 
          void    GetSegment( size_t i, MbGridSegment & seg ) const { seg = segments[i]; }
          /// \ru Выдать точку с заданным номером. \en Get point by the given index. 
  const   MbCartPoint3D & GetPoint ( size_t i ) const { return points[i]; }
          /// \ru Выдать нормаль с заданным номером. \en Get normal by the given index. 
  const   MbVector3D    & GetNormal( size_t i ) const { return ( (normals.size() == 1) ? normals[0] : normals[i] ); }
          /// \ru Выдать треугольник с номером i. \en Get i-th triangle. 
  const   MbTriangle    & GetTriangle  ( size_t i ) const { return triangles[i];   }
          /// \ru Выдать четырёхугольник с номером i. \en Get i-th quadrangle. 
  const   MbQuadrangle  & GetQuadrangle( size_t i ) const { return quadrangles[i]; }
          /// \ru Выдать четырёхугольник с номером i. \en Get i-th quadrangle. 
  const   MbElement     & GetElement   ( size_t i ) const { return elements[i]; }
          /// \ru Выдать сегмент по его номеру. \en Get segment by its index. 
  const   MbGridSegment & GetSegment( size_t i ) const { return segments[i]; }
          /// \ru Выдать полигон с номером i. \en Get i-th polygon. 
  //const   MbExactGrid & GetGrid ( size_t i ) const { return *grids[i];   }

          /// \ru Удалить все треугольники. \en Delete all triangles. 
          void    TrianglesDelete() { triangles.clear();   }
          /// \ru Удалить все четырехугольники. \en Delete all quadrangles. 
          void    QuadranglesDelete() { quadrangles.clear();   }
          /// \ru Удалить все объемные элементы. \en Delete all elements. 
          void    ElementsDelete()  { elements.clear();    }
          /// \ru Удалить все сегменты. \en Delete all segments. 
          void    SegmentsDelete() { segments.clear(); }
          /// \ru Удалить все nhbfyuekzwbb. \en Delete all triangulations. 
          //void    GridsDelete();

          /// \ru Зарезервировать память для контейнеров. \en Reserve memory for some containers. 
          void    ReservePointsNormals( size_t n ) { points.reserve( points.size() + n ); normals.reserve( normals.size() + n ); }
          /// \ru Зарезервировать память для контейнера точек. \en Reserve memory for container of points. 
          void    PointsReserve     ( size_t n ) { points.reserve( points.size() + n ); }
          /// \ru Зарезервировать память для контейнера нормалей. \en Reserve memory for container of normals. 
          void    NormalsReserve    ( size_t n ) { normals.reserve( normals.size() + n ); }
          /// \ru Зарезервировать память для контейнера параметров. \en Reserve memory for container of elements. 
          /// \ru Зарезервировать память для контейнера параметров. \en Reserve memory for container of elements. 
          void    EscordsReserve    ( size_t n ) { escorts.reserve( escorts.size() + n ); }
          /// \ru Зарезервировать память для контейнера треугольников. \en Reserve memory for container of triangles. 
          void    TrianglesReserve  ( size_t n ) { triangles.reserve( triangles.size() + n ); }
          /// \ru Зарезервировать память для контейнера четырехугольников. \en Reserve memory for container of quadrangles. 
          void    QuadranglesReserve( size_t n ) { quadrangles.reserve( quadrangles.size() + n ); }
          /// \ru Зарезервировать память для контейнера элементов. \en Reserve memory for container of elements. 
          void    ElementsReserve   ( size_t n ) { elements.reserve( elements.size() + n ); }
          /// \ru Зарезервировать память для контейнера сегментов. \en Reserve memory for container of segments. 
          void    SegmentsReserve   ( size_t n ) { segments.reserve( segments.size() + n ); }
          /// \ru Зарезервировать память для контейнера полигонов. \en Reserve memory for container of grids. 
          //void    GridReserve       ( size_t n ) { grids.reserve( grids.size() + n ); }

          /// \ru Удалить всю триангуляцию без освобождения памяти, занятую контейнерами. \en Delete all triangulation without freeing the memory occupied by containers. 
          void    Flush()     { points.clear(); normals.clear(); escorts.clear();
                                triangles.clear(); quadrangles.clear(); elements.clear(); segments.clear(); //grids.clear();
                                cube.SetEmpty(); }
          /// \ru Удалить всю триангуляцию и освободить память. \en Delete all triangulation and free the memory. 
          void    HardFlush() { points.clear(); normals.clear(); escorts.clear();
                                triangles.clear(); quadrangles.clear(); elements.clear(); segments.clear(); //grids.clear();
                                points.shrink_to_fit(); normals.shrink_to_fit(); escorts.shrink_to_fit();
                                triangles.shrink_to_fit(); quadrangles.shrink_to_fit(); elements.shrink_to_fit(); segments.shrink_to_fit(); //grids.shrink_to_fit();
                                cube.SetEmpty(); }
          /// \ru Освободить лишнюю память. \en Free the unnecessary memory. 
          void    Adjust()    { 
                                points.shrink_to_fit(); normals.shrink_to_fit(); escorts.shrink_to_fit();
                                triangles.shrink_to_fit(); quadrangles.shrink_to_fit(); elements.shrink_to_fit(); segments.shrink_to_fit(); //grids.shrink_to_fit(); 
                               }

          /// \ru Инициализировать объект. \en Initialize object. 
          void    Init( const MbCollection & init );
          /// \ru Инициализировать объект. \en Initialize object. 
          void    Init( const MbGrid & init );
          /// \ru Инициализировать объект. \en Initialize object. 
          void    Init( const MbMesh & init );

          // \ru Выдать контейнер треугольников. \en Get the container of triangles. 
          template <class TrianglesVector>
          void    GetTriangles( TrianglesVector & tVector ) const {
            tVector.reserve( tVector.size() + triangles.size() );
            for ( size_t i = 0, iCount = triangles.size(); i < iCount; i++ )
              tVector.push_back( triangles[i] );
          }
          // \ru Выдать контейнер четырёхугольников. \en Get the container of quadrangles. 
          template <class QuadranglesVector>
          void    GetQuadrangles( QuadranglesVector & qVector ) const {
            qVector.reserve( qVector.size() + quadrangles.size() );
            for ( size_t i = 0, iCount = quadrangles.size(); i < iCount; i++ )
              qVector.push_back( quadrangles[i] );
          }

          /// \ru Преобразовать четырёхугольники в треугольники. \en Convert quadrangles to triangles. 
          void    ConvertQuadranglesToTriangles();
          /// \ru Преобразовать все объекты в треугольники и уравнять число точек и нормалей. \en Convert all objects to triangles and equalize count of points and count of normals. 
          void    ConvertAllToTriangles();
          /// \ru Удалить дублирующие с заданной точностью друг друга точки. \en Remove redundant points with a given tolerance (duplicates). 
          bool    RemoveRedundantPoints( bool deleteNormals, double epsilon = LENGTH_EPSILON );
  /** \} */
  private:
          /// \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
          MbCollection & operator = ( const MbCollection & );

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCollection )
};

IMPL_PERSISTENT_OPS( MbCollection )

#endif // __COLLECTION_H
