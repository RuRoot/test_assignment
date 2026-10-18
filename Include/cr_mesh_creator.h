////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение полигонального объекта.
         \en Construction of polygonal object. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_MESH_CREATOR_H
#define __CR_MESH_CREATOR_H


#include <creator.h>


class    MbMesh;
class    MbGrid;
class    MbPolygon3D;
class    MbApex3D;
class    MbItem;


//------------------------------------------------------------------------------
/** \brief \ru Строитель полигонального объекта без истории.
           \en Constructor of a polygonal object without history. \~
  \details \ru Строитель полигонального объекта без истории построения. \n
           \en Constructor of a polygonal object (mesh) without history tree. \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbSimpleMeshCreator : public MbCreator {
public :
  RPArray<MbGrid>       m_grids;  ///< \ru Множество указателей на триангуляции. \en A set of pointers to triangulations. 
  RPArray<MbPolygon3D>  m_wires;  ///< \ru Множество указателей на полигоны. \en A set of pointers to polygons. 
  RPArray<MbApex3D>     m_peaks;  ///< \ru Множество указателей на апексы. \en A set of pointers to apexes. 
  const MbRefItem *     m_item;   ///< \ru Источник сетки. \en Source of mesh. 
  MbeSpaceType          m_type;   ///< \ru Тип сетки. \en Mesh type. 
  bool                  m_closed; ///< \ru Замкнутость. \en Closedness. 
  bool                  m_exact;  ///< \ru Объекты построены на числах double. \en Objects builded on double data. 

public :
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по данным полигонального объекта.
             \en Constructor by mesh data. \~
    \param[in] grids  - \ru Множество указателей на триангуляции.
                        \en A set of pointers to triangulations.
    \param[in] wires  - \ru Множество указателей на полигоны.
                        \en A set of pointers to polygons.
    \param[in] peaks  - \ru Множество указателей на апексы.
                        \en A set of pointers to apexes.
    \param[in] item   - \ru Источник сетки.
                        \en Source of mesh.
    \param[in] type   - \ru Тип сетки.
                        \en Mesh type.
    \param[in] closed - \ru Замкнутость.
                        \en Closedness.
    \param[in] exact  - \ru Объекты построены на числах double.
                        \en Objects builded on double data.
    \param[in] n      - \ru Именователь операции.
                        \en An object defining names generation in the operation. \~
    \param[in] same   - \ru Признак использования оригиналов данных.
                        \en Flag of using the original data. \~
  */
  MbSimpleMeshCreator( const RPArray<MbGrid> & grids, const RPArray<MbPolygon3D> & wires,
                       const RPArray<MbApex3D> & peaks, const MbRefItem * item,
                       MbeSpaceType type, bool closed, bool exact,
                       const MbSNameMaker & n, bool same );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по типу операции.
             \en Constructor by a type of operation. \~
    \param[in] n      - \ru Именователь операции.
                        \en An object defining names generation in the operation. \~
    \param[in] exact  - \ru Объекты построены на числах double. \en Objects builded on double data.
                        \en An object defining names generation in the operation. \~
    \param[in] op     - \ru Тип булевой операции.
                        \en A Boolean operation type. \~
    \param[in] exact  - \ru Объекты построены на числах double.
                        \en Objects builded on double data.
  */
  MbSimpleMeshCreator( const MbSNameMaker & n, bool exact = false );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по объекту и типу операции.
             \en Constructor by an object and a type of operation. \~
    \param[in] mesh   - \ru Исходный объект.
                        \en An initial object. \~
    \param[in] n      - \ru Именователь операции.
                        \en An object defining names generation in the operation. \~
    \param[in] exact  - \ru Объекты построены на числах double. \en Objects builded on double data.
                        \en An object defining names generation in the operation. \~
    \param[in] op     - \ru Тип булевой операции.
                        \en A Boolean operation type. \~
    \param[in] same   - \ru Признак использования оригиналов данных.
                        \en Flag of using the original data. \~
  */
  MbSimpleMeshCreator( const MbMesh& mesh, const MbSNameMaker & n, bool same );

private :
  MbSimpleMeshCreator( const MbSimpleMeshCreator &, MbRegDuplicate * iReg ); // \ru Конструктор копирования с регистратором \en Copy-constructor with the registrator 
public :
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbSimpleMeshCreator();

  /** \ru \name Общие функции строителя оболочки.
      \en \name Common functions of the shell creator.
      \{ */
  MbeCreatorType IsA() const override; // \ru Тип элемента \en Type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;             // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;             // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  MbePrompt   GetPropertyName() override;                         // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetProperties( MbProperties & ) override;           // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override;     // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar ( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  bool        SetEqual( const MbCreator & ) override;         // \ru Сделать равным \en Make equal 
  
  // \ru Создать полигональный объект. \en Create a polygonal object. 
  bool        CreateMesh( MbMesh *& mesh, MbeCopyMode sameShell, RPArray<MbSpaceItem> * items = nullptr ) override;

  // \ru Построить оболочку по исходным данным. \en Create a shell from the initial data.
  bool        CreateShell( MbFaceShell *&, MbeCopyMode, RPArray<MbSpaceItem> * ) override;
  // \ru Построить проволочный каркас по исходным данным. \en Create a wire-frame from the source data.
  bool        CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * ) override;
  // \ru Построить точечный каркас по исходным данным. \en Create a point - frame from the source data.
  bool        CreatePointFrame( MbPointFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * ) override;
  
  /** \} */
  size_t      GridsCount() const { return m_grids.size(); } // \ru Выдать количество триангуляций. \en Get the number of triangulations.
  void        AddGrid( MbGrid & gr );                       // \ru Добавить триангуляцию. \en Add triangulation.

  size_t      PolygonsCount() const { return m_wires.size(); }  // \ru Выдать количество полигонов. \en Get the number of polygons.
  void        AddPolygon( MbPolygon3D & );                      // \ru Добавить полигон. \en Add polygon.

  size_t      ApexesCount() const { return m_peaks.size(); }  // \ru Выдать количество апексов. \en Get the number of apexes.
  void        AddApex( MbApex3D & ap );                       // \ru Добавить новый апекс. \en Add new apex.

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSimpleMeshCreator )
OBVIOUS_PRIVATE_COPY( MbSimpleMeshCreator )
}; // MbSimpleMeshCreator

IMPL_PERSISTENT_OPS( MbSimpleMeshCreator )


//------------------------------------------------------------------------------
/** \brief \ru Строитель вывернутого "наизнанку" полигонального объекта.
           \en Constructor of a reversed polygonal object. \~
  \details \ru Строитель вывернутого "наизнанку" полигонального объекта. \n
           \en Constructor of a reversed polygonal object. \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbReverseMeshCreator : public MbCreator {

public :
  /// \ru Конструктор. \en Constructor. \~
  MbReverseMeshCreator( const MbSNameMaker & );

private :
  MbReverseMeshCreator( const MbReverseMeshCreator &, MbRegDuplicate * iReg ); // \ru Конструктор копирования с регистратором \en Copy-constructor with the registrator 
public :
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbReverseMeshCreator();

  /** \ru \name Общие функции строителя оболочки.
      \en \name Common functions of the shell creator.
      \{ */
  MbeCreatorType IsA() const override; // \ru Тип элемента \en Type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;             // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;             // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetProperties( MbProperties & ) override;       // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 

  bool        IsSame( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        SetEqual( const MbCreator & ) override;                  // \ru Сделать равным \en Make equal 
  bool        IsSimilar ( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar 

  /// \ru Создать полигональный объект. \en Create a polygonal object. 
  bool        CreateMesh( MbMesh *& mesh, MbeCopyMode sameShell, RPArray<MbSpaceItem> * items = nullptr ) override;

  // \ru Построить оболочку по исходным данным. \en Create a shell from the initial data.
  bool        CreateShell( MbFaceShell *&, MbeCopyMode, RPArray<MbSpaceItem> * ) override;
  // \ru Построить проволочный каркас по исходным данным. \en Create a wire-frame from the source data.
  bool        CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * ) override;
  // \ru Построить точечный каркас по исходным данным. \en Create a point - frame from the source data.
  bool        CreatePointFrame( MbPointFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * ) override;

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbReverseMeshCreator )
OBVIOUS_PRIVATE_COPY( MbReverseMeshCreator )
}; // MbReverseMeshCreator

IMPL_PERSISTENT_OPS( MbReverseMeshCreator )


//------------------------------------------------------------------------------
/** \brief \ru Строитель булевой операции над полигональными объектами.
           \en Constructor of Boolean operation of polygonal objects. \~
  \details \ru Строитель булевой операции над полигональными объектами. \n
           \en Constructor of Boolean operation of polygonal objects. \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbBooleanMeshCreator : public MbCreator
{
public:
  RPArray<MbCreator> m_creators;    ///< \ru Журнал построения: i < m_firstCount - строители первого объекта; i >= m_firstCount - строители второго объекта. \en History tree: i < m_firstCount - creators of the first object; i >= m_firstCount - creators of the second object.
  size_t             m_firstCount;  ///< \ru Количество строителей первого объекта. \en The number of first-object creators.
  OperationType      m_operation;   ///< \ru Тип булевой операции. \en Boolean operation type. 

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по строителям полигональных объектов.
             \en Constructor by meshes creators. \~
    \param[in] creators12     - \ru Строители объектов (индекс < m_firstCount - строители первого объекта; индекс >= m_firstCount - строители второго объекта).
                                \en Creators of objects (index < m_firstCount - creators of the first object; index >= m_firstCount - creators of the second object). 
    \param[in] firstCount     - \ru Количество строителей первого объекта.
                                \en The number of first object creators.
    \param[in] sameCreators1  - \ru Использовать строители первого объекта без копирования.
                                \en Use creators of the first object without copying.
    \param[in] sameCreators2  - \ru Использовать строители второго объекта без копирования.
                                \en Use creators of the second object without copying.
    \param[in] operation      - \ru Тип булевой операции.
                                \en Boolean operation type. 
    \param[in] n              - \ru Именователь операции.
                                \en An object defining names generation in the operation. \~
  */
  MbBooleanMeshCreator( const RPArray<MbCreator> & creators12,
                        size_t  firstCount,
                        bool sameCreators1,
                        bool sameCreators2,
                        OperationType op,
                        const MbSNameMaker & n );
private:
  MbBooleanMeshCreator( const MbBooleanMeshCreator &, MbRegDuplicate * iReg ); // \ru Конструктор копирования с регистратором \en Copy-constructor with the registrator 
public:
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbBooleanMeshCreator();

  /** \ru \name Общие функции строителя оболочки.
      \en \name Common functions of the shell creator.
      \{ */
  MbeCreatorType IsA() const override; // \ru Тип элемента \en Type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move( const MbVector3D &, MbRegTransform * = nullptr ) override;      // \ru Сдвиг \en Translation
  void        Rotate( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override;  // \ru Повернуть вокруг оси \en Rotate about an axis

  MbePrompt   GetPropertyName() override;                         // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetProperties( MbProperties & ) override;           // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override;     // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  void        GetBasisItems( RPArray<MbSpaceItem> & ) override;   // \ru Дать базовые объекты. \en Get the basis objects.

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  bool        SetEqual( const MbCreator & ) override;         // \ru Сделать равным \en Make equal 

  // \ru Создать полигональный объект. \en Create a polygonal object. 
  bool        CreateMesh( MbMesh *& mesh, MbeCopyMode sameShell, RPArray<MbSpaceItem> * items = nullptr ) override;

  // \ru Построить оболочку по исходным данным. \en Create a shell from the initial data.
  bool        CreateShell( MbFaceShell *&, MbeCopyMode, RPArray<MbSpaceItem> * ) override;
  // \ru Построить проволочный каркас по исходным данным. \en Create a wire-frame from the source data.
  bool        CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * ) override;
  // \ru Построить точечный каркас по исходным данным. \en Create a point - frame from the source data.
  bool        CreatePointFrame( MbPointFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * ) override;


  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbBooleanMeshCreator )
  OBVIOUS_PRIVATE_COPY( MbBooleanMeshCreator )
};

IMPL_PERSISTENT_OPS( MbBooleanMeshCreator )


//------------------------------------------------------------------------------
/** \brief \ru Строитель полигонального объекта, разрезанного плоскостью.
           \en Constructor of a polygonal object cutted by a plane. \~
  \details \ru Строитель полигонального объекта, разрезанного плоскостью. \n
           \en Constructor of a polygonal object cutted by a plane. \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbCutMeshCreator : public MbCreator
{
public:
  MbPlacement3D m_place;        // \ru Секущая плоскость. \en A cutting plane.
  double        m_distance;     // \ru Расстояние до второй режущей плоскости. \en Distance to the second cutting plane.
  bool          m_onlySection;  // \ru Режим отсечения. \en Cutting mode.

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по данным.
             \en Constructor by data. \~
    \param[in] place      -     \ru Секущая плоскость.
                                \en Cutting plane.
    \param[in] m_distance -     \ru Расстояние до второй режущей плоскости.
                                \en Distance to the second cutting plane.
    \param[in] m_onlySection  - \ru Режим отсечения.
                                \en Cutting mode. \~
  */
  MbCutMeshCreator( const MbPlacement3D & place, double distance, bool onlySection,
                    const MbSNameMaker & n );
private:
  MbCutMeshCreator( const MbCutMeshCreator &, MbRegDuplicate * iReg ); // \ru Конструктор копирования с регистратором \en Copy-constructor with the registrator 
public:
  // \ru Деструктор. \en Destructor. 
  virtual ~MbCutMeshCreator();

  /** \ru \name Общие функции строителя оболочки.
    \en \name Common functions of the shell creator.
  \{ */
  MbeCreatorType IsA() const override; // \ru Тип элемента \en Type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;             // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move( const MbVector3D &, MbRegTransform * = nullptr ) override;             // \ru Сдвиг \en Translation
  void        Rotate( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  MbePrompt   GetPropertyName() override;                         // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetProperties( MbProperties & ) override;           // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override;     // \ru Записать свойства объекта \en Set properties of the object 

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  bool        SetEqual( const MbCreator & ) override;         // \ru Сделать равным \en Make equal 

  // \ru Создать полигональный объект. \en Create a polygonal object. 
  bool        CreateMesh( MbMesh *& mesh, MbeCopyMode sameShell, RPArray<MbSpaceItem> * items = nullptr ) override;

  // \ru Построить оболочку по исходным данным. \en Create a shell from the initial data.
  bool        CreateShell( MbFaceShell *&, MbeCopyMode, RPArray<MbSpaceItem> * ) override;
  // \ru Построить проволочный каркас по исходным данным. \en Create a wire-frame from the source data.
  bool        CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * ) override;
  // \ru Построить точечный каркас по исходным данным. \en Create a point - frame from the source data.
  bool        CreatePointFrame( MbPointFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * ) override;


  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCutMeshCreator )
  OBVIOUS_PRIVATE_COPY( MbCutMeshCreator )
};

IMPL_PERSISTENT_OPS( MbCutMeshCreator )

#endif // __CR_MESH_CREATOR_H
