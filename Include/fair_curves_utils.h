////////////////////////////////////////////////////////////////////////////////
//
// Fair curves utils
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __FAIR_CURVES_UTILS_H
#define __FAIR_CURVES_UTILS_H

#include <mb_operation_result.h>
#include <mb_cart_point3d.h>
#include <templ_s_array.h>

//------------------------------------------------------------------------------
  /** \brief \ru Нахождение точки перегиба кубической кривой Безье по двум концевым точкам,
             двум касательным и двум значениям кривизны.
             \en Find an inflection point of a cubic Bezier curve by two bounding points,
             two tangents and two curvatures.
    \details \ru Нахождение точки перегиба и касательной в ней для кубической кривой Безье
             по двум концевым точкам, двум касательным и двум значениям кривизны.
             \en Find an inflection point and a tangent in it for a cubic Bezier curve
             by two bounding points, two tangents and two curvatures.
    \param[in]  p1       - \ru Первая точка.        \en First point.
    \param[in]  v1       - \ru Первая касательная.  \en First tangent.
    \param[in]  p2       - \ru Вторая точка.        \en Second point.
    \param[in]  v2       - \ru Вторая касательная.  \en Second tangent.
    \param[in]  cvt0     - \ru Первая кривизна.     \en First curvature.
    \param[in]  cvt1     - \ru Вторая кривизна.     \en Second curvature.
    \param[out] p3       - \ru Результирующая точка.\en Result point.
    \param[out] v3       - \ru Результирующая касательная.\en Result tangent.
    \return                \ru Возвращает значение результата операции.
                           \en Returns operation result value.
*/
// ---
MATH_FUNC( MbResultType ) BezierCurveInflection( const MbCartPoint3D & p1,
                                                 const MbVector3D & v1,
                                                 const MbCartPoint3D & p2,
                                                 const MbVector3D & v2,
                                                 double cvt0,
                                                 double cvt1,
                                                 MbCartPoint3D & p3,
                                                 MbVector3D & v3 );

//------------------------------------------------------------------------------
  /** \brief \ru Нахождение точки пересечения касательных. \en Find intersection point of two tangents.
    \details \ru Нахождение точки пересечения касательных. \en Find intersection point of two tangents.
    \param[in]  r0       - \ru Первая точка.              \en First point.
    \param[in]  r1       - \ru Вторая точка.              \en Second point.
    \param[in]  dr0      - \ru Первая касательная.        \en First tangent.
    \param[in]  dr1      - \ru Вторая касательная.        \en Second tangent.
    \param[out] p3       - \ru Результирующая точка.      \en Result point.
    \return                \ru Возвращает значение результата операции.
                           \en Returns operation result value.
*/
// ---
MATH_FUNC( MbResultType ) IntersectLines( const MbCartPoint3D & r0,
                                          const MbCartPoint3D & r1,
                                          const MbVector3D & dr0,
                                          const MbVector3D & dr1,
                                          MbCartPoint3D & p3 );


//------------------------------------------------------------------------------
  /** \brief \ru Определение точек пересечения скрещивающихся прямых. \en Find intersection point of two intersecting lines.
    \details \ru Определение точек пересечения скрещивающихся прямых. \en Find intersection point of two intersecting lines.
    \param[in]  p10 - \ru Первая точка.         \en First point.
    \param[in]  p11 - \ru Первая касательная.   \en First tangent.
    \param[in]  p20 - \ru Вторая точка.         \en Second point.
    \param[in]  p21 - \ru Вторая касательная.   \en Second tangent.
    \param[out] xyz - \ru Результирующая точка. \en Result point.
    \return           \ru Возвращает значение результата операции.
                      \en Returns operation result value.
*/
// ---
MATH_FUNC( MbResultType )
LineIntersectIn3DTest( const MbCartPoint3D & p10, const MbVector3D & p11, // первая точка с касательной
                       const MbCartPoint3D & p20, const MbVector3D & p21, // вторая ...
                       MbCartPoint3D & xyz );
// DEBUG20211217 BEGIN


//------------------------------------------------------------------------------
  /** \brief \ru Уплотнить точки на окружности. \en Condense the points on the circle.
    \details \ru Уплотнить точки на окружности. Определить касательные к точкам на окружности.
             \en Condense the points on the circle. Find tangents to the points onthe circle.
    \param[in]  polyLine       - \ru Набор из трех точек.   \en A set of three points.
    \param[in]  tangents       - \ru Набор касательных.     \en A set of tangents.
    \param[out] polyLineRezult - \ru Результирующие точки.  \en Result points.
    \return           \ru Возвращает значение результата операции.
                      \en Returns operation result value.
*/
// ---
MATH_FUNC( MbResultType )
TangentsOnCirleTest( SArray<MbCartPoint3D> & polyLine,  // 3 точки
                     SArray<MbVector3D> & tangents,
                     SArray<MbCartPoint3D> & polyLineRezult,   // Результат 5 точек
                     double & cvt );


#endif // __FAIR_CURVES_UTILS_H