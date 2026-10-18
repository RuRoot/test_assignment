#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <test_draw.h>
#include <test_converter.h>
#include <tool_time_test.h>
#include <templ_dptr.h>
#include <attr_color.h>
#include <cur_cone_spiral.h>
#include <cur_line3d.h>
#include <cur_line_segment3d.h>
#include <cur_plane_curve.h>
#include <cur_polyline3d.h>
#include <cur_surface_intersection.h>
#include <surface.h>
#include <surf_plane.h>
#include <surf_curve_bounded_surface.h>
#include <surf_cylinder_surface.h>
#include <surf_sphere_surface.h>
#include <surf_torus_surface.h>
#include <topology_faceset.h>
#include <part_solid.h>
#include <solid.h>
#include <instance_item.h>
#include <assembly.h>
#include <point_frame.h>
#include <wire_frame.h>
#include <space_instance.h>
#include <mesh_primitive.h>
#include <cr_attribute_provider.h>
#include <cr_simple_creator.h>
#include <alg_base.h>
#include <alg_indicator.h>
#include <action_general.h>
#include <action_mesh.h>
#include <action_point.h>
#include <action_shell.h>
#include <action_solid.h>
#include <action_surface_curve.h>
#include <name_check.h>
#include <name_item.h>
#include <check_geometry.h>
#include <tri_face.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Перестроить тело.
// Rebuild a solid.
// ---
bool RebuildSolid( bool selectCopyMode )
{
  MbItem * solidItem = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &solidItem ) ) {
    MbeCopyMode sameShell = cm_KeepHistory;

    if ( selectCopyMode ) {
      sameShell = cm_Copy;

      int cm = 0;
      GetInt( IDS_ENTER_MEMORY, cm );
      switch ( cm ) {
        case 0  : sameShell = cm_Same;        break;
        case 1  : sameShell = cm_KeepHistory; break;
        case 2  : sameShell = cm_KeepSurface; break;
        default : sameShell = cm_Copy;        break;
      }
    }

    SetWaitCursor( true );
    RPArray<MbSpaceItem> items( 0, 1 );
    TestVariables::viewManager->EraseObject( solidItem );

    DPtr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyCommon ) );

#if defined(C3D_DEBUG)
    SET_TIME_TEST(true);
#endif // C3D_DEBUG
    uint32 miliSec = ::GetTickCountEx();

    bool rebuildRes = false;

    if ( selectCopyMode )
      rebuildRes = solidItem->RebuildItem( sameShell, nullptr, progBar );
    else
      rebuildRes = solidItem->RebuildItem( sameShell, &items, progBar );

    miliSec = ::GetTickCountEx() - miliSec;
#if defined(C3D_DEBUG)
    CHECK_TIME_TEST();
    TestVariables::viewManager->RefreshObjectModel( solidItem );
#endif // C3D_DEBUG

    SetWaitCursor( false );

    if ( !rebuildRes || (items.Count() > 0) ) {
      MessageBoxEx( RT_REBUILDING_NOT_COMPLETELY, IDS_WERE_ERRORS );
      if ( items.Count() > 0 ) {
        MessageBoxEx( RT_REBUILDING_WITH_CHANGED, IDS_OBJECTS_ADDED );
        bool showItem = false; // вылет под Release: нельзя перерисовывать всю модель (OpenGL), т.к. отрисовочная модель может ссылаться на убитые объекты
        for ( size_t i = 0; i < items.Count(); i++ ) {
          TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, items[i], showItem );
        }
      }
    }

    const MbSolid * result = nullptr;

    if ( solidItem->IsA() == st_Solid )
      result = static_cast<MbSolid *>(solidItem);
    else if ( solidItem->IsA() == st_Instance ) {
      const MbInstance * inst = static_cast<const MbInstance *>( solidItem );
      if ( inst->GetItem() != nullptr && inst->GetItem()->IsA() == st_Solid )
        result = static_cast<const MbSolid *>( inst->GetItem() );
    }

    if ( result != nullptr ) {
      TimeTest * testResult = nullptr;
#if defined(C3D_DEBUG)
      testResult = ::GetTimeTestResults();
#endif // C3D_DEBUG

      size_t operationCount = result->GetCreatorsCount();
      size_t facesCount = result->GetFacesCount();

      string_t rebuildMsg( RT_REBUILDING_BODY );

      if ( selectCopyMode ) {
        if ( sameShell == cm_Same )
          rebuildMsg = RT_REBUILDING_SHELL_WITHOUT_COPYING_FACES;
        else if ( sameShell == cm_KeepHistory )
          rebuildMsg = RT_REBUILDING_SHELL_WITH_CONST_FACES;
        else if ( sameShell == cm_KeepSurface )
          rebuildMsg = RT_REBUILDING_SHELL_WITH_SAME_SURF;
        else if ( sameShell == cm_Copy )
          rebuildMsg = RT_REBUILDING_SHELL_WITH_FULL_COPY;
      }
      if ( !rebuildRes ) 
        rebuildMsg = RT_REBUILDING_NOT_COMPLETELY;

      if ( rebuildRes ) 
        ShowTimeTestResult( testResult, operationCount, facesCount, miliSec, st_Solid, rebuildMsg.c_str() );
    }

#if defined(C3D_DEBUG)
    SET_TIME_TEST(false);
#endif // C3D_DEBUG

    TestVariables::viewManager->RefreshObjectModel( solidItem );

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Перестроить все тела и оболочки. 
// Rebuild all solids and shells.
// ---
bool RebuildAllSolids( bool selectCopyMode )
{
  RPArray<MbItem> objs( 0, 1 );
  SArray<MbMatrix3D> matrs( 0, 1 );

  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    
  if ( wind != nullptr )
    wind->GetObjects( st_Solid, objs, matrs );

  size_t i, objsCnt = objs.size();

  if ( objsCnt > 0 && wind != nullptr ) {

    MbeCopyMode sameShell = cm_KeepHistory;

    if ( selectCopyMode ) {
      sameShell = cm_Copy;

      int cm = 0;
      GetInt( IDS_ENTER_MEMORY, cm );
      switch ( cm ) {
        case 0  : sameShell = cm_Same;        break;
        case 1  : sameShell = cm_KeepHistory; break;
        case 2  : sameShell = cm_KeepSurface; break;
        default : sameShell = cm_Copy;        break;
      }
    }

    DPtr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyCommon ) );
    bool result = true;

    TestVariables::viewManager->DeleteTemporal();
    wind->RefreshOGL();
    size_t facesCount = 0;

    uint32 miliSec = ::GetTickCountEx();
    SetWaitCursor( true );

    for ( i = 0; i < objsCnt; ++i ) {
      MbItem * solidItem = objs[i];
      bool rebuildRes = solidItem->RebuildItem( sameShell, nullptr, progBar );
      wind->RedrawObject( solidItem );
      if ( solidItem->IsA() == st_Solid )
        facesCount += ((MbSolid *)solidItem)->GetFacesCount();
      if ( !rebuildRes )
        result = false;
    }

    SetWaitCursor( false );
    miliSec = ::GetTickCountEx() - miliSec;

    if ( result ) {
      TimeTest * testResult = nullptr;
      size_t operationCount = objsCnt;
      string_t rebuildMsg( RT_REBUILDING_BODY );

      if ( selectCopyMode ) {
        if ( sameShell == cm_Same )
          rebuildMsg = RT_REBUILDING_SHELL_WITHOUT_COPYING_FACES;
        else if ( sameShell == cm_KeepHistory )
          rebuildMsg = RT_REBUILDING_SHELL_WITH_CONST_FACES;
        else if ( sameShell == cm_KeepSurface )
          rebuildMsg = RT_REBUILDING_SHELL_WITH_SAME_SURF;
        else if ( sameShell == cm_Copy )
          rebuildMsg = RT_REBUILDING_SHELL_WITH_FULL_COPY;
      }
      if ( !result ) 
        rebuildMsg = RT_REBUILDING_NOT_COMPLETELY;

      if ( result ) 
        ShowTimeTestResult( testResult, operationCount, facesCount, miliSec, st_Solid, rebuildMsg.c_str() );
    }
    else
      MessageBoxEx( RT_REBUILDING_NOT_COMPLETELY, IDS_WERE_ERRORS );

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Развалить тело на составные части.
// Break solid to parts.
// ---
bool BreakSolidToBasis()
{
  MbItem * nearest = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &nearest ) ) {
    TestVariables::viewManager->ToggleObject(nearest);
    ptrdiff_t count = 0;
    if ( GetInt( IDS_ENTER_BUILD_NUMBER, count ) ) {
      RPArray<MbSpaceItem> s( 2, 2 );
      SetWaitCursor( true );

      ((MbSolid *)nearest)->BreakToBasisItem( count, s );

      ptrdiff_t n = s.Count();
      TestVariables::viewManager->ToggleObject(nearest);

      if ( n > 0 ) {
        Style ns( nearest->GetWidth(), nearest->GetColor() );
        TestVariables::viewManager->DeleteObject( nearest ); // Удаление элемента. // Delete element.  
        for ( ptrdiff_t i = 0; i < n; i++ ) {
          TestVariables::viewManager->AddObject( (i==0) ? ns : Style(1,(uint32)TestVariables::colors[i%16]), s[i] );
        }
      }
      SetWaitCursor( false );

      return true;
    }
  }
  return false;
}


//------------------------------------------------------------------------------
// Удалить историю построения
// Delete item (solid, wireframe, pointframe) creators.
// ---
bool DeleteItemCreators( bool exceptLast )
{
  bool res = false;

  if ( Math::SupressCreators() ) {
    MessageBoxEx( IDS_BUILD_LOG_IS_INACTIVE, IDS_TITLE_ERR );
    return res;
  }

  MbItem * selItem = nullptr;

  if ( GetGObj( IDS_SELECT_OBJECT, st_Item, &selItem ) ) {
    TestVariables::viewManager->EraseObject( selItem );
    SetWaitCursor( true );

    if ( exceptLast ) {
      size_t creatorsCnt = selItem->GetCreatorsCount();

      if ( creatorsCnt > 1 ) {
        ptrdiff_t keepCnt = 1;
        GetInt( IDS_ENTER_NUMBER, keepCnt );
        keepCnt = std_max( keepCnt, (ptrdiff_t)1 );
        keepCnt = std_min( keepCnt, (ptrdiff_t)(creatorsCnt-1) );

        selItem->SetActiveCreatorsCount( creatorsCnt-keepCnt );

        MbeCopyMode copyMode = cm_KeepSurface;
        if ( selItem->RebuildItem( copyMode, nullptr, nullptr ) ) {
          bool isSolid = (selItem->IsA() == st_Solid);

          SPtr<MbSNameMaker> snMaker( &selItem->GetCreator( creatorsCnt-keepCnt-1 )->GetYourNameMaker().Duplicate() ); // именователь предпоследнего строителя

          CreatorsSPtrVector lastCreators;
          lastCreators.reserve( keepCnt );
          for ( size_t k = creatorsCnt-keepCnt; k < creatorsCnt; ++k ) {
            lastCreators.push_back( SPtr<MbCreator>( selItem->SetCreator( k ) ) );
          }
          selItem->DeleteCreators();

          if ( isSolid ) {
            MbSolid * solid = static_cast<MbSolid *>( selItem );
            if ( solid->GetShell() != nullptr ) {
              CreatorSPtr simpleCreator( new MbSimpleCreator( *solid->GetShell(), *snMaker, true ) );
              solid->AddCreator( simpleCreator );
              for ( ptrdiff_t k = 0; k < keepCnt; ++k )
                solid->AddCreator( lastCreators[k] );
              solid->SetActiveCreatorsCount( solid->GetCreatorsCount() );
              // результат перестроения без keepCnt последних
              // нужно или перестроить еще раз, или включить код ниже
              // solidItem->RebuildItem( copyMode, nullptr, nullptr );
              res = true;
            }
          }
          else {
            for ( ptrdiff_t k = 0; k < keepCnt; ++k )
              selItem->AddCreator( lastCreators[k] );
            res = true;
          }
        }
        TestVariables::viewManager->RefreshObjectModel( selItem );
      }
    }
    else {
      selItem->DeleteCreators();
      res = true;
    }

    if ( !res ) {
      MessageBoxEx( IDS_FAILED, IDS_TITLE_ERR );
    }

    SetWaitCursor( false );
  }

  return res;
}


//------------------------------------------------------------------------------
// Развалить тело на поверхности граней и ликвидировать его.
// Break solid to surfaces and delete it.
// ---
bool TranslateSolidToSurfaces()
{
  MbItem * solidItem = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &solidItem ) ) {
    std::vector<uint32> colors( 0, 1 ); // KOMPAS-25758
    int width = solidItem->GetWidth();
    uint32 color = solidItem->GetColor();

    TestVariables::viewManager->ToggleObject( solidItem );

    SetWaitCursor( true );

    std::vector< SPtr<MbSurface> > surfaces;
    MbFaceShell * outer = ((MbSolid *)solidItem)->GetShell();
    if ( outer != nullptr ) {
      size_t facesCnt = outer->GetFacesCount();
      surfaces.reserve( facesCnt );
      colors.reserve( facesCnt );
      for ( size_t i = 0; i < facesCnt; ++i ) {
        const MbFace * face = outer->_GetFace(i);
        const MbSurface & faceSurface = face->GetSurface();
        SPtr<MbSurface> surface( static_cast<MbSurface *>( &faceSurface.Duplicate() ) );
        const MbAttribute * attr = face->GetSimpleAttribute( at_Visual ); // выдать простой атрибут данного типа 
        if ( attr != nullptr )
          colors.push_back( static_cast<const MbVisual *>( attr )->GetColor() );
        else
          colors.push_back( color );
        face->GetColor();

        if ( faceSurface.IsA() != st_CurveBoundedSurface ) {
          bool faceSense = face->IsSameSense();

          size_t loopsCnt = face->GetLoopsCount();
          std::vector< SPtr<MbContour> > contours;
          contours.reserve( loopsCnt );
          for ( size_t j = 0; j < loopsCnt; ++j ) {
            if ( const MbLoop * loop = face->_GetLoop( j ) ) {
              SPtr<MbContour> contour( &loop->MakeContour( faceSurface.GetSurface(), faceSense, true ) );
              contours.push_back( contour );
            }
          }
          if ( !contours.empty() ) {
            surface = new MbCurveBoundedSurface( *surface, contours, true );
          }
        }

        surfaces.push_back( surface );
      }
    }
    if ( surfaces.size() == 1 && surfaces[0] != nullptr ) {
      SPtr<MbSpaceInstance> spaceInst( new MbSpaceInstance( *surfaces[0] ) );

      Style st( width, color );
      spaceInst->SetColor( st.GetColor() );
      spaceInst->SetWidth( st.GetWidth() );
      spaceInst->SetStyle( st.GetStyle() );

      TestVariables::viewManager->ReplaceItem( *solidItem, *spaceInst, true ); // В сборке вставится туда, где было тело
    }
    else {
      for ( size_t i = 0; i < surfaces.size(); i++ ) {
        if ( surfaces[i] != nullptr ) {
          Style st( width, (i < colors.size()) ? colors[i] : color ); // KOMPAS-25758
          TestVariables::viewManager->AddObject( st, surfaces[i], false );
        }
      }
      TestVariables::viewManager->DeleteObject( solidItem );
    }

    SetWaitCursor( false );

    TestVariables::viewManager->ShowModel();

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Развалить тело на грани и ликвидировать его.
// Break solid to faces and delete it.
// ---
bool TranslateSolidToFaces()
{
  MbItem * solidItem = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &solidItem ) ) {
    TestVariables::viewManager->ToggleObject( solidItem );

    SetWaitCursor( true );

    std::vector< SPtr<MbSurface> > surfaces;
    MbFaceShell * outer = ((MbSolid *)solidItem)->GetShell();
    if ( outer != nullptr ) {
      size_t facesCnt = outer->GetFacesCount();
      surfaces.reserve( facesCnt );
      for ( size_t i = 0; i < facesCnt; i++ ) {
        SPtr<MbSurface> surface( static_cast<MbSurface *>( &outer->_GetFace(i)->GetSurface().Duplicate() ) );
        surfaces.push_back( surface );
      }
    }
    MbSNameMaker snMaker( SimpleName(-1), MbSNameMaker::i_SideNone, 0 );
    for ( size_t i = 0; i < surfaces.size(); i++ ) {
      if ( surfaces[i] != nullptr ) {
        MbSurfaceShellParams params( *surfaces[i], snMaker, true );
        SolidSPtr surfaceShell;
        ::SurfaceShell( params, surfaceShell );
        if ( surfaceShell != nullptr )
          TestVariables::viewManager->AddObject( TestVariables::SURFACE_Style, surfaceShell, false );
      }
    }
    TestVariables::viewManager->DeleteObject( solidItem );

    SetWaitCursor( false );

    TestVariables::viewManager->ShowModel();

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Разделение тела на части.
// Break solid to parts.
// ---
bool BreakSolidOnParts()
{
  MbItem * obj( nullptr );

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {
    TestVariables::viewManager->ToggleObject( obj );

    MbSolid * solid = static_cast<MbSolid *>(obj);
    RPArray<MbSolid> parts( 0, 1 );

    MbAccurateTimer ac;

    static bool recheckMultiSolidState = true;
    if ( recheckMultiSolidState )
      solid->ResetMultiSolidState();

    ac.Begin();
    // Проверка на разделение частей тела
    bool isMultiSolid = solid->IsMultiSolid();
    ac.End();

    if ( isMultiSolid )
      MessageBoxEx( RT_BODY_IS_MADE_OF_PARTS, IDS_DISGNOSTICS );

    double durTime1 = ac.GetLast();

    MbSNameMaker operNames( ct_DetachSolid, MbSNameMaker::i_SideNone, 0 );

    ac.Begin();
    // Попытка разделения частей тела
    ::DetachParts( *solid, parts, false, operNames );
    ac.End();
    double durTime2 = ac.GetLast();

    TestVariables::viewManager->ToggleObject(obj);

    if ( parts.size() > 0 ) { // Перерисовать модель. // Redraw a model.
      if ( !isMultiSolid ) {
        MessageBoxEx( RT_ONE_PART, IDS_ERROR );
        MessageBoxEx( RT_BUT_CREATE_BODY_OF_PARTS, IDS_ERROR );
      }

      SetWaitCursor( true );
      TestVariables::viewManager->RefreshObjectModel( obj );
      if ( parts.size() > 1 ) {
        for ( size_t i = 0, icnt = parts.size() - 1; i < icnt; ++i )
          TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, parts[i], false );
      }
      TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, parts.back(), true );
      SetWaitCursor( false );
    }

    if ( TestVariables::checkOperationTime ) {
      const size_t bufLen = 1024;
      TCHAR buf[bufLen];
      size_t len = 0;
      len += (size_t)_sntprintf( buf + len, bufLen - len, RT_MULTISOLID_CHECK );
      len += (size_t)_sntprintf( buf + len, bufLen - len, _T( "\n\n" ) );
      len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, durTime1 );
      len += (size_t)_sntprintf( buf + len, bufLen - len, _T( "\n\n" ) );
      len += (size_t)_sntprintf( buf + len, bufLen - len, RT_MULTISOLID_SPLIT );
      len += (size_t)_sntprintf( buf + len, bufLen - len, _T( "\n\n" ) );
      len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, durTime2 );
      ::MessageBoxEx( buf, IDS_RESULTS );
    }

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Удалить внутренние объемы тела 
// Remove of internal body volumes
// ---
bool RemoveInternalVolumes()
{
  MbItem* obj( nullptr );

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {
    TestVariables::viewManager->ToggleObject( obj );

    MbSolid* solid = static_cast< MbSolid* >( obj );

    if ( solid ) 
    {
      if ( solid->IsMultiSolid() )
      {
        MessageBoxEx( RT_BODY_IS_MADE_OF_PARTS, IDS_MESSAGE );
        return false;
      }

      ShellSPtr shell( new MbFaceShell( *solid->GetShell() ) );
      c3d::ShellsSPtrVector parts;
      size_t count = ::DetachShells( *shell, parts, true );

      if ( count > 0 )
      {
        solid->SetShell( shell.detach() );
        SetWaitCursor( true );
        TestVariables::viewManager->RefreshObjectModel( obj );
        SetWaitCursor( false );
        return true;
      }
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Создать тело из частей тела.
// Create solid from parts.
// ---
bool CreatePartSolid()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {
    TestVariables::viewManager->ToggleObject( obj );

    MbSolid * solid = (MbSolid *)obj;

    PArray<MbSolid> parts(2,2,false);
    MbSNameMaker names( ct_DetachSolid, MbSNameMaker::i_SideNone, 0 );
    // Отделение частей тела. // Separate solid parts.
    if ( solid->IsMultiSolid() ) {
      MessageBoxEx( RT_BODY_IS_MADE_OF_PARTS, IDS_OUR_CASE );
      ::CreateParts( *solid, parts, names );
    }

    if ( parts.Count() > 1 ) {
      bool bRes = false;

      solid->AddRef();

      {
        SetWaitCursor( true );

        TestVariables::viewManager->DeleteObject( obj );
        for ( size_t i = 0; i < parts.Count(); i++ )
          TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, parts[i] );

        SetWaitCursor( false );
      }

      MbCartPoint3D refPnt, tiePnt;
      ptrdiff_t allCount = parts.Count();

      MbCube allCube;
      solid->AddYourGabaritTo( allCube );
      allCube.GetCentre( refPnt );

      MbPartSolidIndices partIndices;
      MbPath path;
      uint id = 0;

      while ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {
        TestVariables::viewManager->ToggleObject( obj );
        MbSolid * part = (MbSolid *)obj;
        size_t ind = parts.FindIt( part );

        if ( ind != SYS_MAX_T ) {
          bool bAdd = true;
          for ( size_t k = 0, kcnt = partIndices.Count(); k < kcnt && bAdd; k++ ) {
            MbPartSolidIndex partIndex;
            partIndices.GetPartIndex( k, partIndex );
            if ( partIndex.GetIndex() == (ptrdiff_t)ind )
              bAdd = false;
          }
          if ( bAdd ) {
            MbCube partCube;
            part->AddYourGabaritTo( partCube );
            partCube.GetCentre( tiePnt );
            double diag = partCube.GetDiagonal();
            MbPartSolidIndex psInd( id, path, ind, tiePnt, diag, refPnt, allCount );
            partIndices.AddPartIndex( psInd );
            partIndices.SetSelected( partIndices.MaxIndex(), true );
          }
        }
      }
      //partIndices.SetAllSelected( partIndices.Count() == allCount );

      if ( partIndices.Count() > 0 ) {
        SetWaitCursor( true );

        MbShellPartResults shellResult;
        ::ShellPart( *solid,  MbShellPartParams(id, path, names, partIndices), shellResult );

        for ( ptrdiff_t i = parts.MaxIndex(); i >= 0; i-- ) {
          obj = (MbItem *)TestVariables::viewManager->FindObject( parts[i] );
          if ( obj != nullptr )
            TestVariables::viewManager->DeleteObject( obj );
        }

        if ( shellResult._solid != nullptr )
          TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, shellResult._solid );
        else
        {
          MessageBoxEx( RT_NOT_CREATE_BODY_OF_PARTS, IDS_TITLE_ERR );
        }

        SetWaitCursor( false );
        bRes = true;
      }
      else {
        MessageBoxEx( RT_DO_NOT_DO, IDS_TITLE_WRN );
        TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, solid );
      }

      solid->Release();

      return bRes;
    }
    else {
      MessageBoxEx( RT_ONE_PART, IDS_TITLE_WRN );
    }
  }
  else {
    MessageBoxEx( RT_DO_NOT_DO, IDS_TITLE_WRN );
  }

  return false;
}


//------------------------------------------------------------------------------
// Проверить и устранить наличие общих подложек.
// Check and correct identical base surfaces.
// ---
bool CheckIdenticalBaseSufaces()
{
  bool bRes = false;
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {
    TestVariables::viewManager->ToggleObject( obj );
    SetWaitCursor( true );

    MbSolid * solid = (MbSolid *)obj;
    MbFaceShell * shell = solid->GetShell();

    if ( shell != nullptr ) {
      if ( ::RemoveCommonSurfaceSubstrates( *shell ) )
      {
        MessageBoxEx( FACES_WERE_CORRECTED, IDS_RESULT );
      }
    }

    TestVariables::viewManager->ToggleObject( obj );
    TestVariables::viewManager->RefreshObjectModel( obj );
    SetWaitCursor( false );
  }

  return bRes;
}


//------------------------------------------------------------------------------
// Объединить компланарные грани тела.
// Unify complanar solid faces.
// ---
bool UnifyOwnComplanarFaces()
{
  bool bRes = false;

  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {
    TestVariables::viewManager->ToggleObject( obj );
    SetWaitCursor( true );

    MbSolid * solid = (MbSolid *)obj;
    MbFaceShell * shell = solid->GetShell();

    if ( shell != nullptr ) {
      MbSNameMaker snMaker( ct_UnionSolid, MbSNameMaker::i_SideNone, 0 );
      snMaker.SetVersion( MbNameVersion() );

      size_t cntFaces = shell->GetFacesCount();

      if ( ::UnifyOwnComplanarFaces( *shell, snMaker, true ) )
        bRes = true;

      if ( shell->GetFacesCount() < cntFaces )
        MessageBoxEx( RT_SOME_FACES_MERGED, IDS_RESULT );
    }
    // Необходимо запросить пересчет отрисовочной модели.
    TestVariables::viewManager->RefreshObjectModel( obj );
    TestVariables::viewManager->ToggleObject( obj );
    SetWaitCursor( false );
  }

  return bRes;
}


//------------------------------------------------------------------------------
// Отрисовка ребер.
// Drawing edges.
// ---
template <class EdgesVector>
void DrawEdges( const EdgesVector & edges, int r, int g, int b )
{
  MbCartPoint3D p1, p2;
  for ( size_t i = 0, cnt = edges.size(); i < cnt; ++i ) {
    if ( edges[i] != nullptr ) {
      TestVariables::drawImp->DrawItem( edges[i], TRGB_GREEN );
      edges[i]->Point( 0.0, p1 );
      edges[i]->Point( 1.0, p2 );
      TestVariables::drawImp->DrawPoint( &p1, r, g, b );
      TestVariables::drawImp->DrawPoint( &p2, r, g, b );
      TestVariables::drawImp->DrawItem( edges[i], r, g, b );
    }
  }
}


//------------------------------------------------------------------------------
// Отрисовка вершин с окружением.
// Drawing vertices with the environment.
// ---
template <class VerticesVector, class EdgesVector>
void DrawVertices( const VerticesVector & verts, const EdgesVector & edges, int r, int g, int b )
{
  for ( size_t i = 0, icnt = verts.size(); i < icnt; ++i ) {
    const MbVertex * v = verts[i];
    if ( v != nullptr ) {
      for ( size_t k = 0, kcnt = edges.size(); k < kcnt; ++k ) {
        const MbCurveEdge * e = edges[k];
        if ( e != nullptr && (v == &e->GetBegVertex() || v == &e->GetEndVertex()) )
          TestVariables::drawImp->DrawItem( e, r, g, b );
      }
      TestVariables::drawImp->DrawPoint( &v->GetCartPoint(), r, g, b );
    }
  }
}


//------------------------------------------------------------------------------
// Проверка подложек и указаний на грани. 
// Check base surfaces and face pointers. 
// ---
static
bool CheckBaseSurfacesAndBadFacePointers( const MbFaceShell & shell, bool & areIdenticalBaseSurfaces, bool & areBadFacePointers )
{ 
  bool isValid = true;

  size_t facesCnt = shell.GetFacesCount();
  bool areIdenticalBaseSurfacesLoc = false;
  bool areBadFacePointersLoc = false;
  ::CheckBadFaces( shell, areIdenticalBaseSurfacesLoc, areBadFacePointersLoc );

  if ( areIdenticalBaseSurfacesLoc ) {
    areIdenticalBaseSurfaces = areIdenticalBaseSurfacesLoc;

    // Set the next variable to "true" if you want to see faces with common basis surface
    static bool showSameBaseSurface = false;

    if ( showSameBaseSurface ) {
      for ( size_t k1 = 0; k1 < facesCnt; ++k1 ) {
        const MbFace * face1 = shell._GetFace( k1 );
        const MbSurface * s1 = &face1->GetSurface().GetSurface();
        for ( size_t k2 = k1 + 1; k2 < facesCnt; ++k2 ) {
          const MbFace * face2 = shell._GetFace( k2 );
          const MbSurface * s2 = &face2->GetSurface().GetSurface();
          if ( s1 == s2 ) {
            TestVariables::drawImp->DrawItem( face1, TRGB_YELLOW );
            TestVariables::drawImp->DrawItem( face2, TRGB_YELLOW );

            TestVariables::drawImp->DrawItem( face1, TRGB_PURPLE );
            TestVariables::drawImp->DrawItem( face2, TRGB_AZURE );

            TestVariables::drawImp->DrawItem( face1, TRGB_LIGHTGREEN );
            TestVariables::drawImp->DrawItem( face2, TRGB_LIGHTGREEN );
          }
        }
      }
    }

    isValid = false;
  }

  if ( areBadFacePointersLoc ) {
    areBadFacePointers = areBadFacePointersLoc;
    isValid = false;
  }

  return isValid;
}


//------------------------------------------------------------------------------
// Проверить геометрию набора оболочек
// Check the geometry of the set of shells
// ---
static
bool CheckShellsGeometry( ConstShellsVector & shells )
{
  bool res = true;
  size_t allFacesCnt = 0;
  const size_t shellsCount = shells.size();

  for ( size_t sInd = 0; sInd < shellsCount && res; ++sInd ) {
    res = (shells[sInd] != nullptr);
    if ( res ) {
      allFacesCnt += shells[sInd]->GetFacesCount();
    }
    else {
      C3D_ASSERT_UNCONDITIONAL( false );
    }
  }

  if ( res && allFacesCnt <= c3d::ARRAY_MAX_COUNT ) {
    bool isValid = true;

    ConstEdgesVector allEdges;
    for ( size_t sInd = 0; sInd < shellsCount; ++sInd ) {
      shells[sInd]->GetEdges( allEdges );
    }

    static double mMaxAcc = DELTA_MIN; // Максимальный физический допуск огрехов. // Maximum physical tolerance of flaws.

    TCHAR msgTitleWarning[] = IDS_TITLE_DIAG_WRN;
    TCHAR msgTitleError[] = IDS_TITLE_DIAG_ERR;
    TCHAR msgTitleSuccess[] = IDS_TITLE_DIAG_ALL_RIGHT;

    if ( shellsCount == 1 ) {
      bool isClosed = shells[0]->IsClosed();
      if ( !isClosed )
        ::MessageBoxEx( RT_CHECK_OPEN_SHELL, msgTitleWarning );
    }

    { // Проверка подложек и указаний на грани. // Check base surfaces and face pointers. 
      bool areIdenticalBaseSurfaces = false;
      bool areBadFacePointers = false;
      for ( size_t sInd = 0; sInd < shellsCount; ++sInd ) {
        ::CheckBaseSurfacesAndBadFacePointers( *shells[sInd], areIdenticalBaseSurfaces, areBadFacePointers );
      }
      if ( areIdenticalBaseSurfaces ) {
        ::MessageBoxEx( RT_FACES_HAVE_COMMON_SUBSTRATE, msgTitleError );
        isValid = false;
      }
      if ( areBadFacePointers ) {
        ::MessageBoxEx( RT_INVALID_REFERENCE_TO_FACE, msgTitleError );
        isValid = false;
      }
    }

    { // Поиск некорректных ребер. // Search incorrect edges.
      ConstEdgesVector badEdges;
      bool areBadEdges = ::CheckBadEdges( allEdges, &badEdges );

      if ( areBadEdges ) {
        ::DrawEdges( badEdges, TRGB_BLACK );
        ::MessageBoxEx( RT_INCORRECT_EDGES, msgTitleError );
        ::DrawEdges( badEdges, TRGB_BLACK );
        isValid = false;
      }
    }

    { // Поиск краевых ребер. // Search boundary edges.
      ConstEdgesVector closedShellEdges, openShellEdges;

      for ( size_t sInd = 0; sInd < shellsCount; ++sInd ) {
        if ( shells[sInd]->IsClosed() )
          shells[sInd]->GetEdges( closedShellEdges );
        else
          shells[sInd]->GetEdges( openShellEdges );
      }

      ConstEdgesVector closedShellBoundaryEdges, openShellBoundaryEdges;
      bool areClosedShellsBoundaryEdges = ::CheckBoundaryEdges( closedShellEdges, &closedShellBoundaryEdges );
      bool areEnclosedShellsBoundaryEdges = ::CheckBoundaryEdges( openShellEdges, &openShellBoundaryEdges );

      if ( areClosedShellsBoundaryEdges ) {
        ::DrawEdges( closedShellBoundaryEdges, TRGB_BLACK );

        const size_t bufSize = 1024;
        TCHAR * buf = new TCHAR[bufSize];
        size_t len = 0;
        len += ( size_t )_sntprintf( buf + len, bufSize - len, RT_BOUNDARY_EDGES_COUNT, ::LoUint32( closedShellBoundaryEdges.size() ) );
        //::MessageBoxEx( RT_BOUNDARY_EDGES, msgTitleError );
        ::MessageBoxEx( buf, msgTitleError );
        ::DrawEdges( closedShellBoundaryEdges, TRGB_BLACK );
        isValid = false;
        delete[] buf;
      }
      if ( areEnclosedShellsBoundaryEdges ) {
        ConstEdgesVector boundaryEdges1, boundaryEdges2;

        for ( size_t m = 0, mcnt = openShellBoundaryEdges.size(); m < mcnt; ++m ) {
          const MbCurveEdge * edge = openShellBoundaryEdges[m];
          if ( edge->GetBuildType() == cbt_Boundary ) boundaryEdges1.push_back( edge );
          else                                        boundaryEdges2.push_back( edge );
        }

        ::DrawEdges( boundaryEdges1, TRGB_DARKGRAY );
        ::DrawEdges( boundaryEdges2, TRGB_BLACK );
        ::MessageBoxEx( RT_BOUNDARY_EDGES_OPEN_SHELL, msgTitleWarning );
        ::DrawEdges( boundaryEdges1, TRGB_DARKGRAY );
        ::DrawEdges( boundaryEdges2, TRGB_BLACK );
      }
    }

    { // Поиск неточных вершин. // Search inexact vertices.
      ConstVerticesVector allVerts;
      allVerts.reserve( shellsCount * 10 );

      for ( size_t sInd = 0; sInd < shellsCount; ++sInd )
        shells[sInd]->GetVertices( allVerts );

      ConstVerticesVector inexactVerts;
      inexactVerts.reserve( allVerts.size() / 2 );

      bool areInexactVertices = ::CheckInexactVertices( allVerts, mMaxAcc, &inexactVerts );

      if ( areInexactVertices ) {
        const size_t bufSize = 1024;
        TCHAR * buf = new TCHAR[bufSize];
        size_t len = 0;
        len += ( size_t )_sntprintf( buf + len, bufSize - len, RT_VERTICES_WITH_TOLERANCE, mMaxAcc, ::LoUint32( inexactVerts.size() ) );
        ::DrawVertices( inexactVerts, allEdges, TRGB_TANGERINE );
        ::MessageBoxEx( buf, msgTitleError );
        ::DrawVertices( inexactVerts, allEdges, TRGB_TANGERINE );
        isValid = false;
        delete[] buf;
      }
    }

    std::vector<SpacePointVector> facePntNormData;
    { // Проверить самопересечения циклов и взаимное расположение циклов.
      // Check loops self-intersections and loops interposition.
      bool areLoopsSelfIntersections = false;
      bool areLoopsInterpositionErrors = false;
      bool areWrongSurfaceCubes = false;
      bool areWrongLoopRects = false;

      for ( size_t sInd = 0; sInd < shellsCount; ++sInd ) {
        size_t facesCnt = shells[sInd]->GetFacesCount();
        {
          facePntNormData.reserve( facesCnt );
          const MbCartPoint3D emptyPnt( UNDEFINED_DBL, 0, 0 );
          const MbVector3D emptyNorm( UNDEFINED_DBL, 0, 0 );
          for ( size_t k = 0; k < facesCnt; ++k ) {
            facePntNormData.push_back( std::make_pair( emptyPnt, emptyNorm ) );
          }
        }

        MbSNameMaker snMaker( SimpleName(-1), MbSNameMaker::i_SideNone, 0 );
        std::vector< std::pair<IndicesPair, MbCartPoint3D> > loopPnts;
        bool findInsideEdges = true;

        for ( size_t k = 0; k < facesCnt; ++k ) {
          const MbFace * face = shells[sInd]->_GetFace( k );
          if ( face != nullptr ) {
            loopPnts.clear();
            if ( ::FindLoopsSelfIntersections( *face, snMaker, findInsideEdges, &loopPnts ) ) {
              areLoopsSelfIntersections = true;
              for ( size_t l = 0, lcnt = loopPnts.size(); l < lcnt; ++l ) {
                std::pair<IndicesPair, MbCartPoint3D> & loopPnt = loopPnts[l];
                TestVariables::drawImp->DrawItem( face->GetLoop( loopPnt.first.first ), TRGB_MAGENTA );
                TestVariables::drawImp->DrawItem( face->GetLoop( loopPnt.first.second ), TRGB_AZURE );
                TestVariables::drawImp->DrawPoint( &loopPnt.second, TRGB_BLUE );
              }
            }
            else if ( !::CheckLoopsInterposition( *face ) ) {
              TestVariables::drawImp->DrawItem( face, TRGB_MAGENTA );
              areLoopsInterpositionErrors = true;
            }
            else {
              MbCartPoint3D pnt;
              MbVector3D norm;
              if ( face->GetAnyPointOn( pnt, norm ) ) {
                facePntNormData[k].first = pnt;
                facePntNormData[k].second = norm;
                MbCube cube;
                face->GetSurface().AddYourGabaritTo( cube );
                if ( !cube.Contains( pnt, mMaxAcc ) ) {
                  TestVariables::drawImp->DrawItem( face, TRGB_DARKGRAY );
                  areWrongSurfaceCubes = true;
                }
              }

              { // Check cached 2D-bbox of loop
                for ( size_t loopInd = face->GetLoopsCount(); loopInd--; ) {
                  const MbLoop * loop = face->_GetLoop( loopInd );
                  if ( loop != nullptr ) {
                    MbRect loopRect( loop->GetGabarit( face->GetSurface(), face->IsSameSense() ) );
                    MbRect edgesRect;
                    for ( size_t i = loop->GetEdgesCount(); i--; ) {
                      const MbOrientedEdge * orientEdge = loop->_GetOrientedEdge( i );
                      if ( orientEdge != nullptr ) {
                        const MbCurveEdge & edge = orientEdge->GetCurveEdge();
                        const MbCurve * edgeCurve = edge.ChooseCurve( face, orientEdge->GetOrientation() );
                        if ( edgeCurve != nullptr )
                          edgeCurve->AddYourGabaritTo( edgesRect );
                      }
                    }
                    if ( !c3d::EqualCubes( loopRect, edgesRect, PARAM_NEAR ) ) {
                      areWrongLoopRects = true; // use loop->SetEmptyGabarit(); to reset cached bbox
                      break;
                    }
                  }
                }
              }
            }
          }
        }
      }

      if ( areLoopsSelfIntersections ) {
        ::MessageBoxEx( RT_SELFINTERSECTING_LOOPS, msgTitleError );
        isValid = false;
      }
      else if ( areLoopsInterpositionErrors ) {
        ::MessageBoxEx( RT_WRONG_LOOPS_INTERPOSITION, msgTitleError );
        isValid = false;
      }
      else if ( areWrongSurfaceCubes ) {
        ::MessageBoxEx( RT_CACHED_DATA_ERROR, msgTitleError );
        isValid = false;
      }
      else if ( areWrongLoopRects ) {
        ::MessageBoxEx( RT_CACHED_DATA_ERROR, msgTitleError );
        isValid = false;
      }
    }

    if ( isValid ) { // Проверить связность циклов. // Check loops connection.
      bool areBadLoopConnections = false;
      bool areBadEdgeVertices = false;

      ConstFacesVector       badConnectedFaces;
      ConstOrientEdgesVector badConnectedEdges;
      ConstEdgesVector       badVertexEdges;

      for ( size_t sInd = 0; sInd < shellsCount; ++sInd ) {
        size_t facesCnt = shells[sInd]->GetFacesCount();
        for ( size_t fInd = 0; fInd < facesCnt; ++fInd ) {
          const MbFace * face = shells[sInd]->_GetFace( fInd );
          if ( face != nullptr && face->GetLoopsCount() > 0 ) {
            bool isFaceWithBadLoop = false;
            for ( size_t iLoop = 0, loopsCnt = face->GetLoopsCount(); iLoop < loopsCnt; ++iLoop ) {
              const MbLoop * loop = face->_GetLoop( iLoop );
              if ( loop != nullptr ) {
                double lenTol = LENGTH_EPSILON;
                double paramTol = PARAM_EPSILON;
                // Есть ли дефекты в связности ребер цикла. // Are there defects in loop connection.
                if ( !::CheckLoopConnection( *face, *loop, lenTol, &paramTol, badConnectedEdges, badVertexEdges ) ) {
                  if ( !areBadLoopConnections && badConnectedEdges.size() > 0 ) {
                    ::MessageBoxEx( RT_BREAK_CONNECT_LOOPS, msgTitleError );
                    areBadLoopConnections = true;
                  }
                  if ( !areBadEdgeVertices && badVertexEdges.size() > 0 ) {
                    ::MessageBoxEx( RT_INCORRECT_VERTICES_IN_EDGES, msgTitleError );
                    areBadEdgeVertices = true;
                  }
                  isFaceWithBadLoop = true;
                }
              }
            }
            if ( isFaceWithBadLoop )
              badConnectedFaces.push_back( face );
          }
        }
      }

      {
        if ( badConnectedEdges.size() > 0 ) {
          ::DrawEdges( badConnectedEdges, TRGB_ORANGE );
          ::DrawEdges( badConnectedEdges, TRGB_MAGENTA );
        }
        if ( badVertexEdges.size() > 0 ) {
          ::DrawEdges( badVertexEdges, TRGB_BLACK );
          ::DrawEdges( badVertexEdges, TRGB_YELLOW );
        }
      }

      if ( areBadLoopConnections || areBadEdgeVertices )
        isValid = false;
    }

    { //if ( isValid ) { // Поиск неточных ребер. // Search inexact edges.
      ConstEdgesVector inexactEdges;
      bool areInexactEdges = ::CheckInexactEdges( allEdges, mMaxAcc, &inexactEdges );

      if ( areInexactEdges ) {
        const size_t bufSize = 1024;
        TCHAR * buf = new TCHAR[bufSize];
        size_t len = 0;
        len += ( size_t )_sntprintf( buf + len, bufSize - len, RT_INEXACT_EDGES, ::LoUint32( inexactEdges.size() ) );
        ::DrawEdges( inexactEdges, TRGB_AZURE );
        ::MessageBoxEx( buf, msgTitleError );
        ::DrawEdges( inexactEdges, TRGB_AZURE );
        isValid = false;
        delete[] buf;
      }
    }

    { // поиск возможных наложений граней (возможно нужно опционально из-за увеличения времени проверки)
      bool areOverlappingFaces = false;

      bool areAreasCorrect = true;
      SurfacesSPtrVector wrongAreaSurfaces;

      MbCartPoint3D facePnt, shellPnt;
      MbVector3D faceNorm, shellNorm;
      MbCartPoint uv;

      for ( size_t sInd = 0; sInd < shellsCount; ++sInd ) {
        size_t facesCnt = shells[sInd]->GetFacesCount();
        for ( size_t k = facesCnt; k--; ) {
          const MbFace * face = shells[sInd]->_GetFace( k );
          if ( face != nullptr ) {
            if ( face->GetSurface().IsA() == st_CurveBoundedSurface ) {
              const MbCurveBoundedSurface & bndSurface = static_cast< const MbCurveBoundedSurface & >( face->GetSurface() );
              if ( bndSurface.GetCurvesCount() > 0 ) {
                bool isValidOuter = ( bndSurface.GetCurve( 0 )->Area() >= -EXTENT_EQUAL );
                if ( !isValidOuter ) {
                  wrongAreaSurfaces.push_back( SPtr<MbSurface>( static_cast< MbSurface * >( &bndSurface.Duplicate() ) ) );
                  areAreasCorrect = false;
                }
              }
            }

            bool canCheck = false;
            if ( ( facePntNormData[k].first.x == UNDEFINED_DBL ) || ( facePntNormData[k].second.x == UNDEFINED_DBL ) ) {
              if ( face->GetAnyPointOn( facePnt, faceNorm ) ) {
                facePntNormData[k].first = facePnt;
                facePntNormData[k].second = faceNorm;
                canCheck = true;
              }
            }
            else {
              facePnt = facePntNormData[k].first;
              faceNorm = facePntNormData[k].second;
              canCheck = true;
            }
            if ( canCheck ) {
              size_t ind = SYS_MAX_T;
              shells[sInd]->NearPointProjection( facePnt, ind, uv.x, uv.y );
              if ( ind != k ) {
                const MbFace * shellFace = shells[sInd]->GetFace( ind );
                shellFace->GetSurface()._PointOn( uv.x, uv.y, shellPnt );
                if ( c3d::EqualPoints( facePnt, shellPnt, Math::metricPrecision ) ) {
                  shellFace->GetSurface()._Normal( uv.x, uv.y, shellNorm );
                  if ( faceNorm * shellNorm > 1.0 - PARAM_NEAR ) {
                    TestVariables::drawImp->DrawItem( face, TRGB_ORANGE );
                    TestVariables::drawImp->DrawItem( shellFace, TRGB_OLIVE );
                    areOverlappingFaces = true;
                  }
                }
              }
            }
          }
        }
      }

      if ( areOverlappingFaces ) {
        ::MessageBoxEx( RT_OVERLAPPING_FACES, msgTitleError );
      }
      if ( !areAreasCorrect ) {
        ::MessageBoxEx( RT_WRONG_AREA_SIGN, msgTitleError );
        for ( size_t addInd = wrongAreaSurfaces.size(); addInd--; ) {
          TestVariables::viewManager->AddObject( TestVariables::SURFACE_Style, wrongAreaSurfaces[addInd], false );
        }
      }
    }

    { // Проверка габаритов граней и их базовых поверхностей
      if ( !TestVariables::useBoundingBoxErrorDialog || GetYesNoChoice( RT_BASE_BBOX_ERROR_REQUEST, IDS_TITLE_OBJECT_GABARIT, false ) ) {
        bool isBadFaceLabel = false;
        bool isBadBaseCube = false;
        MbCube faceCube, baseCube;
        for ( size_t sInd = 0; sInd < shellsCount; ++sInd ) {
          size_t facesCnt = shells[sInd]->GetFacesCount();
          for ( size_t k = facesCnt; k--; ) {
            const MbFace * face = shells[sInd]->_GetFace( k );
            if ( face != nullptr ) {
              faceCube.SetEmpty();
              baseCube.SetEmpty();
              face->AddYourGabaritTo( faceCube );
              face->GetSurface().GetSurface().AddYourGabaritTo( baseCube );
              double faceDiag = faceCube.GetDiagonal();
              double baseDiag = baseCube.GetDiagonal();
              if ( baseDiag > 2.0 * faceDiag + METRIC_ACCURACY ) {
                TestVariables::drawImp->DrawItem( &face->GetSurface().GetSurface(), TRGB_DARKGRAY );
                TestVariables::drawImp->DrawItem( face, TRGB_PURPLE );
                isBadBaseCube = true;
              }
              if ( face->ToDelete() )
                isBadFaceLabel = true;
            }
          }
        }
        if ( isBadBaseCube ) {
          ::MessageBoxEx( RT_BASE_BBOX_ERROR, msgTitleError );
          isValid = false;
        }
        if ( isBadFaceLabel ) {
          ::MessageBoxEx( RT_ITEM_LABEL_ERROR, msgTitleError );
          isValid = false;
        }
      }
    }

    bool haveParts = false;
    { // Проверить многочастность тела с проверкой краевых ребер при распаде на части.
      // Check out the multi-part structure of the solid with the verification of boundary edges in the partitioning.

      bool shellCanBreakAPart = false;
      bool shellBrokeIntoParts = false;
      bool notBrokenIntoParts = false;
      bool boundaryEdgesInMostPart = false;

      for ( size_t sInd = 0; sInd < shellsCount; ++sInd ) {
        c3d::ShellSPtr shellCopy( static_cast< MbFaceShell * >( shells[sInd]->Duplicate() ) ); // Создадим копию. // Create a copy.
        if ( shellCopy.get() != nullptr ) {
          c3d::SolidSPtr solidCopy( new MbSolid( &( *shellCopy ), nullptr ) );
          RPArray<MbSolid> parts( 0, 1 );
          if ( solidCopy->IsMultiSolid() ) {
            shellCanBreakAPart = true;
            haveParts = true;
          }
          MbSNameMaker operNames( ct_DetachSolid, MbSNameMaker::i_SideNone, 0 );
          if ( ::DetachParts( *solidCopy, parts, true, operNames ) > 0 ) {
            shellBrokeIntoParts = true;
            ::DeleteItems( parts );
            haveParts = true;
          }
          else if ( haveParts ) {
            notBrokenIntoParts = true;
            haveParts = false;
          }

          if ( haveParts && solidCopy->GetShell() != nullptr ) {
            // Проверим большую часть на наличие краевых ребер. // Check the most part for boundary edges.
            allEdges.clear();
            solidCopy->GetShell()->GetEdges( allEdges );
            if ( solidCopy->GetShell()->IsClosed() ) {
              if ( ::CheckBoundaryEdges( allEdges, (ConstEdgesVector *)nullptr ) ) // Есть краевые ребра. // There are boundary edges.
                boundaryEdgesInMostPart = true;
            }
          }
        }
      }

      if ( shells.size() == 1 ) {
        if ( shellCanBreakAPart )
          ::MessageBoxEx( RT_SHELL_CAN_BREAK_APART, msgTitleWarning );
        if ( shellBrokeIntoParts )
          ::MessageBoxEx( RT_SHELL_BROKE_INTO_PARTS, msgTitleWarning );
        if ( notBrokenIntoParts )
          ::MessageBoxEx( RT_NOT_BROKEN_INTO_PARTS, msgTitleError );
        if ( boundaryEdgesInMostPart )
          ::MessageBoxEx( RT_BOUNDARY_EDGES_IN_MOST_PART, msgTitleError );
      }
      else {
        if ( shellCanBreakAPart )
          ::MessageBoxEx( RT_SOME_SHELLS_CAN_BREAK_APART, msgTitleWarning );
        if ( shellBrokeIntoParts )
          ::MessageBoxEx( RT_SOME_SHELLS_BROKE_INTO_PARTS, msgTitleWarning );
        if ( notBrokenIntoParts )
          ::MessageBoxEx( RT_SOME_SHELLS_NOT_BROKEN_INTO_PARTS, msgTitleError );
        if ( boundaryEdgesInMostPart )
          ::MessageBoxEx( RT_BOUNDARY_EDGES_IN_SOME_SHELLS_MOST_PART, msgTitleError );
      }
    }

    if ( isValid )
      ::MessageBoxEx( RT_DEFECTS_NOT_FOUND, msgTitleSuccess );
    else {
      if ( shellsCount == 1 )
        ::MessageBoxEx( RT_SHELL_DEFECTIVE, msgTitleError );
      else
        ::MessageBoxEx( RT_SOME_SHELLS_DEFECTIVE, msgTitleError );
    }

    res = isValid;
  }

  return res;
}


//------------------------------------------------------------------------------
// Проверить оболочку тела.
// Check solid shell.
// ---
bool CheckShellGeometry()
{
  MbItem * solidObject = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &solidObject ) ) {
    if ( solidObject == nullptr || solidObject->IsA() != st_Solid ) {
      return false;
    }

    TestVariables::viewManager->ToggleObject(solidObject);
    SetWaitCursor( true );

    MbSolid * solid = static_cast<MbSolid *>(solidObject);
    MbFaceShell * shell = solid->GetShell();
    ConstShellsVector shells;
    shells.push_back( shell );
    
    if ( TestVariables::permanentDiagnosticsShow )
    {
      ::ApplyPermanentDiagnosticVisual( *solid );
      TestVariables::viewManager->RefreshModel();
    }
    else{
      ::CheckShellsGeometry( shells );
    }
    
    //чтобы было видно краевые ребра. // to be seen boundary edges. //viewManager->ToggleObject(solidObject);
    //чтобы было видно краевые ребра. // to be seen boundary edges. //viewManager->RefreshObjectModel( solidObject );
    SetWaitCursor( false );
    TestVariables::viewManager->ToggleObject(solidObject);
    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Проверить оболочки тел.
// Check all solids shells.
// ---
bool CheckAllShellsGeometry()
{
  ItemsSPtrVector objs;
  std::vector<MbMatrix3D> matrs;

  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
  if ( wind != nullptr )
    wind->GetObjects( st_Solid, objs, matrs );

  size_t objsCnt = objs.size();

  if ( objsCnt > 0 ) {
    bool noTransform = true;

    ConstShellsVector shells;
    for ( size_t i = 0; i < objsCnt; ++i ) {
      if ( objs[i]->IsA() == st_Solid ) {
        const MbSolid * solid = &static_cast<MbSolid &>( *objs[i] );
        if ( !matrs[i].IsSingle() )
          noTransform = false;
        const MbFaceShell * shell = solid->GetShell();
        shells.push_back( shell );
      }
    }

    C3D_ASSERT( noTransform ); // надо учитывать матрицы или проверять на трансформированных копиях оболочек
    ::CheckShellsGeometry( shells );
  }

  return false;
}


//------------------------------------------------------------------------------
// Проверить совпадение указателей в оболочках.
// Check shells pointers coincidence.
// ---
void CheckShellsPointers()
{
  const TCHAR msgTitleWarning[] = IDS_TITLE_DIAG_WRN;
  const TCHAR msgTitleError[] = IDS_TITLE_DIAG_ERR;
  const TCHAR msgTitleSuccess[] = IDS_TITLE_DIAG_ALL_RIGHT;

  MbItem * obj1 = nullptr;
  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj1 ) ) {
    TestVariables::viewManager->ToggleObject( obj1 );
    MbItem * obj2 = nullptr;
    if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj2 ) ) {

      if ( obj2 != obj1 ) {
        TestVariables::viewManager->ToggleObject( obj2 );

        const MbSolid * solid1 = static_cast<const MbSolid *>(obj1);
        const MbSolid * solid2 = static_cast<const MbSolid *>(obj2);

        if ( solid1->GetFacesCount() > 0 && solid2->GetFacesCount() > 0 ) {
          const MbFaceShell * shell1 = solid1->GetShell();
          const MbFaceShell * shell2 = solid2->GetShell();

          c3d::ConstFacesVector faces1, faces2, faces;
          c3d::ConstEdgesVector edges1, edges2, edges;
          c3d::ConstVerticesVector vertices1, vertices2, vertices;

          shell1->GetFaces( faces1 );
          shell2->GetFaces( faces2 );
          shell1->GetItems( vertices1, edges1 );
          shell2->GetItems( vertices2, edges2 );

          c3d::UniqueSortVector( faces1 );
          c3d::UniqueSortVector( faces2 );
          c3d::UniqueSortVector( edges1 );
          c3d::UniqueSortVector( edges2 );
          c3d::UniqueSortVector( vertices1 );
          c3d::UniqueSortVector( vertices2 );

          faces.resize( faces1.size() + faces2.size() );
          edges.resize( edges1.size() + edges2.size() );
          vertices.resize( vertices1.size() + vertices2.size() );

          c3d::ConstFacesVector::iterator fit;
          c3d::ConstEdgesVector::iterator eit;
          c3d::ConstVerticesVector::iterator vit;

          fit = std::set_intersection( faces1.begin(), faces1.end(), faces2.begin(), faces2.end(), faces.begin() );
          eit = std::set_intersection( edges1.begin(), edges1.end(), edges2.begin(), edges2.end(), edges.begin() );
          vit = std::set_intersection( vertices1.begin(), vertices1.end(), vertices2.begin(), vertices2.end(), vertices.begin() );

          faces.resize( fit - faces.begin() );
          edges.resize( eit - edges.begin() );
          vertices.resize( vit - vertices.begin() );

          bool isOk = true;

          if ( !faces.empty() ) {
            ::MessageBoxEx( RT_FACES_SAME_POINTERS, msgTitleError );
            isOk = false;
          }
          if ( !edges.empty() ) {
            ::MessageBoxEx( RT_EDGES_SAME_POINTERS, msgTitleError );
            isOk = false;
          }
          if ( !vertices.empty() ) {
            ::MessageBoxEx( RT_VERTICES_SAME_POINTERS, msgTitleError );
            isOk = false;
          }

          if ( isOk ) {
            ::MessageBoxEx( RT_DEFECTS_NOT_FOUND, msgTitleSuccess );
          }
        }

        TestVariables::viewManager->ToggleObject( obj2 );
      }
      else {
        const MbSolid * solid = static_cast<const MbSolid *>(obj1);

        if ( solid->GetFacesCount() > 0 ) {
          const MbFaceShell * shell = solid->GetShell();

          c3d::ConstFacesVector faces;
          shell->GetFaces( faces );

          size_t facesCnt = faces.size();

          ::UniqueSortVector( faces );

          if ( faces.size() < facesCnt ) {
            ::MessageBoxEx( RT_FACES_SAME_POINTERS, msgTitleError );
          }
        }
      }
    }
    else {
      ::MessageBoxEx( RT_NOT_SELECTED_SECOND_OBJECT, msgTitleWarning );
    }

    TestVariables::viewManager->ToggleObject( obj1 );
  }
  else {
    ::MessageBoxEx( RT_NOT_SELECTED_FIRTS_OBJECT, msgTitleWarning );
  }
}


//------------------------------------------------------------------------------
// Вывернуть оболочку.
// Reverse a shell.
// ---
void ShellReverse()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {
    TestVariables::viewManager->ToggleObject( obj );

    SetWaitCursor( true );

    MbSolid * solid = static_cast<MbSolid *>(obj);

    if ( solid->GetFacesCount() > 0 ) {
      solid->Reverse();
      /*{ // SD_794021 Устранение дублирования вершин.
        bool doMergingEdges = true;
        RPArray<MbFace> controlFaces( 0, 1 );
        VERSION version = Math::DefaultMathVersion();
        solid->GetShell()->CheckTopology( doMergingEdges, controlFaces, version );
      }*/
      MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );
    }
    else
    {
      MessageBoxEx( IDS_FAILED, IDS_TITLE_MSG );
    }

    SetWaitCursor( false );

    TestVariables::viewManager->ToggleObject( obj );
  }
}


//------------------------------------------------------------------------------
// Reset bounding boxes of surfaces.
// ---
bool ResetSurfaceGabarit()
{
  bool wasSelected = false;

  bool wasChanged = false;

  SolidSPtr solid( ::SelectSolid() );

  bool updateParametricLimits = true;

  if ( solid != nullptr ) {
    wasSelected = true;

    MbFaceShell * shell = solid->GetShell();
    if ( shell != nullptr ) {
      if ( ::ResetSurfacesBoundingBoxes( *shell, updateParametricLimits ) )
        wasChanged = true;
    }
  }
  else {
    SurfaceSPtr surface( ::SelectSurface() );

    if ( surface != nullptr ) {
      wasSelected = true;
      if ( ResetSurfaceBoundingBoxes( *surface, updateParametricLimits ) )
        wasChanged = true;
    }
  }
  if ( wasChanged )
    MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );

  return wasSelected;
}


//------------------------------------------------------------------------------
// Reset temporary data of curves.
// ---
bool ResetCurvesTemporaryData()
{
  bool wasSelected = false;
  SolidSPtr solid( ::SelectSolid() );

  if ( solid != nullptr && solid->GetShell() != nullptr &&
       ::ResetCurvesTemporaryData(*solid->GetShell()) ) {
    MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );
    wasSelected = true;
  }
  return wasSelected;
}


//------------------------------------------------------------------------------
// Reset approximation of surfaces intersection curve
// ---
bool ResetIntCurveApproximation()
{
  bool wasSelected = false;

  SolidSPtr solid( ::SelectSolid() );

  static bool processAll = true;

  if ( solid != nullptr ) {
    wasSelected = true;

    bool isDone = false;
    EdgesSPtrVector edges;
    solid->GetEdges( edges );
    if ( !edges.empty() ) {
      isDone = true;
      for ( size_t i = edges.size(); i--; ) {
        if ( edges[i] != nullptr )
          edges[i]->SetIntersectionCurve().ReleaseSpaceCurve();
      }
    }
    if ( isDone )
      MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );
  }
  else if ( processAll ) {
    RPArray<MbItem> items;
    SArray<MbMatrix3D> matrs;
    TestVariables::viewManager->GetObjects( st_Solid, items, matrs );
    TestVariables::viewManager->GetObjects( st_Curve3D, items, matrs );
    TestVariables::viewManager->GetObjects( st_WireFrame, items, matrs );

    if ( !items.empty() ) {
      bool isDone = false;
      EdgesSPtrVector edges;
      WireEdgesSPtrVector wireEdges;

      for ( size_t k = items.size(); k--; ) {
        MbItem * item = items[k];
        MbeSpaceType itemType = item->Type();

        if ( itemType == st_Solid ) {
          MbSolid * sol = static_cast<MbSolid *>(item);
          if ( sol != nullptr ) {
            edges.clear();
            sol->GetEdges( edges );
            if ( !edges.empty() ) {
              isDone = true;
              for ( size_t i = edges.size(); i--; ) {
                if ( edges[i] != nullptr )
                  edges[i]->SetIntersectionCurve().ReleaseSpaceCurve();
              }
            }
          }
        }
        else if ( itemType == st_WireFrame ) {
          MbWireFrame * wf = static_cast<MbWireFrame *>(item);

          if ( wf->GetEdgesCount() > 0 ) {
            wireEdges.clear();
            wf->SetEdges( wireEdges );
            for ( size_t i = wireEdges.size(); i--; ) {
              if ( wireEdges[i] != nullptr ) {
                MbCurve3D & spaceCurve = wireEdges[i]->SetCurve();
                if ( spaceCurve.GetSubstrate().IsA() == st_SurfaceIntersectionCurve ) {
                  static_cast<MbSurfaceIntersectionCurve &>( spaceCurve.SetSubstrate() ).ReleaseSpaceCurve();
                  isDone = true;
                }
              }
            }
          }
        }
        else {
          SpaceCurveSPtr spaceCurve( ::GetCurve3D( item, true ) );
          if ( spaceCurve != nullptr && spaceCurve->GetSubstrate().IsA() == st_SurfaceIntersectionCurve ) {
            MbSurfaceIntersectionCurve & intCurve = static_cast<MbSurfaceIntersectionCurve &>(*spaceCurve);
            intCurve.ReleaseSpaceCurve();
            isDone = true;
          }
        }
      }
      if ( isDone )
        MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );
    }
  }

  return wasSelected;
}


//------------------------------------------------------------------------------
// Forced change closedness of a shell
// ---
void ForcedChangeShellClosedness()
{
  SPtr<MbSolid> solid( ::SelectSolid() );
  if ( solid->GetFacesCount() > 0 ) {
    bool isClosed = solid->IsClosed();
    isClosed = GetBool( IDS_ENTER_CLOSED_BOOL, isClosed );
    solid->GetShell()->SetClosed( isClosed );
  }
}

//------------------------------------------------------------------------------
// Remove inner loop
// ---
bool RemoveInnerLoop()
{
  MbItem * obj  = nullptr;
  MbTopItem * topItem = nullptr;

  if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &topItem ) ) {
    if ( topItem && topItem->IsAnEdge() ) {
      MbCurveEdge * edge = static_cast<MbCurveEdge *>( topItem );
      MbFace * face1 = edge->GetFacePlus();
      MbFace * face2 = edge->GetFaceMinus();

      bool b1, b2;
      size_t ln1, en1, ln2, en2;
      b1 = b2 = false;
      ln1 = en1 = ln2 = en2 = SYS_MAX_T;

      if ( face1 != nullptr ) {
        b1 = face1->FindEdgeIndex( *edge, ts_neutral, ln1, en1 );
        if ( b1 && ln1 == SYS_MAX_T )
          b1 = false;
      }
      if ( face2 != nullptr ) {
        b2 = face2->FindEdgeIndex( *edge, ts_neutral, ln2, en2 );
        if ( b2 && ln2 == SYS_MAX_T )
          b2 = false;
      }
      if ( b1 && (ln1 < 1) )
        b1 = false;
      if ( b2 && (ln2 < 1) )
        b2 = false;

      bool del = false;

      if ( b1 != b2 ) {
        MbFace * face = b1 ? face1 : face2;
        size_t ln = b1 ? ln1 : ln2;
        if ( ln > 0 ) {
          face->DeleteLoop( ln );
          face->UpdateSurfaceBounds();
          del = true;
        }
      }
      else if ( b1 && b2 ) {
        if ( ln1 > 0 ) {
          face1->DeleteLoop( ln1 );
          face1->UpdateSurfaceBounds();
          del = true;
        }
        if ( ln2 > 0 ) {
          face2->DeleteLoop( ln2 );
          face2->UpdateSurfaceBounds();
          del = true;
        }
      }

      if ( del ) {
        TestVariables::viewManager->RefreshObjectModel( obj );
        ::MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );
        return true;
      }
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Repair inexact edges
// ---
bool RepairInexactEdges()
{
  SolidSPtr solid( SelectSolid() );

  if ( solid && solid->GetFacesCount() > 0 ) {
    MbFaceShell * shell = solid->GetShell();
    double metricAcc = Math::metricPrecision;

    if ( RepairEdges( *shell ) ) {
      bool isDone = true;
      ConstEdgesVector edges;
      shell->GetEdges( edges );
      for ( size_t k = edges.size(); k--; ) {
        if ( edges[k] && ::IsInexactEdge( *edges[k], metricAcc ) ) {
          isDone = false;
          break;
        }
      }
      if ( isDone )
        ::MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );
      else {
        ::MessageBoxEx( RT_DONE_IN_PART, IDS_TITLE_MSG );
      }
      return true;
    }
    else {
      ::MessageBoxEx( RT_DO_NOT_DO, IDS_TITLE_MSG );
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Repair vetrices
// ---
bool RepairVertices()
{
  SolidSPtr solid( SelectSolid() );

  if ( solid && solid->GetFacesCount() > 0 ) {
    MbFaceShell * shell = solid->GetShell();

    MbAccurateTimer ac;
    ac.Begin();

    bool wasChanged = RepairVertices( *shell );

    ac.End();

    if ( TestVariables::checkOperationTime ) {
      const size_t bufLen = 1024;
      TCHAR buf[bufLen];
      size_t len = 0;
      len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
      ::MessageBoxEx( buf, IDS_RESULTS );
    }

    if ( wasChanged ) {
      ::MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );
      return true;
    }
    else {
      ::MessageBoxEx( RT_DO_NOT_DO, IDS_TITLE_MSG );
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Починить тело. Solid repairing. 
// ---
bool SolidRepairing()
{
  bool isDone = false;
  MbMatrix3D * from( nullptr );
  MbSolid * solid = ::SelectSolid( from );

  if ( solid != nullptr && solid->GetFacesCount() > 0 ) {
    double accuracy = Math::metricPrecision;
    ::GetDouble( IDS_ENTER_ACCURACY, accuracy );

    if ( ::SolidRepairing( *solid, accuracy ) ) {
      isDone = true;
      ::MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );
    }
    else
      ::MessageBoxEx( RT_DONE_IN_PART, IDS_TITLE_MSG );
  }

  return isDone;
}


//------------------------------------------------------------------------------
// Разрушить сборки с подсборками на составляющие.
// Decompose assemblies and subassemblies.
// ---
void DecomposeAllAssemblies() {
  TestVariables::viewManager->DecomposeAssemblies();
}


//------------------------------------------------------------------------------
// Удалить все визуальные свойства объектов.
// Remove all visual properties of objects.
// ---
void RemoveAllColorAndVisualAttributes() {
  TestVariables::viewManager->RemoveColorAndVisualAttributes();
}


//------------------------------------------------------------------------------
// Предпроверка тел на пересечение.
// Precheck solids at the intersection.
// ---
bool IsSolidsIntersection()
{
  bool res = false;

  MbItem * obj1 = nullptr;
  MbItem * obj2 = nullptr;
  PathMatrix pathFrom1, pathFrom2;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj1, pathFrom1 ) && (obj1->IsA() == st_Solid) ) {
    TestVariables::viewManager->ToggleObject( obj1 );
    if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj2, pathFrom2 ) && (obj2->IsA() == st_Solid) ) {
      TestVariables::viewManager->ToggleObject( obj2 );

      SolidSPtr solid1( static_cast<MbSolid *>(obj1) );
      SolidSPtr solid2( static_cast<MbSolid *>(obj2) );
      MbSNameMaker snMaker( SimpleName(-1), MbSNameMaker::i_SideNone, 0 );

      bool findAllIntersection = false;

      findAllIntersection = GetBool( IDS_ENTER_SHOW_ALL, findAllIntersection );

      PArray<MbShellsIntersectionData> intData( 0, 1, true );

      if ( findAllIntersection ) {
        static bool checkTangent = true;
        static bool getIntersectionSolids = true;
        static bool checkTouchPoints = false;

        MbAccurateTimer ac;

        SetWaitCursor( true );

        ac.Begin();

        res = ::IsSolidsIntersection( *solid1, pathFrom1.second, *solid2, pathFrom2.second, 
                                      checkTangent, getIntersectionSolids, checkTouchPoints,
                                      intData );
        ac.End();

        SetWaitCursor( false );

        TestVariables::drawImp->RedrawModel(); // убрать отладочную отрисовку появившуюся в процессе булевой

        if ( TestVariables::checkOperationTime ) {
          const size_t bufLen = 256;
          TCHAR buf[bufLen];
          size_t len = 0;
          len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
          ::MessageBoxEx( buf, IDS_RESULTS );
        }
      }
      else {
        if ( !pathFrom1.second.IsSingle() ) {
          ShellSPtr shell( solid1->GetShell()->Duplicate() );
          solid1 = new MbSolid( shell, nullptr );
          solid1->Transform( pathFrom1.second );
        }
        if ( !pathFrom2.second.IsSingle() ) {
          ShellSPtr shell( solid2->GetShell()->Duplicate() );
          solid2 = new MbSolid( shell, nullptr );
          solid2->Transform( pathFrom2.second );
        }

        MbAccurateTimer ac;

        SetWaitCursor( true );

        ac.Begin();

        res = ::IsSolidsIntersection( *solid1, *solid2, snMaker );

        ac.End();

        SetWaitCursor( false );

        TestVariables::drawImp->RedrawModel(); // убрать отладочную отрисовку появившуюся в процессе булевой

        if ( TestVariables::checkOperationTime ) {
          const size_t bufLen = 256;
          TCHAR buf[bufLen];
          size_t len = 0;
          len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
          ::MessageBoxEx( buf, IDS_RESULTS );
        }
      }

      if ( res ) {
        IndicesVector numbers1;
        IndicesVector numbers2;
        c3d::IndicesPairsVector numbersPairs;

        for ( size_t i = 0; i < intData.size(); i++ ) {
          const MbShellsIntersectionData * id = intData[i];
          if ( id == nullptr ) {
            C3D_ASSERT_UNCONDITIONAL( false );
            continue;
          }

          if ( id->IsSurface() ) {
            ConstEdgesVector curves;
            id->GetCurves( curves );
            numbers1.clear();
            numbers2.clear();
            numbersPairs.clear();
            id->GetFaceNumbers( true, numbers1 );
            id->GetFaceNumbers( false, numbers2 );
            id->GetFaceNumbersPairs( numbersPairs );

            size_t j, jCount;
            for ( j = 0, jCount = numbers1.size(); j < jCount; ++j ) {
              MbFace * face = solid1->GetFace( numbers1[j] );
              TestVariables::drawImp->DrawItem( face, TRGB_AZURE, pathFrom1.second );
            }
            for ( j = 0, jCount = numbers2.size(); j < jCount; ++j ) {
              MbFace * face = solid2->GetFace( numbers2[j] );
              TestVariables::drawImp->DrawItem( face, TRGB_OLIVE, pathFrom2.second );
            }
            for ( j = 0, jCount = curves.size(); j < jCount; ++j ) {
              TestVariables::drawImp->DrawItem( curves[j], TRGB_RED, MbMatrix3D::identity, 2 );
            }
          }
          else if ( id->IsSolid() ) {
            const MbSolid * solid = id->GetSolid();
            TestVariables::drawImp->DrawItem( solid, TRGB_CERISE, 2 );
          }
          else if ( id->IsPoint() ) {
            const MbPointFrame * pointsFrame = id->GetPointFrame();
            if ( pointsFrame ) {
              TestVariables::drawImp->DrawItem( pointsFrame, TRGB_BLUE, 3 );
            }
          }
        }

        ::MessageBoxEx( RT_INTERSECT, IDS_TITLE_MSG );

        for ( size_t i = 0; i < intData.size(); i++ ) {
          const MbShellsIntersectionData * id = intData[i];
          if ( id == nullptr )
            continue;

          if ( id->IsSurface() ) {
            ConstEdgesVector curves;
            id->GetCurves( curves );
            numbers1.clear();
            numbers2.clear();
            id->GetFaceNumbers( true, numbers1 );
            id->GetFaceNumbers( false, numbers2 );

            size_t j, jCount;
            for ( j = 0, jCount = curves.size(); j < jCount; j++ ) {
              TestVariables::drawImp->DrawItem( curves[j], TRGB_WHITE );
            }
            for ( j = 0, jCount = numbers1.size(); j < jCount; j++ ) {
              MbFace * face = solid1->GetFace( numbers1[j] );
              TestVariables::drawImp->DrawItem( face, TRGB_WHITE );
            }
            for ( j = 0, jCount = numbers2.size(); j < jCount; j++ ) {
              MbFace * face = solid2->GetFace( numbers2[j] );
              TestVariables::drawImp->DrawItem( face, TRGB_WHITE, MbMatrix3D::identity,2 );
            }
          }
          else if ( id->IsSolid() ) {
            const MbSolid * solid = id->GetSolid();
            TestVariables::drawImp->DrawItem( solid, TRGB_WHITE, 2 );
          }
          else if ( id->IsPoint() ) {
            const MbPointFrame * pointsFrame = id->GetPointFrame();
            if ( pointsFrame ) {
              TestVariables::drawImp->DrawItem( pointsFrame, TRGB_WHITE, 3 );
            }
          }
        }
      }
      else
        ::MessageBoxEx( RT_NOINTERSECT, IDS_TITLE_MSG );

      TestVariables::viewManager->ToggleObject( obj2 );

      res = true;
    }
    TestVariables::viewManager->ToggleObject( obj1 );
  }

  return res;
}


//------------------------------------------------------------------------------
// Проверить абрис триангуляции тел на пересечение.
// Check the outline of triangulation at the intersection.
// ---
bool IsConvexHullsIntersection()
{
  bool res = false;

  MbItem * obj1 = nullptr;
  MbItem * obj2 = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj1 ) ) {
    TestVariables::viewManager->ToggleObject( obj1 );
    if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj2 ) ) {
      TestVariables::viewManager->ToggleObject( obj2 );

      SetWaitCursor( true );

      MbSolid * solid1 = static_cast<MbSolid *>(obj1);
      MbSolid * solid2 = static_cast<MbSolid *>(obj2);
      MbSNameMaker snMaker( SimpleName(-1), MbSNameMaker::i_SideNone, 0 );

      double sag = 0.2;
      MbItem * mesh1 = solid1->CreateMesh( MbStepData(ist_SpaceStep, sag), MbFormNote(true, true, true, TestVariables::exactMesh), nullptr );
      MbItem * mesh2 = solid2->CreateMesh( MbStepData(ist_SpaceStep, sag), MbFormNote(true, true, true, TestVariables::exactMesh), nullptr );
      MbMesh * convexMesh1( nullptr );
      MbMesh * convexMesh2( nullptr );
      double eps = Math::metricNear;
      ::CreateConvexPolyhedron( static_cast<MbMesh &>(*mesh1), eps * 0.5, convexMesh1 );
      ::CreateConvexPolyhedron( static_cast<MbMesh &>(*mesh2), eps * 0.5, convexMesh2 );
      if ( convexMesh1 != nullptr && convexMesh2 != nullptr &&
           ::AreIntersectConvexPolyhedrons(*convexMesh1, *convexMesh2) )
        res = true;
      ::DeleteItem( convexMesh1 );
      ::DeleteItem( convexMesh2 );
      ::DeleteItem( mesh1 );
      ::DeleteItem( mesh2 );

      if ( res )
        MessageBoxEx( RT_INTERSECT, IDS_TITLE_MSG );
      else
        MessageBoxEx( RT_NOINTERSECT, IDS_TITLE_MSG );

      SetWaitCursor( false );

      TestVariables::viewManager->ToggleObject( obj2 );
    }
    TestVariables::viewManager->ToggleObject( obj1 );
  }

  return res;
}


//------------------------------------------------------------------------------
//
// ---
template <class ItemsVector, class Attribute>
void SetColor( const ItemsVector & items, const Attribute & attr )
{
  for ( size_t k = 0, kcnt = items.size(); k < kcnt; k++ ) {
    if ( items[k] != nullptr )
      items[k]->AddAttribute( attr );
  }
}

//------------------------------------------------------------------------------
// Найти родительские грани оболочки.
// Find overlapping similar faces.
// ---
void FindOverlappingSimilarFaces()
{
  MbItem * obj1 = nullptr;
  MbItem * obj2 = nullptr;

  if ( GetGObj( IDS_SELECT_FIRST_SOLID, st_Solid, &obj1 ) ) {
    TestVariables::viewManager->SelectObject( obj1 );
    if ( GetGObj( IDS_SELECT_SECOND_SOLID, st_Solid, &obj2 ) ) {
      TestVariables::viewManager->SelectObject( obj2 );

      const MbSolid * solid1 = static_cast<const MbSolid *>(obj1);
      const MbSolid * solid2 = static_cast<const MbSolid *>(obj2);

      if ( solid1 != nullptr && solid2 != nullptr && solid1->GetShell() != nullptr && solid2->GetShell() != nullptr ) {
        bool found = false;
        std::vector<IndicesPair> simPairs;

        SetWaitCursor( true );

        bool sameNormals = true;
        {
          int iInverse = 0;
          GetInt( IDS_ENTER_INVERSE, iInverse );
          if ( iInverse != 0 )
            sameNormals = false;
        }

        if ( ::FindOverlappingSimilarFaces( *solid1->GetShell(), *solid2->GetShell(), sameNormals, simPairs ) ) {
          for ( size_t k = 0, kcnt = simPairs.size(); k < kcnt; k++ ) {
            size_t ind1 = simPairs[k].first;
            size_t ind2 = simPairs[k].second;

            SPtr<MbFace> face1( solid1->GetFace( ind1 )->DataDuplicate() );
            SPtr<MbFace> face2( solid2->GetFace( ind2 )->DataDuplicate() );

            SPtr<MbVisual> color1( MbVisual::Create( ::RGB2uint32( 1.0, 0.0, 1.0 ) ) );
            SPtr<MbVisual> color2( MbVisual::Create( ::RGB2uint32( 0.0, 0.5, 1.0 ) ) );
            face1->AddAttribute( color1 );
            face2->AddAttribute( color2 );

            RPArray<MbCurveEdge> edges;
            face1->GetEdges( edges );
            ::SetColor( edges, color1 );
            edges.clear();
            face2->GetEdges( edges );
            ::SetColor( edges, color2 );

            c3d::ShellSPtr shell( new MbFaceShell );
            shell->AddFace( *face1 );
            shell->AddFace( *face2 );
            shell->SetClosed( false );

            c3d::SolidSPtr solid( new MbSolid( shell, nullptr ) );

            Style style;
            TestVariables::viewManager->AddObject( style, solid );
            found = true;
          }
        }
        else {
          MessageBoxEx( RT_MATCHES_NOT_FOUND, IDS_TITLE_MSG );
        }

        SetWaitCursor( false );

        if ( !found ) {
          TestVariables::viewManager->UnselectObject( obj1 );
          TestVariables::viewManager->UnselectObject( obj2 );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Найти грани - потенциальные носители ребер.
// Find faces - potential carriers of edges.
// ---
void FindFacesEdgesCarriers()
{
  MbItem * obj1 = nullptr;
  MbItem * obj2 = nullptr;

  if ( GetGObj( IDS_SELECT_FIRST_SOLID, st_Solid, &obj1 ) ) {
    TestVariables::viewManager->SelectObject( obj1 );
    if ( GetGObj( IDS_SELECT_SECOND_SOLID, st_Solid, &obj2 ) ) {
      TestVariables::viewManager->SelectObject( obj2 );

      const MbSolid * solid1 = static_cast<const MbSolid *>(obj1);
      const MbSolid * solid2 = static_cast<const MbSolid *>(obj2);

      if ( solid1 != nullptr && solid2 != nullptr && solid1->GetShell() != nullptr && solid2->GetShell() != nullptr ) {
        bool found = false;
        std::vector<IndicesPair> efPairs;

        SetWaitCursor( true );

        bool sameNormals = true;
        {
          int iInverse = 0;
          GetInt( IDS_ENTER_INVERSE, iInverse );
          if ( iInverse != 0 )
            sameNormals = false;
        }

        ConstEdgesVector edges;
        {
          RPArray<MbCurveEdge> edgesArray;
          solid2->GetEdges( edgesArray );
          size_t edgesCnt = edgesArray.size();
          edges.reserve( edgesCnt );
          for ( size_t k = 0; k < edgesCnt; k++ ) {
            edges.push_back( edgesArray[k] );
          }
        }

        if ( ::FindFacesEdgesCarriers( edges, *solid1->GetShell(), sameNormals, efPairs ) ) {
          for ( size_t k = 0, kcnt = efPairs.size(); k < kcnt; k++ ) {
            size_t edgeInd = efPairs[k].first;
            size_t faceInd = efPairs[k].second;

            SPtr<MbCurve3D> curve( static_cast<MbCurve3D *>( &edges[edgeInd]->GetCurve().Duplicate() ) );
            SPtr<MbWireFrame> wireFrame( new MbWireFrame( *curve ) );
            SPtr<MbFace> face( solid1->GetFace( faceInd )->DataDuplicate() );

            SPtr<MbVisual> color1( MbVisual::Create( ::RGB2uint32( 1.0, 0.0, 1.0 ) ) );
            SPtr<MbVisual> color2( MbVisual::Create( ::RGB2uint32( 0.0, 0.5, 1.0 ) ) );
            wireFrame->AddAttribute( color1 );
            face->AddAttribute( color2 );

            RPArray<MbCurveEdge> faceEdges;
            face->GetEdges( faceEdges );
            ::SetColor( faceEdges, color2 );

            c3d::ShellSPtr shell( new MbFaceShell( *face ) );
            shell->SetClosed( false );

            c3d::SolidSPtr solid( new MbSolid( shell, nullptr ) );

            Style style;
            TestVariables::viewManager->AddObject( style, wireFrame );
            TestVariables::viewManager->AddObject( style, solid );
            found = true;
          }
        }
        else {
          MessageBoxEx( RT_MATCHES_NOT_FOUND, IDS_TITLE_MSG );
        }

        SetWaitCursor( false );

        if ( !found ) {
          TestVariables::viewManager->UnselectObject( obj1 );
          TestVariables::viewManager->UnselectObject( obj2 );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Найти ближайшее тело. Для операции выдавливания до ближайшей оболочки.
// Find nearest solid. For an extrusion operation to the nearest shell.
// ---
void FindNearestSolid()
{
  bool bWireFrame = false;
  DPtr<MbSweptData> sweptData = ::SelectSweptData( bWireFrame );

  if ( sweptData != nullptr ) {
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();

    if ( wind != nullptr ) {
      MbModel & model = wind->SetGeomModel();

      std::vector<ItemSPtr> items;
      std::vector<MbMatrix3D> matrices;
      model.GetItems( st_Solid, items, matrices );

      if ( items.size() == matrices.size() ) {
        for ( size_t i = items.size(); i--; ) {
          if ( items[i] == nullptr || items[i]->IsA() != st_Solid ) {
            items.erase( items.begin() + i );
            matrices.erase( matrices.begin() + i );
          }
        }

        std::vector<SolidSPtr> solids;
        for ( size_t i = 0; i < items.size(); ++i ) {
          MbSolid * solid = static_cast<MbSolid *>( &items[i]->Duplicate() );
          solid->Transform ( matrices[i] );
          solids.push_back ( SolidSPtr( solid ) );
        }

        MbVector3D direction( 1.0, 0.0, 0.0 );
        bool directionByPlaceNormal = false;
        if ( GetBool( IDS_PLACE_NORMAL_DIR, directionByPlaceNormal ) ) {
          MbPlacement3D place;
          if ( sweptData->GetSurface() != nullptr && sweptData->GetSurface()->GetPlacement( &place ) ) {
            direction = place.GetAxisZ();
          }
          else {
            MbCartPoint3D dirPoint;
            if ( GetPoint( IDS_ENTER_DIR, dirPoint ) ) { 
              direction.Init( dirPoint );
            }
          }
        }

        MbSweptLayout::Direction operationDirection = MbSweptLayout::ed_plus;
        int sense = 1;
        GetInt( IDS_ENTER_SENSE, sense ); // (+1: forward, -1: reversed)
        if ( sense == -1 )
          operationDirection = MbSweptLayout::ed_minus;
        else if ( sense == 0 )
          operationDirection = MbSweptLayout::ed_both;

        size_t iterCount = solids.size();
        MbNearestSolidParams params( *sweptData, direction, operationDirection, solids );
        while( iterCount ) {
          MbNearestSolidResult result;
          ::GetNearestSolid( params, result );

          size_t plus = result.GetNearestPlus();
          if ( plus != SYS_MAX_T ) {
            params.ChangeSolid( plus, nullptr ); // solids[plus] = nullptr;
            TestVariables::viewManager->SelectObject( items[plus] );
          }

          size_t minus = result.GetNearestMinus();
          if ( minus != SYS_MAX_T ) {
            params.ChangeSolid( minus, nullptr ); // solids[minus] = nullptr;
            TestVariables::viewManager->SelectObject( items[minus] );
          }

          if ( plus == SYS_MAX_T && minus == SYS_MAX_T )
            break;

          iterCount--;
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Проверить триангуляцию грани.
// Check triangulation of a face.
// ---
void CheckFaceTriangulation()
{
  MbFloatGrid grid; // MbExactGrid grid;
  MbStepData stepData;
  MbFace * face;
  size_t count = 0;
  double minSag( MB_MAXDOUBLE ), maxSag( 0.0 ), averageSag( 0.0 );
  double sag = 0.2;
  double visSag = stepData.GetSag();
  double devSag = stepData.GetAngle();
  double length = stepData.GetLength();
  stepData.Init( ist_SpaceStep, visSag, devSag, length );
  stepData.SetStepType( ist_ParamStep );

  MbItem * obj( nullptr );
  MbTopItem * item( nullptr );

  if ( ::GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &item ) ) {
    face = static_cast<MbFace *>( item );
    MbSurface & surf = face->SetSurface().SetSurface();

    if ( ::GetDouble( IDS_ENTER_SAG_ARROW, sag ) ) {
      stepData.SetSag( sag );

      ::CalculateGrid( *face, stepData, grid, true, false, false );

      size_t triCount = grid.TrianglesCount();

      for ( size_t i = 0; i < triCount; i++ ) {
        MbCartPoint3D p0, p1, p2;
        MbCartPoint3D mp0, mp1, mp2;
        grid.GetTrianglePoints( i, p0, p1, p2 );

        MbCartPoint r0, r1, r2;
        grid.GetTriangleParams( i, r0, r1, r2 );

        mp0.Set( p0, 0.5, p1, 0.5 ); // середины сторон треугольника
        mp1.Set( p1, 0.5, p2, 0.5 );
        mp2.Set( p2, 0.5, p0, 0.5 );

        MbCartPoint smp0( r0 ), smp1( r1 ), smp2( r2 ); // проекции середин сторон треугольника на поверхность
        MbCartPoint3D sp0, sp1, sp2;

        if ( ::fabs( r0.x - r1.x ) < PARAM_EPSILON || ::fabs( r0.y - r1.y ) < PARAM_EPSILON ) { // проверяем прогиб только вдоль координатных линий
          surf.NearPointProjection( mp0, smp0.x, smp0.y, false );
          surf.PointOn( smp0, sp0 );
          double len0 = mp0.DistanceToPoint( sp0 );

          if ( len0 < minSag )
            minSag = len0;
          if ( len0 > maxSag )
            maxSag = len0;

          averageSag += len0;
          count++;
        }

        if ( ::fabs( r1.x - r2.x ) < PARAM_EPSILON || ::fabs( r1.y - r2.y ) < PARAM_EPSILON ) {
          surf.NearPointProjection( mp1, smp1.x, smp1.y, false );
          surf.PointOn( smp1, sp1 );
          double len1 = mp1.DistanceToPoint( sp1 );

          if ( len1 < minSag )
            minSag = len1;
          if ( len1 > maxSag )
            maxSag = len1;

          averageSag += len1;
          count++;
        }

        if ( ::fabs( r2.x - r0.x ) < PARAM_EPSILON || ::fabs( r2.y - r0.y ) < PARAM_EPSILON ) {
          surf.NearPointProjection( mp2, smp2.x, smp2.y, false );
          surf.PointOn( smp2, sp2 );
          double len2 = mp2.DistanceToPoint( sp2 );

          if ( len2 < minSag )
            minSag = len2;
          if ( len2 > maxSag )
            maxSag = len2;

          averageSag += len2;
          count++;
        }
      }
      averageSag /= count;

      TCHAR s[1024];
      _sntprintf( s, 1024, RT_SAG, sag, minSag, maxSag, averageSag );
      MessageBoxEx( s, IDS_DONE );
    }
  }
}


//------------------------------------------------------------------------------
// Показать все ребра, стыкующиеся в вершине, добавив в модель копии кривых.
// Show all edges connected in the vertex, adding to the model copies of the curves.
// ---
void AddVertexEdgesAsCurves()
{
  MbItem * objSolid = nullptr;
  MbTopItem * item = nullptr;

  if ( GetTObj( IDS_SELECT_VERTEX, st_Solid, tt_Vertex, &objSolid, &item, TestVariables::itemPathFrom.second ) &&
       (objSolid->IsA() == st_Solid) && (item->IsA() == tt_Vertex) ) {
    MbVertex * vertex = (MbVertex *)item;
    MbSolid * solid = (MbSolid *)objSolid;

    if ( solid != nullptr && solid->GetShell() != nullptr ) {
      SetWaitCursor( true );

      RPArray<MbCurveEdge> edgeArr( 0, 1 );
      TestVariables::viewManager->ToggleObject(objSolid);

      solid->FindEdgesForVertex( *vertex, edgeArr );

      size_t cntEdges = edgeArr.Count();

      for ( size_t iEdge = 0; iEdge < cntEdges; iEdge++ ) {
        MbCurve3D & curve = (MbCurve3D &)edgeArr[iEdge]->GetCurve().Duplicate();
        TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, &curve );
      }
      TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, new MbPointFrame( *vertex, false ) );

      TCHAR msgResult[64];
      _sntprintf( msgResult, 64, RT_EDGES_COUNT F_TD _T("\n\r"), cntEdges ); //-V111
      MessageBoxEx( msgResult, IDS_TITLE_MSG );

      TestVariables::viewManager->ToggleObject(objSolid);
      SetWaitCursor( false );
    }
  }
}


//------------------------------------------------------------------------------
// Показать все ребра тела, добавив в модель копии кривых.
// Show all edges of the solid, adding to the model copies of the curves.
// ---
void AddSolidEdgesAsCurves()
{
  MbItem * objSolid = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &objSolid ) ) {
    TestVariables::viewManager->ToggleObject(objSolid);
    SetWaitCursor( true );
    MbSolid * solid = (MbSolid *)objSolid;

    if ( solid != nullptr && solid->GetShell() != nullptr ) {
      RPArray<MbCurveEdge> edgeArr( 10, 10 );
      solid->GetEdges( edgeArr );
      size_t cntEdges = edgeArr.Count();

      static bool addApproxCurves = false;

      for ( size_t iEdge = 0; iEdge < cntEdges; iEdge++ ) {
        MbCurve3D & curve = (MbCurve3D &)edgeArr[iEdge]->GetCurve().Duplicate();
        TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, &curve );
        if ( addApproxCurves && curve.IsA() == st_SurfaceIntersectionCurve ) {
          MbSurfaceIntersectionCurve & intCurve = static_cast<MbSurfaceIntersectionCurve &>( curve );
          const MbCurve3D * approxCurve = intCurve.GetSpaceCurve();
          if ( approxCurve && approxCurve != &curve ) {
            TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, static_cast<MbCurve3D *>( &approxCurve->Duplicate() ) );
          }
        }
      }

      TCHAR msgResult[64];
      _sntprintf( msgResult, 64, RT_EDGES_COUNT F_TD _T("\n\r"), cntEdges ); //-V111
      MessageBoxEx( msgResult, IDS_TITLE_MSG );
    }

    SetWaitCursor( false );
  }
}


//------------------------------------------------------------------------------
// Показать все вершины тела, добавив в модель копии точек.
// Show all vertices of the solid, adding to the model copies of the points.
// ---
void AddSolidVertsAsPoints()
{
  MbItem * objSolid = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &objSolid ) ) {
    TestVariables::viewManager->ToggleObject(objSolid);
    SetWaitCursor( true );
    MbSolid * solid = (MbSolid *)objSolid;

    if ( solid != nullptr && solid->GetShell() != nullptr ) {
      RPArray<MbVertex> vertArr( 10, 10 );
      solid->GetVertices( vertArr );

      size_t i, cntVerts = vertArr.Count();

      MbPointFrame * vertPoints = new MbPointFrame();
      for ( i = 0; i < cntVerts; i++ )
        vertPoints->AddVertex( vertArr[i]->GetCartPoint() );
      TestVariables::viewManager->AddObject( TestVariables::POINT_Style, vertPoints );

      TCHAR msgResult[64];
      _sntprintf( msgResult, 64, RT_VERTICES_COUNT F_TD _T("\n\r"), cntVerts ); //-V111
      MessageBoxEx( msgResult, IDS_TITLE_MSG );
    }

    SetWaitCursor( false );
  }
}


//------------------------------------------------------------------------------
// Выставить последнюю версию.
// Set solid last version.
// ---
bool SetSolidLastVersion()
{
  bool result( false );
  MbItem * obj = nullptr;
  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {
    SetWaitCursor( true );

    TestVariables::viewManager->ToggleObject( obj );

    MbSolid * solid = (MbSolid *)obj;

    if ( solid != nullptr ) {
      RPArray<MbCreator> creators( 0, 1 );

      MbTransactions * solidTransactions = static_cast<MbTransactions *>(solid);

      if ( solidTransactions != nullptr )
        solidTransactions->GetCreators( creators );
      size_t cnt = creators.Count();
      if ( cnt > 0 ) {
        MbSNameMaker snMaker( SimpleName(-1), MbSNameMaker::i_SideNone, 0 );
        for ( size_t k = 0; k < cnt; k++ ) {
          MbCreator * creator = creators[k];
          if ( creator != nullptr ) {
            creator->SetYourVersion( Math::DefaultMathVersion(), true );
            result = true;
          }
        }
      }
    }

    TestVariables::viewManager->ToggleObject( obj );
    if ( !result )
      MessageBoxEx( RT_ERROR_RESULT_NOT_OBTAINED, IDS_TITLE_MSG );

    SetWaitCursor( false );
  }
  return result;
}

//------------------------------------------------------------------------------
// Показать направление кривой
// Show curve direction.
// ---
bool ShowCurveDirection()
{
  MbItem * obj = nullptr;

  SPtr<const MbCurve3D> curve;
  bool sense = true;

  if ( curve == nullptr ) { // Выбор кривой. // Choose a curve.
    if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) )
      curve = ::GetCurve3D( obj, true );
    else {
      MbTopItem * tItem = nullptr;
      if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &tItem ) && (tItem != nullptr) ) {
        if ( tItem->IsA() == tt_CurveEdge ) {
          const MbCurveEdge * edge = static_cast<const MbCurveEdge *>(tItem);
          curve = &edge->GetIntersectionCurve();
          sense = edge->IsSameSense();
        }
      }
    }
  }

  if ( curve != nullptr ) {
    double t = sense ? curve->GetTMin() : curve->GetTMax();
    MbCartPoint3D pnt;
    curve->_PointOn( t, pnt );
    MbVector3D dir;
    curve->_FirstDer( t, dir );

    double dirLen = dir.Length(); // SKIP_SA (Не вытирать. Посмотреть значение в отладке)

    double crvLen = curve->GetLengthEvaluation();

    if ( dirLen < DELTA_MOD * crvLen ) {
      dir.Normalize();
      dir *= (DELTA_MOD * crvLen);
    }
    else if ( dirLen > 0.5 * crvLen ) {
      dir.Normalize();
      dir *= (0.5 * crvLen);
    }

    if ( !sense )
      dir = -dir;

    SPtr<MbLineSegment3D> ls( new MbLineSegment3D( pnt, dir ) );

    PointFrameSPtr pf( new MbPointFrame( pnt ) );
    WireFrameSPtr  wf( new MbWireFrame( *ls ) );

    pf->SetWidth( 4 );
    wf->SetWidth( 2 );

    if ( sense ) {
      pf->SetColor( TRGB_RED );
      wf->SetColor( TRGB_PURPLE );
    }
    else {
      pf->SetColor( TRGB_BLUE );
      wf->SetColor( TRGB_AZURE );
    }

    TestVariables::viewManager->AddObject( *pf );
    TestVariables::viewManager->AddObject( *wf );

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Показать направление кривой
// Show surface directions.
// ---
bool ShowSurfaceDirections()
{
  MbItem * obj = nullptr;

  SPtr<const MbSurface> surface;

  { // Выбор поверхности. // Choose a surface.
    MbTopItem * tItem = nullptr;
    if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &tItem ) && tItem != nullptr ) {
      if ( tItem->IsA() == tt_Face ) {
        const MbFace * face = static_cast<const MbFace *>(tItem);
        surface = &face->GetSurface();
      }
    }
    if ( (surface == nullptr) && GetGObj( IDS_SELECT_SURFACE, st_Surface, &obj ) ) {
      surface = static_cast<const MbSurface *>( static_cast<MbSpaceInstance *>(obj)->GetSpaceItem() );
    }
  }

  if ( surface != nullptr ) {
    double u = surface->GetUMid();
    double v = surface->GetVMid();

    MbCartPoint3D pnt;
    MbVector3D uDer, vDer;
    surface->Explore( u, v, true, pnt, uDer, vDer, nullptr, nullptr, nullptr, nullptr );

    MbCube cube;
    surface->AddYourGabaritTo( cube );
    double len = cube.GetDiagonal();
    len *= DELTA_MOD;
    len = std_max( len, DELTA_MOD );

    uDer.Normalize();
    vDer.Normalize();
    uDer *= len;
    vDer *= len;

    SPtr<MbLineSegment3D> uAxis( new MbLineSegment3D( pnt, uDer ) );
    SPtr<MbLineSegment3D> vAxis( new MbLineSegment3D( pnt, vDer ) );

    Style uStyle, vStyle, pStyle;
    pStyle.SetColor( ::RGB2uint32( 0, 0, 1 ) );
    uStyle.SetColor( ::RGB2uint32( 1, 0, 0 ) );
    vStyle.SetColor( ::RGB2uint32( 0, 1, 0 ) );
    pStyle.SetWidth( 2 );
    uStyle.SetWidth( 2 );
    vStyle.SetWidth( 2 );

    TestVariables::viewManager->AddObject( pStyle, new MbPointFrame(pnt) );
    TestVariables::viewManager->AddObject( uStyle, new MbWireFrame(*uAxis) );
    TestVariables::viewManager->AddObject( vStyle, new MbWireFrame(*vAxis) );
    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Проверить точность полигона кривой.
// Check curve polygon accuracy.
// ---
void CheckCurvePolygonAccuracy()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) ) {
    SPtr<const MbCurve3D> curve( ::GetCurve3D( obj, true ) );

    if ( curve != nullptr ) {
      double sag = Math::visualSag;

      MbRect1D range;
      MbCube cube;
      curve->AddYourGabaritTo( cube );
      double minSag = METRIC_NEAR;
      double maxSag = 0.5 * cube.GetDiagonal();
      maxSag = std_max( maxSag, METRIC_NEAR );
      range.Init( minSag, maxSag );

      sag = std_max( sag, minSag );
      sag = std_min( sag, maxSag );
      if ( GetDouble( IDS_ENTER_NUMBER, sag, &range ) ) {
        sag = std_max( sag, minSag );
        sag = std_min( sag, maxSag );

        std::vector< std::pair<double,MbCartPoint3D> > paramPoints;
        MbStepData stepData( ist_SpaceStep, sag );
        ::CalculatePolygon( *curve, stepData, paramPoints );
        size_t cnt = paramPoints.size();

        if ( cnt > 1 ) {
          double dmin =  MB_MAXDOUBLE;
          double dmax = -MB_MAXDOUBLE;
          double dmid = 0.0;
          size_t wrkCnt = 0;
          size_t errCnt = 0;

          double t1 = UNDEFINED_DBL;
          double t2 = UNDEFINED_DBL;
          MbCartPoint3D pnt, pnt0, pnt1, pnt2;
          MbVector3D vect1, vect2;
          std::vector<double> darr;
          darr.reserve( cnt );

          t1 = paramPoints[0].first;
          pnt1 = paramPoints[0].second;

          size_t k;
          for ( k = 1; k < cnt; k++ ) {
            t2 = paramPoints[k].first;
            pnt2 = paramPoints[k].second;

            if ( t1 < t2 ) {
              double t = (t1 + t2) * 0.5;
              curve->PointOn( t, pnt );
              vect1.Init( pnt1, pnt2 );
              vect2.Init( pnt1, pnt );
              vect1.Normalize();
              double dcur = (vect1 | vect2).Length();
              darr.push_back( dcur );
              dmin = std_min( dcur, dmin );
              dmax = std_max( dcur, dmax );
              dmid += dcur;
              wrkCnt++;
            }
            else {
              errCnt++;
            }

            pnt1 = pnt2;
            t1 = t2;
          }

          PRECONDITION( errCnt < 1 );

          if ( (dmin < dmax) && (wrkCnt > 0) ) {
            dmid /= (double)wrkCnt;

            double dsigma = 0.0;
            for ( k = 0; k < wrkCnt; k++ ) {
              double dd = darr[k] - dmid;
              dsigma += dd*dd;
            }
            dsigma /= (double)wrkCnt;
            dsigma = ::sqrt( dsigma );

            float fsag = (float)sag;
            float fdmin = (float)dmin;
            float fdmax = (float)dmax;
            float fdmid = (float)dmid;
            float fdsigma = (float)dsigma;

            TCHAR buf[1024];
            size_t len = 0;
            len += (size_t)_sntprintf( buf+len, 1024-len, RT_TEST_RESULTS );
            len += (size_t)_sntprintf( buf+len, 1024-len, RT_INIT_DISTANCE, fsag );
            len += (size_t)_sntprintf( buf+len, 1024-len, _T("\n") );
            len += (size_t)_sntprintf( buf+len, 1024-len, RT_SAMPLES_COUNT1, LoUint32(wrkCnt) );
            len += (size_t)_sntprintf( buf+len, 1024-len, _T("\n") );
            len += (size_t)_sntprintf( buf+len, 1024-len, RT_MIN_DISTANCE, fdmin );
            len += (size_t)_sntprintf( buf+len, 1024-len, _T("\n") );
            len += (size_t)_sntprintf( buf+len, 1024-len, RT_MAX_DISTANCE, fdmax );
            len += (size_t)_sntprintf( buf+len, 1024-len, _T("\n") );
            len += (size_t)_sntprintf( buf+len, 1024-len, RT_MID_VALUE, fdmid );
            len += (size_t)_sntprintf( buf+len, 1024-len, _T("\n") );
            len += (size_t)_sntprintf( buf+len, 1024-len, RT_SIGMA_VALUE, fdsigma );
            len += (size_t)_sntprintf( buf+len, 1024-len, _T("\n") );
            ::MessageBoxEx( buf, IDS_RESULTS );
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Проверить касание кривых.
// Check curves tangent mating.
// ---
void CheckCurvesTangentMating()
{
  MbItem * obj1 = nullptr;
  MbItem * obj2 = nullptr;

  //MbTopItem * tItem2 = nullptr;
  const MbCurve3D * curve1 = nullptr;
  const MbCurve3D * curve2 = nullptr;

  { // Выбор первой кривой. // Choose a first curve.
    MbTopItem * tItem = nullptr;
    if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj1, &tItem ) && tItem != nullptr ) {
      if ( tItem->IsA() == tt_CurveEdge )
        curve1 = &((const MbCurveEdge *)tItem)->GetIntersectionCurve();
    }
    else if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj1 ) && (obj1->IsA() == st_SpaceInstance) ) {
      const MbSpaceItem * sItem = ((const MbSpaceInstance *)obj1)->GetSpaceItem(); // Дать трёхмерный геометрический объект. // Give a three-dimensional geometric object.
      if ( (sItem != nullptr) && (sItem->Family() == st_Curve3D) ) {
        curve1 = (const MbCurve3D *)sItem;
      }
    }
  }
  TestVariables::viewManager->ToggleObject( obj1 );

//#ifdef _DRAWGI
  TestVariables::drawImp->DrawItem( curve1, TRGB_TANGERINE );
//#endif

  { // Выбор второй кривой. // Choose a second curve.
    MbTopItem * tItem = nullptr;
    if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj2, &tItem ) && tItem != nullptr ) {
      if ( tItem->IsA() == tt_CurveEdge )
        curve2 = &((const MbCurveEdge *)tItem)->GetIntersectionCurve();
    }
    else if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj2 ) && (obj2->IsA() == st_SpaceInstance) ) {
      const MbSpaceItem * sItem = ((MbSpaceInstance *)obj2)->GetSpaceItem(); // Дать трёхмерный геометрический объект. // Give a three-dimensional geometric object.
      if ( (sItem != nullptr) && (sItem->Family() == st_Curve3D) ) {
        curve2 = (const MbCurve3D *)sItem;
      }
    }
  }
  if ( obj1 != obj2 )
    TestVariables::viewManager->ToggleObject( obj2 );

//#ifdef _DRAWGI
  TestVariables::drawImp->DrawItem( curve2, TRGB_AZURE );
//#endif

  if ( curve1 != nullptr && curve2 != nullptr ) {
    double dmin = MB_MAXDOUBLE;
    double t1 = UNDEFINED_DBL;
    double t2 = UNDEFINED_DBL;

    {
      double t1min = curve1->GetTMin();
      double t1max = curve1->GetTMax();
      MbCartPoint3D p1beg, p1end;
      curve1->_PointOn( t1min, p1beg );
      curve1->_PointOn( t1max, p1end );

      double t2min = curve2->GetTMin();
      double t2max = curve2->GetTMax();
      MbCartPoint3D p2beg, p2end;
      curve2->_PointOn( t2min, p2beg );
      curve2->_PointOn( t2max, p2end );

      SArray<double> dists ( 4, 1 );
      SArray<double> t1arr( 4, 1 );
      SArray<double> t2arr( 4, 1 );
      dists.Add( p1beg.DistanceToPoint( p2beg ) ); t1arr.Add( t1min ); t2arr.Add( t2min );
      dists.Add( p1beg.DistanceToPoint( p2end ) ); t1arr.Add( t1min ); t2arr.Add( t2max );
      dists.Add( p1end.DistanceToPoint( p2beg ) ); t1arr.Add( t1max ); t2arr.Add( t2min );
      dists.Add( p1end.DistanceToPoint( p2end ) ); t1arr.Add( t1max ); t2arr.Add( t2max );

      for ( size_t k = 0, cnt = dists.Count(); k < cnt; k++ ) {
        if ( dists[k] < dmin ) {
          dmin = dists[k];
          t1 = t1arr[k];
          t2 = t2arr[k];
        }
      }
    }

    if ( dmin < MB_MAXDOUBLE ) {
      if ( dmin > Math::metricPrecision )
      {
        MessageBoxEx( RT_CURVES_NOT_DOCKED, IDS_TITLE_WRN );
      }
//#ifdef _DRAWGI
      TestVariables::drawImp->DrawItem( curve1, TRGB_TANGERINE );
      TestVariables::drawImp->DrawItem( curve2, TRGB_AZURE );
//#endif

      MbCartPoint3D pnt1, pnt2;
      curve1->_PointOn( t1, pnt1 );
      curve2->_PointOn( t2, pnt2 );
//#ifdef _DRAWGI
      if ( dmin > Math::metricPrecision )
        TestVariables::drawImp->DrawLine( pnt1, pnt2, TRGB_BROWN );
      TestVariables::drawImp->DrawPoint( &pnt1, TRGB_LIGHTRED );
      TestVariables::drawImp->DrawPoint( &pnt2, TRGB_LIGHTBLUE );
//#endif
      MbVector3D tau1, tau2;
      curve1->_Tangent( t1, tau1 );
      curve2->_Tangent( t2, tau2 );

      if ( tau1 * tau2 < -ANGLE_EPSILON )
        tau2.Invert();

      double radAngle = tau1.Angle( tau2 );
      double degAngle = radAngle * (180.0 / M_PI);
      MbVector3D v1( tau1 );
      MbVector3D v2( tau2 );
      double l1 = curve1->GetLengthEvaluation();
      double l2 = curve2->GetLengthEvaluation();
      double ltau = DELTA_MOD * std_max( l1, l2 );
      v1 *= ltau;
      v2 *= ltau;

//#ifdef _DRAWGI
      TestVariables::drawImp->DrawLine( pnt1, v1, TRGB_LIGHTRED );
      TestVariables::drawImp->DrawLine( pnt2, v2, TRGB_LIGHTBLUE );
//#endif

      {
        TCHAR spaceMsg[512];
        size_t len = 0;
        len += (size_t)_sntprintf( spaceMsg+len, 512-len, RT_SPACE_MEASUREMENT );
        len += (size_t)_sntprintf( spaceMsg+len, 512-len, RT_DISTANCE_BETWEEN_POINTS, dmin );
        len += (size_t)_sntprintf( spaceMsg+len, 512-len, RT_ANGLE_BETWEEN_TANGENT, degAngle );
        if ( tau1.Colinear( tau2 ) )
          len += (size_t)_sntprintf( spaceMsg+len, 512-len, RT_TANGENT_COLLINEAR );
        else
          len += (size_t)_sntprintf( spaceMsg+len, 512-len, RT_TANGENT_NOT_COLLINEAR );
        if ( tau1.RoundColinear( tau2 ) )
          len += (size_t)_sntprintf( spaceMsg+len, 512-len, RT_TANGENT_ROUND_COLLINEAR );
        else
          len += (size_t)_sntprintf( spaceMsg+len, 512-len, RT_TANGENT_NOT_ROUND_COLLINEAR );

        MessageBoxEx( spaceMsg, IDS_INFORMATION );
      }

      // Проверка касания в проекциях. // Check touch in projections.
      bool isPlace = false;
      MbPlacement3D projPlace;
      {
        MbCurve * pCurve1 = nullptr;
        MbCurve * pCurve2 = nullptr;
        MbPlacement3D place1, place2;
        if ( curve1->GetPlaneCurve( pCurve1, place1, true ) && curve2->GetPlaneCurve( pCurve2, place2, true ) ) {
          if ( place1.Complanar( place2 ) ) {
            projPlace = place1;
            isPlace = true;
          }
        }
        ::DeleteItem( pCurve1 );
        ::DeleteItem( pCurve2 );

        if ( !isPlace ) {
          // TO_DO: Выбрать плоскость для проецирования. // TO_DO Choose a plane for projection.
        }
      }
      if ( isPlace ) {
        MbMatrix3D projMatr;
        projPlace.GetMatrixInto( projMatr );
        MbCurve * mapCurve1 = curve1->GetMap( projMatr, nullptr );
        MbCurve * mapCurve2 = curve2->GetMap( projMatr, nullptr );
        //MbCurve * mapCurve1 = curve1->GetProjection( projPlace, Math::DefaultVersion() );
        //MbCurve * mapCurve2 = curve2->GetProjection( projPlace, Math::DefaultVersion() );

        ::AddRefItem( mapCurve1 );
        ::AddRefItem( mapCurve2 );

        if ( mapCurve1 != nullptr && mapCurve2 != nullptr ) {
          MbVector mapTau1, mapTau2;
          MbPlaneCurve planeCurve1( projPlace, *mapCurve1, true );
          MbPlaneCurve planeCurve2( projPlace, *mapCurve2, true );
          { // Тест проекций. // Projection test.
            bool wrongProjDir = false;
            MbVector3D sTau, pTau;

            curve1->GetLimitTangent( 1, sTau );
            planeCurve1.GetLimitTangent( 1, pTau );
            if ( sTau * pTau < -ANGLE_EPSILON )
              wrongProjDir = true;

            curve2->GetLimitTangent( 1, sTau );
            planeCurve2.GetLimitTangent( 1, pTau );
            if ( sTau * pTau < -ANGLE_EPSILON )
              wrongProjDir = true;

            if ( wrongProjDir ) {
              MessageBoxEx( RT_PROJ_DIRECTION_INVERTED, IDS_TITLE_ERR );
            }
          }

//#ifdef _DRAWGI
          TestVariables::drawImp->DrawItem( &planeCurve1, TRGB_ORANGE );
          TestVariables::drawImp->DrawItem( &planeCurve2, TRGB_LIGHTCYAN );
//#endif
          mapCurve1->_Tangent( t1, mapTau1 );
          mapCurve2->_Tangent( t2, mapTau2 );
          if ( mapTau1 * mapTau2 < -ANGLE_EPSILON )
            mapTau2.Invert();

          bool isC1 = mapTau1.Colinear( mapTau2 );
          bool isC2 = mapTau1.RoundColinear( mapTau2 );
          double degMapAngle = ::AngleBetweenVectors( mapTau1, mapTau2 ) * (180.0 / M_PI);

          TCHAR mapMsg[512];
          size_t len = 0;
          len += (size_t)_sntprintf( mapMsg+len, 512-len, RT_PROJ_MEASUREMENTS );
          len += (size_t)_sntprintf( mapMsg+len, 512-len, RT_DISTANCE_BETWEEN_POINTS, dmin );
          len += (size_t)_sntprintf( mapMsg+len, 512-len, RT_ANGLE_BETWEEN_TANGENT, degMapAngle );

          if ( isC1 ) len += (size_t)_sntprintf( mapMsg+len, 512-len, RT_TANGENT_COLLINEAR );
          else        len += (size_t)_sntprintf( mapMsg+len, 512-len, RT_TANGENT_NOT_COLLINEAR );

          if ( isC2 ) len += (size_t)_sntprintf( mapMsg+len, 512-len, RT_TANGENT_ROUND_COLLINEAR );
          else        len += (size_t)_sntprintf( mapMsg+len, 512-len, RT_TANGENT_NOT_ROUND_COLLINEAR );

          MessageBoxEx( mapMsg, IDS_INFORMATION );
        }

        ::ReleaseItem( mapCurve1 );
        ::ReleaseItem( mapCurve2 );
      }
    }
  }

  if ( obj1 != obj2 )
    TestVariables::viewManager->ToggleObject( obj2 );
  TestVariables::viewManager->ToggleObject( obj1 );
}


//------------------------------------------------------------------------------
// Удалить тело, оставив базовые объекты.
// Remove the solid, leaving the base objects.
// ---
bool RemoveSolidBasis()
{
  MbItem * nearest = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &nearest ) ) {
    TestVariables::viewManager->ToggleObject(nearest);
    SetWaitCursor( true );

    RPArray<MbSpaceItem> s( 0, 1 );
    ((MbSolid *)nearest)->GetBasisItems( s );
    ::AddRefItems( s );

    ptrdiff_t n = s.Count();
    TestVariables::viewManager->ToggleObject(nearest);

    if ( n > 0 ) {
      Style ns( nearest->GetWidth(), nearest->GetColor() );
      TestVariables::viewManager->DeleteObject( nearest ); // Удаление элемента.  // Delete element.
      for ( ptrdiff_t i = 0; i < n; i++ ) {
        Style st = ns;
        switch ( i ) {
          case 0  : st = ns; break;
          case 1  : st = TestVariables::ELEMENTARY_Style; break;
          case 2  : st = TestVariables::BOOLEAN_Style; break;
          default : st = TestVariables::SOLID_Style; break;
        }
        TestVariables::viewManager->AddObject( st, s[i] );
      }
    }

    ::ReleaseItems( s );

    SetWaitCursor( false );

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Номер ребра в грани с указанием номера цикла.
// Get edge number in the face indicating the loop number.
// ---
bool GetEdgeNumberInFace()
{
  MbItem * edgeObj = nullptr;
  MbItem * faceObj = nullptr;
  MbTopItem * edgeItem = nullptr;
  MbTopItem * faceItem = nullptr;

  if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &edgeObj, &edgeItem ) && edgeItem->IsAnEdge() ) {
    TestVariables::drawImp->DrawItem( edgeItem, TRGB_WHITE );

    if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &faceObj, &faceItem ) && faceItem->IsAFace() ) {
      TestVariables::drawImp->DrawItem( faceItem, TRGB_WHITE );

      if ( edgeObj == faceObj ) {
        MbCurveEdge * edge = static_cast<MbCurveEdge *>(edgeItem);
        MbFace      * face = static_cast<MbFace *>(faceItem);

        size_t lInd = SYS_MAX_T;
        size_t eInd = SYS_MAX_T;

        if ( face->FindEdgeIndex( *edge, true, lInd, eInd ) || face->FindEdgeIndex( *edge, false, lInd, eInd ) ) {
          TCHAR s[210];
          size_t len = 0;
          len += _sntprintf( s+len, 210-len, RT_LOOP_NUMBER_IN_FACE F_TD _T("\n"), lInd );  //-V111
          len += _sntprintf( s+len, 210-len, RT_EDGE_NUMBER_IN_LOOP F_TD _T("\n"), eInd );  //-V111
          MessageBoxEx( s, IDS_RESULT );
          return true;
        }
        else {
          MessageBoxEx( RT_EDGE_NOT_BELONG_FACE, IDS_TITLE_ERR );
        }
      }
      else {
        MessageBoxEx( RT_EDGE_AND_FACES_FROM_DIFF_SHELLS, IDS_TITLE_ERR );
      }
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Определение номера грани.
// Face number definition.
// ---
bool GetFaceNumber()
{
  MbItem * obj = nullptr;
  MbTopItem * surface = nullptr;

  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &surface ) ) {
    MbSolid * solid = (MbSolid *)obj;
    MbFace * face = (MbFace *)surface;
    RPArray<MbFace> faces( 1, 1 );
    SArray<size_t> faceIndex(1,1);

    TestVariables::drawImp->DrawItem( face, TRGB_WHITE );

    faces.Add(face);
    if ( face != nullptr && solid != nullptr &&
         solid->FindFacesIndexByFaces( faces, faceIndex ) &&
         faceIndex.Count() > 0 )
    {
      TCHAR s[210];
      _sntprintf( s, 210, IDS_FACE_NUMBER_IN_SOLID F_TD, faceIndex[0] );  //-V111
      MessageBoxEx( s, IDS_FACE_NUMBER_IN_SOLID );

      return true;
    }
  }
  return false;
}


//------------------------------------------------------------------------------
// Определение типа грани скругления.
// Fillet face type definition.
// ---
bool GetFaceFilletType()
{
  MbItem * obj1 = nullptr;
  MbTopItem * obj2 = nullptr;

  if ( GetTObj(IDS_SELECT_FACE, st_Solid, tt_Face, &obj1, &obj2) ) {
    if ( obj1 != nullptr && obj2 != nullptr ) {
      MbSolid * solid = static_cast<MbSolid *>( obj1 );
      MbFace * face = static_cast<MbFace *>( obj2 );    
      if ( face != nullptr && solid != nullptr  ) {
        // MbeFilletType type;
        TestVariables::drawImp->DrawItem( face, TRGB_WHITE );
        TCHAR s[210];
        if ( ::IsFilletFace(*face, Math::metricAccuracy) )
          _sntprintf( s, 210, RT_FILLET );
        else
          _sntprintf( s, 210, RT_NOT_FILLET );
        
       /* switch ( type )
        { 
          case ft_NotFillet:
           _sntprintf( s, 210, RT_NOT_FILLET );
           break;
          case ft_ConstRadiusFillet:
            _sntprintf( s, 210, RT_CONST_RADIUS_FILLET );
            break;
          case ft_CornerFillet :
            _sntprintf( s, 210, RT_CORNER_FILLET );
            break;
          case ft_VariableRadiusFillet:
            _sntprintf( s, 210, RT_VARIABLE_RADIUS_FILLET );
            break;
          case ft_NotRecognized:
            _sntprintf( s, 210, RT_NOT_RECOGNIZED );
            break;
          default:
            break;
        } */    
        MessageBoxEx( s, IDS_FACE_FILLET_TYPE );
        return true;
      }
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Определение номера ребра.
// Edge number definition.
// ---
bool GetEdgeNumber()
{
  MbItem * obj = nullptr;
  MbTopItem * curve = nullptr;

  if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &curve ) ) {
    MbSolid * solid = static_cast<MbSolid *>(obj);
    MbCurveEdge * edge = static_cast<MbCurveEdge *>(curve);

    RPArray<MbCurveEdge> initEdges( 1, 1 );

    SArray<MbEdgeFacesIndexes> indexes( 1, 1 );

    TestVariables::drawImp->DrawItem( edge, TRGB_WHITE );
    TestVariables::drawImp->DrawItem( &edge->GetEndVertex(), TRGB_DARKGRAY );

    const MbFaceShell* shell = solid->GetShell();

    initEdges.Add(edge);
    if ( edge != nullptr && solid != nullptr &&
         shell->FindFacesIndexByEdges( initEdges, indexes, true ) &&
         indexes.Count() > 0 )
    {
      const size_t bufLen = 1024;
      TCHAR s[bufLen];
      size_t len = 0;
      len += ( size_t ) _sntprintf( s + len, bufLen - len, IDS_EDGE_NUMBER_IN_SOLID F_TD, indexes[0].edgeIndex );
      if ( indexes[0].facePIndex == SYS_MAX_T || indexes[0].faceMIndex == SYS_MAX_T ) {
        len += ( size_t ) _sntprintf( s + len, bufLen - len, RT_BOUNDARY_EDGE );
      }
      _sntprintf( s + len, bufLen - len, RT_FACE_PLUS_NUMBER F_TD RT_FACE_MINUS_NUMBER F_TD, indexes[0].facePIndex, indexes[0].faceMIndex );  //-V111
      MessageBoxEx( s, IDS_EDGE_NUMBER_IN_SOLID );

      c3d::ConstEdgesVector edges( initEdges.begin(), initEdges.end() );
      ::CheckBadEdges( edges, nullptr );

      return true;
    }
  }
  return false;
}


//------------------------------------------------------------------------------
// Определение грани по номеру.
// Define face by number.
// ---
bool GetFaceByNumber()
{
  MbItem * obj = nullptr;
  ptrdiff_t number = 0;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) &&
       GetInt( IDS_ENTER_NUMBER_N, number ) ) {
    MbSolid * solid = static_cast<MbSolid *>(obj);

    if ( solid != nullptr && solid->GetShell() != nullptr ) {
      MbFace * face = solid->GetShell()->GetFace(number);
      TestVariables::drawImp->DrawItem( face, TRGB_WHITE );
      TestVariables::drawImp->DrawItem( face, TRGB_BLACK );
      TestVariables::drawImp->DrawItem( face, TRGB_PURPLE );

      TCHAR s[256];
      _sntprintf( s, 256, IDS_FACE_NUMBER_IN_SOLID F_TD, number ); //-V111
      MessageBoxEx( s, IDS_FACE_NUMBER_IN_SOLID );

      return true;
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Определение ребра по номеру.
// Define edge by number.
// ---
bool GetEdgeByNumber()
{
  MbItem * obj = nullptr;
  ptrdiff_t number = 0;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) &&
       GetInt( IDS_ENTER_NUMBER_N, number ) ) {
    MbSolid * solid = static_cast<MbSolid *>(obj);

    if ( solid!=nullptr && solid->GetShell()!=nullptr ) {
      MbFaceShell *shell = solid->GetShell();
      MbCurveEdge *edge = shell->GetEdge( number );
      size_t ind1 = SYS_MAX_T, ind2 = SYS_MAX_T;
      if ( edge != nullptr )
        shell->FindFacesIndexByEdge( *edge, ind1, ind2 );

      const MbFace * faceP = shell->GetFace(ind1);
      const MbFace * faceM = shell->GetFace(ind2);
      TestVariables::drawImp->DrawItem( faceP, 255, 125, 255 );
      TestVariables::drawImp->DrawItem( faceM, 255, 255, 125 );
      TestVariables::drawImp->DrawItem( edge, TRGB_WHITE );
      TestVariables::drawImp->DrawItem( edge, TRGB_BLACK );
      TestVariables::drawImp->DrawEdge( edge, TRGB_LIGHTRED, true );

      const size_t bufLen = 1024;
      TCHAR s[bufLen];
      size_t len = 0;
      len += ( size_t ) _sntprintf( s + len, bufLen - len, IDS_EDGE_NUMBER_IN_SOLID F_TD, number );
      if ( faceP == nullptr || faceM == nullptr ) {
        len += ( size_t ) _sntprintf( s + len, bufLen - len, RT_BOUNDARY_EDGE );
      }
      _sntprintf( s + len, bufLen - len, RT_FACE_PLUS_NUMBER F_TD RT_FACE_MINUS_NUMBER F_TD, ind1, ind2 ); //-V111
      MessageBoxEx( s, IDS_EDGE_NUMBER_IN_SOLID );

      static bool addEdgeCurve = false;
      if ( addEdgeCurve ) {
        SPtr<MbCurve3D> edgeCurve( static_cast<MbCurve3D *>(&edge->GetCurve().Duplicate()) );
        TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, edgeCurve );
      }

      return true;
    }
  }
  return false;
}


//------------------------------------------------------------------------------
// Распознование граней скругления.
// Recognition fillet faces. 
// ---
bool RecognitionFilletFaces()
{
  MbItem * obj = nullptr;
  
  if ( GetGObj(IDS_SELECT_SOLID, st_Solid, &obj) ) {
    if ( obj != nullptr ) {
      MbSolid * solid = static_cast<MbSolid *>( obj );
      if ( solid != nullptr && solid->GetShell() != nullptr ) {
        MbFaceShell * shell = solid->GetShell();
        RPArray<MbFace> faces;
        RPArray<MbFace> filletFaces;
        shell->GetFaces( faces );
        ::FindFilletFaces( faces, Math::metricAccuracy, filletFaces );
        for ( size_t i = 0, size = filletFaces.size(); i < size; ++i ) {
          MbFace * face = filletFaces[i];
          TestVariables::drawImp->DrawItem( face, TRGB_PURPLE );
        }
        TCHAR s[256];
        if ( filletFaces.size() == 0 ) {
          _sntprintf( s, 256, _T("No fillets in the solid !") ); 
          MessageBoxEx( s, _T("") );
        }
        return true;
      }
    }
  }

  return false;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
void FindByName( const RPArray<Type> & items, const MbName & name, RPArray<Type> & resItems )
{
  for( size_t k = 0, cnt = items.Count(); k < cnt; k++ ) {
    Type * item = items[k];
    if ( item != nullptr ) {
      const MbName & itemName = item->GetName();
      if ( itemName == name ) {
        resItems.Add( item );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Выполнить именование оболочки
// Perform shell naming
// ---
void SetShellNames()
{
  SolidSPtr solid( ::SelectSolid() );

  if ( solid != nullptr && solid->GetFacesCount() > 0 ) {
    MbFaceShell * shell = solid->GetShell();

    bool resetAllNames = GetBool( IDS_BOOL_RESET_NAMES, false );

    std::set<SimpleName> usedMainNames;
    if ( !resetAllNames ) {
      SetWaitCursor( true );
      size_t i;
      ConstFacesVector faces;
      shell->GetFaces( faces );
      for ( i = faces.size(); i--; ) {
        const MbFace * face = faces[i];
        if ( face != nullptr )
          usedMainNames.insert( face->GetName().GetMainName() );
      }
      ConstEdgesVector edges;
      shell->GetEdges( edges );
      for ( i = edges.size(); i--; ) {
        const MbCurveEdge * edge = edges[i];
        if ( edge != nullptr )
          usedMainNames.insert( edge->GetName().GetMainName() );
      }
      ConstVerticesVector vertices;
      shell->GetVertices( vertices );
      for ( i = vertices.size(); i--; ) {
        const MbVertex * vertex = vertices[i];
        if ( vertex != nullptr )
          usedMainNames.insert( vertex->GetName().GetMainName() );
      }
      SetWaitCursor( false );
    }

    SimpleName newMainName( 1 );
    ptrdiff_t newId = ptrdiff_t(newMainName);

    if ( !usedMainNames.empty() ) {
      size_t iterCnt = 0;
      size_t usedCnt = usedMainNames.size();

      while ( (iterCnt <= usedCnt) && (usedMainNames.find( newMainName ) != usedMainNames.end()) ) {
        newMainName++;
        iterCnt++;
      }
      C3D_ASSERT( usedMainNames.find( newMainName ) == usedMainNames.end() );
      newId = ptrdiff_t(newMainName);
    }

    GetInt( IDS_ENTER_NUMBER, newId );
    newMainName = SimpleName( newId );

    SetWaitCursor( true );

    MbSNameMaker nameMaker( newMainName );
    if ( ::SetItemNames( *solid, nameMaker, resetAllNames ) ) {
      SetWaitCursor( false );    
      ::MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );
    }

    SetWaitCursor( false );    
  }
}

//------------------------------------------------------------------------------
// Проверить имена на совпадение.
// Check shell names for a match.
// ---
void CheckShellNames()
{
  bool sameNames = false;

  ItemsSPtrVector items;
  {
    MbItem * item = nullptr;
    PathMatrix pathFrom;
    while ( GetGObj( IDS_SELECT_SOLID, st_Solid, &item, pathFrom ) ) {
      TestVariables::viewManager->ToggleObject( item );
      items.push_back( ItemSPtr( static_cast<MbItem *>( &item->Duplicate() ) ) );
      if ( !pathFrom.second.IsSingle() )
        items.back()->Transform( pathFrom.second );
    }
  }

  SetWaitCursor( true );

  const size_t itemsCnt = items.size();

  static bool collectItems = true;

  SArray<NameIntersectionInfo> infos( itemsCnt, 1 );
  RPArray<const MbFaceShell>   shells( itemsCnt, 1 );

  RPArray<MbFace>      shellFaces( itemsCnt, 1 );
  RPArray<MbCurveEdge> shellEdges( itemsCnt, 1 );
  RPArray<MbVertex>    shellVertices( itemsCnt, 1 );
  
  for ( size_t k = 0; k < itemsCnt; ++k ) {
    const MbSolid * solid = &static_cast<MbSolid &>( *items[k] );
    if ( solid ) {
      const MbFaceShell * shell = solid->GetShell();
      if ( shell != nullptr ) {
        shell->GetFaces( shellFaces );
        shell->GetItems( shellVertices, shellEdges );
        shells.push_back( shell );
      }
    }
  }

  if ( !::CheckShellNames( shells, infos ) ) { // Проверить имена на совпадение. // Check names for a match.
    sameNames = true;

    if ( collectItems ) {
      RPArray<MbFace>      faces( 0, 1 );
      RPArray<MbCurveEdge> edges( 0, 1 );
      RPArray<MbVertex>    vertices( 0, 1 );

      size_t l, namesCnt = infos.Count();

      for ( l = 0; l < namesCnt; l++ ) {
        const MbName * name = infos[l].name;
        if ( name == nullptr )
          continue;

        size_t facesCnt0 = faces.Count();
        size_t edgesCnt0 = edges.Count();
        size_t verticesCnt0 = vertices.Count();
        size_t m;

        for ( m = 0; m < facesCnt0; ++m ) {
          TestVariables::drawImp->DrawItem( faces[m], TRGB_LIGHTGREEN );
        }
        for ( m = 0; m < edgesCnt0; ++m ) {
          TestVariables::drawImp->DrawItem( edges[m], TRGB_LIGHTGREEN );
        }
        for ( m = 0; m < verticesCnt0; ++m ) {
          TestVariables::drawImp->DrawItem( vertices[m], TRGB_LIGHTGREEN );
        }

        ::FindByName( shellFaces,    *name, faces );
        ::FindByName( shellEdges,    *name, edges );
        ::FindByName( shellVertices, *name, vertices );

        size_t facesCnt = faces.Count();
        size_t edgesCnt = edges.Count();
        size_t verticesCnt = vertices.Count();

        for ( m = facesCnt0; m < facesCnt; ++m ) {
          TestVariables::drawImp->DrawItem( faces[m], TRGB_YELLOW );
          TestVariables::drawImp->DrawItem( faces[m], TRGB_AZURE );
        }
        for ( m = edgesCnt0; m < edgesCnt; ++m ) {
          TestVariables::drawImp->DrawItem( edges[m], TRGB_YELLOW );
          TestVariables::drawImp->DrawItem( edges[m], TRGB_BLUE );
        }
        for ( m = verticesCnt0; m < verticesCnt; ++m ) {
          TestVariables::drawImp->DrawItem( vertices[m], TRGB_YELLOW );
          TestVariables::drawImp->DrawItem( vertices[m], TRGB_BLACK );
        }
      }
    }
  }

  SetWaitCursor( false );

  if ( sameNames )
    ::MessageBoxEx( RT_SAME_NAMES, IDS_TITLE_WRN );
  else
    ::MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );
}


//------------------------------------------------------------------------------
// Проверить имена на совпадение.
// Check point frame names for a match.
// ---
void CheckPointFrameNames()
{
  bool sameNames = false;

  PointFrameSPtr pointFrame;

  PathMatrix pathFrom;
  MbItem * item = nullptr;
  if ( GetGObj( IDS_SELECT_POINT, st_Point3D, &item, pathFrom ) ) {
    TestVariables::viewManager->ToggleObject( pathFrom.first );
    if ( item->IsA() == st_PointFrame ) {
      pointFrame = static_cast<MbPointFrame *>(item);

      SetWaitCursor( true );

      ConstVerticesSPtrVector vertices;
      pointFrame->GetVertices( vertices );

      size_t verticesCnt = vertices.size();

      std::vector<SimpleName> hashes;
      {
        hashes.reserve( verticesCnt );
        for ( size_t k = 0; k < verticesCnt; ++k ) {
          hashes.push_back( vertices[k]->GetNameHash() );
        }
      }
      c3d::UniqueSortVector( hashes );
      if ( hashes.size() < verticesCnt )
        sameNames = true;

      SetWaitCursor( false );
    }
  }

  if ( sameNames )
    ::MessageBoxEx( RT_SAME_NAMES, IDS_TITLE_WRN );
  else
    ::MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );
}


//------------------------------------------------------------------------------
// Проверить имена на совпадение.
// Check point frame names for a match.
// ---
void CheckWireFrameNames()
{
  bool sameNames = false;

  WireFrameSPtr wireFrame;

  PathMatrix pathFrom;
  MbItem * item = nullptr;
  if ( GetGObj( IDS_SELECT_POINT, st_Curve3D, &item, pathFrom ) ) {
    TestVariables::viewManager->ToggleObject( pathFrom.first );
    if ( item->IsA() == st_WireFrame ) {
      wireFrame = static_cast<MbWireFrame *>(item);

      SetWaitCursor( true );

      ConstWireEdgesSPtrVector edges;
      wireFrame->GetEdges( edges );

      ConstVerticesSPtrSet vertices;
      wireFrame->GetVerticesSet( vertices );

      size_t edgesCnt = edges.size();
      size_t verticesCnt = vertices.size();

      std::vector<SimpleName> hashes;
      {
        hashes.reserve( edgesCnt + verticesCnt );
        size_t k;
        for ( k = 0; k < edgesCnt; ++k )
          hashes.push_back( edges[k]->GetNameHash() );

        ConstVerticesSPtrSetIt it = vertices.begin();
        while ( it != vertices.end() ) {
          hashes.push_back( (*it)->GetNameHash() );
          ++it;
        }
      }
      c3d::UniqueSortVector( hashes );
      if ( hashes.size() < edgesCnt + verticesCnt )
        sameNames = true;

      SetWaitCursor( false );
    }
  }

  if ( sameNames )
    ::MessageBoxEx( RT_SAME_NAMES, IDS_TITLE_WRN );
  else
    ::MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );
}


//------------------------------------------------------------------------------
// Проверить производные поверхности.
// Check surface derivatives.
// ---
bool CheckSurface()
{
  bool bRes = false;
  MbItem * obj = nullptr;
  MbTopItem * top( nullptr );

  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &top, TestVariables::itemPathFrom.second ) ) {
    WorkWindow * workwin = TestVariables::viewManager->GetActiveWindow();
    if ( workwin != nullptr ) {
      MbCartPoint3D p( TestVariables::cursorPoint );
      const MbVector3D & aZ = workwin->GetAxisZ();
      const MbSurface & surface = ((MbFace *)top)->SetSurface();
      double u, v;

      bRes = surface.NearDirectPointProjection( p, aZ, u, v, true, nullptr );

      if ( !bRes ) {
        u = surface.GetUMid();
        v = surface.GetVMid();
        bRes = true;
      }

      if ( bRes ) {
        MbCartPoint3D pnt;
        MbVector3D uDer, vDer, uuDer, vvDer, uvDer;
        double eps = PARAM_PRECISION;

        surface.Explore( u, v, true, pnt, uDer, vDer, &uuDer, &vvDer, &uvDer, nullptr );
/*
        MbCartPoint3D pnt_; 
        MbVector3D uDer_, vDer_, uuDer_, vvDer_, uvDer_;

        surface._PointOn( u, v, pnt_ );
        surface._DeriveU( u, v, uDer_ );
        surface._DeriveV( u, v, vDer_ );
        surface._DeriveUU( u, v, uuDer_ );
        surface._DeriveVV( u, v, vvDer_ );

        if ( !pnt_.IsSame(pnt, eps) ) {
          ::MessageBoxEx( IDS_FAILED, _T("point") );
        }
        if ( !uDer_.IsSame(uDer, eps) ) {
          ::MessageBoxEx( IDS_FAILED, _T("uDer") );
        }
        if ( !vDer_.IsSame(vDer, eps) ) {
          ::MessageBoxEx( IDS_FAILED, _T("vDer") );
        }
        if ( !uuDer_.IsSame(uuDer, eps) ) {
          ::MessageBoxEx( IDS_FAILED, _T("uuDer") );
        }
        if ( !vvDer_.IsSame(vvDer, eps) ) {
          ::MessageBoxEx( IDS_FAILED, _T("vvDer") );
        }
*/
        MbCartPoint3D pU1, pU2, pV1, pV2, p11, p12, p21, p22;
        double du = eps * surface.GetURange();
        double dv = eps * surface.GetVRange();

        surface._PointOn( u-du, v, pU1 );
        surface._PointOn( u+du, v, pU2 );
        surface._PointOn( u, v-dv, pV1 );
        surface._PointOn( u, v+dv, pV2 );
        surface._PointOn( u-du, v-dv, p11 );
        surface._PointOn( u-du, v+dv, p12 );
        surface._PointOn( u+du, v-dv, p21 );
        surface._PointOn( u+du, v+dv, p22 );

        MbVector3D derU( pU1, pU2 ), derV( pV1, pV2 ), derUU, derVV, derUV;
        derUU.Set( pU1, 1.0, pU2, 1.0, pnt, -2.0 );
        derVV.Set( pV1, 1.0, pV2, 1.0, pnt, -2.0 );
        derU /= 2.0 * du;
        derV /= 2.0 * dv;
        derUU /= du * du;
        derVV /= dv * dv;
        derUV.Set( p22, 1.0, p12, -1.0, p21, -1.0, p11, 1.0 );
        derUV /= 4.0 * du * dv;

//#ifdef _DRAWGI
        TestVariables::drawImp->DrawPoint( &pnt, TRGB_GREEN );
        GetPoint( IDS_SELECT_POINT, p );

        TestVariables::drawImp->DrawLine( pnt, derU, TRGB_RED );
        TestVariables::drawImp->DrawLine( pnt, uDer, TRGB_OLIVE );
        GetPoint( IDS_SELECT_FIRST_POINT, p );
        TestVariables::drawImp->DrawLine( pnt, uDer, TRGB_WHITE );
        TestVariables::drawImp->DrawLine( pnt, derU, TRGB_WHITE );

        TestVariables::drawImp->DrawLine( pnt, derV, TRGB_RED );
        TestVariables::drawImp->DrawLine( pnt, vDer, TRGB_OLIVE );
        GetPoint( IDS_SELECT_SECOND_POINT, p );
        TestVariables::drawImp->DrawLine( pnt, vDer, TRGB_WHITE );
        TestVariables::drawImp->DrawLine( pnt, derV, TRGB_WHITE );

        TestVariables::drawImp->DrawLine( pnt, derUU, TRGB_RED );
        TestVariables::drawImp->DrawLine( pnt, uuDer, TRGB_OLIVE );
        GetPoint( IDS_SELECT_FIRST_CURVE, p );
        TestVariables::drawImp->DrawLine( pnt, uuDer, TRGB_WHITE );
        TestVariables::drawImp->DrawLine( pnt, derUU, TRGB_WHITE );

        TestVariables::drawImp->DrawLine( pnt, derVV, TRGB_RED );
        TestVariables::drawImp->DrawLine( pnt, vvDer, TRGB_OLIVE );
        GetPoint( IDS_SELECT_SECOND_CURVE, p );
        TestVariables::drawImp->DrawLine( pnt, vvDer, TRGB_WHITE );
        TestVariables::drawImp->DrawLine( pnt, derVV, TRGB_WHITE );

        TestVariables::drawImp->DrawLine( pnt, derUV, TRGB_RED );
        TestVariables::drawImp->DrawLine( pnt, uvDer, TRGB_OLIVE );
//#endif // _DRAWGI

      }
    }
  }
  return bRes;
}


//------------------------------------------------------------------------------
// Функция построения множества времён при перестроении тела.
// The function of constructing the set of times when the solid rebuild.
// ---
static bool SolidRebuildFunctions( const MbSolid &solid, MbeCopyMode sameShell,
                                   SArray<double> & timeList, SArray<ptrdiff_t> & faceList )
{
  PArray<MbCreator> creators(0,1,false);

  if ( solid.GetCreators( creators ) ) {

    MbFaceShell * nShell = nullptr;
    MbCreator * creator;
    bool res = true;
    size_t cCount = creators.Count();
    timeList.Reserve(cCount);
    faceList.Reserve(cCount);

    uint begSec = ::GetTickCountEx();

    for ( size_t i = 0; i < cCount; i++ ) {
      creator = creators[i];
      if ( creator!=nullptr ) {
        if ( !creator->CreateShell( nShell, sameShell ) ) {
          res = false;
          if ( sameShell==cm_Same ) { // Оболочка не копировалась и испорчена. // The shell was not copied and spoiled.
            ::DeleteItem( nShell );
            break;
          }
        }
        if ( nShell!=nullptr ) {
          nShell->SetOwnChangedThrough( tct_Unchanged );
          uint miliSec = ::GetTickCountEx();
          ptrdiff_t facesCount = nShell->GetFacesCount();
          timeList.Add( 0.001 * (double)(miliSec-begSec) );
          faceList.Add( facesCount );
          begSec = miliSec;
        }
      }
    }

    if ( !res || nShell==nullptr ) {
      MessageBoxEx( RT_OBJECT_BUILD_NOT_COMPLETELY, IDS_TITLE_ERR );
    }
    if ( nShell!=nullptr ) {
      ::DeleteItem( nShell );
    }

    return res;
  }
  return false;
}


//------------------------------------------------------------------------------
// Функция определения максимумов.
// The function of maximums definition.
// ---
static void FindMaximum( SArray <double> &timeList, SArray <ptrdiff_t> &faceList,
                         double &tMax, ptrdiff_t &fMax )
{
  tMax = 0;
  fMax = 0;

  size_t tCount = timeList.Count();

  if ( tCount > 0 && tCount == faceList.Count() ) {
    for ( ptrdiff_t i = 0; i < (ptrdiff_t)tCount; i++ ) {
      tMax = std_max(tMax,fabs(timeList[i]));
      fMax = std_max(fMax, ::abs_t(faceList[i]));
    }
  }
}


//------------------------------------------------------------------------------
// Функция определения максимумов.
// The function of maximums definition.
// ---
static void DrawAxises( double xSize, double xMax, double xKoef,
                        double ySize, double yMax, double yKoef )
{
  MbCartPoint3D pnt, pnt0(0,0,0);
  pnt.Init(xSize,0,0);
  TestVariables::viewManager->AddObject( TestVariables::TEXT_Style, new MbLineSegment3D(pnt0,pnt) );
  pnt.Init(0,ySize,0);
  TestVariables::viewManager->AddObject( TestVariables::TEXT_Style, new MbLineSegment3D(pnt0,pnt) );

  double xStep = 5; // Операций. // Operations.
  ptrdiff_t i = 1;
  while ( xStep * (double)i < xMax ) {
    TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, new MbPointFrame(xStep * (double)i * xKoef,-0.5,0) );
    i++;
  }
  double yStep = 5; // Секунд. // Seconds.
  i = 1;
  while ( yStep * (double)i < yMax ) {
    TestVariables::viewManager->AddObject( TestVariables::PPOINT_Style, new MbPointFrame(-0.5,yStep * (double)i * yKoef,0) );
    i++;
  }
}


//------------------------------------------------------------------------------
// Перестроить тело и засечь время.
// Rebuild solid and detect the time.
// ---
bool SolidRebuildTimeFunction() {

  MbItem *obj;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {
    TestVariables::viewManager->EraseObject(obj);
    MbSolid *solid = (MbSolid *)obj;
    if ( solid != nullptr ) {

      //size_t hWin = activeHWND;
      //activeHWND = 0; // Отключение отрисовки
      //enableDraw = false;

      // Отключение отрисовки. // Disable drawing.
      PRECONDITION( TestVariables::viewManager );
      bool save = TestVariables::viewManager->IsEnableDrawToActiveWindow();
      TestVariables::viewManager->SetEnableDrawToActiveWindow( false );

      //MbeCopyMode sameShell = cm_Same;
      MbeCopyMode sameShell = cm_KeepHistory;
      //MbeCopyMode sameShell = cm_KeepSurface;
      //MbeCopyMode sameShell = cm_Copy;
      SArray <double> timeList(0,1);
      SArray <ptrdiff_t> faceList(0,1);

      SetWaitCursor( true );

      bool rres = ::SolidRebuildFunctions( *solid, sameShell, timeList, faceList );

      SetWaitCursor( false );

      if ( rres ) {
        double tMax = UNDEFINED_DBL;
        ptrdiff_t fMax = 0;

        FindMaximum( timeList, faceList, tMax, fMax );

        ptrdiff_t tCount = timeList.Count();
        if ( tCount > 1 && tCount == (ptrdiff_t)faceList.Count() ) {
          SArray<MbCartPoint3D> pList1(tCount,2);
          SArray<MbCartPoint3D> pList2(tCount,2);

          double xSize = 300.0, ySize = 200.0;
          double xKoef = xSize / (double)(tCount-1);
          double tKoef = (tMax!=0) ? (ySize / tMax) : 1.0;
          double fKoef = (fMax!=0) ? (0.5*ySize / (double)fMax) : 1.0;

          for ( ptrdiff_t i = 0; i < tCount; i++ ) {
            double x = (double)i;
            x *= xKoef;
            pList1.Add( MbCartPoint3D( x,         timeList[i]*tKoef, 0 ) );
            pList2.Add( MbCartPoint3D( x, (double)faceList[i]*fKoef, 0 ) );
          }

          MbCurve3D * curve1 = new MbPolyline3D( pList1, false );
          Style style1( 3, TestVariables::LIGHTRED );
          TestVariables::viewManager->AddObject( style1, curve1 );
          MbCurve3D * curve2 = new MbPolyline3D( pList2, false );
          Style style2( 1, TestVariables::LIGHTMAGENTA );
          TestVariables::viewManager->AddObject( style2, curve2 );

          DrawAxises( xSize, (double)tCount, xKoef, ySize, tMax, tKoef );

          int side = 1;
          if ( GetInt( IDS_TANGENT_PROLONG, side ) ) {
            if ( side>0 ) {
              sameShell = cm_Same;
              timeList.Flush();
              faceList.Flush();
        
              SetWaitCursor( true );

              rres = ::SolidRebuildFunctions( *solid, sameShell, timeList, faceList );

              if ( rres ) {
                tCount = timeList.Count();
                if ( tCount > 1 ) {
                  pList1.Flush();

                  for ( ptrdiff_t i = 0; i < tCount; i++ ) {
                    double x = (double)i;
                    x *= xKoef;
                    pList1.Add( MbCartPoint3D( x, timeList[i]*tKoef, 0 ) );
                  }

                  MbCurve3D * curve3 = new MbPolyline3D( pList1, false );
                  Style style3( 1, TestVariables::LIGHTCYAN );
                  TestVariables::viewManager->AddObject( style3, curve3 );
                }
              }
              SetWaitCursor( false );
            }
          }
        }
      }
      //activeHWND = hWin;
      //enableDraw = true;
      // Включение отрисовки. // Enable drawing.
      TestVariables::viewManager->SetEnableDrawToActiveWindow( save );
    }
    TestVariables::viewManager->RefreshObjectModel( obj );

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Сдвинуть конец дерева построения не обрезая или обрезать (cut == true).
// Move end of construction tree without cutting, or cut (cut == true).
// ---
void SolidEndMove( bool cut ) {

  MbItem *obj = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {

    TestVariables::viewManager->EraseObject(obj);
    MbSolid * solid = (MbSolid *)obj;

    if ( solid != nullptr ) {
      size_t cCount = solid->GetCreatorsCount();
      ptrdiff_t cnt = (ptrdiff_t)cCount;

      if ( GetInt( IDS_ENTER_NUMBER_OR_SHIFT, cnt ) && cnt != 0 ) {
        if ( cnt < 0 )
          cnt = ((ptrdiff_t)cCount) + cnt;

        if ( (cnt > 0) && (cnt <= (ptrdiff_t)cCount) ) {
          SetWaitCursor( true );

          if ( cut ) { // Обрезать безвозвратно. // Cut irrevocably.
            for ( size_t i = cCount-1; (ptrdiff_t)i >= cnt; i-- )
              solid->DeleteCreator( i );
            // Оставшиеся нужно сделать активными, т.к. до этого они могли быть выбраны как пропущенные.
            // The remaining need to do active, because they could be chosen as missed.
            solid->SetActiveCreatorsCount( solid->GetCreatorsCount() );
          }
          else { // Остальные cCount-cnt сделать не активными.  // Remaining cCount-cnt make inactive.
            solid->SetActiveCreatorsCount( cnt );
          }

          RPArray<MbSpaceItem> items( 0, 1 );
          MbeCopyMode sameShell = cm_KeepSurface;//cm_Same;
          bool res = solid->RebuildItem( sameShell, &items, nullptr );

          TestVariables::viewManager->RefreshObjectModel( obj );

          if ( !res || items.Count()>0 ) {
            MessageBoxEx( RT_OBJECT_BUILD_NOT_COMPLETELY, IDS_TITLE_ERR );
            for ( size_t i = 0; i < items.Count(); i++ ) {
              TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, items[i] );
            }
          }

          SetWaitCursor( false );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Достроить тело для последней невыполненной операции в журнале построения.
// Build a body by the last omitted operation in the build log.
// ---
void SolidFinish() {

  MbItem *obj = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {
    TestVariables::viewManager->EraseObject(obj);
    MbSolid * solid = (MbSolid *)obj;

    if ( solid != nullptr ) {
      SetWaitCursor( true );

      size_t i = 0, iCount = solid->GetCreatorsCount();

      for ( i = iCount-1; i > 0; i-- ) { 
        const MbCreator * cr = solid->GetCreator( i ); 
        if ( (cr != nullptr) &&
              (cr->GetStatus() == mps_Stop ||
              cr->GetStatus() == mps_Skip) ) {
            break;
        }
      }

      if ( i > 0 ) {
        bool res = solid->FinishItem();

        TestVariables::viewManager->RefreshObjectModel( obj );

        SetWaitCursor( false );
        if ( !res ) 
          MessageBoxEx( RT_OBJECT_BUILD_NOT_COMPLETELY, IDS_TITLE_ERR );
      }
      else
        MessageBoxEx( RT_EMD_ERROR_EMPTY_OBJECTS_LIST, IDS_TITLE_ALL_RIGHT );
    }
  }
}


//------------------------------------------------------------------------------
// Удалить элемент дерева построения.
// Delete a construction tree element.
// ---
void SolidCreatorDelete()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {

    TestVariables::viewManager->EraseObject(obj);
    MbSolid * solid = (MbSolid *)obj;

    if ( solid != nullptr ) {
      RPArray<MbCreator> creators( 0, 1 );

      if ( solid->GetCreators( creators ) ) {
        size_t cCount = creators.Count();
        ptrdiff_t cInd = cCount;

        if ( GetInt( IDS_ENTER_NUMBER_OR_SHIFT, cInd ) && (cInd != 0) &&
             (cInd >= -(ptrdiff_t)cCount && cInd <= (ptrdiff_t)cCount) ) {
          SetWaitCursor( true );

          if ( cInd < 0 )
            cInd = ((ptrdiff_t)cCount) + cInd;
          else
            cInd--;

          RPArray<MbCreator>   transactions( cCount-1, 1 );
          RPArray<MbSpaceItem> items( 0, 1 );
          MbFaceShell *nShell = nullptr;
          MbCreator * creator = nullptr;
          MbeCopyMode sameShell = cm_KeepSurface;//cm_Same;
          bool res = true;

          for ( size_t i = 0; i < cCount; i++ ) {
            if ( (ptrdiff_t)i == cInd )
              continue;
            creator = creators[i];
            if ( creator!=nullptr ) {
              if ( !creator->CreateShell( nShell, sameShell, &items ) ) {
                res = false;
                if ( sameShell==cm_Same ) { // Оболочка не копировалась и испорчена. // The shell was not copied and spoiled.
                  ::DeleteItem( nShell );
                  break;
                }
              }
              if ( nShell!=nullptr ) {
                transactions.Add(creator);
                nShell->SetOwnChangedThrough( tct_Unchanged );
              }
            }
          }

          if ( nShell!=nullptr ) {
            Style ns( obj->GetWidth(), obj->GetColor() );
            TestVariables::viewManager->DeleteObject( obj );

            MbRegDuplicate * ireg = nullptr;
            //MbAutoRegDuplicate autoReg( ireg ); // BUG_40923

            MbSolid * newSolid = new MbSolid( *nShell, transactions, false, ireg );
            TestVariables::viewManager->AddObject( ns, newSolid );
          }
          else {
            TestVariables::viewManager->RefreshObjectModel( obj );
          }

          SetWaitCursor( false );

          if ( !res || items.Count()>0 ) {
            MessageBoxEx( RT_OBJECT_BUILD_NOT_COMPLETELY, IDS_TITLE_ERR );
            for ( ptrdiff_t i = 0; i < (ptrdiff_t)items.Count(); i++ ) {
              TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, items[i] );
            }
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Установить последнюю версию элементу дерева. Install the latest version of the tree element.
// ---
void SolidCreatorSetLastVersion()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {
    MbSolid * solid = (MbSolid *)obj;

    if ( solid != nullptr ) {
      size_t cCount = solid->GetCreatorsCount();
      ptrdiff_t cInd = cCount;
      if ( GetInt( IDS_ENTER_NUMBER_OR_SHIFT, cInd ) && cInd > 0 && cInd <= (ptrdiff_t)cCount ) {
        cInd--;
        MbCreator * cr = solid->SetCreator( cInd );
        if ( cr != nullptr )
          cr->SetYourVersion( Math::DefaultMathVersion(), true );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Установить последнюю версию скруглениям. Set the latest version for fillets.
// ---
void SolidFilletsSetLastVersion()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {
    MbSolid * solid = (MbSolid *)obj;

    if ( solid != nullptr ) {
      for ( size_t ind = 0, iCount = solid->GetCreatorsCount(); ind < iCount; ++ind ) {
        MbCreator * cr = solid->SetCreator( ind );
        if ( cr != nullptr ) {
          MbeCreatorType type = cr->IsA();
          if ( type == ct_SmoothSolid ||
               type == ct_ChamferSolid ||
               type == ct_FilletSolid ||
               type == ct_FullFilletSolid ) {
            cr->SetYourVersion( Math::DefaultMathVersion(), true );
          }
          else { // C3D-13355. Случай вложенных построителей.
            c3d::CreatorsSPtrVector iCrs;
            cr->SetInternalCreators( ct_SmoothSolid, iCrs );
            cr->SetInternalCreators( ct_ChamferSolid, iCrs );
            cr->SetInternalCreators( ct_FilletSolid, iCrs );
            cr->SetInternalCreators( ct_FullFilletSolid, iCrs );
            for( size_t i = 0; i < iCrs.size(); ++i ) {
              iCrs[i]->SetYourVersion( Math::DefaultMathVersion(), true );
            }
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Удалить атрибуты.
// Remove attributes.
// ---
static
bool RemoveItemAttributes( MbItem & item )
{
  bool res = false;
  
  if ( item.RemoveAttributes() )
    res = true;

  MbeSpaceType isA = item.IsA();

  switch ( isA ) {

  case st_Solid: {
    MbSolid & solid = static_cast<MbSolid &>(item);

    if ( solid.GetFacesCount() > 0 ) {
      RPArray<MbTopologyItem> topItems( 8 * solid.GetFacesCount(), 1 );
      solid.GetShell()->GetItems( topItems );
      for ( size_t k = 0, cnt = topItems.size(); k < cnt; k++ ) {
        MbTopologyItem * topItem = topItems[k];
        if ( topItem != nullptr ) {
          if ( topItem->RemoveAttributes() )
            res = true;
        }
      }
    }
  } break;

  case st_Instance : {
    MbInstance & instance = static_cast<MbInstance &>(item);
    if ( instance.GetItem() != nullptr ) {
      if ( ::RemoveItemAttributes( *instance.SetItem() ) ) {
        res = true;
      }
    }
  } break;

  case st_Assembly: {
    MbAssembly & assembly = static_cast<MbAssembly &>(item);

    RPArray<MbItem> subItems;
    assembly.GetItems( subItems );
    for ( size_t k = subItems.size(); k--; ) {
      if ( subItems[k] != nullptr ) {
        if ( ::RemoveItemAttributes( *subItems[k] ) )
          res = true;
      }
    }
  } break;

  case st_Mesh: {
    MbMesh & mesh = static_cast<MbMesh &>(item );
    {
      c3d::GridsVector grids;
      size_t elemCnt = mesh.GridsCount();
      for ( size_t k = elemCnt; k--; ) {
        MbGrid * elem = mesh.SetGrid(k);
        if ( elem != nullptr ) {
          if ( elem->RemoveAttributes() )
            res = true;
        }
      }
    }
    {
      c3d::GridsVector grids;
      size_t elemCnt = mesh.PolygonsCount();
      for ( size_t k = elemCnt; k--; ) {
        MbPolygon3D * elem = mesh.SetPolygon( k );
        if ( elem != nullptr ) {
          if ( elem->RemoveAttributes() )
            res = true;
        }
      }
    }
    {
      c3d::GridsVector grids;
      size_t elemCnt = mesh.ApexesCount();
      for ( size_t k = elemCnt; k--; ) {
        MbApex3D * elem = mesh.SetApex( k );
        if ( elem != nullptr ) {
          if ( elem->RemoveAttributes() )
            res = true;
        }
      }
    }
  } break;

  case st_WireFrame: {
    MbWireFrame & wireFrame = static_cast<MbWireFrame &>(item);

    c3d::WireEdgesVector edges;
    wireFrame.SetEdges( edges );
    for ( size_t k = edges.size(); k--; ) {
      MbEdge * edge = edges[k];
      if ( edge != nullptr ) {
        if ( edge->RemoveAttributes() )
          res = true;
        if ( edge->SetBegVertex().RemoveAttributes() )
          res = true;
        if ( edge->SetEndVertex().RemoveAttributes() )
          res = true;
      }
    }
  } break;

  case st_PointFrame: {
    MbPointFrame & pointFrame = static_cast<MbPointFrame &>(item);
    for ( size_t k = pointFrame.GetVerticesCount(); k--; ) {
      MbVertex * vertex = pointFrame.SetVertex(k);
      if ( vertex != nullptr ) {
        if ( vertex->RemoveAttributes() )
          res = true;
      }
    }
  } break;

  default: break;

  }

  return res;
}


//------------------------------------------------------------------------------
// Удалить атрибуты тела.
// Remove solid attributes.
// ---
bool RemoveSolidAttributes()
{
  bool res = false;
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) )
  {
    if ( (obj != nullptr) && (obj->IsA() == st_Solid) ) {
      SetWaitCursor( true );

      TestVariables::viewManager->EraseObject(obj);

      MbSolid & solid = static_cast<MbSolid &>( *obj );

      if ( RemoveItemAttributes( solid ) )
        res = true;

      TestVariables::viewManager->RefreshObjectModel( obj );

      SetWaitCursor( false );
    }
  }
  else {
    bool processAll = true;

    if ( GetBool( IDS_TITLE_SELECT_ALL, processAll ) ) {
      SetWaitCursor( true );

      TestVariables::viewManager->EraseModel();

      RPArray<MbItem> items;
      TestVariables::viewManager->GetAllObjects( items );

      for ( size_t k = items.size(); k--; ) {
        MbItem * item = items[k];
        if ( item != nullptr ) {
          if ( RemoveItemAttributes( *item ) )
            res = true;
        }
      }

      TestVariables::viewManager->DeleteModelAttributes();

      TestVariables::viewManager->RefreshModel();

      SetWaitCursor( false );
    }
  }

  return res;
}


//------------------------------------------------------------------------------
// Добавить атрибут телу или его примитиву.
// Insert attribute to solid or its primitive.
// ---
void InsertAttribToSolid()
{
  MbItem * obj = nullptr;

  if ( GetGObj(IDS_SELECT_SOLID, st_Solid, &obj) )
  {
    SetWaitCursor( true );

    TestVariables::viewManager->EraseObject(obj);
    MbSolid * solid = static_cast<MbSolid *>(obj);

    if ( solid != nullptr )
    {
      PArray<MbCreator> creators(0,1,false);

      if ( !Math::SupressCreators() && solid->GetCreators( creators ) )
      {
        size_t cCount = creators.Count();
        // Индекс строителя, до которого строится тело, включительно.
        // Builder index to which the body is constructed, inclusive.
        ptrdiff_t buildIndex = -1;

        for ( size_t i = 0; i < cCount; i++ )
        {
          MbCreator * creator = creators[i];
          if ( creator == nullptr || creator->GetStatus() != mps_Stop )
              ++buildIndex;
          else
            break;
        }

        if ( buildIndex >= 0 )
        {
          MbAttributeProvider * attrCreator = nullptr;
          MbCreator * buildCreator = creators[buildIndex];
          if ( buildCreator != nullptr )
          {
            if ( buildCreator->IsA() == ct_AttributeProvider )
              attrCreator = static_cast<MbAttributeProvider*>( buildCreator );
            else {
              RPArray<MbCreator>   transactions( cCount + 1, 1 );
              RPArray<MbSpaceItem> items( 0, 1 );
              MbFaceShell * nShell = nullptr;
              MbCreator * creator = nullptr;
              MbeCopyMode sameShell = cm_KeepSurface;//cm_Same;

              for ( size_t i = 0; (ptrdiff_t)i <= buildIndex; i++ ) {
                creator = creators[i];
                if ( creator != nullptr )
                {
                  if ( !creator->CreateShell( nShell, sameShell, &items ) ) {
                    if ( sameShell==cm_Same ) { // Оболочка не копировалась и испорчена. // The shell was not copied and spoiled.
                      ::DeleteItem( nShell );
                      break;
                    }
                  }
                  if ( nShell!=nullptr ) {
                    transactions.Add(creator);
                    nShell->SetOwnChangedThrough( tct_Unchanged );
                  }
                }
              }

              MbSNameMaker names( ct_AttributeProvider, MbSNameMaker::i_SideNone, 0 );
              transactions.Add( new MbAttributeProvider(names) );

              for ( size_t i = buildIndex + 1; i < cCount; i++ )
                transactions.Add(creators[i]);

              if ( nShell != nullptr )
              {
                Style ns( obj->GetWidth(), obj->GetColor() );
                TestVariables::viewManager->DeleteObject( obj );

                MbRegDuplicate * ireg = nullptr;
                //MbAutoRegDuplicate autoReg( ireg ); // BUG_40923

                MbSolid * newSolid = new MbSolid( *nShell, transactions, false, ireg );
                TestVariables::viewManager->AddObject( ns, newSolid );
              }
              else {
                TestVariables::viewManager->RefreshObjectModel( obj );
              }

              ::DeleteItems( transactions );
            }
          }

          // Если поставщика атрибутов только создали - Лучьше добавлять при следующей попытке
          // А если он уже существовал то можно добавлять атрибуты
          if ( attrCreator != nullptr )
          {
            // Можно добавлять атрибуты
            // Будем делать это синхронно и в поставщик и в объект
            // Тогда при следующем перестроении результат останется таким-же
            MbItem * findObj = nullptr;
            MbRefItem * findItem = nullptr;

            COLORREF color;// = BLACK;
            ptrdiff_t i = 0;

            while ( GetObj( IDS_SELECT_GEOM_OBJECT, &findObj, &findItem) )
            {
              if ( findItem != nullptr && findObj == obj )
              {
                MbTopologyItem * topItem = dynamic_cast<MbTopologyItem *>( findItem );
                if ( topItem )
                {
                  MbName topName = topItem->GetName();

                  // Выбрали элемент(часть, примитив) объекта модели. // We chose the element (part, primitive) of the model object.
                  MbAttributeContainer * attrCont = dynamic_cast<MbAttributeContainer*>( findItem );
                  if ( attrCont != nullptr )
                  {
                    if ( i == 0 )
                    {
                      const MbAttribute * colorAttr = attrCont->GetSimpleAttribute( at_Visual );
                      if ( colorAttr != nullptr )
                        color = static_cast<const MbVisual *>( colorAttr )->GetColor();
                    }

                    if ( CMUColor(color) )
                    {
                      attrCreator->AddAttribute( topName, MbVisual::Create( color ) );

                      MbAttribute * colorAttr = attrCont->SetSimpleAttribute( at_Visual );
                      if ( colorAttr )
                        static_cast<MbVisual *>( colorAttr )->SetColor( color );
                      else {
                        attrCont->AddAttribute( MbVisual::Create( color ) );
                      }
                    }
                  }
                }
              }
              i++;
            }

          }
        }
      }
    }

    SetWaitCursor( false );
  }
}


////////////////////////////////////////////////////////////////////////////////
//
// Распознавание спиральных поверхностей в теле.
// Recognition of spiral surfaces in the solid.
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Решение СЛАУ методом Гаусса.
// a - матрица коэффициентов + столбец свободных членов
// x - вектор неизвестных
// Solution of linear systems by Gauss method.
// a - coefficient matrix + column of free terms
// x - definable vector 
// ---
static bool MethodGauss( std::vector<std::vector<double> > & a, std::vector<double> & x)
{
  bool result = true;
  const size_t n = x.size();
  size_t i, jmax, j, imax;
  double z, z1;
  // Номера неизвестных, нужно помнить, если переставляем не только строки, но и столбцы
  std::vector<size_t> ind( n );
  for ( i = 0; i < n; ++i )
  {
    ind[i] = i;
  }
  for ( size_t k = 0; k < n - 1; ++k ) {
    jmax = k;
    for ( j = k + 1; j < n; ++j ) {
      if ( ::fabs(a[k][j]) > ::fabs(a[k][jmax]) ) {
        jmax = j;
      }
    }
    imax = k;
    for ( i = k + 1; i < n; ++i ) {
      for ( j = k; j < n; ++j ) {
        if ( ::fabs(a[i][j]) > ::fabs(a[imax][jmax]) ) {
          imax = i;
          jmax = j;
        }
      }
    }
    if ( ::fabs(a[imax][jmax]) < DOUBLE_EPSILON ) {
      result = false;
      break;
    }
    if ( imax != k ) {
      swap( a[k], a[imax] );
    }
    if ( jmax != k ) {
      for ( i = 0; i < n; ++i ) {
        z = a[i][k];
        a[i][k] = a[i][jmax];
        a[i][jmax] = z;
      }
      i = ind[k];
      ind[k] = ind[jmax];
      ind[jmax] = i;
    }
    z1 = 1.0 / a[k][k];
    for ( i = k + 1; i < n; ++i ) {
      z = a[i][k] * z1;
      for ( j = k + 1; j <= n; ++j ) {
        a[i][j] -= a[k][j] * z;
      }
    }
  }
  if( result && (::fabs(a[n-1][n-1]) < DOUBLE_EPSILON) ) {
    result = false;
  }
  if ( result ) {
    for ( i = n; i--; ) {
      x[ind[i]] = a[i][n];
      for ( j = i + 1; j < n; ++j ) {
        x[ind[i]] -= a[i][j] * x[ind[j]];
      }
      x[ind[i]] /= a[i][i];
    }
  }
  return result;
}


//------------------------------------------------------------------------------
// Вычисление параметров окружности в плоскости наилучшим образом описывающей набор точек.
// center - центр круга в плоскости
// radius - радиус круга
// chi2 - критерий "хи квадрат" в МНК
// Calculation of parameters of a circle in the plane best describes the set of points.
// center - center of the circle in the plane
// radius - circle radius
// chi2 - criterion of "chi square"
// ---
static bool CalcParamsCircle( MbCartPoint & center, double & radius, double & chi2, const std::vector<MbCartPoint> & points )
{
  bool result = true;
  const size_t n = points.size();
  if ( n > 3 ) {
    std::vector<std::vector<double> > a;
    a.resize(3);
    for ( size_t i = 0; i < 3; ++i ) {
      a[i].resize(4);
      for ( size_t j = 0; j < 4; ++j )
        a[i][j] = 0.0;
    }
    std::vector<double> pars( 3, 0.0 );
    double x, y, x2, xy, y2; // Вспомогательные переменные
    // Аппроксимация точек окружностью с центром в точке (a, b) радиуса r
    // Ищется минимум суммы ( (x[i] - a)^2 + (y[i] - b)^2 - r^2 )^2 -> (a*x[i] + b*y[i] + c - (x[i]^2 + y[i]^2)/2)^2
    // Формирование матрицы для метода наименьших квадратов
    for ( size_t i = 0; i < n; ++i ) {
      x = points[i].x;
      y = points[i].y;
      x2 = x * x;
      xy = x * y;
      y2 = y * y;
      a[0][0] += x2; a[0][1] += xy; a[0][2] += x;
      a[1][0] += xy; a[1][1] += y2; a[1][2] += y;
      a[2][0] += x;  a[2][1] += y;
      // Правая часть
      a[0][3] += ( x2 + y2 ) * x;
      a[1][3] += ( x2 + y2 ) * y;
      a[2][3] += ( x2 + y2 );
    }
    a[2][2] = (double)n;
    for ( size_t i = 0; i < 3; ++i ) {
      a[i][3] *= 0.5;
    }
    // pars[0] = a, pars[1] = b, pars[2] = c = (r^2 - a^2 - b^2)/2
    result = ::MethodGauss( a, pars );
    if ( result ) {
      center.x = pars[0];
      center.y = pars[1];
      radius = ::sqrt( ::fabs(2.0 * pars[2] + pars[0] * pars[0] + pars[1] * pars[1]) );
      chi2 = 0.0;
      double z;
      MbVector v;
      for ( size_t i = 0; i < n; ++i ) {
        v = points[i] - center;
        z = v * v - radius * radius;
        chi2 += z * z;
      }
      chi2 = ::sqrt( chi2 ) / ( n * (n - 3) );
    }
  }
  return result;
}


//------------------------------------------------------------------------------
// Проверка равномерного распределения точек в множестве с точностью eps.
// Check uniform distribution of points in the set with eps precision.
// ---
static bool CheckUniform( const SArray<double> & points, double eps )
{
  bool result = true;
  size_t n = points.Count();
  if ( n > 1 ) {
    double dt = points[1] - points[0];

    for ( size_t i = 1; i < n - 1; ++i )
      if ( ::fabs((points[i + 1] - points[i]) - dt) > eps ) {
        result = false;
        break;
      }
  }
  return result;
}


//------------------------------------------------------------------------------
// Определение параметров спирали по образующей кривой и оси (ось может корректироваться).
// Determination of parameters of the spiral along generatrix curve and axis (axis can be adjusted).
// ---
static bool CalcParamsSpiral( const MbCurve3D & curve, MbVector3D & axisZ )
{
  bool result = false; // Кривая не является спиралью
  //const double paramEps  = Math::paramEpsilon;
  const double lengthEps = Math::metricEpsilon;
  const double tmin = curve.GetTMin();
  const double tmax = curve.GetTMax();
  MbCartPoint3D pmin, pmax; // Точки на кривой, соответствующие начальному и конечному параметрам спирали
  curve._PointOn( tmin, pmin );
  curve._PointOn( tmax, pmax );
  // Линия, проходящая через первую точку в направлении оси спирали
  MbLine3D lmin( pmin, axisZ );
  // Параметры точек пересечения кривой и lmin
  SArray<double> tCurve, tLine;
  ::CurveCurveIntersection( curve, lmin, tCurve, tLine, Math::metricNear, Math::DefaultMathVersion() );
  const size_t nCross = tCurve.Count(); // (nCross - 1) - число витков спирали
  // Точки пересечения для линии должны располагаться равномерно
  if ( ::CheckUniform(tLine, Math::paramNear) ) {
    // Шаг от начала спирали (точки будут внутри первого витка)
    const size_t nPnts = 8;
    const double ht = nCross > 1 ? ( tCurve[1] - tmin ) / nPnts : ( tmax - tmin ) /  nPnts;
    std::vector<double> t( nPnts ); // Параметры кривой (изменяются по ходу алгоритма)
    std::vector<MbCartPoint3D> p( nPnts ); // Соответствующие параметрам точки на кривой
    for ( size_t i = 0; i < nPnts; ++i ) {
      t[i] = tmin + i * ht;
      curve._PointOn( t[i], p[i] );
    }
    // Линии, проходящие через промежуточные точки в направлении оси спирали (изменяются)
    std::vector<MbLine3D *> line( nPnts );
    // Проекции начала кривой на линии (лежат на цилиндре)
    for ( size_t i = 1; i < nPnts; ++i ) {
      line[i] = new MbLine3D( p[i], axisZ );
      line[i]->NearPointProjection( p[0], t[i], true );
    }
    // Уточнение направления оси OZ
    if ( t[1] > 0.0 ) {
      axisZ.Invert();
    }
    // Точки на линиях, соответствующие проекциям p[0]
    for ( size_t i = 1; i < nPnts; ++i ) {
      line[i]->_PointOn( t[i], p[i] );
    }
    const MbPlane plane( p[0], p[1], p[2] ); // Плоскость OXY спирали
    std::vector<MbCartPoint> uv( nPnts );
    for ( size_t i = 0; i < nPnts; ++i ) {
      plane.GetPointProjection( p[i], true, uv[i].x, uv[i].y, true );
    }
    MbCartPoint centerArc;
    double radius = 0.0; // Радиус спирали
    double chi2 = 0.0;
    if ( ::CalcParamsCircle(centerArc, radius, chi2, uv) ) { // Вычисление центра и радиуса спирали
      MbCartPoint3D center; // Центр СК спирали
      plane._PointOn( centerArc.x, centerArc.y, center );
      bool direct = false; // Спираль крутится против часовой стрелки
      if ( ((p[1] - center) | (p[2] - center)) * axisZ < 0.0 ) { // Для неравномерной параметризации может потребоваться модификация
        direct = true;
      }
      // Уточнение положения оси OX (после чего первая точка кривой может не совпадать с первой точкой спирали)
      plane.GetPointProjection( pmin, true, uv[0].x, uv[0].y, true );
      plane._PointOn( uv[0].x, uv[0].y, p[0] );
      const MbVector3D axisX( p[0] - center ); // Ось OX спирали
      const MbPlacement3D place( center, axisZ, axisX, direct ); // СК спирали, спираль развернута
      //const double radius = axisX.Length(); // Радиус спирали
      line[1]->Init( center, axisZ ); // Линия, проходящая через центр спирали вдоль ее оси
      line[1]->NearPointProjection( pmax, t[0], true );
      // Точка на проекции (задает высоту)
      line[1]->_PointOn( t[0], p[0] );
      const double height = ( p[0] - center ).Length(); // Высота спирали
      double step( 0.0 ); // Шаг спирали
      if ( nCross > 1 ) {
        curve._PointOn( tCurve[1], p[0] ); // Точка через период на кривой
        step = ( p[0] - pmin ).Length();
      }
      else {
        const MbVector3D v1( pmax - p[0] );
        const MbVector3D v2( axisX | v1 );
        double angle = v1.Angle( axisX );
        if ( v2 * axisZ < 0.0 ) { // Спираль совершает больше, чем пол оборота
          angle = M_PI2 - angle;
        }
        if ( angle > EXTENT_EPSILON )
          step = height * M_PI2 / angle ;
        else
          step = MB_MAXDOUBLE;
      }
      if ( (radius > lengthEps) &&
           (step > lengthEps && step < MB_MAXDOUBLE) &&
           !((step < lengthEps) && (height > lengthEps)) ) {
        SPtr<MbConeSpiral> spiral( MbConeSpiral::Create( place, radius, radius, height, step ) );
        // Близость спирали к кривой на всем интервале параметров
        if ( (spiral != nullptr) && curve.IsSpaceNear(*spiral, Math::metricRegion, false) ) {
          result = true;
        }
      }
    }
  }
  return result;
}


//------------------------------------------------------------------------------
// Определение направления оси axisZ спирали curve методом спуска.
// Determination of axisZ axis direction of the spiral curve by descent method.
// ---
static bool CalcSpiralAxis( const MbCurve3D & curve, MbVector3D & axisZ )
{
  bool result = false; // Кривая точно не спираль
  const double tmin = curve.GetTMin();
  MbVector3D nmin; // Первая нормаль в первой точке
  curve._Normal( tmin, nmin );
  if ( nmin.Length() > Math::metricEpsilon ) { // У спирали ненулевые нормали
    const double t = curve.GetTMax() / Math::PI2; // pi для того, чтобы избежать периодичности
    MbVector3D n; // Нормаль в промежуточной точке
    curve._Normal( t, n );
    if ( (n.Length() > Math::metricEpsilon) &&
         !nmin.Colinear(n, Math::angleRegion) ) {
      axisZ = nmin | n; // Начальное значение оси
      result = true;
    }
  }
  if ( result ) { // Поиск начальных приближений для точек пересечения спирали с образующей линией
    MbCartPoint3D pmin; // Начальная точка кривой
    curve._PointOn( tmin, pmin );
    // Линия, проходящая через первую точку в направлении оси спирали
    const MbLine3D lmin( pmin, axisZ );
    SArray<double> tCurve, tLine; // Точки пересечения кривой с lmin
    ::CurveCurveIntersection( curve, lmin, tCurve, tLine, Math::metricNear, Math::DefaultMathVersion() ); // Обязательно не точнее metriNear
    const size_t nCross = tCurve.Count(); // (nCross - 1) - приближенное число витков спирали
    const size_t n = nCross; // Число витков (+1), точки которых должны лежать на одной прямой
    // Точки пересечения линии со спиралью должны удовлетворять условию r(t0) - r(t1) = r(t1) - r(t2), ..., r(tn-2) - r(tn-1) = r(tn-1) - r(tn) ->
    // r(t0) - n*r(tn-1) + (n - 1)*r(tn) = 0, n - число витков
    if ( n > 2 ) { // Для определения точного пересечения необходимо как минимум три точки
      double t1 = tCurve[n - 2]; // = tn-1
      double t2 = tCurve[n - 1]; // = tn
      MbCartPoint3D p1, p2; // Точки на кривой, отвечающие первым начальным приближениям
      // Уточнение оси
      curve._PointOn( t1, p1 );
      curve._PointOn( t2, p2 );
      axisZ = 0.5 * ( p1 + p2 - 2 * pmin );

      MbVector3D f, dp1, dp2;
      double a, b; // Для метода спуска
      double x1 = t1,
             x2 = t2;
      double dt1 = 1.0,
             dt2 = 1.0;
      bool proc = true; // Нормальный ход поиска решения
      size_t count = 0; // Счетчик итераций
      double nn = (double)n;
      f = pmin - ( nn - 1 ) * p1 + ( nn - 2 ) * p2;
      a = f.Length();
      a *= a;
      size_t j;
      double alpha = 0.1; // Возможно, необходимо определять и уточнять по ходу алгоритма
      // Не быстрый вариант, требуется оптимизация
      do {
        j = 0;
        while ( j == 0 ) {
          curve._PointOn( t1, p1 );
          curve._PointOn( t2, p2 );
          nn = (double)n;
          f = pmin - ( nn - 1 ) * p1 + ( nn - 2 ) * p2;
          curve._FirstDer( t1, dp1 );
          curve._FirstDer( t2, dp2 );
          x1 -= alpha * ( -2.0 * (nn - 1) ) * ( f * dp1 );
          x2 -= alpha * (  2.0 * (nn - 2) ) * ( f * dp2 );
          curve._PointOn( x1, p1 );
          curve._PointOn( x2, p2 );
          f = pmin - ( nn - 1 ) * p1 + ( nn - 2 ) * p2;
          b = f.Length();
          b *= b;
          if ( a > b ) {
            dt1 = ::fabs( t1 - x1 );
            dt2 = ::fabs( t2 - x2 );
            t1 = x1;
            t2 = x2;
            a = b;
          }
          else {
            x1 = t1;
            x2 = t2;
            j = 1;
          }
        }
        alpha /= 10;
        if ( ++count > NEWTON_COUNT_2X ) {
          proc = false;
        }
      }
      while ( proc && (dt1 > PARAM_EPSILON || dt2 > PARAM_EPSILON) );
      if ( proc ) {
        curve._PointOn( t1, p1 );
        curve._PointOn( t2, p2 );
        axisZ = 0.5 * ( p1 + p2 - 2 * pmin );
      }
    }
  }
  return result;
}


//------------------------------------------------------------------------------
// Аппроксимация поверхности surface спиральной.
// Approximation of the surface by spiral surface.
// ---
static bool ApproxSpiralSurface( const MbSurface & surface )
{
  bool result = false;
  const MbCurve3D * curveV1 = surface.CurveV( surface.GetUMin(), nullptr, false );
  if ( curveV1 != nullptr ) {
    MbVector3D axisZ1;
    // Определение направления оси направляющей (спирали) по v при u = umin
    if ( ::CalcSpiralAxis(*curveV1, axisZ1) ) {
      const MbCurve3D * curveV2 = surface.CurveV( surface.GetUMax(), nullptr, false );
      if ( curveV2 != nullptr ) {
        MbVector3D axisZ2;
        // Определение направления оси направляющей (спирали) по v при u = umax
        if ( ::CalcSpiralAxis(*curveV2, axisZ2) ) {
          MbVector3D axisZ = 0.5 * ( axisZ1 + axisZ2 );
          if ( ::CalcParamsSpiral(*curveV1, axisZ) ) { // Определение параметров спирали
            result = true;
          }
        }
        ::DeleteItem( curveV2 );
      }
    }
    ::DeleteItem( curveV1 );
  }
  // Проверка, что в сечение surface линия или дуга окружности
  if ( result ) {
    const MbCurve3D * curveU = surface.CurveU( surface.GetVMin(), nullptr, false );
    result = false;
    if ( curveU != nullptr ) {
      if ( curveU->GetBasisCurve().IsA() == st_LineSegment3D ) {
        result = true;
      }
      else {
        SPtr<MbCurve> curve2d( nullptr );
        MbPlacement3D place;
        if ( curveU->GetPlaneCurve(curve2d, place, true) ) {
          MbCartPoint center; // Центр дуги
          double radius = 0.0; // Радиус дуги
          std::vector<MbCartPoint> points( 4 );
          const double tmin = curveU->GetTMin(),
                       tmax = curveU->GetTMax();
          const double h = 0.25 * ( tmax - tmin );
          double t = tmin;
          for ( size_t i = 0; i < 4; ++i ) {
            curve2d->PointOn( t, points[i] );
            t += h;
          }
          double chi2 = 0.0;
          // Вычисление параметров окружности, аппроксимирующей curve2d
          ::CalcParamsCircle( center, radius, chi2, points );
          // Проверка, что получившаяся окружность действительно подобна curve2d
          if ( chi2 > Math::metricNear ) {
            result = false;
          }
        }
        ::DeleteItem( curveU );
      }
    }
  }
  return result;
}


//------------------------------------------------------------------------------
// Аппроксимация поверхностей solid спиральными поверхностями.
// solid - импортированное тело
// surfaces - набор спиралеподобных поверхностей в solid
// Approximation of the solid surfaces by spiral surfaces.
// solid - imported solid
// surfaces - set of similar to spiral surfaces in solid
// ---
static void ApproxSpiralShell( const MbSolid & solid, std::vector<const MbSurface *> & surfaces )
{
  const MbFaceShell * shell = solid.GetShell();
  RPArray<MbFace> faces;
  shell->GetFaces( faces );
  for ( size_t i = 0, nFaces = faces.Count(); i < nFaces; ++i ) {
    const MbSurface * surface = &faces[i]->GetSurface();
    // Добавляем только спиральные поверхности
    if ( ::ApproxSpiralSurface(*surface) ) {
      surfaces.push_back( surface );
    }
  }
  shell = nullptr;
}


//------------------------------------------------------------------------------
// Распознавание спиральных поверхностей в теле.
// Recognition of spiral surfaces in the solid.
// ---
void RecognitionSpiralSurfaces()
{
  MbItem * obj;
  if ( GetGObj(IDS_SELECT_SOLID, st_Solid, &obj) ) {
    Style st( obj->GetWidth(), obj->GetColor() );
    TestVariables::viewManager->ToggleObject( obj );
    std::vector<const MbSurface *> surfaces; // Набор поверхностей obj, являющихся спиральными
    SetWaitCursor( true );
    ::ApproxSpiralShell( *(static_cast<MbSolid *>(obj)), surfaces );
    SetWaitCursor( false );
    if ( !surfaces.empty() ) {
      //viewManager->DeleteObject( obj );
      std::vector<const MbSurface *>::const_iterator i;
      // Спиральные поверхности obj
      for ( i = surfaces.begin(); i != surfaces.end(); ) {
        TestVariables::viewManager->AddObject( st, const_cast<MbSurface *>(*(i++)) );
      }
    }
    else {
      TestVariables::viewManager->ToggleObject( obj );
      MessageBoxEx( RT_SPIRAL_SURFACES_NOT_FOUND, IDS_EMPTY_VECTOR );
    }
  }
}


//------------------------------------------------------------------------------
// Проверка главных имён рёбер и вершин.
// ---
void CheckShellMainNames()
{
  MbItem * solidObject = nullptr;
  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &solidObject ) )
  {
    if ( solidObject != nullptr && solidObject->IsA() == st_Solid )
    {
      TestVariables::viewManager->ToggleObject(solidObject);
      MbFaceShell * shell = static_cast<MbSolid *>(solidObject)->GetShell();
      if ( shell != nullptr )
      {
        ConstEdgesVector edges;
        ConstVerticesVector vertices;
        if ( !::CheckTopologyItemsMainNames(*shell, edges, vertices) )
        {
          for ( auto it = edges.begin(); it != edges.end(); ++it )
            TestVariables::drawImp->DrawItem(*it, TRGB_LIGHTRED, MbMatrix3D::identity, 2);
          for ( auto it = vertices.begin(); it != vertices.end(); ++it )
            TestVariables::drawImp->DrawItem(*it, TRGB_BLUE, MbMatrix3D::identity, 3);
          MessageBoxEx( RT_WRONG_MAIN_NAMES, IDS_TITLE_WRN );
        }
        else
          ::MessageBoxEx( RT_DEFECTS_NOT_FOUND, IDS_TITLE_DIAG_ALL_RIGHT );
      }
      TestVariables::viewManager->ToggleObject(solidObject);
    }
  }
}
