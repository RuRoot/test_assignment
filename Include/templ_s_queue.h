////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Очередь объектов, которые не имеют деструкторов.
         \en Queue of objects without destructors. \~

*/
////////////////////////////////////////////////////////////////////////////////


#ifndef __TEMPL_S_QUEUE_H
#define __TEMPL_S_QUEUE_H


#include <io_define.h>
#include <tool_quick_sort.h>
#include <tool_err_handling.h>
#include <string.h>


//------------------------------------------------------------------------------
/** \brief \ru Очередь объектов, которые не имеют деструкторов.
           \en Queue of objects without destructors. \~
  \details \ru Очередь объектов, которые не имеют деструкторов. \n

  Требования к объектам очереди такие же, как в SArray<Type>.
           \en Queue of objects without destructors. \n

  Requirement for the objects are the same as in SArray<Type>. \~
  \attention \ru В целях эффективности контейнера проверки обращения к запредельной 
  памяти реализованы только под откладкой
             \en In order to efficiency of the container validation - the usage of prohibitive 
  memory are implemented only for debug version \~
  \par \ru Рекомендации по использованию           
  Если предполагается фиксированный (известный) наибольший размер очереди, то 
  рекомендуется использовать функцию SQueue<class Type>::Push() для добавления 
  объекта. Если предельный размер очереди не известен, то следует применять метод
  SQueue<class Type>::PushAlloc(), который при добавлении "наращивает" память 
  по мере необходимости.
       \en Usage recommendations           
  If the fixed (known) maximum size of queue is assumed then 
  it is recommended to use the function SQueue<class Type>::Push() to add 
  an object. If the limit size of queue is unknown then there should be applied the method
  SQueue<class Type>::PushAlloc() which builds up the memory 
  as necessary. \~

  \par \ru Правило выделения памяти
  Если размера буфера для хранения объектов очереди не достаточно, то выделяется 
  новый участок кучи с размеров на SQueue::delta больше. Если SQueue::delta = 1,
  то размер выделенной памяти уведичивается на 12,5% при каждом достижении предела.
       \en The rule of memory allocation
  If the size of the buffer for storage of queue objects is not enough then allocated 
  a new part of the heap which has size greater on SQueue::delta. If SQueue::delta = 1,
  then the size of allocated memory is increased by 12,5% each time the limit is reached. \~

  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class SQueue 
{
  Type *  data;   ///< \ru Адрес начала выделенной памяти. \en An address of the beginning of the allocated memory. 
  Type *  qlast;  ///< \ru Адрес конца выделенной памяти (последний элемент массива выделенной памяти). \en An address of the end of the allocated memory (the last element of the allocated memory's array) 
  Type *  qp1;    ///< \ru Самый первый извлекаемый (queue pointer 1). \en The first extracted one (queue pointer 1). 
  Type *  qp2;    ///< \ru Следующий элемент от последнего вошедшего (queue pointer 2). \en The next element from the last entering (queue pointer 2). 

public:
  /// \ru Конструктор. \en Constructor. 
  SQueue( size_t capacity = 0 );
  /// \ru Деструктор. \en Destructor. 
  virtual ~SQueue();
public:
  /// \ru Выдать размер выделенной памяти. \en Get the size of the allocated memory. 
  size_t        Capacity() const { return qlast+1-data; }
  /// \ru Последний элемент очереди. \en The last element of the queue. 
  Type &        Back();
  /// \ru Последний элемент очереди. \en The last element of the queue. 
  const Type &  Back() const;
  /// \ru Первый элемент очереди. \en The first element of the queue. 
        Type &  Front();
  /// \ru Первый элемент очереди. \en The first element of the queue. 
  const Type &  Front() const;
  /// \ru Свойство пустого множества. \en A property of the empty set. 
  bool          Empty() const { return qp1 == qp2; }
  /// \ru Свойство исчерпанной памяти. \en An expended memory property. 
  bool          IsFull() const;
  /// \ru Самый первый, выходящий из очереди (корректно работает только для непустой очереди). \en The very first one outgoing from the queue (it works correctly only for nonempty queue). 
  Type &        First() const;
  /// \ru Добавить в очередь и нарастить буфер выделенной памяти при необходимости. \en Add to the queue and increase the buffer of the allocated memory if it is necessary. 
  void          Push( const Type & obj );
  /// \ru Вывести из очереди. \en Move out from queue. 
  void          Pop();
  /// \ru Вывести из очереди. \en Move out from queue. 
  void          Pop( Type & obj );
  /// \ru Зарезервировать дополнительную память. \en Reserve an additional memory. 
  bool          Reserve( size_t );
  /// \ru Очистить очередь. \en Clear the queue. 
  void          SetEmpty() { qp1 = qp2 = data; }
  /// \ru Выдать размер очереди. \en Get the queue size. 
  size_t        Size() const;

private:
  /// \ru Инкремент указателя с учётом зацикленности памяти. \en An increment of the pointer considering a looped memory. 
  Type *       _IncPtr( Type * ptr ) const;
  /// \ru Добавить в очередь без проверки израсходованной памяти. \en Add to the queue without check of consumed memory. 
  void         _Push( const Type & u );
  /// \ru Задать наибольший размер очереди. \en Set the maximum size of the queue. 
  bool         _NewCapacity( size_t max_len, bool clear );
};

//-------------------------------------------------------------------------------
//
// ---
template <class Type>
SQueue<Type>::SQueue( size_t capacity )
  : data( nullptr )
  , qlast( nullptr )
  , qp1( nullptr )
  , qp2( nullptr )
{ 
  if ( capacity > 0 ) {
    try {
      data = new Type[capacity];
    }
    catch ( const std::bad_alloc & ) {
      data = nullptr;
      throw;
    }
  }

  qlast = data+capacity-1;
  qp1 = qp2 = data;
}


//-------------------------------------------------------------------------------
//
// ---
template <class Type>
SQueue<Type>::~SQueue() 
{
  delete [] data;

#ifdef C3D_DEBUG
  ::memset( &qp1,   0xFE, sizeof(Type *) );
  ::memset( &qp2,   0xFE, sizeof(Type *) );
  ::memset( &qlast, 0xFE, sizeof(Type *) );
  ::memset( &data,  0xFE, sizeof(Type *) );
#endif // C3D_DEBUG
}


//-------------------------------------------------------------------------------
/// \ru Кто крайний? (корректно работает только для непустой очереди); \en Which is the last? (it works correctly only for nonempty queue); 
// ---
template <class Type>
inline const Type & SQueue<Type>::Back() const 
{
  PRECONDITION( !Empty() && Capacity() > 0 );
  return qp2 == data ? *qlast : *( qp2 - 1 );
}

//-------------------------------------------------------------------------------
/// \ru Кто крайний? (корректно работает только для непустой очереди) \en Which is the last? (it works correctly only for nonempty queue) 
// ---
template <class Type>
inline Type & SQueue<Type>::Back()
{
  PRECONDITION( !Empty() && Capacity() > 0 );
  return qp2 == data ? *qlast : *( qp2 - 1 );
}

//-------------------------------------------------------------------------------
// \ru Первый элемент очереди. \en The first element of the queue. 
// ---
template <class Type>
inline Type & SQueue<Type>::Front()
{
  PRECONDITION( Capacity() > 0 && !Empty() );
  return *qp1;
}

//-------------------------------------------------------------------------------
// \ru Первый элемент очереди. \en The first element of the queue. 
// ---
template <class Type>
inline const Type & SQueue<Type>::Front() const
{
  PRECONDITION( Capacity() > 0 && !Empty() );
  return *qp1;
}

//-------------------------------------------------------------------------------
// \ru Самый первый, выходящий из очереди (корректно работает только для непустой очереди). \en The very first one outgoing from the queue (it works correctly only for nonempty queue). 
// ---
template <class Type>
inline Type & SQueue<Type>::First() const
{
  PRECONDITION( qp1 <= qlast && qp1 >= data && qp1 != qp2 );
  return *qp1;
}


//-------------------------------------------------------------------------------
/// \ru Свойство полностью исчерпанного буфера \en A expended buffer property. 
/**\ru Фактически свойство отвечает возможно ли добавить в очередь элемент без 
  передислокации буфера.
   \en In fact this property answeres whether it is possible to add in a queue an element without 
  redeployment of the buffer. \~
*/
// ---
template <class Type>
inline bool SQueue<Type>::IsFull() const
{  
  return data == nullptr || _IncPtr( qp2 ) == qp1;
}


//-------------------------------------------------------------------------------
/// \ru Инкремент указателя с учётом зацикленности памяти \en An increment of the pointer considering a looped memory 
//---
template <class Type>
inline Type * SQueue<Type>::_IncPtr( Type * ptr ) const 
{ 
  PRECONDITION( ptr >= data && ptr <= qlast );
  if ( ++ptr > qlast ) 
    ptr = data; 
  return ptr; 
}


//-------------------------------------------------------------------------------
//
// ---
template <class Type>
inline void SQueue<Type>::_Push( const Type & obj ) 
{
  PRECONDITION( !IsFull() && Size() < Capacity() );
  if ( !IsFull() && Size() < Capacity() ) {
    memcpy( qp2, &obj, sizeof( Type ) );
    qp2 = _IncPtr( qp2 );
    PRECONDITION( !Empty() ); // \ru после добавления очередь не может быть пустой \en the queue may bacome empty after the adding 
    PRECONDITION( qp2 >= data && qp2 <= qlast );
  }
}


//-------------------------------------------------------------------------------
/// \ru Добавить в очередь и нарастить буфер выделенной памяти при необходимости \en Add to the queue and increase the buffer of the llocated memory if it is necessary 
/**\ru Тоже, что и #SQueue<Type>::Push, но с перезахватом памяти при исчерпании буфера
   \en The same as  #SQueue<Type>::Push, but with memory reallocation in a case when the buffer is full \~
*/
// ---
template <class Type>
void SQueue<Type>::Push( const Type & obj ) 
{  
  if ( IsFull() ) // \ru Требуется перезахват памяти \en A memory reallocation is required 
  {    
    size_t cap = Capacity();
    cap += ::KsAutoDelta( cap );
    if ( !_NewCapacity(cap, false/*clear*/) )
      return;
  }
  _Push( obj );
  
  PRECONDITION( qp2 >= data && qp2 <= qlast );
}


//-------------------------------------------------------------------------------
//
// ---
template <class Type>
inline void SQueue<Type>::Pop( Type & obj ) 
{
  PRECONDITION( qp2 != qp1 ); // \ru перед извлечением очередь не может быть пустой \en the queue can't be empty before the extraction 
  if ( qp2 != qp1 ) {
    memcpy( &obj, qp1, sizeof( Type ) );
    qp1 = _IncPtr( qp1 );
    PRECONDITION( qp1 <= qlast && qp1 >= data );
  }
}


//-------------------------------------------------------------------------------
//
// ---
template <class Type>
inline void SQueue<Type>::Pop() 
{
  PRECONDITION( qp2 != qp1 ); // \ru перед извлечением очередь не может быть пустой \en the queue can't be empty before the extraction   
  if ( qp2 != qp1 ) {
    qp1 = _IncPtr( qp1 );
    PRECONDITION( qp1 <= qlast && qp1 >= data );
  }
}


//-------------------------------------------------------------------------------
/// \ru Зарезервировать дополнительную память \en Reserve an additional memory 
// ---
template <class Type>
bool SQueue<Type>::Reserve( size_t addCapacity )
{
  PRECONDITION( addCapacity > 0 );
  const size_t futureSize = Size() + addCapacity;  // \ru Предполагаемый размер, растущей очереди \en An assumed size of increasing queue 
  if ( Capacity() < futureSize )
  {
    return _NewCapacity( futureSize, false );
  }  
  return true;
}


//-------------------------------------------------------------------------------
/// \ru Выдать размер очереди \en Get the queue size 
//---
template <class Type>
inline size_t SQueue<Type>::Size() const
{
  if ( qp2 >= qp1 )
  {
    return qp2 - qp1; // \ru Нефрагментированная очередь \en Not fragmented queue 
  }
  return ( qlast + 1 - qp1 ) + ( qp2 - data ); // \ru Сумма длин двух фрагментов очереди \en The lengths sum of two fragments of queue  
}


//-------------------------------------------------------------------------------
/// \ru Задать наибольший размер очереди \en Set the maximum size of the queue 
// ---
template <class Type>
bool SQueue<Type>::_NewCapacity( size_t max_len, bool clear ) 
{
  if ( clear || Empty() || max_len == 0 ) 
  {
    try {
      delete [] data;
      data = qlast = qp1 = qp2 = nullptr;
      if ( max_len > 0 ) {
        data = new Type[max_len];
        qlast = data + max_len - 1;
        qp1 = qp2 = data;
      }
    }
    catch ( ... ) {
      data = qlast = qp1 = qp2 = nullptr;
      C3D_CONTROLED_THROW;
      return false;
    }
  }
  else 
  {
    PRECONDITION( qp1 != qp2 && max_len>0 && clear == false ); // \ru Выражение обязано быть истинным \en The expression should be true 
    Type * n_data = nullptr;
    try {
      n_data = new Type[max_len];
      if ( qp1 < qp2 ) // \ru Вариант без фрагментации \en A variant without fragmentation 
      {
        size_t len = std_min( (size_t)/*OV_x64 (uint)*/( qp2 - qp1 ), max_len ); // \ru Количество ячеек с полезной информацией (не байты) \en The number of cells with useful information (not bites) 
        PRECONDITION( len <= max_len );
        ::memcpy( n_data, qp1, len * sizeof( Type ) );
        delete[] data;
        data = qp1 = qp2 = n_data;
        qp2 += len;
      }
      else if ( qp1 > qp2 ) // \ru Вариант с фрагментированным массивом полезной информации \en A variant with the fragmented array of useful information 
      {
        size_t len1 = std_min( (size_t)( qlast - qp1 + 1 ), max_len );   // \ru Количество ячеек первого фрагмента \en The number of cells of the first fragment 
        size_t len2 = std_min( (size_t)( qp2 - data ), (size_t)( max_len - len1 ) ); // \ru Количество ячеек второго фрагмента \en The number of cells of the second fragment 
        PRECONDITION( len1 + len2 <= max_len );
        ::memcpy( n_data, qp1, len1 * sizeof( Type ) );
        ::memcpy( n_data + len1, data, len2 * sizeof( Type ) );
        delete[] data;
        data = qp1 = qp2 = n_data;
        qp2 += len1;
        qp2 += len2;
      }
      qlast = data + max_len - 1;
    }
    catch ( ... ) {
      C3D_CONTROLED_THROW;
      return false;
    }
  }
  return true;
}


#endif // __TEMPL_S_QUEUE_H
