//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Расчет пересечений тел посредством аппарата булевой операции.
         \en Calculation of intersections between solids using the boolean operations. \~

*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CDET_BOOL_H
#define __CDET_BOOL_H


#include <mb_operation_result.h>
#include <mb_enum.h>
#include <math_define.h>
#include <vector>


class   MbSolid;
class   MbSNameMaker;
class   MbCurveEdge;


//----------------------------------------------------------------------------------------
/** \brief \ru Расчет пересечений тел посредством аппарата булевой операции.
           \en Calculation of intersections between solids using the boolean operations. \~
  \details \ru Расчет пересечений тел посредством аппарата булевой операции. 
           \en Calculation of intersections between solids using the boolean operations. \~ \n
  \param[in] solid1 - \ru Первое тело. \en The first solid. \~
  \param[in] solid2 - \ru Второе тело. \en The second solid. \~
  \param[out] edges - \ru Ребра пересечения тел. \en Intersection edges. \~
  \param[out] intersectedFaces - \ru Пары номеров пересекшихся граней. \n
                               - \en The couples of indeses intersected faces of the solids, \n
  \param[out] touchedFaces     - \ru Пары номеров касающихся граней с противоположно направленными нормалями.
                                 \en The couples of indeses of contacted faces with oppositely directed normals. \~
  \param[out] similarFaces     - \ru Пары номеров касающихся подобных граней, которые могут быть объединены.
                                 \en The couples of indeses of relating to similar faces that can be combined. \~
  \return \ru Код результата операции. \en Operation result code. \~
 
  \warning \ru Тела будут изменены операцией! Если требуется сохранить тела без изменений, 
               передавайте копии, сделанные помощью MbSolid::Duplicate().
           \en The solids will be modified by this operation! To keep the body intact,
               give the copies made using MbSolid::Duplicate(). \~

  \ingroup Collision_Detection
*/
//---
MATH_FUNC (MbResultType) InterferenceSolids( MbSolid & solid1, MbSolid & solid2,
                                             std::vector<MbCurveEdge*> * edges,
                                             c3d::IndicesPairsVector * intersectedFaces,
                                             c3d::IndicesPairsVector * similarFaces,
                                             c3d::IndicesPairsVector * touchedFaces );


#endif // __CDET_BOOL_H

// eof