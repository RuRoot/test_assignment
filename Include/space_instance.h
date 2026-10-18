//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Вставка трёхмерного объекта.
         \en Instance of three-dimensional object. \~

*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SPACE_INSTANCE_H
#define __SPACE_INSTANCE_H


#include <space_item.h>
#include <model_item.h>
#include <mb_placement3d.h>
#include <math_define.h>


class    MbLegend;
class    MbPoint3D;
class    MbCurve3D;
class    MbSurface;


class   MbSpaceInstance;
namespace c3d // namespace C3D
{
typedef SPtr<MbSpaceInstance>                 SInstanceSPtr;
typedef SPtr<const MbSpaceInstance>           ConstSInstanceSPtr;

typedef std::vector<MbSpaceInstance *>        SInstancesVector;
typedef std::vector<const MbSpaceInstance *>  ConstSInstancesVector;

typedef std::vector<SInstanceSPtr>            SInstancesSPtrVector;
typedef std::vector<ConstSInstanceSPtr>       ConstSInstancesSPtrVector;
}


//----------------------------------------------------------------------------------------
/** \brief  \ru Вставка трёхмерного объекта.
            \en Instance of three-dimensional object. \~
  \details  \ru Вставка позволяет работать с трёхмерным геометрическим объектом, как  с 
            объектом геометричекой модели. Вставка позволяет использовать в геометричекой 
            модели любые другие объекты MbSpaceItem, например, резьбу и условные обозначения.
            \en Instance allows to deal with three-dimensional object as with object of 
            geometric model. Instance allows to use any objects inherited from MbSpaceItem
            such as thread and conventional notations in geometric model. \~
  \ingroup Model_Items
*/
// ---
class MATH_CLASS MbSpaceInstance : public MbItem {
protected :
  MbSpaceItem * spaceItem; ///< \ru Трёхмерный геометрический объект. \en Three-dimensional geometric object. 

protected :
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbSpaceInstance( const MbSpaceInstance &, MbRegDuplicate * );
public :
  /// \ru Конструктор по вспомогательному объекту. \en Constructor by auxiliary item. 
  MbSpaceInstance( MbLegend & );
  /// \ru Конструктор по точке. \en Constructor by point. 
  MbSpaceInstance( MbPoint3D & );
  /// \ru Конструктор по кривой. \en Constructor by curve. 
  MbSpaceInstance( MbCurve3D & );
  /// \ru Конструктор по поверхности. \en Constructor by surface. 
  MbSpaceInstance( MbSurface & );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbSpaceInstance();

public :
  VISITING_CLASS( MbSpaceInstance ); 

  // \ru Общие функции геометрического объекта. \en Common functions of a geometric object.  
  MbeSpaceType  IsA()  const override; // \ru Тип объекта. \en A type of an object. 
  MbSpaceItem & Duplicate( MbRegDuplicate * iReg = nullptr ) const override; // \ru Создать копию. \en Create a copy.
  void    Transform( const MbMatrix3D &, MbRegTransform * iReg = nullptr ) override; // \ru Преобразовать согласно матрице. \en Transform according to the matrix.
  void    Move     ( const MbVector3D &, MbRegTransform * iReg = nullptr ) override; // \ru Сдвинуть вдоль вектора. \en Translate along a vector.
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * iReg = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate about an axis.
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Являются ли объекты равными? \en Are the objects equal?
  bool    IsSimilar( const MbSpaceItem & init ) const override; // \ru Являются ли объекты подобными? \en Are the objects similar? 
  bool    SetEqual ( const MbSpaceItem & init ) override; // \ru Сделать объекты равными. \en Make the objects equal. 
  double  DistanceToPoint ( const MbCartPoint3D & ) const override; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  void    AddYourGabaritTo( MbCube & r ) const override; // \ru Добавь свой габарит в куб. \en Add bounding box into a cube. 
  void    CalculateLocalGabarit( const MbMatrix3D & into, MbCube & cube ) const override; // \ru Рассчитать габарит в локальной системы координат. \en Calculate bounding box in the local coordinate system. 
  void    CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.

  MbProperty & CreateProperty( MbePrompt n ) const override; // \ru Создать собственное свойство. \en Create a custom property. 
  void    GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & properties ) override; // \ru Установить свойства объекта. \en Set properties of the object. 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  // \ru Создать полигональный объект - упрощенную копию данного объекта. \en Create a polygonal object - a simplified copy of the given object. 
  MbItem *        CreateMesh( const MbStepData & stepData, const MbFormNote & note, MbRegDuplicate * iReg ) const override;
  // \ru Найти объект по геометрическому объекту (MbSpaceItem). \en Find the object by a geometric object (MbSpaceItem). 
  const MbItem *  FindItem( const MbSpaceItem * s, MbPath & path, MbMatrix3D & from ) const override;
  // \ru Дать все объекты указанного типа. \en Get all objects by type. \~
  bool            GetItems( MbeSpaceType type, const MbMatrix3D & from,
                                    RPArray<MbItem> & items, SArray<MbMatrix3D> & matrs ) override;
  // \ru Дать все объекты указанного типа. \en Get all objects by type. \~
  bool            GetItems( MbeSpaceType type, const MbMatrix3D & from,
                            std::vector<SPtr<MbItem>> & items, std::vector<MbMatrix3D> & matrs ) override;
  // \ru Дать все уникальные объекты указанного типа. \en Get all unique objects by type . \~
  bool            GetUniqItems( MbeSpaceType type, CSSArray<const MbItem *> & items ) const override;

  /** \ru \name Общие функции вставки трёхмерного объекта.
      \en \name Common functions of instance of three-dimensional object.
      \{ */
          /// \ru Выдать трёхмерный геометрический объект. \en Get three-dimensional geometric object. 
  const MbSpaceItem * GetSpaceItem() const; 
          /// \ru Выдать трёхмерный геометрический объект для модификации. \en Get three-dimensional geometric object for modification. 
        MbSpaceItem * SetSpaceItem();       
          /// \ru Заменить геометрический объект. \en Replace geometric object. 
        void          SetSpaceItem( MbSpaceItem * init ); 
  /** \} */

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSpaceInstance ) 
OBVIOUS_PRIVATE_COPY( MbSpaceInstance )
};

IMPL_PERSISTENT_OPS( MbSpaceInstance )

#endif // __SPACE_INSTANCE_H
