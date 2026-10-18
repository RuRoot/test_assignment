#ifndef __TEST_TREE_FILTER_H
#define __TEST_TREE_FILTER_H

#include <tool_cstring.h>
#include <reference_item.h>
#include <model_tree_data.h>
#if !defined(__USE_QT__)
  #include <windows.h>
#endif


//------------------------------------------------------------------------------
// \ru Оконная часть диалогов фильтров
// \en Window part of filters dialogs
//---
class IObjectFilterTool
{
public:
  virtual ~IObjectFilterTool() {}

  // \ru Закрыть это окно \en Destroy this window
  virtual void    CloseWindow() = 0;
  // \ru Проверить, мое ли это окно \en Check if this window is mine
  virtual bool    IsMyWnd ( const void * wndId ) const = 0;
  // \ru Обработчик размещения \en Layout handler
  virtual void    Layout() = 0; 

  // \ru Обработчики списка \en List handlers
  virtual void    SetFocusList() = 0;
  virtual void    SetCurSelList ( size_t cur ) = 0;
  virtual size_t  GetCurSelList() = 0;
  virtual void    SetListTexts( RPArray<c3d::string_t>& a, RPArray<c3d::string_t>* a2 = nullptr ) = 0;
  virtual void    SetListText( size_t cur, const TCHAR* s, const TCHAR* s2 = nullptr ) = 0;

  // \ru Обработчики строки редактирования \en Handlers of editbox
  virtual void    SetEditText ( const TCHAR * s ) = 0;
  virtual void    GetEditText ( TCHAR * s, int len ) = 0;
  virtual void    SelectEditText() = 0;
  virtual void    SetFocusEdit() = 0;
  virtual void    ShowEditItem ( bool show ) = 0;
};

//------------------------------------------------------------------------------
// \ru Базовый класс для диалогов фильтров
// \en Base class for filters dialogs
//---
class ObjectFilterWindow : public MbRefItem
{
protected:
  std::unique_ptr<reader_ex> m_reader; // \ru читатель модели \en model reader
  IObjectFilterTool*       m_tool;   // \ru оконная часть диалога \en window part of the dialog

public:
  ObjectFilterWindow() : m_reader(), m_tool(nullptr) {}
  ObjectFilterWindow ( reader_ex* reader ) : m_reader(reader), m_tool(nullptr) {}
  virtual ~ObjectFilterWindow() { if (m_tool) delete m_tool; }

  // \ru Получить оконную часть \en Get the window part
  IObjectFilterTool& GetTool() const { PRECONDITION(m_tool); return *m_tool; }

  // \ru Проверить, мое ли это окно \en Check if this window is mine
  bool IsMyWnd ( const void * wndId ) const { return ((m_tool != nullptr) ? m_tool->IsMyWnd(wndId) : false); }

  // \ru Обработчики для команд \en Commands handlers 
  virtual bool    FilterCommand ( uint command, uint subCommand, bool isReturn );
  virtual bool    KeyboardCommand ( uint command, bool shift, bool ctrl );
  virtual void    PreDestroyCommand ( bool redraw );

  // \ru Обновить список \en Update the list
  void UpdateList( size_t pos, const TCHAR* s );

  // \ru Показать сообщение о статусе \en Show status message
  virtual void ShowStatus ( const TCHAR* message );

  // \ru Прочитать объекты по фильтрам
  // \en Load objects by filters
  void    LoadObjects ( std::vector<c3d::MbItemData>& filters );
};

//------------------------------------------------------------------------------
// \ru Диалог фильтров по типу \en Type filter dialog
//---
class ObjectTypeWindow : public ObjectFilterWindow
{
  std::vector<MbeSpaceType> m_types;  // \ru типы, доступные для выбора \en available types
  std::set<MbeSpaceType> m_selection; // \ru выбранные типы \en selected types
public:
  ObjectTypeWindow ( reader_ex* reader );

  // \ru Обработчик для команд \en Commands handler
  bool    FilterCommand ( uint command, uint subCommand, bool isReturn );

  // \ru Выбрать тип объекта, если он еще не выбран, или удалить его из выбранных типов
  //     Возвращает true, если тип выбран, и false в противном случае  
  // \en Select an object type if it has not been selected before or remove it if it was already selected
  //     Return true if the type added or false otherwise
  bool    SelectType ( MbeSpaceType type );

  // \ru Прочитать выбранные объекты
  // \en Load selected objects
  void    LoadObjects();
  
protected:
  // \ru Создать диалог фильтров по типу \en Create filter dialog
  void CreateTypeWindow();

  OBVIOUS_PRIVATE_COPY(ObjectTypeWindow)
};

//------------------------------------------------------------------------------
// \ru Диалог фильтров по габариту \en Gabarit filter dialog
//---
class ObjectGabaritWindow : public ObjectFilterWindow
{

  // \ru Форматирование поля габарита
  // \en Formatter of the gabarit field
  struct ValueFormatter
  {
    c3d::string_t m_name;
    double*       m_value;

    // \ru Конструкторы // \en Constructors
    ValueFormatter();
    ValueFormatter ( const TCHAR* str, double* val ) :m_name(str), m_value(val) {}
    ValueFormatter ( const ValueFormatter& val ) :m_name(val.m_name), m_value(val.m_value) {}

    // \ru Форматирование поля \en Format of the field
    c3d::string_t Format();

    // \ru Форматирование значения поля \en Format the field value
    c3d::string_t FormatValue();
  };

  // \ru Соответствие индекса поля габарита в списке и его форматера
  // \en Map: [field index in the list] -> [field formatter]
  std::map<size_t, ValueFormatter> m_values;
  // \ru Значение габарита \en Gabarit value
  MbCube m_gabarit;

public:
  ObjectGabaritWindow ( reader_ex* reader );

  // \ru Обработчик для команд \en Commands handler
  bool    FilterCommand ( uint command, uint subCommand, bool isReturn );

  // \ru Прочитать выбранные объекты
  // \en Load selected objects
  void    LoadObjects();
  
protected:
  // \ru Создать диалог фильтров по габариту \en Create gabarit dialog
  void CreateGabaritWindow();

  // \ru Количество линий в списке данных габарита \en Number of lines in a listbox for gabarit
  int GetDataCount()  const { return 9 /* gabatit + pmin + (x, y, z) + pmax + (x, y, z) */; }

  // \ru Показать сообщение о статусе \en Show status message
  virtual void ShowStatus ( const TCHAR* message );

  OBVIOUS_PRIVATE_COPY(ObjectGabaritWindow)
};

//------------------------------------------------------------------------------
// \ru Диалог фильтров по атрибуту \en Attribute filter dialog
//---
class ObjectAttrWindow : public ObjectFilterWindow
{
  std::vector<c3d::MbeItemDataType>                 m_types;     // \ru типы, доступные для выбора \en available types
  std::map<c3d::MbeItemDataType,c3d::ItemDataBase*> m_selection; // \ru выбранные типы и данные \en selected types and data

public:
  ObjectAttrWindow ( reader_ex* reader );
  ~ObjectAttrWindow();

  // \ru Обработчик для команд \en Commands handler
  bool    FilterCommand ( uint command, uint subCommand, bool isReturn );

  // \ru Выбрать тип, если он еще не выбран, или удалить его из выбранных типов
  // Возвращает объект данных, или nullptr в случае удаления типа.
  // \en Select the type, if it is not selected, or delete it from the selected types.
  // Return the type data, or nullptr if the type is deleted from the selected types.
  c3d::ItemDataBase* SelectType ( c3d::MbeItemDataType type );

  // \ru Обновить список \en Update the list
  void UpdateList( size_t pos, const TCHAR* s, const TCHAR* s2 );

  // \ru Прочитать выбранные объекты
  // \en Load selected objects
  void    LoadObjects();

protected:
  // \ru Создать диалог фильтров по типу \en Create filter dialog
  void CreateAttrWindow();

  // \ru Показать сообщение о статусе \en Show status message
  virtual void ShowStatus ( const TCHAR* message );

  OBVIOUS_PRIVATE_COPY(ObjectAttrWindow)
};

#ifndef __USE_QT__ 
// \ru Создать диалог фильтра по типу \en Create type filter dialog
bool CreateTypeClient( HWND& hwin, HWND& hlist, int& listSize, int& listStart, const TCHAR * title, int count );
// \ru Создать оконную часть диалога фильтра по типу \en Create window part of type filter dialog
IObjectFilterTool* CreateTypeTool( HWND hwin, HWND hlist, int start, int listY );

// \ru Создать диалог фильтра по габариту \en Create gabarit filter window
bool CreateGabaritClient( HWND& hwin, HWND& hedit, HWND& hlist, int& listStart, int& listSize, const TCHAR * title, int count );
// \ru Создать оконную часть диалога фильтра по габариту \en Create window part of gabarit filter dialog
IObjectFilterTool* CreateGabaritTool( HWND hwin, HWND& hedit, HWND hlist, int start, int listY );

// \ru Создать диалог фильтра по атрибуту \en Create attribute filter dialog
bool CreateAttrClient( HWND& hwin, HWND& hedit, HWND& hlist, HWND& hlist2, int& listStart, int& listSize, int& part, const TCHAR * title, int count );
// \ru Создать оконную часть диалога фильтра по атрибуту \en Create window part of attribute filter dialog
IObjectFilterTool* CreateAttrTool( HWND hwin, HWND& hedit, HWND hlist, HWND hlist2, int start, int listY, int part );

#else // __USE_QT__ 
// TODO: implement it
#endif // __USE_QT__ 

#endif//__TEST_TREE_FILTER_H