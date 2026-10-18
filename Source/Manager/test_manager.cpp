#include <fstream>
#include <test_service.h>
#include <test_manager.h>
#include <test_variables.h>
#include <test_gr_draw.h>
#include <test_converter.h>
#include <templ_sptr.h>
#include <templ_s_list.h>
#include <cur_polyline3d.h>
#include <cur_polyline.h>
#include <cur_contour.h>
#include <cur_line_segment.h>
#include <cur_line_segment3d.h>
#include <surf_cone_surface.h>
#include <surf_sector_surface.h>
#include <surf_ruled_surface.h>
#include <assembly.h>
#include <constraint.h>
#include <solid.h>
#include <point_frame.h>
#include <space_instance.h>
#include <plane_instance.h>
#include <tool_enabler.h>
#include <alg_draw.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Прочитать файл с пользовательскими настройками тестового приложения
// формат строки: название=значение, пути без кавычек
// опции конвертации начинаются с converter.**
// например: converter.capvidia.capvidiaLocation=C:/Program Files/Capvidia/Capvidia SDK x64 Edition
// ---
bool ReadC3dTestConfig( eAssertViolationNotify& notifyWin )
{
#if defined(C3D_WINDOWS)
  std::string configPath = std::getenv( "USERPROFILE" ) + std::string( "\\c3d_test.config" );
#else
  std::string configPath = std::getenv( "HOME" ) + std::string( "/c3d_test.config" );
#endif
  std::ifstream config( configPath );
  if ( config.is_open() )
  {
    while ( !( config.eof() || config.bad() ) ) {
      std::string propLine;
      std::getline( config, propLine );
      if ( propLine.empty () || '#' == propLine.front () )
        continue;
      const size_t indDelim = propLine.find( "=" );
      if ( indDelim > 0 )
      {
        std::string propKey = propLine.substr( 0, indDelim );
        std::string propValue = propLine.substr( indDelim + 1, propLine.length() - 1 );
        TestVariables::userProp[c3d::ToC3Dstring( propKey )] = c3d::ToC3Dstring( propValue );
      }
    }
    const c3d::string_t debugDraw( CONFIG_APP_DEBUG_DRAW );
    const c3d::string_t multithreadingMode( CONFIG_APP_MULTITHREADING_MODE );
    const c3d::string_t assertOn( CONFIG_APP_ASSERT_MODE );
    const c3d::string_t permanentDiagHighlightOn( CONFIG_APP_PERMANENT_DIAGNOSTICS_HIGHLIGHT );
    const c3d::string_t highlightingSubObjOn( CONFIG_APP_HIGHLIGHTING_SUB_OBJECT );
    const c3d::string_t forceUseMbColor( CONFIG_APP_FORCE_USE_MBCOLOR );
    const c3d::string_t forceUsePlugin( CONFIG_APP_FORCE_USE_PLUGIN );
    const c3d::string_t readToModelDocument( CONFIG_APP_READ_TO_MODEL_DOCUMENT );
    const c3d::string_t fitOnFirstLoading( CONFIG_APP_FIT_ON_FIRST_LOADING );
    const c3d::string_t importFromBuffer( CONFIG_APP_IMPORT_FROM_BUFFER );
    const c3d::string_t exportIntoBuffer( CONFIG_APP_EXPORT_INTO_BUFFER );
    const c3d::string_t importObFileIntoSolid( CONFIG_APP_IMPORT_OB_FILES );
    const c3d::string_t boundingBoxErrorDialogBox( CONFIG_USE_BOUNDING_BOX_ERROR_DIALOG );
    for ( const std::pair<c3d::string_t, c3d::string_t> & curSettingPair : ::TestVariables::userProp )
    {
      bool bVal( true );
      ptrdiff_t valInt( 0 );
      if ( debugDraw == curSettingPair.first && ::UpdateBool( curSettingPair.second, bVal ) && !bVal )
      {
#ifdef _DRAWGI
        ::SetDrawGI( nullptr );
#endif
      }
      else if ( multithreadingMode == curSettingPair.first )
      {
        if ( 1 == sscanf( c3d::ToSTDstring( curSettingPair.second ).c_str(), "%td", &valInt ) )
        {
          MbeMultithreadedMode mode = valInt == 0 ? mtm_Off :
                                      ( valInt == 1 ? mtm_Standard :
                                       ( valInt == 2 ? mtm_SafeItems :
                                        ( valInt == 3 ? mtm_Items : mtm_Max ) ) );
          Math::SetMultithreadedMode( mode );
        }
      }
      else if ( assertOn == curSettingPair.first && 1 == sscanf( c3d::ToSTDstring( curSettingPair.second ).c_str(), "%td", &valInt ) )
        notifyWin = valInt == 0 ? avn_Mute : ( valInt == 1 ? avn_CERR : avn_ASSERT );
      else if ( permanentDiagHighlightOn == curSettingPair.first  )
      {
        bool bPermanentHighLight = TestVariables::permanentDiagnosticsShow;
        if ( ::UpdateBool( curSettingPair.second, bPermanentHighLight ) )
          TestVariables::permanentDiagnosticsShow = bPermanentHighLight;
      }
      else if ( highlightingSubObjOn == curSettingPair.first )
      {
        bool bHighlightingSubObj = TestVariables::useHighlightObj;
        if ( ::UpdateBool( curSettingPair.second, bHighlightingSubObj ) )
          TestVariables::useHighlightObj = bHighlightingSubObj;
      }
      else if ( forceUsePlugin == curSettingPair.first )
      {
        bool forceUsePluginVal = TestVariables::forceUsePlugin;
        if ( ::UpdateBool( curSettingPair.second, forceUsePluginVal ) )
          TestVariables::forceUsePlugin = forceUsePluginVal;
      }
      else if ( readToModelDocument == curSettingPair.first )
      {
        bool readToModelDoc = TestVariables::readToModelDocument;
        if ( ::UpdateBool( curSettingPair.second, readToModelDoc ) )
          TestVariables::readToModelDocument = readToModelDoc;
      }
      else if( fitOnFirstLoading == curSettingPair.first )
      {
        bool fitOnLoading = TestVariables::fitOnFirstLoading;
        if ( ::UpdateBool( curSettingPair.second, fitOnLoading ) ) {
          TestVariables::fitOnFirstLoading = fitOnLoading;
      }
      }
      else if ( importFromBuffer == curSettingPair.first )
      {
        bool readBuffer = TestVariables::importFromBuffer;
        if ( ::UpdateBool( curSettingPair.second, readBuffer ) )
          TestVariables::importFromBuffer = readBuffer;
      }
      else if ( exportIntoBuffer == curSettingPair.first )
      {
        bool writeBuffer = TestVariables::importFromBuffer;
        if (::UpdateBool(curSettingPair.second, writeBuffer))
          TestVariables::exportIntoBuffer = writeBuffer;
      }
      else if ( importObFileIntoSolid == curSettingPair.first )
      {
        bool writeBuffer = TestVariables::importFromBuffer;
        if (::UpdateBool(curSettingPair.second, writeBuffer))
          TestVariables::importObFileIntoSolid = writeBuffer;
      }
      else if ( boundingBoxErrorDialogBox == curSettingPair.first )
      {
        bool boxErrorDialog = TestVariables::useBoundingBoxErrorDialog;
        if ( ::UpdateBool( curSettingPair.second, boxErrorDialog ) )
          TestVariables::useBoundingBoxErrorDialog = boxErrorDialog;
      }
    }
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
// Чтение пользовательских настроек из config файла.
// ---
bool ApplyC3dTestConfig()
{
  eAssertViolationNotify notifyWin = Math::CheckAssertNotify();
  const bool bRes = ReadC3dTestConfig( notifyWin );
  if ( bRes && Math::CheckAssertNotify() != notifyWin )
    Math::SetAssertNotify( notifyWin );
  return bRes;
}

//------------------------------------------------------------------------------
// Конструктор. 
// ---
Manager::Manager()
  : workWindows( 2, 1, true )
  , propertyWindows( 0, 5 )
  , treeWindows( 0, 5 )
  , constraintModel2D()
  , constraintManager2D()
  , constraintManager3D()
  , cplane     (      )
  , temporal   (      )
  , activeIndex( -1   )
  , lightDirection( 0.1, 0.1, 1.0 )
  , enableDraw ( true )
{
  // Активация лицензии по ключу сохраненному в файле c3d.lic в директории пользователя
  // License activation using the key saved in the c3d.lic file in the user directory
  ActivateLicenseFromFile();  

  eAssertViolationNotify notifyWin( avn_ASSERT );

  // Чтение пользовательских настроек.
  // Read user settings.
  ReadC3dTestConfig( notifyWin );

  // Set notification level.
#if defined(C3D_WINDOWS)
  Math::SetAssertNotify( notifyWin );
#else
  Math::SetAssertNotify(avn_CERR);
#endif

}


//------------------------------------------------------------------------------
// Деструктор
// ---
Manager::~Manager()
{
  DisableMathModules();
}


//------------------------------------------------------------------------------
// Добавление геометрического объекта
// ---
void Manager::ReplaceItem( MbItem & item, MbItem & newItem, bool saveName )
{
  if ( WorkWindow * wind = GetActiveWindow() )
    wind->ReplaceItem( item, newItem, saveName );
}


//------------------------------------------------------------------------------
// Обновить сетку для геометрического объекта.
// ---
void Manager::RefreshDrawItem( MbItem * geomItem )
{
  if ( geomItem != nullptr )
    if ( WorkWindow * wind = GetActiveWindow() )
      wind->RefreshDrawItem( *geomItem );
}


//------------------------------------------------------------------------------
// Добавление геометрического объекта
// ---
void Manager::AddItem( MbItem & geomItem, bool showItem )
{
  if ( WorkWindow * wind = GetActiveWindow() )
    wind->AddItem( geomItem, nullptr, showItem );
}


//------------------------------------------------------------------------------
// Добавление геометрического объекта
// ---
void Manager::AddItem( MbItem & geomItem, const Style & style, bool showItem )
{  
  if ( WorkWindow * wind = GetActiveWindow() )
    wind->AddItem( geomItem, &style, showItem );
}


//------------------------------------------------------------------------------
// Добавление геометрического объекта
// ---
MbItem * Manager::AddObject( MbSpaceItem & sitem, bool showItem )
{
  MbItem * newObj = nullptr;
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    newObj = wind->AddObject( sitem, showItem );
  return newObj;
}


//------------------------------------------------------------------------------
// Добавление геометрического объекта
// ---
MbItem * Manager::AddObject( const Style & style, MbSpaceItem * sitem, bool showItem )
{
  MbItem * newObj = nullptr;
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    newObj = wind->AddObject( style, sitem, showItem );
  return newObj;
}


//------------------------------------------------------------------------------
// Добавление плоского объекта
// ---
MbItem * Manager::AddObject( const Style & style, MbPlaneItem * item, const MbPlacement3D * p, bool showItem )
{
  MbItem * newObj = nullptr;
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    newObj = wind->AddObject( style, item, p, showItem );
  return newObj;
}


//------------------------------------------------------------------------------
// Добавление плоского объекта
// ---
MbItem * Manager::AddObject( const Style & style, const MbCartPoint & p0, const MbPlacement3D * p, bool showItem )
{
  MbItem * newObj = nullptr;
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    newObj = wind->AddObject( style, p0, p, showItem );
  return newObj;
}


//------------------------------------------------------------------------------
// Разрушить сборки с подсборками на составляющие
// ---
void Manager::DecomposeAssemblies()
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->DecomposeAssemblies();
}


//------------------------------------------------------------------------------
// Удалить все визуальные свойства объектов
// ---
void Manager::RemoveColorAndVisualAttributes()
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->RemoveColorAndVisualAttributes();
}


//------------------------------------------------------------------------------
// Удалить все объекты
// ---
void Manager::DeleteModel()
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->DeleteModel();
  DeleteTemporal();
  DeleteCplane(); // нужно чистить, в ней указатель на убиваемый объект
  constraintManager2D.Clear();
}


//------------------------------------------------------------------------------
// Удалить все объекты
// ---
void Manager::DeleteModelAttributes()
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->DeleteModelAttributes();
  DeleteTemporal();
  DeleteCplane(); // нужно чистить, в ней указатель на убиваемый объект
  constraintManager2D.Clear();
}


//------------------------------------------------------------------------------
// Заменить все объекты
// ---
bool Manager::ReplaceModel( const MbModel & newGeomModel )
{
  if ( WorkWindow * wind = GetActiveWindow() ) {
    if ( &wind->GetGeomModel() != &newGeomModel ) {
      wind->DeleteModel();

      DeleteTemporal();
      DeleteCplane(); // нужно чистить, в ней указатель на убиваемый объект
      constraintManager2D.Clear();

      wind->SetGeomModel().AddModel( newGeomModel );
      wind->RefreshModel();
      return true;
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Удалить все плоские объекты модели
// ---
void Manager::DeletePlaneItems()
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->DeleteByType( pt_PlaneItem );
}


//------------------------------------------------------------------------------
// Удалить все плоские объекты модели
// ---
void Manager::DeleteSpaceItems()
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->DeleteByType( st_SpaceItem );
}


//------------------------------------------------------------------------------
// Дать все объекты указанного типа. // Give all objects of the specified type.
// ---
void Manager::GetObjects( MbeSpaceType type, std::vector<SPtr<MbItem>> & items, std::vector<MbMatrix3D> & matrs )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->GetObjects( type, items, matrs );
}


//------------------------------------------------------------------------------
// Дать все объекты указанного типа. // Give all objects of the specified type.
// ---
void Manager::GetObjects( MbeSpaceType type, RPArray<MbItem> & items, SArray<MbMatrix3D> & matrs )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->GetObjects( type, items, matrs );
}


//------------------------------------------------------------------------------
// Дать все поверхности, тела и оболочки. Get all surfaces, bodies, and shells.
// ---
void Manager::GetRenderingObjects( RPArray<MbItem> & renderItems )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->GetRenderingObjects( renderItems );
}


//------------------------------------------------------------------------------
// Дать все кривые и точки. Get all curves and points.
// ---
void Manager::GetLineDrawingObjects( RPArray<MbItem> & wireItems )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->GetLineDrawingObjects( wireItems );
}


//------------------------------------------------------------------------------
// Дать все объекты. Get all objects.
// ---
void Manager::GetAllObjects( RPArray<MbItem> & items )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->GetAllObjects( items );
}


//------------------------------------------------------------------------------
// Создать список математических кривых из объектов вида
// ---
void Manager::CreateCurveArray( MbItem * obj, RPArray<MbCurve> & curves, bool breakCurve )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->CreateCurveArray( obj, curves, breakCurve );
}


//------------------------------------------------------------------------------
// Найти объект по математическому объекту
// ---
const MbItem * Manager::FindObject( const MbSpaceItem * item ) const
{
  const MbItem * find = nullptr;
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    find = wind->FindObject( item );
  return find;
}


//------------------------------------------------------------------------------
// Найти объект по математическому объекту
// ---
const MbItem * Manager::FindObject( const MbPlaneItem * item ) const
{
  const MbItem * find = nullptr;
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    find = wind->FindObject( item );
  return find;
}


//------------------------------------------------------------------------------
// Дать габарит полигональной модели
// ---
void Manager::GetDrawModelGabarit( MbCube & r ) {
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->GetDrawModelGabarit( r );
}


//------------------------------------------------------------------------------
// Разрезать модель
// ---
bool Manager::CutModel( const MbPlacement3D & place, double distance, size_t & operationCount )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    return wind->CutModel( place, distance, operationCount );
  return false;
}


//------------------------------------------------------------------------------
// Число треугольнико в сеточной модели.
// ---
size_t Manager::GetTrianglesCount() const
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    return wind->GetTrianglesCount();
  return 0;
}


//------------------------------------------------------------------------------
// Стереть объект во всех окнах
// ---
void Manager::EraseObject( const MbItem * obj )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->EraseObject( obj );
}


//------------------------------------------------------------------------------
// Перерисовать объект
// ---
void Manager::RedrawObject( const MbItem * obj )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->RedrawObject( obj );
}


//------------------------------------------------------------------------------
// Перерисовать объект
// ---
void Manager::RefreshObjectModel( const MbItem * obj )
{
  DeleteTemporal();
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr ) {
    wind->RefreshOGL();
    wind->RedrawObject( obj );
  }
}


//------------------------------------------------------------------------------
// Выделить объект
// ---
void Manager::SelectObject( MbItem * obj, bool refreshDrawing )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->SelectObject( obj, refreshDrawing );
}

//------------------------------------------------------------------------------
// Выделить объект
// ---
void Manager::SelectObject( const MbPath & objPath, bool refreshDrawing )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->SelectObject( objPath, refreshDrawing );
}

//------------------------------------------------------------------------------
// Выделить объект
// ---
void Manager::UnselectObject( MbItem * obj, bool refreshDrawing )
{  
  if ( WorkWindow * wind = GetActiveWindow() )
    wind->UnselectObject( obj, refreshDrawing );
}

//------------------------------------------------------------------------------
// Выделить объект
// ---
void Manager::UnselectObject( const MbPath & objPath, bool refreshDrawing )
{  
  if ( WorkWindow * wind = GetActiveWindow() )
    wind->UnselectObject( objPath, refreshDrawing );
}

//------------------------------------------------------------------------------
// Инвертировать выделение объекта
// ---
void Manager::ToggleObject( const MbItem * obj, bool refreshDrawing )
{  
  if ( WorkWindow * wind = GetActiveWindow() )
    wind->ToggleObject( obj, refreshDrawing );
}

//------------------------------------------------------------------------------
// Инвертировать выделение объекта
// ---
void Manager::ToggleObject( const MbPath & objPath, bool refreshDrawing )
{  
  if ( WorkWindow * wind = GetActiveWindow() )
    wind->ToggleObject( objPath, refreshDrawing );
}


//------------------------------------------------------------------------------
// Изменить толщину линий объекта
// ---
void Manager::SetObjectWidth( MbItem * obj, int w )
{  
  if ( WorkWindow * wind = GetActiveWindow() )
    wind->SetObjectWidth( obj, w );
}


//------------------------------------------------------------------------------
// Преобразование объекта согласно матрице
// ---
void Manager::TransformObject( MbItem * obj, MbMatrix3D & matr )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->TransformObject( obj, matr );
}


//------------------------------------------------------------------------------
// Сдвиг объекта
// ---
void Manager::MoveObject( MbItem * obj, const MbVector3D & to )
{
  if ( WorkWindow * wind = GetActiveWindow() )
    wind->MoveObject( obj, to );
}

//------------------------------------------------------------------------------
// Сдвиг объекта
// ---
void Manager::MoveObject( const MbPath & objPath, const MbVector3D & to )
{
  if ( WorkWindow * wind = GetActiveWindow() )
    wind->MoveObject( objPath, to );
}


//------------------------------------------------------------------------------
// Повернуть вокруг оси объекта
// ---
void Manager::RotateObject( MbItem * obj, MbAxis3D & axis, double angle )
{  
  if ( WorkWindow * wind = GetActiveWindow() )
    wind->RotateObject( obj, axis, angle );
}


//------------------------------------------------------------------------------
// Удаление объекта
// ---
void Manager::DeleteObject( MbItem * obj )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->DeleteObject( obj );
}

//------------------------------------------------------------------------------
// Удаление объекта
// ---
void Manager::DeleteObject( const MbPath & objPath )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->DeleteObject( objPath );
}


//------------------------------------------------------------------------------
// Дублировать объект
// ---
void Manager::DuplicateObject( const MbItem * obj )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->DuplicateObject( obj );
}


//------------------------------------------------------------------------------
// Дублировать объект
// ---
void Manager::TransformDuplicateObject( const MbItem * obj, const MbMatrix3D & matr )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->TransformDuplicateObject( obj, matr );
}


//------------------------------------------------------------------------------
// Выделить объекты по типу
// ---
void Manager::SelectByType( MbeSpaceType type )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->SelectByType( type );
}


//------------------------------------------------------------------------------
// Выделить объекты по типу
// ---
void Manager::SelectByType( MbePlaneType type )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->SelectByType( type );
}


//------------------------------------------------------------------------------
// Установить видимость объектов по типу
// ---
void  Manager::SetVisibleByType( MbeSpaceType type, bool visible )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->SetVisibleByType( type, visible );
}


//------------------------------------------------------------------------------
// Установить видимость объектов по типу
// ---
void  Manager::SetVisibleByType( MbePlaneType type, bool visible )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->SetVisibleByType( type, visible );
}


//------------------------------------------------------------------------------
// Выделить объекты модели
// ---
void Manager::ModelSelectByObject( const MbItem * obj )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->ModelSelectByObject( obj );
}


//------------------------------------------------------------------------------
// Переместить объект в конструктивной плоскости.
// ---
void Manager::MoveObjectInPlacement( MbItem * obj, const MbVector3D & to )
{
  DeleteTemporal();
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->MoveObjectInPlacement( obj, to );
}


//------------------------------------------------------------------------------
// Повернуть объект в конструктивной плоскости.
// ---
void Manager::RotateObjectInPlacement( MbItem * obj, const MbCartPoint3D & point, double angle )
{
  DeleteTemporal();
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->RotateObjectInPlacement( obj, point, angle );
}


//------------------------------------------------------------------------------
// Трансформировать объекты в конструктивной плоскости. Objects tranforming.
// ---
void Manager::TransformObjectInPlacement( MbItem * obj, const MbMatrix & matr )
{
  DeleteTemporal();
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->TransformObjectInPlacement( obj, matr );
}


//------------------------------------------------------------------------------
// Установить конструктивную плоскость
// ---
void Manager::SetObjectPlacement( MbItem * obj, const MbPlacement3D & pl )
{
  DeleteTemporal();
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->SetObjectPlacement( obj, pl );
}


//------------------------------------------------------------------------------
// Обновить объекты модели
// ---
void Manager::RefreshModel()
{  
  if ( WorkWindow * wind = GetActiveWindow() ) 
  {
    wind->SetVisualSag(); // Установить величину стрелки прогиба для визуализации
    wind->RefreshModel();
  }
}


//------------------------------------------------------------------------------
// Нарисовать модель во всех окнах
// ---
void Manager::ShowModel()
{  
  if ( WorkWindow * wind = GetActiveWindow() )
    wind->ShowModel();
}


//------------------------------------------------------------------------------
// Стереть модель во всех окнах
// ---
void Manager::EraseModel()
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->EraseModel();
}


//------------------------------------------------------------------------------
// Выделить модель во всех окнах
// ---
void Manager::SelectModel()
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->SelectModel();
}


//------------------------------------------------------------------------------
// Выделить модель во всех окнах
// ---
void Manager::UnselectModel()
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->UnselectModel();
}


//------------------------------------------------------------------------------
// Выделить модель во всех окнах
// ---
void Manager::ToggleModel()
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->ToggleModel();
}


//------------------------------------------------------------------------------
// Преобразование выделенных объектов согласно матрице
// ---
void Manager::TransformSelected( const MbMatrix3D & matr )
{
  DeleteTemporal();
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->TransformSelected( matr );
}


//------------------------------------------------------------------------------
// Сдвиг выделенных объектов
// ---
void Manager::MoveSelected( const MbVector3D & to )
{
  DeleteTemporal();
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->MoveSelected( to );
}


//------------------------------------------------------------------------------
// Повернуть вокруг оси выделенных объектов
// ---
void Manager::RotateSelected( const MbAxis3D & axis, double angle )
{
  DeleteTemporal();
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->RotateSelected( axis, angle );
}


//------------------------------------------------------------------------------
// Изменить толщину линий выделенных объектов
// ---
void Manager::SetSelectedObjectWidth( int w )
{
  DeleteTemporal();
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->SetSelectedObjectWidth( w );
}


//------------------------------------------------------------------------------
// Изменить цвет выделенных объектов
// ---
void Manager::SetSelectedObjectColor( COLORREF newColor )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    wind->SetSelectedObjectColor( newColor );
}

//------------------------------------------------------------------------------
// Получить цвет выделенных объектов
// Для диалога мены света
// ---
COLORREF Manager::GetSelectedObjectColor() const
{
  return TestVariables::BLACK;
}

//------------------------------------------------------------------------------
// Нахождение выделенных объектов
// ---
size_t Manager::GetSelectedObjects( RPArray<MbItem> & modelItems )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    return wind->GetSelectedObjects( modelItems );
  return 0;
}


//------------------------------------------------------------------------------
// Получить цвет выделенния объектов.
// ---
COLORREF Manager::GetSelectColor() const 
{ 
  COLORREF sc = TestVariables::sColor;
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    sc = wind->GetSelectColor(); 
  return sc;
} 


//------------------------------------------------------------------------------
// Цвет выделенния объектов
// ---
void Manager::SetSelectColor( COLORREF newColor ) {
  for ( size_t i = 0; i < workWindows.Count(); i++ ) 
    workWindows[i]->SetSelectColor( newColor );
  ShowModel();
}


//------------------------------------------------------------------------------
// Получить цвет фона окон.
// ---
COLORREF Manager::GetBackgrColor() const 
{
  COLORREF bc = TestVariables::bColor;
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr )
    bc = wind->GetBackgrColor(); 
  return bc;
} 


//------------------------------------------------------------------------------
// Изменить цвет окон
// ---
void Manager::SetBackgrColor( COLORREF newColor )
{
  for ( size_t i = 0; i < workWindows.Count(); i++ ) {
    workWindows[i]->GetDrawTool().SetBackground( newColor );
#ifndef __USE_QT__
    DrawContentWindow( i );
#else // __USE_QT__
    InvalidateWindow( i, true );
#endif // __USE_QT__
  }

  ShowModel();
}


//------------------------------------------------------------------------------
// Установить точку наблюдения
// ---
void Manager::SetViewPoint( double d )
{
  WorkWindow * wind = GetActiveWindow();
  if ( wind != nullptr ) {
    wind->EraseModel();
    wind->SetVista( d );
    wind->ShowModel();
  }
}


//------------------------------------------------------------------------------
// Найти в множестве окно по HWND и (при setActive == true) установить его текущим
// ---
ptrdiff_t Manager::FindWindowIndex( const void * wndId, bool setActive )
{
  for ( size_t i = 0, icnt = workWindows.Count(); i < icnt; i++ ) {
    if ( workWindows[i]->IsMyWnd(wndId) ) {
      if ( setActive )
        activeIndex = i;
      return i;
    }
  }
  return SYS_MAX_T;
}


//------------------------------------------------------------------------------
// Вернуть указатель на окно с заданным HWND
// ---
WorkWindow * Manager::GetWindow( ptrdiff_t index )
{
  if ( index >= 0 && index < (ptrdiff_t)workWindows.Count() )
    return workWindows[index];
  return nullptr;
}


//------------------------------------------------------------------------------
// Вернуть указатель на активное окно
// ---
WorkWindow * Manager::GetActiveWindow() const
{
  if ( activeIndex >= 0 && activeIndex < (ptrdiff_t)workWindows.Count() )
    return workWindows[activeIndex];
  return nullptr;
}


//------------------------------------------------------------------------------
// Добавить новое окно
// ---
ptrdiff_t Manager::AddWindow( WorkWindow * wind )
{
  size_t n = workWindows.Count();
  workWindows.Add( wind );

  if ( wind->IsHideline() && (activeIndex == (ptrdiff_t)n) )
    wind->ShowMapping();
  else
    wind->ShowModel();

  if ( temporal.Exist() )
    temporal.Show( wind );
  if ( cplane.Exist() )
    cplane.Show( wind );

  return workWindows.FindIt( wind );
}


//------------------------------------------------------------------------------
// Удалить окно
// ---
void Manager::DeleteWindow( ptrdiff_t index )
{
  ptrdiff_t wndCnt = (ptrdiff_t)workWindows.Count();
  if ( index >= 0 && index < wndCnt ) {
    const WorkWindow * activeWindow = (activeIndex >= 0 && activeIndex < wndCnt) ? workWindows[activeIndex] : nullptr;

    if ( activeWindow != nullptr ) {
      if ( activeWindow == workWindows[index] ) {
        activeWindow = nullptr;
        activeIndex = -1;
      }
    }

    workWindows.RemoveInd( index );
    wndCnt--;

    if ( activeWindow != nullptr ) {
      activeIndex = -1;
      size_t tempIndex = workWindows.FindIt( activeWindow );
      if ( tempIndex != SYS_MAX_T ) {
        activeIndex = (ptrdiff_t)tempIndex;
      }
    }
  }
}


//------------------------------------------------------------------------------
// Нарисовать окно
// ---
void Manager::DrawContentWindow( ptrdiff_t index )
{
  if ( index >= 0 && index < (ptrdiff_t)workWindows.Count() )
    workWindows[index]->DrawContentWindow( temporal, cplane, workWindows[index]->GetSelectColor(), (workWindows[index]->IsHideline() && index == activeIndex) );
}


//------------------------------------------------------------------------------
// Очистить окно
// ---
void Manager::EraseBackground( ptrdiff_t index )
{
  if ( index >= 0 && index < (ptrdiff_t)workWindows.Count() )
    workWindows[index]->EraseBackground();
}


//------------------------------------------------------------------------------
// Очистить окно
// ---
void Manager::EraseWindow( ptrdiff_t index ) {
  InvalidateWindow( index, true );
}


//------------------------------------------------------------------------------
// Перерисовать содержимое окна
// ---
void Manager::InvalidateWindow( ptrdiff_t index, bool bErase )
{
  if ( index >= 0 && index < (ptrdiff_t)workWindows.Count() )
    GetWindow(index)->InvalidateWindow( bErase );
}


//------------------------------------------------------------------------------
// Установить фокус на окно
// ---
void Manager::SetFocus( ptrdiff_t index, bool mdiActivate )
{
  if ( index >= 0 && index < (ptrdiff_t)workWindows.Count() ) {
    GetWindow(index)->SetFocus( mdiActivate );
    SetActiveWindow( index );
  }
}


//------------------------------------------------------------------------------
// Очистить карту окна
// ---
void Manager::RefreshWindow( ptrdiff_t index )
{
  if ( index >= 0 && index < (ptrdiff_t)workWindows.Count() ) {
    if ( temporal.Exist() )
      temporal.CalculateMesh();
    if ( cplane.Exist() )
      cplane.CalculateMesh();
  }
}


//------------------------------------------------------------------------------
// Наличие окна с растровым изображением
// ---
bool Manager::IsWindowRenderType() const
{
  bool grid = false; // Растровое отображение
  WorkWindow * wind = GetActiveWindow(); // Вернуть указатель на активное окно
  if ( wind ) {
    if ( wind->GetRenderType() != wwrt_GDI )
      grid = true;
  }
  return grid;
}


//------------------------------------------------------------------------------
// Установка нуля в центр вида
// ---
void Manager::SetOriginIntoCentre( bool b )
{
  WorkWindow * wind = GetActiveWindow(); // Вернуть указатель на активное окно
  if ( wind )
    wind->SetOriginIntoCentre( b );
}


//------------------------------------------------------------------------------
// Дать прогиб для расчета отрисовочной сетки
// ---
double Manager::GetVisualSag() const {
  return Math::visualSag;
}


//------------------------------------------------------------------------------
// Дать масштаб активного окна
// ---
double Manager::GetWindowScale()
{
  if ( activeIndex > -1 && activeIndex < (ptrdiff_t)workWindows.Count() )
    return workWindows[activeIndex]->GetScale();
  return 1;
}


//------------------------------------------------------------------------------
// Установка масштаба для габарита
// ---
void Manager::SetWindowScale( const MbCube & gab )
{
  WorkWindow * currentView = GetActiveWindow(); // Вернуть указатель на активное окно
  if ( currentView != nullptr ) {
    currentView->EraseWindow(); // KYA медленно // currentView->EraseModel( drawModel, backgr ); // Стереть

    double d = gab.GetDiagonal();
    MbCartPoint3D c;
    gab.GetCentre( c );
    currentView->CalculateScaleFor( d, d );
    currentView->SetCentreIntoPoint( c, true );
    // KYA обновится по WM_PAINT // currentView->ShowModel ( drawModel, select ); // Нарисовать
    RefreshWindow( activeIndex );
    InvalidateWindow( activeIndex, false );
    SetFocus( activeIndex, false );
    currentView->SetVisualSag(); // SD#739011
  }
}


//------------------------------------------------------------------------------
// Установить размер видимой области
// ---
void Manager::SetSize( ptrdiff_t index/*HWND hw*/, int cx, int cy )
{
  if ( index >= 0 && index < (ptrdiff_t)workWindows.Count() ) {
    if ( workWindows[index]->GetRenderType() == wwrt_OGL )
      workWindows[index]->SetSize( cx, cy );
    else if ( workWindows[index]->GetRenderType() == wwrt_GDI ) {
      ; // Ничего не делаем
    }
    else {
      PRECONDITION( false );
    }
  }
}


//------------------------------------------------------------------------------
// Отрисовать
// ---
void Manager::ReDrawGL()
{
  WorkWindow * currentView = GetActiveWindow(); // Вернуть указатель на активное окно
  if ( currentView ) {
    if ( currentView->GetRenderType() == wwrt_OGL ) {
      currentView->EraseModel();
      currentView->ShowModel();
    }
  }
}


//------------------------------------------------------------------------------
// Удалить данные
// ---
void Manager::DeleteGL()
{
  for ( ptrdiff_t i = workWindows.MaxIndex(); i >= 0; i-- ) {
    if ( workWindows[i]->GetRenderType() == wwrt_OGL )
      workWindows[i]->EraseModel();
  }
}


//------------------------------------------------------------------------------
// Дать способ отображения OGL активного окна
// ---
OGLViewMode Manager::GetWindowOGLViewType()
{
  WorkWindow * currentView = GetActiveWindow(); // Вернуть указатель на активное окно
  if ( currentView )
    return currentView->GetRenderTypeOGL();
  return ovm_NormalRenderWithEdges;
}


//------------------------------------------------------------------------------
// Установить способ отображения OGL активного окна
// ---
void Manager::SetWindowOGLViewType( OGLViewMode viewMode )
{
  WorkWindow * currentView = GetActiveWindow(); // Вернуть указатель на активное окно
  if ( currentView && viewMode >= ovm_NormalRender && viewMode < ovm_LastViewType )
    currentView->SetRenderTypeOGL( viewMode );
}

//------------------------------------------------------------------------------
// Выдать свойство
// ---
PropertyWindow * Manager::FindProperty( const void * wndId ) const
{
  for ( size_t i = 0, icnt = propertyWindows.Count(); i < icnt; i++ ) 
  {
    if ( propertyWindows[i]->IsMyWnd( wndId ) )
      return propertyWindows[i];
  }
  return nullptr;
}

//------------------------------------------------------------------------------
// Выдать окно дерева модели
// ---
IModelTreeWindow * Manager::FindTree ( const void * wndId ) const
{
  for ( size_t i = 0, icnt = treeWindows.Count(); i < icnt; ++i )
  {
    if ( treeWindows[i]->IsMyWnd( wndId ) )
      return treeWindows[i];
  }
  return nullptr;
}

//------------------------------------------------------------------------------
// Выдать окно фильтра
// ---
ObjectFilterWindow * Manager::FindFilter ( const void * wndId ) const
{
  for ( size_t i = 0, icnt = filterWindows.Count(); i < icnt; ++i )
  {
    if ( filterWindows[i]->IsMyWnd ( wndId ) )
      return filterWindows[i];
  }
  return nullptr;
}

//------------------------------------------------------------------------------
// Проверка на правильность
// ---
bool Manager::IsMyProperty( const PropertyWindow * wp ) const
{
  for ( size_t i = 0, icnt = propertyWindows.Count(); i < icnt; i++ ) {
    if ( propertyWindows[i] == wp )
      return true;
  }

  return false;
}

//------------------------------------------------------------------------------
// Проверка на правильность
// ---
bool Manager::IsMyTree ( const IModelTreeWindow * wp ) const
{
  for ( size_t i = 0, icnt =treeWindows.Count(); i < icnt; i++ ) {
    if ( treeWindows[i] == wp )
      return true;
  }

  return false;
}

//------------------------------------------------------------------------------
// Записать свойство
// ---
void Manager::SetProperty( PropertyWindow * wp )
{
  if ( wp != nullptr ) {
    wp->SetOwnProperties();
    if ( wp->SetObj() != nullptr ) {
      PrePropType prePropType = wp->GetPropType();
      if ( prePropType == pt_ItemProp )
        RefreshObjectModel( (MbItem *)wp->SetObj() );
    }
  }
}


//------------------------------------------------------------------------------
// Создать и добавить новое свойство
// ---
void Manager::AddProperty( PropertyWindow * newProp )
{
  if ( newProp != nullptr )
    propertyWindows.Add( newProp );
}


//------------------------------------------------------------------------------
// Удалить свойство
// ---
void Manager::RemoveProperty( PropertyWindow * wp, bool redraw )
{
  PRECONDITION( wp );

  for ( ptrdiff_t i = propertyWindows.MaxIndex(); i >= 0; i-- ) 
  {
    PropertyWindow * propWnd = propertyWindows[i];
    if ( propWnd != nullptr && propWnd == wp ) {
      MbItem * propObj = nullptr;

      if ( propWnd->GetObj() != nullptr ) {
        PrePropType prePropType = propWnd->GetPropType();
        if ( prePropType == pt_ItemProp )
          propObj = (MbItem *)propWnd->SetObj();
      }

      propertyWindows.RemoveInd( i );

      if ( propObj != nullptr && redraw )
      {        
        if ( WorkWindow * wind = GetActiveWindow() ) 
        {
          MbItem * geom = propObj;//::FindModelItem( geomModel, propObj, matr ); // Геометрический объект
          wind->RemoveProperty( geom );
        }
      }
      break;
    }
  }
}

//------------------------------------------------------------------------------
// Удалить окно дерева модели
// ---
void Manager::RemoveTree ( IModelTreeWindow * wp )
{
  PRECONDITION ( wp );

  for ( ptrdiff_t i = treeWindows.MaxIndex(); i >= 0; --i ) 
  {
    IModelTreeWindow * propWnd = treeWindows[i];
    if ( propWnd != nullptr && propWnd == wp ) {
      treeWindows.RemoveInd ( i );
    }
  }
}

//------------------------------------------------------------------------------
// Удалить окно фильтра
// ---
void Manager::RemoveFilter ( ObjectFilterWindow * wp )
{
  PRECONDITION ( wp );

  for ( ptrdiff_t i = filterWindows.MaxIndex(); i >= 0; --i ) 
  {
    ObjectFilterWindow * propWnd = filterWindows[i];
    if ( propWnd != nullptr && propWnd == wp ) {
      filterWindows.RemoveInd ( i );
    }
  }
}

//------------------------------------------------------------------------------
// Инициализировать временный объект для редактирования
// ---
void Manager::InitCplane( MbItem * obj, MbTopItem * item, const MbMatrix3D & m, double a )
{
  bool b = cplane.Exist();

  if ( cplane.Init( obj, item, m, a ) ) {
    WorkWindow * wind = GetActiveWindow();

    if ( b )
      cplane.Erase( wind );
    cplane.CalculateMesh();
    cplane.Show( wind );
  }
}


//------------------------------------------------------------------------------
// Дать временные объекты
// ---
const MbItem * Manager::GetCplaneObject() const {
  return cplane.Exist() ? cplane.GetObject() : nullptr;
}


//------------------------------------------------------------------------------
// Дать временные объекты
// ---
const MbTopItem * Manager::GetCplaneItem() const {
  return cplane.Exist() ? cplane.GetTopoItem() : nullptr;
}


//------------------------------------------------------------------------------
// Перерисовать временную плоскость
// ---
void Manager::RedrawCplane()
{
  if ( cplane.Exist() ) {
    WorkWindow * wind = GetActiveWindow();

    cplane.Erase( wind );
    cplane.Reset();
    cplane.CalculateMesh();
    cplane.Show( wind );
  }
}


//------------------------------------------------------------------------------
// Дать плейсемент
// ---
const MbPlacement3D & Manager::GetCplane() {
  return cplane.SetPlacement( GetActiveWindow() ); // Или окна (собственный), или конструктивной плоскости
}


//------------------------------------------------------------------------------
// Дать матрицу
// ---
MbMatrix3D Manager::GetCplaneMatrixFrom() {
  return ( GetCplane().GetMatrixFrom() ); // Или окна (собственный), или конструктивной плоскости
}


//------------------------------------------------------------------------------
// Дать точку конструктивной плоскости
// ---
bool Manager::GetCplanePoint( const MbCartPoint3D & cursorPoint, MbCartPoint & pnt )
{
  const WorkWindow * wind = GetActiveWindow();
  MbPlacement3D pl( wind->GetOwnPlace() );
  //MbPlacement3D pl(GetCplane());
  return pl.DirectPointProjection(cursorPoint,wind->GetAxisZ(),pnt.x,pnt.y);
}


//------------------------------------------------------------------------------
// Дать вектор сдвига в конструктивной плоскости
// ---
void Manager::GetCplaneMoveVector( const MbItem * obj, const MbVector3D & vect, MbVector & to )
{
  MbCartPoint3D cPoint;

  if ( obj != nullptr && obj->IsA() == st_PlaneInstance ) {
    MbPlacement3D pl(((MbPlaneInstance *)obj)->GetPlacement());

    cPoint = pl.GetOrigin() + vect;
    if ( ! pl.DirectPointProjection(cPoint,GetActiveWindow()->GetAxisZ(),to.x,to.y) )
      to.Init(0,0);
  }
}


//------------------------------------------------------------------------------
// Дать неподвижную точку и угол в конструктивной плоскости
// ---
void Manager::GetCplaneRotateAxis( const MbAxis3D & axis, double angle, MbCartPoint & pnt, MbDirection & ang )
{
  MbPlacement3D pl(GetCplane());

  ang = angle * (pl.GetAxisZ() * axis.GetAxisZ());
  if ( !pl.DirectPointProjection(axis.GetOrigin(),axis.GetAxisZ(),pnt.x,pnt.y) ) {
    pnt.Init(0,0);
    ang = 0;
  }
}


//------------------------------------------------------------------------------
// Дать неподвижную точку и угол в конструктивной плоскости
// ---
void Manager::GetCplaneTransformMatrix( const MbMatrix3D & matr, MbMatrix & m )
{
  MbPlacement3D pl( GetCplane() );
  MbMatrix3D mr;

  mr = matr * pl.GetMatrixFrom();
  mr = pl.GetMatrixInto() * mr;

  m.El( 0, 0, mr.El( 0, 0 ) );
  m.El( 1, 0, mr.El( 1, 0 ) );
  m.El( 2, 0, mr.El( 3, 0 ) );
  m.El( 0, 1, mr.El( 0, 1 ) );
  m.El( 1, 1, mr.El( 1, 1 ) );
  m.El( 2, 1, mr.El( 3, 1 ) );
  m.El( 0, 2, mr.El( 0, 3 ) );
  m.El( 1, 2, mr.El( 1, 3 ) );
  m.El( 2, 2, mr.El( 3, 3 ) );
}


//------------------------------------------------------------------------------
// Инициализировать временный объект для редактирования
// ---
void Manager::InitTemporal( MbItem * obj, const MbMatrix3D & matr )
{
  temporal.Init( obj, matr );
  temporal.CalculateMesh();
  temporal.Show( GetActiveWindow() );
}


//------------------------------------------------------------------------------
// Дать временные объекты
// ---
MbItem * Manager::GetObjectTarget()
{
  if ( temporal.Exist() )
    return temporal.SetObject();
  return nullptr;
}


//------------------------------------------------------------------------------
// Перерисовать временный объект
// ---
void Manager::RedrawTemporal()
{
  if ( temporal.Exist() ) {
    WorkWindow * wind = GetActiveWindow();
    temporal.Erase( wind );
    temporal.CalculateMesh();
    temporal.Show( wind );
  }
}


//------------------------------------------------------------------------------
// Сдвиг
// ---
void Manager::MoveTemporal( ptrdiff_t /*index*/, const MbVector3D & to, bool target )
{
  WorkWindow * wind = GetActiveWindow(); // Вернуть указатель на активное окно
  PRECONDITION( wind != nullptr );

  if ( temporal.Exist() && (wind != nullptr) ) {
    MbVector t;
    if ( temporal.GetObject() != nullptr )
      GetCplaneMoveVector( temporal.GetObject(), to, t );

    temporal.Erase( wind );

    if ( temporal.Move( TestVariables::_tindex, to, t, target ) >= 0 )
      wind->RefreshOGL();
    if ( temporal.GetObject() != nullptr )
      RedrawObject( temporal.SetObject() );

    temporal.CalculateMesh();
    temporal.Show( wind );
  }
}


//------------------------------------------------------------------------------
// Сдвиг
// ---
void Manager::MoveTemporal( const MbVector3D & to, bool /*target*/ )
{
  WorkWindow * wind = GetActiveWindow(); // Вернуть указатель на активное окно
  PRECONDITION( wind != nullptr );

  if ( temporal.Exist() && (wind != nullptr) ) {
    MbVector t;
    if ( temporal.GetObject() != nullptr )
      GetCplaneMoveVector( temporal.GetObject(), to, t );

    temporal.Erase( wind );

    if ( temporal.Move( to, t ) >= 0 )
      wind->RefreshOGL();
    if ( temporal.GetObject() != nullptr )
      RedrawObject( temporal.SetObject() );

    temporal.CalculateMesh();
    temporal.Show( wind );
  }
}


//------------------------------------------------------------------------------
// Дать точку
// ---
ptrdiff_t Manager::GetTemporalPoint( ptrdiff_t tindex, ptrdiff_t n, MbCartPoint3D & p3, MbCartPoint & p2 ) {
  return temporal.GetPoint( tindex, n, p3, p2 );
}


//------------------------------------------------------------------------------
// Установить точку
// ---
void Manager::SetTemporalPoint( ptrdiff_t tindex, ptrdiff_t n, ptrdiff_t np, MbCartPoint3D & p3, MbCartPoint & p2 )
{
  WorkWindow * wind = GetActiveWindow(); // Вернуть указатель на активное окно
  PRECONDITION( wind != nullptr );

  if ( temporal.Exist() && (wind != nullptr) ) {
    temporal.Erase( wind );

    ptrdiff_t k;
    if (np == 3) k = temporal.SetPoint( tindex, n, p3 );
    else         k = temporal.SetPoint( tindex, n, p2 );

    if ( temporal.GetObject() != nullptr )
      RedrawObject( temporal.SetObject() );

    temporal.CalculateMesh();
    temporal.Show( wind );
  }
}


//------------------------------------------------------------------------------
// Нахождение ближайшего элемента по проекции на плоскость
// От заданной точки в данной проекции
// ---
bool Manager::FindNearestTemporal( const MbFloatAxis3D & from, double distance,
                                   ptrdiff_t & index, ptrdiff_t & n ) const
{
  float d, dmin = MB_MAXFLOAT;
  bool result = false;
  ptrdiff_t i, j;

  index = SYS_MAX_T;
  n = SYS_MAX_T;

  WorkWindow * wind = GetActiveWindow();
  double maxDistance = wind ? wind->ScaleDistance( distance ) : distance;

  if ( !temporal.Exist() )
    return false;

  for ( i = 0; i < (ptrdiff_t)temporal.GetTenderCount(); i++ ) {
    d = ::DistanceToPoint( temporal.GetTender( i ), from, (float)maxDistance, j );
    if ( d < maxDistance && d < dmin ) {
      dmin = d;
      index = i;
      n = j;
      result = true;
    }
  }

  return result;
}


//----------------------------------------------------------------------------------------
//
//---
bool Manager::AddConstraint( const MbConstraint & constr )
{
  SPtr<MbConstraintItem> cItem( new MbConstraintItem(constr) );
  if ( constraintManager2D.AddConstraint( cItem->GceConstraint() ) ) {
    if ( constraintManager2D.Evaluate() ) {
      constraintManager2D.ApplySolution();
      constraintModel2D.AddConstraint( cItem );
      // Оптимальнее делать частичную перерисовку, но пока оставим, как есть - перерисуем всю графическую сцену
      RefreshModel();
      return true;
    }
    else {
      // ... удалить
    }
  }
  return false;
}

//----------------------------------------------------------------------------------------
//
//---
bool Manager::AddConstraint( MtMateType cType, const MtGeomArgument & a1, const MtGeomArgument & a2
                           , MtParVariant parVar )
{
  if ( constraintManager3D.AddConstraint(cType, a1, a2, parVar) )
  {
    if ( OK(constraintManager3D.Evaluate()) )
    {
      constraintManager3D.Apply();
      // Оптимальнее делать частичную перерисовку, но пока перерисуем всю графическую сцену
      RefreshModel();
      return true;
    }
  }
  return false;
}

//----------------------------------------------------------------------------------------
// Пересчитать все системы сопряжений подсборок (with no redrawing)
//---
MtResultCode3D Manager::EvaluateConstraints()
{
  return constraintManager3D.Evaluate();
}

////////////////////////////////////////////////////////////////////////////////
//
//                             Неклассные функции
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Копия кривой в системе координат окна.
// item - Кривая на плоскости.
// ---
const MbCurve * GetCurveCopyOnWindPlace( const Manager & viewManager, const MbItem & item )
{
  const MbCurve * pCurve = nullptr;

  if ( item.IsA() == st_PlaneInstance ) {
    // Кривые могут быть в разных системах координат.
    const WorkWindow * wind = viewManager.GetActiveWindow();
    const MbPlacement3D & windPlace = wind->GetOwnPlacement();
    const MbPlacement3D & curvePlace = static_cast<const MbPlaneInstance &>(item).GetPlacement();
    MbMatrix fromCurvePlace;
    curvePlace.GetMatrixToPlace( windPlace, fromCurvePlace );

    if ( fromCurvePlace.IsSingle() )
      pCurve = static_cast<const MbCurve *>(static_cast<const MbPlaneInstance &>(item).GetPlaneItem());
    else {
      MbCurve * pCopy = static_cast<MbCurve *>(&static_cast<const MbPlaneInstance &>(item).GetPlaneItem()->Duplicate());
      pCopy->Transform( fromCurvePlace );
      pCurve = pCopy;
    }
  }

  return pCurve;
}
