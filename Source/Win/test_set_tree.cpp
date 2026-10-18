#include <test.h>
#include <test_tree.h>
#include <test_manager.h>
#include <test_variables.h>
#include <last.h>

using namespace c3d;


#ifndef __USE_QT__ 


#define STATIC_HEIGHT 20
#define BUTTON_HEIGHT 24 
#define EDIT_HEIGHT 28

//------------------------------------------------------------------------------
// Processing of messages of model tree window
// ---
LRESULT CALLBACK TreeWndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ) 
{
  switch ( message ) {
    case WM_CLOSE : {
    } break;

    case WM_SIZE : {
      IModelTreeWindow * wp = TestVariables::viewManager->FindTree( (void*)hwnd );
      if ( wp )
        wp->GetTreeTool().Layout();
    } break;

    case WM_COMMAND : {
      IModelTreeWindow * wp = TestVariables::viewManager->FindTree((void*)hwnd);
      if (wp)
        wp->TreeCommand( LOWORD(wParam), HIWORD(wParam), (GetKeyState( VK_RETURN ) & 0x8000) != 0 );
    } break;

    case WM_VKEYTOITEM: {
      IModelTreeWindow * wp = TestVariables::viewManager->FindTree( (void*)hwnd );
      if ( wp ) {
        if ( wp->KeyboardCommand( LOWORD(wParam), (GetKeyState(VK_SHIFT) & 0x8000) != 0, (GetKeyState(VK_CONTROL) & 0x8000) != 0 ) )
          return 0;
      }
    } break;

    case WM_DESTROY : {
      IModelTreeWindow * wp = TestVariables::viewManager->FindTree( (void*)hwnd );
      if ( wp ) {
        wp->PreDestroyCommand( true );
      }
    }
  }

  return DefMDIChildProc( hwnd, message, wParam, lParam );
}

//------------------------------------------------------------------------------
// \ru Создать диалог дерева модели \en Create model tree dialog
// ---
#define CLASS_NAME_TREE  _T("ModelTree")
bool CreateTreeClient ( HWND& hprop, HWND& hchildlist, HWND& hproplist, int& butsize, int& proplistsize, const TCHAR* title, int prop_count, int child_count )
{
  static bool init = false;
  const int nMax = 17; // max number of lines in listbox
  if ( !init )
  {
    init = true;
    WNDCLASS wc;

    // Register ModelTree window class
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)TreeWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInst;
    wc.hIcon         = LoadIcon( hInst, MAKEINTRESOURCE( IDI_ICON_1 ) );
    wc.hCursor       = LoadCursor (nullptr, IDC_ARROW) ;
    wc.hbrBackground = (HBRUSH)GetStockObject (LTGRAY_BRUSH) ;
    wc.lpszMenuName  = nullptr ;
    wc.lpszClassName = CLASS_NAME_TREE;

    RegisterClass( &wc );
  }

  butsize = BUTTON_HEIGHT + 1;
  proplistsize = (STATIC_HEIGHT + 1) * prop_count;
  int optimal_count = std_min(child_count, nMax);
  int optimal_size = (STATIC_HEIGHT + 1) * optimal_count;
  int childlistsize = (STATIC_HEIGHT + 1) * child_count;

  MDICREATESTRUCT    mdicreate ;
  mdicreate.szClass = CLASS_NAME_TREE;
  mdicreate.szTitle = title;
  mdicreate.hOwner  = hInst;
  mdicreate.x       = CW_USEDEFAULT;
  mdicreate.y       = CW_USEDEFAULT;
  mdicreate.cx      = 410; 
  mdicreate.cy      = butsize * 2 + proplistsize + 1 + optimal_size + 1 + EDIT_HEIGHT * 2;
  mdicreate.style   = 0;
  mdicreate.lParam  = 0;//nullptr;

  // tree window
  hprop = (HWND)SendMessage( hwndClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT) &mdicreate );
  PRECONDITION( hprop );

  RECT propRect;
  GetClientRect( hprop, &propRect );

  // buttons
  if (prop_count) {
    int dx = propRect.right / 2;
    CreateWindow( _T("BUTTON"), IDS_TITLE_SELECT, WS_CHILD | WS_VISIBLE,
      0, 0,
      dx, BUTTON_HEIGHT,
      hprop, (HMENU)BUTTON_SELECT , hInst, 0 );

    CreateWindow(_T("BUTTON"), IDS_TITLE_SELECT_CHILD, WS_CHILD | WS_VISIBLE,
      dx, 0,
      dx, BUTTON_HEIGHT,
      hprop, (HMENU)BUTTON_SELECT_CHILD, hInst, 0);
  }
  else {
    int dx = propRect.right / 3 - 1;
    CreateWindow( _T("BUTTON"), IDS_TITLE_SELECT_APPLY, WS_CHILD | WS_VISIBLE,
      0, 0,
      dx, BUTTON_HEIGHT,
      hprop, (HMENU)BUTTON_APPLY, hInst, 0 );

    CreateWindow( _T("BUTTON"), IDS_TITLE_SELECT_ALL, WS_CHILD | WS_VISIBLE,
      dx + 2, 0,
      dx, BUTTON_HEIGHT,
      hprop, (HMENU)BUTTON_SELECT_ALL, hInst, 0 );

    CreateWindow( _T("BUTTON"), IDS_TITLE_SELECT_CHILD2, WS_CHILD | WS_VISIBLE,
      dx * 2 + 2, 0,
      dx, BUTTON_HEIGHT,
      hprop, (HMENU)BUTTON_SELECT_CHILD, hInst, 0 );
  }

  // list of node properties
  hproplist = ::CreateWindow(_T("LISTBOX"), nullptr,
    WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_USETABSTOPS,
    1, butsize,
    propRect.right - 1, proplistsize,
    hprop, (HMENU)H_LIST, hInst, 0);

  // title for children list
  CreateWindow(_T("STATIC"), nullptr, WS_CHILD | WS_VISIBLE,
    0, butsize + proplistsize + 2,
    propRect.right / 6, STATIC_HEIGHT,
    hprop, (HMENU)H_STATIC, hInst, 0);

  CreateWindow(_T("STATIC"),IDS_TITLE_OBJECT_CHILDREN , WS_CHILD | WS_VISIBLE,
    propRect.right / 6, butsize + proplistsize + 2,
    propRect.right * 5 / 6, STATIC_HEIGHT,
    hprop, (HMENU)H_STATIC, hInst, 0);

  // children list
  hchildlist = ::CreateWindow( _T("LISTBOX"), nullptr,
    WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL |
    LBS_NOTIFY | LBS_USETABSTOPS | LBS_WANTKEYBOARDINPUT,
    1, butsize * 2 + proplistsize,
    propRect.right - 1, childlistsize,
    hprop, (HMENU)H_CHILDLIST, hInst, 0 );

  return true;
}
#undef CLASS_NAME_TREE 

//------------------------------------------------------------------------------
// implementation of window part for model tree dialogs
// ---
class HWndTreeTool : public ITreeTool
{
private:
  HWND             hprop;
  HWND             hchildlist;      
  HWND             hproplist;
  int              button_size;
  int              proplist_size;

public:
  // \ru Конструктор // \en Constructor
  HWndTreeTool( HWND _hprop, HWND _hlist, HWND _hedit, int but, int prop )
    : hprop ( _hprop )
    , hchildlist ( _hlist )
    , hproplist ( _hedit )
    , button_size ( but )
    , proplist_size ( prop )
  {}

  // \ru Деструктор // \en Destructor
  virtual ~HWndTreeTool() {
  }

public:
  // \ru Закрыть это окно \en Destroy this window
  virtual void     CloseTreeWindow();

  // \ru Проверить, мое ли это окно \en Check if this window is mine
  virtual bool     IsMyWnd( const void * wndId ) const { return (HWND)wndId == hprop; }

  // \ru Обработчик размещения \en Layout handler
  virtual void     Layout();

  // \ru Обработчики списка свойств объекта \en Handlers of object properties list
  virtual void     SetFocusPropList() { ::SetFocus(hproplist); }
  virtual void     SetCurSelPropList(size_t cur) { ::SendMessage(hproplist, LB_SETCURSEL, cur, 0L); }
  virtual size_t   GetCurSelPropList() { return (size_t)::SendMessage(hproplist, LB_GETCURSEL, 0, 0L); }
  virtual void     SetPropListTexts(RPArray<string_t> & a);

  // \ru Обработчики списка потомков объекта \en Handlers of object children list
  virtual void     SetFocusChildList() { ::SetFocus(hchildlist); }
  virtual void     SetCurSelChildList(size_t cur) { ::SendMessage(hchildlist, LB_SETCURSEL, cur, 0L); }
  virtual size_t   GetCurSelChildList() { return (size_t)::SendMessage(hchildlist, LB_GETCURSEL, 0, 0L); }
  virtual void     SetChildListTexts(RPArray<string_t> & a);
  virtual void     SetChildListText(size_t cur, const TCHAR* s);
};

// \ru Закрыть это окно \en Destroy this window
void HWndTreeTool::CloseTreeWindow() {
  ::PostMessage( hwndClient, WM_MDIDESTROY, (WPARAM)hprop, 0L );
}

// \ru Обработчик списка свойств объекта \en Handler of object properties list
void HWndTreeTool::SetPropListTexts(RPArray<string_t> & a)
{
  ::SendMessage(hproplist, WM_SETREDRAW, false, 0L);
  ::SendMessage(hproplist, LB_RESETCONTENT, 0, 0L);
  for (size_t i = 0; i < a.Count(); ++i)
    ::SendMessage(hproplist, LB_INSERTSTRING, i, (LPARAM)(a[i]->c_str()));

  ::SendMessage(hproplist, WM_SETREDRAW, true, 0L);
}

// \ru Обработчик списка потомков объекта \en Handler of object children list
void HWndTreeTool::SetChildListTexts(RPArray<string_t> & a)
{
  ::SendMessage(hchildlist, WM_SETREDRAW, false, 0L);
  ::SendMessage(hchildlist, LB_RESETCONTENT, 0, 0L);
  for ( size_t i = 0; i < a.Count(); ++i)
    ::SendMessage(hchildlist, LB_INSERTSTRING, i, (LPARAM)(a[i]->c_str()));

  ::SendMessage(hchildlist, WM_SETREDRAW, true, 0L);
}

// \ru Обработчик списка потомков объекта \en Handler of object children list
void HWndTreeTool::SetChildListText(size_t cur, const TCHAR* s)
{
  ::SendMessage(hchildlist, WM_SETREDRAW, false, 0L);
  ::SendMessage(hchildlist, LB_DELETESTRING, cur, 0L);
  ::SendMessage(hchildlist, LB_INSERTSTRING, cur, LPARAM(s));
  ::SendMessage(hchildlist, WM_SETREDRAW, true, 0L);
}

// \ru Обработчик размещения \en Layout handler
void HWndTreeTool::Layout()
{
  size_t currpos_prop = GetCurSelPropList();
  size_t currpos_child = GetCurSelChildList();

  RECT rect;
  GetClientRect( hprop, &rect );
  int  sizeY = rect.bottom - button_size;
  int propY = std_min(sizeY, proplist_size);
  SetWindowPos( hproplist, 0, 0, 0, rect.right - 1, propY, SWP_NOZORDER | SWP_NOMOVE );
  int childY = std_max( sizeY - button_size - propY, 0 );
  SetWindowPos( hchildlist, 0, 0, 0, rect.right - 1, childY, SWP_NOZORDER | SWP_NOMOVE );

  SetCurSelPropList(currpos_prop);
  SetCurSelChildList(currpos_child);
}

//------------------------------------------------------------------------------
// \ru Создать оконную часть диалога дерева модели \en Create window part of model tree dialog
// --- 
ITreeTool * CreateTreeTool( HWND hprop, HWND hchildlist, HWND hproplist, int butsize, int proplistsize) {
  return new HWndTreeTool( hprop, hchildlist, hproplist, butsize, proplistsize );
}

//==============================================================================

//------------------------------------------------------------------------------
// Processing of messages of embodiment tree window
// ---
LRESULT CALLBACK EmbTreeWndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
  switch ( message ) {
  case WM_CLOSE:
  {
  } break;

  case WM_SIZE:
  {
    IModelTreeWindow * wp = TestVariables::viewManager->FindTree( ( void* )hwnd );
    if ( wp )
      wp->GetTreeTool().Layout();
  } break;

  case WM_COMMAND:
  {
    IModelTreeWindow * wp = TestVariables::viewManager->FindTree( ( void* )hwnd );
    if ( wp )
      wp->TreeCommand( LOWORD( wParam ), HIWORD( wParam ), ( GetKeyState( VK_RETURN ) & 0x8000 ) != 0 );
  } break;

  case WM_VKEYTOITEM:
  {
    IModelTreeWindow * wp = TestVariables::viewManager->FindTree( ( void* )hwnd );
    if ( wp ) {
      if ( wp->KeyboardCommand( LOWORD( wParam ), ( GetKeyState( VK_SHIFT ) & 0x8000 ) != 0, ( GetKeyState( VK_CONTROL ) & 0x8000 ) != 0 ) )
        return 0;
    }
  } break;

  case WM_DESTROY:
  {
    IModelTreeWindow * wp = TestVariables::viewManager->FindTree( ( void* )hwnd );
    if ( wp ) {
      wp->PreDestroyCommand( true );
    }
  }
  }

  return DefMDIChildProc( hwnd, message, wParam, lParam );
}

// \ru Создать диалог дерева исполнений \en Create embodiment tree dialog
#define CLASS_NAME_EMB  _T("EmbodimentTree")
bool CreateEmbTreeClient( HWND & hprop, HWND& hchildlist, HWND& hproplist, int& butsize, int& proplistsize, const TCHAR * title, int prop_count, int child_count )
{
  static bool init = false;
  const int nMax = 6; // max number of lines in listbox
  if ( !init ) {
    init = true;
    WNDCLASS wc;

    // Register ModelTree window class
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = ( WNDPROC )EmbTreeWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon( hInst, MAKEINTRESOURCE( IDI_ICON_1 ) );
    wc.hCursor = LoadCursor( nullptr, IDC_ARROW );
    wc.hbrBackground = ( HBRUSH )GetStockObject( LTGRAY_BRUSH );
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = CLASS_NAME_EMB;

    RegisterClass( &wc );
  }

  butsize = BUTTON_HEIGHT + 1;
  proplistsize = ( STATIC_HEIGHT + 1 ) * prop_count;
  int optimal_count = std_min( child_count, nMax );
  int optimal_size = ( STATIC_HEIGHT + 1 ) * optimal_count;
  int childlistsize = ( STATIC_HEIGHT + 1 ) * child_count;

  MDICREATESTRUCT    mdicreate;
  mdicreate.szClass = CLASS_NAME_EMB;
  mdicreate.szTitle = title;
  mdicreate.hOwner = hInst;
  mdicreate.x = CW_USEDEFAULT;
  mdicreate.y = CW_USEDEFAULT;
  mdicreate.cx = 410;
  mdicreate.cy = butsize * 2 + proplistsize + 1 + optimal_size + 1 + EDIT_HEIGHT * 2;
  mdicreate.style = 0;
  mdicreate.lParam = 0;//nullptr;

  // tree window
  hprop = ( HWND )SendMessage( hwndClient, WM_MDICREATE, 0, ( LPARAM )( LPMDICREATESTRUCT )&mdicreate );
  PRECONDITION( hprop );

  RECT propRect;
  GetClientRect( hprop, &propRect );

  // buttons
  int dx = propRect.right / 2;
  CreateWindow( _T( "BUTTON" ), IDS_TITLE_SELECT_APPLY, WS_CHILD | WS_VISIBLE,
    0, 0,
    dx, BUTTON_HEIGHT,
    hprop, ( HMENU )BUTTON_APPLY, hInst, 0 );

  CreateWindow( _T( "BUTTON" ), IDS_TITLE_SELECT_CHILD2, WS_CHILD | WS_VISIBLE,
    dx, 0,
    dx, BUTTON_HEIGHT,
    hprop, ( HMENU )BUTTON_SELECT_CHILD, hInst, 0 );

  // list of node properties
  hproplist = ::CreateWindow( _T( "LISTBOX" ), nullptr,
    WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_USETABSTOPS,
    1, butsize,
    propRect.right - 1, proplistsize,
    hprop, ( HMENU )H_LIST, hInst, 0 );

  // title for children list
  CreateWindow( _T( "STATIC" ), nullptr, WS_CHILD | WS_VISIBLE,
    0, butsize + proplistsize + 2,
    propRect.right / 6, STATIC_HEIGHT,
    hprop, ( HMENU )H_STATIC, hInst, 0 );

  CreateWindow( _T( "STATIC" ), prop_count ? IDS_TITLE_OBJECT_CHILDREN : nullptr, WS_CHILD | WS_VISIBLE,
    propRect.right / 6, butsize + proplistsize + 2,
    propRect.right * 5 / 6, STATIC_HEIGHT,
    hprop, ( HMENU )H_STATIC, hInst, 0 );

  // children list
  hchildlist = ::CreateWindow( _T( "LISTBOX" ), nullptr,
    WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL |
    LBS_NOTIFY | LBS_USETABSTOPS | LBS_WANTKEYBOARDINPUT,
    1, butsize * 2 + proplistsize,
    propRect.right - 1, childlistsize,
    hprop, ( HMENU )H_CHILDLIST, hInst, 0 );

  return true;
}
#undef CLASS_NAME_EMB

//------------------------------------------------------------------------------
// implementation of window part for embodiment tree dialogs
// ---
class HWndEmbTool : public ITreeTool
{
private:
  HWND             hprop;
  HWND             hchildlist;
  HWND             hproplist;
  int              button_size;
  int              proplist_size;

public:
  // \ru Конструктор // \en Constructor
  HWndEmbTool( HWND _hprop, HWND _hlist, HWND _hedit, int but, int prop )
    : hprop( _hprop )
    , hchildlist( _hlist )
    , hproplist( _hedit )
    , button_size( but )
    , proplist_size( prop )
  {}

  // \ru Деструктор // \en Destructor
  virtual ~HWndEmbTool() {
  }

public:
  // \ru Закрыть это окно \en Destroy this window
  virtual void     CloseTreeWindow();

  // \ru Проверить, мое ли это окно \en Check if this window is mine
  virtual bool     IsMyWnd( const void * wndId ) const { return ( HWND )wndId == hprop; }

  // \ru Обработчик размещения \en Layout handler
  virtual void     Layout();

  // \ru Обработчики списка свойств объекта \en Handlers of object properties list
  virtual void     SetFocusPropList() { ::SetFocus( hproplist ); }
  virtual void     SetCurSelPropList( size_t cur ) { ::SendMessage( hproplist, LB_SETCURSEL, cur, 0L ); }
  virtual size_t   GetCurSelPropList() { return ( size_t )::SendMessage( hproplist, LB_GETCURSEL, 0, 0L ); }
  virtual void     SetPropListTexts( RPArray<string_t> & a );

  // \ru Обработчики списка потомков объекта \en Handlers of object children list
  virtual void     SetFocusChildList() { ::SetFocus( hchildlist ); }
  virtual void     SetCurSelChildList( size_t cur ) { ::SendMessage( hchildlist, LB_SETCURSEL, cur, 0L ); }
  virtual size_t   GetCurSelChildList() { return ( size_t )::SendMessage( hchildlist, LB_GETCURSEL, 0, 0L ); }
  virtual void     SetChildListTexts( RPArray<string_t> & a );
  virtual void     SetChildListText( size_t cur, const TCHAR* s );
};

// \ru Закрыть это окно \en Destroy this window
void HWndEmbTool::CloseTreeWindow() {
  ::PostMessage( hwndClient, WM_MDIDESTROY, ( WPARAM )hprop, 0L );
}

// \ru Обработчик списка свойств объекта \en Handler of object properties list
void HWndEmbTool::SetPropListTexts( RPArray<string_t> & a )
{
  ::SendMessage( hproplist, WM_SETREDRAW, false, 0L );
  ::SendMessage( hproplist, LB_RESETCONTENT, 0, 0L );
  for ( size_t i = 0; i < a.Count(); ++i )
    ::SendMessage( hproplist, LB_INSERTSTRING, i, ( LPARAM )( a[i]->c_str() ) );

  ::SendMessage( hproplist, WM_SETREDRAW, true, 0L );
}

// \ru Обработчик списка потомков объекта \en Handler of object children list
void HWndEmbTool::SetChildListTexts( RPArray<string_t> & a )
{
  ::SendMessage( hchildlist, WM_SETREDRAW, false, 0L );
  ::SendMessage( hchildlist, LB_RESETCONTENT, 0, 0L );
  for ( size_t i = 0; i < a.Count(); ++i )
    ::SendMessage( hchildlist, LB_INSERTSTRING, i, ( LPARAM )( a[i]->c_str() ) );

  ::SendMessage( hchildlist, WM_SETREDRAW, true, 0L );
}

// \ru Обработчик списка потомков объекта \en Handler of object children list
void HWndEmbTool::SetChildListText( size_t cur, const TCHAR* s )
{
  ::SendMessage( hchildlist, WM_SETREDRAW, false, 0L );
  ::SendMessage( hchildlist, LB_DELETESTRING, cur, 0L );
  ::SendMessage( hchildlist, LB_INSERTSTRING, cur, LPARAM( s ) );
  ::SendMessage( hchildlist, WM_SETREDRAW, true, 0L );
}

// \ru Обработчик размещения \en Layout handler
void HWndEmbTool::Layout()
{
  size_t currpos_prop = GetCurSelPropList();
  size_t currpos_child = GetCurSelChildList();

  RECT rect;
  GetClientRect( hprop, &rect );
  int  sizeY = rect.bottom - button_size;
  int propY = std_min( sizeY, proplist_size );
  SetWindowPos( hproplist, 0, 0, 0, rect.right - 1, propY, SWP_NOZORDER | SWP_NOMOVE );
  int childY = std_max( sizeY - button_size - propY, 0 );
  SetWindowPos( hchildlist, 0, 0, 0, rect.right - 1, childY, SWP_NOZORDER | SWP_NOMOVE );

  SetCurSelPropList( currpos_prop );
  SetCurSelChildList( currpos_child );
}


// \ru Создать оконную часть диалога дерева исполнений \en Create window part of embodiment tree dialog
ITreeTool * CreateEmbTreeTool( HWND hprop, HWND hchildlist, HWND hproplist, int butsize, int proplistsize )
{
  return new HWndEmbTool( hprop, hchildlist, hproplist, butsize, proplistsize );
}


#endif // __USE_QT__ 
