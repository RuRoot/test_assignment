////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Вспомогательные общие функции.
         \en Common auxiliary functions. \~

*/
////////////////////////////////////////////////////////////////////////////////


#ifndef __ALG_BASE_H
#define __ALG_BASE_H

#include <templ_s_array.h>
#include <templ_array2.h>
#include <mb_rect.h>
#include <mb_placement3d.h>
#include <mb_cart_point.h>
#include <mb_cart_point3d.h>
#include <mb_enum.h>
#include <math_version.h>
#include <limits.h>
#include <vector>


class    MbCurve3D;
class    MbMatrix3D;

namespace c3d // namespace C3D
{

//------------------------------------------------------------------------------
/** \brief \ru Вычислить угол между прямой и осью 0X.
           \en Calculate angle between line and 0X-axis. \~
  \details \ru Прямая задается приращениями dx, dy. Угол лежит в интервале [0, 2*M_PI).
           \en Line is set by dx and dy increments. Angle is in range [0, 2*M_PI). \~
  \param[in] dx - \ru Приращение по X.
                  \en Increment along X. \~
  \param[in] dy - \ru Приращение по Y.
                  \en Increment along Y. \~
  \return \ru Искомый угол.
          \en The required angle. \~
  \ingroup Algorithms_2D
*/ // ---
inline
double CalcAngle0X( double dx, double dy )
{
  if ( ::fabs(dx) < NULL_EPSILON && ::fabs(dy) < NULL_EPSILON )
    return 0.0;

  double angle = atan2( dy, dx );
  if ( ::fabs(angle) < DOUBLE_REGION )
    angle = 0.0;
  return angle < 0.0 ? M_PI2 + angle : angle;
}


//------------------------------------------------------------------------------
/** \brief \ru Вычислить угол между прямой и осью 0X.
           \en Calculate the angle between a line and 0X-axis. \~
  \details \ru Прямая задается приращениями dx, dy. Угол лежит в интервале [0, 2*M_PI).
           \en A line is defined by dx and dy increments. The angle is in range [0, 2*M_PI). \~
  \param[in] dx - \ru Приращение по X.
                  \en Increment along X. \~
  \param[in] dy - \ru Приращение по Y.
                  \en Increment along Y. \~
  \return \ru Искомый угол.
          \en The required angle. \~
  \ingroup Algorithms_2D
*/ // ---
inline
long double CalcAngle0X( long double dx, long double dy )
{
  if ( ::fabsl(dx) < NULL_EPSILON && ::fabsl(dy) < NULL_EPSILON )
    return 0.0;

  long double angle = atan2( dy, dx );
  if ( ::fabsl(angle) < DOUBLE_REGION )
    angle = 0.0;
  return angle < 0.0 ? M_PI2 + angle : angle;
}


//------------------------------------------------------------------------------
/** \brief \ru Вычислить угол между прямой и осью 0X.
           \en Calculate the angle between a line and 0X-axis. \~
  \details \ru Прямая задается двумя точками. Угол лежит в интервале [0, 2*M_PI].
           \en Line is defined by two points. The angle is in range [0, 2*M_PI]. \~
  \param[in] p1 - \ru Первая точка.
                  \en The first point. \~
  \param[in] p2 - \ru Вторая точка.
                  \en The second point \~
  \return \ru Искомый угол.
          \en The required angle. \~
  \ingroup Algorithms_2D
*/ // ---
inline
double CalcAngle0X( const MbCartPoint & p1, const MbCartPoint & p2 ) {
  return c3d::CalcAngle0X( p2.x - p1.x, p2.y - p1.y );
}


//------------------------------------------------------------------------------
/** \brief \ru Нормализовать угол.
           \en Normalize an angle. \~
  \details \ru Исходный угол, если требуется, загоняется в интервал [0, 2*M_PI).
           \en Draw the source angle in range [0, 2*M_PI) if necessary. \~
  \param[out] angle - \ru Исходный угол, который требуется нормализовать.
                      \en The source angle to normalize. \~
  \param[in] angleEpsilon - \ru Погрешность угла.
                            \en Angular tolerance. \~
  \ingroup Base_Algorithms
*/ // ---
inline
double & NormalizeAngle( double & angle, double angleEpsilon = Math::AngleEps )
{
  if ( ::fabs( angle ) < angleEpsilon || ::fabs( angle - M_PI2 ) < angleEpsilon )
    angle = 0.0;
  else {
    while ( angle > M_PI2 ) angle -= M_PI2;
    while ( angle < 0     ) angle += M_PI2;
  }
  return angle;
}


//------------------------------------------------------------------------------
/** \brief \ru Перевести параметр окружности в параметр кривой.
           \en Transform a circle angle to the curve parameter. \~
  \param[in] dir - \ru Угол между осью X окружности и осью X системы координат.
                   \en Angle between X-axes of a circle and of the coordinate system. \~
  \param[in] left - \ru Если true, то левая система координат, иначе - правая.
                    \en If true, then the coordinate system is left, otherwise it is right. \~
  \param[out] t - \ru Параметр, который требуется преобразовать.
                  \en Parameter to transform. \~
  \ingroup Algorithms_2D
*/ // ---
inline
void AngleToParam( double dir, bool left, double & t ) 
{
  if ( ::fabs(dir) > EPSILON || left )
    t = left ? M_PI2 - ( t - dir ) : t - dir;
  c3d::NormalizeAngle( t );
}


//------------------------------------------------------------------------------
/** \brief \ru Вычислить угол между двумерными векторами.
           \en Calculate the angle between two-dimensional vectors. \~
  \details \ru Вычислить угол между двумерными векторами. \n
           \en Calculate the angle between two-dimensional vectors. \n \~
  \param[in] v1 - \ru Вектор 1.
                  \en The first vector. \~
  \param[in] v2 - \ru Вектор 2.
                  \en The second vector. \~
  \return \ru Величину угла.
          \en The angle. \~
  \ingroup Base_Algorithms
*/ // ---
template <class Type>
double AngleBetweenVectors( const Type & v1, const Type & v2 )
{
  double dy = v1 | v2;
  double dx = v1 * v2;

  if ( ::fabs(dx) < NULL_EPSILON && ::fabs(dy) < NULL_EPSILON )
    return 0;

  double angle = ::atan2( dy, dx );
  if ( ::fabs(angle) < DOUBLE_REGION )
    angle = 0.0;

  return angle;
}


//------------------------------------------------------------------------------
/** \brief \ru Вычислить минимальный угол между прямыми.
           \en Calculate the minimal angle between lines. \~
  \details \ru Прямые задаются следующим образом: l1( centre, p1 ) и l2( centre, p3 ).
           \en Lines are specified as follows: l1( centre, p1 ) and l2( centre, p3 ). \~
  \param[in] p1 - \ru Точка прямой 1.
                  \en A point on the first line. \~
  \param[in] centre - \ru Общая точка двух прямых.
                      \en The common point of two lines. \~
  \param[in] p3 - \ru Точка прямой 2.
                  \en A point on the second line. \~
  \return \ru Результат со знаком: \n
    "+" - p3 находится слева от вектора ( centre, p1 );
    "-" - p3 находится справа от вектора ( centre, p1 ).
          \en Signed result: \n
    "+" - p3 lies to the left of vector ( centre, p1 );
    "-" - p3 lies to the right of vector ( centre, p1 ). \~
  \ingroup Algorithms_2D
*/ // ---
inline
double CalcAngle3Points( const MbCartPoint & p1, const MbCartPoint & centre, const MbCartPoint & p3 ) {
  return c3d::AngleBetweenVectors( MbVector( centre, p1 ), MbVector( centre, p3 ) );
}


//------------------------------------------------------------------------------
/** \brief \ru Найти параметр в массиве.
           \en Find a parameter in the array. \~
  \details \ru Находится индекс заданного параметра в массиве. Входной массив должен быть 
    отсортирован по возрастанию параметров. Если задать начальное значение индекса 
    близко к предполагаемому нахождению искомого параметра, поиск будет выполняться быстрее.
    Если в массиве нет параметра равного заданному, то найденным будет считаться 
    первый параметр массива больший заданного. Если в массиве будет 1 элемент, 
    то он будем считаться равным заданному.
           \en Locating the index of a given parameter in the array. Input array is to be 
    sorted in ascending order. If the initial value of the index is set 
    to be close to the expected location of the sought parameter, the search will be faster.
    If there is no parameter in the array equal to the given one, then 
    the first parameter in the array greater than the given one will be chosen. If there is only 1 element in the array, 
    then it will be considered as equal to the given one. \~
  \param[in] arParam - \ru Множество параметров.
                       \en An array of parameters. \~
  \param[in] t - \ru Значение параметра, которое требуется найти в массиве.
                 \en The parameter value to be found in the array. \~
  \param[in,out] id - \ru Индекс найденного параметра в массиве.
                      \en The index of the parameter found in the array. \~
  \return \ru - true, если в массиве есть элементы, \n иначе false
          \en - true, if there are elements in the array, \n otherwise false \~
  \ingroup Base_Algorithms
*/ // ---
template <class ParamsVector>
bool ArFind( const ParamsVector & arParam, double t, ptrdiff_t & id )
{
  bool bRes = (arParam.size() > 0);

  if ( bRes ) { // \ru Если массив не пуст \en If the array is not empty     
    ptrdiff_t idLeft, idRight, rangeId;

    // \ru Если предыдущий индекс нормальный \en If the previous index is normal 
    if ( id >= 0 && ((size_t)id < arParam.size()) ) { // SKIP_SA

      // \ru Используем эту информацию для оптимизации \en Use this information for optimization 
      if ( arParam[id] < t ) { // \ru Если значение в таблице строго меньше t \en If value in table strictly less than t 
        idLeft  = id; // \ru Установить левую границу \en Set the left bound 
        idRight = id + 8; // \ru Установить правую вблизи левой \en Set the right bound close to the left one 
        rangeId = 8; // \ru Установить диапазон по умолчанию \en Set the default range 

        if ( idRight > (ptrdiff_t)arParam.size() ) { // \ru Если правая больше или ровна максимальной \en If the right bound is greater than or equal to the maximum 
          idRight = arParam.size(); // \ru Установить правую максимальной \en Set the right bound to maximum 
          rangeId = idRight - idLeft; // \ru Вычислить новый диапазон \en Calculate the new range 
        }
        else {
          // \ru Если ближайшая правая не правая \en If the nearest right bound is not right 
          if ( idRight != (ptrdiff_t)arParam.size() && arParam[idRight] < t ) {
            idLeft  = idRight; // \ru Установить новую левую \en Set the new left bound 
            idRight = arParam.size(); // \ru Установить правую равной максимальной \en Set right bound to maximum 
            rangeId = idRight - idLeft; // \ru Вычислить новый диапазон \en Calculate the new range 
          }
        }
      }
      else {
        idLeft  = id - 8; // \ru Установить левую вблизи правой \en Set the left bound close to the right one 
        idRight = id; // \ru Установить правую \en Set the right bound 
        rangeId = 8; // \ru Установить диапазон по умолчанию \en Set the default range 

        if ( idLeft < 0 ) { // \ru Если локальная левая меньше минимальной \en If the local left bound is less than minimum 
          idLeft  = 0; // \ru Сделать левую минимальной \en Set the left bound to minimum 
          rangeId = id; // \ru Вычислить новый диапазон \en Calculate the new range 
        }
        else {
          if ( arParam[idLeft] >= t ) { // \ru Если локальная левая не левая \en If the local left bound is not the left 
            idRight = idLeft; // \ru Установить новую правую \en Set new right bound 
            idLeft  = 0; // \ru Установить левую минимальной \en Set the left bound to minimum 
            rangeId = idRight - idLeft; // \ru Вычислить новый диапазон \en Calculate the new range 
          }
        }
      }
    }
    else {
      idLeft  = 0; // \ru Левая минимальная \en The left bound is minimum 
      idRight = arParam.size(); // \ru Правая максимальная \en The right bound is maximum 
      rangeId = arParam.size(); // \ru Выч. новый диапазон \en Calculate the new range 
    }

    while ( rangeId > 8 ) { // \ru До тех пор пока диапазон больше восьми \en Until the range contains more than eight elements 
      size_t mIndex = ( rangeId / 2 ) + idLeft; // \ru Найти индекс в середине диапазона \en Find the index in the middle of the range 

      if ( arParam[mIndex] < t ) // \ru Если t больше серединного параметра \en If t is greater than the middle parameter 
        idLeft = mIndex; // \ru Установить левую границу \en Set the left bound 
      else 
        idRight = mIndex; // \ru Иначе правую \en Otherwise the right one 

      rangeId = idRight - idLeft; // \ru Найти новый диапазон \en Find the new range 
    }

    id = idRight; // \ru Установить индекс \en Set the index 

    // \ru Пройти от левой до правой границы \en Pass from the left bound to the right bound 
    for ( ptrdiff_t i = idLeft; i < idRight; i++ ) {
      if ( arParam[i] < t ) // \ru Если текущий параметр меньше искомого \en If the current parameter is less than the required one, 
        continue; // \ru Продолжить цикл \en Continue the loop 
      id = i;
      break;
    }
  }

  return bRes;
}


//------------------------------------------------------------------------------
/** \brief \ru Разделить отрезок пополам.
           \en Split segment by the middle. \~
  \param[in] p1 - \ru Координаты начала отрезка.
                  \en Coordinates of the segment's start point. \~
  \param[in] p2 - \ru Координаты конца отрезка.
                  \en Coordinates of the segment's end point. \~
  \return \ru Координаты середины отрезка.
          \en Coordinates of the segment's middle point. \~
  \ingroup Algorithms_2D
*/ // ---
inline
MbCartPoint LineSegDivide( const MbCartPoint & p1, const MbCartPoint & p2 ) {
  return MbCartPoint( (p1.x + p2.x) * 0.5, (p1.y + p2.y) * 0.5 );
}


//------------------------------------------------------------------------------
/// \ru i по модулю n (циклический вариант). \en I by modulo n (cyclic case). 
// ---
inline
ptrdiff_t mod( ptrdiff_t i, ptrdiff_t n )
{
  ptrdiff_t m = i % n;
  return ( m >= 0 ) ? m : m + n;
}


//------------------------------------------------------------------------------
/// \ru Определение знака вещественного числа. \en Determination of the sign of a real number. 
// ---
inline
int Sign( double a ) { return (a > 0) ? +1 : -1; }


//------------------------------------------------------------------------------
/// \ru Округление вещественного числа. \en Round-off the real number. 
// ---
inline
int Round( double x ) { return (int)( x + ( (x > 0) ? 0.5 : - 0.5 ) ); }


//------------------------------------------------------------------------------
/// \ru Округление вещественного числа. \en Round-off the real number. 
// ---
inline
int32 LRound( double x ) { return (int32)( x + ( (x > 0) ? 0.5 : - 0.5 ) ); }


//------------------------------------------------------------------------------
/// \ru Округление вещественного числа с проверкой \en Round-off the real number with validation. 
// ---
inline
int32 CheckLRound( double x ) { return (x > SYS_MAX_INT32) ? SYS_MAX_INT32 : ((x < SYS_MIN_INT32) ? SYS_MIN_INT32 : LRound(x)); }


//------------------------------------------------------------------------------
/** \brief \ru Лежит ли число в интервале [x1, x2].
           \en Check if the number is in range [x1, x2]. \~
  \details \ru x1 может быть как началом, так и концом интервала, как и x2.
           \en x1 can be both the start and the end of the range, just as x2. \~
  \param[in] x1 - \ru Начало или конец исходного интервала.
                  \en The start or the end of the source range. \~
  \param[in] x2 - \ru Начало или конец исходного интервала.
                  \en The start or the end of the source range. \~
  \param[in] x - \ru Исходное число, которое надо проверить.
                 \en The input number which should be checked. \~
  \return \ru true, если x лежит внутри интервала, \n иначе false.
          \en True if x lies inside range, \n otherwise false. \~
  \ingroup Base_Algorithms
*/ // ---
inline
bool InRange( double x1, double x2, double x )
{
  if ( x1 > x2 )
    return ( x2 - FLT_EPSILON < x ) && ( x < x1 + FLT_EPSILON );

  return ( x1 - FLT_EPSILON < x ) && ( x < x2 + FLT_EPSILON );
}


//------------------------------------------------------------------------------
/** \brief \ru Находится ли параметр в диапазоне кривой.
           \en Check if parameter is in the range of the curve. \~
  \details \ru Диапазон кривой дается областью определения ее параметра [tmin, tmax].
           \en Range of the curve is given by domain of curve parameters [tmin, tmax]. \~
  \param[in] tmin - \ru Минимальное значение параметра.
                    \en Minimal value of parameter. \~
  \param[in] tmax - \ru Максимальное значение параметра.
                    \en Maximal value of parameter. \~
  \param[in] t - \ru Исходный параметр
                 \en Source parameter \~
  \param[in] treg - \ru Точность задания параметра.
                    \en Accuracy of parameter. \~
  \return \ru true, если t лежит внутри интервала [tmin, tmax], \n иначе false.
          \en True if t lies inside the range [tmin, tmax], \n otherwise false. \~
  \ingroup Base_Algorithms
*/ // ---
inline
bool IsParamOn( double tmin, double tmax, double t, double treg ) {
  return ( ((tmin - treg) < t) && (t < (tmax + treg)) ); 
}


//------------------------------------------------------------------------------
/** \brief \ru Лежит ли число в диапазоне [0, x1).
           \en Check if the number is in range [0, x1). \~
  \details x1 >= 0
  \param[in] x1 - \ru Конец исходного интервала.
                  \en End of the source range. \~
  \param[in] x - \ru Исходное число, которое надо проверить.
                 \en The input number which should be checked. \~
  \param[in] eps - \ru Точность.
                   \en Tolerance. \~
  \return \ru true, если x лежит внутри интервала, \n иначе false.
          \en True if x lies inside range, \n otherwise false. \~
  \ingroup Base_Algorithms
*/ // ---
inline
bool InRangePlus( double x1, double x, double eps = FLT_EPSILON ) {
  return ( - eps < x ) && (x < x1 + eps );
}


//------------------------------------------------------------------------------
/** \brief \ru Нормализован ли массив объектов по возрастанию или убыванию.
           \en Whether vector of objects is ascending or descending. \~
  \details \ru Нормализован ли массив объектов по возрастанию или убыванию. \n
           \en Whether vector of items are ascending or descending. \n \~
  \param[in] items - \ru Одномерный массив объектов.
                     \en Vector of objects. \~
  \param[in] isAscending - \ru Проверять на возрастание.
                           \en Check for ascending. \~
  \param[in] allowEqual - \ru Допускать равенство объектов.
                          \en Allow for equality of objects. \~
  \return \ru true, если массив объектов отсортировано по возрастанию (убыванию), \n иначе false
          \en True if vector is ascending (descending), \n otherwise false. \~
  \ingroup Base_Algorithms
*/ //---
template <class TypeVector>
bool IsMonotonic( const TypeVector & items, bool isAscending, bool allowEqual = false )
{
  bool isOk = false;

  size_t cnt = items.size();

  if ( cnt > 1 ) {
    isOk = true;

    if ( allowEqual ) {
      for ( size_t k = 1; k < cnt; ++k ) {
        if ( isAscending && items[k] < items[k-1] ) {
          isOk = false;
          break;
        }
        else if ( !isAscending && items[k] > items[k-1] ) {
          isOk = false;
          break;
        }
      }
    }
    else {
      for ( size_t k = 1; k < cnt; ++k ) {
        if ( isAscending && items[k] <= items[k-1] ) {
          isOk = false;
          break;
        }
        else if ( !isAscending && items[k] >= items[k-1] ) {
          isOk = false;
          break;
        }
      }
    }
  }

  return isOk;
}


//------------------------------------------------------------------------------
/** \brief \ru Лежат ли точки на линии.
           \en Whether points lie on the line. \~
  \details \ru Лежат ли точки на линии (улучшенный вариант IsPointOnLine). \n
           \en Whether points lie on the line (improved variant of IsPointOnLine). \n \~
  \param[in] pnts - \ru Набор точек.
                    \en Point set. \~
  \param[in] metricEps - \ru Точность проверки.
                         \en Check accuracy. \~
  \return \ru true, если точки лежат на линии, \n иначе false
          \en True if points lie on line, \n otherwise false. \~
  \ingroup Base_Algorithms
*/ // ---
template<class Point, class Vector, class PointsVector>
bool ArePointsOnLine( const PointsVector & pnts, double metricEps = METRIC_EPSILON )
{
  bool onLine = false;

  size_t cnt = pnts.size();
  double lenEps = std_min( LENGTH_EPSILON, metricEps );
  lenEps = std_max( EXTENT_EQUAL, lenEps );

  if ( cnt > 2 ) {
    const Point & pnt0 = pnts[0];

    size_t pos = 1;
    Vector tau( pnt0, pnts[pos] );
    double tauLen = tau.Length();
    if ( tauLen <= lenEps ) {
      for ( size_t k = 2; k < cnt; ++k ) {
        const Point & pntk = pnts[k];
        tau.Init( pnt0, pntk );
        tauLen = tau.Length();
        if ( tauLen > lenEps ) {
          pos = k;
          break;
        }
      }
    }

    if ( pos + 1 < cnt ) {
      ++pos;
      const Vector tau0( tau );
      Vector vec;
      for ( size_t k = pos; k < cnt; ++k ) {
        const Point & pntk = pnts[k];
        vec.Init( pnt0, pntk );
        double vecLen = vec.Length();
        if ( vecLen > lenEps && vecLen > tauLen + lenEps ) {
          if ( tau0 * vec < -lenEps )
            vec = -vec;
          tau = vec;
          tauLen = vecLen;
        }
      }
    }

    if ( tauLen > lenEps ) {
      onLine = true;
      tau /= tauLen;
      Vector vect;
      Point pnt;
      for ( size_t k = 1; k < cnt; k++ ) {
        const Point & pntk = pnts[k];
        vect.Init( pnt0, pntk );
        vect = tau * (vect * tau);
        pnt = pnt0 + vect;
        if ( pntk.DistanceToPoint( pnt ) > metricEps ) {
          onLine = false;
          break;
        }
      }
    }
  }
  else if ( cnt == 2 ) {
    if ( pnts[0].DistanceToPoint( pnts[1] ) > lenEps )
      onLine = true;
  }

  return onLine;
}


//------------------------------------------------------------------------------
/** \brief \ru Лежит ли набор точек на плоскости.
           \en Whether the set of points lies on plane. \~
  \details \ru Лежит ли набор точек на плоскости. \n
           \en Whether the set of points lies on plane. \n \~
  \param[in] pnts - \ru Набор точек.
                    \en Point set. \~
  \param[in,out] place - \ru Система координат, в которой лежат точки.
                         \en Points coordinate system. \~
  \param[in] mEps - \ru Точность проверки.
                    \en Check accuracy. \~
  \return \ru true, если точки лежат на плоскости, \n иначе false
          \en True if points lie on plane, \n otherwise false. \~
  \ingroup Base_Algorithms
*/ // ---
template <class SpacePoints>
MATH_FUNC (bool) IsPlanar( const SpacePoints & pnts, MbPlacement3D * place, double mEps = METRIC_EPSILON );



//------------------------------------------------------------------------------
/** \brief \ru Лежит ли набор точек на плоскости.
           \en Whether the set of points lies on plane. \~
  \details \ru Лежит ли набор точек на плоскости. \n
           \en Whether set of points lies on plane. \n \~
  \param[in] pnts - \ru Набор точек.
                    \en Point set. \~
  \param[in,out] place - \ru Система координат, в которой лежат точки.
                         \en Points coordinate system. \~
  \param[in] mEps - \ru Точность проверки.
                    \en Check accuracy. \~
  \return \ru true, если точки лежат на плоскости, \n иначе false
          \en True if points lie on plane, \n otherwise false. \~
  \ingroup Base_Algorithms
*/ // ---
template <class Point>
bool IsPlanar2( const Array2<Point> & pnts, MbPlacement3D * place, double mEps = METRIC_EPSILON )
{
  bool isPlanar = false;
  const size_t lCnt = pnts.Lines();
  const size_t cCnt = pnts.Columns();
  const size_t pCnt = lCnt * cCnt;

  if ( pCnt > 2 ) {
    std::vector<Point> tmpPnts;
    tmpPnts.reserve( pCnt );

    bool isFailed = false;

    for ( size_t i = 0; i < lCnt; ++i ) {
      for ( size_t j = 0; j < cCnt; ++j ) {
        tmpPnts.push_back( pnts( i, j ) );
        if ( !c3d::IsValidPoint( pnts( i, j ) ) ) {
          isFailed = true;
          break;
        }
      }
      if ( isFailed )
        break;
    }

    C3D_ASSERT( !isFailed );

    if ( !isFailed )
      isPlanar = c3d::IsPlanar( tmpPnts, place, mEps );
  }

  return isPlanar;
}


//------------------------------------------------------------------------------
/** \brief \ru Рассчитать габарит набора точек.
           \en Calculate points bounding box. \~
  \details \ru Рассчитать габарит набора точек. \n
           \en Calculate points bounding box. \n \~
  \param[in] pnts - \ru Набор точек.
                    \en Point set. \~
  \param[out] bbox - \ru Габарит.
                     \en Bounding box. \~
  \return \ru true, если габарит рассчитан, \n иначе false
          \en True if bounding box was calculated, \n otherwise false. \~
  \ingroup Base_Algorithms
*/ // ---
template <class PointsVector, class BBox>
bool CalculateBoundingBox( const PointsVector & pnts, BBox & bbox )
{
  bool isDone = false;

  bbox.SetEmpty();

  const size_t pCnt = pnts.size();

  if ( pCnt > 0 ) {
    isDone = true;

    for ( size_t i = 0; i < pCnt && isDone; ++i ) {
      isDone = false;
      if ( c3d::IsValidPoint( pnts[i] ) ) {
        bbox |= pnts[i];
        isDone = true;
      }
    }
    if ( !isDone ) {
      C3D_ASSERT_UNCONDITIONAL( false );
      bbox.SetEmpty();
    }
  }

  return isDone;
}


//------------------------------------------------------------------------------
/** \brief \ru Рассчитать габарит набора точек.
           \en Calculate points bounding box. \~
  \details \ru Рассчитать габарит набора точек. \n
           \en Calculate points bounding box. \n \~
  \param[in] pnts - \ru Набор точек.
                    \en Point set. \~
  \param[out] bbox - \ru Габарит.
                     \en Bounding box. \~
  \return \ru true, если габарит рассчитан, \n иначе false
          \en True if bounding box was calculated, \n otherwise false. \~
  \ingroup Base_Algorithms
*/ // ---
template <class Point, class BBox>
bool CalculateBoundingBox2( const Array2<Point> & pnts, BBox & bbox )
{
  bool isDone = false;

  bbox.SetEmpty();

  const size_t lCnt = pnts.Lines();
  const size_t cCnt = pnts.Columns();
  const size_t pCnt = lCnt * cCnt;

  if ( pCnt > 0 ) {
    isDone = true;

    for ( size_t i = 0; i < lCnt && isDone; ++i ) {
      for ( size_t j = 0; j < cCnt && isDone; ++j ) {
        isDone = false;
        if ( c3d::IsValidPoint( pnts( i, j ) ) ) {
          bbox |= pnts( i, j );
          isDone = true;
        }
      }
    }
    if ( !isDone ) {
      C3D_ASSERT_UNCONDITIONAL( false );
      bbox.SetEmpty();
    }
  }

  return isDone;
}


//------------------------------------------------------------------------------
/** \brief \ru Установить область изменения параметра.
           \en Set the range of parameter. \~
  \details \ru Установить (репараметризовать) область изменения параметра в массиве. \n
           \en Set (reparametrize) range of parameter in the array. \n \~
  \param[in,out] tarr - \ru Множество параметров, упорядоченных по возрастанию параметра.
                        \en The array of parameters sorted in ascending order. \~
  \param[in] tmin - \ru Минимальное значение параметра.
                    \en Minimal value of parameter. \~
  \param[in] tmax - \ru Максимальное значение параметра.
                    \en Maximal value of parameter. \~
  \ingroup Base_Algorithms
*/ // ---
template <class DoubleVector>
void SetLimitParam( DoubleVector & tarr, double tmin, double tmax, double teps = Math::paramEpsilon )
{
  if ( tarr.size() < 2 )
    return;
  if ( tarr.front() == tmin && tarr.back() == tmax )
    return;

  double trange = tmax - tmin; // New parametric range.
                             
  if ( trange < teps )
    return;

  double tmin0 = tarr.front(); // Old minimal parameter.
  double trange0 = tarr.back() - tmin0; // Old parametric range.

  if ( trange0 > teps ) {
    // Set new limits.
    tarr.front() = tmin;
    tarr.back()  = tmax;
    // Reverse slope coefficient.
    double rsc = trange / trange0;
    // Transform values.
    ptrdiff_t mxInd = (ptrdiff_t)tarr.size() - 1;
    for ( ptrdiff_t i = 1; i < mxInd; i++ ) {
      tarr[i] = (tarr[i] - tmin0) * rsc + tmin;
    }
  }
}

} // namespace C3D


//------------------------------------------------------------------------------
/** \brief \ru Отсортировать массив.
           \en Sort the array. \~
  \details \ru Первый массив сортируется по возрастанию параметра. Элементы второго массива 
    переставляются синхронно с элементами первого. Если установлен соответствующий флаг,
    то проводится проверка на наличие в массиве tt0 совпадающих параметров. Если они есть,
    то оставляется один и соответствующий ему в tt2. При этом в tt0 оставляется тот параметр, 
    для которого соответствующий элемент в tt2 имеет минимальное значение.
           \en First array is sorted in ascending order. Elements of the second array 
    are rearranged synchronously with elements of the first one. If the corresponding flag is set,
    then the array is checked for duplications. If they exist,
    then only one of them is kept with the corresponding parameter in tt2. Meanwhile, the parameter is kept in tt0, 
    which correspondence in tt2 has the minimal value. \~
  \param[out] tt0 - \ru Исходный массив 1 параметров, который требуется отсортировать.
                    \en Source array of first parameters to sort. \~
  \param[out] tt2 - \ru Исходный массив 2 параметров, который требуется отсортировать синхронно с tt0.
                    \en Source array of second parameters to sort synchronously with tt0. \~
  \param[in] eps - \ru Точность сравнения параметров.
                   \en Parameters comparison tolerance. \~
  \param[in] checkCoincidentParams - \ru Флаг проверки на наличие совпадающих параметров.
                                     \en Flag for checking of duplicate parameters. \~
  \ingroup Base_Algorithms
*/ //---
template <class DoubleParamsVector>
MATH_FUNC (bool) SortSynchroArrays( DoubleParamsVector & tt0, DoubleParamsVector & tt2,
                                    double eps, bool checkCoincidentParams );


//------------------------------------------------------------------------------
/** \brief \ru Отсортировать массив.
           \en Sort the array. \~
  \details \ru Множество сортируется по возрастанию параметра. Если установлен соответствующий флаг, 
    то проводится проверка на наличие совпадающих параметров. Если они есть, 
    то оставляется только один из них.
           \en Sort the array in ascending order. If the corresponding flag is set, 
    then the check for duplicate parameters is to be performed. If they exist, 
    then only one of them is kept. \~
  \param[out] tt0 - \ru Исходный массив параметров, который требуется отсортировать.
                    \en Source array of parameters to sort. \~
  \param[in] eps - \ru Точность сравнения параметров.
                   \en Parameters comparison tolerance. \~
  \param[in] checkCoincidentParams - \ru Флаг проверки на наличие совпадающих параметров.
                                     \en Flag for checking of duplicate parameters. \~
  \ingroup Base_Algorithms
*/
//---
template <class DoubleParamsVector>
MATH_FUNC (void) SortArray( DoubleParamsVector & tt0, double eps, bool checkCoincidentParams );


//------------------------------------------------------------------------------
/** \brief \ru Перебросить параметр на период.
           \en Re-roll the parameter for a period. \~
  \details \ru Перебросить параметр на период. Переброс происходит в сторону начального приближения.
           \en Re-roll the parameter for a period. The transfer occurs towards the initial approximation. \~
  \param[in] period - \ru Период.
                      \en Period. \~
  \param[in] p0     - \ru Начальное приближение параметра.
                      \en Initial approximation of the parameter. \~
  \param[in,out] p  - \ru Уточняемый параметр.
                      \en The parameter to refine. \~
  \return \ru Возвращает true, если переброс был выполнен, \n иначе - false.
          \en True if a parameter was changed, \n otherwise - false. \~

  \ingroup Base_Algorithms
*/ // ---
MATH_FUNC( bool ) CorrectCiclicParameter( double period, double p0, double & p );


//------------------------------------------------------------------------------
/** \brief \ru Уточнить параметр.
           \en Refine the parameter. \~
  \details \ru Если параметр замкнутый или должен находится не на расширении параметрической области, 
    то он в случае необходимости загоняется внутрь интервала [pmin, pmax].
           \en If parameter is closed or must not be in the extension of parametric region, 
    then it drawn inside region [pmin, pmax] if necessary. \~
  \param[in] pext - \ru Используется ли расширение параметрической области.
                    \en Whether extension of parametric region is used. \~
  \param[in] pc - \ru Замкнутость области определения параметра.
                  \en Closedness of parameter domain. \~
  \param[in] pmin - \ru Минимальное значение параметра.
                    \en The minimal value of parameter. \~
  \param[in] pmax - \ru Максимальное значение параметра.
                    \en The maximal value of parameter. \~
  \param[out] p - \ru Уточняемый параметр.
                  \en The parameter to refine. \~
  \param[in] eps - \ru Точность вычислений.
                   \en Computational tolerance. \~
  \ingroup Base_Algorithms
*/ // ---
MATH_FUNC (void) CorrectParameter( bool pext, bool pc, double pmin, double pmax,
                                   double & p, double eps = Math::paramRegion );


//------------------------------------------------------------------------------
/** \brief \ru Коррекция параметра с проверкой.
           \en Correction of parameter with validation. \~
  \details \ru Если после коррекции параметр выходит за пределы интервала [tmin, tmax], а флаг 
    использования параметра на расширении параметрической области равен false, то 
    параметр приравнивается либо к tmin, либо к tmax, в зависимости от того, с какой 
    стороны он вышел за пределы интервала [tmin, tmax].
           \en If the parameter is not in range [tmin, tmax] after correction, and flag 
    of use of parameter on extension of parametric region is false, then 
    parameter equates to tmin or tmax, according to which 
    bound it get out from range [tmin, tmax]. \~
  \param[in] tmin - \ru Минимальное значение параметра.
                    \en Minimal value of parameter. \~
  \param[in] tmax - \ru Максимальное значение параметра.
                    \en Maximal value of parameter. \~
  \param[in] tPeriod - \ru Период.
                       \en Period. \~
  \param[in] ext - \ru Используется ли расширение параметрической области.
                   \en Whether extension of parametric region is used. \~
  \param[in] tRegion - \ru Точность задания параметра.
                       \en Accuracy of parameter. \~
  \param[out] t - \ru Уточняемый параметр.
                  \en The parameter to refine. \~
  \return \ru true, если не было выхода за пределы интервала [tmin, tmax] после коррекции, \n иначе false.
          \en True if there was no parameter out of range [tmin, tmax] after correction, \n otherwise false. \~
  \ingroup Base_Algorithms
*/ // ---
MATH_FUNC (bool) CorrectCheckNearParameter( const double & tmin, const double & tmax, 
                                            const double & tPeriod, const bool & ext, const double & tRegion, double & t );


//------------------------------------------------------------------------------
/** \brief \ru Определить матрицу инвертирования значка.
           \en Determine the inversion matrix of roughness symbol. \~
  \details \ru Рассчитывается матрица инвертирования в трехмерных размерах.
           \en Calculating the inversion matrix in three-dimensional space. \~
  \param[in] place3D - \ru Локальная система координат.
                       \en Local coordinate system. \~
  \param[in] pDir - \ru Направление.
                    \en Direction. \~
  \param[in] seeY - \ru Ось Y экрана.
                    \en Screen Y-axis. \~
  \param[in] seeZ - \ru Ось Z экрана.
                    \en Screen Z-axis. \~
  \param[out] matrix - \ru Матрица инвертирования.
                       \en Inversion matrix. \~
  \return \ru true, если матрица найдена, \n иначе false
          \en True if the matrix was found, \n otherwise false \~
  \ingroup Base_Algorithms
*/ // ---
MATH_FUNC (bool) MatrixRoughInverse( const MbPlacement3D & place3D, const MbDirection * pDir, const MbVector3D & seeY,
                                     const MbVector3D & seeZ, MbMatrix & matrix );


//------------------------------------------------------------------------------
/** \brief \ru Определить матрицу инвертирования текста.
           \en Determine the inversion matrix of text. \~
  \details \ru Рассчитывается матрица инвертирования в трехмерных размерах.
           \en Calculating the inversion matrix in three-dimensional space. \~
  \param[in] place3D - \ru Локальная система координат.
                       \en Local coordinate system. \~
  \param[in] pDir - \ru Направление.
                    \en Direction. \~
  \param[in] seeY - \ru Ось Y экрана.
                    \en Screen Y-axis. \~
  \param[in] seeZ - \ru Ось Z экрана.
                    \en Screen Z-axis. \~
  \param[out] matrix - \ru Матрица инвертирования.
                       \en Inversion matrix. \~
  \return \ru true, если матрица найдена, \n иначе false
          \en True if the matrix was found, \n otherwise false \~
  \ingroup Base_Algorithms
*/ // ---
MATH_FUNC (bool) MatrixTextInverse ( const MbPlacement3D & place3D, const MbDirection * pDir, const MbVector3D & seeY,
                                     const MbVector3D & seeZ, MbMatrix & matrix );


//------------------------------------------------------------------------------
/** \brief \ru Округлить значение до n значащих цифр.
           \en The value is rounded to n significant digits. \~
  \details \ru Округлить значение до n значащих цифр. \n
           \en The value is rounded to n significant digits. \n \~
  \param[in,out] value - \ru Округляемое число.
                          \en The value. \~
  \param[in] n - \ru Число значащих цифр.
                 \en Number of significant digits. \~
  \return \ru true, если округление выполнено, иначе false.
          \en true, if value is rounded, otherwise false. \~
  \ingroup Base_Algorithms
*/ // ---
MATH_FUNC (bool) RoundedValue( double & value, uint8 n );


//------------------------------------------------------------------------------
/** \brief \ru Решить квадратное уравнение вида a * x^2 + b * x + c = 0 без внешнего управления погрешностью.
           \en Solve a quadratic equation of the form a * x ^ 2 + b * x + c = 0 without external tolerance control. \~
  \details \ru Решить квадратное уравнение вида a * x^2 + b * x + c = 0 без внешнего управления погрешностью. \n
           \en Solve a quadratic equation of the form a * x ^ 2 + b * x + c = 0 without external tolerance control. \n \~
  \param[in] a - \ru Коэффициент при квадрате неизвестной.
                 \en The second-degree term coefficient. \~
  \param[in] b - \ru Коэффициент при неизвестной.
                 \en The first-degree term coefficient. \~
  \param[in] c - \ru Коэффициент - константный член уравнения.
                 \en The constant term coefficient. \~
  \param[out] d - \ru Дискриминант уравнения.
                  \en The discriminant of equation. \~
  \param[out] res - \ru Корни уравнения.
                    \en Roots of equation. \~
  \return \ru Количество действительных решений и дискриминант уравнения.
          \en Number of real roots and discriminant of equation. \~
  \ingroup Base_Algorithms
*/ // ---
MATH_FUNC (int) QuadraticEquation( double a, double b, double c,
                                   double & d, std::pair<double,double> & res );


//------------------------------------------------------------------------------
/** \brief \ru Решить квадратное уравнение вида a * x^2 + b * x + c = 0.
           \en Solve a quadratic equation of the form a * x ^ 2 + b * x + c = 0. \~
  \details \ru Решить квадратное уравнение вида a * x^2 + b * x + c = 0. \n
           \en Solve a quadratic equation of the form a * x ^ 2 + b * x + c = 0. \n \~
  \param[in] a - \ru Коэффициент при квадрате неизвестной.
                 \en The second-degree term coefficient. \~
  \param[in] b - \ru Коэффициент при неизвестной.
                 \en The first-degree term coefficient. \~
  \param[in] c - \ru Коэффициент - константный член уравнения.
                 \en The constant term coefficient. \~
  \param[out] x1 - \ru Первый корень уравнения.
                   \en The first root of equation. \~
  \param[out] x2 - \ru Второй корень уравнения.
                   \en The second root of equation. \~
  \param[in] epsilon - \ru Погрешность нахождения решения.
                       \en Solution tolerance. \~
  \return \ru Количество действительных решений.
          \en Number of real roots. \~
\ingroup Base_Algorithms
*/ // ---
MATH_FUNC (int) QuadraticEquation( double a, double b, double c,
                                   double & x1, double & x2, double epsilon = Math::paramEpsilon );


//------------------------------------------------------------------------------
/** \brief \ru Решить кубическое уравнение вида a * x^3 + b * x^2 + c * x + d = 0.
           \en Solve a cubic equation of the form a * x^3 + b * x^2 + c * x + d = 0. \~
  \details \ru Решить кубическое уравнение вида a * x^3 + b * x^2 + c * x + d = 0. \n
           \en Solve a cubic equation of the form a * x^3 + b * x^2 + c * x + d = 0. \n \~
  \param[in] a - \ru Коэффициент при кубе неизвестной.
                 \en The third-degree term coefficient. \~
  \param[in] b - \ru Коэффициент при квадрате неизвестной.
                 \en The second-degree term coefficient. \~
  \param[in] c - \ru Коэффициент при неизвестной.
                 \en The first-degree term coefficient. \~
  \param[in] d - \ru Коэффициент - константный член уравнения.
                 \en The constant term coefficient. \~
  \param[out] x - \ru Корни уравнения.
                  \en Roots of equation. \~
  \param[in] epsilon - \ru Погрешность нахождения решения.
                       \en Solution tolerance. \~
  \return \ru Количество действительных решений.
          \en Number of real roots. \~
  \ingroup Base_Algorithms
*/ // ---
MATH_FUNC (int) QubicEquation( double a, double b, double c, double d, double * x, double epsilon );


//------------------------------------------------------------------------------
/** \brief \ru Решить кубическое уравнение вида x^3 - i1 * x^2 + i2 * x - i3 = 0.
           \en Solve a cubic equation of the form x^3 - i1 * x^2 + i2 * x - i3 = 0. \~
  \details \ru Решить кубическое уравнение вида x^3 - i1 * x^2 + i2 * x - i3 = 0. \n
           \en Solve a cubic equation of the form x^3 - i1 * x^2 + i2 * x - i3 = 0. \n \~
  \param[in] i1 - \ru Коэффициент при квадрате неизвестной.
                  \en The second-degree term coefficient. \~
  \param[in] i2 - \ru Коэффициент при неизвестной.
                  \en The first-degree term coefficient. \~
  \param[in] i3 - \ru Коэффициент - константный член уравнения.
                  \en The constant term coefficient. \~
  \param[out] x - \ru Корни уравнения.
                  \en Roots of equation. \~
  \param[in] epsilon - \ru Погрешность нахождения решения.
                       \en Solution tolerance. \~
  \return \ru Количество действительных решений.
          \en Number of real roots. \~
\ingroup Base_Algorithms
*/ // ---
MATH_FUNC (int) CubicEquation( double i1, double i2, double i3, double * x, double epsilon );


//------------------------------------------------------------------------------
/** \brief \ru Решить уравнение четвертой степени вида a * x^4 + b * x^3 + с * x^2 + d * x + e = 0.
           \en Solve a quartic equation of the form a * x^4 + b * x^3 + с * x^2 + d * x + e = 0. \~
  \details \ru Решить уравнение четвертой степени вида a * x^4 + b * x^3 + с * x^2 + d * x + e = 0. \n
           \en Solve a cubic equation of the form a * x^4 + b * x^3 + с * x^2 + d * x + e = 0. \n \~
  \param[in] a - \ru Коэффициент при четвертой степени неизвестной.
                 \en The fourth-degree term coefficient. \~
  \param[in] b - \ru Коэффициент при кубе неизвестной.
                 \en The third-degree term coefficient. \~
  \param[in] c - \ru Коэффициент при квадрате неизвестной.
                 \en The second-degree term coefficient. \~
  \param[in] d - \ru Коэффициент при неизвестной.
                 \en The first-degree term coefficient. \~
  \param[in] e - \ru Коэффициент - константный член уравнения.
                 \en The constant term coefficient. \~
  \param[out] x - \ru Корни уравнения.
                  \en Roots of equation. \~
  \param[in] epsilon - \ru Погрешность нахождения решения.
                       \en Solution tolerance. \~
  \return \ru Количество действительных решений.
          \en Number of real roots. \~
\ingroup Base_Algorithms
*/ // ---
MATH_FUNC (int) Degree4Equation( double a, double b, double c, double d, double e, double * x, double epsilon );


//------------------------------------------------------------------------------
/** \brief \ru Определить собственный вектор матрицы 3 x 3.
           \en Determine the eigenvector of 3 x 3 matrix. \~
  \details \ru Определить собственный вектор матрицы 3 x 3. \n
           \en Determine the eigenvector of 3 x 3 matrix. \n \~
  \param[in] a - \ru Матрица 3 x 3.
                 \en 3 x 3 matrix. \~
  \param[out] vect - \ru Собственный вектор матрицы.
                     \en The eigenvector of matrix. \~
\ingroup Base_Algorithms
*/ // ---
MATH_FUNC (void) EigenVector( double a[c3d::SPACE_DIM][c3d::SPACE_DIM], MbVector3D & vect );


#endif // __ALG_BASE_H
