//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file \brief  \ru Геометрические типы данных C3D Solver.
                \en Geometrical types of data C3D Solver. \~
        \details  \ru Геометрические типы данных для объектно-ориентированной части 
                      программного интерфейса C3D Solver. См. также базовую часть 
                      интерфейса <gcm_types.h>.
                  \en Geometry data types for the object-oriented part of C3D Solver API.
                      See also file <gcm_types.h> that represents basic part of the C3D Solver API. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GCM_GEOM_H
#define __GCM_GEOM_H

#include <tool_cstring.h>
#include <templ_sptr.h>
#include <mb_axis3d.h>
#include <mb_matrix3d.h>
#include <mb_placement3d.h>
#include <marker.h>
#include <gcm_types.h>
#include <cur_line3d.h>

/**
  \addtogroup GCM_3D_ObjectAPI
  \{
*/

//----------------------------------------------------------------------------------------
//
//---
typedef GCM_geom        MtGeomId;
typedef GCM_constraint  MtConstraintId;
typedef GCM_geom        MtPatternId;
typedef GCM_g_type      MtGeomType;
typedef GCM_c_type      MtMateType;

//----------------------------------------------------------------------------------------
/** \brief    \ru Геометрический объект. \en Geometrical object. \~
    \details  \ru Геометрический объект, характеризуется матрицей трансформации, 
   которая переводит его в текущее положение из стандартного положения, совпадающего
   с мировой системой координат.\n
      Например,  если экземпляр ItGeom представляет `точку`, то матрица TransMatrix()
   выдаст перенос в текущее положение точку начала (0,0,0).\n
      Если экземпляром `ItGeom` является жесткое тело в пространстве, то матрица TransMatrix()
   будет служить в качестве ЛСК этого тела и задавать его положение в пространстве 
   моделирования.
           \en Geometric object of this data type is described by a matrix 
  of transformation which transforms this object to real position from a standard position
  which coincides with the global coord system.\n
    For example, if ItGeom is a point, then the matrix TransMatrix()
  is translation to the current location from the origin (0,0,0).\n 
    If `ItGeom` is rigid solid in the space, then the matrix TransMatrix()
  can be used as LCS of this solid and set its position in the space of modeling. \~
    \ingroup GCM_3D_ObjectAPI
*/
//---
struct GCM_CLASS ItGeom
{
  mutable MtGeomId objectId; // a cach for internal use only.
  ItGeom() : objectId( GCM_NULL ) {}
  /// \ru Тип геометрического объекта. \en Type of geometric object. \~
  virtual GCM_g_type    GeomType() const { return GCM_LCS; }
  /// \ru Строковое имя геометрического объекта. \en String name of geometric object.
  virtual const TCHAR * GetName() const = 0;
  /// \ru Выдать положение детали в виде ортонормированной ЛСК. \en Get position of part as orthonormalized LCS.
  virtual void          GetPlacement( MbPlacement3D & pl ) const { pl = MbPlacement3D::global; }
  /// \ru Выдать трансформацию из стандартного положения. \en Get transformation from standard position. \~
  inline  void          GetTransMatrix( MbMatrix3D & ) const;
  /// \ru Трансформация из стандартного положения. \en Transformation from the standard position. \~
          MbMatrix3D    TransMatrix() const;

public: /*
          Reference counting support.
        */
  virtual refcount_t  AddRef() const = 0;
  virtual refcount_t  Release() const = 0;

protected:
  virtual ~ItGeom() {}

private:  // It will be removed ... use GetPlacement instead.
  virtual void  GetGeomPlacement( MbPlacement3D & ) const {}
};

/*
  ItGeom as pointer type.
*/
typedef ItGeom * ItGeomPtr;

//----------------------------------------------------------------------------------------
// \ru Трансформация из стандартного положения \en Transformation from the standard position 
//---
inline MbMatrix3D ItGeom::TransMatrix() const
{
  MbMatrix3D tValue;  
  GetTransMatrix( tValue );
  return tValue;
}

//----------------------------------------------------------------------------------------
//  \ru Выдать трансформацию из стандартного положения. 
//  \en Get transformation from standard position.
//---
inline void ItGeom::GetTransMatrix( MbMatrix3D & mat ) const
{
  MbPlacement3D place;
  GetPlacement( place );
  place.GetMatrixFrom( mat );
}

//----------------------------------------------------------------------------------------
// Internal data types forward declaration.
//---
struct MtUnifiedGeom;
struct MbGeomTol;
class  MtParGeom;

//----------------------------------------------------------------------------------------
/** \brief \ru Геометрический объект, аргумент геометрического ограничения.
           \en Geometric object, argument of geometric constraint. \~
    \ingroup GCM_3D_ObjectAPI
*/
//---
class GCM_CLASS MtGeomVariant
{
public:
  
  MtGeomVariant() : m_value( nullptr ) {} // Default constructor.
  MtGeomVariant( const MtGeomVariant & );
  MtGeomVariant( MtGeomVariant && gVar ) noexcept : m_value( nullptr ) { TakeOn( gVar.m_value ); gVar.m_value = nullptr; }  
  MtGeomVariant( const MbPlacement3D & ); // LCS constructor.
  MtGeomVariant( const MbCartPoint3D & ); // Point constructor.
  MtGeomVariant( const MbAxis3D & );
  MtGeomVariant( const MtUnifiedGeom & );
  MtGeomVariant( const MtParGeom & g ) : m_value( nullptr ) { Assign(g); }
  MtGeomVariant( const GCM_g_type );
  // Constructs rotation surface: cylinder, cone, torus, sphere.
  MtGeomVariant( const GCM_g_type gType, const MbCartPoint3D& org, const MbVector3D& axis, double radius );
  MtGeomVariant & operator = ( const MtGeomVariant & gVar ) { return Assign( gVar ); }
  /// \ru Переносное присвоение. \en Moving assignment.
  MtGeomVariant & operator = ( MtGeomVariant && gVar ) noexcept { TakeOn( gVar.m_value ); gVar.m_value = nullptr; return *this; }
 ~MtGeomVariant();

public:
  /// \ru Тип геометрии. \en Type of geometry.
  GCM_g_type   GeomType() const;
  /// \ru Выдать данные геометрии в унифицированной форме записи. \en Get the uniform geometry data record.
  GCM_g_record GeomRecord() const;
  /// \ru Выдать трансформацию объекта из стандартного положения. \en Get transformation of the object from the standart position.
  MbMatrix3D & GetTransMatrix( MbMatrix3D & ) const;  
  /// \ru Выдать положение детали в виде ортонормированной ЛСК. \en Get position of part as orthonormalized LCS.
  MbPlacement3D & GetPlacement( MbPlacement3D & pl ) const;
  /// \ru Является ли объект пустым. \en Get logic value whether the object is empty.
  bool      IsNull() const;
  /// \ru Проверить равны ли объекты геометрическию. \en Check if objects are geometrically equal.
  bool      IsEqualTo( const MtGeomVariant &, const MbGeomTol & ) const;
  /// \ru Проверить равны ли структуры геометрической записи (gType,O,Z,X,Y,R1,R2). \en Check if corresponding elements of geometric records are equal (gType,O,Z,X,Y,R1,R2).
  bool      IsEqualTuples( const MtGeomVariant & gVar, const MbGeomTol & gTol ) const;
  /// \ru Выдать ось вращения, если она свойственна геометрическому объекту (цилиндр, тор, конус). \en Output the axis of rotation for geometry such as cylinder, torus, cone.
  MbAxis3D  Axis() const;
  /// \ru Радиус сферы, цилиндра или "большой" радиус тора или конуса. \en Radius of the sphere, cylinder, major radius of the tori or cone.
  double    RadiusA() const;
  /// \ru "Малый" радиус тора или конуса. \en Minor radius of the tori or cone.
  double    RadiusB() const;

public: /* Assigning methods.
        */   
  MtGeomVariant & Assign( const MtGeomVariant & );
  MtGeomVariant & Assign( const MtParGeom & );
  MtGeomVariant & Assign( GCM_g_type, const MbCartPoint3D & org, const MbVector3D & zAxis
                        , const MbVector3D & xAxis, double r1 = 0.0, double r2 = 0.0 );
  template<class GeomDS>
  MtGeomVariant & Assign( const GeomDS * );
  MtGeomVariant & Reset(); ///< \ru Задать как тип GCM_NULL_GTYPE. \ru Set as GCM_NULL_GTYPE type. \~
  MtGeomVariant & SetAsPoint( const MbCartPoint3D & );
  MtGeomVariant & SetAsLine( const MbCartPoint3D & org, const MbVector3D & dir );  
  MtGeomVariant & SetAsPlane( const MbCartPoint3D & org, const MbVector3D & normal );
  MtGeomVariant & SetAsPlane( const MbPlacement3D & );
  MtGeomVariant & SetAsMarker( const MbCartPoint3D & org, const MbVector3D & zAxis, const MbVector3D & xAxis );
  MtGeomVariant & SetAsLCS( const MbPlacement3D & );
  MtGeomVariant & SetAsAxis( const MbCartPoint3D & org, const MbVector3D & zAxis);
  MtGeomVariant & Transform( const MbMatrix3D & ); 

public:  /* Methods for internal use.
         */
  const MtParGeom & ParGeom() const;
  MtUnifiedGeom &   GetTuple( MtUnifiedGeom & ) const;

private:
  // Take ownership of a geometric object.
  MtGeomVariant & TakeOn( MtParGeom * );

private:
  MtParGeom * m_value;  // unique ownerhip of the geometric record value.
};

//----------------------------------------------------------------------------------------
//
//---
template<class GeomDS>
MtGeomVariant & MtGeomVariant::Assign( const GeomDS * gDs )
{
  if ( gDs )
  {
    return Assign( *gDs );
  }
  return Reset();
}

//----------------------------------------------------------------------------------------
/** \brief \ru Структура данных сопрягаемой геометрии.
           \en Data structure for matched geometry. \~
  \details \ru Эта структура используется для передачи информации о геометрическом 
  объекте, который участвует в ограничениях. Геометрический объект, передаваемый
  через эту структуру данных, может быть задан в ЛСК жесткого тела или в мировой 
  системе координат модели. \n  
    Следует учитывать, что геометрический объект, возвращаемый функцию 
  #MtMatingGeometry::GetMatingGeom(), задан в ЛСК с матрицей 
  #MtMatingGeometry::LCSMatrix(), т.е. решатель внутри интерпретирует #MtMatingGeometry
  как объект #MtMatingGeometry::GetMatingGeom(), заданный в #MtMatingGeometry::LCSMatrix().
           \en This structure is used for passing information about geometric 
  object involved in constraints. Geometric object passed
  via this data structure can be specified in LCS of a rigid solid or in the world 
  coordinate system. \n
    It should be taken into account that the geometric object returned from the function 
  #MtMatingGeometry::GetMatingGeom(), is given in LCS with matrix 
  #MtMatingGeometry::LCSMatrix(), i.e. the solver interprets  #MtMatingGeometry
  as object #MtMatingGeometry::GetMatingGeom() specified in #MtMatingGeometry::LCSMatrix(). \~
          \ingroup GCM_3D_ObjectAPI
*/
//---
class GCM_CLASS MtMatingGeometry
{
public:
  using Orient = GCM_alignment; ///< \ru Трехзначное свойство ориентации \en Three-valued property of orientation.  
  static const Orient Opposite   = GCM_OPPOSITE; ///< \ru "Обратное направление" \en "Reverse direction" 
  static const Orient Cooriented = GCM_COORIENTED; ///< \ru "Прямое направление" \en "Forward direction" 
  static const Orient Unoriented = GCM_NO_ALIGNMENT;  ///< \ru Ориентация не свойственна или "Прямое направление" \en Orientation is nonrelevant or "Forward direction" 
  
public:
  MtMatingGeometry( const MtMatingGeometry & ) = default;
  MtMatingGeometry & operator = ( const MtMatingGeometry & ) = default;
  MtMatingGeometry()
    : myGeom( nullptr )
    , myOrientation( Unoriented )
    , myLCSMatrix( MbMatrix3D::identity )
    , myGeomType(GCM_UNKNOWN_GTYPE)
  {}

public:
 
  /// \ru Выдать ориентацию; \en Get orientation; 
        Orient        GetOrientation() const { return myOrientation; }
        /// \ru Выдать геометрический объект сопряжения. Если =nullptr, то это точка, заданная MtMatingGeometry::myMatingPoint; \en Get geometric object of the mating. If =nullptr, then this is a point specified by MtMatingGeometry::myMatingPoint;
  const MbSpaceItem * GetMatingGeom() const { return myGeom; }
        /// \ru Выдать матрицу ЛСК, в которой задан геометрический объект сопряжения \en Get matrix of LCS in which the geometric object of the mating is specified 
  const MbMatrix3D &  LCSMatrix() const;


  /**
    \brief \ru Задать структуру данных как представление прямой.
           \en Set the data structure to line representation.  \~
  */
  void  SetAsLine( const MbCartPoint3D &, const MbVector3D & );

  /** \brief  \ru Инициализировать структуру данных маркером.
              \en Initialize the data structure with a marker.\~  */
  void  SetAsMarker( const MbCartPoint3D &, const MbVector3D & z, const MbVector3D & x );
  /**
    \brief  \ru Инициализировать структуру данных маркером.
            \en Initialize the data structure with a marker.\~
    \param  \ru gArg  маркер (! аргумент не передается во владение структуры)
            \en gArg is marker (!  the argument is not transferred to the structure ownership) \~
  */
  void  SetAsMarker( const MbMarker & );
  /**
    \brief  \ru Инициализировать структуру данных осью. \en Initialize the data structure with an axis.\~
  */
  void  Set( const MbAxis3D & );

  /// \ru Присвоить структуре значение ЛСК; \en Assign this data structure with a value of LCS \~ 
  void  SetAsLCS( const MbPlacement3D & lcs );
  void  Assign( const MbPlacement3D & lcs ) { SetAsLCS(lcs); }

  /// \ru Инициализировать структуру данных кривой или поверхностью; \en Initialize data structure with a curve or a surface; 
  /**
    \param \ru gItem геометрический объект, подкласс MbSpaceItem (! передается во владение структуры)
           \en gItem is geometric object, subclass of MbSpaceItem (! transferred to the structure ownership) \~
    \param \ru gDir  флаг ориентации геометрического объекта
           \en gDir is flag of geometric object orientation \~ 
    \param \ru gSpan матрица, задающая подпространство объекта
           \en gSpan is matrix specifying subspace of the object \~
  */
  void  SetAsMatingGeomItem( SPtr<const MbSpaceItem> gItem, Orient gDir, const MbMatrix3D & gSpan );
  /// \ru Задать пустой объект \en Specify an empty object 
  void  SetNull();

public:
  /*
    (!) Deprecated
    \brief  \ru Инициализировать структуру данных маркером или ЛСК.
            \en Initialize the data structure with a marker or a LCS.\~
    \param  \ru gType тип геометрии, для которой маркер является аргументом (описателем геометрии)
            \en gType is a type of geometry for which the marker is an argument (geometry descriptor) \~
    \param  \ru gArg  маркер (! аргумент не передается во владение структуры)
            \en gArg is marker (!  the argument is not transferred to the structure ownership) \~
  */
  void  SetAsMarker( MtGeomType gType, const MbMarker & gArg );

  // Internal use only
  MtGeomVariant GeomVariant( VERSION c3dVer ) const;

private:
  // Get type of mating geometry.
  MtGeomType  GetGeomType() const { return myGeomType; }       
  void       _SetLCSMatrix( const MbMatrix3D & );

private:
  MtGeomType              myGeomType;     // \ru Кодирует тип сопрягаемой геометрии \en Encodes type of mating geometry   
  SPtr<const MbSpaceItem> myGeom;         // \ru Геометрический объект сопряжения \en Geometric object of mating 
  Orient                  myOrientation;  // \ru Ориентация геометрического объекта myMatingGeom \en Orientation of geometric object myMatingGeom   
  MbMatrix3D              myLCSMatrix;    // \ru Матрица преобразования (ЛСК сопрягаемого тела) \en Transformation matrix (LCS of the mating solid) 
};


//----------------------------------------------------------------------------------------
// \ru Выдать матрицу ЛСК, в которой задан геометрический объект сопряжения \en Get matrix of LCS in which the geometric object of the mating is specified 
//---
inline const MbMatrix3D & MtMatingGeometry::LCSMatrix() const
{
  return myLCSMatrix;
}

//----------------------------------------------------------------------------------------
// Задать структуру данных как представление прямой.
//---
inline void MtMatingGeometry::SetAsLine( const MbCartPoint3D & org, const MbVector3D & dir )
{
  myGeomType = GCM_LINE;
  myGeom = new MbLine3D( org, dir );
  myOrientation = Cooriented;
  myLCSMatrix = MbMatrix3D::identity;
}

//----------------------------------------------------------------------------------------
// \ru Инициализировать структуру данных кривой или поверхностью; \en Initialize data structure with a curve or a surface; 
//---
inline void MtMatingGeometry::SetAsMarker( MtGeomType gType, const MbMarker & gArg )
{
  C3D_ASSERT( gType == GCM_MARKER );
  myGeomType = gType;
  myGeom = new MbMarker( gArg );
  myOrientation = Unoriented;
  myLCSMatrix = MbMatrix3D::identity;
}

//----------------------------------------------------------------------------------------
// 
//---
inline void MtMatingGeometry::SetAsMarker( const MbMarker & gArg )
{
  myGeomType = gArg.GetAxisX().IsZero() ? GCM_AXIS : GCM_MARKER;
  myGeom = new MbMarker( gArg );
  myOrientation = Unoriented;
  myLCSMatrix = MbMatrix3D::identity;
}

//----------------------------------------------------------------------------------------
// 
//---
inline void MtMatingGeometry::Set( const MbAxis3D& gArg )
{
  myGeomType = GCM_AXIS;
  myGeom = new MbMarker( gArg.GetOrigin(), gArg.GetAxisZ() );
  myOrientation = Unoriented;
  myLCSMatrix = MbMatrix3D::identity;
}

//----------------------------------------------------------------------------------------
/* \ru Инициализировать структуру данных маркером.
   \en Initialize the data structure with a marker.\~  */
//---
inline void MtMatingGeometry::SetAsMarker( const MbCartPoint3D & org, const MbVector3D & z, const MbVector3D & x )
{
  SetAsMarker({org, z, x});
}

//----------------------------------------------------------------------------------------
// \ru Присвоить структуре значение ЛСК; \en Assign this data structure with a value of LSC
//---
inline void MtMatingGeometry::SetAsLCS( const MbPlacement3D & lcs )
{
  myGeomType = GCM_LCS;  
  myGeom = nullptr; // new MbMarker( MbCartPoint::origin, MbVector3D::zAxis, MbVector3D::xAxis );
  myOrientation = Unoriented;
  _SetLCSMatrix( lcs.GetMatrixFrom() );
}

//----------------------------------------------------------------------------------------
// Initialize the data structure by a curve or a surface 
//---
inline void MtMatingGeometry::SetAsMatingGeomItem(  SPtr<const MbSpaceItem> gItem
                                                  , Orient gDir
                                                  , const MbMatrix3D & mat )
{    
  if ( gItem )
  {
    myGeomType = GCM_LAST_GTYPE;
    myGeom = gItem;
    myOrientation = gDir;      
    _SetLCSMatrix( mat );
  }
}

//----------------------------------------------------------------------------------------
//
//---
inline void MtMatingGeometry::_SetLCSMatrix( const MbMatrix3D & lcs )
{
  myLCSMatrix = lcs;
}

//----------------------------------------------------------------------------------------
// \ru Задать пустой объект. \en Specify an empty object.
//---
inline void MtMatingGeometry::SetNull()
{
  myGeomType = GCM_NULL_GTYPE;
  myGeom = nullptr;
  myOrientation = Unoriented; 
  myLCSMatrix = MbMatrix3D::identity;
}

//----------------------------------------------------------------------------------------
/** \brief \ru Получить геометрический объект типа GCM_geom из типа ItGeom *.
           \en Get a geometric object of type GCM_geom from type of ItGeom *. \~
*/
//--
GCM_FUNC(GCM_geom) GCM_GeomId(GCM_system gSys, const ItGeom* gPtr);

//----------------------------------------------------------------------------------------
/** \brief \ru Получить интерфейс геометрического объекта 'ItGeom' по дескриптору.
           \en Get an interface object ItGeom of geometric entity by the descriptor. \~
    \note Internal use only
*/
//--
GCM_FUNC(ItGeom*) GCM_GeomItem(GCM_system gSys, GCM_geom geomId);


//----------------------------------------------------------------------------------------
/** \brief \ru Получить запись геометрического объекта по типу MtGeomVariant.
           \en Get the geometric record of the variant data type.
*/
// ---
GCM_FUNC( GCM_g_record ) GCM_GeomRecord(const MtGeomVariant&);

//----------------------------------------------------------------------------------------
/** \brief \ru Конвертировать структуру MtMatingGeometry в аргумент ограничения.
           \en Convert the structure MtMatingGeometry to the argument of constraint. \~
    \param[in] cVer - \ru Версия ограничений, для которого будет применяться аргумент.
                      \en The version of constraint using the resulting argument. \~
*/
//---
GCM_FUNC(MtGeomVariant) GeomArgument( const MtMatingGeometry &, VERSION cVer );

//----------------------------------------------------------------------------------------
// Internal use only
/*
  Returns a pointer equal gPtr, if the operation succeeded.
*/
//--
GCM_FUNC(const ItGeom *) GCM_SetDependencyGeom( GCM_system gSys, MtGeomId, const ItGeom * gPtr );

/** \} */

//----------------------------------------------------------------------------------------
//
//---
typedef       ItGeom *  IfGeomPtr; // deprecated.
typedef const ItGeom *  IfConstGeomPtr; // deprecated.

#endif // __IT_GEOM_H

// eof
