////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Общий интерфейс для работы с битовыми флагами.
         \en Common interface for bit-flags treatment. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __NAME_FLAGS_H
#define __NAME_FLAGS_H


#include <io_base.h>
#include <math_define.h>


//------------------------------------------------------------------------------
/** \brief \ru Предоставляет общий интерфейс для работы с битовыми флагами.
           \en Provides the common interface for bit-flags treatment. \~
  \details \ru Это почти копия ItFlags, но весь inline и без виртуальных функций. \n
           \en It is almost the copy of 'ItFlags' but is 'inline' and without virtual functions. \n \~
  \ingroup Base_Items
*/
// ---
class MATH_CLASS MbFlags {
  uint8 flags;

public:
  /// \ru Конструктор. \en Constructor. 
  MbFlags( uint8 f = 0 ) : flags(f) {}
  /// \ru Установить битовые флаги. \en Set the bit-flags. 
  void SetFlagValue( uint8 mask, bool set = true ) { set ? flags |= mask : flags &= (uint8)(~mask); }
  /// \ru Получить битовые флаги. \en Get the bit-flags. 
  uint8 GetFlagValue( uint8 mask = 0xff ) const { return flags & mask; }
  /// \ru Установить все битовые флаги. \en Set all bit-flags. 
  void InitFlags( uint8 f = 0 ) { flags = f; }
  /// \ru Получить все битовые флаги. \en Get all bit-flags. 
  uint8 GetFlags() const { return flags; }

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbFlags, MATH_FUNC_EX )
};


#endif // __NAME_FLAGS_H 
