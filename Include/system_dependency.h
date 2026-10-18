////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Системозависимые функции.
         \en System-dependent functions. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SYSTEM_DEPENDENCY_H
#define __SYSTEM_DEPENDENCY_H

#include <system_types.h>
#include <system_cpp_standard.h>
#include <math_define.h>

#ifdef C3D_WINDOWS //_MSC_VER 
#define LOG_PATH     _T("C:\\Logs\\")
#else // C3D_WINDOWS 
#define LOG_PATH     _T("")
#endif // C3D_WINDOWS 

#ifndef C3D_WINDOWS //_MSC_VER

#include <stdio.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <limits.h>
#include <tool_cstring.h>
#include <unistd.h>


#if (C3D_GLIBC_VERSION >= 2023)  // C3D_GLIBC_VERSION
  #include <cmath>
  #define c3d_isnan(d) std::isnan(d)
#else  // C3D_GLIBC_VERSION
  #include <math.h>
  #define c3d_isnan(d) isnan(d)
#endif  // C3D_GLIBC_VERSION

#define _finite std::isfinite


//------------------------------------------------------------------------------
// \ru RGB-цвет. \en RGB color. 
// ---
typedef uint32 COLORREF;


//------------------------------------------------------------------------------
// RGBs of int colors from WinApi
// ---
#define RGB(r, g ,b)  ((uint32) (((uint8) (r) | ((uint16) (g) << 8)) | (((uint32) (uint8) (b)) << 16))) 


//------------------------------------------------------------------------------
// \ru Дать значение красного цвета в пределах 0 - 255. \en Get value of red color in range from 0 to 255. 
// ---
inline uint8 GetRValue(COLORREF rgb_color)
{
  return static_cast<uint8>(rgb_color);
}


//------------------------------------------------------------------------------
// \ru Дать значение зелёного цвета в пределах 0 - 255. \en Get value of green color in range from 0 to 255. 
// ---
inline uint8 GetGValue(COLORREF rgb_color)
{
  return static_cast<uint8>(rgb_color >> 8);
}


//------------------------------------------------------------------------------
// \ru Дать значение синего цвета в пределах 0 - 255. \en Get value of blue color in range from 0 to 255. 
// ---
inline uint8 GetBValue(COLORREF rgb_color)
{
  return static_cast<uint8>(rgb_color >> 16);
}


// \ru Вывод сообщений об ошибках пока отключаем в Линуксе \en Output of error messages is temporary disabled in Linux 
#define _RPT0(warnlvl, text)
#define _RPT1(a1, a2, a3) // \ru не реализовано \en not implemented  
#define _CRT_WARN
#define _CRT_ASSERT

#else // C3D_WINDOWS

  #define c3d_isnan(d) _isnan(d)

// \ru Одновременное определение _MBCS и _UNICODE недопустимо! \en Simultaneous definition of _MBCS and _UNICODE is not acceptable! 
#if defined(_MBCS) && defined(_UNICODE)
  #error Multibyte Character Sets (MBCS) not supported
#endif // _MBCS

#endif // C3D_WINDOWS

#include <algorithm>

// #error <thread> is not supported when compiling with /clr or /clr:pure.
// #include <thread>

#ifndef threadLocal
# if (__STDC_VERSION__ >= 201112) && !defined(__STDC_NO_THREADS__)
#  define threadLocal _Thread_local
# elif defined C3D_WINDOWS && ( \
       defined _MSC_VER || \
       defined __ICL || \
       defined __DMC__ )
#  define threadLocal __declspec(thread)
/* note that ICC (linux) and Clang are covered by __GNUC__ */
# elif defined __GNUC__ || \
       defined __SUNPRO_C || \
       defined __xlC__
#  define threadLocal __thread
# else
#  error "Cannot define thread_local"
# endif
#endif


class MbTimerImp;
//------------------------------------------------------------------------------
/** \brief \ru Высокоточный таймер.
           \en High resolution timer. \~ 
  \details \ru Высокоточный таймер.
           \en High resolution timer. \~
\ingroup Base_Tools
*/
// ---
class MATH_CLASS MbAccurateTimer {
protected:
  double       lastTime;  ///< \ru Время в секундах. \en Time in seconds.
private:
  MbTimerImp * imp;       ///< \ru Реализация таймера. \en Timer implementation.

public:
  /// \ru Конструктор. \en Constructor. 
  MbAccurateTimer();
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbAccurateTimer();

  /// \ru Сброшен ли таймер. \en Is empty timer?
  virtual bool    IsEmpty () const { return !(lastTime > 0.0); }
  /// \ru Сбросить таймер. \en Reset timer. 
  virtual void    SetEmpty()       { lastTime = 0.0; }
  /// \ru Добавить значение. \en Add time value. 
  virtual bool    SetTime ( double t ) { lastTime = t; return (t >= 0); }
  /// \ru Начать отсчет. \en Start time measurement. 
  virtual void    Begin  ();
  /// \ru Закончить отсчет. \en Finish time measurement. 
  virtual void    End    ();
  /// \ru Получить значение времени. \en Get time value. 
          double  GetLast() const { return lastTime; }
  /// \ru Получить значение частоты. \en Get frequency value. 
          double  PerSec () const { return (lastTime > 0.0 ? 1.0 / lastTime : 0.0); }
};


//------------------------------------------------------------------------------
/** \brief \ru Таймер со статистикой.
           \en Average timer. \~ 
  \details \ru Таймер со статистикой.
           \en Average timer. \~
\ingroup Base_Tools
*/
// ---
class MATH_CLASS MbAverageTimer : public MbAccurateTimer
{
  double  avgTime;  ///< \ru Среднее время в секундах. \en Average time in seconds. 
  double  minTime;  ///< \ru Минимальное время в секундах. \en Min. time in seconds. 
  double  maxTime;  ///< \ru Максимальное время в секундах. \en Max. time in seconds. 
  uint    runCount; ///< \ru Количество запусков. \en Number of launches. 

public:
  /// \ru Конструктор. \en Constructor. 
  MbAverageTimer() : avgTime( 0.0 ), minTime( 0.0 ), maxTime ( 0.0 ), runCount( 0 ) {}
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbAverageTimer() {}

  ///< \ru Пустой таймер? \en Empty timer? 
  virtual bool    IsEmpty() const { return (runCount == 0); }
  ///< \ru Сбросить таймер. \en Reset timer.
  virtual void    SetEmpty();
  ///< \ru Добавить значение. \en Add value. 
  virtual bool    SetTime( double );
  ///< \ru Получить среднее время в секундах. \en Get average time in seconds. 
          double  GetAvg() const { return avgTime; }
  ///< \ru Получить минимальное время в секундах. \en Get minimal time in seconds. 
          double  GetMin() const { return minTime; }
  ///< \ru Получить максимальное время в секундах. \en Get maximal time in seconds. 
          double  GetMax() const { return maxTime; }
};

//------------------------------------------------------------------------------
// \ru Сбросить таймер. \en Reset timer. 
//---
inline void MbAverageTimer::SetEmpty()
{
  MbAccurateTimer::SetEmpty();
  avgTime = minTime = maxTime = 0.0;
  runCount = 0;
}

//------------------------------------------------------------------------------
// \ru Добавить значение. \en Add value. 
//---
inline bool MbAverageTimer::SetTime( double t )
{
  bool res = MbAccurateTimer::SetTime( t );

  if ( IsEmpty() )
    avgTime = minTime = maxTime = t;
  else
  {
    avgTime += (t - avgTime) / (runCount + 1);
    minTime = (std::min)( minTime, t );
    maxTime = (std::max)( maxTime, t );
  }

  runCount++;
  return res;
}

#endif // __SYSTEM_DEPENDENCY_H
