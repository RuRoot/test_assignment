////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Класс для вписывания поверхности в полигональный объект.
         \en Class for fitting surfaces to a polygonal object. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURFACE_FITTING_H
#define __SURFACE_FITTING_H

#include <mb_operation_result.h>
#include <math_define.h>

class MbSurfaceFitToGridParameters;
class MbSurfaceFitToGridResults;
class MbGrid;
class MbPolymeshTopo;


//------------------------------------------------------------------------------
/** \brief \ru Класс для вписывания поверхности в полигональный объект.
           \en Class for fitting surfaces to a polygonal object. \~
  \details \ru Класс для вписывания поверхности в полигональный объект.
               Выполняет аппроксимацию заданного полигонального объекта или его сегментов поверхностями определенного типа.
               Можно вписать аналитическую поверхность (плоскость, цилиндр, сферу, конус или тор),
               поверхность движения (вращения или выдавливания) и NURBS поверхность.
               Автоматическое определение типа позволяет вписать аналитическую поверхность, которая подходит лучшим образом.
               Для вписывания в часть полигонального объекта выбранные треугольники/фасеты и регионы должны быть указаны
               при создании экземпляра класса.
           \en Class for fitting surfaces to a polygonal object.
               Approximates a given polygonal object or its segment to a surface of a given type.
               There can be fit an analitical surface (plane, cylinder, sphere, cone or torus),
               a swept surface (revolution or extrision) and a NURBS surface.\
               During automatic fitting the type of an analitical surface is defined automatically.
               When fitting to the part of a polygonal object selected triangles/facets or regions have to be defined
               while creating the class instance. \~
  \ingroup Algorithms_3D
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbSurfaceFitter
{
protected:
  /// \ru Конструктор. \en Constructor.
  MbSurfaceFitter();

public:
  /// \ru Деструктор. \en Destructor.
  virtual ~MbSurfaceFitter() {}

public:
  /// \ru Создать объект на основе триангуляции. \en Create an object based on a triangulation.
  static MbSurfaceFitter * Create( const MbGrid & grid, const c3d::IndicesVector * indices = nullptr );
  /// \ru Создать объект на основе топологии полигонального объекта. \en Create an object based on a topology of a polygonal object.
  static MbSurfaceFitter * Create( const MbPolymeshTopo & mesh );
  /// \ru Создать объект на основе набора фасетов полигонального объекта. \en Create an object based on given facets of a polygonal object.
  static MbSurfaceFitter * Create( const MbPolymeshTopo & mesh, const c3d::IndicesVector & indices );
  /// \ru Создать объект на основе региона полигонального объекта. \en Create an object based on a region of a polygonal object.
  static MbSurfaceFitter * Create( const MbPolymeshTopo & mesh, size_t iRegion );
  /// \ru Вписать поверхность. \en Fit a surface.
  virtual MbResultType FitSurface( const MbSurfaceFitToGridParameters & params, MbSurfaceFitToGridResults & results ) = 0;

};

#endif // __SURFACE_FITTING_H
