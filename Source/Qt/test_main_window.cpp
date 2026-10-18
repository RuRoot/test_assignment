#ifdef __USE_QT__

#pragma warning(disable: 4127)
#pragma warning(disable: 4512)
#pragma warning(disable: 4100)

#include <test_main_window.h>
#include <test_child_window.h>
#include <system_types.h>
#include <test.h>
#include <test_frame.h>
#include <test_manager.h>
#include <test_gr_draw.h>
#include <test_service.h>
#include <test_variables.h>

#include <qevent.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>
#include <QToolBar>
#include <QMdiArea>
#include <QEventLoop>
#include <QSignalMapper>
#include <QStatusBar>
#include <QMdiSubWindow>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>

#include <math_namespace.h>
#include <last.h>

using namespace c3d;
using namespace TestVariables;


//------------------------------------------------------------------------------
//
// ---
QString QString_fromT(const TCHAR * text) {
  c3d::string_t strText = text != nullptr ? c3d::string_t( text ) : c3d::string_t();
#ifndef _UNICODE
#ifdef __NATIVE_LANGUAGE__
  return QString::fromLocal8Bit( strText.c_str() );
#else
  return QString::fromStdString( strText );
#endif // __NATIVE_LANGUAGE__
#else // _UNICODE
  return QString::fromStdWString( strText );
#endif // _UNICODE
}


//------------------------------------------------------------------------------
//
// ---
MainWindow::MainWindow()
:   mdiArea      ( nullptr )
,   windowMapper ( nullptr )
,   mode         ( eNormal )
,   m_actRunUserCommand0( nullptr )
,   m_actRunUserCommand1( nullptr )
,   m_actRunUserCommand2( nullptr )
,   m_actRunUserCommand3( nullptr )
,   m_actRunUserCommand4( nullptr )
,   m_actRunUserCommand5( nullptr )
,   m_actRunUserCommand6( nullptr )
,   m_actRunUserCommand7( nullptr )
,   m_actRunUserCommand8( nullptr )
,   m_actRunUserCommand9( nullptr )
,   m_actFitScale       ( nullptr )
,   m_actSetIsometry    ( nullptr )
,   m_actSetFront       ( nullptr )
,   m_actSetBack        ( nullptr )
,   m_actSetTop         ( nullptr )
,   m_actSetBottom      ( nullptr )
,   m_actSetLeft        ( nullptr )
,   m_actSetRight       ( nullptr )
{
  mdiArea = new QMdiArea;
  mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  setCentralWidget(mdiArea);
  connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(slot_mdiActivated()));

  windowMapper = new QSignalMapper(this);
  connect(windowMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveSubWindow(QWidget*)));

  createActions();
  createMenus();
  createStandardToolBar();
  createStatusBar();
  //updateMenus();

  setWindowTitle(QString_fromT( TITLE_MAIN_WND ));
  setUnifiedTitleAndToolBarOnMac(true);

  SystemInitFrame(); // Провести неоконные действия при инициализации главного окна задачи
  readSettings();
}

//------------------------------------------------------------------------------
//
// ---
MainWindow::~MainWindow()
{
}

//------------------------------------------------------------------------------
//
// ---
void MainWindow::closeEvent(QCloseEvent *event)
{
    mdiArea->closeAllSubWindows();

    if (mdiArea->currentSubWindow())
        event->ignore();
    else
    {
        //writeSettings();
        event->accept();
    }
    writeSettings();
    QMainWindow::closeEvent(event);
}

//------------------------------------------------------------------------------
//
// ---
void MainWindow::tileSubWindows()
{
    mdiArea->tileSubWindows();
}

//------------------------------------------------------------------------------
//
// ---
void MainWindow::cascadeSubWindows()
{
    mdiArea->cascadeSubWindows();
}

//------------------------------------------------------------------------------
//
// ---
QWidget* MainWindow::createMdiChild()
{
  GraphicsView* child = new GraphicsView;
  QMdiSubWindow* subWindow = mdiArea->addSubWindow( child );
  subWindow->setWindowIcon(QIcon(":/res/geochild.png"));
  return child;
}

//------------------------------------------------------------------------------
//
// ---
void MainWindow::createNewDocument()
{
  ::MenuCommand((uint)CM_VIEWISO);
}

//------------------------------------------------------------------------------
//
// ---
void MainWindow::openDocument()
{
  ::MenuCommand((uint)CM_FILEOPEN);
}

//------------------------------------------------------------------------------
//
// ---
void MainWindow::saveActivDocument()
{
  ::MenuCommand((uint)CM_FILESAVE);
}

//------------------------------------------------------------------------------
//
// ---
void MainWindow::slot_mdiActivated()
{
  ptrdiff_t index = viewManager->FindWindowIndex( (void*)activeMdiChild(), false/*setActive*/ );
  viewManager->SetActiveWindow( index );

  QWidget* wnd = activeMdiChild();
  if ( wnd )
    wnd->setFocus();
}


//------------------------------------------------------------------------------
//
// ---
void MainWindow::createStatusBar()
{
  statusBar()->showMessage( QString() );
}


//------------------------------------------------------------------------------
//
// ---
QWidget *MainWindow::activeMdiChild()
{
  if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
    return qobject_cast<QWidget*>(activeSubWindow->widget());
  return 0;
}


//------------------------------------------------------------------------------
//
// ---
void MainWindow::setActiveSubWindow( QWidget *window )
{
  if ( !window )
    return;

  mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(window));
}


//------------------------------------------------------------------------------
//
// ---
void MainWindow::setActiveMdiChild( QWidget *window )
{
  if ( !window )
    return;

  QList<QMdiSubWindow*> wndList = mdiArea->subWindowList();
  for ( int i = 0, wlNb = wndList.count(); i < wlNb; ++i )
  {
    QWidget* _wnd = qobject_cast<QWidget*>(wndList[i]->widget());
    if ( _wnd == window )
      setActiveSubWindow( qobject_cast<QWidget*>(wndList[i]) );
  }
}


//------------------------------------------------------------------------------
//
// ---
void MainWindow::changeActiveMdiChildState()
{
  QMdiSubWindow* wnd = mdiArea->activeSubWindow();
  if ( wnd )
  {
    if ( wnd->isMaximized() )
      wnd->showNormal();
    else
      wnd->showMaximized();
  }
}

//------------------------------------------------------------------------------
//
// ---
void MainWindow::setMode( inputModeType set )
{
  if ( mode != set )
  {
    switch ( set ) {
      case eFindPoint: setCursor( Qt::CrossCursor ); break;
      default:         setCursor( Qt::ArrowCursor ); break;
    } // switch
    mode = set;
  }
}

QEventLoop * _eventLoop = nullptr;

//------------------------------------------------------------------------------
// Выполнить процесс обработки сообщений
// ---
void MainWindow::ExecuteLoop( inputModeType set )
{
  if ( mode == eNormal ) 
  {
    ::getPointResult = PNT_RES_NONE;
    setMode( set );

    _eventLoop = new QEventLoop;
    _eventLoop->exec();
    delete _eventLoop;

    setMode( eNormal );
  }
}


//------------------------------------------------------------------------------
// Завершить процесс обработки сообщений
// ---
void MainWindow::BreakLoop()
{
  if ( _eventLoop )
    _eventLoop->quit();
}

//------------------------------------------------------------------------------
//
// ---
void MainWindow::createActions()
{
  m_actNewDocument = new QAction(QIcon(":/res/new.png"), tr("&New"), this);
  m_actNewDocument->setStatusTip(tr("Create a new document"));
  m_actNewDocument->setPriority(QAction::LowPriority);
  m_actNewDocument->setShortcut(QKeySequence::New);
  connect(m_actNewDocument, SIGNAL(triggered()), this, SLOT(createNewDocument()));

  m_actOpenDocument = new QAction(QIcon(":/res/open.png"), tr("&Open..."), this);
  m_actOpenDocument->setStatusTip(tr("Open an existing document"));
  m_actOpenDocument->setShortcut(QKeySequence::Open);
  connect(m_actOpenDocument, SIGNAL(triggered()), this, SLOT(openDocument()));

  m_actSaveDocument = new QAction(QIcon(":/res/save.png"), tr("&Save"), this);
  m_actSaveDocument->setStatusTip(tr("Save the active document"));
  m_actSaveDocument->setShortcut(QKeySequence::Save);
  connect(m_actSaveDocument, SIGNAL(triggered()), this, SLOT(saveActivDocument()));
  
  m_actRunUserCommand0 = new QAction(QIcon(":/res/swiss-knife-pink.png"), tr("UserCommand&0"), this);
  m_actRunUserCommand0->setStatusTip(tr("Run user command 0"));
  connect(m_actRunUserCommand0, SIGNAL(triggered()), this, SLOT(slot_CM_MAKEUSERCOMAND0()));

  m_actRunUserCommand1 = new QAction( QIcon( ":/res/swiss-knife-red.png" ), tr( "UserCommand&1" ), this );
  m_actRunUserCommand1->setStatusTip( tr( "Run user command 1" ) );
  connect( m_actRunUserCommand1, SIGNAL( triggered() ), this, SLOT( slot_CM_MAKEUSERCOMAND1() ) );

  m_actRunUserCommand2 = new QAction( QIcon( ":/res/swiss-knife-orange.png" ), tr( "UserCommand&2" ), this );
  m_actRunUserCommand2->setStatusTip( tr( "Run user command 2" ) );
  connect( m_actRunUserCommand2, SIGNAL( triggered() ), this, SLOT( slot_CM_MAKEUSERCOMAND2() ) );

  m_actRunUserCommand3 = new QAction( QIcon( ":/res/swiss-knife-yellow.png" ), tr( "UserCommand&3" ), this );
  m_actRunUserCommand3->setStatusTip( tr( "Run user command 3" ) );
  connect( m_actRunUserCommand3, SIGNAL( triggered() ), this, SLOT( slot_CM_MAKEUSERCOMAND3() ) );

  m_actRunUserCommand4 = new QAction( QIcon( ":/res/swiss-knife-green.png" ), tr( "UserCommand&4" ), this );
  m_actRunUserCommand4->setStatusTip( tr( "Run user command 4" ) );
  connect( m_actRunUserCommand4, SIGNAL( triggered() ), this, SLOT( slot_CM_MAKEUSERCOMAND4() ) );

  m_actRunUserCommand5 = new QAction( QIcon( ":/res/swiss-knife-sky.png" ), tr( "UserCommand&5" ), this );
  m_actRunUserCommand5->setStatusTip( tr( "Run user command 5" ) );
  connect( m_actRunUserCommand5, SIGNAL( triggered() ), this, SLOT( slot_CM_MAKEUSERCOMAND5() ) );

  m_actRunUserCommand6 = new QAction( QIcon( ":/res/swiss-knife-blue.png" ), tr( "UserCommand&6" ), this );
  m_actRunUserCommand6->setStatusTip( tr( "Run user command 6" ) );
  connect( m_actRunUserCommand6, SIGNAL( triggered() ), this, SLOT( slot_CM_MAKEUSERCOMAND6() ) );

  m_actRunUserCommand7 = new QAction( QIcon( ":/res/swiss-knife-violet.png" ), tr( "UserCommand&7" ), this );
  m_actRunUserCommand7->setStatusTip( tr( "Run user command 7" ) );
  connect( m_actRunUserCommand7, SIGNAL( triggered() ), this, SLOT( slot_CM_MAKEUSERCOMAND7() ) );

  m_actRunUserCommand8 = new QAction( QIcon( ":/res/swiss-knife-gray.png" ), tr( "UserCommand&8" ), this );
  m_actRunUserCommand8->setStatusTip( tr( "Run user command 8" ) );
  connect( m_actRunUserCommand8, SIGNAL( triggered() ), this, SLOT( slot_CM_MAKEUSERCOMAND8() ) );

  m_actRunUserCommand9 = new QAction( QIcon( ":/res/swiss-knife-black.png" ), tr( "UserCommand&9" ), this );
  m_actRunUserCommand9->setStatusTip( tr( "Run user command 9" ) );
  connect( m_actRunUserCommand9, SIGNAL( triggered() ), this, SLOT( slot_CM_MAKEUSERCOMAND9() ) );

  m_actFitScale = new QAction( QIcon( ":/res/view_all_32.png" ), tr( "Fit Scale (Shift - F5)" ), this );
  m_actFitScale->setStatusTip( tr( "Fit Scale (Shift - F5)" ) );
  connect( m_actFitScale, SIGNAL( triggered() ), this, SLOT( slot_CM_FITMODELTOWINDOW() ) );

  m_actSetIsometry = new QAction( QIcon( ":/res/isometry_x.png" ), tr( "Isometry view" ), this );
  m_actSetIsometry->setStatusTip( tr( "Isometry view" ) );
  connect( m_actSetIsometry, SIGNAL( triggered() ), this, SLOT( slot_CM_VIEWISOPROJ() ) );

  m_actSetFront = new QAction( QIcon( ":/res/view_front_32.png" ), tr( "Front view" ), this );
  m_actSetFront->setStatusTip( tr( "Front view" ) );
  connect( m_actSetFront, SIGNAL( triggered() ), this, SLOT( slot_CM_VIEWFRONTPROJ() ) );

  m_actSetBack = new QAction( QIcon( ":/res/view_back_32.png" ), tr( "Back view" ), this );
  m_actSetBack->setStatusTip( tr( "Back view" ) );
  connect( m_actSetBack, SIGNAL( triggered() ), this, SLOT( slot_CM_VIEWREARPROJ() ) );

  m_actSetTop = new QAction( QIcon( ":/res/view_top_32.png" ), tr( "Top view" ), this );
  m_actSetTop->setStatusTip( tr( "Top view" ) );
  connect( m_actSetTop, SIGNAL( triggered() ), this, SLOT( slot_CM_VIEWUPPROJ() ) );

  m_actSetBottom = new QAction( QIcon( ":/res/view_bottom_32.png" ), tr( "Bottom view" ), this );
  m_actSetBottom->setStatusTip( tr( "Bottom view" ) );
  connect( m_actSetBottom, SIGNAL( triggered() ), this, SLOT( slot_CM_VIEWDOWNPROJ() ) );

  m_actSetLeft = new QAction( QIcon( ":/res/view_left_32.png" ), tr( "Left view" ), this );
  m_actSetLeft->setStatusTip( tr( "Left view" ) );
  connect( m_actSetLeft, SIGNAL( triggered() ), this, SLOT( slot_CM_VIEWLEFTPROJ() ) );

  m_actSetRight = new QAction( QIcon( ":/res/view_right_32.png" ), tr( "Right view" ), this );
  m_actSetRight->setStatusTip( tr( "Right view" ) );
  connect( m_actSetRight, SIGNAL( triggered() ), this, SLOT( slot_CM_VIEWRIGHTPROJ() ) );
}

//------------------------------------------------------------------------------
// Чтобы не менять синтаксис описания меню (как в ресурсе Windows)? вводим эти define
// ---
#define MENU                                                                  \
    {                                                                         \
      QMenu* _menu_ = nullptr;

#define ENDMENU                                                               \
    }

#define POPUP( _TEXT_ )                                                       \
    {                                                                         \
      QMenu * _owner_ = _menu_;                                               \
      if ( _owner_ )                                                          \
        _menu_ = _owner_->addMenu(QString::fromLocal8Bit( _TEXT_ ));          \
      else                                                                    \
        _menu_ = menuBar()->addMenu(QString::fromLocal8Bit( _TEXT_ ));

#define ENDPOPUP                                                              \
      _menu_ = _owner_;                                                       \
    }

#define MENUITEM( _TEXT_, _CM_ )                                              \
    {                                                                         \
      QAction *act_ = _menu_ ->addAction(QString::fromLocal8Bit( _TEXT_ ));   \
      connect( act_, SIGNAL(triggered()), this, SLOT( slot_ ## _CM_ () ));    \
    }

#define SEPARATOR                                                             \
    _menu_->addSeparator();

#define MENUITEM_QT( _TEXT_, _OWN_, _SLOT_ )                                  \
    {                                                                         \
      QAction *act_ = _menu_ ->addAction(QString::fromLocal8Bit( _TEXT_ ));   \
      connect( act_, SIGNAL(triggered()), _OWN_ , SLOT( _SLOT_ () ));         \
    }

//------------------------------------------------------------------------------
// Popup macro
//---
#if defined ( __NATIVE_LANGUAGE__ )
#pragma code_page(1251)
#define POPUP_IMP( TEXT_RU, TEXT_EN ) POPUP( TEXT_RU )
#else   
#define POPUP_IMP( TEXT_RU, TEXT_EN ) POPUP( TEXT_EN )
#endif

//------------------------------------------------------------------------------
// Menuitem macro
//---
#if defined ( __NATIVE_LANGUAGE__ )
#//define MENUITEM_IMP( _CM_, TEXT_RU, TEXT_EN ) MENUITEM( TEXT_RU, _CM_ )
#define MENUITEM_IMP( _CM_, TEXT_RU, TEXT_EN )                            \
{                                                                         \
  QAction *act_ = _menu_ ->addAction(QString::fromLocal8Bit( TEXT_RU ));  \
  connect( act_, SIGNAL(triggered()), this, SLOT( slot_ ## _CM_ () ));    \
}
#else   
//#define MENUITEM_IMP( _CM_, TEXT_RU, TEXT_EN ) MENUITEM( TEXT_EN, _CM_ )
#define MENUITEM_IMP( _CM_, TEXT_RU, TEXT_EN )                            \
{                                                                         \
  QAction *act_ = _menu_ ->addAction(QString::fromLocal8Bit( TEXT_EN ));  \
  connect( act_, SIGNAL(triggered()), this, SLOT( slot_ ## _CM_ () ));    \
}
#endif

//------------------------------------------------------------------------------
//
// ---
void MainWindow::createMenus()
{
  MENU
#if defined ( __NATIVE_LANGUAGE__ )
    POPUP( "&Файл" )
#else
    POPUP( "&File" )
#endif
      _menu_->addAction(m_actNewDocument);
/*
#if defined ( __NATIVE_LANGUAGE__ )
      MENUITEM( "Открыть",   CM_FILEOPEN )
#else
      MENUITEM( "Open",      CM_FILEOPEN )
#endif
*/
      _menu_->addAction(m_actOpenDocument);
/*
#if defined ( __NATIVE_LANGUAGE__ )
      MENUITEM( "Сохранить", CM_FILESAVE )
#else
      MENUITEM( "Save",      CM_FILESAVE )
#endif
*/
      _menu_->addAction(m_actSaveDocument);

#if defined ( __NATIVE_LANGUAGE__ )
      MENUITEM( "Добавить",  CM_FILEADD )
#else
      MENUITEM( "Add",       CM_FILEADD )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      MENUITEM( "Очистить",  CM_FILENEW )
#else
      MENUITEM( "Clear",     CM_FILENEW )
#endif
      SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
      MENUITEM_QT( "Завершить работу", qApp, closeAllWindows )
#else
      MENUITEM_QT( "Exit", qApp, closeAllWindows )
#endif
    ENDPOPUP

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined ( __NATIVE_LANGUAGE__ )
    POPUP( "&Создать" )
#else
    POPUP( "&Create" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Тело" )
#else
      POPUP( "Solid" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Элементарное" )
#else
        POPUP( "Elementary" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Блок",   CM_NEWBLOCKSOLID )
#else
          MENUITEM( "Block",  CM_NEWBLOCKSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Клин",   CM_NEWWEDGESOLID )
#else
          MENUITEM( "Wedge",  CM_NEWWEDGESOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Цилиндр",   CM_NEWCYLINDERSOLID )
#else
          MENUITEM( "Cylinder",  CM_NEWCYLINDERSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Конус",   CM_NEWCONESOLID )
#else
          MENUITEM( "Cone",    CM_NEWCONESOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Шар",     CM_NEWSPHERESOLID )
#else
          MENUITEM( "Sphere",  CM_NEWSPHERESOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Тор",     CM_NEWTORUSSOLID )
#else
          MENUITEM( "Torus",   CM_NEWTORUSSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Пластину",  CM_NEWPLATESOLID )
#else
          MENUITEM( "Plate",     CM_NEWPLATESOLID )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "По точкам" )
#else
        POPUP( "By points" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Призму",  CM_NEWPRISMSOLID )
#else
          MENUITEM( "Prism",   CM_NEWPRISMSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Пирамиду",  CM_NEWPYRAMIDSOLID )
#else
          MENUITEM( "Pyramid",   CM_NEWPYRAMIDSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Nurbs-блок",  CM_NEWNURBSBLOCKSOLID )
#else
          MENUITEM( "Nurbs-block", CM_NEWNURBSBLOCKSOLID )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На базе кривых" )
#else
        POPUP( "Based on curves" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Выдавливанием поверхностной кривой",    CM_NEWCURVEEXTRUSIONSOLID )
#else
          MENUITEM( "Extrusion of a curve lying on surface", CM_NEWCURVEEXTRUSIONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Выдавливанием трехмерной кривой",  CM_NEWCURVE3DEXTRUSIONSOLID )
#else
          MENUITEM( "Extrusion of a spatial curve",     CM_NEWCURVE3DEXTRUSIONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Вращением поверхностной кривой",           CM_NEWCURVEREVOLUTIONSOLID )
#else
          MENUITEM( "Revolution of a curve lying on surface",   CM_NEWCURVEREVOLUTIONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Вращением трехмерной кривой",      CM_NEWCURVE3DREVOLUTIONSOLID )
#else
          MENUITEM( "Revolution of a spatial curve",    CM_NEWCURVE3DREVOLUTIONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Движением кривой",       CM_NEWCURVEEVOLUTIONSOLID )
#else
          MENUITEM( "Sweeping",   CM_NEWCURVEEVOLUTIONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По сечениям",  CM_NEWCURVELOFTEDSOLID )
#else
          MENUITEM( "Lofting",      CM_NEWCURVELOFTEDSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По сечениям и направляющей",  CM_NEWCURVELOFTEDSOLID_CL )
#else
          MENUITEM( "Lofting with a guide curve",  CM_NEWCURVELOFTEDSOLID_CL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Линейчатое",  CM_NEWRULEDSOLID )
#else
          MENUITEM( "Ruled",       CM_NEWRULEDSOLID )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На базе поверхности" )
#else
        POPUP( "Based on a surface" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Приданием толщины",  CM_NEWSHEETSOLID )
#else
          MENUITEM( "By thickening",      CM_NEWSHEETSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По элементарной поверхности",  CM_NEWELEMENTARYSOLID )
#else
          MENUITEM( "By an elementary surface",     CM_NEWELEMENTARYSOLID )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На базе тела" )
#else
        POPUP( "Based on a solid" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Разрезанное кривой",  CM_SOLID_CONTOUR_CUT )
#else
          MENUITEM( "Cut by a curve",      CM_SOLID_CONTOUR_CUT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Разрезанное оболочкой", CM_SOLID_SHELL_CUT )
#else
          MENUITEM( "Cut by a shell", CM_SOLID_SHELL_CUT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Разрезанное поверхностью",  CM_SOLID_SURFACE_CUT )
#else
          MENUITEM( "Cut by a surface",          CM_SOLID_SURFACE_CUT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Симметричное",  CM_NEWSYMMETRYSOLID )
#else
          MENUITEM( "Symmetric",     CM_NEWSYMMETRYSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "С ребром жесткости",  CM_NEWRIBSOLID )
#else
          MENUITEM( "With a rib",          CM_NEWRIBSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Ребро жёсткости ",  CM_NEWRIBSHELL )
#else
          MENUITEM( "Rib ",              CM_NEWRIBSHELL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "С отверстием",  CM_NEWHOLESOLID )
#else
          MENUITEM( "With a hole",   CM_NEWHOLESOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "С карманом или бобышкой",    CM_NEWPOCKETSOLID )
#else
          MENUITEM( "With a pocket or a boss",    CM_NEWPOCKETSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "С пазом",      CM_NEWSLOTSOLID )
#else
          MENUITEM( "With a slot",  CM_NEWSLOTSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Набор тел",      CM_NEWUNIONSOLID )
#else
          MENUITEM( "Set of solids",  CM_NEWUNIONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Часть набора тел",    CM_CREATE_PARTSOLID )
#else
          MENUITEM( "Set of solids part",  CM_CREATE_PARTSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Разрезанное очерком",      CM_SILHOUETTECONTOURSOLID )
#else
          MENUITEM( "Cut by a silhouette",      CM_SILHOUETTECONTOURSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Выдавливанием тела",       CM_NEWSOLIDEXTRUSIONSOLID )
#else
          MENUITEM( "Extrusion of a solid",     CM_NEWSOLIDEXTRUSIONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Вращением тела",           CM_NEWSOLIDREVOLUTIONSOLID )
#else
          MENUITEM( "Revolution of a solid",    CM_NEWSOLIDREVOLUTIONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Движением тела",           CM_NEWSOLIDEVOLUTIONSOLID )
#else
          MENUITEM( "Evolution of a solid",     CM_NEWSOLIDEVOLUTIONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Размножение по сетке",     CM_DUPLICATION_BYGRID )
#else
          MENUITEM( "Duplication by grid",      CM_DUPLICATION_BYGRID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Размножение матрицами",    CM_DUPLICATION_BYMATRICES )
#else
          MENUITEM( "Duplication by matrices",  CM_DUPLICATION_BYMATRICES )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На базе оболочки" )
#else
        POPUP( "Based on a shell" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Приданием толщины",  CM_THINSOLIDBYSHELL )
#else
          MENUITEM( "Thickening",         CM_THINSOLIDBYSHELL )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Приклеиванием к телу" )
#else
        POPUP( "As union with a solid" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Тела",   CM_BOOLEANUNION )
#else
          MENUITEM( "Solid",  CM_BOOLEANUNION )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Выдавливания кривой",  CM_BOSSCURVEEXTRUSIONSOLID )
#else
          MENUITEM( "Curve extrusion",      CM_BOSSCURVEEXTRUSIONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Вращения кривой",   CM_BOSSCURVEREVOLUTIONSOLID )
#else
          MENUITEM( "Curve revolution",  CM_BOSSCURVEREVOLUTIONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Движения кривой",  CM_BOSSCURVEEVOLUTIONSOLID )
#else
          MENUITEM( "Curve evolution",  CM_BOSSCURVEEVOLUTIONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По сечениям",   CM_BOSSCURVELOFTEDSOLID )
#else
          MENUITEM( "Lofting",       CM_BOSSCURVELOFTEDSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По сечениям и направляющей",  CM_BOSSCURVELOFTEDSOLID_CL )
#else
          MENUITEM( "Lofting with a guide curve",  CM_BOSSCURVELOFTEDSOLID_CL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Набора тел",     CM_BOSSUNIONSOLID )
#else
          MENUITEM( "Set of solids",  CM_BOSSUNIONSOLID )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Вырезанием из тела" )
#else
        POPUP( "As substruction with a solid" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Тела",   CM_BOOLEANDIFFERENCE )
#else
          MENUITEM( "Solid",  CM_BOOLEANDIFFERENCE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Выдавливания кривой",  CM_CUTCURVEEXTRUSIONSOLID )
#else
          MENUITEM( "Curve extrusion",      CM_CUTCURVEEXTRUSIONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Вращения кривой",   CM_CUTCURVEREVOLUTIONSOLID )
#else
          MENUITEM( "Curve revolution",  CM_CUTCURVEREVOLUTIONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Движения кривой",   CM_CUTCURVEEVOLUTIONSOLID )
#else
          MENUITEM(  "Curve evolution",  CM_CUTCURVEEVOLUTIONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По сечениям",  CM_CUTCURVELOFTEDSOLID )
#else
          MENUITEM( "Lofting",      CM_CUTCURVELOFTEDSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По сечениям и направляющей", CM_CUTCURVELOFTEDSOLID_CL )
#else
          MENUITEM( "Lofting with a guide curve", CM_CUTCURVELOFTEDSOLID_CL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Набора тел",     CM_CUTUNIONSOLID )
#else
          MENUITEM( "Set of solids",  CM_CUTUNIONSOLID )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Пересечением с телом" )
#else
        POPUP( "As intersection with a solid" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Тела",   CM_BOOLEANINTERSECTION )
#else
          MENUITEM( "Solid",  CM_BOOLEANINTERSECTION )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Выдавливания кривой",  CM_INTERSECTCURVEEXTRUSIONSOLID )
#else
          MENUITEM( "Curve extrusion",      CM_INTERSECTCURVEEXTRUSIONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Вращения кривой",   CM_INTERSECTCURVEREVOLUTIONSOLID )
#else
          MENUITEM( "Curve revolution",  CM_INTERSECTCURVEREVOLUTIONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Движения кривой",  CM_INTERSECTCURVEEVOLUTIONSOLID )
#else
          MENUITEM( "Curve evolution",  CM_INTERSECTCURVEEVOLUTIONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По сечениям",  CM_INTERSECTCURVELOFTEDSOLID )
#else
          MENUITEM( "Lofting",      CM_INTERSECTCURVELOFTEDSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По сечениям и направляющей", CM_INTERSECTCURVELOFTEDSOLID_CL )
#else
          MENUITEM( "Lofting with a guide curve", CM_INTERSECTCURVELOFTEDSOLID_CL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Набора тел",     CM_INTERSECTUNIONSOLID )
#else
          MENUITEM( "Set of solids",  CM_INTERSECTUNIONSOLID )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Обработкой граней" )
#else
        POPUP( "By faces processing" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Тонкостенное одной толщины",            CM_NEWTHINSOLID_ONESIDE_MONOTHICK )
#else
          MENUITEM( "Thin-walled with a constant thickness", CM_NEWTHINSOLID_ONESIDE_MONOTHICK )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Тонкостенное разных толщин",          CM_NEWTHINSOLID_ONESIDE_POLYTHICK )
#else
          MENUITEM( "Thin-walled with multiple thickness", CM_NEWTHINSOLID_ONESIDE_POLYTHICK )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Приданием толщины",  CM_NEWTHINSOLID_ONESIDE_OFFSET )
#else
          MENUITEM( "Thickening",         CM_NEWTHINSOLID_ONESIDE_OFFSET )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Уклонением граней",  CM_SOLIDNPDRAFT )
#else
          MENUITEM( "Faces drafting",     CM_SOLIDNPDRAFT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Разбивкой грани",  CM_NEW_SPLIT_SOLID )
#else
          MENUITEM( "Face splitting",   CM_NEW_SPLIT_SOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Скруглением грани",  CM_FULLFILLETSOLID )
#else
          MENUITEM( "Face fillet",        CM_FULLFILLETSOLID )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Обработкой рёбер" )
#else
        POPUP( "By edges processing" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Скругление по радиусу",  CM_SOLIDFILLET )
#else
          MENUITEM( "Fillet with radius",     CM_SOLIDFILLET )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Скругление переменное",  CM_SOLIDCHANNEL )
#else
          MENUITEM( "Variable fillet",        CM_SOLIDCHANNEL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Скругление по хорде",  CM_SOLIDSPAN )
#else
          MENUITEM( "Fillet by a chord",    CM_SOLIDSPAN )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Скругление по опоре",  CM_SOLIDSMOOTH )
#else
          MENUITEM( "Fillet by a support",  CM_SOLIDSMOOTH )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Фаска катет-катет",          CM_SOLIDCHAMFER )
#else
          MENUITEM( "Cathetus-cathetus chamfer",  CM_SOLIDCHAMFER )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Фаска катет-угол",        CM_SOLIDSLANT1 )
#else
          MENUITEM( "Cathetus-angle chamfer",  CM_SOLIDSLANT1 )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Фаска угол-катет",        CM_SOLIDSLANT2 )
#else
          MENUITEM( "Angle-cathetus chamfer",  CM_SOLIDSLANT2 )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Обработкой вершин" )
#else
        POPUP( "By vertices processing" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Скруглением вершин",  CM_SOLIDVERTEXFILLET )
#else
          MENUITEM( "Vertices fillet",     CM_SOLIDVERTEXFILLET )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Из листового материала" )
#else
        POPUP( "From sheet material" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По плоскому контуру",  CM_CREATESHEETSOLID )
#else
          MENUITEM( "By a planar contour",  CM_CREATESHEETSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Со сгибом вдоль отрезка",      CM_BENDOVERSEGMENT )
#else
          MENUITEM( "With a bend along a segment",  CM_BENDOVERSEGMENT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "С подсечкой",  CM_JOGSHEETSOLID )
#else
          MENUITEM( "With a jog",   CM_JOGSHEETSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Со сгибом по рёбрам",   CM_BENDBYEDGES )
#else
          MENUITEM( "With a bend by edges",  CM_BENDBYEDGES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "С комбинированным сгибом",  CM_JOINTBEND )
#else
          MENUITEM( "With a combined bend",      CM_JOINTBEND )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Добавлением пластины",    CM_SHEETSOLIDPLATE )
#else
          MENUITEM( "By addition of a plate",  CM_SHEETSOLIDPLATE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Вырезом отверстия",  CM_SHEETSOLIDHOLE )
#else
          MENUITEM( "By making a hole",   CM_SHEETSOLIDHOLE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Пересечением с контуром",         CM_SHEETSOLIDINTERSECT )
#else
          MENUITEM( "By intersection with a contour",  CM_SHEETSOLIDINTERSECT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "С замыканием угла",    CM_CLOSECORNER )
#else
          MENUITEM( "With corner closure",  CM_CLOSECORNER )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "С штамповкой",    CM_STAMPSHEETSOLID )
#else
          MENUITEM( "With stamping",   CM_STAMPSHEETSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "С буртиком",   CM_BEADSHEETSOLID )
#else
          MENUITEM( "With a bead",  CM_BEADSHEETSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "С жалюзи",       CM_JALOUSIESHEETSOLID )
#else
          MENUITEM( "With jalousie",  CM_JALOUSIESHEETSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Обечайку",     CM_CREATERULEDSOLID )
#else
          MENUITEM( "Ruled shell",  CM_CREATERULEDSOLID )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Прямым редактированием" )
#else
        POPUP( "By direct editing" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Трансформированием габарита",    CM_TRANSFORMEDSOLID )
#else
          MENUITEM( "Transformation of bounding box", CM_TRANSFORMEDSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Удалением группы граней",   CM_REMOVEMODIFIEDSOLID )
#else
          MENUITEM( "Removal a group of faces",  CM_REMOVEMODIFIEDSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Созданием из группы граней",      CM_CREATEMODIFIEDSOLID )
#else
          MENUITEM( "Creation from a group of faces",  CM_CREATEMODIFIEDSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Перемещением группы граней",       CM_ACTIONMODIFIEDSOLID )
#else
          MENUITEM( "Translation of a group of faces",  CM_ACTIONMODIFIEDSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Модифицированием грани",  CM_OFFSETMODIFIEDSOLID )
#else
          MENUITEM( "Face modification",       CM_OFFSETMODIFIEDSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Модифицированием скругления", CM_FILLETMODIFIEDSOLID )
#else
          MENUITEM( "Fillet modification",         CM_FILLETMODIFIEDSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Деформированием граней",  CM_SUPPLEMODIFIEDSOLID )
#else
          MENUITEM( "Deformation of faces",    CM_SUPPLEMODIFIEDSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Установкой сопряжений",     CM_SUPPLESETCONJUGATION )
#else
          MENUITEM( "Specifying a conjugation",  CM_SUPPLESETCONJUGATION )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Подобием",       CM_SUPPLESETSIMILARITY )
#else
          MENUITEM( "By similarity",  CM_SUPPLESETSIMILARITY )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Сглаживанием",  CM_SUPPLESETSMOOTHING )
#else
          MENUITEM( "Smoothing",     CM_SUPPLESETSMOOTHING )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Удалением скруглений",  CM_PURIFYMODIFIEDSOLID )
#else
          MENUITEM( "Deletion of fillets",   CM_PURIFYMODIFIEDSOLID )
#endif
        ENDPOPUP
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Оболочку" )
#else
      POPUP( "Shell" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "По точкам" )
#else
        POPUP( "From points" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Nurbs-поверхность",  CM_NEWNURBSSURFACESHELL )
#else
          MENUITEM( "Nurbs-surface",      CM_NEWNURBSSURFACESHELL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По группам точек ",     CM_LOFTEDSHELLBYPOINTS )
#else
          MENUITEM( "By gropes of points ",    CM_LOFTEDSHELLBYPOINTS )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На базе кривых" )
#else
        POPUP( "Based on curves" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Выдавливанием кривой",  CM_NEWCURVEEXTRUSIONSHELL )
#else
          MENUITEM( "Curve extrusion",       CM_NEWCURVEEXTRUSIONSHELL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Вращением кривой",  CM_NEWCURVEREVOLUTIONSHELL )
#else
          MENUITEM( "Curve revolution",  CM_NEWCURVEREVOLUTIONSHELL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Движением кривой",  CM_NEWCURVEEVOLUTIONSHELL )
#else
          MENUITEM( "Curve evolution",   CM_NEWCURVEEVOLUTIONSHELL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По кривым ",     CM_LOFTEDSHELLBYCURVES )
#else
          MENUITEM( "By curves ",    CM_LOFTEDSHELLBYCURVES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По плоским сечениям",  CM_NEWCURVELOFTEDSHELL )
#else
          MENUITEM( "Lofting by planar curves",      CM_NEWCURVELOFTEDSHELL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По сечениям и направляющей", CM_NEWCURVELOFTEDSHELL_CL )
#else
          MENUITEM( "Lofting with a guide curve", CM_NEWCURVELOFTEDSHELL_CL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По сетке кривых",       CM_NEWMESHSHELL )
#else
          MENUITEM( "From a set of curves",  CM_NEWMESHSHELL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Линейчатая по двум кривым",  CM_NEWRULSHELL )
#else
          MENUITEM( "Ruled from two curves",      CM_NEWRULSHELL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Поверхность соединения",  CM_NEWJOINSOLID )
#else
          MENUITEM( "Join surface",            CM_NEWJOINSOLID )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На базе поверхности" )
#else
        POPUP( "Based on a surface" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По поверхности",  CM_SHELLSURFACE )
#else
          MENUITEM( "By surface",      CM_SHELLSURFACE )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На основе оболочки" )
#else
        POPUP( "Based on a shell" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Соединением с оболочкой",  CM_SOLIDVARIETY )
#else
          MENUITEM( "Joining with a shell",     CM_SOLIDVARIETY )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Вычитанием оболочки",     CM_SOLIDEXTERNAL )
#else
          MENUITEM( "Subtraction of a shell",  CM_SOLIDEXTERNAL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Ограничением оболочкой",  CM_SOLIDINTERNAL )
#else
          MENUITEM( "Clipping by a shell",     CM_SOLIDINTERNAL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Разрезанную кривой",  CM_CUTSHELL_BYCONTOUR )
#else
          MENUITEM( "Cut by a curve",      CM_CUTSHELL_BYCONTOUR )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Разрезанную поверхностью",  CM_CUTSHELL_BYSURFACE )
#else
          MENUITEM( "Cut by a surface",          CM_CUTSHELL_BYSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Усечением объектами",   CM_CUTSHELL_BYOBJECTS )
#else
          MENUITEM( "Truncated by objects",  CM_CUTSHELL_BYOBJECTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Удалением граней",      CM_SHELLOFSOLID )
#else
          MENUITEM( "By removing of faces",  CM_SHELLOFSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Эквидистантную к граням",  CM_OFFSETSHELLBYFACES )
#else
          MENUITEM( "Offset to faces",          CM_OFFSETSHELLBYFACES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Удлинением грани",  CM_NEWEXTENSIONSHELL )
#else
          MENUITEM( "Face extension",    CM_NEWEXTENSIONSHELL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Сопряжением граней",  CM_FACESFILLET )
#else
          MENUITEM( "Faces fillet",        CM_FACESFILLET )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Сшивкой граней оболочек",    CM_SHELLSEW )
#else
          MENUITEM( "Stitching faces of shells",  CM_SHELLSEW )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Сшивкой тел",            CM_SOLIDSEW )
#else
          MENUITEM( "Stitching of solids",    CM_SOLIDSEW )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Заплатку" )
#else
        POPUP( "Patch" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По рёбрам",  CM_SHELLPATCH )
#else
          MENUITEM( "By edges",   CM_SHELLPATCH )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По кривым",  CM_CURVEPATCH )
#else
          MENUITEM( "By curves",  CM_CURVEPATCH )
#endif
        ENDPOPUP
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Поверхность" )
#else
      POPUP( "Surface" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Элементаную" )
#else
        POPUP( "Elementary" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Плоскость",  CM_NEWPLANE3POINTS )
#else
          MENUITEM( "Plane", CM_NEWPLANE3POINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Цилиндр",  CM_NEWCYLINDER3POINTS )
#else
          MENUITEM( "Cylinder", CM_NEWCYLINDER3POINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Конус",  CM_NEWCONE3POINTS )
#else
          MENUITEM( "Cone", CM_NEWCONE3POINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Сферу",  CM_NEWSPHERE3POINTS )
#else
          MENUITEM( "Sphere", CM_NEWSPHERE3POINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Тор",  CM_NEWTORUS3POINTS )
#else
          MENUITEM( "Torus", CM_NEWTORUS3POINTS )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "По точкам" )
#else
        POPUP( "Based on points" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Безье треугольник", CM_NEWTRIBEZIERSURFACE )
#else
          MENUITEM( "Bezier triangle",   CM_NEWTRIBEZIERSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "NURBS четарёхугольник", CM_NEWRECTANGLENURBS )
#else
          MENUITEM( "NURBS quadrangle",      CM_NEWRECTANGLENURBS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "NURBS", CM_NEWSPLINESURFACE )
#else
          MENUITEM( "NURBS", CM_NEWSPLINESURFACE )
#endif
#ifdef C3D_DEBUG // пока только под отладкой
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Симплексную", CM_NEWSIMPLEXSURFACE )
#else
          MENUITEM( "Simplex",     CM_NEWSIMPLEXSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Симплексный сплайн", CM_NEWSIMPLEXSPLINESURFACE )
#else
          MENUITEM( "Simplex spline",     CM_NEWSIMPLEXSPLINESURFACE )
#endif
#endif // C3D_DEBUG
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Плоскость по центру ", CM_NEWPLANE )
#else
          MENUITEM( "Plane by a center ",   CM_NEWPLANE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Плоскость через ребро",  CM_NEWPLANEEDGE )
#else
          MENUITEM( "Plane through an edge",  CM_NEWPLANEEDGE )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На базе кривых" )
#else
        POPUP( "Based on curves" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Выдавливания", CM_NEWEXTRUSION )
#else
          MENUITEM( "Extrusion",    CM_NEWEXTRUSION )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Вращения",   CM_NEWREVOLUTION )
#else
          MENUITEM( "Revolution", CM_NEWREVOLUTION )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Движения", CM_NEWEXPANSION )
#else
          MENUITEM( "Motion", CM_NEWEXPANSION )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Кинематическую", CM_NEWEVOLUTION )
#else
          MENUITEM( "Sweeping",      CM_NEWEVOLUTION )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Спиральную", CM_NEWSPIRALSURFACE )
#else
          MENUITEM( "Spiral",     CM_NEWSPIRALSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Линейчатую", CM_NEWRULEDSURFACE )
#else
          MENUITEM( "Ruled",      CM_NEWRULEDSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Секториальную", CM_NEWSECTORSURFACE )
#else
          MENUITEM( "Sectorial",     CM_NEWSECTORSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По трем кривым",  CM_NEWCORNERSURFACE )
#else
          MENUITEM( "By three curves", CM_NEWCORNERSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По четырем кривым", CM_NEWCOVERSURFACE )
#else
          MENUITEM( "By four curves",    CM_NEWCOVERSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "На семействе кривых",  CM_NEWLOFTEDSURFACE )
#else
          MENUITEM( "Lofting", CM_NEWLOFTEDSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "На семействе с направляющей", CM_NEWELEVATIONSURFACE )
#else
          MENUITEM( "Lofting with a guide", CM_NEWELEVATIONSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "На сетке кривых",           CM_NEWMESHSURFACE )
#else
          MENUITEM( "Based on a mesh of curves", CM_NEWMESHSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "На семействе NURBS",      CM_NEWNURBSSURFACE )
#else
          MENUITEM( "Based on a set of NURBS", CM_NEWNURBSSURFACE )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На базе поверхности" )
#else
        POPUP( "Based on a surface" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Эквидистантную", CM_NEWOFFSETSURFACE )
#else
          MENUITEM( "Offset",         CM_NEWOFFSETSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Деформированную", CM_NEWDEFORMEDSURFACE )
#else
          MENUITEM( "Deformed",        CM_NEWDEFORMEDSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Продленную параметрами", CM_NEWEXTENDEDSURFACE )
#else
          MENUITEM( "Extended by parameters", CM_NEWEXTENDEDSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Соединения", CM_NEWJOINSURFACE )
#else
          MENUITEM( "Of joint",   CM_NEWJOINSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Усеченную кривыми",   CM_NEWCURVEBOUNDEDSURFACE )
#else
          MENUITEM( "Truncated by curves", CM_NEWCURVEBOUNDEDSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По триангуляции грани",     CM_NEWGRIDSURFACE )
#else
          MENUITEM( "By the face triangulation", CM_NEWGRIDSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "NURBS копию",   CM_TRANSLATESURFACETONURBS )
#else
          MENUITEM( "NURBS replica", CM_TRANSLATESURFACETONURBS )
#endif
        ENDPOPUP
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Кривую в пространстве" )
#else
      POPUP( "Curve in space" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Прямую" )
#else
        POPUP( "Line" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По двум точкам", CM_NEWLINE3D )
#else
          MENUITEM( "By two points",  CM_NEWLINE3D )
#endif
//#if defined ( __NATIVE_LANGUAGE__ )
//          MENUITEM( "Из точки перпендикулярно кривой",            CM_NEWLINEPOINTPERPCURVE )
//#else
//          MENUITEM( "Through a point perpendicularly to a curve", CM_NEWLINEPOINTPERPCURVE )
//#endif
//#if defined ( __NATIVE_LANGUAGE__ )
//          MENUITEM( "Из точки перпендикулярно поверхности",         CM_NEWLINEPOINTPERPSURFACE )
//#else
//          MENUITEM( "Through a point perpendicularly to a surface", CM_NEWLINEPOINTPERPSURFACE )
//#endif
//#if defined ( __NATIVE_LANGUAGE__ )
//          MENUITEM( "Из точки перпендикулярно телу",              CM_NEWLINEPOINTPERPSOLID )
//#else
//          MENUITEM( "Through a point perpendicularly to a solid", CM_NEWLINEPOINTPERPSOLID )
//#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Пересечения двух плоскостей", CM_NEWLINEPLANEPLANE )
#else
          MENUITEM( "Two planes intersection",     CM_NEWLINEPLANEPLANE )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Плоскую" )
#else
        POPUP( "Planar" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Прямоугольник", CM_RECTANGLEONPLANE )
#else
          MENUITEM( "Rectangle",     CM_RECTANGLEONPLANE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Кубический сплайн", CM_NEWPLANECBSPLN )
#else
          MENUITEM( "Cubic spline",      CM_NEWPLANECBSPLN )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "NURBS", CM_NEWPLANENURBS )
#else
          MENUITEM( "NURBS", CM_NEWPLANENURBS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Эквидистантную", CM_NEWPLANEOFFSET )
#else
          MENUITEM( "Equidistant",    CM_NEWPLANEOFFSET )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Контур",  CM_NEWCONTOURONPLANE )
#else
          MENUITEM( "Contour", CM_NEWCONTOURONPLANE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Копию кривой на плоскости",    CM_NEWCURVEBYPLANE )
#else
          MENUITEM( "Copy of a curve on the plane", CM_NEWCURVEBYPLANE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По плоской пространственной", CM_GETPLANECURVE )
#else
          MENUITEM( "Based on a planar curve",     CM_GETPLANECURVE )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Коническое сечение" )
#else
        POPUP( "Conic section" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Окружность", CM_NEWCIRCLE3D )
#else
          MENUITEM( "Circle",     CM_NEWCIRCLE3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Окружность по центру", CM_NEWCIRCLE3DBYCENTRE )
#else
          MENUITEM( "Circle by a center",   CM_NEWCIRCLE3DBYCENTRE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Окружность по точкам", CM_NEWCIRCLE3DBYPOINTS )
#else
          MENUITEM( "Circle by points",     CM_NEWCIRCLE3DBYPOINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Эллипс",  CM_NEWELLIPSE3D )
#else
          MENUITEM( "Ellipse", CM_NEWELLIPSE3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          POPUP( "NURBS" )
#else
          POPUP( "NURBS" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
            MENUITEM( "По 2-ум точкам вершине и дискриминанту",   CM_NEWNURBSCONIC3D_1 )
#else
            MENUITEM( "By two points, a vertex and discriminant", CM_NEWNURBSCONIC3D_1 )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
            MENUITEM( "По 3-ем точкам и вершине",     CM_NEWNURBSCONIC3D_2 )
#else
            MENUITEM( "By three points and a vertex", CM_NEWNURBSCONIC3D_2 )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
            MENUITEM( "По 3-ем точкам и 2-ум наклонам", CM_NEWNURBSCONIC3D_3 )
#else
            MENUITEM( "By three points and 2 slopes",   CM_NEWNURBSCONIC3D_3 )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
            MENUITEM( "По 2-ум точкам 2-ум наклонам и дискриминанту", CM_NEWNURBSCONIC3D_4 )
#else
            MENUITEM( "By two points, two slopes and discriminant",   CM_NEWNURBSCONIC3D_4 )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
            MENUITEM( "По 4-ем точкам и наклону",   CM_NEWNURBSCONIC3D_5 )
#else
            MENUITEM( "By four points and a slope", CM_NEWNURBSCONIC3D_5 )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
            MENUITEM( "По 5-ти точкам", CM_NEWNURBSCONIC3D_6 )
#else
            MENUITEM( "By five points", CM_NEWNURBSCONIC3D_6 )
#endif
          ENDPOPUP
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "По точкам" )
#else
        POPUP( "By points" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Отрезок", CM_NEWLINESEGMENT3D )
#else
          MENUITEM( "Segment", CM_NEWLINESEGMENT3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Ломаную",  CM_NEWPOLYLINE3D )
#else
          MENUITEM( "Polyline", CM_NEWPOLYLINE3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Кубический сплайн", CM_NEWCUBICSPLINE3D )
#else
          MENUITEM( "Cubic spline",      CM_NEWCUBICSPLINE3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Сплайн Эрмита",  CM_NEWHERMIT3D )
#else
          MENUITEM( "Hermite spline", CM_NEWHERMIT3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Сплайн Безье",  CM_NEWBEZIER3D )
#else
          MENUITEM( "Bezier spline", CM_NEWBEZIER3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "NURBS", CM_NEWNURBS3D )
#else
          MENUITEM( "NURBS", CM_NEWNURBS3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "NURBS c сопряжениями", CM_NEWNURBS3D_WITH_MATING )
#else
          MENUITEM( "NURBS with matings",   CM_NEWNURBS3D_WITH_MATING )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Дугу окружности", CM_NEWARC3D )
#else
          MENUITEM( "Circular arc",    CM_NEWARC3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Дугу по точкам", CM_NEWARC3DBYPOINTS )
#else
          MENUITEM( "Arc by points",  CM_NEWARC3DBYPOINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Спираль", CM_NEWSPIRALCURVE )
#else
          MENUITEM( "Spiral",  CM_NEWSPIRALCURVE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Контур",  CM_NEWCONTOUR3D )
#else
          MENUITEM( "Contour", CM_NEWCONTOUR3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "B_сплайн", CM_CURVE_B_SPLINE )
#else
          MENUITEM( "B_spline", CM_CURVE_B_SPLINE )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На базе кривых" )
#else
        POPUP( "Based on curves" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Усеченную параметрами",  CM_NEWTRIMMEDCURVE3D )
#else
          MENUITEM( "Trimmed by parameters",  CM_NEWTRIMMEDCURVE3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Продленную параметрами",  CM_NEWEXTENDEDCURVE3D )
#else
          MENUITEM( "Extended by parameters",  CM_NEWEXTENDEDCURVE3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Эквидистантную",  CM_NEWOFFSETCURVE3D )
#else
          MENUITEM( "Equidistant",     CM_NEWOFFSETCURVE3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Кривую-мостик двух кривых",  CM_NEWCURVESBRIDGE3D )
#else
          MENUITEM( "Bridge between two curves",  CM_NEWCURVESBRIDGE3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Соединяющую две трубы",  CM_NEWCURVESCOUPLE3D )
#else
          MENUITEM( "Pipes connection",       CM_NEWCURVESCOUPLE3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Скругляющую две кривые",  CM_NEWCURVESFILLET3D )
#else
          MENUITEM( "Smoothing two curves",    CM_NEWCURVESFILLET3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Соединяющую две кривые",  CM_NEWCURVESCONNCURVE3D )
#else
          MENUITEM( "Connecting two curves",    CM_NEWCURVESCONNCURVE3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Контур",   CM_NEWCONTOUR3DBYCURVES )
#else
          MENUITEM( "Contour",  CM_NEWCONTOUR3DBYCURVES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Контур со скруглением",  CM_NEWFILLETCONTOUR3DBYCURVES )
#else
          MENUITEM( "Contour with fillet",    CM_NEWFILLETCONTOUR3DBYCURVES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По двум плоским проекциям",  CM_NEWCURVE_BY_TWO_PROJECTIONS )
#else
          MENUITEM( "By two planar projections",  CM_NEWCURVE_BY_TWO_PROJECTIONS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Копию кривой на плоскости",  CM_NEWSPACECURVEBYPLANE )
#else
          MENUITEM( "Planar curve replica",       CM_NEWSPACECURVEBYPLANE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "NURBS копию",    CM_TRANSLATECURVETONURBS )
#else
          MENUITEM( "NURBS replica",  CM_TRANSLATECURVETONURBS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Аппроксимацией дугами и отрезками",            CM_APPROXBYARCS )
#else
          MENUITEM( "Straight and circular segments approximation", CM_APPROXBYARCS )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На базе поверхности" )
#else
        POPUP( "Based on surfaces" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Границу поверхности",  CM_NEWSURFACEBORDER )
#else
          MENUITEM( "Surface boundary",     CM_NEWSURFACEBORDER )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Проекцию кривой на поверхность",        CM_NEWCURVEONSURFACE )
#else
          MENUITEM( "Projection of a curve onto a surface",  CM_NEWCURVEONSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Проекцию кривой по направлению",        CM_NEWCURVEDIRECTVECTOR )
#else
          MENUITEM( "Projection of a curve by a direction",  CM_NEWCURVEDIRECTVECTOR )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Проекционную кривую на поверхности",  CM_NEWPROJECTIONCURVE_ON_SURFACE )
#else
          MENUITEM( "Projection curve on a surface",       CM_NEWPROJECTIONCURVE_ON_SURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Кривую на поверхности по точкам",  CM_NEWCURVE_ON_SURFACE_BY_POINTS )
#else
          MENUITEM( "Curve on a surface by points",     CM_NEWCURVE_ON_SURFACE_BY_POINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Обертывание кривыми поверхности", CM_NEWCURVESWRAPPING )
#else
          MENUITEM( "Surface curves wrapping",         CM_NEWCURVESWRAPPING )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На базе тела" )
#else
        POPUP( "Based on a solid" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Контур по ребрам",  CM_NEWEDGESCURVE )
#else
          MENUITEM( "Contour by edges",   CM_NEWEDGESCURVE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Контур по ребрам грани",  CM_NEWFACECONTOUR )
#else
          MENUITEM( "Contour by face edges",   CM_NEWFACECONTOUR )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Проекции рёбер грани",        CM_NEWFACEPROJECTION )
#else
          MENUITEM( "Edges projection on a face",  CM_NEWFACEPROJECTION )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Проекцию ребра на грань",    CM_NEWEDGEMAP )
#else
          MENUITEM( "Edge projection on a face",  CM_NEWEDGEMAP )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Пересечения" )
#else
        POPUP( "Intersection" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Двух поверхностей", CM_NEWSURFACEINTERSECTION )
#else
          MENUITEM( "Two surfaces",      CM_NEWSURFACEINTERSECTION )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Поверхности и тела",    CM_NEWSURFACESOLIDINTERSECT )
#else
          MENUITEM( "A surface and a solid", CM_NEWSURFACESOLIDINTERSECT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Двух тел",   CM_NEWSOLIDINTERSECTION )
#else
          MENUITEM( "Two solids", CM_NEWSOLIDINTERSECTION )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Двух граней тел",     CM_NEWFACESINTERSECTION )
#else
          MENUITEM( "Two faces of solids", CM_NEWFACESINTERSECTION )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На базе текста" )
#else
        POPUP( "Based on text" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Вводом формул",          CM_NEWCHARCURVE3D )
#else
          MENUITEM( "By specifying formulae", CM_NEWCHARCURVE3D )
#endif
        ENDPOPUP
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Точку в пространстве" )
#else
      POPUP( "Point in space" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "По координатам", CM_NEWPOINT )
#else
        MENUITEM( "By coordinates", CM_NEWPOINT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На базе кривой" )
#else
        POPUP( "Based on a curve" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Ближайшую проекцию на кривую",          CM_NEWNEARPOINTONCURVE )
#else
          MENUITEM( "The closest projection onto the curve", CM_NEWNEARPOINTONCURVE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Проекцию на кривую",         CM_NEWPOINTONCURVE )
#else
          MENUITEM( "Projection onto the curve",  CM_NEWPOINTONCURVE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Изоклины кривой ",    CM_NEWCURVEISOCLINAL )
#else
          MENUITEM( "Curve's isoclines ",  CM_NEWCURVEISOCLINAL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Вдоль параметра кривой",      CM_NEWEVENPARAM )
#else
          MENUITEM( "Along the curve parameter ",  CM_NEWEVENPARAM )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Вдоль длины кривой",      CM_NEWEVENLENGTH )
#else
          MENUITEM( "Along the curve length",  CM_NEWEVENLENGTH )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "На заданном расстоянии",  CM_DISTANCEALONG )
#else
          MENUITEM( "At the given distance",   CM_DISTANCEALONG )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Точки изоклин кривой",      CM_CREATECURVEISOPOINTS )
#else
          MENUITEM( "Points of isocline curve",  CM_CREATECURVEISOPOINTS )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На базе поверхности" )
#else
        POPUP( "Based on a surface" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Ближайшую проекцию на поверхность",     CM_NEWNEARPOINTONSURFACE )
#else
          MENUITEM( "The closest projection onto a surface", CM_NEWNEARPOINTONSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Проекцию на поверхность",    CM_NEWPOINTONSURFACE )
#else
          MENUITEM( "Projection onto a surface",  CM_NEWPOINTONSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Проекцию на пов. по направлению",         CM_NEWDIRECTPOINTONSURFACE )
#else
          MENUITEM( "Projection onto a surface by direction",  CM_NEWDIRECTPOINTONSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Изоклины поверхности",    CM_NEWSURFACEISOCLINAL )
#else
          MENUITEM( "Isoclines of a surface",  CM_NEWSURFACEISOCLINAL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Группу точек на поверхности",   CM_NEWPOINTS_ON_SURFACE )
#else
          MENUITEM( "Group of points on a surface",  CM_NEWPOINTS_ON_SURFACE )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На базе тела" )
#else
        POPUP( "Based on a solid" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Проекцию на тело",         CM_NEWPOINTONSOLID )
#else
          MENUITEM( "Projection onto a solid",  CM_NEWPOINTONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Проекцию на тело по направлению",      CM_NEWDIRECTPOINTONSOLID )
#else
          MENUITEM( "Projection onto a solid by direction", CM_NEWDIRECTPOINTONSOLID )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Пересечения" )
#else
        POPUP( "Intersection" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Двух кривых", CM_NEWPOINTCURVECROSSING )
#else
          MENUITEM( "Two curves",  CM_NEWPOINTCURVECROSSING )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Кривой и поверхности",  CM_NEWPOINTCURVESURFACE )
#else
          MENUITEM( "A curve and a surface", CM_NEWPOINTCURVESURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Кривой и тела",       CM_NEWPOINTCURVESOLID )
#else
          MENUITEM( "A curve and a solid", CM_NEWPOINTCURVESOLID )
#endif
        ENDPOPUP
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Объект модели" )
#else
      POPUP( "Model object" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Вставку объекта",  CM_NEWINSTANCE )
#else
        MENUITEM( "Assembly",         CM_NEWASSEMBLY )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Сборочную единицу",  CM_NEWASSEMBLY )
#else
        MENUITEM( "Instance",           CM_NEWINSTANCE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Проволочный каркас кривых", CM_NEW_WIREFRAME_BY_CURVES )
#else
        MENUITEM( "Wireframe on curves",  CM_NEW_WIREFRAME_BY_CURVES )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Проекцию объекта" )
#else
      POPUP( "Projection of a object" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Кривой",  CM_NEWCURVEHIDEMAP )
#else
        MENUITEM( "Curve",   CM_NEWCURVEHIDEMAP )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Поверхности",   CM_NEWSURFACEHIDEMAP )
#else
        MENUITEM( "Surface",       CM_NEWSURFACEHIDEMAP )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Тела",   CM_NEWSOLIDHIDEMAP )
#else
        MENUITEM( "Solid",  CM_NEWSOLIDHIDEMAP )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Сборки тел",  CM_NEWSOLIDSHIDEMAP )
#else
        MENUITEM( "Assembly",    CM_NEWSOLIDSHIDEMAP )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Сечение тела контуром",      CM_NEW_SOLID_CONTOUR_SECTION_MAP )
#else
        MENUITEM( "Solid section by a contour", CM_NEW_SOLID_CONTOUR_SECTION_MAP )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Местный разрез тела",   CM_NEW_SOLID_LOCAL_SECTION_MAP )
#else
        MENUITEM( "Solid local section",   CM_NEW_SOLID_LOCAL_SECTION_MAP )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Резьбу" )
#else
      POPUP( "Thread" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Без привязки к телу",        CM_NEWTHREAD_INSPACE )
#else
        MENUITEM( "Without linking to a solid", CM_NEWTHREAD_INSPACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "С привязкой к телу",  CM_NEWTHREAD_ONSOLID )
#else
        MENUITEM( "Linked to a solid",   CM_NEWTHREAD_ONSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Отображение резьбы (по параметрам )", CM_NEWTHREAD_MAPPING_BYPARAMS )
#else
        MENUITEM( "Mapping of a thread (by parameters)", CM_NEWTHREAD_MAPPING_BYPARAMS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Отображение резьбы (по объекту )",  CM_NEWTHREAD_MAPPING_BYTHREAD )
#else
        MENUITEM( "Mapping of a thread (by object)",   CM_NEWTHREAD_MAPPING_BYTHREAD )
#endif
      ENDPOPUP
//#if defined ( __NATIVE_LANGUAGE__ )
//      POPUP( "Конструктивный объект" )
//#else
//      POPUP( "Constructive object" )
//#endif
//#if defined ( __NATIVE_LANGUAGE__ )
//        MENUITEM( "По трем точкам",   CM_NEW_CONSTRUCTIVE_BY_POINTS )
//#else
//        MENUITEM( "By three points",  CM_NEW_CONSTRUCTIVE_BY_POINTS )
//#endif
//      ENDPOPUP
      SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Кривую на плоскости" )
#else
      POPUP( "Curve on plane" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Прямую" )
#else
        POPUP( "Line" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По двум точкам", CM_LINEPNTPNT )
#else
          MENUITEM( "By two points",  CM_LINEPNTPNT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По точке и углу",         CM_LINEPNTANLGE0X )
#else
          MENUITEM( "By a point and an angle", CM_LINEPNTANLGE0X )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По точке и углу к линии",         CM_LINEPOINTANGLE )
#else
          MENUITEM( "By a point and an angle to line", CM_LINEPOINTANGLE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Параллельно на расстоянии", CM_LINEPNTDIST )
#else
          MENUITEM( "Parallel at distance",      CM_LINEPNTDIST )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По точке параллельно",               CM_LINEPRLPNT )
#else
          MENUITEM( "Parallel existing through a point ", CM_LINEPRLPNT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Перпендикуляр из точки",     CM_LINEPRPPNT )
#else
          MENUITEM( "Perpendicular from a point", CM_LINEPRPPNT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Касательную в точке", CM_LINEPNTTANG )
#else
          MENUITEM( "Tangent at a point",  CM_LINEPNTTANG )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Касательную к двум кривым", CM_LINETAN2CIR )
#else
          MENUITEM( "Tangent to two curves",     CM_LINETAN2CIR )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Касательную под углом", CM_LINEANGLETAN_CIRCLE )
#else
          MENUITEM( "Tangent at an angle",   CM_LINEANGLETAN_CIRCLE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Гипотенузу", CM_LINEBISECTOR )
#else
          MENUITEM( "Hypotenuse", CM_LINEBISECTOR )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Горизонтальную", CM_GORISONTALLINE )
#else
          MENUITEM( "Horizontal",     CM_GORISONTALLINE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Вертикальную", CM_VERTICALLINE )
#else
          MENUITEM( "Vertical",     CM_VERTICALLINE )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Окружность" )
#else
        POPUP( "Circle" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По трем точкам",  CM_CIRCLETHREEPOINTS )
#else
          MENUITEM( "By three points", CM_CIRCLETHREEPOINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По центру и точке",       CM_CIRCLECENTREPOINTON )
#else
          MENUITEM( "By a center and a point", CM_CIRCLECENTREPOINTON )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По центру и радиусу",    CM_CIRCLECENTRERADIUS )
#else
          MENUITEM( "By a center and radius", CM_CIRCLECENTRERADIUS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По радиусу и двум точкам",             CM_CIRCLERADIUSTWOPOINTS )
#else
          MENUITEM( "Two circles by radius and two points", CM_CIRCLERADIUSTWOPOINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По центру на объекте, точке и радиусу",   CM_CIRCLECENTREONRADIUSPOINTON )
#else
          MENUITEM( "By centre on a object, point and radius", CM_CIRCLECENTREONRADIUSPOINTON )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По центру на объекте и двум точкам",   CM_CIRCLECENTREONTWOPOINTS )
#else
          MENUITEM( "By centre on a object and two points", CM_CIRCLECENTREONTWOPOINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Касательную по центру",  CM_CIRCLETANENTITYCENTRE )
#else
          MENUITEM( "Tangent given a center", CM_CIRCLETANENTITYCENTRE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Касательную по двум точкам", CM_CIRCLETANENTITYTWOPOINTS )
#else
          MENUITEM( "Tangent by two points",      CM_CIRCLETANENTITYTWOPOINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Касательную по точке и радиусу",    CM_CIRCLETANENTITYRADIUSPOINTON )
#else
          MENUITEM( "Tangent by a point given a radius", CM_CIRCLETANENTITYRADIUSPOINTON )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Касательную к двум объектам по точке", CM_CIRCLETANTWOENTITIESPOINTON )
#else
          MENUITEM( "Tangent to two objects by a point",    CM_CIRCLETANTWOENTITIESPOINTON )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Касательную к двум объектам", CM_CIRCLETANTWOENTITIESRADIUS )
#else
          MENUITEM( "Tangent to two objects",      CM_CIRCLETANTWOENTITIESRADIUS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Касательную к трем объектам", CM_CIRCLETANTHREEENTITIES )
#else
          MENUITEM( "Tangent to three objects",    CM_CIRCLETANTHREEENTITIES )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Дугу окружности" )
#else
        POPUP( "Circular arc" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По трем точкам",  CM_NEWARC_3_POINTS )
#else
          MENUITEM( "By three points", CM_NEWARC_3_POINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По двум точкам и радиусу",   CM_ARC2POINTSRADIUS )
#else
          MENUITEM( "By two points and a radius", CM_ARC2POINTSRADIUS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По центру и двум точкам",      CM_ARCCENTRE2POINTS )
#else
          MENUITEM( "From a center and two points", CM_ARCCENTRE2POINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "По окружности и точкам",    CM_ARCCIRCLE2POINTS )
#else
          MENUITEM( "Given a circle and points", CM_ARCCIRCLE2POINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Касательную по точкам", CM_ARCTANGENTCURVE2POINTS )
#else
          MENUITEM( "Tangent by points",     CM_ARCTANGENTCURVE2POINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Касательную по точке и радиусу",    CM_ARCTANGENTCURVERADPOINTON )
#else
          MENUITEM( "Tangent by a point given a radius", CM_ARCTANGENTCURVERADPOINTON )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Коническое сечение" )
#else
        POPUP( "Conic section" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Эллипс по центру и точкам",      CM_ELLIPSECENTRE3POINTS )
#else
          MENUITEM( "Ellipse by a center and points", CM_ELLIPSECENTRE3POINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Эллипс по точкам",  CM_ELLIPSE3POINTS )
#else
          MENUITEM( "Ellipse by points", CM_ELLIPSE3POINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Эллипс по центру и полуосям",      CM_ELLIPSECENTRE_AB )
#else
          MENUITEM( "Ellipse by a center and semiaxes", CM_ELLIPSECENTRE_AB )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Касательный эллипс", CM_ELLIPSE_TANGENT2CURVES )
#else
          MENUITEM( "Tangent ellipse",    CM_ELLIPSE_TANGENT2CURVES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Дугу эллипса",   CM_NEWELLIPSEARC )
#else
          MENUITEM( "Elliptical arc", CM_NEWELLIPSEARC )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "NURBS дугу", CM_NEWCONIC )
#else
          MENUITEM( "NURBS arc",  CM_NEWCONIC )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "По точкам" )
#else
        POPUP( "Based on points" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Отрезок по точкам", CM_NEWSEGMENTPOINTPOINT )
#else
          MENUITEM( "Segment by points", CM_NEWSEGMENTPOINTPOINT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Отрезок по расстоянию",   CM_NEWLINESEGDMIN )
#else
          MENUITEM( "Segment by the distance", CM_NEWLINESEGDMIN )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Ломаную",  CM_NEWPOLYLINE )
#else
          MENUITEM( "Polyline", CM_NEWPOLYLINE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Прямоугольник", CM_RECTANGLE )
#else
          MENUITEM( "Rectangle",     CM_RECTANGLE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Многоугольник", CM_REGULARPOLYGON )
#else
          MENUITEM( "Polygon",       CM_REGULARPOLYGON )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Кубический сплайн", CM_NEWCBSPLN2D )
#else
          MENUITEM( "Cubic spline",      CM_NEWCBSPLN2D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Сплайн Эрмита",  CM_NEWHERMIT )
#else
          MENUITEM( "Hermite spline", CM_NEWHERMIT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Сплайн Безье",  CM_NEWBEZIER )
#else
          MENUITEM( "Bezier spline", CM_NEWBEZIER )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "NURBS", CM_NEWBSPLINE )
#else
          MENUITEM( "NURBS", CM_NEWBSPLINE )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На базе кривой" )
#else
        POPUP( "Based on a curve" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Эквидистантную", CM_SPLINE_EQUID )
#else
          MENUITEM( "Equidistant",    CM_SPLINE_EQUID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Эквидистантную продолженную", CM_NEWOFFSETEXTENDED )
#else
          MENUITEM( "Extended equidistant",        CM_NEWOFFSETEXTENDED )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Копию пространственной кривой", CM_NEWPLANECURVEBYSPACE )
#else
          MENUITEM( "Spatial curve replica",         CM_NEWPLANECURVEBYSPACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "NURBS копию",   CM_SPLINENURBS )
#else
          MENUITEM( "NURBS replica", CM_SPLINENURBS )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На базе тела" )
#else
        POPUP( "Base on a solid" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Проекцию ребра на грань",   CM_NEWEDGEMAPONPLANE )
#else
          MENUITEM( "Edge projection on a face", CM_NEWEDGEMAPONPLANE )
#endif
        ENDPOPUP
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Контур на плоскости" )
#else
      POPUP( "Contour on a plane" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Построением кривых",       CM_NEWCONTOUR )
#else
        MENUITEM( "Sequancial curves creation", CM_NEWCONTOUR )
#endif
// #if defined ( __NATIVE_LANGUAGE__ )
//         MENUITEM( "По точке внутри",   CM_NEWENVELOPCONTOUR )
// #else
//         MENUITEM( "By a point inside", CM_NEWENVELOPCONTOUR )
// #endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "С обходом по стрелке",     CM_HANDENVELOPE )
#else
        MENUITEM( "With bypass by the arrow", CM_HANDENVELOPE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Эквидистантный", CM_CONTOUREQUID )
#else
        MENUITEM( "Equidistant",    CM_CONTOUREQUID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Со скруглением угла",         CM_CONTOURFILLET )
#else
        MENUITEM( "With rounding of the corner", CM_CONTOURFILLET )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "С фаской угла",              CM_CONTOURCHAMFER )
#else
        MENUITEM( "With chamfer of the corner", CM_CONTOURCHAMFER )
#endif

#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Обхватывающие контуры", CM_ALL_EMBRACING_CNTRS )
#else
        MENUITEM( "Enclosing contours",    CM_ALL_EMBRACING_CNTRS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Вокруг точки",   CM_ENCLOSECONTOURS )
#else
        MENUITEM( "Around a point", CM_ENCLOSECONTOURS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Вокруг кривых", CM_OUTERCONTOURS )
#else
        MENUITEM( "Around curves", CM_OUTERCONTOURS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Восстановить контуры", CM_RECONSTRUCTCONTOURS )
#else
        MENUITEM( "Restore contours",     CM_RECONSTRUCTCONTOURS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Эквидистантный", CM_MAKEOFFSETCONTOUR )
#else
        MENUITEM( "Equidistant",    CM_MAKEOFFSETCONTOUR )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Эквидистантный выдавливания", CM_MAKESLOPECONTOUR )
#else
        MENUITEM( "Equidistant of extrusion",    CM_MAKESLOPECONTOUR )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Эквидистантный вращения", CM_MAKEOFFSETTHINCONTOUR )
#else
        MENUITEM( "Equidistant of rotation", CM_MAKEOFFSETTHINCONTOUR )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Выполнением булевой операции над регионами",   CM_REGIONSBOOLEANOPER )
#else
        MENUITEM( "By performing a boolean operation on regions", CM_REGIONSBOOLEANOPER )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Получением связных групп контуров",         CM_MAKE_ALL_REGIONS )
#else
        MENUITEM( "By obtaining connected groups of contours", CM_MAKE_ALL_REGIONS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Контур с разрывами",  CM_NEWCONTOURWITHBREAKS )
#else
        MENUITEM( "Contour with breaks", CM_NEWCONTOURWITHBREAKS )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Точку на плоскости" )
#else
      POPUP( "Point on the plane" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "По координатам", CM_POINTPOSITION )
#else
        MENUITEM( "By coordinates", CM_POINTPOSITION )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "На базе кривых" )
#else
        POPUP( "Based on curves" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "На кривой", CM_POINTON_CURVE )
#else
          MENUITEM( "On curve", CM_POINTON_CURVE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "На кривой по длине", CM_N_POINTS_ON_CURVE )
#else
          MENUITEM( "On curve by length", CM_N_POINTS_ON_CURVE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "На кривой по параметру", CM_POINTONPARAMCURVE )
#else
          MENUITEM( "On curve by parameter", CM_POINTONPARAMCURVE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Центр тяжести кривой", CM_POINTWEIGHTCENTRE )
#else
          MENUITEM( "Curve center of gravity", CM_POINTWEIGHTCENTRE )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Пересечения" )
#else
        POPUP( "Intersections" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Двух кривых", CM_EDITINTERSECT )
#else
          MENUITEM( "Of two curves", CM_EDITINTERSECT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Ближайщую", CM_POINTINTERSECT )
#else
          MENUITEM( "The closest", CM_POINTINTERSECT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Самопересечения", CM_EDITSELF_INTERSECT )
#else
          MENUITEM( "Self-intersections", CM_EDITSELF_INTERSECT )
#endif
        ENDPOPUP
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Мультилинию на плоскости" )
#else
      POPUP( "Multiline on a plane" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "На базе контура",    CM_CREATECNTRSMULTILINE )
#else
        MENUITEM( "Based on a contour", CM_CREATECNTRSMULTILINE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Построением базовой кривой",        CM_CREATEMULTILINE )
#else
        MENUITEM( "By construction of the base curve", CM_CREATEMULTILINE )
#endif
      ENDPOPUP
      SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Копию" )
#else
      POPUP( "Replica" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Объекта",    CM_DUPLICATEOBJECT )
#else
        MENUITEM( "Of object",  CM_DUPLICATEOBJECT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Зеркальную", CM_MIRRORDUPLICATEOBJECT )
#else
        MENUITEM( "Mirror",     CM_MIRRORDUPLICATEOBJECT )
#endif
      ENDPOPUP
      SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Сопряжение" )
#else
      POPUP( "Mating" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Совпадение",  CM_MATE_COINCIDENCE )
#else
        MENUITEM( "Coincidence", CM_MATE_COINCIDENCE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Параллельность", CM_MATE_PARALLELISM )
#else
        MENUITEM( "Parallelism",    CM_MATE_PARALLELISM )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Перпендикулярность", CM_MATE_PERPENDICULAR )
#else
        MENUITEM( "Perpendicularity",   CM_MATE_PERPENDICULAR )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Касание",  CM_MATE_TANGENCY )
#else
        MENUITEM( "Tangency", CM_MATE_TANGENCY )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Концентричность", CM_MATE_CONCENTRIC )
#else
        MENUITEM( "Concentricity",   CM_MATE_CONCENTRIC )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Метрическое расстояние", CM_MATE_DISTANCE )
#else
        MENUITEM( "Metric distance",        CM_MATE_DISTANCE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Угловое расстояние", CM_MATE_ANGLE )
#else
        MENUITEM( "Angular distance",   CM_MATE_ANGLE )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Геометрические ограничения" )
#else
      POPUP( "Geometric constraints" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Совпадение точек",   CM_GC_COINCIDENCE )
#else
        MENUITEM( "Points coincidence", CM_GC_COINCIDENCE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Параллельность", CM_GC_PARALLELISM )
#else
        MENUITEM( "Parallelism",    CM_GC_PARALLELISM )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Перпендикулярность", CM_GC_PERPENDICULAR )
#else
        MENUITEM( "Perpendicularity",   CM_GC_PERPENDICULAR )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Касание",  CM_GC_TANGENCY )
#else
        MENUITEM( "Tangency", CM_GC_TANGENCY )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Концентричность", CM_GC_CONCENTRIC )
#else
        MENUITEM( "Concentricity",   CM_GC_CONCENTRIC )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Метрическое расстояние", CM_GC_DISTANCE )
#else
        MENUITEM( "Metric distance",        CM_GC_DISTANCE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Угловое расстояние", CM_GC_ANGLE )
#else
        MENUITEM( "Angular distance",   CM_GC_ANGLE )
#endif
      ENDPOPUP
      SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Размер" )
#else
      POPUP( "Dimension" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "По точке на поверхности", CM_RADIUSDIMENSION )
#else
        MENUITEM( "By a point on a surface", CM_RADIUSDIMENSION )
#endif
      ENDPOPUP
    ENDPOPUP
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined ( __NATIVE_LANGUAGE__ )
    POPUP( "&Mодифицировать" )
#else
    POPUP( "&Modify" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Тело или оболочку" )
#else
      POPUP( "Solid or shell" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Перестроить",  CM_REBUILD_SHELL )
#else
        MENUITEM( "Rebuild",      CM_REBUILD_SHELL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Перестроить с выбором режима копирования",  CM_REBUILD_SHELL_WITHCOPYMODE )
#else
        MENUITEM( "Rebuild with a choice of copy mode",        CM_REBUILD_SHELL_WITHCOPYMODE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Превратить в поверхности",  CM_TRANSLATESOLIDTOSURFACES )
#else
        MENUITEM( "Transform to surfaces",     CM_TRANSLATESOLIDTOSURFACES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Вывернуть оболочку",           CM_SHELL_REVERSE )
#else
        MENUITEM( "Reverse a shell",              CM_SHELL_REVERSE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Разделить части",  CM_BREAKSOLIDONPARTS )
#else
        MENUITEM( "Split parts",      CM_BREAKSOLIDONPARTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Выбрать тело из частей тела",           CM_CREATE_PARTSOLID )
#else
        MENUITEM( "Select a solid from parts of a solid",  CM_CREATE_PARTSOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Объединить компланарные грани",  CM_UNIFYOWNCOMPLANARFACES )
#else
        MENUITEM( "Unite complanar faces",          CM_UNIFYOWNCOMPLANARFACES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Достроить пропущенную операцию",  CM_SOLIDFINISH )
#else
        MENUITEM( "Build a keeped operation",        CM_SOLIDFINISH )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Один шаг назад в построении",   CM_TRANSLATESOLIDTOBASIS )
#else
        MENUITEM( "Step back in building",         CM_TRANSLATESOLIDTOBASIS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Рассыпать в базовые объекты",  CM_BREAKSOLIDTOPARTS )
#else
        MENUITEM( "Break up into base objects",   CM_BREAKSOLIDTOPARTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Сдвинуть конец дерева",  CM_SOLIDENDMOVE )
#else
        MENUITEM( "Move end of build log",  CM_SOLIDENDMOVE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Оставить элементов в дереве",  CM_SOLIDENDCUT )
#else
        MENUITEM( "Keep elements in build log",   CM_SOLIDENDCUT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Удалить элемент дерева",           CM_SOLIDCREATORDELETE )
#else
        MENUITEM( "Remove a component of build log",  CM_SOLIDCREATORDELETE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Удалить историю построения",   CM_DELETE_ITEM_CREATORS )
#else
        MENUITEM( "Remove build log",             CM_DELETE_ITEM_CREATORS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Согнуть",  CM_BEND )
#else
        MENUITEM( "Bend",     CM_BEND )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Разогнуть",  CM_UNBEND )
#else
        MENUITEM( "Unbend",     CM_UNBEND )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Добавить атрибут",  CM_ADD_ATTTRIBUTE )
#else
        MENUITEM( "Add attribute",     CM_ADD_ATTTRIBUTE )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Поверхность" )
#else
      POPUP( "Surface" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Спроецировать базовые точки",  CM_GOSURFACETHROUGHPLANE )
#else
        MENUITEM( "Project base points",          CM_GOSURFACETHROUGHPLANE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Пройти через точки",   CM_GOSURFACETHROUGHPOINT )
#else
        MENUITEM( "Pass through points",  CM_GOSURFACETHROUGHPOINT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Развернуть замкнутый NURBS",  CM_CONVERTNURBSSURFACEINTOOPEN )
#else
        MENUITEM( "Open a closed NURBS",         CM_CONVERTNURBSSURFACEINTOOPEN )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Свернуть незамкнутый NURBS",  CM_CONVERTNURBSSURFACEINTOCLOSE )
#else
        MENUITEM( "Close an open NURBS",         CM_CONVERTNURBSSURFACEINTOCLOSE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Превратить в базовые объекты ", CM_TRANSLATESURFACETOBASIS )
#else
        MENUITEM( "Transform to base objects ",    CM_TRANSLATESURFACETOBASIS )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Проволочные каркас" )
#else
      POPUP( "Wire frame" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Перестроить", CM_REBUILD_WIRE_FRAME )
#else
        MENUITEM( "Rebuild", CM_REBUILD_WIRE_FRAME )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Перестроить все проволочные каркасы", CM_REBUILD_ALL_WIRE_FRAMES )
#else
        MENUITEM( "Rebuild all wire frames", CM_REBUILD_ALL_WIRE_FRAMES )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Кривую в пространстве" )
#else
      POPUP( "Curve in space" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Замкнуть/разомкнуть", CM_CURVESETCLOSED )
#else
        MENUITEM( "Set closed/open",     CM_CURVESETCLOSED )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Инвертировать направление", CM_CURVEINVERSE )
#else
        MENUITEM( "Inverse direction",         CM_CURVEINVERSE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Удалить часть", CM_DELETEMIDLEPART )
#else
        MENUITEM( "Remove a part", CM_DELETEMIDLEPART )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Оставить часть", CM_CREATEMIDLEPART )
#else
        MENUITEM( "Keep a part",    CM_CREATEMIDLEPART )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Разбить точкой", CM_CREATEBREAKPARTS )
#else
        MENUITEM( "Split by point", CM_CREATEBREAKPARTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Сдвинуть NURBS точку", CM_MOVECURVEPOINT )
#else
        MENUITEM( "Move NURBS point",     CM_MOVECURVEPOINT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Вставить NURBS точку", CM_CURVEADDPOINT )
#else
        MENUITEM( "Insert NURBS point",   CM_CURVEADDPOINT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Развернуть замкнутый NURBS", CM_CONVERTNURBSINTOOPEN )
#else
        MENUITEM( "Open a closed NURBS",        CM_CONVERTNURBSINTOOPEN )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Свернуть незамкнутый NURBS", CM_CONVERTNURBSINTOCLOSE )
#else
        MENUITEM( "Close an open NURBS",        CM_CONVERTNURBSINTOCLOSE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Состыковать сплайн c объектом", CM_ATTACHNURBSTOOBJECT )
#else
        MENUITEM( "Join a spline with an object",  CM_ATTACHNURBSTOOBJECT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Преобразовать в плоскую",     CM_TRANSLATECURVE3DTOCURVE )
#else
        MENUITEM( "Transform to the planar one", CM_TRANSLATECURVE3DTOCURVE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Вставить NURBS узел", CM_CURVEADDKNOTE )
#else
        MENUITEM( "Insert a NURBS knot", CM_CURVEADDKNOTE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Удалить NURBS узел",  CM_CURVEREMOVEKNOTE )
#else
        MENUITEM( "Remove a NURBS knot", CM_CURVEREMOVEKNOTE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Уменьшить порядок NURBS кривой на 1",     CM_REDUCE_NURBS_DEGREE )
#else
        MENUITEM( "Decrease the degree of NURBS curve by 1", CM_REDUCE_NURBS_DEGREE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Превратить в базовые объекты",  CM_TRANSLATECURVETOBASIS )
#else
        MENUITEM( "Transform to the base objects", CM_TRANSLATECURVETOBASIS )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Точку в пространстве" )
#else
      POPUP( "Point in space" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Сдвинуть вдоль кривой", CM_MOVEPOINTALONGLINE )
#else
        MENUITEM( "Move along a curve",    CM_MOVEPOINTALONGLINE )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Объект модели" )
#else
      POPUP( "Model object" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Перестроить", CM_REBUILD_ITEM )
#else
        MENUITEM( "Rebuild",     CM_REBUILD_ITEM )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Изменить параметры", CM_EDITPROPERTY )
#else
        MENUITEM( "Change parameters",  CM_EDITPROPERTY )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Дать базовые точки",      CM_ADDBASISPOINTS )
#else
        MENUITEM( "Specify the base points", CM_ADDBASISPOINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Превратить в базовые объекты", CM_TRANSLATETOBASIS )
#else
        MENUITEM( "Convert to the base object",   CM_TRANSLATETOBASIS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Превратить в слиток", CM_TRANSLATETOINGOT )
#else
        MENUITEM( "Convert to the ingot",   CM_TRANSLATETOINGOT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Преобразовать полигональную модель в тело", CM_MESH_TO_BREP )
#else
        MENUITEM( "Convert a mesh to the solid",   CM_MESH_TO_BREP )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Резьбу" )
#else
      POPUP( "Thread" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Сопрягаются ли резьбы",  CM_MODTHREAD_ISMATEDTHREADS )
#else
        MENUITEM( "Are the threads mated",  CM_MODTHREAD_ISMATEDTHREADS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Принадлежит ли резьба телу",        CM_MODTHREAD_ISBODYTHREAD )
#else
        MENUITEM( "Does a thread belong to the solid", CM_MODTHREAD_ISBODYTHREAD )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Адаптировать резьбу к телу",  CM_MODTHREAD_ADAPTTOBODY )
#else
        MENUITEM( "Adapt a thread to the solid", CM_MODTHREAD_ADAPTTOBODY )
#endif
      ENDPOPUP
      SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Кривую на плоскости" )
#else
      POPUP( "Curve on a plane" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Замкнуть/разомкнть", CM_SPLINESETCLOSED )
#else
        MENUITEM( "Set closed/open",    CM_SPLINESETCLOSED )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Инвертировать направление", CM_SPLINEINVERSE )
#else
        MENUITEM( "Inverse direction",         CM_SPLINEINVERSE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Удалить часть", CM_DELETEPART )
#else
        MENUITEM( "Delete a part", CM_DELETEPART )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Удалить между точками", CM_DELETEPARTP1P2 )
#else
        MENUITEM( "Delete between points", CM_DELETEPARTP1P2 )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Вырезать", CM_NEWTRIMMEDCURVE )
#else
        MENUITEM( "Cut out",  CM_NEWTRIMMEDCURVE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Разбить точкой",   CM_BREAKCURVE )
#else
        MENUITEM( "Split by a point", CM_BREAKCURVE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Скруглить кривые", CM_FILLETCURVES )
#else
        MENUITEM( "Fillet curves",    CM_FILLETCURVES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Фаска на кривых",   CM_CHAMFERCURVES )
#else
        MENUITEM( "Chamfer on curves", CM_CHAMFERCURVES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Вес точки сплайна",         CM_SPLINESET_WEIGHTS )
#else
        MENUITEM( "Wheight of a spline point", CM_SPLINESET_WEIGHTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Ввести точку в сплайн",      CM_SPLINENEW_POINT )
#else
        MENUITEM( "Insert a point to a spline", CM_SPLINENEW_POINT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Касательная к сплайну", CM_SPLINETANGENT )
#else
        MENUITEM( "Tangent spline",        CM_SPLINETANGENT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Усечь сплайн",  CM_SPLINETRIMM )
#else
        MENUITEM( "Trim a spline", CM_SPLINETRIMM )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Заменить точку сплайна",    CM_SPLINECHANGEPOINT )
#else
        MENUITEM( "Replace a point of spline", CM_SPLINECHANGEPOINT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Изменить порядок", CM_SETDEGREE )
#else
        MENUITEM( "Change the order", CM_SETDEGREE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Вставить узел в сплайн",  CM_SPLINEINSERTKNOT )
#else
        MENUITEM( "Insert a knot to spline", CM_SPLINEINSERTKNOT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Аппроксимировать дугами", CM_APPARCCURVE )
#else
        MENUITEM( "Approximate with arcs",   CM_APPARCCURVE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Преобразовать в пространственную", CM_TRANSLATECURVETOCURVE3D )
#else
        MENUITEM( "Transform to a spatial one",       CM_TRANSLATECURVETOCURVE3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Преобразовать в плоский контур",  CM_TRANSLATECURVETOCONTOURON )
#else
        MENUITEM( "Transform to a planar contour",   CM_TRANSLATECURVETOCONTOURON )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Развернуть замкнутый NURBS", CM_CONVERTPLANENURBSINTOOPEN )
#else
        MENUITEM( "Open a closed NURBS",        CM_CONVERTPLANENURBSINTOOPEN )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Свернуть незамкнутый NURBS", CM_CONVERTPLANENURBSINTOCLOSE )
#else
        MENUITEM( "Close an open NURBS",        CM_CONVERTPLANENURBSINTOCLOSE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Нормализовать", CM_NORMALIZEOBJECT )
#else
        MENUITEM( "Normalize",     CM_NORMALIZEOBJECT )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Контур на плоскости" )
#else
      POPUP( "Contour on a plane" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Установить направление", CM_CONTOURSENSE )
#else
        MENUITEM( "Set direction",          CM_CONTOURSENSE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Разрушить", CM_DISASSEMBLY )
#else
        MENUITEM( "Destroy",   CM_DISASSEMBLY )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Создать пространственную копию", CM_TRANSLATECONTOURTOCONTOURON )
#else
        MENUITEM( "Create a spatial copy",          CM_TRANSLATECONTOURTOCONTOURON )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Добавить невидимый участок в контур с разрывами", CM_ADDINVISIBLEPART )
#else
        MENUITEM( "Add an invisible part to a contour with breaks",  CM_ADDINVISIBLEPART )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Мультилинию на плоскости" )
#else
      POPUP( "Multiline on a plane" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Заменить базовую кривую мультилинии",   CM_REPLACEMLTLINESBASISCURVE )
#else
        MENUITEM( "Replace the base curve of a multiline", CM_REPLACEMLTLINESBASISCURVE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Вставить вершину в мультилинию",   CM_INSERTMLTLINESVERTEX )
#else
        MENUITEM( "Insert a vertex into a multiline", CM_INSERTMLTLINESVERTEX )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Удалить вершину мультилинии",    CM_REMOVEMLTLINESVERTEX )
#else
        MENUITEM( "Remove a vertex of a multiline", CM_REMOVEMLTLINESVERTEX )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Сместить hot-точку базовой кривой мультилинии", CM_SETMLTLINESHOTPOINT )
#else
        MENUITEM( "Move a hot-point of a multiline base curve",    CM_SETMLTLINESHOTPOINT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Удалить hot-точку базовой кривой мультилинии", CM_DELMLTLINESHOTPOINT )
#else
        MENUITEM( "Delete a hot-point of a multiline base curve", CM_DELMLTLINESHOTPOINT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Выставить флаг гладкий стык в вершине мультилинии", CM_SETMLTLINESVERTSMOOTHJOINT )
#else
        MENUITEM( "Set flag of smooth joint at a vertex of multiline", CM_SETMLTLINESVERTSMOOTHJOINT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Выставить тип обхода угла в вершине мультилинии",     CM_SETMLTLINESVERTTRACINGTYPE )
#else
        MENUITEM( "Set type of corner tracing at a vertex of multiline", CM_SETMLTLINESVERTTRACINGTYPE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Выставить радиус специального скругления в вершине мультилинии", CM_SETMLTLINESVERTSPECFILLETRAD )
#else
        MENUITEM( "Set radius of a special fillet at a vertex of multiline",        CM_SETMLTLINESVERTSPECFILLETRAD )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Выставить тип законцовки в вершине мультилинии", CM_SETMLTLINESVERTTIPTYPE )
#else
        MENUITEM( "Set type of a tip at a vertex of multiline",     CM_SETMLTLINESVERTTIPTYPE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Выставить направление законцовки в вершине мультилинии", CM_SETMLTLINESVERTTIPDIR )
#else
        MENUITEM( "Set direction of a tip at a vertex of multiline",        CM_SETMLTLINESVERTTIPDIR )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Выставить радиус эквидистанты мультилинии", CM_SETMLTLINESRADIUS )
#else
        MENUITEM( "Set radius of a multiline's offset",        CM_SETMLTLINESRADIUS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Изменить радиус эквидистанты мультилинии", CM_CHANGEMLTLINESRADIUS )
#else
        MENUITEM( "Change radius of multiline's offset",      CM_CHANGEMLTLINESRADIUS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Добавить радиус эквидистанты мультилинии", CM_ADDMLTLINESRADIUS )
#else
        MENUITEM( "Add radius of multiline's offset",         CM_ADDMLTLINESRADIUS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Удалить радиус эквидистанты мультилинии", CM_REMOVEMLTLINESRADIUS )
#else
        MENUITEM( "Remove radius of multiline's offset",     CM_REMOVEMLTLINESRADIUS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Выставить тип законцовки в начале мультилинии",   CM_SETMLTLINESBEGTIPTYPE )
#else
        MENUITEM( "Set type of a tip at the beginning of multiline", CM_SETMLTLINESBEGTIPTYPE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Выставить параметр законцовки в начале мультилинии",   CM_SETMLTLINESBEGTIPPARAM )
#else
        MENUITEM( "Set parameter of a tip at the beginning of multiline", CM_SETMLTLINESBEGTIPPARAM )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Выставить тип законцовки в конце мультилинии", CM_SETMLTLINESENDTIPTYPE )
#else
        MENUITEM( "Set type of a tip at the end of multiline",    CM_SETMLTLINESENDTIPTYPE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Выставить параметр законцовки в конце мультилинии", CM_SETMLTLINESENDTIPPARAM )
#else
        MENUITEM( "Set parameter of a tip at the end of multiline",    CM_SETMLTLINESENDTIPPARAM )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Выставить использование замкнутости", CM_SETMLTLINESPROCESSCLOSED )
#else
        MENUITEM( "Set flag of using the closedness",    CM_SETMLTLINESPROCESSCLOSED )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Построить скругление базовой кривой мультилинии", CM_FILLETMLTLINESBASISCURVE )
#else
        MENUITEM( "Create fillet of multiline base curve",           CM_FILLETMLTLINESBASISCURVE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Построить фаску базовой кривой мультилинии", CM_CHAMFERMLTLINESBASISCURVE )
#else
        MENUITEM( "Create chamfer of multiline base curve",     CM_CHAMFERMLTLINESBASISCURVE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Разбить мультилинию на две части", CM_BREAKMLTLINE )
#else
        MENUITEM( "Split multiline into two parts",   CM_BREAKMLTLINE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Разбить мультилинию на N частей", CM_BREAKMLTLINENPARTS )
#else
        MENUITEM( "Split multiline into N parts",    CM_BREAKMLTLINENPARTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Удалить последний сегмент мультилинии", CM_DELMLTLINESLASTSEG )
#else
        MENUITEM( "Remove the last segment of multiline",  CM_DELMLTLINESLASTSEG )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Удалить между точками", CM_DELETEPARTMLTLINEP1P2 )
#else
        MENUITEM( "Remove between points", CM_DELETEPARTMLTLINEP1P2 )
#endif
      ENDPOPUP
      SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Произвольный объект" )
#else
      POPUP( "Arbitrary object" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Переместить", CM_MOVEOBJECT )
#else
        MENUITEM( "Move",        CM_MOVEOBJECT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Повернуть в плоскости", CM_PLANEROTATEOBJECT )
#else
        MENUITEM( "Rotate on the plane",   CM_PLANEROTATEOBJECT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Повернуть вокруг линии", CM_LINEROTATEOBJECT )
#else
        MENUITEM( "Rotate about a line",    CM_LINEROTATEOBJECT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Масштабировать", CM_SCALEOBJECT )
#else
        MENUITEM( "Scale",          CM_SCALEOBJECT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Удалить", CM_DELETEOBJECT )
#else
        MENUITEM( "Delete",  CM_DELETEOBJECT )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Выделенные объекты" )
#else
      POPUP( "Selected objects" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Переместить", CM_EDITMOVE )
#else
        MENUITEM( "Move",        CM_EDITMOVE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Повернуть в плоскости зкрана", CM_EDITPLANEROTATE )
#else
        MENUITEM( "Rotate on the screen plane",   CM_EDITPLANEROTATE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Повернуть вокруг линии", CM_EDITLINEROTATE )
#else
        MENUITEM( "Rotate about a line",    CM_EDITLINEROTATE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Масштабировать", CM_EDITSCALESELECTED )
#else
        MENUITEM( "Scale",          CM_EDITSCALESELECTED )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Удалить", CM_DELETESELECTED )
#else
        MENUITEM( "Delete",  CM_DELETESELECTED )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Объекты на плоскости" )
#else
      POPUP( "Objects on the plane" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Переместить", CM_MOVECPLANE )
#else
        MENUITEM( "Move",        CM_MOVECPLANE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Повернуть в плоскость окна",        CM_ROTATEPLANEINVIEW )
#else
        MENUITEM( "Rotate to the plane of the window", CM_ROTATEPLANEINVIEW )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Удалить", CM_DELETECONSTRUCTIVEPLANE )
#else
        MENUITEM( "Delete",  CM_DELETECONSTRUCTIVEPLANE )
#endif
      ENDPOPUP
    ENDPOPUP
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined ( __NATIVE_LANGUAGE__ )
    POPUP( "Выделить" )
#else
    POPUP( "Select" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Все объекты" )
#else
      POPUP( "All objects" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Да",         CM_EDITSELECTALL )
#else
        MENUITEM( "Select all", CM_EDITSELECTALL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Нет",          CM_EDITUNSELECTALL )
#else
        MENUITEM( "Unselect all", CM_EDITUNSELECTALL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Инвертировать", CM_EDITREVERSSELECTALL )
#else
        MENUITEM( "Inverse",       CM_EDITREVERSSELECTALL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Конструктивной плоскости", CM_SELECTBYCPLANE )
#else
        MENUITEM( "Of constructive plane",    CM_SELECTBYCPLANE )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Точки" )
#else
      POPUP( "Points" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Все", CM_SELECTALLPOINTS )
#else
        MENUITEM( "All", CM_SELECTALLPOINTS )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Кривые" )
#else
      POPUP( "Curves" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Все", CM_SELECTALLCURVES )
#else
        MENUITEM( "All", CM_SELECTALLCURVES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Прямые", CM_SELECTLINE )
#else
        MENUITEM( "Lines",  CM_SELECTLINE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Сплайны", CM_SELECTPOLYCURVE )
#else
        MENUITEM( "Splines", CM_SELECTPOLYCURVE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Пересечения",   CM_SELECTINTERSECTIONCURVE )
#else
        MENUITEM( "Intersections", CM_SELECTINTERSECTIONCURVE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Кривые двумерные",         CM_SELECTCONTOURS2D )
#else
        MENUITEM( "Two-dimensional curves",  CM_SELECTCONTOURS2D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Кривые трехмерные",         CM_SELECTCONTOURS3D )
#else
        MENUITEM( "Three-dimensional curves", CM_SELECTCONTOURS3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Кривые на поверхности", CM_SELECTCONTOURSONSURFACE )
#else
        MENUITEM( "Curves on a surface",  CM_SELECTCONTOURSONSURFACE )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Поверхности" )
#else
      POPUP( "Surfaces" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Все", CM_SELECTALLSURFACES )
#else
        MENUITEM( "All", CM_SELECTALLSURFACES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Плоскости", CM_SELECTPLANE )
#else
        MENUITEM( "Planes",    CM_SELECTPLANE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Элементарные", CM_SELECTELEMENTARYSURFACE )
#else
        MENUITEM( "Elementary",   CM_SELECTELEMENTARYSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Движения", CM_SELECTSWEPTSURFACE )
#else
        MENUITEM( "Swept",    CM_SELECTSWEPTSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Сплайновые", CM_SELECTPOLYSURFACE )
#else
        MENUITEM( "Spline",     CM_SELECTPOLYSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Эквидистантные", CM_SELECTOFFSETSURFACE )
#else
        MENUITEM( "Offset",         CM_SELECTOFFSETSURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Усеченные кривыми",   CM_SELECTCURVEBOUNDEDSURFACE )
#else
        MENUITEM( "Truncated by curves", CM_SELECTCURVEBOUNDEDSURFACE )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Тела" )
#else
      POPUP( "Solids" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Все",  CM_SELECT_ALL_SOLIDS )
#else
        MENUITEM( "All",  CM_SELECT_ALL_SOLIDS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Все замкнутые",  CM_SELECT_CLOSED_SOLIDS )
#else
        MENUITEM( "All closed",     CM_SELECT_CLOSED_SOLIDS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Все незамкнутые", CM_SELECT_UNCLOSED_SOLIDS )
#else
        MENUITEM( "All open",        CM_SELECT_UNCLOSED_SOLIDS )
#endif
      ENDPOPUP
    ENDPOPUP
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined ( __NATIVE_LANGUAGE__ )
    POPUP( "Выполнить" )
#else
    POPUP( "Perform" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      MENUITEM( "Резку объектов",     CM_CUTMODELOBJECTS )
#else
      MENUITEM( "Cutting of objects", CM_CUTMODELOBJECTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      MENUITEM( "Перерисовку объектов", CM_EDITREDRAW )
#else
      MENUITEM( "Redrawing of objects", CM_EDITREDRAW )
#endif
      SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Построение" )
#else
      POPUP( "Construction" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Габарита" )
#else
        POPUP( "Bounding box" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Кривой",  CM_CURVEGABARIT )
#else
          MENUITEM( "Curve",   CM_CURVEGABARIT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Поверхности",  CM_SURFACEGABARIT )
#else
          MENUITEM( "Surface",      CM_SURFACEGABARIT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Грани тела",    CM_FACEGABARIT )
#else
          MENUITEM( "Solid's face",  CM_FACEGABARIT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Твердого тела", CM_SOLIDGABARIT )
#else
          MENUITEM( "Solid",         CM_SOLIDGABARIT )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Габарита в локальной системе координат" )
#else
        POPUP( "Bounding box in the local coordinate system" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "2D-Кривой",  CM_LOCALGABARIT_CURVE2D )
#else
          MENUITEM( "2D curve",   CM_LOCALGABARIT_CURVE2D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "3D-Кривой",  CM_LOCALGABARIT_CURVE3D )
#else
          MENUITEM( "3D curve",   CM_LOCALGABARIT_CURVE3D )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Поверхности", CM_LOCALGABARIT_SURFACE )
#else
          MENUITEM( "Surface",     CM_LOCALGABARIT_SURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Ребра тела",    CM_LOCALGABARIT_EDGE )
#else
          MENUITEM( "Solid's edge",  CM_LOCALGABARIT_EDGE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Грани тела",    CM_LOCALGABARIT_FACE )
#else
          MENUITEM( "Solid's face",  CM_LOCALGABARIT_FACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Твердого тела", CM_LOCALGABARIT_SOLID )
#else
          MENUITEM( "Solid",         CM_LOCALGABARIT_SOLID )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Характерных точек и векторов" )
#else
        POPUP( "Control points and vectors" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Нормалей поверхности", CM_SURF_NORMALS )
#else
          MENUITEM( "Surface normals",      CM_SURF_NORMALS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Нормалей грани",  CM_FACE_NORMALS )
#else
          MENUITEM( "Face normals",    CM_FACE_NORMALS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Нормалей сеток", CM_MESH_NORMALS )
#else
          MENUITEM( "Facets normals", CM_MESH_NORMALS )
#endif


        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Очерка" )
#else
        POPUP( "Silhouette" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Поверхности", CM_SURFACEHIDE )
#else
          MENUITEM( "Surface",     CM_SURFACEHIDE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Грани", CM_FACEHIDE )
#else
          MENUITEM( "Face",  CM_FACEHIDE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Твердого тела", CM_SOLIDHIDE )
#else
          MENUITEM( "Solid",         CM_SOLIDHIDE )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Видимых линий" )
#else
        POPUP( "Visible lines" )
#endif

        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Триангуляции" )
#else
        POPUP( "Triangulation" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Поверхности", CM_TRIANGULATESURFACE )
#else
          MENUITEM( "Surface",     CM_TRIANGULATESURFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Твердого тела", CM_TRIANGULATESOLID )
#else
          MENUITEM( "Solid",         CM_TRIANGULATESOLID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Сборки тел", CM_TRIANGULATESOLIDS )
#else
          MENUITEM( "Assembly",   CM_TRIANGULATESOLIDS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Грани тела",   CM_TRIANGULATEFACE )
#else
          MENUITEM( "Solid's face", CM_TRIANGULATEFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Региона", CM_TRIANGULAREREGION )
#else
          MENUITEM( "Region",  CM_TRIANGULAREREGION )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Упрощения триангуляции" )
#else
        POPUP( "Simplified triangulation" )
#endif

        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Развертки" )
#else
        POPUP( "Unfold" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Поверхностной кривой", CM_CURVEMAP )
#else
          MENUITEM( "Curve on a surface",   CM_CURVEMAP )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Поверхности", CM_SURFACEMAP )
#else
          MENUITEM( "Surface",     CM_SURFACEMAP )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Граней твердого тела", CM_SOLIDMAP )
#else
          MENUITEM( "Solid's faces",        CM_SOLIDMAP )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Грани тела",   CM_SOLIDFACEMAP )
#else
          MENUITEM( "Solid's face", CM_SOLIDFACEMAP )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Триангуляции грани тела", CM_PARAMTRIANGULATEFACE )
#else
          MENUITEM( "Solid's triangulation",   CM_PARAMTRIANGULATEFACE )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Характеристических линий" )
#else
        POPUP( "Characteristic lines" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Слайновой кривой", CM_CURVETESTA )
#else
          MENUITEM( "Spline curve",     CM_CURVETESTA )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Слайновой поверхности", CM_SURFACETESTA )
#else
          MENUITEM( "Spline surface",        CM_SURFACETESTA )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Каркаса объекта" )
#else
        POPUP( "Frame of object" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Пространственного", CM_MAKESPACEWIREFRAME )
#else
          MENUITEM( "Spatial",           CM_MAKESPACEWIREFRAME )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Плоского", CM_MAKEPLANEWIREFRAME )
#else
          MENUITEM( "Planar",   CM_MAKEPLANEWIREFRAME )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "На плоскости", CM_MAKEWIREFRAMEONPLANE )
#else
          MENUITEM( "On plane",     CM_MAKEWIREFRAMEONPLANE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Точке триангуляции",  CM_MAKETRIANGLEPOINTFRAME )
#else
          MENUITEM( "Triangulation points", CM_MAKETRIANGLEPOINTFRAME )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Линий триангуляции",  CM_MAKETRIANGLEWIREFRAME )
#else
          MENUITEM( "Triangulation lines", CM_MAKETRIANGLEWIREFRAME )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Пластин триангуляции", CM_MAKETRIANGLEPLANE )
#else
          MENUITEM( "Triangulation plates", CM_MAKETRIANGLEPLANE )
#endif
        ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Графика" )
#else
        POPUP( "Plot" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Кривизны кривой",  CM_CURVECURVATURE )
#else
          MENUITEM( "Curve curvature",  CM_CURVECURVATURE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Кручения кривой",  CM_CURVETORSION )
#else
          MENUITEM( "Curve torsion",    CM_CURVETORSION )
#endif
        ENDPOPUP
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Расчет" )
#else
      POPUP( "Calculation" )
#endif

#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Дерева объемов в детекторе столкновений", CM_COLLISION_CONSTRUCT_BVT_NODES )
#else
        MENUITEM( "Collision detector hierarchical volume tree", CM_COLLISION_CONSTRUCT_BVT_NODES )
#endif

#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Расстояния в детекторе столкновений", CM_COLLISION_DISTANCE_QUERY )
#else
        MENUITEM( "Collision detector distance query", CM_COLLISION_DISTANCE_QUERY )
#endif

#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Столкновений для тел", CM_COLLISION_INTERFERENCE_SOLIDS )
#else
        MENUITEM( "Collision detector query for solids", CM_COLLISION_INTERFERENCE_SOLIDS )
#endif

#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Столкновений для сеток", CM_COLLISION_INTERFERENCE_MESHES )
#else
        MENUITEM( "Collision detector query for meshes", CM_COLLISION_INTERFERENCE_MESHES )
#endif

#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Пересечения посредством булевой операции", CM_CROSS_BOOLEAN )
#else
        MENUITEM( "Intersection via boolean operation", CM_CROSS_BOOLEAN )
#endif

#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Соприкасающихся граней", CM_FIND_TOUCHED_FACES )
#else
        MENUITEM( "Touching faces", CM_FIND_TOUCHED_FACES )
#endif

#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Разбиения контактирующих граней тел", CM_SPLIT_TOUCHED_FACES )
#else
        MENUITEM( "Splitting contacting faces of solids", CM_SPLIT_TOUCHED_FACES )
#endif

#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Объединения тел с контактирующими гранями", CM_MERGE_SOLIDS )
#else
        MENUITEM( "Merging bodies with contacting faces", CM_MERGE_SOLIDS )
#endif

#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Инерционных характеристик тела", CM_GETSOLIDMASSINERTIA )
#else
        MENUITEM( "Inertial properties of solid", CM_GETSOLIDMASSINERTIA )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Инерционных характеристик сечения",  CM_GETCONTOURAREA )
#else
        MENUITEM( "Section inertial properties",        CM_GETCONTOURAREA )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Длины кривой",  CM_GETCURVELENGTH )
#else
        MENUITEM( "Curve length",  CM_GETCURVELENGTH )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Кривизны кривой",  CM_GETCURVERADIUS )
#else
        MENUITEM( "Curve curvature",  CM_GETCURVERADIUS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Угла между гранями ребра",                  CM_GETEDGEFACESANGLE )
#else
        MENUITEM( "Angle between faces incident to the edge",  CM_GETEDGEFACESANGLE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Угла между двумя ребрами",                  CM_GETEDGEEDGEANGLE )
#else
        MENUITEM( "Angle between two edges",                   CM_GETEDGEEDGEANGLE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Расстояния от точки до объекта",       CM_POINTDISTANCETOPOINT )
#else
        MENUITEM( "Distance from a point to the object",  CM_POINTDISTANCETOPOINT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Расстояния от вершины до тела",        CM_GETDISTANCETOPOINT )
#else
        MENUITEM( "Distance from a vertex to the solid",  CM_GETDISTANCETOPOINT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Мин. расстояния между геом. объектами",      CM_GEOMITEMS_MINDISTANCE )
#else
        MENUITEM( "Min. distance between geometrical objects",  CM_GEOMITEMS_MINDISTANCE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Мин. расстояния между топ. объектами",       CM_TOPOITEMS_MINDISTANCE )
#else
        MENUITEM( "Min. distance between topological objects",  CM_TOPOITEMS_MINDISTANCE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Расстояния между осями поверхностей",  CM_SURFAXESDISTANCE )
#else
        MENUITEM( "Distance between axes of surfaces",    CM_SURFAXESDISTANCE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Макс. расстояния между геом. объектами",     CM_GEOMITEMS_MAXDISTANCE )
#else
        MENUITEM( "Max. distance between geometrical objects",  CM_GEOMITEMS_MAXDISTANCE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Макс. расстояния между топ. объектами",      CM_TOPOITEMS_MAXDISTANCE )
#else
        MENUITEM( "Max. distance between topological objects",  CM_TOPOITEMS_MAXDISTANCE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Расстояния между поверхностями по сетке",  CM_GRIDDED_SURF_MINMAXDISTANCES )
#else
        MENUITEM( "Distance between surfaces by the grid",    CM_GRIDDED_SURF_MINMAXDISTANCES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Max_min нормальные кривизны поверхности",  CM_SURF_MAX_MIN_CURVATURES )
#else
        MENUITEM( "Max_min normal curvatures of surface",     CM_SURF_MAX_MIN_CURVATURES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Max_min нормальные кривизны граней",  CM_FACES_MAX_MIN_CURVATURES )
#else
        MENUITEM( "Max_min normal curvatures of faces",  CM_FACES_MAX_MIN_CURVATURES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Центра поверхности грани",  CM_GETFACECENTRE )
#else
        MENUITEM( "Face center",               CM_GETFACECENTRE )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Действие" )
#else
      POPUP( "Action" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Проверить геометрию оболочки",  CM_CHECK_SHELL_GEOMETRY )
#else
        MENUITEM( "Check geometry of a shell",     CM_CHECK_SHELL_GEOMETRY )
#endif
        SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Проверить именование оболочки",  CM_CHECK_SHELL_NAMES )
#else
        MENUITEM( "Check names of a shell",         CM_CHECK_SHELL_NAMES )
#endif
        SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Проверить поверхности-подложки",  CM_CHECKIDENTICALBASESURFACES )
#else
        MENUITEM( "Check the base surfaces",         CM_CHECKIDENTICALBASESURFACES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Проверка тел на пересечение",    CM_CHECK_SOLIDS_INTERSECT )
#else
        MENUITEM( "Check bodies for intersection",  CM_CHECK_SOLIDS_INTERSECT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Проверка абрис триангуляций тел на пересечение", CM_CHECK_CONVEX_HULL_INTERSECT )
#else
        MENUITEM( "Check convex hulls for intersection",            CM_CHECK_CONVEX_HULL_INTERSECT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Найти подобные грани с наложением",     CM_FIND_OVERLAPPING_SIMILAR_FACES )
#else
        MENUITEM( "Find similar faces with overlapping",   CM_FIND_OVERLAPPING_SIMILAR_FACES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Найти грани - потенциальные носители ребер",  CM_FIND_FACES_EDGES_CARRIERS )
#else
        MENUITEM( "Find faces - potential owners of edges",      CM_FIND_FACES_EDGES_CARRIERS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Разрушить все сборки",       CM_DECOMPOSE_ALL_ASSEMBLIES )
#else
        MENUITEM( "Decompose all assemblies",   CM_DECOMPOSE_ALL_ASSEMBLIES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Преобразовать объект в полигоны",   CM_CONVERT_TO_MESH )
#else
        MENUITEM( "Convert an object to the mesh",     CM_CONVERT_TO_MESH )
#endif
        SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Выставить последнюю версию",  CM_SET_SOLID_LAST_VERSION )
#else
        MENUITEM( "Set the last version",        CM_SET_SOLID_LAST_VERSION )
#endif
        SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Создать кривые по ребрам вершины",  CM_ADD_VERTEXEDGES_AS_CURVES )
#else
        MENUITEM( "Create curves by vertex edges",     CM_ADD_VERTEXEDGES_AS_CURVES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Создать кривые по всем ребрам",  CM_ADD_SOLIDEDGES_AS_CURVES )
#else
        MENUITEM( "Create curves by all edges",     CM_ADD_SOLIDEDGES_AS_CURVES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Создать точки по всем вершинам",   CM_ADD_SOLIDVERTS_AS_POINTS )
#else
        MENUITEM( "Create points by all vertices",    CM_ADD_SOLIDVERTS_AS_POINTS )
#endif
/* KYA M15 Комментарю отладочные пункты
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Создать спиральные поверхности по сплайновым",  CM_RECOGNITIONSPIRALSURFACES )
#else
        MENUITEM( "Create spiral surfaces by spline surfaces",     CM_RECOGNITIONSPIRALSURFACES )
#endif
        SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Скорости работы кривой 2D",      CM_CHECK_CURVE2D_SPEED )
#else
        MENUITEM( "Calculation speed of 2D curve",  CM_CHECK_CURVE2D_SPEED )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Скорости работы кривой 3D",      CM_CHECK_CURVE3D_SPEED )
#else
        MENUITEM( "Calculation speed of 3D curve",  CM_CHECK_CURVE3D_SPEED )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Скорости работы поверхности",  CM_CHECK_SURFACE_SPEED )
#else
        MENUITEM( "Speed of surface",             CM_CHECK_SURFACE_SPEED )
#endif
*/
        SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Операционная разбивка поверхности",   CM_CREATE_SURFACE_OPER_TESS )
#else
        MENUITEM( "Operational tesselation of surface",  CM_CREATE_SURFACE_OPER_TESS )
#endif
        SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Проверить касание кривых",  CM_CHECK_CURVES_TANGENT_MATING )
#else
        MENUITEM( "Check curve for tangency",  CM_CHECK_CURVES_TANGENT_MATING )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Проверить кривую 2D на самопересечение",  CM_CHECK_CURVE2D_SELFINT )
#else
        MENUITEM( "Check 2D curve for self-intersection",    CM_CHECK_CURVE2D_SELFINT )
#endif
        SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Показать направление кривой",  CM_SHOW_CURVE_DIRECTION )
#else
        MENUITEM( "Show curve direction",         CM_SHOW_CURVE_DIRECTION )
#endif
        SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Общее тестирование сопряжений",  CM_COMMON_MATES_TEST )
#else
        MENUITEM( "Common testing of matings",      CM_COMMON_MATES_TEST )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Сборка из жестко сопряженных кирпичей",  CM_MATE_CUBE )
#else
        MENUITEM( "Assembly from rigidly mated bricks",     CM_MATE_CUBE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Фрактальное сопряжение",  CM_MATE_FRACTAL )
#else
        MENUITEM( "Fractal mating",          CM_MATE_FRACTAL )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Сопряжения с распределенными степенями свободы",  CM_MATE_DISTRIBUTED_DOF )
#else
        MENUITEM( "Matings with distributed degrees of freedom",     CM_MATE_DISTRIBUTED_DOF )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Линейный массив компонент",  CM_LINEAR_PATTERN )
#else
        MENUITEM( "Linear pattern",     CM_LINEAR_PATTERN )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Угловой массив компонент",  CM_ANGULAR_PATTERN )
#else
        MENUITEM( "Angular pattern",     CM_ANGULAR_PATTERN )
#endif

      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Определение" )
#else
      POPUP( "Definition" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Номера ребра в грани",   CM_GETEDGENUMBER_INFACE )
#else
        MENUITEM( "Number of an edge in face",   CM_GETEDGENUMBER_INFACE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Номера грани в теле",    CM_GETFACENUMBER )
#else
        MENUITEM( "Number of a face in solid",    CM_GETFACENUMBER )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Номера ребра в теле",    CM_GETEDGENUMBER )
#else
        MENUITEM( "Number of an edge in solid",    CM_GETEDGENUMBER )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Грани по номеру в теле", CM_GETFACEBYNUMBER )
#else
        MENUITEM( "Faces by a number in solid", CM_GETFACEBYNUMBER )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Ребра по номеру в теле", CM_GETEDGEBYNUMBER )
#else
        MENUITEM( "Edges by a number in solid", CM_GETEDGEBYNUMBER )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Слайд" )
#else
      POPUP( "Slide" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Всех объектов", CM_RENDERING )
#else
        MENUITEM( "All objects", CM_RENDERING )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Поверхностей", CM_SURFACERENDERING )
#else
        MENUITEM( "Surfaces", CM_SURFACERENDERING )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Твердых тел", CM_SOLIDRENDERING )
#else
        MENUITEM( "Solid bodies", CM_SOLIDRENDERING )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Графика перестроения тела", CM_MANUFACTURING )
#else
        MENUITEM( "Diagram of solid rebuilding", CM_MANUFACTURING )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Расположение" )
#else
      POPUP( "Position" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "По вектору взгляда", CM_SETVIEWVECTOR )
#else
        MENUITEM( "By view vector",     CM_SETVIEWVECTOR )
#endif
      ENDPOPUP
      SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Вывод в файл для печати" )
#else
      POPUP( "Output to file for print" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Векторный вид", CM_PRINTPLOTT )
#else
        MENUITEM( "Plot",          CM_PRINTPLOTT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Растровый вид", CM_PRINTBITMAP )
#else
        MENUITEM( "Bitmap",        CM_PRINTBITMAP )
#endif
      ENDPOPUP
    ENDPOPUP
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined ( __NATIVE_LANGUAGE__ )
    POPUP( "&Hастройка" )
#else
    POPUP( "&Setup" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Цвет" )
#else
      POPUP( "Color" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Объекта", CM_SETOBJECTCOLOR )
#else
        MENUITEM( "Object",  CM_SETOBJECTCOLOR )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Зависимости", CM_SETCONNECTIONCOLOR )
#else
        MENUITEM( "Connection",  CM_SETCONNECTIONCOLOR )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Выделенных объектов", CM_SETCOLORSELECTED )
#else
        MENUITEM( "Selected object",     CM_SETCOLORSELECTED )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Выделенных зависимостей", CM_SETCOLORSELECTEDCONN )
#else
        MENUITEM( "Selected connections",    CM_SETCOLORSELECTEDCONN )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Рабочих окон",       CM_SETBACKGRCOLOR )
#else
        MENUITEM( "Windows background", CM_SETBACKGRCOLOR )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Закраски объекта", CM_GLRENDERCOLOR )
#else
        MENUITEM( "Object rendering", CM_GLRENDERCOLOR )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Выделенный", CM_SETSELECTCOLOR )
#else
        MENUITEM( "Selected",   CM_SETSELECTCOLOR )
#endif
      ENDPOPUP

#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Настройки рисования" )
#else
      POPUP( "Drawing properties" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Способ отображения OGL", CM_SET_OGL_VIEWTYPE )
#else
        MENUITEM( "Type of OpenGL view",    CM_SET_OGL_VIEWTYPE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Прогиба линий при отображении", CM_SETVISUALSAG )
#else
        MENUITEM( "Sag of lines while visualization", CM_SETVISUALSAG )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Толщина линий рисования", CM_SETSTYLEWIDTH )
#else
        MENUITEM( "Line width for draw",    CM_SETSTYLEWIDTH )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Атрибут объекта `Толщина линий`", CM_SETOBJECTWIDTH )
#else
        MENUITEM( "Object attribute `Line width`",     CM_SETOBJECTWIDTH )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Количество u-линий и v-линий", CM_UVMESHCOUNT )
#else
        MENUITEM( "Count of u-line and v-line",   CM_UVMESHCOUNT )
#endif
      ENDPOPUP

#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Оптические свойства" )
#else
      POPUP( "Visual properties" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Материал объекта", CM_GLPROPERT )
#else
        MENUITEM( "Object material",  CM_GLPROPERT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Фон объекта",       CM_GLAMBIENT )
#else
        MENUITEM( "Object background", CM_GLAMBIENT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Диффузия",  CM_GLDIFFUSE )
#else
        MENUITEM( "Diffusion", CM_GLDIFFUSE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Зеркальность", CM_GLSPECULARITY )
#else
        MENUITEM( "Specularity",  CM_GLSPECULARITY )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Блеск",     CM_GLSHININESS )
#else
        MENUITEM( "Shininess", CM_GLSHININESS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Непрозрачность", CM_GLOPACITY )
#else
        MENUITEM( "Opacity", CM_GLOPACITY )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Излучение объекта", CM_GLEMISSION )
#else
        MENUITEM( "Object emission",   CM_GLEMISSION )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Направление света", CM_SETLIGHTDIRECTION )
#else
        MENUITEM( "Light direction",   CM_SETLIGHTDIRECTION )
#endif
      ENDPOPUP

#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Видимость" )
#else
      POPUP( "Visibility" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Сделать объект невидимым",  CM_INVISIBLEOBJECT )
#else
        MENUITEM( "Make the object invisible", CM_INVISIBLEOBJECT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Показать все объекты", CM_VISIBLEALLOBJECTS )
#else
        MENUITEM( "Show all the objects", CM_VISIBLEALLOBJECTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Показать точки", CM_VISIBLEPOINTS )
#else
        MENUITEM( "Show points",    CM_VISIBLEPOINTS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Показать кривые", CM_VISIBLECURVES )
#else
        MENUITEM( "Show curves",     CM_VISIBLECURVES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Показать поверхности", CM_VISIBLESURFACES )
#else
        MENUITEM( "Show surfaces",        CM_VISIBLESURFACES )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Показать тела", CM_VISIBLEBODIES )
#else
        MENUITEM( "Show bodies",   CM_VISIBLEBODIES )
#endif
      ENDPOPUP

#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Параметры" )
#else
      POPUP( "Options" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Рассчитывать триангуляцию модели", CM_SETMAKEGRID )
#else
        MENUITEM( "Calculate model triangulation", CM_SETMAKEGRID )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Слияние элементов", CM_ITEMSMERGING )
#else
        MENUITEM( "Items merging", CM_ITEMSMERGING )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Не копировать строители при редактировании", CM_EDIT_SAME_CREATORS )
#else
        MENUITEM( "Don't copy builders when editing", CM_EDIT_SAME_CREATORS )
#endif

#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Величина" )
#else
        POPUP( "Value" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Углового шага при вычислениях", CM_SETDEVIATIONSAG )
#else
          MENUITEM( "Angular spacing while calculation", CM_SETDEVIATIONSAG )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Шаг смещения изображения", CM_DELTA_RANGE )
#else
          MENUITEM( "Step of image shift", CM_DELTA_RANGE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Шаг поворота изображения", CM_DELTA_ANGLE )
#else
          MENUITEM( "Step of image rotation", CM_DELTA_ANGLE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Шага курсора", CM_SETCURSORSTEP )
#else
          MENUITEM( "Cursor step", CM_SETCURSORSTEP )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Печатать файл замеров времени", CM_REBUILDTIMERESULTFILE )
#else
          MENUITEM( "Print the file of time measurements", CM_REBUILDTIMERESULTFILE )
#endif
        ENDPOPUP

#if defined ( __NATIVE_LANGUAGE__ )
        POPUP( "Порядок NURBS" )
#else
        POPUP( "Degree of NURBS" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Создаваемой кривой", CM_SETCURVEDEGREE )
#else
          MENUITEM( "Created curve", CM_SETCURVEDEGREE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
          MENUITEM( "Создаваемой поверхности", CM_SETSURFACEDEGREE )
#else
          MENUITEM( "Created surface", CM_SETSURFACEDEGREE )
#endif
        ENDPOPUP

      ENDPOPUP

#ifdef C3D_DEBUG
#if defined ( __NATIVE_LANGUAGE__ )
      MENUITEM( "Пропускать ASSERT-ы", CM_SKIP_ASSERTS )
#else
      MENUITEM( "Skip ASSERTs",        CM_SKIP_ASSERTS )
#endif
  
#if defined ( __NATIVE_LANGUAGE__ )
      MENUITEM( "Отладочная отрисовка",       CM_SWITCH_DEBUG_DRAW )
#else
      MENUITEM( "Debug draw",        CM_SWITCH_DEBUG_DRAW )
#endif
  
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      MENUITEM( "Многопоточность",       CM_SWITCH_MULTITHREADING )
#else
      MENUITEM( "Multithreading",        CM_SWITCH_MULTITHREADING )
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      MENUITEM( "Перечитать файл конфигурации",       CM_APPLY_CONFIG_FILE )
#else
      MENUITEM( "Udpate setting from config file",        CM_APPLY_CONFIG_FILE )
#endif

    ENDPOPUP

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined ( __NATIVE_LANGUAGE__ )
    POPUP( "&Oкно" )
#else
    POPUP( "&Window" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Ориентировать" )
#else
      POPUP( "Set" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Изометрия", CM_VIEWISOPROJ )
#else
        MENUITEM( "Isometry", CM_VIEWISOPROJ )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Вид спереди", CM_VIEWFRONTPROJ )
#else
        MENUITEM( "Front view", CM_VIEWFRONTPROJ )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Вид сзади", CM_VIEWREARPROJ )
#else
        MENUITEM( "Rear view", CM_VIEWREARPROJ )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Вид сверху", CM_VIEWUPPROJ )
#else
        MENUITEM( "Top view", CM_VIEWUPPROJ )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Вид снизу", CM_VIEWDOWNPROJ )
#else
        MENUITEM( "Bottom view", CM_VIEWDOWNPROJ )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Вид слева", CM_VIEWLEFTPROJ )
#else
        MENUITEM( "Left-side view", CM_VIEWLEFTPROJ )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Вид справа", CM_VIEWRIGHTPROJ )
#else
        MENUITEM( "Right-side view", CM_VIEWRIGHTPROJ )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Расположить" )
#else
      POPUP( "Arrange" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM_QT( "Мозаикой", mdiArea, tileSubWindows )
#else
        MENUITEM_QT( "Tile", mdiArea, tileSubWindows )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM_QT( "Каскадом", mdiArea, cascadeSubWindows )
#else
        MENUITEM_QT( "Cascade", mdiArea, cascadeSubWindows )
#endif
/* Не реализовано.
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Последовательно", CM_WINDOWARRANGE )
#else
        MENUITEM( "Sequentially", CM_WINDOWARRANGE )
#endif
*/
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Изменить" )
#else
      POPUP( "Change" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Совместить с объектом", CM_SETVIEWPLACEMENT )
#else
        MENUITEM( "Match with object",     CM_SETVIEWPLACEMENT )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Ориентацию",  CM_SETPROJECTION )
#else
        MENUITEM( "Orientation", CM_SETPROJECTION )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Глубину", CM_SETDEPTH )
#else
        MENUITEM( "Depth",   CM_SETDEPTH )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Масштаб", CM_CHANGESCALE )
#else
        MENUITEM( "Scale",   CM_CHANGESCALE )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Точку наблюдения", CM_SETVIEWPOINT )
#else
        MENUITEM( "View point",       CM_SETVIEWPOINT )
#endif
      ENDPOPUP
#if defined ( __NATIVE_LANGUAGE__ )
      POPUP( "Закрыть" )
#else
      POPUP( "Close" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      MENUITEM_QT( "Активное окно", mdiArea, closeActiveSubWindow )
#else
      MENUITEM_QT( "Active window", mdiArea, closeActiveSubWindow )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      MENUITEM_QT( "Все окна", mdiArea, closeAllSubWindows )
#else
      MENUITEM_QT( "All windows", mdiArea, closeActiveSubWindow )
#endif
      ENDPOPUP
    ENDPOPUP
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined ( __NATIVE_LANGUAGE__ )
    POPUP( "&Помощь" )
#else
    POPUP( "&Help" )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      MENUITEM( "Список горячих клавиш", CM_HELPABOUT )
#else
      MENUITEM( "List of hot keys",      CM_HELPABOUT )
#endif
      SEPARATOR
      POPUP_IMP( "Создание новых команд", "Creation of new commands" )

        MENUITEM_IMP( CM_MAKEUSERCOMAND0, "Пользовательская команда 0", "User command 0" )       
        MENUITEM_IMP( CM_MAKEUSERCOMAND1, "Пользовательская команда 1", "User command 1" )

#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Пользовательская команда 2", CM_MAKEUSERCOMAND2 )
#else
        MENUITEM( "User command 2",             CM_MAKEUSERCOMAND2 )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Пользовательская команда 3", CM_MAKEUSERCOMAND3 )
#else
        MENUITEM( "User command 3",             CM_MAKEUSERCOMAND3 )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Пользовательская команда 4", CM_MAKEUSERCOMAND4 )
#else
        MENUITEM( "User command 4",             CM_MAKEUSERCOMAND4 )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Пользовательская команда 5", CM_MAKEUSERCOMAND5 )
#else
        MENUITEM( "User command 5",             CM_MAKEUSERCOMAND5 )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Пользовательская команда 6", CM_MAKEUSERCOMAND6 )
#else
        MENUITEM( "User command 6",             CM_MAKEUSERCOMAND6 )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Пользовательская команда 7", CM_MAKEUSERCOMAND7 )
#else
        MENUITEM( "User command 7",             CM_MAKEUSERCOMAND7 )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Пользовательская команда 8", CM_MAKEUSERCOMAND8 )
#else
        MENUITEM( "User command 8",             CM_MAKEUSERCOMAND8 )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
        MENUITEM( "Пользовательская команда 9", CM_MAKEUSERCOMAND9 )
#else
        MENUITEM( "User command 9",             CM_MAKEUSERCOMAND9 )
#endif
      ENDPOPUP

      // Раздел меню с коллекцие примеров. 
      SEPARATOR

      POPUP_IMP( "Примеры", "Samples" )

        MENUITEM_IMP( CM_SAMPLE_PARAMETRIC_SKETCH, "Параметрический эскиз", "Parametric sketch" )        
        
        MENUITEM_IMP( CM_SAMPLE_WRITE_SKETCH, "Запись параметрического эскиза", "Write a parametric sketch" )
        
        MENUITEM_IMP( CM_SAMPLE_READ_SKETCH, "Чтение параметрического эскиза", "Read a parametric sketch" )
        
        MENUITEM_IMP( CM_SAMPLE_SPINNING_BLOCK, "Угловое сопряжение", "Angular mate" )
        
        MENUITEM_IMP( CM_SAMPLE_DODECAHEDRON, "Сборка додекаэдра", "Dodecahedron assembly " )

        MENUITEM_IMP( CM_SAMPLE_COLLISION_DETECTION, "Пример поиска соударений", "Collision detection demo" )

        MENUITEM_IMP( CM_SAMPLE_CHECK_COLLISIONS, "Поиск покомпонентных соударений", "Detect componentwise collisions" )

        MENUITEM_IMP( CM_SAMPLE_CHECK_COLLISIONS_GROUPS, "Пример поиска соударений между группами объектов", "Detect collisions between groups of objects" )

      ENDPOPUP

      SEPARATOR
#if defined ( __NATIVE_LANGUAGE__ )
      MENUITEM( "Лицензионный ключ", CM_ENTERLICENSEKEY )
#else
      MENUITEM( "License Key",    CM_ENTERLICENSEKEY )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      MENUITEM( "Статус лицензии", CM_LICENSE_STATUS )
#else
      MENUITEM( "License status", CM_LICENSE_STATUS )
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      MENUITEM( "Деактивировать все лицензии", CM_DISABLELICENSES )
#else
      MENUITEM( "Disable all licenses", CM_DISABLELICENSES )
#endif
    ENDPOPUP
  ENDMENU

}

//------------------------------------------------------------------------------
//
// ---
void MainWindow::createStandardToolBar()
{
  QToolBar* standardBar = new QToolBar(tr("StandardBar"), this);
  standardBar->setAllowedAreas(Qt::AllToolBarAreas);
  standardBar->setIconSize(QSize(16, 16));
  standardBar->addAction( m_actNewDocument );
  standardBar->addAction( m_actOpenDocument );
  standardBar->addAction( m_actSaveDocument );
  standardBar->addSeparator();
  standardBar->addSeparator();
  standardBar->addAction( m_actRunUserCommand0 );
  standardBar->addSeparator();
  standardBar->addAction( m_actRunUserCommand1 );
  standardBar->addAction( m_actRunUserCommand2 );
  standardBar->addAction( m_actRunUserCommand3 );
  standardBar->addSeparator();
  standardBar->addAction( m_actRunUserCommand4 );
  standardBar->addAction( m_actRunUserCommand5 );
  standardBar->addAction( m_actRunUserCommand6 );
  standardBar->addSeparator();
  standardBar->addAction( m_actRunUserCommand7 );
  standardBar->addAction( m_actRunUserCommand8 );
  standardBar->addAction( m_actRunUserCommand9 );
  standardBar->addSeparator();
  standardBar->addSeparator();
  standardBar->addAction( m_actFitScale );
  standardBar->addSeparator();
  standardBar->addAction( m_actSetIsometry );
  standardBar->addAction( m_actSetFront );
  standardBar->addAction( m_actSetBack );
  standardBar->addAction( m_actSetTop );
  standardBar->addAction( m_actSetBottom );
  standardBar->addAction( m_actSetLeft );
  standardBar->addAction( m_actSetRight );
  addToolBar(standardBar);
}

//------------------------------------------------------------------------------
//
// ---
void MainWindow::Command( uint wParam ) {

  if ( MenuCommand( (uint)wParam ) ) {
    // Для команд этой группы сохранять wPrev, lPrev, cmPrev не надо
  }
  else {
    if ( CreateSpaceObjects( (uint)wParam ) ||
         CreatePlaneObjects( (uint)wParam ) ||
         MakeAll           ( (uint)wParam ) ) {
      wPrev = wParam;
      lPrev = 0;//lParam;
      cmPrev = wParam;
    }
    else { // Команда не обработанна - передаем ее child'у
      // Не реализовано, нет необходимости
      PRECONDITION( false );
    }
  }
}


//------------------------------------------------------------------------------
//
// ---
void MainWindow::slot_CM_VIEWFRONT                     ()  { Command( CM_VIEWFRONT                      ); }
void MainWindow::slot_CM_VIEWREAR                      ()  { Command( CM_VIEWREAR                       ); }
void MainWindow::slot_CM_VIEWUP                        ()  { Command( CM_VIEWUP                         ); }
void MainWindow::slot_CM_VIEWDOWN                      ()  { Command( CM_VIEWDOWN                       ); }
void MainWindow::slot_CM_VIEWLEFT                      ()  { Command( CM_VIEWLEFT                       ); }
void MainWindow::slot_CM_VIEWRIGHT                     ()  { Command( CM_VIEWRIGHT                      ); }
void MainWindow::slot_CM_VIEWISO                       ()  { Command( CM_VIEWISO                        ); }
void MainWindow::slot_CM_FILEOPEN                      ()  { Command( CM_FILEOPEN                       ); }
void MainWindow::slot_CM_FILESAVE                      ()  { Command( CM_FILESAVE                       ); }
void MainWindow::slot_CM_FILEADD                       ()  { Command( CM_FILEADD                        ); }
void MainWindow::slot_CM_FILENEW                       ()  { Command( CM_FILENEW                        ); }
void MainWindow::slot_CM_FILETAKE                      ()  { Command( CM_FILETAKE                       ); }
void MainWindow::slot_CM_SAT_READ                      ()  { Command( CM_SAT_READ                       ); }
void MainWindow::slot_CM_IGES_READ                     ()  { Command( CM_IGES_READ                      ); }
void MainWindow::slot_CM_XT_READ                       ()  { Command( CM_XT_READ                        ); }
void MainWindow::slot_CM_STEP_READ                     ()  { Command( CM_STEP_READ                      ); }
void MainWindow::slot_CM_FILEEXPORT                    ()  { Command( CM_FILEEXPORT                     ); }
void MainWindow::slot_CM_SAT_WRITE                     ()  { Command( CM_SAT_WRITE                      ); }
void MainWindow::slot_CM_IGES_WRITE                    ()  { Command( CM_IGES_WRITE                     ); }
void MainWindow::slot_CM_XT_WRITE                      ()  { Command( CM_XT_WRITE                       ); }
void MainWindow::slot_CM_STEP_WRITE                    ()  { Command( CM_STEP_WRITE                     ); }
void MainWindow::slot_CM_POINTPOSITION                 ()  { Command( CM_POINTPOSITION                  ); }
void MainWindow::slot_CM_POINTON_CURVE                 ()  { Command( CM_POINTON_CURVE                  ); }
void MainWindow::slot_CM_N_POINTS_ON_CURVE             ()  { Command( CM_N_POINTS_ON_CURVE              ); }
void MainWindow::slot_CM_POINTONPARAMCURVE             ()  { Command( CM_POINTONPARAMCURVE              ); }
void MainWindow::slot_CM_POINTWEIGHTCENTRE             ()  { Command( CM_POINTWEIGHTCENTRE              ); }
void MainWindow::slot_CM_EDITINTERSECT                 ()  { Command( CM_EDITINTERSECT                  ); }
void MainWindow::slot_CM_POINTINTERSECT                ()  { Command( CM_POINTINTERSECT                 ); }
void MainWindow::slot_CM_EDITSELF_INTERSECT            ()  { Command( CM_EDITSELF_INTERSECT             ); }
void MainWindow::slot_CM_LINEPNTPNT                    ()  { Command( CM_LINEPNTPNT                     ); }
void MainWindow::slot_CM_LINEPNTANLGE0X                ()  { Command( CM_LINEPNTANLGE0X                 ); }
void MainWindow::slot_CM_LINEPOINTANGLE                ()  { Command( CM_LINEPOINTANGLE                 ); }
void MainWindow::slot_CM_LINEPNTDIST                   ()  { Command( CM_LINEPNTDIST                    ); }
void MainWindow::slot_CM_LINEPRLPNT                    ()  { Command( CM_LINEPRLPNT                     ); }
void MainWindow::slot_CM_LINEPRPPNT                    ()  { Command( CM_LINEPRPPNT                     ); }
void MainWindow::slot_CM_LINEPNTTANG                   ()  { Command( CM_LINEPNTTANG                    ); }
void MainWindow::slot_CM_LINETAN2CIR                   ()  { Command( CM_LINETAN2CIR                    ); }
void MainWindow::slot_CM_LINEANGLETAN_CIRCLE           ()  { Command( CM_LINEANGLETAN_CIRCLE            ); }
void MainWindow::slot_CM_LINEBISECTOR                  ()  { Command( CM_LINEBISECTOR                   ); }
void MainWindow::slot_CM_GORISONTALLINE                ()  { Command( CM_GORISONTALLINE                 ); }
void MainWindow::slot_CM_VERTICALLINE                  ()  { Command( CM_VERTICALLINE                   ); }
void MainWindow::slot_CM_CIRCLETHREEPOINTS             ()  { Command( CM_CIRCLETHREEPOINTS              ); }
void MainWindow::slot_CM_CIRCLECENTREPOINTON           ()  { Command( CM_CIRCLECENTREPOINTON            ); }
void MainWindow::slot_CM_CIRCLECENTRERADIUS            ()  { Command( CM_CIRCLECENTRERADIUS             ); }
void MainWindow::slot_CM_CIRCLERADIUSTWOPOINTS         ()  { Command( CM_CIRCLERADIUSTWOPOINTS          ); }
void MainWindow::slot_CM_CIRCLECENTREONRADIUSPOINTON   ()  { Command( CM_CIRCLECENTREONRADIUSPOINTON    ); }
void MainWindow::slot_CM_CIRCLECENTREONTWOPOINTS       ()  { Command( CM_CIRCLECENTREONTWOPOINTS        ); }
void MainWindow::slot_CM_CIRCLETANENTITYCENTRE         ()  { Command( CM_CIRCLETANENTITYCENTRE          ); }
void MainWindow::slot_CM_CIRCLETANENTITYTWOPOINTS      ()  { Command( CM_CIRCLETANENTITYTWOPOINTS       ); }
void MainWindow::slot_CM_CIRCLETANENTITYRADIUSPOINTON  ()  { Command( CM_CIRCLETANENTITYRADIUSPOINTON   ); }
void MainWindow::slot_CM_CIRCLETANTWOENTITIESPOINTON   ()  { Command( CM_CIRCLETANTWOENTITIESPOINTON    ); }
void MainWindow::slot_CM_CIRCLETANTWOENTITIESRADIUS    ()  { Command( CM_CIRCLETANTWOENTITIESRADIUS     ); }
void MainWindow::slot_CM_CIRCLETANTHREEENTITIES        ()  { Command( CM_CIRCLETANTHREEENTITIES         ); }
void MainWindow::slot_CM_NEWARC_3_POINTS               ()  { Command( CM_NEWARC_3_POINTS                ); }
void MainWindow::slot_CM_ARC2POINTSRADIUS              ()  { Command( CM_ARC2POINTSRADIUS               ); }
void MainWindow::slot_CM_ARCCENTRE2POINTS              ()  { Command( CM_ARCCENTRE2POINTS               ); }
void MainWindow::slot_CM_ARCCIRCLE2POINTS              ()  { Command( CM_ARCCIRCLE2POINTS               ); }
void MainWindow::slot_CM_ARCTANGENTCURVE2POINTS        ()  { Command( CM_ARCTANGENTCURVE2POINTS         ); }
void MainWindow::slot_CM_ARCTANGENTCURVERADPOINTON     ()  { Command( CM_ARCTANGENTCURVERADPOINTON      ); }
void MainWindow::slot_CM_ELLIPSECENTRE3POINTS          ()  { Command( CM_ELLIPSECENTRE3POINTS           ); }
void MainWindow::slot_CM_ELLIPSE3POINTS                ()  { Command( CM_ELLIPSE3POINTS                 ); }
void MainWindow::slot_CM_ELLIPSECENTRE_AB              ()  { Command( CM_ELLIPSECENTRE_AB               ); }
void MainWindow::slot_CM_ELLIPSE_TANGENT2CURVES        ()  { Command( CM_ELLIPSE_TANGENT2CURVES         ); }
void MainWindow::slot_CM_NEWELLIPSEARC                 ()  { Command( CM_NEWELLIPSEARC                  ); }
void MainWindow::slot_CM_NEWCONIC                      ()  { Command( CM_NEWCONIC                       ); }
void MainWindow::slot_CM_NEWSEGMENTPOINTPOINT          ()  { Command( CM_NEWSEGMENTPOINTPOINT           ); }
void MainWindow::slot_CM_NEWLINESEGDMIN                ()  { Command( CM_NEWLINESEGDMIN                 ); }
void MainWindow::slot_CM_NEWPOLYLINE                   ()  { Command( CM_NEWPOLYLINE                    ); }
void MainWindow::slot_CM_RECTANGLE                     ()  { Command( CM_RECTANGLE                      ); }
void MainWindow::slot_CM_REGULARPOLYGON                ()  { Command( CM_REGULARPOLYGON                 ); }
void MainWindow::slot_CM_NEWCBSPLN2D                   ()  { Command( CM_NEWCBSPLN2D                    ); }
void MainWindow::slot_CM_NEWHERMIT                     ()  { Command( CM_NEWHERMIT                      ); }
void MainWindow::slot_CM_NEWBEZIER                     ()  { Command( CM_NEWBEZIER                      ); }
void MainWindow::slot_CM_NEWBSPLINE                    ()  { Command( CM_NEWBSPLINE                     ); }
void MainWindow::slot_CM_SPLINE_EQUID                  ()  { Command( CM_SPLINE_EQUID                   ); }
void MainWindow::slot_CM_NEWOFFSETEXTENDED             ()  { Command( CM_NEWOFFSETEXTENDED              ); }
void MainWindow::slot_CM_NEWPLANECURVEBYSPACE          ()  { Command( CM_NEWPLANECURVEBYSPACE           ); }
void MainWindow::slot_CM_SPLINENURBS                   ()  { Command( CM_SPLINENURBS                    ); }
void MainWindow::slot_CM_NEWEDGEMAPONPLANE             ()  { Command( CM_NEWEDGEMAPONPLANE              ); }
void MainWindow::slot_CM_NEWCONTOUR                    ()  { Command( CM_NEWCONTOUR                     ); }
void MainWindow::slot_CM_NEWENVELOPCONTOUR             ()  { Command( CM_NEWENVELOPCONTOUR              ); }
void MainWindow::slot_CM_HANDENVELOPE                  ()  { Command( CM_HANDENVELOPE                   ); }
void MainWindow::slot_CM_CONTOUREQUID                  ()  { Command( CM_CONTOUREQUID                   ); }
void MainWindow::slot_CM_CONTOURFILLET                 ()  { Command( CM_CONTOURFILLET                  ); }
void MainWindow::slot_CM_CONTOURCHAMFER                ()  { Command( CM_CONTOURCHAMFER                 ); }
void MainWindow::slot_CM_CONTOURBREAK                  ()  { Command( CM_CONTOURBREAK                   ); }
void MainWindow::slot_CM_CONTOURUNITE                  ()  { Command( CM_CONTOURUNITE                   ); }
void MainWindow::slot_CM_CONTOURCROSS                  ()  { Command( CM_CONTOURCROSS                   ); }
void MainWindow::slot_CM_CONTOURSUBSTR                 ()  { Command( CM_CONTOURSUBSTR                  ); }
void MainWindow::slot_CM_ALL_EMBRACING_CNTRS           ()  { Command( CM_ALL_EMBRACING_CNTRS            ); }
void MainWindow::slot_CM_ENCLOSECONTOURS               ()  { Command( CM_ENCLOSECONTOURS                ); }
void MainWindow::slot_CM_OUTERCONTOURS                 ()  { Command( CM_OUTERCONTOURS                  ); }
void MainWindow::slot_CM_RECONSTRUCTCONTOURS           ()  { Command( CM_RECONSTRUCTCONTOURS            ); }
void MainWindow::slot_CM_MAKEOFFSETCONTOUR             ()  { Command( CM_MAKEOFFSETCONTOUR              ); }
void MainWindow::slot_CM_MAKESLOPECONTOUR              ()  { Command( CM_MAKESLOPECONTOUR               ); }
void MainWindow::slot_CM_MAKEOFFSETTHINCONTOUR         ()  { Command( CM_MAKEOFFSETTHINCONTOUR          ); }
void MainWindow::slot_CM_REGIONSBOOLEANOPER            ()  { Command( CM_REGIONSBOOLEANOPER             ); }
void MainWindow::slot_CM_MAKE_ALL_REGIONS              ()  { Command( CM_MAKE_ALL_REGIONS               ); }
void MainWindow::slot_CM_NEWCONTOURWITHBREAKS          ()  { Command( CM_NEWCONTOURWITHBREAKS           ); }
void MainWindow::slot_CM_CREATECNTRSMULTILINE          ()  { Command( CM_CREATECNTRSMULTILINE           ); }
void MainWindow::slot_CM_CREATEMULTILINE               ()  { Command( CM_CREATEMULTILINE                ); }
void MainWindow::slot_CM_NEWPOINT                      ()  { Command( CM_NEWPOINT                       ); }
void MainWindow::slot_CM_NEWNEARPOINTONCURVE           ()  { Command( CM_NEWNEARPOINTONCURVE            ); }
void MainWindow::slot_CM_NEWPOINTONCURVE               ()  { Command( CM_NEWPOINTONCURVE                ); }
void MainWindow::slot_CM_NEWCURVEISOCLINAL             ()  { Command( CM_NEWCURVEISOCLINAL              ); }
void MainWindow::slot_CM_NEWEVENPARAM                  ()  { Command( CM_NEWEVENPARAM                   ); }
void MainWindow::slot_CM_NEWEVENLENGTH                 ()  { Command( CM_NEWEVENLENGTH                  ); }
void MainWindow::slot_CM_DISTANCEALONG                 ()  { Command( CM_DISTANCEALONG                  ); }
void MainWindow::slot_CM_CREATECURVEISOPOINTS          ()  { Command( CM_CREATECURVEISOPOINTS           ); }
void MainWindow::slot_CM_NEWNEARPOINTONSURFACE         ()  { Command( CM_NEWNEARPOINTONSURFACE          ); }
void MainWindow::slot_CM_NEWPOINTONSURFACE             ()  { Command( CM_NEWPOINTONSURFACE              ); }
void MainWindow::slot_CM_NEWDIRECTPOINTONSURFACE       ()  { Command( CM_NEWDIRECTPOINTONSURFACE        ); }
void MainWindow::slot_CM_NEWSURFACEISOCLINAL           ()  { Command( CM_NEWSURFACEISOCLINAL            ); }
void MainWindow::slot_CM_NEWPOINTS_ON_SURFACE          ()  { Command( CM_NEWPOINTS_ON_SURFACE           ); }
void MainWindow::slot_CM_NEWPOINTONSOLID               ()  { Command( CM_NEWPOINTONSOLID                ); }
void MainWindow::slot_CM_NEWDIRECTPOINTONSOLID         ()  { Command( CM_NEWDIRECTPOINTONSOLID          ); }
void MainWindow::slot_CM_NEWPOINTCURVECROSSING         ()  { Command( CM_NEWPOINTCURVECROSSING          ); }
void MainWindow::slot_CM_NEWPOINTCURVESURFACE          ()  { Command( CM_NEWPOINTCURVESURFACE           ); }
void MainWindow::slot_CM_NEWPOINTCURVESOLID            ()  { Command( CM_NEWPOINTCURVESOLID             ); }
void MainWindow::slot_CM_NEWLINE3D                     ()  { Command( CM_NEWLINE3D                      ); }
void MainWindow::slot_CM_NEWLINEPOINTPERPCURVE         ()  { Command( CM_NEWLINEPOINTPERPCURVE          ); }
void MainWindow::slot_CM_NEWLINEPOINTPERPSURFACE       ()  { Command( CM_NEWLINEPOINTPERPSURFACE        ); }
void MainWindow::slot_CM_NEWLINEPOINTPERPSOLID         ()  { Command( CM_NEWLINEPOINTPERPSOLID          ); }
void MainWindow::slot_CM_NEWLINEPLANEPLANE             ()  { Command( CM_NEWLINEPLANEPLANE              ); }
void MainWindow::slot_CM_RECTANGLEONPLANE              ()  { Command( CM_RECTANGLEONPLANE               ); }
void MainWindow::slot_CM_NEWPLANECBSPLN                ()  { Command( CM_NEWPLANECBSPLN                 ); }
void MainWindow::slot_CM_NEWPLANENURBS                 ()  { Command( CM_NEWPLANENURBS                  ); }
void MainWindow::slot_CM_NEWPLANEOFFSET                ()  { Command( CM_NEWPLANEOFFSET                 ); }
void MainWindow::slot_CM_NEWCONTOURONPLANE             ()  { Command( CM_NEWCONTOURONPLANE              ); }
void MainWindow::slot_CM_NEWCURVEBYPLANE               ()  { Command( CM_NEWCURVEBYPLANE                ); }
void MainWindow::slot_CM_GETPLANECURVE                 ()  { Command( CM_GETPLANECURVE                  ); }
void MainWindow::slot_CM_NEWCIRCLE3D                   ()  { Command( CM_NEWCIRCLE3D                    ); }
void MainWindow::slot_CM_NEWCIRCLE3DBYCENTRE           ()  { Command( CM_NEWCIRCLE3DBYCENTRE            ); }
void MainWindow::slot_CM_NEWCIRCLE3DBYPOINTS           ()  { Command( CM_NEWCIRCLE3DBYPOINTS            ); }
void MainWindow::slot_CM_NEWELLIPSE3D                  ()  { Command( CM_NEWELLIPSE3D                   ); }
void MainWindow::slot_CM_NEWNURBSCONIC3D_1             ()  { Command( CM_NEWNURBSCONIC3D_1              ); }
void MainWindow::slot_CM_NEWNURBSCONIC3D_2             ()  { Command( CM_NEWNURBSCONIC3D_2              ); }
void MainWindow::slot_CM_NEWNURBSCONIC3D_3             ()  { Command( CM_NEWNURBSCONIC3D_3              ); }
void MainWindow::slot_CM_NEWNURBSCONIC3D_4             ()  { Command( CM_NEWNURBSCONIC3D_4              ); }
void MainWindow::slot_CM_NEWNURBSCONIC3D_5             ()  { Command( CM_NEWNURBSCONIC3D_5              ); }
void MainWindow::slot_CM_NEWNURBSCONIC3D_6             ()  { Command( CM_NEWNURBSCONIC3D_6              ); }
void MainWindow::slot_CM_NEWLINESEGMENT3D              ()  { Command( CM_NEWLINESEGMENT3D               ); }
void MainWindow::slot_CM_NEWPOLYLINE3D                 ()  { Command( CM_NEWPOLYLINE3D                  ); }
void MainWindow::slot_CM_NEWCUBICSPLINE3D              ()  { Command( CM_NEWCUBICSPLINE3D               ); }
void MainWindow::slot_CM_NEWHERMIT3D                   ()  { Command( CM_NEWHERMIT3D                    ); }
void MainWindow::slot_CM_NEWBEZIER3D                   ()  { Command( CM_NEWBEZIER3D                    ); }
void MainWindow::slot_CM_NEWNURBS3D                    ()  { Command( CM_NEWNURBS3D                     ); }
void MainWindow::slot_CM_NEWNURBS3D_WITH_MATING        ()  { Command( CM_NEWNURBS3D_WITH_MATING         ); }
void MainWindow::slot_CM_NEWARC3D                      ()  { Command( CM_NEWARC3D                       ); }
void MainWindow::slot_CM_NEWARC3DBYPOINTS              ()  { Command( CM_NEWARC3DBYPOINTS               ); }
void MainWindow::slot_CM_NEWSPIRALCURVE                ()  { Command( CM_NEWSPIRALCURVE                 ); }
void MainWindow::slot_CM_NEWCONTOUR3D                  ()  { Command( CM_NEWCONTOUR3D                   ); }
void MainWindow::slot_CM_CURVE_B_SPLINE                ()  { Command( CM_CURVE_B_SPLINE                 ); }
void MainWindow::slot_CM_NEWTRIMMEDCURVE3D             ()  { Command( CM_NEWTRIMMEDCURVE3D              ); }
void MainWindow::slot_CM_NEWEXTENDEDCURVE3D            ()  { Command( CM_NEWEXTENDEDCURVE3D             ); }
void MainWindow::slot_CM_NEWOFFSETCURVE3D              ()  { Command( CM_NEWOFFSETCURVE3D               ); }
void MainWindow::slot_CM_NEWCURVESBRIDGE3D             ()  { Command( CM_NEWCURVESBRIDGE3D              ); }
void MainWindow::slot_CM_NEWCURVESCOUPLE3D             ()  { Command( CM_NEWCURVESCOUPLE3D              ); }
void MainWindow::slot_CM_NEWCURVESFILLET3D             ()  { Command( CM_NEWCURVESFILLET3D              ); }
void MainWindow::slot_CM_NEWCURVESCONNCURVE3D          ()  { Command( CM_NEWCURVESCONNCURVE3D           ); }
void MainWindow::slot_CM_NEWCONTOUR3DBYCURVES          ()  { Command( CM_NEWCONTOUR3DBYCURVES           ); }
void MainWindow::slot_CM_NEWFILLETCONTOUR3DBYCURVES    ()  { Command( CM_NEWFILLETCONTOUR3DBYCURVES     ); }
void MainWindow::slot_CM_NEWCURVE_BY_TWO_PROJECTIONS   ()  { Command( CM_NEWCURVE_BY_TWO_PROJECTIONS    ); }
void MainWindow::slot_CM_NEWSPACECURVEBYPLANE          ()  { Command( CM_NEWSPACECURVEBYPLANE           ); }
void MainWindow::slot_CM_TRANSLATECURVETONURBS         ()  { Command( CM_TRANSLATECURVETONURBS          ); }
void MainWindow::slot_CM_APPROXBYARCS                  ()  { Command( CM_APPROXBYARCS                   ); }
void MainWindow::slot_CM_NEWSURFACEBORDER              ()  { Command( CM_NEWSURFACEBORDER               ); }
void MainWindow::slot_CM_NEWCURVEONSURFACE             ()  { Command( CM_NEWCURVEONSURFACE              ); }
void MainWindow::slot_CM_NEWCURVEDIRECTVECTOR          ()  { Command( CM_NEWCURVEDIRECTVECTOR           ); }
void MainWindow::slot_CM_NEWPROJECTIONCURVE_ON_SURFACE ()  { Command( CM_NEWPROJECTIONCURVE_ON_SURFACE  ); }
void MainWindow::slot_CM_NEWCURVE_ON_SURFACE_BY_POINTS ()  { Command( CM_NEWCURVE_ON_SURFACE_BY_POINTS  ); }
void MainWindow::slot_CM_NEWCURVESWRAPPING             ()  { Command( CM_NEWCURVESWRAPPING              ); }
void MainWindow::slot_CM_NEWEDGESCURVE                 ()  { Command( CM_NEWEDGESCURVE                  ); }
void MainWindow::slot_CM_NEWFACECONTOUR                ()  { Command( CM_NEWFACECONTOUR                 ); }
void MainWindow::slot_CM_NEWFACEPROJECTION             ()  { Command( CM_NEWFACEPROJECTION              ); }
void MainWindow::slot_CM_NEWEDGEMAP                    ()  { Command( CM_NEWEDGEMAP                     ); }
void MainWindow::slot_CM_NEWSURFACEINTERSECTION        ()  { Command( CM_NEWSURFACEINTERSECTION         ); }
void MainWindow::slot_CM_NEWSURFACESOLIDINTERSECT      ()  { Command( CM_NEWSURFACESOLIDINTERSECT       ); }
void MainWindow::slot_CM_NEWSOLIDINTERSECTION          ()  { Command( CM_NEWSOLIDINTERSECTION           ); }
void MainWindow::slot_CM_NEWFACESINTERSECTION          ()  { Command( CM_NEWFACESINTERSECTION           ); }
void MainWindow::slot_CM_NEWCHARCURVE3D                ()  { Command( CM_NEWCHARCURVE3D                 ); }
void MainWindow::slot_CM_NEWPLANE3POINTS               ()  { Command( CM_NEWPLANE3POINTS                ); }
void MainWindow::slot_CM_NEWPLANE                      ()  { Command( CM_NEWPLANE                       ); }
void MainWindow::slot_CM_NEWPLANEEDGE                  ()  { Command( CM_NEWPLANEEDGE                   ); }
void MainWindow::slot_CM_NEWCYLINDER3POINTS            ()  { Command( CM_NEWCYLINDER3POINTS             ); }
void MainWindow::slot_CM_NEWCONE3POINTS                ()  { Command( CM_NEWCONE3POINTS                 ); }
void MainWindow::slot_CM_NEWSPHERE3POINTS              ()  { Command( CM_NEWSPHERE3POINTS               ); }
void MainWindow::slot_CM_NEWTORUS3POINTS               ()  { Command( CM_NEWTORUS3POINTS                ); }
void MainWindow::slot_CM_NEWTRIBEZIERSURFACE           ()  { Command( CM_NEWTRIBEZIERSURFACE            ); }
void MainWindow::slot_CM_NEWRECTANGLENURBS             ()  { Command( CM_NEWRECTANGLENURBS              ); }
void MainWindow::slot_CM_NEWSPLINESURFACE              ()  { Command( CM_NEWSPLINESURFACE               ); }
void MainWindow::slot_CM_NEWSIMPLEXSURFACE             ()  { Command( CM_NEWSIMPLEXSURFACE              ); }
void MainWindow::slot_CM_NEWSIMPLEXSPLINESURFACE       ()  { Command( CM_NEWSIMPLEXSPLINESURFACE        ); }
void MainWindow::slot_CM_NEWEXTRUSION                  ()  { Command( CM_NEWEXTRUSION                   ); }
void MainWindow::slot_CM_NEWREVOLUTION                 ()  { Command( CM_NEWREVOLUTION                  ); }
void MainWindow::slot_CM_NEWEXPANSION                  ()  { Command( CM_NEWEXPANSION                   ); }
void MainWindow::slot_CM_NEWEVOLUTION                  ()  { Command( CM_NEWEVOLUTION                   ); }
void MainWindow::slot_CM_NEWSPIRALSURFACE              ()  { Command( CM_NEWSPIRALSURFACE               ); }
void MainWindow::slot_CM_NEWRULEDSURFACE               ()  { Command( CM_NEWRULEDSURFACE                ); }
void MainWindow::slot_CM_NEWSECTORSURFACE              ()  { Command( CM_NEWSECTORSURFACE               ); }
void MainWindow::slot_CM_NEWCORNERSURFACE              ()  { Command( CM_NEWCORNERSURFACE               ); }
void MainWindow::slot_CM_NEWCOVERSURFACE               ()  { Command( CM_NEWCOVERSURFACE                ); }
void MainWindow::slot_CM_NEWLOFTEDSURFACE              ()  { Command( CM_NEWLOFTEDSURFACE               ); }
void MainWindow::slot_CM_NEWELEVATIONSURFACE           ()  { Command( CM_NEWELEVATIONSURFACE            ); }
void MainWindow::slot_CM_NEWMESHSURFACE                ()  { Command( CM_NEWMESHSURFACE                 ); }
void MainWindow::slot_CM_NEWNURBSSURFACE               ()  { Command( CM_NEWNURBSSURFACE                ); }
void MainWindow::slot_CM_NEWOFFSETSURFACE              ()  { Command( CM_NEWOFFSETSURFACE               ); }
void MainWindow::slot_CM_NEWDEFORMEDSURFACE            ()  { Command( CM_NEWDEFORMEDSURFACE             ); }
void MainWindow::slot_CM_NEWEXTENDEDSURFACE            ()  { Command( CM_NEWEXTENDEDSURFACE             ); }
void MainWindow::slot_CM_NEWJOINSURFACE                ()  { Command( CM_NEWJOINSURFACE                 ); }
void MainWindow::slot_CM_NEWCURVEBOUNDEDSURFACE        ()  { Command( CM_NEWCURVEBOUNDEDSURFACE         ); }
void MainWindow::slot_CM_NEWGRIDSURFACE                ()  { Command( CM_NEWGRIDSURFACE                 ); }
void MainWindow::slot_CM_TRANSLATESURFACETONURBS       ()  { Command( CM_TRANSLATESURFACETONURBS        ); }
void MainWindow::slot_CM_NEW_WIREFRAME_BY_CURVES       ()  { Command( CM_NEW_WIREFRAME_BY_CURVES        ); }
void MainWindow::slot_CM_NEWBLOCKSOLID                 ()  { Command( CM_NEWBLOCKSOLID                  ); }
void MainWindow::slot_CM_NEWWEDGESOLID                 ()  { Command( CM_NEWWEDGESOLID                  ); }
void MainWindow::slot_CM_NEWCYLINDERSOLID              ()  { Command( CM_NEWCYLINDERSOLID               ); }
void MainWindow::slot_CM_NEWCONESOLID                  ()  { Command( CM_NEWCONESOLID                   ); }
void MainWindow::slot_CM_NEWSPHERESOLID                ()  { Command( CM_NEWSPHERESOLID                 ); }
void MainWindow::slot_CM_NEWTORUSSOLID                 ()  { Command( CM_NEWTORUSSOLID                  ); }
void MainWindow::slot_CM_NEWPLATESOLID                 ()  { Command( CM_NEWPLATESOLID                  ); }
void MainWindow::slot_CM_NEWPRISMSOLID                 ()  { Command( CM_NEWPRISMSOLID                  ); }
void MainWindow::slot_CM_NEWPYRAMIDSOLID               ()  { Command( CM_NEWPYRAMIDSOLID                ); }
void MainWindow::slot_CM_NEWNURBSBLOCKSOLID            ()  { Command( CM_NEWNURBSBLOCKSOLID             ); }
void MainWindow::slot_CM_NEWCURVEEXTRUSIONSOLID        ()  { Command( CM_NEWCURVEEXTRUSIONSOLID         ); }
void MainWindow::slot_CM_NEWCURVE3DEXTRUSIONSOLID      ()  { Command( CM_NEWCURVE3DEXTRUSIONSOLID       ); }
void MainWindow::slot_CM_NEWCURVEREVOLUTIONSOLID       ()  { Command( CM_NEWCURVEREVOLUTIONSOLID        ); }
void MainWindow::slot_CM_NEWCURVE3DREVOLUTIONSOLID     ()  { Command( CM_NEWCURVE3DREVOLUTIONSOLID      ); }
void MainWindow::slot_CM_NEWCURVEEVOLUTIONSOLID        ()  { Command( CM_NEWCURVEEVOLUTIONSOLID         ); }
void MainWindow::slot_CM_NEWCURVELOFTEDSOLID           ()  { Command( CM_NEWCURVELOFTEDSOLID            ); }
void MainWindow::slot_CM_NEWCURVELOFTEDSOLID_CL        ()  { Command( CM_NEWCURVELOFTEDSOLID_CL         ); }
void MainWindow::slot_CM_NEWRULEDSOLID                 ()  { Command( CM_NEWRULEDSOLID                  ); }
void MainWindow::slot_CM_NEWSHEETSOLID                 ()  { Command( CM_NEWSHEETSOLID                  ); }
void MainWindow::slot_CM_NEWELEMENTARYSOLID            ()  { Command( CM_NEWELEMENTARYSOLID             ); }
void MainWindow::slot_CM_BOOLEANUNION                  ()  { Command( CM_BOOLEANUNION                   ); }
void MainWindow::slot_CM_BOSSCURVEEXTRUSIONSOLID       ()  { Command( CM_BOSSCURVEEXTRUSIONSOLID        ); }
void MainWindow::slot_CM_BOSSCURVEREVOLUTIONSOLID      ()  { Command( CM_BOSSCURVEREVOLUTIONSOLID       ); }
void MainWindow::slot_CM_BOSSCURVEEVOLUTIONSOLID       ()  { Command( CM_BOSSCURVEEVOLUTIONSOLID        ); }
void MainWindow::slot_CM_BOSSCURVELOFTEDSOLID          ()  { Command( CM_BOSSCURVELOFTEDSOLID           ); }
void MainWindow::slot_CM_BOSSCURVELOFTEDSOLID_CL       ()  { Command( CM_BOSSCURVELOFTEDSOLID_CL        ); }
void MainWindow::slot_CM_BOSSUNIONSOLID                ()  { Command( CM_BOSSUNIONSOLID                 ); }
void MainWindow::slot_CM_BOOLEANDIFFERENCE             ()  { Command( CM_BOOLEANDIFFERENCE              ); }
void MainWindow::slot_CM_CUTCURVEEXTRUSIONSOLID        ()  { Command( CM_CUTCURVEEXTRUSIONSOLID         ); }
void MainWindow::slot_CM_CUTCURVEREVOLUTIONSOLID       ()  { Command( CM_CUTCURVEREVOLUTIONSOLID        ); }
void MainWindow::slot_CM_CUTCURVEEVOLUTIONSOLID        ()  { Command( CM_CUTCURVEEVOLUTIONSOLID         ); }
void MainWindow::slot_CM_CUTCURVELOFTEDSOLID           ()  { Command( CM_CUTCURVELOFTEDSOLID            ); }
void MainWindow::slot_CM_CUTCURVELOFTEDSOLID_CL        ()  { Command( CM_CUTCURVELOFTEDSOLID_CL         ); }
void MainWindow::slot_CM_CUTUNIONSOLID                 ()  { Command( CM_CUTUNIONSOLID                  ); }
void MainWindow::slot_CM_BOOLEANINTERSECTION           ()  { Command( CM_BOOLEANINTERSECTION            ); }
void MainWindow::slot_CM_INTERSECTCURVEEXTRUSIONSOLID  ()  { Command( CM_INTERSECTCURVEEXTRUSIONSOLID   ); }
void MainWindow::slot_CM_INTERSECTCURVEREVOLUTIONSOLID ()  { Command( CM_INTERSECTCURVEREVOLUTIONSOLID  ); }
void MainWindow::slot_CM_INTERSECTCURVEEVOLUTIONSOLID  ()  { Command( CM_INTERSECTCURVEEVOLUTIONSOLID   ); }
void MainWindow::slot_CM_INTERSECTCURVELOFTEDSOLID     ()  { Command( CM_INTERSECTCURVELOFTEDSOLID      ); }
void MainWindow::slot_CM_INTERSECTCURVELOFTEDSOLID_CL  ()  { Command( CM_INTERSECTCURVELOFTEDSOLID_CL   ); }
void MainWindow::slot_CM_INTERSECTUNIONSOLID           ()  { Command( CM_INTERSECTUNIONSOLID            ); }
void MainWindow::slot_CM_SOLIDFILLET                   ()  { Command( CM_SOLIDFILLET                    ); }
void MainWindow::slot_CM_SOLIDCHANNEL                  ()  { Command( CM_SOLIDCHANNEL                   ); }
void MainWindow::slot_CM_SOLIDSPAN                     ()  { Command( CM_SOLIDSPAN                      ); }
void MainWindow::slot_CM_SOLIDSMOOTH                   ()  { Command( CM_SOLIDSMOOTH                    ); }
void MainWindow::slot_CM_SOLIDCHAMFER                  ()  { Command( CM_SOLIDCHAMFER                   ); }
void MainWindow::slot_CM_SOLIDSLANT1                   ()  { Command( CM_SOLIDSLANT1                    ); }
void MainWindow::slot_CM_SOLIDSLANT2                   ()  { Command( CM_SOLIDSLANT2                    ); }
void MainWindow::slot_CM_SOLID_CONTOUR_CUT             ()  { Command( CM_SOLID_CONTOUR_CUT              ); }
void MainWindow::slot_CM_SOLID_SURFACE_CUT             ()  { Command( CM_SOLID_SURFACE_CUT              ); }
void MainWindow::slot_CM_SOLID_SHELL_CUT               ()  { Command( CM_SOLID_SURFACE_CUT              ); }
void MainWindow::slot_CM_NEWSYMMETRYSOLID              ()  { Command( CM_NEWSYMMETRYSOLID               ); }
void MainWindow::slot_CM_NEWTHINSOLID_ONESIDE_OFFSET   ()  { Command( CM_NEWTHINSOLID_ONESIDE_OFFSET    ); }
void MainWindow::slot_CM_NEWTHINSOLID_ONESIDE_MONOTHICK()  { Command( CM_NEWTHINSOLID_ONESIDE_MONOTHICK ); }
void MainWindow::slot_CM_NEWTHINSOLID_ONESIDE_POLYTHICK()  { Command( CM_NEWTHINSOLID_ONESIDE_POLYTHICK ); }
void MainWindow::slot_CM_FULLFILLETSOLID               ()  { Command( CM_FULLFILLETSOLID                ); }
void MainWindow::slot_CM_SOLIDVERTEXFILLET             ()  { Command( CM_SOLIDVERTEXFILLET              ); }
void MainWindow::slot_CM_SOLIDNPDRAFT                  ()  { Command( CM_SOLIDNPDRAFT                   ); }
void MainWindow::slot_CM_NEW_SPLIT_SOLID               ()  { Command( CM_NEW_SPLIT_SOLID                ); }
void MainWindow::slot_CM_NEWRIBSOLID                   ()  { Command( CM_NEWRIBSOLID                    ); }
void MainWindow::slot_CM_NEWRIBSHELL                   ()  { Command( CM_NEWRIBSHELL                    ); }
void MainWindow::slot_CM_NEWHOLESOLID                  ()  { Command( CM_NEWHOLESOLID                   ); }
void MainWindow::slot_CM_NEWPOCKETSOLID                ()  { Command( CM_NEWPOCKETSOLID                 ); }
void MainWindow::slot_CM_NEWSLOTSOLID                  ()  { Command( CM_NEWSLOTSOLID                   ); }
void MainWindow::slot_CM_NEWUNIONSOLID                 ()  { Command( CM_NEWUNIONSOLID                  ); }
void MainWindow::slot_CM_CREATE_PARTSOLID              ()  { Command( CM_CREATE_PARTSOLID               ); }
void MainWindow::slot_CM_SILHOUETTECONTOURSOLID        ()  { Command( CM_SILHOUETTECONTOURSOLID         ); }
void MainWindow::slot_CM_NEWSOLIDEXTRUSIONSOLID        ()  { Command( CM_NEWSOLIDEXTRUSIONSOLID         ); }
void MainWindow::slot_CM_NEWSOLIDREVOLUTIONSOLID       ()  { Command( CM_NEWSOLIDREVOLUTIONSOLID        ); }
void MainWindow::slot_CM_NEWSOLIDEVOLUTIONSOLID        ()  { Command( CM_NEWSOLIDEVOLUTIONSOLID         ); }
void MainWindow::slot_CM_DUPLICATION_BYGRID            ()  { Command( CM_DUPLICATION_BYGRID             ); }
void MainWindow::slot_CM_DUPLICATION_BYMATRICES        ()  { Command( CM_DUPLICATION_BYMATRICES         ); }
void MainWindow::slot_CM_THINSOLIDBYSHELL              ()  { Command( CM_THINSOLIDBYSHELL               ); }
void MainWindow::slot_CM_CREATESHEETSOLID              ()  { Command( CM_CREATESHEETSOLID               ); }
void MainWindow::slot_CM_BENDOVERSEGMENT               ()  { Command( CM_BENDOVERSEGMENT                ); }
void MainWindow::slot_CM_JOGSHEETSOLID                 ()  { Command( CM_JOGSHEETSOLID                  ); }
void MainWindow::slot_CM_BENDBYEDGES                   ()  { Command( CM_BENDBYEDGES                    ); }
void MainWindow::slot_CM_JOINTBEND                     ()  { Command( CM_JOINTBEND                      ); }
void MainWindow::slot_CM_SHEETSOLIDPLATE               ()  { Command( CM_SHEETSOLIDPLATE                ); }
void MainWindow::slot_CM_SHEETSOLIDHOLE                ()  { Command( CM_SHEETSOLIDHOLE                 ); }
void MainWindow::slot_CM_SHEETSOLIDINTERSECT           ()  { Command( CM_SHEETSOLIDINTERSECT            ); }
void MainWindow::slot_CM_CLOSECORNER                   ()  { Command( CM_CLOSECORNER                    ); }
void MainWindow::slot_CM_STAMPSHEETSOLID               ()  { Command( CM_STAMPSHEETSOLID                ); }
void MainWindow::slot_CM_BEADSHEETSOLID                ()  { Command( CM_BEADSHEETSOLID                 ); }
void MainWindow::slot_CM_JALOUSIESHEETSOLID            ()  { Command( CM_JALOUSIESHEETSOLID             ); }
void MainWindow::slot_CM_CREATERULEDSOLID              ()  { Command( CM_CREATERULEDSOLID               ); }
void MainWindow::slot_CM_TRANSFORMEDSOLID              ()  { Command( CM_TRANSFORMEDSOLID               ); }
void MainWindow::slot_CM_REMOVEMODIFIEDSOLID           ()  { Command( CM_REMOVEMODIFIEDSOLID            ); }
void MainWindow::slot_CM_CREATEMODIFIEDSOLID           ()  { Command( CM_CREATEMODIFIEDSOLID            ); }
void MainWindow::slot_CM_ACTIONMODIFIEDSOLID           ()  { Command( CM_ACTIONMODIFIEDSOLID            ); }
void MainWindow::slot_CM_OFFSETMODIFIEDSOLID           ()  { Command( CM_OFFSETMODIFIEDSOLID            ); }
void MainWindow::slot_CM_FILLETMODIFIEDSOLID           ()  { Command( CM_FILLETMODIFIEDSOLID            ); }
void MainWindow::slot_CM_SUPPLEMODIFIEDSOLID           ()  { Command( CM_SUPPLEMODIFIEDSOLID            ); }
void MainWindow::slot_CM_SUPPLESETCONJUGATION          ()  { Command( CM_SUPPLESETCONJUGATION           ); }
void MainWindow::slot_CM_SUPPLESETSIMILARITY           ()  { Command( CM_SUPPLESETSIMILARITY            ); }
void MainWindow::slot_CM_SUPPLESETSMOOTHING            ()  { Command( CM_SUPPLESETSMOOTHING             ); }
void MainWindow::slot_CM_PURIFYMODIFIEDSOLID           ()  { Command( CM_PURIFYMODIFIEDSOLID            ); }
void MainWindow::slot_CM_MERGERMODIFIEDSOLID           ()  { Command( CM_MERGERMODIFIEDSOLID            ); }
void MainWindow::slot_CM_NEWNURBSSURFACESHELL          ()  { Command( CM_NEWNURBSSURFACESHELL           ); }
void MainWindow::slot_CM_NEWCURVEEXTRUSIONSHELL        ()  { Command( CM_NEWCURVEEXTRUSIONSHELL         ); }
void MainWindow::slot_CM_NEWCURVEREVOLUTIONSHELL       ()  { Command( CM_NEWCURVEREVOLUTIONSHELL        ); }
void MainWindow::slot_CM_NEWCURVEEVOLUTIONSHELL        ()  { Command( CM_NEWCURVEEVOLUTIONSHELL         ); }
void MainWindow::slot_CM_NEWCURVELOFTEDSHELL           ()  { Command( CM_NEWCURVELOFTEDSHELL            ); }
void MainWindow::slot_CM_NEWCURVELOFTEDSHELL_CL        ()  { Command( CM_NEWCURVELOFTEDSHELL_CL         ); }
void MainWindow::slot_CM_NEWMESHSHELL                  ()  { Command( CM_NEWMESHSHELL                   ); }
void MainWindow::slot_CM_NEWRULSHELL                   ()  { Command( CM_NEWRULSHELL                    ); }
void MainWindow::slot_CM_NEWJOINSOLID                  ()  { Command( CM_NEWJOINSOLID                   ); }
void MainWindow::slot_CM_SHELLSURFACE                  ()  { Command( CM_SHELLSURFACE                   ); }
void MainWindow::slot_CM_LOFTEDSHELLBYPOINTS           ()  { Command( CM_LOFTEDSHELLBYPOINTS            ); }
void MainWindow::slot_CM_LOFTEDSHELLBYCURVES           ()  { Command( CM_LOFTEDSHELLBYCURVES            ); }
void MainWindow::slot_CM_SOLIDVARIETY                  ()  { Command( CM_SOLIDVARIETY                   ); }
void MainWindow::slot_CM_SOLIDEXTERNAL                 ()  { Command( CM_SOLIDEXTERNAL                  ); }
void MainWindow::slot_CM_SOLIDINTERNAL                 ()  { Command( CM_SOLIDINTERNAL                  ); }
void MainWindow::slot_CM_CUTSHELL_BYCONTOUR            ()  { Command( CM_CUTSHELL_BYCONTOUR             ); }
void MainWindow::slot_CM_CUTSHELL_BYSURFACE            ()  { Command( CM_CUTSHELL_BYSURFACE             ); }
void MainWindow::slot_CM_CUTSHELL_BYOBJECTS            ()  { Command( CM_CUTSHELL_BYOBJECTS             ); }
void MainWindow::slot_CM_SHELLOFSOLID                  ()  { Command( CM_SHELLOFSOLID                   ); }
void MainWindow::slot_CM_OFFSETSHELLBYFACES            ()  { Command( CM_OFFSETSHELLBYFACES             ); }
void MainWindow::slot_CM_NEWEXTENSIONSHELL             ()  { Command( CM_NEWEXTENSIONSHELL              ); }
void MainWindow::slot_CM_FACESFILLET                   ()  { Command( CM_FACESFILLET                    ); }
void MainWindow::slot_CM_SHELLSEW                      ()  { Command( CM_SHELLSEW                       ); }
void MainWindow::slot_CM_SOLIDSEW                      ()  { Command( CM_SOLIDSEW                       ); }
void MainWindow::slot_CM_SHELLPATCH                    ()  { Command( CM_SHELLPATCH                     ); }
void MainWindow::slot_CM_CURVEPATCH                    ()  { Command( CM_CURVEPATCH                     ); }
void MainWindow::slot_CM_NEWINSTANCE                   ()  { Command( CM_NEWINSTANCE                    ); }
void MainWindow::slot_CM_NEWASSEMBLY                   ()  { Command( CM_NEWASSEMBLY                    ); }
void MainWindow::slot_CM_NEWCURVEHIDEMAP               ()  { Command( CM_NEWCURVEHIDEMAP                ); }
void MainWindow::slot_CM_NEWSURFACEHIDEMAP             ()  { Command( CM_NEWSURFACEHIDEMAP              ); }
void MainWindow::slot_CM_NEWSOLIDHIDEMAP               ()  { Command( CM_NEWSOLIDHIDEMAP                ); }
void MainWindow::slot_CM_NEWSOLIDSHIDEMAP              ()  { Command( CM_NEWSOLIDSHIDEMAP               ); }
void MainWindow::slot_CM_NEW_SOLID_CONTOUR_SECTION_MAP ()  { Command( CM_NEW_SOLID_CONTOUR_SECTION_MAP  ); }
void MainWindow::slot_CM_NEW_SOLID_LOCAL_SECTION_MAP   ()  { Command( CM_NEW_SOLID_LOCAL_SECTION_MAP    ); }
//void MainWindow::slot_CM_NEW_CONSTRUCTIVE_BY_POINTS    ()  { Command( CM_NEW_CONSTRUCTIVE_BY_POINTS     ); }
void MainWindow::slot_CM_DUPLICATEOBJECT               ()  { Command( CM_DUPLICATEOBJECT                ); }
void MainWindow::slot_CM_MIRRORDUPLICATEOBJECT         ()  { Command( CM_MIRRORDUPLICATEOBJECT          ); }
void MainWindow::slot_CM_NEWTHREAD_INSPACE             ()  { Command( CM_NEWTHREAD_INSPACE              ); }
void MainWindow::slot_CM_NEWTHREAD_ONSOLID             ()  { Command( CM_NEWTHREAD_ONSOLID              ); }
void MainWindow::slot_CM_NEWTHREAD_MAPPING_BYPARAMS    ()  { Command( CM_NEWTHREAD_MAPPING_BYPARAMS     ); }
void MainWindow::slot_CM_NEWTHREAD_MAPPING_BYTHREAD    ()  { Command( CM_NEWTHREAD_MAPPING_BYTHREAD     ); }
void MainWindow::slot_CM_MATE_COINCIDENCE              ()  { Command( CM_MATE_COINCIDENCE               ); }
void MainWindow::slot_CM_MATE_PARALLELISM              ()  { Command( CM_MATE_PARALLELISM               ); }
void MainWindow::slot_CM_MATE_PERPENDICULAR            ()  { Command( CM_MATE_PERPENDICULAR             ); }
void MainWindow::slot_CM_MATE_TANGENCY                 ()  { Command( CM_MATE_TANGENCY                  ); }
void MainWindow::slot_CM_MATE_CONCENTRIC               ()  { Command( CM_MATE_CONCENTRIC                ); }
void MainWindow::slot_CM_MATE_DISTANCE                 ()  { Command( CM_MATE_DISTANCE                  ); }
void MainWindow::slot_CM_MATE_ANGLE                    ()  { Command( CM_MATE_ANGLE                     ); }
void MainWindow::slot_CM_GC_COINCIDENCE                ()  { Command( CM_GC_COINCIDENCE                 ); }
void MainWindow::slot_CM_GC_PARALLELISM                ()  { Command( CM_GC_PARALLELISM                 ); }
void MainWindow::slot_CM_GC_PERPENDICULAR              ()  { Command( CM_GC_PERPENDICULAR               ); }
void MainWindow::slot_CM_GC_TANGENCY                   ()  { Command( CM_GC_TANGENCY                    ); }
void MainWindow::slot_CM_GC_CONCENTRIC                 ()  { Command( CM_GC_CONCENTRIC                  ); }
void MainWindow::slot_CM_GC_DISTANCE                   ()  { Command( CM_GC_DISTANCE                    ); }
void MainWindow::slot_CM_GC_ANGLE                      ()  { Command( CM_GC_ANGLE                       ); }
void MainWindow::slot_CM_RADIUSDIMENSION               ()  { Command( CM_RADIUSDIMENSION                ); }
void MainWindow::slot_CM_EDITMOVE                      ()  { Command( CM_EDITMOVE                       ); }
void MainWindow::slot_CM_EDITPLANEROTATE               ()  { Command( CM_EDITPLANEROTATE                ); }
void MainWindow::slot_CM_EDITLINEROTATE                ()  { Command( CM_EDITLINEROTATE                 ); }
void MainWindow::slot_CM_EDITSCALESELECTED             ()  { Command( CM_EDITSCALESELECTED              ); }
void MainWindow::slot_CM_DELETESELECTED                ()  { Command( CM_DELETESELECTED                 ); }
void MainWindow::slot_CM_REBUILD_ITEM                  ()  { Command( CM_REBUILD_ITEM                   ); }
void MainWindow::slot_CM_TRANSLATETOBASIS              ()  { Command( CM_TRANSLATETOBASIS               ); }
void MainWindow::slot_CM_TRANSLATETOINGOT              ()  { Command( CM_TRANSLATETOINGOT               ); }
void MainWindow::slot_CM_MESH_TO_BREP                  ()  { Command( CM_MESH_TO_BREP                   ); }
void MainWindow::slot_CM_ADDBASISPOINTS                ()  { Command( CM_ADDBASISPOINTS                 ); }
void MainWindow::slot_CM_EDITPROPERTY                  ()  { Command( CM_EDITPROPERTY                   ); }
void MainWindow::slot_CM_MOVEOBJECT                    ()  { Command( CM_MOVEOBJECT                     ); }
void MainWindow::slot_CM_PLANEROTATEOBJECT             ()  { Command( CM_PLANEROTATEOBJECT              ); }
void MainWindow::slot_CM_LINEROTATEOBJECT              ()  { Command( CM_LINEROTATEOBJECT               ); }
void MainWindow::slot_CM_SCALEOBJECT                   ()  { Command( CM_SCALEOBJECT                    ); }
void MainWindow::slot_CM_DELETEOBJECT                  ()  { Command( CM_DELETEOBJECT                   ); }
void MainWindow::slot_CM_MOVECPLANE                    ()  { Command( CM_MOVECPLANE                     ); }
void MainWindow::slot_CM_ROTATEPLANEINVIEW             ()  { Command( CM_ROTATEPLANEINVIEW              ); }
void MainWindow::slot_CM_DELETECONSTRUCTIVEPLANE       ()  { Command( CM_DELETECONSTRUCTIVEPLANE        ); }
void MainWindow::slot_CM_SPLINESETCLOSED               ()  { Command( CM_SPLINESETCLOSED                ); }
void MainWindow::slot_CM_SPLINEINVERSE                 ()  { Command( CM_SPLINEINVERSE                  ); }
void MainWindow::slot_CM_DELETEPART                    ()  { Command( CM_DELETEPART                     ); }
void MainWindow::slot_CM_DELETEPARTP1P2                ()  { Command( CM_DELETEPARTP1P2                 ); }
void MainWindow::slot_CM_NEWTRIMMEDCURVE               ()  { Command( CM_NEWTRIMMEDCURVE                ); }
void MainWindow::slot_CM_BREAKCURVE                    ()  { Command( CM_BREAKCURVE                     ); }
void MainWindow::slot_CM_FILLETCURVES                  ()  { Command( CM_FILLETCURVES                   ); }
void MainWindow::slot_CM_CHAMFERCURVES                 ()  { Command( CM_CHAMFERCURVES                  ); }
void MainWindow::slot_CM_SPLINESET_WEIGHTS             ()  { Command( CM_SPLINESET_WEIGHTS              ); }
void MainWindow::slot_CM_SPLINENEW_POINT               ()  { Command( CM_SPLINENEW_POINT                ); }
void MainWindow::slot_CM_SPLINETANGENT                 ()  { Command( CM_SPLINETANGENT                  ); }
void MainWindow::slot_CM_SPLINETRIMM                   ()  { Command( CM_SPLINETRIMM                    ); }
void MainWindow::slot_CM_SPLINECHANGEPOINT             ()  { Command( CM_SPLINECHANGEPOINT              ); }
void MainWindow::slot_CM_SETDEGREE                     ()  { Command( CM_SETDEGREE                      ); }
void MainWindow::slot_CM_SPLINEINSERTKNOT              ()  { Command( CM_SPLINEINSERTKNOT               ); }
void MainWindow::slot_CM_APPARCCURVE                   ()  { Command( CM_APPARCCURVE                    ); }
void MainWindow::slot_CM_TRANSLATECURVETOCURVE3D       ()  { Command( CM_TRANSLATECURVETOCURVE3D        ); }
void MainWindow::slot_CM_TRANSLATECURVETOCONTOURON     ()  { Command( CM_TRANSLATECURVETOCONTOURON      ); }
void MainWindow::slot_CM_CONVERTPLANENURBSINTOOPEN     ()  { Command( CM_CONVERTPLANENURBSINTOOPEN      ); }
void MainWindow::slot_CM_CONVERTPLANENURBSINTOCLOSE    ()  { Command( CM_CONVERTPLANENURBSINTOCLOSE     ); }
void MainWindow::slot_CM_NORMALIZEOBJECT               ()  { Command( CM_NORMALIZEOBJECT                ); }
void MainWindow::slot_CM_CONTOURSENSE                  ()  { Command( CM_CONTOURSENSE                   ); }
void MainWindow::slot_CM_DISASSEMBLY                   ()  { Command( CM_DISASSEMBLY                    ); }
void MainWindow::slot_CM_TRANSLATECONTOURTOCONTOURON   ()  { Command( CM_TRANSLATECONTOURTOCONTOURON    ); }
void MainWindow::slot_CM_ADDINVISIBLEPART              ()  { Command( CM_ADDINVISIBLEPART               ); }
void MainWindow::slot_CM_REPLACEMLTLINESBASISCURVE     ()  { Command( CM_REPLACEMLTLINESBASISCURVE      ); }
void MainWindow::slot_CM_INSERTMLTLINESVERTEX          ()  { Command( CM_INSERTMLTLINESVERTEX           ); }
void MainWindow::slot_CM_REMOVEMLTLINESVERTEX          ()  { Command( CM_REMOVEMLTLINESVERTEX           ); }
void MainWindow::slot_CM_SETMLTLINESHOTPOINT           ()  { Command( CM_SETMLTLINESHOTPOINT            ); }
void MainWindow::slot_CM_DELMLTLINESHOTPOINT           ()  { Command( CM_DELMLTLINESHOTPOINT            ); }
void MainWindow::slot_CM_SETMLTLINESVERTSMOOTHJOINT    ()  { Command( CM_SETMLTLINESVERTSMOOTHJOINT     ); }
void MainWindow::slot_CM_SETMLTLINESVERTTRACINGTYPE    ()  { Command( CM_SETMLTLINESVERTTRACINGTYPE     ); }
void MainWindow::slot_CM_SETMLTLINESVERTSPECFILLETRAD  ()  { Command( CM_SETMLTLINESVERTSPECFILLETRAD   ); }
void MainWindow::slot_CM_SETMLTLINESVERTTIPTYPE        ()  { Command( CM_SETMLTLINESVERTTIPTYPE         ); }
void MainWindow::slot_CM_SETMLTLINESVERTTIPDIR         ()  { Command( CM_SETMLTLINESVERTTIPDIR          ); }
void MainWindow::slot_CM_SETMLTLINESRADIUS             ()  { Command( CM_SETMLTLINESRADIUS              ); }
void MainWindow::slot_CM_CHANGEMLTLINESRADIUS          ()  { Command( CM_CHANGEMLTLINESRADIUS           ); }
void MainWindow::slot_CM_ADDMLTLINESRADIUS             ()  { Command( CM_ADDMLTLINESRADIUS              ); }
void MainWindow::slot_CM_REMOVEMLTLINESRADIUS          ()  { Command( CM_REMOVEMLTLINESRADIUS           ); }
void MainWindow::slot_CM_SETMLTLINESBEGTIPTYPE         ()  { Command( CM_SETMLTLINESBEGTIPTYPE          ); }
void MainWindow::slot_CM_SETMLTLINESBEGTIPPARAM        ()  { Command( CM_SETMLTLINESBEGTIPPARAM         ); }
void MainWindow::slot_CM_SETMLTLINESENDTIPTYPE         ()  { Command( CM_SETMLTLINESENDTIPTYPE          ); }
void MainWindow::slot_CM_SETMLTLINESENDTIPPARAM        ()  { Command( CM_SETMLTLINESENDTIPPARAM         ); }
void MainWindow::slot_CM_SETMLTLINESPROCESSCLOSED      ()  { Command( CM_SETMLTLINESPROCESSCLOSED       ); }
void MainWindow::slot_CM_FILLETMLTLINESBASISCURVE      ()  { Command( CM_FILLETMLTLINESBASISCURVE       ); }
void MainWindow::slot_CM_CHAMFERMLTLINESBASISCURVE     ()  { Command( CM_CHAMFERMLTLINESBASISCURVE      ); }
void MainWindow::slot_CM_BREAKMLTLINE                  ()  { Command( CM_BREAKMLTLINE                   ); }
void MainWindow::slot_CM_BREAKMLTLINENPARTS            ()  { Command( CM_BREAKMLTLINENPARTS             ); }
void MainWindow::slot_CM_DELMLTLINESLASTSEG            ()  { Command( CM_DELMLTLINESLASTSEG             ); }
void MainWindow::slot_CM_DELETEPARTMLTLINEP1P2         ()  { Command( CM_DELETEPARTMLTLINEP1P2          ); }
void MainWindow::slot_CM_MOVEPOINTALONGLINE            ()  { Command( CM_MOVEPOINTALONGLINE             ); }
void MainWindow::slot_CM_CURVESETCLOSED                ()  { Command( CM_CURVESETCLOSED                 ); }
void MainWindow::slot_CM_CURVEINVERSE                  ()  { Command( CM_CURVEINVERSE                   ); }
void MainWindow::slot_CM_DELETEMIDLEPART               ()  { Command( CM_DELETEMIDLEPART                ); }
void MainWindow::slot_CM_CREATEMIDLEPART               ()  { Command( CM_CREATEMIDLEPART                ); }
void MainWindow::slot_CM_CREATEBREAKPARTS              ()  { Command( CM_CREATEBREAKPARTS               ); }
void MainWindow::slot_CM_MOVECURVEPOINT                ()  { Command( CM_MOVECURVEPOINT                 ); }
void MainWindow::slot_CM_CURVEADDPOINT                 ()  { Command( CM_CURVEADDPOINT                  ); }
void MainWindow::slot_CM_CONVERTNURBSINTOOPEN          ()  { Command( CM_CONVERTNURBSINTOOPEN           ); }
void MainWindow::slot_CM_CONVERTNURBSINTOCLOSE         ()  { Command( CM_CONVERTNURBSINTOCLOSE          ); }
void MainWindow::slot_CM_ATTACHNURBSTOOBJECT           ()  { Command( CM_ATTACHNURBSTOOBJECT            ); }
void MainWindow::slot_CM_TRANSLATECURVETOBASIS         ()  { Command( CM_TRANSLATECURVETOBASIS          ); }
void MainWindow::slot_CM_TRANSLATECURVE3DTOCURVE       ()  { Command( CM_TRANSLATECURVE3DTOCURVE        ); }
void MainWindow::slot_CM_CURVEADDKNOTE                 ()  { Command( CM_CURVEADDKNOTE                  ); }
void MainWindow::slot_CM_CURVEREMOVEKNOTE              ()  { Command( CM_CURVEREMOVEKNOTE               ); }
void MainWindow::slot_CM_REDUCE_NURBS_DEGREE           ()  { Command( CM_REDUCE_NURBS_DEGREE            ); }
void MainWindow::slot_CM_GOSURFACETHROUGHPLANE         ()  { Command( CM_GOSURFACETHROUGHPLANE          ); }
void MainWindow::slot_CM_GOSURFACETHROUGHPOINT         ()  { Command( CM_GOSURFACETHROUGHPOINT          ); }
void MainWindow::slot_CM_CONVERTNURBSSURFACEINTOOPEN   ()  { Command( CM_CONVERTNURBSSURFACEINTOOPEN    ); }
void MainWindow::slot_CM_CONVERTNURBSSURFACEINTOCLOSE  ()  { Command( CM_CONVERTNURBSSURFACEINTOCLOSE   ); }
void MainWindow::slot_CM_TRANSLATESURFACETOBASIS       ()  { Command( CM_TRANSLATESURFACETOBASIS        ); }
void MainWindow::slot_CM_REBUILD_WIRE_FRAME            ()  { Command( CM_REBUILD_WIRE_FRAME             ); }
void MainWindow::slot_CM_REBUILD_ALL_WIRE_FRAMES       ()  { Command( CM_REBUILD_ALL_WIRE_FRAMES        ); }
void MainWindow::slot_CM_REBUILD_SHELL                 ()  { Command( CM_REBUILD_SHELL                  ); }
void MainWindow::slot_CM_REBUILD_SHELL_WITHCOPYMODE    ()  { Command( CM_REBUILD_SHELL_WITHCOPYMODE     ); }
void MainWindow::slot_CM_TRANSLATESOLIDTOBASIS         ()  { Command( CM_TRANSLATESOLIDTOBASIS          ); }
void MainWindow::slot_CM_TRANSLATESOLIDTOSURFACES      ()  { Command( CM_TRANSLATESOLIDTOSURFACES       ); }
void MainWindow::slot_CM_BREAKSOLIDTOPARTS             ()  { Command( CM_BREAKSOLIDTOPARTS              ); }
void MainWindow::slot_CM_DELETE_ITEM_CREATORS          ()  { Command( CM_DELETE_ITEM_CREATORS           ); }
void MainWindow::slot_CM_BREAKSOLIDONPARTS             ()  { Command( CM_BREAKSOLIDONPARTS              ); }
void MainWindow::slot_CM_UNIFYOWNCOMPLANARFACES        ()  { Command( CM_UNIFYOWNCOMPLANARFACES         ); }
void MainWindow::slot_CM_SOLIDENDMOVE                  ()  { Command( CM_SOLIDENDMOVE                   ); }
void MainWindow::slot_CM_SOLIDFINISH                   ()  { Command( CM_SOLIDFINISH                    ); }
void MainWindow::slot_CM_SOLIDENDCUT                   ()  { Command( CM_SOLIDENDCUT                    ); }
void MainWindow::slot_CM_SOLIDCREATORDELETE            ()  { Command( CM_SOLIDCREATORDELETE             ); }
void MainWindow::slot_CM_BEND                          ()  { Command( CM_BEND                           ); }
void MainWindow::slot_CM_UNBEND                        ()  { Command( CM_UNBEND                         ); }
void MainWindow::slot_CM_ADD_ATTTRIBUTE                ()  { Command( CM_ADD_ATTTRIBUTE                 ); }
void MainWindow::slot_CM_MODTHREAD_ISMATEDTHREADS      ()  { Command( CM_MODTHREAD_ISMATEDTHREADS       ); }
void MainWindow::slot_CM_MODTHREAD_ISBODYTHREAD        ()  { Command( CM_MODTHREAD_ISBODYTHREAD         ); }
void MainWindow::slot_CM_MODTHREAD_ADAPTTOBODY         ()  { Command( CM_MODTHREAD_ADAPTTOBODY          ); }
void MainWindow::slot_CM_EDITSELECTALL                 ()  { Command( CM_EDITSELECTALL                  ); }
void MainWindow::slot_CM_EDITUNSELECTALL               ()  { Command( CM_EDITUNSELECTALL                ); }
void MainWindow::slot_CM_EDITREVERSSELECTALL           ()  { Command( CM_EDITREVERSSELECTALL            ); }
void MainWindow::slot_CM_SELECTBYCPLANE                ()  { Command( CM_SELECTBYCPLANE                 ); }
void MainWindow::slot_CM_SELECTALLPOINTS               ()  { Command( CM_SELECTALLPOINTS                ); }
void MainWindow::slot_CM_SELECTALLCURVES               ()  { Command( CM_SELECTALLCURVES                ); }
void MainWindow::slot_CM_SELECTLINE                    ()  { Command( CM_SELECTLINE                     ); }
void MainWindow::slot_CM_SELECTPOLYCURVE               ()  { Command( CM_SELECTPOLYCURVE                ); }
void MainWindow::slot_CM_SELECTINTERSECTIONCURVE       ()  { Command( CM_SELECTINTERSECTIONCURVE        ); }
void MainWindow::slot_CM_SELECTCONTOURS2D              ()  { Command( CM_SELECTCONTOURS2D               ); }
void MainWindow::slot_CM_SELECTCONTOURS3D              ()  { Command( CM_SELECTCONTOURS3D               ); }
void MainWindow::slot_CM_SELECTCONTOURSONSURFACE       ()  { Command( CM_SELECTCONTOURSONSURFACE        ); }
void MainWindow::slot_CM_SELECTALLSURFACES             ()  { Command( CM_SELECTALLSURFACES              ); }
void MainWindow::slot_CM_SELECTPLANE                   ()  { Command( CM_SELECTPLANE                    ); }
void MainWindow::slot_CM_SELECTELEMENTARYSURFACE       ()  { Command( CM_SELECTELEMENTARYSURFACE        ); }
void MainWindow::slot_CM_SELECTSWEPTSURFACE            ()  { Command( CM_SELECTSWEPTSURFACE             ); }
void MainWindow::slot_CM_SELECTPOLYSURFACE             ()  { Command( CM_SELECTPOLYSURFACE              ); }
void MainWindow::slot_CM_SELECTOFFSETSURFACE           ()  { Command( CM_SELECTOFFSETSURFACE            ); }
void MainWindow::slot_CM_SELECTCURVEBOUNDEDSURFACE     ()  { Command( CM_SELECTCURVEBOUNDEDSURFACE      ); }
void MainWindow::slot_CM_SELECT_ALL_SOLIDS             ()  { Command( CM_SELECT_ALL_SOLIDS              ); }
void MainWindow::slot_CM_SELECT_CLOSED_SOLIDS          ()  { Command( CM_SELECT_CLOSED_SOLIDS           ); }
void MainWindow::slot_CM_SELECT_UNCLOSED_SOLIDS        ()  { Command( CM_SELECT_UNCLOSED_SOLIDS         ); }
void MainWindow::slot_CM_CUTMODELOBJECTS               ()  { Command( CM_CUTMODELOBJECTS                ); }
void MainWindow::slot_CM_EDITREDRAW                    ()  { Command( CM_EDITREDRAW                     ); }
void MainWindow::slot_CM_CURVEGABARIT                  ()  { Command( CM_CURVEGABARIT                   ); }
void MainWindow::slot_CM_SURFACEGABARIT                ()  { Command( CM_SURFACEGABARIT                 ); }
void MainWindow::slot_CM_FACEGABARIT                   ()  { Command( CM_FACEGABARIT                    ); }
void MainWindow::slot_CM_SOLIDGABARIT                  ()  { Command( CM_SOLIDGABARIT                   ); }
void MainWindow::slot_CM_LOCALGABARIT_CURVE2D          ()  { Command( CM_LOCALGABARIT_CURVE2D           ); }
void MainWindow::slot_CM_LOCALGABARIT_CURVE3D          ()  { Command( CM_LOCALGABARIT_CURVE3D           ); }
void MainWindow::slot_CM_LOCALGABARIT_SURFACE          ()  { Command( CM_LOCALGABARIT_SURFACE           ); }
void MainWindow::slot_CM_LOCALGABARIT_EDGE             ()  { Command( CM_LOCALGABARIT_EDGE              ); }
void MainWindow::slot_CM_LOCALGABARIT_FACE             ()  { Command( CM_LOCALGABARIT_FACE              ); }
void MainWindow::slot_CM_LOCALGABARIT_SOLID            ()  { Command( CM_LOCALGABARIT_SOLID             ); }
void MainWindow::slot_CM_FLOOD_FILL_VOLUME             ()  { Command( CM_FLOOD_FILL_VOLUME              ); }
void MainWindow::slot_CM_SURF_NORMALS                  ()  { Command( CM_SURF_NORMALS                   ); }
void MainWindow::slot_CM_FACE_NORMALS                  ()  { Command( CM_FACE_NORMALS                   ); }
void MainWindow::slot_CM_MESH_NORMALS                  ()  { Command( CM_MESH_NORMALS                   ); }
void MainWindow::slot_CM_SURFACEHIDE                   ()  { Command( CM_SURFACEHIDE                    ); }
void MainWindow::slot_CM_FACEHIDE                      ()  { Command( CM_FACEHIDE                       ); }
void MainWindow::slot_CM_SOLIDHIDE                     ()  { Command( CM_SOLIDHIDE                      ); }
void MainWindow::slot_CM_TRIANGULATESURFACE            ()  { Command( CM_TRIANGULATESURFACE             ); }
void MainWindow::slot_CM_TRIANGULATESOLID              ()  { Command( CM_TRIANGULATESOLID               ); }
void MainWindow::slot_CM_TRIANGULATESOLIDS             ()  { Command( CM_TRIANGULATESOLIDS              ); }
void MainWindow::slot_CM_TRIANGULATEFACE               ()  { Command( CM_TRIANGULATEFACE                ); }
void MainWindow::slot_CM_TRIANGULAREREGION             ()  { Command( CM_TRIANGULAREREGION              ); }
void MainWindow::slot_CM_SIMPLIFY_TRGN_CONVEX_HULL     ()  { Command( CM_SIMPLIFY_TRGN_CONVEX_HULL      ); }
void MainWindow::slot_CM_CURVEMAP                      ()  { Command( CM_CURVEMAP                       ); }
void MainWindow::slot_CM_SURFACEMAP                    ()  { Command( CM_SURFACEMAP                     ); }
void MainWindow::slot_CM_SOLIDMAP                      ()  { Command( CM_SOLIDMAP                       ); }
void MainWindow::slot_CM_SOLIDFACEMAP                  ()  { Command( CM_SOLIDFACEMAP                   ); }
void MainWindow::slot_CM_PARAMTRIANGULATEFACE          ()  { Command( CM_PARAMTRIANGULATEFACE           ); }
void MainWindow::slot_CM_CURVETESTA                    ()  { Command( CM_CURVETESTA                     ); }
void MainWindow::slot_CM_SURFACETESTA                  ()  { Command( CM_SURFACETESTA                   ); }
void MainWindow::slot_CM_MAKESPACEWIREFRAME            ()  { Command( CM_MAKESPACEWIREFRAME             ); }
void MainWindow::slot_CM_MAKEPLANEWIREFRAME            ()  { Command( CM_MAKEPLANEWIREFRAME             ); }
void MainWindow::slot_CM_MAKEWIREFRAMEONPLANE          ()  { Command( CM_MAKEWIREFRAMEONPLANE           ); }
void MainWindow::slot_CM_MAKETRIANGLEPOINTFRAME        ()  { Command( CM_MAKETRIANGLEPOINTFRAME         ); }
void MainWindow::slot_CM_MAKETRIANGLEWIREFRAME         ()  { Command( CM_MAKETRIANGLEWIREFRAME          ); }
void MainWindow::slot_CM_MAKETRIANGLEPLANE             ()  { Command( CM_MAKETRIANGLEPLANE              ); }
void MainWindow::slot_CM_CURVECURVATURE                ()  { Command( CM_CURVECURVATURE                 ); }
void MainWindow::slot_CM_CURVETORSION                  ()  { Command( CM_CURVETORSION                   ); }
void MainWindow::slot_CM_GETSOLIDMASSINERTIA           ()  { Command( CM_GETSOLIDMASSINERTIA            ); }
void MainWindow::slot_CM_COLLISION_CONSTRUCT_BVT_NODES ()  { Command( CM_COLLISION_CONSTRUCT_BVT_NODES  ); }
void MainWindow::slot_CM_COLLISION_DISTANCE_QUERY      ()  { Command( CM_COLLISION_DISTANCE_QUERY       ); }
void MainWindow::slot_CM_COLLISION_INTERFERENCE_SOLIDS ()  { Command( CM_COLLISION_INTERFERENCE_SOLIDS  ); }
void MainWindow::slot_CM_COLLISION_INTERFERENCE_MESHES ()  { Command( CM_COLLISION_INTERFERENCE_MESHES  ); }
void MainWindow::slot_CM_CROSS_BOOLEAN                 ()  { Command( CM_CROSS_BOOLEAN                  ); }
void MainWindow::slot_CM_FIND_TOUCHED_FACES            ()  { Command( CM_FIND_TOUCHED_FACES             ); }
void MainWindow::slot_CM_SPLIT_TOUCHED_FACES           ()  { Command( CM_SPLIT_TOUCHED_FACES            ); }
void MainWindow::slot_CM_MERGE_SOLIDS                  ()  { Command( CM_MERGE_SOLIDS                   ); }
void MainWindow::slot_CM_GETCONTOURAREA                ()  { Command( CM_GETCONTOURAREA                 ); }
void MainWindow::slot_CM_GETCURVELENGTH                ()  { Command( CM_GETCURVELENGTH                 ); }
void MainWindow::slot_CM_GETCURVERADIUS                ()  { Command( CM_GETCURVERADIUS                 ); }
void MainWindow::slot_CM_GETEDGEFACESANGLE             ()  { Command( CM_GETEDGEFACESANGLE              ); }
void MainWindow::slot_CM_GETEDGEEDGEANGLE              ()  { Command( CM_GETEDGEEDGEANGLE               ); }
void MainWindow::slot_CM_POINTDISTANCETOPOINT          ()  { Command( CM_POINTDISTANCETOPOINT           ); }
void MainWindow::slot_CM_GETDISTANCETOPOINT            ()  { Command( CM_GETDISTANCETOPOINT             ); }
void MainWindow::slot_CM_GEOMITEMS_MINDISTANCE         ()  { Command( CM_GEOMITEMS_MINDISTANCE          ); }
void MainWindow::slot_CM_TOPOITEMS_MINDISTANCE         ()  { Command( CM_TOPOITEMS_MINDISTANCE          ); }
void MainWindow::slot_CM_SURFAXESDISTANCE              ()  { Command( CM_SURFAXESDISTANCE               ); }
void MainWindow::slot_CM_GEOMITEMS_MAXDISTANCE         ()  { Command( CM_GEOMITEMS_MAXDISTANCE          ); }
void MainWindow::slot_CM_TOPOITEMS_MAXDISTANCE         ()  { Command( CM_TOPOITEMS_MAXDISTANCE          ); }
void MainWindow::slot_CM_GRIDDED_SURF_MINMAXDISTANCES  ()  { Command( CM_GRIDDED_SURF_MINMAXDISTANCES   ); }
void MainWindow::slot_CM_SURF_MAX_MIN_CURVATURES       ()  { Command( CM_SURF_MAX_MIN_CURVATURES        ); }
void MainWindow::slot_CM_FACES_MAX_MIN_CURVATURES      ()  { Command( CM_FACES_MAX_MIN_CURVATURES       ); }
void MainWindow::slot_CM_GETFACECENTRE                 ()  { Command( CM_GETFACECENTRE                  ); }
void MainWindow::slot_CM_CHECK_SHELL_GEOMETRY          ()  { Command( CM_CHECK_SHELL_GEOMETRY           ); }
void MainWindow::slot_CM_CHECK_SHELL_NAMES             ()  { Command( CM_CHECK_SHELL_NAMES              ); }
void MainWindow::slot_CM_CHECKIDENTICALBASESURFACES    ()  { Command( CM_CHECKIDENTICALBASESURFACES     ); }
void MainWindow::slot_CM_CHECK_SOLIDS_INTERSECT        ()  { Command( CM_CHECK_SOLIDS_INTERSECT         ); }
void MainWindow::slot_CM_CHECK_CONVEX_HULL_INTERSECT   ()  { Command( CM_CHECK_CONVEX_HULL_INTERSECT    ); }
void MainWindow::slot_CM_FIND_OVERLAPPING_SIMILAR_FACES()  { Command( CM_FIND_OVERLAPPING_SIMILAR_FACES ); }
void MainWindow::slot_CM_FIND_FACES_EDGES_CARRIERS     ()  { Command( CM_FIND_FACES_EDGES_CARRIERS      ); }
void MainWindow::slot_CM_SHELL_REVERSE                 ()  { Command( CM_SHELL_REVERSE                  ); }
void MainWindow::slot_CM_DECOMPOSE_ALL_ASSEMBLIES      ()  { Command( CM_DECOMPOSE_ALL_ASSEMBLIES       ); }
void MainWindow::slot_CM_CONVERT_TO_MESH               ()  { Command( CM_CONVERT_TO_MESH                ); }
void MainWindow::slot_CM_SET_SOLID_LAST_VERSION        ()  { Command( CM_SET_SOLID_LAST_VERSION         ); }
void MainWindow::slot_CM_ADD_VERTEXEDGES_AS_CURVES     ()  { Command( CM_ADD_VERTEXEDGES_AS_CURVES      ); }
void MainWindow::slot_CM_ADD_SOLIDEDGES_AS_CURVES      ()  { Command( CM_ADD_SOLIDEDGES_AS_CURVES       ); }
void MainWindow::slot_CM_ADD_SOLIDVERTS_AS_POINTS      ()  { Command( CM_ADD_SOLIDVERTS_AS_POINTS       ); }
void MainWindow::slot_CM_RECOGNITIONSPIRALSURFACES     ()  { Command( CM_RECOGNITIONSPIRALSURFACES      ); }
void MainWindow::slot_CM_CHECK_CURVE2D_SPEED           ()  { Command( CM_CHECK_CURVE2D_SPEED            ); }
void MainWindow::slot_CM_CHECK_CURVE3D_SPEED           ()  { Command( CM_CHECK_CURVE3D_SPEED            ); }
void MainWindow::slot_CM_CHECK_SURFACE_SPEED           ()  { Command( CM_CHECK_SURFACE_SPEED            ); }
void MainWindow::slot_CM_CREATE_SURFACE_OPER_TESS      ()  { Command( CM_CREATE_SURFACE_OPER_TESS       ); }
void MainWindow::slot_CM_CHECK_CURVES_TANGENT_MATING   ()  { Command( CM_CHECK_CURVES_TANGENT_MATING    ); }
void MainWindow::slot_CM_CHECK_CURVE2D_SELFINT         ()  { Command( CM_CHECK_CURVE2D_SELFINT          ); }
void MainWindow::slot_CM_SHOW_CURVE_DIRECTION          ()  { Command( CM_SHOW_CURVE_DIRECTION           ); }
void MainWindow::slot_CM_COMMON_MATES_TEST             ()  { Command( CM_COMMON_MATES_TEST              ); }
void MainWindow::slot_CM_MATE_CUBE                     ()  { Command( CM_MATE_CUBE                      ); }
void MainWindow::slot_CM_MATE_FRACTAL                  ()  { Command( CM_MATE_FRACTAL                   ); }
void MainWindow::slot_CM_MATE_DISTRIBUTED_DOF          ()  { Command( CM_MATE_DISTRIBUTED_DOF           ); }
void MainWindow::slot_CM_LINEAR_PATTERN                ()  { Command( CM_LINEAR_PATTERN                 ); }
void MainWindow::slot_CM_ANGULAR_PATTERN               ()  { Command( CM_ANGULAR_PATTERN                ); }
void MainWindow::slot_CM_GETEDGENUMBER_INFACE          ()  { Command( CM_GETEDGENUMBER_INFACE           ); }
void MainWindow::slot_CM_GETFACENUMBER                 ()  { Command( CM_GETFACENUMBER                  ); }
void MainWindow::slot_CM_GETEDGENUMBER                 ()  { Command( CM_GETEDGENUMBER                  ); }
void MainWindow::slot_CM_GETFACEBYNUMBER               ()  { Command( CM_GETFACEBYNUMBER                ); }
void MainWindow::slot_CM_GETEDGEBYNUMBER               ()  { Command( CM_GETEDGEBYNUMBER                ); }
void MainWindow::slot_CM_RENDERING                     ()  { Command( CM_RENDERING                      ); }
void MainWindow::slot_CM_SURFACERENDERING              ()  { Command( CM_SURFACERENDERING               ); }
void MainWindow::slot_CM_SOLIDRENDERING                ()  { Command( CM_SOLIDRENDERING                 ); }
void MainWindow::slot_CM_MANUFACTURING                 ()  { Command( CM_MANUFACTURING                  ); }
void MainWindow::slot_CM_SETVIEWVECTOR                 ()  { Command( CM_SETVIEWVECTOR                  ); }
void MainWindow::slot_CM_PRINTPLOTT                    ()  { Command( CM_PRINTPLOTT                     ); }
void MainWindow::slot_CM_PRINTBITMAP                   ()  { Command( CM_PRINTBITMAP                    ); }
void MainWindow::slot_CM_SETOBJECTCOLOR                ()  { Command( CM_SETOBJECTCOLOR                 ); }
void MainWindow::slot_CM_SETCONNECTIONCOLOR            ()  { Command( CM_SETCONNECTIONCOLOR             ); }
void MainWindow::slot_CM_SETCOLORSELECTED              ()  { Command( CM_SETCOLORSELECTED               ); }
void MainWindow::slot_CM_SETCOLORSELECTEDCONN          ()  { Command( CM_SETCOLORSELECTEDCONN           ); }
void MainWindow::slot_CM_SETBACKGRCOLOR                ()  { Command( CM_SETBACKGRCOLOR                 ); }
void MainWindow::slot_CM_GLRENDERCOLOR                 ()  { Command( CM_GLRENDERCOLOR                  ); }
void MainWindow::slot_CM_SETSELECTCOLOR                ()  { Command( CM_SETSELECTCOLOR                 ); }
void MainWindow::slot_CM_SETMAKEGRID                   ()  { Command( CM_SETMAKEGRID                    ); }
void MainWindow::slot_CM_SET_OGL_VIEWTYPE              ()  { Command( CM_SET_OGL_VIEWTYPE               ); }
void MainWindow::slot_CM_SETSTYLEWIDTH                 ()  { Command( CM_SETSTYLEWIDTH                  ); }
void MainWindow::slot_CM_SETOBJECTWIDTH                ()  { Command( CM_SETOBJECTWIDTH                 ); }
void MainWindow::slot_CM_ITEMSMERGING                  ()  { Command( CM_ITEMSMERGING                   ); }
void MainWindow::slot_CM_EDIT_SAME_CREATORS            ()  { Command( CM_EDIT_SAME_CREATORS             ); }
void MainWindow::slot_CM_UVMESHCOUNT                   ()  { Command( CM_UVMESHCOUNT                    ); }
void MainWindow::slot_CM_GLPROPERT                     ()  { Command( CM_GLPROPERT                      ); }
void MainWindow::slot_CM_GLAMBIENT                     ()  { Command( CM_GLAMBIENT                      ); }
void MainWindow::slot_CM_GLDIFFUSE                     ()  { Command( CM_GLDIFFUSE                      ); }
void MainWindow::slot_CM_GLSPECULARITY                 ()  { Command( CM_GLSPECULARITY                  ); }
void MainWindow::slot_CM_GLSHININESS                   ()  { Command( CM_GLSHININESS                    ); }
void MainWindow::slot_CM_GLOPACITY                     ()  { Command( CM_GLOPACITY                      ); }
void MainWindow::slot_CM_GLEMISSION                    ()  { Command( CM_GLEMISSION                     ); }
void MainWindow::slot_CM_SETLIGHTDIRECTION             ()  { Command( CM_SETLIGHTDIRECTION              ); }
void MainWindow::slot_CM_INVISIBLEOBJECT               ()  { Command( CM_INVISIBLEOBJECT                ); }
void MainWindow::slot_CM_VISIBLEALLOBJECTS             ()  { Command( CM_VISIBLEALLOBJECTS              ); }
void MainWindow::slot_CM_VISIBLEPOINTS                 ()  { Command( CM_VISIBLEPOINTS                  ); }
void MainWindow::slot_CM_VISIBLECURVES                 ()  { Command( CM_VISIBLECURVES                  ); }
void MainWindow::slot_CM_VISIBLESURFACES               ()  { Command( CM_VISIBLESURFACES                ); }
void MainWindow::slot_CM_VISIBLEBODIES                 ()  { Command( CM_VISIBLEBODIES                  ); }
void MainWindow::slot_CM_SETDEVIATIONSAG               ()  { Command( CM_SETDEVIATIONSAG                ); }
void MainWindow::slot_CM_SETVISUALSAG                  ()  { Command( CM_SETVISUALSAG                   ); }
void MainWindow::slot_CM_DELTA_RANGE                   ()  { Command( CM_DELTA_RANGE                    ); }
void MainWindow::slot_CM_DELTA_ANGLE                   ()  { Command( CM_DELTA_ANGLE                    ); }
void MainWindow::slot_CM_SETCURSORSTEP                 ()  { Command( CM_SETCURSORSTEP                  ); }
void MainWindow::slot_CM_REBUILDTIMERESULTFILE         ()  { Command( CM_REBUILDTIMERESULTFILE          ); }
void MainWindow::slot_CM_SETCURVEDEGREE                ()  { Command( CM_SETCURVEDEGREE                 ); }
void MainWindow::slot_CM_SETSURFACEDEGREE              ()  { Command( CM_SETSURFACEDEGREE               ); }
void MainWindow::slot_CM_SKIP_ASSERTS                  ()  { Command( CM_SKIP_ASSERTS                   ); }
void MainWindow::slot_CM_SWITCH_DEBUG_DRAW             ()  { Command( CM_SWITCH_DEBUG_DRAW              ); }
void MainWindow::slot_CM_SWITCH_MULTITHREADING         ()  { Command( CM_SWITCH_MULTITHREADING          ); }
void MainWindow::slot_CM_APPLY_CONFIG_FILE             ()  { Command( CM_APPLY_CONFIG_FILE              ); }
void MainWindow::slot_CM_VIEWFRONTPROJ                 ()  { Command( CM_VIEWFRONTPROJ                  ); }
void MainWindow::slot_CM_VIEWREARPROJ                  ()  { Command( CM_VIEWREARPROJ                   ); }
void MainWindow::slot_CM_VIEWUPPROJ                    ()  { Command( CM_VIEWUPPROJ                     ); }
void MainWindow::slot_CM_VIEWDOWNPROJ                  ()  { Command( CM_VIEWDOWNPROJ                   ); }
void MainWindow::slot_CM_VIEWLEFTPROJ                  ()  { Command( CM_VIEWLEFTPROJ                   ); }
void MainWindow::slot_CM_VIEWRIGHTPROJ                 ()  { Command( CM_VIEWRIGHTPROJ                  ); }
void MainWindow::slot_CM_VIEWISOPROJ                   ()  { Command( CM_VIEWISOPROJ                    ); }
void MainWindow::slot_CM_SETVIEWPLACEMENT              ()  { Command( CM_SETVIEWPLACEMENT               ); }
void MainWindow::slot_CM_SETPROJECTION                 ()  { Command( CM_SETPROJECTION                  ); }
void MainWindow::slot_CM_SETDEPTH                      ()  { Command( CM_SETDEPTH                       ); }
void MainWindow::slot_CM_CHANGESCALE                   ()  { Command( CM_CHANGESCALE                    ); }
void MainWindow::slot_CM_PTORIGIN                      ()  { Command( CM_PTORIGIN                       ); }
void MainWindow::slot_CM_SETVIEWPOINT                  ()  { Command( CM_SETVIEWPOINT                   ); }
void MainWindow::slot_CM_HELPABOUT                     ()  { Command( CM_HELPABOUT                      ); }
void MainWindow::slot_CM_MAKEUSERCOMAND0               ()  { Command( CM_MAKEUSERCOMAND0                ); }
void MainWindow::slot_CM_MAKEUSERCOMAND1               ()  { Command( CM_MAKEUSERCOMAND1                ); }
void MainWindow::slot_CM_MAKEUSERCOMAND2               ()  { Command( CM_MAKEUSERCOMAND2                ); }
void MainWindow::slot_CM_MAKEUSERCOMAND3               ()  { Command( CM_MAKEUSERCOMAND3                ); }
void MainWindow::slot_CM_MAKEUSERCOMAND4               ()  { Command( CM_MAKEUSERCOMAND4                ); }
void MainWindow::slot_CM_MAKEUSERCOMAND5               ()  { Command( CM_MAKEUSERCOMAND5                ); }
void MainWindow::slot_CM_MAKEUSERCOMAND6               ()  { Command( CM_MAKEUSERCOMAND6                ); }
void MainWindow::slot_CM_MAKEUSERCOMAND7               ()  { Command( CM_MAKEUSERCOMAND7                ); }
void MainWindow::slot_CM_MAKEUSERCOMAND8               ()  { Command( CM_MAKEUSERCOMAND8                ); }
void MainWindow::slot_CM_MAKEUSERCOMAND9               ()  { Command( CM_MAKEUSERCOMAND9                ); }
void MainWindow::slot_CM_SAMPLE_PARAMETRIC_SKETCH      ()  { Command( CM_SAMPLE_PARAMETRIC_SKETCH       ); }
void MainWindow::slot_CM_SAMPLE_WRITE_SKETCH           ()  { Command( CM_SAMPLE_WRITE_SKETCH            ); }
void MainWindow::slot_CM_SAMPLE_READ_SKETCH            ()  { Command( CM_SAMPLE_READ_SKETCH             ); }
void MainWindow::slot_CM_SAMPLE_SPINNING_BLOCK         ()  { Command( CM_SAMPLE_SPINNING_BLOCK          ); }
void MainWindow::slot_CM_SAMPLE_DODECAHEDRON           ()  { Command( CM_SAMPLE_DODECAHEDRON            ); }
void MainWindow::slot_CM_SAMPLE_COLLISION_DETECTION    ()  { Command( CM_SAMPLE_COLLISION_DETECTION     ); }
void MainWindow::slot_CM_SAMPLE_CHECK_COLLISIONS       ()  { Command( CM_SAMPLE_CHECK_COLLISIONS        ); }
void MainWindow::slot_CM_SAMPLE_CHECK_COLLISIONS_GROUPS()  { Command( CM_SAMPLE_CHECK_COLLISIONS_GROUPS ); }
void MainWindow::slot_CM_ENTERLICENSEKEY               ()  { Command( CM_ENTERLICENSEKEY                ); }
void MainWindow::slot_CM_LICENSE_STATUS                ()  { Command( CM_LICENSE_STATUS                 ); }
void MainWindow::slot_CM_DISABLELICENSES               ()  { Command( CM_DISABLELICENSES                ); }
void MainWindow::slot_CM_FITMODELTOWINDOW              ()  { Command( CM_FITMODELTOWINDOW               ); }

//------------------------------------------------------------------------------
//
// ---
void MainWindow::readSettings()
{
    QSettings settings("MainWindow", "TestingKernelC3D");
    QRect geom = QApplication::desktop()->availableGeometry();
    QPoint pos = settings.value("MainWindowPosition", QPoint(200, 200)).toPoint();
    QSize size = settings.value("MainWindowSize", QSize(2 * geom.width() / 3, 2 * geom.height() / 3)).toSize();
    resize(size);
    move(pos);
}

//------------------------------------------------------------------------------
//
// ---
void MainWindow::writeSettings()
{
    QSettings settings("MainWindow", "TestingKernelC3D");
    settings.setValue("MainWindowPosition", pos());
    settings.setValue("MainWindowSize", size());
}


//------------------------------------------------------------------------------
// Создать главное окно задачи
// ---
bool CreateFrameWindow() {
  MainWindow* mainWnd = new MainWindow;
  qMainWnd = (QMainWindow*)mainWnd;

  mainWnd->show();

//  mainWnd->slot_CM_VIEWFRONT();            // PostMessage( hwnd, WM_COMMAND, CM_VIEWFRONT, 0 ); // Вид спереди
//  mainWnd->slot_CM_VIEWISO();              // PostMessage( hwnd, WM_COMMAND, CM_VIEWISO, 0 );   // Изометрия
//  mainWnd->tileSubWindows();               // PostMessage( hwnd, WM_COMMAND, CM_WINDOWTILE, 0 );// Расставить окна "плитками"

  QList<QMdiSubWindow*> wndList = mainWnd->mdiArea->subWindowList();
  for ( int i = 0, count = wndList.count(); i < count; ++i )
  {
    QWidget* wnd = qobject_cast<QWidget *>(wndList[i]);
    mainWnd->setActiveSubWindow( wnd );
    mainWnd->slot_CM_PTORIGIN();
  }
  return true;
}


//------------------------------------------------------------------------------
// Удалить главное окно задачи
// ---
void DestroyFrameWindow() {
  delete qMainWnd;
  qMainWnd = nullptr;
}


//------------------------------------------------------------------------------
// Создать клиенское окно для рисования модели
// ---
QWidget* CreateMdiClient( const TCHAR* title )
{
  if ( qMainWnd )
  {
     MainWindow *main = qobject_cast<MainWindow *>(qMainWnd);
     if ( main )      {
       QWidget* child = main->createMdiChild();
       PRECONDITION( child );
       child->setWindowTitle( QString_fromT(title) );
       child->showMaximized();
       return child;
     }
  }
  return nullptr;
}

//-------------------------------------------------------------------------------
// Запрос у пользователя имени файла для чтения
// ---
bool _GetOpenFileName( TCHAR * nameBuffer, const TCHAR * title, const TCHAR * filter, const TCHAR * extension )
{
  if ( _tcslen(nameBuffer) <= 1 )
    _tcscpy( nameBuffer, ALL_EXT );  // Все файлы

  ::RewriteFileExtension( nameBuffer, extension ); // Перебить или дописать расширение

//  ofn.Flags = OFN_HIDEREADONLY;
  QString fileName = QFileDialog::getOpenFileName( qMainWnd,
                                                   QString_fromT(title),
                                                   QString_fromT(nameBuffer),
                                                   QString_fromT(filter),
                                                   nullptr,
                                                   QFileDialog::HideNameFilterDetails | QFileDialog::DontUseNativeDialog);

  if (!fileName.isEmpty()) {
#ifdef  _UNICODE
    _tcscpy( nameBuffer, fileName.toStdWString().data() );
#else // _UNICODE
    _tcscpy( nameBuffer, fileName.toStdString().data() );
#endif // _UNICODE

    return true;
  }
  return false;
}

#define SAVE_EXT_INFO1 \
  QObject::tr("Models(*.C3D *.c3d);;ACIS(*.SAT *.sat);;IGES(*.IGS *.IGES *.igs *.iges);;PARASOLID(*.X_T *.x_t *.x_b);;STEP(*.STP *.STEP *.stp *.step);;JT(*.JT *.jt);;STL(*.STL *.stl);;WRL(*.WRL *.wrl)")

//-------------------------------------------------------------------------------
// Запрос у пользователя имени файла для записи
// ---
bool _GetSaveFileName( TCHAR * _nameBuffer, const TCHAR * title, const TCHAR * filter, const TCHAR * extension )
{
  if ( _tcslen(_nameBuffer) <= 1 )
    _tcscpy(_nameBuffer, ALL_EXT );  // Все файлы

  ::RewriteFileExtension( _nameBuffer, extension ); // Перебить или дописать расширение

  QString strfilter( SAVE_EXT_INFO1 );
//  ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
  QString fileName = QFileDialog::getSaveFileName( qMainWnd,
                                                   QString_fromT(title),
                                                   QString_fromT(_nameBuffer),
                                                   extension ? QString_fromT(filter) : strfilter );

  if (!fileName.isEmpty()) {
#ifdef  _UNICODE
    _tcscpy( _nameBuffer, fileName.toStdWString().data() );
#else // _UNICODE
    _tcscpy( nameBuffer, fileName.toStdString().data() );
#endif // _UNICODE

   ::RewriteFileExtension( _nameBuffer, extension ); // Перебить или дописать расширение
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
//
// ---
COLORREF GetSystemWindowColor() {
  return RGB(255, 255, 255);
}


//------------------------------------------------------------------------------
// Установить/снять курсор в виде песочных часов
// ---
void SetWaitCursor( bool set ) {
  static int count = 0;
  static QCursor wait( Qt::BusyCursor );

  if ( set )   {
    if ( count == 0 )
      qMainWnd->setCursor( wait );

    ++count;
  }
  else {
    if ( count == 1 )
      qMainWnd->unsetCursor();

    --count;
  }
}


//------------------------------------------------------------------------------
// Отрисовать текст
// ---
void SetStatusBarText( const TCHAR* txt ) {
  QStatusBar* sb = qMainWnd->statusBar();
  if ( sb )
    sb->showMessage( QString_fromT(txt) );
}


//------------------------------------------------------------------------------
// Отрисовать координаты точки
// ---
void SetStatusBarPoint( const MbCartPoint3D& pnt )
{
  QStatusBar* sb = qMainWnd->statusBar();
  if ( sb && viewManager ) {
    TCHAR sX[128];
    _sntprintf( sX, 128, _T("x=%-9.3f  "), pnt.x );

    TCHAR sY[128];
    _sntprintf( sY, 128, _T("y=%-9.3f  "), pnt.y );

    TCHAR sZ[128];
    _sntprintf( sZ, 128, _T("z=%-9.3f  "), pnt.z );

    TCHAR sS[128];
    _sntprintf( sS, 128, _T("M=%-9.3f  "), viewManager->GetWindowScale() );

    QString txt = QString( "%1    %2    %3    %4" )
                       .arg( QString_fromT(sX) )
                       .arg( QString_fromT(sY) )
                       .arg( QString_fromT(sZ) )
                       .arg( QString_fromT(sS) );
    sb->showMessage( txt );
  }
}


//------------------------------------------------------------------------------
// Проверить очередь клавиатуры на нажатие клавиши [ESC]
// ---
bool PeekEscape() {
  // Не реализовано для Qt версии
  return false;
}


//------------------------------------------------------------------------------
// Ожидание нажатия на клавиатуру
// ---
bool WaitingKeyboardEvent() {
  bool isEsc = false;
  MainWindow* mainWnd = qobject_cast<MainWindow*>(qMainWnd);
  if ( mainWnd ) {
    //GetDrawTool().SetWindowFocus( true/*mdiActivate*/ );
    QStatusBar* sb = qMainWnd->statusBar();
    if ( sb ) {
#if defined ( __NATIVE_LANGUAGE__ )
      sb->showMessage( QString_fromT( _T("Нажмите любую клавишу") ) );
#else
      sb->showMessage( QString_fromT( _T("Press any key") ) );
#endif
    }

    mainWnd->ExecuteLoop( eKbdWaiting );
    isEsc = ( getPointResult == 2/*VK_ESC*/ );
  }

  return isEsc;
}


//------------------------------------------------------------------------------
// Синхронизовать индекс активного окна с менеджером окон
// ---
void SyncActiveWindowIndex() {
  MainWindow* mainWnd = qobject_cast<MainWindow*>(qMainWnd);
  if ( mainWnd ) {
    QWidget* wnd = mainWnd->activeMdiChild();

    if ( wnd ) {
      ptrdiff_t index = viewManager->FindWindowIndex( (void*)wnd, false );
      if ( 0 <= index && index < viewManager->GetWindowsCount() )
        viewManager->SetActiveWindow( index );
    }
  }
}


#endif // __USE_QT__
