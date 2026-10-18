////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Автоматический указатель на объекты, не имеющие счетчиков ссылок.
         \en Smart pointer to objects without reference counters. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef  __TEMPL_DPTR_H
#define  __TEMPL_DPTR_H

#include <math_define.h>
#include <system_types.h>

//------------------------------------------------------------------------------
/** \brief \ru Автоматический указатель.
           \en Smart pointer. \~
  \details \ru Автоматический указатель на объекты, не имеющие счетчиков ссылок. \n
           \en Smart pointer to objects without reference counters. \n \~
  \ingroup Base_Tools_SmartPointers
*/
// ---
template <class T>
class DPtr {
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  DPtr() : m_Ptr ( nullptr ), m_Owner( nullptr ) {}
  /// \ru Конструктор по указателю на объект. \en Constructor by pointer to an object. 
  /*explicit*/ DPtr( T * obj );
  /// \ru Конструктор по автоматическому указателю на объект. \en Constructor by smart pointer to an object. 
  DPtr( const DPtr<T> & );
  /// \ru Деструктор. \en Destructor. 
 ~DPtr() { reset(); }

public:
  /// \ru Оператор доступа. \en An access operator. 
            operator  T* ( void ) const { return m_Ptr; }
  /// \ru Оператор доступа. \en An access operator. 
  T       & operator *  ( void ) const { return *m_Ptr; } // SKIP_SA
  /// \ru Оператор доступа. \en An access operator. 
  T       * operator -> ( void ) const { return  m_Ptr; }
  /// \ru Оператор присваивания. \en The assignment operator. 
  DPtr<T> & operator = ( T * );
  /// \ru Оператор присваивания. \en The assignment operator.
  DPtr<T> & operator =  ( const DPtr<T> & src );

public:
  /// \ru Функция освобождения объекта. \en A function of release an object. 
  DPtr<T> &   reset();
  /// \ru Выдать указать. \en Get pointer. 
  const T *   get() const { return m_Ptr; }
  /// \ru Выдать указать. \en Get pointer. 
        T *   get() { return m_Ptr; }
  /// \ru Выдать количество владеющих экземпляров DPtr. \en Get the number of DPtr objects that share ownership.
  refcount_t  use_count() const { return m_Owner ? m_Owner->use_count() : 0; }

  /// \ru Конструктор перемещения. \en Moving constructor.
  DPtr( DPtr<T> && src ) : m_Ptr( src.m_Ptr ), m_Owner( src.m_Owner )
  {
    src.m_Ptr = nullptr;
    src.m_Owner = nullptr;
  }
  /// \ru Оператор перемещения. \en Moving operator.
  DPtr<T> & operator = ( DPtr<T> && src )
  {
    std::swap( m_Ptr, src.m_Ptr );
    std::swap( m_Owner, src.m_Owner );
    return *this;
  }

private:
  /// \ru Счетчик ссылок на объект. \en A counter of references to an object. 
  struct Owner // Поведение аналогично MtRefItem.
  {
    Owner( T * obj )
      : m_Ptr( obj )
      , useCount( 0 )
    { 
      PRECONDITION( obj != nullptr );
    }
    void AddRef() const { ++useCount; }
    void Release() const
    {
      PRECONDITION( useCount > 0 );
      if ( --useCount == 0 )
      {
        delete this;
      }
    }
    const T *   get() const { return m_Ptr; }
          T *   get()       { return m_Ptr; }
    refcount_t  use_count() const { return useCount; }
    OBVIOUS_PRIVATE_COPY( Owner );

  private:   
    T *                 m_Ptr;
    mutable refcount_t  useCount;
   ~Owner() { PRECONDITION( useCount == 0 ); delete m_Ptr; } // Destructor should be private.
  };

private:
  T     * m_Ptr;    ///< \ru Указатель на объект. \en A pointer to an object. 
  Owner * m_Owner;  ///< \ru Счетчик ссылок на объект. \en A counter of references to an object. 
};

//-------------------------------------------------------------------------------
// \ru Конструктор по указателю на объект. \en Constructor by pointer to an object. 
// ---
template <class T>
DPtr<T>::DPtr( T * obj )
  : m_Ptr( obj )
  , m_Owner( nullptr )
{
  if ( obj != nullptr ) 
  {
    m_Owner = new Owner( obj );
    m_Owner->AddRef();
  }
}

//-------------------------------------------------------------------------------
// \ru Конструктор копирования. \en Copy constructor. 
// ---
template <class T>
DPtr<T>::DPtr( const DPtr<T> & dptr )
  : m_Ptr( dptr.m_Ptr )
  , m_Owner( dptr.m_Owner )
{
  if ( m_Owner != nullptr )
    m_Owner->AddRef(); // SKIP_SA
}

//-------------------------------------------------------------------------------
// \ru Оператор присваивания. \en Assignment operator. 
// ---
template <class T>
DPtr<T> & DPtr<T>::operator = ( T * pObj )
{
  if ( m_Ptr != pObj ) 
  {    
    reset();
    if ( pObj != nullptr ) 
    {
      m_Ptr = pObj;
      m_Owner = new Owner( pObj );
      m_Owner->AddRef();
    }
  }  

  return *this;
}

//-------------------------------------------------------------------------------
// \ru Оператор присваивания. \en Assignment operator. 
// ---
template <class T>
DPtr<T> & DPtr<T>::operator = ( const DPtr<T> & dptr )
{
  if ( m_Ptr != dptr.m_Ptr ) 
  {    
    reset();
    if ( dptr.m_Ptr != nullptr ) 
    {
      m_Ptr = dptr.m_Ptr;
      m_Owner = dptr.m_Owner;
      m_Owner->AddRef(); // SKIP_SA
    }
  }
  
  return *this;
}

//-------------------------------------------------------------------------------
// \ru Функция освобождения объекта. \en A function of release an object. 
//---
template <class T>
DPtr<T> & DPtr<T>::reset() 
{ 
  if ( m_Ptr == nullptr ) 
    return *this;

  m_Ptr = nullptr; 
  m_Owner->Release(); // SKIP_SA
  m_Owner = nullptr;
  return *this; 
}

//-------------------------------------------------------------------------------
//
//---
template <class T1, class T2>
bool operator == ( const DPtr<T1> & left, const DPtr<T2> & right ) { return left.get() == right.get(); }
template <class T1, class T2>
bool operator != ( const DPtr<T1> & left, const DPtr<T2> & right ) { return left.get() != right.get(); }

/*
//-------------------------------------------------------------------------------
//
//---
template <class T1, class T2>
bool operator == ( const DPtr<T1> & left, const T2 * right ) { return left.get() == right; }
template <class T1, class T2>
bool operator != ( const DPtr<T1> & left, const T2 * right ) { return left.get() != right; }
//-------------------------------------------------------------------------------
//
//---
template <class T1, class T2>
bool operator == ( const T1 * left, const DPtr<T2> & right ) { return left == right.get(); }
template <class T1, class T2>
bool operator != ( const T1 * left, const DPtr<T2> & right ) { return left != right.get(); }
*/

#endif // __TEMPL_DPTR_H
