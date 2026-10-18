//////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Сбалансированное дерево.
         \en Balanced tree. \~

*/
// \ru Никлаус Вирт "Алгоритмы и структуры данных" \en Niklaus Wirth "Algorithms and Data Structures" 
// \ru Построение сбалансированного дерева (АВЛ- дерево). Разновидность бинарного дерева. \en Construction of balanced tree (AVL-tree). There is a sort of binary tree. 
// \ru Из узла выходит не более двух поддеревьев. \en Each node contains at most two child subtrees. 
// \ru Дерево называется сбалансированным, когда высоты двух поддеревьев из его вершин \en Tree is balanced if heights of two child subtrees of any node 
// \ru отличаются не более чем на единицу. \en differ by at most one. 
//
// \ru BalanceTreeNode     - Узел сбалансированного дерева \en BalanceTreeNode     - Node of balanced tree 
// \ru BalanceTree         - Сбалансированное дерево \en BalanceTree         - Balanced tree 
// \ru BalanceTreeIterator - Класс итератора для сбалансированного дерева \en BalanceTreeIterator - Balanced tree iterator class 
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __BALANCETREE_H
#define __BALANCETREE_H

#include <system_types.h>
#include "templ_delete_define.h"
#include "templ_three_states.h"
#include "templ_s_array.h"


template<class Type> class BalanceTree;


//-----------------------------------------------------------------------------
// \ru сравниваем указатели \en compare pointers 
// ---
template <class Type>
ThreeStates  SimplePointCompFuncT( const Type * t1, const Type * t2 ) {
  return size_t(t1) == size_t(t2) ? ts_neutral : size_t(t1) < size_t(t2) ? ts_negative : ts_positive;
}


//-----------------------------------------------------------------------------
// \ru сравниваем указатели \en compare pointers 
// ---
template <class Type>
ThreeStates  SimplePointCompFuncV( const Type * t1, const void * t2 ) {
  return size_t(t1) == size_t(t2) ? ts_neutral : size_t(t1) < size_t(t2) ? ts_negative : ts_positive;
}


FORVARD_DECL_TEMPLATE_TYPENAME( class BalanceTreeNode );
FORVARD_DECL_TEMPLATE_TYPENAME( void   destroy_tree_node ( BalanceTreeNode<Type> &, DelType del ) );

//-----------------------------------------------------------------------------
/** \brief \ru Узел сбалансированного дерева.
           \en Node of balanced tree. \~
  \details \ru Узел сбалансированного дерева. \n
           \en Node of balanced tree. \n \~
  \ingroup Base_Tools
*/
// ---
template<class Type>
class BalanceTreeNode {
public:
  BalanceTree<Type>     & parent_m;        ///< \ru Родитель узла. \en Parent of node. 
  BalanceTreeNode<Type> * left_m;          ///< \ru Левое поддерево. \en Left subtree. 
  BalanceTreeNode<Type> * right_m;         ///< \ru Правое поддерево. \en Right subtree. 
  ThreeStates             balance_m;       ///< \ru Признак сбалансированности. \en Attribute of balance. 
  Type *                  content_m;       ///< \ru Указатель на элемент. \en Pointer to element. 

public:  
  /// \ru Конструктор. \en Constructor. 
           BalanceTreeNode( BalanceTree<Type> & parent, Type * content ); 
  /// \ru Деструктор. \en Destructor. 
  virtual ~BalanceTreeNode();
  /// \ru Установить левую ветвь. \en Set left branch. 
  void     SetLeft ( BalanceTreeNode<Type> * p );
  /// \ru Установить правую ветвь. \en Set right branch. 
  void     SetRight( BalanceTreeNode<Type> * p );

#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
public:
  void * operator new    ( size_t );
  void   operator delete ( void *, size_t );
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_

private:
  TEMPLATE_FRIEND  void   destroy_tree_node TEMPLATE_SUFFIX ( BalanceTreeNode<Type> &, DelType del );
  void operator = ( const BalanceTreeNode & );
};


FORVARD_DECL_TEMPLATE_TYPENAME( class BalanceTree );
FORVARD_DECL_TEMPLATE_TYPENAME( Type * find_tree( const BalanceTree<Type> &, void * content, bool compT ) );

//-----------------------------------------------------------------------------
/** \brief \ru Сбалансированное дерево.
           \en Balanced tree. \~
  \details \ru Сбалансированное дерево. \n
           \en Balanced tree. \n \~
  \ingroup Base_Tools
*/
// ---
template<class Type>
class BalanceTree { 
public:
  BalanceTreeNode<Type> * root_m;          ///< \ru Корень дерева. \en Root of tree. 
  size_t                  allCount_m;      ///< \ru Количество. \en Count. 
  bool                    owns_m;          ///< \ru Можно ли удалять элементы. \en Whether it is possible to delete elements. 
  bool                    isBranchGrew_m;  ///< \ru Флаг роста дерева. \en Flag of growth of a tree. 
  
public:
  typedef ThreeStates  (*Compare_t)( const Type *, const Type * );
  typedef ThreeStates  (*Compare_v)( const Type *, const void * );

  Compare_t compT_m;  ///< \ru Функция сортировки используется при добавлении объекта. \en Sorting function is used while adding an object. 
  Compare_v compV_m;  ///< \ru Функция сортировки используется при поиске объекта. \en Sorting function is used while search an object. 

#ifdef C3D_DEBUG
static size_t countIsSame;   ///< \ru Число сравнений (для отладки) \en Count of comparisons (for debug) 
#endif // C3D_DEBUG

public:
  /// \ru Конструктор. \en Constructor. 
  BalanceTree( Compare_t c_t = SimplePointCompFuncT, Compare_v c_v = nullptr/*SimplePointCompFuncV*/,
               bool shouldDelete = true ); 
  /// \ru Деструктор. \en Destructor. 
  virtual ~BalanceTree();

public:
  bool                  Add      ( Type * );                                 ///< \ru Добавить элемент. \en Add element. 
  size_t                Count    ()  const { return allCount_m; }            ///< \ru Получить количество элементов. \en Set count of elements. 
  void                  Flush    ( DelType = defDelete );                    ///< \ru Удалить все элементы. \en Delete all elements. 
  bool                  Remove   ( Type * delObject, DelType = defDelete );  ///< \ru Удалить элемент из массива. \en Delete an element from array. 
  bool                  FindIt   ( const Type * ) const;                     ///< \ru Найти элемент по указателю. \en Find an element by a pointer 
  Type *                Find     ( void * )       const;                     ///< \ru Найти элемент. \en Find an element. 
  bool                  Detach   ( const Type * );                           ///< \ru Отсоединить объект. \en Detach an object. 

  typedef void (*IterFunc) ( Type &, void *);
  void     ForEach ( IterFunc f, void *pars );           // \ru НЕ классный итератор \en Iterator IS OUT OF a class 

protected:
  /// \ru Добавить объект в дерево. \en Add object to tree. 
  bool     AddToBalanceTree     ( Type & content, BalanceTreeNode<Type> *& node, 
                                  bool & isBranchGrew );
  /// \ru Удалить объект из дерева. \en Delete an object from tree. 
  bool     DeleteFromBalanceTree( Type & content, BalanceTreeNode<Type> *& node, bool & isBranchGrew, DelType del );
  /// \ru Балансировать левую ветвь. \en Balance left branch. 
  void     BalanceL             ( BalanceTreeNode<Type> *& node, bool & isBranchGrew );
  /// \ru Балансировать правую ветвь. \en Balance right branch. 
  void     BalanceR             ( BalanceTreeNode<Type> *& node, bool & isBranchGrew );
  // \ru Исключить из сбалансированного дерева. \en Exclude from balanced tree. 
  bool     DelFromBalanceTree   ( BalanceTreeNode<Type> *& q, BalanceTreeNode<Type> *& r, bool & isBranchGrew );

private:
  void     DoubleTurningRL      ( BalanceTreeNode<Type> *& node, BalanceTreeNode<Type> * p1, 
                                  BalanceTreeNode<Type> *& p2 );
  void     DoubleTurningLR      ( BalanceTreeNode<Type> *& node, BalanceTreeNode<Type> * p1, 
                                  BalanceTreeNode<Type> *& p2 );

#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
public:
  void * operator new    ( size_t );
  void   operator delete ( void *, size_t );
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_

private:
  TEMPLATE_FRIEND Type * find_tree TEMPLATE_SUFFIX ( const BalanceTree<Type> &, void * content, bool compT );
  void   for_each_in_tree(  BalanceTreeNode<Type> &, typename BalanceTree<Type>::IterFunc  f, void *pars ); // \ru неклассный \en out of class 
};


#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
//------------------------------------------------------------------------------
// \ru Перегрузка оператора new. \en Overloading of the "new" operator. 
// ---
template <class Type>
inline void * BalanceTree<Type>::operator new( size_t size ) {
  return ::Allocate( size, typeid(BalanceTree<Type>).name() );
}

//------------------------------------------------------------------------------
// \ru Перегрузка оператора delete. \en Overloading of the "delete" operator. 
// ---
template <class Type>
inline void BalanceTree<Type>::operator delete ( void * ptr, size_t size ) {
  ::Free( ptr, size, typeid(BalanceTree<Type>).name() );
}
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_


//-----------------------------------------------------------------------------
// \ru Узел \en Node 
// ---
template<class Type>
class PPNode {
public:  
  /// \ru Типы движения по дереву. \en Types of moving in the tree. 
  enum PPNodeType { 
    iRoot   = 0,    // \ru текущий this \en current 'this' 
    iLeft   = 1,    // \ru текущий Left \en current 'Left' 
    iRight  = 2,    // \ru текущий Right \en current 'Right' 
    iRemove = 3     // \ru удалять \en remove 
  };

  BalanceTreeNode<Type> * node_m;
  PPNodeType              typeRAB_m;
public:
  PPNode( BalanceTreeNode<Type> * node = nullptr, PPNodeType t = iRoot )
    : node_m(node)
    , typeRAB_m(t)
  {}
  PPNode( PPNode<Type> & other )
    : node_m( other.node_m )
    , typeRAB_m( other.typeRAB_m )
  {}
  void Init( BalanceTreeNode<Type> * node, PPNodeType t  ) { node_m = node; typeRAB_m = t; }
};


//-----------------------------------------------------------------------------
/** \brief \ru Итератор сбалансированного дерева.
           \en Iterator of balanced tree. \~
  \details \ru Итератор сбалансированного дерева. \n
           \en Iterator of balanced tree. \n \~
  \ingroup Base_Tools
*/
// ---
template<class Type>
class BalanceTreeIterator  {
public:
/// \ru Типы движения по дереву. \en Types of moving through the tree. 
enum IteratorType { 
  /// \ru Умолчательный. \en Default. 
  iDeforder  = 0,
  /// \ru Сверху вниз   R(корень), A(слева),  B(справа). \en Top-down   R(root), A(at the left),  B(at the right).  
  iPreorder  = 1,
  /// \ru Слева направо A(слева),  R(корень), B(справа) по возрастанию. \en From left to right A(at the left),  R(root), B(at the right) in ascending order. 
  iInorder   = 2,
  /// \ru Снизу вверх   A(слева),  B(справа), R(корень). \en Bottom-up   A(at the left),  B(at the right), R(root). 
  iPostorder = 3,
  /// \ru Справа налево  B(справа), R(корень), A(слева) по убыванию. \en From right to left  B(at the right), R(root), A(at the left) in descending order. 
  iBackorder = 4 //-V112
};

protected:
  BalanceTree<Type> &     m_tree;        ///< \ru Дерево, по которому движемся. \en Tree to move through. 
  IteratorType            m_iterType;    ///< \ru Тип движения. \en Type of move. 
  SArray< PPNode<Type> >  m_PPNodes;     ///< \ru Последовательный список узлов итератора(имитация рекурсии). \en Sequential list of iterator nodes (imitation of recursion). 
  PPNode<Type>            m_PPNode;      ///< \ru Для наполнения. \en For filling. 

  BalanceTreeNode<Type> * m_CurNode;     ///< \ru Текущий узел. \en Current node. 

public:
  BalanceTreeIterator( BalanceTree<Type> & tree, IteratorType t = iPreorder )
    : m_tree    ( tree     ) 
    , m_iterType( t        )
    , m_PPNodes   ( 0x100, 0x100 )
    , m_PPNode    (  )
  {
    Restart( m_iterType );
  }
  virtual ~BalanceTreeIterator()
  {}
public:
          Type *    operator  ++  (int);
  virtual           operator  Type * ()  const;

  virtual void      Restart( IteratorType t = iDeforder );
  // \ru КВН K8+  Будет работать правильно, если делать Restart. Чтобы работало в цикле, нужно набирать \en КВН K8+  Will correctly work if doing Restart. For working in cycle it is necessary to gather  
  // \ru КВН K8+  удаляемые объекты в список и удалять в конце цикла или по Restart \en КВН K8+  deleted objects to list and remove it at the end of cycle or by Restart 
  // \ru КВН K8+ bool      Remove ( DelType = defDelete );     // удалить элемент списка и продвинуть вперед \en КВН K8+ bool      Remove ( DelType = defDelete );     // remove element from list and move ahead 
  // \ru КВН K8+ bool      Detach ();                          // отсоединить элемент списка \en КВН K8+ bool      Detach ();                          // detach element of list 

protected:
  void                     Iterate       ( BalanceTreeNode<Type> * node ); 
  typename
  PPNode<Type>::PPNodeType GetNodeType   ( bool add, typename PPNode<Type>::PPNodeType oldTypeRAB );

private:
  void operator = ( const BalanceTreeIterator & );
};


//------------------------------------------------------------------------------
//
//---
#ifdef C3D_DEBUG
template<class Type>
size_t BalanceTree<Type>::countIsSame = 0;
#endif // C3D_DEBUG


////////////////////////////////////////////////////////////////////////////////
//
// \ru BalanceTreeNode - узел сбалансированного дерева \en BalanceTreeNode - node of balanced tree 
//
////////////////////////////////////////////////////////////////////////////////


#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
//------------------------------------------------------------------------------
// \ru Перегрузка оператора new. \en Overloading of the "new" operator. 
// ---
template <class Type>
inline void * BalanceTreeNode<Type>::operator new( size_t size ) {
  return ::Allocate( size, typeid(BalanceTreeNode<Type>).name() );
}

//------------------------------------------------------------------------------
// \ru Перегрузка оператора delete. \en Overloading of the "delete" operator. 
// ---
template <class Type>
inline void BalanceTreeNode<Type>::operator delete ( void * ptr, size_t size ) {
  ::Free( ptr, size, typeid(BalanceTreeNode<Type>).name() );
}
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_


//-------------------------------------------------------------------------------
// \ru конструктор узла дерева \en constructor of node of tree 
// ---
template <class Type>
inline  BalanceTreeNode<Type>::BalanceTreeNode( BalanceTree<Type> & parent, Type * content ) :
  parent_m  ( parent ),
  left_m    ( nullptr ),
  right_m   ( nullptr ),
// \ru КВН K8+     count_m   ( 0 ), \en КВН K8+     count_m   ( 0 ), 
  balance_m ( ts_neutral ),
  content_m ( content ) 
{
}


//------------------------------------------------------------------------------
// \ru деструктор узла дерева \en destructor of node of tree 
// ---
template <class Type>
inline BalanceTreeNode<Type>::~BalanceTreeNode()
{
  destroy_tree_node( *this, defDelete );
}

//-----------------------------------------------------------------------------
//
// ---
template <class Type>
inline void BalanceTreeNode<Type>::SetLeft ( BalanceTreeNode<Type> * p ){
  left_m = p;
}

//-----------------------------------------------------------------------------
//
// ---
template <class Type>
inline void BalanceTreeNode<Type>::SetRight( BalanceTreeNode<Type> * p ){
  right_m = p;
}


////////////////////////////////////////////////////////////////////////////////
//
// \ru BalanceTree         - сбалансированное дерево \en BalanceTree         - balanced tree 
//
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------
// \ru конструктор дерева \en constructor of tree 
// ---
template <class Type>
inline  BalanceTree<Type>::BalanceTree( Compare_t c_t, Compare_v c_v, bool shouldDelete ) :
  root_m        ( nullptr ),
  allCount_m    ( 0 ),
  owns_m        ( shouldDelete ),
  isBranchGrew_m( false ),
  compT_m       ( c_t  ),
  compV_m       ( c_v  )
{
  if ( !compV_m && compT_m == (Compare_t) SimplePointCompFuncT )
    compV_m = SimplePointCompFuncV;

#ifdef C3D_DEBUG
  countIsSame = 0;
#endif // C3D_DEBUG
}


//------------------------------------------------------------------------------
// \ru деструктор дерева \en destructor of tree 
// ---
template <class Type>
inline BalanceTree<Type>::~BalanceTree() {
  delete root_m;
}


//------------------------------------------------------------------------------
// \ru добавить 1 элемент в конец массива \en add 1 element at the end of array 
// ---
template <class Type>
inline bool BalanceTree<Type>::Add( Type * ent ) {

#ifdef C3D_DEBUG
  countIsSame = 0;
#endif // C3D_DEBUG

  bool res = false;
  if ( ent && compT_m ) {
    res = AddToBalanceTree( *ent, root_m, isBranchGrew_m );
  }
  return res;
}


//------------------------------------------------------------------------------
// \ru обнулить количество элементов \en set to null the count of elements 
// ---
template <class Type>
inline void BalanceTree<Type>::Flush( DelType del ) {
  if( root_m ) {
    destroy_tree_node( *root_m, del );
    delete root_m;
    root_m         = nullptr;
  }

  allCount_m     = 0;
  isBranchGrew_m = false;
}


//-----------------------------------------------------------------------------
// \ru удалить элемент из массива \en delete an element from array 
// ---
template <class Type>
inline bool BalanceTree<Type>::Remove( Type * delObject, DelType del ) { 

#ifdef C3D_DEBUG
  countIsSame = 0;
#endif // C3D_DEBUG

  bool res = false;
  if ( delObject && compT_m ) 
    res = DeleteFromBalanceTree( *delObject, root_m, isBranchGrew_m, del );

  return res;
}


//-----------------------------------------------------------------------------
// \ru отсоединить объект \en detach an object  
// ---
template <class Type>
inline bool BalanceTree<Type>::Detach( const Type *d ){
  return Remove( (Type*)d, noDelete );
}


//-----------------------------------------------------------------------------
// \ru найти элемент по указателю \en find an element by a pointer 
// ---
template <class Type>
inline bool BalanceTree<Type>::FindIt ( const Type *  content ) const {

#ifdef C3D_DEBUG
  countIsSame = 0;
#endif // C3D_DEBUG

 
  Type * t = compT_m ? find_tree( *this, (void *)content, true/*compT*/ ) : nullptr;
  return !!t;
}


//-----------------------------------------------------------------------------
// \ru найти элемент \en find an element  
// ---
template <class Type>
inline Type * BalanceTree<Type>::Find ( void * content ) const {

#ifdef C3D_DEBUG
  countIsSame = 0;
#endif // C3D_DEBUG

  return  compV_m ? find_tree( *this, content, false/*compT*/ ) : nullptr;
}


//------------------------------------------------------------------------------
// \ru не классный \en out of class 
// ---
template <class Type>
inline void BalanceTree<Type>::for_each_in_tree( BalanceTreeNode<Type>& node, typename BalanceTree<Type>::IterFunc f, void *pars ) {
  if ( node.left_m )
    for_each_in_tree( *node.left_m, f, pars );
  
  PRECONDITION( f );
  (*f)( *node.content_m, pars );
  
  if ( node.right_m )
    for_each_in_tree( *node.right_m, f, pars );
}


//------------------------------------------------------------------------------
// \ru не классный \en out of class 
// ---
template <class Type>
inline void BalanceTree<Type>::ForEach( IterFunc f, void *pars ) {
  if ( root_m && f )
    for_each_in_tree( *root_m, f, pars );
}


//-----------------------------------------------------------------------------
// \ru двойной RL-поворот \en double RL-rotation 
// ---
template <class Type>
inline  void BalanceTree<Type>::DoubleTurningRL ( BalanceTreeNode<Type> *& node, 
                                                  BalanceTreeNode<Type> * p1, 
                                                  BalanceTreeNode<Type> *& p2 ) {
  p2 = p1->left_m;
  if ( p2 ) {
    p1  ->SetLeft( p2->right_m );
    p2  ->SetRight( p1 );
    node->SetRight( p2->left_m );
    p2  ->SetLeft( node );
    node->balance_m = ( p2->balance_m == ts_positive ) ? ts_negative : ts_neutral;
    p1->balance_m   = ( p2->balance_m == ts_negative ) ? ts_positive : ts_neutral;
    node = p2;
  }
}


//-----------------------------------------------------------------------------
// \ru двойной LR-поворот \en double LR-rotation 
// ---
template <class Type>
inline  void BalanceTree<Type>::DoubleTurningLR ( BalanceTreeNode<Type> *& node, 
                                                  BalanceTreeNode<Type> * p1, 
                                                  BalanceTreeNode<Type> *& p2 ) {
  p2 = p1->right_m;
  if ( p2 ) {
    p1  ->SetRight( p2->left_m );
    p2  ->SetLeft( p1 );
    node->SetLeft( p2->right_m );
    p2  ->SetRight( node );
    node->balance_m = ( p2->balance_m == ts_negative ) ? ts_positive : ts_neutral;
    p1->balance_m   = ( p2->balance_m == ts_positive ) ? ts_negative : ts_neutral;
    node = p2;
  }
}


//-----------------------------------------------------------------------------
// \ru Построение идеально сбалансированного дерева \en Creation of ideally balanced tree 
// ---
template <class Type>
inline  bool BalanceTree<Type>::AddToBalanceTree( Type & content, BalanceTreeNode<Type> *& node,  
                                                  bool & isBranchGrew ) {
  bool isAdd = false;
  if ( !node ) {
    node  = new BalanceTreeNode<Type>( *this, &content );
    isAdd = true;
    isBranchGrew = true;
    allCount_m++; // \ru подсчитываем общее кол-во узлов \en compute the total count of nodes 
  }
  else {
    ThreeStates compRres = compT_m( (Type*)node->content_m, (Type*)&content );

#ifdef C3D_DEBUG
    countIsSame++;
#endif // C3D_DEBUG

    if ( compRres != ts_neutral ) {
      if ( compRres == ts_positive ) {
        isAdd = AddToBalanceTree( content, node->left_m, isBranchGrew );
        // \ru выросла левая ветвь \en left branch is grew up. 
        if ( isAdd && isBranchGrew ) {
          switch ( node->balance_m ) {
            case ts_positive : node->balance_m = ts_neutral;  isBranchGrew = false; break;
            case ts_neutral  : node->balance_m = ts_negative;                       break;
            // \ru балансировка \en balancing 
            case ts_negative : {
              BalanceTreeNode<Type> * p1 = nullptr;
              BalanceTreeNode<Type> * p2 = nullptr;
              p1 = node->left_m;
              if ( p1 ){
                if ( p1->balance_m == ts_negative ) {  // \ru однократный LL поворот \en single LL rotation 
                  node->SetLeft( p1->right_m );

                  p1->SetRight( node );

                  node->balance_m = ts_neutral;
                  node = p1;

                }
                // \ru двойной LR-поворот \en double LR-rotation 
                else {
                  DoubleTurningLR( node, p1, p2 );
                }
              }

              node->balance_m = ts_neutral; 
              isBranchGrew = false;
            } break;
          }
        }
      }
      else {
        isAdd = AddToBalanceTree( content, node->right_m, isBranchGrew );
        if ( isAdd && isBranchGrew ) {
          switch ( node->balance_m ) {
            case ts_negative : node->balance_m = ts_neutral; isBranchGrew = false; break;
            case ts_neutral  : node->balance_m = ts_positive;                      break;
            // \ru балансировка \en balancing 
            case ts_positive : {
              BalanceTreeNode<Type> * p1 = nullptr;
              BalanceTreeNode<Type> * p2 = nullptr;
              p1 = node->right_m;
              if ( p1 ) {
                if ( p1->balance_m == ts_positive ) { // \ru однократный RR поворот \en single RR rotation 
                  node->SetRight( p1->left_m );

                  p1->SetLeft( node );

                  node->balance_m = ts_neutral;
                  node = p1;
                }
                else { // \ru двойной RL-поворот \en double RL-rotation 
                  DoubleTurningRL( node, p1, p2 );
                }
              }

              node->balance_m = ts_neutral; 
              isBranchGrew = false;
            } break;
          }
        }
      }
    }
  }

  return isAdd;
}


//-----------------------------------------------------------------------------
// \ru Балансировка слева \en Balancing at the left 
// ---
template <class Type>
inline  void BalanceTree<Type>::BalanceL( BalanceTreeNode<Type> *& node, bool & isBranchGrew ) {
  switch ( node->balance_m ) {
    case ts_negative : node->balance_m = ts_neutral;                          break;
    case ts_neutral  : node->balance_m = ts_positive;  isBranchGrew = false;  break;
    // \ru балансировка \en balancing 
    case ts_positive : {
      BalanceTreeNode<Type> * p1 = nullptr;
      BalanceTreeNode<Type> * p2 = nullptr;
      p1 = node->right_m;
      if ( p1 ) {
        if ( p1->balance_m >= ts_neutral ) {  // \ru однократный RR поворот \en single RR rotation 
          node->SetRight( p1->left_m );
          p1->SetLeft( node );
          if ( p1->balance_m == ts_neutral ) {
            node->balance_m = ts_positive;
            p1->balance_m   = ts_negative;
            isBranchGrew = false;
          }
          else {
            node->balance_m = ts_neutral;
            p1->balance_m   = ts_neutral;
          }
          node = p1;
        }
        else { // \ru двойной RL-поворот \en double RL-rotation 
          DoubleTurningRL( node, p1, p2 );
          if( p2 )
            p2->balance_m = ts_neutral;
        }
      }
    } break;
  }
}


//-----------------------------------------------------------------------------
// \ru Балансировка справа \en Balancing at the right 
// ---
template <class Type>
inline  void BalanceTree<Type>::BalanceR( BalanceTreeNode<Type> *& node, bool & isBranchGrew ) {
  switch ( node->balance_m ) {
    case ts_positive : node->balance_m = ts_neutral;                          break;
    case ts_neutral  : node->balance_m = ts_negative;  isBranchGrew = false;  break;
    // \ru балансировка \en balancing 
    case ts_negative : {
      BalanceTreeNode<Type> * p1 = nullptr;
      BalanceTreeNode<Type> * p2 = nullptr;
      p1 = node->left_m;
      if ( p1 ) {
        if ( p1->balance_m <= ts_neutral ) {  // \ru однократный LL поворот \en single LL rotation 
          node->SetLeft( p1->right_m );
          p1->SetRight( node );
          if ( p1->balance_m == ts_neutral ) {
            node->balance_m = ts_negative;
            p1->balance_m   = ts_positive;
            isBranchGrew = false;
          }
          else {
            node->balance_m = ts_neutral;
            p1->balance_m   = ts_neutral;
          }
          node = p1;
        }
        else { // \ru двойной LR-поворот \en double LR-rotation 
          DoubleTurningLR( node, p1, p2 );
          if( p2 )
            p2->balance_m = ts_neutral;
        }
      }
    } break;
  }
}


//-----------------------------------------------------------------------------
// \ru Исключение из сбалансированного дерева \en Exclude from balanced tree 
// ---
template <class Type>
inline  bool BalanceTree<Type>::DelFromBalanceTree( BalanceTreeNode<Type> *& q, BalanceTreeNode<Type> *& r, bool & isBranchGrew ) {
  bool res = false;
  if ( r->right_m ) {
    res = DelFromBalanceTree( q, r->right_m, isBranchGrew );
    if ( res && isBranchGrew )
      BalanceR( r, isBranchGrew );
  }
  else {
    Type * content = q->content_m;
    q->content_m = r->content_m;
    r->content_m = content;
    // \ru КВН K8+ q->count_m   = r->count_m; \en КВН K8+ q->count_m   = r->count_m; 
    q = r;
    r = r->left_m;
    isBranchGrew = true;
  }
  return res;
}


//-----------------------------------------------------------------------------
// \ru Исключение из сбалансированного дерева \en Exclude from balanced tree 
// ---
template <class Type>
inline  bool BalanceTree<Type>::DeleteFromBalanceTree( Type & content, BalanceTreeNode<Type> *& node, 
                                                       bool & isBranchGrew, DelType del ) {
  bool res = false;
  if ( node && node->content_m ) {
    ThreeStates compRres = compT_m( (Type*)node->content_m, (Type*)&content );

#ifdef C3D_DEBUG
    countIsSame++;
#endif // C3D_DEBUG

    switch ( compRres ) {
      case ts_positive : {
        res = DeleteFromBalanceTree( content, node->left_m, isBranchGrew, del );
        if ( res && isBranchGrew ) 
          BalanceL( node, isBranchGrew );
        break;
      }
      case ts_negative : {
        res = DeleteFromBalanceTree( content, node->right_m, isBranchGrew, del );
        if ( res && isBranchGrew ) 
          BalanceR( node, isBranchGrew );
        break;
      }
      case ts_neutral : {  // \ru исключение node \en exclude node 
        BalanceTreeNode<Type> * q = node;
        if ( !q->right_m ) {
          node = q->left_m;
          isBranchGrew = true;
        }
        else if ( !q->left_m ) {
          node = q->right_m;
          isBranchGrew = true;
        }
        else {
          // \ru удаляемый узел имеет двух потомков. В этом случае нужно "спуститься" вдоль правой ветви \en deleted node has two descendants. In this case it is necessary to "go down" through right branch 
          // \ru левого поддерева, найти самый правый узел-лист. Перенести информацию из него в node, \en of left subtree and find most right leaf node. Transfer information from it to node,  
          // \ru информацию из node в этот лист( content ). Переуказать q на этот лист, чтобы удалить узел-лист. \en transfer information from node to this leaf ( content ). Re-point q to this leaf to delete the leaf node. 
          res = DelFromBalanceTree( q, q->left_m, isBranchGrew );
          if ( res && isBranchGrew )
            BalanceL( node, isBranchGrew );
        }
        bool oldowns = owns_m;
        owns_m = del == Delete ? true : del == noDelete ? false : owns_m;
        q->left_m = nullptr;
        q->right_m = nullptr;
        delete q;
        owns_m = oldowns;
        allCount_m--; // \ru подсчитываем общее кол-во узлов \en compute the total count of nodes 
        res = true;
        break;
      }

    }
  }
  return res;
}


//-------------------------------------------------------------------------------
// \ru удаление всех указателей, собранных в поддереве \en delete all pointers collected in subtree 
// ---
template <class Type>
void destroy_tree_node( BalanceTreeNode<Type>& treeNode, DelType del ) {

  delete treeNode.left_m;
  treeNode.left_m = nullptr;
  delete treeNode.right_m;
  treeNode.right_m = nullptr;
  
  bool shouldDelete = del == Delete || ( del == defDelete && treeNode.parent_m.owns_m );
  if ( shouldDelete ) {
    delete treeNode.content_m;
    treeNode.content_m = nullptr;
  }
}


//-----------------------------------------------------------------------------
// \ru найти элемент по указателю \en find an element by a pointer 
// ---
template <class Type>
Type * find_tree( const BalanceTree<Type>& tree, void * content, bool compT ) {
  Type * res = nullptr;
  const BalanceTreeNode<Type> * node = tree.root_m;
  if ( node ) {
    ThreeStates compRres = ts_negative;
    while ( node && compRres != ts_neutral ) { 
      if ( compT )
        compRres = tree.compT_m( (Type*)node->content_m, (Type*)content );
      else 
        compRres = tree.compV_m( (Type*)node->content_m, content );

#ifdef C3D_DEBUG
      BalanceTree<Type>::countIsSame++;
#endif // C3D_DEBUG

      switch ( compRres ) {
        case ts_negative : node = node->right_m;  break;
        case ts_positive : node = node->left_m;  break;
        case ts_neutral  : res = node->content_m; break;
      }
    }
  }
  return res;
}


////////////////////////////////////////////////////////////////////////////////
//
// \ru BalanceTreeIterator - класс итератора для сбалансированного дерева \en BalanceTreeIterator - balanced tree iterator class 
//
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//
// ---
template <class Type>
inline void  BalanceTreeIterator<Type>::Restart( IteratorType t ) {
  if ( t )
    m_iterType = t;

  m_PPNodes.Flush();
  m_CurNode = m_tree.root_m; 
  if ( m_CurNode ) 
    Iterate( m_CurNode );
}


//-----------------------------------------------------------------------------
// \ru взять и сместиться \en displace 
// ---
template <class Type>
inline Type * BalanceTreeIterator<Type>::operator ++(int) {
  Type * res = m_CurNode ? m_CurNode->content_m : nullptr;
  if ( m_CurNode ) 
    Iterate( m_CurNode );
  
  return res;
}


//-----------------------------------------------------------------------------
// \ru приведение \en conversion 
// ---
template <class Type>
inline BalanceTreeIterator<Type>::operator Type * () const {
  return  m_CurNode ? m_CurNode->content_m : nullptr;
}


//-----------------------------------------------------------------------------
// \ru сверху вниз     R(корень), A(слева),  B(справа) \en top-down     R(root), A(at the left),  B(at the right)  
// \ru слева на право  A(слева),  R(корень), B(справа)  по возрастанию \en from left to right  A(at the left),  R(root), B(at the right) in ascending order 
// \ru снизу вверх     A(слева),  B(справа), R(корень) \en bottom-up     A(at the left),  B(at the right), R(root) 
// \ru справа на лево  B(справа), R(корень), A(слева)   по убыванию \en from right to left  B(at the right), R(root), A(at the left)   in descending order 
// ---
template <class Type>
inline typename PPNode<Type>::PPNodeType BalanceTreeIterator<Type>::GetNodeType( bool add, typename PPNode<Type>::PPNodeType oldTypeRAB ) {
  typename PPNode<Type>::PPNodeType res = PPNode<Type>::iRoot;
  if ( add ) {  // \ru при добавлении в список \en at addition to list 
    switch ( m_iterType ) {
      case iPreorder  : res = PPNode<Type>::iRoot;  break; // \ru сверху вниз     R(корень), A(слева),  B(справа) \en top-down     R(root), A(at the left),  B(at the right)  
      case iPostorder :                                    // \ru снизу вверх     A(слева),  B(справа), R(корень) \en bottom-up     A(at the left),  B(at the right), R(root) 
      case iInorder   : res = PPNode<Type>::iLeft;  break; // \ru слева на право  A(слева),  R(корень), B(справа)  по возрастанию \en from left to right  A(at the left),  R(root), B(at the right) in ascending order 
      case iBackorder : res = PPNode<Type>::iRight; break; // \ru справа на лево  B(справа), R(корень), A(слева)   по убыванию \en from right to left  B(at the right), R(root), A(at the left)   in descending order 
      default: break;
    }
  }
  else {
    switch ( m_iterType ) {
      case iPreorder  : {  // \ru сверху вниз     R(корень), A(слева),  B(справа) \en top-down     R(root), A(at the left),  B(at the right)  
        switch ( oldTypeRAB ) {  
          case PPNode<Type>::iRoot : res = PPNode<Type>::iLeft;   break; 
          case PPNode<Type>::iLeft : res = PPNode<Type>::iRight;  break; 
          case PPNode<Type>::iRight: res = PPNode<Type>::iRemove; break; 
          default: break;
        }
        break; 
      }
      case iPostorder : { // \ru снизу вверх     A(слева),  B(справа), R(корень) \en bottom-up     A(at the left),  B(at the right), R(root) 
        switch ( oldTypeRAB ) {  
          case PPNode<Type>::iLeft : res = PPNode<Type>::iRight;  break; 
          case PPNode<Type>::iRight: res = PPNode<Type>::iRoot;   break; 
          case PPNode<Type>::iRoot : res = PPNode<Type>::iRemove; break; 
          default: break;
        }
        break; 
      }
      case iInorder   : { // \ru слева на право  A(слева),  R(корень), B(справа)  по возрастанию \en from left to right  A(at the left),  R(root), B(at the right) in ascending order 
        switch ( oldTypeRAB ) {  
          case PPNode<Type>::iLeft : res = PPNode<Type>::iRoot;   break; 
          case PPNode<Type>::iRoot : res = PPNode<Type>::iRight;  break; 
          case PPNode<Type>::iRight: res = PPNode<Type>::iRemove; break; 
          default: break;
        }
        break; 
      }
      case iBackorder : { // \ru справа на лево  B(справа), R(корень), A(слева)   по убыванию \en from right to left  B(at the right), R(root), A(at the left)   in descending order 
        switch ( oldTypeRAB ) {  
          case PPNode<Type>::iRight: res = PPNode<Type>::iRoot;   break; 
          case PPNode<Type>::iRoot : res = PPNode<Type>::iLeft;   break; 
          case PPNode<Type>::iLeft : res = PPNode<Type>::iRemove; break; 
          default: break;
        }
        break; 
      }
      default: break;
    }
  }

  return res;
}


//-----------------------------------------------------------------------------
// \ru Продвинуться. \en Move ahead.  
// \ru сверху вниз     R(корень), A(слева),  B(справа) \en top-down     R(root), A(at the left),  B(at the right)  
// \ru слева на право  A(слева),  R(корень), B(справа)  по возрастанию \en from left to right  A(at the left),  R(root), B(at the right) in ascending order 
// \ru снизу вверх     A(слева),  B(справа), R(корень) \en bottom-up     A(at the left),  B(at the right), R(root) 
// \ru справа на лево  B(справа), R(корень), A(слева)   по убыванию \en from right to left  B(at the right), R(root), A(at the left)   in descending order 
// ---
template <class Type>
inline void BalanceTreeIterator<Type>::Iterate( BalanceTreeNode<Type> * node ) {
  if ( node ) {
    ptrdiff_t /*OV_x64 int*/index = m_PPNodes.MaxIndex();
    
    if ( index < 0 || m_PPNodes[index].node_m != node ) {
      m_PPNode.Init(  node, GetNodeType(true, PPNode<Type>::iRoot) );
      m_PPNodes.Add( m_PPNode );

      index = m_PPNodes.MaxIndex();
    }

    bool fRepeat = true;
    while ( fRepeat ) {
      switch ( m_PPNodes[index].typeRAB_m ) {
        case  PPNode<Type>::iRoot : {
          m_CurNode = node;
          m_PPNodes[index].typeRAB_m = GetNodeType( false/*add*/, m_PPNodes[index].typeRAB_m );
          fRepeat = false;
          break;
        }
        case  PPNode<Type>::iLeft : {
          if ( node->left_m ) {
            Iterate( node->left_m );
            fRepeat = false;
          }
          m_PPNodes[index].typeRAB_m = GetNodeType( false/*add*/, m_PPNodes[index].typeRAB_m );
          break;
        }
        case  PPNode<Type>::iRight : {
          if ( node->right_m ) {
            Iterate( node->right_m );
            fRepeat = false;
          }
          m_PPNodes[index].typeRAB_m = GetNodeType( false/*add*/, m_PPNodes[index].typeRAB_m );
          break;
        }
        case  PPNode<Type>::iRemove : {
          index = m_PPNodes.MaxIndex();
          if ( index >= 0 ) {
            m_PPNodes.RemoveInd( index-- );
            if ( index >= 0 ) {
              m_CurNode = m_PPNodes[index].node_m;
              if ( m_CurNode )
                Iterate( m_CurNode );
            }
            else
              m_CurNode = nullptr;
          }
          else
            m_CurNode = nullptr;
          fRepeat = false;
          break;
        }
      }
    }
  }
}

#endif // __BALANCETREE_H

