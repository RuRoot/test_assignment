////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Параметр с контролем измененности.
         \en Paramenter with the control of being changed. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_PARAMETER_H
#define __TEMPL_PARAMETER_H

#include <io_tape.h>


////////////////////////////////////////////////////////////////////////////////
//
// \ru Предназначен для контроля измененности значения по отношении к настроенному \en Template is used for the control that the value has been changed in relation to configured value 
//
////////////////////////////////////////////////////////////////////////////////
template<class T> class Param
{
  T    m_value;        // \ru значение параметра \en a Parameter value  
  bool m_bHandChanged; // \ru параметр изменен вручную и значение не соответствует настройкам документа \en a parameter has been changed manually and the value does not match the document settings 

public :
  Param( const T & );
  Param( const Param<T> & );

  /// \ru проверить параметр на измененность \en check whether parameter has been changed 
  bool IsHandChanged() const;       

  /// \ru установить флаг, что парамерт изменен из API нужно сбрасывать флаг \en set the flag that parameter has been changed, the flag should be reset from API 
  void SetHandChanged( bool change = true );       

  /// \ru сопоставить значение с умолчательным и при различии взвести флаг m_bHandChanged \en compare the value with default value and set the flag m_bHandChanged = true if they are different 
  void CheckHandChanged  ( const T & );

  /// \ru проинициализировать новым значением, если не было изменено ранее вручную \en initialize by a new value if it was not changed by hand earlier 
  void InitIfNoHandChange( const T & ); 

  /// \ru получить значение \en get the value 
  const T & GetValue() const;

  void operator = ( const Param<T> & );
  void operator = ( const T & );
  /// \ru чтение/запись из потока \en reading/writing from stream 
  void Read ( reader & in );
  void Write( writer &    ) const;
};


//-------------------------------------------------------------------------------
//
// ---
template<class T> 
Param<T>::Param ( const T & val ) :
  m_value       ( val   ),
  m_bHandChanged( false )
{
}


//-------------------------------------------------------------------------------
//
// ---
template<class T> 
Param<T>::Param( const Param<T> & other ) :
  m_value       ( other.m_value        ),
  m_bHandChanged( other.m_bHandChanged )
{
}


//-------------------------------------------------------------------------------
/// \ru проверить параметр на измененность \en check whether parameter has been changed 
// ---
template<class T> 
bool Param<T>::IsHandChanged() const
{
  return m_bHandChanged;
}


//-------------------------------------------------------------------------------
/// \ru установить флаг, что парамерт изменен \en set the flag that parameter has been changed 
// ---
template<class T> 
void Param<T>::SetHandChanged( bool change/* = true*/ )
{
  m_bHandChanged = change;
}


//-------------------------------------------------------------------------------
/// \ru сопоставить значение с умолчательным и при различии взвести флаг m_bHandChanged \en compare the value with default value and set the flag m_bHandChanged = true if they are different 
// ---
template<class T> 
void Param<T>::CheckHandChanged( const T & val )
{
  if ( m_value != val )
    m_bHandChanged = true;
}


//-------------------------------------------------------------------------------
/// \ru проинициализировать новым значением, если не было изменено ранее вручную \en initialize by a new value if it was not changed by hand earlier 
// ---
template<class T> 
void Param<T>::InitIfNoHandChange( const T & val )
{
  if ( !m_bHandChanged )
    m_value = val;
}


//-------------------------------------------------------------------------------
//
// ---
template<class T> 
const T & Param<T>::GetValue() const
{
  return m_value;
}


//-------------------------------------------------------------------------------
//
// ---
template<class T> 
void Param<T>::operator = ( const Param<T> & other )
{
  m_value        = other.m_value;
  m_bHandChanged = other.m_bHandChanged;
}


//-------------------------------------------------------------------------------
//
// ---
template<class T> 
void Param<T>::operator = ( const T & val )
{
  m_value = val;
}


//-------------------------------------------------------------------------------
//
// ---
template<class T> 
void Param<T>::Read ( reader & in )
{
  in >> m_value;
  in >> m_bHandChanged;
}


//-------------------------------------------------------------------------------
//
// ---
template<class T> 
void Param<T>::Write( writer & out ) const
{
  out << m_value;
  out << m_bHandChanged;
}


#endif // __TEMPL_PARAMETER_H
