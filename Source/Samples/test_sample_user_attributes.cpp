//////////////////////////////////////////////////////////////////////////////////////////
//
//    The file contains a source code demonstrating a sample how to use attributes.
//
//    WARNING: This code is only an example. The real application can require more 
//             functionality and data types for implementation.
// 
//////////////////////////////////////////////////////////////////////////////////////////

#include <test_sample_user_attributes.h>
#include <test_variables.h>
#include <test_manager.h>
#include <test_service.h>
//
#include <mb_property.h>
#include <mb_property_title.h>
//
#include <last.h>

using namespace c3d;

CREATE_ATTR_DEF_INSTANCE( MbUserIdentifier, USER_ATTR_ID1 );
CREATE_ATTR_DEF_INSTANCE( MbUserTestData,   USER_ATTR_ID2 );


//------------------------------------------------------------------------------
//
// ---
MbUserIdentifier::MbUserIdentifier( TapeInit )
  : id( SYS_MAX_UINT32 )
  , description()
{}

//------------------------------------------------------------------------------
//
// ---
MbUserIdentifier::MbUserIdentifier( const MbUserIdentifier & a )
  : MbExternalAttribute {}
  , id( a.id )
  , description( a.description )
{}

//------------------------------------------------------------------------------
//
// ---
MbAttribute & MbUserIdentifier::Duplicate( MbRegDuplicate * iReg ) const
{
  __REG_DUPLICATE_IMPL( MbUserIdentifier( *this ) )
    return (MbAttribute &)*copyItem;
}

//------------------------------------------------------------------------------
//
// ---
bool MbUserIdentifier::IsSame( const MbAttribute & attr, double ) const
{
  if ( AttributeFamily() == attr.AttributeFamily() ) {
    const MbExternalAttribute & userAttr = static_cast<const MbExternalAttribute &>(attr);
    if ( userAttr.AttrTypeEx() == AttrTypeEx() ) {
      const MbUserIdentifier & other = static_cast<const MbUserIdentifier &>(userAttr);
      if ( id == other.id && description == other.description )
        return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
//
// ---
bool MbUserIdentifier::Init( const MbAttribute & attr )
{
  if ( attr.AttributeFamily() == AttributeFamily() ) {
    const MbExternalAttribute & userAttr = static_cast<const MbExternalAttribute &>(attr);
    if ( userAttr.AttrTypeEx() == AttrTypeEx() ) {
      const MbUserIdentifier & other = static_cast<const MbUserIdentifier &>(userAttr);
      id = other.id;
      description = other.description;
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
// выдать свойства объекта
// ---
void MbUserIdentifier::GetProperties( MbProperties & properties )
{
  properties.Add( new UIntProperty( IDS_ITEM_0768, id, false ) );
  properties.Add( new StringProperty( IDS_PROP_1033, description.c_str(), false ) );
}

//------------------------------------------------------------------------------
// установить свойства объекта
// ---
size_t MbUserIdentifier::SetProperties( const MbProperties & )
{
  return 2;
}

IMP_PERSISTENT_CLASS( SAMPLE_APP_ID, MbUserIdentifier );

//------------------------------------------------------------------------------
// чтение из потока
// ---
void MbUserIdentifier::Read( reader & in, MbUserIdentifier * obj )
{
  ReadBase( in, static_cast<MbExternalAttribute *>(obj) );
  in >> obj->id;
  in >> obj->description;
}


//------------------------------------------------------------------------------
// запись в поток
// ---
void MbUserIdentifier::Write( writer & out, const MbUserIdentifier * obj )
{
  WriteBase( out, static_cast<const MbExternalAttribute *>(obj) );
  out << obj->id;
  out << obj->description;
}


//------------------------------------------------------------------------------
//
// ---
MbUserTestData::MbUserTestData( TapeInit )
  : pnt ()
  , info()
{
  SetActionForTransform( trn_Self );
  SetActionForMerge( mrg_Self );
}

//------------------------------------------------------------------------------
//
// ---
MbUserTestData::MbUserTestData( const MbUserTestData & a )
  : MbExternalAttribute {}
  , pnt( a.pnt )
  , info ( a.info )
{
  InitActions( a );
}

//------------------------------------------------------------------------------
//
// ---
MbAttribute & MbUserTestData::Duplicate( MbRegDuplicate * iReg ) const
{
  __REG_DUPLICATE_IMPL( MbUserTestData( *this ) )
    return (MbAttribute &)*copyItem;
}

//------------------------------------------------------------------------------
//
// ---
bool MbUserTestData::IsSame( const MbAttribute & attr, double ) const
{
  if ( AttributeFamily() == attr.AttributeFamily() ) {
    const MbExternalAttribute & userAttr = static_cast<const MbExternalAttribute &>(attr);
    if ( userAttr.AttrTypeEx() == AttrTypeEx() ) {
      const MbUserTestData & other = static_cast<const MbUserTestData &>(userAttr);
      if ( pnt == other.pnt && info == other.info )
        return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
//
// ---
bool MbUserTestData::Init( const MbAttribute & attr )
{
  if ( attr.AttributeFamily() == AttributeFamily() ) {
    const MbExternalAttribute & userAttr = static_cast<const MbExternalAttribute &>(attr);
    if ( userAttr.AttrTypeEx() == AttrTypeEx() ) {
      const MbUserTestData & other = static_cast<const MbUserTestData &>(userAttr);
      pnt = other.pnt;
      info = other.info;
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
// Действия при трансформировании владельца.
// ---
void MbUserTestData::OnTransformOwner( const MbAttributeContainer & /*owner*/, const MbMatrix3D & matr, MbRegTransform * )
{
  pnt.Transform( matr );
}


//------------------------------------------------------------------------------
// Действия при перемещении владельца.
// ---
void MbUserTestData::OnMoveOwner( const MbAttributeContainer & /*owner*/, const MbVector3D & to, MbRegTransform * )
{
  pnt.Move( to );
}


//------------------------------------------------------------------------------
// Действия при вращении владельца.
// ---
void MbUserTestData::OnRotateOwner( const MbAttributeContainer & /*owner*/, const MbAxis3D & axis, double angle, MbRegTransform * )
{
  pnt.Rotate( axis, angle );
}


//------------------------------------------------------------------------------
// \ru Выполнить действия при объединении владельца. \en Perform actions when merging the owner. 
// ---
void MbUserTestData::OnMergeOwner( const MbAttributeContainer & owner, MbAttributeContainer & other )
{
  if ( &owner != &other ) {
    AttrVector similarAttributes;
    other.GetAttributes( similarAttributes, at_UserAttribute, MbUserAttribute::AttributeType( USER_ATTR_ID2 ) );

    size_t k, cnt;
    for ( k = 0, cnt = similarAttributes.size(); k < cnt; k++ ) {
      MbUserAttribute * userAttr = static_cast<MbUserAttribute *>( similarAttributes[k] );
      if ( userAttr != nullptr ) {
        const MbExternalAttribute * extAttr = userAttr->GetExternalAttribute();
        if ( extAttr != nullptr && extAttr->AttrTypeEx() == AttrTypeEx() ) {
          const MbUserTestData * utData = static_cast<const MbUserTestData *>( extAttr );
          pnt = (utData->pnt + pnt) * 0.5;
          userAttr->SetExternalAttribute( this );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// выдать свойства объекта
// ---
void MbUserTestData::GetProperties( MbProperties & properties )
{
  properties.Add( new MathItemCopyProperty<MbCartPoint3D>( IDS_PROP_0223, pnt, true ) );
  properties.Add( new StringProperty( IDS_PROP_1043, info.c_str(), false ) );
}

//------------------------------------------------------------------------------
// установить свойства объекта
// ---
size_t MbUserTestData::SetProperties( const MbProperties & )
{
  return 2;
}

IMP_PERSISTENT_CLASS( SAMPLE_APP_ID, MbUserTestData );

//------------------------------------------------------------------------------
// чтение из потока
// ---
void MbUserTestData::Read( reader & in, MbUserTestData * obj )
{
  ReadBase( in, static_cast<MbExternalAttribute *>(obj) );
  in >> obj->pnt;
  in >> obj->info;
}


//------------------------------------------------------------------------------
// запись в поток
// ---
void MbUserTestData::Write( writer & out, const MbUserTestData * obj )
{
  WriteBase( out, static_cast<const MbExternalAttribute *>(obj) );
  out << obj->pnt;
  out << obj->info;
}

// eof