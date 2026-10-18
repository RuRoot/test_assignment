////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Глобально уникальный идентификатор.
         \en Global unique identifier. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef _TOOL_UUID_H_
#define _TOOL_UUID_H_

#include <memory.h>
#include <math_define.h>
#include <templ_three_states.h>
#include <string>
#include <cstring>

class reader;
class writer;


constexpr uint8 uuidSize = 16;


//------------------------------------------------------------------------------
/** \brief \ru Глобально уникальный идентификатор.
           \en Global unique identifier. \~
  \details \ru Глобально уникальный идентификатор - используется как идентификатор 
           типа пользовательского атрибута.
           \en Global unique identifier - it is used as an identifier 
           of a type of user attribute. \~
  \ingroup Model_Attributes
 */
struct MbUuid
{
protected:
  uint8 data[uuidSize];
private:
  mutable ThreeStates isEmpty;

public:
  typedef uint8 *       iterator;
  typedef uint8 const * const_iterator;
public:
  MbUuid() : isEmpty( ts_positive ) { ::memset( data, 0, uuidSize ); }
  MbUuid( const MbUuid & id ) : isEmpty( id.isEmpty ) { ::memcpy( data, id.data, uuidSize ); }
public:
  iterator        begin()        { isEmpty = ts_neutral; return data; }
  iterator        end()          { isEmpty = ts_neutral; return data + uuidSize; }

  const_iterator  cbegin() const { return data; }
  const_iterator  cend()   const { return data + uuidSize; }

  size_t          size()   const { return uuidSize; }

  bool is_nil() const
  {
    if ( isEmpty == ts_neutral ) {
      isEmpty = ts_positive;
      for ( uint8 i = 0; i < uuidSize; i++ ) {
        if ( data[i] != 0U ) {
          isEmpty = ts_negative;
          break;
        }
      }
    }
    return (isEmpty == ts_positive);
  }
  void swap( MbUuid & id ) 
  {
    uint8 temp[16];
    ::memcpy( temp,    data,    uuidSize );
    ::memcpy( data,    id.data, uuidSize );
    ::memcpy( id.data, temp,    uuidSize );
    std::swap( isEmpty, id.isEmpty );
  }

public:
  MbUuid &  operator =  ( const MbUuid & id )
  {
    ::memcpy( data, id.data, uuidSize );
    isEmpty = id.isEmpty;
    return *this;
  }
  bool      operator == ( const MbUuid & id ) const
  { 
    //return std::equal( cbegin(), cend(), id.cbegin() );
    MbUuid::const_iterator first1 = cbegin(), last1 = cend(), first2 = id.cbegin();
    while ( first1 != last1 )
    {
      if ( !(*first1 == *first2) ) 
        return false;
      ++first1; ++first2;
    }
    return true;
  }
  bool      operator != ( const MbUuid & id ) const { return !(*this == id); }
  bool      operator <  ( const MbUuid & id ) const { return std::lexicographical_compare( cbegin(), cend(), id.cbegin(), id.cend() ); }
  bool      operator >  ( const MbUuid & id ) const { return  (id < *this); }
  bool      operator <= ( const MbUuid & id ) const { return !(id < *this); }
  bool      operator >= ( const MbUuid & id ) const { return !(*this < id); }

public:
  friend struct string_generator;
  friend reader & CALL_DECLARATION operator >> ( reader & in, MbUuid & ref );
  friend writer & CALL_DECLARATION operator << ( writer & out, const MbUuid & ref );
  friend writer & CALL_DECLARATION operator << ( writer & out,       MbUuid & ref ) { return operator << ( out, const_cast<const MbUuid &>(ref) ); }
};


//------------------------------------------------------------------------------
/** \brief \ru Генератор MbUuid из string.
           \en Generator of MbUuid from string. \~
  \details \ru Генератор MbUuid из string. Принимает следующие формы: \n
               0123456789abcdef0123456789abcdef, \n
               01234567-89ab-cdef-0123456789abcdef, \n
               {01234567-89ab-cdef-0123456789abcdef}, \n
               {0123456789abcdef0123456789abcdef}. \n
           \en Generator of MbUuid from string. It accepts the next format: \n
               0123456789abcdef0123456789abcdef, \n
               01234567-89ab-cdef-0123456789abcdef, \n
               {01234567-89ab-cdef-0123456789abcdef}, \n
               {0123456789abcdef0123456789abcdef}. \n
           \~
  \ingroup Model_Attributes
*/
//---
struct string_generator 
{
  template <typename ch, typename char_traits, typename alloc>
  MbUuid operator()( std::basic_string<ch, char_traits, alloc> const & s ) const {
    return operator()( s.begin(), s.end() );
  };

  MbUuid operator()( char const * const s ) const {
    return operator()( s, s+std::strlen(s) );
  }

  MbUuid operator()( wchar_t const * const s ) const {
    return operator()( s, s+std::wcslen(s) );
  }

  template <typename CharIterator>
  MbUuid operator()( CharIterator begin, CharIterator end ) const
  {
    typedef typename std::iterator_traits<CharIterator>::value_type char_type;

    // \ru Проверяем открывающую скобку \en Check an opening parenthesis. 
    char_type c = get_next_char( begin, end );
    bool has_open_brace = is_open_brace(c);
    char_type open_brace_char = c;
    if ( has_open_brace ) {
      c = get_next_char( begin, end );
    }

    bool has_dashes = false;

    MbUuid u;
    bool isEmpty = true;

    int i = 0;
    for ( MbUuid::iterator it_byte = u.begin(); it_byte != u.end(); ++it_byte, ++i ) {
      if ( it_byte != u.begin() ) {
        c = get_next_char( begin, end );
      }
      if ( i == 4 ) {
        if ( is_dash(c) ) {
          c = get_next_char( begin, end );
          has_dashes = true;
        }
      }
      if ( has_dashes ) {
        if ( is_dash(c) ) {
          c = get_next_char( begin, end );
        }
      }

      *it_byte = get_value(c);

      c = get_next_char( begin, end );
      *it_byte <<= 4;
      *it_byte |= get_value(c);
      if ( *it_byte != 0U )
        isEmpty = false;
    }

    // \ru Проверяем закрывающую скобку \en Check a closing parenthesis 
    if ( has_open_brace ) {
      c = get_next_char( begin, end );
      check_close_brace( c, open_brace_char );
    }

    u.isEmpty = isEmpty ? ts_positive : ts_negative;
    return u;
  }

private:
  template <typename CharIterator>
  typename std::iterator_traits<CharIterator>::value_type
    get_next_char( CharIterator & begin, CharIterator end ) const {
      if ( begin == end ) {
        _ASSERT( false );
      }
      return *begin++;
  }

  unsigned char get_value( char c ) const {
    static char const*const digits_begin = "0123456789abcdefABCDEF";
    static char const*const digits_end = digits_begin + 22;

    static unsigned char const values[] =
    { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,10,11,12,13,14,15
    , static_cast<unsigned char>(-1) };

    char const * d = std::find( digits_begin, digits_end, c );
    return values[d - digits_begin];
  }

  unsigned char get_value( wchar_t c ) const {
    static wchar_t const*const digits_begin = L"0123456789abcdefABCDEF";
    static wchar_t const*const digits_end = digits_begin + 22;

    static unsigned char const values[] =
    { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,10,11,12,13,14,15
    , static_cast<unsigned char>(-1) };

    wchar_t const * d = std::find( digits_begin, digits_end, c );
    return values[d - digits_begin];
  }

  bool is_dash( char    c ) const { return c == '-'; }
  bool is_dash( wchar_t c ) const { return c == L'-'; }

  // \ru Возвращаем открывающую скобку \en Return an opening parenthesis. 
  bool is_open_brace( char    c ) const { return (c == '{'); }
  bool is_open_brace( wchar_t c ) const { return (c == L'{'); }

  bool check_close_brace( char c, char open_brace ) const {
    if ( open_brace == '{' && c == '}' )
      return true;
    _ASSERT( false );
    return false;
  }

  bool check_close_brace( wchar_t c, wchar_t open_brace ) const {
    if ( open_brace == L'{' && c == L'}' )
      return true;
    _ASSERT( false );
    return false;
  }
};


struct hash8_generator 
{
  uint8 operator()( const MbUuid & trg ) const 
  {
    static uint8 rand8[256] =  
    {
        1,  14, 110,  25,  97, 174, 132, 119, 138, 170, 125, 118,  27, 233, 140,  51,
       87, 197, 177, 107, 234, 169,  56,  68, 30,    7, 173,  73, 188,  40,  36,  65,
       49, 213, 104, 190,  57, 211, 148, 223,  48, 115,  15,   2,  67, 186, 210,  28,
       12, 181, 103,  70,  22,  58,  75,  78, 183, 167, 238, 157, 124, 147, 172, 144,
      176, 161, 141,  86,  60,  66, 128,  83, 156, 241,  79,  46, 168, 198,  41, 254,
      178,  85, 253, 237, 250, 154, 133,  88,  35, 206,  95, 116, 252, 192,  54, 221,
      102, 218, 255, 240, 82,  106, 158, 201,  61,   3,  89,   9,  42, 155, 159,  93,
      166,  80,  50,  34, 175, 195, 100,  99,  26, 150,  16, 145,   4,  33,   8, 189,
      121,  64,  77,  72, 208, 245, 130, 122, 143,  55, 105, 134,  29, 164, 185, 194,
      193, 239, 101, 242,   5, 171, 126,  11,  74,  59, 137, 228, 108, 191, 232, 139,
        6,  24,  81,  20, 127,  17,  91,  92, 251, 151, 225, 207,  21,  98, 113, 112,
       84, 226,  18, 214, 199, 187,  13,  32,  94, 220, 224, 212, 247, 204, 196,  43,
      249, 236,  45, 244, 111, 182, 153, 136, 129,  90, 217, 202,  19, 165, 231,  71,
      230, 142,  96, 227,  62, 179, 246, 114, 162,  53, 160, 215, 205, 180,  47, 109,
       44,  38,  31, 149, 135,   0, 216,  52,  63,  23,  37,  69,  39, 117, 146, 184,
      163, 200, 222, 235, 248, 243, 219,  10, 152, 131, 123, 229, 203,  76, 120, 209 
    };                 

    uint8 h = 0;
    MbUuid::const_iterator curr_iter = trg.cbegin(), end_iter = trg.cend();
    while ( curr_iter != end_iter )
      h = rand8[h ^ *curr_iter++];    
    return h;    
  }
};


#endif // _TOOL_UUID_H_
