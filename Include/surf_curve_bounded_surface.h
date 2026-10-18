////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Поверхность произвольной криволинейной границей и возможными вырезами внутри.
         \en The surface with an arbitrary curved boundary and possible cuts inside. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_CURVE_BOUNDED_SURFACE_H
#define __SURF_CURVE_BOUNDED_SURFACE_H


#include <templ_p_array.h>
#include <cur_contour.h>
#include <surface.h>
#include <cur_contour_on_surface.h>


class    MbRect;
class    MbRect2D;
class              MbContoursSearchTree;


//------------------------------------------------------------------------------
/** \brief \ru Ограниченная кривыми поверхность.
           \en The surface bounded by curves. \~
  \details \ru Поверхность, ограниченная замкнутыми контурами на поверхности MbContourOnSurface, 
    представляет собой общий случай поверхности. \n
    Областью определения параметров такой поверхности является 
    связный кусок двумерной плоскости, который описывается 
    одним внешним двумерным контуром и некоторым числом внутренних контуров. \n
    Контур представляет собой составную замкнутую двумерную кривую.
    Каждый двумерный контур в совокупности с базовой поверхностью образует контур на поверхности MbContourOnSurface, 
    который проходит по одному из краёв поверхности.
    Каждый контур описывает одну замкнутую границу поверхности. \n
    Первый контур контейнера curves описывает внешнюю границу и содержит внутри все остальные контуры, 
    которые описывают внутренние вырезы в поверхности. \n
    Внутренние контуры не могут быть вложены друг в друга.
    Внутренние контуры лежат внутри внешнего контура. \n
    В случае отсутствия вырезов внутри поверхности внутренние контуры отсутствуют.
    Контуры не пересекают друг друга и сами себя. \n
    Контуры, описывающие область определения поверхности, 
    могут выходить за область определения базовой поверхности basisSurface. 
    При выходе за пределы области определения базовая поверхность basisSurface продолжается 
    по своему закону изменения, как, например, элементарные поверхности, или - по касательной в общем случае. \n
    Для правильной работы метода, определяющего положение параметров относительно границ поверхности, 
    контуры должны быть ориентированы (метод NormalizeCurvesOrientation()).
    Если смотреть навстречу нормали поверхности с ориентированными контурами, то 
    внешний контур на поверхности ориентирован против часовой стрелки,
    а внутренние контуры на поверхности ориентированы по часовой стрелке. \n
    Ограниченная кривыми поверхность считается не замкнутой по обоим параметрам.
    Для периодической базовой поверхности область определения периодического параметра 
    ограниченной кривыми поверхности может превышать период базовой поверхности.
    Базовой поверхностью для ограниченной кривыми поверхности не может служить другая ограниченная кривыми поверхность.
    В подобной ситуации выполняется переход к первичной базовой поверхности.
    Поверхность используется для построения граней тел в общем случае.
           \en The surface bounded by closed contours on surface MbContourOnSurface
    represents the general case of a surface. \n
    Domain of parameters of such surface is the 
    connected piece of the two-dimensional plane which is described by 
    one external two-dimensional contour and some internal contours. \n
    Contour is composite closed two-dimensional curve.
    Each two-dimensional contour together with base surface forms a contour on surface MbContourOnSurface 
    which passes through one of surface edges.
    Each contour describes one closed boundary of surface. \n
    First contour of 'curves' container describes external boundary and contains inside all the other contours 
    describing internal cuts of a surface. \n
    Internal contours can not be enclosed each other.
    Internal loops lie inside the external loop. \n
    In case of absence of cuts inside surfaces internal contours are absent.
    Loops don't intersect each other and themselves. \n
    Contours describing the surface domain 
    can exceed the domain of base surface basisSurface. 
    When it is out of domain bounds base surface basisSurface is extended 
    according to its law of change as elementary surfaces for example or by tangent generally. \n
    For the correct work of the method determining the position of parameters relative to surface boundary 
    contours have to be oriented (NormalizeCurvesOrientation() method).
    If look towards to a surface normal with the oriented contours then 
    the external contour of surface is oriented counterclockwise
    but internal contours of surface are oriented clockwise. \n
    The surface bounded by curves is considered to be open by both parameters.
    For a periodic base surface the domain of periodic parameter of 
    the surface bounded by curves can exceed the period of a base surface.
    Another surface bounded by curves can't be the base surface for surface bounded by curves.
    In this situation it changes to the initial base surface.
    The surface is used for creation of faces of solids in general case. \~
  \ingroup Surfaces
*/
// ---
class MATH_CLASS MbCurveBoundedSurface : public MbSurface {
private:
  MbSurface *                     basisSurface; ///< \ru Базовая поверхность. \en Base surface. 
  RPArray<MbContourOnSurface>     curves;       ///< \ru Множество контуров на поверхности, определяющих её края. \en Set of contours on surface determining surface boundary. 
  /// \ru Габариты области определения параметров. \en Bounds of parameter domain. 
  double                          umin;         ///< \ru Минимальное  значение параметра u. \en Minimal value of parameter u. 
  double                          vmin;         ///< \ru Минимальное  значение параметра v. \en Minimal value of parameter v. 
  double                          umax;         ///< \ru Максимальное значение параметра u. \en Maximal value of parameter u. 
  double                          vmax;         ///< \ru Максимальное значение параметра v. \en Maximal value of parameter v. 

  mutable MbContoursSearchTree *  searchTree;   ///< \ru Дерево габаритов для ускорения поиска. \en A tree of bounding boxes for search acceleration. 

public :
  /// \ru Конструктор без установки пределов по u, v. \en Constructor without setting the u, v limits. 
  MbCurveBoundedSurface( const MbSurface & initSurface );
  /// \ru Конструктор с установкой пределов по u, v. \en Constructor with setting the u, v limits. 
  MbCurveBoundedSurface( const MbSurface & initSurface, double uin, double uax, double vin, double vax );
  /// \ru Конструктор с установкой пределов по u, v. \en Constructor with setting the u, v limits. 
  MbCurveBoundedSurface( const MbSurface & initSurface, const MbRect & rect );
  /// \ru Конструктор с установкой пределов по u, v. \en Constructor with setting the u, v limits. 
  MbCurveBoundedSurface( const MbSurface & initSurface, const MbRect2D & rect );
  /// \ru Конструктор с массивом контуров на поверхности. \en Constructor with array of contours on surface. 
  MbCurveBoundedSurface( MbSurface & initSurface, RPArray<MbContourOnSurface> & initCurves, bool sameContours );
  /// \ru Конструктор с массивом контуров на плоскости (двумерных контуров). \en Constructor with array of contours on plane (two-dimensional contours). 
  MbCurveBoundedSurface( MbSurface & initSurface, RPArray<MbContour> & initCurves, bool sameContours );
  /// \ru Конструктор с массивом контуров на плоскости (двумерных контуров). \en Constructor with array of contours on plane (two-dimensional contours). 
  MbCurveBoundedSurface( MbSurface & initSurface, c3d::PlaneContoursSPtrVector & initCurves, bool sameContours );
  /// \ru Конструктор для поверхности c габаритом при чтении грани. \en Constructor for surface with bounding box at face reading. 
  MbCurveBoundedSurface( MbSurface & initSurface, RPArray<MbContour> & initCurves, MbCube & gab );
  /// \ru Конструктор для поверхности c габаритом при чтении грани. \en Constructor for surface with bounding box at face reading. 
  MbCurveBoundedSurface( MbSurface & initSurface, c3d::PlaneContoursSPtrVector & initCurves, MbCube & gab );
  /// \ru Конструктор по контурам, берет за базовую поверхность поверхность первого контура. \en Constructor by contours, uses the surface of first contour as base surface. 
  MbCurveBoundedSurface( MbContourOnSurface & init1, MbContourOnSurface * init2 = nullptr );
  /// \ru Конструктор-копия на новую базовую поверхность. \en Copy-constructor for new base surface. 
  MbCurveBoundedSurface( const MbCurveBoundedSurface & init, MbSurface & newBaseSurface, bool calculateGabarit = true );

protected:
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbCurveBoundedSurface( const MbCurveBoundedSurface & init, MbRegDuplicate * ); 
  /// \ru Конструктор копирования контуров с той же поверхностью (для CurvesDuplicate()). \en Copy-constructor of contours with the same surface (for CurvesDuplicate()). 
  MbCurveBoundedSurface( const MbCurveBoundedSurface * init ); 
private :
  MbCurveBoundedSurface( const MbCurveBoundedSurface & ); // \ru Не реализовано !!! \en Not implemented !!! 
public :
  virtual ~MbCurveBoundedSurface();

public :
  VISITING_CLASS( MbCurveBoundedSurface );
  
  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */
  MbeSpaceType  IsA() const override; // \ru Тип элемента. \en A type of element. 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Является ли объект копией. \en Whether the object is a copy. 
  bool    SetEqual ( const MbSpaceItem & ) override;       // \ru Сделать равным. \en Make equal. 
  bool    IsSimilar( const MbSpaceItem & ) const override; // \ru Являются ли объекты подобными. \en Determine whether the objects are similar.
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector.
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate around an axis.
  double  DistanceToPoint( const MbCartPoint3D & ) const override;   // \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  void    Refresh() override; // \ru Сбросить все временные данные. \en Reset all temporary data.
  void    CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.
  
  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта. \en Set properties of the object. 
  void    GetBasisItems ( RPArray<MbSpaceItem> & ) override;  // \ru Дать базовые поверхности. \en Get base surfaces. 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \} */
  /** \ru \name Функции описания области определения поверхности
      \en \name Functions for surface domain description
      \{ */
  double  GetUMin() const override;    // \ru Минимальное значение параметра u. \en Minimal value of parameter u.
  double  GetVMin() const override;    // \ru Минимальное значение параметра v. \en Minimal value of parameter v.
  double  GetUMax() const override;    // \ru Максимальное значение параметра u. \en Maximal value of parameter u.
  double  GetVMax() const override;    // \ru Максимальное значение параметра v. \en Maximal value of parameter v.

  bool    IsUClosed() const override;  // \ru Замкнута ли гладко поверхность по параметру u без учета граничного контура. \en Whether the surface is smoothly closed by parameter u without regard to the boundary contour.
  bool    IsVClosed() const override;  // \ru Замкнута ли гладко поверхность по параметру v без учета граничного контура. \en Whether the surface is smoothly closed by parameter v without regard to the boundary contour.
  bool    IsUTouch() const override;   // \ru Замкнута ли фактически поверхность по параметру u независимо от гладкости. \en Whether the surface is actually closed by parameter u regardless of the smoothness.
  bool    IsVTouch() const override;   // \ru Замкнута ли фактически поверхность по параметру v независимо от гладкости. \en Whether the surface is actually closed by parameter v regardless of the smoothness.
  bool    IsUPeriodic() const override; // \ru Замкнута ли гладко поверхность по параметру u. \en Whether the surface is smoothly closed by parameter u.
  bool    IsVPeriodic() const override; // \ru Замкнута ли гладко поверхность по параметру v. \en Whether the surface is smoothly closed by parameter v.
  double  GetUPeriod() const override; // \ru Период для замкнутой поверхности или 0. \en Period for closed surface or 0.
  double  GetVPeriod() const override; // \ru Период для замкнутой поверхности или 0. \en Period for closed surface or 0.

  bool    GetPoleUMin() const override;
  bool    GetPoleUMax() const override;
  bool    GetPoleVMin() const override;
  bool    GetPoleVMax() const override;
  bool    IsPole( double u, double v, double paramPrecision = PARAM_PRECISION ) const override; // \ru Является ли точка особенной \en Whether the point is special
  /** \} */
  /** \ru \name Функции для работы в области определения поверхности
            Функции PointOn, Derive... поверхностей корректируют параметры
            при выходе их за пределы прямоугольной области определения параметров.\n
      \en \name Functions for working at surface domain
            Functions PointOn, Derive... of surfaces correct parameters
            when they are out of bounds of rectangular domain of parameters.\n
      \{ */
  void    PointOn  ( double & u, double & v, MbCartPoint3D & ) const override; // \ru Точка на поверхности. \en The point on the surface.
  void    DeriveU  ( double & u, double & v, MbVector3D & ) const override; // \ru Первая производная по u. \en The first derivative with respect to u.
  void    DeriveV  ( double & u, double & v, MbVector3D & ) const override; // \ru Первая производная по v. \en The first derivative with respect to v.
  void    DeriveUU ( double & u, double & v, MbVector3D & ) const override; // \ru Вторая производная по u. \en The second derivative with respect to u.
  void    DeriveVV ( double & u, double & v, MbVector3D & ) const override; // \ru Вторая производная по v. \en The second derivative with respect to v.
  void    DeriveUV ( double & u, double & v, MbVector3D & ) const override; // \ru Вторая производная по uv. \en The second derivative with respect to uv.
  void    DeriveUUU( double & u, double & v, MbVector3D & ) const override; // \ru Третья производная по u. \en The third derivative with respect to u.
  void    DeriveUUV( double & u, double & v, MbVector3D & ) const override; // \ru Третья производная. \en The third derivative.
  void    DeriveUVV( double & u, double & v, MbVector3D & ) const override; // \ru Третья производная. \en The third derivative.
  void    DeriveVVV( double & u, double & v, MbVector3D & ) const override; // \ru Третья производная по v. \en The third derivative with respect to v.
  void    TangentU ( double & u, double & v, MbVector3D & ) const override; // \ru Касательный вектор. \en Tangent vector.
  void    TangentV ( double & u, double & v, MbVector3D & ) const override; // \ru Касательный вектор. \en Tangent vector.
  void    Normal   ( double & u, double & v, MbVector3D & ) const override; // \ru Нормаль. \en Normal.
  void    NormalU  ( double & u, double & v, MbVector3D & ) const override; // \ru Производная нормали по u. \en The derivative of normal with respect to u.
  void    NormalV  ( double & u, double & v, MbVector3D & ) const override; // \ru Производная нормали по v. \en The derivative of normal with respect to v.
  /** \} */
  /** \ru \name Функции для работы внутри и вне области определения поверхности
            функции _PointOn, _Derive... поверхностей не корректируют
            параметры при выходе их за пределы прямоугольной области определения параметров.
      \en \name Functions for working inside and outside the surface's domain
            functions _PointOn, _Derive... of surfaces don't correct
            parameters when they are out of bounds of rectangular domain of parameters
      \{ */
  void   _PointOn  ( double u, double v, MbCartPoint3D & ) const override; // \ru Точка на расширенной поверхности. \en The point on the extended surface.
  void   _DeriveU  ( double u, double v, MbVector3D & ) const override; // \ru Первая производная по u. \en The first derivative with respect to u.
  void   _DeriveV  ( double u, double v, MbVector3D & ) const override; // \ru Первая производная по v. \en The first derivative with respect to v.
  void   _DeriveUU ( double u, double v, MbVector3D & ) const override; // \ru Вторая производная по u. \en The second derivative with respect to u.
  void   _DeriveVV ( double u, double v, MbVector3D & ) const override; // \ru Вторая производная по v. \en The second derivative with respect to v.
  void   _DeriveUV ( double u, double v, MbVector3D & ) const override; // \ru Вторая производная по uv. \en The second derivative with respect to uv.
  void   _DeriveUUU( double u, double v, MbVector3D & ) const override; // \ru Третья производная. \en The third derivative.
  void   _DeriveUUV( double u, double v, MbVector3D & ) const override; // \ru Третья производная. \en The third derivative.
  void   _DeriveUVV( double u, double v, MbVector3D & ) const override; // \ru Третья производная. \en The third derivative.
  void   _DeriveVVV( double u, double v, MbVector3D & ) const override; // \ru Третья производная. \en The third derivative.
  void   _Normal   ( double u, double v, MbVector3D & ) const override; // \ru Нормаль. \en Normal.
  void   _NormalU  ( double u, double v, MbVector3D & ) const override; // \ru Производная нормали. \en The derivative of normal.
  void   _NormalV  ( double u, double v, MbVector3D & ) const override; // \ru Производная нормали. \en The derivative of normal.
  void   _NormalUU ( double u, double v, MbVector3D & ) const override; // \ru Производная нормали. \en The derivative of normal.
  void   _NormalUV ( double u, double v, MbVector3D & ) const override; // \ru Производная нормали. \en The derivative of normal.
  void   _NormalVV ( double u, double v, MbVector3D & ) const override; // \ru Производная нормали. \en The derivative of normal.
  /** \} */
  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметров поверхности.
      \en \name Functions for get of the group of data inside and outside the surface's domain of parameters.
      \{ */
  void    Explore( double & u, double & v, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer,
                   MbVector3D * uuDer, MbVector3D * vvDer, MbVector3D * uvDer, MbVector3D * nor ) const override;
  // \ru Значения производных в точке \en Values of derivatives at point 
  void   _PointNormal( double u, double v,
                       MbCartPoint3D & pnt, MbVector3D & deru, MbVector3D & derv,
                       MbVector3D & norm, MbVector3D & noru, MbVector3D & norv,
                       MbVector3D & deruu, MbVector3D & dervv, MbVector3D & deruv ) const override;
  /** \} */
  /** \ru \name Функции движения по поверхности
      \en \name Functions of moving along the surface
      \{ */
  double  StepU         ( double u, double v, double sag ) const override; // \ru Вычисление шага по u по заданной стрелке прогиба. \en Calculation of the parameter step in u direction by the sag.
  double  StepV         ( double u, double v, double sag ) const override; // \ru Вычисление шага по v по заданной стрелке прогиба. \en Calculation of the parameter step in v direction by the sag.
  double  DeviationStepU( double u, double v, double angle ) const override; // \ru Вычисление шага по u по заданному углу отклонения. \en Calculation of the parameter step in u direction by the deviation angle.
  double  DeviationStepV( double u, double v, double angle ) const override; // \ru Вычисление шага по v по заданному углу отклонения. \en Calculation of the parameter step in v direction by the deviation angle.
  double  MetricStepU   ( double u, double v, double length ) const override; // \ru Вычисление шага по u по заданной длине. \en Calculation of the parameter step in u direction by the given length.
  double  MetricStepV   ( double u, double v, double length ) const override; // \ru Вычисление шага по v по заданной длине. \en Calculation of the parameter step in v direction by the given length.
  size_t  GetUCount() const override; // \ru Количество разбиений по параметру u для проверки событий. \en Count of splittings by parameter u to check for events.
  size_t  GetVCount() const override; // \ru Количество разбиений по параметру v для проверки событий. \en Count of splittings by parameter v to check for events.
  /** \} */
  /** \ru \name Общие функции поверхности
      \en \name Common functions of surface
      \{ */
  const MbSurface & GetSurface()      const override; // \ru Дать базовую поверхность. \en Get the base surface.
  const MbSurface & GetBasisSurface() const override; // \ru Дать базовую поверхность. \en Get the base surface.
        MbSurface & SetSurface() override    ; // \ru Дать базовую поверхность. \en Get the base surface.
        MbSurface & SetBasisSurface() override; // \ru Дать базовую поверхность. \en Get the base surface.
  
  // \ru Выдать граничную точку \en Get the boundary point 
  void    GetLimitPoint( ptrdiff_t num, MbCartPoint3D & ) const override; // \ru Выдать граничную трехмерную точку. \en Get the three-dimensional boundary point.
  void    GetLimitPoint( ptrdiff_t num, MbCartPoint & )   const override; // \ru Выдать граничную двумерную точку (граничные параметры). \en Get the two-dimensional boundary point (boundary parameters).

  double  CurvatureU ( double u, double v ) const override; // \ru Кривизна линии u. \en Curvature of u-line. 
  double  CurvatureV ( double u, double v ) const override; // \ru Кривизна линии v. \en Curvature of v-line. 

  bool    IsSameBase( const MbSurface & ) const override;  // \ru Является ли базовая поверхность копией базовой поверхности данного объекта. \en Whether the base surface is a duplicate of base surface of current object. 
  bool    IsPlanar( double accuracy = METRIC_EPSILON )  const override; // \ru Является ли поверхность плоской. \en Whether the surface is planar. 
  double  GetRadius() const override; // \ru Дать физический радиус объекта или ноль, если это невозможно. \en Get the physical radius of the object or null if it impossible.
  double  GetFilletRadius( const MbCartPoint3D &, double accuracy = METRIC_REGION ) const override; // \ru Является ли поверхность скруглением. \en Whether the surface is fillet.
  MbeParamDir GetFilletDirection( double accuracy = METRIC_REGION ) const override; // \ru Направление поверхности скругления. \en Direction of fillet surface. 
  bool    GetCylinderAxis( MbAxis3D & ) const override; // \ru Дать ось вращения для поверхности. \en Get a rotation axis of a surface.
  bool    GetCenterLines( std::vector<MbCurve3D *> & clCurves ) const override; // \ru Дать осевые (центральные) линии для поверхности. \en Get center lines of a surface.

  void    ChangeCarrier     ( const MbSpaceItem & item, MbSpaceItem & init ) override; // \ru Изменение носителя. \en Changing of carrier.
  bool    ChangeCarrierBorne( const MbSurface & item, MbSurface & init, const MbMatrix & matr ) override; // \ru Изменение носимых элементов. \en Change a carrier elements. 

  MbSplineSurface * NurbsSurface( double u1, double u2, double v1, double v2, bool bmatch = false ) const override; // \ru NURBS копия поверхности. \en NURBS copy of a surface. 
  MbSurface *       NurbsSurface( const MbNurbsParameters & uParam, const MbNurbsParameters & vParam ) const override;
  MbSurface *       Offset( double d, bool same ) const override; // \ru Создание эквидистантной поверхности. \en Create an offset surface. 

  MbCurve3D * CurveU( double v, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии v = const. \en Spatial copy of 'v = const'-line. 
  MbCurve3D * CurveV( double u, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии u = const. \en Spatial copy of 'u = const'-line. 
  MbCurve3D * CurveUV( const MbLineSegment &, bool bApprox = true ) const override; // \ru Пространственная копия линии по параметрической линии. \en Spatial copy of line by parametric line. 

  MbeItemLocation PointClassification( const MbCartPoint &, bool ignoreClosed = false ) const override; // \ru Находится ли точка в области, принадлежащей поверхности. \en Whether the point is in region belonging to the surface.
  double  DistanceToBorder( const MbCartPoint &, double & eps ) const override; // \ru Параметрическое расстояние до ближайшей границы. \en Parametric distance to the nearest boundary.

  // \ru Определение точек пересечения кривой с контурами поверхности. \en Determine intersection points of a curve with the contours on the surface. 
  size_t  CurveClassification( const MbCurve & curve, SArray<double> & tcurv, SArray<MbVector> & dir ) const override;
  /** \brief \ru Определить точки пересечения с двумерной кривой.
             \en Determine points of intersection with two-dimensional uv-curve. \~
    \details \ru Определить точки пересечения плоской кривой и плоских контуров поверхности. \n
             \en Determine intersection points of a planar curve and contours. \n \~
    \param[in] pCurve - \ru Кривая.
                        \en A curve. \~
    \param[out] curveParams - \ru Массив параметров на кривой.
                              \en An array of parameters on the curve. \~
    \return \ru Количество точек пересечения.
            \en The number of points. \~
  */
  size_t  SegmentIntersection( const MbCurve & pCurve, SArray<double> & curveParams, double epsilon = Math::metricEpsilon ) const;

  // \ru Найти ближайшую проекцию точки на поверхность. \en Find the nearest projection of a point onto the surface. 
  bool    NearPointProjection( const MbCartPoint3D & p, double & u, double & v, bool ext, MbRect2D * uvRange = nullptr ) const override; // \ru Ближайшая проекция точки на поверхность. \en The nearest point projection onto the surface.
  // \ru Найти все проекции точки на поверхность вдоль вектора в любом из двух направлений. \en Find all a point projection onto the surface along a vector in either of two directions. 
  void    DirectPointProjection( const MbCartPoint3D & p, const MbVector3D & vect, SArray<MbCartPoint> & uv, bool ext, MbRect2D * uvRange = nullptr ) const override;
  // \ru Все точки пересечения поверхности и кривой. \en All the points of intersection of a surface and a curve. 
  void    CurveIntersection( const MbCurve3D & curv, SArray<MbCartPoint> & uv, SArray<double> & tt, 
                             bool ext0, bool ext, bool touchInclude = false ) const override;
  // \ru Уточнение параметров точки линии очерка поверхности. \en Refinement of parameters of point of isocline curve of the surface. 
  MbeNewtonResult SilhouetteNewton( const MbVector3D & eye, bool perspective, const MbAxis3D * axis, MbeParamDir switchPar,
                                    double funcEpsilon, size_t iterLimit, double & u, double & v, bool ext ) const override;
  // \ru Подобные ли поверхности для объединения (слива). \en Whether the surfaces to union (joining) are similar. 
  bool    IsSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override;
  bool    IsSpecialSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override;
  // \ru Изменение носимых двумерных кривых (точек) поверхности путем проецирования на совпадающую поверхность. \en Change a carrier two-dimensional curves (points) of surface by projecting onto the coinciding surface. 
  bool    ProjectCurveOnSimilarSurface( const MbCurve3D & spaceCurve, const MbCurve & curve, const MbSurface & surfNew, MbCurve *& curveNew ) const override;
  // \ru Дать двумерную матрицу преобразования из своей параметрической области в параметрическую область surf. \en Get two-dimensional transformation matrix from own parametric region to parametric region of 'surf'. 
  bool    GetMatrixToSurface( const MbSurface & surf, MbMatrix & matr, VERSION version, double precision = METRIC_PRECISION ) const override;
  ThreeStates Salient() const override; // \ru Выпуклая ли поверхность. \en Whether the surface is convex. 
  // \ru Построить касательные и нормальные плейсменты конструктивных плоскостей. \en Construct tangent and normal placements of constructive planes. 
  bool    CreateNormalPlacements ( const MbVector3D & axisZ, double angle, SArray<MbPlacement3D> & places, VERSION version = Math::DefaultMathVersion() ) const override;
  bool    CreateTangentPlacements( const MbVector3D & axisZ, SArray<MbPlacement3D> & places ) const override;
  // \ru Определение разбивки параметрической области поверхности вертикалями и горизонталями. \en Determine splitting of parametric region of surface by vertical and horizontal lines. 
  void    GetTesselation( const MbStepData & stepData,
                          double u1, double u2, double v1, double v2,
                          SArray<double> & uu, SArray<double> & vv ) const override;
  // \ru Расчёт площади области определения параметров. \en Calculate area of parameter domain. 
  double  ParamArea() const override;

  size_t  GetUPairs( double v, SArray<double> & u ) const override; // \ru Вычислить U-пары от V. \en Calculate U-pairs by V.
  size_t  GetVPairs( double u, SArray<double> & v ) const override; // \ru Вычислить V-пары от U. \en Calculate V-pairs by U.
     
  void    CalculateGabarit( MbCube & ) const override; // \ru Рассчитать габарит поверхности. \en Calculate bounding box of surface. 
  void    CalculateLocalGabarit( const MbMatrix3D &, MbCube & ) const override; // \ru Рассчитать габарит относительно л.с.к. \en Calculate bounding box relative to the local coordinate system. 

  bool    SetLimit( double u1, double v1, double u2, double v2 ) override; // \ru Установить пределы. \en Set limits. 
  void    IncludePoint( double u, double v ) override; // \ru Расширить параметрические границы поверхности. \en Extend parametric bounds of surface. 

  double  GetParamDelta() const override;  // \ru Дать максимальное приращение параметра. \en Get the maximal increment of parameter.
  double  GetParamPrice() const override;  // \ru Дать минимально различимую величину параметра. \en Get the minimum distinguishable value of parameter. 

  double  GetUParamToUnit() const override; // \ru Дать приращение параметра u, соответствующее единичной длине в пространстве. \en Get increment of u-parameter, corresponding to the unit length in space. 
  double  GetVParamToUnit() const override;  // \ru Дать приращение параметра v, соответствующее единичной длине в пространстве. \en Get increment of v-parameter, corresponding to the unit length in space. 
  double  GetUParamToUnit( double u, double v ) const override; // \ru Дать приращение параметра u, соответствующее единичной длине в пространстве. \en Get increment of u-parameter, corresponding to the unit length in space. 
  double  GetVParamToUnit( double u, double v ) const override; // \ru Дать приращение параметра v, соответствующее единичной длине в пространстве. \en Get increment of v-parameter, corresponding to the unit length in space. 
  void    GetParamsToUnit( double u, double v, double & uParam, double & vParam ) const override; // \ru Дать приращение параметра u и параметра v, соответствующее единичной длине в пространстве. \en Get increment of parameters, corresponding to the unit length in space. 
  
  void    CalculateSurfaceWire( const MbStepData & stepData, size_t beg,  MbMesh & mesh,
                                size_t uMeshCount = c3d::WIRE_MAX, size_t vMeshCount = c3d::WIRE_MAX ) const override;   // \ru Рассчитать сетку. \en Calculate mesh.
        
  size_t  GetUMeshCount() const override; // \ru Выдать количество полигонов по u. \en Get the count of polygons by u. 
  size_t  GetVMeshCount() const override; // \ru Выдать количество полигонов по v. \en Get the count of polygons by v. 

  bool    IsLineU() const override; // \ru Если true все производные по U выше первой равны нулю. \en If true, then all the derivatives by U higher the first one are equal to zero. 
  bool    IsLineV() const override; // \ru Если true все производные по V выше первой равны нулю. \en If true, then all the derivatives by V higher the first one are equal to zero.      
  
  MbContour & MakeContour( bool sense ) const override; // \ru Выдать граничных двумерный контур. \en Get the two-dimensional boundary contour.
  MbCurve & MakeSegment( size_t i, bool sense ) const override; // \ru Дать граничную двумерную кривую. \en Get the two-dimensional boundary curve.
  /// \ru Аппроксимация поверхности треугольными пластинами. \en Approximation of a surface by triangular plates. 
  void    CalculateSurfaceGrid( const MbStepData & stepData, bool sense, MbGrid & grid ) const override;
  /** \} */
  /** \ru \name Функции поверхности MbCurveBoundSurface
      \en \name Functions of MbCurveBoundSurface surface
      \{ */
  
  /** \brief \ru Дать граничную двумерную кривую.
             \en Get the two-dimensional boundary curve. \~
    \details \ru Делает копию кривой.
             \en Makes a copy of a curve. \~
    \param[in] number - \ru Номер кривой
                        \en Index of curve \~
    \param[in] i - \ru Индекс сегмента
                   \en Index of segment \~
    \param[in] sense - \ru Признак совпадения направления кривой
                       \en Attribute of coincidence of curve direction \~
  */
          MbCurve & MakeSegment( size_t number, size_t i, bool sense ) const;     
  /** \brief \ru Дать граничную двумерную кривую.
             \en Get the two-dimensional boundary curve. \~
    \details \ru Дать граничную двумерную кривую.
             \en Get the two-dimensional boundary curve. \~
    \param[in] number - \ru Номер кривой
                        \en Index of curve \~
    \param[in] i - \ru Индекс сегмента
                   \en Index of segment \~    
  */
  const   MbCurve * GetSegment ( size_t number, size_t i ) const;
  /** \brief \ru Дать граничную двумерную кривую.
             \en Get the two-dimensional boundary curve. \~
    \details \ru Дать граничную двумерную кривую.
             \en Get the two-dimensional boundary curve. \~
    \param[in] number - \ru Номер кривой
                        \en Index of curve \~
    \param[in] i - \ru Индекс сегмента
                   \en Index of segment \~    
  */
  MbCurve * SetSegment ( size_t number, size_t i );

  /// \ru Выдать число контуров. \en Get the count of contours. 
  size_t  GetCurvesCount() const { return curves.Count(); } 
  /// \ru Выдать число сегментов в контуре с номером i \en Get the count of segments of i-th contour 
  size_t  GetSegmentsCount( size_t i ) const; 
          
  /** \brief \ru Добавить контур.
             \en Add a contour. \~
    \details \ru Создает контур, ограничивающий поверхность, если число контуров = 0
             \en Creates a contour bounding a surface if count of contours is equal to 0 \~      
  */
  void    AddOuterContour(); 
  /** \brief \ru Удалить контур.
             \en Remove contour. \~
    \details \ru Удаляет указанный контур.
             \en Removes the specified contour. \~
    \param[in] cntr - \ru Удаляемый контур
                      \en Contour to remove \~       
  */
  void    DeleteContour( MbContourOnSurface * cntr );  
  /** \brief \ru Заменить контур.
             \en Replace contour. \~
    \details \ru Заменить контур.
             \en Replace contour. \~
    \param[in] index - \ru Индекс изменяемого контура
                       \en Index of contour to change \~
    \param[in] cntr - \ru Новый контур
                      \en New contour \~      
  */       
  bool    ChangeContour( size_t index, MbContourOnSurface * cntr );
  /// \ru Заменить базовую поверхность. \en Replace base surface. 
  bool    ChangeSurface( const MbSurface & newsurf ); 
  /// \ru Заменить базовую поверхность на ее копию. \en Replace base surface with its copy. 
  void    NewBasisSurface(); 
  /// \ru Вычислить параметрические границы поверхности без сброса габарита. \en Calculate parametric bounds of surface without resetting the bounding box. 
  void    CalculateUVLimitsOnly();    
  /// \ru Вычислить параметрические границы поверхности с пересчетом габарита. \en Calculate parametric bounds of surface with recalculation of the bounding box. 
  void    CalculateUVLimits();
  /** 
    \brief \ru Расширить параметрические границы базовой поверхности.
           \en Extend parametric bounds of base surface. \~
    \details \ru Если это возможно, параметрические границы базовой поверхности расширяются так, \n
      чтобы параметрические границы поверхности, ограниченной кривыми, находились внутри них.
             \en If it is possible, then parametric bounds of base surface are extended so\n
      that the parametric bounds of surface bounded by curves are inside of them. \~
  */
  void    SetBasisSurfaceUVLimits();  
  /** 
    \brief \ru Проверить, входят ли параметры в параметрические границы поверхности.
           \en Check, whether the parameters are in parametric bounds of surface. \~
    \details \ru Параметры проверяются на вхождение в область допустимых значений параметров для данной поверхности. \n
      Если параметр выходит за эту область, ему присваивается максимально или минимально допустимое значение.
             \en Parameters are checked for occurrence in region of permissible values of parameters for current surface. \n
      If parameter is out of this region, then maximum or minimum permissible value is assigned to it. \~
  */
  inline  void    CheckParam( double & u, double & v ) const; 
          
  /** \brief \ru Ориентировать ограничивающие контуры.
              \en Orient bounding contours. \~
    \details \ru Ориентирует внешний контур против часовой стрелки, внутренние контуры - по часовой стрелке.
              \en External contour is oriented counterclockwise, internal contours - clockwise. \~
    \return \ru Возвращает площадь параметрической области поверхности.
            \en Returns area of parametric region of surface. \~
    \warning \ru В конструкторах не вызывается, так как предполагается, что на вход поступает правильный набор контуров, \n
      а сама функция ориентирования требует много времени на ее выполнение.
              \en In constructors isn't called as it is supposed that given the correct set of contours,\n
      but function of orientation needs a lot of time for its execution. \~
              
  */
  double  NormalizeCurvesOrientation();

  // \ru Не используется \en Not used \~  bool    SetCurveEqual( const MbSpaceItem & ); // Сделать равными контуры. \en Make contours equal. 
  // \ru Не используется \en Not used \~  bool    IsCurveEqual ( const MbSpaceItem & ) const;  // Являются ли объекты подобными. \en Whether the objects are equal. 
  /// \ru Удалить все контуры. \en Remove all the contours. 
  void    DeleteCurves(); 
  /** \brief \ru Добавить контур.
              \en Add a contour. \~
    \details \ru Добавить контур. После добавления нужно вызвать CalculateUVLimits().
              \en Add a contour. After addition it is necessary to call CalculateUVLimits(). \~           
  */  
  void    AddCurve( MbContourOnSurface & contour );  
  /** \brief \ru Добавить контур.
              \en Add a contour. \~
    \details \ru Добавить контур. После добавления нужно вызвать CalculateUVLimits().
              \en Add a contour. After addition it is necessary to call CalculateUVLimits(). \~              
  */  
  void    AddCurve( MbContour & contour );
  /** \brief \ru Добавить контур.
              \en Add a contour. \~
    \details \ru Добавить контур. После добавления не нужно вызвать CalculateUVLimits().
              \en Add a contour. After addition it isn't necessary to call CalculateUVLimits(). \~              
  */
  void    AddContour( MbContour & contour ) { AddCurve( contour ); CalculateUVLimits(); } 

  /** \brief \ru Дать контур, ограничивающий поверхность, по его индексу.
             \en Get contour bounding surface by its index. \~
    \details \ru Дать контур, ограничивающий поверхность, по его индексу. С проверкой индекса.
             \en Get contour bounding surface by its index. With index checking. \~
  */    
  const   MbContourOnSurface * GetCurve ( size_t ind ) const { return ( ind < curves.Count() ) ? curves[ind] : nullptr; }
  /** \brief \ru Дать контур, ограничивающий поверхность, по его индексу.
             \en Get contour bounding surface by its index. \~
    \details \ru Дать контур, ограничивающий поверхность, по его индексу. Без проверки индекса.
      Рекомендуется использовать функцию GetCurve с проверкой индекса.
             \en Get contour bounding surface by its index. Without index checking.
      It is recommended to use the GetCurve function with index checking. \~
  */       
  const   MbContourOnSurface *_GetCurve ( size_t ind ) const { return curves[ind]; }
  /** \brief \ru Дать контур, ограничивающий поверхность, по его индексу.
             \en Get contour bounding surface by its index. \~
    \details \ru Дать контур, ограничивающий поверхность, по его индексу. С проверкой индекса.
             \en Get contour bounding surface by its index. With index checking. \~
  */      
          MbContourOnSurface * SetCurve ( size_t ind ) { return ( ind < curves.Count() ) ? curves[ind] : nullptr; }
  /** \brief \ru Дать контур, ограничивающий поверхность, по его индексу.
             \en Get contour bounding surface by its index. \~
    \details \ru Дать контур, ограничивающий поверхность, по его индексу. Без проверки индекса.
      Рекомендуется использовать функцию SetCurve с проверкой индекса.
             \en Get contour bounding surface by its index. Without index checking.
      It is recommended to use the SetCurve function with index checking. \~    
  */           
  MbContourOnSurface *_SetCurve ( size_t ind ) { return curves[ind]; }
  /// \ru Переместить кривую с индексом ind в нулевую позицию массива. \en Move a curve with the 'ind' index to a zero position of the array. 
  void    ReplaceOuterCurveBy( size_t ind ); 
  /// \ru Найти двумерную кривую и заменить ее на другую. \en Find two-dimensional curve and replace it with another one. 
  bool    ChangeCurve2D( MbCurve & oldCrv, MbCurve * newCrv ); 
  /// \ru Слить двумерные сегменты в контурах. \en Merge two-dimensional segments in contours.                
  void    MergeSegments( double eps = Math::LengthEps ); 
  /// \ru Копия объекта со старой базовой поверхностью. \en Copy of object with old base surface. 
  MbCurveBoundedSurface & CurvesDuplicate() const { return *new MbCurveBoundedSurface( this ); }
  /// \ru Проверить на замкнутость по u или v по внешнему контуру. \en Check closeness by u or v using outer contour. 
  bool    CheckTouchByContour( bool byU ) const;
  /** \} */

protected:  
  bool    CreateRectTree() const; ///< \ru Создать и инициализировать дерево поиска. \en Create and initialize the search tree. 
  void    DeleteRectTree() const; ///< \ru Удалить дерево поиска. \en Delete search tree. 
  void    DeleteSearchTree() const; ///< \ru Удалить дерево поиска. \en Delete search tree. 

private:
  // \ru Управление распределением памяти в массиве segments \en Control of memory allocation in the array "segments" 
  // \ru Не используется \en Not used \~  void    CurvesReserve( size_t additionalSpace ) { curves.Reserve( additionalSpace ); } // Зарезервировать место под столько элементов. \en Reserve memory for so many elements. 
  // \ru Не используется \en Not used \~  void    CurvesAdjust ()                         { curves.Adjust();                   } // Удалить лишнюю память. \en  Remove unnecessary memory. 
          
  void    operator = ( const MbCurveBoundedSurface & ); // \ru Не реализовано !!! \en Not implemented!!! 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCurveBoundedSurface )
};

IMPL_PERSISTENT_OPS( MbCurveBoundedSurface )


//------------------------------------------------------------------------------
// \ru Проверить, входят ли параметры в параметрические границы поверхности. \en Check, whether the parameters are in parametric bounds of surface. 
// ---
inline void MbCurveBoundedSurface::CheckParam ( double & u, double & v ) const
{
  if (u < umin)
    u = umin;
  else if ( u > umax )
    u = umax;

  if ( v < vmin)
    v = vmin;
  else if ( v > vmax )
    v = vmax;
}


#endif // __SURF_CURVE_BOUNDED_SURFACE_H
