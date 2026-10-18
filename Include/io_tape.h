////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Сериализация:. Утилиты чтения и записи потоковых классов.
         \en Serialization: utilities for reading and writing of stream classes. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __IO_TAPE_H
#define __IO_TAPE_H


/// \ru Для контроля чтения/записи char* и TCHAR* только через ReadTCHAR/WriteTCHAR() скомпилировать с данным дефайном и увидим все места, где незаконно пишутся/читаются данные через operator << и >>.
/// \en Compile with the given preprocessor define for control of reading/writing char* and TCHAR* only via ReadTCHAR/WriteTCHAR(), not via illegally operator << and >>. 
//#define DISABLE_RWTCHAR  

#include <io_tape_define.h>
#include <io_memory_buffer.h>
#include <templ_pointer.h>
#include <templ_sfdp_array.h>
#include <alg_indicator.h>
#include <tool_cstring.h>
#include <hash32.h>
#include <tool_mutex.h>
#include <map>
#include <typeinfo>


//----------------------------------------------------------------------------------------
/** \brief \ru Массив для регистрации объектов при чтении/записи.
           \en Array for object registration while reading/writing. \~
  \details \ru Массив для регистрации объектов при чтении/записи. \n
           \en Array for object registration while reading/writing. \n \~
  \ingroup Base_Tools_IO
*/ //---
class MATH_CLASS TapeRegistrator 
{
public:
  typedef std::map<const TapeBase *, size_t> TapeIndexMap;
  typedef std::map<size_t, TapeBase *>       IndexTapeMap;
protected:
  TapeIndexMap tapeIndexPairs; /// \ru Ассоциативный массив связок [указатель на объект]-[номер в массиве]. \en Map of [object]-[index] pairs.
  size_t       maxCount;       /// \ru Максимальное количество зарегистрированных объектов. \en Maximal number of registered objects.
  IndexTapeMap indexesAndObjs; /// \ru Ассоциативный массив связок [номер в массиве]-[указатель на объект]. \en Map of [index]-[object] pairs.

public:
  /// \ru Конструктор. \en Constructor. 
  TapeRegistrator();
  /// \ru Деструктор. \en Destructor. 
  virtual ~TapeRegistrator();

  /// \ru Получить количество зарегистрированных объектов. \en Get a number of registered objects. 
  size_t      Count() const;
  /// \ru Зарезервировать место под данное количество элементов. \en Reserve space for a given number of elements.
  bool        Reserve ( size_t n );
  
  /// \ru Найти объект в массиве. \en Find the object in array. 
  size_t      FindIt( const TapeBase * e ) const;
  /// \ru Существует ли объект в массиве. \en Whether the object is in the array. 
  bool        IsExist( const TapeBase * e ) const;
  /// \ru Узнать максимально возможное количество регистрируемых объектов. \en Get the maximal possible number of registered objects. 
  size_t      GetMaxCount() const;
  /// \ru Выдать из массива зарегистрированных объектов указатель по заданному индексу. \en Get the pointer from the registered object array by the given index. 
  TapeBase *  operator[]( size_t ind ) const ;

  ///< \ru Вставить элемент с определенным индексом. \en Insert an element with defined index. 
  void        AddAt( const TapeBase * e, size_t ind );
  /// \ru Добавить объект в массив. \en Add the object to the array. 
  size_t      Add( const TapeBase * e );

  /// \ru Выдать указатель на зарегистрированный объект по заданной позиции в кластере. \en Get the pointer of the registered object by the position in the cluster. 
  virtual TapeBase *        Get( const ClusterReference & ) const { return nullptr; } // unsupported
  /// \ru Выдать позицию в кластере по заданному индексу. \en Get position in the cluster by given index. 
  virtual ClusterReference  GetClusterRef( size_t )   const { return ClusterReference(); } // unsupported
  /// \ru Добавить позицию объекта в кластере. \en Add the object position in the cluster. 
  virtual void              AddClusterRef( size_t, const ClusterReference & ) {} // unsupported
  /// \ru Очистить массив зарегистрированных объектов. \en Flush the array of registered objects. 
  virtual void              FlushRegistered();
  /// \ru Очистить зарегистрированный объект \en Flush the registered object 
  virtual void              FlushObj( const TapeBase * );

  /// \ru Поменять местами массивы для регистрации
  void Swap ( TapeRegistrator & swapReg );

protected:
  /// \ru Очистить зарегистрированный объект \en Flush the registered object 
  void         FlushObjInd( size_t );

#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
public:
  void * operator new    ( size_t size ) {
    return ::Allocate( size, typeid(TapeRegistrator).name() );
  }
  void   operator delete ( void * ptr, size_t size ) {
    ::Free( ptr, size, typeid(TapeRegistrator).name() );
  }
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_

OBVIOUS_PRIVATE_COPY( TapeRegistrator )
};


//----------------------------------------------------------------------------------------
/** \brief \ru Массив для регистрации объектов с сохранением информации о позиции чтения/записи.
           \en Array for registration of objects with information about reading/writing position. \~
  \details \ru Массив для регистрации объектов с сохранением информации о позиции чтения/записи. \n
           \en Array for registration of objects with information about reading/writing position. \n \~
  \ingroup Base_Tools_IO
*/ //---
class MATH_CLASS TapeRegistratorEx : public TapeRegistrator {
public:
  typedef std::map<ClusterReference, size_t>        ClusterIndexMap;
  typedef std::map<size_t, const ClusterReference*> IndexClusterMap;
private:
  /// \ru Ассоциативный массив [cluster position] -> [object index]. Основное хранилище для значений ClusterReference.
  /// \en Map: [cluster position] -> [object index]. The primary container for ClusterReference values.
  ClusterIndexMap  refIndex;
  /// \ru Ассоциативный массив [object index] -> [pointer to cluster position]. Содержит указатели на объекты ClusterReference из массива refIndex.
  /// \en Map: [object index] -> [pointer to cluster position]. Contains pointers to object ClusterReference from refIndex map.
  IndexClusterMap  objReferences;

public:
  /// \ru Конструктор. \en Constructor. 
  TapeRegistratorEx();

  /// \ru Выдать указатель на зарегистрированный объект по заданной позиции в кластере.
  /// \en Get the pointer of the registered object by the position in the cluster. 
  virtual TapeBase *        Get ( const ClusterReference & ref ) const;
  /// \ru Выдать позицию в кластере по заданному индексу. \en Get position in the cluster by given index. 
  virtual ClusterReference  GetClusterRef ( size_t ind ) const;
  /// \ru Добавить позицию объекта в кластере. \en Add the object position in the cluster. 
  virtual void              AddClusterRef( size_t ind, const ClusterReference & ref );
  /// \ru Очистить массив зарегистрированных объектов. \en Flush the array of registered objects. 
  virtual void              FlushRegistered();
  /// \ru Очистить зарегистрированный объект \en Flush the registered object 
  virtual void              FlushObj ( const TapeBase * );

OBVIOUS_PRIVATE_COPY( TapeRegistratorEx )
};


//----------------------------------------------------------------------------------------
/** \brief \ru Индикатор прогресса в области видимости для reader.
           \en Scoped progress indicator for reader. \~
  \details \ru Индикатор прогресса в области видимости для чтения модели с помощью reader.
               Создается дочерний индикатор прогресса для reader. При выходе из области видимости 
               освобождается текущий индикатор прогресса и устанавливается родительский индикатор прогресса. \n
           \en Scoped progress indicator for reader.
               A scoped child progress indicator for reader is created.
               When exiting the scope, the current progress indicator is released and 
               the parent progress indicator is set. \n \~
  \ingroup Base_Tools_IO
*/ // ---
class MATH_CLASS ScopedReadProgress
{
  SPtr<ProgressBarWrapper>  _progress;
  tape & _tape;
public:
  ScopedReadProgress( tape & in );
  ~ScopedReadProgress();
  // \ru Доступ к индикатору прогресса. \en Access to progress indicator.
  ProgressBarWrapper * operator()();

private:
  ScopedReadProgress();
  void operator = ( const ScopedReadProgress & );
};


//----------------------------------------------------------------------------------------
/// \ru Шаблон функции создания нового экземпляра. \en Template of function of a new instance creation. \~ \ingroup Base_Tools_IO
//---
typedef TapeBase * ( CALL_DECLARATION * BUILD_FUNC ) ( void );

//----------------------------------------------------------------------------------------
/** \brief \ru Шаблон функции преобразования.
           \en Template of conversion function. \~
  \details \ru Шаблон функции преобразования из указателя на TapeBase к указателю на класс. \n
           \en Template of function of conversion from a pointer to TapeBase to a pointer to the class. \n \~
  \ingroup Base_Tools_IO
*/ //---
typedef void * ( CALL_DECLARATION * CAST_FUNC ) ( const TapeBase * );

//----------------------------------------------------------------------------------------
/**\ru Шаблон функции чтения экземпляра.
   \en Template of instance reading function. \~
   \ingroup Base_Tools_IO
*/ //---
typedef void ( CALL_DECLARATION * READ_FUNC ) ( reader & in, void * /*obj*/ );

//----------------------------------------------------------------------------------------
/// \ru Шаблон функции записи экземпляра. \en Template of instance writing function. \~ \ingroup Base_Tools_IO
//---
typedef void ( CALL_DECLARATION * WRITE_FUNC ) ( writer & out, void * /*obj*/ );


//----------------------------------------------------------------------------------------
/** \brief \ru Упакованное имя класса.
           \en Packed class name. \~
  \details \ru Упакованное имя одного класса - для набора массива потоковых классов в TapeClass. \n
           \en Packed name of one class - for array of stream classes in TapeClass. \n \~
  \ingroup Base_Tools_IO
*/ // ---
class MATH_CLASS ClassDescriptor
{
protected:
  uint16 val;     ///< \ru Хэш имени класса. \en The class name hash.
  MbUuid appID_;  ///< \ru Дополнительный идентификатор приложения. \en Additional application identifier.
private:
  /// \ru Признак записи appID. \en AppID record flag.
  static const uint16 rwIdFlag;

public:
  /// \ru Конструктор. \en Constructor. 
  ClassDescriptor();
  /// \ru Конструктор по хэшу. \en Constructor by hash. 
  ClassDescriptor( uint16 v );
  /// \ru Конструктор по имени. \en Constructor by name. 
  ClassDescriptor( const char * name );
  /// \ru Конструктор по хэшу. \en Constructor by hash. 
  ClassDescriptor( uint16 v, const MbUuid & appID );
  /// \ru Конструктор по имени. \en Constructor by name. 
  ClassDescriptor( const char * name, const MbUuid & appID );
  /// \ru Конструктор по хэшу. \en Constructor by hash. 
  ClassDescriptor( const ClassDescriptor & other );

  /// \ru Оператор присваивания. \en An assignment operator. 
  ClassDescriptor & operator = ( const ClassDescriptor & other );

  /// \ru Оператор равенства. \en The equality operator. 
  bool  operator == ( const ClassDescriptor & other ) const;
  /// \ru Оператор неравенства. \en The inequality operator. 
  bool  operator != ( const ClassDescriptor & other ) const;
  /// \ru Оператор сравнения. \en Comparison operator. 
  bool operator < ( const ClassDescriptor & other ) const;
  /// \ru Оператор сравнения. \en Comparison operator. 
  bool operator > ( const ClassDescriptor & other ) const;
#ifdef C3D_DEBUG
  /// \ru Оператор доступа. \en An access operator. 
  operator uint16() const { return val; }
#endif
  /// \ru Оператор записи. \en Write operator.
  void Write( writer & out );
  /// \ru Оператор чтения. \en Read operator.
  bool Read( reader & in );
};


//----------------------------------------------------------------------------------------
/** \brief \ru "Обертка" для одного потокового класса.
           \en "Wrapper" for one stream class. \~
  \details \ru "Обертка" для одного потокового класса ( не экземпляра! ).
    Хранит упакованное имя класса и адреса функций, необходимых при чтении/записи. \n
           \en "Wrapper" for one stream class ( not instance! ).
    Stores packed class name and addresses of functions necessary while reading/writing. \n \~
  \ingroup Base_Tools_IO
*/ // ---
class MATH_CLASS TapeClass {
protected:
  ClassDescriptor hashValue; ///< \ru Упакованное имя класса. \en Packed class name. 
  BUILD_FUNC     _builder;   ///< \ru Функция создания нового экземпляра. \en Functions of a new instance creation. 
  CAST_FUNC      _caster;    ///< \ru Функция преобразования от TapeBase к указателю на класс. \en Function of conversion from TapeBase to a pointer to a class. 
  READ_FUNC      _reader;    ///< \ru Функция чтения. \en Read function. 
  WRITE_FUNC     _writer;    ///< \ru Функция записи. \en Write function. 

public:
  /// \ru Конструктор. \en Constructor.
  /// \ru Конструктор. \en Constructor. 
  TapeClass( const char * name, BUILD_FUNC b, CAST_FUNC c, READ_FUNC r, WRITE_FUNC w );
  TapeClass( const char * name, MbUuid appID, BUILD_FUNC b, CAST_FUNC c, READ_FUNC r, WRITE_FUNC w );
  /// \ru Деструктор. \en Destructor. 
  virtual ~TapeClass();
  /// \ru Получить упакованное имя класса. \en Get the packed class name. 
  ClassDescriptor GetPackedClassName() const;
  /// \ru Получить упакованное имя класса для записи с учетом версии. \en Get the packed class name for writing subject to the version. 
  virtual ClassDescriptor GetPackedClassNameForWrite( VERSION ) const;

  friend class  TapeManager;
  friend struct TapeClassContainer;

  OBVIOUS_PRIVATE_COPY( TapeClass )
};


//----------------------------------------------------------------------------------------
/** \brief \ru Менеджер потоков.
           \en Stream manager. \~
  \details \ru Менеджер потоков чтения и записи. \n
           \en Reading and writing streams manager. \n \~
  \ingroup Base_Tools_IO
*/ // ---
class MATH_CLASS TapeManager {
private:
  //static TPointer<TapeManager> &  StaticTapeManager();
protected :
  TapeClass * currentClass; ///< \ru Указатель на объект потокового класса. \en A pointer to the object of stream class. 
protected :
  /// \ru Конструктор. \en Constructor. 
  TapeManager();
  /// \ru Конструктор копирования. \en Copy constructor. 
  TapeManager( const TapeManager & );
public :
  /// \ru Деструктор. \en Destructor. 
  virtual ~TapeManager() {}
public:
  /// \ru Чтение экземпляра. \en Reading of the instance. 
  bool            SpecimenReading             ( reader & r,       TapeBase & o ) const;
  /// \ru Запись экземпляра. \en Writing of the instance. 
  bool            SpecimenWriting             ( writer & w, const TapeBase & o ) const;
  /// \ru Установить текущим класс по упакованному имени класса. \en Set class as default one by packed class name. 
  bool            SetCurrentClassByDescriptor     ( const ClassDescriptor & descr );
  /// \ru Установить текущим класс по упакованному имени класса. \en Set class as default one by packed class name. 
  bool            SetCurrentClassByDescriptor ( const ClassDescriptor & descr, const VersionContainer & ver );
  /// \ru Установить текущим класс по имени класса. \en Set class to be current by class name. 
  //bool            SetCurrentClassByName       ( const char * name );
  /// \ru Создать объект потокового класса. \en Create an object of a stream class. 
  TapeBase *      BuildObject                 () const;   
  /// \ru Есть ли функция записи у класса? \en Is there the writing function in the class? 
  bool            HasWriter                   () const;
  /// \ru Получить упакованное имя класс для записи в версию. \en Get packed class name for writing to the version. 
  ClassDescriptor GetPackedClassNameForWrite  ( VERSION version ) const;

  /// \ru Отпустить менеджер потоков после использования. \en Release the stream manager after using. 
  virtual void    FreeTapeManager();

  /// \ru Получить ссылку на менеджер потоков. \en Get a reference to the stream manager. 
  static TapeManager & GetTapeManager();

private:
  TapeManager & operator = ( const TapeManager & );
};


//----------------------------------------------------------------------------------------
/** \brief \ru Массив регистрации потоковых классов.
           \en Array of stream classes registration. \~
  \details \ru Массив регистрации потоковых классов TapeClass. \n
           \en Array of stream TapeClass classes registration. \n \~
  \ingroup Base_Tools_IO
*/
// ---
struct TapeClassContainer 
{
  /// \ru Создать массив регистрации потоковых классов. \en Create an array of stream classes registration. 
  static EXPORT_DECLARATION TPointer< SFDPArray<TapeClass> > & CALL_DECLARATION StaticTapeClassContainer();

  /// \ru Добавить потоковый класс. \en Add a stream class. 
  static bool Add( TapeClass & tapeClass )
  {
    if ( !StaticTapeClassContainer() )
      StaticTapeClassContainer() = new SFDPArray<TapeClass>( 430, 1, TapeClass_Compare, nullptr ); // \ru не владеет \en doesn't own

    return StaticTapeClassContainer()->AddExact( tapeClass );
  }
  /// \ru Функция сравнения двух TapeClass для поиска. \en Function of two TapeClass comparison for a search. 
  static int  TapeClass_Search( const TapeClass & t1, size_t d ) 
  {
    return ( ( t1.hashValue == *reinterpret_cast<ClassDescriptor*>(d) ) ? 0 : (( t1.hashValue > *reinterpret_cast<ClassDescriptor*>(d) ) ? 1 : -1 ) );
  }
  /// \ru Функция сравнения двух TapeClass для сортировки при вставке. \en Function of two TapeClass comparison for sorting while inserting. 
  static int  TapeClass_Compare( const TapeClass & t1, const TapeClass & t2 ) 
  {
    return ( ( t1.hashValue == t2.hashValue ) ? 0 : (( t1.hashValue > t2.hashValue ) ? 1 : -1 ) );
  }
};


//----------------------------------------------------------------------------------------
/// \ru Функция чтения базового класса. \en Function of reading the base class. \~ \ingroup Base_Tools_IO
// ---
template <class Base>
inline void ReadBase( reader & in, Base * base ) {
  Base::Read( in, base );
}

//----------------------------------------------------------------------------------------
/// \ru Функция записи базового класса. \en Function of writing the base class. \~ \ingroup Base_Tools_IO
// ---
template <class Base>
inline void WriteBase( writer & out, const Base * base ) {
  Base::Write( out, base );
}


//----------------------------------------------------------------------------------------
/// \ru Функция чтения виртуального базового класса. \en Function of reading of a virtual base class. \~ \ingroup Base_Tools_IO
// ---
template <class Base>
void ReadVBase( reader & in, Base * base )
{
  switch( in.readByte() ) {
    case tape::otIndexed:
      break;
    case tape::otObject :
      in.registrate( dynamic_cast<TapeBase*>(base) );
      Base::Read( in, base );
      break;
  }
}


//----------------------------------------------------------------------------------------
/// \ru Функция записи виртуального базового класса. \en Function of writing of a virtual base class. \~ \ingroup Base_Tools_IO
// ---
template <class Base>
void WriteVBase( writer & out, const Base * base )
{
  if ( !out.good() )
    return;
  if ( out.exist( dynamic_cast<TapeBase*>(base) ) )
    out.writeByte( tape::otIndexed );
  else {
    out.registrate( dynamic_cast<TapeBase*>(base) );
    out.writeByte( tape::otObject );
    Base::Write( out, base );
  }
}


//----------------------------------------------------------------------------------------
// \ru Удаление пробелов и записей перед пробелами. \en Deleting of spaces and records before spaces.  \~
// \ingroup Base_Tools_IO
// \ru Для совместимости с предыдущими компиляторами по именам возвращаемым typeid(a).name().
// \en For compatibility with the previous compilers by names returned by typeid(a).name().
// \ru Для определения того, что надо делать, скомпилируйте и запустите из консоли код:
// \en Compile and run the following code from console to define what is to do:
// #include <typeinfo>
// #include <iostream>
// class CLASS_A {
//   public: virtual ~CLASS_A() {} 
// };
//
// int main(int argc, char **argv) {
//   CLASS_A a;
//   std::cout << typeid(a).name() << '\n';
//   return 0;
// }
//
// \ru Выдаваемые значения \en Returned values  
// MS Visual C++ 6.0 ... 2010:     "class CLASS_A"
// gcc (Linux):                    "7CLASS_A"
// Embarcadero C++ 7.20 for Win32  "$CLASS_A"
// \ru Интересующая нас функция должна выдавать "CLASS_A" \en The desired function must write "CLASS_A"
//
// \ru Для имени шаблонного класса функция возвращает строку, состоящую из имени класса и имен параметров.
// \en For a template class name, the function returns a string consisting of the class name and parameter names.
// \ru Например, для имени "class ClassX<class ClassA,class ClassB>" функция возвращает "ClassXClassAClassB".
// \ru For example, for the name "class ClassX<class ClassA,class ClassB>" the function returns "ClassXClassAClassB".
// ---
MATH_FUNC( const char * ) pureName( const char * name );

//----------------------------------------------------------------------------------------
/// \ru Упаковать строку(имя класса) в uint16. \en Pack the string (class name) into uint16. \~ \ingroup Base_Tools_IO
// ---
inline
uint16 hash( const char * name )
{
  const uint16 * c = reinterpret_cast<const uint16 *>(name);

  uint16 h = uint16(strlen(name));    // Mix in the string length.
  uint16 l = h;
  uint16 i = uint16(h / sizeof(uint16));  // Could do "<<" here, but less portable.

  while ( i-- )
    h ^= *c++;  // XOR in the characters.

                // If there are any remaining characters,
                // then XOR in the rest, using a mask:
  if ( (i = uint16(l % sizeof(uint16))) != 0 ) // SKIP_SA
    h ^= uint16(*c & 0xff);

  return h;
}


//----------------------------------------------------------------------------------------
/** \brief \ru Локальное задание локали.
           \en Local setting of the locale. \~
  \details \ru Поддержка RAII (Resource Acquisition Is Initialization) для задания локали. \n
           \en Support of RAII (Resource Acquisition Is Initialization) for the setting of the locale. \~
  \ingroup Base_Tools_IO
*/ // ---
class MATH_CLASS C3DLocaleSwitcher {
  static const char * UNSET;
  
  std::string _locale;
  int         _category;
public:
  /// \ru Конструктор. При fix == false это просто заглушка.
  /// \en Constructor. With fix == false, this is just a stub.
  C3DLocaleSwitcher( const char * loc, bool fix = true, int cat = LC_CTYPE ) : _locale( UNSET ), _category( cat ) {
    if ( fix ) {
      _locale = ::setlocale( cat, nullptr );
      ::setlocale( cat, loc );
    }
  }
  /// \ru Деструктор. \en Destructor.
  ~C3DLocaleSwitcher() {
    if ( _locale != UNSET )
      ::setlocale( _category, _locale.c_str() ); // \ru Вернуть предыдущую локаль. \en Return to the previous locale.
  }
private:
  C3DLocaleSwitcher();
  OBVIOUS_PRIVATE_COPY( C3DLocaleSwitcher )
};

#define FIX_ANSI1251 "ru_RU.CP1251"

//----------------------------------------------------------------------------------------
// \ru Чтение CHAR строки из потока. (в кодировке ANSI, с русской локалью) \en Reading of CHAR string from the stream. (in ANSI coding, with Russian locale) 
// \ru И нулевой указатель и пустая строка возвращаются как нулевой указатель! \en Both null pointer and an empty string are returned as null pointer!  
// \ru Длина строки не может превышать SYS_MAX_UINT16 - 1 \en The string length cannot exceed SYS_MAX_UINT16 - 1 
// \ru Созданную строку кто-то потом должен уничтожить (через delete[]) \en Created string must be deleted by someone then (using delete[]) 
// ---
inline
reader & __readChar( reader & ps, char *& s )
{
  s = nullptr;

  if ( ps.good() )
  {
    uint16 len = 0;
    ps.readBytes( &len, sizeof(len) );  // \ru длина строки - uint16 \en string length - uint16 

    if ( len == 0              ||
         len == SYS_MAX_UINT16 ||
         (len > 0 && ps.eof()) ||
         !ps.good() )
    {
      s = nullptr;
    }
    else // good
    {
      s = new char[len + 1];
      if ( s )
      {
        if ( ps.readBytes(s, len) )
        {
          // \ru прочли сколько нужно - добавить ограничивающий 0 \en have read as much as necessary - the terminating 0 is to be added 
          s[len] = 0;
        }
        else
        {
          // \ru прочли не все, скорее всего ошибка - очистить строку \en not everything has been read, must be an error - clear the string 
          delete [] s;
          s = nullptr;
        }
      }
      else 
        ps.setState( io::outOfMemory );
    }
  }

  return ps;
}


//----------------------------------------------------------------------------------------
// \ru Чтение WCHAR строки из потока. (в потоке хранится как UTF-16) \en Reading of WCHAR string from the stream. (stored in the stream as UTF-16) 
// \ru И нулевой указатель и пустая строка возвращаются как нулевой указатель! \en Both null pointer and an empty string are returned as null pointer!  
// \ru Длина строки не может превышать SYS_MAX_UINT32 - 1 \en The string length cannot exceed SYS_MAX_UINT32 - 1 
// \ru Созданную строку кто-то потом должен уничтожить \en Created string should be deleted by someone then 
// ---
inline
reader & __readWchar( reader & ps, TCHAR * & s )
{
  s = nullptr; // \ru на случай, если ничего не прочитаем \en for case if nothing will be read
  if ( ps.good() )
  {
    uint32 len = 0; 
    if ( ps.readBytes(&len, sizeof(len)) &&  // \ru длина строки в символах без терминального нуля - uint32 \en the string length in symbols without the termination zero - uint32 
         len != 0               &&
         len != SYS_MAX_UINT32  &&
         !ps.eof() )
    {
      uint16 * readBuf = new uint16[static_cast<size_t>(len) + 1]; // \ru длина (количество символов) вычитываемой строки с терминальным нулем \en length (number of symbols) of string being read with terminating 0 
      size_t size = sizeof(uint16) * static_cast<size_t>(len);     // \ru длина (в байтах) вычитываемой строки без терминального нуля \en length (in bytes) of string being read without terminating null 

      if ( ps.readBytes(readBuf, size) ) 
        // \ru прочли сколько нужно - добавить ограничивающий 0 \en have read as much as necessary - the terminating 0 is to be added 
        readBuf[static_cast<size_t>(len)] = 0; 
      else {
        // \ru прочли не все, скорее всего ошибка - очистить строку \en not everything has been read, must be an error - clear the string 
        delete [] readBuf;
        readBuf = nullptr;
      }

      if ( readBuf ) { // is OK
#ifdef _UNICODE  // TCHAR == wchar_t
 #if __SIZEOF_WCHAR_T__ == 2  // sizeof(wchar_t) == sizeof(uint16)
        s = reinterpret_cast<TCHAR *>(readBuf); // \ru собственно ничего конвертировать не нужно \en nothing to convert 
 #else // sizeof(wchar_t) == sizeof(uint32)
        s = reinterpret_cast<TCHAR *>(Utf16ToUcs4(readBuf)); // \ru Конвертировать UTF-16 в WCHAR \en Convert from UTF-16 to WCHAR 
        delete [] readBuf;
 #endif
#else // _UNICODE 
 #if __SIZEOF_WCHAR_T__ == 2  // sizeof(wchar_t) == sizeof(uint16)
        s = wcsnewmbs( (const wchar_t*)readBuf );  // \ru Конвертировать WCHAR в CHAR строку. \en Convert WCHAR-string to CHAR-string. 
 #else // sizeof(wchar_t) == sizeof(uint32)
  uint32* readBuf32 = Utf16ToUcs4(readBuf); // \ru Конвертировать UTF-16 в WCHAR \en Convert from UTF-16 to WCHAR 
  s = wcsnewmbs( (const wchar_t*)readBuf32 );  // \ru Конвертировать WCHAR в CHAR строку. \en Convert WCHAR-string to CHAR-string. 
  delete [] readBuf32;
 #endif
        delete [] readBuf;
#endif // _UNICODE 
      }
    }
  }

  return ps;
}


//----------------------------------------------------------------------------------------
// \ru Чтение WCHAR строки из потока. (в потоке хранится как UTF-16) \en Reading of WCHAR string from the stream. (stored in the stream as UTF-16) 
// \ru И нулевой указатель и пустая строка возвращаются как нулевой указатель! \en Both null pointer and an empty string are returned as null pointer!  
// \ru Длина строки не может превышать SYS_MAX_UINT32 - 1 \en The string length cannot exceed SYS_MAX_UINT32 - 1 
// \ru Созданную строку кто-то потом должен уничтожить \en Created string should be deleted by someone then 
// ---
inline
reader & __readWcharT( reader & ps, wchar_t * & s )
{
  s = nullptr; // \ru на случай, если ничего не прочитаем \en for case if nothing will be read
  if ( ps.good() )
  {
    uint32 len = 0; 
    if ( ps.readBytes(&len, sizeof(len)) &&  // \ru длина строки в символах без терминального нуля - uint32 \en the string length in symbols without the termination zero - uint32 
         len != 0               &&
         len != SYS_MAX_UINT32  &&
         !ps.eof() )
    {
      uint16 * readBuf = new uint16[(size_t)len + 1]; // \ru длина (количество символов) вычитываемой строки с терминальным нулем \en length (number of symbols) of string being read with terminating 0 
      size_t size = sizeof(uint16) * static_cast<size_t>(len);     // \ru длина (в байтах) вычитываемой строки без терминального нуля \en length (in bytes) of string being read without terminating null 

      if ( ps.readBytes(readBuf, size) ) 
        // \ru прочли сколько нужно - добавить ограничивающий 0 \en have read as much as necessary - the terminating 0 is to be added 
        readBuf[static_cast<size_t>(len)] = 0; 
      else {
        // \ru прочли не все, скорее всего ошибка - очистить строку \en not everything has been read, must be an error - clear the string 
        delete [] readBuf;
        readBuf = nullptr;
      }

      if ( readBuf ) { // is OK
#if __SIZEOF_WCHAR_T__ == 2  // sizeof(wchar_t) == sizeof(uint16)
        s = reinterpret_cast<wchar_t *>(readBuf); // \ru собственно ничего конвертировать не нужно \en nothing to convert 
#else // sizeof(wchar_t) == sizeof(uint32)
        s = reinterpret_cast<wchar_t *>(Utf16ToUcs4(readBuf)); // \ru Конвертировать UTF-16 в WCHAR \en Convert from UTF-16 to WCHAR 
        delete [] readBuf;
#endif
      }
    }
  }

  return ps;
}


#ifdef C3D_WINDOWS //_MSC_VER // \ru Код для поддержки КОМПАС \en Code for KOMPAS support 
//----------------------------------------------------------------------------------------
/// \ru Чтение CHAR строки из потока. \en Reading of CHAR string from the stream. \~ \ingroup Base_Tools_IO
// \ru И нулевой указатель и пустая строка возвращаются как нулевой указатель! \en Both null pointer and an empty string are returned as null pointer!  
// ---
#ifndef DISABLE_RWTCHAR
#ifdef _UNICODE
inline
reader & operator >> ( reader & ps, char *& s )
{
  return __readChar( ps, s );
}
#endif // _UNICODE
#endif // DISABLE_RWTCHAR


//----------------------------------------------------------------------------------------
/// \ru Запись CHAR строки в поток. \en Writing CHAR string to the stream. \~ \ingroup Base_Tools_IO
// ---
#ifndef DISABLE_RWTCHAR
#ifdef _UNICODE
inline
writer & operator << ( writer & ps, const char * s )
{
  return ps.__writeChar( s );
}
#endif // _UNICODE
#endif // DISABLE_RWTCHAR

//----------------------------------------------------------------------------------------
/// \ru Чтение WCHAR строки из потока. \en Reading of WCHAR string from the stream. \~ \ingroup Base_Tools_IO
// \ru И нулевой указатель и пустая строка возвращаются как нулевой указатель! \en Both null pointer and an empty string are returned as null pointer!  
// \ru длина строки не может превышать SYS_MAX_UINT32 - 1 \en length of string can't exceed SYS_MAX_UINT32 - 1 
// ---
#ifndef DISABLE_RWTCHAR
inline
reader & operator >> ( reader & ps, TCHAR *& s )
{
  return __readWchar( ps, s );
}
#endif // DISABLE_RWTCHAR


//----------------------------------------------------------------------------------------
/// \ru Запись WCHAR строки в поток. \en Writing WCHAR string to the stream. \~ \ingroup Base_Tools_IO
// \ru длина строки не может превышать SYS_MAX_UINT32 - 1 \en string length can't exceed SYS_MAX_UINT32 - 1 
// ---
#ifndef DISABLE_RWTCHAR
inline
writer & operator << ( writer & ps, const TCHAR * s )
{
  return ps.__writeWchar( s );
}
#endif // DISABLE_RWTCHAR

#endif // C3D_WINDOWS

#ifdef __MOBILE_VERSION__
#ifdef _UNICODE
inline reader & operator >> ( reader & ps, char *& s ){  return __readChar( ps, s );}
inline writer & operator << ( writer & ps, const char * s ){  return ps.__writeChar( s );}
inline reader & operator >> ( reader & ps, TCHAR *& s ){  return __readWchar( ps, s );}
inline writer & operator << ( writer & ps, const TCHAR * s ){  return ps.__writeWchar( s );}
#endif // _UNICODE
#endif // __MOBILE_VERSION__

//----------------------------------------------------------------------------------------
/// \ru Запись bool в поток. \en Writing bool to the stream. \~ \ingroup Base_Tools_IO
// ---
inline
writer & operator << ( writer & ps, bool i )
{
  //unsigned char val = i;
  uint8 val = i ? 1 : 0;
  ps.writeByte( val );
  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Чтение bool в поток. \en Reading of bool to the stream. \~ \ingroup Base_Tools_IO
// ---
inline
reader & operator >> ( reader & ps, bool & i )
{
  if ( IsVersion16bit( ps.MathVersion() ) )
  {
    unsigned short tmp = 0;
    ps.readBytes( &tmp, sizeof(unsigned short) );
    i = !!tmp;
  }
  else 
    i = !!ps.readByte();
  
  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Запись signed int в поток. \en Writing signed int to the stream. \~ \ingroup Base_Tools_IO
// \ru оператор записи для типов: int (не поддерживает int32, long, LONG - для них есть своя реализация) \en write operator for types: int (doesn't support int32, long, LONG - there is a separate implementation for them)  
// \ru поддерживает запись в 32 и 16-битный формат файла \en supports writing to 32- and 16-bit format of file 
// ---
inline
writer & operator << ( writer & ps, signed int i )
{
#ifdef C3D_WINDOWS //_MSC_VER 
  if ( IsVersion16bit( ps.MathVersion() ) )
  {
    // \ru чтение из 16-битной версии файла \en reading from 16-bit version of file 
    // \ru ради этого куска чтение/запись int отделено от чтения/записи long \en reading/writing of int is separated from reading/writing of long for the sake of this fragment 
    int16 val = (int16)i;
    ps.writeBytes( &val, sizeof(val) );
  }
  else {
    int32 val = (int32)i;
    ps.writeBytes( &val, sizeof(val) );
  }
  return ps;
#else // C3D_WINDOWS
  if ( IsVersion16bit( ps.MathVersion() ) )
    ps.setState( io::fail );  // \ru в Linux-версии 16-битные файлы не поддерживаются \en 16-bit files are not supported in Linux 
  else {
    int32 val = static_cast<int32>(i);
    ps.writeBytes( &val, sizeof(val) );
  }
  return ps;
#endif // C3D_WINDOWS 
}


//----------------------------------------------------------------------------------------
/// \ru Запись unsigned int в поток. \en Writing unsigned int to the stream.  \~ \ingroup Base_Tools_IO
// \ru оператор записи для типов: uint (не поддерживает uint32, ulong - для них есть своя реализация) \en write operator for types: uint (doesn't support uint32, ulong - there is a separate implementation for them)  
// \ru поддерживает запись в 32 и 16-битный формат файла \en supports writing to 32- and 16-bit format of file 
// ---
inline
writer & operator << ( writer & ps, unsigned int i )
{
#ifdef C3D_WINDOWS //_MSC_VER 
  if ( IsVersion16bit( ps.MathVersion() ) ) { 
    // \ru чтение из 16-битной версии файла \en reading from 16-bit version of file 
    // \ru ради этого куска чтение/запись int отделено от чтения/записи long \en reading/writing of int is separated from reading/writing of long for the sake of this fragment 
    uint16 val = (uint16)i;
    ps.writeBytes( &val, sizeof(val) );
  }
  else {
    uint32 val = i;
    ps.writeBytes( &val, sizeof(val) );
  }
  return ps;
#else // C3D_WINDOWS 
  if ( IsVersion16bit( ps.MathVersion() ) ) 
    ps.setState( io::fail );  // \ru в Linux-версии 16-битные файлы не поддерживаются \en 16-bit files are not supported in Linux 
  else {
    uint32 val = i;
    ps.writeBytes( &val, sizeof(val) );
  }
  return ps;
#endif // C3D_WINDOWS 
}


//----------------------------------------------------------------------------------------
/// \ru Чтение signed int в поток. \en Reading signed int to the stream. \~ \ingroup Base_Tools_IO
// \ru оператор чтения для типов: int (не поддерживает int32, long, LONG - для них есть своя реализация) \en read operator for types: int (doesn't support int32, long, LONG - there is a separate implementation for them)  
// \ru поддерживает чтение из 32 и 16-битного формата файла \en supports reading from 32- and 16-bit format of file 
// ---
inline
reader & operator >> ( reader & ps, signed int & i )
{
#ifdef C3D_WINDOWS //_MSC_VER // Linux identical int/uint and int32/uint32
  if ( IsVersion16bit( ps.MathVersion() ) ) { 
    // \ru чтение из 16-битной версии файла \en reading from 16-bit version of file 
    // \ru ради этого куска чтение/запись int отделено от чтения/записи long \en reading/writing of int is separated from reading/writing of long for the sake of this fragment 
    int16 val = 0;
    ps.readBytes( &val, sizeof(val) );
    i = val;
  }
  else {
    int32 val = 0;
    ps.readBytes( &val, sizeof(val) );
    i = static_cast<signed int>(val);
  }
  return ps;
#else // C3D_WINDOWS
  if ( IsVersion16bit( ps.MathVersion() ) ) 
    ps.setState( io::fail );  // \ru в Linux-версии 16-битные файлы не поддерживаются \en 16-bit files are not supported in Linux 
  else {
    int32 val = 0;
    ps.readBytes( &val, sizeof(val) );
    i = static_cast<signed int>(val);
  }
  return ps;
#endif // C3D_WINDOWS
}


//----------------------------------------------------------------------------------------
/// \ru Чтение unsigned int в поток. \en Reading unsigned int to the stream. \~ \ingroup Base_Tools_IO
// \ru оператор чтения для типов: uint (не поддерживает uint32, ulong - для них есть своя реализация) \en read operator for types: uint (doesn't support uint32, ulong - there is a separate implementation for them)  
// \ru поддерживает чтение из 32 и 16-битного формата файла \en supports reading from 32- and 16-bit format of file 
// ---
inline
reader & operator >> ( reader & ps, unsigned int & i )
{
#ifdef C3D_WINDOWS //_MSC_VER // Linux identical int/uint and int32/uint32
  if ( IsVersion16bit( ps.MathVersion() ) )
  {
    // \ru чтение из 16-битной версии файла \en reading from 16-bit version of file 
    // \ru ради этого куска чтение/запись int отделено от чтения/записи long \en reading/writing of int is separated from reading/writing of long for the sake of this fragment 
    uint16 val = 0;
    ps.readBytes( &val, sizeof(val) );
    i = val;
  }
  else {
    uint32 val = 0;
    ps.readBytes( &val, sizeof(val) );
    i = static_cast<unsigned int>( val );
  }
  return ps;
#else // C3D_WINDOWS
  if ( IsVersion16bit( ps.MathVersion() ) ) 
    ps.setState( io::fail );  // \ru в Linux-версии 16-битные файлы не поддерживаются \en 16-bit files are not supported in Linux 
  else {
    uint32 val = 0;
    ps.readBytes( &val, sizeof(val) );
    i = static_cast<unsigned int>( val );
  }
  return ps;
#endif // C3D_WINDOWS
}


//----------------------------------------------------------------------------------------
/// \ru Запись int32 в поток. \en Writing int32 to the stream. \~ \ingroup Base_Tools_IO
// \ru оператор записи для типов: long, int32, LONG (не поддерживает int - для него есть своя реализация) \en write operator for types: long, int32, LONG (does not support int - there is a separate implementation for it)  
// \ru данные всегда пишутся в 32-разрядном формате \en data are always written in 32-bit format 
// ---
#ifdef C3D_WINDOWS //_MSC_VER // Linux identical int/uint and int32/uint32
// \ru ВНИМАНИЕ!!! В целях совместимости данных для задач скомпилированных под Windows и Linux \en NOTE!!! To provide data compatibility for tasks compiled for Windows and Linux  
// \ru ЗАПРЕЩАЕТСЯ использовать тип данных long и unsigned long. Используйте int32 и uint32 \en IT IS FORBIDDEN to use long and unsigned long data types. Use int32 and uint32 
inline
writer & operator << ( writer & ps, int32 l )
{
  ps.writeBytes( &l, sizeof(l) );
  return ps;
}
#endif // C3D_WINDOWS


//----------------------------------------------------------------------------------------
/// \ru Запись uint32 в поток. \en Writing uint32 to the stream. \~ \ingroup Base_Tools_IO
// \ru оператор записи для типов: ulong, uint32 (не поддерживает uint - для него есть своя реализация) \en write operator for types: ulong, uint32 (does not support uint - there is a separate implementation for it)  
// \ru данные всегда пишутся в 32-разрядном формате \en data are always written in 32-bit format 
// ---
#ifdef C3D_WINDOWS // Linux identical int/uint and int32/uint32
// \ru ВНИМАНИЕ!!! В целях совместимости данных для задач скомпилированных под Windows и Linux \en NOTE!!! To provide data compatibility for tasks compiled for Windows and Linux  
// \ru ЗАПРЕЩАЕТСЯ использовать тип данных long и unsigned long. Используйте int32 и uint32 \en IT IS FORBIDDEN to use long and unsigned long data types. Use int32 and uint32 
inline
writer & operator << ( writer & ps, uint32 l )
{
  ps.writeBytes( &l, sizeof(l) );
  return ps;
}
#endif // C3D_WINDOWS


//----------------------------------------------------------------------------------------
/// \ru Чтение int32 в поток. \en Reading int32 to the stream. \~ \ingroup Base_Tools_IO
// \ru оператор чтения для типов: long, int32, LONG  (не поддерживает int - для него есть своя реализация) \en read operator for types: long, int32, LONG (does not support int - there is a separate implementation for it)  
// \ru данные всегда читаются в 32-разрядном формате \en data are always read in 32-bit format 
// ---
#ifdef C3D_WINDOWS //_MSC_VER // Linux identical int/uint and int32/uint32
// \ru ВНИМАНИЕ!!! В целях совместимости данных для задач скомпилированных под Windows и Linux \en NOTE!!! To provide data compatibility for tasks compiled for Windows and Linux  
// \ru ЗАПРЕЩАЕТСЯ использовать тип данных long и unsigned long. Используйте int32 и uint32 \en IT IS FORBIDDEN to use long and unsigned long data types. Use int32 and uint32 
inline
reader & operator >> ( reader & ps, int32 & l )
{
  size_t size = sizeof(int32);
  if ( !ps.readBytes(&l, size) )
    l = 0;

  return ps;
}
#endif // C3D_WINDOWS


//----------------------------------------------------------------------------------------
/// \ru Чтение uint32 в поток. \en Reading uint32 to the stream. \~ \ingroup Base_Tools_IO
// \ru оператор чтения для типов: ulong, uint32 (не поддерживает uint - для него есть своя реализация) \en read operator for types: ulong, uint32 (does not support uint - there is a separate implementation for it)  
// \ru данные всегда читаются в 32-разрядном формате \en data are always read in 32-bit format 
// ---
#ifdef C3D_WINDOWS //_MSC_VER // Linux identical int/uint and int32/uint32
// \ru ВНИМАНИЕ!!! В целях совместимости данных для задач скомпилированных под Windows и Linux \en NOTE!!! To provide data compatibility for tasks compiled for Windows and Linux  
// \ru ЗАПРЕЩАЕТСЯ использовать тип данных long и unsigned long. Используйте int32 и uint32 \en IT IS FORBIDDEN to use long and unsigned long data types. Use int32 and uint32 
inline
reader & operator >> ( reader & ps, uint32 & l )
{
  size_t size = sizeof(uint32);
  if ( !ps.readBytes(&l, size) )
    l = 0;

  return ps;
}
#endif // C3D_WINDOWS


//----------------------------------------------------------------------------------------
/// \ru Запись int64 в поток. \en Writing int64 to the stream. \~ \ingroup Base_Tools_IO
// ---
inline
writer & operator << ( writer & ps, int64 val )
{
  ps.writeInt64( val );
  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Чтение int64 в поток. \en Reading int64 to the stream. \~ \ingroup Base_Tools_IO
// ---
inline
reader & operator >> ( reader & ps, int64 & val )
{
  ps.readInt64( val );
  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Чтение signed char в поток. \en Reading signed char to the stream. \~ \ingroup Base_Tools_IO
// ---
inline
reader & operator >> ( reader & ps, signed char & ch )
{
  ch = static_cast<signed char>(ps.readByte());
  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Чтение unsigned char в поток. \en Reading unsigned char to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
reader & operator >> ( reader & ps, unsigned char & ch )
{
  ch = static_cast<unsigned char>(ps.readByte());
  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Чтение char в поток. \en Reading char to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
reader & operator >> ( reader & ps, char & ch )
{
  ch = static_cast<char>(ps.readByte());
  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Чтение signed short в поток. \en Reading signed short to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
reader & operator >> ( reader & ps, signed short & sh )
{
  const size_t size = sizeof(sh);
  
  if ( !ps.readBytes(&sh, size) )
    sh = 0;

  return ps;
}

//----------------------------------------------------------------------------------------
/// \ru Чтение unsigned short в поток. \en Reading unsigned short to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
reader & operator >> ( reader & ps, unsigned short & sh ) 
{
  const size_t size = sizeof(sh);
  
  if ( !ps.readBytes(&sh, size) )
    sh = 0;

  return ps;
}


#ifndef __ATS_BUILD__ // Нужно для сборки решения Tools\ATS...
//----------------------------------------------------------------------------------------
/// \ru Чтение wchar_t в поток. \en Reading wchar_t to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
reader & operator >> ( reader & ps, wchar_t & sh )
{
  sh = 0; //Обнулить, т.к. размер 4 байта, а читаются только 2
  //size_t size = sizeof(sh);
  size_t size = 2; //В windows sizeof(wchar_t) = 2, в Android  sizeof(wchar_t) = 4

  if ( !ps.readBytes(&sh, size) )

  sh = 0;

  return ps;
}
#endif // __ATS_BUILD__


//----------------------------------------------------------------------------------------
/// \ru Чтение float в поток. \en Reading float to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
reader & operator >> ( reader & ps, float & f )
{
  size_t size = sizeof(f);
  
  if ( !ps.readBytes(&f, size) )
    f = 0;

  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Чтение double в поток. \en Reading double to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
reader & operator >> ( reader & ps, double & d )
{
  size_t size = sizeof(d);

  if ( !ps.readBytes(&d, size) )
    d = 0;

  // \ru Проверка числа на определенность \en Check if the number is defined
  if ( c3d_isnan(d) )
    d = 0;

  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Чтение long double из потока. \en Reading long double from the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
reader & operator >> ( reader & ps, long double & l )
{
  size_t size = sizeof(l);

  if ( !ps.readBytes(&l, size) )
    l = 0;

  return ps;
}

//----------------------------------------------------------------------------------------
/// \ru Чтение smart-указателя из потока. \en Reading a smart pointer from the stream. \~ \ingroup Base_Tools_IO
// ---
template<class _Class>
inline
reader & operator >> ( reader & ps, SPtr<_Class> & sPtr )
{
  _Class * ptr = nullptr;
  ps >> ptr;
  sPtr.assign( ptr );
  return ps;
}

//----------------------------------------------------------------------------------------
/// \ru Запись smart-указателя в поток. \en Writing a smart pointer to the stream. \~ \ingroup Base_Tools_IO
// ---
template<class _Class>
inline
writer & operator << ( writer & ps, const SPtr<_Class> & sPtr )
{
  ps << sPtr.get();
  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Запись signed char в поток. \en Write signed char to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
writer & operator << ( writer & ps, signed char ch )
{
  ps.writeByte( ch ); // \ru байт \en byte 
  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Запись unsigned char в поток. \en Write unsigned char to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
writer & operator << ( writer & ps, unsigned char ch )
{
  ps.writeByte( ch ); // \ru байт \en byte 
  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Запись char в поток. \en Write char to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
writer & operator << ( writer & ps, char ch )
{
  ps.writeByte( ch ); // \ru байт \en byte 
  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Запись signed short в поток. \en Write signed short to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
writer & operator << ( writer & ps, signed short sh )
{
  ps.writeBytes( &sh, sizeof(sh) );
  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Запись unsigned short в поток. \en Write unsigned short to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
writer & operator << ( writer & ps, unsigned short sh )
{
  ps.writeBytes( &sh, sizeof(sh) );
  return ps;
}


#ifndef __ATS_BUILD__ // Нужно для сборки решения Tools\ATS...
//----------------------------------------------------------------------------------------
/// \ru Запись wchar_t в поток. \en Write wchar_t to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
writer & operator << ( writer & ps, wchar_t sh )
{
  size_t size = 2; //В windows sizeof(wchar_t) = 2, в Android  sizeof(wchar_t) = 4
  ps.writeBytes( &sh, size );
  return ps;
}
#endif // __ATS_BUILD__


//----------------------------------------------------------------------------------------
/// \ru Запись float в поток. \en Write float to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
writer & operator << ( writer & ps, float f )
{
  ps.writeBytes( &f, sizeof(f) );
  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Запись double в поток. \en Write double to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
writer & operator << ( writer & ps, const double & d )
{
  ps.writeBytes( &d, sizeof(d) );
  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Запись long double в поток. \en Write long double to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
writer & operator << ( writer & ps, const long double & l )
{
  ps.writeBytes( &l, sizeof(l) );
  return ps;
}

//----------------------------------------------------------------------------------------
/// \ru Записать TCHAR строку в поток. \en Write TCHAR string to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
void WriteTCHAR( writer & out, const TCHAR * ts, bool directSingleByte = false ) 
{
  if ( directSingleByte || out.MathVersion() < UNICODE_VERSION ) 
  {
#ifndef C3D_WINDOWS
    C3DLocaleSwitcher lh( FIX_ANSI1251, out.MathVersion() < UNICODE_VERSION );
#endif
    // \ru пишем WCHAR* как CHAR* \en write WCHAR* as CHAR* 
    char * s = _tcsNstr( ts );  // \ru создаем ANSI из TCHAR (если TCHAR == char, то просто дублируем) \en create ANSI from TCHAR (if TCHAR == char, then simply duplicate) 
    out.__writeChar( s );      // \ru пишем строку в формате ANSI \en write string in ANSI format 
    delete [] s;
  }
  else 
  {
    // \ru пишем WCHAR* \en write WCHAR* 
    out.__writeWchar( ts ); 
  }
}

//----------------------------------------------------------------------------------------
/// \ru Прочитать TCHAR строку из потока. \en Read TCHAR string from the stream.  \~ \ingroup Base_Tools_IO
//---
inline
void ReadTCHAR( reader & in, TCHAR *& ts, bool directSingleByte = false ) 
{
  if ( directSingleByte || in.MathVersion() < UNICODE_VERSION )
  {
#ifndef C3D_WINDOWS
    C3DLocaleSwitcher lh( FIX_ANSI1251, in.MathVersion() < UNICODE_VERSION );
#endif
    // \ru читаем WCHAR* из CHAR* \en read WCHAR* from CHAR* 
    char * s = nullptr;
    __readChar( in, s );  // \ru читаем строку в формате ANSI \en read string in ANSI format 
    ts = _strNtcs( s );   // \ru создаем TCHAR из ANSI (если TCHAR == char, то просто дублируем) \en create TCHAR from ANSI (if TCHAR == char, then simply duplicate) 
    delete [] s;
  }
  else 
  {
    // \ru читаем WCHAR* \en read WCHAR* 
    __readWchar( in, ts ); 
  }
}


//----------------------------------------------------------------------------------------
/// \ru Записать wchar_t строку в поток. \en Write wchar_t string to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
void WriteWcharT( writer & out, const wchar_t * ts ) 
{
  out.__writeWcharT( ts ); 
}

//----------------------------------------------------------------------------------------
/// \ru Прочитать TCHAR строку из потока. \en Read TCHAR string from the stream.  \~ \ingroup Base_Tools_IO
//---
inline
void ReadWcharT( reader& in, wchar_t *& ts ) 
{
  __readWcharT( in, ts ); 
}


//----------------------------------------------------------------------------------------
/// \ru Запись size_t в зависимости от версии потока. \en Write size_t subject to the stream version.  \~ \ingroup Base_Tools_IO
// ---
inline
void WriteCOUNT( writer & out, size_t count )
{
  if ( IsVersion64bit( out.MathVersion() ) )
  {
    uint64 count64 = count;

    if ( out.MathVersion() >= 0x0F001001L ) {
      if ( count64 == SYS_MAX_T )
        count64 = SYS_MAX_UINT64;
    }

    out.writeUInt64( count64 );
  }
  else
  {
    // \ru OV_x64 проверить переполнение при записи 64-битных данных в 32-битный поток \en OV_x64 check for overflow while writing 64-bit data to 32-bit stream 
    if ( HiUint32( count ) != 0 )
      out.setState( io::underflow64to32 );

    uint32 _count = static_cast<uint32>(LoUint32(count));
    out << _count;
  }
}

//----------------------------------------------------------------------------------------
/// \ru Запись ptrdiff_t в зависимости от версии потока. \en Writing ptrdiff_t subject to the stream version.  \~ \ingroup Base_Tools_IO
// ---
inline
void WriteINT_T( writer & out, ptrdiff_t count )
{
  if ( IsVersion64bit( out.MathVersion() ) )
  {
    out.writeInt64( count );
  }
  else
  {
    // \ru OV_x64 проверить переполнение при записи 64-битных данных в 32-битный поток \en OV_x64 check for overflow while writing 64-bit data to 32-bit stream 
    if ( static_cast<int64>(count) > static_cast<int64>(SYS_MAX_INT32) || static_cast<int64>(count) < static_cast<int64>(SYS_MIN_INT32) )
      out.setState( io::underflow64to32 );

    int32 _count = static_cast<int32>(LoUint32(count));
    out << _count;
  }
}

//----------------------------------------------------------------------------------------
/// \ru Чтение size_t в зависимости от версии потока. \en Reading size_t subject to the stream version.  \~ \ingroup Base_Tools_IO
//
// \ru Т.к. операторы чтения/записи для uint и uint32 разные нужно \en Since reading/writing operators are different for uint and uint32,  
// \ru уметь читать оба, в зависимости от места где вызывается. \en there should be capability for reading both of them subject to the place where it is called. 
// \ru Решено запись не менять, т.к. в 16 битовую задачу не записываем. \en Decided not to modify writing since we do not write to 16-bit task. 
// ---
inline
size_t ReadCOUNT ( reader & in, bool uint_val = true )
{
  size_t count = 0;
  if ( IsVersion64bit( in.MathVersion() ) )
  {
    uint64  _count = 0;
    in.readUInt64( _count );

    if ( in.MathVersion() >= 0x0F001001L ) {
      if ( _count == SYS_MAX_UINT64 )
        _count = SYS_MAX_T;
    }
    else {
      // \Mapping\a_17356\17356_Телега.c3d PRECONDITION( _count != SYS_MAX_UINT32 ); // неоднозначность интерпретации значения // Primery_Set/Models_8/_58720. 
      if ( _count == SYS_MAX_UINT32 || _count == SYS_MAX_UINT64 )
        _count = SYS_MAX_T;
    }

    count = static_cast<size_t>(_count);

    // \ru OV_x64 проверить переполнение при чтении 64-битных данных в 32-битной задаче \en OV_x64 check for overflow while reading 64-bit data in 32-bit task 
    if ( HiUint32(count) != HiUint32(_count) )
      in.setState( io::underflow64to32 );
  }
  else
  {
    // \ru оператор чтения в uint (в отличии от uint32) поддерживает чтение 16-битных версий файла \en operator of reading to uint (in contrast to uint32) supports reading 16-bit versions of file 
    if ( uint_val )
    {
      uint _count = 0;
      in >> _count;

      count = static_cast<size_t>(_count);
    }
    else
    {
      uint32 _count = 0;
      in >> _count;
      count = static_cast<size_t>(_count);
    }

    if ( count == SYS_MAX_UINT32 )
      count = SYS_MAX_T;
  }

  return count;
}

//----------------------------------------------------------------------------------------
/// \ru Чтение ptrdiff_t в зависимости от версии потока. \en Reading ptrdiff_t subject to the stream version.  \~ \ingroup Base_Tools_IO
//
// \ru Т.к. операторы чтения/записи для uint и uint32 разные нужно \en Since reading/writing operators are different for uint and uint32,  
// \ru уметь читать оба, в зависимости от места где вызывается. \en there should be capability for reading both of them subject to the place where it is called. 
// \ru Решено запись не менять, т.к. в 16 битовую задачу не записываем. \en Decided not to modify writing since we do not write to 16-bit task. 
// ---
// \ru САА K13 31.8.2010 Исправление BUG 52091 \en CAA K13 31.8.2010 Fix for BUG 52091 
// \ru 77 вызовов и из них только 2 с false!!! - поэтому по умолчанию для всех \en 77 calls and only 2 of them with false!!! - so it is default for all 
inline
ptrdiff_t ReadINT_T( reader & in, bool uint_val = true )
{
  ptrdiff_t count = 0;
  if ( IsVersion64bit( in.MathVersion() ) )
  {
    int64  _count = 0;
    in.readInt64( _count );
    count = static_cast<ptrdiff_t>(_count); 

    // \ru OV_x64 проверить переполнение при чтении 64-битных данных в 32-битной задаче \en OV_x64 check for overflow while reading 64-bit data in 32-bit task 
    if ( HiInt32(count) != HiInt32(_count) )
      in.setState( io::underflow64to32 );
  }
  else
  {
    // \ru оператор чтения в uint (в отличии от uint32) поддерживает чтение 16-битных версий файла \en operator of reading to uint (in contrast to uint32) supports reading 16-bit versions of file 
    if ( uint_val )
    {
      int _count = 0;
      in >> _count;
      count = static_cast<ptrdiff_t>(_count);
    }
    else
    {
      int32 _count = 0;
      in >> _count;
      count = static_cast<ptrdiff_t>(_count);
    }
  }

  return count;
}


//----------------------------------------------------------------------------------------
/// \ru Запись size_t в память в зависимости от версии потока. \en Writing size_t to the memory subject to the stream version.  \~ \ingroup Base_Tools_IO
// ---
inline
void WriteCOUNT( void * out, VERSION version, size_t count )
{
  if ( IsVersion64bit(version) )
  {
    const uint64 count64 = static_cast<uint64>(count);
    ::memcpy( out, &count64, sizeof(count64) );
  }
  else
  {
    // \ru OV_x64 проверить переполнение при записи 64-битных данных в 32-битный поток \en OV_x64 check for overflow while writing 64-bit data to 32-bit stream 
    //OV_x64 if ( count > (int64)_I32_MAX || count < (int64)_I32_MIN )
    //OV_x64   out.setState( io::underflow64to32 );

    PRECONDITION( count <= static_cast<size_t>(SYS_MAX_UINT32)/*_UI32_MAX*/ );
    const uint32 count32 = static_cast<uint32>(LoUint32(count));

    ::memcpy( out, &count32, sizeof(count32) );
  }
}

//----------------------------------------------------------------------------------------
/// \ru Запись ptrdiff_t в память в зависимости от версии потока. \en Writing ptrdiff_t to the memory subject to the stream version.  \~ \ingroup Base_Tools_IO
// ---
inline
void WriteCOUNT( void * out, VERSION version, ptrdiff_t count )
{
  if ( IsVersion64bit(version) )
  {
    const int64 count64 = static_cast<int64>(count);
    ::memcpy( out, &count64, sizeof(count64) );
  }
  else
  {
    // \ru OV_x64 проверить переполнение при записи 64-битных данных в 32-битный поток \en OV_x64 check for overflow while writing 64-bit data to 32-bit stream 
    //OV_x64 if ( count > (int64)_I32_MAX || count < (int64)_I32_MIN )
    //OV_x64   out.setState( io::underflow64to32 );

    PRECONDITION( count <= static_cast<ptrdiff_t>(SYS_MAX_INT32)/*_I32_MAX*/ && count >= static_cast<ptrdiff_t>(SYS_MIN_INT32)/*_I32_MIN*/ );
    const int32 count32 = static_cast<int32>(LoUint32(count));

    ::memcpy( out, &count32, sizeof(count32) );
  }
}

//----------------------------------------------------------------------------------------
/// \ru Чтение size_t в память в зависимости от версии потока. \en Reading of size_t to the memory subject to the stream version.  \~ \ingroup Base_Tools_IO
// ---
inline
size_t ReadCOUNT ( void * in, VERSION version )
{
  size_t count = 0;

  if ( IsVersion64bit(version) )
  {
    uint64 count64 = 0;
    ::memcpy( &count64, in, sizeof(count64) );
    PRECONDITION( count64 <= SYS_MAX_T/*SIZE_MAX*/ );
    count = static_cast<size_t>(count64);

    // \ru OV_x64 проверить переполнение при чтении 64-битных данных в 32-битной задаче \en OV_x64 check for overflow while reading 64-bit data in 32-bit task 
    //OV_x64 if ( HiUint32( count ) != HiUint32( _count ) )
    //OV_x64   in.setState( io::underflow64to32 );
  }
  else
  {
    uint32 count32 = 0;
    ::memcpy( &count32, in, sizeof(count32) );
    PRECONDITION( sizeof(count32) <= sizeof(count)); // Cannot readCOUNT because sizof(uint32) > sizeof(size_t)
    count = /*AR (size_t)*/count32;   //-V101
  }

  return count;
}


//----------------------------------------------------------------------------------------
/** \brief \ru Получить упакованное имя класса по значению хэша записанному в поток.
           \en Get the packed class name from the hash value written to the stream. \~
  \details \ru Получить упакованное имя класса по значению хэша записанному в поток. \n
           \en Get the packed class name from the hash value written to the stream. \n \~
  \param[in] classDescr - \ru Обертка хэша.
                          \en Hash value wrapper. \~
  \param[in] ver  - \ru Версия потока в котором записан хэш. 
                    \en The version of the stream in which the hash is written. \~
  \result \ru Возвращает упакованное имя класса.
          \en Returns the packed class name. \~
  \ingroup Base_Tools_IO
*/ // ---
MATH_FUNC (ClassDescriptor) GetPackedClassName( const ClassDescriptor & classDescr, const VersionContainer & ver ); 


//----------------------------------------------------------------------------------------
/** \brief \ru Добавить новое соответствие значения хэша записанного в поток упакованному имени класса.
           \en Add a new mapping of the hash value written to the stream to the packed class name. \~
  \details \ru Добавить новое соответствие значения хэша записанного в поток упакованному имени класса. \n
           \en Add a new mapping of the hash value written to the stream to the packed class name. \n \~
  \param[in]  newClassName - \ru Новое имя класса.
                             \en New class name. \~
  \param[in]  oldClassName - \ru Старое имя класса.
                             \en Old class name. \~
  \param[in]  appIndex     - \ru Индекс приложения, которому принадлежит класс.
                             \en The index of the application that the class belongs to. \~
  \param[in]  lowVersion   - \ru Нижняя граница версии.
                             \en Version lower bound. \~
  \param[in]  highVersion  - \ru Верхняя граница версии.
                             \en Version upper bound. \~
  \ingroup Base_Tools_IO
*/ // ---
MATH_FUNC (void) AddPackedClassNameForVersion( const ClassDescriptor & newClassName, 
                                               const ClassDescriptor & oldClassName, 
                                                     uint              appIndex, 
                                                     VERSION           lowVersion, 
                                                     VERSION           highVersion );


//----------------------------------------------------------------------------------------
/** \brief \ru Диагностика коллизий имени нового класса с зарегистрированными классами.
           \en Diagnostics of collisions of new class name with registered classes. \~
  \details \ru Диагностика коллизий имени нового класса с зарегистрированными классами. \n
           \en Diagnostics of collisions of new class name with registered classes. \n \~
  \param[in]  className - \ru Имя класса. \en The class name. \~
  \param[in]  appID     - \ru Идентификатор приложения. \en The application id. \~
  \return \ru Признак уникальности класса с заданным именем при регистрации с указанным идентификатором приложения.
          \en Whether the class with the specified name is unique if registered with specified application id. \~
  \ingroup Base_Tools_IO
*/ // ---
MATH_FUNC( bool ) IsValidStreamClassName( const char * className, const MbUuid & appID );


#ifndef SIMPLENAME_AS_CLASS

//----------------------------------------------------------------------------------------
/// \ru Запись простого имени. \en Writing of a simple name.  \~ \ingroup Base_Tools_IO
// ---
inline void WriteSimpleName( writer & out, const SimpleName & s ) { out << s; }

//----------------------------------------------------------------------------------------
/// \ru Чтение простого имени. \en Reading of a simple name.  \~ \ingroup Base_Tools_IO
// ---
inline SimpleName ReadSimpleName( reader & in ) { SimpleName s = 0; in >> s; return s; }

#else // SIMPLENAME_AS_CLASS

//----------------------------------------------------------------------------------------
/// \ru Запись простого имени. \en Writing of a simple name.  \~ \ingroup Base_Tools_IO
// ---
inline
void WriteSimpleName( writer & out, const SimpleName & s )
{
  size_t sn = s;
  WriteCOUNT( out, sn );
}

//----------------------------------------------------------------------------------------
/// \ru Чтение простого имени. \en Reading of a simple name.  \~ \ingroup Base_Tools_IO
// ---
inline
SimpleName ReadSimpleName( reader & in )
{
  size_t sn = ReadCOUNT( in );
  return SimpleName(sn);
}

#endif // SIMPLENAME_AS_CLASS

namespace c3d // namespace C3D
{

//----------------------------------------------------------------------------------------
/// \ru Оператор записи хэша. \en Operator of writing hash.  \~ \ingroup Base_Tools_IO
//---
inline
writer & operator << ( writer & out, const StrHash & strHash )
{
  // \ru Нельзя допускать хеш с неопределенным типом и с определенным значением \en Hash with undefined type and with specified value cannot be allowed 
  PRECONDITION( !(IsGoodSimpleName(strHash.m_val) && strHash.m_type == c3d::StrHash::htp_undef) );

  WriteSimpleName( out, strHash.m_val );

  if ( out.MathVersion() >= 0x0A001005L )
    out << strHash.m_type;
  return out;
}

//----------------------------------------------------------------------------------------
/// \ru Оператор чтения хэша. \en Operator of hash reading.  \~ \ingroup Base_Tools_IO
//---
inline
reader & operator >> ( reader & in, c3d::StrHash & strHash )
{
  strHash.m_val = ReadSimpleName( in );

  if ( in.MathVersion() >= UNICODE_VERSION )
    if ( in.MathVersion() >= 0x0A001005L /*\ru введена запись типа хеша \en record of hash type is set */)
      in >> strHash.m_type;
    else
      strHash.m_type = c3d::StrHash::htp_wchar; // \ru !!! Переходный период, могут быть разные \en !!! Period of transition, may be different 
  else
    strHash.m_type = c3d::StrHash::htp_char;

  // \ru Нельзя допускать хеш с неопределенным типом и с определенным значением \en Hash with undefined type and with specified value cannot be allowed 
  PRECONDITION( !(IsGoodSimpleName(strHash.m_val) && strHash.m_type == c3d::StrHash::htp_undef) );

  return in;
}

} // namespace C3D


//----------------------------------------------------------------------------------------
/// \ru Запись строки в поток. \en Writing a string to the stream.  \~ \ingroup Base_Tools_IO
//---
inline
writer & operator << ( writer & ps, const std::string & s )
{
  if ( ps.MathVersion() < UNICODE_VERSION ) 
  {
#ifndef C3D_WINDOWS
    C3DLocaleSwitcher lh( FIX_ANSI1251 );
#endif
    // \ru пишем WCHAR* как CHAR* \en write WCHAR* as CHAR* 
    ps.__writeChar( s.c_str() );      // \ru пишем строку в формате ANSI \en write string in ANSI format 
  } else {
    wchar_t* buf = mbsnewwcs( s.c_str() );
    WriteWcharT( ps, buf );
    delete[] buf;
  }
  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Чтение строки из потока. \en Reading a string from the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
reader & operator >> ( reader & ps, std::string & s )
{
  if ( ps.MathVersion() < UNICODE_VERSION ) 
  {
#ifndef C3D_WINDOWS
    C3DLocaleSwitcher lh( FIX_ANSI1251 );
#endif
    char * str( nullptr );
    __readChar( ps, str );   // \ru читаем строку в формате ANSI \en read string in ANSI format 
    if ( str )
      s = str;
    else
      s.clear();
    delete [] str;
  } else {
    wchar_t * p (nullptr);
    ReadWcharT( ps, p ); // \ru в зависимости от версии потока \en subject to the stream version 
    if ( p ) {
      char* str = wcsnewmbs(p);    
      if ( str )
        s = str;
      else
        s.clear();
      delete[] str;
    }
    else
      s.clear();
    delete [] p;
  }
  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Запись строки в поток. \en Writing a string to the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
writer & operator << ( writer & ps, const std::wstring & s )
{
  if ( ps.MathVersion() < UNICODE_VERSION ) 
  {
#ifndef C3D_WINDOWS
    C3DLocaleSwitcher lh( FIX_ANSI1251 );
#endif
    // \ru пишем WCHAR* как CHAR* \en write WCHAR* as CHAR* 
    char * str = wcsnewmbs( s.c_str() );  // \ru создаем ANSI из TCHAR (если TCHAR == char, то просто дублируем) \en create ANSI from TCHAR (if TCHAR == char, then simply duplicate) 
    ps.__writeChar( str );      // \ru пишем строку в формате ANSI \en write string in ANSI format 
    delete [] str;
  }
  else
    WriteWcharT( ps, s.c_str() ); // \ru в зависимости от версии потока \en subject to the stream version 
  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Чтение строки из потока. \en Reading a string from the stream.  \~ \ingroup Base_Tools_IO
// ---
inline
reader & operator >> ( reader & ps, std::wstring & s )
{
  if ( ps.MathVersion() < UNICODE_VERSION ) 
  {
#ifndef C3D_WINDOWS
    C3DLocaleSwitcher lh( FIX_ANSI1251 );
#endif
    char * str( nullptr );
    __readChar( ps, str );   // \ru читаем строку в формате ANSI \en read string in ANSI format 
    wchar_t* p = mbsnewwcs( str );
    if ( p )
      s = p;
    else
      s.clear();
    delete [] p;
    delete [] str;
  } else {
    wchar_t * p = nullptr;
    ReadWcharT( ps, p ); // \ru в зависимости от версии потока \en subject to the stream version 
    if ( p )
      s = p;
    else
      s.clear();
    delete [] p;
  }
  return ps;
}


//----------------------------------------------------------------------------------------
/// \ru Запись строки в поток. \en Writing a string to the stream.  \~ \ingroup Base_Tools_IO
//---
inline
writer & operator << ( writer & ps, const std::wstring * s )
{
  WriteWcharT( ps, (s ? s->c_str() : nullptr) ); // \ru в зависимости от версии потока \en subject to the stream version
  return ps;
}

//----------------------------------------------------------------------------------------
/// \ru Прочитать кластер. \en Read the cluster.  \~ \ingroup Base_Tools_IO
// ---
inline
void ReadCluster( reader & in, uint16 clusterSize, Cluster & cl ) 
{
  // \ru очистить поле указателя, т.к. в AllocMem есть проверка на 0 \en clear the pointer field since there is a check for 0 in AllocMem 
  cl.SetClusterOffset( 0 ); 
  // \ru распределяем кластер стандартной длины ... \en allocate a cluster of a standard length ... 
  cl.AllocMem( clusterSize );

  uint16 length = 0;
  in >> length;
  PRECONDITION( length <= clusterSize );
  cl.m_l = length;

  // \ru ...а читаем - сколько было занято \en ...but read as much as has been occupied 
  in.readBytes( reinterpret_cast<uint8*>(cl.m_f), length );
}

//----------------------------------------------------------------------------------------
/// \ru Записать кластер, параметр clusterSize используется для проверки корректности длины кластера.
/// \en Write the cluster, the clusterSize parameter is used to checking the cluster length.
//  \~ \ingroup Base_Tools_IO
// ---
#ifdef C3D_DEBUG
inline
void WriteCluster( writer & out, const Cluster & cl, uint16 clusterSize )
#else
inline
void WriteCluster( writer & out, const Cluster & cl, uint16 /*clusterSize*/ )
#endif
{
  uint16 len = cl.m_l;
  PRECONDITION( len <= clusterSize );
  out << len;
  out.writeBytes( reinterpret_cast<const uint8*>(cl.m_f), len );
}

//----------------------------------------------------------------------------------------
/// \ru Записать информацию о кластере. \en Write the information about the cluster.  \~ \ingroup Base_Tools_IO
// ---
inline
size_t WriteClusterInfo( void * out, VERSION version, const Cluster & obj )
{
  WriteCOUNT( out, version, obj.m_f );

  uint16 len = obj.m_l; 
  ::memcpy( static_cast<uint8*>(out) + LenCOUNT(version), &len, sizeof( len ) );

  return Cluster::SizeOf( version );
}

//----------------------------------------------------------------------------------------
/// \ru Прочитать информацию о кластере. \en Read the information about the cluster.  \~ \ingroup Base_Tools_IO
// ---
inline
size_t ReadClusterInfo( void * in, VERSION version, Cluster & obj )
{
  size_t off = ReadCOUNT( in, version );

  uint16 len = 0; 
  ::memcpy( &len, static_cast<uint8*>(in) + LenCOUNT(version), sizeof( len ) );

  obj.AllocFile( off, len ); // \ru запомнить смещение в файле и кол-во байт \en memorize the shift in file and the number of bytes 

  return Cluster::SizeOf( version );
}

//------------------------------------------------------------------------------
// \ru Записать содержимое кластера. Возвращает размер данных кластера или -1, если длина кластера больше заявленной.
// \en Write the cluster's contents. Return size of cluster's data or -1, if the cluster length is greater than the defined one.
// ---
inline
size_t WriteClusterBody( void * out, VERSION version, const Cluster & obj, uint16 clusterSize )
{
  uint8 * m = static_cast<uint8 *>(out);

  // \ru кол-во заполненных байт в кластере \en the number of filled bytes in the cluster 
  uint16 l = obj._len();
  PRECONDITION( l <= clusterSize );
  *reinterpret_cast<uint16*>(m) = l;
  m += sizeof(uint16);

  if ( l > clusterSize )
    return static_cast<size_t>(-1);

  if ( l ) {
    memcpy( m, obj._ptr(), l ); // \ru теперь данные кластера \en the cluster data now 
                                // не используется далее m += l;
  }

  return getMemLen( obj, version ) ;
}

//------------------------------------------------------------------------------
// \ru Прочитать содержимое кластера. Возвращает размер данных кластера или -1, если длина кластера больше заявленной.
// \en Read the cluster's contents. Return size of cluster's data or -1, if the cluster length is greater than the defined one.
// ---
inline
size_t ReadClusterBody( void * in, VERSION version, Cluster & obj, uint16 clusterSize )
{
  uint8 * m = static_cast<uint8 *>(in);

  // \ru прочитать кол-во заполненных байт кластера и заполнить соответствующее поле в нем \en read the number of filled bytes in the cluster and fill its corresponding field 
  uint16 l = *reinterpret_cast<uint16*>(m);
  PRECONDITION( l <= clusterSize );
  obj.SetClusterLength( l );
  m += sizeof(uint16);

  if ( l > clusterSize )
    return static_cast<size_t>(-1);

  if ( l ) {
    memcpy( obj._getMemPointer(), m, l ); // \ru теперь данные кластера \en the cluster data now 
                                          // не используется далее m += l;
  }

  return getMemLen( obj, version );
}

//------------------------------------------------------------------------------
/**
  \brief \ru Стартовать try-catch регион для I/O операций. \en Start try-catch region for I/O operations. \~
  \ingroup Base_Tools
*/
//---
#define C3D_IO_CATCH_START  \
  try {

//------------------------------------------------------------------------------
/**
  \brief \ru Завершить try-catch регион для I/O операций (аргумент iostrm - поток чтения или записи).
         \en Complete try-catch region for I/O operations (argument iostrm - reading or writing stream). \~
  \ingroup Base_Tools
*/
//---
#define C3D_IO_CATCH_END(iostrm)        \
  }                                     \
  catch ( const std::bad_alloc & ) {    \
    iostrm.setState( io::outOfMemory ); \
  }                                     \
  catch ( ... ) {                       \
    iostrm.setState( io::fail );        \
  }


//------------------------------------------------------------------------------
// \ru Реализация функций записи по неконстантной ссылке/указателю (объявленных в KNOWN_OBJECTS_RW_REF_OPERATORS_BASE, KNOWN_OBJECTS_RW_PTR_OPERATORS_BASE)
//     для объектов, для которых при записи и чтении точно известен тип.
// \en Implementation of writing functions by non-constant reference/pointer (declared in KNOWN_OBJECTS_RW_REF_OPERATORS_BASE, KNOWN_OBJECTS_RW_PTR_OPERATORS_BASE)
//     for objects which type is exactly known while reading and writing.
//---
#define IMP_KNOWN_OBJECTS_RW_REF_OPERATORS(Class)                                        \
  writer & CALL_DECLARATION operator << ( writer & out,       Class & ref ) { return operator << ( out, static_cast<const Class &>(ref) ); }

#define IMP_KNOWN_OBJECTS_RW_PTR_OPERATORS(Class)                                        \
  writer & CALL_DECLARATION operator << ( writer & out,       Class * ptr ) { return operator << ( out, static_cast<const Class *>(ptr) ); }

//------------------------------------------------------------------------------
// \ru Реализация функций записи по неконстантной ссылке/указателю (объявленных в KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE, KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE)
//     для объектов, для которых при записи и чтении точно известен тип.
// \en Implementation of writing functions by non-constant reference/pointer (declared in KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE, KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE)
//     for objects which type is exactly known while reading and writing.
//---
#define IMP_KNOWN_OBJECTS_RW_REF_OPERATORS_EX(Class, DLLFUNC)                                    \
  DLLFUNC writer & CALL_DECLARATION operator << ( writer & out,       Class & ref ) { return operator << ( out, static_cast<const Class &>(ref) ); }

#define IMP_KNOWN_OBJECTS_RW_PTR_OPERATORS_EX(Class, DLLFUNC)                                    \
  DLLFUNC writer & CALL_DECLARATION operator << ( writer & out,       Class * ptr ) { return operator << ( out, static_cast<const Class *>(ptr) ); }


//----------------------------------------------------------------------------------------
/**
  \brief \ru Реализация дружественных операторов чтения и записи указателей и ссылок, объявленных в макросе DECLARE_PERSISTENT_OPS_BASE.
         \en Implementation of friend operators of reading and writing of pointers and references declared in DECLARE_PERSISTENT_OPS_BASE macro. \~
  \ingroup Base_Tools_IO
*/
// ---
#define IMP_PERSISTENT_OPS_BASE( Class, DLLFUNC )                                              \
  DLLFUNC reader & CALL_DECLARATION operator >> ( reader & in, Class & ref ) {        \
    in.readObject( dynamic_cast<TapeBase *>(&ref) );                                  \
    return in;                                                                        \
  }                                                                                   \
  DLLFUNC reader & CALL_DECLARATION operator >> ( reader & in, Class *& ptr ) {       \
    ptr = dynamic_cast<Class *>( in.readObjectPointer() );                            \
    return in;                                                                        \
  }                                                                                   \
  DLLFUNC reader & CALL_DECLARATION operator >> ( reader & in, const Class *& ptr )   \
  {                                                                                   \
    ptr = dynamic_cast<Class *>( in.readObjectPointer() );                            \
    return in;                                                                        \
  }                                                                                   \
  DLLFUNC writer & CALL_DECLARATION operator << ( writer & out, const Class & ref ) { \
    out.writeObject( dynamic_cast<const TapeBase *>(&ref) );                          \
    return out;                                                                       \
  }                                                                                   \
  DLLFUNC writer & CALL_DECLARATION operator << ( writer & out, const Class * ptr ) { \
    out.writeObjectPointer( dynamic_cast<const TapeBase*>(ptr) );                     \
    return out;                                                                       \
  }                                                                                   \
  DLLFUNC writer & CALL_DECLARATION operator << ( writer & out, Class & ref ) {       \
    out.writeObject( dynamic_cast<TapeBase *>(&ref) );                                \
    return out;                                                                       \
  }                                                                                   \
  DLLFUNC writer & CALL_DECLARATION operator << ( writer& out, Class * ptr ) {        \
    out.writeObjectPointer( dynamic_cast<TapeBase *>(ptr) );                          \
    return out;                                                                       \
  }

//------------------------------------------------------------------------------
/// \ru Функции получения дескриптора (хэш + APP UID) класса. \~ \ingroup Base_Tools_IO
/// \ru Реализация макроса DECLARE_CLASS_DESC_FUNC. \en Implementation of DECLARE_CLASS_DESC_FUNC marco.
// ---
#define IMP_CLASS_DESC_FUNC( AppID, Class )                                   \
  ClassDescriptor Class::GetClassDescriptor( const VersionContainer & v) const \
    { return ClassDescriptor( GetPureName(v), AppID ); }


//----------------------------------------------------------------------------------------
/** \brief \ru Конструирование нового экземпляра класса.
           \en Construction of a new instance of the class. \~
  \details \ru Конструирование нового экземпляра класса. \n
    Определяются функция конструирования нового экземпляра класса,
    функция преобразования от указателя на TapeBase к указателю на класс
    и класс (не экземпляр!) добавляется в массив потоковых
    путем создания переменной r ## Class типа TapeClass
    (а в конструкторе TapeClass производится
    добавление в массив потоковых классов).
    Символ ## - это указание препроцессору о необходимости "склейки"
    текущего идентификатора с последующим.
           \en Construction of a new instance of the class. \n
    Definition of functions of construction a new instance of the class,
    function of conversion from a pointer to TapeBase to a pointer to the class
    and addition of the class (not an instance) to the array of stream classes
    by creating variable r ## Class of type TapeClass
    (and in constructor of TapeClass
    addition to array of stream classes is performed).
    Symbol ## is a directive for preprocessor about the necessity of "gluing"
    of the current identifier with the next one. \~
  \ingroup Base_Tools_IO
*/
// ---
#define IMP_PERSISTENT_REGISTRATION( AppID, Class )                     \
  TapeBase * CALL_DECLARATION make ## _ ## Class () {                   \
    return new Class(tapeInit);                                         \
  }                                                                     \
  void * CALL_DECLARATION cast ## _ ## Class ( const TapeBase * obj ) { \
    return dynamic_cast<Class *>(const_cast<TapeBase *>(obj) );         \
  }                                                                     \
                                                                        \
  TapeClass  r ## Class(                                                \
    typeid(Class).name(),                                               \
    AppID,                                                              \
    (BUILD_FUNC) make ## _ ## Class,                                    \
    (CAST_FUNC ) cast ## _ ## Class,                                    \
    (READ_FUNC ) Class::Read,                                           \
    (WRITE_FUNC) Class::Write                                           \
  )


//------------------------------------------------------------------------------
// \ru Как записать переименованный класс в старую версию (с) Столяров А.Г. \en How to write the renamed class to the old version (c) Stolyarov A.G. 
/* #define IMP_PERSISTENT_REGISTRATION_OLDCLASS( Class, OldClass )         \
  TapeBase * CALL_DECLARATION make ## _ ## Class () {                   \
    return dynamic_cast<TapeBase *>( new Class(tapeInit) );             \
  }                                                                     \
  void * CALL_DECLARATION cast ## _ ## Class ( const TapeBase * obj ) { \
    return dynamic_cast<Class *>(const_cast<TapeBase *>(obj) );         \
  }                                                                     \
  TapeClass  r ## Class(                                                \
    typeid(Class).name(),                                               \
    typeid(OldClass).name(),                                            \
    (BUILD_FUNC) make ## _ ## Class,                                    \
    (CAST_FUNC ) cast ## _ ## Class,                                    \
    (READ_FUNC ) Class::Read,                                           \
    (WRITE_FUNC) Class::Write                                           \
  )

#define IMP_PERSISTENT_OLDCLASS( Class, OldClass )         \
  IMP_PERSISTENT_REGISTRATION_OLDCLASS( Class, OldClass ); \
  IMP_PERSISTENT_NEW_DELETE_CLASS( Class )

IMP_PERSISTENT_OLDCLASS( Class, OldClass );

class TapeClassForNewObjects : public TapeClass {
protected :
  ClassDescriptor hashValueOld;  // \ru упакованное имя класса для старой версии файла \en packed class name for the old version of file
public :
  TapeClassForNewObjects( const char * name, const char * oldName, BUILD_FUNC b, CAST_FUNC c, READ_FUNC r, WRITE_FUNC w );
  virtual ~TapeClassForNewObjects();
  virtual ClassDescriptor GetPackedClassNameForWrite( long version ) const;
  OBVIOUS_PRIVATE_COPY(TapeClassForNewObjects);
};

TapeClassForNewObjects::TapeClassForNewObjects( const char * name, const char * oldName,
                                                BUILD_FUNC b, CAST_FUNC c, READ_FUNC r, WRITE_FUNC w )
  : TapeClass( name, b, c, r, w )
  , hashValueOld( ::hash(::pureName( oldName ) ) )
{
}

ClassDescriptor TapeClassForNewObjects::GetPackedClassNameForWrite( long version ) const {
  uint16 res = version > CHANGE_VERSION ? TapeClass::GetPackedClassName() : uint16(hashValueOld);
  return res;
}
*/


#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_

//--------------------------------------------------------------------------------------
/// \ru Реализация функций new, delete и операторов доступа (объявленных в DECLARE_NEW_DELETE_CLASS).
/// \en Implementation of functions new, delete and access operators (declared in DECLARE_NEW_DELETE_CLASS).
/// \~ \ingroup Base_Tools_IO
// ---
#define IMP_PERSISTENT_NEW_DELETE_CLASS( Class )

//--------------------------------------------------------------------------------------
/// \ru Реализация операторов new и delete, обеспечивающих последовательное обращение
///  к функциям выделения/освобождения памяти из разных потоков (объявленных в DECLARE_NEW_DELETE_CLASS_EX).
/// \en Implementation of new and delete operators which provide sequential access
/// to the allocation/deallocation functions from different threads (declared in DECLARE_NEW_DELETE_CLASS_EX).  \~
/// \ingroup Base_Tools_IO
// ---
#define IMP_PERSISTENT_NEW_DELETE_CLASS_EX( Class )            \
    void * Class::operator new( size_t size ) {                  \
      return ::Allocate( size, typeid(Class).name() ); }         \
    void Class::operator delete ( void *ptr, size_t size ) {     \
      ::Free( ptr, size, typeid(Class).name() ); }               \
                                                                 \
    void * Class::operator new[] ( size_t size ) {               \
      return ::AllocateArray( size, typeid(Class[]).name()); }   \
    void Class::operator delete[] ( void *ptr ) {                \
      ::FreeArray( ptr, typeid(Class[]).name() ); }              

#else // __DEBUG_MEMORY_ALLOCATE_FREE_

//--------------------------------------------------------------------------------------
/// \ru Реализация функций new, delete и операторов доступа (объявленных в DECLARE_NEW_DELETE_CLASS).
/// \en Implementation of functions new, delete and access operators (declared в DECLARE_NEW_DELETE_CLASS).
/// \~ \ingroup Base_Tools_IO
// ---
#define IMP_PERSISTENT_NEW_DELETE_CLASS( Class )

#if defined(__OVERLOAD_MEMORY_ALLOCATE_FREE_) && !defined(C3D_DEBUG)

  //--------------------------------------------------------------------------------------
  /// \ru Реализация операторов new и delete, обеспечивающая последовательное обращение
  ///  к функциям выделения/освобождения памяти из разных потоков (объявленных в DECLARE_NEW_DELETE_CLASS_EX).
  /// Перегружаются все стандартные операторы new и delete.
  /// \en Implementation of new and delete operators which provides sequential access
  /// to the allocation/deallocation functions from different threads (declared in DECLARE_NEW_DELETE_CLASS_EX).
  /// All standard new and delete operators are overloaded. \~
  /// \ingroup Base_Tools_IO
  // ---
#define IMP_PERSISTENT_NEW_DELETE_CLASS_EX( Class )          \
    void* Class::operator new( size_t size ) {                 \
      SET_MEMORY_SCOPED_LOCK;                                  \
      return ::operator new( size ); }                         \
    void Class::operator delete( void *ptr ) {                 \
      SET_MEMORY_SCOPED_LOCK;                                  \
      ::operator delete( ptr ); }                              \
                                                               \
    void* Class::operator new( size_t size, void *ptr ) {      \
      SET_MEMORY_SCOPED_LOCK;                                  \
      return ::operator new( size, ptr ); }                    \
    void Class::operator delete( void *ptr, void *ptr2 ) {     \
      SET_MEMORY_SCOPED_LOCK;                                  \
      ::operator delete( ptr, ptr2 ); }                        \
                                                               \
    void* Class::operator new[]( size_t size ) {               \
      SET_MEMORY_SCOPED_LOCK;                                  \
      return ::operator new[]( size ); }                       \
    void Class::operator delete[]( void *ptr ) {               \
      SET_MEMORY_SCOPED_LOCK;                                  \
      ::operator delete[]( ptr ); }                            \
                                                               \
    void* Class::operator new []( size_t size, void *ptr ) {   \
      SET_MEMORY_SCOPED_LOCK;                                  \
      return ::operator new[]( size, ptr ); }                  \
    void Class::operator delete []( void *ptr, void *ptr2 ) {  \
      SET_MEMORY_SCOPED_LOCK;                                  \
      ::operator delete[]( ptr, ptr2 ); }                      \
                                                               \
    void* Class::operator new( size_t size, const std::nothrow_t &nt ) throw() {    \
      SET_MEMORY_SCOPED_LOCK;                                                       \
      return ::operator new( size, nt ); }                                          \
    void Class::operator delete( void *ptr, const std::nothrow_t &nt ) throw() {    \
      SET_MEMORY_SCOPED_LOCK;                                                       \
      ::operator delete( ptr, nt ); }                                               \
                                                                                    \
    void* Class::operator new []( size_t size, const std::nothrow_t &nt ) throw() { \
      SET_MEMORY_SCOPED_LOCK;                                                       \
      return ::operator new[]( size, nt ); }                                        \
    void Class::operator delete []( void *ptr, const std::nothrow_t &nt ) throw() { \
      SET_MEMORY_SCOPED_LOCK;                                                       \
      ::operator delete[]( ptr, nt ); }

#else // __OVERLOAD_MEMORY_ALLOCATE_FREE_

  //--------------------------------------------------------------------------------------
  /// \ru Реализация операторов new и delete, обеспечивающая последовательное обращение
  ///  к функциям выделения/освобождения памяти из разных потоков (объявленных в DECLARE_NEW_DELETE_CLASS_EX).
  /// \en Implementation of new and delete operators which provides sequential access
  /// to the allocation/deallocation functions from different threads (declared in DECLARE_NEW_DELETE_CLASS_EX).  \~
  /// \ingroup Base_Tools_IO
  // ---
#define IMP_PERSISTENT_NEW_DELETE_CLASS_EX( Class )

#endif // __OVERLOAD_MEMORY_ALLOCATE_FREE_
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_


//------------------------------------------------------------------------------
/** \brief \ru Реализация объявления DECLARE_PERSISTENT_CLASS_BASE.
           \en Implementation of DECLARE_PERSISTENT_CLASS_BASE declaration. \~
  \details \ru Реализация объявления DECLARE_PERSISTENT_CLASS_BASE.
    Описывает необходимые действия для поточного класса.
    Устанавливается в любой .cpp файл.
    Class должен наследовать от TapeBase.
    Должны быть реализованы функции чтения Read и записи Write. \n
           \en Implementation of DECLARE_PERSISTENT_CLASS_BASE declaration.
    Describes the necessary operations for a stream class.
    It is set into any .cpp file.
    Class must be inherited from TapeBase.
    Function Read of reading and function Write of writing should be implemented. \n \~
  \ingroup Base_Tools_IO
*/
// ---
#define IMP_PERSISTENT_CLASS_OPS( AppID, Class, DLLFUNC ) \
  IMP_PERSISTENT_REGISTRATION( AppID, Class );            \
  IMP_PERSISTENT_NEW_DELETE_CLASS( Class );               \
  IMP_PERSISTENT_OPS_BASE( Class, DLLFUNC );              \
  IMP_CLASS_DESC_FUNC( AppID, Class )


//------------------------------------------------------------------------------
/** \brief \ru Реализация объявления DECLARE_PERSISTENT_CLASS_BASE для абстрактного поточного класса.
           \en Implementation of DECLARE_PERSISTENT_CLASS_BASE declaration for an abstract stream class. \~
  \details \ru Реализация объявления DECLARE_PERSISTENT_CLASS_BASE для абстрактного поточного класса.
    Описывает необходимые действия для поточного класса.
    Устанавливается в любой .cpp файл.
    Class должен наследовать от TapeBase.
    Должны быть реализованы функции чтения Read и записи Write. \n
           \en Implementation of DECLARE_PERSISTENT_CLASS_BASE declaration for an abstract stream class.
    Describes the necessary operations for a stream class.
    It is set into any .cpp file.
    Class must be inherited from TapeBase.
    Function Read of reading and function Write of writing should be implemented. \n \~
  \ingroup Base_Tools_IO
*/
// ---
#define IMP_A_PERSISTENT_CLASS_OPS( AppID, Class, DLLFUNC ) \
  IMP_PERSISTENT_NEW_DELETE_CLASS( Class );               \
  IMP_PERSISTENT_OPS_BASE( Class, DLLFUNC );              \
  IMP_CLASS_DESC_FUNC( AppID, Class )


//------------------------------------------------------------------------------
/** \brief \ru Реализация объявления DECLARE_PERSISTENT_CLASS_NEW_DEL_BASE.
           \en Implementation of DECLARE_PERSISTENT_CLASS_NEW_DEL_BASE declaration. \~
  \details \ru Реализация объявления DECLARE_PERSISTENT_CLASS_NEW_DEL_BASE.
    Описывает необходимые действия для поточного класса.
    Устанавливается в любой .cpp файл.
    Class должен наследовать от TapeBase.
    Должны быть реализованы функции чтения Read и записи Write. \n
           \en Implementation of DECLARE_PERSISTENT_CLASS_NEW_DEL_BASE declaration.
    Describes the necessary operations for a stream class.
    It is set into any .cpp file.
    Class must be inherited from TapeBase.
    Function Read of reading and function Write of writing should be implemented. \n \~
  \ingroup Base_Tools_IO
*/
// ---
#define IMP_PERSISTENT_CLASS_NEW_DEL_OPS( AppID, Class, DLLFUNC ) \
  IMP_PERSISTENT_CLASS_OPS( AppID, Class, DLLFUNC );            \
  IMP_PERSISTENT_NEW_DELETE_CLASS_EX( Class );


//----------------------------------------------------------------------------------------
// \ru Описывает необходимые операции для абстрактного поточного класса, \en Describes the necessary operations for the abstract stream class 
// \ru не наследующего ни от кого кроме TapeBase, и у которого \en which is not inherited from any class except TapeBase and which 
// \ru нет своих полей данных для записи в поток \en has no its own data fields for writing to stream 
// \ru Устанавливается в любой .cpp файл \en It is set to any .cpp file 
// \ru Примечание : \en Note: 
// \ru 1. Class должен наследовать от TapeBase \en 1. Class must be inherited from TapeBase 
// \ru 2. Требуется (!!!) отсутствие (!!!) функций \en 2. The following functions must be (!!!) absent (!!!) 
//      - void Class::Read( reader& in, Class* obj );
//      - void Class::Write( writer& out, const Class* obj );
// \ru где Class - имя класса \en where Class is a class name 
// ---
#define IMP_A_PERSISTENT_CLASS_WD( AppID, Class )   \
  void Class::Read( reader &, Class * )         {}  \
  void Class::Write( writer &, const Class * )  {}  \
  IMP_PERSISTENT_NEW_DELETE_CLASS( Class )          \
  IMP_CLASS_DESC_FUNC( AppID, Class )


//----------------------------------------------------------------------------------------
// \ru Описывает необходимые операции для абстрактного поточного класса с проверкой главной версии потока (версии математического ядра),
// \en Describes the necessary operations for the abstract stream class with check of the main stream version (the mathenatical kernel version),
// \ru не наследующего ни от кого кроме TapeBase, и у которого \en which is not inherited from any class except TapeBase and which 
// \ru нет своих полей данных для записи в поток. \en has no its own data fields for writing to stream. 
// \ru Устанавливается в любой .cpp файл \en It is set to any .cpp file 
// \ru Примечание : \en Note: 
// \ru 1. Class должен наследовать от TapeBase \en 1. Class must be inherited from TapeBase 
// \ru 2. Требуется (!!!) отсутствие (!!!) функций \en 2. The following functions must be (!!!) absent (!!!) 
//      - void Class::Read( reader& in, Class* obj );
//      - void Class::Write( writer& out, const Class* obj );
// \ru где Class - имя класса \en where Class is a class name 
// ---
#define IMP_A_PERSISTENT_MATH_CLASS_WD( AppID, Class )   \
  void Class::Read( reader &, Class * )         {}  \
  void Class::Write( writer & out, const Class * )  \
  { if ( out.MathVersion() < wrv_FirstRelease ) out.setState( io::cantWriteObject ); }  \
  IMP_PERSISTENT_NEW_DELETE_CLASS( Class )          \
  IMP_CLASS_DESC_FUNC( AppID, Class )

//----------------------------------------------------------------------------------------
// \ru Описывает необходимые операции для абстрактного \en Describes the necessary operations for abstract 
// \ru поточного класса \en stream class 
// \ru Устанавливается в любой .cpp файл \en It is set to any .cpp file 
// \ru Примечание : \en Note: 
// \ru 1. Class должен наследовать от TapeBase \en 1. Class must be inherited from TapeBase 
// \ru 2. Требуется наличие функций \en 2. There must be the following functions 
//      - void Class::Read( reader& in, Class* obj );
//      - void Class::Write( writer& out, const Class* obj );
// \ru где Class - имя класса \en where Class is a class name 
// ---
#define IMP_A_PERSISTENT_CLASS( AppID, Class ) \
  IMP_PERSISTENT_NEW_DELETE_CLASS( Class )     \
  IMP_CLASS_DESC_FUNC( AppID, Class )

//----------------------------------------------------------------------------------------
// \ru Описывает необходимые операции абстрактного поточного класса, \en Describes the necessary operations for the abstract stream class 
// \ru наследующего от другого такого же, и у которого \en inherited from another class which is the same and which 
// \ru нет своих полей данных для записи в поток \en has no its own data fields for writing to stream 
// \ru Устанавливается в любой .cpp файл \en It is set to any .cpp file 
// \ru Примечание : \en Note: 
// \ru 1. Class должен наследовать от TapeBase \en 1. Class must be inherited from TapeBase 
// \ru 2. Требуется (!!!) отсутствие (!!!) функций \en 2. The following functions must be (!!!) absent (!!!) 
//      - void Class::Read( reader& in, Class* obj );
//      - void Class::Write( writer& out, const Class* obj );
// \ru где Class - имя класса \en where Class is a class name 
// \ru эти функции генерируются автоматически \en these functions are generated automatically 
// ---
#define IMP_A_PERSISTENT_CLASS_FROM_BASE( AppID, Class, Base )   \
  void Class::Read( reader & in, Class * obj ) {                 \
    Base::Read( in, obj );                                       \
  }                                                              \
  void Class::Write( writer & out, const Class * obj ) {         \
    Base::Write( out, obj );                                     \
  }                                                              \
  IMP_PERSISTENT_NEW_DELETE_CLASS( Class )                       \
  IMP_CLASS_DESC_FUNC( AppID, Class )

//----------------------------------------------------------------------------------------
// \ru Описывает необходимые операции поточного класса, \en Describes the necessary operations of the stream class 
// \ru наследующего от другого такого же, и у которого \en inherited from another class which is the same and which 
// \ru нет своих полей данных для записи в поток \en has no its own data fields for writing to stream 
// \ru Устанавливается в любой .cpp файл \en It is set to any .cpp file 
// \ru Примечание : \en Note: 
// \ru 1. Class должен наследовать от TapeBase \en 1. Class must be inherited from TapeBase 
// \ru 2. Требуется (!!!) отсутствие (!!!) функций \en 2. The following functions must be (!!!) absent (!!!) 
//      - void Class::Read( reader& in, Class* obj );
//      - void Class::Write( writer& out, const Class* obj );
// \ru где Class - имя класса \en where Class is a class name 
// \ru эти функции генерируются автоматически \en these functions are generated automatically 
// ---
#define IMP_PERSISTENT_CLASS_FROM_BASE( AppID, Class, Base ) \
  IMP_PERSISTENT_REGISTRATION( AppID, Class );               \
  IMP_A_PERSISTENT_CLASS_FROM_BASE( AppID, Class, Base )

//------------------------------------------------------------------------------
/** \brief \ru Аналог макроса IMP_PERSISTENT_CLASS_FROM_BASE
           с возможностью перегрузки операторов new/delete,
           обеспечивающий последовательное обращение к функциям
           выделения/освобождения памяти из разных потоков.
           \en Analog of IMP_PERSISTENT_CLASS_FROM_BASE macro
           with support of new/delete operators overloading which provides
           sequential access to the allocation/deallocation functions
           from different threads. \~
  \details \ru Аналог макроса IMP_PERSISTENT_CLASS_FROM_BASE
           с возможностью перегрузки операторов new/delete,
           обеспечивающий последовательное обращение к функциям
           выделения/освобождения памяти из разных потоков
           (включается переменной __OVERLOAD_MEMORY_ALLOCATE_FREE_).
           \en Analog of IMP_PERSISTENT_CLASS_FROM_BASE macro
           with support of new/delete operators overloading which provides
           sequential access to the allocation/deallocation functions
           from different threads
           (enabled by defining __OVERLOAD_MEMORY_ALLOCATE_FREE_). \~
  \ingroup Base_Tools_IO
*/
// ---
#define IMP_PERSISTENT_CLASS_FROM_BASE_NEW_DEL( AppID, Class, Base ) \
  IMP_PERSISTENT_CLASS_FROM_BASE( AppID, Class, Base )             \
  IMP_PERSISTENT_NEW_DELETE_CLASS_EX( Class )

//----------------------------------------------------------------------------------------
// \ru Описывает необходимые операции для поточного класса, \en Describes the necessary operations for the stream class 
// \ru не наследующего ни от кого кроме TapeBase, и у которого \en which is not inherited from any class except TapeBase and which 
// \ru нет своих полей данных для записи в поток \en has no its own data fields for writing to stream 
// \ru Устанавливается в любой .cpp файл \en It is set to any .cpp file 
// \ru Примечание : \en Note: 
// \ru 1. Class должен наследовать от TapeBase \en 1. Class must be inherited from TapeBase 
// \ru 2. Требуется (!!!) отсутствие (!!!) функций \en 2. The following functions must be (!!!) absent (!!!) 
//      - void Class::Read( reader& in, Class* obj );
//      - void Class::Write( writer& out, const Class* obj );
// \ru где Class - имя класса \en where Class is a class name 
// ---
#define IMP_PERSISTENT_CLASS_WD( AppID, Class )     \
  IMP_PERSISTENT_REGISTRATION( AppID, Class );      \
  void Class::Read( reader &, Class * )         {}  \
  void Class::Write( writer &, const Class * )  {}  \
  IMP_PERSISTENT_NEW_DELETE_CLASS( Class )          \
  IMP_CLASS_DESC_FUNC( AppID, Class )

//----------------------------------------------------------------------------------------
/** \brief \ru Аналог макроса IMP_PERSISTENT_CLASS_WD
           с возможностью перегрузки операторов new/delete,
           обеспечивающий последовательное обращение к функциям
           выделения/освобождения памяти из разных потоков.
           \en Analog of IMP_PERSISTENT_CLASS_WD macro
           with support of new/delete operators overloading which provides
           sequential access to the allocation/deallocation functions
           from different threads. \~
  \details \ru Аналог макроса IMP_PERSISTENT_CLASS_WD
           с возможностью перегрузки операторов new/delete,
           обеспечивающий последовательное обращение к функциям
           выделения/освобождения памяти из разных потоков
           (включается переменной __OVERLOAD_MEMORY_ALLOCATE_FREE_).
           \en Analog of IMP_PERSISTENT_CLASS_WD macro
           with support of new/delete operators overloading which provides
           sequential access to the allocation/deallocation functions
           from different threads
           (enabled by defining __OVERLOAD_MEMORY_ALLOCATE_FREE_). \~
  \ingroup Base_Tools_IO
*/
// ---
#define IMP_PERSISTENT_CLASS_WD_NEW_DEL( AppID, Class ) \
  IMP_PERSISTENT_CLASS_WD( AppID, Class );              \
  IMP_PERSISTENT_NEW_DELETE_CLASS_EX( Class )


//------------------------------------------------------------------------------
/** \brief \ru Реализация объявления DECLARE_PERSISTENT_CLASS.
           \en Implementation of DECLARE_PERSISTENT_CLASS declaration. \~
  \details \ru Реализация объявления DECLARE_PERSISTENT_CLASS.
    Описывает необходимые действия для поточного класса.
    Устанавливается в любой .cpp файл.
    Class должен наследовать от TapeBase.
    Должны быть реализованы функции чтения Read и записи Write. \n
           \en Implementation of DECLARE_PERSISTENT_CLASS declaration.
    Describes the necessary operations for a stream class.
    It is set into any .cpp file.
    Class must be inherited from TapeBase.
    Function Read of reading and function Write of writing should be implemented. \n \~
  \ingroup Base_Tools_IO
*/
// ---
#define IMP_PERSISTENT_CLASS( AppID, Class )   \
  IMP_PERSISTENT_REGISTRATION( AppID, Class ); \
  IMP_PERSISTENT_NEW_DELETE_CLASS( Class );    \
  IMP_CLASS_DESC_FUNC( AppID, Class )

/** \brief \ru Аналог макроса IMP_PERSISTENT_CLASS
           с возможностью перегрузки операторов new/delete,
           обеспечивающий последовательное обращение к функциям
           выделения/освобождения памяти из разных потоков.
           \en Analog of IMP_PERSISTENT_CLASS macro
           with support of new/delete operators overloading which provides
           sequential access to the allocation/deallocation functions
           from different threads. \~
  \details \ru Аналог макроса IMP_PERSISTENT_CLASS
           с возможностью перегрузки операторов new/delete,
           обеспечивающий последовательное обращение к функциям
           выделения/освобождения памяти из разных потоков
           (включается переменной __OVERLOAD_MEMORY_ALLOCATE_FREE_).
           \en Analog of IMP_PERSISTENT_CLASS macro
           with support of new/delete operators overloading which provides
           sequential access to the allocation/deallocation functions
           from different threads
           (enabled by defining __OVERLOAD_MEMORY_ALLOCATE_FREE_). \~
  \ingroup Base_Tools_IO
*/
// ---
#define IMP_PERSISTENT_CLASS_NEW_DEL( AppID, Class ) \
  IMP_PERSISTENT_CLASS( AppID, Class );              \
  IMP_PERSISTENT_NEW_DELETE_CLASS_EX( Class );


//----------------------------------------------------------------------------------------
/// \ru Конструирование нового экземпляра класса для класса без записи. \en Construction of a new instance of the class for a class without writing. \~ \ingroup Base_Tools_IO
// ---
#define IMP_PERSISTENT_RO_REGISTRATION( AppID, Class )                    \
  TapeBase * CALL_DECLARATION make ## _ ## Class () {                     \
    return new Class(tapeInit);                                           \
  }                                                                       \
  void * CALL_DECLARATION cast ## _ ## Class ( const TapeBase * obj ) {   \
    return dynamic_cast<Class*>(const_cast<TapeBase *>(obj) );            \
  }                                                                       \
  TapeClass  r ## Class(                                                  \
    typeid(Class).name(),                                                 \
    AppID,                                                                \
    (BUILD_FUNC) make ## _ ## Class,                                      \
    (CAST_FUNC ) cast ## _ ## Class,                                      \
    (READ_FUNC ) Class::Read,                                             \
    (WRITE_FUNC) 0                                                        \
  )


//----------------------------------------------------------------------------------------
// \ru Описывает необходимые действия для поточного класса без записи \en Describes the necessary operations for a stream class without writing. 
// \ru Устанавливается в любой .cpp файл \en It is set to any .cpp file 
// \ru Примечание : \en Note: 
// \ru 1. Class должен наследовать от TapeBase \en 1. Class must be inherited from TapeBase 
// \ru 2. Требуется наличие функций \en 2. There must be the following functions 
//      - void Class:Read( reader& in, Class* obj );
//      - void Class::Write( writer& out, const Class* obj );
// \ru где Class - имя класса \en where Class is a class name 
// ---
#define IMP_PERSISTENT_RO_CLASS( AppID, Class )      \
  IMP_PERSISTENT_RO_REGISTRATION( AppID, Class );    \
  IMP_PERSISTENT_NEW_DELETE_CLASS( Class )

//------------------------------------------------------------------------------
/** \brief \ru Аналог макроса IMP_PERSISTENT_RO_CLASS
           с возможностью перегрузки операторов new/delete,
           обеспечивающий последовательное обращение к функциям
           выделения/освобождения памяти из разных потоков.
           \en Analog of IMP_PERSISTENT_RO_CLASS macro
           with support of new/delete operators overloading which provides
           sequential access to the allocation/deallocation functions
           from different threads. \~
  \details \ru Аналог макроса IMP_PERSISTENT_RO_CLASS
           с возможностью перегрузки операторов new/delete,
           обеспечивающий последовательное обращение к функциям
           выделения/освобождения памяти из разных потоков
           (включается переменной __OVERLOAD_MEMORY_ALLOCATE_FREE_).
           \en Analog of IMP_PERSISTENT_RO_CLASS macro
           with support of new/delete operators overloading which provides
           sequential access to the allocation/deallocation functions
           from different threads
           (enabled by defining __OVERLOAD_MEMORY_ALLOCATE_FREE_). \~
  \ingroup Base_Tools_IO
*/
// ---
#define IMP_PERSISTENT_RO_CLASS_NEW_DEL( AppID, Class ) \
  IMP_PERSISTENT_RO_CLASS( AppID, Class );              \
  IMP_PERSISTENT_NEW_DELETE_CLASS_EX( Class )


//----------------------------------------------------------------------------------------
/// \ru Конструктор для потокового класса. \en Constructor for a stream class. \~ \ingroup Base_Tools_IO
// ---
#define IMP_PERSISTENT_CTOR( Class )  \
  Class::Class( TapeInit ) {}

//----------------------------------------------------------------------------------------
/// \ru Конструктор для класса с одной потоковой базой. \en Constructor for a class with one stream base. \~ \ingroup Base_Tools_IO
// ---
#define IMP_PERSISTENT_CTOR1( Class, Base )       \
  Class::Class( TapeInit ) : Base( tapeInit ) {}

//----------------------------------------------------------------------------------------
/// \ru Конструктор для класса с двумя потоковыми базами. \en Constructor for a class with two stream bases. \~ \ingroup Base_Tools_IO
// ---
#define IMP_PERSISTENT_CTOR2( Class, Base1, Base2 )                   \
  Class::Class( TapeInit ) : Base1( tapeInit ), Base2( tapeInit ) {}


#endif // __IO_TAPE_H

