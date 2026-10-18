////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Полигональный геометрический объект (фасетный объект) с топологией.
         \en A polygonal geometric object with topology - MbPolymesh. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __POLYMESH_H
#define __POLYMESH_H

#include <model_item.h>

class MbPolymeshTopo;
class MbFaceShell;
class IMeshProvider;
class MbRepairInconsistentMeshParams;

//------------------------------------------------------------------------------
/** \brief \ru Полигональный объект модели - представление топологии в виде halfedge объекта.
           \en The polygonal geometric object - represents topology as a halfedge object. \~
  \ingroup Model_Items
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbPolymesh : public MbItem 
{
private:
  MbPolymeshTopo * _pTopo;

private:
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en The copy constructor without implementation prevents from copying by default. 
  MbPolymesh( const MbPolymesh & );
  /// \ru Конструктор копирования с регистратором. \en Copy constructor with registrator. 
  explicit MbPolymesh( const MbPolymesh & other, MbRegDuplicate * iReg );

public:
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  explicit MbPolymesh();
  /// \ru Конструктор по топологии. \en Constructor by topology. 
  explicit MbPolymesh( MbPolymeshTopo * pTopo );
  /// \ru Конструктор. \en Constructor. 
  explicit MbPolymesh( const IMeshProvider & provider, const MbRepairInconsistentMeshParams & params );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbPolymesh();

public: 
  VISITING_CLASS( MbPolymesh );
  
  // \ru Общие функции геометрического объекта. \en Common functions of a geometric object. 

  MbeSpaceType  IsA() const override; // \ru Тип объекта. \en Type of the object. 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Создать копию. \en Create a copy.
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать согласно матрице. \en Transform according to the matrix.
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвинуть вдоль вектора. \en Translate along a vector.
  void    Rotate   ( const MbAxis3D   &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate around an axis.
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Являются ли объекты равными? \en Determine whether objects are equal. 
  bool    SetEqual ( const MbSpaceItem & init ) override;   // \ru Сделать объекты равным. \en Make equal objects. 
  double  DistanceToPoint ( const MbCartPoint3D & ) const override; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  void    AddYourGabaritTo( MbCube & r ) const override; // \ru Добавить свой габарит в куб. \en Add bounding box into a cube. 
  void    CalculateLocalGabarit( const MbMatrix3D & into, MbCube & cube ) const override; // \ru Рассчитать габарит в локальной системе координат. \en Calculate a bounding box in the local coordinate system. 
  void    CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.

  MbProperty & CreateProperty( MbePrompt n ) const override;     // \ru Создать собственное свойство. \en Create own property. 
  void    GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & properties ) override; // \ru Установить свойства объекта. \en Set properties of object. 
  void    GetBasisItems( RPArray<MbSpaceItem> & ) override;   // \ru Выдать базовые объекты. \en Get the base objects. 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  bool    GetCreators( RPArray<MbCreator> & creators ) const override;       // \ru Добавить свои строители в присланный массив. \en Add your own creators to the given array.
  bool    GetCreators( c3d::CreatorsSPtrVector & creators ) const override;  // \ru Добавить свои строители в присланный массив. \en Add your own creators to the given array.

  // \ru Перестроить объект по журналу построения. \en Reconstruct the object according to the history tree. 
  bool    RebuildItem( MbeCopyMode sameShell, RPArray<MbSpaceItem> * items, IProgressIndicator * progInd ) override;
  // \ru Создать полигональный объект - упрощенную копию данного объекта. \en Create a polygonal object - the polygonal copy of a given object. 
  MbItem * CreateMesh( const MbStepData & stepData, const MbFormNote & note, MbRegDuplicate * iReg ) const override; 
  // \ru Добавить себя в присланный полигональный объект mesh без копирования. \en Add itself to the given polygonal object "mesh" without copying. 
  bool    AddYourMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; 
  // \ru Разрезать полигональный объект одной или двумя параллельными плоскостями. \en Cut the polygonal form of an object by one or two parallel planes. 
  MbItem * CutMesh( const MbPlacement3D & cutPlace, double distance, const MbSNameMaker * = nullptr ) const override;
  // \ru Найти ближайший объект или имя ближайшего объекта. \en Find the nearest object or the name of the nearest object.
  // \note \ru В многопоточном режиме выполняется параллельно. \en In multithreaded mode runs in parallel. \~
  bool    NearestMesh( MbeSpaceType sType, MbeTopologyType tType, MbePlaneType pType,
                       const MbAxis3D & axis, double maxDistance, bool gridPriority, double & t, double & dMin, 
                       MbItem *& find, SimpleName & findName,
                       MbRefItem *& element, SimpleName & elementName,
                       MbPath & path, MbMatrix3D & from ) const override;

  MbPolymeshTopo * SetTopo() { return _pTopo; } // \ru Получить для редактирования топологию полигонального объекта. \en Get the topology of the polygonal object for editing. 
  const MbPolymeshTopo * GetTopo() const { return _pTopo; } // \ru Получить топологию полигонального объекта. \en Get the topology of the object. 
  /// \ru Заменить топологию на присланную. \en Replace the topology by the given one. 
  bool ReplaceTopo( MbPolymeshTopo * pTopo );
  /// \ru Получить оболочку-подложку. \en Get the substrate shell. 
  const MbFaceShell * GetSubstrateShell() const;
  /// \ru Удалить оболочку-подложку. \en Delete the substrate shell. 
  void DeleteSubstrateShell();

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  MbPolymesh & operator = ( const MbPolymesh & );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbPolymesh )
}; // MbPolymesh

IMPL_PERSISTENT_OPS( MbPolymesh )

#endif // __POLYMESH_H
