#include <test.h>
#include <test_manager.h>
#include <test_variables.h>
#include <test_service.h>
#include <commctrl.h>
#include <stdio.h>
#include <tool_time_test.h>
#include <op_curve_parameter.h>
#include <last.h>


using namespace c3d;


#ifndef __USE_QT__ 

MbMatrix3D tmpMatrix;

//------------------------------------------------------------------------------
// 
// ---
HWND ParentForDialog() {
  // WorkWindow *currentView = viewManager->GetActiveWindow();
  // return currentView ? currentView->GetHWND() : nullptr;
  return hwndFrame;
}


//------------------------------------------------------------------------------
// Перевод строки в вещественное число
// ---
static bool StringToDouble( const TCHAR * s, double & d )
{
  if ( ::lstrlen( s ) == 0 )
    return false;

  TCHAR * ch = nullptr;
  double d_tmp = _tcstod( s, &ch );

  if ( ch == nullptr || *ch != 0 ) {
    return false;
  }
  if ( d_tmp!= HUGE_VAL && d_tmp!=-HUGE_VAL ) {
    d = d_tmp;
    return true;
  }
  else
    return false;
}


//------------------------------------------------------------------------------
//
// ---
struct DlgStringParam {
  const TCHAR * prompt; // Номер подсказки
  const TCHAR * defstr; // Значение по умолчанию
};


//------------------------------------------------------------------------------
// Обрезка лишнего строки
// ---
static void CutString( TCHAR * s ) {
  ptrdiff_t i = _tcslen( s ) - 1;
  while ( s[i] == _T(' ') && i >= 0 ) i--;
  s[i + 1] = 0;
}


//------------------------------------------------------------------------------
// Обработка сообщений ввода строки
// ---
BOOL CALLBACK DlgStringProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam ) {
  switch (message) {

    case WM_INITDIALOG :
    {
      //LoadString( hInst, ((DlgStringParam *)lParam)->prompt, (TCHAR *)strBuffer, STRINGLENGTH );
      _tcscpy( (TCHAR*)TestVariables::strBuffer, ((DlgStringParam *)lParam)->prompt );

      CutString( TestVariables::strBuffer );
      SetDlgItemText(hDlg, IDC_TEXT, TestVariables::strBuffer );
      SetDlgItemText(hDlg, IDC_EDIT1, ((DlgStringParam*)lParam)->defstr );
      return TRUE;
    }

    case WM_COMMAND :
      switch (LOWORD(wParam)) {
        case IDOK :
        {
          GetDlgItemText(hDlg, IDC_EDIT1, TestVariables::strBuffer, 256);
          EndDialog(hDlg, true);
          return TRUE;
        }

        case IDCANCEL :
        {
          EndDialog(hDlg, true);
          return TRUE;
        }
      }
      break;
  }

  return FALSE;
} 


//------------------------------------------------------------------------------
// Обработка сообщений формы ввода лицензионного ключа
// ---
BOOL CALLBACK DlgLicProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM /*lParam*/ ) {
  switch ( message ) {
    case WM_INITDIALOG :
    {
      return TRUE;
    }
    case WM_COMMAND :
      switch ( LOWORD( wParam ) ) {
        case IDOK :
        {
          GetDlgItemText( hDlg, IDC_EDIT_KEY, TestVariables::strBuffer, STRINGLENGTH );
          GetDlgItemText( hDlg, IDC_EDIT_SIG, TestVariables::strBuffer2, STRINGLENGTH );
          TestVariables::saveLic = ( IsDlgButtonChecked( hDlg, IDC_CHECK_FILE ) == BST_CHECKED );
          EndDialog( hDlg, true );
          return TRUE;
        }
        case IDCANCEL :
        {
          EndDialog( hDlg, false );
          return TRUE;
        }
      }
      break;
  }
  return FALSE;
} 


//------------------------------------------------------------------------------
// Ввод строки
// ---
bool GetString( const TCHAR* nPrompt, const TCHAR * defstr, TCHAR * s, size_t len )
{
  DlgStringParam dp = { nPrompt, defstr };

  DialogBoxParam( hInst, (LPCTSTR)IDD_DIALOG_BASE, ParentForDialog(), (DLGPROC)DlgStringProc, (LPARAM)&dp );

  if ( lstrlen( TestVariables::strBuffer ) == 0 ) {
    MessageBeep( MB_ICONEXCLAMATION );
    return false;
  }

  _tcsncpy( s, TestVariables::strBuffer, len );

  return true;
} 

//------------------------------------------------------------------------------
// Ввод целого числа
// ---
bool GetInt( const TCHAR* nPrompt, int & iresult ) {
  TCHAR s[200];
  _stprintf( s, F_I32D, iresult );

  if ( GetString( nPrompt, s, TestVariables::strBuffer, STRINGLENGTH ) ) {
    int i_tmp = _ttoi( TestVariables::strBuffer );

    for ( ptrdiff_t i = 0; i < (ptrdiff_t)lstrlen( TestVariables::strBuffer ); i++ ) {
      if ( !(( TestVariables::strBuffer[i] >= _T('0') ) && ( TestVariables::strBuffer[i] <= _T('9') ) ||
             ( TestVariables::strBuffer[i] == _T('-') ) ||
             ( TestVariables::strBuffer[i] == _T('+') )) ) {
        MessageBeep( MB_ICONEXCLAMATION );
        return false;
      }
    }
    iresult = i_tmp;
    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Ввод целого числа
// ---
#if defined(PLATFORM_64)
bool GetInt( const TCHAR* nPrompt, ptrdiff_t & iresult )
{
  TCHAR s[20];
  _stprintf( s, F_I64D, iresult );  //-V111

  if ( GetString( nPrompt, s, TestVariables::strBuffer, STRINGLENGTH ) ) {
    ptrdiff_t i_tmp = _ttoi64( TestVariables::strBuffer );

    for ( ptrdiff_t i = 0; i < (ptrdiff_t)lstrlen( TestVariables::strBuffer ); i++ ) {
      if ( !(( TestVariables::strBuffer[i] >= _T('0') ) && ( TestVariables::strBuffer[i] <= _T('9') ) ||
             ( TestVariables::strBuffer[i] == _T('-') ) ||
             ( TestVariables::strBuffer[i] == _T('+') )) ) {
        MessageBeep( MB_ICONEXCLAMATION );
        return false;
      }
    }
    
    iresult = (ptrdiff_t)i_tmp;
    return true;
  }

  return false;
}
#endif // PLATFORM_64


//------------------------------------------------------------------------------
// Ввод вещественного числа
// ---
bool GetFloat( const TCHAR* nPrompt, float & fresult )
{
  TCHAR s[30];
  double dresult = fresult;
  if ( dresult==0 )
    _stprintf( s, _T("%.3f"), dresult );
  else
    _stprintf( s, _T("%.9f"), dresult );

  if ( GetString( nPrompt, s, TestVariables::strBuffer, STRINGLENGTH ) )
    if ( StringToDouble( TestVariables::strBuffer, dresult ) ) {
      fresult = (float)dresult;
      return true;
    }
  return false;
} 


//------------------------------------------------------------------------------
// 
// ---
inline
void PrintDouble( double t, TCHAR * s )
{
  bool shortFormat = false;
  if ( t == 0.0 )
    shortFormat = true;
  else if ( t <= -1.0 || t >= 1.0 )
    shortFormat = (t / ::floor(t) == 1.0) && (t / ::ceil(t) == 1.0);

  if ( shortFormat )
    _stprintf( s, _T("%.3f"), t );
  else
    _stprintf( s, _T("%.9f"), t );
}


//------------------------------------------------------------------------------
// Ввод вещественного числа
// ---
bool GetDouble( const TCHAR * nPrompt, double & dresult, const MbRect1D * range )
{
  TCHAR s[62];
  ::PrintDouble( dresult, s );

  string_t rangedPrompt( nPrompt );

  if ( range != nullptr ) {
    TCHAR s1[64], s2[64];
    ::PrintDouble( range->GetMin(), s1 );
    ::PrintDouble( range->GetMax(), s2 );

    rangedPrompt.append( _T(" [ "));
    rangedPrompt.append( s1 );
    rangedPrompt.append( _T(" , "));
    rangedPrompt.append( s2 );
    rangedPrompt.append( _T(" ] "));
  }

  if ( GetString( rangedPrompt.c_str(), s, TestVariables::strBuffer, STRINGLENGTH ) )
    return StringToDouble( TestVariables::strBuffer, dresult );

  return false;
} 

//------------------------------------------------------------------------------
// Окно выбора из двух вариантов (да/нет)
// ---
bool GetYesNoChoice( const TCHAR * nPrompt, const TCHAR * nTitle, bool defaultFirstButton )
{
  UINT type = MB_YESNO;
  if ( !defaultFirstButton )
    type |= MB_DEFBUTTON2;

  return MessageBox( ParentForDialog(), nPrompt, nTitle, type ) == IDYES ? true : false;
}


//------------------------------------------------------------------------------
// Ввод данных для создания символьной кривой
// ---
bool GetDataCharCurve3D( string_t& fx, 
                         string_t& fy, 
                         string_t& fz, 
                         string_t& arg )
{
  TCHAR arTchar[STRINGLENGTH];

  if ( GetString( IDS_ENTER_PARAMETER, _T("t"), arTchar, STRINGLENGTH ) ) 
  {
    TCHAR fxTchar[STRINGLENGTH];
    TCHAR fyTchar[STRINGLENGTH];
    TCHAR fzTchar[STRINGLENGTH];

    _stprintf( fxTchar, _T("20*cos(%s)"), arTchar );
    _stprintf( fyTchar, _T("30*sin(%s)"), arTchar );
    _stprintf( fzTchar, _T("%s*2"),       arTchar );
    if ( GetString( IDS_ENTER_FORMULA, fxTchar, fxTchar, STRINGLENGTH ) && 
         GetString( IDS_ENTER_FORMULA, fyTchar, fyTchar, STRINGLENGTH ) && 
         GetString( IDS_ENTER_FORMULA, fzTchar, fzTchar, STRINGLENGTH ) 
       ) {
      fx  = fxTchar; 
      fy  = fyTchar;
      fz  = fzTchar;
      arg = arTchar;
      return true;
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Обработка сообщений ввода координат точки на плоскости
// ---
BOOL CALLBACK DlgGetExactPlanePointProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM) {
  switch (message) {

    case WM_INITDIALOG :
    {
      _stprintf( TestVariables::strBuffer, _T("%.3f"), ((MbCartPoint *)TestVariables::exactPoint)->x );
      SetDlgItemText(hDlg, IDC_EDIT7, TestVariables::strBuffer );
      _stprintf( TestVariables::strBuffer, _T("%.3f"), ((MbCartPoint *)TestVariables::exactPoint)->y );
      SetDlgItemText(hDlg, IDC_EDIT8, TestVariables::strBuffer );
      return TRUE;
    }

    case WM_COMMAND :
      switch (LOWORD(wParam)) {

        case IDOK :
        {
          bool xOk, yOk;
          GetDlgItemText(hDlg, IDC_EDIT7, TestVariables::strBuffer, 12 );
          xOk = StringToDouble( TestVariables::strBuffer, ((MbCartPoint *)TestVariables::exactPoint)->x );
          GetDlgItemText(hDlg, IDC_EDIT8, TestVariables::strBuffer, 12 );
          yOk = StringToDouble( TestVariables::strBuffer, ((MbCartPoint *)TestVariables::exactPoint)->y );
          EndDialog(hDlg, xOk && yOk);
          return TRUE;
        }

        case IDCANCEL :
        {
          EndDialog(hDlg, false);
          return TRUE;
        }
      }
      break;
  }

  return FALSE;
} // DlgGetExactPointPlaneProc


//------------------------------------------------------------------------------
// Ввод точных координат точки на плоскости
// ---
bool GetExactPlanePoint( MbCartPoint & cPoint )
{
  TestVariables::exactPoint = &cPoint;
  bool ok = DialogBox( hInst, MAKEINTRESOURCE(IDD_DIALOG_ENTER_PLANEPOINT), ParentForDialog(), (DLGPROC)DlgGetExactPlanePointProc ) != 0;

  if ( !ok )
    MessageBeep( MB_ICONEXCLAMATION );

  return ok;
} // GetExactPlanePoint


//------------------------------------------------------------------------------
// Обработка сообщений ввода координат точки
// ---
BOOL CALLBACK DlgGetExactPointProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM)
{
  switch (message) {
    case WM_INITDIALOG :
    {
      _stprintf( TestVariables::strBuffer, _T("%.3f"), ((MbCartPoint3D *)TestVariables::exactPoint)->x );
      SetDlgItemText(hDlg, IDC_EDIT2, TestVariables::strBuffer );
      _stprintf( TestVariables::strBuffer, _T("%.3f"), ((MbCartPoint3D *)TestVariables::exactPoint)->y );
      SetDlgItemText(hDlg, IDC_EDIT3, TestVariables::strBuffer );
      _stprintf( TestVariables::strBuffer, _T("%.3f"), ((MbCartPoint3D *)TestVariables::exactPoint)->z );
      SetDlgItemText(hDlg, IDC_EDIT4, TestVariables::strBuffer );

      return TRUE;
    }

    case WM_COMMAND :
      switch (LOWORD(wParam)) {

        case IDOK :
        {
          bool xOk, yOk, zOk;
          GetDlgItemText(hDlg, IDC_EDIT2, TestVariables::strBuffer, 12 );
          xOk = StringToDouble( TestVariables::strBuffer, ((MbCartPoint3D *)TestVariables::exactPoint)->x );
          GetDlgItemText(hDlg, IDC_EDIT3, TestVariables::strBuffer, 12 );
          yOk = StringToDouble( TestVariables::strBuffer, ((MbCartPoint3D *)TestVariables::exactPoint)->y );
          GetDlgItemText(hDlg, IDC_EDIT4, TestVariables::strBuffer, 12 );
          zOk = StringToDouble( TestVariables::strBuffer, ((MbCartPoint3D *)TestVariables::exactPoint)->z );
          EndDialog(hDlg, xOk && yOk && zOk);
          return TRUE;
        }

        case IDCANCEL :
        {
          EndDialog(hDlg, false);
          return TRUE;
        }
      }
      break;
  }

  return FALSE;
} // DlgGetExactPointProc


//------------------------------------------------------------------------------
// Ввод точных координат точки
// ---
bool GetExactPoint( MbCartPoint3D & cPoint )
{
  TestVariables::exactPoint = &cPoint;
  bool ok = DialogBox( hInst, MAKEINTRESOURCE(IDD_DIALOG_ENTER_SPACEPOINT), ParentForDialog(), (DLGPROC)DlgGetExactPointProc ) != 0;

  if ( !ok )
    MessageBeep( MB_ICONEXCLAMATION );

  return ok;
} // GetExactPoint


//-----------------------------------------------------------------------------
// создать текстовое окно для заполнения
// ---
static void CreateTextEditField( int innerID, int columns, double value, HWND hDlg, HWND& wnd )
{
  const int peX = 5, peY = 40, peW = 60, peH = 20, /*peID = 2001,*/ knID = 2100;
  const int x = peX + (peX + peW)     * (innerID % columns)
          , y = peY + (peH / 2 + peH) * (innerID / columns) ;
  intptr_t hmenu = knID + innerID;
  wnd = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""), ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
                        x, y, peW, peH, hDlg, (HMENU)hmenu, GetModuleHandle(0), 0);
  if ( wnd == 0 )
    MessageBox( hDlg, _T("Could not create edit box."), _T("Error"), MB_OK | MB_ICONERROR );
  else
  {
    _stprintf( TestVariables::strBuffer, _T("%.3f"), value );
    SetDlgItemText( hDlg, knID + innerID, TestVariables::strBuffer );
  }
}


//-----------------------------------------------------------------------------
// получаем double из текстового поля
// ---
static double GetDouble( HWND textField )
{
  double result;
  GetWindowText( textField, TestVariables::strBuffer, 100 );
  StringToDouble( TestVariables::strBuffer, result);
  return result;
}


//-----------------------------------------------------------------------------
// получаем int из текстового поля
// ---
static int GetInt( HWND textField )
{
  return static_cast<int> ( GetDouble( textField ) );
}


//-----------------------------------------------------------------------------
// получаем bool из текстового поля
// ---
static bool GetBool( HWND textField )
{
  return GetInt( textField ) != 0;
}


//------------------------------------------------------------------------------
// прочитать один ряд параметров для фаски
// ---
static void ReadOneChamferRow( MbCornerChamferParams& par, int row, const std::vector<HWND> & textFields )
{
  par.index     = static_cast< size_t >( GetInt(textFields[row]) ); ++row;
  par.distance  = GetDouble( textFields[row] ); ++row;
  par.distAngle = GetDouble( textFields[row] ); ++row;
  par.type      = GetBool  ( textFields[row] ); ++row;
  if ( par.type )
    par.distAngle *= M_PI / 180.;
}


//------------------------------------------------------------------------------
// Обработка ввода параметров фаски
// ---
static BOOL CALLBACK DlgGetChamferParams( HWND hDlg, UINT message, WPARAM wParam, LPARAM )
{
  std::vector<MbCornerChamferParams> * stChamferParams =
    static_cast< std::vector<MbCornerChamferParams> * >( TestVariables::chamferParams );
  if ( stChamferParams == nullptr ) // такого быть не может, но на всякий случай проверим
    return FALSE;
  const int columns = 4;
  static std::vector<HWND> textFields; // вместо глобальной переменной объявим статичную, чтобы не мучаться с указателями и кастами
  switch ( message )
  {
    case WM_INITDIALOG :
    {
      size_t size = stChamferParams->size();
      if ( size == 0 )
      {
        stChamferParams->push_back( MbCornerChamferParams( 0, 1, 1, true ) );
        size = 1;
      }
      textFields.resize( size * size_t(columns), 0 );

      for ( int i = 0; i < size; ++i )
      {
        int j = 0;
        MbCornerChamferParams par( stChamferParams->at(i) );
        CreateTextEditField( i+j, columns, static_cast<double>( par.index ),     hDlg, textFields[j] ); ++j;
        CreateTextEditField( i+j, columns, par.distance,  hDlg, textFields[j] ); ++j;
        CreateTextEditField( i+j, columns, par.distAngle, hDlg, textFields[j] ); ++j;
        CreateTextEditField( i+j, columns, par.type,      hDlg, textFields[j] ); ++j;
      }
      SendMessage( GetDlgItem( hDlg, IDC_CHAMFERALLEQUAL ), BM_SETCHECK, BST_CHECKED, 0 );
      _stprintf( TestVariables::strBuffer, _T("%i"), TestVariables::numberOfChamfers );
      SetDlgItemText(hDlg, IDC_EDIT1, TestVariables::strBuffer );
      return TRUE;
    }

    case WM_COMMAND :
      switch ( LOWORD(wParam) ) 
      {
        case IDOK :
        {
          if ( IsDlgButtonChecked(hDlg, IDC_CHAMFERALLEQUAL) != BST_CHECKED )
          { // считываем всё, что есть
            size_t size = textFields.size();
            stChamferParams->resize( size/columns );
            for ( int i = 0, jParam = 0; i < size; i += columns, ++jParam )
            {
              MbCornerChamferParams par;
              ReadOneChamferRow( par, i, textFields );
              stChamferParams->at( jParam ) = par;
            }
          }
          else // считываем только первый или дефолтный
          {
            MbCornerChamferParams par( 0, 1, 1, true );
            if ( textFields.size() >= columns )
              ReadOneChamferRow( par, 0, textFields ); // считаем 0 ряд, если он есть
            //читаем, сколько нам нужно узлов
            TestVariables::numberOfChamfers = GetInt( GetDlgItem(hDlg, IDC_EDIT1) );
            stChamferParams->assign( TestVariables::numberOfChamfers, par );
            for ( size_t i = 0; i < TestVariables::numberOfChamfers; ++i )
              stChamferParams->at(i).index = i;
          }
          textFields.clear();    // освободим место вручную, т.к. массив статичный
          EndDialog( hDlg, TRUE );
          return TRUE;
        }

        case IDYES : // добавляем ряд окошек для чтения
        {
          int i = static_cast< int >( textFields.size() );
          // добавляем, если: рядов 0, или рядов < число фасок и не стоит галка "все одинаковые"
          if ( (i < columns || IsDlgButtonChecked(hDlg, IDC_CHAMFERALLEQUAL) != BST_CHECKED)
             && i / columns < TestVariables::numberOfChamfers )
          {
            MbCornerChamferParams par( 0, 1, 1, true );
            if ( i >= columns )
            {
              ReadOneChamferRow( par, i-columns, textFields );
              ++( par.index );
              if ( par.type )
                par.distAngle *= 180. / M_PI;
            }
            textFields.resize( size_t(i) + columns, 0 );
            CreateTextEditField( i, columns, static_cast<double>( par.index ),     hDlg, textFields[i] ); ++i;
            CreateTextEditField( i, columns, par.distance,  hDlg, textFields[i] ); ++i;
            CreateTextEditField( i, columns, par.distAngle, hDlg, textFields[i] ); ++i;
            CreateTextEditField( i, columns, par.type,      hDlg, textFields[i] ); ++i;
          }
          return TRUE;
        }

        case IDNO : // убираем ряд окошек для чтения
        {
          size_t size = textFields.size();
          if ( size >= columns )
          {
            for ( size_t i = 1; i <= columns; ++i )
              DestroyWindow( textFields[size-i] );
            textFields.resize( size - columns );
          }
          return TRUE;
        }

        case IDCANCEL :
        {
          textFields.clear();    // освободим место вручную, т.к. массив статичный
          EndDialog( hDlg, FALSE );
          return TRUE;
        }
      }
      break;
  }
  return FALSE;
} // DlgGetChamferParams


//------------------------------------------------------------------------------
// Ввод набора параметров для фаски
// ---
bool GetChamferParameters( void * chamferParams, size_t nVertexes )
{
  TestVariables::numberOfChamfers = static_cast<int>( nVertexes );
  TestVariables::chamferParams = chamferParams;
  bool ok = DialogBox( hInst, MAKEINTRESOURCE(IDD_DIALOG_CHAMFER_PARAMETERS), ParentForDialog(), (DLGPROC)DlgGetChamferParams ) != 0;

  if ( !ok )
    MessageBeep( MB_ICONEXCLAMATION );

  return ok;
} // GetChamferParameters


//------------------------------------------------------------------------------
// Отрисовать оси координат
// ---
static void ShowAxis( HDC dc )
{
  MbMatrix3D matr( tmpMatrix ); 

  int thick = 8;
  double l = 20 * TestVariables::drawFactor;
  MbCartPoint3D p0( 0, 0, 0 ),
                px( l, 0, 0 ),
                py( 0, l, 0 ),
                pz( 0, 0, l ); // Точки в мировой СК

  // Перевод в экранную СК
//  matr.Invert(1);
  p0.Transform( matr );
  px.Transform( matr );
  py.Transform( matr );
  pz.Transform( matr );
  //matr.Invert(1);

  POINT pd0, pdx, pdy, pdz;

  pd0.x = (int32)p0.x;
  pd0.y = (int32)p0.y;

  pdx.x = (int32)px.x;
  pdx.y = (int32)px.y;

  pdy.x = (int32)py.x;
  pdy.y = (int32)py.y;

  pdz.x = (int32)pz.x;
  pdz.y = (int32)pz.y;

  HPEN hpen;

  int oldMode = SetROP2( dc, R2_XORPEN );

  COLORREF col = GetSysColor( COLOR_WINDOW );
  hpen = HPEN(SelectObject( dc, CreatePen(PS_SOLID, thick, TestVariables::RED ^ col ) ) );
  MoveToEx( dc, pd0.x, -pd0.y, nullptr );
  LineTo( dc, pdx.x, -pdx.y );
  DeleteObject(SelectObject( dc, hpen ) );

  hpen = HPEN(SelectObject( dc, CreatePen(PS_SOLID, thick, TestVariables::GREEN ^ col ) ) );
  MoveToEx( dc, pd0.x, -pd0.y, nullptr );
  LineTo( dc, pdy.x, -pdy.y );
  DeleteObject(SelectObject( dc, hpen ) );

  hpen = HPEN(SelectObject( dc, CreatePen(PS_SOLID, thick, TestVariables::BLUE ^ col ) ) );
  MoveToEx( dc, pd0.x, -pd0.y, nullptr );
  LineTo( dc, pdz.x, -pdz.y );
  DeleteObject(SelectObject( dc, hpen ) );

  SetROP2( dc, oldMode );
} 


//------------------------------------------------------------------------------
// Обработка сообщений
// ---
LRESULT CALLBACK ShowAxisProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
  switch ( message ) {
    case WM_CREATE :
    {
      TestVariables::rememPosit.x = TestVariables::rememPosit.y = 0;
      TestVariables::isDown = false;
      HDC dc = GetDC( hwnd );
      SetMapMode( dc, MM_TEXT );
      
      RECT  r;
      GetClientRect( hwnd, &r );
      SetViewportOrgEx( dc, r.right / 2, r.bottom / 2, nullptr );

      ReleaseDC( hwnd, dc );
    } break;

    case WM_PAINT :
    {
      PAINTSTRUCT ps;
      HDC dc = ::BeginPaint( hwnd, &ps );

      ShowAxis( dc );

      ::EndPaint( hwnd, &ps );
      return TRUE;
    }

    case WM_LBUTTONDOWN :
    {
      TestVariables::isDown = true;
      GetCursorPos( &TestVariables::rememPosit );
      return TRUE;
    }

    case WM_LBUTTONUP :
    {
      TestVariables::isDown = false;
      return TRUE;
    }

    case WM_SETCURSOR :
    {
      if (LOWORD(lParam) == HTCLIENT && TestVariables::isDown ) {
        POINT pos;
        GetCursorPos( &pos );

        if ( pos.x != TestVariables::rememPosit.x ) {
          double rotX = ( pos.x - TestVariables::rememPosit.x );
          TestVariables::rememPosit.x = pos.x;
          tmpMatrix.Rotate( 1, rotX * Math::DEGRAD );
        }

        if ( pos.y != TestVariables::rememPosit.y ) {
          double rotY = ( pos.y - TestVariables::rememPosit.y );
          TestVariables::rememPosit.y = pos.y;
          tmpMatrix.Rotate( 0, rotY * Math::DEGRAD  );
        }

        InvalidateRect( hwnd, nullptr, TRUE );
        return TRUE;
      }
    }

  }

  return DefWindowProc( hwnd, message, wParam, lParam );
}


#define CLASS_NAME_AXISWND  _T("SHOW_AXIS_CTRL")
//------------------------------------------------------------------------------
// 
// ---
static HWND CreateAxisWindow( HWND hDlg, uint dlgCtrlId )  
{
  static bool init = false;
  if ( !init )
  {
    init = true;
    WNDCLASS wc;

    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc   = (WNDPROC)ShowAxisProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInst;
    wc.hIcon         = nullptr;
    wc.hCursor       = nullptr;
    wc.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
    wc.lpszMenuName  = nullptr;
    wc.lpszClassName = CLASS_NAME_AXISWND;

    if ( !RegisterClass( &wc ) ) 
      return nullptr;
  }

  RECT rect;
  if ( dlgCtrlId && GetWindowRect( GetDlgItem(hDlg, dlgCtrlId), &rect) ) {
    ScreenToClient( hDlg, (POINT*)&rect.left );
    ScreenToClient( hDlg, (POINT*)&rect.right );
    SetWindowPos( GetDlgItem(hDlg, dlgCtrlId), 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_HIDEWINDOW );
  }
  else {
    rect.left = 5;
    rect.top = 5;
    rect.right = 125;
    rect.bottom = 125;
  }

  HWND wnd = CreateWindow( CLASS_NAME_AXISWND, nullptr,
                           WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE | WS_BORDER,
                           rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, hDlg, 0, hInst, 0 );
  if ( !wnd )
  {
    PRECONDITION( false );
    return nullptr;
  }
  return wnd;
}

#undef CLASS_NAME_AXISWND

//------------------------------------------------------------------------------
// Обработка сообщений установки проекции вида
// ---
BOOL CALLBACK DlgSetProjectionProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM )
{
  static HWND showHwnd = 0;
  switch (message) {

    case WM_INITDIALOG :
    {
      showHwnd = CreateAxisWindow( hDlg, IDC_STATIC );
      return TRUE;
    }

    case WM_COMMAND :
      switch (LOWORD(wParam)) {

        case IDOK :
        {
          EndDialog(hDlg, true);
          TestVariables::setProj = true;
          return TRUE;
        }

        case IDC_IZO :
        case IDC_X0Y :
        case IDC_Z0X :
        case IDC_Y0Z :
        {
          tmpMatrix.Init();

          switch (wParam) {
            case IDC_IZO :
              //tmpMatrix.Rotate( 0, -90 * Math::DEGRAD );
              tmpMatrix.Rotate( 1, - 45 * Math::DEGRAD );
              tmpMatrix.Rotate( 0, 37 * Math::DEGRAD );
              break;

            case IDC_Z0X :
              tmpMatrix.Rotate( 1, 90 * Math::DEGRAD );
              tmpMatrix.Rotate( 0, 90 * Math::DEGRAD );
              break;

            case IDC_Y0Z :
              tmpMatrix.Rotate( 0, -90 * Math::DEGRAD );
              tmpMatrix.Rotate( 1, -90 * Math::DEGRAD );
              break;
          }
          InvalidateRect( showHwnd, nullptr, TRUE );
          return TRUE;
        }

        case IDCANCEL :
        {
          EndDialog(hDlg, true);
          return TRUE;
        }
      }
      break;
  }

  return FALSE;
} 


//------------------------------------------------------------------------------
// 
// ---
bool ExecuteSetProjectionDlg( MbMatrix3D& matrix ) {
  tmpMatrix = matrix;
  tmpMatrix.SetOffset( MbCartPoint3D(0,0,0) );
  if ( DialogBox( hInst, MAKEINTRESOURCE(IDD_DIALOG_SELECT_PROJECTION), ParentForDialog(), (DLGPROC)DlgSetProjectionProc ) != 0 ) {
    matrix = tmpMatrix;
    return true;
  }
  else
    return false;
}


//------------------------------------------------------------------------------
// Обработка сообщений установки порядка NURBS
// ---
BOOL CALLBACK DlgSetSurfaceDegreeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM)
{
  switch (message) {

    case WM_INITDIALOG :
    {
      _stprintf( TestVariables::strBuffer, F_TD, Math::uSurfaceDegree() );  //-V111
      SetDlgItemText(hDlg, IDC_EDIT5, TestVariables::strBuffer );

      _stprintf( TestVariables::strBuffer, F_TD, Math::vSurfaceDegree() );  //-V111
      SetDlgItemText(hDlg, IDC_EDIT6, TestVariables::strBuffer );

      return TRUE;
    }

    case WM_COMMAND :
      switch (LOWORD(wParam)) {

        case IDOK :
        {
          GetDlgItemText( hDlg, IDC_EDIT5, TestVariables::strBuffer, 12 );
#if defined(PLATFORM_64)
#ifdef USE_VAR_CLASSES
          Math::uSurfaceDegree.SetVarValue( (ptrdiff_t)_ttoi64(TestVariables::strBuffer) );
#else
          Math::uSurfaceDegree = (ptrdiff_t)_ttoi64( TestVariables::strBuffer );
#endif
#else // PLATFORM_64
          Math::uSurfaceDegree.SetVarValue( (ptrdiff_t)_ttoi(TestVariables::strBuffer) );
#endif // PLATFORM_64

          GetDlgItemText( hDlg, IDC_EDIT6, TestVariables::strBuffer, 12 );
#if defined(PLATFORM_64)
#ifdef USE_VAR_CLASSES
          Math::vSurfaceDegree.SetVarValue( (ptrdiff_t)_ttoi64(TestVariables::strBuffer) );
#else
          Math::vSurfaceDegree = (ptrdiff_t)_ttoi64( TestVariables::strBuffer );
#endif
#else // PLATFORM_64
          Math::vSurfaceDegree.SetVarValue( (ptrdiff_t)_ttoi(TestVariables::strBuffer) );
#endif // PLATFORM_64

          EndDialog(hDlg, true);
          return TRUE;
        }

        case IDCANCEL :
        {
          EndDialog(hDlg, false);
          return TRUE;
        }
      }
      break;
  }

  return FALSE;
} // DlgSetSurfaceDegreeProc


//------------------------------------------------------------------------------
// Установить порядок поверхности
// ---
bool SetSurfaceDegree()
{
  return DialogBox( hInst, MAKEINTRESOURCE(IDD_DIALOG_ENTER_DEGREE), ParentForDialog(), (DLGPROC)DlgSetSurfaceDegreeProc ) != 0;
}


//------------------------------------------------------------------------------
//
// ---
struct DlgBoolParam {
  const TCHAR* prompt;   // Номер подсказки
  bool         defval;   // Значение по умолчанию
};


//------------------------------------------------------------------------------
// Обработка сообщений
// ---
BOOL CALLBACK DlgBoolProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
  switch (message) {

    case WM_INITDIALOG :
    {
      DlgBoolParam* param = (DlgBoolParam*)lParam;
      _tcscpy( (TCHAR*)TestVariables::strBuffer, param->prompt );
      CutString( TestVariables::strBuffer );
      SetDlgItemText( hDlg, IDC_TEXT, TestVariables::strBuffer );

      PostMessage( hDlg, WM_USER, param->defval, 0 );

      return TRUE;
    }

    case WM_USER : {  // Установка индикации начального значения
      if ( wParam ) {
        SendDlgItemMessage(hDlg, IDS_YES, BM_SETSTYLE, (WPARAM)LOWORD(BS_DEFPUSHBUTTON | WS_TABSTOP), MAKELPARAM(TRUE, 0) );
        SendDlgItemMessage(hDlg, IDS_NO,  BM_SETSTYLE, (WPARAM)LOWORD(BS_PUSHBUTTON | WS_TABSTOP), MAKELPARAM(TRUE, 0) );

        SetFocus( GetDlgItem(hDlg, IDS_YES) );
      }
      else {
        SendDlgItemMessage(hDlg, IDS_NO,  BM_SETSTYLE, (WPARAM)LOWORD(BS_DEFPUSHBUTTON | WS_TABSTOP), MAKELPARAM(TRUE, 0) );
        SendDlgItemMessage(hDlg, IDS_YES, BM_SETSTYLE, (WPARAM)LOWORD(BS_PUSHBUTTON | WS_TABSTOP), MAKELPARAM(TRUE, 0) );

        SetFocus( GetDlgItem(hDlg, IDS_NO) );
      }
      return TRUE;
    }

    case WM_COMMAND :
      switch (LOWORD(wParam)) {

        case IDS_YES :
          EndDialog(hDlg, 1 );
          return TRUE;

        case IDS_NO:
          EndDialog(hDlg, 0 );
          return TRUE;
      }
      break;
  }

  return FALSE;
} // DlgBoolProc


//------------------------------------------------------------------------------
// Ввод булевского значения
// ---
bool GetBool( const TCHAR* nPrompt, bool defValue )
{
  DlgBoolParam param;
    param.prompt = nPrompt;
    param.defval = defValue;
  return DialogBoxParam( hInst, MAKEINTRESOURCE(IDD_DIALOG_ENTER_BOOLEAN), ParentForDialog(),
                        (DLGPROC)DlgBoolProc, LPARAM(&param) ) == 1;
}


//------------------------------------------------------------------------------
// Обработка сообщений ориентации объекта
// ---
BOOL CALLBACK DlgSetOrientationProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM )
{
  static HWND showHwnd;
  switch (message) {

    case WM_INITDIALOG : {
    showHwnd = CreateAxisWindow( hDlg, IDC_STATIC );  
    return TRUE;
    }

    case WM_COMMAND :
      switch (LOWORD(wParam)) {

        case IDOK : {
          EndDialog(hDlg, true);
          TestVariables::setProj = true;
          return TRUE;
        }

        case IDCANCEL : {
          EndDialog(hDlg, true);
          return TRUE;
        }
      }
      break;
  }

  return FALSE;
} 


//------------------------------------------------------------------------------
// Процедура ввода контура
// ---
BOOL CALLBACK DlgContourProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM )
{
  switch (message) {

    case WM_INITDIALOG :
    {
      return TRUE;
    }

    case WM_CLOSE :
    {
      EndDialog( hDlg, false );
      return TRUE;
    }

    case WM_COMMAND :
      switch (LOWORD(wParam)) {

        case IDC_PUSHBUTTON_SPLINE : // Сплайн
        {
          EndDialog( hDlg, IDC_PUSHBUTTON_SPLINE );
          return TRUE;
        }
        case IDC_PUSHBUTTON_SEGMENT : // Прямая
        {
          EndDialog( hDlg, IDC_PUSHBUTTON_SEGMENT );
          return TRUE;
        }
        case IDC_PUSHBUTTON_POLYLINE : // Полилиния
        {
          EndDialog( hDlg, IDC_PUSHBUTTON_POLYLINE );
          return TRUE;
        }
        case IDC_PUSHBUTTON_ARC : // Дуга
        {
          EndDialog( hDlg, IDC_PUSHBUTTON_ARC );
          return TRUE;
        }
        case IDC_PUSHBUTTON_CLOSE : // Замкнуть
        {
          EndDialog( hDlg, IDC_PUSHBUTTON_CLOSE );
          return TRUE;
        }
        case IDC_PUSHBUTTON_EXIT : // Выход
        {
          EndDialog( hDlg, IDC_PUSHBUTTON_EXIT );
          return TRUE;
        }
      }
      break;
  }

  return FALSE;
} // DlgContourProc


//------------------------------------------------------------------------------
// Процедура ввода контура
//---
ptrdiff_t ExecuteContourDlg()
{
  ptrdiff_t result = 0;
  result = DialogBox( hInst, MAKEINTRESOURCE(IDD_DIALOG_ENTER_CONTOUR), ParentForDialog(), (DLGPROC)DlgContourProc);
  return result;
}


//------------------------------------------------------------------------------
//
// ---
BOOL CALLBACK DlgChooseProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM) {
  switch (message) {
    case WM_INITDIALOG :
      return TRUE;

    case WM_COMMAND :
      switch (wParam) {
        case IDOK :
          EndDialog(hDlg, 0 );
          return TRUE;

        case IDCANCEL : // Отказ
          EndDialog(hDlg, - 2 );
          return TRUE;

        case IDC_PUSHBUTTON_CHANGE : // Изменить
          EndDialog(hDlg, 1 );
          return TRUE;

        case IDC_PUSHBUTTON_STEPBACK : // Шаг назад
          EndDialog(hDlg, - 1 );
          return TRUE;
      }
  }

  return FALSE;
}


//------------------------------------------------------------------------------
// Выбор направления
// ---
ptrdiff_t ExecuteChooseDirectionDlg() {
  return DialogBox( hInst, MAKEINTRESOURCE(IDD_DIALOG_SELECT_DIRECTION), ParentForDialog(), (DLGPROC)DlgChooseProc/*lpfn*/);
}


//------------------------------------------------------------------------------
//AP
// ---
static BOOL CenterWindow( HWND hWnd )
{
  RECT rect;

  if( GetWindowRect(hWnd, &rect) ) {
    const int cx = rect.right - rect.left;
    const int cy = rect.bottom - rect.top;

    if( SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0) ) {
      const int sm_cx = rect.right - rect.left;
      const int sm_cy = rect.bottom - rect.top;

      //OV_x64 HWND hWP = (HWND)GetWindowLong( hWnd, GWL_HWNDPARENT );
      HWND hWP = (HWND)GetWindowLongPtr( hWnd, GWLP_HWNDPARENT );
      if( hWP == nullptr || !GetWindowRect( hWP, &rect ) )
        SetRect( &rect, 0, 0, sm_cx, sm_cy );

      int x = rect.left + ((rect.right - rect.left - cx) / 2);
      int y = rect.top + ((rect.bottom - rect.top - cy) / 2);

      /* // расширенный монитор (два монитора)
      if( x + cx > sm_cx ) 
        x = sm_cx - cx;
      if( x < 0 ) 
        x = 0;
      if( y + cy > sm_cy )
        y = sm_cy - cy;
      if( y < 0 ) 
        y = 0;
      //*/

      return SetWindowPos( hWnd, nullptr, x, y, -1, -1, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSIZE );
    }
  }

  return FALSE;
}


//------------------------------------------------------------------------------
// Выдача результатов теста времени с печатью в файл
// ---
LRESULT CALLBACK DlgTimeTestResultProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{
  switch( uMsg ) 
  {
    case WM_INITDIALOG: 
    {
      CenterWindow( hDlg );

      SetWindowText( hDlg, ((TCHAR**)lParam)[0] );
      SetDlgItemText( hDlg, IDC_TIMETESTRESULT_TEXT, ((TCHAR**)lParam)[3] );
      
      HANDLE hFile = nullptr;      // Дескриптор файла
      c3d::string_t fileName;   // Имя файла
      DWORD numOfBytes = 0;     // Количество реально записанных байт                              
      TCHAR buf[STRINGLENGTH];  

#if defined( C3D_DEBUG ) // Запись в файл      

      const uint sizeEOL = 4;   // Длина записи конца строки
      if ( _tcslen( TestVariables::nameBuffer ) > 3 ) 
      {
        fileName.assign( TestVariables::nameBuffer );
        fileName = fileName.substr( 0, fileName.find_last_of( _T(".") ) );
        fileName.append(_T(".txt"));
      }
      
      if ( TestVariables::rebuildTimeToFile ) 
      {
        // Создаём файл
        hFile = CreateFile( fileName.c_str(), // Имя файла 
                            GENERIC_WRITE,    // Создание на запись
                            0,                // Флаг доступа для другого процесса
                            nullptr,             // Аттрибуты безопасности
                            CREATE_ALWAYS,    // Всегда создаёт новый файл
                            FILE_ATTRIBUTE_NORMAL,// Аттрибуты файла
                            nullptr );
      }

      // Запись в файл, имя файла
      _tcscpy( buf, TestVariables::nameBuffer );

      if ( hFile ) 
      {
        WriteFile(hFile, buf, (DWORD)(_tcslen(buf)), &numOfBytes, nullptr);
        WriteFile( hFile, _T("\r\n\r\n"), sizeEOL, &numOfBytes, nullptr ); // Запись конца строки
        // Запись в файл, действие
        _tcscpy(buf,((TCHAR**)lParam)[0]);
        WriteFile(hFile, buf, (DWORD)(_tcslen(buf)), &numOfBytes, nullptr);
        WriteFile( hFile, _T("\r\n"), 2, &numOfBytes, nullptr ); // Запись конца строки
        // Запись в файл, число операций
        _tcscpy(buf,((TCHAR**)lParam)[1]);
        WriteFile(hFile, buf, (DWORD)(_tcslen(buf)), &numOfBytes, nullptr);
        WriteFile( hFile, _T("\r\n"), 2, &numOfBytes, nullptr ); // Запись конца строки
        // Запись в файл, число оболочек
        _tcscpy(buf,((TCHAR**)lParam)[2]);
        WriteFile(hFile, buf, (DWORD)(_tcslen(buf)), &numOfBytes, nullptr);
        WriteFile( hFile, _T("\r\n"), 2, &numOfBytes, nullptr ); // Запись конца строки
        // Запись в файл, время выполнения
        _tcscpy(buf,((TCHAR**)lParam)[3]);
        WriteFile(hFile, buf, (DWORD)(_tcslen(buf)), &numOfBytes, nullptr);
        WriteFile( hFile, _T("\r\n\r\n"), sizeEOL, &numOfBytes, nullptr ); // Запись конца строки

        // Запись в файл
        WriteFile(hFile, RT_FUNC_NAME_TIME, 
                  (DWORD)_tcslen( RT_FUNC_NAME_TIME ), &numOfBytes, nullptr);
        WriteFile( hFile, _T("\r\n"), 2, &numOfBytes, nullptr ); // Запись конца строки
      }
#endif // C3D_DEBUG
      
      // Вывод в окно
      HWND hWndLV = GetDlgItem( hDlg, IDC_TIMETESTRESULT_LV );

      static TCHAR * szColumnNames[] = { RT_OPERATION_NAME, RT_RUNNING_TIME, RT_AVERAGE_TIME, RT_CALLS_COUNT };
      static int iColumnWidths[] = { 100, 80, 80, 80 };
      ListView_SetExtendedListViewStyle( hWndLV, LVS_EX_FULLROWSELECT  ); // | LVS_EX_GRIDLINES

      LV_COLUMN lvc;
      memset( &lvc, 0, sizeof(lvc) );

      for ( size_t i = 0; i < 4; i++ ) 
      {
        lvc.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
        lvc.fmt = i > 0 ? LVCFMT_RIGHT : LVCFMT_LEFT;
        lvc.cx = iColumnWidths[i];
        lvc.pszText = szColumnNames[i];
        ListView_InsertColumn( hWndLV, LoUint32(i), &lvc );
      }

      LVITEM lvi; // = { 0 };
      memset( &lvi, 0, sizeof(lvi) );

      lvi.mask = LVIF_TEXT;
      lvi.iItem = 0;
      lvi.iSubItem = 0;
      lvi.pszText = _T(" ");
      ListView_InsertItem( hWndLV, &lvi );               //-V202

      lvi.iItem = 1;
      lvi.iSubItem = 0;
      lvi.pszText = const_cast<TCHAR*>( ((TCHAR**)lParam)[2] ); // Число граней
      ListView_InsertItem( hWndLV, &lvi );               //-V202

      lvi.iItem = 2;
      lvi.iSubItem = 0;
      lvi.pszText = const_cast<TCHAR*>( ((TCHAR**)lParam)[1] ); // Число операций
      ListView_InsertItem( hWndLV, &lvi );               //-V202

      lvi.iItem = 3;
      lvi.iSubItem = 0;
      lvi.pszText = _T(" ");
      ListView_InsertItem( hWndLV, &lvi );               //-V202

      // Вывод в окно и запись в файл
      TimeTest * testResult = ((TimeTest**)lParam)[4];   //-V112

      if ( testResult != nullptr )
      {
        std::vector<TimeTestResult> sumResults;
        sumResults.reserve( 20 );

        ::SortResultMeasuring( *testResult, sumResults );
        TCHAR  buffer[15];  // Буфер для записи цифр
        TCHAR  zero[30];    // Буфер для пробелов
        
        for ( size_t m = 0; m < 30; m++ )
          zero[m] = TCHAR(0); // Очистка буфера

        for ( size_t i = 0; i < sumResults.size(); i++ ) 
        {
          lvi.mask = LVIF_TEXT;
          lvi.iItem = (int)LoUint32(i + 4);
          lvi.iSubItem = 0;
          lvi.pszText = const_cast<TCHAR*>( sumResults[(size_t)i].name.c_str() );
          ListView_InsertItem( hWndLV, &lvi );
   
          if ( hFile ) 
          {
            // Запись в файл
            WriteFile( hFile, sumResults[(size_t)i].name.c_str(), (DWORD)_tcslen(sumResults[(size_t)i].name.c_str()), &numOfBytes, nullptr );
            // Запись пробелов
            WriteFile( hFile, zero, (DWORD)(sizeof(zero) - _tcslen(sumResults[(size_t)i].name.c_str()) - 2), &numOfBytes, nullptr );
          }

          int timeResult = (int)sumResults[(size_t)i].GetMiliseconds(); // Перевести timeResult в милисекунды и выдать 

          if ( timeResult > 0 ) {
            _stprintf( buf, _T("%d"), timeResult );
            lvi.iSubItem = 1;
            lvi.pszText = buf;
            ListView_SetItem( hWndLV, &lvi );

            _stprintf( buf, F_TD, ((size_t)timeResult / sumResults[(size_t)i].count) );  //-V111
            lvi.iSubItem = 2;
            lvi.pszText = buf;
            ListView_SetItem( hWndLV, &lvi );

            _stprintf( buf, F_TD, sumResults[(size_t)i].count );  //-V111
            lvi.iSubItem = 3;
            lvi.pszText = buf;
            ListView_SetItem( hWndLV, &lvi );

            // Запись в файл
            memcpy(buffer,zero,sizeof(buffer));
            _itot(timeResult, buffer, 10);// Запись числа в буфер
            if ( hFile ) {
              WriteFile( hFile, zero, (DWORD)(sizeof(buffer) - _tcslen(buffer) - 1), &numOfBytes, nullptr );// Запись пробелов перед числом
              WriteFile( hFile, buffer, (DWORD)(_tcslen(buffer)), &numOfBytes, nullptr );// Запись числа
            }
            // Запись в файл
            memcpy(buffer,zero,sizeof(buffer));
#if defined(PLATFORM_64)
            _ui64tot(sumResults[(size_t)i].averageTime, buffer, 10);// Запись числа в буфер
#else //  PLATFORM_64
            _itot( (uint32)sumResults[(size_t)i].averageTime, buffer, 10);// Запись числа в буфер
#endif // PLATFORM_64
            if ( hFile ) 
            {
              WriteFile( hFile, zero, (DWORD)(sizeof(buffer) - _tcslen(buffer) - 1), &numOfBytes, nullptr );// Запись пробелов перед числом
              WriteFile( hFile, buffer, (DWORD)(_tcslen(buffer)), &numOfBytes, nullptr );// Запись числа
            }
            // Запись в файл
            memcpy(buffer,zero,sizeof(buffer));
#if defined(PLATFORM_64)
            _ui64tot(sumResults[(size_t)i].count, buffer, 10);// Запись числа в буфер
#else //  PLATFORM_64
            _itot( (uint32)sumResults[(size_t)i].count, buffer, 10);// Запись числа в буфер
#endif // PLATFORM_64
            if ( hFile ) {
              WriteFile( hFile, zero, (DWORD)(sizeof(buffer) - _tcslen(buffer) - 1), &numOfBytes, nullptr );// Запись пробелов перед числом
              WriteFile( hFile, buffer, (DWORD)(_tcslen(buffer)), &numOfBytes, nullptr );// Запись числа
            }
          }
          if ( hFile ) 
          {
            // Запись конца строки
            WriteFile( hFile, _T("\r\n"), 2, &numOfBytes, nullptr );
          }
        }
      }

//#endif // C3D_DEBUG

      RECT rect;
      GetClientRect( hWndLV, &rect );
      iColumnWidths[0] = rect.right - rect.left - iColumnWidths[1] - iColumnWidths[2] - iColumnWidths[3];
      ListView_SetColumnWidth( hWndLV, 0, iColumnWidths[0] );

#if defined(C3D_DEBUG)    
      // Закрываем файл
      if ( TestVariables::rebuildTimeToFile ) 
      {
        CloseHandle(hFile);
      }
      fileName.clear();
#endif // C3D_DEBUG

      return TRUE;
    }

    case WM_COMMAND:
      if( LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL ) {
        EndDialog( hDlg, LOWORD(wParam) );
        return TRUE;
      }
      break;
  }

  return FALSE;
}


//------------------------------------------------------------------------------
// 
// ---
bool ExecuteTimeTestResultDlg(const void * params) {
  return DialogBoxParam( hInst, (LPCTSTR)IDD_TIMETESTRESULT, ParentForDialog(), (DLGPROC)DlgTimeTestResultProc, (LPARAM)params ) == IDOK;
}


//------------------------------------------------------------------------------
// Выдать информационное сообщение
// ---
void MessageBoxEx( const TCHAR * text, const TCHAR * title ) {
  ::MessageBox( ParentForDialog(), text, title, MB_ICONEXCLAMATION | MB_OK );
}


//------------------------------------------------------------------------------
// Изменить цвет элемента
// ---
bool CMUColor( COLORREF & crColor )
{
  bool choose = false;
  CHOOSECOLOR ccTemp;
  COLORREF crTemp[16];  // Important, sometimes unused, array

// Set the custom color controls to white. 

  for ( ptrdiff_t i = 0; i < 16; i++ )
    crTemp[i] = RGB(255, 255, 255);

  memset(&ccTemp, 0, sizeof(CHOOSECOLOR));

  ccTemp.lStructSize = sizeof( CHOOSECOLOR );
  ccTemp.hwndOwner = ParentForDialog(); //hWnd;
  ccTemp.hInstance = 0;
  ccTemp.rgbResult = crColor; // CC_RGBINIT flag makes this the default color

  ccTemp.lpCustColors = crTemp;
  ccTemp.Flags = CC_RGBINIT;
  ccTemp.lCustData = 0L;
  ccTemp.lpfnHook = nullptr;
  ccTemp.lpTemplateName = nullptr;

  if ( ChooseColor( &ccTemp ) != 0 ) {
    crColor = ccTemp.rgbResult;
    choose = true;
  }

  //InvalidateRect( hWnd, nullptr, true );
  if ( choose )
  {
    ptrdiff_t index = TestVariables::viewManager->GetActiveWindowIndex();
    TestVariables::viewManager->InvalidateWindow( index, true );
  }
  return choose;
}


#endif // __USE_QT__ 
