//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Объектное API геометрического решателя в 3D.
         \en Object API of geometric solver in 3D. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GCM_MANAGER_H
#define __GCM_MANAGER_H

#include <math_define.h>
#include <mt_ref_item.h>
#include <gcm_res_code.h>
#include <gcm_geom.h>

struct ItConstraintItem;
struct ItPositionManager;
struct ItGCBlackbox;
struct GCM_c_params;

class  MbVector3D;
class  MbCartPoint3D;
class  MbPlacement3D;
class  MtAdjacentConstrsRange;
class  MtGeomSolver;
class  MtParGeom;

/**
  \addtogroup GCM_3D_ObjectAPI
  \{
*/

//---------------------------------------------------------------------------------------
/// \ru Состояние свободы тела. \en State of the solid freedom.
//---
typedef GCM_dof_result MtStateOfFreedom;

typedef GCM_reposition MtRepositionMode;

//----------------------------------------------------------------------------------------
/// \ru Неопределенное значение для некоторого типа. \en Undefined value of some datatype
//---
template<typename T>
struct GCM_undefined
{
private:
  static const int value = -1;
};
template<> struct GCM_undefined<GCM_result>
{
  static const GCM_result value = GCM_RESULT_None;
};
template<> struct GCM_undefined<GCM_tan_choice>
{
  static const GCM_tan_choice value = GCM_TAN_NONE;
};
template<> struct GCM_undefined<GCM_alignment>
{
  static const GCM_alignment value = GCM_NO_ALIGNMENT;
};
template<> struct GCM_undefined<GCM_angle_type>
{
  static const GCM_angle_type value = GCM_NONE_ANGLE;
};
template<> struct GCM_undefined<GCM_scale>
{
  static const GCM_scale value = GCM_NO_SCALE;
};
template<> struct GCM_undefined<GCM_dependency>
{
  static const GCM_dependency value = GCM_NO_DEPENDENCY;
};


//----------------------------------------------------------------------------------------
/** \brief \ru Числовой или перечислительный параметр ограничения.
           \en Numeric or enumerated parameter of constraint. \~
  \details \ru В зависимости от контекста тип #MtParVariant может трактоваться,
  как число с плавающей точкой или целочисленная величина (int, enum, bool).
           \en Depending on the context the type #MtParVariant can be treated 
  as a floating-point or integer (int, enum, bool). \~
*/
//---
class GCM_CLASS MtParVariant
{
public:
  static const MtParVariant undef;  ///< \ru Неопределенное значение. \en Undefined value.

public:
  MtParVariant() : enumVal( SYS_MAX_INT64-2 ), tag( GCM_ARG_UNDEFINED ) {} // \ru Неопределенное значение \en Undefined value
  MtParVariant( float val ) : numVal( static_cast<double>(val) ), tag( GCM_ARG_NUMBER ) {}
  MtParVariant( double val ) : numVal( val ), tag( GCM_ARG_NUMBER ) {}
  template< class _EnumType >
  MtParVariant( _EnumType val ) : enumVal( static_cast<ptrdiff_t>(val) ), tag( GCM_ARG_ENUM ) {}
  MtParVariant( const GCM_interval & interval ) : interVal( interval ), tag( GCM_ARG_INTERVAL ) {}

public:
  template< typename _EnumType >
  bool          GetEnum( _EnumType & ) const;
  template< typename _EnumType >
  _EnumType     AsEnum() const;
  double        AsNumber() const { C3D_ASSERT(GCM_ARG_NUMBER == tag); return numVal; }
  int           AsInteger() const { C3D_ASSERT(GCM_ARG_ENUM == tag); return static_cast<int>( enumVal ); }
  bool          GetInterval( GCM_interval & interval ) const;
  GCM_interval  AsInterval() const { C3D_ASSERT(GCM_ARG_INTERVAL == tag); return interVal; }
  bool          operator == ( const MtParVariant & ) const;
  bool          operator != ( const MtParVariant & v ) const { return !(v == *this); }

public:
  GCM_alignment AlignType() const;
  GCM_c_arg     CArg() const;  ///< \ru Выдать как аргумент ограничения. \en Give as an argument of constraint.

private:
  union
  {
    double       numVal;
    int64        enumVal;
    GCM_interval interVal;
  };
  GCM_argument_type tag;
};

//----------------------------------------------------------------------------------------
//
//---
template< typename _EnumType >
inline _EnumType MtParVariant::AsEnum() const 
{ 
  if ( GCM_ARG_UNDEFINED == tag )
  {
    return GCM_undefined<_EnumType>::value;
  }
  C3D_ASSERT( GCM_ARG_ENUM == tag );
  return static_cast<_EnumType>( enumVal ); 
}

//----------------------------------------------------------------------------------------
//
//---
template< typename _EnumType >
bool MtParVariant::GetEnum( _EnumType & val ) const
{  
  if ( GCM_ARG_ENUM == tag )
  {
    val = AsEnum<_EnumType>(); 
    return true;
  }
  return false;  
}

//----------------------------------------------------------------------------------------
//
//---
inline bool MtParVariant::operator == ( const MtParVariant & var ) const
{
  if ( tag == var.tag )
  {
    return ( (numVal == var.numVal) || (enumVal == var.enumVal) );
  }
  return false;
}

//----------------------------------------------------------------------------------------
//
//---
inline GCM_alignment MtParVariant::AlignType() const
{
  if ( *this == undef )
    return GCM_NO_ALIGNMENT;

  if ( (GCM_ARG_ENUM == tag) && (GCM_MIN_ALIGNMENT <= enumVal && enumVal < GCM_MAX_ALIGNMENT) )
  {
    return AsEnum<GCM_alignment>();
  }
  C3D_ASSERT_UNCONDITIONAL( false );
  return GCM_NO_ALIGNMENT;
}

//----------------------------------------------------------------------------------------
//
//---
inline GCM_c_arg MtParVariant::CArg() const
{
  GCM_c_arg cArg;  
  switch( tag )
  {
    case GCM_ARG_ENUM:
      cArg = enumVal;
      break;
    case GCM_ARG_NUMBER:
      cArg = numVal;
      break;
    case GCM_ARG_INTERVAL:
      cArg = interVal;
      break;
    case GCM_ARG_UNDEFINED:
      cArg = GCM_NULL;
      break;
    default:
      C3D_ASSERT_UNCONDITIONAL( false );
  }
  return cArg;
}

//----------------------------------------------------------------------------------------
/** \brief \ru Геометрический аргумент ограничения.
           \en Geometric argument of constraint. \~
*/
//---
class GCM_CLASS MtArgument
{
public:  
  /// \ru Конструктор аргумента, как геометрический объект. \en Constructor of argument as geometric object.
  MtArgument();
  MtArgument(const MtArgument&);
  /** \brief  \ru Конструктор аргумента, как геометрический объект.
              \en Constructor of argument as geometric object. \~
  */
  MtArgument(ItGeom*);

  /** \brief \ru Конструктор аргумента, как "геометрический объект в кластере".
             \en Constructor of argument as "geometric object in the cluster". \~
    \param[in]  cluster - \ru Кластер, как геометрически-жесткое объединение.
                          \en Cluster as geometrically rigid union. \~
    \param[in]  refGeom - \ru Геометрический объект, заданный в ЛСК кластера.
                          \en Geometric object given in the cluster LCS. \~
    \return \ru Аргумент геометрического ограничения.
            \en Argument of geometric constraint. \~

      \details \ru Аргумент, заданный этим способом, предполагает, что геометрический 
    объект refGeom, задан в ЛСК кластера. При этом в системе ограничений
    решение ищется для кластера, в то время, как refGeom считается неподвижно 
    заданным в ЛСК кластера.
               \en Argument given by this method assumes that the geometric 
    object refGeom is given in the cluster LCS. In this case in system of constraints
    the solution is sought for the cluster and refGeom is considered fixed  
    in the cluster LCS. \~ 

      \par \ru Аргумент, как матрица трансформации.
      Аргумент, созданный данным методом, можно интерпретировать, как матрицу трансформации, 
    заданную в виде произведения: G*T, где T - матрица кластера, переменная задачи ограничений, 
    G - матрица объекта refGeom, константа. Заметим, что если G = I - единичная матрица, 
    то геометрический объект имеет "стандартное положение".
           \en Argument as transformation matrix.
      Argument created by this method can be interpreted as a matrix
    of transformation given as multiplication: G*T, where T is cluster matrix, 
    variable of constraints, G - matrix of object refGeom, constant. 
    Note that if G = I - the identity matrix, then a geometric object 
    has "standard position". \~
  */  
  MtArgument(ItGeomPtr cluster, MtGeomVariant refGeom);
 ~MtArgument();
 
public:
  /// \ru Геометрический объект, вычисляемый в системе ограничений. \en Geometric object calculated in the constraint system.
  ItGeomPtr     Geom() const { return m_geom; }
  /** \brief  \ru Геометрическое значение аргумента, заданное в ЛСК объекта ItGeom * Geom(). 
              \en Geometric value of the argument specified in the LCS of the object ItGeom * Geom().
  */
  MtGeomVariant SubGeom() const;  
  /// \ru Тип геометрии. \en Geometry type.
  MtGeomType    GeomType() const;
  /// \ru Простой аргумент - не задан, как часть кластера. \en A simple argument, i.e. is not given as part of a cluster. \~
  bool          Simple() const;
  /// \ru Выдать положение геометрического объекта в виде ЛСК. \en Get a position of geometric object as its LSC.
  void          GetPlacement( MbPlacement3D & ) const;
  /// \ru Оператор присваивания. \en Assignment operator. 
  MtArgument&   operator = ( const MtArgument & );
  /// \ru Оператор равенства. \en Equality operator.
  inline bool   operator == ( const ItGeom * ) const;

public:
  MtGeomVariant RefGeom() const;  // deprecated. use SubGeom().

private:
  SPtr<ItGeom>  m_geom;     // Geometric object of the constraint system (often, it is a rigid body).
  MtGeomVariant m_subGeom;  // Geometric object given in the vNode's LCS.
};

//----------------------------------------------------------------------------------------
// \ru Оператор равенства. \en Equality operator.
//---
inline bool MtArgument::operator == ( const ItGeom * geom ) const
{
  if ( m_geom == geom )
  {
    return Simple();
  }
  return false;
}

class MtConstraintSystem;

//----------------------------------------------------------------------------------------
//
// ---
struct GCM_CLASS ItConstraintsEnum : public MtRefItem
{
  virtual const ItConstraintItem * GetDataAndGo() = 0;
  virtual void                     Restart() = 0;
};

class MtConstraintManager;  // Internal implementation of 3D solver.
class MtBlackboxManager;    // Internal implementation of Blackbox manager.

//----------------------------------------------------------------------------------------
/** \brief \ru Геометрический решатель.
           \en Geometric constraint solver. \~
  \details \ru Интерфейс геометрического решателя. Клиентское приложение может работать
  с любым количеством систем ограничений, для каждой из них заводится по одному экземпляру
  решателя с помощью  вызова #GCM_CreateSolver.
           \en Interface of geometric solver. Client application can run any number
  of constraint systems, for each of them is put one copy of the solver 
  by calling #GCM_CreateSolver. \~
  
  \ingroup GCM_3D_ObjectAPI
*/
//---
class GCM_CLASS MtGeomSolver: public MtRefItem
{
    /**
      \ru \name Функции задания системы сопряжений.
      \en \name Definition functions of the constraint system.
      \{
    */
public:
    /// \ru Добавить паттерн. \en Add a pattern.
    MtPatternId     AddPattern( MtMateType, MtArgument, MtArgument, MtParVariant par=MtParVariant::undef );
    /// \ru \ru Задать координату паттерна - Система Координат. \en Create the constraint coordinate of the coordinate system pattern.
    MtPatternId     SubPattern( MtPatternId ptrn, GCM_coord_name, MtArgument axis );
    /// \ru Добавить геометрический объект в паттерн. \en Add a geometric object to the pattern.
    MtConstraintId  AddGeomToPattern( MtPatternId ptrn, MtArgument ptrnObj, MtParVariant par1 = MtParVariant::undef, 
                                      MtParVariant par2 = MtParVariant::undef, GCM_scale scale=GCM_RIGID );
    /**
      \brief \ru Добавить ограничение. 
             \en Add constraint.
      \return \ru Вернет GCM_RESULT_Ok, если ограничение успешно добавлено, 
                  в противном случае вернет код ошибки.
              \en Returns GCM_RESULT_Ok if the constraint is successfully added, 
                  otherwise it will return an error code.
    */
    MtResultCode3D  AddConstraintItem( ItConstraintItem & );

    /** \brief \ru Добавить сопряжение для пары геометрических объектов(ограничение).
               \en Add mate (constraint) of two geometric objects. \~
      \param[in]  t - \ru Тип геометрического ограничения.
                      \en Type of geometric constraint. \~
      \param[in]  g1, g2 - \ru Пара геометрических объектов - аргументы ограничения.
                           \en Pair of geometric objects - arguments of constraint. \~ 
      \param[in]  p1 - \ru Условие выравнивания для таких типов ограничений, как GCM_COINCIDENT, 
                       GCM_PARALLEL, GCM_PERPENDICULAR, GCM_CONCENTRIC, GCM_IN_PLACE,
                       GCM_TANGENT. Значения данного параметра берутся из #GCM_alignment.
                       \en Condition of alignment for or these types of constraints like GCM_COINCIDENT, 
                       GCM_PARALLEL, GCM_PERPENDICULAR, GCM_CONCENTRIC, GCM_IN_PLACE,
                       mct_Tangency. Values of this parameter are taken from #GCM_alignment. \~
      \param[in]  p1 - \ru Числовой параметр (double) размерных ограничений с типами GCM_ANGLE, GCM_DISTANCE.
                       \en Numeric parameter (double) of dimensional constraints to the types of GCM_ANGLE, GCM_DISTANCE. \~
      \param[in]  p2 - \ru  Вариант касания #GCM_tan_choice для ограничения типа GCM_TANGENCY 
                            или значение #GCM_alignment для размерных ограничений.
                       \en  Tangency variant #GCM_tan_choice for constraint with type GCM_TANGENCY 
                            or value #GCM_alignment for dimensional constraint. \~
      \param[in]  p3 - \ru Не имеет значения.
                       \en Irrelevant. \~    
      \return \ru Геометрическое ограничение. \en Geometric constraint. \~
    */
    ItConstraintItem *  AddConstraint ( MtMateType t, MtArgument g1, MtArgument g2
                                      , MtParVariant p1 = MtParVariant::undef
                                      , MtParVariant p2 = MtParVariant::undef
                                      , MtParVariant p3 = MtParVariant::undef );
    /** \brief \ru Добавить ограничение для тройки геометрических объектов.
               \en Add constraint of three geometric objects. \~
    */
    ItConstraintItem *  AddConstraint ( MtMateType, MtArgument, MtArgument, MtArgument,
                                        MtParVariant p1 = MtParVariant::undef, MtParVariant p2 = MtParVariant::undef );    

    /** \brief \ru Добавить черный ящик в систему ограничений.
               \en Add black box to the constraint system. \~
      \param[in]  bBox  - \ru Интерфейс чёрного ящика.
                          \en Interface of Interface black box. \~
      \return \ru Код результата. \en Result code. \~      
    */  
    /// \ru Добавить геометрический объект. \en Add the geometric object. 
    ItGeomPtr       AddGeom( MtGeomVariant );

    /** \brief \ru Перенести геометрический объект системы ограничений в подсистему твврдого тела
                   (сделать подчиненным объектом жесткого кластера).
               \en Move a geometric object of a constraint system into a solid's subsystem
                   (make it a subobject of a rigid cluster). \~
      \param[in] g     - \ru Геометрический объект.
                         \en Geometric object. \~
      \param[in] tgtCl - \ru Целевой кластер.
                         \en Target cluster. \~
      \return \ru Код результата операции.
              \en Resulting code of operation. \~
      \details \ru Для переноса геометрического объекта в глобальную систему
                   необходимо в качестве целевого кластера указать `nullptr`.
                   В случае успеха функция возвращает код GCM_RESULT_Ok.
                   Если геометрический объект относится к #MtGeomSolver::Ground()
                   или был использован как аргумент при вызове метода #MtGeomSolver::CreateCluster,
                   то функция оставляет иерархию системы без изменений
                   и возвращает специальный код GCM_RESULT_ImmovableGeom.
                   Если целевой кластер был использован как аргумент при вызове метода #MtGeomSolver::CreateCluster,
                   то функция оставляет иерархию системы без изменений
                   и возвращает специальный код GCM_RESULT_UnacceptableCluster.
                   Если целевой кластер вложен в геометрический объект (в том числе совпадает с ним),
                   то функция оставляет иерархию системы без изменений
                   и возвращает специальный код GCM_RESULT_CyclicMove.
               \en In order to move the geometric object into the global system,
                   the target cluster should be pointed as `nullptr`.
                   On success, the function returns code GCM_RESULT_Ok.
                   If the geometric object relates to #MtGeomSolver::Ground()
                   or has been used as an argument to call method #MtGeomSolver::CreateCluster,
                   then the function leaves the system hierarchy unchanged
                   and returns special code GCM_RESULT_ImmovableGeom.
                   If the target cluster has been used as an argument to call method #MtGeomSolver::CreateCluster,
                   then the function leaves the system hierarchy unchanged
                   and returns special code GCM_RESULT_UnacceptableCluster.
                   If the target cluster is nested into the geometric object (including when it is the same one),
                   then the function leaves the system hierarchy unchanged
                   and returns special code GCM_RESULT_CyclicMove.
      \note \ru При удалении кластера методом #MtGeomSolver::RemoveGeom
                все перенесенные в него геометрические объекты автоматически переносятся
                в его надкластер, если такой имеется, иначе -- в глобальную систему.
            \en On removal of a cluster by method #MtGeomSolver::RemoveGeom,
                all the geometric objects moved into it automatically move into
                its supercluster if such one is present, and to the global system otherwise.
    */
    MtResultCode3D  MoveGeom(ItGeom& g, ItGeomPtr tgtCl);
    // Not yet documented
    MtResultCode3D  AddBlackbox(ItGCBlackbox&);
    /** \brief \ru Выдать зависимый объект данного ограничения. \ru Get a dependent object of constraint.      
        \details  \ru Зависимый объект определен естественным образом для ограничения типа `GCM_DEPENDENT` 
                      или назначается с помощью вызова #MtGeomSolver::SetDependent.
                  \en The dependent object is defined naturally for the `GCM_DEPENDENT` constraint 
                      or is assigned using a #MtGeomSolver::SetDependent call.      
    */
    ItGeomPtr       DependentGeom(ItConstraintItem*);
    /// \ru Задать для данного ограничения зависимый объект. \ru Set a dependent object of constraint.
    ItGeomPtr       SetDependentGeom(ItConstraintItem*, ItGeomPtr);
    /// \ru Сообщить об изменении данных, определяющих сопряжение. \en Report about change in the data defining conjugation. 
    MtResultCode3D  ChangeDefinition( ItConstraintItem & );
    /// \ru Сообщить об изменении вещественного параметра размерного сопряжения. \en Report about change of float-parameter of dimensional conjugation. 
    MtResultCode3D  ChangeDimension( ItConstraintItem & );
    /** 
      \brief  \ru Сообщить о изменении положения сопрягаемых тел.
              \en Report about position change of conjugated solids. \~
      \return \ru Код результата. \en Result code. \~
      \details  \ru Через эту функцию осуществляется синхронизация положения 
      объектов системы ограничений по состоянию объектов стороне клиентского 
      приложения. Надо сказать, что обновление положений происходит только для тех 
      наследников #ItGeom, которые реализованы на стороне приложения. Состояние 
      объектов, добавленных методом #MtGeomSolver::AddGeom остается неизменным.
                \en Using this function implements synchronization of position 
      of objects of constraints as objects of a client 
      application. It should be said that the update of the positions happens only for those 
      inheritors of #ItGeom which are implemented by the application. State 
      of objects added by method #MtGeomSolver::AddGeom remains unchanged. \~
    */
    MtResultCode3D  ChangeGeomPositions();
    /** 
      \brief \ru Создать пользовательский кластер.
             \en Create an user-defined cluster.
      \note \ru Объекты объединяемого множества не должны иметь ограничений на момент вызова. 
                Создавайте кластер перед добавлением ограничений.
            \en The united objects should not have constraints at the moment of the call.
                Create a cluster before adding constraints.
    */
    ItGeom *        CreateCluster( std::vector<ItGeom *> & );
    /**
      \brief \ru Заморозить геометрический объект, сделав неподвижным в ЛСК надкластера (или ГСК, если последний отсутствует).
             \en Freeze a geometric object by making it fixed in the LCS of the supercluster (or the WCS if the latter is not present).
      \warning \ru Устарел. Использовать `FreezeGeom` как замену.
               \en Deprecated. Use `FreezeGeom` instead.
    */
    MtResultCode3D  FixGeom( const ItGeom * );
    /**
      \brief \ru Заморозить геометрический объект, сделав неподвижным в ЛСК надкластера (или ГСК, если последний отсутствует).
             \en Freeze a geometric object by making it fixed in the LCS of the supercluster (or the WCS if the latter is not present).
    */
    MtResultCode3D  FreezeGeom( const ItGeom * );
    /// \ru Узнать зафиксирован ли геометрический объект? \en Check if a geometric object is fixed?
    bool            IsFixed( const ItGeom * );
    /// \ru Очистить систему ограничений. \en Clear the system constraint. 
    void            Flush();
    /// \ru Удалить из системы ограничений все черные ящики. \en Remove all the black boxes from the constraint system. 
    MtResultCode3D  RemoveAllBlackboxes();
    /// \ru Удалить ограничение из системы. \en Remove constraint from system. 
    MtResultCode3D  RemoveConstraint( ItConstraintItem * );
    /// \ru Удалить чёрный ящик из системы ограничений. \en Remove black box from constraint system. 
    MtResultCode3D  RemoveBlackbox( ItGCBlackbox & );
    /// \ru Удалить геометрический объект из системы ограничений. \en Remove geometric object from constraint system. 
    bool            RemoveGeom( const ItGeom * );
    /** 
      \brief \ru Освободить замороженный геометрический объект.
             \en Free a frozen geometric object.
      \warning \ru Устарел. Использовать `FreeGeom` как замену.
               \en Deprecated. Use `FreeGeom` instead.
    */
    MtResultCode3D  UnfixGeom( const ItGeom & );
    /** 
      \brief \ru Освободить замороженный геометрический объект.
             \en Free a frozen geometric object.
    */
    MtResultCode3D  FreeGeom( const ItGeom & );

    /**
      \}
      \ru \name Функции решения и диагностики
      \en \name Functions of solution and diagnostics
      \{
    */
public:
    /// \ru Оценить наилучшее приближение параметров, удовлетворяющих сопряжение. \en The function evaluates a closest parameters that tend to satisfy dimension or constraint.
    GCM_closest_params ClosestParams( const ItConstraintItem * ) const;
    /// \ru Узнать удовлетворено ли ограничение? \en Check if constraint is satisfied? 
    bool            IsSatisfied( const ItConstraintItem & );
    /// \ru Узнать является ли тело полно-заданным или фиксированным ? \en Check if solid is fully-specified or fixed? 
    GCM_dof_result  IsWellConstrained( const ItGeom & );  
    /** 
      \brief  \ru Разослать диагностические коды ограничениям.
              \en Send out diagnostic codes to constraints. \~
      \details  \ru Осуществляется решение системы ограничений. Если она частично не решена, 
      то по результатам решения рассылаются диагностические коды ошибок всем ограничениям. 
      Функция не тратит существенного времени, если  систему ограничений до этого уже пытались решать.
                \en Solving the constraint system. If it is not solved partly, then by the 
      results of the solution diagnostic codes of errors are sent to all the constraints. 
      The function doesn't have a substantial amount of time if have already tried to 
      solve the constraint system before. \~
    */
    void DiagnoseConstraints();
    /**
      \brief    \ru Решить систему сопряжений. \en Solve the constraint system. \~
      \details  \ru Меняет положение геометрических объектов в соответствии с геометрическими ограничениями.
                \en Changes positions of geometric objects according to geometric constraints. \~
    */
    MtResultCode3D    Evaluate();
    /// \ru Текущее или вычисленное положение геометрического объекта \en Current or calculated position of a geometric object 
    MbMatrix3D        TransMatrix(ItGeomPtr) const;
    /**
      \brief \ru Получить кластер, в котором содержится данный геометрический объект. 
             \en Get cluster which contains a given geometric object. \~
    */
    const ItGeom  *   Cluster( const ItGeom * subGeom ) const;

    /**
      \}
      \ru \name Функции для интерактивной манипуляции системой сопряжений
      \en \name Functions for interactive manipulation of the constraint system
      \{
    */
public:
  /// \ru Завершить режим "перетаскивания". \en Finish the dragging mode. 
  void           FinishReposition();
  /** \brief  \ru Выдать объект манипуляции, с которым работает решатель, находясь в режиме вращения/перемещения объектом (драггинг).
              \en Get manipulation object. Solver works with it when being in the dragging mode (rotating or moving).
  */
  ItGeom *       GetMovingGeom() const;
  /** 
    \brief  \ru Инициализировать режим перетаскивания объектов в плоскости экрана. 
            \en Initialize mode of object moving in the screen plane.
    \param movGeom   - \ru Компонент, деталь, которой манипулируют.
                       \en Component, part which is manipulated. \~
    \param projPlane - \ru Плоскость экрана, заданная в ГСК сборки.
                       \en Plane of the screen given in the WCS of assembly. \~
    \param curPnt    - \ru Точка, принадлежащая компоненту, которая проецируется на плоскость 
                          экрана в положение курсора, и за которую осуществляется 'перетаскивание'. 
                          curPnt задана в ЛСК геом.объекта movGeom.
                       \en Point of the component which is projected onto plane of the screen to 
                          cursor position and is 'dragging'. curPnt given in the LCS of 
                          the geometric object movGeom; \~
      \return \ru Код результата. \en Result code. \~      

      \details  
        \ru Функция запускается однократно перед входом в режим перетаскивания компонент, 
      который управляется (по движению мыши) через команду 
      MtResultCode3D SolveReposition( const MbCartPoint3D & ). Режим прекращается вызовом 
      любой иной команды, кроме этих двух; Также есть специальная функция для выхода из 
      режима "перетаскивания" - void FinishReposition(), для явного сбрасывания режима перемещения.
        \en The function runs once before running the dragging mode of components, 
      which is controlled (by movement of the mouse) by the command 
      MtResultCode3D SolveReposition( const MbCartPoint3D & ). Mode is stopped calling any other 
      command other than these two. There is also the special feature. To exit from 
      the dragging mode - void FinishReposition(), for explicit stop of the dragging mode. \~ 
  */
  MtResultCode3D PrepareReposition( ItGeom & movGeom, const MbPlacement3D & projPlane, const MbCartPoint3D & curPnt );
  /** \brief  \ru Инициализировать режим вращения компонента вокруг фиксированной оси. 
              \en Initialize mode of component rotation around a fixed point of axis.      

  \param geom  - \ru Геометрический объект, на которое направлено воздействие.
                  \en The geom object on which is directed at impact. \~
  \param org, axis - \ru Точка и вектор в ГСК, которые задают постоянную ось вращения.
                      \en Point and vector in the GCS which define constant axis of rotation. \~
  \return \ru Код ошибки, перечисленный enum #MtResultCode3D
          \en Error code, enum #MtResultCode3D \~
    \par
    \ru Функция запускается однократно перед входом в режим вращения, который управляется 
  через команду MtResultCode3D SolveReposition( double alpha ), где alpha - входной параметр,
  определяющий угловое положение компонента, и заданный в радианах. Режим прекращается вызовом
  void FinishReposition().
    \en The function runs once before running the rotation mode which is driven 
  by command MtResultCode3D SolveReposition( double alpha ), where "alpha" - input parameter,
  defines the angular position of the component and given in radians. Mode is stopped by 
  calling void FinishReposition(). \~
  */
  MtResultCode3D  PrepareReposition( ItGeom & rotGeom, const MbCartPoint3D & org, const MbVector3D & axis );
  /**
    \brief  \ru Решить систему для произвольного изменения положения одного тела.
            \en Solve the system for an arbitrary change of position of one solid. \~
    \param  \ru gItem    тело, положение которого меняется;
            \en gItem    solid, the position of which is changed; \~
    \param  \ru newPos   новое пололожение тела g_item;
            \en newPos   new position of solid g_item; \~
    \param  \ru movType  код желаемого поведения
            \en movType  code of the desired behavior \~
    \return \ru Код результата. \en Result code. \~
              
    \note \ru Эта функция не позволяет вывести систему сопряжений из состояния решаемости, 
    кроме случаев, когда до вызова функции система уже находилась в нерешенном состоянии. 
    Если новое положение 'newPos' не позволяет удовлетворять системе сопряжений, то новое 
    положение тела окажется наиболее близким к newPos (при сохранении решаемости).
          \en This function doesn't allow to take out constraint system from decided state, 
    except when before call of function the system was already unsolved.  If new  position 
    'newPos' doesn't allow to satisfy the system of constraints, then new position of solid 
    will be the most nearest to newPos (while preserving solvability). \~
  */
  MtResultCode3D  SolveReposition( ItGeom & gItem, const MbPlacement3D & newPos, MtRepositionMode movType );
  /**
    \brief  \ru Решить систему сопряжений для новой позиции курсора в режиме драггинга. 
            \en Solve the system of conjugations for new position of cursor in the dragging mode.
    \param -  \ru curXYZ текущее положение курсора в ГСК.
              \en curXYZ the current position of cursor in the WCS. \~
    \return \ru Код результата. \en Result code. \~

    \details  \ru Рабочая процедура, управляющая режимом перетаскивания, который прекращается 
                  вызовом  любой иной команды, например, добавить новое ограничение или перестроить.
              \en Work procedure which controls dragging mode which are stopped after call
                  any other command. For example: add a new constraint or rebuild. \~
  */
  MtResultCode3D  SolveReposition( const MbCartPoint3D & curXYZ );    
  /**
    \brief  \ru Решить систему с изменением положения компонента через одну координату. 
            \en Solve the system with the position of the component through one coordinate. \~
    
    \param alpha - \ru Управляющий параметр (зачастую задается в радианах).
                   \en Driving parameter (often, it's in radians ). \~
    \return \ru Код результата. \en Result code. \~

    \details  \ru Это рабочая функция, управляющая режимом перепозиционирования, 
    в котором положение тела управляется изменением одной координаты, например, угол 
    вращения вокруг оси. Режим прекращается вызовом #MtGeomSolver::FinishReposition или любой 
    иной командой, меняющей состояние решетеля, например, #MtGeomSolver::AddConstraint.
    Функция для больших систем работает значительно быстрее, чем Solve(bool) или 
    SolveReposition( ItGeom &, const MbPlacement3D &).
              \en This is the work function controls reposition mode, in which the position of 
    the solid is controlled by changing one coordinate. For example the angle of rotation 
    around an axis. Mode is stopped by calling #MtGeomSolver::FinishReposition or any 
    other command, which is changes state of the solver. 
    For example #MtGeomSolver::AddConstraint. Function for large systems is faster than 
    the Solve(bool) or SolveReposition( ItGeom &, const MbPlacement3D &). \~
  */
  MtResultCode3D SolveReposition( double alpha );

/**
    \brief  \ru Перетащить геометрический объект системы ограничений посредством динамического репозиционирования его манипулятора.
            \en Drag a geometric object within a system of constraints by repositioning dynamically its manipulator. \~
    \param[in] g     - \ru Перетаскиваемый геометрический объект.
                       \en Geometric object subject to dragging. \~
    \param[in] manip - \ru Изначальное или целевое ГСК-положение манипулятора.
                       \en Initial or target WCS-placement of manipulator. \~
    \param[in] mode  - \ru Режим перетаскивания.
                       \en Dragging mode. \~
    \return  \ru Код результата.
             \en Result code. \~

    \details  \ru Процедура предназначена для серийных вызовов, динамически перетаскивающих геометрический объект.
                  Манипулятор связывается с объектом при первом вызове и приводит его в движение при последующих. \~
              \en The procedure is intended for serial calls which drag the geometric object in a dynamic manner.
                  The manipulator is bound to the object on the first call and drives it into motion on subsequent ones. \~

    \note  \ru Функционал в разработке.
               Текущая реализация поддерживает все представленные варианты влияния (#GCM_drag_influence)
               и аннотированные ниже значения управления (#GCM_drag_control) в зависимости от опции движения (#GCM_drag_motion).
           \en The feauture is under development.
               The current implementation supports all the presented influence variants (#GCM_drag_influence)
               and the below-noted control values in dependence on the motion option (#GCM_drag_motion). \~

    \note  #GCM_DRAGGING_Translation:   #GCM_DRAGGING_Adaptive, #GCM_DRAGGING_Specific \n
           #GCM_DRAGGING_TranslationXY: #GCM_DRAGGING_Adaptive

    \note  \ru Во избежание неожиданного поведения целевое положение манипулятора между последовательными вызовами
               рекомендуется изменять соответственно заказанному виду движения.
           \en To avoid unexpected behavior, it is recommended to change the target placement of the manipulator
               between successive calls in accordance with the ordered kind of motion. \~
*/
  MtResultCode3D DynamicReposition( ItGeom & g, const MbPlacement3D & manip, const GCM_dragging & mode );

public:
    /**
      \}
      \ru \name Вспомогательные функции и запросы.
      \en \name Auxiliary functions and queries.
      \{
    */

    /** \brief \ru Узнать принадлежит ли системе ограничений геометрический объект. 
               \en Check if geometric object belongs to the system. \~ 
    */
    bool IsMyGeom( const ItGeom & ) const;

    /** \brief  \ru Узнать принадлежит ли системе ограничений геометрическое ограничение. 
                \en Check if geometric constraint belongs to the system.
    */
    bool IsMyConstraint( const ItConstraintItem & ) const;

    /** \brief \ru Выдать кластер неподвижных объектов, заданных в глобальной системой координат.
               \en Get the cluster of rigid subset of objects which are given in global coordinate system. \~    
    */    
    ItGeomPtr Ground() const;

    /** \brief \ru Выдать систему геометрических ограничений, которую обслуживает решатель.
               \en Get a geometric constraint system, which is served by the solver. \~
    */    
    GCM_system  System() const;

    // Not yet documented
    void        WriteSystem( TCHAR * fileName );
    /// \ru Выдать ограничения. \en Get the constraints iterator.
    SPtr<ItConstraintsEnum> GetConstraintsEnum();

    /** \brief \ru Получить диапазон смежных ограничений геометрического объекта.
               \en Get the range of adjacent constraints of a geometric object. \~
    */
    MtAdjacentConstrsRange AdjacentConstraints( const ItGeom * ) const;

public:

  // Internal use only
  GCM_geom       _QueryArgument( const MtArgument & gArg );

protected:
  const ItGeom * _SetDependencyGeom( MtGeomId gId, const ItGeom * gItem );

private:
  GCM_geom       _PrepareGeom( const ItGeom * g );

private:
  // Constructing a solver.
  friend GCM_FUNC(SPtr<MtGeomSolver>) GCM_CreateSolver( SPtr<ItPositionManager> posMan );
  // It will be removed. For testing purposes.
  friend GCM_FUNC(SPtr<MtGeomSolver>) GCT_GetSolver( GCM_system gSys );
  // Constructor for internal use only. Use the call GCM_CreateSolver.
  MtGeomSolver( SPtr<ItPositionManager> );
  // Constructor for internal use only. Use the call GCM_GetSolver.
  MtGeomSolver( GCM_system );

private:
  ~MtGeomSolver(); 
        
        MtConstraintManager * _Impl() { return myImpl; }
  const MtConstraintManager * _Impl() const { return myImpl; }
        MtBlackboxManager *   _BBoxMan();
  const MtBlackboxManager *   _BBoxMan() const;

  MtConstraintManager * myImpl;       ///< \ru Внутренняя реализация экземпляра геометрического солвера. \en Internal implementation of the solver instance.
  MtBlackboxManager *   myBBManager;  ///< \ru Менеджер черных ящиков. \en Manager of blackboxies.

private:
  MtGeomSolver( const MtGeomSolver & );
  MtGeomSolver & operator = ( const MtGeomSolver & );
};

//----------------------------------------------------------------------------------------
/** \brief \ru Создать объектно-ориентированный интерфейс 3D солвера.
           \en Create an object-oriented interface of 3D solver. \~

    \param[in] pMan - \ru Интерфейс, предоставляющий функции репозиции геометрических объектов на стороне приложения.
                      \en Interface that provides functions of reposition of geometric objects on the application side. \~
    \details \ru Вызов создает пустую систему ограничений. Кроме того, в памяти создаются 
      внутренние структуры данных геометрического решателя, обслуживающего систему ограничений. 
      Функция возвращает экземпляр класса, представляющего объектно-ориентированный интерфейс солвера.
             \en The call creates an empty constraint system. Besides, there are created internal 
      data structures of geometric solver maintaining the constraint system. The function returns 
      an instance of class representing an object-oriented interface of the 3D solver. \~
          
  \return \ru Указатель (с автоматическим подсчетом ссылок) на новый экземпляр геометрического решателя.
          \en Smart-pointer to a new instance of geometric constraint solver. \~
*/
//---
GCM_FUNC(SPtr<MtGeomSolver>) GCM_CreateSolver( SPtr<ItPositionManager> pMan );

/** \} */

//----------------------------------------------------------------------------------------
// The call is for internal use only.
/*
  Use method  SPtr<MtGeomSolver> GCM_CreateSolver(ItPositionManager *) to cteate object-oriented
  representation of the C3D Solver. Use call GCM_CreateSystem(void) to work with basic API
  of the geometric solver (gce_api.h).
*/
//---
GCM_FUNC(GCM_system) GCM_CreateSystem( ItPositionManager * );


/*
  Deprecated typenames
*/
typedef MtGeomSolver      IfGCManager;
typedef MtRepositionMode  MtTypeOfReposition;

/// \ru Полно-заданное или фиксированное тело (нулевая степень свободы). \en Fully-specified or fixed solid (zero degree of freedom).
static const GCM_dof_result sof_Zero              = GCM_DOF_RESULT_WellDefined;
/// \ru Полно-заданное или фиксированное тело (нулевая степень свободы). \en Fully-specified or fixed solid (zero degree of freedom).
static const GCM_dof_result sof_WellConstrained   = GCM_DOF_RESULT_WellDefined;
/// \ru Недоопределенное тело, т.е. имеющее степень свободы. \en Underconstrained solid, i.e. having a degree of freedom.
static const GCM_dof_result sof_UnderConstrained  = GCM_DOF_RESULT_UnderDefined;
/// \ru Нет сведений о степени свободы. \en No information about the degree of freedom.
static const GCM_dof_result sof_Unknown           = GCM_DOF_RESULT_Unknown;


#endif // __GCM_MANAGER_H

// eof
