////////////////////////////////////////////////////////////////////////////////
/**
\file
\brief \ru Функции для анализа нормалей и кривизны поверхностей и кривых.
       \en Functions for normals and curvature analysis of surfaces and curves. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ACTION_CURVATURE_ANALYSIS_H
#define __ACTION_CURVATURE_ANALYSIS_H


#include <surface.h>
#include <cur_surface_intersection.h>
#include <topology.h>


class MbCurveMinMaxCurvatureParams;
class MbCurveMinMaxCurvatureResults;


//------------------------------------------------------------------------------
/** \brief \ru Алгоритмы поиска экстремумов на поверхности.
           \en Algorithms for finding extremes on the surface. \~
  \details \ru Константы, задающие вызываемый алгоритм поиска экстремальных значений функции на поверхности.
           \en Constants defining the called algorithm for searching for extreme values of a function on a surface. \~
  \ingroup  Algorithms_3D
*/
enum MbeExtremsSearchingMethod
{
  esm_GradientDescent = 1, ///< \ru Метод градиентного спуска. \en Gradient Descent Method. 
  esm_LineSegregation = 2, ///< \ru Метод выделения линий смены убывания / возрастания функции по u и по v. \en The method of segregation of lines of change of decrease / increase of the function in u and v directions.
  esm_AdaptiveCells   = 3, ///< \ru Метод адаптивного дробления ячеек. \en Adaptive cell splitting method.
};


//------------------------------------------------------------------------------
/** \brief \ru Функция, заданная на поверхности. 
           \en The function define on the surface. \~
  \details \ru Рассчитывает значение самой функции и ее градиент.
           \en Calculates the value of the function itself and its gradient. \~
  \ingroup  Algorithms_3D
*/
typedef void( *SurfaceFunction )( const MbSurface   & surf, // Поверхность,
                                  const MbCartPoint & pnt,  // точка на поверхности
                                        double      & func, // рассчитываемое значение функции,
                                        MbVector    * der );// рассчитываемое значение вектора градиента (если указатель не нулевой).

//------------------------------------------------------------------------------
/** \brief \ru Вычислить в точке поверхности минимальную нормальную кривизну, а также ее градиент.
           \en Calculate at the point of the surface the minimum normal curvature, as well as its gradient. \~
  \details \ru Вычисляется значение кривизны и опционально значение ее градиента в точке (если передается ненулевой указатель).
           \en The curvature value is calculated and, optionally, its gradient value at a point (if a non-zero pointer is passed). \~
  \param[in] surf - \ru Поверхность.
                    \en Surface. \~
  \param[in] pnt - \ru Точка расчета.
                   \en Point of calculation. \~
  \param[out] func - \ru Рассчитываемое значение кривизны.
                     \en Calculated curvature value. \~
  \param[out] der - \ru Рассчитываемое значение градиента кривизны.
                    \en The calculated value of the curvature gradient. \~
  \ingroup  Algorithms_3D
*/ 
MATH_FUNC( void ) MinSurfaceCurvature( const MbSurface &   surf, 
                                       const MbCartPoint & pnt, 
                                             double &      func, 
                                             MbVector *    der = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить в точке поверхности максимальную нормальную кривизну, а также ее градиент.
           \en Calculate at the point of the surface the maximum normal curvature, as well as its gradient. \~
  \details \ru Вычисляется значение кривизны и опционально значение ее градиента в точке (если передается ненулевой указатель).
           \en The curvature value is calculated and, optionally, its gradient value at a point (if a non-zero pointer is passed). \~
  \param[in] surf - \ru Поверхность.
                    \en Surface. \~
  \param[in] pnt - \ru Точка расчета.
                   \en Point of calculation. \~
  \param[out] func - \ru Рассчитываемое значение кривизны.
                     \en Calculated curvature value. \~
  \param[out] der - \ru Рассчитываемое значение градиента кривизны.
                    \en The calculated value of the curvature gradient. \~
  \ingroup  Algorithms_3D
*/ 
MATH_FUNC( void ) MaxSurfaceCurvature( const MbSurface &   surf, 
                                       const MbCartPoint & pnt, 
                                             double &      func, 
                                             MbVector *    der = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить в точке поверхности гауссову кривизну, а также ее градиент.
           \en Calculate at a surface point the Gaussian curvature, as well as its gradient. \~
  \details \ru Вычисляется значение кривизны и опционально значение ее градиента в точке (если передается ненулевой указатель).
           \en The curvature value is calculated and, optionally, its gradient value at a point (if a non-zero pointer is passed). \~
  \param[in] surf - \ru Поверхность.
                    \en Surface. \~
  \param[in] pnt - \ru Точка расчета.
                   \en Point of calculation. \~
  \param[out] func - \ru Рассчитываемое значение кривизны.
                     \en Calculated curvature value. \~
  \param[out] der - \ru Рассчитываемое значение градиента кривизны.
                    \en The calculated value of the curvature gradient. \~
  \ingroup  Algorithms_3D
*/ 
MATH_FUNC( void ) GaussCurvature( const MbSurface &   surf, 
                                  const MbCartPoint & pnt, 
                                        double &      func, 
                                        MbVector *    der = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить в точке поверхности среднюю кривизну, а также ее градиент.
           \en Calculate at the point of the surface the mean curvature, as well as its gradient. \~
  \details \ru Вычисляется значение кривизны и опционально значение ее градиента в точке (если передается ненулевой указатель).
           \en The curvature value is calculated and, optionally, its gradient value at a point (if a non-zero pointer is passed). \~
  \param[in] surf - \ru Поверхность.
                    \en Surface. \~
  \param[in] pnt - \ru Точка расчета.
                   \en Point of calculation. \~
  \param[out] func - \ru Рассчитываемое значение кривизны.
                     \en Calculated curvature value. \~
  \param[out] der - \ru Рассчитываемое значение градиента кривизны.
                    \en The calculated value of the curvature gradient. \~
  \ingroup  Algorithms_3D
*/ 
MATH_FUNC( void ) MeanCurvature( const MbSurface &   surf, 
                                 const MbCartPoint & pnt, 
                                       double &      func, 
                                       MbVector *    der = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить в точке поверхности нормальную кривизна в направлении u , а также ее градиент.
           \en Calculate at the surface point the normal curvature in the direction of u, as well as its gradient. \~
  \details \ru Вычисляется значение кривизны и опционально значение ее градиента в точке (если передается ненулевой указатель).
           \en The curvature value is calculated and, optionally, its gradient value at a point (if a non-zero pointer is passed). \~
  \param[in] surf - \ru Поверхность.
                    \en Surface. \~
  \param[in] pnt - \ru Точка расчета.
                   \en Point of calculation. \~
  \param[out] func - \ru Рассчитываемое значение кривизны.
                     \en Calculated curvature value. \~
  \param[out] der - \ru Рассчитываемое значение градиента кривизны.
                    \en The calculated value of the curvature gradient. \~
  \ingroup  Algorithms_3D
*/ 
MATH_FUNC( void ) UNormalCurvature( const MbSurface &   surf, 
                                    const MbCartPoint & pnt, 
                                          double &      func, 
                                          MbVector *    der = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить в точке поверхности нормальную кривизна в направлении v, а также ее градиент.
           \en Calculate at the surface point the normal curvature in the direction of v, as well as its gradient. \~
  \details \ru Вычисляется значение кривизны и опционально значение ее градиента в точке (если передается ненулевой указатель).
           \en The curvature value is calculated and, optionally, its gradient value at a point (if a non-zero pointer is passed). \~
  \param[in] surf - \ru Поверхность.
                    \en Surface. \~
  \param[in] pnt - \ru Точка расчета.
                   \en Point of calculation. \~
  \param[out] func - \ru Рассчитываемое значение кривизны.
                     \en Calculated curvature value. \~
  \param[out] der - \ru Рассчитываемое значение градиента кривизны.
                    \en The calculated value of the curvature gradient. \~
  \ingroup  Algorithms_3D
*/ 
MATH_FUNC( void ) VNormalCurvature( const MbSurface &   surf, 
                                    const MbCartPoint & pnt, 
                                          double &      func,
                                          MbVector *    der = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки поверхности, в которых выбранная кривизна принимает наибольшие по модулю значения.
           \en Find the points of the surface at which the selected curvature takes the largest in modulus values. \~
  \details \ru Ищутся точки, в которых выбранная кривизна принимает на поверхности наибольшее положительное и наименьшее отрицательное значение.
           \en Looks for points at which the selected curvature takes on the surface the greatest positive and least negative value. \~
  \param[in] surface - \ru Исследуемая поверхность.
                       \en Test surface. \~
  \param[in] func - \ru Функция расчета кривизны в точке.
                    \en The function of calculating the curvature at a point. \~
  \param[out] maxNegCurv - \ru Наибольшее по модулю отрицательное значение кривизны (0, если нет такого).
                           \en The largest in modulus value negative curvature (0, if there is no such). \~
  \param[out] maxNegLoc - \ru Точка, в которой кривизна принимает наибольшее по модулю отрицательное значение.
                          \en The point at which the curvature takes the largest in modulus negative value. \~
  \param[out] maxPosCurv - \ru Наибольшее положительное значение кривизны (0, если нет такого).
                           \en The greatest positive value of curvature (0, if there is no such). \~
  \param[out] maxPosLoc - \ru Точка, в которой кривизна принимает наибольшее положительное значение.
                          \en The point at which the curvature takes the most positive value. \~
  \param[in] method - \ru Алгоритм поиска экстремумов.
                      \en Extremum search algorithm. \~
  \ingroup  Algorithms_3D
*/ 
MATH_FUNC( void ) SurfaceMinMaxCurvature( const MbSurface &               surface, 
                                                SurfaceFunction           func, 
                                                double &                  maxNegCurv, 
                                                MbCartPoint &             maxNegLoc, 
                                                double &                  maxPosCurv, 
                                                MbCartPoint &             maxPosLoc, 
                                                MbeExtremsSearchingMethod method = esm_LineSegregation );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки оболочки, в которых выбранная кривизна принимает наибольшие по модулю значения.
           \en Find the points of the shell at which the selected curvature takes the most modulo values. \~
  \details \ru Ищутся точки на оболочке, в которых выбранная кривизна принимают наибольшее положительное и наименьшее отрицательное значение.
           \en Finds points on the shell at which the selected curvature takes the largest positive and lowest negative values. \~
  \param[in] faces - \ru Грани оболочки.
                     \en Faces of the shell. \~
  \param[in] func - \ru Функция расчета кривизны в точке.
                    \en The function of calculating the curvature at a point. \~
  \param[out] maxNegCurv - \ru Наибольшее по модулю отрицательное значение кривизны (0, если нет такого).
                           \en The largest in modulus value negative curvature (0, if there is no such). \~
  \param[out] maxNegFace - \ru Грань, в которой кривизна принимает наибольшее по модулю отрицательное значение.
                          \en The face at which the curvature takes the largest in modulus negative value. \~
  \param[out] maxNegLoc - \ru Точка, в которой кривизна принимает наибольшее по модулю отрицательное значение.
                          \en The point at which the curvature takes the largest in modulus negative value. \~
  \param[out] maxPosCurv - \ru Наибольшее положительное значение кривизны (0, если нет такого).
                           \en The greatest positive value of curvature (0, if there is no such). \~
  \param[out] maxPosFace - \ru Грань, в которой кривизна принимает наибольшее положительное значение.
                          \en The face at which the curvature takes the most positive value. \~
  \param[out] maxPosLoc - \ru Точка, в которой кривизна принимает наибольшее положительное значение.
                          \en The point at which the curvature takes the most positive value. \~
  \param[in] borderControl - \ru Учитывать границы граней при поиске экстремумов.
                             \en Take into account the boundaries of the faces when searching for extrema. \~
  \param[in] method - \ru Алгоритм поиска экстремумов.
                      \en Extremum search algorithm. \~
  \ingroup  Algorithms_3D
*/ 
MATH_FUNC( void ) FacesMinMaxCurvature( const RPArray<MbFace> &          faces, 
                                              SurfaceFunction            func, 
                                              double &                   maxNegCurv, 
                                              MbFace *&                  maxNegFace, 
                                              MbCartPoint &              maxNegLoc,
                                              double &                   maxPosCurv, 
                                              MbFace *&                  maxPosFace, 
                                              MbCartPoint &              maxPosLoc, 
                                              bool                       borderControl = false, 
                                              MbeExtremsSearchingMethod  method = esm_LineSegregation );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки оболочки, в которых выбранная кривизна принимает наибольшие по модулю значения.
           \en Find the points of the shell at which the selected curvature takes the most modulo values. \~
  \details \ru Ищутся точки на оболочке, в которых выбранная кривизна принимают наибольшее положительное и наименьшее отрицательное значение.
           \en Finds points on the shell at which the selected curvature takes the largest positive and lowest negative values. \~
  \param[in] faces - \ru Грани оболочки.
                     \en Faces of the shell. \~
  \param[in] func - \ru Функция расчета кривизны в точке.
                    \en The function of calculating the curvature at a point. \~
  \param[out] maxNegCurv - \ru Наибольшее по модулю отрицательное значение кривизны (0, если нет такого).
                           \en The largest in modulus value negative curvature (0, if there is no such). \~
  \param[out] maxNegFace - \ru Грань, в которой кривизна принимает наибольшее по модулю отрицательное значение.
                          \en The face at which the curvature takes the largest in modulus negative value. \~
  \param[out] maxNegLoc - \ru Точка, в которой кривизна принимает наибольшее по модулю отрицательное значение.
                          \en The point at which the curvature takes the largest in modulus negative value. \~
  \param[out] maxPosCurv - \ru Наибольшее положительное значение кривизны (0, если нет такого).
                           \en The greatest positive value of curvature (0, if there is no such). \~
  \param[out] maxPosFace - \ru Грань, в которой кривизна принимает наибольшее положительное значение.
                          \en The face at which the curvature takes the most positive value. \~
  \param[out] maxPosLoc - \ru Точка, в которой кривизна принимает наибольшее положительное значение.
                          \en The point at which the curvature takes the most positive value. \~
  \param[in] borderControl - \ru Учитывать границы граней при поиске экстремумов.
                             \en Take into account the boundaries of the faces when searching for extrema. \~
  \param[in] method - \ru Алгоритм поиска экстремумов.
                      \en Extremum search algorithm. \~
  \ingroup  Algorithms_3D
*/ 
MATH_FUNC( void ) FacesMinMaxCurvature( const c3d::ConstFacesVector &    faces, 
                                              SurfaceFunction            func, 
                                              double &                   maxNegCurv, 
                                        const MbFace *&                  maxNegFace, 
                                              MbCartPoint &              maxNegLoc,
                                              double &                   maxPosCurv, 
                                        const MbFace *&                  maxPosFace, 
                                              MbCartPoint &              maxPosLoc, 
                                              bool                       borderControl = false, 
                                              MbeExtremsSearchingMethod  method = esm_LineSegregation );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки оболочки, в которых выбранная кривизна принимает наибольшие по модулю значения.
           \en Find the points of the shell at which the selected curvature takes the most modulo values. \~
  \details \ru Ищутся точки на оболочке, в которых выбранная кривизна принимают наибольшее положительное и наименьшее отрицательное значение.
           \en Finds points on the shell at which the selected curvature takes the largest positive and lowest negative values. \~
  \param[in] faces - \ru Грани оболочки.
                     \en Faces of the shell. \~
  \param[in] func - \ru Функция расчета кривизны в точке.
                    \en The function of calculating the curvature at a point. \~
  \param[out] maxNegCurv - \ru Наибольшее по модулю отрицательное значение кривизны (0, если нет такого).
                           \en The largest in modulus value negative curvature (0, if there is no such). \~
  \param[out] maxNegFace - \ru Грань, в которой кривизна принимает наибольшее по модулю отрицательное значение.
                          \en The face at which the curvature takes the largest in modulus negative value. \~
  \param[out] maxNegLoc - \ru Точка, в которой кривизна принимает наибольшее по модулю отрицательное значение.
                          \en The point at which the curvature takes the largest in modulus negative value. \~
  \param[out] maxPosCurv - \ru Наибольшее положительное значение кривизны (0, если нет такого).
                           \en The greatest positive value of curvature (0, if there is no such). \~
  \param[out] maxPosFace - \ru Грань, в которой кривизна принимает наибольшее положительное значение.
                          \en The face at which the curvature takes the most positive value. \~
  \param[out] maxPosLoc - \ru Точка, в которой кривизна принимает наибольшее положительное значение.
                          \en The point at which the curvature takes the most positive value. \~
  \param[in] borderControl - \ru Учитывать границы граней при поиске экстремумов.
                             \en Take into account the boundaries of the faces when searching for extrema. \~
  \param[in] method - \ru Алгоритм поиска экстремумов.
                      \en Extremum search algorithm. \~
  \ingroup  Algorithms_3D
*/ 
MATH_FUNC( void ) FacesMinMaxCurvature( const c3d::ConstFacesSPtrVector & faces, 
                                              SurfaceFunction             func, 
                                              double &                    maxNegCurv, 
                                        const MbFace *&                   maxNegFace, 
                                              MbCartPoint &               maxNegLoc,
                                              double &                    maxPosCurv, 
                                        const MbFace *&                   maxPosFace, 
                                              MbCartPoint &               maxPosLoc, 
                                              bool                        borderControl = false, 
                                              MbeExtremsSearchingMethod   method = esm_LineSegregation );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки на поверхности, в которых главные нормальные кривизны принимают наибольшие по модулю значения.
           \en Find the points on the surface at which the major normal curvatures take the largest values in the module. \~
  \details \ru Ищутся точки на поверхности, в которых главные нормальные кривизны принимают наибольшее положительное и наименьшее отрицательное значение. 
           \en Looks for points on the surface at which the major normal curvatures take the largest positive and smallest negative values. \~
  \param[in] surface - \ru Исследуемая поверхность.
                       \en Test surface. \~
  \param[out] maxNegCurv - \ru Наибольшее по модулю отрицательное значение кривизны (0, если нет такого).
                           \en The largest in modulus value negative curvature (0, if there is no such). \~
  \param[out] maxNegLoc - \ru Точка, в которой кривизна принимает наибольшее по модулю отрицательное значение.
                          \en The point at which the curvature takes the largest in modulus negative value. \~
  \param[out] maxPosCurv - \ru Наибольшее положительное значение кривизны (0, если нет такого).
                           \en The greatest positive value of curvature (0, if there is no such). \~
  \param[out] maxPosLoc - \ru Точка, в которой кривизна принимает наибольшее положительное значение.
                          \en The point at which the curvature takes the most positive value. \~
  \param[in] method - \ru Алгоритм поиска экстремумов.
                      \en Extremum search algorithm. \~
  \ingroup  Algorithms_3D
*/ 
MATH_FUNC( void ) SurfaceMinMaxCurvature( const MbSurface &               surface, 
                                                double &                  maxNegCurv, 
                                                MbCartPoint &             maxNegLoc, 
                                                double &                  maxPosCurv, 
                                                MbCartPoint &             maxPosLoc, 
                                                MbeExtremsSearchingMethod method = esm_LineSegregation );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки на оболочке, в которых главные нормальные кривизны принимают наибольшие по модулю значения.
           \en Find the points on the shell at which the major normal curvatures take the largest values in the module. \~
  \details \ru Ищутся точки на оболочке, в которых главные нормальные кривизны принимают наибольшее положительное и наименьшее отрицательное значение. 
           \en Looks for points on the shell at which the major normal curvatures take the largest positive and smallest negative values. \~
  \param[in] faces - \ru Грани оболочки.
                     \en Faces of the shell. \~
  \param[out] maxNegCurv - \ru Наибольшее по модулю отрицательное значение кривизны (0, если нет такого).
                           \en The largest in modulus value negative curvature (0, if there is no such). \~
  \param[out] maxNegFace - \ru Грань, в которой кривизна принимает наибольшее по модулю отрицательное значение.
                          \en The face at which the curvature takes the largest in modulus negative value. \~
  \param[out] maxNegLoc - \ru Точка, в которой кривизна принимает наибольшее по модулю отрицательное значение.
                          \en The point at which the curvature takes the largest in modulus negative value. \~
  \param[out] maxPosCurv - \ru Наибольшее положительное значение кривизны (0, если нет такого).
                           \en The greatest positive value of curvature (0, if there is no such). \~
  \param[out] maxPosFace - \ru Грань, в которой кривизна принимает наибольшее положительное значение.
                          \en The face at which the curvature takes the most positive value. \~
  \param[out] maxPosLoc - \ru Точка, в которой кривизна принимает наибольшее положительное значение.
                          \en The point at which the curvature takes the most positive value. \~
  \param[in] borderControl - \ru Учитывать границы граней при поиске экстремумов.
                             \en Take into account the boundaries of the faces when searching for extrema. \~
  \param[in] method - \ru Алгоритм поиска экстремумов.
                      \en Extremum search algorithm. \~
  \ingroup  Algorithms_3D
*/ 
MATH_FUNC( void ) FacesMinMaxCurvature( const RPArray<MbFace> &         faces, 
                                              double &                  maxNegCurv, 
                                              MbFace *&                 maxNegFace, 
                                              MbCartPoint &             maxNegLoc,
                                              double &                  maxPosCurv, 
                                              MbFace *&                 maxPosFace, 
                                              MbCartPoint &             maxPosLoc, 
                                              bool                      borderControl = false, 
                                              MbeExtremsSearchingMethod method = esm_LineSegregation );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки на оболочке, в которых главные нормальные кривизны принимают наибольшие по модулю значения.
           \en Find the points on the shell at which the major normal curvatures take the largest values in the module. \~
  \details \ru Ищутся точки на оболочке, в которых главные нормальные кривизны принимают наибольшее положительное и наименьшее отрицательное значение. 
           \en Looks for points on the shell at which the major normal curvatures take the largest positive and smallest negative values. \~
  \param[in] faces - \ru Грани оболочки.
                     \en Faces of the shell. \~
  \param[out] maxNegCurv - \ru Наибольшее по модулю отрицательное значение кривизны (0, если нет такого).
                           \en The largest in modulus value negative curvature (0, if there is no such). \~
  \param[out] maxNegFace - \ru Грань, в которой кривизна принимает наибольшее по модулю отрицательное значение.
                          \en The face at which the curvature takes the largest in modulus negative value. \~
  \param[out] maxNegLoc - \ru Точка, в которой кривизна принимает наибольшее по модулю отрицательное значение.
                          \en The point at which the curvature takes the largest in modulus negative value. \~
  \param[out] maxPosCurv - \ru Наибольшее положительное значение кривизны (0, если нет такого).
                           \en The greatest positive value of curvature (0, if there is no such). \~
  \param[out] maxPosFace - \ru Грань, в которой кривизна принимает наибольшее положительное значение.
                          \en The face at which the curvature takes the most positive value. \~
  \param[out] maxPosLoc - \ru Точка, в которой кривизна принимает наибольшее положительное значение.
                          \en The point at which the curvature takes the most positive value. \~
  \param[in] borderControl - \ru Учитывать границы граней при поиске экстремумов.
                             \en Take into account the boundaries of the faces when searching for extrema. \~
  \param[in] method - \ru Алгоритм поиска экстремумов.
                      \en Extremum search algorithm. \~
  \ingroup  Algorithms_3D
*/ 
MATH_FUNC( void ) FacesMinMaxCurvature( const c3d::ConstFacesVector &   faces, 
                                              double &                  maxNegCurv, 
                                        const MbFace *&                 maxNegFace, 
                                              MbCartPoint &             maxNegLoc,
                                              double &                  maxPosCurv, 
                                        const MbFace *&                 maxPosFace, 
                                              MbCartPoint &             maxPosLoc, 
                                              bool                      borderControl = false, 
                                              MbeExtremsSearchingMethod method = esm_LineSegregation );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки на оболочке, в которых главные нормальные кривизны принимают наибольшие по модулю значения.
           \en Find the points on the shell at which the major normal curvatures take the largest values in the module. \~
  \details \ru Ищутся точки на оболочке, в которых главные нормальные кривизны принимают наибольшее положительное и наименьшее отрицательное значение. 
           \en Looks for points on the shell at which the major normal curvatures take the largest positive and smallest negative values. \~
  \param[in] faces - \ru Грани оболочки.
                     \en Faces of the shell. \~
  \param[out] maxNegCurv - \ru Наибольшее по модулю отрицательное значение кривизны (0, если нет такого).
                           \en The largest in modulus value negative curvature (0, if there is no such). \~
  \param[out] maxNegFace - \ru Грань, в которой кривизна принимает наибольшее по модулю отрицательное значение.
                          \en The face at which the curvature takes the largest in modulus negative value. \~
  \param[out] maxNegLoc - \ru Точка, в которой кривизна принимает наибольшее по модулю отрицательное значение.
                          \en The point at which the curvature takes the largest in modulus negative value. \~
  \param[out] maxPosCurv - \ru Наибольшее положительное значение кривизны (0, если нет такого).
                           \en The greatest positive value of curvature (0, if there is no such). \~
  \param[out] maxPosFace - \ru Грань, в которой кривизна принимает наибольшее положительное значение.
                          \en The face at which the curvature takes the most positive value. \~
  \param[out] maxPosLoc - \ru Точка, в которой кривизна принимает наибольшее положительное значение.
                          \en The point at which the curvature takes the most positive value. \~
  \param[in] borderControl - \ru Учитывать границы граней при поиске экстремумов.
                             \en Take into account the boundaries of the faces when searching for extrema. \~
  \param[in] method - \ru Алгоритм поиска экстремумов.
                      \en Extremum search algorithm. \~
  \ingroup  Algorithms_3D
*/ 
MATH_FUNC( void ) FacesMinMaxCurvature( const c3d::ConstFacesSPtrVector & faces, 
                                              double &                    maxNegCurv, 
                                        const MbFace *&                   maxNegFace, 
                                              MbCartPoint &               maxNegLoc,
                                              double &                    maxPosCurv, 
                                        const MbFace *&                   maxPosFace, 
                                              MbCartPoint &               maxPosLoc, 
                                              bool                        borderControl = false, 
                                              MbeExtremsSearchingMethod   method = esm_LineSegregation );


//------------------------------------------------------------------------------
/** \brief \ru Ориентированная кривизна для плоской кривой.
           \en Oriented curvature for a plane curve. \~
  \details \ru Для плоской кривой функция возвращает кривизну в точке, ориентированную относительно нормали плоскости, 
               в которой она лежит. Для неплоской кривой функция просто возвращает кривизну в точке.
           \en For a flat curve, the function returns the curvature at a point oriented relative to the normal to the plane,
                in which she lies. For a non-flat curve, the function simply returns the curvature at the point. \~
  \param[in] curve - \ru Исследуемая кривая.
                     \en Test curve. \~
  \param[in] param - \ru Параметр на кривой.
                     \en Parameter on the curve. \~
  \param[in] planeNorm - \ru Нормаль плоскости, в которой лежит кривая. Если нормаль не передается в функцию, алгоритм самостоятельно
                             выполняет проверку, лежит ли кривая в плоскости, и вычисляет нормаль, если проверка выполняется.
                         \en The normal of the plane in which the curve lies. If the normal is not passed to the function, the algorithm itself
                              checks if the curve is in the plane and calculates normal if the test is being performed. \~
  \return  \ru Возвращается значение ориентированной кривизны в точке.
           \en The value of the oriented curvature at the point is returned. \~

  \ingroup  Algorithms_3D
*/ 
MATH_FUNC( double ) CurveOrientedCurvature( const MbCurve3D &  curve, 
                                                  double &     param, 
                                            const MbVector3D * planeNorm = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Направление максимальной нормальной кривизны поверхности.
           \en The direction of the maximum normal surface curvature. \~
  \details \ru Вычисляется направление на поверхности, в котором нормальная кривизна поверхности принимает максимальное значение по модулю.
           \en The direction on the surface is calculated in which the normal curvature of the surface takes a maximum absolute value. \~
  \param[in] surf - \ru Поверхность.
                    \en Surface. \~
  \param[in] pnt - \ru Точка расчета.
                    \en Point of calculation. \~
  \param[out] dir - \ru Рассчитываемое направление.
                    \en The calculated direction. \~
  \ingroup Algorithms_3D
*/ 
MATH_FUNC( void ) SurfaceMaxCurvatureDirection( const MbSurface &   surf, 
                                                const MbCartPoint & pnt, 
                                                      MbVector &    dir );


//------------------------------------------------------------------------------
// Вычисление приращения параметров по заданному направлению
// ---
inline
bool ParamsDirectData( const MbVector3D & direct,
                       const MbVector3D & derU, double & x,
                       const MbVector3D & derV, double & y )
{
  double eps = PARAM_REGION;
  double g11 = derU * derU;
  double g22 = derV * derV;

  if ( g11 > eps && g22 > eps && g11 > g22*eps && g22 > g11*eps ) { // проецируем производную на поверхность
    double g12 = derU * derV;
    double g = g11*g22 - g12*g12;
    double cosAngle = PARAM_NEAR; // DELTA_MOD;
    if ( g > eps && g > cosAngle*g11*g22 ) { // не сильно косоугольная система 
      double du = direct * derU;
      double dv = direct * derV;
      if ( ::fabs( g12 ) > PARAM_EPSILON ) { // не ортогональная система
        x = (g22*du - g12*dv) / g;
        y = (g11*dv - g12*du) / g;
      }
      else {
        x = du / g11;
        y = dv / g22;
      }
      return true;
    }
  }
  return false;
}


//------------------------------------------------------------------------------
/** \brief \ru Входные параметры функции поиска минимальных и максимальных отклонений параметров поверхностей вдоль их кривой пересечения.
           \en Input parameters of the search function for minimum and maximum deviations of surfaces parameters along their intersection curve. \~
  \details \ru Входные параметры функции поиска минимальных и максимальных отклонений параметров поверхностей вдоль их кривой пересечения. \n
           \en Input parameters of the search function for minimum and maximum deviations of surfaces parameters along their intersection curve. \n \~
  \ingroup Algorithms_3D
*/ // ---
class MATH_CLASS MbSurfacesJointAnalysisParams {
public:
  enum OperationMode
  {
    /// \ru Поиск эстремальных отклонений нормалей поверхностей (угол между нормалями поверхностей). \en Search for extreme deviations of surfaces normals (angles between surfaces normals).
    om_SurfacesNormalsDeviation        = 0,
    /// \ru Поиск эстремальных отклонений кривизн поверхностей. \en Search for extreme deviations of surfaces curvatures.
    om_SurfacesCrossCurvatureDeviation = 1,
  };
protected:
  c3d::ConstIntersectionCurveSPtr intCurve;   ///< \ru Кривая пересечения. \en Surfaces intersection curve.
  bool                            sameSense1; ///< \ru Совпадение направления нормали первой поверхности и грани на ее основе. \en Coincidence of direction of the normal of the first surface and the face on its basis.
  bool                            sameSense2; ///< \ru Совпадение направления нормали второй поверхности и грани на ее основе. \en Coincidence of direction of the normal of the second surface and the face on its basis.
  ThreeStates                     dirMatch;   ///< \ru Прямое, неопределенное или обратное соответствие поверхность-грань. \en Direct, undefined, or inverse surface-to-face match.
  OperationMode                   calcMode;   ///< \ru Режим расчета. \en Operation mode.
private:
  const MbSNameMaker &            snMaker;    ///< \ru Именователь с версией операции. \en Names maker with operation version.
public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по кривой пересечения.
             \en Constructor by . \~
    \param[in] intCrv - \ru Кривая пересечения поверхностей.
                        \en Surfaces intersection curve. \~
    \param[in] nm     - \ru Именователь с версией операции.
                        \en Names maker with operation version. \~
  */
  MbSurfacesJointAnalysisParams( const MbSurfaceIntersectionCurve & intCrv, const MbSNameMaker & nm )
    : intCurve  ( &intCrv                     )
    , sameSense1( true                        )
    , sameSense2( true                        )
    , dirMatch  ( ts_neutral                  )
    , calcMode  ( om_SurfacesNormalsDeviation )
    , snMaker   ( nm                          )
  {}
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по ребру.
             \en Constructor by edge. \~
    \param[in] edge - \ru Ребро.
                      \en Edge. \~
    \param[in] nm   - \ru Именователь с версией операции.
                      \en Names maker with operation version. \~
  */
  MbSurfacesJointAnalysisParams( const MbCurveEdge & edge, const MbSNameMaker & nm )
    : intCurve  ( &edge.GetIntersectionCurve() )
    , sameSense1( true                         )
    , sameSense2( true                         )
    , dirMatch  ( ts_neutral                   )
    , calcMode  ( om_SurfacesNormalsDeviation  )
    , snMaker   ( nm                           )
  {
    Init( edge );
  }
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по двум ребрам.
             \en Constructor by two edges. \~
    \param[in] edge1 - \ru Ребро 1.
                      \en Edge 1. \~
    \param[in] edge2 - \ru Ребро 2.
                      \en Edge 2. \~
    \param[in] accuracy - \ru Заявленная точность совпадения ребер.
                          \en Claimed edges matching accuracy. \~
    \param[in] nm   - \ru Именователь с версией операции.
                      \en Names maker with operation version. \~
  */
  MbSurfacesJointAnalysisParams( const MbCurveEdge &  edge1, const MbFace & face1,
                                 const MbCurveEdge &  edge2, const MbFace & face2,
                                 double accuracy, const MbSNameMaker & nm )
    : intCurve  ( nullptr                     )
    , sameSense1( true                        )
    , sameSense2( true                        )
    , dirMatch  ( ts_neutral                  )
    , calcMode  ( om_SurfacesNormalsDeviation )
    , snMaker   ( nm                          )
  {
    Init( edge1, face1, edge2, face2, accuracy );
  }
public:
  /// \ru Есть ли кривая пересечения? \en Does an intersection curve exist?
  bool                            IsCurve() const { return (intCurve != nullptr);  }
  /// \ru Получить кривую пересечения? \en Get intersection curve.
  c3d::ConstIntersectionCurveSPtr GetCurve() const { return intCurve; }
  /// \ru Признак совпадения нормали первой поверхности и грани. \en The flag of the coincidence of the normal of the first surface and the corresponding face .
  bool                            IsSameSense1() const { return sameSense1; }
  /// \ru Признак совпадения нормали второй поверхности и грани. \en The flag of the coincidence of the normal of the second surface and the corresponding face .
  bool                            IsSameSense2() const { return sameSense2; }
  /// \ru Получить режим работы. \en Get operation mode. 
  OperationMode                   GetOperationMode() const { return calcMode; }
  /// \ru Получить ссылку на именователь. \en Get names maker reference. 
  const MbSNameMaker &            GetNameMaker() const { return snMaker; }
public:
  /// \ru Установить режим работы. \en Set operation mode. 
  void                            SetOperationMode( OperationMode mode ) { calcMode = mode; }
private:
  /// \ru Инциализация по кривой пересечения и граням. \en Initialize by intersection curve and faces.
  bool                            Init( const MbSurfaceIntersectionCurve & initCurve, const MbFace * face1, const MbFace * face2 );
  /// \ru Инциализация по ребру. \en Initialize by edge.
  bool                            Init( const MbCurveEdge & );
  /// \ru Инциализация по двум ребрам. \en Initialize by edges pair.
  bool                            Init( const MbCurveEdge & edge1, const MbFace & face1,
                                        const MbCurveEdge & edge2, const MbFace & face2,
                                        double edgesMatchingAccuracy );

OBVIOUS_PRIVATE_COPY( MbSurfacesJointAnalysisParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Выходные параметры функции поиска минимальных и максимальных отклонений параметров поверхностей вдоль их кривой пересечения.
           \en Output parameters of the function of finding the minimum and maximum deviations of surfaces parameters along their intersection curve. \~
  \details \ru Выходные параметры функции поиска минимальных и максимальных отклонений параметров поверхностей вдоль их кривой пересечения. \n
           \en Output parameters of the function of finding the minimum and maximum angles between surfaces normals of the intersection curve. \n \~
  \ingroup Algorithms_3D
*/ // ---
class MATH_CLASS MbSurfacesJointAnalysisResults {
  typedef MbSurfacesJointAnalysisParams::OperationMode OperationMode;
public:
  /// \ru Локальные выходные параметры. \en Output local parameters.
  struct Data {
    /// \ru Параметр на кривой пересечения. \en Intersection curve parameter.
    double      t;
    /// \ru Значение целевой функции (например : f = 1.0 - (cos(a)*cos(a)). \en Objective function value (for example : f = 1.0 - (cos(a)*cos(a)).
    double      f;
    /// \ru Параметр оценки стыковки поверхностей (например - угол между нормалями поверхностей в кривой пересечения). \en Surfaces joint evaluation parameter (for example - angle between surfaces normals in the intersection curve).
    double      v;    
    /// \ru Локальный минимум, максимум или неопределенное состояние. \en Local minimum, maximum or undefined state.
    ThreeStates isMin;
  public:
    Data() : t( UNDEFINED_DBL ), f( UNDEFINED_DBL ), v( UNDEFINED_DBL ), isMin( ts_neutral ) {}
    Data( double f0 ) : t( UNDEFINED_DBL ), f( f0 ), v( UNDEFINED_DBL ), isMin( ts_neutral ) {}
    Data( double t0, double f0 ) : t( t0 ), f( f0 ), v( UNDEFINED_DBL ), isMin( ts_neutral ) {}
    Data( double t0, double f0, double a0 ) : t( t0 ), f( f0 ), v( a0 ), isMin( ts_neutral ) {}
    Data( double t0, double f0, double a0, ThreeStates s ) : t( t0 ), f( f0 ), v( a0 ), isMin( s ) {}
    Data( const Data & d ) : t( d.t ), f( d.f ), v( d.v ), isMin( d.isMin ) {}
  public:
    const Data & operator = ( const Data & d ) { t = d.t; f = d.f; v = d.v; isMin = d.isMin; return *this; }
  public:
    void Reset() { t = f = v = UNDEFINED_DBL; isMin = ts_neutral; }
  public:
    bool IsSameData( const Data & item, double eps ) const
    {
      bool isSameData = false;
      if ( item.isMin == isMin ) {
        if ( ::fabs( item.f - f ) < eps )
          isSameData = true;
      }
      return isSameData;
    }
  };
public:
  std::vector<Data> allParamValues; ///< \ru Все найденные экстремальные параметры. \en All found extrema parameters.
  std::vector<Data> minParamValues; ///< \ru Все найденные локальные минимумы параметра. \en All found local minimum parameters.
  std::vector<Data> maxParamValues; ///< \ru Все найденные локальные максимумы параметра. \en All found local maximum parameters.
  Data              minParamValue;  ///< \ru Глобальный минимальный параметр. \en Global minimum parameter.
  Data              maxParamValue;  ///< \ru Глобальный максимальный параметр. \en Global maximum parameter.  
  MbResultType      resType;        ///< \ru Код результата операции. \en Operation result code.
protected:
  OperationMode     calcMode;       ///< \ru Режим расчета. \en Operation mode.
public:
  /// \ru Конструктор. \en Constructor.
  MbSurfacesJointAnalysisResults();
public:
  /// \ru Получить режим работы. \en Get operation mode. 
  OperationMode GetOperationMode() const { return calcMode; }
  /// \ru Определен ли глобальный минимум. \en Whether a global minimum value is defined.
  bool          IsDefinedMinValue() const;
  /// \ru Определен ли глобальный максимум. \en Whether a global maximum value is defined.
  bool          IsDefinedMaxValue() const;
  /// \ru Очистка данных. \en Data cleaning.
  void          Clear() { allParamValues.clear(); minParamValues.clear(); maxParamValues.clear(); minParamValue.Reset(); maxParamValue.Reset(); }
public:
  /// \ru Получить режим работы. \en Get operation mode. 
  void          SetOperationMode( const MbSurfacesJointAnalysisParams & pars ) { calcMode = pars.GetOperationMode(); }

OBVIOUS_PRIVATE_COPY( MbSurfacesJointAnalysisResults )
};


//------------------------------------------------------------------------------
/** \brief \ru Функция поиска минимальных и максимальных углов между нормалями поверхностей кривой пересечения.
           \en The function of finding the minimum and maximum angles between surfaces normals of the intersection curve. \~
  \details \ru Функция поиска минимальных и максимальных углов между нормалями поверхностей кривой пересечения. \n
           \en The function of finding the minimum and maximum angles between surfaces normals of the intersection curve. \n \~
  \param[in] params   - \ru Входные параметры.
                        \en Input parameters. \~
  \param[out] results - \ru Выходные параметры.
                        \en Output parameters. \~
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC( bool ) SurfacesNormalsMinMaxAngles( const MbSurfacesJointAnalysisParams &  params,
                                                     MbSurfacesJointAnalysisResults & results );

//------------------------------------------------------------------------------
/** \brief \ru Функция поиска минимальных и максимальных значений параметров стыковки поверхностей по кривой пересечения.
           \en The function of finding the minimum and maximum values of surface joint parameter. \~
  \details \ru Функция поиска минимальных и максимальных значений параметров стыковки поверхностей по кривой пересечения. \n
           \en The function of finding the minimum and maximum values of surface joint parameter. \n \~
  \param[in] params   - \ru Входные параметры.
                        \en Input parameters. \~
  \param[out] results - \ru Выходные параметры.
                        \en Output parameters. \~
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC( bool ) SurfacesJointMinMaxValues( const MbSurfacesJointAnalysisParams &  params,
                                                   MbSurfacesJointAnalysisResults & results );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки на кривой, в которых кривизна принимает наибольшее и наименьшее значения.
           \en Find the points on the curve at which the curvature takes the largest and smallest values. \~
    \details \ru Для плоской кривой наибольшее и наименьшее значение может уходить в отрицательную область.
                 Для неплоской кривой наибольшее и наименьшее значение всегда неотрицательны. \n
                 Метод может быть использован для поиска экстремумов, точек разрыва и точек перегиба функции кривизны кривой, 
                 а также для расчета потенциальной энергии кривой. Энергия вычисляется как интеграл от квадрата кривизны по длине кривой. \n
             \en For a flat curve, the largest and smallest value may go into the negative region.
                 For a non-planar curve, the largest and smallest values are always non-negative. \n
                 The method can be used to find local minimum and maximum, break points and bending points of the curvature function and
                 to calculate the potential energy of the curve. The energy is calculated as the integral of the square of curvature along the length of the curve. \n \~
  \param[in] curve    - \ru Исследуемая кривая.
                        \en Test curve. \~
  \param[in] params   - \ru Параметры расчета особых точек кривизны кривой и вычисления потенциальной энергии кривой.
                        \en Parameters for calculation of special points of curvature and curve potential energy.\~
  \param[out] results - \ru Результаты поиска особых точек кривизны и вычисления потенциальной энергии кривой.
                        \en Results of curvature special points search and calculation of curve potential energy.\~
  \ingroup  Algorithms_3D
  \warning \ru В разработке.
           \en Under development. \~
*/
MATH_FUNC( void ) CurveMinMaxCurvature( const MbCurve3D & curve,
                                        const MbCurveMinMaxCurvatureParams & params,
                                        MbCurveMinMaxCurvatureResults & results );

#endif // __ACTION_CURVATURE_ANALYSIS_H
