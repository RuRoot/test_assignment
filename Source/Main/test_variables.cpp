///////////////////////////////////////////////////////
//
//      Внешние переменные задачи
//
///////////////////////////////////////////////////////

#include <test_variables.h>
#include <test.h>
#include <templ_sptr.h>
#include <last.h>

using namespace c3d;


#ifndef __USE_QT__ 
  HWND      hwndFrame = 0;
  HWND      hwndClient = 0;
  HINSTANCE hInst = 0;  // current instance
  int       nCmdShow = 0;
#else // __USE_QT__ 
  QMainWindow * qMainWnd = nullptr;
#endif // __USE_QT__ 


// Цвета для стилей
COLORREF TestVariables::colors[] = {
  RGB(  0,   0,   0),
  RGB(  0,   0, 192),
  RGB(  0, 192,   0),
  RGB(  0, 128, 128),
  RGB(192,   0,   0),
  RGB( 96,   0, 192),
  RGB(192, 128,   0),
  RGB(192, 192, 192),
  RGB(128, 158, 158),
  RGB(  0,   0, 255),
  RGB(  0, 255,   0),
  RGB(  0, 125, 255),
  RGB(255,   0,   0),
  RGB(255,   0, 125),
  RGB(128, 128,   0),
  RGB(255, 255, 255),
  RGB(222, 222, 125),
};

  COLORREF      TestVariables::sColor = RGB(  0, 255, 0  );
  COLORREF      TestVariables::bColor = RGB(255, 255, 255);

  int           TestVariables::lineWidth = 1;     // Толщина линий рисования. // Line width for draw.

Style   // Стили элементов
  TestVariables::SELECT_Style      ( TestVariables::lineWidth, LIGHTGREEN ),
  TestVariables::POINT_Style       ( TestVariables::lineWidth, LIGHTBLUE ),
  TestVariables::CONIC_Style       ( TestVariables::lineWidth, RED ),
  TestVariables::CURVE_Style       ( TestVariables::lineWidth, LIGHTMAGENTA ),
  TestVariables::SURFACECURVE_Style( TestVariables::lineWidth, LIGHTRED ),
  TestVariables::ELEMENTARY_Style  ( TestVariables::lineWidth, LIGHTCYAN ),
  TestVariables::SWEPT_Style       ( TestVariables::lineWidth, LIGHTCYAN ),
  TestVariables::SURFACE_Style     ( TestVariables::lineWidth, DARKGRAY ),
  TestVariables::SOLID_Style       ( TestVariables::lineWidth, LIGHTCYAN ),
  TestVariables::BOOLEAN_Style     ( TestVariables::lineWidth, LIGHTMAGENTA ),
  TestVariables::SHELL_Style       ( TestVariables::lineWidth, LIGHTCYAN ),
  TestVariables::WIRE_Style        ( TestVariables::lineWidth, BROWN ),
  TestVariables::TEXT_Style        ( TestVariables::lineWidth, DARKGRAY ),
  TestVariables::BACKGROUND_Style  ( TestVariables::lineWidth, LIGHTBLUE ),

  TestVariables::PPOINT_Style      ( TestVariables::lineWidth, LIGHTRED ),
  TestVariables::HIDE_Style        ( TestVariables::lineWidth, LIGHTGRAY ),
  TestVariables::TANGENT_Style     ( TestVariables::lineWidth, GREEN ),
  TestVariables::LINE_Style        ( TestVariables::lineWidth, DARKGRAY ),
  TestVariables::LINE_SEG_Style    ( TestVariables::lineWidth, LIGHTBLUE ),
  TestVariables::POLYLINE_Style    ( TestVariables::lineWidth, MAGENTA ),
  TestVariables::BSPLINE_Style     ( TestVariables::lineWidth, BLUE ),
  TestVariables::DIM_Style         ( TestVariables::lineWidth, WHITE ),
  TestVariables::OFFSET_Style      ( TestVariables::lineWidth, LIGHTCYAN ),
  TestVariables::CONTOUR_Style     ( TestVariables::lineWidth, LIGHTMAGENTA ),
  TestVariables::CIRCLE_Style      ( TestVariables::lineWidth, LIGHTCYAN ),
  TestVariables::ARC_Style         ( TestVariables::lineWidth, GREEN ),
  TestVariables::ELLIPSE_Style     ( TestVariables::lineWidth, LIGHTCYAN ),
  TestVariables::EQU_Style         ( TestVariables::lineWidth, BLACK ),
  TestVariables::GOLDEN_Style      ( TestVariables::lineWidth, GOLDEN );

  // Для задачи в целом
  MbFloatAxis3D TestVariables::cursorAxis;         // Ось, проходящая через курсор перпендикулярно плоскости активного окна (от нас)
  MbCartPoint3D TestVariables::cursorPoint;        // Положение курсора
  Manager *     TestVariables::viewManager = nullptr; // Менеджер окон отображения
  IfDrawGI *    TestVariables::drawImp = nullptr;     // Отрисовщик объектов в активном окне
  bool          TestVariables::enableWCS = true;   // Отображать оси глобальной (мировой) системы координат (ГСК)

  PathMatrix    TestVariables::itemPathFrom( std::make_pair( MbPath(), MbMatrix3D::identity ) );    // Путь к элементу модели и его матрица преобразования в глобальную систему координат
  SPtr<MbItem>  TestVariables::hLightedObj( nullptr ); // Запомненный объект и, возможно, его часть
  ItemElement   TestVariables::hLightedItem( std::make_pair( MbPath(), SPtr<MbRefItem>()) );   // Подсвеченный элемент модели. Highligted element of the model.

  // Для отладочной отрисовки.
  int           TestVariables::drawFactor = 10;    // Коэффициент отображения.

  // Для главного меню
  int           TestVariables::waitFlag = -1;      // Флаг ожидания ввода объекта

  ptrdiff_t     TestVariables::wPrev  = CM_REBUILD_SHELL;  // Предыдущее значение параметра
  ptrdiff_t     TestVariables::lPrev  = 0;                 // Предыдущее значение параметра
  ptrdiff_t     TestVariables::cmPrev = CM_REBUILD_SHELL;  // Предыдущее значение CM_....

  // Для рабочих окон
  ptrdiff_t     TestVariables::_tindex;             // Номер временной точки в множестве
  ptrdiff_t     TestVariables::_rindex;             // Номер
  int           TestVariables::tflag;
  MbCartPoint3D TestVariables::rememPoint;         // Запомненное положение точки
  MbVector3D    TestVariables::moveVector;         // Вектор сдвига объекта

  // Для окон диалогов
  TCHAR         TestVariables::strBuffer[STRINGLENGTH + 1];
  TCHAR         TestVariables::strBuffer2[STRINGLENGTH + 1]; // Второй буфер, если используются сразу два.
  void        * TestVariables::exactPoint;         // Точка
  PNT_result    TestVariables::getPointResult;     // Флаг ввода точки
  POINT         TestVariables::mappingPoint;       // Положение курсора
  POINT         TestVariables::rememPosit;
  bool          TestVariables::setProj;
  bool          TestVariables::isDown;
  POINT         TestVariables::posit;
  bool          TestVariables::saveLic;            // Сохранять ли лицензию в файле

  // для создания фасок
  int           TestVariables::numberOfChamfers;
  void        * TestVariables::chamferParams;

  // Точки прямоугольника захвата
  POINT         TestVariables::cursorPoint0;
  POINT         TestVariables::cursorPoint1;
  int           TestVariables::cursorFlag = -1;
  int           TestVariables::plotFlag = -1;
  int           TestVariables::rastrFlag = -1;
  double        TestVariables::delta_range = 8.0;       // Смещение
  double        TestVariables::delta_angle = M_PI/36.0; // Поворот
  double        TestVariables::delta_point = 10;        // Радиус округления при вводе точки
  bool          TestVariables::make_grid = false;       // Рассчитывать триангуляцию для визуализационной модели

  // Параметры операций
  double        TestVariables::depth1 = 50;
  double        TestVariables::angle1 = 0;
  double        TestVariables::thickness1 = 0;
  double        TestVariables::depth2 = 50;
  double        TestVariables::angle2 = 0;
  double        TestVariables::thickness2 = 0;
  double        TestVariables::revolve1 = 180;
  double        TestVariables::revolve2 = 180;
  double        TestVariables::thickness = -1;
  double        TestVariables::distance  = 50.0;
  double        TestVariables::distance1 = 1.0;
  double        TestVariables::distance2 = 1.0;
  double        TestVariables::offset0 = 2.0;
  double        TestVariables::offset1 = 3.0;
  double        TestVariables::offset2 = 4.0;
  double        TestVariables::offset3 = 5.0;
  double        TestVariables::radius1 = 5.0;
  double        TestVariables::radius2 = 5.0;
  double        TestVariables::radius3 = 5.0;
  double        TestVariables::conic = 0.0;
  double        TestVariables::visualSag = ONE_FIFTH;        // Math::visualSag; // MA: вылетало при инициализации под Линукс.
  double        TestVariables::deviateAngle = DEVIATION_SAG; // Math::deviateSag;
  double        TestVariables::maxLength = MAXIMON;
  double        TestVariables::curvatureScale = 50; // Масштаб отображения кривизны.
  double        TestVariables::precision = METRIC_PRECISION;
  double        TestVariables::accuracy = DELTA_MIN;
  double        TestVariables::tolerance = DELTA_MID;
  double        TestVariables::fidelity = DELTA_MOD;
  int           TestVariables::prolong = 1;
  int           TestVariables::keepKerb = 0;
  int           TestVariables::cornerForm = 0;
  int           TestVariables::faceSide = 1;
  int           TestVariables::sideShape = 0;
  int           TestVariables::handling = 2;
  int           TestVariables::split = 0;
  int           TestVariables::equable = 0;
  int           TestVariables::part = 1;
  int           TestVariables::ribPart = 1;
  int           TestVariables::offsetType = 0;
  int           TestVariables::operateType = 4;
  int           TestVariables::form = 2;
  int           TestVariables::type = 0;
  int           TestVariables::way  = 0;
  int           TestVariables::kind = 0;
  int           TestVariables::sot = 0;
  int           TestVariables::count = 50;
  int           TestVariables::count1 = 7;
  int           TestVariables::count2 = 5;
  int           TestVariables::degree = 6;
  int           TestVariables::cType = 4;
  int           TestVariables::kMode = 1;
  bool          TestVariables::closedItem = true; // Создавать замкнутый объект? Create closed object?
  bool          TestVariables::mergeFaces = true; // Объединять ли грани в операциях? Merge faces in operations?
  bool          TestVariables::mergeEdges = true; // Объединять ли грани в операциях? Merge faces in operations?
  bool          TestVariables::exactMesh = false; // Создавать полигональные объекты на числах double(true)/float(false). Create polygonal object on double(true)/float(false) data.
  bool          TestVariables::replaceItems = true; // Replace items?
  bool          TestVariables::copyAttributesOnDemand = false; // Copy attributes on demand
  int           TestVariables::smartCursor = 0; // Использовать привязки к объектам и координатной сетке? Use finding for nearest objects and coordinate cells?
  bool          TestVariables::permanentDiagnosticsShow = false; // Перекрашивать ли элементы тела по результатам диагностики. Whether to assign colour to solid's elements according to diagnostics result
  bool          TestVariables::sameCreators = false;
  TCHAR         TestVariables::nameBuffer[STRINGLENGTH + 1];
  bool          TestVariables::rebuildTimeToFile = false;
  bool          TestVariables::checkReadTime = false;
  bool          TestVariables::checkWriteTime = false;
  bool          TestVariables::checkOperationTime = false;
  bool          TestVariables::useModelTree = false;
  bool          TestVariables::useHighlightObj = true;
  bool          TestVariables::useBoundingBoxErrorDialog = true;
  bool          TestVariables::forceUsePlugin = false;   // Принудительно использовать плагин. Force use of plugin.
  bool          TestVariables::readToModelDocument = false;
  bool          TestVariables::fitOnFirstLoading = true;
  bool          TestVariables::importFromBuffer = false;
  bool          TestVariables::exportIntoBuffer = false;
  bool          TestVariables::importObFileIntoSolid = false;

  // Индекс конфигурации для импорта. Значение -2 соотвествует активной конфигурации.
  // Index of configuration to import. The -2 value corresponds the index of the active one.
  int           TestVariables::configurationToRead = -2;
  std::map<c3d::string_t, c3d::string_t> TestVariables::userProp;
