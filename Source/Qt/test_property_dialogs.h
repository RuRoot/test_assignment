#ifndef __TEST_PROPERTY_DIALOGS_H
#define __TEST_PROPERTY_DIALOGS_H

#include <tool_cstring.h>
#include <templ_rp_array.h>
#include <QDialog>

QT_BEGIN_NAMESPACE
class QCloseEvent;
class QResizeEvent;
class QKeyEvent;
class QLineEdit;
class QListWidget;
QT_END_NAMESPACE

//----------------------------------------------------------------------------------------
// Окно свойств.
// ---
class PropertyDialog : public QDialog
{
  Q_OBJECT

public:
  QPushButton * setButton;
  QPushButton * setexitButton;
  QPushButton * exitButton;
  QLineEdit *   hedit;
  QListWidget * hlist;

public:
  PropertyDialog( const TCHAR* title, int count );
  void layoutCtrls();

public Q_SLOTS:
          void    slot_BUTTON_SET     ();
          void    slot_BUTTON_SET_EXIT();
          void    slot_BUTTON_EXIT    ();
          void    slot_LIST_SELCHANGE ();
          void    slot_LIST_DBLCLK    ();
          void    slot_EDIT_UPDATE    ();

public:
  virtual void    closeEvent   ( QCloseEvent  * event );
  virtual void    resizeEvent  ( QResizeEvent * event );
  virtual void    keyPressEvent( QKeyEvent    * event );

public:
          void    SetFocusList();
          void    SetCurSelList( size_t cur );
          size_t  GetCurSelList();
          void    SetListText( size_t cur, const TCHAR* s );
          void    SetListTexts( RPArray<c3d::string_t>& a );
          void    SetFocusEdit();
          void    SetEditText( const TCHAR* s );
          void    GetEditText( TCHAR* s, int len );
          void    SelectEditText();
};

#endif // __TEST_PROPERTY_DIALOGS_H
