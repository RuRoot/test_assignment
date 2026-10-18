////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Вспомогательный геометрический объект в трехмерном пространстве.
         \en Auxiliary geometric object in the three-dimensional space. \~
  \details \ru Базовый абстрактный класс вспомогательного геометрического объекта. \n
           \en Base abstract class of auxiliary geometric object. \n \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __LEGEND_H
#define __LEGEND_H

#include <space_item.h>


//------------------------------------------------------------------------------
/** \brief \ru Вспомогательный геометрический объект.
           \en Auxiliary geometric object. \~
  \details \ru Базовый абстрактный класс вспомогательного геометрического объекта. \n
    Вспомогательные объекты описывают базовые точки других объектов, резьбу, выносные линии, шероховатости и условные обозначения.\n
           \en Base abstract class of auxiliary geometric object. \n
    Auxiliary objects describe base points of other objects: thread, extension lines, roughness and notation conventions. \n \~
  \ingroup Legend
*/
// ---
class MATH_CLASS MbLegend : public MbSpaceItem 
{
protected :
  /// \ru Конструктор. \en Constructor. 
  MbLegend();
public:
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbLegend();

public: /* \ru Общие функции геометрического объекта. \en Common functions of a geometric object. */

  //virtual MbeSpaceType  IsA()  const = 0; // \ru Тип объекта. \en Type of the object.
  //virtual MbeSpaceType  Type() const = 0; // \ru Тип объекта. \en Type of the object.
  MbeSpaceType  Family() const override; // \ru Семейство элемента. \en Family of the element.
  //virtual MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const = 0; // \ru Создать копию. \en Create a copy.
  //virtual void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) = 0; // \ru Преобразовать согласно матрице. \en Transform according to the matrix.
  //virtual void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) = 0; // \ru Сдвинуть вдоль вектора. \en Translate along a vector.
  //virtual void    Rotate   ( const MbAxis3D   &, double angle, MbRegTransform * = nullptr ) = 0; // \ru Повернуть вокруг оси. \en Rotate around an axis.
  //virtual bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const = 0; // \ru Являются ли объекты равными? \en Determine whether the objects are equal.
  //virtual bool    IsSimilar( const MbSpaceItem & init ) const = 0; // \ru Являются ли объекты подобными? \en Determine whether the objects are similar.
  //virtual bool    SetEqual ( const MbSpaceItem & init ) = 0;   // \ru Сделать объекты равным. \en Make the objects equal.
  //virtual double  DistanceToPoint ( const MbCartPoint3D & ) const = 0; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point.
  //virtual void    AddYourGabaritTo( MbCube & r ) const = 0; // \ru Добавь свой габарит в куб. \en Add bounding box into a cube.
  void    CalculateLocalGabarit( const MbMatrix3D & into, MbCube & cube ) const override; // \ru Рассчитать габарит в локальной системы координат. \en Calculate the bounding box in a local coordinate system. 
  //virtual void    CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const = 0; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.

  //virtual MbProperty & CreateProperty( MbePrompt n ) const = 0;     // \ru Создать собственное свойство. \en Create a custom property.
  //virtual void    GetProperties( MbProperties & properties ) = 0; // \ru Выдать свойства объекта. \en Get properties of the object.
  //virtual void    SetProperties( const MbProperties & properties ) = 0; // \ru Установить свойства объекта. \en Set properties of the object.

  DECLARE_PERSISTENT_CLASS( MbLegend );
  OBVIOUS_PRIVATE_COPY( MbLegend );
};

IMPL_PERSISTENT_OPS( MbLegend )


#endif // __LEGEND_H
