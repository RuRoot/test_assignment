#include <test.h>
#include "test_std_afx.h"
#include <test_service.h>
#include <signal.h>
#include <tool_log.h>
#include <last.h>

using namespace c3d;


#ifdef __cplusplus
   typedef void (*fptr)(int);
#else
   typedef void (*fptr)();
#endif


//TCHAR  szFrameClass[] = _T("3D Frame");


//------------------------------------------------------------------------------
//
// ---
void Catcher( int reglist ) {
  PRECONDITION( false ); // Check exception!!!
  reglist = reglist;
}


#if defined(C3D_DEBUG)
//------------------------------------------------------------------------------
// Контроль утечек памяти
// ВНИМАНИЕ! ВО ВСЕХ МОДУЛЯХ ДОЛЖЕН БЫТЬ ВКЛЮЧЕН ФЛАГ /MDd (Debug Multithreaded DLL)
// ИНАЧЕ КОНТРОЛЬ УТЕЧЕК БУДЕТ ОСУЩЕСТВЛЯТСЯ ТОЛЬКО В ДАННОМ Dll
// (другой вариант: данный класс поместить например в Vars.h (тоже будет работать))
// Дополнение:
// При создании глобального объекта в других dll (см. GCBase\MtDebugTools.cpp) 
// Может некорректно ловить утечки (на примере BUG_21945), т.к. глобальные объекты
// Уничтожаются позднее чем объект класса контроля утечек. В случае необходимости можно
// Создать объект класса контроля утечек глобально и поместить в нижние dll
// PS: Подкорректируйте, если есть некорректность в терминологии
// ---
class MemoryLeaksDebug {
protected:
_CrtMemState checkpoint_f;

public:
  MemoryLeaksDebug()
  { 
    _CrtMemCheckpoint(&checkpoint_f);
  };
  ~MemoryLeaksDebug()
  { 
    //_CrtMemState checkpoint_l;
    //_CrtMemCheckpoint(&checkpoint_l);
    //_CrtMemState diff;
    //_CrtMemDifference(&diff, &checkpoint_f, &checkpoint_l);
    //_CrtMemDumpStatistics(&diff);
    //_CrtMemDumpAllObjectsSince(&diff);
    _CrtMemDumpAllObjectsSince(&checkpoint_f);
  };
};
#endif // defined(C3D_DEBUG)


//------------------------------------------------------------------------------
// Принудительный контроль деления на ноль (SD#739012 — ошибки/исключения при чтении асис файлов)
// ---
#if defined(C3D_DEBUG) && defined(C3D_WINDOWS) //_MSC_VER
void TurnOnFloatingExceptions()
{
  unsigned int cw;
  // Note : same result with controlfp
  cw = _control87(0,0) & MCW_EM;
  //cw &= ~(_EM_INVALID|_EM_ZERODIVIDE|_EM_OVERFLOW); // 25.04.2017, Ladilova, Вылеты при работе через RDP.
  cw &= ~(_EM_ZERODIVIDE|_EM_OVERFLOW);
  _control87(cw,MCW_EM);
}
#else
void TurnOnFloatingExceptions()
{
}
#endif


//------------------------------------------------------------------------------
// Парсинг командной строки
// ---
PTCHAR * CommandLineToArgv( TCHAR * CmdLine, ptrdiff_t * _argc )
{
  PTCHAR * argv;
  PTCHAR  _argv;
  size_t len;
  size_t argc;
  size_t i, j;

  BOOLEAN  in_QM;
  BOOLEAN  in_TEXT;
  BOOLEAN  in_SPACE;

  len = _tcslen( CmdLine );
  i = ((len+2)/2)*sizeof(PVOID) + sizeof(PVOID);   //-V119

  argv = (PTCHAR*)GlobalAlloc(GMEM_FIXED,
    i + (len+2)*sizeof(TCHAR));

  _argv = (PTCHAR)(((PUCHAR)argv)+i);

  argc = 0;
  argv[argc] = _argv;
  in_QM = FALSE;
  in_TEXT = FALSE;
  in_SPACE = TRUE;
  i = 0;
  j = 0;

  TCHAR a = CmdLine[i];
  while( a ) 
  {
    if( in_QM ) 
    {
      if ( a == '\"' )
        in_QM = FALSE;
      else {
        _argv[j] = a;
        j++;
      }
    } 
    else 
    {
      switch(a) {
        case '\"':
        {
          in_QM = TRUE;
          in_TEXT = TRUE;
          if ( in_SPACE ) 
          {
            argv[argc] = _argv+j;
            argc++;
          }
          in_SPACE = FALSE;
          break;
        }
        case ' ':
        case '\t':
        case '\n':
        case '\r':
          if(in_TEXT) {
            _argv[j] = '\0';
            j++;
          }
          in_TEXT = FALSE;
          in_SPACE = TRUE;
          break;
        default:
          in_TEXT = TRUE;
          if(in_SPACE) {
            argv[argc] = _argv+j;
            argc++;
          }
          _argv[j] = a;
          j++;
          in_SPACE = FALSE;
          break;
      }
    }
    i++;

    a = CmdLine[i];
  }

  _argv[j] = '\0';
  argv[argc] = nullptr;

  (*_argc) = argc;
  return argv;
}


////////////////////////////////////////////////////////////////
//
//                        Старт
//
////////////////////////////////////////////////////////////////
#ifndef __USE_QT__ 
int PASCAL _tWinMain( HINSTANCE _hInst, HINSTANCE /*hPrevInstance*/, TCHAR * args, int _nCmdShow )
{
#if defined(C3D_DEBUG) && !defined(ENABLE_VLD)
  c3d::EnableMemoryLeakDump(); // Единообразно с Компас
  // _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)|_CRTDBG_LEAK_CHECK_DF); // SD#771697 — ложные утечки памяти в тестовом приложении 
  // MemoryLeaksDebug mld;
#endif // defined(C3D_DEBUG)

#if defined(C3D_DEBUG) && defined(C3D_WINDOWS) //_MSC_VER
  // Принудительный контроль деления на ноль (SD#739012 — ошибки/исключения при чтении асис файлов)
  TurnOnFloatingExceptions();
#ifdef PROTECTION_ENABLED // for customers: to Output window by default
  int mode = _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_REPORT_MODE );

  if ( TestFlagValue( mode, _CRTDBG_MODE_WNDW ) ) {
    DownFlagValue( mode, _CRTDBG_MODE_WNDW );
    UpFlagValue  ( mode, _CRTDBG_MODE_DEBUG ); // output window
    _CrtSetReportMode( _CRT_ASSERT, mode );
  }
#endif
#endif

  InitCommonControls();

//  signal(SIGFPE, (fptr)Catcher);

  hInst = _hInst;
  nCmdShow = _nCmdShow;

  if ( !CreateFrameWindow() )  // Создать главное окно задачи
    return false;

  hwndClient = GetWindow( hwndFrame, GW_CHILD );

  ptrdiff_t argc = 0;
  PTCHAR * argv  = ::CommandLineToArgv( args, &argc );

  if ( argc > 0 ) {
    if ( argc == 1 ) {
      ReadModel( argv[0] );
      PostMessage( hwndFrame, WM_COMMAND, CM_WINDOWCLOSE_EMPTY, 0 ); // чтобы было одно окно при чтении из командой строки
    }
    else if( !_tcscmp( argv[0], _T("-red") ) ) {
      ReadModel( argv[1] );
      PostMessage( hwndFrame, WM_COMMAND, CM_WINDOWCLOSE_EMPTY, 0 ); // чтобы было одно окно при чтении из командой строки
    }
  }

  VersionContainer::SetDefaultVersion( Math::DefaultVersion() ); // Версия по умолчанию 

  HACCEL hAccel = LoadAccelerators( hInst, MAKEINTRESOURCE( ACCELERATORS_BASE ) );

  MSG msg;
  while ( GetMessage ( &msg, nullptr, 0, 0 ) ) {
    if ( !TranslateMDISysAccel( hwndClient, &msg ) &&
         !TranslateAccelerator( hwndFrame, hAccel, &msg ) ) {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }
  }

  DestroyFrameWindow(); // Удалить главное окно задачи
  ::DeleteObject( _hInst );

  return (int)msg.wParam;
}

#endif // __USE_QT__ 
