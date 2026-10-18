////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Определение расстояния между объектами.
         \en Definition of distance between objects. \~
  \details \ru Функции определения максимальных расстояний между различными 
    трехмерными объектами.
           \en Functions for definition of maximal distances between different 
    three-dimensional objects. \~

*/
////////////////////////////////////////////////////////////////////////////////


#ifndef __ALG_MAX_DISTANCE_H
#define __ALG_MAX_DISTANCE_H


#include <math_define.h>


class    MbCartPoint;
class    MbCartPoint3D;
class    MbAxis3D;
class    MbCurve3D;
class    MbSurface;


//------------------------------------------------------------------------------
/** \brief \ru Найти максимальное расстояние между точкой и кривой.
           \en Find the maximal distance between a point and a curve. \~
  \details \ru Максимальное расстояние между точкой и кривой.
           \en The maximal distance between a point and a curve. \~
  \param[in] pnt - \ru Исходная точка.
                   \en The initial point. \~
  \param[in] curv - \ru Исходная кривая.
                    \en The initial curve. \~
  \param[out] t - \ru Параметр на кривой, при котором достигается искомое расстояние.
                  \en The parameter on a curve where the required distance is reached. \~
  \param[out] distance - \ru Искомое расстояние.
                         \en The required distance. \~
  \return \ru true, если максимальное расстояние было найдено.
          \en true if the maximal distance has been found. \~
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC (bool) MaxDistance( const MbCartPoint3D & pnt, 
                              const MbCurve3D &     curv,
                                    double &        t,
                                    double &        distance );


//------------------------------------------------------------------------------
/** \brief \ru Найти максимальное расстояние между двумя кривыми.
           \en Find the maximal distance between two curves. \~
  \details \ru Найти максимальное расстояние между двумя кривыми.
           \en Find the maximal distance between two curves. \~
  \param[in] curv1, curv2 - \ru Исходные кривая.
                            \en The initial curves. \~
  \param[out] t1, t2 - \ru Параметры на кривых, при которых достигается искомое расстояние.
                       \en The parameters on curves where the required distance is reached. \~
  \param[out] distance - \ru Искомое расстояние.
                         \en The required distance. \~
  \return \ru true, если максимальное расстояние было найдено.
          \en true if the maximal distance has been found. \~
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC (bool) MaxDistance( const MbCurve3D & curv1, 
                              const MbCurve3D & curv2, 
                                    double &    t1,
                                    double &    t2,
                                    double &    distance );


//------------------------------------------------------------------------------
/** \brief \ru Найти максимальное расстояние между точкой и поверхностью.
           \en Find the maximal distance between a point and a surface. \~
  \details \ru Найти максимальное расстояние между точкой и поверхностью.
           \en Find the maximal distance between a point and a surface. \~
  \param[in] pnt - \ru Исходная точка.
                   \en The initial point. \~
  \param[in] surf - \ru Исходная поверхность.
                    \en The initial surface. \~
  \param[out] uv - \ru Параметры точки на поверхности, при которой достигается искомое расстояние.
                   \en The point parameters on a surface where the required distance is reached. \~
  \param[out] distance - \ru Искомое расстояние.
                         \en The required distance. \~
  \return \ru true, если максимальное расстояние было найдено.
          \en true if the maximal distance has been found. \~
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC (bool) MaxDistance( const MbCartPoint3D & pnt, 
                              const MbSurface &     surf, 
                                    MbCartPoint &   uv,
                                    double &        distance );


//------------------------------------------------------------------------------
/** \brief \ru Найти максимальное расстояние между кривой и поверхностью.
           \en Find the maximal distance between a curve and a surface. \~
  \details \ru Найти максимальное расстояние между кривой и поверхностью.
           \en Find the maximal distance between a curve and a surface. \~
  \param[in] curv - \ru Исходная кривая.
                    \en The initial curve. \~
  \param[in] surf - \ru Исходная поверхность.
                    \en The initial surface. \~
  \param[out] t - \ru Параметр на кривой, при котором достигается искомое расстояние.
                  \en The parameter on a curve where the required distance is reached. \~
  \param[out] uv - \ru Параметры точки на поверхности, при которой достигается искомое расстояние.
                   \en The point parameters on a surface where the required distance is reached. \~
  \param[out] distance - \ru Искомое расстояние.
                         \en The required distance. \~
  \return \ru true, если максимальное расстояние было найдено.
          \en true if the maximal distance has been found. \~
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC (bool) MaxDistance( const MbCurve3D &   curv, 
                              const MbSurface &   surf, 
                                    double &      t, 
                                    MbCartPoint & uv,
                                    double &      distance );


//------------------------------------------------------------------------------
/** \brief \ru Найти максимальное расстояние между поверхностями.
           \en Find the maximal distance between two surfaces. \~
  \details \ru Найти максимальное расстояние между поверхностями.
           \en Find the maximal distance between two surfaces. \~
  \param[in] surf1, surf2 - \ru Исходные поверхности.
                            \en The initial surfaces. \~
  \param[out] uv1, uv2 - \ru Параметры точек на поверхностях, при которых достигается искомое расстояние.
                         \en The parameters on surfaces where the required distance is reached. \~
  \param[out] distance - \ru Искомое расстояние.
                         \en The required distance. \~
  \return \ru true, если максимальное расстояние было найдено.
          \en true if the maximal distance has been found. \~
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC (bool) MaxDistance( const MbSurface &   surf1,
                              const MbSurface &   surf2,
                                    MbCartPoint & uv1,
                                    MbCartPoint & uv2,
                                    double &      distance );


//------------------------------------------------------------------------------
/** \brief \ru Найти максимальное расстояние от оси до кривой.
           \en Find the maximal distance between an axis an a curve. \~
  \details \ru Ищется максимальное расстояние от оси до кривой перпендикулярно оси.
           \en Find the maximal distance between an axis and a curve perpendicularly to an axis. \~
  \param[in] axis - \ru Исходная ось.
                    \en The initial axis. \~
  \param[in] curve - \ru Исходная кривая.
                     \en The initial curve. \~
  \param[out] param - \ru Параметр на кривой, при котором достигается искомое расстояние.
                      \en The parameter on a curve where the required distance is reached. \~
  \param[out] distance - \ru Искомое расстояние.
                         \en The required distance. \~
  \return \ru true, если максимальное расстояние было найдено.
          \en true if the maximal distance has been found. \~
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC (bool) MaxDistance( const MbAxis3D &  axis, 
                              const MbCurve3D & curve,
                                    double &    param,
                                    double &    distance );


#endif // __ALG_MAX_DISTANCE_H
