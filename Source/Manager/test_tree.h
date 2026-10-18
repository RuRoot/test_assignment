#ifndef __TEST_TREE_H
#define __TEST_TREE_H

#include <tool_cstring.h>
#include <reference_item.h>
#include <model_tree_data.h>
#if !defined(__USE_QT__)
  #include <windows.h>
#endif

//------------------------------------------------------------------------------
// \ru Оконная часть диалогов дерева модели
// \en Window part of model tree dialogs
//---
class ITreeTool {
public:
  virtual ~ITreeTool() {}

public:
  // \ru Закрыть это окно \en Destroy this window
  virtual void    CloseTreeWindow() = 0;
  // \ru Проверить, мое ли это окно \en Check if this window is mine
  virtual bool    IsMyWnd ( const void * wndId ) const = 0; 
  // \ru Обработчик размещения \en Layout handler
  virtual void    Layout() = 0; 

  // \ru Обработчики списка свойств объекта \en Handlers of object properties list
  virtual void    SetFocusPropList() = 0;
  virtual void    SetCurSelPropList ( size_t cur ) = 0;
  virtual size_t  GetCurSelPropList() = 0;
  virtual void    SetPropListTexts ( RPArray<c3d::string_t> & a ) = 0;

  // \ru Обработчики списка потомков объекта \en Handlers of object children list
  virtual void    SetChildListText ( size_t cur, const TCHAR* s ) = 0;
  virtual void    SetFocusChildList() = 0;
  virtual void    SetCurSelChildList ( size_t cur ) = 0;
  virtual size_t  GetCurSelChildList() = 0;
  virtual void    SetChildListTexts ( RPArray<c3d::string_t> & a ) = 0;
};

//------------------------------------------------------------------------------
// \ru Базовый класс для диалогов дерева
// \en Base class for tree windows
//---
class IModelTreeWindow : public MbRefItem
{
protected:
  std::unique_ptr<reader_ex> m_reader;
  ITreeTool*                m_treeTool; // \ru оконная часть диалога \en window part of the dialog
  IModelTreeWindow*         m_parent;   // \ru диалог родителя \en parent dialog
  IModelTreeWindow*         m_child;    // \ru диалог потомка \en child dialog

public:
  // \ru Конструкторы // \en Constructors
  IModelTreeWindow() : m_reader(), m_treeTool( nullptr ), m_parent( nullptr ), m_child( nullptr ) {}
  IModelTreeWindow( reader_ex* reader ) : m_reader( reader ), m_treeTool( nullptr ), m_parent( nullptr ), m_child( nullptr ) {}
  // \ru Деструктор // \en Destructor
  virtual ~IModelTreeWindow();

  // \ru Проверить, мое ли это окно \en Check if this window is mine
  bool IsMyWnd( const void * wndId ) const { return ( ( m_treeTool != nullptr ) ? m_treeTool->IsMyWnd( wndId ) : false ); }

  // \ru Обработчики для команд \en Commands handlers 
  virtual void PreDestroyCommand( bool ) {}
  virtual bool TreeCommand( uint, uint, bool ) { return false; }
  virtual bool KeyboardCommand( uint, bool, bool ) { return false; }

  // \ru Получить оконную часть \en Get the window part
  ITreeTool& GetTreeTool() const { PRECONDITION( m_treeTool ); return *m_treeTool; }

  // \ru Установить потомка для объекта \en Set a child for the object
  void SetChild( IModelTreeWindow* child ) { m_child = child; }

  // \ru Обновить список потомков \en Update children list
  void UpdateChildList( size_t pos, const TCHAR* s );

  reader_ex* GetReader() const { return m_reader.get(); }

  // \ru Получить родителя объекта \en Get the parent for the object
  const IModelTreeWindow* GetParent() const { return m_parent; }

protected:

  // \ru Проверить, есть ли диалог потомка \en Check if child dialog exists
  bool IsBlocked() const { return ( m_child != nullptr ); }
};


//------------------------------------------------------------------------------
// \ru Базовый класс для диалогов дерева модели
// \en Base class for model tree windows
//---
class ModelTreeWindow : public IModelTreeWindow
{
protected:
  static std::set<const c3d::IModelTreeNode*>  m_selection; // \ru выбранные объекты \en selected objects

public:
  // \ru Конструкторы // \en Constructors
  ModelTreeWindow() : IModelTreeWindow() {}
  ModelTreeWindow ( reader_ex* reader ) : IModelTreeWindow( reader ) {}
  // \ru Деструктор // \en Destructor
  virtual ~ModelTreeWindow() {}

protected:

  // \ru Выбрать узел, если он еще не выбран, или удалить его из выбранных узлов
  //     Возвращает true, если узел выбран, и false в противном случае  
  // \en Select node if it has not been selected before or remove it if it was already selected
  //     Return true if node added or false otherwise
  bool SelectNode ( const c3d::IModelTreeNode* node );
};

//------------------------------------------------------------------------------
// \ru Диалог дерева модели \en Model tree dialog
//---
class TreeWindow : public ModelTreeWindow
{
private:
  const c3d::IModelTree*     m_obj; // \ru дерево модели \en model tree

public:
  // \ru Конструктор // \en Constructor
  TreeWindow(reader_ex* reader);
  // \ru Деструктор // \en Destructor
  ~TreeWindow();

public:

  // \ru Обработчики для команд \en Commands handlers 
  bool TreeCommand ( uint command, uint subCommand, bool isReturn );
  bool KeyboardCommand ( uint command, bool shift, bool ctrl ); 
  void PreDestroyCommand ( bool redraw ); 

protected:
  // \ru Создать диалог дерева модели \en Create model tree dialog
  void CreateTreeWindow();

  // \ru Прочитать выбранные объекты
  // \en Load selected objects
  void LoadObjects();

  // \ru Показать сообщение о статусе \en Show status message
  virtual void ShowStatus ( const TCHAR* message );

  // \ru Получить корневые узлы дерева \en Get the roots of the model tree
  const std::vector<const c3d::IModelTreeNode*>& GetRoots() const { return m_obj->GetRoots(); }
  const std::vector<const c3d::IModelTreeNode*>& GetRoots() { return m_obj->GetRoots(); }

OBVIOUS_PRIVATE_COPY( TreeWindow )
};

//------------------------------------------------------------------------------
// \ru Диалог узла дерева модели \en Model tree node dialog
//---
class NodeWindow : public ModelTreeWindow
{
private:
  const c3d::IModelTreeNode* m_obj; // model tree node

public:
  // \ru Конструкторы // \en Constructors
  NodeWindow ( const c3d::IModelTreeNode* initObj );
  NodeWindow ( const c3d::IModelTreeNode * initObj, ModelTreeWindow * prnt );
  // \ru Деструктор // \en Destructor
  ~NodeWindow();

public:

  // \ru Обработчики для команд \en Commands handlers 
  bool TreeCommand ( uint command, uint subCommand, bool isReturn );
  bool KeyboardCommand ( uint command, bool shift, bool ctrl );
  void PreDestroyCommand ( bool redraw );

protected:
  // \ru Создать диалог узла дерева модели \en Create model tree node dialog
  void CreateNodeWindow();

  // \ru Количество линий в списке данные узла \en Number of lines in a listbox for node data
  int GetDataCount()  const { return 3 /* type, name, gabatit */ + 2 /* pmin, pmax */; }

  // \ru Получить потомков узла дерева \en Get the children of the tree node
  const std::set<const c3d::IModelTreeNode*>& GetChildren() const { return m_obj->GetChildren(); }
  
OBVIOUS_PRIVATE_COPY(NodeWindow)
};

//------------------------------------------------------------------------------
// \ru Базовый класс для диалогов дерева исполнений
// \en Base class for embodiment tree windows
//---
class IEmbodimentTreeWindow : public IModelTreeWindow
{
protected:
  static const c3d::IEmbodimentNode*  m_selection; // \ru выбранный объект \en selected object

public:
  // \ru Конструкторы // \en Constructors
  IEmbodimentTreeWindow() : IModelTreeWindow() {}
  IEmbodimentTreeWindow( reader_ex* reader ) : IModelTreeWindow( reader ) {}
  // \ru Деструктор // \en Destructor
  virtual ~IEmbodimentTreeWindow() {}

  // \ru Прочитать выбранные объекты \en Load selected objects
  void LoadObjects();

  // \ru Заполнить данные диалога \en Populate dialog data
  virtual void Populate() = 0;
  virtual void Populate( size_t pos ) = 0;

protected:

  // \ru Выбрать узел, если он еще не выбран, или удалить его из выбора.
  //     Возвращает true, если узел выбран, и false в противном случае.
  // \en Select node if it has not been selected before or remove it from selection.
  //     Return true if node added or false otherwise.
  bool SelectNode( const c3d::IEmbodimentNode* node );

  // \ru Показать сообщение о статусе \en Show status message
  virtual void ShowStatus( const TCHAR* message );

};

//------------------------------------------------------------------------------
// \ru Диалог дерева исполнений \en Embodiment tree dialog
//---
class EmbodimentTreeWindow : public IEmbodimentTreeWindow
{
private:
  const c3d::IEmbodimentTree* m_emb; // \ru дерево исполнений \en embodiment tree

public:
  // \ru Конструктор // \en Constructor
  EmbodimentTreeWindow( reader_ex* reader );
  // \ru Деструктор // \en Destructor
  ~EmbodimentTreeWindow();

public:

  // \ru Обработчики для команд \en Commands handlers 
  bool TreeCommand( uint command, uint subCommand, bool isReturn );
  bool KeyboardCommand( uint command, bool shift, bool ctrl );
  void PreDestroyCommand( bool redraw );

  // \ru Заполнить данные диалога \en Populate dialog data
  void Populate( size_t pos );
  void Populate();

protected:
  // \ru Создать диалог дерева исполнений \en Create Embodiment tree dialog
  void CreateTreeWindow();

  // \ru Количество линий в списке данных узла \en Number of lines in a listbox for node data
  int GetDataCount()  const { return 2 /* identifier, gabatit */ + 2 /* pmin, pmax */; }

  // \ru Получить корневые узлы дерева \en Get the roots of the model tree
  const std::vector<const c3d::IEmbodimentNode*>& GetRoots() const { return m_emb->GetRoots(); }
  const std::vector<const c3d::IEmbodimentNode*>& GetRoots() { return m_emb->GetRoots(); }

  OBVIOUS_PRIVATE_COPY( EmbodimentTreeWindow )
};

//------------------------------------------------------------------------------
// \ru Диалог узла дерева исполнений \en Embodiment tree node dialog
//---
class NodeEmbodimentWindow : public IEmbodimentTreeWindow
{
private:
  const c3d::IEmbodimentNode*  m_node; // \ru Узел дерева исполнений \en Embodiment tree node

public:
  // \ru Конструкторы // \en Constructors
  NodeEmbodimentWindow( const c3d::IEmbodimentNode * initObj, IModelTreeWindow * prnt );
  // \ru Деструктор // \en Destructor
  ~NodeEmbodimentWindow();

  // \ru Обработчики для команд \en Commands handlers 
  bool TreeCommand( uint command, uint subCommand, bool isReturn );
  bool KeyboardCommand( uint command, bool shift, bool ctrl );
  void PreDestroyCommand( bool redraw );

  // \ru Заполнить данные диалога \en Populate dialog data
  void Populate( size_t pos );
  void Populate();

protected:
  // \ru Создать диалог узла дерева модели \en Create model tree node dialog
  void CreateNodeWindow();

  // \ru Количество линий в списке данных узла \en Number of lines in a listbox for node data
  int GetDataCount()  const { return 2 /* identifier, gabatit */ + 2 /* pmin, pmax */; }

  // \ru Получить потомков узла дерева \en Get the children of the tree node
  const std::set<const c3d::IEmbodimentNode*>& GetChildren() const { return m_node->GetChildren(); }

  OBVIOUS_PRIVATE_COPY( NodeEmbodimentWindow )
};

// \ru Форматировать строку для типа объекта с учетом его выбора
// \en Format type string considering its selection
c3d::string_t FormatTypeString ( MbeSpaceType type, bool selected );

// \ru Форматировать строку для атрибута объекта с учетом его выбора
// \en Format attribute string considering its selection
c3d::string_t FormatAttrString ( c3d::MbeItemDataType type, bool selected );

#ifndef __USE_QT__ 
// \ru Создать оконную часть диалога дерева модели \en Create window part of model tree dialog
ITreeTool * CreateTreeTool ( HWND hprop, HWND hlist, HWND hedit, int butsize, int proplistsize );
// \ru Создать оконную часть диалога дерева исполнений \en Create window part of embodiment tree dialog
ITreeTool * CreateEmbTreeTool ( HWND hprop, HWND hlist, HWND hedit, int butsize, int proplistsize );
// \ru Создать диалог дерева модели \en Create model tree dialog
bool CreateTreeClient ( HWND & hprop, HWND& hchildlist, HWND& hproplist, int& butsize, int& proplistsize, const TCHAR * title, int prop_count, int child_count );
// \ru Создать диалог дерева исполнений \en Create embodiment tree dialog
bool CreateEmbTreeClient ( HWND & hprop, HWND& hchildlist, HWND& hproplist, int& butsize, int& proplistsize, const TCHAR * title, int prop_count, int child_count );
#else // __USE_QT__ 
// TODO: implement it
//class QWidget;
//typedef QWidget * QWidgetPtr;
//ITreeTool* CreateTreeTool( QWidget * hprop );
//bool CreateTreeClient( QWidgetPtr & hprop, const TCHAR * title, int count );
#endif // __USE_QT__ 

#endif//__TEST_TREE_H