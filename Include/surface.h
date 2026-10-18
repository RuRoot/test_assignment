////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Поверхность.
         \en Surface. \~
  \details \ru Поверхности являются представителями семейства трёхмерных геометрических объектов.
    Поверхности играют главную роль в построении геометрической модели. Поверхностями описывают
    гладкие участки геометрической формы моделируемых объектов. Поверхности строятся с помощью
    аналитических функций, по набору точек, на базе кривых и на базе поверхностей.
           \en Surfaces are members of a family of three-dimensional geometric objects.
    Surfaces play a key role in construction of geometric model. Surfaces are used to describe
    smooth parts of geometrical form of modeled objects. Surfaces are constructed by
    analytical functions by a set of points on the basis of curves and on the basis of surfaces. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURFACE_H
#define __SURFACE_H


#include <space_item.h>
#include <mb_cart_point.h>
#include <mb_cube.h>
#include <mb_rect.h>
#include <mb_rect2d.h>
#include <mb_data.h>
#include <templ_three_states.h>
#include <templ_visitor.h>


class MbCurve;
class MbContour;
class MbLineSegment;
class MbCurve3D;
class MbPolygon3D;
class MbSplineSurface;
class MbSurfaceData;
class MbMesh;
class MbGrid;
class MbStepData;
struct MbFormNote;
struct MbNurbsParameters;
class MbSurface;

namespace c3d // namespace C3D
{
typedef SPtr<MbSurface>                               SurfaceSPtr;
typedef SPtr<const MbSurface>                         ConstSurfaceSPtr;

typedef std::vector<MbSurface *>                      SurfacesVector;
typedef std::vector<const MbSurface *>                ConstSurfacesVector;
typedef std::set<MbSurface *>                         SurfacesSet;
typedef std::set<const MbSurface *>                   ConstSurfacesSet;

typedef std::vector<SurfaceSPtr>                      SurfacesSPtrVector;
typedef std::vector<ConstSurfaceSPtr>                 ConstSurfacesSPtrVector;
typedef std::set<SurfaceSPtr>                         SurfacesSPtrSet;
typedef std::set<ConstSurfaceSPtr>                    ConstSurfacesSPtrSet;

typedef SurfacesSet::iterator                         SurfacesSetIt;
typedef SurfacesSet::const_iterator                   SurfacesSetConstIt;
typedef std::pair<SurfacesSetConstIt, bool>           SurfacesSetRet;

typedef ConstSurfacesSet::iterator                    ConstSurfacesSetIt;
typedef ConstSurfacesSet::const_iterator              ConstSurfacesSetConstIt;
typedef std::pair<ConstSurfacesSetConstIt, bool>      ConstSurfacesSetRet;

typedef SurfacesSPtrSet::iterator                     SurfacesSPtrSetIt;
typedef SurfacesSPtrSet::const_iterator               SurfacesSPtrSetConstIt;
typedef std::pair<SurfacesSPtrSetConstIt, bool>       SurfacesSPtrSetRet;

typedef ConstSurfacesSPtrSet::iterator                ConstSurfacesSPtrSetIt;
typedef ConstSurfacesSPtrSet::const_iterator          ConstSurfacesSPtrSetConstIt;
typedef std::pair<ConstSurfacesSPtrSetConstIt, bool>  ConstSurfacesSPtrSetRet;
}


//------------------------------------------------------------------------------
/** \brief \ru Поверхность в трехмерном пространстве.
           \en Surface in three-dimensional space. \~
  \details \ru Родительский класс всех поверхностей в трехмерном пространстве. 
    Поверхность представляет собой векторную функцию двух скалярных параметров,
    принимающих значения на двумерной связной области. Поверхность представляет собой
    непрерывное отображение двумерной связной области в трёхмерное пространство.\n
    Для всех поверхностей, кроме MbCurveBoundedSurface, 
    областью определения является прямоугольник в двумерном пространстве параметров.\n
    Поверхность используется:\n 
    для пространственного моделирования,\n
    для построения граней тел в частных случаях.
           \en The parent class for all surfaces in three-dimensional space. 
    A surface is a vector function of two scalar parameters,
    taking values on the two-dimensional connected region. Surface represents
    a continuous mapping of two-dimensional connected region to three-dimensional space. \n
    For all surfaces, except MbCurveBoundedSurface, 
    the domain is a rectangle in two-dimensional parameter space. \n
    A surface is used:\n 
    for spatial modeling,\n
    for construction of solid faces in special cases. \~
  \ingroup Surfaces
*/
// ---
class MATH_CLASS MbSurface : public MbSpaceItem, public MbNestSyncItem {
protected :
  /** \brief \ru Габаритный куб поверхности. 
             \en Bounding box of surface. \~
      \details \ru Габаритный куб поверхности рассчитывается только при запросе габарита объекта. Габаритный куб в конструкторе объекта и после модификации объекта принимает неопределенное значение. \n
               \en Bounding box of surface is calculated only at the request. Bounding box of surface is undefined after object constructor and after object modifications. \n \~
  */
  mutable MbCube cube; 

protected :
  /// \ru Конструктор. \en Constructor. 
  MbSurface();
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbSurface( const MbSurface & );
public:
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbSurface();

public:
  /// \ru Реализация функции, инициирующей посещение объекта. \en Implementation of a function initializing a visit of an object. 
  virtual void Accept( Visitor & visitor )
  {
    VisitorImpl<MbSurface> * impl = dynamic_cast<VisitorImpl<MbSurface> *>(&visitor);
    if( impl )
      impl->Visit( *this );
  }

  // \ru Общие функции геометрического объекта. \en Common functions of geometric object. 
  MbeSpaceType Type() const override;     // \ru Групповой тип элемента. \en Group element type.
  MbeSpaceType Family() const override;   // \ru Семейство объекта. \en Family of object.

  double  DistanceToPoint( const MbCartPoint3D & ) const override; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  void    AddYourGabaritTo( MbCube & c ) const override; // \ru Добавить габарит поверхности в куб. \en Add the surface bounding box into a cube. 
  void    Refresh() override; // \ru Сбросить все временные данные. \en Reset all temporary data. 

  /** \brief \ru Рассчитать временные (mutable) данные объекта.
             \en Calculate temporary (mutable) data of an object. \~
    \details \ru Рассчитать временные данные объекта в зависимости от параметра forced. 
      Если параметр forced равен false, рассчитываются только ещё не насчитанные данные.
      Если параметр forced равен true, перерасчитываются все временные данные объекта.
             \en Calculate the temporary data of an object depending of the "forced" parameter. 
      Calculate only data that was not calculated earlier if parameter "forced" is equal false.
      Recalculate all temporary data of an object if parameter "forced" is equal true.
    \param[in] forced - \ru Принудительный перерасчёт.
                        \en Forced recalculation. \~
  */
  virtual void    PrepareIntegralData( const bool forced ) const;

  void    CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.

  MbProperty & CreateProperty( MbePrompt name ) const override;     // \ru Создать собственное свойство. \en Create a custom property. 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \ru \name Функции описания области определения поверхности.
      \en \name Functions for surface domain description. 
      \{ */
  /// \ru Вернуть минимальное значение параметра u. \en Get the minimum value of u. 
  virtual double  GetUMin()   const = 0;
  /// \ru Вернуть минимальное значение параметра v. \en Get the minimum value of v. 
  virtual double  GetVMin()   const = 0;
  /// \ru Вернуть максимальное значение параметра u. \en Get the maximum value of u. 
  virtual double  GetUMax()   const = 0;
  /// \ru Вернуть максимальное значение параметра v. \en Get the maximum value of v. 
  virtual double  GetVMax()   const = 0;
  
  /** \brief \ru Определить, замкнута ли поверхность по параметру u.
             \en Determine whether a surface is closed in u-parameter direction. \~
    \details \ru Определить, замкнута ли гладко поверхность по u-параметру без учета граничного контура.
                 Всегда false для MbCurveBoundSurface. Рекомендуется использовать IsUPeriodic.
             \en Determine whether a surface is closed smoothly in u-parameter direction without regard to the boundary contour.
                 It is always false for MbCurveBoundSurface. It is recommended to use IsUPeriodic. \~
  */
  virtual bool    IsUClosed() const = 0;
  
  /** \brief \ru Определить, замкнута ли поверхность по параметру v.
             \en Determine whether a surface is closed in v-parameter direction. \~
    \details \ru Определить, замкнута ли гладко поверхность по v-параметру без учета граничного контура.
                 Всегда false для MbCurveBoundSurface. Рекомендуется использовать IsVPeriodic.
             \en Determine whether a surface is closed smoothly in v-parameter direction without regard to the boundary contour.
                 It is always false for MbCurveBoundSurface. It is recommended to use IsVPeriodic. \~
  */
  virtual bool    IsVClosed() const = 0;

  /** \brief \ru Определить, замкнута ли фактически поверхность по u-параметру независимо от гладкости замыкания.
             \en Determine whether a surface is closed in u-parameter direction regardless of the smoothness of the closure. \~
    \details \ru Определить, замкнута ли фактически поверхность по u-параметру независимо от гладкости замыкания.
             \en Determine whether a surface is actually closed in u-parameter direction regardless of the smoothness of the closure. \~
  */
  virtual bool    IsUTouch()  const;
  /** \brief \ru Определить, замкнута ли фактически поверхность по v-параметру независимо от гладкости замыкания.
             \en Determine whether a surface is closed in v-parameter direction regardless of the smoothness of the closure. \~
    \details \ru Определить, замкнута ли фактически поверхность по v-параметру независимо от гладкости замыкания.
             \en Determine whether a surface is actually closed in v-parameter direction regardless of the smoothness of the closure. \~
  */
  virtual bool    IsVTouch()  const;
  
  /** \brief \ru Определить, замкнута ли поверхность по параметру u.
             \en Determine whether a surface is closed in u-parameter direction. \~
    \details \ru Определить, замкнута ли гладко поверхность по u-параметру без учета граничного контура.
             \en Determine whether a surface is smoothly closed in u-parameter direction. \~
  */
  virtual bool    IsUPeriodic()  const;
  /** \brief \ru Определить, замкнута ли поверхность по параметру v.
             \en Determine whether a surface is closed in v-parameter direction. \~
    \details \ru Определить, замкнута ли гладко поверхность по v-параметру без учета граничного контура.
             \en Determine whether a surface is smoothly closed in v-parameter direction. \~
  */
  virtual bool    IsVPeriodic()  const;

  /// \ru Вернуть период для гладко замкнутой поверхности или 0. \en Return period for smoothly closed surface or 0. 
  virtual double  GetUPeriod() const;
  /// \ru Вернуть период для гладко замкнутой поверхности или 0. \en Return period for smoothly closed surface or 0. 
  virtual double  GetVPeriod() const;
  
  /** \brief \ru Вернуть период.
             \en Return period. \~
    \details \ru Период вычисляется для гладко замкнутой поверхности по одному из параметров.
                 Если поверхность не замкнута по этому параметру, возвращается 0.
             \en Period is calculated for smoothly closed surface in one of its parameters.
                 If a surface is not closed in this parameter then 0 is returned. \~ 
    \param[in] i - \ru Показывает направление: 0 - период по u, 1 - период по v.
                   \en Shows direction: 0 - period in u, 1 - period in v. \~
  */
  virtual double  GetPeriod( ptrdiff_t i ) const; 
  
  /** \brief \ru Определить периодичность.
             \en Determine periodicity. \~
    \details \ru Является ли поверхность периодической.
             \en Whether a surface is periodic. \~
    \return \ru 0 если не периодическая \n 1 если периодическая по U \n 2 если периодическая по V \n 3 если периодическая по U и V
            \en 0 - it is not periodic \n 1 - it is periodic in U \n 2 -  it is periodic in V \n 3 - it is periodic in U and V \~
  */
  virtual size_t  Periodicity() const;  
  
  /// \ru Существует ли полюс на границе параметрической области. \en Whether a pole exists on parametric region boundary. 
  virtual bool    GetPoleUMin() const;
  /// \ru Существует ли полюс на границе параметрической области. \en Whether a pole exists on parametric region boundary. 
  virtual bool    GetPoleUMax() const;
  /// \ru Существует ли полюс на границе параметрической области. \en Whether a pole exists on parametric region boundary. 
  virtual bool    GetPoleVMin() const;
  /// \ru Существует ли полюс на границе параметрической области. \en Whether a pole exists on parametric region boundary. 
  virtual bool    GetPoleVMax() const;
  /// \ru Является ли точка полюсом. \en Whether the point is a pole. 
  virtual bool    IsPole( double u, double v, double paramPrecision = PARAM_PRECISION ) const;
  /// \ru Является ли точка полюсом. \en Whether the point is a pole. 
  bool    IsPole( const MbCartPoint & uv, double paramPrecision = PARAM_PRECISION ) const { return IsPole( uv.x, uv.y, paramPrecision ); }

  /** \} */

  /** \ru \name Функции для работы в области определения поверхности
            Функции PointOn, Derive... поверхностей корректируют параметры
            при выходе их за пределы прямоугольной области определения параметров.\n
            Исключения составляют:\n
            1. MbPlane (плоскость)\n 
               Функции PointOn, Derive... плоскости не корректируют параметры
               при выходе их за пределы прямоугольной области определения параметров.\n
            2. MbSmoothSurface и её наследники (поверхности скругления или фаски)\n
               Функции PointOn и Derive... поверхностей сопряжения не корректируют
               первый параметр при его выходе за пределы определения параметров.
      \en \name Functions for working at surface domain
            Functions PointOn, Derive... correct parameters
            when getting out of rectangular domain bounds. \n
            Exceptions:\n
            1. MbPlane (plane)\n 
               Functions PointOn, Derive... of plane don't correct parameters
               when getting out of rectangular domain bounds. \n
            2. MbSmoothSurface and its inheritors (fillet of chamfer surfaces)\n
               Functions PointOn and Derive... of smooth surfaces don't correct
               the first parameter when getting out of domain bounds.
      \{ */
  
  /** \brief \ru Вычислить точку на поверхности.
             \en Calculate a point on the surface. \~
    \details \ru Скорректировать параметры при выходе их за пределы прямоугольной области определения и вычислить точку на поверхности.
             \en Correct parameters when getting out of rectangular domain bounds and Calculate a point on the surface. \~
    \param[in] u - \ru Первый параметр поверхности.
                   \en First surface parameter. \~ 
    \param[in] v - \ru Второй параметр поверхности.
                   \en Second surface parameter. \~
    \param[out] p - \ru Вычисленная точка на поверхности.
                    \en A point on the surface. \~
    \ingroup Surfaces
  */
  virtual void    PointOn  ( double & u, double & v, MbCartPoint3D & p ) const = 0;
  /// \ru Вычислить первую производную по u. \en Calculate first derivative with respect to u. 
  virtual void    DeriveU  ( double & u, double & v, MbVector3D & ) const = 0;
  /// \ru Вычислить первую производную по v. \en Calculate first derivative with respect to v. 
  virtual void    DeriveV  ( double & u, double & v, MbVector3D & ) const = 0;
  /// \ru Вычислить вторую производную по u. \en Calculate second derivative with respect to u. 
  virtual void    DeriveUU ( double & u, double & v, MbVector3D & ) const = 0;
  /// \ru Вычислить вторую производную по v. \en Calculate second derivative with respect to v. 
  virtual void    DeriveVV ( double & u, double & v, MbVector3D & ) const = 0;
  /// \ru Вычислить вторую производную. \en Calculate second derivative. 
  virtual void    DeriveUV ( double & u, double & v, MbVector3D & ) const = 0;
  /// \ru Вычислить третью производную. \en Calculate third derivative. 
  virtual void    DeriveUUU( double & u, double & v, MbVector3D & ) const = 0;
  /// \ru Вычислить третью производную. \en Calculate third derivative. 
  virtual void    DeriveUUV( double & u, double & v, MbVector3D & ) const = 0;
  /// \ru Вычислить третью производную. \en Calculate third derivative. 
  virtual void    DeriveUVV( double & u, double & v, MbVector3D & ) const = 0;
  /// \ru Вычислить третью производную. \en Calculate third derivative. 
  virtual void    DeriveVVV( double & u, double & v, MbVector3D & ) const = 0;
  /// \ru Вычислить касательный вектор по u. \en Calculate tangent vector in u. 
  virtual void    TangentU ( double & u, double & v, MbVector3D & ) const;
  /// \ru Вычислить касательный вектор по v. \en Calculate tangent vector in v. 
  virtual void    TangentV ( double & u, double & v, MbVector3D & ) const;
  /// \ru Вычислить нормаль. \en Calculate normal. 
  virtual void    Normal   ( double & u, double & v, MbVector3D & ) const;
  /// \ru Вычислить производную нормали по U. \en Calculate derivative of normal with respect to U. 
  virtual void    NormalU  ( double & u, double & v, MbVector3D & ) const;
  /// \ru Вычислить производную нормали по V. \en Calculate derivative of normal with respect to V. 
  virtual void    NormalV  ( double & u, double & v, MbVector3D & ) const;
  /** \brief \ru Вычислить значения точки и первых производных по u и по v.
             \en Calculate point and first derivatives with respect to u and with respect to v. \~
    \details \ru Скорректировать параметры при выходе их за пределы прямоугольной области определения и вычислить значения точки и первых производных по u и по v.
             \en Correct parameters when getting out of rectangular domain bounds and calculate point and first derivatives with respect to u and with respect to v. \~
    \param[in] u   - \ru Параметр.
                     \en Parameter. \~
    \param[in] v   - \ru Параметр.
                     \en Parameter. \~
    \param[out] pnt   - \ru Точка.
                        \en Point. \~
    \param[out] uDer  - \ru Производная по u.
                        \en Derivative with respect to u. \~
    \param[out] vDer  - \ru Производная по v.
                        \en Derivative with respect to v. \~
    \ingroup Surfaces
  */
  virtual void    Explore( double & u, double & v,
                           MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer ) const;

  /** \} */

  /** \ru \name Функции для работы внутри и вне области определения поверхности
            функции _PointOn, _Derive... поверхностей не корректируют
            параметры при выходе их за пределы прямоугольной области определения параметров.
      \en \name Functions for working inside and outside the surface domain.
            functions _PointOn, _Derive... of surfaces don't correct
            parameters when getting out of rectangular domain bounds. 
      \{ */
  
  /** \brief \ru Вычислить точку на поверхности.
             \en Calculate a point on the surface. \~
    \details \ru Вычислить точку на поверхности в том числе и за пределами области определения параметров.
             \en Calculate a point on the surface including the outside area determination parameters. \~
    \param[in] u - \ru Первый параметр поверхности.
                   \en First surface parameter. \~ 
    \param[in] v - \ru Второй параметр поверхности.
                   \en Second surface parameter. \~
    \param[out] p - \ru Вычисленная точка на поверхности.
                       \en A point on the surface or on extended surface. \~
    \ingroup Surfaces
  */
  virtual void   _PointOn  ( double  u, double  v, MbCartPoint3D & p ) const;
  /// \ru Вычислить первую производную по u на расширенной поверхности. \en Calculate first derivative with respect to u on extended surface. 
  virtual void   _DeriveU  ( double  u, double  v, MbVector3D & ) const;
  /// \ru Вычислить первую производную по v на расширенной поверхности. \en Calculate first derivative with respect to v on extended surface. 
  virtual void   _DeriveV  ( double  u, double  v, MbVector3D & ) const;
  /// \ru Вычислить вторую производную по u на расширенной поверхности. \en Calculate second derivative with respect to u on extended surface. 
  virtual void   _DeriveUU ( double  u, double  v, MbVector3D & ) const;
  /// \ru Вычислить вторую производную по v на расширенной поверхности. \en Calculate second derivative with respect to v on extended surface. 
  virtual void   _DeriveVV ( double  u, double  v, MbVector3D & ) const;
  /// \ru Вычислить вторую производную на расширенной поверхности. \en Calculate second derivative on extended surface. 
  virtual void   _DeriveUV ( double  u, double  v, MbVector3D & ) const;
  /// \ru Вычислить третью производную на расширенной поверхности. \en Calculate third derivative on extended surface. 
  virtual void   _DeriveUUU( double  u, double  v, MbVector3D & ) const;
  /// \ru Вычислить третью производную на расширенной поверхности. \en Calculate third derivative on extended surface. 
  virtual void   _DeriveUUV( double  u, double  v, MbVector3D & ) const;
  /// \ru Вычислить третью производную на расширенной поверхности. \en Calculate third derivative on extended surface. 
  virtual void   _DeriveUVV( double  u, double  v, MbVector3D & ) const;
  /// \ru Вычислить третью производную на расширенной поверхности. \en Calculate third derivative on extended surface. 
  virtual void   _DeriveVVV( double  u, double  v, MbVector3D & ) const;
  /// \ru Вычислить касательный вектор по u на расширенной поверхности. \en Calculate tangent vector in u direction on extended surface. 
  virtual void   _TangentU ( double  u, double  v, MbVector3D & ) const;
  /// \ru Вычислить касательный вектор по v на расширенной поверхности. \en Calculate tangent vector in v direction on extended surface. 
  virtual void   _TangentV ( double  u, double  v, MbVector3D & ) const;
  /// \ru Вычислить нормаль на расширенной поверхности. \en Calculate a normal on extended surface. 
  virtual void   _Normal   ( double  u, double  v, MbVector3D & ) const;
  /// \ru Вычислить производную нормали на расширенной поверхности. \en Calculate derivative of normal vector on extended surface. 
  virtual void   _NormalU  ( double  u, double  v, MbVector3D & ) const;
  /// \ru Вычислить производную нормали на расширенной поверхности. \en Calculate derivative of normal vector on extended surface. 
  virtual void   _NormalV  ( double  u, double  v, MbVector3D & ) const;
  /// \ru Вычислить вторую производную нормали на расширенной поверхности. \en Calculate second derivative of normal vector on extended surface. 
  virtual void   _NormalUU ( double  u, double  v, MbVector3D & ) const;
  /// \ru Вычислить вторую производную нормали на расширенной поверхности. \en Calculate second derivative of normal vector on extended surface. 
  virtual void   _NormalUV ( double  u, double  v, MbVector3D & ) const;
  /// \ru Вычислить вторую производную нормали на расширенной поверхности. \en Calculate second derivative of normal vector on extended surface. 
  virtual void   _NormalVV ( double  u, double  v, MbVector3D & ) const;
  
  /// \ru Вычислить производную нормали в точке с параметрами u v вдоль линии du dv. \en Calculate derivative of normal vector at the point with the given parameters u and v along the line with the given direction (du,dv). 
  virtual void   _NormalD  ( double  u, double  v, double du, double dv, MbVector3D & der ) const; 
  /// \ru Вычислить вторую производную нормали в точке с параметрами u v вдоль линии du dv. \en Calculate second derivative of normal vector at the point with the given parameters u and v along the line with the given direction (du,dv). 
  virtual void   _NormalDD( double  u, double  v, double du, double dv, MbVector3D & der ) const;

  /** \brief \ru Вычислить значения точки и производных для заданных параметров.
             \en Calculate point and derivatives of object for given parameters. \~
    \details \ru Значения точки и производных вычисляются в пределах области определения и на расширенной поверхности.
             \en Values of point and derivatives are calculated on parameters area and on extended surface. \~
    \param[in] u   - \ru Параметр.
                     \en Parameter. \~
    \param[in] v   - \ru Параметр.
                     \en Parameter. \~
    \param[in] ext - \ru В пределах области определения (false), на расширенной поверхности (true).
                     \en On parameters area (false), on extended surface (true). \~
    \param[out] pnt   - \ru Точка.
                        \en Point. \~
    \param[out] uDer  - \ru Производная по u.
                        \en Derivative with respect to u. \~
    \param[out] vDer  - \ru Производная по v.
                        \en Derivative with respect to v. \~
    \param[out] uuDer - \ru Вторая производная по u, если не ноль.
                        \en Second derivative with respect to u, if not nullptr. \~
    \param[out] vvDer - \ru Вторая производная по v, если не ноль.
                        \en Second derivative with respect to v, if not nullptr. \~
    \param[out] uvDer - \ru Вторая производная по u и по v, если не ноль.
                        \en Second derivative with respect to u and v, if not nullptr. \~
    \ingroup Surfaces
  */
  virtual void    Explore( double & u, double & v, bool ext,
                           MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer,
                           MbVector3D * uuDer, MbVector3D * vvDer, MbVector3D * uvDer, MbVector3D * nor ) const;

  /** \brief \ru Вычислить значения всех производных в точке.
             \en Calculate all derivatives at point. \~
    \details \ru Значения точки и производных вычисляются на расширенной поверхности.
             \en Values of point and derivatives are calculated an extended surface. \~
    \param[in] u   - \ru Параметр.
                     \en Parameter. \~
    \param[in] v   - \ru Параметр.
                     \en Parameter. \~
    \param[out] pnt - \ru Точка.
                      \en Point. \~
    \param[out] deru - \ru Производная по u.
                       \en Derivative with respect to u. \~
    \param[out] derv - \ru Производная по v.
                       \en Derivative with respect to v. \~
    \param[out] norm - \ru Нормаль.
                       \en Normal. \~
    \param[out] noru - \ru Производная нормали по u.
                       \en Derivative of normal vector with respect to u. \~
    \param[out] norv - \ru Производная нормали по v.
                       \en Derivative of normal vector with respect to v. \~
    \param[out] deruu - \ru Вторая производная по u.
                        \en Second derivative with respect to u. \~
    \param[out] dervv - \ru Вторая производная по v.
                        \en Second derivative with respect to v. \~
    \param[out] deruv - \ru Вторая производная по u и по v.
                        \en Second derivative with respect to u and v. \~
    \ingroup Surfaces
  */
  virtual void   _PointNormal( double u, double v,
                               MbCartPoint3D & pnt, MbVector3D & deru, MbVector3D & derv,
                               MbVector3D & norm, MbVector3D & noru, MbVector3D & norv,
                               MbVector3D & deruu, MbVector3D & dervv, MbVector3D & deruv ) const;
  /** \} */
  /** \ru \name Функции движения по поверхности
      \en \name Functions of moving on surface
      \{ */

  /** \brief \ru Вычислить шаг параметра.
             \en Calculate parameter step. \~
    \details \ru Вычислить шаг параметра для аппроксимации поверхности по величине прогиба 
      вдоль линии с постоянным значением v. 
      Вычисление шага проходит с учетом радиуса кривизны.
      Шаг аппроксимации вдоль кривой выбирается таким образом, 
      чтобы отклонение кривой от ее полигона не превышало заданную величину прогиба.
             \en Calculate parameter step for the surface approximation by its sag value 
      along a line with a constant value of v. 
      Calculation of the step is performed with consideration of curvature radius.
      A step of surface approximation along a curve is chosen in such way, 
      that the deviation from its polygon does not exceed the given value of sag. \~
    \param[in] u   - \ru Параметр, определяющий точку на поверхности.
                     \en A parameter defining a point on the surface. \~
    \param[in] v   - \ru Параметр, определяющий точку на поверхности.
                     \en A parameter defining a point on the surface. \~
    \param[in] sag - \ru Максимально допустимая величина прогиба.
                     \en Maximum feasible sag value. \~
    \return \ru Величина шага по параметру в заданной точке.
            \en A sag value by parameter at given point. \~
    \ingroup Surfaces
  */
  virtual double  StepU( double u, double v, double sag ) const;

  /** \brief \ru Вычислить шаг параметра.
             \en Calculate parameter step. \~
    \details \ru Вычислить шаг параметра для аппроксимации поверхности по величине прогиба 
      вдоль линии с постоянным значением u. 
      Вычисление шага проходит с учетом радиуса кривизны.
      Шаг аппроксимации вдоль кривой выбирается таким образом, 
      чтобы отклонение кривой от ее полигона не превышало заданную величину прогиба.
             \en Calculate parameter step for the surface approximation by its sag value 
      along a line with a constant value of u. 
      Calculation of the step is performed with consideration of curvature radius.
      A step of surface approximation along a curve is chosen in such way, 
      that the deviation from its polygon does not exceed the given value of sag. \~
    \param[in] u   - \ru Параметр, определяющий точку на поверхности.
                     \en A parameter defining a point on the surface. \~
    \param[in] v   - \ru Параметр, определяющий точку на поверхности.
                     \en A parameter defining a point on the surface. \~
    \param[in] sag - \ru Максимально допустимая величина прогиба.
                     \en Maximum feasible sag value. \~
    \return \ru Величина шага по параметру в заданной точке.
            \en A sag value by parameter at given point. \~
    \ingroup Surfaces
  */
  virtual double  StepV( double u, double v, double sag ) const;

  /** \brief \ru Вычислить шаг параметра.
             \en Calculate parameter step. \~
    \details \ru Вычислить шаг параметра для аппроксимации поверхности по углу отклонения касательной 
      вдоль линии с постоянным значением u. 
      Шаг аппроксимации вдоль кривой выбирается таким образом, 
      чтобы угловое отклонение касательной к кривой в следующей точке 
      не превышало заданную величину ang.
             \en Calculate parameter step for the surface approximation by the deviation angle of the tangent vector 
      along a line with a constant value of u. 
      A step of surface approximation along a curve is chosen in such way, 
      that angular deviation of the tangent to the curve at the next point 
      does not exceed the given value ang. \~
    \param[in] u   - \ru Параметр, определяющий точку на поверхности.
                     \en A parameter defining a point on the surface. \~
    \param[in] v   - \ru Параметр, определяющий точку на поверхности.
                     \en A parameter defining a point on the surface. \~
    \param[in] ang - \ru Максимально допустимый угол отклонения касательной.
                     \en The maximum feasible deviation angle of tangent. \~
    \return \ru Величина шага по параметру в заданной точке.
            \en A sag value by parameter at given point. \~
    \ingroup Surfaces
  */
  virtual double  DeviationStepU( double u, double v, double angle ) const;

  /** \brief \ru Вычислить шаг параметра.
             \en Calculate parameter step. \~
    \details \ru Вычислить шаг параметра для аппроксимации поверхности по углу отклонения касательной 
      вдоль линии с постоянным значением v. 
      Шаг аппроксимации вдоль кривой выбирается таким образом, 
      чтобы угловое отклонение касательной к кривой в следующей точке 
      не превышало заданную величину ang.
             \en Calculate parameter step for the surface approximation by the deviation angle of the tangent vector 
      along a line with a constant value of v. 
      A step of surface approximation along a curve is chosen in such way, 
      that angular deviation of the tangent to the curve at the next point 
      does not exceed the given value ang. \~
    \param[in] u   - \ru Параметр, определяющий точку на поверхности.
                     \en A parameter defining a point on the surface. \~
    \param[in] v   - \ru Параметр, определяющий точку на поверхности.
                     \en A parameter defining a point on the surface. \~
    \param[in] ang - \ru Максимально допустимый угол отклонения касательной.
                     \en The maximum feasible deviation angle of tangent. \~
    \return \ru Величина шага по параметру в заданной точке.
            \en A sag value by parameter at given point. \~
    \ingroup Surfaces
  */
  virtual double  DeviationStepV( double u, double v, double angle ) const;

  /** \brief \ru Вычислить шаг параметра.
             \en Calculate parameter step. \~
    \details \ru Вычислить шаг параметра для аппроксимации поверхности по заданной метрической длине шага 
      вдоль линии с постоянным значением u.
             \en Calculate the parameter step for approximation of a surface by the given metric length of a step 
      along a line with a constant value of u. \~ 
    \param[in] u   - \ru Параметр, определяющий точку на поверхности.
                     \en A parameter defining a point on the surface. \~
    \param[in] v   - \ru Параметр, определяющий точку на поверхности.
                     \en A parameter defining a point on the surface. \~
    \param[in] len - \ru Заданная метрическая длина.
                     \en The given metric length. \~
    \return \ru Величина шага по параметру в заданной точке.
            \en A sag value by parameter at given point. \~
    \ingroup Surfaces
  */
  virtual double  MetricStepU( double u, double v, double length ) const;

  /** \brief \ru Вычислить шаг параметра.
             \en Calculate parameter step. \~
    \details \ru Вычислить шаг параметра для аппроксимации поверхности по заданной метрической длине шага 
      вдоль линии с постоянным значением v.
             \en Calculate the parameter step for approximation of a surface by the given metric length of a step 
      along a line with a constant value of v. \~ 
    \param[in] u   - \ru Параметр, определяющий точку на поверхности.
                     \en A parameter defining a point on the surface. \~
    \param[in] v   - \ru Параметр, определяющий точку на поверхности.
                     \en A parameter defining a point on the surface. \~
    \param[in] len - \ru Заданная метрическая длина.
                     \en The given metric length. \~
    \return \ru Величина шага по параметру в заданной точке.
            \en A sag value by parameter at given point. \~
    \ingroup Surfaces
  */
  virtual double  MetricStepV( double u, double v, double length ) const; 

  /** \brief \ru Вычислить шаг параметра.
             \en Calculate parameter step. \~
    \details \ru Вычислить шаг одного из параметров для аппроксимации поверхности или по угловому отклонению нормали, или по величине прогиба, или по метрической длине. 
             \en Calculate parameter step for the surface approximation: by diviation sngle of normal; or by its sag value; or by the metric length. \~
    \param[in]  u        - \ru Первый параметр поверхности.
                           \en First surface parameter. \~
    \param[in]  v        - \ru Второй параметр поверхности.
                           \en Second surface parameter. \~
    \param[in]  alongU   - \ru Вдоль первого (true) или второго (false) параметра поверхности. 
                           \en Along the first (true) or second (false) surface parameter. \~
    \param[in]  stepData - \ru Данные для вычисления шага. \n
                           \en Data for step calculation. \~
    \return \ru Величина шага по параметру в заданной точке.
            \en A sag value by parameter at given point. \~
    \ingroup Surfaces
  */
  double  SurfaceStep( const double & u, const double & v, bool alongU, const MbStepData & stepData ) const;

  /// \ru Количество разбиений по параметру u для проверки событий. \en The number of splittings by u-parameter for a check of events. 
  virtual size_t  GetUCount() const;
  /// \ru Количество разбиений по параметру v для проверки событий. \en The number of splittings by v-parameter for a check of events. 
  virtual size_t  GetVCount() const;
  /** \} */
  /** \ru \name Общие функции поверхности
      \en \name Common functions of surface.
      \{ */
  /// \ru Дать себя (перегружена только у CurveBoundedSurface). \en Get itself (it is overloaded only in CurveBoundedSurface). 
  virtual const MbSurface & GetSurface()      const;
  /// \ru Дать базовую поверхность, если есть, или себя. \en Get the base surface if exists or itself. 
  virtual const MbSurface & GetBasisSurface() const;
  /// \ru Дать себя (перегружена только у CurveBoundedSurface). \en Get itself (it is overloaded only in CurveBoundedSurface). 
  virtual       MbSurface & SetSurface()     ;
  /// \ru Дать базовую поверхность, если есть, или себя. \en Get the base surface if exists or itself. 
  virtual       MbSurface & SetBasisSurface();
    
  // \ru Выдать граничную точку. \en Get the boundary point. 

  /** \brief \ru Вычислить граничную точку.
             \en Calculate the boundary point. \~
    \details \ru Вычислить одну из точек, параметры которых принимают наибольшие или наименьшие значения. \n
             \en Calculate one of the points where parameters take the maximum or the minimum values. \n \~
    \param[in] number - \ru Номер граничной точки. \n
      1 соответствует точке (umin, vmin)\n
      2 - точке (umax, vmin)  \n
      3 - точке (umax, vmax)  \n
      4 - точке (umin, vmax)
                        \en A number of a boundary point. \n
      1 corresponds to the point (umin, vmin)\n
      2 corresponds to the point (umax, vmin)\n
      3 corresponds to the point (umax, vmax)\n
      4 corresponds to the point (umin, vmax) \~
    \param[in, out] pnt - \ru Вычисленная точка.
                          \en A calculated point. \~
  */
  virtual void    GetLimitPoint( ptrdiff_t number, MbCartPoint3D & pnt ) const; // \ru Выдать граничную трехмерную точку. \en Get the boundary three-dimensional point. 

  /** \brief \ru Вычислить двумерную граничную точку.
             \en Calculate the boundary two-dimensional point. \~
    \details \ru Вычислить одну из точек, параметры которых принимают наибольшие или наименьшие значения. \n
             \en Calculate one of the points where parameters take the maximum or the minimum values. \n \~
    \param[in] number - \ru Номер граничной точки. \n 
      1 соответствует точке (umin, vmin) \n
      2 - точке (umax, vmin) \n
      3 - точке (umax, vmax) \n
      4 - точке (umin, vmax)
                        \en A number of a boundary point. \n 
      1 corresponds to the point (umin, vmin)\n
      2 corresponds to the point (umax, vmin)\n
      3 corresponds to the point (umax, vmax)\n
      4 corresponds to the point (umin, vmax) \~
    \param[in, out] pnt - \ru Вычисленная точка.
                          \en A calculated point. \~
  */
  virtual void    GetLimitPoint( ptrdiff_t number, MbCartPoint & pnt ) const; // \ru Выдать граничную двумерную точку (граничные параметры). \en Get the boundary two-dimensional point (boundary parameters). 

  /** \brief \ru Вычислить граничную точку.
             \en Calculate the boundary point. \~
    \details \ru Вычислить одну из точек, параметры которых принимают наибольшие или наименьшие значения.
             \en Calculate one of the points where parameters take the maximum or the minimum values. \~
    \param[in] number - \ru Номер граничной точки. \n 
      1 соответствует точке (umin, vmin) \n
      2 - точке (umax, vmin) \n
      3 - точке (umax, vmax) \n
      4 - точке (umin, vmax)
                        \en A number of a boundary point. \n 
      1 corresponds to the point (umin, vmin)\n
      2 corresponds to the point (umax, vmin)\n
      3 corresponds to the point (umax, vmax)\n
      4 corresponds to the point (umin, vmax) \~
    \return \ru Вычисленная точка.
            \en A calculated point. \~
  */
  MbCartPoint3D   GetLimitPoint( ptrdiff_t number ) const; ///< \ru Выдать граничную трехмерную точку. \en Get the boundary three-dimensional point. 

  /** \brief \ru Вычислить кривизну линии вдоль u.
             \en Calculate line curvature along the direction of u. \~
    \details \ru Вычисляется кривизна линии вдоль u при v = const.
             \en There is calculated a line curvature along the direction of u when v = const. \~ 
    \param[in] u - \ru Параметр.
                   \en Parameter. \~ 
    \param[in] v - \ru Параметр.
                   \en Parameter. \~ 
    \return \ru Кривизна.
            \en Curvature. \~
  */
  virtual double  CurvatureU( double u, double v ) const; // \ru Кривизна линии u. \en Curvature of u line. 

  /** \brief \ru Вычислить кривизну линии вдоль v.
             \en Calculate line curvature along the direction of v. \~
    \details \ru Вычисляется кривизна линии вдоль v при u = const.
             \en There is calculated a line curvature along the direction of v when u = const. \~ 
    \param[in] u - \ru Параметр.
                   \en Parameter. \~ 
    \param[in] v - \ru Параметр.
                   \en Parameter. \~ 
    \return \ru Кривизна.
            \en Curvature. \~
  */
  virtual double  CurvatureV( double u, double v ) const; // \ru Кривизна линии v. \en Curvature of v line. 

  /** \brief \ru Вычислить нормальную кривизну линии вдоль u.
             \en Calculate a normal curvature of line along the direction of u. \~
    \details \ru Вычисляется нормальная кривизна координатной линии вдоль u.
             \en Normal curvature of the coordinate line is calculated along the direction of u. \~ 
    \param[in] u - \ru Параметр.
                   \en Parameter. \~ 
    \param[in] v - \ru Параметр.
                   \en Parameter. \~ 
    \return \ru Кривизна.
            \en Curvature. \~
  */
  double  NormalCurvatureU( double u, double v ) const; // \ru Нормальная кривизна поверхности вдоль линии u. \en A normal curvature of surface along the direction of u. 

  /** \brief \ru Вычислить нормальную кривизну линии вдоль v.
             \en Calculate a normal curvature of line along the direction of v. \~
    \details \ru Вычисляется нормальная кривизна координатной линии вдоль v.
             \en Normal curvature of the coordinate line is calculated along the direction of v. \~ 
    \param[in] u - \ru Параметр.
                   \en Parameter. \~ 
    \param[in] v - \ru Параметр.
                   \en Parameter. \~ 
    \return \ru Кривизна.
            \en Curvature. \~
  */
  double  NormalCurvatureV( double u, double v ) const; // \ru Нормальная кривизна поверхности вдоль линии v. \en A normal curvature of surface along the direction of v. 

  /** \brief \ru Вычислить нормальную кривизну поверхности.
             \en Calculate a normal curvature of surface. \~
    \details \ru Вычисляется нормальная кривизна поверхности вдоль линии du dv.
             \en Normal curvature of surface is calculated along the line (du, dv). \~ 
    \param[in] u - \ru Параметр.
                   \en Parameter. \~ 
    \param[in] v - \ru Параметр.
                   \en Parameter. \~ 
    \param[in] du - \ru Задает направление линии, вдоль которой вычисляется нормальная кривизна.
                    \en Sets the direction of line a normal curvature is calculated along. \~ 
    \param[in] dv - \ru Задает направление линии, вдоль которой вычисляется нормальная кривизна.
                    \en Sets the direction of line a normal curvature is calculated along. \~
    \return \ru Кривизна.
            \en Curvature. \~
  */
  double  NormalCurvature ( double u, double v, double du, double dv ) const; // \ru Нормальная кривизна поверхности. \en Normal curvature of surface. 
                    
  /** \brief \ru Вычислить Среднюю и Гауссову кривизну.
             \en Calculate the mean and the Gaussian curvature. \~    
    \details \ru Средняя и Гауссова кривизна.
             \en The mean and the Gaussian curvature. \~ 
    \param[in] u - \ru Параметр.
                   \en Parameter. \~  
    \param[in] v - \ru Параметр.
                   \en Parameter. \~ 
    \param[out] mean - \ru Средняя кривизна.
                       \en Mean curvature. \~ 
    \param[out] gauss - \ru Гауссова кривизна.
                        \en Gaussian curvature. \~
    \return \ru true в случае успеха операции \n false в противном случае
            \en True if the operation succeeded \n otherwise false. \~
  */
  bool    MeanGaussCurvature( double u, double v, double & mean, double & gauss ) const;

  /** \brief \ru Вычислить главные кривизны и главные направления кривизн.
             \en Calculate the mean and the Gaussian curvature. \~    
    \details \ru Вычислить главные кривизны и главные направления кривизн, которые определяют приращения параметров du и dv.
             \en The mean and the Gaussian curvature. \~ 
    \param[in] u - \ru Первый параметр поверхности.
                   \en First surface parameter. \~  
    \param[in] v - \ru Второй параметр поверхности.
                   \en Second surface parameter. \~ 
    \param[out] c1 - \ru Первая главная кривизна.
                     \en First surface curvature. \~ 
    \param[out] c2 - \ru Вторая главная кривизна.
                     \en Second surface curvature. \~
    \param[out] du - \ru du и dv являются компонентами вектора кривизны.
                     \en du and dv are the components of the curvature vector. \~ 
    \param[out] dv - \ru du и dv являются компонентами вектора кривизны.
                     \en du and dv are the components of the curvature vector. \~
    \return \ru true в случае успеха вычисления \n false в противном случае.
            \en True if the calculation succeeded \n otherwise false. \~
  */
  bool    MainCurvatures( double u, double v, double & c1, double & c2, double & du, double & dv ) const;

  /// \ru Является ли базовая поверхность копией базовой поверхности данного объекта. \en Whether a base surface is a copy of the base surface of the given object. 
  virtual bool    IsSameBase( const MbSurface & ) const;
  /// \ru Является ли поверхность плоской. \en Whether a surface is planar. 
  virtual bool    IsPlanar( double accuracy = METRIC_EPSILON )  const;

  /** \brief \ru Дать физический радиус объекта или ноль, если это невозможно.
             \en Get the physical radius of the object or null if it impossible. \~    
    \details \ru Метод выдает максимальный физический радиус по одному из изопараметрических направлений, если соответствующая изопараметрическая кривая является дугой окружности, в противном случае метод выдает ноль.
             \en Method returns maximum physical radius by one of the parametric direction if the corresponding parametric curve is the arc, method returns zero otherwise. \~ 
  */
  virtual double  GetRadius() const;
  /** \brief    \ru Дать радиус скругления, если поверхность является поверхностью скругления с заданной точностью.
                \en Get fillet radius if the surface is a fillet surface with the set accuracy. \~
      \details  \ru Дать радиус скругления, если поверхность является поверхностью скругления с заданной точностью.
                    Точность нужна для сравнения срезов поверхности с дугой окружности для поиска оси вращения.
                \en Get fillet radius if the surface is a fillet surface with the set accuracy.
                    Accuracy is used to compare surface slices to an arc to find rotation axis. \~
      \param[in] p -        \ru Точка, рядом с которой ищется радиус. Имеет смысл, если радиус скругления переменный.
                            \en Point near which the radius is searched. Used when the radius is variable. \~ 
      \param[in] accuracy - \ru Точность.
                            \en Accuracy. \~ 
      \return \ru Радиус скругления, если возможно, или 0.
              \en Fillet radius if possible or 0. \~
  */
  virtual double  GetFilletRadius( const MbCartPoint3D & p, double accuracy = METRIC_REGION ) const;
  /** \brief    \ru Направление поверхности скругления с точностью.
                \en Direction of fillet surface with accuracy. \~
      \details  \ru Направление поверхности скругления с точностью. Точность нужна для сравнения срезов поверхности с
                    дугой окружности для поиска оси вращения.
                \en Direction of fillet surface with accuracy. Accuracy is used to compare surface slices to an arc to find
                    rotation axis. \~
      \param[in] accuracy - \ru Точность.
                            \en Accuracy. \~ 
      \return \ru Направление скругления вдоль параметра поверхности U или V.
              \en Whether the fillet is along U or V direction of the surface. \~
  */
  virtual MbeParamDir GetFilletDirection( double accuracy = METRIC_REGION ) const;
  /// \ru Дать ось вращения для поверхности. \en Get rotation axis of a surface. 
  virtual bool    GetCylinderAxis( MbAxis3D & ) const;
  /// \ru Выдать центр сферической поверхности. \en Give the center of sphere surface. 
  virtual bool    GetCentre( MbCartPoint3D & c ) const;
  /// \ru Дать осевые (центральные) линии для поверхности. \en Get center lines of a surface. 
  virtual bool    GetCenterLines( std::vector<MbCurve3D *> & clCurves ) const;

  /** \brief \ru Изменение носителя.
             \en Changing of carrier. \~
    \details \ru Используется для объединения компланарных граней.
             \en It is used for union of coplanar faces. \~  
  */
  virtual void    ChangeCarrier( const MbSpaceItem & item, MbSpaceItem & init ); 
  
  /** \brief \ru Изменение носимых элементов.
             \en Changing of carrier elements. \~
    \details \ru Используется для объединения компланарных граней. Поверхности item и init должны быть подобны.
             \en It is used for union of coplanar faces. The surfaces 'item' and 'init' must be similar. \~
    \param[in] item - \ru Изменяемая поверхность
                      \en Changed surface \~ 
    \param[in] init - \ru Новая поверхность
                      \en New surface \~
    \param[in] matr - \ru Матрица перехода из item в init
                      \en Transition matrix from 'item' to 'init' \~
    \return \ru true в случае успеха операции \n false в противном случае
            \en True if the operation succeeded \n otherwise false. \~
  */
  virtual bool    ChangeCarrierBorne( const MbSurface & item, MbSurface & init, const MbMatrix & matr ); 
  
  /** \brief \ru Периодичность направляющей.
             \en Periodicity of the guide. \~ 
    \return \ru true если у поверхности есть направляющая и она периодичная
            \en True if a surface has a guide and it is periodic. \~
  */
  virtual bool    IsSpinePeriodic() const; 

  /** \brief \ru Построить NURBS копию поверхности.
             \en Construct a NURBS copy of a surface. \~
    \details \ru Полученная поверхность всегда не замкнута как по u, так и по v.
                 Исключением является сама поверхность NURBS. 
                 Если поверхность не требует пересчета, то выдается ее копия.
             \en An obtained surface is always unclosed in u and in v.
                 The exception is the NURBS surface itself.
                 If the surface does not require recalculation, then a copy of it is returned. \~
    \param[in] bmatch - \ru true, если при преобразовании нужно сохранить однозначное сответствие параметрических областей
                        \en true, if it is required to keep one-to-one correspondence of parametric regions in mapping. \~  
  */ 
  MbSplineSurface * NurbsSurface( bool bmatch = false ) const; 

  /** \brief \ru Построить NURBS копию усеченной поверхности.
             \en Construct a NURBS copy of trimmed surface. \~
    \details \ru Полученная поверхность всегда не замкнута как по u, так и по v.
      Исключением является сама поверхность NURBS. 
      Если поверхность не требует пересчета, то выдается ее копия
      Параметры u1, u2, v1, v2 - границы усечения поверхности.
             \en An obtained surface is always unclosed in u and in v.
      The exception is the NURBS surface itself.
      If the surface does not require recalculation, then a copy of it is returned.
      Parameters u1, u2, v1 and v2 are the bounds of surface trimming. \~
    \param[in] u1 - \ru Минимальный параметр по U
                    \en Minimum parameter in U. \~
    \param[in] u2 - \ru Максимальный параметр по U
                    \en Maximum parameter in U. \~
    \param[in] v1 - \ru Минимальный параметр по V
                    \en Minimum parameter in V. \~
    \param[in] v2 - \ru Максимальный параметр по V
                    \en Maximum parameter in V. \~
    \param[in] bmatch - \ru true, если при преобразовании нужно сохранить однозначное сответствие параметрических областей
                        \en true, if it is required to keep one-to-one correspondence of parametric regions in mapping. \~  
  */
  virtual MbSplineSurface * NurbsSurface( double u1, double u2, double v1, double v2, bool bmatch = false ) const;

  /** \brief \ru Построить NURBS копию поверхности.
             \en Construct a NURBS copy of a surface. \~
    \details \ru Строит NURBS поверхность, аппроксимирующую исходную с заданными параметрами по каждому направлению. 
      В параметрах можно задать степень и количество узлов сплайна, диапазон изменения параметра кривой.
      Если в параметрах не задан флаг точной аппроксимации, то строит NURBS без кратных узлов.
             \en Constructs a NURBS surface which approximates a given surface with the given parameters in each direction. 
      In parameters the degree and the number of knots of a spline and the range of curve's parameters changing may be set.
      If the flag of accurate approximation is not set in parameters then NURBS without multiple knots is constructed. \~
    \param[in] uParam - \ru Параметры построения по направлению u.
                        \en Parameters of construction in u direction. \~
    \param[in] vParam - \ru Параметры построения по направлению v.
                        \en Parameters of construction in v direction. \~
    \result \ru Построенная NURBS поверхность или nullptr при  неуспешном построении.
            \en The constructed NURBS surface or nullptr in a case of failure. \~
  */
  virtual MbSurface * NurbsSurface( const MbNurbsParameters & uParam, const MbNurbsParameters & vParam ) const; 

  /** \brief \ru Подготовить параметры для преобразования в NURBS поверхность.
             \en Prepare parameters for the transformation to NURBS surface. \~
    \details \ru Подготовить параметры для преобразования в NURBS поверхность. 
      Число точек вдоль выбранного направления определяется в переменной tParam.
             \en Prepare parameters for the transformation to NURBS surface. 
      The number of points along the chosen direction if defined by the variable tParam. \~
    \param[in] tParam - \ru Параметры преобразования поверхности в NURBS по одному из параметров.
                        \en Parameters for the transformation of a surface to NURBS with respect to one of parameters. \~
    \param[in] uParam - \ru Какой параметр поверхности рассматривается: \n
      true - готовит аппроксимацию по u
      false - готовит аппроксимацию по v.
                        \en Which parameter is considered: \n
      true - prepares an approximation with respect to u.
      false - prepares an approximation with respect to v. \~
    \param[in] op1 - \ru Минимальное значение второго параметра.
                     \en Minimal value of the second parameter. \~
    \param[in] op2 - \ru Максимальное значение второго параметра.
                     \en Maximal value of the second parameter. \~
    \param[out] isClosedNurbs - \ru true - если аппроксимирующая поверхность замкнута по выбранному направлению.
                                \en True - the approximating surface is closed in the chosen direction. \~
    \param[out] epsilon - \ru На выходе - параметрическая точность по выбранному направлению.
                          \en At the output - parametric tolerance in the given direction. \~
    \param[out] params - \ru Заполненный массив параметров разбиения вдоль выбранного направления.
                         \en A filled array of splitting parameters along the chosen direction. \~
    \return \ru true, если операция прошла успешно.
            \en True if the operation succeeded. \~  
  */
  bool    NurbsParam( const MbNurbsParameters & tParam, bool uParam, double op1, double op2,
                      bool & isClosedNurbs, double & epsilon, SArray<double> & params ) const;  

  /** \brief \ru Выбрать точки для аппроксимации вдоль параметра.
             \en Chose points for approximation along the parameter. \~
    \details \ru Выбрать точки на поверхности для аппроксимации nurbs вдоль параметра в случае незамкнутой по выбранному направлению поверхности. 
      Если поверхность аппроксимируется с заданным узловым вектором, то проверяются параметры разбиения
      для того, чтобы между элементами узлового вектора содержалась бы хотя бы одна точка разбиения. 
      При необходимости список точек разбиения дополняется.
      Если узловой вектор не задан (пустой), то вычисляется узловой вектор в соответствии с порядком, 
      количеством узлов и предложенным разбиением.
             \en Chose points on a surface for NURBS approximation along the parameter in the case of unclosed in the chosen direction surface. 
      If a surface is approximated with the given knot vector, then splitting parameters are checked
      in order to leave at least one splitting point between the elements of a knot vector. 
      If necessary, the list of splitting points is complemented.
      If a knot vector is not set (empty), then the knot vector is calculated according to the order, 
      the number of knots and the proposed splitting. \~
    \param[in] isU - \ru Какой параметр поверхности рассматривается: \n
      true - готовит аппроксимацию по u.
      false - готовит аппроксимацию по v.
                     \en Which parameter is considered: \n
      true - prepares an approximation with respect to u.
      false - prepares an approximation with respect to v. \~
    \param[in] par - \ru Значение второго параметра.
                     \en A value of second parameter. \~
    \param[in] degree - \ru Порядок NURBS-поверхности по выбранному направлению.
                        \en An order of NURBS surface by a chosen direction. \~
    \param[in] pCount - \ru Количество узлов NURBS-поверхности по выбранному направлению.
                        \en A number of knots of NURBS surface by a chosen direction. \~
    \param[in,out] tList - \ru Параметры разбиения поверхности по выбранному направлению. Вычисляются в функции NurbsParam.
                           \en A splitting parameters of NURBS surface by a chosen direction. They are calculated in the NurbsParam function. \~
    \param[in,out] aKnots - \ru Узловой вектор NURBS-поверхности.
                            \en A knot vector of NURBS surface. \~
  */
  void    CheckApproxPointParamsOpen( bool isU, double par, size_t degree, size_t pCount,
                                      SArray<double> & tList, SArray<double> & aKnots ) const;

  /** \brief \ru Выбрать точки для аппроксимации вдоль параметра.
             \en Chose points for approximation along the parameter. \~
    \details \ru Выбрать точки на поверхности для аппроксимации nurbs вдоль параметра в случае замкнутой по выбранному направлению поверхности. 
      Если поверхность аппроксимируется с заданным узловым вектором, то проверяются параметры разбиения
      для того, чтобы между элементами узлового вектора содержалась бы хотя бы одна точка разбиения. 
      При необходимости список точек разбиения дополняется.
      Если узловой вектор не задан (пустой), то вычисляется узловой вектор в соответствии с порядком, 
      количеством узлов и предложенным разбиением.
             \en Chose points on a surface for NURBS approximation along the parameter in a case of closed in the chosen direction surface. 
      If a surface is approximated with the given knot vector then splitting parameters are checked
      in order to leave at least one splitting point between the elements of a knot vector. 
      If necessary, the list of splitting points is complemented.
      If a knot vector is not set (empty) then the knot vector is calculated according to the order, 
      the number of knots and the proposed splitting. \~
    \param[in] isU - \ru Какой параметр поверхности рассматривается: \n
      true - готовит аппроксимацию по u
      false - готовит аппроксимацию по v.
                     \en Which parameter is considered: \n
      true - prepares an approximation with respect to u.
      false - prepares an approximation with respect to v. \~
    \param[in] par - \ru Значение второго параметра.
                     \en A value of second parameter. \~
    \param[in] degree - \ru Порядок NURBS-поверхности по выбранному направлению.
                        \en An order of NURBS surface by a chosen direction. \~
    \param[in] pCount - \ru Количество узлов NURBS-поверхности по выбранному направлению.
                        \en A number of knots of NURBS surface by a chosen direction. \~
    \param[in,out] tList - \ru Параметры разбиения поверхности по выбранному направлению. Вычисляются в функции NurbsParam.
                           \en A splitting parameters of NURBS surface by a chosen direction. They are calculated in the NurbsParam function. \~
    \param[in,out] aKnots - \ru Узловой вектор NURBS-поверхности.
                            \en A knot vector of NURBS surface. \~
  */
  void    CheckApproxPointParamsClosed( bool isU, double par, size_t degree, size_t pCount,
                                        SArray<double> & tList, SArray<double> & aKnots ) const;

  // \ru Построить эквидистантую поверхность. \en Create an offset surface. 
  /** \brief \ru Построить эквидистантую поверхность.
             \en Create an offset surface. \~
    \param[in] d - \ru Расстояние по нормали от базовой до эквидистантной поверхности.
                   \en Distance by the normal vector from the base surface to the offset surface. \~
    \param[in] same - \ru Флаг, показывающий, использовать ли в эквидистантной поверхности текущую поверхность или ее копию.
                      \en A flag showing whether to use the current surface or its copy in offset surface. \~
  */ 
  virtual MbSurface * Offset( double d, bool same ) const; 
  
  /** \brief \ru Построить пространственную копию линии v = const.
             \en Construct spatial copy of the line v = const. \~
    \param[in] v -        \ru Параметр по направлению V. \en Parameter in V direction. \~
    \param[in] pRgn -     \ru Диапазон параметров по U. \en A range of u-parameters. \~
    \param[in] bApprox -  \ru Если false, то вернет не ноль только в случае, если получится создать точную кривую. \n
                              Если true, то вернет не ноль, если кривая не вырождена.
                          \en If false, then returns non-null only in a case when the exact curve has been successfully created. \n
                              If true, then returns non-null in a case when the curve is not degenerated. \~
    \result \ru Построенная кривая.
            \en Constructed curve. \~
  */ 
  virtual MbCurve3D * CurveU ( double v, MbRect1D * pRgn, bool bApprox = true ) const;     
  
  /** \brief \ru Построить пространственную копию линии u = const.
             \en Construct spatial copy of the line u = const. \~  
    \param[in] u - \ru Параметр по направлению U.
                   \en Parameter in U direction. \~
    \param[in] pRgn - \ru Диапазон параметров по V.
                      \en A range of v-parameters. \~
    \param[in] bApprox - \ru Если false, то вернет не ноль только в случае, если получится создать точную кривую. \n
      Если true, то вернет не ноль, если кривая не вырождена.
                         \en If false, then it returns null only in a case when the exact curve has been successfully created. \n
      If true, then it returns null in a case when the curve is not degenerate. \~
    \result \ru Построенная кривая.
            \en Constructed curve. \~
  */ 
  virtual MbCurve3D * CurveV ( double u, MbRect1D * pRgn, bool bApprox = true ) const;   
  
  /** \brief \ru Построить пространственную копию линии по параметрической линии.
             \en Construct spatial copy of line by parametric line. \~
    \param[in] segm - \ru Отрезок в параметрической плоскости поверхности, пространственную копию которого надо построить.
                      \en A segment in parametric plane of surface, a spatial copy of which is required to construct. \~
    \param[in] bApprox - \ru Если false, то вернет не ноль только в случае, если получится создать точную кривую. \n
      Если true, то вернет не ноль, если кривая не вырождена.
                         \en If false, then it returns null only in a case when the exact curve has been successfully created. \n
      If true, then it returns null in a case when the curve is not degenerate. \~
  */ 
  virtual MbCurve3D * CurveUV( const MbLineSegment & segm, bool bApprox = true ) const; 
                  
  /** \brief \ru Определить, с какой стороны от поверхности находится точка.
             \en Get point location relative to the surface. \~
    \param[in] pnt - \ru Исследуемая точка.
                     \en The investigated point. \~
    \param[in] eps - \ru Точность определения попадания точки на поверхность.
                     \en Tolerance of getting a point onto a surface. \~
    \return \ru iloc_InItem = 1 - точка над поверхностью (со стороны нормали). \n
      iloc_OnItem = 0 - точка на поверхности. \n
      iloc_OutOfItem = -1 - точка под поверхностью.
            \en Iloc_InItem = 1 - point is located over the surface (from the side of normal vector) \n
      iloc_OnItem = 0 - point is located on the surface, \n
      iloc_OutOfItem = -1 - point is located under the surface. \~
  */
  virtual MbeItemLocation PointRelative( const MbCartPoint3D & pnt, double eps = ANGLE_REGION ) const;    

  /** \brief \ru Находится ли точка в области, принадлежащей поверхности.
             \en A point is located inside the region on a surface. \~
    \details \ru Исследуется двумерная точка в параметрической плоскости поверхности.
             \en Investigated two-dimensional point in the parametric plane of a surface. \~
    \param[in] pnt - \ru Исследуемая точка.
                     \en The investigated point. \~
    \param[in] ignoreClosed - \ru Учитывать ли замкнутость поверхности. Если true, то замкнутость не учитывается.
                              \en Whether to consider the surface closedness. If true, then the closedness is not considered. \~ 
    \return \ru iloc_InItem - точка в области поверхности.\n
                iloc_OnItem - точка на границе поверхности.\n
                iloc_OutOfItem - точка вне области поверхности.
            \en iloc_InItem - point is inside surface region.\n
                iloc_OnItem - point belongs to surface boundary.\n
                iloc_OutOfItem - point is outside surface region.\~
  */
  virtual MbeItemLocation PointClassification( const MbCartPoint & pnt, bool ignoreClosed = false ) const; 

  /** \brief \ru Вычислить параметрическое расстояние до ближайшей границы.
             \en Calculate the parametric distance to the nearest boundary. \~
    \details \ru Найденное расстояние до ближайшей границы имеет положительное значение, если точка находится внутри, и отрицательное - если снаружи.
             \en The calculated distance is positive if the point is inside, and is negative if it is outside. \~
    \param[in] point - \ru Исследуемая точка.
                       \en The investigated point. \~
    \param[in] epsilon - \ru Точность определения попадания точки на поверхность.
                         \en Tolerance of getting a point onto a surface. \~
    \return \ru Возвращает расстояние до границы.
            \en Returns the distance to the boundary. \~
  */
  virtual double  DistanceToBorder( const MbCartPoint & point, double & epsilon ) const;

  /** \brief \ru Определить точки пересечения кривой с контурами поверхности.
             \en Determine points of intersections between the curve and the surface contours. \~
    \details \ru Определить точки пересечения кривой с границами поверхности
      для нахождения частей кривой в пределах поверхности
      и векторов для их сдвига в пределы поверхности.
      Вектор сдвига определен для каждой точки пересечения.
      Если вектор сдвига найти нельзя, то в массив сохраняется вектор нулевой длины.
      Он может быть ненулевым, если поверхность замкнута хотя бы по одному из направлений.
      Тогда вектор сдвига позволяет сдвинуть часть кривой на некоторое количество периодов так,
      чтобы найденная точка пересечения кривой с границей находилась бы в пределах поверхности и 
      производная к кривой в этой точке была бы направлена внутрь поверхности.
      С помощью вектора сдвига кривую можно сдвинуть на некоторое количество периодов. 
      То есть метрически кривая не изменится.
             \en Determine points of intersections between the curve and the surface boundaries
      in order to find parts of the curve inside the surface region
      and vectors for their translation to the surface region.
      Translation vector is defined for each intersection point.
      If it is impossible to find a translation vector, then the vector with null size is saved into the array.
      It can be non-null if a surface is closed at least in one of directions.
      In this case the translation vector allows to move a part of a curve by a certain number of periods in such way
      that the found point of intersection between the curve and the boundary is located inside the surface region and 
      the derivative of a curve at this point is directed inside the surface.
      Using the translation vector a curve can be moved by a certain number of periods. 
      I.e. metrically the curve will not change. \~
    \param[in] curve - \ru Заданная кривая.
                       \en A given curve. \~
    \param[in,out] tcurv - \ru Множество параметров кривой, соответствующих точкам пересечения.
                           \en A set of curve parameters corresponding to the intersection points. \~
    \param[in,out] dir - \ru Множество векторов сдвига.
                         \en A set of translation vectors. \~
    \return \ru Количество пересечений.
            \en The number of intersections. \~
  */
  virtual size_t  CurveClassification( const MbCurve & curve, SArray<double> & tcurv, SArray<MbVector> & dir ) const; 

  /** \brief \ru Определить точки пересечения кривой с контурами поверхности.
             \en Determine points of intersections between the curve and the surface contours. \~
    \details \ru Определить точки пересечения кривой с границами поверхности
      для нахождения частей кривой в пределах поверхности
      и векторов для их сдвига в пределы поверхности.
      Вектор сдвига определен для каждой точки пересечения.
      Если вектор сдвига найти нельзя, то в массив сохраняется вектор нулевой длины.
      Он может быть ненулевым, если поверхность замкнута хотя бы по одному из направлений.
      Тогда вектор сдвига позволяет сдвинуть часть кривой на некоторое количество периодов так,
      чтобы найденная точка пересечения кривой с границей находилась бы в пределах поверхности и 
      производная к кривой в этой точке была бы направлена внутрь поверхности.
      С помощью вектора сдвига кривую можно сдвинуть на некоторое количество периодов. 
      То есть метрически кривая не изменится.
             \en Determine points of intersections between the curve and the surface boundaries
      in order to find parts of a curve inside the surface region
      and vectors for their translation to the surface region.
      Translation vector is defined for each intersection point.
      If it is impossible to find a translation vector then the vector with null size is saved into the array.
      It can be non-null if a surface is closed at least in one of directions.
      In this case the translation vector allows to move a part of a curve by a certain number of periods in such way
      that the found point of intersection between the curve and the boundary is located inside the surface region and 
      the derivative of a curve at this point is directed inside the surface.
      Using the translation vector a curve can be moved by a certain number of periods. 
      I.e. metrically the curve will not change. \~
    \param[in] curve - \ru Заданная кривая.
                       \en A given curve. \~
    \param[in,out] tcurv - \ru Множество параметров кривой, соответствующих точкам пересечения.
                           \en A set of curve parameters corresponding to the intersection points. \~
    \param[in,out] dir - \ru Множество векторов сдвига.
                         \en A set of translation vectors. \~
    \return \ru Количество пересечений.
            \en The number of intersections. \~
  */
  size_t  SurfaceBorderIntersection( const MbCurve & curve, SArray<double> & tcurv, SArray<MbVector> & dir ) const; 
  /// \ru Нахождение проекции точки на поверхность. Для внутреннего использования. \en Finding of point projection on surface. For internal use only. 
  virtual MbeNewtonResult PointProjectionNewton( const MbCartPoint3D & p, size_t iterLimit,
                                                 double & u, double & v, bool ext ) const; 
  /** \brief \ru Найти проекцию точки на поверхность.
             \en Find the projection of a point onto the surface. \~
    \details \ru Найти ближайшую проекцию точки на поверхность или ее продолжение по заданному начальному приближению. 
      Если задан диапазон изменения параметров uvRange, то надо найти проекцию в заданном диапазоне.
      Диапазон параметров может выходить за область определения параметров поверхности.
      Используется метод Ньютона.
             \en Find the nearest point projection to the surface or its extension by the given initial approximation. 
      If the range of parameters changing 'uvRange' is set, then it is required to find a projection in the given range.
      A range of parameters may not belong to the domain of a surface.
      The Newton method is used. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
    \param[in,out] u - \ru На входе - начальное приближение, на выходе - параметр, соответствующий ближайшей проекции.
                       \en Input - initial approximation, output - parameter of a surface, corresponding to the nearest projection. \~
    \param[in,out] v - \ru На входе - начальное приближение, на выходе - параметр, соответствующий ближайшей проекции.
                       \en Input - initial approximation, output - parameter of a surface, corresponding to the nearest projection. \~
    \param[in] ext - \ru Флаг, определяющий, искать ли проекцию на продолжении поверхности (если true, то искать).
                     \en A flag defining whether to seek projection on the extension of the surface. \~ 
    \param[in] uvRange - \ru Диапазон изменения параметров, в котором надо найти решение.
                         \en A range of parameters changing in which the solution should be found. \~
    \result \ru true - если найдена проекция, удовлетворяющая всем входным условиям.
            \en True - if there is found a projection which satisfies to all input conditions. \~
  */
  virtual bool    NearPointProjection( const MbCartPoint3D & pnt, double & u, double & v, bool ext, MbRect2D * uvRange = nullptr ) const;
  /// \ru Нахождение проекции точки на поверхность в направлении вектора. Для внутреннего использования. \en Finding of point projections to the surface in direction of the vector. For internal use only. 
  virtual MbeNewtonResult DirectPointProjectionNewton( const MbCartPoint3D & p, const MbVector3D & vect, size_t iterLimit,
                                                       double & u, double & v, double & w, bool ext ) const; 

  /** \brief \ru Найти все проекции точки на поверхность вдоль вектора в любом из двух направлений.
             \en Find all a point projection onto the surface along a vector in either of two directions. \~
    \details \ru Вычислить все точки пересечения поверхности с лучом, выходящим из заданной точки p по направлению vect.
      Если задан диапазон изменения параметров uvRange, то надо найти проекцию в заданном диапазоне.
      Диапазон параметров может выходить за область определения параметров поверхности.
             \en Calculate all points of intersection with the ray outgoing from the given point 'p' by the direction 'vect'.
      If the range of parameters changing 'uvRange' is set, then it is required to find a projection in the given range.
      A range of parameters may not belong to the domain of a surface. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
    \param[in] vect - \ru Вектор направления.
                      \en A direction vector. \~
    \param[out] uv - \ru Множество точек проекции в области определения поверхности.
                     \en A set of projection points in the area of surface definition. \~
    \param[in] ext - \ru Флаг, определяющий, искать ли проекцию на продолжении поверхности (если true, то искать).
                     \en A flag defining whether to seek projection on the extension of the surface. \~ 
    \param[in] uvRange - \ru Диапазон изменения параметров поверхности, в котором надо найти решение.
                         \en A range of surface parameters changing in which the solution should be found. \~
  */
  virtual void    DirectPointProjection( const MbCartPoint3D & pnt, const MbVector3D & vect, SArray<MbCartPoint> & uv, bool ext, MbRect2D * uvRange = nullptr ) const;

  /** \brief \ru Найти ближайшую проекцию точки на поверхность в направлении вектора.
             \en Find the nearest point projection to the surface in the vector direction. \~
    \details \ru Вычислить ближайшую точку пересечения поверхности с лучом, выходящим из заданной точки p по направлению vect.
      Если задан диапазон изменения параметров uvRange - то надо найти проекцию в заданном диапазоне.
      Диапазон параметров может выходить за область определения параметров поверхности.
             \en Calculate the nearest point of intersection with the ray outgoing from the given point 'p' by the direction 'vect'.
      If the range of parameters changing 'uvRange' is set, then it is required to find a projection in the given range.
      A range of parameters may not belong to the domain of a surface. \~
    \param[in] pnt - \ru Заданная точка.
                     \en A given point. \~
    \param[in] vect - \ru Вектор направления.
                      \en A direction vector. \~
    \param[in,out] u - \ru На входе - начальное приближение, на выходе - параметр, соответствующий ближайшей проекции.
                       \en Input - initial approximation, output - parameter of a surface, corresponding to the nearest projection. \~
    \param[in,out] v - \ru На входе - начальное приближение, на выходе - параметр, соответствующий ближайшей проекции.
                       \en Input - initial approximation, output - parameter of a surface, corresponding to the nearest projection. \~
    \param[in] ext - \ru Флаг, определяющий, искать ли проекцию на продолжении кривой (если true, то искать).
                     \en A flag defining whether to seek projection on the extension of the curve. \~ 
    \param[in] uvRange - \ru Диапазон изменения параметров, в котором надо найти решение.
                         \en A range of parameters changing in which the solution should be found. \~
    \result \ru true - если найдена проекция, удовлетворяющая всем входным условиям.
            \en True - if there is found a projection which satisfies to all input conditions. \~
  */
  virtual bool    NearDirectPointProjection( const MbCartPoint3D & pnt, const MbVector3D & vect, double & u, double & v, bool ext,
                                             MbRect2D * uvRange = nullptr, bool onlyPositiveDirection = false ) const;
  /// \ru Решение системы уравнений для определения пересечения поверхности и кривой. Для внутреннего использования. \en Solution of equation system for determination of intersections between a surface and a curve. For internal use only. 
  virtual MbeNewtonResult CurveIntersectNewton( const MbCurve3D & curv1, double funcEpsilon, size_t iterLimit,
                                                double & u0, double & v0, double & t1, bool ext0, bool ext1 ) const; 
  /// \ru Решение системы уравнений для определения касания поверхности и кривой. Для внутреннего использования. \en Solution of equation system for determination of tangency between a surface and a curve. For internal use only. 
  virtual MbeNewtonResult CurveTangentNewton  ( const MbCurve3D & curv1, double funcEpsilon, size_t iterLimit,
                                                double & u0, double & v0, double & t1, bool ext0, bool ext1 ) const; 

  /** \brief \ru Определить точки пересечения поверхности и кривой.
             \en Determine points of intersection between a surface and a curve. \~
    \details \ru Определить точки пересечения поверхности и кривой. \n
             \en Determine points of intersection between a surface and a curve. \n \~
    \param[in] curv - \ru Заданная кривая.
                      \en A given curve. \~
    \param[in,out] uv - \ru Множество точек пересечения на поверхности.
                        \en A set of intersection points on the surface. \~
    \param[in,out] tt - \ru Множество точек пересечения на кривой.
                        \en A set of intersection points on the curve. \~
    \param[in] ext0 - \ru Флаг, определяющий, искать ли пересечения на расширенной поверхности (если true, то искать).
                      \en A flag defining whether to seek intersections on extended surface (if it is true, then seek). \~ 
    \param[in] ext - \ru Флаг, определяющий, искать ли пересечения на продолжении кривой (если true, то искать).
                     \en A flag defining whether to seek intersections on extended curve (if it is true, then seek). \~ 
    \param[in] touchInclude - \ru Считать ли касание пересечением. Если true, то считать.
                              \en Whether to consider tangency as intersection. If true, then consider. \~
  */
  virtual void    CurveIntersection( const MbCurve3D & curv, SArray<MbCartPoint> & uv, SArray<double> & tt, 
                                     bool ext0, bool ext, bool touchInclude = false ) const; 

   /// \ru Решение системы уравнений для определения пересечения поверхностей. Для внутреннего использования. \en Solution of equation system for determination of surfaces intersections. For internal use only. 
  virtual MbeNewtonResult SurfaceIntersectNewton( const MbSurface & surf1, MbeParamDir switchPar, double funcEpsilon, size_t iterLimit,
                                                  double & u0, double & v0, double & u1, double & v1, bool ext0, bool ext1, VERSION version = Math::DefaultMathVersion() ) const;
  /// \ru Решение системы уравнений для определения касания поверхностей. Для внутреннего использования. \en Solution of equation system for determination of surfaces tangency. For internal use only. 
  virtual MbeNewtonResult SurfaceTangentNewton( const MbSurface & surf1, MbeParamDir switchPar, double funcEpsilon, size_t iterLimit,
                                                double & u0, double & v0, double & u1, double & v1, bool ext0, bool ext1, VERSION version = Math::DefaultMathVersion() ) const;
  /// \ru Решение системы уравнений для определение точек очерка поверхности. Для внутреннего использования. \en Solution of equation system for determination of surface silhouette points. For internal use only.  
  virtual MbeNewtonResult SilhouetteNewton( const MbVector3D & eye, bool perspective, const MbAxis3D * axis, MbeParamDir switchPar,  
                                            double funcEpsilon, size_t iterLimit, double & u, double & v, bool ext ) const; 

  /** \brief \ru Определить, подобны ли поверхности для объединения.
             \en Define whether the surfaces are similar for merge. \~
    \details \ru Поверхности подобны для объединения, если геометрически они совпадают или переходят одна в другую.
             \en Surfaces are similar for merge if they coincide geometrically or one surface transits to another. \~
    \param[in] surf - \ru Заданная поверхность.
                      \en A given surface. \~
    \param[in] version - \ru Версия операции.
                         \en Version of operation. \~
    \param[in] precision - \ru Погрешность вычислений.
                           \en Precision of calculation. \~
    \result \ru true - если поверхности подобны.
            \en True - if surfaces are similar. \~
  */
  virtual bool    IsSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const;
   /// \ru Подобные ли поверхности для объединения (слива). Специальный случай. Для внутреннего использования. \en Whether the surfaces are similar to merge. Special case. For internal use only. 
  virtual bool    IsSpecialSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const;

  /** \brief \ru Построение проекции поверхностной кривой на совпадающую поверхность.
             \en Construction of projection of a surface curve on coincident surface. \~
    \details \ru Строится проекция кривой пересечения поверхностей на некоторую заданную поверхность.
      В функции подразумевается, что заданная поверхность подобна одной из несущих поверхностей кривой пересечения, 
      но этот факт не проверяется.
             \en A projection of surface intersection curve is constructed on a given surface.
      It is implied that the given surface is similar to one of surfaces of the intersection curve 
      but this fact is not checked. \~ 
    \param[in] spaceCurve - \ru Заданная кривая пересечения.
                            \en A given intersection curve. \~
    \param[in] curve - \ru Двумерная кривая на поверхности.
                       \en A two-dimensional curve on a surface. \~
    \param[in] surfNew - \ru Новая поверхность.
                         \en New surface. \~
    \param[out] curveNew - \ru Полученная двумерная кривая на новой поверхности.
                           \en An obtained two-dimensional curve on new surface. \~
    \result \ru true - если операция прошла успешно.
            \en True - if the operation succeeded. \~
  */
  virtual bool    ProjectCurveOnSimilarSurface( const MbCurve3D & spaceCurve, const MbCurve & curve, const MbSurface & surfNew, MbCurve *& curveNew ) const;
  /** \brief \ru Построение проекции поверхностной кривой на совпадающую поверхность.
             \en Construction of projection of a surface curve on coincident surface. \~
    \details \ru Строится проекция кривой пересечения поверхностей на некоторую заданную поверхность.
      В функции подразумевается, что заданная поверхность подобна одной из несущих поверхностей кривой пересечения, 
      но этот факт не проверяется.
             \en A projection of surface intersection curve is constructed on a given surface.
      It is implied that the given surface is similar to one of surfaces of the intersection curve 
      but this fact is not checked. \~ 
    \param[in] spaceCurve - \ru Заданная кривая пересечения.
                            \en A given intersection curve. \~
    \param[in] curve - \ru Двумерная кривая на поверхности.
                       \en A two-dimensional curve on a surface. \~
    \param[in] surfNew - \ru Новая поверхность.
                         \en New surface. \~
    \param[out] curveNew - \ru Полученная двумерная кривая на новой поверхности.
                           \en An obtained two-dimensional curve on new surface. \~
    \result \ru true - если операция прошла успешно.
            \en True - if the operation succeeded. \~
  */
  bool    ProjectCurveOnSimilarSurface( const MbCurve3D & spaceCurve, const MbCurve & curve, const MbSurface & surfNew, SPtr<MbCurve> & curveNew ) const;

  /** \brief \ru Дать двумерную матрицу преобразования из своей параметрической области в параметрическую область surf.
             \en Get two-dimensional matrix of transformation from its parametric region to the parametric region of 'surf'. \~
    \details \ru Вычисление матрицы преобразования из одной параметрической области в другую 
      осуществляется для поверхностей, подобных для объединения.
             \en Calculation of matrix of transformation from one parametric region to another. 
      it is performed for the surfaces which are similar for merge. \~
    \param[in] surf - \ru Заданная поверхность.
                      \en A given surface. \~
    \param[out] matr - \ru Матрица преобразования.
                       \en A transformation matrix. \~
    \param[in] version - \ru Версия операции.
                         \en Version of operation. \~
    \param[in] precision - \ru Погрешность вычислений.
                           \en Precision of calculation. \~
    \result \ru true - матрица вычислена.
            \en True - matrix is calculated. \~
  */
  virtual bool    GetMatrixToSurface( const MbSurface & surf, MbMatrix & matr, VERSION version, double precision = METRIC_PRECISION ) const;

  /// \ru Определить, выпуклая ли поверхность. \en Determine whether the surface is convex. 
  /** \brief \ru Определить, выпуклая ли поверхность.
             \en Determine whether the surface is convex.  \~
    \details \ru Определить, выпуклая ли поверхность.
             \en Determine whether the surface is convex. 
    \result \ru ts_positive - поверхность выпуклая, ts_negative - поверхность вогнутая, ts_neutral - невозможно опеределить.
            \en ts_positive - the surface is convex, ts_negative - the surface is concave, ts_neutral - impossible to determine. \~
  */
  virtual ThreeStates Salient() const; 

  /** \brief \ru Вычислить ближайшее расстояние до кривой.
             \en Calculate the nearest distance to a curve. \~
    \details \ru Вычисляется минимальное расстояние между кривой и поверхностью. 
      Определяются точки на поверхности и кривой, между которыми расстояние минимально.
             \en Calculation of the minimum distance between a curve and a surface. 
      Determination of the points on a surface and a curve where the minimum distance is reached. \~
    \param[in] curve - \ru Заданная кривая.
                       \en A given curve. \~
    \param[out] u - \ru Координата вычисленной точки на поверхности.
                    \en Coordinate of the calculated point on surface. \~
    \param[out] v - \ru Координата вычисленной точки на поверхности.
                    \en Coordinate of the calculated point on surface. \~
    \param[in,out] t - \ru На входе - начальное приближение к искомой точке на кривой. 
      На выходе - параметр вычисленной точки на кривой.
                       \en Input - an initial approximation to the required point on a curve. 
      Output - a parameter of the calculated point on a curve. \~
    \param[in] tCalc - \ru Флаг, показывающий, использовать ли t в качестве начального приближения.
                       \en A flag showing whether to use t as initial approximation. \~
    \result \ru Минимальное расстояние между кривой и поверхностью.
            \en The minimum distance between a curve and a surface. \~
  */
  virtual double  DistanceToCurve( const MbCurve3D & curve, double & u, double & v, double & t, bool tCalc = false ) const; // \ru tCalc - флаг инициализации t \en TCalc - flag of initialization of t.  

  /** \brief \ru Вычислить ближайшее расстояние до поверхности.
             \en Calculate the nearest distance to a surface. \~
    \details \ru Вычисляется минимальное расстояние между двумя поверхностями. 
      Определяются точки на поверхностях, между которыми расстояние минимально.
             \en Calculating of minimum distance between two surfaces. 
      Determination of the points on surfaces where the minimum distance is reached. \~
    \param[in] surf1 - \ru Заданная поверхность.
                       \en A given surface. \~
    \param[out] u0 - \ru Координата вычисленной точки на текущей поверхности.
                     \en Coordinate of the calculated point on the current surface. \~
    \param[out] v0 - \ru Координата вычисленной точки на текущей поверхности.
                     \en Coordinate of the calculated point on the current surface. \~
    \param[out] u1 - \ru Координата вычисленной точки на заданной поверхности.
                     \en Coordinate of the calculated point on the given surface. \~
    \param[out] v1 - \ru Координата вычисленной точки на заданной поверхности.
                     \en Coordinate of the calculated point on the given surface. \~
    \result \ru Минимальное расстояние между поверхностями.
            \en Minimal distance between surfaces. \~
  */
  virtual double  DistanceToSurface( const MbSurface & surf1, double & u0, double & v0, double & u1, double & v1 ) const;

  /// \ru Построить нормальные плейсменты конструктивных плоскостей. \en Construct normal placements of constructive planes.    
  virtual bool    CreateNormalPlacements ( const MbVector3D & axisZ, double angle, SArray<MbPlacement3D> & places, VERSION version = Math::DefaultMathVersion() ) const;
  /// \ru Построить касательные плейсменты конструктивных плоскостей. \en Construct tangent placements of constructive planes. 
  virtual bool    CreateTangentPlacements( const MbVector3D & axisZ, SArray<MbPlacement3D> & places ) const; 
          /// \ru Построить касательные плейсменты конструктивных плоскостей. Для внутреннего использования. \en Construct tangent placements of constructive planes. For internal use only. 
          MbeNewtonResult PlacementNewton( const MbVector3D & vec, double angle, MbeParamDir switchPar, size_t iterLimit, double & u, double & v ) const; 
          /// \ru Построить нормальные или касательные плейсменты на v-линиях. \en Construct the normal or tangent placements on v-lines. 
  bool    CreateVconstPlacements ( const MbVector3D & axisZ, double angle, bool normalPlace, SArray<MbPlacement3D> & places ) const; 
          /// \ru Построить нормальные или касательные плейсменты на u-линиях. \en Construct the normal or tangent placements on u-lines. 
  bool    CreateUconstPlacements ( const MbVector3D & axisZ, double angle, bool normalPlace, SArray<MbPlacement3D> & places ) const; 


  /// \ru Вычислить площадь области определения параметров. \en Calculate the area of parameters domain. 
  virtual double  ParamArea() const; 

  /** \brief \ru Вычислить U-пары от V.
             \en Calculate U-pairs by V. \~
    \details \ru Вычислить значимые параметры u поверхности при заданном параметре v. 
      В случае поверхности общего вида - минимальное и максимальное значения параметра u.
             \en Calculate significant u-parameters of a surface with the given parameter v. 
      In a case of general form - minimal and maximal values of u-parameter. \~
    \param[in] v - \ru Заданный параметр.
                   \en Given parameters. \~
    \param[out] u - \ru Множество значений u.
                    \en A set of values of u. \~
    \result \ru Количество вычисленных значений u.
            \en The number of calculated values of u. \~
  */
  virtual size_t  GetUPairs( double v, SArray<double> & u ) const; 

  /** \brief \ru Вычислить V-пары от U.
             \en Calculate V-pairs by U. \~
    \details \ru Вычислить значимые параметры v поверхности при заданном параметре u. 
      В случае поверхности общего вида - минимальное и максимальное значения параметра v.
             \en Calculate significant v-parameters of a surface with the given parameter u. 
      In case of a surface of general form - minimal and maximal values of v-parameter. \~
    \param[in] u - \ru Заданный параметр.
                   \en Given parameters. \~
    \param[out] v - \ru Множество значений v.
                    \en A set of values of v. \~
    \result \ru Количество вычисленных значений v.
            \en The number of calculated values of v. \~
  */
  virtual size_t  GetVPairs( double u, SArray<double> & v ) const; 

  /// \ru Определение параметров точки изоклины поверхности. Для внутреннего использования. \en Determination of parameters of a surface isocline point. For internal use only. 
  MbeNewtonResult IsoclinalNewton( const MbVector3D & dir, size_t iterLimit, double & u, double & v ) const;

  /** \brief \ru Найти все изоклины поверхности.
             \en Find all isoclines of a surface. \~
    \details \ru Найти точки на поверхности, в которых касательная плоскость параллельна некоторой плоскости,
      имеющей нормаль nor.
             \en Find the points on a surface where the tangent plane is parallel to a plane
      having a normal nor. \~
    \param[in] nor - \ru Вектор, задающий плоскость.
                     \en A vector which defines a plane. \~
    \param[out] uv - \ru Множество параметров точек с искомой касательной.
                     \en A set of  parameters of points for the required tangent. \~
  */
  virtual void    GetIsoclinal( const MbVector3D & nor, SArray<MbCartPoint> & uv ) const;
  
  /// \ru Рассчитать габарит поверхности. Рекомендуется использовать GetGabarit. \en Calculate bounding box of surface. It is recommended to use GetGabarit. 
  virtual void    CalculateGabarit( MbCube & cube ) const;
  /// \ru Рассчитать габарит в локальной системе координат. \en Calculate bounding box in the local coordinate system. 
  void    CalculateLocalGabarit( const MbMatrix3D & into, MbCube & cube ) const override;

  /// \ru Рассчитать габаритный куб поверхности. \en Calculate bounding box of surface. 
  const   MbCube & GetGabarit() const { if ( cube.IsEmpty() ) { MbCube tmp;  CalculateGabarit( tmp ); } return cube; }
  /// \ru Вернуть сохраненный габаритный куб. Он должен быть пустой. Рекомендуется использовать GetGabarit. \en Return saved bounding box. It should be empty. It is recommended to use GetGabarit. 
  const   MbCube & Cube()       const { return cube; }
  /// \ru Сделать габарит пустым. Для внутреннего использования. \en Make the bounding box empty. For internal use only. 
  void    SetDirtyGabarit() const { ScopedRecursiveLock ll( GetLock() ); cube.SetEmpty(); }

  /** \brief \ru Скопировать габаритный куб из копии.
             \en Copy the bounding box from the copy. \~
    \details \ru Скопировать из копии готовые метрические оценки, которые в оригинале не были расчитаны.
             \en Copy from the copy ready estimates which were not calculated in the original. \~
    \warning \ru Для скорости проверка идентичности оригинала и копии не выполняется!
             \en A check of identity between a copy and an original is not performed for the time saving! \~
    \param[in] s - \ru Поверхность-копия.
                   \en A surface-copy. \~
  */
  void    CopyGabarit( const MbSurface & s, const MbVector3D * to = nullptr ) { cube = s.cube; if ( (to != nullptr) && !cube.IsEmpty() ) { cube.Move( *to ); } }
  /// \ru Вычислить диагональ габаритного куба. \en Calculate the diagonal of the bounding box. 
  double  GetGabDiagonal() const { if ( cube.IsEmpty() ) { MbCube tmp; CalculateGabarit( tmp ); } return cube.GetDiagonal(); }

  /** \brief \ru Вычислить прямоугольный габарит поверхности в заданной плоскости.
             \en Calculate the rectangular bounding box of a surface in the given plane. \~
    \details \ru Выдать прямоугольный габарит поверхности в плоскости XOY плейсмента.
             \en Get the rectangular bounding box of surfaces in the XAOY plane of the placement. \~  
    \param[in] place - \ru Заданный плейсмент.
                       \en The given placement. \~
    \param[out] rect - \ru Вычисленный прямоугольник.
                       \en Calculated rectangle. \~
  */
  void    CalculateRect( const MbPlacement3D & place, MbRect & rect ) const;

  /** \brief \ru Вернуть граничный двумерный контур.
             \en Return the bounding two-dimensional contour. \~
    \details \ru Функция создает новый двумерный граничный контур.
      После использования объект надо удалить.
             \en The function creates new two-dimensional bounding contour.
      The object should be deleted after using. \~
    \param[in] sense - \ru Совпадает ли направление контура с направлением обхода против часовой стрелки.
                       \en Whether the contour direction coincides with the counterclockwise traverse direction. \~
    \result \ru Граничный контур.
            \en Bounding contour. \~
  */
  virtual MbContour & MakeContour( bool sense ) const;

  /** \brief \ru Вернуть сегмент граничного двумерного контура.
             \en Return a segment of the bounding two-dimensional contour. \~
    \details \ru Функция создает сегмент граничного контура  в соответствии с параметром i. \n
      i = 0 - Отрезок границы при v = vmin \n
      i = 1 - Отрезок границы при u = umax \n
      i = 2 - Отрезок границы при v = vmax \n
      i = 3 - Отрезок границы при u = umin \n
      После использования объект надо удалить.
             \en The function creates a segment of bounding contour according to the parameter i. \n
      i = 0 - A segment of boundary where v = vmin \n
      i = 1 - A segment of boundary where u = umax \n
      i = 2 - A segment of boundary where v = vmax \n
      i = 3 - A segment of boundary where u = umin \n
      The object should be deleted after using. \~
    \param[in] i - \ru Номер сегмента.
                   \en An index of segment. \~
    \param[in] sense - \ru Совпадает ли направление контура с направлением обхода против часовой стрелки.
                       \en Whether the contour direction coincides with the counterclockwise traverse direction. \~
    \result \ru Граничная кривая.
            \en A boundary curve. \~
  */
  virtual MbCurve & MakeSegment( size_t i, bool sense ) const; 

  /** \brief \ru Вернуть сегмент граничного двумерного контура.
             \en Return a segment of the bounding two-dimensional contour. \~
    \details \ru Функция находит крайние точки поверхности с помощью функции GetLimitPoint 
      и строит отрезок от точки с номером number1 до точки с номером number2.
      После использования объект надо удалить.
             \en The function finds the boundary points of a surface using the function GetLimitPoint 
      and constructs a segment from the point with the number 'number1' to the points with the number 'number2'.
      The object should be deleted after using. \~
    \param[in] number1 - \ru Номер граничной точки поверхности.
                         \en A number of a boundary surface. \~
    \param[in] number2 - \ru Номер граничной точки поверхности.
                         \en A number of a boundary surface. \~
    \result \ru Граничная кривая.
            \en A boundary curve. \~
  */
  MbCurve & MakeCurve( size_t number1, size_t number2 ) const; 

  /// \ru Установить пределы поверхности. Для внутреннего использования. \en Set surface limits. For internal use only. 
  virtual bool    SetLimit( double u1, double v1, double u2, double v2 );
  /// \ru Установить пределы поверхности. Для внутреннего использования. \en Set surface limits. For internal use only. 
  bool    SetLimit( const MbRect & );

  /// \ru Установить расширенные пределы поверхности. Для внутреннего использования. \en Set extended limits of surface. For internal use only. 
  virtual void    SetExtendedParamRegion( double u1, double v1, double u2, double v2 );
  /// \ru Включить точку в область определения. Для внутреннего использования. \en Include a point into domain. For internal use only. 
  virtual void    IncludePoint( double u, double v );

  /// \ru Дать максимальное приращение параметра U. \en Get the maximum increment of U-parameter. 
  double  GetMaxParamDeltaU() const { return GetURange() / c3d::COUNT_DELTA; }
  /// \ru Дать максимальное приращение параметра V. \en Get the maximum increment of V-parameter. 
  double  GetMaxParamDeltaV() const { return GetVRange() / c3d::COUNT_DELTA; }
  /// \ru Дать максимальное приращение параметра. \en Get the maximum increment of parameter. 
  virtual double  GetParamDelta() const;
  /// \ru Дать минимально различимую величину параметра. \en Get the minimum distinguishable value of parameter. 
  virtual double  GetParamPrice() const;
  
  /// \ru Дать приращение параметра u, осреднённо соответствующее единичной длине в пространстве. \en Get increment of u-parameter, corresponding to the unit length in space. 
  virtual double  GetUParamToUnit() const;
  /// \ru Дать приращение параметра v, осреднённо соответствующее единичной длине в пространстве. \en Get increment of v-parameter, corresponding to the unit length in space. 
  virtual double  GetVParamToUnit() const;
  /// \ru Дать приращение параметра u, соответствующее единичной длине в пространстве. \en Get increment of u-parameter, corresponding to the unit length in space. 
  virtual double  GetUParamToUnit( double u, double v ) const;
  /// \ru Дать приращение параметра v, соответствующее единичной длине в пространстве. \en Get increment of v-parameter, corresponding to the unit length in space. 
  virtual double  GetVParamToUnit( double u, double v ) const;
  /// \ru Дать приращение параметра u, соответствующее единичной длине в пространстве. \en Get increment of u-parameter, corresponding to the unit length in space. 
  double  GetUParamToUnit( const MbCartPoint & uv ) const { return GetUParamToUnit( uv.x, uv.y ); }
  /// \ru Дать приращение параметра v, соответствующее единичной длине в пространстве. \en Get increment of v-parameter, corresponding to the unit length in space. 
  double  GetVParamToUnit( const MbCartPoint & uv ) const { return GetVParamToUnit( uv.x, uv.y ); }
  /// \ru Дать приращение параметра u и параметра v, соответствующее единичной длине в пространстве. \en Get increment of parameters, corresponding to the unit length in space. 
  virtual void    GetParamsToUnit( double u, double v, double & uParam, double & vParam ) const; 
          
  /// \ru Дать минимально различимую величину параметра U. Соответствует длине в пространстве = Math::metricEpsilon. \en Get the minimum distinguishable value of u-parameter. It corresponds to the length Math::metricEpsilon in space. 
  double  GetUEpsilon() const;
  /// \ru Дать минимально различимую величину параметра V. Соответствует длине в пространстве = Math::metricEpsilon. \en Get the minimum distinguishable value of v-parameter. It corresponds to the length Math::metricEpsilon in space. 
  double  GetVEpsilon() const;
  /// \ru Дать минимально различимую величину параметра U. Соответствует длине в пространстве = Math::metricEpsilon. \en Get the minimum distinguishable value of u-parameter. It corresponds to the length Math::metricEpsilon in space. 
  double  GetUEpsilon( double u, double v ) const;
  /// \ru Дать минимально различимую величину параметра V. Соответствует длине в пространстве = Math::metricEpsilon. \en Get the minimum distinguishable value of v-parameter. It corresponds to the length Math::metricEpsilon in space. 
  double  GetVEpsilon( double u, double v ) const;
          
  /// \ru Дать минимально различимую величину параметра U. Соответствует длине в пространстве = Math::metricRegion. \en Get the minimum distinguishable value of u-parameter. It corresponds to the length Math::metricRegion in space. 
  double  GetURegion() const;
  /// \ru Дать минимально различимую величину параметра V. Соответствует длине в пространстве = Math::metricRegion. \en Get the minimum distinguishable value of v-parameter. It corresponds to the length Math::metricRegion in space. 
  double  GetVRegion() const;
  /// \ru Дать минимально различимую величину параметра U. Соответствует длине в пространстве = Math::metricRegion. \en Get the minimum distinguishable value of u-parameter. It corresponds to the length Math::metricRegion in space. 
  double  GetURegion( double u, double v ) const;
  /// \ru Дать минимально различимую величину параметра V. Соответствует длине в пространстве = Math::metricRegion. \en Get the minimum distinguishable value of v-parameter. It corresponds to the length Math::metricRegion in space. 
  double  GetVRegion( double u, double v ) const;

  /// \ru Выдать количество разбиений по u. \en The the number of splittings in u-direction. 
  virtual size_t  GetUMeshCount() const;
  /// \ru Выдать количество разбиений по v. \en The the number of splittings in v-direction. 
  virtual size_t  GetVMeshCount() const;

  /** \brief \ru Рассчитать полигон по параметру U или V.
             \en Calculate polygon by u or v. \~
    \details \ru В функции строится поверхностная кривая - отрезок вдоль выбранного направления
      и считается полигон, аппроксимирующий эту кривую.
             \en In the function the surface curve is constructed - a segment along the chosen direction
      and after that a curve approximating polygon is calculated. \~
    \param[in] minPar - \ru Минимальное значение параметра по выбранному направлению.
                        \en Minimal value of parameter in the chosen direction. \~
    \param[in] maxPar - \ru Максимальное значение параметра по выбранному направлению.
                        \en Maximal value of parameter in the chosen direction. \~
    \param[in] constPar - \ru Значение второго параметра.
                          \en A value of second parameter. \~
    \param[in] dir - \ru Выбранное направление. \n
      dir == pd_DirU - расчет производится по параметру u, v = const. \n
      dir == pd_DirV - расчет производится по параметру v, u = const.
                     \en A chosen direction. \n
      dir == pd_DirU - calculation is performed by u-parameter, v = const. \n
      dir == pd_DirU - calculation is performed by v-parameter, u = const. \~
    \param[in] sag - \ru Величина прогиба, определяющая точность аппроксимации.
                     \en A sag value, defining the tolerance of approximation. \~
    \param[out] polygon - \ru Насчитанный полигон.
                          \en Calculated polygon. \~
  */
  void    CalculatePolygon( double minPar, double maxPar, double constPar, MbeParamDir dir,
                            const MbStepData & stepData, MbPolygon3D & polygon ) const; 

  /** \brief \ru Рассчитать сетку.
             \en Calculate mesh. \~
    \details \ru В функции производится расчет сетки для отрисовки поверхности.
             \en In the function the mesh calculation for surface drawing is performed. \~
    \param[in] sag - \ru Величина прогиба, определяющая точность.
                     \en A sag value, defining the tolerance.. \~
    \param[in] beg - \ru Номер первого узла, который попадет в сетку, по каждому из направлений. 
      Для построения сетки для всей поверхности beg == 0.
                     \en A number of the the first knot which gets into the mesh by each direction. 
      For the construction of mesh of the whole surface beg == 0. \~
    \param[out] mesh - \ru Насчитанный Фасетный объект.
                       \en A calculated mesh. \~
    \param[in] uMeshCount - \ru Количество u-линий отрисовочной сетки. 
                            \en The number of u-mesh lines. \~
    \param[in] vMeshCount - \ru Количество v-линий отрисовочной сетки.
                            \en The number of v-mesh lines. \~ 
  */
  virtual void    CalculateSurfaceWire( const MbStepData & stepData, size_t beg, MbMesh & mesh, 
                                        size_t uMeshCount = c3d::WIRE_MAX, size_t vMeshCount = c3d::WIRE_MAX ) const;

  /** \brief \ru Определить разбивку параметрической области поверхности вертикалями и горизонталями.
             \en Determine a splitting of parametric region of a surface by verticals and horizontals. \~
    \details \ru Определить разбивку параметрической области поверхности вертикалями и горизонталями при триангуляции.\n
             \en Determine a splitting of parametric region of a surface by verticals and horizontals during triangulation.\n \~
    \param[in] stepData - \ru Данные для вычисления шага при триангуляции.
                          \en Data for step calculation during triangulation. \~
    \param[in] u1 - \ru Минимальное значение параметра u в области разбиения поверхности.
                    \en Minimal value of u-parameter in the region of surface splitting. \~
    \param[in] u2 - \ru Максимальное значение параметра u в области разбиения поверхности.
                    \en Maximal value of u-parameter in the region of surface splitting. \~
    \param[in] v1 - \ru Минимальное значение параметра v в области разбиения поверхности.
                    \en Minimal value of v-parameter in the region of surface splitting. \~
    \param[in] v2 - \ru Максимальное значение параметра v в области разбиения поверхности.
                    \en Maximal value of v-parameter in the region of surface splitting. \~
    \param[out] uu - \ru Множество параметров разбиения по u.
                     \en A set of parameters of splitting by u. \~
    \param[out] vv - \ru Множество параметров разбиения по v.
                     \en A set of parameters of splitting by v. \~
  */
  virtual void    GetTesselation( const MbStepData & stepData,
                                  double u1, double u2, double v1, double v2,
                                  SArray<double> & uu, SArray<double> & vv ) const;

  /** \brief \ru Определить разбивку параметрической области поверхности с учетом направления.
             \en Determine splitting of the surface parametric region according to direction. \~
    \details \ru Определить разбивку параметрической области поверхности вертикалями и горизонталями.
      Добавить параметры в массив в направлении dir. Если массивы uu или vv не пустые, то элементы из них не удаляются,
      то есть происходит уточнение сетки в выбранном направлении.
             \en Determine a splitting of parametric region of a surface by verticals and horizontals.
      Add parameters into an array in the direction dir. If the arrays uu and vv are not empty then their elements are deleted.
      i.e. a clarification of mesh in a given direction is performed. \~
    \param[in] stepData - \ru Данные для вычисления шага при триангуляции.
                          \en Data for step calculation during triangulation. \~
    \param[in] dir - \ru Выбранное направление. \n
      dir == pd_DirU - расчет производится по параметру u, v = const. \n
      dir == pd_DirV - расчет производится по параметру v, u = const.
                     \en A chosen direction. \n
      dir == pd_DirU - calculation is performed by u-parameter, v = const. \n
      dir == pd_DirU - calculation is performed by v-parameter, u = const. \~
    \param[in] u1 - \ru Минимальное значение параметра u в области разбиения поверхности.
                    \en Minimal value of u-parameter in the region of surface splitting. \~
    \param[in] u2 - \ru Максимальное значение параметра u в области разбиения поверхности.
                    \en Maximal value of u-parameter in the region of surface splitting. \~
    \param[in] v1 - \ru Минимальное значение параметра v в области разбиения поверхности.
                    \en Minimal value of v-parameter in the region of surface splitting. \~
    \param[in] v2 - \ru Максимальное значение параметра v в области разбиения поверхности.
                    \en Maximal value of v-parameter in the region of surface splitting. \~
    \param[out] uu - \ru Множество параметров разбиения по u.
                     \en A set of parameters of splitting by u. \~
    \param[out] vv - \ru Множество параметров разбиения по v.
                     \en A set of parameters of splitting by v. \~
  */
  void    AddTesselation( const MbStepData & stepData, MbeParamDir dir, 
                          double u1, double u2, double v1, double v2,
                          SArray<double> & uu, SArray<double> & vv ) const;

  /** \brief \ru Аппроксимировать поверхность треугольными пластинами.
             \en Approximate a surface by triangular plates. \~
    \details \ru Аппроксимировать поверхность треугольными пластинами.\n
             \en Approximate a surface by triangular plates.\n \~
    \param[in] stepData - \ru Данные для вычисления шага при триангуляции.
                          \en Data for step calculation during triangulation. \~
    \param[in] sense - \ru Определяет совпадение нормалей поверхности и треугольников.
                       \en It determines the coincidence of normals of surfaces and triangles. \~ 
    \param[out] grid - \ru Результат разбиения.
                       \en The result of splitting. \~
  */
  virtual void    CalculateSurfaceGrid( const MbStepData & stepData, bool sense, MbGrid & grid ) const;

  /// \ru Определить, ортогональны ли производные по u и v. \en Determine whether derivatives with respect to u and v are orthogonal. 
  virtual bool    IsRectangular() const;
  /// \ru Проверить, что все производные поверхности по U выше первой равны нулю. \en Check that all derivatives of surface with respect to u which have more than first order are equal to null. 
  virtual bool    IsLineU() const;
  /// \ru Проверить, что все производные поверхности по V выше первой равны нулю. \en Check that all derivatives of surface with respect to v which have more than first order are equal to null. 
  virtual bool    IsLineV() const;
  
  /** \brief \ru Определить, является ли объект смещением.
             \en Determine whether the object is a translation. \~
    \details \ru Определить, является ли объект смещением заданного объекта.
             \en Determine whether the object is a translation of a given object. \~ 
    \param[in] obj - \ru Объект - образец.
                     \en A pattern object. \~
    \param[out] dir - \ru Вектор смещения, если объект является смещением.
                      \en A translation vector if the object is translation. \~
    \param[out] isSame - \ru На выходе true, если текущая поверхность и объект-образец идентичны.
                         \en True at the output if the current surface and the pattern object are identical. \~
    \result \ru true - если поверхность является смещением.
            \en True - if a surface is translation \~
  */
  virtual bool    IsShift( const MbSpaceItem & obj, MbVector3D & dir, bool & isSame, double accuracy = LENGTH_EPSILON ) const;

  /** \brief \ru Проверить параметры. Аналог глобальной функции _CheckParams, оптимизированный под использование кэшей.
             \en Check parameters. Analogue of the global function _CheckParams, optimized for caches usage.  \~
    \details \ru Проверить параметры и загнать в область определения, если параметр вышел за полюс.
             \en Check parameters and move them inside domain if parameter is out of pole. \~
    \param[in] surface - \ru Поверхность. \en Surface. \~
    \param[in] u -       \ru Первый параметр. \en First parameter. \~
    \param[in] v -       \ru Второй параметр. \en Second parameter. \~
  */
  virtual void    CheckSurfParams( double & u, double & v ) const;

  /// \ru Дать локальную систему координат плоской поверхности (или только возможность ее выдачи). \en Get a local coordinate system of planar surface (or only a possibility of getting it).
  bool    GetPlacement     ( MbPlacement3D * place, bool exact = false ) const;
  /// \ru Дать локальную систему координат, если поверхность является плоскостью. \en Get a local coordinate system if a surface is a plane. 
  bool    GetPlanePlacement( MbPlacement3D & place ) const;
  /** \brief \ru Построить локальную систему координат с началом в средней точке параметрических пределов базовой поверхности.
             \en Construct a local coordinate system of a surface with origin at the middle point of base surface parametric limits. \~
    \details \ru Построить локальную систему координат с началом в средней точке параметрических пределов поверхности.
             \en Construct a local coordinate system of a surface with origin at the middle point of surface parametric limits. \~
    \param[out] place - \ru Локальная система координат.
                        \en Local coordinate system. \~
    \param[in] sameSense - \ru Относительная ориентация нормали.
                           \en Normal's relative orientation. \~
    \result \ru true - Если получилось построить.
            \en True - if construction has been successfully. \~
  */
  bool    GetControlPlacement( MbPlacement3D & place, bool sameSense = true ) const;
  /// \ru Сориентировать ось Х плейсмента вдоль линии его пересечения с поверхностью. \en Orient an axis X of a placement along the line of its intersection with surface. 
  bool    OrientPlacement  ( MbPlacement3D & place, bool normalSense = true ) const;
  /// \ru Определить, лежит ли точка на поверхности. \en Determine whether a point is located on a surface or not. 
  bool    IsPointOn ( const MbCartPoint3D &, double eps = METRIC_PRECISION ) const;
  /// \ru Вычислить точку на поверхности в области определения поверхности. \en Calculate the point on a surface inside the domain of surface. 
  void    PointOn   ( MbCartPoint & uv, MbCartPoint3D & p ) const;
  /// \ru Вычислить точку на продолженной поверхности. \en Calculate a point on a surface extension. 
          void   _PointOn   ( const MbCartPoint & uv, MbCartPoint3D & p ) const;
  /// \ru Вычислить нормаль к поверхности в области определения поверхности. \en Calculate the normal vector to a surface inside the domain of surface. 
  void    Normal    ( MbCartPoint & uv, MbVector3D    & v ) const;
        
  /** \brief \ru Найти матрицу преобразования для кривых на поверхности при изменении параметризации.
             \en Find a matrix of transformation for the curves on a surface when the parameterization is changed. \~
    \details \ru Для поверхности устанавливается новый диапазон параметров.
      Геометрически поверхность не изменяется. 
      Функция ищет матрицу преобразования для двумерных кривых на поверхности для перехода от старой параметризации к новой.
             \en The new range of parameters is set for a surface.
      A surface doesn't change geometrically. 
      The function searches a matrix of transformation for the two-dimensional curves on a surface for the transition from old parameterization to new parameterization. \~
    \param[in] xMin - \ru Новое минимальное значение параметра u.
                      \en The new minimum value of u. \~
    \param[in] xMax - \ru Новое максимальное значение параметра u.
                      \en The new maximum value of u. \~
    \param[in] yMin - \ru Новое минимальное значение параметра v.
                      \en The new minimum value of v. \~
    \param[in] yMax - \ru Новое максимальное значение параметра v.
                      \en The new maximum value of v. \~
    \param[in] matr - \ru Матрица преобразования.
                      \en A transformation matrix. \~
  */
  bool    GetMatrix( double xMin, double xMax, double yMin, double yMax, MbMatrix & matr ) const;

  /// \ru Среднее значение параметра u. \en The middle value of u. 
  double  GetUMid() const { return ((GetUMin() + GetUMax()) * 0.5); }
  /// \ru Среднее значение параметра v. \en The middle value of v. 
  double  GetVMid() const { return ((GetVMin() + GetVMax()) * 0.5); }
  /// \ru Параметрическая длина по u. \en Parametric length by u. 
  double  GetURange() const { return (GetUMax() - GetUMin()); }
  /// \ru Параметрическая длина по v. \en Parametric length by v. 
  double  GetVRange() const { return (GetVMax() - GetVMin()); }
  /// \ru Получить параметрические границы поверхности. \en Get parametric bounding box. 
  void    GetRect( MbRect & r ) const { r.Set( GetUMin(), GetVMin(), GetUMax(), GetVMax() ); }
  /// \ru Получить параметрические границы поверхности. \en Get parametric bounding box. 
  void    GetRect( MbRect2D & r ) const { r.Init( GetUMin(), GetVMin(), GetUMax(), GetVMax() ); }

  /** \} */

          // \ru Функции унификации объекта и вектора объектов в шаблонных функциях. \en Functions for compatibility of a object and a vector of objects in template functions.
  size_t  size() const { return 1; } ///< \ru Количество объектов при трактовке объекта как вектора объектов. \en Number of objects if object is interpreted as vector of objects. 
          const MbSurface * operator [] ( size_t ) const { return this; }  ///< \ru Оператор доступа. \en An access operator.

protected:
  /// \ru Сдвинуть габарит. \en Move bounding box. 
  void    MoveGabarit( const MbVector3D & v ) { if ( !cube.IsEmpty() ) cube.Move( v ) ; }
  /// \ru Вычислить нормаль по известным производным uDer и vDer в точке с параметрами u, v. \en Normal calculation by derivatives uDer and vDer on point with parameters u, v.
  void    NormalCalculation( const MbVector3D & uDer, const MbVector3D & vDer, double u, double v, bool ext, MbVector3D & nor ) const;
  /// \ru Вычислить шаг по параметру для заданного прогиба. \en Step calculation by sag.
  double  StepAlong( double u, double v, double sag, bool alongU, double stepMinCoeff,
                             const MbVector3D & der, const MbVector3D & sec ) const;
  /// \ru Вычислить шаг по параметру для заданного углового отклонения нормали. \en Step calculation by normal deviation.
  double  DeviationStepAlong( double u, double v, double angle, bool alongU,
                                      const MbVector3D & der, const MbVector3D & sec ) const;
         
private:
  /// \ru Найти аппроксимационную поверхность с помощью метода наименьших квадратов. \en Find an approximation surface using the method of least squares. 
          MbSplineSurface * NurbsSurfaceLSM( MbNurbsParameters & paramU, MbNurbsParameters & paramV, size_t uCount, size_t vCount ) const; 
  /// \ru Найти аппроксимационную поверхность с помощью метода наименьших квадратов. \en Find an approximation surface using the method of least squares. 
          MbSplineSurface * NurbsSurfaceThroughPoints( MbNurbsParameters & paramU, MbNurbsParameters & paramV, size_t uCount, size_t vCount ) const; 
  /// \ru Подготовить параметры для преобразования в NURBS поверхность при заданном узловом векторе для интерполяции. \en Prepare parameters for the transformation to NURBS surface with the given knot vector for the interpolation. 
  bool    NurbsParamForKnots( const MbNurbsParameters & tParam, const SArray<double> & knots, bool uParam, double op1, double op2,
                                      bool & isClosedNurbs, double & epsilon, SArray<double> & params ) const; 

          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию \en Declaration without implementation of the assignment operator to prevent an assignment by default 
  void    operator = ( const MbSurface & ); 

  DECLARE_PERSISTENT_CLASS( MbSurface )
};

IMPL_PERSISTENT_OPS( MbSurface )


//------------------------------------------------------------------------------
// \ru Вычислить точку на поверхности. \en Calculate a point on a surface. 
// ---
inline void MbSurface::PointOn( MbCartPoint & uv, MbCartPoint3D & p ) const {
  PointOn( uv.x, uv.y, p );
}


//------------------------------------------------------------------------------
// \ru Вычислить значения точки и производных для параметров в пределах области определения.
// \en Calculate point and derivatives of object for parameters inside the surface's domain of parameters. \~
// ---
inline void MbSurface::Explore( double & u, double & v,
                                MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer ) const {
  PointOn( u, v, pnt );
  DeriveU( u, v, uDer );
  DeriveV( u, v, vDer );
}


//------------------------------------------------------------------------------
// \ru Вычислить точку на продолженной поверхности. \en Calculate a point on a surface extension. 
// ---
inline void MbSurface::_PointOn( const MbCartPoint & uv, MbCartPoint3D & p ) const {
  _PointOn( uv.x, uv.y, p );
}


//------------------------------------------------------------------------------
// \ru Вычислить нормаль к поверхности в области определения поверхности. \en Calculate the normal vector to a surface inside the domain of surface. 
// ---
inline void MbSurface::Normal( MbCartPoint & uv, MbVector3D & v ) const {
  Normal( uv.x, uv.y, v );
}


//------------------------------------------------------------------------------
/** \brief \ru Проверить параметры. Аналог MbSurface::CheckSurfParams.
           \en Check parameters. Analogue of MbSurface::CheckSurfParams. \~
  \details \ru Проверить параметры и загнать в область определения, если параметр вышел за полюс.
           \en Check parameters and move them inside domain if parameter is out of pole. \~
  \param[in] surface - \ru Поверхность.
                       \en Surface. \~
  \param[in] u - \ru Первый параметр.
                 \en First parameter. \~
  \param[in] v - \ru Второй параметр.
                 \en Second parameter. \~
*/
// ---
inline bool _CheckParams( const MbSurface & surface, double & u, double & v, double del = 0.0 )
{
  bool result = false;
  if ( surface.GetPoleUMin() ) {
    double umin = surface.GetUMin();
    if ( u - del < umin ) {
      u = umin;
      result = true;
    }
  }
  if ( surface.GetPoleUMax() ) {
    double umax = surface.GetUMax();
    if ( u + del > umax ) {
      u = umax;
      result = true;
    }
  }
  if ( surface.GetPoleVMin() ) {
    double vmin = surface.GetVMin();
    if ( v - del < vmin ) {
      v = vmin;
      result = true;
    }
  }
  if ( surface.GetPoleVMax() ) {
    double vmax = surface.GetVMax();
    if ( v + del > vmax ) {
      v = vmax;
      result = true;
    }
  }
  return result;
}


//------------------------------------------------------------------------------
/** \brief \ru Вычислить параметры ближайших точек поверхности и кривой.
           \en Calculate parameters of the nearest points of surface and curve. \~
  \details \ru Вычислить параметры ближайших точек поверхности и кривой и расстояние между этими точками. \n
           \en Calculate parameters of the nearest points of surface and curve and the distance between these points. \n \~
  \param[in] surface - \ru Поверхность.
                       \en Surface. \~
  \param[in] ext0 - \ru Признак поиска на продолжении поверхности.
                    \en An attribute of search at the extension of surface. \~
  \param[in] curve - \ru Кривая.
                     \en A curve. \~
  \param[in] ext1 - \ru Признак поиска на продолжении кривой.
                    \en An attribute of search at the extension of curve. \~
  \param[in,out] u0 - \ru Параметр u точки поверхности.
                      \en U-parameter of surface point. \~
  \param[in,out] v0 - \ru Параметр v точки поверхности.
                      \en V-parameter of surface point. \~
  \param[in,out] t0 - \ru Параметр точки кривой.
                      \en A curve point parameter. \~
  \param[out] dmin - \ru Расстояние между точками.
                     \en The distance between points. \~
  \param[in] tCalc - \ru Флаг инициялизации t0 \n если true, то входные параметры u0, v0, t0 считаются начальными приближениями.
                     \en The initialization flag t0 \n if true, then the output parameters u0, v0 and t0 are considered as initial approximations. \~
  \return \ru Возвращает nr_Success (+1) или nr_Special(0) в случае успешного определения, в случае неудачи возвращает nr_Failure(-1).
          \en Return nr_Success (+1) or nr_Special(0) in a case of successful defining, return nr_Failure(-1) in a case of failure. \~
  \ingroup Surfaces
*/
// ---
MATH_FUNC (MbeNewtonResult) NearestPoints( const MbSurface & surface, bool ext0, 
                                           const MbCurve3D & curve, bool ext1, 
                                           double & u0, double & v0, double & t0, double & dmin, 
                                           bool t0Calculated = false );  


//------------------------------------------------------------------------------
/** \brief \ru Вычислить параметры ближайших точек поверхностей.
           \en Calculate parameters of the nearest points of surfaces. \~
  \details \ru Вычислить параметры ближайших точек поверхностей и расстояние между этими точками.
           \en Calculate parameters of the nearest points of surfaces and the distance between these points. \~
  \param[in] surface0 - \ru Поверхность.
                        \en Surface. \~
  \param[in] ext0 - \ru Признак поиска на продолжении поверхности surface0.
                    \en An attribute of search at the extension of a surface 'surface0'. \~
  \param[in] surface1 - \ru Поверхность.
                        \en Surface. \~
  \param[in] ext1 - \ru Признак поиска на продолжении поверхности surface1.
                    \en An attribute of search at the extension of a surface 'surface1'. \~
  \param[out] u0 - \ru Параметр u точки поверхности surface0.
                   \en U-parameter of a point on the surface 'surface0'. \~
  \param[out] v0 - \ru Параметр v точки поверхности surface0.
                   \en V-parameter of a point on the surface 'surface0'. \~
  \param[out] u1 - \ru Параметр u точки поверхности surface1.
                   \en U-parameter of a point on the surface 'surface1'. \~
  \param[out] v1 - \ru Параметр v точки поверхности surface1.
                   \en V-parameter of a point on the surface 'surface1'. \~
  \param[out] dmin - \ru Расстояние между точками поверхностей.
                     \en The distance between points on surfaces. \~
  \param[in] checkCurvilinearBounds - \ru Всегда проверять по криволинейным границам (если они есть).
                                      \en Check for curvilinear boundaries (if they are). \~
  \return \ru Возвращает nr_Success (+1) или nr_Special(0) в случае успешного определения, в случае неудачи возвращает nr_Failure(-1).
          \en Return nr_Success (+1) or nr_Special(0) in a case of successful defining, return nr_Failure(-1) in a case of failure. \~
  \ingroup Surfaces
*/
// ---
MATH_FUNC (MbeNewtonResult) NearestPoints( const MbSurface & surface0, bool ext0, 
                                           const MbSurface & surface1, bool ext1, 
                                           double & u0, double & v0, double & u1, double & v1, double & dmin,
                                           bool checkCurvilinearBounds );


#endif // __SURFACE_H
