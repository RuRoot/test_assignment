//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Вставка объекта.
         \en Instance of object. \~
  \details \ru Вставка объекта геометрической модели MbItem в локальной системе координат MbPlacement3D
  позволяет накладывать геометрические ограничения на объект в сборке и позиционировать объект. 
  Вставка не может содержать другую вставку. \n
           \en The instance of geometric model MbItem object in local coordinate system MbPlacement3D
  allows to define constraints for an object in the assembly and to locate the object. 
  The instance cannot contain another instance. \n \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __INSTANCE_ITEM_H
#define __INSTANCE_ITEM_H

#include <model_item.h>
#include <mb_placement3d.h>


class   MbSolid;
class   MbInstance;
class   MbAssembly;

namespace c3d // namespace C3D
{
typedef SPtr<MbInstance>                InstanceSPtr;
typedef SPtr<const MbInstance>          ConstInstanceSPtr;

typedef std::vector<MbInstance *>       InstancesVector;
typedef std::vector<const MbInstance *> ConstInstancesVector;

typedef std::vector<InstanceSPtr>       InstancesSPtrVector;
typedef std::vector<ConstInstanceSPtr>  ConstInstancesSPtrVector;
}


//----------------------------------------------------------------------------------------
/** \brief \ru Вставка объекта.
           \en Instance of object. \~
  \details \ru Вставка объекта геометрической модели MbItem в локальной системе координат MbPlacement3D
    позволяет накладывать геометрические ограничения на объект в сборке и позиционоровать объект. 
    Вставка не может содержать другую вставку. \n
           \en The instance of geometric model MbItem object in local coordinate system MbPlacement3D
    allows to define constraints for an object in the assembly and to locate the object. 
    The instance cannot contain another instance. \n \~
  \ingroup Model_Items
*/
// ---
class MATH_CLASS MbInstance : public MbItem 
{
  MbPlacement3D place; ///< \ru Локальная система координат объекта. \en Local coordinate system of the object.   
  SPtr<MbItem>  item;  ///< \ru Геометрический объект. \en A geometric object.

protected :
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbInstance( const MbInstance &, MbRegDuplicate * );

public :
  /// \ru Конструктор по объекту и его системе координат. \en The constructor by an object and its coordinate system.  
  MbInstance( MbItem &, const MbPlacement3D & );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbInstance();

public :
  VISITING_CLASS( MbInstance ); 

  // \ru Общие функции геометрического объекта. \en Common functions of a geometric object.  
  MbeSpaceType  IsA()  const override; // \ru Тип объекта. \en An object type. 
  MbSpaceItem & Duplicate( MbRegDuplicate * iReg = nullptr ) const override; // \ru Создать копию. \en Create a copy.
  void    Transform( const MbMatrix3D &, MbRegTransform * iReg = nullptr ) override; // \ru Преобразовать согласно матрице. \en Transform according to the matrix.
  void    Move     ( const MbVector3D &, MbRegTransform * iReg = nullptr ) override; // \ru Сдвинуть вдоль вектора. \en Move along a vector.
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * iReg = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate about an axis.
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Являются ли объекты равными? \en Whether the objects are equal?
  bool    IsSimilar( const MbSpaceItem & init ) const override; // \ru Являются ли объекты подобными? \en Whether the objects are similar? 
  bool    SetEqual ( const MbSpaceItem & init ) override; // \ru Сделать объекты равными. \en Make the objects equal. 
  double  DistanceToPoint ( const MbCartPoint3D & ) const override; // \ru Вычислить расстояние до точки. \en Calculate distance to point. 
  void    AddYourGabaritTo( MbCube & r ) const override; // \ru Добавь свой габарит в куб. \en Add own bounding box to the bounding box. 
  void    CalculateLocalGabarit( const MbMatrix3D & into, MbCube & cube ) const override; // \ru Рассчитать габарит в локальной системы координат. \en Calculate the bounding box in a local coordinate system. 
  void    CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.

  MbProperty & CreateProperty( MbePrompt n ) const override;     // \ru Создать собственное свойство. \en Create own property. 
  void    GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & properties ) override; // \ru Установить свойства объекта. \en Set properties of the object. 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  // \ru Получить систему координат объекта. \en Get the coordinate system of an item. 
  bool    GetPlacement( MbPlacement3D &, PlanarCheckParams params = PlanarCheckParams() ) const override; 
  // \ru Установить систему координат объекта. \en Set the coordinate system of an item. 
  bool    SetPlacement( const MbPlacement3D & ) override;

  // \ru Перестроить объект по журналу построения. \en Reconstruct object according to the history tree. 
  bool    RebuildItem( MbeCopyMode sameShell, RPArray<MbSpaceItem> * items, IProgressIndicator * progInd ) override;

  // \ru Создать полигональный объект - упрощенную копию данного объекта. \en Create a polygonal object - a simplified copy of the given object. 
  MbItem * CreateMesh( const MbStepData & stepData, const MbFormNote & note, MbRegDuplicate * iReg ) const override; 
  // \ru Добавить полигонную сетку объекта. \en Add a polygon mesh of the object. 
  bool    AddYourMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override;
  // \ru Разрезать полигональный объект одной или двумя параллельными плоскостями. \en Cut the polygonal object by one or two parallel planes. 
  MbItem * CutMesh( const MbPlacement3D & cutPlace, double distance, const MbSNameMaker * = nullptr ) const override;
  // \ru Найти ближайший объект или имя ближайшего объекта. \en Find the closest object or its name. 
  bool    NearestMesh( MbeSpaceType sType, MbeTopologyType tType, MbePlaneType pType,
                               const MbAxis3D & axis, double maxDistance, bool gridPriority, double & t, double & dMin, 
                               MbItem *& find, SimpleName & findName,
                               MbRefItem *& element, SimpleName & elementName,
                               MbPath & path, MbMatrix3D & from ) const override;
  // \ru Дать все объекты указанного типа. \en Get all objects by type. \~
  bool    GetItems( MbeSpaceType type, const MbMatrix3D & from,
                            RPArray<MbItem> & items, SArray<MbMatrix3D> & matrs ) override;
  // \ru Дать все объекты указанного типа. \en Get all objects by type. \~
  bool    GetItems( MbeSpaceType type, const MbMatrix3D & from,
                    std::vector<SPtr<MbItem>> & items, std::vector<MbMatrix3D> & matrs ) override;
  // \ru Дать все уникальные объекты указанного типа. \en Get all unique objects by type . \~
  bool    GetUniqItems( MbeSpaceType type, CSSArray<const MbItem *> & items ) const override; 
  // \ru Дать объект по его пути положения в модели и матрицу преобразования объекта в глобальную систему координат. \en Get the object by its path in the model and get the matrix of transformation of the object to the global coordinate system. 
  const MbItem * GetItemByPath( const MbPath & path, size_t ind, MbMatrix3D & from, size_t currInd = 0 ) const override; 

  // \ru Найти объект по геометрическому объекту (MbSpaceItem). \en Find the object by a geometric object (MbSpaceItem). 
  const MbItem * FindItem( const MbSpaceItem * s, MbPath & path, MbMatrix3D & from ) const override;
  // \ru Найти объект по геометрическому объекту (MbPlaneItem). \en Find the object by a geometric object (MbSpaceItem). 
  const MbItem * FindItem( const MbPlaneItem * s, MbPath & path, MbMatrix3D & from ) const override;
  // \ru Найти объект и матрицу его преобразования в глобальную систему координат. \en Find the object and the matrix of its transformation to the global coordinate system. 
  const MbItem * FindItem( const MbItem * s, MbPath & path, MbMatrix3D & from ) const override; 
  // \ru Дать объект с заданным именем и матрицу его преобразования в глобальную систему координат. \en Get the object with the specified name and the matrix of its transformation to the global coordinate system. 
  const MbItem * GetItemByName( SimpleName n, MbPath & path, MbMatrix3D & from ) const override; 

  // \ru Преобразовать согласно матрице c использованием регистратора содержимый объект, если он селектирован. \en Transform the contained object according to the matrix using the registrator if the object selected.  
  void    TransformSelected( const MbMatrix3D & matr, MbRegTransform * iReg = nullptr ) override;
  // \ru Сдвинуть вдоль вектора с использованием регистратора содержимый объект, если он селектирован. \en Translate the contained object along the vector according to the matrix using the registrator if the object selected. 
  void    MoveSelected( const MbVector3D & to, MbRegTransform * iReg = nullptr ) override;
  // \ru Повернуть вокруг оси на заданный угол с использованием регистратора содержимый объект, если он селектирован. \en Translate the contained object about the axis according to the matrix using the registrator if the object selected.  
  void    RotateSelected( const MbAxis3D & axis, double angle, MbRegTransform * iReg = nullptr ) override;

  /// \ru Дать матрицу преобразования из локальной системы объекта. \en Get transform matrix from local coordinate system of object.
  bool    GetMatrixFrom( MbMatrix3D & from ) const override;
  /// \ru Дать матрицу преобразования в  локальную систему объекта. \en Get transform matrix into local coordinate system of object.
  bool    GetMatrixInto( MbMatrix3D & into ) const override;

  /** \ru \name Функции вставки объекта.
      \en \name Functions of the object instance.
      \{ */
          /// \ru Выдать геометрический объект. \en Get the geometric object. 
  const   MbItem * GetItem() const { return item; }
          /// \ru Выдать геометрический объект для модификации. \en Get the geometric object for modification. 
          MbItem * SetItem()       { return item; }
          /// \ru Установить другой геометрический объект. \en Set another geometric object. 
          void    SetItem( MbItem * init ); 

          /** \brief \ru Заменить объект.
                     \en Replace an item. \~
            \details \ru Заменить объект новым.
                     \en Replace an item by a new one. \~
            \param[in] item - \ru Заменяемый объект.
                              \en An item to be replaced. \~
            \param[in] newItem - \ru Новый объект.
                                 \en A new item. \~
            \return \ru Возвращает true, если замена была выполнена.
                    \en Returns true if the replacement has been performed. \~
          */
          bool    ReplaceItem( const MbItem & item, MbItem & newItem, bool saveName = false ); 

          /// \ru Выдать количество граней. \en Get the number of faces. 
          size_t  GetFacesCount() const;
          /// \ru Заполнить контейнер гранями тела. \en Fill container by solid faces. 
          template <class FacesVector>
          void    GetFacesSet( FacesVector & faces ) const;
          /// \ru Выдать систему координат объекта. \en Get the coordinate system of an item. 
  const   MbPlacement3D & GetPlacement() const { return place; }
          /// \ru Выдать систему координат объекта для редактирования. \en Get the coordinate system of an assembly item for editing. 
          MbPlacement3D & SetPlacement()       { return place; } 

  /** \} */

private:
  // Найти объект по геометрическому объекту
  template<class _ItemType>
  const MbItem * _FindItem( const _ItemType * s, MbPath & path, MbMatrix3D & from ) const;

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbInstance )
OBVIOUS_PRIVATE_COPY( MbInstance )
};

IMPL_PERSISTENT_OPS( MbInstance )


#endif // __INSTANCE_ITEM_H
