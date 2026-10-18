////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Трехпозиционный флаг.
         \en Tree-position flag. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_THREE_STATES_H
#define __TEMPL_THREE_STATES_H


//------------------------------------------------------------------------------
/** \brief \ru Трехпозиционный флаг.
           \en Tree-position flag. \~
  \details \ru Флаг из трех состояний. \n
           \en A flag of three states. \n \~
  \ingroup Base_Tools
*/
// ---
enum ThreeStates {
  ts_negative = -1, ///< \ru Состояние НЕТ. \en The state NO. 
  ts_neutral  =  0, ///< \ru Состояние НЕ ИЗВЕСТНО. \en The state UNKNOWN. 
  ts_positive =  1  ///< \ru Состояние ДА. \en The state YES. 
};


#endif // __TEMPL_THREE_STATES_H
