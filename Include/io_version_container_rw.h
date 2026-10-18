////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Контейнер версий. Чтение/запись.
         \en Container of versions. Reading/writing. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __IO_VERSION_CONTAINER_RW_H
#define __IO_VERSION_CONTAINER_RW_H

#include <io_version_container.h>
#include <templ_s_array_rw.h>


//-----------------------------------------------------------------------------
/// \ru Оператор чтения контейнера версий. \en Operator of version container reading. \~ \ingroup Base_Tools_IO
// ---
MATH_FUNC (reader &) operator >> ( reader &, VersionContainer & );


//-----------------------------------------------------------------------------
/// \ru Оператор записи контейнера версий. \en Operator of version container writing. \~ \ingroup Base_Tools_IO
// ---
MATH_FUNC (writer &) operator << ( writer &, const VersionContainer & );


#endif //__IO_VERSION_CONTAINER_RW_H
