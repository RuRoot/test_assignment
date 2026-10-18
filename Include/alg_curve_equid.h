////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение эквидистанты. Построение штриховки.
         \en Construction of equidistance. Construction of hatching. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ALG_CURVE_EQUID_H
#define __ALG_CURVE_EQUID_H

#include <curve.h>


//------------------------------------------------------------------------------
/** \brief \ru Построение эквидистантных кривых к кривой.
           \en Construction of offset curves to a curve. \~
  \details \ru Построение эквидистантных кривых к произвольной кривой справа и слева.
    Имя каждого эквидистантного контура совпадает с именем исходного.
           \en Construction of equidistant curves to arbitrary curve on the right and on the left.
    A name of every offset contour matches with the name of the initial one. \~
  \param[in]  curve    - \ru Исходная кривая.
                         \en The initial curve \~
  \param[in]  radLeft  - \ru Радиус эквидистанты слева по направлению.
                         \en The equidistance radius on the left by direction. \~
  \param[in]  radRight - \ru Радиус эквидистанты справа по направлению.
                         \en The equidistance radius on the right by direction. \~
  \param[in]  side     - \ru Признак, с какой стороны строить:\n
                         0 - слева по направлению,\n
                         1 - справа по направлению,\n
                         2 - с двух сторон.
                         \en Attribute defining the side to construct:\n
                         0 - on the left by direction,\n
                         1 - on the right by direction,\n
                         2 - on the both sides. \~
  \param[in]  arcMode  - \ru Способ обхода углов:\n
                         true - дугой,
                         false - срезом.
                         \en The way of traverse of angles:\n
                         true - by arc,
                         false - by section. \~
  \param[in]  degState - \ru Признак разрешения вырожденных сегментов:\n
                         0 - вырожденные сегменты запрещены,\n
                         1 - вырожденные сегменты разрешены.
                         \en Attribute of degenerate segments allowance:\n
                         0 - degenerate segments are forbidden,\n
                         1 - degenerate segments are allowed. \~
  \param[out] equLeft  - \ru Множество контуров слева.
                         \en The array of contours on the left side. \~
  \param[out] equRight - \ru Множество контуров справа.
                         \en The array of contours on the right side. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (void) Equid( const MbCurve *         curve, 
                              double            radLeft, 
                              double            radRight,
                              int               side, 
                              bool              arcMode, 
                              bool              degState,
                              PArray<MbCurve> & equLeft, 
                              PArray<MbCurve> & equRight );


//------------------------------------------------------------------------------
/** \brief \ru Построение области штриховки кривых.
           \en Construction of curves hatching region. \~
  \details \ru Построение штриховки заданной ширины внутри или около кривых.
           \en Construction of hatching with a given width inside or near curves. \~
  \param[in]  contour  - \ru Исходная кривые.
                         \en Initial curves \~
  \param[in]  witdh    - \ru Ширина штриховки.
                         \en The hatching width. \~
  \param[out] borders  - \ru Множество линий штриховки и границ штриховки.
                         \en The array of hatching lines and hatching boundaries. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (void) MakeHatchingArea( const PArray<MbCurve> & contour, double witdh,
                                   PArray<MbCurve> & borders );


#endif // __ALG_CURVE_EQUID_H
