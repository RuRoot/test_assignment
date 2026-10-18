#ifndef __USE_QT__ 

#include <test.h>
#include <test_window.h>
#include <test_gr_draw.h>
#include <test_manager.h>
#include <test_service.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Инструмент рисования в конкретном окне 
// Ориентирован на вывод с использованием Windows API (GDI)
// ---
class HWndGrDraw : public IGrDraw 
{
protected:
  HWND          hwnd;         // handle окна
  HDC           dc;           // handle of device context окна
  HGLRC         hrc;          // OpenGL rendering context окна
  SArray<HPEN>  selectPenArr;
  SArray<int>   modeArr;
  PAINTSTRUCT   ps;
  GLuint        meshModel;    // Display list с данными о модели

public:
  HWndGrDraw( HWND _hwnd, WorkWindow & _parent, const MbVector3D & lt );
  virtual ~HWndGrDraw();

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
  virtual void SelectPen ( int cWidth, int R, int G, int B, bool bxor = false );
  virtual void SelectPen ( int cStyle, int cWidth, COLORREF c );
  virtual void RestorePen();

  virtual void MoveTo  ( int x, int y ) { if (dc) ::MoveToEx( dc, x, y, nullptr ); }
  virtual void LineTo  ( int x, int y ) { if (dc) ::LineTo  ( dc, x, y ); }
  virtual void SetPixel( int x, int y, int R, int G, int B/*, bool _DPtoLP*/ );
  virtual void DPtoMM  ( const POINT & p, MbCartPoint & cp, double scale )
  {
    cp.Init( ((double)p.x) / (xPixelPerMM * scale), -((double)p.y) / (yPixelPerMM * scale) ); 
  }
  virtual void MMtoDP  ( const MbCartPoint& cp, POINT& p, double scale )
  { 
    p.x = (int32)( cp.x * xPixelPerMM * scale );
    p.y = (int32)(-cp.y * yPixelPerMM * scale );
  }
  virtual void PutRectangle( POINT & p1, POINT & p2, COLORREF, bool bxor ); // Нарисовать прямоугольник
  virtual void FillRectangle( RECT * rc = nullptr ); // Залить прямоугольник цветом фона
  virtual void GetClientRect( RECT & r ); 
  virtual void Invalidate( bool bErase = true )  { InvalidateRect( hwnd, nullptr, bErase ); }
  virtual void BeginPaint()  { ::BeginPaint( hwnd, & ps ); }
  virtual void EndPaint()    { ::EndPaint( hwnd, & ps ); }

  virtual void SetWindowFocus( bool mdiActivate )
  {
    if ( mdiActivate )
      SendMessage( hwnd, WM_MDIACTIVATE, 0, 0 );
    ::SetFocus( hwnd );
  }
  virtual void  SetWindowTitle( const TCHAR * str )
  {
    ::SetWindowText( hwnd, str );
  }

  virtual void ChangeWindowState()
  {
    if (GetWindowLongPtr( hwnd, GWL_STYLE ) & WS_MAXIMIZE)
      ShowWindow(hwnd,SW_RESTORE);
    else
      ShowWindow(hwnd,SW_SHOWMAXIMIZED);
  }
  virtual bool IsMyWnd( const void * wndId ) const { return (HWND)wndId == hwnd; }

public:
          HWND  GetHWND () const { return hwnd; }

private:
  void          operator = ( const HWndGrDraw & ); // Не реализовано
};


//------------------------------------------------------------------------------
// 
// ---
HWndGrDraw::HWndGrDraw( HWND _hwnd, WorkWindow& _parent, const MbVector3D & lt )
  : IGrDraw ( _parent, lt )
  , hwnd  ( _hwnd )
  , dc    ( nullptr )
  , hrc   ( nullptr )
  , selectPenArr( 1, 1 )
  , modeArr     ( 1, 1 )
  , ps()
  , meshModel(0)
{
  PRECONDITION( hwnd );

  dc = GetDC( hwnd );
  SetMapMode( dc, MM_TEXT );

  xPixelPerMM = double( ::GetDeviceCaps(dc,LOGPIXELSX) )/LOGIC_INCH;
  yPixelPerMM = double( ::GetDeviceCaps(dc,LOGPIXELSY) )/LOGIC_INCH;

  SetLight( lt );
}


//------------------------------------------------------------------------------
// 
// ---
HWndGrDraw::~HWndGrDraw()
{
  ReleaseDC( hwnd, dc );

  if ( hrc != nullptr && wglMakeCurrent( dc, hrc ) )
    wglMakeCurrent( nullptr, nullptr );

  // Контекст удаляется вне зависимости от результата выполнения wglMakeCurrent( dc, hrc ),
  // иначе, утечка памяти.
  if ( hrc != nullptr )
    wglDeleteContext( hrc );
  hrc = nullptr;
  /* Было:
  if ( hrc != nullptr && wglMakeCurrent( dc, hrc ) )
  {
    wglMakeCurrent( nullptr, nullptr );
    wglDeleteContext( hrc );
  }*/
}


//------------------------------------------------------------------------------
// 
// ---
void HWndGrDraw::SelectPen( int cWidth, int R, int G, int B, bool bxor )
{
  if ( dc )
  {
    if ( bxor )
    {
      modeArr.Add( ::SetROP2( dc, R2_XORPEN ) );
      selectPenArr.Add( HPEN( ::SelectObject( dc, ::CreatePen(PS_SOLID, cWidth, RGB( R, G, B ) ^ backgr) ) ) );
    }
    else
    {
      modeArr.Add( ::SetROP2( dc, R2_COPYPEN ) );
      selectPenArr.Add( HPEN( ::SelectObject( dc, ::CreatePen(PS_SOLID, cWidth, RGB( R, G, B ) ) ) ) );
    }
  }
}


//------------------------------------------------------------------------------
// 
// ---
void HWndGrDraw::SelectPen( int cStyle, int cWidth, COLORREF c )
{
  if ( dc )
  {
    modeArr.Add( ::SetROP2( dc, R2_COPYPEN ) );
    selectPenArr.Add( HPEN( ::SelectObject( dc, ::CreatePen(cStyle, cWidth, c ) ) ) );
  }
}


//------------------------------------------------------------------------------
// 
// ---
void HWndGrDraw::RestorePen()
{
  if ( dc && selectPenArr.Count() )
  {
    {
      size_t idx = selectPenArr.MaxIndex();
      ::DeleteObject( ::SelectObject( dc, selectPenArr[idx] ) );
      selectPenArr.RemoveInd( idx );
    }

    {
      size_t idx = modeArr.MaxIndex();
      ::SetROP2( dc, modeArr[idx] );
      modeArr.RemoveInd( idx );
    }
  }
}


//------------------------------------------------------------------------------
// Нарисовать прямоугольник
// ---
void HWndGrDraw::PutRectangle( POINT& p1, POINT& p2, COLORREF color, bool bxor )
{
  if ( dc )
  {
    int  left   = std_min(p1.x, p2.x); // x-coord. of bounding rectangle's upper-left corner
    int  top    = std_min(p1.y, p2.y); // y-coord. of bounding rectangle's upper-left corner
    int  right  = std_max(p1.x, p2.x); // x-coord. of bounding rectangle's lower-right corner
    int  bottom = std_max(p1.y, p2.y); // y-coord. of bounding rectangle's lower-right corner  

    HPEN hpen = HPEN( ::SelectObject( dc, ::CreatePen( PS_SOLID, 1, color ) ) );
    int old = 0;
    if ( bxor )
      old = ::SetROP2( dc, R2_XORPEN );

    ::MoveToEx( dc, left, top, nullptr );
    ::LineTo( dc, left,  bottom );
    ::LineTo( dc, right, bottom );
    ::LineTo( dc, right, top );
    ::LineTo( dc, left,  top );

    if ( bxor )
      ::SetROP2(  dc, old );

    ::DeleteObject( ::SelectObject( dc, hpen ) );
  }
}


//------------------------------------------------------------------------------
// Залить прямоугольник цветом фона
// ---
void HWndGrDraw::FillRectangle( RECT * rc ) 
{
  if ( dc )
  {
    RECT r;
    if ( rc == nullptr )
      GetClientRect( r );

    ::SetBkColor( dc, backgr );
    HPEN hpen = HPEN( ::SelectObject( dc, ::CreatePen(PS_SOLID, 1, backgr ) ) );
    HBRUSH solidBrush = ::CreateSolidBrush( backgr );

    ::FillRect( dc, (rc ? rc : &r), solidBrush ); 

    ::DeleteObject( solidBrush ); // Была утечка, нужно удалять
    ::DeleteObject( SelectObject( dc, hpen ) );
  }
}


//------------------------------------------------------------------------------
// Размер окна
// ---
void HWndGrDraw::GetClientRect( RECT& r ) 
{
  if ( dc )
    ::GetClientRect( hwnd, &r );
}


//------------------------------------------------------------------------------
// Размер окна
// ---
void HWndGrDraw::SetPixel( int x, int y, int R, int G, int B )
{
  if ( dc )
    ::SetPixel( dc, x, y, PALETTERGB(R, G, B) );
}


//------------------------------------------------------------------------------
// Инициализация для работы окна в полутоновом режиме
// ---
bool HWndGrDraw::InitializeGL() {
  static PIXELFORMATDESCRIPTOR pfd = {
      sizeof (PIXELFORMATDESCRIPTOR),             // Size of this structure
      1,                                          // Version number
      PFD_DRAW_TO_WINDOW |                        // Flags
      PFD_SUPPORT_OPENGL |
      PFD_DOUBLEBUFFER,
      PFD_TYPE_RGBA,                              // RGBA pixel values
      64,//24,                                    // 24-bit color
      0, 0, 0, 0, 0, 0,                           // Don't care about these
      64, 0,//0, 0,                               // No alpha buffer
      64, 0, 0, 0, 0,// 0, 0, 0, 0, 0,            // No accumulation buffer
      64,//32,                                    // 32-bit depth buffer
      64,//0,                                     // No stencil buffer
      64,//0,                                     // No auxiliary buffers
      PFD_MAIN_PLANE,                             // Layer type
      0,                                          // Reserved (must be 0)
      0, 0, 0                                     // No layer masks
  };

  int pixelFormat = ::ChoosePixelFormat( dc, &pfd );

  if ( pixelFormat == 0 )
    return false;
  if ( !::SetPixelFormat( dc, pixelFormat, &pfd ) )
    return false;

  hrc = ::wglCreateContext( dc );
  if ( !hrc )
    return false;

  if ( !wglMakeCurrent( dc, hrc ) )
    return false;

  ::DescribePixelFormat( dc, pixelFormat, sizeof (PIXELFORMATDESCRIPTOR), &pfd );

  IGrDraw::InitializeGL();

  wglMakeCurrent( nullptr, nullptr );

  RECT rc;
  GetClientRect( rc );
  SetSizeGL( rc.right, rc.bottom, parent.GetScale() );

  return true;
}


//------------------------------------------------------------------------------
// Установить сцену
// ---
void HWndGrDraw::SetSizeGL( int cx, int cy, double scale ) 
{
  if ( wglGetCurrentContext() != hrc )  // Сделаем текущим rendering context
    wglMakeCurrent( dc, hrc );

  IGrDraw::SetSizeGL( cx, cy, scale ); 

  ::wglMakeCurrent( nullptr, nullptr );
}


//------------------------------------------------------------------------------
// Создать display list с данными и инициализировать состояние.
// ---
void HWndGrDraw::InitModelList( const MbModel & drawModel, COLORREF select )
{
   meshModel = (GLuint)(size_t)( &drawModel );
   glNewList( meshModel, GL_COMPILE );
   PrepareGL( drawModel, select ); 
   glEndList();

   oglModelReady = true;
}


//------------------------------------------------------------------------------
// Отрисовать через OpenGL
// ---
void HWndGrDraw::ShowOGL( const MbModel & drawModel, COLORREF select, std::vector< SPtr<MbItem> > * addItems ) 
{
  if ( hrc == nullptr )
    InitializeGL();

  if ( !::wglMakeCurrent( dc, hrc ) )
    return;

  ClearGL(); // Очистить OpenGL. 
  MatrixGL(); // Загрузить OpenGL видовую матрицу преобразования

  if ( !oglModelReady )
    InitModelList( drawModel, select );

  if ( addItems != nullptr ) {
    MbMatrix3D addMtr;
    //const MbVisual * hostVisual = static_cast<const MbVisual *>( drawModel.GetSimpleAttribute(at_Visual) );
    //const MbColor *  hostColor  = static_cast<const MbColor *> ( drawModel.GetSimpleAttribute(at_Color) );

    for ( size_t k = 0, addCnt = addItems->size(); k < addCnt; k++ ) {
      MbItem * addItem = (*addItems)[k];
      if ( addItem != nullptr ) {
        RenderItemGL( addItem, addMtr, drawModel, select, addItem->IsSelected() );
      }
    }
  }

  glCallList( meshModel ); 

  ::SwapBuffers( dc ); // Вывести содержимое буфера на экран.
  //FinishGL(); // Очистить OpenGL 
  //::wglMakeCurrent( nullptr, nullptr );
}


//------------------------------------------------------------------------------
// // Удалить временные данные (листы) OpenGL.
// ---
void HWndGrDraw::RefreshOGL() {
   // Удалить display list с данными.
   //if ( glIsList(meshModel) )
   glDeleteLists( meshModel, 1 );
   meshModel = 0;
   oglModelReady = false;
}


//------------------------------------------------------------------------------
// Визуальные свойства
// ---
void HWndGrDraw::SetGLProperty( const MbModel & drawModel, double la, double ld, double ls, MbVector3D &ll ) 
{
  lightAmbient  = (float)la;
  lightDiffuse  = (float)ld;
  lightSpecular = (float)ls;
  lightLocal = ll;
  lightSide  = ll;

  if ( parent.GetRenderType() == wwrt_OGL && 
       hrc!=nullptr &&
       wglMakeCurrent(dc, hrc) ) 

  {
    IGrDraw::SetGLProperty( drawModel, la, ld, ls, ll ); 
    wglMakeCurrent( nullptr, nullptr );
  }
}


//------------------------------------------------------------------------------
// Удалить данные
// ---
void HWndGrDraw::DeleteGL( const MbModel & drawModel ) 
{
  if ( hrc!=nullptr && wglMakeCurrent( dc, hrc ) ) {
    IGrDraw::DeleteGL( drawModel ); 

    wglMakeCurrent( nullptr, nullptr );
  }

  // Контекст удаляется вне зависимости от результата выполнения wglMakeCurrent( dc, hrc ),
  // иначе, утечка памяти.
  if ( hrc )
    wglDeleteContext( hrc );
  hrc = nullptr;
  /*Было:
  if ( hrc!=nullptr && wglMakeCurrent( dc, hrc ) ) {
    IGrDraw::DeleteGL( drawModel ); 

    wglMakeCurrent( nullptr, nullptr );
    if ( hrc )
      wglDeleteContext( hrc );
  }*/
}


//------------------------------------------------------------------------------
// Обработка сообщений рабочих окон
// ---
LRESULT CALLBACK ViewWndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
  if ( TestVariables::viewManager != nullptr ) {
    ptrdiff_t index = TestVariables::viewManager->FindWindowIndex( (void*)hwnd, false );
    TestVariables::viewManager->SetActiveWindow( index ); // Установить индекс активного окна

    switch ( message ) {
      case WM_SIZE : {
        LONG nWidth  = LOWORD(lParam); // width of client area
        LONG nHeight = HIWORD(lParam); // height of client area

        { // уменьшение размера дочернего окна при переносе главного окна на второй монитор с меньшим разрешением, при раздокировании
          RECT frameRect;
          GetClientRect( hwndFrame, &frameRect);
          LONG nWidth1  = ::abs_t(frameRect.right - frameRect.left);
          LONG nHeight1 = ::abs_t(frameRect.top - frameRect.bottom);

          bool changeSize = false;

          if ( nWidth > nWidth1 ) {
            nWidth = nWidth1;
            changeSize = true;
          }
          if ( nHeight > nHeight1 ) {
            nHeight = nHeight1;
            changeSize = true;
          }
          if ( changeSize ) {
            lParam = MAKELONG( nWidth, nHeight );
            ::SetWindowPos( hwnd, nullptr, -1, -1, nWidth, nHeight, SWP_NOREPOSITION );
          }
        }

        TestVariables::viewManager->SetSize( index, nWidth, nHeight );
      } break;

      case WM_PAINT : {
        WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();
        if ( wnd != nullptr )
        {
          IGrDraw & tool = wnd->GetDrawTool();
          tool.BeginPaint();
          TestVariables::viewManager->DrawContentWindow( index ); // Нарисовать все окно
          tool.EndPaint();
        }
      } break;

      case WM_ERASEBKGND : {
        TestVariables::viewManager->EraseBackground( index );
      } break;

      case WM_MDIACTIVATE : {
        if ( hwnd == (HWND)lParam ) // Активизация окна
          TestVariables::viewManager->SetActiveWindow( index ); // Установить индекс активного окна
      } break;

      case WM_KEYDOWN : {
        WorkWindow * w = TestVariables::viewManager->GetWindow( index );
        if ( w != nullptr ) {
          if ( w->KeyboardCommand( LoUint32(wParam), isCtrlPressed(), isShiftPressed() ) )
          {
            POINT p = TestVariables::cursorPoint0;// GNN При точном вводе перебиваются значения текущего положения курсра. { LOWORD(lParam), HIWORD(lParam) };
            w->PutCoordinates( p );
          }
#ifdef C3D_WINDOWS //_MSC_VER // Только для Windows, т.к. у Windows не нашел макросов для цифровых и буквенных клавиш, чтобы сделать аналог для QT
          else if ( LoUint32(wParam) == 0x57 ) { // 'W'
            if ( isCtrlPressed() )
              ::PostMessage ( hwnd, WM_CLOSE, 0, 0 );
          }
#endif
          break;
        }
      } break;

      case WM_LBUTTONDOWN : 
      case WM_LBUTTONUP : 
      case WM_LBUTTONDBLCLK : 
      case WM_RBUTTONDOWN : 
      case WM_MBUTTONDOWN : 
      case WM_MBUTTONUP :  
      case 0x020A /*WM_MOUSEWHEEL*/ :
      {
        POINT p = { LOWORD(lParam), HIWORD(lParam) };
        WorkWindow * w = TestVariables::viewManager->GetWindow( index );
        if ( w && 
             w->MouseCommand( message, p, ( GetKeyState(VK_SHIFT) & 0x8000 ) != 0, (int16)HIWORD(wParam) ) )
        {
          if ( message == 0x020A /*WM_MOUSEWHEEL*/ ) { // При прокручивании колесика мыши позицию получаем относительно экрана
            // KYA K11 нужно так для систем с несколькими мониторами
            p.x = LPARAM_TO_XPOS(lParam);
            p.y = LPARAM_TO_YPOS(lParam);
            HWND hw = GetFocus();
            if ( hw != nullptr )
              ScreenToClient( hw, &p );
          }

          w->PutCoordinates( p );
          return 0;
        }
      } break;


      case PASS_FOCUS : {
        HWND hto = (HWND)wParam;
        ptrdiff_t newIndex = TestVariables::viewManager->FindWindowIndex( (void*)hto, false );
        TestVariables::viewManager->SetFocus( newIndex, false );
      } break;

      case WM_MOUSEMOVE : { // Пересчет координат и сдвиг временных точек
        HWND hw = GetFocus();
        if ( hw != hwnd )
          SendMessage ( hw, PASS_FOCUS, (WPARAM)hwnd, 0L );

        POINT p = { LOWORD(lParam), HIWORD(lParam) };
        WorkWindow * w = TestVariables::viewManager->GetWindow( index );

        if ( w ) {
          bool ctrl  = ( (GetKeyState(VK_CONTROL) & 0x8000) != 0 ); 
          bool shift = ( (GetKeyState(VK_SHIFT) & 0x8000) != 0 );
          w->PutCoordinates( p );
          w->MouseMoveCommand( p, shift, ctrl, ((int)LoUint32(wParam) & MK_LBUTTON) != 0,
                                         ((int)LoUint32(wParam) & MK_MBUTTON) != 0 );  
        }
      } break;

      case WM_QUERYENDSESSION:
      case WM_CLOSE: {            // Закрыть все дочерние окна
        //OV_Lnx viewManager->DeleteGL(); <- OV_Lnx делается в FrameWndProc::WM_CLOSE
      } break;

      case WM_DESTROY : {
        TestVariables::viewManager->DeleteWindow( TestVariables::viewManager->GetActiveWindowIndex() );

        index = TestVariables::viewManager->GetWindowsCount() - 1;
        if ( index >= 0 ) 
          TestVariables::viewManager->SetFocus( index, true );
      } break;
    }
  }

  return DefMDIChildProc (hwnd, message, wParam, lParam) ;
}


//------------------------------------------------------------------------------
// Создать клиенское окно для рисования модели
// ---
#define CLASS_NAME_VIEW   _T("View")
HWND CreateMdiClient( const TCHAR* title )
{
  static bool init = false;
  if ( !init )
  {
    init = true;
    WNDCLASS wc;
      wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
      wc.lpfnWndProc   = (WNDPROC)ViewWndProc;
      wc.cbClsExtra    = 0;
      wc.cbWndExtra    = 0;
      wc.hInstance     = hInst;
      wc.hIcon         = LoadIcon( hInst, MAKEINTRESOURCE( IDI_ICON_1 ) );
      wc.hCursor       = LoadCursor (nullptr, IDC_ARROW) ;
      wc.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
      wc.lpszMenuName  = nullptr ;
      wc.lpszClassName = CLASS_NAME_VIEW;

    RegisterClass( &wc );
  }

  WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();
  int xPos = CW_USEDEFAULT;
  int yPos = CW_USEDEFAULT;
  int xSize = CW_USEDEFAULT;
  int ySize = CW_USEDEFAULT;

  if ( wnd == nullptr ) {
    xPos = 0;
    yPos = 0;
    RECT frameRect;
    GetClientRect( hwndFrame, &frameRect);
    xSize = ::abs_t(frameRect.right - frameRect.left);
    ySize = ::abs_t(frameRect.top - frameRect.bottom);
  }

  MDICREATESTRUCT mdicreate ;
    mdicreate.szClass = CLASS_NAME_VIEW;
    mdicreate.hOwner  = hInst;
    mdicreate.x       = xPos;
    mdicreate.y       = yPos;
    mdicreate.cx      = xSize;
    mdicreate.cy      = ySize;
    mdicreate.style   = WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    mdicreate.lParam  = 0;//nullptr;
    mdicreate.szTitle = title;

  PRECONDITION( hwndClient );
  return (HWND)SendMessage( hwndClient, WM_MDICREATE, 0, (LPARAM) (LPMDICREATESTRUCT) &mdicreate );
}
#undef   CLASS_NAME_VIEW


//------------------------------------------------------------------------------
// Обработка сообщений ввода точки
// ---
LRESULT CALLBACK GetPointProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
  switch ( message ) {
    case WM_CHAR :
    {
//      switch ( wParam ) {
//        case VK_RETURN :
//        {
//          POINT p;
//          GetCursorPos( &p );
//          cursorFlag = -1;
//          cursorPoint0 = p;
//          cursorPoint1 = p;
//          cursorPoint = viewManager->GetActiveWindow()->GetCartPoint( p );
//          cursorAxis.Init( cursorPoint, viewManager->GetActiveWindow()->GetAxisZ() );
//          mappingPoint = p;//viewManager->GetActiveWindow()->GetMapPoint( p );
//          if ((GetKeyState(VK_SHIFT) & 0x8000) != 0)
//            getPointResult = 3;
//          else if ((GetKeyState(VK_CONTROL) & 0x8000) != 0) 
//            getPointResult = 4;   //-V112
//          else
//            getPointResult = 1;
//        }
//        break;
//
//        case VK_ESCAPE : 
//          getPointResult = 2;
//        break;
//
//        case VK_TAB : // Точный ввод координат
//        {
//          if ( GetExactPoint( cursorPoint ) )
//            getPointResult = 1;
//        }
//        break;
//      }

      WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();
      if ( wnd )
      {
        POINT p;
        GetCursorPos( &p );
        wnd->KeyboardCommandFindPoint( (uint)wParam, (GetKeyState(VK_CONTROL) & 0x8000) != 0, (GetKeyState(VK_SHIFT) & 0x8000) != 0, p ); 
      }
      return 0;
    }

//    case WM_LBUTTONDOWN :
//    {
//      POINT p = { LOWORD(lParam), HIWORD(lParam) };
//      cursorFlag = -1;
//      cursorPoint0 = p;
//      cursorPoint1 = p;
//      cursorPoint = viewManager->GetActiveWindow()->GetCartPoint( p );
//      cursorAxis.Init( cursorPoint, viewManager->GetActiveWindow()->GetAxisZ() );
//      mappingPoint = p;//viewManager->GetActiveWindow()->GetMapPoint( p );
//      if ((GetKeyState(VK_SHIFT) & 0x8000) != 0)
//        getPointResult = 3;
//      else if ((GetKeyState(VK_CONTROL) & 0x8000) != 0)
//        getPointResult = 4;   //-V112
//      else
//        getPointResult = 1;
//
//      return 0;
//    }
//
//    case WM_RBUTTONDOWN :
//    {
//      getPointResult = 2;
//      return 0;
//    }

    case WM_LBUTTONDOWN :
    case WM_RBUTTONDOWN :
    {
      WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();
      if ( wnd ) {
        POINT p = { LOWORD(lParam), HIWORD(lParam) };
        wnd->MouseCommandFindPoint( message, p, (GetKeyState(VK_CONTROL) & 0x8000) != 0, (GetKeyState(VK_SHIFT) & 0x8000) != 0 ); 
      }

      return 0;
    }

    case PASS_FOCUS :
    {
      HWND to = (HWND)wParam;
      ptrdiff_t indexTo = TestVariables::viewManager->FindWindowIndex( (void*)to, false );
      TestVariables::viewManager->SetFocus( indexTo, false );
    }
    break;

    case WM_SETCURSOR :
    {
      if ( LOWORD(lParam) == HTCLIENT ) {
        SetCursor(LoadCursor( nullptr, IDC_CROSS));
        return 0;
      }

      break;
    }
  }

  return ViewWndProc( hwnd, message, wParam, lParam );
} 


//------------------------------------------------------------------------------
// Выполнить процесс обработки сообщений ввода точки
// ---
void WorkWindow::ExecuteGetPointProc()
{
  IGrDraw& tool = GetDrawTool();  
  if ( HWndGrDraw * wndGrDraw = dynamic_cast<HWndGrDraw*>(&tool) )
  {
    LONG_PTR oldfunc = SetWindowLongPtr( wndGrDraw->GetHWND(), GWLP_WNDPROC, (LONG_PTR)GetPointProc );

    BOOL ret;
    do {
      MSG msg;
      ret = GetMessage(&msg, nullptr, 0, 0);
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    } while ( TestVariables::getPointResult == 0 && ret != 0 );

    SetWindowLongPtr( wndGrDraw->GetHWND(), GWLP_WNDPROC, (LONG_PTR)oldfunc);

    if ( ret == 0 ) 
      PostQuitMessage( 0 );
  }
}


//------------------------------------------------------------------------------
//
// ---
void WorkWindow::BreakProc( PNT_result /*_getPointResult*/ ) 
{
  IGrDraw & tool = GetDrawTool();  
  if ( HWndGrDraw * wndGrDraw = dynamic_cast<HWndGrDraw*>(&tool) )
  {
    SendMessage ( wndGrDraw->GetHWND(), WM_CHAR, VK_ESCAPE, 0 );
  }
}

//------------------------------------------------------------------------------
// Завершить работу приложения
// ---
void WorkWindow::Quit() 
{
  ::SendMessage ( hwndFrame, WM_CLOSE, 0, 0L ) ;
}

//------------------------------------------------------------------------------
// Повторить предыдущую команду.
// ---
void WorkWindow::RepeatPrevCmd()
{
  ::PostMessage ( hwndFrame, WM_COMMAND, WPARAM(TestVariables::cmPrev), 0 );
}

//------------------------------------------------------------------------------
// Перестроить тело. Rebuild solid or shell.
// ---
void WorkWindow::RebuildShellCmd()
{
  ::PostMessage ( hwndFrame, WM_COMMAND, WPARAM(CM_REBUILD_SHELL), 0 );
}

//------------------------------------------------------------------------------
// Выполнить пропущенную олперацию. Build an omitted operation
// ---
void WorkWindow::SolidFinishCmd()
{
  ::PostMessage ( hwndFrame, WM_COMMAND, WPARAM(CM_SOLIDFINISH), 0 );
}

//------------------------------------------------------------------------------
// Сдвинуть конец дерева построения. Move the end of the build log. 
// ---
void WorkWindow::SolidEndMoveCmd()
{
  ::PostMessage( hwndFrame, WM_COMMAND, WPARAM(CM_SOLIDENDMOVE), 0 );
}


//------------------------------------------------------------------------------
// Изменить цвет элементов. Change objects color.
// ---
void WorkWindow::SetColorCmd()
{
  ::PostMessage( hwndFrame, WM_COMMAND, WPARAM(CM_SETOBJECTCOLOR), 0 );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Creator's
//
//        Создание отрисовщика
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IGrDraw * CreateDrawTool( HWND wnd, WorkWindow& parent, const MbVector3D & light )
{
  return new HWndGrDraw( wnd, parent, light );
}


#endif // __USE_QT__ 