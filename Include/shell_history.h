////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru История граней.
         \en Faces history. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SHELL_HISTORY_H
#define __SHELL_HISTORY_H


#include <topology.h>


class   MbFaceShell;


//------------------------------------------------------------------------------
/** \brief \ru История граней.
           \en Faces history. \~
  \details \ru История граней содержит два синхронных множества граней:
    исходных граней и их копий. \n
    История используется после операции для замены в результирующей оболочке 
    неизменённых операцией копий гриней их оригиналами. \n
           \en Faces history contains two synchronous sets of faces:
    initial faces and their copies. \n
    A faces history is used after the operation of replacement in a result shell 
    of unchanged faces copies by their originals. \n \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbShellHistory {
private:
  RPArray<MbFace> originFaces; ///< \ru Множество исходных граней. \en A set of initial faces. 
  RPArray<MbFace> copyFaces;   ///< \ru Множество копий граней. \en A set of faces copies. 
public:
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  MbShellHistory(); 
  /// \ru Деструктор. \en Destructor. 
  ~MbShellHistory();

public:
  /// \ru Очистить массивы для повторного использования. \en Clear arrays for reuse. 
  void    Clear(); 
  /// \ru Запомнить оригиналы. \en Save originals. 
  void    InitOrigins( const RPArray<MbFace> & origin ); 
  /// \ru Выдать контейнер оригиналов для заполнения. \en Get container of originals for filling. 
  RPArray<MbFace> & SetOriginFaces() { return originFaces; }
  /// \ru Выдать контейнер копий для заполнения. \en Get container of copies for filling. 
  RPArray<MbFace> & SetCopyFaces() { return copyFaces; }
  /// \ru Заменить в shell неизменённые copy-объекты на origin-объекты. \en Replace in 'shell' the unchanged 'copy'-objects by the 'origin'-objects. 
  void    SetOrigins ( MbFaceShell & shell ); 

OBVIOUS_PRIVATE_COPY( MbShellHistory )
};


#endif // __SHELL_HISTORY_H
