#ifdef __USE_QT__ 

#pragma warning(disable: 4127)
#pragma warning(disable: 4512)
#pragma warning(disable: 4100)

#include "test_property_dialogs.h"
#include <tool_cstring.h>
#include <test_manager.h>
#include <test_property.h>
#include <test_variables.h>
#include <test_service.h>
#include <test_gr_draw.h>
#include <test.h>

#include <qevent.h>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>

#include <math_namespace.h>
#include <last.h>

using namespace c3d;
using namespace TestVariables;


#define BUTTON_HEIGHT 20 // Высота кнопок
#define EDIT_HEIGHT 24

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PropertyDialog::PropertyDialog( const TCHAR* title, int count )
  : QDialog ( qMainWnd )
  , setButton    ( new QPushButton( QString_fromT( IDS_TITLE_WRITE ), this ) )
  , setexitButton( new QPushButton( QString_fromT( IDS_TITLE_APPLY ), this ) )
  , exitButton   ( new QPushButton( QString_fromT( IDS_TITLE_CANCEL ), this ) )
  , hedit        ( new QLineEdit( this ) )
  , hlist        ( new QListWidget( this ) )
{
  connect(setButton,     SIGNAL(clicked()), this, SLOT(slot_BUTTON_SET()     ));
  connect(setexitButton, SIGNAL(clicked()), this, SLOT(slot_BUTTON_SET_EXIT()));
  connect(exitButton,    SIGNAL(clicked()), this, SLOT(slot_BUTTON_EXIT()    ));

  connect(hlist,         SIGNAL(currentRowChanged(int)),              this, SLOT(slot_LIST_SELCHANGE() ));
  connect(hlist,         SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slot_LIST_DBLCLK() ));
  connect(hedit,         SIGNAL(returnPressed()),                     this, SLOT(slot_EDIT_UPDATE() ));

  resize( 359, BUTTON_HEIGHT*2+1+EDIT_HEIGHT+1+EDIT_HEIGHT*(count)+1 ); 

  setWindowTitle( QString_fromT(title) );
  setModal( false );

  layoutCtrls();
}

//------------------------------------------------------------------------------
//
// ---
void PropertyDialog::layoutCtrls() {        
  int dx = width() / 3;

  setButton->move  ( 0, 0 );
  setButton->resize( dx, BUTTON_HEIGHT );

  setexitButton->move  ( dx + 1, 0 );
  setexitButton->resize( dx, BUTTON_HEIGHT );

  exitButton->move  ( 2 * dx + 2, 0 );
  exitButton->resize( dx, BUTTON_HEIGHT );

  hedit->move  ( 1, BUTTON_HEIGHT + 1 );
  hedit->resize( width() - 1, EDIT_HEIGHT );

  hlist->move  ( 1, BUTTON_HEIGHT + 1 + EDIT_HEIGHT + 1 );
  hlist->resize( width() - 1, height() - hlist->y() - 1 );
}

//------------------------------------------------------------------------------
//
// ---
void PropertyDialog::slot_BUTTON_SET() {        
  PropertyWindow * wp = viewManager->FindProperty( (void*)this ); // Выдать свойство
  if ( wp ) 
    wp->PropertyCommand( BUTTON_SET, 0, false );
}

//------------------------------------------------------------------------------
//
// ---
void PropertyDialog::slot_BUTTON_SET_EXIT() {  
  PropertyWindow * wp = viewManager->FindProperty( (void*)this ); // Выдать свойство
  if ( wp ) 
    wp->PropertyCommand( BUTTON_SET_EXIT, 0, false );
}

//------------------------------------------------------------------------------
//
// ---
void PropertyDialog::slot_BUTTON_EXIT() {  
  PropertyWindow * wp = viewManager->FindProperty( (void*)this ); // Выдать свойство
  if ( wp ) 
    wp->PropertyCommand( BUTTON_EXIT, 0, false );
}

//------------------------------------------------------------------------------
//
// ---
void PropertyDialog::slot_LIST_SELCHANGE() {
  PropertyWindow * wp = viewManager->FindProperty( (void*)this ); // Выдать свойство
  if ( wp ) 
    wp->PropertyCommand( H_LIST, LBN_SELCHANGE, false );
}

//------------------------------------------------------------------------------
//
// ---
void PropertyDialog::slot_LIST_DBLCLK() {
  PropertyWindow * wp = viewManager->FindProperty( (void*)this ); // Выдать свойство
  if ( wp ) 
    wp->PropertyCommand( H_LIST, LBN_DBLCLK, false );
}

//------------------------------------------------------------------------------
//
// ---
void PropertyDialog::slot_EDIT_UPDATE () {
  PropertyWindow * wp = viewManager->FindProperty( (void*)this ); // Выдать свойство
  if ( wp ) 
    wp->PropertyCommand( H_EDIT, EN_UPDATE, true );
}

//------------------------------------------------------------------------------
//
// ---
void PropertyDialog::closeEvent( QCloseEvent *event )
{
  PRECONDITION( event );

  PropertyWindow * wp = viewManager->FindProperty( (void*)this ); 
  if ( wp )
  {
    if ( wp->IsBlocked() )
      event->ignore();
    else {
      wp->PreDestroyCommand( true ); // Отцепиться от менеджера
      if ( viewManager ) {
        WorkWindow* activeWindow = viewManager->GetActiveWindow();
        if ( activeWindow )
          activeWindow->GetDrawTool().SetWindowFocus( true/*mdiActivate*/ );
      }
      event->accept();
    }
  }
}

//------------------------------------------------------------------------------
//
// ---
void PropertyDialog::resizeEvent( QResizeEvent * event )
{
  PRECONDITION( event );

  PropertyWindow * wp = viewManager->FindProperty( (void*)this ); // Выдать свойство
  if ( wp ) 
    wp->GetPropTool().Layout();

  event->accept();
}

//------------------------------------------------------------------------------
// Обработать событие от клавиатуры
// ---
void PropertyDialog::keyPressEvent( QKeyEvent *event )
{
  PRECONDITION( event );
 
  switch ( event->key() ) {
    case Qt::Key_Enter : {
      PropertyWindow * wp = viewManager->FindProperty( (void*)this ); // Выдать свойство
      if ( wp &&
           wp->KeyboardCommand( VK_RETURN, (event->modifiers() & Qt::ControlModifier) != 0, (event->modifiers() & Qt::ShiftModifier) != 0 ) ) {
        event->ignore();
        return;
      }
    } break;

    case Qt::Key_Escape : {
      PropertyWindow * wp = viewManager->FindProperty( (void*)this ); // Выдать свойство
      if ( wp &&
           wp->KeyboardCommand( VK_ESCAPE, (event->modifiers() & Qt::ControlModifier) != 0, (event->modifiers() & Qt::ShiftModifier) != 0 ) ) {
        event->ignore();
        return;
      }
    } break;
  } // switch

  event->accept();
}

//------------------------------------------------------------------------------
// 
// ---
void PropertyDialog::SetFocusList() { 
  hlist->setFocus(); 
}

//------------------------------------------------------------------------------
// 
// ---
void PropertyDialog::SetCurSelList( size_t cur ) { 
  hlist->setCurrentRow( (int)cur ); 
}

//------------------------------------------------------------------------------
// 
// ---
size_t PropertyDialog::GetCurSelList() { 
  return (size_t)hlist->currentRow(); 
}

//------------------------------------------------------------------------------
// 
// ---
void PropertyDialog::SetListText( size_t cur, const TCHAR* s )  {
  QListWidgetItem* item = hlist->item ( (int)cur );
  if ( item )
    item->setText( QString_fromT( s ) );
}

//------------------------------------------------------------------------------
// 
// ---
void PropertyDialog::SetListTexts( RPArray<string_t>& a ) {
  hlist->clear();
  for ( size_t i = 0, c = a.Count(); i < c; ++i )
    hlist->addItem( QString_fromT(a[i]->c_str()) );
}

//------------------------------------------------------------------------------
// 
// ---
void PropertyDialog::SetFocusEdit() { 
  hedit->setFocus(); 
}

//------------------------------------------------------------------------------
// 
// ---
void PropertyDialog::SetEditText( const TCHAR* s ) { 
  hedit->setText( QString_fromT( s ) ); 
}   

//------------------------------------------------------------------------------
// 
// ---
void PropertyDialog::GetEditText( TCHAR* s, int len ) { 
  QString ss = hedit->text(); 
#ifdef  _UNICODE
  _tcsncpy( s, (const TCHAR*)ss.utf16(), len );
#else // _UNICODE
  #ifdef _MSC_VER
    _tcsncpy( s, ss.toLatin1().data(), len );
  #else  // _MSC_VER
    _tcsncpy( s, ss.toUtf8().data(), len );
  #endif // _MSC_VER  
#endif // _UNICODE
}  

//----------------------------------------------------------------------------------------
// 
//---
void PropertyDialog::SelectEditText() { 
  hedit->selectAll(); 
}

//----------------------------------------------------------------------------------------
// Связка с оконной частью редактор свойств
//---
class QtPropTool : public IPropTool
{
private:
  QWidget*  hprop;

public:
  QtPropTool( QWidget* _hprop )
    : hprop ( _hprop )
  {}

  virtual ~QtPropTool() {
  }

public:
  virtual void     ClosePropertyWindow()                      { hprop->close(); }
  virtual bool     IsMyWnd( const void * wndId ) const        { return (QWidget*)wndId == hprop; }
  virtual void     Layout()                                   { qobject_cast<PropertyDialog *>(hprop)->layoutCtrls();  }
  virtual void     SetFocusList()                             { qobject_cast<PropertyDialog *>(hprop)->SetFocusList(); }
  virtual void     SetCurSelList( size_t cur )                { qobject_cast<PropertyDialog *>(hprop)->SetCurSelList(cur); }
  virtual size_t   GetCurSelList()                            { return qobject_cast<PropertyDialog *>(hprop)->GetCurSelList(); }
  virtual void     SetListText( size_t cur, const TCHAR* s )  { qobject_cast<PropertyDialog *>(hprop)->SetListText( cur, s ); }
  virtual void     SetListTexts( RPArray<string_t>& a )       { qobject_cast<PropertyDialog *>(hprop)->SetListTexts( a ); }
  virtual void     SetFocusEdit()                             { qobject_cast<PropertyDialog *>(hprop)->SetFocusEdit(); }
  virtual void     SetEditText( const TCHAR* s )              { qobject_cast<PropertyDialog *>(hprop)->SetEditText( s ); }   
  virtual void     GetEditText( TCHAR* s, int len )           { qobject_cast<PropertyDialog *>(hprop)->GetEditText( s, len ); }
  virtual void     SelectEditText()                           { qobject_cast<PropertyDialog *>(hprop)->SelectEditText(); }
};


/////////////////////////////////////////////////////////////////////////////////////////
//
//  Creators
//
/////////////////////////////////////////////////////////////////////////////////////////

bool CreatePropertyClient( QWidgetPtr& hprop, const TCHAR* title, int count )
{
  PropertyDialog* dlg = new PropertyDialog( title, count );
  dlg->show(); 

  hprop = dlg;
  return true;
}

IPropTool* CreatePropertyTool( QWidget* hprop )
{
  return new QtPropTool( hprop );
}


#endif // __USE_QT__ 
