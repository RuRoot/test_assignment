////////////////////////////////////////////////////////////////////////////////
/** 
  \file \brief  \ru Сериализация: буфер потока, работа с диском, хранитель версий.
                \en Serialization: stream buffer, access to disk, version storage. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __IO_BUFFER_H
#define __IO_BUFFER_H


#include <templ_sp_array.h>
#include <templ_s_array.h>
#include <templ_p_array.h>
#include <math_version.h>
#include <io_version_container.h>
#include <tool_cstring.h>


class reader;
class writer;
class membuf;
class iobuf;

//------------------------------------------------------------------------------
// \ru Имена и перечисления для классов iobuf_Seq, tape, Catalog и их наследников.
// \en Names and enums for classes iobuf_Seq, tape, Catalog and their descendants.
// ---
namespace io
{
  typedef ptrdiff_t pos; ///< \ru Рабочая переменная (позиция, длина, количество). \en Working variable (position, length, count). 
  typedef ptrdiff_t off; ///< \ru Начало кластера (смещение (при работе с диском) или адрес (при работе с памятью)). \en The beginning of the cluster (shift (while working with the disk) or address (while working with the memory)). 

  /// \ru Режимы потоковых операций. \en Modes of stream operations. 
  enum mode_flags 
  {
    /// \ru Открыть для чтения. \en Open for reading. 
    in             = 0x0001,
    /// \ru Открыть для записи. \en Open for writing. 
    out            = 0x0002,
    /// \ru Открыть существующий файл и удалить его содержимое. \en Open the existent file and clear its contents. 
    trunc          = 0x0004, //-V112
    /// \ru Упорядочить при закрытии. \en Sort while closing. 
    speedOnClose   = 0x0008,
    /// \ru Удалить файл, если он оказался пустым. \en Delete file if it is empty. 
    delIfEmpty     = 0x0010,
    /// \ru Удалить файл при закрытии. \en Delete file while closing. 
    delOnClose     = 0x0020, //-V112
    /// \ru Режим восстановления. \en Recovery mode. 
    recovery       = 0x0040,
    /// \ru Вспомогательный флаг приложения. \en Auxiliary flag of application. 
    appSpecial     = 0x0080,
    /// \ru Создать новый файл. Выдается ошибка, если такой файл уже существует. \en Create a new file. An error is generated if the file already exists. 
    createNew      = 0x0100,
    /// \ru Создать новый файл. Если такой файл уже существует, то он перезаписывается. \en Create a new file. If the file already exists, then it is to be rewritten. 
    createAlways   = 0x0200,
    /// \ru Открыть существующий файл. Выдается ошибка, если файл не существует. \en Open the existent file. An error is generated if the file does not exist. 
    openExisting   = 0x0300,
    /// \ru Открыть существующий файл. Если файл не существует, то создается новый. \en Open the existent file. If the file does not exist, a new file is created. 
    openAlways     = 0x0400,
    /// \ru Открыть файл с удалением его содержимого. \en Open the file with clearing of its contents. 
    truncExisting  = 0x0500
  };

  /// \ru Направление поиска. \en Direction of search. 
  enum dir 
  {
    beg  = 0, ///< \ru С начала файла. \en From the beginning of the file. 
    cur  = 1, ///< \ru С текущей позиций от начала к концу файла. \en From the current position from the beginning to the end of the file. 
    end  = 2  ///< \ru С конца файла. \en From the end of the file. 
  };

  /// \ru Флаги состояния потока. \en Flags of stream state. 
  enum state 
  {
    /// \ru Все в порядке (никакие биты не выставлены). \en Everything is all right (no bits are set). 
    good              = 0x00000000L,
    /// \ru Конец файл. \en The end of the file. 
    eof               = 0x00000001L,
    /// \ru Выход за пределы файла. \en Out of limits of the file. 
    outOfRead         = 0x00000002L,
    /// \ru Не получилось выделить необходимую память. \en Failed to allocate the required memory. 
    outOfMemory       = 0x00000004L, //-V112
    /// \ru Ошибка операции ввода-вывода. \en Input-output operation error. 
    fail              = 0x00000008L,
    /// \ru Неверная структура файла. \en Incorrect structure of the file. 
    badData           = 0x00000010L, 
    /// \ru Файл не найден. \en File not found. 
    notFound          = 0x00000020L, //-V112
    /// \ru Доступ запрещен. \en Access is denied. 
    accessViolation   = 0x00000040L,
    /// \ru Не получилось открыть хранилище. \en Can't open the storage. 
    cantOpenStore     = 0x00000080L,
    /// \ru Не получилось создать хранилище. \en Can't create a storage. 
    cantCreateStore   = 0x00000100L,
    /// \ru Нет подписи или подпись чужая. \en There is no signature or the signature is wrong. 
    badSig            = 0x00000200L,
    /// \ru Не получилось прочитать каталог хранилища. \en Can't read the storage catalog. 
    cantReadCatalog   = 0x00000400L,
    /// \ru Не получилось записать каталог хранилища. \en Can't write the catalog of the storage. 
    cantWriteCatalog  = 0x00000800L,
    /// \ru Не получилось найти файл в каталоге. \en Can't find a file in the catalog. 
    cantFind          = 0x00001000L,
    /// \ru Не получилось прочитать файл. \en Can't read the file. 
    cantRead          = 0x00002000L,
    /// \ru Не получилось записать файл. \en Can't write the file. 
    cantWrite         = 0x00004000L,
    /// \ru Прочитанный идентификатор класса не найден в базе данных. \en The identifier of a class was not find in the data base. 
    badClassId        = 0x00008000L,
    /// \ru Попытка повторной регистрации идентификатора класса. \en Attempt for repeated registration of the class identifier. 
    doubledClassId    = 0x00010000L,
    // \ru Версия хранилища старше версии задачи. \en The storage version is older than the task version. 
    //AR storeVerViolation= 0x00020000L,
    /// \ru Версия файла старше версии задачи. \en The file version is older than the task version. 
    verViolation      = 0x00040000L,
    /// \ru Ошибка файловой операции. \en The file operation error. 
    hardFail          = 0x00080000L,
    /// \ru Буфер закрыт. \en The buffer is closed. 
    closed            = 0x00100000L,
    /// \ru У файла установлен атрибут "Только чтение". \en "Read only" attribute is set to the file. 
    writeProtect      = 0x00200000L,
    /// \ru Не могу записать объект \en Can't write the object.  
    /// \ru (версия потока младше версии появления нового класса объектов). \en (the stream version is newer than the new objects class occurrence version). 
    cantWriteObject   = 0x00400000L,
    /// \ru Не могу прочитать файл с 64-битными данными в 32-битной задаче \en Can't read file with 64-bit data in 32-bit task  
    /// \ru (потеря старшего слова uint32 при чтении uin64 в 32-битной задаче). \en (loss of upper word uint32 while reading uint64 in 32-bit task). 
    underflow64to32   = 0x00800000L,
    /// \ru Файл защищен или закодирован. \en File is protected or encrypted. 
    encrypted         = 0x01000000L,
    /// \ru Файл в расширенном формате прочитан частично (неизвестные объекты пропущены). \en Partial read of file in extended format (unknown objects skipped). 
    skippedUnknown    = 0x02000000L,
    /// \ru Чтение файла прервано пользователем. \en File reading aborted by user. 
    readAborted       = 0x04000000L,
    /// \ru Файл в расширенном формате прочитан частично (неизвестные объекты пропущены). \en Partial read of file in extended format (unknown objects skipped). 
    skippedUnknAttr   = 0x08000000L,
    /// \ru Файл нулевой длины. \en Zero-length file. 
    emptyFile         = 0x10000000L,
    /// \ru Лицензия не найдена. \en License not found. 
    licenseMissing    = 0x20000000L,
    /// \ru Запись файла прервано пользователем. \en File writing aborted by user. 
    writeAborted      = 0x40000000L,
    /// \ru Все ошибки. \en All errors. 
    allMask           = 0xffffffffL
  };
};


//------------------------------------------------------------------------------
/** \brief \ru Кластер.
           \en Cluster. \~
  \details \ru Кластер. \n
           \en Cluster. \n \~
  \ingroup Base_Tools_IO
*/
// ---
class MATH_CLASS Cluster
{
private:
  size_t m_f; ///< \ru Начало кластера (для дисковых - смещение, для памяти - адрес). \en The cluster beginning (shift - for disk, address - for memory). 
  uint16 m_l; ///< \ru Длина в байтах  (для памяти - количество заполненных байт, распределено возможно больше). \en The length in bytes (for memory - count of used bytes; more bytes can be allocated). 

public: //OV_x64 protected:
  // \ru Конструктор. \en Constructor. 
  Cluster();

  // \ru деструктора нет, т.к. кластеры хранятся в SArray и деструктор не вызывается \en there is no destructor since the clusters are stored in SArray and the destructor is not called 

public:
  // \ru Доступ к полю длины. \en Access to the length field. 
  uint16         _len() const;
  // \ru Доступ к полю начала. \en Access to the beginning field. 
  size_t         _off() const;
  // \ru Выдать адрес. \en Get the address. 
  const uint8 *  _ptr() const;
  // \ru Выдать адрес. \en Get the address. 
  uint8 *        _getMemPointer();
  // \ru Очистить. \en Clear. 
  void           clear();
  // \ru Запомнить смещение в файле и кол-во байт. \en Memorize the shift in file and the bytes count.
  void           AllocFile( size_t beg, uint16 len );
  // \ru Установить начало кластера. \en Set the cluster's beginning.
  void           SetClusterOffset( size_t off );
  // \ru Установить длину кластера. \en Set the cluster's length.
  void           SetClusterLength( uint16 len );
  // \ru Размер данных о кластере в потоке указанной версии. \en Size of cluster data in the stream of the specified version. 
  static size_t  SizeOf( VERSION version );

protected:
  void           AllocMem( uint16 len );  ///< \ru Захватить столько памяти. \en Allocate memory of the given size. 
  void           FreeMem () ;             ///< \ru Освободить память. \en Free memory. 

//OV_LNX   /// \ru Вычислить размер в памяти. \en Calculate size in memory. 
//OV_LNX   friend size_t  getMemLen   ( const Cluster& c, VERSION version ); 

  /// \ru Записать информацию о кластере. \en Write the information about the cluster. 
  friend MATH_FUNC (size_t)  WriteClusterInfo ( void *, VERSION version, const Cluster & );
  /// \ru Прочитать информацию о кластере. \en Read the information about the cluster. 
  friend MATH_FUNC (size_t)  ReadClusterInfo  ( void *, VERSION version, Cluster & );
  /// \ru Записать содержимое кластера. \en Write the cluster's contents. 
  friend MATH_FUNC (size_t)  WriteClusterBody ( void *, VERSION version, const Cluster & obj, uint16 clusterSize );
  /// \ru Прочитать содержимое кластера. \en Read the cluster's contents. 
  friend MATH_FUNC (size_t)  ReadClusterBody  ( void *, VERSION version, Cluster & obj, uint16 clusterSize );
  /// \ru Записать кластер. \en Write the cluster. 
  friend MATH_FUNC (void)    ReadCluster  ( reader &, uint16 clusterSize, Cluster & );
  /// \ru Прочитать кластер. \en Read the cluster. 
  friend MATH_FUNC (void)    WriteCluster ( writer &, const Cluster &, uint16 clusterSize );

  friend class  membuf;

public:
  bool operator < (const Cluster& e ) const { if ( m_f != e.m_f) return m_f < e.m_f; return m_l < e.m_l; }
  bool operator > (const Cluster& e ) const { return !operator < (e); }

  OBVIOUS_PRIVATE_COPY( Cluster )
};



//------------------------------------------------------------------------------
/** \brief \ru Файловое пространство.
           \en File space. \~
  \details \ru Файловое пространство.
    Место отведенное под файл (это массив индексов в массиве кластеров). \n
           \en File space.
    Space allocated for file (array of indices in array of clusters). \n \~
  \ingroup Base_Tools_IO
*/
// ---
class MATH_CLASS FileSpace
{
protected:
  uint16         m_last; ///< \ru Количество занятых байт в последнем кластере. \en Count of used bytes in the last cluster. 
  SArray<size_t> self;   ///< \ru Массив данных. \en Data array. 
public:
  /// \ru Конструктор. \en Constructor. 
  FileSpace();
  /// \ru Деструктор. \en Destructor. 
  virtual ~FileSpace();
public:
  // \ru Дать количество элементов массива. \en Get the number of elements in array.
  size_t          Count    () const;
  // \ru Обнулить количество элементов. \en Set the number of elements to zero. 
  void            Flush    ();
  // \ru Удалить элемент из массива. \en Delete an element from array. 
  void            RemoveInd( size_t idx );
  // \ru Добавить элемент в конец массива. \en Add an element to the end of the array. 
  size_t *        Add      ();
  // \ru Добавить элемент в конец массива. \en Add an element to the end of the array. 
  size_t *        Add      ( const size_t & el );
  // \ru Установить новый размер массива. \en Set the new size of an array. 
  void            SetSize  ( size_t newSize, bool clear );
  // \ru Зарезервировать место под столько элементов. \en Reserve space for a given number of elements. 
  void            Reserve  ( size_t count );
  // \ru true если элемент найден. \en true if the element found. 
  bool            IsExist  ( size_t & el ) const;
  // \ru индекс элемента, если он найден или -1. \en index of the element if it is found or -1. 
  size_t          FindIt  ( const size_t & el ) const;
  // \ru Вставить пустой элемент перед указанным. \en Insert the empty element before the specified one. 
  size_t *        InsertInd( size_t index, const size_t & el );
  // \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  const size_t *  GetAddr() const;
  // \ru Добавить n элементов в конец массива. \en Add n elements to the end of the array. 
  size_t *        AddItems ( size_t n );
  // \ru Оператор доступа по индексу. \en Operator of access by index. 
  size_t &        operator []( size_t loc ) const;
  // \ru Получить количество занятых байт в последнем кластере. \en Get the number of used bytes in the last cluster. 
  uint16  &       rest();

//OV_LNX   /// \ru Получить общий размер данных FileSpace в потоке указанной версии \en Get the total size of FileSpace data in the stream of the specified version 
//OV_LNX   friend size_t   getMemLen      ( const FileSpace& s, VERSION version ); 
//OV_LNX   /// \ru Получить размер данных FileSpace в потоке указанной версии \en Get the size of FileSpace data in the stream of the specified version 
//OV_LNX   friend size_t   SizeOfFileSpace( VERSION version, size_t cnt, bool calcFull );
  /// \ru Записать файловое пространство. \en Write the file space. 
  friend MATH_FUNC (void)     WriteFileSpace ( void *, VERSION version, const FileSpace &, bool writeFull );
  /// \ru Прочитать   файловое пространство. \en Read the file space. 
  friend MATH_FUNC (bool)     ReadFileSpace  ( void *, VERSION version, size_t & cnt, FileSpace &, const iobuf * owner, bool readFull );
  /// \ru Оператор чтения. \en Read operator. 
  friend MATH_FUNC (reader&)  operator >> ( reader &, FileSpace  & );
  /// \ru Оператор чтения. \en Read operator. 
  friend MATH_FUNC (reader&)  operator >> ( reader &, FileSpace *& );
  /// \ru Оператор записи. \en Write operator. 
  friend MATH_FUNC (writer&)  operator << ( writer &, const FileSpace & );
  /// \ru Оператор записи. \en Write operator. 
  friend MATH_FUNC (writer&)  operator << ( writer &, const FileSpace * );

  OBVIOUS_PRIVATE_COPY( FileSpace )
};

//------------------------------------------------------------------------------
/** \brief \ru Позиция в кластере для чтения/записи.
           \en Position for reading/writing in a cluster. \~
    \details \ru Позиция в кластере для чтения/записи.
             \en Position for reading/writing in a cluster. \~
    \ingroup Base_Tools_IO
*/
// ---
struct ClusterReference
{
  size_t clusterIndex; ///< \ru Индекс кластера в массиве кластеров iobuf_Seq. \en Index of the cluster in the cluster array of iobuf_Seq. 
  uint16 offset;       ///< \ru Смещение в данном кластере. \en Offset in the cluster.

  ClusterReference() : clusterIndex ( SYS_MAX_T ), offset ( static_cast<uint16>(-1) ) {}
  explicit ClusterReference( size_t idx, uint16 off ) : clusterIndex( idx ), offset( off ) {}
  ClusterReference( const ClusterReference & ref ) : clusterIndex( ref.clusterIndex ), offset( ref.offset ) {}

  bool operator == ( const ClusterReference & ref ) const {
    return clusterIndex == ref.clusterIndex && offset == ref.offset;
  }
  bool operator < ( const ClusterReference & ref ) const {
    if ( clusterIndex == ref.clusterIndex )
      return offset < ref.offset;
    return clusterIndex < ref.clusterIndex;
  }
  ClusterReference & operator = ( const ClusterReference & ref ) {
    clusterIndex = ref.clusterIndex; offset = ref.offset; return *this;
  }
  bool IsValid() const { return clusterIndex != SYS_MAX_T && offset != static_cast<uint16>(-1); }
};

//------------------------------------------------------------------------------
/** \brief \ru Потоковый буфер, обеспечивает только последовательную запись, без возможности удалить или перезаписать файл.
           \en Stream buffer. \~
  \details \ru Потоковый буфер - базовый класс. \n

    Буфер iobuf_Seq и его наследники служат для выполнения операций чтения и записи в интересах потока (класс tape). \n
    Классы iobuf_Seq, tape и их наследников следует рассматривать в совокупности. \n

    Терминология: \n
      - Хранилище - файл на диске или область памяти. \n
      - Файл - файл внутри хранилища. \n

    Класс iobuf_Seq содержит массив кластеров. \n

    Класс Кластер (Cluster) - это структурированная информация о кластере (его начало и длина).
    Он используется для операций с диском, где тогда начало - это смещение,
    и операций с памятью, где начало - это адрес в памяти.
    Все операции чтения и записи производятся по-кластерно,
    используя индекс в массиве кластеров iobuf_Seq. \n

    Класс Файловое пространство (FileSpace) - это место, отведенное под файл, это массив индексов в массиве кластеров. 
    Это как раз то, что здесь мы называем файлом. \n

    Внимание: с этого места и далее вместо слова кластер следует читать - 
    индекс кластера в массиве кластеров iobuf_Seq. \n

    \n
    Важнейшие поля данных iobuf_Seq: \n

      FileSpace sys - "системный" файл. Он открывается в конструктор класса tape. \n
        1. Если класс iobuf_Seq используется классом tape непосредственно
           (например: writer potok( "file.ext" ); 
           или reader * potok = new reader( "file.ext" ),
           то в файле sys содержится непосредственно информация, 
           которая записывалась в file.ext. \n
        2. Если класс iobuf_Seq используется классом Catalog, 
           то в sys хранится структура каталогов и файлов внутри Catalog
           (он зачитывается в конструкторе класса Catalog). \n

      PArray<FileSpace> files - список файлов содержащихся в iobuf_Seq.
        Первым эл-том в нем всегда лежит адрес sys. 
        При записи Catalog содержимое files записывается в sys, 
        исключая, естественно, первый элемент. \n

      uint32 stateFlag - состояние буфера.
        Предполагается, что все операции чтения-записи устанавливают этот флаг когда надо
        и проверяют его состояние перед реальным выполнением.
        С этим флагом работают следующие функции:
        iobuf_Seq::good, iobuf_Seq::eof, iobuf_Seq::state, iobuf_Seq::setState, iobuf_Seq::clearState;
        tape::good, tape::eof, tape::state;
        Catalog::good, Catalog::goodeof, Catalog::goodstate. \n

      VERSION storageVers - версия хранилища. \n
      VERSION curFileVers - версия текущего открытого файла (потока). \n
        В общем случае версия хранилища и версия любого файла в нем могут не совпадать. \n

      uint8 bufferMode - Режим, в котором может работать буфер. \n
      uint8 curFileMode - Режим открытия текущего файла. \n
        В общем случае режим хранилища и режим открытого файла в нем могут не совпадать. 
        Ограничение - если режим буфера io::in, то попытка открыть файл на запись (io::out) 
        не приводит к открытию. \n

      uint8 * base - Указатель на начало буфера в памяти. \n
      uint8 * ptr - Указатель на след символ в памяти. \n
      uint8 * end - Указатель на конец буфера в памяти. \n
        При работе с диском указатели устанавливаются на фиксированный
        участок памяти, куда подгружаются участки файла при чтении.
        При работе с памятью membuf устанавливает их на выделенную под кластер память. \n

    \n
    Важнейшие функции iobuf_Seq: \n
      Функция setup() - установить в buffer(переменные base,ptr,end) следующий кластер.
      Вызывается из функций overflow (при записи) и underflow (при чтении),
      когда заканчивается очередной буфер.
      Они в свою очередь вызываются из функций чтения-записи символов из потока
      (gc(), getn(), getln(), pc(), putn(), putln()).

      Функция flush() - сбросить буфер.
      Вызывается перед подъемом следующего кластера. 
      При чтении ничего не делает. При записи, при работе с диском,
      сохраняет предыдущий кластер на диск, а при работе с памятью
      запоминает размер последнего заполненного кластера. \n

    Операции чтения выполняются внутри открытого в данный момент файла, не выходя за его конец. \n
           \en Stream buffer - the base class. \n

    Buffer iobuf_Seq and its descendants are used for read and write operations in the interest of the stream (class tape). \n
    Classes iobuf_Seq, tape, Catalog and their descendants should be considered together. \n

    Terminology: \n
      - Storage - file on the disk or memory space. \n
      - Catalog - catalog inside the storage. \n
      - File - file inside the storage. \n

    Class iobuf_Seq contains a cluster array. \n

    Class Cluster is structured information about the cluster (its beginning and length).
    It is used for operations with disk, in this case the beginning is a shift,
    and for operations with memory, in this case the beginning is an address in memory.
    All the operations of reading and writing are performed clusterwise
    using the index in the array of clusters iobuf_Seq. \n

    Class FileSpace is a place allocated for the file, is an array of indices in the array of clusters. 
    It is just what we call 'file' here. \n

    Attention: from this place the word 'cluster' should be read as 
    index of cluster in the array iobuf_Seq of clusters. \n

    \n
    The most important data fields of iobuf_Seq: \n

      FileSpace sys - "system" file. It is opened in the constructor of the class tape. \n
        1. If the class iobuf_Seq is directly used by class tape
           (for instance: writer potok( "file.ext" ); 
           or reader * potok = new reader( "file.ext" ),
           then the file sys contains the information 
           which were written to file.ext. \n
        2. If the class iobuf_Seq is used by class Catalog, 
           then file sys contains the structure of catalogs and files inside Catalog
           (it is read in the constructor of class Catalog). \n

      PArray<FileSpace> files - the list of files contained in iobuf_Seq.
        The first element is always the address of sys. 
        While writing Catalog, the contents of files is written to sys, 
        except the first element, naturally. \n

      uint32 stateFlag - the buffer state.
        It is considered that all the read-write operations set this flag when it is necessary
        and check its state before the real execution.
        The following functions work with this flag:
        iobuf_Seq::good, iobuf_Seq::eof, iobuf_Seq::state, iobuf_Seq::setState, iobuf_Seq::clearState;
        tape::good, tape::eof, tape::state;
        Catalog::good, Catalog::goodeof, Catalog::goodstate. \n

      VERSION storageVers - the storage version. \n
      VERSION curFileVers - version of the current open file (stream). \n
        In the common case the storage version and the version of any file in the storage can be different. \n

      uint8 bufferMode - A possible buffer mode. \n
      uint8 curFileMode - Mode of the current file opening. \n
        In the common case the storage mode and the mode of open file in this storage can be different. 
        Constraint - if the buffer mode is io::in, then an attempt to open the file for writing (io::out) 
        doesn't result in opening. \n

      uint8 * base - Pointer to the beginning of the buffer in memory. \n
      uint8 * ptr - Pointer to the next symbol in memory. \n
      uint8 * end - Pointer to the end of buffer in the memory. \n
        When working with the disk, pointers are set to a fixed
        memory block the sections of file are loaded to while reading.
        When working with the memory, 'membuf' set them to the memory allocated for the cluster. \n

    \n
    The most important functions of iobuf_Seq: \n
      Function setup() - set the next cluster to 'buffer' (variables 'base', 'ptr', 'end').
      Called from functions 'overflow' (while writing) and 'underflow' (while reading)
      when the current buffer is over.
      These functions are in turn called from functions of reading-writing of symbols from the stream
      (gc(), getn(), getln(), pc(), putn(), putln()).

      Function flush() - flush the buffer.
      Called before getting the next cluster. 
      Do nothing while reading. When writing, when working with the disk,
      saves the previos cluster on the disk, and, when working with the memory,
      stores the size of the last filled cluster. \n

    Read operations are performed inside the file which is open at the moment, no read after end of the file. \n \~

  \ingroup Base_Tools_IO
*/
// ---
class MATH_CLASS iobuf_Seq
{
protected:
  uint8 *            base;          ///< \ru Указатель на начало буфера. \en Pointer to the beginning of the buffer. 
  uint8 *            ptr;           ///< \ru Указатель на следующий байт. \en Pointer to the next byte. 
  uint8 *            end;           ///< \ru Указатель на конец буфера. \en Pointer to the end of the file. 

  VERSION            storageVers;   ///< \ru Версия хранилища (должна быть ранее curFileVers). \en The storage version (must be before 'curFileVers'). 
  VersionContainer   curFileVers;   ///< \ru Версии текущего открытого файла (потока). \en Version of the current open file (stream). 
  VERSION            formatVersion; ///< \ru Версия формата. \en The format version. 

  FileSpace          sys;           ///< \ru Системный файл. \en System file. 
  PArray<FileSpace>  files;         ///< \ru Список файлов содержащихся в iobuf_Seq (первый элемент - адрес sys). \en List of files contained in iobuf_Seq (the first element is the address of 'sys'). 
  
  FileSpace *        curr;          ///< \ru Текущий открытый файл (поток). \en Current open file (stream). 
  size_t             part;          ///< \ru Текущий кластер в текущем открытом файле. \en Current cluster in the current open file. 

  uint16             clusterSize;   ///< \ru Размер кластера. \en Cluster size. 

  uint8              bufferMode;    ///< \ru Режим работы буфера. \en Buffer mode. 
                                    //AR todo io::mode

  uint8              curFileMode;   ///< \ru Режим открытия текущего файла. \en Mode of opening the current file. 

private:
  uint32             stateFlag;     ///< \ru Состояние буфера. \en The buffer state. 
                                    // todo io::state
  SArray<Cluster>    content;       // \ru Массив кластеров. \en Clusters array.

protected:
  bool               modifiedFlag;  ///< \ru Буфер модифицирован. \en The buffer has been modified. 
  bool               freshFlag;     ///< \ru Свежий ли буфер. \en Is the buffer fresh. 

public:
  /// \ru Конструктор \en Constructor 
           iobuf_Seq( uint16 clusterSize );
  /// \ru Деструктор. \en Destructor. 
  virtual ~iobuf_Seq() {}

public:

  // \ru Методы доступа к массиву кластеров. \en Methods for an access to the clusters array.

  ///< \ru Зарезервировать место под заданное количество элементов. \en Reserve space for a given number of elements. 
  void       Reserve     ( size_t n, bool addAdditionalSpace = true );
  ///< \ru Обнулить количество элементов. \en Set the number of elements to null. 
  void       Flush      ();
  ///< \ru Освободить всю память. \en Free the whole memory. 
  void       HardFlush  ();
  ///< \ru Удалить лишнюю память. \en Free the unnecessary memory. 
  void       Adjust     ();
  ///< \ru Добавить элемент в конец массива. \en Add an element to the end of the array. 
  Cluster *  Add        ();
  ///< \ru Добавить данный элемент в конец массива. \en Add a given element to the end of the array. 
  Cluster *  Add        ( const Cluster &e );
  ///< \ru Дать количество элементов массива. \en Get the number of elements in array. 
  size_t     Count      ()    const;
  /// \ru Оператор доступа по индексу. \en Access by index operator. 
  Cluster &  operator []( size_t loc ) const;

  ///< \ru Зарезервировать место под заданное количество файлов. \en Reserve space for a given number of FileSpace. 
  void       ReserveFiles( size_t n, bool addAdditionalSpace ) { files.Reserve( n, addAdditionalSpace ); }

public:

          /// \ru Получить следующий байт из буфера. \en Get the next byte from the buffer. 
          int           gc   ();                   // get next char and advance
          /// \ru Получить следующие n байт из буфера поэлементно. \en Get the next n bytes from the buffer elementwise. 
          size_t        getn ( void *, size_t );   // get next n chars and advance
          /// \ru Получить следующие n байт из буфера копированием участка памяти. \en Get the next n bytes from the buffer by copying the storage area. 
          size_t        getln( void *, size_t  );  // get next n chars and advance
          /// \ru Получить очередной байт из буфера, но указатель на следующий не сдвигать. \en Get the next byte from the buffer but don't shift the pointer to the next one. 
//AR      int           ch   ();                   // return next char and do not advance
          /// \ru Поместить байт в буфер. \en Put byte to the buffer. 
          int           pc   ( uint8 c );
          /// \ru Поместить следующие n байт в буфер поэлементно \en Put the next n bytes to the buffer elementwise. 
          size_t        putn ( const void *, size_t );  // put n chars with 'pc' func and advance
          /// \ru Поместить следующие n байт в буфер копированием участка памяти. \en Put the next n bytes to the buffer by copying the storage area. 
          size_t        putln( const void *, size_t );  // put long string and advance
          /// \ru Если кончился текущий буфер, переместить указатель на следующий. \en If current buffer exhausted, advance pointer to the next one. 
          void          advance();
          /// \ru Корректно ли состояние буфера. \en Whether the buffer state is correct. 
          bool          good () const;
          /// \ru Достигнут ли конец файла? \en Is the end of file reached? 
          bool          eof  () const;
          /// \ru Получить состояние буфера. \en Get the buffer state. 
          uint32        state() const; //AR getState
          /// \ru Добавить состояние буфера. \en Add the buffer state. 
          void          setState  ( io::state add ); /*AR const*/
          /// \ru Убрать состояние буфера. \en Remove the buffer state. 
          void          clearState( io::state sub ); /*AR const*/
          /// \ru Посчитать размер текущего файла. \en Calculate the current file size. 
          io::pos       size() const;
          /// \ru Возвращает текущую позицию в файле \en Returns the current position in the file. 
          io::pos       tell() const;
          /// \ru Установить текущую позицию в буфере \en Set current position in the buffer. 
          void          lseek   ( size_t pos = SYS_MAX_T );
          /// \ru Присоединить файл к буферу с проверкой или без. \en Attach the file to the buffer with or without checking. 
          bool          attach  ( FileSpace & file, bool check = true );
          /// \ru Открыть файл, если он свой. Флаг fullCheck == false отключает избыточные проверки (ради производительности).
          /// \en Open the file if it is one's own file. The flag fullCheck == false switches off excessive checks (for the sake of performance).
  virtual bool          open   ( FileSpace & file, uint8 om, const VersionContainer &, bool fullCheck = true );
          /// \ru Открыть системный файл. \en Open the system file. 
          bool          openSys( uint8 om ); 
          /// \ru Закрыть файл. \en Close the file. 
  virtual void          close  ();
          /// \ru Закрыть буфер. \en Close the buffer. 
  virtual void          closeBuff();

          /// \ru Установить для записи FileSpace с заданным индексом (при необходимости создать новый).
          ///\en Set FileSpace with given index for writing (create if necessary). 
  virtual FileSpace *   enterFileSpace  ( uint8 ) { return nullptr; } // не реализовано; not implemeneted
          /// \ru Установить позицию для записи/чтения по заданному ClusterReference.
          /// Сохранить предыдущую позицию, если saveCurr = true.
          /// \en Set position for for writing/reading by given ClusterReference.
          /// If saveCurr = true, save previous position.
  virtual FileSpace *   enterFileSpace ( const ClusterReference & , bool ) { return nullptr; }  // не реализовано; not implemeneted
          /// \ru Установить позицию для записи/чтения по заданным FileSpace и ClusterReference.
          /// Внимание, здесь ClusterReference.clusterIndex должен содержать индекс в массиве индексов кластеров в FileSpace!
          /// Сохранить предыдущую позицию, если saveCurr = true.
          /// \en Set position for writing/reading by given FileSpace and ClusterReference.
          /// Warning: in this function, ClusterReference.clusterIndex should contain an index in array of cluster indices in FileSpace!
          /// If saveCurr = true, save previous position.
  virtual FileSpace *   enterFileSpace ( const ClusterReference &, FileSpace *, bool ) { return nullptr; }  // не реализовано; not implemeneted
          /// \ru Установить предыдущий FileSpace для записи/чтения.
          ///\en Set previous FileSpace for writing/reading. 
  virtual FileSpace *   returnToPreviousFileSpace() { return nullptr; }  // не реализовано; not implemeneted

          /// \ru Получить текущую позицию в буфере. \en Get current position in the buffer. 
          ClusterReference getCurrentClusterPos();

          /// \ru Получить доступ к системному файлу. \en Get access to the system file. 
          FileSpace &   sysFile   ();
          /// \ru Получить доступ к открытому файлу. \en Get access to the open file. 
          FileSpace *   openedFile()          const;

          /// \ru Размер данных хранилища (файла на диске). \en The storage data size (of the file on the disk). 
  virtual size_t        DOSFileLen ()         const;
          /// \ru Имя хранилища (файла на диске). \en Storage name (of file on the disk). 
  virtual const TCHAR * DOSFileName()         const;

          /// \ru Свежий ли буфер? \en Is the buffer fresh? 
          bool          fresh()               const;
          /// \ru Установить состояние свежести буфера. \en Set the state of buffer freshness. 
          void          fresh( bool f );
          /// \ru Модифицирован ли буфер? \en Is the buffer modified? 
          bool          modified()            const;
          /// \ru Установить состояние модифицированности буфера. \en Set the state of modified buffer. 
          void          modified( bool m );
          /// \ru Узнать режим работы буфера. \en Get the buffer mode. 
          uint8         mode()                const; // getMode
          /// \ru Установить режим работы буфера. \en Set the buffer mode. 
          void          mode( uint8 m ); // curMode
          void          setMode( uint8 m );
          /// \ru Находимся в режиме чтения? \en Is in the reading mode? 
          bool          IsInMode()            const;
          /// \ru Находимся в режиме записи? \en Is in the writing mode? 
          bool          IsOutMode()           const;
          /// \ru Находимся в режиме чтения или записи? \en Is in the reading or writing mode? 
          bool          IsInOrOutMode()       const;
          /// \ru Нужно ли удалять пустой файл? \en Is the empty file to be deleted? 
          bool          deleteIfEmpty()       const;
          /// \ru Установить флаг необходимости удаления пустого файла. \en Set the flag of deleting the empty file. 
          void          deleteIfEmpty( bool s );
          /// \ru Нужно ли удалять файл при закрытии буфера? \en Is the file to be deleted while closing the buffer? 
          bool          deleteOnClose()       const;
          /// \ru Установить флаг необходимости удаления файла при закрытии буфера. \en Set the flag of deleting the file while closing the buffer. 
          void          deleteOnClose( bool s );
          /// \ru Установить текущую версию равной версии хранилища. \en Set the current version to be equal to the storage version. 
          void          SetVersionsByStorage();
          /// \ru Вернуть главную версию (математического ядра). \en Return the main version (of the mathematical kernel). 
          VERSION       MathVersion()         const;
          /// \ru Вернуть дополнительную версию (конечного приложения). \en Return the additional version (of the target application). 
          VERSION       AppVersion( size_t ind = -1 )     const;
          /// \ru Получить версии буфера. \en Get the buffer versions. 
          const VersionContainer & GetVersionsContainer() const;
protected:
          int           underflow();          ///< \ru Вызывается когда прочитан весь буфер, а еще хочется. \en Called when the whole buffer are read, but it is necessary to continue reading. 
          int           overflow( uint8 ch ); ///< \ru Вызывается когда заполнен весь буфер, а еще хочется. \en Called when the buffer is full but it is necessary to continue writing. 
  virtual int           setup() = 0;          ///< \ru Установить следующий буфер. \en Set the next buffer. 
  virtual int           flush() = 0;          ///< \ru Сбросить буфер. \en Flush the buffer. 
          void          checkEof();           ///< \ru Установить конец файла, если необходимо. \en Set the end of file if necessary. 

          /// \ru Узнать количество необработанных байт в буфере. \en Get the number of unprocessed bytes in the buffer. 
          size_t        avail  () const;
          /// \ru Узнать количество обработанных байт в буфере. \en Get the number of processed bytes in the buffer. 
          size_t        waiting() const;

          bool          mine  ( FileSpace &  ); ///< \ru Проверить, мой ли это файл. \en Check if the file is mine. 

          /// \ru Установить версию открытого файла. \en Set the version of open file. 
          void          SetVersionsContainer( const VersionContainer & vers );
          /// \ru Установить версию хранилища. \en Set the storage version. 
          VERSION       SetStorageVersion( VERSION ); 
public:
          /// \ru Узнать версию хранилища. \en Get the storage version. 
          VERSION       GetStorageVersion();

          /// \ru Узнать версию формата. \en Get the format version. 
          VERSION       GetFormatVersion() const;
          /// \ru Установить версию формата. \en Set the format version. 
          void          SetFormatVersion ( VERSION version );

  friend  class tape;

  OBVIOUS_PRIVATE_COPY( iobuf_Seq );
};


//------------------------------------------------------------------------------
/** \brief \ru Потоковый буфер с произвольным доступом
             , расширяет функциональность iobuf_Seq возможностью удаления и перезаписи файлов.
           \en Stream buffer. \~
  \details \ru Потоковый буфер - базовый класс. \n
  
  FileSpace freed - список освобожденных кластеров.
  При удалении файла все его кластеры переносятся сюда.
  При записи, когда необходимо распределить новый кластер, 
  сначала проверяется этот массив на наличие свободных кластеров.
  Кластеры в freed всегда лежат упорядоченными по смещению от начала файла. \n

  \en Stream buffer - the base class. \n
  
  FileSpace freed - the list of freed clusters.
  When the file is deleted, all its clusters are moved here.
  When the modified Catalog is being closed, all the clusters are deleted from 'freed'
  which physically lie after the last used cluster.
  After that the contents of 'freed' is written to 'sys'.
  While writing, when it is necessary to allocate the new cluster, 
  this array is firstly checked for availability of free clusters.
  Clusters in 'freed' are always ordered by shift from the beginning of the file. \n

  */
// ---
class MATH_CLASS iobuf : public iobuf_Seq
{
protected:
  FileSpace          freed;         ///< \ru Список освобожденных кластеров. \en List of released clusters. 

public:
  /// \ru Конструктор \en Constructor 
  iobuf( uint16 clusterSize );
  /// \ru Деструктор. \en Destructor. 
  virtual ~iobuf();

public:
  /// \ru Открыть файл, если он свой. Флаг fullCheck == false отключает избыточные проверки (ради производительности).
  /// \en Open the file if it is one's own file. The flag fullCheck == false switches off excessive checks (for the sake of performance).
  virtual bool  open   ( FileSpace & file, uint8 om, const VersionContainer &, bool fullCheck = true );

  /// \ru Освободить место, занимаемое файлом. \en Free space allocated for the file. 
  bool          del     ( FileSpace & file );
  /// \ru Урезать файл. \en Truncate the file. 
  bool          truncate( FileSpace & file, size_t  from ); 

  ///< \ru Отсоединить файл от буфера. \en Detach file from the buffer. 
  bool          detach( FileSpace & );  

  /// \ru Нужно ли упорядочивать при закрытии. \en Is to be ordered while closing. 
  bool          speedOnClose()        const;
  /// \ru Установить флаг необходимости упорядочивания при закрытии. \en Set the flag of ordering while closing. 
  void          speedOnClose( bool s );

  virtual void  free( size_t c );  ///< \ru Освободить кластер. \en Free the cluster. 
  
  friend  class tape;

  OBVIOUS_PRIVATE_COPY( iobuf );
};


//------------------------------------------------------------------------------
/// \ru Конец файла. \en End of file. \~ \ingroup Base_Tools_IO
//---
#define EOF (-1)

//------------------------------------------------------------------------------
/**\ru Сравнить два кластера.
\en Compare two clusters. \~
\ingroup Base_Tools_IO
*/
//---
inline bool IsEqualSArrayItems( const Cluster & /*obj1*/, const Cluster & /*obj2*/ ) { 
  return 0;
}

//------------------------------------------------------------------------------
/// \ru Длина данных size_t в потоке. \en Length of size_t data in the stream. \~ \ingroup Base_Tools_IO
// ---
inline size_t LenCOUNT( VERSION version )
{
  if ( IsVersion64bit(version) )
    return sizeof(uint64);
  else
    return sizeof(uint32);
}


//------------------------------------------------------------------------------
/// \ru Размер данных FileSpace в потоке указанной версии. \en Length of FileSpace data in stream of the specified version. \~ \ingroup Base_Tools_IO
// ---
inline size_t SizeOfFileSpace( VERSION version, size_t cnt, bool calcFull )   
{
  const size_t lenCount = IsVersion16bit(version) ? sizeof( uint16 ) 
    : LenCOUNT( version );

  size_t len  = (calcFull ? lenCount : 0); // count
  len += cnt * lenCount;
  len += (calcFull ? sizeof(uint16) : 0); // m_last

  return len;
}

//------------------------------------------------------------------------------
/** \brief \ru Размер данных кластера.
\en Size of cluster's data. \~
\details \ru Размер данных кластера в потоке указанной версии. \n
\en Size of cluster data in the stream of the specified version. \n \~
\ingroup Base_Tools_IO
*/
//---
inline size_t getMemLen( const Cluster & c, VERSION /*version*/ ) 
{
  // \ru для кластера будет записываться uint16(кол-во заполненных байт) \en the following number will be stored for a cluster: uint16 (the number of filled bytes) 
  // \ru плюс заполненные байты \en plus filled bytes 
  return (c._len() + sizeof(uint16));
}


//------------------------------------------------------------------------------
/** \brief \ru Размер данных файла.
\en The file data size. \~
\details \ru Размер данных файлового пространства в потоке указанной версии. \n
\en The file space data size in the stream of the specified version. \n \~
\ingroup Base_Tools_IO
*/
// ---
inline size_t getMemLen( const FileSpace & s, VERSION version ) {
  return SizeOfFileSpace( version, s.Count(), true/*calcLast*/ );
}

//------------------------------------------------------------------------------
/** \brief \ru Проверить массив кластеров файла.
\en Check the cluster array of the file. \~
\details \ru Проверить массив кластеров, из которых состоит этот файл. \n
\en Check array of clusters this file consists of. \n \~
\ingroup Base_Tools_IO
*/
// ---
inline bool IsGoodFile( const FileSpace & file, const iobuf_Seq & owner )
{
  bool good = true;

  size_t clustersCount = owner.Count();
  for ( size_t i = 0, fileCount = file.Count(); i < fileCount && good ; i++ ) 
  {
    size_t fileIndex = file[i];
    good = ( static_cast<ptrdiff_t>(fileIndex) >= 0 && static_cast<ptrdiff_t>(fileIndex) < static_cast<ptrdiff_t>(clustersCount) );
  }

  return good;
}

#endif // __IO_BUFFER_H
