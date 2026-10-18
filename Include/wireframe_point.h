////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Точка на каркасе.
         \en Wire frame point. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __WIREFRAME_POINT_H
#define __WIREFRAME_POINT_H

#include <mb_cart_point3d.h>
#include <io_tape.h>


class MbWireFrame;
class MbWireFramePoint;
class MbEdgePoint;
class MbEdge;


namespace c3d // namespace C3D
{
typedef std::pair<MbWireFramePoint, MbWireFramePoint> WireFramePointsPair;
typedef std::vector<WireFramePointsPair> WireFramePointsPairsVector;
typedef std::pair<MbEdgePoint, MbEdgePoint> EdgePointsPair;
typedef std::vector<EdgePointsPair> EdgePointsPairsVector;
} // namespace C3D


//-------------------------------------------------------------------------------
/** \brief \ru Точка на ребре.
           \en A point on an edge. \~
  \details \ru Точка на ребре. Точка лежит на ребре, учитывая толерантность вершин. Содержит информацию о параметре ребра. \n
           \en A point on an edge. A point belong to the edge, taking into account the tolerance of the vertices. It contains information about edge parameter. \n \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbEdgePoint {
private:
  MbCartPoint3D _point;     ///< \ru Точка на ребре с учетом толерантности вершин. \en A point on the edge taking into account vertex tolerance.
  double        _edgeParam; ///< \ru Параметр на ребре. \en Parameter on the edge.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbEdgePoint()
    : _point    ( UNDEFINED_DBL, UNDEFINED_DBL, UNDEFINED_DBL )
    , _edgeParam( UNDEFINED_DBL                               )
  {
  }

  /// \ru Конструктор копирования. \en Copy constructor.
  MbEdgePoint( const MbEdgePoint & other )
    : _point    ( other._point     )
    , _edgeParam( other._edgeParam )
  {}

  /// \ru Инициализация. \en Initialization.
  void Init( const MbCartPoint3D & point,
                   double          edgeParam )
  {
    _point = point;
    _edgeParam = edgeParam;
  }

  /** \brief \ru Инициализация по параметрам.
             \en Initialization by parameters. \~
    \details \ru Инициализация точки по вершине ребра.
             \en Point's initialization by edge's vertex. \~
    \param[in] edge - \ru Ребро.
                      \en Edge. \~
    \param[in] begVertex - \ru Начальная вершина.
                           \en Starting vertex. \~
  */
  void InitPointOnVertex( const MbEdge & edge,
                                bool     begVertex );

  /** \brief \ru Инициализация по параметрам.
             \en Initialization by parameters. \~
    \details \ru Инициализация точки на ребре по параметру ребра.
             \en Initialization a point on an edge by edge's parameter. \~
    \param[in] edge - \ru Ребро.
                      \en Edge. \~
    \param[in] edgeParam - \ru Параметр на ребре.
                           \en Edge parameter. \~
  */
  void InitPointOnEdge( const MbEdge & edge,
                              double   edgeParam );

  /// \ru Выдать точку. \en Get point.
  const MbCartPoint3D & GetEdgePoint() const { return _point; }
  /// \ru Выдать параметр на ребре. \en Get edge parameter.
  double                GetEdgeParam() const { return _edgeParam; }
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal?  
  bool                  IsSame( const MbEdgePoint & other, double accuracy ) const;
  /// \ru Преобразовать элемент согласно матрице. \en Transform element by matrix.
  void                  Transform( const MbMatrix3D & matrix ) { _point.Transform( matrix ); }
  /// \ru Поворот точки вокруг произвольной оси \en rotate a point around an arbitrary axis.
  void                  Rotate( const MbAxis3D & axis, double angle ) { _point.Rotate( axis, angle ); }
  /// \ru Сдвиг точки на произвольный вектор \en Move a point around on arbitrary vector.
  void                  Move( const MbVector3D & to ) { _point.Move( to ); }

  /// \ru Проверка на корректность созданного объекта. \en Checking the correctness of the created object.
  bool                  IsValid() const
  {
    return _point.x != UNDEFINED_DBL &&
           _point.y != UNDEFINED_DBL &&
           _point.z != UNDEFINED_DBL &&
           _edgeParam != UNDEFINED_DBL;
  }

  /// \ru Оператор присваивания. \en Assignment operator.
  const MbEdgePoint & operator = ( const MbEdgePoint & other ) {
    _point = other._point;
    _edgeParam = other._edgeParam;
    return *this;
  }

  /// \ru Оператор сравнения. \en Comparison operator.
  bool operator == ( const MbEdgePoint & other ) const {
    return ::fabs( _edgeParam - other._edgeParam ) < DOUBLE_EPSILON &&
           _point == other._point;
  }

  /// \ru Оператор сравнения. \en Comparison operator.
  bool operator < ( const MbEdgePoint & other ) const {
    return _edgeParam - other._edgeParam < -DOUBLE_EPSILON;
  }

  /// \ru Проверить на равенство. \en Check for an equality. 
  bool IsEqual( const MbEdgePoint & other, double tRegion, double mEpsilon ) const {
    return ::fabs( _edgeParam - other._edgeParam ) < tRegion &&
           _point.DistanceToPoint( other._point ) < mEpsilon;
  }

  KNOWN_OBJECTS_RW_REF_OPERATORS( MbEdgePoint ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class. \~
};


//-------------------------------------------------------------------------------
/** \brief \ru Точка на каркасе.
           \en Wire frame point. \~
  \details \ru Точка на каркасе. Лежит на одном из ребер каркаса, учитывая толерантность вершин. Содержит информацию о номере ребра и параметре ребра. \n
           \en Wire frame point. Belong to on one of the edges of the frame, taking into account the tolerance of the vertices. Contains information about the edge number and edge parameter. \n \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbWireFramePoint {
private:
  MbEdgePoint _edgePoint; ///< \ru Точка на ребре каркаса с учетом толерантности вершин. \en A point on a frame edge taking into account vertex tolerance.
  size_t      _edgeIndex; ///< \ru Номер ребра каркаса. \en Frame edge number.
 
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbWireFramePoint()
    : _edgePoint(           )
    , _edgeIndex( SYS_MAX_T )   
  {
  }

  /// \ru Конструктор копирования. \en Copy constructor.
  MbWireFramePoint( const MbWireFramePoint & other )
    : _edgePoint( other._edgePoint )
    , _edgeIndex( other._edgeIndex )
  {}

  /// \ru Инициализация. \en Initialization.
  void Init( const MbEdgePoint & edgePoint,
                   size_t        edgeIndex )
  {
    _edgePoint = edgePoint;
    _edgeIndex = edgeIndex;
  }

  /** \brief \ru Инициализация по параметрам.
             \en Initialization by parameters. \~
    \details \ru Инициализация вершины ребра каркаса по параметрам.
             \en Initialization wire frame edge vertex by parameters. \~
    \param[in] wireFrame - \ru Проволочный каркас.
                           \en WireFrame. \~
    \param[in] edgeIndex - \ru Индекс ребра в каркасе.
                           \en Index of the edge in the frame. \~
    \param[in] firstVertex - \ru Первая вершина.
                             \en First vertex. \~
  */
  bool InitPointOnVertex( const MbWireFrame & wireFrame, 
                                size_t        edgeIndex, 
                                bool          firstVertex );

  /** \brief \ru Инициализация по параметрам.
             \en Initialization by parameters. \~
    \details \ru Инициализация точки на кривой ребра.
             \en Initialization point on edge curve. \~
    \param[in] wireFrame - \ru Проволочный каркас.
                           \en WireFrame. \~
    \param[in] edgeIndex - \ru Индекс ребра в каркасе.
                           \en Index of the edge in the frame. \~
    \param[in] edgeParam - \ru Параметр на ребре.
                           \en Edge parameter. \~
  */
  bool InitPointOnEdge( const MbWireFrame & wireFrame, 
                              size_t        edgeIndex, 
                              double        edgeParam );

  /// \ru Выдать точку. \en Get point.
  const MbCartPoint3D & GetEdgePoint() const { return _edgePoint.GetEdgePoint(); }
  /// \ru Выдать номер ребра. \en Get edge number.
  size_t                GetEdgeIndex() const { return _edgeIndex; }
  /// \ru Выдать параметр на ребре. \en Get edge parameter.
  double                GetEdgeParam() const { return _edgePoint.GetEdgeParam(); }
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal?  
  bool                  IsSame( const MbWireFramePoint & other, double accuracy ) const;
  /// \ru Преобразовать элемент согласно матрице. \en Transform element by matrix.
  void                  Transform( const MbMatrix3D & matrix ) { _edgePoint.Transform( matrix ); }
  /// \ru Поворот точки вокруг произвольной оси \en rotate a point around an arbitrary axis.
  void                  Rotate( const MbAxis3D & axis, double angle ) { _edgePoint.Rotate( axis, angle ); }
  /// \ru Сдвиг точки на произвольный вектор \en Move a point around on arbitrary vector.
  void                  Move( const MbVector3D & to ) { _edgePoint.Move( to ); }

  /// \ru Проверка на корректность созданного объекта. \en Checking the correctness of the created object.
  bool                  IsValid() const
  {
    return _edgePoint.IsValid() &&  
           _edgeIndex != SYS_MAX_T;    
  }

  /// \ru Установить номер ребра. \en Set edge number.
  void                  SetEdgeIndex( size_t ind ) { _edgeIndex = ind; }

  /// \ru Оператор присваивания. \en Assignment operator.
  const MbWireFramePoint & operator = ( const MbWireFramePoint & other ) {
    _edgePoint = other._edgePoint;
    _edgeIndex = other._edgeIndex;
    return *this;
  }

  /// \ru Оператор сравнения. \en Comparison operator.
  bool operator == ( const MbWireFramePoint & other ) const {
    return _edgeIndex == other._edgeIndex &&  
           _edgePoint == other._edgePoint;
  }

  /// \ru Оператор сравнения. \en Comparison operator.
  bool operator < ( const MbWireFramePoint & other ) const {
    bool res = _edgeIndex < other._edgeIndex;

    if ( _edgeIndex == other._edgeIndex ) 
      res =_edgePoint < other._edgePoint;
    
    return res;
  }

  /// \ru Проверить на равенство. \en Check for an equality. 
  bool IsEqual( const MbWireFramePoint & other, double tRegion, double mEpsilon ) const {
    return _edgeIndex == other._edgeIndex &&       
           _edgePoint.IsEqual( other._edgePoint, tRegion, mEpsilon );
  }

  KNOWN_OBJECTS_RW_REF_OPERATORS( MbWireFramePoint ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class. \~
};


#endif // __OP_WIREFRAME_POINT_H
