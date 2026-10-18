#ifndef __TEST_VARIABLES_H
#define __TEST_VARIABLES_H


#include <test_style.h>
#include <mb_matrix3d.h>
#include <mesh_float_point3d.h>
#include <mb_placement3d.h>
#include <attr_color.h>
#include <model_item.h>
#include <templ_sptr.h>
#include <utility>


//#define __EXPORT(retType) __declspec( dllexport ) retType CALL_DECLARATION

// #define ENABLE_IMPORT_FROM_OB_FILE // Включить после полного перехода на C++17!!!

#define BLACK        colors[0]
#define BLUE         colors[1]
#define GREEN        colors[2]
#define CYAN         colors[3]
#define RED          colors[4]
#define MAGENTA      colors[5]
#define BROWN        colors[6]
#define LIGHTGRAY    colors[7]
#define DARKGRAY     colors[8]
#define LIGHTBLUE    colors[9]
#define LIGHTGREEN   colors[10]
#define LIGHTCYAN    colors[11]
#define LIGHTRED     colors[12]
#define LIGHTMAGENTA colors[13]
#define YELLOW       colors[14]
#define WHITE        colors[15]
#define GOLDEN       colors[16]

#define SENSE_RADIUS      16                // Радиус чувствительности к операции указания
#define SENSE_AREA        250               // Максимальный радиус чувствительности к операции указания
#define SHOW_COORDINATES  WM_USER + 100
#define SHOW_STRING       WM_USER + 101
#define PASS_FOCUS        WM_USER + 102
#define SPEEDBAR_HEIGHT   24
#define STRINGLENGTH      256

#define WCS_AXIS_LENGTH    20
#define WCS_AXIS_THICKNESS 8

#define LPARAM_TO_XPOS(lp)  ((int)(short)LOWORD(lp))
#define LPARAM_TO_YPOS(lp)  ((int)(short)HIWORD(lp))

#ifndef __USE_QT__
#define ALL_EXT       _T("*\0") // Все расширения.
#else
#define ALL_EXT       _T("*")   // Все расширения.
#endif
#define NON_EXT       _T("\0")  // Выбранная группа расширений.
#define C3D_EXT       _T(".c3d\0")
#define MAT_EXT       _T(".mat\0")
#define BMP_EXT       _T(".bmp\0")
#define TXT_EXT       _T(".txt\0")
#define SAT_EXT       _T(".sat\0")
#define SAB_EXT       _T(".sab\0")
#define IGS_EXT       _T(".igs\0")
#define IGES_EXT      _T(".iges\0")
#define X_T_EXT       _T(".x_t\0")
#define X_B_EXT       _T(".x_b\0")
#define STP_EXT       _T(".stp\0")
#define STEP_EXT      _T(".step\0")
#define JT_EXT        _T(".jt\0")
#define STL_EXT       _T(".stl\0")
#define VRML_EXT      _T(".wrl\0")
#define THREEMF_EXT   _T(".3mf\0")
#define OBJ_EXT       _T(".obj\0")
#define GRDECL_EXT    _T(".grdecl\0")
#define ASC_EXT       _T(".asc\0")
#define XYZ_EXT       _T(".xyz\0")
#define OB_EXT        _T(".ob\0")


// Ключи и значения настроек, передаваемых через конфигурационный файл c3d_test.config.
// Names and values of setting the application gets from the c3d_test.config file.

// префикс настройки конверетров 
// prefix of converter's settings
#define CONFIG_CONV_SETTINGS_REPAIR_MESH             _T("converter.settings.repair_mesh.")
#define CONFIG_CONV_SETTINGS_DEBUG                   _T("converter.settings.debug.")
#define CONFIG_CONV_SETTINGS                         _T("converter.settings.")

// Активация лечения неправильных сеток
#define CONFIG_CONV_SETTINGS_REPAIR_MESH_ENABLE               _T("enable")

// Названия полей класса MbRepairInconsistentMeshParams, которые переопределяются при передаче в конвертер
// через вызов интерфейса IConvertorProperty3D::RepairInconsistentMesh

// Names of fields of the MbRepairInconsistentMeshParams class, which are defined for
// the IConvertorProperty3D::RepairInconsistentMesh method call
#define CONFIG_CONV_SETTINGS_REPAIR_MESH_ORIENT_OUTWARD       _T("orientOutward")
#define CONFIG_CONV_SETTINGS_REPAIR_MESH_TOLERANCE            _T("tolerance")
#define CONFIG_CONV_SETTINGS_REPAIR_MESH_ANGLE_SHARP          _T("angleSharp")
#define CONFIG_CONV_SETTINGS_REPAIR_MESH_NORMAL_AVERAGING     _T("normalAveraging")

// Названия полей структуры C3DConverterDebugSettings, которые переопределяются при передаче в конвертер
// через вызов интерфейса IConvertorProperty3D::GetDebugSettings

// Names of fields of the C3DConverterDebugSettings structure, which are defined for
// the IConvertorProperty3D::GetDebugSettings method call
#define CONFIG_CONVERTER_DEBUG_SETTING_ENABLE_CERR_OUT        _T("enableCERRout")
#define CONFIG_CONVERTER_DEBUG_SETTING_LOGGING_MODE           _T("loggingMode")
#define CONFIG_CONVERTER_DEBUG_SETTING_USER_TREE              _T("cerrOutUserTreeTraverse")
#define CONFIG_CONVERTER_DEBUG_SETTING_IM_TREE                _T("cerrOutIntermediateTreeTraverse")
#define CONFIG_CONVERTER_DEBUG_SETTING_OUT_STATISTICS         _T("cerrOutImportStatistic")
#define CONFIG_CONVERTER_DEBUG_SETTING_SAVE_TWIN              _T("saveModelTwin")
#define CONFIG_CONVERTER_DEBUG_SETTING_ID_FINE_DEBUG          _T("elementIdFineDebug")
#define CONFIG_CONVERTER_DEBUG_SETTING_OUT_FINE_DEBUG         _T("pathFineDebug")
#define CONFIG_CONVERTER_DEBUG_SETTING_ATTHACH_THISID_ATTRIBUTE  _T("attachThisIdAttribute")

#define CONFIG_CONVERTER_SETTING_STITCH_ON                    _T("stitch_on")
#define CONFIG_CONVERTER_SETTING_STITCH_PRECISION             _T("stitch_precision")
#define CONFIG_CONVERTER_SETTING_JOIN_SIMILAR_FACES           _T("join_similar_faces")
#define CONFIG_CONVERTER_SETTING_CONFIGURATION_TO_READ        _T("configuration_to_read")
#define CONFIG_CONVERTER_SETTING_VISUAL_PROPERTIES_STRATEGY   _T("c3dVisualPropertiesReadStrategy")
#define CONFIG_CONVERTER_SETTING_IMPORT_MESHES_WITH_TOPOLOGY  _T("import_meshes_with_topology")
#define CONFIG_CONVERTER_SETTING_IMPORT_INSTANCE_ATTRIBUTES   _T("import_instance_attributes")

#define CONFIG_APP_DEBUG_DRAW                                 _T("app.debug.draw")
#define CONFIG_APP_ASSERT_MODE                                _T("app.debug.assert_mode")
#define CONFIG_APP_MULTITHREADING_MODE                        _T("app.multithreading_mode")
#define CONFIG_APP_PERMANENT_DIAGNOSTICS_HIGHLIGHT            _T("app.permanent_diagnostics_highlight")
#define CONFIG_APP_HIGHLIGHTING_SUB_OBJECT                    _T("app.highlighting_sub_object")
#define CONFIG_APP_FORCE_USE_MBCOLOR                          _T("app.forceUseMbColor")
#define CONFIG_APP_FORCE_USE_PLUGIN                           _T("app.forceUsePlugin")
#define CONFIG_APP_READ_TO_MODEL_DOCUMENT                     _T("app.readToModelDocument")
#define CONFIG_APP_FIT_ON_FIRST_LOADING                       _T("app.fitOnFirstLoading")
#define CONFIG_APP_IMPORT_FROM_BUFFER                         _T("app.importFromBuffer")
#define CONFIG_APP_EXPORT_INTO_BUFFER                         _T("app.exportIntoBuffer")
#define CONFIG_APP_IMPORT_OB_FILES                            _T("app.importObFileIntoSolid")
#define CONFIG_USE_BOUNDING_BOX_ERROR_DIALOG                  _T("app.useBoundingBoxErrorDialog")

// Фильтры чтения объектов
#define CONFIG_FILTER_SOLID                                   _T("filter.solid")
#define CONFIG_FILTER_SURFACE                                 _T("filter.surface")
#define CONFIG_FILTER_CURVE                                   _T("filter.curve")
#define CONFIG_FILTER_POINT                                   _T("filter.point")
#define CONFIG_FILTER_POLYGONAL                               _T("filter.polygonal")
#define CONFIG_FILTER_BREP                                    _T("filter.brep")
#define CONFIG_FILTER_VISUAL                                  _T("filter.visual")
#define CONFIG_FILTER_ATTRIBUTE                               _T("filter.attribute")
#define CONFIG_FILTER_PMI                                     _T("filter.pmi")
#define CONFIG_FILTER_DENSITY                                 _T("filter.density")
#define CONFIG_FILTER_VALIDATION                              _T("filter.validation")


// Значения булевых полей структуры C3DConverterDebugSettings, которые переопределяются при передаче в конвертер
// через вызов интерфейса IConvertorProperty3D::GetDebugSettings

// Values of boolean fields of the C3DConverterDebugSettings structure, which are defined for
// the IConvertorProperty3D::GetDebugSettings method call
#define CONFIG_CONVERTER_DEBUG_SETTING_TRUE            _T("true")
#define CONFIG_CONVERTER_DEBUG_SETTING_FALSE           _T("false")

// полный путь до плагина          full path to pligin
#define CONFIG_CONV_PLUGIN_FULL_PATH                 _T("converter.plugin.FULL_PATH")
// префикс настройки плагина       prefix for plugin's option
#define CONFIG_CONV_PLUGIN_SETTINGS_PREFIX           _T("converter.plugin.settings.")


class    MbVector;
class    MbVector3D;
class    MbCartPoint;
class    MbCartPoint3D;
class    MbMatrix3D; 
class    MbMatrix; 
class    MbMesh;
class    MbSpaceItem;
class    MbPlaneItem;
class    MbTopItem;
class    MbItem;
class    MbSolid;
class    MbMesh;
class              Manager;
class              IfDrawGI;

#ifndef __USE_QT__ 
  extern HWND       hwndFrame;
  extern HWND       hwndClient;
  extern HINSTANCE  hInst;
  extern int        nCmdShow;
  HWND ParentForDialog();
#else // __USE_QT__ 
  class QMainWindow;
  class QWidget;

  extern QMainWindow* qMainWnd;
  QWidget* ParentForDialog();
#endif // __USE_QT__ 

//----------------------------------------------------------------------------------------
// Путь к элементу модели и его матрица преобразования в глобальную систему координат
//---
typedef std::pair<MbPath,MbMatrix3D> PathMatrix;

//----------------------------------------------------------------------------------------
// Запомненный объект и, возможно, его часть.
//---
typedef std::pair< SPtr<MbItem>, SPtr<MbRefItem> > ItemPart;

//----------------------------------------------------------------------------------------
// Элемент модели и его путь. Element of the model and its path.
//---
typedef std::pair< MbPath, SPtr<MbRefItem> > ItemElement;

//----------------------------------------------------------------------------------------
// Тип указания объекта
//---
typedef enum 
{
    PNT_RES_NONE  = 0
  , PNT_RES_USUAL = 1 // Обычное указание точки (usual pointing)
  , PNT_RES_ESC   = 2 // Отмена указания
  , PNT_RES_SHIFT = 3 // Детализированное указание (detailed pointing with shift key)
  , PNT_RES_CTRL  = 4 // Указание с нажатием CTRL    
} PNT_result;

//----------------------------------------------------------------------------------------
// Тип фильтра для чтения объектов
//---
enum EObjectFilter
{
  of_type,
  of_gabarit,
  of_selection,
  of_attribute,
  of_embodiment
} ;

//----------------------------------------------------------------------------------------
// Глобальные параметры. Global parameters of testing app
// ---
namespace TestVariables {
  // Цвета для стилей
  extern COLORREF      colors[17];
  extern COLORREF      sColor;
  extern COLORREF      bColor;

  extern int           lineWidth;  // Толщина линий рисования. // Line width for draw.
  // Стили элементов
  extern Style
    SELECT_Style,        // LIGHTGREEN
    POINT_Style,         // LIGHTBLUE
    CONIC_Style,         // RED
    CURVE_Style,         // LIGHTMAGENTA
    SURFACECURVE_Style,  // LIGHTRED
    ELEMENTARY_Style,    // LIGHTCYAN
    SWEPT_Style,         // LIGHTCYAN
    SURFACE_Style,       // DARKGRAY
    SOLID_Style,         // LIGHTCYAN
    BOOLEAN_Style,       // LIGHTMAGENTA
    SHELL_Style,         // LIGHTCYAN
    WIRE_Style,          // BROWN
    TEXT_Style,          // DARKGRAY
    BACKGROUND_Style,    // LIGHTBLUE

    PPOINT_Style,        // LIGHTRED
    TANGENT_Style,       // LIGHTGRAY
    LINE_Style,          // GREEN
    HIDE_Style,          // DARKGRAY
    LINE_SEG_Style,      // LIGHTBLUE
    POLYLINE_Style,      // MAGENTA
    BSPLINE_Style,       // BLUE
    DIM_Style,           // WHITE
    OFFSET_Style,        // LIGHTCYAN
    CONTOUR_Style,       // LIGHTMAGENTA
    CIRCLE_Style,        // LIGHTCYAN
    ARC_Style,           // GREEN
    ELLIPSE_Style,       // LIGHTCYAN
    EQU_Style,           // BLACK
    GOLDEN_Style;        // GOLDEN

  // Для задачи в целом.
  extern MbFloatAxis3D cursorAxis;     // Ось, проходящая через курсор перпендикулярно плоскости активного окна (от нас)
  extern MbCartPoint3D cursorPoint;    // Положение курсора
  extern Manager     * viewManager;    // Менеджер окон отображения
  extern IfDrawGI    * drawImp;        // Отрисовщик объектов в активном окне
  extern bool          enableWCS;      // Отображать оси глобальной (мировой) системы координат (ГСК)

  extern PathMatrix    itemPathFrom;    // Путь к элементу модели и его матрица преобразования в глобальную систему координат
  extern SPtr<MbItem>  hLightedObj;     // It will be removed. Use 'hLightedItem' instead.
  extern ItemElement   hLightedItem;    // Объект подсветки в модели. The higlighted item of the model.                                                                                                                
  static void          ResetHLighted(); // Reset the highlighted item
  // Для отладочной отрисовки.
  extern int           drawFactor;      // Коэффициент отображения.

  // Для главного меню.
  extern int           waitFlag;       // Флаг ожидания ввода объекта

  extern ptrdiff_t     wPrev;          // Предыдущее значение параметра
  extern ptrdiff_t     lPrev;          // Предыдущее значение параметра
  extern ptrdiff_t     cmPrev;         // Предыдущее значение CM_....

  // Для рабочих окон.
  extern ptrdiff_t     _tindex;        // Номер временной точки в множестве
  extern ptrdiff_t     _rindex;        // Номер
  extern int           tflag;
  extern MbCartPoint3D rememPoint;     // Запомненное положение точки
  extern MbVector3D    moveVector;     // Вектор сдвига объекта

  // Для окон диалогов.
  extern TCHAR         strBuffer[STRINGLENGTH + 1];
  extern TCHAR         strBuffer2[STRINGLENGTH + 1];
  extern void *        exactPoint;     // Точка
  extern PNT_result    getPointResult; // Флаг ввода точки (flag of pointing)
  extern POINT         mappingPoint;   // Положение курсора

  extern POINT         rememPosit;
  extern bool          setProj;
  extern bool          isDown;
  extern POINT         posit;
  extern bool          saveLic;

  // для создания фасок
  extern int           numberOfChamfers;
  extern void        * chamferParams;

  // Точки прямоугольника.
  extern POINT         cursorPoint0;
  extern POINT         cursorPoint1;
  extern int           cursorFlag;
  extern int           plotFlag;
  extern int           rastrFlag;
  extern double        delta_range;    // Смещение      
  extern double        delta_angle;    // Поворот
  extern double        delta_point;    // Радиус округления при вводе точки
  extern bool          make_grid;      // Рассчитывать триангуляцию для визуализационной модели

  // Параметры операций.
  extern double        depth1;
  extern double        angle1;
  extern double        thickness1;
  extern double        depth2;
  extern double        angle2;
  extern double        thickness2;
  extern double        revolve1;
  extern double        revolve2;
  extern double        thickness;
  extern double        distance;       // Расстояние при резке триангуляции двумя плоскостями.
  extern double        distance1;
  extern double        distance2;
  extern double        offset0;
  extern double        offset1;
  extern double        offset2;
  extern double        offset3;
  extern double        radius1;
  extern double        radius2;
  extern double        radius3;
  extern double        conic;
  extern double        visualSag;
  extern double        deviateAngle;
  extern double        maxLength;
  extern double        curvatureScale; // Масштаб отображения кривизны.
  extern double        precision;
  extern double        accuracy;
  extern double        tolerance;
  extern double        fidelity;
  extern int           prolong;
  extern int           keepKerb;
  extern int           cornerForm;
  extern int           faceSide;
  extern int           sideShape;
  extern int           handling;
  extern int           split;
  extern int           equable;
  extern int           part;
  extern int           ribPart;
  extern int           offsetType;
  extern int           operateType;
  extern int           form;
  extern int           type;
  extern int           way ;
  extern int           kind;
  extern int           sot;
  extern int           count;
  extern int           count1;
  extern int           count2;
  extern int           degree;
  extern int           cType;
  extern int           kMode;
  extern bool          closedItem; // Создавать замкнутый объект? Create closed object?
  extern bool          mergeFaces; // Объединять ли грани в операциях? Merge faces in operations?
  extern bool          mergeEdges; // Объединять ли ребра в операциях? Merge edges in operations?
  extern bool          exactMesh;  // Создавать полигональные объекты на числах double(true)/float(false). Create polygonal object on double(true)/float(false) data.
  extern bool          replaceItems; // Replace items?
  extern bool          copyAttributesOnDemand; // Copy attributes on demand
  extern int           smartCursor; // Использовать привязки к объектам и координатной сетке? Use finding for nearest objects and coordinate cells?
  extern bool          permanentDiagnosticsShow; // Перекрашивать ли элементы тела по результатам диагностики. Whether to assign colour to solid's elements according to diagnostics result

  // Objects properties: use same creators
  extern bool          sameCreators;

  // Filename buffer.
  extern TCHAR         nameBuffer[STRINGLENGTH + 1];
  // Output of results ( rebuild time ) to a file
  extern bool          rebuildTimeToFile;
  // Check read time
  extern bool          checkReadTime;
  // Check write time
  extern bool          checkWriteTime;
  // Check operation time
  extern bool          checkOperationTime;

  // switch for writing model tree to file
  extern bool         useModelTree;

 // switch to highlight menu sub-objects
  extern bool         useHighlightObj;

  // Use a dialog box to checking for base bounding box error
  extern bool         useBoundingBoxErrorDialog;

  extern std::map<c3d::string_t, c3d::string_t> userProp; // пользовательские настройки приложения

  // Принудительно использовать плагин. Force use of plugin.
  extern bool         forceUsePlugin;

  // Выбор сущности, в которую читаются модели - модельный документ ItModelDocument или MbItem/MbModel.
  // Selection the entity where to read models - model document ItModelDocument or MbItem/MbModel.
  extern bool         readToModelDocument;

  // Выбор режима импорта файла - импорт через буфер.
  // Selection the import mode - import from buffer.
  extern bool         importFromBuffer;

  // Выбор режима экспорта файла - экспорт через буфер.
  // Selection the export mode - export from buffer.
  extern bool         exportIntoBuffer;

  // Индекс конфигурации для импорта. Значение -2 соотвествует активной конфигурации.
  // Index of configuration to import. The -2 value corresponds the index of the active one.
  extern int          configurationToRead;

  // Использовать адаптивное значение VisualSag при загрузке модели
  // Use adaptive Visual Sag value on model loading
  extern bool         fitOnFirstLoading;

  // Открывать файлы с форматом .ob и импортировать их в MbSolid
  // Open files with the .ob format and import them into MbSolid
  extern bool         importObFileIntoSolid;
};

//-----------------------------------------------------------------------------
// Reset the highlighted item
//---
inline static void TestVariables::ResetHLighted()
{
  hLightedObj = nullptr;
  hLightedItem.first.clear();
  hLightedItem.second = nullptr;
}


#endif // __TEST_VARIABLES_H
