////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Фасет.
         \en Facet. \~

*/
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// specify_facet.h
//
// \ru Реализация класса specify_facet для поддержки идиомы RAII (Resource \en Implementation of the class specify_facet for the support of the idiom RAII (Resource  
// \ru Acquisition Is Initialization) при задании фасетов потоку. \en Acquisition Is Initialization) when defining the facets for a stream.  
// \ru В конструкторе задаём фасет, в деструкторе восстанавливаем локаль \en Set the facet in constructor, restore the locale in destructor 
//
///////////////////////////////////////////////////////////////////////////////


#ifndef __TEMPL_SPECIFY_FACET_H
#define __TEMPL_SPECIFY_FACET_H


#include <locale>


namespace c3d // namespace C3D
{

///////////////////////////////////////////////////////////////////////////////
//
// specify_facet
// \ru Поддержка RAII (Resource Acquisition Is Initialization) для задания \en Support of RAII (Resource Acquisition Is Initialization) for the setting of  
// \ru фасетов потоку. В конструкторе задаём новый фасет, в деструкторе \en facets for a stream. Set a new facet in constructor, in destructor 
// \ru восстанавливаем поведение \en restore the behaviour 
//
// \ru Использование: \en The usage: 
//   std::wofstream file;
//
//   sys_io::specify_facet<std::wofstream, my_codecvt> sp_facet( file );
//   file.open( "file_name", std::ios_base::out | std::ios_base::binary 
//                                              | std::ios_base::trunc );
//
// \ru Параметры шаблона: \en Template parameters: 
// \ru StreamType - тип потока, FacetType - тип фасета \en StreamType - a stream type, FacetType - a facet type 
//
///////////////////////////////////////////////////////////////////////////////
template <typename StreamType, typename FacetType>
class specify_facet
{
private:
  typedef StreamType _StType;  // \ru Тип потока \en A stream type 
  typedef FacetType  _FcType;  // \ru Тип фасета \en A facet type 
  
  _StType &          _stream;  // \ru Ссылка на поток, которым управляем \en A reference to the controlled stream 
  std::locale        _locale;  // \ru Старая локаль, которую надо будет восстанавливать \en An old locale to be restored 
                    
public:
  specify_facet( _StType & stream )
    : _stream( stream )
  {
    // \ru Код разный в зависимости от компилятора. С переходом на VS2005 убрать \en The code is different, according to the compiler. Remove it after the moving to VS2005 
//OV_x64 #if defined(_MSC_VER) && _MSC_VER < 1400
//OV_x64   _locale = _stream.imbue( _stream.getloc()._Addfac(new _FcType, _FcType::id, _FcType::_Getcat()) );
//OV_x64 #else
    _locale = _stream.imbue( std::locale(_stream.getloc(), new _FcType) );
//OV_x64 #endif
  }

  ~specify_facet()
  {
    // \ru Восстанавливаем старую локаль \en Restore the old locale 
    _stream.imbue( _locale );
  }
};

} // namespace C3D


#endif // __TEMPL_SPECIFY_FACET_H
