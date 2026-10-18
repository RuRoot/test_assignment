//////////////////////////////////////////////////////////////////////////////////////////
//
//    The file contains source codes demonstrating samples 
//    how to read and write c3d data as part of binary file.
//
//    WARNING: This code is only an example. The real application can require more 
//             functionality and data types for implementation.
// 
//////////////////////////////////////////////////////////////////////////////////////////

#include <test_samples.h>
#include <test_variables.h>
#include <test_manager.h>
#include <test_service.h>
//
#include <templ_s_array.h>
#include <templ_sptr.h>
#include <io_tape.h>
#include <io_memory_buffer.h>
#include <mb_operation_result.h>
#include <mb_cart_point3d.h>
#include <name_item.h>
#include <creator.h>
#include <solid.h>
#include <model_entity.h>
#include <action_solid.h>
//
#include <last.h>

using namespace c3d;


// Unique ID of an application that writes the data to a file stream         
static const MbUuid SAMPLE_APP_ID = string_generator()( "{01234567-TEST-2016-PARAMETRICSKETCH}" );

#if defined (C3D_WINDOWS) && !defined(ALL_WARNINGS) // _MSC_VER // Set warnings level
#pragma warning(push)                               // Preserve current state of warning settings
#pragma warning(disable: 4505)                      // 'XXX': unreferenced function with internal linkage has been removed
#endif
  

////////////////////////////////////////////////////////////////////////////////
// How to implement stream (serializable) classes.
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// A simple stream object.
// A simple object can live in a model only as a part of another object (for example, MbCartPoint3D, MbPlacement3D, etc).
// A simple object is read and written inside of another object, so its type is known at read/write time.
// 
// A simple stream object can be written to the stream and read from the stream using \en << and >> operators.
// ---

//------------------------------------------------------------------------------
// Simple object's class SimpleObj.
// ---
class SimpleObj
{
private:
  MbVector _value;
public:
  SimpleObj() : _value() {}
  SimpleObj( const MbVector & v ) : _value( v ) {}

  void   SetValue( const MbVector & v ) { _value = v; }
  MbVector GetValue() const { return _value; }

  /// Reading and writing operators. 
  KNOWN_OBJECTS_RW_REF_OPERATORS( SimpleObj );
};

//------------------------------------------------------------------------------
// Implementation of reading and writing operators for SimpleObj.
// ---
reader & operator >> ( reader & in, SimpleObj & ptr )
{
  in >> ptr._value;
  return in;
}

writer & operator << ( writer & out, const SimpleObj & ptr )
{
  out << ptr._value;
  return out;
}

//------------------------------------------------------------------------------
// Let's name an object, that is not simple, a core streaming object.
// A core stream object can live in the model on its own (for example, MbLineSegment3D)
// so its type can be unknown while reading/writing.
// 
// Class for which the type is unknown while reading and writing:
//   - Should inherit the class from TapeBase directly or via ancestors.
//   - Should set DECLARE_PERSISTENT_CLASS macro in the declaration of the class.
//   - Should set one of the following macros in the implementation of the class:
//     IMP_A_PERSISTENT_CLASS_WD for an abstract class which has no its own data fields for writing,
//     IMP_A_PERSISTENT_CLASS for an abstract class which has its own data fields for writing,
//     IMP_PERSISTENT_CLASS for an inherited class.
// ---

//------------------------------------------------------------------------------
// Abstract class definition.
// ---
class CoreObjBase : public TapeBase
{
public:
  virtual void  Calculate( double ) = 0;
  virtual void  Verify() const = 0;
  virtual ~CoreObjBase() {} 

protected:
  CoreObjBase() : TapeBase() {}

  DECLARE_PERSISTENT_CLASS( CoreObjBase )
};

//------------------------------------------------------------------------------
// CoreObjBase implementation.
// ---
CoreObjBase::CoreObjBase( TapeInit ) : TapeBase() // Constructor for reading
{
}
// Macro for an absract class which has no its own data fields for writing to stream
IMP_A_PERSISTENT_CLASS_WD( SAMPLE_APP_ID, CoreObjBase );


//----------------------------------------------------------------------------------------
// Inherited class definition.
//---
class CoreObj : public CoreObjBase
{
  SimpleObj _data;
public:
  CoreObj() : CoreObjBase(), _data() {}
  CoreObj( const MbVector &  v ) : CoreObjBase(), _data( v ) {}
  virtual ~CoreObj() {}

  void  Calculate( double ) override { /* ... */ }
  void  Verify() const      override { /* ... */ }

private:
  DECLARE_PERSISTENT_CLASS( CoreObj );
};

//------------------------------------------------------------------------------
// CoreObj implementation.
// ---
CoreObj::CoreObj( TapeInit tapeInit ) : CoreObjBase( tapeInit ) // Constructor for reading
{
}

void  CoreObj::Read( reader & in, CoreObj * obj ) // Reading function.
{
  ReadBase( in, static_cast<CoreObjBase *>( obj ) ); // Read base class.
  in >> obj->_data;
}

void  CoreObj::Write( writer & out, const CoreObj * obj ) // Writing function.
{
  WriteBase( out, static_cast<const CoreObjBase *>(obj) ); // Write base class.
  out << obj->_data;
}

IMP_PERSISTENT_CLASS( SAMPLE_APP_ID, CoreObj );



////////////////////////////////////////////////////////////////////////////////
// How to read and write c3d data as part of binary file.
///////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// 
// ---
inline
MbSolid * CreateSampleSphere( double radius, double xpos )
{
  SpacePointsVector points;
  points.push_back( MbCartPoint3D( xpos, 0.0, 0.0) );
  points.push_back( MbCartPoint3D( xpos, 0.0, radius) );
  points.push_back( MbCartPoint3D( xpos + radius, 0.0, 0.0) );
  MbSNameMaker operNames( ct_ElementarySolid, MbSNameMaker::i_SideNone, 0 );
  MbElementarySolidResults results;
  ::ElementarySolid( MbElementarySolidParams(ElementaryShellType::et_Sphere, points, operNames), results ); // ::ElementarySolid( points, ElementaryShellType::et_Sphere, operNames, sphere );

  PRECONDITION( results.GetResultSolid() != nullptr );
  return results.DetachSolid();
}

//------------------------------------------------------------------------------
// 
// ---
static
void ReadWriteModelAsBinaryPart()
{
  SPtr<MbModel> model;

  SPtr<MbSolid> solid1( ::CreateSampleSphere( 10.0,  0.0 ) );
  SPtr<MbSolid> solid2( ::CreateSampleSphere( 10.0, 50.0 ) );

  model = new MbModel;
  model->AddItem( *solid1, 1 );
  model->AddItem( *solid2, 2 );

  uint64 signature0 = 3210123; // sample signature of c3d-data in binary file.

  { // Write model items
    std::ofstream file( "c:/temp/rwTest.dat", std::ios::binary );

    if ( file.is_open() ) {
      membuf memBuf;
      {
        writer::writer_ptr wrt = writer::CreateMemWriter( memBuf, 0 );

        ::WriteModelItems( *wrt, *model );
        memBuf.setMaxRegCount( wrt->GetMaxRegisteredCount() );
        memBuf.closeBuff();

        uint64 bufsize = memBuf.getMemLen();
        const char * buffer = nullptr;
        memBuf.toMemory(buffer);

        file.write( reinterpret_cast<char *>(&signature0), sizeof(signature0) );
        file.write( reinterpret_cast<char *>(&bufsize), sizeof(bufsize) );
        file.write( buffer, std::streamsize(bufsize) );
        _ASSERT( !file.fail() );
        file.close();

        delete[] buffer;
      }
    }
  }

  model = nullptr;

  { // Read model items
    std::ifstream file("c:/temp/rwTest.dat",std::ios::binary);

    if ( file.is_open() ) {
      uint64 signature = 0;
      file.read( reinterpret_cast<char *>(&signature), sizeof(signature) );
      PRECONDITION( signature0 == signature );
      if ( signature0 == signature ) {
        uint64 bufsize = 0;
        file.read( reinterpret_cast<char *>(&bufsize), sizeof(bufsize) );
        PRECONDITION( bufsize > 0 );
        if ( bufsize > 0 ) {
          char * buffer = new char[bufsize];
          file.read( buffer, bufsize );
          file.close();

          model = new MbModel();

          membuf memBuf;
          memBuf.fromMemory( buffer );
          {
            reader::reader_ptr rdr = reader::CreateMemReader( memBuf, 0 );
            ::ReadModelItems( *rdr, *model );
            PRECONDITION( model->ItemsCount() == 2 );
          }
          delete [] buffer;
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// How to read and write c3d data as part of binary file.
// ---
void SampleReadWriteModelAsPart()
{
  ::ReadWriteModelAsBinaryPart();
}


////////////////////////////////////////////////////////////////////////////////
// How to read selected items from c3d file written in an extended format.
///////////////////////////////////////////////////////////////////////////////

/*
static
void ReadSelectedItemsFromFile()
{
  const TCHAR * fileName( _T("c:/temp/rwTest.c3d") );
  reader::reader_ptr reader( reader_ex::CreateReaderEx(std::unique_ptr<iobuf_Seq>(&::createiobuf(fileName) ), io::in) );

  // Read items by a type.
  std::vector< SPtr<MbItem> > solids;
  {
    MbItemData filter;
    filter.type = st_Solid; // Filter by item type (read solids).

    std::vector<MbItemData> filters;
    filters.push_back( filter );

    // Build the tree with data of objects to read by filtering the model tree.
    std::unique_ptr<const c3d::IModelTree> tree( reader->GetModelTree()->GetFilteredTree(filters) );
    ::ReadModelItemsFromTree( *reader, tree.get(), solids );
  }

  // Read items by an attribute.
  std::vector< SPtr<MbItem> > coloredItems;
  {
    MbItemData filter;
    ItemDataBase * color = ItemDataBase::Create( idtAttrColor ); // Data for a color attribute.
    static_cast<ItemAttrColor *>( color )->m_value = 192;
    filter.properties.Associate( idtAttrColor, color );  // Filter by color attribute.

    std::vector<MbItemData> filters;
    filters.push_back( filter );

    // Build the tree with data of objects to read by filtering the model tree.
    std::unique_ptr<const c3d::IModelTree> tree( reader->GetModelTree()->GetFilteredTree(filters) );
    ::ReadModelItemsFromTree( *reader, tree.get(), coloredItems );
  }

}
*/

#if defined (C3D_WINDOWS) && !defined(ALL_WARNINGS) // _MSC_VER // Set warnings level
#pragma warning(pop)                                // Restore state of warning settings
#endif
