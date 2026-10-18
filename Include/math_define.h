////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Базовые макросы и функции.
         \en Base macros and functions. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef MATH_DEFINE_H
#define MATH_DEFINE_H


#include <system_types.h>

#ifdef C3D_WINDOWS //_MSC_VER  // define PRECONDITION
  #include <crtdbg.h>
  #define PRECONDITION  _ASSERT ///< \ru Определение PRECONDITION. \en The PRECONDITION definition.   \~ \ingroup Base_Tools
#else // C3D_WINDOWS
  #include <assert.h>
  #define _ASSERT assert
  #ifndef PRECONDITION
    #ifdef C3D_DEBUG
      #include <iostream>
      inline void __precondition( bool expr, const char* file, int line, const char* func, const char* expression ) {
          if ( !expr ) fprintf(stderr, "In file %s, line %d:\nfailed PRECONDITION `%s' in function: %s.\n", file, line, expression, func);
      }
      #define PRECONDITION(expr) __precondition(expr, __FILE__, __LINE__, __PRETTY_FUNCTION__, #expr)  ///< \ru Определение PRECONDITION. \en The PRECONDITION definition.   \~ \ingroup Base_Tools
    #else
      #define PRECONDITION(expr) ((void)0)  ///< \ru Определение PRECONDITION. \en The PRECONDITION definition.   \~ \ingroup Base_Tools
    #endif  // C3D_DEBUG
  #endif  // PRECONDITION
#endif // C3D_WINDOWS

#include <algorithm>
#include <utility>
#include <vector>
#include <set>
#include <functional>

namespace c3d // namespace C3D
{
typedef std::pair<size_t, size_t>               IndicesPair;       ///< \ru Пара целочисленных неотрицательных индексов. \en Pair of non-negative integer indices. 
typedef std::pair<ptrdiff_t, ptrdiff_t>         NumbersPair;       ///< \ru Пара целочисленных номеров. \en Pair of integer numbers. 
typedef std::pair<uint, uint>                   UintPair;          ///< \ru Пара 32-битных целочисленных неотрицательных индексов. \en Pair of 32-bit non-negative integer indices. 
typedef std::pair<bool, bool>                   BoolPair;          ///< \ru Пара флагов. \en Bool pair.
typedef std::pair<double, double>               DoublePair;        ///< \ru Пара действительных чисел двойной точности с плавающей запятой. \en Pair of doubles. 
typedef std::pair<IndicesPair, double>          IndicesPairDouble; ///< \ru Пара индексов и числа. \en A pair of indices and double. 
typedef std::pair<double, IndicesPair>          DoubleIndicesPair; ///< \ru Число и пара индексов. \en Double and a pair of indices. 

typedef std::pair<size_t, bool>                 IndexBool;     ///< \ru Пара номер-флаг. \en Index-double pair. 
typedef std::pair<bool, size_t>                 BoolIndex;     ///< \ru Пара флаг-номер. \en Double-index pair.
typedef std::pair<ptrdiff_t, bool>              NumberBool;    ///< \ru Пара номер-флаг. \en Index-double pair. 
typedef std::pair<bool, ptrdiff_t>              BoolNumber;    ///< \ru Пара флаг-номер. \en Double-index pair.
typedef std::pair<size_t, double>               IndexDouble;   ///< \ru Пара номер-число. \en Index-double pair. 
typedef std::pair<double, size_t>               DoubleIndex;   ///< \ru Пара число-номер. \en Double-index pair. 
typedef std::pair<bool, double>                 FlagDouble;    ///< \ru Пара флаг-число. \en Flag-double pair. 
typedef std::pair<double, bool>                 DoubleFlag;    ///< \ru Пара число-флаг. \en Double-flag pair.
typedef FlagDouble                              BoolDouble;    ///< \ru Пара флаг-число. \en Flag-double pair. 
typedef DoubleFlag                              DoubleBool;    ///< \ru Пара число-флаг. \en Double-flag pair.

typedef std::vector<size_t>                     IndicesVector; ///< \ru Вектор целочисленных неотрицательных индексов. \en Vector of non-negative integer indices. 
typedef std::vector<ptrdiff_t>                  NumbersVector; ///< \ru Вектор целочисленных номеров. \en Vector of integer numbers. 
typedef std::vector<uint>                       UintVector;    ///< \ru Вектор 32-битных целочисленных неотрицательных индексов. \en Vector of 32-bit non-negative integer indices. 
typedef std::vector<bool>                       BoolVector;    ///< \ru Вектор флагов. \en Bool vector.
typedef std::vector<double>                     DoubleVector;  ///< \ru Вектор double. \en Double vector.

typedef std::vector<IndicesPair>                IndicesPairsVector; ///< \ru Вектор пар целочисленных неотрицательных индексов. \en Vector of pairs of non-negative integer indices. 
typedef std::vector<NumbersPair>                NumbersPairsVector; ///< \ru Вектор пар целочисленных индексов. \en Vector of pairs of integer indices. 
typedef std::vector<DoublePair>                 DoublePairsVector;  ///< \ru Вектор пар double. \en Vector of double pairs. 

typedef std::set<size_t>                        IndicesSet;     ///< \ru Набор целочисленных неотрицательных индексов. \en Set of non-negative integer indices. 
typedef IndicesSet::iterator                    IndicesSetIt;
typedef IndicesSet::const_iterator              IndicesSetConstIt;
typedef std::pair<IndicesSetConstIt, bool>      IndicesSetRet;

typedef std::set<ptrdiff_t>                     NumbersSet;     ///< \ru Набор целочисленных номеров. \en Set of integer numbers. 
typedef NumbersSet::iterator                    NumbersSetIt;
typedef NumbersSet::const_iterator              NumbersSetConstIt;
typedef std::pair<NumbersSetConstIt, bool>      NumbersSetRet;

typedef std::set<uint>                          UintSet;        ///< \ru Набор 32-битных целочисленных неотрицательных индексов. \en Set of 32-bit non-negative integer indices. 
typedef UintSet::iterator                       UintSetIt;
typedef UintSet::const_iterator                 UintSetConstIt;
typedef std::pair<UintSetConstIt, bool>         UintSetRet;

typedef std::set<UintPair>                      UintPairsSet;   ///< \ru Набор пар 32-битных целочисленных неотрицательных индексов. \en Set of pairs of 32-bit non-negative integer indices. 
typedef UintPairsSet::iterator                  UintPairsSetIt;
typedef UintPairsSet::const_iterator            UintPairsSetConstIt;
typedef std::pair<UintPairsSetConstIt, bool>    UintPairsSetRet;

typedef std::set<IndicesPair>                   IndicesPairsSet; ///< \ru Набор пар целочисленных неотрицательных индексов. \en Set of pairs of non-negative integer indices.
typedef IndicesPairsSet::iterator               IndicesPairsSetIt;
typedef IndicesPairsSet::const_iterator         IndicesPairsSetConstIt;
typedef std::pair<IndicesPairsSetConstIt, bool> IndicesPairsSetRet;

typedef std::pair<IndicesPair,IndicesPair>      IndicesPairsPair; ///< \ru Пара индексных пар. \en Pair of indices' pair. 

template <typename T>
using RefWrap = std::reference_wrapper<T>;

template <typename T>
using ConstRefWrap = std::reference_wrapper<const T>;

//------------------------------------------------------------------------------
/** \brief \ru Проверка нулевого указателя.
           \en Null pointer check . \~
  \details \ru Проверка нулевого указателя. \n
           \en Null pointer check. \n \~
  \ingroup Base_Tools
*/
// ---
template <class ItemPtr>
bool IsNullPointer( const ItemPtr * itemPtr ) {
  return ((nullptr == itemPtr) ? true : false);
}

//------------------------------------------------------------------------------
/** \brief \ru Сортировка массива с удалением дубликатов.
           \en Sorting an array with removing duplicates. \~
  \details \ru Сортировка массива с удалением дубликатов. \n
           \en Sorting an array with removing duplicates. \n \~
  \ingroup Base_Tools
*/
// ---
template <class ElementsVector>
void UniqueSortVector( ElementsVector & items )
{
  if ( items.size() > 1 ) {
    std::sort( items.begin(), items.end() );
    items.erase( std::unique( items.begin(), items.end() ), items.end() );
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Поиск элемента в не сортированном массиве.
           \en Finding an element in a unsorted array. \~
  \details \ru Поиск элемента в не сортированном массиве. \n
           \en Finding an element in a unsorted array. \n \~
  \ingroup Base_Tools
*/
// ---
template <class ElementsVector, class Element>
size_t DirectSearch( const ElementsVector & items, const Element & item )
{
  if ( items.size() > 0 ) {
    size_t dist = std::distance( items.begin(), std::find( items.begin(), items.end(), item ) );
    if ( dist < items.size() )
      return dist;
  }
  return SYS_MAX_T;
}


//------------------------------------------------------------------------------
/** \brief \ru Поиск элемента в сортированном массиве.
           \en Finding an element in a sorted array. \~
  \details \ru Поиск элемента в сортированном массиве. \n
           \en Finding an element in a sorted array. \n \~
  \ingroup Base_Tools
*/
// ---
template <class ElementsVector, class Element>
size_t BinarySearch( const ElementsVector & items, const Element & item )
{
  typename ElementsVector::const_iterator it = std::lower_bound( items.begin(), items.end(), item );
  if ( (it != items.end()) && !(item < *it) ) {
    return std::distance( items.begin(), it );
  }
  return SYS_MAX_T;
}

} // namespace C3D


#ifdef C3D_WINDOWS //_MSC_VER
//------------------------------------------------------------------------------
/** \brief \ru Определение CALL_DECLARATION.
           \en The CALL_DECLARATION definition. \~
  \details \ru Определение CALL_DECLARATION. \n
           \en The CALL_DECLARATION definition. \n \~
  \ingroup Base_Tools
*/
// ---
#define CALL_DECLARATION    __cdecl
//------------------------------------------------------------------------------
/** \brief \ru Определение EXPORT_DECLARATION.
           \en The EXPORT_DECLARATION definition. \~
  \details \ru Определение EXPORT_DECLARATION. \n
           \en The EXPORT_DECLARATION definition. \n \~
  \ingroup Base_Tools
*/
// ---
#define EXPORT_DECLARATION  __declspec(dllexport)
#else // C3D_WINDOWS
//------------------------------------------------------------------------------
/** \brief \ru Определение CALL_DECLARATION.
           \en The CALL_DECLARATION definition. \~
  \details \ru Определение CALL_DECLARATION. \n
           \en The CALL_DECLARATION definition. \n \~
  \ingroup Base_Tools
*/
// ---
#define CALL_DECLARATION
//------------------------------------------------------------------------------
/** \brief \ru Определение EXPORT_DECLARATION.
           \en The EXPORT_DECLARATION definition. \~
  \details \ru Определение EXPORT_DECLARATION. \n
           \en The EXPORT_DECLARATION definition. \n \~
  \ingroup Base_Tools
*/
// ---
#define EXPORT_DECLARATION

#endif // C3D_WINDOWS

//#ifdef C3D_WINDOWS //_MSC_VER // std_min() / std_max()
//
//#define std_max(a,b)      (std::max)(a,b)
//#define std_min(a,b)      (std::min)(a,b)
//#define std_maxRef(a,b)   (std::max)(a,b)
//#define std_minRef(a,b)   (std::min)(a,b)
//
//#ifndef NOMINMAX
//#include <algorithm>
//#endif // NOMINMAX
//
//#else // _MSC_VER

#include <algorithm>

#define std_max(a,b)      (std::max)(a,b)
#define std_min(a,b)      (std::min)(a,b)
#define std_maxRef(a,b)   (std::max)(a,b)
#define std_minRef(a,b)   (std::min)(a,b)

//#endif  // _MSC_VER

//------------------------------------------------------------------------------
// \ru Для совместимости с VC < 2005 \en To be compatible with VC < 2005 
//---
#if defined(_MSC_VER) && (_MSC_VER < 1400)
  #define TEMPLATE_TYPENAME
#else
  #define TEMPLATE_TYPENAME template<typename Type>
#endif // _MSC_VER

//------------------------------------------------------------------------------
// \ru Синтаксис дружественной шаблонной функции шаблона \en Syntax of friendly template function of a template 
#if !(defined (_MSC_VER)) || (_MSVC_PERMISSIVE_OFF)

  #define TEMPLATE_FRIEND friend // \ru по стандарту C++98 \en by the C++98 standard 
  #define TEMPLATE_SUFFIX <Type>
  #define TEMPLATE_SUFFIX2 <T1, T2>
  #define FORVARD_DECL_TEMPLATE_TYPENAME( _FUNC ) template<typename Type> _FUNC
  #define FORVARD_DECL_TEMPLATE_TYPENAME2( _FUNC ) template<typename T1, typename T2> _FUNC
  
#else // _MSC_VER

  #define TEMPLATE_FRIEND template<typename Type> friend // \ru для VS2005 \en for VS2005 
  #define TEMPLATE_SUFFIX
  #define TEMPLATE_SUFFIX2 
  #define FORVARD_DECL_TEMPLATE_TYPENAME( _FUNC ) 
  #define FORVARD_DECL_TEMPLATE_TYPENAME2( _FUNC ) 
  
#endif // _MSC_VER


//------------------------------------------------------------------------------
/** \brief \ru Объявление оператора присваивания и конструктора копирования.
           \en The declaration of assignment operator and copy constructor. \~
  \details \ru Объявление приватных оператора присваивания и конструктора копирования.
    Используется для запрета неявной реализации этой функциональности, т.к.
    при отсутствии в классах явного дублирующего конструктора и оператора присваивания
    автоматически генерируются неявные - в основном, через копирование памяти,
    что может привести к некорректному поведению системы.
           \en The declaration of private assignment operator and copy constructor.
    This is used to prohibit an implicit implementation of this functionality because
    if there is no an explicit copy constructor or an assignment operator
    implicit copy constructor and assigment operator are generated automatically mostly by copying of memory,
    that can lead to incorrect behaviour of the system. \~
  \ingroup Base_Tools
*/
//---
#define OBVIOUS_PRIVATE_COPY( ClassName )       \
private:                                        \
  ClassName & operator = ( const ClassName & ); \
  ClassName( const ClassName & );


//------------------------------------------------------------------------------
/// \ru Получить количество элементов массива. \en Get the number of array elements. \~ \ingroup Base_Tools
//---
#define COUNTOF(array) (sizeof(array)/sizeof(array[0]))


//------------------------------------------------------------------------------
// \ru Объявить тест кейс как дружественный по отношению к классу, для \en Declare the test case as friendly to the class in order 
// \ru того чтобы была возможность доступа к закрытым членам этого класса. \en to be able to access to the private members of the class. 
//---
#define DECLARE_FRIEND_TEST_CASE( SuiteName, CaseName ) friend class SuiteName ## _ ## CaseName ## _Test


//------------------------------------------------------------------------------
// \ru Макросы __TODO__ и __WARN__ предназначены для \en Macros __ TODO__ and __ WARN__ are designed for  
// \ru генерации сообщений совместно с #pragma message \en generation of messages with #pragma message 
// \ru и позволяют в среде MsDev переходить на строку \en and allow to move to the next line in the MsDev environment  
// \ru кода с сообщением по двойному клику в окне Output \en with the message by double click in the Output window  
//
// \ru примеры: \en examples: 
// \ru #pragma message( __TODO__ "Восстановить закрытый код" ) \en #pragma message( __TODO__ "Restore the private code" ) 
// \ru #pragma message( __WARN__ "Отсутствует проверка на nullptr" ) \en #pragma message( __WARN__ "There is no check for nullptr" )
//---
#define __ANYTOSTR__(x) #x
#define __DEFTOSTR__(x) __ANYTOSTR__(x)

#ifdef _MSC_VER // __TODO__ / __WARN__

#define __TODO__ __FILE__ "(" __DEFTOSTR__(__LINE__) ") : TODO: "
#define __WARN__ __FILE__ "(" __DEFTOSTR__(__LINE__) ") : warning: "

#else // _MSC_VER
// For linux
#define __TODO__ 
#define __WARN__ 

#endif // _MSC_VER

//------------------------------------------------------------------------------
// \ru Макрос для вывода диагностических и отладочных сообщений ядра в стандартный поток вывода ошибок stderr.
// \en Macro for outputting diagnostic and debug kernel messages to standard error output stream stderr.
// ---

#define C3D_WARNING(expr) \
    fprintf(stderr, "WARNING: In file %s(line: %d), in function %s:\n\t%s\n\n", \
        __FILE__, __LINE__, __FUNCTION__, c3d::ToSTDstring(c3d::string_t(expr)).c_str() );

//------------------------------------------------------------------------------
/** \brief \ru Объявление экспортности или импортности классов.
           \en The declaration of export or import classes. \~
  \details \ru Объявление экспортности в данном модуле или импортности в других подключаемых модулях. \n
           \en The declaration of export in this module or import in other plugged modules. \n \~
  \ingroup Base_Tools
*/
// ---
// \ru Модуль геометрического моделирования. \en Geometric modeling module. 
#ifdef C3D_WINDOWS //_MSC_VER
  #if defined ( _BUILDMATHDLL )
    #define MATH_CLASS         __declspec( dllexport )
    #define MATH_FUNC(retType) __declspec( dllexport ) retType CALL_DECLARATION
    #define MATH_FUNC_EX       __declspec( dllexport ) // \ru для KNOWN_OBJECTS_RW_REF_OPERATORS_EX и KNOWN_OBJECTS_RW_PTR_OPERATORS_EX \en for KNOWN_OBJECTS_RW_REF_OPERATORS_EX and KNOWN_OBJECTS_RW_PTR_OPERATORS_EX
  #else
    #define MATH_CLASS         __declspec( dllimport )
    #define MATH_FUNC(retType) __declspec( dllimport ) retType CALL_DECLARATION
    #define MATH_FUNC_EX       __declspec( dllimport )
  #endif
#else // C3D_WINDOWS
    #define MATH_CLASS         __attribute__ ((visibility ("default")))
    #define MATH_FUNC(retType) __attribute__ ((visibility ("default"))) retType
    #define MATH_FUNC_EX       __attribute__ ((visibility ("default")))
#endif

// \ru Модуль геометрических ограничений. \en Geometric constraints module. 
#define   GCE_CLASS    MATH_CLASS
#define   GCM_CLASS    MATH_CLASS
#define   GCE_FUNC     MATH_FUNC
#define   GCM_FUNC     MATH_FUNC
#ifdef GCT_EXPORT
  #define GCT_CLASS    MATH_CLASS
  #define GCT_FUNC     MATH_FUNC
#else
  #define GCT_CLASS
  #define GCT_FUNC(retType)  retType
#endif // GCT_EXPORT

// \ru Модуль конвертеров. \en Converters module. 
#define   CONV_CLASS   MATH_CLASS
#define   CONV_FUNC    MATH_FUNC
#define   CONV_FUNC_EX MATH_FUNC_EX

namespace c3d // namespace C3D
{

//------------------------------------------------------------------------------
/// \ru Максимальное количество элементов матрицы MxN. \en Maximum number of MxN matrix elements. 
//---
constexpr size_t MATRIX_MAX_COUNT = 1000000000; // 1e9

//------------------------------------------------------------------------------
/// \ru Максимальный размер массива. \en Maximum size of array. 
//---
constexpr size_t ARRAY_MAX_COUNT = 1000000; // 1e6

//------------------------------------------------------------------------------
/**
  \brief \ru Проверить точки на равенство.
         \en Check points for equality. \~
  \details \ru Точки считаются равными, если их координаты отличаются на величину, 
               не превышающую заданную погрешность.
           \en Points are considered as equal if their coordinates differ by a value 
               which doesn't exceed a given tolerance. \~
  \param[in] p1 - \ru Первая декартова точка.
                  \en The first cartesian point. \~
  \param[in] p2 - \ru Вторая декартова точка.
                  \en The second cartesian point. \~
  \param[in] eps - \ru Метрическая погрешность совпадения точек.
                   \en Metric tolerance of points coincidence. \~
  \return \ru true, если точки совпадают, \n иначе false.
          \en True if points coincide, \n false otherwise. \~
  \ingroup Base_Algorithms
*/ 
// ---
template <class Point>
bool EqualPoints( const Point & p1, const Point & p2, double eps ) {
  return p1.IsSame( p2, eps );
}

//------------------------------------------------------------------------------
/**
  \brief \ru Проверить точки на равенство.
         \en Check points for equality. \~
  \param[in] p1 - \ru Первая декартова точка.
                  \en The first cartesian point. \~
  \param[in] p2 - \ru Вторая декартова точка.
                  \en The second cartesian point. \~
  \param[in] xEpsilon - \ru Метрическая погрешность совпадения точек вдоль оси X.
                        \en The metric tolerance of points coincidence along the X axis. \~
  \param[in] yEpsilon - \ru Метрическая погрешность совпадения точек вдоль оси Y.
                        \en The metric tolerance of points coincidence along the Y axis. \~
  \return \ru true, если точки совпадают, \n иначе false.
          \en True if points coincide, \n false otherwise. \~
  \ingroup Algorithms_2D
*/
// ---
template <class Point>
bool EqualPoints( const Point & p1, const Point & p2, double xEpsilon, double yEpsilon ) {
  return (::fabs(p1.x - p2.x) < xEpsilon && ::fabs(p1.y - p2.y) < yEpsilon);
}

//------------------------------------------------------------------------------
/**
  \brief \ru Проверить точки на равенство.
         \en Check points for equality. \~
  \param[in] p1 - \ru Первая декартова точка.
                  \en The first cartesian point. \~
  \param[in] p2 - \ru Вторая декартова точка.
                  \en The second cartesian point. \~
  \param[in] xEpsilon - \ru Метрическая погрешность совпадения точек вдоль оси X.
                        \en The metric tolerance of points coincidence along the X axis. \~
  \param[in] yEpsilon - \ru Метрическая погрешность совпадения точек вдоль оси Y.
                        \en The metric tolerance of points coincidence along the Y axis. \~
  \param[in] zEpsilon - \ru Метрическая погрешность совпадения точек вдоль оси Z.
                        \en The metric tolerance of points coincidence along the Y axis. \~
  \return \ru true, если точки совпадают, \n иначе false.
          \en True if points coincide, \n false otherwise. \~
  \ingroup Algorithms_3D
*/
// ---
template <class Point>
bool EqualPoints( const Point & p1, const Point & p2, double xEpsilon, double yEpsilon, double zEpsilon ) {
  return (::fabs(p1.x - p2.x) < xEpsilon && ::fabs(p1.y - p2.y) < yEpsilon && ::fabs(p1.z - p2.z) < zEpsilon);
}

//------------------------------------------------------------------------------
/** \brief \ru Проверить векторы на равенство с заданной точностью.
           \en Check equality of vectors with given tolerance. \~
  \details \ru Проверка равенства векторов с заданной точностью. 
    Векторы считаются равными, если их координаты отличаются на величину, не превышающую заданную погрешность.
           \en Check equality of vectors with given tolerance. 
    Vectors are equal if their coordinates differs less than given tolerance. \~
  \param[in] p1 - \ru Первый вектор.
                  \en The first vector. \~
  \param[in] p2 - \ru Второй вектор.
                  \en The second vector. \~
  \param[in] eps - \ru Погрешность координат.
                   \en Coordinate tolerance. \~
  \return \ru Возвращает true, если векторы равны.
          \en Returns true if the vectors are equal. \~
  \ingroup Base_Algorithms
*/
// ---
template <class Vector>
bool EqualVectors( const Vector & p1, const Vector & p2, double eps ) {
  return p1.IsSame( p2, eps );
}

//------------------------------------------------------------------------------
/** \brief \ru Проверить векторы на равенство с заданной точностью.
           \en Check equality of vectors with given tolerance. \~
  \details \ru Проверка равенства векторов с заданной точностью. 
    Векторы считаются равными, если их координаты отличаются на величину, не превышающую заданную погрешность.
           \en Check equality of vectors with given tolerance. 
    Vectors are equal if their coordinates differs less than given tolerance. \~
  \param[in] p1 - \ru Первый вектор.
                  \en The first vector. \~
  \param[in] p2 - \ru Второй вектор.
                  \en The second vector. \~
  \param[in] xEpsilon - \ru Метрическая погрешность совпадения точек вдоль оси X.
                        \en The metric tolerance of points coincidence along the X axis. \~
  \param[in] yEpsilon - \ru Метрическая погрешность совпадения точек вдоль оси Y.
                        \en The metric tolerance of points coincidence along the Y axis. \~
  \return \ru Возвращает true, если векторы равны.
          \en Returns true if the vectors are equal. \~
  \ingroup Algorithms_2D
*/
// ---
template <class Vector>
bool EqualVectors( const Vector & p1, const Vector & p2, double xEpsilon, double yEpsilon )
{
  return (::fabs(p1.x - p2.x) < xEpsilon && ::fabs(p1.y - p2.y) < yEpsilon);
}

//------------------------------------------------------------------------------
/** \brief \ru Проверить векторы на равенство с заданной точностью.
           \en Check equality of vectors with given tolerance. \~
  \details \ru Проверка равенства векторов с заданной точностью. 
    Векторы считаются равными, если их координаты отличаются на величину, не превышающую заданную погрешность.
           \en Check equality of vectors with given tolerance. 
    Vectors are equal if their coordinates differs less than given tolerance. \~
  \param[in] p1 - \ru Первый вектор.
                  \en The first vector. \~
  \param[in] p2 - \ru Второй вектор.
                  \en The second vector. \~
  \param[in] xEpsilon - \ru Метрическая погрешность совпадения точек вдоль оси X.
                        \en The metric tolerance of points coincidence along the X axis. \~
  \param[in] yEpsilon - \ru Метрическая погрешность совпадения точек вдоль оси Y.
                        \en The metric tolerance of points coincidence along the Y axis. \~
  \param[in] zEpsilon - \ru Метрическая погрешность совпадения точек вдоль оси Z.
                        \en The metric tolerance of points coincidence along the Y axis. \~
  \return \ru Возвращает true, если векторы равны.
          \en Returns true if the vectors are equal. \~
  \ingroup Algorithms_3D
*/
// ---
template <class Vector>
bool EqualVectors( const Vector & p1, const Vector & p2, double xEpsilon, double yEpsilon, double zEpsilon )
{
  return (::fabs(p1.x - p2.x) < xEpsilon && ::fabs(p1.y - p2.y) < yEpsilon && ::fabs(p1.z - p2.z) < zEpsilon);
}

//------------------------------------------------------------------------------
/** \brief \ru Сравнить матрицы.
           \en Compare matrices \~
  \details \ru Толерантное сравнение двух матриц.
           \en Tolerant comparison of two matrices. \~
  \param[in] m1, m2 - \ru Исходные матрицы.
                      \en Initial matrices. \~
  \param[in] accuracy - \ru Толерантность.
                        \en A tolerance. \~
  \return \ru true, если матрицы равны, \n false в противном случае.
          \en Returns true if matrices are equal, \n false otherwise. \~
  \ingroup Base_Algorithms
*/
// ---
template <class Matrix>
bool EqualMatrices( const Matrix & m1, const Matrix & m2, double accuracy ) {
  return m1.IsSame( m2, accuracy );
}

//------------------------------------------------------------------------------
/** \brief \ru Проверка кубов на равенство с управляемой погрешностью.
           \en Check for equality of boxes with controlled tolerance. \~
  \details \ru Проверка кубов на равенство с управляемой погрешностью.\n
           \en Check for equality of boxes with controlled tolerance. \n \~
  \ingroup Mathematic_Base_3D
*/
// ---
template <class BBox>
bool EqualCubes( const BBox & c1, const BBox & c2, double eps )
{
  if ( !c1.IsEmpty() && !c2.IsEmpty() )
    return c1.IsSame( c2, eps );
  return false;
}

//-------------------------------------------------------------------------------
/** \brief \ru Вычисление косинуса и синуса.
           \en Calculation of the cosine and sine. \~
  \details \ru В 1.7 раза быстрее, чем отдельное вычисление sin и cos, 
    проверено под release с оптимизацией.
           \en 1.7 times faster than a single calculation of sin and cos, 
    Tested in release configuration with optimization. \~
  \param[in] tt - \ru Угол (в радианах).
                  \en Angle (in radians). \~
  \param[out] cosT - \ru Косинус угла tt.
                     \en Cosine of the angle tt. \~
  \param[out] sinT - \ru Синус угла tt.
                     \en Sine of the angle tt. \~
  \ingroup Base_Algorithms
*/
// ---
inline void GetCosSin( const double & tt, double & cosT, double & sinT )
{
// A single approach for PLATFORM_64 / !PLATFORM_64
//#if !defined(PLATFORM_64) && defined(_MSC_VER)  // Use inline _asm
//  __asm {
//    mov     eax, tt
//    mov     ebx, cosT
//    mov     ecx, sinT
//    FLD     qword ptr [eax]
//    FSINCOS
//    FSTP    qword ptr [ebx]
//    FSTP    qword ptr [ecx]
//  }
//#else // PLATFORM_64
  cosT = ::cos( tt );
  sinT = ::sin( tt );
//#endif // PLATFORM_64
}

//-------------------------------------------------------------------------------
// \ru Погасить отладочное требование. \en . Mute debug assert.
// ---
inline void DummyAssert( bool ) {
}

  namespace older_stl_support
  {
    //------------------------------------------------------------------------------
    /** \brief \ru Суррогат "iterator erase( iterator pos )" для старых версий std:map и std::set.
               \en "iterator erase( iterator pos )" surrogate for older versions of std:map and std:: set.. \~
      \details \ru Для унификации кода при поддержке версий std:map и std::set,
                   в которых не реализована iterator erase( iterator pos ). \n
               \en To unify the code with support for STD:map and std::set versions
                   that do not implement iterator erase( iterator pos). \n \~
      \param[in] obj - \ru Контейнер из которого удаляется элемент.
                       \en Container that the item is being removed from. \~
      \param[in] pos - \ru Итератор на удаляемый элемент.
                       \en Iterator to the element to remove. \~
      \return \ru Итератор на элемент, следующий за удаленным.
              \en Iterator following the removed element. \~
      \ingroup Base_Tools
    */
    // ---
    template<class T> inline typename T::iterator erase( T& obj, typename T::iterator pos )
    {
    #if defined(__GNUC__) && ( C3D_GCC_VERSION < 40805 )
      typename T::iterator temp = pos;
      pos++;
      obj.erase( temp );
      return pos;
    #else
      return obj.erase( pos );
    #endif
    }
  }


} // namespace C3D


//-------------------------------------------------------------------------------
// \ru Работа с OpenMP. // \en Work with OpenMP.
// ---
#ifndef _OPENMP
//-------------------------------------------------------------------------------
// \ru Заглушки для OpenMP. \en Stubs for OpenMP.
// ---
#if defined(__cplusplus)
extern "C" {
#endif  // __cplusplus
  typedef struct{ void * _lk; } omp_lock_t;
  typedef struct{ void * _lk; } omp_nest_lock_t;
  inline void   CALL_DECLARATION omp_set_num_threads   ( int )              {}
  inline int    CALL_DECLARATION omp_get_num_threads   ( void )             { return 1; }
  inline int    CALL_DECLARATION omp_get_max_threads   ( void )             { return 1; }
  inline int    CALL_DECLARATION omp_get_thread_num    ( void )             { return 0; }
  inline int    CALL_DECLARATION omp_get_num_procs     ( void )             { return 1; }
  inline void   CALL_DECLARATION omp_set_dynamic       ( int )              {}
  inline int    CALL_DECLARATION omp_get_dynamic       ( void )             { return 0; }
  inline int    CALL_DECLARATION omp_in_parallel       ( void )             { return 0; }
  inline void   CALL_DECLARATION omp_set_nested        ( int )              {}
  inline int    CALL_DECLARATION omp_get_nested        ( void )             { return 0; }
  inline void   CALL_DECLARATION omp_init_lock         ( omp_lock_t* )      {}
  inline void   CALL_DECLARATION omp_destroy_lock      ( omp_lock_t* )      {}
  inline void   CALL_DECLARATION omp_set_lock          ( omp_lock_t* )      {}
  inline void   CALL_DECLARATION omp_unset_lock        ( omp_lock_t* )      {}
  inline int    CALL_DECLARATION omp_test_lock         ( omp_lock_t* )      { return 1; }
  inline void   CALL_DECLARATION omp_init_nest_lock    ( omp_nest_lock_t* ) {}
  inline void   CALL_DECLARATION omp_destroy_nest_lock ( omp_nest_lock_t* ) {}
  inline void   CALL_DECLARATION omp_set_nest_lock     ( omp_nest_lock_t* ) {}
  inline void   CALL_DECLARATION omp_unset_nest_lock   ( omp_nest_lock_t* ) {}
  inline int    CALL_DECLARATION omp_test_nest_lock    ( omp_nest_lock_t* ) { return 1; }
  inline double CALL_DECLARATION omp_get_wtime         ( void )             { return 1.0; }
  inline double CALL_DECLARATION omp_get_wtick         ( void )             { return 1.0; }
#if defined(__cplusplus)
}
#endif  // __cplusplus
#else  // _OPENMP
  #include <omp.h>
#endif // _OPENMP

#endif // MATH_DEFINE_H
