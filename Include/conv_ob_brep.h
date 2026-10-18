////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Объекты для передачи топологии модели для создания тела по пользовательским объектам.
         \en Objects for transferring model topology to create a solid based on user-defined objects. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CONV_OB_BREP_H
#define __CONV_OB_BREP_H

#include <conv_ob_geometry.h>
#include <math_define.h>
#include <vector>

namespace c3d_conv_user
{
  using Id = int; ///< \ru Id для Ob Объекта. \en Id for Ob object. 


  //------------------------------------------------------------------------------
  /** \brief  \ru Массив id. 
              \en Array of id's.\~
      \attention \ru Экспериментальная.
                \en Experimental.   
      \ingroup Data_Interface
  */
  // ---
  struct CONV_CLASS ObIdArray {
    std::vector<Id> values; ///< \ru Значения массива. \en Array values.
  };


  //------------------------------------------------------------------------------
  /** \brief  \ru Массив 2D примитивов. 
              \en Array of 2D primitives.\~
      \attention \ru Экспериментальная.
                \en Experimental.   
      \ingroup Data_Interface
  */
  // ---
  struct CONV_CLASS ObPrimitive2DArray {
    ObPrimitive2DArray() {} ///< \ru Конструктор. \en Constructor.
    ObPrimitive2DArray( std::vector<ObPrimitive2D> initValues ) : values( initValues ) {}
    std::vector<ObPrimitive2D> values; ///< \ru Значения массива. \en Array values.
  };


  //------------------------------------------------------------------------------
  /** \brief  \ru Массив 3D примитивов. 
              \en Array of 3D primitives.\~
      \attention \ru Экспериментальная.
                \en Experimental.   
      \ingroup Data_Interface
  */
  // ---
  struct CONV_CLASS ObPrimitive3DArray {
    ObPrimitive3DArray() {} ///< \ru Конструктор. \en Constructor.
    ObPrimitive3DArray( std::vector<ObPrimitive3D> initValues ) : values( initValues ) {}
    std::vector<ObPrimitive3D> values; ///< \ru Значения массива. \en Array values.
  };


  //------------------------------------------------------------------------------
  /** \brief  \ru Массив параметров. 
              \en Array of parameters
  */
  // ---
  struct CONV_CLASS ObDoubleArray {
    std::vector<double> values; ///< \ru Значения массива. \en Array values.
  };


  //------------------------------------------------------------------------------
  /** \brief  \ru Данные для оболочки. 
              \en Data for shell.\~
      \attention \ru Экспериментальная.
                \en Experimental.   
      \ingroup Data_Interface
  */
  // ---
  struct CONV_CLASS ObShell {
    std::vector<Id> faces;  ///< \ru Грани. \en Faces.
    bool closed;              ///< \ru Оболочка замкнута. \en A shell is closed.
  };

  //------------------------------------------------------------------------------
  /** \brief  \ru Данные для грани. 
              \en Data for face.\~
      \attention \ru Экспериментальная.
                \en Experimental.   
      \ingroup Data_Interface
  */
  // ---
  struct CONV_CLASS ObFace {
    Id surface;                ///< \ru Поверхность. \en A surface.
    std::vector<Id> loops;     ///< \ru Циклы. \en Loops.  
    bool sense;                  ///< \ru Ориентация. \en A sense.  
  };


  //------------------------------------------------------------------------------
  /** \brief  \ru Данные для цикла. 
              \en Data for loop.\~
      \attention \ru Экспериментальная.
                \en Experimental.   
      \ingroup Data_Interface
  */
  // ---
  struct CONV_CLASS ObLoop {
    std::vector<Id> coedges {}; ///< \ru Ориентированные ребра. \en Coedges.
  };

  //------------------------------------------------------------------------------
  /** \brief  \ru Данные для ориентированного ребра. 
              \en Data for coedge.\~
      \attention \ru Экспериментальная.
                \en Experimental.    
      \ingroup Data_Interface
  */
  // ---
  struct CONV_CLASS ObCoedge {
    Id edge;        ///< \ru Ребро. \en An edge.
    bool orientation; ///< \ru Ориентация. \en An orientation.
  };

  //------------------------------------------------------------------------------
  /** \brief  \ru Данные для ребра. 
              \en Data for edge.\~
      \attention \ru Экспериментальная.
                \en Experimental.   
      \ingroup Data_Interface
  */
  // ---
  struct CONV_CLASS ObEdge {
    Id curve;      ///< \ru Кривая ребра. \en A curve.
    Id begVertex;  ///< \ru Начальная вершина. \en A start vertex.
    Id endVertex;  ///< \ru Конечная вершина. \en An end vertex.
  };

  //------------------------------------------------------------------------------
  /** \brief  \ru Данные для вершины.
              \en Data for vertex.\~
      \attention \ru Экспериментальная.
                \en Experimental.
      \ingroup Data_Interface
  */
  // ---
  struct CONV_CLASS ObVertex {
    ObPrimitive3D position;      ///< \ru Положение вершины. \en Vertex position.
  };
}

#endif // __CONV_OB_BREP_H
