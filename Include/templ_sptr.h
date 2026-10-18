////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Автоматические указатель и ссылка на объекты с подсчетом ссылок.
         \en Smart pointer and reference to objects with reference counter. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_SPTR_H
#define __TEMPL_SPTR_H

#include <system_cpp_standard.h>
#include <math_define.h>


//------------------------------------------------------------------------------
/** \brief \ru Отладочная проверка на nullptr.
           \en Debug check for nullptr. \~
  \details \ru Отладочная проверка на nullptr. \n
           \en Debug check for nullptr. \n \~
  \ingroup Base_Tools_SmartPointers
*/
// ---
#define NULL_CHECK PRECONDITION( m_pI != nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Автоматический указатель на объекты с подсчетом ссылок.
           \en Smart pointer to objects with reference counter. \~
  \details \ru Автоматический указатель (smart pointer) на объекты с подсчетом ссылок.
           Требует от параметра шаблона реализации функций AddRef() и Release(). \n
           \en Smart pointer to objects with reference counter.
           It requires Implementation of functions AddRef() and Release() from the template parameter. \n \~
  \ingroup Base_Tools_SmartPointers
*/
// ---
template <class T>
class SPtr 
{
  T * m_pI;   // \ru Указатель на объект. \en A pointer to an object.

public:  
  /// \ru Конструктор. \en Constructor. 
  SPtr () : m_pI( nullptr ) {}

  /// \ru Конструктор по нулевому указателю. \en Constructor by the null pointer. 
  SPtr( std::nullptr_t ) : m_pI( nullptr ) {}
  /// \ru Конструктор по указателю. \en Constructor by pointer. 
  explicit SPtr ( T * elem )
  {
    if ( (m_pI = elem) != nullptr )
      m_pI->AddRef();
  }
  /// \ru Конструктор копирования. \en Copy constructor.   
  SPtr( const SPtr<T> & ptr ) : m_pI( nullptr ) { assign(ptr.m_pI); } // SKIP_SA
  /// \ru Конструктор по совместимому указателю \en Constructor by compatible pointer 
  template<class _T>
  SPtr( const SPtr<_T> & ptr ) : m_pI( nullptr ) { assign(ptr.get()); }
  /// \ru Деструктор. \en Destructor. 
 ~SPtr() { reset(); }

public: // \ru Перегрузка операторов \en Operators overloading 
  /// \ru Оператор преобразования к типу T* . \en An operator for conversion to the type T*. 
  operator T* ( void ) const { return m_pI; } // SKIP_SA
  /// \ru Оператор преобразования к совместимому указателю. \en An operator for conversion to a compatible pointer. 
  /*
  template<class _T>
  operator SPtr<_T> () const { return SPtr<_T>( m_pI ); }  
  */
  /// \ru Оператор доступа. \en An access operator. 
  T &       operator * () const { NULL_CHECK return *m_pI; } // SKIP_SA
  /// \ru Оператор доступа. \en An access operator. 
  T       * operator -> () const { NULL_CHECK return m_pI; } // SKIP_SA
  /// \ru Оператор присваивания. \en The assignment operator. 
  SPtr<T> & operator = ( T * elem ) { return assign( elem ); }
  /// \ru Оператор присваивания. \en The assignment operator. 
  SPtr<T> & operator = ( const SPtr<T> & ptr ) { return assign( ptr.get() ); }
  /// \ru Оператор присваивания для совместимого указателя. \en An assignment operator for a compatible pointer. 
  template<class _T>
  SPtr<T> & operator = ( const SPtr<_T> & ptr ) { return assign( ptr.get() ); }  
  /// \ru Оператор проверки на равенство. \en An operator for equality check. 
  template<class _T>
  bool      operator == ( const SPtr<_T> & ptr ) const { return ( m_pI == ptr.get() ); }
  /// \ru Оператор проверки на равенство. \en An operator for equality check. 
  template<class _T>
  bool      operator == ( const _T * elem ) const { return ( m_pI == elem ); }
  /// \ru Оператор проверки на равенство. \en An operator for equality check. 
  bool      operator == ( T * elem ) const { return ( m_pI == elem ); }
  /// \ru Оператор проверки на неравенство. \en An operator for inequality check. 
  template<class _T>
  bool      operator != ( const SPtr<_T> & ptr ) const { return ( !(operator == (ptr)) ); }
  /// \ru Оператор проверки на неравенство. \en An operator for inequality check. 
  template<class _T>
  bool      operator != ( const _T * elem ) const { return ( !(operator == (elem)) ); }
  /// \ru Оператор проверки на неравенство. \en An operator for inequality check.   
  bool      operator != ( T * elem ) const { return ( !(operator == (elem)) ); }
  /// \ru Отношение порядка. \en Order relation. 
  template<class _T>
  bool      operator < ( const _T * elem ) const { return ( m_pI < elem ); }
  /// \ru Отношение порядка. \en Order relation. 
  template<class _T>
  bool      operator < ( const SPtr<_T> & elem ) const { return ( m_pI < elem.get() ); }

public:
  /// \ru Функция присваивания указателем. \en A function of assignment by pointer. 
  template <class _T>
  SPtr<T> & assign( _T * elem )
  {
    if ( m_pI != elem ) // SKIP_SA
    {
      if ( elem != nullptr ) { elem->AddRef(); } // SKIP_SA
      if ( m_pI != nullptr ) { m_pI->Release(); } // SKIP_SA
      m_pI = elem;
    }
    return *this; // SKIP_SA
  }
  /// \ru Функция присваивания указателем. \en A function of assignment by pointer. 
  template <class _T>
  SPtr<T> & assign( const SPtr<_T> & elem ) { return assign( elem.get() ); }
  /// \ru Обменяться указателями с другим SPtr. \en Swap pointers with another SPtr.
  template <class _T>
  void      swap( SPtr<_T> & other ) { std::swap( m_pI, other.m_pI ); }
  /// \ru Функция освобождения объекта. \en A function of release an object.
  SPtr<T> & assign( std::nullptr_t ) { return reset(); }
  /// \ru Функция освобождения объекта. \en A function of release an object.
  SPtr<T> & reset( void ) { if ( m_pI != nullptr )  { m_pI->Release(); m_pI = nullptr; } return *this; } // SKIP_SA
  /// \ru Функция доступа к элементу данных. \en A function of access to data element. 
  T *       get() const { return m_pI; } // SKIP_SA
  /// \ru Функция отсоединяет объект. \en A function detaches an object. 
  T *       detach() { T * obj = m_pI; m_pI = nullptr; if ( obj != nullptr ) obj->DecRef(); return obj; } // SKIP_SA
  /// \ru Нулевой указатель? \en Is null pointer? 
  bool      is_null() const { return nullptr == m_pI; }

public:
  /// \ru Конструктор перемещения. \en Moving constructor.
  SPtr( SPtr<T> && src ) : m_pI ( src.m_pI )
  {
    src.m_pI = 0;
  }
  /// \ru Оператор перемещения. \en Moving operator.
  SPtr<T> & operator = ( SPtr<T> && src )
  {
    T * tmp = m_pI;
    m_pI = src.m_pI;
    src.m_pI = tmp;
    return *this;
  }

  /// \ru Преобразовать к SPtr на другой класс. \en Cast to SPtr to another class.
  template<typename To>
  SPtr<To> static_cast_to() const {
    return SPtr<To> { static_cast<To *>(get()) };
  }
};


//------------------------------------------------------------------------------
/** \brief \ru Автоматическая ссылка на объекты с подсчетом ссылок.
           \en Smart reference to objects with reference counter. \~
  \details \ru Автоматическая ссылка (smart reference) на объекты с подсчетом ссылок.
           Фактически тоже самое, что и SPtr, но без возможности равенства nullptr.\n
           Требует от параметра шаблона реализации функций AddRef() и Release(). \n
           \en Smart reference to objects with reference counter.
           Actually it is the same as SPtr but without the possibility of equality to nullptr \n
           It requires Implementation of functions AddRef() and Release() from the template parameter. \n \~
  \ingroup Base_Tools_SmartPointers
*/
// ---
template <class T>
class SRef 
{
  T * m_pI; ///< \ru Указатель на значение \en A pointer to the value 

public:
  /// \ru Конструктор по ссылке. \en Constructor by reference. 
  SRef ( T & ref ) : m_pI( & ref )
  {
    m_pI->AddRef();
  }
  /// \ru Конструктор копирования. \en Copy constructor. 
  SRef( const SRef & src ): m_pI( src.m_pI )  
  {
    m_pI->AddRef();
  }
  /// \ru Деструктор. \en Destructor. 
 ~SRef( void )
  {
    m_pI->Release();
  }
public:
  /// \ru Функция доступа. \en An access function. 
  T &       get() const { return *m_pI; }
  /// \ru Оператор доступа. \en An access operator. 
            operator T & ( void ) const { return *m_pI; }
  /// \ru Оператор доступа. \en An access operator. 
  T &       operator()() const { return *m_pI; }
  /// \ru Оператор проверки на равенство. \en An operator for equality check. 
  bool      operator == ( const SRef & src ) const { return ( m_pI == src.m_pI );   }
  /// \ru Оператор проверки на равенство. \en An operator for equality check. 
  bool      operator == ( T & pObj ) const { return ( m_pI == &pObj ); }
  /// \ru Оператор проверки на неравенство. \en An operator for inequality check. 
  bool      operator != ( const SRef & src ) const { return ( !(operator == (src)) );  }
  /// \ru Оператор проверки на неравенство. \en An operator for inequality check. 
  bool      operator != ( T & pObj ) const { return ( !(operator == (pObj)) ); }
  /// \ru Оператор присваивания. \en The assignment operator. 
  SRef<T> & operator = ( const SRef & ref ) { return operator =( *ref.m_pI ); }
  /// \ru Оператор присваивания. \en The assignment operator. 
  SRef<T> & operator =  ( T & obj )
  {
    obj.AddRef();
    m_pI->Release();
    m_pI = &obj;
    return *this;
  }
};


//------------------------------------------------------------------------------
/** \brief \ru Пара автоматических указателей.
           \en A pair of smart pointers. \~
  \details \ru Пара автоматических указателей. \n
    Удобно применять там, где требуется передавать в качестве аргумента 
    функции или результата пару указателей на объекты со счетчиком ссылок.
    Все данные и методы класса намеренно сделаны открытыми.
           \en A pair of smart pointers. \n
    It is comfortable to use this when it is required to pass as an argument 
    of a function or a result, a pair of smart pointers to objects with reference counters.
    All data and methods of the class are purposely made public. \~
  \ingroup Base_Tools_SmartPointers
*/
// ---
template <class RefItem >
struct SPtrPair
{
  SPtr<RefItem> first;  ///< \ru Автоматический указатель на первый объект. \en A smart pointer to the first object. 
  SPtr<RefItem> second; ///< \ru Автоматический указатель на второй объект. \en A smart pointer to the second object. 
  /// \ru Конструктор. \en Constructor. 
  SPtrPair(): first(), second() {}
  /// \ru Конструктор копирования. \en Copy constructor. 
  SPtrPair( const SPtrPair & sPair ) : first(sPair.first), second(sPair.second) {} 
  /// \ru Оператор присваивания. \en The assignment operator. 
  SPtrPair  & operator = ( const SPtrPair & sPair ) { first = sPair.first; second = sPair.second; }
};

namespace c3d {

//------------------------------------------------------------------------------
// \ru  Создать объект по параметрам и вернуть автоматический указатель на него.
// \en  Create an object by parameters and return a smart pointer to it.
//---
template<class T, class ... Args>
inline SPtr<T> make_sptr( Args && ... args )
{
  return SPtr<T>( new T(std::forward<Args>( args )...) );
}

} // namespace c3d

#endif // __TEMPL_SPTR_H
