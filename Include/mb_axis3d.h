////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Ось - вектор, привязанный к фиксированной точке.
         \en The axis-vector which is attached to a fixed point. \~

*/
////////////////////////////////////////////////////////////////////////////////


#ifndef __MB_AXIS3D_H
#define __MB_AXIS3D_H


#include <mb_cart_point3d.h>
#include <io_base.h>

class   MbCartPoint;
class   MbMatrix3D;
class   MbPlacement3D;


//------------------------------------------------------------------------------
/** \brief \ru Ось - вектор, привязанный к фиксированной точке.
           \en The axis-vector which is attached to a fixed point. \~
  \details \ru Фиксированная точка - начало оси. Вектор задает положительное направление. 
   Используется для поворота объектов в пространстве \n
           \en The fixed point is the axis origin. The vector defines a positive direction. 
   Is used for a rotation of objects in the space \n \~
  \ingroup Mathematic_Base_3D
*/ 
// ---
class MATH_CLASS MbAxis3D 
{
  MbCartPoint3D origin; ///< \ru Положение начала. \en An origin position. 
  MbVector3D    axisZ;  ///< \ru Направление оси (вектор единичной длины). \en An axis direction (unit length vector). 

public: // \ru Оси мировой системы координат \en Axes of the world coordinate system 
  static const MbAxis3D xAxis;
  static const MbAxis3D yAxis;
  static const MbAxis3D zAxis;

public :
  /// \ru Пустой конструктор, ось расположена в начале глобальных координат и совпадает с третьей осью глобальных координат. \en Empty constructor. The axis is in the origin of global coordinates and coincides with the third axis of global coordinates. 
  MbAxis3D() : origin( 0, 0, 0 ), axisZ( 1, 0, 0 ) {};
  /// \ru Конструктор по точке и вектору. \en Constructor by a point and a vector. 
  MbAxis3D( const MbCartPoint3D & pnt0, const MbVector3D & dir ) : origin( pnt0 ), axisZ( dir ) { axisZ.Normalize(); C3D_ASSERT( !axisZ.IsDegenerate( LENGTH_EPSILON ) ); }
  /// \ru Конструктор по двум точкам. \en Constructor by two points. 
  MbAxis3D( const MbCartPoint3D & pnt0, const MbCartPoint3D & pnt ) : origin( pnt0 ), axisZ( pnt0, pnt ) { axisZ.Normalize(); C3D_ASSERT( !axisZ.IsDegenerate( LENGTH_EPSILON ) ); }
  /// \ru Конструктор по плейсменту и двум точкам на нем. \en Constructor by a placement and two points on it. 
  MbAxis3D( const MbPlacement3D & place, const MbCartPoint & p1, const MbCartPoint & p2 );
  /// \ru Конструктор по другой оси. \en Constructor by another axis. 
  MbAxis3D( const MbAxis3D & axis ) : origin( axis.origin ), axisZ( axis.axisZ ) {} 
  /// \ru Конструктор по вектору. \en The constructor by a vector. 
  explicit MbAxis3D( const MbVector3D & v ) : origin(), axisZ( v ) { axisZ.Normalize(); C3D_ASSERT( !axisZ.IsDegenerate( LENGTH_EPSILON ) ); }

public :

  /// \ru Инициализация по другой оси. \en The initialization by another axis. 
  void        Init( const MbAxis3D & axis ) { origin = axis.origin; axisZ = axis.axisZ; }
  /// \ru Инициализация по точке и вектору. \en The initialization by a point and a vector 
  void        Init( const MbCartPoint3D & pnt0, const MbVector3D & dir ) { origin = pnt0; axisZ = dir; axisZ.Normalize(); C3D_ASSERT( !axisZ.IsDegenerate( LENGTH_EPSILON ) ); } 
  /// \ru Инициализация по двум точкам. \en The initialization by two points. 
  void        Init( const MbCartPoint3D & pnt0, const MbCartPoint3D & pnt ) { origin = pnt0; axisZ.Init( pnt0, pnt ); axisZ.Normalize(); C3D_ASSERT( !axisZ.IsDegenerate( LENGTH_EPSILON ) ); }
  /// \ru Инициализация только по направлению с сохранением "начала" \en The initialization only by a direction with saving of the "origin" 
  MbAxis3D &  SetAxisZ( const MbVector3D & zAx ) { axisZ = zAx; axisZ.Normalize(); C3D_ASSERT( !axisZ.IsDegenerate( LENGTH_EPSILON ) ); return *this; } 

  /** 
    \ru \name Функции трехмерного объекта
    \en \name The functions of a three-dimensional object
    \{ */
  /// \ru Преобразование согласно матрице. \en The transformation according to a matrix. 
  void        Transform( const MbMatrix3D & );
  /// \ru Сдвиг. \en Move. 
  void        Move     ( const MbVector3D & to ) { origin.Move( to ); }
  /// \ru Поворот вокруг оси. \en The rotation around an axis. 
  void        Rotate   ( const MbAxis3D &, double angle );
  /// \ru Сделать копию элемента. \en Create a copy of the element. 
  MbAxis3D  & Duplicate() const;
  /// \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  double      DistanceToPoint( const MbCartPoint3D & ) const;
  /// \ru Вычислить квадрат расстояния до точки. \en Calculate the square of the distance to a point.
  double      DistanceToPoint2( const MbCartPoint3D & ) const;
  /// \ru Вычислить расстояние до отрезка. \en Calculate the distance to a segment. 
  double      DistanceToSegment( const MbCartPoint3D & p1, const MbCartPoint3D & p2, MbCartPoint3D & p ) const;
  /// \ru Вычислить квадрат расстояния до отрезка. \en Calculate the square of the distance to a segment. 
  double      DistanceToSegment2( const MbCartPoint3D & p1, const MbCartPoint3D & p2, MbCartPoint3D & p ) const;

  /// \ru Проекция точки на ось. \en The point projection on the axis. 
  double      PointProjection( const MbCartPoint3D & p0, MbCartPoint3D & proj ) const;
  /// \ru Проекция точки на ось. \en The point projection on the axis. 
  double      PointProjection( const MbCartPoint3D & p0 ) const;
  /// \ru Проверка соосности. \en The check of complanarity. 
  bool        Complanar      ( const MbPlacement3D & p, double eps = Math::angleRegion ) const;
  /// \ru Проверка коллинеарности осей. \en The check of axes collinearity. 
  bool        Colinear       ( const MbAxis3D &a, double eps = Math::angleRegion ) const;
  /// \ru Положить ось на плейсемент. \en Set the axis on a placement. 
  void        SetOnPlacement ( const MbPlacement3D & );
  /// \ru Масштабирование оси. \en Scaling of the axis. 
  void        Scale( double sx, double sy, double sz );
  /// \ru Дать пространственную точку по параметру на оси. \en Get the space point by a parameter on axis. 
  void          PointOn( const double & t, MbCartPoint3D & p ) const { p.Set( origin, axisZ, t ); }
  MbCartPoint3D PointOn( const double & t ) const { return origin + axisZ*t; }

  void GetProperties( MbProperties & properties ); // \ru Выдать свойства объекта \en Get properties of the object 
  void SetProperties( const MbProperties & ); // \ru Записать свойства объекта \en Set properties of the object 

  /** \} */
  /** \ru \name Функции доступа к полям
      \en \name Functions for access to fields
  \{ */
  /// \ru Получить начало оси. \en Get the axis origin. 
  const MbCartPoint3D & GetOrigin() const { return origin; }
  /// \ru Получить вектор оси. \en Get the axis vector. 
  const MbVector3D &    GetAxisZ () const { return axisZ;  }
        /// \ru Изменить начало оси. \en Edit the axis origin. 
        MbCartPoint3D & SetOrigin()       { return origin; }
        /// \ru Изменить вектор оси. \en Edit the axis vector. 
        MbVector3D &    SetAxisZ ()       { return axisZ;  }

  /// \ru Оператор присваивания. \en An assignment operator. 
  void        operator = ( const MbAxis3D & init ) { origin = init.origin; axisZ = init.axisZ; }
  /// \ru Являются ли объекты равными? \en Are the objects equal?
  bool        IsSame( const MbAxis3D & other, double accuracy ) const;
  /** \} */

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbAxis3D, MATH_FUNC_EX )
  DECLARE_NEW_DELETE_CLASS( MbAxis3D )
  DECLARE_NEW_DELETE_CLASS_EX( MbAxis3D )
}; // MbAxis3D


//------------------------------------------------------------------------------
/** \brief \ru Являются ли объекты равными?
           \en Are the objects equal? \~
  \details \ru Равными считаются объекты, данные которых равны с заданной точностью.
           \en The objects are equal if their data are equal with a given accuracy. \~
  \param[in] other -    \ru Объект для сравнения.
                        \en The object to compare. \~
  \param[in] accuracy - \ru Точность сравнения.
                        \en The accuracy to compare. \~
  \return \ru Равны ли объекты.
          \en Whether the objects are equal.
*/
inline bool MbAxis3D::IsSame( const MbAxis3D & other, double accuracy ) const
{
  return ( origin.IsSame( other.origin, accuracy ) &&
           axisZ.IsSame( other.axisZ, accuracy ) );
}


#endif // __MB_AXIS3D_H
