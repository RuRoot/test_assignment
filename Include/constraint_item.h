//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Двухмерные геометрические ограничения для объектов C3D-модели
         \en 2D-constraints between geometric objects of C3D-model. 
         \~
  \attention \ru Данный файл содержит типы данных и вызовы, предназначенные для тестирования
            и отладки, поэтому могут быть изменены или удалены из API C3D Kernel без 
            предупреждения. Для применения функциональности решателя двухмерных ограничений 
            рекомендуется реализация собственного модуля встраивания в приложение на 
            основе интерфейсов gce_api.h и gce_types.h.
           
           \en This file contains data types and calls for testing and debugging, so they 
           can be modified or removed from the C3D Kernel API without notice. To use 
           the 2D constraint solver, it is recommended to implement a custom embedding module
           based on th interfaces gce_api.h and gce_types.h.
           \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CONSTRAINT_ITEM_H
#define __CONSTRAINT_ITEM_H


#include <templ_sptr.h>
#include <reference_item.h>
#include <gce_types.h>
#include <mt_ref_item.h>
#include <io_tape.h>
#include <mb_cart_point.h>
#include <hash32.h>


//----------------------------------------------------------------------------------------
/// \ru Кодировка геометрического примитива \en Geometric primitive encoding
//---
struct GeomCode
{    
  enum Type
  {    
    // \ru Словарь примитивов плоскости (соответствует словарю решателя; типы геометрических подмножеств плоскости) \en Plane primitive dictionary (corresponds to the solver dictionary; types of geometric subsets of plane) 
    NULL_GEOM,    ///< \ru пустое геометрическое множество \en empty geometric set 
    POINT,        ///< \ru Точка: элемент плоскости \en Point: element of the plane 
    PROPER_POINT = POINT, ///< \ru Контрольная точка по индексу \en Control point by index 
    LINE,         ///< \ru Прямая \en Line 
    CIRCLE,
    ELLIPSE,
    SPLINE,
    PARAMETRIC,   ///< \ru Неподвижная параметрическая кривая \en Fixed parametric curve     
    
    // \ru Дифференциация подтипов точки \en Differentiation of subtypes of the point     
    FIRST_END,    ///< \ru Начальная точка кривой \en Start point of a curve 
    SECOND_END,   ///< \ru Конечная точка кривой \en End point of a curve 
    MIDDLE_POINT, ///< \ru Средняя точка кривой \en Middle point of a curve 
    CENTRE_POINT, ///< \ru Центральная точка эллипса \en Central point of an ellipse 
    SPLINE_POINT, ///< \ru Контрольная точка сплайна по индексу \en Control point of a spline by index 
    Q1_POINT,     ///< \ru Квадрантная точка на 3 ч \en Quadrant point at 3 o'clock 
    Q2_POINT,     ///< \ru Квадрантная точка на 12 ч \en Quadrant point at 12 o'clock 
    Q3_POINT,     ///< \ru Квадрантная точка на 9 ч \en Quadrant point at 9 o'clock 
    Q4_POINT,     ///< \ru Квадрантная точка на 6 ч \en Quadrant point at 6 o'clock 

    // \ru Размеры \en Sizes 
    /*
    LINEAR_DIM,
    ANGULAR_DIM,
    */
  };
  Type    type;   ///< \ru Тип геометрии объекта модели (из словаря типов, поддерживаемых решателем) \en Type of geometry of a model object (from dictionary of types supported by the solver) 
  size_t  index;  ///< \ru Номер примитива для данного объекта модели (кодируется в индивидуальных адаптерах) \en Number of a primitive for a given object of the model (encoded in individual adapters) 

  GeomCode( GeomCode::Type t ) : type(t),index(0) {}
  bool        operator != ( const GeomCode & g ) const { return type != g.type || index != g.index; }
  GeomCode &  operator = ( const Type & gType ) { type = gType; index = 0; return *this; }
};


//////////////////////////////////////////////////////////////////////////////////////////
//
/// \ru Аргумент ограничения (или геометрический примитив решателя) \en Argument of constraint (or geometric primitive of the solver) 
/**\ru Этот тип:
  1) Кодирует информацию о геометрии, которая является аргументом для ограничений;
  2) соответствует одному из примитивных типов словаря решателя
  
  Словарь примитивов решателя: точка, прямая, окружность, эллипс, сплайн.
   \en This type:
  1) Encodes the information about the geometry which is the argument for constraints;
  2) Corresponds to one of primitive types of the solver dictionary
  
  The solver primitives dictionary: point, line, circle, ellipse, spline. \~

*/
//////////////////////////////////////////////////////////////////////////////////////////

class MATH_CLASS GcArgument {
public:
  typedef MbRefItem * ParObject; ///< \ru Ассоциированный тип: владелец примитива, которого он содержит, как свою составную часть \en Associated type: primitives owner which contain the primitive as a component 

public:
  GeomCode          m_geom;    ///< \ru Тип геометрии объекта модели (из словаря типов, поддерживаемых решателем) \en Type of geometry of a model object (from dictionary of types supported by the solver) 
  c3d::RefItemSPtr  m_item;    ///< \ru Объект модели \en The model object 

public:
  GcArgument() : m_geom(GeomCode::NULL_GEOM), m_item() {}
  GcArgument( GeomCode type, MbRefItem & item ) : m_geom(type), m_item(&item) {}
  GcArgument( const GcArgument & ag ) : m_geom(ag.m_geom), m_item(ag.m_item) {}
  GcArgument &  operator  = ( const GcArgument &  g ) { m_geom = g.m_geom; m_item = g.m_item; return *this; }
  bool          operator != ( const GcArgument & g ) const { return m_item.get() != g.m_item.get() && m_geom != g.m_geom; }
};

//----------------------------------------------------------------------------------------
//
//---
inline GcArgument::ParObject Owner(       GcArgument & g ) { return g.m_item; }
inline GcArgument::ParObject Owner( const GcArgument & g ) { return g.m_item; }

//----------------------------------------------------------------------------------------
/// \ru Параметры размерного ограничения \en Parameters of size constraint 
//---
struct DimParameters
{
  double  dimValue;
  double  dirAngle; ///< \ru Угол для направленного размера \en Angle for a directed size 
};


//////////////////////////////////////////////////////////////////////////////////////////
//
/// \ru Элементарное ограничение \en Elementary constraint 
/**\ru Элементарное ограничение соответствует типам ограничений из словаря решателя и не более того. 
  Ограничения более сложных типов описываются набором классов MbConstraint.
   \en Elementary constraint corresponds to the types of constraints from the dictionary of the solver and nothing more. 
  Constraints of more complex types are described by a set of classes MbConstraint. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

class MATH_CLASS MbConstraint
{
public:  
  typedef GcArgument                    Argument;  ///< \ru Тип аргумента ограничения \en Type of argument of the constraint 
  typedef std::vector<Argument>         arg_list;
  typedef arg_list::const_iterator      arg_iter;
  typedef std::pair<arg_iter,arg_iter>  arg_iter_pair;
  static  const Argument                null_arg;

private:
  constraint_type m_type; ///< \ru Тип ограничения из словаря решателя \en Type of the argument of the solver dictionary 
  arg_list        m_args; ///< \ru Аргументы ограничения (примитивы из словаря решателя) \en Arguments of the constraint (primitives from the solver dictionary) 
  DimParameters   m_pars; ///< \ru Параметры размера (можно считать тоже аргументом, но представлен другим типом) \en Parameters of size (it can be considered as an argument but it is represented by another type) 

public:
  MbConstraint( constraint_type, const Argument &, const Argument & ); // \ru Бинарное ограничение \en Binary constraint 
  MbConstraint( const MbConstraint & );

public:
  const Argument &  GetGeom( size_t nb ) const { --nb; return nb < m_args.size() ? m_args[nb] : null_arg; }
  constraint_type   Type() const { return m_type; }
  arg_iter_pair     Arguments() const { return arg_iter_pair( m_args.begin(), m_args.end() ); }
  
  /*
  bool          operator < ( const MbConstraint & c ) const;
  bool          operator == ( const MbConstraint & c ) const;
  */

public:
  MbConstraint & operator = ( const MbConstraint & );
};


//----------------------------------------------------------------------------------------
/// \ru Формирователь модели в решателе \en Generator of a model in the solver 
//---
/* struct GcFormer
{
  // \ru Геометрические объекты \en Geometric objects
  virtual bool Point( const GcArgument & ) = 0;
  virtual bool Line( const GcArgument & ) = 0;
  virtual bool LineSeg( const GcArgument &, const GcArgument & ) = 0;
  virtual bool Circle( const MbRefItem &, const MbCartPoint &, double ) = 0;
  virtual bool Circle( const GcArgument & ) = 0;

  // \ru Геометрические ограничения \en Geometric constraints
  virtual bool Coincidence( const GcArgument &, const GcArgument & ) = 0;
  virtual bool Incidence( const GcArgument &, const GcArgument & ) = 0;
  virtual bool Vertical( const GcArgument &, const GcArgument & ) = 0;
  virtual bool Horizontal( const GcArgument &, const GcArgument & ) = 0;  

  // \ru Размерные ограничения \en Dimensional constraints
  virtual bool LinearDimension( const MbConstraint & ) = 0;
};
*/


//////////////////////////////////////////////////////////////////////////////////////////
//
/// \ru Ограничение модели \en Model constraints 
//
//////////////////////////////////////////////////////////////////////////////////////////

class MbConstraintItem: public TapeBase
                      , public MtRefItem
{
  MbConstraint  m_arg;

public:
  MbConstraintItem();  
  MbConstraintItem( const MbConstraint & c ) : MtRefItem(), m_arg(c) {}
  constraint_type       GceType() const { return m_arg.Type(); }
  const MbConstraint &  GceConstraint() const { return m_arg; }

  ClassDescriptor GetClassDescriptor( const VersionContainer & ) const override
  { 
    C3D_ASSERT_UNCONDITIONAL( false ); // Неполная реализация класса
    return ClassDescriptor( ::pureName(typeid(*this).name()), Math::DeprecatedMathID() );
  }
};


//////////////////////////////////////////////////////////////////////////////////////////
//
/// \ru Размерное ограничение \en Dimensional constraint 
//
//////////////////////////////////////////////////////////////////////////////////////////

class MbDimensional: public MbConstraintItem
{
  MbCartPoint legendPos;  ///< \ru Положение размерной надписи в ЛСК размера \en Position of a dimension legend in LCS of the dimension 

private:
  /// \ru Выдать ЛСК размера \en Get LCS of the dimension 
  virtual void GetPlacement( MbPlacement & ) const;
};

//////////////////////////////////////////////////////////////////////////////////////////
//
/// \ru Система геометрических ограничений \en Geometric constraints system 
//
//////////////////////////////////////////////////////////////////////////////////////////

class MATH_CLASS MbConstraintSystem2D
{  
  typedef SPtr<MbConstraintItem> ConstraintPtr;
  std::vector<ConstraintPtr> myConstraints;

public:
   MbConstraintSystem2D();
  ~MbConstraintSystem2D();

public:
  void AddConstraint( SPtr<MbConstraintItem> );
};


#endif // __CONSTRAINT_ITEM_H


// eof
