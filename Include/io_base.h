////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Облегченные макросы сериализации, которые содержат только объявления функций.
         \en Lightweight macros of serialization which contain functions declarations only. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __IO_BASE_H
#define __IO_BASE_H


#include <math_define.h>


class reader;
class writer;

//------------------------------------------------------------------------------
// \ru Объявление операторов чтения/записи для объектов, для которых при записи и чтении точно известен тип.
// \en Declaration of read/write operators for objects which type is exactly known while reading and writing.
// \ru Такие объекты могут записываться в поток и читаться из потока с помощью операторов << и >> .
// \en Such objects can be written to the stream and read from the stream using operators << and >> .
// \ru Реализация операторов, объявленных макросами KNOWN_OBJECTS_RW_REF_OPERATORS_BASE и KNOWN_OBJECTS_RW_PTR_OPERATORS_BASE,
//     находится в макросах IMP_KNOWN_OBJECTS_RW_REF_OPERATORS и IMP_KNOWN_OBJECTS_RW_PTR_OPERATORS.
// \en Implementation of the operators, declared by the macros KNOWN_OBJECTS_RW_REF_OPERATORS_BASE and KNOWN_OBJECTS_RW_PTR_OPERATORS_BASE,
//     is located in the macros IMP_KNOWN_OBJECTS_RW_REF_OPERATORS and IMP_KNOWN_OBJECTS_RW_PTR_OPERATORS.
//---
#define KNOWN_OBJECTS_RW_REF_OPERATORS_BASE(Class)                                   \
  friend reader & CALL_DECLARATION operator >> ( reader & in, Class & ref );         \
  friend writer & CALL_DECLARATION operator << ( writer & out, const Class & ref );  \
  friend writer & CALL_DECLARATION operator << ( writer & out,       Class & ref );

#define KNOWN_OBJECTS_RW_PTR_OPERATORS_BASE(Class)                                   \
  friend reader & CALL_DECLARATION operator >> ( reader & in, Class *& ptr );        \
  friend writer & CALL_DECLARATION operator << ( writer & out, const Class * ptr );  \
  friend writer & CALL_DECLARATION operator << ( writer & out,       Class * ptr );

// \ru Объявление аналогичных операторов чтения/записи для экспорта/импорта.
// \en Declaration of similar read/write operators for export/import.
// \ru Макросы для DLLFUNC -> __declspec( dllexport ) или __declspec( dllimport ) объявлены в файле math_define.h (см. MATH_FUNC_EX).
// \en Macros for DLLFUNC -> __declspec( dllexport ) or __declspec( dllimport ) are declared the file math_define.h (see MATH_FUNC_EX).
// \ru Реализация операторов, объявленных макросами KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE и KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE,
//     находится в макросах IMP_KNOWN_OBJECTS_RW_REF_OPERATORS_EX и IMP_KNOWN_OBJECTS_RW_PTR_OPERATORS_EX.
// \en Implementation of the operators, declared by the macros KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE and KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE,
//     is located in the macros IMP_KNOWN_OBJECTS_RW_REF_OPERATORS_EX and IMP_KNOWN_OBJECTS_RW_PTR_OPERATORS_EX.
#define KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE(Class, DLLFUNC)                               \
  friend DLLFUNC reader & CALL_DECLARATION operator >> ( reader & in, Class & ref );         \
  friend DLLFUNC writer & CALL_DECLARATION operator << ( writer & out, const Class & ref );  \
  friend DLLFUNC writer & CALL_DECLARATION operator << ( writer & out,       Class & ref );

#define KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE(Class, DLLFUNC)                               \
  friend DLLFUNC reader & CALL_DECLARATION operator >> ( reader & in, Class *& ptr );        \
  friend DLLFUNC writer & CALL_DECLARATION operator << ( writer & out, const Class * ptr );  \
  friend DLLFUNC writer & CALL_DECLARATION operator << ( writer & out,       Class * ptr );


/** \brief \ru Переменная включает перегрузку операторов new/delete,
           обеспечивающую последовательное обращение к функциям
           выделения/освобождения памяти из разных потоков.
           \en The variable enables overloading of new/delete operators
           which provides sequential access to the allocation/deallocation functions
           from different threads. \~
  \details \ru Переменная включает перегрузку операторов new/delete,
           обеспечивающую последовательное обращение к функциям
           выделения/освобождения памяти из разных потоков.
           \en The variable enables overloading of new/delete operators
           which provides sequential access to the allocation/deallocation functions
           from different threads. \~
\ingroup Base_Tools_IO
*/
// ---
#define __OVERLOAD_MEMORY_ALLOCATE_FREE_

#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
  //----------------------------------------------------------------------------------------
  /// \ru Объявление функций new, delete и операторов доступа. \en Declaration of functions new, delete and access operators.  \~ \ingroup Base_Tools_IO
  // \ru операторы * и -> автоматически не перегружаются, \en operators * and -> are not overloaded automatically,  
  // \ru для их использования нужно писать примерно так: \n \en one should write like this to use them: \n 
  // \ru вместо ptr->F();   ptr->operator ->()->F();     \n \en instead of ptr->F();   ptr->operator ->()->F();     \n 
  // \ru или                ptr->operator *().F();       \n \en or                ptr->operator *().F();       \n 
  // \ru или                ptr->operator Class*()->F(); \n \en or                ptr->operator Class*()->F(); \n 
  // \ru Для ссылок так же. \en Similarly for references. 
  // \ru Реализация находится в макросе IMP_PERSISTENT_NEW_DELETE_CLASS. \en The implementation is located in the macros IMP_PERSISTENT_NEW_DELETE_CLASS.
  // ---
#define DECLARE_NEW_DELETE_CLASS( Class )

//--------------------------------------------------------------------------------------
/// \ru Объявление операторов new и delete, обеспечивающих последовательное обращение
/// к функциям выделения/освобождения памяти из разных потоков.
/// \en Declaration of new and delete operators which provide sequential access
/// to the allocation/deallocation functions from different threads.  \~
/// \ru Реализация находится в макросе IMP_PERSISTENT_NEW_DELETE_CLASS_EX. \en The implementation is located in the macros IMP_PERSISTENT_NEW_DELETE_CLASS_EX.
/// \ingroup Base_Tools_IO
// ---
#define DECLARE_NEW_DELETE_CLASS_EX( Class )       \
    public:                                        \
    void  * operator new       ( size_t );         \
    void    operator delete    ( void *, size_t ); \
    void  * operator new    [] ( size_t );         \
    void    operator delete [] ( void * );         

#else // __DEBUG_MEMORY_ALLOCATE_FREE_
  //--------------------------------------------------------------------------------------
  /// \ru Объявление функций new, delete и операторов доступа. \en Declaration of functions new, delete and access operators.  \~ \ingroup Base_Tools_IO
  // \ru Реализация находится в макросе IMP_PERSISTENT_NEW_DELETE_CLASS. \en The implementation is located in the macros IMP_PERSISTENT_NEW_DELETE_CLASS.
  // ---
#define DECLARE_NEW_DELETE_CLASS( Class )

#if defined(__OVERLOAD_MEMORY_ALLOCATE_FREE_) && !defined(C3D_DEBUG)

  //--------------------------------------------------------------------------------------
  /// \ru Объявление операторов new и delete, обеспечивающих последовательное обращение
  /// к функциям выделения/освобождения памяти из разных потоков.
  /// Перегружаются все стандартные операторы new и delete.
  /// \en Declaration of new and delete operators which provide sequential access
  /// to the allocation/deallocation functions from different threads.
  /// All standard new and delete operators are overloaded. \~
  /// \ru Реализация находится в макросе IMP_PERSISTENT_NEW_DELETE_CLASS_EX.
  /// \en The implementation is located in the macros IMP_PERSISTENT_NEW_DELETE_CLASS_EX.
  /// \ingroup Base_Tools_IO
  // ---
#define DECLARE_NEW_DELETE_CLASS_EX( Class )                              \
    public:                                                               \
    void * operator new       ( size_t );                                 \
    void * operator new       ( size_t, const std::nothrow_t & ) throw(); \
    void * operator new       ( size_t, void * );                         \
    void * operator new []    ( size_t );                                 \
    void * operator new []    ( size_t, const std::nothrow_t & ) throw(); \
    void * operator new []    ( size_t, void * );                         \
    void   operator delete    ( void * );                                 \
    void   operator delete    ( void *, const std::nothrow_t & ) throw(); \
    void   operator delete    ( void *, void* );                          \
    void   operator delete [] ( void * );                                 \
    void   operator delete [] ( void *, const std::nothrow_t & ) throw(); \
    void   operator delete [] ( void *, void * );

#else // __OVERLOAD_MEMORY_ALLOCATE_FREE_

  //--------------------------------------------------------------------------------------
  /// \ru Объявление операторов new и delete, обеспечивающих последовательное обращение
  /// к функциям выделения/освобождения памяти из разных потоков.
  /// \en Declaration of new and delete operators which provide sequential access
  /// to the allocation/deallocation functions from different threads.  \~
  /// \ru Реализация находится в макросе IMP_PERSISTENT_NEW_DELETE_CLASS_EX.
  /// \en The implementation is located in the macros IMP_PERSISTENT_NEW_DELETE_CLASS_EX.
  /// \ingroup Base_Tools_IO
  // ---
#define DECLARE_NEW_DELETE_CLASS_EX( Class )

#endif // __OVERLOAD_MEMORY_ALLOCATE_FREE_
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_


//----------------------------------------------------------------------------------------
/**
  \brief \ru Объявление дружественных операторов чтения и записи указателей и ссылок.
         \en Declaration of friend operators of reading and writing of pointers and references. \~
         \ru Реализация находится в макросе IMP_PERSISTENT_OPS_BASE.
         \en The implementation is located in the macros IMP_PERSISTENT_OPS_BASE.
  \ingroup Base_Tools_IO
*/
// ---
#define DECLARE_PERSISTENT_OPS_BASE( Class, DLLFUNC )                                                     \
  friend DLLFUNC reader & CALL_DECLARATION operator >> ( reader & in, Class & ref );        \
  friend DLLFUNC reader & CALL_DECLARATION operator >> ( reader & in, Class *& ptr );       \
  friend DLLFUNC reader & CALL_DECLARATION operator >> ( reader & in, const Class *& ptr ); \
  friend DLLFUNC writer & CALL_DECLARATION operator << ( writer & out, const Class & ref ); \
  friend DLLFUNC writer & CALL_DECLARATION operator << ( writer & out, const Class * ptr ); \
  friend DLLFUNC writer & CALL_DECLARATION operator << ( writer & out, Class & ref );       \
  friend DLLFUNC writer & CALL_DECLARATION operator << ( writer& out, Class * ptr );        \


//----------------------------------------------------------------------------------------
/// \ru Конструктор для потокового класса. \en Constructor for a stream class. \~ \ingroup Base_Tools_IO
/// \ru Реализация должна быть обеспечена пользователем.
/// \en The implementation should be provided by the user.
// ---
#define DECLARE_PERSISTENT_CTOR( Class )  \
  public:                                 \
    Class( TapeInit )


//----------------------------------------------------------------------------------------
/// \ru Функции чтения и записи. \en Functions of reading and writing. \~ \ingroup Base_Tools_IO
/// \ru Реализация должна быть обеспечена пользователем.
/// \en The implementation should be provided by the user.
// ---
#define DECLARE_PERSISTENT_FUNCS( Class )                 \
  public:                                                 \
    static void Read ( reader & in,  Class * obj );       \
    static void Write( writer & out, const Class * obj )

//------------------------------------------------------------------------------
/// \ru Функции получения дескриптора класса. \~ \ingroup Base_Tools_IO
/// \ru Реализация находится в макросе IMP_CLASS_DESC_FUNC.
/// \en The implementation is located in the macros IMP_CLASS_DESC_FUNC.
// ---
#define DECLARE_CLASS_DESC_FUNC( Class )      \
  public:                                     \
    ClassDescriptor GetClassDescriptor( const VersionContainer & ) const override;


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
   \ru Реализация находится в макросе IMP_PERSISTENT_CLASS_OPS.
   \en The implementation is located in the macros IMP_PERSISTENT_CLASS_OPS.
  \ingroup Base_Tools_IO
*/
// ---
#define DECLARE_PERSISTENT_CLASS_BASE( Class, DLLFUNC ) \
  DECLARE_PERSISTENT_FUNCS( Class );                    \
  DECLARE_PERSISTENT_OPS_BASE( Class, DLLFUNC );        \
  DECLARE_PERSISTENT_CTOR( Class );                     \
  DECLARE_NEW_DELETE_CLASS( Class );                    \
  DECLARE_CLASS_DESC_FUNC(Class)


/** \brief \ru Аналог макроса DECLARE_PERSISTENT_CLASS_BASE
           с возможностью перегрузки операторов new/delete,
           обеспечивающий последовательное обращение к функциям
           выделения/освобождения памяти из разных потоков.
           \en Analog of DECLARE_PERSISTENT_CLASS_BASE macro
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
   \ru Реализация находится в макросе IMP_PERSISTENT_CLASS_NEW_DEL_OPS.
   \en The implementation is located in the macros IMP_PERSISTENT_CLASS_NEW_DEL_OPS.
\ingroup Base_Tools_IO
*/
// ---
#define DECLARE_PERSISTENT_CLASS_NEW_DEL_BASE( Class, DLLFUNC ) \
  DECLARE_PERSISTENT_CLASS_BASE( Class, DLLFUNC )               \
  DECLARE_NEW_DELETE_CLASS_EX( Class )


//----------------------------------------------------------------------------------------
/**
  \brief \ru Операторы чтения и записи указателей и ссылок.
         \en  Operators of reading and writing of pointers and references. \~
  \ingroup Base_Tools_IO
*/
// ---
#define IMPL_PERSISTENT_OPS( Class )


#endif // __IO_BASE_H
