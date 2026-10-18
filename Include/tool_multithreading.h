////////////////////////////////////////////////////////////////////////////////
/**
\file
\brief \ru Управление параллельной обработкой данных.
       \en Managing of parallel data processing. \~
\details \ru Управление параллельной обработкой данных.\n
        \en Managing of parallel data processing. \n \~
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __TOOL_MULTITHREADING_H
#define __TOOL_MULTITHREADING_H

#include <system_dependency.h>
#include <tool_mutex.h>
#include <tool_err_handling.h>

//------------------------------------------------------------------------------
/**
  \brief \ru Базовый класс для объектов, требующих сборки мусора.
         \en Base class for objects which require a garbage collection. \~
\details \ru Базовый класс для объектов, требующих сборки мусора.
         Класс, наследующий от CacheCleaner, должен реализовать метод ResetCacheData,
         который будет вызываться для сборки мусора.
         \en Base class for objects which require a garbage collection.
         A class, inheriting from CacheCleaner, should implement the method ResetCacheData,
         which will be called for garbage collection. \~
\ingroup Base_Tools
*/
//---
class MATH_CLASS CacheCleaner
{
  int  subscribed;
public:
  CacheCleaner();
  virtual ~CacheCleaner();

  /** \brief \ru Подписан ли объект на сборку мусора.
             \en Whether the object is subscribed for garbage collection. \~
  */
  bool IsSubscribed() { return subscribed > 0;  }

  /** \brief \ru Очистить кэшированные данные. Возвращает true, если кэшированные данные были очищены и объект отписан от сборки мусора.
             \en Reset cached data. Return true if cached data was reset and the object was unsubscribed from garbage collection.\~
  */
  virtual bool ResetCacheData() = 0;

  /** \brief \ru Принудительное удаление всех кэшей (используется сборщиком мусора).
             \en Forced deletion of all caches (used by the garbage collector). \~
  */
  virtual void HardReset() = 0;

  /** \brief \ru Подписаться на сборку мусора.
             \en Subscribe for garbage collection. \~
  */
  void SubcribeOnCleaning();

  /** \brief \ru Отписаться от сборки мусора.
             \en Unsubscribe from garbage collection. \~
  */
  void UnsubcribeOnCleaning();
};

//------------------------------------------------------------------------------
/**
  \brief \ru Сборщик мусора в объектах, использующих кэширование данных.
         \en Garbage collector in objects which use data caching. \~
\details \ru Сборщик мусора. По требованию очищает кэши в зарегистрированных объектах CacheCleaner,
         вызывая метод ResetCacheData каждого объекта. \n
         \en Garbage collector. At request clears caches in registered CacheCleaner objects
         by calling the method ResetCacheData of each object. \n \~
\ingroup Base_Tools
*/
//---
class MATH_CLASS MbGarbageCollection
{
public:

  /** \brief \ru Подписать объект на сборку мусора.
             \en Subscribe the object for garbage collection. \~
  */
  static void Subscribe( CacheCleaner * obj );

  /** \brief \ru Отписать объект от сборки мусора.
             \en Unsubscribe the object from garbage collection. \~
  */
  static void Unsubscribe( CacheCleaner * obj );

  /** \brief   \ru Выполнить сборку мусора. \en Perform garbage collection.
      \details \ru Функция должна вызываться в последовательном участке кода.
               При вызове в параллельном регионе ничего не делает.
               Вызывает метод ResetCacheData каждого объекта.
               \en The function should be called in sequential code.
               When called in a parallel region, does nothing.
               Calls method ResetCacheData of each object.
      \param[in] force - \ru Если false, то инициируется сборка мусора в кэшах, созданных для потоков, которые уже завершены,
                         если true, то инициируется принудительная сборка мусора во всех кэшах.
                         \en If false, then run garbage collection in caches created for threads which are finished,
                         if true, then force garbage collection in all caches.
      \return \ru Возвращает TRUE, если сборка проведена. \en Returns TRUE if the garbage collection is done. \~
  */
  static bool Run( bool force = false );

  /** \brief   \ru Сбросить все зарегистрированные кэши. \en Reset all registered caches.
      \details \ru Сбросить все зарегистрированные кэш после фатальной ошибки.
               Должна вызываться в последовательном участке кода. При вызове в параллельном регионе ничего не делает.
               Вызывает метод HardReset каждого объекта.
               \en Reset all registered caches after fatal error. Should be called in sequential code.
               When called in a parallel region, does nothing.
               Calls method HardReset of each object.
  */
  static void Reset();

  /** \brief \ru Активировать/деактивировать сбор данных для проведения сборки мусора.
             По умолчанию, сбор данных для сборки мусора активирован.
             \en Enable/disable collecting data for garbage collection.
             By default, collecting data for garbage collection is enabled. \~
  */
  static void Enable( bool allow = true );
};

//------------------------------------------------------------------------------
// \ru Принудительно вернуть освобожденную динамическую память операционной системе.
//     Может быть полезна после выполнения операций с интенсивным использованием памяти.
// \en Force to return freed heap memory to the operating system.
//     May be useful after performing memory-intensive operations.
// ---
MATH_FUNC( void ) ReleaseMemory();

//------------------------------------------------------------------------------
/**
\brief \ru Родительский класс данных для менеджера параллельной обработки.
       \en Parent class of data for manager of parallel processing. \~
\details \ru Родительский класс для данных, которые могут обрабатываться параллельно
         с помощью менеджера кэшей.
         \en Parent class for data which could be processed in parallel using the cache manager. \~
  \ingroup Base_Tools
*/
// ---
class MATH_CLASS AuxiliaryData {
public:
  AuxiliaryData() {}
  AuxiliaryData( const AuxiliaryData & ) {}
  virtual ~AuxiliaryData() {}

  /** \brief \ru Объединить с указанными данными.
             \en Merge with specified data. 
    \details \ru Функция вызывается Менеджером кэшей для данных основного потока
             с данными каждого многопоточного кэша в качестве параметра.
             Должна возвращать true, если MergeWith() не пустая и объединение произошло,
             или false в противном случае.
             После завершения функции Менеджер кэшей удаляет многопоточный кэш.
             \en The function is called by CacheManager for the main thread data with 
             each multithreaded cache data as a parameter.
             Should return true if MergeWith() is not empty and the merge occurred,
             or false otherwise.
             When the function completed, the CacheManager deletes the multithreaded cache. \~
  */
  virtual bool MergeWith( AuxiliaryData * ) { return false; }
};

//#define CACHE_DELETE_LOCK
//------------------------------------------------------------------------------
/**
\brief \ru Менеджер параллельной обработки данных (менеджер кэшей) с возможностью пост-обработки кэшей потоков.
       \en Manager for parallel data processing (the cache manager) with support of caches post-processing. \~
\details \ru Менеджер кэшей представляет шаблон, содержащий:
       longTerm - данные главного потока при последовательном выполнении и
       tcache - список кэшей с данными, которые используются при параллельном выполнении.
       Каждый поток по идентификатору threadKey использует только свою копию данных.
       Для многопоточной обработки зависимых (имеющих общие данные) объектов должен использоваться режим
       многопоточных вычислений не ниже mtm_SafeItems.
       Менеджер предоставляет функцию Postprocess() для пост-обработки кэшей, которая вызывается после
       выхода из параллельных вычислений. Указанная функция итерируется по кэшам, использованным
       при параллельных вычислениях, и вызывает функцию longTerm.MergeWith() с данными каждого кэша
       в качестве параметра. После завершения работы функции Postprocess() кэши удаляются. \n
       \en The cache manager is a template which contains:
       longTerm - data of the main thread in sequential execution, and
       tcache - a list of caches with data which are used in parallel calculations. 
       Each thread uses its own copy of data according to threadKey.
       For multithreaded processing of dependent (with shared data) objects the multithreading mode mtm_SafeItems
       or higher should be used.
       The Manager provides a Postprocess() function for caches post-processing which is called
       after exiting parallel computing. The specified function iterates through the caches used
       in parallel computing and calls the function longTerm.MergeWith() with the data of the each cache
       as a parameter. After the function Postprocess() finished the caches are destroyed. \n \~
*/
// ---
template<class T>
class CacheManager : public CacheCleaner {
  struct List
  {
    unsigned int _id;
    T*           _data;
    List*        _next;
    bool         _valid;
    List( unsigned int id, T* data ) :
      _id( id ),
      _data( data != nullptr ? data : new T() ), // Always _data != nullptr.
      _next( nullptr ),
      _valid( true ) {}
    ~List() {
      if ( _data != nullptr )
        delete _data;
      _data = nullptr;
      if ( _next != nullptr ) // Also deletes linked List.
        delete _next;
      _next = nullptr;
    }
  private:
    List() : _id( 0 ), _data( nullptr ), _next( nullptr ) {}
  };

private:
  T*            longTerm; // \ru Данные главного потока при последовательном выполнении. \en Data of the main thread in sequential execution.
  List*         tcache;   // \ru Данные, которые используются при параллельном выполнении. \en Caches which are used in parallel execution.
  CommonMutex*  lock;     // \ru Блокировка для операций с кэшами. \en Lock for operations with caches.

public:
#ifdef CACHE_DELETE_LOCK
  CacheManager( bool createLock = false );
#else
  CacheManager( bool createLock = true );
#endif
  CacheManager( const CacheManager & );
  ~CacheManager();

  /** \brief \ru Оператор (). Возвращает указатель на кэш (данные) текущего потока. Всегда возвращает ненулевое значение.
             \en Operator (). Returns a pointer to the cache (data) of the current thread. Always returns non-null value. \~
  */
  T *   operator ()();

  /** \brief \ru Удалить данные в кэшах. Если resetLongTerm == true, удалить также данные кэша главного потока.
             \en Delete caches data. If resetLongTerm == true, also delete data of the main thread cache.
  */
  void  Reset    ( bool resetLongTerm = false );

  /** \brief \ru Получить указатель на кэш (данные) главного потока. Всегда возвращает ненулевое значение.
                 Все операции с кэшем главного потока должны быть защищены блокировкой кэша.
             \en Get a pointer to cache (data) of the main thread. Always returns non-null value.
                 All operations with the main thread cache should be protected by the cache lock. \~
  */
  T *   LongTerm ();

  /** \brief \ru Получить указатель на блокировку для операций с кэшем главного потока, учитывая, исполняется ли код параллельно
             Может возвращать нулевое значение (удобно для использования с ScopedLock).
             \en Get a pointer to the lock for operations with the main thread cache, considering whether the code runs in parallel.
             Can return null value (good for use with ScopedLock). \~
  */
  CommonMutex* GetLock() { if ( ::LocksEnabled() ) return GetLockHard();  return nullptr; }

  /** \brief \ru Функция очистки, используемая сборщиком мусора.
             \en Cleaning function, used by the garbage collector. \~
  */
  virtual bool ResetCacheData() { CleanAll( true, true ); return true; }

  /** \brief \ru Принудительное удаление всех кэшей (используется сборщиком мусора).
             \en Forced deletion of all caches (used by the garbage collector). \~
  */
  virtual void HardReset();

private:
  /** \brief \ru Удалить все кэши и отписаться от сборки мусора. Должна вызываться в последовательном участке кода.
             \en Delete all caches and unsubscribe from the garbage collection. Should be called in sequential code. \~
  */
  void CleanAll( bool doPostproc, bool force = false );
  
  /** \brief \ru Получить указатель на блокировку для операций с кэшем главного потока. Всегда возвращает ненулевое значение.
             \en Get a pointer to the lock for operations with the main thread cache. Always returns non-null value. \~
  */
  CommonMutex* GetLockHard();

  /** \brief \ru Пост-обработка кэшей после выхода из параллельных вычислений.
             \en Caches post-processing after exiting the parallel calculations. \~
  */
  void Postprocess();

  CacheManager & operator = ( const CacheManager & ); // \ru Не разрешен. \en Not allowed.
};


//------------------------------------------------------------------------------
// \ru Конструктор. \en Constructor. 
// ---
template<class T>
inline CacheManager<T>::CacheManager( bool createLock )
  : longTerm  ( nullptr )
  , tcache    ( nullptr )
  , lock      ( nullptr )
{
  if ( createLock )
    lock = new CommonMutex();
}



//------------------------------------------------------------------------------
// \ru Конструктор. \en Constructor. 
// ---
template<class T>
inline CacheManager<T>::CacheManager( const CacheManager<T> & item )
  : longTerm  ( nullptr )
  , tcache    ( nullptr )
  , lock      ( nullptr )
{
  if ( item.longTerm != nullptr )
    longTerm = new T( *item.longTerm );
#ifndef CACHE_DELETE_LOCK
  lock = new CommonMutex();
#endif
}


//------------------------------------------------------------------------------
// \ru Деструктор. \en Destructor. 
// ---
template<class T>
inline CacheManager<T>::~CacheManager()
{ 
  CleanAll( false, true );
  if ( longTerm != nullptr )
    delete longTerm;
  if ( lock != nullptr )
    delete lock;
}

//------------------------------------------------------------------------------
// \ru Получить указатель на кэш главного потока. Всегда возвращает ненулевое значение.
//     Все операции с кэшем главного потока должны быть защищены блокировкой кэша.
// \en Get a pointer to the main thread cache. Always returns non-null value.
//     All operations with the main thread cache should be protected by the cache lock.
// ---
template<class T>
inline T* CacheManager<T>::LongTerm ()
{
  try {
    if ( longTerm == nullptr )
      longTerm = new T();
  }
  catch ( const std::bad_alloc & ) {
    FatalErrorHandler::SetError( rt_NotEnoughMemory );
  }
  return longTerm;
}

//------------------------------------------------------------------------------
// \ru Получить указатель на блокировку для операций с кэшем главного потока. Всегда возвращает ненулевое значение.
// \en Get a pointer to the lock for operations with the main thread cache. Always returns non-null value.
// ---
template<class T>
inline CommonMutex* CacheManager<T>::GetLockHard()
{
  if ( lock == nullptr ) {
    CommonMutex* ll = GetGlobalLock();
    ll->Lock();
    if ( lock == nullptr )
      lock = new CommonMutex();
    ll->Unlock();
  }
  return lock;
}

//------------------------------------------------------------------------------
// \ru Оператор (). Возвращает указатель на кэш текущего потока (всегда ненулевое значение).
// \en Operator (). Returns a pointer to the current thread cache (always non-null value).
// ---
template<class T>
inline T * CacheManager<T>::operator()()
{
// \ru Создать данные по данным кэша главного потока. \en Create data using the data of the main thread cache.
#define INIT_BY_LONGTERM  ( longTerm != nullptr ? new T( *longTerm ) : new T() )

  if ( !IsSafeMultithreading() || !IsInParallel() ) {
    CleanAll( true );
    return LongTerm();
  }

  T * res = nullptr;
  unsigned int threadKey = GetThreadKey();

  if ( FatalErrorHandler::HasError() ) {
    ScopedLock sl( GetLock(), false );
    return LongTerm();
  }

  if ( tcache == nullptr ) {
    // \ru Подписаться на сборку мусора, так как используются многопоточные кэши.
    // \en Subscribe on garbage collection because using multithreaded caches.
    SubcribeOnCleaning();
    {
      // \ru Используется блокировка при изменении списка кэшей. \en Use lock when changing the cache list.
      ScopedLock sl( GetLock(), false );
      if ( tcache == nullptr ) {
        try {
            tcache = new List( threadKey, INIT_BY_LONGTERM );
            return tcache->_data;
        }
        catch ( const std::bad_alloc & ) {
          FatalErrorHandler::SetError( rt_NotEnoughMemory );
          return LongTerm();
        }
      }
    }
  }

  List* entry = tcache;
  while( entry != nullptr ) {
    if ( entry->_id == threadKey ) {
      if ( !entry->_valid ) {
        try {
          delete entry->_data;
          {
            ScopedLock sl( GetLock(), false );
            entry->_data = INIT_BY_LONGTERM;
          }
          entry->_valid = true;
        }
        catch ( const std::bad_alloc & ) {
          FatalErrorHandler::SetError( rt_NotEnoughMemory );
          return LongTerm();
        }
      }
      return entry->_data;
    }
    // \ru Если кэш не найден в списке, 'entry' содержит последний (на данный момент) элемент в списке.
    // \en If cache not found in the list, 'entry' contains the last element in the list (at that point).
    if ( entry->_next == nullptr )
      break; 
    entry = entry->_next;
  }
  {
    // \ru Используется блокировка при изменении списка кэшей. \en Use lock when changing the cache list.
    ScopedLock sl( GetLock(), false );
    try {
      res = INIT_BY_LONGTERM;
      List * newList = new List( threadKey, res );
      // \ru На данный момент, entry может быть не последним элементом в списке.
      // \en At that point, entry could be not a last element in the list.
      while ( entry->_next != nullptr ) {
        entry = entry->_next;
      }
      entry->_next = newList;
    }
    catch ( const std::bad_alloc & ) {
      FatalErrorHandler::SetError( rt_NotEnoughMemory );
      return LongTerm();
    }
  } // ScopedLock

  return res;

}


//------------------------------------------------------------------------------
// \ru Аннулировать данные в кэшах. Если resetLongTerm == true, удалить данные кэша главного потока.
// \en Invalidate caches data. If resetLongTerm == true, delete data of the main thread cache.
// ---
template<class T>
inline void CacheManager<T>::Reset( bool resetLongTerm )
{
  if ( tcache != nullptr ) {
    ScopedLock sl( GetLock() );
    List* entry = tcache;
    while ( entry != nullptr ) {
      entry->_valid = false;
      entry = entry->_next;
    }
  }
  if ( resetLongTerm ) {
    ScopedLock sl( GetLock() );
    delete longTerm;
    longTerm = nullptr;
    // \ru Если нет параллельности, удаляется блокировка. \en If no parallelism, delete the lock.
#ifdef CACHE_DELETE_LOCK
    if ( !sl.IsLocked() ) {
      if ( lock != nullptr )
        delete lock;
      lock = nullptr;
    }
#endif
  }
}

//------------------------------------------------------------------------------
// \ru Удалить все кэши и отписаться от сборки мусора. Должна вызываться в последовательном участке кода.
// \en Delete all caches and unsubscribe from the garbage collection. Should be called in sequential code.
// ---
template<class T>
inline void CacheManager<T>::CleanAll( bool doPostproc, bool force )
{
  if ( force || CacheCleanupAllowed() ) {
    if ( tcache != nullptr ) {
      if ( doPostproc )
        Postprocess();
      delete tcache;
      tcache = nullptr;
    }
#ifdef CACHE_DELETE_LOCK
    if ( lock != nullptr ) {
      delete lock;
      lock = nullptr;
    }
#endif
    if ( IsSubscribed() )
      UnsubcribeOnCleaning();
  }
}

//------------------------------------------------------------------------------
// \ru Пост-обработка кэшей после выхода из параллельных вычислений.
// \en Caches post-processing after exiting the parallel calculations. \~
// ---
template<class T>
inline void CacheManager<T>::Postprocess()
{
  if ( tcache != nullptr ) {
    LongTerm(); // Create longTerm
    List * entry = tcache;
    // Incorporate thread data into main thread data.
    while ( entry != nullptr && longTerm->MergeWith( entry->_data ) ) {
      entry = entry->_next;
    }
  }
}

//------------------------------------------------------------------------------
// \ru Принудительное удаление всех кэшей (используется сборщиком мусора).
// \en Forced deletion of all caches (used by the garbage collector).
//------------------------------------------------------------------------------
template<class T>
inline void CacheManager<T>::HardReset()
{
  if ( tcache != nullptr ) {
    delete tcache;
    tcache = nullptr;
  }
  if ( longTerm != nullptr ) {
    delete longTerm;
    longTerm = nullptr;
  }

  if ( lock != nullptr ) {
    delete lock;
    lock = nullptr;
  }
}

#endif // __TOOL_MULTITHREADING_H
