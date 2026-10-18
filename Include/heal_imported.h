////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Постобработка импортированных тел.
         \en Pospprocessing of imported solids. \~
  \details  \ru Установка характеристик и геометрии в соответствие с критериями C3D Modeler.
            \en Tuning characteristric and geometry of solids according to the C3D Modeler criteria. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __HEAL_IMPORTED_H
#define __HEAL_IMPORTED_H

#include <math_define.h>
class MbSolid;

/** \brief \ru Скорректировать тип кривых пересечения в рёбрах.
           \en Set the right type of intersecion curves in edges. \~
\param[out] solid - \ru Тело для обработки.
                  \en Solid to be processed. \~
\details \ru В кривых пересечения, построенных по точкам и объявленных cbt_Tolerant проводится
проверка нормалей поверхностей на колинеарность. В случае, если нормали не колинеарны,
тип меняется на cbt_Specific.
        \en In curves built by points and classified as cbt_Toleranc the check if the surfaces' normals are
 colinear is performed. In case the nornals are not colinear the type is switched to the cbt_Specific. \~
\ingroup Data_Exchange
*/
CONV_FUNC( void ) AdjustIntersectionCurvesType( MbSolid& solid );


/** \brief \ru Установить точки вершин по рёбрам.
           \en Set the verticis' poins by edges. \~
\param[out] solid - \ru Тело для обработки.
                  \en Solid to be processed. \~
\details \ru В качестве точки вершины устанавливается средняя точка концов кривых в рёбрах
 примыкающих к врешине.
        \en The location of the vertes is set as the average value of end points of the curves of adjacent edges. \~
\ingroup Data_Exchange
*/
CONV_FUNC( void ) AdjustVerticisGeometryByEdges( MbSolid& solid );


#endif // __HEAL_IMPORTED_H
