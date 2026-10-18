#include <test.h>
#include <test_prompt.h>
#include <test_manager.h>
#include <test_variables.h>
#include <last.h>

using namespace c3d;


#ifndef __USE_QT__ 


#define BUTTON_HEIGHT 20 // Высота кнопок
#define EDIT_HEIGHT 24


////////////////////////////////////////////////////////////////////////////////
//
// Обработка сообщений окон свойств.
//
//   16 разрядов      32 разряда
//            WM_COMMAND
//   wParam           LOWORD(wParam)
//   LOWORD(lParam)   lParam
//   HIWORD(lParam)   HIWORD(wParam)
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Обработка сообщений окна свойств
// ---
LRESULT CALLBACK PropertyWndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ) 
{
  // На WM_CREATE поле propTool еще не определено (== nullptr)
  //PropertyWindow * wp = viewManager->FindProperty( (void*)hwnd ); // Выдать свойство

  switch ( message ) {
    case WM_CLOSE : {
      //PropertyWindow * wp = viewManager->FindProperty( (void*)hwnd ); // Выдать свойство
      //if ( wp != nullptr && wp->IsBlocked() )
      //  return 0;
    } break;

    case WM_SIZE : {
      PropertyWindow * wp = TestVariables::viewManager->FindProperty( (void*)hwnd ); // Выдать свойство
      if ( wp )
        wp->GetPropTool().Layout();
    } break;

    case WM_COMMAND : {
      PropertyWindow * wp = TestVariables::viewManager->FindProperty( (void*)hwnd ); // Выдать свойство
      if ( wp ) {
        wp->PropertyCommand( LOWORD(wParam), HIWORD(wParam), (GetKeyState( VK_RETURN ) & 0x8000) != 0 );
      } // if
    } break;

    case WM_VKEYTOITEM : {
      PropertyWindow * wp = TestVariables::viewManager->FindProperty( (void*)hwnd ); // Выдать свойство
      if ( wp ) {
        if ( wp->KeyboardCommand( LOWORD(wParam), (GetKeyState(VK_SHIFT) & 0x8000) != 0, (GetKeyState(VK_CONTROL) & 0x8000) != 0 ) )
          return 0;
      }
    } break;

    case WM_DESTROY : {
      PropertyWindow * wp = TestVariables::viewManager->FindProperty( (void*)hwnd ); // Выдать свойство
      if ( wp ) {
        wp->PreDestroyCommand( true );
      }
    }
  }

  return DefMDIChildProc( hwnd, message, wParam, lParam );
}


#define CLASS_NAME_PROPERTY  _T("MbProperty")
//------------------------------------------------------------------------------
// Создать окно свойств
// ---
bool CreatePropertyClient( HWND& hprop, HWND& hlist, HWND& hedit, const TCHAR* title, int count )
{
  static bool init = false;
  if ( !init )
  {
    init = true;
    WNDCLASS wc;

    // Register MbProperty window class
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)PropertyWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInst;
    wc.hIcon         = LoadIcon( hInst, MAKEINTRESOURCE( IDI_ICON_1 ) );
    wc.hCursor       = LoadCursor (nullptr, IDC_ARROW) ;
    wc.hbrBackground = (HBRUSH)GetStockObject (LTGRAY_BRUSH) ;
    wc.lpszMenuName  = nullptr ;
    wc.lpszClassName = CLASS_NAME_PROPERTY;

    RegisterClass( &wc );
  }

  MDICREATESTRUCT    mdicreate ;
    mdicreate.szClass = CLASS_NAME_PROPERTY;
    mdicreate.szTitle = title;
    mdicreate.hOwner  = hInst;
    mdicreate.x       = CW_USEDEFAULT;
    mdicreate.y       = CW_USEDEFAULT;
    mdicreate.cx      = 359; 
    mdicreate.cy      = BUTTON_HEIGHT*2+1+EDIT_HEIGHT+1+EDIT_HEIGHT*(count)+1; 
    mdicreate.style   = 0;//WS_CHILD; // Не помогло
    mdicreate.lParam  = 0;//nullptr;

  // Окно свойства
  hprop = (HWND)SendMessage( hwndClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT) &mdicreate );
  PRECONDITION( hprop );

  RECT propRect;
  GetClientRect( hprop, &propRect );
  int dx = propRect.right / 3;

  CreateWindow( _T("BUTTON"), IDS_TITLE_WRITE, WS_CHILD | WS_VISIBLE,
                0, 0,
                dx, BUTTON_HEIGHT,
                hprop, (HMENU)BUTTON_SET, hInst, 0 );

  CreateWindow( _T("BUTTON"), IDS_TITLE_APPLY, WS_CHILD | WS_VISIBLE,
                dx, 0,
                dx, BUTTON_HEIGHT,
                hprop, (HMENU)BUTTON_SET_EXIT, hInst, 0 );

  CreateWindow( _T("BUTTON"), IDS_TITLE_CANCEL, WS_CHILD | WS_VISIBLE,
                dx * 2, 0,
                dx, BUTTON_HEIGHT,
                hprop, (HMENU)BUTTON_EXIT, hInst, 0 );

  hedit = ::CreateWindow( _T("EDIT"), nullptr, WS_CHILD | ES_AUTOVSCROLL |
                              WS_VISIBLE | WS_BORDER | ES_MULTILINE,
                              1, BUTTON_HEIGHT + 1,
                              propRect.right - 1, EDIT_HEIGHT,
                              hprop, (HMENU)H_EDIT, hInst, 0 ), 
  hlist = ::CreateWindow( _T("LISTBOX"), nullptr,
                              WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL |
                              LBS_NOTIFY | LBS_USETABSTOPS | LBS_WANTKEYBOARDINPUT,
                              1, BUTTON_HEIGHT + 1 + EDIT_HEIGHT + 1, 1, EDIT_HEIGHT*(count),//1,
                              hprop, (HMENU)H_LIST, hInst, 0 );

  return true;
}
#undef CLASS_NAME_PROPERTY 


//------------------------------------------------------------------------------
// Связка с оконной частью редактор свойств
// ---
class HWndPropTool : public IPropTool
{
private:
  HWND             hprop;
  HWND             hlist;      
  HWND             hedit;

public:
  HWndPropTool( HWND _hprop, HWND _hlist, HWND _hedit )
    : hprop ( _hprop )
    , hlist ( _hlist )
    , hedit ( _hedit )
  {}

  virtual ~HWndPropTool() {
  }

public:
  virtual void     ClosePropertyWindow();
  virtual bool     IsMyWnd( const void * wndId ) const { return (HWND)wndId == hprop; }
  virtual void     Layout(); // Переразместить окна свойств

  virtual void     SetFocusList()              { ::SetFocus( hlist );  }
  virtual void     SetCurSelList( size_t cur ) { ::SendMessage( hlist, LB_SETCURSEL, cur, 0L ); }
  virtual size_t   GetCurSelList()             { return (size_t)::SendMessage( hlist, LB_GETCURSEL, 0, 0L ); }
  virtual void     SetListText( size_t cur, const TCHAR * s );  
  virtual void     SetListTexts( RPArray<string_t> & a );  
  virtual void     SetFocusEdit()                    { ::SetFocus( hedit ); }
  virtual void     SetEditText( const TCHAR * s )    { ::SetWindowText( hedit, s ); }   
  virtual void     GetEditText( TCHAR * s, int len ) { ::GetWindowText( hedit, s, len ); }  
  virtual void     SelectEditText()                  { ::SendMessage( hedit, EM_SETSEL, 0, MAKELONG (0, LBN_DBLCLK) ); }
};


//------------------------------------------------------------------------------
//
// ---
void HWndPropTool::SetListText( size_t cur, const TCHAR* s )  
{
  ::SendMessage( hlist, WM_SETREDRAW, false, 0L );
  ::SendMessage( hlist, LB_DELETESTRING, cur, 0L );
  ::SendMessage( hlist, LB_INSERTSTRING, cur, LPARAM( s ) );
  ::SendMessage( hlist, WM_SETREDRAW, true, 0L );
}


//------------------------------------------------------------------------------
//
// ---
void HWndPropTool::SetListTexts( RPArray<string_t> & a )
{
  ::SendMessage( hlist, WM_SETREDRAW, false, 0L );
  ::SendMessage( hlist, LB_RESETCONTENT, 0, 0L );
  for ( size_t i = 0; i < a.Count(); ++i ) 
    ::SendMessage( hlist, LB_INSERTSTRING, i, (LPARAM)(a[i]->c_str()) );

  ::SendMessage( hlist, WM_SETREDRAW, true, 0L );
}


//------------------------------------------------------------------------------
// Закрыть окно свойств
// ---
void HWndPropTool::ClosePropertyWindow() {
  ::PostMessage( hwndClient, WM_MDIDESTROY, (WPARAM)hprop, 0L );
}


//------------------------------------------------------------------------------
// Переразместить окна свойств
// ---
void HWndPropTool::Layout()
{
  size_t currpos = GetCurSelList();

  RECT rect;
  GetClientRect( hprop, &rect );
  int  sizeY   = rect.bottom - BUTTON_HEIGHT - 1 - EDIT_HEIGHT - 1 - 1;
  SetWindowPos( hlist, 0, 0, 0, rect.right - 1, sizeY, SWP_NOZORDER | SWP_NOMOVE );

  SetCurSelList( currpos );
}


//------------------------------------------------------------------------------
// Создание обработчика свойств.
// --- 
IPropTool * CreatePropertyTool( HWND hprop, HWND hlist, HWND hedit ) {
  return new HWndPropTool( hprop, hlist, hedit );
}


#endif // __USE_QT__ 
