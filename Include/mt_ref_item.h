//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file 
  \brief  \ru Надкласс для объектов, время жизни которых автоматически регулируется счетчиком ссылок.
          \en Superclass for objects their lifetime is automatically regulated by reference counter.
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MT_REF_ITEM_H
#define  __MT_REF_ITEM_H

#include <math_define.h>

#if defined (C3D_WINDOWS ) && !defined(ALL_WARNINGS)  //_MSC_VER // Set warnings level
#pragma warning(disable: 4275)  //AP non dll-interface class '1' used as base for dll-interface class '2' (deriving exported from non-exported)
#endif


//----------------------------------------------------------------------------------------
/** \brief  \ru Базовый класс для объектов с подсчетом ссылок.
            \en Base class for objects with reference counting. \~
    \ingroup Base_Items
    \sa #MbRefItem, #MtRefItem, #SPtr
*/
//---
template<class Derived>
class MtRefBase
{
  mutable refcount_t useCount;

protected:
  MtRefBase() : useCount(0) {}
 ~MtRefBase() {}

public: 
  /// \ru Добавить одну ссылку на объект. \en Adds a reference to this object. 
  refcount_t  AddRef()  const { return ++useCount; }
  /// \ru Освободить одну ссылку на объект. \en Releases a reference to this object.
  refcount_t  Release() const;

public:
  /// \ru Вернуть количество объектов, ссылающихся на данный. \en Returns a number of objects referring to this.
  refcount_t  GetUseCount() const { return useCount; }

  OBVIOUS_PRIVATE_COPY( MtRefBase );
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Базовый класс для объектов с подсчетом ссылок.
          \en Base class for objects with reference counting. \~
    \ingroup Base_Items
    \sa #MbRefItem, #SPtr
    \note It is used as an ancestor class for ATS-interfaces.
    \attention  
    \ru Данный класс имеет консервативную реализацию подсчета ссылок, т.е. 
  требует особых мер для работы в многопоточном режиме. Другой класс с аналогичными функциями 
  #MbRefItem предусматривает обращение из разных потоков.
    \en This class has a conservative implementation of reference counting, i.e. requires 
  special tricks to work in multi-threaded mode. Another class with similar functions, 
  #MbRefItem provides calling in different theads.
*/
//---
class MtRefItem
{
  mutable refcount_t useCount;

protected:
           MtRefItem() : useCount(0) {}
  virtual ~MtRefItem() {} // Деструктор всех наследников должен быть закрыт. Экземпляры данного класса хранить в SPtr.

public: 
  /// \ru Добавить одну ссылку на объект. \en Adds a reference to this object.
  refcount_t  AddRef()  const { return ++useCount; }
  /// \ru Освободить одну ссылку на объект. \en Releases a reference to this object.
  refcount_t  Release() const;

public:
  /// \ru Вернуть количество объектов, ссылающихся на данный. \en Returns a number of objects referring to this.
  refcount_t  GetUseCount() const { return useCount; }

  OBVIOUS_PRIVATE_COPY( MtRefItem );
};

inline refcount_t MtRefItem::Release() const 
{
  if ( --useCount < 1 )
  {
    PRECONDITION( useCount == 0 );    
    delete this; // SKIP_SA // Call of virtual destructor.
    return 0;
  }

  return useCount;
}

template<class Derived>
refcount_t MtRefBase<Derived>::Release() const
{ 
  if ( --useCount < 1 )
  {
    PRECONDITION( useCount == 0 );
    delete static_cast<const Derived*>( this );
    return 0;                            
  }

  return useCount;
}                      

#endif // __MT_REF_ITEM_H

// eof