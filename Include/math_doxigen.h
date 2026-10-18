////////////////////////////////////////////////////////////////////////////////
/** 
  \file     
  \brief \ru Группы для документирования с помощью Doxygen.
         \en Groups for documenting by Doxygen. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MATH_DOXIGEN_H
#define __MATH_DOXIGEN_H


//-----------------------------------------------------------------------------
//
//
// \ru Группы геометрического ядра \en Groups of the Geometric Kernel \~
//
//
//-----------------------------------------------------------------------------

/**
  \ru \defgroup  Geometric_Modelling    C3D Modeler: Модуль геометрического моделирования
  \en \defgroup  Geometric_Modelling    C3D Modeler: The Geometric Modelling Module
  \~ \ingroup Geometric_Kernel
*/


/**
  \ru \defgroup  Geometric_Constraints  C3D Solver: Модуль геометрических ограничений
  \en \defgroup  Geometric_Constraints  C3D Solver: The Geometric Constraints Module
  \~ \ingroup Geometric_Kernel
*/


/**
  \ru \defgroup  Data_Exchange          C3D Converter: Модуль конвертеров
  \en \defgroup  Data_Exchange          C3D Converter: The Converters Module
  \~ \ingroup Geometric_Kernel
*/


//-----------------------------------------------------------------------------
//
// \ru Подгруппа Geometric_Modelling - Модуль геометрического моделирования \en The Geometric Modelling Module \~
//
//-----------------------------------------------------------------------------
/**
  \ru \defgroup Geometric_Items         Геометрические объекты
  \en \defgroup Geometric_Items         Geometric Objects
  \~ \ingroup  Geometric_Modelling
*/
/**
  \ru \defgroup  Base_Items             Объекты алгоритмов
  \en \defgroup  Base_Items             Algorithm Objects
  \~ \ingroup  Geometric_Modelling
*/
/**
  \ru \defgroup Modelling_Functions     Методы геометрических построений
  \en \defgroup Modelling_Functions     Geometric Construction Methods
  \~ \ingroup  Geometric_Modelling
*/
/**
  \ru \defgroup  Geometric_Computation  Методы геометрических вычислений
  \en \defgroup  Geometric_Computation  Geometric Computations Methods
  \~ \ingroup Geometric_Modelling
*/
/**
  \ru \defgroup  Base_Tools             Библиотека шаблонов и сериализации
  \en \defgroup  Base_Tools             Templates and Serializations Library
  \~ \ingroup Geometric_Modelling
*/


//-----------------------------------------------------------------------------------
// \ru Подгруппа Geometric_Items - Геометрические объекты \en Geometric Objects \~
//-----------------------------------------------------------------------------------
/**
  \ru \defgroup Model_Items             Объекты геометрической модели
  \en \defgroup Model_Items             Geometric Model Objects
  \~ \ingroup  Geometric_Items
*/
/**
  \ru \defgroup Topology_Items          Топологические объекты
  \en \defgroup Topology_Items          Topological Objects
  \~ \ingroup  Geometric_Items
*/
/**
  \ru \defgroup Surfaces                Поверхности
  \en \defgroup Surfaces                Surfaces
  \~ \ingroup  Geometric_Items
*/
/**
  \ru \defgroup Curves_3D               Кривые
  \en \defgroup Curves_3D               Curves
  \~ \ingroup  Geometric_Items
*/
/**
  \ru \defgroup Point_3D                Точка
  \en \defgroup Point_3D                Point
  \~ \ingroup  Geometric_Items
*/
/**
  \ru \defgroup Legend                  Вспомогательные объекты
  \en \defgroup Legend                  Ancillary Items
  \~ \ingroup  Geometric_Items
*/
/**
  \ru \defgroup Curves_2D               Двумерные кривые
  \en \defgroup Curves_2D               Two-Dimensional uv-Curves
  \~ \ingroup  Geometric_Items
*/
/**
  \ru \defgroup Region_2D               Двумерные области
  \en \defgroup Region_2D               Two-Dimensional Regions
  \~ \ingroup  Geometric_Items
*/


//-----------------------------------------------------------------------------
// \ru Подгруппа Base_Items - базовые объекты \en Base Objects \~
//-----------------------------------------------------------------------------
/**  
  \ru \defgroup Mathematic_Base_3D      Трёхмерные базовые объекты
  \en \defgroup Mathematic_Base_3D      Three-Dimensional Base Objects
  \~ \ingroup  Base_Items
*/
/**  
  \ru \defgroup Mathematic_Base_2D      Двумерные базовые объекты
  \en \defgroup Mathematic_Base_2D      Two-Dimensional Base Objects
  \~ \ingroup  Base_Items
*/
/**  
  \ru \defgroup Model_Creators          Строители
  \en \defgroup Model_Creators          Creators
  \~ \ingroup  Base_Items
*/
/**  
  \ru \defgroup Model_Attributes        Атрибуты
  \en \defgroup Model_Attributes        Attributes
  \~ \ingroup  Base_Items
*/
/**
  \ru \defgroup Functions               Скалярные функции
  \en \defgroup Functions               Scalar Functions
  \~ \ingroup  Base_Items
*/
/**
  \ru \defgroup Build_Parameters        Параметры операций
  \en \defgroup Build_Parameters        Operation Parameters
  \~ \ingroup  Base_Items
*/
/**  
  \ru \defgroup Model_Properties        Свойства
  \en \defgroup Model_Properties        Properties
  \~ \ingroup  Base_Items
*/
/**
  \ru \defgroup Data_Structures         Структуры данных
  \en \defgroup Data_Structures         Data Structures
  \~ \ingroup  Base_Items
*/
/**
  \ru \defgroup Parser                  Разбор строки
  \en \defgroup Parser                  Parser
  \~ \ingroup  Base_Items
*/
/**
  \ru \defgroup Names                   Имена
  \en \defgroup Names                   Names
  \~ \ingroup  Base_Items
*/
/**
  \ru \defgroup Model                   Модель
  \en \defgroup Model                   Model
  \~ \ingroup  Base_Items
*/


//-----------------------------------------------------------------------------
// \ru Подгруппа Build_Parameters - параметры операций \en Operation Parameters \~
//-----------------------------------------------------------------------------
/**
  \ru \defgroup Shell_Building_Parameters           Параметры построения оболочек
  \en \defgroup Shell_Building_Parameters           Shell Building Parameters
  \~ \ingroup  Build_Parameters
*/
/**
  \ru \defgroup Sheet_Metal_Building_Parameters     Параметры построения листовых тел
  \en \defgroup Sheet_Metal_Building_Parameters     Sheet Metal Building Parameters
  \~ \ingroup  Build_Parameters
*/
/**
  \ru \defgroup Direct_Building_Parameters          Параметры прямого редактирования
  \en \defgroup Direct_Building_Parameters          Direct Building Parameters
  \~ \ingroup  Build_Parameters
*/
/**
  \ru \defgroup Surface_Building_Parameters         Параметры построения поверхностей
  \en \defgroup Surface_Building_Parameters         Surface Building Parameters
  \~ \ingroup  Build_Parameters
*/
/**
  \ru \defgroup Curve3D_Building_Parameters         Параметры построения кривых в трехмерном пространстве
  \en \defgroup Curve3D_Building_Parameters         Space Curve Building Parameters
  \~ \ingroup  Build_Parameters
*/
/**
  \ru \defgroup Curve_Building_Parameters           Параметры построения кривых
  \en \defgroup Curve_Building_Parameters           Curve Building Parameters
  \~ \ingroup  Build_Parameters
*/



//-----------------------------------------------------------------------------------
// \ru Подгруппа Modelling_Functions - Методы геометрических построений \en Methods of Geometric Constructions \~
//-----------------------------------------------------------------------------------
/**
  \ru \defgroup Solid_Modeling          Построение тел
  \en \defgroup Solid_Modeling          Solid Modeling
  \~ \ingroup  Modelling_Functions
*/
/**
  \ru \defgroup Shell_Modeling          Построение оболочек
  \en \defgroup Shell_Modeling          Shell Modeling
  \~ \ingroup  Modelling_Functions
*/
/**
  \ru \defgroup Sheet_Metal_Modeling    Построения листовых тел
  \en \defgroup Sheet_Metal_Modeling    Sheet Metal Modeling
  \~ \ingroup  Modelling_Functions
*/
/**
  \ru \defgroup Direct_Modeling         Прямое редактирование тел
  \en \defgroup Direct_Modeling         Direct Solid Modeling
  \~ \ingroup  Modelling_Functions
*/
/**
  \ru \defgroup Surface_Modeling        Построение поверхностей
  \en \defgroup Surface_Modeling        Construction of Surfaces
  \~ \ingroup  Modelling_Functions
*/
/**
  \ru \defgroup Curve3D_Modeling        Построение кривых в трёхмерном пространстве
  \en \defgroup Curve3D_Modeling        Construction of Curves
  \~ \ingroup  Modelling_Functions
*/
/**
  \ru \defgroup Curve_Modeling          Построение кривых в двумерном пространстве
  \en \defgroup Curve_Modeling          Construction of uv-Curves in Two-Dimensional Space
  \~ \ingroup  Modelling_Functions
*/
/**
  \ru \defgroup Point_Modeling          Операции с точками
  \en \defgroup Point_Modeling          Operations with Points
  \~ \ingroup  Modelling_Functions
*/
/**
  \ru \defgroup Base_Algorithms         Базовые алгоритмы
  \en \defgroup Base_Algorithms         Base Algorithms
  \~ \ingroup  Modelling_Functions
*/
/**
  \ru \defgroup Algorithms_3D           Алгоритмы в трёхмерном пространстве
  \en \defgroup Algorithms_3D           Algorithms in Three-Dimensional Space
  \~ \ingroup  Modelling_Functions
*/
/**
  \ru \defgroup Algorithms_2D           Алгоритмы в двумерном пространстве
  \en \defgroup Algorithms_2D           Algorithms in Two-Dimensional Space
  \~ \ingroup  Modelling_Functions
*/


//----------------------------------------------------------------------------
// \ru Подгруппа Geometric_Computation - Методы геометрических расчетов \en Geometric Computations Methods \~
//----------------------------------------------------------------------------
/**
  \ru \defgroup Polygonal_Objects        Полигональные объекты
  \en \defgroup Polygonal_Objects        Polygonal Objects
  \~ \ingroup  Geometric_Computation
*/
/**
  \ru \defgroup Triangulation           Триангуляция
  \en \defgroup Triangulation           Triangulation
  \~ \ingroup  Geometric_Computation
*/
/**
  \ru \defgroup Mapping                 Построение плоских проекций
  \en \defgroup Mapping                 Construction of Plane Projections
  \~ \ingroup  Geometric_Computation
*/
/**
  \ru \defgroup Inertia_Computation     Вычисление инерционных характеристик
  \en \defgroup Inertia_Computation     Mass-Inertial Properties
  \~ \ingroup  Geometric_Computation
*/
/**
  \ru \defgroup Collision_Detection     Определение столкновений
  \en \defgroup Collision_Detection     Collision Detection
  \~ \ingroup  Geometric_Computation
*/
/**
  \ru \defgroup Drawing                 Визуализация объектов
  \en \defgroup Drawing                 Objects Visualization
  \~ \ingroup  Geometric_Computation
*/


//-----------------------------------------------------------------------------
// \ru Подгруппа Base_Tools - Библиотека шаблонов и сериализации \en Library of Templates and Serializations \~
//-----------------------------------------------------------------------------
/**
  \ru \defgroup  Base_Tools_Containers     Контейнеры
  \en \defgroup  Base_Tools_Containers     Containers
  \~ \ingroup  Base_Tools
*/
/**
  \ru \defgroup  Base_Tools_SmartPointers  Автоматические указатели
  \en \defgroup  Base_Tools_SmartPointers  Smart Pointers
  \~ \ingroup  Base_Tools
*/
/**
  \ru \defgroup  Base_Tools_String         Работа со строками
  \en \defgroup  Base_Tools_String         Work with Strings
  \~ \ingroup  Base_Tools
*/
/**
  \ru \defgroup  Base_Tools_IO             Работа с потоками
  \en \defgroup  Base_Tools_IO             Work with Streams
  \~ \ingroup  Base_Tools
*/
/**
  \ru \defgroup  Base_GraphLib             Библиотека графов
  \en \defgroup  Base_GraphLib             Graph Library
  \~ \ingroup  Base_Tools
*/

//-----------------------------------------------------------------------------
// \ru Подгруппа Geometric_Constraints - Модуль геометрических ограничений. \en The Geometric Constraints Module. \~
//-----------------------------------------------------------------------------
/**
  \ru \defgroup Solver_3D                     Геометрические ограничения трёхмерных объектов
  \en \defgroup Solver_3D                     Geometric Constraint Solver in Three-Dimensional Space
  \~ \ingroup  Geometric_Constraints
*/
/**
  \ru \defgroup Solver_2D                     Геометрические ограничения двумерных объектов
  \en \defgroup Solver_2D                     Geometric Constraint Solver in Two-Dimensional Space
  \~ \ingroup  Geometric_Constraints
*/

//-----------------------------------------------------------------------------
// \ru Подгруппа трехмерного геометрического решателя  \en The subgroup of three-dimensional geometric constraint manager (GCM)
//-----------------------------------------------------------------------------
/**
  \ru \defgroup GCM_3D_API  Базовые функции и типы данных
  \en \defgroup GCM_3D_API  Basic functions and data types
  \~ \ingroup  Solver_3D
*/
/**
  \ru \defgroup GCM_3D_ObjectAPI  Объектный интерфейс
  \en \defgroup GCM_3D_ObjectAPI  Object-oriented interface
  \~ \ingroup  Solver_3D
*/
/**
  \ru \defgroup GCM_3D_Routines   Вспомогательные процедуры
  \en \defgroup GCM_3D_Routines   Auxiliary routines
  \~ \ingroup  Solver_3D
*/

//-----------------------------------------------------------------------------
// \ru Подгруппа двумерного геометрического решателя \en The subgroup of two-dimensional geometric constraint engine (GCE)
//-----------------------------------------------------------------------------
/**
  \ru \defgroup Constraints2D_API     Интерфейс 
  \en \defgroup Constraints2D_API     Solver Interface
  \~ \ingroup  Solver_2D
*/

//-----------------------------------------------------------------------------------
//
// \ru Подгруппа Data_Exchange - Модуль конвертеров \en The Converters Module \~
//
//-----------------------------------------------------------------------------------
/**  
  \ru \defgroup Exchange_Interface    Интерфейс конвертеров
  \en \defgroup Exchange_Interface    Converters Interface 
  \~ \ingroup  Data_Exchange
*/
/**  
  \internal
  \ru \defgroup Exchange_Base              Базовые объекты конвертеров
  \en \defgroup Exchange_Base              Converters Basic Objects 
  \~ \ingroup  Data_Exchange
  \endinternal
*/
/**  
  \internal
  \ru \defgroup Exchange_Util              Вспомогательные объекты конвертеров
  \en \defgroup Exchange_Util              Converters Ancillary Facilities 
  \~ \ingroup  Data_Exchange
  \endinternal
*/
/**  
  \internal
  \ru \defgroup Exchange_Algorithms        Алгоритмы конвертеров
  \en \defgroup Exchange_Algorithms        Converters Algorithms 
  \~ \ingroup  Data_Exchange
  \endinternal
*/
/**
  \ru \defgroup Exchange_Formats           Поддерживаемые форматы данных
  \en \defgroup Exchange_Formats           Supported Data Formats 
  \~ \ingroup  Data_Exchange
*/


//-----------------------------------------------------------------------------------
// \ru Подгруппа Exchange_Formats - Поддерживаемые конвертером форматы данных \en Converters Supported Data Formats \~
//-----------------------------------------------------------------------------------
/**
  \ru \defgroup Parasolid_Exchange         Parasolid конвертер
  \en \defgroup Parasolid_Exchange         Parasolid Converter
  \~ \ingroup  Exchange_Formats
*/
/**
  \ru \defgroup ACIS_Exchange              ACIS конвертер
  \en \defgroup ACIS_Exchange              ACIS Converter
  \~ \ingroup  Exchange_Formats
*/
/**
  \ru \defgroup IGES_Exchange              IGES конвертер
  \en \defgroup IGES_Exchange              IGES Converter
  \~ \ingroup  Exchange_Formats
*/
/**
  \ru \defgroup STEP_Exchange              STEP конвертер
  \en \defgroup STEP_Exchange              STEP Converter
  \~ \ingroup  Exchange_Formats
*/
/**
  \ru \defgroup STL_Exchange               STL конвертер
  \en \defgroup STL_Exchange               STL Converter
  \~ \ingroup  Exchange_Formats
*/
/**
  \ru \defgroup VRML_Exchange              VRML конвертер
  \en \defgroup VRML_Exchange              VRML Converter
  \~ \ingroup  Exchange_Formats
*/
/**
  \ru \defgroup DXF_Exchange               DXF конвертер 3D
  \en \defgroup DXF_Exchange               DXF Converter for 3D
  \~ \ingroup  Exchange_Formats
*/


#endif // __MATH_DOXIGEN_H
