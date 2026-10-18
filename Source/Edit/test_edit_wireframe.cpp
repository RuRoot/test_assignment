#include <test_manager.h>
#include <test_service.h>
#include <test_converter.h>
#include <tool_time_test.h>
#include <instance_item.h>
#include <wire_frame.h>
#include <alg_indicator.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Перестроить проволочный каркас.
// Rebuild a wire frame.
// ---
bool RebuildWireFrame( bool selectCopyMode )
{
  MbItem * wireFrameItem = SelectWireFrame();

  if ( wireFrameItem != nullptr ) {
    MbeCopyMode sameEdges = cm_KeepHistory;

    if ( selectCopyMode ) {
      sameEdges = cm_Copy;

      int cm = 0;
      GetInt( IDS_ENTER_MEMORY, cm );
      switch ( cm ) {
        case 0:  sameEdges = cm_Same;        break;
        case 1:  sameEdges = cm_KeepHistory; break;
        case 2:  sameEdges = cm_KeepSurface; break;
        default: sameEdges = cm_Copy;        break;
      }
    }

    SetWaitCursor( true );
    RPArray<MbSpaceItem> items( 0, 1 );
    TestVariables::viewManager->EraseObject( wireFrameItem );

    DPtr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyCommon ) );

#if defined(C3D_DEBUG)
    SET_TIME_TEST( true );
#endif // C3D_DEBUG
    uint32 miliSec = ::GetTickCountEx();

    bool rebuildRes = false;

    if ( selectCopyMode )
      rebuildRes = wireFrameItem->RebuildItem( sameEdges, nullptr, progBar );
    else
      rebuildRes = wireFrameItem->RebuildItem( sameEdges, &items, progBar );

    miliSec = ::GetTickCountEx() - miliSec;
#if defined(C3D_DEBUG)
    CHECK_TIME_TEST();
    TestVariables::viewManager->RefreshObjectModel( wireFrameItem );
#endif // C3D_DEBUG

    SetWaitCursor( false );

    if ( !rebuildRes || ( items.Count() > 0 ) ) {
      MessageBoxEx( RT_REBUILDING_NOT_COMPLETELY, IDS_WERE_ERRORS );
      if ( items.Count() > 0 ) {
        MessageBoxEx( RT_REBUILDING_WITH_CHANGED, IDS_OBJECTS_ADDED );
        bool showItem = false; // вылет под Release: нельзя перерисовывать всю модель (OpenGL), т.к. отрисовочная модель может ссылаться на убитые объекты
        for ( size_t i = 0; i < items.Count(); i++ ) {
          TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, items[i], showItem );
        }
      }
    }

    const MbWireFrame * result = nullptr;

    if ( wireFrameItem->IsA() == st_WireFrame )
      result = static_cast<MbWireFrame *>( wireFrameItem );
    else if ( wireFrameItem->IsA() == st_Instance ) {
      const MbInstance * inst = static_cast<const MbInstance *>( wireFrameItem );
      if ( inst->GetItem() != nullptr && inst->GetItem()->IsA() == st_WireFrame )
        result = static_cast<const MbWireFrame *>( inst->GetItem() );
    }

    if ( result != nullptr ) {
      TimeTest * testResult = nullptr;
#if defined(C3D_DEBUG)
      testResult = ::GetTimeTestResults();
#endif // C3D_DEBUG

      size_t operationCount = result->GetCreatorsCount();
      size_t edgesCount = result->GetEdgesCount();

      string_t rebuildMsg( RT_REBUILDING_WIREFRAME );

      if ( selectCopyMode ) {
        if ( sameEdges == cm_Same )
          rebuildMsg = RT_REBUILDING_WIREFRAME_WITHOUT_COPYING_EDGES;
        else if ( sameEdges == cm_KeepHistory )
          rebuildMsg = RT_REBUILDING_WIREFRAME_WITH_CONST_EDGES;
        else if ( sameEdges == cm_KeepSurface )
          rebuildMsg = RT_REBUILDING_WIREFRAME_WITH_SAME_CURV;
        else if ( sameEdges == cm_Copy )
          rebuildMsg = RT_REBUILDING_WIREFRAME_WITH_FULL_COPY;
      }
      if ( !rebuildRes )
        rebuildMsg = RT_REBUILDING_NOT_COMPLETELY;

      if ( rebuildRes )
        ShowTimeTestResult( testResult, operationCount, edgesCount, miliSec, st_WireFrame, rebuildMsg.c_str() );
    }

#if defined(C3D_DEBUG)
    SET_TIME_TEST( false );
#endif // C3D_DEBUG

    TestVariables::viewManager->RefreshObjectModel( wireFrameItem );

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Перестроить все тела и оболочки. 
// Rebuild all solids and shells.
// ---
bool RebuildAllWireFrames( bool selectCopyMode )
{
  RPArray<MbItem> objs( 0, 1 );
  SArray<MbMatrix3D> matrs( 0, 1 );

  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();

  if ( wind != nullptr )
    wind->GetObjects( st_WireFrame, objs, matrs );

  size_t i, objsCnt = objs.size();

  if ( objsCnt > 0 && wind != nullptr ) {

    MbeCopyMode sameEdges = cm_KeepHistory;

    if ( selectCopyMode ) {
      sameEdges = cm_Copy;

      int cm = 0;
      GetInt( IDS_ENTER_MEMORY, cm );
      switch ( cm ) {
        case 0:  sameEdges = cm_Same;        break;
        case 1:  sameEdges = cm_KeepHistory; break;
        case 2:  sameEdges = cm_KeepSurface; break;
        default: sameEdges = cm_Copy;        break;
      }
    }

    DPtr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyCommon ) );
    bool result = true;

    TestVariables::viewManager->DeleteTemporal();
    wind->RefreshOGL();
    size_t edgesCount = 0;

    uint32 miliSec = ::GetTickCountEx();
    SetWaitCursor( true );

    for ( i = 0; i < objsCnt; ++i ) {
      MbItem * wireFrameItem = objs[i];
      bool rebuildRes = wireFrameItem->RebuildItem( sameEdges, nullptr, progBar );
      wind->RedrawObject( wireFrameItem );
      if ( wireFrameItem->IsA() == st_WireFrame )
        edgesCount += static_cast<MbWireFrame *>( wireFrameItem )->GetEdgesCount();
      if ( !rebuildRes )
        result = false;
    }

    SetWaitCursor( false );
    miliSec = ::GetTickCountEx() - miliSec;

    if ( result ) {
      TimeTest * testResult = nullptr;
      size_t operationCount = objsCnt;
      string_t rebuildMsg( RT_REBUILDING_WIREFRAME );

      if ( selectCopyMode ) {
        if ( sameEdges == cm_Same )
          rebuildMsg = RT_REBUILDING_WIREFRAME_WITHOUT_COPYING_EDGES;
        else if ( sameEdges == cm_KeepHistory )
          rebuildMsg = RT_REBUILDING_WIREFRAME_WITH_CONST_EDGES;
        else if ( sameEdges == cm_KeepSurface )
          rebuildMsg = RT_REBUILDING_WIREFRAME_WITH_SAME_CURV;
        else if ( sameEdges == cm_Copy )
          rebuildMsg = RT_REBUILDING_WIREFRAME_WITH_FULL_COPY;
      }
      if ( !result )
        rebuildMsg = RT_REBUILDING_NOT_COMPLETELY;

      if ( result )
        ShowTimeTestResult( testResult, operationCount, edgesCount, miliSec, st_WireFrame, rebuildMsg.c_str() );
    }
    else
      MessageBoxEx( RT_REBUILDING_NOT_COMPLETELY, IDS_WERE_ERRORS );

    return true;
  }

  return false;
}
