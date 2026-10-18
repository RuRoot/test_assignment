////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Сведения о версии плагина.
         \en Information about plugin version. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CONV_PLUGIN_VERSION_INFO_H
#define __CONV_PLUGIN_VERSION_INFO_H

#include <string>
#include "tool_cstring.h"

namespace c3d {

/**
  \addtogroup Exchange_Interface
  \{
*/

//------------------------------------------------------------------------------
/** \brief  \ru Сведения о версии плагина.
\en Information about plugin version.\~
\ingroup Data_Interface
*/
// ---
struct PluginVersionInfo {
  c3d::path_string pathToPlugin;
  std::string version;
};

}

#endif
