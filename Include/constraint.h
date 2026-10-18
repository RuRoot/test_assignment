//////////////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Геометрическое ограничение.
         \en Geometric constraint. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CONSTRAINT_H
#define __CONSTRAINT_H

#include <templ_sptr.h>
#include <reference_item.h>
#include <assembly.h>
#include <mesh.h>
#include <vector>

struct CNodeIterator;

//----------------------------------------------------------------------------------------
/** \brief \ru Аргумент геометрического ограничения.
           \en An argument of geometric constraint. \~
  \details \ru Аргумент ограничения - это любой объект модели, который участвует в геометрическом
               ограничении или размере. MtGeomArgument хранит ссылку на объект MbRefItem,
               принадлежащий элементу сборки (MbItem) и таким образом может адресовать любые
               геометрические объекты сождержащиеся в сборке (MbAssembly). Например, ограничение
               может указывать на грань тела MbSolid, которое в свою очередь принадлежит сборке (MbAssembly).\n
               Заметим, что тела в сборке обычно присутствуют в виде вставок MbInstance, и тогда аргумент
               ограничения хранит цепочку: Корневая сборка -> Вставка -> Тело -> Грань. \n
               Также MtGeomArgument учитывает, что ограничение может ссылаться на элементы модели,
               лежащие внутри подсборок рекурсивно.
           \en The constraint argument is any model object that connected to a geometric constraint
               or dimension. MtGeomArgument keeps a reference to the MbRefItem object belonging
               to the assembly item (MbItem) and thus can address any geometric objects
               contained in the root assembly (MbAssembly). For example, the constraint can point to
               the face of the MbSolid, which in turn belongs to the assembly (MbAssembly).\n
               Note that solids in the assembly are usually present in the form of MbInstance,
               and then the constraint argument stores the chain: Root assembly -> Instance -> Solid -> Face.\ n
               Also MtGeomArgument takes into account that the constraint can refer to model elements
               lying inside subassemblies recursively.\~
  \ingroup Model_Items
*/
//---
class MATH_CLASS MtGeomArgument
{
private:
  SPtr<const MbRefItem> propItem; ///< \ru Элемент объекта, непосредственно выбранный для связи. \en The element of geom object which constraints are connected to.
  SimpleName            propName; ///< \ru Имя связываемого элемента. \en Name of a connected element.
  SimpleName            hash;     ///< \ru Имя объекта сборки или подсборки, содержащего объект связи с ограничением. \en Hash code of the path from the root to the item.
  SPtr<const MbItem>    item;     ///< \ru Объект сборки или подсборки, содержащий объект связи с ограничением. \en Assembly object that hosts geom entity connected to constraint.
  const MbAssembly *    root;     ///< \ru Сборка, содержащая объект с ограничением. \en The assembly that hosts geom object with entity connected to constraint.

public:
  static const MtGeomArgument null; ///< \ru Пустой аргумент. \en An empty argument.

public:
  MtGeomArgument( const MbRefItem * p, const MbItem * h );
  MtGeomArgument( const MtGeomArgument & );
  MtGeomArgument()  : propItem( nullptr ), propName( c3d::UNDEFINED_SNAME )
                    , hash( c3d::UNDEFINED_SNAME ), item( nullptr ), root( nullptr ) {}

public:
  /** \brief \ru Получить непосредственный объект сборки, содержащий ссылочный объект.
             \en Get immediate object of the assembly containing the reference object.
      \param trans - \ru Матрица ссылочного объекта в системе координат непосредственного объекта сборки.
                   - \en Matrix from the reference object to the sub-item of the assembly. \~
  */
  const MbItem *  SubItemOf( const MbAssembly *, MbMatrix3D & trans ) const;
  /// \ru Объект геометрической модели, владеющий аргументом. \en Geometry model object which is a host of an argument.
  const MbItem *  HostItem() const { return item; }
  /// \ru Аргумент ограничения, заданный в ЛСК хозяина. \en Geometric constraint argument given in the host's LCS.
  const MbRefItem * PropItem() const { return propItem; }
  ///< \ru Выдать имя связываемого элемента. \en Get name of a connected element.
  SimpleName       GetPropName() const { return propName; }
  ///< \ru Выдать имя объекта сборки или подсборки, содержащего объект связи с ограничением. \en Get hash code of the path from the root to the item.
  SimpleName       GetHash() const { return hash; }
  ///< \ru Выдать сборку, содержащую объект с ограничением. \en Get assembly that hosts geom object with entity connected to constraint.
  const MbAssembly * GetRoot() const { return root; }
  /// \ru Выдать значение геометрии аргумента, заданное в ЛСК хозяина. \en Get geometric value of argument given in the host LCS.
  MtGeomVariant   PropGeom() const;
  /// \ru Выдать хэш-имя объекта. \en Get a hash name of the object.
  SimpleName      PropName() const;
  /// \ru Равны ли объекты? \en Are objects equal? \~
  bool            IsSame( const MtGeomArgument & r ) const { 
                      return ( (propItem == r.propItem) && (propName == r.propName) && (item == r.item) && (hash == r.hash) );
                    }
  /// \ru Равны ли ссылкпи на объект модели? \en Are the references to the model object equal? \~
  bool            IsSameItemReference( const MtGeomArgument & r ) const { // MtItemReference 
                      return ( (hash == r.hash) && (item == r.item) && (root == r.root) );
                    }
  /// \ru Оператор равенства объектов. \en Objects equality operator. \~
  bool             operator == ( const MtGeomArgument & ) const;
  /// \ru Оператор копирования. \en Copy operator. \~
  MtGeomArgument & operator = ( const MtGeomArgument & );

KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MtGeomArgument, MATH_FUNC_EX ) // Serializing into a file format
}; // MtGeomArgument


//----------------------------------------------------------------------------------------
/** \brief \ru Геометрическое ограничение.
           \en Geometric constraint. \~
  \details \ru Этот класс представляет все виды ограничений, включая геометрические и 
               размерные отношения между объектами модели.
           \en This class represents all kinds of constraints of assembly, including 
               geometrical and dimensional relationships between the model objects. \~
  \ingroup Model_Items
*/
//---
class MATH_CLASS MtGeomConstraint
{
private:
  const ItConstraintItem *    m_cItem;     ///< \ru Указатель на реализацию геометрического ограничения. \en Pointer to geometric constraint implementation. \~
  std::vector<MtGeomArgument> m_arguments; ///< \ru Аргументы геометрического ограничения. \en The arguments of geometric constraint. \~
  SPtr<const MbItem>          m_mesh;      ///< \ru Объект для демонстрации геометрического ограничения. \en The draw object of geometric constraint. \~

public:
  MtGeomConstraint( const MtGeomConstraint & );
 ~MtGeomConstraint(); 

public:
  /// \ru Возвращает true, если ограничение не действительно. \en Return true if the constraint is invalid.
  bool            IsNull() const { return m_cItem == nullptr; }
  /// \ru Тип сопряжения (геометрического ограничения). \en Type of geometric constraint. 
  MtMateType      ConstraintType() const;
  /// \ru Текущее значение размера. \en Current value of the dimension. 
  double          DimValue() const;
  /// \ru Создать полигональный объект для отображения геометрических ограничений. \en Create a polygonal object for visualization.
  bool            CreateMesh( const MbAssembly & assem, const MbStepData & stepData, const MbFormNote & note, double meshUnit, uint32 color );
  /// \ru Выдать указатель на реализацию геометрического ограничения. \en Get a pointer to geometric constraint implementation. \~
  const ItConstraintItem * ConstraintItem() const { return m_cItem; }
  /// \ru Выдать аргументы геометрического ограничения. \en Get arguments of geometric constraint. \~
  const std::vector<MtGeomArgument> & GeomArguments() const { return m_arguments; }
  /// \ru Выдать указатель на объект для демонстрации геометрического ограничения. \en Get a pointer to draw object of geometric constraint. \~
  const MbItem *  GetMesh() const { return m_mesh.get(); }
  /// \ru Оператор присваивания. \en The assignment operator.
  MtGeomConstraint & operator = (const MtGeomConstraint & );
  /// \ru Оператор равенства. \en The equality operator.
  bool               operator == ( const MtGeomConstraint & ) const;

protected:  
  friend class MbConstraintSystem;  
  friend class MtConstraintIter;  

  MtGeomConstraint( const ItConstraintItem * cItem, const MtGeomArgument & a1, const MtGeomArgument & a2 );  
  MtGeomConstraint( const MbConstraintSystem &, const ItConstraintItem * cItem );  
}; // MtGeomConstraint


//----------------------------------------------------------------------------------------
/// \ru Итератор обходящий ограничения сборки. \en Iterator traversing assembly constraints.
//---
class MATH_CLASS MtConstraintIter
{
private:
  CNodeIterator *            m_cIter;
  const MbConstraintSystem * m_gcSystem;

public: 
  MtConstraintIter();
  MtConstraintIter( const MtConstraintIter & );
  MtConstraintIter & operator = ( const MtConstraintIter & );
 ~MtConstraintIter();

public:
  MtGeomConstraint          Get() const;
  MtConstraintIter &        Set( const MbConstraintSystem *, CNodeIterator & );
  const MtConstraintIter &  Next();
  bool                      EqualTo( const MtConstraintIter & ) const;

public:
  //operator CNodeIterator& () { return *impl; }
  MtGeomConstraint operator*() const { return Get(); }
  // prefix operator
  const MtConstraintIter & operator++() { return Next(); }
  bool operator ==( const MtConstraintIter & iter ) const { return  EqualTo( iter ); }
  bool operator !=( const MtConstraintIter & iter ) const { return !EqualTo( iter ); }

}; // MtConstraintIter


//----------------------------------------------------------------------------------------
/// \ru Обработчик события, связанные с решением сборки. \en The event handles related to solving the assembly.
//---
struct MATH_CLASS ItAssemblyReactor
{  
public:
  /// \ru Захватить сборкой объект для дальнейшей работы. \en Capture the reactor instance by the assembly for further work.
  virtual void Capture( const MbAssembly * ) = 0;
  /// \ru Отпустить сборкой объект, прекратить работать с ним. \en Release this instance by the assembly, stop working with it.
  virtual void Release() = 0;
  /// \ru Геометрический решатель не пытался удовлетворить ограничение. \en This called when geometric solver failed to try for constraint satisfaction.
  virtual void EvaluationFailed( const MbAssembly * ) const {}
  /// \ru Геометрический решатель нашел новую позицию под-объекта сборки. \en The geometric solver found a new position of a constrained sub-object belonging the assembly.
  virtual void PositionChanged( const MbAssembly *, const MbItem * /*subItem*/ ) const {}
 
protected:
  ~ItAssemblyReactor() {}
}; // ItAssemblyReactor


//----------------------------------------------------------------------------------------
/** \brief \ru Интерфейс для системы ограничения импорта сборки из приложения. 
           \en The user defined interface for import constraint system of an assembly from CAD application.
*/
//---
struct MATH_CLASS ItAssemblyImportData
{
protected:
  ~ItAssemblyImportData() {}

public:
  /// \ru Импорт системы ограничений сборки. \en Import a constraint system of the assembly.
  virtual bool            ImportCSystem( const MbAssembly &, GCM_system & ) const = 0;
  /// \ru Получить дескриптор элемента сборки в системе ограничений. \en Get a descriptor of assembly sub-item which used in the constraint system.
  virtual MtGeomId        GeomId( const MbAssembly &, const MbItem * ) const = 0;
  /// \ru Получить объект модели, являющийся аргументом геометрического ограничения. \en Get the model object that is the argument of the geometric constraint.
  virtual MtGeomArgument  GeomSubItem( const MtArgument & ) const { return MtGeomArgument(); }
  /// \ru Получить объект модели, являющийся аргументом геометрического ограничения. \en Get the model object that is the argument of the geometric constraint.
  virtual MtGeomArgument  GeomSubItem( MtGeomId ) const { return MtGeomArgument(); }
}; //ItAssemblyImportData

//----------------------------------------------------------------------------------------
/// \ru Оператор равенства объектов. \en Objects equality operator. \~
//---
inline bool MtGeomArgument::operator == ( const MtGeomArgument & r ) const 
{
  if ( propItem != r.propItem )
    return false;
  if ( propName != r.propName )
    return false;
  if ( (item == r.item) && (hash == r.hash) )
    return true;
  if ( (hash == r.hash) && (root == r.root) ) 
    return true;
  return false;
}

//----------------------------------------------------------------------------------------
/// \ru Оператор копирования. \en Copy operator. \~
//---
inline MtGeomArgument & MtGeomArgument::operator = ( const MtGeomArgument & arg )
{ 
  propItem = arg.propItem;
  propName = arg.propName;
  hash = arg.hash;
  item = arg.item;
  root = arg.root;
  return *this; 
}

#endif // __CONSTRAINT_H
