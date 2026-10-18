////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строка.
         \en String. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TOOL_CSTRING_H
#define __TOOL_CSTRING_H

#include <math_x.h>
#include <string.h>
#include <wchar.h>
#include <tool_string_util.h>
#include <fstream>
#include <sstream>
#include <math_define.h>

#ifndef C3D_WINDOWS //_MSC_VER  
  #include <stdio.h>
  #include <stdlib.h>
  #include <string>
#else // C3D_WINDOWS 
  #include <tchar.h>
  #include <iosfwd>
  #include <system_dependency.h>
#endif // C3D_WINDOWS 

#ifndef C3D_WINDOWS //_MSC_VER  
  #ifndef _UNICODE
    typedef char TCHAR;
    #define _T(x)      x
    #define _tcsstr    strstr 
    #define _tcscpy    strcpy
    #define _tcsncpy   strncpy
    #define _tcslen    strlen
    #define _tcscmp    strcmp
    #define _tcsnccmp  strncmp
    #define _tcsftime  strftime
  #else // _UNICODE
    typedef wchar_t TCHAR;
    #define _T(x)      L ## x
    #define _tcsstr    wcsstr
    #define _tcscpy    wcscpy
    #define _tcsncpy   wcsncpy
    #define _tcslen    wcslen
    #define _tcscmp    wcscmp
    #define _tcsnccmp  wcsncmp
    #define _tcsftime  wcsftime
  #endif // _UNICODE

  #define _gcvt  gcvt
  #define _ecvt  ecvt

  #define _istspace  isspace
  #define _istdigit  isdigit
  #define _istalnum  isalnum

  ////////////////////////////////////////////////////////////////////////////////////////////
  //
  // \ru Функции конвертации чисел в строку и обратно \en Functions for the convertation of numbers to strings and strings to numbers. 
  //
  ////////////////////////////////////////////////////////////////////////////////////////////
  #define _saprintf  sprintf       // char*

  #ifndef __MOBILE_VERSION__
    //#define _sntprintf  sprintf       // TCHAR*
    //#define _tcscat    strcat
    #ifdef _UNICODE
      #define _sntprintf  swprintf       // TCHAR*
      #define _tcscat    wcscat
    #else // _UNICODE
      #define _sntprintf  snprintf       // TCHAR*
      #define _tcscat    strcat
    #endif // _UNICODE
  #else // __MOBILE_VERSION__
    #ifdef _UNICODE
      #define _sntprintf  swprintf       // TCHAR*
      #define _tcscat    wcscat
    #else // _UNICODE
      #define _sntprintf  snprintf       // TCHAR*
      #define _tcscat    strcat
    #endif // _UNICODE
  #endif // __MOBILE_VERSION__

  #define AF_I32D     "%d"
  #define AF_I64D     "%ld"
  #define AF_I32H     "%x"
  #define AF_I64H     "%lx"

  #define F_I32D     _T("%d")
  #define F_I64D     _T("%ld")
  #define F_I32U     _T("%u")
  #define F_I64U     _T("%lu")
  #define F_I32H     _T("%x")
  #define F_I64H     _T("%lx")
  #define F_I32D_03  _T("%03d")
  #define F_I32D_06  _T("%06d")
  #define F_I64D_03  _T("%03ld")
  #define F_I64D_06  _T("%06ld")

  #if defined(_UNICODE)
    #if defined(PLATFORM_64)
      #define LF_TD     L"%ld"
    #else  // PLATFORM_64
      #define LF_TD     L"%d"
    #endif  // PLATFORM_64
  #endif  // _UNICODE

  #if defined( __MOBILE_VERSION__ )
    #define LLOG_PATH   L"~/Logs/"
    #define LF_I32D     L"%d"
    #if defined(PLATFORM_64)
      #define LF_TU     L"%lu"
      #define LF_TH     L"%lx"
      #define LF_TD_03  L"%03ld"
      #define LF_TD_06  L"%06ld"
    #else //  PLATFORM_64
      #define LF_TU     L"%u"
      #define LF_TH     L"%x"
      #define LF_TD_03  L"%03d"
      #define LF_TD_06  L"%06d"
    #endif //  PLATFORM_64
  #endif

  #ifndef _UNICODE
    #define _acstoi    strtol        // char*  -> int32
    #define _acstoi64  strtol        // char*  -> int64
    #define _acstoui   strtoul       // char*  -> uint32
    #define _acstoui64 strtoul       // char*  -> uint64
    #define _acstod    strtod        // char*  -> double

    #define _tcstoi    strtol        // TCHAR*  -> int32
    #define _tcstoi64  strtol        // TCHAR*  -> int64
    #define _tcstoui   strtoul       // TCHAR*  -> uint32
    #define _tcstoui64 strtoul       // TCHAR*  -> uint64
    #define _tcstod    strtod        // TCHAR*  -> double

    #define _as16toi   strtol        // char* (HEX_IMAGE)  -> int32
  #else // _UNICODE
    #define _acstoi    wcstol        // char*  -> int32
    #define _acstoi64  wcstol        // char*  -> int64
    #define _acstoui   wcstoul       // char*  -> uint32
    #define _acstoui64 wcstoul       // char*  -> uint64
    #define _acstod    wcstod        // char*  -> double

    #define _tcstoi    wcstol        // TCHAR*  -> int32
    #define _tcstoi64  wcstol        // TCHAR*  -> int64
    #define _tcstoui   wcstoul       // TCHAR*  -> uint32
    #define _tcstoui64 wcstoul       // TCHAR*  -> uint64
    #define _tcstod    wcstod        // TCHAR*  -> double

    #define _as16toi   wcstol        // char* (HEX_IMAGE)  -> int32
  #endif // _UNICODE

  #if defined(PLATFORM_64)
    #define _acstot    _acstoi64     // char*  -> ptrdiff_t
    #define _acstout   _acstoui64    // char*  -> size_t
    #define _tcstot    _tcstoi64     // TCHAR* -> ptrdiff_t
    #define _tcstout   _tcstoui64    // TCHAR* -> size_t

    #define AF_TD      AF_I64D
    #define AF_TH      AF_I64H

    #define F_TD       F_I64D
    #define F_TU       F_I64U
    #define F_TH       F_I64H
    #define F_TD_03    F_I64D_03
    #define F_TD_06    F_I64D_06
  #else //  PLATFORM_64
    #define _acstot    _acstoi       // char*  -> ptrdiff_t
    #define _acstout   _acstoui      // char*  -> size_t
    #define _tcstot    _tcstoi       // TCHAR* -> ptrdiff_t
    #define _tcstout   _tcstoui      // TCHAR* -> size_t

    #define AF_TD      AF_I32D
    #define AF_TH      AF_I32H

    #define F_TD       F_I32D
    #define F_TU       F_I32U
    #define F_TH       F_I32H
    #define F_TD_03    F_I32D_03
    #define F_TD_06    F_I32D_06
  #endif //  PLATFORM_64

//#ifndef _MSC_VER

  #define _tcsrev  reverse_string

  inline TCHAR *reverse_string( TCHAR *s ) {
    size_t len = _tcslen( s );
   
    if ( len > 1 ) {
      TCHAR *a = s;
      TCHAR *b = s + len - 1;
   
      for (; a < b; ++a, --b) { 
        TCHAR _save = *a; *a = *b; *b = _save;
      }  
    }  
    return s;
  }
//#endif // _MSC_VER

#else // C3D_WINDOWS

  ////////////////////////////////////////////////////////////////////////////////////////////
  //
  // \ru Функции конвертации чисел в строку и обратно \en Functions for the convertation of numbers to strings and strings to numbers 
  //
  ////////////////////////////////////////////////////////////////////////////////////////////
  #define _saprintf  sprintf       // char*
  // #define _sntprintf             // TCHAR*
  #define F_I32D     _T("%d")
  #define F_I64D     _T("%I64d")
  #define F_I32U     _T("%u")
  #define F_I64U     _T("%I64u")
  #define F_I32H     _T("%x")
  #define F_I64H     _T("%I64x")
  #define F_I32D_03  _T("%03d")
  #define F_I32D_06  _T("%06d")
  #define F_I64D_03  _T("%03I64d") 
  #define F_I64D_06  _T("%06I64d") 

  #define AF_I32D     "%d"
  #define AF_I64D     "%I64d"
  #define AF_I32U     "%u"
  #define AF_I64U     "%I64u"
  #define AF_I32H     "%x"
  #define AF_I64H     "%I64x"
  #define AF_I32D_03  "%03d"
  #define AF_I32D_06  "%06d"
  #define AF_I64D_03  "%03I64d"
  #define AF_I64D_06  "%06I64d"

#if defined(PLATFORM_64)
  #define AF_TD      AF_I64D
  #define AF_TH      AF_I64H
#else
  #define AF_TD      AF_I32D
  #define AF_TH      AF_I32H
#endif

  #define _acstoi    strtol        // char*  -> int32
  #define _acstoi64  _strtoi64     // char*  -> int64
  #define _acstoui64 _strtoui64    // char*  -> uint64

  #define _acstoui   strtoul       // char*  -> uint32
  #define _acstod    strtod        // char*  -> double

  #define _tcstoi    _tcstol       // TCHAR*  -> int32
  #define _tcstoui   _tcstoul      // TCHAR*  -> uint32

  #define _as16toi   strtol        // char* (HEX_IMAGE)  -> int32

  #if defined(PLATFORM_64)
    #define _acstot    _acstoi64     // char*  -> ptrdiff_t
    #define _acstout   _acstoui64    // char*  -> size_t
    #define _tcstot    _tcstoi64     // TCHAR* -> ptrdiff_t
    #define _tcstout   _tcstoui64    // TCHAR* -> size_t

    #define F_TD       F_I64D
    #define F_TU       F_I64U
    #define F_TH       F_I64H
    #define F_TD_03    F_I64D_03
    #define F_TD_06    F_I64D_06
  #else //  PLATFORM_64
    #define _acstot    _acstoi       // char*  -> ptrdiff_t
    #define _acstout   _acstoui      // char*  -> size_t
    #define _tcstot    _tcstoi       // TCHAR* -> ptrdiff_t
    #define _tcstout   _tcstoui      // TCHAR* -> size_t

    #define F_TD       F_I32D
    #define F_TU       F_I32U
    #define F_TH       F_I32H
    #define F_TD_03    F_I32D_03
    #define F_TD_06    F_I32D_06
  #endif //  PLATFORM_64
#endif //C3D_WINDOWS


namespace c3d // namespace C3D
{

//------------------------------------------------------------------------------
/** \brief \ru Определение строки для модуля ядра C3D.
           \en Definition of the string used by the C3D kernel. \~  
  \ingroup Base_Tools_String
*/
typedef std::basic_string<TCHAR> string_t;
typedef TCHAR    mt_char;
typedef string_t mt_string;

#ifdef C3D_WINDOWS //_MSC_VER 
typedef string_t path_string;
#else
typedef std::string path_string;
#endif


#ifdef  _UNICODE
typedef std::wofstream      t_ofstream;
typedef std::wifstream      t_ifstream;
typedef std::wostringstream t_stringstream;
#else // _UNICODE
typedef std::ofstream       t_ofstream;
typedef std::ifstream       t_ifstream;
typedef std::ostringstream  t_stringstream;
#endif // _UNICODE


typedef std::vector<string_t>         StringTVector;
typedef std::vector<string_t>&        StringTVectorRef;
typedef const std::vector<string_t>&  StringTVectorCRef;
typedef string_t&                     StringTRef;
typedef const string_t&               StringTCRef;

typedef path_string&                  PathStringRef;
typedef const path_string&            PathStringCRef;


//------------------------------------------------------------------------------
/** \brief \ru Размер строки в памяти.
           \en Memory allocated by string. \~  
  \ingroup Base_Tools_String
*/
inline size_t size_of( string_t s )
{
  size_t size = sizeof( string_t );
  size += s.length() * sizeof(TCHAR);
  return size;
}


//------------------------------------------------------------------------------
/** \brief \ru Функция преобразования мультибайтовой строки к строке с широким символом.
           \en String transformation from multibyte to wide-char. \~  
  \ingroup Base_Tools_String
*/
inline std::wstring StdToWString( const std::string & s ) {
  std::wstring result;
  if ( !s.empty() ) {
    wchar_t * str = mbsnewwcs( s.c_str() );
    if ( str )
      result.assign( str );
    delete [] str;
  }
  return result;
}


//------------------------------------------------------------------------------
/** \brief \ru Функция преобразования строки с широким символом к мультибайтовой.
           \en String transformation from wide-char to multibyte. \~  
  \ingroup Base_Tools_String
*/
inline std::string WToStdString( const std::wstring & s ) {
  std::string result;
  if ( !s.empty() ) {
    char* str = wcsnewmbs( s.c_str() );
    if ( str )
      result.assign( str );
    delete [] str;
  }
  return result;
}


//------------------------------------------------------------------------------
/** \brief \ru Функция преобразования строки к формату C3D.
           \en String transformation to the C3D form. \~  
  \ingroup Base_Tools_String
*/
inline string_t ToC3Dstring( const std::string & s )
{ 
#ifdef _UNICODE
    return StdToWString(s);
#else // _UNICODE
    return s.empty() ? string_t() :  s.c_str(); 
#endif // _UNICODE
}


//------------------------------------------------------------------------------
/** \brief \ru Функция преобразования строки к стандартной.
           \en String transformation to the standard form. \~  
  \ingroup Base_Tools_String
*/
inline std::string ToSTDstring( const string_t & s )
{ 
#ifdef _UNICODE
  return WToStdString(s);
#else // _UNICODE
  return s.empty() ? std::string() : s.c_str(); 
#endif // _UNICODE
}


//------------------------------------------------------------------------------
/** \brief \ru Функция преобразования строки к формату C3D.
           \en String transformation to the C3D form. \~
  \ingroup Base_Tools_String
*/
inline string_t ToC3Dstring( const std::wstring & s )
{
#ifdef _UNICODE
    return s.empty() ? string_t() :  s.c_str();
#else // _UNICODE
  return WToStdString(s);    
#endif // _UNICODE
}


//------------------------------------------------------------------------------
/** \brief \ru Функция преобразования строки к стандратной для пути.
           \en String transformation to the standard form. \~
  \ingroup Base_Tools_String
*/
inline path_string WToPathstring( const std::wstring & s )
{
#ifdef C3D_WINDOWS //_MSC_VER 
#ifdef _UNICODE
    return s;
#else  // _UNICODE
  return WToStdString(s);
#endif  // _UNICODE
  #else
    return WToStdString(s);    
  #endif
}


//------------------------------------------------------------------------------
/** \brief \ru Функция преобразования строки к стандартной.
           \en String transformation to the standard form. \~
  \ingroup Base_Tools_String
*/
inline std::wstring ToWstring( const string_t & s )
{
#ifdef _UNICODE
  return s.empty() ? std::wstring() : s.c_str();
#else // _UNICODE
  return StdToWString(s);
#endif // _UNICODE
}

//------------------------------------------------------------------------------
/** \brief \ru Функция преобразования строки к стандартной.
            \en String transformation to the standard form. \~
  \ingroup Base_Tools_String
*/
inline std::string PathToSTDstring( const path_string & s )
{
#ifdef _UNICODE
  #ifdef C3D_WINDOWS //_MSC_VER 
    return WToStdString(s);
  #else  // C3D_WINDOWS
    return s;
  #endif // C3D_WINDOWS
#else // _UNICODE
  #ifdef C3D_WINDOWS //_MSC_VER 
    return s;
  #else  // C3D_WINDOWS
    return s;
  #endif // C3D_WINDOWS
#endif // _UNICODE
}

//------------------------------------------------------------------------------
/** \brief \ru Функция преобразования строки к стандартной.
            \en String transformation to the standard form. \~
  \ingroup Base_Tools_String
*/
inline string_t PathToC3Dstring( const path_string & s )
{
#ifdef _UNICODE
  #ifdef C3D_WINDOWS //_MSC_VER 
    /*string_t result;
    if ( !s.empty() ) {
      char* str = wcsnewmbs( s.c_str() );
      if ( str )
        result.assign( str );
      delete [] str;
    }
    return result;*/
    return s;
  #else  // C3D_WINDOWS
    return ToC3Dstring( s );
  #endif // C3D_WINDOWS
#else // _UNICODE
  #ifdef C3D_WINDOWS //_MSC_VER 
    return s;
  #else  // C3D_WINDOWS
    return s;
  #endif // C3D_WINDOWS
#endif // _UNICODE
}

//------------------------------------------------------------------------------
/** \brief \ru Функция преобразования строки к стандартной.
            \en String transformation to the standard form. \~
  \ingroup Base_Tools_String
*/
inline path_string StdToPathstring( const std::string & s )
{
#ifdef _UNICODE
  #ifdef C3D_WINDOWS //_MSC_VER
    return StdToWString(s);
  #else  // C3D_WINDOWS
    return s;
  #endif // C3D_WINDOWS
#else // _UNICODE
  #ifdef C3D_WINDOWS //_MSC_VER
    return s;
  #else  // C3D_WINDOWS
    return s;
  #endif // C3D_WINDOWS
#endif // _UNICODE
}

//------------------------------------------------------------------------------
/** \brief \ru Функция преобразования строки к стандартной.
            \en String transformation to the standard form. \~
  \ingroup Base_Tools_String
*/
inline path_string C3DToPathstring( const string_t & s )
{
#ifdef _UNICODE
  #ifdef C3D_WINDOWS //_MSC_VER
    return s;
  #else // C3D_WINDOWS
    return ToSTDstring( s );
  #endif // C3D_WINDOWS
#else // _UNICODE
  #ifdef C3D_WINDOWS //_MSC_VER
    return s;
  #else // C3D_WINDOWS
    return ToSTDstring( s );
  #endif // C3D_WINDOWS
#endif // _UNICODE
}


//------------------------------------------------------------------------------
/** \brief \ru Перевести символы в нижний регистр.
            \en convert symbols to lower case. \~
  \ingroup Base_Tools_String
*/
inline void ToLower( ::std::string & v ) {
  for( size_t i = 0, sz = v.size(); i < sz; i++ )
    v[i] = char(tolower( v[i] ));
}


//------------------------------------------------------------------------------
/** \brief \ru Перевести символы в нижний регистр.
            \en convert symbols to lower case. \~
  \ingroup Base_Tools_String
*/
inline void ToLower( ::std::wstring & v ) {  
  for( size_t i = 0, sz = v.size(); i < sz; i++ )
    v[i] = towlower( v[i] );
}


#ifndef C3D_WINDOWS //_MSC_VER 
//------------------------------------------------------------------------------
/** \brief \ru Перевести символы в верхний регистр.
           \en Convert symbols to upper case. \~
*/
inline
TCHAR * CharUpper( TCHAR * s ) {
  for( TCHAR * ch(s); *ch != '\0'; ++ch ) {
    #ifdef _UNICODE
      *ch = towupper( *ch );
    #else
      *ch = toupper( *ch );
    #endif
  }
  return s;
}
#endif // C3D_WINDOWS


} // namespace C3D


#endif  // __TOOL_CSTRING_H
