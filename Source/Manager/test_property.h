#ifndef __TEST_PROPERTY_H
#define __TEST_PROPERTY_H


#include <mb_property.h>
#include <tool_cstring.h>
#include <test_variables.h>
#include <test_prompt.h>
#include <model_entity.h>

//------------------------------------------------------------------------------
// Связка с оконной частью редактор свойств
//---
class IPropTool {
public:
  virtual ~IPropTool() {}

public:
  virtual void    ClosePropertyWindow() = 0;
  virtual bool    IsMyWnd( const void * wndId ) const = 0; 
  virtual void    Layout() = 0; 
  virtual void    SetFocusList() = 0;
  virtual void    SetCurSelList( size_t cur ) = 0;  
  virtual size_t  GetCurSelList() = 0;  
  virtual void    SetListText( size_t cur, const TCHAR * s ) = 0;  
  virtual void    SetListTexts( RPArray<c3d::string_t> & a ) = 0;  
  virtual void    SetFocusEdit() = 0;
  virtual void    SetEditText( const TCHAR * s ) = 0;  
  virtual void    GetEditText( TCHAR * s, int len ) = 0;  
  virtual void    SelectEditText() = 0;  
};


//------------------------------------------------------------------------------
// Редактор свойств графического объекта
//---
class PropertyWindow : public MbRefItem {
private:
  IPropTool *      propTool;   // Связка с оконной частью
  PropertyWindow * parent;     // Указатель на родителя  
  MbProperty  *    parentProp; // Родительсткое свойство (герантируется существованием родительского окна)
  PropertyWindow * child;      // Указатель на потомка

  void *           obj;        // Объект
  PrePropType      type;       // Тип свойства в окне
  MbProperties     properties; // Свойства объекта
  bool             changeable; // Признак редактируемости
  bool             changed;    // Свойства изменены

public:
  template<class _Item>
  PropertyWindow( _Item * initObj, bool isChangeable );
 ~PropertyWindow();

private:
  PropertyWindow( MbProperty & rootProp, PropertyWindow * prnt ); 

public:
  bool                 IsBlocked()    const { return (child != nullptr); }

  const void         * GetObj() const { return obj; }
        void         * SetObj()       { return obj; }

  const PropertyWindow * GetParent() const { return parent; }
  void                 SetOwnProperties(); // Записать новые свойства в объект

  IPropTool &          GetPropTool() const { PRECONDITION(propTool); return *propTool; }
  PrePropType          GetPropType() const { return type; }
  PropertyWindow *     CreateProperty( size_t index ); // Редактировать свойство

  bool                 IsMyWnd( const void * wndId ) const { return ((propTool != nullptr) ? propTool->IsMyWnd( wndId ) : false); }

  bool                 PropertyCommand( uint command, uint subCommand, bool isReturn ); 
  bool                 KeyboardCommand( uint command, bool shift, bool ctrl ); 
  void                 PreDestroyCommand( bool redraw ); 

protected:
  void                 CreatePropertyWindow();  // Создать окно свойств

  void                 SetChanged();
  bool                 IsChangeable() const { return changeable; }

  const MbProperties & GetProperties() const { return properties; }
        MbProperties & Properties()          { return properties; }  
        size_t         GetPropCount()  const { return properties.Count(); }

  void                 SetString( TCHAR * s ); // Записать новое свойство по индексу в списке
  void                 SetBoolean( bool & b ); // Записать новое свойство по индексу в списке
  void                 SetProperties();        // Записать новые свойства в объект
  void                 ChangeProperty();       // Сменить текст в редакторе при изменении позиции в листе 
  void                 EditProperty();         // Редактировать свойство 
  void                 HighlightObj( MbProperty & prop, TCHAR * s ); // Выделить текущий объект в списке

OBVIOUS_PRIVATE_COPY( PropertyWindow ) // Не реализовано
};

//----------------------------------------------------------------------------------------
// Отдельный случай сборки свойств для типа MbTransactions.
//---
inline void _GetProperties( MbTransactions * item, MbProperties & props )
{
  item->GetProperties( props, TestVariables::sameCreators );
}

//----------------------------------------------------------------------------------------
// Отдельный случай сборки свойств для типа MbModel.
//---
inline void _GetProperties( MbModel * item, MbProperties & props )
{
  item->GetProperties( props );

  c3d::string_t fileName( TestVariables::nameBuffer );
  if ( !fileName.empty() ) { // Добавить имя файла модели
    TCHAR sep = _T('\\');
    size_t pos = fileName.rfind( sep );

    if ( pos != SYS_MAX_T )
      fileName.erase( 0, pos+1 );

    props.Add( new StringProperty( IDS_PROP_0000, IDS_INFO_FILENAME, false ) );  
    props.Add( new StringProperty( IDS_PROP_0000, fileName.c_str(), false ) );  
  }  
}

//----------------------------------------------------------------------------------------
// Общий случай для типа с функцией GetProperties (специализации выше)
//---
template<class _Item>
inline void _GetProperties( _Item * item, MbProperties & props )
{
  item->GetProperties( props );
}

//----------------------------------------------------------------------------------------
// Конструктор
// ---
template<class _Item>
PropertyWindow::PropertyWindow( _Item * initObj, bool isChangeable ) 
  : MbRefItem()
  , propTool( nullptr )
  , parent( nullptr )
  , parentProp( nullptr )
  , child( nullptr )
  , obj( initObj )
  , type( PropType<_Item>::propId )
  , properties()
  , changeable( isChangeable )
  , changed( false )
{
  PRECONDITION( obj != nullptr );
  if ( initObj ) 
  {    
    _GetProperties( initObj, properties );
  }

  CreatePropertyWindow(); // Собственно создание оконной части
}


//----------------------------------------------------------------------------------------
// Создание редактора свойств объекта
//---
#ifndef __USE_QT__ 
IPropTool * CreatePropertyTool( HWND hprop, HWND hlist, HWND hedit );
bool CreatePropertyClient( HWND & hprop, HWND& hlist, HWND & hedit, const TCHAR * title, int count );
#else // __USE_QT__ 
class QWidget;
typedef QWidget * QWidgetPtr;
IPropTool* CreatePropertyTool( QWidget * hprop );
bool CreatePropertyClient( QWidgetPtr & hprop, const TCHAR * title, int count );
#endif // __USE_QT__ 


#endif // __TEST_PROPERTY_H
