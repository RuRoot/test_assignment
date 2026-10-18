////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Параметры операций.
         \en Parameters of operations. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __OP_GENERAL_PARAMETERS_H
#define __OP_GENERAL_PARAMETERS_H

#include <math_define.h>

//------------------------------------------------------------------------------
/** \brief \ru Параметры для многократного проецирования точек на оболочку.
           \en Parameters for multiple projection of points onto the shell. \~
    \details \ru Параметры для многократного проецирования точек на оболочку.
                 Возможны два метода - с помощью вызова метода оболчки, и с помощью проецирования на линеаризацию 
                 оболочки и последующего уточнения.
             \en Parameters for multiple projection of points onto the shell. 
                 There are two methods are possible. First one is calling to shell projection method.
                 Second one is using projection onto shell triangulation as a hint for futher correction. \~
    \ingroup Algorithms_3D
    \warning \ru В разработке.
             \en Under development. \~
*/
// ---
class MATH_CLASS MbShellProjectionParams
{
public:
  //------------------------------------------------------------------------------
  /** \brief \ru Перечисление методов проецирования на оболочку.
  */
  // ---
  enum class MethodProjection
  {
    byShellMethodCall,   ///< \ru Вызов метода оболочки. \en Shell method calling.
    byLinearizationCache ///< \ru С использованием кэша линеаризации. \en Using linearization cache.
  };

private:
  MethodProjection _method;        ///< \ru Метод проецирования. \en Projection method.

public:
  /// \ru Конструктор. \en Constructor.
  MbShellProjectionParams( MethodProjection method )
    : _method( method )
  {}
  /// \ru Деструктор. \en Destructor.
  ~MbShellProjectionParams() {}

  ///< \ru Получить способ проецирования. \en Get projection method.
  MethodProjection GetMethod() const { return _method; }

  OBVIOUS_PRIVATE_COPY( MbShellProjectionParams )
};

#endif // __OP_GENERAL_PARAMETERS_H
