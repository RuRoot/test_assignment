////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Макросы сериализации, подавление предупреждений, контроль памяти.
         \en Macros of serialization, warnings suppression, memory control. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __IO_DEFINES_H
#define __IO_DEFINES_H


#include <math_define.h>


class reader;
class writer;

//------------------------------------------------------------------------------
// \ru Объекты, для которых при записи и чтении точно известен тип \en Objects which type is exactly known while reading and writing 
// \ru могут записываться в поток и читаться из потока с помощью \en can be written to the stream and read from the stream using 
// \ru операторов << и >> \en operators << and >> 
//---
#define KNOWN_OBJECTS_RW_REF_OPERATORS(Class)                                        \
  friend reader & CALL_DECLARATION operator >> ( reader & in,        Class & ref );  \
  friend writer & CALL_DECLARATION operator << ( writer & out, const Class & ref );  \
  friend writer & CALL_DECLARATION operator << ( writer & out,       Class & ref ) { return operator << ( out, static_cast<const Class &>(ref) ); }

#define KNOWN_OBJECTS_RW_PTR_OPERATORS(Class)                                        \
  friend reader & CALL_DECLARATION operator >> ( reader & in,        Class *& ptr ); \
  friend writer & CALL_DECLARATION operator << ( writer & out, const Class * ptr );  \
  friend writer & CALL_DECLARATION operator << ( writer & out,       Class * ptr ) { return operator << ( out, static_cast<const Class *>(ptr) ); }

// \ru тоже для экспорта/импорта \en for export/import too 
// \ru DLLFUNC -> __declspec( dllexport ) или __declspec( dllimport ) объявляется специально \en DLLFUNC -> __declspec( dllexport ) or __declspec( dllimport ) are specially declared 
// \ru для этих дефайнов в файлах типа ????_def.h (см. MATH_FUNC_EX выше) \en for this definitions in files like ?????_def.h (see MATH_FUNC_EX above) 
#define KNOWN_OBJECTS_RW_REF_OPERATORS_EX(Class, DLLFUNC)                                    \
  friend DLLFUNC reader & CALL_DECLARATION operator >> ( reader & in,        Class & ref );  \
  friend DLLFUNC writer & CALL_DECLARATION operator << ( writer & out, const Class & ref );  \
  friend DLLFUNC writer & CALL_DECLARATION operator << ( writer & out,       Class & ref ) { return operator << ( out, static_cast<const Class &>(ref) ); }

#define KNOWN_OBJECTS_RW_PTR_OPERATORS_EX(Class, DLLFUNC)                                    \
  friend DLLFUNC reader & CALL_DECLARATION operator >> ( reader & in,        Class *& ptr ); \
  friend DLLFUNC writer & CALL_DECLARATION operator << ( writer & out, const Class * ptr );  \
  friend DLLFUNC writer & CALL_DECLARATION operator << ( writer & out,       Class * ptr ) { return operator << ( out, static_cast<const Class *>(ptr) ); }


#endif // __IO_DEFINES_H
