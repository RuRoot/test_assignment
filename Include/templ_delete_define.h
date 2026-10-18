////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Типы удаления элементов из массива.
         \en Types of deletion of elements from array. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_DELETE_DEFINE_H
#define __TEMPL_DELETE_DEFINE_H


//------------------------------------------------------------------------------
/** \brief \ru Типы удаления элементов.
           \en Types of elemets deletion. \~
  \details \ru Типы удаления элементов при удалении из массива.
           \en Types of elements deletion while the deletion from an array. \~
  \ingroup Base_Tools
*/
// ---
enum DelType {
  defDelete, ///< \ru Удалять объект по умолчанию. \en Delete an object by default. 
  noDelete,  ///< \ru Не удалять объект. \en Do not delete an object. 
  Delete     ///< \ru Удалять объект. \en Delete an object. 
};


#endif // __TEMPL_DELETE_DEFINE_H
