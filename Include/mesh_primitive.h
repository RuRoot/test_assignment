////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Базовый класс для структур данных сетки (#MbMesh). Вершина. Полигон.
         \en Base class for mesh data structures (#MbMesh). Vertex. Polygon. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MESH_PRIMITIVE_H
#define __MESH_PRIMITIVE_H


#include <mb_cart_point3d.h>
#include <templ_rp_array.h>
#include <templ_dptr.h>
#include <mesh_triangle.h>
#include <attribute_container.h>
#include <mb_cube.h>
#include <mb_data.h>
#include <model_item.h>


class   MbRect;
class   MbCurve;
class   MbPolyline3D;
class   MbPlacement3D;
class   MbPrimitive;
class   MbPolygon3D;
class   MbPolygon;
class   MbFloatAxis3D;
class   MbFloatPoint3D;
class   MbGrid;
struct            MbGridSearchTree;


namespace c3d // namespace C3D
{
typedef SPtr<MbPrimitive>                           PrimitiveSPtr;
typedef SPtr<const MbPrimitive>                     ConstPrimitiveSPtr;

typedef SPtr<MbGrid>                                GridSPtr;
typedef SPtr<const MbGrid>                          ConstGridSPtr;

typedef std::vector<MbPrimitive *>                  PrimitivesVector;
typedef std::vector<const MbPrimitive *>            ConstPrimitivesVector;

typedef std::vector<PrimitiveSPtr>                  PrimitivesSPtrVector;
typedef std::vector<ConstPrimitiveSPtr>             ConstPrimitivesSPtrVector;

typedef std::set<MbPrimitive *>                     PrimitivesSet;
typedef PrimitivesSet::iterator                     PrimitivesSetIt;
typedef PrimitivesSet::const_iterator               PrimitivesSetConstIt;
typedef std::pair<PrimitivesSetConstIt, bool>       PrimitivesSetRet;

typedef std::set<const MbPrimitive *>               ConstPrimitivesSet;
typedef ConstPrimitivesSet::iterator                ConstPrimitivesSetIt;
typedef ConstPrimitivesSet::const_iterator          ConstPrimitivesSetConstIt;
typedef std::pair<ConstPrimitivesSetConstIt, bool>  ConstPrimitivesSetRet;

typedef std::vector<MbGrid *>                       GridsVector;
typedef std::vector<const MbGrid *>                 ConstGridsVector;

typedef std::vector<GridSPtr>                       GridsSPtrVector;
typedef std::vector<ConstGridSPtr>                  ConstGridsSPtrVector;
}


//------------------------------------------------------------------------------
/** \brief \ru Типы примитивов.
           \en Types of primitives. \~
  \details \ru Типы примитивов полигонального объекта.
           \en Get type of primitive of polygonal object. \~ 
  \ingroup Polygonal_Objects
*/
// ---
enum MbePrimitiveType {
  pt_Apex3D,           ///< \ru Апекс. \en Apex. 
  pt_ExactApex3D,      ///< \ru Апекс на числах double. \en Apex on double data. 
  pt_FloatApex3D,      ///< \ru Апекс на числах float. \en Apex on float data. 
  pt_Polygon3D,        ///< \ru Полигон. \en Polygon. 
  pt_ExactPolygon3D,   ///< \ru Полигон на числах double. \en Polygon on double data. 
  pt_FloatPolygon3D,   ///< \ru Полигон на числах float. \en Polygon on float data. 
  pt_Grid,             ///< \ru Триангуляция. \en Triangulation. 
  pt_ExactGrid,        ///< \ru Триангуляция на числах double. \en Triangulation on double data. 
  pt_FloatGrid,        ///< \ru Триангуляция на числах float.  \en Triangulation on float data. 
};


////////////////////////////////////////////////////////////////////////////////
/** \brief \ru Примитив.
           \en Primitive. \~
  \details \ru Родительский класс элемента полигонального объекта служит для 
  аппроксимации геометрического объекта. Каждый экземпляр класса MbPrimitive несет имя и 
  указатель на исходный геометрический объект (если есть такой) и атрибуты.\n
  MbPrimitive является предком для триангуляции (#MbGrid), полигона (#MbPolygon3D) и апекса (#MbApex3D).
           \en Parent class for polygonal elements is used to represent the approximated geometric object. 
  Each instance of MbPrimitive class has name and pointer to the source geometric object (if it exists) and attributes.\n
  MbPrimitive is ancestor for triangulation (#MbGrid), polygon (#MbPolygon3D), and apex (#MbApex3D). \~
  \ingroup Polygonal_Objects
*/ 
////////////////////////////////////////////////////////////////////////////////
class MATH_CLASS MbPrimitive : public MbAttributeContainer, public MbRefItem, public MbNestSyncItem {
protected:
  MbName            name;       ///< \ru Имя примитива. \en Name of primitive.
  const MbRefItem * parentItem; ///< \ru Породивший объект (не владеем). \en Begetter object (don't own). 
  MbeRefType        type;       ///< \ru Тип примитива. \en Type of primitive.

protected:
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en The copy constructor without implementation prevents from copying by default. 
  MbPrimitive( const MbPrimitive & );
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbPrimitive( const MbPrimitive &, MbRegDuplicate * iReg );
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  MbPrimitive();
public:
  /// \ru Деструктор \en Destructor 
  virtual ~MbPrimitive();

public:

  /** \ru \name Общие функции примитива.
      \en \name Common functions of primitive.
  \{ */  
  /// \ru Получить тип объекта. \en Get the object type. 
  virtual MbePrimitiveType Type() const = 0;
  virtual MbePrimitiveType IsA() const = 0; // \ru Тип объекта. \en A type of an object. 
  /// \ru Регистрационный тип (для копирования, дублирования). \en Registration type (for copying, duplication). 
  MbeRefType  RefType() const override; 

  /** \brief \ru Создать копию примитива.
             \en Create a copy of primitive. \~
  \details \ru Создать копию примитива с использованием регистратора. 
    Регистратор используется для предотвращения многократного копирования примитива, 
    входящего в состав нескольких объектов копируемых одновременно.
    При копировании одиночного объекта или набора не связанных между собой объектов допустимо не использовать регистратор.
    Регистратор необходимо использовать, если надо последовательно копировать несколько взаимосвязанных объектов.
    Взаимосвязь представляет собой наличие в объектах ссылок на общие примитивы. 
    Тогда, при копировании без использования регистратора, можно получить набор копий, 
    содержащих ссылки на разные копии одного и того же вложенного примитива, что ведет к потере связи между копиями.
           \en Create a copy of primitive using the registrator. 
    Registrator is used for preventing multiple copying of primitive 
    contained in several simultaneously copied objects.
    It is allowed not to use the registrator while copying a single object or a set of disconnected objects.
    The registrator must be used to copy several correlated objects successively.
    Correlation is an existence of references to common primitives in objects. 
    Then, while copying without using the registrator, one can get a set of copies 
    which contains references to the different copies of a single included primitive, what leads to loss of connection between the copies. \~
  \param[in] iReg - \ru Регистратор.
                    \en Registrator. \~
  \return \ru Копия объекта.
          \en The object copy. \~
  */
  virtual MbPrimitive & Duplicate( MbRegDuplicate * iReg = nullptr ) const { return *Clone(iReg).detach(); };
  
  /** \brief \ru Создать копию примитива.
             \en Create a copy of primitive. \~
  \details \ru Создать копию примитива с использованием регистратора.
    Регистратор используется для предотвращения многократного копирования примитива,
    входящего в состав нескольких объектов копируемых одновременно.
    При копировании одиночного объекта или набора не связанных между собой объектов допустимо не использовать регистратор.
    Регистратор необходимо использовать, если надо последовательно копировать несколько взаимосвязанных объектов.
    Взаимосвязь представляет собой наличие в объектах ссылок на общие примитивы.
    Тогда, при копировании без использования регистратора, можно получить набор копий,
    содержащих ссылки на разные копии одного и того же вложенного примитива, что ведет к потере связи между копиями.
           \en Create a copy of primitive using the registrator.
    Registrator is used for preventing multiple copying of primitive
    contained in several simultaneously copied objects.
    It is allowed not to use the registrator while copying a single object or a set of disconnected objects.
    The registrator must be used to copy several correlated objects successively.
    Correlation is an existence of references to common primitives in objects.
    Then, while copying without using the registrator, one can get a set of copies
    which contains references to the different copies of a single included primitive, what leads to loss of connection between the copies. \~
  \param[in] iReg - \ru Регистратор.
                    \en Registrator. \~
  \return \ru Копия объекта.
          \en The object copy. \~
  */
  virtual SPtr<MbPrimitive> Clone( MbRegDuplicate * iReg = nullptr ) const = 0;

  /** \brief \ru Преобразовать примитив согласно матрице.
             \en Transform primitive according to the matrix. \~
  \details \ru Преобразовать примитив согласно матрице c использованием регистратора. 
    Регистратор служит для предотвращения многократного преобразования примитива,
    входящего в состав нескольких объектов, трансформируемых одновременно.
    Допустимо использовать функцию без регистратора, если надо преобразовать одиночный объект.
    Если надо преобразовать набор взаимосвязанных объектов, необходимо использовать регистратор для 
    предотвращения повторного преобразования вложенных примитивов, поскольку не исключена ситуация,
    когда несколько объектов из набора содержат ссылки на один или несколько общих примитивов, подлежащих трансформации.
           \en Transform primitive according to the matrix using the registrator. 
    The registrator is used for preventing multiple transformation of primitive
    contained in several simultaneously transformed objects.
    The function can be used without the registrator to transform a single object.
    The registrator must be used to transform a set of interdependent objects to 
    prevent repeated transformation of the nested primitives, since it is not ruled out
    that several objects from the set contain references to one or several common primitives subject to transformation. \~
  \param[in] matr - \ru Матрица преобразования.
                    \en A transformation matrix. \~
  */
  virtual void    Transform( const MbMatrix3D & matr ) = 0;

  /** \brief \ru Сдвинуть примитив вдоль вектора.
             \en Move primitive along a vector. \~
  \details \ru Сдвинуть примитив вдоль вектора с использованием регистратора. 
    Регистратор служит для предотвращения многократного преобразования примитива,
    входящего в состав нескольких объектов, трансформируемых одновременно.
    Допустимо использовать функцию без регистратора, если надо преобразовать одиночный объект.
    Если надо преобразовать набор взаимосвязанных объектов, необходимо использовать регистратор для 
    предотвращения повторного преобразования вложенных примитивов, поскольку не исключена ситуация,
    когда несколько объектов из набора содержат ссылки на один или несколько общих примитивов, подлежащих сдвигу.
           \en Move primitive along the vector using the registrator. 
    The registrator is used for preventing multiple transformation of primitive
    contained in several simultaneously transformed objects.
    The function can be used without the registrator to transform a single object.
    The registrator must be used to transform a set of interdependent objects to 
    prevent repeated transformation of the nested primitives, since it is not ruled out
    that several objects from the set contain references to one or several common primitives subject to moving. \~
  \param[in] to   - \ru Вектор сдвига.
                    \en Movement vector. \~
  */
  virtual void    Move( const MbVector3D & to ) = 0;

  /** \brief \ru Повернуть примитив вокруг оси на заданный угол.
             \en Rotate primitive about an axis by a given angle. \~
  \details \ru Повернуть примитив вокруг оси на заданный угол с использованием регистратора. 
    Регистратор служит для предотвращения многократного преобразования примитива,
    входящего в состав нескольких объектов, трансформируемых одновременно.
    Допустимо использовать функцию без регистратора, если надо преобразовать одиночный объект.
    Если надо преобразовать набор взаимосвязанных объектов, необходимо использовать регистратор для 
    предотвращения повторного преобразования вложенных примитивов, поскольку не исключена ситуация,
    когда несколько объектов из набора содержат ссылки на один или несколько общих примитивов, подлежащих повороту.
           \en Rotate primitive about an axis by a given angle using the registrator. 
    The registrator is used for preventing multiple transformation of primitive
    contained in several simultaneously transformed objects.
    The function can be used without the registrator to transform a single object.
    The registrator must be used to transform a set of interdependent objects to 
    prevent repeated transformation of the nested primitives, since it is not ruled out
    that several objects from the set contain references to one or several common primitives subject to rotation. \~
  \param[in] axis - \ru Ось поворота.
                    \en The rotation axis. \~
  \param[in] angle - \ru Угол поворота.
                     \en The rotation angle. \~
  */
  virtual void    Rotate( const MbAxis3D & axis, double angle ) = 0;

  // \ru Тип контейнера атрибутов. \en Type of attribute container. 
  MbeImplicationType ImplicationType() const override; 
  /// \ru Расширить присланный габаритный куб так, чтобы он включал в себя данный объект. \en Extend given bounding box so that it encloses the given object. 
  virtual void    AddYourGabaritTo( MbCube & r ) const = 0; 
  /// \ru Вычислить расстояние до точки. \en Calculate distance to point.  
  virtual double  DistanceToPoint( const MbCartPoint3D & pnt ) const = 0; 
  /// \ru Вычислить расстояние до оси. \en Calculate the distance to the axis.  
  virtual double  DistanceToLine( const MbAxis3D & axis, double extDistance, double & t ) const = 0; 
  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  virtual void    GetProperties( MbProperties & ) = 0;
  /// \ru Записать свойства объекта. \en Set properties of the object. 
  virtual void    SetProperties( const MbProperties & ) = 0;

          // \ru Выдать имя примитива. \en Get name of primitive.
          SimpleName    GetPrimitiveName() const { return name.GetMainName(); }
          // \ru Установить имя примитива. \en Set name of primitive. 
          void          SetPrimitiveName( SimpleName n ) { name.SetMainName( n ); }
          /// \ru Выдать имя объекта. \en Get name of object. 
  const   MbName &      GetName()     const { return name; }
          /// \ru Выдать имя объекта для модификации. \en Get name of object for modification. 
          MbName &      SetName() { return name; }
          /// \ru Получить первое имя. \en Get first name. 
          SimpleName    GetFirstName() const { return (name.CountBase() > (size_t)MbName::i_First) ? name.GetFirstNameDirect() : 0; }
          /// \ru Выдать hash имени. \en Get hash of name. 
          SimpleName    GetNameHash() const { return name.Hash(); }
          /// \ru Установить имя. \en Set name. 
          void          SetName( const MbName & n ) { name.SetName( n ); }

          /// \ru Выдать породивший примитив объект. \en Get begetter object of primitive. 
  const   MbRefItem *   GetItem() const { return parentItem; }
          /// \ru Установить породивший примитив объект. \en Set begetter object of primitive. 
          void          SetItem( const MbRefItem * g ) { parentItem = g; }
          /// \ru Дать тип объекта. \en Get type of object. 
          MbeRefType    GetPrimitiveType() const { return type; } 
          /// \ru Установить тип объекта. \en Set type of object. 
          void          SetPrimitiveType( MbeRefType t ) { type = t; } 

          /// \ru Удовлетворяет ли примитив критериям поиска ближайшего объекта? \en Does the primitive satisfy the nearest object search criteria? 
          bool          NearestType( MbeSpaceType sType, MbeTopologyType tType, MbePlaneType pType ) const;

          /// \ru Получить пространственный объект, для которого построен примитив. \en Get spatial object for which the primitive is constructed. 
  const   MbSpaceItem * SpaceItem() const { return ((parentItem != nullptr && parentItem->RefType() == rt_SpaceItem) ? (const MbSpaceItem *)parentItem : nullptr); }
          /// \ru Получить двумерный объект, для которого построен примитив. \en Get two-dimensional object for which the primitive is constructed. 
  const   MbPlaneItem * PlaneItem() const { return ((parentItem != nullptr && parentItem->RefType() == rt_PlaneItem) ? (const MbPlaneItem *)parentItem : nullptr); }
          /// \ru Получить топологический объект, для которого построен примитив. \en Get the topological object for which the primitive is constructed. 
  const   MbTopItem *   TopItem()   const { return ((parentItem != nullptr && parentItem->RefType() == rt_TopItem) ? (const MbTopItem *)parentItem : nullptr); }
          /// \ru Получить объект геометрической модели, для которого построен примитив. \en Get geometric model object for which the primitive is constructed. 
  const   MbItem *      Item()      const { return ((parentItem != nullptr && parentItem->RefType() == rt_SpaceItem) ? (static_cast<const MbItem *>(parentItem)) : nullptr); }

          /// \ru Чтение примитива из потока. \en Reading of primitive from the stream.  
          void    PrimitiveRead ( reader & ); 
          /// \ru Запись примитива в поток. \en Writing of primitive to the stream.  
          void    PrimitiveWrite( writer & ) const; 
  /** \} */
private:
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию \en Declaration without implementation of the assignment operator to prevent an assignment by default 
        void      operator = ( const MbPrimitive & init );
}; // MbPrimitive


////////////////////////////////////////////////////////////////////////////////
/** \brief \ru Апекс (точка).
           \en Apex (point). \~
  \details \ru Апекс определяет положение точки, вершины или другого точечного объекта в пространстве.\n
           \en Apex defines position of a point, of a vertex or the other point-object in space.\n \~ 
  \ingroup Polygonal_Objects
*/ 
////////////////////////////////////////////////////////////////////////////////
class MATH_CLASS MbApex3D : public MbPrimitive {

protected:
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию \en Declaration without implementation of the copy-constructor to prevent a copying by default 
  MbApex3D( const MbApex3D & ); 
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbApex3D( const MbApex3D &, MbRegDuplicate * );
public :
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  MbApex3D() : MbPrimitive() {}
  /// \ru Деструктор \en Destructor 
  virtual ~MbApex3D();

public:

  // \ru Общие функции примитива. \en Common functions of the primitive. 
  MbePrimitiveType Type() const override;    // \ru Тип объекта. \en A type of an object.
  //virtual MbePrimitiveType IsA() const = 0; // \ru Тип объекта. \en A type of an object. 
  //virtual MbPrimitive & Duplicate( MbRegDuplicate * iReg = nullptr ) const = 0; // \ru Создать копию объекта. \en Create a copy of the object.
  //virtual void    Transform( const MbMatrix3D & ) = 0;                   // \ru Преобразовать согласно матрице. \en Transform according to the matrix.
  //virtual void    Move     ( const MbVector3D & ) = 0;                   // \ru Сдвинуть вдоль вектора. \en Translate along a vector.
  //virtual void    Rotate   ( const MbAxis3D &, double angle ) = 0;       // \ru Повернуть вокруг оси на угол. \en Rotate about an axis by an angle.
  void    AddYourGabaritTo( MbCube & ) const override;                    // \ru Расширить присланный габаритный куб так, чтобы он включал в себя данный объект. \en Extend given bounding box so that it encloses the given object.  
  double  DistanceToPoint( const MbCartPoint3D & ) const override;        // \ru Вычислить расстояние до точки. \en Calculate distance to the point.  
  double  DistanceToLine( const MbAxis3D &, double extDistance, double & t ) const override;  // \ru Вычислить расстояние до оси. \en Calculate the distance to the axis.  
  //virtual void    GetProperties( MbProperties & ) = 0;                   // \ru Выдать свойства объекта. \en Get properties of the object.
  //virtual void    SetProperties( const MbProperties & ) = 0;             // \ru Записать свойства объекта. \en Set properties of the object.

  /** \ru \name Функции апекса.
      \en \name Functions of the apex.
  \{ */  
  /// \ru Инициализировать апекс по точке. \en Initialize apex by a point.  
  virtual void    Init( const MbCartPoint3D  & ) = 0;
  /// \ru Инициализировать апекс по точке. \en Initialize apex by a point.  
  virtual void    Init( const MbFloatPoint3D & ) = 0;
  /// \ru Инициализировать точку по апексу. \en Initialize point by an apex.  
  virtual void    GetPoint( MbCartPoint3D  & ) const = 0;
  /// \ru Инициализировать точку по апексу. \en Initialize point by an apex.  
  virtual void    GetPoint( MbFloatPoint3D & ) const = 0;
  /// \ru Являются ли объекты равными? \en Determine whether objects are equal. 
  virtual bool    IsSame( const MbApex3D &, double eps ) const = 0;
  /// \ru Инициировать по другому объекту. \en Init by other apex. 
  virtual void    Init( const MbApex3D & ) = 0;

  /** \} */
private:
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation, to prevent an assignment by default. 
          void    operator = ( const MbApex3D & init );

}; // MbApex3D


////////////////////////////////////////////////////////////////////////////////
/** \brief \ru Апекс на числах double.
           \en Apex on double data. \~
  \details \ru Апекс определяет положение точки, вершины или другого точечного объекта в пространстве.\n
           \en Apex defines position of a point, of a vertex or the other point-object in space.\n \~ 
  \ingroup Polygonal_Objects
*/ 
////////////////////////////////////////////////////////////////////////////////
class MATH_CLASS MbExactApex3D : public MbApex3D {
private :
  MbCartPoint3D vertex; ///< \ru Положение апекса. \en Position of the apex. 

protected:
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию \en Declaration without implementation of the copy-constructor to prevent a copying by default 
  MbExactApex3D( const MbExactApex3D & ); 
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbExactApex3D( const MbExactApex3D &, MbRegDuplicate * );
public :
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  MbExactApex3D() : MbApex3D(), vertex() {}
  /// \ru Конструктор по точке. \en Constructor by point. 
  MbExactApex3D( const MbFloatPoint3D & vert ) : MbApex3D(), vertex( vert.x, vert.y, vert.z ) {}
  /// \ru Конструктор по точке. \en Constructor by point. 
  MbExactApex3D( const MbCartPoint3D  & vert ) : MbApex3D(), vertex( vert ) {}
  /// \ru Деструктор \en Destructor 
  virtual ~MbExactApex3D();

public:

  // \ru Общие функции примитива. \en Common functions of the primitive. 
  MbePrimitiveType IsA() const override; // \ru Тип объекта. \en A type of an object.
  SPtr<MbPrimitive> Clone( MbRegDuplicate * iReg = nullptr ) const override; // \ru Создать копию объекта \en Create a copy of the object
  void    Transform( const MbMatrix3D & ) override;                        // \ru Преобразовать согласно матрице. \en Transform according to the matrix. 
  void    Move     ( const MbVector3D & ) override;                        // \ru Сдвинуть вдоль вектора. \en Translate along a vector. 
  void    Rotate   ( const MbAxis3D &, double angle ) override;            // \ru Повернуть вокруг оси на угол. \en Rotate about an axis by an angle. 
  void    GetProperties( MbProperties & ) override;                        // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & ) override;                  // \ru Записать свойства объекта. \en Set properties of the object. 

  /** \ru \name Функции апекса.
      \en \name Functions of the apex.
  \{ */  
  /// \ru Инициализировать апекс по точке. \en Initialize apex by a point.  
  void    Init( const MbCartPoint3D  & ) override;
  /// \ru Инициализировать апекс по точке. \en Initialize apex by a point.  
  void    Init( const MbFloatPoint3D & ) override;
  /// \ru Инициализировать точку по апексу. \en Initialize point by an apex.  
  void    GetPoint( MbCartPoint3D & p ) const override { p = vertex; }
  /// \ru Инициализировать точку по апексу. \en Initialize point by an apex.  
  void    GetPoint( MbFloatPoint3D & p ) const override { p.Init( vertex.x, vertex.y, vertex.z );}
  /// \ru Являются ли объекты равными? \en Determine whether objects are equal. 
  bool    IsSame( const MbApex3D &, double eps ) const override;
  /// \ru Инициировать по другому объекту. \en Init by other apex. 
  void    Init( const MbApex3D & ) override;

  /** \} */
private:
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation, to prevent an assignment by default. 
          void    operator = ( const MbExactApex3D & );

  DECLARE_NEW_DELETE_CLASS( MbExactApex3D )
  DECLARE_NEW_DELETE_CLASS_EX( MbExactApex3D )
  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbExactApex3D, MATH_FUNC_EX );
  KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE( MbExactApex3D, MATH_FUNC_EX );
}; // MbExactApex3D


////////////////////////////////////////////////////////////////////////////////
/** \brief \ru Апекс на числах float.
           \en Apex on float data. \~
  \details \ru Апекс определяет положение точки, вершины или другого точечного объекта в пространстве.\n
           \en Apex defines position of a point, of a vertex or the other point-object in space.\n \~ 
  \ingroup Polygonal_Objects
*/ 
////////////////////////////////////////////////////////////////////////////////
class MATH_CLASS MbFloatApex3D : public MbApex3D {
private :
  MbFloatPoint3D vertex; ///< \ru Положение апекса. \en Position of the apex. 

protected:
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию \en Declaration without implementation of the copy-constructor to prevent a copying by default 
  MbFloatApex3D( const MbFloatApex3D & ); 
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbFloatApex3D( const MbFloatApex3D &, MbRegDuplicate * );
public :
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  MbFloatApex3D() : MbApex3D(), vertex() {}
  /// \ru Конструктор по точке. \en Constructor by point. 
  MbFloatApex3D( const MbFloatPoint3D & vert ) : MbApex3D(), vertex( vert ) {}
  /// \ru Конструктор по точке. \en Constructor by point. 
  MbFloatApex3D( const MbCartPoint3D  & vert ) : MbApex3D(), vertex( vert.x, vert.y, vert.z ) {}
  /// \ru Деструктор \en Destructor 
  virtual ~MbFloatApex3D();

public:

  // \ru Общие функции примитива. \en Common functions of the primitive. 
  MbePrimitiveType IsA() const override; // \ru Тип объекта. \en A type of an object.
  SPtr<MbPrimitive> Clone( MbRegDuplicate * iReg = nullptr ) const override; // \ru Создать копию объекта \en Create a copy of the object
  void    Transform( const MbMatrix3D & ) override;                        // \ru Преобразовать согласно матрице. \en Transform according to the matrix. 
  void    Move     ( const MbVector3D & ) override;                        // \ru Сдвинуть вдоль вектора. \en Translate along a vector. 
  void    Rotate   ( const MbAxis3D &, double angle ) override;            // \ru Повернуть вокруг оси на угол. \en Rotate about an axis by an angle. 
  void    GetProperties( MbProperties & ) override;                        // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & ) override;                  // \ru Записать свойства объекта. \en Set properties of the object. 

  /** \ru \name Функции апекса.
      \en \name Functions of the apex.
  \{ */  
  /// \ru Инициализировать апекс по точке. \en Initialize apex by a point.  
  void    Init( const MbCartPoint3D  & ) override;
  /// \ru Инициализировать апекс по точке. \en Initialize apex by a point.  
  void    Init( const MbFloatPoint3D & ) override;
  /// \ru Инициализировать точку по апексу. \en Initialize point by an apex.  
  void    GetPoint( MbCartPoint3D & p ) const override { p.Init( vertex.x, vertex.y, vertex.z ); }
  /// \ru Инициализировать точку по апексу. \en Initialize point by an apex.  
  void    GetPoint( MbFloatPoint3D & p ) const override { p = vertex; }
  /// \ru Являются ли объекты равными? \en Determine whether objects are equal. 
  bool    IsSame( const MbApex3D &, double eps ) const override;
  /// \ru Инициировать по другому объекту. \en Init by other apex. 
  void    Init( const MbApex3D & ) override;

  /** \} */
private:
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation, to prevent an assignment by default. 
          void    operator = ( const MbFloatApex3D & );

  DECLARE_NEW_DELETE_CLASS( MbFloatApex3D )
  DECLARE_NEW_DELETE_CLASS_EX( MbFloatApex3D )
  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbFloatApex3D, MATH_FUNC_EX );
  KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE( MbFloatApex3D, MATH_FUNC_EX );
}; // MbFloatApex3D


////////////////////////////////////////////////////////////////////////////////
/** \brief \ru Полигон.
           \en Polygon. \~
  \details \ru Полигон представляет собой упорядоченное множество точек в пространстве, 
    последовательное соединение которых даёт ломаную линию, аппроксимирующую некоторый объект или часть объекта. \n
           \en Polygon is an ordered set of points in space, 
    sequential connection of these points produces polyline that approximates an object or part of an object. \n \~ 
  \ingroup Polygonal_Objects
*/
////////////////////////////////////////////////////////////////////////////////
class MATH_CLASS MbPolygon3D : public MbPrimitive {
protected :
  MbStepData     stepData; ///< \ru Параметры расчета полигона (стрелка прогиба или угол отклонения). \en Parameters of polygon calculation (sag or angle of deviation). 
  /** \brief \ru Габаритный куб объекта (не записывается в поток и не читается). 
             \en Bounding box of a polygon (not read from stream and not written to stream). \~
      \details \ru Габаритный куб объекта рассчитывается только при запросе габарита объекта. Габаритный куб в конструкторе объекта и после модификации объекта принимает неопределенное значение.
               \en Bounding box of object is calculated only at the request. Bounding box of object is undefined after object constructor and after object modifications \n \~
  */
  mutable MbCube cube;     ///< \ru Габаритный куб полигона (не записывается в поток и не читается). \en . 

protected :
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию \en Declaration without implementation of the copy-constructor to prevent copying by default 
  MbPolygon3D( const MbPolygon3D & );
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbPolygon3D( const MbPolygon3D &, MbRegDuplicate * ); 
public :
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  MbPolygon3D();
  /// \ru Деструктор \en Destructor 
  virtual ~MbPolygon3D();

public:

  // \ru Общие функции примитива. \en Common functions of the primitive. 
  MbePrimitiveType Type() const override; // \ru Вернуть тип объекта \en Get the object type.
  //virtual MbePrimitiveType IsA() const = 0; // \ru Вернуть тип объекта \en Get the object type.
  //virtual MbPrimitive & Duplicate( MbRegDuplicate * iReg = nullptr ) const = 0; // \ru Создать копию объекта \en Create a copy of the objec
  //virtual void    Transform( const MbMatrix3D & ) = 0;                // \ru Преобразовать полигон согласно матрице \en Transform polygon according to the matrix
  //virtual void    Move     ( const MbVector3D & ) = 0;                // \ru Сдвиг полигона \en Translation of the polygon.
  //virtual void    Rotate   ( const MbAxis3D &, double angle ) = 0;    // \ru Поворот полигона вокруг оси \en Rotation of the polygon around an axis
  //virtual void    AddYourGabaritTo( MbCube & ) const = 0;             // \ru Расширить присланный габаритный куб так, чтобы он включал в себя данный объект. \en Extend given bounding box so that it encloses the given object.
  double  DistanceToPoint( const MbCartPoint3D & ) const override; // \ru Вычислить расстояние до точки. \en Calculate distance to the point.
  double  DistanceToLine( const MbAxis3D &, double extDistance, double & t ) const override; // \ru Вычислить расстояние до оси. \en Calculate the distance to the axis.
  //virtual void    GetProperties( MbProperties & ) = 0;                // \ru Выдать свойства объекта. \en Get properties of the object.
  //virtual void    SetProperties( const MbProperties & ) = 0;          // \ru Записать свойства объекта. \en Set properties of the object.

  /** \ru \name Функции полигона.
      \en \name Functions of polygon.
  \{ */  
  /// \ru Выдать размер занимаемой памяти. \en Get the size of taken memory. 
  virtual size_t  SizeOf() const = 0;
  /// \ru Зарезервировать место для полигона. \en Reserve memory for polygon. 
  virtual void    Reserve( size_t cnt ) = 0;
  /// \ru Удалить лишнюю память. \en Free the unnecessary memory. 
  virtual void    Adjust() = 0;
  /// \ru Очистить полигон удалив все точки. \en Clear the polygon by deleting all the points. 
  virtual void    Flush() = 0;
  /// \ru Выдать количество точек. \en Get count of points. 
  virtual size_t  Count() const = 0;
  /// \ru Добавить точку в конец полигона. \en Add point to the end of the polygon. 
  virtual void    AddPoint( const MbCartPoint3D  & dpnt ) = 0;
  /// \ru Добавить точку в конец полигона. \en Add point to the end of the polygon. 
  virtual void    AddPoint( const MbFloatPoint3D & fpnt ) = 0;

  /// \ru Выдать точку по её номеру. \en Get point by its index. 
  virtual void    GetPoint( size_t i, MbCartPoint3D  & ) const = 0;
  /// \ru Выдать точку по её номеру. \en Get point by its index. 
  virtual void    GetPoint( size_t i, MbFloatPoint3D & ) const = 0;

  /// \ru Установить точку с номером. \en Set point by index. 
  virtual void    SetPoint( size_t i, MbCartPoint3D & ) = 0;

  /**\ru Проверить, лежат ли точки полигона в одной плоскости c заданной точностью metricAccuracy.
      Если да, то инициализировать плоскость plane.
      \en Check whether all points of polygon lie on the same plane with the given metricAccuracy accuracy.
      if so, then initialize 'plane' plane. \~
  */
  virtual bool    IsPlanar( MbPlacement3D & plane, double metricAccuracy = Math::metricRegion ) const = 0;
  /// \ru Если точки полигона лежат в одной плоскости, то инициализировать plane и заполнить полигон poly. \en If points of polygon lie on the same plane, then initialize 'plane' and fill the 'poly' polygon. 
  virtual bool    GetPlanePolygon( MbPlacement3D & plane, MbPolygon & poly ) const = 0;
  /// \ru Проверить наличие точек в объекте. \en Check existence of points in object. 
  virtual bool    IsComplete() const = 0;

          /// \ru Выдать параметр расчета триангуляции (стрелку прогиба или угол отклонения). \en Get the parameter of triangulation calculation. 
          const   MbStepData &  GetStepData() const { return stepData; }
          /// \ru Установить параметр расчета триангуляции (стрелку прогиба или угол отклонения). \en Set the parameter of triangulation calculation. 
          void    SetStepData( const MbStepData & stData ) { stepData = stData; }
          /// \ru Установить параметр расчета полигона (стрелку прогиба или угол отклонения). \en Set the parameter of polygon calculation (sag or angle of deviation). 
          void    SetStepBySag( double s ) { stepData.InitStepBySag( s ); }

  /// \ru Добавить к полигону полигон с удалением совпадающих точек стыка. \en Add a polygon to the polygon with removing the coincident points of joint. 
  virtual void    AddPolygon( const MbPolygon3D & ) = 0;
  /// \ru Добавить к полигону полигон с удалением совпадающих точек стыка. \en Add a polygon to the polygon with removing the coincident points of joint. 
  virtual void    operator += ( const MbPolygon3D & ) = 0;
  /// \ru Являются ли объекты равными? \en Determine whether objects are equal. 
  virtual bool    IsSame( const MbPolygon3D &, double eps ) const = 0;
  /// \ru Инициировать по другому полигону. \en Init by other polygon. 
  virtual void    Init( const MbPolygon3D & ) = 0;

  /// \ru Создать ломаную по полигону. \en Create a polyline on the base of the polygon. 
  virtual MbPolyline3D * CreatePolyline() const = 0;

  /// \ru Инвертировать последовательность точек. \en Reverse the sequence of points. 
  virtual void    Reverse() = 0;

  /** \} */

          /// \ru Выдать все точки полигона. \en Get all the points of the polygon. 
          void    GetPoints( std::vector<MbCartPoint3D> & pnts ) const;
          /// \ru Выдать все точки полигона. \en Get all the points of the polygon. 
          void    GetPoints( std::vector<MbFloatPoint3D> & pnts ) const;

private :
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation, to prevent an assignment by default. 
          void    operator = ( const MbPolygon3D & );

}; // MbPolygon3D


////////////////////////////////////////////////////////////////////////////////
/** \brief \ru Триангуляция.
           \en Triangulation. \~
  \details \ru Триангуляция представляет собой набор треугольных и четырёхугольных пластин, 
    стыкующихся друг с другом по общим сторонам.\n
      В простейшем случае триангуляция состоит из набора точек и набора треугольников.
    Треугольник - это три номера из набора точек, определяющих вершины треугольника.
    Триангуляция используется для аппроксимации криволинейных поверхностей.
    В определенных случаях использование плоских пластин значительно упрощает работу с криволинейными поверхностями. 
    Триангуляция описывает геометрическую форму объектов с определённой степенью точности.\n
      Триангуляция используется для получения точечных изображений, вычисления 
    масс-инерционных характеристик, проверки столкновений объектов, проведения численных 
    экспериментов над моделями. Для одного и того же объекта триангуляция может 
    иметь разное наполнение данных, в зависимости от назначения.\n
           \en Triangulation represents a set of triangular and quadrangular plates 
    which are joined to each other by their common sides.\n
      In simple case triangulation consists of a set of points and a set of triangles.
    Triangle is represented as three numbers from the set of points defining vertices of triangle.
    Triangulation is used for approximation of curved surfaces.
    In certain cases use of flat plates significantly simplifies work 
    with curved surfaces. Triangulation describes geometric form of objects 
    with the specified accuracy.\n
      Triangulation is used for obtaining point-images, for calculating 
    mass-inertial properties, for collision detection, for numerical 
    experiments with models. For the same object triangulation can 
    have different data depending on its purpose.\n \~
      \par \ru Назначения триангуляции
      От назначения триангуляции зависит наполнение данных:\n
      Если триангуляция предназначена для визуализации геометрической формы, 
    то заполняются множества points и normals, 
    шаг движения вдоль кривых и поверхностей вычисляется по стрелке прогиба, работают функции Step, StepU, StepV.\n
      Если триангуляция предназначена для аппроксимации геометрической формы с привязкой к параметрам поверхности, 
    то заполняются множества params, points и normals,
    шаг движения вдоль кривых и поверхностей вычисляется по стрелке прогиба.\n
      Если триангуляция предназначена для вычисления инерционных характеристик, 
    то заполняется множество params, шаг движения вдоль кривых и поверхностей вычисляется по углу отклонения нормали, 
    работают функции DeviationStep, DeviationStepU, DeviationStepV.\n
      Если триангуляция предназначена для определения столкновений элементов модели,
    то заполняются множества params и points, шаг движения вдоль кривых и поверхностей 
    вычисляется по стрелке прогиба, работают функции Step, StepU, StepV.\n
      Если триангуляция предназначена для разбивки на элементы,
    то заполняются множества points и normals, шаг движения вдоль кривых и поверхностей 
    вычисляется с ограничением длины сторон треугольников, может быть добавлено ограничение по стрелке прогиба и углу отклонения нормали.
    (работают функции MetricStep, MetricStepU, MetricStepV).\n
           \en Purposes of triangulation
      Data depends on purposes of triangulation:\n
      If triangulation is intended for visualization of a geometric form, 
    then 'points' and 'normals' sets are filled (in case of planes 'normals' contains one 
    element), spacing of sample along the curves and the surfaces is calculated by stepData
    (Step, StepU, StepV functions work).\n
      If triangulation is intended for visualization of geometric form and 
    texture, then 'params', 'points' and 'normals' sets are filled, spacing of sample along 
    curves and surfaces is calculated by stepData.\n
      If triangulation is intended for calculation of mass-inertial properties, 
    then 'params' set is filled, spacing of sample along curves and surfaces 
    is calculated by angle of deviation (DeviationStep, DeviationStepU, 
    DeviationStepV functions work).\n
      If triangulation is intended for collision detection between elements of model,
    then 'params' and 'points' sets are filled, spacing of sample along curves and surfaces 
    is calculated by stepData (Step, StepU, StepV functions work).\n
      If triangulation is intended for splitting into elements,
    then 'normals' and 'points' sets are filled, spacing of sampling along curves and surfaces 
    is calculated by angle of deviation with limiting of length 
    (MetricStep, MetricStepU, MetricStepV functions work).\n \~
  \ingroup Polygonal_Objects
*/
////////////////////////////////////////////////////////////////////////////////
class MATH_CLASS MbGrid : public MbPrimitive {
protected:
  std::vector<MbTriangle>       triangles;   ///< \ru Индексное множество треугольных пластин содержит номера элементов множества params и/или множеств points и normals. \en Set of triangular plates contains numbers of elements of 'params' set and/or of 'points' and 'normals' sets. 
  std::vector<MbQuadrangle>     quadrangles; ///< \ru Индексное множество четырёхугольных пластин содержит номера элементов множества params и/или множеств points и normals. \en Set of quadrangular plates contains numbers of elements of 'params' set and/or of 'points' and 'normals' sets. 
  std::vector<MbGridLoop *>     loops;       ///< \ru Индексное множество граничных циклов триангуляции содержит номера элементов множества points и/или params. Объект владеет элементами множества. \en Set of bounding loops contains numbers of elements of 'points' set and/or of 'params' set. Ownership of elements of set.
  MbStepData                    stepData;    ///< \ru Параметр расчета триангуляции. \en Parameter of triangulation calculation.
  /** \brief \ru Габаритный куб объекта.
             \en Bounding box of object. \~
      \details \ru Габаритный куб объекта рассчитывается только при запросе габарита объекта. Габаритный куб в конструкторе объекта и после модификации объекта принимает неопределенное значение.
               \en Bounding box of object is calculated only at the request. Bounding box of object is undefined after object constructor and after object modifications \n \~
  */
  mutable MbCube                cube;
  mutable MbGridSearchTree *    searchTree;  ///< \ru Дерево поиска элементов. \en Elements search tree.

protected:
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en The copy constructor without implementation prevents from copying by default. 
  MbGrid( const MbGrid & );
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbGrid( const MbGrid &, MbRegDuplicate * );
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  MbGrid();
public:
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbGrid();

public:

  /** \ru \name Общие функции примитива.
      \en \name Common functions of primitive.
      \{ */
  MbePrimitiveType Type() const override; // \ru Тип объекта. \en A type of an object.
  //virtual MbePrimitiveType IsA() const = 0; // \ru Тип объекта. \en A type of an object.
  //virtual MbPrimitive & Duplicate( MbRegDuplicate * iReg = nullptr ) const = 0; // \ru Создать копию объекта. \en Create a copy of the object.
  //virtual void    Transform( const MbMatrix3D & ) = 0;                 // \ru Преобразовать сетку согласно матрице. \en Transform mesh according to the matrix.
  //virtual void    Move     ( const MbVector3D & ) = 0;                 // \ru Сдвиг сетки. \en Move mesh.
  //virtual void    Rotate   ( const MbAxis3D &, double angle ) = 0;     // \ru Поворот сетки вокруг оси. \en Rotation of mesh about an axis.
  //virtual void    AddYourGabaritTo( MbCube & ) const = 0;              // \ru Расширить присланный габаритный куб так, чтобы он включал в себя данный объект. \en Extend given bounding box so that it encloses the given object.
  virtual double  DistanceToPoint( const MbCartPoint3D & ) const override;  // \ru Вычислить расстояние до точки. \en Calculate distance to point.
  virtual double  DistanceToLine( const MbAxis3D &, double extDistance, double & t ) const override;  // \ru Вычислить расстояние до оси. \en Calculate the distance to an axis.
  //virtual void    GetProperties( MbProperties & ) = 0;                 // \ru Выдать свойства объекта. \en Get properties of the object.
  //virtual void    SetProperties( const MbProperties & ) = 0;           // \ru Записать свойства объекта. \en Set properties of the object.

  /** \} */
  /** \ru \name Функции триангуляции.
      \en \name Functions of triangulation.
      \{ */
  /// \ru Проверить наличие данных. \en Check data availability. 
          bool    IsComplete() const { return (triangles.size() > 0) || (quadrangles.size() > 0) || (loops.size() > 0); }
  /// \ru Выдать количество точек. \en Get the number of points. 
  virtual size_t  PointsCount()  const = 0;
  /// \ru Выдать количество нормалей. \en Get the number of normals. 
  virtual size_t  NormalsCount() const = 0;
  /// \ru Выдать количество параметров. \en Get the number of parameters. 
  virtual size_t  ParamsCount()  const = 0;
  /// \ru Выдать количество значений. \en Get count of values. 
  virtual size_t  EscortsCount() const = 0;
  /// \ru Выдать количество точек минус 1 (максимальный индекс). \en Get the number of points minus one (maximal index). 
  virtual ptrdiff_t PointsMaxIndex()  const = 0;
  /// \ru Выдать количество нормалей минус 1 (максимальный индекс). \en Get the number of normals minus one (maximal index). 
  virtual ptrdiff_t NormalsMaxIndex() const = 0;
  /// \ru Выдать количество параметров минус 1 (максимальный индекс). \en Get the number of parameters minus one (maximal index). 
  virtual ptrdiff_t ParamsMaxIndex()  const = 0;

          /// \ru Выдать количество треугольников. \en Get the number of triangles. 
          size_t  TrianglesCount() const { return triangles.size(); }
          /// \ru Выдать количество четырёхугольников. \en Get the number of quadrangles. 
          size_t  QuadranglesCount() const { return quadrangles.size(); }
          /// \ru Выдать количество полигонов. \en Get the number of loops. 
          size_t  LoopsCount() const { return loops.size(); }

  /// \ru Добавить в триангуляцию параметры, точку и нормаль триангулируемой поверхности в точке. \en Add parameters, point and normal of triangulated surface at point to triangulation. 
  virtual void    AddPoint( const MbCartPoint & p2D, const MbCartPoint3D & p3D, const MbVector3D & n3D ) = 0;
  /// \ru Добавить в триангуляцию параметры и точку. \en Add parameters and a point to triangulation. 
  virtual void    AddPoint( const MbCartPoint & p2D, const MbCartPoint3D & p3D ) = 0;
  /// \ru Добавить в триангуляцию точку и нормаль в точке. \en Add a point and normal at the point to triangulation. 
  virtual void    AddPoint( const MbCartPoint3D & p3D, const MbVector3D & n3D ) = 0;
  /// \ru Добавить в триангуляцию точку. \en Add a point to triangulation. 
  virtual void    AddPoint( const MbCartPoint3D & p3D ) = 0;
  /// \ru Добавить в триангуляцию нормаль. \en Add a normal to triangulation. 
  virtual void    AddNormal( const MbVector3D & n3D ) = 0;
  /// \ru Добавить в триангуляцию параметры триангулируемой поверхности. \en Add parameters of triangulated surface to triangulation. 
  virtual void    AddParam( const MbCartPoint & p2D ) = 0;

  /// \ru Добавить в триангуляцию параметры, точку и нормаль триангулируемой поверхности в точке. \en Add parameters, point and normal of triangulated surface at point to triangulation. 
  virtual void    AddPoint( const MbFloatPoint & p2D, const MbFloatPoint3D & p3D, const MbFloatVector3D & n3D ) = 0;
  /// \ru Добавить в триангуляцию параметры и точку. \en Add parameters and a point to triangulation. 
  virtual void    AddPoint( const MbFloatPoint & p2D, const MbFloatPoint3D & p3D ) = 0;
  /// \ru Добавить в триангуляцию точку и нормаль в точке. \en Add a point and normal at the point to triangulation. 
  virtual void    AddPoint( const MbFloatPoint3D & p3D, const MbFloatVector3D & n3D ) = 0;
  /// \ru Добавить в триангуляцию точку. \en Add a point to triangulation. 
  virtual void    AddPoint( const MbFloatPoint3D & p3D ) = 0;
  /// \ru Добавить в триангуляцию нормаль. \en Add a normal to triangulation. 
  virtual void    AddNormal( const MbFloatVector3D & n3D ) = 0;
  /// \ru Добавить в триангуляцию параметры триангулируемой поверхности. \en Add parameters of triangulated surface to triangulation. 
  virtual void    AddParam( const MbFloatPoint & p2D ) = 0;

          /// \ru Добавить в триангуляцию точки. \en Add points to triangulation. 
          template <class PointsVector>
          void    AddPoints( const PointsVector & pnts )
          {
            size_t addCnt = pnts.size();
            PointsReserve( addCnt );
            for ( size_t k = 0; k < addCnt; ++k ) {
              AddPoint( pnts[k] );
            }
            DeleteSearchTree();
            cube.SetEmpty();
          }
          /// \ru Добавить в триангуляцию нормали. \en Add normals to triangulation. 
          template <class NormalsVector>
          void    AddNormals( const NormalsVector & norms )
          {
            size_t addCnt = norms.size();
            NormalsReserve( addCnt );
            for ( size_t k = 0; k < addCnt; ++k ) {
              AddNormal( norms[k] );
            }
          }
          /// \ru Добавить в триангуляцию параметры триангулируемой поверхности. \en Add parameters of triangulated surface to triangulation.
          template <class ParamsVector>
          void    AddParams( const ParamsVector & pars )
          {
            size_t addCnt = pars.size();
            ParamsReserve( addCnt );
            for ( size_t k = 0; k < addCnt; ++k ) {
              AddParam( pars[k] );
            }
          }
          /// \ru Добавить в триангуляцию объекты. \en Add objects to triangulation.
          template <class TrianglesVector>
          void    AddTriangles( const TrianglesVector & trngs )
          {
            size_t addCnt = trngs.size();
            triangles.reserve( triangles.size() + addCnt );
            for ( size_t k = 0; k < addCnt; ++k )
              triangles.push_back( trngs[k] );
            DeleteSearchTree();
          }
          /// \ru Добавить в триангуляцию объекты. \en Add objects to triangulation.
          template <class QuadranglesVector>
          void    AddQuadrangles( const QuadranglesVector & qrngs )
          {
            size_t addCnt = qrngs.size();
            quadrangles.reserve( quadrangles.size() + addCnt );
            for ( size_t k = 0; k < addCnt; ++k )
              quadrangles.push_back( qrngs[k] );
            DeleteSearchTree();
          }

  /// \ru Добавить в коллекцию данных. \en Add scores to collection. 
  virtual void    AddEscorts( const std::vector<uint32> & scores ) = 0;
  /** \brief \ru Добавить цвет как дополнительную информацию в точке (0 <= r,g,b,a <= 1).
             \en Add color as an additional information of point (0 <= r,g,b,a <= 1). \~
    \details
    \warning \ru Значения компонент цвета должны лежать в диапазоне [ 0; 1 ].
             \en Values of color components should belong to the range [ 0; 1 ]. \~
  \ingroup Polygonal_Objects
*/
  virtual void    AddColor( float r, float g, float b, float a ) = 0;

  /// \ru Выдать точку по её номеру. \en Get point by its index. 
  virtual void    GetPoint( size_t i, MbCartPoint3D & p ) const = 0;
  /// \ru Выдать нормаль по её номеру. \en Get normal by its index. 
  virtual void    GetNormal( size_t i, MbVector3D & n ) const = 0;
  /// \ru Выдать параметр по его номеру. \en Get parameter by its index. 
  virtual void    GetParam( size_t i, MbCartPoint & p ) const = 0;
  /// \ru Выдать дополнительную информацию по её номеру. \en Get additional information by its index. 
  virtual const   uint32 & GetEscort( size_t i ) const = 0;

  /// \ru Выдать точку по её номеру. \en Get point by its index. 
  virtual void    GetPoint( size_t i, MbFloatPoint3D & p ) const = 0;
  /// \ru Выдать нормаль по её номеру. \en Get normal by its index. 
  virtual void    GetNormal( size_t i, MbFloatVector3D & n ) const = 0;
  /// \ru Выдать параметр по его номеру. \en Get parameter by its index. 
  virtual void    GetParam( size_t i, MbFloatPoint & p ) const = 0;

  /// \ru Установить точку с заданным номером. \en Set point by the given index. 
  virtual void    SetPoint( size_t i, const MbCartPoint3D & p ) = 0;
  /// \ru Установить нормаль с заданным номером. \en Set normal by the given index. 
  virtual void    SetNormal( size_t i, const MbVector3D & n ) = 0;
  /// \ru Установить параметр с заданным номером. \en Set parameter by the given index. 
  virtual void    SetParam( size_t i, const MbCartPoint & p ) = 0;
  /// \ru Установить дополнительную информацию по её номеру. \en Set additional information by its index. 
  virtual void    SetEscort( size_t i, const uint32 & e ) = 0;

  /// \ru Удалить точку с заданным номером. \en Delete point by the given index. 
  virtual void    PointRemove( size_t i ) = 0;
  /// \ru Удалить нормаль с заданным номером. \en Delete normal by the given index. 
  virtual void    NormalRemove( size_t i ) = 0;
  /// \ru Удалить параметры поверхности с заданным номером. \en Delete parameters of surface by the given index. 
  virtual void    ParamRemove( size_t i ) = 0;
          /// \ru Удалить треугольник по его индексу. \en Delete triangle by its index. 
          void    TriangleRemove( size_t k );
          /// \ru Удалить четырёхугольник по его индексу. \en Delete quadrangle by its index. 
          void    QuadrangleRemove( size_t k );
          /// \ru Удалить полигон по его индексу. \en Delete polygon by its index. 
          void    LoopRemove( size_t k );

  /// \ru Удалить точки. \en Delete points. 
  virtual void    PointsDelete() = 0;
  /// \ru Удалить нормали. \en Delete normal. 
  virtual void    NormalsDelete() = 0;
  /// \ru Удалить параметры. \en Delete params. 
  virtual void    ParamsDelete() = 0;
  /// \ru Удалить дополнительную информацию. \en Delete additional information. 
  virtual void    EscortsDelete() = 0;
          /// \ru Удалить все треугольники. \en Delete all triangles. 
          void    TrianglesDelete();
          /// \ru Удалить все четырёхугольники. \en Delete all quadrangles. 
          void    QuadranglesDelete();
          /// \ru Удалить все полигоны. \en Delete all polygons. 
          void    LoopsDelete();

  /// \ru Инвертировать нормали. \en Invert normals. 
  virtual void    NormalsInvert() = 0;

          /// \ru Добавить треугольник. \en Add a triangle. 
          void    AddTriangle( const MbTriangle & triangle ) { triangles.push_back( triangle ); DeleteSearchTree(); }
          /// \ru Добавить треугольник с заданными номерами вершин. \en Add a triangle by the given indices of vertices 
          void    AddTriangle( uint j0, uint j1, uint j2, bool o ) { MbTriangle t( j0, j1, j2, o ); triangles.push_back( t ); DeleteSearchTree(); }
          /// \ru Добавить четырёхугольник. \en Add a quadrangle. 
          void    AddQuadrangle( const MbQuadrangle & quadrangle ) { quadrangles.push_back( quadrangle ); DeleteSearchTree(); }
          /// \ru Добавить четырёхугольник с заданными номерами вершин. \en Add a quadrangle by the given indices of vertices. 
          void    AddQuadrangle( uint j0, uint j1, uint j2, uint j3, bool o ) { MbQuadrangle t( j0, j1, j2, j3, o ); quadrangles.push_back( t ); DeleteSearchTree(); }

          /// \ru Добавить полигон. \en Add a polygon. 
          void    AddGridLoop( MbGridLoop & poly ) { loops.push_back( &poly ); }
          /// \ru Собрать внешние полигоны. \en Collect outer loops. 
          void    CollectEdges( std::vector<c3d::UintPair> & edges ) const;

          /// \ru Выдать индексы точек в массиве points для i-го треугольника (связанного или несвязанного). \en Get indices of points in 'points' array for i-th triangle (adjacent or non-adjacent). 
          bool    GetTrianglePointIndex( size_t i, uint & ind0, uint & ind1, uint & ind2 ) const;
          /// \ru Выдать индексы точек в массиве points для i-го четырехугольника (связанного или несвязанного). \en Get indices of points in 'points' array for i-th quadrangle (adjacent or non-adjacent). 
          bool    GetQuadranglePointIndex( size_t i, uint & ind0, uint & ind1, uint & ind2, uint & ind3 ) const;

  // \ru Выдать для треугольника с номером i в общей нумерации (с полосами) параметры поверхности. \en Get parameters of surface for i-th triangle in general numbering (with strips). 
  virtual bool    GetTriangleParams( size_t i, MbCartPoint & r0, MbCartPoint & r1, MbCartPoint & r2 ) const = 0;
  // \ru Выдать для треугольника с номером i в общей нумерации (с полосами) точки вершин. \en Get points of vertices for i-th triangle in general numbering (with strips). 
  virtual bool    GetTrianglePoints( size_t i, MbCartPoint3D & p0, MbCartPoint3D & p1, MbCartPoint3D & p2 ) const = 0;
  // \ru Выдать для треугольника с номером i в общей нумерации (с полосами) точки вершин. \en Get points of vertices for i-th triangle in general numbering (with strips). 
  virtual bool    GetTrianglePoints( size_t i, MbFloatPoint3D & p0, MbFloatPoint3D & p1, MbFloatPoint3D & p2 ) const = 0;
  // \ru Выдать для треугольника с номером i в общей нумерации (с полосами) нормали в вершинах. \en Get normals at vertices for i-th triangle in general numbering (with strips). 
  virtual bool    GetTriangleNormals( size_t i, MbVector3D & n0, MbVector3D & n1, MbVector3D & n2 ) const = 0;
  // \ru Выдать для треугольника с номером i в общей нумерации (с полосами) нормали в вершинах. \en Get normals at vertices for i-th triangle in general numbering (with strips). 
  virtual bool    GetTriangleNormals( size_t i, MbFloatVector3D & n0, MbFloatVector3D & n1, MbFloatVector3D & n2 ) const = 0;

  // \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) параметры поверхности. \en Get parameters of surface for i-th quadrangle in general numbering (with strips). 
  virtual bool    GetQuadrangleParams( size_t i, MbCartPoint & r0, MbCartPoint & r1, MbCartPoint & r2, MbCartPoint & r3 ) const = 0;
  // \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) точки вершин. \en Get points of vertices for i-th quadrangle in general numbering (with strips). 
  virtual bool    GetQuadranglePoints( size_t i, MbCartPoint3D & p0, MbCartPoint3D & p1, MbCartPoint3D & p2, MbCartPoint3D & p3 ) const = 0;
  // \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) точки вершин. \en Get points of vertices for i-th quadrangle in general numbering (with strips). 
  virtual bool    GetQuadranglePoints( size_t i, MbFloatPoint3D & p0, MbFloatPoint3D & p1, MbFloatPoint3D & p2, MbFloatPoint3D & n3 ) const = 0;
  // \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) нормали в вершинах. \en Get normals at vertices for i-th quadrangle in general numbering (with strips). 
  virtual bool    GetQuadrangleNormals( size_t i, MbVector3D & n0, MbVector3D & n1, MbVector3D & n2, MbVector3D & n3 ) const = 0;
  // \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) нормали в вершинах. \en Get normals at vertices for i-th quadrangle in general numbering (with strips). 
  virtual bool    GetQuadrangleNormals( size_t i, MbFloatVector3D & n0, MbFloatVector3D & n1, MbFloatVector3D & n2, MbFloatVector3D & n3 ) const = 0;

  // \ru Выдать первую нормаль для плоской триангуляции, если количество точек больше количества нормалей (только для плоской триангуляции). \en Get first normal for flat triangulation if count of points is greater than count of normals (only for planar triangulation). 
  virtual bool    GetSingleNormal( MbVector3D & ) const = 0;
  // \ru Выдать первую нормаль для плоской триангуляции, если количество точек больше количества нормалей (только для плоской триангуляции). \en Get first normal for flat triangulation if count of points is greater than count of normals (only for planar triangulation). 
  virtual bool    GetSingleNormal( MbFloatVector3D & ) const = 0;
  /// \ru Если количество точек больше количества нормалей, то добавить недостающие нормали (только для плоской триангуляции). \en If count of points is greater than count of normals, then add missing normals (only for planar triangulation). 
  virtual void    SynchronizeNormals() = 0;
  /// \ru Нормализовать нормали. \en Normalize normals. 
  virtual void    NormalizeNormals() = 0;

          /// \ru Выдать треугольник с номером i. \en Get i-th triangle. 
          const   MbTriangle & GetTriangle( size_t i ) const { return triangles[i]; }
          /// \ru Выдать четырёхугольник с номером i. \en Get i-th quadrangle. 
          const   MbQuadrangle & GetQuadrangle( size_t i ) const { return quadrangles[i]; }
          /// \ru Выдать полигон с номером i. \en Get i-th polygon. 
          const   MbGridLoop & GetGridLoop( size_t i ) const { return *loops[i]; }

          /// \ru Выдать для треугольника с номером i номера точек вершин. \en Get indices of vertex points for i-th triangle. 
          bool    GetTriangleIndex( size_t i, uint & i0, uint & i1, uint & i2 ) const;
          /// \ru Выдать для четырёхугольника с номером i номера точек вершин. \en Get indices of points of vertices for i-th quadrangle. 
          bool    GetQuadrangleIndex( size_t i, uint & i0, uint & i1, uint & i2, uint & i3 ) const;

          /// \ru Выдать контейнер параметров. \en Get the container of parameters. 
          template <class Param, class ParamsVector>
          void    GetParams( ParamsVector & paramsVector ) const
          {
            Param param;
            paramsVector.reserve( paramsVector.size() + ParamsCount() );
            for ( size_t i = 0, iCount = ParamsCount(); i < iCount; ++i ) {
              GetParam( i, param );
              paramsVector.push_back( param );
            }
          }
          /// \ru Выдать контейнер точек. \en Get the container of points. 
          template <class Point, class PointsVector>
          void    GetPoints( PointsVector & pointsVector ) const
          {
            Point point;
            pointsVector.reserve( pointsVector.size() + PointsCount() );
            for ( size_t i = 0, iCount = PointsCount(); i < iCount; ++i ) {
              GetPoint( i, point );
              pointsVector.push_back( point );
            }
          }
          /// \ru Выдать контейнер нормалей. \en Get the container of normals. 
          template <class Normal, class NormalsVector>
          void    GetNormals( NormalsVector & normalsVector ) const
          {
            Normal normal;
            normalsVector.reserve( normalsVector.size() + NormalsCount() );
            for ( size_t i = 0, iCount = NormalsCount(); i < iCount; ++i ) {
              GetNormal( i, normal );
              normalsVector.push_back( normal );
            }
          }
          /// \ru Выдать контейнер треугольников. \en Get the container of triangles. 
          template <class TrianglesVector>
          void    GetTriangles( TrianglesVector & tVector ) const
          {
            tVector.reserve( tVector.size() + triangles.size() );
            for ( size_t i = 0, iCount = triangles.size(); i < iCount; ++i )
              tVector.push_back( triangles[i] );
          }
          /// \ru Выдать контейнер четырёхугольников. \en Get the container of quadrangles. 
          template <class QuadranglesVector>
          void    GetQuadrangles( QuadranglesVector & qVector ) const
          {
            qVector.reserve( qVector.size() + quadrangles.size() );
            for ( size_t i = 0, iCount = quadrangles.size(); i < iCount; ++i )
              qVector.push_back( quadrangles[i] );
          }
          /// \ru Преобразовать четырёхугольники в треугольники. \en Convert quadrangles to triangles. 
          void    ConvertQuadranglesToTriangles();
          /// \ru Преобразовать все объекты в треугольники и уравнять число точек и нормалей. \en Convert all objects to triangles and equalize count of points and count of normals. 
          void    ConvertAllToTriangles();
  
          /// \ru Вывернуть треугольник относительно заданного ребра. \en Flip triangle around given edge.     
          bool    FlipTriangle( size_t triIndex, size_t edgeIndex );

  /// \ru Определить, пересекается ли проекция на глобальную плоскость XY треугольника с заданным номером с присланным прямоугольником. \en Determine whether the projection of triangle with a given index to the global XY-plane intersects the given rectangle. 
  virtual bool    TriangleIntersectRect( size_t i, MbRect & rect ) const = 0;
  /// \ru Рассчитать габаритный прямоугольник проекции на глобальную плоскость XY треугольника с заданным номером. \en Determine bounding box of the projection of triangle with given index to the global XY-plane. 
  virtual void    TriangleGetGabRect( size_t i, MbRect & rect ) const = 0;

  /// \ru Определить, пересекается ли проекция на глобальную плоскость XY четырёхугольника с заданным номером с присланным прямоугольником. \en Determine whether the projection of quadrangle with given index to the global XY-plane intersects the given rectangle. 
  virtual bool    QuadrangleIntersectRect( size_t i, MbRect & rect ) const = 0;
  /// \ru Рассчитать габаритный прямоугольник проекции на глобальную плоскость XY четырёхугольника с заданным номером. \en Determine bounding box of the projection of quadrangle with given index to the global XY-plane. 
  virtual void    QuadrangleGetGabRect( size_t i, MbRect & rect ) const = 0;

  /// \ru Расширить присланный габаритный прямоугольник так, чтобы он включал в себя проекцию данного объекта на глобальную плоскость XY. \en Extend given bounding box so that it enclose projection of this object to the global XY-plane. 
  virtual void    AddRect( MbRect & rect ) const = 0;
  /// \ru Расширить присланный габаритный куб так, чтобы он включал в себя данный объект. \en Extend given bounding box so that it encloses the given object. 
  virtual void    AddCube( MbCube & r ) const = 0;

  /// \ru Удалить дублирующие с заданной точностью друг друга точки. \en Remove redundant points with a given tolerance (duplicates).
  virtual bool    RemoveRedundantPoints( bool deleteNormals, double epsilon = LENGTH_EPSILON ) = 0;

  /**
    \brief \ru Определить положение объекта относительно плоскости.
            \en Define the object position relative to the plane. \~
    \details \ru Определить положение объекта относительно плоскости XY локальной системы координат.
              \en Determine the object position relative to the XY-plane of a local coordinate system. \~
    \param[in] pl - \ru Локальная система координат, задающая плоскость.
                    \en A local coordinate system which defines a plane. \~
    \param[in] eps - \ru Метрическая точность.
                      \en A metric tolerance. \~
    \param[in] onlyInItem - \ru Интересует только положение объекта над плоскостью XY локальной системы координат.
                            \en Whether the object position relative to the XY-plane of a local coordinate system is interested only. \~
    \return \ru iloc_OnItem - объект пересекает плоскость XY локальной системы координат,\n
            iloc_InItem - объект расположен над плоскостью XY локальной системы координат,\n
            iloc_OutOfItem - объект расположен под плоскостью XY локальной системы координат.
            \en Iloc_OnItem - object intersects the XY plane of a local coordinate system,\n
            iloc_InItem - object is located over the XY plane of a local coordinate system,\n
            iloc_OutOfItem - object is located under the XY plane of a local coordinate system. \~
  */
  virtual MbeItemLocation GetLocation( const MbPlacement3D & pl, double eps, bool onlyInItem = false ) const = 0;

  /**
    \brief \ru Определить положение объекта относительно трубы.
            \en Determine the object position relative to the tube. \~
    \details \ru Определить, расположен ли объект внутри трубы прямоугольного сечения,
      заданного прямоугольником в плоскости XY локальной системы координат.
              \en Determine whether the object is inside the tube of rectangular section
      given by a rectangle in the XY plane of a local coordinate system. \~
    \param[in] place - \ru Локальная система координат, в в плоскости XY которой лежит сечение трубы.
                        \en A local coordinate system in the XY plane of which a tube section is located. \~
    \param[in] rect - \ru Прямоугольник, задающая сечение трубы.
                      \en A rectangle which defines a tube section. \~
    \param[in] eps - \ru Метрическая точность.
                      \en A metric tolerance. \~
    \param[in] onlyInItem - \ru Интересует только положение объекта внутри трубы.
                            \en Whether the object position relative to the tube is interested only. \~
    \return \ru true, если объект расположен внутри трубы.
            \en True if the object is inside the tube. \~
  */
  virtual bool    InsideLocation( const MbPlacement3D & place, MbRect & rect, double eps ) const = 0;
  /// \ru Преобразовать триангуляцию так, чтобы её параллельная проекция выглядела как центральная проекция, наблюдаемая из заданной точки vista. \en Transform triangulation so that its parallel projection looks as the central projection observed from the given 'vista' point. 
  virtual void    SetVista( const MbCartPoint3D & vista ) = 0;
  /// \ru Отменить преобразование триангуляцию для центральной проекции, наблюдаемой из заданной точки vista. \en Undo the transformation of triangulation for central projection observed from given 'vista' point. 
  virtual void    DeleteVista( const MbCartPoint3D & vista ) = 0;

  /// \ru Зарезервировать память для контейнера параметров. \en Reserve memory for container of elements. 
  virtual void    ParamsReserve( size_t n ) = 0;
  /// \ru Зарезервировать память для контейнера точек. \en Reserve memory for container of points. 
  virtual void    PointsReserve( size_t n ) = 0;
  /// \ru Зарезервировать память для контейнера нормалей. \en Reserve memory for container of normals. 
  virtual void    NormalsReserve( size_t n ) = 0;
  /// \ru Зарезервировать память для контейнера параметров. \en Reserve memory for container of elements. 
  virtual void    EscordsReserve( size_t n ) = 0;

  /// \ru Зарезервировать память для контейнера треугольников. \en Reserve memory for container of triangles. 
          void    TrianglesReserve( size_t n ) { triangles.reserve( triangles.size() + n ); }
  /// \ru Зарезервировать память для контейнера четырёхугольников. \en Reserve memory for container of quadrangles. 
          void    QuadranglesReserve( size_t n ) { quadrangles.reserve( quadrangles.size() + n ); }
  /// \ru Зарезервировать память для контейнера полигонов. \en Reserve memory for container of loops. 
          void    LoopsReserve( size_t n ) { loops.reserve( loops.size() + n ); }

  /// \ru Зарезервировать память для контейнеров. \en Reserve memory for some containers. 
  virtual void    ReserveParamsPoints( size_t n ) = 0;
  /// \ru Зарезервировать память для контейнеров. \en Reserve memory for some containers. 
  virtual void    ReservePointsNormals( size_t n ) = 0;
  /// \ru Зарезервировать память для контейнеров. \en Reserve memory for some containers. 
  virtual void    ReserveParamsPointsNormals( size_t n ) = 0;

  /// \ru Удалить всю триангуляцию без освобождения памяти, занятую контейнерами. \en Delete all triangulation without freeing the memory occupied by containers. 
  virtual void    Flush() = 0;
  /// \ru Удалить всю триангуляцию и освободить память. \en Delete all triangulation and free the memory. 
  virtual void    HardFlush() = 0;
  /// \ru Освободить лишнюю память. \en Free the unnecessary memory. 
  virtual void    Adjust() = 0;

  /// \ru Выдать размер занимаемой памяти. \en Get the size of taken memory. 
  virtual size_t  SizeOf() const = 0;
  /// \ru Инвертировать последовательность вершин треугольников и четырехугольников. \en Reverse the sequence of vertices of triangles and quadrilaterals. 
  virtual void    Reverse() = 0;
  /// \ru Являются ли объекты равными? \en Determine whether objects are equal. 
  virtual bool    IsSame( const MbGrid & init, double eps ) const = 0;

          /// \ru Выдать параметр расчета триангуляции (стрелку прогиба или угол отклонения). \en Get the parameter of triangulation calculation. 
          const   MbStepData &      GetStepData() const { return stepData; }
          /// \ru Установить параметр расчета триангуляции (стрелку прогиба или угол отклонения). \en Set the parameter of triangulation calculation. 
          void                      SetStepData( const MbStepData & stData ) { stepData = stData; }
          /// \ru Вернуть габаритный куб. \en Return bounding box. 
          const   MbCube &          Cube() const { return cube; }
          /// \ru Вернуть габаритный куб. \en Return bounding box. 
          const   MbCube &          GetCube() const;

  // \ru Инициировать по другой триангуляции. \en Init by other triangulation. 
  virtual void                      Init( const MbGrid & grid ) = 0;

          /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
          const   MbTriangle *      GetTrianglesAddr() const { return (!triangles.empty() ? &(triangles[0]) : nullptr); }
          /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
          const   MbQuadrangle *    GetQuadranglesAddr() const { return (!quadrangles.empty() ? &(quadrangles[0]) : nullptr); }
  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  virtual const   MbCartPoint3D *   GetExactPointsAddr() const = 0;
  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  virtual const   MbVector3D *      GetExactNormalsAddr() const = 0;
  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  virtual const   MbCartPoint *     GetExactParamsAddr() const = 0;
  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  virtual const   MbFloatPoint3D *  GetFloatPointsAddr() const = 0;
  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  virtual const   MbFloatVector3D * GetFloatNormalsAddr() const = 0;
  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  virtual const   MbFloatPoint *    GetFloatParamsAddr() const = 0;

  /** \} */

public:
          /** \brief \ru Вычислить расстояние до точки.
                     \en Calculate distance to point. \~
              \details \ru Вычислить расстояние от точки до ближайшей опорной точки триангуляции. \n
                       \en Calculate the distance from a point to the nearest triangulation reference point. \n \~
              \param[in] pnt - \ru Точка.
                               \en Point. \~
              \param[out] ind - \ru Индекс ближайшей точки в массиве опорных точек триангуляции.
                                \en Index of the nearest point in the array of triangulation reference points. \~
              \return \ru Расстояние между заданной точкой и ближайшей опорной точкой триангуляции.
                      \en Distance between the given point and the nearest triangulation reference point. \~
          */
  double  DistanceToPoint( const MbCartPoint3D & pnt, size_t & ind ) const;

  /** \brief \ru Вычислить расстояние до оси.
             \en Calculate the distance to an axis. \~
      \details \ru Вычислить расстояние от оси до ближайшего треугольника или четырехугольника триангуляции. \n
               \en Calculate the distance from the axis to the nearest triangle or quadrangle of triangulation. \n \~
      \param[in] axis - \ru Ось (луч).
                        \en Axis (view ray). \~
      \param[in] extDistance - \ru Расширение габарита триангуляции (по умолчанию можно поставить 0).
                               \en Triangulation bounding box extension (set it equal zero by default). \~
      \param[in,out] t - \ru Параметр на луче, где он пересекает триангуляцию.
                         \en Parameter on the ray where ray intersects the triangulation. \~
      \param[out] tqInd - \ru Индекс ближайшего треугольника или четырехугольника.
                          \en Index of nearest triangle or quadrangle. \~
      \return \ru Ноль, если найдено пересечение между осью и ближайшим треугольником или четырехугольником триангуляции.
              \en Zero if intersection was found, between the axis and the nearest triangulation triangle or quadrangle. \~
  */
  double  DistanceToLine( const MbAxis3D & axis, double extDistance, double & t, c3d::IndicesPair & tqInd ) const;

  /// \ru Дать объекты, содержащие указанный индекс точки. \ en Get all triangles with point index ind.
  bool    FindTrianglesByPoint( uint ind, c3d::IndicesVector & ) const;
  /// \ru Дать объекты, содержащие указанный индекс точки. \ en Get all quadrangles with point index ind.
  bool    FindQuadranglesByPoint( uint ind, c3d::IndicesVector & ) const;
  /// \ru Сделать триангуляцию односвязной. \ en Make simply connected triangulation (make cuts from outer loop to inner loops with duplicated points of cuts).
  bool    MakeSimplyConnected();

public:
  /// \ru Готово ли дерево поиска элементов. \en Whether is elements search tree ready.
  bool    IsSearchTreeReady() const;
  /// \ru Создать дерево поиска элементов. \en Create elements search tree.
  bool    CreateSearchTree( size_t leafElemCount = 100 ) const;
  /// \ru Удалить дерево поиска элементов. \en Delete elements search tree.
  void    DeleteSearchTree() const;
  /** \brief \ru Найти индексы треугольников и четырехугольников, потенциально пересекающихся с лучом.
             \en Find indices of triangles and quadrangles potentially intersecting with a ray. \~
      \details \ru Найти индексы треугольников и четырехугольников, потенциально пересекающихся с лучом, по дереву габаритов этих элементов. \n
               \en Find indices of triangles and quadrangles potentially intersecting with a ray using elements cubes tree (if it can be created). \n \~
      \param[in] ray          - \ru Луч.
                                \en Ray. \~
      \param[out] triIndices  - \ru Индексы треугольников.
                                \en Triangles indices. \~
      \param[out] quadIndices - \ru Индексы четырехугольников.
                                \en Quadrangles indices. \~
      \param[in] eps - \ru Точность проверки пересечения луча с габаритным кубом треугольника или четырехугольника.
                       \en Accuracy of checking the intersection of a ray with a bounding box of a triangle or quadrangle. \~
      \return \ru Возвращает true, если был выполнен поиск по дереву габаритов.
              \en Returns true if a search by cubes tree was performed. \~
  */
  bool    FindIntersectingElementsByCubesTree( const MbAxis3D & ray, c3d::IndicesVector & triIndices, c3d::IndicesVector & quadIndices, double eps = METRIC_EPSILON ) const;
  /** \brief \ru Найти индекс треугольника или четырехугольника, ближайших к точке.
             \en Find index of triangle or quadrangle which is closest to a point. \~
      \param[in] pnt          - \ru Точка.
                                \en Point. \~
      \param[out] pntProj  - \ru Ближайшая точка на сетке.
                             \en Nearest point from mesh. \~
      \param[out] tqInd - \ru Индекс ближайшего треугольника или четырехугольника.
                          \en Index of nearest triangle or quadrangle. \~
      \return \ru Расстояние до ближайшей точки.
              \en Distance to the nearest point. \~
  */
  double    FindNearest( const MbCartPoint3D & pnt, MbCartPoint3D & pntProj, c3d::IndicesPair & tqInd ) const;

          /** \brief \ru Найти индексы треугольников и четырехугольников, пересекающихся с лучом.
                     \en Find indices of triangles and quadrangles intersecting with a ray. \~
              \details \ru Найти индексы треугольников и четырехугольников, пересекающихся с лучом. \n
                       \en Find indices of triangles and quadrangles intersecting with a ray. \n \~
              \param[in] ray          - \ru Луч.
                                        \en Ray. \~
              \param[out] triIndices  - \ru Индексы треугольников.
                                        \en Triangles indices. \~
              \param[out] quadIndices - \ru Индексы четырехугольников.
                                        \en Quadrangles indices. \~
              \param[in] eps - \ru Точность проверки пересечения луча с габаритным кубом треугольника или четырехугольника.
                               \en Accuracy of checking the intersection of a ray with a bounding box of a triangle or quadrangle. \~
              \return \ru Возвращает true, если что-то было найдено.
                      \en Returns true if something was found. \~
          */
          bool    FindIntersectingElements( const MbAxis3D & ray, c3d::IndicesVector & triIndices, c3d::IndicesVector & quadIndices, double eps = METRIC_EPSILON ) const;
protected:
          /// \ru Переместить дерево поиска элементов. \en Move elements search tree.
          void    MoveSearchTree( const MbVector3D & ) const;
private :
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation, to prevent an assignment by default. 
          void    operator = ( const MbGrid & );
          /** \brief \ru Найти индекс треугольника или четырехугольника, ближайших к точке, простым перебором.
                     \en Find index of triangle or quadrangle which is closest to a point by a brutforce method. \~
            \param[in] pnt          - \ru Точка.
                                      \en Point. \~
            \param[out] pntProj  - \ru Ближайшая точка на сетке.
                                   \en Nearest point from mesh. \~
            \param[out] tqInd - \ru Индекс ближайшего треугольника или четырехугольника.
                                \en Index of nearest triangle or quadrangle. \~
            \return \ru Расстояние до ближайшей точки.
                    \en Distance to the nearest point. \~
          */
          double    FindNearestBrutforce( const MbCartPoint3D & pnt, MbCartPoint3D & pntProj, c3d::IndicesPair & tqInd ) const;
}; // MbGrid


//------------------------------------------------------------------------------
/** \brief \ru Вычислить квадрат расстояния от прямой линии до сегмента полигона.
           \en Calculate squared distance from straight line to segment of polygon. \~
  \details \ru Вычислить квадрат расстояния от прямой линии до сегмента полигона, заданного начальной и конечной точками. \n
    Возвращает значение параметра ближайшей точки на линии tRes и квадрат расстояния от этой точки до сегмента. \n
    Возвращает значение параметра ближайшей точки на линии tRes и квадрат расстояния от этой точки до сегмента. \n
           \en Calculate squared distance from straight line to segment of polygon defined by start and end points. \n
    Returns value of parameter of nearest point on tRes line and squared distance from this point to the segment. \n
    Returns value of parameter of nearest point on tRes line and squared distance from this point to the segment. \n \~
  \param[in] seg_base - \ru Начальная точка сегмента.
                        \en Start point of the segment. \~
  \param[in] seg_end - \ru Начальная точка сегмента.
                       \en Start point of the segment. \~
  \param[in] line - \ru Прямая линия, до которой вычисляется расстояние.
                    \en Straight line to calculate the distance to. \~
  \param[out] tRes - \ru Значение параметра ближайшей точки на линии.
                     \en Value of the nearest point parameter on the line. \~
  \return \ru Квадрат расстояния ближайшей точки до линии.
          \en Squared distance between the nearest point and the line. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC(float) LineToSegmentDistanceSquared( const MbFloatPoint3D & seg_base, 
                                               const MbFloatPoint3D & seg_end,
                                               const MbFloatAxis3D &  line,
                                                     float &          tRes );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить квадрат расстояния от прямой линии до сегмента полигона.
           \en Calculate squared distance from straight line to segment of polygon. \~
  \details \ru Вычислить квадрат расстояния от прямой линии до сегмента полигона, заданного начальной и конечной точками. \n
    Возвращает значение параметра ближайшей точки на линии tRes, вектор между ближайшими точками 
    и квадрат расстояния от этой точки до сегмента. \n
           \en Calculate squared distance from straight line to segment of polygon defined by start and end points. \n
    Returns the value of parameter of nearest point on the line, the vector between the nearest points 
    and the squared distance from this point to the segment. \n \~
  \param[in] seg_base - \ru Начальная точка сегмента.
                        \en Start point of the segment. \~
  \param[in] seg_end - \ru Начальная точка сегмента.
                       \en Start point of the segment. \~
  \param[in] line - \ru Прямая линия, до которой вычисляется расстояние.
                    \en Straight line to calculate the distance to. \~
  \param[out] vRes - \ru Вектор от ближайшей точки на линии до ближайшей точки на сегменте.
                     \en Vector from the nearest point on the line to the nearest point on the segment. \~
  \param[out] tRes - \ru Значение параметра ближайшей точки на линии.
                     \en Value of the nearest point parameter on the line. \~
  \return \ru Квадрат расстояния ближайшей точки до линии.
          \en Squared distance between the nearest point and the line. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC(float) LineToSegmentDistanceSquared( const MbFloatPoint3D &  seg_base, 
                                               const MbFloatPoint3D &  seg_end,
                                               const MbFloatAxis3D &   line,
                                                     MbFloatVector3D & vRes,
                                                     float &           tRes );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить квадрат расстояния от линии до полигона.
           \en Calculate squared distance from a line to a polygon. \~
  \details \ru При вычислении квадрата расстояния от линии до полигона проверяется расстояние от каждого 
    сегмента полигона до первого попадания в окрестность delta.
    Возвращается значение параметра ближайшей точки на линии tRes и квадрат расстояния 
    от этой точки до сегмента полигона. \n
           \en During calculation of squared distance from a line to a polygon the distance from each 
    segment of the polygon is checked until the first getting to 'delta' neighborhood.
    Returns the value of the nearest point parameter on tRes line and the squared distance 
    from this point to a segment of the polygon. \n \~
  \param[in] poly - \ru Тестируемый полигон.
                    \en Polygon to check. \~
  \param[in] line - \ru Линия, до которой вычисляется расстояние.
                    \en Line to calculate the distance to. \~
  \param[in] delta - \ru Радиус окрестности вокруг линии.
                     \en Neighborhood radius around the line. \~
  \param[out] nearestPoint - \ru Ближайшая к лучу точка.
                             \en The nearest point of the polygon. \~
  \param[out] tRes - \ru Значение параметра ближайшей точки линии.
                     \en The value of parameter of the nearest point on the line. \~
  \return \ru Квадрат расстояния ближайшей точки до линии.
          \en Squared distance between the nearest point and the line. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC(float) LineToPolyDistanceSquared( const MbPolygon3D &    poly,
                                            const MbFloatAxis3D &  line,
                                                  float            delta,
                                                  MbFloatPoint3D & nearestPoint,
                                                  float &          tRes );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить квадрат расстояния от линии до полигона.
           \en Calculate squared distance from a line to a polygon. \~
  \details \ru При вычислении квадрата расстояния от линии до полигона проверяется расстояние от каждого 
    сегмента полигона до первого попадания в окрестность delta.
    Возвращается значение параметра ближайшей точки на линии tRes и квадрат расстояния 
    от этой точки до сегмента полигона. \n
           \en During calculation of squared distance from a line to a polygon the distance from each 
    segment of the polygon is checked until the first getting to 'delta' neighborhood.
    Returns the value of the nearest point parameter on tRes line and the squared distance 
    from this point to a segment of the polygon. \n \~
  \param[in] poly - \ru Тестируемый полигон.
                    \en Polygon to check. \~
  \param[in] line - \ru Линия, до которой вычисляется расстояние.
                    \en Line to calculate the distance to. \~
  \param[in] delta - \ru Радиус окрестности вокруг линии.
                     \en Neighborhood radius around the line. \~
  \param[in] cutPlace - \ru Отсекающая плоскость.
                        \en Cutting plane. \~
  \param[out] nearestPoint - \ru Ближайшая к лучу точка.
                             \en The nearest point of the polygon. \~
  \param[out] tRes - \ru Значение параметра ближайшей точки линии.
                     \en The value of parameter of the nearest point on the line. \~
  \return \ru Квадрат расстояния ближайшей точки до линии.
          \en Squared distance between the nearest point and the line. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC(float) LineToPolyDistanceSquared( const MbPolygon3D &    poly,
                                            const MbFloatAxis3D &  line,
                                                  float            delta,
                                            const MbPlacement3D &  cutPlace,
                                                  MbFloatPoint3D & nearestPoint,
                                                  float &          tRes );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить квадрат расстояния от линии до полигона.
           \en Calculate squared distance from a line to a polygon. \~
  \details \ru При вычислении квадрата расстояния от линии до полигона проверяется расстояние от каждого 
    сегмента полигона до первого попадания в окрестность delta.
    Возвращается значение параметра ближайшей точки на линии tRes и квадрат расстояния 
    от этой точки до сегмента полигона. \n
           \en During calculation of squared distance from a line to a polygon the distance from each 
    segment of the polygon is checked until the first getting to 'delta' neighborhood.
    Returns the value of the nearest point parameter on tRes line and the squared distance 
    from this point to a segment of the polygon. \n \~
  \param[in] poly - \ru Тестируемый полигон.
                    \en Polygon to check. \~
  \param[in] line - \ru Линия, до которой вычисляется расстояние.
                    \en Line to calculate the distance to. \~
  \param[in] delta - \ru Радиус окрестности вокруг линии.
                     \en Neighborhood radius around the line. \~
  \param[in] cutPlaces - \ru Отсекающая плоскости.
                         \en Cutting planes. \~
  \param[out] nearestPoint - \ru Ближайшая к лучу точка.
                             \en The nearest point of the polygon. \~
  \param[out] tRes - \ru Значение параметра ближайшей точки линии.
                     \en The value of parameter of the nearest point on the line. \~
  \return \ru Квадрат расстояния ближайшей точки до линии.
          \en Squared distance between the nearest point and the line. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC(float) LineToPolyDistanceSquared( const MbPolygon3D &                poly,
                                            const MbFloatAxis3D &              line,
                                                  float                        delta,
                                            const std::vector<MbPlacement3D> & cutPlaces,
                                                  MbFloatPoint3D &             nearestPoint,
                                                  float &                      tRes );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить расстояние от линии до точки.
           \en Calculate the distance from a line to a point. \~
  \details \ru Для корректного вычисления расстояния от линии до точки вектор направления линии должен быть нормализован. \n
           \en For correct calculating of the distance from a line to a point the line direction vector must be normalized. \n \~
  \param[in] line - \ru Линия.
                    \en Line. \~
  \param[in] to - \ru Точка.
                  \en Point. \~
  \param[out] tRes - \ru Значение параметра ближайшей точки линии.
                     \en The value of parameter of the nearest point on the line. \~
  \return \ru Расстояние от точки до линии.
          \en The distance from a point to a line. \~
  \ingroup Algorithms_3D
*/
// ---
template <class Double, class Point, class Vector, class Axis>
Double LineToPointDistance( const Axis &   line,
                            const Point &  to, 
                                  Double & tRes )
{
  C3D_ASSERT( ::fabs(line.GetAxisZ().Length() - 1.0) < METRIC_ACCURACY );

  Vector vect( line.GetOrigin(), to );
  tRes = vect * line.GetAxisZ();
  return ( line.GetAxisZ() | vect ).Length();
}


//------------------------------------------------------------------------------
/** \brief \ru Вычислить расстояние от линии до точки.
           \en Calculate the distance from a line to a point. \~
  \details \ru Для корректного вычисления расстояния от линии до точки вектор направления линии должен быть нормализован. \n
           \en For correct calculating of the distance from a line to a point the line direction vector must be normalized. \n \~
  \param[in] line - \ru Линия.
                    \en Line. \~
  \param[in] to - \ru Точка.
                  \en Point. \~
  \param[in] cutPlace - \ru Отсекающая плоскость.
                        \en Cutting plane. \~
  \param[out] tRes - \ru Значение параметра ближайшей точки линии.
                     \en The value of parameter of the nearest point on the line. \~
  \return \ru Расстояние от точки до линии.
          \en The distance from a point to a line. \~
  \ingroup Algorithms_3D
*/
// ---
template <class Double, class Point, class Vector, class Axis, class Placement>
Double LineToPointDistance( const Axis &      line,
                            const Point &     to,
                            const Placement & cutPlace,
                                  Double &    tRes )
{
  C3D_ASSERT( ::fabs(line.GetAxisZ().Length() - 1.0) < METRIC_ACCURACY );
  C3D_ASSERT( cutPlace.IsNormal() );

  if ( cutPlace.PointRelative( to, METRIC_EPSILON ) != iloc_InItem ) { // Под плоскостью или на плоскости (Below plane or on plane)
    Vector vect( line.GetOrigin(), to );
    tRes = vect * line.GetAxisZ();
    return ( line.GetAxisZ() | vect ).Length();
  }

  tRes = FLT_MAX;
  return FLT_MAX;
}


#endif // __MESH_PRIMITIVE_H
