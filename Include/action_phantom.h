////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение фантомов операций.
         \en Creation of phantom operations. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ACTION_PHANTOM_H
#define __ACTION_PHANTOM_H


#include <templ_s_array.h>
#include <mb_cart_point3d.h>
#include <space_item.h>
#include <op_shell_parameter.h>
#include <op_swept_parameter.h>
#include <position_data.h>
#include <topology_faceset.h>
#include <mb_operation_result.h>


class   MbCurve;
class   MbCurve3D;
class   MbSurface;
class   MbCurveEdge;
class   MbFace;
class   MbSolid;
class   MbSNameMaker;


//------------------------------------------------------------------------------
/** \brief \ru Построить фантомные поверхности скругления/фаски. 
           \en Create phantom surfaces of fillet/chamfer.\~
  \details \ru Построить фантомные поверхности скругления/фаски и сложить в контейнер surfaces. \n
    По окончании работ поверхности можно и нужно удалить.
           \en Create phantom surfaces of fillet/chamfer and store them in the container 'surfaces'. \n
    After finish working with the surfaces they should be deleted. \~
  \param[in]  params - \ru Параметры построения фантомных поверхностей скругления/фаски.
                       \en Parameters of creating phantom surfaces of fillet/chamfer. \~
  \param[out] result - \ru Поверхности скругления/фаски.
                       \en The fillet/chamfer surfaces. \~
  \return \ru Возвращает код результата построения.
          \en Returns the creation result code. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (MbResultType) SmoothPhantom( const MbSmoothPhantomParams & params,
                                              MbSmoothPhantomResult & result );


//------------------------------------------------------------------------------
/** \brief \ru Построить последовательности гладко стыкующихся рёбер.
           \en Create a sequence of smoothly connected edges.\~
  \details \ru Построить последовательности гладко стыкующихся рёбер, скругляемых одновременно,
               а также поверхности скругления/фаски. \n
               По окончании работ поверхности можно и нужно удалить.
           \en Construct sequences of smoothly connected edges filleted at the same time,
               and fillet/chamfer surfaces. \n
               On the work completion, the surfaces can be and should deleted. \~
  \param[in]  params - \ru Параметры построения.
                       \en Construction parameters. \~
  \param[out] result - \ru Результат операции (построенные последовательности гладко стыкующихся рёбер и поверхности скругления/фаски..
                       \en The operation result (constructed sequences of smoothly connected edges and fillet/chamfer surfaces). \~
  \return \ru Возвращает код результата построения.
          \en \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (MbResultType) SmoothSequence( const MbSmoothSequenceParams & params,
                                               MbSmoothSequenceResult & result );


//------------------------------------------------------------------------------
/** \brief \ru Построить фантомные эквидистантные поверхности для граней оболочки.
           \en Create phantom offset surfaces for faces of a shell. \~
  \details \ru Построить фантомные эквидистантные поверхности для граней оболочки, \n
    кроме имеющих перечислены  кроме имеющих перечисленные индексы, и сложить в массив surfaces. \n
    По окончании работ поверхности можно и нужно удалить.
           \en Create phantom offset surfaces for faces of a shell, \n 
    except the faces with specified indices and store them in array 'surfaces'. \n
    After finish working with the surfaces they should be deleted. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The initial solid. \~
  \param[in] outFaces - \ru Множество вскрываемых граней тела.
                        \en An array of shelling faces of the solid. \~
  \param[in] offFaces - \ru Множество граней, для которых заданы индивидуальные значения толщин.
                        \en An array of faces for which the individual values of thickness are specified. \~
  \param[in] offDists - \ru Множество индивидуальных значений толщин (должен быть синхронизирован с массивом offFaces).
                        \en An array of individual values of thickness (must be synchronized with the array 'offFaces'). \~
  \param[in] params - \ru Параметры операции.
                      \en The operation parameters. \~
  \param[in] operNames - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[out] result - \ru Результат операции.
                       \en The operation result. \~
  \param[out] hpShellFaceInd - \ru Номер грани в исходной оболочки для построения хот-точки.
                               \en The face number in the initial shell for a hot-point creation. \~
  \return \ru Возвращает код результата построения.
          \en Returns the creation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbResultType) OffsetPhantom( const MbSolid &         solid, 
                                              RPArray<MbFace> & outFaces, 
                                              RPArray<MbFace> & offFaces,
                                              SArray<double> &  offDists,
                                        const SweptValues &     params, 
                                        const MbSNameMaker &    operNames,
                                              MbFaceShell *&    result,
                                              size_t *          hpShellFaceInd = nullptr ); // \ru Номер грани в исходной оболочки для построения хот-точки); \en The face number in the initial shell for a hot-point creation);


//------------------------------------------------------------------------------
/** \brief \ru Построить фантом габаритного куба в локальной системе координат.
           \en Create a phantom of a bounding box in local coordinate system. \~
  \details \ru Построить фантом габаритного куба в локальной системе координат. \n
           \en Create a phantom of a bounding box in local coordinate system. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The initial solid. \~
  \param[in] params - \ru Список параметров #MbLocalCubePhantomParams.
                      \en List of parameters #MbLocalCubePhantomParams. \~
  \param[out] result - \ru Фантом локального куба.
                       \en The local bounding box phantom. \~
  \return \ru Возвращает код результата построения.
          \en Returns the creation result code. \~
  \ingroup Shell_Modeling
*/
// ---
MATH_FUNC (MbResultType) LocalCubePhantom( const MbSolid                  & solid,
                                           const MbLocalCubePhantomParams & params,
                                                 c3d::SolidSPtr           & result );


//------------------------------------------------------------------------------
/** \brief \ru Определить фантомное направление усечения.
           \en Determine a phantom direction of truncation. \~
  \details \ru Определить фантомное направление усечения по усеченной грани исходного тела. \n
           \en Determine a phantom direction of truncation given the truncated face of the initial solid. \n \~
  \param[in] truncatingEdge - \ru Ребро усеченной грани исходного тела.
                              \en An edge of truncated face of the initial solid. \~
  \param[in] dirPlace - \ru Система координат направления усечения (Ось Z - направление усечения).
                        \en A coordinate system of truncation direction (Z-axis is a truncation direction). \~
  \return \ru Возвращает true, если получилось определить направление.
          \en Returns true if the direction has been successfully determined. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) TruncatDirection( const MbCurveEdge &   truncatingEdge, 
                                         MbPlacement3D & dirPlace );


//------------------------------------------------------------------------------
/** \brief \ru Построить опорные точки размеров операции скругления/фаски.
           \en Create support points of fillet/chamfer operation sizes. \~
  \details \ru Построить опорные точки размеров операции скругления/фаски и сложить в контейнер data. \n
    Первые две точки лежат на краях поверхности скругления/фаски.
           \en Create support points of fillet/chamfer operation sizes and store them in container 'data'. \n
    The first two points lie on the fillet/chamfer surface boundary. \~
  \param[in]  params - \ru Параметры для построения опорных точек размеров операции скругления/фаски.
                       \en Parameters for creating support points of fillet/chamfer operation sizes. \~
  \param[out] result - \ru Выходные данные построения опорных точек размеров операции скругления/фаски.
                       \en The output data of creating support points of fillet/chamfer operation sizes. \~
  \return \ru Возвращает код результата построения.
          \en Returns the creation result code. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC( MbResultType ) SmoothPositionData( const MbSmoothPositionDataParams & params,
                                                    MbSmoothPositionDataResults & result );


//------------------------------------------------------------------------------
/** \brief \ru Построение функции изменения указанной координаты кривой.
           \en Create a function by one of three coordinates of curve. \~
  \details \ru Для указанной координаты кривой построить склярную функцию её изменения, зависящую от параметра кривой. \n
           \en A function creation for behavior of selected curve coordinate with curve parameter. \n
  \deprecated \ru Метод устарел. Взамен использовать #CreateFunction с набором параметров #MbFunctionParameters.
              \en The method is deprecated. Use instead #CreateFunction with the parameters #MbFunctionParameters. \~
  \param[in]  curve - \ru Кривая.
                      \en The curve. \~
  \param[in]  place - \ru Локальная система координат, в которой используется кривая.
                      \en The local coordinate system that uses the curve. \~
  \param[in]  coordinate - \ru Номер (0,1,2) координаты кривой в локальной системе координат для построения функции.
                           \en The number (0,1,2) of the curve coordinate in the local coordinate system for constructing the function. \~
  \return \ru Возвращает построенную функцию.
          \en Returns the created function. \~
  \ingroup Algorithms_3D
*/
// ---
//DEPRECATE_DECLARE_REPLACE(CreateFunction with MbFunctionParameters)
MATH_FUNC (MbFunction *) CreateFunction( const MbCurve3D & curve, 
                                         const MbPlacement3D & place,
                                               size_t coordinate );


//------------------------------------------------------------------------------
/** \brief \ru Построение скалярной функции изменения Y-координаты кривой.
           \en Create a scalar function by Y-coordinate of curve. \~
  \details \ru Для Y-координаты кривой построить скалярную функцию её изменения, зависящую от X-координаты кривой. \n
           \en A function creation for behavior of Y-curve coordinate with X-curve parameter. \n
  \param[in]  curve - \ru Кривая.
                      \en The curve. \~
  \param[in]  params - \ru Параметры создания скалярной функции.
                       \en The parameters of creating a scalar function. \~
  \return \ru Возвращает построенную функцию.
          \en Returns the created function. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (c3d::FunctionSPtr) CreateFunction( const MbCurve &              curve,
                                              const MbFunctionParameters & params );


//------------------------------------------------------------------------------
/** \brief \ru Вычисление данных фантома для торцев поверхности переменного сечения.
           \en Calculation of the phantom data for the ends of the mutable section surface. \~
  \details \ru Вычисление плоскости сечения, точек направляющих, сторон охватывающего треугольника и вершины на торце поверхности. \n
           \en Calculating the section plane, guide points, sides of the enclosing triangle, and apex at the ends of the surface. \n
  \param[in]  sectionData   - \ru Параметры поверхности переменного сечения.
                              \en The parameters of the mutable section surface. \~
  \param[out] sctionPhantom - \ru Данные охватывающих треугольников на торцах поверхности переменного сечения.
                              \en The data of the enclosing triangles at the ends of the mutable section surface. \~
  \return \ru Возвращает код результата построения.
          \en Returns the creation result code. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (MbResultType) SectionPhantom( const MbSectionData & sectionData,
                                               MbSectionPhantom & sctionPhantom ); 


#endif // __ACTION_PHANTOM_H
