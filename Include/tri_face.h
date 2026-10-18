////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Аппроксимация грани массивом треугольных и четырёхугольных пластин.
         \en Approximation of face by array of triangular and quadrangular plates. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TRI_FACE_H
#define __TRI_FACE_H


#include <mb_data.h>


class    MbFace;
class    MbGrid;
class    MbCube;


//------------------------------------------------------------------------------
/** \brief \ru Аппроксимировать грань.
           \en Approximation of face. \~
  \details \ru Аппроксимировать грань массивом треугольных и четырёхугольных пластин.\n
           \en Approximation of face by array of triangular and quadrangular plates.\n \~
  \param[in]  face       - \ru Грань.
                           \en A face. \~
  \param[in] stepData    - \ru Данные для вычисления шага при триангуляции.
                           \en Data for step calculation during triangulation. \~
  \param[out] grid       - \ru Результат - триангуляции:
                           для визуализации заполняются массивы params, points и normals;\n 
                           для вычисления масс-инерционных характеристик - params;\n 
                           для проверки столкновений тел - params и points;\n
                           для разбивки на конечные элементы - points и normals.\n
                           \en Triangulations as a result:
                           for visualization arrays 'params', 'points' and 'normals' are filled;\n 
                           for calculation of the mass-inertial properties an array 'params' is filled;\n 
                           for solids collision detection arrays 'params' and 'points' are filled;\n
                           for splitting into finite elements arrays 'points' and 'normals' are filled.\n \~ 
  \param[in] dualSeams   - \ru Флаг сохранения полигонов шовных ребер и их совпадающих точек.\n 
                           \en Whether to keep seam edges polygons and their coincident points. \~
  \param[in] quad        - \ru Строить четырёхугольники (true) при триангуляции поверхностей (по возможности). 
                           \en Whether to build quadrangles (true) in triangulations of surfaces (if possible). \~
  \param[in] fair        - \ru Удалить вырожденные треугольники (true). 
                           \en Degenerate triangles removing (if surface has pole). \~
  \param[in] mere        - \ru Использовать специальный алгоритм для плоской поверхности (true). 
                           \en Use a special algorithm for a flat surface. \~
  \ingroup Triangulation
*/
// ---
MATH_FUNC (void) CalculateGrid( const MbFace &     face, 
                                const MbStepData & stepData, 
                                      MbGrid &     grid, 
                                      bool         dualSeams = true,
                                      bool         quad = false,
                                      bool         fair = false,
                                      bool         mere = true );


//------------------------------------------------------------------------------
/** \brief \ru Настройка данных вычисления шага триангуляции stepData.
           \en Configuring step data for triangulation. \~
  \details \ru Проверка и коррекция данных вычисления шага stepData так, 
               чтобы количество точек триангуляции не превосходило заданную величину на порядок. \n
           \en Checking and Correction step data for triangulation so that the number of points 
               does not exceed a predetermined count on the order of magnitude of.\n \~
  \param[in]  cube    - \ru Габаритный куб.
                        \en A dimensional cube. \~
  \param[in]  count   - \ru Предположительное предельное количество точек триангуляции для одной грани (10 000 000).\n
                        \en Proposed limit on the number of triangulation points for one face. \~
  \param[in] stepData - \ru Данные для вычисления шага при триангуляции.
                        \en Data for step calculation during triangulation. \~
  \return \ru true - были внесены изменения в stepData, false - изменений не было.
          \en True - stepData was change, false - there was no change. \~
  \ingroup Triangulation
*/
// ---
MATH_FUNC (bool) StepDataTune( const MbCube & cube, size_t count, MbStepData & stepData );


#endif // __TRI_FACE_H