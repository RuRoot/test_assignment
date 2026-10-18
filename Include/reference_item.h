//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Счетчик ссылок (владельцев объекта).
         \en Reference counter (of an object owners). \~

*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __REFERENCE_ITEM_H
#define __REFERENCE_ITEM_H


#include <math_define.h>
#include <system_types.h>
#include <templ_rp_array.h>
#include <templ_s_array.h>
#include <templ_sptr.h>
#include <system_cpp_standard.h>
#include <system_dependency.h>
#include <tool_memory_leaks_check.h>
#include <mb_variables.h>
#include <vector>


//----------------------------------------------------------------------------------------
/** \brief \ru Типы объекта со счетчиком ссылок.
           \en Types of object with reference counter. \~
  \details \ru Тип несёт информацию об объекте-наследнике. \n
           \en Type has an information of inheritor object. \n \~
  \ingroup Geometric_Items
*/
// ---
enum MbeRefType
{
  rt_RefItem = 0,  ///< \ru Некоторый объект. \en Some object. 
  rt_PlaneItem,    ///< \ru Двумерный геометрически объект. \en Two-dimensional geometric object. 
  rt_SpaceItem,    ///< \ru Трехмерный геометрический объект. \en Three-dimensional geometric object. 
  rt_TopItem,      ///< \ru Топологический объект. \en A topological object. 
  rt_Creator,      ///< \ru Строитель объекта. \en Object constructor 
  rt_Attribute,    ///< \ru Атрибут объекта. \en Attribute of an object.  
  rt_Primitive,    ///< \ru Элемент полигонального объекта. \en Element of polygonal object.
  // \ru В конец можно добавлять новые нужные \en It is possible to add new necessary ones to the end 
};


class  MbRefItem;
class  MbRegDuplicate;
class  DupRegArray;
namespace c3d // namespace C3D
{
typedef SPtr<MbRefItem>                       RefItemSPtr;
typedef SPtr<const MbRefItem>                 ConstRefItemSPtr;

typedef std::vector<MbRefItem *>              RefItemsVector;
typedef std::vector<const MbRefItem *>        ConstRefItemsVector;

typedef std::vector<RefItemSPtr>              RefItemsSPtrVector;
typedef std::vector<ConstRefItemSPtr>         ConstRefItemsSPtrVector;
}

//----------------------------------------------------------------------------------------
/** \brief \ru Объект с подсчетом ссылок.
           \en Reference-counted object.  \~
  \details \ru Объект, считающий количество своих владельцев. \n
      Используется в качестве одного из родительских классов геометрических объектов. \n
      Если экземпляр наследника от данного класса захватывается другим объектом или алгоритмом,
  то другой объект или алгоритм должен увеличить счетчик ссылок на единицу методом AddRef().
  При освобождении экземпляра данного класса другим объектом (например, при деструктурировании указателя SPtr) 
  или алгоритмом другой объект или алгоритм должны вызвать метод Release() для уменьшения счетчика 
  ссылок. Такое правило позволяет гарантировать действительность экземпяра класса, пока существует 
  хотя бы один его прользователь.\n              
           \en Object counting number of its owners. \n
      Is used as one of parent classes of geometric objects. \n
      If an instance inheriting this class is captured by other object or used in algorithms, 
  then the other object or algorithm has to increase reference counter by one by AddRef() method.
  When an instance of this class is released by another object (for example, when the SPtr pointer is destructed) 
  or by the using algorithm, the using object or algorithm must call the Release() method to decrease 
  the reference count. Such rule allows you to guarantee the validity of the MbRefItem 
  instance as long as there is at least one of its users.\n\~

  \note \ru Рекомендуется применение автоматических указателей типа SPtr к экземплярам 
    данного класса. Это упростит работу с кодом, где нужно позаботится об автоматической 
    сборке мусора. 
        \en It is recommended to use smart pointers of type SPtr to instances of 
  this class. This will simplify the work with the code where you want to take care of 
  the automatic garbage collection. \~
  \sa #SPtr
  \ingroup Geometric_Items
*/
// ---
#ifndef ENABLE_MEMORY_LEAKS_CHECK
class MATH_CLASS MbRefItem {
#else
class MATH_CLASS MbRefItem: virtual public c3d::MemoryLeaksVerifiable {
#endif
private:
  mutable use_count_type useCount; ///< \ru Счетчик ссылок на объект, изменяемый владельцами объекта. \en A counter of references to an object modifiable by owners of object.  
  mutable DupRegArray *  dupRegs;  ///< \ru Регистраторы копирования, в которых зарегистрирован данный объект. \en Duplicate registrators in which the object is registered.  
protected:  
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  MbRefItem();
public:
  virtual ~MbRefItem();

public:
  /** \ru \name Функции регистрации ссылок на геометрический объект владельцами объекта.
      \en \name Functions for registration of references to geometric object by owners of object.
      \{ */
  /// \ru Выдать количество ссылок (выдать количество владельцев объекта). \en Get count of references (get count of owners of an object). 
  refcount_t GetUseCount() const;
  /// \ru Увеличить количество ссылок на единицу. \en Increase count of references by one. 
  refcount_t AddRef() const;
  /// \ru Уменьшить количество ссылок на единицу. \en Decrease count of references by one. 
  refcount_t DecRef() const;
  /// \ru Уменьшить количество ссылок на единицу и, если количество ссылок стало равным нулю, удалить себя. \en Decrease count of references by one and if count of references became zero, then remove itself. 
  refcount_t Release() const; 
  /** \} */

  /// \ru Регистрационный тип (для копирования, дублирования). \en Registration type (for copying, duplication). 
  virtual MbeRefType RefType() const; 

  /// \ru Добавить регистратор. \en Add the register.
  void AddRegistrator( MbRegDuplicate * reg ) const;
  /// \ru Удалить регистратор. \en Remove the register.
  void RemoveRegistrator( MbRegDuplicate * reg ) const;

OBVIOUS_PRIVATE_COPY( MbRefItem )
};


//----------------------------------------------------------------------------------------
/** \brief \ru Шаблон класса сериализации (порядковой нумерации создаваемых объектов).
           \en Serialization class template (ordinal numbering of created objects).  \~
  \details \ru Шаблон класса сериализации (порядковой нумерации создаваемых объектов). \n
               Применение: наследовать от данного класса.
               Аргумент шаблона позволяет иметь несколько сериализаций одновременно: \n
               если нужна сериализация объектов класса T, можно записать class T: virtual public MbSerialItem<T> \n
               если объекты U должны иметь ту же сериализацию, можно записать class U: virtual public MbSerialItem<T> \n
               или же создать пустой класс для сериализации типа class MbBasicSerializer {}
               и наследоваться от MbSerialItem<MbBasicSerializer>. \n
              
           \en Serialization class template (ordinal numbering of created objects). \n
               How to use: inherit from this class. \n
               The template argument allows multiple serializations at the same time: \n
               if you need serialization of objects of class T, you can write class T: virtual public MbSerialItem <T> \n
               if U objects must have the same serialization, you can write a class U: virtual public MbSerialItem <T> \n
               or you can create an empty class to serialize the class type MbBasicSerializer \n
               and inherit it from MbSerialItem <MbBasicSerializer>. \n \~

  \ingroup Geometric_Items
*/
// ---
template <typename Type> class MbSerialItem {
private:
  static  serial_type   serialLast;  ///< \ru Индекс последнего созданного объекта (0 в случае отсутствия объектов). \en Index of the last object created (0 if there are no objects).  
  static  serial_type   serialCount; ///< \ru Счетчик объектов. \en Counter of objects.
          serial_type   serialThis;  ///< \ru Уникальный порядковый номер объекта, выдается при создании. В дальнейшем не меняется. \en The unique sequence number of the object, issued at creation. In the future it is not changed. 
public:
  MbSerialItem(); ///< \ru Конструктор. \en Constructor. 
protected:
  virtual ~MbSerialItem(); ///< \ru Деструктор. \en Destructor. 
public:
  size_t  GetSerial() const { return SerialTypeValue( serialThis ); } ///< \ru Выдать порядковый номер объекта. \en Get object serial identifier.

OBVIOUS_PRIVATE_COPY( MbSerialItem )
};


// \ru Инициализация статических переменных шаблона класса сериализации. \en Initializing static variables of the serialization class template.
template <typename Type> serial_type MbSerialItem<Type>::serialLast( 0 );
template <typename Type> serial_type MbSerialItem<Type>::serialCount( 0 );


//-----------------------------------------------------------------------------
// \ru Конструктор. \en Constructor. 
// ---
template <typename Type> MbSerialItem<Type>::MbSerialItem()
{ // Set the serial number and promote the counters
  PRECONDITION( serialLast != SYS_MAX_T ); // number overflow
  serialThis = ++serialLast;
  ++serialCount;
}


//-----------------------------------------------------------------------------
// \ru Деструктор. \en Destructor. 
// ---
template <typename Type> MbSerialItem<Type>::~MbSerialItem()
{
  PRECONDITION( SerialTypeValue( serialCount ) != 0 ); // If it equals 0 then it means that the counter is damaged.
  if ( --serialCount == 0 ) // The case when the last of the instantiated objects is deleted
    serialLast = 0; // Set serialization to zero  - at the subsequent creation of objects the numbering will start again (serially since 1).
}



//------------------------------------------------------------------------------
/// \ru Удалить объект без ссылок. \en Delete an object without references. 
//---
template <class Type>
inline void DeleteMatItem( Type *& item )
{
  if ( item != nullptr ) {
    delete item;
    item = nullptr;
  }
}


//------------------------------------------------------------------------------
/// \ru Удалить объекты без ссылок. \en Delete objects without references. 
// ---
template <class Vector>
void DeleteMatItems( Vector & items )
{
  for ( size_t k = 0, itemsCnt = items.size(); k < itemsCnt; ++k )
    ::DeleteMatItem( items[k] );
  items.clear();
}


//------------------------------------------------------------------------------
// \ru Сделать копию объекта и вернуть умный указатель на неё. \en Create a copy if object and return a smart pointer of the copy.
// ---
template <class Type, class RegType>
SPtr<Type> Clone( const Type & item, RegType * iReg )
{
  return SPtr<Type>{ &(item.Duplicate( iReg )) };
}


//------------------------------------------------------------------------------
/// \ru Сделать копию, если объект используется, иначе вернуть оригинал. \en Create a copy if object is used, otherwise return original. 
// ---
template <class Type>
Type & DuplicateIfUsed( Type & item )
{
  Type * resItem = &item;
  if ( item.GetUseCount() > 0 ) // \ru Если оригинал, то делаем копию. \en If there is original, then make a copy. 
    resItem = static_cast<Type *>( &item.Duplicate() );

  return *resItem;
}


//------------------------------------------------------------------------------
/// \ru Сделать копию, если объект используется, иначе вернуть оригинал. \en Create a copy if object is used, otherwise return original. 
// ---
template <class Type, class RegType>
Type & DuplicateIfUsed( Type & item, RegType * iReg )
{
  Type * resItem = &item;
  if ( item.GetUseCount() > 0 ) // \ru Если оригинал, то делаем копию. \en If there is original, then make a copy. 
    resItem = static_cast<Type *>( &item.Duplicate( iReg ) );

  return *resItem;
}


//------------------------------------------------------------------------------
/// \ru Сделать копию, если объект используется, иначе вернуть оригинал. \en Create a copy if object is used, otherwise return original. 
// ---
template <class Type>
Type * DuplicateIfUsed( SPtr<Type> & item )
{
  if ( item == nullptr )
    return nullptr;
  Type * resItem = item.get();
  if ( item->GetUseCount() > 1 ) // \ru Если оригинал, то делаем копию. \en If there is original, then make a copy. 
    resItem = static_cast<Type *>( &item->Duplicate() );

  return resItem;
}


//------------------------------------------------------------------------------
/// \ru Сделать копию, если объект используется, иначе вернуть оригинал. \en Create a copy if object is used, otherwise return original. 
// ---
template <class Type, class RegType>
Type * DuplicateIfUsed( SPtr<Type> & item, RegType * iReg )
{
  if ( item == nullptr )
    return nullptr;
  Type * resItem = item.get();
  if ( item->GetUseCount() > 1 ) // \ru Если оригинал, то делаем копию. \en If there is original, then make a copy. 
    resItem = static_cast<Type *>( &item->Duplicate( iReg ) );

  return resItem;
}

//------------------------------------------------------------------------------
/// \ru Удалить объект, если он больше никому не нужен. \en Delete an object if it is unnecessary. 
// ---
template <class Type>
void DeleteItem( Type *& item )
{
  if ( item != nullptr ) {
    if ( item->GetUseCount() < 1 ) // SKIP_SA
      delete item;
    item = nullptr;
  }
}

//------------------------------------------------------------------------------
/// \ru Освободить ссылку на объект. \en Release the reference to object. 
// ---
template <class Type>
void ReleaseItem( Type *& item )
{
  if ( item != nullptr ) {
    item->Release();
    item = nullptr; // SKIP_SA
  }
} // SKIP_SA

//------------------------------------------------------------------------------
/// \ru Захватить объект. \en Catch an object. 
// ---
template <class Type>
void AddRefItem( const Type * item )
{
  if ( item != nullptr )
    item->AddRef();
}

//------------------------------------------------------------------------------
/// \ru Отпустить объект без удаления. \en Detach an object without removing. 
// ---
template <class Type>
void DecRefItem( const Type * item )
{
  if ( item != nullptr )
    item->DecRef();
}


//------------------------------------------------------------------------------
/// \ru Захватить объекты. \en Catch objects. 
// ---
template <class ItemsVector>
void AddRefItems( const ItemsVector & items )
{
  for ( size_t k = 0, itemsCnt = items.size(); k < itemsCnt; ++k ) {
    if ( items[k] != nullptr )
      items[k]->AddRef();
  }
}


//------------------------------------------------------------------------------
/// \ru Отпустить объекты без удаления. \en Detach objects without removing. 
// ---
template <class ItemsVector>
void DecRefItems( const ItemsVector & items )
{
  for ( size_t k = 0, itemsCnt = items.size(); k < itemsCnt; ++k ) {
    if ( items[k] != nullptr )
      items[k]->DecRef();
  }
}


//------------------------------------------------------------------------------
/// \ru Удалить никому не нужные объекты. \en Remove unnecessary objects. 
// ---
template <class ItemsVector>
void DeleteItems( ItemsVector & items )
{
  for ( size_t k = 0, itemsCnt = items.size(); k < itemsCnt; ++k )
    ::DeleteItem( items[k] );
  items.clear();
}


//------------------------------------------------------------------------------
/// \ru Отпустить объекты с возможным удалением. \en Detach objects with possible removing. 
// ---
template <class ItemsVector>
void ReleaseItems( ItemsVector & items )
{
  for ( size_t k = 0, itemsCnt = items.size(); k < itemsCnt; ++k )
    ::ReleaseItem( items[k] );
  items.clear();
}


//------------------------------------------------------------------------------
/// \ru Удалить никому не нужные объекты. \en Remove unnecessary objects. 
// ---
template <class ItemsVector, class coType>
void DeleteItems( ItemsVector & items, SArray<coType> & coItems )
{
  size_t itemsCnt = items.size();
  if ( itemsCnt > 0 ) {
    for ( size_t k = 0; k < itemsCnt; ++k )
      ::DeleteItem( items[k] );
    items.clear();
    coItems.clear();
  }
}


//------------------------------------------------------------------------------
/// \ru Удалить никому не нужные объекты. \en Remove unnecessary objects. 
// ---
template <class ItemsVector, class coType>
void DeleteItems( ItemsVector & items, std::vector<coType> & coItems )
{
  size_t itemsCnt = items.size();
  if ( itemsCnt > 0 ) {
    for ( size_t k = 0; k < itemsCnt; ++k )
      ::DeleteItem( items[k] );
    items.clear();
    coItems.clear();
  }
}


//------------------------------------------------------------------------------
/// \ru Отпустить объекты с возможным удалением. \en Detach objects with possible removing. 
// ---
template <class ItemsVector, class coType>
void ReleaseItems( ItemsVector & items, SArray<coType> & coItems )
{
  size_t itemsCnt = items.size();
  if ( itemsCnt > 0 ) {
    for ( size_t k = 0; k < itemsCnt; ++k )
      ::ReleaseItem( items[k] );
    items.clear();
    coItems.clear();
  }
}


//------------------------------------------------------------------------------
/// \ru Отпустить объекты с возможным удалением. \en Detach objects with possible removing. 
// ---
template <class ItemsVector, class coType>
void ReleaseItems( ItemsVector & items, std::vector<coType> & coItems )
{
  size_t itemsCnt = items.size();
  if ( itemsCnt > 0 ) {
    for ( size_t k = 0; k < itemsCnt; ++k )
      ::ReleaseItem( items[k] );
    items.clear();
    coItems.clear();
  }
}


//------------------------------------------------------------------------------
/// \ru Переложить элементы с захватом и возможным копированием. \en Put elements with capturing and with possible copying. 
// ---
template <class TypeVector, class Type>
void AddRefItems( const TypeVector & srcItems, bool same, RPArray<Type> & dstItems )
{
  if ( (srcItems.size() > 0) && reinterpret_cast<ptrdiff_t>( &srcItems ) != reinterpret_cast<ptrdiff_t>( &dstItems ) ) {
    dstItems.reserve( dstItems.size() + srcItems.size() );
    for ( size_t k = 0, cnt = srcItems.size(); k < cnt; ++k ) {
      if ( srcItems[k] != nullptr ) {
        Type * srcItem = &const_cast<Type &>(*srcItems[k]);
        Type * dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate() );
        dstItem->AddRef();
        dstItems.push_back( dstItem );
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Переложить элементы с захватом и возможным копированием. \en Put elements with capturing and with possible copying. 
// ---
template <class TypeVector, class Type>
void AddRefItems( const TypeVector & srcItems, bool same, std::vector< SPtr<Type> > & dstItems )
{
  if ( (srcItems.size() > 0) && reinterpret_cast<ptrdiff_t>(&srcItems) != reinterpret_cast<ptrdiff_t>(&dstItems) ) {
    dstItems.reserve( dstItems.size() + srcItems.size() );
    for ( size_t k = 0, cnt = srcItems.size(); k < cnt; k++ ) {
      if ( srcItems[k] != nullptr ) {
        Type * srcItem = &const_cast<Type &>(*srcItems[k]);
        SPtr<Type> dstItem;
        dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate() );
        dstItems.push_back( dstItem );
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Переложить элементы с захватом и возможным копированием. \en Put elements with capturing and with possible copying. 
// ---
template <class TypeVector, class Type>
void AddRefItems( const TypeVector & srcItems, bool same, std::vector<Type *> & dstItems )
{
  if ( (srcItems.size() > 0) && reinterpret_cast<ptrdiff_t>( &srcItems ) != reinterpret_cast<ptrdiff_t>( &dstItems ) ) {
    dstItems.reserve( dstItems.size() + srcItems.size() );
    for ( size_t k = 0, cnt = srcItems.size(); k < cnt; ++k ) {
      if ( srcItems[k] != nullptr ) {
        Type * srcItem = &const_cast<Type &>(*srcItems[k]);
        Type * dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate() );
        dstItem->AddRef();
        dstItems.push_back( dstItem );
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Переложить элементы с захватом и возможным копированием. \en Put elements with capturing and with possible copying. 
// ---
template <class TypeVector, class Type, class RegType >
void AddRefRegItems( const TypeVector & srcItems, bool same, RPArray<Type> & dstItems, RegType * iReg )
{
  if ( (srcItems.size() > 0) && reinterpret_cast<ptrdiff_t>(&srcItems) != reinterpret_cast<ptrdiff_t>(&dstItems) ) {
    dstItems.reserve( dstItems.size() + srcItems.size() );
    for ( size_t k = 0, cnt = srcItems.size(); k < cnt; ++k ) {
      if ( srcItems[k] != nullptr ) {
        Type * srcItem = &const_cast<Type &>(*srcItems[k]);
        Type * dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate( iReg ) );
        dstItem->AddRef();
        dstItems.push_back( dstItem );
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Переложить элементы с захватом и возможным копированием. \en Put elements with capturing and with possible copying. 
// ---
template <class TypeVector, class Type, class RegType >
void AddRefRegItems( const TypeVector & srcItems, bool same, std::vector< SPtr<Type> > & dstItems, RegType * iReg )
{
  if ( (srcItems.size() > 0) && reinterpret_cast<ptrdiff_t>(&srcItems) != reinterpret_cast<ptrdiff_t>(&dstItems) ) {
    dstItems.reserve( dstItems.size() + srcItems.size() );
    for ( size_t k = 0, cnt = srcItems.size(); k < cnt; ++k ) {
      if ( srcItems[k] != nullptr ) {
        Type * srcItem = &const_cast<Type &>(*srcItems[k]);
        SPtr<Type> dstItem;
        dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate( iReg ) );
        dstItems.push_back( dstItem );
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Переложить элементы с захватом и возможным копированием. \en Put elements with capturing and with possible copying. 
// ---
template <class TypeVector, class Type, class RegType >
void AddRefRegItems( const TypeVector & srcItems, bool same, std::vector<Type *> & dstItems, RegType * iReg )
{
  if ( (srcItems.size() > 0) && reinterpret_cast<ptrdiff_t>(&srcItems) != reinterpret_cast<ptrdiff_t>(&dstItems) ) {
    dstItems.reserve( dstItems.size() + srcItems.size() );
    for ( size_t k = 0, cnt = srcItems.size(); k < cnt; ++k ) {
      if ( srcItems[k] != nullptr ) {
        Type * srcItem = &const_cast<Type &>(*srcItems[k]);
        Type * dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate( iReg ) );
        dstItem->AddRef();
        dstItems.push_back( dstItem );
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Являются ли объекты подобными. \en Determine whether the objects are similar. 
// ---
template <class Item>
bool IsItemSame( const Item * item1, const Item * item2, double accuracy )
{
  if ( (item1 == nullptr) && (item2 == nullptr) )
    return true;
  else if ( (item1 != nullptr) && (item2 != nullptr) && item1->IsSame( *item2, accuracy ) )
    return true;
  return false;
}


//------------------------------------------------------------------------------
/// \ru Являются ли объекты подобными. \en Determine whether the objects are similar. 
// ---
template <class Vector>
bool AreItemsSame( const Vector & items1, const Vector & items2, double accuracy )
{
  bool areEqual = false;

  const size_t cnt = items1.size();
  if ( cnt == items2.size() ) {
    areEqual = true;
    for ( size_t k = 0; k < cnt; ++k ) {
      if ( (items1[k] == nullptr) || (items2[k] == nullptr) || !items1[k]->IsSame( *items2[k], accuracy ) ) {
        areEqual = false;
        break;
      }
    }
  }

  return areEqual;
}


//------------------------------------------------------------------------------
/// \ru Являются ли объекты подобными. \en Determine whether the objects are similar. 
// ---
template <class Vector>
bool AreObjectsSame( const Vector & items1, const Vector & items2, double accuracy )
{
  bool areEqual = false;

  const size_t cnt = items1.size();
  if ( cnt == items2.size() ) {
    areEqual = true;
    for ( size_t k = 0; k < cnt; ++k ) {
      if ( !items1[k].IsSame( items2[k], accuracy ) ) {
        areEqual = false;
        break;
      }
    }
  }

  return areEqual;
}


//------------------------------------------------------------------------------
/// \ru Являются ли объекты подобными. \en Determine whether the objects are similar. 
// ---
template <class Vector>
bool AreItemsSimilar( const Vector & items1, const Vector & items2 )
{
  bool areEqual = false;

  const size_t cnt = items1.size();
  if ( cnt == items2.size() ) {
    areEqual = true;
    for ( size_t k = 0; k < cnt; ++k ) {
      if ( (items1[k] == nullptr) || (items2[k] == nullptr) || !items1[k]->IsSimilar( *items2[k] ) ) {
        areEqual = false;
        break;
      }
    }
  }

  return areEqual;
}


//------------------------------------------------------------------------------
/// \ru Сделать равными. \en Make equal. 
// ---
template <class Vector>
bool SetItemsEqual( const Vector & srcItems, Vector & dstItems )
{
  bool setEqual = false; 

  size_t cnt = srcItems.size();
  if ( cnt == dstItems.size() ) {
    setEqual = ::AreItemsSimilar( srcItems, dstItems );

    if ( setEqual ) {
      for ( size_t k = 0; k < cnt; ++k ) {
        if ( srcItems[k] == nullptr || dstItems[k] == nullptr || !dstItems[k]->SetEqual( *srcItems[k] )  ) {
          setEqual = false;
          break;
        }
      }
    }
  }

  return setEqual;
}


//------------------------------------------------------------------------------
/// \ru Дублировать c регистратором (опционально переложить оригиналы). \en Duplicate with registrator (optionally put originals). 
// ---
template <class TypeVector, class Type, class RegType>
void DuplicateItems( const TypeVector & srcItems, RegType * iReg, bool same, RPArray<Type> & dstItems )
{
  C3D_ASSERT( dstItems.size() < 1 );
  dstItems.Reserve( srcItems.size() );
  for ( size_t k = 0, cnt = srcItems.size(); k < cnt; ++k ) {
    Type * srcItem = srcItems[k];
    if ( srcItem != nullptr ) {
      Type * dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate( iReg ) );
      dstItems.push_back( dstItem );
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Дублировать c регистратором (опционально переложить оригиналы). \en Duplicate with registrator (optionally put originals). 
// ---
template <class TypeVector, class Type, class RegType>
void DuplicateItems( const TypeVector & srcItems, RegType * iReg, bool same, std::vector< SPtr<Type> > & dstItems )
{
  C3D_ASSERT( dstItems.size() < 1 );
  dstItems.reserve( dstItems.size() + srcItems.size() );
  for ( size_t k = 0, cnt = srcItems.size(); k < cnt; ++k ) {
    Type * srcItem = srcItems[k];
    if ( srcItem != nullptr ) {
      SPtr<Type> dstItem;
      dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate( iReg ) );
      dstItems.push_back( dstItem );
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Дублировать c регистратором (опционально переложить оригиналы). \en Duplicate with registrator (optionally put originals). 
// ---
template <class TypeVector, class Type, class RegType>
void DuplicateItems( const TypeVector & srcItems, RegType * iReg, bool same, std::vector<Type *> & dstItems )
{
  C3D_ASSERT( dstItems.size() < 1 );
  dstItems.reserve( dstItems.size() + srcItems.size() );
  for ( size_t k = 0, cnt = srcItems.size(); k < cnt; ++k ) {
    Type * srcItem = srcItems[k];
    if ( srcItem != nullptr ) {
      Type * dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate( iReg ) );
      dstItems.push_back( dstItem );
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Преобразовать элементы согласно матрице. \en Transform elements according to the matrix. 
// ---
template <class Array, class Matrix, class RegType>
void TransformItems( Array & items, const Matrix & matr, RegType * iReg )
{
  for ( size_t k = 0, cnt = items.size(); k < cnt; ++k ) {
    if ( items[k] != nullptr )
      items[k]->Transform( matr, iReg );
  }
}

//------------------------------------------------------------------------------
/// \ru Преобразовать элементы согласно матрице. \en Transform elements according to the matrix. 
// ---
template <class Array, class Matrix>
void TransformItems( Array & items, const Matrix & matr )
{
  for ( size_t k = 0, cnt = items.size(); k < cnt; ++k ) {
    if ( items[k] != nullptr )
      items[k]->Transform( matr );
  }
}

//------------------------------------------------------------------------------
/// \ru Преобразовать элементы согласно матрице. \en Transform elements according to the matrix. 
// ---
template <class Array, class Matrix>
void TransformObjects( Array & objects, const Matrix & matr )
{
  for ( size_t k = 0, cnt = objects.size(); k < cnt; ++k )
    objects[k].Transform( matr );
}


//------------------------------------------------------------------------------
/// \ru Сдвинуть вдоль объекты вектора. \en Translate objects along a vector. 
// ---
template <class Array, class Vector, class RegType>
void MoveItems( Array & items, const Vector & to, RegType * iReg )
{
  for ( size_t k = 0, cnt = items.size(); k < cnt; ++k ) {
    if ( items[k] != nullptr )
      items[k]->Move( to, iReg );
  }
}

//------------------------------------------------------------------------------
/// \ru Сдвинуть вдоль объекты вектора. \en Translate objects along a vector. 
// ---
template <class Array, class Vector>
void MoveItems( Array & items, const Vector & to )
{
  for ( size_t k = 0, cnt = items.size(); k < cnt; ++k ) {
    if ( items[k] != nullptr )
      items[k]->Move( to );
  }
}

//------------------------------------------------------------------------------
/// \ru Сдвинуть вдоль объекты вектора. \en Translate objects along a vector. 
// ---
template <class Array, class Vector>
void MoveObjects( Array & objects, const Vector & to )
{
  for ( size_t k = 0, cnt = objects.size(); k < cnt; ++k )
    objects[k].Move( to );
}


//------------------------------------------------------------------------------
/// \ru Повернуть вокруг оси. \en Rotate about an axis. 
// ---
template <class Array, class Axis, class RegType>
void RotateItems( Array & items, const Axis & axis, double angle, RegType * iReg )
{
  for ( size_t k = 0, cnt = items.size(); k < cnt; ++k ) {
    if ( items[k] != nullptr )
      items[k]->Rotate( axis, angle, iReg );
  }
}

//------------------------------------------------------------------------------
/// \ru Повернуть вокруг оси. \en Rotate about an axis. 
// ---
template <class Array, class Axis>
void RotateItems( Array & items, const Axis & axis, double angle )
{
  for ( size_t k = 0, cnt = items.size(); k < cnt; ++k ) {
    if ( items[k] != nullptr )
      items[k]->Rotate( axis, angle );
  }
}

//------------------------------------------------------------------------------
/// \ru Повернуть вокруг оси. \en Rotate about an axis. 
// ---
template <class Array, class Axis>
void RotateObjects( Array & objects, const Axis & axis, double angle )
{
  for ( size_t k = 0, cnt = objects.size(); k < cnt; ++k )
    objects[k].Rotate( axis, angle );
}


//------------------------------------------------------------------------------
/// \ru Запись объектов в поток. \en Write objects to the stream. 
// ---
template <class Vector, class Writer>
void WriteRefItems( const Vector & items, Writer & out )
{
  size_t k, cnt = items.size();

  for ( k = 0; k < cnt; ++k ) {
    if ( items[k] == nullptr )
      cnt--;
  }

  WriteCOUNT( out, cnt );

  if ( out.good() ) {
    for ( k = 0; k < cnt; ++k ) {
      if ( items[k] != nullptr ) {
        items[k]->PrepareWrite();
        out << &(*items[k]);
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Запись объекты в поток. \en Write objects to the stream. 
// ---
template <class Type, class Writer>
void WriteRefItems( const std::vector< SPtr<Type> > & items, Writer & out )
{
  size_t k, cnt = items.size();

  for ( k = 0; k < cnt; ++k ) {
    if ( items[k] == nullptr )
      cnt--;
  }

  WriteCOUNT( out, cnt );

  if ( out.good() ) {
    for ( k = 0; k < cnt; ++k ) {
      if ( items[k] != nullptr ) {
        items[k]->PrepareWrite();
        out << items[k].get();
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Чтение массива объектов из потока с захватом. \en Read an array of objects with capturing from the stream. 
// ---
template <class Type, class Reader>
void ReadRefItems( Reader & in, RPArray<Type> & items )
{  
  size_t cnt = ReadCOUNT( in );

  if ( in.good() && cnt > 0 ) {
    items.reserve( items.size() + cnt );

    for ( size_t i = 0; i < cnt; ++i ) {
      Type * item = nullptr;
      in >> item;
      if ( item != nullptr ) {
        items.push_back( item );
        item->AddRef();
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Чтение массива объектов из потока с захватом. \en Read an array of objects with capturing from the stream. 
// ---
template <class Type, class Reader>
void ReadRefItems( Reader & in, std::vector<Type *> & items )
{  
  size_t cnt = ReadCOUNT( in );

  if ( in.good() && cnt > 0 ) {
    for ( size_t i = 0; i < cnt; ++i ) {
      Type * item = nullptr;
      in >> item;
      if ( item != nullptr ) {
        items.push_back( item );
        item->AddRef();
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Чтение массива объектов из потока с захватом. \en Read an array of objects with capturing from the stream. 
// ---
template <class Type, class Reader>
void ReadRefItems( Reader & in, std::vector< SPtr<Type> > & items )
{  
  size_t cnt = ReadCOUNT( in );

  if ( in.good() && cnt > 0 ) {
    for ( size_t i = 0; i < cnt; ++i ) {
      Type * item = nullptr;
      in >> item;
      if ( item != nullptr )
        items.push_back( SPtr<Type>(item) );
    }
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Отцепить от массива и уменьшить счетчик ссылок.
           \en Detach from array and decrease reference counter. \~
  \details \ru Отцепить от массива и уменьшить счетчик ссылок без проверок.\n
           \en Detach from array and decrease reference counter without checks.\n \~  
  \param[in,out] items - \ru Множество элементов.
                         \en An array of elements. \~
  \param[in]     index - \ru Номер элемента. Не проверяется на корректность.
                         \en Index of element. Isn't checked for correctness. \~
*/
// --- 
template <class Vector>
void ReleaseAndDetachItem_( Vector & items, size_t index )
{
  items[index]->Release();
  items.erase( items.begin() + index );
}


//------------------------------------------------------------------------------
/** \brief \ru Добавить объект и увеличить счетчик ссылок.
           \en Add an object and increase reference counter. \~
  \details \ru Добавить объект и увеличить счетчик ссылок без проверок.\n
           \en Add an object and increase reference counter without checks.\n \~    
  \param[in,out] items   - \ru Множество элементов.
                           \en An array of elements. \~
  \param[in]     newItem - \ru Новый элемент.
                           \en New element. \~       
*/
// ---    
template <class Vector, class Type>
void AddRefAndAddItem_( Vector & items, Type * newItem )
{
  newItem->AddRef();
  items.push_back( newItem );     
}


//------------------------------------------------------------------------------
/** \brief \ru Добавить объект и увеличить счетчик ссылок.
           \en Add an object and increase reference counter. \~
  \details \ru Добавить объект и увеличить счетчик ссылок без проверок.\n
           \en Add an object and increase reference counter without checks.\n \~    
  \param[in,out] items   - \ru Множество элементов.
                           \en An array of elements. \~
  \param[in]     newItem - \ru Новый элемент.
                           \en New element. \~
  \param[in]     index   - \ru Номер элемента. Не проверяется на корректность.
                           \en Index of element. Isn't checked for correctness. \~
*/
// ---
template <class Vector, class Type>
void AddRefAndAddAtItem_( Vector & items, Type * newItem, size_t index )
{
  newItem->AddRef();
  items.insert( index, newItem );
}


//------------------------------------------------------------------------------
/** \brief \ru Отцепить от массива и уменьшить счетчик ссылок.
           \en Detach from array and decrease reference counter. \~
  \details \ru Отцепить от массива и уменьшить счетчик ссылок без проверок.\n
           \en Detach from array and decrease reference counter without checks.\n \~  
  \param[in,out] items - \ru Множество элементов.
                         \en An array of elements. \~
  \param[in]     index - \ru Номер элемента. Не проверяется на корректность.
                         \en Index of element. Isn't checked for correctness. \~
*/
// --- 
template <class Type>
void ReleaseAndDetachItem_( std::vector<SPtr<Type> > & items, size_t index )
{
  items[index].reset();
  items.erase( items.begin() + index );
}


//------------------------------------------------------------------------------
/** \brief \ru Добавить объект и увеличить счетчик ссылок.
           \en Add an object and increase reference counter. \~
  \details \ru Добавить объект и увеличить счетчик ссылок без проверок.\n
           \en Add an object and increase reference counter without checks.\n \~    
  \param[in,out] items   - \ru Множество элементов.
                           \en An array of elements. \~
  \param[in]     newItem - \ru Новый элемент.
                           \en New element. \~
*/
// ---
template <class Type>
void AddRefAndAddItem_( std::vector<SPtr<Type> > & items, Type * newItem )
{
  items.push_back( SPtr<Type>(newItem) );
}


//------------------------------------------------------------------------------
/** \brief \ru Добавить объект и увеличить счетчик ссылок.
           \en Add an object and increase reference counter. \~
  \details \ru Добавить объект и увеличить счетчик ссылок без проверок.\n
           \en Add an object and increase reference counter without checks.\n \~    
  \param[in,out] items   - \ru Множество элементов.
                           \en An array of elements. \~
  \param[in]     newItem - \ru Новый элемент.
                           \en New element. \~
  \param[in]     index   - \ru Номер элемента. Не проверяется на корректность.
                           \en Index of element. Isn't checked for correctness. \~
*/
// ---
template <class Type>
void AddRefAndAddAtItem_( std::vector<SPtr<Type> > & items, Type * newItem, size_t index )
{
  items.insert( items.begin() + index, SPtr<Type>(newItem) );
}


//------------------------------------------------------------------------------
/// \ru Отцепить объект из владеющего указателя. \en Detach object from owning pointer. 
// ---
template <class Type>
inline Type * DetachItem( SPtr<Type> & itemOwner )
{
  return itemOwner.detach();
}


//------------------------------------------------------------------------------
// \ru Отцепить каждый элемент массива из владеющего указателя. \en Detach every element of array from owning pointer. 
// ---
template <class ItemsVector>
void DetachItems( ItemsVector & vect )
{
  for ( size_t i = vect.size(); i-- > 0; )
    ::DetachItem( vect[i] );
}


//------------------------------------------------------------------------------
/// \ru Заменить объект на копию. \en Replace object by copy. 
// ---
template <class Type>
void ReplaceByCopy( Type *& item )
{
  if ( item != nullptr ) {
    Type * temp = static_cast<Type *>( &item->Duplicate() );
    ::DeleteItem( item );
    item = temp;
  }
}


//------------------------------------------------------------------------------
/// \ru Включить габариты объектов массива в общий габарит. \en Include bounding boxes of an array of objects in a common bounding box. 
//---
template<class Objects, class Gab>
void AddYourGabaritTo( Objects & objects, Gab & gab )
{
  for ( size_t k = 0, cnt = objects.size(); k < cnt; ++k ) {
    if ( objects[k] != nullptr )
      objects[k]->AddYourGabaritTo( gab );
  }
}


#endif // __REFERENCE_ITEM_H
