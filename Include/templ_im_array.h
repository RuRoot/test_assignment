////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Упорядоченный массив индексов присланного PArray.
         \en Ordered array of indices of the given PArray. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_IM_ARRAY_H
#define __TEMPL_IM_ARRAY_H


#include <templ_s_array.h>
#include <templ_p_array.h>


FORVARD_DECL_TEMPLATE_TYPENAME( class IMArray );
FORVARD_DECL_TEMPLATE_TYPENAME( Type *  add_to_array        ( IMArray<Type> &, size_t ind, size_t * ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t  add_to_array        ( IMArray<Type> &, Type * el,  size_t * ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t  find_in_array       ( const IMArray<Type> &, const void *, size_t * ) );
FORVARD_DECL_TEMPLATE_TYPENAME( void    array_remove_ind    ( IMArray<Type> &, size_t delIndex, bool completely ) );
FORVARD_DECL_TEMPLATE_TYPENAME( void    array_remove_obj    ( IMArray<Type> &, const size_t & delObj, bool completely ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t  find_my_index       ( const IMArray<Type> &, size_t ) );
FORVARD_DECL_TEMPLATE_TYPENAME( Type *  reindex_array_obj   ( IMArray<Type> &, size_t ind, size_t * myIndex ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t  reindex_array_obj   ( IMArray<Type> &, Type * el,  size_t * myIndex ) );
FORVARD_DECL_TEMPLATE_TYPENAME( Type *  reindex_array_ind   ( IMArray<Type> &, size_t myIndex ) );
FORVARD_DECL_TEMPLATE_TYPENAME( bool    exchange_to_array   ( IMArray<Type> &, size_t ind1, size_t ind2 ) );
FORVARD_DECL_TEMPLATE_TYPENAME( void    reindexall_to_array ( IMArray<Type> & ) );
FORVARD_DECL_TEMPLATE_TYPENAME( void    array_reduction_obj ( IMArray<Type> & arr, const size_t & delObject ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader& CALL_DECLARATION operator >> ( reader& in, IMArray<Type> & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer& CALL_DECLARATION operator << ( writer& out, const IMArray<Type> & ref ) );


//------------------------------------------------------------------------------
/** \brief \ru Упорядоченный массив индексов присланного PArray.
           \en Ordered array of indices of the given PArray. \~
  \details \ru Упорядоченный массив индексов присланного PArray. \n
           \en Ordered array of indices of the given PArray. \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class IMArray : private SArray<size_t> {
public :
  typedef int (Type::*Compare_t)( const Type * );
  typedef int (Type::*Compare_v)( const void * );

  PArray<Type> & array;
  Compare_t      compT;  // \ru функция сортировки используется при добавлении объекта \en sorting function is used while adding an object 
  Compare_v      compV;  // \ru функция сортировки используется при поиске объекта \en sorting function is used while search an object 

public:
  IMArray( PArray< Type > & arr, Compare_t c_t, Compare_v c_v,  size_t maxCnt = 0, uint16 delt = 1 );

  using SArray<size_t>::Flush;
  using SArray<size_t>::Count;
  using SArray<size_t>::Sort; // \ru сортировать массив \en sort the array 
  using SArray<size_t>::Reserve;
  using SArray<size_t>::SetSize;

  using SArray<size_t>::empty;
  using SArray<size_t>::size;
  using SArray<size_t>::reserve;
  using SArray<size_t>::capacity;
  using SArray<size_t>::begin;
  using SArray<size_t>::end;
  using SArray<size_t>::front;
  using SArray<size_t>::back;
  using SArray<size_t>::clear;
 
  Type  *         Add( size_t ind, size_t * );   // \ru добавить элемент с упорядочиванием по массиву \en add element with sorting 
  size_t          Add( Type * ent, size_t * );   // \ru добавить элемент с упорядочиванием по массиву \en add element with sorting 

  Type *          operator [] ( size_t ) const;
  size_t &        operator () ( size_t ) const;
  IMArray<Type> & operator =  ( const IMArray<Type> & );

  size_t          Find      ( const void *, size_t * ) const; // \ru найти элемент в упорядоченном массиве \en find an element in ordered array 
  size_t          GetIndex  ( size_t myIndex ) const;
  size_t          GetMyIndex( size_t parentIndex ) const;

  void            RemoveInd ( size_t delIndex, bool completely = true );          // \ru удалить элемент из массива \en delete an element from array 
  void            RemoveObj ( const size_t & delObject, bool completely = true ); // \ru удалить элемент из массива \en delete an element from array 

  Type  *         ReindexInd( size_t ind, size_t * = nullptr ); // \ru заменить элемент с упорядочиванием по массиву \en replace element with sorting
  size_t          ReindexObj( Type * ent, size_t * = nullptr ); // \ru заменить элемент с упорядочиванием по массиву \en replace element with sorting

  Type  *         ReindexMyInd( size_t ); // \ru заменить элемент с упорядочиванием по массиву \en replace element with sorting 

  bool            Exchange    ( size_t ind1, size_t ind2 ); // \ru поменять местами по индексам в папином массиве \en swap by indices in the parent array 
  void            ReindexAll  ();                           // \ru перестроить индексный массив \en reconstruct the index array 

  bool            ReductionObj( const size_t & delObject ); // \ru понижение всех индексов > delObject \en decrease of all indices greater than 'delObject' 

  TEMPLATE_FRIEND Type *  add_to_array        TEMPLATE_SUFFIX ( IMArray<Type> &, size_t ind, size_t * );
  TEMPLATE_FRIEND size_t  add_to_array        TEMPLATE_SUFFIX ( IMArray<Type> &, Type * el, size_t * );
  TEMPLATE_FRIEND size_t  find_in_array       TEMPLATE_SUFFIX ( const IMArray<Type> &, const void *, size_t * );
  TEMPLATE_FRIEND void    array_remove_ind    TEMPLATE_SUFFIX ( IMArray<Type> &, size_t delIndex, bool completely );
  TEMPLATE_FRIEND void    array_remove_obj    TEMPLATE_SUFFIX ( IMArray<Type> &, const size_t & delObj, bool completely );
  TEMPLATE_FRIEND size_t  find_my_index       TEMPLATE_SUFFIX ( const IMArray<Type> &, size_t  );
  TEMPLATE_FRIEND Type *  reindex_array_obj   TEMPLATE_SUFFIX ( IMArray<Type> &, size_t ind, size_t * myIndex );
  TEMPLATE_FRIEND size_t  reindex_array_obj   TEMPLATE_SUFFIX ( IMArray<Type> &, Type* el, size_t * myIndex );
  TEMPLATE_FRIEND Type *  reindex_array_ind   TEMPLATE_SUFFIX ( IMArray<Type> &, size_t myIndex );
  TEMPLATE_FRIEND bool    exchange_to_array   TEMPLATE_SUFFIX ( IMArray<Type> &, size_t ind1, size_t ind2 );
  TEMPLATE_FRIEND void    reindexall_to_array TEMPLATE_SUFFIX ( IMArray<Type> & );
  TEMPLATE_FRIEND void    array_reduction_obj TEMPLATE_SUFFIX ( IMArray<Type> &arr, const size_t & delObject );

private:
  IMArray( const IMArray<Type> & ); // \ru запрещено !!! \en forbidden !!! 
  TEMPLATE_FRIEND reader& CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader& in, IMArray<Type> & ref );
  TEMPLATE_FRIEND writer& CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer& out, const IMArray<Type> & ref );


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
inline void * IMArray<Type>::operator new( size_t size ) {
  return ::Allocate( size, typeid(IMArray<Type>).name() );
}

//------------------------------------------------------------------------------
// \ru Перегрузка оператора delete. \en Overloading of the "delete" operator. 
// ---
template <class Type>
inline void IMArray<Type>::operator delete ( void * ptr, size_t size ) {
  ::Free( ptr, size, typeid(IMArray<Type>).name() );
}
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_


//-------------------------------------------------------------------------------
// \ru конструктор массива \en constructor of an array 
// ---
template <class Type>
inline  IMArray<Type>::IMArray( PArray< Type > & arr, Compare_t c_t, Compare_v c_v,  size_t maxCnt, uint16 delt )
  : SArray< size_t >( maxCnt, delt ), array(arr), compT(c_t), compV(c_v)  {}


//-------------------------------------------------------------------------------
// \ru добавить объект по индексу в PAarray \en add an object by the index in PArray 
// ---
template <class Type>
inline Type* IMArray<Type>::Add( size_t ind, size_t * myIndex ) {
  return add_to_array( *this, ind, myIndex );
}


//-------------------------------------------------------------------------------
// \ru добавить объект, объект в PArray должен быть \en add an object, the object in PArray should exist 
// ---
template <class Type>
inline size_t IMArray<Type>::Add( Type * el, size_t * myIndex ) {
  return add_to_array( *this, el, myIndex );
}


//-------------------------------------------------------------------------------
// \ru удалить объект по индексу \en delete an object by index 
// \ru completely  = true - с понижением всех индексов > delIndex \en completely  = true - with decrease of all indices greater than delIndex 
// ---
template <class Type>
inline void IMArray<Type>::RemoveInd( size_t delIndex, bool completely ) {
  array_remove_ind( *this, delIndex, completely );
}


//-------------------------------------------------------------------------------
// \ru удалить объект \en delete an object  
// \ru completely  = true - с понижением всех индексов > delIndex \en completely  = true - with decrease of all indices greater than delIndex 
// ---
template <class Type>
inline void IMArray<Type>::RemoveObj( const size_t & delObject, bool completely ){
  array_remove_obj( *this, delObject, completely );
}


//-------------------------------------------------------------------------------
// \ru понижение всех индексов > delObject \en decrease of all indices greater than 'delObject' 
// ---
template <class Type>
inline bool IMArray<Type>::ReductionObj( const size_t & delObject ) {
// \ru АВВ К12 Результат нужен только из этой функции \en АВВ К12 Only the result of this function is required 
// \ru АВВ К12 array_reduction_obj сделал void-ной для ускорения \en АВВ К12 array_reduction_obj is void for acceleration 
// \ru АВВ К12   return array_reduction_obj( *this, delObject ); \en АВВ К12   return array_reduction_obj( *this, delObject ); 
  bool res = false;
  size_t * parr = (size_t *)GetAddr();
  size_t * end  = parr + count;
  
  while ( parr < end )
  {  
    if ( *parr > delObject ) 
    {
      --(*parr);
      res = true;
    }  
    parr++;
  }

  return res;
}


//-------------------------------------------------------------------------------
// \ru переупорядочить объект по индексу в PArray \en reorder an object by the index in PArray 
// \ru функция возвращает указатель на объект и индекс объекта в IMArray \en the function returns a pointer to the object and an index of the object in IMArray 
// ---
template <class Type>
inline Type* IMArray<Type>::ReindexInd( size_t ind, size_t * myIndex ) {
  return reindex_array_obj( *this, ind, myIndex );
}


//-------------------------------------------------------------------------------
// \ru переупорядочить объект \en reorder an object  
// \ru функция возвращает индекс объекта в PArray и индекс объекта в IMArray \en the function returns an index of the object in PArray and an index of the object in IMArray 
// ---
template <class Type>
inline size_t IMArray<Type>::ReindexObj( Type * ent, size_t * myIndex ) {
  return reindex_array_obj( *this, ent, myIndex );
}


//-------------------------------------------------------------------------------
// \ru переупорядочить объект \en reorder an object  
// \ru функция возвращает указатель на объект \en the function returns a pointer to the object 
// ---
template <class Type>
inline Type * IMArray<Type>::ReindexMyInd( size_t myIndex ) {
  return reindex_array_ind( *this, myIndex );
}


//-------------------------------------------------------------------------------
// \ru поменять идексы в PArray местами \en swap indices in PArray 
// ---
template <class Type>
inline bool IMArray<Type>::Exchange ( size_t ind1, size_t ind2 ) {
  return exchange_to_array( *this, ind1, ind2 );
}


//-------------------------------------------------------------------------------
// \ru переупорядочить весь массив \en reorder the whole array 
// ---
template <class Type>
inline void  IMArray<Type>::ReindexAll() {
  reindexall_to_array( *this );
}


//-------------------------------------------------------------------------------
// \ru оператор индексирования \en indexing operator 
// ---
template <class Type>
inline Type* IMArray<Type>::operator [] ( size_t ind ) const {
  return array[ SArray < size_t >::operator []( ind ) ];
}


//-------------------------------------------------------------------------------
// \ru оператор присвоения \en assignment operator 
// ---
template <class Type>
inline IMArray<Type>& IMArray<Type>::operator = ( const IMArray<Type> & o ) {
  SArray< size_t >::operator = ( o );
  return *this;
}


//-------------------------------------------------------------------------------
// \ru найти объект удовлетворяющий условию \en find an object satisfying the condition 
// ---
template <class Type>
inline size_t IMArray<Type>::Find( const void * val, size_t * myIndex ) const {
  return find_in_array( *this, val, myIndex );
}


//-------------------------------------------------------------------------------
// \ru получить индекс объекта в PArray по иедексу в IMArray \en get an index of object in PArray by the index in IMArray 
// ---
template <class Type>
inline size_t IMArray<Type>::GetIndex( size_t myIndex ) const {
  return SArray< size_t >::operator[] (myIndex);
}


//-------------------------------------------------------------------------------
// \ru получить индекс объекта в IMArray по индексу в PArray (перебор) \en get an index of object in PArray by the index in IMArray (full search) 
// ---
template <class Type>
inline size_t IMArray<Type>::GetMyIndex( size_t parentIndex ) const {
  return find_my_index( *this, parentIndex );
}


//-------------------------------------------------------------------------------
// \ru оператор приведения \en adduction operator 
// ---
template <class Type>
inline size_t & IMArray<Type>::operator ()( size_t myIndex ) const {
  return SArray< size_t >::operator[] ( myIndex );
}


//-------------------------------------------------------------------------------
// \ru добавить объект с упорядочиванием \en add an object with sorting 
// ---
template <class Type>
Type * add_to_array( IMArray<Type> & arr, size_t ind, size_t * myIndex ) {
  PRECONDITION( ind < arr.array.Count() );

  Type * el = 0;
  size_t mcIArr = SYS_MAX_T; // \ru текущий индекс в IMArray \en the current index in IMArray 

  if ( ind < arr.array.Count() ) {
    el = arr.array[ind];

    if ( el ) {

      if ( !arr.count ) {
        arr.SArray< size_t >::Add( ind );
        mcIArr = 0;
      }
      else {
        size_t ml = 0;
        mcIArr = ml;
        size_t mcPArr = arr.GetIndex( mcIArr ); // \ru текущий индекс в PArray \en the current index in PArray 

        // \ru проверяем первый эл. \en check the first element 
        int resL = ( el->*arr.compT )( arr.array[mcPArr] );

        // \ru если элемент меньше первого, вставляем перед первым \en if an element is less than the first element then place it before the first element 
        if ( resL < 0 )
          arr.InsertInd( mcIArr, ind );
        else {
          size_t mr = arr.count - 1;
          mcIArr = mr;
          mcPArr = arr.GetIndex( mcIArr );

          // \ru проверяем последний эл. \en check the last element 
          int resR = !mcIArr ? resL : ( el->*arr.compT )( arr.array[mcPArr] );

          // \ru если один объект в массиве или эл. больше или равен последнему, \en if one object in the array or an element is not less than the last element, 
          // \ru должны попасть сюда \en should be here 
          if ( !mcIArr || resR >= 0 ) {
            arr.SArray< size_t >::Add( ind );
            mcIArr = mr + 1;
          }
          else {
            if ( arr.count == 2 )
              arr.InsertInd( mcIArr, ind );
            else {

              // \ru сюда попадаем если resL >= 0 и resR < 0 \en this is the case when resL >= 0 and resR < 0   
              while ( ml + 1 < mr ) {  // \ru пока не нашли - ищем \en seek until do not find 

                if ( !resL ) {
                  mcIArr = ml;

                  //   68759              do {
                  //                   mcIArr++;
                  //                   mcPArr = arr.GetIndex( mcIArr );
                  //                 } while ( (el->*arr.compT)( arr.array[mcPArr] ) == 0 );
                  mcIArr++;
                  mcPArr = arr.GetIndex( mcIArr );

                  break;
                }
                else {
                  size_t md = ( ml + mr ) / 2;
                  mcPArr = arr.GetIndex( md );

                  int res = ( el->*arr.compT )( arr.array[mcPArr] );

                  if ( res > 0 )
                    ml = md;
                  else if ( res < 0 ) {
                    mr = md;
                    mcIArr = md;
                  }
                  else {
                    resL = res;     //res = 0
                    mcIArr = md + 1;  // \ru если  ml + 1 < mr не выполнится объект должен добавиться после md \en if ml + 1 is not less than mr then the object should be added after md 
                    ml = md;
                  }
                }
              }

              arr.InsertInd( mcIArr, ind );
            }
          }
        }
      }
    }
  }

  if ( myIndex )
    *myIndex = mcIArr;

  return el;
}


//-------------------------------------------------------------------------------
// \ru добавить объект с упорядочиванием \en add an object with sorting 
// \ru получить индекс объекта в PArray \en get an index of the object in PArray 
// ---
template <class Type>
size_t add_to_array( IMArray<Type> & arr, Type * el, size_t * myIndex ) {
  size_t ind = SYS_MAX_T;
  ptrdiff_t  mi  = arr.array.MaxIndex();

  if ( mi >= 0 ) {

    if ( arr.array[mi] == el )
      ind = mi;
    else
      ind = arr.array.FindIt(el);
  }

  if ( ind == SYS_MAX_T ) {

    if ( myIndex )
      *myIndex = SYS_MAX_T;
  }
  else
    add_to_array( arr, ind, myIndex );

  return ind; 
}


//-------------------------------------------------------------------------------
// \ru найти объект в массиве, удовлетворяющий условию \en find an object in array, satisfying the condition 
// \ru поиск ведется методом половинных делений \en a search is performed by the bisection method 
// ---
template <class Type>
size_t find_in_array( const IMArray<Type> & arr, const void * val, size_t * myIndex ) {
  // \ru общий случай - элементов больше двух \en the common case - the number of elements is more than two 
  int res = 1;

  size_t mcIArr; // \ru текущий индекс в IMArray \en the current index in IMArray 
  size_t mcPArr; // \ru текущий индекс в PArray \en the current index in PArray 

  if ( arr.count > 3 ) {
    size_t mr = arr.count - 1;

    size_t mxc = mr;
    size_t ml  = 0;

    while ( ml + 1 < mr ) {  // \ru пока не нашли - ищем \en seek until do not find 
      mcIArr = ( ml + mr ) / 2;
      mcPArr = arr.GetIndex( mcIArr );
      res    = (arr.array[mcPArr]->*arr.compV)( val );

      if ( res == 1 ) 
        mr = mcIArr; 
      else if ( res == - 1 ) 
        ml = mcIArr;
      else
        break;
    }

    if ( res ) {
      // \ru проверка по границам \en check by bounds 
      mcIArr = 0;
      mcPArr = arr.GetIndex( mcIArr );
      res    = (arr.array[mcPArr]->*arr.compV)( val );

      if ( res ) {
        mcIArr = mxc;
        mcPArr = arr.GetIndex( mcIArr );
        res    = (arr.array[mcPArr]->*arr.compV)( val );
      }
    }
  }
  else {
    
    for( mcIArr = 0; mcIArr < arr.count; mcIArr++ ) {
      mcPArr = arr.GetIndex( mcIArr );
      res    = (arr.array[mcPArr]->*arr.compV)( val );

      if ( res >= 0 )
        break;
    }
  }

  if ( res ) {
    mcIArr = SYS_MAX_T;
    mcPArr = SYS_MAX_T;
  }

  if ( myIndex )
    *myIndex = mcIArr;

  return mcPArr;
}


//-------------------------------------------------------------------------------
// \ru удалить объект по индексу \en delete an object by index 
// \ru completely  = true - с понижением всех индексов > delIndex \en completely  = true - with decrease of all indices greater than delIndex 
// ---
template <class Type>
void array_remove_ind( IMArray<Type> & arr, size_t delIndex, bool completely ){
  PRECONDITION( delIndex < arr.count );
  if ( delIndex < arr.count ) {
    size_t arrayIndex = arr.GetIndex( delIndex ); //arr.parr[ delIndex ];
    arr.SArray< size_t >::RemoveInd( delIndex );
    if ( completely )
      array_reduction_obj( arr, arrayIndex );
  }
}


//-------------------------------------------------------------------------------
// \ru удалить объект \en delete an object 
// \ru completely  = true - с понижением всех индексов > delIndex \en completely  = true - with decrease of all indices greater than delIndex 
// ---
template <class Type>
void array_remove_obj( IMArray<Type> & arr, const size_t & delObject, bool completely ){
  arr.SArray< size_t >::RemoveObj( delObject );

  if ( completely )
    array_reduction_obj( arr, delObject );
}

// \ru АВВ К12 //------------------------------------------------------------------------------- \en АВВ К12 //------------------------------------------------------------------------------- 
// \ru АВВ К12 // понижение всех индексов > delObject \en АВВ К12 // decrease of all indices greater than delObject 
// \ru АВВ К12 // --- \en АВВ К12 // --- 
// \ru АВВ К12 template <class Type> \en АВВ К12 template <class Type> 
// \ru АВВ К12 bool array_reduction_obj( IMArray<Type> & arr, const uint & delObject ){ \en АВВ К12 bool array_reduction_obj( IMArray<Type> & arr, const uint & delObject ){ 
// \ru АВВ К12   bool res = false; \en АВВ К12   bool res = false; 
// \ru АВВ К12   const uint *parr = arr.GetAddr(); \en АВВ К12   const uint *parr = arr.GetAddr(); 
// \ru АВВ К12 \en АВВ К12  
// \ru АВВ К12   for( uint i = 0; i < arr.count; i++ ) { \en АВВ К12   for( uint i = 0; i < arr.count; i++ ) { 
// \ru АВВ К12     uint *parrI = (uint *)(parr + i); \en АВВ К12     uint *parrI = (uint *)(parr + i); 
// \ru АВВ К12 \en АВВ К12  
// \ru АВВ К12     if ( *parrI > delObject ){ \en АВВ К12     if ( *parrI > delObject ){ 
// \ru АВВ К12       (*parrI)--; \en АВВ К12       (*parrI)--;  
// \ru АВВ К12       res = true; \en АВВ К12       res = true; 
// \ru АВВ К12     } \en АВВ К12     } 
// \ru АВВ К12   } \en АВВ К12   } 
// \ru АВВ К12   return res; \en АВВ К12   return res; 
// \ru АВВ К12 } \en АВВ К12 } 


//-------------------------------------------------------------------------------
// \ru понижение всех индексов > delObject \en decrease of all indices greater than 'delObject' 
// ---
template <class Type>
void array_reduction_obj( IMArray<Type> & arr, const size_t & delObject ){
  size_t *parr = (size_t *)arr.GetAddr();
  size_t *end  = parr + arr.count;

  while ( parr < end )
  {  
    if ( *parr > delObject ) 
      --(*parr);
    
    parr++;
  }
}


//-------------------------------------------------------------------------------
// \ru получить индекс объекта в IMArray по индексу в PArray \en get an index of object in PArray by the index in IMArray  
// ---
template <class Type>
size_t find_my_index( const IMArray<Type> & arr, size_t parentIndex ) {

  for ( size_t i = 0; i < arr.count; i++ )

    if ( arr.GetIndex(i) == parentIndex )
      return i;

  return SYS_MAX_T;
}


//-------------------------------------------------------------------------------
// \ru переупорядочить объект в массиве \en reorder an object in the array 
// ---
template <class Type>
size_t reindex_array_obj( IMArray<Type> & arr, Type * el, size_t * myIndex ) {
  size_t ind = arr.array.FindIt(el);

  if ( ind == SYS_MAX_T ) {

    if ( myIndex )
      *myIndex = SYS_MAX_T;
    return ind;
  }

  reindex_array_obj( arr, ind, myIndex );

  return ind;
}


//-------------------------------------------------------------------------------
// \ru переупорядочить объект в массиве \en reorder an object in the array 
// ---
template <class Type>
Type* reindex_array_obj( IMArray<Type> & arr, size_t ind, size_t * myIndex ) {
  arr.SArray< size_t >::RemoveObj( ind );
  return add_to_array( arr, ind, myIndex );
}


//-------------------------------------------------------------------------------
// \ru переупорядочить объект в массиве \en reorder an object in the array 
// ---
template <class Type>
Type* reindex_array_ind( IMArray<Type> & arr, size_t myIndex ) {
  size_t ind = arr.GetIndex( myIndex ); //arr.parr[ myIndex ];
  arr.SArray< size_t >::RemoveInd( myIndex );
  return add_to_array( arr, ind, 0 );
}


//-------------------------------------------------------------------------------
// \ru переупорядочить весь массив \en reorder the whole array 
// ---
template <class Type>
void reindexall_to_array( IMArray<Type> & arr ) {

  arr.SArray< size_t >::Flush();

  for( size_t i = 0; i < arr.array.Count(); i++ )
    add_to_array( arr, i, 0 );
}


//-------------------------------------------------------------------------------
// \ru поменять идексы в PArray местами \en swap indices in PArray 
// ---
template <class Type>
bool exchange_to_array( IMArray<Type> & arr, size_t ind1, size_t ind2 ) {
  size_t myInd1 = arr.SArray< size_t >::FindIt( ind1 );

  if ( !(myInd1 == SYS_MAX_T) ) {
    size_t myInd2 = arr.SArray< size_t >::FindIt( ind2 );

    if ( !(myInd2 == SYS_MAX_T) ) {
      const size_t *parr = arr.GetAddr();
      *(size_t *)(parr + myInd1) = ind2;
      *(size_t *)(parr + myInd2) = ind1;
      return true;
    }
  }
  return false;
}


#endif // __TEMPL_IM_ARRAY_H
