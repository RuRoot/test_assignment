//////////////////////////////////////////////////////////////////////////////////////////
//  Модуль: TEST_COMANAGER
//  Цель:   Менеджер геометрических ограничений для MbModel
//                                                                                 MA 2014
//////////////////////////////////////////////////////////////////////////////////////////

#include <test_comanager.h>
#include <model_entity.h>
#include <assembly.h>
#include <constraint.h>
#include <mb_class_traits.h>
#include <test_manager.h>
#include <test_variables.h>
#include <last.h>

using namespace c3d;

//------------------------------------------------------------------------------
// Сборка, выбранная в качестве "текущей"
// ---
static MbModel * _CurrentModel()
{
  if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() )
  {
    return &wind->SetGeomModel();
  }
  return nullptr;
}

//------------------------------------------------------------------------------
// Запросить или создать новую сборку
// ---
static MbAssembly * _PrepareAssembly()
{
  if ( MbModel * model = _CurrentModel() ) 
  {
    for ( MbModel::ItemIterator iter = model->Begin(), last = model->End(); iter != last; ++iter )
    { 
      if ( MbAssembly * assembly = isa_cast<MbAssembly *>(*iter) )
      {
        return assembly;
      }
    }
    // (!) Пока решим вопрос примитивно
    SPtr<MbAssembly> assembly( new MbAssembly );
    return static_cast<MbAssembly*>( model->AddItem(*assembly) );

  }
  return nullptr;
}

//------------------------------------------------------------------------------
// Найти первую сборку с сопряжениями
// ---
static MbAssembly * _FirstAssembly()
{
  if ( MbModel * model = _CurrentModel() ) 
  {
    for ( MbModel::ItemIterator iter = model->Begin(), last = model->End(); iter != last; ++iter )
    { 
      if ( MbAssembly * assembly = isa_cast<MbAssembly *>(*iter) )
      {
        return assembly;
      }
    }
  }
  return nullptr;
}
/*
//----------------------------------------------------------------------------------------
// Подготовить аргумент для встраивания в систему сопряжений
//---
static void _Prepare( MtGeomArgument & ga )
{
  if ( MbModel * model = _CurrentModel() ) 
  {    
    if ( MbAssembly * assm = _PrepareAssembly() )
    {
      if ( const MbItem * item = ga.HostItem() )
      {
        if ( item == model->SubItem(item->GetItemName()) )
        {
          SPtr<MbItem> itemSptr( const_cast<MbItem *>(item) );
          model->DetachItem( itemSptr );
          assm->AddItem( *itemSptr );
          ga = MtGeomArgument( ga.PropItem(), itemSptr );
        }
      }
    }
  }
}
*/

//-----------------------------------------------------------------------------
//
//---
ConstraintManager::ConstraintManager() {}

//----------------------------------------------------------------------------------------
//
//---
ConstraintManager::~ConstraintManager() {}

//-----------------------------------------------------------------------------
// Добавить новое ограничение в систему
//---
bool ConstraintManager::AddConstraint( MtMateType cType
                                     , const MtGeomArgument & ga1, const MtGeomArgument & ga2
                                     , MtParVariant parVar )
{
  if ( MbAssembly * block = _PrepareAssembly() )
  {
    //_Prepare( ga1 );
    //_Prepare( ga2 );
    return block->AddConstraint( cType, ga1, ga2, parVar ).ConstraintType() == cType;
  }
  return false;
}

//-----------------------------------------------------------------------------
/// Рассчитать систему ограничений
//---
MtResultCode3D ConstraintManager::Evaluate()
{
  if ( MbAssembly * block = _FirstAssembly() )
  {
    return block->EvaluateConstraints();
  }
  return GCM_RESULT_None;
}

//-----------------------------------------------------------------------------
/// Применить решение
//---
void ConstraintManager::Apply()
{
  // m_gcFormer.ApplySolution();
}

//-----------------------------------------------------------------------------
/// Очистить весь контекст решателя
//---
void ConstraintManager::Clear()
{
  // m_gcFormer.Clear();
}

// eof