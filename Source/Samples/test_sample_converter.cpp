//////////////////////////////////////////////////////////////////////////////////////////
//
//    The file contains source codes demonstrating samples 
//    how to read import and export models using exchange format files.
//
//    WARNING: This code is only an example. The real application can require more 
//             functionality and data types for implementation.
// 
//////////////////////////////////////////////////////////////////////////////////////////

#include <test_samples.h>

#include <model_entity.h>
#include <conv_model_exchange.h>
//
#include <last.h>

using namespace c3d;



//------------------------------------------------------------------------------
// 
// ---
static
void ModelExchange()
{
  MbModel model;
  const MbeConvResType importResult = ImportFromFile( model, WToPathstring(L"sample.stp") );
  if ( importResult == cnv_Success ) {
    ExportIntoFile( model, WToPathstring(L"sample.c3d") );
    ExportIntoFile( model, WToPathstring(L"sample.igs") );
  }
}


//------------------------------------------------------------------------------
// How to import and export models using exchange format files.
// ---
void SampleImportExport()
{
  ::ModelExchange();
}
