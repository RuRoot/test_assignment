////////////////////////////////////////////////////////////////////////////////
/**
\file
\brief \ru Примеры пользовательских атрибутов.
       \en Samples of user attributes. \~

*/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEST_SAMPLE_USER_ATTRIBUTES_H
#define __TEST_SAMPLE_USER_ATTRIBUTES_H

#include <tool_uuid.h>
#include <item_registrator.h>
#include <attr_user_attribute.h>
#include <mb_cart_point3d.h>


// Unique ID of an application that writes the data to a file stream         
static const MbUuid SAMPLE_APP_ID = string_generator()("{01234567-TEST-2016-USERAPPLICATION0}");

// Unique ID of user attribute 1
static const MbUuid USER_ATTR_ID1 = string_generator()("{01234567-TEST-2016-USERATTRIBUTE001}");
// Unique ID of user attribute 2
static const MbUuid USER_ATTR_ID2 = string_generator()("{01234567-TEST-2016-USERATTRIBUTE002}");


//------------------------------------------------------------------------------
// User identifier attribute
// ---
class MbUserIdentifier : public MbExternalAttribute {
private:
  uint32        id;
  c3d::string_t description;

public:
  MbUserIdentifier( uint32 n, const c3d::string_t & text ) : id( n ), description( text ) {}
  virtual ~MbUserIdentifier() {}

public:
  MbUserAttribType AttrTypeEx() const override { return USER_ATTR_ID1; }

  MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool          IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal.
  bool          Init( const MbAttribute & ) override;         // \ru Инициализировать данные по присланным. \en Initialize data.

  void          GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object.
  size_t        SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of the object. 

DECLARE_PERSISTENT_CLASS( MbUserIdentifier )
OBVIOUS_PRIVATE_COPY( MbUserIdentifier )
};

IMPL_PERSISTENT_OPS( MbUserIdentifier )


//------------------------------------------------------------------------------
// User test data attribute
// ---
class MbUserTestData : public MbExternalAttribute {
private:
  MbCartPoint3D pnt;
  c3d::string_t info;

public:
  MbUserTestData( const MbCartPoint3D & p, const c3d::string_t & text )
    : pnt( p )
    , info( text )
  { 
    SetActionForTransform( trn_Self );
    SetActionForMerge( mrg_Self );
  }
  virtual ~MbUserTestData() {}

public:
  MbUserAttribType AttrTypeEx() const override { return USER_ATTR_ID2; }

  MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool          IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal.
  bool          Init( const MbAttribute & ) override;         // \ru Инициализировать данные по присланным. \en Initialize data.

  // \ru Выполнить действия при трансформировании владельца. \en Perform actions when transforming the owner. 
  void          OnTransformOwner( const MbAttributeContainer & owner, const MbMatrix3D &, MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при перемещении владельца. \en Perform actions when moving the owner. 
  void          OnMoveOwner( const MbAttributeContainer & owner, const MbVector3D &, MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при вращении владельца. \en Perform actions when rotating the owner. 
  void          OnRotateOwner( const MbAttributeContainer & owner, const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при объединении владельца. \en Perform actions when merging the owner. 
  void          OnMergeOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override;

  void          GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object.
  size_t        SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of the object. 

  const MbCartPoint3D & GetPoint() const { return pnt; }
        MbCartPoint3D & SetPoint()       { return pnt; }
        void            SetPoint( const MbCartPoint3D & pnt0 ) { pnt = pnt0; }


DECLARE_PERSISTENT_CLASS( MbUserTestData )
OBVIOUS_PRIVATE_COPY( MbUserTestData )
};

IMPL_PERSISTENT_OPS( MbUserTestData )

#define CREATE_ATTR_DEF_INSTANCE( attrClassName, type ) \
  UserAttrDefinitionInstance<UserAttrDefinition<attrClassName>> inst ## _ ## attrClassName(type)


#endif // __TEST_SAMPLE_USER_ATTRIBUTES_H
