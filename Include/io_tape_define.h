////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Сериализация: чтение и запись потоковых классов. Основные определения.
         \en Serialization: reading and writing of stream classes. Main definitions. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __IO_TAPE_DEFINE_H
#define __IO_TAPE_DEFINE_H


////////////////////////////////////////////////////////////////////////////////
//
// \ru Классы, для которых при записи и чтении точно известен тип, могут записываться в поток
//     и читаться из потока с помощью операторов << и >>.
//    Это, например, классы лежащие в массиве SArray. 
// \en Classes for which the type is exactly known while reading and writing can be written
//     to the stream and read from the stream using << and >> operators. 
//     They are, for instance, classes contained in array SArray.
//
// \ru Для таких объектов необходимо в описании класса установить : \en For such objects one should set in the class definition: 
// \ru KNOWN_OBJECTS_RW_REF_OPERATORS( Class ) - для работы со ссылками и объектами класса \en KNOWN_OBJECTS_RW_REF_OPERATORS( Class ) - for work with references and class objects 
// \ru KNOWN_OBJECTS_RW_PTR_OPERATORS( Class ) - для работы с указателями на объекты класса \en KNOWN_OBJECTS_RW_PTR_OPERATORS( Class ) - for work with pointers to class objects 
//
// \ru и определить тела самих операторов : \en and define the solids of operators: 
//
// \ru -- для ссылок \en -- for references 
//  inline reader& operator >> ( reader& in, Class& ref ) {
//    return in >> ref. >> ref.... ;
//  }
//
//  writer& operator << ( writer& out, const Class& ref ) {
//    return out << ref. << ref. ... ;
//  }
//
// \ru -- для указателей \en -- for pointers 
//  inline reader& operator >> ( reader& in, Class*& ptr ) {
//    ptr = new Class(...);
//    return in >> ptr-> >> ptr->.... ;
//  }
//
//  writer& operator << ( writer& out, const Class* ptr ) {
//    return out << ptr-> << ptr-> ... ;
//  }
//
// \ru Классы, для которых при записи и чтении тип не известен. \en Classes for which the type is unknown while writing and reading. 
// \ru Для описания такого класса Class как поточного в общем случае требуется : \en There are the following requirements for description of such class Class as a stream class: 
// \ru -- Наследовать класс от TapeBase напрямую или через своих предков \en -- Inherit the class from TapeBase directly or via ancestors 
// \ru Примечание : \en Note: 
// \ru Если класс наследует более чем от одного поточного класса, \en If the class is inherited from more than one stream class, 
// \ru то его предки !!!обязательно!!! должны наследовать от TapeBase \en then its parents MUST be inherited from TapeBase 
// \ru виртуально, т.е. \en virtually, i.e. 
//           class A : public virtual TapeBase {
//             ...
//           };
//
//           class B : public virtual TapeBase {
//             ...
//           };
//
//           class C : public A, public B {
//             ...
//           };
//
// \ru При этом классы A,B равно как и C нельзя укладывать в SArray, \en At the same time classes A,B cannot be put to array SArray, as well as class C, 
// \ru поскольку они неявно содержат указатель на виртуальную базу \en since they implicitly contain a pointer to the virtual base 
//
// \ru -- В декларации класса установить : \en -- Set in declaration of the class: 
// \ru DECLARE_PERSISTENT_CLASS( Class ) - если Class не имеет поточных предков \en DECLARE_PERSISTENT_CLASS( Class ) - if Class does not have stream ancestors 
//
// \ru -- В любом *.cpp файле установить : \en -- Set in any *.cpp file: 
// \ru IMP_PERSISTENT_CLASS( AppID, Class ); - требуется написание конструктора \en IMP_PERSISTENT_CLASS( AppID, Class ); - the constructor implementation is required 
//
// \ru -- Описать тела функций : \en -- Describe the solids of functions: 
//        void Class::Read( reader& in, Class* obj );
//        void Class::Write( writer& out, const Class* obj );
//
// \ru Если Class не содержит своих полей данных, которые необходимо \en If Class does not contain its own data fields which should 
// \ru писать в поток и читать оттуда, то вместо \en be written to stream and be read from stream, then instead of 
// \ru IMP_PERSISTENT_CLASS следует применять \en IMP_PERSISTENT_CLASS one should use 
//    IMP_PERSISTENT_CLASS_FROM_BASE( Class, Base ),
// \ru при этом не надо определять функции Class::Read и Class::Write \en at that the functions Class::Read and Class::Write don't have to be defined 
//
// \ru Если Class абстрактный -- применять \en If Class is abstract - apply 
//    IMP_A_PERSISTENT_CLASS( Class );
//
// \ru Если Class не наследует ни от кого кроме TapeBase и плюс к этому \en If Class does not inherit from any class except TapeBase and, in addition, 
// \ru не имеет полей данных для записи, применять : \en does not have data fields for writing, apply: 
// \ru в cpp-файле \en in cpp-file 
// \ru для абстрактного -- IMP_AWD_PERSISTENT_CLASS( Class ); \en for the abstract one -- IMP_AWD_PERSISTENT_CLASS( Class ); 
// \ru для обычного     -- IMP_WD_PERSISTENT_CLASS( Class ); \en for the ordinary one    -- MP_WD_PERSISTENT_CLASS( Class ); 
// \ru примечание : WD - Without Data \en note: WD - Without Data 
//
// \ru Если Class наследует более чем от одного TapeBase'а \en If Class inherit from more than one TaperBase class, 
// \ru наследование от него должно быть virtual'ным, например : \en the inheritance from it should be virtual, for instance: 
//    class first : virtual public TapeBase {
//      ...
//      DECLARE_PERSISTENT_CLASS( AppID, first );
//    };
//
//    class second : virtual public TapeBase {
//      ...
//      DECLARE_PERSISTENT_CLASS( AppID, second );
//    };
//
//    class third : public first, public second {
//      ...
//      DECLARE_PERSISTENT_CLASS( AppID, third );
//    };
//    IMP_PERSISTENT_CLASS( AppID, first )
//    IMP_PERSISTENT_CLASS( AppID, second )
//    IMP_PERSISTENT_CLASS( AppID, third )
// \ru + функции чтения - записи \en + functions of reading-writing 
// \ru + соответствующие конструкторы чтения \en + the corresponding reading constructors   
//
// \ru Если Class template'ный, то для описания класса поточным требуется : \en If Class is a template class, then the following is required for definition the class as a stream class: 
// \ru -- Наследовать template от TapeBase \en -- Inherit template from TapeBase 
//
// \ru -- В декларации template'а установить : \en -- Set in the declaration of template : 
//        DECLARE_T_PERSISTENT_CLASS( Templ, Arg );
// \ru где Templ - имя самого template'а \en where Templ is the name of template 
// \ru Arg   - имя формального template'ного аргумента \en Arg   - the name of formal argument of template 
//
// \ru -- В этом же h-файле вне декларации template'а установить \en -- Set in the same h-file outside the template declaration 
//        IMP_T_PERSISTENT_OPS( Templ );
//
// \ru -- В этом же h-файле вне декларации template'а  описать тела функций : \en -- In the same h-file outside the template declaration describe solids of functions: 
//        template <class Arg>
//          void Templ<Agr>::Read( reader& in, Templ<Agr>* obj );
//        template <class Arg>
//          void Templ<Agr>::Write( writer& out, const Templ<Agr>* obj );
// \ru где \en where 
// \ru Arg - формальный аргумент \en Arg - formal argument 
//
// \ru -- В любом(ых) cpp-файле(ах) установить для каждого применения template'а : \en -- In any cpp-files set for each use of template: 
//        IMP_T_PERSISTENT_CLASS( Templ, Class );
// \ru где \en where 
// \ru Class - имя класса с которым применяется template ( фактический аргумент ) \en Class - name of the class the template is used with (the actual argument) 
//
// \ru Примечание : для классов List, DList, SArray, PArray, Array2 все действия \en Note: for classes List, DList, SArray, PArray, Array2 all the instructions 
// \ru уже выполнены, кроме последнего пункта. \en already applied except the last one. 
//
//
// \ru По поводу функции Class::Read( reader& in, Class* obj ) рекомендуется \en Regarding function Class::Read( reader& in, Class* obj ) it is recommended 
// \ru обратить внимание на : \en to pay attention to: 
//
// \ru 1.Поля данных базового класса самостоятельно читать не нужно, вместо этого \en 1.Data fields of the base class don't require any special code to be read, instead of it 
// \ru нужно вызвать функцию ReadBase( out, (Base*)obj ). \en one should call function ReadBase( out, (Base*)obj ). 
// \ru Вызывать ее желательно в голове функции Class::Read \en It is desirable to call it in the head of function Class::Read 
// \ru Обратите внимание на преобразование типа (Base*)obj ! \en Pay attention to the conversion like (Base*)obj ! 
//
// \ru 2.Функция Class::Read декларирована статической (static), поэтому она не имеет \en 2. Function Class::Read is declared as static so it has no 
// \ru указателя this ==> не пытайтесь делать что-нибудь типа : \en 'this' pointer ==> don't try to do something like: 
//        in >> field
// \ru правильно : \en the correct variant is: 
//        in >> obj->field;
//
// \ru По поводу функции Class::Write( writer& out, const Class* obj ) рекомендуется \en As for function Class::Write( writer& out, const Class* obj ), it is recommended 
// \ru обратить внимание на : \en to pay attention to: 
//
// \ru 1.Поля данных базового класса самостоятельно писать не нужно, вместо этого \en 1.Data fields of the base class don't require any special code to be written, instead of it 
// \ru нужно вызвать функцию WriteBase( out, (const Base*)obj ). \en one should call function WriteBase( out, (const Base*)obj ). 
// \ru Вызывать ее желательно в голове функции Class::Write \en It is desirable to call it in the head of function Class::Write 
// \ru Обратите внимание на преобразование типа (const Base*)obj ! \en Pay attention to the conversion like (const Base*)obj ! 
//
// \ru 2.Функция Write декларирована статической (static), поэтому она не имеет \en 2.Function Class::Write is declared as static so it has no 
// \ru указателя this ==> не пытайтесь делать что-нибудь типа : \en 'this' pointer ==> don't try to do something like: 
//        out << field
// \ru правильно : \en the correct variant is: 
//        out << obj->field;
//
//
// \ru По поводу поддержки версий : \en As for versions support: 
// \ru 1. При записи - \en 1. While reading - 
// \ru iobuf имеет статическое поле данных iobuf_defaultVersionCont - контейнер версии \en iobuf has static data field iobuf_defaultVersionCont - the version container 
// \ru и статическую функцию void iobuf::SetDefaultVersion( VERSION ); \en and the static function void iobuf::SetDefaultVersion( VERSION); 
// \ru которую можно вызывать в любом месте программы, например : \en which can be called in any place of the program, for instance: 
//          iobuf::SetDefaultVersion( 192 );
// \ru все потоки, которые будут записываться после этого, будут записывать этот \en all the streams which will be written after this will write this 
// \ru номер в качестве версии \en number as a version 
// \ru 2. При чтении - \en 2. While reading - 
// \ru номер версии, записанный в поток возвращается функцией \en the version number written to the stream is returned by function 
//        VERSION in.MathVersion(),
// \ru где in - экземпляр потока \en where in - is the stream instance 
//
////////////////////////////////////////////////////////////////////////////////

#include <io_base.h>
#include <io_buffer.h>
#include <tool_uuid.h>
#include <system_dependency.h>
#include <io_tree.h>
#include <tool_memory_leaks_check.h>

#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
#include <tool_memory_debug.h>
#endif

//----------------------------------------------------------------------------------------
// \ru Предварительное объявление классов.
// \en The forward declaration of classes. \~
// ---
class  TapeManager;
class  TapeRegistrator;
class  iobuf_Seq;
class  IProgressIndicator;
class  ProgressBarWrapper;
class  ClassDescriptor;
struct TapeClassContainer;

//----------------------------------------------------------------------------------------
/** \brief \ru Типы регистрации объектов.
           \en Types of objects registration. \~
  \details \ru Типы регистрации потоковых объектов. \n
           \en Types of stream objects registration. \n \~
  \ingroup Base_Tools_IO
*/ //---
enum RegistrableRec { 
  noRegistrable, ///< \ru Нерегистрируемый объект. \en Unregistrable object. 
  registrable    ///< \ru Регистрируемый объект. \en Registrable object. 
};


//----------------------------------------------------------------------------------------
/** \brief \ru Типы инициализации объектов.
           \en The objects initialization types. \~
  \details \ru Типы регистрации потоковых объектов. \n
           \en Types of stream objects registration. \n \~
  \ingroup Base_Tools_IO
*/ //---
enum TapeInit {
  tapeInit ///< \ru По умолчанию. \en By default. 
};


//----------------------------------------------------------------------------------------
/** \brief \ru \ru Способы обработки атрибутов цвета.
           \en The methods of color attributes processing. \~
  \details \ru Способы обработки атрибутов цвета в потоке чтения. \n
           \en The methods of color attributes processing in a read stream. \n \~
  \ingroup Base_Tools_IO
*/ //---
enum class ColorProcessing : uint16 { 
  cr_Color,   ///< \ru Использовать только данные MbColor. \en Use MbColor data only. 
  cr_Visual,  ///< \ru Использовать только данные MbVisual. \en Use MbVisual data only. 
  cr_Both,    ///< \ru Смешивать данные MbVisual и MbColor. \en Mix MbVisual and MbColor data. 
};


//------------------------------------------------------------------------------
/** \brief \ru Базовый класс для потоковых классов.
           \en Base class for stream classes. \~
  \details \ru Базовый класс для потоковых классов. \n
           \en Base class for stream classes. \n \~
  \ingroup Base_Tools_IO
*/ // ---
#ifndef ENABLE_MEMORY_LEAKS_CHECK
class MATH_CLASS TapeBase {
#else
class MATH_CLASS TapeBase : virtual public c3d::MemoryLeaksVerifiable {
#endif
private:
  mutable use_count_type  m_countRegistrable; ///< \ru Счетчик ссылок регистрируемого объекта. \en Number of usages of the registrable object. 

public:
  /// \ru Конструктор. \en Constructor. 
  TapeBase( RegistrableRec regs = noRegistrable );
  /// \ru Конструктор копирования \en Copy-constructor. 
  TapeBase( const TapeBase & );
  /// \ru Деструктор. \en Destructor. 
  virtual ~TapeBase();

public:
  /// \ru Является ли потоковый класс регистрируемым. \en Whether the stream class is registrable. 
          RegistrableRec  GetRegistrable() const;
  /// \ru Установить состояние регистрации потокового класса. \en Set the state of registration of the stream class. 
          void            SetRegistrable( RegistrableRec regs = registrable ) const;
  /// \ru Получить дескриптор класса
  //virtual ClassDescriptor GetClassDescriptor( const VersionContainer & ) const { return ClassDescriptor( ::pureName(typeid(*this).name()) ); }
  virtual ClassDescriptor GetClassDescriptor( const VersionContainer & ) const = 0;
  /// \ru Получить имя класса. \en Get the class name.
  virtual const char *    GetPureName( const VersionContainer & ) const;
  /// \ru Принадлежит ли объект к регистрируемому семейству. \en Whether the object belongs to a registrable family.
  virtual bool            IsFamilyRegistrable() const;

private:
  /// \ru Функция-пустышка для обеспечения полиморфизма данного класса и его наследников. \en Dummy function for providing polymorphism of the given class and its descendants. 
  virtual void   dummy();   // I need this to make class polymorphic
  /// \ru Оператор присваивания \en Assignment operator 
  void operator = ( const TapeBase & other );
};


//----------------------------------------------------------------------------------------
/** \brief \ru Способы записи указателей.
           \en Methods of writing pointers. \~
  \details \ru Способы записи указателей. \n
           \en Methods of writing pointers. \n \~
  \ingroup Base_Tools_IO
*/ //---
enum TapePointerType { 
  tpt_Null              = 0x00, ///< \ru Нулевой указатель. \en Null pointer. 
  tpt_Indexed16         = 0x01, ///< \ru Индекс указателя в массиве регистрации (2 байта). \en Pointer index in the registration array (2 bytes). 
  tpt_Object            = 0x02, ///< \ru Тело объекта. \en The object solid. 
  tpt_Indexed8          = 0x03, ///< \ru Индекс указателя в массиве регистрации (1 байт). \en Index of pointer in the registration array (1 byte). 
  tpt_Indexed32         = 0x04, ///< \ru Индекс указателя в массиве регистрации (4 байта). \en Pointer index in the registration array (4 bytes). 
  tpt_Indexed64         = 0x05, ///< \ru Индекс указателя в массиве регистрации (8 байт). \en Index of pointer in the registration array (8 byte). 
  tpt_DetachedObject    = 0x06, ///< \ru Тело объекта в отдельном FileSpace. \en The object solid in separated FileSpace. 
  tpt_ObjectCatalog     = 0x07, ///< \ru Каталог объектов в отдельном FileSpace. \en The object catalog in separated FileSpace. 
};


#pragma pack( push, 1 )
//----------------------------------------------------------------------------------------
/** \brief \ru Базовый класс потока для реализации чтения и записи.
           \en The base class of the stream for implementation of reading and writing. \~
  \details \ru Базовый класс потока для реализации чтения и записи. \n
           \en The base class of the stream for implementation of reading and writing. \n \~
  \ingroup Base_Tools_IO
*/ // ---
class MATH_CLASS tape {
public:
  /// \ru Конфигурация потока чтения и записи. \en Configuration of reading and writing stream. 
  struct Config {
    ColorProcessing colorFlag; ///< \ru Способ обработки атрибутов цвета. \en The method of processing color attributes.

    Config() : colorFlag( ColorProcessing::cr_Both ) {}
  };

protected:
  iobuf_Seq   &                 buf;         ///< \ru Буфер для данных. \en Buffer for data. 
  TapeManager &                 manager;     ///< \ru Менеджер потоков. \en Stream manager. 
  uint8                         level;       ///< \ru Уровень вложенности при чтении/записи. \en Nesting level while reading/writing. 
  TapeRegistrator &             registrator; ///< \ru Структура для регистрации записанных/прочитанных адресов. \en Structure for registration of written/read addresses. 
  mutable ProgressBarWrapper *  progress;    ///< \ru Индикатор прогресса. \en Progress indicator.
  Config                        config;      ///< \ru Конфигурация потока. \en The stream configuration.

private:
  uint8                         ownBuf;      ///< \ru Владеет ли буфером. \en Whether it owns the buffer. 
  bool                          ownReg;      ///< \ru Признак владения регистратором. \en Whether it owns of the registrar. 

public:
  /// \ru Тип объекта. \en An object type. 
  enum objectType { 
    otNull,    
    otIndexed, 
    otObject
  };
  /// \ru Деструктор. \en Destructor. 
  virtual ~tape();
  
  /// \ru Получить доступ к буферу. \en Get access to the buffer. 
  const iobuf_Seq & GetIOBuffer() const;
  /// \ru Получить доступ к буферу. \en Get access to the buffer. 
        iobuf_Seq & GetIOBuffer();

  /// \ru Узнать режим работы буфера. \en Get the buffer mode. 
  uint8             mode()               const; //AR getMode
  /// \ru Установить режим работы буфера. \en Set the buffer mode. 
  void              setMode( uint8 m );
  /// \ru Убрать состояние буфера. \en Remove the buffer state. 
  void              clearState( io::state sub );
  /// \ru Добавить состояние буфера. \en Add the buffer state. 
  void              setState  ( io::state add );

  /// \ru Установить текущую версию равной версии хранилища. \en Set the current version to be equal to the storage version. 
  void              SetVersionsByStorage();
  /// \ru Вернуть главную версию (математического ядра). \en Return the main version (of the mathematical kernel). 
  VERSION           MathVersion()        const;
  /// \ru Вернуть дополнительную версию (конечного приложения). \en Return the additional version (of the target application). 
  VERSION           AppVersion( size_t ind = -1 ) const;

  /// \ru Получить доступ к контейнеру версий. \en Get access to the version container. 
  const VersionContainer &  GetVersionsContainer() const;
  /// \ru Установить версию открытого файла. \en Set the version of open file. 
        void                SetVersionsContainer( const VersionContainer & vers ) const;
  /// \ru Установить версию хранилища. \en Set the storage version. 
        VERSION             SetStorageVersion( VERSION v );

  /// \ru Свежий ли буфер? \en Is the buffer fresh? 
  int               fresh() const;
  /// \ru Корректно ли состояние буфера.. \en Whether the buffer state is correct. 
  bool              good()  const;
  /// \ru Достигнут ли конец файла? \en Is the end of file reached? 
  virtual uint8     eof()   const;
  /// \ru Получить флаг состояния буфера. \en Get the flag of the buffer state. 
  virtual uint32    state() const;
  /// \ru Получить текущую позицию в потоке. \en Get current position in stream
  virtual io::pos   tell();
  ///< \ru Зарегистрировать указатель. \en Register the pointer. 
  void              registrate( const TapeBase * e );
  ///< \ru Отменить регистрацию указателя. \en Unregister the pointer. 
  void              unregistrate( const TapeBase * e );
  ///< \ru Есть ли зарегистрированный объект? \en Does a registered object exist? 
  bool              exist ( const TapeBase * e ) const;
  ///< \ru Очистить массив регистрации. \en Flush the registration array. 
  void              flushRegister  ();
  ///< \ru Получить количество зарегистрированных объектов. \en Get the number of registered objects. 
  size_t            RegisteredCount() const;
  ///< \ru Получить максимально возможное количество объектов для регистрации. \en Get the maximal possible number of objects for registration. 
  size_t            GetMaxRegisteredCount() const;
  ///< \ru Зарезервировать память под n объектов. \en Reserve memory for n objects. 
  void              ReserveRegistered( size_t n );

  /// \ru Владеем ли буфером? \en Do we own the buffer? 
  bool              IsOwnBuffer() const;
  /// \ru Установить флаг владения буфером. \en Set the flag of buffer ownership. 
  void              SetOwnBuffer( bool own );

  /// \ru Получить тип индекса. \en Get index type.
  uint8             GetIndexType( size_t index ) const;

  /// \ru Работа с индикатором прогресса. \en Work with progress indicator.

  /// Инициализировать индикатор прогресса. \en Initialize progress indicator.
  void                  InitProgress( IProgressIndicator * pr );
  void                  InitProgress( ProgressBarWrapper & pr );
  /// \ru Освободить текущий индикатор прогресса. Установить родительский индикатор прогресса, если он есть.
  /// \en Release current progress indicator. Set parent progress indicator if it exists.
  void                  ResetProgress(); 
  /// \ru Получить индикатор прогресса. \en Get progress indicator.
  ProgressBarWrapper *  GetProgress();
  /// \ru Завершить индикатор прогресса. \en End the progress indicator.
  void                  FinishProgress();

  /// \ru Получить конфигурацию. \en Get configuration.
  const Config & GetConfig() const { return config; }
        Config & SetConfig()       { return config; }

protected:
  /// \ru Конструктор. \en Constructor.
  tape( membuf &, bool openSys, uint8 om, TapeRegistrator * , bool ownReg = false);
  
  /// \ru Конструктор. \en Constructor. 
  tape( iobuf_Seq & buf, bool ownBuf, bool openSys, uint16 om, TapeRegistrator * , bool ownReg = false); //AR(DP)

private:
  /// \ru Открыть системный файл в соответствующем режиме (чтение или запись). \en Open the system file in the appropriate mode (reading or writing). 
  void init( uint8 om );

OBVIOUS_PRIVATE_COPY( tape )
};
#pragma pack( pop )


//----------------------------------------------------------------------------------------
/** \brief \ru Поток для чтения.
           \en Stream for reading. \~
  \details \ru Поток для чтения. \n
           \en Stream for reading. \n \~
  \ingroup Base_Tools_IO
*/ // ---
class MATH_CLASS reader : public virtual tape {
  std::set<TapeBase *>  m_registeredDelete;
public:
  typedef std::unique_ptr<reader>     reader_ptr;
protected:
  /// \ru Конструктор. \en Constructor. 
  reader( iobuf_Seq & buf, bool ownbuf, bool openSys, uint16 om, TapeRegistrator * reg );

  /// \ru Конструктор. \en Constructor. \~ \deprecated \ru Метод устарел. \en The method is deprecated.
  reader( membuf & sb, uint8 om );
  /// \ru Конструктор. \en Constructor. \~ \deprecated \ru Метод устарел. \en The method is deprecated.
  reader( iobuf_Seq & buf, uint16 om );
  
public:
  virtual ~reader();

public:
  /// \ru Создать читатель для последовательного буфера. \en Create reader for iobuf_Seq.
  static reader_ptr   CreateReader ( std::unique_ptr<iobuf_Seq> buf, uint16 om );
  /// \ru Создать читатель для буфера в памяти. \en Create reader for membuf.
  static reader_ptr   CreateMemReader ( membuf & sb, uint8 om );

public:
  /// \ru Прочитать объект. \en Read the object. 
          TapeBase *  readObject ( TapeBase * mem = 0 );
  /// \ru Прочитать указатель на объект. \en Read a pointer to the object. 
          TapeBase *  readObjectPointer();

  /// \ru Читать каталог объектов. \en Read the object catalog. 
  virtual void        ReadObjectCatalog();
  /// \ru Читать объект по позиции в кластере. \en Read an object by position in cluster. 
  virtual TapeBase *  ReadObjectByPosition ( const ClusterReference & ) { return nullptr; }
  /// \ru Установить позицию чтения. \en Set reading position. 
  virtual bool        SetReadPosition ( ClusterReference & ) { return false; } // not supported

  /// \ru Прочитать беззнаковое 64-разрядное целое \en Read unsigned 64-bit integer. 
          bool        readUInt64( uint64 & );
  /// \ru Прочитать 64-разрядное целое \en Read 64-bit integer.  
          bool        readInt64( int64 & );
  
  /// \ru Прочитать байт из буфера. \en Read a byte from the buffer. 
  virtual int         readByte();
  /// \ru Прочитать последовательность байт из буфера. \en Read a sequence of bytes from the buffer. 
  virtual bool        readBytes( void * bf, size_t len );

  /// \ru Получить указатель на дерево модели. \en Get pointer to the model tree. 
  virtual const c3d::IModelTree * GetModelTree() const { return nullptr; } // not supported

  /// \ru Получить признак полного чтения текущего объекта. \en Get indicator of full reading of the current object. 
  /// \ru Установить признак полного чтения текущего объекта. \en Set indicator of full reading of the current object. 
  virtual bool        IsFullRead() { return true; } // not supported
  virtual void        SetFullRead( bool ) {} // not supported

  /// \ru Временно сохранить зарегистрированный объект, который будет удален после завершения чтения. \en Temporarily preserve registered object to be deleted after reading complete. 
  virtual void        PreserveRegisteredObj( TapeBase * );

  /// \ru Получить ошибки чтения. \en Get reading errors. 
  virtual uint32      GetLastError();

  /// \ru Получить версию формата потока чтения. \en Get format version of the read stream. 
  virtual VERSION     GetFormatVersion() { return buf.GetFormatVersion(); }
  /// \ru Установить версию формата. Возвращает false, если версия не применима для данной реализации потока чтения.
  /// \en Set the format version. Returns false, if specified version is not applicable to the current implementation of the read stream.
  virtual bool        SetFormatVersion( VERSION version );

  // \ru Работа с индикатором прогресса. \en Work with progress indicator.
          void        InitProgress( IProgressIndicator * pr );
          void        InitProgress( ProgressBarWrapper & pr );

  // \ru Работа с конфигурацией. \en Work with configuration.
  void                SetColorFlag( ColorProcessing flag ) { config.colorFlag = flag; }
  ColorProcessing     GetColorFlag() const                 { return config.colorFlag; }

protected:
  /// \ru Читаем объект по заданной позиции. \en Read object on defined position. 
  virtual TapeBase *  ReadDetachedObject();
  /// \ru Регистрируем объект. \en Register the object. 
  virtual void        RegisterObject( TapeBase * obj, uint8 regId, ClusterReference ref = ClusterReference() );
  /// \ru Читаем индекс объекта. \en Read object index. 
          size_t      ReadObjectIndex();

OBVIOUS_PRIVATE_COPY( reader )
};


//----------------------------------------------------------------------------------------
/** \brief \ru Поток для чтения с возможностью чтения из нескольких FileSpaces по заданным позициям.
           \en Stream for reading from several FileSpaces by given positions in clusters. \~
  \details \ru Поток для чтения с возможностью чтения из разных FileSpaces по заданным позициям. \n
            \en Stream for reading from several FileSpace by given positions in clusters. \n \~
  \ingroup Base_Tools_IO
*/ // ---
class MATH_CLASS reader_ex : public reader
{
  std::unique_ptr<c3d::IModelTree>  m_tree;
  uint32                            m_lastError;
  bool                              m_fullRead;
protected:
  /// \ru Конструктор. \en Constructor. 
  reader_ex( iobuf_Seq & buf, bool ownbuf, bool openSys, uint16 om );

  /// \ru Конструктор. \en Constructor. \~ \deprecated \ru Метод устарел. \en The method is deprecated.
  reader_ex( membuf & sb, uint8 om );

  /// \ru Конструктор. \en Constructor. \~ \deprecated \ru Метод устарел. \en The method is deprecated.
  reader_ex( iobuf_Seq & buf, uint16 om );

public:
  virtual ~reader_ex() {}

public:
  /// \ru Создать экземпляр reader_ex для последовательного буфера. \en Create reader_ex instance for sequential buffer. 
  static std::unique_ptr<reader_ex> CreateReaderEx( std::unique_ptr<iobuf_Seq> buf, uint16 om );

  /// \ru Создать читатель для буфера в памяти. \en Create reader for membuf.
  static std::unique_ptr<reader_ex> CreateMemReaderEx ( membuf & sb, uint8 om );


public:
  /// \ru Читаем каталог объектов. \en Read the object catalog. 
          void        ReadObjectCatalog() override;
  /// \ru Читать объект по позиции в кластере. \en Read an object by position in cluster. 
          TapeBase *  ReadObjectByPosition ( const ClusterReference& position ) override;
  /// \ru Установить позицию чтения. \en Set reading position. 
          bool        SetReadPosition ( ClusterReference & ) override;

  /// \ru Получить указатель на дерево модели. \en Get pointer to the model tree. 
          const c3d::IModelTree * GetModelTree() const override;

  /// \ru Признак полного чтения текущего объекта.
  ///     При чтении произвольного объекта может возникнуть необходимость чтения некоторых данных его родителя.
  ///     В этом случае объект родителя читается не полностью и имеет флаг FullRead = false.
  /// \en Indicator of full reading of the current object.
  ///     While reading an arbitrary object there can be a need to read some data from its parent.
  ///     In this case the parent object is read partially and has the flag FullRead = false.

  /// \ru Получить признак полного чтения текущего объекта. \en Get indicator of full reading of the current object. 
          bool        IsFullRead() override;
  /// \ru Установить признак полного чтения текущего объекта. \en Set indicator of full reading of the current object. 
          void        SetFullRead( bool full ) override;

  /// \ru Получить ошибки чтения. \en Get reading errors. 
          uint32      GetLastError() override;

  /// \ru Установить версию формата. Возвращает false, если версия не применима для данной реализации потока чтения.
  /// \en Set the format version. Returns false, if specified version is not applicable to the current implementation of the read stream.
          bool        SetFormatVersion( VERSION version ) override;

protected:
  /// \ru Читать объект по заданной позиции. \en Read object on defined position. 
          TapeBase *  ReadDetachedObject() override;
  /// \ru Зарегистрировать объект. \en Register the object. 
          void        RegisterObject( TapeBase * obj, uint8 regId, ClusterReference ref = ClusterReference() ) override;

OBVIOUS_PRIVATE_COPY( reader_ex )
};


//----------------------------------------------------------------------------------------
/** \brief \ru Поток для записи.
           \en Stream for writing. \~
  \details \ru Поток для записи. \n
           \en Stream for writing. \n \~
  \ingroup Base_Tools_IO
*/ // ---
class MATH_CLASS writer : public virtual tape {
public:
  typedef std::unique_ptr<writer>     writer_ptr;
protected:
  /// \ru Конструктор. \en Constructor. 
  writer ( iobuf_Seq  & buf, bool ownBuf, bool openSys, uint16 om, TapeRegistrator * reg );

  /// \ru Конструктор. \en Constructor. \~ \deprecated \ru Метод устарел. \en The method is deprecated.
  writer ( membuf & sb, uint8  om );
  /// \ru Конструктор. \en Constructor. \~ \deprecated \ru Метод устарел. \en The method is deprecated.
  writer ( iobuf_Seq & buf, uint16 om );

public:
  virtual ~writer() {}

public:
  /// \ru Создать писатель для последовательного буфера. \en Create writer for iobuf_Seq.
  static writer_ptr   CreateWriter( std::unique_ptr<iobuf_Seq> buf, uint16 om );
  /// \ru Создать писатель для буфера в памяти. \en Create writer for membuf.
  static writer_ptr   CreateMemWriter( membuf & sb, uint8  om );

public:
  /// \ru Записать объект. \en Write the object. 
          void        writeObject( const TapeBase * );
  /// \ru Записать указатель на объект. \en Write the pointer to the object. 
          void        writeObjectPointer( const TapeBase * );

  /// \ru Записать дерево модели. \en Write the model tree. 
  virtual void        WriteModelCatalog();
  /// \ru Выдать следующую позицию записи. \en Get next writing position. 
  virtual ClusterReference GetNextWritePosition () { return ClusterReference(); } // not supported

  /// \ru Записать байт в буфер. \en Write the byte to the buffer. 
  virtual void        writeByte  ( uint8 ch );
  /// \ru Записать последовательность байт в буфер. \en Write the sequence of bytes to the buffer. 
  virtual void        writeBytes ( const void * bf, size_t len );
  /// \ru Записать беззнаковое 64-разрядное целое. \en Write unsigned 64-bit integer. \~ \return \ru Возвращает количество записанных байт. \en Returns the number of written bytes. \~
          void        writeUInt64( const uint64 & val );
  /// \ru Записать 64-разрядное целое. \en Write 64-bit integer. \~ \return \ru Возвращает количество записанных байт. \en Returns the number of written bytes. \~
          void        writeInt64 ( const int64 & val );

          // \ru Запись CHAR строки в поток (кодировка ANSI, русская локаль). \en Writing CHAR string to the stream. (ANSI coding, Russian locale).
          writer &    __writeChar ( const char * s );
          // \ru Запись WCHAR строки в поток (в потоке хранится как UTF-16). \en Writing WCHAR string to the stream (stored in the stream as UTF-16).
          writer &    __writeWchar( const TCHAR * s );
          // \ru Запись WCHAR строки в поток (в потоке хранится как UTF-16). \en Writing WCHAR string to the stream (stored in the stream as UTF-16). 
          writer &    __writeWcharT( const wchar_t * s );
          // \ru Длина записи WCHAR строки в поток (в потоке хранится как UTF-16). \en Length of WCHAR string in the stream (stored in the stream as UTF-16).
          size_t      __lenWchar( const TCHAR * s );

  /// \ru Получить указатель на дерево модели. \en Get pointer to the model tree. 
  virtual const c3d::IModelTree * GetModelTree() const { return nullptr; } // not supported

  /// \ru Получить версию формата потока записи. \en Get format version of the write stream. 
  virtual VERSION     GetFormatVersion() { return buf.GetFormatVersion(); }
  /// \ru Установить версию формата. Возвращает false, если версия не применима для данной реализации потока записи.
  /// \en Set the format version. Returns false, if specified version is not applicable to the current implementation of the write stream.
  virtual bool        SetFormatVersion( VERSION version );

  // \ru Работа с индикатором прогресса. \en Work with progress indicator.
          void        InitProgress( IProgressIndicator * pr );
          void        InitProgress( ProgressBarWrapper & pr );

  // \ru Тестовые функции. \en Testing functions.
  virtual void        SetTestData( const TapeBase * ) {}

protected:
  /// \ru Записать объект и тип. \en Write the object and type. 
  virtual void        WriteObjectAndType  ( const TapeBase * );
  /// \ru Зарегистрировать объект. \en Register the object. 
  virtual void        RegisterObject      ( const TapeBase * );
  /// \ru Завершить запись объекта. \en Finish writing the object. 
  virtual void        EndWriteObject      ( const TapeBase * );
  /// \ru Добавить ссылку на объект в каталог. \en Add reference to the object to the object catalog. 
  virtual void        UpdateObjectCatalog ( const TapeBase * , const ClusterReference & );
  /// \ru Является ли объект регистрируемым. \en Whether the object is registrable.
  virtual bool        IsRegistrable( const TapeBase * mem );
  /// Записать индекс объекта
          void        WriteObjectIndex    ( size_t index );

OBVIOUS_PRIVATE_COPY( writer )
};

//----------------------------------------------------------------------------------------
/** \brief \ru Поток для записи в разные FileSpaces.
           \en Stream for writing to several FileSpaces. \~
  \details \ru Поток для записи в разные FileSpaces. \n
           \en Stream for writing to several FileSpaces. \n \~
  \ingroup Base_Tools_IO
*/ // ---
class MATH_CLASS writer_ex : public writer
{
  std::unique_ptr<c3d::IModelTree>  m_tree;
  ClusterReference                  m_catalogRef;
protected:
  /// \ru Конструктор. \en Constructor. 
  writer_ex ( iobuf_Seq  & buf, bool ownBuf, bool openSys, uint16 om );

  /// \ru Конструктор. \en Constructor. \~ \deprecated \ru Метод устарел. \en The method is deprecated.
  writer_ex ( membuf & sb, uint8  om );
  /// \ru Конструктор. \en Constructor. \~ \deprecated \ru Метод устарел. \en The method is deprecated.
  writer_ex ( iobuf_Seq & buf, uint16 om );

public:
  virtual ~writer_ex() {}

public:
  /// \ru Создать писатель для последовательного буфера. \en Create writer for iobuf_Seq.
  static std::unique_ptr<writer_ex> CreateWriterEx( std::unique_ptr<iobuf_Seq> buf, uint16 om );
  /// \ru Создать писатель для буфера в памяти. \en Create writer for membuf.
  static std::unique_ptr<writer_ex> CreateMemWriterEx( membuf & sb, uint8  om );

public:
  /// \ru Записать дерево модели. \en Write the model tree. 
          void              WriteModelCatalog() override;
  /// \ru Выдать следующую позицию записи. \en Get next writing position. 
          ClusterReference  GetNextWritePosition () override;
  /// \ru Получить указатель на дерево модели. \en Get pointer to the model tree. 
          const c3d::IModelTree * GetModelTree () const override;

  /// \ru Установить версию формата. Возвращает false, если версия не применима для данной реализации потока записи.
/// \en Set the format version. Returns false, if specified version is not applicable to the current implementation of the write stream.
          bool        SetFormatVersion( VERSION version ) override;

protected:
  /// \ru Записать объект и тип. \en Write the object and type. 
          void        WriteObjectAndType  ( const TapeBase * ) override;
  /// \ru Зарегистрировать объект. \en Register the object. 
          void        RegisterObject      ( const TapeBase * ) override;
  /// \ru Завершить запись объекта. \en Finish writing the object. 
          void        EndWriteObject      ( const TapeBase * ) override;
  /// \ru Добавить ссылку на объект в каталог. \en Add reference to the object to the object catalog. 
          void        UpdateObjectCatalog ( const TapeBase *mem, const ClusterReference& ref ) override;
  /// \ru Является ли объект регистрируемым. \en Whether the object is registrable.
          bool        IsRegistrable( const TapeBase * mem ) override;

OBVIOUS_PRIVATE_COPY( writer_ex )
};


//----------------------------------------------------------------------------------------
/**
  \brief \ru Дружественные операторы чтения и записи указателей и ссылок. 
         \en Friend operators of reading and writing of pointers and references. \~ 
  \ingroup Base_Tools_IO
*/
// ---
#define DECLARE_PERSISTENT_OPS( Class )                                                     \
  friend inline reader & CALL_DECLARATION operator >> ( reader & in, Class & ref ) {        \
    in.readObject( dynamic_cast<TapeBase *>(&ref) );                                        \
    return in;                                                                              \
  }                                                                                         \
  friend inline reader & CALL_DECLARATION operator >> ( reader & in, Class *& ptr ) {       \
    ptr = dynamic_cast<Class *>( in.readObjectPointer() );                                  \
    return in;                                                                              \
  }                                                                                         \
  friend inline reader & CALL_DECLARATION operator >> ( reader & in, const Class *& ptr )   \
  {                                                                                         \
    ptr = dynamic_cast<Class *>( in.readObjectPointer() );                                  \
    return in;                                                                              \
  }                                                                                         \
  friend inline writer & CALL_DECLARATION operator << ( writer & out, const Class & ref ) { \
    out.writeObject( dynamic_cast<const TapeBase *>(&ref) );                                \
    return out;                                                                             \
  }                                                                                         \
  friend inline writer & CALL_DECLARATION operator << ( writer & out, const Class * ptr ) { \
    out.writeObjectPointer( dynamic_cast<const TapeBase*>(ptr) );                           \
    return out;                                                                             \
  }                                                                                         \
  friend inline writer & CALL_DECLARATION operator << ( writer & out, Class & ref ) {       \
    out.writeObject( dynamic_cast<TapeBase *>(&ref) );                                      \
    return out;                                                                             \
  }                                                                                         \
  friend inline writer & CALL_DECLARATION operator << ( writer& out, Class * ptr ) {        \
    out.writeObjectPointer( dynamic_cast<TapeBase *>(ptr) );                                \
    return out;                                                                             \
  }


//------------------------------------------------------------------------------
/** \brief \ru Объявление класса Class поточным.
           \en Declaration of class Class as a stream one. \~
  \details \ru Объявление класс Class поточным.
    Устанавливается в декларации класса в файле *.h.
    Декларирует операторы <<, >>, а также функции Read и Write,
    которые должны быть определены в любом файле *.cpp
    Class должен наследовать от TapeBase.
    Для этого класса должен быть определен конструктор чтения,
    а его тело должно быть в .cpp файле. \n
           \en Declaration of class Class as a stream one.
    It is set in the declaration of class in file *.h.
    Declares operators <<, >> and also functions Read and Write
    which must be defined in any file *.cpp
    Class must be inherited from TapeBase.
    The read constructor must be defined for the class
    and its solid should be in .cpp file. \n \~
  \ingroup Base_Tools_IO
*/
// ---
#define DECLARE_PERSISTENT_CLASS( Class )    \
  DECLARE_PERSISTENT_FUNCS( Class );         \
  DECLARE_PERSISTENT_OPS( Class );           \
  DECLARE_PERSISTENT_CTOR( Class );          \
  DECLARE_NEW_DELETE_CLASS( Class );         \
  DECLARE_CLASS_DESC_FUNC(Class)

/** \brief \ru Аналог макроса DECLARE_PERSISTENT_CLASS
           с возможностью перегрузки операторов new/delete,
           обеспечивающий последовательное обращение к функциям
           выделения/освобождения памяти из разных потоков.
           \en Analog of DECLARE_PERSISTENT_CLASS macro
           with support of new/delete operators overloading which provides
           sequential access to the allocation/deallocation functions
           from different threads. \~
\details \ru Аналог макроса DECLARE_PERSISTENT_CLASS
           с возможностью перегрузки операторов new/delete,
           обеспечивающий последовательное обращение к функциям
           выделения/освобождения памяти из разных потоков
           (включается переменной __OVERLOAD_MEMORY_ALLOCATE_FREE_).
           \en Analog of DECLARE_PERSISTENT_CLASS macro
           with support of new/delete operators overloading which provides
           sequential access to the allocation/deallocation functions
           from different threads
           (enabled by defining __OVERLOAD_MEMORY_ALLOCATE_FREE_). \~
\ingroup Base_Tools_IO
*/
// ---
#define DECLARE_PERSISTENT_CLASS_NEW_DEL( Class ) \
  DECLARE_PERSISTENT_CLASS( Class )               \
  DECLARE_NEW_DELETE_CLASS_EX( Class )


#endif // __IO_TAPE_DEFINE_H

