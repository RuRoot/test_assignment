////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Данные для размеров операции.
         \en Data for operation dimensions. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __POSITION_DATA_H
#define __POSITION_DATA_H


#include <mb_cart_point3d.h>
#include <name_item.h>
#include <templ_rp_array.h>
#include <templ_s_array.h>


class    MbCurveEdge;


//------------------------------------------------------------------------------
/** \brief \ru Данные для размеров операции.
           \en Data for operation dimensions. \~
  \details \ru Данные для позиционирования размеров операции скругления и фаски рёбер. \n
           \en Data for positioning dimensions of operations of fillet and chamfer of edges. \n \~
  \ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbPositionData {
public:
  MbCartPoint3D point1;   ///< \ru Опорная точка размера. \en Support point of dimension. 
  MbCartPoint3D point2;   ///< \ru Опорная точка размера. \en Support point of dimension. 
  MbCartPoint3D origin;   ///< \ru Начальна точка или центр. \en Start point or center. 
  MbVector3D    normal;   ///< \ru Нормаль плоскости размера. \en Plane normal of dimension. 
  double        param;    ///< \ru Положение размера в процентах длины ребра. \en Position of dimension in percentage of edge length. 
  MbName        itemName; ///< \ru Имя объекта. \en A name of an object. 

public:
  /// \ru Пустой конструктор. \en Empty constructor. 
  MbPositionData()
    : point1()
    , point2()
    , origin()
    , normal( 0.0, 0.0, 1.0 )
    , param (0.5)
    , itemName()
  {}
  /// \ru Конструктор по толщинам и замкнутости. \en Constructor by thicknesses and closedness. 
  MbPositionData( const MbCartPoint3D & p1, const MbCartPoint3D & p2, const MbCartPoint3D & _or, 
                  const MbVector3D & nor, double t, const MbName & n )
    : point1( p1 )
    , point2( p2 )
    , origin( _or )
    , normal( nor )
    , param ( t )
    , itemName( n )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbPositionData( const MbPositionData & other )
    : point1( other.point1 )
    , point2( other.point2 )
    , origin( other.origin )
    , normal( other.normal )
    , param ( other.param )
    , itemName( other.itemName )
  {}
  /// \ru Деструктор. \en Destructor. 
  ~MbPositionData() {}
  /// \ru Функция копирования данных. \en Copy function of data. 
  void                  Init( const MbPositionData & other ) { 
    point1 = other.point1;
    point2 = other.point2;
    origin = other.origin;
    normal = other.normal;
    param  = other.param; 
    itemName.SetName( other.itemName );
  }

  /// \ru Первая контрольная точка. \en The first control point. 
  const MbCartPoint3D & GetPoint1() const { return point1; }
  /// \ru Вторая контрольная точка. \en The second control point. 
  const MbCartPoint3D & GetPoint2() const { return point2; }
  /// \ru Начальная точка. \en The starting point. 
  const MbCartPoint3D & GetOrigin() const { return origin; }
  /// \ru Нормаль плоскости размера. \en Plane normal of dimension. 
  const MbVector3D &    Normal()    const { return normal; }
  /// \ru Положение размера в процентах длины ребра. \en Position of dimension in percentage of edge length. 
  const double &        GetParam()  const { return param;  }
  /// \ru Имя объекта. \en A name of an object. 
  const MbName &        GetName()   const { return itemName; }
  /// \ru Оператор присваивания. \en Assignment operator. 
  void                  operator = ( const MbPositionData & other ) {
    point1 = other.point1;
    point2 = other.point2;
    origin = other.origin;
    normal = other.normal;
    param  = other.param; 
    itemName.SetName( other.itemName );
  }

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbPositionData, MATH_FUNC_EX );
  KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE( MbPositionData, MATH_FUNC_EX );
  DECLARE_NEW_DELETE_CLASS( MbPositionData )
  DECLARE_NEW_DELETE_CLASS_EX( MbPositionData )
};


//------------------------------------------------------------------------------
/** \brief \ru Последовательность рёбер.
           \en Sequence of edges. \~
  \details \ru Последовательность гладко стыкующихся рёбер, скругляемых одновременно. \n
           \en Sequence of smooth mating edges rounded at the same time. \n \~
  \ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbEdgeSequence {
public:
  RPArray<const MbCurveEdge> edges;  ///< \ru Рёбра последовательности. \en Edges of sequence. 
  SArray<bool>               sense;  ///< \ru Направленность рёбер в последовательности. \en Direction of edges in the sequence. 
  bool                       closed; ///< \ru Замкнутость последовательности. \en Closedness of sequence. 

public:
  /// \ru Пустой конструктор. \en Empty constructor. 
  MbEdgeSequence()
    : edges( 0, 1 )
    , sense( 0, 1 )
    , closed( false )
  {}
  /// \ru Конструктор по толщинам и замкнутости. \en Constructor by thicknesses and closedness. 
  MbEdgeSequence( const MbCurveEdge & edge, bool s, bool c )
    : edges( 1, 1 )
    , sense( 1, 1 )
    , closed( c )
  {
    edges.Add( &edge );
    sense.Add( s );
  }
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbEdgeSequence( const MbEdgeSequence & other )
    : edges( other.edges.Count(), 1 )
    , sense( other.sense )
    , closed( other.closed )
  {
    edges.AddArray( other.edges );
  }
  /// \ru Деструктор. \en Destructor. 
  ~MbEdgeSequence() {}
  /// \ru Функция копирования данных. \en Copy function of data. 
  void                  Init( const MbEdgeSequence & other ) { 
    edges.DetachAll(); 
    sense.Flush();
    edges.AddArray( other.edges );
    sense = other.sense;
    closed = other.closed;
  }
  /// \ru Зарезервировать место под столько элементов. \en Reserve space for a given count of elements. 
  void                  Reserve( size_t count ) { 
    edges.Reserve( count );
    sense.Reserve( count );
  }
  /// \ru Добавить ребро. \en Add an edge. 
  void                  AddEdge( const MbCurveEdge & edge, bool s ) {
    edges.Add( &edge );
    sense.Add( s );
  }
  /// \ru Добавить в последовательность все гладко стыкующиеся с ней ребра. \en Add all smooth mating edges to a sequence. 
  bool                  CollectEdges( double epsilon );
  /// \ru Установить замкнутость. \en Set closedness. 
  void                  SetClosed( bool c ) { closed = c; }

  const MbCurveEdge *   Edge ( size_t i ) const { return ( i < edges.Count() ) ? edges[i] : nullptr; }
  const bool            Sense( size_t i ) const { return ( i < sense.Count() ) ? sense[i] : false; }
  size_t                Count() const { return edges.Count(); }
  /// \ru Оператор присваивания. \en Assignment operator. 
  void                  operator = ( const MbEdgeSequence & other ) {
    Init( other );
  }

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbEdgeSequence, MATH_FUNC_EX );
  KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE( MbEdgeSequence, MATH_FUNC_EX );
  DECLARE_NEW_DELETE_CLASS( MbEdgeSequence )
  DECLARE_NEW_DELETE_CLASS_EX( MbEdgeSequence )
};


#endif // __POSITION_DATA_H
