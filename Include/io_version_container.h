////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Контейнер версий.
         \en Container of versions. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __IO_VERSION_CONTAINER_H
#define __IO_VERSION_CONTAINER_H


#include <templ_s_array.h>
#include <math_define.h>


//-----------------------------------------------------------------------------
/** \brief \ru Контейнер версий.
           \en Container of versions. \~
  \details \ru Контейнер версий объектов. \n
           \en Container of versions of objects. \n \~
  \ingroup Base_Tools_IO
*/
// ---
class MATH_CLASS VersionContainer
{
private:
  // \ru static VersionContainer iobuf_defaultVersionCont; // Версия по умолчанию. \en static VersionContainer iobuf_defaultVersionCont; // Default version.
  static VersionContainer & StaticVersionContainer();
protected:
  SArray<VERSION> m_self; ///< \ru Массив версий. \en Array of versions. 
protected:
  /// \ru Конструктор. \en Constructor. 
  VersionContainer( VERSION/*firstInit*/ );
public:
  /// \ru Конструктор. \en Constructor. 
  VersionContainer();
  /// \ru Конструктор копирования. \en Copy-constructor. 
  VersionContainer( const VersionContainer & other );
  /// \ru Деструктор. \en Destructor. 
  virtual ~VersionContainer();

  /// \ru Получить экземпляр контейнера версий. \en Get the instance of version container. 
  static const VersionContainer &  defaultVersionContainer();
  /// \ru Установить версии из другого контейнера \en Set versions from another container 
  static void  SetDefaultVersion( const VersionContainer & v );

  // \ru Вернуть главную версию (математического ядра). \en Return the main version (of the mathematical kernel). 
  VERSION GetMathVersion() const;
  // \ru Вернуть дополнительную версию (конечного приложения). \en Return the additional version (of the target application). 
  VERSION GetAppVersion ( size_t ind = -1 )  const;
  // \ru Очистить контейнер. \en Flush the container. 
  void    Flush         ();
  // \ru Установить версию по индексу. \en Set the version by the index. 
  void    SetVersion    ( size_t index, VERSION ver );
  // \ru Записать в кусок памяти. \en Write to the memory block. 
  size_t  ToMemory    ( const char *& memory ) const;
  // \ru Прочитать из куска памяти. \en Read from the memory block. 
  size_t  FromMemory  ( const char *  memory );

  /// \ru Оператор присваивания. \en An assignment operator. 
  VersionContainer &  operator = ( const VersionContainer & other );

  /// \ru Оператор чтения. \en Read operator. 
  friend MATH_FUNC (reader &) operator >> ( reader &, VersionContainer & );
  /// \ru Оператор записи. \en Write operator. 
  friend MATH_FUNC (writer &) operator << ( writer &, const VersionContainer & );

protected:
  /// \ru Функция инициализации по другому контейнеру. \en Function of initialization by another container. 
  void Init( const VersionContainer & );
};

#endif //__IO_VERSION_CONTAINER_H
