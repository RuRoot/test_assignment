//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file \brief  \ru Характеристики типов для математических классов ядра "Mb..."
                \en Type traits of the math basic classes of the kernel "Mb..." \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_CLASS_TRAITS_H
#define __MB_CLASS_TRAITS_H

#include <plane_item.h>
#include <space_item.h>
#include <curve3d.h>
#include <surface.h>
#include <topology_item.h>

class MbLineSegment;
class MbArc;
class MbNurbs;
class MbLine;
class MbItem;
class MbPlaneInstance;
class MbSpaceInstance;
class MbAssembly;
class MbInstance;
class MbSolid;
class MbWireFrame;
class MbMesh;
class MbLineSegment3D;
class MbArc3D;
class MbFace;
class MbEdge;
class MbCurveEdge;
class MbVertex;


//----------------------------------------------------------------------------------------
/** \brief  \ru Характеристики классов геометрического ядра C3D. 
            \en Class traits of C3D geometric kernel.
    \attention  \ru Экспериментальный класс. Пока приведены не все типы классов.
                \en Experimental class. While not all listed types of classes. \~
*/
//---
template<class Type>
struct MbClassTraits
{
private:
  // \ru Идентификатор класса математического ядра. \en Identifier of class of the geometric kernel.
  static const MbeSpaceType typeId = st_Undefined;
};

//----------------------------------------------------------------------------------------
// For instantiated math classes.
//---
struct _IsInstant
{
  template<class ParentType, class ClassEnum>
  inline bool operator()( const ParentType * obj, const ClassEnum _typeId )
  {
    return obj == nullptr ? true : obj->IsA() == _typeId;
  }
};

//----------------------------------------------------------------------------------------
// For abstract family classes.
//---
struct _IsFamily
{
  template<class ParentType, class ClassEnum>
  inline bool operator()( const ParentType * obj, const ClassEnum _typeId )
  {
    return obj == nullptr ? true : obj->Family() == _typeId;
  }
};

/*
  2D-curve sub-classes.
*/
template<>
struct MbClassTraits<MbLineSegment>:_IsInstant { static const MbePlaneType typeId = pt_LineSegment; };
template<>
struct MbClassTraits<MbArc>:_IsInstant { static const MbePlaneType typeId = pt_Arc; };
template<>
struct MbClassTraits<MbNurbs>:_IsInstant { static const MbePlaneType typeId = pt_Nurbs; };
template<>
struct MbClassTraits<MbLine>:_IsInstant { static const MbePlaneType typeId = pt_Line; };

/*
  Declarations of abstract classes inherited from MbSpaceItem.
*/
template<>
struct MbClassTraits<MbItem>:_IsFamily { static const MbeSpaceType typeId = st_Item; };
template<>
struct MbClassTraits<MbCurve3D>:_IsFamily { static const MbeSpaceType typeId = st_Curve3D; };
template<>
struct MbClassTraits<MbSurface>:_IsFamily { static const MbeSpaceType typeId = st_Surface; };

/*
  C3D model sub-classes. Inherited from MbItem.
*/
template<>
struct MbClassTraits<MbPlaneInstance>:_IsInstant { static const MbeSpaceType typeId = st_PlaneInstance; };
template<>
struct MbClassTraits<MbSpaceInstance>:_IsInstant { static const MbeSpaceType typeId = st_SpaceInstance; };
template<>
struct MbClassTraits<MbAssembly>:_IsInstant   { static const MbeSpaceType typeId = st_Assembly; };
template<>
struct MbClassTraits<MbInstance>:_IsInstant   { static const MbeSpaceType typeId = st_Instance; };
template<>
struct MbClassTraits<MbSolid>:_IsInstant      { static const MbeSpaceType typeId = st_Solid; };
template<>
struct MbClassTraits<MbWireFrame>:_IsInstant  { static const MbeSpaceType typeId = st_WireFrame; };
template<>
struct MbClassTraits<MbMesh>:_IsInstant { static const MbeSpaceType typeId = st_Mesh; };

/*
  3D-curve sub-classes.
*/
template<>
struct MbClassTraits<MbLineSegment3D>:_IsInstant { static const MbeSpaceType typeId = st_LineSegment3D; };
template<>
struct MbClassTraits<MbArc3D>:_IsInstant { static const MbeSpaceType typeId = st_Arc3D; };

/*
  Topology sub-classes.
*/
template<>
struct MbClassTraits<MbFace>:_IsInstant { static const MbeTopologyType typeId = tt_Face; };
template<>
struct MbClassTraits<MbEdge>:_IsInstant { static const MbeTopologyType typeId = tt_Edge; };
template<>
struct MbClassTraits<MbCurveEdge>:_IsInstant { static const MbeTopologyType typeId = tt_CurveEdge; };
template<>
struct MbClassTraits<MbVertex>:_IsInstant { static const MbeTopologyType typeId = tt_Vertex; };

//----------------------------------------------------------------------------------------
// \ru Статическое приведение из типа <Type*> к <Type> (разадресация типа) \en Static cast from type <Type*> to <Type> 
//---
template<class _Type> struct Deref { private: typedef _Type Type; };
template<class _Type> struct Deref<const _Type*> { typedef _Type Type; };
template<class _Type> struct Deref<_Type*> { typedef _Type Type; };
template<class _Type> struct Deref<_Type&> { typedef _Type Type; };

//----------------------------------------------------------------------------------------
/** \brief  \ru Динамическое приведение типа, основанное на функции Derived::IsA(). 
            \en Dynamic type cast based on the function Derived::IsA().
*/
//---
template< class DerivedPtr, class ParentType >
inline DerivedPtr _IsaCast( ParentType * obj )
{ 
  MbClassTraits<typename Deref<DerivedPtr>::Type> isAClass;
  if ( isAClass(obj, isAClass.typeId) )
  {
    return static_cast<DerivedPtr>( obj );
  }
  return static_cast<DerivedPtr>( nullptr );
}

//----------------------------------------------------------------------------------------
//
//---
template< class DerivedPtr, class ParentPtr >
inline DerivedPtr _IsaCast( ParentPtr * obj, const MbSpaceItem * item )
{  
  if ( (obj != item) && obj->RefType() == rt_SpaceItem )
  {
    item = static_cast<const MbSpaceItem*>( obj );
  } 
  return _IsaCast<DerivedPtr>( item );
}

//----------------------------------------------------------------------------------------
//
//---
template< class DerivedPtr, class ParentPtr >
inline DerivedPtr _IsaCast( ParentPtr * obj, const MbTopItem * tItem )
{  
  if ( (obj != tItem) && obj->RefType() == rt_TopItem )
  {
    tItem = static_cast<const MbTopItem*>( obj );
  } 
  return _IsaCast<DerivedPtr>( tItem );
}

//----------------------------------------------------------------------------------------
/** \brief  \ru Динамическое приведение типа, основанное на функции Derived::IsA(). 
            \en Dynamic type cast based on the function Derived::IsA().
*/
//---
template< class DerivedPtr, class ParentType >
DerivedPtr isa_cast( ParentType * obj )
{  
  return _IsaCast<DerivedPtr>( obj );
}

//----------------------------------------------------------------------------------------
/** \brief  \ru Динамическое приведение типа, основанное на функции Derived::IsA(). 
            \en Dynamic type cast based on the function Derived::IsA().
*/
//---
template< class DerivedPtr >
DerivedPtr isa_cast( const MbRefItem * obj )
{  
  DerivedPtr resPtr = nullptr;
  return _IsaCast<DerivedPtr>( obj, resPtr );
}

//----------------------------------------------------------------------------------------
/** \brief  \ru Динамическое приведение типа, основанное на функции Derived::IsA(). 
            \en Dynamic type cast based on the function Derived::IsA().
*/
//---
template< class DerivedPtr >
DerivedPtr isa_cast( MbRefItem * obj )
{  
  DerivedPtr resPtr = nullptr;
  return _IsaCast<DerivedPtr>( obj, resPtr );
}

#endif // __MB_CLASS_TRAITS_H

// eof