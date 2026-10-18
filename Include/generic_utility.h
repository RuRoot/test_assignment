//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file 
  \brief \ru Шаблонные утилиты. 
         \en Template utilities. \~
*/
/////////////////////////////////////////////////////////////////////////////// C3D API///

#ifndef __GENERIC_UTILITY_H
#define __GENERIC_UTILITY_H

#include <memory.h>
//
#include <templ_sptr.h>
//
#include <assert.h>
#include <mb_variables.h>

#include <new>
#include <utility>
#include <functional>
#include <vector>

//----------------------------------------------------------------------------------------
/// \ru Пустой тип данных. \en Empty data type.
//---
struct null_type 
{
  static const null_type value() { return null_type(); }
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Шаблон для получения индексного типа (для статического сопоставления типов на этапе компиляции)
            \en Template to generate an indexed type (for static type-matching in compile-time)           
*/
//---
template<size_t>
struct index_tag
{
  index_tag() {} // Constructor under GCC compiler
};

//----------------------------------------------------------------------------------------
/// \ru Цветовая маркировка (применяется для графов) \en Color marking (used for graphs) 
//---
enum color_code: char
{
    white_color = 0
  , gray_color  = 1
  , green_color = 2
  , black_color = 3    
  , red_color  
  , orange_color 
  , visited_color 
};

//----------------------------------------------------------------------------------------
// Constant valued function
//---
template<bool boolVal>
bool boolFunc() { return boolVal; }

//----------------------------------------------------------------------------------------
/// \ru Графовые характеристики типов. \en Graph datatype traits.
//---
template<class Graph>
struct graph_traits
{
  /*
    Ассоциативные типы данных концепции графа.
    Associative datatypes of the graph concept.
  */
  using vertex             = typename  Graph::vertex            ;  // Тип, интерпретируемый, как вершина графа.
  using edge               = typename  Graph::edge              ;  // Тип, интерпретируемый, как ребро графа.
  using vertex_iterator    = typename  Graph::vertex_iterator   ;  // Обход всех вершин графа.
  using adjacency_iterator = typename  Graph::adjacency_iterator;  // Обход смежных вершин некоторой вершины.
  using vertices_size_t    = typename  Graph::vertices_size_t   ;  // Целочисленный тип размера графа.
  using degree_size_t      = typename  Graph::degree_size_t     ;  // Целочисленный тип вершинной степени.
  using edge_iterator      = typename  Graph::edge_iterator     ;  // Итератор обхода исходящих ребер [или неориентированных ребер].
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Подходящий тип ассоциативного контейнера для хранения данных, ассоциированных с вершинами графа.
            \en A suitable type of associative container for storing data associated with graph vertices. \~.
*/
//---
template<class Graph, class Property>
struct property_map
{
private:
  using type = struct{};
};

//----------------------------------------------------------------------------------------
/// \ru Наибольшее из двух. \en Maximum of two.
// ---
template<class T>
inline const T & max_of( const T & elem1, const T & elem2 ) 
{
  if ( elem2 < elem1 )
    return elem1;
  return elem2;
}

//----------------------------------------------------------------------------------------
/// \ru Наибольшее из двух. \en Maximum of two.
// ---
template<class T>
inline const T & min_of( const T & elem1, const T & elem2 ) 
{
  if ( elem1 < elem2 )
    return elem1;
  return elem2;
}

//----------------------------------------------------------------------------------------
/// \ru Поменять местами значения. \en Swap the values.
// ---
template<class T>
inline void swap_vals( T & elem1, T & elem2 )
{  
  T tmp = elem1;
  elem1 = elem2;
  elem2 = tmp;
}

//----------------------------------------------------------------------------------------
/// \ru Поменять местами значения указателей. \en Swap the values of pointers.
// ---
template<class T>
inline void swap_ptrs( T* & elem1, T* & elem2 ) 
{  
  T * tmp = elem1;
  elem1 = elem2;
  elem2 = tmp;
}

//----------------------------------------------------------------------------------------
/// \ru Поменять местами значения указателей. \en Swap the values of pointers.
//---
template<class Type>
inline void swap_ptrs( SPtr<Type> & p1, SPtr<Type> & p2 )
{
  SPtr<Type> t = p1;
  p1 = p2;
  p2 = t;
}

//----------------------------------------------------------------------------------------
// \ru Равенство пары указателей \en Equality of pointer pair 
//---
template< class Type1, class Type2 >
bool equal_ptrs( const Type1 * ptr1, const Type2 * ptr2 )
{
  return static_cast<const Type2*>(ptr1) == ptr2;
}

//----------------------------------------------------------------------------------------
// \ru Равенство пары указателей \en Equality of pointer pair 
//---
template< class Type1, class Type2 >
bool equal_ptrs( SPtr<Type1> ptr1, const Type2 * ptr2 )
{
  return static_cast<const Type2*>(ptr1.get()) == ptr2;
}

//----------------------------------------------------------------------------------------
// \ru Равенство пары двухмерных векторов или точек \en Equality of 2D points or vectors
//---
template< class XY1, class XY2 >
bool equal_xy( const XY1 & v1, const XY2 & v2, double eps )
{
  if ( fabs(v1.x-v2.x) > eps )
    return false;
  if ( fabs(v1.y-v2.y) > eps )
    return false;
  return true;
}

//----------------------------------------------------------------------------------------
// \ru Наименьший общий делитель \en The lowest common denominator 
// ---
template < typename Integer >
Integer euclid_algo ( Integer a, Integer b )
{
  Integer const zero = static_cast<Integer>( 0 );

  bool goOn = true;
  while ( goOn )
  {
    if ( a == zero ) {
      goOn = false;
      return b;
    }

    b %= a;

    if ( b == zero ) {
      goOn = false;
      return a;
    }

    a %= b;
  }
  return zero;
}

//----------------------------------------------------------------------------------------
/// \ru Получить НОД для пары целых чисел \en Get GCD for a pair of integers 
// ---
template < typename IntegerType >
inline IntegerType gcd( IntegerType a, IntegerType b )
{
  IntegerType const zero = static_cast<IntegerType>( 0 );
  IntegerType const result = ::euclid_algo( a, b );
  return ( result < zero ) ? -result : result;
}


//----------------------------------------------------------------------------------------
// \ru Функциональный объект - коллектор \en The functional object - collector 
/*\ru Играет роль посетителя foreach-алгоритмов, осуществляющий накачку STL-совместимых контейнеров
  \en Serves as a visitor of foreach-algorithms exercising pumping of STL-compatible containers \~
*/
//---
template<class _Cont>
struct collector
{
  typedef typename _Cont::value_type value_type;  
  _Cont & container;  // \ru STL-совместимый контейнер \en STL-compatible container 
  
  collector( _Cont & arr )
    : container( arr ) {}
  collector( const collector & c ) : container( c.container ) {}
  void operator () ( const value_type & elem ) const
  {
    container.push_back( elem );
  }

private: // \ru не реализовано \en not implemented   
  collector & operator = ( const collector & );
};


//----------------------------------------------------------------------------------------
/** \brief \ru Статический вектор. 
           \en Static vector.
    \note  \ru Требуется, что бы элементы вектора имели конструктор по умолчанию,
               конструктор копирования и оператор присвоения.
           \en Required that the vector elements have a default constructor, 
               copy constructor and assignment operator. \~
*/
//---
template<class Elem, size_t arrSize>
class static_array
{
public:
  typedef Elem value_type;  // \ru ассоциативный тип элемента массива \en associative type of array element 

private:  
  value_type arr[arrSize]; // \ru статическое выделение памяти под массив \en static allocation for the array 

public:
  /// \ru Инициализация одним элементом. \en Initialization of one element.
  explicit static_array( const Elem & val )
  {    
    fill( val );
  }
  /// \ru Инициализация парой элементов. \en Initialization of a pair of elements.
  static_array( const Elem & e1, const Elem & e2 )  
  {    
    PRECONDITION( arrSize == 2 );
    arr[0] = e1;
    arr[1] = e2;
  }
  /// \ru Конструктор по тройке. \en Constructs as a triplet.
  static_array( const Elem & e1, const Elem & e2, const Elem & e3 )  
  {    
    PRECONDITION( arrSize == 3 );
    arr[0] = e1;
    arr[1] = e2;
    arr[2] = e3;
  }
  explicit static_array( const static_array & vec )
  {
    _Assign( vec );
  }

  template<class _Vector>
  static_array( const _Vector & vec )
  {
    _Assign( vec );
  }

  /// \ru Инициализация одним элементом. \en Initialization of one element.
  static_array & fill( const Elem & val )
  {    
    for( size_t idx = 0; idx<arrSize; ++idx )
    {
      arr[idx] = val;      
    }    
    return *this;
  }

  template<class _Iter>
  static_array & assign( _Iter iter, _Iter last )
  {
    for ( value_type * myIter = arr ; iter!=last; ++iter, ++myIter )
    {
      PRECONDITION( myIter < arr+arrSize );
      *myIter = *iter;
    }
    return *this;
  }

  inline value_type & operator[] ( size_t idx )
  {
    PRECONDITION( idx < arrSize );
    return arr[idx];
  }
  inline const value_type & operator[] ( size_t idx ) const
  {
    PRECONDITION( idx < arrSize );
    return arr[idx];
  }
  template<class _Vector>
  static_array & operator = ( const _Vector & vec )
  {
    _Assign( vec );
    return *this;
  }

  /// \ru Равенство. \en Equality.
  template<class _Vector>
  bool operator == ( const _Vector & vec ) const
  { 
    if ( arrSize != vec.size() )
    {
      return false;
    }
    for( size_t idx = 0; idx<arrSize; ++idx )
    {
      if ( arr[idx] != vec[idx] )
      {
        return false;
      }
    }    
    return true;
  }

  inline const Elem *       c_arr() const { return arr; }
  inline       Elem *       c_arr() { return arr; }
  inline size_t             size() const { return arrSize; }
  inline value_type &       front() { return *arr; }
  inline value_type &       back() { PRECONDITION(arrSize>0); return arr[arrSize-1]; }
  inline const value_type & front() const { return *arr; }
  inline const value_type & back() const { PRECONDITION(arrSize>0); return arr[arrSize-1]; }

private:
  template< class _Vector >
  void _Assign( const _Vector & vec )
  {
    PRECONDITION( vec.size() == size() );    
    for ( size_t idx = ::min_of( arrSize, vec.size() ); idx > 0; )
    {
      idx--;
      arr[idx] = vec[idx];
    }
  }  
};

// \ru (!) Запретить пустые статические массивы \en (!) Prevent empty static arrays 
template<class Elem>  class static_array<Elem,0> {};

//----------------------------------------------------------------------------------------
/// \ru Статический вектор двух элементов (пара). \en Static vector of two elements (pair).
//---
template<class Elem>
struct static_pair: public static_array<Elem,2>
{
  typedef static_array<Elem,2> parent_type;
            // static_pair(): parent_type() {}
  explicit  static_pair( const Elem & el ): parent_type( el ) {}
            static_pair( const Elem & el1, const Elem & el2 ): parent_type( el1, el2 ) {}
  explicit  static_pair( const static_pair & pair ) : parent_type( pair ) {}

  static_pair & operator = ( const static_pair & vec )
  {
    parent_type::operator=( vec );
    return *this;
  }            
};

//----------------------------------------------------------------------------------------
/// \ru Обнулить структуру данных (использовать осторожно!). \en Reset the data structure (use with caution!).
//---
template< class DataSt >
inline DataSt null_struct()
{
  DataSt data;
  ::memset( &data, 0, sizeof(DataSt) );
  return data;
}

//----------------------------------------------------------------------------------------
/// \ru Отладочный инспектор union-контейнера (НЕдоделан!). \en Debug Inspector of union-container (NOT completed yet!).
//---
template<class _PairUnion>
struct dbg_inspector
{
  union data_t
  {
    typename _PairUnion::value_type * first;
  };
  data_t  data;
  dbg_inspector()  { data.first = 0; }
  void init( const _PairUnion & ) {}
};

//----------------------------------------------------------------------------------------
/// \ru Хвостовой элемент для рекурсивного определения типа recursive_union<T1,T2>. \en Tail element for recursive determination of type recursive_union<T1,T2>.
//---
struct empty_variant
{
  static const size_t dataSize = 0;
  static const size_t power = 0;
  struct value_type {};
  bool empty() { return true; }
};

//----------------------------------------------------------------------------------------
/// \ru Получить номер типа из списка union-контейнера. \en Get type index from the list of union-container.
//---
template<class _PairUnion, class _Type>
struct which_type
{
  static const int value = 1 + which_type<typename _PairUnion::tail_type,_Type>::value;
};

// \ru Специализация 1 \en Specialization 1 
template<class _PairUnion>
struct which_type<_PairUnion,typename _PairUnion::value_type>
{
  static const int value = 0;  
};

// \ru Специализация 2 \en Specialization 2 
template<class _Type>
struct which_type<empty_variant,_Type>
{
  static const int value = -1;
};

//----------------------------------------------------------------------------------------
/// \ru Получить тип варианта с заданным номером. \en Get variant type with a given index.
//---
template<class T, int idx >
struct type_which
{
  typedef typename T::tail_type                  tail_t;
  typedef typename type_which<tail_t,idx-1>::value_t value_t;
};

template<class T>
struct type_which<T,0>
{
  typedef typename T::value_type value_t;
};

template<class T>
struct type_which<T,-1>
{
private:
  typedef null_type value_t;
};

//----------------------------------------------------------------------------------------
/// \ru Проводник посетителя для рекурсивно-заданного контейнера. \en Conductor of visitor for recursively given container.
//---
template<class _PairUnion, int typeNb, int power >
struct union_conductor
{  
  typedef typename type_which<_PairUnion,typeNb>::value_t _Type;  
  
  /// \ru Статическое приведение типа. \en Static cast of type.
  template<class U, class T>
  static T * unsafe_cast( U & u ) { return u.template unsafe_cast<T>();  }

  /// \ru Статическое приведение типа. \en Static cast of type.
  template<class U, class T>
  static const T * unsafe_cast( const U & u ) { return u.template unsafe_cast<T>();  }
  
  /// \ru Применить функтор. \en Apply the functor.
  template<class _Visitor>
  static inline void apply( const _Visitor & vis, _PairUnion & oper )
  {    
    if ( oper.which() == typeNb )
    {
      vis( *unsafe_cast<_PairUnion,_Type>(oper) );
    }
    else
    {
      union_conductor<_PairUnion,typeNb+1,power>::apply( vis, oper );
    }
  }
};

//----------------------------------------------------------------------------------------
// \ru Вызвать деструктор для указателя, если его тип попадает в диапазон от t до power. \en Call the destructor for the pointer if its type is within the range from t to power.
//---
template<class _PairUnion, int power >
struct union_conductor<_PairUnion,power,power>
{
  template<class _Visitor>
  static inline void apply( const _Visitor & , _PairUnion &  ) {}
};

//----------------------------------------------------------------------------------------
/**
  \brief \ru Рекурсивное определение класса "union-контейнер". 
         \en Recursive definition of class "union-container".
  \details  \ru Контейнер, который может хранить элемент типа "value_type" или один из типов 
                хвостового контейнера.
            \en Container which can store the element of type "value_type" or one of types  
                of tail container. \~
*/
//---
template<class Type, class Tail = empty_variant >
class recursive_union
{
  typedef recursive_union<Type,Tail> _Myt;

public: // \ru доступные ассоциативные типы и константы \en available associative types and constants 
  static const size_t dataSize = sizeof(Type) > Tail::dataSize ? sizeof(Type) : Tail::dataSize;  
  static const size_t power = 1 + Tail::power; // \ru Количество типов, которое может обеспечить вариант \en The count of types which can provide a variant 
  
  typedef Type  value_type;
  typedef Tail  tail_type;

public:
  recursive_union() : m_typeNb(-1) { ::memset(m_data, 0, dataSize); }  
  recursive_union( const Type & );

#ifndef __DEBUG_MEMORY_ALLOCATE_FREE_
  template<class _Type>
  recursive_union( const _Type & elem )
    : m_typeNb( which_type<_Myt,_Type>::value )
    , dbg_data()
  {
    if ( m_typeNb >= 0 )
    {
      new ( (void*)m_data ) _Type( elem );
    }
  }
#else // __DEBUG_MEMORY_ALLOCATE_FREE_ 
  template<class _Type>
  recursive_union( const _Type & )
    : m_typeNb( which_type<_Myt,_Type>::value )
    , dbg_data()
  {
    // (!) The placement form of operator new is required.
    C3D_ASSERT_UNCONDITIONAL( false );
  }
#endif //__DEBUG_MEMORY_ALLOCATE_FREE_

 ~recursive_union();

private:  // \ru вспомогательные объекты \en assisting objects 
  // \ru Посетитель для вызова конструктора типа, которым занят union-контейнер \en The visitor to call the type constructor which is occupied by union-container 
  struct assigner
  {    
    assigner( _Myt & d ) : lOper(&d) {}

    template<class _Type>
    void operator() ( const _Type & elem ) const
    {    
      C3D_ASSERT( lOper );
      *lOper = elem;
    }
  private:
    mutable _Myt * lOper; // \ru Левый операнд присвоения \en The left operand of assignment 

  private:    
    assigner & operator = ( const assigner & );
  };

  // \ru Посетитель для вызова деструктора типа, которым занят union-контейнер \en The visitor to call the type destructor which is occupied by union-container   
  struct destroyer
  {
    template<class _Type>
    static void ignore(const _Type & ) {} // \ru для подавления сообщений \en for suppression of messages 

    template<class _Type>
    void operator() (const _Type & elem ) const 
    { 
      ignore( elem );
      elem.~_Type();
    }
  };

  // \ru Проверка на равенство \en The check for equality 
  struct comparer
  {
    const _Myt &  data;
    mutable bool  result;
    comparer( const _Myt & d ) : data( d ), result(false) {}
    template<class _Type>
    void operator() ( const _Type & elem ) const
    { 
      result = (data == elem);
    }
  private:
    comparer & operator = ( const comparer & );
  };  

  struct       conductor: public union_conductor<_Myt,0,power> {};
  struct const_conductor: public union_conductor<const _Myt,0,power> {};

public:
  int     which() const { return m_typeNb; }
  // \ru Проверить пустой ли контейнер \en Check whether the container is empty 
  bool    empty() const { return m_typeNb < 0; }
  
  // \ru Безопасное динамическое приведение типа \en Secure dynamic cast of type 
  template<class _Type>
  _Type * safe_cast()
  {
    if ( which_type<_Myt,_Type>::value == m_typeNb )
    {
      return (_Type*)m_data;
    }
    return 0;
  }
  // \ru Безопасное динамическое приведение типа \en Secure dynamic cast of type 
  template<class _Type>
  const _Type * safe_cast() const
  {
    if ( which_type<_Myt,_Type>::value == m_typeNb )
    {
      return (const _Type*)m_data;
    }
    return 0;
  }
  
  // \ru Статическое приведение  типа \en Static cast of type 
  template<class _Type>
  _Type * unsafe_cast()
  { 
    assert( (which_type<_Myt,_Type>::value == m_typeNb) );    
    return (_Type*)m_data;       
  }
  
  // \ru Статическое приведение  типа \en Static cast of type 
  /*
  template<int _typeNb> typename type_which<_Myt,_typeNb>::value_t &
  unsafe_get();
  */
  /*
  { 
    assert( (which_type<_Myt,_Type>::value == m_typeNb) );    
    return (type_which<_Myt,_typeNb>::value_t*)m_data;       
  }
  */

  // \ru Статическое приведение  типа \en Static cast of type 
  template<class _Type>
  const _Type * unsafe_cast() const
  { 
    assert( (which_type<_Myt,_Type>::value == m_typeNb) );    
    return reinterpret_cast<const _Type*>( m_data );
  }

  void release()
  {
    accept( destroyer() );
    m_typeNb = -1;
  }
  /*
  template<class _Type>
  void release()
  {
    if ( which_type<_Myt,_Type>::value == m_typeNb )
    {
      ((_Type*)m_data)->~_Type();
      m_typeNb = -1;
    }
  }
  */
  
  // \ru Присвоение другого union-контейнера \en Assignment of another union-container 
  _Myt & operator = ( const _Myt & v )
  {
    release();    
    v.accept( assigner(*this) );
    C3D_ASSERT( m_typeNb == v.m_typeNb );
    return *this;
  }
  
  // \ru Присвоение произвольного типа \en Assignment of arbitrary type 
  /*
  template<class _Type>
  _Myt & operator = ( const _Type & elem )
  {
    m_typeNb = which_type<_Myt,_Type>::value;
    if ( m_typeNb >= 0 )
    {
      new ( (void*)m_data ) _Type( elem );
    }
    return *this;
  }
  */
  
  // \ru Равенство \en Equality   
  bool operator == ( const _Myt & v ) const
  {
    if ( m_typeNb == v.m_typeNb )
    {
      comparer cmp( *this );
      v.accept( cmp );
      return cmp.result;
    }
    return false;
  }  
  
  // \ru Равенство \en Equality 
  template<class _Type>
  bool operator == ( const _Type & elem ) const
  {
    if ( m_typeNb == which_type<_Myt,_Type>::value )
    {
      return elem == *unsafe_cast<_Type>();
    }
    return false;
  }  

  // \ru Доступ посетителя \en Visitor access 
  template<class _Visitor>
  void accept( const _Visitor & vis ) const
  {
    const_conductor::apply( vis, *this );
  }  
  // \ru Доступ посетителя \en Visitor access 
  template<class _Visitor>
  void accept( const _Visitor & vis )
  {
    conductor::apply( vis, *this );
  }  

private:  // \ru данные \en data 
  char                m_data[dataSize];
  int                 m_typeNb;
  dbg_inspector<_Myt> dbg_data;
};

//----------------------------------------------------------------------------------------
//
//---
template<class Type, class Tail>
recursive_union<Type,Tail>::recursive_union( const Type & elem )
  : m_typeNb( 0 )
  , dbg_data()
{
  dbg_data.init( *this );  
  new ( (void*)m_data ) Type( elem );
}

//----------------------------------------------------------------------------------------
//
//---
template<class Type, class Tail>
recursive_union<Type,Tail>::~recursive_union()
{
  release();
}

template<class T, class Tail >
struct def_pair_union // \ru определитель рекурсивного контейнера для пары типов \en determinant of a recursive container for a pair of types 
{
  typedef recursive_union<T,Tail> value_t;
};
template<class T>
struct def_pair_union<T,empty_variant>
{
  typedef recursive_union<T,empty_variant> value_t;
};
template<>
struct def_pair_union<null_type,empty_variant>
{
  typedef empty_variant value_t;
};


//----------------------------------------------------------------------------------------
/** \brief \ru union-контейнер для экземпляра типа из определенного набора типов. 
           \en union-container for instance of type from a specific set of types.
    \details  \ru Позволяет создать тип, принимающий значения из некоторого набора разнородных 
                  типов.
              \en Allows to create a type which takes values ??from a set of heterogeneous 
                  types. \~
*/
//---
template<
    class T0
  , class T1
  , class T2 = null_type
  , class T3 = null_type
  , class T4 = null_type
  , class T5 = null_type>
class aligned_union
{
  typedef empty_variant                                _Tail6;
  typedef typename def_pair_union<T5,_Tail6>::value_t  _Tail5;
  typedef typename def_pair_union<T4,_Tail5>::value_t  _Tail4;
  typedef typename def_pair_union<T3,_Tail4>::value_t  _Tail3;
  typedef typename def_pair_union<T2,_Tail3>::value_t  _Tail2;
  typedef typename def_pair_union<T1,_Tail2>::value_t  _Tail1;
  typedef typename def_pair_union<T0,_Tail1>::value_t  _Variant;
  
  typedef aligned_union<T0,T1,T2,T3,T4,T5>  _Myt;

public:
  aligned_union(): m_data() {}
  template<class T>
  aligned_union( const T & elem ) : m_data( elem ) {}

public:
  /// \ru Выдать номер текущего типа, которым занят контейнер \en Get a index of the current type which is occupied container 
  int     which() const { return m_data.which(); }
  /// \ru Проверить пустой ли контейнер \en Check whether the container is empty 
  bool    empty() const { return m_data.empty(); }
  /// \ru Применить функтор (посетитель) \en Apply the functor (visitor) 
  template<class _Vis>
  void    accept( const _Vis & vis ) const { m_data.accept( vis ); }
  /// \ru Применить функтор (посетитель) \en Apply the functor (visitor) 
  template<class _Vis>
  void    accept( const _Vis & vis ) { m_data.accept( vis ); }
  /// \ru Сделать контейнер пустым \en Make an empty container 
  void    clear() { m_data.release(); }
  /// \ru Операция присвоения \en Assignment operation 
  _Myt &  operator = ( const _Myt & elem ) { m_data = elem.m_data; return *this; }
  template<class T >
  _Myt &  operator = ( const T & elem ) { m_data = elem; return *this; }
  // \ru Операция сравнения \en Compare operation 
  bool    operator == ( const _Myt & elem ) const { return m_data == elem.m_data; }
  template<class T >
  bool    operator == ( const T & elem ) const { return m_data == elem; }
  /// \ru Безопасно преобразовать тип контейнера к указателю \en Safely convert type of container to a pointer 
  template<class T >
  T *     safe_cast() { return m_data.template safe_cast<T>(); }
  template<class T >
  const T * safe_cast() const { return m_data.template safe_cast<T>(); }
  template<class T >
  bool get( T & val ) const 
  { 
    if ( const T * ptr = m_data.template safe_cast<T>() )
    {
      val = *ptr;
      return true;
    }
    return false;
  }

private:
  _Variant m_data;
};

//----------------------------------------------------------------------------------------
//
// ---
template<class _Iterator, class _Element>
constexpr bool is_exist( _Iterator begIt, _Iterator endIt, const _Element & elem )
{
  return std::find( begIt, endIt, elem ) != endIt;
}

//----------------------------------------------------------------------------------------
//
// ---
template<class _Iterator, class UnaryPredicate>
constexpr bool is_exist_if( _Iterator begIt, _Iterator endIt, UnaryPredicate _Pred )
{
  return std::find_if( begIt, endIt, _Pred ) != endIt;
}

namespace c3d
{

struct color_label
{
  color_code val;
  color_label() : val( white_color ) {}
  bool operator == ( color_code col ) const { return col == val; }
  color_label & operator = ( color_code col ) { val = col; return *this; }
};

//----------------------------------------------------------------------------------------
//
//---
template <typename Iterator>
struct _IterTraits 
{
  using value_type = typename Iterator::value_type;
  using reference = typename Iterator::reference;
};

template <typename T>
struct _IterTraits<T*> 
{ 
  typedef T  value_type; 
  typedef T& reference; 
};


//----------------------------------------------------------------------------------------
// Диапазон итераторов.
//---
template<class Iterator>
struct range : public std::pair<Iterator, Iterator>
{
  typedef std::pair<Iterator, Iterator> _Pair;
  
//typedef std::iterator_traits<Iterator> _IterTraits; // Стандартный пока не действует на итераторы графа.
  typedef typename _IterTraits<Iterator>::value_type value_type;
  typedef typename _IterTraits<Iterator>::reference  reference;  
  
  range( const Iterator & iter, const Iterator & last ) : _Pair( iter, last ) {}
  range( Iterator && iter, Iterator && last ) : _Pair( iter, last ) {}
  range( const _Pair & other ) : _Pair( other ) {}
  range() : _Pair() {}
  Iterator  begin() const { return _Pair::first; }
  Iterator  end() const { return _Pair::second; }
  bool      empty() const { return _Pair::first == _Pair::second; }
  size_t    size() const { return (size_t)std::distance( _Pair::first, _Pair::second ); }
  void      clear() { _Pair::first = _Pair::second; }
  range &   move_front() { ++_Pair::first; return *this; }
  range &   drop_front() { ++_Pair::first; return *this; }
  range &   drop_back() { --_Pair::second; return *this; }

  const reference front() const { return *_Pair::first; }
  const reference back() const { auto last = _Pair::second;  return *(--last); }
};

//----------------------------------------------------------------------------------------
// Get a range of the STL-container
//---
template<class _Cont>
inline range<typename _Cont::const_iterator> range_of( const _Cont & list )
{
  return {list.begin(), list.end()};
}

template<class _Cont>
inline range<typename _Cont::iterator> range_of( _Cont & list )
{
  return {list.begin(), list.end()};
}

//----------------------------------------------------------------------------------------
// Get a range of iterators
//---
template<class _Iterator>
inline range<_Iterator> make_range( _Iterator first, _Iterator last )
{
  return {first, last};
}

//----------------------------------------------------------------------------------------
/// \ru Пара ссылок. \en A pair of references.
//---
template<class _Ty1, class _Ty2> 
struct ref_pair
{
  _Ty1 & first;
  _Ty2 & second;

  ref_pair( _Ty1 & val1, _Ty2 & val2 )
    : first(val1), second(val2)
  {}
  ref_pair( const ref_pair & other )
    : first(other.first), second(other.second)
  {}

  template<class _Other1, class _Other2>
  ref_pair( const std::pair<_Other1, _Other2> & right )
    : first(right.first), second(right.second)
  {}
  
  template<class _Other1, class _Other2>
  ref_pair & operator = ( const std::pair<_Other1, _Other2> & right )
  {
    first = right.first;
    second = right.second;
    return *this;
  }

private:
  ref_pair & operator = ( const ref_pair & ); // \ru не реализуемо \en not implemented 
};

//----------------------------------------------------------------------------------------
/// \ru Выдать ссылки одной связкой. \en Get references as one bunch.
//---
template<typename Type1, typename Type2 >
inline ref_pair<Type1,Type2>
tie( Type1 & iter1, Type2 & iter2 )
{  
  return ref_pair<Type1,Type2> ( iter1, iter2 );
}

  //----------------------------------------------------------------------------------------
  //
  // ---
  template< class InputIt, class UnaryPredicate >
  inline bool all_of( InputIt iter, InputIt last, UnaryPredicate Pred )
  {
    for ( ; iter != last; ++iter )
    {
      if ( !Pred(*iter) )
      {
        return false;
      }
    }
    return true;
  }


}; // namespace c3d


//----------------------------------------------------------------------------------------
/** 
  \brief  \ru Динамический контейнер для хранения элементов упорядоченного множества. 
          \en Dynamic container for storing elements of an ordered set.

  \details 
          \ru Тип элемента контейнера должен иметь операторы порядка. Не стоит путать этот 
  тип контейнера с set или map. Он вовсе не обязан всегда поддерживаться в отсортированном 
  состоянии, а только тогда, когда это закажут (с кэшированием алгоритма сортировки).
  Гарантируется, что вектор отсортирован сразу после вызова функций get_sorted или sort.
  Константные методы, а также метод erase не нарушают сортировки.\n
           \en Type of container element must have order operators. Do not  confuse this 
  type of container with a set or map. It is not obliged always  be supported in a sorted 
  state, and only when it is needed  (with caching of sorting algorithm). It is guaranteed 
  that the vector is sorted immediately after the function call get_sorted or sort.
  Const methods and the erase method does not break sorting. \n \~

    \par  \ru Про эффективность

    Часто сортированный вектор оказывается более эффективным, чем std::map или std::set, 
  особенно если добавление/удаление элементов массива осуществляется серийно и достаточно 
  редко перемежаются, с запросами быстрого (бинарного) поиска элемента или его места по 
  порядку. В отличие от map или set минимально дефрагментируется память и не требуется
  избыточной информации для хранения указателей (может занимать в 4 раза меньше памяти).
    Для быстрых запросов можно применять стандартные алгоритмы, такие 
    как std::binary_search, std::lower_bound и т.п.

          \en About efficiency

    Often sorted vector is more effective than 
  std::map or std::set especially when adding/removing elements
  of the array is standard and is rarely interspersed 
  with queries quickly (binary) search of element or its place by  
  the order. In contrast to the map or set minimal defragmented 
  memory and does not require excess information for storage of pointers  
  (can occupy memory in less than 4 times).
    For fast queries, can use standard algorithms such 
    as std::binary_search, std::lower_bound etc. \~
*/
//---
template<class KeyType, class _Pr = std::less<KeyType> > // \ru KeyType - тип элемента с операторами порядка "<" \en KeyType - the element type with the operators of order "<" 
class sorting_array
{
public:  
  typedef std::vector<KeyType>                    container_type;
  typedef typename container_type::value_type     value_type;
  typedef typename container_type::size_type      size_type;
  typedef typename container_type::const_iterator iterator;
  typedef typename container_type::iterator      _iterator;
  typedef          c3d::range<iterator>           iter_range;
  typedef _Pr                                     key_compare; // \ru отношение порядка (предикат) \en order relation (predicate) 

public:
  sorting_array() : m_vector(), m_sorted( true ) {}
  sorting_array(const sorting_array&) = default;
  sorting_array & operator= (const sorting_array&) = default;

public:
  iter_range      get_sorted() { sort(); return iter_range(m_vector.begin(), m_vector.end()); }
  iter_range      range() const { return iter_range(m_vector.begin(), m_vector.end()); }
  const KeyType & sorted_back() { sort(); return m_vector.back(); }
  bool            empty() const { return m_vector.empty(); }
  iterator        begin() const { return m_vector.begin(); }
  iterator        end() const { return m_vector.end(); }
 _iterator       _begin() { return m_vector.begin(); }
 _iterator       _end() { return m_vector.end(); }
  const KeyType & front() const { return m_vector.front(); }
  const KeyType & back() const { return m_vector.back(); }
  void            erase( iterator );
  void            erase( iterator f, iterator l );
  bool            is_sorted() const { return m_sorted; }
  iterator        insert( iterator _whereItr, const KeyType & val );  // \ru вставить элемент перед позицией whereItr \en insert element before position whereItr 
  template <class InputIterator>
  void            insert( iterator position, InputIterator first, InputIterator last )
  {
    m_vector.insert( position, first, last );
    m_sorted = false;
  }
  template <class InputIterator>
  void      assign ( InputIterator first, InputIterator last )
  {
    m_vector.assign( first, last );
    m_sorted = false;
  }
  void      resize( size_t n, const KeyType & val );
  void      reserve ( size_t n ) { m_vector.reserve( n ); }
  void      push_back( const KeyType & val );
  void      sort() 
  {
    if ( !m_sorted )
    {
      std::sort( m_vector.begin(), m_vector.end(), _Pr() );
      m_sorted = true;
    }
  } 
  void      clear() { m_vector.clear(); }
  size_t    size() const { return m_vector.size(); }
        KeyType & operator[] ( size_t n ) { PRECONDITION( n < m_vector.size() ); return m_vector[n]; }
  const KeyType & operator[] ( size_t n ) const { PRECONDITION( n < m_vector.size() ); return m_vector[n]; }
  
private:
  container_type m_vector;
  bool           m_sorted;
};

//----------------------------------------------------------------------------------------
//
// ---
template<class KeyType, class _Pr >
void sorting_array<KeyType,_Pr>::push_back( const KeyType & val ) 
{
  m_sorted = m_vector.empty() ? true : m_sorted && _Pr()( m_vector.back(), val );
  m_vector.push_back( val ); 
}

//----------------------------------------------------------------------------------------
//
//---
template<class KeyType, class _Pr >
void sorting_array<KeyType,_Pr>::erase( iterator ersItr )
{  
  m_vector.erase( m_vector.begin() + (ersItr - begin()) );
}

//----------------------------------------------------------------------------------------
//
//---
template<class KeyType, class _Pr >
void sorting_array<KeyType,_Pr>::erase( iterator f, iterator l )
{ 
  typename container_type::iterator first, last;
  first = last = m_vector.begin();
  std::advance( first, std::distance(begin(),f) ); // convert from const-iterator to non-const 
  std::advance( last,  std::distance(begin(),l) );
  m_vector.erase( first, last );
}

//----------------------------------------------------------------------------------------
// \ru Вставить элемент перед позицией whereItr \en Insert element before position whereItr 
//---
template<class KeyType, class _Pr >
typename sorting_array<KeyType,_Pr>::iterator 
sorting_array<KeyType,_Pr>::insert( iterator _whereItr, const KeyType & val )
{ 
  typename container_type::iterator whereItr = m_vector.begin();
  std::advance( whereItr, std::distance(begin(),_whereItr) ); // \ru перевод из конст-итератора в неконст \en convert from const-iterator to non-const 
  // \ru Далее проверяем не нарушает ли новая вставка упорядоченности массива \en Next, whether new insert does not break ordering of the array 
  if ( m_sorted && (_whereItr != m_vector.end()) )
  {
    m_sorted = ! key_compare()( *_whereItr, val );
    if ( m_sorted ) // val <= _where
    {
      m_sorted = ( _whereItr == m_vector.begin() ) || !key_compare()( val, *(--_whereItr) );
    } 
  }    

  // \ru Вставка \en Insert   
  return m_vector.insert( whereItr, val );
}

//----------------------------------------------------------------------------------------
//
//---
template<class KeyType, class _Pr >
void sorting_array<KeyType,_Pr>::resize( size_t n, const KeyType & val )
{  
  if ( m_sorted && (m_vector.size() < n) && !m_vector.empty() )
  {
    m_sorted = !key_compare()( val, m_vector.back() ); // m_vector.back() <= val
  }

  m_vector.resize( n, val );
}

//----------------------------------------------------------------------------------------
/// \ru Проверка упорядоченности массива \en Check for ordering array 
//---
template<class SortedArray>
bool check_ordering( const SortedArray & arr )
{
  if ( arr.is_sorted() && !arr.empty() )
  {
    typename SortedArray::value_type prev( *arr.begin() );
    typename SortedArray::iterator first = arr.begin()+1;
    typename SortedArray::iterator last = arr.end();
    for( ; first!=last; ++first )
    {
      if ( (*first) < prev )
      {
        return false; // \ru нарушен порядок следования \en order has been broken 
      }
      prev = *first;
    }
  }

  return true;
}

#endif // __GENERIC_UTILITY_H

// eof
