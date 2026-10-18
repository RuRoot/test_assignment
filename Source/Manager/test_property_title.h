////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Свойства математических объектов.
         \en Properties of mathematical objects. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEST_PROPERTY_TITLE_H
#define __TEST_PROPERTY_TITLE_H


#include <mb_property_title.h>
#include <tool_cstring.h>
/*
  \ru Меняет формат числа.                   \en Changes a number format.
  \ru Не помогло #pragma setlocale("russian. \en #pragma setlocale("russian. didn't help.
*/

#if defined ( __NATIVE_LANGUAGE__ )
#define PROMPT_MESSAGE_RU( strValue ) _sntprintf( v, 256, _T(strValue) )
#define PROMPT_MESSAGE_EN( strValue )
#else
#define PROMPT_MESSAGE_RU( strValue )
#define PROMPT_MESSAGE_EN( strValue ) _sntprintf( v, 256, _T(strValue) )
#endif

#define PROMPT_CASE_( strId, strValue ) case strId : { _sntprintf( v, 256, _T(strValue) ); } break
#if defined ( __NATIVE_LANGUAGE__ )
#define PROMPT_CASE_RU( strId, strValue ) PROMPT_CASE_( strId, strValue )
#define PROMPT_CASE_EN( strId, strValue )
#else
#define PROMPT_CASE_RU( strId, strValue )  
#define PROMPT_CASE_EN( strId, strValue ) PROMPT_CASE_( strId, strValue )
#endif

//------------------------------------------------------------------------------
/** \brief \ru Выдать свойства.
           \en Get properties. \~
  \details \ru Выдать свойства двумерного вектора MbVector.\n
           \en Get properties of two-dimensional vector MbVector.\n \~
  \ingroup Model_Properties
*/
// ---
inline
void GetPromptValue( MbePrompt p, TCHAR * v )
{
  if ( v != nullptr )
  {
    switch ( p ) 
    {
      PROMPT_CASE_RU( IDS_ITEM_0000, " Неопределенный объект" );
      PROMPT_CASE_EN( IDS_ITEM_0000, " Undefined object" );

// \ru Базовые объекты двумерной математики \en Base objects of two-dimensional mathematics

      PROMPT_CASE_RU( IDS_ITEM_0001, " Двумерная точка" );
      PROMPT_CASE_EN( IDS_ITEM_0001, " Two-dimensional Point" );

      PROMPT_CASE_RU( IDS_ITEM_0002, " Двумерный вектор"  );
      PROMPT_CASE_EN( IDS_ITEM_0002, " Two-dimensional Vector" );

      PROMPT_CASE_RU( IDS_ITEM_0003, " Двумерная матрица преобразования" );
      PROMPT_CASE_EN( IDS_ITEM_0003, " Two-dimensional Matrix of Transformation" );

      PROMPT_CASE_RU( IDS_ITEM_0004, " Двумерная локальная система" );
      PROMPT_CASE_EN( IDS_ITEM_0004, " Two-dimensional Local System" );

      PROMPT_CASE_RU( IDS_ITEM_0005, " Двумерный единичный вектор" );
      PROMPT_CASE_EN( IDS_ITEM_0005, " Two-dimensional Unit Vector" );

// \ru Типы двумерных кривых \en Types of two-dimensional curves

      PROMPT_CASE_RU( IDS_ITEM_0011, " Двумерная кривая" );
      PROMPT_CASE_EN( IDS_ITEM_0011, " Two-dimensional Curve" );
      PROMPT_CASE_RU( IDS_ITEM_0013, " Двумерная прямая" );
      PROMPT_CASE_EN( IDS_ITEM_0013, " Two-dimensional Line" );
      PROMPT_CASE_RU( IDS_ITEM_0014, " Двумерный отрезок" );
      PROMPT_CASE_EN( IDS_ITEM_0014, " Two-dimensional Segment" );

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0015 : { _sntprintf( v, 256, _T(" Двумерный отрезок прямой") ); } break;
#else
      case  IDS_ITEM_0015 : { _sntprintf( v, 256, _T(" Two-dimensional Line Segment") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0016 : { _sntprintf( v, 256, _T(" Двумерная дуга окружности") ); } break;
#else
      case  IDS_ITEM_0016 : { _sntprintf( v, 256, _T(" Two-dimensional Circular Arc") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0017 : { _sntprintf( v, 256, _T(" Двумерная усеченная кривая") ); } break;
#else
      case  IDS_ITEM_0017 : { _sntprintf( v, 256, _T(" Two-dimensional Truncated Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0018 : { _sntprintf( v, 256, _T(" Двумерная эквидистантная кривая") ); } break;
#else
      case  IDS_ITEM_0018 : { _sntprintf( v, 256, _T(" Two-dimensional Offset Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0019 : { _sntprintf( v, 256, _T(" Двумерная эквидистанта") ); } break;
#else
      case  IDS_ITEM_0019 : { _sntprintf( v, 256, _T(" Two-dimensional Equidistant") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0020 : { _sntprintf( v, 256, _T(" Двумерная окружность") ); } break;
#else
      case  IDS_ITEM_0020 : { _sntprintf( v, 256, _T(" Two-dimensional Circle") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0021 : { _sntprintf( v, 256, _T(" Двумерный эллипс") ); } break;
#else
      case  IDS_ITEM_0021 : { _sntprintf( v, 256, _T(" Two-dimensional Ellipse") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0022 : { _sntprintf( v, 256, _T(" Двумерная парабола") ); } break;
#else
      case  IDS_ITEM_0022 : { _sntprintf( v, 256, _T(" Two-dimensional Parabola") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0023 : { _sntprintf( v, 256, _T(" Двумерная дуга эллипса") ); } break;
#else
      case  IDS_ITEM_0023 : { _sntprintf( v, 256, _T(" Two-dimensional Elliptical Arc") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0024 : { _sntprintf( v, 256, _T(" Двумерная ломаная") ); } break;
#else
      case  IDS_ITEM_0024 : { _sntprintf( v, 256, _T(" Two-dimensional Polyline") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0025 : { _sntprintf( v, 256, _T(" Двумерная NURBS кривая") ); } break;
#else
      case  IDS_ITEM_0025 : { _sntprintf( v, 256, _T(" Two-dimensional NURBS Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0026 : { _sntprintf( v, 256, _T(" Двумерный сплайн Эрмита") ); } break;
#else
      case  IDS_ITEM_0026 : { _sntprintf( v, 256, _T(" Two-dimensional Hermite Spline") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0027 : { _sntprintf( v, 256, _T(" Двумерный сплайн Безье") ); } break;
#else
      case  IDS_ITEM_0027 : { _sntprintf( v, 256, _T(" Two-dimensional Bezier Spline") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0028 : { _sntprintf( v, 256, _T(" Двумерный кубический сплайн") ); } break;
#else
      case  IDS_ITEM_0028 : { _sntprintf( v, 256, _T(" Two-dimensional Cubic Spline") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0029 : { _sntprintf( v, 256, _T(" Двумерная репараметризованная кривая") ); } break;
#else
      case  IDS_ITEM_0029 : { _sntprintf( v, 256, _T(" Two-dimensional Reparametrized Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0030 : { _sntprintf( v, 256, _T(" Двумерный контур") ); } break;
#else
      case  IDS_ITEM_0030 : { _sntprintf( v, 256, _T(" Two-dimensional Contour") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0031 : { _sntprintf( v, 256, _T(" Двумерная косинусоида") ); } break;
#else
      case  IDS_ITEM_0031 : { _sntprintf( v, 256, _T(" Two-dimensional Cosine Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0032 : { _sntprintf( v, 256, _T(" Двумерная точечная кривая") ); } break;
#else
      case  IDS_ITEM_0032 : { _sntprintf( v, 256, _T(" Two-dimensional Point Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0040 : { _sntprintf( v, 256, _T(" Двумерная область") ); } break;
#else
      case  IDS_ITEM_0040 : { _sntprintf( v, 256, _T(" Two-dimensional Region") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0050 : { _sntprintf( v, 256, _T(" Двумерный объект") ); } break;
#else
      case  IDS_ITEM_0050 : { _sntprintf( v, 256, _T(" Two-dimensional Object") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0051 : { _sntprintf( v, 256, _T(" Двумерная мультилиния") ); } break;
#else
      case  IDS_ITEM_0051 : { _sntprintf( v, 256, _T(" Two-dimensional Multiline") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0052 : { _sntprintf( v, 256, _T(" Двумерная кривая на конусе, соответствующая кривой на коническом изгибе плоскости") ); } break;
#else
      case  IDS_ITEM_0052 : { _sntprintf( v, 256, _T(" Two-dimensional Curve on Cone Corresponding to a Curve on Conic Bend of a Plane") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0053 : { _sntprintf( v, 256, _T(" Двумерная кривая на плоскости, соответствующая кривой на изгибе конуса") ); } break;
#else
      case  IDS_ITEM_0053 : { _sntprintf( v, 256, _T(" Two-dimensional Curve on Plane Corresponding to a Curve on a Bend of Cone") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0054 : { _sntprintf( v, 256, _T(" Двумерная кривая, координатные функции которой заданы в символьном виде") ); } break;
#else
      case  IDS_ITEM_0054 : { _sntprintf( v, 256, _T(" Functionally Defined Two-dimensional Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0055 : { _sntprintf( v, 256, _T(" Образ трехмерной кривой на поверхности при движении по направляющей") ); } break;
#else
      case  IDS_ITEM_0055 : { _sntprintf( v, 256, _T(" Image of a Three-dimensional Curve on Surface while moving along the guide curve") ); } break;
#endif

// \ru Типы полигональных объектов \en Types of polygonal objects

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0060 : { _sntprintf( v, 256, _T(" Параметр") ); } break;
#else
      case  IDS_ITEM_0060 : { _sntprintf( v, 256, _T(" Parameter") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0061 : { _sntprintf( v, 256, _T(" Вершина") ); } break;
#else
      case  IDS_ITEM_0061 : { _sntprintf( v, 256, _T(" Vertex") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0062 : { _sntprintf( v, 256, _T(" Нормаль") ); } break;
#else
      case  IDS_ITEM_0062 : { _sntprintf( v, 256, _T(" Normal") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0063 : { _sntprintf( v, 256, _T(" Треугольник") ); } break;
#else
      case  IDS_ITEM_0063 : { _sntprintf( v, 256, _T(" Triangle") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0064 : { _sntprintf( v, 256, _T(" Quadrangle") ); } break;
#else
      case  IDS_ITEM_0064 : { _sntprintf( v, 256, _T(" Quadrangle") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0071 : { _sntprintf( v, 256, _T(" Полигональный объект на числах double") ); } break;
#else
      case  IDS_ITEM_0071 : { _sntprintf( v, 256, _T(" Polygonal Object on Double Data") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0072 : { _sntprintf( v, 256, _T(" Апекс на числах double") ); } break;
#else
      case  IDS_ITEM_0072 : { _sntprintf( v, 256, _T(" Apex on Double Data") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0073 : { _sntprintf( v, 256, _T(" Полигон на числах double") ); } break;
#else
      case  IDS_ITEM_0073 : { _sntprintf( v, 256, _T(" Polygon on Double Data") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0074 : { _sntprintf( v, 256, _T(" Триангуляция на числах double") ); } break;
#else
      case  IDS_ITEM_0074 : { _sntprintf( v, 256, _T(" Triangulation on Double Data") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0075 : { _sntprintf( v, 256, _T(" Полигональный объект на числах float") ); } break;
#else
      case  IDS_ITEM_0075 : { _sntprintf( v, 256, _T(" Polygonal Object on Float Data") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0076 : { _sntprintf( v, 256, _T(" Апекс на числах float") ); } break;
#else
      case  IDS_ITEM_0076 : { _sntprintf( v, 256, _T(" Apex on Float Data") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0077 : { _sntprintf( v, 256, _T(" Полигон на числах float") ); } break;
#else
      case  IDS_ITEM_0077 : { _sntprintf( v, 256, _T(" Polygon on Float Data") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0078 : { _sntprintf( v, 256, _T(" Триангуляция на числах float") ); } break;
#else
      case  IDS_ITEM_0078 : { _sntprintf( v, 256, _T(" Triangulation on Float Data") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0079 : { _sntprintf( v, 256, _T(" Вывернутый полигональный объект") ); } break;
#else
      case  IDS_ITEM_0079 : { _sntprintf( v, 256, _T(" Reversed polygonal object") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0080:  { _sntprintf( v, 256, _T(" Разрезанный полигональный объект") ); } break;
#else
      case  IDS_ITEM_0080:  { _sntprintf( v, 256, _T(" Cutted polygonal object") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0081:  { _sntprintf( v, 256, _T(" Только сечение") ); } break;
#else
      case  IDS_ITEM_0081:  { _sntprintf( v, 256, _T(" Only section") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0082: { _sntprintf( v, 256, _T( " Полигональная сетка " ) ); } break;
#else
      case  IDS_ITEM_0082: { _sntprintf( v, 256, _T( " Polymesh " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0083: { _sntprintf( v, 256, _T( " Регион " ) ); } break;
#else
      case  IDS_ITEM_0083: { _sntprintf( v, 256, _T( " Region " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0084: { _sntprintf( v, 256, _T( " Подрегион " ) ); } break;
#else
      case  IDS_ITEM_0084: { _sntprintf( v, 256, _T( " Subregion " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0085: { _sntprintf( v, 256, _T( " Цикл " ) ); } break;
#else
      case  IDS_ITEM_0085: { _sntprintf( v, 256, _T( " Loop " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0086: { _sntprintf( v, 256, _T( " Ребро " ) ); } break;
#else
      case  IDS_ITEM_0086: { _sntprintf( v, 256, _T( " Edge " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0087: { _sntprintf( v, 256, _T( " Вершина " ) ); } break;
#else
      case  IDS_ITEM_0087: { _sntprintf( v, 256, _T( " Vertex " ) ); } break;
#endif

// \ru Базовые объекты трехмерной математики \en Base objects of three-dimensional mathematics

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0101 : { _sntprintf( v, 256, _T(" Точка") ); } break;
#else
      case  IDS_ITEM_0101 : { _sntprintf( v, 256, _T(" Point") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0102 : { _sntprintf( v, 256, _T(" Вектор") ); } break;
#else
      case  IDS_ITEM_0102 : { _sntprintf( v, 256, _T(" Vector") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0103 : { _sntprintf( v, 256, _T(" Матрица преобразования") ); } break;
#else
      case  IDS_ITEM_0103 : { _sntprintf( v, 256, _T(" Transformation Matrix") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0104 : { _sntprintf( v, 256, _T(" Локальная система координат") ); } break;
#else
      case  IDS_ITEM_0104 : { _sntprintf( v, 256, _T(" Local Coordinate System") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0105 : { _sntprintf( v, 256, _T(" Ось") ); } break;
#else
      case  IDS_ITEM_0105 : { _sntprintf( v, 256, _T(" Axis") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0107 : { _sntprintf( v, 256, _T(" Перемещение") ); } break;
#else
      case  IDS_ITEM_0107 : { _sntprintf( v, 256, _T(" Translation") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0108 : { _sntprintf( v, 256, _T(" Вращение") ); } break;
#else
      case  IDS_ITEM_0108 : { _sntprintf( v, 256, _T(" Rotation") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0109 : { _sntprintf( v, 256, _T(" Трансформация") ); } break;
#else
      case  IDS_ITEM_0109 : { _sntprintf( v, 256, _T(" Transformation") ); } break;
#endif

// \ru Типы функций \en Types of functions

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0111 : { _sntprintf( v, 256, _T(" Константная функция") ); } break;
#else
      case  IDS_ITEM_0111 : { _sntprintf( v, 256, _T(" Constant Function") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0112 : { _sntprintf( v, 256, _T(" Линейная функция") ); } break;
#else
      case  IDS_ITEM_0112 : { _sntprintf( v, 256, _T(" Linear Function") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0113 : { _sntprintf( v, 256, _T(" Кубическая функция") ); } break;
#else
      case  IDS_ITEM_0113 : { _sntprintf( v, 256, _T(" Cubic Function") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0114 : { _sntprintf( v, 256, _T(" Кубическай сплайн-функция") ); } break;
#else
      case  IDS_ITEM_0114 : { _sntprintf( v, 256, _T(" Cubic Spline Function") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0115 : { _sntprintf( v, 256, _T(" Символьная функция") ); } break;
#else
      case  IDS_ITEM_0115 : { _sntprintf( v, 256, _T(" Symbolic Function") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0116 : { _sntprintf( v, 256, _T(" Степенная функция") ); } break;
#else
      case  IDS_ITEM_0116 : { _sntprintf( v, 256, _T(" Power Function") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0117 : { _sntprintf( v, 256, _T(" Синус функция") ); } break;
#else
      case  IDS_ITEM_0117 : { _sntprintf( v, 256, _T(" Sinus Function") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0118 : { _sntprintf( v, 256, _T(" Служебная функция") ); } break;
#else
      case  IDS_ITEM_0118 : { _sntprintf( v, 256, _T("Service function") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0119 : { _sntprintf( v, 256, _T(" Монотонная функция") ); } break;
#else
      case  IDS_ITEM_0119 : { _sntprintf( v, 256, _T(" Monotonous function") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0120 : { _sntprintf( v, 256, _T(" NURBS функция") ); } break;
#else
      case  IDS_ITEM_0120 : { _sntprintf( v, 256, _T(" NURBS function") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0121 : { _sntprintf( v, 256, _T(" Функция координаты кривой") ); } break;
#else
      case  IDS_ITEM_0121 : { _sntprintf( v, 256, _T(" Function by curve coordinate") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0191 : { _sntprintf( v, 256, _T(" Символьная функция") ); } break;
#else
      case  IDS_ITEM_0191 : { _sntprintf( v, 256, _T(" Symbolic function") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0192 : { _sntprintf( v, 256, _T(" Символьная функция на модельном выражении") ); } break;
#else
      case  IDS_ITEM_0192 : { _sntprintf( v, 256, _T(" Symbolic function in model expression") ); } break;
#endif

// \ru Типы трехмерных кривых \en Types of three-dimensional curves

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0201 : { _sntprintf( v, 256, _T(" Кривая") ); } break;
#else
      case  IDS_ITEM_0201 : { _sntprintf( v, 256, _T(" Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0202 : { _sntprintf( v, 256, _T(" B-сплайн") ); } break;
#else
      case  IDS_ITEM_0202 : { _sntprintf( v, 256, _T(" B-Spline") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0213 : { _sntprintf( v, 256, _T(" Прямая линия") ); } break;
#else
      case  IDS_ITEM_0213 : { _sntprintf( v, 256, _T(" Straight Line") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0214 : { _sntprintf( v, 256, _T(" Отрезок") ); } break;
#else
      case  IDS_ITEM_0214 : { _sntprintf( v, 256, _T(" Segment") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0215 : { _sntprintf( v, 256, _T(" Дуга эллипса") ); } break;
#else
      case  IDS_ITEM_0215 : { _sntprintf( v, 256, _T(" Elliptic Arc") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0216 : { _sntprintf( v, 256, _T(" Дуга окружности") ); } break;
#else
      case  IDS_ITEM_0216 : { _sntprintf( v, 256, _T(" Circular Arc") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0217 : { _sntprintf( v, 256, _T(" Усеченная кривая") ); } break;
#else
      case  IDS_ITEM_0217 : { _sntprintf( v, 256, _T(" Truncated Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0218 : { _sntprintf( v, 256, _T(" Эквидистантная кривая") ); } break;
#else
      case  IDS_ITEM_0218 : { _sntprintf( v, 256, _T(" Offset Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0219 : { _sntprintf( v, 256, _T(" Коническая спираль") ); } break;
#else
      case  IDS_ITEM_0219 : { _sntprintf( v, 256, _T(" Conical spiral") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0220 : { _sntprintf( v, 256, _T(" Окружность") ); } break;
#else
      case  IDS_ITEM_0220 : { _sntprintf( v, 256, _T(" Circle") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0221 : { _sntprintf( v, 256, _T(" Эллипс") ); } break;
#else
      case  IDS_ITEM_0221 : { _sntprintf( v, 256, _T(" Ellipse") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0222 : { _sntprintf( v, 256, _T(" Парабола") ); } break;
#else
      case  IDS_ITEM_0222 : { _sntprintf( v, 256, _T(" Parabola") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0223 : { _sntprintf( v, 256, _T(" Гипербола") ); } break;
#else
      case  IDS_ITEM_0223 : { _sntprintf( v, 256, _T(" Hyperbola") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0224 : { _sntprintf( v, 256, _T(" Ломаная линия") ); } break;
#else
      case  IDS_ITEM_0224 : { _sntprintf( v, 256, _T(" Polyline") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0225 : { _sntprintf( v, 256, _T(" NURBS кривая") ); } break;
#else
      case  IDS_ITEM_0225 : { _sntprintf( v, 256, _T(" NURBS Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0226 : { _sntprintf( v, 256, _T(" Сплайн Эрмита") ); } break;
#else
      case  IDS_ITEM_0226 : { _sntprintf( v, 256, _T(" Hermite Spline") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0227 : { _sntprintf( v, 256, _T(" Сплайн Безье") ); } break;
#else
      case  IDS_ITEM_0227 : { _sntprintf( v, 256, _T(" Bezier Spline") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0228 : { _sntprintf( v, 256, _T(" Кубический сплайн") ); } break;
#else
      case  IDS_ITEM_0228 : { _sntprintf( v, 256, _T(" Cubic Spline") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0229 : { _sntprintf( v, 256, _T(" Репараметризованная кривая") ); } break;
#else
      case  IDS_ITEM_0229 : { _sntprintf( v, 256, _T(" Reparametrized Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0231 : { _sntprintf( v, 256, _T(" Плоская кривая") ); } break;
#else
      case  IDS_ITEM_0231 : { _sntprintf( v, 256, _T(" Plane Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0232 : { _sntprintf( v, 256, _T(" Спираль с переменым радиусом") ); } break;
#else
      case  IDS_ITEM_0232 : { _sntprintf( v, 256, _T(" Spiral with variable radius") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0233 : { _sntprintf( v, 256, _T(" Спираль с криволинейной осью") ); } break;
#else
      case  IDS_ITEM_0233 : { _sntprintf( v, 256, _T(" Spiral with a Curved Axis") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0234 : { _sntprintf( v, 256, _T(" Кривая-мостик") ); } break;
#else
      case  IDS_ITEM_0234 : { _sntprintf( v, 256, _T(" Bridge Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0235 : { _sntprintf( v, 256, _T(" Символьная кривая") ); } break;
#else
      case  IDS_ITEM_0235 : { _sntprintf( v, 256, _T(" Functionally Defined Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0236 : { _sntprintf( v, 256, _T(" Кривая на поверхности") ); } break;
#else
      case  IDS_ITEM_0236 : { _sntprintf( v, 256, _T(" Curve on a Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0237 : { _sntprintf( v, 256, _T(" Линия пересечения поверхностей") ); } break;
#else
      case  IDS_ITEM_0237 : { _sntprintf( v, 256, _T(" Intersection Curve of Surfaces") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0238 : { _sntprintf( v, 256, _T(" Контур на поверхности") ); } break;
#else
      case  IDS_ITEM_0238 : { _sntprintf( v, 256, _T(" Contour on a Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0239 : { _sntprintf( v, 256, _T(" Контур на плоскости") ); } break;
#else
      case  IDS_ITEM_0239 : { _sntprintf( v, 256, _T(" Contour on a Plane") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0240 : { _sntprintf( v, 256, _T(" Контур") ); } break;
#else
      case  IDS_ITEM_0240 : { _sntprintf( v, 256, _T(" Contour") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0241 : { _sntprintf( v, 256, _T(" Проекционная кривая") ); } break;
#else
      case  IDS_ITEM_0241 : { _sntprintf( v, 256, _T(" Projection Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0242 : { _sntprintf( v, 256, _T(" Силуэтная кривая") ); } break;
#else
      case  IDS_ITEM_0242 : { _sntprintf( v, 256, _T(" Silhouette Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0243 : { _sntprintf( v, 256, _T(" Кривая сопряжения кривых") ); } break;
#else
      case  IDS_ITEM_0243 : { _sntprintf( v, 256, _T(" Curve of Curves Conjugation") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0244 : { _sntprintf( v, 256, _T(" Кривая производных Кунса") ); } break;
#else
      case  IDS_ITEM_0244 : { _sntprintf( v, 256, _T(" Curve of Coons Derivetives") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0249 : { _sntprintf( v, 256, _T(" Направляющая кривая") ); } break;
#else
      case  IDS_ITEM_0249 : { _sntprintf( v, 256, _T(" Guide Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0250 : { _sntprintf( v, 256, _T(" Кривая пересечения") ); } break;
#else
      case  IDS_ITEM_0250 : { _sntprintf( v, 256, _T(" Intersection Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0251 : { _sntprintf( v, 256, _T(" Первое направляющее ребро") ); } break;
#else
      case  IDS_ITEM_0251 : { _sntprintf( v, 256, _T(" First Guide Edge") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0252 : { _sntprintf( v, 256, _T(" Первая направляющая кривая") ); } break;
#else
      case  IDS_ITEM_0252 : { _sntprintf( v, 256, _T(" First Guide Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0253 : { _sntprintf( v, 256, _T(" Первая опорная грань") ); } break;
#else
      case  IDS_ITEM_0253 : { _sntprintf( v, 256, _T(" First Reference Face") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0254 : { _sntprintf( v, 256, _T(" Первая контрольная кривая") ); } break;
#else
      case  IDS_ITEM_0254 : { _sntprintf( v, 256, _T(" First Control Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0255 : { _sntprintf( v, 256, _T(" Первая функция") ); } break;
#else
      case  IDS_ITEM_0255 : { _sntprintf( v, 256, _T(" First Function") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0256 : { _sntprintf( v, 256, _T(" Второе направляющее ребро") ); } break;
#else
      case  IDS_ITEM_0256 : { _sntprintf( v, 256, _T(" Second Guide Edge") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0257 : { _sntprintf( v, 256, _T(" Вторая направляющая кривая") ); } break;
#else
      case  IDS_ITEM_0257 : { _sntprintf( v, 256, _T(" Second Guide Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0258 : { _sntprintf( v, 256, _T(" Вторая опорная грань") ); } break;
#else
      case  IDS_ITEM_0258 : { _sntprintf( v, 256, _T(" Second Reference Face") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0259 : { _sntprintf( v, 256, _T(" Вторая контрольная кривая") ); } break;
#else
      case  IDS_ITEM_0259 : { _sntprintf( v, 256, _T(" Second Control Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0260 : { _sntprintf( v, 256, _T(" Вторая функция") ); } break;
#else
      case  IDS_ITEM_0260 : { _sntprintf( v, 256, _T(" Second Function") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0261 : { _sntprintf( v, 256, _T(" Угол к хорде") ); } break;
#else
      case  IDS_ITEM_0261 : { _sntprintf( v, 256, _T(" Angle from Chord") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0262 : { _sntprintf( v, 256, _T(" Угол к касательной поверхности") ); } break;
#else
      case  IDS_ITEM_0262 : { _sntprintf( v, 256, _T(" Angle from Surface Tangent") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0263 : { _sntprintf( v, 256, _T(" Угол к нормали поверхности") ); } break;
#else
      case  IDS_ITEM_0263 : { _sntprintf( v, 256, _T(" Angle from Surface Normal") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0264 : { _sntprintf( v, 256, _T(" Удлиненная кривая") ); } break;
#else
      case  IDS_ITEM_0264 : { _sntprintf( v, 256, _T(" Extended curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0269: { _sntprintf( v, 256, _T(" Развёрнутая кривая") ); } break;
#else
      case  IDS_ITEM_0269: { _sntprintf( v, 256, _T(" Unwrapped curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0270: { _sntprintf( v, 256, _T(" Свёрнутая кривая") ); } break;
#else
      case  IDS_ITEM_0270: { _sntprintf( v, 256, _T(" Wrapped curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0271: { _sntprintf( v, 256, _T(" Балочная кривая") ); } break;
#else
      case  IDS_ITEM_0271: { _sntprintf( v, 256, _T(" Beam curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0272: { _sntprintf( v, 256, _T( " Скругленный каркас" ) ); } break;
#else
      case  IDS_ITEM_0272: { _sntprintf( v, 256, _T( " Filleted wire frame" ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0273: { _sntprintf( v, 256, _T( " NURBS по каркасу" ) ); } break;
#else
      case  IDS_ITEM_0273: { _sntprintf( v, 256, _T( " NURBS by wire frame" ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0274: { _sntprintf(v, 256, _T( " Усечение каркаса") ); } break;
#else
      case  IDS_ITEM_0274: { _sntprintf(v, 256, _T( " Trimmed wire frame") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0275: { _sntprintf( v, 256, _T( " Развернутая точка" ) ); } break;
#else
      case  IDS_ITEM_0275: { _sntprintf( v, 256, _T( " Unwrapped point" ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0276: { _sntprintf( v, 256, _T( " Свёрнутая точка" ) ); } break;
#else
      case  IDS_ITEM_0276: { _sntprintf( v, 256, _T( " Wrapped point" ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0277: { _sntprintf( v, 256, _T( " Эквидистанта каркаса" ) ); } break;
#else
      case  IDS_ITEM_0277: { _sntprintf(v, 256, _T( " Offset wire frame") ); } break;
#endif


#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_2030: { _sntprintf( v, 256, _T(" Тип входных данных") ); } break;
#else
      case  IDS_ITEM_2030: { _sntprintf( v, 256, _T(" Type of input data") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_2031: { _sntprintf( v, 256, _T(" Тип изменений") ); } break;
#else
      case  IDS_ITEM_2031: { _sntprintf( v, 256, _T(" Change type") ); } break;
#endif

// \ru Типы параметрических поверхностей \en Types of parametric surfaces

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0301 : { _sntprintf( v, 256, _T(" Поверхность") ); } break;
#else
      case  IDS_ITEM_0301 : { _sntprintf( v, 256, _T(" Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0302 : { _sntprintf( v, 256, _T(" Поверхность движения") ); } break;
#else
      case  IDS_ITEM_0302 : { _sntprintf( v, 256, _T(" Swept Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0303 : { _sntprintf( v, 256, _T(" Поверхность сдвига") ); } break;
#else
      case  IDS_ITEM_0303 : { _sntprintf( v, 256, _T(" Motion Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0304 : { _sntprintf( v, 256, _T(" Поверхность выдавливания") ); } break;
#else
      case  IDS_ITEM_0304 : { _sntprintf( v, 256, _T(" Extrusion Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0305 : { _sntprintf( v, 256, _T(" Поверхность вращения") ); } break;
#else
      case  IDS_ITEM_0305 : { _sntprintf( v, 256, _T(" Revolution Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0306 : { _sntprintf( v, 256, _T(" Линейчатая поверхность") ); } break;
#else
      case  IDS_ITEM_0306 : { _sntprintf( v, 256, _T(" Ruled Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0307 : { _sntprintf( v, 256, _T(" Поверхность по кривой и точке") ); } break;
#else
      case  IDS_ITEM_0307 : { _sntprintf( v, 256, _T(" Surface Defined by a Curve and a Point") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0308 : { _sntprintf( v, 256, _T(" Четырехугольная поверхность") ); } break;
#else
      case  IDS_ITEM_0308 : { _sntprintf( v, 256, _T(" Quadrangular Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0309 : { _sntprintf( v, 256, _T(" Треугольная поверхность") ); } break;
#else
      case  IDS_ITEM_0309 : { _sntprintf( v, 256, _T(" Triangular Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0310 : { _sntprintf( v, 256, _T(" Поверхность движения с доворотом") ); } break;
#else
      case  IDS_ITEM_0310 : { _sntprintf( v, 256, _T(" Sweep Surface with Rotating Ends") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0311 : { _sntprintf( v, 256, _T(" Поверхность на семействе кривых и направляющей") ); } break;
#else
      case  IDS_ITEM_0311 : { _sntprintf( v, 256, _T(" Loft Surface with Guide Curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0312 : { _sntprintf( v, 256, _T(" Спиральная поверхность") ); } break;
#else
      case  IDS_ITEM_0312 : { _sntprintf( v, 256, _T(" Spiral Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0313 : { _sntprintf( v, 256, _T(" Цилиндрически согнутая поверхность") ); } break;
#else
      case  IDS_ITEM_0313 : { _sntprintf( v, 256, _T(" Cylindrically Bent Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0314 : { _sntprintf( v, 256, _T(" Цилиндрически разогнутая поверхность") ); } break;
#else
      case  IDS_ITEM_0314 : { _sntprintf( v, 256, _T(" Cylindrically Unbent Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0315 : { _sntprintf( v, 256, _T(" Конически согнутая поверхность") ); } break;
#else
      case  IDS_ITEM_0315 : { _sntprintf( v, 256, _T(" Conically Bent Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0316 : { _sntprintf( v, 256, _T(" Конически разогнутая поверхность") ); } break;
#else
      case  IDS_ITEM_0316 : { _sntprintf( v, 256, _T(" Conically Unbent Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0317 : { _sntprintf( v, 256, _T(" Поверхность заметания с масштабированием и поворотом") ); } break;
#else
      case  IDS_ITEM_0317 : { _sntprintf( v, 256, _T(" Swept Surface with Scaling and Winding") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0319 : { _sntprintf( v, 256, _T(" Плоскость") ); } break;
#else
      case  IDS_ITEM_0319 : { _sntprintf( v, 256, _T(" Plane") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0320 : { _sntprintf( v, 256, _T(" Сферическая поверхность") ); } break;
#else
      case  IDS_ITEM_0320 : { _sntprintf( v, 256, _T(" Spherical Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0321 : { _sntprintf( v, 256, _T(" Тороидальная поверхность") ); } break;
#else
      case  IDS_ITEM_0321 : { _sntprintf( v, 256, _T(" Toroidal Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0322 : { _sntprintf( v, 256, _T(" Цилиндрическая поверхность") ); } break;
#else
      case  IDS_ITEM_0322 : { _sntprintf( v, 256, _T(" Cylindrical Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0323 : { _sntprintf( v, 256, _T(" Коническая поверхность") ); } break;
#else
      case  IDS_ITEM_0323 : { _sntprintf( v, 256, _T(" Conical Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0325 : { _sntprintf( v, 256, _T(" NURBS поверхность") ); } break;
#else
      case  IDS_ITEM_0325 : { _sntprintf( v, 256, _T(" NURBS Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0326 : { _sntprintf( v, 256, _T(" Треугольная NURBS поверхность") ); } break;
#else
      case  IDS_ITEM_0326 : { _sntprintf( v, 256, _T(" Triangular NURBS Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0327 : { _sntprintf( v, 256, _T(" Поверхность Безье") ); } break;
#else
      case  IDS_ITEM_0327 : { _sntprintf( v, 256, _T(" Bezier Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0328 : { _sntprintf( v, 256, _T(" Эквидистантная поверхность") ); } break;
#else
      case  IDS_ITEM_0328 : { _sntprintf( v, 256, _T(" Offset Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0329 : { _sntprintf( v, 256, _T(" Деформированная поверхность") ); } break;
#else
      case  IDS_ITEM_0329 : { _sntprintf( v, 256, _T(" Deformed Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0331 : { _sntprintf( v, 256, _T(" Поверхность соединения") ); } break;
#else
      case  IDS_ITEM_0331 : { _sntprintf( v, 256, _T(" Joint Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0332 : { _sntprintf( v, 256, _T(" Поверхность объединения") ); } break;
#else
      case  IDS_ITEM_0332 : { _sntprintf( v, 256, _T(" Join Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0333 : { _sntprintf( v, 256, _T(" Поверхность на трех кривых") ); } break;
#else
      case  IDS_ITEM_0333 : { _sntprintf( v, 256, _T(" Surface Based on Three Curves") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0334 : { _sntprintf( v, 256, _T(" Поверхность на четырех кривых") ); } break;
#else
      case  IDS_ITEM_0334 : { _sntprintf( v, 256, _T(" Surface Based on Four Curves") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0335 : { _sntprintf( v, 256, _T(" Поверхность-фаска") ); } break;
#else
      case  IDS_ITEM_0335 : { _sntprintf( v, 256, _T(" Chamfer Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0336 : { _sntprintf( v, 256, _T(" Поверхность скругления") ); } break;
#else
      case  IDS_ITEM_0336 : { _sntprintf( v, 256, _T(" Fillet Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0337 : { _sntprintf( v, 256, _T(" Переменная поверхность скругления") ); } break;
#else
      case  IDS_ITEM_0337 : { _sntprintf( v, 256, _T(" Variable Fillet Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0338 : { _sntprintf( v, 256, _T(" Поверхность на семействе кривых") ); } break;
#else
      case  IDS_ITEM_0338 : { _sntprintf( v, 256, _T(" Lofted Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0339 : { _sntprintf( v, 256, _T(" Поверхность на сетке кривых") ); } break;
#else
      case  IDS_ITEM_0339 : { _sntprintf( v, 256, _T(" Surface Defined on a Mesh of Curves") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0340 : { _sntprintf( v, 256, _T(" Поверхность скругления по кромке") ); } break;
#else
      case  IDS_ITEM_0340 : { _sntprintf( v, 256, _T(" Surface of Fillet by Border") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0341 : { _sntprintf( v, 256, _T(" Поверхность по замкнутому контуру") ); } break;
#else
      case  IDS_ITEM_0341 : { _sntprintf( v, 256, _T(" Surface on Closed Contour") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0342 : { _sntprintf( v, 256, _T(" Плазовая поверхность") ); } break;
#else
      case  IDS_ITEM_0342 : { _sntprintf( v, 256, _T(" Spiling Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0343 : { _sntprintf( v, 256, _T(" Поверхность Кунса") ); } break;
#else
      case  IDS_ITEM_0343 : { _sntprintf( v, 256, _T(" Coons Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0345 : { _sntprintf( v, 256, _T(" Поверхность на сетке точек") ); } break;
#else
      case  IDS_ITEM_0345 : { _sntprintf( v, 256, _T(" Surface Based on a Point Grid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0346 : { _sntprintf( v, 256, _T(" Треугольная поверхность Безье") ); } break;
#else
      case  IDS_ITEM_0346 : { _sntprintf( v, 256, _T(" Triangular Bezier Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0349 : { _sntprintf( v, 256, _T(" Усеченная контурами поверхность") ); } break;
#else
      case  IDS_ITEM_0349 : { _sntprintf( v, 256, _T(" Curve bounded Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0350 : { _sntprintf( v, 256, _T(" Поверхность-копия") ); } break;
#else
      case  IDS_ITEM_0350 : { _sntprintf( v, 256, _T(" Copy Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0351 : { _sntprintf( v, 256, _T(" Поверхность соединения") ); } break;
#else
      case  IDS_ITEM_0351 : { _sntprintf( v, 256, _T(" Joint Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0352: { _sntprintf( v, 256, _T( " Поверхность полного скругления" ) ); } break;
#else
      case  IDS_ITEM_0352: { _sntprintf( v, 256, _T( " Full Fillet Surface" ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0353: { _sntprintf( v, 256, _T( " Поверхность заметания с масштабированием" ) ); } break;
#else
      case  IDS_ITEM_0353: { _sntprintf( v, 256, _T( " Swept Surface with Scaling" ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0354: { _sntprintf( v, 256, _T( " Поверхность переменного сечения" ) ); } break;
#else
      case  IDS_ITEM_0354: { _sntprintf( v, 256, _T( " Swept Section Surface" ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0355: { _sntprintf( v, 256, _T( " Пользовательская поверхность" ) ); } break;
#else
      case  IDS_ITEM_0355: { _sntprintf( v, 256, _T( " User Defined Surface" ) ); } break;
#endif


// \ru Типы тел \en Types of solids

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0401 : { _sntprintf( v, 256, _T(" Тело") ); } break;
#else
      case  IDS_ITEM_0401 : { _sntprintf( v, 256, _T(" Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0402 : { _sntprintf( v, 256, _T(" Оболочка") ); } break;
#else
      case  IDS_ITEM_0402 : { _sntprintf( v, 256, _T(" Shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0403 : { _sntprintf( v, 256, _T(" Проволочный каркас") ); } break;
#else
      case  IDS_ITEM_0403 : { _sntprintf( v, 256, _T(" Wire Frame") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0404 : { _sntprintf( v, 256, _T(" Точечный каркас") ); } break;
#else
      case  IDS_ITEM_0404 : { _sntprintf( v, 256, _T(" Point Frame") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0405 : { _sntprintf( v, 256, _T(" Коллекция элементов") ); } break;
#else
      case  IDS_ITEM_0405 : { _sntprintf( v, 256, _T(" Collection of Elements") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0411 : { _sntprintf( v, 256, _T(" Тело первой группы") ); } break;
#else
      case  IDS_ITEM_0411 : { _sntprintf( v, 256, _T(" Solid of The First Set") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0412 : { _sntprintf( v, 256, _T(" Тело второй группы") ); } break;
#else
      case  IDS_ITEM_0412 : { _sntprintf( v, 256, _T(" Solid of The Second Set") ); } break;
#endif

// \ru Параметры операций. \en Operation options.

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0471 : { _sntprintf( v, 256, _T(" Сохранять параметризацию") ); } break;
#else
      case  IDS_PROP_0471 : { _sntprintf( v, 256, _T(" Save parameterization") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0472 : { _sntprintf( v, 256, _T(" Строить на продолжении") ); } break;
#else
      case  IDS_PROP_0472 : { _sntprintf( v, 256, _T(" Build on continuation") ); } break;
#endif



// \ru Типы строителей \en Types of creators

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0501 : { _sntprintf( v, 256, _T(" Журнал построения") ); } break;
#else
      case  IDS_ITEM_0501 : { _sntprintf( v, 256, _T(" Build Log") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0502 : { _sntprintf( v, 256, _T(" Шар") ); } break;
#else
      case  IDS_ITEM_0502 : { _sntprintf( v, 256, _T(" Sphere") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0503 : { _sntprintf( v, 256, _T(" Тор") ); } break;
#else
      case  IDS_ITEM_0503 : { _sntprintf( v, 256, _T(" Torus") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0504 : { _sntprintf( v, 256, _T(" Цилиндр") ); } break;
#else
      case  IDS_ITEM_0504 : { _sntprintf( v, 256, _T(" Cylinder") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0505 : { _sntprintf( v, 256, _T(" Конус") ); } break;
#else
      case  IDS_ITEM_0505 : { _sntprintf( v, 256, _T(" Cone") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0506 : { _sntprintf( v, 256, _T(" Блок") ); } break;
#else
      case  IDS_ITEM_0506 : { _sntprintf( v, 256, _T(" Block") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0507 : { _sntprintf( v, 256, _T(" Клин") ); } break;
#else
      case  IDS_ITEM_0507 : { _sntprintf( v, 256, _T(" Wedge") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0508 : { _sntprintf( v, 256, _T(" Призма") ); } break;
#else
      case  IDS_ITEM_0508 : { _sntprintf( v, 256, _T(" Prism") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0509 : { _sntprintf( v, 256, _T(" Пирамида") ); } break;
#else
      case  IDS_ITEM_0509 : { _sntprintf( v, 256, _T(" Pyramid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0510 : { _sntprintf( v, 256, _T(" Твёрдое тело") ); } break;
#else
      case  IDS_ITEM_0510 : { _sntprintf( v, 256, _T(" Solid ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0511 : { _sntprintf( v, 256, _T(" Плита") ); } break;
#else
      case  IDS_ITEM_0511 : { _sntprintf( v, 256, _T(" Plate ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0512 : { _sntprintf( v, 256, _T(" Икосаэдр") ); } break;
#else
      case  IDS_ITEM_0512 : { _sntprintf( v, 256, _T(" Icosahedron ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0513 : { _sntprintf( v, 256, _T(" Многогранник") ); } break;
#else
      case  IDS_ITEM_0513 : { _sntprintf( v, 256, _T(" Polyhedron ") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0515 : { _sntprintf( v, 256, _T(" Объединение оболочек") ); } break;
#else
      case  IDS_ITEM_0515 : { _sntprintf( v, 256, _T(" Shells Union") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0516 : { _sntprintf( v, 256, _T(" Пересечение оболочек") ); } break;
#else
      case  IDS_ITEM_0516 : { _sntprintf( v, 256, _T(" Shells Intersection") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0517 : { _sntprintf( v, 256, _T(" Разность оболочек") ); } break;
#else
      case  IDS_ITEM_0517 : { _sntprintf( v, 256, _T(" Shells Subtraction") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0518 : { _sntprintf( v, 256, _T(" Нормализация отверстия") ); } break;
#else
      case  IDS_ITEM_0518 : { _sntprintf( v, 256, _T(" Holes normalization") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0520 : { _sntprintf( v, 256, _T(" Отверстие") ); } break;
#else
      case  IDS_ITEM_0520 : { _sntprintf( v, 256, _T(" Hole") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0521 : { _sntprintf( v, 256, _T(" Карман/Бобышка") ); } break;
#else
      case  IDS_ITEM_0521 : { _sntprintf( v, 256, _T(" Pocket/Boss") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0522 : { _sntprintf( v, 256, _T(" Паз") ); } break;
#else
      case  IDS_ITEM_0522 : { _sntprintf( v, 256, _T(" Groove") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0523 : { _sntprintf( v, 256, _T(" Заплатка ") ); } break;
#else
      case  IDS_ITEM_0523 : { _sntprintf( v, 256, _T(" Patch ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0524 : { _sntprintf( v, 256, _T(" Тонкая оболочка") ); } break;
#else
      case  IDS_ITEM_0524 : { _sntprintf( v, 256, _T(" Thin Shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0525: { _sntprintf( v, 256, _T( " Листовое тело по произвольному телу" ) ); } break;
#else
      case  IDS_ITEM_0525: { _sntprintf( v, 256, _T( " Sheet solid based on an arbitrary solid" ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0526 : { _sntprintf( v, 256, _T(" Оболочка на семействе кривых") ); } break;
#else
      case  IDS_ITEM_0526 : { _sntprintf( v, 256, _T(" Shell Defined by a Set of Curves") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0527 : { _sntprintf( v, 256, _T(" Продолженная оболочка") ); } break;
#else
      case  IDS_ITEM_0527 : { _sntprintf( v, 256, _T(" Extended Shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0528 : { _sntprintf( v, 256, _T(" Эквидистантная оболочка") ); } break;
#else
      case  IDS_ITEM_0528 : { _sntprintf( v, 256, _T(" Offset Shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0529 : { _sntprintf( v, 256, _T(" Срединная оболочка") ); } break;
#else
      case  IDS_ITEM_0529 : { _sntprintf( v, 256, _T(" Median Shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0530 : { _sntprintf( v, 256, _T(" Оболочка переменного сечения") ); } break;
#else
      case  IDS_ITEM_0530 : { _sntprintf( v, 256, _T(" Swept Mutable Section Shell") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0531 : { _sntprintf( v, 256, _T(" Булево объединение тел") ); } break;
#else
      case  IDS_ITEM_0531 : { _sntprintf( v, 256, _T(" Boolean Union of Solids") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0532 : { _sntprintf( v, 256, _T(" Булево пересечение тел") ); } break;
#else
      case  IDS_ITEM_0532 : { _sntprintf( v, 256, _T(" Boolean Intersection of Solids") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0533 : { _sntprintf( v, 256, _T(" Булева разность тел") ); } break;
#else
      case  IDS_ITEM_0533 : { _sntprintf( v, 256, _T(" Boolean Subtraction of Solids") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0534 : { _sntprintf( v, 256, _T(" Разрезанное тело") ); } break;
#else
      case  IDS_ITEM_0534 : { _sntprintf( v, 256, _T(" Cut Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0535 : { _sntprintf( v, 256, _T(" Фаски ребер") ); } break;
#else
      case  IDS_ITEM_0535 : { _sntprintf( v, 256, _T(" Edges Chamfers") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0536 : { _sntprintf( v, 256, _T(" Скругление ребер") ); } break;
#else
      case  IDS_ITEM_0536 : { _sntprintf( v, 256, _T(" Edges Fillets") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0537 : { _sntprintf( v, 256, _T(" Симметричное тело") ); } break;
#else
      case  IDS_ITEM_0537 : { _sntprintf( v, 256, _T(" Symmetric Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0538 : { _sntprintf( v, 256, _T(" Оболочечное тело") ); } break;
#else
      case  IDS_ITEM_0538 : { _sntprintf( v, 256, _T(" Thin Shell Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0539 : { _sntprintf( v, 256, _T(" Тело приданием толщины") ); } break;
#else
      case  IDS_ITEM_0539 : { _sntprintf( v, 256, _T(" Solid of Thickening") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0540 : { _sntprintf( v, 256, _T(" Оболочка с удалёнными гранями") ); } break;
#else
      case  IDS_ITEM_0540 : { _sntprintf( v, 256, _T(" Shell with Removed Faces") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0541 : { _sntprintf( v, 256, _T(" Коробчатое тело") ); } break;
#else
      case  IDS_ITEM_0541 : { _sntprintf( v, 256, _T(" Box-like Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0542 : { _sntprintf( v, 256, _T(" Кинематическое тело") ); } break;
#else
      case  IDS_ITEM_0542 : { _sntprintf( v, 256, _T(" Sweeping Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0543 : { _sntprintf( v, 256, _T(" Тело заметания") ); } break;
#else
      case  IDS_ITEM_0543 : { _sntprintf( v, 256, _T(" Swept Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0544 : { _sntprintf( v, 256, _T(" Тело выдавливания") ); } break;
#else
      case  IDS_ITEM_0544 : { _sntprintf( v, 256, _T(" Extrusion Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0545 : { _sntprintf( v, 256, _T(" Тело вращения") ); } break;
#else
      case  IDS_ITEM_0545 : { _sntprintf( v, 256, _T(" Revolution Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0546 : { _sntprintf( v, 256, _T(" Тело по сечениям") ); } break;
#else
      case  IDS_ITEM_0546 : { _sntprintf( v, 256, _T(" Loft Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0547 : { _sntprintf( v, 256, _T(" Простое тело") ); } break;
#else
      case  IDS_ITEM_0547 : { _sntprintf( v, 256, _T(" Simple Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0548 : { _sntprintf( v, 256, _T(" Ребро жесткости тела") ); } break;
#else
      case  IDS_ITEM_0548 : { _sntprintf( v, 256, _T(" Rib of a Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0549 : { _sntprintf( v, 256, _T(" Набор тел") ); } break;
#else
      case  IDS_ITEM_0549 : { _sntprintf( v, 256, _T(" Set of Solids") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0550 : { _sntprintf( v, 256, _T(" Часть набора тел") ); } break;
#else
      case  IDS_ITEM_0550 : { _sntprintf( v, 256, _T(" Set of Solids part") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0551 : { _sntprintf( v, 256, _T(" Уклон граней тела") ); } break;
#else
      case  IDS_ITEM_0551 : { _sntprintf( v, 256, _T(" Solid's Faces Drafting") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0552 : { _sntprintf( v, 256, _T(" Разбивка граней  тела") ); } break;
#else
      case  IDS_ITEM_0552 : { _sntprintf( v, 256, _T(" Splitting of Solid's Faces") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0553 : { _sntprintf( v, 256, _T(" Сшитое из оболочек тело") ); } break;
#else
      case  IDS_ITEM_0553 : { _sntprintf( v, 256, _T(" Stitched Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0554 : { _sntprintf( v, 256, _T(" Сшитая из оболочек оболочка") ); } break;
#else
      case  IDS_ITEM_0554 : { _sntprintf( v, 256, _T(" Shell Stitched from Shells") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0555 : { _sntprintf( v, 256, _T(" Оболочка из NURBS-поверхностей") ); } break;
#else
      case  IDS_ITEM_0555 : { _sntprintf( v, 256, _T(" Shell from NURBS-surfaces") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0556 : { _sntprintf( v, 256, _T(" Трансформированное тело") ); } break;
#else
      case  IDS_ITEM_0556 : { _sntprintf( v, 256, _T(" Transformed Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0557 : { _sntprintf( v, 256, _T(" Модифицированное тело") ); } break;
#else
      case  IDS_ITEM_0557 : { _sntprintf( v, 256, _T(" Modified Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0558 : { _sntprintf( v, 256, _T(" Оболочка из линейчатых поверхностей" ) ); } break;
#else
      case  IDS_ITEM_0558 : { _sntprintf( v, 256, _T(" Shell from Ruled Surfaces" ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0559 : { _sntprintf( v, 256, _T(" Усеченная оболочка" ) ); } break;
#else
      case  IDS_ITEM_0559 : { _sntprintf( v, 256, _T(" Truncated Shell" ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0560 : { _sntprintf( v, 256, _T(" Оболочка соединения" ) ); } break;
#else
      case  IDS_ITEM_0560 : { _sntprintf( v, 256, _T(" Joint Shell" ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0561 : { _sntprintf( v, 256, _T(" Тело с восстановленными боковыми рёбрами") ); } break;
#else
      case  IDS_ITEM_0561 : { _sntprintf( v, 256, _T(" Solid with Restored Lateral Edges") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0562 : { _sntprintf( v, 256, _T(" Объединение с кинематическим телом") ); } break;
#else
      case  IDS_ITEM_0562 : { _sntprintf( v, 256, _T(" Union with a Sweeping Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0563 : { _sntprintf( v, 256, _T(" Модифицированное тело NURBS-поверхностями") ); } break;
#else
      case  IDS_ITEM_0563 : { _sntprintf( v, 256, _T(" Solid Modified with NURBS-surfaces") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0564 : { _sntprintf( v, 256, _T(" Объединение с телом выдавливания") ); } break;
#else
      case  IDS_ITEM_0564 : { _sntprintf( v, 256, _T(" Union with an Extrusion Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0565 : { _sntprintf( v, 256, _T(" Объединение с телом вращения") ); } break;
#else
      case  IDS_ITEM_0565 : { _sntprintf( v, 256, _T(" Union with a Revolution Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0566 : { _sntprintf( v, 256, _T(" Объединение с телом по сечениям") ); } break;
#else
      case  IDS_ITEM_0566 : { _sntprintf( v, 256, _T(" Union with a Lofted Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0567 : { _sntprintf( v, 256, _T(" Объединение с простым телом") ); } break;
#else
      case  IDS_ITEM_0567 : { _sntprintf( v, 256, _T(" Union with an Elementary Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0568 : { _sntprintf( v, 256, _T(" Модифицированная NURBS-поверхность грани") ); } break;
#else
      case  IDS_ITEM_0568 : { _sntprintf( v, 256, _T(" Modified NURBS-Surface of the Face") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0569 : { _sntprintf( v, 256, _T(" Объединение с набором тел") ); } break;
#else
      case  IDS_ITEM_0569 : { _sntprintf( v, 256, _T(" Union with a Solid Set") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0570 : { _sntprintf( v, 256, _T(" Оболочка грани соединения") ); } break;
#else
      case  IDS_ITEM_0570 : { _sntprintf( v, 256, _T(" Joint Face Shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0571 : { _sntprintf( v, 256, _T(" Скругление грани") ); } break;
#else
      case  IDS_ITEM_0571 : { _sntprintf( v, 256, _T(" Face Fillet") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0572 : { _sntprintf( v, 256, _T(" Разность с кинематическим телом") ); } break;
#else
      case  IDS_ITEM_0572 : { _sntprintf( v, 256, _T(" Subtraction with a Sweeping Solid") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0573 : { _sntprintf( v, 256, _T(" Удаление результата операции") ); } break;
#else
      case  IDS_ITEM_0573 : { _sntprintf( v, 256, _T(" Delete the result of the operation") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0574 : { _sntprintf( v, 256, _T(" Разность с телом выдавливания") ); } break;
#else
      case  IDS_ITEM_0574 : { _sntprintf( v, 256, _T(" Subtraction with an Extrusion Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0575 : { _sntprintf( v, 256, _T(" Разность с телом вращения") ); } break;
#else
      case  IDS_ITEM_0575 : { _sntprintf( v, 256, _T(" Subtraction with a Revolution Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0576 : { _sntprintf( v, 256, _T(" Разность с телом по сечениям") ); } break;
#else
      case  IDS_ITEM_0576 : { _sntprintf( v, 256, _T(" Subtraction with a Lofted Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0577 : { _sntprintf( v, 256, _T(" Разность с простым телом") ); } break;
#else
      case  IDS_ITEM_0577 : { _sntprintf( v, 256, _T(" Subtraction with an Elementary Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0578 : { _sntprintf( v, 256, _T(" Упрощение развёртки") ); } break;
#else
      case  IDS_ITEM_0578 : { _sntprintf( v, 256, _T(" Flat pattern simplification") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0579 : { _sntprintf( v, 256, _T(" Разность с набором тел") ); } break;
#else
      case  IDS_ITEM_0579 : { _sntprintf( v, 256, _T(" Subtraction with a Set of Solids") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0780 : { _sntprintf( v, 256, _T(" Вывернутое тело") ); } break;
#else
      case  IDS_ITEM_0780 : { _sntprintf( v, 256, _T(" Reversed Solid") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0581 : { _sntprintf( v, 256, _T(" Сгиб нелистового тела") ); } break;
#else
      case  IDS_ITEM_0581 : { _sntprintf( v, 256, _T(" Bend of a Non-sheet Solid") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0582 : { _sntprintf( v, 256, _T(" Пересечение с кинематическим телом") ); } break;
#else
      case  IDS_ITEM_0582 : { _sntprintf( v, 256, _T(" Intersection with a Sweeping Solid") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0583 : { _sntprintf( v, 256, _T(" Сферическая штамповка") ); } break;
#else
      case  IDS_ITEM_0583 : { _sntprintf( v, 256, _T(" Spherical Stamping") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0584 : { _sntprintf( v, 256, _T(" Пересечение с телом выдавливания") ); } break;
#else
      case  IDS_ITEM_0584 : { _sntprintf( v, 256, _T(" Intersection with an Extrusion Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0585 : { _sntprintf( v, 256, _T(" Пересечение с телом вращения") ); } break;
#else
      case  IDS_ITEM_0585 : { _sntprintf( v, 256, _T(" Intersection with a Revolution Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0586 : { _sntprintf( v, 256, _T(" Пересечение с телом по сечениям") ); } break;
#else
      case  IDS_ITEM_0586 : { _sntprintf( v, 256, _T(" Intersection with a Lofted Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0587 : { _sntprintf( v, 256, _T(" Пересечение с простым телом") ); } break;
#else
      case  IDS_ITEM_0587 : { _sntprintf( v, 256, _T(" Intersection with an Elementary Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0588 : { _sntprintf( v, 256, _T(" Обечайка") ); } break;
#else
      case  IDS_ITEM_0588 : { _sntprintf( v, 256, _T(" Ruled Shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0589 : { _sntprintf( v, 256, _T(" Пересечение с набором тел") ); } break;
#else
      case  IDS_ITEM_0589 : { _sntprintf( v, 256, _T(" Intersection with a Set of Solids") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0590 : { _sntprintf( v, 256, _T(" Оболочка по сети кривых") ); } break;
#else
      case  IDS_ITEM_0590 : { _sntprintf( v, 256, _T(" Shell Defined by a Mesh of Curves") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0591 : { _sntprintf( v, 256, _T(" Комбинированный сгиб") ); } break;
#else
      case  IDS_ITEM_0591 : { _sntprintf( v, 256, _T(" Combined Bend") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0592 : { _sntprintf( v, 256, _T(" Кинематическая оболочка") ); } break;
#else
      case  IDS_ITEM_0592 : { _sntprintf( v, 256, _T(" Sweeping Shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0593 : { _sntprintf( v, 256, _T(" Жалюзи") ); } break;
#else
      case  IDS_ITEM_0593 : { _sntprintf( v, 256, _T(" Jalousie") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0594 : { _sntprintf( v, 256, _T(" Оболочка выдавливания") ); } break;
#else
      case  IDS_ITEM_0594 : { _sntprintf( v, 256, _T(" Extrusion Shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0595 : { _sntprintf( v, 256, _T(" Оболочка вращения") ); } break;
#else
      case  IDS_ITEM_0595 : { _sntprintf( v, 256, _T(" Revolution Shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0596 : { _sntprintf( v, 256, _T(" Оболочка по сечениям") ); } break;
#else
      case  IDS_ITEM_0596 : { _sntprintf( v, 256, _T(" Loft Shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0597 : { _sntprintf( v, 256, _T(" Тонкая оболочка") ); } break;
#else
      case  IDS_ITEM_0597 : { _sntprintf( v, 256, _T(" Thin Shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0598 : { _sntprintf( v, 256, _T(" Разрезанная оболочка") ); } break;
#else
      case  IDS_ITEM_0598 : { _sntprintf( v, 256, _T(" Split Shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0599 : { _sntprintf( v, 256, _T(" Буртик") ); } break;
#else
      case  IDS_ITEM_0599 : { _sntprintf( v, 256, _T(" Bead") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0600 : { _sntprintf( v, 256, _T(" Сгиб/разгиб листового тела") ); } break;
#else
      case  IDS_ITEM_0600 : { _sntprintf( v, 256, _T(" Bend/Unbend of a Sheet Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0601 : { _sntprintf( v, 256, _T(" Сгиб листового тела по отрезку") ); } break;
#else
      case  IDS_ITEM_0601 : { _sntprintf( v, 256, _T(" Bend of a sheet Solid by a Segment") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0602 : { _sntprintf( v, 256, _T(" Сгиб листового тела по рёбрам") ); } break;
#else
      case  IDS_ITEM_0602 : { _sntprintf( v, 256, _T(" Bend of a Sheet Solid along Edges") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0603 : { _sntprintf( v, 256, _T(" Замыкание угла листового тела") ); } break;
#else
      case  IDS_ITEM_0603 : { _sntprintf( v, 256, _T(" Closure of a Sheet Solid Corner") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0604 : { _sntprintf( v, 256, _T(" Листовое тело") ); } break;
#else
      case  IDS_ITEM_0604 : { _sntprintf( v, 256, _T(" Sheet Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0605 : { _sntprintf( v, 256, _T(" Пластина листового тела") ); } break;
#else
      case  IDS_ITEM_0605 : { _sntprintf( v, 256, _T(" Sheet Solid Plate") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0606 : { _sntprintf( v, 256, _T(" Вырез листового тела") ); } break;
#else
      case  IDS_ITEM_0606 : { _sntprintf( v, 256, _T(" Cut of a Sheet Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0607 : { _sntprintf( v, 256, _T(" Пересечение листового тела") ); } break;
#else
      case  IDS_ITEM_0607 : { _sntprintf( v, 256, _T(" Sheet Solid Intersection") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0608 : { _sntprintf( v, 256, _T(" Подсечка листового тела") ); } break;
#else
      case  IDS_ITEM_0608 : { _sntprintf( v, 256, _T(" Jog of a Sheet Solid") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0609 : { _sntprintf( v, 256, _T(" Штамповка") ); } break;
#else
      case  IDS_ITEM_0609 : { _sntprintf( v, 256, _T(" Stamping") ); } break;
#endif

// \ru Способы построения оболочек \en Shells construction methods

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0610 : { _sntprintf( v, 256, _T(" Оболочка") ); } break;
#else
      case  IDS_ITEM_0610 : { _sntprintf( v, 256, _T(" Shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0611 : { _sntprintf( v, 256, _T(" Оболочка на базе поверхности") ); } break;
#else
      case  IDS_ITEM_0611 : { _sntprintf( v, 256, _T(" Shell Based on a Surface") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0614 : { _sntprintf( v, 256, _T(" Оболочка тела") ); } break;
#else
      case  IDS_ITEM_0614 : { _sntprintf( v, 256, _T(" Shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0615 : { _sntprintf( v, 256, _T(" Журнал построения") ); } break;
#else
      case  IDS_ITEM_0615 : { _sntprintf( v, 256, _T(" Build Log") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0616 : { _sntprintf( v, 256, _T(" Атрибуты объекта") ); } break;
#else
      case  IDS_ITEM_0616 : { _sntprintf( v, 256, _T(" Object Attributes") ); } break;
#endif // __NATIVE_LANGUAGE__

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0620 : { _sntprintf( v, 256, _T(" Оболочка тела") ); } break;
#else
      case  IDS_ITEM_0620 : { _sntprintf( v, 256, _T(" Shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0621 : { _sntprintf( v, 256, _T(" Ориентированное ребро цикла") ); } break;
#else
      case  IDS_ITEM_0621 : { _sntprintf( v, 256, _T(" Oriented Edge of a Loop") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0622 : { _sntprintf( v, 256, _T(" Цикл грани") ); } break;
#else
      case  IDS_ITEM_0622 : { _sntprintf( v, 256, _T(" Face Loop") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0623 : { _sntprintf( v, 256, _T(" Грань оболочки") ); } break;
#else
      case  IDS_ITEM_0623 : { _sntprintf( v, 256, _T(" Face of a Shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0624 : { _sntprintf( v, 256, _T(" Вершина") ); } break;
#else
      case  IDS_ITEM_0624 : { _sntprintf( v, 256, _T(" Vertex") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0625 : { _sntprintf( v, 256, _T(" Ребро оболочки") ); } break;
#else
      case  IDS_ITEM_0625 : { _sntprintf( v, 256, _T(" Edge of a Shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0626 : { _sntprintf( v, 256, _T(" Ребро каркаса") ); } break;
#else
      case  IDS_ITEM_0626 : { _sntprintf( v, 256, _T(" Edge of a Frame") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0627 : { _sntprintf( v, 256, _T(" Вершина") ); } break;
#else
      case  IDS_ITEM_0627 : { _sntprintf( v, 256, _T(" Vertex") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0628 : { _sntprintf( v, 256, _T(" Разделенная оболочка") ); } break;
#else
      case  IDS_ITEM_0628 : { _sntprintf( v, 256, _T(" Divided Shell") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0630 : { _sntprintf( v, 256, _T(" Оболочка кругового сечения") ); } break;
#else
      case  IDS_ITEM_0630 : { _sntprintf( v, 256, _T(" Round Section Shell") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0631 : { _sntprintf( v, 256, _T(" Оболочка лигнейчатого сечения") ); } break;
#else
      case  IDS_ITEM_0631 : { _sntprintf( v, 256, _T(" Ruled Section Shell") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0632 : { _sntprintf( v, 256, _T(" Оболочка конического сечения") ); } break;
#else
      case  IDS_ITEM_0632 : { _sntprintf( v, 256, _T(" Conic Section Shell") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0633 : { _sntprintf( v, 256, _T(" Оболочка кубического сечения") ); } break;
#else
      case  IDS_ITEM_0633 : { _sntprintf( v, 256, _T(" Cubic Section Shell") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0634 : { _sntprintf( v, 256, _T(" Оболочка сплайнового сечения") ); } break;
#else
      case  IDS_ITEM_0634 : { _sntprintf( v, 256, _T(" Spline Section Shell") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0635 : { _sntprintf( v, 256, _T(" Оболочка скругления диском") ); } break;
#else
      case  IDS_ITEM_0635 : { _sntprintf( v, 256, _T(" Disk Fillet Shell") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0636 : { _sntprintf( v, 256, _T(" Оболочка скругления сферой") ); } break;
#else
      case  IDS_ITEM_0636 : { _sntprintf( v, 256, _T(" Sphere Fillet Shell") ); } break;
#endif

// \ru Способы построения проекций тела\оболочки \en Solid\shell projections creation methods

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0650 : { _sntprintf( v, 256, _T(" Проекция тела ") ); } break;
#else
      case  IDS_ITEM_0650 : { _sntprintf( v, 256, _T(" Solid Projection ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0651 : { _sntprintf( v, 256, _T(" Разрез тела ") ); } break;
#else
      case  IDS_ITEM_0651 : { _sntprintf( v, 256, _T(" Solid Cutting ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0652 : { _sntprintf( v, 256, _T(" Сечение тела ") ); } break;
#else
      case  IDS_ITEM_0652 : { _sntprintf( v, 256, _T(" Solid Section ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0653 : { _sntprintf( v, 256, _T(" Копия объекта") ); } break;
#else
      case  IDS_ITEM_0653 : { _sntprintf( v, 256, _T(" Object copy") ); } break;
#endif

// \ru Вспомогательный объект \en The helper object

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0669 : { _sntprintf( v, 256, _T(" Вспомогательный объект ") ); } break;
#else
      case  IDS_ITEM_0669 : { _sntprintf( v, 256, _T(" The Helper Object ") ); } break;
#endif

// \ru Резьба \en A thread

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0670 : { _sntprintf( v, 256, _T(" Резьба ") ); } break;
#else
      case  IDS_ITEM_0670 : { _sntprintf( v, 256, _T(" Thread ") ); } break;
#endif

// \ru Обозначение \en Notation

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0671 : { _sntprintf( v, 256, _T(" Условное обозначение ") ); } break;
#else
      case  IDS_ITEM_0671 : { _sntprintf( v, 256, _T(" Symbolic Notation ") ); } break;
#endif


#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0672 : { _sntprintf( v, 256, _T(" Элемент аннотации ") ); } break;
#else
      case  IDS_ITEM_0672 : { _sntprintf( v, 256, _T(" PMI ") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0673 : { _sntprintf( v, 256, _T(" Тип элемента аннотации ") ); } break;
#else
      case  IDS_ITEM_0673 : { _sntprintf( v, 256, _T(" PMI type ") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0674 : { _sntprintf( v, 256, _T(" Текстовый элемент ") ); } break;
#else
      case  IDS_ITEM_0674 : { _sntprintf( v, 256, _T(" Text item ") ); } break;
#endif


#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0675 : { _sntprintf( v, 256, _T(" Составной текст ") ); } break;
#else
      case  IDS_ITEM_0675 : { _sntprintf( v, 256, _T(" Composite text ") ); } break;
#endif


// \ru Объекты \en Objects

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0700 : { _sntprintf( v, 256, _T(" Геометрический объект") ); } break;
#else
      case  IDS_ITEM_0700 : { _sntprintf( v, 256, _T(" Geometric Object") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0701 : { _sntprintf( v, 256, _T(" Переменная уравнения") ); } break;
#else
      case  IDS_ITEM_0701 : { _sntprintf( v, 256, _T(" Equation Variable") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0702 : { _sntprintf( v, 256, _T(" Объект на плоскости") ); } break;
#else
      case  IDS_ITEM_0702 : { _sntprintf( v, 256, _T(" Object on a Plane") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0703 : { _sntprintf( v, 256, _T(" Объект в пространстве") ); } break;
#else
      case  IDS_ITEM_0703 : { _sntprintf( v, 256, _T(" Object in Space") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0704 : { _sntprintf( v, 256, _T(" Объект модели") ); } break;
#else
      case  IDS_ITEM_0704 : { _sntprintf( v, 256, _T(" Model object") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0705 : { _sntprintf( v, 256, _T(" Сборочная единица") ); } break;
#else
      case  IDS_ITEM_0705 : { _sntprintf( v, 256, _T(" Assembly Unit") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0706 : { _sntprintf( v, 256, _T(" Вспомогательный объект ") ); } break;
#else
      case  IDS_ITEM_0706 : { _sntprintf( v, 256, _T(" Auxiliary Object") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0707 : { _sntprintf( v, 256, _T(" Вставка объекта") ); } break;
#else
      case  IDS_ITEM_0707 : { _sntprintf( v, 256, _T(" Object Instance") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0708 : { _sntprintf( v, 256, _T(" Количество элементов ") ); } break;
#else
      case  IDS_ITEM_0708 : { _sntprintf( v, 256, _T(" Number of Elements ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0709 : { _sntprintf( v, 256, _T(" Геометрическая модель ") ); } break;
#else
      case  IDS_ITEM_0709 : { _sntprintf( v, 256, _T(" Geometric Model ") ); } break;
#endif

// \ru Атрибуты \en Attributes

  PROMPT_CASE_RU( IDS_ITEM_0729, " Атрибуты модели" );
  PROMPT_CASE_EN( IDS_ITEM_0729, " Model Attributes" );

  PROMPT_CASE_RU( IDS_ITEM_0730, " Поставщик атрибутов" );
  PROMPT_CASE_EN( IDS_ITEM_0730, " Attributes Provider" );

  PROMPT_CASE_RU( IDS_ITEM_0731, " Атрибуты объекта" );
  PROMPT_CASE_EN( IDS_ITEM_0731, " Object Attributes" );

  PROMPT_CASE_RU( IDS_ITEM_0732, " Атрибут" );
  PROMPT_CASE_EN( IDS_ITEM_0732, " Attribute" );

  PROMPT_CASE_RU( IDS_ITEM_0733, " Имя примитива" );
  PROMPT_CASE_EN( IDS_ITEM_0733, " Primitive Name" );

  PROMPT_CASE_RU( IDS_ITEM_0734, " Поведение атрибутов" );
  PROMPT_CASE_EN( IDS_ITEM_0734, " Attributes Behavior" );

  PROMPT_CASE_RU( IDS_ITEM_0751, " Механические характеристики" );
  PROMPT_CASE_EN( IDS_ITEM_0751, " Mechanical Properties" );

  PROMPT_CASE_RU( IDS_ITEM_0754, " Деформации" );
  PROMPT_CASE_EN( IDS_ITEM_0754, " Strains" );

  PROMPT_CASE_RU( IDS_ITEM_0761, " Исполнение (вариант реализации)" );
  PROMPT_CASE_EN( IDS_ITEM_0761, " Embodiment (variant of implementation)" );

  PROMPT_CASE_RU( IDS_ITEM_0762, " Количество линий" );
  PROMPT_CASE_EN( IDS_ITEM_0762, " Lines Count" );

  PROMPT_CASE_RU( IDS_ITEM_0763, " Плотность" );
  PROMPT_CASE_EN( IDS_ITEM_0763, " Density" );

  PROMPT_CASE_RU( IDS_ITEM_0764, " Цвет" );
  PROMPT_CASE_EN( IDS_ITEM_0764, " Color" );

  PROMPT_CASE_RU( IDS_ITEM_0765, " Толщина" );
  PROMPT_CASE_EN( IDS_ITEM_0765, " Thickness" );

  PROMPT_CASE_RU( IDS_ITEM_0766, " Стиль" );
  PROMPT_CASE_EN( IDS_ITEM_0766, " Style" );

  PROMPT_CASE_RU( IDS_ITEM_0767, " Визуальные свойства" );
  PROMPT_CASE_EN( IDS_ITEM_0767, " Visual Properties" );

  PROMPT_CASE_RU( IDS_ITEM_0768, " Идентификатор " );
  PROMPT_CASE_EN( IDS_ITEM_0768, " Identifier " );

  PROMPT_CASE_RU( IDS_ITEM_0769, " Селектированность " );
  PROMPT_CASE_EN( IDS_ITEM_0769, " Selectivity " );

  PROMPT_CASE_RU( IDS_ITEM_0770, " Видимость" );
  PROMPT_CASE_EN( IDS_ITEM_0770, " Visibility " );

  PROMPT_CASE_RU( IDS_ITEM_0771, " Измененность " );
  PROMPT_CASE_EN( IDS_ITEM_0771, " Modification " );

  PROMPT_CASE_RU( IDS_ITEM_0772, " Топологическое имя " );
  PROMPT_CASE_EN( IDS_ITEM_0772, " Topological Name " );

  PROMPT_CASE_RU( IDS_ITEM_0773, " Якорь" );
  PROMPT_CASE_EN( IDS_ITEM_0773, " Anchor" );

  PROMPT_CASE_RU( IDS_ITEM_0774, " Геометрический атрибут " );
  PROMPT_CASE_EN( IDS_ITEM_0774, " Geometric Attribute " );

  PROMPT_CASE_RU( IDS_ITEM_0775, " Метка времени обновления " );
  PROMPT_CASE_EN( IDS_ITEM_0775, " Label of Update Time " );

  PROMPT_CASE_RU( IDS_ITEM_0776, " Уникальность ключей " );
  PROMPT_CASE_EN( IDS_ITEM_0776, " Keys uniqueness " );

  PROMPT_CASE_RU( IDS_ITEM_0777, " Имя объекта в модели ");
  PROMPT_CASE_EN( IDS_ITEM_0777, " Name of object in the model ");

  PROMPT_CASE_RU( IDS_ITEM_0778, " Данные об изделии ");
  PROMPT_CASE_EN( IDS_ITEM_0778, " Product data ");

  PROMPT_CASE_RU( IDS_ITEM_0779, " Атрибут ребра жесткости листового тела ");
  PROMPT_CASE_EN( IDS_ITEM_0779, " Attribute of stamp rib of sheet Solid ");

  PROMPT_CASE_RU( IDS_ITEM_0781, " Атрибут Uuid" );
  PROMPT_CASE_EN( IDS_ITEM_0781, " Uuid Attribute" );

  PROMPT_CASE_RU( IDS_ITEM_0782, " Атрибут пользовательский ");
  PROMPT_CASE_EN( IDS_ITEM_0782, " Custom Attribute ");

  PROMPT_CASE_RU( IDS_ITEM_0783, " Атрибут обобщенный " );
  PROMPT_CASE_EN( IDS_ITEM_0783, " Generalized Attribute" );

  PROMPT_CASE_RU( IDS_ITEM_0784, " Атрибут булев " );
  PROMPT_CASE_EN( IDS_ITEM_0784, " Boolean Attribute ");

  PROMPT_CASE_RU( IDS_ITEM_0785, " Атрибут целочисленный ( 32-битный )" );
  PROMPT_CASE_EN( IDS_ITEM_0785, " Integer Attribute ( 32 bits )" );

  PROMPT_CASE_RU( IDS_ITEM_0786, " Атрибут действительный " );
  PROMPT_CASE_EN( IDS_ITEM_0786, " Real Attribute " );

  PROMPT_CASE_RU( IDS_ITEM_0787, " Атрибут строковый " );
  PROMPT_CASE_EN( IDS_ITEM_0787, " String Attribute " );

  PROMPT_CASE_RU( IDS_ITEM_0788, " Атрибут элементарный " );
  PROMPT_CASE_EN( IDS_ITEM_0788, " Elementary Attribute " );

  PROMPT_CASE_RU( IDS_ITEM_0789, " Подсказка" );
  PROMPT_CASE_EN( IDS_ITEM_0789, " Prompt" );

  PROMPT_CASE_RU( IDS_ITEM_0790, " Атрибут целочисленный ( 64-битный )" );
  PROMPT_CASE_EN( IDS_ITEM_0790, " Integer Attribute ( 64 bits )" );

  PROMPT_CASE_RU( IDS_ITEM_0791, " Атрибут бинарный" );
  PROMPT_CASE_EN( IDS_ITEM_0791, " Binary Attribute" );

  PROMPT_CASE_RU( IDS_ITEM_0792, " Атрибут массив целочисленных значений ( 32-битных )" );
  PROMPT_CASE_EN( IDS_ITEM_0792, " Array of Integers ( 32 bits ) Attribute" );

  PROMPT_CASE_RU( IDS_ITEM_0793, " Атрибут массив целочисленных значений ( 64-битных )" );
  PROMPT_CASE_EN( IDS_ITEM_0793, " Array of Integers ( 64 bits ) Attribute" );

  PROMPT_CASE_RU( IDS_ITEM_0794, " Атрибут массив действительных чисел типа double" );
  PROMPT_CASE_EN( IDS_ITEM_0794, " Array of Real ( double ) Values Attribute" );

  PROMPT_CASE_RU( IDS_ITEM_0796, " Атрибут диапазона действительных чисел типа double" );
  PROMPT_CASE_EN( IDS_ITEM_0796, " Double precision range attribute" );

  PROMPT_CASE_RU( IDS_ITEM_0797, " Атрибут ссылочного объекта " );
  PROMPT_CASE_EN( IDS_ITEM_0797, " Reference-counted object attribute " );


// \ru Сообщения \en Messages

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0900 : { _sntprintf( v, 256, _T("!   Ошибка   !") ); } break;
#else
      case  IDS_ITEM_0900 : { _sntprintf( v, 256, _T("!   Error   !") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0901 : { _sntprintf( v, 256, _T("  Остановлено ") ); } break;
#else
      case  IDS_ITEM_0901 : { _sntprintf( v, 256, _T("  Stopped ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_ITEM_0902 : { _sntprintf( v, 256, _T("   Пропущено  ") ); } break;
#else
      case  IDS_ITEM_0902 : { _sntprintf( v, 256, _T("   Missed  ") ); } break;
#endif

// \ru Состав объектов \en Structure of objects

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0000 : { _sntprintf( v, 256, _T(" ") ); } break;
#else
      case  IDS_PROP_0000 : { _sntprintf( v, 256, _T(" ") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0001 : { _sntprintf( v, 256, _T(" Кривая на плоскости ") ); } break;
#else
      case  IDS_PROP_0001 : { _sntprintf( v, 256, _T(" Curve on a plane ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0002 : { _sntprintf( v, 256, _T(" Параметр кривой ") ); } break;
#else
      case  IDS_PROP_0002 : { _sntprintf( v, 256, _T(" Parameter of a curve ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0003 : { _sntprintf( v, 256, _T(" Кривая 1 на плоскости ") ); } break;
#else
      case  IDS_PROP_0003 : { _sntprintf( v, 256, _T(" Curve 1 on the plane ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0004 : { _sntprintf( v, 256, _T(" Кривая 2 на плоскости ") ); } break;
#else
      case  IDS_PROP_0004 : { _sntprintf( v, 256, _T(" Curve 2 on the plane ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0005 : { _sntprintf( v, 256, _T(" Параметр кривой 1 ") ); } break;
#else
      case  IDS_PROP_0005 : { _sntprintf( v, 256, _T(" Parameter of curve 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0006 : { _sntprintf( v, 256, _T(" Параметр кривой 2 ") ); } break;
#else
      case  IDS_PROP_0006 : { _sntprintf( v, 256, _T(" Parameter of curve 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0007 : { _sntprintf( v, 256, _T(" Начальная точка ") ); } break;
#else
      case  IDS_PROP_0007 : { _sntprintf( v, 256, _T(" Start point ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0008 : { _sntprintf( v, 256, _T(" Направление ") ); } break;
#else
      case  IDS_PROP_0008 : { _sntprintf( v, 256, _T(" Direction ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0009 : { _sntprintf( v, 256, _T(" Конечная  точка ") ); } break;
#else
      case  IDS_PROP_0009 : { _sntprintf( v, 256, _T(" End point ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0014 : { _sntprintf( v, 256, _T(" Начальный параметр усечения ") ); } break;
#else
      case  IDS_PROP_0014 : { _sntprintf( v, 256, _T(" Start parameter of truncation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0015 : { _sntprintf( v, 256, _T(" Конечный  параметр усечения ") ); } break;
#else
      case  IDS_PROP_0015 : { _sntprintf( v, 256, _T(" End parameter of truncation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0016 : { _sntprintf( v, 256, _T(" Совпадение направления ") ); } break;
#else
      case  IDS_PROP_0016 : { _sntprintf( v, 256, _T(" Coincidence of direction ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0017 : { _sntprintf( v, 256, _T(" Циклическая частота ") ); } break;
#else
      case  IDS_PROP_0017 : { _sntprintf( v, 256, _T(" Cyclic frequency ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0018 : { _sntprintf( v, 256, _T(" Начальная фаза (град) ") ); } break;
#else
      case  IDS_PROP_0018 : { _sntprintf( v, 256, _T(" Initial phase (degrees) ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0019 : { _sntprintf( v, 256, _T(" Амплитуда ") ); } break;
#else
      case  IDS_PROP_0019 : { _sntprintf( v, 256, _T(" Amplitude ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0020 : { _sntprintf( v, 256, _T(" Перевернуть направление") ); } break;
#else
      case  IDS_PROP_0020 : { _sntprintf( v, 256, _T(" Reverse the direction") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0021 : { _sntprintf( v, 256, _T(" Неподвижная точка ") ); } break;
#else
      case  IDS_PROP_0021 : { _sntprintf( v, 256, _T(" Fixed point ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0022 : { _sntprintf( v, 256, _T(" Использовать точку ") ); } break;
#else
      case  IDS_PROP_0022 : { _sntprintf( v, 256, _T(" Use a point ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0023 : { _sntprintf( v, 256, _T(" Равномерное преобразование ") ); } break;
#else
      case  IDS_PROP_0023 : { _sntprintf( v, 256, _T(" Uniform transformation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0024 : { _sntprintf( v, 256, _T(" Точность " ) ); } break;
#else
      case  IDS_PROP_0024 : { _sntprintf( v, 256, _T(" Tolerance " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0025 : { _sntprintf( v, 256, _T(" Начальное значение " ) ); } break;
#else
      case  IDS_PROP_0025 : { _sntprintf( v, 256, _T(" Start value " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0026 : { _sntprintf( v, 256, _T(" Коэффициент усиления " ) ); } break;
#else
      case  IDS_PROP_0026 : { _sntprintf( v, 256, _T(" Scale gain " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0027 : { _sntprintf( v, 256, _T(" Амплитуда " ) ); } break;
#else
      case  IDS_PROP_0027 : { _sntprintf( v, 256, _T(" Amplitude " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0028 : { _sntprintf( v, 256, _T(" Сдвиг параметра " ) ); } break;
#else
      case  IDS_PROP_0028 : { _sntprintf( v, 256, _T(" Parameter shift " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0029 : { _sntprintf( v, 256, _T(" Степень возведения " ) ); } break;
#else
      case  IDS_PROP_0029 : { _sntprintf( v, 256, _T(" Exponent parameter " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0030 : { _sntprintf( v, 256, _T(" Циклическая частота " ) ); } break;
#else
      case  IDS_PROP_0030 : { _sntprintf( v, 256, _T(" Frequency " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0031 : { _sntprintf( v, 256, _T(" Эквидистантное смещение " ) ); } break;
#else
      case  IDS_PROP_0031 : { _sntprintf( v, 256, _T(" The offset range " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0032 : { _sntprintf( v, 256, _T(" Тип кривой " ) ); } break;
#else
      case  IDS_PROP_0032 : { _sntprintf( v, 256, _T(" Curve type " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0033 : { _sntprintf( v, 256, _T(" Прогиб " ) ); } break;
#else
      case  IDS_PROP_0033 : { _sntprintf( v, 256, _T(" Sag " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0034 : { _sntprintf( v, 256, _T(" Угловое отклонение " ) ); } break;
#else
      case  IDS_PROP_0034 : { _sntprintf( v, 256, _T(" Angular deviation " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0035 : { _sntprintf( v, 256, _T(" Расстояние " ) ); } break;
#else
      case  IDS_PROP_0035 : { _sntprintf( v, 256, _T(" Distance " ) ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0101 : { _sntprintf( v, 256, _T(" Координата X ") ); } break;
#else
      case  IDS_PROP_0101 : { _sntprintf( v, 256, _T(" Coordinate X ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0102 : { _sntprintf( v, 256, _T(" Координата Y ") ); } break;
#else
      case  IDS_PROP_0102 : { _sntprintf( v, 256, _T(" Coordinate Y ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0103 : { _sntprintf( v, 256, _T(" Координата Z ") ); } break;
#else
      case  IDS_PROP_0103 : { _sntprintf( v, 256, _T(" Coordinate Z ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0104 : { _sntprintf( v, 256, _T(" Матрица ") ); } break;
#else
      case  IDS_PROP_0104 : { _sntprintf( v, 256, _T(" Matrix ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0107 : { _sntprintf( v, 256, _T(" Угол с осью X ") ); } break;
#else
      case  IDS_PROP_0107 : { _sntprintf( v, 256, _T(" Angle with axis X ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0108 : { _sntprintf( v, 256, _T(" Угол с осью Y ") ); } break;
#else
      case  IDS_PROP_0108 : { _sntprintf( v, 256, _T(" Angle with axis Y ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0109 : { _sntprintf( v, 256, _T(" Угол с осью Z ") ); } break;
#else
      case  IDS_PROP_0109 : { _sntprintf( v, 256, _T(" Angle with axis Z ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0110 : { _sntprintf( v, 256, _T(" Точка ") ); } break;
#else
      case  IDS_PROP_0110 : { _sntprintf( v, 256, _T(" Point ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0111 : { _sntprintf( v, 256, _T(" Компонента 1.X ") ); } break;
#else
      case  IDS_PROP_0111 : { _sntprintf( v, 256, _T(" Component 1.X ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0112 : { _sntprintf( v, 256, _T(" Компонента 1.Y ") ); } break;
#else
      case  IDS_PROP_0112 : { _sntprintf( v, 256, _T(" Component 1.Y ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0113 : { _sntprintf( v, 256, _T(" Компонента 1.Z ") ); } break;
#else
      case  IDS_PROP_0113 : { _sntprintf( v, 256, _T(" Component 1.Z ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0114 : { _sntprintf( v, 256, _T(" Компонента 2.X ") ); } break;
#else
      case  IDS_PROP_0114 : { _sntprintf( v, 256, _T(" Component 2.X ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0115 : { _sntprintf( v, 256, _T(" Компонента 2.Y ") ); } break;
#else
      case  IDS_PROP_0115 : { _sntprintf( v, 256, _T(" Component 2.Y ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0116 : { _sntprintf( v, 256, _T(" Компонента 2.Z ") ); } break;
#else
      case  IDS_PROP_0116 : { _sntprintf( v, 256, _T(" Component 2.Z ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0117 : { _sntprintf( v, 256, _T(" Компонента 3.X ") ); } break;
#else
      case  IDS_PROP_0117 : { _sntprintf( v, 256, _T(" Component 3.X ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0118 : { _sntprintf( v, 256, _T(" Компонента 3.Y ") ); } break;
#else
      case  IDS_PROP_0118 : { _sntprintf( v, 256, _T(" Component 3.Y ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0119 : { _sntprintf( v, 256, _T(" Компонента 3.Z ") ); } break;
#else
      case  IDS_PROP_0119 : { _sntprintf( v, 256, _T(" Component 3.Z ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0120 : { _sntprintf( v, 256, _T(" Вектор ") ); } break;
#else
      case  IDS_PROP_0120 : { _sntprintf( v, 256, _T(" Vector ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0121 : { _sntprintf( v, 256, _T(" Сдвиг по X ") ); } break;
#else
      case  IDS_PROP_0121 : { _sntprintf( v, 256, _T(" Shift by X ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0122 : { _sntprintf( v, 256, _T(" Сдвиг по Y ") ); } break;
#else
      case  IDS_PROP_0122 : { _sntprintf( v, 256, _T(" Shift by Y ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0123 : { _sntprintf( v, 256, _T(" Сдвиг по Z ") ); } break;
#else
      case  IDS_PROP_0123 : { _sntprintf( v, 256, _T(" Shift by Z ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0124 : { _sntprintf( v, 256, _T(" Ось X ") ); } break;
#else
      case  IDS_PROP_0124 : { _sntprintf( v, 256, _T(" Axis X ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0125 : { _sntprintf( v, 256, _T(" Ось Y ") ); } break;
#else
      case  IDS_PROP_0125 : { _sntprintf( v, 256, _T(" Axis Y ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0126 : { _sntprintf( v, 256, _T(" Ось Z ") ); } break;
#else
      case  IDS_PROP_0126 : { _sntprintf( v, 256, _T(" Axis Z ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0127 : { _sntprintf( v, 256, _T(" Длина оси X ") ); } break;
#else
      case  IDS_PROP_0127 : { _sntprintf( v, 256, _T(" Length of axis X ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0128 : { _sntprintf( v, 256, _T(" Длина оси Y ") ); } break;
#else
      case  IDS_PROP_0128 : { _sntprintf( v, 256, _T(" Length of axis Y ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0129 : { _sntprintf( v, 256, _T(" Длина оси Z ") ); } break;
#else
      case  IDS_PROP_0129 : { _sntprintf( v, 256, _T(" Length of axis Z ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0130 : { _sntprintf( v, 256, _T(" Центр ") ); } break;
#else
      case  IDS_PROP_0130 : { _sntprintf( v, 256, _T(" Center ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0131 : { _sntprintf( v, 256, _T(" Левая система ") ); } break;
#else
      case  IDS_PROP_0131 : { _sntprintf( v, 256, _T(" Left system ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0132 : { _sntprintf( v, 256, _T(" Прямоугольная ") ); } break;
#else
      case  IDS_PROP_0132 : { _sntprintf( v, 256, _T(" Rectangular ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0133 : { _sntprintf( v, 256, _T(" Базовая точка") ); } break;
#else
      case  IDS_PROP_0133 : { _sntprintf( v, 256, _T(" Base point") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0134 : { _sntprintf( v, 256, _T(" Строка ") ); } break;
#else
      case  IDS_PROP_0134 : { _sntprintf( v, 256, _T(" String ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0135 : { _sntprintf( v, 256, _T(" Шрифт  ") ); } break;
#else
      case  IDS_PROP_0135 : { _sntprintf( v, 256, _T(" Font  ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0136 : { _sntprintf( v, 256, _T(" Позиция начала ") ); } break;
#else
      case  IDS_PROP_0136 : { _sntprintf( v, 256, _T(" Start position ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0137 : { _sntprintf( v, 256, _T(" Высота ") ); } break;
#else
      case  IDS_PROP_0137 : { _sntprintf( v, 256, _T(" Height ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0138 : { _sntprintf( v, 256, _T(" Сужение ") ); } break;
#else
      case  IDS_PROP_0138 : { _sntprintf( v, 256, _T(" Taper ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0139 : { _sntprintf( v, 256, _T(" Угол наклона ") ); } break;
#else
      case  IDS_PROP_0139 : { _sntprintf( v, 256, _T(" Slope angle ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0140 : { _sntprintf( v, 256, _T(" Радиус ") ); } break;
#else
      case  IDS_PROP_0140 : { _sntprintf( v, 256, _T(" Radius ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0141 : { _sntprintf( v, 256, _T(" Масштаб по X") ); } break;
#else
      case  IDS_PROP_0141 : { _sntprintf( v, 256, _T(" Scale by X") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0142 : { _sntprintf( v, 256, _T(" Масштаб по Y") ); } break;
#else
      case  IDS_PROP_0142 : { _sntprintf( v, 256, _T(" Scale by Y") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0143 : { _sntprintf( v, 256, _T(" Масштаб по Z") ); } break;
#else
      case  IDS_PROP_0143 : { _sntprintf( v, 256, _T(" Scale by Z") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0144 : { _sntprintf( v, 256, _T(" Функция масштабирования") ); } break;
#else
      case  IDS_PROP_0144 : { _sntprintf( v, 256, _T(" The function of scaling") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0145 : { _sntprintf( v, 256, _T(" Функция вращения") ); } break;
#else
      case  IDS_PROP_0145 : { _sntprintf( v, 256, _T(" The function of rotation") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0146 : { _sntprintf( v, 256, _T(" Толерантность") ); } break;
#else
      case  IDS_PROP_0146 : { _sntprintf( v, 256, _T(" The tolerance") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0150 : { _sntprintf( v, 256, _T(" Угол   ") ); } break;
#else
      case  IDS_PROP_0150 : { _sntprintf( v, 256, _T(" Angle   ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0151 : { _sntprintf( v, 256, _T(" Шаг    ") ); } break;
#else
      case  IDS_PROP_0151 : { _sntprintf( v, 256, _T(" Step    ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0152 : { _sntprintf( v, 256, _T(" Смещение зазора") ); } break;
#else
      case  IDS_PROP_0152 : { _sntprintf( v, 256, _T(" Gap displacement") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0153 : { _sntprintf( v, 256, _T(" Перемещение ") ); } break;
#else
      case  IDS_PROP_0153 : { _sntprintf( v, 256, _T(" Translation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0154 : { _sntprintf( v, 256, _T(" Вращение ") ); } break;
#else
      case  IDS_PROP_0154 : { _sntprintf( v, 256, _T(" Rotation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0155 : { _sntprintf( v, 256, _T(" Масштабирование ") ); } break;
#else
      case  IDS_PROP_0155 : { _sntprintf( v, 256, _T(" Scaling ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0156 : { _sntprintf( v, 256, _T(" Зеркальность ") ); } break;
#else
      case  IDS_PROP_0156 : { _sntprintf( v, 256, _T(" Specularity ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0157 : { _sntprintf( v, 256, _T(" Только ортогональность ") ); } break;
#else
      case  IDS_PROP_0157 : { _sntprintf( v, 256, _T(" Orthogonality only ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0158 : { _sntprintf( v, 256, _T(" Трансформация ") ); } break;
#else
      case  IDS_PROP_0158 : { _sntprintf( v, 256, _T(" Transformation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0159 : { _sntprintf( v, 256, _T(" Перспектива ") ); } break;
#else
      case  IDS_PROP_0159 : { _sntprintf( v, 256, _T(" Perspective ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0160 : { _sntprintf( v, 256, _T(" Локальная система координат ") ); } break;
#else
      case  IDS_PROP_0160 : { _sntprintf( v, 256, _T(" Local coordinate system ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0161 : { _sntprintf( v, 256, _T(" Начальное значение ") ); } break;
#else
      case  IDS_PROP_0161 : { _sntprintf( v, 256, _T(" Start value ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0162 : { _sntprintf( v, 256, _T(" Конечное  значение ") ); } break;
#else
      case  IDS_PROP_0162 : { _sntprintf( v, 256, _T(" End value ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0163 : { _sntprintf( v, 256, _T(" Значение ") ); } break;
#else
      case  IDS_PROP_0163 : { _sntprintf( v, 256, _T(" Value ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0164 : { _sntprintf( v, 256, _T(" Функция изменения радиусов ") ); } break;
#else
      case  IDS_PROP_0164 : { _sntprintf( v, 256, _T(" Radius function ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0165 : { _sntprintf( v, 256, _T(" Функция изменения веса ") ); } break;
#else
      case  IDS_PROP_0165 : { _sntprintf( v, 256, _T(" Weight function ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0166 : { _sntprintf( v, 256, _T(" Функция ") ); } break;
#else
      case  IDS_PROP_0166 : { _sntprintf( v, 256, _T(" Function ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0167 : { _sntprintf( v, 256, _T(" Минимум ") ); } break;
#else
      case  IDS_PROP_0167 : { _sntprintf( v, 256, _T(" Minimum ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0168 : { _sntprintf( v, 256, _T(" Максимум ") ); } break;
#else
      case  IDS_PROP_0168 : { _sntprintf( v, 256, _T(" Maximum ") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0169 : { _sntprintf( v, 256, _T(" Продлевать вверх? ") ); } break;
#else
      case  IDS_PROP_0169 : { _sntprintf( v, 256, _T(" Extend upwards? ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0170 : { _sntprintf( v, 256, _T(" Поворт оси вокруг нормали ") ); } break;
#else
      case  IDS_PROP_0170 : { _sntprintf( v, 256, _T(" Rotation of axis about the normal ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0171 : { _sntprintf( v, 256, _T(" Угол между осью и нормалью ") ); } break;
#else
      case  IDS_PROP_0171 : { _sntprintf( v, 256, _T(" Angle between the axis and the normal ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0172 : { _sntprintf( v, 256, _T(" Диаметр головки ") ); } break;
#else
      case  IDS_PROP_0172 : { _sntprintf( v, 256, _T(" Cap diameter ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0173 : { _sntprintf( v, 256, _T(" Глубина под головку ") ); } break;
#else
      case  IDS_PROP_0173 : { _sntprintf( v, 256, _T(" Depth for a cap ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0174 : { _sntprintf( v, 256, _T(" Угол фаски под головку ") ); } break;
#else
      case  IDS_PROP_0174 : { _sntprintf( v, 256, _T(" Chamfer angle for a cap ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0175 : { _sntprintf( v, 256, _T(" Диаметр отверстия под резьбу ") ); } break;
#else
      case  IDS_PROP_0175 : { _sntprintf( v, 256, _T(" Diameter of hole for a cap ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0176 : { _sntprintf( v, 256, _T(" Глубина отверстия под резьбу ") ); } break;
#else
      case  IDS_PROP_0176 : { _sntprintf( v, 256, _T(" Depth of hole for a cap ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0177 : { _sntprintf( v, 256, _T(" Угол конусности отверстия ") ); } break;
#else
      case  IDS_PROP_0177 : { _sntprintf( v, 256, _T(" Taper angle of hole ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0178 : { _sntprintf( v, 256, _T(" Угол раствора конца отверстия ") ); } break;
#else
      case  IDS_PROP_0178 : { _sntprintf( v, 256, _T(" Apical angle of hole end") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0179 : { _sntprintf( v, 256, _T(" Тип отверстия ") ); } break;
#else
      case  IDS_PROP_0179 : { _sntprintf( v, 256, _T(" Hole type ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0180 : { _sntprintf( v, 256, _T(" Способ модификации ") ); } break;
#else
      case  IDS_PROP_0180 : { _sntprintf( v, 256, _T(" Method of modification ") ); } break;
#endif

// \ru Карман/бобышка \en Pocket/boss

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0181  : { _sntprintf( v, 256, _T(" Тип(true-бобышка, false- карман)") ); } break;
#else
      case  IDS_PROP_0181  : { _sntprintf( v, 256, _T(" Type(true-boss, false- pocket)") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0182  : { _sntprintf( v, 256, _T(" Тип") ); } break;
#else
      case  IDS_PROP_0182  : { _sntprintf( v, 256, _T(" Type") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0183  : { _sntprintf( v, 256, _T(" Ширина" ) ); } break;
#else
      case  IDS_PROP_0183  : { _sntprintf( v, 256, _T(" Width" ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0184  : { _sntprintf( v, 256, _T(" Ширина 2" ) ); } break;
#else
      case  IDS_PROP_0184  : { _sntprintf( v, 256, _T(" Width 2" ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0185  : { _sntprintf( v, 256, _T(" Способ") ); } break;
#else
      case  IDS_PROP_0185  : { _sntprintf( v, 256, _T(" Method") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0186  : { _sntprintf( v, 256, _T(" Новая грань") ); } break;
#else
      case  IDS_PROP_0186  : { _sntprintf( v, 256, _T(" New face") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0188 : { _sntprintf( v, 256, _T(" Направление (вниз/вверх) ") ); } break;
#else
      case  IDS_PROP_0188 : { _sntprintf( v, 256, _T(" Direction (down/up) ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0189 : { _sntprintf( v, 256, _T(" Радиус дуги ") ); } break;
#else
      case  IDS_PROP_0189 : { _sntprintf( v, 256, _T(" Arc radius ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0190 : { _sntprintf( v, 256, _T(" Дискриминантная функция ") ); } break;
#else
      case  IDS_PROP_0190 : { _sntprintf( v, 256, _T(" Discriminant function ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0191 : { _sntprintf( v, 256, _T(" Дискриминантная кривая ") ); } break;
#else
      case  IDS_PROP_0191 : { _sntprintf( v, 256, _T(" Discriminant curve ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0192 : { _sntprintf( v, 256, _T(" Дискриминантная поверхность ") ); } break;
#else
      case  IDS_PROP_0192 : { _sntprintf( v, 256, _T(" Discriminant surface ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0193 : { _sntprintf( v, 256, _T(" Дискриминантная оболочка ") ); } break;
#else
      case  IDS_PROP_0193 : { _sntprintf( v, 256, _T(" Discriminant shell ") ); } break;
#endif

// \ru Параметры \en Parameters

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0203 : { _sntprintf( v, 256, _T(" Кривая 1 ") ); } break;
#else
      case  IDS_PROP_0203 : { _sntprintf( v, 256, _T(" Curve 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0204 : { _sntprintf( v, 256, _T(" Кривая 2 ") ); } break;
#else
      case  IDS_PROP_0204 : { _sntprintf( v, 256, _T(" Curve 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0205 : { _sntprintf( v, 256, _T(" Кривая 3 ") ); } break;
#else
      case  IDS_PROP_0205 : { _sntprintf( v, 256, _T(" Curve 3 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0206 : { _sntprintf( v, 256, _T(" Кривая 4 ") ); } break;
#else
      case  IDS_PROP_0206 : { _sntprintf( v, 256, _T(" Curve 4 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0207 : { _sntprintf( v, 256, _T(" Начальная точка ") ); } break;
#else
      case  IDS_PROP_0207 : { _sntprintf( v, 256, _T(" Start point ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0208 : { _sntprintf( v, 256, _T(" Направляющий вектор ") ); } break;
#else
      case  IDS_PROP_0208 : { _sntprintf( v, 256, _T(" Direction vector ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0209 : { _sntprintf( v, 256, _T(" Конечная точка ") ); } break;
#else
      case  IDS_PROP_0209 : { _sntprintf( v, 256, _T(" End point ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0210 : { _sntprintf( v, 256, _T(" Нормаль к плоскости") ); } break;
#else
      case  IDS_PROP_0210 : { _sntprintf( v, 256, _T(" Normal to Surface") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0211 : { _sntprintf( v, 256, _T(" Первая полуось ") ); } break;
#else
      case  IDS_PROP_0211 : { _sntprintf( v, 256, _T(" First semiaxis ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0212 : { _sntprintf( v, 256, _T(" Вторая полуось ") ); } break;
#else
      case  IDS_PROP_0212 : { _sntprintf( v, 256, _T(" Second semiaxis ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0213 : { _sntprintf( v, 256, _T(" Фокусное расстояние") ); } break;
#else
      case  IDS_PROP_0213 : { _sntprintf( v, 256, _T(" Focal distance") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0214 : { _sntprintf( v, 256, _T(" Параметр min ") ); } break;
#else
      case  IDS_PROP_0214 : { _sntprintf( v, 256, _T(" Parameter min ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0215 : { _sntprintf( v, 256, _T(" Параметр max ") ); } break;
#else
      case  IDS_PROP_0215 : { _sntprintf( v, 256, _T(" Parameter max ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0216 : { _sntprintf( v, 256, _T(" Действительная полуось") ); } break;
#else
      case  IDS_PROP_0216 : { _sntprintf( v, 256, _T(" Real semiaxis") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0217 : { _sntprintf( v, 256, _T(" Мнимая полуось") ); } break;
#else
      case  IDS_PROP_0217 : { _sntprintf( v, 256, _T(" Imaginary semiaxis") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0218 : { _sntprintf( v, 256, _T(" Приращение начального параметра ") ); } break;
#else
      case  IDS_PROP_0218 : { _sntprintf( v, 256, _T(" Increment of start parameter ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0219 : { _sntprintf( v, 256, _T(" Приращение конечного параметра ") ); } break;
#else
      case  IDS_PROP_0219 : { _sntprintf( v, 256, _T(" Increment of end parameter ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0220 : { _sntprintf( v, 256, _T(" Замкнутость ") ); } break;
#else
      case  IDS_PROP_0220 : { _sntprintf( v, 256, _T(" Closedness ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0221 : { _sntprintf( v, 256, _T(" Порядок ") ); } break;
#else
      case  IDS_PROP_0221 : { _sntprintf( v, 256, _T(" Order ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0222 : { _sntprintf( v, 256, _T(" Количество точек ") ); } break;
#else
      case  IDS_PROP_0222 : { _sntprintf( v, 256, _T(" Number of points ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0223 : { _sntprintf( v, 256, _T(" Точка ") ); } break;
#else
      case  IDS_PROP_0223 : { _sntprintf( v, 256, _T(" Point ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0224 : { _sntprintf( v, 256, _T(" Начальный параметр ") ); } break;
#else
      case  IDS_PROP_0224 : { _sntprintf( v, 256, _T(" Start parameter ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0225 : { _sntprintf( v, 256, _T(" Производная параметра ") ); } break;
#else
      case  IDS_PROP_0225 : { _sntprintf( v, 256, _T(" Derivative of parameter ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0226 : { _sntprintf( v, 256, _T(" Параметрическая длина ") ); } break;
#else
      case  IDS_PROP_0226 : { _sntprintf( v, 256, _T(" Parametric length ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0227 : { _sntprintf( v, 256, _T(" Положительное направление  ") ); } break;
#else
      case  IDS_PROP_0227 : { _sntprintf( v, 256, _T(" Positive direction  ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0228 : { _sntprintf( v, 256, _T(" Вес ") ); } break;
#else
      case  IDS_PROP_0228 : { _sntprintf( v, 256, _T(" Weight ") ); } break;
#endif
        /*
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0229 : { _sntprintf( v, 256, _T(" Касание ") ); } break;
#else
      case  IDS_PROP_0229 : { _sntprintf( v, 256, _T(" Tangency ") ); } break;
#endif
*/
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0230 : { _sntprintf( v, 256, _T(" Расстояние ") ); } break;
#else
      case  IDS_PROP_0230 : { _sntprintf( v, 256, _T(" Distance ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0232 : { _sntprintf( v, 256, _T(" Число кривых контура ") ); } break;
#else
      case  IDS_PROP_0232 : { _sntprintf( v, 256, _T(" Number of curves of contour ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0233 : { _sntprintf( v, 256, _T(" Кривая ") ); } break;
#else
      case  IDS_PROP_0233 : { _sntprintf( v, 256, _T(" Curve ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0234 : { _sntprintf( v, 256, _T(" Начальный параметр кривой ") ); } break;
#else
      case  IDS_PROP_0234 : { _sntprintf( v, 256, _T(" Start parameter of a curve ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0235 : { _sntprintf( v, 256, _T(" Конечный параметр кривой ") ); } break;
#else
      case  IDS_PROP_0235 : { _sntprintf( v, 256, _T(" End parameter of a curve ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0236 : { _sntprintf( v, 256, _T(" Число узлов ") ); } break;
#else
      case  IDS_PROP_0236 : { _sntprintf( v, 256, _T(" Number of knots ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0237 : { _sntprintf( v, 256, _T(" Значение узла ") ); } break;
#else
      case  IDS_PROP_0237 : { _sntprintf( v, 256, _T(" Knot value ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0238 : { _sntprintf( v, 256, _T(" Вторая производная в точке ") ); } break;
#else
      case  IDS_PROP_0238 : { _sntprintf( v, 256, _T(" Second derivative at a point ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0239 : { _sntprintf( v, 256, _T(" Производная ") ); } break;
#else
      case  IDS_PROP_0239 : { _sntprintf( v, 256, _T(" Derivative ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0240 : { _sntprintf( v, 256, _T(" Опорная кривая ") ); } break;
#else
      case  IDS_PROP_0240 : { _sntprintf( v, 256, _T(" Reference curve ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0241 : { _sntprintf( v, 256, _T(" Зависимость по X ") ); } break;
#else
      case  IDS_PROP_0241 : { _sntprintf( v, 256, _T(" Dependency by X ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0242 : { _sntprintf( v, 256, _T(" Зависимость по Y ") ); } break;
#else
      case  IDS_PROP_0242 : { _sntprintf( v, 256, _T(" Dependency by Y ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0243 : { _sntprintf( v, 256, _T(" Зависимость по Z ") ); } break;
#else
      case  IDS_PROP_0243 : { _sntprintf( v, 256, _T(" Dependency by Z ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0244 : { _sntprintf( v, 256, _T(" Расширение minPar") ); } break;
#else
      case  IDS_PROP_0244 : { _sntprintf( v, 256, _T(" Extension minPar") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0245 : { _sntprintf( v, 256, _T(" Расширение maxPar") ); } break;
#else
      case  IDS_PROP_0245 : { _sntprintf( v, 256, _T(" Extension maxPar") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0246 : { _sntprintf( v, 256, _T(" Количество сплайнов ") ); } break;
#else
      case  IDS_PROP_0246 : { _sntprintf( v, 256, _T(" Number of splines ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0250 : { _sntprintf( v, 256, _T(" Базовая кривая ") ); } break;
#else
      case  IDS_PROP_0250 : { _sntprintf( v, 256, _T(" Base curve ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0260 : { _sntprintf( v, 256, _T(" Двумерная кривая ") ); } break;
#else
      case  IDS_PROP_0260 : { _sntprintf( v, 256, _T(" Two-dimensional curve ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0263 : { _sntprintf( v, 256, _T(" Угол между OX плоскости и прямой касания ее с конусом ") ); } break;
#else
      case  IDS_PROP_0263 : { _sntprintf( v, 256, _T(" Angle of OX axis of a plane and the line of its tangency with a cone ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0264 : { _sntprintf( v, 256, _T(" Расстояние от листовой грани до нейтрального слоя") ); } break;
#else
      case  IDS_PROP_0264 : { _sntprintf( v, 256, _T(" Distance from a sheet face to the neutral layer") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0265 : { _sntprintf( v, 256, _T(" Угол образующей конуса, касательной к плоскости при разгибе") ); } break;
#else
      case  IDS_PROP_0265 : { _sntprintf( v, 256, _T(" Angle of a cone generatrix tangent to the plane while unbending") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0266 : { _sntprintf( v, 256, _T(" Количество образующих кривых ") ); } break;
#else
      case  IDS_PROP_0266 : { _sntprintf( v, 256, _T(" Number of generating lines ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0267 : { _sntprintf( v, 256, _T(" Объект модифицирован ") ); } break;
#else
      case  IDS_PROP_0267 : { _sntprintf( v, 256, _T(" Object is modified ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0268 : { _sntprintf( v, 256, _T(" Количество нормалей ") ); } break;
#else
      case  IDS_PROP_0268 : { _sntprintf( v, 256, _T(" Number of normals ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0269 : { _sntprintf( v, 256, _T(" Нормаль ") ); } break;
#else
      case  IDS_PROP_0269 : { _sntprintf( v, 256, _T(" Normal ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0270 : { _sntprintf( v, 256, _T(" Аппроксимационная кривая ") ); } break;
#else
      case  IDS_PROP_0270 : { _sntprintf( v, 256, _T(" Approximation curve ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0271 : { _sntprintf( v, 256, _T(" Удаление выбранных граней ") ); } break;
#else
      case  IDS_PROP_0271 : { _sntprintf( v, 256, _T(" Remove selected faces ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0272 : { _sntprintf( v, 256, _T(" Создание тела из выбранных граней ") ); } break;
#else
      case  IDS_PROP_0272 : { _sntprintf( v, 256, _T(" Solid creation by selected faces ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0273 : { _sntprintf( v, 256, _T(" Перемещение выбранных граней ") ); } break;
#else
      case  IDS_PROP_0273 : { _sntprintf( v, 256, _T(" Move selected faces ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0274 : { _sntprintf( v, 256, _T(" Эквидистантное смещение выбранных граней ") ); } break;
#else
      case  IDS_PROP_0274 : { _sntprintf( v, 256, _T(" Offset selected faces ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0275 : { _sntprintf( v, 256, _T(" Изменение радиусов выбранных скруглений ") ); } break;
#else
      case  IDS_PROP_0275 : { _sntprintf( v, 256, _T(" Change selected fillets ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0276 : { _sntprintf( v, 256, _T(" Замена выбранных граней деформируемыми ") ); } break;
#else
      case  IDS_PROP_0276 : { _sntprintf( v, 256, _T(" Replace selected faces by deformed ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0277 : { _sntprintf( v, 256, _T(" Удаление выбранных скруглений ") ); } break;
#else
      case  IDS_PROP_0277 : { _sntprintf( v, 256, _T(" Remove selected features ") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0278 : { _sntprintf( v, 256, _T(" Слияние вершин выбранных ребер ") ); } break;
#else
      case  IDS_PROP_0278 : { _sntprintf( v, 256, _T(" Merging vertices of selected edges ") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0279: { _sntprintf( v, 256, _T( " Замена гладко стыкующихся граней одной гранью " ) ); } break;
#else
      case  IDS_PROP_0279: { _sntprintf( v, 256, _T( " Replacing smoothly joined faces with one face " ) ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0280: { _sntprintf( v, 256, _T( " Поворот выбранный граней " ) ); } break;
#else
      case  IDS_PROP_0280: { _sntprintf( v, 256, _T( " Faces rotation " ) ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0281: { _sntprintf( v, 256, _T( " Замена выбранных граней " ) ); } break;
#else
      case  IDS_PROP_0281: { _sntprintf( v, 256, _T( " Faces replacement " ) ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0282 : { _sntprintf( v, 256, _T(" Вектор модификации ") ); } break;
#else
      case  IDS_PROP_0282 : { _sntprintf( v, 256, _T(" Vector of modification ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0283 : { _sntprintf( v, 256, _T(" Количество модифицированных граней ") ); } break;
#else
      case  IDS_PROP_0283 : { _sntprintf( v, 256, _T(" Number of modified faces ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0284 : { _sntprintf( v, 256, _T(" Положение срединной оболочки относительно образующих граней ") ); } break;
#else
      case  IDS_PROP_0284 : { _sntprintf( v, 256, _T(" Position of median shell in reference to parent faces ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0285 : { _sntprintf( v, 256, _T(" Минимальное отстояние между гранями ") ); } break;
#else
      case  IDS_PROP_0285 : { _sntprintf( v, 256, _T(" Minimal distance between faces ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0286 : { _sntprintf( v, 256, _T(" Максимальное отстояние между гранями ") ); } break;
#else
      case  IDS_PROP_0286 : { _sntprintf( v, 256, _T(" Maximal distance between faces ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0290: { _sntprintf( v, 256, _T(" Изменение размера цилиндрической или сферической грани ") ); } break;
#else
      case  IDS_PROP_0290: { _sntprintf( v, 256, _T(" Cylinder or sphere radius change ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0291: { _sntprintf( v, 256, _T(" Замена группы граней ") ); } break;
#else
      case  IDS_PROP_0291: { _sntprintf( v, 256, _T(" Replacement group of faces ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0292 : { _sntprintf( v, 256, _T(" Удаление скруглений меньших радиуса ") ); } break;
#else
      case  IDS_PROP_0292 : { _sntprintf( v, 256, _T(" Remove fillets less then radius ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0293 : { _sntprintf( v, 256, _T(" Удаление замкнутых граней вращения меньших радиуса ") ); } break;
#else
      case  IDS_PROP_0293 : { _sntprintf( v, 256, _T(" Remove the closed faces of revolution less then radius ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0294 : { _sntprintf( v, 256, _T(" Удаление замкнутых граней вращения меньших радиуса c прилегающими скруглениями ") ); } break;
#else
      case  IDS_PROP_0294 : { _sntprintf( v, 256, _T(" Remove the closed faces of revolution less then radius with adjacent fillets") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0295 : { _sntprintf( v, 256, _T(" Удаление выбранных граней c прилегающими скруглениями ") ); } break;
#else
      case  IDS_PROP_0295 : { _sntprintf( v, 256, _T(" Remove selected faces with adjacent fillets") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0296 : { _sntprintf( v, 256, _T(" Изменение радиусов скруглений на заданную величину ") ); } break;
#else
      case  IDS_PROP_0296 : { _sntprintf( v, 256, _T(" Change fillet radius by value") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0297 : { _sntprintf( v, 256, _T(" Установка радиуса для цепочки скруглений ") ); } break;
#else
      case  IDS_PROP_0297 : { _sntprintf( v, 256, _T(" Set radius for fillets chain ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0298 : { _sntprintf( v, 256, _T(" Удаление цепочек скруглений ") ); } break;
#else
      case  IDS_PROP_0298 : { _sntprintf( v, 256, _T(" Fillets chain removal ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0299 : { _sntprintf( v, 256, _T(" Поворот выбранных граней без адаптации скруглений ") ); } break;
#else
      case  IDS_PROP_0299 : { _sntprintf( v, 256, _T(" Face rotation without fillet adaptation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0300 : { _sntprintf( v, 256, _T(" Перемещение выбранных граней без адаптации скруглений ") ); } break;
#else
      case  IDS_PROP_0300 : { _sntprintf( v, 256, _T(" Move faces without fillet adaptation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0301 : { _sntprintf( v, 256, _T(" Начальный параметр U ") ); } break;
#else
      case  IDS_PROP_0301 : { _sntprintf( v, 256, _T(" Start parameter U ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0302 : { _sntprintf( v, 256, _T(" Начальный параметр V ") ); } break;
#else
      case  IDS_PROP_0302 : { _sntprintf( v, 256, _T(" Start parameter V ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0303 : { _sntprintf( v, 256, _T(" Положительное направление U ") ); } break;
#else
      case  IDS_PROP_0303 : { _sntprintf( v, 256, _T(" Positive direction by U ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0304 : { _sntprintf( v, 256, _T(" Положительное направление V ") ); } break;
#else
      case  IDS_PROP_0304 : { _sntprintf( v, 256, _T(" Positive direction by V ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0305 : { _sntprintf( v, 256, _T(" Параметрическая длина U ") ); } break;
#else
      case  IDS_PROP_0305 : { _sntprintf( v, 256, _T(" Parametric length by U ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0306 : { _sntprintf( v, 256, _T(" Параметрическая длина V ") ); } break;
#else
      case  IDS_PROP_0306 : { _sntprintf( v, 256, _T(" Parametric length by V ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0307 : { _sntprintf( v, 256, _T(" Положительное направление 1 ") ); } break;
#else
      case  IDS_PROP_0307 : { _sntprintf( v, 256, _T(" Positive direction 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0308 : { _sntprintf( v, 256, _T(" Положительное направление 2 ") ); } break;
#else
      case  IDS_PROP_0308 : { _sntprintf( v, 256, _T(" Positive direction 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0309 : { _sntprintf( v, 256, _T(" Тип сопряжения") ); } break;
#else
      case  IDS_PROP_0309 : { _sntprintf( v, 256, _T(" Conjugation type") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0310 : { _sntprintf( v, 256, _T(" Точка ") ); } break;
#else
      case  IDS_PROP_0310 : { _sntprintf( v, 256, _T(" Point ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0311 : { _sntprintf( v, 256, _T(" Замкнутость по U ") ); } break;
#else
      case  IDS_PROP_0311 : { _sntprintf( v, 256, _T(" Closedness by U ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0312 : { _sntprintf( v, 256, _T(" Замкнутость по V ") ); } break;
#else
      case  IDS_PROP_0312 : { _sntprintf( v, 256, _T(" Closedness by V ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0313 : { _sntprintf( v, 256, _T(" Число порций по U ") ); } break;
#else
      case  IDS_PROP_0313 : { _sntprintf( v, 256, _T(" Number of portions by U ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0314 : { _sntprintf( v, 256, _T(" Число порций по V ") ); } break;
#else
      case  IDS_PROP_0314 : { _sntprintf( v, 256, _T(" Number of portions by V ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0315 : { _sntprintf( v, 256, _T(" Порядок по U ") ); } break;
#else
      case  IDS_PROP_0315 : { _sntprintf( v, 256, _T(" Order by U ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0316 : { _sntprintf( v, 256, _T(" Порядок по V ") ); } break;
#else
      case  IDS_PROP_0316 : { _sntprintf( v, 256, _T(" Order by V ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0317 : { _sntprintf( v, 256, _T(" Число точек по U ") ); } break;
#else
      case  IDS_PROP_0317 : { _sntprintf( v, 256, _T(" Number of points by U ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0318 : { _sntprintf( v, 256, _T(" Число точек по V ") ); } break;
#else
      case  IDS_PROP_0318 : { _sntprintf( v, 256, _T(" Number of points by V ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0320 : { _sntprintf( v, 256, _T(" Локальная система ") ); } break;
#else
      case  IDS_PROP_0320 : { _sntprintf( v, 256, _T(" Local system ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0321 : { _sntprintf( v, 256, _T(" Радиус основания ") ); } break;
#else
      case  IDS_PROP_0321 : { _sntprintf( v, 256, _T(" Radius of base") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0322 : { _sntprintf( v, 256, _T(" Высота ") ); } break;
#else
      case  IDS_PROP_0322 : { _sntprintf( v, 256, _T(" Height ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0323 : { _sntprintf( v, 256, _T(" Половина угла  ") ); } break;
#else
      case  IDS_PROP_0323 : { _sntprintf( v, 256, _T(" Half-angle  ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0324 : { _sntprintf( v, 256, _T(" Большой радиус ") ); } break;
#else
      case  IDS_PROP_0324 : { _sntprintf( v, 256, _T(" Major radius ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0325 : { _sntprintf( v, 256, _T(" Малый радиус   ") ); } break;
#else
      case  IDS_PROP_0325 : { _sntprintf( v, 256, _T(" Minor radius   ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0326 : { _sntprintf( v, 256, _T(" Длина ") ); } break;
#else
      case  IDS_PROP_0326 : { _sntprintf( v, 256, _T(" Length ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0327 : { _sntprintf( v, 256, _T(" Смещение  ") ); } break;
#else
      case  IDS_PROP_0327 : { _sntprintf( v, 256, _T(" Shift  ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0328 : { _sntprintf( v, 256, _T(" Форма ") ); } break;
#else
      case  IDS_PROP_0328 : { _sntprintf( v, 256, _T(" Shape ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0329 : { _sntprintf( v, 256, _T(" Закрепление границы поверхности ") ); } break;
#else
      case  IDS_PROP_0329 : { _sntprintf( v, 256, _T(" Surface boundary fixation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0330 : { _sntprintf( v, 256, _T(" Отличается от базовой поверхности ") ); } break;
#else
      case  IDS_PROP_0330 : { _sntprintf( v, 256, _T(" Differs from the base surface ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0331 : { _sntprintf( v, 256, _T(" Видимая длина Xmin ") ); } break;
#else
      case  IDS_PROP_0331 : { _sntprintf( v, 256, _T(" Visible length Xmin ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0332 : { _sntprintf( v, 256, _T(" Видимая длина Ymin ") ); } break;
#else
      case  IDS_PROP_0332 : { _sntprintf( v, 256, _T(" Visible length Ymin ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0333 : { _sntprintf( v, 256, _T(" Видимая длина Xmax ") ); } break;
#else
      case  IDS_PROP_0333 : { _sntprintf( v, 256, _T(" Visible length Xmax ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0334 : { _sntprintf( v, 256, _T(" Видимая длина Ymax ") ); } break;
#else
      case  IDS_PROP_0334 : { _sntprintf( v, 256, _T(" Visible length Ymax ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0336 : { _sntprintf( v, 256, _T(" Число узлов по U  ") ); } break;
#else
      case  IDS_PROP_0336 : { _sntprintf( v, 256, _T(" Number of knots by U  ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0337 : { _sntprintf( v, 256, _T(" Значение U узла ") ); } break;
#else
      case  IDS_PROP_0337 : { _sntprintf( v, 256, _T(" Value of U knot ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0338 : { _sntprintf( v, 256, _T(" Число узлов по V  ") ); } break;
#else
      case  IDS_PROP_0338 : { _sntprintf( v, 256, _T(" Number of knots by V  ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0339 : { _sntprintf( v, 256, _T(" Значение V узла ") ); } break;
#else
      case  IDS_PROP_0339 : { _sntprintf( v, 256, _T(" Value of V knot ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0340 : { _sntprintf( v, 256, _T(" Поверхность ") ); } break;
#else
      case  IDS_PROP_0340 : { _sntprintf( v, 256, _T(" Surface ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0341 : { _sntprintf( v, 256, _T(" Направляющая кривая ") ); } break;
#else
      case  IDS_PROP_0341 : { _sntprintf( v, 256, _T(" Guide curve ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0342 : { _sntprintf( v, 256, _T(" Образующая кривая ") ); } break;
#else
      case  IDS_PROP_0342 : { _sntprintf( v, 256, _T(" Generating curve ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0343 : { _sntprintf( v, 256, _T(" Вектор смещения ") ); } break;
#else
      case  IDS_PROP_0343 : { _sntprintf( v, 256, _T(" Translation vector ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0344 : { _sntprintf( v, 256, _T(" Вектор направления ") ); } break;
#else
      case  IDS_PROP_0344 : { _sntprintf( v, 256, _T(" Direction vector ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0345 : { _sntprintf( v, 256, _T(" Точка оси вращения ") ); } break;
#else
      case  IDS_PROP_0345 : { _sntprintf( v, 256, _T(" Point of the rotation axis ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0346 : { _sntprintf( v, 256, _T(" Направление оси ") ); } break;
#else
      case  IDS_PROP_0346 : { _sntprintf( v, 256, _T(" Axis direction ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0347 : { _sntprintf( v, 256, _T(" Угол вращения ") ); } break;
#else
      case  IDS_PROP_0347 : { _sntprintf( v, 256, _T(" Rotation angle ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0348 : { _sntprintf( v, 256, _T(" Вершина ") ); } break;
#else
      case  IDS_PROP_0348 : { _sntprintf( v, 256, _T(" Vertex ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0350 : { _sntprintf( v, 256, _T(" Базовая поверхность ") ); } break;
#else
      case  IDS_PROP_0350 : { _sntprintf( v, 256, _T(" Base surface ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0351 : { _sntprintf( v, 256, _T(" Поверхность 1") ); } break;
#else
      case  IDS_PROP_0351 : { _sntprintf( v, 256, _T(" Surface 1") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0352 : { _sntprintf( v, 256, _T(" Поверхность 2") ); } break;
#else
      case  IDS_PROP_0352 : { _sntprintf( v, 256, _T(" Surface 2") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0353 : { _sntprintf( v, 256, _T(" Контур ") ); } break;
#else
      case  IDS_PROP_0353 : { _sntprintf( v, 256, _T(" Contour ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0354 : { _sntprintf( v, 256, _T(" Число контуров ") ); } break;
#else
      case  IDS_PROP_0354 : { _sntprintf( v, 256, _T(" Number of contours ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0355 : { _sntprintf( v, 256, _T(" Двумерный контур ") ); } break;
#else
      case  IDS_PROP_0355 : { _sntprintf( v, 256, _T(" Two-dimensional contour ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0356 : { _sntprintf( v, 256, _T(" Двумерная кривая ") ); } break;
#else
      case  IDS_PROP_0356 : { _sntprintf( v, 256, _T(" Two-dimensional curve ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0357 : { _sntprintf( v, 256, _T(" Контур 1 ") ); } break;
#else
      case  IDS_PROP_0357 : { _sntprintf( v, 256, _T(" Contour 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0358 : { _sntprintf( v, 256, _T(" Контур 2 ") ); } break;
#else
      case  IDS_PROP_0358 : { _sntprintf( v, 256, _T(" Contour 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0360 : { _sntprintf( v, 256, _T(" Плоскость ") ); } break;
#else
      case  IDS_PROP_0360 : { _sntprintf( v, 256, _T(" Plane ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0361 : { _sntprintf( v, 256, _T(" Вес поверхности 1 ") ); } break;
#else
      case  IDS_PROP_0361 : { _sntprintf( v, 256, _T(" Weight of surface 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0362 : { _sntprintf( v, 256, _T(" Вес поверхности 2 ") ); } break;
#else
      case  IDS_PROP_0362 : { _sntprintf( v, 256, _T(" Weight of surface 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0363 : { _sntprintf( v, 256, _T(" Производная в начале ") ); } break;
#else
      case  IDS_PROP_0363 : { _sntprintf( v, 256, _T(" Derivative at the beginning ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0364 : { _sntprintf( v, 256, _T(" Производная в конце ") ); } break;
#else
      case  IDS_PROP_0364 : { _sntprintf( v, 256, _T(" Derivative at the end ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0365 : { _sntprintf( v, 256, _T(" Установлена нормаль в начале ") ); } break;
#else
      case  IDS_PROP_0365 : { _sntprintf( v, 256, _T(" Normal is set at the start ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0366 : { _sntprintf( v, 256, _T(" Установлена нормаль в конце ") ); } break;
#else
      case  IDS_PROP_0366 : { _sntprintf( v, 256, _T(" Normal is set at the end ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0367 : { _sntprintf( v, 256, _T(" Множитель длины производной в начале ") ); } break;
#else
      case  IDS_PROP_0367 : { _sntprintf( v, 256, _T(" Derivative factor at the start ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0368 : { _sntprintf( v, 256, _T(" Множитель длины производной в конце ") ); } break;
#else
      case  IDS_PROP_0368 : { _sntprintf( v, 256, _T(" Derivative factor at the end ") ); } break;
#endif

      PROMPT_CASE_RU( IDS_PROP_0370, " Кривая на поверхности 0 " );
      PROMPT_CASE_EN( IDS_PROP_0370, " Curve on surface 0 " );

      PROMPT_CASE_RU( IDS_PROP_0371, " Кривая на поверхности 1 " );
      PROMPT_CASE_EN( IDS_PROP_0371, " Curve on surface 1 " );

      PROMPT_CASE_RU( IDS_PROP_0372, " Кривая на поверхности 2 " );
      PROMPT_CASE_EN( IDS_PROP_0372, " Curve on surface 2 " );

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0373 : { _sntprintf( v, 256, _T(" Кривая вершин ") ); } break;
#else
      case  IDS_PROP_0373 : { _sntprintf( v, 256, _T(" Curve of vertices ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0374 : { _sntprintf( v, 256, _T(" Параметр Umin ") ); } break;
#else
      case  IDS_PROP_0374 : { _sntprintf( v, 256, _T(" Parameter Umin ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0375 : { _sntprintf( v, 256, _T(" Параметр Umax ") ); } break;
#else
      case  IDS_PROP_0375 : { _sntprintf( v, 256, _T(" Parameter Umax ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0376 : { _sntprintf( v, 256, _T(" Параметр Vmin ") ); } break;
#else
      case  IDS_PROP_0376 : { _sntprintf( v, 256, _T(" Parameter Vmin ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0377 : { _sntprintf( v, 256, _T(" Параметр Vmax ") ); } break;
#else
      case  IDS_PROP_0377 : { _sntprintf( v, 256, _T(" Parameter Vmax ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0378 : { _sntprintf( v, 256, _T(" Производная dU ") ); } break;
#else
      case  IDS_PROP_0378 : { _sntprintf( v, 256, _T(" Derivative dU ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0379 : { _sntprintf( v, 256, _T(" Производная dV ") ); } break;
#else
      case  IDS_PROP_0379 : { _sntprintf( v, 256, _T(" Derivative dV ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0380 : { _sntprintf( v, 256, _T(" Кромка проходит по Vmin ") ); } break;
#else
      case  IDS_PROP_0380 : { _sntprintf( v, 256, _T(" Boundary passes through Vmin ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0384 : { _sntprintf( v, 256, _T(" Расширение minUPar ") ); } break;
#else
      case  IDS_PROP_0384 : { _sntprintf( v, 256, _T(" Extension minUPar ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0385 : { _sntprintf( v, 256, _T(" Расширение maxUPar") ); } break;
#else
      case  IDS_PROP_0385 : { _sntprintf( v, 256, _T(" Extension maxUPar") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0386 : { _sntprintf( v, 256, _T(" Расширение minVPar") ); } break;
#else
      case  IDS_PROP_0386 : { _sntprintf( v, 256, _T(" Extension minVPar") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0387 : { _sntprintf( v, 256, _T(" Расширение maxVPar") ); } break;
#else
      case  IDS_PROP_0387 : { _sntprintf( v, 256, _T(" Extension maxVPar") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0390 : { _sntprintf( v, 256, _T(" Число кривых ") ); } break;
#else
      case  IDS_PROP_0390 : { _sntprintf( v, 256, _T(" Number of curves ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0391 : { _sntprintf( v, 256, _T(" Кривая по U ") ); } break;
#else
      case  IDS_PROP_0391 : { _sntprintf( v, 256, _T(" Curve by U ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0392 : { _sntprintf( v, 256, _T(" Кривая по V ") ); } break;
#else
      case  IDS_PROP_0392 : { _sntprintf( v, 256, _T(" Curve by V ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0393 : { _sntprintf( v, 256, _T(" Число кривых по U  ") ); } break;
#else
      case  IDS_PROP_0393 : { _sntprintf( v, 256, _T(" Number of curves by U  ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0394 : { _sntprintf( v, 256, _T(" Число кривых по V ") ); } break;
#else
      case  IDS_PROP_0394 : { _sntprintf( v, 256, _T(" Number of curves by V ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0395 : { _sntprintf( v, 256, _T(" Тип поверхности ") ); } break;
#else
      case  IDS_PROP_0395 : { _sntprintf( v, 256, _T(" Type of surface ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0397 : { _sntprintf( v, 256, _T(" Нейтральная плоскость") ); } break;
#else
      case  IDS_PROP_0397 : { _sntprintf( v, 256, _T(" Neutral plane") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0398 : { _sntprintf( v, 256, _T(" Плоскость контура ") ); } break;
#else
      case  IDS_PROP_0398 : { _sntprintf( v, 256, _T(" Plane of the contour ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0399 : { _sntprintf( v, 256, _T(" Через элементы") ); } break;
#else
      case  IDS_PROP_0399 : { _sntprintf( v, 256, _T(" Through elements") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0400 : { _sntprintf( v, 256, _T(" Сопряжение на границе ") ); } break;
#else
      case  IDS_PROP_0400 : { _sntprintf( v, 256, _T(" Conjugation on the boundary ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0401 : { _sntprintf( v, 256, _T(" Натяжение на границе ") ); } break;
#else
      case  IDS_PROP_0401 : { _sntprintf( v, 256, _T(" Tension on the boundary ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0402 : { _sntprintf( v, 256, _T(" Параметр определения длины производных ") ); } break;
#else
      case  IDS_PROP_0402 : { _sntprintf( v, 256, _T(" Parameter of derivetives length definition ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0403 : { _sntprintf( v, 256, _T(" Не сохранять длину производной ") ); } break;
#else
      case  IDS_PROP_0403 : { _sntprintf( v, 256, _T(" Do not keep the derivative length ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0404 : { _sntprintf( v, 256, _T(" Тип сопряжения (0-4) ") ); } break;
#else
      case  IDS_PROP_0404 : { _sntprintf( v, 256, _T(" Conjugation type (0-4) ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0405 : { _sntprintf( v, 256, _T(" Использовать готовый узловой вектор ") ); } break;
#else
      case  IDS_PROP_0405 : { _sntprintf( v, 256, _T(" Use prepared knot vector ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0406 : { _sntprintf( v, 256, _T(" Количество узлов ") ); } break;
#else
      case  IDS_PROP_0406 : { _sntprintf( v, 256, _T(" Number of knots ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0407 : { _sntprintf( v, 256, _T(" Проверка самопересечений") ); } break;
#else
      case  IDS_PROP_0407 : { _sntprintf( v, 256, _T(" Check for self-intersections") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0408 : { _sntprintf( v, 256, _T(" Используется общий вес точек") ); } break;
#else
      case  IDS_PROP_0408 : { _sntprintf( v, 256, _T(" The common weight of points is used") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0409 : { _sntprintf( v, 256, _T(" Построена по пласту точек") ); } break;
#else
      case  IDS_PROP_0409 : { _sntprintf( v, 256, _T(" Build from a cloud of points") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0410 : { _sntprintf( v, 256, _T(" Построена по сети точек") ); } break;
#else
      case  IDS_PROP_0410 : { _sntprintf( v, 256, _T(" Build from a mesh of points") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0411 : { _sntprintf( v, 256, _T(" В виде набора треугольников") ); } break;
#else
      case  IDS_PROP_0411 : { _sntprintf( v, 256, _T(" As a set of triangles") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0412 : { _sntprintf( v, 256, _T(" Использовать проекционную кривую") ); } break;
#else
      case  IDS_PROP_0412 : { _sntprintf( v, 256, _T(" Use projection curve") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0413 : { _sntprintf( v, 256, _T(" Усекать границами") ); } break;
#else
      case  IDS_PROP_0413 : { _sntprintf( v, 256, _T(" Truncate by bounds") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0414 : { _sntprintf( v, 256, _T(" Привязка к началу") ); } break;
#else
      case  IDS_PROP_0414 : { _sntprintf( v, 256, _T(" Binding to the beginning") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0415 : { _sntprintf( v, 256, _T(" Соединять скруглениями") ); } break;
#else
      case  IDS_PROP_0415 : { _sntprintf( v, 256, _T(" Join by fillets") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0416 : { _sntprintf( v, 256, _T(" Сохранять радиус") ); } break;
#else
      case  IDS_PROP_0416 : { _sntprintf( v, 256, _T(" Keep the radius") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0417 : { _sntprintf( v, 256, _T(" Притуплять острый угол") ); } break;
#else
      case  IDS_PROP_0417 : { _sntprintf( v, 256, _T(" Blunt a sharp angle") ); } break;
#endif

      PROMPT_CASE_RU( IDS_PROP_0418, " Проверка пересечений" );
      PROMPT_CASE_EN( IDS_PROP_0418, " Check for intersections" );

      PROMPT_CASE_RU( IDS_PROP_0419, " Слияние подобных граней" );
      PROMPT_CASE_EN( IDS_PROP_0419, " Similar faces merging" );
      PROMPT_CASE_RU( IDS_PROP_0420, " Слияние подобных рёбер" );
      PROMPT_CASE_EN( IDS_PROP_0420, " Similar edges merging" );

      PROMPT_CASE_RU( IDS_PROP_0421, " Номер соседнего объекта" );
      PROMPT_CASE_EN( IDS_PROP_0421, " The number of neighbour object" );

      PROMPT_CASE_RU( IDS_PROP_0422, " Слияние подобных кривых" );
      PROMPT_CASE_EN( IDS_PROP_0422, " Similar curves merging" );
      PROMPT_CASE_RU( IDS_PROP_0423, " Резка кривых" );
      PROMPT_CASE_EN( IDS_PROP_0423, " Curves cutting" );
      PROMPT_CASE_RU( IDS_PROP_0424, " Резка рёбер" );
      PROMPT_CASE_EN( IDS_PROP_0424, " Edges cutting" );
      PROMPT_CASE_RU( IDS_PROP_0425, " Резка поверхностей" );
      PROMPT_CASE_EN( IDS_PROP_0425, " Surfaces cutting" );
      PROMPT_CASE_RU( IDS_PROP_0426, " Резка граней" );
      PROMPT_CASE_EN( IDS_PROP_0426, " Faces cutting" );

      PROMPT_CASE_RU( IDS_PROP_0427, " Вдоль поверхности" );
      PROMPT_CASE_EN( IDS_PROP_0427, " Along surface" );

      PROMPT_CASE_RU( IDS_PROP_0429, " Площадь");
      PROMPT_CASE_EN( IDS_PROP_0429, " Area");

      PROMPT_CASE_RU( IDS_PROP_0430, " Способ продления начала" );
      PROMPT_CASE_EN( IDS_PROP_0430, " Extension way of start point" );
      PROMPT_CASE_RU( IDS_PROP_0431, " Способ продления конца" );
      PROMPT_CASE_EN( IDS_PROP_0431, " Extension way of end point" );
      PROMPT_CASE_RU( IDS_PROP_0432, " Длина в начале" );
      PROMPT_CASE_EN( IDS_PROP_0432, " Length at start point" );
      PROMPT_CASE_RU( IDS_PROP_0433, " Длина в конце" );
      PROMPT_CASE_EN( IDS_PROP_0433, " Length at end point" );

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0440 : { _sntprintf( v, 256, _T(" Круговое сечение поверхности ") ); } break;
#else
      case  IDS_PROP_0440 : { _sntprintf( v, 256, _T(" Round section surface ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0441 : { _sntprintf( v, 256, _T(" Линейное сечение поверхности ") ); } break;
#else
      case  IDS_PROP_0441 : { _sntprintf( v, 256, _T(" Line section surface ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0442 : { _sntprintf( v, 256, _T(" Коническое сечение поверхности ") ); } break;
#else
      case  IDS_PROP_0442 : { _sntprintf( v, 256, _T(" Conic section surface ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0443 : { _sntprintf( v, 256, _T(" Кубическое сечение поверхности ") ); } break;
#else
      case  IDS_PROP_0443 : { _sntprintf( v, 256, _T(" Cubic section surface ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0444 : { _sntprintf( v, 256, _T(" Сплайновое сечение поверхности ") ); } break;
#else
      case  IDS_PROP_0444 : { _sntprintf( v, 256, _T(" Spline section surface ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0445 : { _sntprintf( v, 256, _T(" Поверхность скругления движением диска") ); } break;
#else
      case  IDS_PROP_0445 : { _sntprintf( v, 256, _T(" Fillet disk movement surface ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0446 : { _sntprintf( v, 256, _T(" Поверхность скругления движением сферы ") ); } break;
#else
      case  IDS_PROP_0446 : { _sntprintf( v, 256, _T(" Fillet sphere movement surface ") ); } break;
#endif

      // \ru Резьба \en Thread

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0450 : { _sntprintf( v, 256, _T(" Начальный радиус (поверхность)") ); } break;
#else
      case  IDS_PROP_0450 : { _sntprintf( v, 256, _T(" Start radius (surface)") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0451 : { _sntprintf( v, 256, _T(" Конечный радиус  (резьба)     ") ); } break;
#else
      case  IDS_PROP_0451 : { _sntprintf( v, 256, _T(" End radius  (thread)     ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0452 : { _sntprintf( v, 256, _T(" Длина резьбы                  ") ); } break;
#else
      case  IDS_PROP_0452 : { _sntprintf( v, 256, _T(" Thread length                  ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0453 : { _sntprintf( v, 256, _T(" Угол коничности резьбы        ") ); } break;
#else
      case  IDS_PROP_0453 : { _sntprintf( v, 256, _T(" Taper angle of the thread        ") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0459: { _sntprintf( v, 256, _T( " Количество регионов " ) ); } break;
#else
      case  IDS_PROP_0459: { _sntprintf( v, 256, _T( " Regions count " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0460: { _sntprintf( v, 256, _T( " Количество подрегионов " ) ); } break;
#else
      case  IDS_PROP_0460: { _sntprintf( v, 256, _T( " Subregions count " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0461 : { _sntprintf( v, 256, _T(" Триангуляция ") ); } break;
#else
      case  IDS_PROP_0461 : { _sntprintf( v, 256, _T(" Triangulation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0462 : { _sntprintf( v, 256, _T(" Количество точек триангуляции ") ); } break;
#else
      case  IDS_PROP_0462 : { _sntprintf( v, 256, _T(" Number of points of triangulation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0463 : { _sntprintf( v, 256, _T(" Количество двумерных точек ") ); } break;
#else
      case  IDS_PROP_0463 : { _sntprintf( v, 256, _T(" Number of two-dimension points ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0464 : { _sntprintf( v, 256, _T(" Количество точек полигонов ") ); } break;
#else
      case  IDS_PROP_0464 : { _sntprintf( v, 256, _T(" Number of points of polygons ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0465: { _sntprintf( v, 256, _T( " Количество фасетов " ) ); } break;
#else
      case  IDS_PROP_0465: { _sntprintf( v, 256, _T( " Facets count " ) ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0466 : { _sntprintf( v, 256, _T(" Форма поверхности сопряжения ") ); } break;
#else
      case  IDS_PROP_0466 : { _sntprintf( v, 256, _T(" Shape of blend surface ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0467 : { _sntprintf( v, 256, _T(" Форма обрезки боков поверхности ") ); } break;
#else
      case  IDS_PROP_0467 : { _sntprintf( v, 256, _T(" Shape of cropping the surface sides ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0468 : { _sntprintf( v, 256, _T(" Обработка опорных граней ") ); } break;
#else
      case  IDS_PROP_0468 : { _sntprintf( v, 256, _T(" Initial faces processing ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0469 : { _sntprintf( v, 256, _T(" Разделять оболочку на грани ") ); } break;
#else
      case  IDS_PROP_0469 : { _sntprintf( v, 256, _T(" Division the shell into faces ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0470 : { _sntprintf( v, 256, _T(" Обрезать опору по касательной ") ); } break;
#else
      case  IDS_PROP_0470 : { _sntprintf( v, 256, _T(" Trim the support faces tangentially ") ); } break;
#endif

// \ru Параметры \en Parameters
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0434 : { _sntprintf( v, 256, _T(" Величина изменения радиуса скругления. ") ); } break;
#else
      case  IDS_PROP_0434 : { _sntprintf( v, 256, _T(" Value of a fillet radius change. ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0499: { _sntprintf( v, 256, _T( " Есть оболочка-подложка " ) ); } break;
#else
      case  IDS_PROP_0499: { _sntprintf( v, 256, _T( " Has the substrate shell " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0500: { _sntprintf( v, 256, _T( " Число фасетов " ) ); } break;
#else
      case  IDS_PROP_0500: { _sntprintf( v, 256, _T( " Number of facets " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0501 : { _sntprintf( v, 256, _T(" Число вершин ") ); } break;
#else
      case  IDS_PROP_0501 : { _sntprintf( v, 256, _T(" Number of vertices ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0502 : { _sntprintf( v, 256, _T(" Число ребер ") ); } break;
#else
      case  IDS_PROP_0502 : { _sntprintf( v, 256, _T(" Number of edges ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0503 : { _sntprintf( v, 256, _T(" Число граней ") ); } break;
#else
      case  IDS_PROP_0503 : { _sntprintf( v, 256, _T(" Number of faces ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0504 : { _sntprintf( v, 256, _T(" Ориентация вершины ") ); } break;
#else
      case  IDS_PROP_0504 : { _sntprintf( v, 256, _T(" Vertex orientation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0505 : { _sntprintf( v, 256, _T(" Ориентация ребра ") ); } break;
#else
      case  IDS_PROP_0505 : { _sntprintf( v, 256, _T(" Edge orientation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0506 : { _sntprintf( v, 256, _T(" Ориентация грани ") ); } break;
#else
      case  IDS_PROP_0506 : { _sntprintf( v, 256, _T(" Face orientation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0508 : { _sntprintf( v, 256, _T(" Ребро ") ); } break;
#else
      case  IDS_PROP_0508 : { _sntprintf( v, 256, _T(" Edge ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0509 : { _sntprintf( v, 256, _T(" Грань ") ); } break;
#else
      case  IDS_PROP_0509 : { _sntprintf( v, 256, _T(" Face ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0510 : { _sntprintf( v, 256, _T(" Число циклов ") ); } break;
#else
      case  IDS_PROP_0510 : { _sntprintf( v, 256, _T(" Number of loops ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0511 : { _sntprintf( v, 256, _T(" Цикл ") ); } break;
#else
      case  IDS_PROP_0511 : { _sntprintf( v, 256, _T(" Loop ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0512 : { _sntprintf( v, 256, _T(" Автоопределение ") ); } break;
#else
      case  IDS_PROP_0512 : { _sntprintf( v, 256, _T(" Automatic identification ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0513 : { _sntprintf( v, 256, _T(" Автоматическое ") ); } break;
#else
      case  IDS_PROP_0513 : { _sntprintf( v, 256, _T(" Automatic ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0514 : { _sntprintf( v, 256, _T(" Тип размножения ") ); } break;
#else
      case  IDS_PROP_0514 : { _sntprintf( v, 256, _T(" Type of duplication ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0515 : { _sntprintf( v, 256, _T(" Кол-во шагов ") ); } break;
#else
      case  IDS_PROP_0515 : { _sntprintf( v, 256, _T(" Number steps ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0516 : { _sntprintf( v, 256, _T(" Шагов ") ); } break;
#else
      case  IDS_PROP_0516 : { _sntprintf( v, 256, _T(" Step ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0517 : { _sntprintf( v, 256, _T(" Кол-во угловых шагов ") ); } break;
#else
      case  IDS_PROP_0517 : { _sntprintf( v, 256, _T(" Number of angular step ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0518 : { _sntprintf( v, 256, _T(" Элемент ") ); } break;
#else
      case  IDS_PROP_0518 : { _sntprintf( v, 256, _T(" Element ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0519: { _sntprintf( v, 256, _T( " Сегмент полигональной сетки " ) ); } break;
#else
      case  IDS_PROP_0519: { _sntprintf( v, 256, _T( " Segment of polygonal mesh " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0520: { _sntprintf( v, 256, _T( " Направляющая поверхность " ) ); } break;
#else
      case  IDS_PROP_0520: { _sntprintf( v, 256, _T( " Guide surface " ) ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0521 : { _sntprintf( v, 256, _T(" Длина  Lx ") ); } break;
#else
      case  IDS_PROP_0521 : { _sntprintf( v, 256, _T(" Length  Lx ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0522 : { _sntprintf( v, 256, _T(" Ширина Ly ") ); } break;
#else
      case  IDS_PROP_0522 : { _sntprintf( v, 256, _T(" Width Ly ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0523 : { _sntprintf( v, 256, _T(" Высота Lz ") ); } break;
#else
      case  IDS_PROP_0523 : { _sntprintf( v, 256, _T(" Height Lz ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0524 : { _sntprintf( v, 256, _T(" Малая длина lx ") ); } break;
#else
      case  IDS_PROP_0524 : { _sntprintf( v, 256, _T(" Minor length lx ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0525 : { _sntprintf( v, 256, _T(" Толщина ") ); } break;
#else
      case  IDS_PROP_0525 : { _sntprintf( v, 256, _T(" Thickness ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0526 : { _sntprintf( v, 256, _T(" Толщина стенки ") ); } break;
#else
      case  IDS_PROP_0526 : { _sntprintf( v, 256, _T(" Wall thickness ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0527 : { _sntprintf( v, 256, _T(" Число вскрытых граней ") ); } break;
#else
      case  IDS_PROP_0527 : { _sntprintf( v, 256, _T(" Number of opened faces ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0528 : { _sntprintf( v, 256, _T(" Форма ") ); } break;
#else
      case  IDS_PROP_0528 : { _sntprintf( v, 256, _T(" Shape ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0529 : { _sntprintf( v, 256, _T(" Сохранять кромку ") ); } break;
#else
      case  IDS_PROP_0529 : { _sntprintf( v, 256, _T(" Keep the boundary ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0530 : { _sntprintf( v, 256, _T(" Продолжить далее ") ); } break;
#else
      case  IDS_PROP_0530 : { _sntprintf( v, 256, _T(" Continue ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0531 : { _sntprintf( v, 256, _T(" Катет 1 ") ); } break;
#else
      case  IDS_PROP_0531 : { _sntprintf( v, 256, _T(" Cathetus 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0532 : { _sntprintf( v, 256, _T(" Катет 2 ") ); } break;
#else
      case  IDS_PROP_0532 : { _sntprintf( v, 256, _T(" Cathetus 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0533 : { _sntprintf( v, 256, _T(" Число фасок ") ); } break;
#else
      case  IDS_PROP_0533 : { _sntprintf( v, 256, _T(" Number of chamfers ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0534 : { _sntprintf( v, 256, _T(" Число скруглений ") ); } break;
#else
      case  IDS_PROP_0534 : { _sntprintf( v, 256, _T(" Number of fillets ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0535 : { _sntprintf( v, 256, _T(" Радиус скругления ") ); } break;
#else
      case  IDS_PROP_0535 : { _sntprintf( v, 256, _T(" Fillet radius ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0536 : { _sntprintf( v, 256, _T(" Номер грани  ") ); } break;
#else
      case  IDS_PROP_0536 : { _sntprintf( v, 256, _T(" Face number  ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0537 : { _sntprintf( v, 256, _T(" Параметр U ") ); } break;
#else
      case  IDS_PROP_0537 : { _sntprintf( v, 256, _T(" Parameter U ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0538 : { _sntprintf( v, 256, _T(" Параметр V ") ); } break;
#else
      case  IDS_PROP_0538 : { _sntprintf( v, 256, _T(" Parameter V ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0539 : { _sntprintf( v, 256, _T(" Число модифицированных граней ") ); } break;
#else
      case  IDS_PROP_0539 : { _sntprintf( v, 256, _T(" Number of modified faces ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0540 : { _sntprintf( v, 256, _T(" Тело ") ); } break;
#else
      case  IDS_PROP_0540 : { _sntprintf( v, 256, _T(" Solid ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0541 : { _sntprintf( v, 256, _T(" Строитель тела ") ); } break;
#else
      case  IDS_PROP_0541 : { _sntprintf( v, 256, _T(" Construct Solid ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0542 : { _sntprintf( v, 256, _T(" Количество четырёхугольников ") ); } break;
#else
      case  IDS_PROP_0542 : { _sntprintf( v, 256, _T(" Number of quadrangles") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0543 : { _sntprintf( v, 256, _T(" Остановка от начала ") ); } break;
#else
      case  IDS_PROP_0543 : { _sntprintf( v, 256, _T(" Termination from the start ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0544 : { _sntprintf( v, 256, _T(" Остановка до конца  ") ); } break;
#else
      case  IDS_PROP_0544 : { _sntprintf( v, 256, _T(" Termination to the end  ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0545 : { _sntprintf( v, 256, _T(" Радиус скругления 1 ") ); } break;
#else
      case  IDS_PROP_0545 : { _sntprintf( v, 256, _T(" Fillet radius 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0546 : { _sntprintf( v, 256, _T(" Радиус скругления 2 ") ); } break;
#else
      case  IDS_PROP_0546 : { _sntprintf( v, 256, _T(" Fillet radius 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0547 : { _sntprintf( v, 256, _T(" Коэффициент полноты ") ); } break;
#else
      case  IDS_PROP_0547 : { _sntprintf( v, 256, _T(" Coefficient of completeness ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0548 : { _sntprintf( v, 256, _T(" Способ обработки углов стыковки рёбер ") ); } break;
#else
      case  IDS_PROP_0548 : { _sntprintf( v, 256, _T(" Method of processing corners of edges connection ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0549 : { _sntprintf( v, 256, _T(" Четырехугольник ") ); } break;
#else
      case  IDS_PROP_0549 : { _sntprintf( v, 256, _T(" Quadrangle ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0550 : { _sntprintf( v, 256, _T(" Базовое тело ") ); } break;
#else
      case  IDS_PROP_0550 : { _sntprintf( v, 256, _T(" Base solid ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0551 : { _sntprintf( v, 256, _T(" Тело 1 ") ); } break;
#else
      case  IDS_PROP_0551 : { _sntprintf( v, 256, _T(" Solid 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0552 : { _sntprintf( v, 256, _T(" Тело 2 ") ); } break;
#else
      case  IDS_PROP_0552 : { _sntprintf( v, 256, _T(" Solid 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0553 : { _sntprintf( v, 256, _T(" Исходное тело ") ); } break;
#else
      case  IDS_PROP_0553 : { _sntprintf( v, 256, _T(" Initial solid ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0554 : { _sntprintf( v, 256, _T(" Режущая поверхность ") ); } break;
#else
      case  IDS_PROP_0554 : { _sntprintf( v, 256, _T(" Cutting surface ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0555 : { _sntprintf( v, 256, _T(" Оставляемая часть ") ); } break;
#else
      case  IDS_PROP_0555 : { _sntprintf( v, 256, _T(" A part to keep ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0556 : { _sntprintf( v, 256, _T(" Точка симметрии ") ); } break;
#else
      case  IDS_PROP_0556 : { _sntprintf( v, 256, _T(" Symmetry point ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0557 : { _sntprintf( v, 256, _T(" Ось X симметрии ") ); } break;
#else
      case  IDS_PROP_0557 : { _sntprintf( v, 256, _T(" Axis X of symmetry ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0558 : { _sntprintf( v, 256, _T(" Ось Y симметрии ") ); } break;
#else
      case  IDS_PROP_0558 : { _sntprintf( v, 256, _T(" Axis Y of symmetry ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0559 : { _sntprintf( v, 256, _T(" Поверхность ") ); } break;
#else
      case  IDS_PROP_0559 : { _sntprintf( v, 256, _T(" Surface ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0560 : { _sntprintf( v, 256, _T(" Внешняя оболочка ") ); } break;
#else
      case  IDS_PROP_0560 : { _sntprintf( v, 256, _T(" Outer shell ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0561 : { _sntprintf( v, 256, _T(" Пустотная оболочка ") ); } break;
#else
      case  IDS_PROP_0561 : { _sntprintf( v, 256, _T(" Void shell ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0562 : { _sntprintf( v, 256, _T(" Число пустот ") ); } break;
#else
      case  IDS_PROP_0562 : { _sntprintf( v, 256, _T(" Number of voids ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0563 : { _sntprintf( v, 256, _T(" Глубина 1 ") ); } break;
#else
      case  IDS_PROP_0563 : { _sntprintf( v, 256, _T(" Depth 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0564 : { _sntprintf( v, 256, _T(" Глубина 2 ") ); } break;
#else
      case  IDS_PROP_0564 : { _sntprintf( v, 256, _T(" Depth 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0565 : { _sntprintf( v, 256, _T(" Угол уклона 1 ") ); } break;
#else
      case  IDS_PROP_0565 : { _sntprintf( v, 256, _T(" Slope angle 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0566 : { _sntprintf( v, 256, _T(" Угол уклона 2 ") ); } break;
#else
      case  IDS_PROP_0566 : { _sntprintf( v, 256, _T(" Slope angle 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0567 : { _sntprintf( v, 256, _T(" Толщина стенки 1 ") ); } break;
#else
      case  IDS_PROP_0567 : { _sntprintf( v, 256, _T(" Wall thickness 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0568 : { _sntprintf( v, 256, _T(" Толщина стенки 2 ") ); } break;
#else
      case  IDS_PROP_0568 : { _sntprintf( v, 256, _T(" Wall thickness 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0569 : { _sntprintf( v, 256, _T(" Толщина ") ); } break;
#else
      case  IDS_PROP_0569 : { _sntprintf( v, 256, _T(" Thickness ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0570 : { _sntprintf( v, 256, _T(" Глубина ") ); } break;
#else
      case  IDS_PROP_0570 : { _sntprintf( v, 256, _T(" Depth ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0571 : { _sntprintf( v, 256, _T(" Способ построения ") ); } break;
#else
      case  IDS_PROP_0571 : { _sntprintf( v, 256, _T(" Method of construction ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0572 : { _sntprintf( v, 256, _T(" Число оболочек ") ); } break;
#else
      case  IDS_PROP_0572 : { _sntprintf( v, 256, _T(" Number of shells ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0573 : { _sntprintf( v, 256, _T(" Количество треугольников ") ); } break;
#else
      case  IDS_PROP_0573 : { _sntprintf( v, 256, _T(" Number of triangles ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0575 : { _sntprintf( v, 256, _T(" Угол вращения 1 ") ); } break;
#else
      case  IDS_PROP_0575 : { _sntprintf( v, 256, _T(" Rotation angle 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0576 : { _sntprintf( v, 256, _T(" Угол вращения 2 ") ); } break;
#else
      case  IDS_PROP_0576 : { _sntprintf( v, 256, _T(" Rotation angle 2 ") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0577 : { _sntprintf( v, 256, _T(" Первая вершина ") ); } break;
#else
      case  IDS_PROP_0577 : { _sntprintf( v, 256, _T(" The first vertex ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0578 : { _sntprintf( v, 256, _T(" Вторая вершина ") ); } break;
#else
      case  IDS_PROP_0578 : { _sntprintf( v, 256, _T(" The second vertex ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0579 : { _sntprintf( v, 256, _T(" Третья вершина ") ); } break;
#else
      case  IDS_PROP_0579 : { _sntprintf( v, 256, _T(" The third vertex ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0580 : { _sntprintf( v, 256, _T(" Четвертая вершина ") ); } break;
#else
      case  IDS_PROP_0580 : { _sntprintf( v, 256, _T(" The fourth vertex ") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0581 : { _sntprintf( v, 256, _T(" Способ построения 1 ") ); } break;
#else
      case  IDS_PROP_0581 : { _sntprintf( v, 256, _T(" Method of construction 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0582 : { _sntprintf( v, 256, _T(" Способ построения 2 ") ); } break;
#else
      case  IDS_PROP_0582 : { _sntprintf( v, 256, _T(" Method of construction 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0583 : { _sntprintf( v, 256, _T(" Расстояние 1 ") ); } break;
#else
      case  IDS_PROP_0583 : { _sntprintf( v, 256, _T(" Distance 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0584 : { _sntprintf( v, 256, _T(" Расстояние 2 ") ); } break;
#else
      case  IDS_PROP_0584 : { _sntprintf( v, 256, _T(" Distance 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0585 : { _sntprintf( v, 256, _T(" Треугольник ") ); } break;
#else
      case  IDS_PROP_0585 : { _sntprintf( v, 256, _T(" Triangle ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0586 : { _sntprintf( v, 256, _T(" Количество апексов ") ); } break;
#else
      case  IDS_PROP_0586 : { _sntprintf( v, 256, _T(" Number of apexes ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0587 : { _sntprintf( v, 256, _T(" Количество полигонов ") ); } break;
#else
      case  IDS_PROP_0587 : { _sntprintf( v, 256, _T(" Number of polygons ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0588 : { _sntprintf( v, 256, _T(" Количество сеток триангуляции ") ); } break;
#else
      case  IDS_PROP_0588 : { _sntprintf( v, 256, _T(" Number of triangulation grids ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0589 : { _sntprintf( v, 256, _T(" Контур ") ); } break;
#else
      case  IDS_PROP_0589 : { _sntprintf( v, 256, _T(" Contour ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0590 : { _sntprintf( v, 256, _T(" Число сечений ") ); } break;
#else
      case  IDS_PROP_0590 : { _sntprintf( v, 256, _T(" Number of sections ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0591 : { _sntprintf( v, 256, _T(" Сечение ") ); } break;
#else
      case  IDS_PROP_0591 : { _sntprintf( v, 256, _T(" Section ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0592 : { _sntprintf( v, 256, _T(" Параллельность ") ); } break;
#else
      case  IDS_PROP_0592 : { _sntprintf( v, 256, _T(" Parallelity ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0593 : { _sntprintf( v, 256, _T(" Ориентация образующей  ") ); } break;
#else
      case  IDS_PROP_0593 : { _sntprintf( v, 256, _T(" Generatrix orientation  ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0594 : { _sntprintf( v, 256, _T(" Положение образующей  ") ); } break;
#else
      case  IDS_PROP_0594 : { _sntprintf( v, 256, _T(" Generatrix position  ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0595 : { _sntprintf( v, 256, _T(" Сфероид (0) или тороид (1) ") ); } break;
#else
      case  IDS_PROP_0595 : { _sntprintf( v, 256, _T(" Spheroid (0) or toroid (1) ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0596 : { _sntprintf( v, 256, _T(" Полюс в начале ") ); } break;
#else
      case  IDS_PROP_0596 : { _sntprintf( v, 256, _T(" Pole at the beginning ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0597 : { _sntprintf( v, 256, _T(" Полюс в конце ") ); } break;
#else
      case  IDS_PROP_0597 : { _sntprintf( v, 256, _T(" Pole at the end ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0598 : { _sntprintf( v, 256, _T(" Продолжение ") ); } break;
#else
      case  IDS_PROP_0598 : { _sntprintf( v, 256, _T(" Extension ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0599 : { _sntprintf( v, 256, _T(" Смещение ") ); } break;
#else
      case  IDS_PROP_0599 : { _sntprintf( v, 256, _T(" Shift ") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0600 : { _sntprintf( v, 256, _T(" Оболочка тела ") ); } break;
#else
      case  IDS_PROP_0600 : { _sntprintf( v, 256, _T(" Shell ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0601 : { _sntprintf( v, 256, _T(" Ориентация ребра в цикле ") ); } break;
#else
      case  IDS_PROP_0601 : { _sntprintf( v, 256, _T(" Sense of edge in the loop ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0602 : { _sntprintf( v, 256, _T(" Ориентация кривой ребра ") ); } break;
#else
      case  IDS_PROP_0602 : { _sntprintf( v, 256, _T(" Edge curve orientation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0603 : { _sntprintf( v, 256, _T(" Ориентация нормали оболочки ") ); } break;
#else
      case  IDS_PROP_0603 : { _sntprintf( v, 256, _T(" Orientation of a shell normal ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0604 : { _sntprintf( v, 256, _T(" Кривая ребра ") ); } break;
#else
      case  IDS_PROP_0604 : { _sntprintf( v, 256, _T(" Edge curve ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0605 : { _sntprintf( v, 256, _T(" Двумерная кривая ребра ") ); } break;
#else
      case  IDS_PROP_0605 : { _sntprintf( v, 256, _T(" Two-dimensional edge curve ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0606 : { _sntprintf( v, 256, _T(" Поверхность грани ") ); } break;
#else
      case  IDS_PROP_0606 : { _sntprintf( v, 256, _T(" Surface of a face ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0607 : { _sntprintf( v, 256, _T(" Вершина-начало ") ); } break;
#else
      case  IDS_PROP_0607 : { _sntprintf( v, 256, _T(" Start vertex ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0608 : { _sntprintf( v, 256, _T(" Вершина-конец ") ); } break;
#else
      case  IDS_PROP_0608 : { _sntprintf( v, 256, _T(" End vertex ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0609 : { _sntprintf( v, 256, _T(" Количество ссылок ") ); } break;
#else
      case  IDS_PROP_0609 : { _sntprintf( v, 256, _T(" References count ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0611 : { _sntprintf( v, 256, _T(" Грань плюс ") ); } break;
#else
      case  IDS_PROP_0611 : { _sntprintf( v, 256, _T(" Face plus ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0612 : { _sntprintf( v, 256, _T(" Грань минус ") ); } break;
#else
      case  IDS_PROP_0612 : { _sntprintf( v, 256, _T(" Face minus ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0613 : { _sntprintf( v, 256, _T(" Указатель на грань") ); } break;
#else
      case  IDS_PROP_0613 : { _sntprintf( v, 256, _T(" Pointer to a face") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0614 : { _sntprintf( v, 256, _T(" Номер по порядку ") ); } break;
#else
      case  IDS_PROP_0614 : { _sntprintf( v, 256, _T(" Number by and index ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0615 : { _sntprintf( v, 256, _T(" Сортировка ") ); } break;
#else
      case  IDS_PROP_0615 : { _sntprintf( v, 256, _T(" Sorting ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0616: { _sntprintf( v, 256, _T( " Пуансон или матрица " ) ); } break;
#else
      case  IDS_PROP_0616: { _sntprintf( v, 256, _T( " A punch or a die " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0617: { _sntprintf( v, 256, _T( " Скруглять острые ребра инструмента. " ) ); } break;
#else
      case  IDS_PROP_0617: { _sntprintf( v, 256, _T( " Fillet sharp edges of tool solid " ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0651 : { _sntprintf( v, 256, _T(" Разрезанное тело ") ); } break;
#else
      case  IDS_PROP_0651 : { _sntprintf( v, 256, _T(" Cutting solid ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0652 : { _sntprintf( v, 256, _T(" Плоскость раскроя ") ); } break;
#else
      case  IDS_PROP_0652 : { _sntprintf( v, 256, _T(" Cutting plane ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0654 : { _sntprintf( v, 256, _T(" Наличие штриховки ") ); } break;
#else
      case  IDS_PROP_0654 : { _sntprintf( v, 256, _T(" Whether there is hatching ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0655 : { _sntprintf( v, 256, _T(" Шаг штриховки ") ); } break;
#else
      case  IDS_PROP_0655 : { _sntprintf( v, 256, _T(" Hatching step ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0656 : { _sntprintf( v, 256, _T(" Угол штриховки ") ); } break;
#else
      case  IDS_PROP_0656 : { _sntprintf( v, 256, _T(" Hatching angle ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0657 : { _sntprintf( v, 256, _T(" Проекционная плоскость") ); } break;
#else
      case  IDS_PROP_0657 : { _sntprintf( v, 256, _T(" Projection plane") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0658 : { _sntprintf( v, 256, _T(" Наличие невидимых линий ") ); } break;
#else
      case  IDS_PROP_0658 : { _sntprintf( v, 256, _T(" Whether there are invisible lines ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0659 : { _sntprintf( v, 256, _T(" Hash имени ") ); } break;
#else
      case  IDS_PROP_0659 : { _sntprintf( v, 256, _T(" Hash of name ") ); } break;
#endif

// \ru Листовой металл \en Sheet metal

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0660 : { _sntprintf( v, 256, _T(" Коэффициент нейтрального слоя ") ); } break;
#else
      case  IDS_PROP_0660 : { _sntprintf( v, 256, _T(" Neutral layer coefficient ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0661 : { _sntprintf( v, 256, _T(" Радиус сгиба ") ); } break;
#else
      case  IDS_PROP_0661 : { _sntprintf( v, 256, _T(" Bend radius ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0662 : { _sntprintf( v, 256, _T(" Угол сгиба ") ); } break;
#else
      case  IDS_PROP_0662 : { _sntprintf( v, 256, _T(" Bend angle ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0663 : { _sntprintf( v, 256, _T(" Длина продолжения сгиба ") ); } break;
#else
      case  IDS_PROP_0663 : { _sntprintf( v, 256, _T(" Bend extension length ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0664 : { _sntprintf( v, 256, _T(" Смещение сгиба ") ); } break;
#else
      case  IDS_PROP_0664 : { _sntprintf( v, 256, _T(" Bend shift ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0665 : { _sntprintf( v, 256, _T(" Отступ от края сгиба 1 ") ); } break;
#else
      case  IDS_PROP_0665 : { _sntprintf( v, 256, _T(" Distance from the bound of bend 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0666 : { _sntprintf( v, 256, _T(" Отступ от края сгиба 2 ") ); } break;
#else
      case  IDS_PROP_0666 : { _sntprintf( v, 256, _T(" Distance from the bound of bend 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0667 : { _sntprintf( v, 256, _T(" Угол уклона края сгиба 1 ") ); } break;
#else
      case  IDS_PROP_0667 : { _sntprintf( v, 256, _T(" Slope angle of the bound of bend 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0668 : { _sntprintf( v, 256, _T(" Угол уклона края сгиба 2 ") ); } break;
#else
      case  IDS_PROP_0668 : { _sntprintf( v, 256, _T(" Slope angle of the bound of bend 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0669 : { _sntprintf( v, 256, _T(" Угол уклона продолжения сгиба 1 ") ); } break;
#else
      case  IDS_PROP_0669 : { _sntprintf( v, 256, _T(" Slope angle of bend extension 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0670 : { _sntprintf( v, 256, _T(" Угол уклона продолжения сгиба 2 ") ); } break;
#else
      case  IDS_PROP_0670 : { _sntprintf( v, 256, _T(" Slope angle of bend extension 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0671 : { _sntprintf( v, 256, _T(" Расширение продолжения сгиба 1 ") ); } break;
#else
      case  IDS_PROP_0671 : { _sntprintf( v, 256, _T(" Expansion of extension of bend 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0672 : { _sntprintf( v, 256, _T(" Расширение продолжения сгиба 2 ") ); } break;
#else
      case  IDS_PROP_0672 : { _sntprintf( v, 256, _T(" Expansion of extension of bend 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0673 : { _sntprintf( v, 256, _T(" Ширина разгрузки сгиба ") ); } break;
#else
      case  IDS_PROP_0673 : { _sntprintf( v, 256, _T(" Width of bend relief") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0674 : { _sntprintf( v, 256, _T(" Глубина разгрузки сгиба ") ); } break;
#else
      case  IDS_PROP_0674 : { _sntprintf( v, 256, _T(" Depth of bend relief ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0675 : { _sntprintf( v, 256, _T(" Радиус скругления разгрузки ") ); } break;
#else
      case  IDS_PROP_0675 : { _sntprintf( v, 256, _T(" Radius of relief rounding ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0676 : { _sntprintf( v, 256, _T(" Способ освобождения углов ") ); } break;
#else
      case  IDS_PROP_0676 : { _sntprintf( v, 256, _T(" Method of freeing the corners ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0677 : { _sntprintf( v, 256, _T(" Фиксированная часть грани слева ") ); } break;
#else
      case  IDS_PROP_0677 : { _sntprintf( v, 256, _T(" Fixed part of a face on the left ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0678 : { _sntprintf( v, 256, _T(" Строить разогнутым ") ); } break;
#else
      case  IDS_PROP_0678 : { _sntprintf( v, 256, _T(" Build in unbent state ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0679 : { _sntprintf( v, 256, _T(" Зазор ") ); } break;
#else
      case  IDS_PROP_0679 : { _sntprintf( v, 256, _T(" Gap ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0680 : { _sntprintf( v, 256, _T(" Перехлёстывающая сторона слева ") ); } break;
#else
      case  IDS_PROP_0680 : { _sntprintf( v, 256, _T(" Overlapping side on the left ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0681 : { _sntprintf( v, 256, _T(" С добавлением материала ") ); } break;
#else
      case  IDS_PROP_0681 : { _sntprintf( v, 256, _T(" With addition of material ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0682 : { _sntprintf( v, 256, _T(" Высота ") ); } break;
#else
      case  IDS_PROP_0682 : { _sntprintf( v, 256, _T(" Height ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0683 : { _sntprintf( v, 256, _T(" Коэффициент сгиба 1 ") ); } break;
#else
      case  IDS_PROP_0683 : { _sntprintf( v, 256, _T(" Coefficient of bend 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0684 : { _sntprintf( v, 256, _T(" Радиус сгиба 1 ") ); } break;
#else
      case  IDS_PROP_0684 : { _sntprintf( v, 256, _T(" Radius of bend 1 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0685 : { _sntprintf( v, 256, _T(" Коэффициент сгиба 2 ") ); } break;
#else
      case  IDS_PROP_0685 : { _sntprintf( v, 256, _T(" Coefficient of bend 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0686 : { _sntprintf( v, 256, _T(" Радиус сгиба 2 ") ); } break;
#else
      case  IDS_PROP_0686 : { _sntprintf( v, 256, _T(" Radius of bend 2 ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0687 : { _sntprintf( v, 256, _T(" Радиус скругления эскиза ") ); } break;
#else
      case  IDS_PROP_0687 : { _sntprintf( v, 256, _T(" Radius of a sketch fillet") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0688 : { _sntprintf( v, 256, _T(" Радиус скругления основания ") ); } break;
#else
      case  IDS_PROP_0688 : { _sntprintf( v, 256, _T(" Radius of a base fillet") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0689 : { _sntprintf( v, 256, _T(" Радиус скругления дна ") ); } break;
#else
      case  IDS_PROP_0689 : { _sntprintf( v, 256, _T(" Radius of a bottom fillet ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0690 : { _sntprintf( v, 256, _T(" Открытая штамповка ") ); } break;
#else
      case  IDS_PROP_0690 : { _sntprintf( v, 256, _T(" Open stamping ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0691 : { _sntprintf( v, 256, _T(" Боковая стенка внутри ") ); } break;
#else
      case  IDS_PROP_0691 : { _sntprintf( v, 256, _T(" Side wall is inside ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0692 : { _sntprintf( v, 256, _T(" Ширина основания ") ); } break;
#else
      case  IDS_PROP_0692 : { _sntprintf( v, 256, _T(" Width of base ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0693 : { _sntprintf( v, 256, _T(" Ширина выпуклой части ") ); } break;
#else
      case  IDS_PROP_0693 : { _sntprintf( v, 256, _T(" Width of a salient part ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0694 : { _sntprintf( v, 256, _T(" Зазор рубленой законцовки ") ); } break;
#else
      case  IDS_PROP_0694 : { _sntprintf( v, 256, _T(" Gap of a cropped tip ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0695 : { _sntprintf( v, 256, _T(" Тип буртика ") ); } break;
#else
      case  IDS_PROP_0695 : { _sntprintf( v, 256, _T(" Bead type ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0696 : { _sntprintf( v, 256, _T(" Тип законцовки ") ); } break;
#else
      case  IDS_PROP_0696 : { _sntprintf( v, 256, _T(" Type of a tip ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0697 : { _sntprintf( v, 256, _T(" Вытяжка ") ); } break;
#else
      case  IDS_PROP_0697 : { _sntprintf( v, 256, _T(" Stretch ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0698 : { _sntprintf( v, 256, _T(" По нормали к толщине ") ); } break;
#else
      case  IDS_PROP_0698 : { _sntprintf( v, 256, _T(" By the normal to thickness ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0699 : { _sntprintf( v, 256, _T(" Способ замыкания цилиндрических частей ") ); } break;
#else
      case  IDS_PROP_0699 : { _sntprintf( v, 256, _T(" Method of cylindric parts closure ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0700 : { _sntprintf( v, 256, _T(" Разрешение на замыкание углов ") ); } break;
#else
      case  IDS_PROP_0700 : { _sntprintf( v, 256, _T(" Permission for corners closure ") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0701 : { _sntprintf( v, 256, _T(" Имя ") ); } break;
#else
      case  IDS_PROP_0701 : { _sntprintf( v, 256, _T(" Name ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0702 : { _sntprintf( v, 256, _T(" Значение ") ); } break;
#else
      case  IDS_PROP_0702 : { _sntprintf( v, 256, _T(" Value ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0703 : { _sntprintf( v, 256, _T(" Положение ") ); } break;
#else
      case  IDS_PROP_0703 : { _sntprintf( v, 256, _T(" Position ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0704 : { _sntprintf( v, 256, _T(" Число ") ); } break;
#else
      case  IDS_PROP_0704 : { _sntprintf( v, 256, _T(" Number ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0705 : { _sntprintf( v, 256, _T(" Ориентация ") ); } break;
#else
      case  IDS_PROP_0705 : { _sntprintf( v, 256, _T(" Orientation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0706 : { _sntprintf( v, 256, _T(" Длина ") ); } break;
#else
      case  IDS_PROP_0706 : { _sntprintf( v, 256, _T(" Length ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0707 : { _sntprintf( v, 256, _T(" Толщина ") ); } break;
#else
      case  IDS_PROP_0707 : { _sntprintf( v, 256, _T(" Thickness ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0708 : { _sntprintf( v, 256, _T(" Угол ") ); } break;
#else
      case  IDS_PROP_0708 : { _sntprintf( v, 256, _T(" Angle ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0709 : { _sntprintf( v, 256, _T(" Параметр ") ); } break;
#else
      case  IDS_PROP_0709 : { _sntprintf( v, 256, _T(" Parameter ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0710 : { _sntprintf( v, 256, _T(" Геометрический объект ") ); } break;
#else
      case  IDS_PROP_0710 : { _sntprintf( v, 256, _T(" Geometric object ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0711 : { _sntprintf( v, 256, _T(" Точка ") ); } break;
#else
      case  IDS_PROP_0711 : { _sntprintf( v, 256, _T(" Point ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0712 : { _sntprintf( v, 256, _T(" Контрольная кривая ") ); } break;
#else
      case  IDS_PROP_0712 : { _sntprintf( v, 256, _T(" Control curve ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0713 : { _sntprintf( v, 256, _T(" Поверхность ") ); } break;
#else
      case  IDS_PROP_0713 : { _sntprintf( v, 256, _T(" Surface ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0714 : { _sntprintf( v, 256, _T(" Вершина ") ); } break;
#else
      case  IDS_PROP_0714 : { _sntprintf( v, 256, _T(" Vertex ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0715 : { _sntprintf( v, 256, _T(" Ребро грани ") ); } break;
#else
      case  IDS_PROP_0715 : { _sntprintf( v, 256, _T(" Edge of a face ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0716 : { _sntprintf( v, 256, _T(" Цикл грани ") ); } break;
#else
      case  IDS_PROP_0716 : { _sntprintf( v, 256, _T(" Face loop ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0717 : { _sntprintf( v, 256, _T(" Грань ") ); } break;
#else
      case  IDS_PROP_0717 : { _sntprintf( v, 256, _T(" Face ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0718 : { _sntprintf( v, 256, _T(" Полюс при umin") ); } break;
#else
      case  IDS_PROP_0718 : { _sntprintf( v, 256, _T(" Pole at umin") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0719 : { _sntprintf( v, 256, _T(" Полюс при umax") ); } break;
#else
      case  IDS_PROP_0719 : { _sntprintf( v, 256, _T(" Pole at umax") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0720 : { _sntprintf( v, 256, _T(" Полюс при vmin") ); } break;
#else
      case  IDS_PROP_0720 : { _sntprintf( v, 256, _T(" Pole at vmin") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0721 : { _sntprintf( v, 256, _T(" Полюс при vmax") ); } break;
#else
      case  IDS_PROP_0721 : { _sntprintf( v, 256, _T(" Pole at vmax") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0724 : { _sntprintf( v, 256, _T(" Вершина ") ); } break;
#else
      case  IDS_PROP_0724 : { _sntprintf( v, 256, _T(" Vertex ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0725 : { _sntprintf( v, 256, _T(" Ребро ") ); } break;
#else
      case  IDS_PROP_0725 : { _sntprintf( v, 256, _T(" Edge ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0726 : { _sntprintf( v, 256, _T(" Цикл ") ); } break;
#else
      case  IDS_PROP_0726 : { _sntprintf( v, 256, _T(" Loop ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0727 : { _sntprintf( v, 256, _T(" Грань ") ); } break;
#else
      case  IDS_PROP_0727 : { _sntprintf( v, 256, _T(" Face ") ); } break;
#endif

      PROMPT_CASE_RU( IDS_PROP_0729, " Количество граней "    );
      PROMPT_CASE_EN( IDS_PROP_0729, " Number of faces "      );

      PROMPT_CASE_RU( IDS_PROP_0730, " Количество операций "  );
      PROMPT_CASE_EN( IDS_PROP_0730, " Number of operations " );

      PROMPT_CASE_RU( IDS_PROP_0731, " Количество объектов "  );
      PROMPT_CASE_EN( IDS_PROP_0731, " Number of objects "    );

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0732 : { _sntprintf( v, 256, _T(" Объединение граней ") ); } break;
#else
      case  IDS_PROP_0732 : { _sntprintf( v, 256, _T(" Faces unification ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0733 : { _sntprintf( v, 256, _T(" Обработка углов ") ); } break;
#else
      case  IDS_PROP_0733 : { _sntprintf( v, 256, _T(" Corners treatment ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0734 : { _sntprintf( v, 256, _T(" Операция объединения ") ); } break;
#else
      case  IDS_PROP_0734 : { _sntprintf( v, 256, _T(" Union operation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0735 : { _sntprintf( v, 256, _T(" Операция пересечения ") ); } break;
#else
      case  IDS_PROP_0735 : { _sntprintf( v, 256, _T(" Intersection operation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0736 : { _sntprintf( v, 256, _T(" Операция разности    ") ); } break;
#else
      case  IDS_PROP_0736 : { _sntprintf( v, 256, _T(" Subtraction operation    ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0737 : { _sntprintf( v, 256, _T(" Базовая операция ") ); } break;
#else
      case  IDS_PROP_0737 : { _sntprintf( v, 256, _T(" Base operations ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0738 : { _sntprintf( v, 256, _T(" Флаг состояния ") ); } break;
#else
      case  IDS_PROP_0738 : { _sntprintf( v, 256, _T(" Flag of state ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0739 : { _sntprintf( v, 256, _T(" Параметр полюса по U ") ); } break;
#else
      case  IDS_PROP_0739 : { _sntprintf( v, 256, _T(" Parameter of a pole by U ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0740 : { _sntprintf( v, 256, _T(" Номер грани ") ); } break;
#else
      case  IDS_PROP_0740 : { _sntprintf( v, 256, _T(" Number of a face ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0741 : { _sntprintf( v, 256, _T(" Номер ребра ") ); } break;
#else
      case  IDS_PROP_0741 : { _sntprintf( v, 256, _T(" Number of an edge ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0742 : { _sntprintf( v, 256, _T(" Номер грани плюс  ") ); } break;
#else
      case  IDS_PROP_0742 : { _sntprintf( v, 256, _T(" Number of a face plus  ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0743 : { _sntprintf( v, 256, _T(" Номер грани минус ") ); } break;
#else
      case  IDS_PROP_0743 : { _sntprintf( v, 256, _T(" Number of a face minus ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0744 : { _sntprintf( v, 256, _T(" Формировать твёрдое тело ") ); } break;
#else
      case  IDS_PROP_0744 : { _sntprintf( v, 256, _T(" Create a solid ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0745 : { _sntprintf( v, 256, _T(" Точность сшивки ") ); } break;
#else
      case  IDS_PROP_0745 : { _sntprintf( v, 256, _T(" Stitching tolerance ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0746 : { _sntprintf( v, 256, _T(" Через сгиб ") ); } break;
#else
      case  IDS_PROP_0746 : { _sntprintf( v, 256, _T(" Through a bend ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0747 : { _sntprintf( v, 256, _T(" Полюс ") ); } break;
#else
      case  IDS_PROP_0747 : { _sntprintf( v, 256, _T(" Pole ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0748 : { _sntprintf( v, 256, _T(" Край ") ); } break;
#else
      case  IDS_PROP_0748 : { _sntprintf( v, 256, _T(" Border ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0749 : { _sntprintf( v, 256, _T(" Шов ") ); } break;
#else
      case  IDS_PROP_0749 : { _sntprintf( v, 256, _T(" Seam ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0750 : { _sntprintf( v, 256, _T(" Линия перехода ") ); } break;
#else
      case  IDS_PROP_0750 : { _sntprintf( v, 256, _T(" Transition line ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0751 : { _sntprintf( v, 256, _T(" Адрес начальной вершины ") ); } break;
#else
      case  IDS_PROP_0751 : { _sntprintf( v, 256, _T(" Start vertex address ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0752 : { _sntprintf( v, 256, _T(" Адрес конечной  вершины ") ); } break;
#else
      case  IDS_PROP_0752 : { _sntprintf( v, 256, _T(" End vertex address ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0753 : { _sntprintf( v, 256, _T(" Адрес грани слева  ") ); } break;
#else
      case  IDS_PROP_0753 : { _sntprintf( v, 256, _T(" Address of a face on the left  ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0754 : { _sntprintf( v, 256, _T(" Адрес грани справа ") ); } break;
#else
      case  IDS_PROP_0754 : { _sntprintf( v, 256, _T(" Address of a face on the right ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0755 : { _sntprintf( v, 256, _T(" Примитив разрезан ") ); } break;
#else
      case  IDS_PROP_0755 : { _sntprintf( v, 256, _T(" Primitive is cut ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0756 : { _sntprintf( v, 256, _T(" Листовой примитив ") ); } break;
#else
      case  IDS_PROP_0756 : { _sntprintf( v, 256, _T(" Sheet primitive ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0757 : { _sntprintf( v, 256, _T(" Внутренняя грань сгиба ") ); } break;
#else
      case  IDS_PROP_0757 : { _sntprintf( v, 256, _T(" Internal face of a bend ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0758 : { _sntprintf( v, 256, _T(" Внешняя грань сгиба ") ); } break;
#else
      case  IDS_PROP_0758 : { _sntprintf( v, 256, _T(" External face of a bend ") ); } break;
#endif


// \ru Версии \en Versions

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0760 : { _sntprintf( v, 256, _T(" Версия ") ); } break;
#else
      case  IDS_PROP_0760 : { _sntprintf( v, 256, _T(" Version ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0761 : { _sntprintf( v, 256, _T(" Версия имени ") ); } break;
#else
      case  IDS_PROP_0761 : { _sntprintf( v, 256, _T(" Version of name ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0762 : { _sntprintf( v, 256, _T(" Версия операции ") ); } break;
#else
      case  IDS_PROP_0762 : { _sntprintf( v, 256, _T(" Version of operation ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0763 : { _sntprintf( v, 256, _T(" Версия объекта ") ); } break;
#else
      case  IDS_PROP_0763 : { _sntprintf( v, 256, _T(" Version of object ") ); } break;
#endif

// \ru Информация от геометрической модели \en Information from geometric model

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0771 : { _sntprintf( v, 256, _T(" Количество вершин ") ); } break;
#else
      case  IDS_PROP_0771 : { _sntprintf( v, 256, _T(" Number of vertices ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0772 : { _sntprintf( v, 256, _T(" Количество кривых ") ); } break;
#else
      case  IDS_PROP_0772 : { _sntprintf( v, 256, _T(" Number of curves ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0773 : { _sntprintf( v, 256, _T(" Количество поверхностей ") ); } break;
#else
      case  IDS_PROP_0773 : { _sntprintf( v, 256, _T(" Number of surfaces ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0774 : { _sntprintf( v, 256, _T(" Количество тел     ") ); } break;
#else
      case  IDS_PROP_0774 : { _sntprintf( v, 256, _T(" Number of solids   ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0775 : { _sntprintf( v, 256, _T(" Количество полигональных объектов ") ); } break;
#else
      case  IDS_PROP_0775 : { _sntprintf( v, 256, _T(" Number of polygonal objects ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0776 : { _sntprintf( v, 256, _T(" Количество проволочных каркасов ") ); } break;
#else
      case  IDS_PROP_0776 : { _sntprintf( v, 256, _T(" Number of wireframes ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0777 : { _sntprintf( v, 256, _T(" Количество точечных каркасов ") ); } break;
#else
      case  IDS_PROP_0777 : { _sntprintf( v, 256, _T(" Number of point frames ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0778 : { _sntprintf( v, 256, _T(" Количество сборочных единиц ") ); } break;
#else
      case  IDS_PROP_0778 : { _sntprintf( v, 256, _T(" Number of assembly units ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0779 : { _sntprintf( v, 256, _T(" Количество вставок ") ); } break;
#else
      case  IDS_PROP_0779 : { _sntprintf( v, 256, _T(" Number of instances ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0780 : { _sntprintf( v, 256, _T(" Количество других объектов ") ); } break;
#else
      case  IDS_PROP_0780 : { _sntprintf( v, 256, _T(" Number of other objects ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0781 : { _sntprintf( v, 256, _T(" Количество регионов ") ); } break;
#else
      case  IDS_PROP_0781 : { _sntprintf( v, 256, _T(" Number of regions ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0782 : { _sntprintf( v, 256, _T(" Количество элементов ") ); } break;
#else
      case  IDS_PROP_0782 : { _sntprintf( v, 256, _T(" Number of elements ") ); } break;
#endif

      PROMPT_CASE_RU( IDS_PROP_0783, " Количество сегментов " );
      PROMPT_CASE_EN( IDS_PROP_0783, " Number of segments " );
      PROMPT_CASE_RU( IDS_PROP_0784, " Количество всех граней" );
      PROMPT_CASE_EN( IDS_PROP_0784, " Number of all faces" );
      PROMPT_CASE_RU( IDS_PROP_0785, " Количество всех уникальных граней" );
      PROMPT_CASE_EN( IDS_PROP_0785, " Number of all unique faces" );

      PROMPT_CASE_RU( IDS_PROP_0791, " Первая деформация" );
      PROMPT_CASE_EN( IDS_PROP_0791, " First strain" );

      PROMPT_CASE_RU( IDS_PROP_0792, " Вторая деформация" );
      PROMPT_CASE_EN( IDS_PROP_0792, " Second strain" );

      PROMPT_CASE_RU( IDS_PROP_0793, " Третья деформация" );
      PROMPT_CASE_EN( IDS_PROP_0793, " Third strain" );

      PROMPT_CASE_RU( IDS_PROP_0797, " Модуль Юнга" );
      PROMPT_CASE_EN( IDS_PROP_0797, " Young's modulus" ); 

      PROMPT_CASE_RU( IDS_PROP_0798, " Коэффициент Пуассона" );
      PROMPT_CASE_EN( IDS_PROP_0798, " Poisson's ratio" );

      /*
        Property strings for C3D Solver
      */

      PROMPT_CASE_RU( IDS_PROP_1100, " Геометрический решатель" );
      PROMPT_CASE_EN( IDS_PROP_1100, " Geom solver" );
      PROMPT_CASE_RU( IDS_PROP_1101, " Схема сопряжений" );
      PROMPT_CASE_EN( IDS_PROP_1101, " Scheme of mates" );
      PROMPT_CASE_RU( IDS_PROP_1102, " Система ограничений" );
      PROMPT_CASE_EN( IDS_PROP_1102, " Constraint system" );
      PROMPT_CASE_RU( IDS_PROP_1103, " Ограничение" );
      PROMPT_CASE_EN( IDS_PROP_1103, " Constraint" );
      PROMPT_CASE_RU( IDS_PROP_1104, " Тип сопряжения" );
      PROMPT_CASE_EN( IDS_PROP_1104, " Type of mate" );
      PROMPT_CASE_RU( IDS_PROP_1105, " Тип ограничения" );
      PROMPT_CASE_EN( IDS_PROP_1105, " Type of constraint" );
      PROMPT_CASE_RU( IDS_PROP_1106, " Выравнивание" );      
      PROMPT_CASE_EN( IDS_PROP_1106, " Alignment" );            
      PROMPT_CASE_RU( IDS_PROP_1107, " Количество ограничений" );
      PROMPT_CASE_EN( IDS_PROP_1107, " Number of constraints" );
      PROMPT_CASE_RU( IDS_PROP_1108, " Тип взаимоориентации" );
      PROMPT_CASE_EN( IDS_PROP_1108, " Coorientation type" );
      PROMPT_CASE_RU( IDS_PROP_1109, " Сопряжение" );
      PROMPT_CASE_EN( IDS_PROP_1109, " Mate" );
      PROMPT_CASE_RU( IDS_PROP_1110, " Базовый объект" );
      PROMPT_CASE_EN( IDS_PROP_1110, " Base object" );
      PROMPT_CASE_RU( IDS_PROP_1111, " Объект 1" );
      PROMPT_CASE_EN( IDS_PROP_1111, " Object 1" );
      PROMPT_CASE_RU( IDS_PROP_1112, " Объект 2" );
      PROMPT_CASE_EN( IDS_PROP_1112, " Object 2" );
      PROMPT_CASE_RU( IDS_PROP_1113, " Вещественный параметр" );
      PROMPT_CASE_EN( IDS_PROP_1113, " Real parameter" );
      PROMPT_CASE_RU( IDS_PROP_1114, " Величина взаимоориентации" );
      PROMPT_CASE_EN( IDS_PROP_1114, " Value of coorientation" );
      
      /*
        Geometric constraint types
      */
      PROMPT_CASE_RU( IDS_PROP_1140, "Радиусный размер" );
      PROMPT_CASE_EN( IDS_PROP_1140, "Radial dimension" );
      PROMPT_CASE_RU( IDS_PROP_1130, "Совпадение" );
      PROMPT_CASE_EN( IDS_PROP_1130, "Coincident" );
      PROMPT_CASE_RU( IDS_PROP_1131, "Параллельность" );
      PROMPT_CASE_EN( IDS_PROP_1131, "Parallel" );
      PROMPT_CASE_RU( IDS_PROP_1132, "Перпендикулярность" );
      PROMPT_CASE_EN( IDS_PROP_1132, "Perpendicular" );
      PROMPT_CASE_RU( IDS_PROP_1133, "Касание" );
      PROMPT_CASE_EN( IDS_PROP_1133, "Tangent" );
      PROMPT_CASE_RU( IDS_PROP_1134, "Концентричность" );
      PROMPT_CASE_EN( IDS_PROP_1134, "Concentric" );
      PROMPT_CASE_RU( IDS_PROP_1135, "На расстоянии" );
      PROMPT_CASE_EN( IDS_PROP_1135, "Distance" );
      PROMPT_CASE_RU( IDS_PROP_1136, "По углом" );
      PROMPT_CASE_EN( IDS_PROP_1136, "Angle" );
      PROMPT_CASE_RU( IDS_PROP_1137, "По месту" );
      PROMPT_CASE_EN( IDS_PROP_1137, "In place" );
      PROMPT_CASE_RU( IDS_PROP_1138, "Механическая передача" );
      PROMPT_CASE_EN( IDS_PROP_1138, "Transmittion" );
      PROMPT_CASE_RU( IDS_PROP_1139, "Кулачковый механизм" );
      PROMPT_CASE_EN( IDS_PROP_1139, "Cam mechanism" );
      PROMPT_CASE_RU( IDS_PROP_1145, "Симметричность" );
      PROMPT_CASE_EN( IDS_PROP_1145, "Symmetric" );
      PROMPT_CASE_RU( IDS_PROP_1146, "Зависимый объект");
      PROMPT_CASE_EN( IDS_PROP_1146, "Dependent" );
      PROMPT_CASE_RU( IDS_PROP_1147, "Элемент паттерна" );
      PROMPT_CASE_EN( IDS_PROP_1147, "Patterned" );
      PROMPT_CASE_RU( IDS_PROP_1148, "Линейный паттерн" );
      PROMPT_CASE_EN( IDS_PROP_1148, "Linear pattern" );
      PROMPT_CASE_RU( IDS_PROP_1149, "Угловой паттерн" );
      PROMPT_CASE_EN( IDS_PROP_1149, "Angular pattern" );
      PROMPT_CASE_RU( IDS_PROP_1157, "Опция растяжения");
      PROMPT_CASE_EN( IDS_PROP_1157, "Scale option");
      PROMPT_CASE_RU( IDS_PROP_1158, "Паттерн");
      PROMPT_CASE_EN( IDS_PROP_1158, "Pattern");

// \ru Мультилиния \en Multiline

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0830 : { _sntprintf( v, 256, _T(" Число радиусов эквидистант") ); } break;
#else
      case  IDS_PROP_0830 : { _sntprintf( v, 256, _T(" Number of offsets radii") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0831 : { _sntprintf( v, 256, _T(" Гладкий стык") ); } break;
#else
      case  IDS_PROP_0831 : { _sntprintf( v, 256, _T(" Smooth joint") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0832 : { _sntprintf( v, 256, _T(" Тип обхода угла") ); } break;
#else
      case  IDS_PROP_0832 : { _sntprintf( v, 256, _T(" Type of corner bypass") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0833 : { _sntprintf( v, 256, _T(" Радиус специального скругления") ); } break;
#else
      case  IDS_PROP_0833 : { _sntprintf( v, 256, _T(" Radius of a special fillet") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0834 : { _sntprintf( v, 256, _T(" Тип законцовки") ); } break;
#else
      case  IDS_PROP_0834 : { _sntprintf( v, 256, _T(" Type of a tip") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0835 : { _sntprintf( v, 256, _T(" Законцовка первого сегмента") ); } break;
#else
      case  IDS_PROP_0835 : { _sntprintf( v, 256, _T(" Tip of the first segment") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0836 : { _sntprintf( v, 256, _T(" Тип законцовки в начале") ); } break;
#else
      case  IDS_PROP_0836 : { _sntprintf( v, 256, _T(" Type of tip at the beginning") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0837 : { _sntprintf( v, 256, _T(" Тип законцовки в конце") ); } break;
#else
      case  IDS_PROP_0837 : { _sntprintf( v, 256, _T(" Type of tip at the end") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0838 : { _sntprintf( v, 256, _T(" Параметр законцовки") ); } break;
#else
      case  IDS_PROP_0838 : { _sntprintf( v, 256, _T(" Parameter of a tip") ); } break;
#endif

// \ru Поверхностное моделирование \en Surface modeling

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0839 : { _sntprintf( v, 256, _T(" Параметр построения заплатки") ); } break;
#else
      case  IDS_PROP_0839 : { _sntprintf( v, 256, _T(" Parameter of a patch construction") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0840 : { _sntprintf( v, 256, _T(" Число кривых образующего контура") ); } break;
#else
      case  IDS_PROP_0840 : { _sntprintf( v, 256, _T(" Number of curves of a generating contour") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0841 : { _sntprintf( v, 256, _T(" Даны образующие грани") ); } break;
#else
      case  IDS_PROP_0841 : { _sntprintf( v, 256, _T(" The generating faces are given") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0842 : { _sntprintf( v, 256, _T(" Кривая образующего контура") ); } break;
#else
      case  IDS_PROP_0842 : { _sntprintf( v, 256, _T(" Curve of a generaing contour") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0843 : { _sntprintf( v, 256, _T(" Ориентация кривой контура") ); } break;
#else
      case  IDS_PROP_0843 : { _sntprintf( v, 256, _T(" Orientation of a curve of the contour") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0844 : { _sntprintf( v, 256, _T(" Сторона существующей грани оболочки") ); } break;
#else
      case  IDS_PROP_0844 : { _sntprintf( v, 256, _T(" Side of an existent face of the shell") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0845 : { _sntprintf( v, 256, _T("ДА") ); } break;
#else
      case  IDS_PROP_0845 : { _sntprintf( v, 256, _T("YES") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0846 : { _sntprintf( v, 256, _T("НЕТ") ); } break;
#else
      case  IDS_PROP_0846 : { _sntprintf( v, 256, _T("NO") ); } break;
#endif

// \ru Контейнер атрибутов \en Attribute container

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0847 : { _sntprintf( v, 256, _T(" Поставщик атрибутов") ); } break;
#else
      case  IDS_PROP_0847 : { _sntprintf( v, 256, _T(" Attribute provider") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0848 : { _sntprintf( v, 256, _T(" Количество контейнеров") ); } break;
#else
      case  IDS_PROP_0848 : { _sntprintf( v, 256, _T(" Number of containers") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0849 : { _sntprintf( v, 256, _T(" Контейнер ") ); } break;
#else
      case  IDS_PROP_0849 : { _sntprintf( v, 256, _T(" Container ") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0851 : { _sntprintf( v, 256, _T(" Количество атрибутов") ); } break;
#else
      case  IDS_PROP_0851 : { _sntprintf( v, 256, _T(" Number of attributes") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0853 : { _sntprintf( v, 256, _T(" При изменении") ); } break;
#else
      case  IDS_PROP_0853 : { _sntprintf( v, 256, _T(" While changing") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0854 : { _sntprintf( v, 256, _T(" При конвертации") ); } break;
#else
      case  IDS_PROP_0854 : { _sntprintf( v, 256, _T(" While convertation") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0855 : { _sntprintf( v, 256, _T(" При трансформировании") ); } break;
#else
      case  IDS_PROP_0855 : { _sntprintf( v, 256, _T(" While transforming") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0856 : { _sntprintf( v, 256, _T(" При копировании") ); } break;
#else
      case  IDS_PROP_0856 : { _sntprintf( v, 256, _T(" While copying") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0857 : { _sntprintf( v, 256, _T(" При объединении") ); } break;
#else
      case  IDS_PROP_0857 : { _sntprintf( v, 256, _T(" While joining") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0858 : { _sntprintf( v, 256, _T(" При замене") ); } break;
#else
      case  IDS_PROP_0858 : { _sntprintf( v, 256, _T(" While replacing") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0859 : { _sntprintf( v, 256, _T(" При разделении") ); } break;
#else
      case  IDS_PROP_0859 : { _sntprintf( v, 256, _T(" While splitting") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0860 : { _sntprintf( v, 256, _T(" При удалении") ); } break;
#else
      case  IDS_PROP_0860 : { _sntprintf( v, 256, _T(" While deleting") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0861 : { _sntprintf( v, 256, _T(" Свободность") ); } break; // \ru на gramota.ru такого слова нет \en there no such word on gramota.ru
#else
      case  IDS_PROP_0861 : { _sntprintf( v, 256, _T(" Freeness") ); } break; // \ru на gramota.ru такого слова нет \en there no such word on gramota.ru
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0862 : { _sntprintf( v, 256, _T(" Копируемость") ); } break;
#else
      case  IDS_PROP_0862 : { _sntprintf( v, 256, _T(" Whether it can be copied") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0863 : { _sntprintf( v, 256, _T(" Количество u-линий") ); } break;
#else
      case  IDS_PROP_0863 : { _sntprintf( v, 256, _T(" The number of u-lines") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0864 : { _sntprintf( v, 256, _T(" Количество v-линий") ); } break;
#else
      case  IDS_PROP_0864 : { _sntprintf( v, 256, _T(" The number of v-lines") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0869 : { _sntprintf( v, 256, _T(" Имя исполнения ") ); } break;
#else
      case  IDS_PROP_0869 : { _sntprintf( v, 256, _T(" Embodiment name ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0870 : { _sntprintf( v, 256, _T(" Имя родительского исполнения ") ); } break;
#else
      case  IDS_PROP_0870 : { _sntprintf( v, 256, _T(" Parent embodiment name ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0871 : { _sntprintf( v, 256, _T(" Красный ") ); } break;
#else
      case  IDS_PROP_0871 : { _sntprintf( v, 256, _T(" Red ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0872 : { _sntprintf( v, 256, _T(" Зелёный ") ); } break;
#else
      case  IDS_PROP_0872 : { _sntprintf( v, 256, _T(" Green ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0873 : { _sntprintf( v, 256, _T(" Синий   ") ); } break;
#else
      case  IDS_PROP_0873 : { _sntprintf( v, 256, _T(" Blue   ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0874 : { _sntprintf( v, 256, _T(" Толщина") ); } break;
#else
      case  IDS_PROP_0874 : { _sntprintf( v, 256, _T(" Thickness") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0875 : { _sntprintf( v, 256, _T(" Стиль") ); } break;
#else
      case  IDS_PROP_0875 : { _sntprintf( v, 256, _T(" Slyle") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0876 : { _sntprintf( v, 256, _T(" Плотность") ); } break;
#else
      case  IDS_PROP_0876 : { _sntprintf( v, 256, _T(" Density") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0877 : { _sntprintf( v, 256, _T(" Идентификатор") ); } break;
#else
      case  IDS_PROP_0877 : { _sntprintf( v, 256, _T(" Identifier") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0878 : { _sntprintf( v, 256, _T(" Селектированность") ); } break;
#else
      case  IDS_PROP_0878 : { _sntprintf( v, 256, _T(" Selectivity") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0879 : { _sntprintf( v, 256, _T(" Видимость") ); } break;
#else
      case  IDS_PROP_0879 : { _sntprintf( v, 256, _T(" Visibility") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0880 : { _sntprintf( v, 256, _T(" Изменённость") ); } break;
#else
      case  IDS_PROP_0880 : { _sntprintf( v, 256, _T(" Modified") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0881 : { _sntprintf( v, 256, _T(" Общий фон") ); } break;
#else
      case  IDS_PROP_0881 : { _sntprintf( v, 256, _T(" Background") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0882 : { _sntprintf( v, 256, _T(" Диффузное отражение (red)") ); } break;
#else
      case  IDS_PROP_0882 : { _sntprintf( v, 256, _T(" Diffuse reflection (red)") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0892: { _sntprintf( v, 256, _T(" Диффузное отражение (green)") ); } break;
#else
      case  IDS_PROP_0892: { _sntprintf( v, 256, _T(" Diffuse reflection (green)") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0893: { _sntprintf( v, 256, _T(" Диффузное отражение (blue)") ); } break;
#else
      case  IDS_PROP_0893: { _sntprintf( v, 256, _T(" Diffuse reflection (blue)") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0883 : { _sntprintf( v, 256, _T(" Зеркальное отражение") ); } break;
#else
      case  IDS_PROP_0883 : { _sntprintf( v, 256, _T(" Specular reflection") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0895: { _sntprintf( v, 256, _T(" Зеркальное отражение объекта") ); } break;
#else
      case  IDS_PROP_0895: { _sntprintf( v, 256, _T(" Specular reflection for object") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0884 : { _sntprintf( v, 256, _T(" Блеск") ); } break;
#else
      case  IDS_PROP_0884 : { _sntprintf( v, 256, _T(" Shininess") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0885 : { _sntprintf( v, 256, _T(" Непрозрачность") ); } break;
#else
      case  IDS_PROP_0885 : { _sntprintf( v, 256, _T(" Opacity") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0886 : { _sntprintf( v, 256, _T(" Излучение") ); } break;
#else
      case  IDS_PROP_0886 : { _sntprintf( v, 256, _T(" Emission") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0887 : { _sntprintf( v, 256, _T(" Количество родительских объектов ") ); } break;
#else
      case  IDS_PROP_0887 : { _sntprintf( v, 256, _T(" Number of parent objects ") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0888 : { _sntprintf( v, 256, _T(" Родительский объект ") ); } break;
#else
      case  IDS_PROP_0888 : { _sntprintf( v, 256, _T(" Parent object ") ); } break;
#endif
      PROMPT_CASE_RU( IDS_ITEM_0795, " Атрибут хот-точки" );
      PROMPT_CASE_EN( IDS_ITEM_0795, " Hot-point attribute" );

      PROMPT_CASE_RU( IDS_PROP_0889, " Имя топологического объекта" );
      PROMPT_CASE_EN( IDS_PROP_0889, " Topological object name" );

      PROMPT_CASE_RU( IDS_PROP_0890, " Имя объекта" );
      PROMPT_CASE_EN( IDS_PROP_0890, " Object name" );

      PROMPT_CASE_RU( IDS_PROP_0891, " Хэш имени объекта" );
      PROMPT_CASE_EN( IDS_PROP_0891, " Object name hash" );

// \ru Сопряжения в точке, типы параметризации \en Conjugations at point, types of parametrization

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0900 : { _sntprintf( v, 256, _T(" Сопряжение в точке") ); } break;
#else
      case  IDS_PROP_0900 : { _sntprintf( v, 256, _T(" Conjugation at point") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0901 : { _sntprintf( v, 256, _T(" Тип сопряжения:") ); } break;
#else
      case  IDS_PROP_0901 : { _sntprintf( v, 256, _T(" Conjugation type:") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0902 : { _sntprintf( v, 256, _T(" Без сопряжения") ); } break;
#else
      case  IDS_PROP_0902 : { _sntprintf( v, 256, _T(" Without conjugation") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0903 : { _sntprintf( v, 256, _T(" По позиции") ); } break;
#else
      case  IDS_PROP_0903 : { _sntprintf( v, 256, _T(" By position") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0904 : { _sntprintf( v, 256, _T(" По касательной") ); } break;
#else
      case  IDS_PROP_0904 : { _sntprintf( v, 256, _T(" By tangent") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0905 : { _sntprintf( v, 256, _T(" По нормали") ); } break;
#else
      case  IDS_PROP_0905 : { _sntprintf( v, 256, _T(" By normal") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0906 : { _sntprintf( v, 256, _T(" По G2") ); } break;
#else
      case  IDS_PROP_0906 : { _sntprintf( v, 256, _T(" By G2") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0907 : { _sntprintf( v, 256, _T(" По G3") ); } break;
#else
      case  IDS_PROP_0907 : { _sntprintf( v, 256, _T(" By G3") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0908 : { _sntprintf( v, 256, _T(" Касательный вектор") ); } break;
#else
      case  IDS_PROP_0908 : { _sntprintf( v, 256, _T(" Tangent vector") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0909 : { _sntprintf( v, 256, _T(" Первая производная касательного вектора") ); } break;
#else
      case  IDS_PROP_0909 : { _sntprintf( v, 256, _T(" First derivative of tangent vector") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0910 : { _sntprintf( v, 256, _T(" Вторая производная касательного вектора") ); } break;
#else
      case  IDS_PROP_0910 : { _sntprintf( v, 256, _T(" Second derivative of tangent vector") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0911 : { _sntprintf( v, 256, _T(" Можно ли двигать точки") ); } break;
#else
      case  IDS_PROP_0911 : { _sntprintf( v, 256, _T(" Whether points can be moved") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0912 : { _sntprintf( v, 256, _T(" Только по направлению") ); } break;
#else
      case  IDS_PROP_0912 : { _sntprintf( v, 256, _T(" Only along the direction") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0913 : { _sntprintf( v, 256, _T(" Сопряжение ") ); } break;
#else
      case  IDS_PROP_0913 : { _sntprintf( v, 256, _T(" Conjugation ") ); } break;
#endif

#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0920 : { _sntprintf( v, 256, _T(" Тип параметризации:") ); } break;
#else
      case  IDS_PROP_0920 : { _sntprintf( v, 256, _T(" Parametrization type:") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0921 : { _sntprintf( v, 256, _T(" Пользовательская") ); } break;
#else
      case  IDS_PROP_0921 : { _sntprintf( v, 256, _T(" Custom") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0922 : { _sntprintf( v, 256, _T(" Равномерная") ); } break;
#else
      case  IDS_PROP_0922 : { _sntprintf( v, 256, _T(" Uniform") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0923 : { _sntprintf( v, 256, _T(" По длине хорды") ); } break;
#else
      case  IDS_PROP_0923 : { _sntprintf( v, 256, _T(" By chord length") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0924 : { _sntprintf( v, 256, _T(" Центростремительная") ); } break;
#else
      case  IDS_PROP_0924 : { _sntprintf( v, 256, _T(" Centripetal") ); } break;
#endif
// \ru Сопряжения \en Conjugations
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0925 : { _sntprintf( v, 256, _T(" Через вершины") ); } break;
#else
      case  IDS_PROP_0925 : { _sntprintf( v, 256, _T(" Through vertices") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0926 : { _sntprintf( v, 256, _T(" Равномерная параметризация") ); } break;
#else
      case  IDS_PROP_0926 : { _sntprintf( v, 256, _T(" Uniform parametrization") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0927 : { _sntprintf( v, 256, _T(" Текущее исполнение") ); } break;
#else
      case  IDS_PROP_0927 : { _sntprintf( v, 256, _T(" Current embodiment") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0928 : { _sntprintf( v, 256, _T(" Упрощение граней") ); } break;
#else
      case  IDS_PROP_0928 : { _sntprintf( v, 256, _T(" Faces simplification") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0929 : { _sntprintf( v, 256, _T(" Разделять оболочку на грани") ); } break;
#else
      case  IDS_PROP_0929 : { _sntprintf( v, 256, _T(" Divide the shell into faces") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0930 : { _sntprintf( v, 256, _T(" Допустимое несовпадение") ); } break;
#else
      case  IDS_PROP_0930 : { _sntprintf( v, 256, _T(" Permissible mismatch") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0931 : { _sntprintf( v, 256, _T(" Совпадение направлений") ); } break;
#else
      case  IDS_PROP_0931 : { _sntprintf( v, 256, _T(" Directions matching") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0932 : { _sntprintf( v, 256, _T(" Исходный проволочный каркас") ); } break;
#else
      case  IDS_PROP_0932 : { _sntprintf( v, 256, _T(" Initial wire frame") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0933 : { _sntprintf( v, 256, _T(" Строгое построения уклона 1") ); } break;
#else
      case  IDS_PROP_0933 : { _sntprintf( v, 256, _T(" Strict construction a slope 1") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0934 : { _sntprintf( v, 256, _T(" Строгое построения уклона 2") ); } break;
#else
      case  IDS_PROP_0934 : { _sntprintf( v, 256, _T(" Strict construction a slope 2") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_0935 : { _sntprintf( v, 256, _T(" Направляющий проволочный каркас") ); } break;
#else
      case  IDS_PROP_0935 : { _sntprintf( v, 256, _T(" Guide wire frame") ); } break;
#endif

// \ru Строитель оболочки на поверхности переменного сечения.  \en Constructor of the shell on mutable section surface.
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_2032: { _sntprintf( v, 256, _T(" Число индексов частей скругления") ); } break;
#else
      case  IDS_PROP_2032: { _sntprintf( v, 256, _T(" Number of indices of fillet parts") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_2033: { _sntprintf( v, 256, _T(" Индекс части скругления") ); } break;
#else
      case  IDS_PROP_2033: { _sntprintf( v, 256, _T(" Index of fillet part") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_2034: { _sntprintf( v, 256, _T(" Опция выбора результата") ); } break;
#else
      case  IDS_PROP_2034: { _sntprintf( v, 256, _T(" Option for the result selection") ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_2035: { _sntprintf( v, 256, _T( " Режим адаптации" ) ); } break;
#else
      case  IDS_PROP_2035: { _sntprintf( v, 256, _T( " Adaptation mode" ) ); } break;
#endif
#if defined ( __NATIVE_LANGUAGE__ )
      case  IDS_PROP_2036: { _sntprintf( v, 256, _T( " Поверхности, параллельные проецированию" ) ); } break;
#else
      case  IDS_PROP_2036: { _sntprintf( v, 256, _T( " Surfaces parallel to projection" ) ); } break;
#endif

// \ru Конвертеры \en Converters

      PROMPT_CASE_RU( IDS_PROP_1000, " Заголовок" );
      PROMPT_CASE_EN( IDS_PROP_1000, " Title" );
      PROMPT_CASE_RU( IDS_PROP_1001, " Название" );
      PROMPT_CASE_EN( IDS_PROP_1001, " Name" );
      PROMPT_CASE_RU( IDS_PROP_1002, " Дата и время" );
      PROMPT_CASE_EN( IDS_PROP_1002, " Date and time" );
      PROMPT_CASE_RU( IDS_PROP_1003, " Автор(ы)" );
      PROMPT_CASE_EN( IDS_PROP_1003, " Author(s))" );
      PROMPT_CASE_RU( IDS_PROP_1004, " Организация(и)" );
      PROMPT_CASE_EN( IDS_PROP_1004, " Organization(s)" );
      PROMPT_CASE_RU( IDS_PROP_1005, " Процессор STEP" );
      PROMPT_CASE_EN( IDS_PROP_1005, " Processor STEP" );
      PROMPT_CASE_RU( IDS_PROP_1006, " Система" );
      PROMPT_CASE_EN( IDS_PROP_1006, " System" );
      PROMPT_CASE_RU( IDS_PROP_1007, " Авторизация" );
      PROMPT_CASE_EN( IDS_PROP_1007, " Authorization" );

      PROMPT_CASE_RU( IDS_PROP_1010, " Лицо и организация" );
      PROMPT_CASE_EN( IDS_PROP_1010, " Person and organization" );
      PROMPT_CASE_RU( IDS_PROP_1011, " Идентификатор лица" );
      PROMPT_CASE_EN( IDS_PROP_1011, " Identifier of a person" );
      PROMPT_CASE_RU( IDS_PROP_1012, " Фамилия" );
      PROMPT_CASE_EN( IDS_PROP_1012, " Surname" );
      PROMPT_CASE_RU( IDS_PROP_1013, " Имя" );
      PROMPT_CASE_EN( IDS_PROP_1013, " Name" );
      PROMPT_CASE_RU( IDS_PROP_1014, " Средние имена" );
      PROMPT_CASE_EN( IDS_PROP_1014, " Middle names" );
      PROMPT_CASE_RU( IDS_PROP_1015, " Титулы предшествующие" );
      PROMPT_CASE_EN( IDS_PROP_1015, " Prefix titles" );
      PROMPT_CASE_RU( IDS_PROP_1016, " Титулы завершающие" );
      PROMPT_CASE_EN( IDS_PROP_1016, " Suffix titles" );
      PROMPT_CASE_RU( IDS_PROP_1017, " Идентификатор организации" );
      PROMPT_CASE_EN( IDS_PROP_1017, " Identifier of organization" );
      PROMPT_CASE_RU( IDS_PROP_1018, " Название организации" );
      PROMPT_CASE_EN( IDS_PROP_1018, " Name of organization" );
      PROMPT_CASE_RU( IDS_PROP_1019, " Описание организации" );
      PROMPT_CASE_EN( IDS_PROP_1019, " Description of organization" );

      PROMPT_CASE_RU( IDS_PROP_1030, " Изделие" );
      PROMPT_CASE_EN( IDS_PROP_1030, " Product" );
      PROMPT_CASE_RU( IDS_PROP_1031, " Идентификатор" );
      PROMPT_CASE_EN( IDS_PROP_1031, " Identifier" );
      PROMPT_CASE_RU( IDS_PROP_1032, " Название" );
      PROMPT_CASE_EN( IDS_PROP_1032, " Name" );
      PROMPT_CASE_RU( IDS_PROP_1033, " Описание" );
      PROMPT_CASE_EN( IDS_PROP_1033, " Description" );

      PROMPT_CASE_RU( IDS_PROP_1043, " Элемент описания" );
      PROMPT_CASE_EN( IDS_PROP_1043, " Description element" );

// \ru Моделирование \en Modeler

      PROMPT_CASE_RU( IDS_PROP_2001, " Внимание: " );
      PROMPT_CASE_EN( IDS_PROP_2001, " Attention: " );

      PROMPT_CASE_RU( IDS_PROP_2002, " Начало общих операций " );
      PROMPT_CASE_EN( IDS_PROP_2002, " Beginning of the shared operations " );
      PROMPT_CASE_RU( IDS_PROP_2003, " Начало группы операций " );
      PROMPT_CASE_EN( IDS_PROP_2003, " Beginning of the operations group " );
      PROMPT_CASE_RU( IDS_PROP_2004, " Начало первой группы операций " );
      PROMPT_CASE_EN( IDS_PROP_2004, " Beginning of the first operations group " );
      PROMPT_CASE_RU( IDS_PROP_2005, " Начало второй группы операций " );
      PROMPT_CASE_EN( IDS_PROP_2005, " Beginning of the second operations group " );

      PROMPT_CASE_RU( IDS_PROP_2006, " Начало объекта " );
      PROMPT_CASE_EN( IDS_PROP_2006, " Beginning of an object " );
      PROMPT_CASE_RU( IDS_PROP_2007, " Начало первого объекта " );
      PROMPT_CASE_EN( IDS_PROP_2007, " Beginning of the first object " );
      PROMPT_CASE_RU( IDS_PROP_2008, " Начало второго объекта " );
      PROMPT_CASE_EN( IDS_PROP_2008, " Beginning of the second object " );

      PROMPT_CASE_RU( IDS_PROP_2009, " Копировать атрибуты " );
      PROMPT_CASE_EN( IDS_PROP_2009, " Copy attributes " );

      PROMPT_CASE_RU( IDS_PROP_2010, " Количество общих групп операций " );
      PROMPT_CASE_EN( IDS_PROP_2010, " Number of shared operations groups " );

      PROMPT_CASE_RU( IDS_PROP_2011, " Количество выбранных граней " );
      PROMPT_CASE_EN( IDS_PROP_2011, " Number of selected faces " );
      PROMPT_CASE_RU( IDS_PROP_2012, " Количество выбранных ребер " );
      PROMPT_CASE_EN( IDS_PROP_2012, " Number of selected edges " );
      PROMPT_CASE_RU( IDS_PROP_2013, " Количество выбранных вершин " );
      PROMPT_CASE_EN( IDS_PROP_2013, " Number of selected vertices " );

      PROMPT_CASE_RU( IDS_PROP_2014, " Секущий эскиз " );
      PROMPT_CASE_EN( IDS_PROP_2014, " Cutting sketch " );
      PROMPT_CASE_RU( IDS_PROP_2015, " Секущие 3D-кривые " );
      PROMPT_CASE_EN( IDS_PROP_2015, " Cutting 3D-curves " );
      PROMPT_CASE_RU( IDS_PROP_2016, " Секущая поверхности " );
      PROMPT_CASE_EN( IDS_PROP_2016, " Cutting surfaces " );
      PROMPT_CASE_RU( IDS_PROP_2017, " Секущее тело " );
      PROMPT_CASE_EN( IDS_PROP_2017, " Cutting solid " );

      PROMPT_CASE_RU( IDS_PROP_2018, " Поверхность сопряжения на границе 0 " );
      PROMPT_CASE_EN( IDS_PROP_2018, " Adjacent surface on the border 0 " );
      PROMPT_CASE_RU( IDS_PROP_2019, " Поверхность сопряжения на границе 1 " );
      PROMPT_CASE_EN( IDS_PROP_2019, " Adjacent surface on the border 1 " );
      PROMPT_CASE_RU( IDS_PROP_2020, " Поверхность сопряжения на границе 2 " );
      PROMPT_CASE_EN( IDS_PROP_2020, " Adjacent surface on the border 2 " );
      PROMPT_CASE_RU( IDS_PROP_2021, " Поверхность сопряжения на границе 3 " );
      PROMPT_CASE_EN( IDS_PROP_2021, " Adjacent surface on the border 3 " );
      PROMPT_CASE_RU( IDS_PROP_2022, " Поверхность сопряжения на границе 4 " );
      PROMPT_CASE_EN( IDS_PROP_2022, " Adjacent surface on the border 4 " );
      PROMPT_CASE_RU( IDS_PROP_2023, " Тип сопряжения на границе 0 = " );
      PROMPT_CASE_EN( IDS_PROP_2023, " Conjugation type on the boundary 0 = " );
      PROMPT_CASE_RU( IDS_PROP_2024, " Тип сопряжения на границе 1 = " );
      PROMPT_CASE_EN( IDS_PROP_2024, " Conjugation type on the boundary 1 = " );
      PROMPT_CASE_RU( IDS_PROP_2025, " Тип сопряжения на границе 2 = " );
      PROMPT_CASE_EN( IDS_PROP_2025, " Conjugation type on the boundary 2 = " );
      PROMPT_CASE_RU( IDS_PROP_2026, " Тип сопряжения на границе 3 = " );
      PROMPT_CASE_EN( IDS_PROP_2026, " Conjugation type on the boundary 3 = " );
      PROMPT_CASE_RU( IDS_PROP_2027, " Тип сопряжения на границе 4 = " );
      PROMPT_CASE_EN( IDS_PROP_2027, " Conjugation type on the boundary 4 = " );
      PROMPT_CASE_RU( IDS_PROP_2028, " Число ребер сгиба " );
      PROMPT_CASE_EN( IDS_PROP_2028, " Count of bend edges " );
      PROMPT_CASE_RU( IDS_PROP_2029, " Число ребер стыка " );
      PROMPT_CASE_EN( IDS_PROP_2029, " Count of rip edges " );

      default :             { 
        if ( p > IDS_PROP_LAST ) {
          PROMPT_MESSAGE_RU( " Пользовательские данные" );
          PROMPT_MESSAGE_EN( " User data" );
        }
        else {
          PRECONDITION( false );
          PROMPT_MESSAGE_RU( " Неопознанный объект" );
          PROMPT_MESSAGE_EN( " Unidentified object" );
        }
      } break;
    }
  }
}


#endif // __TEST_PROPERTY_TITLE_H
