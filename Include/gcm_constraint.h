//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file \brief  \ru Интерфейс для геометрического ограничения в 3D.
                \en Interface for geometric constraint in 3D. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GCM_CONSTRAINT_H
#define __GCM_CONSTRAINT_H

#include <gcm_manager.h>
#include <templ_dptr.h>
#include <templ_sptr.h>
#include <mb_cart_point3d.h>
#include <mb_matrix3d.h>

class   MbTopologyItem;
class   MtConstraintNode;
class   MtGeomNode;
struct  AdjacentCNodesRange;
struct  CNodesRange;

/**
  \addtogroup GCM_3D_ObjectAPI
  \{
*/

//----------------------------------------------------------------------------------------
/// \ru Ось планарного угла. \en Axis of a planar angle.
//---
struct GCM_CLASS GCM_geom_axis
{
  MbVector3D      axis;     ///< \ru Направляющий вектор оси планарного угла (задана в ЛСК тела geomPtr). \en Vector of planar angle axis direction (specified in LCS of geomPtr solid).
  const ItGeom *  geomPtr;  ///< \ru Тело, которому принадлежит ось планарного угла. \en solid the axis of a planar angle belongs to.
  GCM_geom_axis() 
    : axis( MbVector3D::zero )
    , geomPtr( nullptr ) {}

  GCM_geom_axis( const MbVector3D & axis, const ItGeom * g )
    : axis( axis )
    , geomPtr( g ) {}
};


//----------------------------------------------------------------------------------------
/// \ru Структура параметров ограничения. \en Structure of constraint parameters.
//---
struct GCM_CLASS GCM_c_params
{
  VERSION         m_Version;  ///< \ru Версия создания ограничения. \en Version of the constraint creation.
  GCM_c_type      m_Type;     ///< \ru Тип сопряжения. \en Type of mating.
  GCM_alignment   m_Align;
  GCM_tan_choice  m_TanChoice;
  GCM_angle_type  m_AngType;
  GCM_scale       m_Scale;
  GCM_coord_name  m_CrdName;
  double          m_RealPar;  ///< \ru Вещественный параметр. \en Real parameter.
  MtParVariant    m_Interval;

  GCM_c_params()
    : m_Version( GetCurrentMathFileVersion() )
    , m_Type( GCM_UNKNOWN )
    , m_Align( GCM_NO_ALIGNMENT )
    , m_TanChoice( GCM_TAN_NONE )
    , m_AngType( GCM_NONE_ANGLE )
    , m_Scale(GCM_NO_SCALE)
    , m_CrdName( GCM_NULL_CRD )
    , m_RealPar( UNDEFINED_DBL )
    , m_Interval()
  {}
private:
  static const GCM_scale m_scale = GCM_NO_SCALE;  // MA: Deprecated data field. Use m_Scale.
};


//----------------------------------------------------------------------------------------
/** \brief  \ru Геометрического ограничение.
            \en Geometric constraint. \~
    \details  \ru Абстрактный класс для структуры данных геометрического ограничения. 
  Класс #ItConstraintItem может быть реализован клиентским приложением, он играет 
  роль интерфейса, через который решатель MtGeomSolver берет данные об ограничении из 
  геометрической модели CAD-системы. Кроме этого указатель ItConstraintItem* в решателе 
  рассматривается, как тип данных "ограничение" его значение уникально идентифицирует
  конкретное ограничение на этапе выполнения (run-time) программы. Экземпляр класса
  ItConstraintItem может быть реализован, как внутри решателя, так и в клиентском 
  приложении.
            \en Abstract class or data structure of geometric constraint. Class 
  #ItConstraintItem can be implemented as a client application, it is used as
  interface via which the solver receives data  about a constraint from a geometric 
  model of CAD-system. Besides, pointer ItConstraintItem* in the solver MtGeomSolver 
  is considered as data type "constraint", its value uniquely identifies
  a certain constraint during program run-time. Instance of class
  ItConstraintItem can be implemented both inside the solver and in a client 
  application. \~  
            \ingroup GCM_3D_ObjectAPI
*/
//---
struct GCM_CLASS ItConstraintItem
{ 
public: /*
          Constraint data inquiries.
        */
  /// \ru Условие выравнивания \en Condition of alignment 
  virtual GCM_alignment   AlignType() const = 0;
  /// \ru Разновидность углового ограничения ("3D" или "Планарный"). \en Kind of angular constraint ("3D" or "Planar"). 
  virtual GCM_angle_type  AngleType() const = 0;
  /// \ru Ось углового сопряжения, заданная в ЛСК некоторого тела. Только для планарной разновидности. \en Axis of angular mating specified in LCS of some solid. Only for planar kind of constraint.
  virtual GCM_geom_axis   AxisOfPlanarAngle() const = 0;
  /** \brief \ru Ось углового сопряжения с разновидностью GCM_3D_ANGLE.
             \en Axis of angular mating with king GCM_3D_ANGLE. \~
      \details  \ru Функция выдает вектор, задающий ось ротации для углового размера с
                    3D-типом. Вектор задается в ЛСК первого объекта, GeomItem(1).
                \en The function gives a vector of the rotation axis for angular dimension with 3D-kind.
                    The vector is assigned in local coordinates of the first object, GeomItem(1). \~
  */
  virtual MbVector3D      AxisOf3DAngle() const { return AxisOf3DAngleType(); }  
  /// \ru Тип сопряжения (геометрического ограничения). \en Type of geometric constraint. 
  virtual GCM_c_type      ConstraintType() const = 0;
  /// \ru Диагностический код ошибки, прикрепленный к данному ограничению. \en Diagnostic error code attached to this constraint. 
  virtual GCM_result      ErrorCode() const = 0;
  /// \ru Версия математического ядра, в которой было создано сопряжение. \en The version of mathematical kernel in which the mating was created.   
  virtual VERSION         Version() const = 0;
  /** 
    \brief  \ru Числовой параметр размерного ограничения. 
            \en Numerical parameter of the dimensional constraint.
    \details  \ru Если размерное ограничение является угловым, то возвращаемое значение 
                  функции задается в радианах.\n
              \en If the dimensional constraint is angular, then the returning parameter 
                  is specified in radians. \~
  */
  virtual double          DimParameter() const = 0;
  /// \ru Вариант касания для ограничения c типом 'GCM_TANGENT'. \en Variant of tangency for constraint of type 'GCM_TANGENT'. \~
  virtual GCM_tan_choice  TangencyChoice() const = 0;

public: /*
          Dependency constraint inquiries
        */

  /** \brief \ru Зависимый объект ограничения с типом GCM_DEPENDENT, он всегда первый. 
             \en Dependent geom of type GCM_DEPENDENT, it is always first geom item.\~
  */
          ItGeomPtr DependentGeom() const;
  /**
    \brief  \ru Функция обратного вызова, которая определяет закон зависимости первого
                геометрического объекта от остальных участников данного ограничения.
            \en Callback function which defines a law of positioning of the first
                geometric object which is dependent on positions of other objects.
  */
  virtual GCM_dependent_func  Function() const { return nullptr; }
  virtual GCM_extra_param     ExtraParam() const { return GCM_extra_param(); }

public: /*
          Mating geometry inquiries
        */

  /**    
    \brief  \ru Сопрягаемый объект ограничения по номеру аргумента.
            \en Mating object of the constraint by a number of an argument . \~
    \param geomN - \ru Номер геометрического аргумента от 1 и более.
                   \en Number of geom argument from 1 and greater. \~        
    \return \ru Сопрягаемый объект вычисляемый в системе ограничений. \en Mating object calculating in the constraint system. \~
  */
  virtual ItGeomPtr GeomItem( int geomN ) const = 0;

  /**    
    \brief  \ru Геометрический аргумент ограничения.
            \en Geometric argument of the constraint.
    \details  \ru Функция выдает геометрический аргумент данного ограничения по номеру от 1 до Arity().             
              \en The function gives a geometric argument of the constraint by a number from 1 up to Arity().
              \~
    \param geomN - \ru Номер аргумента от 1 и более.
                   \en Number of argument from 1 and greater. 
                   \~
    \return \ru Объект, значение которого рассматривается, как аргумент ограничения.
            \en Object which value is considered as argument of the constraint. \~
  */    

  MtArgument  GeomArg( int geomN ) const;

  /**
    \brief  \ru Геометрическое значение аргумента ограничения, заданное заданный в ЛСК сопрягаемого объекта GeomItem(geomN).
            \en Geometric value of the argument, given in LCS of a "mating" object GeomItem(geomN). \~
    \details  \ru Функция выдает геометрическое значение объекта, на которое ссылается ограничение. Данный объект задан 
                  в ЛСК сопрягаемого тела, возвращаемого функцией GeomItem(), именно тело GeomItem(geomN) является предметом 
                  вычислений решателя, а sub-geom задает подчиненный объект стыковки, принадлежащий данному телу.         
                  (см. #ItConstraintItem::GeomItem).
              \en The function gives a geometric value of an object which the constraint refer to. Given object specified 
                  in local CS of the mating solid, which is returned by GeomItem() func. Namely, solid GeomItem(geomN) is 
                  a subject of the evaluation, and a sub-geom specify suborinated object of mate, belonging the solid. 
                  (see #ItConstraintItem::GeomItem). \~
    \param geomNb - \ru Номер аргумента от 1 и более.
                    \en Number of argument from 1 and greater. \~
    \return \ru Объект, значение которого рассматривается, как аргумент ограничения.
            \en Object which value is considered as argument of the constraint. \~
  */      
  MtGeomVariant   SubGeom( int geomNb ) const { return _LinkageItem( geomNb ); }

public:
  /// \ru Количество геометрических объектов, участвующих в ограничении. \en Number of geoms involved in the constraint. \~
  virtual int  Arity() const;
          void GetParams( GCM_c_params & ) const;

public: /*
          The functions for internal use in the solver.
        */
  // \ru Регистрация аргумента ограничения в решателе. \en Register an argument of constraint. 
  MtArgument  _RegisterArgument( int geomNb, MtGeomSolver & );
  // \ru Освободить от регистрации под узлом. \en Release from the constraint node registered in the solver.
  void        _Unregister( const MtConstraintNode * );

public: // \ru Методы для обратной связи (задающие). \en Methods for feedback (driving).
  /// \ru Задать код ошибки для неудовлетворенного сопряжения. \en Specify error code for unsatisfied mating.
  virtual void    SetErrorCode( MtResultCode3D ) = 0;
  /** \brief  \ru Задать ось для углового сопряжения с трехмерным типом измерения (GCM_3D_ANGLE).
              \en Specify the axis for angular mating with three-dimensional type of dimension (GCM_3D_ANGLE). \~
      \note \ru Ось задается и запоминается в СК первого тела GeomItem(1).
            \en The axis is specified and stored in CS of the first object, solid given by GeomItem(1).
  */
  virtual void    SetAxisOf3DAngle( const MbVector3D & axis ) { SetAxisOf3DAngleType(axis); }

public: // \ru Методы для управления временем жизни. См. также шаблон SPtr. \en Methods for lifetime management. See also template SPtr. 
  virtual refcount_t  AddRef() const = 0;
  virtual refcount_t  Release() const = 0;

private:  
  virtual MtGeomVariant _LinkageItem( int geomN ) const = 0;
  virtual int           _GeomsNb() const { return 0; }
          ItGeomPtr     _GArg( int geomN ) const;

private: // (!) The members below will be removed in a future version (V17 or later).
  typedef GCM_geom_axis   PlanarAngleAxis;
  typedef GCM_angle_type  EnAngleType;  

private: // (!) The members below will be removed in a future version (V17 or later).
  virtual MbVector3D  AxisOf3DAngleType() const = 0;
  virtual void        SetAxisOf3DAngleType( const MbVector3D & ) = 0;

private: // (!) The constants below will be removed in a future version (V17 or later).  
  static const GCM_angle_type at_Planar = GCM_2D_ANGLE;
  static const GCM_angle_type at_3D     = GCM_3D_ANGLE;

protected:
  virtual GCM_scale      _ScaleType() const { return GCM_NO_SCALE; }
  virtual GCM_coord_name _CoordName() const { return GCM_NULL_CRD; }
  virtual MtParVariant   _DimValue() const { return MtParVariant::undef; }

protected:
   ItConstraintItem() : m_args() {}
  ~ItConstraintItem() {}

private:
  std::vector<SPtr<ItGeom>> m_args;
  OBVIOUS_PRIVATE_COPY( ItConstraintItem );
};


//----------------------------------------------------------------------------------------
// Number of geoms involved in the constraint
/*
  Each constraint type has a strictly defined number of involved
  geom arguments with the exception of GCM_DEPENDENT.
*/
//---
inline int ItConstraintItem::Arity() const
{
  switch ( ConstraintType() )
  {
  case GCM_UNKNOWN:
    return 0;

  case GCM_RADIUS:
    return 1;

  case GCM_COINCIDENT:
  case GCM_PARALLEL:
  case GCM_PERPENDICULAR:
  case GCM_TANGENT:
  case GCM_CONCENTRIC:
  case GCM_DISTANCE:  
  case GCM_IN_PLACE:  
  case GCM_TRANSMITTION:
  case GCM_CAM_MECHANISM:
  case GCM_PATTERN_COORDINATE:
    return 2;

  case GCM_ANGLE:
  case GCM_SYMMETRIC:
  case GCM_LINEAR_PATTERN:
  case GCM_ANGULAR_PATTERN:
  case GCM_CS_PATTERN:
  case GCM_PATTERNED: // под вопросом, т.к. в API задается 2 аргумента
    return 3;

  case GCM_DEPENDENT:
  default:
    return _GeomsNb();
  }
}

//----------------------------------------------------------------------------------------
//
//---
inline void ItConstraintItem::GetParams( GCM_c_params & pars ) const
{
  pars.m_Version = Version();
  pars.m_Type = ConstraintType();
  pars.m_Align = AlignType();
  pars.m_TanChoice = TangencyChoice();
  pars.m_AngType = AngleType();
  pars.m_Scale = _ScaleType();
  pars.m_CrdName = _CoordName();
  GCM_interval interval;
  if ( _DimValue().GetInterval(interval) )
  {
    pars.m_Interval = interval;
  }
  else
  {
    pars.m_RealPar = DimParameter();
  }
}

//----------------------------------------------------------------------------------------
// Зависимый объект (Dependent geom).
//---
inline ItGeomPtr ItConstraintItem::DependentGeom() const 
{ 
  return (ConstraintType() == GCM_DEPENDENT) ? GeomItem(1) : nullptr;
}

//----------------------------------------------------------------------------------------
/** \brief \ru Интерфейс "Механическая передача для двух тел".
           \en Interface "Mechanical transmission for two solids". \~ 
    \ingroup GCM_3D_ObjectAPI
*/
//---
struct GCM_CLASS ItMateTransmission
{
  enum Motion ///< \ru Тип движения. \en Type of motion.
  {
    NoDefined,    ///< \ru Не задано. \en Not specified.
    Translation,  ///< \ru Линейное перемещение. \en Linear increment.
    Rotation,     ///< \ru Вращение. \en Rotation.
  };

  /// \ru Выдать первое или второе тело (nb -номер тела 1,2); \en Get the first or the second solid (ng is the number of solid 1,2); 
  virtual ItGeomPtr GetGeom( short nb ) const = 0;
  /// \ru Выдать первое или второе тело, задающее направление вращения/перемещения (nb - номер тела 1,2); \en Get the first or the second solid specifying the direction of rotation/translation (nb is the number of solid 1,2); 
  virtual ItGeomPtr GetDirectionGeom( short nb ) const = 0;
  /// \ru Выдать направление и тип движения для первого или второго тела, axis задается в ЛСК тела GetDirectionGeom(). \en Get direction and type of motion for the first or the second solid, axis is specified in LCS of solid GetDirectionGeom().
  virtual Motion    GetAxis( short nb, MbAxis3D & axis ) const = 0;
  /// \ru Выдать соотношение N1:N2; \en Get ratio N1:N2; 
  virtual double    GetRatio() const = 0; 
};

/**
  \fn  ItMateTransmission::GetAxis( short nb, MbAxis3D & axis )
  \ru Ось задается в ЛСК тела, возвращаемого в функции  ItGeom  * ItMateTransmission::GetDirectionGeom( short nb ) const
  \en The axis is specified in LCS of the solid returned in function  ItGeom  * ItMateTransmission::GetDirectionGeom( short nb ) const \~
*/

//----------------------------------------------------------------------------------------
/** \brief \ru Интерфейс "Кулачковый механизм".
           \en Interface "Cam mechanism". \~
  \details \ru Интерфейс для получения из модели "3D" исходных данных, описывающих 
  кулачковый механизм.\n
  Кулачковый мехнизм описывается:\n
    1) Тело кулачка и тело толкателя;\n
    2) Оси движения кулачка и толкателя, а также типы движения (вращательное/потступательное);\n
    3) Подмножество граней, принадлежащих кулачку, контактирующих с толкателем;
    4) Подмножество граней, принадлежащих толкателю, контактирующих с кулачком;
           \en Interface for extraction from "3D" model of the initial data describing 
  a cam mechanism.\n
  Cam mechanism is described:\n
    1) Solid of cam and solid of follower;\n
    2) Axes of motion of the cam and the follower, and types of motion (rotational/translation);\n
    3) Subset of faces belonging to the cam and contacting with the follower;
    4) Subset of faces belonging to the follower and contacting with the cam; \~
  
  \ingroup GCM_3D_ObjectAPI
*/
//---
struct GCM_CLASS ItCamMechanism
{
  enum Geom { // \ru Нумерация должна быть согласована с нумерацией участников кинематической пары ItMateTransmission \en Numeration should be compatible with the numeration of components of the kinematic pair ItMateTransmission 
    Cam      = 1, ///< \ru Кулачок(1-е тело); \en Cam (1st solid); 
    Follower = 2  ///< \ru Толкатель(2-е тело); \en Follower(2nd solid); 
  };
  
  /// \ru Выдать набор топологических объектов контактирования для 1-го или 2-го тела (nb=1-кулачок, nb=2-толкатель); \en Get a set of topological objects of contact for the 1st and the 2nd solid (nb=1- the cam, nb=2-the follower); 
  virtual void    GetTouchFaceSet( Geom nb, RPArray<MbTopologyItem> & faceSet ) const = 0;
  /// \ru Выдать матрицу преобразования топологических объектов касания в СК кулачка или толкателя (nb=1-кулачок, nb=2-толкатель); \en Get the transformation matrix of topological objects of tangency in coordinate system of the cam or the follower (nb=1- the cam, nb=2- the follower); 
  virtual void    GetMatrixToGeomLCS( Geom nb, MbMatrix3D & toGeomLCS ) const = 0;
  /// \ru Добавить ссылку на объект \en Add a reference to the object 
  virtual refcount_t  AddRef() const = 0;
  /// \ru Освободить ссылку на объект \en Free the reference to the object 
  virtual refcount_t  Release() const = 0;
};

//----------------------------------------------------------------------------------------
/// \ru Выдать трехзначную величину ориентации {-1,0,+1}. \en Get three-valued orientation {-1,0,+1}.
// ---
inline int AlignmentSign( GCM_alignment aVal )
{
  switch ( aVal )
  {
    case  GCM_ALIGNED_0:
    case  GCM_ALIGNED_1:
    case  GCM_ALIGNED_2:
    case  GCM_ALIGNED_3:
      return 1;

    case  GCM_REVERSE_0:
    case  GCM_REVERSE_1:
    case  GCM_REVERSE_2:
    case  GCM_REVERSE_3:
      return -1;

    default:
      return 0;
  }
}

//----------------------------------------------------------------------------------------
/// \ru Выдать двузначную величину ориентации \en Get two-valued orientation 
// ---
inline bool Orient( GCM_alignment aVal ) { return AlignmentSign(aVal) > 0; }

//----------------------------------------------------------------------------------------
/// \ru Выдать двузначную величину варианта касания \en Get two-valued variant of tangency 
// ---
inline bool TangVariant( GCM_alignment aVal ) 
{
  return  aVal == GCM_REVERSE_1 || 
          aVal == GCM_ALIGNED_1 ||
          aVal == GCM_REVERSE_3 || 
          aVal == GCM_ALIGNED_3;
}

//----------------------------------------------------------------------------------------
/// \ru Выдать двузначную величину подварианта касания. \en Get two-valued subvariant of tangency.
// ---
inline bool TangSubVariant( GCM_alignment aVal )
{
  return  aVal == GCM_REVERSE_2 || 
          aVal == GCM_ALIGNED_2 ||
          aVal == GCM_REVERSE_3 || 
          aVal == GCM_ALIGNED_3;
}

//----------------------------------------------------------------------------------------
/// \ru Выдать код условия выравнивания по трем двухзначным флагам ориентации, варианта и подварианта касания \en Get code of alignment condition by three two-valued flags of orientation, variant and subvariant of tangency. 
// ---
inline GCM_alignment AlignOption( bool axisOrient, bool tangOrient, bool tangSubvariant = false ) 
{
  if ( axisOrient ) 
  {
    if ( tangOrient ) {
      return tangSubvariant ? GCM_ALIGNED_3 : GCM_ALIGNED_1;
    }
    else {
      return tangSubvariant ? GCM_ALIGNED_2 : GCM_ALIGNED_0;
    }
  }
  else 
  {
    if ( tangOrient ) {
      return tangSubvariant ? GCM_REVERSE_3 : GCM_REVERSE_1;
    }
    else {
      return tangSubvariant ? GCM_REVERSE_2 : GCM_REVERSE_0;
    }
  }
}

//----------------------------------------------------------------------------------------
/// \ru Выдать следующий вариант выравнивания. \en Get the next variant of alignment.
// ---
GCM_FUNC(void) NextSolution( GCM_alignment & );

//----------------------------------------------------------------------------------------
/// \ru Выдать предыдущий вариант выравнивания. \en Get the previous variant of alignment.
// ---
GCM_FUNC(void) PrevSolution( GCM_alignment & );

/** \} */

//----------------------------------------------------------------------------------------
//
//---
inline ItGeomPtr ItConstraintItem::_GArg( int geomN ) const
{
  return size_t(geomN-1) < m_args.size() ? m_args[geomN-1] : ItGeomPtr( nullptr );
}

//----------------------------------------------------------------------------------------
// Аргумент ограничения
//---
inline MtArgument ItConstraintItem::GeomArg( int geomN ) const
{
  if ( _GArg(geomN) )
  {
    return MtArgument( _GArg(geomN) );
  }
  return MtArgument( GeomItem(geomN), SubGeom(geomN) );
}

//----------------------------------------------------------------------------------------
/** \brief  \ru Диапазон из набора геометрических ограничений.
            \en The range of the geometric constraints set. \~
    \note it is for testing purposes.
*/
//---
class GCM_CLASS GeConstraintsRange
{ 
public:
  struct constraint_t: public MtObjectId {};
  using value_type = constraint_t;
public:
  GeConstraintsRange();
  GeConstraintsRange( const GeConstraintsRange & );
  GeConstraintsRange( const MtConstraintSystem & );
  GeConstraintsRange & operator = ( const GeConstraintsRange & );
public:
  bool                  empty() const;
  size_t                size() const;
  GeConstraintsRange &  drop_front();
  GeConstraintsRange &  drop_back();
  GCM_constraint        frontId() const;
  GCM_constraint        backId() const;
private:
  SPtr<MtRefItem>  m_pOwner;
  CNodesRange *    m_pImpl;
};

//----------------------------------------------------------------------------------------
// Get a range to traverse constraints of the system.
// for testing only.
//---
GCM_FUNC(GeConstraintsRange) GCM_GetConstraints( GCM_system gSys );

//----------------------------------------------------------------------------------------
/** \brief  \ru Диапазон смежных ограничений геометрического объекта.
            \en The range of adjacent constraints of a geometric object. \~
*/
//---
class GCM_CLASS MtAdjacentConstrsRange
{ 
public:
  struct constraint_t: public MtObjectId {};
  using value_type = constraint_t;
public:
  MtAdjacentConstrsRange();
  MtAdjacentConstrsRange( const MtAdjacentConstrsRange & );
  MtAdjacentConstrsRange( const MtGeomNode & );
  MtAdjacentConstrsRange & operator = ( const MtAdjacentConstrsRange & );
public:
  bool                      empty() const;
  size_t                    size() const;
  MtAdjacentConstrsRange &  drop_front();
  MtAdjacentConstrsRange &  drop_back();
  GCM_constraint            frontId() const;
  GCM_constraint            backId() const;
private:
  SPtr<MtRefItem>       m_pOwner;
  AdjacentCNodesRange * m_pImpl;
};

//----------------------------------------------------------------------------------------
/** \brief \ru Получить диапазон смежных ограничений геометрического объекта.
           \en Get the range of adjacent constraints of a geometric object. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~  
  \param[in] g    - \ru Дескриптор геометрического объекта.
                    \en Descriptor of geometric object \~
  \return \ru Диапазон из набора геометрических ограничений.
          \en The range of the geometric constraints set. \~
*/
//---
GCM_FUNC(MtAdjacentConstrsRange) GCM_AdjacentConstrsRange( GCM_system gSys, GCM_geom g );

//----------------------------------------------------------------------------------------
// Get unified data record of a geometric constraint.
// for testing only.
//---
GCM_FUNC(GCM_c_record) GCM_ConstraintRecord( GCM_system gSys, GCM_constraint conId );

//----------------------------------------------------------------------------------------
// Get S-expression of a geometric constraint.
// for testing only.
//---
GCM_FUNC(std::string&) GCM_SExprRecord( GCM_system gSys, GCM_constraint conId, std::string& str );

//----------------------------------------------------------------------------------------
/** \brief \ru Получить интерфейсный объект геометрического ограничения ItConstraintItem по дескриптору.
           \en Get an interface object ItConstraintItem of geometric constraint by the descriptor. \~
    \note Internal use only
*/
//--
GCM_FUNC(const ItConstraintItem *) GCM_ConstraintItem( GCM_system gSys, GCM_constraint conId );

//----------------------------------------------------------------------------------------
/** \brief \ru Получить геометрическое ограничение типа GCM_constraint из типа ItConstraintItem *.
           \en Get a geometric constraint of type GCM_constraint from type of ItConstraintItem *. \~
*/
//--
GCM_FUNC( GCM_constraint ) GCM_ConstraintId( GCM_system gSys, const ItConstraintItem * cPtr );

#endif // __GCM_CONSTRAINT_H

// eof
