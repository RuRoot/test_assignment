////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Методы инициализации дефолтными значениями пользовательских объектов геометрии и топологии.
         \en Methods for initializing custom geometry and topology objects with default values. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CONV_OB_INIT_H
#define __CONV_OB_INIT_H

#include "conv_ob_brep.h"
#include "conv_ob_geometry.h"
#include "math_define.h"

namespace c3d_conv_user
{

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать массив id.
            \en Initialize a array of id's.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObIdArray & ob );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать массив 2D примитивов.
            \en Initialize a array of 2D primitives.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObPrimitive2DArray & ob );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать массив 3D примитивов.
            \en Initialize a array of 3D primitives.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObPrimitive3DArray & ob );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать массив параметров.
            \en Initialize a array of parameters.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObDoubleArray & ob );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать данные для оболочки.
            \en Initialize a data for shell.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObShell & s );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать данные для грани.
            \en Initialize a data for face.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObFace & f );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать данные для цикла.
            \en Initialize a data for loop.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObLoop & l );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать данные для ориентированного ребра.
            \en Initialize a data for coedge.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObCoedge & c );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать данные для ребра.
            \en Initialize a data for edge.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObEdge & e );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать данные для вершины.
            \en Initialize a data for vertex.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObVertex & v );

////////////////////////////////////////////////////////////////////////////////
//
// Curves
//
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать 2D примитив.
            \en Initialize a 2D primitive.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObPrimitive2D & p );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать СК.
            \en Initialize a 2D location.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObLocation2D & l );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать одномерный диапазон.
            \en Initialize a one dimensional range.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObRange1D & r );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать двумерный отрезок.
            \en Initialize a planar line segment.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObSegment2D & s );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать двумерную полилинию.
            \en Initialize a planar polyline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObPolyline2D & p );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать двумерную дугу.
            \en Initialize a planar arc.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObArc2D & a );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать двумерный NURBS.
            \en Initialize a planar NURBS.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObNURBSCurve2D & n );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать двумерную составную кривую.
            \en Initialize a planar composite curve.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObCompositeCurve2D & c );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать трехмерную кривую пересечения.
            \en Initialize a spatial intersection curve.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObIntersectionCurve & n );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать двумерный кубический сплайн.
            \en Initialize a 2D cubic spline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObCubicSpline2D & n );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать двумерный сплайн Эрмита.
            \en Initialize a 2D Hermit spline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObHermit2D & n );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать эквидистантную двумерную кривую.
            \en Initialize a 2D offset curve.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObOffsetCurve2D & s );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать примитив трехмерного пространства.
            \en Initialize a spatial primitive.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObPrimitive3D & p );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать примитив как ось ox.
            \en Initialize the primitive as the ox axis.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) InitOx( ObPrimitive3D & p );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать примитив как ось oy.
            \en Initialize the primitive as the oy axis.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) InitOy( ObPrimitive3D & p );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать примитив как ось oz.
            \en Initialize the primitive as the oz axis.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) InitOz( ObPrimitive3D & p );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать ЛС координат в трехмерном пространстве.
            \en Initialize a spatial placement.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObLocation3D & l );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать трехмерный отрезок.
            \en Initialize a spatial line segment.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObSegment3D & s );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать трехмерную полилинию.
            \en Initialize a spatial polyline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObPolyline3D & p );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать трехмерную дугу.
            \en Initialize a spatial arc.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObArc3D & a );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать пространственную параболу.
            \en Initialize a spatial parabola.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObParabola3D & h );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать пространственную гиперболу.
            \en Initialize a spatial hyperbola.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObHyperbola3D & h );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать сплайн.
            \en Initialize spline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init ( ObSplineCurve3D& s );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать трехмерный NURBS.
            \en Initialize a spatial NURBS.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObNURBSCurve3D & n );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать трехмерную составную кривую.
            \en Initialize a spatial composite curve.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObCompositeCurve3D & c );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать трехмерный кубический сплайн.
            \en Initialize a 3D cubic spline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObCubicSpline3D & n );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать касательные в граничных точках 3д-кривой.
            \en Initialize a tangents of limit point on 3D curves.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObLimitPointsTangents3D & n );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать данные для трехмерного кубического сплайна.
            \en Initialize a data for 3D cubic spline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObDataCubicSpline3D & n );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать трехмерный сплайн Эрмита.
            \en Initialize a 3D Hermit spline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObHermit3D & n );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать эквидистантную трехмерную кривую.
            \en Initialize a 3D offset curve.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObOffsetCurve3D & s );

////////////////////////////////////////////////////////////////////////////////
//
// Surfaces
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать плоскость.
            \en Initialize a plane.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObPlane & p );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать цилиндрическую поверхность.
            \en Initialize a cylinder surface.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObCylinder & c );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать коническую поверхность.
            \en Initialize a cone surface.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObCone & c );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать сферическую поверхность.
            \en Initialize a sphere surface.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObSphere & s );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать тороидальную поверхность.
            \en Initialize a torus surface.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObTorus & t );

//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать поверхность выдавливания.
            \en Initialize a extrusion surface.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObExtrusion & e );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать поверхность вращения.
            \en Initialize a revolution surface.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObRevolution & r );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать линейчатую поверхность.
            \en Initialize a ruled surface.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObRuled & r );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать сплайновую поверхность.
            \en Initialize a spline surface.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObNURBSSurface & n );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать смещённую поверхность.
            \en Initialize a offset surface.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObOffsetSurface & o );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать трансформированную копию поверхности.
            \en Initialize a transformed surface replica.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObReplicaSurface & o );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать коническую спираль.
            \en Initialize a cone spiral.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObConeSpiral & s );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать усеченную двумерную кривую.
            \en Initialize a 2D trimmed curve.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObTrimmedCurve2D & s );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать двумерный сплайн 5 степени.
            \en Initialize a 2D 5-degree spline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObFifthDegreeSpline2D & s );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать усеченную трехмерная кривую.
            \en Initialize a 3D trimmed curve.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObTrimmedCurve3D & s );


//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать трехмерный сплайн 5 степени.
            \en Initialize a 3D 5-degree spline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObFifthDegreeSpline3D & s );



//------------------------------------------------------------------------------
/** \brief  \ru Инициализировать копию кривой.
            \en Initialize a replica of curve.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
CONV_FUNC( void ) Init( ObReplicaCurve3D & o );

} // namespace c3d_conv_user

#endif // __CONV_OB_INIT_H
