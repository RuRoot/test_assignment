#ifdef __USE_QT__


#pragma warning(disable: 4127)
#pragma warning(disable: 4512)
#pragma warning(disable: 4100)


#include <QtGui>
#include <QGraphicsLineItem>
#include <qstyleoption.h>
#include <qgraphicssceneevent.h>
#include <test_child_window.h>
#include <test_main_window.h>
#include <test.h>
#include <test_manager.h>
#include <test_window.h>
#include <test_gr_draw.h>
#include <test_variables.h>
#include <test_service.h>
#include <templ_p_array.h>
#include <mb_homogeneous3d.h>
#include <mb_cube.h>
#include <solid.h>
#include <mesh_grid.h>
#include <mesh.h>
#include <space_instance.h>
#include <plane_instance.h>
#include <attr_color.h>
#if defined(C3D_MacOS) // mac os X
  #include <OpenGL/gl.h>
#else
  #include <GL/gl.h>
#endif // mac os X
#include <time.h>
#include <math_namespace.h>
#include <last.h>

using namespace c3d;
using namespace TestVariables;


//------------------------------------------------------------------------------
// Преобразование  Qt::Key в VK_xxx
// ---
static uint toVK( int key )
{
  typedef struct { int Qt_Key; uint VK_Key; } KeyPair ;
  static KeyPair arr[] = { { Qt::Key_Tab,       VK_TAB      },
                           { Qt::Key_Insert,    VK_INSERT   },
                           { Qt::Key_Delete,    VK_DELETE   },
                           { Qt::Key_Up,        VK_UP       },
                           { Qt::Key_Down,      VK_DOWN     },
                           { Qt::Key_Left,      VK_LEFT     },
                           { Qt::Key_Right,     VK_RIGHT    },
                           { Qt::Key_PageDown,  VK_NEXT     },
                           { Qt::Key_PageUp,    VK_PRIOR    },
                           { Qt::Key_End,       VK_END      },
                           { Qt::Key_Home,      VK_HOME     },
                           { Qt::Key_F1,        VK_F1       },
                           { Qt::Key_F2,        VK_F2       },
                           { Qt::Key_F3,        VK_F3       },
                           { Qt::Key_F4,        VK_F4       },
                           { Qt::Key_F5,        VK_F5       },
                           { Qt::Key_F6,        VK_F6       },
                           { Qt::Key_F7,        VK_F7       },
                           { Qt::Key_F8,        VK_F8       },
                           { Qt::Key_F9,        VK_F9       },
                           { Qt::Key_F10,       VK_F10      },
                           { Qt::Key_F11,       VK_F11      },
                           { Qt::Key_F12,       VK_F12      },
                           { Qt::Key_Backspace, VK_BACK     },
                           { Qt::Key_Asterisk,  VK_MULTIPLY },
                           { Qt::Key_Minus,     VK_SUBTRACT },
                           { Qt::Key_Plus,      VK_ADD      },
                           { Qt::Key_Return,    VK_RETURN   },
                           { Qt::Key_Enter,     VK_RETURN   },
                           { Qt::Key_Escape,    VK_ESCAPE   },
                           { 0,                 0           } };

  for ( size_t i = 0; arr[i].Qt_Key != 0; ++i )
    if ( key == arr[i].Qt_Key )
      return arr[i].VK_Key;

  return 0;
}


SelectRectangleItem::SelectRectangleItem()
  : m_colorNormalSelect( QColor(68, 152, 255) )
  , m_colorNormalFillSelect( QColor(193, 222, 244, 130) )
  , m_colorReverselSelect( QColor(20, 182, 68) )
  , m_colorReverseFillSelect( QColor(204, 255, 204, 130) )
{
}

SelectRectangleItem::~SelectRectangleItem()
{
}

void SelectRectangleItem::setCoordinates( const QPoint& begin, const QPoint& end )
{
  m_begin = begin;
  m_end = end;
  setRect( begin.x(), begin.y(), end.x() - begin.x(), end.y() - begin.y() );
}

void SelectRectangleItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
  Direction uDirectionFrame = dir_NormalFrame;

  QPoint beginPoint( m_begin );
  QPoint endPoint( m_end );
  if ( beginPoint.x() > endPoint.x() && beginPoint.y() > endPoint.y() )
  {
    uDirectionFrame = dir_ReverseFrame;
  }
  else if ( beginPoint.x() > endPoint.x() && beginPoint.y() < endPoint.y() )
  {
    uDirectionFrame = dir_ReverseFrame;
  }

  glPushAttrib( GL_ALL_ATTRIB_BITS );
  glDisable( GL_CULL_FACE );

  painter->fillRect( option->rect, uDirectionFrame == dir_NormalFrame ? m_colorNormalFillSelect : m_colorReverseFillSelect );

  QPen penOld = painter->pen();
  painter->setPen( uDirectionFrame == dir_NormalFrame ? m_colorNormalSelect : m_colorReverselSelect );
  painter->drawRect( option->rect.adjusted( 0, 0, -1, -1) );
  painter->setPen( penOld );

  glPopAttrib();
}

////////////////////////////////////////////////////////////////////////////////
//
// Инструмент рисования в конкретном окне
// Ориентирован на вывод с использованием Qt
//
////////////////////////////////////////////////////////////////////////////////
class QtGrDraw : public IGrDraw
{
protected:
  class LineParam {
  protected:
    QPen   pen;
    QPoint p1;
    QPoint p2;
  public:
    LineParam( QPen& _pen, QPoint& _p1, QPoint& _p2 ) : pen( _pen ), p1( _p1 ), p2( _p2 ) {}
    void Draw( QPainter& painter )
    {
      QPen oldPen = painter.pen();
      painter.setPen( pen );
      painter.drawLine( p1, p2 );
      painter.setPen( oldPen );
    }
  };

protected:
  GraphicsView&          mdiChild;
  QPainter*              painter;
  QPoint                 cp;
  QPen                   pen;
  PArray<LineParam>      arr;
  GLuint                 meshModel;    // Display list с данными о модели
  QList<QGraphicsItem *> onScene;

public:
  QtGrDraw( GraphicsView& _mdiChild, WorkWindow& _parent, const MbVector3D & lt )
  : IGrDraw ( _parent, lt )
  , mdiChild      ( _mdiChild )
  , painter       ( nullptr )
  , cp            ( 0, 0 )
  , pen           ( (QRgb)0 )
  , arr           ( 1000, 200 )
  , meshModel     ( 0 )
  {
    _parent.enableDrawRect = false;
    SetLight( lt );
  }

  virtual ~QtGrDraw();

public:
  void SetPainter( QPainter* p ) { painter = p; }
  void CleanItems() {
    for ( QList<QGraphicsItem *>::iterator it = onScene.begin(); it != onScene.end(); ++it ) {
      mdiChild.scene()->removeItem( *it );
      delete *it;
    }
    onScene.clear();
  }


public:

public:
  virtual bool InitializeGL();                                                                              // Инициализация для работы окна в полутоновом режиме
  virtual void SetSizeGL( int cx, int cy, double scale );                                                   // Установить сцену
  virtual void ShowOGL( const MbModel & drawModel, COLORREF select, std::vector< SPtr<MbItem> > * addItems = nullptr ); // Отрисовать через OpenGL
  virtual void RefreshOGL();                                                                                // Обновить данные OpenGL
  virtual void SetGLProperty( const MbModel & drawModel, double la, double ld, double ls, MbVector3D &ll ); // Визуальные свойства
  virtual void DeleteGL( const MbModel & drawModel );                                                       // Удалить данные

private:
  // Создать display list с данными и инициализировать состояние.
  void InitModelList( const MbModel & drawModel, COLORREF select );

public:
  virtual void SelectPen( int cWidth, int R, int G, int B, bool _xor = false );
  virtual void SelectPen( int cStyle, int cWidth, COLORREF c );
  virtual void RestorePen();
  virtual void MoveTo  ( int x, int y );
  virtual void LineTo  ( int x, int y );
  virtual void SetPixel( int x, int y, int R, int G, int B );
  virtual void DPtoMM  ( const POINT& p, MbCartPoint& cp, double scale );
  virtual void MMtoDP  ( const MbCartPoint& cp, POINT& p, double scale );
  virtual void PutRectangle ( POINT& p1, POINT& p2, COLORREF color, bool _xor ); // Нарисовать прямоугольник
  virtual void FillRectangle( RECT* rc = nullptr ); // Залить прямоугольник цветом фона
  virtual void GetClientRect( RECT& r );
  virtual void Invalidate( bool bErase = true );
  virtual void BeginPaint();
  virtual void EndPaint();
  virtual void SetWindowFocus( bool mdiActivate );
  virtual void SetWindowTitle( const TCHAR * ) {}
  virtual void ChangeWindowState();
  virtual bool IsMyWnd( const void * wndId ) const;
  virtual void UpdateWindow(){ mdiChild.viewport()->repaint(); }

private:
  void          operator = ( const QtGrDraw & ); // Не реализовано
};


//------------------------------------------------------------------------------
//
// ---
QtGrDraw::~QtGrDraw()
{
  RefreshOGL();
}


//------------------------------------------------------------------------------
//
// ---
void QtGrDraw::SelectPen( int cWidth, int R, int G, int B, bool _xor )
{
  pen = QPen( QColor(R, G, B) );
  if ( cWidth > 1 )
  {
    pen.setWidth( cWidth );
    pen.setCapStyle( Qt::RoundCap );
  }
}


//------------------------------------------------------------------------------
//
// ---
void QtGrDraw::SelectPen( int cStyle, int cWidth, COLORREF c )
{
  pen = QPen( QColor(GetRValue(c), GetGValue(c), GetBValue(c)) );
  if ( cWidth > 1 )
  {
    pen.setWidth( cWidth );
    pen.setCapStyle( Qt::RoundCap );
  }
}


//------------------------------------------------------------------------------
//
// ---
void QtGrDraw::RestorePen()
{
  // Ничего не делать старые инструменты очистятся в end()
}


//------------------------------------------------------------------------------
//
// ---
void QtGrDraw::MoveTo( int x, int y )
{
  cp.setX(x);
  cp.setY(y);
}


//------------------------------------------------------------------------------
//
// ---
void QtGrDraw::LineTo( int x, int y )
{
  QPoint np(x, y);

  if ( painter == nullptr ) {
    QGraphicsLineItem * qLine = new QGraphicsLineItem ( x, y, cp.x(), cp.y() );
    qLine->setPen( pen );
    onScene.append( qLine );
    mdiChild.scene()->addItem( qLine );
    //return;
  }
  else
//ЭМ  if ( paintCount )
    LineParam( pen, cp, np ).Draw( *painter );
//ЭМ  else
//ЭМ    arr.Add( new LineParam( pen, cp, np ) );

  cp = np;
}


//------------------------------------------------------------------------------
//
// ---
void QtGrDraw::SetPixel( int x, int y, int R, int G, int B )
{
  SelectPen( 1, R, G, B, false/*xor*/ );
  MoveTo( x, y );
  LineTo( x+1, y );
}


//------------------------------------------------------------------------------
//
// ---
void QtGrDraw::DPtoMM( const POINT& p, MbCartPoint& cp, double scale )
{
    cp.Init( ((double)p.x) / (xPixelPerMM * scale), -((double)p.y) / (yPixelPerMM * scale) );
}


//------------------------------------------------------------------------------
//
// ---
void QtGrDraw::MMtoDP( const MbCartPoint& cp, POINT& p, double scale )
{
    p.x = (int32)( cp.x * xPixelPerMM * scale );
    p.y = (int32)(-cp.y * yPixelPerMM * scale );
}


//------------------------------------------------------------------------------
//
// ---
void QtGrDraw::PutRectangle( POINT& p1, POINT& p2, COLORREF color, bool _xor )  // Нарисовать прямоугольник
{
  SelectPen( 1, GetRValue( color ), GetGValue( color ), GetBValue( color ), _xor );
  MoveTo( p1.x, p1.y );
  LineTo( p2.x, p1.y );
  LineTo( p2.x, p2.y );
  LineTo( p1.x, p2.y );
  LineTo( p1.x, p1.y );
}


//------------------------------------------------------------------------------
//
// ---
void QtGrDraw::FillRectangle( RECT* rc )  // Залить прямоугольник цветом фона
{
  if ( painter == nullptr )
    return;

  QRect qr = mdiChild.rect();
  const auto pxRatde = mdiChild.devicePixelRatio();
  if ( rc )
  {
    qr.setLeft  ( rc->left * pxRatde );
    qr.setRight ( rc->right * pxRatde );
    qr.setTop   ( rc->top * pxRatde );
    qr.setBottom( rc->bottom * pxRatde );
  }

  QColor colorBack( GetRValue(backgr), GetGValue(backgr), GetBValue(backgr) );
  QPen oldPen = painter->pen();
  QBrush oldBrush = painter->brush();
  painter->setPen  ( QPen(colorBack) );
  painter->setBrush( QBrush(colorBack) );

  painter->drawRect( qr );

  painter->setBrush( oldBrush);
  painter->setPen( oldPen );
}


//------------------------------------------------------------------------------
//
// ---
void QtGrDraw::GetClientRect( RECT& r )
{
  QRect qr = mdiChild.rect();
  const auto pxRatde = mdiChild.devicePixelRatio();
  
  r.top    = qr.top() * pxRatde;
  r.left   = qr.left() * pxRatde;
  r.bottom = qr.bottom() * pxRatde;
  r.right  = qr.right() * pxRatde;
};


//------------------------------------------------------------------------------
//
// ---
void QtGrDraw::Invalidate( bool /*bErase*/ )
{
  mdiChild.scene()->invalidate();
}

//------------------------------------------------------------------------------
//
// ---
void QtGrDraw::BeginPaint()
{
  if ( painter == nullptr )
    return;

  cp.setX(0);
  cp.setY(0);

//ЭМ  if ( paintCount == 0 )
//ЭМ    painter->begin( &mdiChild );

//ЭМ  paintCount++;
}


//------------------------------------------------------------------------------
//
// ---
void QtGrDraw::EndPaint()
{
  if ( painter == nullptr )
    return;

//ЭМ  paintCount--;

//ЭМ  if ( paintCount == 0 )

    // Отработать объекты отложенного рисования
    for ( size_t i = 0, c = arr.Count(); i < c; ++i )
      arr[i]->Draw( *painter );
    arr.Flush(); // Очистить очереди отложенных отрисовок

//ЭМ    painter->end();

}


//------------------------------------------------------------------------------
//
// ---
void QtGrDraw::SetWindowFocus( bool mdiActivate )
{
  if ( mdiActivate )
  {
    MainWindow* mainWnd = qobject_cast<MainWindow*>(qMainWnd);
    if ( mainWnd )
      mainWnd->setActiveMdiChild( &mdiChild );
  }

  mdiChild.setFocus();
}


//------------------------------------------------------------------------------
//
// ---
void QtGrDraw::ChangeWindowState()
{
  MainWindow* mainWnd = qobject_cast<MainWindow*>(qMainWnd);
  if ( mainWnd )
    mainWnd->changeActiveMdiChildState();
}


//------------------------------------------------------------------------------
//
// ---
bool QtGrDraw::IsMyWnd( const void * wndId ) const
{
  return (QObject*)wndId == mdiChild.scene() || (QObject*)wndId == &mdiChild;
}


//------------------------------------------------------------------------------
// Инициализация для работы окна в полутоновом режиме
// ---
bool QtGrDraw::InitializeGL()
{
  if ( painter == nullptr )
    return false;

  if ( oglInitialized )
    return true;
  else
    oglInitialized = true;
  
  IGrDraw::InitializeGL();

  RECT rc;
  GetClientRect( rc );
  SetSizeGL( rc.right, rc.bottom, parent.GetScale() );

  return true;
}


//------------------------------------------------------------------------------
// Установить сцену
// ---
void QtGrDraw::SetSizeGL( int cx, int cy, double scale )
{
  IGrDraw::SetSizeGL( cx, cy, scale );
}

//------------------------------------------------------------------------------
// Создать display list с данными и инициализировать состояние.
// ---
void QtGrDraw::InitModelList( const MbModel & drawModel, COLORREF select )
{
  meshModel = (GLuint)(uint)(size_t)(&drawModel);//glGenLists(1);

  glNewList( meshModel, GL_COMPILE );
  PrepareGL( drawModel, select );
  glEndList();

  oglModelReady = true;
}

//------------------------------------------------------------------------------
// Отрисовать через OpenGL
// ---
void QtGrDraw::ShowOGL( const MbModel & drawModel, COLORREF select, std::vector< SPtr<MbItem> >* addItems )
{
  InitializeGL();

  ClearGL(); // Очистить OpenGL.
  MatrixGL(); // Загрузить OpenGL видовую матрицу преобразования

  if ( !oglModelReady )
    InitModelList( drawModel, select );

  if ( addItems != nullptr ) {
    MbMatrix3D addMtr;
    
    for ( size_t k = 0, addCnt = addItems->size(); k < addCnt; k++ ) {
      MbItem * addItem = (*addItems)[k];
      if ( addItem != nullptr ) {
        RenderItemGL( addItem, addMtr, drawModel, select, addItem->IsSelected() );
      }
    }
  }

  glCallList( meshModel );

  //if ( QGLWidget* glWidget = qobject_cast<QGLWidget*>(mdiChild.viewport()) )
  //  glWidget->swapBuffers(); // Вывести содержимое буфера на экран.

}


//------------------------------------------------------------------------------
// Обновить данные OpenGL
// ---
void QtGrDraw::RefreshOGL()
{
  // Удалить display list с данными.
  //if ( glIsList(meshModel) )
  glDeleteLists( meshModel, 1 );
  meshModel = 0;
  oglModelReady = false;
}


//------------------------------------------------------------------------------
// Визуальные свойства
// ---
void QtGrDraw::SetGLProperty( const MbModel & drawModel, double la, double ld, double ls, MbVector3D &ll )
{
  lightAmbient  = (float)la;
  lightDiffuse  = (float)ld;
  lightSpecular = (float)ls;
  lightLocal = ll;
  lightSide  = ll;

  IGrDraw::SetGLProperty( drawModel, la, ld, ls, ll );
}

//------------------------------------------------------------------------------
// Удалить данные
// ---
void QtGrDraw::DeleteGL( const MbModel & drawModel )
{
  IGrDraw::DeleteGL( drawModel );
  oglInitialized = false;
}


//------------------------------------------------------------------------------
// Выполнить процесс обработки сообщений ввода точки
// ---
void WorkWindow::ExecuteGetPointProc()
{
  MainWindow * mainWnd = qobject_cast<MainWindow*>(qMainWnd);
  if ( mainWnd ) {
    GetDrawTool().SetWindowFocus( true/*mdiActivate*/ );
    mainWnd->ExecuteLoop( eFindPoint );
  }
}


//------------------------------------------------------------------------------
// Завершить процесс обработки сообщений ввода точки
// ---
void WorkWindow::BreakProc( PNT_result _getPointResult )
{  
  if ( MainWindow * mainWnd = qobject_cast<MainWindow*>(qMainWnd) )
  {
    if ( !mainWnd->IsNormalMode() )
    {
      getPointResult = _getPointResult;
      if ( getPointResult != 0 )
        mainWnd->BreakLoop();
    }
  }
}

//------------------------------------------------------------------------------
// Завершить работу приложения
// ---
void WorkWindow::Quit()
{
  PRECONDITION( qMainWnd );

  QThread* thread = qMainWnd->thread();
  if ( thread )
    thread->quit();
}

//------------------------------------------------------------------------------
// Повторить предыдущую команду
// ---
void WorkWindow::RepeatPrevCmd()
{
  // Уже нахордимся в слоте, поэтому просто вызываем обработчик
  MainWindow * mainWnd = qobject_cast<MainWindow*>(qMainWnd);
  if ( mainWnd )
    mainWnd->Command( (uint)cmPrev );
  //::PostMessage( hwndFrame, WM_COMMAND, WPARAM(TestVariables::cmPrev), 0 ); // Windows
}

//------------------------------------------------------------------------------
// Перестроить тело. Rebuild solid or shell.
// ---
void WorkWindow::RebuildShellCmd()
{
  //MainWindow * mainWnd = qobject_cast<MainWindow*>(qMainWnd);
  //if ( mainWnd )
  //  mainWnd->Command( (uint)CM_REBUILD_SHELL );
  //::PostMessage( hwndFrame, WM_COMMAND, WPARAM(CM_REBUILD_SHELL), 0 ); // Windows
}

//------------------------------------------------------------------------------
// Выполнить пропущенную олперацию. Build an omitted operation
// ---
void WorkWindow::SolidFinishCmd()
{
  //MainWindow * mainWnd = qobject_cast<MainWindow*>(qMainWnd);
  //if ( mainWnd )
  //  mainWnd->Command( (uint)CM_SOLIDFINISH );
  //::PostMessage( hwndFrame, WM_COMMAND, WPARAM(CM_SOLIDFINISH), 0 ); // Windows
}

//------------------------------------------------------------------------------
// Сдвинуть конец дерева построения. Move the end of the build log. 
// ---
void WorkWindow::SolidEndMoveCmd()
{
  //MainWindow * mainWnd = qobject_cast<MainWindow*>(qMainWnd);
  //if ( mainWnd )
  //  mainWnd->Command( (uint)CM_SOLIDENDMOVE );
  //::PostMessage( hwndFrame, WM_COMMAND, WPARAM(CM_SOLIDENDMOVE), 0 ); // Windows
}

//------------------------------------------------------------------------------
// Изменить цвет элементов. Change objects color.
// ---
void WorkWindow::SetColorCmd()
{
  //MainWindow * mainWnd = qobject_cast<MainWindow*>(qMainWnd);
  //if ( mainWnd )
  //  mainWnd->Command( (uint)CM_SETOBJECTCOLOR );
  //::PostMessage( hwndFrame, WM_COMMAND, WPARAM(CM_SETOBJECTCOLOR), 0 ); // Windows
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Creator's
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IGrDraw* CreateDrawTool( QWidget * wnd, WorkWindow& parent, const MbVector3D & light )
{
  GraphicsView* mdiChild = qobject_cast<GraphicsView*>(wnd);
  if ( mdiChild )
    return new QtGrDraw( *mdiChild, parent, light );
  else
  {
    PRECONDITION( false );
    return nullptr;
  }
}


bool GraphicsScene::shiftPress    = false;
bool GraphicsScene::ctrlPress     = false;
bool GraphicsScene::capslockPress = false;
////////////////////////////////////////////////////////////////////////////////
//
//
//
//
////////////////////////////////////////////////////////////////////////////////
GraphicsScene::GraphicsScene( QObject* parent )
  : QGraphicsScene(parent)
  , rectangleItem( nullptr )
{
}

//------------------------------------------------------------------------------
//
// ---
GraphicsScene::~GraphicsScene()
{
}

//------------------------------------------------------------------------------
//
// ---
bool isShiftPressed()
{
  return GraphicsScene::IsShiftPress();
}


//------------------------------------------------------------------------------
//
// ---
bool isCtrlPressed()
{
  return GraphicsScene::IsCtrlPress();
}


//------------------------------------------------------------------------------
//
// ---
bool isCapitalPressed()
{
  return GraphicsScene::IsCapslockPress();
}

//------------------------------------------------------------------------------
//
// ---
void GraphicsScene::renderWorld( QPainter* painter )
{
  ptrdiff_t index = viewManager->FindWindowIndex( (void*)this, false/*setActive*/ );
  if ( 0 <= index && index < viewManager->GetWindowsCount() )
  {
    WorkWindow* parent = viewManager->GetWindow( index );
    const double pxRat = painter->paintEngine()->paintDevice()->devicePixelRatio();
    if ( parent->GetRenderType() == wwrt_OGL )
    {
      bool useOpenGL= (painter->paintEngine()->type() == QPaintEngine::OpenGL);
      useOpenGL= useOpenGL || (painter->paintEngine()->type() == QPaintEngine::OpenGL2);

      if (!useOpenGL)
      {
        qWarning("OpenGLScene: drawBackground needs a QGLWidget to be set as viewport on the graphics view");
        return;
      }
    }
    
    if ( parent->GetRenderType() == wwrt_OGL )
      parent->SetSize( static_cast<int>(width() * pxRat ), static_cast<int>(height() * pxRat) );

    QtGrDraw& tool = (QtGrDraw&)parent->GetDrawTool();


    tool.SetPainter(painter);
    tool.BeginPaint();

    if ( parent->GetRenderType() == wwrt_OGL )
      glEnable( GL_DEPTH_TEST );

    viewManager->DrawContentWindow( index ); // Нарисовать все окно

    if ( parent->GetRenderType() == wwrt_OGL )
      glDisable( GL_DEPTH_TEST );

    tool.EndPaint();
    tool.SetPainter(nullptr);
  }
}

//------------------------------------------------------------------------------
//
// ---
void GraphicsScene::mouseMoveCommand( POINT& p, bool lBtn, bool mBtn )
{
  if ( WorkWindow* w = viewManager->GetWindow( viewManager->FindWindowIndex((void*)this, false/*setActive*/)) )
  {
    w->PutCoordinates( p );

    bool processed = false;
    bool showCoordinates = true;

    MainWindow* mainWnd = qobject_cast<MainWindow*>(qMainWnd);
    if ( mainWnd && !mainWnd->IsFindPointMode() )
      showCoordinates = false;

    if ( mainWnd->IsKbdWaiting() )
    {
      showCoordinates = false;
      processed = true;
    }
    else
      processed = w->MouseMoveCommand( p, isShiftPressed(), isCtrlPressed(), lBtn, mBtn );

    if ( processed && showCoordinates )
      w->PutCoordinates( p );

    if ( !rectangleItem )
      invalidate();

    // Переставить фокус на окно
//    if ( mainWnd && !mainWnd->IsKbdWaiting() )
//      mainWnd->setActiveMdiChild( (QWidget*)this->parent() );
  }
}


//------------------------------------------------------------------------------
//
// ---
void GraphicsScene::mouseCommand( uint message, POINT& p, int paramCount )
{
  WorkWindow* w = viewManager->GetWindow( viewManager->FindWindowIndex( (void*)this, false/*setActive*/ ) );
  if ( w )
  {
    bool processed = false;
    bool showCoordinates = true;
    const bool ctrlPressed = isCtrlPressed(), shftPressed = isShiftPressed();

    MainWindow* mainWnd = qobject_cast<MainWindow*>(qMainWnd);
    if ( mainWnd && mainWnd->IsFindPointMode() )
    {
      showCoordinates = false;
      processed = w->MouseCommandFindPoint( message, p, ctrlPressed, shftPressed );
      if ( processed && getPointResult )
        w->BreakProc( getPointResult );
    }

    if ( !processed )
    {
      if ( mainWnd->IsKbdWaiting() )
      {
        showCoordinates = false;
        processed = true;
      }
      else
        processed = w->MouseCommand( message, p, shftPressed, paramCount );
    }

    if ( processed && showCoordinates )
      w->PutCoordinates( p );

    invalidate();
  }
}

//------------------------------------------------------------------------------
//
// ---
void GraphicsScene::keyboardCommand( int key )
{
  WorkWindow* w = viewManager->GetWindow( viewManager->FindWindowIndex( (void*)this, false/*setActive*/ ) );
  if ( w )
  {
    MainWindow* mainWnd = qobject_cast<MainWindow*>(qMainWnd);
    if ( mainWnd )
    {
      if ( mainWnd->IsKbdWaiting() )
      {
        w->BreakProc( (PNT_result)key );
      }
      else
      {
        uint vk = ::toVK( key );
        if ( vk )
        {
          WorkRenderType typeOld = w->GetRenderType();

          bool processed = false;
          bool showCoordinates = true;

          if ( mainWnd->IsFindPointMode() )
          {
            showCoordinates = false;

            POINT p;
            // Замена GetCursorPos( &p );
            QPoint pos = ((QWidget*)parent())->cursor().pos();
            p.x = pos.x();
            p.y = pos.y();

            processed = w->KeyboardCommandFindPoint( vk, isCtrlPressed(), isShiftPressed(), p );
            if ( processed && getPointResult )
              w->BreakProc( getPointResult );
          }

          if ( !processed )
            processed = w->KeyboardCommand( vk, isCtrlPressed(), isShiftPressed() );

          if ( processed && showCoordinates )
            w->PutCoordinates( posit );

          if ( w->GetRenderType() != typeOld )
          {
            w->EraseModel();

            GraphicsView* parentView = qobject_cast< GraphicsView* >( parent() );
            Q_ASSERT( parentView != nullptr );

            parentView->setViewport( w->GetRenderType() == wwrt_OGL ? new QGLWidget(new QGLContext(QGLFormat(QGL::SampleBuffers)), parentView) : new QWidget(parentView) );

            QWidget* viewport = parentView->viewport();
            Q_ASSERT( viewport != nullptr );
            viewport->setAutoFillBackground( false ); // for compatibility
            viewport->setMouseTracking( true );
          }
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
//
// ---
void GraphicsScene::drawBackground(QPainter* painter, const QRectF& rect)
{
  Q_UNUSED( rect );
  renderWorld( painter );
}

//------------------------------------------------------------------------------
//
// ---
bool GraphicsScene::event( QEvent* event )
{
  PRECONDITION( event );
  switch ( event->type() )
  {
    case QEvent::KeyPress  :
      {
        QKeyEvent *k = (QKeyEvent *)event;
        switch ( k->key() )
        {
          case Qt::Key_Shift    : shiftPress    = true; break;
          case Qt::Key_Control  : ctrlPress     = true; break;
          case Qt::Key_CapsLock : capslockPress = true; break;
          default: break;
        }
      }
      break;
    case QEvent::KeyRelease:
      {
        QKeyEvent *k = (QKeyEvent *)event;
        switch ( k->key() )
        {
          case Qt::Key_Shift    : shiftPress    = false; break;
          case Qt::Key_Control  : ctrlPress     = false; break;
          case Qt::Key_CapsLock : capslockPress = false; break;
          default: break;
        }
      }
      break;

    default:
      break;
  }

  return QGraphicsScene::event( event );
}

//------------------------------------------------------------------------------
//
// ---
void GraphicsScene::keyPressEvent( QKeyEvent* event )
{
  PRECONDITION( event );
  keyboardCommand( event->key() );
  event->accept();
}

//------------------------------------------------------------------------------
//
// ---
void GraphicsScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* event )
{
  PRECONDITION( event );

  uint message = 0;
  switch( event->button() )
  {
    case Qt::LeftButton  : message = WM_LBUTTONDBLCLK; break;
    case Qt::MiddleButton   : message = WM_MBUTTONDBLCLK; break;
    case Qt::RightButton : message = WM_RBUTTONDBLCLK; break;
    default: break;
  }

  QPointF pnt = event->scenePos() * event->widget()->devicePixelRatio();
  POINT p = { static_cast<int>(pnt.x()), static_cast<int>(pnt.y()) };
  mouseCommand( message, p );

  event->accept();
}

//------------------------------------------------------------------------------
//
// ---
void GraphicsScene::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
  PRECONDITION( event );
  auto dpr = event->widget()->devicePixelRatio();
  QPoint pnt = event->scenePos().toPoint() * dpr;
  //const auto pxRatde = mdiChild.devicePixelRatio();
  POINT p = { static_cast<int>(pnt.x()), static_cast<int>(pnt.y()) };
  mouseMoveCommand( p, (event->buttons() & Qt::LeftButton) != 0, (event->buttons() & Qt::MiddleButton) != 0 );

  if ( (event->buttons() & Qt::LeftButton) && rectangleItem && (::fabs(dpr) > NULL_EPSILON) )
    rectangleItem->setCoordinates( pntBegin, pnt / dpr );

  event->accept();
}

//------------------------------------------------------------------------------
//
// ---
void GraphicsScene::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
  PRECONDITION( event );

  uint message = 0;
  switch( event->button() )
  {
    case Qt::LeftButton  : message = WM_LBUTTONDOWN; break;
    case Qt::MiddleButton   : message = WM_MBUTTONDOWN; break;
    case Qt::RightButton : message = WM_RBUTTONDOWN; break;
    default: break;
  }

  const auto dpr = event->widget()->devicePixelRatio();
  QPointF pnt = event->scenePos();
/*
  GraphicsView* parentView = qobject_cast< GraphicsView* >( parent() );
  Q_ASSERT( parentView != nullptr );
  QWidget* viewport = parentView->viewport();
  Q_ASSERT( viewport != nullptr );
  pnt = viewport->mapFromGlobal( pnt );
*/
  POINT p = { static_cast<int>(pnt.x() * dpr ), static_cast<int>(pnt.y() * dpr) };
  mouseCommand( message, p );

  if ( event->button() == Qt::LeftButton && tflag < 0 )
  {
    if ( rectangleItem )
    {
      removeItem( rectangleItem );
      rectangleItem = nullptr;
      event->accept();
      return;
    }

    rectangleItem = new SelectRectangleItem();
    addItem( rectangleItem );
    pntBegin = pnt.toPoint();
  }

  event->accept();
}

//------------------------------------------------------------------------------
//
// ---
void GraphicsScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
  PRECONDITION( event );

  uint message = 0;
  switch( event->button() )
  {
    case Qt::LeftButton  : message = WM_LBUTTONUP; break;
    case Qt::MiddleButton   : message = WM_MBUTTONUP; break;
    case Qt::RightButton : message = WM_RBUTTONUP; break;
    default: break;
  }

  QPointF pnt = event->scenePos() * event->widget()->devicePixelRatio();
  POINT p = { static_cast<int>(pnt.x()), static_cast<int>(pnt.y()) };
  mouseCommand( message, p );

  if ( rectangleItem )
  {
    removeItem( rectangleItem );
    rectangleItem = nullptr;
  }

  event->accept();
}

//------------------------------------------------------------------------------
//
// ---
void GraphicsScene::wheelEvent( QGraphicsSceneWheelEvent* event )
{
  PRECONDITION( event );
  if ( !(Qt::MouseButton::MiddleButton & event->buttons()) ) {

    QPointF pnt = event->scenePos() * event->widget()->devicePixelRatio();
    POINT p = { static_cast<int>(pnt.x()), static_cast<int>(pnt.y()) };
    mouseCommand( 0x020A/*WM_MOUSEWHEEL*/, p, event->delta() );

    ptrdiff_t index = viewManager->FindWindowIndex( (void*)this, false/*setActive*/ );
    if ( 0 <= index && index < viewManager->GetWindowsCount() )
    {
      WorkWindow* parent = viewManager->GetWindow( index );
      QtGrDraw& tool = (QtGrDraw&)parent->GetDrawTool();
      tool.CleanItems();
    }
  }


  event->accept();
}

//------------------------------------------------------------------------------
//
// ---
void GraphicsScene::focusInEvent(QFocusEvent* event)
{
  QGraphicsScene::focusInEvent( event );
}


////////////////////////////////////////////////////////////////////////////////
//
//
//
//
////////////////////////////////////////////////////////////////////////////////
GraphicsView::GraphicsView()
  : QGraphicsView()
{
  setMouseTracking( true );
  setViewport( new QGLWidget(new QGLContext(QGLFormat(QGL::SampleBuffers)), this) );
  setViewportUpdateMode( QGraphicsView::FullViewportUpdate );

  viewport()->setAutoFillBackground( false ); // for compatibility

  //    QGraphicsView::setRenderHint(QPainter::SmoothPixmapTransform);
  //    QGraphicsView::setRenderHint(QPainter::Antialiasing);
  //    QGraphicsView::setResizeMode(QDeclarativeView::SizeViewToRootObject);

  setScene(new GraphicsScene( this ));
  viewport()->setMouseTracking( true );
}

//------------------------------------------------------------------------------
//
// ---
GraphicsView::~GraphicsView()
{
}

bool GraphicsView::event( QEvent* event )
{
  bool result = QGraphicsView::event( event );
  return result;
}

//------------------------------------------------------------------------------
//
// ---
void GraphicsView::closeEvent( QCloseEvent* event )
{
  QGraphicsView::closeEvent( event );
  viewManager->DeleteWindow( viewManager->FindWindowIndex( (void*)this->scene(), false /*setActive*/ ) );
}

//------------------------------------------------------------------------------
//
// ---
void GraphicsView::resizeEvent(QResizeEvent *event)
{
  if (QGraphicsScene* grScene = scene())
  {
    grScene->setSceneRect(QRect(QPoint(0, 0), event->size()));
    QGraphicsView::resizeEvent(event);
  }
}



#endif // __USE_QT__
