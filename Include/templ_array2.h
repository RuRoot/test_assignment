////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Двумерный массив объектов.
         \en Two-dimensional array of objects. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_ARRAY2_H
#define __TEMPL_ARRAY2_H


#include <stdlib.h>
#include <io_define.h>
#include <tool_quick_sort.h>
#include <string.h>
#include <utility>
#include <tool_err_handling.h>


class  reader;
class  writer;

FORVARD_DECL_TEMPLATE_TYPENAME( class Array2 );
FORVARD_DECL_TEMPLATE_TYPENAME( reader & operator >> ( reader & in,        Array2<Type>  & ptr ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer & operator << ( writer & out, const Array2<Type>  & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader & operator >> ( reader & in,        Array2<Type> *& ptr ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer & operator << ( writer & out, const Array2<Type>  * ptr ) );
FORVARD_DECL_TEMPLATE_TYPENAME( bool     realloc_line ( Type *& line, size_t oldSize, size_t newSize ) );
FORVARD_DECL_TEMPLATE_TYPENAME( bool     assign_to_array( Array2<Type> &, const Array2<Type> & src ) );
FORVARD_DECL_TEMPLATE_TYPENAME( bool     set_array_size ( Array2<Type> &, size_t lSize, size_t cSize ) );
FORVARD_DECL_TEMPLATE_TYPENAME( bool     insert_column_to_array  ( Array2<Type> &, size_t ind ) );
FORVARD_DECL_TEMPLATE_TYPENAME( bool     add_column_to_array     ( Array2<Type> & ) );
FORVARD_DECL_TEMPLATE_TYPENAME( void     remove_column_from_array( Array2<Type> &, size_t ind ) );
FORVARD_DECL_TEMPLATE_TYPENAME( void     zero_array( Array2<Type> & ) );


//------------------------------------------------------------------------------
/** \brief \ru Двумерный массив объектов.
           \en Two-dimensional array of objects. \~
  \details \ru Двумерный массив объектов. \n
           \en Two-dimensional array of objects. \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class Array2 {
protected :
  Type ** parr; ///< \ru Указатель на первый элемент массива (элементами массива являются указатели на содержимое строк). \en Pointer to first element of array (elements of array are pointers to contents of rows). 
  size_t  l;    ///< \ru Количество строк массива. \en Count of rows in the array. 
  size_t  c;    ///< \ru Количество столбцов массива (то есть длина каждой строки). \en Count of columns of array (i.e. length of each row). 

protected:
  /// \ru Конструктор по заданной размерности. \en The constructor by a given dimension. 
  Array2( size_t lsz, size_t csz );
public:
  /// \ru Конструктор. \en Constructor. 
  Array2() : parr( 0 ), l( 0 ), c( 0 ) {}
  /// \ru Конструктор ограниченной размерности. \en The constructor of restricted dimension. 
  Array2( const uint16 & lsz, const uint16 & csz );
  /// \ru Конструктор копирования. \en Copy-constructor. 
  explicit Array2( const Array2<Type> & );
  /// \ru Деструктор. \en Destructor. 
  ~Array2() { set_array_size( *this, 0, 0 ); }

public:
  /// \ru Создать массив заданной размерности (возвращает nullptr в случае неудачи).
  /// \en Create an array of a given dimension (returns nullptr in case of failure).
  static Array2<Type> * Create( size_t lSize, size_t cSize );

public: // Общие методы матриц (двумерных массивов)
        size_t          Lines  () const { return l; } ///< \ru Количество строк. \en Count of rows. 
        size_t          Columns() const { return c; } ///< \ru Количество столбцов. \en Count of columns. 
        size_t          Count  () const { return (l*c); } ///< \ru Количество элементов. \en Count of elements.  
   c3d::IndicesPair     GetSize() const { return c3d::IndicesPair( l, c ); } ///< \ru Дать размер массива. \en Give the size of the array. 
        bool            SetSize( c3d::IndicesPair sz ) { return SetSize( sz.first, sz.second ); } ///< \ru Установить размер. \en Set size. 
        bool            SetSize( size_t lsz, size_t csz ); ///< \ru Установить размер. \en Set size. 
        bool            SetSize( size_t n ) { return SetSize( n, n ); } ///< \ru Установить размер. \en Set size. 

        /// \ru Получить элемент массива. \en Get an element of the array. 
  const Type &          GetElem( size_t ln, size_t cn ) const;
        /// \ru Установить элемент массива. \en Set an element of the array. 
        void            SetElem( size_t ln, size_t cn, const Type & v );
        /// \ru Оператор доступа по индексам. \en Access by indices operator. 
  const Type &          operator () ( size_t i, size_t j ) const;
        /// \ru Расписать массив нулями. \en Assign zeros to array. 
        Array2<Type> &  SetZero() { zero_array( *this ); return *this; }
        /// \ru Функция присваивания. \en An assignment function. 
        bool            Init( const Array2<Type> & src ) { return assign_to_array( *this, src ); }
        /// \ru Оператор присваивания. \en The assignment operator. 
        Array2<Type> &  operator = ( const Array2<Type> & src ) { Init( src ); return *this; }
        /// \ru Поменять местами строки. \en Swap lines. 
        bool            SwapLines( size_t ln1, size_t ln2 );

public:
        /// \ru Оператор доступа по индексам. \en Access by indices operator. 
        Type &          operator () ( size_t i, size_t j );
        /// \ru Выдать адрес начала строки. \en Get an address of the row start. 
  const Type *          GetLine( size_t i = 0 ) const;
        /// \ru Выдать адрес начала строки. \en Get an address of the row start. 
        Type *          SetLine( size_t i = 0 );
        /// \ru Инициировать элемент. \en Initiate an element. 
        void            Init( size_t ln, size_t cn, const Type & v );

        /// \ru Функции, выделяющие потенциально большие участки памяти, возвращают результат операции (успех/ошибка).
        /// \en Functions that allocate potentially large memory, return the operation result (success/error).
        bool            InsertColumn( size_t i = 0 ); ///< \ru Вставить столбец перед указанным. \en Insert column before the specified one. 
        bool            AddColumn();                  ///< \ru Добавить столбец в конец массива. \en Add column to the end of the array. 
        void            RemoveColumn( size_t i = 0 ); ///< \ru Удалить столбец из массива. \en Delete column from array. 

        bool            InsertLine  ( size_t i = 0 ); ///< \ru Вставить строку перед указанной. \en Insert row before the specified one. 
        bool            AddLine();                    ///< \ru Добавить строку в конец массива. \en Add row to the end of the array. 
        void            RemoveLine  ( size_t i = 0 ); ///< \ru Удалить строку из массива. \en Delete row from array. 
protected :
        bool            CatchLinePointers( size_t newCount ); ///< \ru Взять память под заданное кол-во указателей на строки. \en Allocate memory for the given count of pointers to rows. 

  /// \ru Оператор чтения. \en Read operator. 
  TEMPLATE_FRIEND reader & operator >> TEMPLATE_SUFFIX ( reader & in,        Array2<Type>  & ptr );
  /// \ru Оператор записи. \en Write operator. 
  TEMPLATE_FRIEND writer & operator << TEMPLATE_SUFFIX ( writer & out, const Array2<Type>  & ref );
  /// \ru Оператор чтения. \en Read operator. 
  TEMPLATE_FRIEND reader & operator >> TEMPLATE_SUFFIX ( reader & in,        Array2<Type> *& ptr );
  /// \ru Оператор записи. \en Write operator. 
  TEMPLATE_FRIEND writer & operator << TEMPLATE_SUFFIX ( writer & out, const Array2<Type>  * ptr );

  /// \ru Перезахватить память под одну строку. \en Reallocate memory for one row. 
  TEMPLATE_FRIEND bool     realloc_line             TEMPLATE_SUFFIX ( Type *& line, size_t oldSize, size_t newSize );
  /// \ru Скопировать массив. \en Copy an array. 
  TEMPLATE_FRIEND bool     assign_to_array          TEMPLATE_SUFFIX ( Array2<Type> &, const Array2<Type> & src );
  /// \ru Установить размер массива. \en Set the size of the array. 
  TEMPLATE_FRIEND bool     set_array_size           TEMPLATE_SUFFIX ( Array2<Type> &, size_t lSize, size_t cSize );
  /// \ru Вставить колонку перед указанной и заполнить ее нулями. \en Insert column before the specified one and fill it with zeros. 
  TEMPLATE_FRIEND bool     insert_column_to_array   TEMPLATE_SUFFIX ( Array2<Type> &, size_t ind );
  /// \ru Добавить в массив колонку и заполнить ее нулями. \en Add column to array and fill it with zeros. 
  TEMPLATE_FRIEND bool     add_column_to_array      TEMPLATE_SUFFIX ( Array2<Type> & );
  /// \ru Удалить колонку из массива. \en Delete column from array. 
  TEMPLATE_FRIEND void     remove_column_from_array TEMPLATE_SUFFIX ( Array2<Type> &, size_t ind );
  /// \ru Заполнить массив нулями. \en Fill the array with zeros. 
  TEMPLATE_FRIEND void     zero_array               TEMPLATE_SUFFIX ( Array2<Type> & );

#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
public:
        void *          operator new    ( size_t );
        void            operator delete ( void *, size_t );
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_

};


#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
//------------------------------------------------------------------------------
// \ru Перегрузка оператора new. \en Overloading of the "new" operator. 
// ---
template <class Type>
inline void * Array2<Type>::operator new( size_t size ) {
  return ::Allocate( size, typeid(Array2<Type>).name() );
}

//------------------------------------------------------------------------------
// \ru Перегрузка оператора delete. \en Overloading of the "delete" operator. 
// ---
template <class Type>
inline void Array2<Type>::operator delete( void * ptr, size_t size ) {
  ::Free( ptr, size, typeid(Array2<Type>).name() );
}
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_


//------------------------------------------------------------------------------
// \ru конструктор массива \en constructor of an array 
// ---
template <class Type>
inline Array2<Type>::Array2( size_t lSize, size_t cSize )
  : parr( 0 )
  , l   ( 0 )
  , c   ( 0 )
{
  if ( !set_array_size( *this, lSize, cSize ) && !ExceptionMode::IsEnabled() )
    throw std::bad_alloc(); // \ru Бросить исключение при любом режиме. \en Throw exception in case of any mode.

  PRECONDITION( !!parr || (lSize == 0 && cSize == 0) );
}


//------------------------------------------------------------------------------
// \ru конструктор массива \en constructor of an array 
// ---
template <class Type>
inline Array2<Type>::Array2( const uint16 & lSize, const uint16 & cSize )
  : parr( 0 )
  , l   ( 0 )
  , c   ( 0 )
{
  if ( !set_array_size( *this, lSize, cSize ) && !ExceptionMode::IsEnabled() )
    throw std::bad_alloc(); // \ru Бросить исключение при любом режиме. \en Throw exception in case of any mode.

  PRECONDITION( !!parr || (lSize == 0 && cSize == 0) );
}


//------------------------------------------------------------------------------
// \ru конструктор копирования \en copy-constructor 
// ---
template <class Type>
inline Array2<Type>::Array2( const Array2<Type> & source )
  : parr( 0 )
  , l   ( 0 )
  , c   ( 0 )
{
  if ( !::assign_to_array( *this, source ) && !ExceptionMode::IsEnabled() )
    throw std::bad_alloc(); // \ru Бросить исключение при любом режиме. \en Throw exception in case of any mode.

  PRECONDITION( !!parr || (l == 0 && c == 0) );
}


//------------------------------------------------------------------------------
// \ru Создать массив заданной размерности. \en Create an array of a given dimension. 
// ---
template <class Type>
inline Array2<Type> * Array2<Type>::Create( size_t lSize, size_t cSize )
{
  Array2<Type> * arr = nullptr;
  if ( lSize * cSize < c3d::MATRIX_MAX_COUNT ) {
    try {
      arr = new Array2<Type>( lSize, cSize );
    }
    catch ( const std::bad_alloc & ) {
      arr = nullptr;
    }
  }
  return arr;
}


//------------------------------------------------------------------------------
// \ru Установить размер массива \en Set the size of the array 
// ---
template <class Type>
inline bool Array2<Type>::SetSize( size_t lSize, size_t cSize ) {
  return set_array_size( *this, lSize, cSize );
}

//------------------------------------------------------------------------------
// \ru Получить элемент массива. \en Get an element of the array. 
// ---
template <class Type>
const Type & Array2<Type>::GetElem( size_t ln, size_t cn ) const {
  PRECONDITION( !!parr && ln < l && cn < c );
  return parr[ln][cn]; // SKIP_SA
}

//------------------------------------------------------------------------------
// \ru Установить элемент массива. \en Set an element of the array. 
// ---
template <class Type>
void Array2<Type>::SetElem( size_t ln, size_t cn, const Type & v ) {
  PRECONDITION( !!parr && ln < l && cn < c );
  if ( !!parr && ln < l && cn < c )
    parr[ln][cn] = v; // SKIP_SA
}

//------------------------------------------------------------------------------
// \ru Оператор доступа по индексам. \en Access by indices operator. 
// ---
template <class Type>
const Type & Array2<Type>::operator() ( size_t i, size_t j ) const {
  PRECONDITION( i < l && j < c );
  return parr[i][j]; // SKIP_SA
}

/// \ru Оператор доступа по индексам. \en Access by indices operator. 
template <class Type>
Type & Array2<Type>::operator() ( size_t i, size_t j ) {
    PRECONDITION( i < l && j < c );
    return parr[i][j]; // SKIP_SA
}

//------------------------------------------------------------------------------
// \ru Выдать адрес начала строки. \en Get an address of the row start. 
// ---
template <class Type>
const Type * Array2<Type>::GetLine( size_t i ) const {
  PRECONDITION( i < l );
  if ( i < l )
    return parr[i];
  return nullptr;
}

//------------------------------------------------------------------------------
// \ru Выдать адрес начала строки. \en Get an address of the row start. 
// ---
template <class Type>
Type * Array2<Type>::SetLine( size_t i ) {
  PRECONDITION( i < l );
  if ( i < l )
    return parr[i]; // SKIP_SA
  return nullptr;
}

//------------------------------------------------------------------------------
// \ru Инициировать элемент. \en Initiate an element. 
// ---
template <class Type>
void Array2<Type>::Init( size_t ln, size_t cn, const Type & v ) {
  PRECONDITION( !!parr && ln < l && cn < c );
  if ( !!parr && ln < l && cn < c )
    parr[ln][cn] = v; // SKIP_SA
}


//------------------------------------------------------------------------------
// \ru Поменять местами строки. \en Swap lines. 
// ---
template <class Type>
inline bool Array2<Type>::SwapLines( size_t ln1, size_t ln2 )
{
  if ( !!parr && ln1 < l && ln2 < l ) {
    Type * tmp = parr[ln1];
    parr[ln1] = parr[ln2];
    parr[ln2] = tmp;
    return true;
  }
  PRECONDITION( false );
  return false;
}


//------------------------------------------------------------------------------
// \ru вставить столбец перед указанным \en insert column before the specified one 
// ---
template <class Type>
inline bool Array2<Type>::InsertColumn( size_t ind ) {
  return insert_column_to_array( *this, ind );
}


//------------------------------------------------------------------------------
// \ru добавить столбец в конец массива \en add column to the end of the array 
// ---
template <class Type>
inline bool Array2<Type>::AddColumn() {
  return add_column_to_array( *this );
}


//------------------------------------------------------------------------------
// \ru удалить столбец \en delete column 
// ---
template <class Type>
inline void Array2<Type>::RemoveColumn( size_t ind ) {
  remove_column_from_array( *this, ind );
}


//------------------------------------------------------------------------------
// \ru вставить строку перед указанной и заполнить ее нулями \en insert row before the specified one and fill it with zeros 
// \ru если ind больше количества строк, то просто добавить строку в конец массива \en if 'ind' is greater than count of rows then simply add row to the end of the array 
// ---
template <class Type>
inline bool Array2<Type>::InsertLine( size_t ind )
{
  bool res = true;
  size_t oldL = l; // \ru прежнее кол-во строк \en previous count of rows 
  if ( AddLine() ) { // \ru кол-во строк увеличилось на 1 \en count of rows is increased by 1 

    if ( ind < oldL ) {
      // \ru переставить новую строку с последнего места на указанное \en swap the new row from last position with row of the specified position 
      Type * newLine = parr[l - 1];
      // \ru переместить хвост на один элемент дальше \en move the tale by one element 
      memmove( parr + ind + 1, parr + ind, (oldL - ind) * SIZE_OF_POINTER );

      parr[ind] = newLine; // \ru теперь это будет строка с индексом ind \en now it is the row with 'ind' index 
    }
  }
  else
    res = false;
  return res;
}


//------------------------------------------------------------------------------
// \ru добавить строку в конец массива и заполнить ее нулями \en add to the end of the array and fill it with zeros 
// ---
template <class Type>
inline bool Array2<Type>::AddLine()
{
  bool res = CatchLinePointers( l + 1 );
  if ( res ) { // \ru добавить память для одного указателя на строку \en add memory for one pointer to row 
                              // \ru (l увеличится на 1) \en (l will be increased by 1) 
    Type * newLine = 0; // \ru указатель на содержимое новой строки \en pointer to contents of new row 
    if ( c ) {
      res = realloc_line( newLine, 0/*oldSize*/, c/*newSize*/ ); // \ru захватить память под строку \en allocate memory for one row 
      if ( res )
        memset( newLine, 0, c * sizeof(Type) );
    }
    parr[l - 1] = newLine; // \ru записать указатель в массив \en store pointer to the array // SKIP_SA
  }
  return res;
}


//------------------------------------------------------------------------------
// \ru удалить строку из массива \en delete row from the array 
// ---
template <class Type>
inline void Array2<Type>::RemoveLine( size_t ind )
{
  PRECONDITION( ind < l );

  if ( ind < l ) {
    realloc_line( parr[ind], c/*oldSize*/, 0/*newSize*/ ); // \ru освободить память, занятую строкой \en free memory occupied by row 

    // \ru передвинуть хвост ближе к началу на один элемент \en move tale closer to start by one element 
    size_t tail = l - (ind + 1);
    if ( tail ) // \ru если это не последний указатель \en if it is not the last pointer, 
      memmove( parr + ind, parr + ind + 1, tail * SIZE_OF_POINTER );

    CatchLinePointers( l - 1/*newCount*/ ); // \ru освободить память из-под последнего указателя на строку \en then free memory for last pointer to row 
  }
}


//------------------------------------------------------------------------------
// \ru Взять память под заданное кол-во указателей на строки \en Allocate memory for the given count of pointers to rows 
// \ru Если кол-во строк уменьшается, то память из-под лишних строк уже должна быть освобождена ! \en If count of rows decreases, then memory from excess rows has to be already released ! 
// ---
template <class Type>
inline bool Array2<Type>::CatchLinePointers( size_t newCount )
{
  if ( l != newCount )
  {
    // \ru половина адресного пространства для 64- и 32-разрядного приложения \en a half of address space for 64- and 32-bit application  
    if ( ::TestNewSize( SIZE_OF_POINTER, newCount ) )
    { 
      try {
#ifdef __REALLOC_ARRAYS_STATISTIC_
        void * oldParr = parr;
        size_t oldSize = l;
#endif // __REALLOC_ARRAYS_STATISTIC_

#ifdef USE_REALLOC_IN_ARRAYS
        parr = (Type**)REALLOC_ARRAY_SIZE( parr, newCount * SIZE_OF_POINTER, false/*clear*/ );
        // \ru ИР parr = (Type**) ::realloc( parr, newCount * SIZE_OF_POINTER ); \en ИР parr = (Type**) ::realloc( parr, newCount * SIZE_OF_POINTER ); 
#else
        Type ** p_tmp = newCount ? new Type*[newCount] : 0;

        if ( parr ) {
          if ( p_tmp )
            memcpy( p_tmp, parr, std_min(l, newCount) * SIZE_OF_POINTER );

          delete[] parr;
        }

        parr = p_tmp;
#endif // USE_REALLOC_IN_ARRAYS

        l = newCount;

#ifdef __REALLOC_ARRAYS_STATISTIC_
        REALLOC_ARRAY_STATISTICS( oldParr, oldSize * SIZE_OF_POINTER, parr, newCount * SIZE_OF_POINTER, 2/*Array2*/ );
#endif // __REALLOC_ARRAYS_STATISTIC_
      }
      catch ( const std::bad_alloc & ) {
        C3D_CONTROLED_THROW;
        return false;
      }
      catch ( ... ) {
        if ( newCount == 0 ) {// \ru Не смогли удалить parr. \en Failed to delete parr.
          parr = nullptr;
          l = c = 0;
        }
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


//------------------------------------------------------------------------------
/// \ru Перезахватить память под одну строку. \en Reallocate memory for one row. 
// ---
template <class Type>
inline bool realloc_line( Type *& line, size_t oldSize, size_t newSize )
{
  if ( oldSize != newSize )
  {
    size_t sizeOfType = sizeof(Type);

    // \ru половина адресного пространства для 64- и 32-разрядного приложения \en a half of address space for 64- and 32-bit application  
    if ( ::TestNewSize( sizeOfType, newSize ) )
    { 
      try {
#ifdef __REALLOC_ARRAYS_STATISTIC_
        void * oldParr = line;
#endif // __REALLOC_ARRAYS_STATISTIC_

#ifdef USE_REALLOC_IN_ARRAYS
        line = (Type *) REALLOC_ARRAY_SIZE( line, newSize * sizeOfType, false/*clear*/ );
#else
        Type * p_tmp = newSize ? new Type[newSize] : 0;

        if ( line ) // SKIP_SA
        {
          if ( p_tmp )
            memcpy( p_tmp, line, std_min(oldSize, newSize) * sizeOfType );

          delete [] line;
        }

        line = p_tmp;
#endif // USE_REALLOC_IN_ARRAYS

#ifdef __REALLOC_ARRAYS_STATISTIC_
        REALLOC_ARRAY_STATISTICS( oldParr, oldSize * sizeOfType, line, newSize * sizeOfType, 2/*Array2*/ );
#endif // __REALLOC_ARRAYS_STATISTIC_
      }
      catch ( const std::bad_alloc & ) {
        C3D_CONTROLED_THROW;
        return false;
      }
      catch ( ... ) {
        if ( newSize == 0 )// \ru Не смогли удалить line. \en Failed to delete line.
          line = nullptr;
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


//------------------------------------------------------------------------------
// \ru Присвоить массиву новое содержимое, заменив его текущее содержимое. 
// \en Assign new contents to the array, replacing its current contents.
// ---
template <class Type>
bool assign_to_array( Array2<Type> & arr, const Array2<Type> & source )
{
  if ( set_array_size( arr, source.l, source.c ) ) {
    if ( arr.l && arr.c ) {
      Type ** aParr = arr.parr;
      Type ** sParr = source.parr;
      size_t n = arr.c * sizeof(Type);
      for ( size_t i = 0; i < arr.l; i++, aParr++, sParr++ )
        ::memcpy( *aParr, *sParr, n ); // SKIP_SA
    }
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
bool set_array_size( Array2<Type> & arr, size_t lSize, size_t cSize )
{
  bool res = false;

  if ( lSize*cSize <= c3d::MATRIX_MAX_COUNT ) {
    size_t i, oldL = arr.l;
    res = true;

    if ( arr.l != lSize ) {
      // \ru Если кол-во строк уменьшается - освободить память из-под лишних строк \en If count of rows decreases then release memory from excess rows 
      if ( arr.c && oldL && lSize < oldL ) {
        size_t removeLines = oldL - lSize;
        for ( i = 1; i <= removeLines; i++ )
          realloc_line( arr.parr[oldL-i], arr.c/*oldSize*/, 0/*newSize*/ ); // \ru освободить память, занятую строкой \en free memory occupied by row 
      }

      // \ru Взять память под заданное кол-во указателей на строки (изменится arr.l) \en Allocate memory for the given count of pointers to rows (arr.l tro be changed) 
      res = arr.CatchLinePointers( lSize );
      for ( i = oldL; res && i < arr.l/*\ru уже изменено \en already changed */; i++ )
        arr.parr[i] = 0; // \ru обнулить добавленные указатели \en set added pointers to null 
    }

    if ( arr.c != cSize ) {
      // \ru Перезахватить память под каждую строку \en Reallocate memory for each row 
      for ( i = 0; res && i < arr.l/*\ru уже изменено \en already changed */; i++ )
        res = realloc_line( arr.parr[i], arr.c/*oldSize*/, cSize/*newSize*/ );

      if ( res )
        arr.c = cSize;
    }
    else if ( arr.l > oldL && arr.c > 0 ) { // \ru BUG_46010 KYA K12 А кто будет выделять память для новых строк \en BUG_46010 KYA K12 And who will allocate memory for new rows 
      Type * newLine = 0; // \ru указатель на содержимое новой строки \en pointer to contents of new row 
      for ( i = oldL; res && i < arr.l; i++ ) {
        newLine = nullptr;
        res = ::realloc_line( newLine, 0, arr.c ); // \ru захватить память под строку \en allocate memory for one row 
        if ( res ) {
          ::memset( newLine, 0, arr.c * sizeof(Type) );
          arr.parr[i] = newLine; // \ru записать указатель в массив \en store pointer to the array 
        }
      }
    }
  }

  PRECONDITION( res );
  return res;
}


//------------------------------------------------------------------------------
/// \ru Вставить колонку перед указанной и заполнить ее нулями \en Insert column before specified one and fill it with zeros 
// \ru если ind больше количества колонок, то просто добавить колонку в конец массива \en if 'ind' is greater than count of columns, then simply add the column to the end of the array 
// ---
template <class Type>
bool insert_column_to_array( Array2<Type> & arr, size_t ind )
{
  bool res = true;
  if ( arr.l ) {
    size_t oldC = arr.c; // \ru прежнее кол-во колонок \en previous count of columns 
    if ( add_column_to_array( arr ) ) { // \ru добавить колонку в конец строк (увеличилось arr.c) \en add column to the end of rows (arr.c increased) 
      if ( ind < oldC ) {
        size_t tail = oldC - ind; // \ru кол-во колонок, которые нужно сдвинуть \en count of columns to be moved 
        for ( size_t i = 0; i < arr.l; i++ ) {
          Type * line = arr.parr[i];
          // \ru передвинуть хвосты всех строк ближе к концу на один элемент (при этом \en move tales of all rows by one element closer to end (thus 
          // \ru затрем добавленный обнуленный элемент) \en erase added nullified element) 
          memmove( line + ind + 1, line + ind, tail * sizeof(Type) );
          // \ru обнулить элемент ind \en set 'ind' element to null 
          memset( line + ind, 0, sizeof(Type) );
        }
      }
    }
    else
      res = false;
  }
  return res;
}


//------------------------------------------------------------------------------
/// \ru Добавить в массив колонку (последнюю) и обнулить ее. \en Add (last) column to array and fill it with zeros. 
// ---
template <class Type>
bool add_column_to_array( Array2<Type> & arr )
{
  bool res = true;
  if ( arr.l ) {
    // \ru увеличить память под каждой строкой на 1 элемент \en increase memory under each row by 1 element 
    size_t newSize  = arr.c + 1;
    for ( size_t i = 0; res && i < arr.l; i++ ) {
      res = realloc_line( arr.parr[i]/*\ru здесь может меняться!!! \en can be changed!!! */, arr.c/*oldSize*/, newSize );
      // \ru обнулить добавленную ячейку (последний элемент в строке) \en set added cell (last element in row) to zero 
      if ( res )
        memset( arr.parr[i] + arr.c/*\ru еще не увеличенное \en not yet increased */, 0, sizeof(Type) );
    }
    if ( res )
      arr.c = newSize;
  }
  return res;
}


//------------------------------------------------------------------------------
/// \ru Убрать из массива колонку по индексу. \en Delete column from array by the index. 
// ---
template <class Type>
void remove_column_from_array( Array2<Type> & arr, size_t ind )
{
  PRECONDITION( ind < arr.c );

  if ( ind < arr.c ) {
    // \ru передвинуть хвосты всех строк ближе к началу на один элемент \en move tales of all rows closer to start by one element 
    size_t tail = arr.c - (ind + 1);
    if ( tail ) { // \ru это не последняя колонка в строке \en this is not last column in row 
      for ( size_t i = 0; i < arr.l; i++ ) {
        Type *line = arr.parr[i];
        memmove( line + ind, line + ind + 1, tail * sizeof(Type) );
      }
    }

    // \ru уменьшить память под каждой строкой на 1 элемент \en decrease memory under each row by 1 element 
    size_t newSize = arr.c - 1;
    for ( size_t i = 0; i < arr.l; i++ )
      realloc_line( arr.parr[i], arr.c/*oldSize*/, newSize );

    arr.c = newSize;
  }

}


//------------------------------------------------------------------------------
/// \ru Заполнить массив нулями. \en Fill the array with zeros. 
// ---
template <class Type>
void zero_array( Array2<Type> & arr )
{
  size_t n = arr.c * sizeof(Type);
  if ( n ) {
    Type ** parr = arr.parr;
    for ( size_t i = 0; i < arr.l; i++, parr++ )
      memset( *parr, 0, n );
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Транспонирование матрицы.
           \en Transpose a matrix. \~
  \details \ru Транспонирование матрицы. \n
           \en Transpose a matrix. \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
void Transpose( const Array2<Type> & srcMtr, Array2<Type> & dstMtr )
{
  const size_t l = srcMtr.Lines();
  const size_t c = srcMtr.Columns();
  dstMtr.SetSize( c, l );
  for ( size_t nc = 0; nc < c; nc++ ) {
    for ( size_t nl = 0; nl < l; nl++ )
      dstMtr( nc, nl ) = srcMtr( nl, nc );
  }
}


#endif  // __TEMPL_ARRAY2_H
