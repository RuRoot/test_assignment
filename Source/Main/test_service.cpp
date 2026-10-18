#include <test_service.h>
#include <test_manager.h>
#include <test_variables.h>
#include <test_computation.h>
#include <test_converter.h>
#include <test.h>

#ifdef C3D_WINDOWS // _MSC_VER
  #include <windows.h>
#endif // _WIN32

#ifdef __USE_QT__
  #include <qmessagebox.h>
  #include <QMainWindow>
  #include <test_main_dialogs.h>
#endif // __USE_QT__

#ifdef ENABLE_IMPORT_FROM_OB_FILE
  #include "system_cpp_standard.h"

  #ifdef C3D_STANDARD_CXX_17
    #include <conv_ob_container.h>
  #endif
#endif

#include <iostream>
#include <mb_operation_result.h>
#include <gcm_res_code.h>
#include <reference_item.h>
#include <item_registrator.h>
#include <cur_contour3d.h>
#include <surface.h>
#include <surf_plane.h>
#include <space_item.h>
#include <space_instance.h>
#include <solid.h>
#include <polymesh.h>
#include <wire_frame.h>
#include <model_entity.h>
#include <op_swept_parameter.h>
#include <action_curve3d.h>
#include <conv_model_exchange.h>
#include <conv_model_document.h>
#include <conv_exchange_settings.h>
#include <alg_draw.h>
#include <tool_enabler.h>
#include <locale.h>
#include <last.h>

using namespace c3d;


static bool debugMessages = false; // Выдавать отладочные сообщения

// В диалоге открыть/сохранить файл будут одновременно отображаться файлы всех поддерживаемых типов.
// В противном случае тип файла нужно выбирать из выпадающего списка.
// In the file open/save dialog files of all supported extensions will be shown.
// Otherwise the file type should be chosen in a combo box.
static bool allSupportedFormats = true;

#if defined (C3D_WINDOWS) && !defined(ALL_WARNINGS)  // _MSC_VER // Set warnings level
#pragma warning(push)          // Preserve current state of warning settings
#pragma warning(disable: 4244) // warning C4244: a potential loss of data during a type conversion
#endif


//----------------------------------------------------------------------------------------
// Послать строку в виде сообщения
// ---
void ShowString( const TCHAR * str )
{
  size_t strLen = _tcslen(str);
  TestVariables::waitFlag = (strLen > 0) ? 1 : -1;
  { // Очистка поля вывода
    size_t clearLen = 2 * strLen;
    clearLen = std_max( (size_t)256, clearLen );
    string_t clearStr;
    for ( size_t k = 0; k < clearLen; k++ )
      clearStr.append( _T(" ") );

    SetStatusBarText( clearStr.c_str() );
  }

  SetStatusBarText( str );
}


//----------------------------------------------------------------------------------------
// Ввод точки
// ---
static void GetIntPoint( MbCartPoint3D & pnt )
{
  if ( ::fabs( TestVariables::delta_point ) > EXTENT_EQUAL ) {
    double g;
    int i;

    g = pnt.x / TestVariables::delta_point;
    if (pnt.x>=0)   i = (int)( g + 0.5 );
    else            i = (int)( g - 0.5 );
    pnt.x = i*TestVariables::delta_point;

    g = pnt.y / TestVariables::delta_point;
    if (pnt.y>=0)   i = (int)( g + 0.5 );
    else            i = (int)( g - 0.5 );
    pnt.y = i*TestVariables::delta_point;

    g = pnt.z / TestVariables::delta_point;
    if (pnt.z>=0)   i = (int)( g + 0.5 );
    else            i = (int)( g - 0.5 );
    pnt.z = i*TestVariables::delta_point;
  }
}


//----------------------------------------------------------------------------------------
// Ввод точки
// ---
bool GetPoint( const TCHAR * strResID, MbCartPoint3D & pnt )
{
  SyncActiveWindowIndex();

  TestVariables::getPointResult = PNT_RES_NONE;
  ShowString( strResID ); //ShowResString( strResID );

  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
  if ( wind != nullptr ) {
    wind->ExecuteGetPointProc(); // Выполнить процесс обработки сообщений ввода точки

    ShowString( _T("") );

    switch ( TestVariables::getPointResult )
    {
      case PNT_RES_USUAL : {
        pnt = TestVariables::cursorPoint;
        return true;
      }
      case PNT_RES_SHIFT : { // Привязка к ближайшей точке в проекции
        double d = 4*SENSE_RADIUS;//wind->ScaleDistance( SENSE_RADIUS );
        if ( !wind->FindNearestSpaceProject( st_SpaceItem, TestVariables::cursorAxis, std_min( TestVariables::delta_point, d ), TestVariables::cursorPoint, pnt ) ) {
          pnt = TestVariables::cursorPoint;
          GetIntPoint( pnt );
        }
        return true;
      }
      case PNT_RES_CTRL : { // Привязка к ближайшей точке объекта   //-V112
        double d = 4*SENSE_RADIUS;//wind->ScaleDistance( SENSE_RADIUS );
        if ( !wind->FindNearestSpacePoint( st_SpaceItem, TestVariables::cursorAxis, std_min(TestVariables::delta_point,d), TestVariables::cursorPoint, pnt ) ) {
          pnt = TestVariables::cursorPoint;
          GetIntPoint( pnt );
        }
        return true;
      }
      default: {
        return false;
      }
    }
  }
  return false;
} // GetPoint


//----------------------------------------------------------------------------------------
// Ввод точки
// ---
bool GetPoint( const TCHAR * nStr, MbCartPoint & pnt, MbPlacement3D * dstPlace )
{
  SyncActiveWindowIndex();

  TestVariables::getPointResult = PNT_RES_NONE;
  ShowString( nStr );

  if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() )
  {
    double d = SENSE_RADIUS;//wind->ScaleDistance( SENSE_RADIUS );
    wind->ExecuteGetPointProc(); // Выполнить процесс обработки сообщений ввода точки

    ShowString( _T("") );

    switch ( TestVariables::getPointResult )
    {
      case PNT_RES_USUAL:
      {
        // return TestVariables::viewManager->GetCplanePoint( TestVariables::cursorPoint, pnt );
        // ниже основное содержимое GetCplanePoint

        bool b = false;

        MbPlacement3D pl( wind->GetOwnPlace() );

        if ( dstPlace != nullptr )
          b = dstPlace->DirectPointProjection( TestVariables::cursorPoint, wind->GetAxisZ(), pnt.x, pnt.y );
        else
          b = pl.DirectPointProjection( TestVariables::cursorPoint, wind->GetAxisZ(), pnt.x, pnt.y );

        return b;
      }

      case PNT_RES_SHIFT:
      { // Привязка к ближайшей точке в проекции
        bool b = wind->FindNearestPlaneProject( pt_PlaneItem, TestVariables::cursorAxis, std_min(TestVariables::delta_point,d), TestVariables::cursorPoint, pnt );
        if ( !b ) {
          MbCartPoint3D p3d;
          b = wind->FindNearestSpaceProject( st_SpaceItem, TestVariables::cursorAxis, std_min(TestVariables::delta_point,d), TestVariables::cursorPoint, p3d );
          if ( b ) {
            b = false;

            MbPlacement3D pl( wind->GetOwnPlace() );

            if ( dstPlace != nullptr )
              b = dstPlace->DirectPointProjection( TestVariables::cursorPoint, wind->GetAxisZ(), pnt.x, pnt.y );
            else
              b = pl.DirectPointProjection( TestVariables::cursorPoint, wind->GetAxisZ(), pnt.x, pnt.y );

            return b;
          }
        }
        return b;
      }

      case PNT_RES_CTRL: // Привязка к ближайшей точке объекта   //-V112
      {
        bool b = wind->FindNearestPlanePoint( pt_PlaneItem, TestVariables::cursorAxis, std_min(TestVariables::delta_point,d), TestVariables::cursorPoint, pnt );
        if ( !b ) {
          MbCartPoint3D p3d;
          b = wind->FindNearestSpacePoint( st_SpaceItem, TestVariables::cursorAxis, std_min(TestVariables::delta_point,d), TestVariables::cursorPoint, p3d );
          if ( b ) {
            b = false;

            MbPlacement3D pl( wind->GetOwnPlace() );

            if ( dstPlace != nullptr )
              b = dstPlace->DirectPointProjection( TestVariables::cursorPoint, wind->GetAxisZ(), pnt.x, pnt.y );
            else
              b = pl.DirectPointProjection( TestVariables::cursorPoint, wind->GetAxisZ(), pnt.x, pnt.y );

            return b;
          }
        }
        return b;
      }

      default:
        return false;
    } // switch
  }
  return false;
} // GetPoint


//----------------------------------------------------------------------------------------
// Дать объект
// ---
void CommonGetObj( const TCHAR * nStr )
{
  SyncActiveWindowIndex();

  TestVariables::getPointResult = PNT_RES_NONE;
  ShowString( nStr );

  if ( WorkWindow * activeWindow = TestVariables::viewManager->GetActiveWindow() )
    activeWindow->ExecuteGetPointProc();  // Выполнить процесс обработки сообщений ввода точки

  ShowString( _T("") );
}


//----------------------------------------------------------------------------------------
// Указание элемента модели
// ---
bool GetObj( const TCHAR * nStr, MbItem ** find, PathMatrix & pathFrom )
{
  CommonGetObj( nStr );

  if ( TestVariables::getPointResult == 1 ) {
    MbRefItem * geom = nullptr;
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    if ( wind != nullptr ) {
      if ( wind->FindGeometricItem( st_SpaceItem, tt_Undefined, pt_PlaneItem, TestVariables::cursorAxis, SENSE_RADIUS, find, &geom, pathFrom ) )
        return true;
    }
  }

  return false;
} // GetObj


//----------------------------------------------------------------------------------------
// Указание объекта модели или его элемента(части)
// ---
bool GetObj( const TCHAR * nStr, MbItem ** find, MbRefItem ** item, PathMatrix & pathFrom )
{
  bool result = false;

  CommonGetObj( nStr );

  if ( TestVariables::getPointResult == 1 || TestVariables::getPointResult == 3 ) {
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    if ( wind != nullptr ) {
      if ( wind->FindGeometricItem( st_SpaceItem, tt_TopItem, pt_PlaneItem, TestVariables::cursorAxis, SENSE_RADIUS, find, item, pathFrom ) ) {
        if ( TestVariables::getPointResult == 1 )
          *item = nullptr;
        result = true;
      }
    }
  }

  return result;
}


//----------------------------------------------------------------------------------------
// Указание ближайшей к курсору кривой модели
// ---
bool GetAnyCurve( const TCHAR * nStr, MbItem ** find, PathMatrix & pathFrom )
{
  CommonGetObj( nStr );

  if ( TestVariables::getPointResult == 1 || TestVariables::getPointResult == 3 )
  {
    MbRefItem * geom = nullptr;
    if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() )
    {
      if ( wind->FindGeometricItem( st_Curve3D, tt_Undefined, pt_Curve, TestVariables::cursorAxis, SENSE_RADIUS, find, &geom, pathFrom ) )
        return true;
    }
  }
  return false;
} // GetAnyCurve


//----------------------------------------------------------------------------------------
// Указание сборки или вставки.
// ---
bool GetRigidObj( const TCHAR * nStr, MbItem *& find, MbRefItem *& geom, PathMatrix & pathFrom )
{
  CommonGetObj( nStr );

  if ( TestVariables::getPointResult == 1 ) {
    geom = nullptr;
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    if ( wind != nullptr ) {
      // Ищем сборочную единицу. // Find assembly.
      if ( wind->FindGeometricItem(st_Assembly, tt_Undefined, pt_Undefined, TestVariables::cursorAxis, SENSE_RADIUS, &find, &geom, pathFrom ) ) {
        if ( find != nullptr && find->IsA() == st_Assembly )
          return true;
      }
      // Ищем вставку. // Find instance.
      if ( wind->FindGeometricItem(st_Instance, tt_Undefined, pt_Undefined, TestVariables::cursorAxis, SENSE_RADIUS, &find, &geom, pathFrom ) ) {
        if ( find != nullptr && find->IsA() == st_Instance )
          return true;
      }
    }
  }
  return false;
}

//----------------------------------------------------------------------------------------
// Указание произвольного 3D-объекта
// ---
const MbRefItem * GetSObj( const TCHAR * nStr, const MbItem ** find, PathMatrix & pathFrom )
{
  CommonGetObj( nStr );

  if ( TestVariables::getPointResult == PNT_RES_USUAL
    || TestVariables::getPointResult == PNT_RES_SHIFT )
  {
    if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() )
    {
      MbItem * item = nullptr;
      MbRefItem * refObj = nullptr;
      if ( wind->FindGeometricItem( st_SpaceItem, tt_TopItem, pt_Undefined, TestVariables::cursorAxis
                                  , SENSE_RADIUS, &item, &refObj, pathFrom ) )
      {
        *find = item;
        return refObj;
      }
    }
  }

  pathFrom.first.clear();
  pathFrom.second = MbMatrix3D::identity;
  *find = nullptr;
  return nullptr;
}

//----------------------------------------------------------------------------------------
// Указание элемента модели по его типу
// ---
bool GetGObj( const TCHAR * nStr, MbeSpaceType sType, MbItem ** find, PathMatrix & pathFrom )
{
  CommonGetObj( nStr );

  if ( TestVariables::getPointResult == PNT_RES_USUAL )
  {
    MbRefItem * geom = nullptr;
    if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() )
    {
      if ( wind->FindGeometricItem( sType, tt_Undefined, pt_Undefined, TestVariables::cursorAxis, SENSE_RADIUS, find, &geom, pathFrom ) )
        return true;
    }
  }

  return false;
} // GetGObj

//----------------------------------------------------------------------------------------
// Указание элемента модели по его типу
//---
SPtr<MbItem> GetGObj( const TCHAR* nStr, MbeSpaceType sType, PathMatrix & pathFrom )
{
  MbItem * find = nullptr;
  if ( GetGObj(nStr, sType, &find, pathFrom) )
  {
    return SPtr<MbItem>( find );
  }
  return SPtr<MbItem>();
}

//----------------------------------------------------------------------------------------
// Указание элемента модели по его типу и типу полигона
// ---
bool GetGObj( const TCHAR * nStr, MbeSpaceType sType,
              MbItem ** find, MbSpaceItem ** geom, PathMatrix & pathFrom )
{
  CommonGetObj( nStr );

  if ( TestVariables::getPointResult == 1 || TestVariables::getPointResult == 3 )
  {
    MbRefItem * item = nullptr;
    if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() )
    {
      if ( wind->FindGeometricItem( sType, tt_Undefined, pt_Undefined, TestVariables::cursorAxis, SENSE_RADIUS, find, &item, pathFrom ) )
      {
        if ( (item != nullptr) && (item->RefType() == rt_SpaceItem) )
        {
          *geom = static_cast<MbSpaceItem*>( item );
          return true;
        }
      }
    }
  }

  return false;
} // GetGObj


//----------------------------------------------------------------------------------------
// Указание элемента модели по его типу и типу полигона
// ---
bool GetTObj( const TCHAR * nStr, MbeSpaceType sType, MbeTopologyType tType,
              MbItem ** find, MbTopItem ** item, MbMatrix3D & matr )
{
  CommonGetObj( nStr );

  if ( TestVariables::getPointResult == 1 || TestVariables::getPointResult == 3 )
  {
    if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() )
    {
      if ( wind->FindNearestItem( sType, tType, TestVariables::cursorAxis, SENSE_RADIUS, find, item, matr ) )
        return true;
    }
  }

  return false;
} // GetGObj


//----------------------------------------------------------------------------------------
// Указание элемента модели по его типу и типу полигона
// ---
bool GetTObj( const TCHAR * nStr, MbeSpaceType sType, MbeTopologyType tType,
              MbItem ** find, MbTopItem ** item, PathMatrix & pathFrom )
{
  CommonGetObj( nStr );

  if ( TestVariables::getPointResult == 1 || TestVariables::getPointResult == 3 )
  {
    if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() )
    {
      if ( wind->FindNearestItem( sType, tType, TestVariables::cursorAxis, SENSE_RADIUS, find, item, pathFrom ) )
        return true;
    }
  }

  return false;
} // GetGObj


//----------------------------------------------------------------------------------------
// Указание кривой. Работает как с геометрическими объектами, так и с топологическими (ребра).
// На текущий момент значение параметра part определяется тем, нажал ли пользователь клавишу shift
// при выборе объекта.
// ---
bool GetCurveObject( const TCHAR * nStr, bool & part, const MbeSpaceType sGeomType, const MbeSpaceType sTopologyType, const MbeTopologyType tType,
                     MbItem ** find, MbRefItem ** item, PathMatrix & pathFrom, MbMatrix3D & matr )
{
  CommonGetObj( nStr );

  if ( (TestVariables::getPointResult == PNT_RES_USUAL) || (TestVariables::getPointResult == PNT_RES_SHIFT) ) {
    part = ( TestVariables::getPointResult == PNT_RES_SHIFT );
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    if ( wind != nullptr ) {
      if ( wind->FindGeometricItem(sGeomType, tt_Undefined, pt_Undefined, TestVariables::cursorAxis, SENSE_RADIUS, find, item, pathFrom) )
        return true;

      if ( wind->FindNearestItem(sTopologyType, tType, TestVariables::cursorAxis, SENSE_RADIUS, find, (MbTopItem**)item, matr) )
        return true;
    }
  }

  return false;
}


//----------------------------------------------------------------------------------------
// Указание элемента модели по его типу
// ---
bool GetMObj( const TCHAR * nStr, MbePlaneType pType, MbItem ** find, PathMatrix & pathFrom )
{
  CommonGetObj( nStr );

  if ( TestVariables::getPointResult == 1 )
  {
    MbRefItem * item = nullptr;
    if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() )
    {
      if ( wind->FindGeometricItem( st_Undefined, tt_Undefined, pType, TestVariables::cursorAxis, SENSE_RADIUS, find, &item, pathFrom ) )
        return true;
    }
  }

  return false;
} // GetGObj


//----------------------------------------------------------------------------------------
// Указание элемента модели по его типу и типу полигона
// ---
bool GetMObj( const TCHAR * nStr, MbePlaneType pType,
              MbItem ** find, MbPlaneItem ** geom, PathMatrix & pathFrom )
{
  CommonGetObj( nStr );

  if ( TestVariables::getPointResult == 1 || TestVariables::getPointResult == 3 )
  {
    MbRefItem * item = nullptr;
    if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() )
    {
      if ( wind->FindGeometricItem( st_Undefined, tt_Undefined, pType, TestVariables::cursorAxis, SENSE_RADIUS, find, &item, pathFrom ) )
      {
        if ( (item != nullptr) && (item->RefType() == rt_PlaneItem) ) {
          *geom = (MbPlaneItem *)item;
          return true;
        }
      }
    }
  }
  return false;
} // GetGObj

//----------------------------------------------------------------------------------------
// Выбрать на экране экземпляр геометрии с учетом повторно-используемых тел.
// Select a geom object, preferably an instance of the re-inserted solid of the model.
//---
MbItem * GetInstance( const TCHAR * nStr, PathMatrix & pathFrom )
{
  MbItem * obj = 0;
  if ( GetObj(nStr, &obj, pathFrom) )
  {
    if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() )
    {
      const MbModel & model = wind->GetGeomModel();
      MbPath reducedPath( pathFrom.first );
      reducedPath.pop_back();
      MbMatrix3D m;
      if ( const MbItem * item = model.GetItemByPath( reducedPath, m ) )
      {
        if ( item->IsA() == st_Instance )
        {
          pathFrom.first = reducedPath;
          pathFrom.second = m;
          return const_cast<MbItem*>( item );
        }
      }
    }
  }

  return obj;
}

//----------------------------------------------------------------------------------------
// Нахождение ближайшего элемента к заданной точке
// ---
bool FindNearestObj( MbCartPoint3D & from_p, MbItem ** find, PathMatrix & pathFrom )
{
  MbRefItem * item = nullptr;
  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
  if ( wind != nullptr ) {
    MbFloatAxis3D cAxis( from_p, wind->GetAxisZ() );
    if ( wind->FindGeometricItem( st_SpaceItem, tt_Undefined, pt_PlaneItem, cAxis, SENSE_AREA, find, &item, pathFrom ) )
      return true;
  }
  return false;
} // FindNearestObj


//----------------------------------------------------------------------------------------
// Нахождение ближайшего элемента к заданной точке
// ---
bool FindNearestObj( MbCartPoint & from_p, MbePlaneType pType, MbItem ** find, PathMatrix & pathFrom )
{

  MbRefItem * item = nullptr;
  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
  if ( wind != nullptr ) {
    // SD#776923: Был получена точка конструктивной плоскости bool GetPoint( const TCHAR * nStr, MbCartPoint & pnt )
    // POINT mp =  wind->GetDevicePoint( from_p );
    // MbCartPoint3D cPoint( wind->GetCartPoint( mp ) );
    // А код дает точку в ЛСК экрана
    MbCartPoint3D cPoint;
    MbPlacement3D cPlace( wind->GetOwnPlace() );
    cPlace.PointOn( from_p, cPoint );

    MbFloatAxis3D cAxis( cPoint, wind->GetAxisZ() );
    if ( wind->FindGeometricItem( st_Undefined, tt_Undefined, pType, cAxis, SENSE_AREA, find, &item, pathFrom ) )
      return true;
  }
  return false;
} // FindNearestObj


//----------------------------------------------------------------------------------------
// Нахождение ближайшего элемента к заданной точке
// ---
bool FindNearestObj( MbCartPoint3D & from_p, MbePlaneType pType, MbItem ** find, PathMatrix & pathFrom )
{
  MbRefItem * item = nullptr;
  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
  if ( wind != nullptr ) {
    MbFloatAxis3D cAxis( from_p, wind->GetAxisZ() );
    if ( wind->FindGeometricItem( st_Undefined, tt_Undefined, pType, cAxis, SENSE_AREA, find, &item, pathFrom ) )
      return true;
  }
  return false;
} // FindNearestObj


//----------------------------------------------------------------------------------------
// Нахождение ближайшего элемента к заданной точке
// ---
bool FindNearestObj( MbCartPoint3D & from_p, MbeSpaceType sType, MbItem ** find, PathMatrix & pathFrom )
{
  MbRefItem * item = nullptr;
  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
  if ( wind != nullptr ) {
    MbFloatAxis3D cAxis( from_p, wind->GetAxisZ() );
    if ( wind->FindGeometricItem( sType, tt_Undefined, pt_Undefined, cAxis, SENSE_AREA, find, &item, pathFrom ) )
      return true;
  }
  return false;
} // FindNearestObj


//----------------------------------------------------------------------------------------
// Указание элемента модели по его типу и типу полигона
// ---
bool FindNearestObj( MbCartPoint3D & from_p, MbeSpaceType sType, MbeTopologyType tType,
                     MbItem ** find, MbTopItem ** item, MbMatrix3D & matr )
{
  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
  if ( wind != nullptr ) {
    MbFloatAxis3D cAxis( from_p, wind->GetAxisZ() );
    if ( wind->FindNearestItem( sType, tType, cAxis, SENSE_RADIUS, find, item, matr ) )
      return true;
  }
  return false;
} // FindNearestObj


//----------------------------------------------------------------------------------------
// Установка проекции
// ---
void SetProjection()
{
  SyncActiveWindowIndex();

  WorkWindow *currentView = TestVariables::viewManager->GetActiveWindow();

  MbPlacement3D placeTmp( currentView->GetOwnPlacement() );
  MbMatrix3D tmpMatrix = placeTmp.GetMatrixInto(); // Берем обратную матрицу
  TestVariables::setProj = false;

  ExecuteSetProjectionDlg( tmpMatrix );

  if ( TestVariables::setProj ) {
    MbCartPoint3D origin = currentView->GetOrigin();
    MbMatrix3D divMatr;
    tmpMatrix.Div( divMatr );

    MbVector3D x = divMatr.GetColumn( 0 );
    MbVector3D z = divMatr.GetColumn( 2 );

    // Очистить проекции модели в активном виде
    ptrdiff_t index = TestVariables::viewManager->GetActiveWindowIndex();
    TestVariables::viewManager->EraseWindow( index );
    TestVariables::viewManager->RefreshWindow( index );
    currentView->InitPlacement( origin, z, x );
    currentView->CalculateMatrix();

    TestVariables::viewManager->DrawContentWindow( index );
  }
} // SetProjection


//----------------------------------------------------------------------------------------
// Изменить глубину проекции
// ---
bool SetDepth()
{
  double depth = 0;

  if ( GetDouble( IDS_ENTER_DEPTH_INCREM, depth ) ) {
    SyncActiveWindowIndex();

    WorkWindow *currentView = TestVariables::viewManager->GetActiveWindow();
    if ( currentView ) {
      MbCartPoint3D origin = currentView->GetOrigin();
      origin.z += depth;
      currentView->SetOrigin( origin );
    }
    return true;
  }
  return false;
}


//----------------------------------------------------------------------------------------
// Установить масштаб вида
// ---
bool SetScale()
{
  SyncActiveWindowIndex();

  WorkWindow * currentView = TestVariables::viewManager->GetActiveWindow();
  if ( currentView ) {
    double scale = currentView->GetScale(); // Выдать текущий масштаб вида

    if ( GetDouble( IDS_ENTER_MAP_SCALE, scale ) ) {
      currentView->SetScale( scale );
      ptrdiff_t index = TestVariables::viewManager->GetActiveWindowIndex();
      // Очистить проекции модели в активном виде
      TestVariables::viewManager->EraseWindow( index );
      TestVariables::viewManager->InvalidateWindow( index, true );

      return true;
    }
  }
  return false;
}


//----------------------------------------------------------------------------------------
// Ввод булевского значения
// ---
void GetBoolean( const TCHAR * nPrompt, bool & bVal, bool resetToFalse )
{
  if ( resetToFalse )
    bVal = false;

  bVal = GetBool( nPrompt, bVal );
}


//----------------------------------------------------------------------------------------
// Ввод булевского значения
// ---
bool GetBoolean( const TCHAR * nPrompt ) {
  return GetBool( nPrompt, true/*defVal*/ );
}


//----------------------------------------------------------------------------------------
// Найти первую '.' с конца строки
//---
static
const TCHAR * FindLastDot( const TCHAR * str )
{
  const TCHAR * c = nullptr;

  if ( str != nullptr ) {
    c = str;

    // Найти конец строки
    while ( *c != _T('\0') ) {
      c++;
    }
    // Найти первую '.' с конца строки
    const TCHAR * s = c;
    while ( *s != _T('.') && (s != str) ) {
      s--;
    }
    if ( *s == _T('.') )
      c = s;
  }

  return c;
}


//----------------------------------------------------------------------------------------
// Перевести в верхний регистр.
//---
static void ToUpper( c3d::string_t& s ) {
  std::use_facet<std::ctype<TCHAR>>(std::locale()).toupper( &s[0], &s[0] + s.size() );
}


//----------------------------------------------------------------------------------------
// Есть ли такое расширение в имени
//---
bool SameExtension( const TCHAR * fullname, const TCHAR * extname )
{
  bool res = false;

  if ( (fullname != nullptr) && (extname != nullptr) ) {
    const TCHAR * checkExtName = ::FindLastDot( fullname );
    if ( checkExtName != nullptr ) {
      if ( _tcscmp( checkExtName, extname ) == 0 )
        res = true;
      else {
        string_t EXTname( extname );
        ToUpper( EXTname );
        if ( _tcscmp( checkExtName, EXTname.c_str() ) == 0 )
          res = true;
      }
    }
  }

  return res;
}


//----------------------------------------------------------------------------------------
// Есть ли такое расширение в имени
//---
inline
const TCHAR * FindExtname( const TCHAR * fullname, const TCHAR * extname )
{
  const TCHAR * foundExtName = nullptr;

  if ( fullname != nullptr ) {
    if ( (extname != nullptr) && (_tcslen(extname) > 1) ) {
      const TCHAR * checkExtName = ::FindLastDot( fullname );

      if ( (checkExtName != nullptr) && (_tcslen(checkExtName) > 1) ) {
        if ( _tcscmp( fullname, ALL_EXT ) != 0 )
          foundExtName = _tcsstr( checkExtName, extname );

#ifdef C3D_WINDOWS // _MSC_VER
        if ( foundExtName == nullptr && (_tcscmp( fullname, ALL_EXT ) != 0) ) {
          string_t EXTname( extname );
          ToUpper( EXTname );
          foundExtName = _tcsstr( checkExtName, EXTname.c_str() );
        }
#endif
      }
    }
  }

  return foundExtName;
}


//----------------------------------------------------------------------------------------
// Перезаписать или добавить расширение имени файла типа (".ext\0")
//---
void RewriteFileExtension( TCHAR * fullname, const TCHAR * extname, bool forWriting )
{
  PRECONDITION( fullname != nullptr );

  if ( fullname != nullptr && extname != nullptr ) {
    if ( debugMessages )
      MessageBoxEx( fullname, IDS_FILE_NAME_BEFORE_CHANGING_EXT );

    ptrdiff_t count = STRINGLENGTH + 1;

    // Найти конец строки
    TCHAR * c = fullname;
    while ( count-- > 0 && *c != _T('\0') ) {
      c++;
    }

    ptrdiff_t fullNameLen = _tcslen( fullname );
    ptrdiff_t extNameLen = (extname != nullptr) ? _tcslen( extname ) : 0;

    // Осталось ли память буфера для добавления расширения ?
    if ( extNameLen < count ) {
      string_t finalExtName( extname );
      const TCHAR * foundExtName = ::FindExtname( fullname, extname );

      if ( foundExtName == nullptr ) {
        foundExtName = ::FindExtname( fullname, X_T_EXT );
        if ( foundExtName == nullptr )
          foundExtName = ::FindExtname( fullname, X_B_EXT );
        if ( foundExtName != nullptr ) {
          if ( _tcscmp( extname, X_T_EXT ) == 0 )
            finalExtName = foundExtName;
        }
      }
      if ( foundExtName == nullptr ) {
        foundExtName = ::FindExtname( fullname, STP_EXT );
        if ( foundExtName == nullptr )
          foundExtName = ::FindExtname( fullname, STEP_EXT );
        if ( foundExtName != nullptr ) {
          if ( _tcscmp( extname, STP_EXT ) == 0 )
            finalExtName = foundExtName;
        }
      }
      if ( foundExtName == nullptr ) {
        foundExtName = ::FindExtname( fullname, JT_EXT );
        if ( foundExtName == nullptr )
          foundExtName = ::FindExtname( fullname, JT_EXT );
        if ( foundExtName != nullptr ) {
          if ( _tcscmp( extname, JT_EXT ) == 0 )
            finalExtName = foundExtName;
        }
      }
      if ( foundExtName == nullptr ) {
        foundExtName = ::FindExtname( fullname, IGS_EXT );
        if ( foundExtName == nullptr )
          foundExtName = ::FindExtname( fullname, IGES_EXT );
        if ( foundExtName != nullptr ) {
          if ( _tcscmp( extname, IGS_EXT ) == 0 )
            finalExtName = foundExtName;
        }
      }
      if ( foundExtName == nullptr ) {
        foundExtName = ::FindExtname( fullname, SAT_EXT );
        if ( foundExtName == nullptr )
          foundExtName = ::FindExtname( fullname, SAB_EXT );
        if ( foundExtName != nullptr ) {
          if ( _tcscmp( extname, SAT_EXT ) == 0 )
            finalExtName = foundExtName;
          else if ( _tcscmp( extname, SAB_EXT ) == 0 )
            finalExtName = foundExtName;
        }
      }
      if ( foundExtName == nullptr ) {
        foundExtName = ::FindExtname( fullname, ASC_EXT );
        if ( foundExtName == nullptr ) {
          foundExtName = ::FindExtname( fullname, XYZ_EXT );
          if ( foundExtName == nullptr )
            foundExtName = ::FindExtname( fullname, TXT_EXT );
          }
        if( foundExtName != nullptr ) {
          if( _tcscmp( extname, ASC_EXT ) == 0 )
            finalExtName = foundExtName;
          else if( _tcscmp( extname, XYZ_EXT ) == 0 )
            finalExtName = foundExtName;
          else if( _tcscmp( extname, TXT_EXT ) == 0 )
            finalExtName = foundExtName;
        }
      }

      if ( foundExtName == nullptr ) {
        foundExtName = ::FindExtname( fullname, C3D_EXT );
        if ( foundExtName == nullptr )
          foundExtName = ::FindExtname( fullname, MAT_EXT );
        if ( foundExtName == nullptr )
          foundExtName = ::FindExtname( fullname, STL_EXT );
        if ( foundExtName == nullptr )
          foundExtName = ::FindExtname( fullname, VRML_EXT );
        if ( foundExtName == nullptr )
          foundExtName = ::FindExtname( fullname, OBJ_EXT );
        if ( foundExtName == nullptr )
          foundExtName = ::FindExtname( fullname, THREEMF_EXT );
        if ( foundExtName == nullptr )
          foundExtName = ::FindExtname( fullname, GRDECL_EXT );
      }

      if ( (foundExtName != nullptr) || ((fullNameLen + extNameLen) > STRINGLENGTH) ) {
        { // Найти первую '.' с конца строки
          TCHAR * s = c;
          while ( *s != _T('.') && (s != fullname) ) {
            s--;
          }
          if ( *s == _T('.') )
            c = s;
        }

        // Перебить расширение файла
#ifdef C3D_WINDOWS // _MSC_VER
;//        finalExtName.to_lower();
#endif
        if ( foundExtName == nullptr )
          _tcscpy( c, finalExtName.c_str() );
        else {
          if ( _tcscmp( finalExtName.c_str(), NON_EXT ) != 0 ) // если расширение известно
            _tcscpy( c, finalExtName.c_str() ); // устанавливаем расширение
          else { // если расширение неизвестно
            if ( forWriting ) // то при записи
              _tcscpy( c, NON_EXT ); // удаляем расширение
            else // а при чтении
              _tcscpy( c, foundExtName ); // сохраняем расширение
          }
        }
      }
      else { // Ничего не надо перебивать, т.к. наших расширений в имени нет, поэтому просто добавить расширение
        if ( forWriting )
          _tcscat( fullname, extname );
        else if ( _tcscmp( fullname, ALL_EXT ) != 0 )
          _tcscat( fullname, extname );
        else
          _tcscpy( fullname, NON_EXT ); // чтение с несколькими расширениям
      }
    }

    if ( debugMessages )
      MessageBoxEx( fullname, IDS_FILE_NAME_AFTER_CHANGING_EXT );
  }
}


//#define __DEBUG_MODEL_TREE__
//----------------------------------------------------------------------------------------
// Прочитать файл C3D средствами Modeler
// ---
static bool ReadC3D( const TCHAR * fileName, WorkWindow * wind )
{
  bool bRes {false};
  uint32 readFlag = 0;
  MbAccurateTimer ac;
  if ( ::SameExtension( fileName, C3D_EXT ) ) { // C3D
    std::unique_ptr<reader_ex> pl = reader_ex::CreateReaderEx( std::unique_ptr<iobuf_Seq>(&::createiobuf( fileName )), io::in );
    std::unique_ptr<IProgressIndicator> progBar{ &::CreateProgressIndicator( new StrSpyCommon ) };
    pl->InitProgress( progBar.get() );
    const bool isGoodRead = pl->good();
    
    if ( isGoodRead ) {
      SetWaitCursor( true );

      ac.Begin();

      wind->ReadModel( *pl );

      ac.End();

      SetWaitCursor( false );

#ifdef __DEBUG_MODEL_TREE__
      const IModelTree* tree = pl.GetModelTree();
      if ( tree ) {
        TCHAR nameBuff[257];
        _tcscpy ( nameBuff, fileName );

        RewriteFileExtension ( nameBuff, _T("_read.xml") );

        t_ofstream treeFile(c3d::C3DToPathstring(nameBuff ? c3d::string_t(nameBuff) : c3d::string_t()).c_str());
        if ( treeFile.is_open() )
          treeFile << *tree;
      }
#endif
    }
    readFlag = pl->state();
    bRes = pl->good();
    pl->FinishProgress();
  }
  if ( bRes )
  {
    if ( ( readFlag & io::skippedUnknAttr ) != 0 ) {
      MessageBoxEx( RT_FILE_WITH_UNKNOWN_ATTR, IDS_TITLE_WRN );
    }
    else if ( ( readFlag & io::skippedUnknown ) != 0 ) {
      MessageBoxEx( RT_FILE_WITH_UNKNOWN_OBJECTS, IDS_TITLE_WRN );
    }
    else if ( ( readFlag & io::verViolation ) != 0 ) {
      MessageBoxEx( RT_WAR_FILE_VERSION, IDS_TITLE_WRN );
    }

    if ( TestVariables::checkReadTime ) {
      const size_t bufLen = 1024;
      TCHAR strBuf[bufLen];
      _sntprintf( strBuf, bufLen, RT_TIME_SEC, ac.GetLast() );
      ::MessageBoxEx( strBuf, IDS_INFORMATION );
    }

  }
  else
  {
    if ( (readFlag & io::cantReadCatalog) != 0 )
      MessageBoxEx( RT_FILE_FAIL_READ_CATALOG, IDS_TITLE_ERR );
    else if ( ( readFlag & io::badData ) != 0)
      MessageBoxEx( RT_FILE_BAD_FORMAT, IDS_TITLE_ERR );
    else if ( (readFlag & io::badClassId) != 0 )
      MessageBoxEx( RT_FILE_BAD_CLASSID, IDS_TITLE_ERR );
    else if ( ( readFlag & io::outOfMemory ) != 0 ) {
      MessageBoxEx( RT_FILE_OUT_OF_MEM, IDS_TITLE_ERR );
    }
    else if ( ( readFlag & io::emptyFile ) != 0 ) {
      MessageBoxEx( RT_NULL_FILE, IDS_TITLE_ERR );
    }
    else if ( ( readFlag & io::notFound ) != 0 ) {
      MessageBoxEx( RT_FILE_NOT_FOUND, IDS_TITLE_ERR );
    }
    else if ( ( readFlag & io::verViolation ) != 0 ) {
      MessageBoxEx( RT_ERR_FILE_VERSION, IDS_TITLE_ERR );
    }
    else if ( ( readFlag & io::licenseMissing ) != 0 ) {
      MessageBoxEx( RT_ERR_LICENSE, IDS_TITLE_ERR );
    }
    else
      MessageBoxEx( RT_FILE_NOT_OPEN, IDS_TITLE_ERR );
  }
  return bRes;
}


//----------------------------------------------------------------------------------------
// Прочитать MAT
// ---
static bool ReadMAT( const TCHAR * fileName, WorkWindow * wind )
{
  bool bRes {false};
  if ( ::SameExtension( fileName, MAT_EXT ) )
  { // MAT (устаревший, только чтение)
    MbAccurateTimer ac;
    std::unique_ptr<reader> pl = reader::CreateReader( std::unique_ptr<iobuf_Seq>(&::createiobuf(fileName)), io::in );
    bRes = pl->good();

    if ( bRes ) {
      SetWaitCursor( true );

      ac.Begin();

      bool isRead = wind->ReadItem( *pl );

      ac.End();

      SetWaitCursor( false );

      if ( !isRead )
        MessageBoxEx( RT_ERROR_READING_PARTLY, IDS_TITLE_ERR );
    }
  }
  return bRes;
}


//----------------------------------------------------------------------------------------
// Вывести сообщение с деталями сбоя импорта
// ---
static void ShowReadFailureDetais( MbeConvResType importCode )
{
  switch (importCode)
  {
    case cnv_Success :
    case cnv_PartialSuccess :
    // type nothing
    // Сообщения не требуется
    break;
    case cnv_UserCanceled :
      MessageBoxEx( RT_USER_CANCELED, IDS_TITLE_ERR );
    break;
    case cnv_LicenseNotFound :
      MessageBoxEx( RT_LICENCE_CHECK_FAILURE, IDS_TITLE_ERR );
    break;
    case cnv_UnknownExtension :
      MessageBoxEx( RT_FORMAT_NOT_SUPPORTED, IDS_TITLE_ERR );
    break;
    case cnv_UnsupportedVersion :
      MessageBoxEx( RT_FORMAT_VERSION_NOT_SUPPORTED, IDS_TITLE_ERR );
    break;
    case cnv_CannotLoadFormatSpecificComponent:
      MessageBoxEx( RT_CANNOT_LOAD_FORMAT_SPECIFIC_PLUGIN, IDS_TITLE_ERR );
    break;
    case cnv_NoObjects :
    case cnv_NoBody:
      MessageBoxEx( RT_FILE_NO_OBJECT, IDS_TITLE_ERR );
      break;
    
    case cnv_Error :
    case cnv_FileOpenError :
    case cnv_FileWriteError :
    case cnv_FileDeleteError :
    case cnv_ImpossibleReadAssembly :
    case cnv_NotEnoughMemory :
    default:
      MessageBoxEx( RT_FILE_NOT_OPEN, IDS_TITLE_ERR );
    break;
  }
}

//----------------------------------------------------------------------------------------
// Чтение модели из файла через буффер
// ---
template<class TModel>
static MbeConvResType ImportFromBufferLocal( TModel & modelDoc, ConvConvertorProperty3D& convProperties)
{
  MbeConvResType importCode { cnv_Error };
  if ( std::ifstream is { convProperties.fileName, std::ios::binary | std::ios::ate } ) { 
    std::streampos size = is.tellg();
    size_t sizeT = static_cast< size_t > ( size );
    is.seekg( 0 );
    std::string bufStr( sizeT, '\0' );
    if ( is.read( &bufStr[0], size ) ) {
      is.close();
      C3DExchangeBuffer buffer;
      buffer.Init( bufStr.c_str(), sizeT );
      importCode = ::ImportFromBuffer( modelDoc, buffer, mxf_autodetect, &convProperties );
    }
  }
  return importCode;
}

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

//----------------------------------------------------------------------------------------
// Чтение модели из файла
// ---
void ReadModel( const TCHAR * path )
{
  FatalErrorHandler::Reset();

  const TCHAR * fileName( path );

  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();

  PluginLoader pluginLoader {};

  if ( fileName == nullptr ) {
    

    if ( _GetOpenFileName( TestVariables::nameBuffer, SELECT_FILE_INFO, GetFilenameFilter(&pluginLoader).c_str(), NON_EXT ) ) {
      fileName = TestVariables::nameBuffer;
    }
  }

  if ( wind == nullptr ) {
    wind = new WorkWindow( v_Isometry );
    wind->CreateWorkWindow( IDS_TITLE_VIEWISOMETRY, TestVariables::viewManager->GetBackgrColor(), TestVariables::viewManager->GetLightDirection()/*lightDirection*/ );
    wind->OrientateWindow( v_Isometry );

    ptrdiff_t newIndex = TestVariables::viewManager->AddWindow( wind );
    TestVariables::viewManager->SetFocus( newIndex, false );
  }

  if ( fileName != nullptr ) {
    TestVariables::ResetHLighted();
    const size_t bufLen = 1024;
    TCHAR buf[bufLen];
    size_t len = 0;
    if ( TestVariables::checkReadTime )
      len += (size_t)_sntprintf( buf+len, bufLen-len, RT_TEST_RESULTS );
    
    // MbAccurateTimer ac;
    
    bool isGoodRead { false };
    MbeConvResType importCode{ cnv_Error };

    ConvConvertorProperty3D convProperties;
    convProperties.fileName = c3d::C3DToPathstring( fileName );
    ::UpdateConverterSettings( convProperties );
    AskForImportOptions( ::GetFileFormat( c3d::C3DToPathstring( fileName ) ), convProperties );

    // Код для открытия файлов с форматом "ob" 
    if( TestVariables::importObFileIntoSolid && ::SameExtension( fileName, OB_EXT ) ) {
      #if defined(ENABLE_IMPORT_FROM_OB_FILE) && defined(C3D_STANDARD_CXX_17)
        std::shared_ptr<c3d::ObContainer> obContainer = c3d::ObContainer::ReadFromFile( fileName );
        if( obContainer != nullptr ) {
          c3d::SolidSPtr solid;
          importCode = c3d::ImportSolid( solid, obContainer, &convProperties );
          if( importCode == cnv_Success || importCode == cnv_PartialSuccess ) {
            ::AddContentToManager( *solid, false );
            wind->SetModelName( fileName );
            return;
          }
        }
      #else
        MessageBoxEx( RT_COMPILER_VERSION, IDS_TITLE_ERR );
      #endif

    }

    if ( TestVariables::readToModelDocument ) {
      if ( TestVariables::forceUsePlugin )
      {
        importCode = WrConvertor3D::PluginRead( fileName, pluginLoader );
        isGoodRead = ( cnv_Success == importCode ) || ( cnv_PartialSuccess == importCode );
        ::ShowReadFailureDetais( importCode );
      }
      else //if ( TestVariables::forceUsePlugin )
      {
        if ( allSupportedFormats )
        {
          std::unique_ptr<IProgressIndicator> progBar { &::CreateProgressIndicator( new StrSpyConverters ) };
          C3dModelDocument modelDoc;

          if ( TestVariables::importFromBuffer )
          {
            importCode = ImportFromBufferLocal( modelDoc, convProperties );
          }
          else
          {
            importCode = ::ImportFromFile( modelDoc, convProperties.fileName, &convProperties, progBar.get() );
          }
          if ( ( importCode == cnv_Success ) || ( importCode == cnv_PartialSuccess ) ) {
            SPtr<MbItem> importedContent{ modelDoc.GetContent() };
            isGoodRead = !importedContent.is_null();
            if ( isGoodRead )
            {
              ::AddContentToManager( *importedContent, false );
            }
            else
            {
              importCode = cnv_NoObjects;
            }
          }
          else if ( !TestVariables::importFromBuffer && ( importCode == cnv_Error || importCode == cnv_UnknownExtension || importCode == cnv_UnsupportedVersion ) )
          {
            importCode = WrConvertor3D::PluginRead( fileName, pluginLoader );
            isGoodRead = ( cnv_Success == importCode ) || ( cnv_PartialSuccess == importCode );
          }
          ::ShowReadFailureDetais( importCode );

        } 
      }
    }
    else //if ( TestVariables::readToModelDocument ) {
    {
      if ( ::SameExtension( fileName, C3D_EXT ) ) { // C3D
        isGoodRead = ::ReadC3D( fileName, wind );
      }
      else if ( ::SameExtension( fileName, MAT_EXT ) ) { // MAT (устаревший, только чтение)
        isGoodRead = ::ReadMAT( fileName, wind );
      }
      else
      {
        std::unique_ptr<IProgressIndicator> progBar{ &::CreateProgressIndicator( new StrSpyConverters ) };
        c3d::ItemSPtr itemReadTo;

        if ( TestVariables::importFromBuffer )
        {
          importCode = ImportFromBufferLocal( itemReadTo, convProperties );
        }
        else
          importCode = ::ImportFromFile( itemReadTo, C3DToPathstring( fileName ), &convProperties, progBar.get() );
        if ( (importCode == cnv_Success) || (importCode == cnv_PartialSuccess) )
        {
          isGoodRead = !itemReadTo.is_null();
          if ( isGoodRead )
          {
            ::AddContentToManager( *itemReadTo, false );
          }
        }
        ShowReadFailureDetais( importCode );
      }
    }
    if ( isGoodRead )
    {
      wind->SetModelName( fileName );
    }
  } //if ( fileName != nullptr ) {

  FatalErrorHandler::Reset(); // Сброс ошибок, если они были.
}

//----------------------------------------------------------------------------------------
// Запись модели в файл
// ---
void WriteModel( TCHAR * fileName0, VERSION dstVersion )
{
  if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() )
  {
    bool nameIsReady = false;

    if ( fileName0 != nullptr && (_tcslen(fileName0) > 4) ) {
      if ( ::SameExtension( fileName0, C3D_EXT ) ) { // C3D
        _tcscpy( TestVariables::nameBuffer, fileName0 ); // Имя файла модели.
        nameIsReady = true;
      }
    }
    if ( !nameIsReady ) {
      if ( !wind->IsModelNameEmpty() )
        _tcscpy( TestVariables::nameBuffer, wind->GetModelName() ); // Имя файла модели.

      //if ( _GetSaveFileName( TestVariables::nameBuffer, RT_WRITTING_IN_MODEL_FILE, C3D_EXT_INFO, C3D_EXT ) ) {
      if ( _GetSaveFileName( TestVariables::nameBuffer, RT_WRITTING_IN_MODEL_FILE, nullptr, nullptr ) ) {
        nameIsReady = true;
      }
    }
    if ( nameIsReady ) {
      bool isGoodWrite = false;
      uint32 writeFlag = 0;

      const size_t bufLen = 1024;
      TCHAR strBuf[bufLen];
      size_t len = 0;
      if ( TestVariables::checkWriteTime )
        len += (size_t)_sntprintf( strBuf+len, bufLen-len, RT_TEST_RESULTS );

      MbAccurateTimer ac;

      if ( ::SameExtension( TestVariables::nameBuffer, C3D_EXT ) ) { // C3D
        ac.Begin();

        membuf memBuf;           // Создаем membuf
        {
          bool canUseModelTree = false;

          if ( !::CanWriteToMathFileVersion( dstVersion, &canUseModelTree ) ) {
            C3D_ASSERT_UNCONDITIONAL( false );
            dstVersion = ::GetCurrentMathFileVersion();
          }

          bool saveToPrevVersion = (dstVersion < ::GetCurrentMathFileVersion());

          // \ru Создаем подходящий writer в зависимости от опции
          writer::writer_ptr out = ( TestVariables::useModelTree && canUseModelTree ) // \ru Если заказано сохранение в компактном формате, то используем его.
            ? writer::writer_ptr( writer_ex::CreateMemWriterEx(memBuf, 0).release() )
            : writer::writer_ptr( writer::CreateMemWriter(memBuf, 0) );

          std::unique_ptr<IProgressIndicator> progBar{ &::CreateProgressIndicator( new StrSpyCommon ) };
          out->InitProgress( progBar.get() );

          isGoodWrite = out->good();
          if ( !isGoodWrite )
            writeFlag = out->state();

          if ( isGoodWrite && saveToPrevVersion ) { // C3D-1182
            VersionContainer verCont = out->GetVersionsContainer();
            verCont.SetVersion( 0, dstVersion );
            out->SetVersionsContainer( verCont );
            out->SetStorageVersion( dstVersion );
          }

          if ( isGoodWrite ) {
            SetWaitCursor( true );
            //if ( canUseModelTree )
            //  out->SetFormatVersion( GetPrevExtendedFormatVersion() ); // \ru Проверка сохранения в предыдущую версию расширенного формата. \en Check saving to the previous extended format version.
            wind->WriteModel ( *out );
            memBuf.setMaxRegCount ( out->GetMaxRegisteredCount() ); // \ru Максимальное количество регистрируемых объектов. \en The maximum number of registered facilities.

            if ( out->good() )
              wind->SetModelName( TestVariables::nameBuffer );

#ifdef __DEBUG_MODEL_TREE__
            const IModelTree * tree = out->GetModelTree();
            if ( tree ) {
              TCHAR nameBuff[257];
              _tcscpy ( nameBuff, TestVariables::nameBuffer );

              RewriteFileExtension ( nameBuff, _T("_write.xml") );

              t_ofstream treeFile(c3d::C3DToPathstring(nameBuff ? c3d::string_t(nameBuff) : c3d::string_t()).c_str());
              if ( treeFile.is_open() )
                treeFile << *tree;
            }
#endif

            SetWaitCursor( false );

            isGoodWrite = out->good();
            writeFlag = out->state();
          }
          out->FinishProgress();
        }

        if ( isGoodWrite )
          isGoodWrite = ::writeiobuftodisk( TestVariables::nameBuffer, memBuf ); // Пишем его на диск

        ac.End();
      }
      else {
        MbeConvResType importCode{ cnv_Error };
        const TCHAR * fileName = TestVariables::nameBuffer;
        ConvConvertorProperty3D convProperties;
        convProperties.fileName = c3d::C3DToPathstring(fileName);
        ::UpdateConverterSettings(convProperties);
        AskForExportOptions(::GetFileFormat(c3d::C3DToPathstring(fileName)), convProperties);
        if (TestVariables::exportIntoBuffer)
        {
          importCode = ::WriteIntoBuffer(fileName, convProperties);
        }

        else
        {
          SPtr<MbItem> content(::ModelToContent(TestVariables::viewManager->GetActiveWindow()->SetGeomModel()));
          C3dModelDocument modelDoc;
          modelDoc.SetContent(content);
          std::unique_ptr<IProgressIndicator> progBar{&::CreateProgressIndicator(new StrSpyConverters)};
          importCode = ::ExportIntoFile(modelDoc, convProperties.fileName, &convProperties, progBar.get());
        }

        if ((importCode == cnv_Success) || (importCode == cnv_PartialSuccess))
          isGoodWrite = true;
      }

      if ( TestVariables::checkWriteTime ) {
        len += (size_t)_sntprintf( strBuf+len, bufLen-len, RT_TIME_SEC, ac.GetLast() );
        ::MessageBoxEx( strBuf, IDS_INFORMATION );
      }

      if ( !isGoodWrite ) {
        if ( ( writeFlag & io::licenseMissing ) != 0 ) {
          MessageBoxEx( RT_WRITE_ERR_LICENSE, IDS_TITLE_ERR );
        }
        else {
          MessageBoxEx( RT_FILE_WRITE_ERR, IDS_TITLE_ERR);
        }
      }
    }
  }
}

//----------------------------------------------------------------------------------------
// Запросить опции экспорта.
// ---
void AskForExportOptions(MbeModelExchangeFormat modelFormat, ConvConvertorProperty3D &convProperties)
{  
  if (convProperties.fileName.empty()) {
    _GetOpenFileName(TestVariables::nameBuffer, SELECT_FILE_INFO, nullptr, nullptr);
    convProperties.fileName.assign(c3d::C3DToPathstring(TestVariables::nameBuffer));
  }

  if (!TestVariables::checkWriteTime) // не спрашиваем при замере времени
    ::GetDouble(IDS_UNIT_OF_MEASURE, convProperties.lengthUnitsFactor);
  switch (modelFormat)
  {
  case mxf_IGES:
  {
    int exportType = 0;
    ::GetInt(IDS_SAVE_TOPOLIGY, exportType);
    convProperties.exportIGESTopology = (exportType != 0);
    break;
  }

  //case mxf_Parasolid:
  //case mxf_ACIS:
  //{
  //  int sType = 0;
  //  ::GetInt(IDS_ENTER_WRITE_FILETYPE, sType);
  //  convProperties.fileASCII = sType == 0;
  //  break;
  //}

  case mxf_JT:
  {
    ::InitStepData(convProperties.tesseleationStepData, false, TestVariables::operateType);
    break;
  }
  case mxf_STL:
  {
    int sType = 0;
    ::GetInt( IDS_ENTER_WRITE_FILETYPE, sType );
    convProperties.fileASCII = sType == 0;
    break;
  }
  case mxf_VRML:
  {
    int sType = 0;
    if (!TestVariables::checkWriteTime) // не спрашиваем при замере времени
      ::GetInt(IDS_ENTER_WRITE_FILETYPE, sType);
    convProperties.fileASCII = sType == 0;
    ::InitStepData(convProperties.tesseleationStepData, false, TestVariables::operateType);
    bool dualSeams = convProperties.dualSeams;
    int ttt = dualSeams ? 1 : 0;
    if (!TestVariables::checkWriteTime) // не спрашиваем при замере времени
      ::GetInt(IDS_ENTER_DUAL_SEAMS, ttt);
    dualSeams = (ttt != 0);
    convProperties.dualSeams = dualSeams;
    break;
  }
  default:
    break;
  }
  UpdateConverterSettings(convProperties);
}

//----------------------------------------------------------------------------------------
// Запросить опции импорта.
// ---
void AskForImportOptions(MbeModelExchangeFormat modelFormat, ConvConvertorProperty3D &convProperties)
{  
  if (convProperties.fileName.empty()) {
    _GetOpenFileName(TestVariables::nameBuffer, SELECT_FILE_INFO, nullptr, nullptr);
    convProperties.fileName.assign(c3d::C3DToPathstring(TestVariables::nameBuffer));
  }

  switch (modelFormat)
  {
  case mxf_ACIS:
  case mxf_ACISBinary:
  {
    convProperties.enableAutostitch = false; //::GetAutostitchData( convProperties.autostitchPrecision );
    convProperties.joinSimilarFaces = ::GetJoinSimilarFacesFlag();
    convProperties.addRemovedFacesAsShells = ::GetAddRemovedFacesAsShellsFlag();
    break;
  }

  case mxf_IGES:
  case mxf_Parasolid:
  case mxf_ParasolidBinary:
  case mxf_STEP:
  {
    convProperties.enableAutostitch = ::GetAutostitchData(convProperties.autostitchPrecision);
    convProperties.joinSimilarFaces = ::GetJoinSimilarFacesFlag();
    convProperties.addRemovedFacesAsShells = ::GetAddRemovedFacesAsShellsFlag();
    break;
  }

  case mxf_JT:
  {
    int importBrep(0);
    ::GetInt(IDS_ENTER_IMPORT_BREP, importBrep);
    convProperties.ioPermissions[iop_rBRep] = (importBrep != 0);
    if (convProperties.ioPermissions[iop_rBRep])
    {
      convProperties.enableAutostitch = ::GetAutostitchData(convProperties.autostitchPrecision);
      convProperties.joinSimilarFaces = ::GetJoinSimilarFacesFlag();
      convProperties.addRemovedFacesAsShells = ::GetAddRemovedFacesAsShellsFlag();
    }
    break;
  }

  case mxf_STL:
  {
    convProperties.SetIoPermission(iop_rSolid, false); // false - не удалять избыточные точки.
    break;
  }
  default:
    break;
  }
  UpdateConverterSettings(convProperties);
}

//----------------------------------------------------------------------------------------
// Запись тела в файл
// ---
bool WriteSolid( const TCHAR * pathName, MbSolid & solid )
{
  bool isGoodWrite = false;

  if ( ::SameExtension( pathName, C3D_EXT ) ) { // C3D
    solid.AddRef();
    MbModel * geomModel = new MbModel();
    geomModel->AddItem( solid, 0 );

    membuf memBuf;           // Создаем membuf
    std::unique_ptr<writer> rc = writer::CreateMemWriter( memBuf, 0 );
    isGoodWrite = rc->good();

    if ( isGoodWrite ) {
      ::WriteModelItems( *rc.get(), *geomModel );
      memBuf.setMaxRegCount ( rc->GetMaxRegisteredCount() ); // \ru Максимальное количество регистрируемых объектов. \en The maximum number of registered facilities.

      isGoodWrite = ::writeiobuftodisk( pathName, memBuf ); // Пишем его на диск
    }
    ::DeleteItem( geomModel );
    solid.DecRef();
  }
  return isGoodWrite;
}


//----------------------------------------------------------------------------------------
// Добавить в модель файл
// ---
void AddModel()
{
  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
  if ( wind != nullptr ) {
    if ( !wind->IsModelNameEmpty() )
      _tcscpy( TestVariables::nameBuffer, wind->GetModelName() ); // Имя файла модели.

    string_t nameBufferBak( TestVariables::nameBuffer );
    bool isGoodRead = false;
    bool hasUnknownObjects = false;

    PluginLoader pluginLoader {};

    if ( _GetOpenFileName( TestVariables::nameBuffer, SELECT_FILE_INFO, GetFilenameFilter(&pluginLoader).c_str(), NON_EXT ) ) {
      if ( ::SameExtension( TestVariables::nameBuffer, C3D_EXT ) ) { // C3D
        std::unique_ptr<reader_ex> pl =
          reader_ex::CreateReaderEx( std::unique_ptr<iobuf_Seq>(&::createiobuf(TestVariables::nameBuffer)), io::in );
        isGoodRead = pl->good();

        if ( isGoodRead ) {
          SetWaitCursor( true );

          wind->ReadAddModel( *pl );
          hasUnknownObjects = (pl->state() & io::skippedUnknown) != 0;

          SetWaitCursor( false );
        }
      }
      else { // импортируемые форматы
        bool repl = TestVariables::replaceItems;
        TestVariables::replaceItems = false;
        std::string const currentLocale { setlocale( LC_CTYPE, nullptr ) };  // иначе не читаются пути с кириллицей
        setlocale(LC_CTYPE, "");
        const TCHAR * fileName = TestVariables::nameBuffer;

        MbeConvResType importCode{cnv_Error};
        std::unique_ptr<IProgressIndicator> progBar{&::CreateProgressIndicator(new StrSpyConverters)};
        ConvConvertorProperty3D convProperties;
        C3dModelDocument modelDoc;
        convProperties.fileName = c3d::C3DToPathstring( fileName );
        ::UpdateConverterSettings( convProperties );
        AskForImportOptions(::GetFileFormat(c3d::C3DToPathstring(fileName)), convProperties);
        
        if (TestVariables::importFromBuffer)
        {
          importCode = ImportFromBufferLocal(modelDoc, convProperties);
        }
        else
        {
          importCode = ::ImportFromFile(modelDoc, convProperties.fileName, &convProperties, progBar.get());
        }
        if ((importCode == cnv_Success) || (importCode == cnv_PartialSuccess))
        {
          SPtr<MbItem> importedContent{modelDoc.GetContent()};
          isGoodRead = !importedContent.is_null();
          if (isGoodRead)
          {
            ::AddContentToManager(*importedContent, false);
          }
          else
          {
            importCode = cnv_NoObjects;
          }
        }
        else if (!TestVariables::importFromBuffer && (importCode == cnv_Error || importCode == cnv_UnknownExtension || importCode == cnv_UnsupportedVersion))
        {
          importCode = WrConvertor3D::PluginRead(fileName, pluginLoader);
          isGoodRead = (cnv_Success == importCode) || (cnv_PartialSuccess == importCode);
        }
        ::ShowReadFailureDetais(importCode);

        setlocale(LC_CTYPE, currentLocale.c_str());
        TestVariables::replaceItems = repl;
      }
    }
    if ( !nameBufferBak.empty() )
      _tcscpy( TestVariables::nameBuffer, nameBufferBak.c_str() ); // вернуть имя файла, куда добавляли
    if ( hasUnknownObjects )
      MessageBoxEx( RT_FILE_WITH_UNKNOWN_OBJECTS, IDS_TITLE_WRN );
    else if ( !isGoodRead )
      MessageBoxEx( RT_FILE_NOT_OPEN, IDS_TITLE_ERR );
  }
}


//----------------------------------------------------------------------------------------
// Чтение геометрической модели из файла *.MAT
// Read the geometric model from *.MAT file
// ---
void ReadItem()
{
  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
  if ( wind != nullptr ) {
    if ( !wind->IsModelNameEmpty() )
      _tcscpy( TestVariables::nameBuffer, wind->GetModelName() ); // Имя файла модели.

    if ( _GetOpenFileName( TestVariables::nameBuffer, SELECT_FILE_INFO, MAT_EXT_INFO, MAT_EXT ) ) {
      std::unique_ptr<reader> pl =
        reader::CreateReader( std::unique_ptr<iobuf_Seq>(&::createiobuf(TestVariables::nameBuffer)), io::in );

      if ( pl->good() ) {
        SetWaitCursor( true );

        // Запомненный объект и его элемент
        TestVariables::ResetHLighted();


        bool isRead = wind->ReadItem( *pl );
        wind->SetModelName( TestVariables::nameBuffer );

        SetWaitCursor( false );

        if ( !isRead )
          MessageBoxEx( RT_ERROR_READING_PARTLY, IDS_TITLE_ERR );
      }
      else
        MessageBoxEx( RT_FILE_NOT_OPEN, IDS_TITLE_ERR );
    }
  }
}


//----------------------------------------------------------------------------------------
// Запись геометрической модели в формат *.MAT
// Write the geometric model into format *.MAT
// ---
void WriteItem()
{
  if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() )
  {
    if ( !wind->IsModelNameEmpty() )
      _tcscpy( TestVariables::nameBuffer, wind->GetModelName() ); // Имя файла модели.

    if ( _GetSaveFileName( TestVariables::nameBuffer, SELECT_FILE_INFO, MAT_EXT_INFO, MAT_EXT ) )
    {
      membuf buf; // Создаем membuf
      {
        writer::writer_ptr rc = writer::CreateMemWriter( buf, 0 );
        if ( rc->good() ) {
          SetWaitCursor( true );
          wind->WriteItem( *rc );
          buf.setMaxRegCount( rc->GetMaxRegisteredCount() ); // \ru Максимальное количество регистрируемых объектов. \en The maximum number of registered facilities.
          SetWaitCursor( false );
        }
        else
          MessageBoxEx( RT_FILE_NOT_WRITTEN, IDS_TITLE_ERR );
      }
      ::writeiobuftodisk( TestVariables::nameBuffer, buf ); // Пишем его на диск
    }
  }
}

//----------------------------------------------------------------------------------------
// AP
// ---
static
void SecondsToTimeString( int32 time, TCHAR* lpOut )
{
  if( lpOut == nullptr )
    return;

  // maximum: 99:59:59:999
  if( time > 359999999 ) {
    ::_tcscpy( lpOut, _T("infinite") );
    return;
  }

  int32 h, m, s, ms;

  if( time <= 0 ) {
    h = 0; m = 0; s = 0, ms = 0;
  }
  else {
    h = time / 3600000;
    time %= 3600000;
    m = time / 60000;
    time %= 60000;
    s = time / 1000;
    time %= 1000;
    ms = time;
  }

  _sntprintf( lpOut, 256, _T("%02u:%02u:%02u:%03u"), h, m, s, ms );
}


//----------------------------------------------------------------------------------------
// Выдать информацию о времени работы
// ---
void ShowTimeTestResult( TimeTest * testResult,
                         uint32 miliSec,
                         const TCHAR * sss )
{
  TCHAR buf0[128];
  TCHAR buf3[128];

  SecondsToTimeString( miliSec, buf0 );

  _sntprintf( buf3, 128, RT_TEST_TIME_MSEC, (TCHAR*)buf0, miliSec );

  const void * params[5] = { sss, nullptr, nullptr, buf3, testResult };

  ExecuteTimeTestResultDlg(&params);
}

//----------------------------------------------------------------------------------------
// Output the collision detection summary.
// facePairsNb - the number of unigue pairs of faces for which intersection was found.
//---
void ShowInterferenceResult( TimeTest * testResult,
                             size_t facePairsNb,
                             uint32 miliSec )
{
  TCHAR buf0[128];
  TCHAR buf1[128];
  TCHAR buf2[128];

  SecondsToTimeString( miliSec, buf0 );

  _sntprintf( buf1, 128, IDS_COLLISION_PAIRS _T(": ") F_TD, facePairsNb );
  _sntprintf( buf2, 128, RT_TEST_TIME_MSEC, (TCHAR*)buf0, miliSec );
  _sntprintf( buf0, 128, IDS_COLLISION_DETECTION );

  const void * params[5] = { buf0, buf1, buf2, nullptr, testResult };

  ExecuteTimeTestResultDlg(&params);
}

//----------------------------------------------------------------------------------------
// Выдать информацию о времени работы
//---
void ShowTimeTestResult( TimeTest * testResult,
                         size_t operationCount,
                         size_t facesCount,
                         uint32 miliSec,
                         MbeSpaceType itemType,
                         const TCHAR * sss )
{
  TCHAR buf0[128];
  TCHAR buf1[128];
  TCHAR buf2[128];
  TCHAR buf3[128];

  SecondsToTimeString( miliSec, buf0 );

  switch ( itemType ) {
    case st_WireFrame: {
      _sntprintf( buf1, 128, RT_OPERATIONS_COUNT F_TD _T( " " ), operationCount );           //-V111
      _sntprintf( buf2, 128, RT_EDGES_COUNT      F_TD _T( " " ), facesCount );               //-V111
    } break;
    case st_Solid: {
      _sntprintf( buf1, 128, RT_OPERATIONS_COUNT F_TD _T( " " ), operationCount );           //-V111
      _sntprintf( buf2, 128, RT_FACES_COUNT      F_TD _T( " " ), facesCount );               //-V111
    } break;
    case st_PlaneInstance: {
      _sntprintf( buf1, 128, RT_VISIBLE_LINES_COUNT F_TD _T( " " ), operationCount );     //-V111
      _sntprintf( buf2, 128, RT_HIDE_LINES_COUNT    F_TD _T( " " ), facesCount );         //-V111
    } break;
    default:
      break;
  }

  _sntprintf( buf3, 128, RT_TEST_TIME_MSEC, (TCHAR*)buf0, miliSec );

  const void * params[5] = { sss, buf1, buf2, buf3, testResult };

  ExecuteTimeTestResultDlg(&params);
}

//----------------------------------------------------------------------------------------
// Выдать информацию о времени работы теста сопряжений.
// ---
void ShowMateTimeTestResult( TimeTest *     testResult
                           , size_t         mateCount
                           , size_t         bodyCount
                           , GCM_result result
                           , const TCHAR *  sss )
{
  TCHAR buf0[128];
  TCHAR buf1[128];
  TCHAR buf2[128];
  TCHAR buf3[128];

  SecondsToTimeString( result, buf0 );
  _sntprintf( buf1, 128, RT_MATES_COUNT F_TD _T(" "), mateCount );
  _sntprintf( buf2, 128, RT_BLOCKS_COUNT F_TD _T(" "), bodyCount );
  _sntprintf( buf3, 128, RT_SOLVING_CODE, result );

  const void * params[5] = { sss, buf1, buf2, buf3, testResult };
  ExecuteTimeTestResultDlg( &params );
}

//----------------------------------------------------------------------------------------
// Вернуть resId ошибки построения solida
// Перевод enuma MbResultType в идентификаторы main.rh
// ---
static const TCHAR* /*uint*/ GetSolidErrorResId( MbResultType solidError )
{
  switch ( solidError )
  {
    case rt_Success:                     // Нормальная работа
      return RT_OK;
    case rt_Empty:
      return RT_EMPTY;              // Пустой
    case rt_ToManyAxis:
      return RT_TOMANYAXIS;         // Слишком много осей
    case rt_ToFewAxis:
      return RT_TOFEWAXIS;          // Не хватает осей
    case rt_ToManyContours:
      return RT_TOMANYCONTOURS;     // Слишком много контуров
    case rt_Stars:
      return RT_STARS;              // Есть "звезда"
    case rt_SelfIntersection:
      return RT_SELFINTERSECTION;   // Самопересечение
    case rt_Intersection:
      return RT_INTERSECTION;       // Пересечение контуров
    case rt_NoIntersectSolid:       // Контур не пересекает поверхность тела
      return RT_NO_INTERSECTION_SOLID;
    case rt_NoIntersectSection:     // Образующий контур не пересекает сечения (для операции построения тела по сечениям)
      return RT_NO_INTERSECT_SECTION;  // Образующий контур не пересекает сечения (для операции построения тела по сечениям)
    case rt_MustBeClosed:
      return RT_MUST_BE_CLOSED;     // Должен быть замкнут
    case rt_MustBeOpen:
      return RT_MUST_BE_OPEN;       // Должен быть разомкнут
    case rt_AxisIntersection:
      return RT_AXIS_INTERSECTION;  // Пересечение с осью
    case rt_InvalidType:
      return RT_INVALID_TYPE;       // Неподходящий тип кривой

    // Типы ошибок
    case rt_NoSequenceCurveAndSections:  // Не последовательное расположение сечений вдоль кривой (для операции построения тела по сечениям)
      return RT_NO_SEQUENCE_CURVE_AND_SECTIONS;  // Не последовательное расположение сечений вдоль кривой (для операции построения тела по сечениям)
    case rt_MultiSolid:             // Тело состоит из отдельных частей
      return RT_MULTISOLID;
    case rt_CurveError:             // Ошибочная кривая
      return RT_CURVEERROR;
    case rt_ContourError:           // Ошибочный контур
      return RT_CONTOURERROR;
    case rt_SurfaceError:           // Ошибочная поверхность
      return RT_SURFACEERROR;
    case rt_SolidError:             // Ошибочное тело
      return RT_SOLIDERROR;
    case rt_ParameterError:         // Ошибочный параметр
      return RT_PARAMETERERROR;
    case rt_ThicknessError:         // Неверно задана толщина
       return RT_THICKNESSERROR;
    case rt_SelfIntersect:          // Объект самопересекается
      return RT_SELFINTERSECT;
    case rt_SelfIntWhenExtended:    // Самопересечение в продолжении контура
      return RT_SELFINTWHENEXTENDED;
    case rt_Intersect:              // Объекты пересекаются
      return RT_INTERSECT;
    case rt_NoIntersect:            // Объекты не пересекаются
      return RT_NOINTERSECT;
    case rt_OffsetIntersectError:         // Объекты пересекается с ошибкой
      return RT_ERRORINTERSECT;
    case rt_BooleanError:           // Ошибка в булевой операции
      return RT_BOOLEANERROR;
    case rt_NoEdges:                // Ребра не найдены
      return RT_NOEDGES;
    case rt_PrepareError:           // Ошибка при подготовке операции
      return RT_PREPAREERROR;
    case rt_ChamferError:           // Ошибка при создании фаски ребра
      return RT_CHAMFERERROR;
    case rt_FilletError:            // Ошибка при скруглении ребра
      return RT_FILLETERROR;
    case rt_ChamferSurfaceError:    // Ошибка при создании поверхности фаски ребра
      return RT_CHAMFER_SURFACE_ERROR;
    case rt_FilletSurfaceError:     // Ошибка при создании поверхности скругления ребра
      return RT_FILLET_SURFACE_ERROR;
    case rt_TooLargeChamfer:        // Слишком большые катеты фаски
      return RT_TOO_LARGE_CHAMFER;
    case rt_TooLargeFillet:         // Слишком большой радиус скругления
      return RT_TOO_LARGE_FILLET;
    case rt_SemiChamfer:           // Фаски построены на для всех ребер
      return RT_SEMI_CHAMFER;
    case rt_SemiFillet:            // Скруглены не все ребра
      return RT_SEMI_FILLET;
    case rt_CuttingError :          // Ошибка резки поверхностью
      return RT_CUTTINGERROR;
    case rt_ThinError:              // Ошибка при создании тонкостенного тела
      return RT_THINERROR;
    case rt_RibError :              // Ошибка постановки ребра жесткости
      return RT_RIBERROR;
    case rt_DraftError:             // Неизвестная ошибка уклона граней тела
      return RT_DRAFTERROR;
    case rt_CutBySilhouetteError:   // Ошибка разреза силуэтной линией.
      return RT_CUTBYSILHOUETTE;
    case rt_SplitWireNotIntersectFace:  // Контур не пересекает ни одну из граней или совпадает с кромкой грани(MA 17.07.2001)
      return RT_SPLIT_WIRE_NOT_INTERSECT_FACE;

    case rt_SplitWireNotSplitFace:      // Контур не разбивает
      return RT_SPLIT_WIRE_NOT_SPLIT_FACE;
    case rt_NotAllContoursUsed: // Not all contours or curves were used
      return RT_NOT_ALL_ITEMS_WERE_USED;

    case rt_ProcessIsStopped:
      return RT_PROCESS_INTERRUPTED;

    case rt_LicenseError:
      return RT_LICENCE_CHECK_FAILURE;

    case rt_Error:                      // Ошибка
    default :
      return RT_ERROR_;
  }
} // GetSolidErrorResId

//----------------------------------------------------------------------------------------
// Строковый результат поиска столкновений.
//---
static const TCHAR* CollisionDetectionResult(cdet_result cdmResCode)
{
  switch (cdmResCode)
  {
    case CDM_RESULT_None:
      return CDM_MESSAGE_NO_RESULT;
    case CDM_RESULT_MeasuringSucceed:
      return CDM_MESSAGE_MEASURE_FULFILLED;
    case CDM_RESULT_NoCollisionDetected:
      return CDM_MESSAGE_NO_COLLISION;
    case CDM_RESULT_CollisionDetected:
      return CDM_MESSAGE_COLLISION_DETECTED;

    default:
    case CDM_RESULT_Error:
      return CDM_MESSAGE_ERROR;
  }
}

//----------------------------------------------------------------------------------------
// Вернуть resId ошибки сшивки
// Перевод enuma MbStitchResType в идентификаторы main.rh
// ---
static const TCHAR* /*uint*/ GetStitchErrorResId( uint stitchError )
{
  switch ( stitchError ) {

    case stch_Success:                    // Нормальная работа
      return RT_OK;
    case stch_PrepareError:               // Ошибка при подготовке операции
      return RT_PREPAREERROR;
    case stch_CoorientFaceError:          // Невозможно выставить согласованную ориентацию граней
      return RT_COORIENTFACEERROR;
    case stch_SomeEdgesUnstitched:        // Некоторые рёбра остались не сшитыми
      return RT_SOMEEDGESUNSTITCHED;
    case stch_OutwardOrientError:         // Не удалось установить нормали граней наружу тела
      return RT_OUTWARDORIENTERROR;
    case stch_NoEdgeWasStitched:          // Не было сшито ни одного ребра
      return RT_NOEDGEWASSTITCHED;
    case stch_SeparatePartsResult:        // После сшивки остались несвязанные между собой куски
      return RT_SEPARATEPARTSRESULT;

    default :                             // Ошибка
      return RT_ERROR_;
  }
} // GetStitchErrorResId


//-----------------------------------------------------------------------------------------
// Функция записи параметров документа в файл
// ---
#ifdef C3D_DEBUG

#ifndef C3D_WINDOWS // _MSC_VER
  #define SIZE_WRITE  size_t
  #define HANDLE      FILE*
  #define CloseHandle fclose
  static void WriteFile( HANDLE      hFile,
                         const void *lpBuffer,
                         SIZE_WRITE  nNumberOfBytesToWrite,
                         SIZE_WRITE  *lpNumberOfBytesWritten,
                         const void *lpOverlapped )
  {
    size_t size = fwrite( lpBuffer, nNumberOfBytesToWrite, 1/*count*/,  hFile );
  }
#else // _WIN32
  #define SIZE_WRITE DWORD
#endif // _WIN32

static void WriteDocToFile( void * ptext )
{
  string_t fileName;
  fileName.assign( TestVariables::nameBuffer );
  if ( fileName.empty() ) {
    fileName.assign( _T("noname") );
    fileName.append( TXT_EXT );
  }
  else {
    string_t dotStr(_T("."));
    size_t pos = fileName.find_last_of( dotStr );
    if ( pos == string_t::npos )
      fileName.append( TXT_EXT );
    else {
      size_t len = fileName.length();
      if ( pos == len - 4 ) {
        dotStr.assign( TXT_EXT );
        fileName.replace( pos, 4, dotStr );
      }
      else {
        fileName.append( TXT_EXT );
      }
    }
  }

  HANDLE hFile = nullptr; // Имя файла
  if ( TestVariables::rebuildTimeToFile != 0 ) {
    //создаём файл
#ifdef C3D_WINDOWS // _MSC_VER
    hFile = CreateFile( fileName.c_str(),     // Имя файла
                        GENERIC_WRITE,        // Создание на запись
                        0,                    // Флаг доступа для другого процесса
                        nullptr,                 // Аттрибуты безопасности
                        CREATE_ALWAYS,        // Открываем файл
                        FILE_ATTRIBUTE_NORMAL,// Аттрибуты файла
                        nullptr);
#else // _WIN32
    hFile = fopen( c3d::C3DToPathstring(fileName).c_str(), "wt" );
#endif // _WIN32

    SIZE_WRITE numOfBytes;// Количество реально записанных байт
    uint sizeEOL = 4; // Длина записи конца строки //-V112

    // Вывод общих данных
    TCHAR buf[128];
    // Запись в файл, имя файла
    _tcscpy( buf, TestVariables::nameBuffer );
    WriteFile(hFile, buf, (SIZE_WRITE)LoUint32(_tcslen(buf)), &numOfBytes, nullptr);
    WriteFile(hFile, _T("\r\n\r\n"), sizeEOL, &numOfBytes, nullptr ); // Запись конца строки
    // Запись в файл, действие
    _tcscpy(buf,((TCHAR**)ptext)[0]);
    WriteFile(hFile, buf, (SIZE_WRITE)LoUint32(_tcslen(buf)), &numOfBytes, nullptr);
    WriteFile( hFile, _T("\r\n\r\n"), sizeEOL, &numOfBytes, nullptr ); // Запись конца строки
    // Запись в файл, число тел
    _tcscpy(buf,((TCHAR**)ptext)[1]);
    WriteFile(hFile, buf, (SIZE_WRITE)LoUint32(_tcslen(buf)), &numOfBytes, nullptr);
    WriteFile( hFile, _T("\r\n\r\n"), sizeEOL, &numOfBytes, nullptr ); // Запись конца строки
    // Запись в файл, общее число граней
    _tcscpy(buf,((TCHAR**)ptext)[2]);
    WriteFile(hFile, buf, (SIZE_WRITE)LoUint32(_tcslen(buf)), &numOfBytes, nullptr);
    WriteFile( hFile, _T("\r\n\r\n"), sizeEOL, &numOfBytes, nullptr ); // Запись конца строки
    // Запись в файл, число точек
    _tcscpy(buf,((TCHAR**)ptext)[3]);
    WriteFile(hFile, buf, (SIZE_WRITE)LoUint32(_tcslen(buf)), &numOfBytes, nullptr);
    WriteFile( hFile, _T("\r\n\r\n"), sizeEOL, &numOfBytes, nullptr ); // Запись конца строки
    // Запись в файл, число треугольников
    _tcscpy(buf,((TCHAR**)ptext)[4]);   //-V112
    WriteFile(hFile, buf, (SIZE_WRITE)LoUint32(_tcslen(buf)), &numOfBytes, nullptr);
    WriteFile( hFile, _T("\r\n\r\n"), sizeEOL, &numOfBytes, nullptr ); // Запись конца строки
    // Запись в файл, число четырехугольников
    _tcscpy(buf,((TCHAR**)ptext)[5]);
    WriteFile(hFile, buf, (SIZE_WRITE)LoUint32(_tcslen(buf)), &numOfBytes, nullptr);
    WriteFile( hFile, _T("\r\n\r\n"), sizeEOL, &numOfBytes, nullptr ); // Запись конца строки
    // Запись в файл, общее время выполнения
    _tcscpy(buf,((TCHAR**)ptext)[6]);
    WriteFile(hFile, buf, (SIZE_WRITE)LoUint32(_tcslen(buf)), &numOfBytes, nullptr);
    WriteFile( hFile, _T("\r\n\r\n"), sizeEOL, &numOfBytes, nullptr ); // Запись конца строки

    // Закрываем файл
    CloseHandle(hFile);
  }
}

#ifndef C3D_WINDOWS // _MSC_VER
  #undef SIZE_WRITE
  #undef HANDLE
  #undef CloseHandle
#else // _WIN32
  #undef SIZE_WRITE
#endif // _WIN32

#endif // C3D_DEBUG


//-----------------------------------------------------------------------------------------
// Выдать информацию о времени и размере триангуляции
// ---
void ShowTimeTestResult( TimeTest * testResult,
                         size_t solidsCount,
                         size_t facesCount,
                         size_t pointsCount,
                         size_t trianglesCount,
                         size_t quadranglesCount,
                         uint32 miliSec,
                         const TCHAR * sss )
{
  TCHAR info[9999];
  _tcscpy( info, _T("\r") );

  TCHAR buf0[128];
  TCHAR buf1[128];
  TCHAR buf2[128];
  TCHAR buf3[128];
  TCHAR buf4[128];
  TCHAR buf5[128];
  TCHAR buf6[128];

  SecondsToTimeString( miliSec, buf0 );

  _sntprintf( buf6, 128, RT_BODIES_COUNT      F_TD _T(" "), solidsCount      );  //-V111
  _sntprintf( buf1, 128, RT_TOTAL_FACES_COUNT F_TD _T(" "), facesCount       );  //-V111
  _sntprintf( buf2, 128, RT_POINTS_COUNT      F_TD _T(" "), pointsCount      );  //-V111
  _sntprintf( buf3, 128, RT_TRIANGLES_COUNT   F_TD _T(" "), trianglesCount   );  //-V111
  _sntprintf( buf4, 128, RT_QUADRANGLES_COUNT F_TD _T(" "), quadranglesCount );  //-V111

  _sntprintf( buf5, 128, RT_TEST_TIME_MSEC, (TCHAR*)buf0, miliSec );

  _tcscat( info, buf6 );
  _tcscat( info, _T("\n\r") );
  _tcscat( info, buf1 );
  _tcscat( info, _T("\n\r") );
  _tcscat( info, buf2 );
  _tcscat( info, _T("\n\r") );
  _tcscat( info, buf3 );
  _tcscat( info, _T("\n\r") );
  _tcscat( info, buf4 );
  _tcscat( info, _T("\n\r") );
  _tcscat( info, buf5 );

  MessageBoxEx( info, sss );

#ifdef C3D_DEBUG
  const void * params[8] = {sss, buf6, buf1, buf2, buf3, buf4, buf5, testResult };
  ::WriteDocToFile( params );
#else
  testResult = nullptr;
#endif
}

//----------------------------------------------------------------------------------------
// Выдаст сообщение о причине неудачи построения solid'а.
//---
void PutErrorMessage( uint solidError, const TCHAR * name )
{
  if ( solidError > rt_ErrorTotal )
    MessageBoxEx( _T("UnknownError"), name);
  else
    MessageBoxEx( ::GetSolidErrorResId( static_cast<MbResultType>(solidError) ), name );
}

//----------------------------------------------------------------------------------------
// Выдаст сообщение о причине неудачи построения solid'а.
//---
void PutErrorMessage( MbResultType solidError, const TCHAR * name )
{
  MessageBoxEx( ::GetSolidErrorResId( solidError ), name );
}

//----------------------------------------------------------------------------------------
// Выдаст сообщение о причине неудачи построения solid'а.
//---
void PutResultMessage( cdet_result resCode, const TCHAR * name )
{
  MessageBoxEx( CollisionDetectionResult(resCode), name );
}


//----------------------------------------------------------------------------------------
// Выдаст сообщение о причине неудачи построения solid'а
//---
void StitchErrorMessage( int stitchError, const TCHAR * name  )
{
  MessageBoxEx( ::GetStitchErrorResId( stitchError ), name );
}


//----------------------------------------------------------------------------------------
// Провести неоконные действия при инициализации главного окна задачи
// ---
void SystemInitFrame()
{
//OV_Lnx   _tcscpy(fontName,_T("Courier New"));
  _tcscpy( TestVariables::nameBuffer,_T(""));
  TestVariables::viewManager = new Manager();
}

//----------------------------------------------------------------------------------------
// Возвращает имя файла с лицензионным ключом
// Return a name of the file with a license key
// ---
inline static c3d::string_t GetLicenceFileName()
{
#if defined(C3D_WINDOWS)
 #ifdef _UNICODE
  c3d::string_t userPath( _wgetenv( _T( "USERPROFILE" ) ) );
 #else // _UNICODE
  c3d::string_t userPath( std::getenv( "USERPROFILE" ) );
 #endif // _UNICODE
  return userPath + string_t( _T( "\\c3d.lic" ) );
#else
  c3d::string_t userPath = ToC3Dstring( std::getenv( "HOME" ) );
  return userPath + string_t( _T( "/c3d.lic" ) );
#endif // C3D_WINDOWS
}

//----------------------------------------------------------------------------------------
// Ввести данные о лицензионном ключе
// ---
void EnterLicenseKey()
{
  std::string stKey;
  std::string stSig;
  bool save2file = false;

  if ( GetLicenseSigKey( stKey, stSig, &save2file ) ) {
    if ( !VerifyLicenseKey(stKey.c_str(),stSig.c_str()) ) {
      MessageBoxEx( RT_LICENSE_INVALID_KEY, RT_LICENSE );
      return;
    }

    if (IsLicenseActive())
      DisableMathModules();

    EnableMathModules( stKey.c_str(), ( int )stKey.length(), stSig.c_str(), ( int )stSig.length() );

    if ( save2file ) {
      std::list<std::pair <std::string,std::string>> keysList;
      {
        std::ifstream licfile( c3d::C3DToPathstring( GetLicenceFileName() ).c_str() );
        if (licfile && licfile.is_open()) {
          ReadKeysFromFile( licfile, keysList );
          licfile.close();
        }
      }
      keysList.push_front( std::pair <std::string,std::string>( stKey, stSig ) );
      if ( !WriteKeysToFile( keysList ) ) {
        MessageBoxEx( RT_SAVING_LICENCE_ERROR_DESCRIPTION, RT_SAVING_LICENCE_ERROR );
      } 
    }
  }
}

//----------------------------------------------------------------------------------------
// Показать данные о текущей активированной лицензии
// ---
void ShowLicenseStatus()
{
  string_t respond = RT_LICENSE;
  if ( IsLicenseActive() ) {
    respond = _T("");
    if ( IsMathModelerEnable() )
      respond += _T("Modeler\n");
    if ( IsMathConverterEnable() )
      respond += _T("Converter\n");
    if ( IsMathSolverEnable() )
      respond += _T("Solver\n");
    if ( IsMathBShaperEnable() )
      respond += _T("BShaper\n");
    if ( IsMathPolyShaperEnable() )
      respond += _T("PolyShaper\n");
    if ( IsMathCollisionEnable() )
      respond += _T("Collision Detection\n");
    if ( IsMathMappingEnable() )
      respond += _T("Mapping\n");
    if ( IsMathVisionEnable() )
      respond += _T("Vision");
  }
#ifndef __USE_QT__
  MessageBoxEx( respond.c_str(), RT_LICENSE_STATUS );
#else
  QMessageBox messageBox;
  messageBox.setWindowTitle( QString_fromT( RT_LICENSE_STATUS ) );
  messageBox.setText( QString_fromT( respond.c_str() ) );
  messageBox.exec();
#endif // !__USE_QT__
}

//----------------------------------------------------------------------------------------
// Активировать лицензию по ключу и подписи из файла c3d.lic в папке пользователя
// License activation using the key saved in the c3d.lic file in the user directory
// ---
void ActivateLicenseFromFile()
{
  std::ifstream licfile( c3d::C3DToPathstring( GetLicenceFileName() ).c_str() ); // License file: two string - key, signature.
  if ( !licfile ) {
    EnterLicenseKey();
  }
  else {
    std::list<std::pair <std::string,std::string>> keysList;
    ReadKeysFromFile( licfile, keysList );

    bool isLicenseEnable = false;
    for( auto& key: keysList ) {
      isLicenseEnable = VerifyLicenseKey( key.first.c_str(),key.second.c_str() );
      if ( isLicenseEnable ) {
          EnableMathModules( key.first.c_str(), ( int )key.first.length(), key.second.c_str(), ( int )key.second.length() );
          break;
        }
    }

    if ( !isLicenseEnable ) {
      TCHAR s[512];
      size_t len = 0;
  #if defined(C3D_WINDOWS)
      len += _sntprintf( s + len, 512 - len, RT_LICENSE_DISABLED , GetLicenceFileName().c_str() );
  #else
      len += _sntprintf( s + len, 512 - len, RT_LICENSE_DISABLED, GetLicenceFileName().c_str() );
  #endif
      MessageBoxEx( s, RT_LICENCE_CHECK_FAILURE );
    }
  }
}

//----------------------------------------------------------------------------------------
// Заполнить вектор пар ключ/значение из файла
// Filling a vector of key/value pairs from a file
// ---
bool ReadKeysFromFile( std::ifstream& file, std::list<std::pair <std::string,std::string> >& keys )
{
  if ( !file.is_open() )
    return false;

  while ( !file.eof() )
  {    
    std::string stKey, stSign;
    std::getline( file, stKey );
    if ( file.eof() )
      return true;
    std::getline( file, stSign );
    keys.push_back( std::pair<std::string,std::string>{ stKey,stSign });
  }
  return true;
}

//----------------------------------------------------------------------------------------
// Записать вектор пар ключ/значение в файл
// Writing a vector of key/value pairs to a file
// ---
bool WriteKeysToFile( std::list<std::pair<std::string,std::string>> & keys )
{
  std::ofstream licfile( c3d::C3DToPathstring( GetLicenceFileName() ).c_str(), std::ios::out | std::ios::trunc );
  bool operationDone = false;
  if ( licfile.is_open() ) {
    for( auto& item : keys )
    {
      licfile << item.first << std::endl;
      licfile << item.second << std::endl;
    }
    licfile.flush();
    licfile.close();
    operationDone = true;
  }
  return operationDone;
}

#ifndef __USE_QT__ 
BOOL CALLBACK DlgLicProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );
#endif // !__USE_QT__ 


//------------------------------------------------------------------------------
// Ввод ключа и подписи лицензии
// ---
bool    GetLicenseSigKey( std::string & key, std::string & sig, bool * save2file )
{
#ifndef __USE_QT__
  if (DialogBox( hInst, ( LPCTSTR )IDD_DIALOG_LICENSE, ParentForDialog(), ( DLGPROC )DlgLicProc ) ) {
    c3d::string_t tKey, tSig;
    tKey.assign( TestVariables::strBuffer );
    tSig.assign( TestVariables::strBuffer2 );
    key.assign( ToSTDstring( tKey ) );
    sig.assign( ToSTDstring( tSig ) );
    *save2file = TestVariables::saveLic;
    return true;
  }
#else
  LicenseKeyDialog dialog( qMainWnd );

    if(dialog.exec() == QDialog::Accepted) {
      key.assign( dialog.GetKey() );
      sig.assign( dialog.GetSignature() );
      *save2file = dialog.GetSavingFlag();
      return true;
    }

#endif // !__USE_QT__
    return false;
}

//----------------------------------------------------------------------------------------
// Проверить, активирована ли лицензия для любого из компонентов ядра
// ---
bool IsLicenseActive()
{
  return IsMathModelerEnable() || IsMathConverterEnable() || IsMathSolverEnable() ||
    IsMathVisionEnable() || IsMathBShaperEnable() || IsMathCollisionEnable() ||
    IsMathMappingEnable();
}


///////////////////////////////////////////////////////////////////////////////////////////
//
// Неклассные функции конкретного окна.
//
///////////////////////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------------------------
// Создать новое окно и модель.
// ---
bool CreateViewWindow( uint command )
{
  wProjections viewNumber = v_Isometry;  // Номер проекции
  const TCHAR * title = nullptr;
  switch (command)
  {
    case CM_VIEWFRONT : // Спереди
      title = IDS_TITLE_VIEWFRONT;
      viewNumber = v_Front;
      break;

    case CM_VIEWREAR  : // Сзади
      title = IDS_TITLE_VIEWREAR;
      viewNumber = v_Rear;
      break;

    case CM_VIEWUP    : // Сверху
      title = IDS_TITLE_VIEWUP;
      viewNumber = v_Up;
      break;

    case CM_VIEWDOWN  : // Снизу
      title = IDS_TITLE_VIEWDOWN;
      viewNumber = v_Down;
      break;

    case CM_VIEWLEFT  : // Слева
      title = IDS_TITLE_VIEWLEFT;
      viewNumber = v_Left;
      break;

    case CM_VIEWRIGHT : // Справа
      title = IDS_TITLE_VIEWRIGHT;
      viewNumber = v_Right;
      break;

    case CM_VIEWISO   : // Изометрия
      title = IDS_TITLE_VIEWISOMETRY;
      viewNumber = v_Isometry;
      break;
  }

  if ( TestVariables::viewManager )
  {
    WorkWindow * wind = new WorkWindow( viewNumber );
    wind->CreateWorkWindow( title, TestVariables::viewManager->GetBackgrColor(), TestVariables::viewManager->GetLightDirection()/*lightDirection*/ );
    wind->OrientateWindow( viewNumber );

    ptrdiff_t newIndex = TestVariables::viewManager->AddWindow( wind );
    TestVariables::viewManager->SetFocus( newIndex, false );
  }

  return true;
}


//----------------------------------------------------------------------------------------
// Расположить по вектору взгляда.
// ---
void SetViewVector()
{
  if ( TestVariables::viewManager != nullptr )
  {
    size_t wInd = TestVariables::viewManager->GetActiveWindowIndex();
    WorkWindow * window = TestVariables::viewManager->GetWindow( wInd ) ;

    if ( window != nullptr ) {
      // Запросить вектор взгляда.
      MbCartPoint3D p;
      if( GetExactPoint(p) ) {
        if ( p.MaxFactor() > METRIC_PRECISION ) {
          MbVector3D eye( p );
          eye.Invert();
          const MbCartPoint3D & origin = window->GetOrigin();
          MbPlacement3D place( origin, eye );
          window->OrientateWindow( place );
        }
      }
    }
  }
}


//----------------------------------------------------------------------------------------
// Установить ориентацию окна
// ---
void SetViewWindow( wProjections proj )
{
  if ( TestVariables::viewManager != nullptr )
  {
    size_t wInd = TestVariables::viewManager->GetActiveWindowIndex();
    WorkWindow * window = TestVariables::viewManager->GetWindow( wInd ) ;

    if ( window != nullptr ) {
      window->OrientateWindow( proj );
      window->SetWindowTitle();
    }
  }
}


//----------------------------------------------------------------------------------------
// Select a mesh
// ---
MbMesh * SelectMesh( MbMatrix3D * from )
{
  MbMesh * mesh = nullptr;
  {
    MbItem * obj = nullptr;
    PathMatrix pathFrom;
    if ( GetGObj( IDS_SELECT_MESH, st_Mesh, &obj, pathFrom ) && (obj->IsA() == st_Mesh) ) {
      mesh = static_cast<MbMesh *>(obj);
      if ( from != nullptr )
        from->Init( pathFrom.second );
    }
  }
  return mesh;
}


//----------------------------------------------------------------------------------------
// Select a polymesh
// ---
MbPolymesh * SelectPolymesh( MbMatrix3D * from )
{
  MbPolymesh * mesh = nullptr;
  {
    MbItem * obj = nullptr;
    PathMatrix pathFrom;
    if ( GetGObj( IDS_SELECT_MESH, st_Polymesh, &obj, pathFrom ) && ( obj->IsA() == st_Polymesh ) ) {
      mesh = static_cast<MbPolymesh *>( obj );
      if ( from != nullptr )
        from->Init( pathFrom.second );
    }
  }
  return mesh;
}


//----------------------------------------------------------------------------------------
// Select a wire frame
// ---
MbWireFrame * SelectWireFrame( MbMatrix3D * from )
{
  MbWireFrame * wireFrame = nullptr;
  {
    MbItem * wireFrameItem = nullptr;
    PathMatrix pathFrom;
    if ( GetGObj( IDS_SELECT_WIREFRAME, st_SpaceItem, &wireFrameItem, pathFrom ) && (wireFrameItem->IsA() == st_WireFrame) ) {
      wireFrame = static_cast<MbWireFrame *>( wireFrameItem );
      if ( from != nullptr )
        from->Init( pathFrom.second );
    }
  }
  return wireFrame;
}


//----------------------------------------------------------------------------------------
// Select an edge
// ---
MbEdge * SelectEdge( MbItem * & obj )
{
  MbEdge * selectEdge = nullptr;
  MbItem * edgeObj = nullptr;
  MbTopItem * edgeItem = nullptr;

  if ( GetTObj(IDS_SELECT_EDGE, st_Item, tt_TopItem, &edgeObj, &edgeItem) &&
    (edgeItem->IsA() == tt_Edge || edgeItem->IsA() == tt_CurveEdge) ) {
    selectEdge = static_cast<MbEdge *>( edgeItem );
    if ( edgeObj != nullptr )
      obj = edgeObj;
  }

  return selectEdge;
}


//----------------------------------------------------------------------------------------
// Select a solid
// ---
MbSolid * SelectSolid( MbMatrix3D * from )
{
  MbSolid * solid = nullptr;
  {
    MbItem * obj = nullptr;
    PathMatrix pathFrom;
    if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj, pathFrom ) && (obj->IsA() == st_Solid) ) {
      solid = static_cast<MbSolid *>( obj );
      if ( from != nullptr )
        from->Init( pathFrom.second );
    }
  }
  return solid;
}


//----------------------------------------------------------------------------------------
// Select a solid
// ---
MbSolid * SelectSolid( PathMatrix & pathFrom )
{
  MbSolid * solid = nullptr;
  {
    MbItem * obj = nullptr;
    if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj, pathFrom ) && (obj->IsA() == st_Solid) ) {
      solid = static_cast<MbSolid *>(obj);
    }
  }
  return solid;
}


//----------------------------------------------------------------------------------------
// Select shell faces
// ---
template <class FacesVector>
MbSolid * SelectShellFacesImp( FacesVector & selFaces, PathMatrix * selPathFrom, bool drawItems )
{
  MbSolid * solid = nullptr;

  C3D_ASSERT( selFaces.empty() );
  selFaces.clear();

  MbFace * face = nullptr;
  {
    MbItem * obj = nullptr;
    MbTopItem * topItem = nullptr;

    PathMatrix pathFrom;
    if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &topItem, pathFrom ) ) {
      if ( topItem->IsAFace() ) {
        solid = static_cast<MbSolid *>(obj);

        face = static_cast<MbFace *>(topItem);
        selFaces.push_back( face );

        if ( selPathFrom != nullptr )
          (*selPathFrom) = pathFrom;

        if ( drawItems ) {
          TestVariables::viewManager->ToggleObject( obj );
          TestVariables::drawImp->DrawItem( topItem, TRGB_WHITE );
          TestVariables::drawImp->DrawItem( topItem, TRGB_LIGHTCYAN );
        }

        const MbItem * obj0 = obj;
        const PathMatrix pathFrom0( pathFrom );

        while ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &topItem, pathFrom ) && (face != nullptr) ) {
          if ( obj == obj0 ) {
            if ( drawItems ) {
              TestVariables::drawImp->DrawItem( topItem, TRGB_WHITE );
            }
            face = static_cast<MbFace *>(topItem);
            size_t delInd = c3d::DirectSearch( selFaces, face );

            if ( delInd != SYS_MAX_T ) {
              if ( drawItems ) {
                TestVariables::drawImp->DrawItem( topItem, TRGB_LIGHTGREEN );
              }
              selFaces.erase( selFaces.begin() + delInd );
            }
            else {
              selFaces.push_back( face );
            }
            if ( drawItems ) {
              for ( auto & selFace : selFaces ) {
                TestVariables::drawImp->DrawItem( selFace, TRGB_LIGHTCYAN );
              }
            }
          }
        }
      }
    }
    else {
      if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj, pathFrom ) ) {
        solid = static_cast<MbSolid *>(obj);

        if ( selPathFrom != nullptr )
          (*selPathFrom) = pathFrom;
      }
    }
  }

  return solid;
}

//----------------------------------------------------------------------------------------
// Select shell faces
// ---
MbSolid * SelectShellFaces( FacesVector & selFaces, PathMatrix * pathFrom, bool drawItems )
{
  return ::SelectShellFacesImp( selFaces, pathFrom, drawItems );
}

//----------------------------------------------------------------------------------------
// Select shell faces
// ---
MbSolid * SelectShellFaces( RPArray<MbFace> & selFaces, PathMatrix * pathFrom, bool drawItems )
{
  return ::SelectShellFacesImp( selFaces, pathFrom, drawItems );
}


//----------------------------------------------------------------------------------------
// Select a face
// ---
MbFace * SelectFace( MbMatrix3D * from )
{
  MbFace * face = nullptr;
  {
    MbItem * obj = nullptr;
    MbTopItem * topItem = nullptr;

    MbMatrix3D matrFrom;
    if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &topItem, matrFrom ) ) {
      if ( topItem->IsAFace() ) {
        face = static_cast<MbFace *>(topItem);
        if ( from != nullptr )
          from->Init( matrFrom );
      }
    }
  }
  return face;
}


//----------------------------------------------------------------------------------------
// Select a surface
// ---
MbSurface * SelectSurface( MbMatrix3D * from )
{
  MbSurface * surface = nullptr;
  {
    MbItem * obj = nullptr;
    PathMatrix pathFrom;
    if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &obj, pathFrom ) ) {
      MbSpaceInstance * spInst = static_cast<MbSpaceInstance *>(obj);
      if ( spInst->GetSpaceItem() != nullptr ) {
        MbSpaceItem * spItem = static_cast<MbSpaceItem *>(spInst->SetSpaceItem());
        if ( spItem->Family() == st_Surface ) {
          surface = static_cast<MbSurface *>(spItem);
          if ( from != nullptr ) {
            from->Init( pathFrom.second );
          }
        }
      }
    }
  }
  return surface;
}


//----------------------------------------------------------------------------------------
// Select a curve copy
// ---
SPtr<MbCurve3D> SelectAnyCurveCopy( PathMatrix & pathFrom, std::vector< SPtr<MbCurve3D> > * otherCurvesPtr, MbItem ** curveCarrier )
{
  SPtr<MbCurve3D> curve;

  MbItem * item = nullptr;

  if ( GetAnyCurve( IDS_SELECT_CURVE, &item, pathFrom ) ) {
    if ( ::GetSpaceCurve( *item, true, curve, otherCurvesPtr ) && (curve != nullptr) ) {
      if ( curveCarrier != nullptr )
        *curveCarrier = item;

      MbRegDuplicate * iReg = nullptr;
      MbAutoRegDuplicate autoReg( iReg );

      curve = ::DuplicateIfUsed( curve, iReg );
      if ( otherCurvesPtr != nullptr ) {
        std::vector< SPtr<MbCurve3D> > & otherCurves = *otherCurvesPtr;
        std::vector< SPtr<MbCurve3D> >::iterator it = otherCurves.begin();
        while ( it != otherCurves.end() ) {
          *it = ::DuplicateIfUsed( *it, iReg );
          ++it;
        }
      }
    }
  }

  return curve;
}


//----------------------------------------------------------------------------------------
// Select a point
// ---
bool SelectPoint( MbCartPoint3D & pnt )
{
  if ( GetPoint( IDS_SELECT_POINT, pnt ) ) {
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// Выбор образующей для операции.
// ---
MbSweptData * SelectSweptData( bool bWireFrame )
{
  MbSweptData * resData = nullptr;

  if ( bWireFrame ) {
    while ( true )
    {
      MbWireFrame * wireFrame = nullptr;
      MbSweptData::MbeWireFrameType wireFrameType = MbSweptData::MbeWireFrameType::undefined;
      MbSurface * wireFrameSurface = nullptr;

      MbItem * wireFrameItem = nullptr;
      if ( GetGObj( IDS_SELECT_WIREFRAME, st_SpaceItem, &wireFrameItem) && (wireFrameItem != nullptr) ) {
        if ( wireFrameItem->IsA() == st_WireFrame ) {
          wireFrame = static_cast<MbWireFrame *>( wireFrameItem );
        }
      }
      else {
        // Создание каркаса по грани + указание поверхности грани в MbSweptData.
        MbItem * obj = nullptr;
        MbTopItem * topItem = nullptr;
        if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &topItem, TestVariables::itemPathFrom.second ) ) {
          MbFace * face = static_cast< MbFace * >( topItem );

          if ( face != nullptr ) {
            WireEdgesSPtrVector edges;
            face->GetEdges( edges );
            wireFrame = MbWireFrame::Create( edges, nullptr, nullptr, false, Math::DefaultMathVersion() ); // Создание каркаса на копии исходных ребер.
            wireFrameType = MbSweptData::MbeWireFrameType::surfaceContours;
            wireFrameSurface = &face->SetSurface();
          }
        }
        else {
          break;
        }
      }

      if ( wireFrame != nullptr ) {
        if ( resData == nullptr )
          resData = new MbSweptData();
        resData->AddData( *wireFrame, wireFrameType, wireFrameSurface );
      }
    }
    if ( resData != nullptr ) {
      MessageBoxEx( IDS_GENERATRIC_PREPARED, IDS_TITLE_INFO );
    }
  }
  else {
    MbItem * curCurveObj(0); // Образующая. Generatrix.
    MbItem * prevCurveObj(0);

    SpaceContoursSPtrVector contours3d;
    MbPlacement3D contoursPlace;
    SurfaceSPtr surface;
    PlaneContoursSPtrVector contours;

    while ( GetAnyCurve ( IDS_SELECT_GENER_CURVE, &curCurveObj ) ) {

      if ( curCurveObj != prevCurveObj ) {
        MbPlacement3D cntrPlace;
        MbContour * contour = ::GetContourOnPlace( curCurveObj, cntrPlace );
        if ( contour != nullptr ) {
          contours.push_back( PlaneContourSPtr( contour ) );
          if ( prevCurveObj != nullptr ) {
            TestVariables::viewManager->ToggleObject( prevCurveObj );
            if ( !contoursPlace.Colinear( cntrPlace ) )
              break;
            MbMatrix mtr;
            if ( cntrPlace.GetMatrixToPlace( contoursPlace, mtr ) && !mtr.IsSingle() )
              contour->Transform( mtr );
          }
          else {
            contoursPlace.Init( cntrPlace );
            surface = new MbPlane( contoursPlace );
          }
          TestVariables::viewManager->ToggleObject( curCurveObj );
          prevCurveObj = curCurveObj;
        }
      }

      if ( surface == nullptr && GetCurve3D( curCurveObj, true )  ) {
        SPtr<MbCurve3D> crv( ::GetCurve3D( curCurveObj, false ) );
        if ( crv != nullptr ) {
          MbContour3D * contour = nullptr;
          if ( crv->Type() == st_Contour3D )
            contour = &static_cast<MbContour3D &>(*crv);
          else {
            RPArray<MbCurve3D> curves;
            curves.Add( crv );
            contour = new MbContour3D( curves, false/*same*/, Math::DefaultMathVersion() );
          }
          contours3d.push_back( SpaceContourSPtr( contour ) );
          if ( prevCurveObj ) 
            TestVariables::viewManager->ToggleObject(prevCurveObj);
          TestVariables::viewManager->ToggleObject(curCurveObj);
          prevCurveObj = curCurveObj;
        }
      }

      if ( contours.size() == 0 && contours3d.size() == 0 ) {
        // Образующая из контуров на поверхности. Generatrix as contours on a surface.
        MbItem * surfObj = nullptr;
        MbTopItem * tItem = nullptr;
        if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &surfObj, &tItem, TestVariables::itemPathFrom.second ) && tItem->IsAFace() ) {
          MbFace * face = static_cast< MbFace * >( tItem );
          surface = face->GetSurfaceCurvesData( contours );
        }
      }
    }

    if ( prevCurveObj )
      TestVariables::viewManager->ToggleObject( prevCurveObj );

    if ( contours3d.size() > 0 )
      resData = new MbSweptData( contours3d );
    else if ( surface != nullptr && contours.size() > 0 ) 
      resData = new MbSweptData( *surface, contours );
  }

  return resData;
}


//----------------------------------------------------------------------------------------
// Чтение объектов модели из файла по фильтру
// ---
void ReadModelTree ( TCHAR * fileName, EObjectFilter filter )
{
  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();

  if ( fileName == nullptr ) {
    auto const filterStr = FormatFiltersForFileSelectionDialog({FormatInfo(_T("C3D"), {_T("*.c3d")})});
    if ( _GetOpenFileName ( TestVariables::nameBuffer, SELECT_FILE_INFO, filterStr.c_str(), nullptr ) )
      fileName = TestVariables::nameBuffer;
  }

  if ( wind == nullptr ) {
    wind = new WorkWindow ( v_Isometry );
    wind->CreateWorkWindow ( IDS_TITLE_VIEWISOMETRY, TestVariables::viewManager->GetBackgrColor(), TestVariables::viewManager->GetLightDirection()/*lightDirection*/ );
    wind->OrientateWindow ( v_Isometry );

    ptrdiff_t newIndex = TestVariables::viewManager->AddWindow ( wind );
    TestVariables::viewManager->SetFocus( newIndex, false );
  }

  if ( fileName != nullptr ) {
    const size_t bufLen = 1024;
    TCHAR buf[bufLen];
    size_t len = 0;
    if ( TestVariables::checkReadTime )
      len += (size_t)_sntprintf ( buf+len, bufLen-len, RT_TEST_RESULTS );

    MbAccurateTimer ac;

    if ( ::SameExtension( fileName, C3D_EXT ) ) { // C3D
      reader_ex* pl = reader_ex::CreateReaderEx( std::unique_ptr<iobuf_Seq>(&::createiobuf(fileName)), io::in ).release();

      if ( pl->good() ) {
        SetWaitCursor ( true );

        ac.Begin();

        pl->ReadObjectCatalog();

        ac.End();

        SetWaitCursor ( false );

        if ( pl->good() && pl->GetModelTree() ) {
          switch (filter) {
            case of_selection: {
              TestVariables::viewManager->CreateTree ( pl ); // Now this dialog owns reader
            } break;
            case of_type: {
              TestVariables::viewManager->CreateTypeFilter ( pl ); // Now this dialog owns reader
            } break;
            case of_gabarit: {
              TestVariables::viewManager->CreateGabaritFilter ( pl ); // Now this dialog owns reader
            } break;
            case of_attribute: {
              TestVariables::viewManager->CreateAttributeFilter ( pl ); // Now this dialog owns reader
            } break;
            case of_embodiment: {
              TestVariables::viewManager->CreateEmbodimentTree( pl ); // Now this dialog owns reader
            } break;
          }
        }
      }
    }
  }
}

#if defined(C3D_WINDOWS)
  //----------------------------------------------------------------------------------------
  // Обработчик консоли.
  // Console event handler.
  // ---
  BOOL CtrlHandler( DWORD ctrlType )
  {
    switch( ctrlType )
    {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
      FreeConsole();
      return TRUE;
    case CTRL_CLOSE_EVENT:
      return TRUE;

    default:
      return FALSE;
    }
  }

  //----------------------------------------------------------------------------------------
  // Запустить консоль.
  // Run the console.
  // ---
  void RunConsole()
  {
    BOOL res = AttachConsole( ATTACH_PARENT_PROCESS );
    if ( !res )
    {
      res = AllocConsole();
      SetConsoleCtrlHandler( (PHANDLER_ROUTINE)CtrlHandler, TRUE );

      freopen("CONIN$", "r", stdin);
      freopen("CONOUT$", "w", stdout);
      freopen("CONOUT$", "w", stderr);

      std::cin.sync_with_stdio();
      std::cout.sync_with_stdio();
      std::cerr.sync_with_stdio();
      std::clog.sync_with_stdio();
    }
  }
#else  // C3D_WINDOWS
  //----------------------------------------------------------------------------------------
  // Запустить консоль.
  // Run the console.
  // ---
  void RunConsole()
  {}
#endif  // C3D_WINDOWS

#if defined (C3D_WINDOWS) && !defined(ALL_WARNINGS) // _MSC_VER // Set warnings level
#pragma warning(pop) // Restore state of warning settings
#endif
