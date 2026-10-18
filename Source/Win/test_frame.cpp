#ifndef __USE_QT__ 

#include <test.h>
#include <test_prompt.h>
#include <test_frame.h>
#include <test_manager.h>
#include <test_service.h>
#include <test_draw.h>
#if !defined( __WIN32__ )
#include <commdlg.h>
#endif
#ifdef UNICODE
#include<codecvt>
#endif 

#include <last.h>

using namespace c3d;


HMENU     hMenuInitWindow;
HMENU     hMenuInit;
HWND      hStatusBar;

#define X_POS   5
#define Y_POS  85
#define Z_POS 165
#define M_POS 255
#define COORD_POS    2
#define COORD_LEN  240
#define PROMPT_POS 400
#define PROMPT_LEN 350


/////////////////////////////////////////////////////////////////////////////////////////
//
//  Status Bar
//
/////////////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Отображение координат и других элементов окна подсказки
// ---
LRESULT CALLBACK StatusBarWndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ) {

  switch( message ) {

    case SHOW_COORDINATES:
    {
      if( TestVariables::viewManager != nullptr ) {
        MbCartPoint3D *p3D = (MbCartPoint3D *)lParam;
        TCHAR s[ 128 ];
        HDC hdc = GetDC( hwnd );

        SetBkColor( hdc, TestVariables::LIGHTGRAY );

        _stprintf( s, _T( "x=%-9.3f  " ), p3D->x );
        TextOut( hdc, X_POS, 3, s, lstrlen( s ) );

        _stprintf( s, _T( "y=%-9.3f  " ), p3D->y );
        TextOut( hdc, Y_POS, 3, s, lstrlen( s ) );

        _stprintf( s, _T( "z=%-9.3f  " ), p3D->z );
        TextOut( hdc, Z_POS, 3, s, lstrlen( s ) );

        _stprintf( s, _T( "M=%-9.3f  " ), TestVariables::viewManager->GetWindowScale() );
        TextOut( hdc, M_POS, 3, s, lstrlen( s ) );

        ReleaseDC( hwnd, hdc );
      }
      return 0;
    }

    case SHOW_STRING:
    {
      TCHAR *str = (TCHAR *)lParam;
      HDC dc = GetDC( hwnd );
      RECT r = { PROMPT_POS + 1, 3, PROMPT_POS + PROMPT_LEN - 1, SPEEDBAR_HEIGHT - 5 };
      FillRect( dc, &r, (HBRUSH)GetStockObject( LTGRAY_BRUSH ) );

      SetBkColor( dc, TestVariables::LIGHTGRAY );
      TextOut( dc, PROMPT_POS + 5, 3, str, lstrlen( str ) );

      ReleaseDC( hwnd, dc );

      return 0;
    }
  }

  return DefMDIChildProc( hwnd, message, wParam, lParam );
}


#define CLASS_NAME_STATUSBAR  _T("SPEEDBAR")
//------------------------------------------------------------------------------
// Создать StatusBar
// ---
bool CreateStatusBar()
{
  static bool init = false;
  if( !init )
  {
    init = true;
    WNDCLASS wc;

    // Register speedbar window class
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)StatusBarWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = (HBRUSH)GetStockObject( LTGRAY_BRUSH );
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = CLASS_NAME_STATUSBAR;

    if( !RegisterClass( &wc ) )
      return false;
  }

  hStatusBar = ::CreateWindow( CLASS_NAME_STATUSBAR, nullptr,
                               WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_BORDER,
                               0, 0, 1, 1, hwndFrame, (HMENU)2, hInst, 0 );

  if( !hStatusBar )
  {
    PRECONDITION( false );
    return false;
  }

  LOGFONT lf;
  lf.lfHeight = 19;
  lf.lfWidth = 0;
  lf.lfEscapement = 0;
  lf.lfOrientation = 0;
  lf.lfWeight = FW_BOLD;
  lf.lfItalic = 0;
  lf.lfUnderline = 0;
  lf.lfStrikeOut = 0;
  lf.lfCharSet = ANSI_CHARSET;
  lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
  lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
  lf.lfQuality = DEFAULT_QUALITY;
  lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
  _tcscpy( lf.lfFaceName, _T( "Courier New Cyr" ) );

  HFONT hfont = CreateFontIndirect( (LPLOGFONT)&lf );
  SendMessage( hStatusBar, WM_SETFONT, (WPARAM)hfont, (LPARAM)MAKELONG( (WORD)TRUE, 0 ) );
  ::DeleteObject( hfont ); // Была утечка, нужно удалять

  return true;
}
#undef CLASS_NAME_STATUSBAR


//------------------------------------------------------------------------------
// Удалить StatusBar
// ---
void DestroyStatusBar()
{
  ::DeleteObject( hStatusBar );
}


//------------------------------------------------------------------------------
// Отрисовать текст
// ---
void SetStatusBarText( const TCHAR* txt )
{
  SendMessage( hStatusBar, SHOW_STRING, 0, (LPARAM)( txt ) );
}


//------------------------------------------------------------------------------
// Отрисовать координаты точки
// ---
void SetStatusBarPoint( const MbCartPoint3D& pnt )
{
  SendMessage( hStatusBar, SHOW_COORDINATES, 0, (WPARAM)( &pnt ) );
}


//------------------------------------------------------------------------------
// 
// ---
void AlignStatusBar( uint clientWidth, uint clientHeight )
{
  SetWindowPos( hStatusBar, 0, 0,
                clientHeight - SPEEDBAR_HEIGHT, clientWidth, SPEEDBAR_HEIGHT,
                SWP_NOZORDER );
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//  MDI Client
//
/////////////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Создать окно MDI-client
// ---
bool CreateClientWindow()
{
  CLIENTCREATESTRUCT clientcreate;
  clientcreate.hWindowMenu = hMenuInitWindow;
  clientcreate.idFirstChild = CM_FIRSTCHILD;

  hwndClient = CreateWindow( _T( "MDICLIENT" ), nullptr,
                             WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                             CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                             hwndFrame, (HMENU)1, hInst,
                             (CLIENTCREATESTRUCT *)&clientcreate );
  if( !hwndClient )
  {
    PRECONDITION( false );
    return false;
  }
  return true;
}


//------------------------------------------------------------------------------
// Удалить окно MDI-client
// ---
void DestroyClientWindow()
{
  ::DeleteObject( hwndClient );
}


//------------------------------------------------------------------------------
//
// ---
void AlignClientWindow( uint clientWidth, uint clientHeight )
{
  SetWindowPos( hwndClient, 0, 0, 0, clientWidth, clientHeight - SPEEDBAR_HEIGHT, SWP_NOZORDER | SWP_NOMOVE );
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//  Main Frame
//
/////////////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Процедура обработки сообщений
// ---
bool FAR PASCAL CloseEnumProc( HWND hwnd, LONG )
{
  if( ::GetWindow( hwnd, GW_OWNER ) )         // check for icon title
    return 1;

  ::SendMessage( ::GetParent( hwnd ), WM_MDIRESTORE, (WPARAM)hwnd, 0L );

  if( !::SendMessage( hwnd, WM_QUERYENDSESSION, 0, 0L ) )
    return 1;

  ::SendMessage( ::GetParent( hwnd ), WM_MDIDESTROY, (WPARAM)hwnd, 0L );
  return 1;
}


//------------------------------------------------------------------------------
// 
// ---
void CommandClientWindow( uint command )
{
  bool maximizeSingleWindow = false;

  switch( command )
  {
    case CM_WINDOWTILE:    // Мозаика окон
    {
      ::SendMessage( hwndClient, WM_MDITILE, 0, 0L );
      maximizeSingleWindow = true;
    }
    break;

    case CM_WINDOWCASCADE: // Каскад окон
    {
      ::SendMessage( hwndClient, WM_MDICASCADE, 0, 0L );
      maximizeSingleWindow = true;
    }
    break;

    case CM_WINDOWARRANGE:
    {
      ::SendMessage( hwndClient, WM_MDIICONARRANGE, 0, 0L );
      maximizeSingleWindow = true;
    }
    break;

    case CM_WINDOWCLOSE_ALL: // Attempt to close all children
    {
      ::EnumChildWindows( hwndClient, (WNDENUMPROC)CloseEnumProc, 0L );
    }
    break;

    case CM_WINDOWCLOSE_ACTIVE: // Закрыть активное окно
    {
      HWND hwndChild = ( HWND )::SendMessage( hwndClient, WM_MDIGETACTIVE, 0, 0L );
      if( ::SendMessage( hwndChild, WM_QUERYENDSESSION, 0, 0L ) )
        ::SendMessage( hwndClient, WM_MDIDESTROY, (WPARAM)hwndChild, 0L );
      maximizeSingleWindow = true;
    }
    break;

    case CM_WINDOWCLOSE_EMPTY: // Delete empty windows
    {
      if( TestVariables::viewManager ) {
        ptrdiff_t wndCnt = TestVariables::viewManager->GetWindowsCount();
        for( ptrdiff_t k = wndCnt; k--; ) {
          WorkWindow * wnd = TestVariables::viewManager->GetWindow( k );
          if( ( wnd == nullptr ) || wnd->IsModelEmpty() ) {
            TestVariables::viewManager->SetFocus( k, false );
            HWND hwndChild = ( HWND )::SendMessage( hwndClient, WM_MDIGETACTIVE, 0, 0L );
            if( ::SendMessage( hwndChild, WM_QUERYENDSESSION, 0, 0L ) )
              ::SendMessage( hwndClient, WM_MDIDESTROY, (WPARAM)hwndChild, 0L );
          }
        }
        wndCnt = TestVariables::viewManager->GetWindowsCount();
        if( wndCnt > 0 ) {
          if( TestVariables::viewManager->GetActiveWindowIndex() >= wndCnt )
            TestVariables::viewManager->SetFocus( 0, false );
          maximizeSingleWindow = true;
        }
      }
    }
    break;
  }

  if( maximizeSingleWindow ) {
    if( TestVariables::viewManager && TestVariables::viewManager->GetWindowsCount() == 1 ) {
      //      HWND hwndChild = (HWND)::SendMessage( hwndClient, WM_MDIGETACTIVE, 0, 0L );
      //      ::SendMessage( hwndClient, WM_MDIMAXIMIZE, (WPARAM)hwndChild, 0L ); // Сделать окно максимально развернутым.
    }
  }
}

//------------------------------------------------------------------------------
// Процедура обработки сообщения WM_DROPFILES
// ---
void DropFilesHandler( HDROP param )
{
  const uint bufferSize = 0xFFFF;
  TCHAR buffer[ bufferSize ];

  UINT filesCount = DragQueryFile( param, 0xFFFFFFFF, buffer, bufferSize );

  for( UINT i = 0; i < filesCount; ++i )
  {
    if( 0 < DragQueryFile( param, i, buffer, bufferSize ) )
    {
      const TCHAR * validExts = _T( ".C3D.SAT.IGS.IGES.X_T.STP.STEP.JT.STL.WRL.OBJ.GRDECL" );
      const TCHAR * ext = _tcsrchr( buffer, '.' );

      if( nullptr != ext )
      {
        TCHAR * extUp = _tcsupr( _tcsdup( ext ) );

        if( nullptr != _tcsstr( validExts, extUp ) )
        {
          WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
          bool needNewWindow = false;

          if( nullptr == wind )
            needNewWindow = true;
          else
          {
            if( !wind->IsModelNameEmpty() )
              needNewWindow = true;
          }

          if( needNewWindow )
          {
            wind = new WorkWindow( v_Isometry );
            wind->CreateWorkWindow( IDS_TITLE_VIEWISOMETRY,
                                    TestVariables::viewManager->GetBackgrColor(),
                                    TestVariables::viewManager->GetLightDirection()/*lightDirection*/ );
            wind->OrientateWindow( v_Isometry );

            TestVariables::viewManager->SetFocus( TestVariables::viewManager->AddWindow( wind ), false );
          }

          ReadModel( buffer );
        }

        free( extUp );
      }
    }
  }
}

//------------------------------------------------------------------------------
// Процедура обработки команд окна задачи
// ---
LRESULT CALLBACK FrameWndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ) {

  HWND hwndChild;

  switch( message ) {
    case WM_CREATE:
    {        // Create the client window
      hwndFrame = hwnd;

      SystemInitFrame();

      CreateClientWindow();
      CreateStatusBar();

      // Окна для работы с видами
      PostMessage( hwnd, WM_COMMAND, CM_VIEWISO, 0 );   // Изометрия
      PostMessage( hwnd, WM_COMMAND, CM_WINDOWTILE, 0 );// Расставить окна "плитками"
      PostMessage( hwnd, WM_COMMAND, CM_PTORIGIN, 0 ); // Центрировать начало координат
      //PostMessage( hwnd, WM_COMMAND, CM_VIEWFRONT, 0 ); // Вид спереди
      //PostMessage( hwnd, WM_COMMAND, CM_PTORIGIN, 0 );

      return 0;
    }

    case WM_SIZE:
    {
      WORD nWidth = LOWORD( lParam ); // width of client area
      WORD nHeight = HIWORD( lParam ); // height of client area
      AlignClientWindow( nWidth, nHeight );
      AlignStatusBar( nWidth, nHeight );
      return 0;
    }

    case WM_QUERYENDSESSION:
    case WM_CLOSE:
    {            // Закрыть все дочерние окна
      if( TestVariables::viewManager != nullptr ) {
        // Сначала закроем все дочерние окна, в которых могут хранится указатели на объекты модели
        // Иначе будет вылет при попытке закрыть приложение с открытыми окнами свойств объектов
        CommandClientWindow( CM_WINDOWCLOSE_ALL );

        TestVariables::viewManager->DeleteGL();
        // KYA выполняется в DeleteModel // viewManager->EraseModel();
        TestVariables::viewManager->DeleteModel(); // Удалить все элементы

        SendMessage( hwnd, WM_COMMAND, CM_WINDOWCLOSE_ALL, 0L );

        delete TestVariables::drawImp;
        delete TestVariables::viewManager;

        DestroyStatusBar();
        DestroyClientWindow();
      }

      TestVariables::viewManager = nullptr;
    } break;

    case WM_DESTROY:
    {
      PostQuitMessage( 0 );
      return 0;
    }

    case WM_MENUSELECT:
    {
      if( TestVariables::waitFlag > 0 ) { // Если не закончена предыдущая команда
        TestVariables::waitFlag = -1;
        SetStatusBarText( _T( "" ) );
        if( TestVariables::viewManager != nullptr ) {
          WorkWindow * activeWindow = TestVariables::viewManager->GetActiveWindow();
          if( activeWindow != nullptr )
            activeWindow->BreakProc();
        }
      }
    } break;

    // Обработка команд меню
    case WM_COMMAND:
    {

      if( TestVariables::viewManager != nullptr ) {
        switch( wParam ) {
          case CM_TESTEXIT:
          { // Выход из программы
            SendMessage( hwnd, WM_CLOSE, 0, 0L );
            return 0;
          }

          case CM_WINDOWTILE:         // Мозаика окон
          case CM_WINDOWCASCADE:      // Каскад окон
          case CM_WINDOWARRANGE:
          case CM_WINDOWCLOSE_ACTIVE: // Close active window
          case CM_WINDOWCLOSE_ALL:    // Attempt to close all children windows
          case CM_WINDOWCLOSE_EMPTY:  // Close empty windows
            CommandClientWindow( (uint)wParam );
            return 0;

          case CM_VIEWISOPROJ:
          case CM_VIEWRIGHTPROJ:
          case CM_VIEWLEFTPROJ:
          case CM_VIEWDOWNPROJ:
          case CM_VIEWUPPROJ:
          case CM_VIEWREARPROJ:
          case CM_VIEWFRONTPROJ:
          case CM_IGES_READ:
          case CM_SAT_READ:
          case CM_STEP_READ:
          case CM_XT_READ:
          case CM_FILETAKE:
          case CM_FILEOPEN:
          { // Прочитать модель из файла
            WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();

            bool createNewWindow = false;

            if( wParam == CM_FILEOPEN ) {
              if( ( wind != nullptr && !wind->IsModelEmpty() ) || ( wind == nullptr ) )
                createNewWindow = true;
            }
            else {
              if( wind == nullptr )
                createNewWindow = true;
            }

            if( createNewWindow ) {
              //PostMessage( hwnd, WM_COMMAND, CM_WINDOWCASCADE, 0 );// Расставить окна "каскдом"
              PostMessage( hwnd, WM_COMMAND, CM_VIEWISO, 0 );  // Изометрия
              PostMessage( hwnd, WM_COMMAND, CM_PTORIGIN, 0 ); // Центрировать начало координат
              PostMessage( hwnd, WM_COMMAND, wParam, 0 ); // Прочитать модель из файла в вновь открытое окно.
              return 0;
            }
          }

          default:
          {
            if( MenuCommand( (uint)wParam ) ) {
              // Для команд этой группы сохранять wPrev, lPrev, cmPrev не надо
            }
            else {
              if( CreateSpaceObjects( (uint)wParam ) ||
                  CreatePlaneObjects( (uint)wParam ) ||
                  MakeAll( (uint)wParam ) ) {
                TestVariables::wPrev = wParam;
                TestVariables::lPrev = 0;//lParam;
                TestVariables::cmPrev = wParam;
                return 0;
              }
              else { // Команда не обработанна - передаем ее child'у
                // Pass to active child
                hwndChild = ( HWND )::SendMessage( hwndClient, WM_MDIGETACTIVE, 0, 0L );
                if( ::IsWindow( hwndChild ) )
                  ::SendMessage( hwndChild, WM_COMMAND, wParam, lParam );
              }
            }
          }
          break; // default and then to DefFrameProc
        }
      }
    }
    break; // WM_COMMAND
    //return 0; // WM_COMMAND обрабатывать DefFrameProc нельзя

    case WM_DROPFILES:
      DropFilesHandler( (HDROP)wParam );
      break;
  }

  // Pass unprocessed messages to DefFrameProc (not DefWindowProc)
  return DefFrameProc( hwnd, hwndClient, message, wParam, lParam );
}


#define CLASS_NAME_FRAME  _T("3D_FRAME")
//------------------------------------------------------------------------------
// Создать главное окно
// ---
bool CreateFrameWindow()
{
  //const TCHAR  szFrameClass[] = _T("3D Frame");

  static bool init = false;

  if( !init )
  {
    init = true;
    WNDCLASS wc;

    // Register the frame window class
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)FrameWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon( hInst, MAKEINTRESOURCE( IDI_ICON_1 ) );
    wc.hCursor = LoadCursor( nullptr, IDC_ARROW );
    wc.hbrBackground = (HBRUSH)( COLOR_APPWORKSPACE + 1 );
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = CLASS_NAME_FRAME;

    if( !RegisterClass( &wc ) )
      return false;
  }

  HMENU hMenuInit_ = nullptr;

  hMenuInit_ = LoadMenu( hInst, MAKEINTRESOURCE( IDM_MENU_1 ) );
  hMenuInitWindow = GetSubMenu( hMenuInit_, 6 ); // Подцепление открываемых окон

  {
    std::string vi = std::string(", ") + GetC3dLibInfo().substr( 7 );
    vi.erase( vi.size() - 2, 1 ); // erase '.'

  #ifdef UNICODE
    std::wstring title_main_wnd(TITLE_MAIN_WND);
    title_main_wnd.insert( title_main_wnd.size() - 1, StdToWString( vi ) );
  #else
    std::string title_main_wnd(TITLE_MAIN_WND);
    title_main_wnd.insert( title_main_wnd.size() - 1, vi );
  #endif

    hwndFrame = ::CreateWindow( CLASS_NAME_FRAME, title_main_wnd.c_str(),
                                WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                ::GetSystemMetrics( SM_CXSCREEN ) - 2, ::GetSystemMetrics( SM_CYSCREEN ) - 2,
                                nullptr, hMenuInit_, hInst, nullptr );
  }

  if( !hwndFrame )
  {
    PRECONDITION( false );
    return false;
  }

  DragAcceptFiles( hwndFrame, TRUE );

  ShowWindow( hwndFrame, SW_SHOWMAXIMIZED );//nCmdShow);
  UpdateWindow( hwndFrame );

  return true;
}
#undef CLASS_NAME_FRAME


//------------------------------------------------------------------------------
// Удалить главное окно задачи
// ---
void DestroyFrameWindow()
{
  ::DestroyMenu( hMenuInit );
  ::DestroyMenu( hMenuInitWindow );
  ::DeleteObject( hwndFrame );  // Надо удалять утекали ресурсы
}


//-------------------------------------------------------------------------------
// Запрос у пользователя имени файла для записи
// ---
bool _GetSaveFileName( TCHAR * nameBuffer, const TCHAR * title, const TCHAR * filter, const TCHAR * extension )
{
  TCHAR szFile[ 256 ];
  szFile[ 0 ] = _T( '\0' );

  OPENFILENAME ofn;
  ::memset( &ofn, 0, sizeof( OPENFILENAME ) );
  ofn.lStructSize = sizeof( OPENFILENAME );
  ofn.hwndOwner = hwndFrame;
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof( szFile );
  ofn.lpstrTitle = title;
  ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;

  ofn.nFilterIndex = SAVE_NUMBER_C3D;
  if( ( filter != nullptr ) || ( extension != nullptr ) )
    ofn.lpstrFilter = filter;
  else { // множественный выбор форматов
    ofn.lpstrFilter = SAVE_EXT_INFO;
    extension = NON_EXT;
  }

  ofn.lpstrDefExt = C3D_EXT + 1; // заставляем GetSaveFileName менять расширения

  if( _tcslen( nameBuffer ) <= 1 ) {
    _tcscpy( nameBuffer, ALL_EXT );  // Все файлы
  }

  bool multiSelect = false;
  if( _tcscmp( extension, NON_EXT ) == 0 ) {
    extension = C3D_EXT;
    multiSelect = true;
  }

  ::RewriteFileExtension( nameBuffer, extension, true ); // Перебить или дописать расширение
  _tcscpy( ofn.lpstrFile, nameBuffer ); // Скопировать из буфера

  if( GetSaveFileName( &ofn ) ) { // Получить имя файла из диалога
    if( multiSelect ) {
      extension = C3D_EXT;
      switch( ofn.nFilterIndex )
      {
        case SAVE_NUMBER_C3D:    extension = C3D_EXT;    break;
        case SAVE_NUMBER_SAT:    extension = SAT_EXT;    break;
        case SAVE_NUMBER_IGS:    extension = IGS_EXT;    break;
        case SAVE_NUMBER_X_T:    extension = X_T_EXT;    break;
        case SAVE_NUMBER_STP:    extension = STP_EXT;    break;
        case SAVE_NUMBER_JT:     extension =  JT_EXT;    break;
        case SAVE_NUMBER_STL:    extension = STL_EXT;    break;
        case SAVE_NUMBER_WRL:    extension = VRML_EXT;   break;
        case SAVE_NUMBER_GRDECL: extension = GRDECL_EXT; break;
        case SAVE_NUMBER_ASCII:  extension = TXT_EXT;    break;
        case SAVE_NUMBER_OBJ:    extension = OBJ_EXT;    break;
        case SAVE_NUMBER_3MF:    extension = THREEMF_EXT;    break;
        case SAVE_NUMBER_SAB:    extension = SAB_EXT;    break;
        case SAVE_NUMBER_X_B:    extension = X_B_EXT;    break;
        default:                 extension = C3D_EXT;    break;
      }
    }

    _tcscpy( nameBuffer, ofn.lpstrFile ); // Взять название нового файла в буфер
    ::RewriteFileExtension( nameBuffer, extension, true ); // Перебить или дописать расширение
    return true;
  }
  else
    return false;
}


//-------------------------------------------------------------------------------
// Запрос у пользователя имени файла для чтения
// ---
bool _GetOpenFileName( TCHAR * nameBuffer, const TCHAR * title, const TCHAR * filter, const TCHAR * extension )
{
  TCHAR szFile[ 256 ];
  szFile[ 0 ] = _T( '\0' );

  OPENFILENAME ofn;
  memset( &ofn, 0, sizeof( OPENFILENAME ) );
  ofn.lStructSize = sizeof( OPENFILENAME );
  ofn.hwndOwner = hwndFrame;
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof( szFile );
  ofn.lpstrTitle = title;
  ofn.Flags = OFN_HIDEREADONLY;

  ofn.nFilterIndex = SAVE_NUMBER_C3D;
  if( ( filter != nullptr ) || ( extension != nullptr ) )
    ofn.lpstrFilter = filter;

  if( _tcslen( nameBuffer ) <= 1 )
    _tcscpy( nameBuffer, ALL_EXT ); // Все файлы

  ::RewriteFileExtension( nameBuffer, extension, false );
  _tcscpy( ofn.lpstrFile, nameBuffer );

  if( GetOpenFileName( &ofn ) ) {
    _tcscpy( nameBuffer, ofn.lpstrFile );
    ::RewriteFileExtension( nameBuffer, extension, false ); // Не открывал, если изменить существующее имя при открытии
    return true;
  }
  else
    return false;
}


//------------------------------------------------------------------------------
// 
// ---
COLORREF  GetSystemWindowColor()
{
  return GetSysColor( COLOR_WINDOW );
}


//------------------------------------------------------------------------------
// Установить/снять курсор в виде песочных часов
// ---
void SetWaitCursor( bool set )
{
  static int count = 0;
  static HCURSOR save = 0;

  if( set )
  {
    if( count == 0 )
      save = SetCursor( LoadCursor( nullptr, IDC_WAIT ) );
    ++count;
  }
  else
  {
    if( count == 1 )
    {
      SetCursor( save );
      save = 0;
    }
    --count;
  }
}


//------------------------------------------------------------------------------
// Проверить очередь клавиатуры на нажатие клавиши [ESC] 
// ---
bool PeekEscape()
{
  MSG msg; // Сообщения клавиатуры
  if( ::PeekMessage( &msg, 0, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE ) )
    if( ( msg.wParam == (WPARAM)VK_ESCAPE ) && ( msg.message == WM_KEYDOWN ) )
      return true;

  return false;
}


//------------------------------------------------------------------------------
// Ожидание нажатия на клавиатуру
// ---
bool WaitingKeyboardEvent()
{
  bool isEsc = false;
  bool isWaiting = true;
  while( isWaiting ) {
    MSG msg;
    msg.hwnd = 0;
    msg.message = 0;
    msg.wParam = 0;
    msg.lParam = 0;
    msg.time = 0;

    isWaiting = !( ::PeekMessage( &msg, 0, WM_KEYUP, WM_KEYUP, PM_REMOVE ) );
    if( isWaiting
        //OV_Lnx - не реализовано: -------------------------------------------
        // && GetActiveWindow() != hwndFrame // Чтобы не было зависания при переключении на другое окно
        //OV_Lnx - не реализовано: -------------------------------------------
        )
      isWaiting = false;

    if( (WPARAM)msg.wParam == VK_ESCAPE )
      isEsc = true;
  }
  return isEsc;
}


//------------------------------------------------------------------------------
// Синхронизовать индекс активного окна с менеджером окон
// ---
void SyncActiveWindowIndex()
{
  const void * activeWndId = (void *)(HWND)SendMessage( hwndClient, WM_MDIGETACTIVE, 0, 0L );
  if( activeWndId ) {
    ptrdiff_t index = TestVariables::viewManager->FindWindowIndex( activeWndId, false );
    if( 0 <= index && index < TestVariables::viewManager->GetWindowsCount() )
      TestVariables::viewManager->SetActiveWindow( index );
  }
}

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// 
// ---
bool isShiftPressed()
{
  return ( GetKeyState( VK_SHIFT ) & 0x8000 ) != 0;
}

//------------------------------------------------------------------------------
// 
// ---
bool isCtrlPressed()
{
  return ( GetKeyState( VK_CONTROL ) & 0x8000 ) != 0;
}

//------------------------------------------------------------------------------
// 
// ---
bool isCapitalPressed()
{
  return ( GetKeyState( VK_CAPITAL ) & 0x8000 ) != 0;
}


#endif // __USE_QT__ 
