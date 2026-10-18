////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Вспомогательные функции по работе со строками.
         \en Utility functions for working with strings. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TOOL_STRING_UTIL_H
#define __TOOL_STRING_UTIL_H


#include <system_types.h>
#include <string>

#ifdef C3D_WINDOWS // _MSC_VER 
#include <clocale>
 #include <stdlib.h>
#else // C3D_WINDOWS
 #include <cstdlib>
#endif // C3D_WINDOWS

#ifdef __MOBILE_VERSION__
#include <UtfConverter.h>
#endif // __MOBILE_VERSION__

// The keyword -D_CRT_SECURE_NO_WARNINGS added. Pragma below were disable deprecated warnings.
#if defined (C3D_WINDOWS) && !defined(ALL_WARNINGS)  // _MSC_VER // Set warnings level
#pragma warning(push)          // Preserve current state of warning settings
#pragma warning(disable: 4996)  // This function or variable may be unsafe. Consider using strcpy_s instead.
#endif

#ifndef C3D_WINDOWS // _MSC_VER
inline const char* strret( const char* str ) { return str; }  /// \ru Возврат CHAR-строки. \en Return a CHAR-string.   \~ \ingroup Base_Tools_String
#endif //C3D_WINDOWS

//------------------------------------------------------------------------------
/** \brief \ru Дублировать CHAR-строку.
           \en Duplicate a CHAR-string \~
  \details \ru Дублировать CHAR-строку, удалять по delete[]. \n
           \en Duplicate a CHAR-string, delete by the delete[] operator. \n \~
  \ingroup Base_Tools_String
*/
//---
inline char * strnewdup( const char * str, size_t minLen = 0 )
{
  if ( !str )
    return nullptr;

  size_t len = strlen( str );

  if ( len < minLen ) 
    len = minLen;

  return strcpy( new char[len + 1], str );
}

//------------------------------------------------------------------------------
/** \brief \ru Дублировать WCHAR-строку.
           \en Duplicate a WCHAR-string. \~
  \details \ru Дублировать WCHAR-строку, удалять по delete[]. \n
           \en Duplicate a WCHAR-string, delete by delete[] operator. \n \~
  \ingroup Base_Tools_String
*/
// ---
inline wchar_t * wcsnewdup( const wchar_t * str, size_t minLen = 0 )
{
  if ( !str )
    return nullptr;

  size_t len = wcslen( str );

  if ( len < minLen ) 
    len = minLen;

  return wcscpy( new wchar_t[len + 1], str ); // SKIP_SA
}

//------------------------------------------------------------------------------
/** \brief \ru Конвертировать CHAR в WCHAR строку.
           \en Convert CHAR to WCHAR. \~
  \details \ru Конвертировать CHAR в WCHAR строку, удалять по delete[]. \n
           \en Convert CHAR-string to WCHAR-string, delete by the delete[] operator. \n \~
  \ingroup Base_Tools_String
*/
//---
inline wchar_t * mbsnewwcs( const char * str )
{
  wchar_t * res = nullptr;

  if ( str )
  {
#ifndef __MOBILE_VERSION__
#ifdef C3D_WINDOWS // _MSC_VER 
    size_t n = mbstowcs( nullptr, str, 0 );
#else // C3D_WINDOWS
    size_t n = std::mbstowcs( nullptr, str, 0 );
#endif // C3D_WINDOWS
    
    if ( n != NSIZE )
    {
      n++;
      res = new wchar_t[n];
      // Note.
      // In most implementations, mbstowcs updates a global static object of type mbstate_t as it processes
      // through the string, and cannot be called simultaneously by two threads, mbsrtowcs should be used in such cases.
      // POSIX specifies a common extension: if dst is a null pointer, this function returns the number of wide characters
      // that would be written to dst, if converted. Similar behavior is standard for mbstowcs_s and for mbsrtowcs. 
#ifdef C3D_WINDOWS // _MSC_VER 
      mbstowcs( res, str, n );
#else // C3D_WINDOWS
      std::mbstowcs( res, str, n );
#endif // C3D_WINDOWS 
    }
    // \ru ID K12 Ошибка 42704 \en ID K12 Error 42704 
    else // \ru переводим по одному символу, вместо непереведенных ставим "?" \en convert by word and replace not converted words by the symbol "?" 
    {
      size_t len = strlen( str );
      res = new wchar_t[len + 1];
      memset( res, 0, (len + 1)*sizeof(wchar_t) );

      for ( size_t i = 0; i < len; i++ )
      {
#ifdef C3D_WINDOWS // \ru Актуально только под Windows см. Ошибка 42704 \en It is actual only for Windows Error 42704 
        wchar_t c;
        if ( mbtowc(&c, &str[i], 1) == sizeof(char) ) // \ru только для однобайтных символов! \en only for single-byte symbols 
          res[i] = c;
        else
#endif // C3D_WINDOWS
          res[i] = '?';
      }
    }
#else // __MOBILE_VERSION__
    res = cp1251_to_WChar(str);
    //wchar_t unChar = L'?';
    //size_t len = strlen( str );
    //res = new wchar_t[len + 1];
    //memset( res, 0, (len + 1)*sizeof(wchar_t) );
    //for ( size_t i = 0; i < len; i++ )
    //{
    //  if ( str[i] < 128 )
    //    memcpy( res+i, str+i, 1 );
    //  else
    //    memcpy( res+i, &unChar, 1 );
    //}
    //std::mbstowcs( res, str, len );
#endif // __MOBILE_VERSION__
  }

  return res;
}

//------------------------------------------------------------------------------
/** \brief \ru Конвертировать WCHAR в CHAR строку.
           \en Convert WCHAR-string to CHAR-string. \~
  \details \ru Конвертировать WCHAR в CHAR строку, удалять по delete[]. \n
           \en Convert WCHAR-string to CHAR-string, delete by the delete[] operator. \n \~
  \ingroup Base_Tools_String
*/
//---
inline char * wcsnewmbs( const wchar_t * str ) 
{
  char * res = nullptr;

  if ( str )
  {
    // \ru один WCHAR может занять более одного CHAR! \en one WCHAR may replace more than one CHAR! 
    size_t n = std::wcstombs( nullptr, str, 0 );

    if ( n != NSIZE )
    {
      n++;
      res = new char[n];
      std::wcstombs( res, str, n );
    }
    else // \ru переводим по одному символу, вместо непереведенных ставим "?" \en convert by word and replace not converted words by the symbol "?" 
    {
#ifdef C3D_WINDOWS
      int category = LC_ALL;
      char * currentLocaleStr = std::setlocale( LC_ALL, nullptr );
      std::string currentLocale{ currentLocaleStr != nullptr ? currentLocaleStr : std::string() };
      // \en The wcstombs relies on the current locale settings to perform the conversion correctly.
      // \en Setting LC_ALL to an empty string "" configures the program to use the locale defined in the user's environment.
      std::setlocale( category, "" );
#endif

      bool shift = std::wctomb( nullptr, 0 ); // \en Reset the conversion state.
      size_t len = wcslen( str );
      std::string resStr;

      if ( !shift )
        for ( size_t i = 0; i < len; i++ )
        {
          std::string mb( MB_CUR_MAX, '\0' );
          int ret = std::wctomb( &mb[0], str[i] );
          resStr += ret > 0 ? mb : "?";
        }
      else
        for ( size_t i = 0; i < len; i++ )
          resStr += "?";

      n = resStr.size();
      res = new char[n + 1];
      ::memcpy( res, resStr.c_str(), n );
      res[n] = 0;

#ifdef C3D_WINDOWS
      if ( !currentLocale.empty() )
        std::setlocale( category, currentLocale.c_str() );  // \ru Вернуть предыдущую локаль. \en Return to the previous locale.
#endif
    }

  }

  return res;
}


//------------------------------------------------------------------------------
/** \brief \ru Буфер CHAR-строки.
           \en A buffer of a CHAR-string \~
  \details \ru Буфер CHAR-строки. \n
           \en A buffer of a CHAR-string \n \~
  \ingroup Base_Tools_String
*/
// ---
class strbuf
{
  char * buf; ///< \ru Буфер строки. \en A buffer of a string. 

public:
  /// \ru Конструктор дублирования. \en Constructor of duplicating. 
  strbuf( const char * str ) {
    buf = strnewdup( str );
  }
  /// \ru Конструктор конвертирования из WCHAR. \en Constructor of converting from WCHAR. 
  strbuf( const wchar_t * str ) {
    buf = wcsnewmbs( str );
  }
  /// \ru Конструктор резервирования. \en Constructor of reservation. 
  strbuf( size_t len ) {
    buf = new char[len];
    *buf = 0;
  }
  /// \ru Деструктор. \en Destructor. 
  ~strbuf() { 
    delete [] buf; 
  }
  /// \ru Оператор доступа. \en An access operator. 
  operator const char * () const { return buf; }
  /// \ru Оператор доступа. \en An access operator. 
  operator       char * ()       { return buf; }

  /// \ru Оператор присваивания. \en Assignment operator. 
  strbuf & operator = ( const wchar_t * str ) {
    delete[] buf;
    buf = wcsnewmbs( str );
    return *this;
  }
  /// \ru Оператор присваивания. \en Assignment operator. 
  strbuf & operator = ( const char * str ) {
    delete[] buf;
    buf = strnewdup( str );
    return *this;
  }
  /// \ru Оператор присваивания. \en Assignment operator. 
  strbuf & operator = ( const strbuf & wb ) {
    if ( &wb != this ) {
      delete[] buf;
      buf = strnewdup( wb.buf );
    }
    return *this;
  }


};


//------------------------------------------------------------------------------
/** \brief \ru Буфер WCHAR-строки.
           \en A buffer of a WCHAR-string. \~
  \details \ru Буфер WCHAR-строки. \n
           \en A buffer of a WCHAR-string. \n \~
  \ingroup Base_Tools_String
*/
// ---
class wcsbuf
{
  wchar_t * buf; ///< \ru Буфер строки. \en A buffer of a string. 

public:
  /// \ru Конструктор дублирования. \en Constructor of duplicating. 
  wcsbuf( const wchar_t * str ) {
    buf = wcsnewdup( str );
  }
  /// \ru Конструктор конвертирования из CHAR. \en Constructor of converting from CHAR. 
  wcsbuf( const char * str ) {
    buf = mbsnewwcs( str );
  }
  /// \ru Конструктор резервирования. \en Constructor of reservation. 
  wcsbuf( size_t len ) {
    buf = new wchar_t[len];
    *buf = 0;
  }
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  wcsbuf() : buf( nullptr ) {}

  /// \ru Деструктор. \en Destructor. 
  ~wcsbuf() {
    delete [] buf; 
  }
  /// \ru Оператор доступа. \en An access operator. 
  operator const wchar_t * () const { return buf; }
  /// \ru Оператор доступа. \en An access operator. 
  operator       wchar_t * ()       { return buf; }

  /// \ru Оператор присваивания. \en Assignment operator. 
  wcsbuf & operator = ( const wchar_t * str ) {
    delete [] buf; 
    buf = wcsnewdup( str );
    return *this;
  }
  /// \ru Оператор присваивания. \en Assignment operator. 
  wcsbuf & operator = ( const char * str ) {
    delete[] buf;
    buf = mbsnewwcs( str );
    return *this;
  }
  /// \ru Оператор присваивания. \en Assignment operator. 
  wcsbuf & operator = ( const wcsbuf & wb ) {
    if ( &wb != this ) {
      delete[] buf;
      buf = wcsnewdup( wb.buf );
    }
    return *this;
  }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// \ru Конвертирование CHAR, WCHAR и TCHAR-строк \en Converting of CHAR-, WCHAR- and TCHAR-strings  
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef C3D_WINDOWS // _MSC_VER 

#ifdef _UNICODE

#define _tcsbuf        wcsbuf          ///< \ru Буфер TCHAR-строки \en A buffer of a TCHAR-string 
#define _tcsnewdup     wcsnewdup       ///< \ru Дублировать TCHAR-строку \en Duplicate a TCHAR-string 

inline const wchar_t * wcsret( const wchar_t * str ) { return str; }  /// \ru Возврат WCHAR-строки. \en Return a WCHAR-string   \~ \ingroup Base_Tools_String

// \ru Конвертирование CHAR, WCHAR и TCHAR-строк НА СТЕКЕ! \en Converting of CHAR-, WCHAR- and TCHAR-strings ON STACK! 
#define _tcs2str strbuf // \ru TCHAR в CHAR  те WCHAR в CHAR \en TCHAR to CHAR i.e. WCHAR to CHAR 
#define _str2tcs wcsbuf // \ru CHAR  в TCHAR те CHAR  в WCHAR \en CHAR  to TCHAR i.e. CHAR to WCHAR 
#define _tcs2wcs wcsret // \ru TCHAR в WCHAR те WCHAR в WCHAR \en TCHAR to WCHAR i.e. WCHAR to WCHAR 
#define _wcs2tcs wcsret // \ru WCHAR в TCHAR те WCHAR в WCHAR \en TCHAR to WCHAR i.e. WCHAR to WCHAR 

#define _tcsNstr wcsnewmbs // \ru TCHAR в CHAR \en TCHAR to CHAR   
#define _strNtcs mbsnewwcs // \ru CHAR  в TCHAR \en CHAR to TCHAR  

#else // _UNICODE

#define _tcsbuf        strbuf
#define _tcsnewdup     strnewdup       ///< \ru Дублировать TCHAR-строку \en Duplicate a TCHAR-string 

inline const char    * strret( const char    * str ) { return str; }  /// \ru Возврат CHAR-строки. \en Return a CHAR-string.   \~ \ingroup Base_Tools_String

// \ru Конвертирование CHAR, WCHAR и TCHAR-строк НА СТЕКЕ! \en Converting of CHAR-, WCHAR- and TCHAR-strings ON STACK! 
#define _tcs2str strret // \ru TCHAR в CHAR  те CHAR  в CHAR \en TCHAR to CHAR  i.e. CHAR  to CHAR  
#define _str2tcs strret // \ru CHAR  в TCHAR те CHAR  в CHAR \en CHAR  to TCHAR i.e. CHAR  to CHAR 
#define _tcs2wcs wcsbuf // \ru TCHAR в WCHAR те CHAR  в WCHAR \en TCHAR to WCHAR i.e. CHAR  to WCHAR 
#define _wcs2tcs strbuf // \ru WCHAR в TCHAR те WCHAR в CHAR \en WCHAR to TCHAR i.e. WCHAR to CHAR 

#define _tcsNstr strnewdup // \ru TCHAR в CHAR \en TCHAR to CHAR   
#define _strNtcs strnewdup // \ru CHAR  в TCHAR \en CHAR to TCHAR  

#endif // _UNICODE

#else // C3D_WINDOWS

// \ru Linux: заменяем реализацию string на стандартную (std::string); \en Linux: replace implementation of string by the standard (std::string);  
// \ru все вызовы внутри математики переделаны под стандартную реализацию строк (std::string) \en all calls inside the mathematics reimplemented for the standard implementation of strings (std::string) 
// \ru Под Linux определение _UNICODE не должно влиять на компиляцию кода \en In Linux the definition _UNICODE should not influence the code compiling 

#ifdef _UNICODE

#define _tcsbuf        wcsbuf
#define _tcsnewdup     wcsnewdup       ///< \ru Дублировать TCHAR-строку \en Duplicate a TCHAR-string

//inline const char* strret( const char* str ) { return str; }  /// \ru Возврат CHAR-строки. \en Returns CHAR-string.   \~ \ingroup Base_Tools_String

// \ru Конвертирование CHAR, WCHAR и TCHAR-строк НА СТЕКЕ! \en Converting of CHAR-, WCHAR- and TCHAR-strings ON STACK!
#define _tcs2str strret // \ru TCHAR в CHAR  те CHAR  в CHAR \en TCHAR to CHAR  i.e. CHAR  to CHAR
#define _str2tcs strret // \ru CHAR  в TCHAR те CHAR  в CHAR \en CHAR  to TCHAR i.e. CHAR  to CHAR
#define _tcs2wcs wcsbuf // \ru TCHAR в WCHAR те CHAR  в WCHAR \en TCHAR to WCHAR i.e. CHAR  to WCHAR
#define _wcs2tcs strbuf // \ru WCHAR в TCHAR те WCHAR в CHAR \en WCHAR to TCHAR i.e. WCHAR to CHAR

#define _tcsNstr wcsnewmbs // \ru TCHAR в CHAR \en TCHAR to CHAR
#define _strNtcs mbsnewwcs // \ru CHAR  в TCHAR \en CHAR to TCHAR

#else // _UNICODE

#define _tcsbuf        strbuf
#define _tcsnewdup     strnewdup       ///< \ru Дублировать TCHAR-строку \en Duplicate a TCHAR-string 

//inline const char* strret( const char* str ) { return str; }  /// \ru Возврат CHAR-строки. \en Returns CHAR-string.   \~ \ingroup Base_Tools_String

// \ru Конвертирование CHAR, WCHAR и TCHAR-строк НА СТЕКЕ! \en Converting of CHAR-, WCHAR- and TCHAR-strings ON STACK! 
#define _tcs2str strret // \ru TCHAR в CHAR  те CHAR  в CHAR \en TCHAR to CHAR  i.e. CHAR  to CHAR  
#define _str2tcs strret // \ru CHAR  в TCHAR те CHAR  в CHAR \en CHAR  to TCHAR i.e. CHAR  to CHAR 
#define _tcs2wcs wcsbuf // \ru TCHAR в WCHAR те CHAR  в WCHAR \en TCHAR to WCHAR i.e. CHAR  to WCHAR 
#define _wcs2tcs strbuf // \ru WCHAR в TCHAR те WCHAR в CHAR \en WCHAR to TCHAR i.e. WCHAR to CHAR 

#define _tcsNstr strnewdup // \ru TCHAR в CHAR \en TCHAR to CHAR   
#define _strNtcs strnewdup // \ru CHAR  в TCHAR \en CHAR to TCHAR  

#endif // _UNICODE

#endif // C3D_WINDOWS


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// \ru Конвертирование UTF-16(ISO-10646-UTF-16 encoded) и UCS-4(ISO-10646-UCS-4 encoded) -строк \en Converting of UTF-16(ISO-10646-UTF-16 encoded) and UCS-4(ISO-10646-UCS-4 encoded) -strings  
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
/** \brief \ru Конвертировать UTF-16 в UCS-4 строку.
           \en Convert from UTF-16 to UCS-4 string. \~
  \details \ru Конвертировать UTF-16 в UCS-4 строку, удалять по delete[]. \n
           \en Convert from UTF-16 to UCS-4 string, delete with delete[] operator. \n \~
  \ingroup Base_Tools_String
*/
//---
inline uint32* Utf16ToUcs4( uint16* source, size_t* calculateCountSymbol = nullptr )
{
  size_t count = 0; // \ru количество символов в строке \en a number of symbols in string 
  uint32 * outBuf = nullptr;
  if ( source )
  {
    while (source[count] != 0)
      ++count;

    outBuf = new uint32[count + 1];  // \ru буфер для выдачи наружу \en a buffer for external using 
    memset( outBuf, 0, sizeof(uint32/*outBuf*/)*(count + 1) );
    for ( size_t i = 0; i < count; ++i )
      outBuf[i] = MkUint32( source[i], 0 );
  }  

  if ( calculateCountSymbol ) 
    *calculateCountSymbol = count;

  return outBuf;
}

//------------------------------------------------------------------------------
/** \brief \ru Конвертировать UCS-4 в UTF-16 строку.
           \en Convert from UCS-4 to UTF-16 string. \~
  \details \ru Конвертировать UCS-4 в UTF-16 строку, удалять по delete[]. \n
           \en Convert from UCS-4 to UTF-16 string, delete with delete[] operator. \n \~
  \ingroup Base_Tools_String
*/
//---
inline uint16* Ucs4ToUtf16( uint32* source, size_t* calculateCountSymbol = nullptr )
{
  size_t count = 0; // \ru количество символов в строке \en a number of symbols in string 
  uint16 * outBuf = nullptr;
  if ( source )
  {
    while (source[count] != 0)
      ++count;

    outBuf = new uint16[count + 1];  // \ru буфер для выдачи наружу \en a buffer for external using
#ifndef __MOBILE_VERSION__
    memset( outBuf, 0, sizeof(uint16/*BUG_82447 outBuf*/)*(count + 1) );
    for ( size_t i = 0; i < count; ++i )
      if ( HiUint16( source[i] ) == 0 )
        outBuf[i] = LoUint16( source[i] );
      else
        outBuf[i] = static_cast<uint16>( '?' );
#else // __MOBILE_VERSION__
    for ( size_t i = 0; i < count; ++i )
      outBuf[i] = LoUint16( source[i] );
    outBuf[count] = uint16(0);
#endif // __MOBILE_VERSION__
  }  

  if ( calculateCountSymbol ) 
    *calculateCountSymbol = count;

  return outBuf;
}

#if defined (C3D_WINDOWS) && !defined(ALL_WARNINGS) // _MSC_VER // Set warnings level
#pragma warning(pop) // Restore state of warning settings
#endif

#endif // __TOOL_STRING_UTIL_H
