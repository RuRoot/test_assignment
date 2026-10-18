////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Сведения о поддерживаемом формате геометрических моделей.
         \en Information about supported 3D model format. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CONV_FORMAT_INFO_H
#define __CONV_FORMAT_INFO_H

#include <tool_cstring.h>
#include <vector>

namespace c3d {

/**
  \addtogroup Exchange_Interface
  \{
*/

//------------------------------------------------------------------------------
/** \brief  \ru Сведения о поддерживаемом формате геометрических моделей.
\en Information about supported 3D model format.\~
\ingroup Data_Interface
*/
// ---
class CONV_CLASS FormatInfo final {
public:
  // \ru Конструктор. \en Constructor.
  FormatInfo(string_t const && title, std::vector<string_t> const && filters);

  // \ru Получить название формата. \en Get format title.
  string_t const & GetTitle() const;

  // \ru Получить список фильтров имён файлов. \en Get list of filename filters.
  std::vector<string_t> const & GetFilters() const;

private:
  string_t title_;  // \ru Название формата. \en Initialize buffer.
  std::vector<string_t> filters_;  // \ru Список фильтров имён файлов. \en List of filename filters.
};

}

#endif