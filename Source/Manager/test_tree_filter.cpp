#include "test_tree_filter.h"
#include "test_variables.h"
#include <tool_cstring.h>
#include <templ_p_array.h>
#include <test.h>
#include <test_manager.h>
#include <model_tree_data.h>
#include <last.h>
#include <test_service.h>

using namespace c3d;

//------------------------------------------------------------------------------

// \ru Обработчик для команд \en Commands handler 
bool ObjectFilterWindow::FilterCommand( uint , uint , bool )
{
  return false;
}

// \ru Обработчик для команд \en Commands handler 
bool ObjectFilterWindow::KeyboardCommand( uint command, bool shift, bool ctrl )
{
  switch ( command ) {
  case VK_RETURN: {
    if ( ctrl || shift ) {
      GetTool().CloseWindow();
    }
  } break;

  case VK_ESCAPE: {
    GetTool().CloseWindow();
  } break;

  default:
    return false;
  }
  return true;
}

// \ru Обработчик для команд \en Commands handler 
void ObjectFilterWindow::PreDestroyCommand( bool )
{
  TestVariables::viewManager->RemoveFilter( this );
}

// \ru Обновить список \en Update the list
void ObjectFilterWindow::UpdateList( size_t pos, const TCHAR* s )
{
  GetTool().SetListText( pos, s );
  GetTool().SetCurSelList( pos );
}

// \ru Показать сообщение о статусе \en Show status message
void ObjectFilterWindow::ShowStatus( const TCHAR* message )
{
  if ( message ) {
    PArray<c3d::string_t> a;
    a.Add( new c3d::string_t(message) );
    GetTool().SetListTexts( a );
    GetTool().SetCurSelList( 0 );
    GetTool().Layout();
    GetTool().SetFocusList();
    GetTool().SetEditText( _T(" ") );
  }
}

// \ru Прочитать объекты по фильтрам // \en Load objects by filters
void ObjectFilterWindow::LoadObjects ( std::vector<c3d::MbItemData>& filters )
{
  std::unique_ptr<const c3d::IModelTree> tree ( m_reader->GetModelTree()->GetFilteredTree ( filters ) );
  size_t num = 0;
  std::set<const c3d::IModelTreeNode*> mnodes;
  if ( tree.get() != nullptr ) {
    const std::vector<const c3d::IModelTreeNode*>& roots = tree->GetRoots();
    std::vector<const c3d::IModelTreeNode*>::const_iterator itr = roots.begin();
    while ( itr != roots.end() ) {
      num += c3d::IModelTree::GetSubtreeSignificantNodes( *itr, mnodes );
      ++itr;
    }
  }
  if ( num == 0 ) {
    ShowStatus( IDS_TITLE_SELECT_MESSAGE_NO );
  }
  else {
    TCHAR message[128];
    _sntprintf ( message, 128, IDS_TITLE_SELECT_MESSAGE_FORMAT, LoUint32(num) );
    ShowStatus ( message );

    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    // read object and add to the model
    std::vector< SPtr<MbItem> > items;
    SetWaitCursor ( true );
    ::ReadModelItemsFromTree( *m_reader, tree.get(), items );
    SetWaitCursor ( false );
    if ( m_reader->state() & io::skippedUnknown )
      MessageBoxEx( RT_FILE_WITH_UNKNOWN_OBJECTS, IDS_TITLE_WRN );
    if ( m_reader->state() & io::verViolation )
      MessageBoxEx( RT_WAR_FILE_VERSION, IDS_TITLE_WRN );
    if ( items.size() ) {
      ShowStatus( IDS_TITLE_SELECT_MESSAGE_DRAW );
      wind->AddObjectsToModel( items );
    }
  }
  GetTool().CloseWindow();
}

//------------------------------------------------------------------------------
// \ru Конструктор // \en Constructor
ObjectTypeWindow::ObjectTypeWindow( reader_ex* reader ) : ObjectFilterWindow( reader ) 
{
  for ( uint16 type = (uint16)st_Item + 1; type < (uint16)st_Item + 11; ++type )
    m_types.push_back( (MbeSpaceType)type );
  CreateTypeWindow();
}

// \ru Выбрать тип объекта, если он еще не выбран, или удалить его из выбранных типов
//     Возвращает true, если тип выбран, и false в противном случае  
// \en Select an object type if it has not been selected before or remove it if it was already selected
//     Return true if the type added or false otherwise
bool ObjectTypeWindow::SelectType( MbeSpaceType type )
{
  std::set<MbeSpaceType>::iterator sel = m_selection.find( type );
  if ( sel != m_selection.end() ) {
    m_selection.erase( sel );
    return false;
  }
  m_selection.insert( type );
  return true;
}

// \ru Обработчик для команд \en Commands handler
bool ObjectTypeWindow::FilterCommand(uint command, uint subCommand, bool )
{
  switch ( command ) {
  case BUTTON_APPLY: {
    LoadObjects();
  } break;
  case BUTTON_SELECT_TYPE: {
    size_t currpos = GetTool().GetCurSelList();
    if ( currpos < m_types.size() ) {
      bool added = SelectType( m_types[currpos] );
      c3d::string_t s = FormatTypeString ( m_types[currpos], added );
      UpdateList ( currpos, s.c_str() );
    }
  } break;
  case H_LIST: {
    switch ( subCommand ) {
    case LBN_SELCHANGE:
      break;
    case LBN_DBLCLK:
      break;
    }
  } break;
  default:
    return false;
  }
  return true;
}

// \ru Прочитать выбранные объекты \en Load selected objects
void ObjectTypeWindow::LoadObjects()
{
    std::vector<MbItemData> filters;
    for ( std::set<MbeSpaceType>::const_iterator itr = m_selection.begin(); itr != m_selection.end(); ++itr ) {
      MbItemData data;
      data.type = *itr;
      filters.push_back( data );
    }
  ObjectFilterWindow::LoadObjects ( filters );
}

// \ru Создать диалог фильтров по типу \en Create filter dialog
void ObjectTypeWindow::CreateTypeWindow()
{
#ifndef __USE_QT__
  HWND hwin = 0;
  HWND hlist = 0;
  int start, listY;
  if ( CreateTypeClient(hwin, hlist, listY, start, IDS_TITLE_SELECT_BY_TYPE, (int)m_types.size()) )
    m_tool = CreateTypeTool( hwin, hlist, start, listY );
#else // __USE_QT__ 
  // TODO: implement it
#endif // __USE_QT__

  // populate list
  PArray<c3d::string_t> a;
  for ( std::vector<MbeSpaceType>::const_iterator type = m_types.begin(); type != m_types.end(); ++type ) {
    std::set<MbeSpaceType>::const_iterator sel = m_selection.find( *type );
    bool added = sel != m_selection.end();
    a.Add( new c3d::string_t(FormatTypeString(*type, added)) );
  }

  GetTool().SetListTexts( a );
  GetTool().SetCurSelList( 0 );
}

//------------------------------------------------------------------------------
// \ru Конструктор // \en Constructor
ObjectGabaritWindow::ObjectGabaritWindow( reader_ex* reader ) :
  ObjectFilterWindow( reader )
{
  m_gabarit.pmin.SetZero(); m_gabarit.pmax.SetZero();
  if ( const c3d::IModelTree* tree = reader->GetModelTree() ) {
    const std::vector<const IModelTreeNode*>& roots = tree->GetRoots();
    for ( std::vector<const IModelTreeNode*>::const_iterator itr = roots.begin(); itr != roots.end(); ++itr ) {
      m_gabarit.Include ( (*itr)->GetData().gabarit.pmin );
      m_gabarit.Include ( (*itr)->GetData().gabarit.pmax );
    }
  }
  CreateGabaritWindow();
}

// \ru Создать диалог фильтров по габариту \en Create gabarit dialog
void ObjectGabaritWindow::CreateGabaritWindow()
{
#ifndef __USE_QT__
  HWND hwin = 0, hedit = 0, hlist = 0;
  int start, listY;
  int count = GetDataCount();
  if ( CreateGabaritClient(hwin, hedit, hlist, start, listY, IDS_TITLE_SELECT_BY_GABARIT, count) )
    m_tool = CreateGabaritTool ( hwin, hedit, hlist, start, listY );
#else // __USE_QT__ 
  // TODO: implement it
#endif // __USE_QT__

  // populate list
  TCHAR s1[STRINGLENGTH];
  PArray<c3d::string_t> a1;
  _sntprintf( s1, STRINGLENGTH, _T("%s"), IDS_TITLE_OBJECT_GABARIT );
  a1.Add( new c3d::string_t(s1) );

  _sntprintf( s1, STRINGLENGTH, _T("  %s"), _T("pmin") );
  a1.Add( new c3d::string_t(s1) );
 
  {// pmin.x
    ValueFormatter formatter( _T("x"), &m_gabarit.pmin.x );
    m_values[2] = formatter;
    a1.Add( new c3d::string_t(formatter.Format()) );

    GetTool().SetEditText( formatter.FormatValue().c_str() );
  }
  {// pmin.y
    ValueFormatter formatter( _T("y"), &m_gabarit.pmin.y );
    m_values[3] = formatter;
    a1.Add( new c3d::string_t(formatter.Format()) );
  }
  {// pmin.z
    ValueFormatter formatter( _T("z"), &m_gabarit.pmin.z );
    m_values[4] = formatter;
    a1.Add( new c3d::string_t(formatter.Format()) );
  }

  _sntprintf( s1, STRINGLENGTH, _T("  %s"), _T("pmax") );
  a1.Add( new c3d::string_t(s1) );

  {// pmax.x
    ValueFormatter formatter( _T("x"), &m_gabarit.pmax.x );
    m_values[6] = formatter;
    a1.Add( new c3d::string_t(formatter.Format()) );
  }
  {// pmax.y
    ValueFormatter formatter( _T("y"), &m_gabarit.pmax.y );
    m_values[7] = formatter;
    a1.Add( new c3d::string_t(formatter.Format()) );
  }
  {// pmax.z
    ValueFormatter formatter( _T("z"), &m_gabarit.pmax.z );
    m_values[8] = formatter;
    a1.Add( new c3d::string_t(formatter.Format()) );
  }

  GetTool().SetListTexts( a1 );
  GetTool().SetCurSelList( 2 );
  GetTool().SelectEditText();
  GetTool().Layout();
  GetTool().SetFocusEdit();
}

// \ru Форматирование поля \en Format of the field
c3d::string_t ObjectGabaritWindow::ValueFormatter::Format()
{
  TCHAR s[STRINGLENGTH];
  _sntprintf( s, STRINGLENGTH, _T("    %s\t%f"), m_name.c_str(), m_value ? *m_value : 0 );
  return c3d::string_t(s);
}

// \ru Конструктор // \en Constructor
ObjectGabaritWindow::ValueFormatter::ValueFormatter() : m_value(nullptr)
{
}

// \ru Форматирование значения поля \en Format the field value
c3d::string_t ObjectGabaritWindow::ValueFormatter::FormatValue()
{
  TCHAR s[STRINGLENGTH];
  _sntprintf( s, STRINGLENGTH, _T("%f"), m_value ? *m_value : 0 );
  return c3d::string_t( s );
}

// \ru Прочитать выбранные объекты \en Load selected objects
void ObjectGabaritWindow::LoadObjects()
{
    std::vector<MbItemData> filters;
    MbItemData data;
    data.gabarit = m_gabarit;
    filters.push_back( data );
  ObjectFilterWindow::LoadObjects ( filters );
}

// \ru Показать сообщение о статусе \en Show status message
void ObjectGabaritWindow::ShowStatus( const TCHAR* message )
{
  if ( message ) {
    PArray<c3d::string_t> a;
    a.Add( new c3d::string_t(message) );
    GetTool().SetListTexts( a );
    GetTool().SetCurSelList( 0 );
    GetTool().Layout();
    GetTool().SetFocusList();
    GetTool().ShowEditItem( false );
    GetTool().Layout();
  }
}

// \ru Обработчик для команд \en Commands handler
bool ObjectGabaritWindow::FilterCommand( uint command, uint subCommand, bool isReturn )
{
  switch ( command ) {
  case BUTTON_APPLY: {
    LoadObjects();
  } break;
  case H_EDIT: {
    if ( subCommand == EN_UPDATE && isReturn ) {
      TCHAR s[STRINGLENGTH];
      _tcscpy( s, _T("") );
      GetTool().GetEditText( s, STRINGLENGTH );
      TCHAR * p = _tcsstr( s, _T("\r\n") );
      if ( !p )
        p = _tcsstr( s, _T("\n\r") );
      if ( p )
        *p = 0;
      TCHAR *ch;
      double d = _tcstod( s, &ch );
      if ( ch != nullptr && *ch == 0 && d != +HUGE_VAL && d != -HUGE_VAL ) {
        size_t currpos = GetTool().GetCurSelList();
        std::map<size_t, ValueFormatter>::iterator itr = m_values.find( currpos );
        if ( itr != m_values.end() ) {
          *itr->second.m_value = d;
          GetTool().SetEditText( itr->second.FormatValue().c_str() );
          GetTool().SetListText( currpos, itr->second.Format().c_str() );
          GetTool().SetCurSelList( currpos );
          GetTool().SetFocusList();
        }
      }
    }
  } break;
  case H_LIST: {
    switch ( subCommand ) {
    case LBN_SELCHANGE: {
      size_t currpos = GetTool().GetCurSelList();
      if ( currpos < static_cast<size_t>(GetDataCount()) ) {
        std::map<size_t, ValueFormatter>::iterator itr = m_values.find( currpos );
        if ( itr != m_values.end() ) {
          GetTool().SetEditText( itr->second.FormatValue().c_str() );
          GetTool().SelectEditText();
          GetTool().SetFocusEdit();
        }
        else
          GetTool().SetEditText( _T(" ") );
      }
    } break;
    case LBN_DBLCLK:
      break;
    }
  } break;
  default:
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
// ObjectAttrWindow

#ifndef C3D_WINDOWS // _MSC_VER
#define stdToInt64 strtoll
#else
#define stdToInt64 _acstoi64
#endif

//------------------------------------------------------------------------------
// \ru Форматирование значения BOOL
// \en Formatting BOOL value
//---
struct BoolFormatter
{
  #define TRUE_VALUE   _T("TRUE")
  #define TRUE_VALUE2  _T("true")
  #define TRUE_VALUE3  _T("True")
  #define FALSE_VALUE  _T("FALSE")

  static string_t FormatValue( bool value ) {
    return value ? TRUE_VALUE : FALSE_VALUE;
  }

  static bool GetValue( const TCHAR* value ) {
    string_t val( value );
    char* failChar;
    if ( val == string_t(TRUE_VALUE) ||
      val == string_t(TRUE_VALUE2) ||
      val == string_t(TRUE_VALUE3) ||
      strtol( ToSTDstring(val).c_str(), &failChar, 10 ) >= 1 )
      return true;
    return false;
  }
};

//------------------------------------------------------------------------------
// \ru Форматирование COLOR
// \en Formatting COLOR
//---
struct ColorFormatter
{
#define DELIMETER _T(",") // assumed 1 char

  static uint32 GetValue( const TCHAR* value )
  {
    char* failChar;
    string_t strValue( value );
    string_t::size_type pos = strValue.find( DELIMETER );
    if ( pos == string_t::npos )
      return 0;
    string_t val = strValue.substr( 0, pos );
    uint rVal = static_cast<uint>(strtoul( ToSTDstring(val).c_str(), &failChar, 10 ));
    strValue = strValue.substr( pos + 1, strValue.size() - pos - 1 );
    pos = strValue.find( DELIMETER );
    if ( pos == string_t::npos )
      return 0;
    val = strValue.substr( 0, pos );
    uint gVal = static_cast<uint>(strtoul( ToSTDstring(val).c_str(), &failChar, 10 ));
    val = strValue.substr( pos + 1, strValue.size() - pos - 1 );
    uint bVal = static_cast<uint>(strtoul( ToSTDstring(val).c_str(), &failChar, 10 ));
    return RGB( rVal, gVal, bVal );
  }

  static string_t FormatValue( uint32 value )
  {
    TCHAR buff[64];
    uint rVal = static_cast<uint>( GetRValue(value) );
    uint gVal = static_cast<uint>( GetGValue(value) );
    uint bVal = static_cast<uint>( GetBValue(value) );
    _sntprintf( buff, 64, _T("%u%s%u%s%u"), rVal, DELIMETER, gVal, DELIMETER, bVal );
    return string_t(buff);
  }
};

//------------------------------------------------------------------------------
// \ru Форматировать значение данных. \en Format data value.
//---
static string_t FormatValue( ItemDataBase* data )
{
// Приведение объекта 'item' к типу данных 'Type' (тип проверен заранее).
#define CAST(Type, item) dynamic_cast<Type*>(const_cast<ItemDataBase*>(item))
  if ( data == nullptr )
    return string_t();
  if ( data->IgnoreValue() )
    return _T("*");

  TCHAR buff[64];
  buff[0] = _T('\0');
  switch( data->Type() ) {
  case idtBool:            return BoolFormatter::FormatValue( CAST(ItemDataBool,data)->m_value );
  case idtInteger:         _sntprintf( buff, 64, _T("%d"),    CAST(ItemDataInteger,data)->m_value ); break;
  case idtDouble:          _sntprintf( buff, 64, _T("%f"),    CAST(ItemDataDouble,data)->m_value ); break;
  case idtString:          _sntprintf( buff, 64, _T("%s"),    CAST(ItemDataString,data)->m_value.c_str() ); break;
  case idtAttrBool:        return BoolFormatter::FormatValue( CAST(ItemAttrBool,data)->m_value );
  case idtAttrInt:         _sntprintf( buff, 64, _T("%d"),    CAST(ItemAttrInteger,data)->m_value ); break;
  case idtAttrDouble:      _sntprintf( buff, 64, _T("%f"),    CAST(ItemAttrDouble,data)->m_value ); break;;
  case idtAttrString:      _sntprintf( buff, 64, _T("%s"),    CAST(ItemAttrString,data)->m_value.c_str() ); break;
  case idtAttrInt64:       _sntprintf( buff, 64, _T("%I64d"), CAST(ItemAttrInt64,data)->m_value ); break;
  case idtAttrIdentifier:  _sntprintf( buff, 64, _T("%d"),    CAST(ItemAttrIdentifier,data)->m_value ); break;
  case idtAttrColor:       return ColorFormatter::FormatValue(CAST(ItemAttrColor,data)->m_value ); break;
  case idtAttrWidth:       _sntprintf( buff, 64, _T("%d"),    CAST(ItemAttrWidth,data)->m_value ); break;
  case idtAttrStyle:       _sntprintf( buff, 64, _T("%d"),    CAST(ItemAttrStyle,data)->m_value ); break;
  case idtAttrSelected:    return BoolFormatter::FormatValue( CAST(ItemAttrSelected,data)->m_value );
  case idtAttrVisible:     return BoolFormatter::FormatValue( CAST(ItemAttrVisible,data)->m_value );
  case idtAttrChanged:     return BoolFormatter::FormatValue( CAST(ItemAttrChanged,data)->m_value );
  case idtAttrDencity:     _sntprintf( buff, 64, _T("%f"),    CAST(ItemAttrDencity,data)->m_value ); break;
  case idtAttrUpdateStamp: _sntprintf( buff, 64, _T("%u"),    CAST(ItemAttrUpdateStamp,data)->m_value ); break;
  case idtAttrAnchor:      _sntprintf( buff, 64, _T("%u"),    CAST(ItemAttrAnchor,data)->m_value ); break;
  case idtAttrProductInfo: _sntprintf( buff, 64, _T("%s"),    CAST(ItemAttrProductInfo,data)->m_value.c_str() ); break;
  case idtAttrVisual:                 return  _T( "*" );
  case idtAttrWireCount:              return  _T( "*" );
  case idtAttrName:                   return  _T( "*" );
  case idtAttrGeom:                   return  _T( "*" );
  case idtAttrStampRib:               return  _T( "*" );
  case idtAttrModelInfo:              return  _T( "*" );
  case idtAttrPersonOrganizationInfo: return  _T( "*" );
  case idtAttrSTEPTextDescription:    return  _T( "*" );
  case idtAttrSTEPReferenceHolder:    return  _T( "*" );
  case idtAttrBinary:                 return  _T( "*" );
  case idtAttrInt32Vector:            return  _T( "*" );
  case idtAttrInt64Vector:            return  _T( "*" );
  case idtAttrDoubleVector:           return  _T( "*" );
  case idtAttrStrains:                return  _T( "*" );
  case idtAttrElasticity:             return  _T( "*" );
  case idtAttrSheetFlanging:          return  _T( "*" );
  default : break;
  }
  return buff;
}

//------------------------------------------------------------------------------
// \ru Конструктор. \en Constructor.
//---
ObjectAttrWindow::ObjectAttrWindow( reader_ex* reader ) : ObjectFilterWindow( reader ) 
{
  for ( uint16 type = (uint16)idtAttrBool; type < idtCount; ++type )
    m_types.push_back( (MbeItemDataType)type );
  CreateAttrWindow();
}

//------------------------------------------------------------------------------
// \ru Деструктор. \en Destructor.
//---
ObjectAttrWindow::~ObjectAttrWindow()
{
  std::map<MbeItemDataType,ItemDataBase*>::iterator sel = m_selection.begin();
  while ( sel != m_selection.end() ) {
    if ( sel->second != nullptr )
      delete sel->second;
    sel->second = nullptr;
    sel++;
  }
}

//------------------------------------------------------------------------------
// \ru Выбрать тип, если он еще не выбран, или удалить его из выбранных типов.
// Возвращает объект данных заданного типа или nullptr в случае удаления типа.
// \en Select the type, if it is not selected, or delete it from the selected types.
// Return the type data, or nullptr if the type is deleted from the selected types.
//---
ItemDataBase* ObjectAttrWindow::SelectType ( MbeItemDataType type )
{
  std::map<MbeItemDataType,ItemDataBase*>::iterator sel = m_selection.find( type );
  if ( sel != m_selection.end() ) {
    if ( sel->second != nullptr )
      delete sel->second;
    m_selection.erase( sel );
    return nullptr;
  }
  ItemDataBase* data = ItemDataBase::Create( type );
  m_selection.insert( std::pair<MbeItemDataType, ItemDataBase*>(type, data) );
  return data;
}

//------------------------------------------------------------------------------
// \ru Обновить список \en Update the list
//---
void ObjectAttrWindow::UpdateList( size_t pos, const TCHAR* s, const TCHAR* s2 )
{
  GetTool().SetListText( pos, s, s2 );
  GetTool().SetCurSelList( pos );
}

//------------------------------------------------------------------------------
// \ru Обновить значение данных. \en Update data value.
//------------------------------------------------------------------------------
static void UpdateValue( ItemDataBase* data, string_t& value )
{
  // Приведение объекта 'item' к типу данных 'Type' (тип проверен заранее).
#define CAST(Type, item) dynamic_cast<Type*>(const_cast<ItemDataBase*>(item))
  if ( data == nullptr )
    return;

  if ( value == _T("*") ) {
    data->SetIgnoreValue( true );
    return;
  }

  char* failChar;
  switch( data->Type() ) {
  case idtBool:            CAST(ItemDataBool,data)->m_value        = BoolFormatter::GetValue( value.c_str() ); break;
  case idtInteger:         CAST(ItemDataInteger,data)->m_value     = static_cast<int>(strtol( ToSTDstring(value).c_str(), &failChar, 10 )); break;
  case idtDouble:          CAST(ItemDataDouble,data)->m_value      = strtod( ToSTDstring(value).c_str(), &failChar ); break;
  case idtString:          CAST(ItemDataString,data)->m_value      = value; break;
  case idtAttrBool:        CAST(ItemAttrBool,data)->m_value        = BoolFormatter::GetValue( value.c_str() ); break;
  case idtAttrInt:         CAST(ItemAttrInteger,data)->m_value     = static_cast<int>(strtol( ToSTDstring(value).c_str(), &failChar, 10 )); break;
  case idtAttrDouble:      CAST(ItemAttrDouble,data)->m_value      = strtod( ToSTDstring(value).c_str(), &failChar ); break;
  //case idtAttrString:      CAST(ItemAttrString,data)->SetPrompt( value ); CAST(ItemAttrString,data)->SetIgnoreValue( true ); break; // \ru Проверка фильтра по подсказке. \en Check filtering by a prompt.
  case idtAttrString:      CAST(ItemAttrString,data)->m_value      = value; break;
  case idtAttrInt64:       CAST(ItemAttrInt64,data)->m_value       = stdToInt64( ToSTDstring(value).c_str(), &failChar, 10 ); break;
  case idtAttrIdentifier:  CAST(ItemAttrIdentifier,data)->m_value  = static_cast<int32>(strtol( ToSTDstring(value).c_str(), &failChar, 10 )); break;
  case idtAttrColor:       CAST(ItemAttrColor,data)->m_value       = ColorFormatter::GetValue( value.c_str() ); break;
  case idtAttrWidth:       CAST(ItemAttrWidth,data)->m_value       = static_cast<int>(strtol( ToSTDstring(value).c_str(), &failChar, 10 )); break;
  case idtAttrStyle:       CAST(ItemAttrStyle,data)->m_value       = static_cast<int>(strtol( ToSTDstring(value).c_str(), &failChar, 10 )); break;
  case idtAttrSelected:    CAST(ItemAttrSelected,data)->m_value    = BoolFormatter::GetValue( value.c_str() ); break;
  case idtAttrVisible:     CAST(ItemAttrVisible,data)->m_value     = BoolFormatter::GetValue( value.c_str() ); break;
  case idtAttrChanged:     CAST(ItemAttrChanged,data)->m_value     = BoolFormatter::GetValue( value.c_str() ); break;
  case idtAttrDencity:     CAST(ItemAttrDencity,data)->m_value     = strtod( ToSTDstring(value).c_str(), &failChar ); break;
  case idtAttrUpdateStamp: CAST(ItemAttrUpdateStamp,data)->m_value = static_cast<uint>(strtoul( ToSTDstring(value).c_str(), &failChar, 10 )); break;
  case idtAttrAnchor:      CAST(ItemAttrAnchor,data)->m_value      = (uint8)strtoul( ToSTDstring(value).c_str(), &failChar, 10 ); break;
  default:                 return;
  }
}

//------------------------------------------------------------------------------
// \ru Обработчик для команд \en Commands handler
//---
bool ObjectAttrWindow::FilterCommand(uint command, uint subCommand, bool isReturn )
{
  switch ( command ) {
  case BUTTON_APPLY: {
    LoadObjects();
  } break;
  case BUTTON_SELECT_TYPE: {
    size_t currpos = GetTool().GetCurSelList();
    if ( currpos < m_types.size() ) {
      ItemDataBase* data = SelectType( m_types[currpos] );
      c3d::string_t value = FormatValue(data);
      c3d::string_t formatted = FormatAttrString ( m_types[currpos], data != nullptr );
      UpdateList( currpos, formatted.c_str(), value.c_str() );
      GetTool().SetEditText( value.c_str() );
    }
  } break;
  case H_EDIT: {
    if ( subCommand == EN_UPDATE && isReturn ) {
      size_t currpos = GetTool().GetCurSelList();
      if ( currpos < m_types.size() ) {
        std::map<c3d::MbeItemDataType,c3d::ItemDataBase*>::iterator itr = m_selection.find( m_types[currpos] );
        if ( itr != m_selection.end() ) {
          ItemDataBase* data = itr->second;
          if ( data != nullptr ) {
            TCHAR s[STRINGLENGTH];
            _tcscpy( s, _T("") );
            GetTool().GetEditText( s, STRINGLENGTH );
            string_t intValue( s );
            string_t::size_type pos = intValue.find( _T("\r\n") );
            if ( pos == string_t::npos )
              pos = intValue.find( _T("\n\r") );
            if ( pos != string_t::npos ) {
              string_t partStr = intValue.substr( pos + 2, intValue.size() - pos - 2 );
              intValue = intValue.substr( 0, pos );
              intValue += partStr;
            }
            // value to data
            UpdateValue( data, intValue );
            c3d::string_t value = FormatValue(data);      
            c3d::string_t formatted = FormatAttrString ( m_types[currpos], true );
            UpdateList( currpos, formatted.c_str(), value.c_str() );
            GetTool().SetEditText( value.c_str() );
            GetTool().SetFocusList();
          }
        }
      }
    }
  } break;
  case H_LIST: {
    switch ( subCommand ) {
    case LBN_SELCHANGE:{
      size_t currpos = GetTool().GetCurSelList();
      if ( currpos < m_types.size() ) {
        c3d::string_t value;
        std::map<c3d::MbeItemDataType,c3d::ItemDataBase*>::iterator itr = m_selection.find( m_types[currpos] );
        if ( itr != m_selection.end() ) {
          ItemDataBase* data = itr->second;
          if ( data != nullptr ) {
            value = FormatValue(data); 
          }
        }
        UpdateList(currpos, nullptr, value.c_str());
        GetTool().SetEditText(value.c_str());
      } 
    } break;
    case LBN_DBLCLK:
      break;
    }
  } break;
  default:
    return false;
  }
  return true;
}

// \ru Прочитать выбранные объекты \en Load selected objects
void ObjectAttrWindow::LoadObjects()
{
  std::vector<MbItemData> filters;
  for ( std::map<MbeItemDataType,ItemDataBase*>::const_iterator itr = m_selection.begin(); itr != m_selection.end(); ++itr ) {
    MbItemData data;
    data.properties.Associate( itr->first, ItemDataBase::Create(itr->second) );
    filters.push_back( data );
  }
  ObjectFilterWindow::LoadObjects ( filters );
}

// \ru Показать сообщение о статусе \en Show status message
void ObjectAttrWindow::ShowStatus( const TCHAR* message )
{
  if ( message ) {
    PArray<c3d::string_t> a, a2;
    a.Add( new c3d::string_t(message) );
    GetTool().SetListTexts( a, &a2 );
    GetTool().SetCurSelList( 0 );
    GetTool().Layout();
    GetTool().SetFocusList();
    GetTool().ShowEditItem( false );
    GetTool().Layout();
  }
}

// \ru Создать диалог фильтров по габариту \en Create gabarit dialog
void ObjectAttrWindow::CreateAttrWindow()
{
#ifndef __USE_QT__
  HWND hwin = 0, hedit = 0, hlist = 0, hlist2 = 0;
  int start, listY, part;
  if ( CreateAttrClient(hwin, hedit, hlist, hlist2, start, listY, part, IDS_TITLE_SELECT_BY_ATTRIBUTE, (int)m_types.size()) )
    m_tool = CreateAttrTool ( hwin, hedit, hlist, hlist2, start, listY, part );
#else // __USE_QT__ 
  // TODO: implement it
#endif // __USE_QT__

  // populate list
  PArray<c3d::string_t> a, a1;
  for ( std::vector<MbeItemDataType>::const_iterator type = m_types.begin(); type != m_types.end(); ++type ) {
    if ( *type == idtAttrEmbodiment )
      continue;
    c3d::string_t formatted = FormatAttrString( *type, false );
    a.Add( new c3d::string_t(formatted) );
    a1.Add( new c3d::string_t() );
  }

  GetTool().SetListTexts( a, &a1 );
  GetTool().SetCurSelList( 0 );

  GetTool().SelectEditText();
  GetTool().Layout();
  GetTool().SetFocusEdit();
}

