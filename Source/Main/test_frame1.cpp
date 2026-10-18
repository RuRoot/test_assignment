#include <test.h>
#include <test_manager.h>
#include <test_gr_draw.h>
#include <test_service.h>
#include <test_converter.h>
#include <test_space.h>
#include <test_computation.h>
#include <test_mates.h>
#include <test_edit_space.h>
#include <test_plane.h>
#include <surface.h>
#include <solid.h>
#include <attr_color.h>
#include <tool_enabler.h>
#include <tool_cstring.h>
#include <conv_model_document.h>
#include <alg_draw.h>
#include <last.h>

using namespace c3d;

#if defined (C3D_WINDOWS) && !defined(ALL_WARNINGS)  // _MSC_VER // Set warnings level
#pragma warning(push)          // Preserve current state of warning settings
#pragma warning(disable: 4244) // warning C4244: a potential loss of data during a type conversion
#endif

//----------------------------------------------------------------------------------------
// Получить формат файла.
// ---
inline MbeModelExchangeFormat GetFileFormat( const c3d::path_string& path )
{
  MbeModelExchangeFormat impFrom = mxf_autodetect;
  std::map< c3d::path_string, MbeModelExchangeFormat > extensions;
  extensions[c3d::StdToPathstring( "step" )] = mxf_STEP;
  extensions[c3d::StdToPathstring( "stp" )] = mxf_STEP;
  extensions[c3d::StdToPathstring( "iges" )] = mxf_IGES;
  extensions[c3d::StdToPathstring( "igs" )] = mxf_IGES;
  extensions[c3d::StdToPathstring( "jt" )] = mxf_JT;
  extensions[c3d::StdToPathstring( "x_t" )] = mxf_Parasolid;
  extensions[c3d::StdToPathstring( "xmt_txt" )] = mxf_Parasolid;
  extensions[c3d::StdToPathstring( "xmp_txt" )] = mxf_Parasolid;
  extensions[c3d::StdToPathstring( "x_b" )] = mxf_ParasolidBinary;
  extensions[c3d::StdToPathstring( "xmt_bin" )] = mxf_ParasolidBinary;
  extensions[c3d::StdToPathstring( "xmp_bin" )] = mxf_ParasolidBinary;
  extensions[c3d::StdToPathstring( "m_t" )] = mxf_Parasolid;
  extensions[c3d::StdToPathstring( "sat" )] = mxf_ACIS;
  extensions[c3d::StdToPathstring( "sab" )] = mxf_ACISBinary;
  extensions[c3d::StdToPathstring( "stl" )] = mxf_STL;
  extensions[c3d::StdToPathstring( "wrl" )] = mxf_VRML;
  extensions[c3d::StdToPathstring( "3mf" )] = mxf_3MF;
  extensions[c3d::StdToPathstring( "obj" )] = mxf_OBJ;
  extensions[c3d::StdToPathstring( "txt" )] = mxf_ASCIIPoint;
  extensions[c3d::StdToPathstring( "asc" )] = mxf_ASCIIPoint;
  extensions[c3d::StdToPathstring( "xyz" )] = mxf_ASCIIPoint;
  extensions[c3d::StdToPathstring( "c3d" )] = mxf_C3D;
  size_t dotPos = path.find_last_of( c3d::StdToPathstring( "." ) );
  if ( dotPos != SYS_MAX_T )
  {
    c3d::path_string fileExt( path.c_str() + dotPos + 1 );          //OV_Lnx string fileExt (path.get_wcs() + dotPos + 1);
// #ifdef C3D_WINDOWS //_MSC_VER
//     fileExt = ::CharLowerEx( (TCHAR*) fileExt.c_str() );  //OV_LNX fileExt.to_lower();
// #else  // C3D_WINDOWS
//                                                                    // AVA Внимание!!! tolower работает гарантированно корректно только для ASCII символов.
//                                                                    // Но в случае расширений файлов этого достаточно.
//     
//#endif
  std::transform( fileExt.begin(), fileExt.end(), fileExt.begin(), ::tolower );
  std::map<c3d::path_string, MbeModelExchangeFormat>::iterator itr = extensions.find(fileExt);
  if (itr != extensions.end())
    impFrom = itr->second;
  }
  return impFrom;
}

//------------------------------------------------------------------------------
// Обработка команд главного меню окна видов
// ---
bool MenuCommand( uint wParam ) {

  if ( TestVariables::viewManager == nullptr )
    return false;

  switch (wParam) {

    // ******************************** Read / Write model, object from/into a file

    case CM_FILEOPEN : // Read a model from a file
      ::ReadModel( nullptr );
      break;

    case CM_FILEADD : // Add into the model objects from a file
      ::AddModel();
      break;

    case CM_FILENEW :
      TestVariables::viewManager->DeleteModel(); // Clear model
      TestVariables::viewManager->EraseBackground( TestVariables::viewManager->GetActiveWindowIndex() );
      // Запомненный объект и его элемент
      TestVariables::ResetHLighted();       
      break;

    case CM_FILETAKE : // Read an object from a file
      ::ReadItem();
      break;

    case CM_FILESAVE : // Write model into a file using the current working version
      ::WriteModel( nullptr, GetCurrentMathFileVersion() );
      break;

    case CM_FILESAVE_PREV_RELEASE: // Write model into a file using the previous release version
      ::WriteModel( nullptr, wrv_PrevRelease );
      break;

    case CM_FILESAVE_LAST_RELEASE : // Write model into a file using the last release version
      ::WriteModel( nullptr, wrv_LastRelease );
      break;

    case CM_CONSOLE:  // Launch console
      ::RunConsole();
      break;

    case CM_FILEEXPORT : // Write model into a mat-file // DEPRECATED format
      ::WriteItem();
      break;

    case CM_TREEOPEN: { // Прочитать дерево модели из файла
      ::ReadModelTree(nullptr, of_selection);
    } break;

    case CM_TREE_EMBODIMENT: { // Прочитать дерево модели из файла
      ::ReadModelTree(nullptr, of_embodiment);
    } break;

    case CM_TREE_TYPE: {  // Прочитать объекты по типу
      ::ReadModelTree(nullptr, of_type);
    } break;

    case CM_TREE_GABARIT: { // Прочитать объекты по габариту
      ::ReadModelTree ( nullptr, of_gabarit );
    } break;

    case CM_TREE_ATTRIBUTE: { // Прочитать объекты по атрибуту
      ::ReadModelTree ( nullptr, of_attribute );
    } break;

    case CM_WRITE_MODEL_TREE: { // Включить/выключить запись дерева модели
      if ( TestVariables::useModelTree ) {
        TestVariables::useModelTree = false;
        ::MessageBoxEx ( IDS_WRITING_MODEL_TREE_OFF, IDS_TITLE_MSG );
      }
      else {
        TestVariables::useModelTree = true;
        ::MessageBoxEx ( IDS_WRITING_MODEL_TREE_ON, IDS_TITLE_MSG );
      }
    }  break;

    case CM_ADD_VIEW_ATTR: {
      TestVariables::viewManager->GetActiveWindow()->AddViewData( true );
      break;
    }
    case CM_REMOVE_VIEW_ATTR: {
      TestVariables::viewManager->GetActiveWindow()->RemoveViewData();
      break;
    }
    case  CM_WORK_OF_CREATORS: { // Включить/выключить работу построителей // "Enable/disable the work of constructors.
      bool flag = Math::SupressCreators();
      Math::SetSupressCreators( !flag );
      if ( flag )
        ::MessageBoxEx( IDS_CONSTRUCTORS_ENABLED, IDS_TITLE_MSG );
      else
        ::MessageBoxEx( IDS_CONSTRUCTORS_DISABLED, IDS_TITLE_MSG );
    } break;

    case CM_MODE_SPECIAL_HIGHLIGHT:
    { // включить/выключить подсветку подобъектов
      if ( TestVariables::useHighlightObj ) {
        TestVariables::useHighlightObj = false;
        ::MessageBoxEx( IDS_HIGHLIGHTINHG_OBJ_OFF, IDS_TITLE_MSG );
      }
      else {
        TestVariables::useHighlightObj = true;
        ::MessageBoxEx( IDS_HIGHLIGHTINHG_OBJ_ON, IDS_TITLE_MSG );
      }
    }  break;

    case CM_SAT_READ: // Прочитать SAT
    case CM_IGES_READ: // Прочитать IGES
    case CM_XT_READ: // Прочитать X_T
    case CM_STEP_READ: // Прочитать STEP
    case CM_STL_READ: // Прочитать STL
    case CM_VRML_READ: // Прочитать VRML
    {
      std::unique_ptr<IProgressIndicator> progBar{&::CreateProgressIndicator(new StrSpyConverters)};
      ConvConvertorProperty3D convProperties;
      c3d::ItemSPtr itemReadTo;
      //convProperties.fileName = c3d::C3DToPathstring(TestVariables::nameBuffer);
      AskForImportOptions(::GetFileFormat(c3d::C3DToPathstring(TestVariables::nameBuffer)), convProperties);
      bool isGoodRead = ::ImportFromFile(itemReadTo, C3DToPathstring(TestVariables::nameBuffer), &convProperties, progBar.get());
      if (isGoodRead)
      {
        ::AddContentToManager(*itemReadTo, false);
      }
    }
    break;

    case CM_SAT_WRITE: // Записать SAT
    case CM_IGES_WRITE: // Записать IGES
    case CM_XT_WRITE: // Записать X_T
    case CM_STEP_WRITE: // Записать STEP
    case CM_STL_WRITE: // Записать STL
    case CM_VRML_WRITE: // Записать VRML
    {
      ConvConvertorProperty3D convProperties;
      C3dModelDocument modelDoc;
      //convProperties.fileName = c3d::C3DToPathstring(TestVariables::nameBuffer);
      AskForExportOptions(::GetFileFormat(c3d::C3DToPathstring(TestVariables::nameBuffer)), convProperties);
      std::unique_ptr<IProgressIndicator> progBar{&::CreateProgressIndicator(new StrSpyConverters)};
      ::ExportIntoFile(modelDoc, convProperties.fileName, &convProperties, progBar.get());
    }
      break;

    case CM_EDITREDRAW : { // Перерисовать модель
      SetWaitCursor( true );
      TestVariables::viewManager->RefreshModel();
      SetWaitCursor( false );
    } break;

    case CM_CUTMODELOBJECTS :   // Разрезать модель
      CutModelObjects();
      break;

    // ******************************** установочные операции

    case CM_SETCOLORSELECTED : // Изменить цвет выделенных элементов
    {
      COLORREF color = TestVariables::viewManager->GetSelectedObjectColor(); //BLACK;
      if ( CMUColor( color ) )
      {
        RPArray<MbItem> selItems( 0, 1 );
        TestVariables::viewManager->GetSelectedObjects( selItems );
        for ( size_t k = 0, kcnt = selItems.Count(); k < kcnt; k++ ) {
          SPtr<MbItem> selItem( selItems[k] );
          if ( selItem->IsA() == st_Solid ) {
            MbFaceShell * shell =  static_cast<MbSolid *>( selItem.get() )->GetShell();
            for ( size_t i = 0; i < shell->GetFacesCount(); i++ ) {
              MbFace * face = shell->GetFace( i );
              if ( face != nullptr ) {
                const MbAttribute * attr = face->GetSimpleAttribute( at_Visual );
                if ( attr != nullptr && static_cast<const MbVisual*>(attr)->GetColor() != color )
                  face->RemoveAttribute( attr );
              }
            }
          }
          selItem->SetColor( color );
        }
      }
    } break;

    case CM_SETSELECTCOLOR : // Изменить цвет выделения
    {
      COLORREF color = TestVariables::viewManager->GetSelectColor();//WHITE;
      if ( CMUColor( color ) )
        TestVariables::viewManager->SetSelectColor( color );
    } break;

    case CM_SETBACKGRCOLOR : // Изменить цвет окон
    {
      COLORREF color = TestVariables::viewManager->GetBackgrColor();
      if ( CMUColor( color ) )
        TestVariables::viewManager->SetBackgrColor( color );
    } break;

    case CM_GLRENDERCOLOR : // Изменить цвет всех граней тела.
    {
      SetObjectColor( true );
    } break;

    case CM_GLPROPERT : // Установить свойства поверхности для OpenGL
    case CM_GLAMBIENT :
    case CM_GLDIFFUSE :
    case CM_GLSPECULARITY :
    case CM_GLSHININESS :
    case CM_GLOPACITY :
    case CM_GLEMISSION :
    { // Установить
      WorkWindow *vc = TestVariables::viewManager->GetActiveWindow();
      if (vc) {
        float a,d,sp,sh,t,e;
        MbItem * obj = nullptr;
        while ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj ) ) {
          obj->GetVisual(a,d,sp,sh,t,e);
          {
            TestVariables::viewManager->ToggleObject(obj);
            switch (wParam) {
              case CM_GLPROPERT :
                if ( GetFloat( IDS_ENTER_BACKGROUND, a  ) &&
                     GetFloat( IDS_ENTER_DIFFUSION, d  ) &&
                     GetFloat( IDS_ENTER_REFLECTIVITY, sp ) &&
                     GetFloat( IDS_ENTER_BRIGHTNESS, sh ) &&
                     GetFloat( IDS_ENTER_OPACITY, t  ) &&
                     GetFloat( IDS_ENTER_EMISSION, e  ) )
                  obj->SetVisual(a,d,sp,sh,t,e);
              break;
              case CM_GLAMBIENT :
                if ( GetFloat( IDS_ENTER_BACKGROUND, a  ) )
                  obj->SetVisual(a,d,sp,sh,t,e);
              break;
              case CM_GLDIFFUSE :
                if ( GetFloat( IDS_ENTER_DIFFUSION, d  ) )
                  obj->SetVisual(a,d,sp,sh,t,e);
              break;
              case CM_GLSPECULARITY :
                if ( GetFloat( IDS_ENTER_REFLECTIVITY, sp ) )
                  obj->SetVisual(a,d,sp,sh,t,e);
              break;
              case CM_GLSHININESS :
                if ( GetFloat( IDS_ENTER_BRIGHTNESS, sh ) )
                  obj->SetVisual(a,d,sp,sh,t,e);
              break;
              case CM_GLOPACITY :
                if ( GetFloat( IDS_ENTER_OPACITY, t  ) )
                  obj->SetVisual(a,d,sp,sh,t,e);
              break;
              case CM_GLEMISSION  :
                if ( GetFloat( IDS_ENTER_EMISSION, e  ) )
                  obj->SetVisual(a,d,sp,sh,t,e);
              break;
            }
            TestVariables::viewManager->ToggleObject(obj);
            vc->GetDrawTool().ResetGL();
//            wPrev = wParam;
//            lPrev = lParam;
//            cmPrev = wParam;
          }
        }
      }
    } break;

    case CM_SETLIGHTDIRECTION : { // Установить направление падающего света
      WorkWindow * vc = TestVariables::viewManager->GetActiveWindow();
      if ( vc ) {
        //cPoint = vc->lightLocal;
        MbVector3D v = vc->GetDrawTool().GetLight();
        MbCartPoint3D cPoint; cPoint = v;

        if ( GetExactPoint( cPoint ) )
        {
          //lightDirection = cPoint;
          //if ( !lightDirection.Normalize() )
          //  lightDirection.Init(2,2,3);
          TestVariables::viewManager->SetLightDirection( cPoint ); // Сохраняем для создания новых окон

          //vc->lightLocal = lightDirection;
          //vc->lightSide = lightDirection;
          vc->GetDrawTool().SetLight( TestVariables::viewManager->GetLightDirection() );

//          wPrev = wParam;
//          lPrev = lParam;
//          cmPrev = wParam;
        }
      }
    } break;

    case CM_UVMESHCOUNT :
    { // Установить количество линий отрисовки.
      WorkWindow * vc = TestVariables::viewManager->GetActiveWindow();
      if ( vc ) {
        MbItem * obj = nullptr;
        MbTopItem * topoItem = nullptr;

        if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &topoItem, TestVariables::itemPathFrom.second ) ) {
          MbSolid * solid = (MbSolid *)obj;
          MbFace * face = (MbFace *)topoItem;

          if ( (solid != nullptr) && (face != nullptr) ) {
            size_t uMeshCount, vMeshCount;

            MbAttribute * attr = face->SetSimpleAttribute( at_WireCount ); // выдать простой атрибут данного типа
            if ( attr != nullptr ) {
              ((MbWireCount *)attr)->Get( uMeshCount, vMeshCount );
              ptrdiff_t uCount( uMeshCount ), vCount( vMeshCount );
              if ( GetInt( IDS_ENTER_U_POINT_NUMBER, uCount ) &&
                   GetInt( IDS_ENTER_V_POINT_NUMBER, vCount ) ) {
                if ( ((size_t)uCount != uMeshCount) || ((size_t)vCount != vMeshCount) )
                  ((MbWireCount *)attr)->Init( (size_t)uCount, (size_t)vCount );
              }
            }
            else {
              uMeshCount = face->GetSurface().GetUMeshCount();
              vMeshCount = face->GetSurface().GetVMeshCount();
              ptrdiff_t uCount( uMeshCount ), vCount( vMeshCount );
              if ( GetInt( IDS_ENTER_U_POINT_NUMBER, uCount ) &&
                   GetInt( IDS_ENTER_V_POINT_NUMBER, vCount ) ) {
                if ( ((size_t)uCount != uMeshCount) || ((size_t)vCount != vMeshCount) ) {
                  MbWireCount * newAttr = new MbWireCount( (size_t)uCount, (size_t)vCount );
                  face->AddAttribute( newAttr );
                  ::DeleteItem( newAttr );
                }
              }
            }
          }
        }
        else { // For all selected solids.
          ptrdiff_t uCount( 4 ), vCount( 4 );

          if ( GetInt( IDS_ENTER_U_POINT_NUMBER, uCount ) &&
               GetInt( IDS_ENTER_V_POINT_NUMBER, vCount ) && 
               (uCount >= 0) && (vCount >= 0) ) {
            RPArray<MbItem> items( 10, 1 );
            SArray<MbMatrix3D> matrs( 10, 1 );

            vc->GetObjects( st_Solid, items, matrs ); // Дать все объекты указанного типа.

            for ( size_t i = 0, iCount = items.Count(); i < iCount; i++ ) {
              MbItem * item = items[i];
              if ( (item != nullptr) && (item->IsA() == st_Solid) ) {
                MbSolid * solid = (MbSolid *)item;
                for ( size_t j = 0, jCount = solid->GetFacesCount(); j < jCount; j++ ) {
                  MbFace * face = solid->GetFace( j );

                  if ( face != nullptr ) {
                    MbAttribute * attr = face->SetSimpleAttribute( at_WireCount ); // выдать простой атрибут данного типа
                    if ( attr != nullptr ) {
                      ((MbWireCount *)attr)->Init( (size_t)uCount, (size_t)vCount );
                    }
                    else {
                      MbWireCount * newAttr = new MbWireCount( (size_t)uCount, (size_t)vCount );
                      face->AddAttribute( newAttr );
                      ::DeleteItem( newAttr );
                    }
                  }
                }
              }
            }
          }
        }
      }
    } break;

    case CM_CURVATURE_PLOT : { // Масштаб отображения кривизны.
      double d = TestVariables::curvatureScale;
      if ( GetDouble( IDS_ENTER_CURVATURE_PLOT, d ) ) {
        if ( d != 0 )
          TestVariables::curvatureScale = fabs(d);
      }
    } break;

    case CM_DELTA_RANGE : { // Установить
      double d = TestVariables::delta_range;
      if ( GetDouble( IDS_ENTER_IM_SHIFT_STEP, d ) ) {
        if (d!=0)
          TestVariables::delta_range = fabs(d);
      }
    } break;

    case CM_DELTA_ANGLE : { // Установить
      double d = TestVariables::delta_angle;
      d *= 180/M_PI;
      if ( GetDouble( IDS_ENTER_IM_SHIFT_ANG, d ) ) {
        if (d!=0) {
          d *= M_PI/180;
          TestVariables::delta_angle = fabs(d);
        }
      }
    } break;

    case CM_SETCURVEDEGREE : { // Установить порядок кривой
      ptrdiff_t degree = Math::curveDegree;
      if ( GetInt( IDS_ENTER_SPLINE_ORDER, degree ) )
#ifdef USE_VAR_CLASSES
        Math::curveDegree.SetVarValue( std_max((ptrdiff_t)2, degree) );
#else
        Math::curveDegree = std_max( (ptrdiff_t)2, degree );
#endif
    } break;

    case CM_SETSURFACEDEGREE : // Установить порядок поверхности
      SetSurfaceDegree();
      break;

    case CM_SKIP_ASSERTS : { // пропускать ассерты
#ifdef _MSC_VER
      int mode = _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_REPORT_MODE );

      if ( TestFlagValue(mode, _CRTDBG_MODE_WNDW) )
      {
        DownFlagValue( mode, _CRTDBG_MODE_WNDW  );
        UpFlagValue  ( mode, _CRTDBG_MODE_DEBUG ); // output window
        ::MessageBoxEx( IDS_SKIP_ASSERT_ON, IDS_TITLE_MSG );
      }
      else
      {
        DownFlagValue( mode, _CRTDBG_MODE_DEBUG );
        UpFlagValue  ( mode, _CRTDBG_MODE_WNDW  ); // dialog window
        ::MessageBoxEx( IDS_SKIP_ASSERT_OFF, IDS_TITLE_MSG );
      }
      _CrtSetReportMode( _CRT_ASSERT, mode );
#endif
    } break;

#ifdef _DRAWGI
    case CM_SWITCH_DEBUG_DRAW: // Отключить отладочную отрисовку.
    {
      static bool swithOn = true;
      if ( swithOn ) {
        ::SetDrawGI( nullptr );
        swithOn = false;
        ::MessageBoxEx( IDS_SKIP_DEBUG_DRAW_OFF, IDS_TITLE_MSG );
      }
      else {
        ::SetDrawGI( TestVariables::drawImp );
        swithOn = true;
        ::MessageBoxEx( IDS_SKIP_DEBUG_DRAW_ON, IDS_TITLE_MSG );
      }
    } break;
#endif

    case CM_APPLY_CONFIG_FILE: 
    {
      if(ApplyC3dTestConfig())
      ::MessageBoxEx( CONFID_FILE_UPDATED, IDS_TITLE_MSG );
      else
      {
#if defined(C3D_WINDOWS)
        std::string configPath = std::getenv( "USERPROFILE" ) + std::string( "\\c3d_test.config" );
#else
        std::string configPath = std::getenv( "HOME" ) + std::string( "/c3d_test.config" );
#endif
        c3d::string_t errCfg = ToC3Dstring( configPath ) + CONFID_FILE_NOT_FOUND ;
        ::MessageBoxEx( errCfg.c_str(), IDS_TITLE_MSG );
      }
      break;
    }

    case CM_SWITCH_MULTITHREADING : { // Переключить режим многопоточности
      int im = (int)Math::MultithreadedMode();
      GetInt( IDS_MULTITHREADED_MODE, im );
      MbeMultithreadedMode mode = ( MbeMultithreadedMode )im;
      switch( mode )
      {
      case mtm_Off:  
        Math::SetMultithreadedMode( mode );
        ::MessageBoxEx( MUTLTHREADING_MODE_OFF_EX, IDS_TITLE_MSG ); 
        break;
      case mtm_Standard:  
        Math::SetMultithreadedMode( mode );
        ::MessageBoxEx( MUTLTHREADING_MODE_STANDARD_EX, IDS_TITLE_MSG ); 
        break;
      case mtm_SafeItems:  
        Math::SetMultithreadedMode( mode );
        ::MessageBoxEx( MUTLTHREADING_MODE_ITEMS_EX, IDS_TITLE_MSG ); 
        break;
      case mtm_Items:  
        Math::SetMultithreadedMode( mode );
        ::MessageBoxEx( MUTLTHREADING_MODE_PARALLEL_ITEMS_EX, IDS_TITLE_MSG ); 
        break;
      case mtm_Max: 
        Math::SetMultithreadedMode( mode );
        ::MessageBoxEx( MUTLTHREADING_MODE_MAX_EX, IDS_TITLE_MSG ); 
        break;
      default: 
        Math::SetMultithreadedMode( mtm_Off );
        ::MessageBoxEx( MUTLTHREADING_MODE_INCORRECT, IDS_TITLE_MSG );
      }
    } break;

    case CM_MULTITHREADED_MODE: { // Узнать режим многопоточности
      int mode = Math::MultithreadedMode();
      switch(mode)
      {
      case mtm_Off:  
        ::MessageBoxEx( MUTLTHREADING_MODE_OFF_EX, IDS_TITLE_MSG ); 
        break;
      case mtm_Standard:  
        ::MessageBoxEx( MUTLTHREADING_MODE_STANDARD_EX, IDS_TITLE_MSG ); 
        break;
      case mtm_SafeItems:  
        ::MessageBoxEx( MUTLTHREADING_MODE_ITEMS_EX, IDS_TITLE_MSG ); 
        break;
      case mtm_Items:  
        ::MessageBoxEx( MUTLTHREADING_MODE_PARALLEL_ITEMS_EX, IDS_TITLE_MSG ); 
        break;
      case mtm_Max: 
        ::MessageBoxEx( MUTLTHREADING_MODE_MAX_EX, IDS_TITLE_MSG ); 
        break;
      }
    } break;

    case CM_LIST_OF_MODES: { // Список режимов многопоточности
      const size_t/*ulong*/ maxLen = 3 * 1024;
      TCHAR sMtmMOdes[maxLen];

      size_t/*ulong*/ len = 0;

      len += _sntprintf( sMtmMOdes + len, maxLen-len, MUTLTHREADING_MODE_OFF );
      len += _sntprintf( sMtmMOdes + len, maxLen-len, MUTLTHREADING_MODE_STANDARD );
      len += _sntprintf( sMtmMOdes + len, maxLen-len, MUTLTHREADING_MODE_ITEMS );
      len += _sntprintf( sMtmMOdes + len, maxLen-len, MUTLTHREADING_MODE_PARALLEL_ITEMS );
      len += _sntprintf( sMtmMOdes + len, maxLen-len, MUTLTHREADING_MODE_MAX );


      MessageBoxEx( sMtmMOdes, MUTLTHREADING_MODES );
    } break;

    case CM_SETSTYLEWIDTH : // Установить толщину стиля для новых объектов
    {
      int is = TestVariables::lineWidth;
      if ( GetInt( IDS_ENTER_THICKNESS, is ) ) {
        if (is<=0) is = 1;
        TestVariables::CONIC_Style.SetWidth() = is;
        TestVariables::CURVE_Style.SetWidth() = is;

        TestVariables::LINE_Style.SetWidth()     = is;
        TestVariables::LINE_SEG_Style.SetWidth() = is;
        TestVariables::POLYLINE_Style.SetWidth() = is;
        TestVariables::BSPLINE_Style.SetWidth()  = is;
        TestVariables::OFFSET_Style.SetWidth()   = is;
        TestVariables::CONTOUR_Style.SetWidth()  = is;
        TestVariables::CIRCLE_Style.SetWidth()   = is;
        TestVariables::ARC_Style.SetWidth()      = is;
        TestVariables::ELLIPSE_Style.SetWidth()  = is;
        TestVariables::EQU_Style.SetWidth()      = is;
        TestVariables::SURFACECURVE_Style.SetWidth() = is;
      }
      TestVariables::viewManager->SetSelectedObjectWidth( is );
      TestVariables::lineWidth = is;
    } break;

    case CM_SETMAKEGRID : // Рассчитывать триангуляцию модели.
    {
      WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
      if ( wind ) {
        int iMakeGrid = wind->IsNeedGrid() ? 1 : 0;
        if ( GetInt( IDS_ENTER_MAKE_GRID, iMakeGrid ) )
          wind->SetNeedGrid( iMakeGrid > 0 ); // Рассчитывать триангуляцию модели/Не рассчитывать триангуляцию модели.
      }
      break;
    }
    case CM_SET_OGL_VIEWTYPE : // Установить способ отображения OGL
    {
      OGLViewMode viewMode = TestVariables::viewManager->GetWindowOGLViewType();
      int viewType = (int)viewMode;
      if ( GetInt( IDS_OGL_VIEWTYPE, viewType ) ) {
        viewMode = (OGLViewMode)viewType;
        TestVariables::viewManager->SetWindowOGLViewType( viewMode );
        TestVariables::viewManager->ReDrawGL();
      }
      break;
    }
    case CM_SMARTCURSOR : // Установить привязки к объектам и координатной сетке. Use finding for nearest objects and coordinate cells
    {
      //OGLViewMode viewMode = TestVariables::viewManager->GetWindowOGLViewType();
      int cursorType = TestVariables::smartCursor;
      if ( GetInt( IDS_OGL_VIEWTYPE, cursorType ) ) {
        TestVariables::smartCursor = cursorType;
      }
      break;
    }

    case CM_SETVISUALSAG : // Установить стрелку прогиба
    {
      double vsag = Math::visualSag;
      if ( GetDouble( IDS_ENTER_NUMBER, vsag ) )
#ifdef USE_VAR_CLASSES
        Math::visualSag.SetVarValue( ::fabs(vsag) );
#else
        Math::visualSag = ::fabs( vsag );
#endif
      break;
    }

    case CM_SETDEVIATIONSAG : { // Угловое отклонение
      double d = Math::deviateSag;
      if ( GetDouble( IDS_ENTER_NUMBER, d ) ) {
        d = ::fabs( d );
        d = std_min( d, M_PI_2 );
        d = std_max( d, ANGLE_EPSILON );
#ifdef USE_VAR_CLASSES
        Math::deviateSag.SetVarValue( d );
#else
        Math::deviateSag = d;
#endif
      }
    } break;

    case CM_SETCURSORSTEP : { // Установить шаг округления координат курсора при вводе точки
      double d = TestVariables::delta_point;
      if ( GetDouble( IDS_ENTER_ACCURACY, d ) ) {
        if (d!=0)
          TestVariables::delta_point = fabs(d);
      }
    } break;

    case CM_REBUILDTIMERESULTFILE : { // Записывать в файл результаты замеров времени
      int iBool = 0;
      GetInt( IDS_ENTER_TIMERESULTFILE, iBool );
      TestVariables::rebuildTimeToFile = (iBool != 0);
    } break;

    case CM_CHECK_READ_TIME:
    { // Выводить время чтения модели ( Show read time of model )
      int iBool = TestVariables::checkReadTime ? 1 : 0;
      GetInt( IDS_ENTER_CHECK_READ_TIME, iBool );
      TestVariables::checkReadTime = (iBool != 0);
    } break;

    case CM_CHECK_WRITE_TIME:
    { // Выводить время чтения модели ( Show read time of model )
      int iBool = TestVariables::checkWriteTime ? 1 : 0;
      GetInt( IDS_ENTER_CHECK_WRITE_TIME, iBool );
      TestVariables::checkWriteTime = (iBool != 0);
    } break;

    case CM_CHECK_OPERATION_TIME:
    { // Выводить время операции ( Show operation time )
      int iBool = TestVariables::checkOperationTime ? 1 : 0;
      GetInt( IDS_ENTER_CHECK_PROC_TIME, iBool );
      TestVariables::checkOperationTime = (iBool != 0);
    } break;

    case CM_EDIT_SAME_CREATORS : {
      GetBoolean( IDS_ENTER_USE_OR_NOT_BOOL, TestVariables::sameCreators, true );
    } break;

    // ******************************** выделение объектов

    case CM_EDITSELECTALL : // Выделить все объекты
      TestVariables::viewManager->SelectModel();
      break;

    case CM_EDITUNSELECTALL : // Погасить все выделенные объекты
      TestVariables::viewManager->UnselectModel();
      break;

    case CM_EDITREVERSSELECTALL : // Инверсировать все выделенные объекты
      TestVariables::viewManager->ToggleModel();
      break;

    // ******************************** выделение точек

    case CM_SELECTALLPOINTS : // Выделение всех точек
      TestVariables::viewManager->SelectByType( st_Point3D );
      TestVariables::viewManager->SelectByType( st_PointFrame );
      break;

    // ******************************** выделение кривых

    case CM_SELECTALLCURVES : // Выделение всех кривых
      TestVariables::viewManager->SelectByType( pt_Curve );
      TestVariables::viewManager->SelectByType( st_Curve3D );
      TestVariables::viewManager->SelectByType( st_WireFrame );
      break;

    case CM_SELECTLINE : // Выделение всех прямых
      TestVariables::viewManager->SelectByType( pt_Line );
      TestVariables::viewManager->SelectByType( st_Line3D );
      break;

    case CM_SELECTPOLYCURVE : // Выделение всех B-сплайн кривых
      TestVariables::viewManager->SelectByType( st_Hermit3D );
      TestVariables::viewManager->SelectByType( st_Bezier3D );
      TestVariables::viewManager->SelectByType( st_Nurbs3D );
      break;

    case CM_SELECTCONTOURS2D :    // Выделение всех плоских кривых
      TestVariables::viewManager->SelectByType( pt_Curve );
      //TestVariables::viewManager->SelectByType( pt_Contour );
      //TestVariables::viewManager->SelectByType( pt_ContourWithBreaks );
      break;

    case CM_SELECTCONTOURS3D :    // Выделение всех пространственных кривых
      TestVariables::viewManager->SelectByType( st_Curve3D );
      break;

    case CM_SELECTCONTOURSONSURFACE :  // Выделение всех кривых на поверхности
      TestVariables::viewManager->SelectByType( st_SurfaceCurve );
      TestVariables::viewManager->SelectByType( st_SilhouetteCurve );
      TestVariables::viewManager->SelectByType( st_ContourOnSurface );
      TestVariables::viewManager->SelectByType( st_ContourOnPlane );
      break;

    case CM_SELECTINTERSECTIONCURVE : // Выделение всех кривых пересечения
      TestVariables::viewManager->SelectByType( st_SurfaceIntersectionCurve );
      break;

    // ******************************** выделение поверхностей

    case CM_SELECTALLSURFACES : // Выделение всех поверхностей
      TestVariables::viewManager->SelectByType( st_Surface );
      break;

    case CM_SELECTPLANE : // Выделение всех плоскостей
      TestVariables::viewManager->SelectByType( st_Plane );
      break;

    case CM_SELECTELEMENTARYSURFACE : // Выделение всех элементарных поверхностей
      TestVariables::viewManager->SelectByType( st_ConeSurface );
      TestVariables::viewManager->SelectByType( st_CylinderSurface );
      TestVariables::viewManager->SelectByType( st_SphereSurface );
      TestVariables::viewManager->SelectByType( st_TorusSurface );
      break;

    case CM_SELECTSWEPTSURFACE : // Выделение всех поверхностей
      TestVariables::viewManager->SelectByType( st_ExtrusionSurface );
      TestVariables::viewManager->SelectByType( st_RevolutionSurface );
      TestVariables::viewManager->SelectByType( st_EvolutionSurface );
      TestVariables::viewManager->SelectByType( st_RuledSurface );
      TestVariables::viewManager->SelectByType( st_SectorSurface );
      break;

    case CM_SELECTPOLYSURFACE : // Выделение всех поверхностей
      TestVariables::viewManager->SelectByType( st_SplineSurface );
      TestVariables::viewManager->SelectByType( st_HermitSurface );
      break;

    case CM_SELECTOFFSETSURFACE : // Выделение всех поверхностей
      TestVariables::viewManager->SelectByType( st_OffsetSurface );
      break;

    case CM_SELECTCURVEBOUNDEDSURFACE : // Выделение всех усеченных кривыми поверхностей
      TestVariables::viewManager->SelectByType( st_CurveBoundedSurface );
      break;

    // ******************************** выделение тел

    case CM_SELECT_ALL_SOLIDS : // Выделение всех тел
      TestVariables::viewManager->SelectByType( st_Solid );
      break;

    case CM_SELECT_CLOSED_SOLIDS : // Выделение всех замкнутых тел
      {
        TestVariables::viewManager->SelectByType( st_Solid );
        RPArray<MbItem> selItems( 0, 1 );
        TestVariables::viewManager->GetSelectedObjects( selItems );
        for ( size_t k = 0, kcnt = selItems.Count(); k < kcnt; k++ ) {
          MbItem * selItem = selItems[k];
          if ( selItem != nullptr && selItem->IsA() == st_Solid ) {
            if ( !((const MbSolid *)selItem)->IsClosed() )
              TestVariables::viewManager->UnselectObject( selItem );
          }
        }
      }
      break;

    case CM_SELECT_UNCLOSED_SOLIDS : // Выделение всех незамкнутых тел
      {
        TestVariables::viewManager->SelectByType( st_Solid );
        RPArray<MbItem> selItems( 0, 1 );
        TestVariables::viewManager->GetSelectedObjects( selItems );
        for ( size_t k = 0, kcnt = selItems.Count(); k < kcnt; k++ ) {
          MbItem * selItem = selItems[k];
          if ( selItem != nullptr && selItem->IsA() == st_Solid ) {
            if ( ((const MbSolid *)selItem)->IsClosed() )
              TestVariables::viewManager->UnselectObject( selItem );
          }
        }
      }
      break;

    case CM_FITONFIRSTLOADING : // Выкл/Вкл адаптацию модели в окно.
      TestVariables::fitOnFirstLoading = !TestVariables::fitOnFirstLoading;
      if ( TestVariables::fitOnFirstLoading ) // Включена адаптация ЛСК окна для текущей модели.
        ::MessageBoxEx( FIT_FIRST_LOADING_ON, IDS_TITLE_MSG );
      else // Выключена адаптация модели что-бы сохранить настроенную ориентацию ЛСК окна.
        ::MessageBoxEx( FIT_FIRST_LOADING_OFF, IDS_TITLE_MSG );
      break;

    case CM_FITMODELTOWINDOW: // Вписать модель в размер окна. // Fit model to window
      SetWaitCursor( true );
      TestVariables::viewManager->GetActiveWindow()->FitModelToWindow();  
      TestVariables::viewManager->RefreshModel();
      SetWaitCursor( false );
      break;

    case CM_SETVIEWPLACEMENT :
      while ( ViewRotateInObjectPlane() );    // Повернуть плоскость вида плоскость в объекта
      break;

    case CM_SETPROJECTION : // Установка проекции
      SetProjection();
      break;

    case CM_SETDEPTH : // Изменить глубину проекции
      SetDepth();
      break;

    case CM_CHANGESCALE : // Установить масштаб вида
      SetScale();
      break;

    case CM_VIEWFRONT : // Спереди
    case CM_VIEWREAR  : // Сзади
    case CM_VIEWUP    : // Сверху
    case CM_VIEWDOWN  : // Снизу
    case CM_VIEWLEFT  : // Слева
    case CM_VIEWRIGHT : // Справа
    case CM_VIEWISO   : // Изометрия
      CreateViewWindow( (uint)wParam );
      break;

    case CM_VIEWFRONTPROJ : // Спереди
      SetViewWindow( v_Front );
      break;
    case CM_VIEWREARPROJ  : // Сзади
      SetViewWindow( v_Rear );
      break;
    case CM_VIEWUPPROJ    : // Сверху
      SetViewWindow( v_Up );
      break;
    case CM_VIEWDOWNPROJ  : // Снизу
      SetViewWindow( v_Down );
      break;
    case CM_VIEWLEFTPROJ  : // Слева
      SetViewWindow( v_Left );
      break;
    case CM_VIEWRIGHTPROJ : // Справа
      SetViewWindow( v_Right );
      break;
    case CM_VIEWISOPROJ   : // Изометрия
      SetViewWindow( v_Isometry );
      break;

//        case ZOOMIN : // Увеличение масштаба
//          viewManager->EnlargeScale();
//          break;

//        case ZOOMOUT : // Уменьшение масштаба
//          viewManager->ReduceScale();
//          break;

    case CM_PTORIGIN/*pt_Origin*/ : // Установка нуля в центр вида
      TestVariables::viewManager->SetOriginIntoCentre(false);
      break;

    case CM_SETVIEWPOINT : { // Установить точку наблюдения перспективной проекции
      double d = TestVariables::viewManager->GetActiveWindow()->GetVista().z;
      if ( GetDouble( IDS_ENTER_DISTANCE, d ) )
        TestVariables::viewManager->SetViewPoint( d );
    } break;

    // ******************************** помощь

    case CM_HELPABOUT : { // Помощь
      const size_t/*ulong*/ maxLen = 4 * 1024;
      TCHAR sHotKeys[maxLen];

      size_t/*ulong*/ len = 0;

      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_KEYBOARD );
      len += _sntprintf( sHotKeys+len, maxLen-len, _T("----------------------------------------\n") );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_KEYS_ARROWS );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_CTRL_KEYS_ARROWS );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_SHIFT_KEYS_ARROWS );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_CTRL_O );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_CTRL_S );
      len += _sntprintf( sHotKeys+len, maxLen-len, _T("\n") );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_TAB_LEFT_MOUSE_BUTTON );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_INSERT_RIGHT_MOUSE_BUTTON );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_BACKSPACE );
      len += _sntprintf( sHotKeys+len, maxLen-len, _T("\n") );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_F2 );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_F3 );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_F4 );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_SHIFT_F4 );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_F5);
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_SHIFT_F5 );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_F6 );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_SHIFT_F6 );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_F7 );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_SHIFT_F7 );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_F8 );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_F9 );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_F11 );
      //len += _sntprintf( sHotKeys+len, maxLen-len, IDS_F12 );
      len += _sntprintf( sHotKeys+len, maxLen-len, _T("\n") );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_NUMPAD_ASTERISK );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_NUMPAD_MINUS );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_NUMPAD_PLUS );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_CTRL_NUMPAD_ASTERISK );
      len += _sntprintf( sHotKeys+len, maxLen-len, _T("\n") );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_CTRL_F8 );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_SHIFT_F8 );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_CAPSLOCK_F8 );
      len += _sntprintf( sHotKeys+len, maxLen-len, _T("\n") );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_MOUSE );
      len += _sntprintf( sHotKeys+len, maxLen-len, _T("----------------------------------------\n") );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_MOUSE_MOVE );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_SHIFT );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_CENTRE_MOUSE_KEY );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_SHIFT_CENTRE_MOUSE_KEY );
      len += _sntprintf( sHotKeys+len, maxLen-len, _T("\n") );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_LEFT_MOUSE_KEY );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_SHIFT_LEFT_MOUSE_KEY );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_DBL_LEFT_MOUSE_KEY );
      len += _sntprintf( sHotKeys+len, maxLen-len, IDS_RIGHT_MOUSE_KEY );
      len += _sntprintf( sHotKeys+len, maxLen-len, _T("\n") );

      // KYA K11 Допишите, если что-либо забыл или появилось что-то новое
      PRECONDITION( len < maxLen );

      MessageBoxEx( sHotKeys, IDS_HOTKEYS );

    } break;

    default:
      return false;
  }

  return true;
}


//------------------------------------------------------------------------------
// Выполнение расчетов
// ---
bool MakeAll( uint wParam ) {

  if ( TestVariables::viewManager==nullptr )
    return false;

  switch (wParam) {

    // ******************************** Выполнение расчетов

    case  CM_POINTLOCATION_RELATIVETOBODY : // Point location relative to a body
      PointLocation();
      break;

    case CM_POINTDISTANCETOPOINT : // Расстояние от точки до точки объекта
      while( PointDistanceToPoint() );
      break;

    case CM_GETDISTANCETOPOINT : // Расстояние от точки до объекта
      DistanceToPointObject();
      break;

    case CM_GETCURVELENGTH : // Вычислить длину кривой
      while ( CurveLength() );
      break;

    case CM_GETWIREFRAMELENGTH: // Вычислить длину каркаса.
      WireFrameLength();
      break;

    case CM_GETCURVERADIUS : // Вычислить радиус кривизны кривой
      CurveRadius();
      break;

    case CM_GETEDGEFACESANGLE : // Угол между гранями ребра
      while( GetEdgeFacesAngle() );
      break;

    case CM_GETEDGEEDGEANGLE : // Угол между двумя ребрами. // Angle between two edges.
      while( GetEdgeEdgeAngle() );
      break;

    case CM_GEOMITEMS_MINDISTANCE : // Расстояние между геометрическими объектами
      while ( GeomItemsMinDistance() );
      break;

    case CM_TOPOITEMS_MINDISTANCE : // Расстояние между топологическими объектами
      TopoItemsMinDistance();
      break;

    case CM_SOLIDS_MINDISTANCE : // Расстояние между топологическими объектами
      while ( SolidsMinDistance() );
      break;

    case CM_COLLISION_CONSTRUCT_BVT_NODES :
      ComputeBVTree(); // Расчет иерархии деревьев объемов
      break;

    case CM_COLLISION_DISTANCE_QUERY :
      CollisionDistanceQuery(); // Расчет расстояния между телами
      break;

    case CM_COLLISION_INTERFERENCE_SOLIDS :
      CollisionInterferenceSolids(); // Расчет коллизий тел в статике
      break;

    case CM_COLLISION_INTERFERENCE_MESHES :
      CollisionInterferenceMeshes(); // Расчет коллизий сеток в статике
      break;

    case CM_CROSS_BOOLEAN :
      InterferenceSolids(); // Расчет пересечения тел в статике булевой операцией
      break;

    case CM_FIND_TOUCHED_FACES :
      InterferenceDetection( IT_FindTouchedFaces ); // Расчет контактирующих граней
      break;

    case CM_SPLIT_TOUCHED_FACES :
      InterferenceDetection( IT_SplitTouchedFaces ); // Расчет разбиения контактирующих граней тел
      break;

    case CM_MERGE_SOLIDS :
      InterferenceDetection( IT_TouchedSolidsMerging ); // Расчет объединения тел по контактирующим граням
      break;

    case CM_GETSOLIDMASSINERTIA :
      SolidMassInertia();    // Определение массо-центровочных характеристик тела
      break;

    case CM_GETCONTOURAREA :
      ContourArea();         // Определение площади замкнутой кривой
      break;

    case CM_GETMESHINERTIA:
      MeshMassInertia();    // Определение массо-центровочных характеристик полигональных объектов
      break;

    case CM_GETFACECENTRE :
      FaceCentre();          // Расчет центра поверхности грани.
      break;

    case CM_CHECK_SURFACE_SPEED :
      CheckSurfaceSpeed();   // Проверить скорость работы поверхности
      break;

    case CM_CHECK_CURVE3D_SPEED :
      CheckCurve3DSpeed();   // Проверить скорость работы пространственной кривой
      break;

    case CM_CHECK_CURVE2D_SPEED :
      CheckCurve2DSpeed();   // Проверить скорость работы двумерной кривой
      break;

    case CM_CHECK_CURVE2D_SELFINT:
      CheckCurve2DSelfInt(); // Проверить двумерную кривую на самопересечение
      break;

    case CM_CHECK_CURVE3D_SELFINT:
      CheckCurve3DSelfInt(); // Проверить трехмерную кривую на самопересечение
      break;

    case CM_CHECK_SURFACE_SELFINT:
      CheckSurfaceSelfInt(); // Проверить трехмерную кривую на самопересечение
      break;

    case CM_CHECK_SKETCH:
      CheckSketch(); // Проверить эскиз
      break;

    case CM_CREATE_SURFACE_OPER_TESS : // Создать операционную разбивку поверхности
      CreateSurfaceOperTesselation();
      break;

    case CM_SURFAXESDISTANCE : // Расстояние между осями поверхностей, имеющих оси
      SurfAxesDistance();
      break;

    case CM_GEOMITEMS_MAXDISTANCE : // Максимальное расстояние между геометрическими объектами
      while ( GeomItemsMaxDistance( nullptr, nullptr ) );
      break;

    case CM_TOPOITEMS_MAXDISTANCE : // Максимальное расстояние между топологическими объектами
      TopoItemsMaxDistance();
      break;

    case CM_GRIDDED_SURF_MINMAXDISTANCES : // Расстояния между поверхностями по сетке
      GriddedSurfMinMaxDistances();
      break;

    case CM_SURF_MAX_MIN_CURVATURES:    // Максимальная и минимальная кривизна поверхности
      SurfaceMaxMinCurvatures();
      break;

    case CM_FACES_MAX_MIN_CURVATURES : // Максимальная и минимальные кривизны граней
      FacesMaxMinCurvatures();
      break;

    case CM_CURVE_MAX_MIN_CURVATURES:    // Максимальная и минимальная кривизна кривой.
      CurveMaxMinCurvatures();
      break;

    /*
      Тестовые процедуры для сопряжений.
      Testing procedures for mates.
    */

    case CM_COMMON_MATES_TEST: // Общее тестирование сопряжений.
      CommonMatesTest();
      break;
    case CM_MATE_CUBE: // Создать сборку из кирпичей, жестко соединенных  в линию, стенку или куб.
      RigidConnectedBricks();
      break;
    case CM_MATE_FRACTAL: // Создать фрактальную сборку.
      MateFractal();
      break;
    case CM_MATE_DISTRIBUTED_DOF: // Создать сборку с распределенными степенями свободы.
      DistributedDoFMates();
      break;
    case CM_LINEAR_PATTERN: // Создать линейный массив компонент
      LinearPattern();
      break;
    case CM_ANGULAR_PATTERN: // Создать угловой массив компонент
      AngularPattern();
      break;

    /*
      Выполнение раскраски.
      Rendering commands.
    */

    case CM_SURFACERENDERING : // Тонирование поверхностей
      SurfaceRendering();
      break;

    case CM_SOLIDRENDERING : // Тонирование твердых тел
      SolidRendering();
      break;

    case CM_RENDERING : // Тонирование модели - раскрасить модель в отдельном окне
      ModelRendering();
      break;

    case CM_SETVIEWVECTOR : // Расположить по вектору.
      SetViewVector();
    break;

    case CM_TRIANGULATESURFACE : // Преобразовать поверхность в множество треугольных поверхностей
      TriangulateSurface();
      break;

    case CM_TRIANGULATESOLID :   // Аппроксимировать тело в множеством треугольных пластин
      TriangulateSolid();
      break;

    case CM_TRIANGULATEFACE :    // Аппроксимировать грань тела в множеством треугольных пластин
      TriangulateFace();
      break;

    case CM_TRIANGULATEPOINTCLOUD :  // Построить триангуляцию по облаку точек.
      TriangulatePointCloud();
    break;

    case CM_PARAMTRIANGULATEFACE : // аппроксимировать грань тела треугольными пластинами и нарисовать а области параметров
      ParamTriangulateFace();

    case CM_TRIANGULATESOLIDS :  // Аппроксимировать все тела множеством треугольных пластин
      TriangulateAllSolids();
      break;

    case CM_SIMPLIFY_TRGN_CONVEX_HULL :
      SimplifySolidTrgn();  // Произвести упрощение триангуляции поверхности
      break;

    case CM_TRIANGULAREREGION :
      TriangulateRegion(); // Триангуляция региона
      break;

    case CM_CURVEGABARIT :   // Габарит кривой
      while ( PutCurveGabarit() );
      break;

    case CM_SURFACEGABARIT : // Габарит поверхности
      while ( PutSurfaceGabarit() );
      break;

    case CM_FACEGABARIT :    // Габарит грани тела
      while ( PutFaceGabarit() );
      break;

    case CM_SOLIDGABARIT :   // Габарит тела
      while ( PutSolidGabarit() );
      break;

    case CM_LOCALGABARIT_CURVE2D :  // Габарит объекта относительно л.с.к.
      PutCurve2DLocalGabarit();
      break;

    case CM_LOCALGABARIT_CURVE3D :  // Габарит объекта относительно л.с.к.
      PutCurve3DLocalGabarit();
      break;

    case CM_LOCALGABARIT_SURFACE :  // Габарит объекта относительно л.с.к.
      PutSurfaceLocalGabarit();
      break;

    case CM_LOCALGABARIT_EDGE :     // Габарит объекта относительно л.с.к.
      PutEdgeLocalGabarit();
      break;

    case CM_LOCALGABARIT_FACE :     // Габарит объекта относительно л.с.к.
      PutFaceLocalGabarit();
      break;

    case CM_LOCALGABARIT_SOLID :    // Габарит объекта относительно л.с.к.
      PutSolidLocalGabarit();
      break;

    // ******************************** Выполнение построений

    case CM_FLOOD_FILL_VOLUME : // Объёма заполненного сосуда. // The volume of flood fill.
      FloodFill();
      break;

    case CM_SURF_NORMALS : // Нормали поверхности
      while ( PutSurfNormals( nullptr, false ) );
      break;

    case CM_FACE_NORMALS : // Нормали грани
      while ( PutFaceNormals( nullptr ) );
      break;

    case CM_MESH_NORMALS:
      while ( PutMeshNormals( nullptr ) );
      break;

    case CM_CURVEMAP : // Развертка кривой на поверхности
      CurveOnSurfaceMap();
      break;
    case CM_CURVESMAP : // Развертка множества кривых на поверхности
      SetCurvesOnSurfaceMap();
      break;

    case CM_SURFACEMAP : // Развертка поверхности
      SurfaceBorderMap();
      break;

    case CM_SOLIDMAP : // Развертка твердого тела
      SolidFaceLoopMap();
      break;

    case CM_SOLIDFACEMAP : // Развертка грани твердого тела
      while ( FaceLoopMap() );
      break;

    case CM_SURFACEHIDE : // Получение очерка поверхности
      while ( SurfacesHide() );
      break;

    case CM_FACEHIDE : // Получение очерка грани
      while ( FaceHide() );
      break;

    case CM_SOLIDHIDE : // Получение очерка тела
      while ( SolidsHide() );
      break;

    case CM_CURVETESTA :
      LineSegmentNURBSCurve();               // Построение характеристической ломаной NURBS
      break;

    case CM_SURFACETESTA :
      LineSegmentNURBSSurface();             // Построение характеристической ломаной NURBS
      break;

    case CM_MAKESPACEWIREFRAME :
      MakeSpaceWire();                       // Построение каркаса
      break;

    case CM_MAKEPLANEWIREFRAME :
      MakePlaneWire();                       // Построение плоского каркаса
      break;

    case CM_MAKEWIREFRAMEONPLANE :
      MakeWireOnPlane();                     // Построение каркаса на плоскости
      break;

    case CM_MAKETRIANGLEPOINTFRAME :
      MakeTriangulationPoints();             // Построение каркаса из линий триангуляции
      break;

    case CM_MAKETRIANGLEWIREFRAME :
      MakeTriangulationWire();               // Построение каркаса из линий триангуляции
      break;

    case CM_MAKETRIANGLEPLANE :
      MakeTriangulationPlates();             // Построение каркаса из пластин триангуляции
      break;

    case CM_PRINTPLOTT :
      UpPlotFlag();                         // Поднятие флага
      break;

    case CM_PRINTBITMAP :
      UpRastrFlag();                        // Поднятие флага
      break;

    case CM_MANUFACTURING :
      SolidRebuildTimeFunction();           // График перестроения тела
      break;

    case CM_CURVATUREPROFILE:
      ::CurveCurvatureProfile();            // Эпюра кривизны кривой, построенная на кривой.
      break;

    case CM_CURVECURVATURE:
      ::PlotCurveCurvature();               // График кривизны кривой
      break;
    case CM_CURVETORSION:
      ::PlotCurveTorsion();                 // График кручения кривой
      break;

    case CM_DERIVATIVEPROFILE:
      ::SurfaceDerivativeProfile();         // Эпюра производных поверхности. // The surface derivative profile.
      break;

    // ******************************** Сопряжения сборки

    case  CM_MATE_COINCIDENCE:
      CreateMateConstraint ( GCM_COINCIDENT );  // Создать сопряжение
      break;

    case  CM_MATE_PARALLELISM:
      CreateMateConstraint ( GCM_PARALLEL );  // Создать сопряжение
      break;

    case  CM_MATE_PERPENDICULAR:
      CreateMateConstraint ( GCM_PERPENDICULAR );  // Создать сопряжение
      break;

    case  CM_MATE_TANGENCY:
      CreateMateConstraint ( GCM_TANGENT );  // Создать сопряжение
      break;

    case  CM_MATE_CONCENTRIC:
      CreateMateConstraint ( GCM_CONCENTRIC );  // Создать сопряжение
      break;

    case  CM_MATE_DISTANCE:
      CreateMateConstraint ( GCM_DISTANCE );  // Создать сопряжение
      break;

    case  CM_MATE_ANGLE:
      CreateMateConstraint ( GCM_ANGLE );  // Создать сопряжение
      break;

    // ******************************** Геометрические ограничения

    case  CM_GC_COINCIDENCE:
      AddCoincident2Points();  // СОвпадение двух точек
      break;

    case CM_GC_TANGENCY:
      AddTangency();
      break;

    case CM_GC_PERPENDICULAR:
      AddPerpendicular();
      break;

//CM_GC_PARALLELISM
//CM_GC_CONCENTRIC
//CM_GC_DISTANCE
//CM_GC_ANGLE

    // ******************************** создать размеры

    case CM_RADIUSDIMENSION :
      CreateRadiusDimension3D();
      break;

    // ******************************** ввести лицензионный ключ
    case CM_ENTERLICENSEKEY : 
      EnterLicenseKey();
      break;
   
    // ******************************** показать текущий статус лицензии
    case CM_LICENSE_STATUS :
      ShowLicenseStatus();
      break;

    case CM_DISABLELICENSES: {
      DisableMathModules();
      break;
    }

    default:
      return false;

  }

  return true;
}

#if defined (C3D_WINDOWS) && !defined(ALL_WARNINGS) // _MSC_VER // Set warnings level
#pragma warning(pop) // Restore state of warning settings
#endif
