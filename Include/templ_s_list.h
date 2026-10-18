////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Список.
         \en List. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_S_LIST_H
#define __TEMPL_S_LIST_H


#include <system_types.h>
#include <templ_delete_define.h>
#include <io_define.h>
#include <tool_err_handling.h>



//-----------------------------------------------------------------------------
/** \brief \ru Элемент списка.
           \en The list element. \~
  \details \ru Элемент списка. \n
           \en The list element. \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class  ListItem {
public:
  ListItem<Type>  * next; ///< \ru Указатель на следующий элемент. \en A pointer to the next element. 
  Type            * data; ///< \ru Указатель на поле данных. \en A pointer to field of data. 

public:
  ListItem()          : next( 0 ), data( 0 ) {}
  ListItem( Type *d ) : next( 0 ), data( d ) {}
  ListItem( Type *d, ListItem<Type> &prev ); // \ru добавить себя после заданного элемента \en add self after the given element 

private:
  ListItem        ( const ListItem& ); // \ru запрещено \en forbidden 
  void operator = ( const ListItem& ); // \ru запрещено \en forbidden 
};


//------------------------------------------------------------------------------
// \ru вставляет себя после item \en inserts itself after 'item' 
//---
template <class Type>
inline ListItem<Type>::ListItem( Type *d, ListItem<Type> &prev )  {
  next = prev.next;
  prev.next = this;
  data = d;
}


//------------------------------------------------------------------------------
//
// ---
FORVARD_DECL_TEMPLATE_TYPENAME( class  LIterator ); 
FORVARD_DECL_TEMPLATE_TYPENAME( class List ); 
FORVARD_DECL_TEMPLATE_TYPENAME( void                     add_to_list ( List<Type> &, List<Type> & ) );
FORVARD_DECL_TEMPLATE_TYPENAME( void                     add_to_list ( List<Type> &, Type* data, const Type* after ) );
FORVARD_DECL_TEMPLATE_TYPENAME( void                     insert_to_list ( List<Type> &, List<Type> & ) );
FORVARD_DECL_TEMPLATE_TYPENAME( void                     remove_from_list ( List<Type> &, DelType shdl ) );
FORVARD_DECL_TEMPLATE_TYPENAME( void                     remove_from_list_release ( List<Type> & ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t                   remove_from_list ( List<Type> &, List<Type> &, DelType shdl ) );
FORVARD_DECL_TEMPLATE_TYPENAME( bool                     detach_from_list ( List<Type> &, const Type * ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t                   recalc_list ( List<Type> & ) );
FORVARD_DECL_TEMPLATE_TYPENAME( bool                     is_exist_in_list ( const List<Type> &, const Type * ) );
FORVARD_DECL_TEMPLATE_TYPENAME( ListItem<Type> *         find_prev_in_list ( const List<Type> &, ListItem<Type>* now ) );

class reader;
class writer;
FORVARD_DECL_TEMPLATE_TYPENAME( reader& CALL_DECLARATION operator >> ( reader& in, List<Type> & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer& CALL_DECLARATION operator << ( writer& out, const List<Type> & ref ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader& CALL_DECLARATION operator >> ( reader& in, List<Type> *& ptr ) );
FORVARD_DECL_TEMPLATE_TYPENAME( writer& CALL_DECLARATION operator << ( writer& out, const List<Type> * ptr ) );


//-----------------------------------------------------------------------------
/** \brief \ru Список указателей на элементы.
           \en List of pointers to elements. \~
  \details \ru Список указателей на элементы. \n
           \en List of pointers to elements. \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class  List {
protected:
  bool              owns;   ///< \ru Признак владения элементами. \en Attribute of ownership of elements. 
  size_t            count;  ///< \ru Кол-во эл-тов в списке. \en The number of elements in the list. 
  ListItem<Type>  * first;  ///< \ru Указатель на первый элемент в списке. \en A pointer to the first list element. 
  ListItem<Type>  * last;   ///< \ru Указатель на последний элемент в списке. \en A pointer to the last list element. 

  ListItem<Type>  * nowDelItem;  // \ru Временно, для отладки \en Temporarily, for debugging 
  Type            * nowDelElem;  // \ru Временно, для отладки \en Temporarily, for debugging 

public:
  explicit List( bool ownsEl = true )
    : owns( ownsEl )
    , count( 0 )
    , first( nullptr )
    , last( nullptr )
    , nowDelItem( nullptr )
    , nowDelElem( nullptr )
  {}
  virtual ~List();

  bool    OwnsElem() const                              { return owns;   }
  void    OwnsElem( bool ownsEl )                       { owns = ownsEl; }

  void    Add( Type * );                             // \ru добавить элемент в конец списка \en add an element to the end of the list 
  void    Add( Type *, const Type *after );          // \ru добавить элемент после заданного элемента \en add an element after the given element 
  void    Add( Type *, bool check );                 // \ru добавить элемент в конец списка с проверкой на существование \en add an element in the end of the list with existence validation. 
  void    Add      ( ListItem<Type> & );             // \ru добавить элемент в конец списка \en add an element to the end of the list 
  void    Add      ( List<Type>     &l ) { add_to_list(*this, l); } // \ru добавить список list в конец данного списка \en add a list to the end of the given list 
  void    AddAndEat( List<Type>     & );             // \ru съесть список list в конец данного списка \en destroy a list and add it to the end of the given list 

  void    Insert      ( Type * );                    // \ru вставить элемент в начало списка \en insert an element to the beginning of the list 
  void    Insert      ( ListItem<Type> & );          // \ru вставить элемент в начало списка \en insert an element to the beginning of the list 
  void    Insert      ( List<Type> &l ) { insert_to_list(*this, l); } // \ru вставить список в начало данного списка \en insert a list to the beginning of the given list 
  void    InsertAndEat( List<Type> & );              // \ru съесть список в начало данного списка \en destroy and add a list to the beginning of the given list 

  void    Flush ( DelType shdl=defDelete )           { Remove( shdl );  }  // \ru удалить все элементы списка \en delete all elements of the list 
  void    Remove( DelType shdl=defDelete )           { remove_from_list(*this, shdl); } // \ru удалить все элементы списка \en delete all elements of the list 

  void    FlushRelease()                             { RemoveRelease();  }  // \ru удалить все элементы списка \en delete all elements of the list 
  void    RemoveRelease()                            { remove_from_list_release(*this); } // \ru удалить все элементы списка \en delete all elements of the list 
  
  bool    Remove( Type *, DelType=defDelete );       // \ru удалить один элемент списка \en delete one element of the list 
  size_t  Remove( List<Type> &l, DelType shdl=defDelete ) { return remove_from_list(*this, l, shdl ); } // \ru удалить все элементы принадлежащие списку l \en delete all elements from the list l 
  bool    Detach( const Type *d )                    { return detach_from_list(*this, d); } // \ru отсоединить один элемент списка \en detach one element from the list 
  size_t  Detach( List<Type> &l )                    { return Remove(l, noDelete); } // \ru отсоединить от списка другой список элементов \en detach other list from the list 

  void    Close();                                   // \ru замкнуть список \en close the list 
  void    Split();                                   // \ru разомкнуть список \en split the list 

  size_t  Count()    const {
    PRECONDITION( nowDelItem == nullptr && nowDelElem == nullptr );
    return count;      }        // \ru дать количество элементов в списке \en get the number of elements in the list 

  size_t  ReCalc()         { return recalc_list(*this); }// \ru пересчитать количество элементов в списке \en count the number of elements in the list 
  bool    IsEmpty () const {
    PRECONDITION( nowDelItem == nullptr && nowDelElem == nullptr );
    return first == 0; }        // \ru проверить, пустой ли список \en check whether the list is empty 

  bool    IsExist( const Type * d ) const { return is_exist_in_list(*this, d);} // \ru найти элемент по равенству указателей \en find an element by the equality of pointers 

  Type  * GetFirstData() const {
    PRECONDITION( nowDelItem == nullptr && nowDelElem == nullptr );
    PRECONDITION( first ); return first->data; } // \ru получить данные первого элемента списка \en get the data of the first element of the list 
  Type  * GetLastData()  const {
    PRECONDITION( nowDelItem == nullptr && nowDelElem == nullptr );
    PRECONDITION( last  ); return last->data;  } // \ru получить данные последнего элемента списка \en get the data of the last element of the list 

protected:
  TEMPLATE_FRIEND class LIterator TEMPLATE_SUFFIX;

private:
  ListItem<Type> * findPrev( ListItem<Type> *now ) { return find_prev_in_list(*this, now); } // \ru найти предыдущий \en find the previous 

  List            ( const List<Type>&  ); // \ru запрещено \en forbidden 
  void operator = ( const List<Type>&  ); // \ru запрещено \en forbidden 

  TEMPLATE_FRIEND void                     add_to_list       TEMPLATE_SUFFIX ( List<Type> &, List<Type> & );
  TEMPLATE_FRIEND void                     add_to_list       TEMPLATE_SUFFIX ( List<Type> &, Type* data, const Type* after );
  TEMPLATE_FRIEND void                     insert_to_list    TEMPLATE_SUFFIX ( List<Type> &, List<Type> & );
  TEMPLATE_FRIEND void                     remove_from_list  TEMPLATE_SUFFIX ( List<Type> &, DelType shdl );
  TEMPLATE_FRIEND void                     remove_from_list_release TEMPLATE_SUFFIX ( List<Type> & );
  TEMPLATE_FRIEND size_t                   remove_from_list  TEMPLATE_SUFFIX ( List<Type> &, List<Type> &, DelType shdl );
  TEMPLATE_FRIEND bool                     detach_from_list  TEMPLATE_SUFFIX ( List<Type> &, const Type * );
  TEMPLATE_FRIEND size_t                   recalc_list       TEMPLATE_SUFFIX ( List<Type> & );
  TEMPLATE_FRIEND bool                     is_exist_in_list  TEMPLATE_SUFFIX ( const List<Type> &, const Type * );
  TEMPLATE_FRIEND ListItem<Type> *         find_prev_in_list TEMPLATE_SUFFIX ( const List<Type> &, ListItem<Type>* now );

  TEMPLATE_FRIEND reader& CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader& in, List<Type> & ref );
  TEMPLATE_FRIEND writer& CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer& out, const List<Type> & ref );
  TEMPLATE_FRIEND reader& CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader& in, List<Type> *& ptr );
  TEMPLATE_FRIEND writer& CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer& out, const List<Type> * ptr );

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
inline void * List<Type>::operator new( size_t size ) {
  return ::Allocate( size, typeid(List<Type>).name() );
}

//------------------------------------------------------------------------------
// \ru Перегрузка оператора delete. \en Overloading of the "delete" operator. 
// ---
template <class Type>
inline void List<Type>::operator delete( void * ptr, size_t size ) {
  ::Free( ptr, size, typeid(List<Type>).name() );
}
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_


//-----------------------------------------------------------------------------
/** \brief \ru Итератор списка.
           \en Iterator of list. \~
  \details \ru Итератор списка. \n
           \en Iterator of list. \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class  LIterator {
protected:
  List<Type>      * list;
  ListItem<Type>  * curr;
  ListItem<Type>  * prev;

public:
  LIterator()                           : list( nullptr ), curr( nullptr ), prev( nullptr ) {}
  LIterator( const List<Type> &     l ) : list( nullptr ), curr( nullptr ), prev( nullptr ) { Set(l); }
  LIterator( const LIterator<Type> &i ) : list( i.list ), curr( i.curr ), prev( i.prev ) {}
  virtual ~LIterator() {}

  void          Set( const List<Type>& l )       { list = (List<Type>*)&l; Restart(); }
  void          Restart()                        { PRECONDITION(list); curr = list->first; prev = nullptr; }
  Type        * GetData()                  const { return curr ? curr->data : nullptr; }
  Type        * GetDataAndGo();                  // \ru взять данные и продвинуть итератор \en take the data and move the iterator 
  List<Type>  * GetList()                  const { return list; }

  Type& operator*   ()                     const { PRECONDITION(curr && curr->data ); return *curr->data; }
  Type* operator () ()                     const { return curr ? curr->data : nullptr; }
        operator ListItem<Type>* ()        const { return curr; }
        operator ListItem<Type>& ()        const { PRECONDITION(curr); return *curr; }
  Type* operator ++()                      { prev = curr; if (curr) {curr=curr->next; return curr ? curr->data : nullptr;} else return nullptr; }
  Type* operator ++(int)                   { prev = curr; if (curr) {Type* ret=curr->data; curr=curr->next; return ret;} else return nullptr; }
  Type* operator --()                      { PRECONDITION(list); curr=prev; prev=list->findPrev(prev); return curr ? curr->data : nullptr; }
  Type* operator --(int)                   { PRECONDITION(list); if (curr) {Type* ret=curr->data; curr=prev; prev=list->findPrev(prev); return ret;} else return nullptr; }
  Type* operator ->()                      { return curr ? curr->data : nullptr; }

  bool  operator == ( const LIterator<Type> &o ) const { return list==o.list && curr==o.curr; }
  bool  operator != ( const LIterator<Type> &o ) const { return ! operator == (o); }
  LIterator<Type> & operator = ( const List<Type> &l ) { Set(l); return* this; }
  LIterator<Type> & operator = ( const LIterator<Type> &o ) { list=o.list; curr = o.curr; prev = o.prev; return *this; }

  void  Add      ( Type * );               // \ru добавить элемент после заданного элемента \en add an element after the given element 
  void  AddAndEat( List<Type> & );         // \ru съесть list после текущего элемента \en destroy the list and add it after the current element 

  void  Insert      ( Type * );            // \ru вставить элемент перед заданным элементом \en insert an element before the given element 
  void  InsertAndEat( List<Type> & );      // \ru съесть list перед заданным элементом \en destroy the list and add it before the given element 

  void  Remove( DelType = defDelete );     // \ru удалить элемент списка и продвинуть вперед \en delete an element from the list and move forward 
  void  Detach();                          // \ru отсоединить элемент списка \en detach an element from the list 

  bool  IsOK()                             const { return curr != nullptr; }
  ListItem<Type>  * Next()                 const { PRECONDITION(curr); return curr->next; }
  void  Go()                                     { prev = curr; if ( curr ) curr = curr->next; }
  void  GoLast()                                 { PRECONDITION(list); curr = list->last; prev = list->findPrev(curr); }

  bool  IsFirst()                          const { PRECONDITION(list); return curr == list->first; }
  bool  IsLast()                           const { PRECONDITION(list); return curr == list->last;  }
};


////////////////////////////////////////////////////////////////////////////////
//
// \ru список указателей на элементы \en a list of pointers to the elements 
//
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//
//---
template <class Type>
inline List<Type>::~List() {
  PRECONDITION( nowDelItem == nullptr && nowDelElem == nullptr );

  Remove();
}


//------------------------------------------------------------------------------
// \ru добавить элемент в конец списка \en add an element to the end of the list 
//---
template <class Type>
inline void List<Type>::Add( Type* data ) {
  PRECONDITION( nowDelItem == nullptr && nowDelElem == nullptr );

  if ( last )
    last = new ListItem<Type>( data, *last );
  else
    first = last = new ListItem<Type>( data );

  count++;
}


//------------------------------------------------------------------------------
// \ru добавить элемент после заданного элемента \en add an element after the given element 
//---
template <class Type>
inline void List<Type>::Add( Type* data, const Type* after ) {
  PRECONDITION( nowDelItem == nullptr && nowDelElem == nullptr );

  add_to_list( *this, data, after );
}


//------------------------------------------------------------------------------
// \ru добавить элемент в конец списка с проверкой на существование \en add an element in the end of the list with existence validation 
//---
template <class Type>
inline void List<Type>::Add( Type * data, bool /*check*/ )
{
  PRECONDITION( nowDelItem == nullptr && nowDelElem == nullptr );
  if ( !IsExist(data) )
    Add( data );
}


//------------------------------------------------------------------------------
// \ru добавить элемент в конец списка \en add an element to the end of the list 
//---
template <class Type>
inline void List<Type>::Add( ListItem<Type> & item )
{
  PRECONDITION( nowDelItem == nullptr && nowDelElem == nullptr );
  item.next = nullptr;

  if ( last )
    last->next = &item;
  else
    first = &item;

  last = &item;
  count++;
}


//------------------------------------------------------------------------------
// \ru добавить список list в конец данного списка \en add a list to the end of the given list 
// \ru list после добавления становится пустым! \en a list becomes empty after the adding of it! 
//---
template <class Type>
inline void List<Type>::AddAndEat( List<Type> & list )
{
  PRECONDITION( nowDelItem == nullptr && nowDelElem == nullptr );

  if ( list.first ) {

    if ( last ) {
      last->next = list.first;
      last = list.last;
    }
    else {
      first = list.first;
      last  = list.last;
    }

    count += list.count;
    list.count = 0;
    list.first = list.last = nullptr;
  }
}


//------------------------------------------------------------------------------
// \ru вставить элемент в начало списка \en insert an element to the beginning of the list 
//---
template <class Type>
inline void List<Type>::Insert( Type * data )
{
  PRECONDITION( nowDelItem == nullptr && nowDelElem == nullptr );
  Insert( *new ListItem<Type>(data) );
}


//------------------------------------------------------------------------------
// \ru вставить элемент в начало списка \en insert an element to the beginning of the list 
//---
template <class Type>
inline void List<Type>::Insert( ListItem<Type> & item )
{
  PRECONDITION( nowDelItem == nullptr && nowDelElem == nullptr );

  ListItem<Type>* old = first;

  first = &item;
  first->next = old;

  if ( !last )
    last = first;

  count++;
}


//------------------------------------------------------------------------------
// \ru съесть список list в начало данного списка \en destroy and add a list to the beginning of the given list 
//---
template <class Type>
inline void List<Type>::InsertAndEat( List<Type> & list )
{
  PRECONDITION( nowDelItem == nullptr && nowDelElem == nullptr );
  if ( list.first ) {

    if ( first ) {
      PRECONDITION( list.last );
      list.last->next = first;
      first = list.first;
    }
    else {
      first = list.first;
      last  = list.last;
    }

    count += list.count;
    list.count = 0;
    list.first = list.last = nullptr;
  }
}


//------------------------------------------------------------------------------
// \ru удалить один элемент списка \en delete one element of the list 
//---
template <class Type>
inline bool List<Type>::Remove( Type * del, DelType shdl )
{
  PRECONDITION( nowDelItem == nullptr && nowDelElem == nullptr );
  if ( Detach(del) ) {

    if ( shdl==Delete || (shdl==defDelete && owns) ) {
      nowDelElem = del;
      delete del;
      nowDelElem = nullptr;
    }

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// \ru замкнуть список \en close the list 
//---
template <class Type>
inline void List<Type>::Close()
{
  PRECONDITION( nowDelItem == nullptr && nowDelElem == nullptr );
  if ( last )
    last->next = first;
}


//------------------------------------------------------------------------------
// \ru разомкнуть список \en split the list 
//---
template <class Type>
inline void List<Type>::Split()
{
  PRECONDITION( nowDelItem == nullptr && nowDelElem == nullptr );
  if ( last )
    last->next = nullptr;
}


////////////////////////////////////////////////////////////////////////////////
//
// \ru итератор списка \en Iterator of list 
//
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// \ru взять данные и продвинуть итератор \en take the data and move the iterator. 
//---
template <class Type>
inline Type * LIterator<Type>::GetDataAndGo()
{
  PRECONDITION( list && list->nowDelItem == nullptr && list->nowDelElem == nullptr );

  if ( curr ) {
    Type * ret = curr->data;
    prev = curr;
    curr = curr->next;
    return ret;
  }

  return nullptr;
}


//------------------------------------------------------------------------------
// \ru добавить элемент после текущего элемента \en add an element after the current element 
//---
template <class Type>
inline void LIterator<Type>::Add( Type * data )
{
  PRECONDITION( list && list->nowDelItem == nullptr && list->nowDelElem == nullptr );

  if ( list ) {
    if ( curr ) {
      ListItem<Type> * newItem = new ListItem<Type>( data, *curr ); // \ru поставит себя после curr \en inserts itself after 'curr' 
      if ( list->last == curr )
        list->last = newItem;
      list->count++;
    }
    else
      list->Add( data );
  }
}


//------------------------------------------------------------------------------
// \ru съесть list после текущего элемента \en destroy a list and add it after the current element 
//---
template <class Type>
inline void LIterator<Type>::AddAndEat( List<Type> & l )
{
  PRECONDITION( list && list->nowDelItem == nullptr && list->nowDelElem == nullptr );

  if ( list && l.first ) {

    if ( curr ) {
      PRECONDITION( l.last );
      l.last->next = curr->next;
      curr->next = l.first;

      if ( list->last == curr )
        list->last = l.last;
      list->count += l.count;

      l.count = 0;
      l.first = l.last = nullptr;
    }
    else
      list->AddAndEat( l ); // \ru съесть список l в конец данного списка \en destroy a list l and add it to the end of the given list 
  }
}


//------------------------------------------------------------------------------
// \ru вставить элемент перед текущим \en insert an element before the specified one 
//---
template <class Type>
inline void LIterator<Type>::Insert( Type * data )
{
  PRECONDITION( list && list->nowDelItem == nullptr && list->nowDelElem == nullptr );

  if ( list ) {
    if ( prev ) {
      ListItem<Type> * newItem = new ListItem<Type>( data, *prev ); // \ru ставит себя после prev \en inserts itself after 'prev' 

      if ( list->last == prev )
        list->last = newItem;

      prev = newItem;
      list->count++;
    }
    else
      list->Insert( data ); // \ru вставить элемент в начало списка \en insert an element to the beginning of the list 
  }
}


//------------------------------------------------------------------------------
// \ru съесть list перед моим текущим элементом \en destroy a list and add it before mine current element 
//---
template <class Type>
inline void LIterator<Type>::InsertAndEat( List<Type> & l )
{
  PRECONDITION( list && list->nowDelItem == nullptr && list->nowDelElem == nullptr );

  if ( list && l.first ) {

    if ( prev ) {
      PRECONDITION( l.last );
      prev->next = l.first;
      l.last->next = curr;
      prev = l.last;

      list->count += l.count;

      l.count = 0;
      l.first = l.last = nullptr;
    }
    else
      list->InsertAndEat( l ); // \ru съесть список l в конец данного списка \en destroy a list l and add it to the end of the given list 
  }
}


//------------------------------------------------------------------------------
// \ru удалить текущий элемент итератора \en delete the current element of the iterator 
//---
template <class Type>
inline void LIterator<Type>::Remove( DelType shdl )
{
  PRECONDITION( list && list->nowDelItem == nullptr && list->nowDelElem == nullptr );

  if ( list && curr ) {
    if ( shdl==Delete || (shdl==defDelete && list->owns) ) {
      list->nowDelElem = curr->data;
      delete curr->data;
      list->nowDelElem = nullptr;
    }

    Detach();
  }
}


//------------------------------------------------------------------------------
// \ru отсоединить элемент списка \en detach an element from the list 
//---
template <class Type>
inline void LIterator<Type>::Detach()
{
  PRECONDITION( list && list->nowDelItem == nullptr && list->nowDelElem == nullptr );

  if ( list && curr ) {
    ListItem<Type>* next = curr->next;

    if ( prev )
      prev->next = next;
    else
      list->first = next;

    if ( !next )
      list->last = prev;

    list->nowDelItem = curr;
    delete curr;
    list->nowDelItem = nullptr;
    curr = next;

    prev = list->findPrev( curr );
    list->count--;
  }
}


//------------------------------------------------------------------------------
template <class Type>
void add_to_list( List<Type> & list, Type * data, const Type * after )
{
  PRECONDITION( list.nowDelItem == nullptr && list.nowDelElem == nullptr );

  if ( after ) {
    ListItem<Type> *c = list.first;

    while ( c ) {
      if ( c->data == after ) {
        ListItem<Type> *newItem = new ListItem<Type>( data, *c ); // \ru поставит себя после c \en inserts itself after 'c' 
        if ( list.last == c )
          list.last = newItem;
        list.count++;
        return;
      }
      else
        c = c->next;
    }
  }

  list.Add( data );
}


//------------------------------------------------------------------------------
template <class Type>
void add_to_list( List<Type> & to, List<Type> & from )
{
  PRECONDITION( to.nowDelItem == nullptr && to.nowDelElem == nullptr );
  PRECONDITION( from.nowDelItem == nullptr && from.nowDelElem == nullptr );

  ListItem<Type> *curr = from.first;
  while ( curr ) {
    to.Add( curr->data );
    curr = curr->next;
  }

  if ( to.owns )
    from.owns = false;
}


//------------------------------------------------------------------------------
template <class Type>
void insert_to_list( List<Type> & to, List<Type> & from )
{
  PRECONDITION( to.nowDelItem == nullptr && to.nowDelElem == nullptr );
  PRECONDITION( from.nowDelItem == nullptr && from.nowDelElem == nullptr );

  ListItem<Type> *curr = from.first;
  while ( curr ) {
    to.Insert( curr->data );
    curr = curr->next;
  }

  if ( to.owns )
    from.owns = false;
}


//------------------------------------------------------------------------------
// \ru Очистить лист с возможным удаленим данных \en Clear the list with data deletion 
// ---
template <class Type>
void remove_from_list( List<Type> & list, DelType shdl )
{
  PRECONDITION( list.nowDelItem == nullptr && list.nowDelElem == nullptr );

  bool del = shdl==Delete || (shdl==defDelete && list.owns);

  ListItem<Type> *first = list.first;
  list.first  = nullptr;
  list.last   = nullptr;
  list.count  = 0;
  while ( first ) {
    ListItem<Type> *temp = first;
    first = first->next;

    if ( del ) {
      list.nowDelElem = temp->data;
      delete temp->data;
      list.nowDelElem = nullptr;
    }

    list.nowDelItem = temp;
    delete temp;
    list.nowDelItem = nullptr;
  }
}


//------------------------------------------------------------------------------
// \ru Очистить лист с возможным удаленим данных \en Clear the list with data deletion 
// ---
template <class Type>
void remove_from_list_release( List<Type> & list )
{
  PRECONDITION( list.nowDelItem == nullptr && list.nowDelElem == nullptr );
  
  ListItem<Type> *first = list.first;
  list.first  = nullptr;
  list.last   = nullptr;
  list.count  = 0;
  while ( first ) {
    ListItem<Type> *temp = first;
    first = first->next;
    
    list.nowDelElem = temp->data;
    if ( temp->data )
      temp->data->Release();
    list.nowDelElem = nullptr;
    
    list.nowDelItem = temp;
    delete temp;
    list.nowDelItem = nullptr;
  }
}


//------------------------------------------------------------------------------
// \ru Отцепить один список от другого с возможным удалением данных \en Detach one list from another with data deletion 
// ---
template <class Type>
size_t remove_from_list( List<Type> & list, List<Type> & deList, DelType shdl )
{
  PRECONDITION( &list != &deList );
  PRECONDITION( list.nowDelItem   == nullptr && list.nowDelElem   == nullptr );
  PRECONDITION( deList.nowDelItem == nullptr && deList.nowDelElem == nullptr );

  if ( !list.first || !deList.first ) // \ru какой-то из списков пуст ! \en one of the lists is empty ! 
    return 0;

  bool willDel = shdl==Delete || (shdl==defDelete && list.owns);

  list.last->next         = list.first;    // \ru закольцуем список \en close the list 
  ListItem<Type> *curr    = list.first;    // \ru начнем сначала \en start from the beginning 
  ListItem<Type> *prev    = list.last;
  ListItem<Type> *del     = deList.first;  // \ru текущий удаляемый \en the current deleted 
  ListItem<Type> *pdel    = nullptr;          // \ru предыдущий удаляемый \en the previous deleted
  size_t deleted = 0;             // \ru отцепленных 0 \en there are 0 detached 

  while( del && list.first ) {             // \ru есть еще пока чего удалять и откуда \en there are elements to delete 
    ListItem<Type> *from  = curr;          // \ru начинаем с текущего \en start from the current one 
    bool            found = false;
    do {
      if ( curr->data == del->data ) {     // \ru поймался !! \en found !! 
        ListItem<Type> *condemned = curr;  // \ru его будем удалять \en it will be deleted 
        found = true;

        if ( list.first == list.last ) { // \ru если всего один элемент, то ничего не останется \en if there is only one element then nothing will be left 
          list.count = 0;
          list.first = nullptr;
          list.last  = nullptr;
          // \ru curr продвигать не нужно - все равно заканчиваем \en 'curr' should not be moved 
        }
        else {
          list.count--;                      // \ru скорректируем счетчик \en correct the counter 
          if ( list.first == curr )          // \ru если вдруг в начале списка - \en if it is in the beginning of the list 
            list.first = curr->next;         // \ru начало сдвинуть \en then move the beginning 

          if ( list.last == curr )           // \ru если вдруг в конце списка - \en if it is in the end of the list 
            list.last = prev;                // \ru конец сдвинуть \en then move the end 

          prev->next = curr->next;           // \ru отцепим найденный \en detach the found one 
          curr = curr->next;                 // \ru продвинемся \en move 
        }

        if ( willDel ) {                     // \ru если надо - удалим данные \en delete the data if it is necessary 
          list.nowDelElem = condemned->data;
          delete condemned->data;
          list.nowDelElem = nullptr;
        }

        list.nowDelItem = condemned;
        delete condemned;                    // \ru удалим квартиру \en delete condemned 
        list.nowDelItem = nullptr;

        deleted++;                           // \ru еще один удалили \en another one has been deleted 
        break;
      }
      else {                            // \ru если не нашли - продвинемся на следующий \en if nothing is found then move to the next 
        prev = curr;
        curr = curr->next;
      }
    } while( curr != from );            // \ru есть где поискать \en there is something to find 


    ListItem<Type> *next = del->next;
    if ( found && willDel ) {           // \ru удаляем квартиру в list'е задающем список удаляемых \en delete a condemned from the list which sets the list of deleted elements 
      if ( pdel )
        pdel->next = next;
      else
        deList.first = next;

      if ( !next )
        deList.last = pdel;

      deList.nowDelItem = del;
      delete del;                       // \ru помним, что данные мы уже удалили \en remember that the data has already been deleted 
      deList.nowDelItem = nullptr;

      deList.count--;
    }
    else
      pdel = del;

    del = next;
  }

  if ( list.last )
    list.last->next = nullptr;                // \ru разорвать список \en split the list

  return deleted;
}


//------------------------------------------------------------------------------
template <class Type>
bool detach_from_list( List<Type> & from, const Type * del )
{
  PRECONDITION( from.nowDelItem == nullptr && from.nowDelElem == nullptr );

  ListItem<Type>* curr = from.first;
  ListItem<Type>* prev = nullptr;

  while( curr ) {
    if ( curr->data == del ) { // \ru нашли \en found 
      ListItem<Type> *next = curr->next;
      if ( prev )
        prev->next = next;
      else
        from.first = next;

      if ( !next )
        from.last = prev;

      from.nowDelItem = curr;
      delete curr;
      from.nowDelItem = nullptr;
      
      from.count--;
      return true;
    }
    else {
      prev = curr;
      curr = curr->next;
    }
  }

  return false;
}


//------------------------------------------------------------------------------
template <class Type>
size_t recalc_list( List<Type> & list )
{
  PRECONDITION( list.nowDelItem == nullptr && list.nowDelElem == nullptr );

  list.count = 0;
  ListItem<Type> *curr = list.first;
  while ( curr ) {
    list.count++;
    curr = curr->next;
  }
  return list.count;
}


//------------------------------------------------------------------------------
template <class Type>
bool is_exist_in_list( const List<Type> & list, const Type * what )
{
  PRECONDITION( list.nowDelItem == nullptr && list.nowDelElem == nullptr );
  bool exist = false;

  ListItem<Type> * curr = list.first;
  while ( curr && !exist ) {
    exist = ( curr->data == what );
    curr = curr->next;
  }

  return exist;
}


//------------------------------------------------------------------------------
template <class Type>
ListItem<Type> * find_prev_in_list( const List<Type> & list, ListItem<Type> * now )
{
  PRECONDITION( list.nowDelItem == nullptr && list.nowDelElem == nullptr );

  if ( now ) {
    ListItem<Type> *tmp = list.first;
    while( tmp && tmp->next != now )
      tmp = tmp->next;
    return tmp;
  }

  return nullptr;
}


#endif // __TEMPL_S_LIST_H
