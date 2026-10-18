////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Получение линий очерка.
         \en Obtaining the isocline curves. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ALG_SILHOUETTE_HIDE_H
#define __ALG_SILHOUETTE_HIDE_H


#include <templ_rp_array.h>
#include <math_define.h>
#include <mb_variables.h>


class    MbVector3D;
class    MbPlacement3D;
class    MbCurve;
class    MbSurface;
class    MbMesh;


//------------------------------------------------------------------------------
/** \brief \ru Получить массив кривых плоской проекции очерка поверхности.
           \en Get the array of surface silhouette curves of planar projection. \~
  \details \ru Получить массив кривых плоской проекции очерка поверхности. \n
           \en Get the array of surface silhouette curves of planar projection. \n \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) CreateSurfaceHide( const MbSurface & surf, const MbPlacement3D & eyePlace, double sag, 
                                    RPArray<MbCurve> & hideCurves, VERSION version = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Рассчитать сетку.
           \en Calculate mesh. \~
  \details \ru Рассчитать сетку массива кривых очерка поверхности. \n
           \en Calculate mesh of array of surface silhouette curves. \n \~
  \ingroup Polygonal_Objects
*/
// ---
MATH_FUNC (void) CalculateHideMesh( const MbSurface & surf, const MbVector3D & eyeDir, double sag, 
                                    MbMesh *& mesh, VERSION version = Math::DefaultMathVersion() );


#endif // __ALG_SILHOUETTE_HIDE_H
