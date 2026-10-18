////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение триангуляции тела.
         \en Construction of solid triangulation. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MIP_SOLID_AREA_VOLUME_H
#define __MIP_SOLID_AREA_VOLUME_H


#include <mesh.h>


class   MbFace;
class   MbSurface;
class   MbFaceShell;
class   MbSolid;


//------------------------------------------------------------------------------
/** \brief \ru Определить площадь.
           \en Determine area. \~
  \details \ru Определить площадь поверхности.
           \en Determine surface area. \~
  \param[in] surface - \ru Поверхность.
                       \en The surface. \~
  \param[in] angle   - \ru Ограничение углового отклонения при аппроксимации оболочки треугольными пластинами.
                       \en Bounding of angular deviation during shell approximation by triangular plates. \~
  \return \ru Площадь поверхности.
          \en Surface area. \~
  \ingroup Inertia_Computation
*/
// ---
MATH_FUNC (double) CalculateArea( const MbSurface & surface, 
                                  double angle = Math::deviateSag ); 


//------------------------------------------------------------------------------
/** \brief \ru Определить объем.
           \en Determine volume. \~
  \details \ru Определить объем тела.
           \en Determine volume of solid. \~
  \param[in] solid - \ru Тело.
                     \en A solid. \~
  \param[in] angle - \ru Ограничение углового отклонения при аппроксимации оболочки треугольными пластинами.
                     \en Bounding of angular deviation during shell approximation by triangular plates. \~
  \return \ru Значение объема тела.
          \en Value of solid volume. \~       
  \ingroup Inertia_Computation
*/
// ---
MATH_FUNC (double) CalculateVolume( const MbSolid &solid, 
                                    double angle );


//------------------------------------------------------------------------------
/** \brief \ru Определить объем.
           \en Determine volume. \~
  \details \ru Определить объем полигонального объекта.
           \en Determine volume of polygonal object. \~
  \param[in] mesh - \ru Пполигональный объект.
                    \en A polygonal object. \~
  \return \ru Значение объема полигонального объекта.
          \en Value of polygonal object volume. \~       
  \ingroup Inertia_Computation
*/
// ---
MATH_FUNC (double) CalculateVolume( const MbMesh & mesh );


//------------------------------------------------------------------------------
/** \brief \ru Определить площадь и центр масс.
           \en Determine area and center of mass. \~
  \details \ru Определить площадь поверхности оболочки и центр масс этой поверхности. \n
    В общем случае центр масс объёма объекта не совпадает с центром масс его поверхности. \n
           \en Determine area of shell surface and its center of mass. \n
    Center of mass of volumetric object doesn't coincide with center of mass of its surface. \n \~
  \param[in]  shell  - \ru Оболочка.
                       \en A shell. \~
  \param[in]  angle  - \ru Ограничение углового отклонения при аппроксимации оболочки треугольными пластинами.
                       \en Bounding of angular deviation during shell approximation by triangular plates. \~
  \param[out] centre - \ru Центр масс площади оболочки.
                       \en Center of mass of shell area. \~       
  \result \ru Площадь поверхности оболочки.
          \en Area of shell surface. \~
  \ingroup Inertia_Computation
*/
// ---
MATH_FUNC (double) CalculateAreaCentre( const MbFaceShell & shell, 
                                        double angle, 
                                        MbCartPoint3D & centre ); 


//------------------------------------------------------------------------------
/** \brief \ru Определить площадь и центр масс.
           \en Determine area and center of mass. \~
  \details \ru Определить площадь поверхности грани и центр масс этой поверхности. \n
    В общем случае центр масс объёма объекта не совпадает с центром масс его пловерхности. \n
           \en Determine area of face surface and its center of mass. \n
    Center of mass of volumetric object doesn't coincide with center of mass of its surface. \n \~
  \param[in]  face    - \ru Грань.
                        \en A face. \~
  \param[in]  angle   - \ru Ограничение углового отклонения при аппроксимации грани треугольными пластинами.
                        \en Bounding of angular deviation during face approximation by triangular plates. \~
  \param[in]  byOuter - \ru При true расчет выполняется только по внешней границе без учёта внутренних вырезов грани.
                        \en If true calculation is performed only on external boundary without taking internal cuts of face into account. \~
  \param[in]  version - \ru Версия, по умолчанию - последняя.
                        \en Version, last by default. \~
  \param[out] centre  - \ru Центр масс площади грани.
                        \en Center of mass of face area. \~
  \result \ru Площадь поверхности грани.
          \en Face surface area. \~
  \ingroup Inertia_Computation
*/
// ---
MATH_FUNC (double) CalculateAreaCentre( const MbFace & face, 
                                        double angle, 
                                        bool byOuter,
                                        VERSION version, 
                                        MbCartPoint3D & centre ); 


//------------------------------------------------------------------------------
/** \brief \ru Определение площади поверхности граней.
           \en Determination of faces area. \~
  \details \ru Определение площади поверхности граней. 
    Определение выполняется методом вычисления масс-центровочных характеристик набора граней.
           \en Determination of area of set of faces. \~
  \note    \ru В многопоточном режиме выполняется параллельно.
           \en In multithreaded mode m_Items runs in parallel. \~
  \param[in]  faces        - \ru Набор граней.
                             \en A set of faces. \~
  \param[in]  deviateAngle - \ru Параметр управления точностью расчёта - угловое отклонение нормали поверхности или касательных кривой на участке численного интегрирования.
                             \en Tolerance - the angular deviation of surface or curve in the neighboring points on the region of numerical integration. \~
  \return \ru Площадь граней.
          \en Faces area. \~
  \ingroup Inertia_Computation
*/
// ---
MATH_FUNC (double) CalculateArea( const RPArray<MbFace> & faces, 
                                  double deviateAngle ); // (0.35 - 0.01)


//------------------------------------------------------------------------------
/** \brief \ru Определение площади.
           \en Determination of area. \~
  \details \ru Определение площади поверхности грани.
           \en Determination of face surface area. \~
  \param[in] face          - \ru Грань.
                             \en A face. \~ 
  \param[in]  deviateAngle - \ru Параметр управления точностью расчёта - угловое отклонение нормали поверхности или касательных кривой на участке численного интегрирования.
                             \en Tolerance - the angular deviation of surface or curve in the neighboring points on the region of numerical integration. \~
  \return \ru Площадь грани.
          \en Face area. \~
  \ingroup Inertia_Computation
*/
// ---
MATH_FUNC (double) CalculateArea( const MbFace & face, 
                                  double deviateAngle ); // (0.35 - 0.01)


#endif // __MIP_SOLID_AREA_VOLUME_H
