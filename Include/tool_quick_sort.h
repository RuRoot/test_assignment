////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Функция сортировки.
         \en A sorting function. \~

*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// \ru Стандартная функция void qsort( void *base, size_t num, size_t width, int ( *compare )(const void *elem1, const void *elem2 ) ) 
//     определенная в <stdlib.h> and <search.h> может работать неверно на массивах длиной <= 8. Пожалуйста, избегайте ее применения. 
// \en Standard function   void qsort( void *base, size_t num, size_t width, int ( *compare )(const void *elem1, const void *elem2 ) ) 
//     defined in <stdlib.h> and <search.h> may work uncorrectly in arrays with length <= 8. Please, avoid using of this. 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __TOOL_QUICK_SORT_H
#define __TOOL_QUICK_SORT_H


#include <io_define.h>
#include <math_define.h>


//------------------------------------------------------------------------------
// \ru Функция сравнения \en A comparison function 
//---
typedef int (CALL_DECLARATION *KsQSortCompFunc)( const void*, const void* );


namespace c3d // namespace C3D
{

//------------------------------------------------------------------------------
 /** \brief \ru Поменять местами два элемента. \en Swap two elements.
   \details \ru Поменять местами два элемента размером width. \en Swap two elements of 'width' size.
   \param[in] a -     \ru Указатель на элемент массива. \en Pointer to the array element. \~
   \param[in] b -     \ru Указатель на элемент массива. \en Pointer to the array element. \~
   \param[in] width - \ru Размер каждого элемента в байтах. \en Size in bytes of each element. \~
   */
//---
static void CALL_DECLARATION swap( char * a, char * b, size_t width ) 
{
  if ( a != b ) {
    char tmp;
    // \ru Менять местами по одному символу, чтобы избежать возможных проблем выравнивания.
    // \en Swap one character at a time to avoid potential alignment problems.
    while ( width-- ) {
        tmp = *a;
        *a++ = *b;
        *b++ = tmp;
    }
  }
}

} // namespace C3D


//------------------------------------------------------------------------------
 /** \brief \ru Сортировка массива.
            \en An array sorting.  \~
   \details \ru Реализует быструю сортировку. Сортирует на месте.
            \en Implements a quicksort of the array of elements. Sorts in place. \~
    \param[out] base -  \ru Указатель на основание массива. \en Pointer to the base of the array. \~
    \param[in] num  -   \ru Количество элементов массиве. \en Number of elements in the array. \~
    \param[in] width -  \ru Размер элемента массива в байтах. \en Size in bytes of the array element. \~
    \param[in] compareFunc - \ru Указатель на функцию сравнения элементов первого массива.
                        Аналог функции strcmp, предоставляемый пользователем для сравнения элементов массива.
                        Принимает 2 указателя на элементы и возвращает:
                        отрицательное значение, если 1<2; 0, если 1=2; положительное значение, если 1>2. \~
                        \en Pointer to the comparison function for the elements of the first array.
                        Analog of strcmp for strings, supplied by user for comparing the array elements.
                        Accepts 2 pointers to elements and returns:
                        negative value, if 1<2; 0, if 1=2; positive value, if 1>2. \~
*/
//---
inline void KsQSort( void          * base, 
                     size_t          num, 
                     size_t          width, 
                     KsQSortCompFunc compareFunc )
{
  // \ru Границы сортируемого подмассива. \en Boundaries of a subarray currently being sorted.
  char *lowElem, *hiElem;
  // \ru Серединный элемент сортируемого подмассива. \en Middle element of a subarray currently being sorted.
  char *midElem;
  // \ru Бегущие указатели для разбиения подмассива. \en Traveling cursors for a subarray partitioning.
  char *lowCursor, *hiCursor;
  // ru Размер подмассива. \en Size of a subarray.
  size_t size;
  // \ru Стек для хранения границ сортируемых подмассивов. Количество требуемых записей стека <= 1 + log2(size),
  // поэтому для сортировки любого массива достаточно глубины 30.
  // \en Stack to store the boundaries of subarrays to be sorted. Number of stack entries required is <= 1 + log2(size),
  // so the depth of 30 is sufficient for sorting any array.
  char *lowStack[30], *hiStack[30];
  // \ru Текущий указатель стека. // \en Current stack pointer.
  int stackPtr = -1; // \ru Пустой стек. \en Empty stack.

  if (num < 2 || width == 0)
      return;

  lowElem = static_cast<char *>( base );
  hiElem = static_cast<char *>( base ) + width * (num-1);

  if ( num == 2 ) {
    if ( compareFunc( lowElem, hiElem ) >= 0 )
      c3d::swap( lowElem, hiElem, width );
    return;
  }

  // \ru Точка входа псевдо-рекурсии. Сортируется подмассив между lowElem и hiElem (включительно).
  // \en Entry point of a pseudo-recursion. Sort a subarray between lowElem and hiElem (inclusive).
  for ( ;;) {

    size = ( hiElem - lowElem ) / width + 1; // \ru Количество сортируемых элементов. \en Number of elements to sort.

    // ru В качестве разделяющего выбрать серединный элемент и поместить его в начало подмассива.
    // \en Choose a middle element as a partitioning one and swap it to the beginning of the subarray.
    midElem = lowElem + ( size / 2 ) * width;
    c3d::swap( midElem, lowElem, width );

    lowCursor = lowElem;
    hiCursor = hiElem + width;

    // \ru hiCursor уменьшается, а lowCursor увеличивается на каждой итерации, поэтому цикл должен закончиться.
    // \en hiCursor decreases and lowCursor increases on every iteration, so loop must terminate.
    for ( ;;) {
      do {
        lowCursor += width;
      } while ( lowCursor <= hiElem && compareFunc( lowCursor, lowElem ) <= 0 );

      do {
        hiCursor -= width;
      } while ( hiCursor > lowElem && compareFunc( hiCursor, lowElem ) >= 0 );

    // \ru Цикл закончился. \en The loop terminates.
      if ( hiCursor < lowCursor )
        break;

      c3d::swap( lowCursor, hiCursor, width );
    }

    // ru Возвратить разделяющий элемент на место. /en Put the partition element in place.
    c3d::swap( lowElem, hiCursor, width );

    // \ru Теперь будут сортироваться подмассивы [lowElem, hiCursor-1] и [lowCursor, hiElem].
    // Сначала меньшие по размеру, чтобы минимизировать глубину стека.
    // \en Now sort the subarrays [lowElem, hiCursor-1] and [lowCursor, hiElem]].
    // The smaller one first to minimize stack usage.
    if ( hiCursor - 1 - lowElem >= hiElem - lowCursor ) {
      // \ru Сохранить больший подмассив для последующей обработки. \en Save the bigger subarray for later processing.
      if ( lowElem + width < hiCursor ) {
        lowStack[++stackPtr] = lowElem;
        hiStack[stackPtr] = hiCursor - width;
      }

      // \ru Обработать меньший подмассив. \en Process smaller subarray.
      if ( lowCursor < hiElem ) {
        lowElem = lowCursor;
        continue;
      }
    }
    else {
      // \ru Сохранить больший подмассив для последующей обработки. \en Save the bigger subarray for later processing.
      if ( lowCursor < hiElem ) {
        lowStack[++stackPtr] = lowCursor;
        hiStack[stackPtr] = hiElem;
      }

      // \ru Обработать меньший подмассив. \en Process smaller subarray.
      if ( lowElem + width < hiCursor ) {
        hiElem = hiCursor - width;
        continue;
      }
    }

    // \ru Текущий подмассив отсортирован. Проверить наличие отложенных сортировок в стеке.
    // \en Current subarray have been sorted. Check for any pending sorts on the stack.
    if ( stackPtr < 0 )
      break; // \ru Все подмассивы обработаны. \en All subarrays are done.
    lowElem = lowStack[stackPtr];
    hiElem = hiStack[stackPtr--];
  }
}

//------------------------------------------------------------------------------
 /** \brief \ru Перестановка 2 элементов в массиве. \en Swapping of 2 elements in the array. \~
   \details \ru Переставляются заданные элементы массива. \en Swapping of given elements in the array. \~
   \param[out] base - \ru Указатель на массив. \en Pointer to the array. \~
   \param[in] ind1 -  \ru Индекс первого элемента. \en Index of the first element. \~
   \param[in] ind2 -  \ru Индекс второго элемента. \en Index of the second element. \~
\ingroup Base_Algorithms
*/
//---
template<class Type>
void Swap( Type* arr, size_t ind1, size_t ind2 )
{
  Type tmp = arr[ind1];
  arr[ind1] = arr[ind2];
  arr[ind2] = tmp;
}

//------------------------------------------------------------------------------
/** \brief \ru Сортировка массива с возможностью синхронной перестановки элементов в двух других массивах.
           Эффективна для небольших массивов.
           \en An array sorting with an ability of synchronous rearrangement in two other specified arrays.
           Effective for small arrays.\~
  \details \ru Первый массив сортируется по возрастанию параметра. Элементы второго и третьего массивов
           переставляются синхронно с элементами первого. Подразумевается, что массивы имеют одинаковый размер.
           \en First array is sorted in ascending order. Elements of the second and third arrays
           are rearranged synchronously with the first one. Assumed that arrays have the same size. \~
  \param[out] base -       \ru Указатель на первый массив, который требуется отсортировать.
                           \en Pointer to the first array to sort. \~
  \param[in] num   -       \ru Количество элементов в массиве.
                           \en Number of elements in the array. \~
  \param[in] compareFunc - \ru Указатель на функцию сравнения элементов первого массива.
                           Аналог функции strcmp, предоставляемый пользователем для сравнения элементов массива.
                           Принимает 2 указателя на элементы и возвращает:
                           отрицательное значение, если 1<2; 0, если 1=2; положительное значение, если 1>2. \~
                           \en Pointer to the comparison function for the elements of the first array.
                           Analog of strcmp for strings, supplied by user for comparing the array elements.
                           Accepts 2 pointers to elements and returns:
                           negative value, if 1<2; 0, if 1=2; positive value, if 1>2. \~
  \param[out] base2 -      \ru Указатель на второй массив (может быть nullptr).
                           \en Pointer to the second array (could be nullptr). \~
  \param[out] base3 -      \ru Указатель на третий массив (может быть nullptr).
                           \en Pointer to the third array (could be nullptr). \~
\ingroup Base_Algorithms
*/
//---
template <class Type, class Type2, class Type3 >
void InsertSort( Type * base,
                 size_t num,
                 KsQSortCompFunc compareFunc, Type2* base2 = nullptr, Type3* base3 = nullptr )

{
  if ( num < 2 )
    return;

  if ( num == 2 ) {
    if ( compareFunc( base, base + 1 ) >= 0 ) {
      Swap<Type>( base, 0, 1 );
      if ( base2 != nullptr ) {
        Swap<Type2>( base2, 0, 1 );
        if ( base3 != nullptr )
          Swap<Type3>( base3, 0, 1 );
      }
    }
    return;
  }

  for ( ptrdiff_t i = 1; i < static_cast<ptrdiff_t>(num); ++i ) {
    for ( ptrdiff_t j = i; j > 0 && compareFunc( base + j - 1, base + j ) >= 0; j-- ) {
      Swap<Type>( base, j - 1, j );
      if ( base2 != nullptr ) {
        Swap<Type2>( base2, j - 1, j );
        if ( base3 != nullptr )
          Swap<Type3>( base3, j - 1, j );
      }
    }
  }
}

//------------------------------------------------------------------------------
 /** \brief \ru Сортировка массива с возможностью синхронной перестановки элементов в двух других массивах.
            Работает с массивами элементов, которые предоставляют оператор присваивания.
            Не гарантирует сохранение порядка равных элементов.
            \en An array sorting with an ability of synchronous rearrangement in two other specified arrays.
            Works with arrays of elements which support assignment operators.
            Not guarantees preserving of the order of equal elements. \~
   \details \ru Первый массив сортируется по возрастанию параметра.
            Элементы второго и третьего массивов переставляются синхронно с элементами первого. 
            Подразумевается, что массивы имеют одинаковый размер.
            \en First array is sorted in ascending order.
            Elements of the second and third arrays are rearranged synchronously with the first one.
            Assumed that arrays have the same size. \~
   \param[out] base -       \ru Указатель на первый массив, который требуется отсортировать.
                            \en Pointer to the first array to sort. \~
   \param[in] num   -       \ru Количество элементов в массиве.
                            \en Number of elements in the array. \~
   \param[in] compareFunc - \ru Указатель на функцию сравнения элементов первого массива.
                            Аналог функции strcmp, предоставляемый пользователем для сравнения элементов массивы.
                            Принимает 2 указателя на элементы и возвращает:
                              отрицательное значение, если 1<2;
                              0, если 1=2,
                              положительное значение, если 1>2. \~
                            \en Pointer to the comparison function for the elements of the first array. 
                            Analog of strcmp for strings, supplied by user for comparing the array elements.
                            Accepts 2 pointers to elements and returns:
                              negative value, if 1<2;
                              0, if 1=2,
                              positive value, if 1>2. \~
   \param[out] base2 -      \ru Указатель на второй массив (может быть nullptr).
                            \en Pointer to the second array (could be nullptr). \~
   \param[out] base3 -      \ru Указатель на третий массив (может быть nullptr).
                            \en Pointer to the third array (could be nullptr). \~
\ingroup Base_Algorithms
*/
//---
template <class Type, class Type2, class Type3 >
void QuickSort( Type * base,
                size_t num,
                KsQSortCompFunc compareFunc, Type2* base2 = nullptr, Type3* base3 = nullptr )
{
  #define QSORT_THRESHOLD 25       // \ru Порог перехода на другой тип сортировки.\en Threshold of transition to another sorting.

  ptrdiff_t lInd = 0, rInd = 0;    // \ru Промежуточные левый и правый индексы.\en Intermediate left and right indices.
  ptrdiff_t leftIndex, rightIndex; // \ru Текущие левый и правый индексы. \en Current left and right indices.
  ptrdiff_t midIndex;              // \ru Текущий базовый индекс.\en Current base indices.
  c3d::NumbersPair iterStack[30];
  int stackCount = 0;

  if ( num < 2 )
    return;

  if ( num == 2 ) {
    if ( compareFunc( base, base + 1 ) >= 0 ) {
      Swap<Type>( base, 0, 1 );
      if ( base2 != nullptr ) {
        Swap<Type2>( base2, 0, 1 );
        if ( base3 != nullptr )
          Swap<Type3>( base3, 0, 1 );
      }
    }
    return;
  }

  if ( num <= QSORT_THRESHOLD )
    return InsertSort<Type,Type2,Type3>( base, num, compareFunc, base2, base3 );

  iterStack[stackCount] = std::make_pair( 0, num - 1 );

  while ( stackCount >= 0 ) {
    leftIndex = iterStack[stackCount].first;
    rightIndex = iterStack[stackCount--].second;

    // \ru Выбирается базовый элемент (используется средний). \en Select a base element (use the middle one). 
    midIndex = ( rightIndex + leftIndex ) / 2;
    Swap<Type>( base, midIndex, leftIndex );
    if ( base2 != nullptr ) {
      Swap<Type2>( base2, midIndex, leftIndex );
      if ( base3 != nullptr )
        Swap<Type3>( base3, midIndex, leftIndex );
    }

    // \ru Далее массив делится на 3 части:
    //    часть из элементов, которые <= базовому элементу,
    //    часть из элементов, которые == базовому элементу,
    //    часть из элементов, которые >= parts элементу,
    // \en Divide the array into three pieces:
    //    the part of elements which <= the base element,
    //    the part of elements which == the base element,
    //    the part of elements which >= the base element.

    lInd = leftIndex;
    rInd = rightIndex + 1;

    for ( ;;) {
      do {
        lInd++;
      } while ( lInd <= rightIndex && compareFunc( base + lInd, base + leftIndex ) <= 0 );

      do {
        rInd--;
      } while ( rInd > leftIndex && compareFunc( base + rInd, base + leftIndex ) >= 0 );

      if ( rInd < lInd )
        break;

      Swap<Type>( base, lInd, rInd );
      if ( base2 != nullptr ) {
        Swap<Type2>( base2, lInd, rInd );
        if ( base3 != nullptr )
          Swap<Type3>( base3, lInd, rInd );
      }
    }

    Swap<Type>( base, leftIndex, rInd );
    if ( base2 != nullptr ) {
      Swap<Type2>( base2, leftIndex, rInd );
      if ( base3 != nullptr )
        Swap<Type3>( base3, leftIndex, rInd );
    }

    // \ru Теперь будут сортироваться подмассивы [leftIndex, rInd-1] и [lInd, rigthIndex].
    // Сначала меньшие по размеру, чтобы минимизировать глубину стека.
    // \en Now sort the subarrays [leftIndex, rInd-1] and [lInd, rigthIndex].
    // The smaller one first to minimize stack usage.
    if ( rInd - 1 - leftIndex >= rightIndex - lInd ) {
      if ( leftIndex + 1 < rInd ) {
        ptrdiff_t count = rInd - leftIndex;
        if ( count <= QSORT_THRESHOLD )
          InsertSort<Type, Type2, Type3>( base + leftIndex, count, compareFunc, base2, base3 );
        else
          iterStack[++stackCount] = std::make_pair( leftIndex, rInd - 1 );
      }

      if ( lInd < rightIndex ) {
        ptrdiff_t count = rightIndex - lInd + 1;
        if ( count <= QSORT_THRESHOLD )
          InsertSort<Type, Type2, Type3>( base + lInd, count, compareFunc, base2, base3 );
        else
          iterStack[++stackCount] = std::make_pair( lInd, rightIndex );
      }
    }
    else {
      if ( lInd < rightIndex ) {
        ptrdiff_t count = rightIndex - lInd + 1;
        if ( count <= QSORT_THRESHOLD )
          InsertSort<Type, Type2, Type3>( base + lInd, count, compareFunc, base2, base3 );
        else
          iterStack[++stackCount] = std::make_pair( lInd, rightIndex );
      }

      if ( leftIndex + 1 < rInd ) {
        ptrdiff_t count = rInd - leftIndex;
        if ( count <= QSORT_THRESHOLD )
          InsertSort<Type, Type2, Type3>( base + leftIndex, count, compareFunc, base2, base3 );
        else
          iterStack[++stackCount] = std::make_pair( leftIndex, rInd - 1 );
      }
    }
  } // while
}

//------------------------------------------------------------------------------
// \ru Функция для сортировки элементов double по возрастанию. \en A function for sorting of double elements in ascending order.
// ---
inline int DoubleCompare( const double * first, const double * second ) {
  return *second < *first ? 1 : -1;
}

//------------------------------------------------------------------------------
/// \ru Функция автоматического наращивания памяти \en A function of automatic allocating of the memory 
/**\ru Функция применяется в библиотеке шаблонов Sys для автоматического наращивания памяти SArray, RPArray, SQueue ...
   \en This function is used for the template library Sys for the automatic allocation of the memory in  SArray, RPArray, SQueue ... \~
*/
//---
inline size_t KsAutoDelta( size_t count )
{ 
  return std_min( static_cast<size_t>(1024), std_max( static_cast<size_t>(4), count / 8) ); //-V112
} 


//------------------------------------------------------------------------------
// \ru Вычисление автоматического выделения памяти массива \en Calculating of the automatic memory allocation of an array. 
//  (this avoids heap fragmentation in many situations)
//---
inline size_t KsAutoDelta( size_t count, uint16 delta )
{ 
  return ( delta > 2 ) ? delta : KsAutoDelta( count );
} 


//------------------------------------------------------------------------------
// \ru тест на запрос распределения памяти в пределах половины адресного пространства для 32- и 64-разрядного приложения \en a test for the request of memory allocation inside a half of address space for 32- and 64-bit applications. 
// ---
inline bool TestNewSize( size_t sizeOfType, size_t count )
{
  return ( double(count) * double(sizeOfType) < double(SYS_MAX_ST) );
}


#endif // __TOOL_QUICK_SORT_H
