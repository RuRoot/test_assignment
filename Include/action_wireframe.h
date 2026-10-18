////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Методы построения каркаса.
         \en Functions for wire frame creation. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ACTION_WIREFRAME_H
#define __ACTION_WIREFRAME_H


#include <wire_frame.h>
#include <wireframe_point.h>


class  MbWireFrameFilletsParams;
class  MbNurbsWireFrameParams;
class  MbTrimmedWireFrameParams;
class  MbWireFrameProjParams;
class  MbWireFrameProjResult;
class  MbOffsetWireFrameParams;
class  MbProjectionWireFrameParams;
class  MbProjectionWireFrameResults;

//------------------------------------------------------------------------------
/** \brief \ru Создать скругленный каркас.
           \en Create a filleted wire frame. \~
  \details \ru Создать каркас в виде плавного соединения (скругления) всех ребер каркаса. \n
               Если два ребра в каркасе гладко стыкуются, в этом стыке скругление не делается, радиус игнорируется. \n
           \en Create a wire frame as fillet of all the edges of a wire frame. \n
               If two edges in wire frame are smoothly connected, the fillet is not created at this joint, the radius is ignored. \n \~
  \param[in] wireframe - \ru Исходный каркас.
                         \en The initial wire frame. \~
  \param[in] sameEdges - \ru Режим копирования входного каркаса.
                         \en Whether to copy the input wire frame. \~
  \param[in] params - \ru Параметры скругления.
                      \en A fillet parameters. \~
  \param[out] result - \ru Скругленный каркас.
                       \en The filleted wire frame. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup WireFrame_Modeling
*/
// ---
MATH_FUNC( MbResultType ) CreateWireFrameFillets(       c3d::WireFrameSPtr &       wireframe,
                                                        MbeCopyMode                sameEdges,
                                                  const MbWireFrameFilletsParams & params,
                                                        c3d::WireFrameSPtr &       result );


//------------------------------------------------------------------------------
/** \brief \ru Построить NURBS копию каркаса.
           \en Construct a NURBS copy of a wire frame. \~
  \details \ru Строит проволочный каркас, содержащий NURBS кривые, аппроксимирующие заданный каркас. 
     По возможности, строит точные кривые, возможно, с кратными узлами.
     Количеством узлов для NURBS определяется в зависимости от кривой.
            \en Constructs a wire frame contains NURBS curves which approximates a given wire frame.
     If it is possible, constructs the accurate curves, perhaps with multiple knots.
     The number of knots for NURBS is defined depending on the curve. \~
  \param[in] wireFrame - \ru Исходный каркас.
                         \en The initial wire frame. \~
  \param[in] sameWire - \ru Режим копирования входного каркаса.
                        \en Whether to copy the input wire frame. \~
  \param[in] params - \ru Параметры скругления.
                      \en A fillet parameters. \~
  \param[out] result - \ru Построенный каркас с NURBS кривыми или nullptr при неуспешном построении.
                       \en he constructed wire frame with NURBS curve or nullptr in a case of failure. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
MATH_FUNC( MbResultType ) CreateNurbsWireFrame(       c3d::WireFrameSPtr &     wireFrame, 
                                                      MbeCopyMode              sameWire,
                                                const MbNurbsWireFrameParams & params, 
                                                      c3d::WireFrameSPtr &     result );


//------------------------------------------------------------------------------
/** \brief \ru Создать усеченный каркас.
           \en Create trimmed wire frame. \~
  \details \ru Создать каркас в виде связной цепочки ребер исходного каркаса, заключенной между заданными точками усечения. \n
               Исходный каркас должен быть нормализован.\n
           \en Create a wire frame in the form of a connected chain of edges of the original wire frame enclosed between truncation points. \n 
               Init wire frame should be normalized. \n \~
  \param[in] wireframe - \ru Исходный каркас.
                         \en The initial wire frame. \~
  \param[in] sameEdges - \ru Режим копирования входного каркаса.
                         \en Whether to copy the input wire frame. \~
  \param[in] params - \ru Параметры усечения.
                      \en Trimmed parameters. \~
  \param[out] result - \ru Усеченный каркас.
                       \en The trimmed wire frame. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup WireFrame_Modeling
*/
// ---
MATH_FUNC( MbResultType ) CreateTrimmedWireFrame(       c3d::WireFrameSPtr &       wireframe,
                                                        MbeCopyMode                sameEdges,
                                                  const MbTrimmedWireFrameParams & params,
                                                        c3d::WireFrameSPtr &       result );


//------------------------------------------------------------------------------
/** \brief  \ru Найти проекцию точки на проволочный каркас.
            \en Find the point projection to the wire frame. \~
    \details \ru Найти ближайшую проекцию точки на проволочный каркас в диапазоне изменения параметра или на его продолжении.
                 Аналог MbContour3D::NearPointProjection().
                 Проволочный каркас должен быть нормализован.
             \en Find the nearest projection of a point to the wire frame within the parameter range or its extension.
                 Analogue of MbContour3D::NearPointProjection().
                 The wire frame must be normalized. \~
    \param[in] wireFrame - \ru Проволочный каркас для поиска проекции.
                           \en Wire frame for searching projection. \~
    \param[in] params - \ru Параметры операции.
                        \en Operation parameters. \~
    \param[in] result - \ru Результаты операции.
                        \en Results of the operation. \~
    \return \ru Код результата операции.
            \en Operation result code. \~
  */
// ---
MATH_FUNC( MbResultType ) NearPointProjection( const MbWireFrame &           wireFrame,
                                               const MbWireFrameProjParams & params,
                                                     MbWireFrameProjResult & result );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки пересечения двух проволочных каркасов.
           \en Calculate intersection points of two wire frames. \~
  \details \ru Найти параметры точек пересечения двух проволочных каркасов. \n
           \en Calculate the parameters of intersection points of two wireframes. \n \~
  \param[in] wireFrame1 - \ru Первый каркас.
                          \en The first wireframe. \~
  \param[in] wireFrame2 - \ru Второй каркас.
                          \en The second wireframe. \~
  \param[in] eps - \ru Возможная максимальная погрешность найденных пересечений.
                   \en The intersection tolerance. \~
  \param[out] result - \ru Пары точек пересечения каркасов.
                       \en The intersection pairs of wire frames. \~
  \return \ru Возвращает код результата операции.
          \en Returns the code of the operation result.
  \ingroup WireFrame_Modeling
*/
// ---
MATH_FUNC( MbResultType ) WireFramesIntersect( const MbWireFrame &                     wireFrame1,
                                               const MbWireFrame &                     wireFrame2, 
                                                     double                            eps,
                                                     c3d::WireFramePointsPairsVector & result );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки пересечения двух ребер с учетом толерантности вершин.
           \en Calculate intersection points of two edges, taking into account the tolerance of the vertices. \~
  \details \ru Найти точки пересечения двух ребер с учетом толерантности вершин. \n
           \en Calculate intersection points of two edges, taking into account the tolerance of the vertices. \n \~
  \param[in] edge1 - \ru Первое ребро.
                     \en The first edge. \~
  \param[in] edge2 - \ru Второе ребро.
                     \en The second edge. \~
  \param[in] eps - \ru Возможная максимальная погрешность найденных пересечений кривых, лежащих под ребрами.
                   \en The intersection tolerance of edge's curves. \~
  \param[out] result - \ru Пары точек пересечения ребер c учетом толерантности вершин.
                       \en The intersection points of two edges, taking into account the tolerance of the vertices. \~
  \return \ru Возвращает количество точек пересечения.
          \en Returns the number of intersection points.
  \ingroup WireFrame_Modeling
*/
// ---
MATH_FUNC( size_t ) EdgesIntersect( const MbEdge &                     edge1,
                                    const MbEdge &                     edge2,
                                          double                       eps,
                                          c3d::EdgePointsPairsVector & result );


//------------------------------------------------------------------------------
  /** \brief \ru Продлить каркас.
             \en Extend the wire frame. \~
    \details \ru Продлить каркас в соответствии с параметрами. \n
             \en Extend the wire frame according to the parameters. \~ \n
    \param[in] wireframe - \ru Исходный (продлеваемый) каркас.
                           \en Source (extended) wire frame. \~
    \param[in] sameEdges - \ru Режим копирования входного каркаса.
                           \en Whether to copy the input wire frame. \~
    \param[in] params    - \ru Параметры продления.
                           \en Parameters of extension. \~
    \param[out] result   - \ru Результирующий каркас.
                           \en Result wire frame. \~
    \return \ru Возвращает код результата операции. В случае успеха всегда вернется ненулевой каркас (возможно, он будет совпадать с исходным каркасом).
            \en Returns the code of the operation result. In case of success, the result wire frame cannot be nullptr (it will be possibly the same as the source one).
    \ingroup Curve3D_Modeling
  */
// ---
MATH_FUNC( MbResultType ) ExtendWireFrame(       c3d::WireFrameSPtr &           wireframe,
                                                 MbeCopyMode                    sameEdges,
                                           const MbCurveExtensionParameters3D & params,
                                                 c3d::WireFrameSPtr &           result );


//------------------------------------------------------------------------------
/** \brief \ru Создать эквидистантный каркас.
           \en Create an offset wire frame. \~
  \details \ru Создать эквидистантный каркас смещением базового каркаса.
           \en Create an offset wire frame by shifting the base wire frame. \~
  \param[in] wireFrame - \ru Исходный каркас.
                         \en The initial wire frame. \~
  \param[in] sameWire - \ru Режим копирования входного каркаса.
                        \en Whether to copy the input wire frame. \~
  \param[in] params - \ru Параметры операции.
                      \en Operation parameters. \~
  \param[out] result - \ru Эквидистантный каркас.
                       \en The offset wire frame. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup WireFrame_Modeling
*/
// ---
MATH_FUNC( MbResultType ) CreateOffsetWireFrame(       c3d::WireFrameSPtr &      wireFrame,
                                                       MbeCopyMode               sameWire,
                                                 const MbOffsetWireFrameParams & params,
                                                       c3d::WireFrameSPtr &      result );


//------------------------------------------------------------------------------
/** \brief \ru Операция построения проекционного проволочного каркаса.
           \en Create a projection wireframe. \~
  \details \ru Операция построения проекции набора кривых по нормали или по
               направлению на тело, набор граней или поверхность. \n
           \en Create a normal or vector projection of a set of curves to
               a solid, set of faces or a surface. \n \~
  \param[in] params   - \ru Параметры операции.
                        \en Operation parameters. \~
  \param[out] results - \ru Результаты операции.
                        \en Operation results. \~
  \return \ru Возвращает код результата операции. Непустой результат возвращается
              при успешном проецировании хотя бы одной из исходных кривых. Если хотя бы одна
              кривая не спроецировалась успешно, возвращается статус ошибки (не rt_Success).
          \en Returns operation result code. Non empty result is returned if at least
              one of the source curves has been projected successfully. If at least one
              source curve has not been projected successfully error status 
              (not rt_Success) will be returned. \~
  \ingroup WireFrame_Modeling
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
MATH_FUNC (MbResultType) ProjectionWireFrame( const MbProjectionWireFrameParams &  params,
                                                    MbProjectionWireFrameResults & results );
#endif // __ACTION_WIREFRAME_H
