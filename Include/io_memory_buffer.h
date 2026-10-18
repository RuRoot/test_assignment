////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Сериализация: буфер в памяти.
         \en Serialization: memory buffer. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __IO_MEMORY_BUFFER_H
#define __IO_MEMORY_BUFFER_H


#include <io_buffer.h>
#include <fstream>
#include <tool_cstring.h>
#include <math_define.h>
#include <math_version.h>
#include <stack>
#include <map>

class mem;
class iobuf;
class reader;
class writer;



//------------------------------------------------------------------------------
/** \brief \ru Потоковый буфер памяти.
           \en Memory stream buffer. \~
  \details \ru Потоковый буфер памяти. \n
      Потоковый буфер памяти предназначен для использования в потоках чтения и записи.
  Кроме необходимых для буфера принадлежностей имеет функции для упаковки в непрерывный блок памяти, 
  что позволяет передавать файлы через память в другое приложение.
           \en Memory stream buffer. \n
      Stream memory buffer is intended for using in read and write streams. Besides the 
  instruments needed for the buffer, it has functions for packing to a contiguous memory block, 
  what allows to transfer files to another application via the memory. \~
            \ingroup Base_Tools_IO
*/
// ---
class MATH_CLASS membuf : public iobuf
{
protected:
  /// \ru Максимальное количество регистрируемых объектов. \en The maximum number of registered facilities. 
  size_t maxRegCount;

  /// \ru Вектор filesPool хранит все используемые FileSpaces ,кроме sys, который определен в iobuf_Seq.
  /// При использовании расширенного формата объекты каждого уровня записываются в отдельный FileSpace. 
  /// При этом индекс FileSpace в векторе соответствует уровню объекта в модели.
  /// \en Keep all used FileSpaces (except sys, which is defined in iobuf_Seq). 
  /// When using the extended format, objects of each level are saved to a separate FileSpace.
  /// Index of FileSpace in the vector corresponds to the level of the object in the model.
  std::vector<FileSpace *> filesPool;

  /// \ru Стек FileSpaces.
  /// \en Stack of FileSpaces.
  struct FileStackEntry
  {
    ClusterReference  _ref;
    FileSpace *       _file;

    FileStackEntry() : _file(nullptr) {}
    FileStackEntry ( ClusterReference r, FileSpace * f ) : _ref(r), _file(f) {}
  };
  std::stack<FileStackEntry> filesStack;

  /// \ru Кэш для данных о кластерах в FileSpace.
  /// map: индекс кластера в массиве кластеров iobuf_Seq -> данные FileSpace (указатель на FileSpace и индекс в массиве индексов кластеров).
  /// \en Cache for clusters data in FileSpace. 
  /// map: cluster index in cluster array iobuf_Seq -> FileSpace data (pointer to FileSpace and index in array of cluster indices in FileSpace).
  std::map<size_t, std::pair<FileSpace *, size_t> > fileClusterIndexCache;

public:
  /// \ru Конструктор. \en Constructor. 
  membuf();

  /// \ru Деструктор. \en Destructor. 
  virtual ~membuf();

public:
  /// \ru Буфер пуст? \en Is the buffer empty? 
          bool        isEmpty() const;
  /// \ru Записать в непрерывную память.
  ///     Функция подразумевает вполне определенное толкование значений входных данных, поэтому она не должна вызываться с неинициализированными аргументами.
  /// \param[in,out] memory  - память, куда писать. Если memory == nullptr, то память выделяется.
  /// \param[in]     addSize - размер памяти, которую надо дополнительно выделить при выделении памяти.
  ///     Смысл addSize зависит от начального значения параметра memory:
  ///       если memory != nullptr (т.е.память уже распределена), то addSize должен быть равен размеру памяти (addSize >= getMemLen() !!!).
  ///       если memory == nullptr, то addSize определяет, столько байт дополнительно добавить (обнулив) в начале при выделении памяти.
  /// \en Write to contiguous memory.
  ///     The function implies a well-defined interpretation of the input values, so it should not be called with uninitialized arguments.
  /// \param[in,out] memory  - memory to write to. If memory == nullptr, then memory is allocated.
  /// \param[in]     addSize - size of memory, which should be allocated additionally when allocating memory.
  ///     The meaning of addSize depends on the initial value of the parameter 'memory':
  ///       if memory != nullptr (i.e. the memory is already allocated), then addSize should be equal to memory size (addSize >= getMemLen() !!!).
  ///       if memory == nullptr, then addSize defines a number of bytes to be added (and zeroed) at the beginning when allocating memory.
          size_t      toMemory( const char *& memory, size_t addSize = 0 ) const;
  /// \ru Прочитать из непрерывной памяти. \en Read from the contiguous memory. 
          bool        fromMemory( const char * memory );
  /// \ru Вычислить необходимую длину непрерывного куска памяти для буфера. \en Compute the necessary length of the contiguous memory block for a buffer. 
          size_t      getMemLen() const;

  // \ru Установить максимальное количество регистрируемых объектов. \en Set the maximum number of registered facilities. 
          void        setMaxRegCount( size_t n );
  // \ru Получить максимальное количество регистрируемых объектов. \en Get the maximum number of registered facilities. 
          size_t      getMaxRegCount() const;

  /// \ru Подготовить поток чтения. \en Prepare read stream. 
          reader &    read( reader & in );
  /// \ru Подготовить поток записи. \en Prepare write stream. 
          writer &    write( writer & out ) const;
  /// \ru Очистить буфер. \en Clear the buffer. 
          void        clean();

  ///< \ru Закрыть буфер. \en Close the buffer.
  virtual void        closeBuff();  

  /// \ru Оператор чтения. \en Read operator. 
  friend MATH_FUNC (reader &)     operator >> ( reader & in, membuf *& ptr );
  /// \ru Оператор чтения. \en Read operator. 
  friend MATH_FUNC (reader &)     operator >> ( reader & in, membuf & ref );
  /// \ru Оператор записи. \en Write operator. 
  friend MATH_FUNC (writer &)     operator << ( writer & out, const membuf * ptr );
  /// \ru Оператор записи. \en Write operator. 
  friend MATH_FUNC (writer &)     operator << ( writer & out, const membuf & ref );

protected:
  virtual int         setup();                  ///< \ru Установить буфер для следующего кластера. \en Set the buffer for the next cluster. 
  virtual int         flush();                  ///< \ru Сбросить буфер. \en Flush the buffer. 
          void        PrepareToRead( const VersionContainer & vers ); ///< \ru Инициировать все поля перед чтением. \en Initialize all the fields before reading. 
                                                        
  /// \ru Установить новый FileSpace для записи объекта уровня level. \en Set new FileSpace for writing an object of given level. 
  virtual FileSpace * enterFileSpace ( uint8 level );
  /// \ru Установить позицию для записи/чтения по заданному ClusterReference.
  /// Сохранить предыдущую позицию в стеке File Spaces, если updateStack = true.
  /// \en Set position for writing/reading by given ClusterReference.
  /// If updateStack = true, push previous position to the File Spaces stack.
  virtual FileSpace * enterFileSpace ( const ClusterReference & ref, bool updateStack );
  /// \ru Установить позицию для записи/чтения по заданным FileSpace и ClusterReference.
  /// Внимание, в данном случае ClusterReference.clusterIndex указывает на индекс в массиве индексов кластеров в FileSpace!
  /// Сохранить предыдущую позицию в стеке File Spaces, если updateStack = true.
  /// \en Set position for writing/reading by given FileSpace and ClusterReference.
  /// Warning: in this case, ClusterReference.clusterIndex is an index in the array of cluster indices in FileSpace!
  /// If updateStack = true, push previous position to the File Spaces stack.
  virtual FileSpace * enterFileSpace ( const ClusterReference & ref, FileSpace * file, bool updateStacke );
  /// \ru Извлечь предыдущий FileSpace из стека и установить его для записи/чтения. \en Pop the previous FileSpace from the stack and set it up for writing/reading. 
  virtual FileSpace * returnToPreviousFileSpace();

private:
          bool        alloc();                    // \ru Добавить новый кластер; \en Add a new cluster. 
          bool        fromFilebuf( mem & p );     // \ru Прочитать данные из файла записанного через дисковый буфер. \en Read data from the file recorded via the disk buffer. 
          void        toFilebuf( mem & p, VERSION& version ) const; // \ru Записать данные. \en Write data. 

OBVIOUS_PRIVATE_COPY( membuf )
};

//------------------------------------------------------------------------------
/// \ru Прочитать буфер с диска. \en Read the buffer from the disk. \~ \ingroup Base_Tools_IO
// ---
#ifndef __MOBILE_VERSION__
MATH_FUNC (iobuf &) createiobuf( const TCHAR * fileName );

//------------------------------------------------------------------------------
/// \ru Записать буфер на диск. \en Write the buffer to the disk. \~ \ingroup Base_Tools_IO
// ---
MATH_FUNC (bool) writeiobuftodisk( const TCHAR * fileName, membuf & buf );
#endif // __MOBILE_VERSION__

#endif // __IO_MEMORY_BUFFER_H
