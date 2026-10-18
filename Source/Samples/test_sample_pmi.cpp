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

#include <space_instance.h>
#include <point3d.h>
#include <conv_model_exchange.h>
#include <mb_pmi.h>
#include <assembly.h>
//
#include <math_namespace.h>
#include <last.h>

using namespace c3d;



//------------------------------------------------------------------------------
// 
// ---
static
void WritePMISamples()
{
  SPtr<MbPMI> workGeneral, workTR;
  workGeneral = MbPMI::CreateGeneral( MbPlacement3D{MbCartPoint3D{10.,-25., 31.4}}, 
                            _T("some PMI"), 
                            c3d::ItemsSPtrVector{ c3d::ItemSPtr{ new MbSpaceInstance{ * new MbPoint3D{} } } }, 
                            std::vector<SPtr<MbTextItem>>{ SPtr<MbTextItem>{ new MbTextLiteral{ _T("some text"), MbPlacement{}, MbeTextLiteralForm::unformattedString } } } );

  workTR = MbPMI::CreateTechnicalRequirements( MbPlacement3D{MbCartPoint3D{10.,-25., 31.4}}, 
                            _T("some technical requirements"), 
                            c3d::ItemsSPtrVector{ c3d::ItemSPtr{ new MbSpaceInstance{ * new MbPoint3D{ MbCartPoint3D{ 10., 20., 30. } } } } }, 
                            std::vector<SPtr<MbTextItem>>{ SPtr<MbTextItem>{ new MbTextLiteral{ _T("some text of requirements"), MbPlacement{}, MbeTextLiteralForm::formattedString } } } );
  auto rootAssm = new MbAssembly{};
  rootAssm->AddItem( * new MbSpaceInstance{ *workGeneral } );
  rootAssm->AddItem( * new MbSpaceInstance{ *workTR } );
  c3d::ItemSPtr rootGuard{ rootAssm };
  c3d::ExportIntoFile( *rootGuard, c3d::StdToPathstring("D:/spag/23/letters/0613/pmi-c3d.c3d") );
}


//------------------------------------------------------------------------------
// How to import and export models using exchange format files.
// ---
void SampleWritePMI()
{
  ::WritePMISamples();
}
