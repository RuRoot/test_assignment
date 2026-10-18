////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Объекты для передачи геометрии для создания тела по пользовательским объектам.
         \en Objects for transferring geometry to create a solid based on user-defined objects. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CONV_OB_IMPORT_GEOMETRY_H
#define __CONV_OB_IMPORT_GEOMETRY_H

#include "math_define.h"

namespace c3d_conv_user
{

/**  \brief \ru Значения для булевых переменных.
             \en Values ​​for boolean variables. \~
    \note  \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
            \en EXPEREIMENTAL \~.
*/
// DEPRECATE_DECLARE
#define C3D_OB_BOOL   int
#define C3D_OB_TRUE   1
#define C3D_OB_FALSE  0


//------------------------------------------------------------------------------
/** \brief  \ru Примитив двумерного пространства (точка либо вектор).
            \en Planar primitive (point or vector).\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObPrimitive2D {
  double x, y;
};


//------------------------------------------------------------------------------
/** \brief  \ru Локальная система координат в двумерном пространстве.
            \en Planar placement.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObLocation2D {
  ObPrimitive2D origin, axisX, axisY;
};


//------------------------------------------------------------------------------
/** \brief  \ru Одномерный диапазон.
            \en One dimensional range.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObRange1D {
  double start, end;
};


//------------------------------------------------------------------------------
/** \brief  \ru Двумерный отрезок.
            \en Planar line segment.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObSegment2D {
  static const int CurveType = 1;
  ObRange1D     domain;
  ObPrimitive2D start, end;
};


//------------------------------------------------------------------------------
/** \brief  \ru Двумерная полилиния.
            \en Planar polyline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObPolyline2D {
  static const int CurveType = 2;
  int pointsArrayId;
};


//------------------------------------------------------------------------------
/** \brief  \ru Двумерная дуга.
            \en Planar arc.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObArc2D {
  static const int CurveType = 3;
  ObLocation2D location;
  double majorAxis, minorAxis;
  ObRange1D domain;
};



//------------------------------------------------------------------------------
/** \brief  \ru Двумерный NURBS.
            \en Planar NURBS.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObNURBSCurve2D {
  static const int CurveType = 7;
  int knotsId, vertcisId, weightsId;
  int degree;
  C3D_OB_BOOL closed;
  ObRange1D domain;
};



//------------------------------------------------------------------------------
/** \brief  \ru Двумерная составная кривая.
            \en Planar composite curve.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObCompositeCurve2D {
  static const int CurveType = 8;
  int segmentsArrIndicis;
};


//------------------------------------------------------------------------------
/** \brief  \ru Трехмерная кривая пересечения.
            \en Spatial intersection curve.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObIntersectionCurve
{
  static const int CurveType = 9;
  int surface1, surface2;
  int curveOnSurface1, curveOnSurface2;
};


//------------------------------------------------------------------------------
/** \brief  \ru Двумерный кубический сплайн.
            \en 2D cubic spline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObCubicSpline2D {
  static const int CurveType = 10;
  int pointsId, paramsId, vectorsId;
  C3D_OB_BOOL closed;
};


//------------------------------------------------------------------------------
/** \brief  \ru Двумерный сплайн Эрмита.
            \en 2D Hermit spline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObHermit2D {
  static const int CurveType = 11;
  int pointsId, paramsId, vectorsId;
  C3D_OB_BOOL closed;
};


//------------------------------------------------------------------------------
/** \brief  \ru Эквидистантная двумерная кривая.
            \en 2D offset curve.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObOffsetCurve2D {
  static const int CurveType = 13;
  int curveId;
  double offsetStart, offsetEnd;
};


//------------------------------------------------------------------------------
/** \brief  \ru Примитив трехмерного пространства (точка либо вектор).
            \en Spatial primitive (point or vector).\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObPrimitive3D {
  double x, y, z;
};



//------------------------------------------------------------------------------
/** \brief  \ru Локальная система координат в трехмерном пространстве.
            \en Spatial placement.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObLocation3D {
  ObPrimitive3D origin, axisX, axisY, axisZ;
};


//------------------------------------------------------------------------------
/** \brief  \ru Трехмерный отрезок.
            \en Spatial line segment.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObSegment3D {
  static const int CurveType = 1;
  ObRange1D     domain;
  ObPrimitive3D start, end;
};



//------------------------------------------------------------------------------
/** \brief  \ru Трехмерная полилиния.
            \en Spatial polyline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObPolyline3D {
  static const int CurveType = 2;
  int pointsArrayId;
};


//------------------------------------------------------------------------------
/** \brief  \ru Трехмерная дуга.
            \en Spatial arc.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObArc3D {
  static const int CurveType = 3;
  ObLocation3D location;
  double majorAxis, minorAxis;
  ObRange1D domain;
};


//------------------------------------------------------------------------------
/** \brief  \ru Пространственная парабола.
            \en Spatial parabola.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObParabola3D {
  static const int CurveType = 4;
  ObLocation3D location;
  double focalParameter;
  ObRange1D domain;
};


//------------------------------------------------------------------------------
/** \brief  \ru Пространственная гипербола.
            \en Spatial hyperbola.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObHyperbola3D {
  static const int CurveType = 5;
  ObLocation3D location;
  double majorSemiAxis, minorSemiAxis;
  ObRange1D domain;
};

//------------------------------------------------------------------------------
/** \brief  \ru Трехмерный сплайн.
            \en Spatial spline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObSplineCurve3D {
  static const int CurveType = 6;
  int knotsId, vertcisId;
  int degree;
  int closed;
};

//------------------------------------------------------------------------------
/** \brief  \ru Трехмерный NURBS.
            \en Spatial NURBS.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObNURBSCurve3D {
  static const int CurveType = 7;
  int knotsId, vertcisId, weightsId;
  int degree;
  C3D_OB_BOOL closed;
  ObRange1D domain;
};


//------------------------------------------------------------------------------
/** \brief  \ru Трехмерная составная кривая.
            \en Spatial composite curve.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObCompositeCurve3D {
  static const int CurveType = 8;
  int segmentsArrIndicis;
};


//------------------------------------------------------------------------------
/** \brief  \ru Трехмерный кубический сплайн.
            \en 3D cubic spline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObCubicSpline3D {
  static const int CurveType = 10;
  int pointsId, paramsId, vectorsId;
  C3D_OB_BOOL closed;
};


//------------------------------------------------------------------------------
/** \brief  \ru Касательные в граничных точках 3д-кривой.
            \en Tangents of limit point on 3D curves.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObLimitPointsTangents3D {
  C3D_OB_BOOL useStartTangent, useEndTangent;
  ObPrimitive3D startTangent, endTangent;
};


//------------------------------------------------------------------------------
/** \brief  \ru Данные для трехмерного кубического сплайна.
            \en Data for 3D cubic spline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObDataCubicSpline3D {
  static const int CurveType = ObCubicSpline3D::CurveType;
  ObCubicSpline3D spline;
  ObLimitPointsTangents3D geometryLimits;
};


//------------------------------------------------------------------------------
/** \brief  \ru Трехмерный сплайн Эрмита.
            \en 3D Hermit spline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObHermit3D {
  static const int CurveType = 11;
  int pointsId, paramsId, vectorsId;
  C3D_OB_BOOL closed;
};

//------------------------------------------------------------------------------
/** \brief  \ru Эквидистантная трехмерная кривая.
            \en 3D offset curve.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObOffsetCurve3D {
  static const int CurveType = 13;
  int curveId;
  ObPrimitive3D offset;
};


////////////////////////////////////////////////////////////////////////////////
//
// Surfaces
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
/** \brief  \ru Плоскость.
            \en Plane.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObPlane {
  static const int SurfaceType = 1;
  ObLocation3D location;
  ObRange1D uDomain, vDomain;
};


//------------------------------------------------------------------------------
/** \brief  \ru Цилиндрическая поверхность.
            \en Cylinder surface.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObCylinder {
  static const int SurfaceType = 2;
  ObLocation3D location;
  double r, h;
  ObRange1D uDomain, vDomain;
};



//------------------------------------------------------------------------------
/** \brief  \ru Коническая поверхность.
            \en Cone surface.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObCone {
  static const int SurfaceType = 3;
  ObLocation3D location;
  double r, h, halfAngle;
  ObRange1D uDomain, vDomain;
};


//------------------------------------------------------------------------------
/** \brief  \ru Сферическая поверхность.
            \en Sphere surface.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObSphere {
  static const int SurfaceType = 4;
  ObLocation3D location;
  double r;
  ObRange1D uDomain, vDomain;
};



//------------------------------------------------------------------------------
/** \brief  \ru Тороидальная поверхность.
            \en Torus surface.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObTorus {
  static const int SurfaceType = 5;
  ObLocation3D location;
  double rMin, rMax;
  ObRange1D uDomain, vDomain;
};


//------------------------------------------------------------------------------
/** \brief  \ru Поверхность выдавливания.
            \en Extrusion surface.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObExtrusion {
  static const int SurfaceType = 6;
  int curve; // идентификатор 3D кривой
  ObPrimitive3D direction;
};

//------------------------------------------------------------------------------
/** \brief  \ru Поверхность вращения.
            \en Revolution surface.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObRevolution {
  static const int SurfaceType = 7;
  int curveId; // идентификатор 3D кривой
  ObPrimitive3D axisOrigin, axisDirection;
  ObRange1D vRange;
};


//------------------------------------------------------------------------------
/** \brief  \ru Линейчатая поверхность.
            \en Ruled surface.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObRuled {
  static const int SurfaceType = 8;
  int curve1, curve2;
};


//------------------------------------------------------------------------------
/** \brief  \ru Сплайновая поверхность.
            \en Spline surface.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObNURBSSurface {
  static const int SurfaceType = 10;
  int knotsUId; // идентификатор ObDoubleArray
  int knotsVId; // идентификатор ObDoubleArray
  int verticisListListId; // идентификатор ObIdArray, элементы которого являются
                          // идентификаторами ObPrimitive3DArray
  int weightsListListId;  // идентификатор ObIdArray, элементы которого являются
                          // идентификаторами ObDoubleArray
  int degreeU, degreeV;
  C3D_OB_BOOL closedU, closedV;
};


//------------------------------------------------------------------------------
/** \brief  \ru Смещённая поверхность.
            \en Offset surface.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObOffsetSurface {
  static const int SurfaceType = 11;
  int baseSurface;
  double offset;
};

//------------------------------------------------------------------------------
/** \brief  \ru Трансформированная копия поверхности.
            \en Transformed surface replica.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObReplicaSurface {
  static const int SurfaceType = 12;
  int baseSurface;
  ObLocation3D location;
};


//------------------------------------------------------------------------------
/** \brief  \ru Коническая спираль.
            \en Cone spiral.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObConeSpiral {
  static const int CurveType = 14;
  ObLocation3D location;
  double radius1;
  double radius2;
  double height;
  double step;
};


//------------------------------------------------------------------------------
/** \brief  \ru Усеченная двумерная кривая.
            \en 2D trimmed curve.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObTrimmedCurve2D{
  static const int CurveType = 14;
  int curveId;
  double paramStart, paramEnd;
};

//------------------------------------------------------------------------------
/** \brief  \ru Двумерный сплайн 5 степени.
            \en 2D 5-degree spline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObFifthDegreeSpline2D {
  static const int CurveType = 15;
  int pointsId, paramsId, firstDersId, secondDersId;
  C3D_OB_BOOL closed;
};


//------------------------------------------------------------------------------
/** \brief  \ru Усеченная трехмерная кривая.
            \en 3D trimmed curve.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObTrimmedCurve3D {
  static const int CurveType = 15;
  int curveId;
  double paramStart, paramEnd;
};

//------------------------------------------------------------------------------
/** \brief  \ru Трехмерный сплайн 5 степени.
            \en 3D 5-degree spline.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObFifthDegreeSpline3D {
  static const int CurveType = 16;
  int pointsId, paramsId, firstDersId, secondDersId;
  C3D_OB_BOOL closed;
};


//------------------------------------------------------------------------------
/** \brief  \ru Копия кривой.
            \en Replica of curve.\~
    \attention \ru Экспериментальная.
              \en Experimental.
    \ingroup Data_Interface
*/
// ---
struct CONV_CLASS ObReplicaCurve3D {
  static const int CurveType = 18;
  int baseCurve;
  ObLocation3D location;
};

} // namespace c3d_conv_user

#endif // __CONV_OB_IMPORT_GEOMETRY_H
