#include "test_tree.h"
#include "test_variables.h"
#include <tool_cstring.h>
#include <templ_p_array.h>
#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <model_tree_data.h>
#include <test_property_title.h>

//----------------------------------------------------------------------------------------
// IModelTreeWindow
// ---

// \ru Деструктор // \en Destructor
IModelTreeWindow::~IModelTreeWindow()
{
  if ( m_parent != nullptr ) {
    if ( this == m_parent->m_child )
      m_parent->m_child = nullptr;
    m_parent->Release();
  }
  if ( m_treeTool != nullptr )
    delete m_treeTool;
}

// \ru Обновить список потомков \en Update children list
void IModelTreeWindow::UpdateChildList ( size_t pos, const TCHAR* s )
{
  GetTreeTool().SetChildListText ( pos, s );
  GetTreeTool().SetCurSelChildList ( pos );
}

//----------------------------------------------------------------------------------------
// ModelTreeWindow
// ---
std::set<const c3d::IModelTreeNode*>  ModelTreeWindow::m_selection;

// \ru Выбрать узел, если он еще не выбран, или удалить его из выбранных узлов
//     Возвращает true, если узел выбран, и false в противном случае  
// \en Select node if it has not been selected before or remove it if it was already selected
//     Return true if node added or false otherwise
bool ModelTreeWindow::SelectNode( const c3d::IModelTreeNode* node )
{
  std::set<const c3d::IModelTreeNode*>::iterator sel = m_selection.find ( node );
  if ( sel != m_selection.end() ) {
    m_selection.erase ( sel );
    return false;
  }
  m_selection.insert ( node );
  return true;
}

//----------------------------------------------------------------------------------------
// TreeWindow
// ---

// \ru Конструктор // \en Constructor
TreeWindow::TreeWindow ( reader_ex* reader ) : ModelTreeWindow ( reader )
, m_obj(reader->GetModelTree())
{
  PRECONDITION ( m_obj != nullptr );
  m_selection.clear();
  CreateTreeWindow(); //  create window
}

// \ru Деструктор // \en Destructor
TreeWindow::~TreeWindow()
{
}

static c3d::string_t ItemString( MbeSpaceType type )
{
  TCHAR name[100];
  switch (type)
  {
  case st_AssistedItem:  ::GetPromptValue(IDS_PROP_0160, name); break;
  case st_Collection:    ::GetPromptValue(IDS_ITEM_0405, name); break;
  case st_PointFrame:    ::GetPromptValue(IDS_ITEM_0404, name); break;
  case st_WireFrame:     ::GetPromptValue(IDS_ITEM_0403, name); break;
  case st_Solid:         ::GetPromptValue(IDS_ITEM_0401, name); break;
  case st_Instance:      ::GetPromptValue(IDS_ITEM_0707, name); break;
  case st_Assembly:      ::GetPromptValue(IDS_ITEM_0705, name); break;
  case st_Mesh:          ::GetPromptValue(IDS_ITEM_0071, name); break;
  case st_SpaceInstance: ::GetPromptValue(IDS_ITEM_0703, name); break;
  case st_PlaneInstance: ::GetPromptValue(IDS_ITEM_0702, name); break;
#if defined ( __NATIVE_LANGUAGE__ )
  default:               return _T("Неизвестный объект");
#else
  default:               return _T("Unknown Item");
#endif
  }
  return name;
};

// \ru Форматировать строку для типа объекта с учетом его выбора
// \en Format type string considering its selection
c3d::string_t FormatTypeString ( MbeSpaceType t, bool selected )
{
  TCHAR s[STRINGLENGTH];
  _sntprintf ( s, STRINGLENGTH, _T(" %s\t%s"), selected ? _T("+") : _T(" "), ItemString( t ).c_str() );
  return c3d::string_t(s);
}

using namespace c3d;

static c3d::string_t AttrString( MbeItemDataType type )
{
  TCHAR name[100];
  switch (type)
  {
  case idtAttrBool:                   ::GetPromptValue(IDS_ITEM_0784, name); break;
  case idtAttrInt:                    ::GetPromptValue(IDS_ITEM_0785, name); break;
  case idtAttrDouble:                 ::GetPromptValue(IDS_ITEM_0786, name); break;
  case idtAttrString:                 ::GetPromptValue(IDS_ITEM_0787, name); break;
  case idtAttrInt64:                  ::GetPromptValue(IDS_ITEM_0790, name); break;
  case idtAttrIdentifier:             ::GetPromptValue(IDS_ITEM_0768, name); break;
  case idtAttrColor:                  ::GetPromptValue(IDS_ITEM_0764, name); break;
  case idtAttrWidth:                  ::GetPromptValue(IDS_ITEM_0765, name); break;
  case idtAttrStyle:                  ::GetPromptValue(IDS_ITEM_0766, name); break;
  case idtAttrSelected:               ::GetPromptValue(IDS_ITEM_0769, name); break;
  case idtAttrVisible:                ::GetPromptValue(IDS_ITEM_0770, name); break;
  case idtAttrChanged:                ::GetPromptValue(IDS_ITEM_0771, name); break;
  case idtAttrDencity:                ::GetPromptValue(IDS_ITEM_0763, name); break;
  case idtAttrUpdateStamp:            ::GetPromptValue(IDS_ITEM_0775, name); break;
  case idtAttrAnchor:                 ::GetPromptValue(IDS_ITEM_0773, name); break;
  case idtAttrVisual:                 ::GetPromptValue(IDS_ITEM_0767, name); break;
  case idtAttrWireCount:              ::GetPromptValue(IDS_ITEM_0762, name); break;
  case idtAttrName:                   ::GetPromptValue(IDS_ITEM_0772, name); break;
  case idtAttrGeom:                   ::GetPromptValue(IDS_ITEM_0774, name); break;
  case idtAttrStampRib:               ::GetPromptValue(IDS_ITEM_0779, name); break;
  case idtAttrModelInfo:              ::GetPromptValue(IDS_PROP_1000, name); break;
  case idtAttrPersonOrganizationInfo: ::GetPromptValue(IDS_PROP_1010, name); break;
  case idtAttrProductInfo:            ::GetPromptValue(IDS_PROP_1030, name); break;
  case idtAttrBinary:                 ::GetPromptValue(IDS_ITEM_0791, name); break;
  case idtAttrInt32Vector:            ::GetPromptValue(IDS_ITEM_0792, name); break;
  case idtAttrInt64Vector:            ::GetPromptValue(IDS_ITEM_0793, name); break;
  case idtAttrDoubleVector:           ::GetPromptValue(IDS_ITEM_0794, name); break;
  case idtAttrStrains:                ::GetPromptValue(IDS_ITEM_0754, name); break;
  case idtAttrElasticity:             ::GetPromptValue(IDS_ITEM_0751, name); break;
  case idtAttrSheetFlanging:          ::GetPromptValue(IDS_ITEM_0780, name); break;
#if defined ( __NATIVE_LANGUAGE__ )
  case idtAttrSTEPTextDescription:    _sntprintf( name, 100, _T(" Описание STEP") ); break;
  case idtAttrSTEPReferenceHolder:    _sntprintf( name, 100, _T(" Объект ссылки STEP") ); break;
  default:                            _sntprintf( name, 100, _T(" Неизвестный атрибут")); break;
#else
  case idtAttrSTEPTextDescription:    _sntprintf( name, 100, _T(" Description STEP") ); break;
  case idtAttrSTEPReferenceHolder:    _sntprintf( name, 100, _T(" Reference object STEP") ); break;
  default:                            _sntprintf( name, 100, _T(" Unknown attribute") ); break;
#endif
  }
  string_t res(name);
  if ( type == idtAttrColor )
    res += _T(" [RGB]");
  return res;
};


// \ru Форматировать строку для атрибута объекта с учетом его выбора
// \en Format attribute string considering its selection
c3d::string_t FormatAttrString ( MbeItemDataType type, bool selected )
{
  TCHAR s[STRINGLENGTH];
  if ( selected )
    _sntprintf( s, STRINGLENGTH, _T( "+   %s" ), AttrString( type ).c_str() );
  else
    _sntprintf( s, STRINGLENGTH, _T( "    %s" ), AttrString( type ).c_str() );
  return c3d::string_t( s );
}

// \ru Создать диалог дерева модели \en Create model tree dialog
void TreeWindow::CreateTreeWindow()
{
#ifndef __USE_QT__ 
  int prop_count = 0;
  int child_count = (int)GetRoots().size();
  HWND hprop = 0;
  HWND hchildlist = 0;
  HWND hproplist = 0;
  int butsize, proplistsize;
  if ( CreateTreeClient(hprop, hchildlist, hproplist, butsize, proplistsize, IDS_TITLE_MODEL_CATALOG, prop_count, child_count) )
    m_treeTool = CreateTreeTool ( hprop, hchildlist, hproplist, butsize, proplistsize );
#else // __USE_QT__ 
  // TODO: implement it
  //QWidget * hprop = nullptr;
  //f ( CreatTreeClient( hprop, title, npc ) )
  //  m_treeTool = CreateTreeTool( hprop );
#endif // __USE_QT__

  // populate root nodes
  PArray<c3d::string_t> a;
  for ( std::vector<const c3d::IModelTreeNode*>::const_iterator node = GetRoots().begin(); node != GetRoots().end(); ++node ) {
    std::set<const c3d::IModelTreeNode*>::const_iterator sel = m_selection.find(*node);
    bool added = sel != m_selection.end();
    const c3d::MbItemData& data = (*node)->GetData();
    a.Add ( new c3d::string_t(FormatTypeString(data.type, added)) );
  }

  GetTreeTool().SetChildListTexts ( a );
  GetTreeTool().SetCurSelChildList ( 0 );
  GetTreeTool().Layout();
  GetTreeTool().SetFocusChildList();

  if (this->m_obj->GetType() == c3d::IModelTree::mtt_Embodiment )
    MessageBoxEx( IDS_TITLE_CATALOG_MESSAGE_EMBODI, IDS_TITLE_WRN );
}

// \ru Прочитать выбранные объекты \en Load selected objects
void TreeWindow::LoadObjects()
{
  if ( m_selection.size() && m_reader.get() != nullptr && m_reader->GetModelTree() ) {
    if ( m_reader->GetModelTree()->GetType() == c3d::IModelTree::mtt_Embodiment ) {
      ShowStatus( IDS_TITLE_SELECT_MESSAGE_EMBODI );
      MessageBoxEx( IDS_TITLE_SELECT_MESSAGE_EMBODI, IDS_TITLE_ERR );
    }
    else {

      std::vector<const c3d::IModelTreeNode*> nodes;
      for ( std::set<const c3d::IModelTreeNode*>::const_iterator obj = m_selection.begin(); obj != m_selection.end(); ++obj ) {
        if ( *obj )
          nodes.push_back( *obj );
      }
      std::unique_ptr<const c3d::IModelTree> tree( m_reader->GetModelTree()->GetFilteredTree( nodes ) );
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
        _sntprintf( message, 128, IDS_TITLE_SELECT_MESSAGE_FORMAT, LoUint32( num ) );
        ShowStatus( message );

        WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
        // read object and add to the model
        std::vector< SPtr<MbItem> > items;
        SetWaitCursor( true );
        ::ReadModelItemsFromTree( *m_reader, tree.get(), items );
        SetWaitCursor( false );
        if ( m_reader->state() & io::skippedUnknown )
          MessageBoxEx( RT_FILE_WITH_UNKNOWN_OBJECTS, IDS_TITLE_WRN );
        if ( m_reader->state() & io::verViolation )
          MessageBoxEx( RT_WAR_FILE_VERSION, IDS_TITLE_WRN );
        if ( items.size() ) {
          ShowStatus( IDS_TITLE_SELECT_MESSAGE_DRAW );
          wind->AddObjectsToModel( items );
        }
      }
    }
  }
  GetTreeTool().CloseTreeWindow();
}

// \ru Показать сообщение о статусе \en Show status message
void TreeWindow::ShowStatus ( const TCHAR* message )
{
  if (message) {
    PArray<c3d::string_t> a;
    a.Add ( new c3d::string_t(message) );
    GetTreeTool().SetChildListTexts ( a );
    GetTreeTool().SetCurSelChildList ( 0 );
    GetTreeTool().Layout();
    GetTreeTool().SetFocusChildList();
  }
}

// \ru Обработчик для команд \en Commands handler 
bool TreeWindow::TreeCommand ( uint command, uint subCommand, bool )
{
  switch ( command ) {
  case BUTTON_APPLY: {
      LoadObjects();
    } break;
  case BUTTON_SELECT_ALL: {
    size_t currpos = GetTreeTool().GetCurSelChildList();
    for ( size_t pos = 0; pos < GetRoots().size(); ++pos ) {
      bool added = SelectNode ( GetRoots()[pos] );
      const c3d::MbItemData& data = GetRoots()[pos]->GetData();
      c3d::string_t s = FormatTypeString ( data.type, added );
      UpdateChildList ( pos, s.c_str() );
    }
    GetTreeTool().SetCurSelChildList ( currpos );
  } break;
  case BUTTON_SELECT_CHILD: {
    size_t currpos = GetTreeTool().GetCurSelChildList();
    if ( currpos < GetRoots().size() ) {
      bool added = SelectNode ( GetRoots()[currpos] );
      const c3d::MbItemData& data = GetRoots()[currpos]->GetData();
      c3d::string_t s = FormatTypeString ( data.type, added );
      UpdateChildList ( currpos, s.c_str() );
    }
  } break;
  case H_CHILDLIST: {
      switch ( subCommand ) {
      case LBN_SELCHANGE:
        break;
      case LBN_DBLCLK: {
          if ( !IsBlocked() ) {
            size_t currpos = GetTreeTool().GetCurSelChildList();
            if ( currpos < GetRoots().size() ) {
              TestVariables::viewManager->CreateTreeNode ( GetRoots()[currpos], this );
            }
          }
        } break;
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

// \ru Обработчик для команд \en Commands handler 
bool TreeWindow::KeyboardCommand ( uint command, bool shift, bool ctrl )
{
  switch ( command ) {
    case VK_RETURN: {
      if ( ctrl || shift ) {
        GetTreeTool().CloseTreeWindow();
      }
      else {
        if ( !IsBlocked() ) {
          size_t currpos = GetTreeTool().GetCurSelChildList();
          if ( currpos < GetRoots().size() ) {
            TestVariables::viewManager->CreateTreeNode ( GetRoots()[currpos], this );
          }
        }
      }
    } break;

    case VK_ESCAPE: {
      GetTreeTool().CloseTreeWindow();
    } break;

    default:
      return false;
  }

  return true;
}

// \ru Обработчик для команд \en Commands handler 
void TreeWindow::PreDestroyCommand ( bool  )
{
  if ( m_child != nullptr ) {
    m_child->GetTreeTool().CloseTreeWindow();
    m_child->PreDestroyCommand ( false );
  }

  TestVariables::viewManager->RemoveTree ( this );
}

//------------------------------------------------------------------------------
// NodeWindow
// ---

// \ru Конструктор // \en Constructor
NodeWindow::NodeWindow ( const c3d::IModelTreeNode* initObj )
  : ModelTreeWindow()
  ,m_obj(initObj)
{
  PRECONDITION ( m_obj != nullptr );

  CreateNodeWindow(); // create window
}

// \ru Конструктор // \en Constructor
NodeWindow::NodeWindow ( const c3d::IModelTreeNode * initObj, ModelTreeWindow * initParent )
  : m_obj(initObj)
{
  PRECONDITION ( initParent != nullptr );
  m_parent = initParent;
  if ( m_parent != nullptr )
  {
    m_parent->SetChild ( this );
    m_parent->AddRef();
  }
  CreateNodeWindow(); //  create window
}

// \ru Деструктор // \en Destructor
NodeWindow::~NodeWindow()
{
}

// \ru Обработчик для команд \en Commands handler 
bool NodeWindow::TreeCommand ( uint command, uint subCommand, bool )
{
  switch ( command ) {
  case BUTTON_SELECT: {
    bool added = SelectNode ( m_obj );
    if ( m_parent ) {
      const c3d::MbItemData& data = m_obj->GetData();
      c3d::string_t s = FormatTypeString ( data.type, added );
      size_t pos = m_parent->GetTreeTool().GetCurSelChildList();
      m_parent->UpdateChildList ( pos, s.c_str() );
    }
    } break;
    case BUTTON_SELECT_CHILD: {
      size_t const currpos = GetTreeTool().GetCurSelChildList();
      if ( currpos < GetChildren().size() ) {
        size_t pos = currpos;
        std::set<const c3d::IModelTreeNode*>::const_iterator itr = GetChildren().begin();
        while ( pos > 0 ) {
          ++itr;
          --pos;
        }
        bool added = SelectNode ( *itr );
        const c3d::MbItemData& data = (*itr)->GetData();
        c3d::string_t s = FormatTypeString ( data.type, added );
        UpdateChildList ( currpos, s.c_str() );
      }
    } break;
    case H_CHILDLIST: {
      switch ( subCommand ) {
        case LBN_DBLCLK: {
          if ( !IsBlocked() ) {
            size_t currpos = GetTreeTool().GetCurSelChildList();
            if ( currpos < GetChildren().size() ) {
              std::set<const c3d::IModelTreeNode*>::const_iterator itr = GetChildren().begin();
              while ( currpos > 0 ) {
                ++itr;
                --currpos;
              }
              TestVariables::viewManager->CreateTreeNode( *itr, this );
            }
          }
        } break;
      }
    } break;
    default:
      return false;
  }
  return true;
}

// \ru Обработчик для команд \en Commands handler 
bool NodeWindow::KeyboardCommand ( uint command, bool shift, bool ctrl )
{
  switch ( command ) {
    case VK_RETURN: {
      if ( ctrl || shift ) {
        GetTreeTool().CloseTreeWindow();
      }
      else {
        if ( !IsBlocked() ) {
          size_t currpos = GetTreeTool().GetCurSelChildList();
          if ( currpos < GetChildren().size() ) {
            std::set<const c3d::IModelTreeNode*>::const_iterator itr = GetChildren().begin();
            while ( currpos > 0 ) {
              ++itr;
              --currpos;
            }
            TestVariables::viewManager->CreateTreeNode( *itr, this );
          }
        }
      }
    } break;

    case VK_ESCAPE: {
      GetTreeTool().CloseTreeWindow();
    } break;

    default:
      return false;
  }

  return true;
}

// \ru Обработчик для команд \en Commands handler 
void NodeWindow::PreDestroyCommand ( bool redraw )
{
  if ( m_child != nullptr ) {
    m_child->GetTreeTool().CloseTreeWindow();
    m_child->PreDestroyCommand ( false );
  }
  if ( redraw && (m_parent != nullptr) && TestVariables::viewManager->IsMyTree(m_parent) )
    m_parent->GetTreeTool().SetFocusPropList();

  TestVariables::viewManager->RemoveTree ( this );
}

// \ru Создать диалог узла дерева модели \en Create model tree node dialog
void NodeWindow::CreateNodeWindow()
{
  int prop_count = GetDataCount();
  int child_count = (int)GetChildren().size();

#ifndef __USE_QT__ 
  HWND hprop = 0;
  HWND hchildlist = 0;
  HWND hproplist = 0;
  int butsize, proplistsize;
  if ( CreateTreeClient(hprop, hchildlist, hproplist, butsize, proplistsize, IDS_TITLE_OBJECT, prop_count, child_count) )
    m_treeTool = CreateTreeTool ( hprop, hchildlist, hproplist, butsize, proplistsize );
#else // __USE_QT__ 
  // TODO: implement it
  //QWidget * hprop = nullptr;
  //if (CreateTreeClient(hprop, title, npc))
  //  m_treeTool = CreatePropertyTool(hprop);
#endif // __USE_QT__

  // populate properties
  TCHAR s1[STRINGLENGTH];
  PArray<c3d::string_t> a1;
  const c3d::MbItemData& data = m_obj->GetData();
  _sntprintf ( s1, STRINGLENGTH, _T("%s \t%s"), IDS_TITLE_OBJECT_TYPE, ItemString( data.type ).c_str() );
  a1.Add ( new c3d::string_t(s1) );
  _sntprintf ( s1, STRINGLENGTH, _T("%s \t%d"), IDS_TITLE_OBJECT_NAME, data.name );
  a1.Add ( new c3d::string_t(s1) );
  _sntprintf ( s1, STRINGLENGTH, _T("%s"), IDS_TITLE_OBJECT_GABARIT );
  a1.Add ( new c3d::string_t(s1) );
  _sntprintf ( s1, STRINGLENGTH, _T("   %s\t%f %f %f"), _T("pmin"), data.gabarit.pmin.x, data.gabarit.pmin.y, data.gabarit.pmin.z );
  a1.Add ( new c3d::string_t(s1) );
  _sntprintf ( s1, STRINGLENGTH, _T("   %s\t%f %f %f"), _T("pmax"), data.gabarit.pmax.x, data.gabarit.pmax.y, data.gabarit.pmax.z );
  a1.Add ( new c3d::string_t(s1) );
  GetTreeTool().SetPropListTexts ( a1 );

  // populate children
  PArray<c3d::string_t> a;
  for ( std::set<const c3d::IModelTreeNode*>::const_iterator node = GetChildren().begin(); node != GetChildren().end(); ++node ) {
    std::set<const c3d::IModelTreeNode*>::const_iterator sel = m_selection.find ( *node );
    bool added = sel != m_selection.end();
    const c3d::MbItemData& d = (*node)->GetData();
    a.Add ( new c3d::string_t(FormatTypeString(d.type, added)) );
  }

  GetTreeTool().SetChildListTexts ( a );
  GetTreeTool().SetCurSelChildList ( 0 );
  GetTreeTool().SetCurSelPropList ( 0 );
  GetTreeTool().SetFocusPropList();
  GetTreeTool().Layout();
}

//---------------------------------------------------------------------
//              IEmbodimentTreeWindow
//---------------------------------------------------------------------

const c3d::IEmbodimentNode* IEmbodimentTreeWindow::m_selection; // \ru выбранный объект \en selected object

// \ru Выбрать узел, если он еще не выбран, или удалить его из выбора.
// \en Select node if it has not been selected before or remove it from selection.
bool IEmbodimentTreeWindow::SelectNode( const c3d::IEmbodimentNode* node )
{
  if ( node == m_selection ) {
    m_selection = nullptr;
    return false;
  }
  m_selection = node;
  return true;
}

// \ru Показать сообщение о статусе \en Show status message
void IEmbodimentTreeWindow::ShowStatus( const TCHAR* message )
{
  if ( message ) {
    PArray<c3d::string_t> a;
    a.Add( new c3d::string_t( message ) );
    GetTreeTool().SetChildListTexts( a );
    GetTreeTool().SetCurSelChildList( 0 );
    GetTreeTool().Layout();
    GetTreeTool().SetFocusChildList();
  }
}

// \ru Прочитать выбранные объекты \en Load selected objects
void IEmbodimentTreeWindow::LoadObjects()
{
  if ( m_selection != nullptr ) {
    reader_ex* reader = GetReader();
    if ( reader == nullptr ) {
      IModelTreeWindow* parent = const_cast<IModelTreeWindow*>( GetParent() );
      if ( parent != nullptr )
        static_cast<IEmbodimentTreeWindow*>( parent )->LoadObjects();
    }
    else {
      WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
      const c3d::IModelTreeNode * node = m_selection->GetModelTreeNode();
      std::vector< SPtr<MbItem> > items;
      SetWaitCursor( true );
      ShowStatus( IDS_TITLE_SELECT_MESSAGE_READ );
      wind->DeleteModel();
      ::ReadModelItemsFromTree( *reader, node, wind->SetGeomModel(), true );
      if ( reader->state() & io::skippedUnknown )
        MessageBoxEx( RT_FILE_WITH_UNKNOWN_OBJECTS, IDS_TITLE_WRN );
      if ( m_reader->state() & io::verViolation )
        MessageBoxEx( RT_WAR_FILE_VERSION, IDS_TITLE_WRN );
      ShowStatus( IDS_TITLE_SELECT_MESSAGE_DRAW );
      {// show model
        wind->FillDrawModel();
        wind->CalculateGabarit();
        wind->ShowModel();
      }
      SetWaitCursor( false );

      GetTreeTool().CloseTreeWindow();
    }
  }
}

//---------------------------------------------------------------------
//              EmbodimentTreeWindow
//---------------------------------------------------------------------

// \ru Конструктор // \en Constructor
EmbodimentTreeWindow::EmbodimentTreeWindow( reader_ex* reader ) : IEmbodimentTreeWindow( reader )
  , m_emb( nullptr )
{
  if ( reader->GetModelTree()->GetType() == IModelTree::mtt_Embodiment ) {
    m_emb = reader->GetModelTree()->GetEmbodimentsTree();
  }

  m_selection = nullptr;
  CreateTreeWindow(); //  create window
}

// \ru Деструктор // \en Destructor
EmbodimentTreeWindow::~EmbodimentTreeWindow()
{
}

// \ru Форматировать строку для исполнения
// \en Format embodiment string considering its selection
static c3d::string_t FormatIDString( size_t id, bool selected )
{
  TCHAR s[STRINGLENGTH];
  if ( selected )
    _sntprintf( s, STRINGLENGTH, _T( "+   " ) IDS_TITLE_EMBODIMENT _T( " %zi" ), id );
  else
    _sntprintf( s, STRINGLENGTH, _T( "    " ) IDS_TITLE_EMBODIMENT _T( " %zi" ), id );
  return c3d::string_t( s );
}

// \ru Форматировать строку свойств для исполнения
// \en Format string of embodiment properties considering its selection
static void GetEmbPropStrings( const c3d::MbItemData& data, PArray<c3d::string_t>& a1 )
{
  // populate properties
  TCHAR s1[STRINGLENGTH];
  MultiMap<MbeItemDataType, ItemDataBase*>::Iterator itr = data.properties.Find( idtAttrProductInfo );
  c3d::string_t id = ( !itr.Empty() ) ? static_cast< ItemAttrProductInfo* >( itr.Value() )->m_value : c3d::string_t();
  _sntprintf( s1, STRINGLENGTH, _T( "%s \t%s" ), IDS_TITLE_OBJECT_ID, id.c_str() );
  a1.Add( new c3d::string_t( s1 ) );
  _sntprintf( s1, STRINGLENGTH, _T( "%s" ), IDS_TITLE_OBJECT_GABARIT );
  a1.Add( new c3d::string_t( s1 ) );
  _sntprintf( s1, STRINGLENGTH, _T( "   %s\t%f %f %f" ), _T( "pmin" ), data.gabarit.pmin.x, data.gabarit.pmin.y, data.gabarit.pmin.z );
  a1.Add( new c3d::string_t( s1 ) );
  _sntprintf( s1, STRINGLENGTH, _T( "   %s\t%f %f %f" ), _T( "pmax" ), data.gabarit.pmax.x, data.gabarit.pmax.y, data.gabarit.pmax.z );
  a1.Add( new c3d::string_t( s1 ) );

}

// \ru Заполнить данные диалога \en Populate dialog data
void EmbodimentTreeWindow::Populate( size_t pos )
{
  PArray<c3d::string_t> a;
  const std::vector<const c3d::IEmbodimentNode*>& roots = GetRoots();
  // populate root nodes
  for ( std::vector<const c3d::IEmbodimentNode*>::const_iterator node = roots.begin(); node != roots.end(); ++node ) {
    a.Add( new c3d::string_t( FormatIDString( ( *node )->GetEmbodimentData().name, m_selection == *node ) ) );
  }
  GetTreeTool().SetChildListTexts( a );
  GetTreeTool().SetCurSelChildList( pos );
}

// \ru Заполнить данные диалога \en Populate dialog data
void EmbodimentTreeWindow::Populate()
{
  size_t currpos = GetTreeTool().GetCurSelChildList();
  Populate( currpos );
}

// \ru Создать диалог дерева исполнений \en Create Embodiment tree dialog
void EmbodimentTreeWindow::CreateTreeWindow()
{
  if ( m_emb == nullptr ) {
    MessageBoxEx( IDS_CATALOG_MESSAGE_EMBODI_ERR, IDS_TITLE_ERR );
    return;
  }
  int prop_count = GetDataCount();
  int child_count = ( int )GetRoots().size();

#ifndef __USE_QT__ 
  HWND hprop = 0;
  HWND hchildlist = 0;
  HWND hproplist = 0;
  int butsize, proplistsize;
  if ( CreateEmbTreeClient( hprop, hchildlist, hproplist, butsize, proplistsize, IDS_TITLE_EMBODIMENT_CATALOG, prop_count, child_count ) )
    m_treeTool = CreateTreeTool( hprop, hchildlist, hproplist, butsize, proplistsize );
#else // __USE_QT__ 
  // TODO: implement it
#endif // __USE_QT__

  PArray<c3d::string_t> a1;
  if ( GetRoots().size() > 0 ) {
    // populate properties of the first embodiment or selected embodiment
    const c3d::MbItemData& data = m_selection != nullptr ? m_selection->GetEmbodimentData() : (*GetRoots().begin())->GetEmbodimentData();
    GetEmbPropStrings( data, a1 );
  }
  Populate( 0 );

  GetTreeTool().SetPropListTexts( a1 );
  GetTreeTool().Layout();
  GetTreeTool().SetFocusChildList();
}

// \ru Обработчик для команд \en Commands handler 
bool EmbodimentTreeWindow::TreeCommand( uint command, uint subCommand, bool )
{
  switch ( command ) {
  case BUTTON_APPLY:
  {
    LoadObjects();
  } break;
  case BUTTON_SELECT_CHILD:
  {
    size_t currpos = GetTreeTool().GetCurSelChildList();
    if ( currpos < GetRoots().size() ) {
      SelectNode( GetRoots()[currpos] );
      Populate( currpos );
    }
  } break;
  case H_CHILDLIST:
  {
    switch ( subCommand ) {
    case LBN_SELCHANGE:
    {
      size_t currpos = GetTreeTool().GetCurSelChildList();
      if ( currpos < GetRoots().size() ) {
        PArray<c3d::string_t> a1;
        const c3d::MbItemData& data = GetRoots()[currpos]->GetEmbodimentData();
        GetEmbPropStrings( data, a1 );
        GetTreeTool().SetPropListTexts( a1 );
      }
      break;
    }
    case LBN_DBLCLK:
    {
      if ( !IsBlocked() ) {
        size_t currpos = GetTreeTool().GetCurSelChildList();
        if ( currpos < GetRoots().size() ) {
          const c3d::IEmbodimentNode* node = GetRoots()[currpos];
          if( node->GetChildren().size() != 0 )
            TestVariables::viewManager->CreateEmbodimentNode( node, this );
        }
      }
    } break;
    }
  } break;
  case H_LIST:
  {
    switch ( subCommand ) {
    case LBN_SELCHANGE:
    {
      size_t currpos = GetTreeTool().GetCurSelChildList();
      if ( currpos < GetRoots().size() ) {
        PArray<c3d::string_t> a1;
        const c3d::MbItemData& data = GetRoots()[currpos]->GetEmbodimentData();
        GetEmbPropStrings( data, a1 );
        GetTreeTool().SetPropListTexts( a1 );
      }
      break;
    }
    case LBN_DBLCLK:
      break;
    }
  } break;
  default:
    return false;
  }
  return true;
}

// \ru Обработчик для команд \en Commands handler 
bool EmbodimentTreeWindow::KeyboardCommand( uint command, bool shift, bool ctrl )
{
  switch ( command ) {
  case VK_RETURN:
  {
    if ( ctrl || shift ) {
      GetTreeTool().CloseTreeWindow();
    }
    else {
      if ( !IsBlocked() ) {
        size_t currpos = GetTreeTool().GetCurSelChildList();
        if ( currpos < GetRoots().size() ) {
          const c3d::IEmbodimentNode* node = GetRoots()[currpos];
          if ( node->GetChildren().size() != 0 )
            TestVariables::viewManager->CreateEmbodimentNode( node, this );
        }
      }
    }
  } break;

  case VK_ESCAPE:
  {
    GetTreeTool().CloseTreeWindow();
  } break;

  default:
    return false;
  }

  return true;
}

// \ru Обработчик для команд \en Commands handler 
void EmbodimentTreeWindow::PreDestroyCommand( bool )
{
  if ( m_child != nullptr ) {
    m_child->GetTreeTool().CloseTreeWindow();
    m_child->PreDestroyCommand( false );
  }

  TestVariables::viewManager->RemoveTree( this );
}

//---------------------------------------------------------------------
//  NodeEmbodimentWindow
//---------------------------------------------------------------------

NodeEmbodimentWindow::NodeEmbodimentWindow( const c3d::IEmbodimentNode * initObj, IModelTreeWindow * initParent )
  : IEmbodimentTreeWindow(), m_node( initObj )
{
  PRECONDITION( initParent != nullptr );
  m_parent = initParent;
  if ( m_parent != nullptr ) {
    m_parent->SetChild( this );
    m_parent->AddRef();
  }
  CreateNodeWindow(); //  create window
}

// \ru Деструктор // \en Destructor
NodeEmbodimentWindow::~NodeEmbodimentWindow()
{
}

// \ru Обработчик для команд \en Commands handler 
bool NodeEmbodimentWindow::TreeCommand( uint command, uint subCommand, bool )
{
  switch ( command ) {
  case BUTTON_APPLY:
  {
    LoadObjects();
  } break;
  case BUTTON_SELECT_CHILD:
  {
    size_t currpos = GetTreeTool().GetCurSelChildList();
    if ( currpos < GetChildren().size() ) {
      size_t pos = currpos;
      std::set<const c3d::IEmbodimentNode*>::const_iterator itr = GetChildren().begin();
      while ( pos > 0 ) {
        ++itr;
        --pos;
      }
      bool added = SelectNode( *itr );
      Populate( currpos );
      if ( added ) {
        // \ru обновить все родительские окна \en update all parent windows
        IModelTreeWindow* parent = const_cast<IModelTreeWindow*>( GetParent() );
        while ( parent != nullptr ) {
          static_cast< IEmbodimentTreeWindow* >( parent )->Populate();
          parent = const_cast<IModelTreeWindow*>( parent->GetParent() );
        }

      }
    }
  } break;
  case H_CHILDLIST:
  {
    switch ( subCommand ) {
    case LBN_SELCHANGE:
    {
      size_t currpos = GetTreeTool().GetCurSelChildList();
      if ( currpos < GetChildren().size() ) {
        size_t pos = currpos;
        std::set<const c3d::IEmbodimentNode*>::const_iterator itr = GetChildren().begin();
        while ( pos > 0 ) {
          ++itr;
          --pos;
        }
        PArray<c3d::string_t> a1;
        const c3d::MbItemData& data = (*itr)->GetEmbodimentData();
        GetEmbPropStrings( data, a1 );
        GetTreeTool().SetPropListTexts( a1 );
      }
      break;
    }
    case LBN_DBLCLK:
    {
      if ( !IsBlocked() ) {
        size_t currpos = GetTreeTool().GetCurSelChildList();
        if ( currpos < GetChildren().size() ) {
          std::set<const c3d::IEmbodimentNode*>::const_iterator itr = GetChildren().begin();
          while ( currpos > 0 ) {
            ++itr;
            --currpos;
          }
          if ( (*itr)->GetChildren().size() != 0 )
            TestVariables::viewManager->CreateEmbodimentNode( *itr, this );
        }
      }
    } break;
    }
  } break;
  default:
    return false;
  }
  return true;
}

// \ru Обработчик для команд \en Commands handler 
bool NodeEmbodimentWindow::KeyboardCommand( uint command, bool shift, bool ctrl )
{
  switch ( command ) {
  case VK_RETURN:
  {
    if ( ctrl || shift ) {
      GetTreeTool().CloseTreeWindow();
    }
    else {
      if ( !IsBlocked() ) {
        size_t currpos = GetTreeTool().GetCurSelChildList();
        if ( currpos < GetChildren().size() ) {
          std::set<const c3d::IEmbodimentNode*>::const_iterator itr = GetChildren().begin();
          while ( currpos > 0 ) {
            ++itr;
            --currpos;
          }
          if ( ( *itr )->GetChildren().size() != 0 )
            TestVariables::viewManager->CreateEmbodimentNode( *itr, this );
        }
      }
    }
  } break;

  case VK_ESCAPE:
  {
    GetTreeTool().CloseTreeWindow();
  } break;

  default:
    return false;
  }

  return true;
}

// \ru Обработчик для команд \en Commands handler 
void NodeEmbodimentWindow::PreDestroyCommand( bool redraw )
{
  if ( m_child != nullptr ) {
    m_child->GetTreeTool().CloseTreeWindow();
    m_child->PreDestroyCommand( false );
  }
  if ( redraw && ( m_parent != nullptr ) && TestVariables::viewManager->IsMyTree( m_parent ) )
    m_parent->GetTreeTool().SetFocusPropList();

  TestVariables::viewManager->RemoveTree( this );
}

// \ru Заполнить данные диалога \en Populate dialog data
void NodeEmbodimentWindow::Populate( size_t pos )
{
  PArray<c3d::string_t> a;
  for ( std::set<const c3d::IEmbodimentNode*>::const_iterator node = GetChildren().begin(); node != GetChildren().end(); ++node ) {
    a.Add( new c3d::string_t( FormatIDString( ( *node )->GetEmbodimentData().name, m_selection == *node ) ) );
  }

  GetTreeTool().SetChildListTexts( a );
  GetTreeTool().SetCurSelChildList( pos );
}

// \ru Заполнить данные диалога \en Populate dialog data
void NodeEmbodimentWindow::Populate()
{
  size_t currpos = GetTreeTool().GetCurSelChildList();
  Populate( currpos );
};

// \ru Создать диалог узла дерева исполнений \en Create embodiment tree node dialog
void NodeEmbodimentWindow::CreateNodeWindow()
{
  int prop_count = GetDataCount();
  int child_count = ( int )GetChildren().size();

#ifndef __USE_QT__ 
  HWND hprop = 0;
  HWND hchildlist = 0;
  HWND hproplist = 0;
  int butsize, proplistsize;
  if ( CreateEmbTreeClient( hprop, hchildlist, hproplist, butsize, proplistsize, IDS_TITLE_OBJECT, prop_count, child_count ) )
    m_treeTool = CreateEmbTreeTool( hprop, hchildlist, hproplist, butsize, proplistsize );
#else // __USE_QT__ 
  // TODO: implement it
  //QWidget * hprop = nullptr;
  //if (CreateTreeClient(hprop, title, npc))
  //  m_treeTool = CreatePropertyTool(hprop);
#endif // __USE_QT__

  // populate children
  Populate( 0 );

  // populate properties
  PArray<c3d::string_t> a1;
  if ( GetChildren().size() != 0 ) {
    const c3d::MbItemData& data = (*GetChildren().begin() )->GetEmbodimentData();
    GetEmbPropStrings( data, a1 );
    GetTreeTool().SetPropListTexts( a1 );
  }

  GetTreeTool().SetCurSelPropList( 0 );
  GetTreeTool().SetFocusPropList();
  GetTreeTool().Layout();
}
