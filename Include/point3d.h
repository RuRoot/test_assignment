////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Пространственная точка со свойствами геометрического объекта.
         \en Spatial point with properties of geometric object. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __POINT3D_H
#define __POINT3D_H


#include <space_item.h>
#include <mb_cart_point3d.h>


//------------------------------------------------------------------------------
/** \brief \ru Трёхмерная точка со свойствами геометрического объекта.
           \en Three-dimensional point with properties of geometric object. \~
  \details \ru Точка содержит трехмерную точку MbCartPoint3D и является
  наследником геометрического объекта в пространстве.\n
           \en The point contains three-dimensional point MbCartPoint3D and also is
  the inheritor of a geometric object in space.\n \~
  \ingroup Point_3D
*/
// ---
class MATH_CLASS MbPoint3D : public MbSpaceItem {
protected:
  MbCartPoint3D point; ///< \ru Трехмерная точка. \en Three-dimensional point. 

private:
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbPoint3D ( const MbPoint3D & ); 
public:
  /// \ru Конструктор. \en Constructor. 
  MbPoint3D ();
  /// \ru Конструктор по точке. \en Constructor by point. 
  MbPoint3D ( const MbCartPoint3D & );
  /// \ru Конструктор по координатам. \en Constructor by coordinates. 
  MbPoint3D ( double x, double y, double z );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbPoint3D ();

public:

  // \ru Общие функции геометрического объекта. \en Common functions of a geometric object.  

  MbeSpaceType IsA()  const override; // \ru Тип объекта. \en A type of an object.
  MbeSpaceType Type() const override; // \ru Групповой тип объекта. \en Group type of object. 
  MbeSpaceType Family() const override; // \ru Семейство объекта. \en Family of object. 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Создать копию. \en Create a copy.
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать согласно матрице. \en Transform according to the matrix.
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвинуть вдоль вектора. \en Translate along a vector.
  void    Rotate   ( const MbAxis3D   &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate about an axis.
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Являются ли объекты равными? \en Are the objects equal? 
  bool    SetEqual ( const MbSpaceItem & init ) override;   // \ru Сделать объекты равным. \en Make the objects equal. 
  double  DistanceToPoint ( const MbCartPoint3D & ) const override; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  void    AddYourGabaritTo( MbCube & r ) const override; // \ru Добавь свой габарит в куб. \en Add bounding box into a cube. 
  void    CalculateLocalGabarit( const MbMatrix3D & into, MbCube & cube ) const override; // \ru Рассчитать габарит в локальной системы координат. \en Calculate bounding box in the local coordinate system. 
  void    CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.

  MbProperty & CreateProperty( MbePrompt n ) const override;     // \ru Создать собственное свойство. \en Create a custom property. 
  void    GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & properties ) override; // \ru Установить свойства объекта. \en Set properties of the object. 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \ru \name Функции точки.
      \en \name Functions of point.
  \{ */  
          /// \ru Выдать декартову точку. \en Get Cartesian point. 
          void    GetCartPoint( MbCartPoint3D & ) const; 
          /// \ru Выдать декартову точку. \en Get Cartesian point. 
  const   MbCartPoint3D & GetCartPoint() const { return point; } 
          /// \ru Выдать декартову точку для возможного редактирования. \en Get Cartesian point for possible editing. 
          MbCartPoint3D & SetCartPoint()       { return point; } 
          /// \ru Проверить равенство с другой точкой. \en Check for equality with another point. 
          bool    operator == ( const MbPoint3D & ) const;
          /// \ru Проверить на неравенство с другой точкой. \en Check for inequality with another point. 
          bool    operator != ( const MbPoint3D & ) const;
          /// \ru Инициализировать точку по другой точке. \en Initialize point by another point. 
          void    Init( const MbPoint3D & init ) { point.Init(init.point); }
          /// \ru Инициализировать точку по другой точке. \en Initialize point by another point. 
          void    Init( const MbCartPoint3D & init ) { point.Init(init); }
          /// \ru Инициализировать точку по координатам. \en Initialize point by coordinates. 
          void    Init( double xx, double yy, double zz ) { point.Init(xx,yy,zz); }
          /// \ru Обнуление координат. \en Set coordinates to zero. 
          void    SetZero() { point.SetZero(); }
  /** \} */

private:
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation, to prevent an assignment by default. 
          void    operator = ( const MbPoint3D & ); 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbPoint3D )
}; // MbPoint3D

IMPL_PERSISTENT_OPS( MbPoint3D )

#endif // __POINT3D_H
