////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Триангуляция двумерной области.
         \en Triangulation of two-dimensional region. \~

*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// "PGrid.cpp"
// \ru PGrid.h - заголовочный файл триангуляции массива двумерных точек. \en PGrid.h - header file of two-dimensional point array triangulation. 
// \ru Содержит следующие разделы: \en Contains the following sections: 
// \ru - Триангуляция двумерной области \en - Triangulation of a two-dimensional region 
// \ru - Вершина полигона \en - Vertex of a polygon 
// \ru - Многоугольник \en - Polygon 
// \ru - Аппроксимация плоской области треугольными пластинами \en - Approximation of a planar region by triangular plates 
// \ru - Триангуляция двумерного региона \en - Triangulation of a two-dimensional region 
// \ru - Выпуклая триангуляция неупорядоченного массива двумерных точек \en - Convex triangulation of unordered array of two-dimensional points 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __MESH_PLANE_GRID_H
#define __MESH_PLANE_GRID_H

#include <reference_item.h>
#include <templ_s_array.h>
#include <templ_p_array.h>
#include <mesh_float_point.h>
#include <mb_rect.h>
#include <mb_cart_point.h>
#include <mesh_triangle.h>


class  MbRegion;
class  MbMatrix3D;
class  ProgressBarWrapper;


//------------------------------------------------------------------------------
/** \brief \ru Треугольник.
           \en Triangle. \~
  \details \ru Треугольник определен, как тройка точек, заданных индексами 
           вершин триангуляции MbPlanarGrid. \n
           \en Triangle is defined as triple of points defined by indices 
           of vertices of MbPlanarGrid triangulation. \n \~
  \ingroup Algorithms_2D
*/
// ---
class MbTri {
protected :
  size_t pIndex[3];  // \ru Номера вершин треугольника в массиве точек \en Indices of triangle vertices in array of points 

public :
  MbTri() { pIndex[0] = pIndex[1] = pIndex[2] = SYS_MAX_T; }
  MbTri( size_t j0, size_t j1, size_t j2, bool orientation ) { Init( j0, j1, j2, orientation ); }
  ~MbTri() {}
private:
  MbTri( const MbTri & ); // \ru Не реализовано \en Not implemented 
  void operator = ( const MbTri & ); // \ru Не реализовано \en Not implemented 
public :

  void    Init( size_t j0, size_t j1, size_t j2, bool orientation )
  {
    if ( orientation ) { // \ru Совпадает направление обхода \en Traverse direction coincides 
      pIndex[0] = j0;
      pIndex[1] = j1;
      pIndex[2] = j2;
    }
    else {
      pIndex[1] = j1;
      pIndex[2] = j0;
      pIndex[0] = j2;
    }
  }

  bool    GetTriangle ( size_t & i0, size_t & i1, size_t & i2 ) const
  {
    i0 = pIndex[0];
    i1 = pIndex[1];
    i2 = pIndex[2];
    return true;
  }

  size_t  GetIndex( size_t n ) const { return pIndex[n % 3]; }

  bool    IsTriangleEdge( size_t k0, size_t k1, size_t & eInd ) const
  {
    eInd = SYS_MAX_T;

    if ( k0 == pIndex[0] && k1 == pIndex[1] )
      eInd = 0;
    else if ( k0 == pIndex[1] && k1 == pIndex[2] )
      eInd = 1;
    else if ( k0 == pIndex[2] && k1 == pIndex[0] )
      eInd = 2;

    return (eInd != SYS_MAX_T);
  }
}; // MbTri


//------------------------------------------------------------------------------
/** \brief \ru Триангуляция двумерной области.
           \en Triangulation of a two-dimensional region. \~
  \details \ru Триангуляция двумерной области. \n
           \en Triangulation of a two-dimensional region. \n \~ 
  \ingroup Algorithms_2D
*/
// ---
class MATH_CLASS MbPlanarGrid : public MbRefItem {
private:
  SArray<MbCartPoint> points;    ///< \ru Вершины триангуляции. \en Vertices of triangulation. 
  SArray<MbTri>       triangles; ///< \ru Множество треугольников. \en Array of triangles. 

public:
  MbPlanarGrid() : points( 0, 1 ), triangles( 0, 1 ) {}
 ~MbPlanarGrid() {};

public:
  void    PointsReserve       ( size_t size ) { points.Reserve( size );        }
  void    TrianglesReserve    ( size_t size ) { triangles.Reserve( size );     }
  void    PointsSetMaxDelta   ( uint16 size ) { points.SetMaxDelta( size );    }
  void    TrianglesSetMaxDelta( uint16 size ) { triangles.SetMaxDelta( size ); }
  void    PointsAdjust()                      { points.Adjust();               }
  void    TrianglesAdjust()                   { triangles.Adjust();            }

  size_t  GetPointsCount   () const { return points.Count();    }
  size_t  GetTrianglesCount() const { return triangles.Count(); }

  void    AddTriangle( size_t j0, size_t j1, size_t j2, bool o ) { triangles.Add()->Init( j0, j1, j2,o ); }
  void    AddPoint   ( const MbFloatPoint & p ) { points.Add( p ); }
  void    AddPoint   ( const MbCartPoint  & p ) { points.Add( p ); }
  /// \ru Выдать декартову точку. \en Get Cartesian point. 
  bool    GetPoint   ( size_t k, MbCartPoint  & p ) const; 
  /// \ru Выдать точку. \en Get point. 
  bool    GetPoint   ( size_t k, MbFloatPoint & p ) const;
  /// \ru Получить индексы точек треугольника. \en Get indices of points of the triangle. 
  bool    GetTriangle( size_t k, size_t & i0, size_t & i1, size_t & i2 ) const; 
  /// \ru Получить точки треугольника. \en Get points of the triangle. 
  bool    GetTrianglePoints( size_t k, MbCartPoint & p0, MbCartPoint & p1, MbCartPoint & p2 ) const; 

OBVIOUS_PRIVATE_COPY( MbPlanarGrid )
};


//------------------------------------------------------------------------------
/// \ru Выдать декартову точку \en Get Cartesian point 
// ---
inline bool MbPlanarGrid::GetPoint( size_t k, MbCartPoint & p ) const
{
  if ( k < points.Count() ) {
    p = points[k];
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
/// \ru Выдать точку \en Get point 
// ---
inline bool MbPlanarGrid::GetPoint( size_t k, MbFloatPoint & p ) const
{
  if ( k < points.Count() ) {
    p = points[k];
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
/// \ru Вершина полигона \en Vertex of a polygon 
// ---
class TriVertex {
private:
  TriVertex * next;  ///< \ru Следующая вершина \en Next vertex 
  TriVertex * prev;  ///< \ru Предыдущая вершина \en Previous vertex 
  size_t      index; ///< \ru Номер точки в массиве точек триангуляции \en Index of point in array of points of triangulation 

public:
  TriVertex( size_t i ) : index(i) { next = this; prev = this; }

public:
  ~TriVertex() {}

public:
  /// \ru Смежная вершина (следующая или предыдущая). \en Adjacent vertex (next or previous). 
  TriVertex *         Neighbor  ( MbeMoveType ) const; 
  /// \ru Следующая вершина. \en Next vertex. 
  TriVertex *         Next      () const { return next;  } // SKIP_SA
  /// \ru Предыдущая вершина. \en Previous vertex. 
  TriVertex *         Prev      () const { return prev;  }  // SKIP_SA
  /// \ru Номер точки текущей вершины. \en Index of point of current vertex. 
  size_t              Index     () const { return index; } 
  /// \ru Вставить вершину после текущей. \en Insert vertex after the current one. 
  TriVertex *         Insert    ( TriVertex &  ); 
  /// \ru Исключить текущую вершину (возвращает исключенную, текущей становится предыдущая) \en Exclude the current vertex (returns excluded vertex, previous vertex becomes the current one) 
  TriVertex *         Detach    (); 
  /// \ru Соединить две цепочки вершин. \en Connect two vertex chains. 
  void                Splice    ( TriVertex &  );
  /// \ru Разделить цепочку вершин вершиной. \en Disconnect vertex chain by a vertex. 
  TriVertex *         Split     ( TriVertex &  );

OBVIOUS_PRIVATE_COPY( TriVertex )
};


//------------------------------------------------------------------------------
/// \ru Многоугольник \en Polygon 
// ---
class TriPoly {
private:
  TriVertex * vertex; ///< \ru Текущая вершина. \en Current vertex. 
  intptr_t    size;   ///< \ru Количество вершин. \en Count of vertices. 

public:
  TriPoly()                   : vertex( nullptr ), size( 0 ) {}
  TriPoly( TriVertex * vert ) : vertex( vert ), size( 0 ) {  Resize(); }

public:
  ~TriPoly();

public:
  intptr_t    Size() const { return size; } ///< \ru Размер цепочки вершин \en Size of vertex chain 
  size_t      Index() const { return (vertex != nullptr) ?  vertex->Index() : SYS_MAX_T; } ///< \ru Индекс вершины \en Index of vertex

  TriVertex * This() const { return vertex; } ///< \ru Текущая вершина \en Current vertex 
  TriVertex * Next() const; ///< \ru Следующая вершина \en Next vertex 
  TriVertex * Prev() const; ///< \ru Предыдущая вершина \en Previous vertex 
  TriVertex * Neighbor( MbeMoveType rotation ) const; ///< \ru Соседняя вершина \en Neighboring vertex 

  TriVertex * Advance ( MbeMoveType rotation ); ///< \ru Перемещение указателя текущей вершины на соседа \en Move current vertex pointer to the neighbor 
  TriVertex * SetVertex( TriVertex * );         ///< \ru Перемещение указателя текущей вершины на указанную \en Move current vertex pointer to the given one 

  TriVertex * Insert( TriVertex & ); ///< \ru Вставка новой вершины \en Insert new vertex 
  void        Remove(); ///< \ru Удаление текущей вершины \en Remove the current vertex 

  TriPoly  *  Split( TriVertex &, bool createNew ); ///< \ru Резка многоугольника вдоль хорды \en Cutting of polygon along a chord 
  bool        GetTriangle( size_t & k0, size_t & k1, size_t & k2 ) const; ///< \ru Получить индексы вершин треугольника (когда вершин больше 2) \en Get indices of triangle vertices (when there are more than 2 vertices) 

  template <class Point, class Gab>
  bool        CalculateGab( const SArray<Point> & pnts, Gab & rect ) const;

  TriPoly *   DeleteDegeneratedBridge( MbPlanarGrid & grid ); ///< \ru Удалить вырожденный мостик от текущей вершины. \en Remove the degenerate bridge from the current vertex.

private:
  void        Resize(); ///< \ru Обновление размера \en Set size to zero 

OBVIOUS_PRIVATE_COPY( TriPoly )
};


//------------------------------------------------------------------------------
// \ru Посчитать габарит цепочки вершин \en Calculate bounding box of vertex chain 
// ---
template <class Point, class Gab>
bool TriPoly::CalculateGab( const SArray<Point> & pnts, Gab & rect ) const
{
  bool isDone = false;

  rect.SetEmpty();
  size_t pntsCnt = pnts.Count();

  if ( Size() > 0 && pntsCnt > 0 ) {
    const TriVertex * v0 = This();
    size_t ind0 = v0->Index();

    if ( ind0 < pntsCnt ) {
      isDone = true;
      const Point & pnt0 = pnts[ind0];
      rect |= pnt0;

      TriVertex * v = v0->Next();
      while( v0 != v && isDone ) {
        isDone = false;
        size_t ind = v->Index();
        if ( ind < pntsCnt ) {
          const Point & pnt = pnts[ind];
          rect |= pnt;
          v = v->Next();
          isDone = true;
        }
      }
    }
  }

  C3D_ASSERT( isDone );
  return isDone;
}


/*
//------------------------------------------------------------------------------
// \ru Треугольник со ссылками на смежные(соседние) с ним треугольники \en Triangle with references to triangles adjacent (neighboring) to it 
// ---
class MbLinkedTri : public MbTri {
protected:
  MbTri * neighbors[3]; // \ru Соседние треугольники \en Neighboring triangles 
  // \ru Нумерация соседних треугольников: \en Numeration of neighboring triangles: 
  // \ru 0 - смежный через ребро на вершинах 0,1 \en 0 - adjacent at edge with vertices 0,1 
  // \ru 1 - смежный через ребро на вершинах 1,2 \en 1 - adjacent at edge with vertices 1,2 
  // \ru 2 - смежный через ребра на вершинах 2,0 \en 2 - adjacent at edge with vertices 2,0 

public:
  MbLinkedTri() : MbTri() { neighbors[0] = neighbors[1] = neighbors[2] = nullptr; };
  ~MbLinkedTri() {};
public:
  MbTri * GetNeighbor( size_t n ) const { return neighbors[n % 3]; }
  void    SetNeighbor( size_t n, MbTri * neighbor ) { neighbors[n % 3] = neighbor; }
  bool    IsBoundary() const; // \ru Является ли треугольник граничным \en Whether the triangle is boundary 

OBVIOUS_PRIVATE_COPY( MbLinkedTri )
};


//------------------------------------------------------------------------------
// \ru Является ли треугольник граничным \en Whether the triangle is boundary 
// ---
inline bool MbLinkedTri::IsBoundary() const
{
  bool isBoundary = (neighbors[0] == nullptr) ||
                    (neighbors[1] == nullptr) ||
                    (neighbors[2] == nullptr);
  return isBoundary;
}
// */


//------------------------------------------------------------------------------
// \ru Аппроксимация плоской области треугольными пластинами \en Approximation of planar region by triangular plates 
// \ru Функция удаляет полигоны точек из массива \en The function removes polygons of points from the array 
// ---
MATH_FUNC (void) CalculatePlanarGrid( PArray< SArray<MbCartPoint> > & poly, MbPlanarGrid & grid );


//------------------------------------------------------------------------------
/** \brief \ru Триангуляция двумерного региона
           \en Triangulation of a two-dimensional region \~
  \details \ru Триангуляция двумерного региона.
    Регион region должен быть корректным (на некорректном работает неправильно)
           \en Triangulation of a two-dimensional region.
    'region' region has to be correct (improper handling of incorrect ones) \~
  \ingroup Polygonal_Objects
*/
// ---
MATH_FUNC (void) TriangulateRegion( const MbRegion & region, double sag, MbPlanarGrid & grid );


//------------------------------------------------------------------------------
/// \ru Выпуклая триангуляция неупорядоченного массива двумерных точек \en Convex triangulation of unordered array of two-dimensional points 
// ---
MATH_FUNC (bool) TriangulateConvexCloud( const SArray<MbCartPoint> & uvPnts, double xEps, double yEps, const MbMatrix3D & from,
                                         SArray<MbTri> & triangles,
                                         ProgressBarWrapper * progBar );


#endif // __PGRID_H
