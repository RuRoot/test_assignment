#ifdef __USE_QT__ 

#pragma warning(disable: 4127)
#pragma warning(disable: 4512)
#pragma warning(disable: 4100)

#include "test_main_dialogs.h"
#include "test_main_window.h"
#include <tool_time_test.h>
#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <test_variables.h>

#include <QGridLayout>
#include <QLabel>
#include <QInputDialog>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QColorDialog>

#include <math_namespace.h>
#include <last.h>

using namespace c3d;
using namespace TestVariables;


//------------------------------------------------------------------------------
// 
// ---
QWidget* ParentForDialog() {
  return qMainWnd;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Ввод строки
// ---
bool GetString( const TCHAR* nPrompt, const TCHAR * defstr, TCHAR * result, size_t len )
{
  QInputDialog dialog( ParentForDialog(), Qt::Tool/*Tip*/ | Qt::Window );
  dialog.setWindowTitle( QString_fromT( TITLE_MAIN_WND ) );
  dialog.setLabelText( QString_fromT( nPrompt ) );
  dialog.setTextValue( QString_fromT( defstr ) );

  dialog.resize( 350, dialog.height() );
  dialog.move( 40, 85 );
  dialog.setSizeGripEnabled( true );
  if ( dialog.exec() )
  {
#ifdef  _UNICODE
    _tcsncpy( result, (const TCHAR*)dialog.textValue().utf16(), len );
#else // _UNICODE
  #ifdef _MSC_VER
    _tcsncpy( result, (const TCHAR*)dialog.textValue().toLatin1().data(), len );
  #else  // _MSC_VER
    _tcsncpy( result, (const TCHAR*)dialog.textValue().toUtf8().data(), len );
  #endif // _MSC_VER  
#endif // _UNICODE
    return true;
  }
  else
    return false;
} 


//------------------------------------------------------------------------------
// Ввод целого числа
// ---
bool GetInt( const TCHAR* nPrompt, int & iresult ) {
  QInputDialog dialog( ParentForDialog(), Qt::Tool/*Tip*/ | Qt::Window );
  dialog.setWindowTitle( QString_fromT( TITLE_MAIN_WND ) );
  dialog.setLabelText( QString_fromT( nPrompt ) );
  dialog.setIntRange ( SYS_MIN_INT32, SYS_MAX_INT32 );
  dialog.setIntStep( 1 );
  dialog.setIntValue( iresult );

  dialog.resize( 350, dialog.height() );
  dialog.move( 40, 85 );
  dialog.setSizeGripEnabled( true );
  if ( dialog.exec() )
  {
    iresult = dialog.intValue();
    return true;
  }
  else
    return false;
}


//------------------------------------------------------------------------------
// Ввод целого числа
// ---
#if defined(PLATFORM_64)
bool GetInt( const TCHAR* nPrompt, ptrdiff_t & iresult ) {
  QInputDialog dialog( ParentForDialog(), Qt::Tool/*Tip*/ | Qt::Window );
  dialog.setWindowTitle( QString_fromT( TITLE_MAIN_WND ) );
  dialog.setLabelText( QString_fromT( nPrompt ) );
  dialog.setIntRange ( SYS_MIN_INT32, SYS_MAX_INT32 );
  dialog.setIntStep( 1 );
  dialog.setIntValue( (int)iresult );

  dialog.resize( 350, dialog.height() );
  dialog.move( 40, 85 );
  dialog.setSizeGripEnabled( true );
  if ( dialog.exec() )
  {
    iresult = (ptrdiff_t)dialog.intValue();
    return true;
  }
  else
    return false;
}
#endif // PLATFORM_64


//------------------------------------------------------------------------------
// Ввод вещественного числа
// ---
bool GetFloat( const TCHAR* nPrompt, float & fresult ) {
  QInputDialog dialog( ParentForDialog(), Qt::Tool/*Tip*/ | Qt::Window );
  dialog.setWindowTitle( QString_fromT( TITLE_MAIN_WND ) );
  dialog.setLabelText( QString_fromT( nPrompt ) );
  dialog.setDoubleRange ( (double)SYS_MIN_INT32, (double)SYS_MAX_INT32 );
  dialog.setDoubleDecimals( fabs((double)fresult) < 0.001 ? 3 : 9 );
  dialog.setDoubleValue( (double)fresult );

  dialog.resize( 350, dialog.height() );
  dialog.move( 40, 85 );
  dialog.setSizeGripEnabled( true );
  if ( dialog.exec() )
  {
    fresult = (float)dialog.doubleValue();
    return true;
  }
  else
    return false;
} 


//------------------------------------------------------------------------------
// Ввод вещественного числа
// ---
bool GetDouble( const TCHAR* nPrompt, double & dresult, const MbRect1D * range ) 
{
  QInputDialog dialog( ParentForDialog(), Qt::Tool/*Tip*/ | Qt::Window );
  dialog.setWindowTitle( QString_fromT( TITLE_MAIN_WND ) );
  dialog.setLabelText( QString_fromT( nPrompt ) );
  dialog.setDoubleRange ( (double)SYS_MIN_INT32, (double)SYS_MAX_INT32 );
  dialog.setDoubleDecimals( fabs((double)dresult) < 0.001 ? 3 : 9 );
  dialog.setDoubleValue( (double)dresult );

  dialog.resize( 350, dialog.height() );
  dialog.move( 40, 85 );
  dialog.setSizeGripEnabled( true );
  if ( dialog.exec() )
  {
    dresult = (double)dialog.doubleValue();
    return true;
  }
  else
    return false;
} 


//------------------------------------------------------------------------------
// Окно выбора из двух вариантов (да/нет)
// ---
bool GetYesNoChoice( const TCHAR * nPrompt, const TCHAR * nTitle, bool defaultFirstButton )
{
  QMessageBox msgBox( ParentForDialog() );
  msgBox.setWindowTitle( QString_fromT( nTitle ) );
  msgBox.setInformativeText( QString_fromT( nPrompt ) );
  msgBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );

  if ( defaultFirstButton )
    msgBox.setDefaultButton( QMessageBox::Yes );
  else
    msgBox.setDefaultButton( QMessageBox::No );

  return msgBox.exec() == QMessageBox::Yes ? true : false;
}


//------------------------------------------------------------------------------
// Ввод данных для создания символьной кривой
// ---
bool GetDataCharCurve3D( string_t& fx,
                         string_t& fy,
                         string_t& fz,
                         string_t& arg ) {
  TCHAR arTchar[STRINGLENGTH];

  if ( GetString( IDS_ENTER_FORMULA, _T("t"), arTchar, STRINGLENGTH ) ) 
  {
    TCHAR fxTchar[STRINGLENGTH];
    TCHAR fyTchar[STRINGLENGTH];
    TCHAR fzTchar[STRINGLENGTH];

    _sntprintf( fxTchar, STRINGLENGTH, _T("20*cos(%s)"), arTchar );
    _sntprintf( fyTchar, STRINGLENGTH, _T("30*sin(%s)"), arTchar );
    _sntprintf( fzTchar, STRINGLENGTH, _T("%s*2"),       arTchar );
    if ( GetString( IDS_ENTER_FORMULA, fxTchar, fxTchar, STRINGLENGTH ) && 
         GetString( IDS_ENTER_FORMULA, fyTchar, fyTchar, STRINGLENGTH ) && 
         GetString( IDS_ENTER_FORMULA, fzTchar, fzTchar, STRINGLENGTH ) 
       ) {
      fx  = fxTchar; 
      fy  = fyTchar;
      fz  = fzTchar;
      arg = arTchar;
      return true;
    }
  }

  return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Диалог 
// ---
ExactPointDialog::ExactPointDialog( QWidget *parent, bool is3D )
: QDialog( parent )
, edX ( nullptr )
, edY ( nullptr )
, edZ ( nullptr )
{
  QGridLayout *layout = new QGridLayout;
  layout->setColumnStretch(1, 1);

  QLabel* labelX = new QLabel;
  labelX->setText( QString_fromT(_T("X")) );
  layout->addWidget( labelX, 0, 0 );

  QLabel* labelY = new QLabel;
  labelY->setText( QString_fromT(_T("Y")) );
  layout->addWidget( labelY, 1, 0 );

  if ( is3D ) {
    QLabel* labelZ = new QLabel;
    labelZ->setText( QString_fromT(_T("Z")) );
    layout->addWidget( labelZ, 2, 0 );
  }

  edX = new QDoubleSpinBox;
  edX->setRange ( -100000.0, 100000.0 );
  edX->setDecimals ( 3 );
  edX->setSingleStep ( 1.0 );
  layout->addWidget( edX, 0, 1 );

  edY = new QDoubleSpinBox;
  edY->setRange ( -100000.0, 100000.0 );
  edY->setDecimals ( 3 );
  edY->setSingleStep ( 1.0 );
  layout->addWidget( edY, 1, 1 );

  if ( is3D ) {
    edZ = new QDoubleSpinBox;
    edZ->setRange ( -100000.0, 100000.0 );
    edZ->setDecimals ( 3 );
    edZ->setSingleStep ( 1.0 );
    layout->addWidget( edZ, 2, 1 );
  }

  QDialogButtonBox* bbox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal );
    connect(bbox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(bbox, SIGNAL(rejected()), this, SLOT(reject()));
  layout->addWidget( bbox, (is3D ? 3 : 4), 1 );

  setLayout(layout);

#if defined ( __NATIVE_LANGUAGE__ )
  setWindowTitle( QString_fromT(_T("Введите координаты точки")) );
#else
  setWindowTitle( QString_fromT(_T("Enter point coordinates")) );
#endif
  setSizeGripEnabled( true );
}

//------------------------------------------------------------------------------
// Диалог ввода ключа и подписи лицензии
// ---
LicenseKeyDialog::LicenseKeyDialog( QWidget *parent )
  : QDialog( parent )
  ,m_key ( nullptr )
  ,m_signature ( nullptr )
  ,m_save2file ( nullptr )
{
#if defined ( __NATIVE_LANGUAGE__ )
  setWindowTitle( QString_fromT( _T( "Введите ключ и подпись лицензии" ) ) );
#else
  setWindowTitle( QString_fromT( _T( "Enter a key and a signature of the license" ) ) );
#endif

  QGridLayout* editLayout = new QGridLayout();
  editLayout->setRowStretch( 1, 0 );

  QLabel* labelKey = new QLabel();
#if defined ( __NATIVE_LANGUAGE__ )
  labelKey->setText( QString( QString_fromT( _T( "Ключ" ) ) ) );
#else
  labelKey->setText( QString( QString_fromT( _T( "Key" ) ) ) );
#endif  
  editLayout->addWidget( labelKey, 0, 0 );

  QLabel* labelSignature = new QLabel();
#if defined ( __NATIVE_LANGUAGE__ )
  labelSignature->setText( QString( QString_fromT( _T( "Подпись" ) ) ) );
#else
  labelSignature->setText( QString( QString_fromT( _T( "Signature" ) ) ) );
#endif 
  editLayout->addWidget( labelSignature, 1, 0 );

  m_save2file = new QCheckBox();
#if defined ( __NATIVE_LANGUAGE__ )
  m_save2file->setText( QString_fromT( _T( "Записать данные лицензии в файл?" ) ) );
#else
  m_save2file->setText( QString_fromT( _T( "Write the license data to a file?" ) ) );
#endif 
  editLayout->addWidget( m_save2file, 3, 0, 1, 2, Qt::AlignLeft );

  m_key = new QLineEdit( this );
  m_key->setMinimumWidth(300);
  editLayout->addWidget( m_key, 0, 1 );

  m_signature = new QLineEdit();
  editLayout->addWidget( m_signature, 1, 1 );

  QDialogButtonBox* buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal );
  connect( buttonBox, SIGNAL( accepted() ), this, SLOT( accept() ) );
  connect( buttonBox, SIGNAL( rejected() ), this, SLOT( reject() ) );

  QVBoxLayout* vBoxLayout = new QVBoxLayout();
  vBoxLayout->addLayout( editLayout );
  vBoxLayout->addWidget( buttonBox );
  this->setLayout( vBoxLayout );
}

std::string LicenseKeyDialog::GetKey()
{
  return m_key->text().toStdString();
}

std::string LicenseKeyDialog::GetSignature()
{
  return m_signature->text().toStdString();
}

bool LicenseKeyDialog::GetSavingFlag()
{
  return m_save2file->checkState() == Qt::CheckState::Checked;
}

//------------------------------------------------------------------------------
// Ввод точных координат точки
// ---
void ExactPointDialog::SetValue( double x, double y, double z )
{
  PRECONDITION( edX && edY );
  edX->setValue ( x );
  edY->setValue ( y );
  if ( edZ )
    edZ->setValue ( z );
}


//------------------------------------------------------------------------------
// Ввод точных координат точки
// ---
void ExactPointDialog::GetValue( double& x, double& y ) const
{
  PRECONDITION( edX && edY );
  x = edX->value();
  y = edY->value();
}


//------------------------------------------------------------------------------
// Ввод точных координат точки
// ---
void ExactPointDialog::GetValue( double& x, double& y, double& z ) const
{
  GetValue( x, y );
  PRECONDITION( edZ );
  z = edZ->value();
}

//------------------------------------------------------------------------------
// Ввод точных координат точки на плоскости
// ---
bool GetExactPlanePoint( MbCartPoint & cPoint )
{
  bool retVal = false;

  ExactPointDialog* dlg = new ExactPointDialog( ParentForDialog(), false );
  dlg->SetValue( cPoint.x, cPoint.y );
  if ( dlg->exec() == QDialog::Accepted ) {
    dlg->GetValue( cPoint.x, cPoint.y );
    retVal = true;
  }

  delete dlg;

  return retVal;
} 


//------------------------------------------------------------------------------
// Ввод точных координат точки
// ---
bool GetExactPoint( MbCartPoint3D & cPoint )
{
  bool retVal = false;

  ExactPointDialog* dlg = new ExactPointDialog( ParentForDialog(), true );
  dlg->SetValue( cPoint.x, cPoint.y, cPoint.z );
  if ( dlg->exec() == QDialog::Accepted ) {
    dlg->GetValue( cPoint.x, cPoint.y, cPoint.z );
    retVal = true;
  }

  delete dlg;

  return retVal;
} 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// 
// ---
SurfaceDegreeDialog::SurfaceDegreeDialog( QWidget *parent )
: QDialog( parent )
, edU ( nullptr )
, edV ( nullptr )
{
  QGridLayout *layout = new QGridLayout;
  layout->setColumnStretch(1, 1);

  QLabel* labelU = new QLabel;
#if defined ( __NATIVE_LANGUAGE__ )
  labelU->setText( QString_fromT(_T("Порядок по U")) );
#else
  labelU->setText( QString_fromT(_T("Order by U")) );
#endif
  layout->addWidget( labelU, 0, 0 );

  QLabel* labelV = new QLabel;
#if defined ( __NATIVE_LANGUAGE__ )
  labelV->setText( QString_fromT(_T("Порядок по V")) );
#else
  labelV->setText( QString_fromT(_T("Order by V")) );
#endif
  layout->addWidget( labelV, 1, 0 );

  edU = new QSpinBox;
  edU->setRange ( 1, 100 );
  layout->addWidget( edU, 0, 1 );

  edV = new QSpinBox;
  edV->setRange ( 1, 100 );
  layout->addWidget( edV, 1, 1 );

  QDialogButtonBox* bbox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal );
    connect(bbox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(bbox, SIGNAL(rejected()), this, SLOT(reject()));
  layout->addWidget( bbox, 2, 1 );

  setLayout(layout);

#if defined ( __NATIVE_LANGUAGE__ )
  setWindowTitle( QString_fromT(_T("Задание порядка поверхности")) );
#else
  setWindowTitle( QString_fromT(_T("Specification of surface order")) );
#endif
  setSizeGripEnabled( true );
}


//------------------------------------------------------------------------------
//
// ---
void SurfaceDegreeDialog::SetValue( size_t u, size_t v )
{
  PRECONDITION( edU && edV );
  edU->setValue ( (int)u );
  edV->setValue ( (int)v );
}


//------------------------------------------------------------------------------
//
// ---
void SurfaceDegreeDialog::GetValue( size_t& u, size_t& v ) const
{
  PRECONDITION( edU && edV );
  u = (size_t)edU->value();
  v = (size_t)edV->value();
}


//------------------------------------------------------------------------------
// Установить порядок поверхности
// ---
bool SetSurfaceDegree()
{
  bool retVal = false;

  SurfaceDegreeDialog* dlg = new SurfaceDegreeDialog( ParentForDialog() );
  dlg->SetValue( Math::uSurfaceDegree, Math::vSurfaceDegree );
  if ( dlg->exec() == QDialog::Accepted ) {
    size_t udegree( 4 ), vdegree( 4 );
    dlg->GetValue( udegree, vdegree );
    Math::uSurfaceDegree.SetVarValue( udegree );
    Math::vSurfaceDegree.SetVarValue( vdegree );
    retVal = true;
  }

  delete dlg;

  return retVal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Ввод булевского значения
// ---
bool GetBool( const TCHAR* nPrompt, bool defValue )
{
  QMessageBox dialog( ParentForDialog()/*, Qt::Tool | Qt::Window*/ ); 
  dialog.setWindowTitle( QString_fromT( TITLE_MAIN_WND ) ); 
  dialog.setText       ( QString_fromT(nPrompt) );
  dialog.addButton     ( QMessageBox::Yes );
  dialog.addButton     ( QMessageBox::No );
  dialog.setDefaultButton( defValue ? QMessageBox::Yes : QMessageBox::No);
  dialog.setIcon       ( QMessageBox::NoIcon );

  dialog.resize( 350, dialog.height() );
  dialog.move( 40, 85 );
  dialog.setSizeGripEnabled( true );
  if ( dialog.exec() )
    return dialog.result() == QMessageBox::Yes;
  else
    return defValue;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// 
// ---
bool ExecuteSetOrientationDlg( MbMatrix3D& matrix ) {
  PRECONDITION( false ); // Не реализовано - (Голованов Н.Н. 01.06.2012 нет необходимости)
  return true;
}


//------------------------------------------------------------------------------
// 
// ---
bool ExecuteSetProjectionDlg( MbMatrix3D& matrix ) {
  PRECONDITION( false ); // Не реализовано - (Голованов Н.Н. 01.06.2012 нет необходимости)
  return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Процедура ввода контура
//---
ptrdiff_t ExecuteContourDlg()
{
  QMessageBox dialog( ParentForDialog()/*, Qt::Tool | Qt::Window*/ ); 
  dialog.setWindowTitle( QString_fromT( TITLE_MAIN_WND ) ); 
#if defined ( __NATIVE_LANGUAGE__ )
  dialog.setText       ( QString_fromT( _T("Ввод контура") ) );
#else
  dialog.setText       ( QString_fromT( _T("Enter a contour") ) );
#endif
#if defined ( __NATIVE_LANGUAGE__ )
  QPushButton*  btn_PUSHBUTTON_SPLINE   = dialog.addButton ( QString_fromT( _T("B-сплайн") ),  QMessageBox::AcceptRole );
#else
  QPushButton*  btn_PUSHBUTTON_SPLINE   = dialog.addButton ( QString_fromT( _T("B-spline") ),  QMessageBox::AcceptRole );
#endif
#if defined ( __NATIVE_LANGUAGE__ )
  QPushButton*  btn_PUSHBUTTON_SEGMENT  = dialog.addButton ( QString_fromT( _T("Отрезок") ),   QMessageBox::AcceptRole );
#else
  QPushButton*  btn_PUSHBUTTON_SEGMENT  = dialog.addButton ( QString_fromT( _T("Segment") ),   QMessageBox::AcceptRole );
#endif
#if defined ( __NATIVE_LANGUAGE__ )
  QPushButton*  btn_PUSHBUTTON_POLYLINE = dialog.addButton ( QString_fromT( _T("Полилиния") ), QMessageBox::AcceptRole );
#else
  QPushButton*  btn_PUSHBUTTON_POLYLINE = dialog.addButton ( QString_fromT( _T("Polyline") ), QMessageBox::AcceptRole );
#endif
#if defined ( __NATIVE_LANGUAGE__ )
  QPushButton*  btn_PUSHBUTTON_ARC      = dialog.addButton ( QString_fromT( _T("Дуга") ),      QMessageBox::AcceptRole );
#else
  QPushButton*  btn_PUSHBUTTON_ARC      = dialog.addButton ( QString_fromT( _T("Arc") ),      QMessageBox::AcceptRole );
#endif
#if defined ( __NATIVE_LANGUAGE__ )
  QPushButton*  btn_PUSHBUTTON_CLOSE    = dialog.addButton ( QString_fromT( _T("Замкнуть") ),  QMessageBox::AcceptRole );
#else
  QPushButton*  btn_PUSHBUTTON_CLOSE    = dialog.addButton ( QString_fromT( _T("Close") ),  QMessageBox::AcceptRole );
#endif
#if defined ( __NATIVE_LANGUAGE__ )
                                          dialog.addButton ( QString_fromT( _T("Выход") ),     QMessageBox::AcceptRole );
#else
                                          dialog.addButton ( QString_fromT( _T("Exit") ),     QMessageBox::AcceptRole );
#endif

  dialog.setIcon( QMessageBox::NoIcon );

  dialog.resize( 350, dialog.height() );
  dialog.move( 40, 85 );
  dialog.setSizeGripEnabled( true );
  if ( dialog.exec() )
  {
  QAbstractButton * clicked = dialog.clickedButton();
    if ( clicked == btn_PUSHBUTTON_SPLINE   ) return IDC_PUSHBUTTON_SPLINE;   // Сплайн
  else
    if ( clicked == btn_PUSHBUTTON_SEGMENT  ) return IDC_PUSHBUTTON_SEGMENT;  // Прямая
  else
    if ( clicked == btn_PUSHBUTTON_POLYLINE ) return IDC_PUSHBUTTON_POLYLINE; // Полилиния
  else
    if ( clicked == btn_PUSHBUTTON_ARC      ) return IDC_PUSHBUTTON_ARC;      // Дуга
  else
    if ( clicked == btn_PUSHBUTTON_CLOSE    ) return IDC_PUSHBUTTON_CLOSE;    // Замкнуть
  else
    return IDC_PUSHBUTTON_EXIT;     // Выход
  }
  else
    return IDC_PUSHBUTTON_EXIT;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Выбор направления
// ---
ptrdiff_t ExecuteChooseDirectionDlg() {
  QMessageBox dialog( ParentForDialog()/*, Qt::Tool | Qt::Window*/ ); 
  dialog.setWindowTitle( QString_fromT( TITLE_MAIN_WND ) ); 
#if defined ( __NATIVE_LANGUAGE__ )
  dialog.setText       ( QString_fromT( _T("Выбор направления") ) );
#else
  dialog.setText       ( QString_fromT( _T("Specifying the direction") ) );
#endif
#if defined ( __NATIVE_LANGUAGE__ )
  QPushButton*  btn_PUSHBUTTON_CHANGE   = dialog.addButton ( QString_fromT( _T("Изменить") ),  QMessageBox::AcceptRole );
#else
  QPushButton*  btn_PUSHBUTTON_CHANGE   = dialog.addButton ( QString_fromT( _T("Change") ),  QMessageBox::AcceptRole );
#endif
#if defined ( __NATIVE_LANGUAGE__ )
  QPushButton*  btn_PUSHBUTTON_STEPBACK = dialog.addButton ( QString_fromT( _T("Шаг назад") ), QMessageBox::AcceptRole );
#else
  QPushButton*  btn_PUSHBUTTON_STEPBACK = dialog.addButton ( QString_fromT( _T("Back") ), QMessageBox::AcceptRole );
#endif
#if defined ( __NATIVE_LANGUAGE__ )
  QPushButton*  btn_PUSHBUTTON_IDOK     = dialog.addButton ( QString_fromT( _T("OK") ),        QMessageBox::AcceptRole );
#else
  QPushButton*  btn_PUSHBUTTON_IDOK     = dialog.addButton ( QString_fromT( _T("OK") ),        QMessageBox::AcceptRole );
#endif
#if defined ( __NATIVE_LANGUAGE__ )
  dialog.addButton ( QString_fromT( _T("Отказ") ),     QMessageBox::AcceptRole );
#else
  dialog.addButton ( QString_fromT( _T("Cancel") ),     QMessageBox::AcceptRole );
#endif
 
  dialog.setIcon( QMessageBox::NoIcon );

  dialog.resize( 350, dialog.height() );
  dialog.move( 40, 85 );
  dialog.setSizeGripEnabled( true );
  if ( dialog.exec() )
  {
  QAbstractButton * clicked = dialog.clickedButton();
    if ( clicked == btn_PUSHBUTTON_CHANGE   ) return 1;
  else
    if ( clicked == btn_PUSHBUTTON_STEPBACK ) return -1;
  else
    if ( clicked == btn_PUSHBUTTON_IDOK     ) return 0; 
  else
    return -2;
  }
  else
    return -2;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// 
// ---
TimeTestResultDialog::TimeTestResultDialog( QWidget *parent, const void * params )
: QDialog( parent )
{
  QGridLayout *layout = new QGridLayout;
  layout->setRowStretch(1, 1);
  layout->setRowMinimumHeight(1, 250);

  QLabel* label = new QLabel;
  label->setText( QString_fromT(((TCHAR**)params)[3]) );
  layout->addWidget( label, 0, 0 );

  QTableWidget* tw = new QTableWidget;
#if defined ( __NATIVE_LANGUAGE__ )
  static const TCHAR * szColumnNames[] = { _T("Имя операции"), _T("Время исполнения"), _T("Среднее время"), _T("Кол. вызовов") };
#else
  static const TCHAR * szColumnNames[] = { _T("Operation name"), _T("Execution time"), _T("Average time"), _T("Number of calls") };
#endif
  static int iColumnWidths[] = { 150, 120, 120, 120 };

  tw->setColumnCount(4);
  for ( int i = 0; i < 4; i++ )
  {
    tw->setColumnWidth( i, iColumnWidths[i] ); 
    tw->setHorizontalHeaderItem ( i, new QTableWidgetItem( QString_fromT(szColumnNames[i]) ) );
  }
  tw->horizontalHeader()->setSectionResizeMode (0, QHeaderView::Stretch);
  tw->verticalHeader()->hide();

  //-----------------------------------------------------------------
  TimeTest * testResult = ((TimeTest**)params)[4];   //-V112

  if ( testResult != nullptr )
  {
    std::vector<TimeTestResult> sumResult;
    ::SortResultMeasuring( *testResult, sumResult );

    tw->setRowCount((int)sumResult.size());

    for ( size_t i = 0; i < sumResult.size(); i++ )
    {
      tw->setItem( (int)i, 0, new QTableWidgetItem( QString_fromT(sumResult[(size_t)i].name.c_str()) ) );  // [0-column]
   
      int timeResult = (int)sumResult[(size_t)i].GetMiliseconds(); // Перевести timeResult в милисекунды и выдать 
      if ( timeResult > 0 ) {
        TCHAR  buf[15]; // Буфер для записи цифр
        _sntprintf( buf, 15, _T("%d"), timeResult );
            
        tw->setItem( (int)i, 1, new QTableWidgetItem( QString_fromT(buf) ) );  // [1-column]

        _sntprintf( buf, 15, F_TD, ((size_t)timeResult / sumResult[(size_t)i].count) );
        tw->setItem( (int)i, 2, new QTableWidgetItem( QString_fromT(buf) ) );  // [2-column]

        _sntprintf( buf, 15, F_TD, sumResult[(size_t)i].count );  //-V111
        tw->setItem( (int)i, 3, new QTableWidgetItem( QString_fromT(buf) ) );  // [3-column]
      }
    }
  }
  //-----------------------------------------------------------------

  layout->addWidget( tw, 1, 0 );

  QDialogButtonBox* bbox = new QDialogButtonBox( QDialogButtonBox::Ok, Qt::Horizontal );
    connect(bbox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(bbox, SIGNAL(rejected()), this, SLOT(reject()));
  layout->addWidget( bbox, 2, 0 );

  layout->setColumnMinimumWidth ( 0, 200 );
  setLayout(layout);

  setWindowTitle( QString_fromT(((TCHAR**)params)[0]) );
  setSizeGripEnabled( true );
}


//------------------------------------------------------------------------------
// 
// ---
bool ExecuteTimeTestResultDlg( const void * params ) {
  
  TimeTestResultDialog* dlg = new TimeTestResultDialog( ParentForDialog(), params );
  dlg->exec();
  delete dlg;

  return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Выдать информационное сообщение
// ---
void MessageBoxEx( const TCHAR* text, const TCHAR* title ) {
  QMessageBox::information( ParentForDialog(), QString_fromT(title), QString_fromT(text) );
}


//------------------------------------------------------------------------------
// Изменить цвет элемента
// ---
bool CMUColor( COLORREF &crColor ) {
  QColor sourceColor( GetRValue(crColor), GetGValue(crColor), GetBValue(crColor) );
  QColor resColor = QColorDialog::getColor( sourceColor, ParentForDialog() );

  
  if ( resColor.isValid () && 
       sourceColor != resColor )
  {
    crColor = RGB( resColor.red(), resColor.green(), resColor.blue() );
    viewManager->InvalidateWindow( viewManager->GetActiveWindowIndex(), true );
    return true;
  }

  return false;
}


#endif // __USE_QT__ 


