//////////////////////////////////////////////////////////////////////////////////////////
//
//    The file contains a source code demonstrating a sample how to use attributes.
//
//    WARNING: This code is only an example. The real application can require more 
//             functionality and data types for implementation.
// 
//////////////////////////////////////////////////////////////////////////////////////////

#include <test_samples.h>
#include <test_sample_user_attributes.h>
#include <test_variables.h>
#include <test_manager.h>
#include <test_service.h>
//
#include <templ_sptr.h>
#include <io_tape.h>
#include <item_registrator.h>
#include <attribute_item.h>
#include <attribute_container.h>
#include <attr_color.h>
#include <attr_common_attribute.h>
#include <attr_user_attribute.h>
#include <solid.h>
#include <action_solid.h>
#include <alg_draw.h>
//
#include <last.h>

using namespace c3d;

const size_t delay_msec1 = 250;
const size_t delay_msec2 = 500;
const size_t delay_msec3 = 1000;

static bool useSleep = true; // false;
static bool rwFirst = true;


//------------------------------------------------------------------------------
//
// ---
static
MbSolid * CreateCylinderSolid( SimpleName uniqueMainName )
{
  double radius = 20;
  double height = 100;

  MbCartPoint3D p0, pz, px;
  p0.SetZero();
  pz.Init( 0, 0, height );
  px.Init( radius, 0, 0 );

  SpacePointsVector points;
  points.push_back( p0 );
  points.push_back( pz );
  points.push_back( px );

  MbSNameMaker names( uniqueMainName, MbSNameMaker::i_SideNone, 0 );
  MbElementarySolidResults results;
  ::ElementarySolid( MbElementarySolidParams( et_Cylinder, points, names ), results ); // ::ElementarySolid( points, et_Cylinder, names, solid );

  return results.DetachSolid();
}

//------------------------------------------------------------------------------
// Write\Read Model
// ---
static
bool WriteReadModel( SPtr<MbSolid> & solid )
{
  if ( TestVariables::viewManager->GetActiveWindow()->IsModelEmpty() ) {
    TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, solid, false );
  }

  // Write model
  if ( rwFirst ) {
    WriteModel( nullptr, GetCurrentMathFileVersion() ); // Function also writes used pathname to TestVariables::nameBuffer
    rwFirst = false;
  }
  else {
    WriteModel( TestVariables::nameBuffer, GetCurrentMathFileVersion() ); // Function also writes used pathname to TestVariables::nameBuffer
  }

  solid = nullptr;
  TestVariables::viewManager->DeleteModel();
  if ( useSleep )
    ::SleepCP( delay_msec2 );

  // Read model
  ReadModel( TestVariables::nameBuffer );

  RPArray<MbItem> items;
  SArray<MbMatrix3D> matrs;
  TestVariables::viewManager->GetObjects( st_Solid, items, matrs );

  if ( items.size() > 0 && items[0] != nullptr )
    solid = static_cast<MbSolid *>( items[0] );
  PRECONDITION( solid != nullptr );
  return (solid != nullptr);
}


//------------------------------------------------------------------------------
// Sample with string attributes
// ---
static
void SampleStringAttributes( MbSolid & solid )
{
  if ( solid.GetFacesCount() != 3 ) {
    PRECONDITION( false );
    return;
  }

  AttrVector attributes;

  string_t strId_FaceWithColor( _T( "id_UniqueString_FaceWithOwnColor" ) );
  string_t strId_Dummy        ( _T( "id_UniqueString_Dummy" ) );
  string_t strId_SolidInfo    ( _T( "id_UniqueString_SolidInfo" ) );

  string_t strMsg_FaceWithColor( _T( "Face with own color" ) );
  string_t strMsg_FaceWithColor1( _T( "Face with own color (modified)" ) );
  string_t strMsg_Dummy    ( _T( "Dummy" ) );
  string_t strMsg_SolidInfo( _T( "Cylinder with attributes" ) );

  bool checkSame = false;
  bool canEdit = false;

  // add
  SPtr<MbStringAttribute> strAttr( new MbStringAttribute( strId_SolidInfo, false, strMsg_SolidInfo ) );
  solid.AddAttribute( strAttr, checkSame ); // this attribute will be added
  // check
  solid.GetAttributes( attributes, at_CommonAttribute, at_StringAttribute );
  PRECONDITION( attributes.size() == 1 );
  attributes.clear();
  // try to add
  checkSame = true;
  solid.AddAttribute( strAttr, checkSame ); // but this - not - because the same is exist
  // check
  solid.GetAttributes( attributes, at_CommonAttribute, at_StringAttribute );
  PRECONDITION( attributes.size() == 1 );
  attributes.clear();

  canEdit = true;
  SPtr<MbStringAttribute> strAttr1( new MbStringAttribute( strId_FaceWithColor, canEdit, strMsg_FaceWithColor ) );
  canEdit = false;
  SPtr<MbStringAttribute> strAttr2( new MbStringAttribute( strId_Dummy, canEdit, strMsg_Dummy ) );

  PRECONDITION( solid.IsColored() );
  uint32 solidColor = solid.GetColor();

  size_t k, facesCnt = solid.GetFacesCount();
  for ( k = 0; k < facesCnt; ++k ) {
    MbFace * face = solid.GetFace( k );
    if ( (face == nullptr) || !face->IsColored() ) // skip faces without own color
      continue;
    uint32 faceColor = face->GetColor();
    if ( faceColor != solidColor ) {
      MbAttribute * attr = face->AddAttribute( strAttr2, true ); // add original

      attr = face->AddAttribute( strAttr1, true ); // add original
      PRECONDITION( attr != nullptr && attr == strAttr1 ); // check
      face->RemoveAttribute( attr ); // remove original

      attr = face->AddAttribute( *strAttr1, true ); // add copy
      PRECONDITION( attr != nullptr && attr != strAttr1 ); // check

      face->GetCommonAttributes( attributes, strId_FaceWithColor, at_StringAttribute );
      PRECONDITION( attributes.size() == 1 ); // check
      PRECONDITION( attr == attributes[0] ); // we found added attribute

      static_cast<MbStringAttribute *>(attributes[0])->SetValue( strMsg_FaceWithColor1 ); // change string content
    }
  }
}


//------------------------------------------------------------------------------
//
// ---
void SampleAttributes()
{
  TestVariables::viewManager->DeleteModel();

  // Create cylinder solid
  SPtr<MbSolid> solid( ::CreateCylinderSolid( 0 ) );

  // Add solid to manager
  TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, solid );
  if ( useSleep )
    ::SleepCP( delay_msec1 );

  // Set solid and face colors
  {
    // Set solid color
    solid->SetColor( TRGB_OLIVE );
    TestVariables::viewManager->RefreshModel();
    TestVariables::viewManager->ShowModel();
    if ( useSleep )
      ::SleepCP( delay_msec1 );

    // Set face color
    solid->GetFace(0)->SetColor( TRGB_TANGERINE );
    TestVariables::viewManager->RefreshModel();
    TestVariables::viewManager->ShowModel();
    if ( useSleep )
      ::SleepCP( delay_msec1 );
  }

  // Write\Read Model
  ::WriteReadModel( solid );

  // Actions with string attribute
  if ( solid != nullptr )
    ::SampleStringAttributes( *solid );

  // Write\Read Model
  ::WriteReadModel( solid );

  if ( solid != nullptr ) { // Actions with user attribute
    uint32 userId0 = 100;
    string_t userMsg0( _T( "Face" ) );
    MbCartPoint3D pnt;

    size_t k, faceCnt = solid->GetFacesCount();
    for ( k = 0; k < faceCnt; k++ ) {
      SPtr<MbUserIdentifier> userAttr0( new MbUserIdentifier( userId0+(uint32)k, userMsg0 ) );
      SPtr<MbAttribute> attr( solid->GetFace( k )->AddAttribute( userAttr0, false ) );
      PRECONDITION( attr->AttributeFamily() == at_UserAttribute );
      PRECONDITION( attr != userAttr0 );
      {
        PRECONDITION( solid->GetFace( k )->AddAttribute( userAttr0, true ) == nullptr );
      }
      if ( attr->AttributeFamily() == at_UserAttribute ) {
        MbUserAttribute * userAttr = &static_cast<MbUserAttribute &>( *attr );
        PRECONDITION( userAttr->AttributeType() != userAttr0->AttributeType() );
        PRECONDITION( userAttr->AttributeType() == MbUserAttribute::AttributeType( userAttr0->AttrTypeEx()) );
        userAttr = nullptr;
      }
      if ( k == 0 ) {
        solid->GetFace( 0 )->Point( 0.5, 0.5, pnt );
        string_t userMsg1( _T( "Point + info" ) );
        SPtr<MbUserTestData> userAttr1( new MbUserTestData( pnt, userMsg1 ) );
        attr = solid->GetFace( 0 )->AddAttribute( userAttr1, false );
      }
      userAttr0 = nullptr;
    }
  }

  // Write\Read Model
  ::WriteReadModel( solid );

  if ( solid != nullptr ) { // Actions with user attribute
    SPtr<MbExternalAttribute> extAttr1, extAttr2;

    std::vector<MbUserAttribute *> userAttributes;
    solid->GetFace( 0 )->GetUserAttributes( userAttributes, USER_ATTR_ID1 );
    PRECONDITION( userAttributes.size() == 1 );
    MbUserAttribute * userAttr = userAttributes[0];

    MbUserAttribType attrId;
    userAttr->GetUserAttribId( attrId );
    IAttrDefinition * attrDef = ::GetUserAttrDefinition( attrId );

    if ( attrDef != nullptr )
      extAttr1 = attrDef->AdvanceUserAttrib( *userAttr );

    PRECONDITION( extAttr1 != nullptr );

    userAttributes.clear();
    solid->GetFace( 0 )->GetUserAttributes( userAttributes, USER_ATTR_ID2 );
    PRECONDITION( userAttributes.size() == 1 );
    userAttr = userAttributes[0];

    userAttr->GetUserAttribId( attrId );
    attrDef = ::GetUserAttrDefinition( attrId );

    if ( attrDef != nullptr )
      extAttr2 = attrDef->AdvanceUserAttrib( *userAttr );

    PRECONDITION( extAttr2 != nullptr );

    AttrVector attributes;
    solid->GetFace( 0 )->GetAttributes( attributes, at_UserAttribute, at_Undefined );
    PRECONDITION( attributes.size() == 2 );
    attributes.clear();
    solid->GetFace( 0 )->GetAttributes( attributes, at_UserAttribute, MbUserAttribute::AttributeType( USER_ATTR_ID2 ) );
    PRECONDITION( attributes.size() == 1 );
  }

  if ( solid != nullptr ) { // изменение пользовательских атрибутов и их слияние
    std::vector<MbUserAttribute *> userAttributes;
    solid->GetFace( 0 )->GetUserAttributes( userAttributes, USER_ATTR_ID2 );
    if ( !userAttributes.empty() ) {
      MbUserAttribute * userAttr = userAttributes.front();
      PRECONDITION( userAttr->GetExternalAttribute() );
      if ( userAttr->GetExternalAttribute() ) {
        SPtr<MbExternalAttribute> extAttr( &static_cast<MbExternalAttribute &>(userAttr->GetExternalAttribute()->Duplicate()) );
        if ( extAttr != nullptr ) {
          MbUserTestData * utData = &static_cast<MbUserTestData &>( *extAttr );
          utData->SetPoint().SetZero();
          userAttr->SetExternalAttribute( *utData );
        }
      }
    }

    SPtr<MbSolid> solid1( static_cast<MbSolid *>( &solid->Duplicate() ) );
    MbVector3D move( 0, 0, 10 );
    solid1->Move( move );

    MbSNameMaker snMaker;
    MbeCopyMode cm = cm_Copy;

    SPtr<MbSolid> unionSolid;
    ::BooleanResult( solid, cm, solid1, cm, MbBooleanOperationParams( bo_Union, true, snMaker ), unionSolid ); //::BooleanSolid( *solid, cm, *solid1, cm, bo_Union, snMaker, resSolid );

    if ( unionSolid && unionSolid->GetFacesCount() > 0 ) {
      MbCartPoint3D testPnt( 0, 0, 5 );
      for ( size_t k = unionSolid->GetFacesCount(); k--; ) {
        const MbFace * face = unionSolid->GetFace( k );
        userAttributes.clear();
        face->GetUserAttributes( userAttributes, USER_ATTR_ID2 );
        if ( !userAttributes.empty() ) {
          const MbUserTestData * utData = static_cast<const MbUserTestData *>( userAttributes.front()->GetExternalAttribute() );
          if ( !c3d::EqualPoints( utData->GetPoint(), testPnt, LENGTH_EPSILON ) ) {
            PRECONDITION( false );
            solid1 = nullptr;
          }
        }
      }

      solid = unionSolid;
      unionSolid = nullptr;

      TestVariables::viewManager->DeleteModel();
      ::WriteReadModel( solid );
    }
  }
}

// eof