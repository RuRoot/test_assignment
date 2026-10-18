#ifndef __TEST_SERVICE_H
#define __TEST_SERVICE_H

#include <test_prompt.h>
#include <test_style.h>
#include <test_variables.h>
#include <test_window.h>
#include <templ_sptr.h>
#include <plane_item.h>
#include <space_item.h>
#include <topology_item.h>
#include <gcm_res_code.h>
#include <cdm_types.h>
#include <vector>
#include <conv_exchange_settings.h>
#include <conv_model_exchange.h>

//------------------------------------------------------------------------------
// Предоставляет общий интерфейс для работы с битовыми флагами
// Используется для флагов кнопок, box-ов, edit-ов, param-етров и т.д.
// Можно использовать для собственных нужд
// ---
#define UpFlagValue( f, m )         ( (f) |= uint8(m) )
#define DownFlagValue( f, m )       ( (f) &= uint8(~(m)) )
#define TestFlagValue( f, m )       ( (f) & (m) )
#define ToggleFlagValue( f, m )     ( TestFlagValue() ? DownFlagValue(f,m) : UpFlagValue(f,m))
#define UpDownFlagValue( f, m, p )  ( (p) ? UpFlagValue(f,m) : DownFlagValue(f,m) )


class    MbVector;
class    MbVector3D;
class    MbCartPoint;
class    MbCartPoint3D;
class    MbMatrix3D; 
class    MbMatrix; 
class    MbRect1D;
class    MbCurve;
class    MbContour;
class    MbMesh;
class    MbPolymesh;
class    MbCurve3D;
class    MbContour3D;
class    MbSurface;
class    MbPlane;
class    MbContourOnSurface;
class    MbContourOnPlane;
class    MbSurfaceIntersectionCurve;
class    MbCurveEdge;
class    MbLoop;
class    MbFace;
struct   MbItemIndex; 
class    MbSpaceItem;
class    MbPlaneItem;
class    MbTopItem;
class    MbItem;
class    MbSolid;
class    MbMesh;
class    MbSweptData;
class    TimeTest;
class    IGrDraw;
class    WorkWindow;


//----------------------------------------------------------------------------------------
// Послать строку в виде сообщения
// ---
void ShowString( const TCHAR * str ); 


//----------------------------------------------------------------------------------------
// Ввод точки
// ---
bool GetPoint( const TCHAR * strResID, MbCartPoint3D & pnt );


//----------------------------------------------------------------------------------------
// Ввод точки
// ---
bool GetPoint( const TCHAR * nStr, MbCartPoint & pnt, MbPlacement3D * dstPlace = nullptr );


//----------------------------------------------------------------------------------------
// Дать объект
// ---
void CommonGetObj( const TCHAR * nStr );


//----------------------------------------------------------------------------------------
// Указание элемента модели
// ---
bool GetObj( const TCHAR * nStr, MbItem ** find, PathMatrix & pathFrom = TestVariables::itemPathFrom );


//----------------------------------------------------------------------------------------
// Указание объекта модели или его элемента(части)
// ---
bool GetObj( const TCHAR * nStr, MbItem ** find, MbRefItem ** item, PathMatrix & pathFrom = TestVariables::itemPathFrom );


//----------------------------------------------------------------------------------------
// Указание ближайшей к курсору кривой модели 
// ---
bool GetAnyCurve( const TCHAR * nStr, MbItem ** find, PathMatrix & pathFrom = TestVariables::itemPathFrom );


//----------------------------------------------------------------------------------------
// Указание сборки или вставки.
// ---
bool GetRigidObj( const TCHAR * nStr, MbItem *& find, MbRefItem *& geom, PathMatrix & pathFrom = TestVariables::itemPathFrom );


//----------------------------------------------------------------------------------------
// Указание элемента модели по его типу
// ---
SPtr<MbItem> GetGObj( const TCHAR * nStr, MbeSpaceType sType, PathMatrix& pathFrom = TestVariables::itemPathFrom );
bool GetGObj(const TCHAR* nStr, MbeSpaceType sType, MbItem** find, PathMatrix& pathFrom = TestVariables::itemPathFrom);

//----------------------------------------------------------------------------------------
// Указание элемента модели по его типу и типу полигона
// ---
bool GetGObj( const TCHAR * nStr, MbeSpaceType sType, MbItem ** find, MbSpaceItem ** geom, PathMatrix & pathFrom = TestVariables::itemPathFrom );

//-----------------------------------------------------------------------------------------
// Указание произвольного 3D-объекта
// ---
const MbRefItem * GetSObj( const TCHAR * nStr, const MbItem ** find, PathMatrix & pathFrom = TestVariables::itemPathFrom );

//----------------------------------------------------------------------------------------
// Указание элемента модели по его типу и типу полигона
// ---
bool GetTObj( const TCHAR * nStr, MbeSpaceType sType, MbeTopologyType tType, 
              MbItem ** find, MbTopItem ** geom, MbMatrix3D & matr );


//----------------------------------------------------------------------------------------
// Указание элемента модели по его типу и типу полигона
// ---
bool GetTObj( const TCHAR * nStr, MbeSpaceType sType, MbeTopologyType tType,
              MbItem ** find, MbTopItem ** geom, PathMatrix & pathFrom = TestVariables::itemPathFrom );


//----------------------------------------------------------------------------------------
// Указание кривой. Работает как с геометрическими объектами, так и с топологическими (ребра).
// Если выбранный объект является частью более общего объекта (например, ребро каркаса),
// параметру part будет присвоено значение true.
// ---
bool GetCurveObject( const TCHAR * nStr, bool & part, const MbeSpaceType sGeomType, const MbeSpaceType sTopologyType, const MbeTopologyType tType,
                     MbItem ** find, MbRefItem ** item, PathMatrix & pathFrom = TestVariables::itemPathFrom, MbMatrix3D & matr = TestVariables::itemPathFrom.second );


//----------------------------------------------------------------------------------------
// Указание элемента модели по его типу
// ---
bool GetMObj( const TCHAR * nStr, MbePlaneType pType, MbItem ** find, PathMatrix & pathFrom = TestVariables::itemPathFrom );


//----------------------------------------------------------------------------------------
// Указание элемента модели по его типу и типу полигона
// ---
bool GetMObj( const TCHAR * nStr, MbePlaneType pType,
              MbItem ** find, MbPlaneItem ** geom, PathMatrix & pathFrom = TestVariables::itemPathFrom );

//----------------------------------------------------------------------------------------
// Выбрать на экране экземпляр геометрии с учетом повторно-используемых тел.
// Select a geom object, preferably an instance of the re-inserted solid of the model.
//---
MbItem * GetInstance( const TCHAR * nStr, PathMatrix & pathFrom = TestVariables::itemPathFrom );

//----------------------------------------------------------------------------------------
// Нахождение ближайшего элемента к заданной точке
// ---
bool FindNearestObj( MbCartPoint3D & from_p, MbItem ** findObj, PathMatrix & pathFrom = TestVariables::itemPathFrom );


//----------------------------------------------------------------------------------------
// Нахождение ближайшего элемента к заданной точке
// ---
bool FindNearestObj( MbCartPoint & from_p, MbePlaneType typeObj, MbItem ** findObj, PathMatrix & pathFrom = TestVariables::itemPathFrom );


//----------------------------------------------------------------------------------------
// Нахождение ближайшего элемента к заданной точке
// ---
bool FindNearestObj( MbCartPoint3D & from_p, MbePlaneType typeObj, MbItem ** findObj, PathMatrix & pathFrom = TestVariables::itemPathFrom );


//----------------------------------------------------------------------------------------
// Нахождение ближайшего элемента к заданной точке
// ---
bool FindNearestObj( MbCartPoint3D & from_p, MbeSpaceType typeObj, MbItem ** findObj, PathMatrix & pathFrom = TestVariables::itemPathFrom );


//----------------------------------------------------------------------------------------
// Указание элемента модели по его типу и типу полигона
// ---
bool FindNearestObj( MbCartPoint3D & from_p, MbeSpaceType typeObj, MbeTopologyType polyType, 
                     MbItem ** findObj, MbTopItem ** findGeom, MbMatrix3D & matr = TestVariables::itemPathFrom.second );


//----------------------------------------------------------------------------------------
// Установка проекции
// ---
void SetProjection();

//----------------------------------------------------------------------------------------
// Изменить глубину проекции
// ---
bool SetDepth(); 

//----------------------------------------------------------------------------------------
// Установить масштаб вида
// ---
bool SetScale(); 

//----------------------------------------------------------------------------------------
// Ввод булевского значения
// ---
void GetBoolean( const TCHAR* nPrompt, bool & bVal, bool resetToFalse );

//----------------------------------------------------------------------------------------
// Ввод булевского значения
// ---
bool GetBoolean( const TCHAR* nPrompt );


//----------------------------------------------------------------------------------------
// Есть ли такое расширение в имени
//---
bool SameExtension( const TCHAR * fullname, const TCHAR * extname );

//----------------------------------------------------------------------------------------
// Перезаписать или добавить расширение имени файла типа (".ext\0")
//---
void RewriteFileExtension( TCHAR * fullname, const TCHAR * extname, bool forWriting = true );

//----------------------------------------------------------------------------------------
// Чтение модели из файла / Read model from a file
// ---
void ReadModel( const TCHAR * path );

//----------------------------------------------------------------------------------------
// Запись модели в файл / Write model to a file
// ---
void WriteModel( TCHAR * fileName, VERSION dstVersion ); // Last version = GetCurrentMathFileVersion()

//----------------------------------------------------------------------------------------
// Запись тела в файл / Write a solid to a file
// ---
bool WriteSolid( const TCHAR * pathName, MbSolid & solid );

//----------------------------------------------------------------------------------------
// Добавить в модель файл
// ---
void AddModel();

//----------------------------------------------------------------------------------------
// Чтение геометрической модели из файла *.MAT
// Read the geometric model from *.MAT file
// ---
void ReadItem();

//----------------------------------------------------------------------------------------
// Записать геометрической модели в формат *.MAT (Устаревший формат)
// Write the geometric model into format *.MAT (Deprecated format)
// ---
//DEPRECATE_DECLARE
void WriteItem();

//----------------------------------------------------------------------------------------
// Чтение модели из файла по фильтру
// ---
void ReadModelTree ( TCHAR * fileName, EObjectFilter filter);

//----------------------------------------------------------------------------------------
// Запросить опции экспорта.
// Get export options.
// ---
void AskForExportOptions(MbeModelExchangeFormat modelFormat, ConvConvertorProperty3D& convProperties);

//----------------------------------------------------------------------------------------
// Запросить опции импорта.
// Get import options.
// ---
void AskForImportOptions(MbeModelExchangeFormat modelFormat, ConvConvertorProperty3D& convProperties);

//----------------------------------------------------------------------------------------
// Запустить консоль.
// Run the console.
// ---
void RunConsole();

//----------------------------------------------------------------------------------------
// Выдать информацию о времени работы
// ---
void ShowTimeTestResult( TimeTest * testResult, 
                         uint32 miliSec, 
                         const TCHAR * sss );

//----------------------------------------------------------------------------------------
// Выдать информацию о времени работы
// ---
void ShowTimeTestResult( TimeTest * testResult, 
                         size_t operationCount, 
                         size_t facesCount, 
                         uint32 miliSec, 
                         MbeSpaceType itemType,
                         const TCHAR * sss );

//----------------------------------------------------------------------------------------
// Выдать информацию о времени работы теста сопряжений.
// ---
void ShowMateTimeTestResult( TimeTest *     testResult
                           , size_t         mateCount
                           , size_t         bodyCount
                           , GCM_result result
                           , const TCHAR *  sss );

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
                         const TCHAR * sss );

//----------------------------------------------------------------------------------------
// Output the collision detection summary.
// facePairsNb - the number of unigue pairs of faces for which intersection was found.
// ---
void ShowInterferenceResult( TimeTest * testResult, size_t facePairsNb, uint32 miliSec );

//----------------------------------------------------------------------------------------
// Выдаст сообщение о причине неудачи построения solid'а
//---
void PutErrorMessage( MbResultType, const TCHAR * name  ); 
void PutErrorMessage( uint solidError, const TCHAR * name );
void PutResultMessage( cdet_result, const TCHAR * name  );

//----------------------------------------------------------------------------------------
// Выдаст сообщение о причине неудачи построения solid'а
//---
void StitchErrorMessage( int stitchError, const TCHAR * name  ); 


//----------------------------------------------------------------------------------------
// Провести неоконные действия при инициализации главного окна задачи.
// Perform non-window actions when initializing the main task window.
// ---
void SystemInitFrame(); 

//----------------------------------------------------------------------------------------
// Ввести данные о лицензионном ключе.
// Enter license key.
// ---
void EnterLicenseKey(); 

//----------------------------------------------------------------------------------------
// Показать данные о текущей активированной лицензии.
// Show status of the currently activated license.
// ---
void ShowLicenseStatus(); 

//----------------------------------------------------------------------------------------
// Активировать лицензию по ключу и подписи из файла c3d.lic в папке пользователя.
// License activation using the key saved in the c3d.lic file in the user directory.
// ---
void ActivateLicenseFromFile(); 

//----------------------------------------------------------------------------------------
// Заполнить вектор пар ключ/значение из файла.
// Filling a vector of key/value pairs from a file.
// ---
bool ReadKeysFromFile( std::ifstream & file, std::list<std::pair<std::string,std::string>> & keys ); 

//----------------------------------------------------------------------------------------
// Записать вектор пар ключ/значение в файл
// Writing a vector of key/value pairs to a file
// ---
bool WriteKeysToFile( std::list<std::pair<std::string,std::string>> & keys ); 

//----------------------------------------------------------------------------------------
// Проверить, активирована ли лицензия для любого из компонентов ядра.
// Check if the license is activated for any of the kernel components.
// ---
bool IsLicenseActive(); 

//----------------------------------------------------------------------------------------
// Функции из других файлов.
// Functions from other files.
// ---
bool    GetString( const TCHAR* nPrompt, const TCHAR * defstr, TCHAR * s, size_t len );
bool    GetInt( const TCHAR* nPrompt, int & iresult ); // Ввод целого числа
#if defined(PLATFORM_64)
bool    GetInt( const TCHAR* nPrompt, ptrdiff_t & iresult ); // Ввод целого числа
#endif // PLATFORM_64
bool    GetFloat( const TCHAR * nPrompt, float & fresult ); // Ввод вещественного числа
bool    GetDouble( const TCHAR * nPrompt, double & dresult, const MbRect1D * range = nullptr ); // Ввод вещественного числа
bool    GetYesNoChoice( const TCHAR * nPrompt, const TCHAR * nTitle, bool defaultFirstButton = true ); // Окно выбора из двух вариантов (да/нет)
bool    GetDataCharCurve3D( c3d::string_t& fx, c3d::string_t& fy, c3d::string_t& fz, c3d::string_t& arg );
bool    GetExactPoint( MbCartPoint3D & cursorPoint ); // Ввод точных координат точки
bool    GetExactPlanePoint( MbCartPoint & cursorPlanePoint ); // Ввод точных координат точки на плоскости
bool    GetChamferParameters( void * chamferParams, size_t nVertexes );
bool    GetLicenseSigKey( std::string & key, std::string & sig, bool * save2file );
bool    CMUColor( COLORREF & crColor ); // Изменить цвет элемента в активном окне
bool    SetSurfaceDegree(); // Установить порядок поверхности

ptrdiff_t ExecuteContourDlg();       // Процедура ввода контура
ptrdiff_t ExecuteChooseDirectionDlg();  // Выбор направления

bool    _GetSaveFileName( TCHAR * nameBuffer, const TCHAR * title, const TCHAR * filter, const TCHAR * extension ); // Запрос у пользователя имени файла для записи 
bool    _GetOpenFileName( TCHAR * nameBuffer, const TCHAR * title, const TCHAR * filter, const TCHAR * extension ); // Запрос у пользователя имени файла для чтения

void    SetWaitCursor( bool set ); // Установить/снять курсор в виде песочных часов
void    MessageBoxEx( const TCHAR * text, const TCHAR * title ); // Выдать информационное сообщение
bool    PeekEscape(); // Проверить очередь клавиатуры на нажатие клавиши [ESC] 
bool    WaitingKeyboardEvent(); // Ожидание нажатия на клавиатуру

COLORREF GetSystemWindowColor(); 

void    SyncActiveWindowIndex(); // Синхронизовать индекс активного окна с менеджером окон

// Главное окно задачи
bool    CreateFrameWindow();    // Создать главное окно задачи
void    DestroyFrameWindow();   // Удалить главное окно задачи
// Рабочее окно
bool    CreateViewWindow( uint command ); // Создать рабочее окно 

void    SetViewVector(); // Расположить по вектору взгляда.
// Установить ориентацию окна
void    SetViewWindow( wProjections proj );


// StatusBar
void SetStatusBarText( const TCHAR* txt );  // Отрисовать текст
void SetStatusBarPoint( const MbCartPoint3D& pnt );  // Отрисовать координаты точки

bool ExecuteSetProjectionDlg( MbMatrix3D& matrix );
bool ExecuteSetOrientationDlg( MbMatrix3D& matrix );
bool ExecuteTimeTestResultDlg( const void * params ); 
bool GetBool( const TCHAR* nPrompt, bool defVal );


#ifdef __USE_QT__ 

  #include <QString>
  QString QString_fromT(const TCHAR * text);

  class QWidget;
  QWidget* CreateMdiClient( const TCHAR* title ); // Создать клиенское окно для рисования модели
  IGrDraw * CreateDrawTool( QWidget* wnd, WorkWindow& parent, const MbVector3D & light );  // Создание отрисовщика

#else // __USE_QT__ 

  HWND      CreateMdiClient( const TCHAR* title );  // Создать клиенское окно для рисования модели
  IGrDraw * CreateDrawTool( HWND wnd, WorkWindow& parent, const MbVector3D & light );  // Создание отрисовщика

#endif // __USE_QT__ 


//----------------------------------------------------------------------------------------
// Select a mesh
// ---
MbMesh * SelectMesh( MbMatrix3D * from = nullptr );

//----------------------------------------------------------------------------------------
// Select a polymesh
// ---
MbPolymesh * SelectPolymesh( MbMatrix3D * from = nullptr );

//----------------------------------------------------------------------------------------
// Select a wire frame
// ---
MbWireFrame * SelectWireFrame( MbMatrix3D * from = nullptr );


//----------------------------------------------------------------------------------------
// Select an edge
// ---
MbEdge * SelectEdge( MbItem * & obj );

//----------------------------------------------------------------------------------------
// Select a solid
// ---
MbSolid * SelectSolid( MbMatrix3D * from = nullptr );

//----------------------------------------------------------------------------------------
// Select a solid
// ---
MbSolid * SelectSolid( PathMatrix & pathFrom );

//----------------------------------------------------------------------------------------
// Select shell faces
// ---
MbSolid * SelectShellFaces( c3d::FacesVector & faces, PathMatrix * pathFrom = nullptr, bool drawItems = true );

//----------------------------------------------------------------------------------------
// Select shell faces
// ---
MbSolid * SelectShellFaces( RPArray<MbFace> & faces, PathMatrix * pathFrom = nullptr, bool drawItems = true );

//----------------------------------------------------------------------------------------
// Select a face
// ---
MbFace * SelectFace( MbMatrix3D * from = nullptr );

//----------------------------------------------------------------------------------------
// Select a surface
// ---
MbSurface * SelectSurface( MbMatrix3D * from = nullptr );

//----------------------------------------------------------------------------------------
// Select a curve copy
// ---
SPtr<MbCurve3D> SelectAnyCurveCopy( PathMatrix & pathFrom, std::vector< SPtr<MbCurve3D> > * otherCurves = nullptr, MbItem ** curveCarrier = nullptr );

//----------------------------------------------------------------------------------------
// Select a point
// ---
bool SelectPoint( MbCartPoint3D & );

//------------------------------------------------------------------------------
// Select generatrix
// ---
MbSweptData * SelectSweptData( bool bWireFrame );


//----------------------------------------------------------------------------------------
//  Порт необходимого тестовому приложению функционала из system_dependency_utils.h
//----------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// \ru Используется во многих местах для замеров производительности при отладке \en Used in many places for performance measurements in debug mode 
// ---
inline uint32 GetTickCountEx()
{
#ifndef  C3D_WINDOWS //_MSC_VER 
  tms tm;
  return 10. * times(&tm);  // микросекунды, поэтому умножаем на 10, чтобы привести тоже к миллисекундам
#else // C3D_WINDOWS 
  return ::GetTickCount();  // миллисекунды
#endif // C3D_WINDOWS 
}

//------------------------------------------------------------------------------
// \ru Кросс-платформенная функция сна. \en Cross-platform sleep function.
// ---
inline void SleepCP( uint32 msec )
{
#ifdef C3D_WINDOWS //_MSC_VER
  ::Sleep( msec );
#else
  usleep( msec * 1000 ); // usleep takes sleep time in us (1 millionth of a second)
#endif
}

//------------------------------------------------------------------------------
// \ru Используется только в tool_time_test.cpp \en Used only in tool_time_test.cpp  
// ---
inline void ThreadPriority( bool normalPriority )
{
#ifdef C3D_WINDOWS //_MSC_VER
  ::SetThreadPriority( ::GetCurrentThread(), normalPriority ? THREAD_PRIORITY_NORMAL : THREAD_PRIORITY_ABOVE_NORMAL );
#else // C3D_WINDOWS
  if ( normalPriority ) 
  {
    // Для подавления warnings.
  }
#endif // C3D_WINDOWS
}

#endif // __TEST_SERVICE_H
