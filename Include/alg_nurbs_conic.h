////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение конических сечений в виде NURBS-кривой.
         \en Construction of conic sections as NURBS curves. \~
  \details \ru Построение конических сечений производится следующими способами: 
    по двум точкам, вершине и дискриминанту, по трем точкам и вершине, 
    по трем точкам и двум наклонам, по двум точкам, двум наклонам и дискриминанту, 
    по четырем точкам и наклону и по пяти точкам. \n
    NURBS кривая, описывающая конику, строится по трем точкам: началу и концу коники и 
    средней точке (вершине угола, в который надо вписать конику).
    Принимая весы начальной и конечной точки равными 1 и рассчитывая вес средней точки, 
    по трем точкам и трем весам строится NURBS 3-го порядка, который будет искомой коникой.
           \en Construction of conic sections is performed in the following way: 
    by two points, a vertex and a discriminant, by three points and a vertex, 
    by three points and two inclinations, by two points, two inclinations and discriminant, 
    by four points  and inclination and by five points. \n
    A NURBS curve describing a conic is constructed by three points: a start and an end of a conic and 
    an average point (a vertex of angle which should be inscribed into the conic).
    Let weights of the start point and the end point be equal to 1. After calculating of the weight of the average point 
    NURBS of third degree is constructed by these three weights. This NURBS is the required conic. \~

*/
////////////////////////////////////////////////////////////////////////////////


#ifndef __ALG_NURBS_CONIC_H
#define __ALG_NURBS_CONIC_H


#include <alg_curve_distance.h>


class MbCurve3D;
class MbNurbs3D;
class MbCartPoint3D;
class MbVector3D;


//------------------------------------------------------------------------------
/** \brief \ru Построить коническое сечение по двум точкам вершине и дискриминанту.
           \en Construct a conic section by two points, an angle vertex and a discriminant. \~
  \details \ru Построение конического сечения в виде NURBS-кривой 3-го порядка по 
    двум точкам, которые задают начало и конец кривой, вершине инженерного 
    треугольника и дискриминанту, который используется для определения третьей точки кривой.
           \en Construction of a conic section as a NURBS curve of the third degree by 
    two points setting ends of a curve, a vertex of enginer 
    triangle and a discriminant which is used for the definition of the third point. \~
  \param[in] mbPoint0 - \ru Координаты начала коники.
                        \en Coordinates of the conic start point. \~
  \param[in] mbPoint1 - \ru Координаты вершины угла, в который надо вписать конику.
                        \en Coordinates of the vertex of angle which should be inscribed into the conic. \~
  \param[in] mbPoint2 - \ru Координаты конца коники.
                        \en Coordinates of the conic end point. \~
  \param[in] fDiscr - \ru Дискриминант < 1, если задать дискриминант >= 1, то он 
    автоматически будет сброшен до значения 0.99999999.
                      \en The discriminant is less than 1. Otherwise it 
    will be set to 0.99999999 automatically. \~
  \return \ru Указатель на построенную кривую \n
    nullptr, если не удалось построить конику для заданных параметров.
          \en The pointer to the constructed curve \n
    is nullptr if a try to construct a conic for a given parameters has failed. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC ( MbCurve3D * ) NurbsConic_1( const MbCartPoint3D & mbPoint0, const MbCartPoint3D & mbPoint1, 
                                        const MbCartPoint3D & mbPoint2, double fDiscr );


//------------------------------------------------------------------------------
/** \brief \ru Построить коническое сечение по двум точкам вершине и дискриминанту.
           \en Construct a conic section by two points, an angle vertex and a discriminant. \~
  \details \ru Построение конического сечения в виде NURBS-кривой 3-го порядка по 
    двум точкам, которые задают начало и конец кривой, вершине инженерного 
    треугольника и дискриминанту, который используется для определения третьей точки кривой.
           \en Construction of a conic section as a NURBS curve of the third degree by 
    two points setting ends of a curve, a vertex of enginer 
    triangle and a discriminant which is used for the definition of the third point. \~
  \param[in] mbPoint0 - \ru Координаты начала коники.
                        \en Coordinates of the conic start point. \~
  \param[in] mbPoint1 - \ru Координаты вершины угла, в который надо вписать конику.
                        \en Coordinates of the vertex of angle which should be inscribed into the conic. \~
  \param[in] mbPoint2 - \ru Координаты конца коники.
                        \en Coordinates of the conic end point. \~
  \param[in] fDiscr - \ru Дискриминант < 1, если задать дискриминант >= 1, то он 
    автоматически будет сброшен до значения 0.99999999.
                      \en The discriminant is less than 1. Otherwise it 
    will be set to 0.99999999 automatically. \~
  \return \ru Указатель на построенную кривую \n
    nullptr, если не удалось построить конику для заданных параметров.
          \en The pointer to the constructed curve \n
    is nullptr if a try to construct a conic for a given parameters has failed. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC ( MbCurve * ) NurbsConic_1( const MbCartPoint & mbPoint0, const MbCartPoint & mbPoint1, 
                                       const MbCartPoint & mbPoint2, double fDiscr );


//------------------------------------------------------------------------------
/** \brief \ru Построить коническое сечение по трем точкам и вершине.
           \en Construct a conic section by three points, and an angle vertex. \~
  \details \ru Построение конического сечения в виде NURBS-кривой 3-го порядка по 
    трем точкам: началу, концу и средней точке кривой, а также вершине угла, в который 
    должна быть вписана коника.
           \en Construction of a conic section as a NURBS curve of the third degree by 
    three points: ends of a curve, its average point and by a vertex of an angle, 
    a conic should be inscribed in. \~
  \param[in] vmbConicPoints - \ru Контейнер точек коники: начало, средняя точка, конец; 
    точек должно быть 3.
                              \en The container for points of a conic: start point, average point and end point; 
    there should be exactly 3 points. \~
  \param[in] mbVertex - \ru Координаты вершины угла, в который надо вписать конику.
                        \en Coordinates of the vertex of angle which should be inscribed into the conic. \~
  \return \ru Указатель на построенную кривую \n
    nullptr, если не удалось постороить конику для заданных параметров.
          \en The pointer to the constructed curve \n
    is nullptr if a try to construct a conic for given parameters has failed. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC ( MbCurve3D * ) NurbsConic_2( std::vector<MbCartPoint3D> & vmbConicPoints, const MbCartPoint3D & mbVertex );


//------------------------------------------------------------------------------
/** \brief \ru Построить коническое сечение по трем точкам и вершине.
           \en Construct a conic section by three points, and an angle vertex. \~
  \details \ru Построение конического сечения в виде NURBS-кривой 3-го порядка по 
    трем точкам: началу, концу и средней точке кривой, а также вершине угла, в который 
    должна быть вписана коника.
           \en Construction of a conic section as a NURBS curve of the third degree by 
    three points: ends of a curve, its average point and by a vertex of an angle, 
    a conic should be inscribed in. \~
  \param[in] vmbConicPoints - \ru Контейнер точек коники: начало, средняя точка, конец; 
    точек должно быть 3.
                              \en The container for points of a conic: start point, average point and end point; 
    there should be exactly 3 points. \~
  \param[in] mbVertex - \ru Координаты вершины угла, в который надо вписать конику.
                        \en Coordinates of the vertex of angle which should be inscribed into the conic. \~
  \return \ru Указатель на построенную кривую \n
    nullptr, если не удалось постороить конику для заданных параметров.
          \en The pointer to the constructed curve \n
    is nullptr if a try to construct a conic for given parameters has failed. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC ( MbCurve * ) NurbsConic_2( std::vector<MbCartPoint> & vmbConicPoints, const MbCartPoint & mbVertex );


//------------------------------------------------------------------------------
/** \brief \ru Построить коническое сечение по трем точкам и двум наклонам.
           \en Construct a conic section by three points and two inclinations. \~
  \details \ru Построение конического сечения в виде NURBS-кривой 3-го порядка по 
    3-ем точкам, которые задают начало, конец и среднюю точку кривой и двум 
    наклонам, выходящим из начальной и конечной точек.
           \en Construction of a conic section as a NURBS curve of the third degree by 
    3 points setting begin, end and an average point of a curve and two 
    inclinations outgoing from the start point and from the end point \~
  \param[in] vmbConicPoints - \ru Контейнер точек коники: начало, средняя точка, конец;
    точек должно быть 3.
                              \en The container for points of a conic: start point, average point and end point;
    there should be exactly 3 points. \~
  \param[in] mbTangent1 - \ru Наклон в начале кривой.
                          \en Inclination at start of a curve. \~
  \param[in] mbTangent2 - \ru Наклон в конце кривой.
                          \en Inclination at end of a curve. \~
  \return \ru Указатель на построенную кривую \n
    nullptr, если не удалось постороить конику для заданных параметров.
          \en The pointer to the constructed curve \n
    is nullptr if a try to construct a conic for given parameters has failed. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC ( MbCurve3D * ) NurbsConic_3( const std::vector<MbCartPoint3D> & vmbConicPoints, 
                                        MbVector3D & mbTangent1, MbVector3D & mbTangent2 );


//------------------------------------------------------------------------------
/** \brief \ru Построить коническое сечение по трем точкам и двум наклонам.
           \en Construct a conic section by three points and two inclinations. \~
  \details \ru Построение конического сечения в виде NURBS-кривой 3-го порядка по 
    3-ем точкам, которые задают начало, конец и среднюю точку кривой и двум 
    наклонам, выходящим из начальной и конечной точек.
           \en Construction of a conic section as a NURBS curve of the third degree by 
    3 points setting begin, end and an average point of a curve and two 
    inclinations outgoing from the start point and from the end point \~
  \param[in] vmbConicPoints - \ru Контейнер точек коники: начало, средняя точка, конец;
    точек должно быть 3.
                              \en The container for points of a conic: start point, average point and end point;
    there should be exactly 3 points. \~
  \param[in] mbTangent1 - \ru Наклон в начале кривой.
                          \en Inclination at start of a curve. \~
  \param[in] mbTangent2 - \ru Наклон в конце кривой.
                          \en Inclination at end of a curve. \~
  \return \ru Указатель на построенную кривую \n
    nullptr, если не удалось постороить конику для заданных параметров.
          \en The pointer to the constructed curve \n
    is nullptr if a try to construct a conic for given parameters has failed. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC ( MbCurve * ) NurbsConic_3( const std::vector<MbCartPoint> & vmbConicPoints, MbVector & mbTangent1, MbVector & mbTangent2 );


//------------------------------------------------------------------------------
/** \brief \ru Построить коническое сечение по двум точкам, двум наклонам и дискриминанту.
           \en Construct a conic section by two points, two inclinations and a discriminant. \~
  \details \ru Построение конического сечения в виде NURBS-кривой 3-го порядка по 
    2-ум точкам, которые задают начало и конец кривой, двум наклонам, выходящим из этих точек 
    и дискриминанту.
           \en Construction of a conic section as a NURBS curve of the third degree by 
    2 points setting start and end of a curve, two incllinations outgoing from these points 
    and a discriminant. \~
  \param[in] mbPoint1 - \ru Координаты начала коники.
                        \en Coordinates of the conic start point. \~
  \param[in] mbPoint2 - \ru Координаты конца коники.
                        \en Coordinates of the conic end point. \~
  \param[in] mbTangent1 - \ru Наклон в начале коники.
                          \en Inclination at start of conic. \~
  \param[in] mbTangent2 - \ru Наклон в конце коники.
                          \en Inclination at end of conic. \~
  \param[in] fDiscr - \ru Дискриминант < 1, если задать дискриминант >= 1, то он 
    автоматически будет сброшен до значения 0.99999999.
                      \en The discriminant is less than 1. Otherwise it 
    will be set to 0.99999999 automatically. \~
  \return \ru Указатель на построенную кривую \n
    nullptr, если не удалось построить конику для заданных параметров.
          \en The pointer to the constructed curve \n
    is nullptr if a try to construct a conic for given parameters has failed. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC ( MbCurve3D * ) NurbsConic_4( const MbCartPoint3D & mbPoint1, const MbCartPoint3D & mbPoint2, 
                                        const MbVector3D & mbTangent1, const MbVector3D & mbTangent2, double fDiscr );


//------------------------------------------------------------------------------
/** \brief \ru Построить коническое сечение по двум точкам, двум наклонам и дискриминанту.
           \en Construct a conic section by two points, two inclinations and a discriminant. \~
  \details \ru Построение конического сечения в виде NURBS-кривой 3-го порядка по 
    2-ум точкам, которые задают начало и конец кривой, двум наклонам, выходящим из этих точек 
    и дискриминанту.
           \en Construction of a conic section as a NURBS curve of the third degree by 
    2 points setting start and end of a curve, two incllinations outgoing from these points 
    and a discriminant. \~
  \param[in] mbPoint1 - \ru Координаты начала коники.
                        \en Coordinates of the conic start point. \~
  \param[in] mbPoint2 - \ru Координаты конца коники.
                        \en Coordinates of the conic end point. \~
  \param[in] mbTangent1 - \ru Наклон в начале коники.
                          \en Inclination at start of conic. \~
  \param[in] mbTangent2 - \ru Наклон в конце коники.
                          \en Inclination at end of conic. \~
  \param[in] fDiscr - \ru Дискриминант < 1, если задать дискриминант >= 1, то он 
    автоматически будет сброшен до значения 0.99999999.
                      \en The discriminant is less than 1. Otherwise it 
    will be set to 0.99999999 automatically. \~
  \return \ru Указатель на построенную кривую \n
    nullptr, если не удалось построить конику для заданных параметров.
          \en The pointer to the constructed curve \n
    is nullptr if a try to construct a conic for given parameters has failed. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC ( MbCurve * ) NurbsConic_4( const MbCartPoint & mbPoint1, const MbCartPoint & mbPoint2, 
                                      const MbVector & mbTangent1, const MbVector & mbTangent2, double fDiscr );


//------------------------------------------------------------------------------
/** \brief \ru Построить коническое сечение по четырем точкам и наклону.
           \en Construct a conic section by four points, and an inclination. \~
  \details \ru Построение конического сечения в виде NURBS-кривой 3-го порядка по 
    4-ем точкам и наклону в первой из них. \n
    Путем подставления начальных точек в общее уравнение коники Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0 
    и касательной к ней в начальной точке (x1, y1): (2Ax1 + By1 + D)(x - x1) + (2Cy1 + Bx1 + E)(y - y1) = 0 
    получим СЛАУ. Решив СЛАУ относительно параметров A,B,C,D,E, найдем искомую конику.
           \en Construction of a conic section as a NURBS curve of the third degree by 
    4 points and inclination in the first of them. \n
    By substituting of start points in the common equation of the conic Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0 
    and its tangent at the start point (x1, y1): (2Ax1 + By1 + D)(x - x1) + (2Cy1 + Bx1 + E)(y - y1) = 0 
    we get the SLAE. Having SLAE solved relative to parameters A,B,C,D,E we find the required conic. \~
  \param[in] vmbConicPoints - \ru Контейнер точек коники: первая точка начальная, последняя - конечная;
    точек должно быть 4.
                              \en The container for points of a conic: the first point is start point, the last point is end point.
    there should be exactly 4 points. \~
  \param[in] mbTangent1 - \ru Наклон в точке коники.
                          \en Inclination at point of conic. \~
  \param[in] tanPntNb - \ru Номер точке, в которой задан наклон.
                        \en Point number at which the inclination is specified. \~
  \return \ru Указатель на построенную кривую \n
    nullptr, если не удалось постороить конику для заданных параметров.
          \en The pointer to the constructed curve \n
    is nullptr if a try to construct a conic for given parameters has failed. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC ( MbCurve3D * ) NurbsConic_5( const std::vector<MbCartPoint3D> & vmbConicPoints, MbVector3D & mbTangent1, size_t tanPntNb = 1 );


//------------------------------------------------------------------------------
/** \brief \ru Построить коническое сечение по четырем точкам и наклону.
           \en Construct a conic section by four points, and an inclination. \~
  \details \ru Построение конического сечения в виде NURBS-кривой 3-го порядка по 
    4-ем точкам и наклону в первой из них. \n
    Путем подставления начальных точек в общее уравнение коники Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0 
    и касательной к ней в начальной точке (x1, y1): (2Ax1 + By1 + D)(x - x1) + (2Cy1 + Bx1 + E)(y - y1) = 0 
    получим СЛАУ. Решив СЛАУ относительно параметров A,B,C,D,E, найдем искомую конику.
           \en Construction of a conic section as a NURBS curve of the third degree by 
    4 points and inclination in the first of them. \n
    By substituting of start points in the common equation of the conic Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0 
    and its tangent at the start point (x1, y1): (2Ax1 + By1 + D)(x - x1) + (2Cy1 + Bx1 + E)(y - y1) = 0 
    we get the SLAE. Having SLAE solved relative to parameters A,B,C,D,E we find the required conic. \~
  \param[in] vmbConicPoints - \ru Контейнер точек коники: первая точка начальная, последняя - конечная;
    точек должно быть 4.
                              \en The container for points of a conic: the first point is start point, the last point is end point.
    there should be exactly 4 points. \~
  \param[in] mbTangent1 - \ru Наклон в точке коники.
                          \en Inclination at point of conic. \~
  \param[in] tanPntNb - \ru Номер точке, в которой задан наклон.
                        \en Point number at which the inclination is specified. \~
  \return \ru Указатель на построенную кривую \n
    nullptr, если не удалось постороить конику для заданных параметров.
          \en The pointer to the constructed curve \n
    is nullptr if a try to construct a conic for given parameters has failed. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC ( MbCurve * ) NurbsConic_5( const std::vector<MbCartPoint> & vmbConicPoints, MbVector & mbTangent1, size_t tanPntNb = 1 );


//------------------------------------------------------------------------------
/** \brief \ru Построить коническое сечение по пяти точкам.
           \en Construct a conic section by five points. \~
  \details \ru Построение конического сечения в виде NURBS-кривой 3-го порядка по 5-ти точкам.\n
    Путем подставления начальных точек в общее уравнение коники Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0 получим СЛАУ. 
    Решив СЛАУ относительно параметров A,B,C,D,E, найдем искомую конику.
           \en Construction of a conic section as a NURBS curve of the third degree by 5 points.\n
    By substituting of start points in the common equation of the conic Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0 we get the SLAE. 
    Having SLAE solved relative to parameters A,B,C,D,E we find the required conic. \~
  \param[in] vmbConicPoints - \ru Контейнер точек коники: первая точка начальная, последняя - конечная; 
    точек должно быть 5.
                              \en The container for points of a conic: the first point is start point, the last point is end point. 
    there should be exactly 5 points. \~
  \return \ru Указатель на построенную кривую \n
    nullptr, если не удалось постороить конику для заданных параметров.
          \en The pointer to the constructed curve \n
    is nullptr if a try to construct a conic for given parameters has failed. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC ( MbCurve3D * ) NurbsConic_6( const std::vector<MbCartPoint3D> & vmbConicPoints );


//------------------------------------------------------------------------------
/** \brief \ru Построить коническое сечение по пяти точкам.
           \en Construct a conic section by five points. \~
  \details \ru Построение конического сечения в виде NURBS-кривой 3-го порядка по 5-ти точкам.\n
    Путем подставления начальных точек в общее уравнение коники Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0 получим СЛАУ. 
    Решив СЛАУ относительно параметров A,B,C,D,E, найдем искомую конику.
           \en Construction of a conic section as a NURBS curve of the third degree by 5 points.\n
    By substituting of start points in the common equation of the conic Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0 we get the SLAE. 
    Having SLAE solved relative to parameters A,B,C,D,E we find the required conic. \~
  \param[in] vmbConicPoints - \ru Контейнер точек коники: первая точка начальная, последняя - конечная; 
    точек должно быть 5.
                              \en The container for points of a conic: the first point is start point, the last point is end point. 
    there should be exactly 5 points. \~
  \return \ru Указатель на построенную кривую \n
    nullptr, если не удалось постороить конику для заданных параметров.
          \en The pointer to the constructed curve \n
    is nullptr if a try to construct a conic for given parameters has failed. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC ( MbCurve * ) NurbsConic_6( const std::vector<MbCartPoint> & vmbConicPoints );


#endif // __ALG_NURBS_CONIC_H
