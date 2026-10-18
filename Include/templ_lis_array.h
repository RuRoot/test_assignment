////////////////////////////////////////////////////////////////////////////////
/** 
  \file     
  \brief \ru Little SArray - укороченный SArray.
         \en Little SArray - shortened SArray. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_LIS_ARRAY_H
#define __TEMPL_LIS_ARRAY_H


#include <stdlib.h>
#include <io_define.h>


FORVARD_DECL_TEMPLATE_TYPENAME( class LiSArray );
FORVARD_DECL_TEMPLATE_TYPENAME( void set_array_size( LiSArray<Type> &, size_t newSize ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader & CALL_DECLARATION operator >> ( reader& in, LiSArray<Type> & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer & CALL_DECLARATION operator << ( writer& out, const LiSArray<Type> & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader & CALL_DECLARATION operator >> ( reader& in, LiSArray<Type> *& ptr ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer & CALL_DECLARATION operator << ( writer& out, const LiSArray<Type> * ptr ) );


//------------------------------------------------------------------------------
// \ru Little SArray - укороченный SArray \en Little SArray - shortened SArray 
// \ru одномерный массив объектов не содержащих указателей !!!!!!! \en one-dimensional array of objects without pointers !!! 
// \ru В массиве нельзя хранить объекты содержащие указатели или \en An array should not contain objects with pointers or 
// \ru классы с указателями, а также абстрактные классы с наследниками \en classes with pointers and abstract classes with inheritors 
// ---
template <class Type>
class LiSArray {
protected :
  uint8   count; // \ru размерность массива \en a size of the array 
  Type  * parr;

  // \ru Этот массив прирастает всегда по 2. \en This array always increase by 2.
  enum {
    li_delta = 2,
  };

public :
  LiSArray();
  LiSArray( const LiSArray<Type> & );
  virtual ~LiSArray() { set_array_size( *this, 0 );  }
public:
  void              Flush();                                 // \ru обнулить количество элементов \en set the number of elements to null. 
  Type *            Add( const Type & );                     // \ru добавить элемент в конец массива \en add element to the end of array 
  Type *            InsertInd( size_t index, const Type & ); // \ru вставить элемент перед указанным \en insert element before the specified one 
  void              RemoveInd( size_t delIndex );            // \ru удалить элемент из массива \en delete an element from array 
  size_t            Count() const { return count; }          // \ru дать количество элементов массива \en get the number of elements 

  LiSArray<Type> &  operator =  ( const LiSArray<Type> & );
  LiSArray<Type> &  operator += ( const LiSArray<Type> & );
  const Type &      operator []( size_t loc ) const { C3D_ASSERT(loc < count); return parr[loc]; } // \ru ИР C3D_ASSERT нужен для оценки выхода за пределы массива. Я один раз вышел полдня потом искал!!! \en ИР C3D_ASSERT is necessary to detect out of bounds. - 
        Type &      operator []( size_t loc )       { C3D_ASSERT(loc < count); return parr[loc]; } // \ru ИР C3D_ASSERT нужен для оценки выхода за пределы массива. Я один раз вышел полдня потом искал!!! \en ИР C3D_ASSERT is necessary to detect out of bounds. - 
  const Type *      GetAddr() const { return parr; }         // \ru выдать адрес начала массива \en get address of the beginning of an array 

protected :
  void              CatchMemory();                           // \ru захватить память \en catch memory 

  TEMPLATE_FRIEND void set_array_size TEMPLATE_SUFFIX ( LiSArray<Type> &, size_t newSize );
  TEMPLATE_FRIEND reader & CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader & in,        LiSArray<Type> &  ref );
  TEMPLATE_FRIEND writer & CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer & out, const LiSArray<Type> &  ref );
  TEMPLATE_FRIEND reader & CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader & in,        LiSArray<Type> *& ptr );
  TEMPLATE_FRIEND writer & CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer & out, const LiSArray<Type> *  ptr );

#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
public:
  void * operator new    ( size_t );
  void   operator delete ( void *, size_t );
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_
};

#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
//------------------------------------------------------------------------------
// \ru Перегрузка оператора new. \en Overloading of the "new" operator. 
// ---
template <class Type>
inline void * LiSArray<Type>::operator new( size_t size ) {
  return ::Allocate( size, typeid(LiSArray<Type>).name() );
}

//------------------------------------------------------------------------------
// \ru Перегрузка оператора delete. \en Overloading of the "delete" operator. 
// ---
template <class Type>
inline void LiSArray<Type>::operator delete( void * ptr, size_t size ) {
  ::Free( ptr, size, typeid(LiSArray<Type>).name() );
}
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_

//------------------------------------------------------------------------------
// \ru Конструктор массива \en Constructor of an array 
// ---
template <class Type>
inline LiSArray<Type>::LiSArray()
  : count ( 0 )
  , parr  ( 0 )
{}

//------------------------------------------------------------------------------
// \ru Конструктор копирования массива \en Copy-constructor of an array 
// ---
template <class Type>
inline LiSArray<Type>::LiSArray( const LiSArray<Type> & o )
  : count ( 0 )
  , parr  ( 0 )
{
  *this = o;
}

//------------------------------------------------------------------------------
// \ru сбросить массив \en reset an array 
// ---
template <class Type>
inline void LiSArray<Type>::Flush()
{
  set_array_size( *this, 0 );
  count = 0;
}

//------------------------------------------------------------------------------
// \ru добавить элемент в конец массива \en add element to the end of array 
// ---
template <class Type>
inline Type* LiSArray<Type>::Add( const Type & ent ) 
{
  CatchMemory();
  C3D_ASSERT( count < 253 );
  return (Type*)memcpy( parr+count++, &ent, sizeof(Type) ); // SKIP_SA
}

//------------------------------------------------------------------------------
// \ru вставить элемент перед указанным \en insert element before the specified one 
// ---
template <class Type>
inline Type * LiSArray<Type>::InsertInd( size_t index, const Type & ent ) 
{
  C3D_ASSERT( index <= count );
  CatchMemory();  // \ru добавить памяти, если все использовано \en add memory if whole allocated memory is used 

  if ( index >= count )
    index = count;
  else {
    memmove( parr+index+1, parr+index, (count-index) * sizeof(Type) ); // SKIP_SA
  }
  count++;
  C3D_ASSERT( count < 254 );               
  // \ru передвинем вправо все элементы массива с последнего до указанного \en move to the right all elements of the array from the last to the specified one 
  return (Type*)memcpy( parr+index, &ent, sizeof(Type) );  // \ru записываем новый элемент \en writing new element // SKIP_SA
}

//------------------------------------------------------------------------------
// \ru удалить элемент из массива \en delete an element from array 
// ---
template <class Type>
inline void LiSArray<Type>::RemoveInd( size_t delIndex ) 
{
  C3D_ASSERT( delIndex < count );
  C3D_ASSERT( delIndex < 254 );
  C3D_ASSERT( count < 254 );
  if ( delIndex < count ) {
    memcpy( parr+delIndex, parr+delIndex+1, (count - delIndex-1)*sizeof(Type) );
    count--;
    C3D_ASSERT( count < 254 );
  }
}

//------------------------------------------------------------------------------
// \ru присвоение массива массиву \en assignment of an array to array 
// ---
template <class Type>
inline LiSArray<Type> & LiSArray<Type>::operator = ( const LiSArray<Type> & o ) 
{
  set_array_size( *this, o.count );  // \ru обеспечить память на такое кол-во элементов \en allocate memory for the given number of elements 
  count = o.count;
  C3D_ASSERT( count < 254 );
  if ( count > 0 && parr != nullptr )
    memcpy( parr, o.parr, count * sizeof(Type) ); // SKIP_SA

  return *this;
}

//------------------------------------------------------------------------------
// \ru добавление массива к массиву \en add the array to the array 
// ---
template <class Type>
inline LiSArray<Type> & LiSArray<Type>::operator += ( const LiSArray<Type> & o ) 
{
  if ( o.count ) {
    set_array_size( *this, count + o.count );  // \ru обеспечить память на такое кол-во элементов \en allocate memory for the given number of elements 
    if ( parr != nullptr )
      memcpy( parr+count, o.parr, o.count * sizeof(Type) );

    count = (uint8)(count + o.count);
    C3D_ASSERT( count < 254 );
  }

  return *this;
}

//------------------------------------------------------------------------------
// \ru захват большего куска памяти ( если нужно ) \en allocate the large piece of memory (if it is necessary) 
// ---
template <class Type>
inline void LiSArray<Type>::CatchMemory() 
{
  C3D_ASSERT( count < 254 );
  set_array_size( *this, count + 1 );
  // \ru здесь count не увеличивается! \en 'count' is not increased! 
}

//------------------------------------------------------------------------------
// \ru Если захвачен не такой размер памяти, то захватить новый \en If the size of caught memory is not appropriated then allocate memory again 
// \ru если clear = true, то присвоить arr.count=0 и старое содержимое не копировать \en if clear = true then set arr.count=0 and do not copy the old content 
// ---
template <class Type>
void set_array_size( LiSArray<Type> & arr, size_t newSize ) 
{
  C3D_ASSERT( newSize <= 0xff ); // \ru поскольку здесь uint8 count \en since uint8 count 
  C3D_ASSERT( arr.Count() < 254 );
  uint8 newUpper = (uint8)(newSize   + newSize   % (size_t)LiSArray<Type>::li_delta);
  uint8 oldUpper = (uint8)(arr.Count() + arr.Count() % (size_t)LiSArray<Type>::li_delta);

  if ( newUpper != oldUpper ) {
#ifdef __REALLOC_ARRAYS_STATISTIC_
    void *oldParr = arr.parr;
#endif // __REALLOC_ARRAYS_STATISTIC_

#ifdef USE_REALLOC_IN_ARRAYS
      arr.parr = (Type*) ::realloc( arr.parr, newUpper * sizeof(Type) );
#else
      Type *p_tmp = newUpper ? (Type*)new TCHAR[ newUpper * sizeof(Type) ] : 0;

      if ( arr.parr && p_tmp )
        memcpy( p_tmp, arr.parr, std_min(oldUpper, (uint8)newUpper)* sizeof(Type) );

      if ( arr.parr )
        delete [] (TCHAR *) arr.parr;

      arr.parr = p_tmp;
#endif // USE_REALLOC_IN_ARRAYS

// \ru !!! здесь count не изменяется!!! \en !!! 'count' is not changed!! 

#ifdef __REALLOC_ARRAYS_STATISTIC_
      REALLOC_ARRAY_STATISTICS( oldParr, oldUpper * sizeof(Type), arr.parr, newUpper * sizeof(Type), 3/*LiSArray*/ );
#endif // __REALLOC_ARRAYS_STATISTIC_
  }
}

#endif  // __TEMPL_LIS_ARRAY_H
