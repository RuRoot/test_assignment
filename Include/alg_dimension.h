////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Радиальный размер к поверхности. Расстояние между поверхностями.
         \en Radial dimension of surface. Distance between surfaces. \~
  \details \ru Функции построения окружности или дуги для радиального размера к поверхности.
    Функция вычисления экстремальных расстояний между поверхностями.
           \en Functions of construction of a circle or an arc for radial dimension of surface.
    A function of calculation of extreme distances between surfaces. \~
*/
////////////////////////////////////////////////////////////////////////////////


#ifndef __ALG_DIMENSION_H
#define __ALG_DIMENSION_H


#include <templ_s_array.h>
#include <mb_enum.h>
#include <mb_cart_point.h>
#include <curve3d.h>
#include <surface.h>


class    MbCartPoint3D;
class    MbVector3D;
class    MbPlacement3D;
class    MbAxis3D;
class    MbPlaneCurve;
class    IProgressIndicator;


////////////////////////////////////////////////////////////////////////////////
// 
// \ru Построение окружности или дуги для радиального размера к поверхности, \en Construction of a circle or an arc for radial dimension of surface 
// \ru имеющей круговую параметрическую линию u=const или v=const \en which has a circular parametric line u=const or v=const. 
// \ru Перечень поверхностей, имеющих параметрическую линию u=const или u=const: \en The enumeration of surfaces which have a parametric line u=const or u=const: 
// MbCylinderSurface,      v=const
// MbConeSurface,          v=const
// MbSphereSurface,        u=const
// MbTorusSurface,         u=const
// MbLoftedSurface,        v=const
// MbElevationSurface,     v=const
// MbExtrusionSurface,     v=const
// MbRevolutionSurface,    u=const
// MbEvolutionSurface,     u=const
// MbExactionSurface,      u=const
// 
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
/** \brief \ru Построить окружность или дугу для радиального размера к поверхности.
           \en Construct a circle or an arc for radial dimension of surface. \~
  \details \ru Построение выполняется по заданной параметрической точке поверхности. Поверхность 
    должна иметь круговую параметрическую линию u=const или v=const. Перечень поверхностей,
    имеющих параметрическую линию u=const или v=const:
    MbCylinderSurface     v=const, \n   MbConeSurface         v=const, \n
    MbSphereSurface       u=const, \n   MbTorusSurface        u=const, \n
    MbLoftedSurface       v=const, \n   MbElevationSurface    v=const, \n
    MbExtrusionSurface    v=const, \n   MbRevolutionSurface   u=const, \n
    MbEvolutionSurface    u=const, \n   MbExactionSurface     u=const.
           \en Construction is performed by the given parametric point on surface. A surface 
    should have a circular parametric line u=const or v=const. The enumeration of surfaces
    which have a parametric line u=const or v=const.
    MbCylinderSurface     v=const, \n   MbConeSurface         v=const, \n
    MbSphereSurface       u=const, \n   MbTorusSurface        u=const, \n
    MbLoftedSurface       v=const, \n   MbElevationSurface    v=const, \n
    MbExtrusionSurface    v=const, \n   MbRevolutionSurface   u=const, \n
    MbEvolutionSurface    u=const, \n   MbExactionSurface     u=const. \~
  \param[in] surface - \ru Исходная поверхность.
                       \en The initial surface. \~
  \param[in] surface_uv - \ru Координаты исходной точки на поверхности.
                          \en Coordinates of the initial point on surface. \~
  \param[out] plane_curve - \ru Требуемая окружность или дуга.
                            \en The required circle or an arc. \~ 
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC (void) RadiusDimension3D( const MbSurface     & surface, 
                                    const MbCartPoint   & surface_uv,
                                          MbPlaneCurve *& plane_curve ); 


//------------------------------------------------------------------------------
/** \brief \ru Построить окружность или дугу для радиального размера к поверхности.
           \en Construct a circle or an arc for radial dimension of surface. \~
  \details \ru Построение выполняется по заданной по заданной пространственной точке. Поверхность 
    должна иметь круговую параметрическую линию u=const или v=const. Перечень поверхностей,
    имеющих параметрическую линию u=const или u=const:
    MbCylinderSurface     v=const, \n   MbConeSurface         v=const, \n
    MbSphereSurface       u=const, \n   MbTorusSurface        u=const, \n
    MbLoftedSurface       v=const, \n   MbElevationSurface    v=const, \n
    MbExtrusionSurface    v=const, \n   MbRevolutionSurface   u=const, \n
    MbEvolutionSurface    u=const, \n   MbExactionSurface     u=const.
           \en Construction is performed by the given spatial point. A surface 
    should have a circular parametric line u=const or v=const. The enumeration of surfaces
    which have a parametric line u=const or v=const.
    MbCylinderSurface     v=const, \n   MbConeSurface         v=const, \n
    MbSphereSurface       u=const, \n   MbTorusSurface        u=const, \n
    MbLoftedSurface       v=const, \n   MbElevationSurface    v=const, \n
    MbExtrusionSurface    v=const, \n   MbRevolutionSurface   u=const, \n
    MbEvolutionSurface    u=const, \n   MbExactionSurface     u=const. \~
  \param[in] surface - \ru Исходная поверхность.
                       \en The initial surface. \~
  \param[in] point - \ru Пространственные координаты исходной точки.
                     \en Space coordinates of the initial point. \~
  \param[out] plane_curve - \ru Требуемая окружность или дуга.
                            \en The required circle or an arc. \~ 
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC (void) RadiusDimension3D( const MbSurface     & surface, 
                                    const MbCartPoint3D & point,
                                          MbPlaneCurve *& plane_curve ); 


//------------------------------------------------------------------------------
/** \brief \ru Построить окружность или дугу для радиального размера к поверхности.
           \en Construct a circle or an arc for radial dimension of surface. \~
  \details \ru Построение выполняется по заданному плейсменту. Поверхность должна иметь 
    круговую параметрическую линию u=const или v=const. Перечень поверхностей, имеющих 
    параметрическую линию u=const или u=const:
    MbCylinderSurface     v=const, \n   MbConeSurface         v=const, \n
    MbSphereSurface       u=const, \n   MbTorusSurface        u=const, \n
    MbLoftedSurface       v=const, \n   MbElevationSurface    v=const, \n
    MbExtrusionSurface    v=const, \n   MbRevolutionSurface   u=const, \n
    MbEvolutionSurface    u=const, \n   MbExactionSurface     u=const.
           \en Construction is performed by the given placement. A surface should have 
    a circular parametric line u=const or v=const. The enumeration of surfaces with 
    a parametric line u=const or v=const.
    MbCylinderSurface     v=const, \n   MbConeSurface         v=const, \n
    MbSphereSurface       u=const, \n   MbTorusSurface        u=const, \n
    MbLoftedSurface       v=const, \n   MbElevationSurface    v=const, \n
    MbExtrusionSurface    v=const, \n   MbRevolutionSurface   u=const, \n
    MbEvolutionSurface    u=const, \n   MbExactionSurface     u=const. \~
  \param[in] surface - \ru Исходная поверхность.
                       \en The initial surface. \~
  \param[in] place - \ru Исходный плейсмент.
                     \en The initial placement. \~
  \param[out] plane_curve - \ru Требуемая окружность или дуга.
                            \en The required circle or an arc. \~ 
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC (void) RadiusDimension3D( const MbSurface     & surface, 
                                    const MbPlacement3D & place,
                                          MbPlaneCurve *& plane_curve ); 


//------------------------------------------------------------------------------
/** \brief \ru Можно ли построить окружность или дугу для радиального размера к поверхности.
           \en Whether a circle or an arc can be constructed for radial dimension of surface. \~
  \details \ru Можно построить, если тип базовой поверхности: st_CylinderSurface или st_ConeSurface,
    или st_SphereSurface, или st_TorusSurface.
           \en It can be constructed if the type of a base surface is st_CylinderSurface or st_ConeSurface,
    or st_SphereSurface, or st_TorusSurface. \~
  \param[in] surface - \ru Исходная поверхность.
                       \en The initial surface. \~
  \return \ru true, если можно построить.
          \en true if it can be constructed. \~
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC (bool) IsPossibleRadiusDimension3D( const MbSurface & surface ); 


//------------------------------------------------------------------------------
/** \brief \ru Результат замера расстояния и угла между поверхностями.
           \en The result of measurement of dimension and angle between surfaces. \~
  \details \ru Результат замера расстояния и угла между поверхностями.
           \en The result of measurement of dimension and angle between surfaces. \~
  \ingroup Algorithms_3D
*/ // ---
enum MbeSurfAxesMeasureRes
{
  // \ru ошибочные результат \en mistaken result 
  samr_SurfSurf_Failed = -3,  ///< \ru Ошибка при работе с поверхностями. \en An error is occurred while working with surfaces. 
  samr_AxisSurf_Failed = -2,  ///< \ru Ошибка при работе с осью и поверхностью. \en An error is occurred while working with axis and surface. 
  samr_AxisAxis_Failed = -1,  ///< \ru Ошибка при работе с осями. \en An error is occurred while working with axes. 
  // \ru пустой результат \en an empty result. 
  samr_Undefined = 0,         ///< \ru Не получилось или не измерялось. \en Failed or didn't measured. 
  // \ru две оси \en two axes 
  samr_AxisAxis_Coaxial,      ///< \ru Оси совпадают. \en Axes are coincident. 
  samr_AxisAxis_Parallel,     ///< \ru Оси параллельны. \en Axes are parallel. 
  samr_AxisAxis_Intersecting, ///< \ru Оси пересекаются. \en Axes are crossed. 
  samr_AxisAxis_Distant,      ///< \ru Оси на расстоянии. \en Axes are located at a distance. 
  // \ru одна ось (какая из осей есть, см. по возвращаемому флагу функции замера) \en one axis (see the returned flag of measurement function to detect which one exactly) 
  samr_AxisSurf_Colinear,     ///< \ru Ось лежит на поверхности. \en The axis lies on the surface. 
  samr_AxisSurf_Parallel,     ///< \ru Ось параллельна поверхности. \en The axis is parallel to the surface. 
  samr_AxisSurf_Intersecting, ///< \ru Ось пересекает поверхность. \en The axis crosses the surface. 
  samr_AxisSurf_Distant,      ///< \ru Ось на расстоянии от поверхности. \en The axis is located at a distance from the surface. 
  // \ru две плоские поверхности \en two planar surfaces 
  samr_SurfSurf_Colinear,     ///< \ru Одна поверхность лежит на другой. \en One surface lies on another one. 
  samr_SurfSurf_Parallel,     ///< \ru Поверхности параллельны. \en Surfaces are parallel. 
  samr_SurfSurf_Intersecting, ///< \ru Поверхности пересекаются. \en Surfaces are intersecting inside domain. 
  // \ru samr_SurfSurf_Distant,      // находятся на расстоянии \en samr_SurfSurf_Distant,      // located at a distance 

};


//------------------------------------------------------------------------------
/** \brief \ru Расстояние между осями поверхностей.
           \en Distance between axes of surfaces. \~
  \details \ru Рассчитывается расстояние между осями поверхностей, имеющих оси вращения,
    или расстояние между поверхностью, имеющей ось, и плоской поверхностью.
           \en Calculate distance between axes of revolution surfaces
    or distance between revolution surface and planar surface. \~
  \param[in] surface1, sameSense1 - \ru Первая поверхность и ее направление.
                                    \en The first surface and its direction. \~
  \param[in] surface2, sameSense2 - \ru Вторая поверхность и ее направление.
                                    \en The second surface and its direction. \~
  \param[out] axis1, exist1 - \ru Ось первой поверхности и флаг ее наличия.
                              \en The axis of the first surface and the flag of its existence. \~
  \param[out] axis2, exist2 - \ru Ось второй поверхности и флаг ее наличия.
                              \en The axis of the second surface and the flag of its existence. \~
  \param[out] p1 - \ru Точка на первой оси или поверхности.
                   \en The point on the first axis or surface. \~
  \param[out] p2 - \ru Точка на второй оси или поверхности.
                   \en The point on the second axis or surface. \~
  \param[out] angle - \ru Угол между осями или осью поверхностью.
                      \en The angle between axes or between an axis and a surface. \~
  \param[out] distance - \ru Минимальное расстояние между осями.
                         \en Minimal distance between axes. \~
  \param[in] angle - \ru Угловая погрешность.
                     \en The angle accuracy. \~
  \return \ru Вариант полученного замера или вариант ошибки.
          \en The variant of the obtained measurement or the variant of error. \~ 
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC (MbeSurfAxesMeasureRes) SurfAxesDistAngle( const MbSurface & surface1, bool sameSense1,
                                                     const MbSurface & surface2, bool sameSense2,
                                                     MbAxis3D & axis1, bool & exist1,
                                                     MbAxis3D & axis2, bool & exist2,
                                                     MbCartPoint3D & p1,
                                                     MbCartPoint3D & p2,
                                                     double & angle,
                                                     double & distance,
                                                     double angleEps = ANGLE_EPSILON );


//------------------------------------------------------------------------------
/** \brief \ru Расстояние между точками на объектах (кривая-кривая, кривая-поверхность или поверхность-поверхность).
           \en Distance between points on objects (curve-curve, curve-surface, surface-surface). \~
  \details \ru Класс содержит данные о расстоянии между точками и координатами этих точек на объектах.
           \en The class contains data about the distance between points and their coordinates on objects surfaces. \~
  \ingroup Algorithms_3D
*/ // ---
template <class Param1, class Param2>
class MbItemItemDist {
  template <typename Par1, typename Par2>
  friend class MbMinMaxItemItemDistances;
private:
  double  d;    ///< \ru Расстояние. \en Distance. 
  Param1  par1; ///< \ru Параметр на первом объекте. \en Parameter on the first object. 
  Param2  par2; ///< \ru Параметр на втором объекте. \en Parameter on the second object. 
  uint8   sign; ///< \ru Знак расстояния. \en Sign of direction. 

public:
  /// \ru Параметрическая точка (1D или 2D). \en Parametric point (1D or 2D). 
  struct ParamPoint {
    double x;
    double y;
    bool   s;
    ParamPoint( double t )              : x( t   ), y( t   ), s( false ) {}
    ParamPoint( const MbCartPoint & p ) : x( p.x ), y( p.y ), s( true  ) {}
  };

public:
  /// \ru Конструктор. \en Constructor. 
  MbItemItemDist() : d( UNDEFINED_DBL ), par1(), par2(), sign( 1 ) {}
  /// \ru Конструктор по данным. \en The constructor by data. 
  MbItemItemDist( double _d, const Param1 & _par1, const Param2 & _par2, bool plus ) { Init( _d, _par1, _par2, plus ); }
  /// \ru Конструктор копирования. \en Copy constructor. 
  MbItemItemDist( const MbItemItemDist & other ) { Init( other ); }
  /// \ru Деструктор. \en The destructor. 
  virtual ~MbItemItemDist() {}
public:
        /// \ru Функция копирования. \en Copy function. 
        void              Init( const MbItemItemDist & obj ) { d = obj.d; par1 = obj.par1; par2 = obj.par2; sign = obj.sign; }
        /// \ru Получить расстояние. \en Get distance. 
        double            GetDistance() const { return d;   } 
        /// \ru Получить точку на первой поверхности. \en Get the point on the first surface. 
  const Param1 &          GetParamOne() const { return par1; }
        /// \ru Получить точку на второй поверхности. \en Get the point on the second surface. 
  const Param2 &          GetParamTwo() const { return par2; }
        /// \ru Расстояние положительное? \en Is the distance positive? 
        bool              IsPositive() const { return (sign > 0); } 
        /// \ru Расстояние отрицательное? \en Is the distance negative? 
        bool              IsNegative() const { return (sign < 1); } 
        /// \ru Оператор присваивания. \en Assignment operator. 
  const MbItemItemDist  & operator = ( const MbItemItemDist & other ) { Init( other ); return (*this); }

private:
        // \ru Инициализатор. \en Initializer.
        void          Init( double _d, const Param1 & _par1, const Param2 & _par2, bool plus )
        {
          d = _d;
          par1 = _par1;
          par2 = _par2;
          sign = plus ? 1 : 0;
        }
};

//------------------------------------------------------------------------------
// MbItemItemDist typedefs
// ---
typedef MbItemItemDist<double, double>            MbCurvCurvDist;
typedef MbItemItemDist<double, MbCartPoint>       MbCurvSurfDist;
typedef MbItemItemDist<MbCartPoint, double>       MbSurfCurvDist;
typedef MbItemItemDist<MbCartPoint, MbCartPoint>  MbSurfSurfDist;


//------------------------------------------------------------------------------
// сортировать по возрастанию расстояния
// ---
template <class ItemItemDist>
bool ItemItemDistCompFunc( const ItemItemDist & sd1, const ItemItemDist & sd2 )
{
  bool isFirstLessSecond = false;

  const double mEps = LENGTH_EPSILON;
  const double pEps = PARAM_EPSILON;
  double d1 = sd1.GetDistance();
  double d2 = sd2.GetDistance();

  if ( d1 < d2 - mEps )
    isFirstLessSecond = true;
  else if ( ::fabs( d1 - d2 ) <= mEps ) {
    const typename ItemItemDist::ParamPoint parOne1( sd1.GetParamOne() );
    const typename ItemItemDist::ParamPoint parOne2( sd2.GetParamOne() );

    if ( parOne1.x < parOne2.x - pEps )
      isFirstLessSecond = true;
    else if ( parOne1.s && parOne2.s && ::fabs( parOne1.x - parOne2.x ) <= pEps ) {
      if ( parOne1.y < parOne2.y - pEps )
        isFirstLessSecond = true;
      else if ( ::fabs( parOne1.y - parOne2.y ) <= pEps ) {
        if ( d1 < d2 )
          isFirstLessSecond = true;
      }
    }
  }

  return isFirstLessSecond;
}


//------------------------------------------------------------------------------
/** \brief \ru Расстояния с точками между поверхностями.
           \en Distances between surfaces with points. \~
  \details \ru Расстояния с точками между поверхностями.
           \en Distances between surfaces with points. \~
  \ingroup Algorithms_3D
*/ // ---
template <class Param1, class Param2>
class MbMinMaxItemItemDistances {
private :
  std::vector< MbItemItemDist<Param1,Param2> >  allDistances; ///< \ru Расстояние и параметры на поверхностях. \en Distance and parameters on surfaces. 
  mutable double                                midDistance;  ///< \ru Среднее расстояние. \en Average distance. 
  mutable double                                minDistance;  ///< \ru Минимальное расстояние. \en Minimal distance. 
  mutable double                                maxDistance;  ///< \ru Максимальное расстояние. \en Maximal distance. 
  mutable bool                                  sorted;       ///< \ru Признак сортированности. \en Attribute of being sorted. 
public:
  /// \ru Конструктор. \en Constructor. 
  MbMinMaxItemItemDistances( size_t nReserve = 0 )
    : allDistances(               )
    , midDistance ( UNDEFINED_DBL )
    , minDistance ( UNDEFINED_DBL )
    , maxDistance ( UNDEFINED_DBL )
    , sorted      ( false         )
  {
    allDistances.reserve( std_min( nReserve, (size_t)c3d::COUNT_MAX ) );
  }
  /// \ru Деструктор. \en Destructor.
  virtual ~MbMinMaxItemItemDistances()
  {
    RemoveAll( true );
  }
public:
  bool      IsEmpty()     const { return allDistances.empty(); } ///< \ru Есть ли замеры? \en Are there any measurements? 
  size_t    GetCount()    const { return allDistances.size();  } ///< \ru Количество замеров. \en The number of measurements. 
  ptrdiff_t GetMaxIndex() const { return ((ptrdiff_t)allDistances.size() - 1); } ///< \ru Индекс последнего замера. \en Index of the last measurement 

  /// \ru Зарезервировать память под nReserve элементов. \en Reserve memory for 'nReserve' elements. 
  void      Reserve( size_t nReserve )
  {
    allDistances.reserve( allDistances.size() + nReserve );
  }
  /// \ru Удалить все элементы. \en Delete all elements. 
  void      RemoveAll( bool freeMemory )
  {
    allDistances.clear();
    if ( freeMemory ) {
      allDistances.shrink_to_fit();
    }
    midDistance = minDistance = maxDistance = UNDEFINED_DBL;
  }
  /// \ru Освободить лишнюю память. \en Free the unnecessary memory. 
  void      AdjustMemory()
  {
    allDistances.shrink_to_fit();
  }
public:
  /// \ru Получить расстояние по индексу. \en Get the distance by the index. 
  bool GetDistance( size_t k, double & d ) const
  {
    if ( k < allDistances.size() ) {
      d = allDistances[k].GetDistance();
      return true;
    }
    return false;
  }
  /// \ru Получить расстояние со знаком, по индексу. \en Get the signed distance by the index. 
  bool GetSignedDistance( size_t k, double & d ) const
  {
    if ( k < allDistances.size() ) {
      d = allDistances[k].GetDistance();
      if ( allDistances[k].IsNegative() )
        d = -d;
      return true;
    }
    return false;
  }
  /// \ru Считаем ли вы расстояние отрицательным. \en Whether the distance is negative. 
  bool IsNegativeDistance( size_t k ) const { return ((k < allDistances.size()) ? allDistances[k].IsNegative() : false); }

  /// \ru Получить минимальное расстояние. \en Get minimal distance. 
  bool GetMinDistance( double & d ) const
  {
    bool res = false;
    size_t count = allDistances.size();

    if ( count > 0 ) {
      if ( minDistance != UNDEFINED_DBL ) {
        d = minDistance;
        res = true;
      }
      else if ( count > 1 ) {
        minDistance = MB_MAXDOUBLE;
        for ( size_t k = 0; k < count; ++k ) {
          double curDistance = allDistances[k].GetDistance();
          if ( curDistance < minDistance ) {
            minDistance = curDistance;
            res = true;
          }
        }
        if ( res )
          d = minDistance;
        else
          minDistance = UNDEFINED_DBL;
      }
      else {
        minDistance = allDistances.front().GetDistance();
        d = minDistance;
        res = true;
      }
    }

    return res;
  }
  /// \ru Получить максимальное расстояние. \en Get maximal distance. 
  bool GetMaxDistance( double & d ) const
  {
    bool res = false;
    size_t count = allDistances.size();

    if ( count > 0 ) {
      if ( maxDistance != UNDEFINED_DBL ) {
        d = maxDistance;
        res = true;
      }
      else if ( count > 1 ) {
        maxDistance = -MB_MAXDOUBLE;
        for ( size_t k = 0; k < count; ++k ) {
          double curDistance = allDistances[k].GetDistance();
          if ( curDistance > maxDistance ) {
            maxDistance = curDistance;
            res = true;
          }
        }
        if ( res )
          d = maxDistance;
        else
          maxDistance = UNDEFINED_DBL;
      }
      else {
        maxDistance = allDistances.front().GetDistance();
        d = maxDistance;
        res = true;
      }
    }

    return res;
  }
  /// \ru Получить среднее расстояние. \en Get average distance. 
  bool GetMidDistance( double & d ) const
  {
    bool res = false;
    size_t count = allDistances.size();

    if ( count > 0 ) {
      if ( count > 1 ) {
        if ( midDistance != UNDEFINED_DBL ) {
          d = midDistance;
          res = true;
        }
        else {
          midDistance = 0.0;
          for ( size_t k = 0; k < count; ++k )
            midDistance += allDistances[k].GetDistance();
          midDistance /= ((double)count);
          d = midDistance;
          res = true;
        }
      }
      else {
        midDistance = allDistances.front().GetDistance();
        d = midDistance;
        res = true;
      }
    }

    return res;
  }
  
  /// \ru Получить расстояние и точки на поверхностях. \en Get distance and points on surface. 
  bool GetItemDistance( size_t k, double & d, Param1 & par1, Param2 & par2 ) const
  {
    if ( k < allDistances.size() ) {
      d = allDistances[k].GetDistance();
      par1 = allDistances[k].GetParamOne();
      par2 = allDistances[k].GetParamTwo();
      return true;
    }
    return false;
  }

  /// \ru Получить расстояние и точки на поверхностях. \en Get distance and points on surface. 
  bool GetItemDistance( size_t k, double & d, bool & plus, Param1 & par1, Param2 & par2 ) const
  {
    if ( k < allDistances.size() ) {
      d = allDistances[k].GetDistance();
      par1 = allDistances[k].GetParamOne();
      par2 = allDistances[k].GetParamTwo();
      plus = allDistances[k].IsPositive();
      return true;
    }
    return false;
  }

  /// \ru Добавить расстояние и точки на поверхностях. \en Add distance and points on surface. 
  bool AddItemDistance( double d, bool plus, const Param1 & par1, const Param2 & par2, 
                        bool addEqual, double eps = LENGTH_EPSILON )
  {
    if ( ::fabs( d ) < LENGTH_EPSILON )
      d = 0.0;
    C3D_ASSERT( d >= 0 );

    if ( d > -NULL_EPSILON ) { // KOMPAS-66491. Нулевые расстояния должны добавляться в результат.
      size_t count = allDistances.size();

      bool add = true;

      if ( !addEqual ) {
        for ( size_t k = 0; k < count; ++k ) {
          if ( ::fabs( d - allDistances[k].GetDistance() ) < eps ) {
            add = false;
            break;
          }
        }
      }
      if ( add ) {
        if ( count > 0 ) {
          if ( maxDistance != UNDEFINED_DBL && d > maxDistance )
            maxDistance = d;
          else if ( minDistance != UNDEFINED_DBL && d < minDistance )
            minDistance = d;
        }
        else
          minDistance = maxDistance = d;

        MbItemItemDist<Param1,Param2> surfDistance( d, par1, par2, plus );
        allDistances.push_back( surfDistance );
        midDistance = UNDEFINED_DBL;
        return true;
      }
    }

    return false;
  }

  /// \ru Сортировать по возрастанию расстояния. \en Sort by distance in the ascending order. 
  void Sort()
  {
    if ( allDistances.size() > 1 ) {
      std::sort( allDistances.begin(), allDistances.end(), ItemItemDistCompFunc< MbItemItemDist<Param1,Param2> > );
      minDistance = allDistances.front().GetDistance();
      maxDistance = allDistances.back().GetDistance();
      sorted = true;
    }
  }
  /// \ru Убрать объекты с одинаковыми расстояниями. \en Remove objects with similar distances. 
  void RemoveEqualDistances( double eps = LENGTH_EPSILON )
  {
    if ( allDistances.size() > 1 ) {
      if ( !sorted )
        Sort();

      bool wasRemoved = false;
      for ( ptrdiff_t k = ((ptrdiff_t)allDistances.size() - 1); k > 0; k-- ) {
        ptrdiff_t m = k - 1;
        double dThis = allDistances[k].GetDistance();
        double dPrev = allDistances[m].GetDistance();
        if ( ::fabs( dThis - dPrev ) < eps ) {
          allDistances.erase( allDistances.begin() + m );
          wasRemoved = true;
        }
      }
      if ( wasRemoved ) {
        midDistance = UNDEFINED_DBL;
        minDistance = allDistances.front().GetDistance();
        maxDistance = allDistances.back().GetDistance();
      }
    }
  }
  /// \ru Оператор присваивания. \en Assignment operator. 
  void operator = ( const MbMinMaxItemItemDistances & other )
  {
    allDistances = other.allDistances;  // \ru расстояние и параметры на поверхностях
    midDistance = other.midDistance;    // \ru среднее расстояние
    minDistance = other.minDistance;    // \ru минимальное расстояние
    maxDistance = other.maxDistance;    // \ru максимальное расстояние
    sorted = other.sorted;              // \ru признак сортированности
  }

private:
  MbMinMaxItemItemDistances( const MbMinMaxItemItemDistances & );
};

//------------------------------------------------------------------------------
// MbMinMaxSurfSurfDists typedefs
// ---
typedef MbMinMaxItemItemDistances<double, double>           MbMinMaxCurvCurvDists;
typedef MbMinMaxItemItemDistances<double, MbCartPoint>      MbMinMaxCurvSurfDists;
typedef MbMinMaxItemItemDistances<MbCartPoint, double>      MbMinMaxSurfCurvDists;
typedef MbMinMaxItemItemDistances<MbCartPoint, MbCartPoint> MbMinMaxSurfSurfDists;


//------------------------------------------------------------------------------
/** \brief \ru Параметры операции сеточного поиска минимумов и максимумов.
           \en Parameters of the operation of the grid search for minima and maxima. \~
  \details \ru Параметры операции сеточного поиска минимумов и максимумов расстояний между объектами.
           \en Parameters of the grid search operation for minima and maxima of distances between objects. \~
  \ingroup Algorithms_3D
*/ // ---
class MATH_CLASS MbMinMaxGridDistancesParams {
public:
  /// \ru Режим проецирования сетки базового объекта на целевой объект. \en Projection mode of the base object grid onto the target object.
  enum class ProjectionMode
  {
    om_Radiance           = 0, ///< \ru Поиск экстремальных расстояний по нормали к базовому объекту. \en Search for extreme distances along the normal to the base object. \~
    om_NearestDistance    = 1, ///< \ru Поиск экстремальных расстояний по нормали к целевому объекту. \en Search for extreme distances along the normal to the target object. \~
    om_SpecifiedDirection = 2  ///< \ru Поиск экстремальных расстояний в заданном направлении. \en Search for extreme distances in a given direction. \~
  };

public:
  c3d::ConstSpaceItemSPtr       srcItem;              ///< \ru Базовый объект (кривая или поверхность). \en Base object (curve or surface). \~
  c3d::IndicesPair              srcSplitsCount;       ///< \ru Количество разбиений (точек). \en Number of partitions (points). \~
  c3d::ConstSpaceItemSPtr       dstItem;              ///< \ru Целевой объект (кривая или поверхность). \en Target object (curve or surface). \~
  MbVector3D                    projDirection;        ///< \ru Вектор заданного направления (если нет, то по нормали). \en The vector of direction (if not set then by the normal). \~
  MbeSenseValue                 projOrient;           ///< \ru Относительное направление поиска. \en Relative search direction. \~
  bool                          useEqualDistances;    ///< \ru Оставлять равные расстояния. \en Whether to use the equal distances. \~
  size_t                        desiredMinimaNumber;  ///< \ru Желаемое число выдаваемых минимумов. \en Desired minima number.
  size_t                        desiredMaximaNumber;  ///< \ru Желаемое число выдаваемых максимумов. \en Desired maxima number.
  VERSION                       version;              ///< \ru Версия. \en Version.
private:
  mutable IProgressIndicator *  progress;             ///< \ru Индикатор прогресса выполнения операции. \en A progress indicator of the operation.
          ProjectionMode        projectionMode;       ///< \ru Режим проецирования сетки базового объекта на целевой объект. \en Projection mode of the base object grid onto the target object.
private:
  MbMinMaxGridDistancesParams(); // \ru Не реализовано. \en Not implemented.

public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор параметров операции для измерения экстремальных расстояний между поверхностями. Режим проецирования - по нормали к базовой поверхности.
             \en Constructor of operation parameters for measuring extreme distances between surfaces. The projection mode is along the normal to the base surface. \~
    \param[in] srcSurface - \ru Базовая поверхность.
                            \en Base surface. \~
    \param[in] srcUCnt    - \ru Количество разбиений базовой поверхности по U.
                            \en Number of partitions of the base surface in U. \~
    \param[in] srcVCnt    - \ru Количество разбиений базовой поверхности по V.
                            \en Number of partitions of the base surface in V. \~
    \param[in] dstSurface - \ru Целевая поверхность.
                            \en Target surface. \~
    \param[in] ver        - \ru Версия операции.
                            \en Operation version. \~
  */
  MbMinMaxGridDistancesParams( const MbSurface & srcSurface,
                                     size_t      srcUCnt,
                                     size_t      srcVCnt,
                               const MbSurface & dstSurface,
                                     VERSION     ver = Math::DefaultMathVersion() )
    : srcItem            ( &srcSurface                 )
    , srcSplitsCount     ( srcUCnt, srcVCnt            )
    , dstItem            ( &dstSurface                 )
    , projDirection      (                             )
    , projOrient         ( orient_BOTH                 )
    , useEqualDistances  ( false                       )
    , desiredMinimaNumber( 1                           )
    , desiredMaximaNumber( 1                           )
    , version            ( ver                         )
    , progress           ( nullptr                     )
    , projectionMode     ( ProjectionMode::om_Radiance )
  {}
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор параметров операции для измерения экстремальных расстояний между поверхностями. Если не задан вектор направления, то режим проецирования - по нормали к базовой поверхности.
             \en Constructor of operation parameters for measuring extreme distances between surfaces. If the direction vector is not specified, then the projection mode is normal to the base surface. \~
    \param[in] srcSurface    - \ru Базовая поверхность.
                               \en Base surface. \~
    \param[in] srcUCnt       - \ru Количество разбиений базовой поверхности по U.
                               \en Number of partitions of the base surface in U. \~
    \param[in] srcVCnt       - \ru Количество разбиений базовой поверхности по V.
                               \en Number of partitions of the base surface in V. \~
    \param[in] dir           - \ru Вектор заданного направления (если нет, то по нормали).
                               \en The vector of direction (if not set then by the normal). \~
    \param[in] orient        - \ru Относительное направление поиска.
                               \en Relative search direction. \~
    \param[in] useEqualDists - \ru Оставлять равные расстояния.
                               \en Whether to use the equal distances. \~
    \param[in] dstSurface    - \ru Целевая поверхность.
                               \en Target surface. \~
    \param[in] nMin          - \ru Желаемое число выдаваемых минимумов.
                               \en Desired minima number. \~
    \param[in] nMax          - \ru Желаемое число выдаваемых максимумов.
                               \en Desired maxima number. \~
    \param[in] ver           - \ru Версия операции.
                               \en Operation version. \~
  */
  MbMinMaxGridDistancesParams( const MbSurface &     srcSurface,
                                     size_t          srcUCnt,
                                     size_t          srcVCnt,
                               const MbVector3D *    dir,
                               const MbeSenseValue & orient,
                                     bool            useEqualDists,
                               const MbSurface &     dstSurface,
                                     size_t          nMin,
                                     size_t          nMax,
                                     VERSION         ver = Math::DefaultMathVersion() )
    : srcItem            ( &srcSurface                 )
    , srcSplitsCount     ( srcUCnt, srcVCnt            )
    , dstItem            ( &dstSurface                 )
    , projDirection      (                             )
    , projOrient         ( orient                      )
    , useEqualDistances  ( useEqualDists               )
    , desiredMinimaNumber( nMin                        )
    , desiredMaximaNumber( nMax                        )
    , version            ( ver                         )
    , progress           ( nullptr                     )
    , projectionMode     ( ProjectionMode::om_Radiance )
  {
    SetProjectionDirection( dir, orient );
    if ( dir != nullptr )
      projectionMode = ProjectionMode::om_SpecifiedDirection;
  }
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор параметров операции для измерения экстремальных расстояний между поверхностями. Режим проецирования - по нормали к целевой поверхности.
             \en Constructor of operation parameters for measuring extreme distances between surfaces. Projection mode is along the normal to the target surface. \~
    \param[in] srcSurface    - \ru Базовая поверхность.
                               \en Base surface. \~
    \param[in] srcUCnt       - \ru Количество разбиений базовой поверхности по U.
                               \en Number of partitions of the base surface in U. \~
    \param[in] srcVCnt       - \ru Количество разбиений базовой поверхности по V.
                               \en Number of partitions of the base surface in V. \~
    \param[in] useEqualDists - \ru Оставлять равные расстояния.
                               \en Whether to use the equal distances. \~
    \param[in] dstSurface    - \ru Целевая поверхность.
                               \en Target surface. \~
    \param[in] nMin          - \ru Желаемое число выдаваемых минимумов.
                               \en Desired minima number. \~
    \param[in] nMax          - \ru Желаемое число выдаваемых максимумов.
                               \en Desired maxima number. \~
    \param[in] ver           - \ru Версия операции.
                               \en Operation version. \~
  */
  MbMinMaxGridDistancesParams( const MbSurface & srcSurface,
                                     size_t      srcUCnt,
                                     size_t      srcVCnt,
                                     bool        useEqualDists,
                               const MbSurface & dstSurface,
                                     size_t      nMin,
                                     size_t      nMax,
                                     VERSION     ver = Math::DefaultMathVersion() )
    : srcItem            ( &srcSurface                        )
    , srcSplitsCount     ( srcUCnt, srcVCnt                   )
    , dstItem            ( &dstSurface                        )
    , projDirection      (                                    )
    , projOrient         (                                    )
    , useEqualDistances  ( useEqualDists                      )
    , desiredMinimaNumber( nMin                               )
    , desiredMaximaNumber( nMax                               )
    , version            ( ver                                )
    , progress           ( nullptr                            )
    , projectionMode     ( ProjectionMode::om_NearestDistance )
  {}
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор параметров операции для измерения экстремальных расстояний между кривой и поверхностью. Режим проецирования - по нормали к целевой поверхности.
             \en Constructor of operation parameters for measuring extreme distances between the curve and the surface. The projection mode is along the normal to the target surface. \~
    \param[in] srcCurve   - \ru Базовая кривая.
                            \en Base curve. \~
    \param[in] srcTCnt    - \ru Количество разбиений базовой кривой.
                            \en Number of partitions of the base curve. \~
    \param[in] dstSurface - \ru Целевая поверхность.
                            \en Target surface. \~
    \param[in] ver        - \ru Версия операции.
                            \en Operation version. \~
  */
  MbMinMaxGridDistancesParams( const MbCurve3D & srcCurve,
                                     size_t      srcTCnt, 
                               const MbSurface & dstSurface, 
                                     VERSION     ver = Math::DefaultMathVersion() )
    : srcItem            ( &srcCurve                          )
    , srcSplitsCount     ( srcTCnt, 0                         )
    , dstItem            ( &dstSurface                        )
    , projDirection      (                                    )
    , projOrient         ( orient_BOTH                        )
    , useEqualDistances  ( false                              )
    , desiredMinimaNumber( 1                                  )
    , desiredMaximaNumber( 1                                  )
    , version            ( ver                                )
    , progress           ( nullptr                            )
    , projectionMode     ( ProjectionMode::om_NearestDistance )
  {}
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор параметров операции для измерения экстремальных расстояний между кривой и поверхностью. Если не задан вектор направления, то режим проецирования - по нормали к целевой поверхности.
             \en Constructor of operation parameters for measuring extreme distances between the curve and the surface. If the direction vector is not specified, then the projection mode is normal to the target surface. \~
    \param[in] srcCurve      - \ru Базовая кривая.
                               \en Base curve. \~
    \param[in] srcTCnt       - \ru Количество разбиений базовой кривой.
                               \en Number of partitions of the base curve. \~
    \param[in] dir           - \ru Вектор заданного направления (если нет, то по нормали).
                               \en The vector of direction (if not set then by the normal). \~
    \param[in] orient        - \ru Относительное направление поиска.
                               \en Relative search direction. \~
    \param[in] useEqualDists - \ru Оставлять равные расстояния.
                               \en Whether to use the equal distances. \~
    \param[in] dstSurface    - \ru Целевая поверхность.
                               \en Target surface. \~
    \param[in] nMin          - \ru Желаемое число выдаваемых минимумов.
                               \en Desired minima number. \~
    \param[in] nMax          - \ru Желаемое число выдаваемых максимумов.
                               \en Desired maxima number. \~
    \param[in] ver           - \ru Версия операции.
                               \en Operation version. \~
  */
  MbMinMaxGridDistancesParams( const MbCurve3D &     srcCurve,
                                     size_t          srcTCnt,
                               const MbVector3D *    dir,
                               const MbeSenseValue & orient,
                                     bool            useEqualDists,
                               const MbSurface &     dstSurface,
                                     size_t          nMin,
                                     size_t          nMax,
                                     VERSION         ver = Math::DefaultMathVersion() )
    : srcItem            ( &srcCurve                          )
    , srcSplitsCount     ( srcTCnt, 0                         )
    , dstItem            ( &dstSurface                        )
    , projDirection      (                                    )
    , projOrient         ( orient                             )
    , useEqualDistances  ( useEqualDists                      )
    , desiredMinimaNumber( nMin                               )
    , desiredMaximaNumber( nMax                               )
    , version            ( ver                                )
    , progress           ( nullptr                            )
    , projectionMode     ( ProjectionMode::om_NearestDistance )
  {
    SetProjectionDirection( dir, orient );
    if ( dir != nullptr )
      projectionMode = ProjectionMode::om_SpecifiedDirection;
  }
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор параметров операции для измерения экстремальных расстояний между кривыми. Режим проецирования - по нормали к целевой кривой.
             \en Constructor of operation parameters for measuring extreme distances between curves. The projection mode is along the normal to the target curve. \~
    \param[in] srcCurve - \ru Базовая кривая.
                          \en Base curve. \~
    \param[in] srcTCnt  - \ru Количество разбиений базовой кривой.
                          \en Number of partitions of the base curve. \~
    \param[in] dstCurve - \ru Целевая кривая.
                          \en Target curve. \~
    \param[in] ver      - \ru Версия операции.
                          \en Operation version. \~
  */
  MbMinMaxGridDistancesParams( const MbCurve3D & srcCurve,
                                     size_t      srcTCnt,
                               const MbCurve3D & dstCurve,
                                     VERSION     ver = Math::DefaultMathVersion() )
    : srcItem            ( &srcCurve                          )
    , srcSplitsCount     ( srcTCnt, 0                         )
    , dstItem            ( &dstCurve                          )
    , projDirection      (                                    )
    , projOrient         ( orient_BOTH                        )
    , useEqualDistances  ( false                              )
    , desiredMinimaNumber( 1                                  )
    , desiredMaximaNumber( 1                                  )
    , version            ( ver                                )
    , progress           ( nullptr                            )
    , projectionMode     ( ProjectionMode::om_NearestDistance )
  {}
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор параметров операции для измерения экстремальных расстояний между кривыми. Режим проецирования - либо по нормали к целевой кривой, либо в плоскости, перпендикулярной касательной базовой кривой.
             \en Constructor of operation parameters for measuring extreme distances between curves. Projection mode - either normal to the target curve, or in a plane perpendicular to the tangent of the base curve. \~
    \param[in] srcSurface    - \ru Базовая кривая.
                               \en Base curve. \~
    \param[in] srcTCnt       - \ru Количество разбиений базовой кривой.
                               \en Number of partitions of the base curve. \~
    \param[in] useEqualDists - \ru Оставлять равные расстояния.
                               \en Whether to use the equal distances. \~
    \param[in] byNormal      - \ru byNormal = true задает режим проецирования по нормали к целевой кривой.
                               \en byNormal = true sets the projection mode normal to the target curve. \~
    \param[in] dstCurve      - \ru Целевая кривая.
                               \en Target curve. \~
    \param[in] nMin          - \ru Желаемое число выдаваемых минимумов.
                               \en Desired minima number. \~
    \param[in] nMax          - \ru Желаемое число выдаваемых максимумов.
                               \en Desired maxima number. \~
    \param[in] ver           - \ru Версия операции.
                               \en Operation version. \~
  */
  MbMinMaxGridDistancesParams( const MbCurve3D & srcCurve,
                                     size_t      srcTCnt,
                                     bool        useEqualDists,
                                     bool        byNormal,
                               const MbCurve3D & dstCurve,
                                     size_t      nMin,
                                     size_t      nMax,
                                     VERSION     ver = Math::DefaultMathVersion() )
    : srcItem            ( &srcCurve                          )
    , srcSplitsCount     ( srcTCnt, 0                         )
    , dstItem            ( &dstCurve                          )
    , projDirection      (                                    )
    , projOrient         ( orient_FORWARD                     )
    , useEqualDistances  ( useEqualDists                      )
    , desiredMinimaNumber( nMin                               )
    , desiredMaximaNumber( nMax                               )
    , version            ( ver                                )
    , progress           ( nullptr                            )
    , projectionMode     ( ProjectionMode::om_NearestDistance )
  {
    if ( !byNormal ) {
      MbVector3D vect( 1.0, 1.0, 1.0 );
      SetProjectionDirection( &vect, orient_FORWARD );
      projectionMode = ProjectionMode::om_Radiance;
    }
  }

public:
        /// \ru Получить базовый объект. \en Get base object.
  const MbSpaceItem &         GetBaseItem  () const { return *srcItem; }
        /// \ru Получить целевой объект. \en Get target object.
  const MbSpaceItem &         GetTargetItem() const { return *dstItem; }
        /// \ru Получить режим проецирования. \en Get projection mode.
  const ProjectionMode &      GetProjectionMode() const { return projectionMode; }
        /// \ru Получить количество разбиений базового объекта. \en Get base object splits count.
  const c3d::IndicesPair &    GetBaseSplitsCount() const { return srcSplitsCount; }
        /// \ru Получить общий вектора поиска. \en Get general search direction.
        bool                  GetProjectionDirection( MbVector3D & dir, MbeSenseValue & orient ) const
        {
          orient = projOrient; // KOMPAS-60343, KOMPAS-60407.
          if ( projDirection.Length() > LENGTH_EPSILON ) {
            dir = projDirection;
            return true;
          }
          return false;
        }
        /// \ru Получить флаг добавления в результат равных расстояний. \en Get the flag for adding equal distances to the result.
        bool                  GetUseEqualDistances() const { return useEqualDistances;  }
        /// \ru Получить желаемое число выдаваемых минимумов. \en Get desired minima number.
        size_t                GetDesiredMinimaNumber() const { return desiredMinimaNumber; }
        /// \ru Получить желаемое число выдаваемых максимумов. \en Get desired maxima number.
        size_t                GetDesiredMaximaNumber() const { return desiredMaximaNumber; }
        /// \ru Получить версию. \en Get version.
        VERSION               GetVersion() const { return version; }
public:
        /// \ru Установить общее направление поиска. \en Set general search direction.
        bool                  SetProjectionDirection( const MbVector3D * dirPtr, MbeSenseValue orient )
        {
          if ( dirPtr != nullptr ) {
            const MbVector3D & dir = *dirPtr;
            double dirLen = dir.Length();
            if ( dirLen > LENGTH_EPSILON && dirLen < c3d::MAX_LENGTH ) {
              projDirection = dir;
              projOrient = orient;
              return true;
            }
          }
          return false;
        }
        /// \ru Установить флаг использования одинаковых расстояний. \en Set flag to use equal distances.
        void                  SetUseEqualDistances( bool useEqualDists ) { useEqualDistances = useEqualDists; }
        /// \ru Установить желаемые числа выдаваемых минимумов и максимумов. \en Get desired minima and maxima numbers.
        void                  SetDisiredMinMaxNumbers( size_t minNum, size_t maxNum ) { desiredMinimaNumber = minNum; desiredMaximaNumber = maxNum; }
public:
        /// \ru Установить внешний индикатор прогресса выполнения. \en Set external progress indicator.
        void                  SetProgressIndicator( IProgressIndicator * prog ) { progress = prog; }
        /// \ru Взять внешний индикатор прогресса выполнения. \en Take external progress indicator.
        IProgressIndicator *  TakeProgressIndicator() const { return progress; }

OBVIOUS_PRIVATE_COPY( MbMinMaxGridDistancesParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Результаты операции сеточного поиска минимумов и максимумов.
           \en Results of the operation of the grid search for minima and maxima. \~
  \details \ru Результаты операции сеточного поиска минимумов и максимумов расстояний между объектами.
           \en Results of the grid search operation for minima and maxima of distances between objects. \~
  \ingroup Algorithms_3D
*/ // ---
template <class Param1, class Param2>
class MbMinMaxGridDistancesResults {
public:
  MbMinMaxItemItemDistances<Param1, Param2> allResults; ///< \ru Все результаты. \en All results. \~
  MbMinMaxItemItemDistances<Param1, Param2> minResults; ///< \ru Результаты-минимумы. \en Results-minimums. \~
  MbMinMaxItemItemDistances<Param1, Param2> maxResults; ///< \ru Результаты-максимумы. \en Results-maximums. \~
private:
  MbResultType                              resType { rt_Empty }; ///< \ru Код результата операции. \en Operation result code. \~

public:
  /// \ru Получить фактическое число минимумов. \en Get real minima number.
  size_t                GetActualMinimaNumber()  const { return minResults.GetCount(); }
  /// \ru Получить фактическое число максимумов. \en Get desired maxima number.
  size_t                GetActualMaximaNumber()  const { return maxResults.GetCount(); }
  /// \ru Получить код результата операции. \en Get operation result code.
  MbResultType          GetOperationResultCode() const { return resType; }
  /// \ru Установить код результата операции. \en Set operation result code.
  void                  SetOperationResultCode( MbResultType rt ) { resType = rt; }
};


//------------------------------------------------------------------------------
/** \brief \ru Экстремальные расстояния между объектами (кривыми или поверхностями).
           \en Extreme distances between objects (curves or surfaces). \~
  \details \ru Экстремальные расстояния между объектами по сетке на первом объекте,
               причем замеры выполняются в заданном направлении (если есть вектор)
               или по нормалям к первому объекту.
           \en Extreme distances between objects along the grid on the first object, 
               and measurements are taken in a given direction (if there is a vector) 
               or along the normal to the first object. \~
  \param[in] params -   \ru Параметры операции сеточного поиска минимумов и максимумов.
                        \en Parameters of the operation of the grid search for minima and maxima. \~
  \param[out] results - \ru Результаты операции сеточного поиска минимумов и максимумов.
                        \en Results of the operation of the grid search for minima and maxima. \~
  \return \ru Возвращает результат замера (получен, не получен или же процесс был прерван).
          \en Returns the result of measurement (obtained, not obtained, or the process has been aborted). \~
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC (MbeProcessState) MinMaxSurfaceSurfaceGridDistances( const MbMinMaxGridDistancesParams &                            params,
                                                                     MbMinMaxGridDistancesResults<MbCartPoint, MbCartPoint> & results );



//------------------------------------------------------------------------------
/** \brief \ru Экстремальные расстояния между объектами (кривыми и поверхностями).
           \en Extreme distances between objects (curves and surfaces). \~
  \details \ru Экстремальные расстояния между объектами по сетке на кривой,
               причем замеры выполняются в заданном направлении (если есть
               вектор) или по нормали к поверхности.
           \en Extreme distances between objects along the grid on the curve, 
               and measurements are taken in a given direction (if there is
               a vector) or along the normal to the surface. \~
  \param[in] params   - \ru Параметры операции сеточного поиска минимумов и максимумов.
                        \en Parameters of the operation of the grid search for minima and maxima. \~
  \param[out] results - \ru Результаты операции сеточного поиска минимумов и максимумов.
                        \en Results of the operation of the grid search for minima and maxima. \~
  \return \ru Возвращает результат замера (получен, не получен или же процесс был прерван).
          \en Returns the result of measurement (obtained, not obtained, or the process has been aborted). \~
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC (MbeProcessState) MinMaxCurveSurfaceGridDistances( const MbMinMaxGridDistancesParams &                       params,
                                                                   MbMinMaxGridDistancesResults<double, MbCartPoint> & results );


//------------------------------------------------------------------------------
/** \brief \ru Экстремальные расстояния между кривыми.
           \en Extreme distances between curves. \~
  \details \ru Экстремальные расстояния между кривыми по сетке,
               причем замеры осуществляются в точках разбивки первой кривой по
               нормали ко второй кривой или между точками разбивки первой кривой
               и точками второй кривой.
           \en Extreme distances between curves along the grid, 
               and measurements are taken at the break points of the first curve
               along the normal to the second curve or between the break points
               of the first curve and the points of the second curve. \~
  \param[in] params   - \ru Параметры операции сеточного поиска минимумов и максимумов.
                        \en Parameters of the operation of the grid search for minima and maxima. \~
  \param[out] results - \ru Результаты операции сеточного поиска минимумов и максимумов.
                        \en Results of the operation of the grid search for minima and maxima. \~
  \return \ru Возвращает результат замера (получен, не получен или же процесс был прерван).
          \en Returns the result of measurement (obtained, not obtained, or the process has been aborted). \~
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC (MbeProcessState) MinMaxCurveCurveGridDistances( const MbMinMaxGridDistancesParams &                  params,
                                                                 MbMinMaxGridDistancesResults<double, double> & results );
#endif // __ALG_DIMENSION_H
