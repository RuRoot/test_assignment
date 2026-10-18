////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Структуры данных триангуляции.
         \en Triangulation data structures. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MESH_TRIANGLE_H
#define __MESH_TRIANGLE_H


#include <templ_s_array.h>
#include <mesh_float_point3d.h>
#include <mb_rect.h>
#include <vector>


class  MbTriangle;

namespace c3d // namespace C3D
{
  typedef MbTriangle                MeshTriangle;        ///< \ru Треугольник. \en Triangle.
  typedef std::vector<MbTriangle>   MeshTrianglesVector; ///< \ru Вектор треугольников. \en Mesh triangles vector.
}


//------------------------------------------------------------------------------
/// \ru Направление движения. \en Motion direction. 
/** 
  \ingroup Polygonal_Objects
*/
// ---
enum MbeMoveType {
  mt_Forward  = 0, ///< \ru Вперед. \en Forward. 
  mt_Backward = 1, ///< \ru Назад. \en Backward. 
};


////////////////////////////////////////////////////////////////////////////////
/** \brief \ru Треугольник.
           \en Triangle. \~
  \details \ru Треугольник определен, как тройка точек, заданных индексами 
           вершин триангуляции MbGrid. \n
           \en Triangle is defined as a triple of points defined by indices 
           of vertices of MbGrid triangulation. \n \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbTriangle {
protected :
  uint pIndex[3]; ///< \ru Номера вершин треугольника в массиве точек. \en Indices of triangle vertices in the array of points. 

public :
  /// \ru Конструктор. \en Constructor. 
  MbTriangle() { pIndex[0] = pIndex[1] = pIndex[2] = SYS_MAX_UINT; }
  /// \ru Конструктор. \en Constructor. 
  MbTriangle( uint j0, uint j1, uint j2, bool orientation );
  /// \ru Конструктор копирования. \en The copy-constructor. 
  MbTriangle( const MbTriangle & init ) {
    pIndex[0] = init.pIndex[0];
    pIndex[1] = init.pIndex[1];
    pIndex[2] = init.pIndex[2];
  }
  /// \ru Деструктор. \en Destructor. 
  ~MbTriangle();
  /// \ru Оператора присваивания. \en The assignment operator. 
  MbTriangle & operator = ( const MbTriangle & init ) {
    pIndex[0] = init.pIndex[0];
    pIndex[1] = init.pIndex[1];
    pIndex[2] = init.pIndex[2];
    return *this;
  }
public :

  /// \ru Инициализация. \en Initialization. 
  void    Init( uint j0, uint j1, uint j2, bool orientation );
  /// \ru Выдать номера вершин треугольника в массиве точек. \en Get indices of triangle vertices in the array of points. 
  bool    GetTriangle ( uint & i0, uint & i1, uint & i2 ) const;
  /// \ru Выдать номер вершины n треугольника в массиве точек. \en Get index of n-th triangle vertex in the array of points. 
  uint    GetIndex( size_t n ) const { return pIndex[n % 3]; }
  /// \ru Инвертировать последовательность вершин. \en Reverse the sequence of vertices. 
  void    Reverse();
  /// \ru Инвертировать последовательность вершин в заданном номером ребра порядке. \en Reverse the sequence of vertices with given by edge order.
  bool    FlipByEdge( size_t edgeIndex );

  /// \ru Определить, пересекается ли проекция на глобальную плоскость XY треугольника с присланным прямоугольником. \en Determine whether the projection of the triangle to the global XY-plane intersects the given rectangle. 
  template <class ParamPoints>
  bool    IntersectRect( const MbRect & rect, const ParamPoints & points ) const
  { 
    if ( points.size() < 3 )
      return false;

    double x1 = std_min( points[pIndex[0]].x, std_min( points[pIndex[1]].x, points[pIndex[2]].x ) );
    double x2 = std_max( points[pIndex[0]].x, std_max( points[pIndex[1]].x, points[pIndex[2]].x ) );

    double y1 = std_min( points[pIndex[0]].y, std_min( points[pIndex[1]].y, points[pIndex[2]].y ) );
    double y2 = std_max( points[pIndex[0]].y, std_max( points[pIndex[1]].y, points[pIndex[2]].y ) );

    return std_max( x1, rect.left   ) <= std_min( x2, rect.right ) &&
           std_max( y1, rect.bottom ) <= std_min( y2, rect.top   );
  }

  /// \ru Рассчитать габаритный прямоугольник проекции на глобальную плоскость XY треугольника. \en Calculate bounding rectangle of the projection of the triangle to the global XY-plane. 
  template <class ParamPoints>
  void    GetGabRect   (       MbRect & rect, const ParamPoints & points ) const
  { 
    if ( points.size() < 3 )
      return;

    rect.left   = std_min( points[pIndex[0]].x, std_min( points[pIndex[1]].x, points[pIndex[2]].x ) );
    rect.right  = std_max( points[pIndex[0]].x, std_max( points[pIndex[1]].x, points[pIndex[2]].x ) );

    rect.bottom = std_min( points[pIndex[0]].y, std_min( points[pIndex[1]].y, points[pIndex[2]].y ) );
    rect.top    = std_max( points[pIndex[0]].y, std_max( points[pIndex[1]].y, points[pIndex[2]].y ) );
  }

  /// \ru Принадлежит ли ребро треугольнику. \en Is triangle's edge? 
  bool    IsTriangleEdge( uint k0, uint k1, size_t & eInd ) const
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

  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  void    GetProperties( MbProperties & properties );
  /// \ru Записать свойства объекта. \en Set properties of the object. 
  void    SetProperties( const MbProperties & properties );

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbTriangle, MATH_FUNC_EX );
  KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE( MbTriangle, MATH_FUNC_EX );
}; // MbTriangle


//------------------------------------------------------------------------------
// \ru Инициализация \en Initialization 
// ---
inline void MbTriangle::Init( uint j0, uint j1, uint j2, bool orientation )
{
  if ( orientation ) {  // \ru Совпадает направление обхода \en Traverse direction coincides 
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


//------------------------------------------------------------------------------
// \ru Получить индексы треугольной пластины \en Get indices of triangle plate 
// ---
inline bool MbTriangle::GetTriangle ( uint & i0, uint & i1, uint & i2 ) const
{
  i0 = pIndex[0];
  i1 = pIndex[1];
  i2 = pIndex[2];
  return true;
}


//------------------------------------------------------------------------------
// \ru Инвертировать последовательность вершин. \en Reverse the sequence of vertices. 
// ---
inline void MbTriangle::Reverse()
{
  uint ind = pIndex[1];
  pIndex[1] = pIndex[2];
  pIndex[2] = ind;
}


//------------------------------------------------------------------------------
// \ru Инвертировать последовательность вершин в заданном номером ребра порядке. \en Reverse the sequence of vertices with given by edge order. 
// ---
inline bool MbTriangle::FlipByEdge( size_t edgeIndex ) {
  if ( edgeIndex < 3 ) {
    std::swap( pIndex[edgeIndex], pIndex[(edgeIndex + 1) % 3] );
    return true;
  }
  return false;
}


////////////////////////////////////////////////////////////////////////////////
/** \brief \ru Четырёхугольник.
           \en Quadrangle. \~
  \details \ru Четырёхугольник задан, как четверка индексов элементов из массива 
           вершин триангуляции MbGrid. \n
           \en Quadrangle defined as a quadruple of elements' indices form the array 
           of vertices of MbGrid triangulation. \n \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbQuadrangle {
protected :
  uint pIndex[4];  ///< \ru Номера вершин четырёхугольника в массиве точек. //-V112 \en Indices of quadrangle vertices in the array of points. //-V112 

public :
  /// \ru Конструктор. \en Constructor. 
  MbQuadrangle();
  /// \ru Конструктор. \en Constructor. 
  MbQuadrangle( uint j0, uint j1, uint j2, uint j3, bool orientation );
  /// \ru Конструктор копирования. \en The copy-constructor. 
  MbQuadrangle( const MbQuadrangle & init ) {
    pIndex[0] = init.pIndex[0];
    pIndex[1] = init.pIndex[1];
    pIndex[2] = init.pIndex[2];
    pIndex[3] = init.pIndex[3];
  }
  /// \ru Деструктор. \en Destructor. 
  ~MbQuadrangle();
  /// \ru Оператора присваивания. \en The assignment operator. 
  MbQuadrangle & operator = ( const MbQuadrangle & init ) {
    pIndex[0] = init.pIndex[0];
    pIndex[1] = init.pIndex[1];
    pIndex[2] = init.pIndex[2];
    pIndex[3] = init.pIndex[3];
    return *this;
  }
public :

  /// \ru Инициализация. \en Initialization. 
  void    Init( uint j0, uint j1, uint j2, uint j3, bool orientation );
  /// \ru Выдать номера вершин четырёхугольника в массиве точек. \en Get indices of quadrangle vertices in the array of points. 
  bool    GetQuadrangle ( uint & i0, uint & i1, uint & i2, uint & i3 ) const;
  /// \ru Выдать пару треугольников, составляющих четырехугольник. \en Get a pair of triangles consisting the quadrangle.
  void    GetTriangles( MbTriangle &, MbTriangle & ) const;
  /// \ru Выдать номер вершины n четырёхугольника в массиве точек. \en Get index of n-th quadrangle vertex in the array of points. 
  uint    GetIndex( size_t n ) const { return pIndex[n % 4]; } //-V112
  /// \ru Инвертировать последовательность вершин. \en Reverse the sequence of vertices. 
  void    Reverse();
  /// \ru Определить, пересекается ли проекция на глобальную плоскость XY четырёхугольника с присланным прямоугольником. \en Determine whether the projection of the quadrangle to the global XY-plane intersects the given rectangle. 
  template <class ParamPoints>
  bool    IntersectRect( const MbRect & rect, const ParamPoints & points ) const
  { 
    if ( points.size() < 4 )  //-V112
      return false;

    double x1 = std_min( points[pIndex[0]].x, std_min( points[pIndex[1]].x, std_min( points[pIndex[2]].x, points[pIndex[3]].x ) ) );
    double x2 = std_max( points[pIndex[0]].x, std_max( points[pIndex[1]].x, std_max( points[pIndex[2]].x, points[pIndex[3]].x ) ) );

    double y1 = std_min( points[pIndex[0]].y, std_min( points[pIndex[1]].y, std_min( points[pIndex[2]].y, points[pIndex[3]].y ) ) );
    double y2 = std_max( points[pIndex[0]].y, std_max( points[pIndex[1]].y, std_max( points[pIndex[2]].y, points[pIndex[3]].y ) ) );

    return std_max( x1, rect.left   ) <= std_min( x2, rect.right ) &&
           std_max( y1, rect.bottom ) <= std_min( y2, rect.top   );
  }

  /// \ru Рассчитать габаритный прямоугольник проекции на глобальную плоскость XY четырёхугольника. \en Calculate bounding rectangle of the projection of quadrangle to the global XY-plane. 
  template <class ParamPoints>
  void    GetGabRect   (       MbRect & rect, const ParamPoints & points ) const
  { 
    if ( points.size() < 4 ) //-V112
      return;

    rect.left   = std_min( points[pIndex[0]].x, std_min( points[pIndex[1]].x, std_min( points[pIndex[2]].x, points[pIndex[3]].x ) ) );
    rect.right  = std_max( points[pIndex[0]].x, std_max( points[pIndex[1]].x, std_max( points[pIndex[2]].x, points[pIndex[3]].x ) ) );

    rect.bottom = std_min( points[pIndex[0]].y, std_min( points[pIndex[1]].y, std_min( points[pIndex[2]].y, points[pIndex[3]].y ) ) );
    rect.top    = std_max( points[pIndex[0]].y, std_max( points[pIndex[1]].y, std_max( points[pIndex[2]].y, points[pIndex[3]].y ) ) );
  }
  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  void    GetProperties( MbProperties &properties );
  /// \ru Записать свойства объекта. \en Set properties of the object. 
  void    SetProperties( const MbProperties &properties );

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbQuadrangle, MATH_FUNC_EX );
  KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE( MbQuadrangle, MATH_FUNC_EX );
}; // MbQuadrangle


//------------------------------------------------------------------------------
// \ru Инициализация \en Initialization 
// ---
inline void MbQuadrangle::Init( uint j0, uint j1, uint j2, uint j3, bool orientation )
{
  if ( orientation ) {  // \ru Совпадает направление обхода \en Traverse direction coincides 
    pIndex[0] = j0;
    pIndex[1] = j1;
    pIndex[2] = j2;
    pIndex[3] = j3;
  }
  else {
    pIndex[2] = j1;
    pIndex[3] = j0;
    pIndex[0] = j3;
    pIndex[1] = j2;
  }
}


//------------------------------------------------------------------------------
/// \ru Получить индексы четырехугольной пластины \en Get indices of quadrangle plate 
// ---
inline bool MbQuadrangle::GetQuadrangle ( uint & i0, uint & i1, uint & i2, uint & i3 ) const
{
  i0 = pIndex[0];
  i1 = pIndex[1];
  i2 = pIndex[2];
  i3 = pIndex[3];
  return true;
}

//------------------------------------------------------------------------------
// \ru Выдать пару треугольников, составляющих четырехугольник. \en Get a pair of triangles consisting the quadrangle.
//---
inline void MbQuadrangle::GetTriangles( MbTriangle & fTri, MbTriangle & sTri ) const
{
  fTri.Init( pIndex[0], pIndex[1], pIndex[2], true );
  sTri.Init( pIndex[2], pIndex[3], pIndex[0], true );
}

//------------------------------------------------------------------------------
// \ru Инвертировать последовательность вершин. \en Reverse the sequence of vertices. 
// ---
inline void MbQuadrangle::Reverse()
{
  uint ind = pIndex[1];
  pIndex[1] = pIndex[3];
  pIndex[3] = ind;
}


////////////////////////////////////////////////////////////////////////////////
/** \brief \ru Объемный элемент.
           \en Element of tesselation of solid volume. \~
  \details \ru Элемент задан, как восемь индексов точек из массива вершин объекта MbGrid. \n
           \en Element defined as an elements' indices form the array of vertices of MbGrid. \n \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbElement {
protected :
  uint pIndex[8]; ///< \ru Номера вершин елемента в массиве точек. \en Indices of element vertices in the array of points.
  size_t estate;  ///< \ru Свойство элемента. \en Estate of element.
  double props;   ///< \ru Характеристика элемента. \en Property of element.

public :
  /// \ru Конструктор. \en Constructor. 
  MbElement();
  /// \ru Конструктор. \en Constructor. 
  MbElement( uint j0, uint j1, uint j2, uint j3, uint j4, uint j5, uint j6, uint j7 );
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbElement( const MbElement & );
  /// \ru Деструктор. \en Destructor. 
  ~MbElement();
  // \ru Оператор присваивания. \en Assignment operator. 
  MbElement & operator = ( const MbElement & ); 

public :

  /// \ru Инициализация. \en Initialization. 
  void    Init( uint j0, uint j1, uint j2, uint j3, uint j4, uint j5, uint j6, uint j7 );
  /// \ru Выдать номера вершин четырёхугольника в массиве точек. \en Get indices of quadrangle vertices in the array of points. 
  bool    GetElement ( uint & i0, uint & i1, uint & i2, uint & i3, uint & i4, uint & i5, uint & i6, uint & i7 ) const;
  /// \ru Выдать номер вершины n четырёхугольника в массиве точек. \en Get index of n-th quadrangle vertex in the array of points. 
  uint    GetIndex( size_t n ) const { return pIndex[n % 8]; }
  /// \ru Дать свойство элемента. \en Get estate of element.
  size_t  GetEstate() const { return estate; } 
  /// \ru Изменить свойство элемента. \en Set estate of element.
  void    SetEstate( uint32 e ) { estate = e; } 
  /// \ru Дать характеристику элемента. \en Get property of element.
  double  GetProps()  const { return props; }  
  /// \ru Изменить характеристику элемента. \en Get property of element.
  void    SetProps( double p ) { props = p; }  

  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  void    GetProperties( MbProperties &properties );
  /// \ru Записать свойства объекта. \en Set properties of the object. 
  void    SetProperties( const MbProperties &properties );

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbElement, MATH_FUNC_EX );
  KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE( MbElement, MATH_FUNC_EX );
}; // MbElement


//------------------------------------------------------------------------------
// \ru Инициализация \en Initialization 
// ---
inline void MbElement::Init( uint j0, uint j1, uint j2, uint j3, uint j4, uint j5, uint j6, uint j7 )
{
  pIndex[0] = j0;
  pIndex[1] = j1;
  pIndex[2] = j2;
  pIndex[3] = j3;
  pIndex[4] = j4;
  pIndex[5] = j5;
  pIndex[6] = j6;
  pIndex[7] = j7;
}


//------------------------------------------------------------------------------
/// \ru Получить индексы четырехугольной пластины \en Get indices of quadrangle plate 
// ---
inline bool MbElement::GetElement ( uint & i0, uint & i1, uint & i2, uint & i3, uint & i4, uint & i5, uint & i6, uint & i7 ) const
{
  i0 = pIndex[0];
  i1 = pIndex[1];
  i2 = pIndex[2];
  i3 = pIndex[3];
  i4 = pIndex[4];
  i5 = pIndex[5];
  i6 = pIndex[6];
  i7 = pIndex[7];
  return true;
}


////////////////////////////////////////////////////////////////////////////////
/** \brief \ru Граница триангуляции.
           \en Border of triangulation. \~
  \details \ru Граница триангуляции используется для описания набора ребер грани оболочки. \n
               Граница триангуляции содержит номера последовательности вершин.
           \en Border of triangulation is used to describe edge sequence of shell's face. \n
               Border of triangulation contains indices of vertices sequence. \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbGridLoop {
private:
  std::vector<uint> pIndices; ///< \ru Номера вершин в массиве точек. \en Indices of vertices in the array of points. 

public:
  /// \ru Конструктор. \en Constructor. 
  MbGridLoop( size_t n = 0 ) : pIndices() { if ( n > 0 ) pIndices.reserve( n ); }
  /// \ru Конструктор. \en Constructor. 
  template <class UintVector>
  explicit MbGridLoop( const UintVector & init ) : pIndices() { 
    pIndices.reserve( init.size() );
    for ( size_t i = 0, iCount = init.size(); i < iCount; i++ )
      pIndices.push_back( init[i] ); 
  }
  /// \ru Деструктор. \en Destructor. 
  ~MbGridLoop() {}
  
public:
  /// \ru Инициализация. \en Initialization. 
  template <class UintVector>
  void    Init( const UintVector & init ) { 
    pIndices.clear(); pIndices.reserve( init.size() ); 
    for ( size_t i = 0, iCount = init.size(); i < iCount; i++ )
      pIndices.push_back( init[i] ); 
  }
  /// \ru Выдать количество вершин полосы. \en Get the count of strip vertices. 
  size_t  Count() const { return pIndices.size(); }
  /// \ru Добавить номер вершины. \en Add vertex number. 
  void    Add( uint n ) { pIndices.push_back(n); }
  /// \ru Выдать количество вершин полосы. \en Get the count of strip vertices. 
  uint    GetIndex( size_t i ) const { return pIndices[i]; }
  /// \ru Выдать количество вершин полосы. \en Get the count of strip vertices. 
  uint &  SetIndex( size_t i ) { return pIndices[i]; }
  /// \ru Очистить полосу. \en Clear the strip. 
  void    Flush() { pIndices.clear(); }
  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  const   uint * GetIndicesAddr() const { return &(pIndices[0]); }
  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  const   std::vector<uint> & GetIndices() const { return pIndices; }
  /// \ru Выдать контейнер номеров вершин. \en Get the container of vertex numbers. 
  template <class IndicesVector>
  void    GetIndices( IndicesVector & iVector ) const { 
    iVector.reserve( iVector.size() + pIndices.size() );
    for ( size_t i = 0, iCount = pIndices.size(); i < iCount; i++ )
      iVector.push_back( pIndices[i] );
  }
  /// \ru Есть ли такой индекс в цикле? \en Is exist index n in the loop?
  bool    IsExist( uint n ) const { return ( std::find(pIndices.begin(), pIndices.end(), n) != pIndices.end() ); }

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbGridLoop, MATH_FUNC_EX );
  KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE( MbGridLoop, MATH_FUNC_EX );
  OBVIOUS_PRIVATE_COPY( MbGridLoop )
};


////////////////////////////////////////////////////////////////////////////////
/** \brief \ru Сегмент(результат сегментации) полигональной сетки.
           \en A polygonal mesh segment (segmentation result). \~
  \details \ru Сегмент определен множеством треугольников полигональной сетки. \n
           \en Segment is defined as a set of triangles of polygonal mesh. \n \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbGridSegment {
private:
  std::vector<size_t> faces;  ///< \ru Вектор индексов треугольников сегмента. \en A vector of segment triangles indicies. 

public:
  /// \ru Конструктор. \en Constructor. 
  MbGridSegment() : faces() {}
  /// \ru Конструктор. \en Constructor. 
  MbGridSegment( const std::vector<size_t> & initFaces ) : faces( initFaces ) {}
  /// \ru Выдать вектор индексов треугольников сегмента. \en Get the vector of segment triangles indicies. 
  const std::vector<size_t> & GetFaces() const { return faces; }
  /// \ru Выдать количество треугольников сегмента. \en Get the count of of segment triangles. 
  size_t GetFaceCount() const { return faces.size(); }
  /// \ru Выдать индекс треугольника сегмента. \en Get the index of segment triangle. 
  size_t GetFace( size_t idx ) const { return faces[idx]; }

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbGridSegment, MATH_FUNC_EX );
  KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE( MbGridSegment, MATH_FUNC_EX );
};

#endif // __MESH_TRIANGLE_H
