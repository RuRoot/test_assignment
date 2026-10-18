////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Кривая пересечения двух поверхностей.
         \en Intersection curve of two surfaces. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_SURFACE_INTERSECTION_H
#define __CUR_SURFACE_INTERSECTION_H


#include <cur_surface_curve.h>
#include <mb_cube.h>
#include <curve.h>
#include <utility>
#include <array>
#include <set>
#include <tool_multithreading.h>
#include <templ_sptr.h>


class MbMatrix;
class MbCurve;
class MbSurface;
class MbSurfaceIntersectionData;
class MbSurfaceIntersectionCurve;
class MbReparamCurve;
class MbCurveIntoNurbsInfo;

namespace c3d // namespace C3D
{
  typedef SPtr<MbSurfaceIntersectionCurve>                IntersectionCurveSPtr;
  typedef SPtr<const MbSurfaceIntersectionCurve>          ConstIntersectionCurveSPtr;

  typedef std::vector<MbSurfaceIntersectionCurve *>       IntersectionCurvesVector;
  typedef std::vector<const MbSurfaceIntersectionCurve *> ConstIntersectionCurvesVector;

  typedef std::vector<IntersectionCurveSPtr>              IntersectionCurvesSPtrVector;
  typedef std::vector<ConstIntersectionCurveSPtr>         ConstIntersectionCurvesSPtrVector;
}

//------------------------------------------------------------------------------
/** \brief \ru Кривая пересечения двух поверхностей.
           \en Intersection curve of two surfaces. \~
  \details \ru Кривая пересечения поверхностей содержит две кривые на пересекаемых поверхностях 
    curveOne и curveTwo, подчинённые следующим правилам:\n
    1. кривые имеют одинаковые области определения,\n
    2. кривые выдают один и тот же радиус-вектор и его производные при одинаковых параметрах, то есть, кривые одинаково направлены и совпадают в пространстве.\n
    Параметр buildType кривой пересечения информирует о том, каким образом выполняется второе правило. 
    Если параметр buildType==cbt_Ordinary или buildType==cbt_Boundary, то правила выполняются точно.
    Если параметр buildType==cbt_Specific или buildType==cbt_Tolerant, то второе правило выполняются приближённо. \n
    В общем случае (buildType==cbt_Specific) кривая пересечения поверхностей представлена в виде двух двумерных сплайнов curveOne.curve и curveTwo.curve. 
    Сплайны проходят через двумерные опорные точки в пространстве параметров поверхностей curveOne.surface и curveTwo.surface, соответственно. 
    Каждой опорной точке сплайна curveOne соответствует опорная точка сплайна curveTwo. 
    В соответствующих опорных точках параметры сплайнов равны, а сплайны совпадают в пространстве. 
    Таким образом, в опорных точках сплайнов второе правило кривой пересечения выполняется точно. \n
      Если через опорные точки провести пространственную ломаную, то угол между её соседними отрезками не будет превышать 0,04pi. 
    Изменение параметра при переходе от одной опорной точки к следующей опорной точке кривой пропорционально длине участка ломаной между соседними точками. \n
      Для любого значения параметра точка пересечения поверхностей вычисляются точно из решения системы уравнений пересечения поверхностей.\n
    Для определения точного пересечения поверхностей между соседними опорными точками сплайнов выполняются следующие действия. 
    Строится плоскость, перпендикулярная отрезку, начинающемуся и оканчивающемуся в соседних опорных точках. 
    Далее численным методом определяется точка пересечения трёх поверхностей: curveOne.surface, curveTwo.surface и плоскости.\n
      В частном случае кривая пересечения может описывать край поверхности (buildType==cbt_Boundary), тогда кривые равны и лежат на одной и той же поверхности.\n
      В редких случаях второе правило кривой пересечения выполнить точно невозможно, но оно выполняется с известной погрешностью.
    В этих случаях параметр buildType==cbt_Tolerant, пересекающиеся поверхности касаются друг друга по кривой пересечения,
    а вычисление точки кривой из решения системы уравнений пересечения поверхностей затруднено из-за неоднозначности решения.\n
      Все действия, связанные с построением кривой пересечения, обеспечением её точности и определением параметра buildType, 
    выполняются до вызова конструктора кривой.\n
      Кривая spaceCurve может отсутствовать, она насчитывается при необходимости, ничего не знает о поверхностях и
    в общем случае является аппроксимационной. Она используется там, где не важна точность.\n
      Кривая пересечения поверхностей используется для стыковки поверхностей или для описания ребра стыковки двух граней.
    Кривая пересечения может описывать разные типы рёбер стыковки двух граней: \n
    обычное ребро - поверхности разные, двумерные кривые разные,\n
    ребро-шов - поверхность одна и та же, двумерные кривые разные не равные,\n
    Ребро-линия разъема - поверхности копии, двумерные кривые копии,\n
    ребро-край - поверхность одна и та же, двумерная кривая одна и та же,\n
    ребро-полюс - поверхность одна и та же, двумерные кривые копии.\n
      Если две двумерные кривые кривых на поверхности curveOne curveTwo являются контурами, то количество сегментов в них должно быть одинаковым.
           \en Intersection curve of surfaces contains two curves on intersected surfaces - 
    curveOne and curveTwo, conformed to the next rules:\n
    1. curves have the same domains,\n
    2. curves return the same radius-vector and its derivatives at the same parameters, that is, curves are equally directed and coincide in space.\n
    'buildType' parameter of the intersection curve informs about the way the second rule is carried out. 
    If 'buildType' parameter is equal to cbt_Ordinary or cbt_Boundary, then the rules are satisfied exactly.
    If 'buildType' parameter is equal to cbt_Specific or cbt_Tolerant, then the rules are satisfied approximately. \n
    In the common case (buildType==cbt_Specific) intersection curve of surfaces is presented in form of two splines curveOne.curve and curveTwo.curve. 
    Splines pass through two-dimensional support points in parameter spaces of curveOne.surface and curveTwo.surface surfaces correspondingly. 
    Support points of 'curveTwo' spline corresponds to each support point of 'curveOne' spline. 
    Parameters of splines are equal for corresponding support points, and splines coincide in space. 
    Thus, the second rule of intersection curve is satisfied exactly in support points. \n
      If one passes a spatial polyline through the support points, then the angle between its neighboring segments won't exceed 0,04pi. 
    The change of parameter while moving from one support point of curve to the next one is proportional to the length of the polyline segment between the neighboring points. \n
      For any value of parameter the point of surfaces intersection is calculated precisely from the solution of surfaces intersection equations system.\n
    For determination of precise surfaces intersection between neighboring support points of splines the following actions are performed. 
    The plane perpendicular to a segment which starts and ends at neighboring support points is constructed. 
    Then, the point of three surfaces intersection is determined by the numerical method  (curveOne.surface, curveTwo.surface and plane).\n
      In specific case the intersection curve can circumscribe the surface boundary (buildType==cbt_Boundary), then the curves are equal and lie on the same surface.\n
      In rare cases the second rule of the intersection curve can't be satisfied precisely, but it is satisfied with a certain error.
    In these cases parameter 'buildType' is equal to cbt_Tolerant and the intersected surfaces touch each other by an intersection curve,
    and calculation of point of curve from solution of surfaces intersection equation system is complicated due to the ambiguity of solution.\n
       All activities related to the construction of the intersection curve, ensuring its accuracy and parameter definition buildType,
    executed before the constructor of the curve.\n
      'spaceCurve' curve can be absent, it is calculated if necessary, it knows nothing about surfaces and
    generally is approximating. It is used when accuracy isn't important.\n
      Intersection curve of surfaces is used to connect the surfaces or to describe connection edge of two faces.
    Intersection curve can describe different types of connection edges of two faces: \n
    an ordinary edge - different surfaces, different two-dimensional curves,\n
    a seam edge - single surface, different two-dimensional curves,\n
    Parting edge - duplicated surfaces, duplicated two-dimensional curves,\n
    a boundary edge - same surface, same two-dimensional curve,\n
    edge-pole - same surface, duplicated two-dimensional curves.\n
      if two-dimensional curves of curveOne and curveTwo curves on surface are contours, then count of segments in them has to be the same. \~
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbSurfaceIntersectionCurve : public MbCurve3D {
private :

  //------------------------------------------------------------------------------
  /** \brief \ru Кэш для пространственной аппроксимационной кривой.
             \en Cache for the spatial approximating curve. \~
    \details \ru Кэш служит для потокобезопасности операций с аппроксимационной кривой.
             \en The cache is used for thread-safety of operations with the approximating curve. \n \~
  */
  // ---
  struct SpaceCurveAuxiliaryData : public AuxiliaryData
  {
    SPtr<MbCurve3D> spaceCurve; ///< \ru Пространственная аппроксимационная кривая. \en The spatial approximating curve. \~
    SpaceCurveAuxiliaryData() : spaceCurve( nullptr ) {}
    SpaceCurveAuxiliaryData( const SpaceCurveAuxiliaryData & c ) : AuxiliaryData {}, spaceCurve( nullptr ) {
      if ( c.spaceCurve != nullptr ) {
        spaceCurve = static_cast<MbCurve3D *>( &c.spaceCurve->Duplicate() );
      }
    }
    ~SpaceCurveAuxiliaryData() {}
  };

  //------------------------------------------------------------------------------
  /** \brief \ru Класс кэша пространственной аппроксимационной кривой.
             \en Class for a cache of the spatial approximating curve. \~
    \details \ru Класс кэша пространственной аппроксимационной кривой.
             \en Class for a cache of the spatial approximating curve. \~
  */
  // ---
  class SpaceCurveCacheManager : public CacheManager<SpaceCurveAuxiliaryData> {
    std::set< SPtr<MbCurve3D> > curvesCache;
  public:

#ifdef CACHE_DELETE_LOCK
    SpaceCurveCacheManager( bool createLock = false ) : CacheManager<SpaceCurveAuxiliaryData>( createLock ) {}
#else
    SpaceCurveCacheManager( bool createLock = true ) : CacheManager<SpaceCurveAuxiliaryData>( createLock ) {}
#endif
    SpaceCurveCacheManager( const CacheManager & m ) : CacheManager<SpaceCurveAuxiliaryData> ( m ) {}

    bool ResetCacheData() final { bool res = CacheManager<SpaceCurveAuxiliaryData>::ResetCacheData(); curvesCache.clear(); return res; }

    void PreserveSpaceCurve( SPtr<MbCurve3D> & curve ) {
      curvesCache.insert( curve );
    }
    void SetSpaceCurve( MbCurve3D * curve ) {
      if ( curve != nullptr ) {
        {
          ScopedLock cacheLock( GetLock() );
          c3d::SpaceCurveSPtr longTermCurve = LongTerm()->spaceCurve;
          if ( longTermCurve != nullptr && cacheLock.IsLocked() )
            PreserveSpaceCurve( longTermCurve );
          LongTerm()->spaceCurve = curve;
        }
        Reset();
      }
    }
  };

  mutable SpaceCurveCacheManager spaceCurveCache; ///< \ru Кэш пространственной аппроксимационной кривой. \en Cache of a spatial approximating curve. \~

  MbSurfaceCurve    curveOne;  ///< \ru Кривая на первой поверхности. \en Curve on the first surface. 
  MbSurfaceCurve    curveTwo;  ///< \ru Кривая на второй поверхности. \en Curve on the second surface. 
  MbeCurveBuildType buildType; ///< \ru Тип кривой по построению. \en A curve type by construction. 

  mutable MbeCurveGlueType  glueType; ///< \ru Тип кривой по топологии. \en A curve type by topology. 
  mutable double            tolerance;        ///< \ru Погрешность построения кривой. \en The tolerance of curve construction. \~
  mutable MbCube            cube;             ///< \ru Габаритный куб кривой. \en Bounding box of a curve. \~
  mutable double            metricLength;     ///< \ru Метрическая длина кривой. \en Metric length of a curve. \~
  mutable double            lengthEvaluation; ///< \ru Оценочная длина кривой. \en Estimated length of a curve. \~
  mutable double            curveRadius;      ///< \ru Радиус кривой, если она является дугой окружности в пространстве. \en The radius of the curve, if the curve is a spatial arc.
  mutable c3d::DoublePair   radiusAccuracy;   ///< \ru Точности вычисления curveRadius. \en The precisions for the calculation of curveRadius.
 
#ifdef C3D_SIGNAL_ENABLED
  mutable atomic_bool       inChange;         ///< \ru Указывает на нахождение в процессе изменений. \en Indicates to being in the process of changes.
#endif

  //------------------------------------------------------------------------------
  /** \brief \ru Вспомогательные данные. 
             \en Auxiliary data. \~
    \details \ru Вспомогательные данные служат для ускорения работы объекта.
             \en Auxiliary data are used for fast calculations. \n \~
  */
  // ---
  class MbIntersectionCurveAuxiliaryData : public AuxiliaryData {
  public:
    double        t;    ///< \ru Модифицированный параметр. \en Modified parameter.
    bool          res;  ///< \ru Результат итерационного процесса. \en The result of iterative intersection.
    MbCartPoint   uv1;  ///< \ru Точка. \en Point.
    MbCartPoint   uv2;  ///< \ru Точка. \en Point.
    MbCartPoint3D pnt;  ///< \ru Точка. \en Point.
    MbVector3D    fder; ///< \ru Первая производная. \en First derivative.
    MbVector3D    sder; ///< \ru Вторая производная. \en Second derivative.
    MbVector3D    tder; ///< \ru Третья производная. \en Third derivative.

    MbIntersectionCurveAuxiliaryData();
    MbIntersectionCurveAuxiliaryData( const MbIntersectionCurveAuxiliaryData & );
    virtual ~MbIntersectionCurveAuxiliaryData() {}

    void Init();
    void Init( const MbIntersectionCurveAuxiliaryData & );
    void Move( const MbVector3D & );
  };
  
  mutable CacheManager<MbIntersectionCurveAuxiliaryData> cache;

public :
  /** \brief \ru Конструктор по поверхностям и двумерным кривым.
             \en Constructor by surfaces and two-dimensional curves. \~
    \details \ru Конструктор кривой пересечения по поверхностям и двумерным кривым. \n
             \en Constructor of intersection curve by surfaces and two-dimensional curves. \n \~
    \param[in] surf1 - \ru Первая поверхность.
                       \en The first surface. \~
    \param[in] curve1 - \ru Первая двумерная кривая.
                        \en The first two-dimensional curve. \~
    \param[in] surf2 - \ru Вторая поверхность.
                       \en The second surface. \~
    \param[in] curve2 - \ru Вторая двумерная кривая.
                        \en The second two-dimensional curve. \~
    \param[in] buildType - \ru Тип кривой пересечения по построению.
                           \en An intersection curve type by construction. \~
    \param[in] sameOne - \ru Использовать оригинал первой двумерной кривой.
                         \en Use the original of the first two-dimensional curve. \~
    \param[in] sameTwo - \ru Использовать оригинал второй двумерной кривой.
                         \en Use the original of the second two-dimensional curve. \~
    \param[in,out] iReg - \ru Регистратор дублирования.
                          \en Registrator of duplication. \~
  */ 
  MbSurfaceIntersectionCurve( const MbSurface & surf1, const MbCurve & curve1,
                              const MbSurface & surf2, const MbCurve & curve2,
                              MbeCurveBuildType buildType, bool sameOne, bool sameTwo,
                              MbRegDuplicate * iReg = nullptr );
  /** \brief \ru Конструктор по поверхностям и двумерным кривым.
             \en Constructor by surfaces and two-dimensional curves. \~
    \details \ru Конструктор кривой пересечения по поверхностям и двумерным кривым. \n
             \en Constructor of intersection curve by surfaces and two-dimensional curves. \n \~
    \param[in] surf1 - \ru Первая поверхность.
                       \en The first surface. \~
    \param[in] curve1 - \ru Первая двумерная кривая.
                        \en The first two-dimensional curve. \~
    \param[in] surf2 - \ru Вторая поверхность.
                       \en The second surface. \~
    \param[in] curve2 - \ru Вторая двумерная кривая.
                        \en The second two-dimensional curve. \~
    \param[in] buildType - \ru Тип кривой пересечения по построению.
                           \en An intersection curve type by construction. \~
    \param[in] sameOne - \ru Использовать оригинал первой двумерной кривой.
                         \en Use the original of the first two-dimensional curve. \~
    \param[in] sameTwo - \ru Использовать оригинал второй двумерной кривой.
                         \en Use the original of the second two-dimensional curve. \~
    \param[in] saveParLenAndLaw - \ru Сохранять параметрическую длину и закон для поверхностных кривых.
                                  \en Save parametric length and law for surface curves. \~
    \param[in,out] iReg - \ru Регистратор дублирования.
                          \en Registrator of duplication. \~
  */ 
  MbSurfaceIntersectionCurve( const MbSurface & surf1, const MbCurve & curve1,
                              const MbSurface & surf2, const MbCurve & curve2,
                              MbeCurveBuildType buildType, bool sameOne, bool sameTwo, 
                              bool saveParLenAndLaw, MbRegDuplicate * iReg = nullptr );
  /** \brief \ru Конструктор по поверхностям и двумерным точкам.
             \en Constructor by surfaces and two-dimensional points. \~
    \details \ru Конструктор кривой пересечения по поверхностям и двумерным точкам. \n
             \en Constructor of an intersection curve by surfaces and two-dimensional points. \n \~
    \param[in] surf1 - \ru Первая поверхность.
                       \en The first surface. \~
    \param[in] surf1p0 - \ru Начальная точка на поверхности.
                         \en Start point on the surface. \~
    \param[in] surf1p1 - \ru Конечная точка на поверхности.
                         \en End point on the surface. \~
    \param[in] surf2 - \ru Вторая поверхность.
                       \en The second surface. \~
    \param[in] surf2p0 - \ru Начальная точка на поверхности.
                         \en Start point on the surface. \~
    \param[in] surf2p1 - \ru Конечная точка на поверхности.
                         \en End point on the surface. \~
  */ 
  MbSurfaceIntersectionCurve( const MbSurface & surf1, const MbCartPoint & surf1p0, const MbCartPoint & surf1p1,
                              const MbSurface & surf2, const MbCartPoint & surf2p0, const MbCartPoint & surf2p1 );
  /** \brief \ru Конструктор по поверхностям, двумерным кривой и точкам.
             \en Constructor by surfaces, two-dimensional curves and points. \~
    \details \ru Конструктор кривой пересечения по поверхностям, двумерным кривой и точкам. \n
             \en Constructor of intersection curve by surfaces, two-dimensional curves and points. \n \~
    \param[in] surf1 - \ru Первая поверхность.
                       \en The first surface. \~
    \param[in] curve1 - \ru Первая двумерная кривая.
                        \en The first two-dimensional curve. \~
    \param[in] surf2 - \ru Вторая поверхность.
                       \en The second surface. \~
    \param[in] surf2p0 - \ru Начальная точка на поверхности.
                         \en Start point on the surface. \~
    \param[in] surf2p1 - \ru Конечная точка на поверхности.
                         \en End point on the surface. \~
    \param[in] buildType - \ru Тип кривой пересечения по построению.
                           \en An intersection curve type by construction. \~
  */ 
  MbSurfaceIntersectionCurve( const MbSurface & surf1, const MbCurve & curve1,
                              const MbSurface & surf2, const MbCartPoint & surf2p0, const MbCartPoint & surf2p1, 
                              MbeCurveBuildType buildType );
  /** \brief \ru Конструктор по поверхностям, двумерным точкам и кривой.
             \en Constructor by surfaces, two-dimensional points and curve. \~
    \details \ru Конструктор кривой пересечения по поверхностям, двумерным точкам и кривой. \n
             \en Constructor of an intersection curve by surfaces, two-dimensional points and curve. \n \~
    \param[in] surf1 - \ru Первая поверхность.
                       \en The first surface. \~
    \param[in] surf1p0 - \ru Начальная точка на поверхности.
                         \en Start point on the surface. \~
    \param[in] surf1p1 - \ru Конечная точка на поверхности.
                         \en End point on the surface. \~
    \param[in] surf2 - \ru Вторая поверхность.
                       \en The second surface. \~
    \param[in] curve2 - \ru Вторая двумерная кривая.
                        \en The second two-dimensional curve. \~
    \param[in] buildType - \ru Тип кривой пересечения по построению.
                           \en An intersection curve type by construction. \~
  */ 
  MbSurfaceIntersectionCurve( const MbSurface & surf1, const MbCartPoint & surf1p0, const MbCartPoint & surf1p1,
                              const MbSurface & surf2, const MbCurve & curve2,
                              MbeCurveBuildType buildType );
  /** \brief \ru Конструктор для конвертеров по поверхностям и двумерным кривым.
             \en Constructor for converters by surfaces and two-dimensional curves. \~
    \details \ru Конструктор кривой пересечения для конвертеров по поверхностям и двумерным кривым. \n
             \en Constructor of intersection curve for converters by surfaces and two-dimensional curves. \n \~
    \param[in] surf1 - \ru Первая поверхность.
                       \en The first surface. \~
    \param[in] curve1 - \ru Первая двумерная кривая.
                        \en The first two-dimensional curve. \~
    \param[in] surf2 - \ru Вторая поверхность.
                       \en The second surface. \~
    \param[in] curve2 - \ru Вторая двумерная кривая.
                        \en The second two-dimensional curve. \~
    \param[in] spaceCurve - \ru Аппроксимация кривой пересечения (параметрические пределы должны соответствовать пределам поверхностных кривых).
                            \en Approximation of the intersection curve (parametric limits must correspond to the limits of surface curves). \~
    \param[in] buildType - \ru Тип кривой пересечения по построению.
                           \en An intersection curve type by construction. \~
    \param[in] glueType - \ru Тип кривой пересечения по топологии.
                          \en An intersection curve type by topology. \~
    \param[in] tol - \ru Неточность построения кривой пересечения.
                     \en Inaccuracy of intersection curve construction. \~
  */ 
  MbSurfaceIntersectionCurve( const MbSurface & surf1, const MbCurve & curve1,
                              const MbSurface & surf2, const MbCurve & curve2,
                              const MbCurve3D * spaceCurve, MbeCurveBuildType buildType, 
                              MbeCurveGlueType glueType, double tol ); // \ru Используется в конвертерах \en Used in converters  
protected:
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbSurfaceIntersectionCurve( const MbSurfaceIntersectionCurve &, MbRegDuplicate * );
  /// \ru Конструктор копирования двумерных кривых с теми же поверхностями для CurvesDuplicate(). \en Copy-constructor of two-dimensional curves with the same surfaces for CurvesDuplicate(). 
  explicit MbSurfaceIntersectionCurve( const MbSurfaceIntersectionCurve * );
private:
  MbSurfaceIntersectionCurve( const MbSurfaceIntersectionCurve & ); // \ru Не реализовано!!! \en Not implemented!!! 
public:
  virtual ~MbSurfaceIntersectionCurve();

public:
  /// \ru Реализация функции, инициирующей посещение объекта. \en Implementation of a function initializing a visit of an object. 
  VISITING_CLASS( MbSurfaceIntersectionCurve );

  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */

  MbeSpaceType  IsA() const override; // \ru Дать тип элемента. \en Get element type. 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  /// \ru Сделать копию кривой на тех же поверхностях. \en Create a copy of a curve on the same surfaces. 
  MbSurfaceIntersectionCurve & CurvesDuplicate() const { return *new MbSurfaceIntersectionCurve( this ); }
  bool        IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Определить, равны ли объекты. \en Determine whether the objects are equal. 
  bool        SetEqual ( const MbSpaceItem & ) override; // \ru Сделать равным. \en Make equal. 
  bool        IsSimilar( const MbSpaceItem & ) const override; // \ru Определить, являются ли объекты подобными. \en Determine whether the objects are similar. 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвинуть вдоль вектора. \en Move along a vector.
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate about an axis.
  void        AddYourGabaritTo( MbCube & r ) const override; // \ru Добавить свой габарит в куб. \en Add your own bounding box into a cube.
  void        CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.
  void        Refresh() override; // \ru Сбросить все временные данные. \en Reset all temporary data. 
#ifdef C3D_SIGNAL_ENABLED
  void        Refresh( MbeObjectChangeType ) override; // \ru Сбросить временные данные с учетом типа изменений. \en Reset temporary data based on the type of changes.
#endif
  void        PrepareIntegralData( const bool forced ) const override; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.

  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта. \en Set properties of the object. 
  void        GetBasisItems( RPArray<MbSpaceItem> & ) override; // \ru Дать базовые объекты. \en Get the basis objects. 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \} */
  // \ru Общие функции кривой. \en Common functions of curve. 

  // \ru Функции описания области определения кривой. \en Functions for description of a curve domain. 
  double      GetTMax()  const override;  // \ru Вернуть максимальное значение параметра. \en Get the maximum value of parameter. 
  double      GetTMin()  const override;  // \ru Вернуть минимальное значение параметра. \en Get the minimum value of parameter. 
  bool        IsClosed() const override;  // \ru Проверить замкнутость кривой. \en Check for curve closedness. 
  double      GetPeriod() const override; // \ru Вернуть период периодической кривой. \en Get period of a periodic curve.
  bool        IsPeriodic() const override; // \ru Проверить периодичность кривой. \en Check for curve periodic.
  // \ru Функции кривой для работы в области определения кривой. \en Functions of curve for working at curve domain. 
  void        PointOn  ( double & t, MbCartPoint3D & ) const override; // \ru Вычислить точку на кривой. \en Calculate a point on the curve. 
  void        FirstDer ( double & t, MbVector3D & )    const override; // \ru Вычислить первую производную. \en Calculate the first derivative. 
  void        SecondDer( double & t, MbVector3D & )    const override; // \ru Вычислить вторую производную. \en Calculate the second derivative. 
  void        ThirdDer ( double & t, MbVector3D & )    const override; // \ru Вычислить третью производную по t. \en Calculate the third derivative by t. 
  void        Tangent  ( double & t, MbVector3D & )    const override; // \ru Вычислить тангенциальный вектор (нормализованный). \en Calculate tangential vector (normalized).
  // \ru Функции кривой для работы внутри и вне области определения кривой. \en Functions of curve for working inside and outside of the curve domain. 
  void       _PointOn  ( double t, MbCartPoint3D & )  const override; // \ru Вычислить точку на расширенной кривой. \en Calculate a point on the extended curve. 
  void       _FirstDer ( double t, MbVector3D & )     const override; // \ru Вычислить первую производную. \en Calculate the first derivative. 
  void       _SecondDer( double t, MbVector3D & )     const override; // \ru Вычислить вторую производную. \en Calculate the second derivative. 
  void       _ThirdDer ( double t, MbVector3D & )     const override; // \ru Вычислить третью производную по t. \en Calculate the third derivative by t. 
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void        Explore  ( double & t, bool ext,
                                 MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const override;
  // \ru Вычислить двумерные значения точки и производных для поверхностной кривой (0 - первая, 1 -  вторая).
  // \en Calculate the two-dimensional values of the point and derivatives for the surface curve (0 - first, 1 - second). \~
          bool        SurfaceCurveExplore( size_t ind, double  t, MbCartPoint & pnt, 
                                           MbVector * fir, MbVector * sec, MbVector * thir ) const;

  /// \ru Выдать подложку или себя. \en Get a substrate or itself. 
  MbCurve3D & SetSubstrate() override;

  // \ru Функции приближённого быстрого вычисления точки и производных на кривой. \en Functions of approximate fast calculation of point and derivatives on the curve. 
  void        FastApproxExplore( double & t, MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec ) const override;

  void        Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление. \en Change the direction.

  // \ru Функции движения по кривой. \en Functions of moving along the curve. 
  // \ru Вычислить шаг параметра по величине прогиба кривой. \en Calculate step of parameter by value of sag of curve. 
  double      Step         ( double t, double sag ) const override;
  // \ru Вычислить шаг параметра по углу отклонения касательной. \en Calculate step of parameter by angle of deviation of tangent. 
  double      DeviationStep( double t, double angle ) const override;
  // \ru Вычисление шага параметра по длине. \en Calculation of parameter step by the given length. 
  double      MetricStep   ( double t, double length ) const override;
          // \ru Возможен ли разрыв длины первой производной? \en Is it possible to break the length of the first derivative?
          bool        CanDerivateJump() const;

  // \ru Преобразовать в NURBS кривую. \en Transform to a NURBS-curve. 
  MbNurbs3D * NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;

  // \ru Построить усеченную кривую c учетом заданной трехмерной точности. \en Construct a trimmed curve with the given three-dimensional accuracy.
  MbCurve3D * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyzEps, bool saveParamLenAndLaw ) const override;
  using MbCurve3D::Trimmed; // \ru Метод базового класса MbCurve3D без учета трехмерной точности. Точность по умолчанию - MbDimAccuracy::threeDimAcc.  \en Method of MbCurve3D base class without three-dimensional accuracy. The default accuracy is MbDimAccuracy::threeDimAcc.

          /// \ru Создать усеченную кривую на тех же поверхностях. \en Create a trimmed curve on the same surfaces. 
          MbSurfaceIntersectionCurve * TrimmedIntersection( double t1, double t2, int sense, bool saveParamLenAndLaw = false  ) const;

  double      GetMetricLength() const override; // \ru Вычислить метрическую длину кривой. \en Calculate the metric length of a curve. 
  double      GetLengthEvaluation() const override; // \ru Оценить метрическую длину кривой. \en Estimate the metric length of a curve. 
          /**\ru Скопировать из копии готовые метрические оценки, которые в оригинале не были рассчитаны.
             \en Copy the finished metric estimations from duplicate which weren't calculated in the original. \~
            \warning \ru Внимание: для скорости проверка идентичности оригинала и копии не выполняется!
                     \en Attention: for speed purposes the check of equality of the original and the copy isn't performed! \~
          */
          bool        CopyReadyMutable( const MbSurfaceIntersectionCurve & s );
  double      CalculateLength( double t1, double t2 ) const override; // \ru Вычислить метрическую длину. \en Calculate the metric length. 
  double      GetRadius( double accuracy = METRIC_REGION ) const override; // \ru Дать физический радиус объекта или ноль, если это невозможно. \en Get the physical radius of the object or null if it impossible.
  bool        GetCircleAxis  ( MbAxis3D & ) const override; // \ru Дать ось кривой. \en Get the curve axis. 
  void        GetCentre( MbCartPoint3D & ) const override; // \ru Вычислить центр кривой. \en Calculate the center of a curve. 
  void        GetWeightCentre( MbCartPoint3D & ) const override; // \ru Вычислить центр тяжести кривой. \en Calculate the center of gravity of a curve. 
  // \ru Вычислить центр тяжести кривой. \en Calculate the center of gravity of a curve. 
          void        CalculateWeightCentre( MbCartPoint3D & ) const;

  void        CalculateGabarit( MbCube & c ) const override; // \ru Вычислить габарит кривой. \en Calculate bounding box of a curve. 
          // \ru Получить габарит кривой. \en Get bounding box of curve. 
  const   MbCube &    GetGabarit() const;
          // \ru Сбросить габаритный куб. \en Reset bounding box. 
          void        SetDirtyGabarit() const;

  bool        DistanceAlong( double & t, double len, int curveDir, double eps = Math::metricPrecision,
                                     VERSION version = Math::DefaultMathVersion() ) const override; // \ru Сдвинуть параметр t на расстояние len по направлению. \en Translate parameter 't' on the distance 'len' by the direction.
  void        CalculatePolygon( const MbStepData & stepData, MbPolygon3D &polygon ) const override; // \ru Рассчитать полигон. \en Calculate a polygon.

  // \ru Построить плоскую проекцию некоторой части пространственной кривой. \en Construct a planar projection of a piece of a space curve. 
  MbCurve *   GetMap( const MbMatrix3D & into, MbRect1D * pRegion = nullptr,
                              VERSION version = Math::DefaultMathVersion(), bool * coincParams = nullptr ) const override;
  // \ru Дать проекцию ребра на плоскость. \en Get the edge projection onto plane. 
  MbCurve *   GetProjection( const MbPlacement3D & place, VERSION version ) const override; 

  // \ru Определить количество разбиений для прохода в операциях. \en Define the number of splittings for one passage in operations. 
  size_t      GetCount() const override;

  bool        IsStraight( bool ignoreParams = false ) const override; // \ru Определить, является ли линия прямолинейной. \en Determine whether the line is straight. 
  void        ChangeCarrier     ( const MbSpaceItem & item, MbSpaceItem & init ) override; // \ru Изменить носитель. \en Change the carrier. 
  bool        ChangeCarrierBorne( const MbSpaceItem & item, MbSpaceItem & init, const MbMatrix & matr ) override; // \ru Изменение носимые элементы. \en Change a carrier elements. 
  bool        IsPlanar( double accuracy = METRIC_EPSILON ) const override; // \ru Определить, является ли кривая плоской. \en Determine whether the curve is planar. 
  bool        IsSmoothConnected( double angleEps, bool ignoreLimits = false ) const override; // \ru Определить, являются ли стыки контура\кривой гладкими. \en Determine whether the joints of contour\curve are smooth. 

  double      DistanceToPlace( const MbPlacement3D & place, double & t0, double & angle ) const override; // \ru Вычислить ближайшую точку кривой к плейсменту. \en Calculate the curve point nearest to a placement. 
  // \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская (после использования вызывать DeleteItem на двумерную кривую). \en Get the planar curve and placement if the spatial curve is planar (call DeleteItem for two-dimensional curve after using ). 
  bool        GetPlaneCurve( MbCurve *& curve2d, MbPlacement3D & place3d, bool saveParams, PlanarCheckParams params = PlanarCheckParams() ) const override;
  // \ru Заполнить плейсмент, если кривая плоская. \en Fill the placement if the curve is planar. 
  bool        GetPlacement( MbPlacement3D & place, PlanarCheckParams params = PlanarCheckParams() ) const override;
  /**\ru Дать поверхностную кривую, если пространственная кривая поверхностная. После использования вызывать
         DeleteItem на аргументы. В некоторых случаях переданная поверхность surface может быть
         использована для устранения неопределённости выбора кривой (например, при стыке двух
         одинаковых по IsSame или IsSimilar поверхностей).
     \en Get a surface curve if a spatial curve lies on a surface. Call 'DeleteItem' for arguments after use.
         In some cases the input 'surface' argument may be can be used to eliminate uncertainty in the choice
         of a curve (e.g. curves on a junction of surfaces which are same by IsSame or IsSimilar checks).        
  */
  bool        GetSurfaceCurve( MbCurve *& curve2d, MbSurface *& surface, VERSION version = Math::DefaultMathVersion() ) const override;

  double      GetParamToUnit() const override; // \ru Дать приращение параметра, осреднённо соответствующее единичной длине в пространстве. \en Get parameter increment which averagingly corresponds to the unit length in space. 
  double      GetParamToUnit( double t ) const override; // \ru Дать приращение параметра, соответствующее единичной длине в пространстве. \en Get parameter increment which corresponds to the unit length in space. 

  // \ru Определить, является ли объект смещением. \en Determine whether the object is a translation. 
  bool        IsShift( const MbSpaceItem &, MbVector3D &, bool & isSame, double accuracy = LENGTH_EPSILON ) const override;
  // \ru Определить, подобные ли кривые для объединения (слива). \en Determine whether the curves for union (joining) are similar. 
  bool        IsSimilarToCurve( const MbCurve3D & other, double precision = METRIC_PRECISION ) const override;
  // \ru Определить, являются ли объекты идентичными в пространстве? \en Determine whether the objects are equal in space. 
  bool        IsSpaceSame( const MbSpaceItem & item, double eps = METRIC_REGION ) const override;

          /// \ru Получить тип кривой по топологии. \en Get a curve type by topology. 
          MbeCurveGlueType  GetGlueType() const { return glueType; }
          /// \ru Установить тип кривой по топологии. \en Set a curve type by topology. 
          void        SetGlueType( MbeCurveGlueType type ) { glueType = type; }
          /// \ru Установить тип кривой по топологии. \en Set a curve type by topology. 
          void        SetPoleGlueType() const;

          /// \ru Получить тип кривой по построению. \en Get a curve type by construction. 
          MbeCurveBuildType GetBuildType() const { return buildType; }
          /// \ru Установить тип кривой по построению. \en Set a curve type by construction. 
          void        SetBuildType( MbeCurveBuildType type ) { buildType = type; }

  // \ru Проверка параметра. \en Check parameter. 
  inline  void        CheckParam       ( double & t ) const;
  /// \ru Вычислить точки на пересекаемых поверхностях. \en Calculate points on intersecting surfaces. 
          bool        PointOn( double t, MbCartPoint & r1, MbCartPoint & r2 ) const;
  /// \ru Вычислить точку. \en Calculate a point. 
  inline  void        GetPointOn       ( double & t, MbCartPoint3D & ) const;
  /// \ru Вычислить первую производную. \en Calculate the first derivative. 
  inline  void        GetFirstDer      ( double & t, MbVector3D & ) const;

  /// \ru Найти все особые точки функции кривизны кривой и вычислить значение потенциальной энергии кривой.
  /// \en Find all the special points of the curvature function of the curve and calculate potential energy of the curve. \~
  void    GetCurvatureSpecialPoints( std::vector<c3d::DoublePair> & points, double * energy = nullptr ) const override;

          /** \brief \ru Уточнить кривую общего случая пересечения.
                     \en Refine a curve of intersection of common case. \~
            \details \ru Уточнить кривую общего случая пересечения с флагом cbt_Specific (для других флагов ничего не выполняется).
            При флаге кривой cbt_Specific для параметра t определяются 
            двумерные точки на пересекаемых поверхностях и вставляются в сплайны curveOne.curve и curveTwo.curve.
                     \en Refine common case intersection curve with cbt_Specific flag (for other flags is performed nothing).
            If a curve flag is equal to cbt_Specific, then 
            the two-dimensional points on intersected surfaces are determined and are inserted into curveOne.curve and curveTwo.curve splines for 't' parameter. \~ 
            \param[in] t   - \ru Параметр точки уточнения,
                             \en Parameter of point to refine, \~
            \param[in] pointsPair - \ru Параметрические точки уточнения, полученные в функции PointOn( t, pointsPair->first, pointsPair->second),
                                    \en Parametric points of refinement, obtained by the function of PointOn (t, points Pair-> first, pointsPair-> second), \~
            \param[in] tCheck - \ru Контрольный параметр точки уточнения, если он не равен t, то вставляемые точки не сдвинутся так, чтобы параметр t стал равен tCheck.
                                \en Control parameter of a point to refine, if it isn't equal to 't', then the inserted points won't move so that parameter 't' became equal to 'tCheck'. \~
            \return \ru Возвращает true, если произошло присоединение кривой.
                    \en Returns true if there was a curve joining. \~
            \warning \ru Для внутреннего использования.
                     \en For internal use only. \~      
          */
          bool        InsertPoints     ( double & t, const std::pair<MbCartPoint,MbCartPoint> * pointsPair, double & tCheck ); // \ru Вставить точку и выдать её параметр. \en Insert point and get its parameter. 

          /** \brief \ru Разрезать кривую на две части.
                     \en Cutaway a curve into two pieces. \~
            \details \ru Разрезать кривую на две части точкой кривой с заданным параметром.
                     \en Cutaway a curve into two pieces by a point of the curve with a given parameter. \~
            \param[in] t   - \ru Параметр точки разбиения,
                             \en Parameter of a point to split, \~
            \param[in] beg - \ru Кривая сохранит начальную половину (true) или кривая сохранит конечную половину (false),
                             \en Curve will keep a beginning piece (true) or curve will keep an end piece (false) \~
            \param[in] surface - \ru Для толерантной кривой требуется указать поверхность, к кривой которой относится параметр
                                 \en For tolerant curve it is required to specify a surface which contain a curve a parameter belongs to \~
            \return \ru Возвращает отрезанную часть кривой.
                    \en Returns the cut piece of curve. \~
          */
          MbSurfaceIntersectionCurve * BreakCurve( double t, bool beg, const MbSurface * surface ); ///< \ru Разбить кривую на две. \en Split curve into two. 

          /** \brief \ru Усечь кривую.
                     \en Trim a curve. \~
            \details \ru Усечь кривую по заданным параметрам. \n
                     \en Trim a curve by the given parameters. \n \~ 
            \param[in] t1 - \ru Начальный параметр усечения,
                            \en Beginning parameter of trimming \~
            \param[in] t2 - \ru Конечный  параметр усечения,
                            \en End parameter of trimming \~
            \param[in] surface - \ru Для толерантной кривой требуется указать поверхность, к кривой которой относятся параметры усечения
                                 \en For tolerant curve it is required to specify a surface which contain a curve a trimming parameters belong to \~
            \return \ru Возвращает true, если произошло усечение.
                    \en Returns true if there was a curve trimming. \~
          */
          bool        TruncateCurve( double t1, double t2, const MbSurface * surface );

          /** \brief \ru Присоединить к данной кривой другую кривую.
                     \en Join this curve and another curve. \~
            \details \ru Сделать из двух кривых пересечения одну - вызывается для объединения двух ребер из функции MbCurveEdge::MergeEdges.
              Объединяемые кривые должны описывать пересечение одних и тех же поверхностей.
              Объединяются только гладко стыкующиеся кривые. 
              Должно быть точное совпадение кривых, поверхностей этих кривых и касательных в месте склеивания.
              После присоединения другую кривую можно удалить. \n
                     \en Make a single curve curve from two intersection curves - is called for union of two edges from MbCurveEdge::MergeEdges function.
              United curves should represent the intersection of the same surfaces.
              Only smoothly joining curves are united. 
              There must be an exact coincidence of curves, surfaces of these curves and tangents at joining place.
              Another curve can be deleted after joining. \n \~ 
            \param[in] addCurve  - \ru Добавляемая кривая (другая кривая),
                                   \en Curve to join (another curve), \~
            \param[in] toBegin   - \ru Пристыковываем к началу this (true) или пристыковываем к концу this (false),
                                   \en Join to the beginning of this (true) or join to the end of this (false) \~
            \param[in] fromBegin - \ru Пристыковываем начало addCurve (true) или пристыковываем конец addCurve (false),
                                   \en Join the beginning of 'addCurve' (true) or join the end of 'addCurve' (false), \~
            \param[in] allowCntr - \ru Флаг, разрешающий заменять curveOne.curve и curveTwo.curve двумерными контурами. \n
                                   \en Flag, which allows to replace curveOne.curve and curveTwo.curve by two-dimensional contours. \n \~  
            \param[in] version   - \ru Версия математики. \n
                                   \en The version of mathematics. \n \~  
            \param[in] insertInterimPoints - \ru Флаг, разрешающий вставлять дополнительные точки в кривые типа cbt_Specific. \n
                                             \en Flag, which allows to insert interim points into curve of type cbt_Specific. \n \~
            \param[in] eps - \ru Точность сравнения точек и классификации расположения точек относительно кривой. \n
                           - \en The precision of points comparison and classification of point location relative to the curve. \n \~
            \return \ru Возвращает true, если произошло присоединение кривой.
                    \en Returns true if there was a curve joining. \~
          */
          bool        MergeCurves( const MbSurfaceIntersectionCurve & addCurve, bool toBegin, bool fromBegin, bool allowCntr,
                                   const VERSION version, bool insertInterimPoints = true, double eps = Math::paramNear );

          /** \brief \ru Продлить кривую.
                     \en Extend curve. \~
            \details \ru Продлить кривую до точки с заданным параметром. \n
                     \en Extend curve to a point with a given parameter. \n \~ 
            \param[in] t   - \ru Параметр, до точки которого продлить кривую,
                             \en Parameter the curve extends to point of \~
            \param[in] beg - \ru Продлить начало кривой (true) или продлить конец кривой (false),
                             \en Extend the beginning of the curve (true) or extend the end of the curve (false) \~
            \param[in] version - \ru Версия математики. \n
                                 \en The version of mathematics. \n \~
            \param[in] eps - \ru Точность построений.\n
                           - \en Build precision.\n \~
            \return \ru Возвращает true, если произошло продление.
                    \en Returns true if there was an extension. \~
          */
          bool        ProlongCurve( double & t, bool beg, double sag, const VERSION version, double eps = Math::paramNear );
          /// \ru Согласовать параметрическую длину двумерных кривых. \en Match parametric length of two-dimensional curves. 
          void        Normalize( bool saveParLenAndLaw = false ); 

          /// \ru Выбрать кривую шва по ориентации грани и ориентации двумерной кривой. \en Select a seam curve by face orientation and orientation of two-dimensional curve. 
          const MbCurve * ChooseCurve( const MbSurface & surf, bool faceSense, bool curveSense ) const;
          /// \ru Выбрать кривую шва по ориентации грани и ориентации двумерной кривой. \en Select a seam curve by face orientation and orientation of two-dimensional curve. 
                MbCurve * ChooseCurve_( const MbSurface & surf, bool faceSense, bool curveSense );

          /** \brief \ru Вычислить вектор сдвига двумерной кривой шва.
                     \en Calculate a shift vector of two-dimensional curve of seam. \~
            \details \ru Вычислить вектор сдвига двумерной кривой шва отрицательно ориентированной относительно заданной. \n
                     \en Calculate a shift vector of two-dimensional curve of seam negatively oriented with respect to the given. \n \~ 
          */
          bool        GetMoveVector( const MbSurface & surf, bool faceSense, bool curveSense, MbVector & to ) const;
          /// \ru Заменить двумерную кривую. \en Replace the two-dimensional curve. 
          bool        ChangeCurve  ( const MbCurve * oldCrv, MbCurve & newCrv );
          /// \ru Заменить поверхность. \en Replace surface. 
          bool        ChangeSurface( const MbSurface & oldSrf, MbSurface & newSrf, bool faceSense, bool curveSense );
          /// \ru Заменить поверхности. \en Replace surfaces. 
          bool        ChangeSurfaces( const MbSurface & surf1, const MbSurface & surf2 );
          /// \ru Заменить поверхности на такие же (IsSame) с другой кривой. \en Replace surfaces with the same ones (IsSame) with a different curve. 
          bool        ReplaceSameSurfaces( const MbSurfaceIntersectionCurve & );

          /// \ru Установить область изменения параметра. \en Set range of parameter.
          bool        SetLimitParam( double newTMin, double newTMax );
          /// \ru Поменять местами поверхностные кривые. \en Swap two-dimensional curves and surfaces. 
          bool        SwapSurfaceCurves();

          /// \ru Вычислить векторное произведение нормалей поверхностей. \en Calculate vector-product of normals of surfaces. 
          bool        Direction    ( double & t, MbVector3D & tau, double eps = Math::paramNear ) const;
          /// \ru Вычислить векторное произведение нормалей поверхностей вблизи линии. \en Calculate vector-product of normals of surfaces near to the line. 
          bool        NearDirection( double & t, const MbSurface & surfOne, const MbSurface & surfTwo, MbVector3D & tau, double delta,
                                     MbCartPoint & point1,  MbCartPoint & point2,
                                     MbVector3D  & normal1, MbVector3D  & normal2 ) const;
          /// \ru Вычислить тангенциальный и трансверсальный векторы, касательные к поверхностям. \en Calculate tangential and transversal vectors tangent to surfaces. 
          bool        TransversalReper( double & t, MbVector3D & tau0, MbVector3D & tau1, MbVector3D & tau2 ) const;

          /// \ru Сделать равными двумерные кривые. \en Make two-dimensional curves equal. 
          bool        SetCurveEqual( const MbSpaceItem & init );
          /// \ru Определить, являются ли объекты подобными. \en Determine whether the objects are similar. 
          bool        IsCurveEqual ( const MbSpaceItem & init ) const;

          /// \ru Получить поверхностную кривую по номеру (0 - первая, 1 -  вторая). \en Get surface curve by index (0 - first, 1 - second). 
  const   MbSurfaceCurve & GetCurve( ptrdiff_t i ) const { return i ? curveTwo : curveOne; }
          /// \ru Получить поверхностную кривую по номеру (0 - первая, 1 -  вторая). \en Get surface curve by index (0 - first, 1 - second). 
          MbSurfaceCurve & SetCurve( ptrdiff_t i )       { return i ? curveTwo : curveOne; }
          /// \ru Получить первую поверхностную кривую. \en Get the first surface curve. 
  const   MbSurfaceCurve & GetCurveOne() const { return curveOne; }
          /// \ru Получить первую поверхностную кривую. \en Get the first surface curve. 
          MbSurfaceCurve & SetCurveOne()       { return curveOne; }
          /// \ru Получить вторую поверхностную кривую. \en Get the second surface curve. 
  const   MbSurfaceCurve & GetCurveTwo() const { return curveTwo; }
          /// \ru Получить вторую поверхностную кривую. \en Get the second surface curve. 
          MbSurfaceCurve & SetCurveTwo()       { return curveTwo; }

          /// \ru Получить двумерную кривую первой поверхностной кривой. \en Get two-dimensional curve of the first surface curve. 
  const   MbCurve &   GetCurveOneCurve() const { return curveOne.GetCurve(); }
          /// \ru Получить двумерную кривую первой поверхностной кривой. \en Get two-dimensional curve of the first surface curve. 
          MbCurve &   SetCurveOneCurve()       { return curveOne.SetCurve(); }
          /// \ru Получить двумерную кривую второй поверхностной кривой. \en Get two-dimensional curve of the second surface curve. 
  const   MbCurve &   GetCurveTwoCurve() const { return curveTwo.GetCurve(); }
          /// \ru Получить двумерную кривую второй поверхностной кривой. \en Get two-dimensional curve of the second surface curve. 
          MbCurve &   SetCurveTwoCurve()       { return curveTwo.SetCurve(); }

          /// \ru Получить поверхность первой поверхностной кривой. \en Get surface of the first surface curve. 
  const   MbSurface & GetCurveOneSurface() const { return curveOne.GetSurface(); }
          /// \ru Получить поверхность первой поверхностной кривой. \en Get surface of the first surface curve. 
          MbSurface & SetCurveOneSurface()       { return curveOne.SetSurface(); }
          /// \ru Получить поверхность второй поверхностной кривой. \en Get surface of the second surface curve. 
  const   MbSurface & GetCurveTwoSurface() const { return curveTwo.GetSurface(); }
          /// \ru Получить поверхность второй поверхностной кривой. \en Get surface of the second surface curve. 
          MbSurface & SetCurveTwoSurface()       { return curveTwo.SetSurface(); }
          /// \ru Получить поверхность по номеру (0 - из первой, 1 - из второй поверхностной кривой). \en Get surface by index (0 - from first surface curve, 1 - from second one). 
  const   MbSurface & GetSurface( ptrdiff_t i ) const { return i ? curveTwo.GetSurface() : curveOne.GetSurface(); }
          /// \ru Получить поверхность по номеру (0 - из первой, 1 - из второй поверхностной кривой). \en Get surface by index (0 - from first surface curve, 1 - from second one). 
          MbSurface & SetSurface( ptrdiff_t i )       { return i ? curveTwo.SetSurface() : curveOne.SetSurface(); }
  
  /// \ru Дать указатель на первую кривую на поверхности. \en Get a pointer to the first curve on surface. 
  const   MbSurfaceCurve * GetSCurveOne() const { return &curveOne; }
  /// \ru Дать указатель на вторую кривую на поверхности. \en Get a pointer to the second curve on surface. 
  const   MbSurfaceCurve * GetSCurveTwo() const { return &curveTwo; }
  /// \ru Дать указатель на двумерную кривую. \en Get a pointer to the two-dimensional curve. 
  const   MbCurve *   GetPCurveOne () const { return &( curveOne.GetCurve() ); }
  /// \ru Дать указатель на двумерную кривую. \en Get a pointer to the two-dimensional curve. 
  const   MbCurve *   GetPCurveTwo () const { return &( curveTwo.GetCurve() ); }
  /// \ru Дать указатель на поверхность первой кривой. \en Get a pointer to the surface of the first curve. 
  const   MbSurface * GetSurfaceOne() const { return &( curveOne.GetSurface() ); }
  /// \ru Дать указатель на поверхность второй кривой. \en Get a pointer to the surface of the second curve. 
  const   MbSurface * GetSurfaceTwo() const { return &( curveTwo.GetSurface() ); }

  /// \ru Получить одну из поверхностей, отличную от заданной. \en Get one of the surfaces different from the given one. 
  const   MbSurface * GetAnotherSurface( const MbSurface & surface ) const;
  /// \ru Получить одну из двумерных кривых, отличную от заданной. \en Get one of two-dimensional curves different from the given one. 
  const   MbCurve *   GetAnotherCurve  ( const MbCurve &   curve   ) const;

  void        GetPointsByEvenLengthDelta( size_t n, std::vector<MbCartPoint3D> & pnts ) const override; // \ru Выдать n точек кривой с равными интервалами по длине дуги. \en Get n points of a curve with equally spaced by the arc length. 

          /// \ru Вычислить точки изменения выпуклости-вогнутости кривой пересечения. \en Calculate points of changing the convexity-concavity of intersection curve. 
          MbeNewtonResult ConvexoConcaveNewton( size_t iterLimit, double & t ) const;
          /// \ru Определить наличие точек изменения выпуклости-вогнутости. \en Determine existence of points of changing the convexity-concavity. 
          bool            IsConvexoConcave( SArray<double> & params, VERSION version ) const;
          /// \ru Определить наличие точек изменения выпуклости-вогнутости. \en Determine existence of points of changing the convexity-concavity. 
          bool            IsConvexoConcave( c3d::DoubleVector & params, VERSION version ) const;

          /// \ru Построить участок пространственной копии кривой. \en Construct a piece of a spatial curve copy. 
          MbCurve3D * MakeCurve( double t1, double t2 ) const;
          /// \ru Построить пространственную копию кривой. \en Construct a spatial curve copy. 
          MbCurve3D * MakeCurve() const;

          // \ru Функции аппроксимации неявной кривой пересечения. \en Functions for approximation of implicit intersection curve. 

          /// \ru Дать пространственную аппроксимацию кривой пересечения. \en Get an approximate spatial curve for interpretation of the intersection. 
    const MbCurve3D * GetSpaceCurve( VERSION version = Math::DefaultMathVersion() ) const;
          /// \ru Дать пространственную аппроксимацию кривой пересечения. \en Get an approximate spatial curve for interpretation of the intersection. 
          MbCurve3D * SetSpaceCurve( VERSION version = Math::DefaultMathVersion() );
          /// \ru Дать точную пространственную копию или себя. \en Get exact spatial copy or itself. 
    const MbCurve3D & GetExactCurve( bool saveParams = true, VERSION version = Math::DefaultMathVersion() ) const;
          /// \ru Удалить пространственную кривую. \en Remove a spatial curve. 
          void        ReleaseSpaceCurve() const;

          /// \ru Разрезать кривую пересечения на три части по заданным параметрам и вернуть одну из крайних частей в зависимости от sense. \en Cutaway an intersection curve into three pieces by given parameters and return one of end pieces depending on 'sense'. 
          MbSurfaceIntersectionCurve * BreakWithGap( double tt, double ttP, bool sense ); // \ru Используется в конвертерах. \en Used in converters. 

          /// \ru Усечь кривую пересечения по двум точкам и заданному направлению. \en Trim intersection curve by two points and the given direction. 
          MbCurve3D * Trimmed( const MbCartPoint3D & p1, const MbCartPoint3D & p2, bool sense, bool saveParamLenAndLaw = false  ) const;

          /// \ru Определить, гладкая ли кривая пересечения. \en Determine whether the intersection curve is smooth. 
          bool        IsSmooth() const;
          /// \ru Определить, полюсная ли кривая пересечения. \en Determine whether the intersection curve is pole.  
          bool        IsPole()   const;
          /// \ru Определить, является ли кривая пересечения кривой разъема. \en Determine whether the curve is a parting curve. 
          bool        IsSplit( bool strict = false ) const;

          /// \ru Получить толерантность кривой. \en Get tolerance of the curve. 
          double      GetTolerance() const;
          /// \ru Выставить толерантность кривой. \en Set tolerance of the curve. 
          void        SetTolerance( double tol ) { tolerance = tol; }
          /// \ru Сбросить толерантность кривой. \en Reset tolerance of the curve. 
          void        ResetTolerance() { tolerance = UNDEFINED_DBL; }
private:
          /// \ru Вычисление точек и производных пересекающихся поверхностей. \en Points and derivatives calculation for intersection surfaces.
          bool        Explorer( double t, bool readyOne, bool readyTwo,
                                MbCartPoint & pointOne, MbVector & firstOne, MbVector & secondOne, 
                                MbCartPoint & pointTwo, MbVector & firstTwo, MbVector & secondTwo, 
                                MbCartPoint3D & pnt1, MbVector3D & uDer1, MbVector3D & vDer1, MbVector3D & uuDer1, MbVector3D & vvDer1, MbVector3D & uvDer1, MbVector3D & nor1,
                                MbCartPoint3D & pnt2, MbVector3D & uDer2, MbVector3D & vDer2, MbVector3D & uuDer2, MbVector3D & vvDer2, MbVector3D & uvDer2, MbVector3D & nor2 ) const;
          /// \ru Вычислить значения точки и производных. \en Calculate the point and the first derivative. 
          bool        SpecificExplore( double  t, size_t ord, std::array<MbVector3D, 4> & sDer,
                                       std::array<MbVector, 4> & pDer1, std::array<MbVector, 4> & pDer2 ) const;
          bool        SpecificExplore( double  t, MbCartPoint3D & pnt, MbVector3D * fir, MbVector3D * sec, MbVector3D * thir ) const;
          // \ru Вычислить толерантность кривой. \en Calculate tolerance of the curve. 
          void        CalculateTolerance() const;
          // \ru Создать пространственную кривую по проекционной кривой. \en Create a spatial curve from a projection curve. 
          bool        TryProjection() const;
          // \ru Создать явную пространственную кривую. \en Create an explicit spatial curve. 
          bool        CreateSpaceCurve( VERSION version = Math::DefaultMathVersion() ) const;
          // \ru Создать аппроксимационную кривую по кривой пересечения \en Create an approximating curve by an intersection curve 
          MbCurve3D * CreateApproxCurve( bool doContinuous, double & appoxTolerance, VERSION version ) const;

          // \ru Проверить на равенство количества сегментов контуров на поверхностях. \en Check for equality of count of segments of contours on surfaces. 
          bool        IsSurfContoursCorrect() const;

          // \ru Добавить точки одной полилинии в другую. \en Add points of one polyline to the another one. 
          bool        AddCurveToCurve( const MbCurve & from1, const MbCurve & from2,
                                       bool fromBegin, bool toBegin, MbeCurveBuildType & spec,
                                       bool insertInterimPoints, double tolerance, const VERSION version );

          // \ru Добавить базовые кривые усеченных кривых. \en Add the base curves of trimmed curves. 
          bool        AddTrimmedToTrimmed( const MbCurve * addCurveOne, const MbCurve * addCurveTwo,
                                           bool fromBegin, bool toBegin, double eps, const VERSION version );

          // \ru Добавить базовые кривые репараметризованных кривых. \en Add the base curves of reparametrized curves. 
          bool        AddReparamSegment( const MbCurve * fromRep, MbReparamCurve * toRep,
                                         const MbCurve * fromOther, MbSurfaceCurve & curveOther,
                                         bool fromBegin, bool toBegin, double eps,
                                         const VERSION version );

          // \ru Создать контуры из кривых. \en Create contours from curves. 
          void        ChangeToContour( const MbCurve * addCurveOne, const MbCurve * addCurveTwo,
                                       bool fromBegin, bool toBegin );

private:
          // \ru Определить топологический тип кривой по внутренним данным. \en Identify topology type by internal data. 
          void        SetGlueType() const;

private:
          void        operator = ( const MbSurfaceIntersectionCurve & ); // \ru Не реализовано !!! \en Not implemented!!! 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSurfaceIntersectionCurve )
};

IMPL_PERSISTENT_OPS( MbSurfaceIntersectionCurve )

//------------------------------------------------------------------------------
// \ru Скопировать из копии готовые метрические оценки, которые в оригинале не были рассчитаны. \en Copy the finished metric estimations from duplicate which weren't calculated in the original. 
// \ru Внимание: для скорости проверка идентичности оригинала и копии не выполняется! \en Attention: for speed purposes the check of equality of the original and the copy isn't performed! 
// ---
inline bool MbSurfaceIntersectionCurve::CopyReadyMutable( const MbSurfaceIntersectionCurve & s ) 
{ 
  bool changed = false;

  if ( lengthEvaluation < 0.0 && s.lengthEvaluation >= 0.0 ) {
    lengthEvaluation = s.lengthEvaluation;
    changed = true;
  }
  if ( metricLength < 0.0 && s.metricLength >= 0.0 ) {
    metricLength = s.metricLength;
    changed = true;
  }
  if ( cube.IsEmpty() && !s.cube.IsEmpty() ) {
    cube = s.cube;
    changed = true;
  }
  MbCurve3D * sspaceCurve = s.spaceCurveCache()->spaceCurve;
  if ( sspaceCurve != nullptr && spaceCurveCache.LongTerm()->spaceCurve == nullptr ) {
    {
      ScopedLock cacheLock( spaceCurveCache.GetLock() );
      spaceCurveCache.LongTerm()->spaceCurve = (MbCurve3D *)&sspaceCurve->Duplicate();
    }
    spaceCurveCache.Reset();
    changed = true;
  }

  return changed;
}


//------------------------------------------------------------------------------
// \ru Проверить параметр. \en Check parameter. 
// ---
inline void MbSurfaceIntersectionCurve::CheckParam( double & t ) const
{
  double tmin = std_max( curveOne.GetCurve().GetTMin(), curveTwo.GetCurve().GetTMin() );
  double tmax = std_min( curveOne.GetCurve().GetTMax(), curveTwo.GetCurve().GetTMax() );
  bool closed = ( curveOne.IsClosed() || curveTwo.IsClosed() );

  if ( t < tmin ) {
    if ( closed ) {
      double tmp = tmax - tmin;
      t -= ::floor((t - tmin) / tmp) * tmp; 
    }
    else
      t = tmin;
  }
  else if ( t > tmax ) {
    if ( closed ) {
      double tmp = tmax - tmin;
      t -= ::floor((t - tmin) / tmp) * tmp; 
    }
    else
      t = tmax;
  }
}


//------------------------------------------------------------------------------
// \ru Вычислить точку. \en Calculate a point. 
// ---
inline void MbSurfaceIntersectionCurve::GetPointOn( double & t, MbCartPoint3D & pnt ) const
{
  MbCartPoint3D p1, p2;
  // \ru Получить точку с кривой пересечения, лежащей на первой поверхности \en Get point on the intersection curve on the first surface 
  curveOne.PointOn( t, p1 );
  // \ru Получить точку с кривой пересечения, лежащей на второй поверхности \en Get point on the intersection curve on the second surface 
  curveTwo.PointOn( t, p2 );
  // \ru Получить точку на кривой пересечения поверхностей, как среднее \en Get point on the intersection curve as average 
  pnt.Set( p1, 0.5, p2, 0.5 );
}


//------------------------------------------------------------------------------
// \ru Вычислить первую производную по t. \en Calculate first derivative with respect to t. 
// ---
inline void MbSurfaceIntersectionCurve::GetFirstDer( double & t, MbVector3D & fd ) const
{
  MbVector3D vect1, vect2;
  // \ru Вычислить производную для кривой, лежащей на первой поверхности \en Calculate derivative of curve on the first surface 
  curveOne.FirstDer( t, vect1 );
  // \ru Вычислить производную для кривой, лежащей на второй поверхности \en Calculate derivative of the curve on the second surface 
  curveTwo.FirstDer( t, vect2 );
  // \ru Получить производную на кривой пересечения поверхностей, как среднее \en Get derivative of surfaces intersection curve as average 
  fd.Set( vect1, 0.5, vect2, 0.5 );
}

//------------------------------------------------------------------------------
// \ru Конструктор вспомогательных данных. \en Auxiliary data constructor.
// ---
inline MbSurfaceIntersectionCurve::MbIntersectionCurveAuxiliaryData::MbIntersectionCurveAuxiliaryData()
  : AuxiliaryData()
  , t( UNDEFINED_DBL )
  , res( false )
  , uv1( UNDEFINED_DBL, 0 )
  , uv2( UNDEFINED_DBL, 0 )
  , pnt( UNDEFINED_DBL, 0, 0 )
  , fder( UNDEFINED_DBL, 0, 0 )
  , sder( UNDEFINED_DBL, 0, 0 )
  , tder( UNDEFINED_DBL, 0, 0 )
{
}

//------------------------------------------------------------------------------
// \ru Конструктор копирования вспомогательных данных. \en Copy constructor of auxiliary data.
// ---
inline MbSurfaceIntersectionCurve::MbIntersectionCurveAuxiliaryData::MbIntersectionCurveAuxiliaryData( const MbIntersectionCurveAuxiliaryData & init )
  : AuxiliaryData()
  , t( init.t )
  , res( init.res )
  , uv1( init.uv1 )
  , uv2( init.uv2 )
  , pnt( init.pnt )
  , fder( init.fder )
  , sder( init.sder )
  , tder( init.tder )
{
}

//------------------------------------------------------------------------------
// \ru Инициализация вспомогательных данных. \en Initialization of auxiliary data.
// ---
inline void MbSurfaceIntersectionCurve::MbIntersectionCurveAuxiliaryData::Init()
{
  t = UNDEFINED_DBL;
  res = false;
  uv1.Init( UNDEFINED_DBL, 0 );
  uv2.Init( UNDEFINED_DBL, 0 );
  pnt.Init( UNDEFINED_DBL, 0, 0 );
  fder.Init( UNDEFINED_DBL, 0, 0 );
  sder.Init( UNDEFINED_DBL, 0, 0 );
  tder.Init( UNDEFINED_DBL, 0, 0 );
}

//------------------------------------------------------------------------------
// \ru Инициализация вспомогательных данных. \en Initialization of auxiliary data.
// ---
inline void MbSurfaceIntersectionCurve::MbIntersectionCurveAuxiliaryData::Init( const MbIntersectionCurveAuxiliaryData & init )
{
  t = init.t;
  res = init.res;
  uv1.Init( init.uv1 );
  uv2.Init( init.uv2 );
  pnt.Init( init.pnt );
  fder.Init( init.fder );
  sder.Init( init.sder );
  tder.Init( init.tder );
}

#endif // __CUR_SURFACE_INTERSECTION_H
