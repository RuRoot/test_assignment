////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Автоматический указатель.
         \en Smart pointer. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_POINTER_H
#define __TEMPL_POINTER_H


#include <io_define.h>


////////////////////////////////////////////////////////////////////////////////
//
// A pair of smart pointer template classes. Provides basic conversion
// operator to T*, as well as dereferencing (*), and 0-checking (!).
// These classes assume that they alone are responsible for deleting the
// object or array unless Relinquish() is called.
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
/** \brief \ru Базовый класс автоматического указателя.
           \en Base class of smart pointer. \~
  \details \ru Базовый класс автоматического указателя. \n
           \en Base class of smart pointer. \n \~
  \ingroup Base_Tools_SmartPointers
*/
// ---
template<class T>
class TPointerBase {
public:
  T & operator  * () const { return *P; }
      operator T* () const { return P; }

  int     operator  ! () const { return (P == nullptr);}
  T *     Relinquish() {T * p = P; P = nullptr; return p;}

  T *           Get()        { return P; }
  const T *     Get() const  { return P; }

protected:
  TPointerBase( T * pointer ) : P(pointer) {}
  TPointerBase() : P( nullptr ) {}
protected:
  T * P;
private:
  void *  operator new( size_t ); // prohibit use of new
  void    operator delete( void * p ) { (static_cast<TPointerBase<T>*>(p))->P = nullptr; }

// СМВ К15 MVS 2012
private:
  TPointerBase( const TPointerBase<T> & other );
public:
  TPointerBase( TPointerBase<T> && _Right ): P( _Right.P ) { _Right.P = nullptr; }
  TPointerBase<T> & operator = ( TPointerBase<T> && _Right ) 
  { 
    if ( this != &_Right ) 
    { P = _Right.P; _Right.P = nullptr; }
    return *this; 
  }
};


//------------------------------------------------------------------------------
/** \brief \ru Автоматический указатель на объект.
           \en A smart pointer to an object. \~
  \details \ru Автоматический указатель на объект. 
    Обеспечивает доступ через оператор "->". \n
           \en A smart pointer to an object. 
    Provides an access by the operator "->". \n \~
  \ingroup Base_Tools_SmartPointers
*/
// ---
template<class T>
class TPointer : public TPointerBase<T> {
public:
  TPointer() : TPointerBase<T>() {}
  TPointer( T * pointer ) : TPointerBase<T>( pointer ) {}
 ~TPointer()
 {
   delete TPointerBase<T>::P;
 }
public:
  TPointer<T> & operator = ( T * src )
  { 
    if ( src != TPointerBase<T>::P )
    {
      delete TPointerBase<T>::P;
      TPointerBase<T>::P = src;
    }
    return *this;
  }
  T *           operator ->()          { return TPointerBase<T>::P; }  // Could throw exception if P==nullptr
  const T *     operator ->() const    { return TPointerBase<T>::P; }  // Could throw exception if P==nullptr

// СМВ К15 MVS 2012
#ifndef __MOBILE_VERSION__
private:
#endif // __MOBILE_VERSION__
  TPointer( const TPointer<T> & other );

public:
  TPointer( TPointer<T> && _Right )
    : TPointerBase<T>( std::move(_Right) )
  {  }
  TPointer<T> & operator = ( TPointer<T> && _Right ) 
  {
    if ( this != &_Right )
    {
      delete TPointerBase<T>::P;
      TPointerBase<T>::P = _Right.P;
      _Right.P = nullptr;
    }
    return *this;
  }
};


//------------------------------------------------------------------------------
/** \brief \ru Автоматический указатель на объект.
           \en A smart pointer to an object. \~
  \details \ru Автоматический указатель на объект. 
    Обеспечивает доступ через оператор "->".
    Есть флаг владения объектом. \n
           \en A smart pointer to an object. 
    Provides an access by the operator "->".
    There is a flag of ownership of object. \n \~
  \ingroup Base_Tools_SmartPointers
*/
// ---
template<class T>
class TOwnPointer : public TPointerBase<T> {
  bool own;  // own = false - no delete
public:
  TOwnPointer() : TPointerBase<T>(), own(true) {}
  TOwnPointer( T * pointer ) : TPointerBase<T>(pointer), own(true) {}
  ~TOwnPointer()
  { 
    if ( own )
      delete TPointerBase<T>::P;
  }
public:
  TOwnPointer<T> & operator = ( T * src )
  { 
    if ( src != TPointerBase<T>::P )
    {
      if ( own )
        delete TPointerBase<T>::P;
      TPointerBase<T>::P = src;
    }
    return *this;
  }
  T *       operator ->() { return TPointerBase<T>::P; }  // Could throw exception if P==nullptr
  bool      GetOwn() const { return own; }
  void      SetOwn( bool val ) { own = val; }

// СМВ К15 MVS 2012
private:
  TOwnPointer( const TOwnPointer<T> & other );
  TOwnPointer<T> & operator = ( const TOwnPointer<T> & _Right );
public:
  TOwnPointer( TOwnPointer<T> && _Right ) 
    : TPointerBase<T>( std::move(_Right) )
    , own ( std::move(_Right.own) )
  {  }
  TOwnPointer<T> & operator = ( TOwnPointer<T> && _Right )
  {
    if ( this != &_Right )
    {
      if ( own )
        delete TPointerBase<T>::P;
      TPointerBase<T>::P = _Right.P;
      own = _Right.own;
      _Right.P = nullptr;
    }
    return *this;
  }
};


//------------------------------------------------------------------------------
/** \brief \ru Автоматический указатель на массив объектов.
           \en A smart pointer to an array of objects. \~
  \details \ru Автоматический указатель на массив объектов.
    Обеспечивает доступ к элементам массива по индексу.
    Удаляет массив через delete[]. \n
           \en A smart pointer to an array of objects.
    Provides an access to elements of array by index.
    Deletes an array by the operator delete[]. \n \~
  \ingroup Base_Tools_SmartPointers
*/
// ---
template<class T>
class TAPointer : public TPointerBase<T> {
public:
  TAPointer() : TPointerBase<T>() {}
  TAPointer( T* array ) : TPointerBase<T>( array ) {}
 ~TAPointer()
 {
   delete[] TPointerBase<T>::P;
 }
public:
  TAPointer<T> &  operator = ( T * src )
  {
    if ( src != TPointerBase<T>::P )
    {
      delete[] TPointerBase<T>::P;
      TPointerBase<T>::P = src;
    }
    return *this;
  }
  T &             operator []( size_t i ) { return TPointerBase<T>::P[i]; }  // Could throw exception if P==nullptr

// СМВ К15 MVS 2012
//private: // g++4.7 KUbuntu 
  TAPointer( const TAPointer<T> & other );
public:
  TAPointer(TAPointer<T> && _Right)
    : TPointerBase<T>( std::move(_Right) )
  {  }
  TAPointer<T> & operator = (TAPointer<T> && _Right)
  {
    if (this != &_Right)
    {
      delete[] TPointerBase<T>::P;
      TPointerBase<T>::P = _Right.P;
      _Right.P = nullptr;
    }
    return *this;
  }
};


//------------------------------------------------------------------------------
// Obsolete, should use TAPointer<char> for char[]'s
//---
//------------------------------------------------------------------------------
/** \brief \ru Автоматический указатель на массив символов.
           \en A smart pointer to an array of symbols. \~
  \details \ru Автоматический указатель на массив символов. \n
           \en A smart pointer to an array of symbols. \n \~
  \warning \ru Устаревший класс, пользуйтесь TAPointer<char>
           \en This class is out-of-date, use TAPointer<char> \~
  \ingroup Base_Tools_SmartPointers
*/
// ---
template <>
class TPointer<char> : public TPointerBase<char> {
public:
  TPointer() : TPointerBase<char>() {}
  TPointer( char pointer[] ) : TPointerBase<char>( pointer ) {}
 ~TPointer() { delete[] P; }
public:
  char * operator = ( char src[] ) { delete[] P; return P = src; }

  char * operator = ( const TPointer<char> & src )
  {
    if ( &src != this ) {
      delete[] P;
      return P = src.P;
    }
    return nullptr;
  }

  char & operator []( size_t i ) { return P[i]; }

public:
  TPointer( TPointer<char> && _Right ) 
    : TPointerBase<char>( std::move(_Right) )
  {  }
  TPointer<char> & operator = ( TPointer<char> && _Right )
  {
    if ( this != &_Right )
    {
      delete[] P;
      P = _Right.P;
      _Right.P = nullptr;
    }
    return *this;
  }
};


#endif // __TEMPL_POINTER_H
