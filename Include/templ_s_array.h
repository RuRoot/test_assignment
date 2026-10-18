////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Одномерный массив объектов.
         \en One-dimensional array of objects. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_S_ARRAY_H
#define __TEMPL_S_ARRAY_H


#include <math_x.h>
#include <io_define.h>
#include <math_define.h>
#include <tool_quick_sort.h>
#include <tool_err_handling.h>
#include <vector>
#include <cstring>
#include <utility>
#include <algorithm>


FORVARD_DECL_TEMPLATE_TYPENAME( class SArray );
FORVARD_DECL_TEMPLATE_TYPENAME( bool   set_array_size (       SArray<Type> &, size_t newSize, bool clear ) );
FORVARD_DECL_TEMPLATE_TYPENAME( Type * add_n_to_array (       SArray<Type> &, size_t n ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t find_in_array  ( const SArray<Type> &, const Type & object ) );
FORVARD_DECL_TEMPLATE_TYPENAME( bool   fill_array     (       SArray<Type> &, size_t fillCount, const Type & fillData ) );
FORVARD_DECL_TEMPLATE_TYPENAME( bool   fill_array_zero(       SArray<Type> &, size_t fillCount, size_t startIndex ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader & CALL_DECLARATION operator >> ( reader & in,        SArray<Type> & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer & CALL_DECLARATION operator << ( writer & out, const SArray<Type> & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader & CALL_DECLARATION operator >> ( reader & in,        SArray<Type> *& ptr ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer & CALL_DECLARATION operator << ( writer & out, const SArray<Type> * ptr ) );


//------------------------------------------------------------------------------
// \ru Функция сравнения для сортировки упорядочивает элементы по возрастанию (элементы должны иметь функции > и <). 
// \en The comparison function for sorting orders elements in ascending (elements should have functions > and  <). \~
// ---
template <class Type>
inline int CompareSArrayItems( const Type * f1, const Type * f2 ) {
  return (*f1 > *f2) ? 1 : ((*f1 < *f2) ? -1 : 0);
}


//-----------------------------------------------------------------------------
/** \brief \ru Массив простых структур данных.
           \en Array of plain old data structures (POD). \~
  \details \ru Шаблонный массив, работающий только с простыми (POD) данными, которые 
  могут копироваться или перемещаться в памяти методом memcpy без нарушения целостности объекта. 
  Например, в массиве нельзя хранить объекты, динамически выделяющие память или 
  классы с указателями, а также классы с виртуальными функциями. \n
           \en A template array working with POD-only data that can be copied or moved in 
  memory by 'memcpy' maintaining the validity of the object. For sample, the array should 
  not contain objects which dynamically allocate memory, or classes with virtual functions.
           \~  
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class SArray {
public:
  typedef Type value_type;
  typedef Type const & const_reference;
  typedef Type       & reference;
protected :
  size_t  count; ///< \ru Количество элементов в массиве. \en The number of elements in array. 
  size_t  upper; ///< \ru Под какое количество элементов выделена память. \en The number of elements the memory is allocated for. 
  uint16  delta; ///< \ru Приращение по количеству элементов при выделении дополнительной памяти. \en Increment by the number of elements while the allocation of additional memory. 
private :
  Type *  parr; ///< \ru Указатель на первый элемент массива. \en A pointer to the first array element. 

public :
  /// \ru Конструктор. \en Constructor. 
  explicit SArray( size_t i_max = 0, uint16 i_delta = 1 );
  /// \ru Конструктор копирования. \en Copy constructor. 
  SArray( const SArray<Type> & );
  /// \ru Конструктор копирования. \en Copy constructor. 
  explicit SArray( const std::vector<Type> & );
  /// \ru Деструктор. \en Destructor. 
  virtual ~SArray() { set_array_size( *this, 0, true/*clear*/ ); }
  
public:
  /// \ru Количество элементов, под которое зарезервирована память. \en The number of elements the memory is allocated for.
          size_t  Upper() const                  { return upper; }
  /// \ru Получить приращение по количеству элементов при выделении дополнительной памяти. \en Get the increment by the number of elements while the allocation of additional memory. 
          uint16  Delta() const                  { return delta; }
  /// \ru Установить приращение по количеству элементов при выделении дополнительной памяти (1 - автоприращение). \en Set the increment by the number of elements while the allocation of additional memory (1 - autoincrement). 
          void    Delta( uint16 newDelta ) { delta = newDelta; }
  /// \ru Установить максимальное из приращений. \en Set the maximum increment. 
          void    SetMaxDelta( uint16 newDelta ) { if ( delta < newDelta ) delta = newDelta; }

  /// \ru Функции, выделяющие потенциально большие участки памяти, возвращают результат операции (успех/ошибка).
  /// \en Functions that allocate potentially large memory, return an operation result (success/error). 
          bool    SetSize( size_t newSize, bool clear/*=true*/ );       ///< \ru Установить новый размер массива. \en Set the new size of an array. 
          bool    Reserve( size_t n, bool addAdditionalSpace = true );  ///< \ru Зарезервировать место под столько элементов. \en Reserve space for a given number of elements. 

          void    Flush    () { count = 0; }                            ///< \ru Обнулить количество элементов. \en Set the number of elements to null. 
          void    HardFlush() { Flush(); Adjust(); }                    ///< \ru Освободить всю память. \en Free the whole memory. 
          void    Adjust();                                             ///< \ru Удалить лишнюю память. \en Free the unnecessary memory. 
          Type *  Add();                                                ///< \ru Добавить элемент в конец массива. \en Add an element to the end of the array. 
          Type *  AddItems ( size_t n );                                ///< \ru Добавить n элементов в конец массива. \en Add n elements to the end of the array. 
          Type *  Add      ( const Type & );                            ///< \ru Добавить элемент в конец массива. \en Add an element to the end of the array. 
          Type *  AddAt    ( const Type & ent, size_t index ) { return InsertInd( index, ent ); } ///< \ru Вставить элемент по индексу. \en Insert an element by the index. 
          Type *  AddAfter ( const Type &, size_t index );              ///< \ru Добавить элемент после указанного. \en Add an element after the specified one. 
          Type *  InsertObj( const Type & index, const Type & ent );    ///< \ru Вставить элемент перед указанным. \en Insert an element before the specified one. 
          Type *  InsertInd( size_t index, const Type & );              ///< \ru Вставить элемент перед указанным. \en Insert an element before the specified one. 
          Type *  InsertInd( size_t index );                            ///< \ru Вставить пустой элемент перед указанным. \en Insert the empty element before the specified one. 
          void    Remove( Type * firstItr, Type * lastItr );            ///< \ru Удалить элементы из массива начиная с позиции firstItr до lastItr-1 включительно. \en Delete elements from the array from firstItr to lastItr-1 inclusively. 
          void    RemoveInd( size_t firstIdx, size_t lastIdx );         ///< \ru Удалить элементы из массива начиная с индекса firstIdx до lastIdx-1 включительно. \en Delete elements from the array from firstIdx to lastIdx-1 inclusively. 
          void    RemoveInd( size_t idx );                              ///< \ru Удалить элемент из массива по индексу. \en Delete an element from array by the index. 
          size_t  RemoveObj( const Type & delObject );                  ///< \ru Удалить элемент из массива. \en Delete an element from array. 
          bool    Fill( size_t fillCount, const Type & fillData );      ///< \ru Заполнить массив значениями. \en Fill an array. 
          bool    FillZero( size_t fillCount, size_t startIndex = 0 );  ///< \ru Заполнить массив байтами содержащими 0. \en Fill an array by bites consisting of 0. 
          size_t  FindIt   ( const Type & ) const;                      ///< \ru Вернуть индекс элемента в массиве. \en Return an index of the element in the array. 
          bool    IsExist  ( const Type & ) const;                      ///< \ru true если элемент найден. \en true if the element is found. 
          size_t  Count()    const { return count; }                    ///< \ru Дать количество элементов массива. \en Get the number of elements in array. 
          ptrdiff_t MaxIndex() const { return ( static_cast<ptrdiff_t>(count) - 1); } ///< \ru Дать количество элементов массива. \en Get the number of elements in array. 

          bool    SetCArray( const Type * o, size_t count );            ///< \ru Присвоить значения из c-массива. \en Assign the value from the c-array. 

          void    Swap     ( SArray & arr );                            ///< \ru Обменять местами данные массивов. \en Swap data of arrays. 

  SArray<Type> &  operator =  ( const SArray<Type> & );           ///< \ru Оператор присваивания. \en Assignment operator. 
  SArray<Type> &  operator =  ( const std::vector<Type> & );      ///< \ru Оператор присваивания. \en Assignment operator. 
  SArray<Type> &  operator += ( const SArray<Type> & );           ///< \ru Оператор слияния. \en Merging operator. 
  SArray<Type> &  operator += ( const std::vector<Type> & );      ///< \ru Оператор слияния. \en Merging operator. 
          bool    operator == ( const SArray<Type>& w ) const;    ///< \ru Оператор равенства. \en Equality operator. 

  /// \ru Оператор доступа по индексу. \en Access by index operator. 
          Type &  operator []( size_t loc ) const;

  typedef int     (*CompFunc)( const Type *, const Type * ); ///< \ru Шаблон функции сортировки. \en A template of sorting function. 
  void            Sort ( CompFunc comp = CompareSArrayItems<Type> );  ///< \ru Сортировать массив. По умолчанию сортирует в порядке возрастания. \en Sort the array. Sort in ascending order by default. 

  const   Type *  GetAddr() const { return parr; }            ///< \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  const   Type *  GetEndAddr() const { return parr + count; }  ///< \ru Выдать указатель конца (следующим за крайним). \en Get a pointer of the end (which follows the last element). 

    /**
      \name \ru Унификация с контейнерами STL.
            \en Unification with STL-compatible containers. \~
      \{
    */

public: 
          bool    empty() const { return 0 == count; }          ///< \ru Проверить не пустой ли массив (т.е. не равен ли его размер 0). \en Test whether vector is empty (i.e. whether its size is 0).
          size_t  size() const { return count; }              ///< \ru Дать количество элементов массива. \en Get the number of elements in array. 
          void    reserve( size_t n ) { Reserve( n, false ); }  ///< \ru Зарезервировать место под столько элементов. \en Reserve space for a given number of elements. 
          void    resize( size_t n, Type val = Type() );        ///< \ru Изменить размер массива. \en Resizes the container so that it contains n elements. 
          size_t  capacity() const { return Upper(); }          ///< \ru Под какое количество элементов выделена память? \en What is the number of elements the memory is allocated for? 
          void    push_back( const Type & e ) { Add( e ); }     ///< \ru Добавить элемент в конец массива. \en Add an element to the end of the array. 
          void    pop_back() { RemoveInd( MaxIndex() ); }       ///< \ru Удалить элемент из конца массива. \en Removes the last element in the array, reducing the array size by one.
          template <class Iterator>
          void    insert( Iterator pos, const Type & e );       ///< \ru Вставить элемент перед указанным. \en Insert an element before the specified one. 
          template <class Iterator>
          void    erase( Iterator pos );                        ///< \ru Удалить элемент из массива по индексу. \en Delete an element from array by the index. 
          template <class Iterator>
          void    erase( Iterator first, Iterator last );       ///< \ru Удалить элементы из массива начиная с индекса first до last-1 включительно. \en Delete elements from the array from first to last-1 inclusively. 
          void    clear() { Flush(); }                          ///< \ru Обнулить количество элементов. \en Set the number of elements to null. 
          void    shrink_to_fit() { Adjust(); }                 ///< \ru Удалить лишнюю память. \en Free the unnecessary memory (Reduce capacity).  

          template <class Iterator>
          void    assign ( Iterator first, Iterator last ); ///< \ru Присвоить массиву новое содержимое, заменив его текущее содержимое. \en Assign new contents to the array, replacing its current contents.
          void    assign ( size_t n, const Type & val ) { resize( n, val ); } ///< \ru Присвоить массиву новое содержимое, заменив его текущее содержимое. \en Assign new contents to the array, replacing its current contents.

  const   Type *  begin()   const { return parr; }          ///< \ru Получить указатель на первый элемент массива. \en Get the pointer to the first array element. 
          Type *  begin()         { return parr; }          ///< \ru Получить указатель на первый элемент массива. \en Get the pointer to the first array element. 
  const   Type *  end()     const { return parr + count; }  ///< \ru Получить указатель на участок памяти после массива. \en Get the pointer to the piece of memory after the array. 
          Type *  end()           { return parr + count; }  ///< \ru Получить указатель на участок памяти после массива. \en Get the pointer to the piece of memory after the array.   
  const   Type *  cbegin()  const { return parr; }          ///< \ru Получить указатель на первый элемент массива. \en Get the pointer to the first array element. 
  const   Type *  cend()    const { return parr + count; }  ///< \ru Получить указатель на участок памяти после массива. \en Get the pointer to the piece of memory after the array. 
  const   Type &  front()   const;
          Type &  front();
  const   Type &  back()    const;
          Type &  back();

    /**
      \}
    */

protected :
  bool    CatchMemory();          ///< \ru Захватить память. \en Catch memory. 
  bool    AddMemory( size_t n );  ///< \ru Обеспечить место под n элементов, независимо от AutoDelta. \en Provide memory for n elements, independently from AutoDelta. 
  size_t  AutoDelta() const { return ::KsAutoDelta( count, delta ); } ///< \ru Вычислить автоприращение. \en Calculate autoincrement. 
  
  /// \ru Перезахватить память. \en Reallocate memory. 
  TEMPLATE_FRIEND bool    set_array_size  TEMPLATE_SUFFIX (       SArray<Type> &, size_t newSize, bool clear );
  /// \ru Добавить памяти под n элментов массива и вернуть указатель на начало выделеного участка памяти. \en Add memory for n elements of the array and return a pointer to the beginning of the selected piece of memory. 
  TEMPLATE_FRIEND Type *  add_n_to_array  TEMPLATE_SUFFIX (       SArray<Type> &, size_t n );
  /// \ru Найти элемент в массиве. \en Find an element in the array. 
  TEMPLATE_FRIEND size_t  find_in_array   TEMPLATE_SUFFIX ( const SArray<Type> &, const Type & object );
  /// \ru Заполнить fillCount элементов массива копиями объекта fillData. \en Fill fillCount elements of the array by copies of the object fillData. 
  TEMPLATE_FRIEND bool    fill_array      TEMPLATE_SUFFIX (       SArray<Type> &, size_t fillCount, const Type & fillData );
  /// \ru Заполнить fillCount элементов массива нулями. \en Fill fillCount elements of the array by nulls. 
  TEMPLATE_FRIEND bool    fill_array_zero TEMPLATE_SUFFIX (       SArray<Type> &, size_t fillCount, size_t startIndex );

  /// \ru Оператор чтения. \en Read operator. 
  TEMPLATE_FRIEND reader & CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader & in,        SArray<Type> & ref );
  /// \ru Оператор записи. \en Write operator. 
  TEMPLATE_FRIEND writer & CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer & out, const SArray<Type> & ref );
  /// \ru Оператор чтения. \en Read operator. 
  TEMPLATE_FRIEND reader & CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader & in,        SArray<Type> *& ptr );
  /// \ru Оператор записи. \en Write operator. 
  TEMPLATE_FRIEND writer & CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer & out, const SArray<Type> * ptr );

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
inline void * SArray<Type>::operator new( size_t size ) {
  return ::Allocate( size, typeid(SArray<Type>).name() );
}

//------------------------------------------------------------------------------
// \ru Перегрузка оператора delete. \en Overloading of the "delete" operator. 
// ---
template <class Type>
inline void SArray<Type>::operator delete( void * ptr, size_t size ) {
  ::Free( ptr, size, typeid(SArray<Type>).name() );
}
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_


//------------------------------------------------------------------------------
// \ru Конструктор массива \en Constructor of an array 
// ---
template <class Type>
inline SArray<Type>::SArray( size_t i_max, uint16 i_delta )
  : count( 0 )
  , upper( 0 )
  , delta( i_delta )
  , parr ( 0 ) // i_max ? (Type*)new char[ i_max * sizeof(Type) ] : 0 )
{
  if ( !set_array_size( *this, i_max, true/*\ru clear - здесь неважно \en clear - it is not necessary here */ ) && !ExceptionMode::IsEnabled() )
    throw std::bad_alloc(); // \ru Бросить исключение при любом режиме. \en Throw exception in case of any mode.
}


//------------------------------------------------------------------------------
// \ru Конструктор копирования массива \en Copy-constructor of an array 
// ---
template <class Type>
inline SArray<Type>::SArray( const SArray<Type> & o )
  : count( 0 )
  , upper( 0 )
  , delta( o.delta )
  , parr ( 0 )
{
  if ( o.upper ) {
    // \ru operator = копирует только o.count элементов, а нужно все до o.upper \en the operator = copies only o.count elements, but there must be copied all elements to o.upper 
    if ( set_array_size( *this, o.upper, true/*\ru clear - здесь неважно \en clear - it is not necessary here */ ) ) {
      memcpy( static_cast<void *>(parr), static_cast<const void *>(o.parr), o.upper * sizeof(Type) );
      count = o.count;
    }
    else if ( !ExceptionMode::IsEnabled() )
      throw std::bad_alloc(); // \ru Бросить исключение при любом режиме. \en Throw exception in case of any mode.
  }
}


//------------------------------------------------------------------------------
// \ru Конструктор копирования массива \en Copy-constructor of an array 
// ---
template <class Type>
inline SArray<Type>::SArray( const std::vector<Type> & o )
  : count( 0 )
  , upper( 0 )
  , delta( 1 )
  , parr ( 0 )
{
  const size_t oCount = o.size();
  if ( oCount ) {
    // \ru operator = копирует только o.count элементов, а нужно все до o.upper \en the operator = copies only o.count elements, but there must be copied all elements to o.upper 
    if ( set_array_size(*this, oCount, true/*\ru clear - здесь неважно \en clear - it is not necessary here */) ) {
      memcpy( parr, &o[0], oCount * sizeof(Type) );
      count = oCount;
    }
    else if ( !ExceptionMode::IsEnabled() )
      throw std::bad_alloc(); // \ru Бросить исключение при любом режиме. \en Throw exception in case of any mode.
  }
}

//------------------------------------------------------------------------------
// \ru Оператор доступа по индексу. \en Access by index operator. 
// ---
template <class Type>
inline Type & SArray<Type>::operator []( size_t loc ) const
{
  return parr[loc];
}

//------------------------------------------------------------------------------
// \ru Доступ к первому элементу. \en Access to th first element. 
// ---
template <class Type>
inline const Type & SArray<Type>::front() const
{
  PRECONDITION( !empty() );
  return *parr;
}

//------------------------------------------------------------------------------
// \ru Доступ к первому элементу. \en Access to th first element. 
// ---
template <class Type>
inline Type & SArray<Type>::front()
{
  PRECONDITION( !empty() );
  return *parr;
}


//------------------------------------------------------------------------------
// \ru Доступ к последнему элементу. \en Access to th last element. 
// ---
template <class Type>
inline const Type & SArray<Type>::back() const
{
  PRECONDITION( !empty() );
  return *( parr + count - 1 );
}

//------------------------------------------------------------------------------
// \ru Доступ к последнему элементу. \en Access to th last element. 
// ---
template <class Type>
inline Type &SArray<Type>::back()
{
  PRECONDITION( !empty() );
  return *( parr + count - 1 );
}

//------------------------------------------------------------------------------
// \ru Указать новый размер массива. \en Set the new size of an array.
// \ru если clear = true, то массив очистится !!! \en if 'clear' is true than the array will be cleared !!! 
// \ru если clear = true, то присвоить count=0 и старое содержимое не копировать \en if clear = true then set count=0 and do not copy the old content 
// ---
template <class Type>
inline bool SArray<Type>::SetSize( size_t newSize, bool clear ) {
  return set_array_size( *this, newSize, clear );
}


//------------------------------------------------------------------------------
// \ru Зарезервировать место под n элементов. \en Reserve memory for n elements. 
// ---
template <class Type>
inline bool SArray<Type>::Reserve( size_t n, bool addAdditionalSpace ) {
  if ( addAdditionalSpace ) {
    size_t nn = count + n;
    if ( upper < nn )
      return set_array_size( *this, nn, false/*clear*/ );
  }
  else {
    // \ru Захватить память, если требуется памяти больше, чем есть сейчас. \en if there is required more memory that exists at the moment catch it.
    if ( upper < n )
      return set_array_size( *this, n, false/*clear*/ );
    else {
      // C3D_ASSERT( upper <= n ); // Use SetSize!!!
    }
  }
  return true;
}


//------------------------------------------------------------------------------
// \ru Добавить память под n элементов. \en Add memory for n elements.
// ---
template <class Type>
inline bool SArray<Type>::AddMemory( size_t n ) {
  if ( upper - count < n )
    return set_array_size( *this, count + n, false/*clear*/ );
  return true;
}


//------------------------------------------------------------------------------
// \ru Изменить размер массива. \en Resizes the container so that it contains n elements. 
// ---
template <class Type>
inline void SArray<Type>::resize( size_t n, Type val )
{
  if ( n > count ) {
    size_t n0 = count;
    if ( AddItems( n - count ) != 0 ) {
      if ( parr != nullptr ) {
        for ( size_t k = n0; k < n; ++k )
          parr[k] = val;
      }
    }
  }
  else if ( n < count ) {
    count = n;
    Adjust();
  }
}


//------------------------------------------------------------------------------
// \ru отсечь неиспользуемую часть \en cut the useless part 
// ---
template <class Type>
inline void SArray<Type>::Adjust() {
  if ( count < upper )
    set_array_size( *this, count, false/*clear*/ );
}


//------------------------------------------------------------------------------
// \ru добавить 1 элемент в конец массива и вернуть указатель на него \en add 1 element to the end of the array and return a pointer to it 
// ---
template <class Type>
inline Type * SArray<Type>::Add() {
  if ( CatchMemory() )
    return &parr[ count++ ];
  return nullptr;
}


//------------------------------------------------------------------------------
// \ru добавить n элементов в конец массива и вернуть указатель на первый добавленный \en add n elements to the end of the array and return a pointer to the first of added elements 
// ---
template <class Type>
inline Type * SArray<Type>::AddItems( size_t n ) {
  return add_n_to_array( *this, n );
}


//------------------------------------------------------------------------------
// \ru добавить 1 элемент в конец массива \en add 1 element to the end of array 
// ---
template <class Type>
inline Type * SArray<Type>::Add( const Type & ent ) 
{
  if ( CatchMemory() )
    return static_cast<Type *>( memcpy(static_cast<void *>(parr+count++), static_cast<const void *>(&ent), sizeof(Type)) );
  return nullptr;
}


//------------------------------------------------------------------------------
// \ru добавить 1 элемент после заданного \en add 1 element after the given one 
// ---
template <class Type>
inline Type * SArray<Type>::AddAfter( const Type & ent, size_t index ) {
  PRECONDITION( index < count );
  if ( index < count && CatchMemory() ) {

    memmove( parr + index + 2, parr + index + 1, sizeof(Type)*(count - index - 1) );
    count++;

    return static_cast<Type*>( memcpy(parr + index + 1, &ent, sizeof(Type)) );
  }
  return nullptr;
}


//------------------------------------------------------------------------------
// \ru вставить 1 элемент перед указанным \en insert 1 element before the specified one 
// ---
template <class Type>
inline Type * SArray<Type>::InsertInd( size_t index, const Type & ent ) {
  PRECONDITION( index <= count );
  if ( index <= count && CatchMemory() ) { // \ru добавить памяти, если все использовано \en add memory if whole allocated memory is used 

    if ( index >= count )
      index = count;
    else {
      // \ru передвинем вправо все элементы массива с последнего до указанного \en move to the right all elements of the array from the last to the specified one 
      memmove( parr + index + 1, parr + index, (count - index) * sizeof(Type) );
    }
    count++;

    return static_cast<Type*>( memcpy(parr + index, &ent, sizeof(Type)) );  // \ru записываем новый элемент \en writing new element 
  }
  return nullptr;
}


//------------------------------------------------------------------------------
// \ru вставить пустой элемент перед указанным \en insert the empty element before the specified one. 
// ---
template <class Type>
inline Type * SArray<Type>::InsertInd( size_t index ) {
  PRECONDITION( index <= count );
  if ( index <= count && CatchMemory() ) { // \ru добавить памяти, если все использовано \en add memory if whole allocated memory is used 

  // \ru передвинем вправо все элементы массива с последнего до указанного \en move to the right all elements of the array from the last to the specified one 
    memmove( parr + index + 1, parr + index, (count - index) * sizeof(Type) );
    count++;

    return static_cast<Type*>( parr + index );  // \ru записываем новый элемент \en writing new element 
  }
  return nullptr;
}


//------------------------------------------------------------------------------
// \ru вставить элемент перед указанным \en insert element before the specified one 
// ---
template <class Type>
inline Type * SArray<Type>::InsertObj( const Type & ind, const Type & ent ) {
  size_t index = FindIt(ind);
  return ( index != SYS_MAX_T) ? InsertInd( index, ent ) : 0;
}


//------------------------------------------------------------------------------
// \ru вернуть индекс элемента в массиве \en return an index of the element in the array 
// ---
template <class Type>
inline size_t SArray<Type>::FindIt( const Type & object ) const {
  return find_in_array( *this, object );
}


//------------------------------------------------------------------------------
// \ru true если элемент найден \en true if the element was found 
// ---
template <class Type>
inline bool SArray<Type>::IsExist( const Type & object ) const {
  return find_in_array( *this, object ) != SYS_MAX_T;
}


//------------------------------------------------------------------------------
// \ru удалить элемент из массива \en delete an element from array 
// ---
template <class Type>
inline size_t SArray<Type>::RemoveObj( const Type & delObject )
{
  size_t ind = FindIt( delObject );
  if ( ind != SYS_MAX_T )
    RemoveInd( ind );
  return ind;
}


//------------------------------------------------------------------------------
// \ru удалить элемент из массива \en delete an element from array 
// ---
template <class Type>
inline void SArray<Type>::RemoveInd( size_t idx ) 
{
  // RemoveInd( idx, idx+1 );
  if ( parr && idx < count ) {
    Type * ptr = parr + idx;
    Remove( ptr, ptr+1 );
  }
}


//------------------------------------------------------------------------------
// \ru Вставить элемент перед указанным. \en Insert an element before the specified one. 
// ---
template <class Type>
template <class Iterator>
void SArray<Type>::insert( Iterator pos, const Type & e )
{
  if ( !begin() ) {
    reserve( 1 );
    pos = begin();
  }
  if ( begin() ) {
    const ptrdiff_t k = std::distance( static_cast<Iterator>(begin()), pos );
    if ( k >= 0 && k <= static_cast<ptrdiff_t>(count) )
      InsertInd( k, e );
  }
}


//------------------------------------------------------------------------------
// \ru Удалить элемент из массива по индексу. \en Delete an element from array by the index. 
// ---
template <class Type>
template <class Iterator>
void SArray<Type>::erase( Iterator pos )
{
  if ( begin() ) {
    const ptrdiff_t k = std::distance( static_cast<Iterator>(begin()), pos );
    if ( k >= 0 && k < static_cast<ptrdiff_t>(count) )
      RemoveInd( k );
  }
}

//------------------------------------------------------------------------------
// \ru Удалить элементы из массива начиная с индекса first до last-1 включительно. \en Delete elements from the array from first to last-1 inclusively. 
// ---
template <class Type>
template <class Iterator>
void SArray<Type>::erase( Iterator first, Iterator last )
{
  if ( begin() ) {
    const ptrdiff_t k1 = std::distance( static_cast<Iterator>(begin()), first );
    const ptrdiff_t k2 = std::distance( static_cast<Iterator>(begin()), last );

    if ( k1 >= 0 && k1 < k2 && k2 <= static_cast<ptrdiff_t>(count) ) {
      RemoveInd( k1, k2 );
    }
  }
}


//------------------------------------------------------------------------------
// \ru Удалить группу элементов начиная с firstIdx до lastIdx-1 включительно \en Delete the group of elements from firstIdx to lastIdx-1 inclusively 
// ---
template <class Type>
inline void SArray<Type>::RemoveInd( size_t firstIdx, size_t lastIdx ) {
  if ( parr )
    Remove( parr+firstIdx, parr+lastIdx );
}


//------------------------------------------------------------------------------
// \ru Удалить элементы из массива начиная с позиции firstItr до lastItr-1 включительно \en Delete elements from the array from firstItr to lastItr-1 inclusively 
// ---
template <class Type>
inline void SArray<Type>::Remove( Type * firstItr, Type * lastItr )
{  
  PRECONDITION( firstItr >= parr && firstItr < lastItr && (lastItr - parr) <= static_cast<ptrdiff_t>(count) );
  if ( firstItr >= parr && firstItr < lastItr ) {
    ptrdiff_t copyCount = ( parr + count ) - lastItr;
    if ( copyCount >= 0 ) {
      if ( copyCount > 0 )
        memmove( static_cast<void *>( firstItr ), static_cast<const void *>( lastItr ), copyCount * sizeof( Type ) );
      count -= ( lastItr - firstItr );
    }
  }
}


//------------------------------------------------------------------------------
// \ru заполнить массив значениями \en fill an array 
// ---
template <class Type>
inline bool SArray<Type>::Fill( size_t fillCount, const Type & fillData ) {
  return fill_array( *this, fillCount, fillData );
}


//------------------------------------------------------------------------------
// \ru заполнить массив байтами содержащими 0 \en fill an array by bites consisting of 0 
// \ru размер полученного массива устанавливается в fillCount + startIndex \en a size of the obtained array is set to fillCount + startIndex. 
// ---
template <class Type>
inline bool SArray<Type>::FillZero( size_t fillCount, size_t startIndex ) {
  return fill_array_zero( *this, fillCount, startIndex );
}


//------------------------------------------------------------------------------
// \ru присвоение массива массиву \en assignment of an array to array 
// ---
template <class Type>
inline SArray<Type> & SArray<Type>::operator = ( const SArray<Type> & o ) {
  if ( this != &o ) { // \ru при присваивании самому себе delete делать нельзя \en when it is assigned to itself operato delete should not be used 
    Flush();
    if ( AddMemory(o.count) ) {  // \ru обеспечить память на такое кол-во элементов \en allocate memory for the given number of elements 
      count = o.count;
      if ( count && parr )
        memcpy( static_cast<void *>(parr), static_cast<const void *>(o.parr), count * sizeof(Type) );
    }
  }
  return *this;
}


//------------------------------------------------------------------------------
// \ru присвоение массива массиву \en assignment of an array to array 
// ---
template <class Type>
inline SArray<Type> & SArray<Type>::operator = ( const std::vector<Type> & o ) {
  Flush();
  if ( AddMemory(o.size()) ) {  // \ru обеспечить память на такое кол-во элементов \en allocate memory for the given number of elements 
    count = o.size();
    if ( count && parr )
      memcpy( static_cast<void *>(parr), static_cast<const void *>(&o[0]), count * sizeof(Type) );
  }
  return *this;
}


//------------------------------------------------------------------------------
// \ru присвоение массива массиву \en assignment of an array to array 
// ---
template <class Type>
inline bool SArray<Type>::SetCArray( const Type * o, size_t countC )
{
  Flush();
  bool bRes = AddMemory( countC );
  if ( bRes ) {  // \ru обеспечить память на такое кол-во элементов \en allocate memory for the given number of elements   
    count = countC;
    if ( count && parr )
      memcpy( parr, o, count * sizeof(Type) );
  }
  return bRes;
}


//-------------------------------------------------------------------------------
// \ru обменять местами данные массивов \en swap data of arrays 
// ---
template <class Type>
void SArray<Type>::Swap( SArray & arr ) {
  std::swap( count, arr.count );
  std::swap( upper, arr.upper );
  std::swap( delta, arr.delta );
  std::swap( parr,  arr.parr  );
}


//------------------------------------------------------------------------------
// \ru добавление массива к массиву \en add the array to the array 
// ---
template <class Type>
inline SArray<Type> & SArray<Type>::operator += ( const SArray<Type> & o ) {
  if ( o.size() ) {
    size_t addSize = o.size();
    if ( AddMemory(addSize) ) {  // \ru обеспечить память на такое кол-во элементов \en allocate memory for the given number of elements 
      if ( parr )
        memcpy( static_cast<void *>(parr + count), static_cast<const void *>(o.parr), addSize * sizeof(Type) );
      count += addSize;
    }
  }
  return *this;
}


//------------------------------------------------------------------------------
// \ru добавление массива к массиву \en add the array to the array 
// ---
template <class Type>
inline SArray<Type> & SArray<Type>::operator += ( const std::vector<Type> & o ) {
  if ( o.size() ) {
    size_t addSize = o.size();
    if ( AddMemory(addSize) ) {  // \ru обеспечить память на такое кол-во элементов \en allocate memory for the given number of elements 
      if ( parr )
        memcpy( static_cast<void *>(parr + count), static_cast<const void *>(&o[0]), addSize * sizeof(Type) );
      count += addSize;
    }
  }
  return *this;
}


//------------------------------------------------------------------------------
// \ru Если вся память использована - захватить по больше \en If the whole memory is used then catch more memory 
// \ru (применяется перед добавлением в массив одного элемента) \en (it is used before adding one element in the array) 
// ---
template <class Type>
inline bool SArray<Type>::CatchMemory() {
  if ( upper == count )
    return set_array_size( *this, upper + AutoDelta(), false/*clear*/ );
  if ( parr != nullptr )
    return true;
  return false;
}


//------------------------------------------------------------------------------
// \ru сортировать массив \en sort the array 
// --- 
template <class Type>
inline void SArray<Type>::Sort( CompFunc fcmp ) {
  ::KsQSort( static_cast<void *>(parr), count, sizeof(Type), reinterpret_cast<KsQSortCompFunc>(fcmp) );
}


//----------------------------------------------------------------------------------------
// \ru Присвоить массиву новое содержимое, заменив его текущее содержимое. 
// \en Assign new contents to the array, replacing its current contents.
//---
template <class Type>
template <class Iterator>
void SArray<Type>::assign( Iterator first, Iterator last )
{
  const ptrdiff_t newCount = std::distance( first, last );
  if ( set_array_size(*this, newCount, true) ) {
    PRECONDITION( newCount <= static_cast<ptrdiff_t>(upper) && count == 0 );
    for ( ; first != last; ++first, ++count ) {
      parr[count] = *first; // SKIP_SA
    }
  }
}


//------------------------------------------------------------------------------
// \ru Если захвачен не такой размер памяти, то захватить новый если clear = true, то присвоить arr.count=0 и старое содержимое не копировать.
// \en If the size of caught memory is not appropriated then allocate memory again if clear = true then set arr.count=0 and do not copy the old content. 
// ---
template <class Type>
bool set_array_size( SArray<Type> & arr, size_t newSize, bool clear )
{
  bool res = true;
  if ( newSize != arr.upper ) {
    size_t sizeOfType = sizeof(Type);

    // \ru половина адресного пространства для 64- и 32-разрядного приложения \en a half of address space for 64- and 32-bit application  
    //    if ( double(newSize) * sizeOfType < /*OV_x64 0x7FFFFFFF*/(double)SYS_MAX_ST ) { //-V113
    if ( ::TestNewSize( sizeOfType, newSize ) ) {
      try {
#ifdef __REALLOC_ARRAYS_STATISTIC_
        void * oldParr = arr.parr;
        size_t oldSize = arr.upper;
#endif // __REALLOC_ARRAYS_STATISTIC_

#ifdef USE_REALLOC_IN_ARRAYS
        //YYK V15 #77319 Некорректная отрисовка на видеокартах семейства NVIDIA Quadro, 
        //YYK V15        требуется 16-байтовое выравнивание массивов вершин и нормалей.
        //YYK V15        Исправлять приходится глобально для всех экземпляров SArray
        //YYK V15 arr.parr = (Type*) REALLOC_ARRAY_SIZE( arr.parr, newSize * sizeOfType, clear );
#ifdef C3D_WINDOWS //_MSC_VER // win
        arr.parr = (Type *)_aligned_realloc( arr.parr, newSize * sizeOfType, 16 );
#else
        arr.parr = (Type *)REALLOC_ARRAY_SIZE( arr.parr, newSize * sizeOfType, clear );
#endif // win
#else
        //YYK V15 #77319 Type * p_tmp = newSize ? (Type*)new char[ newSize * sizeOfType ] : 0;
#ifdef C3D_WINDOWS //_MSC_VER // win
        Type * p_tmp = newSize ? static_cast<Type*>( _aligned_malloc(newSize * sizeOfType, 16) ) : nullptr;
#else
        Type * p_tmp = newSize ? reinterpret_cast<Type*>(new char[newSize * sizeOfType]) : 0;
#endif // win

        if ( !clear && arr.parr && p_tmp )
          memcpy( static_cast<void *>(p_tmp), static_cast<const void *>(arr.parr), std_min(arr.upper, newSize) * sizeOfType );

        if ( arr.parr )
          //YYK V15 #77319 delete [] (char *) arr.parr;
#ifdef C3D_WINDOWS //_MSC_VER // win
          _aligned_free( arr.parr );
#else
          delete[]reinterpret_cast<char *>( arr.parr );
#endif // win

        arr.parr = p_tmp;
#endif //USE_REALLOC_IN_ARRAYS

        arr.upper = newSize;

#ifdef __REALLOC_ARRAYS_STATISTIC_
        REALLOC_ARRAY_STATISTICS( oldParr, oldSize * sizeOfType, arr.parr, newSize * sizeOfType, 0/*SArray*/ );
#endif // __REALLOC_ARRAYS_STATISTIC_
      }
      catch ( const std::bad_alloc & ) {
        newSize = 0; // \ru т.к. ниже есть код с применением newSize \en because there is a code with using of newSize below 
        C3D_CONTROLED_THROW;
        res = false;
      }
      catch ( ... ) {
        if ( newSize == 0 ) { // \ru Не смогли корректно удалить arr.parr. \en Failed to delete arr.parr correctly.
          arr.parr = nullptr;
          arr.upper = newSize;
        }
        newSize = 0; // \ru т.к. ниже есть код с применением newSize \en because there is a code with using of newSize below 
        C3D_CONTROLED_THROW;
        res = false;
      }
    }
    else {
      PRECONDITION( false ); // \ru не бывает столько памяти \en incorrect size of memory 
      newSize = 0; // \ru т.к. ниже есть код с применением newSize \en because there is a code with using of newSize below 
      C3D_CONTROLED_THROW_EX( std::bad_alloc() );
      res = false;
    }
  }

  if ( clear )
    arr.count = 0;
  else
    if ( newSize < arr.count )
      arr.count = newSize;

  return res;
}


//------------------------------------------------------------------------------
// \ru добавить n элементов в конец массива и вернуть указатель на первый добавленный \en add n elements to the end of the array and return a pointer to the first of added elements 
// ---
template <class Type>
Type * add_n_to_array( SArray<Type> & to, size_t n ) {
  if ( n ) {
    if ( to.AddMemory(n) ) {  // \ru обеспечить память на такое кол-во элементов \en allocate memory for the given number of elements 
      size_t oldCount = to.count;
      to.count += n;
      return &to[oldCount];
    }
  }
  return 0;
}


//------------------------------------------------------------------------------
// \ru Внеклассные функции для сравнения содержимого объектов в методах SArray. У объектов должен быть реализован оператор сравнения на равенство.
// \en Out-of-class functions for the comparison of objects contents in the methods of SArray. There should be implemented an operator of comparison for equality in objects.
// ---
template <class Type>
bool IsEqualSArrayItems( const Type & item1, const Type & item2 ) { 
  return item1 == item2;
}

template <class Type>
inline bool IsEqualSArrayItems( const std::pair<Type,Type> & item1, const std::pair<Type,Type> & item2 ) {
  return ((item1.first == item2.first) && (item1.second == item2.second));
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
size_t find_in_array( const SArray<Type> & arr, const Type & object ) {
  for ( size_t  i = 0; i < arr.count; i++ )
    // \ru OV K6 Использование функции memcmp дает неверные результаты, если для участка кода, \en OV K6 Using of the function memcmp causes incorrect results if for a part of the code  
    // \ru где она вызывается, поставлено выравнивание 8, а размер сравниваемых данные не кратен 8. \en where it is called set the alignment 8 but the size of compared date is not a multiple of 8.  
    // \ru Пусть каждый объект сам решает как ему сравниваться с себе подобным \en Let every object define the way of comparison to the similar object 
    //OV K6     if ( !memcmp( &arr[i], &object, sizeof(Type) ) )
    if ( ::IsEqualSArrayItems( arr[i], object ) )
      return i;

  return SYS_MAX_T;
}


//-------------------------------------------------------------------------------
// \ru Оператор сравнения двух массивов. \en An operator of two arrays comparison.
/**
  \details  
  \ru Осуществляется поэлементной стравнение с возможностью перегрузки
  операции сравния с помощью метода IsEqualSArrayItems.
  \en An element-by-element comparison is performed with the ability to redefine the
  comparison operation using the overloading global function IsEqualSArrayItems.
*/
// ---
template <class Type>
inline bool SArray<Type>::operator == ( const SArray<Type> & w ) const 
{
  if ( count != w.count )
    return false;
  /*
    При размещении в памяти с выравниванием не равным 1, между элементами массива
    возможно появление "дырок" заполненного случайным мусором, т.к. сравнивать этот мусор
    нам незачем, будем сравнивать содержимое массивов поэлементно (через оператор == или 
    путем перегрузки IsEqualSArrayItems).

    While the memory allocation with alignment which is not equal 1 between elements of 
    the array  may appear "holes" filled by random trash, since there is not reason to 
    compare this trash we will compare the content of arrays element by element 
    (using the operator == of an object or by overloading the func IsEqualSArrayItems).
  */
  for ( size_t i = 0; i < count; i++ ) 
  {
    if ( !::IsEqualSArrayItems( (*this)[i], w[i] ) )
      return false;
  }

  return true;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
bool fill_array( SArray<Type> & arr, size_t fillCount, const Type & fillData ) 
{
  arr.Flush();
  bool res = arr.AddMemory( fillCount );
  if ( res ) {  // \ru обеспечить память на такое кол-во элементов \en allocate memory for the given number of elements 
    arr.count = fillCount;
    for ( size_t i = 0; i < arr.count; i++ ) {
      arr[i] = fillData; // Потребуем наличия оператора присвоения
    }
  }
  return res;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
bool fill_array_zero( SArray<Type> & arr, size_t fillCount, size_t startIndex ) 
{
  bool res = arr.AddMemory( fillCount + startIndex );  // \ru обеспечить память на такое кол-во элементов \en allocate memory for the given number of elements 
  if ( res ) {
    arr.count = fillCount + startIndex;       // \ru установить размер массива \en set the size of the array 
    memset( static_cast<void*>(&arr[startIndex]), 0, fillCount * sizeof(Type) );
  }
  return res;
}


////////////////////////////////////////////////////////////////////////////////
//
// \ru Массив с итераторными функциями \en An Array with Iterator Functions 
//
////////////////////////////////////////////////////////////////////////////////


FORVARD_DECL_TEMPLATE_TYPENAME( class   SIArray );
FORVARD_DECL_TEMPLATE_TYPENAME( void    for_each_in_array  ( const SIArray<Type> &, typename SIArray<Type>::IteratorFunc ) );
FORVARD_DECL_TEMPLATE_TYPENAME( void    for_each_in_array  ( const SIArray<Type> &, typename SIArray<Type>::ParIteratorFunc, void * pars ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t  first_that_in_array( const SIArray<Type> &, typename SIArray<Type>::CompareFunc,     void * pars, size_t from ) );


//-----------------------------------------------------------------------------
/** \brief \ru Одномерный массив обьектов с итераторными функциями.
           \en One-dimensional array of objects with iterator functions. \~
  \details \ru Одномерный массив обьектов, не содержащих указателей (вернее, не имеющих деструкторов).
    В массиве нельзя хранить объекты, содержащие указатели или классы с указателями, 
    а также абстрактные классы с наследниками. \n
           \en One-dimensional array of objects without pointers (i.e. without destructors).
    The array should not contain objects with pointers or classes with pointers, 
    or abstract classes with inheritors. \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class SIArray : public SArray<Type> {
public :
  SIArray( size_t i_max=0, uint16 i_delta=1 ) : SArray<Type>( i_max, i_delta ) {}
  SIArray( const SArray<Type> & other )       : SArray<Type>( other ) {}

  typedef void    (*IteratorFunc) ( Type & obj );
  void            ForEach( IteratorFunc func ) const;
  typedef void    (*ParIteratorFunc) ( Type & obj, void * pars );
  void            ForEach( ParIteratorFunc func, void * pars ) const;
  typedef bool    (*CompareFunc) ( Type & obj, void * pars );
  size_t          FirstThat( CompareFunc func, void * pars, size_t from = 0 ) const;

private:
  TEMPLATE_FRIEND void    for_each_in_array   TEMPLATE_SUFFIX ( const SIArray<Type> &, IteratorFunc );
  TEMPLATE_FRIEND void    for_each_in_array   TEMPLATE_SUFFIX ( const SIArray<Type> &, ParIteratorFunc, void * pars );
  TEMPLATE_FRIEND size_t  first_that_in_array TEMPLATE_SUFFIX ( const SIArray<Type> &, CompareFunc,     void * pars, size_t from );
};


//------------------------------------------------------------------------------
// \ru выполнить функцию для каждого элемента \en perform the function for every element 
// ---
template <class Type>
inline void SIArray<Type>::ForEach( IteratorFunc func ) const {
  for_each_in_array<Type>( *this, func );
}


//------------------------------------------------------------------------------
// \ru выполнить функцию с параметрами для каждого элемента \en perform the function with parameters for every element 
// ---
template <class Type>
inline void SIArray<Type>::ForEach( ParIteratorFunc func, void * pars ) const {
  for_each_in_array<Type>( *this, func, pars );
}


//------------------------------------------------------------------------------
// \ru найти элемент по условию \en find an element by condition 
// ---
template <class Type>
inline size_t SIArray<Type>::FirstThat( CompareFunc func, void* pars, size_t from ) const {
  return first_that_in_array<Type>( *this, func, pars, from );
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
void for_each_in_array( const SIArray<Type> & arr, typename SIArray<Type>::IteratorFunc func ) {
  for( size_t i = 0; i < arr.Count(); i++ )
    func( arr[i] );
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
void for_each_in_array( const SIArray<Type>& arr, typename SIArray<Type>::ParIteratorFunc func, void* pars ) {
  for( size_t i = 0; i < arr.Count(); i++ )
    func( arr[i], pars );
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
size_t first_that_in_array( const SIArray<Type> & arr, typename SIArray<Type>::CompareFunc func, void * pars, size_t from ) {
  for( size_t i = from; i < arr.Count(); i++ ) {
    if ( func(arr[i],pars) )
      return i;
  }
  return SYS_MAX_T;
}


//-----------------------------------------------------------------------------
/** \brief \ru Инициализация массива списком значений.
           \en Initialization of an array by the list of values. \~
  \details \ru Инициализация массива списком значений. \n
    Пример использования: \n
    SArray_assign<double> arr; \n
    arr = val1, val2, val3; \n
           \en Initialization of an array by the list of values. \n
    example of using: \n
    SArray_assign<double> arr; \n
    arr = val1, val2, val3; \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class T>
class SArray_assign {
protected:
  SArray<T> m_arr; ///< \ru Массив объектов. \en An array of objects. 
public:
  /// \ru Конструктор. \en Constructor. 
  SArray_assign() : m_arr() {}
  /// \ru Конструктор. \en Constructor. 
  explicit SArray_assign( size_t k ) : m_arr( k, 1 ) {}
  /// \ru Конструктор копирования. \en Copy constructor. 
  SArray_assign( const SArray_assign & other ) : m_arr( other.m_arr ) {}
  /// \ru Оператор добавления "через запятую". \en Operator of adding "separated by commas". 
  SArray_assign & operator , (const T & t)  { Set().Add( t ); return *this; }
  /// \ru Оператор присваивания. \en The assignment operator. 
  SArray_assign & operator = (const T & t)  { Set().Flush(); Set().Add( t ); return *this; }

  operator SArray<T>   () const { return m_arr; } ///< \ru Оператор копирования данных. \en An operator of data copying. 
  operator SArray<T> & ()       { return Set(); } ///< \ru Оператор доступа. \en An access operator. 
  const SArray<T> & Get() const { return m_arr; } ///< \ru Оператор доступа. \en An access operator. 
        SArray<T> & Set()       { return m_arr; } ///< \ru Оператор доступа. \en An access operator. 
};


//-------------------------------------------------------------------------------
// \ru функции сравнения двух массивов \en functions of two arrays comparison 
// ---
template <class Type>
inline bool Eq( const SArray<Type> & ar1, const SArray<Type> & ar2 ) {
  return ( ar1 == ar2 );
}


//------------------------------------------------------------------------------
// \ru Внеклассные ф-ии для сравнения содержимого объектов в методах SArray \en Out-of-class functions for the comparison of objects contents in the methods of SArray 
// \ru должны выполнять действия, аналогичные ::memcmp( &obj1, obj2, sizeof(obj1) ) \en they should perform similar to ::memcmp( &obj1, obj2, sizeof(obj1) ) operations 
// ---
template <class Type> 
bool IsLessThanSArrayItems( const Type& obj1, const Type& obj2 ); 

inline bool IsLessThanSArrayItems( const float  &obj1, const float  &obj2 ) { return obj1 < obj2; }
inline bool IsLessThanSArrayItems( const double &obj1, const double &obj2 ) { return obj1 < obj2; }

inline bool IsLessThanSArrayItems( const int8   &obj1, const int8   &obj2 ) { return obj1 < obj2; }
inline bool IsLessThanSArrayItems( const uint8  &obj1, const uint8  &obj2 ) { return obj1 < obj2; }
inline bool IsLessThanSArrayItems( const int16  &obj1, const int16  &obj2 ) { return obj1 < obj2; }
inline bool IsLessThanSArrayItems( const uint16 &obj1, const uint16 &obj2 ) { return obj1 < obj2; }
inline bool IsLessThanSArrayItems( const int    &obj1, const int    &obj2 ) { return obj1 < obj2; }
inline bool IsLessThanSArrayItems( const uint   &obj1, const uint   &obj2 ) { return obj1 < obj2; }
#ifdef C3D_WINDOWS //_MSC_VER // Linux identical int/uint and int32/uint32 
inline bool IsLessThanSArrayItems( const int32  &obj1, const int32  &obj2 ) { return obj1 < obj2; }
inline bool IsLessThanSArrayItems( const uint32 &obj1, const uint32 &obj2 ) { return obj1 < obj2; }
#endif // C3D_WINDOWS

#if defined(PLATFORM_64) // \ru x32 совпадение типов ptrdiff_t и int \en x32 coincidence of ptrdiff_t and int types 
inline bool IsLessThanSArrayItems( const size_t    &obj1, const size_t    &obj2 ) { return obj1 < obj2; }
inline bool IsLessThanSArrayItems( const ptrdiff_t &obj1, const ptrdiff_t &obj2 ) { return obj1 < obj2; }
#endif // PLATFORM_64


//-------------------------------------------------------------------------------
// \ru функции сравнения двух массивов \en functions of two arrays comparison 
// ---
template <class Type>
inline bool Less( const SArray<Type> & ar1, const SArray<Type> & ar2 ) {
  ptrdiff_t count1 = ar1.Count();
  ptrdiff_t count2 = ar2.Count();

  // \ru OV K6 При размещении в памяти с выравниванием не равным 1, между элементами массива \en OV K6 While the memory allocation with alignment which is not equal 1 between elements of the array  
  // \ru возможно появление "дырок" заполненного случайным мусором, т.к. сравнивать этот мусор \en may appear "holes" filled by random trash, since there is not reason to compare this trash 
  // \ru нам ни к чему, будем сравнивать содержимое массивов поэлементно \en we will compare contents of arrays element by element 
  //OV K6 return ::memcmp( ar1.GetAddr(), ar2.GetAddr(), sizeof(Type) * count1 ) < 0;
  for ( ptrdiff_t i = 0, c = std_min(count1, count2); i < c; i++ ) {
    const Type & obj1 = ar1[i]; 
    const Type & obj2 = ar2[i]; 
    if ( !::IsEqualSArrayItems( obj1, obj2 ) )
      // \ru встретили различающиеся элементы \en there are different elements 
      return IsLessThanSArrayItems( obj1, obj2 );
  }

  if ( count1 < count2 )
    return true;

  return false; // \ru массивы полностью идентичны \en arrays are fully identical 
}

//------------------------------------------------------------------------------
// Добавить точки массива в другой массив
// ---
template<typename T, template<typename V> class C>
void AddArrayToArray( const C<T> & fromList, // Источник добавления элементов.
                            C<T> & toList,   // Расширяемый массив.
                            bool   fromBegin,// Начинать брать элементы с начала или конца fromList
                            bool   toBegin ) // Добавлять элементы в начало или в конец toList.
{
  ptrdiff_t from1Count = fromList.size();
  ptrdiff_t i;
  if ( fromBegin ) { // добавить в прямом направлении
    for ( i = 1; i < from1Count; i++ ) {
      if ( toBegin ) // добавить в начало
        toList.insert( toList.begin(), fromList[i] );
      else // добавить в конец
        toList.push_back( fromList[i] );
    }
  }
  else { // добавить в обратном направлении
    for ( i = from1Count-2; i >= 0 && i < from1Count; i-- ) {
      if ( toBegin ) // добавить в начало
        toList.insert( toList.begin(), fromList[i] );
      else
        toList.push_back( fromList[i] );
    }
  }
}

#endif // __TEMPL_S_ARRAY_H
