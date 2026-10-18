#include "test_tree_filter.h"
#include "test_variables.h"
#include <tool_cstring.h>
#include <templ_p_array.h>
#include <test.h>
#include <test_manager.h>
#include <last.h>

using namespace c3d;

#ifndef __USE_QT__ 

#define STATIC_HEIGHT 20
#define BUTTON_HEIGHT 24 
#define EDIT_HEIGHT 28

//------------------------------------------------------------------------------
// Processing of messages of filter window
// ---
LRESULT CALLBACK FilterWndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
  switch ( message ) {
  case WM_CLOSE: {
  } break;

  case WM_SIZE: {
    ObjectFilterWindow * wp = TestVariables::viewManager->FindFilter( (void*)hwnd );
    if ( wp )
      wp->GetTool().Layout();
  } break;

  case WM_COMMAND: {
    ObjectFilterWindow * wp = TestVariables::viewManager->FindFilter( (void*)hwnd );
    if ( wp )
      wp->FilterCommand( LOWORD(wParam), HIWORD(wParam), (GetKeyState(VK_RETURN) & 0x8000) != 0 );
  } break;

  case WM_VKEYTOITEM: {
    ObjectFilterWindow * wp = TestVariables::viewManager->FindFilter( (void*)hwnd );
    if ( wp ) {
      if ( wp->KeyboardCommand(LOWORD(wParam), (GetKeyState(VK_SHIFT) & 0x8000) != 0, (GetKeyState(VK_CONTROL) & 0x8000) != 0) )
        return 0;
    }
  } break;

  case WM_DESTROY: {
    ObjectFilterWindow * wp = TestVariables::viewManager->FindFilter( (void*)hwnd );
    if ( wp ) {
      wp->PreDestroyCommand( true );
    }
  }
  }

  return DefMDIChildProc( hwnd, message, wParam, lParam );
}

//------------------------------------------------------------------------------
// \ru Создать диалог фильтра по типу \en Create type filter dialog
// ---
#define CLASS_NAME_TREE_TYPE  _T("TypeModelTree")
bool CreateTypeClient( HWND& hwin, HWND& hlist, int& listSize, int& listStart, const TCHAR * title, int count )
{
  static bool init = false;
  if ( !init )
  {
    init = true;
    WNDCLASS wc;

    // Register TypeModelTree window class
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)FilterWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon( hInst, MAKEINTRESOURCE(IDI_ICON_1) );
    wc.hCursor = LoadCursor( nullptr, IDC_ARROW );
    wc.hbrBackground = (HBRUSH)GetStockObject( LTGRAY_BRUSH );
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = CLASS_NAME_TREE_TYPE;

    RegisterClass( &wc );
  }

  listStart = BUTTON_HEIGHT + 1;
  listSize = (STATIC_HEIGHT + 1) * count;

  MDICREATESTRUCT    mdicreate;
  mdicreate.szClass = CLASS_NAME_TREE_TYPE;
  mdicreate.szTitle = title;
  mdicreate.hOwner = hInst;
  mdicreate.x = CW_USEDEFAULT;
  mdicreate.y = CW_USEDEFAULT;
  mdicreate.cx = 410;
  mdicreate.cy = listStart + listSize + 1;
  mdicreate.style = 0;
  mdicreate.lParam = 0;//nullptr;

  // filter window
  hwin = (HWND)SendMessage( hwndClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mdicreate );
  PRECONDITION( hwin );

  RECT rect;
  GetClientRect( hwin, &rect );

  int dx = rect.right / 2;
  CreateWindow( _T("BUTTON"), IDS_TITLE_SELECT_APPLY, WS_CHILD | WS_VISIBLE,
    0, 0,
    dx, BUTTON_HEIGHT,
    hwin, (HMENU)BUTTON_APPLY, hInst, 0 );

  CreateWindow( _T("BUTTON"), IDS_TITLE_SELECT_TYPE, WS_CHILD | WS_VISIBLE,
    dx, 0,
    dx, BUTTON_HEIGHT,
    hwin, (HMENU)BUTTON_SELECT_TYPE, hInst, 0 );

  // list of node properties
  hlist = ::CreateWindow( _T("LISTBOX"), nullptr,
    WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_USETABSTOPS,
    1, listStart,
    rect.right - 1, listSize,
    hwin, (HMENU)H_LIST, hInst, 0 );

  return true;
}
#undef CLASS_NAME_TREE_TYPE 

//--------------------------------------------------------------
// Window part of type filter dialog
//---
class ObjectTypeTool : public IObjectFilterTool
{
  HWND  m_h;
  HWND  m_hList;
  int   m_startList;
  int   m_listY;

public:
  ObjectTypeTool( HWND  hwin, HWND hList, int startList, int listY ) :
    m_h(hwin), m_hList(hList), m_startList(startList), m_listY(listY) {}

  // \ru Это мое окно? \en Is this my window?
  virtual bool    IsMyWnd( const void * wndId ) const { return (HWND)wndId == m_h; }

  // \ru Обработчик размещения \en Layout handler
  virtual void    Layout();

  // \ru Обработчики списка \en List handlers
  virtual void    SetFocusList() { ::SetFocus(m_hList); }
  virtual void    SetCurSelList( size_t cur ) { ::SendMessage ( m_hList, LB_SETCURSEL, cur, 0L ); }
  virtual size_t  GetCurSelList() { return (size_t)::SendMessage ( m_hList, LB_GETCURSEL, 0, 0L ); }
  virtual void    SetListTexts( RPArray<c3d::string_t>& a, RPArray<c3d::string_t>* a2 = nullptr );
  virtual void    SetListText( size_t cur, const TCHAR* s, const TCHAR* s2 = nullptr );

  // \ru Обработчики строки редактирования \en Handlers of editbox
  virtual void    SetEditText( const TCHAR * ) {}
  virtual void    GetEditText( TCHAR * , int ) {}
  virtual void    SelectEditText() {}
  virtual void    ShowEditItem( bool ) {}
  virtual void    SetFocusEdit() {}

  // \ru Закрыть это окно \en Destroy this window
  virtual void    CloseWindow() { ::PostMessage ( hwndClient, WM_MDIDESTROY, (WPARAM)m_h, 0L ); }
};

// \ru Обработчики списка \en List handlers
void ObjectTypeTool::SetListText( size_t cur, const TCHAR* s, const TCHAR* )
{
  ::SendMessage( m_hList, WM_SETREDRAW, false, 0L );
  ::SendMessage( m_hList, LB_DELETESTRING, cur, 0L );
  ::SendMessage( m_hList, LB_INSERTSTRING, cur, LPARAM(s) );
  ::SendMessage( m_hList, WM_SETREDRAW, true, 0L );
}

void ObjectTypeTool::SetListTexts( RPArray<c3d::string_t> & a, RPArray<c3d::string_t>* )
{
  ::SendMessage ( m_hList, WM_SETREDRAW, false, 0L );
  ::SendMessage ( m_hList, LB_RESETCONTENT, 0, 0L );
  for ( size_t i = 0; i < a.Count(); ++i )
    ::SendMessage ( m_hList, LB_INSERTSTRING, i, (LPARAM)(a[i]->c_str()) );

  ::SendMessage ( m_hList, WM_SETREDRAW, true, 0L );
}

// \ru Обработчик размещения \en Layout handler
void ObjectTypeTool::Layout()
{
  size_t currpos = GetCurSelList();

  RECT rect;
  GetClientRect ( m_h, &rect );
  int  sizeY = rect.bottom - m_startList;
  int listY = std_min(sizeY, m_listY);
  SetWindowPos ( m_hList, 0, 0, 0, rect.right - 1, listY, SWP_NOZORDER | SWP_NOMOVE );

  SetCurSelList ( currpos );
}

// \ru Создать оконную часть диалога фильтра по типу \en Create window part of type filter dialog
IObjectFilterTool * CreateTypeTool ( HWND hwin, HWND hlist, int start, int list_y ) {
  return new ObjectTypeTool ( hwin, hlist, start, list_y );
}

//--------------------------------------------------------------
// \ru Создать диалог фильтра по габариту \en Create gabarit filter window
//---
#define CLASS_NAME_TREE_GABARIT  _T("GabaritModelTree")
bool CreateGabaritClient ( HWND& hwin, HWND& hedit,  HWND& hlist,int& listStart, int& listSize, const TCHAR * title, int count )
{
  static bool init = false;
  if ( !init )
  {
    init = true;
    WNDCLASS wc;

    // Register GabaritModelTree window class
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)FilterWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon ( hInst, MAKEINTRESOURCE(IDI_ICON_1) );
    wc.hCursor = LoadCursor ( nullptr, IDC_ARROW );
    wc.hbrBackground = (HBRUSH)GetStockObject ( LTGRAY_BRUSH );
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = CLASS_NAME_TREE_GABARIT;

    RegisterClass ( &wc );
  }

  listStart = BUTTON_HEIGHT * 2 + 2;
  listSize = (STATIC_HEIGHT + 1) * count;

  MDICREATESTRUCT    mdicreate;
  mdicreate.szClass = CLASS_NAME_TREE_GABARIT;
  mdicreate.szTitle = title;
  mdicreate.hOwner = hInst;
  mdicreate.x = CW_USEDEFAULT;
  mdicreate.y = CW_USEDEFAULT;
  mdicreate.cx = 410;
  mdicreate.cy = listStart + listSize + 1;
  mdicreate.style = 0;
  mdicreate.lParam = 0;//nullptr;

  // Окно дерева
  hwin = (HWND)SendMessage ( hwndClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mdicreate );
  PRECONDITION( hwin );

  RECT rect;
  GetClientRect ( hwin, &rect );

  int dx = rect.right / 2;
  CreateWindow ( _T("BUTTON"), IDS_TITLE_SELECT_APPLY, WS_CHILD | WS_VISIBLE,
    0, 0,
    dx, BUTTON_HEIGHT,
    hwin, (HMENU)BUTTON_APPLY, hInst, 0);

  hedit = ::CreateWindow(_T("EDIT"), nullptr, WS_CHILD | ES_AUTOVSCROLL |
    WS_VISIBLE | WS_BORDER | ES_MULTILINE,
    1, BUTTON_HEIGHT + 1,
    rect.right - 1, BUTTON_HEIGHT,
    hwin, (HMENU)H_EDIT, hInst, 0 );
    
  // list of node properties
  hlist = ::CreateWindow ( _T("LISTBOX"), nullptr,
    WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL |
    LBS_NOTIFY | LBS_USETABSTOPS | LBS_WANTKEYBOARDINPUT,
    1, listStart,
    rect.right - 1, listSize,
    hwin, (HMENU)H_LIST, hInst, 0 );

  return true;
}
#undef CLASS_NAME_TREE_GABARIT

//--------------------------------------------------------------
// Window part of gabarit filter dialog
//---
class ObjectGabaritTool : public IObjectFilterTool
{
  HWND  m_h;
  HWND  m_hEdit;
  HWND  m_hList;
  int   m_startList;
  int   m_listY;

public:
  ObjectGabaritTool ( HWND  hwin, HWND hEdit, HWND hList, int startList, int listY ) :
    m_h(hwin), m_hList(hList), m_hEdit(hEdit), m_startList(startList), m_listY(listY) {}

  // \ru Это мое окно? \en Is this my window?
  virtual bool    IsMyWnd ( const void * wndId ) const { return (HWND)wndId == m_h; }

  // \ru Обработчик размещения \en Layout handler
  virtual void    Layout();

  // \ru Обработчики списка \en List handlers
  virtual void    SetFocusList() { ::SetFocus ( m_hList ); }
  virtual void    SetCurSelList ( size_t cur ) { ::SendMessage ( m_hList, LB_SETCURSEL, cur, 0L ); }
  virtual size_t  GetCurSelList() { return (size_t)::SendMessage ( m_hList, LB_GETCURSEL, 0, 0L ); }
  virtual void    SetListTexts( RPArray<c3d::string_t> & a, RPArray<c3d::string_t>* a1 = nullptr );
  virtual void    SetListText( size_t cur, const TCHAR* s, const TCHAR* s2 = nullptr );

  // \ru Обработчики строки редактирования \en Handlers of editbox
  virtual void    SetEditText ( const TCHAR * s ) { ::SetWindowText ( m_hEdit, s ); }
  virtual void    GetEditText ( TCHAR * s, int len ) { ::GetWindowText ( m_hEdit, s, len ); }
  virtual void    SelectEditText() { ::SendMessage ( m_hEdit, EM_SETSEL, 0, MAKELONG(0, LBN_DBLCLK) ); }
  virtual void    SetFocusEdit() { ::SetFocus( m_hEdit ); }
  virtual void    ShowEditItem ( bool show ) { if (show) ::ShowWindow ( m_hEdit, SW_SHOW ); else ::ShowWindow ( m_hEdit, SW_HIDE ); }

  // \ru Закрыть это окно \en Destroy this window
  virtual void    CloseWindow() { ::PostMessage ( hwndClient, WM_MDIDESTROY, (WPARAM)m_h, 0L ); }
};

// \ru Обработчики списка \en List handlers
void ObjectGabaritTool::SetListText( size_t cur, const TCHAR* s, const TCHAR* )
{
  ::SendMessage ( m_hList, WM_SETREDRAW, false, 0L );
  ::SendMessage ( m_hList, LB_DELETESTRING, cur, 0L );
  ::SendMessage ( m_hList, LB_INSERTSTRING, cur, LPARAM(s) );
  ::SendMessage ( m_hList, WM_SETREDRAW, true, 0L );
}

void ObjectGabaritTool::SetListTexts( RPArray<c3d::string_t> & a, RPArray<c3d::string_t>* )
{
  ::SendMessage( m_hList, WM_SETREDRAW, false, 0L );
  ::SendMessage( m_hList, LB_RESETCONTENT, 0, 0L );
  for ( size_t i = 0; i < a.Count(); ++i )
    ::SendMessage ( m_hList, LB_INSERTSTRING, i, (LPARAM)(a[i]->c_str()) );

  ::SendMessage ( m_hList, WM_SETREDRAW, true, 0L );
}

// \ru Обработчик размещения \en Layout handler
void ObjectGabaritTool::Layout()
{
  size_t currpos = GetCurSelList();

  RECT rect;
  GetClientRect ( m_h, &rect );
  int  sizeY = rect.bottom - m_startList;
  SetWindowPos ( m_hList, 0, 0, 0, rect.right - 1, sizeY, SWP_NOZORDER | SWP_NOMOVE );

  SetCurSelList(currpos);
}

// \ru Создать оконную часть диалога фильтра по габариту \en Create window part of gabarit filter dialog
IObjectFilterTool* CreateGabaritTool( HWND hwin, HWND& hedit, HWND hlist, int start, int listY )
{
  return new ObjectGabaritTool ( hwin, hedit, hlist, start, listY );
}


//--------------------------------------------------------------
// \ru Создать диалог фильтра по атрибутам. \en Create attribute filter window.
//---
#define CLASS_NAME_TREE_ATTRIBUTE  _T("AttributeModelTree")
bool CreateAttrClient ( HWND& hwin, HWND& hedit, HWND& hlist, HWND& hlist2,int& listStart, int& listSize, int& part, const TCHAR * title, int count )
{
  static bool init = false;
  if ( !init )
  {
    init = true;
    WNDCLASS wc;

    // Register AttributeModelTree window class
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)FilterWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon ( hInst, MAKEINTRESOURCE(IDI_ICON_1) );
    wc.hCursor = LoadCursor ( nullptr, IDC_ARROW );
    wc.hbrBackground = (HBRUSH)GetStockObject ( LTGRAY_BRUSH );
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = CLASS_NAME_TREE_ATTRIBUTE;

    RegisterClass ( &wc );
  }

  listStart = BUTTON_HEIGHT * 2 + 2;
  listSize = (STATIC_HEIGHT) * count;
  part = 3;

  MDICREATESTRUCT    mdicreate;
  mdicreate.szClass = CLASS_NAME_TREE_ATTRIBUTE;
  mdicreate.szTitle = title;
  mdicreate.hOwner = hInst;
  mdicreate.x = CW_USEDEFAULT;
  mdicreate.y = CW_USEDEFAULT;
  mdicreate.cx = 500;
  mdicreate.cy = listStart + listSize + 1;
  mdicreate.style = 0;
  mdicreate.lParam = 0;//nullptr;

  // Окно дерева
  hwin = (HWND)SendMessage ( hwndClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mdicreate );
  PRECONDITION( hwin );

  RECT rect;
  GetClientRect ( hwin, &rect );

  int dx = rect.right / 2;
  CreateWindow( _T("BUTTON"), IDS_TITLE_SELECT_APPLY, WS_CHILD | WS_VISIBLE,
    0, 0,
    dx, BUTTON_HEIGHT,
    hwin, (HMENU)BUTTON_APPLY, hInst, 0 );

  CreateWindow( _T("BUTTON"), IDS_TITLE_SELECT_TYPE, WS_CHILD | WS_VISIBLE,
    dx, 0,
    dx, BUTTON_HEIGHT,
    hwin, (HMENU)BUTTON_SELECT_TYPE, hInst, 0 );

  hedit = ::CreateWindow(_T("EDIT"), nullptr, WS_CHILD | ES_AUTOVSCROLL | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
    1, BUTTON_HEIGHT + 1,
    rect.right - 1, BUTTON_HEIGHT,
    hwin, (HMENU)H_EDIT, hInst, 0 );

  // lists of node properties
  LONG listWidth2 = rect.right / part;
  LONG listWidth = rect.right - listWidth2 - 1;
  hlist = ::CreateWindow( _T("LISTBOX"), nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER /*| WS_VSCROLL*/ | LBS_NOTIFY | LBS_USETABSTOPS/* | LBS_WANTKEYBOARDINPUT*/,
    1, listStart,
    listWidth, listSize,
    hwin, (HMENU)H_LIST, hInst, 0 );

  hlist2 = ::CreateWindow( _T("LISTBOX"), nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER /*| WS_VSCROLL*/ | LBS_NOSEL | LBS_USETABSTOPS,
    listWidth + 1, listStart,
    listWidth2, listSize,
    hwin, (HMENU)H_CHILDLIST, hInst, 0 );

  return true;
}
#undef CLASS_NAME_TREE_ATTRIBUTE

//--------------------------------------------------------------
// Window part of attribute filter dialog
//---
class ObjectAttributeTool : public IObjectFilterTool
{
  HWND  m_h;
  HWND  m_hEdit;     // value edit box
  HWND  m_hList;     // attribute types list
  HWND  m_hList2;    // attribute values list
  int   m_startList; // list start
  int   m_listY;     // list end
  int   m_part;      // lists partition

public:
  ObjectAttributeTool ( HWND  hwin, HWND hEdit, HWND hList, HWND hList2, int startList, int listY, int part ) :
    m_h(hwin), m_hList(hList), m_hList2(hList2), m_hEdit(hEdit), m_startList(startList), m_listY(listY), m_part(part) {}

  // \ru Это мое окно? \en Is this my window?
  virtual bool    IsMyWnd ( const void * wndId ) const { return (HWND)wndId == m_h; }

  // \ru Обработчик размещения \en Layout handler
  virtual void    Layout();

  // \ru Обработчики списка \en List handlers
  virtual void    SetFocusList() { ::SetFocus ( m_hList ); }
  virtual void    SetCurSelList ( size_t cur ) { ::SendMessage ( m_hList, LB_SETCURSEL, cur, 0L ); }
  virtual size_t  GetCurSelList() { return (size_t)::SendMessage ( m_hList, LB_GETCURSEL, 0, 0L ); }
  virtual void    SetListTexts( RPArray<c3d::string_t>& a, RPArray<c3d::string_t>* a2 = nullptr );
  virtual void    SetListText( size_t cur, const TCHAR* s, const TCHAR* s2 = nullptr );

  // \ru Обработчики строки редактирования \en Handlers of editbox
  virtual void    SetEditText ( const TCHAR * s ) { ::SetWindowText ( m_hEdit, s ); }
  virtual void    GetEditText ( TCHAR * s, int len ) { ::GetWindowText ( m_hEdit, s, len ); }
  virtual void    SelectEditText() { ::SendMessage ( m_hEdit, EM_SETSEL, 0, MAKELONG(0, LBN_DBLCLK) ); }
  virtual void    SetFocusEdit() { ::SetFocus( m_hEdit ); }
  virtual void    ShowEditItem ( bool show ) { if (show) ::ShowWindow ( m_hEdit, SW_SHOW ); else ::ShowWindow ( m_hEdit, SW_HIDE ); }

  // \ru Закрыть это окно \en Destroy this window
  virtual void    CloseWindow() { ::PostMessage ( hwndClient, WM_MDIDESTROY, (WPARAM)m_h, 0L ); }
};

// \ru Обработчики списка \en List handlers
void ObjectAttributeTool::SetListText( size_t cur, const TCHAR* s, const TCHAR* s2 )
{
  if ( s != nullptr ) {
    ::SendMessage ( m_hList, WM_SETREDRAW, false, 0L );
    ::SendMessage ( m_hList, LB_DELETESTRING, cur, 0L );
    ::SendMessage ( m_hList, LB_INSERTSTRING, cur, LPARAM(s) );
    ::SendMessage ( m_hList, WM_SETREDRAW, true, 0L );
  }
  if ( s2 != nullptr ) {
    ::SendMessage( m_hList2, WM_SETREDRAW, false, 0L );
    ::SendMessage( m_hList2, LB_DELETESTRING, cur, 0L );
    ::SendMessage( m_hList2, LB_INSERTSTRING, cur, LPARAM(s2) );
    ::SendMessage( m_hList2, WM_SETREDRAW, true, 0L );
  }
}

void ObjectAttributeTool::SetListTexts( RPArray<c3d::string_t>& a, RPArray<c3d::string_t>* a2 )
{
  ::SendMessage( m_hList, WM_SETREDRAW, false, 0L );
  ::SendMessage( m_hList, LB_RESETCONTENT, 0, 0L );
  for ( size_t i = 0; i < a.Count(); ++i )
    ::SendMessage ( m_hList, LB_INSERTSTRING, i, (LPARAM)(a[i]->c_str()) );

  ::SendMessage ( m_hList, WM_SETREDRAW, true, 0L );

  if ( a2 != nullptr ) {
    ::SendMessage( m_hList2, WM_SETREDRAW, false, 0L );
    ::SendMessage( m_hList2, LB_RESETCONTENT, 0, 0L );
    for ( size_t i = 0; i < a2->Count(); ++i )
      ::SendMessage( m_hList2, LB_INSERTSTRING, i, (LPARAM)((*a2)[i]->c_str()) );

    ::SendMessage( m_hList2, WM_SETREDRAW, true, 0L );
  }
}

// \ru Обработчик размещения \en Layout handler
void ObjectAttributeTool::Layout()
{
  size_t currpos = GetCurSelList();

  RECT rect;
  GetClientRect ( m_h, &rect );
  int  sizeY = rect.bottom - m_startList;
  LONG listWidth2 = rect.right / m_part;
  LONG listWidth = rect.right - listWidth2 - 1;

  SetWindowPos(m_hList, 0, 0, 0, listWidth, sizeY, SWP_NOZORDER | SWP_NOMOVE);
  SetWindowPos(m_hList2, 0, listWidth - 1, m_startList, listWidth2, sizeY, SWP_NOZORDER);

  SetCurSelList(currpos);
}

// \ru Создать оконную часть диалога фильтра по габариту \en Create window part of gabarit filter dialog
IObjectFilterTool* CreateAttrTool( HWND hwin, HWND& hedit, HWND hlist, HWND hlist2, int start, int listY, int part )
{
  return new ObjectAttributeTool ( hwin, hedit, hlist, hlist2, start, listY, part );
}

#endif // __USE_QT__

