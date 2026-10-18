////////////////////////////////////////////////////////////////////////////////
/** 
  \file     
  \brief \ru parray c возможность сортировки по любому признаку 
         \en Array of pointers (of parray type) with ability of sorting by any criteria. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_PSRT_ARRAY_H
#define __TEMPL_PSRT_ARRAY_H


#include <templ_p_array.h>


#ifdef __FreeBSD__
#include <stdlib.h>
#endif


//------------------------------------------------------------------------------
// \ru константы для поиска \en constants for search 
// ---
enum PArraySortFind {
  // \ru граница выравнивания трудоемкости простого перебора с делением пополам \en balancing boundary of complexity of simple search with bisection 
  // \ru (а) для int ключа \en (a) for integer (int) key  
  // \ru ((4(*) + 4(/)) + 2(== >1) + 2(смещ2)) / (1(== >1) + 1(смещ2)) = 5 трудоемкость одной итерации поиска, \en ((4(*) + 4(/)) + 2(== >1) + 2(смещ2)) / (1(== >1) + 1(смещ2)) = 5 complexity of one iteration of search, 
  // \ru выравнивание при ~ 25 (5 делений(32) - 25 сравнений) \en balancing for ~ 25 (5 bisections (32) - 25 comparisons) 
  // \ru (b) для double ключа \en (b) for double key  
  // \ru ((4(*) + 4(/)) + 2(== >2) + 2(смещ2)) / (1(== >2) + 1(смещ2)) = 4 трудоемкость одной итерации поиска, \en ((4(*) + 4(/)) + 2(== >2) + 2(смещ2)) / (1(== >2) + 1(смещ2)) = 4 complexity of one iteration of search, 
  // \ru выравнивание при ~ 20 (5 делений(32) - 20 сравнений) \en balancing for ~ 20 (5 bisections (32) - 20 comparisons)   
  paf_Flat   = 20,   // \ru для double если меньше 20 элементов простой перебор выгодней \en simple search is more advantageous for double values if the number of elements is less than 20 
  paf_Bytwo  = 4096,
  paf_Byline = 2048,
};


//------------------------------------------------------------------------------
//
// ---
enum PArraySortDivide {
  pad_Two  = 2,
  pad_Line = 10,
};


//------------------------------------------------------------------------------
//
// ---
enum PArraySortMemory {
  pam_1      = 100,
  pam_1Delta = 10,
  pam_2      = 1000,
  pam_2Delta = 200,
  pam_3      = 10000,
  pam_3Delta = 2300,
  pam_4      = 100000,
  pam_4Delta = 27182,
};


//------------------------------------------------------------------------------
//
// ---
enum PResSO {
  pso_More,
  pso_Less,
  pso_Equal,
};


//------------------------------------------------------------------------------
//
// ---
class PArrayReg {
public:
  size_t armin;
  size_t armax;
public:
  PArrayReg( size_t oMin, size_t oMax = SYS_MAX_T )
    : armin( oMin )
    , armax( oMax )
  {}
};


//------------------------------------------------------------------------------
// \ru массив указателей с быстрой сортировкой и поиском \en array of pointers with quick sorting and searching 
// ---
template <class Type>
class PArraySort : public PArray<Type> {
private:
  typedef size_t (*PArSortAddress      )( const Type * );
  typedef size_t (*PArSortObj          )( const void *, const Type * );
  typedef bool   (*PArSortRangeCompFunc)( const void *, const Type *, const Type * );

public:
  PArraySort( size_t i_upper = 5, uint16 i_delta = 5, bool shouldDelete = true ); // \ru конструктор \en constructor 

  void    Add         ( Type * ent           );                 // \ru добавить элемент в конец массива \en add element to the end of array 
  void    AddAt       ( Type * ent, size_t k )  { Insert( k, ent ); }
  void    AddAfter    ( Type * ent, size_t k ); // \ru добавить элемент после указанного \en add element after specified one 
  void    Add         ( PArray<Type> &           ); // \ru добавить массив \en add array 
  void    Add         ( PArray<Type> &, size_t k ); // \ru добавить массив в позицию \en add array to the position 
  void    Insert      ( size_t k, Type * ent ); // \ru вставить элемент перед указанным \en insert element before the specified one 

  void    RemoveRng   ( size_t, size_t, DelType del = defDelete );  // \ru удалить диапазон указателей из массива \en delete a range of pointers from array 
  void    DetachRng   ( size_t, size_t );                           // \ru отцепить из массива диапазон указателей \en detach a range of pointers from array 

  typedef int (*PArSortCompFunc)( const Type **, const Type ** );
  void    Sort        ( PArSortCompFunc, PArrayReg * = nullptr ); // \ru быстрая сортировка в любом диапазоне \en quick sorting in any range
  void    Sort        ( const void *, PArSortRangeCompFunc,  size_t armin = 0, size_t armax = SYS_MAX_T ); // \ru быстрая сортировка в заданном диапазоне \en quick sorting in a given range 
  bool    Find        ( const Type *, PArSortAddress, size_t &, size_t armin = 0, size_t armax = SYS_MAX_T ); // \ru найти адрес в любом поле объекта \en find address in any field of object 
  bool    Find        ( const size_t, PArSortAddress, size_t &, size_t armin = 0, size_t armax = SYS_MAX_T ); // \ru найти адрес в любом поле объекта \en find address in any field of object 
  bool    Find        ( const void *, PArSortObj,     size_t &, size_t armin = 0, size_t armax = SYS_MAX_T ); // \ru найти данный объект в сортированном массиве \en find a given object in sorted array 

  int     FindObj     ( const Type *, PArSortCompFunc, size_t &, PArrayReg * = nullptr ) const; // \ru найти данный объект в сортированном массиве \en find a given object in sorted array
  Type *  AddSort     (       Type *, PArSortCompFunc, size_t & ); // \ru добавить элемент в сортированном порядке \en add element in sorted order 

  void    Inverse     (); // \ru инверсия массива \en inversion of array 

  void    Reserve     ( size_t n ); // \ru зарезервировать место под n элементов \en reserve memory for n elements 

private:
  bool    FindObject  ( const void *, PArSortObj,     size_t &, size_t, size_t ); // \ru найти данный объект в сортированном массиве \en find a given object in sorted array 
  bool    FindAddress ( const size_t, PArSortAddress, size_t &, size_t, size_t ); // \ru найти адрес в любом поле объекта \en find address in any field of object 
  void    SortRange   ( const void *, PArSortRangeCompFunc,  size_t, size_t ); // \ru быстрая сортировка в заданном диапазоне \en quick sorting in a given range 
  void    CatchMemory (); // \ru захват памяти \en capture of memory 
  size_t  CalculateDelta();

private:
  PArraySort( const PArraySort & ); // \ru не реализовано \en not implemented 
  void operator = ( const PArraySort & ); // \ru не реализовано \en not implemented 
};


//-----------------------------------------------------------------------------
// \ru конструктор \en constructor 
// ---
template<class Type>
inline PArraySort<Type>::PArraySort( size_t i_upper, uint16 i_delta, bool shouldDelete )
  : PArray<Type>( i_upper, i_delta, shouldDelete ) {
}


//------------------------------------------------------------------------------
// \ru добавить элемент \en add element 
// ---
template <class Type>
inline void PArraySort<Type>::Add( PArray<Type> & o )
{
  PArray<Type>::AddArray( o );
  if ( PArray<Type>::owns )
    o.OwnsElem( false );
}


//------------------------------------------------------------------------------
// \ru добавить элемент \en add element 
// ---
template <class Type>
inline void PArraySort<Type>::Add( PArray<Type> & o, size_t index )
{
  PArray<Type>::InsertArray( o, index );
  if ( PArray<Type>::owns )
    o.OwnsElem( false );
}


//-----------------------------------------------------------------------------
// \ru добавить элемент \en add element 
// ---
template <class Type>
inline void PArraySort<Type>::Add( Type * ent )
{
  CatchMemory();
  this->operator[](  PArray<Type>::count++ ) = ent;
}


//------------------------------------------------------------------------------
// \ru добавить элемент \en add element 
// ---
template <class Type>
inline void PArraySort<Type>::AddAfter( Type * ent, size_t index )
{
  if ( ! PArray<Type>::count )
    Add( ent );
  else {
    CatchMemory(); // \ru добавить памяти, если все использовано \en add memory if whole allocated memory is used 
    PArray<Type>::AddAfter( ent, index );
  }
}


//------------------------------------------------------------------------------
// \ru вставить элемент \en insert element 
// ---
template <class Type>
inline void PArraySort<Type>::Insert( size_t index, Type * ent )
{
  CatchMemory(); // \ru добавить памяти, если все использовано \en add memory if whole allocated memory is used 
  PArray<Type>::Insert( index, ent );
}


//------------------------------------------------------------------------------
// \ru удалить диапазон из массива \en delete a range from array 
// ---
template <class Type>
void PArraySort<Type>::RemoveRng( size_t startIndex, size_t countRng, DelType del )
{
  C3D_ASSERT(  PArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily

  // \ru если диапазон не пуст и индекс принадлежит массиву \en if range is not empty and index belongs to array 
  if ( countRng && startIndex <  PArray<Type>::count ) {

    // \ru корректируем размер удаляемого региона \en correct the deleted region size 
    if ( startIndex + countRng > PArray<Type>::count )
      countRng = PArray<Type>::count - startIndex;

    // \ru вычислить флаг необходимости удаления объектов \en calculate a flag of necessity to delete objects 
    if ( del == Delete || ( del == defDelete &&  PArray<Type>::owns ) ) {

      // \ru будем удалять \en to delete 
      size_t i = 0;
      for( const Type ** parr =  PArray<Type>::GetAddr() + startIndex; i < countRng; i++, parr++ ) {
        Type * d = (Type*)*parr;
        *parr = nullptr; // \ru сначала обнулим указатель ... \en set pointer to null ...

        C3D_ASSERT( !d || PArray<Type>::nowDeletedElem != d );  // \ru ЯТ - временно \en ЯТ - temporarily 
        PArray<Type>::nowDeletedElem = d;

        delete d;

        PArray<Type>::nowDeletedElem = nullptr;
      }
    }

    // \ru вычислить новую позицию \en calculate a new position 
    size_t newPos = startIndex + countRng;

    // \ru передвинуть элементы в конце на стартовую позицию \en move elements at the end to the start position 
    const Type **parr =  PArray<Type>::GetAddr();

    memcpy( (void*)(parr + startIndex), (void*)(parr + newPos), (PArray<Type>::count - newPos) * SIZE_OF_POINTER );

    PArray<Type>::count -= countRng; // \ru установить новое количество элементов в массиве \en set a new number of elements in array 
  }
}


//------------------------------------------------------------------------------
// \ru Отсоединить от массива диапазон указателей без удаления \en Detach from array a range of pointers without deletion 
// ---
template <class Type>
inline void PArraySort<Type>::DetachRng( size_t startIndex, size_t countRng ) {
  RemoveRng( startIndex, countRng, noDelete );
}


//------------------------------------------------------------------------------
// \ru Определить величину приращения \en Determine a value of increment 
// ---
template <class Type>
inline size_t PArraySort<Type>::CalculateDelta()
{
  if (  PArray<Type>::count > pam_1/*100*/ ) {
    if (  PArray<Type>::count < pam_2/*1000*/ ) {
      if (  PArray<Type>::delta < (uint16)pam_1Delta/*10*/ )
         PArray<Type>::delta = (uint16)pam_1Delta;
    }
    else if (  PArray<Type>::count < pam_3/*10000*/ ) {
      if (  PArray<Type>::delta < (uint16)pam_2Delta/*200*/ )
         PArray<Type>::delta = (uint16)pam_2Delta;
    }
    else if (  PArray<Type>::count < pam_4/*100000*/ ) {
      if (  PArray<Type>::delta < (uint16)pam_3Delta/*2300*/ )
         PArray<Type>::delta = (uint16)pam_3Delta;
    }
    else { // >= 100000
      if (  PArray<Type>::delta < (uint16)pam_4Delta/*27182*/ )
         PArray<Type>::delta = (uint16)pam_4Delta;
    }
  }
  return  PArray<Type>::delta;
}


//-----------------------------------------------------------------------------
// \ru захват большего куска памяти ( если нужно ) \en allocate the larger piece of memory (if it is necessary) 
// ---
template <class Type>
inline void PArraySort<Type>::CatchMemory()
{
  PRECONDITION(  PArray<Type>::nowDeletedElem == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily
  if (  PArray<Type>::upper ==  PArray<Type>::count )
    set_Parray_size( *this, PArray<Type>::upper + CalculateDelta(), false/*clear*/ );
}


//------------------------------------------------------------------------------
// \ru зарезервировать место под n элементов \en reserve memory for n elements 
// ---
template <class Type>
inline void PArraySort<Type>::Reserve( size_t n )
{
  C3D_ASSERT(  PArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily
  // \ru если требуется памяти больше, чем есть сейчас, и больше, чем оказалось бы \en if there is required more memory that exists at the moment and more than it would become 
  // \ru при следующем захвате, то захватить ее \en on the next allocation then allocate it 
  size_t space =  PArray<Type>::upper -  PArray<Type>::count;
  if ( space < n && (space + CalculateDelta() < n) )
    set_Parray_size( *this, PArray<Type>::count + n, false/*clear*/ );
}


//-----------------------------------------------------------------------------
// \ru быстрая сортировка всего массива по признаку заданному функцией fcmp \en quick sort of the whole array with the specified by the function 'fcmp' criteria 
// ---
template<class Type>
inline void PArraySort<Type>::Sort( PArSortCompFunc fcmp, PArrayReg * arReg )
{
  PRECONDITION(  PArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily
  if (  PArray<Type>::count > 1 ) { // \ru если хотя бы два элемента в массиве \en if there are at least two elements in array 
    typedef int (*QCompFunc)( const void*, const void* );
    if ( !arReg ) {
      ::qsort( (void *) PArray<Type>::GetAddr(),  PArray<Type>::count, sizeof(size_t), (QCompFunc)fcmp );
    }
    else if ( arReg->armin < arReg->armax ) { // \ru если сортируемый диапазон правильный \en if the sorted range is correct 
      size_t maxInd = PArray<Type>::count - 1;
      // \ru если минимальная граница меньше максимального элемента в массиве \en if the minimum bound is less than the maximum element in array 
      if ( arReg->armin < maxInd ) {
        if ( arReg->armax > maxInd )
          arReg->armax = maxInd;
        ::qsort( (void *)( PArray<Type>::GetAddr() + arReg->armin), (arReg->armax - arReg->armin + 1), sizeof(size_t), (QCompFunc)fcmp );
      }
    }
  }
}


//-----------------------------------------------------------------------------
// \ru быстрая сортировка в заданном диапазоне по признаку заданному функцией fcmp \en quick sort of the given range with the specified by the function 'fcmp' criteria 
// ---
template<class Type>
inline void PArraySort<Type>::Sort( const void * obj, PArSortRangeCompFunc fcmp, size_t armin, size_t armax )
{
  C3D_ASSERT(  PArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily
  if (  PArray<Type>::count > 1 ) { // \ru если хотя бы два элемента в массиве \en if there are at least two elements in array 
    if ( armax > armin ) { // \ru если диапазон сортировки правильный \en if the sorting range is correct 
      if ( armin < PArray<Type>::count - 1 ) { // \ru если минимальная граница меньше максимального элемента в массиве \en if the minimum bound is less than the maximum element in array 
        if ( armax >= PArray<Type>::count ) // \ru если максимальная граница больше или равна максимальному индексу плюс один \en if the maximum boundis not less than maximum index plus one 
          armax = PArray<Type>::count - 1; // \ru установить armax равным максимальному индексу в массиве \en set 'armax' to be equal to the maximum index in array 
        SortRange( obj, fcmp, armin, armax ); // \ru функция быстрой сортировки \en the quick sort function 
      }
    }
  }
}


//-----------------------------------------------------------------------------
// \ru быстрая сортировка в заданном диапазоне по признаку заданному функцией fcmp \en quick sort of the given range with the specified by the function 'fcmp' criteria 
// ---
template<class Type>
void PArraySort<Type>::SortRange( const void * obj, PArSortRangeCompFunc fcmp, size_t ilo, size_t ihi )
{
  C3D_ASSERT(  PArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily
  size_t lo  = ilo; // \ru нижняя граница \en lower bound 
  size_t hi  = ihi; // \ru верхняя граница \en upper bound 

  const Type * mid = (const Type*)(*this)/*parr*/[(lo + hi) / 2]; // \ru найти значение в середине диапазона \en find the value in the middle of the range 

  do { // \ru до тех пор пока верхняя граница больше или равна нижней \en until the upper bound is not less than the lower bound 
    while ( !fcmp(obj, (const Type*)(*this)/*parr*/[lo], mid ) )
      lo++; // \ru если нет необходимости переставлять нижний индекс \en if there is no reason to change position of the lower index 

    while ( !fcmp(obj, mid, (const Type*)(*this)/*parr*/[hi]) )
      hi--; // \ru если нет необходимости переставлять верхний индекс \en if there is no reason to change position of the upper index 

    if ( lo <= hi ) { // \ru если нижняя граница меньше или равна верхней то \en if the lower bound is not greater than the upper bound then 
      Type *t  = (*this)/*parr*/[lo]; // \ru переставить объекты в массиве \en exchange positions of objects in array 
      (*this)/*parr*/[lo] = (*this)/*parr*/[hi];
      (*this)/*parr*/[hi] = t;
      lo++; // \ru сдвинуть границу \en move the bound 
      hi--;
    }
    else
      break;

  } while ( true );

  if ( hi > ilo ) // \ru рекурсия для верхней и нижней границы \en recursion for upper and lower bounds 
    SortRange( obj, fcmp, ilo, hi );

  if ( lo < ihi )
    SortRange( obj, fcmp, lo, ihi );
}


//-----------------------------------------------------------------------------
// \ru найти адрес в любом поле объекта \en find address in any field of object 
// ---
template<class Type>
inline bool PArraySort<Type>::Find( const size_t address, PArSortAddress fadr, size_t & findedAddress,
                                    size_t armin, size_t armax )
{
  C3D_ASSERT(  PArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily
  bool res = false;

  findedAddress = SYS_MAX_T/*OV_x64 -1*/;

  if (  PArray<Type>::count && armin <  PArray<Type>::count && armax >= armin ) {

    if ( armax >=  PArray<Type>::count ) {
      armax =  PArray<Type>::count;
      armax--;
    }

    findedAddress = armax;
    findedAddress++;

    res = FindAddress( address, fadr, findedAddress, armin, armax );
  }

  return res;
}


//-----------------------------------------------------------------------------
// \ru найти адрес в любом поле объекта \en find address in any field of object 
// ---
template<class Type>
inline bool PArraySort<Type>::Find( const Type * member, PArSortAddress fadr, size_t & findedAddress, size_t armin, size_t armax )
{
  C3D_ASSERT(  PArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily
  return Find( fadr(member), fadr, findedAddress, armin, armax );
}


//------------------------------------------------------------------------------
// \ru найти данный объект в сортированном массиве \en find a given object in sorted array 
// ---
template<class Type>
inline bool PArraySort<Type>::Find( const void * obj, PArSortObj fobj, size_t & findedId, size_t armin, size_t armax )
{
  C3D_ASSERT(  PArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily

  bool res = false;

  findedId = SYS_MAX_T;

  if ( PArray<Type>::count && armin <  PArray<Type>::count && armax >= armin ) {

    if ( armax >=  PArray<Type>::count ) {
      armax = PArray<Type>::count;
      armax--;
    }

    findedId = armax;
    findedId++;

    res = FindObject( obj, fobj, findedId, armin, armax );
  }

  return res;
}


//------------------------------------------------------------------------------
// \ru найти данный объект в сортированном массиве \en find a given object in sorted array 
// ---
template<class Type>
bool PArraySort<Type>::FindObject( const void * obj, PArSortObj fobj, size_t & findedId, size_t armin, size_t armax )
{
  C3D_ASSERT(  PArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily

  bool res = false; // \ru флаг, который указывает существует ли искомый объект в массиве \en flag which specifies whether the required object is in array 

  size_t id  = armax - armin; // \ru количество элементов в диапазоне \en the number of elements in range 

  if ( id < paf_Flat ) { // \ru если количество элементов в диапазоне небольшое, то просматриваем сначала \en if the number of elements in range is not too large then start search from the beginning 

    for ( size_t i = armin; i <= armax; i++ ) { // \ru (т.к. одно умножение в 4 раза дольше сравнения) \en (because one multiplying takes in 4 times more time than a comparison) 
      size_t pso = fobj( obj, (*this)/*parr*/[i] );

      if ( !(pso == pso_More) ) { // \ru возвращаем номер вершины \en return a number of vertex 
        res = pso == pso_Equal;
        findedId = i;
        break;
      }
    }
  }
  else {
    // \ru находим вершину находящуюся в середине диапазона (целочисленное деление) \en find a vertex in the middle of range (integer division) 
    size_t half = id / pad_Two + armin;
    size_t pso  = fobj( obj, (*this)/*parr*/[half] );

    if ( pso == pso_More )
      armin = half;
    else
      armax = half;

    res = FindObject( obj, fobj, findedId, armin, armax ); // \ru рекурсивно вызываем функцию с новым диапазоном \en recursively call the function with new range 
  }

  return res;
}


//-----------------------------------------------------------------------------
// \ru найти адрес в любом поле объекта \en find address in any field of object 
// ---
template<class Type>
bool PArraySort<Type>::FindAddress( const size_t address, PArSortAddress fadr, size_t & findedAddress, size_t armin, size_t armax )
{
  C3D_ASSERT(  PArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily

  bool res = false; // \ru флаг, который указывает существует ли искомый адресс в массиве \en flag which specifies whether the required address is in array 
  size_t id  = armax - armin; // \ru количество элементов в диапазоне \en the number of elements in range 

  if ( id < paf_Flat ) { // \ru если количество элементов в диапазоне не большое, то просматривем сначала \en if the number of elements in range is not too large then start search from the beginning 

    for ( size_t i = armin; i <= armax; i++ ) { // \ru (т.к. одно умножение в 4 раза дольше сравнения) \en (because one multiplying takes in 4 times more time than a comparison) 
      size_t currentAddress = fadr( (*this)/*parr*/[i] );

      if ( !(currentAddress < address) ) { // \ru возвращаем номер вершины, \en return a number of vertex, 
        res = currentAddress == address;
        findedAddress = i;
        break;
      }
    }
  }
  else {
    // \ru если количество элементов в массиве достаточное, то деление пополам \en if the number of elements in array is enough then perform a bisection 
    if ( id < paf_Bytwo ) {
      // \ru находим вершину находящуюся в середине диапазона (целочисленное деление) \en find a vertex in the middle of range (integer division) 
      size_t half        = id / pad_Two + armin;
      size_t addressHalf = fadr( (*this)/*parr*/[half] );

      if ( addressHalf < address )
        armin = half;
      else
        armax = half;

      res = FindAddress( address, fadr, findedAddress, armin, armax ); // \ru рекурсивно вызываем функцию с новым диапазоном \en recursively call the function with new range 
    }
    else { // \ru если вершин очень много в массиве, то используем линейную аппроксимацию \en if there are too many vertices in array then use a linear approximation 
      size_t minAddress = fadr( (*this)/*parr*/[armin] ); // \ru минимальный адрес \en minimal address 

      if ( minAddress != address ) { // \ru если не равен искомому адресу \en if it is not equal to the required address 
        size_t maxAddress = fadr( (*this)/*parr*/[armax] ); // \ru максимальный адрес в массиве \en maximal address in array 

        size_t d = maxAddress - minAddress; // \ru диапазон адресов в массиве \en range of addresses in array 
        size_t i = ( address - minAddress ) * id / d + armin; // \ru предполагаемый индекс искомого адреса \en the expected index of the required address 

        if ( i < armin ) // \ru выровнять диапазон \en justify the range 
          i = armin;
        else if ( i > armax )
          i = armax;

        size_t middleAddress = fadr( (*this)/*parr*/[i] ); // \ru найти адрес вершины по данному индексу \en find the address of vertex by the given index 

        if ( middleAddress < address ) { // \ru если искомый адрес больше \en if the required address is greater 

          armin = i; // \ru установить нижнюю границу \en set the lower bound 
          id    = armax - armin; // \ru вычислить новый диапазон индексов \en calculate the new range of indices 

          if ( id > paf_Byline ) { // \ru если он достаточно большой оценить верхнюю границу \en if it is rather large then estimate the upper bound 
            size_t h        = id / pad_Line + armin;
            size_t haddress = fadr( (*this)/*parr*/[h] );

            if ( haddress < address )
              armin = h;
            else
              armax = h;
          }

          res = FindAddress( address, fadr, findedAddress, armin, armax );
        }
        else { // \ru если искомый адрес меньше \en if the required address is less 

          armax = i; // \ru установить верхнюю границу \en set the upper bound 
          id    = armax - armin; // \ru вычислить новый диапазон индексов \en calculate the new range of indices 

          if ( id > paf_Byline ) { // \ru если он достаточно большой оценить нижнюю границу \en if it is rather large then estimate the lower bound 
            size_t h        = armax - id / pad_Line;
            size_t haddress = fadr( (*this)/*parr*/[h] );

            if ( haddress < address )
              armin = h;
            else
              armax = h;
          }

          res = FindAddress( address, fadr, findedAddress, armin, armax );
        }
      }
      else { // \ru иначе искомая вершина находится в начале массива \en otherwise the required value is situated in the begin of array 
        res           = true;
        findedAddress = armin;
      }
    }
  }

  return res;
}


//------------------------------------------------------------------------------
// \ru найти данный объект в сортированном массиве (!!!obj - обязан быть первым в fcmp!!!) \en find the given object in sorted array (!!! 'obj'  sould be the first in 'fcmp' !!!) 
// ---
template<class Type>
int PArraySort<Type>::FindObj( const Type * obj, PArSortCompFunc fcmp, size_t & iFnd, PArrayReg * arReg ) const
{
  PRECONDITION(  PArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily
  if ( (PArray<Type>::count) > 0 ) {
    size_t id = PArray<Type>::count; // \ru количество элементов в диапазоне \en the number of elements in range 
    id--;
    size_t armin, armax, half;
    if ( !arReg ) {
      if ( fcmp((const Type **)&obj, (const Type **)&((*this)[id])) > 0 ) {
        iFnd = id;
        return 2;
      }
      armin = 0;
      armax = id;
    }                                         
    else {
      if ( arReg->armax < id ) {
        if ( fcmp((const Type **)&obj, (const Type **)&((*this)[arReg->armax])) > 0 ) {
          iFnd = arReg->armax;
          return 1;
        }
        armax = arReg->armax;
      }
      else {
        if ( fcmp((const Type **)&obj, (const Type **)&((*this)[id])) > 0 ) {
          iFnd = id;
          return 2;
        }
        armax = id;
      }
      armin = ( (arReg->armin < armax) ? arReg->armin : armax );
      id = ( armax - armin );
    }
    int ires = fcmp( (const Type **)&obj, (const Type **)&((*this)[armin]) );
    if ( ires <= 0 ) { // \ru возвращаем номер вершины, \en return a number of vertex, 
      iFnd = armin;
      return ires;
    }
    while ( id > paf_Flat ) {
      id /= 2;
      half = id + armin;
      ires = fcmp( (const Type **)&obj, (const Type **)&((*this)[half]) );
      if ( ires > 0 ) {
        armin = half;
      }
      else if ( ires < 0 ) {
        armax = half;
      }
      else {
        iFnd = half;
        while ( (half > 0) && !fcmp((const Type **)&obj, (const Type **)&((*this)[--half])) )
          iFnd = half;
        return 0;
      }
    }
    // \ru если количество элементов в диапазоне не большое, то просматриваем сначала \en if the number of elements in range is not too large then start search from the beginning 
    for ( size_t i = armin; i <= armax; i++ ) { // \ru (т.к. одно умножение в 4 раза дольше сравнения) \en (because one multiplying takes in 4 times more time than a comparison) 
      ires = fcmp( (const Type **)&obj, (const Type **)&((*this)[i]) );
      if ( ires <= 0 ) { // \ru возвращаем номер вершины, \en return a number of vertex, 
        iFnd = i;
        return ires;
      }
    }
    iFnd = armax;
    return 1;
  }
  return -2;
}


//------------------------------------------------------------------------------
// \ru добавить элемент в сортированном порядке \en add element in sorted order 
// ---
template<class Type>
inline Type * PArraySort<Type>::AddSort( Type * obj, PArSortCompFunc fcmp, size_t & iFnd )
{
  PRECONDITION(  PArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily
  int ires = FindObj( obj, fcmp, iFnd );
  if ( (ires == -2) || (ires == 2) ) {
    iFnd = ( (ires == -2) ? 0 : PArray<Type>::count );
    Add( obj );
  }
  else
    if ( ires == -1 )
      AddAt( obj, iFnd );

  return ires ? (*this)/*parr*/[iFnd] : nullptr;
}


//------------------------------------------------------------------------------
// \ru инверсия массива \en inversion of array 
// ---
template<class Type>
void PArraySort<Type>::Inverse()
{
  C3D_ASSERT(  PArray<Type>::nowDeletedElem  == nullptr ); // \ru ЯТ - временно \en ЯТ - temporarily

  if (  PArray<Type>::count ) {
    size_t endI = PArray<Type>::count - 1; // \ru проверено count > 0 \en count > 0 validated 
    size_t fstI = 0;

    while ( (ptrdiff_t)fstI < (ptrdiff_t)endI ) {

      Type * fstObj = (*this)/*parr*/[fstI]; // \ru запомнить указатель на объект (это копия указателя!!!) \en remember the pointer to the object (this is the copy of the pointer!!!) 

      (*this)/*parr*/[fstI] = (*this)/*parr*/[endI];
      (*this)/*parr*/[endI] = fstObj;

      fstI++;
      endI--;
    }
  }
}


#endif // __TEMPL_PSRT_ARRAY_H
