////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Поверхность переменного сечения.
         \en The mutable section surface. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_SECTION_SURFACE_H
#define __SURF_SECTION_SURFACE_H


#include <surface.h>
#include <surf_spine.h>
#include <cur_surface_curve.h>
#include <cur_polycurve.h>
#include <function.h>
#include <mb_placement3d.h>
#include <mb_enum.h>
#include <tool_multithreading.h>
#include <templ_dptr.h>


#define   _RO_MIN_    0.001                             // \ru Минимальный дискриминант переменного сечения. \en The minimum discriminant of the mutable section.
#define   _CIRCLE_    0.4142135623730950488016887242097 // \ru Дискриминант для дуги окружности в сечении. \en The discriminant is corresponding to the circle.
#define   _PARABOLA_  0.5                               // \ru Дискриминант для параболы в сечении. \en The discriminant is corresponding to the parabola.
#define   _RO_MAX_    0.999                             // \ru Максимальный дискриминант переменного сечения. \en The maximum discriminant of the mutable section.


class  MbSurfaceWorkingData;


//------------------------------------------------------------------------------
/** \brief \ru Поверхность переменного сечения.
           \en The mutable section surface. \~
  \details \ru Поверхность переменного (конического) сечения образуется путем движения плоской кривой, являющейся коническим сечением, вдоль опорной кривой.
    В процессе движения форма плоской кривой меняется в соответствии с дискриминантом конического сечения.
    Начало плоской кривой располагается на начальной точке направляющей кривой, а конец - на конечной точке направляющей кривой.
    Плоскость переменного сечения сохраняет ортогональность опорной кривой в процессе движения.
    Первый параметр поверхности совпадает с параметром плоской кривой.
    Второй параметр поверхности совпадает с параметром опорной кривой. 
           \en The mutable section surface is formed by moving the flat conic section curve along the reference curve.
    In the process of movement, the shape of the flat curve changes in accordance with the discriminant of the conic section.
    The beginning of the flat curve is located on the first point guide curve and the end is located on the second point guide curve.
    The plane of the conic section preserves the orthogonality to the reference curve during movement.
    First parameter of surface coincides with the parameter of conic section curve.
    Second parameter of surface coincides with the parameter of reference curve. \~ 
  \ingroup Surfaces
*/
// ---
class MATH_CLASS MbSectionSurface : public MbSurface {

  //------------------------------------------------------------------------------
  /// \ru Параметры кривых. \en The curves parameters. \n \~
  // ---
  class MbSettings {
  public:
    double ts; ///< \ru Параметр опорной кривой. \en The parameter of the spine. 
    double t1; ///< \ru Параметр первой направляющей кривой. \en The parameter of the guide1. 
    double t2; ///< \ru Параметр второй направляющей кривой. \en The parameter of the guide2. 
    double t0; ///< \ru Параметр средней кривой. \en The XY parameter of the curves[0]. 

  public:
    MbSettings() : ts( 0.0 ), t1( 0.0 ), t2( 0.0 ), t0( 0.0 ) {}
    MbSettings( const MbSettings & other ) : ts( other.ts ), t1( other.t1 ), t2( other.t2 ), t0( other.t0 ) {}
    ~MbSettings() {}

    void Set( double tts, double tt1, double tt2, double tt0 ) { ts = tts; t1 = tt1; t2 = tt2; t0 = tt0; } // \ru Добавление данных. \en Set data. 
    void Get( double & tts, double & tt1, double & tt2, double & tt0 ) const { tts = ts; tt1 = t1; tt2 = t2; tt0 = t0; } // \ru Добавление данных. \en Get data. 
    void Init( const MbSettings & other ) { ts = other.ts; t1 = other.t1; t2 = other.t2; t0 = other.t0; }
    void operator = ( const MbSettings & other ) { ts = other.ts; t1 = other.t1; t2 = other.t2; t0 = other.t0; }
  };

protected:
  MbSpine *                spine;    ///< \ru Опорная кривая. \en The reference curve. \~ 
  MbFunction *             radius1;  ///< \ru Функция первого радиуса для поверхности скругления (может быть nullptr). \en The function of the first fillet surface radius (may be nullptr).
  MbFunction *             radius2;  ///< \ru Функция второго радиуса для поверхности скругления (может быть nullptr). \en The function of the second fillet surface radius (may be nullptr).
  MbCurve3D *              guide1;   ///< \ru Первая направляющая кривая. \en The first  guide curve. \~ 
  MbCurve3D *              guide2;   ///< \ru Вторая направляющая кривая. \en The second guide curve. \~ 
  std::vector<MbCurve3D *> curves;   ///< \ru Дополнительные контрольные кривые (могут отсутствовать). \en The additional control curves (may be empty). \~ 
  MbFunction *             function; ///< \ru Функция управления сечением (дискриминант, может быть nullptr)). \en Section control function (discriminant, may be nullptr). \~ 
  MbPolyCurve *            pattern;  ///< \ru Образующая кривая при form==cs_Shape (для других форм nullptr). \en Forming curve for form==cs_Shape (nullptr on other case). \~ 
  std::vector<MbCartPoint> shape;    ///< \ru Описание сечения при form==cs_Shape (пуст в других случаях). \en Description of shape cross-section for form==cs_Shape (is empty on other case). \~ 
  std::vector<double>      knots;    ///< \ru Узловой вектор сплайна. \en Knot vector of the spline. \~ 
  size_t                   order;    ///< \ru Порядок сплайна. \en Order of spline. \~ 
  MbeSectionShape          form;     ///< \ru Форма сечения поверхности при фиксированном втором параметре. \en The surface cross-section shape with the second parameter fixed. \~ 
  MbeSmoothForm            sform;    ///< \ru Форма поверхности скругления. \en The fillet surface form. \~
  double                   umin;     ///< \ru Минимальное значение первого параметра u. \en Minimal value of parameter u. \~ 
  double                   umax;     ///< \ru Максимальное значение первого параметра u. \en Maximal value of parameter u. \~ 
  double                   vmin;     ///< \ru Минимальное значение второго параметра v. \en Minimal value of parameter v. \~ 
  double                   vmax;     ///< \ru Максимальное значение второго параметра v. \en Maximal value of parameter v. \~ 
  bool                     uclosed;  ///< \ru Признак замкнутости поверхности по параметру u. \en An attribute of closedness in u-parameter direction. \~ 
  bool                     vclosed;  ///< \ru Признак замкнутости поверхности по параметру v. \en An attribute of closedness in v-parameter direction. \~ 
  bool                     poleVMin; ///< \ru Наличие полюса поверхности при vmin. \en Existence of a pole at vmin. \~ 
  bool                     poleVMax; ///< \ru Наличие полюса поверхности при vmax. \en Existence of a pole at vmax. \~ 
  std::vector<double>      vBreaks;  ///< \ru Параметры опорной кривой, для которых направляющие терпят излом. \en Parameters of the reference curve for which the guides have a break. \~
  std::vector<MbSettings>  settings; ///< \ru Нулевые приближения параметров опорных кривых служат для ускорения работы объекта. \en Zero approximations of the parameters of the reference curves are used for fast calculations. \n \~

  //------------------------------------------------------------------------------
  /// \ru Вспомогательные данные служат для ускорения работы объекта. \en Auxiliary data are used for fast calculations. \n \~
  // ---
  class MbSectionSurfaceAuxiliaryData : public AuxiliaryData {
  public:
    DPtr<MbSurfaceWorkingData> wData;  ///< \ru Рабочие данные для расчета поверхности. \en Working data for the calculation of a surface. 
    MbPlacement3D              wPlace; ///< \ru Плоскость XY локальной системы, перпендикулярная опорной кривой в текущей точке. \en The XY plane of local coordinate system perpendicular to the reference curve at its current points. 
    MbCartPoint                wPnt0;  ///< \ru Точка на плоскости от её пересечения с  первой направляющей кривой. \en The point on plane of its intersection with first  guide curve on surface.
    MbCartPoint                wPnt1;  ///< \ru Точка на плоскости от её пересечения с  первой направляющей кривой. \en The point on plane of its intersection with first  guide curve on surface.
    MbCartPoint                wPnt2;  ///< \ru Точка на плоскости от её пересечения со второй направляющей кривой. \en The point on plane of its intersection with second guide curve on surface.
    MbVector                   wVec1;  ///< \ru Вектор на плоскости от её пересечения с поверхностью первой направляющей. \en The vector on plane of its intersection with surface of first  guide curve.
    MbVector                   wVec2;  ///< \ru Вектор на плоскости от её пересечения с поверхностью второй направляющей. \en The vector on plane of its intersection with surface of second guide curve.
    double                     wPar0;  ///< \ru Параметр первой направляющей кривой её пересечения с плоскости. \en The parameter of the first  guide curve.
    double                     wPar1;  ///< \ru Параметр первой направляющей кривой её пересечения с плоскости. \en The parameter of the first  guide curve.
    double                     wPar2;  ///< \ru Параметр второй направляющей кривой её пересечения с плоскости. \en The parameter of the second guide curve.
    double                     wPar;   ///< \ru Параметр v, для которого вычислена плоскость wPlace. \en The parameter v for which was calculated the plane wPlace.
    MbVector3D                 wOrig1; ///< \ru Первая производная по v начала координат плоскости. \en The first derivative of the v origin of the wPlace.
    MbVector3D                 wOrig2; ///< \ru Вторая производная по v начала координат плоскости. \en The second derivative of the v origin of the wPlace.
    MbVector3D                 wDerv1; ///< \ru Первая производная по v оси axisX плоскости. \en The first derivative of axisX of wPlace with respect to v.
    MbVector3D                 wDerv2; ///< \ru Первая производная по v оси axisY плоскости. \en The first derivative of axisY of wPlace with respect to v.
    MbVector3D                 wDerv3; ///< \ru Первая производная по v оси axisZ плоскости. \en The first derivative of axisZ of wPlace with respect to v.
    MbVector3D                 wSecn1; ///< \ru Вторая производная по v оси axisX плоскости. \en The second derivative of axisX of wPlace with respect to v.
    MbVector3D                 wSecn2; ///< \ru Вторая производная по v оси axisY плоскости. \en The second derivative of axisY of wPlace with respect to v.
    MbVector3D                 wSecn3; ///< \ru Вторая производная по v оси axisZ плоскости. \en The second derivative of axisZ of wPlace with respect to v.
    std::vector<double>        wSplns; ///< \ru Рабочее множество В-сплайнов. \en The work set of B-splines.
    std::vector<MbCartPoint>   wxy;    ///< \ru Рабочее множество точек. \en The work set of the points.
    std::vector<MbVector>      wxyV;   ///< \ru Рабочее множество первых производных по V. \en The work set of the first derivatives.
    std::vector<MbVector>      wxyVV;  ///< \ru Рабочее множество вторых производных по V. \en The work set of the second derivatives.
    std::vector<MbVector>      wxyVVV; ///< \ru Рабочее множество третьих производных по V. \en The work set of the third derivatives.
    double                     wVal;   ///< \ru Параметр v, для которого вычислены производные начала и орт плоскости wPlace. \en The parameter v for which was calculated the plane origin derivative and axises derivatives.

    MbSectionSurfaceAuxiliaryData();
    MbSectionSurfaceAuxiliaryData( const MbSectionSurfaceAuxiliaryData & );
    virtual ~MbSectionSurfaceAuxiliaryData();

    void Init(); // \ru Инициализация данных. \en Init data. 
    void Init( const MbSectionSurfaceAuxiliaryData & );
    void Move( const MbVector3D & ); // \ru Сдвиг. \en Move. 
  };
  
  mutable CacheManager<MbSectionSurfaceAuxiliaryData> cache;

protected:
  /** \brief \ru Конструктор поверхности переменного сечения.
             \en The mutable section surface constructor. \~
    \details \ru Конструктор поверхности переменного сечения по опорной кривой и двум направляющим.
             \en The mutable section surface constructor by reference curve and guide curves. \~ 
    \param[in] sp -   \ru Опорная кривая.
                      \en The reference curve (spine). \~
    \param[in] c1 -   \ru Первая направляющая кривая.
                      \en The first guide curve. \~
    \param[in] c2 -   \ru Вторая направляющая кривая.
                      \en The second guide curve. \~
    \param[in] cs -   \ru Дополнительные направляющие кривые (могут отсутствовать). 
                      \en The additional guide curves (may be empty). \~
    \param[in] f -    \ru Форма сечения поверхности.
                      \en The form of the surface section. \~
    \param[in] uBeg - \ru Минимальное значение первого параметра поверхности.
                      \en The minimum value of the first surface parameter. \~
    \param[in] uEnd - \ru Максимальное значение первого параметра поверхности.
                      \en The maximum value of the first surface parameter. \~
    \param[in] func - \ru Функция управления сечением поверхности.
                      \en Section control function. \~
    \param[in] patt - \ru Образующая кривая при form==cs_Shape. 
                      \en Forming curve for form==cs_Shape. \~
    \param[in] sh   - \ru Описание сечения при form==cs_Shape (пуст в других случаях). 
                      \en Description of shape cross-section for form==cs_Shape. \~
  */
  MbSectionSurface( MbSpine & sp, MbFunction * r1, MbFunction * r2, 
                    MbCurve3D & c1, MbCurve3D & c2, 
                    std::vector<MbCurve3D *> cs, MbeSectionShape f, MbeSmoothForm sf,
                    double uBeg, double uEnd, MbFunction * func, 
                    MbPolyCurve * patt, const std::vector<MbCartPoint> & sh ); 

  /** \brief \ru Конструктор поверхности переменного сечения.
             \en The mutable section surface constructor. \~
  \details \ru Конструктор поверхности переменного сечения по осевой кривой и закону изменения радиуса вращения.
           \en The mutable section surface constructor with axis curve and radius law. \~
    \param[in] sp -   \ru Опорная кривая.
                      \en The reference curve (spine). \~
    \param[in] c0 -   \ru Осевая кривая.
                      \en The axis curve. \~
    \param[in] f -    \ru Форма сечения поверхности (cs_Round).
                      \en The form of the surface section (cs_Round). \~
    \param[in] uBeg - \ru Минимальное значение первого параметра поверхности (uBeg<uEnd).
                      \en The minimum value of the first surface parameter (uBeg<uEnd). \~
    \param[in] uEnd - \ru Максимальное значение первого параметра поверхности (0<uEnd<M_IP2).
                      \en The maximum value of the first surface parameter (0<uEnd<M_IP2). \~
    \param[in] func - \ru Функция изменения радиуса сечения поверхности.
                      \en Section radius function. \~
  */
  MbSectionSurface( MbSpine & sp, MbCurve3D & c0, MbeSectionShape f, double uBeg, double uEnd, MbFunction & func ); 
  // \ru Конструктор-копия. \en Copy constructor.
  MbSectionSurface( const MbSectionSurface &, MbRegDuplicate * );
private:
  MbSectionSurface( const MbSectionSurface & ); // \ru Не реализовано. \en Not implemented. 
public:
  // \ru Деструктор. \en The destructor.
  virtual ~MbSectionSurface();

public:
  VISITING_CLASS( MbSectionSurface );

  /** \brief \ru Создание поверхности переменного сечения.
             \en The mutable section surface creation. \~
    \details \ru Создание поверхности переменного сечения по опорной кривой и двум направляющим.
             \en The mutable section surface creation by reference curve and guide curves. \~ 
    \param[in] rc -       \ru Опорная кривая.
                          \en The reference curve (spine). \~
    \param[in] g1 -       \ru Первая направляющая кривая.
                          \en The first guide curve. \~
    \param[in] g2 -       \ru Вторая направляющая кривая (g1==g2 совпадает с первой при cs_Round).
                          \en The second guide curve (g1==g2 the same first guide for cs_Round). \~
    \param[in] c0 -       \ru Дополнительная направляющая кривая (может быть nullptr).
                          \en The additional guide curve (may be nullptr). \~
    \param[in] f -        \ru Форма сечения поверхности.
                          \en The form of the surface section. \~
    \param[in] sense -    \ru Направление нормали поверхности направляющей кривой (для guide1==guide2).
                          \en The normal direction of guide curve surface (for guide1==guide2). \~
    \param[in] uBeg -     \ru Минимальное значение первого параметра поверхности.
                          \en The minimum value of the first surface parameter. \~
    \param[in] uEnd -     \ru Максимальное значение первого параметра поверхности.
                          \en The maximum value of the first surface parameter. \~
    \param[in] func -     \ru Функция управления сечением поверхности.
                          \en Section control function. \~
    \param[in] patt -     \ru Образующая кривая при form==cs_Shape. 
                          \en Forming curve for form==cs_Shape. \~
    \param[in] buildSag - \ru Угловое отклонение при движении по кривым и поверхностям.
                          \en Angular deviation while moving along curves and surfaces. \~
    \param[in] accuracy - \ru Точность построения. 
                          \en The accuracy of building. \~
    \param[in] countBin - \ru Минимальное количество шагов при движении по опорной кривой.
                          \en Minimum number of steps while moving along the reference curve. \~
    \param[in] vers -     \ru Версия поверхности.
                          \en The surface version. \~
    \param[out] resType - \ru Код ошибки построения.
                          \en Result code of building. \~
    \result \ru Возвращает построенную поверхность.
            \en Returns the constructed surface. \~
  */
  static  MbSectionSurface * Create( const MbCurve3D & rc, 
                                     const MbFunction * r1, const MbFunction * r2, 
                                     const MbCurve3D & g1, const MbCurve3D & g2, 
                                     const MbCurve3D * c0, 
                                           MbeSectionShape f, MbeSmoothForm sf,
                                           bool sense,
                                           double uBeg, double uEnd, 
                                     const MbFunction * func, 
                                     const MbCurve * patt, 
                                           double buildSag, 
                                           double accuracy, 
                                           uint32 countBin,
                                           VERSION vers, 
                                           MbResultType & resType );

  /** \brief \ru Создание поверхности переменного сечения на заданном участке опорной кривой.
             \en The mutable section surface creation on the given section of the reference curve. \~
    \details \ru Создание поверхности переменного сечения по опорной кривой и двум направляющим на заданном участке опорной кривой.
             \en The mutable section surface creation by reference curve and guide curves on the given section of the reference curve. \~
    \param[in] rc -       \ru Опорная кривая.
                          \en The reference curve (spine). \~
    \param[in] g1 -       \ru Первая направляющая кривая.
                          \en The first guide curve. \~
    \param[in] g2 -       \ru Вторая направляющая кривая (g1==g2 совпадает с первой при cs_Round).
                          \en The second guide curve (g1==g2 the same first guide for cs_Round). \~
    \param[in] c0 -       \ru Дополнительная направляющая кривая (может быть nullptr).
                          \en The additional guide curve (may be nullptr). \~
    \param[in] f -        \ru Форма сечения поверхности.
                          \en The form of the surface section. \~
    \param[in] sense -    \ru Направление нормали поверхности направляющей кривой (для guide1==guide2).
                          \en The normal direction of guide curve surface (for guide1==guide2). \~
    \param[in] uBeg -     \ru Минимальное значение первого параметра поверхности.
                          \en The minimum value of the first surface parameter. \~
    \param[in] uEnd -     \ru Максимальное значение первого параметра поверхности.
                          \en The maximum value of the first surface parameter. \~
    \param[in] func -     \ru Функция управления сечением поверхности.
                          \en Section control function. \~
    \param[in] patt -     \ru Образующая кривая при form==cs_Shape.
                          \en Forming curve for form==cs_Shape. \~
    \param[in] rcTBeg -   \ru Начальный параметр опорной кривой.
                          \en Start parameter of the reference curve. \~                    
    \param[in] rcTEnd -   \ru Конечный параметр опорной кривой.
                          \en End parameter of the reference curve. \~
    \param[in] buildSag - \ru Угловое отклонение при движении по кривым и поверхностям.
                          \en Angular deviation while moving along curves and surfaces. \~
    \param[in] accuracy - \ru Точность построения.
                          \en The accuracy of building. \~
    \param[in] countBin - \ru Минимальное количество шагов при движении по опорной кривой.
                          \en Minimum number of steps while moving along the reference curve. \~
    \param[in] vers -     \ru Версия поверхности.
                          \en The surface version. \~
    \param[out] resType - \ru Код ошибки построения.
                          \en Result code of building. \~
    \result \ru Возвращает построенную поверхность.
            \en Returns the constructed surface. \~
  */
  static  MbSectionSurface * Create( const MbCurve3D & rc,
                                     const MbFunction * r1, const MbFunction * r2,
                                     const MbCurve3D & g1, const MbCurve3D & g2,
                                     const MbCurve3D * c0,
                                           MbeSectionShape f, MbeSmoothForm sf,
                                           bool sense,
                                           double uBeg, double uEnd,
                                     const MbFunction * func,
                                     const MbCurve * patt,
                                           double rcTBeg,
                                           double rcTEnd,
                                           double buildSag,
                                           double accuracy,
                                           uint32 countBin,
                                           VERSION vers,
                                           MbResultType & resType );

  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */

  MbeSpaceType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbeSpaceType  Type() const override; // \ru Групповой тип элемента. \en Group element type. 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Равны ли объекты. \en Whether the objects are equal. 
  bool    SetEqual ( const MbSpaceItem & ) override;       // \ru Сделать равным. \en Make equal. 
  bool    IsSimilar  ( const MbSpaceItem & ) const override; // \ru Являются ли объекты подобными. \en Determine whether the objects are similar.
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;          // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;          // \ru Сдвиг. \en Move.
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate around an axis.
  
  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта. \en Set properties of the object. 
  void    GetBasisItems  ( RPArray <MbSpaceItem> & ) override; // \ru Дать базовые объекты. \en Get the base objects.
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \} */
  /** \ru \name Функции описания области определения поверхности
      \en \name Functions for surface domain description 
      \{ */

  double  GetUMin() const override; // \ru Вернуть минимальное значение параметра u. \en Get the minimum value of u. 
  double  GetVMin() const override; // \ru Вернуть минимальное значение параметра v. \en Get the minimum value of v. 
  double  GetUMax() const override; // \ru Вернуть максимальное значение параметра u. \en Get the maximum value of u. 
  double  GetVMax() const override; // \ru Вернуть максимальное значение параметра v. \en Get the maximum value of v. 
  bool    IsUClosed() const override; // \ru Проверка замкнутости по параметру u. \en Check of surface closedness in u direction. 
  bool    IsVClosed() const override; // \ru Проверка замкнутости по параметру v. \en Check of surface closedness in v direction. 

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
  void    DeriveUUU( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUUV( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUVV( double & u, double & v, MbVector3D & ) const override;
  void    DeriveVVV( double & u, double & v, MbVector3D & ) const override;
  void    Normal   ( double & u, double & v, MbVector3D & ) const override; // \ru Вычислить нормаль. \en Calculate normal. 
  /** \} */
  /** \ru \name Функции для работы внутри и вне области определения поверхности
            функции _PointOn, _Derive... поверхностей не корректируют
            параметры при выходе их за пределы прямоугольной области определения параметров.
      \en \name Functions for working inside and outside the surface's domain
            functions _PointOn, _Derive... of surfaces don't correct
            parameters when they are out of bounds of rectangular domain of parameters.
      \{ */
  void   _PointOn  ( double u, double v, MbCartPoint3D & ) const override; // \ru Точка на расширенной поверхности. \en The point on the extended surface. 
  void   _DeriveU  ( double u, double v, MbVector3D & ) const override; // \ru Первая производная по u. \en The first derivative with respect to u. 
  void   _DeriveV  ( double u, double v, MbVector3D & ) const override; // \ru Первая производная по v. \en The first derivative with respect to v. 
  void   _DeriveUU ( double u, double v, MbVector3D & ) const override; // \ru Вторая производная по u. \en The second derivative with respect to u. 
  void   _DeriveVV ( double u, double v, MbVector3D & ) const override; // \ru Вторая производная по v. \en The second derivative with respect to v. 
  void   _DeriveUV ( double u, double v, MbVector3D & ) const override; // \ru Вторая производная по uv. \en The second derivative with respect to uv. 
  void   _DeriveUUU( double u, double v, MbVector3D & ) const override;
  void   _DeriveUUV( double u, double v, MbVector3D & ) const override;
  void   _DeriveUVV( double u, double v, MbVector3D & ) const override;
  void   _DeriveVVV( double u, double v, MbVector3D & ) const override;
  void   _Normal   ( double u, double v, MbVector3D & ) const override; // \ru Вычислить нормаль на расширенной поверхности. \en Calculate a normal on extended surface. 

  // \ru Функции доступа к группе данных для работы внутри и вне области определения параметров поверхности.
  // \en Functions for get of the group of data inside and outside the surface's domain of parameters.
  void    Explore( double & u, double & v, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer,
                   MbVector3D * uuDer, MbVector3D * vvDer, MbVector3D * uvDer, MbVector3D * nor ) const override;

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
  size_t  GetUCount() const override;
  size_t  GetVCount() const override;
  bool    GetPoleVMin() const override; // \ru Существует ли полюс на границе параметрической области. \en Whether a pole exists on parametric region boundary. 
  bool    GetPoleVMax() const override; // \ru Существует ли полюс на границе параметрической области. \en Whether a pole exists on parametric region boundary. 
  bool    IsPole( double u, double v, double paramPrecision = PARAM_PRECISION ) const override; // \ru Является ли точка полюсом. \en Whether the point is a pole. 

  /** \} */
  /** \ru \name Общие функции поверхности
      \en \name Common functions of surface
      \{ */

  double  CurvatureU    ( double u, double v ) const override; // \ru Кривизна линии по u. \en Curvature of line by u. 
  bool    IsPlanar( double accuracy = METRIC_EPSILON ) const override; // \ru Является ли поверхность плоской. \en Whether the surface is planar. 
    
  void    ChangeCarrier( const MbSpaceItem & item, MbSpaceItem & init ) override; // \ru Изменение носителя. \en Changing of carrier. 

  void    CalculateGabarit( MbCube & ) const override; // \ru Выдать габарит. \en Get the bounding box. 
  void    CalculateLocalGabarit( const MbMatrix3D &, MbCube & ) const override; // \ru Рассчитать габарит относительно л.с.к. \en Calculate bounding box relative to the local coordinate system. 

  MbSplineSurface * NurbsSurface( double, double, double, double, bool bmatch = false ) const override; // \ru NURBS копия поверхности. \en NURBS copy of a surface. 
  MbSurface *       NurbsSurface( const MbNurbsParameters & uParam, const MbNurbsParameters & vParam ) const override; // \ru NURBS копия поверхности. \en NURBS copy of a surface. 
  MbSurface *       Offset( double d, bool same ) const override; // \ru Создание эквидистантной поверхности. \en Create an offset surface. 

  // \ru Подобные ли поверхности для объединения (слива) \en Whether the surfaces to union (joining) are similar 
  //virtual bool    IsSpecialSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override; // \ru Специальный случай \en Special case 

  MbCurve3D * CurveU( double v, MbRect1D *pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии v = const. \en Spatial copy of 'v = const'-line.
  MbCurve3D * CurveV( double u, MbRect1D *pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии u = const. \en Spatial copy of 'u = const'-line.

  MbeParamDir GetFilletDirection( double accuracy = METRIC_REGION ) const override; // \ru Направление поверхности скругления. \en Direction of fillet surface. 
  // \ru Построить касательные и нормальные плейсменты конструктивных плоскостей. \en Construct tangent and normal placements of constructive planes. 
  bool    CreateNormalPlacements ( const MbVector3D & axisZ, double angle, SArray<MbPlacement3D> & places, VERSION version = Math::DefaultMathVersion() ) const override;
  bool    CreateTangentPlacements( const MbVector3D & axisZ, SArray<MbPlacement3D> & places ) const override;
  // \ru Подобные ли поверхности для объединения (слива). \en Whether the surfaces to union (joining) are similar. 
  bool    IsSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override;
  // \ru Дать двумерную матрицу преобразования из своей параметрической области в параметрическую область surf. \en Get two-dimensional transformation matrix from own parametric region to parametric region of 'surf'. 
  bool    GetMatrixToSurface( const MbSurface & surf, MbMatrix & matr, VERSION version, double precision = METRIC_PRECISION ) const override;

  /// \ru Определить, выпуклая ли поверхность. \en Determine whether the surface is convex. 
  ThreeStates Salient() const override; 

  /// \ru Касается ли поверхность опорных граней? \en Does the surface tangent with its reference faces?
  bool IsSurfaceTangent() const { return ( sform==st_Fillet || sform == st_Ellipse || sform == st_Span ) && ( form == cs_Conic || form == cs_Cubic || form == cs_Disked || form == cs_Fillet ); }

  // \ru Определение разбивки параметрической области поверхности вертикалями и горизонталями. \en Determine splitting of parametric region of surface by vertical and horizontal lines. 
  void    GetTesselation( const MbStepData & stepData,
                          double u1, double u2, double v1, double v2,
                          SArray<double> & uu, SArray<double> & vv ) const override;
  //virtual bool    IsSpinePeriodic() const; // \ru Периодичность направляющей. \en Periodicity of a reference curve. 

  size_t  GetUMeshCount() const override; // \ru Выдать количество полигонов по u. \en Get the count of polygons by u. 
  size_t  GetVMeshCount() const override; // \ru Выдать количество полигонов по v. \en Get the count of polygons by v. 

  /** \} */
  /** \ru \name Функции поверхности переменного сечения.
      \en \name Functions of the mutable section surface.
      \{ */

  /// \ru Дать опорный спайн. \en Get reference spine. 
  const MbSpine   & GetSpine() const { return *spine; } 
  /// \ru Дать опорную кривую. \en Get reference curve. 
  const MbCurve3D & GetSpineCurve() const { return spine->GetCurve(); } 
  /// \ru Дать функцию первого радиуса поверхности скругления. \en Get function of the first fillet surface radius.
  const MbFunction * GetRadius1() const { return radius1; }
  /// \ru Дать функцию второго радиуса поверхности скругления. \en Get function of the second fillet surface radius.
  const MbFunction * GetRadius2() const { return radius2; }
  /// \ru Дать первую направляющую кривую (может быть nullptr). \en Get first  guide curve (may be nullptr).
  const MbCurve3D * GetGuide1() const { return guide1; }
  /// \ru Дать вторую направляющую кривую (может быть nullptr). \en Get second guide curve (may be nullptr).
  const MbCurve3D * GetGuide2() const { return guide2; }
  /// \ru Дать кривую вершин (может отсутствовать). \en Get apex curve (may be nullptr).
  const MbCurve3D * GetApexCurve() const { return ( curves.size() > 0 ) ? curves[0] : nullptr; }
  /// \ru Дать дополнительную направляющую кривую (может отсутствовать). \en Get  additional guide curve (may be absence).
  const MbCurve3D * GetCurve( size_t i ) const { return ( i < curves.size() ) ? curves[i] : nullptr; }
  /// \ru Дать функцию управления сечением (радиус или дискриминант, может быть nullptr)). \en Get section control function (radius or discriminant, may be nullptr).
  const MbFunction * GetFunction() const { return function; }
  /// \ru Дать образующую кривую при form==cs_Shape (для других форм nullptr). \en Get forming curve for form==cs_Shape (nullptr on other case).
  const MbPolyCurve * GetPattern() const { return pattern; }
  /// \ru Дать форму сечения поверхности при фиксированном втором параметре. \en Get the surface cross-section shape with the second parameter fixed.
        MbeSectionShape GetSectionForm() const { return form; }
  ///< \ru Дать форму поверхности скругления. \en Get the fillet surface form. \~
        MbeSmoothForm   GetSmoothForm() const { return sform; }
  /// \ru Дать параметры опорной кривой, для которых направляющие терпят излом. \en Get parameters of the reference curve for which the guides have a break. 
  void    GetBreakVParams( std::vector<double> & vParams ) const;

  /// \ru Вычисление параметров направляющих кривых по второму параметру поверхности. \en Calculating the parameters of guide curves by the second surface parameter.
  bool    GuideParams( double v, double & t1, double & t2 ) const;
  /// \ru Вычисление параметра вершинной кривой по второму параметру поверхности. \en Calculating the parameter of apex curve by the second surface parameter.
  bool    ApexParam( double v, double & t0 ) const;
  /// \ru Вычисление точки поверхности по параметру направляющей кривой. \en Calculating the surface point by the parameter of first the guide curves.
  bool    ParamByGuide1( double t1, MbCartPoint & p ) const;
  /// \ru Вычисление точки поверхности по параметру направляющей кривой. \en Calculating the surface point by the parameter of the second guide curves.
  bool    ParamByGuide2( double t2, MbCartPoint & p ) const;
  /// \ru Вычисление второго параметра поверхности по параметру вершинной кривой. \en Calculating the second surface parameter by the parameter of the apex curve.
  bool    ParamByApex( double t0, double & v ) const;
  /// \ru Вычисление точек поверхности по параметрам первой направляющей кривой. \en Calculating surface points by the first guide curve.
  bool    PointsByGuide1( std::vector<MbCartPoint> & points ) const;
  /// \ru Вычисление точек поверхности по параметрам второй направляющей кривой. \en Calculating surface points by the second guide curve.
  bool    PointsByGuide2( std::vector<MbCartPoint> & points ) const;
  /// \ru Вычисление точек пересечения направляющих кривых и кривой вершин с плоскостью сечения, заданной вторым параметром поверхности. \en Calculating the intersection points of guide curves and apex curve with the section plane specified by the second surface parameter.
  bool    PhantomPoints( double v, MbCartPoint3D & guideP1, MbCartPoint3D & guideP2, MbCartPoint3D & apex, double & discrim ) const;

  /** \} */

protected :
  void    Init(); // \ru Инициализация данных. \en Data init. 
  void    BreakPoints( const MbCurve3D * , double precision = ANGLE_REGION ); // \ru Определение точек излома направляющих. \en Determination of the break points of the guides.
  void    SettingsInit( double epsilon = PARAM_EPSILON ); // \ru Определение соответствующих пересечений кривых с плоскостью сечения. \en Determination of the corresponding intersections of curves with the section plane.
  ptrdiff_t GetSetting( double t, double & t1, double & t2, double & t0 ) const; // \ru Определение параметров. \en Parameters calculation.
  void    CacheInit( bool resetLongTerm = true ); // \ru Инициализация вспомогательных данных. \en Auxiliary data init.
          // \ru Проверить и изменить при необходимости параметры поверхности. \en Check and correct parameters of the surface. 
  void    CheckParams( double & u, double & v, bool ext ) const; 
          // \ru Вычисление пересечений направляющих с плоскостью сечения поверхности. \ en Calculating the intersections of guides with the surface cross-section plane.
  bool    SectionData( const MbCurve3D * curve0, const double & v, MbPlacement3D & place, 
                       MbCartPoint & xy0, double & t0, 
                       MbCartPoint & xy1, MbVector & vec1, double & t1,
                       MbCartPoint & xy2, MbVector & vec2, double & t2 ) const;
          // \ru // Уточнение точек пересечений направляющих с плоскостью сечения поверхности. \ en Refinement of the intersections points of guides with the surface cross-section plane.
  bool    SmoothSection( const double & v, const MbPlacement3D & place, const double & t1, const double & t2, 
                         MbCartPoint & xy1, MbCartPoint & xy2, MbCartPoint & xy0 ) const;
          // \ru Расчет двумерных точек и их производных на плоском сечении поверхности. \en Calculation of two-dimension points and their derivatives on the surface plane section.
  void    Round( const double & u, const double & v, 
                 const MbCartPoint & xy1, const MbVector & xy1V, const MbVector & xy1VV, const MbVector & xy1VVV, 
                 const MbCartPoint & xy0, const MbVector & xy0V, const MbVector & xy0VV, const MbVector & xy0VVV, 
                 MbCartPoint & p, MbVector & pU, MbVector & pV, MbVector * pUU, MbVector * pUV, MbVector * pVV, 
                 MbVector * pUUU, MbVector * pUUV, MbVector * pUVV, MbVector * pVVV ) const;
  void    Linea( const double & u, 
                 const MbCartPoint & xy1, const MbVector & xy1V, const MbVector & xy1VV, const MbVector & xy1VVV, 
                 const MbCartPoint & xy2, const MbVector & xy2V, const MbVector & xy2VV, const MbVector & xy2VVV, 
                 MbCartPoint & p, MbVector & pU, MbVector & pV, MbVector * pUU, MbVector * pUV, MbVector * pVV, 
                 MbVector * pUUU, MbVector * pUUV, MbVector * pUVV, MbVector * pVVV ) const;
  void    Conic( const double & u, const double & v, 
                 const MbCartPoint & xy1, const MbVector & xy1V, const MbVector & xy1VV, const MbVector & xy1VVV, 
                 const MbCartPoint & xy2, const MbVector & xy2V, const MbVector & xy2VV, const MbVector & xy2VVV, 
                 const MbCartPoint & xy0, const MbVector & xy0V, const MbVector & xy0VV, const MbVector & xy0VVV, 
                 MbCartPoint & p, MbVector & pU, MbVector & pV, MbVector * pUU, MbVector * pUV, MbVector * pVV, 
                 MbVector * pUUU, MbVector * pUUV, MbVector * pUVV, MbVector * pVVV ) const;
  void    Cubic( const double & u, const double & v, std::vector<double> & bSplines,
                 const std::vector<MbCartPoint> & xy, const std::vector<MbVector> & xyV, 
                 const std::vector<MbVector> & xyVV, const std::vector<MbVector> & xyVVV,
                 MbCartPoint & p, MbVector & pU, MbVector & pV, MbVector * pUU, MbVector * pUV, MbVector * pVV, 
                 MbVector * pUUU, MbVector * pUUV, MbVector * pUVV, MbVector * pVVV ) const;
  void    Shape( const double & u, const double & v, 
                 const MbCartPoint & xy1, const MbVector & xy1V, const MbVector & xy1VV, const MbVector & xy1VVV, 
                 const MbCartPoint & xy2, const MbVector & xy2V, const MbVector & xy2VV, const MbVector & xy2VVV, 
                 const MbCartPoint & xy0, const MbVector & xy0V, const MbVector & xy0VV, const MbVector & xy0VVV, 
                 MbCartPoint & p, MbVector & pU, MbVector & pV, MbVector * pUU, MbVector * pUV, MbVector * pVV, 
                 MbVector * pUUU, MbVector * pUUV, MbVector * pUVV, MbVector * pVVV ) const;
  ptrdiff_t B_Splanes( double & u, std::vector<double> & bSplanes ) const; // \ru Вычисление B-сплайнов.\en B_splines calculation.
  // \ru Вычисление радиуса-вектора точки его производных поверхности. \en The surface radius-vector and it derivatives calculation.
  void    Section ( const double & u, const double & v, MbCartPoint3D & pnt, 
                    MbVector3D * uDer, MbVector3D * vDer, 
                    MbVector3D * uuDer, MbVector3D * uvDer, MbVector3D * vvDer, 
                    MbVector3D * uuuDer, MbVector3D * uuvDer, MbVector3D * uvvDer, MbVector3D * vvvDer,
                    MbSectionSurfaceAuxiliaryData * loccache ) const;
  void    PointOn  ( double & v, double & u, bool ext, MbCartPoint3D & p ) const; // \ru Вычисления точки поверхности. \en Calculate surface point.
  void    DeriveU  ( double & u, double & v, bool ext, MbVector3D & ) const; // \ru Первая производная по u. \en The first derivative with respect to u. 
  void    DeriveV  ( double & u, double & v, bool ext, MbVector3D & ) const; // \ru Первая производная по v. \en The first derivative with respect to v. 
  void    DeriveUU ( double & u, double & v, bool ext, MbVector3D & ) const; // \ru Вторая производная по u. \en The second derivative with respect to u. 
  void    DeriveUV ( double & u, double & v, bool ext, MbVector3D & ) const; // \ru Вторая производная по uv. \en The second derivative with respect to uv. 
  void    DeriveVV ( double & u, double & v, bool ext, MbVector3D & ) const; // \ru Вторая производная по v. \en The second derivative with respect to v. 
  void    DeriveUUU( double & u, double & v, bool ext, MbVector3D & ) const; // \ru Третья производная по uuu. \en The third derivative with respect to uuu. 
  void    DeriveUUV( double & u, double & v, bool ext, MbVector3D & ) const; // \ru Третья производная по uuv. \en The third derivative with respect to uuv. 
  void    DeriveUVV( double & u, double & v, bool ext, MbVector3D & ) const; // \ru Третья производная по uvv. \en The third derivative with respect to uvv. 
  void    DeriveVVV( double & u, double & v, bool ext, MbVector3D & ) const; // \ru Третья производная по vvv. \en The third derivative with respect to vvv. 
  void    Normal   ( double & u, double & v, bool ext, MbVector3D & ) const; // \ru Нормаль. \en The normal. 
  void    Normal   ( const MbVector3D & uDer, const MbVector3D & vDer, double u, double v, bool ext, MbVector3D & nor ) const;
  // \ru Вычисление точек для создания NURBS копии кривых поверхности. \en Points calculation for NURBS copy surface.
  bool    CollectNurbsPoints( double vin, double vax, size_t pCount, double angle,
                              SArray<double> & params,
                              SArray<MbCartPoint3D> & points1, 
                              SArray<MbCartPoint3D> & points2, 
                              SArray<MbCartPoint3D> & points_0, 
                              SArray<MbCartPoint3D> & points_1,
                              SArray<MbCartPoint3D> & points_2,
                              SArray<MbCartPoint3D> & points_3,
                              SArray<MbCartPoint3D> & points_4 ) const;

private:
  void    operator = ( const MbSectionSurface & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSectionSurface )
};


IMPL_PERSISTENT_OPS( MbSectionSurface )


#endif // __SURF_SECTION_SURFACE_H
