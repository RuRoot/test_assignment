////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Одномерный массив указателей.
         \en One-dimensional array of pointers. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_RP_ARRAY_H
#define __TEMPL_RP_ARRAY_H

#include <system_cpp_standard.h>
#include <io_define.h>
#include <tool_quick_sort.h>
#include <templ_delete_define.h>
#include <string.h>
#include <tool_err_handling.h>



FORVARD_DECL_TEMPLATE_TYPENAME( class   RPArray );
FORVARD_DECL_TEMPLATE_TYPENAME( bool    set_Rarray_size( RPArray<Type> &, size_t newSize ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t  find_in_array  ( const RPArray<Type> & arr, const Type * el ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader & CALL_DECLARATION operator >> ( reader & in, RPArray<Type> & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer & CALL_DECLARATION operator << ( writer & out, const RPArray<Type> & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader & CALL_DECLARATION operator >> ( reader & in, RPArray<Type> *& ptr ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer & CALL_DECLARATION operator << ( writer & out, const RPArray<Type> * ptr ) );


//------------------------------------------------------------------------------
/** \brief \ru Одномерный массив указателей на объекты.
           \en One-dimensional array of pointers to objects. \~
  \details \ru Шаблонный массив, работающий с указателями на объекты. \n
           \en A template array working with pointers to objects. \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class RPArray {
public:
  /// \ru Имя указателя на объект. \en A name of the pointer to the object. 
  typedef Type * TPtr; 
  typedef Type * value_type;
  typedef Type * const & const_reference;
  typedef Type *       & reference;  

protected:
  size_t  count; ///< \ru Количество элементов в массиве. \en The number of elements in array. 
  size_t  upper; ///< \ru Под какое количество элементов выделена память. \en The number of elements the memory is allocated for. 
  uint16  delta; ///< \ru Приращение по количеству элементов при выделении дополнительной памяти. \en Increment by the number of elements while the allocation of additional memory. 
private:
  TPtr *  parr;  ///< \ru Указатель на первый элемент массива. \en A pointer to the first array element. 

public :
  /// \ru Конструктор. \en Constructor. 
  RPArray();
  /// \ru Конструктор. \en Constructor. 
  RPArray ( size_t i_upper, uint16 i_delta = 1 );//, bool shouldNullSet = false );
  /// \ru Деструктор. \en Destructor. 
  virtual ~RPArray();

public:
  
  /// \ru Получить приращение по количеству элементов при выделении дополнительной памяти. \en Get the increment by the number of elements while the allocation of additional memory. 
          uint16  Delta() const { return delta; }
  /// \ru Количество элементов, для которых выделена память? \en The number of elements the memory is allocated for. 
          size_t  Upper() const { return upper; }
  /// \ru Установить приращение по количеству элементов при выделении дополнительной памяти (1 - автоприращение). \en Set the increment by the number of elements while the allocation of additional memory (1 - autoincrement). 
          void    Delta( uint16 newDelta )       { delta = newDelta; }
  /// \ru Установить максимальное из приращений. \en Set the maximum increment. 
          void    SetMaxDelta( uint16 newDelta ) { if ( delta < newDelta ) delta = newDelta; }

  /// \ru Функции, выделяющие потенциально большие участки памяти, возвращают результат операции (успех/ошибка).
  /// \en Functions that allocate potentially large memory, return an operation result (success/error). 
          bool    SetSize ( size_t newSize );                           ///< \ru Установить новый размер массива. \en Set the new size of an array. 
          bool    Reserve ( size_t n, bool addAdditionalSpace = true ); ///< \ru Зарезервировать место под столько элементов. \en Reserve space for a given number of elements. 

          bool    Add     ( Type * );                 ///< \ru Добавить элемент в конец массива. \en Add an element to the end of the array. 
          bool    AddAt   ( Type * e, size_t index ) { return Insert( index, e ); } ///< \ru Вставить элемент в указанную позицию. \en Insert an element to the given position. 
          bool    AddAfter( Type * e, size_t index ); ///< \ru Добавить элемент после указанного. \en Add an element after the specified one. 
          bool    Insert  ( size_t index, Type * );   ///< \ru Вставить элемент перед указанным. \en Insert an element before the specified one. 

          bool    AddArray   ( const RPArray<Type> & );               ///< \ru Добавить массив. \en Add array. 
          bool    AddCArray  ( const Type **, size_t count );         ///< \ru Добавить C-массив. \en Add C-array. 
          bool    InsertArray( const RPArray<Type> &, size_t index ); ///< \ru Добавить массив в позицию. \en Add an array to the position. 

          void    DetachAll();                          ///< \ru Удалить все элементы обнулить количество элементов. \en Delete all elements and set the number of elements to null. 
          void    Adjust();                             ///< \ru Удалить лишнюю память. \en Free the unnecessary memory. 
          Type *  DetachInd( size_t delIndex );         ///< \ru Отсоединить элемент от массива. \en Detach an element from the array. 
          bool    DetachObj( const Type * delObject );  ///< \ru Отсоединить элемент от массива. \en Detach an element from the array. 

  virtual Type *  RemoveInd( size_t delIndex, DelType /*del*/ = defDelete ) { return DetachInd( delIndex); } ///< \ru Удалить элемент из массива по индексу. \en Delete an element from array by the index. 

          void    Swap( RPArray & arr );          ///< \ru Обменять местами данные массивов. \en Swap data of arrays. 

          size_t  FindIt ( const Type * ) const;  ///< \ru Найти элемент по указателю. \en Find an element by a pointer. 
          bool    IsExist( const Type * ) const;  ///< \ru Есть ли элемент в массиве. \en Whether an element belongs the array. 
          size_t  Count() const { return count; } ///< \ru Получить количество элементов массива. \en Get the number of array elements. 
          ptrdiff_t MaxIndex() const { return ( static_cast<ptrdiff_t>(count) - 1); } ///< \ru Получить индекс последнего объект в массиве. \en Get the index of the last element in the array. 

  typedef int     (*CompFunc)( const Type **, const Type ** ); ///< \ru Шаблон функции сортировки. \en A template of sorting function. 
          void    Sort ( CompFunc comp ); ///< \ru Сортировать массив. \en Sort the array. 

  /// \ru Оператор доступа по индексу. \en Access by index operator. 
          Type *& operator []( size_t loc ) const;
  /// \ru Получить адрес последнего элемента в массиве. \en Get the address of the last element in the array. 
          Type *  GetLast() const { return ((count > 0) ? parr[count-1] : static_cast<Type*>(nullptr)); }

public: // \ru унификация с вектором STL \en unification with STL vector 
          bool    empty() const { return count == 0; }
          size_t  size() const { return count; }             ///< \ru Дать количество элементов массива. \en Get the number of elements in array. 
          bool    reserve( size_t n ) { return Reserve( n, false ); } ///< \ru Зарезервировать место под столько элементов. \en Reserve space for a given number of elements. 
          size_t  capacity() const { return Upper(); }         ///< \ru Под какое количество элементов выделена память? \en What is the number of elements the memory is allocated for? 
          void    push_back( const Type * e ) { Add( const_cast<Type *>(e) ); } ///< \ru Добавить элемент в конец массива. \en Add an element to the end of the array. 
          void    emplace_back( const Type * e ) { Add( const_cast<Type *>(e) ); } ///< \ru Добавить элемент в конец массива. \en Add an element to the end of the array. 
          template <class Iterator>
          void    insert( Iterator pos, const Type * e ); ///< \ru Вставить элемент перед указанным. \en Insert an element before the specified one. 
  // \ru прописывать поведение у всех классов-наследников! \en define the behavior for all classes-inheritors! 
          template <class Iterator>
          void    erase( Iterator pos );                  ///< \ru Удалить элемент из массива по индексу. \en Delete an element from array by the index. 
          template <class Iterator>
          void    erase( Iterator first, Iterator last ); ///< \ru Удалить элементы из массива начиная с индекса first до last-1 включительно. \en Delete elements from the array from first to last-1 inclusively. 
  virtual void    clear() { DetachAll(); }      ///< \ru Обнулить количество элементов. \en Set the number of elements to null. 
          void    shrink_to_fit() { Adjust(); } ///< \ru Удалить лишнюю память. \en Free the unnecessary memory (Reduce capacity).  
  
  const   TPtr *        begin() const { return parr; } ///< \ru Получить указатель на первый элемент массива. \en Get the pointer to the first array element. 
          TPtr *        begin()       { return parr; } ///< \ru Получить указатель на первый элемент массива. \en Get the pointer to the first array element. 
  const   TPtr *        end()   const { return parr + count; }  ///< \ru Получить указатель на участок памяти после массива. \en Get the pointer to the piece of memory after the array. 
          TPtr *        end()         { return parr + count; }  ///< \ru Получить указатель на участок памяти после массива. \en Get the pointer to the piece of memory after the array.   
  const   TPtr *        cbegin() const { return parr; }       ///< \ru Получить указатель на первый элемент массива. \en Get the pointer to the first array element. 
  const   TPtr *        cend()   const { return parr + count; } ///< \ru Получить указатель на участок памяти после массива. \en Get the pointer to the piece of memory after the array. 
  const   value_type    front()  const;
          value_type &  front();
  const   value_type    back()   const;
          value_type &  back();
        const_reference at( size_t idx ) const;
          reference     at( size_t idx );
  
protected:
  const   Type ** GetAddr() const { return const_cast<const Type **>(parr); } ///< \ru Получить указатель на первый элемент массива. \en Get the pointer to the first array element. 
  const   TPtr * _Begin()   const { return parr; } ///< \ru Получить указатель на первый элемент массива. \en Get the pointer to the first array element. 
          TPtr * _Begin()         { return parr; } ///< \ru Получить указатель на первый элемент массива. \en Get the pointer to the first array element. 
  const   TPtr * _End()     const { return parr + count; } ///< \ru Получить указатель на участок памяти после массива. \en Get the pointer to the piece of memory after the array. 
         
          bool    CatchMemory();         ///< \ru Захватить память. \en Catch memory. 
          bool    AddMemory( size_t n ); ///< \ru Обеспечить место в памяти под n элементов, независимо от AutoDelta \en Provide memory for n elements, independently from AutoDelta 
          size_t  AutoDelta() const { return ::KsAutoDelta( count, delta ); } ///< \ru Вычислить автоприращение. \en Calculate autoincrement. 
  
private:
  RPArray( const RPArray<Type> & );                     // \ru запрещено !!! \en forbidden !!! 
  RPArray<Type> & operator =( const RPArray<Type> & );  // \ru запрещено !!! \en forbidden !!! 

  TEMPLATE_FRIEND bool    set_Rarray_size TEMPLATE_SUFFIX ( RPArray<Type> &, size_t newSize );
  TEMPLATE_FRIEND size_t  find_in_array   TEMPLATE_SUFFIX ( const RPArray<Type> & arr, const Type * el );
#if defined(_MSC_VER) && !defined(_MSVC_PERMISSIVE_OFF)
  TEMPLATE_FRIEND size_t  find_in_array   TEMPLATE_SUFFIX ( const RPArray<const Type> & arr, const Type * el );
#endif // _MSC_VER 

  TEMPLATE_FRIEND reader & CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader & in, RPArray<Type> & ref );
  TEMPLATE_FRIEND writer & CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer & out, const RPArray<Type> & ref );
  TEMPLATE_FRIEND reader & CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader & in, RPArray<Type> *& ptr );         
  TEMPLATE_FRIEND writer & CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer & out, const RPArray<Type> * ptr );

#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
public:
  void * operator new    ( size_t );
  void   operator delete ( void *, size_t );
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_

public:
  RPArray( RPArray<Type> && );                     ///< \ru Конструктор перемещения массива. \en Constructor of an array moving.
  RPArray<Type> & operator = ( RPArray<Type> && ); ///< \ru Оператор перемещения массива. \en Operator of an array moving.

};


#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
//------------------------------------------------------------------------------
// \ru Перегрузка оператора new. \en Overloading of the "new" operator. 
// ---
template <class Type>
inline void * RPArray<Type>::operator new( size_t size ) {
  return ::Allocate( size, typeid(RPArray<Type>).name() );
}

//------------------------------------------------------------------------------
// \ru Перегрузка оператора delete. \en Overloading of the "delete" operator. 
// ---
template <class Type>
inline void RPArray<Type>::operator delete( void * ptr, size_t size ) {
  ::Free( ptr, size, typeid(RPArray<Type>).name() );
}
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_


//------------------------------------------------------------------------------
// \ru конструктор массива \en constructor of an array 
// ---
template <class Type>
inline RPArray<Type>::RPArray()
  : count( 0 )
  , upper( 0 )
  , delta( 1 )
  , parr ( 0 )
{}


//------------------------------------------------------------------------------
// \ru конструктор массива \en constructor of an array 
// ---
template <class Type>
inline RPArray<Type>::RPArray( size_t i_upper, uint16 i_delta )//, bool shouldNullSet )
  : count( 0 )
  , upper( 0 )
  , delta( i_delta )
  , parr ( 0 ) //i_upper ? new TPtr[i_upper] : 0 )
{
  if ( !set_Rarray_size(*this, i_upper) && !ExceptionMode::IsEnabled() )
    throw std::bad_alloc(); // \ru Бросить исключение при любом режиме. \en Throw exception in case of any mode.

  //if ( shouldNullSet && upper > 0 ) {
  //  count = upper;
  //  memset( parr, 0, count * SIZE_OF_POINTER );
  //}
}


//------------------------------------------------------------------------------
// \ru Конструктор перемещения массива. \en Constructor of an array moving.
// ---
template <class Type>
inline RPArray<Type>::RPArray( RPArray<Type> && _Right )
  : count(_Right.count)
  , upper(_Right.upper)
  , delta(_Right.delta)
  , parr (_Right.parr)
{
  _Right.count = 0;
  _Right.upper = 0;
  _Right.delta = 1;
  _Right.parr  = nullptr;
}

//------------------------------------------------------------------------------
// \ru Оператор перемещения массива. \en Operator of an array moving.
// ---
template <class Type>
RPArray<Type> & RPArray<Type>::operator = ( RPArray<Type> && _Right )
{
  if ( this != &_Right )
  {
    set_Rarray_size( *this, 0 );
    _Right.Swap( *this );
  }
  return (*this);
}


//------------------------------------------------------------------------------
// \ru деструктор массива \en destructor of array 
// ---
template <class Type>
inline RPArray<Type>::~RPArray() {
  set_Rarray_size( *this, 0 ); //delete [] parr;
}

//------------------------------------------------------------------------------
// \ru Оператор доступа по индексу. \en Access by index operator. 
// ---
template <class Type>
inline Type *& RPArray<Type>::operator []( size_t loc ) const
{
  return parr[loc];
}

//------------------------------------------------------------------------------
// \ru Доступ к первому элементу. \en Access to the first element. 
// ---
template <class Type>
inline const typename RPArray<Type>::value_type RPArray<Type>::front() const {
  PRECONDITION( !empty() );
  return *parr;
}

//-------------------------------------------------------------------------------
// \ru Доступ к первому элементу. \en Access to the first element. 
// ---
template <class Type>
inline typename RPArray<Type>::value_type & RPArray<Type>::front() {
  PRECONDITION( !empty() );
  return *parr;
}

//-------------------------------------------------------------------------------
// \ru Доступ к последнему элементу. \en Access to the last element. 
// ---
template <class Type>
inline const typename RPArray<Type>::value_type RPArray<Type>::back() const
{
  PRECONDITION( !empty() );
  return *( parr + count - 1 );
}

//-------------------------------------------------------------------------------
// \ru Доступ к последнему элементу. \en Access to the last element. 
// ---
template <class Type>
inline typename RPArray<Type>::value_type & RPArray<Type>::back()
{
  PRECONDITION( !empty() );
  return *( parr + count - 1 );
}

//-------------------------------------------------------------------------------
// \ru Доступ по индексу. \en Access by index operator. 
// ---
template <class Type>
inline typename RPArray<Type>::const_reference RPArray<Type>::at( size_t idx ) const
{
  PRECONDITION( idx < count );
  return parr[idx];
}

//-------------------------------------------------------------------------------
// \ru Доступ по индексу. \en Access by index operator. 
// ---
template <class Type>
inline typename RPArray<Type>::reference RPArray<Type>::at( size_t idx )
{
  PRECONDITION( idx < count );
  return parr[idx];
}

//-------------------------------------------------------------------------------
// \ru обнулить количество элементов \en set the number of elements to null 
// ---
template <class Type>
inline void RPArray<Type>::DetachAll() {
  count = 0;
}


//------------------------------------------------------------------------------
// \ru Указать новый размер массива. \en Set the new size of an array.
// ---
template <class Type>
inline bool RPArray<Type>::SetSize( size_t newSize ) {
  return set_Rarray_size( *this, newSize );
}


//------------------------------------------------------------------------------
// \ru Зарезервировать место под n элементов. \en Reserve memory for n elements. 
// ---
template <class Type>
inline bool RPArray<Type>::Reserve( size_t n, bool addAdditionalSpace ) {
  if ( addAdditionalSpace )
    n += count;
  // \ru Захватить память, если требуется памяти больше, чем есть сейчас. \en if there is required more memory that exists at the moment catch it.
  if ( upper < n ) {
    return set_Rarray_size( *this, n );
  }
  else {
    // C3D_ASSERT( upper <= n ); // Use SetSize!!!
  }
  return true;
}


//------------------------------------------------------------------------------
// \ru Обеспечить место под n элементов, независимо от AutoDelta \en Provide memory for n elements, independently from AutoDelta 
// ---
template <class Type>
inline bool RPArray<Type>::AddMemory( size_t n ) {
  if ( upper - count < n ) {
    return set_Rarray_size( *this, count + n );
  }
  return true;
}


//------------------------------------------------------------------------------
// \ru освободить лишнюю память \en free unnecessary memory 
// ---
template <class Type>
inline void RPArray<Type>::Adjust() {
  if ( count < upper )
    set_Rarray_size( *this, count );
}


//------------------------------------------------------------------------------
// \ru добавить 1 элемент в конец массива \en add 1 element to the end of array 
// ---
template <class Type>
inline bool RPArray<Type>::Add( Type * ent ) {
  if ( CatchMemory() ) {
    parr[count++] = ent;
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru добавить 1 элемент после заданного \en add 1 element after the given one 
// ---
template <class Type>
inline bool RPArray<Type>::AddAfter( Type * ent, size_t  index ) {
  if ( !count )
    return Add( ent );
  else {
    if ( CatchMemory() ) {        // \ru добавить памяти, если все использовано \en add memory if whole allocated memory is used 
      if ( index > count - 1 )
        index = count - 1;
      // \ru передвинем вправо все элементы массива с index+1 до последнего \en move to the right all elements of the array from index+1 to the last 
      memmove( parr + index + 2, parr + index + 1, (count - index - 1) * SIZE_OF_POINTER );
      parr[index + 1] = ent; // \ru записываем новый элемент \en writing new element 
      count++;
      return true;
    }
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru добавить массив \en add array 
// \ru эти функции раньше обнуляли признак владения у второго массива, \en These functions set to null the attribute of second element ownership earlier, 
// \ru Теперь об этом нужно заботиться самостоятельно ! \en Now you should take care of it ! 
// ---
template <class Type>
inline bool RPArray<Type>::AddArray( const RPArray<Type> & from ) {
  return AddCArray( from.GetAddr(), from.count );
}


//------------------------------------------------------------------------------
// \ru добавить C-массив \en add C-array 
// ---
template <class Type>
inline bool RPArray<Type>::AddCArray( const Type ** from, size_t fromCount ) {
  if ( fromCount ) {
    if ( AddMemory( fromCount ) ) {  // \ru обеспечить память на такое кол-во элементов \en allocate memory for the given number of elements 
      memcpy( parr + count, from, fromCount * SIZE_OF_POINTER );
      count += fromCount;
      return true;
    }
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru добавить массив в позицию index \en Add an array to the position index 
// \ru (если index>count, то в конец) \en (if 'index' > 'count' then add to the end) 
// \ru эти функции раньше обнуляли признак владения у второго массива, \en These functions set to null the attribute of second element ownership earlier, 
// \ru теперь об этом нужно заботиться самостоятельно ! \en Now you should take care of it ! 
// ---
template <class Type>
inline bool RPArray<Type>::InsertArray( const RPArray<Type> & from, size_t index ) {
  if ( from.count ) {
    if ( index > count )
      index = count;
    if ( AddMemory( from.count ) ) {
      if ( count > index ) // \ru переместить хвост подальше \en move a tale farther 
        memmove( parr + index + from.count, parr + index, (count - index) * SIZE_OF_POINTER );
      memcpy( parr + index, from.parr, from.count * SIZE_OF_POINTER );
      count += from.count;
      return true;
    }
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru вставить 1 элемент перед указанным \en insert 1 element before the specified one 
// ---
template <class Type>
inline bool RPArray<Type>::Insert( size_t index, Type * ent ) {
  if ( CatchMemory() ) {         // \ru добавить памяти, если все использовано \en add memory if whole allocated memory is used 
    if ( index >= count )
      index = count;
    else {
      // \ru передвинем вправо все элементы массива с последнего до указанного \en move to the right all elements of the array from the last to the specified one 
      memmove( parr + index + 1, parr + index, (count - index) * SIZE_OF_POINTER );
    }
    parr[index] = ent;     // \ru записываем новый элемент \en writing new element 
    count++;
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru вернуть индекс элемента в массиве \en return an index of the element in the array 
// ---
template <class Type>
inline size_t RPArray<Type>::FindIt( const Type * el ) const 
{
  // \ru MA Linux return find_in_array( *this, el ); // ошибка при сопоставлении шаблонов для RPArray<const Type> \en MA Linux return find_in_array( *this, el ); // an error in matching of templates for RPArray<const Type> 
  if ( parr ) {
    TPtr * iterLast = parr+count;
    TPtr * iter = std::find( parr, iterLast, el );
    if ( iter != iterLast )
    {
      return std::distance( parr, iter );
    }
  }
  return SYS_MAX_T;
}


//------------------------------------------------------------------------------
// \ru Есть ли в массиве такой указатель \en Whether such pointer exists in the array 
// ---
template <class Type>
inline bool RPArray<Type>::IsExist( const Type * el ) const {
  return (FindIt( el ) != SYS_MAX_T);
}


//------------------------------------------------------------------------------
// \ru отсоединить элемент от массива (по индексу) \en detach an element from the array (by the index) 
// ---
template <class Type>
inline Type * RPArray<Type>::DetachInd( size_t delIndex )
{
  Type * r = 0;

  if ( parr ) {
    PRECONDITION( delIndex < count );
    if ( delIndex < count ) {
      r = parr[delIndex];
      memmove( parr + delIndex, parr + delIndex + 1, ( count-- - delIndex - 1 ) * SIZE_OF_POINTER );
    }
  }

  return r;
}


//------------------------------------------------------------------------------
// \ru Вставить элемент перед указанным. \en Insert an element before the specified one. 
// ---
template <class Type>
template <class Iterator>
void RPArray<Type>::insert( Iterator pos, const Type * e )
{
  if ( !begin() ) {
    reserve( 1 );
    pos = begin();
  }
  if ( begin() ) {
    const ptrdiff_t k = std::distance( static_cast<Iterator>(begin()), pos );
    if ( k >= 0 && k <= static_cast<ptrdiff_t>(count) )
      Insert( k, const_cast<Type *>(e) );
  }
}


//------------------------------------------------------------------------------
// \ru Удалить элемент из массива по индексу. \en Delete an element from array by the index. 
// ---
template <class Type>
template <class Iterator>
void RPArray<Type>::erase( Iterator pos )
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
void RPArray<Type>::erase( Iterator first, Iterator last )
{
  if ( begin() ) {
    const ptrdiff_t k1 = std::distance( static_cast<Iterator>(begin()), first );
    const ptrdiff_t k2 = std::distance( static_cast<Iterator>(begin()), last );

    if ( k1 >= 0 && k1 < k2 && k2 <= static_cast<ptrdiff_t>(count) ) {
      for ( ptrdiff_t k = k2-1; k >= k1; k-- ) {
        RemoveInd( k );
      }
    }
  }
}


//------------------------------------------------------------------------------
// \ru отсоединить элемент от массива (по указателю) \en detach an element from the array (by the pointer) 
// ---
template <class Type>
inline bool RPArray<Type>::DetachObj( const Type * delObject ) 
{
  size_t i = FindIt( delObject );
  if ( i != SYS_MAX_T ) 
  {
    DetachInd( i );
    return true;
  }
  return false;
}


//-------------------------------------------------------------------------------
// \ru обменять местами данные массивов \en swap data of arrays 
// ---
template <class Type>
void RPArray<Type>::Swap( RPArray & arr ) {
  std::swap( count, arr.count );
  std::swap( upper, arr.upper );
  std::swap( delta, arr.delta );
  std::swap( parr,  arr.parr  );
}


//------------------------------------------------------------------------------
// \ru захват большего куска памяти ( если нужно ) \en allocate the large piece of memory (if it is necessary) 
// ---
template <class Type>
inline bool RPArray<Type>::CatchMemory() {
  if ( upper == count )
    return set_Rarray_size( *this, upper + AutoDelta() );
  if ( parr != nullptr )
    return true;
  return false;
}


//------------------------------------------------------------------------------
// \ru сортировать массив \en sort the array 
// --- !!!!!!!!!!
template <class Type>
inline void RPArray<Type>::Sort( CompFunc fcmp ) {
  ::KsQSort( static_cast<void *>(parr), count, SIZE_OF_POINTER, reinterpret_cast<KsQSortCompFunc>(fcmp) );
}


//------------------------------------------------------------------------------
// \ru Установить присланный размер массива. Если он меньше имеющегося count, то \en Set the given size of an array. If it is less than existed 'count' then 
// \ru лишние элементы массива будут удалены \en extra elements of the array will be deleted 
// ---
template <class Type>
bool set_Rarray_size( RPArray<Type> & arr, size_t newSize )
{
  if ( newSize != arr.upper ) {
    // \ru половина адресного пространства для 64- и 32-разрядного приложения \en a half of address space for 64- and 32-bit application  
    if ( ::TestNewSize( SIZE_OF_POINTER, newSize ) ) {
      try {
#ifdef __REALLOC_ARRAYS_STATISTIC_
        void * oldParr = arr.parr;
        size_t oldSize = arr.upper;
#endif // __REALLOC_ARRAYS_STATISTIC_

#ifdef USE_REALLOC_IN_ARRAYS
        arr.parr = (typename RPArray<Type>::TPtr *)REALLOC_ARRAY_SIZE( arr.parr, newSize * SIZE_OF_POINTER, false/*clear*/ );
#else
        typename RPArray<Type>::TPtr *p_tmp = newSize ? new typename RPArray<Type>::TPtr[newSize] : 0;

        if ( arr.parr && p_tmp )
          memcpy( p_tmp, arr.parr, (arr.upper < newSize ? arr.upper : newSize) * SIZE_OF_POINTER );

        if ( arr.parr )
          delete[] arr.parr;

        arr.parr = p_tmp;
#endif // USE_REALLOC_IN_ARRAYS

        arr.upper = newSize;
        arr.count = newSize < arr.count ? newSize : arr.count;

#ifdef __REALLOC_ARRAYS_STATISTIC_
        REALLOC_ARRAY_STATISTICS( oldParr, oldSize * SIZE_OF_POINTER, arr.parr, newSize * SIZE_OF_POINTER, 1/*RParray*/ );
#endif // __REALLOC_ARRAYS_STATISTIC_
      }
      catch ( const std::bad_alloc & ) {
        C3D_CONTROLED_THROW;
        return false;
      }
      catch ( ... ) {
        if ( newSize == 0 )// \ru Не смогли корректно удалить arr.parr. \en Failed to delete arr.parr correctly.
          arr.parr = nullptr;
        C3D_CONTROLED_THROW;
        return false;
      }
    }
    else {
      PRECONDITION( false ); // \ru не бывает столько памяти \en incorrect size of memory 
      C3D_CONTROLED_THROW_EX( std::bad_alloc() );
      return false;
    }
  }
  return true;
}


//-------------------------------------------------------------------------------
// \ru найти объект в массиве \en find an object in the array 
// ---
template <class Type>
size_t find_in_array( const RPArray<Type> & arr, const Type * el ) {
  typename RPArray<Type>::TPtr * parr = arr.parr;
  for ( size_t i = 0, c = arr.count; i < c; i++, parr++ )
    if ( *parr == el )
      return i;

  return SYS_MAX_T;
}


#if defined(_MSC_VER) && !defined(_MSVC_PERMISSIVE_OFF) // LF-Linux: ambiguous template specialization
//-------------------------------------------------------------------------------
// \ru найти объект в массиве \en find an object in the array 
// ---
template <class Type>
size_t find_in_array( const RPArray<const Type> & arr, const Type * el ) {
  typename RPArray<const Type>::TPtr * parr = arr.parr;
  for ( size_t i = 0, c = arr.count; i < c; i++, parr++ )
    if ( *parr == el )
      return i;
  return SYS_MAX_T;
}
#endif // _MSC_VER


//------------------------------------------------------------------------------
// \ru Вычислить приращение для массива в зависимости от количества предполагаемых \en Calculate an increment for the array according to the number of assumed 
// \ru объектов в этом массив (делаю так, чтобы захватов памяти было не более 100) - \en objects in this array (so the number of memory captures will not exceed 100) - 
// \ru иначе при количестве объектов 75000 операции с захватом памяти занимают \en otherwise if the number of objects is 75000 then operations with memory captures take 
// \ru бесконечное время \en infinite time 
// ---
inline uint16 CalcArrayDelta( size_t objsCount ) {
  size_t delta = objsCount / 100;
  delta = delta > 65000 ? 50000 : delta > 0 ? delta : 1;
  return static_cast<uint16>(delta);
}


#endif  // __TEMPL_RP_ARRAY_H
