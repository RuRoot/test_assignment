////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Вставка двумерного объекта.
         \en Instance of a two-dimensional object. \~ 

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __PLANE_INSTANCE_H
#define __PLANE_INSTANCE_H


#include <model_item.h>
#include <mb_placement3d.h>
#include <templ_sptr.h>
#include <vector>


class   MbPlaneInstance;
namespace c3d // namespace C3D
{
typedef SPtr<MbPlaneInstance>                 PInstanceSPtr;
typedef SPtr<const MbPlaneInstance>           ConstPInstanceSPtr;

typedef std::vector<MbPlaneInstance *>        PInstancesVector;
typedef std::vector<const MbPlaneInstance *>  ConstPInstancesVector;

typedef std::vector<PInstanceSPtr>            PInstancesSPtrVector;
typedef std::vector<ConstPInstanceSPtr>       ConstPInstancesSPtrVector;
}


//------------------------------------------------------------------------------
/** \brief \ru Вставка двумерных объектов эскиза.
           \en Instance of a two-dimensional sketch objects. \~
  \details \ru Вставка позволяет работать с двумерными геометрическими объектами как с объектом геометрической модели.
    Двумерные геометрические объекты MbPlaneItem располагаются в плоскости XOY локальной системы координат MbPlacement3D.\n
           \en The instance allows to deal with two-dimensional geometric objects as with object of geometric model.
    Two-dimensional MbPlaneItem geometric objects located in XOY-plane of MbPlacement3D local coordinate system.\n \~
  \ingroup Model_Items
*/
// ---
class MATH_CLASS MbPlaneInstance : public MbItem {
protected :
  MbPlacement3D              place;      ///< \ru Локальная система координат объекта. \en Local coordinate system of the object. 
  std::vector<MbPlaneItem *> planeItems; ///< \ru Множество двумерных объектов эскиза. \en A set of two-dimensional sketch objects. 

protected :
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbPlaneInstance( const MbPlaneInstance &, MbRegDuplicate * );
public :
  /// \ru Конструктор по локальной системе координат без двумерных объектов. \en Constructor by a local coordinate system without two-dimensional objects. 
  MbPlaneInstance( const MbPlacement3D & );
  /// \ru Конструктор по двумерному объекту (используется оригинал) и локальной системе координат. \en Constructor by two-dimensional object (original is used) and a local coordinate system.
  MbPlaneInstance( const MbPlaneItem &, const MbPlacement3D & );
  /// \ru Конструктор по двумерным объектам (используются оригиналы) и локальной системе координат. \en Constructor by two-dimensional object (originals are used) and a local coordinate system. 
  template <class PlaneItems>
  MbPlaneInstance( const MbPlacement3D &, const PlaneItems & );
  /// \ru Деструктор \en Destructor 
  virtual ~MbPlaneInstance();

public :
  VISITING_CLASS( MbPlaneInstance ); 

  // \ru Общие функции геометрического объекта. \en Common functions of a geometric object.  

  MbeSpaceType  IsA()  const override; // \ru Тип объекта. \en A type of an object. 
  MbSpaceItem & Duplicate( MbRegDuplicate * iReg = nullptr ) const override; // \ru Создать копию. \en Create a copy.
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать согласно матрице. \en Transform according to the matrix.
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвинуть вдоль вектора. \en Translate along a vector.
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate about an axis.
  bool    IsSame   ( const MbSpaceItem &, double accuracy = LENGTH_EPSILON ) const override; // \ru Являются ли объекты равными? \en Are the objects equal? 
  bool    IsSimilar( const MbSpaceItem & ) const override; // \ru Являются ли объекты подобными? \en Are the objects similar? 
  bool    SetEqual ( const MbSpaceItem & ) override; // \ru Сделать объекты равными. \en Make the objects equal. 
  double  DistanceToPoint ( const MbCartPoint3D & ) const override; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  void    AddYourGabaritTo( MbCube & ) const override; // \ru Добавь свой габарит в куб. \en Add bounding box into a cube. 
  void    CalculateLocalGabarit( const MbMatrix3D & into, MbCube & cube ) const override; // \ru Рассчитать габарит в локальной системы координат. \en Calculate bounding box in the local coordinate system. 
  void    CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.

  MbProperty & CreateProperty( MbePrompt ) const override;     // \ru Создать собственное свойство. \en Create a custom property. 
  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of the object. 
  void    GetBasisPoints( MbControlData & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  // \ru Получить локальную систему координат объекта. \en Get the local coordinate system of an object. 
  bool    GetPlacement( MbPlacement3D &, PlanarCheckParams params = PlanarCheckParams() ) const override; 
  // \ru Установить локальную систему координат объекта. \en Set the local coordinate system of an object. 
  bool    SetPlacement( const MbPlacement3D & ) override;

  // \ru Создать полигональный объект - упрощенную копию данного объекта. \en Create a polygonal object - a simplified copy of the given object. 
  MbItem * CreateMesh( const MbStepData & stepData, const MbFormNote & note, MbRegDuplicate * iReg ) const override; 
  // \ru Найти объект по геометрическому объекту (MbPlaneItem). \en Find the object by a geometric object (MbSpaceItem). 
  const MbItem * FindItem( const MbPlaneItem * s, MbPath & path, MbMatrix3D & from ) const override;

  /** \ru \name Общие функции вставки двумерного объекта.
      \en \name Common functions of instance of two-dimensional object.
      \{ */
  /// \ru Выдать число двумерных геометрических объектов. \en Get number of two-dimensional geometric objects. 
          size_t  PlaneItemsCount() const;
          /// \ru Выдать двумерный геометрический объект. \en Get two-dimensional geometric object. 
  const   MbPlaneItem * GetPlaneItem( size_t ind = 0 ) const; 
          /// \ru Выдать двумерный геометрический объект для возможного редактирования. \en Get two-dimensional object for possible editing. 
          MbPlaneItem * SetPlaneItem( size_t ind = 0 );
          /// \ru Заменить двумерный геометрический объект. \en Replace two-dimensional geometric object. 
          bool    SetPlaneItem( MbPlaneItem * init, size_t ind = 0 ); 
          /// \ru Добавить двумерный геометрический объект. \en Add two-dimensional geometric object. The method returns the index of added or existing object in MbPlaneInstance (the method returns SYS_MAX_T if the object is nullptr).
          size_t  AddPlaneItem( MbPlaneItem * init ); 
          /// \ru Метод возвращает индекс двумерного геометрического объекта. \en The method returns the index of two-dimensional geometric object in MbPlaneInstance (the method returns SYS_MAX_T if the object was not finded). 
          size_t  GetIndex( MbPlaneItem * init ); 
          /// \ru Выдать локальную систему координат объекта. \en Get the local coordinate system of an object. 
  const   MbPlacement3D & GetPlacement() const { return place; }
          /// \ru Выдать локальную систему координат объекта для редактирования. \en Get the local coordinate system of an object for editing. 
          MbPlacement3D & SetPlacement()       { return place; } 
          
          /// \ru Преобразовать двумерный объект согласно матрице. \en Transform two-dimensional object according to the matrix. 
          void    Transform( const MbMatrix &, MbRegTransform * iReg = nullptr );
          /// \ru Сдвинуть двумерный объект вдоль вектора. \en Translate two-dimensional object along a vector. 
          void    Move     ( const MbVector &, MbRegTransform * iReg = nullptr );
          /// \ru Повернуть двумерный объект вокруг точки на заданный угол. \en Rotate two-dimensional object at a given angle around an axis. 
          void    Rotate   ( const MbCartPoint & pnt, const MbDirection & angle, MbRegTransform * iReg = nullptr );
          /// \ru Повернуть двумерный объект вокруг точки на заданный угол. \en Rotate two-dimensional object at a given angle around an axis. 
          void    Rotate   ( const MbCartPoint & pnt, double angle, MbRegTransform * iReg = nullptr );

          /// \ru Удалить все объекты эскиза. \en Delete all the sketch items. 
          void    DeleteItems(); 
          /// \ru Удалить объект эскиза. \en Delete the sketch item by index. 
          bool    DeleteItem( size_t ind ); 
          /// \ru Удалить объект эскиза. \en Delete the sketch item by index. 
          MbPlaneItem * DetachItem( size_t ind ); 

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
          bool    ReplaceItem( const MbPlaneItem & item, MbPlaneItem & newItem ); 

          /// \ru Выдать все объекты. \en Get all the items.  
          template <class PlaneItems>
          void    GetItems( PlaneItems & ) const; 

  /** \} */

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbPlaneInstance ) 
OBVIOUS_PRIVATE_COPY( MbPlaneInstance )
};

IMPL_PERSISTENT_OPS( MbPlaneInstance );


//------------------------------------------------------------------------------
// Конструктор
// ---
template <class PlaneItems>
inline MbPlaneInstance::MbPlaneInstance( const MbPlacement3D & p, const PlaneItems & inits )
  : MbItem    (   )   
  , place     ( p )
  , planeItems(   )
{
  size_t addCnt = inits.size();
  planeItems.reserve( addCnt );
  for ( size_t k = 0; k < addCnt; ++k ) {
    const MbPlaneItem * planeItem = inits[k];
    if ( planeItem != nullptr ) {
      planeItem->AddRef();
      planeItems.push_back( const_cast<MbPlaneItem *>( planeItem ) );
    }
  }
  C3D_ASSERT( !planeItems.empty() );
}


//------------------------------------------------------------------------------
// дать все объекты 
// ---
template <class PlaneItems>
void MbPlaneInstance::GetItems( PlaneItems & items ) const
{
  size_t addCnt = planeItems.size();
  items.reserve( items.size() + addCnt );
  SPtr<MbPlaneItem> item_i;
  for ( size_t i = 0; i < addCnt; ++i ) {
    if ( planeItems[i] != nullptr ) {
      item_i = planeItems[i];
      items.push_back( item_i );
    }
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Набор ошибок - результат диагностики эскиза.
           \en A set of errors is the result of a sketch diagnosis. \~
  \details \ru Набор ошибок и соответствующих объектов эскиза - результат диагностики эскиза. \n
           \en A set of errors and corresponding sketch objects is the result of sketch diagnostics. \n \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbSketchErrors {
private:
  std::multimap<MbResultType, std::vector<MbPlaneItem *>> errors; ///< \ru Набор ошибок ошибок. \en Set of errors.

public:
  /// \ru Конструктор. \en Constructor. 
  MbSketchErrors() {}

public:
  /// \ru Добавить к набору тип ошибки и соответствующие объекты эскиза. \en Add error type and corresponding sketch objects to the set.
  void AddError( MbResultType errorType, std::vector<MbPlaneItem *> & items ) {
    errors.insert( std::pair< MbResultType, std::vector<MbPlaneItem *> >( errorType, items ) );
  }

  /// \ru Есть ли ошибка типа errorType. \en Is there any error like errorType.
  bool IsAnyError( MbResultType errorType ) const { return errors.find( errorType ) != errors.end(); }

  /// \ru Дать набор объектов по типу ошибки. \en Give a set of objects by type of error.
  void GetItemsByErrorType( MbResultType errorType, std::vector<MbPlaneItem *> & items ) const;

  /// \ru Дать набор пар пересекающихся контуров. \en Give a set of pairs of intersecting contours.
  void GetIntersectedItemsPairs( std::vector<std::pair<MbPlaneItem *, MbPlaneItem *>> & items ) const;
};


//-------------------------------------------------------------------------------
/** \brief \ru Параметры диагностики эскиза.
           \en Sketch diagnostic parameters. \~
  \details \ru Параметры диагностики эскиза с набором необходимых проверок. По умолчанию проводятся все проверки. \n
               Возможные проверки: \n
               - проверка контуров на взаимное пересечение (rt_Intersection), \n
               - самопересечение (rt_SelfIntersection), \n
               - поиск разрывов (rt_ContourGapError), \n
               - поиск наложений (rt_ContourSegmentsOverlapError), \n
               - соответствие флага замкнутости и фактической замкнутости (rt_MustBeOpen, rt_MustBeClosed), \n
               - проверка гладкой стыковки сегментов по касательной ( G1 ) с указанной угловой погрешностью (rt_ContourSegmentsNoTangentJoint). \n
           \en Sketch diagnostic parameters with a set of necessary checks. By default, all checks are performed. \n            
               Possible checks: \n
               - checking contours for intersections (rt_Intersection), \n
               - self-intersections (rt_SelfIntersection), \n
               - searching for gaps (rt_ContourGapError), \n
               - searching overlaps (rt_ContourSegmentsOverlapError), \n
               - correspondence of the flag of closure and actual closure (rt_MustBeOpen, rt_MustBeClosed), \n 
               - check of tangent smooth joining of segments ( G1 ) with the specified angular tolerance (rt_ContourSegmentsNoTangentJoint). \n \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbSketchCheckParameters {
private:
  std::set<MbResultType> checkedErrors;  ///< \ru Набор проверок. При пустом контейнере выполняются все проверки. \en A set of checks. If the container is empty, all checks are performed.
  double                 metricEpsilon;  ///< \ru Метрическая погрешность. \en Metric tolerance.
  double                 angleEpsilon;   ///< \ru Угловая погрешность. \en Angular tolerance.

public:
  /// \ru Конструктор. \en Constructor. 
  MbSketchCheckParameters()
    : metricEpsilon( Math::metricEpsilon )
    , angleEpsilon ( Math::angleEpsilon  )
  {}

  /// \ru Можно ли задать этот тип проверки? \en Can I set this type of check?
  bool CanSetThisCheckType( MbResultType errorType ) const {
    return errorType == rt_InvalidType ||
           errorType == rt_SelfIntersection ||
           errorType == rt_Intersection ||
           errorType == rt_ContourGapError ||
           errorType == rt_ContourSegmentsOverlapError ||
           errorType == rt_MustBeOpen ||
           errorType == rt_MustBeClosed ||
           errorType == rt_ContourSegmentsNoTangentJoint;
  }

  /// \ru Добавить тип проверки эскиза. \en Add a sketch verification type.
  void AddCheckType( MbResultType errorType ) {
    if ( CanSetThisCheckType( errorType ) )
      checkedErrors.insert( errorType ); 
  }

  /// \ru Установить метрическую погрешность. \en Set metric tolerance.
  void SetMetricEpsilon( double mEpsilon ) { metricEpsilon = mEpsilon; }
  /// \ru Установить угловую погрешность. \en Set angular tolerance.
  void SetAngleEpsilon( double aEpsilon ) { angleEpsilon = aEpsilon; }

  /// \ru Проверять ли на наличие ошибки типа errorType. \en Do I need to check for errors like errorType.
  bool IsErrorTypeSetToCheck( MbResultType errorType ) const {
    if ( checkedErrors.empty() )
      return CanSetThisCheckType( errorType );
    else
      return std::find( checkedErrors.begin(), checkedErrors.end(), errorType ) != checkedErrors.end();
  }

  /// \ru Выдать метрическую погрешность. \en Get metric tolerance.
  double GetMetricEpsilon() const { return metricEpsilon; }
  /// \ru Выдать угловую погрешность. \en Get angular tolerance.
  double GetAngleEpsilon() const { return angleEpsilon; }
};


//------------------------------------------------------------------------------
/** \brief \ru Проверить эскиз.
           \en Check sketch. \~
  \details \ru Диагностика эскиза. Проверка контуров на взаимное пересечение (rt_Intersection),
               самопересечение (rt_SelfIntersection),
               поиск разрывов (rt_ContourGapError) и наложений (rt_ContourSegmentsOverlapError),
               соответствие флага замкнутости и фактической замкнутости (rt_MustBeOpen, rt_MustBeClosed).
           \en Sketch diagnostics. Checking contours for intersections (rt_Intersection),
               self-intersections (rt_SelfIntersection),
               searching for gaps (rt_ContourGapError) and overlaps (rt_ContourSegmentsOverlapError),
               correspondence of the flag of closure and actual closure (rt_MustBeOpen, rt_MustBeClosed). \~
  \param[in] planeInstance - \ru Эскиз, содержащий набор контуров.
                             \en A sketch containing a set of contours. \~
  \param[in] parameters    - \ru Параметры проверки.
                             \en Parameters of check. \~
  \param[out] errors       - \ru Набор ошибок.
                             \en Errors set. \~
*/
// ---
MATH_FUNC( void ) CheckSketch( const MbPlaneInstance         & planeInstance, 
                               const MbSketchCheckParameters & parameters, 
                                     MbSketchErrors          & errors );


#endif // __PLANE_INSTANCE_H
