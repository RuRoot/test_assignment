////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Индикатор прогресса.
         \en A progress indicator. \~

*/
////////////////////////////////////////////////////////////////////////////////


#ifndef __ALG_INDICATOR_H
#define __ALG_INDICATOR_H


#include <tool_cstring.h>
#include <system_types.h>
#include <templ_visitor.h>
#include <math_define.h>
#include <mb_property_title.h>
#include <mb_enum.h>
#include <reference_item.h>
#include <tool_multithreading.h>


//------------------------------------------------------------------------------
/** \brief \ru Данные о строке.
           \en Data of a string \~
  \details \ru Данные о строке (абстракция с возможностью посещения).
           \en Data of a string (an abstraction with a possibility of visit). \~
  \ingroup Base_Items
*/
//---
class MATH_CLASS IStrData {
public:
  IStrData() {}          ///< \ru Конструктор по умолчанию. \en Default constructor. 
  virtual ~IStrData() {} ///< \ru Деструктор. \en Destructor. 
public:
  virtual bool Accept( Visitor & ) = 0; ///< \ru Прием посетителя. \en Acceptance of a visitor. 
};


//------------------------------------------------------------------------------
/** \brief \ru Конкретные данные о строке.
           \en Specific data of a string \~
  \details \ru Конкретные данные о строке. \n
           \en Specific data of a string \n \~
  \ingroup Base_Items
*/
//---
template<typename T>
class StrData : public IStrData {
private:
  T m_msg; ///< \ru Данные. \en Data. 

public:
  /// \ru Конструктор по данным. \en Constructor by data. 
  StrData( T msg ) : m_msg( msg ) {} 
  /// \ru Деструктор. \en Destructor. 
  virtual ~StrData() {}

  /// \ru Прием посетителя. \en Acceptance of a visitor. 
  virtual bool Accept( Visitor & visitor ) 
  {
    VisitorImpl<T> * impl = dynamic_cast<VisitorImpl<T> *>(&visitor);
    if( impl )
      impl->Visit( m_msg );
    else
      C3D_ASSERT_UNCONDITIONAL( false ); // \ru не реализована ф-ия посещения этого типа данных! \en the function of visit for this data type is not implemented! 

    return !!impl;
  }
};


//------------------------------------------------------------------------------
/** \brief \ru Базовый класс для потокобезопасного посетителя, извлекающего строку.
           \en Base class for thread-safe visitor extracting a string. \~
  \details \ru Базовый класс для потокобезопасного посетителя, извлекающего строку. 
           Можно использовать как образец при создании потокобезопасных посетителей, работающих с другими данными. \n
           \en Base class for thread-safe visitor extracting a string. 
           Can be used as a sample when creating thread-safe visitors, working with other data.\n \~
\ingroup Base_Items
*/
//---
//------------------------------------------------------------------------------
// \ru Базовый класс для потокобезопасного посетителя, извлекающего строку.
// \en Base class for thread-safe visitor extracting a string.
// ---
class BaseStrVisitor : public Visitor, public VisitorImpl<const TCHAR*> {
protected:
  /// \ru Данные посетителя. \en Visitor data.
  struct BaseAuxiliaryData : public AuxiliaryData
  {
    c3d::string_t data;
    BaseAuxiliaryData() : data() {}
  };

  ///< \ru Менеджер, обеспечивающий потокобезопасный доступ к данным. \en Manager providing thread-safe access to the data.
  mutable CacheManager<BaseAuxiliaryData> cache;

public:
  /// \ru Конструктор. \en Constructor.
  BaseStrVisitor() : cache() {}
  /// \ru Деструктор. \en Destructor.
  virtual ~BaseStrVisitor() {}

public:

  ///< \ru Обработка посещения объекта. \en Processing of the object visit. 
  virtual void Visit( const TCHAR*& str ) { 
    if ( str )
      cache()->data.assign( str );
  }
  
  ///< \ru Извлечение строки объекта. \en Extracting a string of the object. 
  virtual const TCHAR* GetString() const {
    return cache()->data.c_str();
  }

  ///< \ru Доступ к данным объекта. \en Access to the object data. 
  c3d::string_t& Data() {
    return cache()->data;
  }
};


#define EMPTY_STR StrData<const TCHAR *>( nullptr ) ///< \ru Создание пустой строки \en Creation of an empty string


//------------------------------------------------------------------------------
/** \brief \ru Добытчик строки из данных о строке.
           \en The getter of a string from string data. \~
  \details \ru Добытчик строки из данных о строке. \n
           \en The getter of a string from string data. \n \~
  \ingroup Base_Items
*/
//---
class MATH_CLASS IGetMsg {
public:
  IGetMsg() {}          ///< \ru Конструктор по умолчанию. \en Default constructor. 
  virtual ~IGetMsg() {} ///< \ru Деструктор. \en Destructor. 
public:
  /// \ru Данные о строке в строку. \en Convert data of a string to string. 
  virtual const TCHAR * Msg( IStrData & msg ) const = 0; 
};


//------------------------------------------------------------------------------
/** \brief \ru Интерфейс индикатора прогресса выполнения.
           \en Interface of the execution progress indicator. \~
  \details \ru Интерфейс индикатора прогресса выполнения. \n
           \en Interface of the execution progress indicator. \n \~
  \ingroup Base_Items
*/
//---
class MATH_CLASS IProgressIndicator : public IGetMsg {
public:
  IProgressIndicator() {}          ///< \ru Конструктор по умолчанию. \en Default constructor. 
  virtual ~IProgressIndicator() {} ///< \ru Деструктор. \en Destructor. 
public:
  /// \ru Установка диапазона индикации, сброс состояния. \en Setting of an indication range, reset state. 
  virtual bool Initialize( size_t range, size_t delta, IStrData & msg ) = 0; 
  /// \ru Обработать прогресс на n у.е., вернет false - пора останавливаться \en Process the progress by 'n' units, if it returns 'false', then it is time to stop. 
  virtual bool Progress  ( size_t n ) = 0;
  /// \ru Ликвидация ошибок округления дорастим прогресс бар до 100% \en Rounding errors liquidation, increase of a progress bar to 100% 
  virtual void Success   () = 0;

  /// \ru Проверка, не пора ли остановиться \en Check whether it is time to stop. 
  virtual bool IsCancel  () = 0;
  /// \ru Скажем, что пора остановиться. \en It is time to stop. 
  virtual void SetCancel ( bool c ) = 0;
  /// \ru Команда пора остановиться \en Command to stop. 
  virtual void Stop      () = 0;
};


//------------------------------------------------------------------------------
/** \brief \ru Проверка остановки индикатора прогресса.
           \en Check the progress indicator stop. \~
  \param[in] prog - \ru Индикатор прогресса выполнения.
                    \en The execution progress indicator. \~
  \return \ru true, если выполнение остановлено.
          \en true if the operation was stopped. \~
  \ingroup Base_Items
*/
// ---
inline bool IsProgressStopped( IProgressIndicator * prog )
{
  if ( prog != nullptr && prog->IsCancel() )
    return true;
  return false;
}

//------------------------------------------------------------------------------
/** \brief \ru Обертка индикатора прогресса выполнения.
           \en The wrapper of the execution progress indicator. \~
  \details \ru Обертка индикатора прогресса выполнения 
               (потокобезопасна при условии, если реализация IProgressIndicator также потокобезопасна). \n
           \en The wrapper of the execution progress indicator
               (thread-safe provided that IProgressIndicator implementation is also thread-safe). \n \~
  \ingroup Base_Items
*/
// ---
class MATH_CLASS ProgressBarWrapper : public MbRefItem {
private:
  /// \ru Данные индикатора прогресса. \en The progress indicator data.
  struct ProgressBarWrapperData : public AuxiliaryData
  {
    c3d::string_t         name;           ///< \ru Название процесса. \en A name of a process. 
    size_t                range;          ///< \ru Диапазон значений. \en A range of  values. 
    size_t                delta;          ///< \ru Минимальное приращение прогресса. \en A minimal increase of a progress. 
    size_t                value;          ///< \ru Текущий прогресс. \en A current index. 
    bool                  useParentName;  ///< \ru Использовать имя родителя для наследника. \en Whether to use a name of a parent for its successor. 

    ProgressBarWrapperData();
  };

  IProgressIndicator &  progBar;        ///< \ru Общий индикатор прогресса. \en A common progress indicator. 
  ProgressBarWrapper *  parentProgBar;  ///< \ru Родительский индикатор прогресса. \en A parent progress indicator. 
  mutable CacheManager<ProgressBarWrapperData> cache; ///< \ru Менеджер, обеспечивающий потокобезопасный доступ к данным. \en Manager providing thread-safe access to the data.

public:
  /// \ru Конструктор по индикатору прогресса выполнения. \en Constructor by an indicator of execution progress. 
  ProgressBarWrapper( IProgressIndicator & pBar );
  virtual ~ProgressBarWrapper(); ///< \ru Деструктор. \en Destructor. 

public:

  /// \ru Проверка на остановку процесса. \en Check whether a process stopped. 
  bool    IsCancel() { return progBar.IsCancel(); }
  /// \ru Окончание процесса. \en End the process. 
  void    Success() { progBar.Success(); }
  /// \ru Остановка процесса. \en Stop the process. 
  void    Stop() { progBar.Stop(); }
  /// \ru Восстановление данных процесса. \en Restoring of a process data. 
  bool    Reset();
  /// \ru Установка состояния. \en Set the state. 
  bool    Init( size_t range, size_t delta, size_t value, IStrData & msg );
  /// \ru Установка состояния. \en Set the state. 
  bool    Init( size_t range, size_t delta, size_t value );
  /// \ru Узнать текущее состояние прогресса. \en Get the current state of a progress. 
  size_t  GetValue() const { return cache()->value; }
  /// \ru Задать имя процесса. \en Set the name of a process. 
  bool    SetName( IStrData & msg );
  /// \ru Увеличить прогресс выполнения. \en Increase the execution progress. 
  bool    SetProgress( size_t v );
  /// \ru Создать наследника (если msg нулевой, то используется имя родителя). \en Create a successor (if 'msg' is empty, then the name of a parent is used). 
  ProgressBarWrapper & CreateChildAddRef( IStrData & msg ) const;
  /// \ru Создать наследника (если msg нулевой, то используется имя родителя). \en Create a successor (if 'msg' is empty, then the name of a parent is used). 
  ProgressBarWrapper & CreateChild( IStrData & msg ) const;
  /// \ru Использовать базовое имя при создании наследника. \en Use a base name while creating a successor. 
  void    UseParentName( bool s ) { cache()->useParentName = s; }
  /// \ru Используется ли базовое имя. \en Whether a base name is used. 
  bool    IsParentNameUsed() const { return cache()->useParentName; }
  /// \ru Получить родительский индикатор прогресса. \en Get parent progress indicator. 
  ProgressBarWrapper * GetParent() { return parentProgBar; }
  /// \ru Получить общий индикатор прогресса. \en Get common progress indicator. 
  IProgressIndicator * GetCommonProgress() { return &progBar; }

OBVIOUS_PRIVATE_COPY( ProgressBarWrapper )
};


//------------------------------------------------------------------------------
/** \brief \ru Создать индикатор прогресса.
           \en < Create a progress indicator. \~
  \param[in] progInd - \ru Интерфейс индикатора прогресса выполнения.
                       \en Interface of the execution progress indicator. \~
  \param[in] msg - \ru Данные о строке.
                   \en Data of a string \~
  \return \ru Обертку индикатора прогресса выполнения.
          \en The wrapper of the execution progress indicator. \~
  \ingroup Base_Items
*/
// ---
MATH_FUNC (ProgressBarWrapper *) CreateProgressBarAddRef( IProgressIndicator * progInd, IStrData & msg );


//------------------------------------------------------------------------------
/** \brief \ru Создать индикатор прогресса.
           \en < Create a progress indicator. \~
  \param[in] progInd - \ru Интерфейс индикатора прогресса выполнения.
                       \en Interface of the execution progress indicator. \~
  \param[in] msg - \ru Данные о строке.
                   \en Data of a string \~
  \return \ru Обертку индикатора прогресса выполнения.
          \en The wrapper of the execution progress indicator. \~
  \ingroup Base_Items
*/
// ---
MATH_FUNC (ProgressBarWrapper *) CreateProgressBar( IProgressIndicator * progInd, IStrData & msg );


//------------------------------------------------------------------------------
/** \brief \ru Установить имя прогресса.
           \en Set the progress name. \~
  \param[in] progBar - \ru Обертка индикатора прогресса выполнения.
                       \en The wrapper of the execution progress indicator. \~
  \param[in] msg - \ru Данные о строке.
                   \en Data of a string \~
  \return \ru true, если progBar != nullptr и удалось задать имя процесса.
          \en true if 'progBar' is not null and the process name is successfully set. \~
  \ingroup Base_Items
*/
// ---
inline bool SetProgressBarName( ProgressBarWrapper * progBar, IStrData & msg )
{
  if ( progBar != nullptr )
    return progBar->SetName( msg );
  return false;
}


//------------------------------------------------------------------------------
/** \brief \ru Установить значение прогресса.
           \en Set the value of a progress. \~
  \details \ru Установить значение прогресса.
           \en Set the value of a progress. \~
  \param[in] progBar - \ru Обертка индикатора прогресса выполнения.
                       \en The wrapper of the execution progress indicator. \~
  \param[in] v - \ru Значение прогресса.
                 \en The value of a progress. \~
  \return \ru true, в случае успешного выполнение операции.
          \en true if the operation is successful. \~
  \ingroup Base_Items
*/
// ---
inline bool SetProgressBarValue( ProgressBarWrapper * progBar, size_t v )
{
  if ( progBar != nullptr && !progBar->IsCancel() )
    return progBar->SetProgress( v );
  return false;
}

//------------------------------------------------------------------------------
/** \brief \ru Увеличить значение прогресса на заданный шаг.
           \en Increase the progress value by a given step. \~
  \details \ru Увеличить значение прогресса на заданный шаг.
           \en Increase the progress value by a given step. \~
  \param[in] progBar - \ru Обертка индикатора прогресса выполнения.
                       \en The wrapper of the execution progress indicator. \~
  \param[in] v - \ru Значение шага прогресса.
                 \en The value of a progress step. \~
  \return \ru true, в случае успешного выполнение операции.
          \en true if the operation is successful. \~
  \ingroup Base_Items
*/
// ---
inline bool AddProgressBarValue( ProgressBarWrapper * progBar, size_t v )
{
  if ( progBar != nullptr && !progBar->IsCancel() )
    return progBar->SetProgress( progBar->GetValue() + v );
  return false;
}


//------------------------------------------------------------------------------
/** \brief \ru Завершить индикатор прогресса.
           \en End the progress indicator. \~
  \details \ru Либо индикатор останавливается, либо, если он уже дошел до 100%, выдается 
    сообщение об этом.
           \en Either the indicator stops or, if it already has reached 100%, then 
    the corresponding message appears. \~
  \param[in] progBar - \ru Обертка индикатора прогресса выполнения.
                       \en The wrapper of the execution progress indicator. \~
  \ingroup Base_Items
*/
// ---
inline void FinishProgressBar( ProgressBarWrapper * progBar )
{
  if ( progBar != nullptr ) {
    if ( progBar->IsCancel() ) progBar->Stop();
    else                       progBar->Success();
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Удалить индикатор прогресса.
           \en Delete the progress indicator. \~
  \param[in] progBar - \ru Обертка индикатора прогресса выполнения.
                       \en The wrapper of the execution prorgress indicator. \~
  \return \ru true, в случае успешного выполнение операции.
          \en true if the operation is successful. \~
  \ingroup Base_Items
*/
// ---
inline bool StopProgressBar( ProgressBarWrapper * progBar )
{
  if ( progBar != nullptr && progBar->IsCancel() ) {
    progBar->Stop();
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
/** \brief \ru Использовать имя родителя для наследника.
           \en Whether to use the name of a parent for its successor. \~
  \param[in] progBar - \ru Обертка индикатора прогресса выполнения.
                       \en The wrapper of the execution progress indicator. \~
  \param[in] useParentName - \ru Флаг использования имени родителя.
                             \en The flag of using the parent name. \~
  \return \ru true, если progBar != nullptr.
          \en true if 'progBar' is not null. \~
  \ingroup Base_Items
*/
// ---
inline bool UseParentName( ProgressBarWrapper * progBar, bool useParentName )
{
  if ( progBar != nullptr ) {
    progBar->UseParentName( useParentName );
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
/** \brief \ru Используется ли имя родителя для наследника.
           \en Whether the name of a parent is used for its successor. \~
  \param[in] progBar - \ru Обертка индикатора прогресса выполнения.
                       \en The wrapper of the execution progress indicator. \~
  \return \ru true, если используется.
          \en true if it is used. \~
  \ingroup Base_Items
*/
// ---
inline bool IsParentNameUsed( const ProgressBarWrapper * progBar )
{
  if ( progBar != nullptr )
    return progBar->IsParentNameUsed();

  return false;
}


#endif // __ALG_INDICATOR_H
