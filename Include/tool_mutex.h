////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Реализация блокировок на базе системных механизмов синхронизации 
          и OpenMP блокировок.
         \en Locks implementation on base of system synchronization mechanisms
         and OpenMP locks. \~
  details \ru Реализация блокировок (в том числе блокировки в области видимости) 
          на базе системных механизмов синхронизации и OpenMP блокировок.\n
          \en Implementation of locks (including scoped lock) on base of 
          system synchronization mechanisms and OpenMP locks.\n \~
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __TOOL_MUTEX_H
#define __TOOL_MUTEX_H

#include <math_define.h>


//------------------------------------------------------------------------------
/**
\brief \ru Режимы многопоточных вычислений.
       \en Multithreading modes. \~
\details \ru Режимы многопоточных вычислений. Режим многопоточности ядра управляет механизмом потокобезопасности объектов ядра и определяет, какие операции ядра будут распараллеливаться. \n
         \en Multithreading modes. Multithreading mode of the kernel manages the mechanism of thread-safity of the kernel objects and defines which \n \~
\ingroup Data_Structures
*/
//---
enum MbeMultithreadedMode
{
  ///< \ru Многопоточность ядра полностью отключена. Не разрешено использовать интерфейсы ядра в многопоточных вычислениях.
  ///< \en Kernel multithreading is completely disabled. Using kernel interfaces in parallel computing is prohibited.
  mtm_Off = 0,
  ///< \ru Разрешена многопоточная обработка только для объектов, не имеющих общих данных (независимых объектов). Включены внутренние многопоточные вычисления только для независимых объектов.
  ///< \en Allowed parallel processing of objects that do not have common data (independent objects). Enabled internal multi-threaded calculations only for independent objects.
  mtm_Standard = 1,
  ///< \ru Обеспечивается потокобезопасность интерфейсов ядра. Разрешено использовать все интерфейсы ядра в многопоточных вычислениях. При этом включены внутренние многопоточные вычисления только для независимых объектов.
  ///< \en Ensured thread safety of kernel interfaces. Allowed using all kernel interfaces in parallel computing. Enabled internal multithreaded calculations only for independent objects.
  mtm_SafeItems = 2,
  ///< \ru Обеспечивается потокобезопасность объектов ядра. Разрешено использовать все интерфейсы ядра в многопоточных вычислениях. Полностью включены внутренние многопоточные вычисления.
  ///< \en Ensured thread-safety of kernel interfaces. Allowed using all kernel interfaces in parallel computing. Enabled all internal parallel calculations in the kernel.
  mtm_Items = 3,
  ///< \ru Включена максимальная многопоточность ядра. Можно использовать интерфейсы ядра в параллельных вычислениях. Полностью включены внутренние многопоточные вычисления.
  ///< \en Maximal kernel multithreading is ON. Allowed using all kernel interfaces in parallel computing. Enabled all internal parallel calculations in the kernel.
  mtm_Max = 31
};


////////////////////////////////////////////////////////////////////////////////
//
// \ru Поддержка многопоточности в пользовательском приложении.
// 
// \ru Перед запуском параллельных вычислений, необходимо проверить, что установленный режим
//     многопоточности ядра, позволяет использовать параллельные вычисления.
// \ru Чтобы использовать интерфейсы ядра в нескольких потоках,
// \ru в ядре должен быть установлен режим многопоточных вычислений не ниже mtm_SafeItems.
// \ru По умолчанию в ядре установлен режим максимальной многопоточности.
//
// \ru При использовании параллельных вычислений необходимо нотифицировать ядро
//     о входе в параллельный регион и выходе из него.
// \ru Для этого могут быть использованы:
//     либо класс ParallelRegionGuard (защитник параллельного региона в области видимости),
//     либо парные функции EnterParallelRegion и ExitParallelRegion,
//     либо парные макросы ENTER_PARALLEL и EXIT_PARALLEL.
// 
// \ru Примеры:
//
//  // Использование класса ParallelRegionGuard.
//  if( Math::CheckMultithreadedMode( mtm_Items ) ){
//    ParallelRegionGuard l;
//    std::thread t1( function1 );
//    std::thread t2( function2 );
//    t1.join();
//    t2.join();
//  }
//
//  // Использование функций.
//  if( Math::CheckMultithreadedMode( mtm_Items ) ){
//    EnterParallelRegion();
//    std::thread t1( function1 );
//    std::thread t2( function2 );
//    t1.join();
//    t2.join();
//    ExitParallelRegion();
//  }
//
//  // Использование макросов.
//  bool useParallel = Math::CheckMultithreadedMode( mtm_Items );
//  ENTER_PARALLEL( useParallel );
//  std::thread t1( function1 );
//  std::thread t2( function2 );
//  t1.join();
//  t2.join();
//  EXIT_PARALLEL( useParallel );
//
//
// \en Support of multithreading in user application.
//
// \en Before running parallel calculations, you need to check that the kernel multithreading mode 
// \en allows parallel computing.
// \en For using the kernel interfaces in several threads, the multithreading mode mtm_SafeItems
// \en or higher should be defined in the kernel.
// \en By default, the maximal multithreading mode is set in the kernel.
//
// \en While using parallel calculations, you need to notify the kernel about entering
// \en and exiting a parallel region.
// \en For that, you can use one of the ways:
//     the class ParallelRegionGuard (a scoped guard of parallel region),
// \en or the pair of the functions EnterParallelRegion and ExitParallelRegion, 
//     or the pair of the macros ENTER_PARALLEL and EXIT_PARALLEL.
// 
// \en Examples:
//
//  // Using the ParallelRegionGuard class.
//  if( Math::CheckMultithreadedMode( mtm_Items ) )
//  {
//    ParallelRegionGuard l;
//    std::thread t1( function1 );
//    std::thread t2( function2 );
//    t1.join();
//    t2.join();
//  }
//
//  // Using the pair of the functions.
//  if( Math::CheckMultithreadedMode( mtm_Items ) )
//  {
//    EnterParallelRegion();
//    std::thread t1( function1 );
//    std::thread t2( function2 );
//    t1.join();
//    t2.join();
//    ExitParallelRegion();
//  }
//
// // Using the pair of the macros.
//  bool useParallel = Math::CheckMultithreadedMode( mtm_Items );
//  ENTER_PARALLEL( useParallel );
//  std::thread t1( function1 );
//  std::thread t2( function2 );
//  t1.join();
//  t2.join();
//  EXIT_PARALLEL( useParallel );
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// \ru Управление блокировками.
// \en Locks management. \~
//
// \ru Переменная C3D_NATIVE_LOCK включает использование блокировок на базе
// \ru системных механизмов синхронизации вместо OpenMP,
// \ru что позволяет использовать механизмы распараллеливания, отличные от OpenMP.
//
// \en The variable C3D_NATIVE_LOCK enables using locks on base of system
// \en synchronization mechanisms instead of OpenMP, that allows
// \en use of parallelization frameworks, other than OpenMP. \~
//
////////////////////////////////////////////////////////////////////////////////
#define C3D_NATIVE_LOCK


#ifdef C3D_NATIVE_LOCK
class ToolLock;

//------------------------------------------------------------------------------
/** \brief \ru Класс блокировки. \en Lock class. \~
    \details \ru Класс блокировки (реализация на базе системных механизмов синхронизации).
             \en Lock class (implementation on base of system synchronization mechanisms). \~
    \ingroup Base_Tools
*/
// ---
class MATH_CLASS CommonMutex
{
  ToolLock* m_lock;
public:
  CommonMutex();
  ~CommonMutex();

  /** \brief \ru Установить блокировку. \en Set a lock. \~
  */
  void Lock();
  /** \brief \ru Снять блокировку. \en Unset a lock. \~
  */
  void Unlock();

private:
  // \ru Запрет копирования. \en Copy forbidden.
  CommonMutex ( const CommonMutex& );
  CommonMutex& operator = ( const CommonMutex& );
};

//------------------------------------------------------------------------------
/** \brief \ru Одинаковая реализация CommonMutex и CommonRecursiveMutex.
           \en Same implementation of CommonMutex and CommonRecursiveMutex. \~
    \ingroup Base_Tools
*/
#define CommonRecursiveMutex CommonMutex

#else // C3D_NATIVE_LOCK

//------------------------------------------------------------------------------
/** \brief \ru Класс блокировки.
           \en Lock class. \~
    \details \ru Класс блокировки на базе OpenMP lock.
           \en Lock class on base of OpenMP lock. \~
    \ingroup Base_Tools
*/
// ---
class MATH_CLASS CommonMutex
{
  omp_lock_t m_lock;
public:
  // For correct work, CommonMutex implementation should be encapsulated in cpp.
  CommonMutex();
  ~CommonMutex();
  void lock();
  void unlock();
};

//------------------------------------------------------------------------------
/** \brief \ru Класс блокировки на базе вложенного OpenMP lock.
           \en Wrapper for nested OpenMP lock. \~
    \ingroup Base_Tools
*/
// ---
class MATH_CLASS CommonRecursiveMutex
{
  omp_nest_lock_t m_lock;
public:
  // For correct work, CommonRecursiveMutex implementation should be encapsulated in cpp.
  CommonRecursiveMutex();
  ~CommonRecursiveMutex();
  void lock();
  void unlock();
};

#endif // C3D_NATIVE_LOCK


//------------------------------------------------------------------------------
/** \brief \ru Защитник параллельного региона в области видимости.
           \en Scoped guard of parallel region. \~
  \details \ru Класс защищает регион кода, выполняющийся параллельно. Работает в области видимости.
           Должен использоваться для защиты параллельного кода.
           Пример использования:
             if( Math::CheckMultithreadedMode( mtm_Items ) )
             {
               ParallelRegionGuard l;
               std::thread t1( function1 );
               std::thread t2( function2 );
               t1.join();
               t2.join();
             }
           \en The class guards a code region running in parallel. Works in scope.
           Should be used to protect parallel code.
           Example of use:
             if( Math::CheckMultithreadedMode( mtm_Items ) )
             {
               ParallelRegionGuard l;
               std::thread t1( function1 );
               std::thread t2( function2 );
               t1.join();
               t2.join();
             }
   \ingroup Base_Tools
*/
// ---
class MATH_CLASS ParallelRegionGuard
{
public:
  ParallelRegionGuard();
  ~ParallelRegionGuard();
};

//------------------------------------------------------------------------------
/** \brief \ru Функция нотифицирует ядро о входе в параллельный блок кода.
           Вызов функции должен стоять перед началом параллельного блока.
           \en The function notifies the kernel about entering a parallel region.
           The function call should be placed before the start of a parallel block
*/
// ---
MATH_FUNC( void ) EnterParallelRegion();

//------------------------------------------------------------------------------
/** \brief \ru Функция нотифицирует ядро о выходе из параллельного блока кода.
           Вызов функции должен стоять после окончания параллельного блока.
           \en The function notifies the kernel about exiting a parallel region.
           The function call should be placed after the end of the parallel block.
*/
// ---
MATH_FUNC( void ) ExitParallelRegion();


//------------------------------------------------------------------------------
// \ru Макросы для нотификации о входе и выходе из параллельного цикла.
// Вызов макросов при использовании OpenMP не обязателен, однако значительно ускоряет 
// выполнение параллельного цикла.
// Вызов ENTER_PARALLEL должен стоять перед началом параллельного блока.
// Вызов EXIT_PARALLEL должен стоять после окончания параллельного блока.
// Пример использования:
//   bool useParallel = Math::CheckMultithreadedMode( mtm_Items );
//   ENTER_PARALLEL( useParallel );
//   #pragma omp parallel for
//   for ( ptrdiff_t i = 0; i < count; ++i ) {
//     /* Cycle body */
//   }
//   EXIT_PARALLEL( useParallel );
//
/// \ru Macros for notification of entering and exiting a parallel block.
// Calling the macros when using OpenMP is not required, but significantly speeds up  
// the execution of parallel cycle.
// The call ENTER_PARALLEL should be placed before the start of a parallel block.
// The call EXIT_PARALLEL should be placed after the end of the parallel block.
// Example of use:
//   bool useParallel = Math::CheckMultithreadedMode( mtm_Items );
//   ENTER_PARALLEL( useParallel );
//   #pragma omp parallel for
//   for ( ptrdiff_t i = 0; i < count; ++i ) {
//     /* Cycle body */
//   }
//   EXIT_PARALLEL( useParallel );
// ---

//------------------------------------------------------------------------------
/** \brief \ru Если useParallel == true, нотифицирует ядро о входе в параллельный блок кода.
           \en If useParallel == true, notifies the kernel about entering a parallel region.
  \details \ru Если useParallel == true, нотифицирует ядро о входе в параллельный блок кода.
           Вызов должен стоять перед началом параллельного блока (перед прагмой OpenMP).
           Использование макроса значительно ускоряет параллельные циклы OpenMP.
           \en If useParallel == true, notifies the kernel about entering a parallel region.
           The call should be placed before the start of a parallel block (before OpenMP pragma).
           Using a macro speeds up parallel OpenMP cycles significantly.
*/
// ---
#define ENTER_PARALLEL(useParallel) if ( useParallel ) EnterParallelRegion();

//------------------------------------------------------------------------------
/** \brief \ru Если useParallel == true, нотифицирует ядро о выходе из параллельного блока кода.
           \en If useParallel == true, notifies the kernel about exiting a parallel region.
  \details \ru Если useParallel == true, нотифицирует ядро о выходе из параллельного блока кода.
           Вызов должен стоять после окончания параллельного блока.
           Использование макроса значительно ускоряет параллельные циклы OpenMP.
           \en If useParallel == true, notifies the kernel about exiting a parallel region.
           The call should be placed after the end of the parallel block.
           Using a macro speeds up parallel OpenMP cycles significantly.
*/
// ---
#define EXIT_PARALLEL(useParallel)  if ( useParallel ) ExitParallelRegion();

//------------------------------------------------------------------------------
/** \brief \ru Нотифицирует ядро о входе в параллельный блок кода.
           \en Notifies the kernel about entering a parallel region.
  \details \ru Нотифицирует ядро о входе в параллельный блок кода.
           Вызов должен стоять перед началом параллельного блока (перед прагмой OpenMP).
           Использование макроса значительно ускоряет параллельные циклы OpenMP.
           \en Notifies the kernel about entering a parallel region.
           The call should be placed before the start of a parallel block (before OpenMP pragma).
           Using a macro speeds up parallel OpenMP cycles significantly.
*/
// ---
#define ENTER_PARALLEL_FORCED EnterParallelRegion();

//------------------------------------------------------------------------------
/** \brief \ru Нотифицирует ядро о выходе из параллельного блока кода.
           \en Notifies the kernel about exiting a parallel region.
  \details \ru Нотифицирует ядро о выходе из параллельного блока кода.
           Вызов должен стоять после окончания параллельного блока.
           Использование макроса значительно ускоряет параллельные циклы OpenMP.
           \en Notifies the kernel about exiting a parallel region.
           The call should be placed after the end of the parallel block.
           Using a macro speeds up parallel OpenMP cycles significantly.
*/
// ---
#define EXIT_PARALLEL_FORCED ExitParallelRegion();

//------------------------------------------------------------------------------
/** \brief \ru Функция определяет, выполняется ли код параллельно.
           \en The function determines whether the code is executed in parallel.
*/
// ---
MATH_FUNC( bool ) IsInParallel();

//------------------------------------------------------------------------------
/** \brief \ru Функция определяет, разрешена ли очистка кэшей.
           \en The function determines whether the caches cleanup is allowed.
*/
// ---
MATH_FUNC( bool ) CacheCleanupAllowed();


////////////////////////////////////////////////////////////////////////////////
//
// \ru Блокировки и другие средства синхронизации.
// \en Locks and other synchronization objects. \~

// \ru В качестве блокировок должны использоваться CommonMutex и CommonRecursiveMutex
// \ru (OpenMP lock не должны использоваться напрямую).
//
// \en CommonMutex and CommonRecursiveMutex should be used as locks
// \en (OpenMP locks should not be used directly).
//
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
/** \brief \ru Блокировка в области видимости. Может принимать нулевой указатель на мьютекс.
           Блокировка происходит, если указатель на мьютекс ненулевой и код выполняется параллельно.
           \en Scoped lock. Can accept a null pointer to a mutex.
           Locking occurs if the pointer to the mutex is nonzero and the code runs in parallel. \~
    \ingroup Base_Tools
*/
// ---
class MATH_CLASS ScopedLock
{
  CommonMutex* m_mutex;
public:
  ScopedLock( CommonMutex* mtx, bool parallelCheck = true );
  ~ScopedLock();

  /** \brief \ru Выполнена ли реальная блокировка. \en Whether a real locking performed. \~
  */
  bool IsLocked();

private:
  ScopedLock();
  ScopedLock ( const ScopedLock& );
  ScopedLock& operator = ( const ScopedLock& );
};

//------------------------------------------------------------------------------
/** \brief \ru Рекурсивная блокировка в области видимости. Может принимать нулевой указатель на мьютекс.
           Блокировка происходит, если указатель на мьютекс ненулевой и код выполняется параллельно.
           \en Recursive scoped lock. Can accept a null pointer to a mutex.
           Locking occurs if the pointer to the mutex is nonzero and the code runs in parallel. \~
\ingroup Base_Tools
*/
// ---
class MATH_CLASS ScopedRecursiveLock
{
  CommonRecursiveMutex* m_mutex;
public:
  ScopedRecursiveLock( CommonRecursiveMutex* mtx, bool parallelCheck = true );
  ~ScopedRecursiveLock();

  /** \brief \ru Выполнена ли реальная блокировка. \en Whether a real locking performed. \~
  */
  bool IsLocked();

private:
  ScopedRecursiveLock();
  ScopedRecursiveLock ( const ScopedRecursiveLock& );
  ScopedRecursiveLock& operator = ( const ScopedRecursiveLock& );
};

//------------------------------------------------------------------------------
/** \brief \ru Базовый объект синхронизации с отложенной инициализацией.
           \en Base synchronization object with lazy initialization. \~
    \details \ru Базовый объект, предоставляющий средства синхронизации и создающий блокировку при необходимости. \n
             \en Base object which provides means of synchronization and creates a lock when needed. \n \~
\ingroup Base_Tools
*/  
// ---  
class MATH_CLASS MbSyncItem {
protected:
  mutable CommonMutex * m_comLock;  // \ru Критическая секция для монопольного доступа к объекту. \en The critical section for exclusive access to the object.
  mutable int           m_locked;

public:
  MbSyncItem();
  virtual ~MbSyncItem();

  /** \brief \ru Включить блокировку (блокировка происходит только при наличии параллельности).
             \en Switch lock on (locking happens only in parallel region).
  */
  void Lock() const;

  /** \brief  \ru Снять блокировку, если она была установлена.
              \en Switch lock off if locking has been set.
  */
  void Unlock() const;

  // \ru Выдать указатель на объект мьютекса. Возращает nullptr, если параллельности нет. Для использования в ScopedLock.
  // \en Get a pointer to the mutex object. Return nullptr if no parallelism. For use in ScopedLock.
  CommonMutex * GetLock() const;
};

//------------------------------------------------------------------------------
/** \brief \ru Базовый объект синхронизации с отложенной инициализацией, поддерживающий множественные блокировки.
           \en Base synchronization object with lazy initialization which supports nested locks. \~
    \details \ru Базовый объект синхронизации, поддерживающий множественные блокировки и создающий блокировку при необходимости. \n
             \en Base synchronization object with support of nested locks which creates a lock if necessary. \n \~
\ingroup Base_Tools
*/  
// ---  
class MATH_CLASS MbNestSyncItem {
protected:
  mutable CommonRecursiveMutex * m_comLock;  // \ru Критическая секция для монопольного доступа к объекту. \en The critical section for exclusive access to the object.
  mutable int                    m_locked;

public:
  MbNestSyncItem();
  virtual ~MbNestSyncItem();

  /** \brief \ru Включить блокировку (блокировка происходит только при наличии параллельности).
             \en Switch lock on (locking happens only in parallel region).
  */
  void Lock() const;

  /** \brief \ru Снять блокировку, если она была установлена.
             \en Switch lock off if locking has been set.
  */
  void Unlock() const;

  /** \brief \ru Выдать указатель на объект мьютекса. Возращает nullptr, если параллельности нет. Для использования в ScopedLock.
             \en Get a pointer to the mutex object. Return nullptr if no parallelism. For use in ScopedLock.
  */
  CommonRecursiveMutex * GetLock() const;
};

//------------------------------------------------------------------------------
/** \brief \ru Базовый объект, предоставляющий средства синхронизации.
           \en Base object providing means of synchronization. \~
  \details \ru Базовый объект, предоставляющий средства синхронизации. \n
           \en Base object providing means of synchronization. \n \~
\ingroup Base_Tools
*/
// ---  
class MATH_CLASS MbPersistentSyncItem {
protected:
  mutable CommonMutex m_comLock;  // \ru Критическая секция для монопольного доступа к объекту. \en The critical section for exclusive access to the object.
  mutable int         m_locked;

public:
  MbPersistentSyncItem();
  virtual ~MbPersistentSyncItem();

  /** \brief \ru Включить блокировку.
             \en Switch lock on.
  */
  void Lock() const;

  /** \brief \ru Снять блокировку, если она была установлена.
             \en Switch lock off if locking has been set.
  */
  void Unlock() const;

  /** \brief \ru Выдать указатель на объект мьютекса.
             \en Get a pointer to the mutex object.
  */
  CommonMutex * GetLock() const;

protected:
  MbPersistentSyncItem( const MbPersistentSyncItem & );
  MbPersistentSyncItem & operator = ( const MbPersistentSyncItem & );
};

//------------------------------------------------------------------------------
/** \brief \ru Базовый объект синхронизации, поддерживающий множественные блокировки.
           \en Base synchronization object with support of nested locks. \~
  \details \ru Базовый объект синхронизации, поддерживающий множественные блокировки. \n
           \en Base synchronization object with support of nested locks. \n \~
\ingroup Base_Tools
*/
// ---  
class MATH_CLASS MbPersistentNestSyncItem {
protected:
  mutable CommonRecursiveMutex m_comLock;  // \ru Критическая секция для монопольного доступа к объекту. \en The critical section for exclusive access to the object.
  mutable int                  m_locked;

public:
  MbPersistentNestSyncItem();
  virtual ~MbPersistentNestSyncItem();

  /** \brief \ru Включить блокировку. \en Switch lock on.
  */
  void Lock() const;

  /** \brief \ru Снять блокировку, если она была установлена.
             \en Switch lock off if locking has been set.
  */
  void Unlock() const;

  /** \brief \ru Выдать указатель на объект мьютекса.
             \en Get a pointer to the mutex object.
  */
  CommonRecursiveMutex * GetLock() const;

protected:
  MbPersistentNestSyncItem( const MbPersistentNestSyncItem & );
  MbPersistentNestSyncItem & operator = ( const MbPersistentNestSyncItem & );
};


//------------------------------------------------------------------------------
/** \brief \ru Установлен ли режим безопасной многопоточности (используется в CacheManager).
           \en Whether is enabled a safe multithreading mode (used in CacheManager). \~
    \ingroup Base_Tools
*/
MATH_FUNC(bool) IsSafeMultithreading();

//------------------------------------------------------------------------------
/** \brief \ru Включены ли блокировки (режим многопоточности >= mtm_Standard и код выполняется параллельно).
           \en Whether locks are enabled (multithreading is on and code is executed in parallel). \~
    \ingroup Base_Tools
*/
MATH_FUNC( bool ) LocksEnabled();

//------------------------------------------------------------------------------
/** \brief \ru Получить псевдо-идентификатор текущего потока.
           \en Get a current thread pseudo-identifier.
*/
MATH_FUNC( unsigned int ) GetThreadKey();

//------------------------------------------------------------------------------
/** \brief \ru Получить указатель на глобальный мьютекс (используется в CacheManager).
           \en Get a pointer to the global mutex (used in CacheManager).
*/
MATH_FUNC( CommonMutex* ) GetGlobalLock();

//------------------------------------------------------------------------------
/** \brief \ru Получить указатель на глобальный рекурсивный мьютекс (используется для операций выделения и освобождения памяти).
           \en Get a pointer to the global recursive mutex (used for memory allocation and deallocation operations).
*/
MATH_FUNC( CommonRecursiveMutex* ) GetGlobalRecursiveLock();

//------------------------------------------------------------------------------
/** \brief \ru Установить блокировку в области видимости для операций выделения и освобождения памяти.
           \en Set scoped lock for memory allocation and deallocation operations.
*/
#define SET_MEMORY_SCOPED_LOCK ScopedRecursiveLock memScopedLock( GetGlobalRecursiveLock() );

#endif // __TOOL_MUTEX_H
