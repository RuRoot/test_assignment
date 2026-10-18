////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Параметры операций над телами.
         \en Parameters of operations on the solids. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __OP_SHELL_PARAMETERS_H
#define __OP_SHELL_PARAMETERS_H


#include <cur_contour_on_surface.h>
#include <cur_arc3d.h>
#include <cr_split_data.h>
#include <mb_nurbs_function.h>
#include <op_direct_mod_parameter.h> // deprecated
#include <op_swept_parameter.h>
#include <position_data.h>
#include <surf_plane.h>
#include <part_solid.h>
#include <surf_spine.h>


class MbArc3D;
class MbPoint3D;
class MbPolyCurve3D;
class MbPolyline3D;
class MbCurveBoundedSurface;
class MbBeamCreator;
class MbSectionData;
class MbSectionRail;
class MbRegTransform;
class MbRegDuplicate;
class IProgressIndicator;


//------------------------------------------------------------------------------
/** \brief \ru Параметры скругления или фаски ребра.
           \en Parameters of fillet or chamfer of edge. \~
  \details \ru Параметры скругления или фаски ребра содержат информацию, необходимую для выполнения операции. \n
           \en The parameter of fillet or chamfer of edge contain Information necessary to perform the operation. \n \~
  \ingroup Shell_Building_Parameters
*/ // ---
struct MATH_CLASS SmoothValues {
public:
  /// \ru Способы обработки углов стыковки трёх рёбер. \en Methods of processing corners of connection by three edges.
  enum CornerForm {
    ec_pointed = 0, ///< \ru Обработка угла отсутствует. \en Processing of corner is missing.
    ec_either  = 1, ///< \ru Стыкующиеся в одной точке три ребра обрабатываются в порядке внутренней нумерации ребер без учета выпуклости и вогнутости. \en Mating at one point of three edges are processed in the order of internal indexation of edges without convexity and concavity.
    ec_uniform = 2, ///< \ru Если в точке стыкуются два выпуклых (вогнутых) и одно вогнутое (выпуклое) ребро, то первым  обрабатывается вогнутое (выпуклое) ребро. \en If two convex (concave) and one concave (convex) edge are mated at the point, then concave (convex) edge is processed at the first.
    ec_sharp   = 3, ///< \ru Если в точке стыкуются два выпуклых (вогнутых) и одно вогнутое (выпуклое) ребро, то первыми обрабатываются выпуклые (вогнутые) ребра. \en If two convex (concave) and one concave (convex) edge are mated at the point, then concave (convex) edges are processed at the first.
  };

 /* \ru ВНИМАНИЕ!!! Все открытые поля будут перенесены в закрытую секцию.
        Необходимо использовать соответствующие функции инициализации и функции доступа.
    \en ATTENTION!!! All open fields will be moved to the closed section. 
        It is necessary to use the appropriate initialization functions and access functions.
 */      
public:
  double        distance1;    ///< \ru Радиус кривизны/катет на первой поверхности. \en Radius of curvature/leg on the first surface.
  double        distance2;    ///< \ru Радиус кривизны/катет на второй поверхности. \en Radius of curvature/leg on the second surface.
  double        conic;        ///< \ru Коэффициент формы, изменяется от 0.05 до 0.95 (при 0 - дуга окружности). \en Coefficient of shape is changed from 0.05 to 0.95 (if 0 - circular arc).
  double        begLength;    ///< \ru Расстояние от начала скругления до точки остановки (UNDEFINED_DBL - остановки нет). \en Distance from the beginning of fillet to the stop point (UNDEFINED_DBL  - no stop).
  double        endLength;    ///< \ru Расстояние от конца  скругления до точки остановки (UNDEFINED_DBL - остановки нет). \en Distance from the end of fillet to the stop point (UNDEFINED_DBL  - no stop).
  MbeSmoothForm form;         ///< \ru Тип сопряжения скругление/фаска. \en Mate type of fillet/chamfer.
  CornerForm    smoothCorner; ///< \ru Способ обработки углов стыковки трёх рёбер. \en Method of processing corners of connection by three edges.
  bool          prolong;      ///< \ru Продолжить по касательной. \en Prolong along the tangent.
  ThreeStates   keepCant;     ///< \ru Автоопределение сохранения кромки (ts_neutral), сохранение поверхности (ts_negative), сохранение кромки (ts_positive). \en Auto detection of boundary saving (ts_neutral), surface saving (ts_negative), boundary saving (ts_positive).
  bool          strict;       ///< \ru При false скруглить хотя бы то,  что возможно. \en If false - round at least what is possible.
  bool          equable;      ///< \ru В углах сочленения вставлять тороидальную поверхность (для штамповки листового тела). \en In corners of the joint insert toroidal surface (for stamping sheet solid).

private:
  MbVector3D    vector1;      ///< \ru Вектор нормали к плоскости, по которой выполняется усечение скругления в начале цепочки. \en Normal vector of the plane cutting the fillet at the beginning of chain.
  MbVector3D    vector2;      ///< \ru Вектор нормали к плоскости, по которой выполняется усечение скругления в конце цепочки.  \en Normal vector of the plane cutting the fillet at the end of chain.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  SmoothValues()
    : distance1     ( 1.0         )
    , distance2     ( 1.0         )
    , conic         ( c3d::_ARC_  )
    , begLength     (UNDEFINED_DBL)
    , endLength     (UNDEFINED_DBL)
    , form          ( st_Fillet   )
    , smoothCorner  ( ec_uniform  )
    , prolong       ( false       )
    , keepCant      ( ts_negative )
    , strict        ( true        )
    , equable       ( false       )
    , vector1       (             )
    , vector2       (             )
  {}

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \deprecated \ru Конструктор устарел, взамен использовать функции инициализации.
                \en The constructor is deprecated. Instead, use initialization functions. \~
    \param[in] d1, d2 - \ru Радиусы кривизны/катеты.
                        \en Radii of curvature/catheti. \~
    \param[in] f      - \ru Способ построения поверхности сопряжения.
                        \en Method of construction of mating surface. \~
    \param[in] c      - \ru Коэффициент формы, изменяется от 0.05 до 0.95 (при 0 - дуга окружности).
                        \en Coefficient of shape is changed from 0.05 to 0.95 (if 0 - circular arc). \~
    \param[in] pro    - \ru Продолжить по касательной.
                        \en Prolong along the tangent. \~
    \param[in] cor    - \ru Способ скругления "чемоданных" углов.
                        \en Method for bending corner of three surfaces. \~
    \param[in] autoS  - \ru Автоопределение сохранения кромки/поверхности.
                        \en Auto detection of boundary/surface saving. \~
    \param[in] keep   - \ru Сохранять кромку (true) или сохранять поверхность скругления/фаски (false).
                        \en Keep boundary (true) or keep surface of fillet/chamfer (false). \~
    \param[in] str    - \ru Строгое скругление. Если false, скруглить хотя бы то, что возможно.
                        \en Strict fillet. If false - round at least what is possible. \~
    \param[in] equ    - \ru В углах сочленения вставлять тороидальную поверхность.
                        \en In corners of the joint insert toroidal surface. \~
  */
  DEPRECATE_DECLARE_REPLACE ( SmoothValues::Init... )
  SmoothValues( double d1, double d2, MbeSmoothForm f, double c, bool pro,
                CornerForm cor, bool autoS, bool keep, bool str, bool equ )
    : distance1     ( d1    )
    , distance2     ( d2    )
    , conic         ( c     )
    , begLength     (UNDEFINED_DBL)
    , endLength     (UNDEFINED_DBL)
    , form          ( f     )
    , smoothCorner  ( cor   )
    , prolong       ( pro   )
    , keepCant      ( ts_negative )
    , strict        ( str   )
    , equable       ( equ   )
    , vector1       (       )
    , vector2       (       )
  {
    if ( form == st_Fillet && ::fabs(::fabs(d1) - ::fabs(d2)) > METRIC_EPSILON )
      form = st_Ellipse; 
    keepCant = autoS ? ts_neutral : ts_negative;
    if ( keep )
      keepCant = ts_positive;

  }

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \deprecated \ru Конструктор устарел, взамен использовать функции инициализации.
                \en The constructor is deprecated. Instead, use initialization functions. \~
    \param[in] d1, d2     - \ru Радиусы кривизны/катеты.
                            \en Radii of curvature/catheti. \~
    \param[in] smoothForm - \ru Способ построения поверхности сопряжения.
                            \en Method of construction of mating surface. \~
    \param[in] conicValue - \ru Коэффициент формы, изменяется от 0.05 до 0.95 (при 0 - дуга окружности).
                            \en Coefficient of shape is changed from 0.05 to 0.95 (if 0 - circular arc). \~
    \param[in] bProlong   - \ru Продолжить по касательной.
                            \en Prolong along the tangent. \~
    \param[in] cornerForm - \ru Способ скругления "чемоданных" углов.
                            \en Method for bending corner of three surfaces. \~
    \param[in] keepForm   - \ru Автоопределение сохранения кромки (ts_neutral), сохранение поверхности (ts_negative), сохранение кромки (ts_positive).
                            \en Auto detection of boundary saving (ts_neutral), surface saving (ts_negative), boundary saving (ts_positive). \~
    \param[in] isStrict   - \ru Строгое скругление. Если false, скруглить хотя бы то, что возможно.
                            \en Strict fillet. If false - round at least what is possible. \~
    \param[in] isEquable  - \ru В углах сочленения вставлять тороидальную поверхность.
                            \en In corners of the joint insert toroidal surface. \~
  */
  DEPRECATE_DECLARE_REPLACE ( SmoothValues::Init... )
  SmoothValues( double        d1, 
                double        d2, 
                MbeSmoothForm smoothForm, 
                double        conicValue, 
                bool          bProlong,
                CornerForm    cornerForm, 
                ThreeStates   keepForm, 
                bool          isStrict, 
                bool          isEquable )
    : distance1     ( d1            )
    , distance2     ( d2            )
    , conic         ( conicValue    )
    , begLength     ( UNDEFINED_DBL )
    , endLength     ( UNDEFINED_DBL )
    , form          ( smoothForm    )
    , smoothCorner  ( cornerForm    )
    , prolong       ( bProlong      )
    , keepCant      ( keepForm      )
    , strict        ( isStrict      )
    , equable       ( isEquable     )
    , vector1       (               )
    , vector2       (               )
  { 
    if ( form == st_Fillet && ::fabs(::fabs(d1) - ::fabs(d2)) > METRIC_EPSILON )
      form = st_Ellipse; 
  }

  /// \ru Конструктор копирования. \en Copy-constructor.
  SmoothValues( const SmoothValues & other, MbRegDuplicate * iReg = nullptr );
  /// \ru Деструктор. \en Destructor.
  virtual ~SmoothValues(){}

  /// \ru Функция инициализации. \en Initialization function.
  void        Init( const SmoothValues & other );

  /** \brief \ru Функция инициализации скругления сферой с равными отступами от поверхностей.
             \en Function for initializing filleting with a sphere with equal offsets from surfaces. \~
    \details \ru Функция устанавливает форму и величину отступов, не меняя остальные свойства класса.
                 Первая опорная кривая, отложенная по нормали к первой поверхности на расстояние rad,
                 будет совпадать со второй опорной кривой, отложенной по нормали ко второй 
                 поверхности на расстояние rad.
             \en The function sets the shape and size of the offsets without changing other properties of the class.
                 The first support curve, laid out along the normal to the first surface at a distance of rad, 
                 will coincide with the second support curve, laid out along the normal to the second 
                 surface at a distance of rad.\~ 
    \param[in] rad     - \ru Радиус сферы.
                         \en Radius of a sphere. \~
    \return \ru Возвращает true в случае успеха.
            \en Returns true on success. \~
  */
  bool InitEquidistantFillet( double rad );

  /** \brief \ru Функция инициализации скругления сферой с разными отступами от поверхностей.
             \en Function for initializing filleting with a sphere with different offsets from surfaces. \~
    \details \ru Функция устанавливает форму и величину отступов, не меняя остальные свойства класса.
                 Первая опорная кривая, отложенная по нормали к первой поверхности на расстояние r1,
                 будет совпадать со второй опорной кривой, отложенной по нормали ко второй 
                 поверхности на расстояние r2. Не путать со скруглением эллипсом!
             \en The function sets the shape and size of the offsets without changing other properties of the class.
                 The first support curve, laid out along the normal to the first surface at a distance of r1, 
                 will coincide with the second support curve, laid out along the normal to the second 
                 surface at a distance of r2. Not to be confused with ellipse filleting!\~ 
    \param[in] r1, r2  - \ru Величины отступов.
                         \en Offset values. \~
    \return \ru Возвращает true в случае успеха.
            \en Returns true on success. \~
  */
  bool InitEquidistantFillet( double r1, double r2 );

  /** \brief \ru Функция инициализации скругления сферой с заданной хордой между точками касания.
             \en Function for initializing filleting by a sphere with a given chord between the points of contact of the surfaces. \~
    \details \ru Функция устанавливает форму и величину отступов, не меняя остальные свойства класса.
             \en The function sets the shape and size of the offsets without changing other properties of the class. \~
    \param[in] chord  - \ru Величин хорды.
                         \en Chord value. \~
    \return \ru Возвращает true в случае успеха.
            \en Returns true on success. \~
  */
  bool InitChordFillet( double chord );


  /** \brief \ru Функция инициализации скругления "дугой эллипса"
             \en Function for initializing the rounding by "ellipse arc" \~
    \details \ru Функция устанавливает форму и величину отступов, не меняя остальные свойства класса.
                 Каждое сечение скругления представляет собой эллипс с полуосями a и b, оринтированный
                 таким образом, чтобы большая полуось была направлена по нормали к соответствующей поверхности.
                 Не путать со скруглением сферой с разными отступами! 
             \en The function sets the shape and size of the offsets without changing other properties of the class.
                 Each fillet section is an ellipse with semi-axes a and b, oriented so that the major semi-axis 
                 is directed along the normal to the corresponding surface. Not to be confused with a sphere fillet with different offsets!\~ 
    \param[in] a, b  - \ru Полуоси эллипса.
                       \en Semi-axes of an ellipse. \~
    \return \ru Возвращает true в случае успеха.
            \en Returns true on success. \~
  */
  bool InitEllipseFillet( double a, double b ) ;

  /** \brief \ru Функция инициализации фаски с равными отступами.
             \en Function for initializing a chamfer with equal offsets. \~
    \details \ru Функция устанавливает форму и величину отступов, не меняя остальные свойства класса.
             \en The function sets the shape and size of the offsets without changing other properties of the class. \~
    \param[in] dist    - \ru Величина отступа.
                         \en Offset value. \~
    \return \ru Возвращает true в случае успеха.
            \en Returns true on success. \~
  */
  bool InitChamfer( double dist );

  /** \brief \ru Функция инициализации фаски с разными отступами.
             \en Function for initializing a chamfer with different offsets. \~
    \details \ru Функция устанавливает форму и величину отступов, не меняя остальные свойства класса.
             \en The function sets the shape and size of the offsets without changing other properties of the class. \~
    \param[in] dist1, dist2    - \ru Величины отступов.
                                 \en Offset values. \~
    \return \ru Возвращает true в случае успеха.
            \en Returns true on success. \~
  */
  bool InitChamfer( double dist1, double dist2 );

  /** \brief \ru Функция инициализации фаски по катету и углу.
             \en Function for initializing chamfers by cathetus and angle. \~
    \details \ru Функция устанавливает форму и величину отступов, не меняя остальные свойства класса.
             \en The function sets the shape and size of the offsets without changing other properties of the class. \~
    \param[in] firstSide    - \ru true - если катет задан для первой поверхности.
                              \en true - if the cathetus is specified for the first surface. \~
    \param[in] dist    - \ru Величина катета.
                         \en Cathetus value. \~
    \param[in] ang    - \ru Величина угла в градусах (от 0 до 90).
                        \en The angle value in degrees (from 0 to 90). \~
    \return \ru Возвращает true в случае успеха.
            \en Returns true on success. \~
  */
  bool InitOneSideChamfer( bool firstSide, double dist, double ang );

  /** \brief \ru Расчет угла по двум катетам.
             \en Calculation of an angle using two cathetus. \~
    \param[in] d1    - \ru Прилежащий катет.
                       \en Adjacent cathetus. \~
    \param[in] d2    - \ru Противолежащий катет.
                       \en Opposite cathetus. \~
    \param[in] ang    - \ru Величина угла в градусах (от 0 до 90).
                        \en The angle value in degrees (from 0 to 90). \~
    \return \ru Возвращает угол в градусах.
            \en Returns angle value in degrees. \~
  */
  static double GetAngleBySide( double d1, double d2 );


  /** \brief \ru Функция модификации величины отступов.
             \en Function for modifying the size of offsets. \~
    \details \ru Функция меняет отступы в соответствии с установленной формой в функциях инициализации.
                 Для скругления хордой используется только первый отступ.
                 Для фаски по катету и углу будет произведен перерасчет угла по двум катетам
                 Для остальных форм будут установлены переданные в функцию отступы.
             \en The function changes the offsets in accordance with the form set in the initialization functions. 
                 For chord filleting, only the first offset is used. For chamfering by cathetus and angle, the angle 
                 will be recalculated by two cathetus. For other forms, the offsets passed to the function will be set. \~
    \param[in] d1, d2    - \ru Величины отступов.
                           \en Offset values. \~
    \return \ru Возвращает true в случае успеха.
            \en Returns true on success. \~
  */
  bool ModifyDistances( double d1, double d2 );

  /** \brief \ru Установка опций операции.
             \en Setting operation options. \~
    \param[in] c      - \ru Коэффициент формы, изменяется от 0.05 до 0.95 (при 0 - дуга окружности).
                        \en Coefficient of shape is changed from 0.05 to 0.95 (if 0 - circular arc). \~
    \param[in] pro    - \ru Продолжить по касательной.
                        \en Prolong along the tangent. \~
    \param[in] cor    - \ru Способ скругления "чемоданных" углов.
                        \en Method for bending corner of three surfaces. \~
    \param[in] autoS  - \ru Автоопределение сохранения кромки/поверхности.
                        \en Auto detection of boundary/surface saving. \~
    \param[in] keep   - \ru Сохранять кромку (true) или сохранять поверхность скругления/фаски (false).
                        \en Keep boundary (true) or keep surface of fillet/chamfer (false). \~
    \param[in] str    - \ru Строгое скругление. Если false, скруглить хотя бы то, что возможно.
                        \en Strict fillet. If false - round at least what is possible. \~
    \param[in] equ    - \ru В углах сочленения вставлять тороидальную поверхность.
                        \en In corners of the joint insert toroidal surface. \~
  */
  void SetProperties( double c, bool pro, CornerForm cor, bool autoS, bool keep, bool str, bool equ);

  /// \ru Преобразовать объект согласно матрице. \en Transform an object according to the matrix.
  virtual void    Transform( const MbMatrix3D &, MbRegTransform * ireg = nullptr );
  /// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector.
  virtual void    Move     ( const MbVector3D &, MbRegTransform * /*ireg*/ = nullptr ){}
  /// \ru Повернуть объект вокруг оси на заданный угол. \en Rotate an object at a given angle around an axis.
  virtual void    Rotate   ( const MbAxis3D &, double ang, MbRegTransform * ireg = nullptr );

  /// \ru Установить плоскость, параллельно которой будет выполнена остановка скругления в начале цепочки. \en Set the plane by which parallel will be carry out stop of the fillet at the begin.
  bool        SetStopObjectAtBeg( const MbSurface * object, bool byObject = true );
  /// \ru Установить плоскость, параллельно которой будет выполнена остановка скругления в конце цепочки.  \en Set the plane by which parallel will be carry out stop of the fillet at the end.
  bool        SetStopObjectAtEnd( const MbSurface * object, bool byObject = true );
  /// \ru Установить вектор нормали к плоскости остановки скругления в начале цепочки. \en Set normal to the bound plane at the begin.
  void        SetBegVector( const MbVector3D & vect ) { vector1.Init( vect ); }
  MbVector3D & SetBegVector() { return vector1; }
  /// \ru Установить вектор нормали к плоскости остановки скругления в конце цепочки.  \en Set normal to the bound plane at the end.
  void        SetEndVector( const MbVector3D & vect ) { vector2.Init( vect ); }
  MbVector3D & SetEndVector() { return vector2; }
  /// \ru Получить вектор нормали к плоскости остановки в начале скругления. \en Get normal vector to the bound plane at the begin of the fillet.
  void        GetBegVector( MbVector3D & vect ) const { vect.Init( vector1 ); }
  /// \ru Получить вектор нормали к плоскости остановки в конце скругления. \en Get normal vector to the bound plane at the end of the fillet.
  void        GetEndVector( MbVector3D & vect ) const { vect.Init( vector2 ); }
  /// \ru Дать радиус кривизны/катет на первой поверхности. \en Get radius of curvature/leg on the first surface.
  double      GetDistance1() const { return distance1; }
  /// \ru Дать радиус кривизны/катет на второй поверхности. \en Get radius of curvature/leg on the second surface.
  double      GetDistance2() const { return distance2; }
  
  ///< \ru Тип сопряжения скругление/фаска. \en Mate type of fillet/chamfer.
  MbeSmoothForm GetForm() const { return form; }

  ///< \ru Способ обработки углов стыковки трёх рёбер. \en Method of processing corners of connection by three edges.
  CornerForm  GetSmoothCorner() const { return smoothCorner; }
  CornerForm &SetSmoothCorner() { return smoothCorner; }
  void        SetSmoothCorner( CornerForm f ) { smoothCorner = f; }
  
  ///< \ru Продолжить по касательной. \en Prolong along the tangent.
  bool        GetProlong() const { return prolong; }
  bool &      SetProlong() { return prolong; }
  void        SetProlong( bool p ) { prolong = p; }
  
  ///< \ru Автоопределение сохранения кромки (ts_neutral), сохранение поверхности (ts_negative), сохранение кромки (ts_positive). \en Auto detection of boundary saving (ts_neutral), surface saving (ts_negative), boundary saving (ts_positive).
  ThreeStates GetKeepCant() const { return keepCant; }
  ThreeStates&SetKeepCant() { return keepCant; }
  void        SetKeepCant( ThreeStates s ) { keepCant = s; }
  
  ///< \ru При false скруглить хотя бы то, что возможно. \en If false - round at least what is possible.
  bool        GetStrict() const { return strict; }
  bool &      SetStrict() { return strict; }
  void        SetStrict( bool s ) { strict = s; }
  
  ///< \ru В углах сочленения вставлять тороидальную поверхность (для штамповки листового тела). \en In corners of the joint insert toroidal surface (for stamping sheet solid).
  bool        GetEquable() const { return equable; }
  bool &      SetEquable() { return equable; }
  void        SetEquable( bool e ) { equable = e; }

  ///< \ru Расстояние от начала скругления до точки остановки \en Distance from the beginning of fillet to the stop point.
  double      GetBegLength() const { return begLength; }
  double &    SetBegLength() { return begLength; }
  
  ///< \ru Расстояние от конца  скругления до точки остановки. \en Distance from the end of fillet to the stop point.  
  double      GetEndLength() const { return endLength; }
  double &    SetEndLength() { return endLength; }
  
  ///< \ru Коэффициент формы\en Coefficient of shape.
  double      GetConic() const { return conic; }
  double &    SetConic() { return conic; }

  /// \ru Оператор присваивания. \en Assignment operator.
  SmoothValues & operator = ( const SmoothValues & other ) {
    Init( other );
    return *this;
  }
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        IsSame( const SmoothValues & other, double accuracy ) const;
  /// \ru Являются ли объекты равными по части параметров. \en Determine whether objects are partially equal.
  bool        IsSimilar ( const SmoothValues & other, double accuracy = PARAM_EPSILON ) const {
    return ::fabs(distance1 - other.distance1) < accuracy &&
           ::fabs(distance2 - other.distance2) < accuracy &&
           ::fabs(conic - other.conic) < accuracy &&
           keepCant == other.keepCant;
  }

public:
KNOWN_OBJECTS_RW_REF_OPERATORS( SmoothValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры скругления грани.
           \en Parameters of face fillet. \~
  \details \ru Параметры скругления грани содержат информацию, необходимую для выполнения операции. \n
           \en The parameters of face fillet contain Information necessary to perform the operation. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS FullFilletValues {
public:
  bool        prolong; ///< \ru Продолжить по касательной. \en Prolong along the tangent.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  FullFilletValues()
    : prolong       ( true )
  {}

  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  FullFilletValues( bool prlg )
    : prolong       ( prlg )
  {}

  /// \ru Конструктор копирования. \en Copy-constructor.
  FullFilletValues( const FullFilletValues & other, MbRegDuplicate * iReg = nullptr );

  /// \ru Деструктор. \en Destructor.
  ~FullFilletValues(){}
public:
  /// \ru Функция инициализации. \en Initialization function.
  void        Init( const FullFilletValues & other );
  /// \ru Преобразовать объект согласно матрице. \en Transform an object according to the matrix.
  void        Transform( const MbMatrix3D &, MbRegTransform * ireg = nullptr );
  /// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector.
  void        Move     ( const MbVector3D &, MbRegTransform * /*ireg*/ = nullptr ){}
  /// \ru Повернуть объект вокруг оси на заданный угол. \en Rotate an object at a given angle around an axis.
  void        Rotate   ( const MbAxis3D &, double ang, MbRegTransform * ireg = nullptr );

  /// \ru Оператор присваивания. \en Assignment operator.
  FullFilletValues & operator = ( const FullFilletValues & other ) {
    Init( other );
    return *this;
  }
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        IsSame( const FullFilletValues & other, double accuracy ) const; 

public:
KNOWN_OBJECTS_RW_REF_OPERATORS( FullFilletValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры скругления вершины.
           \en Parameters of vertex fillet. \~
  \details \ru Параметры скругления вершины, в которой стыкуются три ребра, содержат информацию, необходимую для выполнения операции. \n
           \en Fillet parameters of vertex (where three edges are connected) contain information necessary to perform the operation \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS CornerValues {
public:
  /// \ru Способы скругления вершины стыковки трёх рёбер. \en Methods of vertices fillet of connection by three edges.
  enum CornerForm {
    ef_sphere = 0,  ///< \ru Скругление вершины сферической поверхностью. \en Vertex fillet by spherical surface.
    ef_smart  = 1,  ///< \ru Скругление вершины гладкой поверхностью. \en Vertex fillet by smooth surface.
    ef_delta  = 3,  ///< \ru Скругление вершины треугольной поверхностью. \en Vertex fillet by triangular surface.
    ef_elbow1 = 4,  ///< \ru Скругление вершины четырёхугольной поверхностью, четвёртую сторону располагать напротив range1. \en Vertex fillet by quadrangular surface, the fourth side is opposite the range1.
    ef_elbow2 = 5,  ///< \ru Скругление вершины четырёхугольной поверхностью, четвёртую сторону располагать напротив range2. \en Vertex fillet by quadrangular surface, the fourth side is opposite the range2.
    ef_elbow3 = 6,  ///< \ru Скругление вершины четырёхугольной поверхностью, четвёртую сторону располагать напротив range3. \en Vertex fillet by quadrangular surface, the fourth side is opposite the range3.
  };

public:
  double     radius0;    ///< \ru Радиус сферы в вершине. \en Radius of the sphere of the vertex.
  double     radius1;    ///< \ru Радиус первого  ребра вершины. \en Radius of the first edge of the vertex.
  double     radius2;    ///< \ru Радиус второго  ребра вершины. \en Radius of the second edge of the vertex.
  double     radius3;    ///< \ru Радиус третьего ребра вершины. \en Radius of the third edge of the vertex.
  CornerForm cornerForm; ///< \ru Способ скругления вершины стыковки трёх рёбер. \en Method of vertex fillet of connection by three edges.
  uint8      additive;   ///< \ru Сдвиг в нумерации рёбер вершины (добавка к номеру ребра). \en Shift in the indexation of vertex edges (addition to the index of edge).

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  CornerValues()
    : radius0   ( 0.0 )
    , radius1   ( 1.0 )
    , radius2   ( 1.0 )
    , radius3   ( 1.0 )
    , cornerForm( ef_smart )
    , additive  ( 0 )
  {}
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  CornerValues( double r0, double r1, double r2, double r3, CornerForm ck )
    : radius0   ( ::fabs(r0) )
    , radius1   ( ::fabs(r1) )
    , radius2   ( ::fabs(r2) )
    , radius3   ( ::fabs(r3) )
    , cornerForm( ck )
    , additive  ( 0 )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  CornerValues( const CornerValues & other )
    : radius0   ( other.radius0    )
    , radius1   ( other.radius1    )
    , radius2   ( other.radius2    )
    , radius3   ( other.radius3    )
    , cornerForm( other.cornerForm )
    , additive  ( other.additive   )
  {}
  /// \ru Деструктор. \en Destructor.
  virtual ~CornerValues();

  /// \ru Функция инициализации. \en Initialization function.
  void        Init( const CornerValues & other ) {
    radius0    = other.radius0;
    radius1    = other.radius1;
    radius2    = other.radius2;
    radius3    = other.radius3;
    cornerForm = other.cornerForm;
    additive   = other.additive;
  }
  /// \ru Циклическая перестановка параметров. \en Cyclic permutation of the parameters.
  void        CiclicSwap( bool increase );
  /// \ru Поменять местами радиусы (constRadius = 1,2,3). \en Swap radii (constRadius = 1,2,3).
  void        Swap( int constRadius );
  /// \ru Оператор присваивания. \en Assignment operator.
  CornerValues & operator = ( const CornerValues & other ) {
    Init( other );
    return *this;
  }
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        IsSame( const CornerValues & other, double accuracy ) const; 

KNOWN_OBJECTS_RW_REF_OPERATORS( CornerValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//------------------------------------------------------------------------------
/** \brief \ru Типы выемки.
           \en Types of notch. \~
  \details \ru Типы выемки. Служат для определения одного из построений: отверстий, карманов, пазов. \n
           \en Types of notch. These are used to determine one from the constructions: holes, pockets, grooves. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
enum MbeHoleType {
  ht_BorerValues  = 0, ///< \ru Отверстие. \en Hole.
  ht_PocketValues = 1, ///< \ru Карман. \en Pocket.
  ht_SlotValues   = 2, ///< \ru Паз. \en Slot.
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры выемки.
           \en The parameters of notch. \~
  \details \ru Общие параметры построения выемки: отверстия, фигурного паза, кармана (бобышки). \n
           \en The common parameters of notch construction: holes, figure slot, pocket (boss). \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS HoleValues {
public:
  double          placeAngle;     ///< \ru Угол между осью и нормалью к поверхности (0 <= placeAngle <= M_PI_2). \en Angle between axis and normal to the surface (0 <= placeAngle <= M_PI_2).
  double          azimuthAngle;   ///< \ru Угол поворота оси вокруг нормали поверхности (-M_PI2 <= azimuthAngle <= M_PI2). \en Angle of rotation around the surface normal (-M_PI2 <= azimuthAngle <= M_PI2).
protected:
  MbSurface *     surface;        ///< \ru Обрабатываемая поверхность (если nullptr, то считается плоской). \en Processing surface (if nullptr, then is considered planar).
  bool            doPhantom;      ///< \ru Создавать фантом результата операции. \en Create the phantom of the operation.
  MbMergingFlags  boolMergeFlags; ///< \ru Управляющие флаги слияния элементов оболочки при выполнении булевой операции с предыдущим телом. \en Control flags of shell items merging when performing a boolean operation with the previous body.

protected:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
    \details \ru Конструктор параметров выемки с нулевыми углами и плоской поверхностью.
             \en Constructor of notch parameters with zero angles and planar surfaces. \~
  */
  HoleValues();

  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default.
  HoleValues( const HoleValues & other );
  /// \ru Конструктор копирования. \en Copy-constructor.
  HoleValues( const HoleValues & other, MbRegDuplicate * iReg );

public:
  /// \ru Деструктор. \en Destructor.
  virtual ~HoleValues();

public:
  /// \ru Тип выемки. \en Type of notch.
  virtual MbeHoleType Type() const = 0;
  /// \ru Построить копию объекта. \en Create a copy of the object.
  virtual HoleValues & Duplicate( MbRegDuplicate * = nullptr ) const = 0;
  /// \ru Преобразовать объект согласно матрице. \en Transform an object according to the matrix.
  virtual void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) = 0;
  /// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector.
  virtual void        Move     ( const MbVector3D &, MbRegTransform * = nullptr );
  /// \ru Повернуть объект вокруг оси на заданный угол. \en Rotate an object at a given angle around an axis.
  virtual void        Rotate   ( const MbAxis3D &, double ang, MbRegTransform * = nullptr );

  /// \ru Являются ли объекты равными? \en Determine whether an object is equal?
  virtual bool        IsSame( const HoleValues &, double accuracy ) const;

  /// \ru Оператор присваивания. \en Assignment operator.
  virtual void        operator = ( const HoleValues & ) = 0;

public:
          /// \ru Функция копирования. \en Copy function.
          void              Init( const HoleValues & );
          /// \ru Получить поверхность. \en Get the surface.
  const   MbSurface *       GetSurface() const { return surface; }
          /// \ru Получить поверхность для редактирования. \en Get the surface for editing.
          MbSurface *       SetSurface()       { return surface; }
          /// \ru Заменить поверхность. \en Replace surface.
          void              SetSurface( MbSurface * s );
          /// \ru Установить флаг создания фантома. \en Set the phantom flag.
          void              SetPhantom( bool s ) { doPhantom = s; }
          /// \ru Получить флаг создания фантома. \en Get the phantom flag.
          bool              GetPhantom() const   { return doPhantom; }

          /// \ru Сливать подобные грани (true). \en Whether to merge similar faces (true). 
          bool              MergeFaces() const { return boolMergeFlags.MergeFaces(); }
          /// \ru Сливать подобные ребра (true). \en Whether to merge similar edges (true). 
          bool              MergeEdges() const { return boolMergeFlags.MergeEdges(); }
          /// \ru Получить флаги слияния подобных элементов. \en Get flags of merging. 
  const   MbMergingFlags &  GetMerging() const { return boolMergeFlags; }
          /// \ru Сливать подобные грани. \en Whether to merge similar faces. 
          void              SetMergingFaces( bool s ) { boolMergeFlags.SetMergingFaces( s ); }
          /// \ru Сливать подобные ребра. \en Whether to merge similar edges. 
          void              SetMergingEdges( bool s ) { boolMergeFlags.SetMergingEdges( s ); }
          /// \ru Сливать подобные элементы. \en Whether to merge similar items. 
          void              SetMerging( const MbMergingFlags & f ) { boolMergeFlags.SetMerging( f ); }
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры отверстия.
           \en The hole parameters. \~
  \details \ru Параметры для построения отверстий различных типов. \n
    Законцовка отверстия управляется параметром spikeAngle.
    При #spikeAngle = 0    - сферическая законцовка отверстия, \n
    при #spikeAngle = M_PI - плоская законцовка отверстия, \n
    в остальных случаях   - коническая законцовка отверстия. \n
           \en The parameters for construction of holes with different types. \n
    Tip of hole is controlled by the spikeAngle parameter.
    If # spikeAngle = 0 - spherical tip of hole, \n
    If # spikeAngle = M - planar tip of hole, \n
    in other cases - conical tip of hole. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS BorerValues : public HoleValues {
public:
  /** \brief \ru Типы отверстий.
             \en Types of holes. \~
    \details \ru Тип определяет форму отверстия.
             \en The type determines the hole shape. \~
    \ingroup Shell_Building_Parameters
  */
  enum BorerType {
                             //
                             //    _______________
                             //   /|             |
                             //  +-+-------------+
    bt_SimpleCylinder  = 0,  ///< \ru Простое цилиндрическое отверстие. \en Simple cylindrical hole.
                             //                 __
                             //    _____________||
                             //   /|            ||
                             //  +-+------------++
    bt_TwofoldCylinder = 1,  ///< \ru Двойное цилиндрическое отверстие. \en Double cylindrical hole.
                             //                  /
                             //    _____________/|
                             //   /|            ||
                             //  +-+------------++
    bt_ChamferCylinder = 2,  ///< \ru Цилиндрическое отверстие с фаской. \en Cylindrical hole with a chamfer.
                             //               ____
                             //    __________/|  |
                             //   /|         ||  |
                             //  +-+---------++--+
    bt_ComplexCylinder = 3,  ///< \ru Двойное цилиндрическое отверстие с переходом. \en Double cylindrical hole with a transition.
                             //
                             //    _______________
                             //   /|             |
                             //  +-+-------------+
    bt_SimpleCone      = 4,  ///< \ru Простое коническое отверстие. \en Simple conical hole.
                             //                  |
                             //    ____________ /|
                             //   /|         |   |
                             //  +-+---------+---+
    bt_ArcCylinder     = 5,  ///< \ru Центровое отверстие формы R (дугообразное). \en Center hole of form R (arcuate).
                             //                 __
                             //                 ||
                             //    ____________/||
                             //   /|          | ||
                             //  +-+----------+-++
    bt_DoubleCylinder  = 6,  ///< \ru Двойное цилиндрическое отверстие со скруглением. \en Double cylindrical hole with a fillet.
  };

public:
  double         capDiameter; ///< \ru Диаметр головки                          (для отверстий типа #bt_TwofoldCylinder, #bt_ChamferCylinder, #bt_ComplexCylinder, #bt_DoubleCylinder). \en Diameter cap (for hole with type #bt_TwofoldCylinder, #bt_ChamferCylinder, #bt_ComplexCylinder, #bt_DoubleCylinder).
  double         capDepth;    ///< \ru Глубина под головку                      (для отверстий типа #bt_TwofoldCylinder, #bt_ComplexCylinder, #bt_DoubleCylinder). \en Depth for cap (for hole with type #bt_TwofoldCylinder, #bt_ChamferCylinder, #bt_ComplexCylinder, #bt_DoubleCylinder).
  double         capAngle;    ///< \ru Угол фаски под головку                   (для отверстий типа #bt_ChamferCylinder, #bt_ComplexCylinder), capAngle <= M_PI. \en Chamfer angle for cap (for holes with type #bt_ChamferCylinder, #bt_ComplexCylinder), capAngle <= M_PI.
  double         diameter;    ///< \ru Диаметр отверстия под резьбу             (для всех типов отверстий). \en Hole diameter for thread (for all the types of holes).
  double         depth;       ///< \ru Глубина отверстия под резьбу             (для всех типов отверстий). \en Hole depth for thread (for all the types of holes).
  double         angle;       ///< \ru Угол конусности отверстия под резьбу     (для отверстия типа #bt_SimpleCone), 0 < angle < M_PI. \en Angle of hole conicity for thread (for hole with type #bt_SimpleCone), 0 < angle < M_PI.
  double         spikeAngle;  ///< \ru Угол раствора конца отверстия            (для всех типов отверстий), spikeAngle <= M_PI. \en Apex angle of the hole end               (for all the types of holes), spikeAngle <= M_PI.
  double         arcRadius;   ///< \ru Радиус дуги или скругления               (для отверстия типа #bt_ArcCylinder и #bt_DoubleCylinder). \en Arc radius or Fillet radius  (for hole with type #bt_ArcCylinder and #bt_DoubleCylinder).
  bool           prolong;     ///< \ru Флаг продления сверла в обратную сторону (для всех типов отверстий), по умолчанию true (есть продление). \en Flag of drill extension along the opposite direction (for all the types of holes), default true (the extension exists).
  bool           down;        ///< \ru Направление оси отверстия: true - прямое (против оси Z локальной системы), false - обратное. \en Direction of hole axis: true - forward (opposite to the Z of the local system), false - backward.
  BorerType      type;        ///< \ru Тип отверстия. \en Type of hole.
  bool           addThrAttr;  ///< \ru Добавить атрибуты резьбы (не пишется). \en Add thread attribute (not writeable).

private :
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default.
  BorerValues( const BorerValues & other );
  /// \ru Конструктор копирования. \en Copy-constructor.
  BorerValues( const BorerValues & other, MbRegDuplicate * ireg );
public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
    \details \ru Конструктор простого цилиндрического отверстия.
             \en Constructor of simple cylindrical hole. \~
  */
  BorerValues()
    : HoleValues ()
    , capDiameter( 20.0 )
    , capDepth   ( 5.0 )
    , capAngle   ( M_PI_2 )
    , diameter   ( 10.0 )
    , depth      ( 25.0 )
    , angle      ( M_PI_2 )
    , spikeAngle ( M_PI * c3d::TWO_THIRD )
    , arcRadius  (  5.0 )
    , prolong    ( true )
    , down       ( true )
    , type       ( bt_SimpleCylinder )
    , addThrAttr ( false )
  {}

  /// \ru Деструктор. \en Destructor.
  virtual ~BorerValues();

public:
  virtual MbeHoleType Type() const; // \ru Тип выемки. \en Type of notch.
  virtual HoleValues & Duplicate( MbRegDuplicate * ireg = nullptr ) const; // \ru Построить копию. \en Create a copy.
  virtual void        Transform( const MbMatrix3D & matr, MbRegTransform * ireg = nullptr ); // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  virtual bool        IsSame( const HoleValues &, double accuracy ) const; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  virtual void        operator = ( const HoleValues & other ); // \ru Оператор присваивания. \en Assignment operator.
private:
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default.
          void        operator = ( const BorerValues & other );

public:
KNOWN_OBJECTS_RW_REF_OPERATORS( BorerValues ) // \ru Для работы со ссылками и объектами класса \en For treatment of references and objects of the class
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры кармана или бобышки.
           \en The parameters of pocket or boss. \~
  \details \ru Параметры прямоугольного кармана или бобышки со скруглёнными углами. \n
           \en The parameters of rectangular pocket or boss with rounded corners. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS PocketValues : public HoleValues {
public:
  double        length;       ///< \ru Длина   кармана или бобышки. \en The length of pocket or boss.
  double        width;        ///< \ru Ширина  кармана или бобышки. \en The width of pocket or boss.
  double        depth;        ///< \ru Глубина кармана или бобышки. \en The depth of pocket or boss.

  /** \brief \ru Радиус скругления углов кармана или бобышки.
             \en Fillet radius of corners of pocket or boss. \~
    \details \ru Радиус скругления углов кармана или бобышки, 2 * cornerRadius <= std_min( width, length ).
             При length == width == 2 * cornerRadius получим карман в виде отверстия.
             \en Fillet radius of corners of pocket or boss, 2 * cornerRadius <= std_min( width, length ).
             If length == width == 2 * cornerRadius, then pocket as a hole. \~
  */
  double        cornerRadius;

  double        floorRadius;  ///< \ru Радиус скругления дна кармана или верха бобышки. \en Fillet radius of bottom of pocket or top of boss.
  double        taperAngle;   ///< \ru Угол уклона стенок кармана или верха бобышки (отклонение от вертикали в радианах). \en Draft angle of pocket walls or top of boss (vertical deviation in radians)
  bool          type;         ///< \ru type == false - карман, type == true - бобышка. \en Type == false - pocket, type == true - boss.

private :
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default.
  PocketValues( const PocketValues & other );
  /// \ru Конструктор копирования. \en Copy-constructor.
  PocketValues( const PocketValues & other, MbRegDuplicate * ireg );

public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
    \details \ru Конструктор кармана.
             \en Constructor of pocket. \~
  */
  PocketValues()
    : HoleValues  ()
    , length      ( 20.0 )
    , width       ( 10.0 )
    , depth       (  5.0 )
    , cornerRadius(  2.0 )
    , floorRadius (  1.0 )
    , taperAngle  (  0.0 )
    , type        ( false )
  {}

  /// \ru Деструктор. \en Destructor.
  virtual ~PocketValues();

public:
  virtual MbeHoleType Type() const; // \ru Тип выемки. \en Type of notch.
  virtual HoleValues & Duplicate( MbRegDuplicate * ireg = nullptr ) const; // \ru Построить копию. \en Create a copy.
  virtual void        Transform( const MbMatrix3D & matr, MbRegTransform * ireg = nullptr ); // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  virtual bool        IsSame( const HoleValues &, double accuracy ) const; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  virtual void        operator = ( const HoleValues & other ); // \ru Оператор присваивания. \en Assignment operator.

private:
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default.
          void        operator = ( const PocketValues & other );

public:
KNOWN_OBJECTS_RW_REF_OPERATORS( PocketValues ) // \ru Для работы со ссылками и объектами класса \en For treatment of references and objects of the class
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры паза.
           \en The parameters of slot. \~
  \details \ru Параметры фигурного паза. \n
    Вид паза сверху представляет собой разрезанную пополам окружность,
    половинки которой раздвинуты на длину паза, а края соединены отрезками.
           \en The parameters of figure slot. \n
    View of slot from above is cut in half to circle,
    halves of which are spread apart by the length of slot and the edges are connected by segments. \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS SlotValues : public HoleValues {
public:
                             /* \ru Вид паза сверху. \en View of slot from above. \~
                             //          --
                             //        /    \
                             //       |      |
                             //       |      |
                             //       |      |
                             //       |      |
                             //        \    /
                             //          --
                             */
  enum SlotType {
                             //       ________         *
                             //       |      |         *
                             //       +------+         *
                             //        \    /          *
                             //          --            *
    st_BallEnd     = 0,  ///< \ru Цилиндрический в донной части. \en Cylindrical in the bottom part.
                             //       ________         *
                             //       |      |         *
                             //       |      |         *
                             //       |      |         *
                             //       +------+         *
    st_Rectangular = 1,  ///< \ru Прямоугольный. \en Rectangular.
                             //       ________         *
                             //       |      |         *
                             //    +--+------+--+      *
                             //    |            |      *
                             //    +------------+      *
    st_TShaped     = 2,  ///< \ru T-образный. \en T-shaped.
                             //       ________         *
                             //      /        \        *
                             //     /          \       *
                             //    /            \      *
                             //   +--------------+     *
    st_DoveTail    = 3,  ///< \ru Ласточкин хвост. \en Dovetail
  };

public:
  double        length;      ///< \ru Длина   паза. \en Slot length.
  double        width;       ///< \ru Ширина  паза. \en Slot width.
  double        depth;       ///< \ru Глубина паза. \en Slot depth.
  double        bottomWidth; ///< \ru Ширина донной части T-образного паза, должна превосходить ширину width. \en Width of the bottom part of T-shaped slot must be greater than the width "width".
  double        bottomDepth; ///< \ru Глубина донной части ласточкиного хвоста. \en Depth of the bottom part of dovetail.

  /** \brief \ru Радиус скругления дна паза.
             \en Fillet radius of the slot bottom. \~
    \details \ru Радиус скругления дна паза (2 * floorRadius <= width).
             При width == 2 * floorRadius получим паз типа st_BallEnd.
             floorRadius = 0 для пазов типа st_TShaped и st_DoveTail.
             \en Fillet radius of slot bottom (2 * floorRadius <= width).
             If width == 2 * floorRadius, then slot has type st_BallEnd.
             floorRadius = 0 for slots with type st_TShaped and st_DoveTail. \~
  */
  double        floorRadius;
  double        tailAngle;   ///< \ru Угол уклона стенок паза типа st_DoveTail (отклонение от вертикали в радианах). \en Draft angle of walls of slot with type st_DoveTail (vertical deviation in radians).
  SlotType      type;        ///< \ru Тип паза. \en Type of slot.

private :
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default.
  SlotValues( const SlotValues & other );
  /// \ru Конструктор копирования. \en Copy-constructor.
  SlotValues( const SlotValues & other, MbRegDuplicate * ireg );

public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
    \details \ru Конструктор прямоугольного паза.
             \en Constructor of rectangular slot. \~
  */
  SlotValues()
    : HoleValues ()
    , length     ( 10.0  )
    , width      ( 10.0  )
    , depth      (  5.0  )
    , bottomWidth( 15.0  )
    , bottomDepth( 10.0  )
    , floorRadius(  1.0  )
    , tailAngle  ( M_PI_4 )
    , type       ( st_Rectangular )
  {}

  /// \ru Деструктор. \en Destructor.
  virtual ~SlotValues();

public:
  virtual MbeHoleType Type() const; // \ru Тип выемки. \en Type of notch.
  virtual HoleValues & Duplicate( MbRegDuplicate * ireg = nullptr ) const; // \ru Построить копию. \en Create a copy.
  virtual void        Transform( const MbMatrix3D & matr, MbRegTransform * ireg = nullptr ); // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  virtual bool        IsSame( const HoleValues &, double accuracy ) const; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  virtual void        operator = ( const HoleValues & other ); // \ru Оператор присваивания. \en Assignment operator.

private:
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default.
          void        operator = ( const SlotValues & other );

public:
KNOWN_OBJECTS_RW_REF_OPERATORS( SlotValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//------------------------------------------------------------------------------
/** \brief    \ru Параметры крепежа.
              \en The parameters of fastener elements. \~
    \details  \ru Параметры крепежных элементов. \n
              \en The parameters of fastener elements. \n \~
*/
// ---
class MATH_CLASS FastenersValues {
public:
  //------------------------------------------------------------------------------
  /** \brief  \ru Типы крепежа.
              \en Fastener Types. \~
  */
  // ---
  enum MbeFastenerType {
    ft_CountersunkHeadRivet = 0,    ///< \ru Заклепка с (полу)потайной головкой. \en (semi)Countersunk head rivet.  
    ft_UniversalHeadRivet,          ///< \ru Заклепка с универсальной головкой. \en Universal head rivet.
    ft_RoundHeadRivet,              ///< \ru Заклепка с полукруглой головкой. \en Round head rivet.
    ft_FlatHeadRivet                ///< \ru Заклепка с плоской головкой. \en Flat head rivet.
  };
private:
  MbeFastenerType fastenerType; ///< \ru Тип крепежа. \en Fastener type.
  double diameter;              ///< \ru Диаметр крепежа. \en Fastener diameter.
  double angle;                 ///< \ru Угол фаски. \en Countersunk angle.
  double depth;                 ///< \ru Глубина фаски. \en Depth of chamfer.
  double headDiameter;          ///< \ru Диаметр основания головки. \en Diameter of the head base.
  double headHeight;            ///< \ru Высота головки. \en Head height.

  ThreeStates rivetAndHole;     ///< \ru ts_negative - создать только отверстия (без крепежа), ts_neutral - создать только крепёж (без отверстий), ts_positive - создать крепеж и отверстия.
                                ///< \en ts_negative - create holes only (without rivets), ts_neutral - create rivets only (without holes), ts_positive - create rivets and holes. \~

public:
  /** \brief  \ru Конструктор крепежа по типу и диаметру.
              \en Constructor of fastener based on type and diameter. \~
  \details \ru Конструктор крепежа по типу и диаметру.
           \en Constructor of fastener based on type and diameter. \~
  \param[in] ft - \ru Тип крепежа.
                  \en Fastener type. \~
  \param[in] d -  \ru Диаметр крепежа.
                  \en Fastener diameter. \~
  */
  FastenersValues( MbeFastenerType ft, double d ) 
    : fastenerType      ( ft )
    , diameter          ( d  )
    , angle             ( M_PI_4  )
    , depth             ( d * c3d::ONE_HALF )
    , headDiameter      ( 2 * d )
    , headHeight        ( d )
    , rivetAndHole      ( ts_positive )
  {}

  /** \brief  \ru Конструктор крепежа по типу, диаметру, углу, катету.
              \en Constructor of fastener based on type, diameter, angle and side length. \~
  \details \ru Конструктор крепежа по типу, диаметру, углу, катету.
           \en Constructor of fastener based on type, diameter, angle and side length. \~
  \param[in] ft   -   \ru Тип крепежа.
                      \en Fastener type. \~
  \param[in] d    -   \ru Диаметр крепежа.
                      \en Fastener diameter. \~
  \param[in] a    -   \ru Угол.
                      \en Angle. \~
  \param[in] dd   -   \ru Глубина (фаски).
                      \en Depth. \~
  \param[in] hd  -    \ru Диаметр основания головки.
                      \en Head base diameter \~
  \param[in] hh   -   \ru Высота головки.
                      \en Head height. \~
  
  \param[in] ho -  \ru Создать только отверстие.
                   \en Create hole only. \~
  */
  FastenersValues( MbeFastenerType ft, double d, double a, double dd, double hd, double hh, ThreeStates ho ) 
    : fastenerType      ( ft  )
    , diameter          ( d   )
    , angle             ( a   )
    , depth             ( dd  )
    , headDiameter      ( hd )
    , headHeight        ( hh  )
    , rivetAndHole      ( ho )
  {}

  /// \ru Выдать тип крепежа. \en Return type of fastener.
  MbeFastenerType GetType() const { return fastenerType; }
  /// \ru Выдать значение диаметра. \en Return diameter value.
  double GetDiameter() const { return diameter; }
  /// \ru Установить значение диаметра. \en Set diameter value.
  void SetDiameter( double d ) { diameter = d; }
  /// \ru Выдать значение угла. \en Return angle value.
  double GetAngle() const { return angle; }
  /// \ru Установить значение угла. \en Set angle value.
  void SetAngle( double a ) { angle = a; }
  /// \ru Выдать значение глубины фаски. \en Return chamfer depth value.
  double GetDepth() const { return depth; }
  /// \ru Установить значение глубины фаски. \en Set chamfer depth value.
  void SetDepth( double d ) { depth = d; }
  /// \ru Выдать значение диаметра основания головки. \en Return chamfer depth value.
  double GetHeadDiameter() const { return headDiameter; }
  /// \ru Установить значение диаметра основания головки. \en Set chamfer depth value.
  void SetHeadDiameter( double hd ) { headDiameter = hd; }
  /// \ru Выдать значение высоты головки. \en Return head height value.
  double GetHeadHeight() const { return headHeight; }
  /// \ru Установить значение высоты головки. \en Set head height value.
  void SetHeadHeight( double hh ) { headHeight = hh; }

  /// \ru Только отверстие? \en Hole only?
  ThreeStates RivetAndHole() const { return rivetAndHole; }
  /// \ru Функция инициализации. \en Initialization function.
  void   Init( const FastenersValues & other ) {
    fastenerType      = other.fastenerType;
    diameter          = other.diameter;
    angle             = other.angle;
    depth             = other.depth;
    headDiameter      = other.headDiameter;
    headHeight        = other.headHeight;
    rivetAndHole      = other.rivetAndHole;
  }
  /// \ru Оператор присваивания. \en Assignment operator.
  FastenersValues & operator = ( const FastenersValues & other ) {
    Init( other );
    return *this;
  }

private:
  /// \ru Конструктор по умолчанию - запрещен. \en Default constructor - forbidden.
  FastenersValues() 
  {}

public:
KNOWN_OBJECTS_RW_REF_OPERATORS( FastenersValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


class MbPatchCurveMating;


//------------------------------------------------------------------------------
/** \brief \ru Сопряжение по кривой заплатки.
           \en Patch curve conjugation. \~
  \details \ru Сопряжение по кривой заплатки. \n
           \en Patch curve conjugation. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbPatchMating {

protected:
  /// \ru Конструктор. \en Constructor.
  MbPatchMating() {}
  /// \ru Конструктор. \en Constructor.
  MbPatchMating( const MbPatchMating & ) {}
public:
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbPatchMating() {}

public:
  /// \ru Установить тип сопряжения. \en Set conjugation type.
  virtual void SetMate( MbePatchMatingType newType, const MbSurface * newSurface ) = 0;
  /// \ru Установить тип сопряжения сегмента. \en Set conjugation type of segment.
  virtual void SetMate( size_t segInd, MbePatchMatingType newType, const MbSurface * newSurface ) = 0;
  /// \ru Установить тип сопряжения по кривой на поверхности или контуру из кривых на поверхности. \en Set conjugation type to a curve on a surface or a contour from curves on a surface.
  virtual bool SetMateByCurve( MbePatchMatingType newType, const MbCurve3D & curve ) = 0;

  /// \ru Выдать тип сопряжения. \en Get the type of conjugation.
  virtual MbePatchMatingType GetMatingType() const = 0;
  /// \ru Выдать поверхность. \en Get surface.
  virtual const MbSurface * GetSurface() const = 0;

  /// \ru Сопряжение для сегмента номер segInd. \en The conjugation by segment number segInd.
  virtual const MbPatchCurveMating * GetSegmentMate( size_t segInd ) const = 0;
  /// \ru Чье сопряжение. \en The conjugation owner type.
  virtual MbeSpaceType GetOwnerType() const = 0;

  /// \ru Являются ли объекты равными. \en Determine whether the objects are same.
  virtual bool IsSame( const MbPatchMating & other, double accuracy ) const = 0;
  /// \ru Являются ли объекты подобными. \en Determine whether the objects are similar.
  virtual bool IsSimilar( const MbPatchMating & other ) const = 0;
  /// \ru Сделать равными. \en Make equal. 
  virtual bool SetEqual( const MbPatchMating & other ) = 0;
  /// \ru Преобразовать элементы согласно матрице. \en Transform elements according to the matrix. 
  virtual void Transform( const MbMatrix3D & matr, MbRegTransform * iReg ) = 0;
  /// \ru Сдвинуть вдоль объекты вектора. \en Translate objects along a vector. 
  virtual void Move( const MbVector3D & to, MbRegTransform * iReg ) = 0;
  /// \ru Повернуть вокруг оси. \en Rotate about an axis. 
  virtual void Rotate( const MbAxis3D & axis, double angle, MbRegTransform * iReg ) = 0;
  
private:
  MbPatchMating & operator = ( const MbPatchMating & );
};


//------------------------------------------------------------------------------
/** \brief \ru Кривая с сопряжением.
           \en Curve with conjugation. \~
  \details \ru Кривая с сопряжением для построения заплатки. \n
           \en Curve with conjugation for the patch construction. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbCurveMate: public MbRefItem,
                              public TapeBase {
protected:
  c3d::SpaceCurveSPtr _curve;        ///< \ru Кривая. \en A curve.
  DPtr<MbPatchMating> _mating;       ///< \ru Сопряжение. \en The conjugation.

public:
  /// \ru Конструктор по кривой. \en Constructor by a curve.
  MbCurveMate( MbCurve3D & );
  /// \ru Конструктор по копии кривой. \en Constructor by a copy of curve.
  MbCurveMate( const MbCurve3D &, MbRegDuplicate * iReg );
  /// \ru Конструктор по копии кривой и сопряжению. \en Constructor by a copy of curve and conjugation.
  MbCurveMate( const MbCurve3D & crv, const MbPatchMating & mating, MbRegDuplicate * iReg );
  /// \ru Конструктор по кривой (копирует кривую, трансформируя по матрице). \en Constructor by a curve (copies a curve, transforms by the matrix).
  MbCurveMate( const MbCurve3D &, const MbMatrix3D & );
  /// \ru Конструктор по ребру (копирует кривую, трансформируя по матрице). \en Constructor by an edge (copies a curve, transforms by the matrix).
  MbCurveMate( const MbEdge &, const MbMatrix3D & );
  /// \ru Конструктор копирования. \en Copy constructor.
  MbCurveMate( const MbCurveMate & other, MbRegDuplicate * ireg );

  /// \ru Деструктор. \en Destructor.
  virtual ~MbCurveMate();

  /// \ru Получить кривую. \en Get a curve.
  const MbCurve3D & GetCurve() const { return *_curve; }
  /// \ru Получить кривую для изменения. \en Get a curve for changing.
        MbCurve3D & SetCurve()       { return *_curve; }

  /// \ru Выдать тип сопряжения. \en Get the type of conjugation.
  MbePatchMatingType GetMatingType() const { return  _mating->GetMatingType(); }
  /// \ru Выдать поверхность сопряжения. \en Get surface of conjugation.
  const MbSurface * GetMatingSurface() const { return _mating->GetSurface(); }

  /// \ru Установить сопряжение. \en Set the conjugation.
  void SetMating( const MbPatchMating & other ) { _mating->SetMate( other.GetMatingType(), other.GetSurface() ); }
  /// \ru Установить сопряжение. \en Set the conjugation.
  void SetMating( MbePatchMatingType type, const MbSurface * surface ) { _mating->SetMate( type, surface ); }
  /// \ru Установить сопряжение сегмента. \en Set the conjugation of segment.
  void SetMating( size_t segInd, MbePatchMatingType type, const MbSurface * surface ) { _mating->SetMate( segInd, type, surface ); }
  /// \ru Установить тип сопряжения по кривой на поверхности или контуру из кривых на поверхности. \en Set conjugation type to a curve on a surface or a contour from curves on a surface.
  bool SetMating( MbePatchMatingType newType, const MbCurve3D & curve ) { return _mating->SetMateByCurve( newType, curve ); }

        MbPatchMating & SetMating()       { return *_mating; } ///< Получить сопряжение. \en Get the conjugation.
  const MbPatchMating & GetMating() const { return *_mating; } ///< Получить сопряжение. \en Get the conjugation.

        MbCurveMate & Duplicate( MbRegDuplicate * iReg ) const; /// \ru Сделать копию элемента. \en Create a copy of the element.

  /// \ru Являются ли объекты равными. \en Determine whether the objects are same.
  bool IsSame( const MbCurveMate & other, double accuracy ) const;
  /// \ru Являются ли объекты подобными. \en Determine whether the objects are similar.
  bool IsSimilar( const MbCurveMate & other ) const;
  /// \ru Сделать равными. \en Make equal. 
  bool SetEqual( const MbCurveMate & other );
  /// \ru Преобразовать элементы согласно матрице. \en Transform elements according to the matrix. 
  void Transform( const MbMatrix3D & matr, MbRegTransform * iReg );
  /// \ru Сдвинуть вдоль объекты вектора. \en Translate objects along a vector. 
  void Move( const MbVector3D & to, MbRegTransform * iReg );
  ///  \ru Повернуть вокруг оси. \en Rotate about an axis.
  void Rotate( const MbAxis3D & axis, double angle, MbRegTransform * iReg );

private:
  // \ru Инициализация сопряжения. \en Mating initialization.
  void InitMating();
  // \ru Инициализация сопряжения. \en Mating initialization.
  void InitMating( const MbPatchMating & other );

OBVIOUS_PRIVATE_COPY( MbCurveMate )
DECLARE_PERSISTENT_CLASS( MbCurveMate )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры заплатки.
           \en The parameters of patch. \~
  \details \ru Параметры заплатки. \n
    Содержат информацию о типе заплатки и флаге проверки самопересечений.
    Если задан тип #ts_plane, будет построена заплатка плоскими гранями. \n
    Тип #ts_byCurves нужно задавать, если требуется построить заплатку с сопряжением по некоторым границам.
    В этом случае для нужной границы #MbPatchCurve нужно задать сопряжение методом #SetMating.
           \en The parameters of patch. \n
    Contain Information about type of patch and flag of checking self-intersection. 
    If the #ts_plane type is given, the patch will be built with flat faces. \n
    The #ts_byCurves type must be set if it is required to build a patch with conjugation along some boundaries.
    In this case, for the desired #MbPatchCurve border, you need to set the pairing using the #SetMating method.\~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS PatchValues {
public:
  /** \brief \ru Тип заплатки.
             \en Type of patch. \~
    \details \ru Флаг можно установить через вызов PatchValues::SetType(). 
             \en The flag can be set by calling PatchValues::SetType(). \~
  */
  enum SurfaceType {
    ts_tang,         ///< \ru По касательной. \en Along the tangent.
    ts_norm,         ///< \ru По нормали. \en Along the normal.
    ts_none,         ///< \ru Не определено. \en Undefined.
    ts_plane,        ///< \ru Плоская заплатка. \en Plane patch.
    ts_byCurves,     ///< \ru Построение задается сопряжениями по каждой кривой. \en The construction is defined by conjugations on curves.
  };
private:
  SurfaceType        type;                ///< \ru Тип заплатки. \en Type of patch.
  bool               checkSelfInt;        ///< \ru Флаг проверки самопересечений (вычислительно "тяжелыми" методами). \en Flag for checking of self-intersection (computationally by "heavy" methods).
  bool               mergeEdges;          ///< \ru Сливать подобные ребра (true). \en Whether to merge similar edges (true). 
  bool               tolerantData;        ///< \ru Построить неточную заплатку по неточным входным данным. \en Build an tolerant patch from tolerant input data.
  MbePatchMatingType internalMatingType;  ///< \ru Тип сопряжения между частями заплатки. Используется при type = ts_byCurves. \en The type of mating between parts of the patch. Used when type = ts_byCurves. \warning \ru В разработке. \en Under development. \~

public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
    \details \ru Конструктор параметров заплатки не определенного типа без проверки самопересечений.
             \en Constructor of parameters of patch with undefined type and without checking of self-intersection. \~
  */
  PatchValues()
    : type            ( ts_none  )
    , checkSelfInt    ( false    )
    , mergeEdges      ( true     )
    , tolerantData    ( false    )
    , internalMatingType( pmt_None )
  {}

  /// \ru Конструктор копирования. \en Copy-constructor.
  PatchValues( const PatchValues & other )
    : type            ( other.type             )
    , checkSelfInt    ( other.checkSelfInt     )
    , mergeEdges      ( other.mergeEdges       )
    , tolerantData    ( other.tolerantData     )
    , internalMatingType( other.internalMatingType )
  {}

  /// \ru Деструктор. \en Destructor.
  ~PatchValues()
  {}

public:
  /// \ru Выдать тип заплатки. \en Get type of patch.
  SurfaceType GetType() const { return type; }
  /// \ru Выдать тип заплатки для изменения. \en Get type of patch for changing.
  SurfaceType & SetType()     { return type; }
  /// \ru Получить флаг проверки самопересечений. \en Get the flag of checking self-intersection.
  bool        CheckSelfInt() const { return checkSelfInt; }
  /// \ru Установить флаг проверки самопересечений. \en Set the flag of checking self-intersection.
  void        SetCheckSelfInt( bool c ) { checkSelfInt = c; }
  /// \ru Сливать подобные ребра (true)? \en Whether to merge similar edges (true)? 
  bool        MergeEdges() const { return mergeEdges; }
  /// \ru Сливать подобные ребра. \en Whether to merge similar edges. 
  void        SetMergingEdges( bool s ) { mergeEdges = s; }

  /// \ru Выдать флаг построения неточной заплатки по неточным входным данным. \en Get the flag for building an tolerant patch from tolerant input data.
  bool        IsTolerantData() const { return tolerantData; }
  /// \ru Установить флаг построения неточной заплатки по неточным входным данным. \en Set the flag for building an tolerant patch from tolerant input data.
  void        SetTolerantData( bool tolData ) { tolerantData = tolData;  }

  /// \ru Выдать тип сопряжения между кусками заплатки. \en Get the type of mating between pieces of the patch.
  MbePatchMatingType  GetInternalMatingType() const { return internalMatingType; }
  /// \ru Установить тип сопряжения между кусками заплатки. \en Set the type of mating between pieces of the patch.
  void                SetInternalMatingType( MbePatchMatingType matingType ) { internalMatingType = matingType;  }

  reader &    Read ( reader & in,        std::vector<DPtr<MbPatchMating>> & matings );
  writer &    Write( writer & out, const std::vector<SPtr<MbCurveMate>> & curves ) const;

  /// \ru Оператор присваивания. \en Assignment operator.
  void        operator = ( const PatchValues & other ) { 
    type             = other.type; 
    checkSelfInt     = other.checkSelfInt;
    mergeEdges       = other.mergeEdges; 
    tolerantData     = other.tolerantData;
    internalMatingType = other.internalMatingType;
  }

  /// \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSame( const PatchValues & obj, double ) const {
    return ( (obj.type             == type) && 
             (obj.checkSelfInt     == checkSelfInt) &&
             (obj.mergeEdges       == mergeEdges) &&
             (obj.tolerantData     == tolerantData) &&
             (obj.internalMatingType == internalMatingType) );
  }

KNOWN_OBJECTS_RW_REF_OPERATORS( PatchValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//------------------------------------------------------------------------------
/** \brief \ru Кривая для построения заплатки.
           \en Curve for the patch construction. \~
  \details \ru Кривая для построения заплатки и параметры её окружения. \n
           \en Curve for the patch construction and parameters of its environment. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbPatchCurve : public MbRefItem {
private:
  SPtr<MbCurveMate> curve;        ///< \ru Кривая. \en A curve.
  double            begTolerance; ///< \ru Толерантность привязки в начале. \en Binding tolerance at the start.
  double            endTolerance; ///< \ru Толерантность привязки в начале. \en Binding tolerance at the start.
  bool              isSurfaceOne; ///< \ru В ребре есть грань с первой поверхностью из кривой пересечения. \en There is face with the first surface from the intersection curve in the edge.
  bool              isSurfaceTwo; ///< \ru В ребре есть грань со второй поверхностью из кривой пересечения. \en There is face with the second surface from the intersection curve in the edge.
  mutable bool      isUsed;       ///< \ru Флаг использования. \en An using flag.

public:
  /// \ru Конструктор по кривой (копирует кривую, трансформируя по матрице). \en Constructor by a curve (copies a curve, transforms by the matrix).
  MbPatchCurve( const MbCurve3D &, const MbMatrix3D & );
  /// \ru Конструктор по ребру (копирует кривую, трансформируя по матрице). \en Constructor by an edge (copies a curve, transforms by the matrix).
  MbPatchCurve( const MbCurveEdge &, const MbMatrix3D & );
  /// \ru Конструктор по ребру (копирует кривую, трансформируя по матрице). \en Constructor by an edge (copies a curve, transforms by the matrix).
  MbPatchCurve( const MbEdge &, const MbMatrix3D & );
  /// \ru Конструктор по кривой с сопряжением без копирования. \en Curve constructor with mate without copying.
  MbPatchCurve( MbCurveMate & curveMate );
  /// \ru Конструктор копирования с регистратором копий. \en Copy constructor with copy registrar.
  MbPatchCurve( const MbPatchCurve &, MbRegDuplicate * iReg );

  /// \ru Деструктор. \en Destructor.
  virtual ~MbPatchCurve();

public:
  /// \ru В ребре есть грань с первой поверхностью из кривой пересечения. \en There is face with the first surface from the intersection curve in the edge.
  bool        IsSurfOne() const { return isSurfaceOne; }
  /// \ru В ребре есть грань со второй поверхностью из кривой пересечения. \en There is face with the second surface from the intersection curve in the edge.
  bool        IsSurfTwo() const { return isSurfaceTwo; }

  /// \ru Толерантность привязки в начале. \en Binding tolerance at the start.
  double      GetBegTolerance() const { return begTolerance; }
  /// \ru Толерантность привязки в начале. \en Binding tolerance at the start.
  double      GetEndTolerance() const { return endTolerance; }

  /// \ru Получить кривую. \en Get a curve.
  const MbCurve3D & GetCurve() const { return curve->GetCurve(); }
  /// \ru Получить кривую для изменения. \en Get a curve for changing.
  MbCurve3D & SetCurve()       { return curve->SetCurve(); }
  /// \ru Кривая используется? \en Is curve used?
  bool        IsUsed() const { return isUsed; }
  /// \ru Установить флаг использования кривой. \en Set flag of using curve.
  void        SetUsed( bool b ) const { isUsed = b; }
  
  const MbCurveMate * GetCurveWithMate() const { return curve; }              ///< \ru Получить кривую. \en Get a curve.
        MbCurveMate * SetCurveWithMate()       { return curve; }              ///< \ru Получить кривую. \en Get a curve.
  const MbPatchMating &   GetMating()    const { return curve->GetMating(); } ///< \ru Получить сопряжение. \en Get the conjugation.
        MbPatchMating &   SetMating()          { return curve->SetMating(); } ///< \ru Получить сопряжение. \en Get the conjugation.

  /// \ru Установить сопряжение. \en Set the conjugation.
  void SetMating( MbePatchMatingType type, const MbSurface * surface )                { curve->SetMating( type, surface ); }
  /// \ru Установить сопряжение сегмента. \en Set the conjugation of segment.
  void SetMating( size_t segInd, MbePatchMatingType type, const MbSurface * surface ) { curve->SetMating( segInd, type, surface ); }

  /** \brief \ru Максимальный тип сопряжения, который может быть задан.
             \en The maximum mating type that can be set. \~
    \details \ru Максимальный тип сопряжения, который может быть задан для данной границы и данной поверхности. \n
             \en The maximum mating type that can be specified for the given curve and given surface. \n \~
    \param[in] prevCurve - \ru Предыдущая граница заплатки.
                           \en The previous patch boundary. \~
    \param[in] nextCurve - \ru Следующая граница заплатки.
                           \en The next patch boundary. \~
    \param[in] matingSurface - \ru Поверхность для сопряжения.
                               \en Mating surface. \~
    \return \ru pmt_None     - Если нельзя задать сопряжение к данной поверхности для данной границы.
                pmt_Tangent  - Если можно задать сопряжение типа pmt_Tangent.
                pmt_SmoothG2 - Если можно задать сопряжения типов pmt_Tangent и pmt_SmoothG2.
            \en pmt_None     - If it is impossible to set a mating to a given surface for a given boundary.
                pmt_Tangent  - If you can specify a pairing of type pmt_Tangent.
                pmt_SmoothG2 - If you can specify a pairings of types pmt_Tangent and pmt_SmoothG2. \~
  */
  MbePatchMatingType GetMaximumMatingType( const MbPatchCurve & prevCurve, 
                                           const MbPatchCurve & nextCurve,
                                           const MbSurface &    matingSurface ) const;

  /// \ru Разбить контур на сегменты. \en Divide the path into segments.
  void Decompose( std::vector<SPtr<MbPatchCurve>> & patchCurves ) const;
private:
  /// \ru Установить толерантность привязки в начале. \en Set binding tolerance at the start.
  void        SetBegTolerance( double tolerance ) { begTolerance = tolerance; }
  /// \ru Установить толерантность привязки в начале. \en Set binding tolerance at the start.
  void        SetEndTolerance( double tolerance ) { endTolerance = tolerance; }

OBVIOUS_PRIVATE_COPY( MbPatchCurve )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры операции "Заплатка".
           \en Patch operation parameters. \~
    \details \ru Параметры операции "Заплатка". \n
                 Содержат информацию о границах заплатки, параметрах построения PatchValues, именователе. \n
             \en Patch operation parameters. \n 
                 Contains information about patch boundaries, PatchValues ​​build parameters, object defining names generation in the operation. \n \~
    \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbPatchShellParams: public MbPrecision
{
private:
  std::vector<SPtr<MbPatchCurve>> _curves;    ///< \ru Набор кривых. \en A set of curves.
  PatchValues                     _values;    ///< \ru Параметры операции. \en The operation parameters.
  SPtr<MbSNameMaker>              _names;     ///< \ru Именователь. \en An object for naming the new objects.

  MbPatchShellParams(); ///< \ru Конструктор по умолчанию. Запрещён. \en Default constructor. Forbidden.

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по набору кривых и именователю. \n
             \en Constructor by set of curves and object defining names generation in the operation. \n \~
    \param[in] initCurves - \ru Набор кривых.
                            \en A set of curves. \~
    \param[in] patchValues - \ru Параметры заплатки.
                             \en Patch parameters. \~
    \param[in] names - \ru Именователь.
                       \en An object defining names generation in the operation. \~
  */
  MbPatchShellParams( const std::vector<SPtr<MbPatchCurve>> & initCurves,
                      const PatchValues &                     patchValues,
                      const MbSNameMaker &                    names );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по набору кривых, параметрам и именователю. \n
             \en Constructor by set of curves, parameters and object defining names generation in the operation. \n \~
    \param[in] initCurves - \ru Набор кривых.
                            \en A set of curves. \~
    \param[in] patchValues - \ru Параметры заплатки.
                             \en Patch parameters. \~
    \param[in] names - \ru Именователь.
                       \en An object defining names generation in the operation. \~
  */
  MbPatchShellParams( const RPArray<MbPatchCurve> & initCurves,
                      const PatchValues &           patchValues,
                      const MbSNameMaker &          names );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по набору кривых, параметрам и именователю. \n
             \en Constructor by set of curves, parameters and object defining names generation in the operation. \n \~
    \param[in] initCurves - \ru Набор кривых.
                            \en A set of curves. \~
    \param[in] patchValues - \ru Параметры заплатки.
                             \en Patch parameters. \~
    \param[in] names - \ru Именователь.
                       \en An object defining names generation in the operation. \~
  */
  MbPatchShellParams( const RPArray<MbCurve3D> & initCurves,
                      const PatchValues &        patchValues,
                      const MbSNameMaker &       names );

  /// \ru Конструктор копирования. \en Copy constructor.
  MbPatchShellParams( const MbPatchShellParams & other );

  /// \ru Получить набор кривых. \en Get the curves.
  void GetCurves( std::vector<SPtr<const MbPatchCurve>> & curves ) const;
 
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker &                    GetNameMaker()  const { return *_names; }

  /// \ru Получить параметры операции. \en Get the operation parameters.
  const PatchValues &                     GetValues()     const { return _values; }
  /// \ru Взять параметры операции для изменения. \en Get the operation parameters for changes.
        PatchValues &                     SetValues()           { return _values; }

  /// \ru Оператор присваивания. \en Assignment operator.
  const MbPatchShellParams & operator = ( const MbPatchShellParams & );

private:
  /// \ru Инициализация набора кривых. \en Initialization of curves.
  template<class CurvesVector>
  void InitPatchCurves  ( const CurvesVector & initCurves );

  /// \ru Инициализация набора кривых. \en Initialization of curves.
  template<class CurvesVector>
  void CreatePatchCurves( const CurvesVector & initCurves );
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры масштабирования объекта.
           \en The parameters of object scaling. \~
  \details \ru Масштабирование объекта выполняется преобразованием по матрице. \n
           \en Object scaling is performed by the transformation matrix. \n \~
  \ingroup Direct_Building_Parameters
*/
// ---
struct MATH_CLASS TransformValues {
protected:
  MbMatrix3D    matrix;     ///< \ru Матрица преобразования . \en A transformation matrix.
  // \ru Остальные параметры не обязательны (нужны для расчета matrix по деформации габаритного куба функцией MbCube::CalculateMatrix) \en Other parameters are optional (they are necessary for the calculation of matrix by deformation of bounding box by the function MbCube::CalculateMatrix)
  MbCartPoint3D fixedPoint; ///< \ru Неподвижная точка преобразования (используется, если useFixed = true). \en A fixed point of transformation. (It is used if useFixed = true).
  bool          useFixed;   ///< \ru Использовать неподвижную точку преобразования (если true). \en Use fixed point of transformation (if true).
  bool          isotropy;   ///< \ru Использовать одинаковое масштабирование по осям (если true). \en Use the same axes scaling (if true).

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  TransformValues()
    : matrix()
    , fixedPoint()
    , useFixed( false )
    , isotropy( false )
  {}
  /// \ru Конструктор по матрице. \en Constructor by matrix.
  TransformValues( const MbMatrix3D & m )
    : matrix( m )
    , fixedPoint()
    , useFixed( false )
    , isotropy( false )
  {}
  /// \ru Конструктор по матрице и неподвижной точке преобразования. \en Constructor by matrix and fixed point of transformation.
  TransformValues( const MbMatrix3D & m, const MbCartPoint3D & f, bool fix = false, bool iso = false )
    : matrix( m )
    , fixedPoint( f )
    , useFixed( fix )
    , isotropy( iso )
  {}
  /// \ru Конструктор по неподвижной точке преобразования и масштабам по осям. \en Constructor by fixed point of transformation and axes scale.
  TransformValues( double sX, double sY, double sZ, const MbCartPoint3D & fP );
  /// \ru Конструктор. \en Constructor.
  TransformValues( const TransformValues & other )
    : matrix ( other.matrix )
    , fixedPoint ( other.fixedPoint )
    , useFixed ( other.useFixed )
    , isotropy ( other.isotropy )
  {}
  /// \ru Деструктор. \en Destructor.
  ~TransformValues() {}
public:
  /// \ru Функция инициализации. \en Initialization function.
  void        Init( const TransformValues & other ) {
    matrix = other.matrix;
    fixedPoint = other.fixedPoint;
    useFixed = other.useFixed;
    isotropy = other.isotropy;
  }
  /// \ru Оператор присваивания. \en Assignment operator.
  TransformValues & operator = ( const TransformValues & other ) {
    matrix = other.matrix;
    fixedPoint = other.fixedPoint;
    useFixed = other.useFixed;
    isotropy = other.isotropy;
    return *this;
  }

  /// \ru Выдать матрицу преобразования для использования. \en Get a transformation matrix for use.
  const MbMatrix3D & GetMatrix() const { return matrix; }
  /// \ru Выдать неподвижную точку преобразования для использования. \en A fixed point of transformation for use.
  const MbCartPoint3D & GetFixedPoint() const { return fixedPoint; } 
  /// \ru Использовать неподвижную точку преобразования?. \en Is fixed point use?
  bool        IsFixed() const { return useFixed; } 
  /// \ru Одинаковое масштабирование по осям? \en Is the isotropic scaling?
  bool        Isisotropy() const { return isotropy; }

  /// \ru Выдать матрицу преобразования для редактирования. \en Get a transformation matrix for modify.
  MbMatrix3D & SetMatrix() { return matrix; }
  /// \ru Выдать неподвижную точку преобразования для редактирования. \en A fixed point of transformation for modify.
  MbCartPoint3D & SetFixedPoint() { return fixedPoint; } 
  /// \ru Использовать неподвижную точку преобразования. \en Use fixed point of transformation.
  void        SetFixed( bool b ) { useFixed = b; } 
  /// \ru Использовать одинаковое масштабирование по осям. \en Use the same axes scaling.
  void        SetIsotropy( bool b ) { isotropy = b; }  

  /// \ru Используется ли неподвижная точка преобразования? \en Whether the fixed point of transformation is used?
  bool        IsUsingFixed() const { return useFixed; }
  /// \ru Является ли преобразование изотропным? \en Whether the transformation is isotropic?
  bool        IsIsotropy()   const { return isotropy; }
  /// \ru Рассчитать неподвижную точку преобразования. \en Calculate a fixed point of transformation.
  bool        CalculateFixedPoint();
  /// \ru Преобразовать объект согласно матрице. \en Transform an object according to the matrix.
  void        Transform( const MbMatrix3D & matr );
  /// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector.
  void        Move     ( const MbVector3D & to );
  /// \ru Повернуть объект вокруг оси на заданный угол. \en Rotate an object at a given angle around an axis.
  void        Rotate   ( const MbAxis3D & axis, double ang );
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        IsSame( const TransformValues & other, double accuracy ) const; 

KNOWN_OBJECTS_RW_REF_OPERATORS( TransformValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры деформируемой грани.
           \en Parameters of the deformable face. \~
  \details \ru Параметры деформируемой грани используются при замене поверхности выбранной грани тела
    NURBS-поверхностью и при дальнейшем редактировании этой грани. \n
           \en Parameters of the deformable face are used when replacing the surface of selected face of solid
    by NURBS-surface and with further editing of this face. \n \~
  \ingroup Direct_Building_Parameters
*/
// ---
struct MATH_CLASS NurbsValues {
public:
  MbNurbsParameters uParameters; ///< \ru Параметры u-направления NURBS-поверхности. \en Parameters of u-direction of NURBS-surface.
  MbNurbsParameters vParameters; ///< \ru Параметры v-направления NURBS-поверхности. \en Parameters of v-direction of NURBS-surface.

public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
    \details \ru Конструктор параметров деформированной грани для замены
      поверхности NURBS-поверхностью 4 порядка по всей области определения по направлениям u и v.
             \en Constructor of parameters of deformed face for replacement
      of surface by NURBS-surface of the fourth order in the entire domain along the u and v directions. \~
  */
  NurbsValues()
    : uParameters()
    , vParameters()
  {}

  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор параметров деформированной грани.
             \en Constructor of parameters of deformed face. \~
    \param[in] ud,         vd         - \ru Порядок NURBS-копии по u и по v.
                                        \en Order of NURBS-copy along u and v. \~
    \param[in] uc,         vc         - \ru Количество контрольных точек по u и по v.
                                        \en The count of control points along u and v. \~
    \param[in] umin, umax, vmin, vmax - \ru Диапазоны параметров по u и v для деформирования грани.
                                        \en Parameter ranges along u and v for deforming face. \~
    \param[in] uapprox,    vapprox    - \ru Флаги возможного построения приближенной поверхности, а не точной.
                                        \en Flags of the possible constructing of approximate surface, not exact. \~
  */
  NurbsValues( size_t ud, size_t uc, double umin, double umax, bool uapprox,
               size_t vd, size_t vc, double vmin, double vmax, bool vapprox )
    : uParameters( ud, uc, umin, umax, uapprox )
    , vParameters( vd, vc, vmin, vmax, vapprox )
  {}

  /// \ru Конструктор копирования. \en Copy-constructor.
  NurbsValues( const NurbsValues & other )
    : uParameters( other.uParameters )
    , vParameters( other.vParameters )
  {}
  /// \ru Деструктор. \en Destructor.
  ~NurbsValues() {}

public:
  /// \ru Функция копирования. \en Copy function.
  void        Init( const NurbsValues & other ) {
    uParameters = other.uParameters;
    vParameters = other.vParameters;
  }
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        IsSame( const NurbsValues & other, double accuracy ) const; 
  /// \ru Оператор присваивания. \en Assignment operator.
  NurbsValues & operator = ( const NurbsValues & other ) {
    uParameters = other.uParameters;
    vParameters = other.vParameters;
    return *this;
  }

KNOWN_OBJECTS_RW_REF_OPERATORS( NurbsValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//--------------------------------------------------------------------
/** \brief \ru Параметры для построения NURBS-блока.
           \en The parameters for construction of NURBS-block. \~
  \details \ru Параметры для построения блока из NURBS-поверхностей. \n
           \en The parameters for construction of block.from NURBS-surfaces. \n \~
  \ingroup Direct_Building_Parameters
*/
// ---
struct MATH_CLASS NurbsBlockValues {
public:
  // \ru Параметры для построения блока из nurbs-поверхностей. \en The parameters for construction of block.from nurbs-surfaces.
  //
  // \ru +Z   (N) - номер грани   *-----* \en +Z   (N) - index of face   *-----*
  //         |                       |     |
  //         *--------*              | (5) |
  //        /|       /|              |     |
  //       / |      / |        *-----*-----*-----*-----*
  //      /  |     /  |        |     |     |     |     |
  //     *---+----*   |        | (2) | (3) | (4) | (1) |
  //     |   |    |   |        |     |     |     |     |
  //     |   *----+---*-- +Y   *-----*-----*-----*-----*
  //     |  /     |  /               |     |
  //     | /      | /                | (0) |
  //     |/       |/                 |     |
  //     *--------*                  *-----*
  //    /
  // \ru +X  Развертка граней блока внешней стороной к наблюдателю. \en +X  Unfolding the outer side of block faces to the viewer.
  //
  // \ru Принцип соответствия номеров и граней. \en Principle of correspondence of indices and faces.
  // \ru Элементы матрицы структуры соответствуют параметрам поверхностей следующих граней блока: \en Matrix elements of the structure correspond to surfaces parameters the following blocks:
  // \ru - элемент 0 - грани 0, 5 ( нижняя и верхняя грани ); \en - element 0 - faces 0, 5 ( lower and upper faces );
  // \ru - элемент 1 - грани 1, 3 ( боковые грани ); \en - element 1 - faces 1, 3 ( lateral faces );
  // \ru - элемент 2 - грани 2, 4 ( боковые грани ). \en - element 2 - faces 2, 4 ( lateral faces ).

  ptrdiff_t udeg[3]; ///< \ru Порядок nurbs-сплайнов по первому параметру для трех пар поверхностей граней блока. \en Order of nurbs-splines along the first parameter for three pairs of block faces.
  ptrdiff_t vdeg[3]; ///< \ru Порядок nurbs-сплайнов по второму параметру для трех пар поверхностей граней блока. \en Order of nurbs-splines along the second parameter for three pairs of block faces.
  ptrdiff_t ucnt[3]; ///< \ru Количество контрольных точек вдоль первого параметра для трех пар поверхностей граней блока. \en The count of matrix elements of control points along the first and for three pairs of block faces.
  ptrdiff_t vcnt[3]; ///< \ru Количество контрольных точек вдоль второго параметра для трех пар поверхностей граней блока. \en The count of matrix elements of control points along the second and for three pairs of block faces.
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры сплайновой поверхности.
           \en The parameters of spline surface. \~
  \details \ru Параметры определяют контрольные точки, веса, узлы сплайновой поверхности. \n
           \en The parameters determines control points, weights, knots of spline surface. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS NurbsSurfaceValues {
public:
  friend class MbNurbsSurfacesSolid;
private:
  ptrdiff_t                udegree;        ///< \ru Порядок В-сплайна по U. \en Spline degree along U.
  ptrdiff_t                vdegree;        ///< \ru Порядок В-сплайна по V. \en Spline degree along V.
  bool                     uclosed;        ///< \ru Признак замкнутости по U. \en Attribute of closedness along U.
  bool                     vclosed;        ///< \ru Признак замкнутости по V. \en Attribute of closedness along V.
  Array2<MbCartPoint3D>    points;         ///< \ru Множество точек. \en Set of points.
  double                   weight;         ///< \ru Вес точек в случае одинаковости весов. \en Points weight in the case of equal weights.
  Array2<double> *         weights;        ///< \ru Веса точек (может быть nullptr). \en Weights of points (can be nullptr).
  bool                     throughPoints;  ///< \ru Строить поверхность, проходящую через точки. \en Build surface passing through points.
  bool                     pointsCloud;    ///< \ru Облако точек (массив не упорядочен). \en Point cloud (disordered array).
  MbPlane *                cloudPlane;     ///< \ru Опорная плоскость облака точек. \en Support plane of point cloud.
  bool                     ownCloudPlane;  ///< \ru Собственная опорная плоскость облака точек. \en Own support plane of point cloud.
  bool                     checkSelfInt;   ///< \ru Искать самопересечения. \en Find self-intersection.
  mutable CSSArray<size_t> checkLnNumbers; ///< \ru Номера проверяемых строк. \en The indices of checked rows.
  mutable CSSArray<size_t> checkCnNumbers; ///< \ru Номера проверяемых столбцов. \en The indices of checked columns.
  mutable ptrdiff_t        minCloudDegree; ///< \ru Минимально  возможный порядок сплайнов по облаку точек. \en The smallest possible order of splines by point cloud.
  mutable ptrdiff_t        maxCloudDegree; ///< \ru Максимально возможный порядок сплайнов по облаку точек. \en The maximum possible order of splines by point cloud.

public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
    \details \ru Конструктор параметров не замкнутой сплайновой поверхности
      второго порядка по направлениям u и v.
             \en Constructor of parameters of non-closed spline surface
      of second order along the u and v directions. \~
  */
  NurbsSurfaceValues();

  /// \ru Конструктор копирования. \en Copy-constructor.
  NurbsSurfaceValues( const NurbsSurfaceValues & );
  /// \ru Деструктор. \en Destructor.
  ~NurbsSurfaceValues();

public:
  /** \brief \ru Инициализация по сетке точек.
             \en Initialization by grid of points. \~
    \details \ru Инициализация параметров сплайновой поверхности по сетке точек.
             \en Initialization of parameters of spline surface by grid of points. \~
    \param[in] uDeg, vDeg   - \ru Порядок по u и по v.
                              \en Order along u and v. \~
    \param[in] uCls, vCls   - \ru Признаки замкнутости поверхности по u и по v.
                              \en Attribute of surface closedness along u and v. \~
    \param[in] pnts         - \ru Набор точек.
                              \en A point set. \~
    \param[in] checkSelfInt - \ru Признак проверки на самопересечение.
                              \en Attribute of check for self-intersection. \~
    \return \ru false при некорректных параметрах.
            \en False if incorrect parameters. \~
  */
  bool        InitMesh( ptrdiff_t uDeg, bool uCls,
                        ptrdiff_t vDeg, bool vCls,
                        const Array2<MbCartPoint3D> & pnts,
                        bool checkSelfInt );

  /** \brief \ru Инициализация по сетке точек.
             \en Initialization by grid of points. \~
    \details \ru Инициализация параметров сплайновой поверхности по сетке точек.
             \en Initialization of parameters of spline surface by grid of points. \~
    \param[in] uDeg, vDeg   - \ru Порядок по u и по v.
                              \en Order along u and v. \~
    \param[in] uCls, vCls   - \ru Признаки замкнутости поверхности по u и по v.
                              \en Attribute of surface closedness along u and v. \~
    \param[in] pnts         - \ru Набор точек.
                              \en A point set. \~
    \param[in] wts          - \ru Веса точек.
                              \en Weights of points. \~
    \param[in] checkSelfInt - \ru Признак проверки на самопересечение.
                              \en Attribute of check for self-intersection. \~
    \return \ru false при некорректных параметрах.
            \en False if incorrect parameters. \~
  */
  bool        InitMesh( ptrdiff_t uDeg, bool uCls,
                        ptrdiff_t vDeg, bool vCls,
                        const Array2<MbCartPoint3D> & pnts,
                        const Array2<double> * wts, bool checkSelfInt );

  /** \brief \ru Инициализация по облаку точек.
             \en Initialization by point cloud. \~
    \details \ru Инициализация по облаку точек (используется оригинал плоскости).\n
      Если uvDeg < 0, то будет создаваться набор треугольных пластин \n
      (триангуляцией проекций точек на cloudPlace)
             \en Initialization by point cloud (used the original plane).\n
      If uvDeg < 0, then set of triangular plates is created \n
      (by triangulation of points projections on the cloudPlace) \~
    \param[in] uvDeg        - \ru Порядок по u и по v.
                              \en Order along u and v. \~
    \param[in] pnts         - \ru Множество точек.\n
                              Набор точек подходит для инициализации (является облаком точек)
                              в случае, если это одномерный массив точек,
                              не лежащих на одной прямой, без совпадений.
                              \en Set of points.\n
                              Set of points is suitable for initialization (is a point cloud)
                              if it is one-dimensional array of points
                              which don't lie on a straight line without coincidence. \~
    \param[in] cloudPlace   - \ru Опорная плоскость облака точек.
                              \en Support plane of point cloud. \~
    \param[in] checkSelfInt - \ru Признак проверки на самопересечение.
                              \en Attribute of check for self-intersection. \~
    \return \ru true при корректных параметрах.
            \en True if correct parameters. \~
  */
  bool        InitCloud(       ptrdiff_t               uvDeg,
                         const Array2<MbCartPoint3D> & pnts,
                         const MbPlacement3D *         cloudPlace,
                               bool                    checkSelfInt );

  /// \ru Оператор копирования. \en Copy-operator.
  void        operator = ( const NurbsSurfaceValues & );

public:
  /// \ru Первичная проверка корректности параметров. \en Initial check of parameters correctness
  bool        IsValid( bool checkPoints ) const;

  /// \ru Получить порядок сплайнов по U. \en Get splines degree along U.
  ptrdiff_t   GetUDegree() const { return udegree; }
  /// \ru Получить порядок сплайнов по V. \en Get splines degree along V.
  ptrdiff_t   GetVDegree() const { return vdegree; }
  /// \ru Замкнутость по U. \en Closedness along U.
  bool        GetUClosed() const { return uclosed; }
  /// \ru Замкнутость по V. \en Closedness along V.
  bool        GetVClosed() const { return vclosed; }
  /// \ru Количество точек по U. \en A count of points along U.
  size_t      GetUCount() const { return points.Columns(); }
  /// \ru Количество точек по V. \en A count of points along V.
  size_t      GetVCount() const { return points.Lines();   }

  /// \ru Установить порядок сплайна по u. \en Set spline degree along u.
  bool        SetUDegree( size_t uDeg );
  /// \ru Установить порядок сплайна по v. \en Set spline degree along v.
  bool        SetVDegree( size_t vDeg );
  /// \ru Установить замкнутость по U. \en Set closedness along U.
  void        SetUClosed( bool uCls ) { uclosed = uCls; }
  /// \ru Установить замкнутость по V. \en Set closedness along V.
  void        SetVClosed( bool vCls ) { vclosed = vCls; }

  /// \ru Получить точку по позиции. \en Get point by position.
  bool        GetUVPoint ( size_t ui, size_t vi, MbCartPoint3D & ) const;
  /// \ru Получить вес по позиции. \en Get weight by position.
  bool        GetUVWeight( size_t ui, size_t vi, double        & ) const;
  /// \ru Получить общий вес (вернет true, если вес у всех точек одинаковый). \en Get total weight (return true if all the weights are the same).
  bool        GetCommonWeight( double & ) const;
  /// \ru Установить точки по позиции. \en Set points by position.
  bool        SetUVPoint ( size_t ui, size_t vi, const MbCartPoint3D & );
  /// \ru Установить вес по позиции. \en Set weight by position.
  bool        SetUVWeight( size_t ui, size_t vi, const double        & );
  /// \ru Установить общий вес. \en Set total weight.
  bool        SetCommonWeight( double );

  /// \ru Преобразовать данные согласно матрице. \en Transform data according to the matrix.
  void        Transform( const MbMatrix3D &, MbRegTransform * ireg );
  /// \ru Сдвинуть данные вдоль вектора. \en Move data along a vector.
  void        Move     ( const MbVector3D &, MbRegTransform * ireg );
  /// \ru Повернуть данные вокруг оси на заданный угол. \en Rotate data at a given angle around an axis.
  void        Rotate   ( const MbAxis3D   &, double angle, MbRegTransform * ireg );

  bool        IsSame( const NurbsSurfaceValues &, double accuracy ) const; // \ru Являются ли объекты равными? \en Determine whether an object is equal?

  /// \ru Установить размерность массивов точек и весов без сохранения или с сохранением имеющихся данных. \en Set the size of arrays of points and weights without saving or with saving of existing data.
  bool        SetSize( size_t ucnt, size_t vcnt, bool keepData = false );
  /// \ru Установить флаг прохождения поверхности через точки. \en Set flag of surface passing through the points.
  void        SetThroughPoints( bool tp );
  /// \ru Будет ли поверхность проходить через точки? \en Whether the surface passes through the points?
  bool        IsThroughPoints() const { return throughPoints; }
  /// \ru Является ли массив облаком точек? \en Whether the array is point cloud?
  bool        IsPointsCloud() const   { return pointsCloud; }
  /// \ru Используется ли собственная плоскость проецирования (в случае массива по облаку точек)? \en Whether the own plane of projection is used (in the case of array by point cloud)?
  bool        IsOwnCloudPlane() const { return ownCloudPlane; }
  /// \ru Нужно ли проверять самопересечения? \en Whether it is necessary to check self-intersections?
  bool        CheckSelfInt() const { return checkSelfInt; }
  /// \ru Получить массив номеров проверяемых строк. \en Get the array of indices of checked rows.
  void        GetCheckLines( CSSArray<size_t> & checkNumbers ) const { checkNumbers = checkLnNumbers; }
  /// \ru Получить массив номеров проверяемых столбцов. \en Get the array of indices of checked columns.
  void        GetCheckCols ( CSSArray<size_t> & checkNumbers ) const { checkNumbers = checkCnNumbers; }
  /// \ru Получить количество строк. \en Get the count of rows.
  size_t      GetPointsLines  () const { return points.Lines();   } //-V524
  /// \ru Получить количество столбцов. \en Get the count of columns.
  size_t      GetPointsColumns() const { return points.Columns(); } //-V524
  /// \ru Получить массив точек. \en Get array of points.
  bool        GetPoints ( Array2<MbCartPoint3D> & pnts ) const { return pnts.Init( points ); }
  /// \ru Если ли веса? \en Is there weights?
  bool        IsWeighted() const { return (weights != nullptr); }
  /// \ru Получить массив весов. \en Get array of weights.
  bool        GetWeights( Array2<double> & wts ) const;
  /// \ru Получить плоскость проецирования. \en Get the plane of projection.
  const MbPlane * GetCloudPlane() const { return (pointsCloud ? cloudPlane : nullptr); } // SKIP_SA

  /** \brief \ru Минимально возможный порядок сплайнов в случае облака точек.
             \en The smallest possible order of splines in the case of point cloud. \~
    \details \ru Минимально возможный порядок сплайнов в случае облака точек.\n
      Запрашивать после успешного создания поверхности, иначе вернет отрицательное значение.
             \en The smallest possible order of splines in the case of point cloud.\n
      Request after the successful creation of the surface otherwise returns a negative value. \~
  */
  ptrdiff_t   GetMinCloudDegree() const { return minCloudDegree; }

  /** \brief \ru Максимально возможный порядок сплайнов в случае облака точек.
             \en The maximum possible order of splines in the case of point cloud. \~
    \details \ru Максимально возможный порядок сплайнов в случае облака точек.\n
      Запрашивать после успешного создания поверхности, иначе вернет отрицательное значение.
             \en The maximum possible order of splines in the case of point cloud.\n
      Request after the successful creation of the surface otherwise returns a negative value. \~
  */
  ptrdiff_t   GetMaxCloudDegree() const { return maxCloudDegree; }

  /// \ru Выставить максимально возможный порядок по обработанному (регуляризованному) облаку точек. \en Set the maximum possible order by processed (regularized) point cloud.
  bool        SetCloudDegreeRange( const NurbsSurfaceValues & meshParam ) const;

private:
  void        DeleteWeights();
  bool        CreateWeights( double wt );
  void        SetCloudPlane( MbPlane * );
  bool        CreateOwnCloudPlane();

public:
KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( NurbsSurfaceValues, MATH_FUNC_EX )
};


//------------------------------------------------------------------------------
// \ru Получить веса \en Get weights
// ---
inline bool NurbsSurfaceValues::GetWeights( Array2<double> & wts ) const
{
  if ( weights != nullptr ) {
    if ( wts.Init( *weights ) )
      return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru Установить порядок сплайна по u \en Set spline degree along u
// ---
inline bool NurbsSurfaceValues::SetUDegree( size_t uDeg )
{
  if ( uDeg > 1 ) {
    udegree = uDeg;
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru Установить порядок сплайна по v \en Set spline degree along v
// ---
inline bool NurbsSurfaceValues::SetVDegree( size_t vDeg )
{
  if ( vDeg > 1 ) {
    vdegree = vDeg;
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru Получить точку \en Get a point
// ---
inline bool NurbsSurfaceValues::GetUVPoint( size_t ui, size_t vi, MbCartPoint3D & pnt ) const
{
  if ( ui < GetUCount() && vi < GetVCount() ) {
    pnt = points( vi, ui );
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru Установить точку \en Set a point
// ---
inline bool NurbsSurfaceValues::SetUVPoint( size_t ui, size_t vi, const MbCartPoint3D & pnt )
{
  bool bRes = false;

  if ( ui < GetUCount() && vi < GetVCount() ) {
    MbCartPoint3D bakPoint( points( vi, ui ) );
    points( vi, ui ) = pnt;

    if ( pointsCloud && ownCloudPlane ) {
      if ( CreateOwnCloudPlane() )
        bRes = true;
      else
        points( vi, ui ) = bakPoint;
    }
  }
  return bRes;
}


//------------------------------------------------------------------------------
// \ru Получить вес \en Get a weight
// ---
inline bool NurbsSurfaceValues::GetUVWeight( size_t ui, size_t vi, double & wt ) const
{
  if ( weights != nullptr && ui < GetUCount() && vi < GetVCount() ) {
    wt = (*weights)( vi, ui );
    return (wt != UNDEFINED_DBL); //-V550
  }
  wt = weight;
  return (wt != UNDEFINED_DBL); //-V550
}


//------------------------------------------------------------------------------
// \ru Получить общий вес, вернет true, если вес у вес одинаковый \en Get total weight, return true if all the weights are the same
// ---
inline bool NurbsSurfaceValues::GetCommonWeight( double & wt ) const
{
  if ( weights == nullptr && weight != UNDEFINED_DBL ) { //-V550
    wt = weight;
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru Установить флаг прохождения поверхности через точки \en Set flag of surface passing through the points
// ---
inline void NurbsSurfaceValues::SetThroughPoints( bool tp )
{
  throughPoints = tp;

  if ( throughPoints ) {
    DeleteWeights();
    weight = 1.0;
  }
  if ( weights == nullptr && weight == UNDEFINED_DBL ) //-V550
    weight = 1.0;
}


//------------------------------------------------------------------------------
/** \brief \ru Параметры сплайновой поверхности.
           \en The parameters of spline surface.\~
  \details \ru Параметры определяют контрольные точки, веса, узлы сплайновой поверхности.\n
           \en The parameters determine control points, weights, knots of spline surface.\n\~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbNurbsSurfacesShellParams : public MbPrecision {

private:
  NurbsSurfaceValues & _params;     ///< \ru Параметры операции. \en The operation parameters.
  SPtr<MbSNameMaker>   _operNames;  ///< \ru Именователь операции. \en An object defining names generation in the operation.
  bool                 _isPhantom;  ///< \ru Создается ли фантомное тело. \en Is phantom shell created.

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbNurbsSurfacesShellParams( NurbsSurfaceValues & values, const MbSNameMaker & operNames, bool isPhantom )
    : MbPrecision(                        )
    , _params    (  values                )
    , _operNames ( &operNames.Duplicate() )
    , _isPhantom (  isPhantom             )
  {}

  /// \ru Деструктор. \en Destructor.
  ~MbNurbsSurfacesShellParams() {}

  /// \ru Получить параметры операции. \en Get the operation parameters.
  const NurbsSurfaceValues & GetParams() const { return _params; }
        NurbsSurfaceValues & SetParams()       { return _params; }
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker & GetNameMaker() const { return *_operNames; }
  /// \ru Создается ли фантомное тело. \en Is phantom shell created.
  bool IsPhantom() const { return _isPhantom; }

  OBVIOUS_PRIVATE_COPY( MbNurbsSurfacesShellParams )
};


//-----------------------------------------------------------------------------
/** \brief \ru Параметры поверхности по сетке кривых.
           \en Surface parameter by grid of curves. \~
  \details \ru Параметры содержат необходимые данные для построения поверхности по сетке кривых. \n
           \en The parameters contain the necessary data to construct a surface by grid of curves. \n \~
  \ingroup Shell_Building_Parameters
*/
//---
struct MATH_CLASS MeshSurfaceValues {
public:
  friend class MbMeshShell;

private:
  RPArray<MbCurve3D> curvesU;     ///< \ru Набор кривых по первому направлению. \en Set of curves along the first direction.
  RPArray<MbCurve3D> curvesV;     ///< \ru Набор кривых по второму направлению. \en Set of curves along the second direction.
  RPArray<MbPolyline3D> chainsU; ///< \ru Набор цепочек по первому направлению. \en Set of chains along the first direction. 
  RPArray<MbPolyline3D> chainsV; ///< \ru Набор цепочек по второму направлению. \en Set of chains along the second direction. 
  bool               uClosed;     ///< \ru Замкнутость по U направлению. \en Closedness along U direction.
  bool               vClosed;     ///< \ru Замкнутость по V направлению. \en Closedness along V direction.
  bool               checkSelfInt;///< \ru Искать самопересечения. \en Find self-intersections.
  // \ru Сопряжения на границе (если сопряжения заданы, то кривые должны быть SurfaceCurve или контур из SurfaceCurve). \en Mates on the boundary (if mates are given, then curves must be SurfaceCurve or contour from SurfaceCurve).
  MbeMatingType      type0;       ///< \ru Сопряжение на границе 0. \en Mate on the boundary 0.
  MbeMatingType      type1;       ///< \ru Сопряжение на границе 1. \en Mate on the boundary 1.
  MbeMatingType      type2;       ///< \ru Сопряжение на границе 2. \en Mate on the boundary 2.
  MbeMatingType      type3;       ///< \ru Сопряжение на границе 3. \en Mate on the boundary 3.

  c3d::SurfacesVector surface0; /// \ru Сопрягаемые поверхности через curvesU[0] \en Mating surfaces through curvesU[0]
  c3d::SurfacesVector surface1; /// \ru Сопрягаемые поверхности через curvesV[0] \en Mating surfaces through curvesV[0]
  c3d::SurfacesVector surface2; /// \ru Сопрягаемые поверхности через curvesU[maxU] \en Mating surfaces through curvesU[maxU]
  c3d::SurfacesVector surface3; /// \ru Сопрягаемые поверхности через curvesV[maxV] \en Mating surfaces through curvesV[maxV]
  MbPoint3D *         point;       ///< \ru Точка на поверхности. Используется для уточнения. \en Point on the surface. Used for specializing.

  bool               defaultDir0; ///< \ru Направление сопряжения на границе 0 по умолчанию. \en Default mate direction through the boundary 0.
  bool               defaultDir1; ///< \ru Направление сопряжения на границе 1 по умолчанию. \en Default mate direction through the boundary 1.
  bool               defaultDir2; ///< \ru Направление сопряжения на границе 2 по умолчанию. \en Default mate direction through the boundary 2.
  bool               defaultDir3; ///< \ru Направление сопряжения на границе 3 по умолчанию. \en Default mate direction through the boundary 3.
  bool               outDirs[4];  /// \ru Направления сопряжений для случая смены типа автопределения направления для сохранения формы ППСК.
                                  /// \en Mating direction in case of changing the type of automatic direction detection to maintain the surface shape.
  bool               autoDirection; ///< \ru Тип автоопределения направления \en Type of automatic direction detection
  mutable uint8      directOrderV;///< \ru По второму семейству кривых порядок кривых совпадает. \en Order of the curves coincides by the second set of curves.
  bool               tesselate;   ///< \ru Достраивать ли дополнительные сечения. \en Whether to build additional sections.
  bool               g2Cont;      ///< \ru Требуется ли гладкость g2 для граней оболочки. \en Is the smoothness g2 required for the faces of the shell.
  bool               useProport;  ///< \ru Выравнивать параметризацию кривых по длине. \en Align the parametrization of curves along the length.
  std::map<size_t, bool> splitU;// Набор кривых U-разделителей.
  std::map<size_t, bool> splitV;// Набор кривых V-разделителей.

public:
  /// \ru Конструктор копирования. \en Copy-constructor.
  MeshSurfaceValues( const MeshSurfaceValues &, MbRegDuplicate * ireg );
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MeshSurfaceValues();
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \param[in] curvesU, curvesV - \ru Наборы кривых по первому и второму направлению.
                                  \en Sets of curves along the first and second directions. \~
    \param[in] uClosed, vClosed - \ru Признак замкнутости по направлениям u и v.
                                  \en Closedness attribute along the u and v directions. \~
  */
  MeshSurfaceValues( const RPArray<MbCurve3D> & curvesU, bool uClosed,
                     const RPArray<MbCurve3D> & curvesV, bool vClosed );
  /// \ru Деструктор. \en Destructor.
  ~MeshSurfaceValues();
  /// \ru Оператор копирования. \en Copy operator.
  //MeshSurfaceValues & operator = ( const MeshSurfaceValues & initVal );

  /** \brief \ru Функция инициализации.
             \en Initialization function. \~
    \details \ru Функция инициализации на оригиналах кривых и копиях поверхностей.
             \en Initialization function on the original curves and copies of surfaces. \~
    \param[in] curvesU, curvesV - \ru Наборы кривых по первому и второму направлению.
                                  \en Sets of curves along the first and second directions. \~
    \param[in] chainsU, chainsV - \ru Наборы цепочек по первому и второму направлению.
                                  \en Sets of chains along the first and second directions. \~
    \param[in] uClosed, vClosed - \ru Признак замкнутости по направлениям u и v.
                                  \en Closedness attribute along the u and v directions. \~
    \param[in] checkSelfInt - \ru Флаг проверки на самопересечение.
                              \en Flag of check for self-intersection. \~
    \param[in] tesselate    - \ru Достраивать ли дополнительные сечения. 
                              \en Whether to build additional sections. \~
    \param[in] type0, type1, type2, type3 - \ru Типы сопряжений на границах.
                                            \en Mates types on the boundaries. \~
    \param[in] surf0, surf1, surf2, surf3 - \ru Соответствующие сопрягаемые поверхности.
                                            \en Corresponding mating surfaces. \~
    \param[in] point - \ru Точка на поверхности. Используется для уточнения. 
                       \en Point on the surface. Used for specializing.\~
    \param[in] modify - \ru Флаг модификации кривых по сопряжениям.
                        \en Flag of curves modification by mates. \~
    \param[in] direct0, direct1, direct2, direct3 - \ru Направление поверхности на границе сопряжения.
                                                    \en The direction of the surface at the border of mating. \~
    \return            \ru Статус выполнения. 
                       \en Execution status.
  */
  bool        Init( const RPArray<MbCurve3D> & curvesU, bool uClosed,
                    const RPArray<MbCurve3D> & curvesV, bool vClosed,
                    bool checkSelfInt,
                    bool tess = false,
                    const RPArray<MbPolyline3D> * chainsU = nullptr,
                    const RPArray<MbPolyline3D> * chainsV = nullptr,
                    MbeMatingType type0 = trt_Position, MbeMatingType type1 = trt_Position,
                    MbeMatingType type2 = trt_Position, MbeMatingType type3 = trt_Position,
                    const c3d::ConstSurfacesVector * surf0 = nullptr, // \ru Сопрягаемые поверхности через curvesU[0] \en Mating surfaces through curvesU[0]
                    const c3d::ConstSurfacesVector * surf1 = nullptr, // \ru Сопрягаемые поверхности через curvesV[0] \en Mating surfaces through curvesV[0]
                    const c3d::ConstSurfacesVector * surf2 = nullptr, // \ru Сопрягаемые поверхности через curvesU[maxU] \en Mating surfaces through curvesU[maxU]
                    const c3d::ConstSurfacesVector * surf3 = nullptr, // \ru Сопрягаемые поверхности через curvesV[maxV] \en Mating surfaces through curvesV[maxV]
                    const MbPoint3D * pnt = nullptr,
                    bool modify = true,
                    bool direct0 = true, bool direct1 = true, bool direct2 = true, bool direct3 = true );

  /** \brief \ru Функция инициализации.
             \en Initialization function. \~
    \details \ru Функция инициализации на оригиналах кривых и копиях поверхностей.
             \en Initialization function on the original curves and copies of surfaces. \~
    \param[in] curvesU, curvesV - \ru Наборы кривых по первому и второму направлению.
                                  \en Sets of curves along the first and second directions. \~
    \param[in] uClosed, vClosed - \ru Признак замкнутости по направлениям u и v.
                                  \en Closedness attribute along the u and v directions. \~
    \param[in] types - \ru Типы сопряжений на границах.
                       \en Mates types on the boundaries. \~
    \param[in] surfaces - \ru Соответствующие сопрягаемые поверхности. Ноль, если не задано.
                          \en Corresponding mating surfaces. Zero if not defined.\~
    \param[in] useDefaultDir - \ru Направление поверхности на границе сопряжения.
                               \en The direction of the surface at the border of mating. \~
    \param[in] checkSelfInt - \ru Флаг проверки на самопересечение.
                              \en Flag of check for self-intersection. \~
    \param[in] tess    - \ru Достраивать ли дополнительные сечения. 
                         \en Whether to build additional sections. \~
    \param[in] smooth   - \ru Требуется ли гладкость g2 для граней оболочки. 
                          \en Is the smoothness g2 required for the faces of the shell.
    \param[in] chainsU, chainsV - \ru Наборы цепочек по первому и второму направлению. Ноль, если не задано.
                                  \en Sets of chains along the first and second directions. Zero if not defined.\~
    \param[in] point - \ru Точка на поверхности. Используется для уточнения. Ноль, если не задано.
                       \en Point on the surface. Used for specializing. Zero if not defined.\~
    \param[in] modify - \ru Флаг модификации кривых по сопряжениям.
                        \en Flag of curves modification by mates. \~
    \return            \ru Статус выполнения. 
                       \en Execution status.
  */
  bool        Init( const RPArray<MbCurve3D>          & curvesU,
                    const RPArray<MbCurve3D>          & curvesV,
                          bool                          uClosed,
                          bool                          vClosed,
                          MbeMatingType              ( &types )[4],
                    const c3d::ConstSurfacesVector * ( &surfaces )[4],
                          bool                       ( &useDefaultDir )[4],
                          bool                          checkSelfInt,
                          bool                          tess,
                          bool                          smooth,
                    const RPArray<MbPolyline3D>       * chainsU,
                    const RPArray<MbPolyline3D>       * chainsV,
                    const MbPoint3D                   * pnt,
                          bool                          modify ); 

  /** \brief \ru Функция инициализации.
             \en Initialization function. \~
    \details \ru Функция инициализации на оригиналах или копиях кривых и поверхностей.
             \en Initialization function on the originals or copies of curves and surfaces. \~
    \param[in] pars - \ru Исходные параметры.
                      \en Initial parameters. \~
    \param[in] sameItems - \ru Флаг использования оригиналов кривых и поверхностей.
                           \en Flag of using originals of curves and surfaces. \~
  */
  void        Init( const MeshSurfaceValues & pars, bool sameItems );

  /** \brief \ru Лежит ли кривая на поверхности.
             \en Determine whether the curve lies on the surface. \~
    \details \ru Лежит ли кривая полностью на поверхности.
             \en Determine whether the curve entirely lies on the surface. \~
    \param[in] curve - \ru Проверяемая кривая.
                       \en Checking curve. \~
    \param[in] surf  - \ru Проверяемая поверхность.
                       \en Checking surface. \~
    \return           \ru Признак, что кривая лежит на поверхности. 
                      \en A sign that the curve is on the surface.
  */
  bool        IsCurveOnSurface( const MbCurve3D & curve, const MbSurface & surf ) const;

  /** \brief \ru Сопрягаются ли кривые с поверхностью  в точках пересечения с borderCurve.
             \en Whether the curves are mated with surface in the points of intersection with borderCurve. \~
    \details \ru Сопрягаются ли кривые (касательно, по нормали, гладко) с поверхностью в точках пересечения с borderCurve.
             \en Whether the curves are mated (tangentially, along the normal, smoothly) with surface in the points of intersection with borderCurve. \~
    \param[in]  curves     - \ru Набор проверяемых кривых.
                             \en Set of checking curves. \~
    \param[in]  borderCurve- \ru Кривая на поверхности.
                             \en Curve on the surface. \~
    \param[in]  matSurfaces- \ru Набор поверхностей сопряжения.
                             \en Set of mating surfaces. \~
    \param[out] isTangent  - \ru Признак касательного сопряжения.
                             \en Attribute of the tangent mate. \~
    \param[out] isNormal   - \ru Признак сопряжения по нормали.
                             \en Attribute of normal mate. \~
    \param[out] isSmooth   - \ru Признак гладкого сопряжения.
                             \en Attribute of the smooth mate. \~
    \param[in]  borderCls -  \ru Замкнутость поверхности по границе сопряжения.
                             \en Surface closeness by mating boundary. \~
    \param[in]  transCls -  \ru Замкнутость поверхности в трансверсальном направлении.
                             \en Closeness of the surface in the transverse direction. \~
    \param[in]  vers -      \ru Версия математики.
                             \en Math version. \~
    \return                  \ru Признак, что кривая лежит на поверхности. 
                             \en A sign that the curve is on the surface.
  */
  static bool AreCurvesMatingToSurface( const RPArray<MbCurve3D>      & curves,
                                        const MbCurve3D               * borderCurve,
                                        const c3d::SurfacesVector     & matSurfaces,
                                              bool                    & isTangent,
                                              bool                    & isNormal,
                                              bool                    & isSmooth,
                                              bool                      borderCls,
                                              bool                      transCls,
                                              VERSION                   vers = VERSION(MATH_20_TR_VERSION-1) );

  /// \ru Сопрягаются ли кривые с поверхностью  в точках пересечения с поверхностной кривой (borderCurve).
  /// \en Whether the curves are mated with surface in the points of intersection with the surface curve (borderCurve).
  static bool AreCurvesMatingToSurface( const RPArray<MbCurve3D> & curves, const MbCurve3D * borderCurve, 
                                        bool & isTangent, bool & isNormal, bool & isSmooth, bool uc, bool vc );

  /// \ru Получить точки скрещивания-пересечения кривой с семейством кривых. \en Get crossing-intersection points of curves with the set of curves.
  bool        GetPointsOfCrossing( const MbCurve3D & curve, const RPArray<MbCurve3D> & otherCurves,
                                   SArray<MbCartPoint3D> & res ) const;
  /// \ru Проверка на наличие контуров и ломаных. \en Check for contours and broken lines.
  bool        CheckMultiSegment( const MbSNameMaker & snMaker ) const;
  /// \ru Обратить порядок следования кривых по второму направлению, чтобы directOrderV был true. \en Invert order of curves along the second direction to directOrderV is true.
  void        InvertCurvesV ();

  /// \ru Получить кривую на границе с номером i. \en Get i-th curve on the boundary.
  const MbCurve3D * GetBorderCurve( ptrdiff_t i ) const;
  /// \ru Получить тип сопряжения на границе с номером i. \en Get i-th mate type on the boundary.
  MbeMatingType GetTransitType( ptrdiff_t i ) const;
  /// \ru Получить поверхность сопряжения на границе с номером i. \en Get i-th mate surface on the boundary.
  void        GetSurface( size_t i, c3d::ConstSurfacesVector & surfaces ) const;
  /// \ru Получить поверхность сопряжения на границе с номером i. \en Get i-th mate surface on the boundary.
  void        SetSurface( size_t i, c3d::SurfacesVector & surfaces );
  /// \ru Получить направление сопряжения на границе с номером i. \en Get i-th mate direction on the boundary.
  bool IsDefaultDirection( size_t i ) const;
  /// \ru Установить направление сопряжения на границе с номером i. \en Set  mate direction at the i-th boundary.
  void SetDefaultDirection( size_t i, bool fl );

  /// \ru Замкнутость по U направлению. \en Closedness along U direction.
  bool        GetUClosed() const { return uClosed; }
  /// \ru Замкнутость по V направлению. \en Closedness along V direction.
  bool        GetVClosed() const { return vClosed; }
  /// \ru Замкнутость по U направлению. \en Closedness along U direction.
  void        SetUClosed( bool cls ) { uClosed = cls; }
  /// \ru Замкнутость по V направлению. \en Closedness along V direction.
  void        SetVClosed( bool cls ) { vClosed = cls; }

  /// \ru Количество кривых по U. \en The count of curves along U.
  size_t      GetCurvesUCount() const { return curvesU.Count(); }
  /// \ru Максимальный индекс в массиве кривых по U. \en The maximum index in the array of curves along U.
  ptrdiff_t   GetCurvesUMaxIndex() const { return curvesU.MaxIndex(); }
  /// \ru Получить кривую по индексу. \en Get the curve by the index.
  const MbCurve3D * GetCurveU( size_t k ) const { return ((k < curvesU.Count()) ? curvesU[k] : nullptr); }
  /// \ru Получить кривую по индексу. \en Get the curve by the index.
  MbCurve3D * SetCurveU( size_t k )       { return ((k < curvesU.Count()) ? curvesU[k] : nullptr); }
  /// \ru Получить кривые по U. \en Get curves along U.
  void        GetCurvesU( RPArray<MbCurve3D> & curves ) const { curves.AddArray(curvesU); }
  /// \ru Установить кривые по U. \en Set curves along U.
  void        SetCurvesU( const RPArray<MbCurve3D> & newCurves );
  /// \ru Отцепить кривые по U. \en Detach curves along U.
  void        DetachCurvesU( RPArray<MbCurve3D> & curves );
  /// \ru Найти кривую. \en Find curve.
  size_t      FindCurveU( const MbCurve3D * curve ) const { return curvesU.FindIt( curve ); }

  /// \ru Количество кривых по V. \en The count of curves along V.
  size_t      GetCurvesVCount() const { return curvesV.Count(); }
  /// \ru Максимальный индекс в массиве кривых по V. \en The maximum index in the array of curves along V.
  ptrdiff_t   GetCurvesVMaxIndex() const { return curvesV.MaxIndex(); }
  /// \ru Получить кривую по индексу. \en Get the curve by the index.
  const MbCurve3D * GetCurveV( size_t k ) const { return ((k < curvesV.Count()) ? curvesV[k] : nullptr); }
  /// \ru Получить кривую по индексу. \en Get the curve by the index.
  MbCurve3D * SetCurveV( size_t k ) const { return ((k < curvesV.Count()) ? curvesV[k] : nullptr); }
  /// \ru Получить кривые по V. \en Get curves along V.
  void        GetCurvesV( RPArray<MbCurve3D> & curves ) const { curves.AddArray(curvesV); }
  /// \ru Установить кривые по V. \en Set curves along V.
  void        SetCurvesV( const RPArray<MbCurve3D> & newCurves );
  /// \ru Отцепить кривые по V. \en Detach curves along V.
  void        DetachCurvesV( RPArray<MbCurve3D> & curves );
  /// \ru Найти кривую. \en Find curve.
  size_t      FindCurveV( const MbCurve3D * curve ) const { return curvesV.FindIt( curve ); }

  /// \ru Количество цепочек по U. \en The count of chains along U.
  size_t      GetChainsUCount() const { return chainsU.Count(); }
  /// \ru Максимальный индекс в массиве цепочек по U. \en The maximum index in the array of chains along U.
  ptrdiff_t   GetChainsUMaxIndex() const { return chainsU.MaxIndex(); }
  /// \ru Получить цепочку по индексу. \en Get the chain by the index.
  const MbPolyline3D * GetChainU( size_t k ) const { return ( ( k < chainsU.Count() ) ? chainsU[k] : nullptr ); }
  /// \ru Получить цепочку по индексу. \en Get the chain by the index.
  MbPolyline3D * SetChainU( size_t k ) { return ( ( k < chainsU.Count() ) ? chainsU[k] : nullptr ); }
  /// \ru Получить цепочки по U. \en Get chains along U.
  void        GetChainsU( RPArray<MbPolyline3D> & chains ) const { chains.AddArray( chainsU ); }
  /// \ru Установить цепочки по U. \en Set chains along U.
  void        SetChainsU( const RPArray<MbPolyline3D> & newChains );
  /// \ru Отцепить цепочки по U. \en Detach chains along U.
  void        DetachChainsU( RPArray<MbPolyline3D> & chains );
  /// \ru Найти цепочку. \en Find chain.
  size_t      FindChainU( const MbPolyline3D * curve ) const { return chainsU.FindIt( curve ); }

  /// \ru Количество цепочек по V. \en The count of chains along V.
  size_t      GetChainsVCount() const { return chainsV.Count(); }
  /// \ru Максимальный индекс в массиве цепочек по V. \en The maximum index in the array of chains along V.
  ptrdiff_t   GetChainsVMaxIndex() const { return chainsV.MaxIndex(); }
  /// \ru Получить цепочку по индексу. \en Get the chain by the index.
  const MbPolyline3D * GetChainV( size_t k ) const { return ( ( k < chainsV.Count() ) ? chainsV[k] : nullptr ); }
  /// \ru Получить цепочку по индексу. \en Get the chain by the index.
  MbPolyline3D * SetChainV( size_t k ) { return ( ( k < chainsV.Count() ) ? chainsV[k] : nullptr ); }
  /// \ru Получить цепочки по V. \en Get chains along V.
  void        GetChainsV( RPArray<MbPolyline3D> & chains ) const { chains.AddArray( chainsV ); }
  /// \ru Установить цепочки по V. \en Set chains along V.
  void        SetChainsV( const RPArray<MbPolyline3D> & newChains );
  /// \ru Отцепить цепочки по V. \en Detach chains along V.
  void        DetachChainsV( RPArray<MbPolyline3D> & chains );
  /// \ru Найти цепочку. \en Find chain.
  size_t      FindChainV( const MbPolyline3D * curve ) const { return chainsV.FindIt( curve ); }

  /// \ru Установить деление оболочки по указанной кривой. \en Set the division of the shell along the specified curve.
  bool        SetSplitingCurve  ( const MbCurve3D * curve, bool onIndependentParts = false );
  /// \ru Получить тип деления оболочки для указанной кривой. \en Get the shell division type for the specified curve.
  bool        GetSplitingProp  ( const MbCurve3D * curve, bool & onIndependentParts );
  /// \ru Отменить деление оболочки по указанной кривой. \en Cancel division of the shell along the specified curve.
  bool        UnsetSplitingCurve( const MbCurve3D * curve );

  /// \ru Установить точку. \en Set point.
  void        SetPoint( const MbPoint3D * pnt );
  /// \ru Получить точку. \en Get point.
  const MbPoint3D * GetPoint() const { return point; }
  /// \ru Получить точку. \en Get point.
  MbPoint3D * SetPoint() { return point; }


  /**
    \ru \name Вспомогательные функции геометрических преобразований.
    \en \name Auxiliary functions of geometric transformations.
    \{ */
  /// \ru Преобразовать кривые согласно матрице. \en Transform curves according to the matrix.
  void        Transform( const MbMatrix3D &, MbRegTransform * ireg );
  /// \ru Сдвинуть кривые вдоль вектора. \en Move curves along a vector.
  void        Move     ( const MbVector3D &, MbRegTransform * ireg );
  /// \ru Повернуть кривые вокруг оси на заданный угол. \en Rotate curves at a given angle around an axis.
  void        Rotate   ( const MbAxis3D   &, double angle, MbRegTransform * ireg );
  /// \ru Привести кривые к поверхностной форме (кривые на поверхности) \en Convert curves to the surface form (curves on the surface)
  bool        TransformCurves( VERSION vers );
  /// \ru Привести кривые к поверхностной форме (кривые на поверхности) \en Convert curves to the surface form (curves on the surface)
  bool        TransformForCompositeSurfaceMating( const double mPrec, VERSION vers );
  /// \ru Обеспечить непрерывность длины первой производной для кривых семейства dirU.
  /// \en Ensure continuity of the length of the first derivative for the curves of the dirU family.
  bool        SetContinuousDerivativeLength( bool dirU, bool & smooth, const double aEps, VERSION version );
  /** \} */

  /// \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSame( const MeshSurfaceValues &, double accuracy ) const;

  /// \ru Нужно ли проверять самопересечения? \en Whether it is necessary to check self-intersections?
  bool        CheckSelfInt() const { return checkSelfInt; }
  /// \ru Достраивать ли дополнительные сечения. \en Whether to build additional sections.
  bool        IsTesselate() const { return tesselate; }
  /// \ru Выравнивать параметризацию кривых по длине. \en Align the parametrization of curves along the length.
  bool        UseProportional() const { return useProport; }
  /// \ru Установить параметризацию кривых по длине. \en Set the parametrization of curves along the length.
  void        SetProportional( bool val )  { useProport = val; }
  /// \ru Требуется ли гладкость g2 для граней оболочки. \en Is the smoothness g2 required for the faces of the shell.
  bool        IsSmooth() const { return g2Cont;  }
  /// \ru Установить тип автоопределения направления \en Set the auto-detect direction type.
  void        SetAutoDirection( bool fl ) { autoDirection = fl; }
  /// \ru Получить тип автоопределения направления \en Get the auto-detect direction type.
  bool        GetAutoDirection() const { return autoDirection; }
  // \ru Установить направления сопряжений для случая смены типа автопределения направления для сохранения форма ППСК
  // \en Set the mating direction in case of changing the type of automatic direction detection to maintain the surface shape.
  void        SetOutDirection( size_t bnd, bool outD ) { outDirs[bnd > 3 ? 3 : bnd] = outD;  }
  // \ru Получить направления сопряжений для случая смены типа автопределения направления для сохранения форма ППСК
  // \en Get the mating direction in case of changing the type of automatic direction detection to maintain the surface shape.
  bool        GetOutDirection( size_t bnd ) const { return outDirs[bnd > 3 ? 3 : bnd];  }

  /// \ru Получить поверхность сопряжения к граничной кривой по параметру на кривой. 
  /// \en Get the mating surface to the border curve by the curve parameter.
  static const MbSurface * 
              GetMatingSurface( const MbCurve3D * borderCurve, double param );
  /// \ru Получить поверхность сопряжения к граничной кривой в точке пересечения с трансверсальной кривой. 
  /// \en Get the mating surface to the border curve at the intersection with the transversal curve.
  static const MbSurface * 
              GetMatingSurface( const MbCurve3D * borderCurve, const MbCurve3D * transCurve );
  /// \ru Получить поверхностную кривую в указанной точке на границе сопряжения. 
  /// \en Get a surface curve at a specified point on the mating border.
  static bool GetSurfaceCurve( const MbCurve3D * borderCurve, double param, const MbSurfaceCurve *& sCurve, double & t );
  /// \ru Является ли кривая поверхностной по типу. \en Is the curve surface in type.
  static bool IsSurfaceCurveType( const MbCurve3D * borderCurve );
private:
  void        AddRefCurves();    // \ru Увеличить счетчик ссылок у кривых. \en Increase the reference count of curves.
  void        AddRefPoint();    // \ru Увеличить счетчик ссылок у точки. \en Increase the reference count of point.
  void        AddRefSurfaces();  // \ru Увеличить счетчик ссылок у поверхностей. \en Increase the reference count of surfaces.
  void        ReleaseCurves();   // \ru Удалить кривые. \en Release curves.
  void        ReleasePoint();   // \ru Удалить точку. \en Release point.
  void        ReleaseSurfaces(); // \ru Удалить поверхности. \en Release surfaces.
  // \ru Определить порядок следования кривых по второму направлению. \en Determine the order of curves along the second direction.
  void        CalculateOrderV() const;
  // \ru Привести кривую к типу поверхностной кривой или к контура из SurfaceCurve. \en Convert the curve to type of surface curve or contour from SurfaceCurve.
  static bool TransformToSurfaceCurve( const MbCurve3D          & initCurve,
                                             bool                 isWhole,
                                       const MbSurface          & surface,
                                       const RPArray<MbCurve3D> & constrCurves,
                                             MbSurfaceCurve    *& resCurve,
                                       const double               mPrec,
                                             VERSION              vers );
  // \ru Привести кривую к типу поверхностной кривой или к контура из SurfaceCurve. \en Convert the curve to type of surface curve or contour from SurfaceCurve.
  static bool TransformToSurfaceCurve( const MbCurve3D           & initCurve,
                                        const c3d::SurfacesVector & surfaces, 
                                        const RPArray<MbCurve3D>  & constrCurves, 
                                              MbCurve3D          *& resCurve,
                                        const double                mPrec,
                                              VERSION               vers );

public:
KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MeshSurfaceValues, MATH_FUNC_EX )
OBVIOUS_PRIVATE_COPY( MeshSurfaceValues )
};



//------------------------------------------------------------------------------
/** \brief \ru Результаты построения поверхности по сети кривых.
           \en Results of construction of a shell by mesh of curves. \~
  \details \ru Результаты построения поверхности по сети кривых.
           \en Results of construction of a shell by mesh of curves. \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbMeshShellResults : public MbOperationResults {
private:
  c3d::SolidSPtr _solid;      // \ru Оболочка \en Result shell.
  bool           _outDirs[4]; // \ru Направления сопряжений для случая смены типа автопределения направления для сохранения форма ППСК
                              // \en Mating direction in case of changing the type of automatic direction detection to maintain the surface shape.
public:
  MbMeshShellResults() 
    : MbOperationResults()
    , _solid            ()
  {
    for( size_t i = 0; i < 4; i++ )
      _outDirs[i] = false;
  }
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbMeshShellResults( const MbMeshShellResults & other )
    : MbOperationResults( other        )
    , _solid            ( other._solid )
  {
    for( size_t i = 0; i < 4; i++ )
      _outDirs[i] = other._outDirs[i];
  }
  ~MbMeshShellResults() {}
  /// \ru Оператор присваивания. \en Assignment operator.
  MbMeshShellResults & operator = ( const MbMeshShellResults & other ) {
    MbOperationResults::operator =( static_cast<const MbOperationResults &>(other) );
    _solid = other._solid;
    for( size_t i = 0; i < 4; i++ )
      _outDirs[i] = other._outDirs[i];
    return *this;
  }
  /// \ru Получить результирующее тело. \en Get resulting solid.
  c3d::SolidSPtr GetResultSolid() const { return _solid; }
  /// \ru Получить результирующее тело. \en Get resulting solid.
  c3d::SolidSPtr & SetResultSolid() { return _solid; }
  // \ru Получить направления сопряжений для случая смены типа автопределения направления для сохранения форма ППСК
  // \en Get the mating direction in case of changing the type of automatic direction detection to preserve the surface shape.
  bool        GetOutDirection( size_t bnd ) const { return _outDirs[bnd > 3 ? 3 : bnd];  }
  /// \ru Функция инициализации. \en Initialization function.
  void Init ( MbSolid & s, const bool (&dirFlags)[4] ){
    _solid = &s;
    for( size_t i = 0; i < 4; i++ )
      _outDirs[i] = dirFlags[i];
  }
};

//------------------------------------------------------------------------------
/** \brief \ru Параметры построения поверхности по сети кривых.
           \en Parameters for creating the shell by mesh of curves. \~
  \details \ru Параметры построения поверхности по сети кривых.
           \en Parameters for creating the shell by mesh of curves. \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbMeshShellParameters : public MbPrecision
{
private:
  MeshSurfaceValues    _params;      ///< \ru Параметры операции. \en The operation parameters.
  SPtr<MbSNameMaker>   _operNames;   ///< \ru Именователь операции. \en An object defining names generation in the operation.
  bool                 _isFantom;    ///< \ru Создается ли фантомное тело. \en Is phantom shell created.
private:
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbMeshShellParameters( const MbMeshShellParameters &, MbRegDuplicate * ireg );
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbMeshShellParameters( const MeshSurfaceValues & vals, const MbSNameMaker & operNames, bool isFant );
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker &  GetNameMaker() const { return *_operNames; }
  /// \ru Получить параметры операции. \en Get the operation parameters.
  const MeshSurfaceValues & GetParams() const { return _params; }
  /// \ru Создается ли фантомное тело. \en Is phantom shell created.
        bool  IsPhantom() const { return _isFantom; }

OBVIOUS_PRIVATE_COPY( MbMeshShellParameters )
};


//------------------------------------------------------------------------------
// \ru Получить тип сопряжения на границе с номером i \en Get i-th mate type on the boundary
//---
inline
MbeMatingType MeshSurfaceValues::GetTransitType( ptrdiff_t i ) const
{
  MbeMatingType res = trt_Position;

  switch ( i ) {
    case 0: res = type0; break;
    case 1: res = type1; break;
    case 2: res = type2; break;
    case 3: res = type3; break;
  }

  return res;
}


//------------------------------------------------------------------------------
/** \brief \ru Данные для построения линейчатой поверхности.
           \en Data for the construction of a ruled surface. \~
  \details \ru Данные для построения линейчатой поверхности по двум кривым. \n
           \en Data for the construction of a ruled surface by two curves. \n \~
  \ingroup Shell_Building_Parameters
*/
//---
struct MATH_CLASS RuledSurfaceValues {
private:
  MbCurve3D *    curve0;         ///< \ru Первая кривая \en The first curve.
  MbCurve3D *    curve1;         ///< \ru Вторая кривая. \en The second curve.
  SArray<double> breaks0;        ///< \ru Параметры разбиения первой кривой curve0. \en Splitting parameters of the first curve0 curve.
  SArray<double> breaks1;        ///< \ru Параметры разбиения второй кривой curve1. \en Splitting parameters of the second curve1 curve.
  bool           joinByVertices; ///< \ru Соединять контура с одинаковым количеством сегментов через вершины. \en Join contours with the same count of segments through vertices.
  bool           checkSelfInt;   ///< \ru Искать самопересечения. \en Find self-intersections.
  bool           simplifyFaces;  ///< \ru Упрощать грани. \en Simplify faces.
  bool           proportional;   ///< \ru Пропорциональная натуральной параметризация кривых. \en Parameterization of curves is proportional to the natural.
  double         mismatchMax;    ///< \ru Допустимое несовпадение противолежащих точек стыковки сегментов. \en Permissible mismatch of opposite points of connecting segments.
  bool           segmentSplit;   ///< \ru Разделять оболочку на грани по сегментам контуров. \en Divide the shell into faces by contour segments.

public:
  /// \ru Конструктор копирования. \en Copy-constructor.
  RuledSurfaceValues( const RuledSurfaceValues &, MbRegDuplicate * ireg );
  /// \ru Конструктор по умолчанию. \en Default constructor.
  RuledSurfaceValues();
  /// \ru Деструктор. \en Destructor.
  ~RuledSurfaceValues();

public:
  /** \brief \ru Функция инициализации.
             \en Initialization function. \~
    \details \ru Функция инициализации на оригиналах кривых.
      Контейнеры параметров разбиения кривых будут очищены.
             \en Initialization function on the curves originals.
      Containers of parameters of splitting curves will be cleared. \~
    \param[in] inCurve0 - \ru Кривая для замены первой кривой.
                          \en The curve for the replacement of the first curve. \~
    \param[in] inCurve1 - \ru Кривая для замены второй кривой.
                          \en The curve for the replacement of the second curve. \~
    \param[in] selfInt  - \ru Флаг проверки самопересечений.
                          \en Flag of self-intersections checking. \~
    \param[in] prop     - \ru Флаг параметризации кривых пропорционально натуральной.
                          \en Flag of proportional to the natural parameterization of curves. \~
    \return \ru Результат первичной проверки параметров.
            \en The result of the primary scan of parameters. \~
  */
  bool        Init( const MbCurve3D      & inCurve0,
                    const MbCurve3D      & inCurve1,
                          bool             selfInt = false,
                          bool             prop = false );

  /** \brief \ru Функция инициализации.
             \en Initialization function. \~
    \details \ru Функция инициализации на оригиналах кривых.
             \en Initialization function on the curves originals. \~
    \param[in] inCurve0 - \ru Кривая для замены первой кривой.
                          \en The curve for the replacement of the first curve. \~
    \param[in] inCurve1 - \ru Кривая для замены второй кривой.
                          \en The curve for the replacement of the second curve. \~
    \param[in] pars0    - \ru Параметры разбиения кривой inCurve0.
                          \en The parameters of splitting curve inCurve0. \~
    \param[in] pars1    - \ru Параметры разбиения кривой inCurve1.
                          \en The parameters of splitting curve inCurve1. \~
    \param[in] selfInt  - \ru Флаг проверки самопересечений.
                          \en Flag of self-intersections checking. \~
    \param[in] prop     - \ru Флаг параметризации кривых пропорционально натуральной.
                          \en Flag of proportional to the natural parameterization of curves. \~
    \return \ru Результат первичной проверки параметров.
            \en The result of the primary scan of parameters. \~
  */
  bool        Init( const MbCurve3D      & inCurve0,
                    const MbCurve3D      & inCurve1,
                    const SArray<double> & pars0,
                    const SArray<double> & pars1,
                          bool             selfInt = false,
                          bool             prop = false );

  /** \brief \ru Функция инициализации.
             \en Initialization function. \~
    \details \ru Функция инициализации на оригиналах или копиях кривых.
             \en Initialization function on the curves originals or copies of curve. \~
    \param[in] obj - \ru Копируемые параметры.
                     \en Copy parameters. \~
    \param[in] sameCurves - \ru Флаг использования оригиналов кривых.
                            \en Flag of using originals of curves. \~
  */
  void        Init( const RuledSurfaceValues & obj, bool sameCurves );

  /// \ru Первичная проверка корректности параметров. \en Initial check of parameters correctness
  bool        IsValid() const;
  /// \ru Преобразовать по матрице. \en Transform by matrix.
  void        Transform( const MbMatrix3D &, MbRegTransform * ireg );
  /// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector.
  void        Move     ( const MbVector3D &, MbRegTransform * ireg );
  /// \ru Повернуть объект вокруг оси на заданный угол. \en Rotate an object at a given angle around an axis.
  void        Rotate   ( const MbAxis3D   &, double angle, MbRegTransform * ireg );
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSame( const RuledSurfaceValues &, double accuracy ) const;

  /// \ru Получить кривую (первую или вторую). \en Get curve (the first or second).
  const MbCurve3D * GetCurve( bool first ) const { return (first ? curve0 : curve1); }
  const MbCurve3D * GetCurve0() const { return curve0; }
  const MbCurve3D * GetCurve1() const { return curve1; }
  /// \ru Получить кривую (первую или вторую). \en Get curve (the first or second).
  MbCurve3D * SetCurve( bool first )       { return (first ? curve0 : curve1); }
  MbCurve3D * SetCurve0()       { return curve0; }
  MbCurve3D * SetCurve1()       { return curve1; }

  /// \ru Выдать количество параметров разбиения. \en Get the count of splitting parameters.
  size_t      GetParamsCount( bool first ) const { return (first ? breaks0.Count() : breaks1.Count()); }
  size_t      GetBreaks0Count() const { return breaks0.Count(); }
  size_t      GetBreaks1Count() const { return breaks1.Count(); }
  /// \ru Выдать массив разбиения. \en Get splitting array.
  void        GetParams( bool first, SArray<double> & breaks ) const { breaks = (first ? breaks0 : breaks1); }
  /// \ru Получить параметр разбиения по индексу. \en Get splitting parameter by index.
  double      GetParam( bool first, size_t k ) const { C3D_ASSERT( k < GetParamsCount( first ) ); return (first ? breaks0[k] : breaks1[k]); }
  double      GetBreaks0( size_t k ) const { C3D_ASSERT( k < breaks0.Count() ); return breaks0[k]; }
  double      GetBreaks1( size_t k ) const { C3D_ASSERT( k < breaks1.Count() ); return breaks1[k]; }
  /// \ru Установить массив параметров разбиения. \en Set array of splitting parameters.
  void        SetParams( bool first, const SArray<double> & ps ) { if ( first ) breaks0 = ps; else breaks1 = ps; }
  /// \ru Заполнены ли массивы параметров разбиения? \en Whether arrays of splitting parameters are filled?
  bool        IsEmpty()  const { return (breaks0.Count() < 1); }
  /// \ru Установить флаг проверять ли самопересечения \en Set flag of necessary to check self-intersections
  void        CheckSelfInt( bool c ) { checkSelfInt = c; }
  /// \ru Нужно ли проверять самопересечения \en Whether it is necessary to check self-intersections
  bool        CheckSelfInt() const { return checkSelfInt; }
  /// \ru Установить флаг соединения через вершины \en Set flag of connection through vertices
  void        SetJoinByVertices( bool byVerts ) { joinByVertices = byVerts; }
  /// \ru Соединяются ли кривые через вершины? \en Whether curves are joined through vertices?
  bool        GetJoinByVertices() const { return joinByVertices; }
  /// \ru Установить флаг упрощения граней. \en Set flag of faces simplification.
  void        SetSimplifyFaces( bool simplFaces ) { simplifyFaces = simplFaces; }
  /// \ru Получить флаг упрощения граней. \en Get flag of faces simplification.
  bool        GetSimplifyFaces() const { return simplifyFaces; }
  /// \ru Установить флаг параметризации кривых пропорционально натуральной. \en Set flag of proportional to the natural parameterization of curves.
  void        SetProportional( bool prop ) { proportional = prop; }
  /// \ru Получить флаг параметризации кривых пропорционально натуральной. \en Get flag of proportional to the natural parameterization of curves.
  bool        GetProportional() const { return proportional; }
  /// \ru Установить допустимое несовпадение противолежащих точек стыковки сегментов. \en Set permissible mismatch of opposite points of connecting segments.
  void        SetMismatch( double mMax ) { mismatchMax = mMax; }
  /// \ru Получить допустимое несовпадение противолежащих точек стыковки сегментов. \en Get permissible mismatch of opposite points of connecting segments.
  double      GetMismatch() const { return mismatchMax; }
  /// \ru Установить флаг разделять оболочку на грани по сегментам контуров. \en Set flag of divide the shell into faces by contour segments.
  void        SetSegmentSplit( bool s ) { segmentSplit = s; }
  /// \ru Получить флаг разделять оболочку на грани по сегментам контуров. \en Get flag of divide the shell into faces by contour segments.
  bool        GetSegmentSplit() const { return segmentSplit; }

private:
  // \ru Проверить наличие параметров вершин контура в массиве параметров \en Check for loop vertices parameters in the parameters array
  bool        CheckVertices( const MbCurve3D      & curve,
                             const SArray<double> & breaks ) const;

public:
KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( RuledSurfaceValues, MATH_FUNC_EX )
OBVIOUS_PRIVATE_COPY( RuledSurfaceValues )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры удлинения оболочки.
           \en The shell extension parameters. \~
  \details \ru Параметры удлинения оболочки путём продления грани или достраивания грани. \n
           \en The parameters of extension shell by extending or face-filling. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS ExtensionValues {
public:
  /** \brief \ru Типы удлинения.
             \en Types of extension. \~
    \details \ru Типы удлинения оболочки. Указывает форму поверхности удлинения.
             \en Types of shell extension. Indicates the form extension surface. \~
  */
  enum ExtensionType {
    et_same = 0,           ///< \ru По той же поверхности. \en Along the same surface.
    et_tangent,            ///< \ru По касательной к краю. \en Along tangent to the edge.
    et_direction,          ///< \ru По направлению. \en Along the direction.
    et_total,              ///< \ru Количество всех режимов. \en Number of all modes.
  };
  /** \brief \ru Способы удлинения.
             \en Ways of extension. \~
    \details \ru Способы удлинения оболочки.
             \en Ways of shell extension. \~
  */
  enum ExtensionWay {
    ew_distance         = -2, ///< \ru Продолжить на расстояние. \en Prolong on the distance.
    ew_vertex           = -1, ///< \ru Продолжить до вершины. \en Prolong to the vertex.
    ew_shell            =  0, ///< \ru Продолжить до оболочки. \en Prolong to the shell.
  };
  /** \brief \ru Способы построения боковых рёбер.
             \en Methods of construction of the lateral edges. \~
    \details \ru Способы построения боковых рёбер при удлинении оболочки.
             \en Methods of construction of the lateral edges when extending shell. \~
  */
  enum LateralKind {
    le_normal = 0,         ///< \ru По нормали к кромке. \en Along the normal to boundary.
    le_prolong,            ///< \ru Продлить исходные рёбра. \en Extend the initial edges.
    le_total,              ///< \ru Количество всех режимов. \en Number of all modes.
  };
  /** \brief \ru Способы продления до оболочки.
             \en Ways to extend to shell. \~
    \details \ru Способы продления до оболочки.
             \en Ways to extend to shell. \~
  */
  enum ShellObstacleType {
    sot_ToShell = 0,       ///< \ru До оболочки. \en To shell.
    sot_ThroughShellLayer, ///< \ru Через первый слой оболочки (до первого выхода из оболочки). \en Through the first shell layer.
    sot_ToShellWithoutCut, ///< \ru До оболочки без подрезки. \en To shell without cutting.
    sot_ToGabaritsOfShell, ///< \ru До габаритов оболочки без подрезки. \en To gabarits of shell without cutting.
    sot_Total,             ///< \ru Количество всех режимов. \en Number of all modes.
  };
  /** \brief \ru Варианты представления результирующего тела.
             \en Options for representing the resulting body. \~
    \details \ru Варианты представления результирующего тела.
             \en Options for arepresenting the resulting body. \~
  */
  enum ExtensionOutput {
    eo_Original = 0, ///< \ru Удлинить исходные грани. \en Extend the original faces. 
    eo_newFace =  1, ///< \ru Сформировать результат продления в виде новых граней. \en Form the result of the extension in the form of new faces.
    eo_newShell = 2, ///< \ru Сформировать результат продления в виде новой оболочки. \en Form the result of the extension in the form of a new shell.
  };

public:
  ExtensionType     type;      ///< \ru Тип удлинения. \en Type of extension.
  ExtensionWay      way;       ///< \ru Способ удлинения. \en Way of extension.
  LateralKind       kind;      ///< \ru Способ построения боковых рёбер. \en Method of construction of the lateral edges.
  MbCartPoint3D     point;     ///< \ru Точка, до которой удлинить. \en The point to extend.up to which.
  MbVector3D        direction; ///< \ru Направление удлинения. \en Direction of extension.
  double            distance;  ///< \ru Расстояние. \en Distance.
  bool              prolong;   ///< \ru Продолжить по гладко стыкующимся рёбрам. \en Prolong along smoothly mating edges.
  bool              combine;   ///< \ru Объединять грани при возможности. \en Combine faces if it is possible.
  /** \brief \ru Построить упрощенную эквидистанту.
             \en Create equidistant with simplification. \~
    \details \ru Флаг, отвечающий за построение эквидистанты по средствам сдвига кривой по нормали к отрезку, соединяющему ее концы на расстояние distance.
      При этом, сдвинутая эквидистанта заменяется отрезком, если последний не пересекает габарита исходной кривой. 
             \en A flag responsible for constructing an equidistant by shifting the curve along the normal to the segment connecting its ends at a given distance.
      In this case, the shifted equidistant is replaced by a segment if the latter does not intersect the gabarit of the original curve. \~
  */
  bool              simplify;  
private:
  MbFaceShell *     shell;        ///< \ru Оболочка, до которой продляются грани. \en A shell to which the faces are extended.
  MbItemIndex       faceIndex;    ///< \ru Индекс грани в оболочке. \en The index of face in the shell.
  ExtensionOutput   output;       ///< \ru Варианты представления результирующего тела. \en Options for representing the resulting body.
  ShellObstacleType toShellType;  ///< \ru Способ продления до оболочки. \en Way to extend up to shell.
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  ExtensionValues();
  /// \ru Конструктор копирования. \en Copy-constructor.
  ExtensionValues( const ExtensionValues & other );
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \param[in] type - \ru Тип удлинения.
                      \en Type of extension. \~
    \param[in] way - \ru Способ удлинения.
                     \en Way of extension. \~
    \param[in] kind - \ru Способ построения боковых рёбер.
                      \en Method of construction of the lateral edges. \~
    \param[in] out - \ru Варианты представления результирующего тела.
                     \en Options for representing the resulting body. \~
    \param[in] pt - \ru Точка, до которой строится удлинение.
                    \en The point to construct up to. \~
    \param[in] dir - \ru Направление удлинения.
                     \en Direction of extension. \~
    \param[in] dist - \ru Расстояние. 
                      \en Distance.
    \param[in] pro - \ru Продолжить по гладко стыкующимся рёбрам. 
                     \en Prolong along smoothly mating edges. \~
    \param[in] comb - \ru Объединять грани при возможности. 
                      \en Combine faces if it is possible. \~
    \param[in] addShell - \ru Оболочка, до которой продляются грани. 
                          \en A shell to which the faces are extended. \~
    \param[in] fIndex - \ru Индекс грани в оболочке. 
                       \en The index of face in the shell. \~
  */
  ExtensionValues(       ExtensionType   type, 
                         ExtensionWay    way, 
                         LateralKind     kind, 
                         ExtensionOutput out, 
                   const MbCartPoint3D & pt,
                   const MbVector3D &    dir, 
                         double          dist, 
                         bool            pro,
                         bool            comb, 
                   const MbFaceShell *   addShell, 
                   const MbItemIndex &   fIndex );
  /** \brief \ru Конструктор по параметрам.
              \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \param[in] type - \ru Тип удлинения.
                      \en Type of extension. \~
    \param[in] way - \ru Способ удлинения.
                     \en Way of extension. \~
    \param[in] kind - \ru Способ построения боковых рёбер.
                      \en Method of construction of the lateral edges. \~
    \param[in] pt - \ru Точка, до которой строится удлинение.
                    \en The point to construct up to. \~
    \param[in] dir - \ru Направление удлинения.
                     \en Direction of extension. \~
    \param[in] dist - \ru Расстояние.
                      \en Distance.
    \param[in] pro - \ru Продолжить по гладко стыкующимся рёбрам.
                     \en Prolong along smoothly mating edges. \~
    \param[in] comb - \ru Объединять грани при возможности.
                      \en Combine faces if it is possible. \~
    \param[in] addShell - \ru Оболочка, до которой продляются грани.
                          \en A shell to which the faces are extended. \~
    \param[in] fIndex - \ru Индекс грани в оболочке.
                        \en The index of face in the shell. \~
  */
  ExtensionValues(       ExtensionType   type, 
                         ExtensionWay    way, 
                         LateralKind     kind, 
                   const MbCartPoint3D & pt,
                   const MbVector3D &    dir,
                         double          dist, 
                         bool            pro, 
                         bool            comb,
                   const MbFaceShell *   addShell, 
                   const MbItemIndex &   fIndex );
  /// \ru Деструктор. \en Destructor.
  virtual ~ExtensionValues();
public:
  /** \brief \ru Функция инициализации.
             \en Initialization function. \~
    \details \ru Функция инициализации по параметрам.
             \en Initialization function by parameters. \~
    \param[in] other - \ru Тип удлинения.
                       \en Type of extension. \~
    \param[in] copyShell - \ru Копировать режущую оболочку-препятствие
                           \en Copy obstacle shell. \~
  */
  void        Init( const ExtensionValues & other, bool copyShell = true );
  /** \brief \ru Функция инициализации.
             \en Initialization function. \~
    \details \ru Функция инициализации удлинения на расстояние.
             \en Initialization function of extending to a distance. \~
    \param[in] t - \ru Тип удлинения.
                   \en Type of extension. \~
    \param[in] k - \ru Способ построения боковых рёбер.
                   \en Method of construction of the lateral edges. \~
    \param[in] v - \ru Направление удлинения.
                   \en Direction of extension. \~
    \param[in] d - \ru Величина удлинения.
                   \en Value of extension. \~
  */
  void        InitByDistance( ExtensionType t, LateralKind k, const MbVector3D & v, double d );

  /** \brief \ru Функция инициализации.
             \en Initialization function. \~
    \details \ru Функция инициализации удлинения до вершины.
             \en Initialization function of extension to the vertex. \~
    \param[in] t - \ru Тип удлинения.
                   \en Type of extension. \~
    \param[in] k - \ru Способ построения боковых рёбер.
                   \en Method of construction of the lateral edges. \~
    \param[in] v - \ru Вершина, до которой строится удлинение.
                   \en The vertex to construct up to. \~
  */
  void        InitByVertex  ( ExtensionType t, LateralKind k, const MbCartPoint3D & v );

  /** \brief \ru Функция инициализации.
             \en Initialization function. \~
    \details \ru Функция инициализации удлинения до (грани) оболочки.
             \en Initialization function of extension to the face of a shell. \~
    \param[in] t - \ru Тип удлинения.
                   \en Type of extension. \~
    \param[in] k - \ru Способ построения боковых рёбер.
                   \en Method of construction of the lateral edges. \~
    \param[in] f - \ru Грань оболочки для остановки удлинения.
                   \en Shell face as stopper of elongation. \~
    \param[in] s - \ru Целевая оболочка до (грани) которой выполняется удлинение оболочки по ребрам.
                   \en Target shell (to the face of which) the working shell will be extending by the edges. \~
    \return \ru Возвращает true, если инициализация была выполнена успешно.
            \en Returns true, if initialization was successful or false otherwise. \~
  */
  bool        InitByShell ( ExtensionType t, LateralKind k, const MbFace * f, const MbSolid * s );
  /** \brief \ru Функция инициализации.
             \en Initialization function. \~
    \details \ru Функция инициализации удлинения до оболочки или через оболочку.
             \en Initialization function of extension up to a shell or through a shell. \~
    \param[in] t - \ru Тип удлинения.
                   \en Type of extension. \~
    \param[in] k - \ru Способ построения боковых рёбер.
                   \en Method of construction of the lateral edges. \~
    \param[in] sh - \ru Целевая оболочка до (грани) которой выполняется удлинение оболочки по ребрам.
                    \en Target shell (to the face of which) the working shell will be extending by the edges. \~
    \param[in] sot - \ru Режим обрезки с помощью оболочки.
                     \en Cutting mode using shell. \~
    \return \ru Возвращает true, если инициализация была выполнена успешно.
            \en Returns true, if initialization was successful or false otherwise. \~
  */
  bool        InitByShell( ExtensionType t, LateralKind k, const MbFaceShell * sh, ShellObstacleType sot = sot_ToShell );
  /** \brief \ru Функция инициализации.
             \en Initialization function. \~
    \details \ru Функция инициализации удлинения до оболочки или через оболочку.
             \en Initialization function of extension up to a shell or through a shell. \~
    \param[in] t - \ru Тип удлинения.
                   \en Type of extension. \~
    \param[in] k - \ru Способ построения боковых рёбер.
                   \en Method of construction of the lateral edges. \~
    \param[in] s - \ru Целевая оболочка до (грани) которой выполняется удлинение оболочки по ребрам.
                   \en Target shell (to the face of which) the working shell will be extending by the edges. \~
    \param[in] sot - \ru Режим обрезки с помощью оболочки.
                     \en Cutting mode using shell. \~
    \return \ru Возвращает true, если инициализация была выполнена успешно.
            \en Returns true, if initialization was successful or false otherwise. \~
  */
  bool        InitByShell( ExtensionType t, LateralKind k, const MbSolid * s, ShellObstacleType sot = sot_ToShell );

  /// \ru Преобразовать объект согласно матрице. \en Transform an object according to the matrix.
  void        Transform( const MbMatrix3D & matr, MbRegTransform * ireg = nullptr );
  /// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector.
  void        Move     ( const MbVector3D & to, MbRegTransform * ireg = nullptr );
  /// \ru Повернуть объект вокруг оси на заданный угол. \en Rotate an object at a given angle around an axis.
  void        Rotate   ( const MbAxis3D & axis, double ang, MbRegTransform * ireg = nullptr );

  /// \ru Получить оболочку. \en Get the shell.
  const MbFaceShell *     GetShell() const { return shell; }
  /// \ru Номер грани в оболочке. \en The index of face in the shell.
  const MbItemIndex &     GetFaceIndex() const { return faceIndex; }
  /// \ru Номер грани в оболочке. \en The index of face in the shell.
        void              SetFaceIndex( const MbItemIndex & index ) { faceIndex.Init( index ); }
  /// \ru Номер грани в оболочке. \en The index of face in the shell.
        void              SetFaceIndex( size_t index ) { faceIndex.Init( index ); }
  /// \ru Получить режим обработки оболочки-препятствия. \en Get obstacle shell processing mode.
        ShellObstacleType GetObstacleType() const { return toShellType; }
  /// \ru Получить вариант представления результирующего тела. \en Get options for representing the resulting body.
        ExtensionOutput   GetOutput() const { return output; }
  /** \brief \ru Замена оболочки (и ее выбранной грани).
             \en Replacement of shell (and its selected face). \~
    \details \ru Замена оболочки (и ее выбранной грани). Оболочка копируется.
             \en Replacement of shell (and its selected face). The shell is copied. \~
    \param[in] f - \ru Грань оболочки для остановки удлинения.
                   \en Shell face as stopper of elongation. \~
    \param[in] sh - \ru Целевая оболочка до (грани) которой выполняется удлинение оболочки по ребрам.
                    \en Target shell (to the face of which) the working shell will be extending by the edges. \~
    \return \ru Возвращает true, если инициализация была выполнена успешно.
            \en Returns true, if initialization was successful or false otherwise. \~
  */
  bool        SetShell( const MbFace * f, const MbFaceShell * sh );
  /** \brief \ru Замена оболочки (и ее выбранной грани).
             \en Replacement of shell (and its selected face). \~
    \details \ru Замена оболочки (и ее выбранной грани). Оболочка копируется.
             \en Replacement of shell (and its selected face). The shell is copied. \~
    \param[in] f - \ru Грань оболочки для остановки удлинения.
                   \en Shell face as stopper of elongation. \~
    \param[in] s - \ru Целевая оболочка до (грани) которой выполняется удлинение оболочки по ребрам.
                   \en Target shell (to the face of which) the working shell will be extending by the edges. \~
    \return \ru Возвращает true, если инициализация была выполнена успешно.
            \en Returns true, if initialization was successful or false otherwise. \~
  */
  bool        SetShell( const MbFace * f, const MbSolid * s );
  /// \ru Задать представление результирующего тела. \en Set representing the resulting body.
  void        SetOutput( ExtensionOutput out ){ output = out; }
  /// \ru Оператор присваивания. \en Assignment operator.
  void        operator = ( const ExtensionValues & other );

  /// \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSame( const ExtensionValues & other, double accuracy ) const;

KNOWN_OBJECTS_RW_REF_OPERATORS( ExtensionValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//------------------------------------------------------------------------------
/** \brief \ru Данные для построения поверхности соединения.
           \en Data for construction of surface of the joint. \~
  \details \ru Данные для построения поверхности соединения по двум кривым на поверхностях. \n
           \en Data for the construction of surface of the joint by two curves on the surfaces. \n \~
  \ingroup Shell_Building_Parameters
*/
//---
struct MATH_CLASS JoinSurfaceValues {
public:
  /** \brief \ru Типы сопряжения поверхностей.
             \en Type of surfaces join. \~
    \details \ru Типы сопряжения поверхностей определяет стыковку края сопрягаемой поверхности и поверхности сопряжения.
             \en Types of join of surfaces determines join of edge of joining surface and surface of the joint. \~
  */
  enum JoinConnType {
    js_Position = 0,  ///< \ru По позиции. \en By position.
    js_NormPlus,      ///< \ru По нормали в положительном направлении вектора нормали. \en Along the normal in the positive direction of normal vector.
    js_NormMinus,     ///< \ru По нормали в отрицательном направлении вектора нормали. \en Along the normal in the negative direction of normal vector.
    js_G1Plus,        ///< \ru По касательной к поверхности, слева по направлению касательной к кривой пересечения. \en The type of conjugation along the tangent to the surface, to the left along the tangent to the intersection curve.
    js_G1Minus,       ///< \ru По касательной к поверхности, справа по направлению касательной к кривой пересечения. \en The type of conjugation along the tangent to the surface, to the right along the tangent to the intersection curve.
    js_G2Plus,        ///< \ru По касательной к поверхности, слева по направлению касательной к кривой пересечения, гладкая. \en The type of conjugation along the tangent to the surface, to the left along the tangent to the intersection curve, smooth.
    js_G2Minus,       ///< \ru По касательной к поверхности, справа по направлению касательной к кривой пересечения, гладкая. \en The type of conjugation along the tangent to the surface, to the right along the tangent to the intersection curve, smooth.
  };
public:
  JoinConnType          connType1;        ///< \ru Тип сопряжения поверхности соединения с поверхностью 1. \en Join type of surface of the joint with the surface 1.
  JoinConnType          connType2;        ///< \ru Тип сопряжения поверхности соединения с поверхностью 2. \en Join type of surface of the joint with the surface 2.
  double                tension1;         ///< \ru Натяжение для соединения с поверхностью 1. \en Tension for joining with surface 1.
  double                tension2;         ///< \ru Натяжение для соединения с поверхностью 2. \en Tension for joining with surface 2.
  SArray<double>        breaks0;          ///< \ru Параметры разбиения первой кривой curve0. \en Splitting parameters of the first curve0 curve.
  SArray<double>        breaks1;          ///< \ru Параметры разбиения первой кривой curve1. \en Splitting parameters of the first curve1 curve.
  bool                  checkSelfInt;     ///< \ru Искать самопересечения. \en Find self-intersections.
  bool                  edgeConnType1;    ///< \ru Построение боковой границы как продолжение ребра. \en Construct lateral boundary as edge extension.
  bool                  edgeConnType2;    ///< \ru Построение боковой границы как продолжение ребра. \en Construct lateral boundary as edge extension.
  MbVector3D          * boundDirection11; ///< \ru Вектор направления, определяющий боковую границу, в точке (0, 0) поверхности. \en Direction vector determines lateral boundary in the point (0, 0) of the surface.
  MbVector3D          * boundDirection12; ///< \ru Вектор направления, определяющий боковую границу, в точке (1, 0) поверхности. \en Direction vector determines lateral boundary in the point (1, 0) of the surface.
  MbVector3D          * boundDirection21; ///< \ru Вектор направления, определяющий боковую границу, в точке (0, 1) поверхности. \en Direction vector determines lateral boundary in the point (0, 1) of the surface.
  MbVector3D          * boundDirection22; ///< \ru Вектор направления, определяющий боковую границу, в точке (1, 1) поверхности. \en Direction vector determines lateral boundary in the point (1, 1) of the surface.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  JoinSurfaceValues()
    : connType1         ( js_G1Plus )
    , connType2         ( js_G1Plus )
    , tension1          ( 0.5       )
    , tension2          ( 0.5       )
    , breaks0           ( 0, 1      )
    , breaks1           ( 0, 1      )
    , checkSelfInt      ( false     )
    , edgeConnType1     ( false     )
    , edgeConnType2     ( false     )
    , boundDirection11  ( nullptr      )
    , boundDirection12  ( nullptr      )
    , boundDirection21  ( nullptr      )
    , boundDirection22  ( nullptr      )
  {}
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  JoinSurfaceValues( JoinConnType t1, JoinConnType t2, double tens1, double tens2, bool selfInt = false )
    : connType1         ( t1      )
    , connType2         ( t2      )
    , tension1          ( tens1   )
    , tension2          ( tens2   )
    , breaks0           ( 0, 1    )
    , breaks1           ( 0, 1    )
    , checkSelfInt      ( selfInt )
    , edgeConnType1     ( false   )
    , edgeConnType2     ( false   )
    , boundDirection11  ( nullptr    )
    , boundDirection12  ( nullptr    )
    , boundDirection21  ( nullptr    )
    , boundDirection22  ( nullptr    )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  JoinSurfaceValues( const JoinSurfaceValues & other );

public:
  /// \ru Деструктор. \en Destructor.
  virtual ~JoinSurfaceValues();
  /// \ru Функция инициализации. \en Initialization function.
  bool        Init( const SArray<double> & initBreaks0,
                    const SArray<double> & initBreaks1,
                          bool             initCheckSelfInt,
                          JoinConnType     initConnType1,
                          double           initTension1,
                          bool             initEdgeConnType1,
                    const MbVector3D *     initBoundDir11,
                    const MbVector3D *     initBoundDir12,
                          JoinConnType     initConnType2,
                          double           initTension2,
                          bool             initEdgeConnType2,
                    const MbVector3D *     initBoundDir21,
                    const MbVector3D *     initBoundDir22 );
  /// \ru Функция копирования. \en Copy function.
  void        Init( const JoinSurfaceValues & other );
  /// \ru Оператор присваивания. \en Assignment operator.
  void        operator = ( const JoinSurfaceValues & other ) { Init( other ); }

  /// \ru Преобразовать объект согласно матрице. \en Transform an object according to the matrix.
  void        Transform( const MbMatrix3D &, MbRegTransform * ireg );
  /// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector.
  void        Move     ( const MbVector3D &, MbRegTransform * ireg );
  /// \ru Повернуть объект вокруг оси на заданный угол. \en Rotate an object at a given angle around an axis.
  void        Rotate   ( const MbAxis3D   &, double angle, MbRegTransform * ireg );

  /// \ru Выдать количество параметров разбивки. \en Get the count of splitting parameters.
  size_t      GetParamsCount( bool first ) const { return (first ? breaks0.size() : breaks1.size()); }
  /// \ru Получить параметры разбивки (первую или вторую группу). \en Get splitting parameters (the first or second group).
  void        GetParams( bool first, SArray<double> & breaks ) const { breaks = (first ? breaks0 : breaks1); }
  /// \ru Получить параметры разбивки (первую или вторую группу). \en Get splitting parameters (the first or second group).
  double      GetParam( bool first, size_t k ) const { C3D_ASSERT( k < GetParamsCount( first ) ); return (first ? breaks0[k] : breaks1[k]); }
  /// \ru Установить параметры разбивки. \en Set splitting parameters.
  void        SetParams( bool first, const SArray<double> & ps ) { if ( first ) breaks0 = ps; else breaks1 = ps; }
  /// \ru Параметры разбивки не заполнены? \en Whether splitting parameters are not filled?
  bool        IsEmpty()  const { return breaks0.empty(); }
  /// \ru Получить флаг проверки самопересечений. \en Get the flag of checking self-intersection.
  bool        CheckSelfInt() const { return checkSelfInt; }
  /// \ru Установить флаг проверки самопересечений. \en Set the flag of checking self-intersection.
  void        SetSelfInt( bool aChech ) { checkSelfInt = aChech; }

  /// \ru Выдать параметры параметры установки боковых граней. \en Get setting parameters of lateral faces.
  bool        GetEdgeConnType( bool isFirst = true ) const { return isFirst ? edgeConnType1 : edgeConnType2; }
  /// \ru Установить параметры параметры установки боковых граней. \en Set setting parameters of lateral faces.
  void        SetEdgeConnType( bool connType, bool isFirst = true ) { isFirst ? edgeConnType1 = connType : edgeConnType2 = connType; }

  /** \brief \ru Выдать вектор направления.
             \en Get the direction vector. \~
    \details \ru Выдать вектор направления, определяющий боковую границу.
             \en Get the direction vector determining lateral boundary. \~
    \param[in] num - \ru Номер границы:\n
                     num = 1 - вектор boundDirection11,\n
                     num = 2 - вектор boundDirection12,\n
                     num = 3 - вектор boundDirection21,\n
                     num = 4 - вектор boundDirection22.
                     \en The index of boundary:\n
                     num = 1 - vector boundDirection11,\n
                     num = 2 - vector boundDirection12,\n
                     num = 3 - vector boundDirection21,\n
                     num = 4 - vector boundDirection22. \~
  */
  const MbVector3D * GetBoundDirection( size_t num ) const;

  /** \brief \ru Установить вектор направления.
             \en Set the direction vector. \~
    \details \ru Установить вектор направления, определяющий боковую границу.
             \en Set the direction vector determining lateral boundary. \~
    \param[in] num - \ru Номер границы:\n
                     num = 1 - изменяем вектор boundDirection11,\n
                     num = 2 - изменяем вектор boundDirection12,\n
                     num = 3 - изменяем вектор boundDirection21,\n
                     num = 4 - изменяем вектор boundDirection22.
                     \en The index of boundary:\n
                     num = 1 - change vector boundDirection11,\n
                     num = 2 - change vector boundDirection12,\n
                     num = 3 - change vector boundDirection21,\n
                     num = 4 - change vector boundDirection22. \~
    \param[in] aDirect - \ru Новый вектор направления.
                         \en The new direction vector. \~
  */
  void        SetBoundDirection( size_t num, const MbVector3D * aDirect );

  /// \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSame( const JoinSurfaceValues & other, double accuracy ) const
  {
    bool isSame = false;

    if ( (other.connType1 == connType1) &&
         (other.connType2 == connType2) &&
         (other.checkSelfInt == checkSelfInt) &&
         (other.edgeConnType1 == edgeConnType1) &&
         (other.edgeConnType2 == edgeConnType2) &&
         (::fabs(other.tension1 - tension1) < accuracy) &&
         (::fabs(other.tension2 - tension2) < accuracy) )
    {
      const size_t breaksCnt0 = breaks0.size();
      const size_t breaksCnt1 = breaks1.size();
      if ( (other.breaks0.size() == breaksCnt0) && (other.breaks1.size() == breaksCnt1) ) {
        isSame = true;

        size_t k;
        for ( k = 0; k < breaksCnt0 && isSame; ++k ) {
          if ( ::fabs(other.breaks0[k] - breaks0[k]) > accuracy )
            isSame = false;
        }
        if ( isSame ) {
          for ( k = 0; k < breaksCnt1 && isSame; ++k ) {
            if ( ::fabs(other.breaks1[k] - breaks1[k]) > accuracy )
              isSame = false;
          }
        }
        if ( isSame ) {
          bool isBoundDir11 = ((other.boundDirection11 != nullptr) && (boundDirection11 != nullptr));
          bool isBoundDir12 = ((other.boundDirection12 != nullptr) && (boundDirection12 != nullptr));
          bool isBoundDir21 = ((other.boundDirection21 != nullptr) && (boundDirection21 != nullptr));
          bool isBoundDir22 = ((other.boundDirection22 != nullptr) && (boundDirection22 != nullptr));

          if ( isSame && isBoundDir11 )
            isSame = c3d::EqualVectors( *other.boundDirection11, *boundDirection11, accuracy );
          if ( isSame && isBoundDir12 )
            isSame = c3d::EqualVectors( *other.boundDirection12, *boundDirection12, accuracy );
          if ( isSame && isBoundDir21 )
            isSame = c3d::EqualVectors( *other.boundDirection21, *boundDirection21, accuracy );
          if ( isSame && isBoundDir22 )
            isSame = c3d::EqualVectors( *other.boundDirection22, *boundDirection22, accuracy );
        }
      }
    }
    return isSame;
  }

public:
KNOWN_OBJECTS_RW_REF_OPERATORS( JoinSurfaceValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
DECLARE_NEW_DELETE_CLASS( JoinSurfaceValues )
DECLARE_NEW_DELETE_CLASS_EX( JoinSurfaceValues )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры преобразования триангуляции в оболочку.
           \en Operation parameters of grids-to-shell conversion. \~
  \details \ru Параметры преобразования триангуляции в оболочку.
           \en Operation parameters of grids-to-shell conversion. \~
  \deprecated \ru Параметры устарели, взамен использовать #MbGridShellParams.
              \en The parameters are deprecated, instead use  #MbGridShellParams. \~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS GridsToShellValues {
public:
  bool sewGrids;        ///< \ru Сшивать наборы граней от разных сеток триангуляции. \en Sew together faces of grids.
  bool mergeFaces;      ///< \ru Сливать подобные грани (true). \en Whether to merge similar faces (true). 
  bool useGridSurface;  ///< \ru Использовать поверхность на базе триангуляции. \en Use the surface based on triangulation.
  bool copyAttributes;  ///< \ru Копировать атрибуты. \en Copy attributes.
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  GridsToShellValues()
    : sewGrids      ( true  )
    , mergeFaces    ( false )
    , useGridSurface( false )
    , copyAttributes( false  )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  GridsToShellValues( const GridsToShellValues & other )
    : sewGrids      ( other.sewGrids       )
    , mergeFaces    ( other.mergeFaces     )
    , useGridSurface( other.useGridSurface )
    , copyAttributes( other.copyAttributes )
  {}
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  GridsToShellValues( bool sg, bool mf, bool ugs = false )
    : sewGrids      ( sg    )
    , mergeFaces    ( mf    )
    , useGridSurface( ugs   )
    , copyAttributes( false )
  {}
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  GridsToShellValues( bool sg, bool mf, bool ugs, bool ca )
    : sewGrids      ( sg  )
    , mergeFaces    ( mf  )
    , useGridSurface( ugs )
    , copyAttributes( ca  )
  {}
  /// \ru Оператор присваивания. \en Assignment operator.
  GridsToShellValues & operator = ( const GridsToShellValues & other )
  {
    sewGrids = other.sewGrids;
    mergeFaces = other.mergeFaces;
    useGridSurface = other.useGridSurface;
    copyAttributes = other.copyAttributes;
    return *this;
  }
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры  преобразования триангуляции в оболочку.
           \en Operation parameters of grids-to-shell conversion. \~
  \details \ru Параметры преобразования триангуляции в оболочку.
           \en Operation parameters of grids-to-shell conversion. \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbGridShellParams {
private:
  bool               _sewGrids;       ///< \ru Сшивать наборы граней от разных сеток триангуляции. \en Sew together faces of grids.
  bool               _mergeFaces;     ///< \ru Сливать подобные грани (true). \en Whether to merge similar faces (true). 
  bool               _useGridSurface; ///< \ru Использовать поверхность на базе триангуляции. \en Use the surface based on triangulation.
  bool               _copyAttributes; ///< \ru Копировать атрибуты. \en Copy attributes.
  SPtr<MbSNameMaker> _operNames;      ///< \ru Именователь операции. \en An object defining names generation in the operation.
public:
  mutable IProgressIndicator * _progress;  ///< \ru Индикатор прогресса выполнения операции. \en A progress indicator of the operation.

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbGridShellParams( const MbSNameMaker & operNames, IProgressIndicator * prog = nullptr )
    : _sewGrids      ( true                   )
    , _mergeFaces    ( false                  )
    , _useGridSurface( false                  )
    , _copyAttributes( false                  )
    , _operNames     ( &operNames.Duplicate() )
    , _progress      ( prog                   )
  {}
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbGridShellParams( bool sg, bool mf, bool ugs, bool ca, const MbSNameMaker & operNames, IProgressIndicator * prog = nullptr )
    : _sewGrids      ( sg                     )
    , _mergeFaces    ( mf                     )
    , _useGridSurface( ugs                    )
    , _copyAttributes( ca                     )
    , _operNames     ( &operNames.Duplicate() )
    , _progress      ( prog                   )
  {}
  /// \ru Сшивать ли наборы граней от разных триангуляций. \en Sew together faces of grids or not.
        bool           IsSewGrids()       const { return _sewGrids; }
  /// \ru Сливать ли подобные грани. \en Merge similar faces or not.
        bool           IsMergeFaces()     const { return _mergeFaces; }
  /// \ru Использовать ли поверхности на базе триангуляции. \en Use the surface based on triangulation or not.
        bool           UseGridSurface()   const { return _useGridSurface; }
  /// \ru Копировать ли атрибуты. \en Copy attributes or not.
        bool           IsCopyAttributes() const { return _copyAttributes; }
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker & GetNameMaker()     const { return *_operNames; }

  OBVIOUS_PRIVATE_COPY( MbGridShellParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры создания срединной оболочки между выбранными гранями тела.
           \en Operation parameters of median shell between selected faces of solid. \~
  \details \ru Параметры создания срединной оболочки между выбранными гранями тела.
               Выбранные грани должны быть эквидистантны по отношению друг к другу.
               Грани должны принадлежать одному и тому же телу.
           \en Operation parameters of median shell between selected faces of solid.
               Selected face pairs should be offset from each other. 
               The faces must belong to the same body.\~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS MedianShellValues {
public:
  /** \brief \ru Тип расчета радиуса скругления между гранями срединной оболочки.
             \en Type of fillet radius calculation between faces of median shell. \~
    \details \ru Флаг можно установить через вызов MedianShellValues::SetType(). 
             \en The flag can be set by calling MedianShellValues::SetType(). \~
  */
  enum FilletType {
    tf_none     = 0,  ///< \ru Не определено. \en Undefined.
    tf_internal = 1,  ///< \ru По внутренней грани скругления. \en By internal fillet face.
    tf_external = 2,  ///< \ru По внешней грани скругления. \en By external fillet face.
    tf_average  = 3   ///< \ru По среднему значению. \en By average value.
  };

  /** \brief \ru Флаг расчета и записи в атрибуты грани значения толщины срединной оболочки.
             \en The flag of calculation and writing into face attributes the value of median shell thickness. \~
    \details \ru Флаг можно установить через вызов MedianShellValues::SetMedianAttr().
             \en The flag can be set by calling MedianShellValues::SetMedianAttr(). \~
  */
  enum MedianAttr {
    ma_none = 0, ///< \ru Не записывать. \en Do not write.
    ma_min  = 1, ///< \ru Записывать, для граней скругления, создаваемых при filletType !=tf_none, принимать толщину как минимальное значение среди соседних граней. \en Write. With setted flag filletType !=tf_none, the median thickness calculated as minimal value of adjanced faces. 
    ma_mid  = 2, ///< \ru Записывать, для граней скругления, создаваемых при filletType !=tf_none, принимать толщину как среднее значение среди соседних граней. \en Write. With setted flag filletType !=tf_none, the median thickness calculated as average value of adjanced faces.
    ma_max  = 3  ///< \ru Записывать, для граней скругления, создаваемых при filletType !=tf_none, принимать толщину как максимальное значение среди соседних граней. \en Write. With setted flag filletType !=tf_none, the median thickness calculated as maximal value of adjanced faces.
  };

  /** \brief \ru Вид входных граней.
             \en Input faces mode. \~
    \details \ru Уточняет как трактуются заданные пары граней.
             \en Clarifies how the specified face pairs are interpreted. \~
  */
  enum InputMode {
    im_pairs = 0,    ///< \ru Пары граней. \en Pairs of faces.
    im_twogroups = 1 ///< \ru Две группы граней. Первая грань в каждой паре идет в первую группу, вторая - во вторую. \en Two groups of faces. First face in each pair goes to the first group, second face goes to the second group.
  };

public:
  FilletType filletType;      ///< \ru Флаг обработки скруглений. \en Fillet processing flag.
  MedianAttr medianAttribute; ///< \ru Флаг расчета и записи толщины срединной оболочки в атрибуты граней. \en The flag of calculation and writing into face attributes the value of median shell thickness .
  double     position;        ///< \ru Параметр смещения срединной оболочки относительно первой грани из пары. По умолчанию равен 50% расстояния между гранями. \en Parameter of shift the median surface from first face in faces pair. By default is 50% from distance between faces in pair.
  double     dmin;            ///< \ru Минимальный параметр эквидистантности. \en Minimal equidistation value.
  double     dmax;            ///< \ru Максимальный параметр эквидистантности. \en Maximal equidistation value.
  bool       cutByBordes;     ///< \ru Флаг подрезки срединной оболочки границами родительской оболочки. \en Flag indicates is need to truncate median shell by parent shell faces.
  InputMode  inputMode;       ///< \ru Вид входных граней. \en Input faces mode.
  bool       detectPairs;     ///< \ru При заданном построении по двум наборам граней, попробовать распознать в них пары эквидистантных граней. \en When two groups of faces are specified, try to detect pairs of offset faces in them.
  c3d::BoolPair groupsInvNormals; ///< \ru При заданном построении по двум наборам граней каждый флаг из пары задает признак инвертирования нормалей для соответствующего набора граней. \en When two groups of faces are specified each flag from pair indicates inversion of normals for corresponding group.
  bool       respGroupExt;    ///< \ru При заданном построении по двум наборам граней, флаг определяет, будут ли при необходимости продолжены грани из ответного (второго) набора граней. \en When two groups of faces are specified, this flag defines whether faces from response (second) group may be extended when necessary.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MedianShellValues()
    : filletType      ( tf_average   )
    , medianAttribute ( ma_none      )
    , position        ( 0.5          )
    , dmin            ( 0.0          )
    , dmax            ( 0.0          )
    , cutByBordes     ( false        )
    , inputMode       ( im_pairs     )
    , detectPairs     ( false        )
    , respGroupExt    ( true         )
    , groupsInvNormals( false, false )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  MedianShellValues( const MedianShellValues & other )
    : filletType   ( other.filletType   )
    , medianAttribute( ma_none          )
    , position     ( other.position     )
    , dmin         ( other.dmin         )
    , dmax         ( other.dmax         )
    , cutByBordes  ( other.cutByBordes  )
    , inputMode    ( other.inputMode    )
    , detectPairs  ( other.detectPairs  )
    , respGroupExt ( other.respGroupExt )
    , groupsInvNormals ( other.groupsInvNormals )
  {}
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MedianShellValues( double pos, double d1, double d2, bool cut )
    : filletType   ( tf_average )
    , medianAttribute( ma_none  )
    , position     ( pos        )
    , dmin         ( d1         )
    , dmax         ( d2         )
    , cutByBordes  ( cut        )
    , inputMode    ( im_pairs   )
    , detectPairs  ( false      )
    , respGroupExt ( true       )
    , groupsInvNormals ( false, false )
  {}

public:
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool IsSame( const MedianShellValues & obj, double accuracy ) const
  {
    return ( filletType == obj.filletType ) &&
           ( medianAttribute == obj.medianAttribute ) &&
           ( ::fabs(dmin - obj.dmin) < accuracy ) &&
           ( ::fabs(dmax - obj.dmax) < accuracy ) &&
           ( ::fabs(position - obj.position) < accuracy ) &&
           ( cutByBordes == obj.cutByBordes ) &&
           ( inputMode == obj.inputMode ) &&
           ( detectPairs == obj.detectPairs ) &&
           ( respGroupExt == obj.respGroupExt ) &&
           ( groupsInvNormals == obj.groupsInvNormals );
  }

  /// \ru Выдать тип скругления. \en Get type of fillet.
  FilletType GetType() const { return filletType; }
  /// \ru Выдать тип скругления для изменения. \en Get type of fillet for changing.
  FilletType & SetType() { return filletType; }
  /// \ru Выдать тип скругления. \en Get type of fillet.
  MedianAttr GetMedianAttr() const { return medianAttribute; }
  /// \ru Выдать тип скругления для изменения. \en Get type of fillet for changing.
  MedianAttr & SetMedianAttr() { return medianAttribute; }

public:
  /// \ru Оператор присваивания. \en Assignment operator.
  MedianShellValues & operator = ( const MedianShellValues & other )
  {
    filletType = other.filletType;
    medianAttribute = other.medianAttribute;
    position = other.position;
    dmin = other.dmin;
    dmax = other.dmax;
    cutByBordes = other.cutByBordes;
    inputMode = other.inputMode;
    detectPairs = other.detectPairs;
    respGroupExt = other.respGroupExt;
    groupsInvNormals = other.groupsInvNormals;

    return *this;
  }

KNOWN_OBJECTS_RW_REF_OPERATORS( MedianShellValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//------------------------------------------------------------------------------
/** \brief \ru Множество граней для создания срединной оболочки.
           \en Set of faces for build a median shell. \~
\details \ru Множество граней для создания срединной оболочки.
         \en Set of faces for build a median shell.\~
\ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MedianShellFaces {
private:
  std::vector<c3d::ItemIndexPair> facePairs; ///< \ru Набор пар выбранных граней. \en Set of selected faces pairs.
  std::vector<double>             distances; ///< \ru Вектор смещений второй грани по отношению к первой в каждой паре. \en Vector of shift values of second face in reference to first face in each pair.
  std::vector<c3d::BoolPair>      invFacesNormals; ///< \ru Каждый флаг из пары задает признак инвертирования нормали для соответствующей грани. \en Each flag from pair indicates inversion of normal for the corresponding face.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MedianShellFaces() {
    facePairs.resize( 0 );
    distances.resize( 0 );
    invFacesNormals.resize( 0 );
  }
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MedianShellFaces( const std::vector<c3d::ItemIndexPair> & pairs )
  {
    facePairs = pairs;
    distances.resize( pairs.size() );
    invFacesNormals.resize( pairs.size() );
  }
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MedianShellFaces( const MedianShellFaces & other )
  {
    facePairs = other.facePairs;
    distances = other.distances;
    invFacesNormals = other.invFacesNormals;
  }
  /// \ru Деструктор. \en Destructor.
  ~MedianShellFaces() {}

public:
  /// \ru Преобразовать объект согласно матрице. \en Transform an object according to the matrix.
  void        Transform( const MbMatrix3D & );
  /// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector.
  void        Move( const MbVector3D & );
  /// \ru Повернуть объект вокруг оси на заданный угол. \en Rotate an object at a given angle around an axis.
  void        Rotate( const MbAxis3D   &, double angle );
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSame( const MedianShellFaces & obj, double accuracy ) const;

public:
  /// \ru Добавить в набор пару граней. \en Add pair of faces.
  void                  AddFacePair( const MbItemIndex & f1, const MbItemIndex & f2, double dist = 0.0 )
  {
    facePairs.push_back( c3d::ItemIndexPair(f1,f2) );
    distances.push_back( dist );
    invFacesNormals.emplace_back( false, false );
  }
  /// \ru Добавить в набор пару граней. \en Add pair of faces.
  void                  AddFacePair( const MbItemIndex &   f1,
                                     const MbItemIndex &   f2,
                                     const c3d::BoolPair & invNormals,
                                     const double          dist )
  {
    facePairs.push_back( c3d::ItemIndexPair(f1,f2) );
    distances.push_back( dist );
    invFacesNormals.emplace_back( invNormals );
  }
  /// \ru Получить пару граней по индексу. \en Get pair of faces by index.
  const c3d::ItemIndexPair & _GetFacePair( size_t index ) const { return facePairs[index]; }
  /// \ru Установить пару граней по индексу. \en Set pair of faces by index.
  c3d::ItemIndexPair & SetFacePair( size_t index ) { return facePairs[index]; }
  /// \ru Удалить пару граней из набора. \en Remove pair of faces from set.
  void                  RemovePairByIndex( size_t index )
  {
    facePairs.erase( facePairs.begin() + index );
    distances.erase( distances.begin() + index );
    invFacesNormals.erase( invFacesNormals.begin() + index );
  }
  /// \ru Вернуть расстояние между гранями. \en Get distance between faces.
  const double &       _GetDistance( size_t index ) const { return distances[index]; }
  /// \ru Установить расстояние между гранями. \en Set distance between faces.
  void                 _SetDistance( size_t index, double value ) { distances[index] = value; }
  /// \ru Вернуть флаги, задающие признак инвертирования нормалей для каждой из граней в паре. \en Get the flags, which indicate inversion of normals for each face in the pair.
  const c3d::BoolPair & _GetInvFacesNormals( size_t index ) const { return invFacesNormals[index]; }
  /// \ru Установить флаги, задающие признак инвертирования нормалей для каждой из граней в паре. \en Set the flags, which indicate inversion of normals for each face in the pair.
  void                  _SetInvFacesNormals( size_t index, const c3d::BoolPair & value ) { invFacesNormals[index] = value; }
  /// \ru Инвертировать пару граней в наборе. \en Inverse face pair.
  void                  Inverse( size_t index ) {
    std::swap( facePairs[index].first, facePairs[index].second );
    std::swap( invFacesNormals[index].first, invFacesNormals[index].second );
  }
  /// \ru Вернуть количество пар граней в наборе. \en Get count of pairs in given set.
  size_t                Count() const { return facePairs.size(); }
  /// \ru Оператор присваивания. \en Assignment operator.
  MedianShellFaces &    operator = ( const MedianShellFaces & other ) {
    facePairs = other.facePairs;
    distances = other.distances;
    invFacesNormals = other.invFacesNormals;
    return *this;
  }
  /// \ru Очистка текущего набора. \en Clear current faces set.
  void                  Clear() { facePairs.clear(); distances.clear(); invFacesNormals.clear(); }
  /// \ru Проверить наличие пары в наборе. \en Check if pair already in set.
  size_t                IsExist( const MbItemIndex & ind, size_t start_pos, size_t end_pos, bool & first ) const;

KNOWN_OBJECTS_RW_REF_OPERATORS( MedianShellFaces ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры построения срединной оболочки.
           \en Parameters for creating the median shell. \~
  \details \ru Параметры построения срединной оболочки.
           \en Parameters for creating the median shell.\~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbMedianShellParams : public MbPrecision {
private:
  c3d::IndicesPairsVector _faceIndexes; ///< \ru Набор пар индексов выбранных граней(может быть пустым). \en Set of selected face indices pairs(can be empty).
  MedianShellValues       _params;      ///< \ru Параметры операции. \en The operation parameters.
  SPtr<MbSNameMaker>      _operNames;   ///< \ru Именователь операции. \en An object defining names generation in the operation.
public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbMedianShellParams( const c3d::IndicesPairsVector & faceIndexes, const MedianShellValues & params, const MbSNameMaker & operNames );
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbMedianShellParams( const c3d::IndicesSet &   facesSet1,
                       const c3d::IndicesSet &   facesSet2,
                       const MedianShellValues & params,
                       const MbSNameMaker &      operNames );
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker &            GetNameMaker()    const { return *_operNames; }
  /// \ru Получить параметры операции. \en Get the operation parameters.
  const MedianShellValues &       GetParams()       const { return _params; }
  /// \ru Получить набор пар индексов выбранных граней. \en Get the set of selected face indices pairs.
  const c3d::IndicesPairsVector & GetFaceIndicies() const { return _faceIndexes; }

OBVIOUS_PRIVATE_COPY( MbMedianShellParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Результаты построения срединной оболочки.
           \en Results of creating the median shell. \~
  \details \ru Результаты построения срединной оболочки.
           \en Results of creating the median shell.\~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS MbMedianShellResults : MbOperationResults {
public:
  MedianShellFaces _medianFaces; ///< \ru Информация о гранях исходного тела, на базе которых выполнено построение. \en The source solid faces data, on the basis of which was performed the building of median shell.
  c3d::SolidSPtr   _solid;       ///< \ru Результирующее тело срединной оболочки. \en The resultant solid of median shell.

public:
  /// \ru Конструктор. \en Constructor.
  MbMedianShellResults() : MbOperationResults(), _medianFaces(), _solid() {}
OBVIOUS_PRIVATE_COPY( MbMedianShellResults )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры сшивки.
           \en Stitch parameters. \~
  \details \ru Параметры сшивки оболочек. \n
           \en Shells stitch parameters. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS MbShellStitchParams {
private:
  SPtr<MbSNameMaker>            _nameMaker;      ///< \ru Именователь операции. \en An object defining names generation in the operation.
  double                        _stitchAccuracy; ///< \ru Точность сшивки (точность поиска парных ребер). \en Stitching accuracy (search accuracy of edges pairs).
  bool                          _formSolidBody;  ///< \ru Флаг формирования твердого тела из результирующей оболочки. \en Whether to form a solid solid from the resultant shell.
  bool                          _mergeEdges;     ///< \ru Сливать подобные ребра (true). \en Whether to merge similar edges (true). 
private:
  mutable IProgressIndicator *  _progress;       ///< \ru Индикатор прогресса выполнения операции. \en A progress indicator of the operation.
private:
  MbShellStitchParams();
public:
  /** \brief \ru Конструктор параметров сшивки оболочек.
             \en Stitch faces of several solids into single solid. \~
    \details \ru Сшить стыкующиеся друг с другом грани нескольких тел в одно тело. Ориентация граней может быть изменена. \n
             \en Stitch faces of several solids with coincident edges into single solid. The faces orientation can be changed. \n \~
    \param[in] operNames - \ru Именователь операции.
                           \en An object defining names generation in the operation. \~
    \param[in] formBody - \ru Флаг формирования твердого тела из результирующей оболочки.
                          \en Whether to form a solid solid from the resultant shell. \~
    \param[in] sewingAccuracy - \ru Точность сшивки (точность поиска парных ребер).
                                \en Stitching accuracy (search accuracy of edges pairs). \~
    \param[in] edgesMerging - \ru Сливать подобные ребра (true).
                              \en Whether to merge similar edges (true). \~
  */
  MbShellStitchParams( const MbSNameMaker & operNames, bool formBody, double sewingAccuracy, bool edgesMerging = true )
    : _nameMaker     ( &operNames.Duplicate() )
    , _stitchAccuracy( sewingAccuracy         )
    , _formSolidBody ( formBody               )
    , _mergeEdges    ( edgesMerging           )
  {}
  ~MbShellStitchParams()
  {}
public:
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker &  GetNameMaker()        const { return *_nameMaker; }
  /// \ru Получить точность поиска парных ребер. \en Get search accuracy of edges pairs.
  double                GetStitchAccuracy()   const { return _stitchAccuracy; }
  /// \ru Пытаться формировать твердое тело из результирующей оболочки.. \en Whether to try forming a solid solid from the resultant shell.
  bool                  TryBodyForming()      const { return _formSolidBody; }
  /// \ru Сливать подобные ребра (true). \en Whether to merge similar edges (true).
  bool                  PerformEdgesMerging() const { return _mergeEdges; }
public:
  /// \ru Установить внешний индикатор прогресса выполнения. \en Set external progress indicator.
  void                  SetProgressIndicator( IProgressIndicator * prog ) { _progress = prog; }
  /// \ru Взять внешний индикатор прогресса выполнения. \en Take external progress indicator.
  IProgressIndicator *  TakeProgressIndicator() const { return _progress; }

OBVIOUS_PRIVATE_COPY ( MbShellStitchParams )
};


//------------------------------------------------------------------------------
/// \ru Типы продления секущих объектов (поверхностей, контура). \en Prolongation types of cutter objects (surfaces, contour).
// ---
enum MbeSurfaceProlongType {
  cspt_None                  = 0x00,  // 00000000  ///< \ru Не продлевать. \en Don't prolong. 
  cspt_Planar                = 0x01,  // 00000001  ///< \ru Плоские поверхности. \en Planar surfaces. 
  cspt_RevolutionAxis        = 0x02,  // 00000010  ///< \ru Поверхности вращения (вдоль оси). \en Revolution surfaces (along axis). 
  cspt_RevolutionAngle       = 0x04,  // 00000100  ///< \ru Поверхности вращения (по углу). \en Revolution surfaces (by angle). 
  cspt_Revolution            = 0x06,  // 00000110  ///< \ru Поверхности вращения. \en Revolution surfaces. 
  cspt_ExtrusionGeneratrix   = 0x08,  // 00001000  ///< \ru Поверхности выдавливания (по образующей). \en Extrusion surfaces (by generatrix).
  cspt_ExtrusionDistance     = 0x10,  // 00010000  ///< \ru Поверхности выдавливания (по расстоянию). \en Extrusion surfaces (by distance).
  cspt_Extrusion             = 0x18,  // 00011000  ///< \ru Поверхности выдавливания. \en Extrusion surfaces.
  cspt_Contour               = 0x20,  // 00100000  ///< \ru Продление секущего контура. \en Extension of the cutter contour.
  cspt_RuledAlongGeneratrix  = 0x40,  // 01000000  ///< \ru Линейчатая поверхность (вдоль образующей). \en Ruled Surface (along generatrix).
  cspt_RuledAcrossGeneratrix = 0x80,  // 10000000  ///< \ru Линейчатая поверхность (поперек образующей). \en Ruled Surface (across generatrix).
  cspt_Ruled                 = 0xC0,  // 11000000  ///< \ru Линейчатая поверхность. \en Ruled Surface.
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры операции резки оболочки.
           \en Shell cutting operation parameters. \~
  \details \ru Параметры операции резки оболочки. \n
               Предполагается взаимоисключающее состояние флагов при их активном состоянии в #MbShellCuttingParams : \n
               1. #cspt_Planar - продлять или нет грани на основе плоскости; \n
               2. #cspt_Revolution - продлять или нет поверхности, имеющие ось вращения (у которых функция поверхности GetCylinderAxis возвращает true), 
                  причем возможно раздельное управление : \n
                  #cspt_RevolutionAngle - замкнуть поверхность по углу, \n
                  #cspt_RevolutionAxis - продлить поверхность вдоль оси так, чтобы включить габарит тела, если это возможно; \n
               3. #cspt_Extrusion - продлять или нет поверхности, являющиеся поверхностями выдавливания (поверхность выдавливания, цилиндрическая поверхность), 
                  причем возможно раздельное управление : \n
                  #cspt_ExtrusionGeneratrix - продлить вдоль образующей для включения габарита тела или замкнуть, если образующая периодична (дуга), \n
                  #cspt_ExtrusionDistance -  продлить поверхность вдоль направления выдавливания так, чтобы включить габарит тела, если это возможно; \n
               4. #cspt_Contour - продлить режущий контур, чтобы включить габарит тела, если это возможно; \n
               5. #cspt_Ruled - продлить линейчатую поверхность, \n
                  возможно раздельное управление:\n
                  #cspt_RuledAlongGeneratrix - продлить линейчатую поверхность вдоль образующих, \n
                  #cspt_RuledAcrossGeneratrix - продлить линейчатую поверхность поперек образующих. \n
           \en Shell cutting operation parameters. \n
               The mutually exclusive state of flags is assumed when they are active in #MbShellCuttingParams: \n
               1. #cspt_Planar - extend or not the face based on the plane; \n
               2. #cspt_Revolution - extend or not surfaces having a rotation axis (for which the GetCylinderAxis surface function returns true),
                  moreover, separate control is possible: \n
                  #cspt_RevolutionAngle - close the surface by angle, \n
                  #cspt_RevolutionAxis - extend the surface along the axis so as to embrace the body, if it's possible; \n
               3. #cspt_Extrusion - to extend or not surfaces that are extrusion surfaces (extrusion surface, cylindrical surface),
                  moreover, separate control is possible: \n
                  #cspt_ExtrusionGeneratrix - extend along the generatrix to cover the body or close if the generatrix is ​​periodic (like an arc), \n
                  #cspt_ExtrusionDistance - extend the surface along the extrusion direction so as to embrace the body, if it's possible; \n
               4. #cspt_Contour - extend cutter contour so as to embrace the body, if it's possible; \n
               5. #cspt_Ruled - extend the ruled surface, \n
                  separate control possible: \n
                  #cspt_RuledAlongGeneratrix - extend the ruled surface along the generatrix, \n
                  #cspt_RuledAcrossGeneratrix - extend the ruled surface across the generatrices. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbShellCuttingParams {
public:
  /// \ru Состояние типа продления секущих поверхностей. \en State of prolongation types of cutter surfaces.
  struct ProlongState {
    typedef uint8 UintType;
  protected:
    bool      active; ///< \ru Включено ли дополнительное управление продлением. \en Whether additional prolongation control is active.
    UintType  type;   ///< \ru Тип продления секущей поверхности. \en Prolongation type of cutting surface. 
  public:
    ProlongState() : active( false ), type( cspt_None ) {}
    explicit ProlongState( MbeSurfaceProlongType t ) : active( true ), type( (UintType)t ) {}
    ProlongState( const ProlongState & ps ) { active = ps.active; type = ps.type; }
  public:
    const ProlongState &  operator =  ( const ProlongState & ps ) { active = ps.active; type = ps.type; return *this; }
          bool            operator == ( const ProlongState & ps ) const { return (active == ps.active && type == ps.type); }
  public:
          void            Reset() { active = false; type = cspt_None; }
          void            Init( const ProlongState & ps ) { active = ps.active; type = ps.type; }
          void            Init( bool a, UintType t ) { active = a; type = t; }
          bool            IsActive() const { return active; }
          void            SetActive( bool a ) { active = a; }
          UintType        GetType() const { return type;  }
          void            SetType( MbeSurfaceProlongType t ) { type  = (UintType)t; }
          void            AddType( MbeSurfaceProlongType t ) { type |= (UintType)t; }
          void            SetActiveType( bool a, MbeSurfaceProlongType t ) { active = a; type =  (UintType)t; }
          void            AddActiveType( bool a, MbeSurfaceProlongType t ) { active = a; type |= (UintType)t; }
  };
private:
  MbSplitData        cutterData;    ///< \ru Данные секущего объекта. \en Cutter object(s) data.
  MbBooleanFlags     booleanFlags;  ///< \ru Управляющие флаги булевой операции. \en Control flags of the Boolean operation.
  SPtr<MbSNameMaker> nameMaker;     ///< \ru Именователь операции. \en An object defining names generation in the operation.
  ThreeStates        retainedPart;  ///< \ru Направление отсечения (сохраняемая часть исходной оболочки). \en The direction of cutting off (a part of the source shell to be kept).
  ProlongState       prolongState;  ///< \ru Тип продления режущей поверхности. \en Prolongation type of cutter surface.
public:
  mutable IProgressIndicator * progress;  ///< \ru Индикатор прогресса выполнения операции. \en A progress indicator of the operation.

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор. \n
             \en Constructor. \n \~
    \param[in]  part           - \ru Сохраняемая часть исходной оболочки (+1, 0, -1), 0 - оставить обе части.
                                 \en A part of the source shell to be kept (+1, 0, -1), if 0 retain both parts. \~
    \param[in]  mergingFlags   - \ru Флаги слияния элементов оболочки.
                                 \en Control flags of shell items merging. \~
    \param[in]  cutAsClosed    - \ru Построить замкнутую оболочку.
                                 \en Create a closed shell. \~
    \param[in]  snMaker        - \ru Именователь операции.
                                 \en An object defining names generation in the operation. \~
    \param[in/out]  prog       - \ru Индикатор прогресса выполнения операции.
                                 \en A progress indicator of the operation. \~
  */
  MbShellCuttingParams( int part, const MbMergingFlags & mergingFlags, bool cutAsClosed, 
                        const MbSNameMaker & snMaker, IProgressIndicator * prog = nullptr )
    : cutterData  (                      )
    , booleanFlags(                      )
    , nameMaker   ( &snMaker.Duplicate() )
    , retainedPart( ts_neutral           )
    , prolongState(                      )
    , progress    ( prog                 )
  {
    booleanFlags.InitCutting( cutAsClosed );
    booleanFlags.SetMerging( mergingFlags );
    SetRetainedPart( part );
  }
  /** \brief \ru Конструктор по контуру.
             \en Constructor by a contour. \~
    \details \ru Конструктор по контуру. \n
             \en Constructor by a contour. \n \~
    \param[in]  place          - \ru Локальная система координат, в плоскости XY которой расположен двумерный контур.
                                 \en A local coordinate system the two-dimensional contour is located in XY plane of. \~
    \param[in]  contour        - \ru Двумерный контур выдавливания расположен в плоскости XY локальной системы координат.
                                 \en The two-dimensional contour of extrusion is located in XY plane of the local coordinate system. \~
    \param[in]  sameContour    - \ru Использовать исходный контур (true) или его копию (false).
                                 \en Use the source contour (true) or its copy (false). \~
    \param[in]  dir            - \ru Направление выдавливания контура.
                                 \en Extrusion direction of the contour. \~
    \param[in]  part           - \ru Сохраняемая часть исходной оболочки (+1, 0, -1), 0 - оставить обе части.
                                 \en A part of the source shell to be kept (+1, 0, -1), if 0 retain both parts. \~
    \param[in]  mergingFlags   - \ru Флаги слияния элементов оболочки.
                                 \en Control flags of shell items merging. \~
    \param[in]  cutAsClosed    - \ru Построить замкнутую оболочку.
                                 \en Create a closed shell. \~
    \param[in]  snMaker        - \ru Именователь операции.
                                 \en An object defining names generation in the operation. \~
    \param[in/out]  prog       - \ru Индикатор прогресса выполнения операции.
                                 \en A progress indicator of the operation. \~
  */
  MbShellCuttingParams( const MbPlacement3D & place, const MbContour & contour, bool sameContour, const MbVector3D & dir, int part, 
                        const MbMergingFlags & mergingFlags, bool cutAsClosed,
                        const MbSNameMaker & snMaker, IProgressIndicator * prog = nullptr )
    : cutterData  ( place, dir, contour, sameContour )
    , booleanFlags(                      )
    , nameMaker   ( &snMaker.Duplicate() )
    , retainedPart( ts_neutral           )
    , prolongState(                      )
    , progress    ( prog                 )
  {
    booleanFlags.InitCutting( cutAsClosed );
    booleanFlags.SetMerging( mergingFlags );
    SetRetainedPart( part );
  }
  /** \brief \ru Конструктор по контуру.
             \en Constructor by a contour. \~
    \details \ru Конструктор по контуру. \n
             \en Constructor by a contour. \n \~
    \param[in]  place          - \ru Локальная система координат, в плоскости XY которой расположен двумерный контур.
                                 \en A local coordinate system the two-dimensional contour is located in XY plane of. \~
    \param[in]  contour        - \ru Двумерный контур выдавливания расположен в плоскости XY локальной системы координат.
                                 \en The two-dimensional contour of extrusion is located in XY plane of the local coordinate system. \~
    \param[in]  sameContour    - \ru Использовать исходный контур (true) или его копию (false).
                                 \en Use the source contour (true) or its copy (false). \~
    \param[in]  dir            - \ru Направление выдавливания контура.
                                 \en Extrusion direction of the contour. \~
    \param[in]  mergingFlags   - \ru Флаги слияния элементов оболочки.
                                 \en Control flags of shell items merging. \~
    \param[in]  cutAsClosed    - \ru Построить замкнутую оболочку.
                                 \en Create a closed shell. \~
    \param[in]  snMaker        - \ru Именователь операции.
                                 \en An object defining names generation in the operation. \~
    \param[in/out]  prog       - \ru Индикатор прогресса выполнения операции.
                                 \en A progress indicator of the operation. \~
  */
  MbShellCuttingParams( const MbPlacement3D & place, const MbContour & contour, bool sameContour, const MbVector3D & dir, 
                        const MbMergingFlags & mergingFlags, bool cutAsClosed,
                        const MbSNameMaker & snMaker, IProgressIndicator * prog = nullptr )
    : cutterData  ( place, dir, contour, sameContour )
    , booleanFlags(                      )
    , nameMaker   ( &snMaker.Duplicate() )
    , retainedPart( ts_neutral           )
    , prolongState(                      )
    , progress    ( prog                 )
  {
    booleanFlags.InitCutting( cutAsClosed );
    booleanFlags.SetMerging( mergingFlags );
  }
  /** \brief \ru Конструктор по поверхности.
             \en Constructor by a surface. \~
    \details \ru Конструктор по поверхности. \n
             \en Constructor by a surface. \n \~
    \param[in]  surface        - \ru Режущая поверхность.
                                 \en Cutting plane. \~
    \param[in]  sameSurface    - \ru Использовать исходную поверхность (true) или её копию (false).
                                 \en Use the source surface (true) or its copy (false). \~
    \param[in]  part           - \ru Сохраняемая часть исходной оболочки (+1, 0, -1), 0 - оставить обе части.
                                 \en A part of the source shell to be kept (+1, 0, -1), if 0 retain both parts. \~
    \param[in]  mergingFlags   - \ru Флаги слияния элементов оболочки.
                                 \en Control flags of shell items merging. \~
    \param[in]  cutAsClosed    - \ru Построить замкнутую оболочку.
                                 \en Create a closed shell. \~
    \param[in]  snMaker        - \ru Именователь операции.
                                 \en An object defining names generation in the operation. \~
    \param[in/out]  prog       - \ru Индикатор прогресса выполнения операции.
                                 \en A progress indicator of the operation. \~
  */
  MbShellCuttingParams( const MbSurface & surface, bool sameSurface, int part, 
                        const MbMergingFlags & mergingFlags, bool cutAsClosed,
                        const MbSNameMaker & snMaker, IProgressIndicator * prog = nullptr )
    : cutterData  ( surface, sameSurface )
    , booleanFlags(                      )
    , nameMaker   ( &snMaker.Duplicate() )
    , retainedPart( ts_neutral           )
    , prolongState(                      )
    , progress    ( prog                 )
  {
    booleanFlags.InitCutting( cutAsClosed );
    booleanFlags.SetMerging( mergingFlags );
    SetRetainedPart( part );
  }
  /** \brief \ru Конструктор по поверхности.
             \en Constructor by a surface. \~
    \details \ru Конструктор по поверхности. \n
             \en Constructor by a surface. \n \~
    \param[in]  surface        - \ru Режущая поверхность.
                                 \en Cutting plane. \~
    \param[in]  sameSurface    - \ru Использовать исходную поверхность (true) или её копию (false).
                                 \en Use the source surface (true) or its copy (false). \~
    \param[in]  part           - \ru Сохраняемая часть исходной оболочки (+1, 0, -1), 0 - оставить обе части.
                                 \en A part of the source shell to be kept (+1, 0, -1), if 0 retain both parts. \~
    \param[in]  mergingFlags   - \ru Флаги слияния элементов оболочки.
                                 \en Control flags of shell items merging. \~
    \param[in]  cutAsClosed    - \ru Построить замкнутую оболочку.
                                 \en Create a closed shell. \~
    \param[in]  snMaker        - \ru Именователь операции.
                                 \en An object defining names generation in the operation. \~
  */
  MbShellCuttingParams( const MbSurface & surface, bool sameSurface,
                        const MbMergingFlags & mergingFlags, bool cutAsClosed,
                        const MbSNameMaker & snMaker, IProgressIndicator * prog = nullptr )
    : cutterData  ( surface, sameSurface )
    , booleanFlags(                      )
    , nameMaker   ( &snMaker.Duplicate() )
    , retainedPart( ts_neutral           )
    , prolongState(                      )
    , progress    ( prog                 )
  {
    booleanFlags.InitCutting( cutAsClosed );
    booleanFlags.SetMerging( mergingFlags );
  }
  /** \brief \ru Конструктор по оболочке.
             \en Constructor by a shell. \~
    \details \ru Конструктор по оболочке. \n
             \en Constructor by a shell. \n \~
    \param[in]  solid          - \ru Режущая оболочка.
                                 \en Cutting shell. \~
    \param[in]  sameSolid      - \ru Использовать исходную поверхность (true) или её копию (false).
                                 \en Use the source surface (true) or its copy (false). \~
    \param[in]  part           - \ru Сохраняемая часть исходной оболочки (+1, 0, -1), 0 - оставить обе части.
                                 \en A part of the source shell to be kept (+1, 0, -1), if 0 retain both parts. \~
    \param[in]  mergingFlags   - \ru Флаги слияния элементов оболочки.
                                 \en Control flags of shell items merging. \~
    \param[in]  cutAsClosed    - \ru Построить замкнутую оболочку.
                                 \en Create a closed shell. \~
    \param[in]  snMaker        - \ru Именователь операции.
                                 \en An object defining names generation in the operation. \~
  */
  MbShellCuttingParams( const MbSolid & solid, bool sameSolid,
                        int part, const MbMergingFlags & mergingFlags, bool cutAsClosed,
                        const MbSNameMaker & snMaker, IProgressIndicator * prog = nullptr )
    : cutterData  ( solid, sameSolid, true )
    , booleanFlags(                        )
    , nameMaker   ( &snMaker.Duplicate()   )
    , retainedPart( ts_neutral             )
    , prolongState(                        )
    , progress    ( prog                 )
  {
    booleanFlags.InitCutting( cutAsClosed );
    booleanFlags.SetMerging( mergingFlags );
    SetRetainedPart( part );
  }
  /// \ru Копирующий конструктор. \en Copy constructor. 
  MbShellCuttingParams( const MbShellCuttingParams & other, MbRegDuplicate * iReg )
    : cutterData  ( other.cutterData, false, iReg )
    , booleanFlags( other.booleanFlags )
    , nameMaker   ( other.nameMaker    )
    , retainedPart( other.retainedPart )
    , prolongState( other.prolongState )
    , progress    ( other.progress     )
  {
  }
  ~MbShellCuttingParams()
  {}
public:
  /** \brief \ru Инициализация по контуру.
             \en Initialize by a contour. \~
    \details \ru Инициализация по контуру. \n
             \en Initialize by a contour. \n \~
    \param[in]  place          - \ru Локальная система координат, в плоскости XY которой расположен двумерный контур.
                                 \en A local coordinate system the two-dimensional contour is located in XY plane of. \~
    \param[in]  contour        - \ru Двумерный контур выдавливания расположен в плоскости XY локальной системы координат.
                                 \en The two-dimensional contour of extrusion is located in XY plane of the local coordinate system. \~
    \param[in]  sameContour    - \ru Использовать исходный контур (true) или его копию (false).
                                 \en Use the source contour (true) or its copy (false). \~
    \param[in]  dir            - \ru Направление выдавливания контура.
                                 \en Extrusion direction of the contour. \~
    \param[in]  part           - \ru Сохраняемая часть исходной оболочки (+1, 0, -1), 0 - оставить обе части.
                                 \en A part of the source shell to be kept (+1, 0, -1), if 0 retain both parts. \~
    \param[in]  mergingFlags   - \ru Флаги слияния элементов оболочки.
                                 \en Control flags of shell items merging. \~
    \param[in]  cutAsClosed    - \ru Построить замкнутую оболочку.
                                 \en Create a closed shell. \~
    \param[in]  snMaker        - \ru Именователь операции.
                                 \en An object defining names generation in the operation. \~
  */
  bool InitPlaneContour( const MbPlacement3D & place, const MbContour & contour, bool sameContour, const MbVector3D & dir,
                         int part, const MbMergingFlags & mergingFlags, bool cutAsClosed,
                         const MbSNameMaker & snMaker, IProgressIndicator * prog = nullptr )
  {
    if ( cutterData.InitPlaneContour( place, dir, contour, sameContour ) ) {
      nameMaker->SetNameMaker( snMaker, true );
      booleanFlags.InitCutting( cutAsClosed );
      booleanFlags.SetMerging( mergingFlags );
      SetRetainedPart( part );
      prolongState.Reset();
      progress = prog;
      return true;
    }
    return false;
  }
  /** \brief \ru Инициализация по контуру.
             \en Initialize by a contour. \~
    \details \ru Инициализация по контуру. \n
             \en Initialize by a contour. \n \~
    \param[in]  place          - \ru Локальная система координат, в плоскости XY которой расположен двумерный контур.
                                 \en A local coordinate system the two-dimensional contour is located in XY plane of. \~
    \param[in]  contour        - \ru Двумерный контур выдавливания расположен в плоскости XY локальной системы координат.
                                 \en The two-dimensional contour of extrusion is located in XY plane of the local coordinate system. \~
    \param[in]  sameContour    - \ru Использовать исходный контур (true) или его копию (false).
                                 \en Use the source contour (true) or its copy (false). \~
    \param[in]  dir            - \ru Направление выдавливания контура.
                                 \en Extrusion direction of the contour. \~
  */
  bool InitPlaneContour( const MbPlacement3D & place, const MbContour & contour, bool sameContour, const MbVector3D & dir, IProgressIndicator * prog = nullptr )
  {
    if ( cutterData.InitPlaneContour( place, dir, contour, sameContour ) ) {
      prolongState.Reset();
      progress = prog;
      return true;
    }
    return false;
  }
  /** \brief \ru Конструктор по поверхности.
             \en Constructor by a surface. \~
    \details \ru Конструктор по поверхности. \n
             \en Constructor by a surface. \n \~
    \param[in]  surface        - \ru Режущая поверхность.
                                 \en Cutting plane. \~
    \param[in]  sameSurface    - \ru Использовать исходную поверхность (true) или её копию (false).
                                 \en Use the source surface (true) or its copy (false). \~
    \param[in]  part           - \ru Сохраняемая часть исходной оболочки (+1, 0, -1), 0 - оставить обе части.
                                 \en A part of the source shell to be kept (+1, 0, -1), if 0 retain both parts. \~
    \param[in]  mergingFlags   - \ru Флаги слияния элементов оболочки.
                                 \en Control flags of shell items merging. \~
    \param[in]  cutAsClosed    - \ru Построить замкнутую оболочку.
                                 \en Create a closed shell. \~
    \param[in]  snMaker        - \ru Именователь операции.
                                 \en An object defining names generation in the operation. \~
  */
  bool InitSurface( const MbSurface & surface, bool sameSurface,
                    int part, const MbMergingFlags & mergingFlags, bool cutAsClosed,
                    const MbSNameMaker & snMaker, IProgressIndicator * prog = nullptr )
  {
    if ( cutterData.InitSurfaces( surface, sameSurface ) ) {
      nameMaker->SetNameMaker( snMaker, true );
      booleanFlags.InitCutting( cutAsClosed );
      booleanFlags.SetMerging( mergingFlags );
      SetRetainedPart( part );
      prolongState.Reset();
      progress = prog;
      return true;
    }
    return false;
  }
  /** \brief \ru Конструктор по поверхности.
             \en Constructor by a surface. \~
    \details \ru Конструктор по поверхности. \n
             \en Constructor by a surface. \n \~
    \param[in]  surface        - \ru Режущая поверхность.
                                 \en Cutting plane. \~
    \param[in]  sameSurface    - \ru Использовать исходную поверхность (true) или её копию (false).
                                 \en Use the source surface (true) or its copy (false). \~
    \param[in]  prType         - \ru Тип продления режущей поверхности.
                                 \en Cutter surface prolong type. \~
  */
  bool InitSurface( const MbSurface & surface, bool sameSurface, ProlongState prState, IProgressIndicator * prog = nullptr )
  {
    if ( cutterData.InitSurfaces( surface, sameSurface ) ) {
      prolongState.Init( prState );
      progress = prog;
      return true;
    }
    return false;
  }
  /** \brief \ru Конструктор по оболочке.
             \en Constructor by a shell. \~
    \details \ru Конструктор по оболочке. \n
             \en Constructor by a shell. \n \~
    \param[in]  solid          - \ru Режущая оболочка.
                                 \en Cutting shell. \~
    \param[in]  sameSolid      - \ru Использовать исходную поверхность (true) или её копию (false).
                                 \en Use the source surface (true) or its copy (false). \~
    \param[in]  part           - \ru Сохраняемая часть исходной оболочки (+1, 0, -1), 0 - оставить обе части.
                                 \en A part of the source shell to be kept (+1, 0, -1), if 0 retain both parts. \~
    \param[in]  mergingFlags   - \ru Флаги слияния элементов оболочки.
                                 \en Control flags of shell items merging. \~
    \param[in]  cutAsClosed    - \ru Построить замкнутую оболочку.
                                 \en Create a closed shell. \~
    \param[in]  snMaker        - \ru Именователь операции.
                                 \en An object defining names generation in the operation. \~
  */
  bool InitSolid( const MbSolid & solid, bool sameSolid,
                  int part, const MbMergingFlags & mergingFlags, bool cutAsClosed,
                  const MbSNameMaker & snMaker, IProgressIndicator * prog = nullptr )
  {
    if ( cutterData.InitSolid( solid, sameSolid, true ) ) {
      nameMaker->SetNameMaker( snMaker, true );
      booleanFlags.InitCutting( cutAsClosed );
      booleanFlags.SetMerging( mergingFlags );
      SetRetainedPart( part );
      prolongState.Reset();
      progress = prog;
      return true;
    }
    return false;
  }
  /** \brief \ru Конструктор по оболочке.
             \en Constructor by a shell. \~
    \details \ru Конструктор по оболочке. \n
             \en Constructor by a shell. \n \~
    \param[in]  solid          - \ru Режущая оболочка.
                                 \en Cutting shell. \~
    \param[in]  sameSolid      - \ru Использовать исходную поверхность (true) или её копию (false).
                                 \en Use the source surface (true) or its copy (false). \~
  */
  bool InitSolid( const MbSolid & solid, bool sameSolid, IProgressIndicator * prog = nullptr )
  {
    if ( cutterData.InitSolid( solid, sameSolid, true ) ) {
      prolongState.Reset();
      progress = prog;
      return true;
    }
    return false;
  }
  /** \brief \ru Конструктор по оболочке.
             \en Constructor by a shell. \~
    \details \ru Конструктор по оболочке. \n
             \en Constructor by a shell. \n \~
    \param[in]  creators       - \ru Построители режущей оболочки.
                                 \en Cutting shell creators. \~
    \param[in]  sameCreators   - \ru Использовать оригиналы (true) или копии (false) построителей.
                                 \en Use original creators (true) or its copies (false). \~
  */
  template <class CreatorsVector>
  bool InitSolid( const CreatorsVector & creators, bool sameCreators, IProgressIndicator * prog = nullptr )
  {
    if ( cutterData.InitSolid( creators, sameCreators ) ) {
      prolongState.Reset();
      progress = prog;
      return true;
    }
    return false;
  }
public:
  /// \ru Это резка плоским контуром? \en Is cutting by planar contour?
        bool              IsCuttingByPlanarContour()  const { return (cutterData.GetSketchCurvesCount() > 0 && cutterData.GetSketchCurve(0) != nullptr); }
  /// \ru Это резка поверхностью? \en Is cutting by surface?
        bool              IsCuttingBySurface() const { return (cutterData.GetSurfacesCount() > 0 && cutterData.GetSurface(0) != nullptr); }
  /// \ru Это резка оболочкой? \en Is cutting by shell?
        bool              IsCuttingBySolid()   const { return (cutterData.GetCreatorsCount() > 0 && cutterData.GetCreator(0) != nullptr) || (cutterData.GetSolidShell() != nullptr); }

  /// \ru Получить данные секущего объекта. \en Get cutter object(s) data.
  const MbSplitData &     GetCutterData() const { return cutterData; }

  /// \ru Получить управляющие флаги булевой операции. \en Get control flags of the Boolean operation.
  const MbBooleanFlags &  GetBooleanFlags() const { return booleanFlags; }
  /// \ru Получить управляющие флаги булевой операции. \en Get control flags of the Boolean operation.
        MbBooleanFlags &  SetBooleanFlags()       { return booleanFlags; }

  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker &    GetNameMaker() const { return *nameMaker;  }

  /// \ru Получить требование по оставляемой части. \en Get retained part demand.
        ThreeStates       GetRetainedPart() const { return retainedPart; }
  /// \ru Установить требование по оставляемой части. \en Set retained part demand.
        void              SetRetainedPart( int part );

  /// \ru Получить тип продления режущей поверхности. \en Get cutter surface prolong type.
  const ProlongState &    GetProlongState() const { return prolongState; }
  /// \ru Сбросить тип продления режущей поверхности. \en Reset cutter surface prolong type.
        void              ResetProlongState() { prolongState.Reset(); }
  /// \ru Добавить тип продления режущей поверхности. \en Add cutter surface prolong type.
        void              SetSurfaceProlongType( MbeSurfaceProlongType pt ) { prolongState.SetActiveType( true, pt ); }
  /// \ru Добавить тип продления режущей поверхности. \en Add cutter surface prolong type.
        void              AddSurfaceProlongType( MbeSurfaceProlongType pt ) { prolongState.AddActiveType( true, pt ); }

  /// \ru Получить локальную систему координат двумерных кривых. \en Get the local coordinate system of two-dimensional curves. 
  const MbPlacement3D &   GetSketchPlace() const { return cutterData.GetSketchPlace(); }
  /// \ru Получить вектор направления выдавливания двумерных кривых. \en Get the extrusion direction vector of two-dimensional curves. 
  const MbVector3D &      GetSketchDirection() const { return cutterData.GetSketchDirection(); }
  /// \ru Получить двумерную кривую. \en Get two-dimensional curve. 
  const MbContour *       GetSketchCurve() const { return cutterData.GetSketchCurve( 0 ); }

  /// \ru Получить поверхность. \en Get a surface. 
  const MbSurface *       GetSurface() const { return cutterData.GetSurface( 0 );  }

  /// \ru Сливать подобные грани (true)? \en Whether to merge similar faces (true)? 
        bool              MergeFaces() const { return booleanFlags.MergeFaces(); }
  /// \ru Сливать подобные ребра (true)? \en Whether to merge similar edges (true)? 
        bool              MergeEdges() const { return booleanFlags.MergeEdges(); }
  /// \ru Построить замкнутую оболочку. \en Create a closed shell.
        bool              IsCuttingAsClosed() const  { return booleanFlags.IsCutting(); }

OBVIOUS_PRIVATE_COPY ( MbShellCuttingParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры операции усечения оболочки.
           \en Shell truncating operation parameters. \~
  \details \ru Параметры операции усечения оболочки. \n
           \en Shell truncating operation parameters. \n \~
  \ingroup Shell_Building_Parameters
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbTruncateShellParams : public MbPrecision {
private:
  c3d::IndicesVector        _selIndices;          ///< \ru Номера выбранных граней (если массив пуст, то вся оболочка). \en The numbers of selected faces (if the array is empty, the whole shell is selected).
  c3d::SpaceItemsSPtrVector _truncatingItems;     ///< \ru Усекающие объекты ( кривые, поверхности или тела ). \en Truncating objects ( curves, surfaces or solids ).
  c3d::BoolVector           _truncatingOrients;   ///< \ru Ориентации усекающих объектов. \en  The truncating objects orientations.
  MbeCopyMode               _truncatingCopyMode;  ///< \ru Режим копирования усекающих оболочек. \en Whether to copy the truncating shells.
  bool                      _truncatingSplitMode; ///< \ru Кривые используются как линии разъема. \en The curves are used as parting lines.
  MbMergingFlags            _mergeFlags;          ///< \ru Флаги слияния элементов оболочки. \en Control flags of shell items merging.
  c3d::SNameMakerSPtr       _nameMaker;           ///< \ru Именователь. \en An object for naming the new objects.
  
private:
  MbTruncateShellParams(); /// \ru Конструктор по умолчанию. Запрещен. \en. Default constructor. Forbidden.

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
  \details \ru Конструктор по набору граней, усекающим объектам и их ориентациям. \n
           \en Constructor by set of curves and object defining names generation in the operation. \n \~
  \param[in] selIndices -          \ru Номера выбранных граней.
                                   \en The numbers of selected faces. \~
  \param[in] truncatingItems -     \ru Усекающие объекты ( кривые, поверхности или тела ).
                                   \en Truncating objects ( curves, surfaces or solids ). \~
  \param[in] truncatingOrients -   \ru Ориентации усекающих объектов.
                                   \en The truncating objects orientations. \~
  \param[in] truncatingCopyMode -  \ru Режим копирования усекающих оболочек.
                                   \en Whether to copy the truncating shells. \~
  \param[in] truncatingSplitMode - \ru Кривые используются как линии разъема.
                                   \en The curves are used as parting lines. \~
  \param[in] mergeFlags -          \ru Флаги слияния элементов оболочки.
                                   \en Control flags of shell items merging. \~
  \param[in] nameMaker -           \ru Именователь.
                                   \en An object for naming the new objects. \~
*/
  template<class IndicesVec, class ItemVec, class BoolVec>
  MbTruncateShellParams( const IndicesVec &   selIndices,
                               ItemVec &      truncatingItems,
                         const BoolVec &      truncatingOrients,
                               MbeCopyMode    truncatingCopyMode,
                               bool           truncatingSplitMode,
                               MbMergingFlags mergeFlags,
                         const MbSNameMaker & nameMaker );

  /// \ru Деструктор. \en Destructor. \~
  ~MbTruncateShellParams() {};

public:
  /// \ru Получить номера выбранных граней. \en Get the numbers of selected faces.
  void GetIndices( c3d::IndicesVector & indices ) const { indices.assign( _selIndices.begin(), _selIndices.end() ); }
  /// \ru Получить усекающие объекты. \en Get the truncating objects.
  void GetTruncateItems( c3d::SpaceItemsSPtrVector & items ) const { items.assign( _truncatingItems.begin(), _truncatingItems.end() ); }
  /// \ru Получить ориентации усекающих объектов. \en Get the truncating objects orientation.
  void GetTruncateOrients( c3d::BoolVector & orients ) const { orients.assign( _truncatingOrients.begin(), _truncatingOrients.end() ); }
  /// \ru Режим копирования усекающих оболочек. \en Whether to copy the truncating shells.
  MbeCopyMode GetTruncateCopyMode() const { return _truncatingCopyMode; }
  /// \ru Кривые используются как линии разъема. \en The curves are used as parting lines.
  bool GetTruncateSplitMode() const { return _truncatingSplitMode; }
  /// \ru Флаги слияния элементов оболочки. \en Control flags of shell items merging.
  const MbMergingFlags & GetMerging() const { return _mergeFlags; }
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker & GetNameMaker() const { return *_nameMaker; }

private:
  /// \ru Инициализация параметров операции усечения. \en The initialization of truncating operation parameters.
  template<class IndicesVec, class ItemVec, class BoolVec>
  void InitTruncateParams( const IndicesVec & indices, ItemVec & items, const BoolVec & orients );

OBVIOUS_PRIVATE_COPY( MbTruncateShellParams )
};


//------------------------------------------------------------------------------
// \ru Шаблонный конструктор. \en The Template constructor.
// ---
template<class IndicesVec, class ItemVec,  class BoolVec >
MbTruncateShellParams::MbTruncateShellParams( const IndicesVec &    indices,
                                                    ItemVec &       items,
                                              const BoolVec &       orients,
                                                    MbeCopyMode     copyMode,
                                                    bool            splitMode,
                                                    MbMergingFlags  mergeFlags,
                                              const MbSNameMaker &  names )
  : MbPrecision()
  , _selIndices( )
  , _truncatingItems( )
  , _truncatingOrients( )
  , _truncatingCopyMode( copyMode )
  , _truncatingSplitMode( splitMode )
  , _mergeFlags( mergeFlags )
  , _nameMaker ( &(names.Duplicate()) )
{
  InitTruncateParams( indices, items, orients );
}


//------------------------------------------------------------------------------
// \ru Инициализация параметров операции усечения. \en The initialization of truncating operation parameters.
// ---
template<class IndicesVec, class ItemVec, class BoolVec>
void MbTruncateShellParams::InitTruncateParams( const IndicesVec & indices, ItemVec & items, const BoolVec & orients )
{
  const size_t indicesCnt = indices.size();
  const size_t itemsCnt   = items.size();
  const size_t orientsCnt = orients.size();

  _truncatingItems.clear();
  _truncatingItems.reserve( itemsCnt );
  for ( size_t i = 0; i < itemsCnt; ++i ) {
    MbSpaceItem * item = items[i];
    if ( item != nullptr )
      _truncatingItems.emplace_back( item );
  }

  _truncatingOrients.clear();
  _truncatingOrients.reserve( orientsCnt );
  for ( size_t i = 0; i < orientsCnt; ++i )
    _truncatingOrients.push_back( orients[i] );

  _selIndices.clear();
  _selIndices.reserve( indicesCnt );
  for ( size_t i = 0; i < indicesCnt; ++i )
    _selIndices.push_back( indices[i] );
}


//------------------------------------------------------------------------------
/** \brief \ru Результаты операции усечения оболочки.
           \en Results of shell truncating operation. \~
  \details \ru Результаты операции усечения оболочки.
           \en Results of shell truncating operation. \~
  \ingroup ingroup Shell_Building_Parameters
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MbTruncateShellResults {
private:
  c3d::SolidSPtr _solid;       ///< \ru Результирующее тело. \en The resulting solid.
  MbPlacement3D  _resultPlace; ///< \ru Фантомное направление усечения. \en A phantom direction of truncation.

public:
  /// \ru Конструктор. \en Constructor.
  MbTruncateShellResults()
    : _solid()
    , _resultPlace()
  {}
  /// \ru Деструктор. \en Destructor.
  ~MbTruncateShellResults() {}

  /// \ru Конструктор копирования. \en Copy-constructor.
  MbTruncateShellResults( const MbTruncateShellResults & other )
    : _solid( other._solid )
    , _resultPlace( other._resultPlace )
  {
  }
  /// \ru Оператор присваивания. \en Assignment operator.
  MbTruncateShellResults & operator = ( const MbTruncateShellResults & other ) {
    _solid       = other._solid;
    _resultPlace = other._resultPlace;
    return *this;
  }

public:
  /// \ru Функция инициализации. \en Initialization function.
  void Init( const c3d::SolidSPtr & solid, const MbPlacement3D & resultPlace ) {
    _solid = solid;
    _resultPlace = resultPlace;
  }

  /// \ru Получить результирующее тело. \en Get resulting solid.
  c3d::SolidSPtr GetResultSolid() const { return _solid; }
  /// \ru Получить фантомное направление усечения. \en Get phantom direction of truncation.
  const MbPlacement3D & GetResultPlace() const { return _resultPlace; }

  /// \ru Отцепить результирующее тело. \en Detach resulting solid. 
  MbSolid * DetachSolid() { return _solid.detach(); }
};


//------------------------------------------------------------------------------
// \ru Установить требование по оставляемой части. \en Set retained part demand.
// ---
inline
void MbShellCuttingParams::SetRetainedPart( int part )
{
  retainedPart = ts_neutral;    // If part == 0 retain both parts 
  if ( part > 0 )               // If part > 0
    retainedPart = ts_positive; // retain a part above cutter surface.
  else if ( part < 0 )          // if part < 0
    retainedPart = ts_negative; // Retain part below cutter surface.
}


//------------------------------------------------------------------------------
/** \brief \ru Параметры развёртки грани на плоскость.
           \en Parameter for an unwrapping the face on a plane. \~
  \details \ru Параметры развёртки грани на плоскость. \n
    Параметры содержат информацию о положении развёртки и свойствах материала.
           \en Parameter for an unwrapping the face on a plane. \n
    The parameters contain information about the scan position and material properties. \~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS RectifyValues {
protected :
  MbPlacement3D place;    ///< \ru Локальная система координат развернутой поверхности грани. \en The local coordinate system for result surface. \~
  MbCartPoint   init;     ///< \ru Параметры поверхности, которые будут соответствовать начальной точке place. \en The parameters of the surface, which will correspond to the origin of the place. \~
  MbStepData    stepData; ///< \ru Данные для вычисления шага при триангуляции. \en Data for step calculation during triangulation. \~
  double        myu;      ///< \ru Коэффициент Пуассона материала грани. \en The Poisson's ratio of face material. \~
  bool          faceted;  ///< \ru Добавить в атрибуты мозаичный объект (true) или нет (false). \en Add to the attributes a mosaic object (true), or don't. \~

public :
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
    \details \ru Конструктор параметров операции удаления из тела выбранных граней.
             \en Constructor of operation parameters of removing the specified faces from the solid. \~
  */
  RectifyValues()
    : place()
    , init()
    , stepData()
    , myu( 0.25 )
    , faceted( false )
  {}
  /// \ru Конструктор по способу модификации и вектору перемещения. \en Constructor by way of modification and movement vector.
  RectifyValues( const MbPlacement3D & pl, const MbCartPoint & r, const MbStepData & s, double m = 0.25, bool fset = false )
    : place( pl )
    , init( r )
    , stepData( s )
    , myu( m )
    , faceted( fset )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  RectifyValues( const RectifyValues & other )
    : place   ( other.place )
    , init    ( other.init )
    , stepData( other.stepData )
    , myu     ( other.myu )
    , faceted ( other.faceted )
  {}
  /// \ru Деструктор. \en Destructor.
  ~RectifyValues() {}
public:
  /// \ru Функция копирования. \en Copy function.
  void        Init( const RectifyValues & other ) {
    place.Init( other.place );
    init.Init( other.init );
    stepData.Init( other.stepData );
    myu     = other.myu;
    faceted = other.faceted;
  }
  /// \ru Оператор присваивания. \en Assignment operator.
  RectifyValues & operator = ( const RectifyValues & other ) {
    place.Init( other.place );
    init.Init( other.init );
    stepData.Init( other.stepData );
    myu     = other.myu;
    faceted = other.faceted;
    return *this;
  }

  // \ru Локальная система координат развернутой поверхности грани. \en The local coordinate system for result surface. \~
  void        SetPlacement( const MbPlacement3D & pl ) { place.Init( pl ); } 
  const MbPlacement3D & GetPlacement() const { return place; }
  // \ru Параметры поверхности, которые будут соответствовать начальной точке place. \en The parameters of the surface, which will correspond to the origin of the place. \~
  void        SetOrigin( const MbCartPoint & r ) { init = r; }
  const MbCartPoint & GetOrigin()    const { return init; }
  // \ru Данные для вычисления шага при триангуляции. \en Data for step calculation during triangulation. \~
  void        SetStepData( const MbStepData & step ) { stepData = step; } 
  const MbStepData & GetStepData()  const { return stepData; }
  // \ru Коэффициент Пуассона материала грани. \en The Poisson's ratio of face material. \~
  void        SetPoissonsRatio( double m ) { myu = m; }
  double      GetPoissonsRatio() const { return myu; }
  // \ru Добавить в атрибуты мозаичный объект (true) или нет (false). \en Add to the attributes a mosaic object (true), or don't. \~
  void        SetAddFacet( bool b ) { faceted = b; }
  bool        AddFacet() const { return faceted; }

  /// \ru Преобразовать объект согласно матрице. \en Transform an object according to the matrix.
  void        Transform( const MbMatrix3D & matr );
  /// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector.
  void        Move     ( const MbVector3D & to );
  /// \ru Повернуть объект вокруг оси на заданный угол. \en Rotate an object at a given angle around an axis.
  void        Rotate   ( const MbAxis3D & axis, double ang );
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        IsSame( const RectifyValues & other, double accuracy ) const; 

KNOWN_OBJECTS_RW_REF_OPERATORS( RectifyValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
}; // RectifyValues


//------------------------------------------------------------------------------
/** \brief \ru Класс параметров для построения фантома габаритного куба в локальной системе координат.
           \en Parameter class for the creation of a bounding box phantom in a local coordinate system. \~
  \details \ru Класс параметров для построения фантома габаритного куба в локальной системе координат. \n
           \en Parameter class for the creation of a bounding box phantom in a local coordinate system. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbLocalCubePhantomParams 
{
protected:
  MbPlacement3D _place;  ///< \ru Локальная система координат (ЛСК). \en A local coordinate system (LCS). \~
  MbSNameMaker  _names;  ///< \ru Именователь. \en An object for the new objects naming. \~
  bool          _bScale; ///< \ru Является ли ЛСК масштабирующей. \en Whether the LCS is scaling. \~

  MbLocalCubePhantomParams(); ///< \ru Конструктор по-умолчанию запрещён. \en The default constructor is forbidden. \~

public:
  /// \ru Конструктор по параметрам. \en Parameter constructor. \~
  MbLocalCubePhantomParams ( const MbPlacement3D & place,
                                   bool            bScale,
                             const MbSNameMaker  & names );
  /// \ru Конструктор копирования. \en Copy constructor. \~
  MbLocalCubePhantomParams ( const MbLocalCubePhantomParams & other );
  /// \ru Возвращает локальную систему координат. \en Get the local coordinate system. \~
  const MbPlacement3D & GetPlacement() const { return _place; }
  /// \ru Возвращает генератор имен (именователь) топологических объектов. \en Get a generator of names (name-maker) of topological objects. \~
  const MbSNameMaker  & GetNameMaker() const { return _names; }
  /// \ru Является ли ЛСК масштабирующей. \en Whether the LCS is scaling. \~
  bool IsScalable () const { return _bScale; }

  /// \ru Установить локальную систему координат. \en Set the local coordinate system. \~
  void SetPlacement ( const MbPlacement3D & place ) { _place = place; }
  /// \ru Установить генератор имен (именователь) топологических объектов. \en Set a generator of names (name-maker) of topological objects. \~
  void SetNameMaker ( const MbSNameMaker  & names, bool version = false ) { _names.SetNameMaker( names, version ); }
  /// \ru Установить, является ли ЛСК масштабирующей. \en Set whether the LCS is scaling. \~
  void SetScalable  ( bool bScale ) { _bScale = bScale; }

  /// \ru Оператор копирования. \en Copy operator. \~
  void operator = ( const MbLocalCubePhantomParams & other );
};


//------------------------------------------------------------------------------
// \ru Инициализация массива SPtr<Type>. \en Initialization of a array of SPtr<Type>.
//  \param[in]  initArr  - \ru Начальный массив указателей на Type. \en Initial array of pointers of Type.
//  \param[out] destArr  - \ru Результирующий массив объектов SPtr<Type>. \en Result array of objects SPtr<Type>.
// ---
template<class PtrArray, class Type>
void InitSPtrArray( const PtrArray & initArr, std::vector<SPtr<Type>> & destArr )
{
  destArr.clear();
  destArr.reserve( initArr.size() );
  for ( size_t i = 0; i < initArr.size(); ++i ) {
    Type * obj = initArr[i];
    destArr.push_back( SPtr<Type>( obj ) );
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Параметры построения фаски ребер тела.
           \en Parameters for creating chamfers for edges of a solid. \~
  \details \ru Параметры построения фаски ребер тела.
           \en Parameters for creating chamfers for edges of a solid. \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbShellChamferParams
{
protected:
  c3d::EdgesSPtrVector  _initCurves; ///< \ru Массив скругляемых ребер тела. \en An array of edges to create chamfer on.
  SPtr<MbSNameMaker>    _nameMaker;  ///< \ru Именователь операции. \en An object defining names generation in the operation.
public:
  SmoothValues          _params;     ///< \ru Параметры операции. \en The operation parameters.

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор параметров построения фаски ребер тела.
             \en Constructor of parameters for creating chamfers for edges of a solid. \~
    \param[in] initCurves - \ru Массив скругляемых ребер тела.
                            \en An array of edges to create chamfer on. \~
    \param[in] params     - \ru Параметры операции.
                            \en The operation parameters. \~
    \param[in] names      - \ru Именователь.
                            \en An object for naming the new objects. \~
  */
  MbShellChamferParams( const c3d::EdgesSPtrVector & initCurves, const SmoothValues & params, const MbSNameMaker & names )
    : _initCurves( initCurves )
    , _nameMaker( &names.Duplicate() )
    , _params( params ) {}

public:
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker & GetNameMaker() const { return *_nameMaker; }

  /// \ru Получить набор скругляемых ребер. \en Get the edges to create chamfer on.
  const c3d::EdgesSPtrVector & GetCurves() const { return _initCurves; }

OBVIOUS_PRIVATE_COPY( MbShellChamferParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Класс параметров разделения оболочки на части по заданному контейнеру ребер.
           \en Parameter class for a shell division into parts using given edges container. \~
    \details \ru Класс параметров разделения оболочки на части по заданному контейнеру ребер. \n
             \en Parameter class for a shell division into parts using given edges container. \n \~
    \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbDivideShellParams 
{
private:
  std::vector<c3d::EdgeSPtr> _edges;     ///< \ru Контейнер рёбер. \en The edges container. \~
  MbSNameMaker               _operNames; ///< \ru Именователь. \en An object for naming the new objects. \~

  MbDivideShellParams(); ///< \ru Конструктор по-умолчанию запрещён. \en The default constructor is forbidden. \~

public:
/** \brief \ru Конструктор по параметрам операции.
           \en Constructor by the operation parameters. \~
    \details \ru Конструктор параметров разделения оболочки на части по заданному набору ребер.
             \en Constructor of parameters to divide a shell into parts using given set of edges. \~
    \param[in] edges      - \ru Набор ребер.
                            \en A set of edges. \~
    \param[in] operNames  - \ru Именователь.
                            \en An object for naming the new objects. \~
*/
  MbDivideShellParams( const RPArray<MbCurveEdge> & edges, 
                       const MbSNameMaker         & operNames );
/** \brief \ru Конструктор по параметрам операции.
           \en Constructor by the operation parameters. \~
    \details \ru Конструктор параметров разделения оболочки на части по заданному набору ребер.
             \en Constructor of parameters to divide a shell into parts using given set of edges. \~
    \param[in] edges      - \ru Набор ребер.
                            \en A set of edges. \~
    \param[in] operNames  - \ru Именователь.
                            \en An object for naming the new objects. \~
*/
  MbDivideShellParams( const c3d::EdgesSPtrVector & edges, 
                       const MbSNameMaker         & operNames );

/** \brief \ru Конструктор по параметрам операции без контейнера рёбер.
           \en Constructor by the operation parameters without an edges container. \~
    \details \ru Конструктор параметров разделения оболочки на части без контейнера рёбер.
             \en Constructor of parameters to divide a shell into parts  without an edges container. \~
    \param[in] operNames  - \ru Именователь.
                            \en An object for naming the new objects. \~
*/
  MbDivideShellParams( const MbSNameMaker  & operNames );

  /// \ru Конструктор копирования. \en Copy constructor. \~
  MbDivideShellParams( const MbDivideShellParams & other );

public:
  /// \ru Добавление рёбер в контейнер. \en Add the edges to the container. \~
  template<class EdgeContainer>
  void Add( const EdgeContainer & edges );

  /// \ru Добавление ребра в контейнер рёбер. \en Add an edge to the edges container. \~
  void Add( const c3d::EdgeSPtr & edge );
  /// \ru Добавление ребра в контейнер рёбер. \en Add an edge to the edges container. \~
  void Add( MbCurveEdge & edge );
  /// \ru Добавление ребра в контейнер рёбер. \en Add an edge to the edges container. \~
  void Add( MbCurveEdge * edge );

  /// \ru Получить контейнер рёбер. \en Get the edges container. \~
  void GetEdges( std::vector<c3d::EdgeSPtr> & edges ) const { edges.insert( edges.end(), _edges.begin(), _edges.end() ); }
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation. \~
  const MbSNameMaker               & GetNameMaker()   const { return _operNames; }
  /// \ru Оператор копирования. \en Copy operator. \~
  void operator = ( const MbDivideShellParams & other );
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры скругления ребер тела.
           \en Parameters for creating a fillet of edges of a solid. \~
  \details \ru Параметры скругления ребер тела.
               Задает параметры для следующих операций:
                - Скруглить ребра тела постоянным радиусом.
                - Скруглить ребра тела переменным радиусом.
                - Скруглить вершины и примыкающие к ней рёбра тела постоянным радиусом.
           \en Parameters for creating a fillet of edges of a solid.
               Defines parameters for following operations:
                - Fillet the specified edges of the solid with a constant radius.
                - Fillet the given edges of the solid with a variable radius.
                - Create fillets on vertices and the edges of the solid adjacent to these vertices with a constant radius. \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbShellFilletValues : public MbShellChamferParams
{
private:
  std::vector<MbEdgeFunction> _initFunctions; ///< \ru Множество скругляемых ребер тела с функциями изменения радиуса. \en An array of edges of the solid to fillet together with the radius laws.
  c3d::FacesSPtrVector        _initBounds;    ///< \ru Множество граней для обрезки торцов. \en A set of faces for trimming of the butt - ends.
  c3d::VerticesSPtrVector     _initVertices;  ///< \ru Множество скругляемых вершин. \en A set of vertices to fillet.
public:
  CornerValues                _cornerData;    ///< \ru Параметры скругления вершин. \en Parameters of vertices fillet.

public:
  /** \brief \ru Конструктор параметров для скругления ребер постоянным радиусом.
             \en Constructor of parameters for filleting edges with a constant radius. \~
    \details \ru Конструктор параметров для скругления ребер постоянным радиусом.
             \en Constructor of parameters for filleting edges with a constant radius. \~
    \param[in] initCurves - \ru Множество скругляемых ребер тела.
                            \en A set of edges of the solid to fillet. \~
    \param[in] initBounds - \ru Множество граней для обрезки торцов.
                            \en A set of faces for trimming of the butt-ends. \~
    \param[in] params     - \ru Параметры скругления рёбер.
                            \en Parameters of edges fillet. \~
    \param[in] names      - \ru Именователь.
                            \en An object for naming the new objects. \~
  */
  MbShellFilletValues( c3d::EdgesSPtrVector & initCurves, c3d::FacesSPtrVector & initBounds,
                       const SmoothValues & params, const MbSNameMaker & names )
    : MbShellChamferParams( initCurves, params, names )
    , _initBounds( initBounds ) {}

  /** \brief \ru Конструктор параметров для скругления вершин и примыкающих к ним рёбер постоянным радиусом.
             \en Constructor of parameters for filleting vertices and the edges adjacent to them with a constant radius. \~
    \details \ru Конструктор параметров для скругления вершин и примыкающих к ним рёбер постоянным радиусом.
             \en Constructor of parameters for filleting vertices and the edges adjacent to them with a constant radius. \~
    \param[in] initCurves   - \ru Множество скругляемых ребер тела.
                              \en A set of edges of the solid to fillet. \~
    \param[in] initBounds   - \ru Множество граней для обрезки торцев.
                              \en A set of faces for trimming of the butt-ends. \~
    \param[in] initVertices - \ru Множество скругляемых вершин.
                              \en A set of vertices to fillet. \~
    \param[in] cornerData   - \ru Параметры скругления вершин.
                              \en Parameters of vertices fillet. \~
    \param[in] params       - \ru Параметры скругления рёбер.
                              \en Parameters of edges fillet. \~
    \param[in] names        - \ru Именователь.
                              \en An object for naming the new objects. \~
  */
  MbShellFilletValues( c3d::EdgesSPtrVector & initCurves, c3d::FacesSPtrVector & initBounds,
                       c3d::VerticesSPtrVector & initVertices, 
                       const CornerValues & cornerData, const SmoothValues & params, const MbSNameMaker & names )
    : MbShellChamferParams( initCurves, params, names )
    , _initBounds  ( initBounds )
    , _initVertices( initVertices )
    , _cornerData  ( cornerData ) {}

  /** \brief \ru Конструктор параметров для скругления ребер переменным радиусом.
             \en Constructor of parameters for filleting edges with a variable radius. \~
    \details \ru Конструктор параметров для скругления ребер переменным радиусом, задаваемым MbEdgeFunction.function.
             \en Constructor of parameters for filleting edges with a variable radius specified by MbEdgeFunction.function. \~
    \param[in] initFunctions  - \ru Множество скругляемых ребер тела с функциями изменения радиуса.
                                \en An array of edges of the solid to fillet together with the radius laws. \~
    \param[in] initBounds     - \ru Множество граней для обрезки торцов.
                                \en A set of faces for trimming of the butt-ends. \~
    \param[in] names          - \ru Именователь.
                                \en An object for naming the new objects. \~
    \param[in] params         - \ru Параметры скругления рёбер.
                                \en Parameters of edges fillet. \~
  */
  MbShellFilletValues( const std::vector<MbEdgeFunction> & initFunctions, c3d::FacesSPtrVector & initBounds,
                       const MbSNameMaker & names, const SmoothValues & params )
    : MbShellChamferParams( c3d::EdgesSPtrVector(), params, names )
    , _initFunctions( initFunctions )
    , _initBounds   ( initBounds ) {}

public:
  /// \ru Получить набор скругляемых ребер тела с функциями изменения радиуса. \en Get the edges to fillet together with the radius laws.
  const std::vector<MbEdgeFunction> & GetEdgeFunctions() const { return _initFunctions; }

  /// \ru Получить набор скругляемых вершин. \en Get the vertices to fillet.
  const c3d::VerticesSPtrVector & GetVertices() const { return _initVertices; }

  /// \ru Получить набор граней для обрезки торцов. \en Get the faces for trimming of the butt - ends.
  const c3d::FacesSPtrVector & GetBounds() const { return _initBounds; }

private:
  /// \ru Инициализация std::vector массивом объектов Type. \en Initialization of std::vector with an array of Type objects.
  template<class TypeArray, class Type> // TypeArray - type of initial array, Type - type of objects in arrays.
  void InitTypeArray( const TypeArray & initArr, std::vector<Type> & destArr );

OBVIOUS_PRIVATE_COPY( MbShellFilletValues )
};


//------------------------------------------------------------------------------
// \ru Инициализация std::vector массивом объектов Type. \en Initialization of std::vector with an array of Type objects.
//  \param[in]  initArr - \ru Начальный массив объектов Type. \en Initial array of Type objects.
//  \param[out] destArr - \ru Результирующий std::vector объектов Type. \en Result std::vector of objects Type.
// ---
template<class TypeArray, class Type>
void MbShellFilletValues::InitTypeArray( const TypeArray & initArr, std::vector<Type> & destArr )
{
  destArr.clear();
  destArr.reserve( initArr.size() );
  destArr.insert( destArr.end(), initArr.begin(), initArr.end() );
}


//------------------------------------------------------------------------------
/** \brief \ru Параметры операции LoftedShell.
           \en LoftedShell operation parameters. \~
    \details \ru Параметры операции LoftedShell. \n
                 Содержат информацию о наборе кривых, именователе, идентификаторе. \n
             \en LoftedShell operation parameters. \n
                 Contains information about curves, object defining names generation in the operation, identifier. \n \~
    \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbLoftedCurvesShellParams: public MbPrecision
{
private:
  std::vector<c3d::SpaceCurveSPtr> _curves;  ///< \ru Набор кривых. \en A set of curves.
  MbSNameMaker                     _names;   ///< \ru Именователь. \en An object defining names generation in the operation.
  SimpleName                       _name;    ///< \ru Идентификатор. \en An identifier.

private:
  MbLoftedCurvesShellParams(); /// \ru Конструктор по умолчанию. Запрещен. \en. Default constructor. Forbidden.

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по набору кривых, именователю и идентификатору. \n
             \en Constructor by set of curves, object defining names generation in the operation and identifier. \n \~
    \param[in] curves - \ru Набор кривых.
                        \en A set of curves. \~
    \param[in] names - \ru Именователь.
                       \en An object defining names generation in the operation. \~
    \param[in] name - \ru Идентификатор.
                      \en An identifier. \~
  */
  template<class CurvesVector>
  MbLoftedCurvesShellParams( const CurvesVector & curves,
                             const MbSNameMaker & names, 
                             const SimpleName &   name );

  /// \ru Конструктор копирования. \en Copy constructor.
  MbLoftedCurvesShellParams( const MbLoftedCurvesShellParams & other );
  /// \ru Оператор присваивания. Assignment operator.
  const MbLoftedCurvesShellParams & operator = ( const MbLoftedCurvesShellParams &  other );

  /// \ru Получить набор кривых. \en Get the curves.
  void GetCurves( std::vector<c3d::ConstSpaceCurveSPtr> & curves ) const;

  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker & GetNameMaker()  const { return _names; }

  /// \ru Получить идентификатор операции. \en Get the identifier in the operation.
  const SimpleName & GetName()  const { return _name; }

private:
  /// \ru Инициализация набора кривых. \en Initialization of curves.
  template<class CurvesVector>
  void InitCurves( const CurvesVector & initCurves );
};


//------------------------------------------------------------------------------
// Конструктор.
// ---
template<class CurvesVector>
MbLoftedCurvesShellParams::MbLoftedCurvesShellParams( const CurvesVector & curves,
                                                      const MbSNameMaker & names, 
                                                      const SimpleName &   name )
  : MbPrecision()
  , _curves()
  , _names()
  , _name( name   )
{
  InitCurves( curves );
  _names.SetNameMaker( names, true );
}


//------------------------------------------------------------------------------
// Инициализация набора кривых.
// ---
template<class CurvesVector>
void MbLoftedCurvesShellParams::InitCurves( const CurvesVector & initCurves )
{
  _curves.clear();
  _curves.reserve( initCurves.size() );
  for ( size_t i = 0; i < initCurves.size(); ++i ) {
    MbCurve3D * curve = initCurves[i];
    if ( curve != nullptr )
      _curves.push_back( SPtr<MbCurve3D>( curve ) );
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Параметры операции LoftedShell.
           \en LoftedShell operation parameters. \~
    \details \ru Параметры операции LoftedShell. \n
                 Содержат информацию о наборе точек, именователе, идентификаторе. \n
             \en LoftedShell operation parameters. \n
                 Contains information about points, object defining names generation in the operation, identifier. \n \~
    \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbLoftedPointsShellParams: public MbPrecision
{
private:
  std::vector<c3d::SpacePointsVector> _points;     ///< \ru Наборы точек. \en A sets of points.
  MbSNameMaker                        _names;      ///< \ru Именователь. \en An object defining names generation in the operation.
  SimpleName                          _name;       ///< \ru Идентификатор. \en An identifier.

  MbLoftedPointsShellParams(); /// \ru Конструктор по умолчанию. Запрещен. \en. Default constructor. Forbidden.

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по наборам точек, именователю и идентификатору. \n
             \en Constructor by sets of points, object defining names generation in the operation and identifier. \n \~
    \param[in] points - \ru Набор точек.
                        \en A set of points. \~
    \param[in] names - \ru Именователь.
                       \en An object defining names generation in the operation. \~
    \param[in] name - \ru Идентификатор.
                      \en An identifier. \~
  */
  MbLoftedPointsShellParams( const RPArray<SArray<MbCartPoint3D>> & points,
                             const MbSNameMaker &                   names, 
                             const SimpleName &                     name );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по наборам точек, именователю и идентификатору. \n
             \en Constructor by sets of points, object defining names generation in the operation and identifier. \n \~
    \param[in] points - \ru Набор точек.
                        \en A set of points. \~
    \param[in] names - \ru Именователь.
                       \en An object defining names generation in the operation. \~
    \param[in] name - \ru Идентификатор.
                      \en An identifier. \~
  */
  MbLoftedPointsShellParams( const std::vector<c3d::SpacePointsVector> & points,
                             const MbSNameMaker &                        names, 
                             const SimpleName &                          name );

  /// \ru Конструктор копирования. \en Copy constructor.
  MbLoftedPointsShellParams( const MbLoftedPointsShellParams & other );
  /// \ru Оператор присваивания. Assignment operator.
  const MbLoftedPointsShellParams & operator = ( const MbLoftedPointsShellParams &  other );

  /// \ru Получить набор точек. \en Get the points.
  void GetPoints( std::vector<std::vector<MbCartPoint3D>> & points ) const { points = _points; }

  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker & GetNameMaker()  const { return _names; }

  /// \ru Получить идентификатор операции. \en Get the identifier in the operation.
  const SimpleName & GetName()  const { return _name; }

private:
  /// \ru Инициализация набора точек. \en Initialization of точек.
  template<class VectorOfPointVectors>
  void InitPoint( const VectorOfPointVectors & initPoints );
};


//------------------------------------------------------------------------------
/** \brief \ru Исходные данные для операции построения тела по сечениям.
           \en Input data for lofted solid operation.
  \details \ru Исходные данные для операции построения тела по сечениям. \n
    Возможно при этом выполнение Булевой операции над уже имеющимися телами.
    Поддерживаемые типы Булевой операции: \n
      - bo_Union      - объединение, \n
      - bo_Intersect  - пересечение, \n
      - bo_Difference - вычитание.
    Значение по умолчанию: bo_Base (никакая операция не делается).
           \en Input data for lofted solid operation. \n \~
    At that, making some Boolean operation with already existing solids is possible.
    Following types of Boolean operation are supported: \n
      - bo_Union      - union, \n
      - bo_Intersect  - intersection, \n
      - bo_Difference - subtraction. \~
    The default value: bo_Base (no boolean operation will be made). \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbLoftedSolidParams
{
protected:
  c3d::ContourOnSurfaceSPtrVector _surfaceSections; ///< \ru Множество образующих контуров. \en An array of generating contours. \~
  c3d::SpaceCurvesSPtrVector      _guideCurves; ///< \ru Множество направляющих кривых, задающих траектории соответствующих точек контуров. \en An array of the guide curves that determines the trajectories of the corresponding points of the contours. \~
  c3d::SpacePointsVector          _guidePoints;  ///< \ru Множество точек на образующих контурах, задающее соответствующие точки (цепочки точек). \en A point array on the generating contours which determines the corresponding points of the contours (chains of points). \~
  c3d::SNamesMakerSPtrVector      _sectionsNames; ///< \ru Именователи образующих контуров. \en The objects defining the names of generating contours. \~
  c3d::ConstSpaceCurveSPtr        _spine; ///< \ru Осевая кривая (может быть nullptr). \en A guide curve (can be nullptr). \~
  c3d::SNameMakerSPtr             _operName;  ///< \ru Именователь операции. \en An object defining names generation in the operation. \~
  LoftedValues                    _loftParams; ///< \ru Параметры операции. \en The operation parameters. \~
  OperationType                   _operType;  ///< \ru Тип Булевой операции. \en A boolean operation type. \~
public:
  mutable IProgressIndicator * _progress;  ///< \ru Индикатор прогресса выполнения операции. \en A progress indicator of the operation.

public:
/** \brief \ru Конструктор по параметрам операции.
           \en Constructor by the operation parameters. \~
    \details \ru Конструктор параметрам операции.
             \en Constructor by the operation parameters. \~
  \param[in] places - \ru Множество систем координат образующих контуров.
                      \en An array of generating contours coordinate systems. \~
  \param[in] planeConours - \ru Множество образующих контуров.
                            \en An array of generating contours. \~
  \param[in] spine - \ru Осевая кривая (может быть nullptr).
                     \en A guide curve (can be nullptr). \~
  \param[in] params - \ru Параметры операции.
                      \en The operation parameters. \~
  \param[in] guideCurves - \ru Множество направляющих кривых, задающих траектории соответствующих точек контуров.
                           \en An array of the guide curves that determines the trajectories of the corresponding points of the contours. \~
  \param[in] guidePoints - \ru Множество точек на образующих контурах, задающее соответствующие точки (цепочки точек).
                           \en A point array on the generating contours which determines the corresponding points of the contours (chains of points). \~
  \param[in] operName - \ru Именователь операции.
                        \en An object defining names generation in the operation. \~
  \param[in] contourNames - \ru Именователи образующих контуров.
                            \en The objects defining the names of generating contours. \~
  \param[in/out] prog     - \ru Индикатор прогресса выполнения операции.
                            \en A progress indicator of the operation. \~
*/
  MbLoftedSolidParams( const std::vector<MbPlacement3D> &   places,
                       const c3d::PlaneContoursSPtrVector & planeConours,
                       const c3d::ConstSpaceCurveSPtr &     spine,
                       const LoftedValues &                 params,
                       const c3d::SpaceCurvesSPtrVector *   guideCurves,
                       const c3d::SpacePointsVector *       guidePoints,
                       const MbSNameMaker &                 operName,
                       const c3d::SNamesMakerSPtrVector &   contourNames,
                       IProgressIndicator *                 prog = nullptr );

/** \brief \ru Конструктор по параметрам операции.
           \en Constructor by the operation parameters. \~
    \details \ru Конструктор по параметрам операции.
             \en Constructor by the operation parameters. \~
  \param[in] surfaces - \ru Множество поверхностей образующих контуров.
                        \en An array of surfaces of generating contours. \~
  \param[in] planeConours - \ru Множество образующих контуров.
                            \en An array of generating contours. \~
  \param[in] spine - \ru Осевая кривая (может быть nullptr).
                     \en A guide curve (can be nullptr). \~
  \param[in] params - \ru Параметры операции.
                      \en The operation parameters. \~
  \param[in] guideCurves - \ru Множество направляющих кривых, задающих траектории соответствующих точек контуров.
                           \en An array of the guide curves that determines the trajectories of the corresponding points of the contours. \~
  \param[in] guidePoints - \ru Множество точек на образующих контурах, задающее соответствующие точки (цепочки точек).
                           \en A point array on the generating contours which determines the corresponding points of the contours (chains of points). \~
  \param[in] operName - \ru Именователь операции.
                        \en An object defining names generation in the operation. \~
  \param[in] contourNames - \ru Именователи образующих контуров.
                            \en The objects defining the names of generating contours. \~
  \param[in/out] prog     - \ru Индикатор прогресса выполнения операции.
                            \en A progress indicator of the operation. \~
*/
  MbLoftedSolidParams( const c3d::SurfacesSPtrVector &      surfaces,
                       const c3d::PlaneContoursSPtrVector & planeConours,
                       const c3d::ConstSpaceCurveSPtr &     spine,
                       const LoftedValues &                 params,
                       const c3d::SpaceCurvesSPtrVector *   guideCurves,
                       const c3d::SpacePointsVector *       guidePoints,
                       const MbSNameMaker &                 operName,
                       const c3d::SNamesMakerSPtrVector &   contourNames,
                             IProgressIndicator *           prog  = nullptr );

/** \brief \ru Конструктор по параметрам операции.
           \en Constructor by the operation parameters. \~
    \details \ru Конструктор по параметрам операции.
             \en Constructor by the operation parameters. \~
  \param[in] places - \ru Множество систем координат образующих контуров.
                      \en An array of generating contours coordinate systems. \~
  \param[in] planeConours - \ru Множество образующих контуров.
                            \en An array of generating contours. \~
  \param[in] spine - \ru Осевая кривая (может быть nullptr).
                     \en A guide curve (can be nullptr). \~
  \param[in] params - \ru Параметры операции.
                      \en The operation parameters. \~
  \param[in] oType - \ru Тип булевой операции.
                     \en A boolean operation type. \~
  \param[in] guideCurves - \ru Множество направляющих кривых, задающих траектории соответствующих точек контуров.
                           \en An array of the guide curves that determines the trajectories of the corresponding points of the contours. \~
  \param[in] guidePoints - \ru Множество точек на образующих контурах, задающее соответствующие точки (цепочки точек).
                           \en A point array on the generating contours which determines the corresponding points of the contours (chains of points). \~
  \param[in] operName - \ru Именователь операции.
                        \en An object defining names generation in the operation. \~
  \param[in] contourNames - \ru Именователи образующих контуров.
                            \en The objects defining the names of generating contours. \~
  \param[in/out] prog     - \ru Индикатор прогресса выполнения операции.
                            \en A progress indicator of the operation. \~
*/

  MbLoftedSolidParams( const std::vector<MbPlacement3D> &   places,
                       const c3d::PlaneContoursSPtrVector & planeConours,
                       const c3d::ConstSpaceCurveSPtr &     spine,
                       const LoftedValues &                 params,
                       const OperationType                  oType,
                       const c3d::SpaceCurvesSPtrVector *   guideCurves,
                       const c3d::SpacePointsVector *       guidePoints,
                       const MbSNameMaker &                 operName,
                       const c3d::SNamesMakerSPtrVector &   contourNames,
                             IProgressIndicator *           prog = nullptr );

/** \brief \ru Конструктор по параметрам операции.
           \en Constructor by the operation parameters. \~
    \details \ru Конструктор параметрам операции.
             \en Constructor by the operation parameters. \~
  \param[in] surfaces - \ru Множество поверхностей образующих контуров.
                        \en An array of surfaces of generating contours. \~
  \param[in] planeConours - \ru Множество образующих контуров.
                            \en An array of generating contours. \~
  \param[in] spine - \ru Осевая кривая (может быть nullptr).
                     \en A guide curve (can be nullptr). \~
  \param[in] params - \ru Параметры операции.
                      \en The operation parameters. \~
  \param[in] oType  - \ru Тип булевой операции.
                      \en A boolean operation type. \~
  \param[in] guideCurves - \ru Множество направляющих кривых, задающих траектории соответствующих точек контуров.
                           \en An array of the guide curves that determines the trajectories of the corresponding points of the contours. \~
  \param[in] guidePoints - \ru Множество точек на образующих контурах, задающее соответствующие точки (цепочки точек).
                           \en A point array on the generating contours which determines the corresponding points of the contours (chains of points). \~
  \param[in] operName - \ru Именователь операции.
                        \en An object defining names generation in the operation. \~
  \param[in] contourNames - \ru Именователи образующих контуров.
                            \en The objects defining the names of generating contours. \~
  \param[in/out] prog     - \ru Индикатор прогресса выполнения операции.
                            \en A progress indicator of the operation. \~
*/
  MbLoftedSolidParams( const c3d::SurfacesSPtrVector &      surfaces,
                       const c3d::PlaneContoursSPtrVector & planeConours,
                       const c3d::ConstSpaceCurveSPtr &     spine,
                       const LoftedValues &                 params,
                       const OperationType                  oType,
                       const c3d::SpaceCurvesSPtrVector *   guideCurves,
                       const c3d::SpacePointsVector *       guidePoints,
                       const MbSNameMaker &                 operName,
                       const c3d::SNamesMakerSPtrVector &   contourNames,
                             IProgressIndicator *           prog  = nullptr );

  /// \ru Деструктор. \en Destructor. \~
  ~MbLoftedSolidParams();

  /// \ru Получить осевую кривую. Если не задана, возвращает nullptr. \en Get the guide curve. If it is not set then nullptr will be returned. \~
  const c3d::ConstSpaceCurveSPtr & GetSpine() const;

  /// \ru Получить именователь образующего контура с заданным индексом. \en Get the object defining the name of the generating contours with given index. \~
  const MbNameMaker * GetContourName( const size_t idx ) const;

  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation. \~
  const MbSNameMaker & GetOperationName() const;

  /// \ru Получить параметры операции. \en Get the operation parameters. \~
  const LoftedValues & GetLoftParameters() const;

  /// \ru Количество сечений. \en Get number of the sections. \~
  size_t GetSectionsCount() const;

  /// \ru Получить поверхность образующего контура с данным индексом. \en Get the surface of generating contour with given index. \~
  const MbSurface * GetSectionSurface( const size_t idx ) const;

  /// \ru Получить образующий контур с данным индексом. \en Get the generating contour with given index. \~
  const MbContour * GetSectionCurve( const size_t idx ) const;

  /// \ru Получить массивы элементов сечений (планарный контур и поверхность, на которой он построен). \en Get the arrays of sections' elements (planar contour and surfaces, which it is created on). \~
  void GetSections( c3d::SurfacesSPtrVector & surfaces, c3d::PlaneContoursSPtrVector & contours ) const;

  /// \ru Получить массивы направляющих (кривых и цепочек точек). Можно выборочно, занулив соответствующий указатель. \en Get the arrays of guide items (curves and chain points). They can be requested separately if correspond pointer is nullptr. \~
  void GetGuides( c3d::SpaceCurvesSPtrVector * guideCurves, c3d::SpacePointsVector * guidePoints ) const;

  /// \ru Получить именователи контуров. NamesPointers - вектор указателей (можно SPtr) на MbSNameMaker. \en Get the objects defining the names of generating contours. NamesPointers is a vector of pointers (SPtr is available, too) to MbSNameMaker. \~
  template<class NamesPointers>
  void GetContoursNames( NamesPointers & contourNames ) const;

  /// \ru Тип Булевой операции. \en A boolean operation type. \~
  OperationType GetBooleanOperationType() const;

private:
  /// \ru Задать именователи контуров. NamesPointers - вектор указателей (можно SPtr) на MbSNameMaker. \en Set the objects defining the names of generating contours. NamesPointers is a vector of pointers (SPtr is available, too) to MbSNameMaker. \~
  template<class NamesPointers>
  bool AddContoursNames( const NamesPointers & contourNames );

  /// \ru Добавить сечения из заданного массива. SurfacePointers - вектор указателей (можно SPtr) на MbSurface. ContourPointers - вектор указателей (можно SPtr) на MbContour. \en Add the sections from given array. SurfacePointers is a vector of pointers (SPtr is available, too) to MbSurface. ContourPointers is a vector of pointers (SPtr is available, too) to MbContour.\~
  template<class SurfacePointers, class ContourPointers>
  bool AddSectionsSurf( const SurfacePointers & surfaces, const ContourPointers & curves );

  /// \ru Добавить сечения из заданного массива. ArrPlaces - вектор MbPlacement3D. ArrContoursPointer - вектор указателей (можно SPtr) на MbContour. \en Add the sections from given array. ArrPlaces is a vector of MbPlacement3D. ArrContoursPointer is a vector of pointers (SPtr is available, too) to MbContour. \~
  template<class Places, class ContourPointers>
  bool AddSectionsPlaces( const Places & places, const ContourPointers & curves );

OBVIOUS_PRIVATE_COPY( MbLoftedSolidParams )
};


//------------------------------------------------------------------------------
//  \ru Задать именователи контуров. NamesPointers - вектор указателей (можно SPtr) на MbSNameMaker.
//  \en Set the objects defining the names of generating contours. NamesPointers is a vector of pointers (SPtr is available, too) to MbSNameMaker. \~
// ---
template<class NamesPointers>
bool MbLoftedSolidParams::AddContoursNames( const NamesPointers & contourNames )
{
  bool retVal( false );

  for ( size_t i = 0, cntElements = contourNames.size(); i < cntElements; ++i ) {
    _sectionsNames.push_back( c3d::SNameMakerSPtr(&(contourNames[i]->Duplicate())) );
    retVal = true;
  }

  return retVal;
}


//------------------------------------------------------------------------------
//  \ru Добавить сечения из заданного массива. SurfacePointers - вектор указателей (можно SPtr) на MbSurface. ContourPointers - вектор указателей (можно SPtr) на MbContour.
//  \en Add the sections from given array. SurfacePointers is a vector of pointers (SPtr is available, too) to MbSurface. ContourPointers is a vector of pointers (SPtr is available, too) to MbContour. \~
// ---
template<class SurfacePointers, class ContourPointers>
bool MbLoftedSolidParams::AddSectionsSurf( const SurfacePointers & surfaces,
                                           const ContourPointers & curves )
{
  bool retVal( false );
  const size_t cntElements = surfaces.size();

  if ( curves.size() == cntElements ) {
    _surfaceSections.reserve( _surfaceSections.size() + cntElements );
    for ( size_t i = 0; i < cntElements; ++i ) {
      c3d::ContourOnSurfaceSPtr cont( new MbContourOnSurface(*surfaces[i], *curves[i], true) );
      _surfaceSections.push_back( cont );
      retVal = true;
    }
  }

  return retVal;
}


//------------------------------------------------------------------------------
//  \ru Добавить сечения из заданного массива. Places - вектор MbPlacement3D. ContourPointers - вектор указателей (можно SPtr) на MbContour.
//  \en Add the sections from given array. Places is a vector of MbPlacement3D. ContourPointers is a vector of pointers (SPtr is available, too) to MbContour. \~
// ---
template<class Places, class ContourPointers>
bool MbLoftedSolidParams::AddSectionsPlaces( const Places &          places,
                                             const ContourPointers & curves )
{
  bool retVal( false );
  const size_t cntElements = places.size();
  if ( curves.size() == cntElements ) {
    _surfaceSections.reserve( _surfaceSections.size() + cntElements );

    for ( size_t i = 0; i < cntElements; ++i ) {
      if ( curves[i] != nullptr ) {
        MbSurface * surf( new MbPlane( places[i] ) );
        c3d::ContourOnSurfaceSPtr cont( new MbContourOnSurface(*surf, *curves[i], true) );
        _surfaceSections.push_back( cont );
        retVal = true;
      }
    }
  }

  return retVal;
}


//------------------------------------------------------------------------------
//  \ru Получить именователи контуров. NamesPointers - вектор указателей (можно SPtr) на MbSNameMaker.
//  \en Get the objects defining the names of generating contours. NamesPointers is a vector of pointers (SPtr is available, too) to MbSNameMaker. \~
// ---
template<class NamesPointers>
void MbLoftedSolidParams::GetContoursNames( NamesPointers & sectionsNames ) const
{
  const size_t iCnt = _sectionsNames.size();
  sectionsNames.reserve( sectionsNames.size() + iCnt );
  for ( size_t i = 0; i < iCnt; ++i ) {
    sectionsNames.push_back( _sectionsNames[i] );
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Данные, необходимые для построения элемента по траектории с динамической параметризацией сечения.
           \en Data required for sweeping operation with variable section. \~
  \details \ru Данные, необходимые для построения элемента по траектории с динамической параметризацией сечения.
           \en Data required for sweeping operation with variable section. \~
  \ingroup Shell_Building_Parameters
  \warning \ru В разработке. \en Under development.
*/
// ---
struct MATH_CLASS VariableSectionValues
{
private:
  const IVariableSectionData & _sectionData; ///< \ru Интерфейс для получения сечений в кинематической операции с динамической параметризацией сечения. \en An interface for obtaining sections for sweeping operation with variable section. \~
  std::set<const MbFunction *> _constrFuncs; ///< \ru Функции ограничений, наложенных на образующие контуры. Область определения функций должна быть [0, 1]. Значение параметра интерпретируется как часть метрической длины части траектории, соответствующей области определения элемента. \en Functions of constraints imposed on generating contours. The domain of functions must be [0, 1]. The parameter value is interpreted as part of the metric length of the spine's part corresponding to the domain of sweeping solid with variable section. \~
  std::map<size_t, std::set<const MbSpaceItem *>> _constrObjs; ///< \ru Ассоциативный контейнер: ключ - индекс образующего контура, значение - множество пространственных объектов, используемых в ограничениях, наложенных на соответствующий образующий контур. \en Associative container: key - index of generating contour, value - set of space items used in constraints imposed on the corresponding generating contour. \~
  c3d::DoubleVector _spineTValsForReqSections; ///< \ru Значения параметра направляющей кривой, в которых обязательно должны быть построены сечения. \en Spine curve's parameter values in which sections must be constructed. \~
  bool _splitAtG1Corners; ///< \ru Разбивать элемент на грани вдоль образующей кривой и вдоль траектории в местах стыковки сегментов образующей кривой, траектории и внешних объектов ограничений (кривых), которые стыкуются с гладкостью G1 и выше. \en Split into faces along generating curve and along spine at points where segments of generating curve, spine or curves used in constraints join smoothly (G1 and up). \~

public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by the parameters. \~
      \details \ru Конструктор по параметрам.
               \en Constructor by the parameters. \~
    \param[in] sectionData - \ru Интерфейс для получения сечений в кинематической операции с динамической параметризацией сечения.
                             \en An interface for obtaining sections for sweeping operation with variable section. \~
  */
  VariableSectionValues( const IVariableSectionData & sectionData );

  /** \brief \ru Добавить функцию ограничения, наложенного на образующие контуры.
             \en Add function of constraint imposed on generating contours. \~
      \details \ru Добавить функцию ограничения, наложенного на образующие контуры.
                   Область определения функции должна быть [0, 1]. Значение параметра интерпретируется
                   как часть метрической длины части траектории, соответствующей области определения элемента.
               \en Add function of constraint imposed on generating contours.
                   The domain of a function must be [0, 1]. The parameter value is interpreted as part of the metric
                   length of the spine's part corresponding to the domain of sweeping solid with variable section. \~
    \param[in] func - \ru Функция ограничения, наложенного на образующие контуры.
                      \en Function of constraint imposed on generating contours. \~
    \return \ru true, если функция была добавлена.
            \en true if function was added. \~
  */
  bool AddConstraintFunction( const MbFunction * func );

  /** \brief \ru Добавить пространственный объект, используемый в ограничении.
             \en Add space item used in constraint. \~
      \details \ru Добавить пространственный объект, используемый в ограничении, наложенном на образующий контур с заданным индексом.
               \en Add space item used in constraint imposed generating contour with specified index. \~
    \param[in] cntrIdx - \ru Индекс образующего контура, на который действует ограничение.
                         \en Index of generating contour subjected to the constraint. \~
    \param[in] obj     - \ru Пространственный объект, используемый в ограничении.
                         \en Space item used in constraint. \~
    \return \ru true, если объект был добавлен.
            \en true if object was added. \~
  */
  bool AddConstraintObject( const size_t cntrIdx, const MbSpaceItem * obj );

  /// \ru Добавить значение параметра направляющей кривой, при котором обязательно должно быть построено сечение. \en Add spine curve's parameter value at which section must be constructed. \~
  void AddSpineTValForReqSection( const double t );

  /// \ru Установить флаг разбиения элемента на грани вдоль образующей кривой и вдоль траектории в местах стыковки сегментов образующей кривой, траектории и внешних объектов ограничений (кривых), которые стыкуются с гладкостью G1 и выше. \en Set the flag of splitting into faces along generating curve and along spine at points where segments of generating curve, spine or curves used in constraints join smoothly (G1 and up). \~
  void SetSplittingAtSmoothCorners( const bool split );

  /// \ru Получить интерфейс для получения сечений в кинематической операции с динамической параметризацией сечения. \en Get an interface for obtaining sections for sweeping operation with variable section. \~
  const IVariableSectionData & GetSectionData() const;

  /// \ru Получить функции ограничений, наложенных на образующие контуры. \en Get functions of constraints imposed on generating contours. \~
  const std::set<const MbFunction *> & GetFuncs() const;

  /// \ru Получить пространственные объекты, используемые в ограничениях, наложенных на образующие контуры. \en Get space items used in constraints imposed on generating contours. \~
  const std::map<size_t, std::set<const MbSpaceItem *>> & GetConstrObjects() const;

  /// \ru Получить значения параметра направляющей кривой, в которых обязательно должны быть построены сечения. \en Get spine curve's parameter values at which sections must be constructed. \~
  const c3d::DoubleVector & GetSpineTValsForReqSections() const;

  /// \ru Получить флаг разбиения элемента на грани вдоль образующей кривой и вдоль траектории в местах стыковки сегментов образующей кривой, траектории и внешних объектов ограничений (кривых), которые стыкуются с гладкостью G1 и выше. \en Set the flag of splitting into faces along generating curve and along spine at points where segments of generating curve, spine or curves used in constraints join smoothly (G1 and up). \~
  bool GetSplittingAtSmoothCorners() const;

  OBVIOUS_PRIVATE_COPY( VariableSectionValues )
};


//------------------------------------------------------------------------------
/** \brief \ru Исходные данные для операции построения кинематического тела.
           \en Input data for sweeping solid operation.
  \details \ru Исходные данные для операции построения кинематического тела. \n
           \en Input data for sweeping solid operation. \n \~
  \ingroup Shell_Building_Parameters
  \warning \ru В разработке. \en Under development.
*/
// ---
class MATH_CLASS MbEvolutionShellParams : public MbPrecision
{
protected:
  const MbSweptData &               _sweptData;     ///< \ru Данные об образующей. \en The generating curve data. \~
  const MbCurve3D *                 _spineCurve;    ///< \ru Направляющая кривая. \en The spine curve. \~
  const MbWireFrame *               _spineFrame;    ///< \ru Направляющая кривая в виде проволочного каркаса. \en Spine curve as a wire frame. \~
  const MbSpine *                   _spine;         ///< \ru Направляющая кривая c дополнительной информацией. \en The spine curve with additional data. \~  
  const MbSNameMaker &              _operName;      ///< \ru Именователь операции. \en An object defining names generation in the operation. \~
  const MbSNameMaker &              _spineName;     ///< \ru Именователь направляющей. \en An object defining the name of a spine curve. \~
  std::vector<const MbSNameMaker *> _contoursNames; ///< \ru Именователь контуров образующей. \en An object defining the names of generating curve contours. \~
  const EvolutionValues &           _evolParams;    ///< \ru Параметры операции. \en The operation parameters. \~
  const VariableSectionValues *     _varSectVals;   ///< \ru Данные, необходимые для построения элемента по траектории с динамической параметризацией сечения. \en Data required for sweeping operation with variable section. \~
  MbRect1D                          _varSectDomain; ///< \ru Рассчитанная область значений параметра направляющей кривой, в которой строится кинематическое тело при использовании динамической параметризации сечения. \en The calculated range of spine curve's parameter where creation of sweeping solid with variable section takes place. \~
  MbResultType                      _vsDomCalcRes;  ///< \ru Код результата расчета области значений параметра направляющей кривой, в которой строится кинематическое тело при использовании динамической параметризации сечения. \en The result code of calculating the range of spine curve's parameter where creation of sweeping solid with variable section takes place. \~

public:
  mutable IProgressIndicator * _progress; ///< \ru Индикатор прогресса выполнения операции. \en An operation progress indicator.

public:
  /** \brief \ru Конструктор по параметрам операции.
             \en Constructor by the operation parameters. \~
      \details \ru Конструктор параметрам операции.
               \en Constructor by the operation parameters. \~
    \param[in] sweptData    - \ru Данные об образующей.
                              \en The generating curve data. \~
    \param[in] spineCurve   - \ru Направляющая кривая.
                              \en The spine curve. \~
    \param[in] evolParams   - \ru Параметры операции.
                              \en The operation parameters. \~
    \param[in] operName     - \ru Именователь операции.
                              \en An object defining names generation in the operation. \~
    \param[in] contourNames - \ru Именователь контуров образующей.
                              \en An object defining the names of generating curve contours. \~
    \param[in] spineName    - \ru Именователь направляющей.
                              \en An object defining the name of a guide curve. \~
    \param[in/out] prog     - \ru Индикатор прогресса выполнения операции.
                              \en An operation progress indicator. \~
  */
  MbEvolutionShellParams( const MbSweptData &           sweptData,
                          const MbCurve3D &             spineCurve,
                          const EvolutionValues &       evolParams,
                          const MbSNameMaker &          operName,
                          const RPArray<MbSNameMaker> & contoursNames,
                          const MbSNameMaker &          spineName,
                                IProgressIndicator *    prog = nullptr );

  /** \brief \ru Конструктор по параметрам операции.
             \en Constructor by the operation parameters. \~
      \details \ru Конструктор параметрам операции.
               \en Constructor by the operation parameters. \~
    \param[in] sweptData    - \ru Данные об образующей.
                              \en The generating curve data. \~
    \param[in] spineFrame   - \ru Направляющая кривая в виде проволочного каркаса.
                              \en Spine curve as a wire frame. \~
    \param[in] evolParams   - \ru Параметры операции.
                              \en The operation parameters. \~
    \param[in] operName     - \ru Именователь операции.
                              \en An object defining names generation in the operation. \~
    \param[in] contourNames - \ru Именователь контуров образующей.
                              \en An object defining the names of generating curve contours. \~
    \param[in] spineName    - \ru Именователь направляющей.
                              \en An object defining the name of a guide curve. \~
    \param[in/out] prog     - \ru Индикатор прогресса выполнения операции.
                              \en An operation progress indicator. \~
  */
  MbEvolutionShellParams( const MbSweptData &           sweptData,
                          const MbWireFrame &           spineFrame,
                          const EvolutionValues &       evolParams,
                          const MbSNameMaker &          operName,
                          const RPArray<MbSNameMaker> & contoursNames,
                          const MbSNameMaker &          spineName,
                                IProgressIndicator *    prog = nullptr );

  /** \brief \ru Конструктор по параметрам операции.
             \en Constructor by the operation parameters. \~
      \details \ru Конструктор параметрам операции.
               \en Constructor by the operation parameters. \~
    \param[in] sweptData    - \ru Данные об образующей.
                              \en The generating curve data. \~
    \param[in] spine        - \ru Направляющая кривая c дополнительной информацией.
                              \en The spine curve with additional data. \~
    \param[in] evolParams   - \ru Параметры операции.
                              \en The operation parameters. \~
    \param[in] operName     - \ru Именователь операции.
                              \en An object defining names generation in the operation. \~
    \param[in] contourNames - \ru Именователь контуров образующей.
                              \en An object defining the names of generating curve contours. \~
    \param[in] spineName    - \ru Именователь направляющей.
                              \en An object defining the name of a guide curve. \~
    \param[in/out] prog     - \ru Индикатор прогресса выполнения операции.
                              \en An operation progress indicator. \~
  */
  MbEvolutionShellParams( const MbSweptData &           sweptData,
                          const MbSpine &               spine,
                          const EvolutionValues &       evolParams,
                          const MbSNameMaker &          operName,
                          const RPArray<MbSNameMaker> & contoursNames,
                          const MbSNameMaker &          spineName,
                                IProgressIndicator *    prog = nullptr );

  /** \brief \ru Конструктор по параметрам операции.
             \en Constructor by the operation parameters. \~
      \details \ru Конструктор по параметрам операции.
               \en Constructor by the operation parameters. \~
    \param[in] sweptData   -   \ru Данные об образующей.
                               \en The generating curve data. \~
    \param[in] spine       -   \ru Направляющая кривая c дополнительной информацией.
                               \en The spine curve with additional data. \~
    \param[in] evolParams  -   \ru Параметры операции.
                               \en The operation parameters. \~
    \param[in] operName    -   \ru Именователь операции.
                               \en An object defining names generation in the operation. \~
    \param[in] contoursNames - \ru Именователь контуров образующей.
                               \en An object defining the names of generating curve contours. \~
    \param[in] spineName   -   \ru Именователь направляющей.
                               \en An object defining the name of a guide curve. \~
    \param[in] varSectVals -   \ru Данные, необходимые для построения элемента по траектории с динамической параметризацией сечения.
                               \en Data required for sweeping operation with variable section. \~
    \param[in/out] prog     - \ru Индикатор прогресса выполнения операции.
                              \en An operation progress indicator. \~
  */
  MbEvolutionShellParams( const MbSweptData &           sweptData,
                          const MbSpine &               spine,
                          const EvolutionValues &       evolParams,
                          const MbSNameMaker &          operName,
                          const RPArray<MbSNameMaker> & contoursNames,
                          const MbSNameMaker &          spineName,
                          const VariableSectionValues & varSectVals,
                                IProgressIndicator *    prog = nullptr );

  /** \brief \ru Конструктор копирования с заменой образующей
             \en Copy constructor with generator replacement. \~
      \details \ru Конструктор копирования с заменой образующей
               \en Copy constructor with generator replacement. \~
    \param[in] other - \ru Параметры для копирования.
                       \en Options for copying.
    \param[in] sweptData    - \ru Данные об образующей для замены.
                              \en The generating curve data for replacement. \~
    \param[in] contourNames - \ru Именователь контуров образующей для замены.
                              \en An object defining the names of generating curve contours for replacement. \~
  */
  MbEvolutionShellParams( const MbEvolutionShellParams & other,
                          const MbSweptData &            sweptData,
                          const RPArray<MbSNameMaker> &  contoursNames );

  /// \ru Деструктор. \en Destructor. \~
  ~MbEvolutionShellParams();

  /// \ru Получить данные об образующей. \en Get the generating curve data. \~
  const MbSweptData & GetSweptData() const;

  /// \ru Получить направляющую кривую. \en Get the spine curve. \~
  const MbCurve3D * GetSpineCurve() const;

  /// \ru Получить направляющую кривую с дополнительной информацией. \en Get the spine curve with additional information. \~
  const MbSpine * GetSpine() const;

  /// \ru Получить направляющую кривую в виде проволочного каркаса. \en Get the spine curve as a wire frame. \~
  const MbWireFrame * GetSpineFrame() const;

  /// \ru Получить параметры операции. \en Get the operation parameters. \~
  const EvolutionValues & GetEvolutionParameters() const;

  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation. \~
  const MbSNameMaker & GetNameMaker() const;

  /// \ru Получить именователи контуров. NamesPointers - вектор указателей (можно SPtr) на MbSNameMaker. \en Get the objects defining the names of generating contours. NamesPointers is a vector of pointers (SPtr is available, too) to MbSNameMaker. \~
  template<class NamesPointers>
  void GetContoursNames( NamesPointers & contoursNames ) const;

  /// \ru Получить именователь направляющей. \en Get the object defining the name of a spine. \~
  const MbSNameMaker & GetSpineName() const;

  /// \ru Получить данные, необходимые для построения элемента по траектории с динамической параметризацией сечения. \en Get data required for sweeping operation with variable section. \~
  const VariableSectionValues * GetVariableSectionValues() const;

  /// \ru Получить рассчитанную область значений параметра направляющей кривой, в которой строится кинематическое тело при использовании динамической параметризации сечения. \en Get the calculated range of spine curve's parameter where creation of sweeping solid with variable section takes place. \~
  MbResultType GetVariableSectionDomain( MbRect1D & dom ) const;

private:
  /// \ru Задать именователи контуров. NamesPointers - вектор указателей (можно SPtr) на MbSNameMaker. \en Set the objects defining the names of generating contours. NamesPointers is a vector of pointers (SPtr is available, too) to MbSNameMaker. \~
  template<class NamesPointers>
  void SetContoursNames( const NamesPointers & contoursNames );

  /// \ru Рассчитать область значений параметра направляющей кривой, в которой строится кинематическое тело при использовании динамической параметризации сечения. \en Calculate range of spine curve's parameter where creation of sweeping solid with variable section will take place. \~
  void CalcVariableSectionDomain();

OBVIOUS_PRIVATE_COPY( MbEvolutionShellParams )
};


//------------------------------------------------------------------------------
// \ru Получить именователи контуров. NamesPointers - вектор указателей (можно SPtr) на MbSNameMaker.
// \en Get the objects defining the names of generating contours. NamesPointers is a vector of pointers (SPtr is available, too) to MbSNameMaker. \~
// ---
template<class NamesPointers>
void MbEvolutionShellParams::GetContoursNames( NamesPointers & contoursNames ) const
{
  contoursNames.reserve( contoursNames.size() + _contoursNames.size() );
  for ( auto cntrNamePtr : _contoursNames )
    contoursNames.push_back( cntrNamePtr );
}


//------------------------------------------------------------------------------
// \ru Задать именователи контуров. NamesPointers - вектор указателей (можно SPtr) на MbSNameMaker.
// \en Set the objects defining the names of generating contours. NamesPointers is a vector of pointers (SPtr is available, too) to MbSNameMaker. \~
// ---
template<class NamesPointers>
inline void MbEvolutionShellParams::SetContoursNames( const NamesPointers & contoursNames )
{
  _contoursNames.clear();
  _contoursNames.reserve( contoursNames.size() );
  for ( const auto & cntrNamePtr : contoursNames ) {
    if ( cntrNamePtr != nullptr )
      _contoursNames.push_back( &(*cntrNamePtr) );
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Класс параметров продолжения незамкнутого тела выдавливанием.
           \en Parameter class an extension of an open solid by extrusion. \~
  \details \ru Класс параметров продолжения незамкнутого тела выдавливанием. \n
           \en Parameter class an extension of an open solid by extrusion. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbExtensionShellParams 
{
private:
  c3d::EdgesSPtrVector  _edges;     ///< \ru Контейнер рёбер. \en The edges container. \~
  MbSNameMaker          _operNames; ///< \ru Именователь. \en An object for naming the new objects. \~
  ExtensionValues       _params;    ///< \ru Параметры операции. \en The operation parameters. \~

public:
  /** \brief \ru Конструктор по параметрам операции.
             \en Constructor by the operation parameters. \~
  \details \ru Конструктор параметров продолжения незамкнутого тела выдавливанием по заданному набору ребер.
           \en Constructor of parameters extension of an open solid by extrusion using given set of edges. \~
  \param[in] edges     - \ru Множество краевых ребер, через которые выполняется продление.
                         \en An array of boundary edges through which to extend the face. \~
  \param[in] params    - \ru Параметры операции.
                         \en The operation parameters. \~
  \param[in] operNames - \ru Именователь.
                         \en An object for naming the new objects. \~
  */
  MbExtensionShellParams( const c3d::EdgesSPtrVector & edges, 
                          const ExtensionValues &      params, 
                          const MbSNameMaker &         operNames );
  /** \brief \ru Конструктор по параметрам операции.
             \en Constructor by the operation parameters. \~
  \details \ru Конструктор параметров продолжения незамкнутого тела выдавливанием без контейнера рёбер.
           \en Constructor of parameters extension of an open solid by extrusion without an edges container. \~
  \param[in] params    - \ru Параметры операции.
                         \en The operation parameters. \~
  \param[in] operNames - \ru Именователь.
                         \en An object for naming the new objects. \~
  */
  MbExtensionShellParams( const ExtensionValues & params, 
                          const MbSNameMaker &    operNames );

  /// \ru Конструктор "копирования" (не копирует ребра!!!). \en "Copy" constructor (does not copy edges!!!). \~
  MbExtensionShellParams( const MbExtensionShellParams & other );

public:
  /// \ru Добавление рёбер в контейнер. \en Add the edges to the container. \~
  template<class EdgeVector>
  void Add( const EdgeVector & edges )
  {
    size_t addCnt = edges.size();
    _edges.reserve( _edges.size() + addCnt );
    for ( size_t i = 0; i < addCnt; ++i )
      Add( edges[i] );
  }
  /// \ru Добавление ребра в контейнер рёбер. \en Add an edge to the edges container. \~
  void Add( MbCurveEdge * edge );
  /// \ru Добавление ребра в контейнер рёбер. \en Add an edge to the edges container. \~
  void Add( const c3d::EdgeSPtr & edge );
  /// \ru Получить контейнер рёбер. \en Get the edges container. \~
  void GetEdges( c3d::EdgesSPtrVector & edges ) const { edges.insert(edges.end(), _edges.begin(), _edges.end()); }
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation. \~
  const MbSNameMaker & GetNameMaker() const { return _operNames; }
  /// \ru Получить параметры операции. \en Get the operation parameters. \~
  const ExtensionValues & GetParams() const { return _params; }
  /// \ru Оператор "копирования" (не копирует ребра!!!). \en "Copy" operator (does not copy edges!!!). \~
  void operator = ( const MbExtensionShellParams & other );
};


//------------------------------------------------------------------------------
/** \brief \ru Класс параметров для построения линейчатого незамкнутого тела.
           \en Parameter class for the creation an open ruled solid. \~
  \details \ru Класс параметров для построения фантома габаритного куба в локальной системе координат. \n
           \en Parameter class for the creation of a bounding box phantom in a local coordinate system. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbRuledShellParams : public MbPrecision
{
private:
  RuledSurfaceValues   _pars;      ///< \ru Параметры операции.\en The operation parameters. \~
  MbSNameMaker         _operNames; ///< \ru Именователь.\en An object for naming the new objects. \~
  bool                 _isPhantom; ///< \ru Режим создания фантома.\en Create in the phantom mode. \~

public:
   /** \brief \ru Конструктор по параметрам операции.
             \en Constructor by the operation parameters. \~
  \details \ru Конструктор параметров продолжения незамкнутого тела выдавливанием по заданному набору ребер.
           \en Constructor of parameters extension of an open solid by extrusion using given set of edges. \~
  \param[in] pars      - \ru Параметры операции.
                         \en The operation parameters. \~
  \param[in] operNames - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[in] isPhantom - \ru Режим создания фантома.
                         \en Create in the phantom mode. \~
  */
  MbRuledShellParams ( const RuledSurfaceValues & pars,
                       const MbSNameMaker  &      operNames,
                             bool                 isPhantom );
  /// \ru Конструктор копирования. \en Copy constructor. \~
  MbRuledShellParams ( const MbRuledShellParams & other );
  /// \ru Возвращает генератор имен (именователь) топологических объектов. \en Get a generator of names (name-maker) of topological objects. \~
  const MbSNameMaker  & GetNameMaker() const { return  _operNames; }
  /// \ru Режим создания фантома.\en Create in the phantom mode. \~
  bool IsPhantom () const { return  _isPhantom; }
  /// \ru Установить генератор имен (именователь) топологических объектов. \en Set a generator of names (name-maker) of topological objects. \~
  void SetNameMaker ( const MbSNameMaker  & names, bool version = false ) { _operNames.SetNameMaker( names, version ); }
  /// \ru Установить режим создания фантома.\en Set the phantom mode. \~
  void SetPhantom  ( bool isPhantom ) { _isPhantom = isPhantom; }
  /// \ru Получить параметры операции. \en Get the operation parameters. \~
  const RuledSurfaceValues & GetParams() const { return _pars; }
  /// \ru Изменить параметры операции. \en Change the operation parameters. \~
  RuledSurfaceValues & SetParams() { return _pars; }

  /// \ru Оператор копирования. \en Copy operator. \~
  void operator = ( const MbRuledShellParams  & other );
};


//------------------------------------------------------------------------------
/** \brief \ru Выходные данные для построения линейчатого незамкнутого тела.
           \en Output data for the creation an open ruled solid. \~
  \details \ru Класс содержит параметры операции и тело. \n
           \en The output data contains operation parameters and solid. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbRuledShellResult 
{
private:
  RuledSurfaceValues   _outPars;  ///< \ru Параметры операции.\en The operation parameters. \~
  c3d::SolidSPtr       _outSolid; ///< \ru Тело.\en Solid. \~ 
public:
  /// \ru Конструктор по умолчанию. \en Default constructor. \~
  MbRuledShellResult  ();
  /// \ru Конструктор копирования. \en Copy constructor. \~
  MbRuledShellResult  ( const MbRuledShellResult  & other );
  /// \ru Получить параметры операции. \en Get the operation parameters. \~
  const RuledSurfaceValues &  GetParams() const { return _outPars; }
  /// \ru Получить выходное тело. \en Get output shell. \~
  c3d::SolidSPtr & SetSolid() { return _outSolid; }
  /// \ru Установить параметры операции. \en Set the operation parameters. \~
  void SetParams( const RuledSurfaceValues & outPars ) { _outPars.Init(outPars, false); }
  /// \ru Задать выходное тело. \en Set output solid. \~
  void SetSolid ( const c3d::SolidSPtr & outSolid ) { _outSolid = outSolid; }
  
  /// \ru Оператор копирования. \en Copy operator. \~
  void operator = ( const MbRuledShellResult   & other );
};


//------------------------------------------------------------------------------
/** \brief \ru Класс параметров для создания тела-слитка из заданного набора тел.
           \en Parameter class for an ingot solid creation from the set of specified objects. \~
    \details \ru Класс параметров для создания тела-слитка из заданного набора тел. \n
             \en Parameter class to create an ingot solid from the set of specified objects. \n \~
    \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbIngotSolidParams : public MbPrecision
{
private:
  c3d::SolidsSPtrVector _solids;    ///< \ru Контейнер тел. \en The bodies container. \~
  MbSNameMaker          _operNames; ///< \ru Именователь. \en An object for naming the new objects. \~
public:
  mutable IProgressIndicator * _progress;  ///< \ru Индикатор прогресса выполнения операции. \en A progress indicator of the operation.

public:
/** \brief \ru Конструктор по параметрам операции.
           \en Constructor by the operation parameters. \~
    \details \ru Конструктор параметров создания тела-слитка из заданного набора тел.
             \en Constructor of parameters to create an ingot solid from the set of specified objects. \~
    \param[in] solid     - \ru Тело.
                           \en A solid. \~
    \param[in] operNames - \ru Именователь.
                           \en An object for naming the new objects. \~
    \param[in/out] prog  - \ru Индикатор прогресса выполнения операции.
                           \en An operation progress indicator. \~
*/
  MbIngotSolidParams( const c3d::SolidSPtr &     solid, 
                      const MbSNameMaker &       operNames,
                            IProgressIndicator * prog = nullptr );

/** \brief \ru Конструктор по параметрам операции.
           \en Constructor by the operation parameters. \~
    \details \ru Конструктор параметров создания тела-слитка из заданного набора тел.
             \en Constructor of parameters to create an ingot solid from the set of specified objects. \~
    \param[in] item      - \ru Тело, может быть #MbAssembly, #MbInstance или #MbSolid.
                           \en A solid, may be #MbAssembly, #MbInstance or #MbSolid. \~
    \param[in] operNames - \ru Именователь.
                           \en An object for naming the new objects. \~
    \param[in/out] prog  - \ru Индикатор прогресса выполнения операции.
                           \en An operation progress indicator. \~
*/
  MbIngotSolidParams( const c3d::ItemSPtr &      item, 
                      const MbSNameMaker &       operNames,
                            IProgressIndicator * prog = nullptr );

/** \brief \ru Конструктор по параметрам операции.
           \en Constructor by the operation parameters. \~
    \details \ru Конструктор параметров создания тела-слитка из заданного набора тел.
             \en Constructor of parameters to create an ingot solid from the set of specified objects. \~
    \param[in] solids    - \ru Контейнер тел.
                           \en The bodies container. \~
    \param[in] operNames - \ru Именователь.
                           \en An object for naming the new objects. \~
    \param[in/out] prog  - \ru Индикатор прогресса выполнения операции.
                           \en An operation progress indicator. \~
*/
  MbIngotSolidParams( const c3d::SolidsSPtrVector & solids, 
                      const MbSNameMaker &          operNames,
                            IProgressIndicator *    prog = nullptr );

/** \brief \ru Конструктор по параметрам операции.
           \en Constructor by the operation parameters. \~
    \details \ru Конструктор параметров создания тела-слитка из заданного набора тел.
             \en Constructor of parameters to create an ingot solid from the set of specified objects. \~
    \param[in] items     - \ru Контейнер тел, может содержать #MbAssembly, #MbInstance и #MbSolid.
                           \en The bodies container, may consist of #MbAssembly, #MbInstance and #MbSolid. \~
    \param[in] operNames - \ru Именователь.
                           \en An object for naming the new objects. \~
    \param[in/out] prog  - \ru Индикатор прогресса выполнения операции.
                           \en An operation progress indicator. \~
*/
  MbIngotSolidParams( const c3d::ItemsSPtrVector & items, 
                      const MbSNameMaker &         operNames,
                            IProgressIndicator *   prog = nullptr );

/** \brief \ru Конструктор по параметрам операции без контейнера тел.
           \en Constructor by the operation parameters without an bodies container. \~
    \details \ru Конструктор параметров разделения оболочки на части без контейнера тел.
             \en Constructor of parameters to divide a shell into parts  without an bodies container. \~
    \param[in] operNames - \ru Именователь.
                           \en An object for naming the new objects. \~
    \param[in/out] prog  - \ru Индикатор прогресса выполнения операции.
                           \en An operation progress indicator. \~
*/
  MbIngotSolidParams( const MbSNameMaker &       operNames,
                            IProgressIndicator * prog = nullptr );

  /// \ru Добавление тел в контейнер. \en Add the solids to the container. \~
  void Add( const c3d::SolidsSPtrVector & solids );
  /// \ru Добавление тела в контейнер тел. \en Add a solid to the bodies container. \~
  void Add( const c3d::SolidSPtr & solid );
  /// \ru Добавление тел в контейнер. \en Add items to the container. \~
  void Add( const c3d::ItemsSPtrVector & items );
  /// \ru Добавление тела в контейнер тел. \en Add an item to the bodies container. \~
  void Add( const c3d::ItemSPtr & item );

  /// \ru Получить контейнер тел. \en Get the bodies container. \~
  void GetSolids( c3d::SolidsSPtrVector & solids ) const { solids.insert( solids.end(), _solids.begin(), _solids.end() ); }
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation. \~
  const MbSNameMaker & GetNameMaker() const { return _operNames; }

OBVIOUS_PRIVATE_COPY( MbIngotSolidParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры скругления цепочки граней тела.
            \en Parameters for creating a fillet of a chain of the body faces. \~
  \details \ru Параметры скругления цепочки граней тела.
            \en Parameters for creating a fillet of a chain of the body faces. \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbShellFullFilletParams
{
private:
  c3d::FacesSPtrVector  _initFaces;       ///< \ru Набор граней для скругления. \en A set of faces to fillet.
  c3d::FacesSPtrVector  _initFacesLeft;   ///< \ru Набор ограничивающих слева граней. \en A set of left bounding faces.
  c3d::FacesSPtrVector  _initFacesRight;  ///< \ru Набор ограничивающих справа граней. \en A set of right bounding faces.
  SPtr<MbSNameMaker>    _nameMaker;       ///< \ru Именователь операции. \en An object defining names generation in the operation.
public:
  FullFilletValues      _params;          ///< \ru Параметры скругления. \en Parameters of fillet.

    /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор параметров скругления цепочки граней тела.
             \en Constructor of parameters for creating a fillet of a chain of the body faces. \~
    \param[in] initFaces      - \ru Набор граней для скругления.
                                \en A set of faces to fillet. \~
    \param[in] initFacesLeft  - \ru Набор ограничивающих слева граней.
                                \en A set of left bounding faces. \~
    \param[in] initFacesRight - \ru Набор ограничивающих справа граней.
                                \en A set of right bounding faces. \~
    \param[in] params         - \ru Параметры операции.
                                \en The operation parameters. \~
    \param[in] names          - \ru Именователь.
                                \en An object for naming the new objects. \~
  */
  MbShellFullFilletParams( const c3d::FacesSPtrVector & initFaces, const c3d::FacesSPtrVector & initFacesLeft,
                           const c3d::FacesSPtrVector & initFacesRight, const FullFilletValues & params, const MbSNameMaker & names )
    : _initFaces     ( initFaces )
    , _initFacesLeft ( initFacesLeft )
    , _initFacesRight( initFacesRight )
    , _nameMaker     ( &names.Duplicate() )
    , _params        ( params ) {}

public:
  /// \ru Получить набор граней для скругления. \en Get faces to fillet.
  const c3d::FacesSPtrVector & GetFaces() const { return _initFaces; }

  /// \ru Получить набор ограничивающих слева граней. \en Get left bounding faces.
  const c3d::FacesSPtrVector & GetLeftFaces() const { return _initFacesLeft; }

  /// \ru Получить набор ограничивающих справа граней. \en Get right bounding faces.
  const c3d::FacesSPtrVector & GetRightFaces() const { return _initFacesRight; }

  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker & GetNameMaker() const { return *_nameMaker; }

OBVIOUS_PRIVATE_COPY( MbShellFullFilletParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры для определения оси токарного сечения и построения кривых сечения для тела.
           \en Parameters for looking the lathe axis and construction of lathe elements for the solid. \~
  \details \ru Параметры для поиска токарной оси среди граней вращения и построения токарного сечения в некоторой плоскости. \n 
           \en Parameters for finding the lathe axis by rotation faces and constructing the curves of lathe-section in a plane. \n \~
\ingroup Curve_Building_Parameters
*/
// ---
class MATH_CLASS MbLatheCurvesParams : public MbPrecision {

private:
  SPtr<const MbSolid> solid; ///< \ru Тело. \en Solid. \~ 
  const MbAxis3D *    axis;  ///< \ru Ось токарного сечения (может быть нуль). \en Lathe axis (may be null). \~ 
  double              angle; ///< \ru Угол, управляющий построением перпендикулярных оси сечения отрезками, рекомендуется M_PI_4-M_PI. \en The angle, managing the construction of segments which perpendicular to the axis, recomended M_PI_4-M_PI. \~ 
  double              astep; ///< \ru Угловой шаг при движении вдоль объекта. \en An anfgelar step (Math::deviateSag). \~ 

public:
   /** \brief \ru Конструктор параметров построения кривых токарного сечения для тела.
             \en Constructor of parameters for constructing curves of the lathe section of the body. \~
  \details \ru Конструктор параметров для определения оси токарного сечения и построения кривых токарного сечения тела.
           \en Constructor of parameters for determining the lathe axis and constructing curves of the lathe section of the body. \~
  \param[in] sol - \ru Тело. \en Solid. \~ 
  \param[in] axs - \ru Ось токарного сечения (может быть нуль). \en Lathe axis (may be null). \~ 
  \param[in] ang - \ru Угол, управляющий построением перпендикулярных оси сечения отрезками, рекомендуется M_PI_4-M_PI. \en The angle, managing the construction of segments which perpendicular to the axis, recomended M_PI_4-M_PI. \~ 
  \param[in] stp - \ru Угловой шаг при движении вдоль объекта. \en An anfgelar step (Math::deviateSag). \~ 
  */
  MbLatheCurvesParams ( const MbSolid &  sol,
                        const MbAxis3D * axs,
                              double     ang,
                              double     stp );
  /// \ru Конструктор копирования. \en Copy constructor. \~
  MbLatheCurvesParams ( const MbLatheCurvesParams & other );

public:

  /// \ru Тело. \en Solid. \~ 
  SPtr<const MbSolid>  GetSolid() const { return solid; }
  /// \ru Ось токарного сечения (может быть нуль). \en Lathe axis (may be null). \~ 
  const MbAxis3D *  GetAxis() const { return axis; }
  /// \ru Угол, управляющий построением перпендикулярных оси сечения отрезками, рекомендуется M_PI_4-M_PI. \en The angle, managing the construction of segments which perpendicular to the axis, recomended M_PI_4-M_PI. \~ 
  double            GetAngle() const { return angle; } 
  void              SetAngle( double ang ) { angle = ang; } 
  /// \ru Угловой шаг при движении вдоль объекта. \en An anfgelar step (Math::deviateSag). \~ 
  double            GetStep() const { return astep; } 
  void              SetStep( double ang = Math::deviateSag ) { astep = ang; } 

  /// \ru Оператор копирования. \en Copy operator. \~
  void              operator = ( const MbLatheCurvesParams  & other );
}; // MbLatheCurvesParams


//------------------------------------------------------------------------------
/** \brief \ru Выходные данные токарного сечения для тела.
           \en The output data of the lathe section for the solid. \~
  \details \ru Выходные данные содержат локальную систему координат (ЛСК) и двумерные кривые сечения, расположенные в плоскости XY ЛСК. \n 
           \en The output data contains a local coordinate system (LCS) and two-dimensional curves located in the XY plane of the LCS. \n \~
\ingroup Curve_Building_Parameters
*/
// ---
class MATH_CLASS MbLatheCurvesResult {

private:
  MbPlacement3D               position; ///< \ru ЛСК, в плоскости XY которой лежат кривые сечения, а ось X является осью токарного сечения. \en The local coordinate system (LCS), axis X is a axis of section. \~  
  std::vector<SPtr<MbCurve> > curves;   ///< \ru Кривые токарного сечения в плоскости XY ЛСК position. \en The curves of section located on the plane XY of the position. \~

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. \~
  MbLatheCurvesResult();
  /// \ru Конструктор копирования. \en Copy constructor. \~
  MbLatheCurvesResult( const MbLatheCurvesResult & other );

public:

  /// \ru Дать локальную систему координат. \en Get local coordinate system. \~ 
  const MbPlacement3D & GetPosition() const { return position; }
  void              SetPosition( const MbPlacement3D & pos ) { position = pos; }
  /// \ru Дать количество кривых токарного сечения. \en Get the curves count. \~ 
  size_t            GetCurvesCount() const { return curves.size(); }
  /// \ru Добавить кривые. \en Add curves. \~ 
  void              AddCurve( MbCurve & crs );
  void              AddCurves( RPArray<MbCurve> & crs );
  /// \ru Дать двумерные кривые. \en Get two-dimensional curves. \~ 
  void              GetCurves( std::vector<SPtr<MbCurve> > & crs ) const;
  void              DetachCurves( RPArray<MbCurve> & crs );
  std::vector<SPtr<MbCurve> > & SetCurves() { return curves; }

  /// \ru Оператор копирования. \en Copy operator. \~
  void              operator = ( const MbLatheCurvesResult  & other );
}; // MbLatheCurvesResult


//------------------------------------------------------------------------------
/** \brief \ru Параметры ребра жёсткости.
           \en Parameters of a rib. \~
  \details \ru Параметры построения ребра жёсткости по кривой, задающей его форму. \n
           \en The construction parameters of rib by curve gives its shape. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbRibSolidParameters
{
private:
  MbPlacement3D     _place;      ///< \ru Система координат образующего контура. 
                                 ///< \en The generating contour coordinate system.
  const MbContour & _contour;    ///< \ru Формообразующий контур на плоскости XY системы координат place. 
                                 ///< \en The generating contour on XY - plane of coordinate system 'place'.
  size_t            _index;      ///< \ru Номер сегмента в контуре. \en The segment number in the contour.
  RibValues         _params;     ///< \ru Параметры ребра жёсткости. \en Parameters of a rib.
  SPtr<MbSNameMaker>_operNames;  ///< \ru Именователь операции. \en An object defining names generation in the operation.
private:
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbRibSolidParameters( const MbRibSolidParameters &, MbRegDuplicate * ireg );
public:
  /// \ru Конструктор. \en Constructor.
  MbRibSolidParameters( const MbPlacement3D & place, const MbContour & contour,
                        size_t index, const RibValues & pars, const MbSNameMaker &  names );
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker &  GetNameMaker() const { return *_operNames; }
  /// \ru Получить параметры операции. \en Get the operation parameters.
  const RibValues & GetParams() const { return _params; }
  /// \ru Получить формообразующий контур. \en Get the generating contour .
  const MbContour & GetContour()const { return _contour; }
  /// \ru Получить номер сегмента в контуре. \en Get the segment number in the contour.
        size_t      GetIndex()  const { return _index; }
  /// \ru Получить систему координат образующего контура. \en Get the generating contour coordinate system.
  const MbPlacement3D & GetPlacement()  const { return _place; }

OBVIOUS_PRIVATE_COPY( MbRibSolidParameters )
};


//------------------------------------------------------------------------------
/** \brief \ru Решетчатая оболочка по трем управляющим точкам, параметрам решетки и количеству элементов. 
           \en Lattice shell based on three control points, lattice parameters and number of elements. \~
  \details \ru Решетчатая оболочка по трем управляющим точкам, параметрам решетки и количеству элементов. 
           \en Lattice shell based on three control points, lattice parameters and number of elements. \~
   \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbOctaLatticeParams
{
private:
  MbCartPoint3D      _points[3]; ///< \ru Управляющие точки. \en Control points.
  double             _radius[3]; ///< \ru Радиусы для направлений X,Y,Z. \en Radii for X, Y, Z directions.
  size_t             _counts[3]; ///< \ru Количество элементов для направлений X,Y,Z. \en Number of elements for directions X, Y, Z
  SPtr<MbSNameMaker> _operNames; ///< \ru Именователь операции. \en An object defining names generation in the operation.
private:
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbOctaLatticeParams( const MbOctaLatticeParams &, MbRegDuplicate * ireg );
public:
  /// \ru Конструктор. \en Constructor.
  MbOctaLatticeParams( const MbCartPoint3D & p0, const MbCartPoint3D & p1, const MbCartPoint3D & p2,
                       double radiusX, double radiusY, double radiusZ,
                       size_t xCount, size_t yCount, size_t zCount, 
                       const MbSNameMaker &  names );
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker &  GetNameMaker() const { return *_operNames; }
  /// \ru Получить управляющие точки. \en Get the control points.
  const MbCartPoint3D & GetPoint( size_t i ) const { return _points[i]; }
  /// \ru Получить радиусы. \en Get radii.
        double          GetRadius( size_t i ) const { return _radius[i]; }
  /// \ru Получить количество элементов. \en Get the number of elements.
        size_t          GetCount( size_t i ) const { return _counts[i]; }

OBVIOUS_PRIVATE_COPY( MbOctaLatticeParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры булевой операции.
           \en Parameters of Boolean operation. \~
  \details \ru Параметры булевой операции над телами.
           \en Parameters of Boolean operation. \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbBooleanOperationParams : public MbPrecision {
private:
  OperationType      _oType;         ///< \ru Тип булевой операции. \en A boolean operation type. \~
  MbBooleanFlags     _booleanFlags;  ///< \ru Управляющие флаги булевой операции. \en Control flags of the Boolean operation.
  SPtr<MbSNameMaker> _nameMaker;     ///< \ru Именователь операции. \en An object defining names generation in the operation.
 
public:
  mutable IProgressIndicator *  _progress;  ///< \ru Индикатор прогресса выполнения операции. \en A progress indicator of the operation.

public:
  /** \brief \ru Конструктор по параметрам операции.
             \en Constructor by the operation parameters. \~
    \details \ru Конструктор параметров булевой операции над телами.
             \en Constructor of parameters of Boolean operation. \~
    \param[in] oType     - \ru Тип булевой операции.
                           \en A Boolean operation type. \~
    \param[in] flags     - \ru Управляющие флаги булевой операции.
                           \en Control flags of the Boolean operation \~
    \param[in] operNames - \ru Именователь.
                           \en An object for naming the new objects. \~
    \param[in/out] progress  - \ru Индикатор прогресса выполнения операции.
                               \en A progress indicator of the operation.
  */
  MbBooleanOperationParams(       OperationType        oType, 
                            const MbBooleanFlags &     flags,
                            const MbSNameMaker &       operNames,
                                  IProgressIndicator * progress = nullptr )
    : MbPrecision  (                        )
    , _oType       ( oType                  )
    , _booleanFlags( flags                  )
    , _nameMaker   ( &operNames.Duplicate() )
    , _progress    ( progress               )
  {}
  /** \brief \ru Конструктор по параметрам операции.
             \en Constructor by the operation parameters. \~
    \details \ru Конструктор параметров булевой операции над телами.
             \en Constructor of parameters of Boolean operation. \~
    \param[in] oType     - \ru Тип булевой операции.
                           \en A Boolean operation type. \~
    \param[in] closed    - \ru Замкнутость оболочек операндов.
                           \en Closedness of operands' shells. \~
    \param[in] operNames - \ru Именователь.
                           \en An object for naming the new objects. \~
    \param[in/out] progress  - \ru Индикатор прогресса выполнения операции.
                               \en A progress indicator of the operation.
  */
  MbBooleanOperationParams(       OperationType        oType, 
                                  bool                 closed,
                            const MbSNameMaker &       operNames,
                                  IProgressIndicator * progress = nullptr )
    : MbPrecision  (                        )
    , _oType       ( oType                  )
    , _booleanFlags(                        )
    , _nameMaker   ( &operNames.Duplicate() )
    , _progress    ( progress               )
  {
    _booleanFlags.InitBoolean( closed );
  }
public:
  /// \ru Тип Булевой операции. \en A boolean operation type. \~
  OperationType          GetOperationType() const { return _oType; }
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker &   GetNameMaker()     const { return *_nameMaker; }
  /// \ru Получить управляющие флаги булевой операции. \en Get control flags of the Boolean operation.
  const MbBooleanFlags & GetBooleanFlags()  const { return _booleanFlags; }
  
OBVIOUS_PRIVATE_COPY( MbBooleanOperationParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры построения элементарного тела.
           \en Parameters for creating an elementary solid. \~
  \details \ru Параметры построения элементарного тела.
           \en Parameters for creating an elementary solid. \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbElementarySolidParams {

public:
  /** \brief \ru Тип точки, находящейся в центре плейсмента параллелепипеда.
             \en The type of point located in the center of the placement of the parallelepiped. \~
  */
  enum class BlockPointType
  {
    vertex =     0, ///< \ru Вершина параллелепипеда. \en Vertex of a parallelepiped.
    edgeCenter = 1, ///< \ru Центр ребра параллелепипеда, направленного по оси Ox. \en Center of the parallelepiped edge directed along the Ox axis.
    faceCenter = 2  ///< \ru Центр грани лежащей в плоскости XY. \en The center of a face lying in the XY plane.
  };

public:
  ElementaryShellType _solidType; ///< \ru Тип создаваемого тела. \en The solid type.

protected:
  c3d::SpacePointsVector          _points;      ///< \ru Множество точек. \en Set of points.
  SPtr<const MbElementarySurface> _initSurface; ///< \ru Исходная поверхность. \en Initial surface.
  c3d::SNameMakerSPtr             _operNames;   ///< \ru Именователь операции. \en An object defining names generation in the operation.

private:
  double                          _offset;      ///< \ru Параметр смещения. \en Offset parameter.
  bool                            _isInverted;  ///< \ru Признак инвертированного тела. \en Sign of an inverted body.

public:
  /** \brief \ru Конструктор параметров элементарного тела.
             \en Elementary solid parameter constructor. \~
    \details \ru Конструктор параметров элементарного тела с неопределенным типом.
             \en Elementary solid parameters constructor with undefined type. \~
    \param[in] operNames - \ru Именователь операции.
                           \en An object for naming the new objects. \~
  */
  MbElementarySolidParams( const MbSNameMaker & operNames );

  /** \brief \ru Конструктор параметров элементарного тела.
             \en Elementary solid parameter constructor. \~
      \details \ru Конструктор параметров элементарного тела по набору точек.
               \en Constructor of parameters for creating an elementary solid by a set of points. \~
      \param[in] solidType - \ru Тип создаваемого тела.
                             \en The solid type. \~
      \param[in] points    - \ru Множество точек.
                             \en Set of points. \~
      \param[in] operNames - \ru Именователь операции.
                             \en An object for naming the new objects. \~
  */
  MbElementarySolidParams( const ElementaryShellType & solidType, const c3d::SpacePointsVector & points, const MbSNameMaker & operNames );

  /** \brief \ru Конструктор параметров элементарного тела.
             \en Elementary solid parameter constructor. \~
      \details \ru Конструктор параметров элементарного тела по элементарной поверхности.
               \en Constructor of parameters for creating an elementary solid by an elementary surface. \~
      \param[in] surface   - \ru Элементарная поверхность.\n
                                 Допускается тип поверхности - шар, тор, цилиндр, конус.
                             \en Elementary surface.\n
                                 The acceptable surface types are sphere, torus, cylinder, cone. \~
      \param[in] operNames - \ru Именователь операции.
                             \en An object for naming the new objects. \~
  */
  MbElementarySolidParams( const SPtr<const MbElementarySurface> & surface, const MbSNameMaker & operNames );

  /// \ru Деструктор. \en Destructor. \~
  ~MbElementarySolidParams() {}

  /** \brief \ru Метод инициализации параметров элементарного тела.
             \en Initialization method for elementary solid parameters. \~
      \details \ru Метод инициализации параметров элементарного тела по типу тела и набору точек.
               \en Initialization method for elementary solid parameters by solid type and a set of points. \~
      \param[in] solidType - \ru Тип создаваемого тела.
                             \en The solid type. \~
      \param[in] points    - \ru Множество точек.
                             \en Set of points. \~
      \return \ru Возвращает true в случае успеха.
              \en Returns true in case of success. \~
  */
  bool Init( const ElementaryShellType & solidType, const c3d::SpacePointsVector & points );

  /** \brief \ru Метод инициализации параметров элементарного тела.
             \en Initialization method for elementary solid parameters. \~
      \details \ru Метод инициализации параметров элементарного тела по элементарной поверхности.
               \en Initialization method for elementary solid parameters by an elementary surface. \~
      \param[in] surface   - \ru Элементарная поверхность.\n
                                 Допускается тип поверхности - шар, тор, цилиндр, конус.
                             \en Elementary surface.\n
                                 The acceptable surface types are sphere, torus, cylinder, cone. \~
      \return \ru Возвращает true в случае успеха.
              \en Returns true in case of success. \~
  */
  bool Init( const SPtr<const MbElementarySurface> & surface );

  /** \brief \ru Метод инициализации параметров элементарного тела для конуса.
             \en Initialization method of elementary solid parameters for cone. \~
    \details \ru Метод инициализации параметров элементарного тела для конуса по плейсменту, радиусам оснований конуса и 
                 значению, которое может быть высотой (отрицательной или положительной) или углом при вершине конуса.
             \en Initialization method for elementary solid parameters by placement, radiuses of the cone bases 
                 and a value that can be the height or angle at the cone vertex. \~
    \param[in] place   - \ru Плейсмент. \n
                         \en Placement. \~
    \param[in] r1      - \ru Радиус основания конуса на плоскости XY. \n
                         \en Radius of the cone base on the XY plane. \~
    \param[in] r2      - \ru Радиус второго основания конуса, 0 ≤ r2 < r1. \n
                         \en Radius of the second cone base, 0 ≤ r2 < r1. \~
    \param[in] bHeight - \ru Если true, то value - высота, иначе угол. \n
                         \en If true, then a value is a height, else the value is an angle. \~
    \param[in] value   - \ru Если bHeight = true, то value = высота (отрицательная или положительная), 
                             иначе value = угол при вершине конуса (между двумя противоположными образующими),
                             0 < угол < Pi. \n
                         \en If bHeight = true, then value = height (negative or positive), else value = angle 
                             at the cone vertex (between two opposite generatrices), 0 < angle < Pi \~
    \return \ru Возвращает true в случае успеха.
            \en Returns true in case of success. \~
  */
  bool InitCone( const MbPlacement3D & place, double r1, double r2, bool bHeight, double value );

  /**\brief \ru Метод инициализации параметров элементарного тела для сферы.
            \en Initialization method of elementary solid parameters for sphere. \~
    \details \ru Метод инициализации параметров элементарного тела для сферы по плейсменту и радиусу сферы.
             \en Initialization method of elementary solid parameters for sphere by placement and radius of the sphere. \~
    \param[in] place - \ru Плейсмент. \n
                       \en Placement. \~
    \param[in] r     - \ru Радиус сферы. \n
                       \en Radius of sphere. \~
    \return \ru Возвращает true в случае успеха.
            \en Returns true in case of success. \~
  */
  bool InitSphere( const MbPlacement3D & place, double r );

  /** \brief \ru Метод инициализации параметров элементарного тела для сферы.
            \en Initialization method of elementary solid parameters for sphere. \~
   \details \ru Метод инициализации параметров элементарного тела для сферы по дуге, определяющей центр, диаметр, и плейсмент сферы.
            \en Initialization method of elementary solid parameters for sphere by an arc that defines the center, diameter,
                and placement of the sphere. \~
   \param[in] arc - \ru Дуга, определяющая центр, диаметр, и плейсмент сферы. \n
                    \en An arc defining the center, diameter, and placement of a sphere. \~
   \return \ru Возвращает true в случае успеха.
           \en Returns true in case of success. \~
 */
  bool InitSphere( const MbArc3D & arc );

  /** \brief \ru Метод инициализации параметров тела параллелепипеда.
             \en Method for initializing block body parameters. \~
      \details \ru Метод инициализации параметров тела параллелепипеда по
                   плесменту, точке диагонали и высотам.
               \en Method for initializing parameters of a parallelepiped body by
                   placement, diagonal point and heights. \~
      \param[in] place  - \ru Плейсмент. Центр плейсмента является одной из вершин параллелепипеда.
                          \en Placement. The center of placement is one of the vertices of the parallelepiped. \~
      \param[in] pt  - \ru Точка, проекция которой является вершиной параллелепипеда лежащей
                           в плоскоси XY по диагонали относительно центра плейсмента.\n
                       \en A point whose projection is the vertex of a parallelepiped lying
                           in the XY plane diagonally relative to the center of the placement. \~
      \param[in] h1  - \ru Высота параллелепипеда от плоскости XY в направлении оси Z.
                       \en The height of the parallelepiped from the XY plane in the direction of the Z axis. \~
      \param[in] h2  - \ru Высота параллелепипеда от плоскости XY противоположно оси Z.
                       \en The height of the parallelepiped from the XY plane is opposite to the Z axis. \~
      \return  - \ru Возвращает true в случае успеха.
                 \en Returns true in case of success. \~
  */
  bool InitBlock( const MbPlacement3D & place, const MbCartPoint3D & pt, double h1, double h2 );

  /** \brief \ru Метод инициализации параметров тела параллелепипеда.
             \en Method for initializing block body parameters. \~
      \details \ru Метод инициализации параметров тела параллелепипеда по плесменту,
                   длине, ширине и высотам.
               \en Method for initializing the parameters of a parallelepiped body by placement,
                   length, width and heights. \~
      \param[in] place  - \ru Плейсмент.
                          \en Placement. \~
      \param[in] dx  - \ru Длина.
                       \en Length. \~
      \param[in] dy  - \ru Ширина.
                       \en Width. \~
      \param[in] h1  - \ru Высота параллелепипеда от плоскости XY в направлении оси Z.
                       \en The height of the parallelepiped from the XY plane in the direction of the Z axis. \~
      \param[in] h2  - \ru Высота параллелепипеда от плоскости XY противоположно оси Z.
                       \en The height of the parallelepiped from the XY plane is opposite to the Z axis. \~
      \param[in] pointType  - \ru Тип точки, находящейся в центре плейсмента:
                                  1. Вершина.
                                  2. Центр ребра направленного по оси X.
                                  3. Центр нижнего основания.
                              \en Type of point located in the center of the placement:
                                  1. Vertex.
                                  2. Center of the edge directed along the X axis.
                                  3. Center of the lower base. \~
      \return  - \ru Возвращает true в случае успеха.
                 \en Returns true in case of success. \~
  */
  bool InitBlock( const MbPlacement3D & place, double dx, double dy, double h1, double h2, BlockPointType pointType );

  /** \brief \ru Метод инициализации параметров тела цилиндра.
             \en Method for initializing cylinder body parameters. \~
      \details \ru Метод инициализации параметров тела цилиндра по плесменту, радиусу и высотам.
               \en Method for initializing cylinder body parameters by placement, radius and heights. \~
      \param[in] place  - \ru Плейсмент.
                          \en Placement. \~
      \param[in] r  - \ru Радиус.
                      \en Radius. \~
      \param[in] h1  - \ru Высота цилиндра от плоскости XY в направлении оси Z.
                       \en The height of the cylinder from the XY plane in the direction of the Z axis. \~
      \param[in] h2  - \ru Высота цилиндра от плоскости XY противоположно оси Z.
                       \en The height of the cylinder from the XY plane is opposite to the Z axis. \~
      \return  - \ru Возвращает true в случае успеха.
                 \en Returns true in case of success. \~
  */
  bool InitCylinder( const MbPlacement3D & place, double r, double h1, double h2 );

  /** \brief \ru Метод инициализации параметров тела цилиндра.
             \en Method for initializing cylinder body parameters. \~
      \details \ru Метод инициализации параметров тела цилиндра по окружности основания и высотам.
               \en Method for initializing the parameters of a cylinder body by base circumference and heights. \~
      \param[in] arc  - \ru Основание цилиндра.\n
                        \en Cylinder base. \~
      \param[in] h1  - \ru Высота цилиндра от плоскости XY в направлении оси Z.
                       \en The height of the cylinder from the XY plane in the direction of the Z axis. \~
      \param[in] h2  - \ru Высота цилиндра от плоскости XY противоположно оси Z.
                       \en The height of the cylinder from the XY plane is opposite to the Z axis. \~
      \return  - \ru Возвращает true в случае успеха.
                 \en Returns true in case of success. \~
  */
  bool InitCylinder( const MbArc3D & arc, double h1, double h2 );

  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker & GetNameMaker() const { return *_operNames; }

  /// \ru Получить множество точек. \en Get the set of points.
  const c3d::SpacePointsVector & GetPoints() const { return _points; }

  /// \ru Получить исходную поверхность. \en Get the initial surface.
  const SPtr<const MbElementarySurface> & GetInitSurface() const { return _initSurface; }

  /// \ru Получить смещение. \en Get offset.
  const double GetOffsetValue() const { return _offset; }

  /// \ru Получить признак инвертировонности тела. \en Get sign of an inverted body.
  const bool GetIsInvertedValue() const { return _isInverted; }

OBVIOUS_PRIVATE_COPY( MbElementarySolidParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Результаты построения элементарного тела.
           \en Results of construction an elementary solid. \~
  \details \ru Результаты построения элементарного тела.
           \en Results of construction an elementary solid. \~
\ingroup ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbElementarySolidResults : public MbOperationResults {
private:
  c3d::SolidSPtr _solid; /// \ru Результирующее тело. \en Resulting solid.
public:
  MbElementarySolidResults()
    : MbOperationResults()
    , _solid            ()
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbElementarySolidResults( const MbElementarySolidResults & other )
    : MbOperationResults( other        )
    , _solid            ( other._solid )
  {}
  ~MbElementarySolidResults() {}
  /// \ru Оператор присваивания. \en Assignment operator.
  MbElementarySolidResults & operator = ( const MbElementarySolidResults & other ) {
    MbOperationResults::operator =( static_cast<const MbOperationResults &>(other) );
    _solid = other._solid;
    return *this;
  }
    /// \ru Функция инициализации. \en Initialization function.
  void Init( MbSolid * resultSolid ) {  
    _solid = resultSolid;
  }
  /// \ru Получить результирующее тело. \en Get resulting solid.
  const c3d::SolidSPtr & GetResultSolid() const { return _solid; }
  /// \ru Отцепить результирующее тело. \en Detach resulting solid.
  MbSolid * DetachSolid() { return _solid.detach(); }
  /** \brief \ru Выдать хот-точки элементарного тела.
             \en Get hot points of elementary solid. \~
    \details \ru Выдать хот-точки элементарных тел: сфера, цилиндр, конус, параллелепипед.\n
             \en Get hot points of elementary solids: sphere, cylinder, cone, block.\n
    \param[out] hotPoints  - \ru Набор хот-точек в зависимости от типа элементарного тела: \n
                                 et_Sphere: хот-точка радиуса сферы; \n
                                 et_Cylinder: хот-точка радиуса, хот-точка высоты в первом направлении, хот-точка высоты во втором направлении; \n
                                 et_Cone: хот-точка радиуса нижнего основания, хот-точка радиуса верхнего основания,  хот-точка радиуса высоты, хот-точка радиуса угла в вершине; \n
                                 et_Block: хот-точка длины, хот-точка ширины, хот-точка высоты в первом направлении, хот-точка высоты во втором направлении; \n
                            \en Hot points set depending on the type of elementary solid:
                                 et_Sphere: sphere radius hot-point; \n
                                 et_Cylinder: radius hot-point, height hot-point in 1st direction, height hot-point in 2nd direction; \n
                                 et_Cone: bottom radius hot-point, top radius hot-point, height hot-point,angle hot-point; \n
                                 et_Block: length hot-point, width hot-point, height hot-point in 1st direction, height hot-point in 2nd direction; \n
                           \~
    \return \ru true, в случае успеха.
            \en true if success. \~
  */
  bool GetElementarySolidHotPoints( std::vector<c3d::SpacePointVector> & hotPoints ) const;
};

//------------------------------------------------------------------------------
/** \brief \ru Параметры выделения в отдельное тело указанной части тела.
           \en Parameters for extracting a specified part of a decomposing solid. \~
    \details \ru Параметры выделения в отдельное тело указанной части тела.
             \en Parameters for extracting a specified part of a decomposing solid. \~
    \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbShellPartParams {

protected:
   size_t             _id;          ///< \ru Номер выбранной части тела. \en The number of selected part of the solid.
   MbPath             _path;        ///< \ru Идентификатор для выбранной части. \en An identifier for the selected part.
   SPtr<MbSNameMaker> _operNames;   ///< \ru Именователь операции. \en An object defining names generation in the operation.
   MbPartSolidIndices _partIndices; ///< \ru Индексы частей тела. \en Indices of the parts of the solid.

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
      \details \ru Конструктор параметров оболочки выделения.
               \en Constructor of extraction shell parameters. \~
      \param[in] id          - \ru Номер выбранной части тела.
                               \en The number of selected part of the solid. \~
      \param[in] path        - \ru Идентификатор для выбранной части.
                               \en An identifier for the selected part. \~
      \param[in] operNames   - \ru Именователь операции.
                               \en An object for naming the new objects. \~
      \param[in] partIndices - \ru Индексы частей тела.
                               \en Indices of the parts of the solid. \~
  */
  MbShellPartParams( size_t id, const MbPath & path, const MbSNameMaker & operNames, const MbPartSolidIndices & partIndices )
  : _id         ( id                     )
  , _path       ( path                   )
  , _operNames  ( &operNames.Duplicate() )
  , _partIndices( partIndices            )
  {
  }

  /// \ru Деструктор. \en Destructor.
  ~MbShellPartParams() {}
  /// \ru Получить номер выбранной части тела. \en Get the number of selected part of the solid.
  size_t GetId() const { return _id; }
  /// \ru Получить идентификатор для выбранной части. \en Get the identifier for the selected part.
  const MbPath & GetPath() const { return _path; }
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker & GetNameMaker() const { return *_operNames; }
  /// \ru Получить индексы частей тела. \en Get the indices of the parts of the solid.
  const MbPartSolidIndices & GetPartIndices() const { return _partIndices; }

OBVIOUS_PRIVATE_COPY( MbShellPartParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Результаты выделения в отдельное тело указанной части тела.
           \en Results of extracting a specified part of a decomposing solid. \~
    \details \ru Результаты выделения в отдельное тело указанной части тела.
             \en Results of extracting a specified part of a decomposing solid. \~
    \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbShellPartResults {
private:
  MbPartSolidIndices _partIndices; ///< \ru Индексы частей тела. \en Indices of the parts of the solid.
public:
  c3d::SolidSPtr     _solid;       ///< \ru Результирующее тело. \en The resultant solid.

public:
  /// \ru Конструктор. \en Constructor.
  MbShellPartResults()
  : _partIndices()
  , _solid()
  {
  }

  /// \ru Конструктор по индексам частей тела. \en Constructor by the indices of the parts of the solid.
  MbShellPartResults( const MbPartSolidIndices & partIndices )
  : _partIndices( partIndices )
  , _solid()
  {
  }

  /// \ru Деструктор. \en Destructor.
  ~MbShellPartResults() {}

  /// \ru Получить индексы частей тела. \en Get the indices of the parts of the solid.
  const MbPartSolidIndices & GetPartIndices() const { return _partIndices; }

  /// \ru Инициализировать индексы частей тела. \en Initialize indices of body parts.
  void InitPartIndices( const MbPartSolidIndices & partIndices )
  {
    _partIndices.RemoveAllIndices();
    _partIndices.AddPartIndices( partIndices );
  }

OBVIOUS_PRIVATE_COPY( MbShellPartResults )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры построения оболочки с отверстием, карманом, или фигурным пазом.
           \en Parameters for creating a shell with a hole, a pocket or a groove. \~
  \details \ru Параметры построения оболочки с отверстием, карманом, или фигурным пазом.
           \en Parameters for creating a shell with a hole, a pocket or a groove. \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbHoleSolidParams {

protected:
        MbPlacement3D       _place;      ///< \ru Локальная система координат. \en A local coordinate system.
  const HoleValues &        _parameters; ///< \ru Параметры построения. \en Parameters of a shell creation.
        c3d::SNameMakerSPtr _operNames;  ///< \ru Именователь операции. \en An object defining names generation in the operation.

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
      \details \ru Конструктор параметров оболочки с отверстием, карманом, или фигурным пазом.
               \en Constructor of parameters for creating a shell with a hole, a pocket or a groove. \~
      \param[in] place     - \ru Локальная система координат.
                             \en A local coordinate system. \~
      \param[in] params    - \ru Параметры операции.
                             \en Parameters of operation. \~
      \param[in] operNames - \ru Именователь операции.
                             \en An object for naming the new objects. \~
  */
  MbHoleSolidParams( const MbPlacement3D & place, const HoleValues & params, const MbSNameMaker & operNames )
  : _place     ( place                  )
  , _parameters( params                 )
  , _operNames ( &operNames.Duplicate() )
  {
  }

  /// \ru Деструктор. \en Destructor.
  ~MbHoleSolidParams() {}
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker & GetNameMaker() const { return *_operNames; }
  /// \ru Получить локальную систему координат. \en Get the local coordinate system.
  const MbPlacement3D & GetPlace() const { return _place; }
  /// \ru Получить параметры операции. \en Get the parameters of operation.
  const HoleValues & GetParameters() const { return _parameters; }

OBVIOUS_PRIVATE_COPY( MbHoleSolidParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Набор граней одного тела для построения скругления несвязных групп граней.
           \en Faces of one solid to create a fillet between two disjoint sets of faces. \~
  \ingroup Shell_Building_Parameters
  \warning \ru В разработке. \en Under development.
*/
// ---
class MATH_CLASS MbFilletBundle
{
private:
  c3d::SolidSPtr           _solid;     ///< \ru Тело. Всегда не null. \en The solid. Not null.
  c3d::FacesSPtrVector     _faces;     ///< \ru Грани тела. Хотя бы одна грань не null. \en The faces of the solid. At least one face is not null.
  std::vector<bool>        _faceSide;  ///< \ru Сторона грани, с которой ее будет касаться поверхность скругления (синхронизованно с _faces). \en Side of a face that fillet surface will touch (synchronized with _faces).
  std::vector<MbItemIndex> _faceIndex; ///< \ru Номера опорных граней. \en The reference face numbers (may be empty). \~
  c3d::FunctionSPtr        _function;  ///< \ru Функция радиуса для набора граней (всегда не null). \en The function of the fillet radius for the face set (always not null). 

public:
  /// \ru Конструктор по умолчанию. \en Empty constructor.
  MbFilletBundle();
  /// \ru Конструктор по параметрам для набора граней. \en Constructor by parameters for a face set.
  MbFilletBundle( const c3d::SolidSPtr & sol, const c3d::FunctionSPtr & func,
                  const c3d::FacesSPtrVector & faces, const std::vector<bool> & fSide );
  /// \ru Конструктор по параметрам для одной грани. \en Constructor by parameters for one face.
  MbFilletBundle( const c3d::SolidSPtr & sol, const c3d::FunctionSPtr & func,
                  const c3d::FaceSPtr & face, bool fSide );
  /// \ru Конструктор копирования с регистратором. \en Copy constructor with registrator.
  MbFilletBundle( const MbFilletBundle & other, MbRegDuplicate * iReg );
  /// \ru Конструктор копирования без копирования указателей. \en Copy-constructor without pointer copy.
  MbFilletBundle( MbSectionRail & other );

public:
  /// \ru Получить тело. \en Get the solid.
  const c3d::SolidSPtr & GetSolid() const { return _solid; }
  MbSolid *       SetSolid() { return _solid.get(); }
  /// \ru Получить тело. \en Get the solid.
  void            SetSolid( MbSolid & sol ) { if (_solid.get() != &sol) _solid = &sol; }

  /// \ru Добавить в данные поверхность. \en Add surface to data. \~
  bool            AddFace( MbFace & face, bool side, MbSolid * sol = nullptr, MbFunction * func = nullptr);
  /// \ru Выдать количество опорных граней. \en Get guide faces count.
  size_t          GetFacesCount() const { return _faces.size(); }
  /// \ru Выдать грани. \en Get faces.
  const c3d::FacesSPtrVector & GetFaces() const { return _faces; }
  void            GetFaces( std::vector<MbFace *> & fas ) const;
  void            GetFaces( RPArray<MbFace> & fas ) const;
  /// \ru Выдать опорную грань. \en Get guide face.
  const MbFace *  GetFace( size_t i ) const { return ( i < _faces.size() ) ? _faces[i].get() : nullptr; }
  MbFace *        SetFace( size_t i ) { return ( i < _faces.size() ) ? _faces[i].get() : nullptr; }

  /// \ru Выдать количество опорных граней. \en Get guide faces count.
  size_t          GetFaceSideCount() const { return _faceSide.size(); }
  /// \ru С каких сторон касаться поверхностей? \en On which sides to touch surfaces?
  const std::vector<bool> & GetFaceSide() const { return _faceSide; }
  bool            GetFaceSide( size_t i ) const { return _faceSide[i]; }
  void            GetFaceSide( std::vector<bool> & fSide ) const;
  /// \ru Установить сторону касания грани. \en Set face side.
  void            SetFaceSide( size_t i, bool s ) { if ( i < _faceSide.size() ) _faceSide[i] = s; }
  void            AddFaceSide( bool s ) { _faceSide.push_back( s ); }

  /// \ru Выдать количество опорных граней. \en Get guide faces count.
  size_t          GetFaceIndexCount() const { return _faceIndex.size(); }
  /// \ru Выдать номера опорных граней. \en Get reference face numbers.
  const std::vector<MbItemIndex> & GetFaceIndex() const { return _faceIndex; }
  void            GetFaceIndex( std::vector<MbItemIndex> & fInd ) const;
  const MbItemIndex & GetFaceIndex( size_t i ) const { return _faceIndex[i]; }
  void            AddFaceIndex( MbItemIndex & ind ) { _faceIndex.push_back( ind ); }

  /// \ru Получить функцию радиуса скругления для набора граней. \en Get the function of fillet radius for the face set.
  const c3d::FunctionSPtr & GetFunction() const { return _function; }
  MbFunction *    SetFunction() { return _function.get(); }
  /// \ru Установить функцию радиуса скругления для набора граней. \en Set the function of fillet radius for the face set.
  void            SetFunction( MbFunction & f ) { if ( _function.get() != &f) _function = &f; }

  /// \ru Преобразовать объект. \en Transform the object. \~
  void            Transform( const MbMatrix3D & matr, MbRegTransform * iReg = nullptr );
  /// \ru Сдвинуть объект. \en Move the object. \~
  void            Move     ( const MbVector3D & to, MbRegTransform * iReg = nullptr );
  /// \ru Повернуть объект. \en Rotate the object. \~
  void            Rotate   ( const MbAxis3D & axis, double angle, MbRegTransform * iReg = nullptr );
  /// \ru Определить, являются ли объекты равными? \en Determine whether an object is equal?
  bool            IsSame( const MbFilletBundle & other, double accuracy ) const;
  /// \ru Определить, являются ли объекты подобными. \en Determine whether the objects are similar. \~
  bool            IsSimilar( const MbFilletBundle & other ) const;
  /// \ru Сделать объекты равным. \en Make objects equal. \~
  bool            SetEqual ( const MbFilletBundle & other );
  /// \ru Выдать базовые объекты. \en Get basis objects.
  void            GetBasisItems(RPArray<MbSpaceItem>& s) const;
  /// \ru Заполнить контейнер граней по контейнеру индексаов. \en Find a set of faces by a set of combined indices. 
  void            FindFaceByIndex();

  /// \ru Оператор присваивания без копирования топологических объектов. \en Assignment operator without copying topological objects.
  void            operator = ( const MbFilletBundle & other );

private :
  /// \ru Конструктор копирования не реализован. \en Copy-constructor not realize.
  MbFilletBundle( const MbFilletBundle & other );

public:
  KNOWN_OBJECTS_RW_REF_OPERATORS( MbFilletBundle ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.

}; // MbFilletBundle


//------------------------------------------------------------------------------
/** \brief \ru Данные для построения сопряжения несвязных граней.
           \en Data to create fillet between disjoint faces. \~
  \details \ru Данные для построения гладкого сопряжения двух несвязных (непересекающихся) наборов граней.
               Каждый набор может состоять только из связных граней одного тела.
           \en Data to create a smooth fillet faces between two disjoint sets of faces.
               Each set can consists of connected faces of one solid. \~
  \ingroup Shell_Building_Parameters
  \warning \ru В разработке. \en Under development. \~
  \warning \ru Член класса SmoothValues _params будет удален в версии 2024. \en Class member SmoothValues _params will be removed in version 2024.
*/
// ---
class MATH_CLASS MbFilletData : public MbPrecision
{
private:
  MbFilletBundle       _faceSet1;  ///< \ru Первый набор сопрягаемых граней. \en The first set of conjugating faces. \~
  MbFilletBundle       _faceSet2;  ///< \ru Второй набор сопрягаемых граней. \en The second set of conjugating faces. \~
  MbeCopyMode          _copyMode;  ///< \ru Способы передачи данных при копировании оболочек. \en Methods of transferring data while copying shells. \~
  MbeSmoothForm        _sform;     ///< \ru Форма поверхности сопряжения (скругления или фаски). \en The blend surface cross-section form (fillet or chamfer). \~
  c3d::SpaceCurveSPtr  _spine;     ///< \ru Опорная кривая для задания переменных величин (может отсутствовать). \en The reference curve to set variables (may be nullptr). \~ 
  c3d::FunctionSPtr    _descript;  ///< \ru Функция управления сечением (дискриминант, может быть nullptr). \en Section control function (discriminant). \~
  MbeSideShape         _sideShape; ///< \ru Форма обрезки боков поверхности. \en The form of cropping the sides of the surface. \~
  MbeFaceHandling      _handling;  ///< \ru Обработка исходных опорных граней. \en The processing of initial reference faces. \~
  bool                 _faceSplit; ///< \ru Разделять оболочку на грани по сегментам опорных кривых. \en Split the shell into faces by segments of support curves. \~
  bool                 _elongated; ///< \ru Обрезать опорную оболочку на границе по касательной (true)/по нормали (false) к граничному ребру. \en The support shell will cutted along the tangent (true)/along the normal (false) to the boundary edge. \~
  bool                 _prolong;   ///< \ru Продолжить по касательной. \en Prolong along the tangent. \~
  ThreeStates          _keepCant;  ///< \ru Автоопределение сохранения кромки (ts_neutral), сохранение поверхности (ts_negative), сохранение кромки (ts_positive). \en Auto detection of boundary saving (ts_neutral), surface saving (ts_negative), boundary saving (ts_positive). \~
  bool                 _equable;   ///< \ru Флаг обработки некасательных стыков. True, если в углах сочленения вставлять тороидальную поверхность. \en Non tangent joints handling flag. True, if insert toroidal surface in corners of the joint. \~
  double               _buildSag;  ///< \ru Шаг построения. \en Build step.
  bool                 _check;     ///< \ru Проверять самопересечение построенной поверхности. \en Check the self-intersection of the constructed surface (default false). \~
  MbSNameMaker         _nameMaker; ///< \ru Именователь операции. \en An object defining names generation in the operation.

public:
  /// \ru Конструктор по умолчанию. \en Empty constructor.
  MbFilletData();

  /** \brief \ru Конструктор по параметрам. \en Constructor by parameters. \~
    \details \ru Конструктор данных построения гладкого сопряжения двух несвязных наборов граней.
             \en Constructor by parameters to create smooth fillet faces between two disjoint sets of faces. \~
    \param[in] faces1    - \ru Первый набор сопрягаемых граней.
                           \en The first set of conjugating faces. \~
    \param[in] faces2    - \ru Второй набор сопрягаемых граней.
                           \en The second set of conjugating faces. \~
    \param[in] faceSplit - \ru Разделять оболочку на грани по сегментам направляющих кривых.
                           \en Split the shell into faces by segments of guide curves. \~
    \param[in] sideShape - \ru Форма обрезки боков поверхности.
                           \en The form of cropping the sides of the surface. \~
    \param[in] function1 - \ru Функция радиуса для первого набора граней (всегда не null). Константная функция в случае скругления с постоянным радиусом.
                           \en The function of the fillet radius for the first set of faces (always not null). Constant function in case of fillet with constant radius.\~
    \param[in] function2 - \ru Функция радиуса для второго набора граней (всегда не null). Константная функция в случае скругления с постоянным радиусом.
                           \en The function of the fillet radius for the second set of faces (always not null). Constant function in case of fillet with constant radius. \~
    \param[in] conic     - \ru Коэффициент формы, изменяется от 0.05 до 0.95 (при 0 - дуга окружности).
                           \en Coefficient of shape is changed from 0.05 to 0.95 (if 0 - circular arc). \~
    \param[in] prolong   - \ru Продолжить по касательной.
                           \en Prolong along the tangent. \~
    \param[in] keepCant  - \ru Автоопределение сохранения кромки (ts_neutral), сохранение поверхности (ts_negative), сохранение кромки (ts_positive).
                           \en Auto detection of boundary saving (ts_neutral), surface saving (ts_negative), boundary saving (ts_positive). \~
    \param[in] equable   - \ru В углах сочленения вставлять тороидальную поверхность.
                           \en In corners of the joint insert toroidal surface. \~
    \param[in] nameMaker  - \ru Именователь новых граней операции.
                            \en An object defining names generation in the operation. \~
  */
  MbFilletData( const MbFilletBundle & faces1, const MbFilletBundle & faces2,
                bool split, bool elong, MbeSideShape shape,
                bool prol, ThreeStates kCant, bool equable, const MbSNameMaker & nameMaker );

  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with registrator.
  MbFilletData( const MbFilletData & other, MbRegDuplicate * iReg );
  /// \ru Конструктор копирования без копирования указателей. \en Copy-constructor without pointer copy.
  MbFilletData( MbSectionData & d );
  /// \ru Деструктор. \en Destructor.
  ~MbFilletData() {};

public:

  /// \ru Получить первый набор сопрягаемых граней. \en Get the first solid and set of conjugating faces.
  const MbFilletBundle & GetFaceSet1() const { return _faceSet1; }
  MbFilletBundle & SetFaceSet1() { return _faceSet1; }
  /// \ru Получить второй набор сопрягаемых граней. \en Get the second solid and set of conjugating faces.
  const MbFilletBundle & GetFaceSet2() const { return _faceSet2; }
  MbFilletBundle & SetFaceSet2() { return _faceSet2; }

  /// \ru Получить первое тело. \en Get the first solid.
  const c3d::SolidSPtr & GetSolid1() const { return _faceSet1.GetSolid(); }
  /// \ru Получить второе тело. \en Get the second solid.
  const c3d::SolidSPtr & GetSolid2() const { return _faceSet2.GetSolid(); }

  /// \ru Выдать поверхности. \en Get surfaces.
  void            GetFaces1( std::vector<MbFace *> & fas ) const { _faceSet1.GetFaces( fas ); }
  void            GetFaces1( RPArray<MbFace> & fas ) const { _faceSet1.GetFaces( fas ); }
  /// \ru Выдать поверхности. \en Get surfaces.
  void            GetFaces2( std::vector<MbFace *> & fas ) const { _faceSet2.GetFaces( fas ); }
  void            GetFaces2( RPArray<MbFace> & fas ) const { _faceSet2.GetFaces( fas ); }
  /// \ru С каких сторон касаться поверхностей? \en On which sides to touch surfaces?
  void            GetFaceSide1( std::vector<bool> & fSide ) const { _faceSet1.GetFaceSide( fSide ); }
  /// \ru С каких сторон касаться поверхностей? \en On which sides to touch surfaces?
  void            GetFaceSide2( std::vector<bool> & fSide ) const { _faceSet2.GetFaceSide( fSide ); }
  /// \ru Выдать номера опорных граней. \en Get reference face numbers.
  void            GetFaceIndex1( std::vector<MbItemIndex> & fas ) const { _faceSet1.GetFaceIndex( fas ); }
  void            GetFaceIndex2( std::vector<MbItemIndex> & fas ) const { _faceSet2.GetFaceIndex( fas ); }

  /// \ru Получить грань первого набора по индексу. \en Get the face by index in first bundle.
  const MbFace *  GetFace1( size_t i = 0 ) const { return _faceSet1.GetFace( i ); }
  /// \ru Получить грань второго набора по индексу. \en Get the face by index in second bundle.
  const MbFace *  GetFace2( size_t i = 0 ) const { return _faceSet2.GetFace( i ); }
  /// \ru Установить первый набор сопрягаемых граней. \en Get the first set of conjugating faces.
  void            SetFaceSet1( const MbFilletBundle & set ) { _faceSet1 = set; }
  /// \ru Установить второй набор сопрягаемых граней. \en Get the second set of conjugating faces.
  void            SetFaceSet2( const MbFilletBundle & set ) { _faceSet2 = set; }

  /// \ru Установить опорную кривую. \en Set reference curve. 
  void              SetSpine( MbCurve3D & s ) { _spine = &s; }
  /// \ru Выдать опорную кривую. \en Get reference curve. 
  const MbCurve3D * GetSpine() const { return _spine.get(); }
  /// \ru Выдать опорную кривую. \en Get reference curve. 
        MbCurve3D * SetSpine()       { return _spine.get(); }

  /// \ru Получить функцию радиуса скругления для первого набора граней. \en Get the function of fillet radius for the first face set.
  const c3d::FunctionSPtr & GetFunction1() const { return _faceSet1.GetFunction(); }
  /// \ru Получить функцию радиуса скругления для второго набора граней. \en Get the function of fillet radius for the second face set.
  const c3d::FunctionSPtr & GetFunction2() const { return _faceSet2.GetFunction(); }
  /// \ru Установить функцию радиуса скругления для набора граней. \en Set the function of fillet radius for the face set.
  void            SetFunction1( MbFunction & f ) { _faceSet1.SetFunction( f ); }
  /// \ru Установить функцию радиуса скругления для набора граней. \en Set the function of fillet radius for the face set.
  void            SetFunction2( MbFunction & f ) { _faceSet2.SetFunction( f ); }

  /// \ru Получить способы передачи данных при копировании оболочек. \en Get methods of transferring data while copying shells. \~
  MbeCopyMode     GetCopyMode() const { return _copyMode; }
  /// \ru Установить способы передачи данных при копировании оболочек. \en Set methods of transferring data while copying shells. \~
  void            SetCopyMode( MbeCopyMode m ) { _copyMode = m; }
  /// \ru Выдать форму поверхности сопряжения (скругления или фаски). \en Get a blend surface cross-section form (fillet or chamfer). \~
  MbeSmoothForm   GetSubForm() const { return _sform; }
  /// \ru Установить форму поверхности сопряжения (скругления или фаски). \en Set a blend surface cross-section form (fillet or chamfer). \~
  void            SetSubForm(MbeSmoothForm f) { _sform = f; }

  /// \ru Выдать данные управления сечением. \en Get section control data.
  const c3d::FunctionSPtr & GetDescript() const { return _descript; }
  const MbFunction * GetDescription() const { return _descript.get(); }
  MbFunction *    SetDescription() { return _descript.get(); }

  /// \ru Выдать форму обрезки боков поверхности. \en Get the shape of cropping the sides of the surface. \~
  MbeSideShape    GetSideShape() const { return _sideShape; }
  /// \ru Установить форму обрезки боков поверхности. \en Set the shape of cropping the sides of the surface. \~
  void            SetSideShape( MbeSideShape s ) { _sideShape = s; }
  MbeFaceHandling GetFaceHandling() const { return _handling; }
  void            SetFaceHandling( MbeFaceHandling h ) { _handling = h; }
  bool            GetFaceSplit() const { return _faceSplit; }
  /// \ru Установить деление оболочки на грани по сегментам направляющих кривых. \en Set division the shell into faces by segments of guides.
  void            SetFaceSplit( bool s ) { _faceSplit = s; }
  /// \ru Обрезать опорную оболочку на границе по касательной или по нормали к граничному ребру? \en How to cut the support shell on the boundary edge? \~
  bool            GetElongated() const { return _elongated; }
  /// \ru Установить обрезку опорной оболочки на границе к граничному ребру. \en Set the support shell cutting on the boundary edge. \~
  void            SetElongated( bool e ) { _elongated = e; }
  /// \ru Получить флаг продолжения по касательной. \en Get prolong along the tangent flag.
  bool            GetProlong() const { return _prolong; }
  /// \ru Установить флаг продолжения по касательной. \en Set prolong along the tangent flag.
  void            SetProlong( bool p ) { _prolong = p; }
  /// \ru Получить флаг сохранения кромки. \en Get keep cant state flag.
  ThreeStates     GetKeepCant() const { return _keepCant; }
  /// \ru Установить флаг сохранения кромки. \en Set keep cant state flag.
  void            SetKeepCant( ThreeStates ts ) { _keepCant = ts; }
  /// \ru Получить флаг обработки некасательных стыков. \en Get non tangent joints handling flag.
  bool            GetEquable() const { return _equable; }
  /// \ru Установить флаг обработки некасательных стыков. \en Set non tangent joints handling flag.
  void            SetEquable( bool e ) { _equable = e; }
  /// \ru Угловое отклонение при движении по кривым и поверхностям. \en Angular deviation while moving along curves and surfaces. 
  double          GetBuildSag() const { return _buildSag; }
  void            SetBuildSag( double s = Math::deviateSag ) { _buildSag = s; }

  /// \ru Проверять самопересечение построенной поверхности. \en Check the self-intersection of the constructed surface.
  bool            GetCheck() const { return _check; }
  void            SetCheck( bool c ) { _check = c; }
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker & GetNameMaker() const { return _nameMaker; }
  ///< \ru Установить именователь операции. \en Set an object defining names generation in the operation.
  void            SetNameMaker( const MbSNameMaker & name );
  // \ru Выдать версию построения. \en Get the build version. \~
  VERSION         GetVersion() const { return _nameMaker.GetMathVersion(); }
  // \ru Установить версию построения. \en Set the build version. \~
  void            SetVersion( VERSION v );

  /// \ru Возвращает true, если скругляются ровно две несвязные грани. \en Returns true if filleting exactly two disjoint faces.
  bool            IsTwoFaceFillet() const { return _faceSet1.GetFaces().size() == 1 && _faceSet2.GetFaces().size() == 1; }

  /// \ru Преобразовать объект. \en Transform the object. \~
  void            Transform( const MbMatrix3D & matr, MbRegTransform * iReg = nullptr );
  /// \ru Сдвинуть объект. \en Move the object. \~
  void            Move     ( const MbVector3D & to, MbRegTransform * iReg = nullptr );
  /// \ru Повернуть объект. \en Rotate the object. \~
  void            Rotate   ( const MbAxis3D & axis, double angle, MbRegTransform * iReg = nullptr );
  /// \ru Определить, являются ли объекты равными? \en Determine whether an object is equal?
  bool            IsSame( const MbFilletData & other, double accuracy ) const;
  /// \ru Определить, являются ли объекты подобными. \en Determine whether the objects are similar. \~
  bool            IsSimilar( const MbFilletData & other ) const;
  /// \ru Сделать объекты равным. \en Make objects equal. \~
  bool            SetEqual ( const MbFilletData & other );
  /// \ru Выдать базовые объекты. \en Get basis objects.
  void            GetBasisItems( RPArray<MbSpaceItem> & s ) const;

  /// \ru Оператор присваивания без копирования топологических объектов. \en Assignment operator without copying topological objects.
  void            operator = ( const MbFilletData & other );

private:
  /// \ru Конструктор копирования не реализован. \en Copy-constructor not realize.
  MbFilletData( const MbFilletData & other );

public:
  KNOWN_OBJECTS_RW_REF_OPERATORS( MbFilletData ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.

}; // MbFilletData


//------------------------------------------------------------------------------
/** \brief \ru Параметры построения оболочки соединения.
           \en Parameters for creating a join shell. \~
    \details \ru Параметры построения оболочки соединения.
             \en Parameters for creating a join shell. \~
    \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbJoinShellParams
{
protected:
  c3d::ConstEdgesSPtrVector  _edges1;     ///< \ru Первый набор ребер. \en The first set of edges. \~
  c3d::BoolVector            _orients1;   ///< \ru Ориентация рёбер первого набора. \en Orientation of edges from the first set. \~
  c3d::ConstEdgesSPtrVector  _edges2;     ///< \ru Второй набор ребер. \en The second set of edges. \~
  c3d::BoolVector            _orients2;   ///< \ru Ориентация рёбер второго набора. \en Orientation of edges of the second set. \~
  MbMatrix3D                 _matr1;      ///< \ru Матрица преобразования рёбер первого набора. \en Transformation matrix of edges from the first set. \~
  MbMatrix3D                 _matr2;      ///< \ru Матрица преобразования рёбер второго набора. \en Transformation matrix of edges from the second set. \~
  JoinSurfaceValues          _parameters; ///< \ru Параметры построения. \en Parameters of a shell creation. \~
  c3d::SNameMakerSPtr        _names;      ///< \ru Именователь граней. \en An object for naming faces. \~
  bool                       _isPhantom;  ///< \ru Режим создания фантома. \en Create in the phantom mode. \~
  SPtr<const MbSurfaceCurve> _curve1;     ///< \ru Кривая на первой соединяемой поверхности. \en A curve on the first surface to join. \~
  SPtr<const MbSurfaceCurve> _curve2;     ///< \ru Кривая на второй соединяемой поверхности. \en A curve on the second surface to join. \~

public:
  /** \brief \ru Конструктор параметров оболочки соединения.
           \en A shell of join parameter constructor. \~
    \details \ru Конструктор параметров оболочки соединения по двум наборам ребер.
             \en Constructor of parameters for creating a shell of join given two sets of edges. \~
    \param[in] edges1     - \ru Первый набор ребер.
                            \en The first set of edges. \~
    \param[in] orients1   - \ru Ориентации ребер первого набора.
                            \en The edges senses in the first set. \~
    \param[in] edges2     - \ru Второй набор ребер.
                            \en The second set of edges. \~
    \param[in] orients2   - \ru Ориентации ребер второго набора.
                            \en The edges senses in the second set. \~
    \param[in] matr1      - \ru Матрица преобразования первого набора ребер в единую систему координат.
                            \en The matrix of transformation of the first set of edges to the common coordinate system. \~
    \param[in] matr2      - \ru Матрица преобразования второго набора ребер в единую систему координат.
                            \en The matrix of transformation of the second set of edges to the common coordinate system. \~
    \param[in] parameters - \ru Параметры операции.
                            \en The operation parameters. \~
    \param[in] names      - \ru Именователь операции.
                            \en An object for naming the new objects. \~
    \param[in] isPhantom  - \ru Режим создания фантома.
                            \en Create in the phantom mode. \~
  */
  MbJoinShellParams( const c3d::ConstEdgesSPtrVector & edges1, const c3d::BoolVector & orients1,
                     const c3d::ConstEdgesSPtrVector & edges2, const c3d::BoolVector & orients2,
                     const MbMatrix3D & matr1, const MbMatrix3D & matr2, const JoinSurfaceValues & parameters,
                     const MbSNameMaker & names, bool isPhantom = false )
    : _edges1    ( edges1             )
    , _orients1  ( orients1           )
    , _edges2    ( edges2             )
    , _orients2  ( orients2           )
    , _matr1     ( matr1              )
    , _matr2     ( matr2              )
    , _parameters( parameters         )
    , _names     ( &names.Duplicate() )
    , _isPhantom ( isPhantom          )
    , _curve1    (                    )
    , _curve2    (                    )
  {
  }

  /** \brief \ru Конструктор параметров оболочки соединения.
             \en A shell of join parameter constructor. \~
      \details \ru Конструктор параметров оболочки соединения по двум кривым на поверхности.
               \en Constructor of parameters for creating a shell of join from two curves on a surface. \~
      \param[in] curve1     - \ru Первая поверхностная кривая.
                              \en The first curve on a surface. \~
      \param[in] curve2     - \ru Вторая поверхностная кривая.
                              \en The second curve on a surface. \~
      \param[in] parameters - \ru Параметры операции.
                              \en The operation parameters. \~
      \param[in] names      - \ru Именователь операции.
                              \en An object for naming the new objects. \~
  */
  MbJoinShellParams( const SPtr<const MbSurfaceCurve> & curve1, const SPtr<const MbSurfaceCurve> & curve2,
                     const JoinSurfaceValues & parameters, const MbSNameMaker & names )
  : _edges1    (                    )
  , _orients1  (                    )
  , _edges2    (                    )
  , _orients2  (                    )
  , _matr1     (                    )
  , _matr2     (                    )
  , _parameters( parameters         )
  , _names     ( &names.Duplicate() )
  , _isPhantom ( false              )
  , _curve1    ( curve1             )
  , _curve2    ( curve2             )
  {
  }

  /// \ru Деструктор. \en Destructor.
  ~MbJoinShellParams() {}
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker & GetNameMaker() const { return *_names; }
  /// \ru Получить параметры операции. \en Get the operation parameters.
  const JoinSurfaceValues & GetParams() const { return _parameters; }
  /// \ru Установить параметры операции. \en Set the operation parameters.
  JoinSurfaceValues & SetParams() { return _parameters; }
  /// \ru Получить исходный набор первых ребер. \en Get the initial set of the first edges.
  const c3d::ConstEdgesSPtrVector & GetFirstEdges() const { return _edges1; }
  /// \ru Получить исходный набор вторых ребер. \en Get the initial set of the second edges.
  const c3d::ConstEdgesSPtrVector & GetSecondEdges() const { return _edges2; }
  /// \ru Получить исходный набор ориентации первых ребер. \en Get the initial set of the first edges orientations.
  const c3d::BoolVector & GetFirstOrientations() const { return _orients1; }
  /// \ru Получить исходный набор ориентации вторых ребер. \en Get the initial set of the second edges orientations.
  const c3d::BoolVector & GetSecondOrientations() const { return _orients2; }
  /// \ru Получить матрицу преобразования первой группы ребер. \en Get the matrix of transformation of the first group of edges.
  const MbMatrix3D & GetFirstMatrix() const { return _matr1; }
  /// \ru Получить матрицу преобразования второй группы ребер. \en Get the matrix of transformation of the second group of edges.
  const MbMatrix3D & GetSecondMatrix() const { return _matr2; }
  /// \ru Режим фантома? \en Is the phantom mode?
  bool IsPhantom() const { return _isPhantom; }
  /// \ru Получить первую поверхностную кривую. \en Get the first surface curve.
  const SPtr<const MbSurfaceCurve> & GetFirstCurve() const { return _curve1; }
  /// \ru Получить вторую поверхностную кривую. \en Get the second surface curve.
  const SPtr<const MbSurfaceCurve> & GetSecondCurve() const { return _curve2; }

OBVIOUS_PRIVATE_COPY( MbJoinShellParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Результаты построения оболочки соединения.
           \en Results of creating the shell of join. \~
    \details \ru Результаты построения оболочки соединения.
             \en Results of creating the shell of join. \~
    \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbJoinShellResults {
public:
  c3d::SolidSPtr      _solid;   ///< \ru Результирующая оболочка соединения. \en The resultant shell of join.
  std::vector<double> _breaks0; ///< \ru Параметры разбиения первой кривой curve0. \en Splitting parameters of the first curve0 curve.
  std::vector<double> _breaks1; ///< \ru Параметры разбиения первой кривой curve1. \en Splitting parameters of the first curve1 curve.

  /// \ru Конструктор. \en Constructor.
  MbJoinShellResults()
  : _solid()
  , _breaks0()
  , _breaks1()
  {
  }

  /// \ru Деструктор. \en Destructor.
  ~MbJoinShellResults() {}

  /// \ru Установить параметры разбивки. \en Set splitting parameters.
  template<class Container>
  void SetParams( bool first, const Container & ps ) {
    ( first ? _breaks0 : _breaks1 ).assign( ps.begin(), ps.end() );
  }

  /// \ru Копировать результаты в данные для построения поверхности. \en Copy results to data for construction a surface.
  void CopyResultsToValues( JoinSurfaceValues & values ) const {
    SArray<double> breaks0( _breaks0 );
    SArray<double> breaks1( _breaks1 );
    values.SetParams( true,  breaks0 );
    values.SetParams( false, breaks1 );
  }

OBVIOUS_PRIVATE_COPY( MbJoinShellResults )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры построения фантомных поверхностей скругления/фаски.
           \en Parameters for creating phantom surfaces of fillet/chamfer. \~
  \details \ru Параметры построения фантомных поверхностей скругления/фаски.
           \en Parameters for creating phantom surfaces of fillet/chamfer. \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbSmoothPhantomParams
{
protected:
  c3d::ConstSolidSPtr         _initSolid;      ///< \ru Исходное тело. \en The initial solid.
  c3d::EdgesSPtrVector        _initCurves;     ///< \ru Множество ребер для скругления/фаски. \en An array of edges for fillet / chamfer.
  std::vector<MbEdgeFunction> _initFunctions;  ///< \ru Множество ребер и функций изменения радиуса для скругления/фаски. \en An array of edges and radius laws for fillet / chamfer.
public:
  SmoothValues                _params;         ///< \ru Параметры операции скругления. \en The fillet operation parameters.

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по множеству рёбер.
             \en Constructor by an array of edges. \~
    \param[in]  solid   - \ru Исходное тело.
                          \en The initial solid. \~
    \param[in]  edges   - \ru Множество выбранных ребер для скругления/фаски.
                          \en An array of edges for fillet/chamfer. \~
    \param[in]  params  - \ru Параметры операции скругления/фаски.
                          \en Parameters of the fillet/chamfer operation. \~
  */
  MbSmoothPhantomParams( const c3d::ConstSolidSPtr & solid, const c3d::EdgesSPtrVector & edges,
                         const SmoothValues & params )
    : _initSolid     ( solid )
    , _initCurves    ( edges )
    , _params        ( params )
  {}

  /** \brief \ru Конструктор.
           \en Constructor. \~
  \details \ru Конструктор по множеству рёбер и функций изменения радиуса.
           \en Constructor by an array of edges and radius laws. \~
    \param[in]  solid   - \ru Исходное тело.
                          \en The initial solid. \~
  \param[in]  edges     - \ru Множество выбранных ребер и функций изменения радиуса для скругления/фаски.
                          \en An array of edges and radius laws for fillet/chamfer. \~
    \param[in]  params  - \ru Параметры операции скругления/фаски.
                          \en Parameters of the fillet/chamfer operation. \~
*/
  MbSmoothPhantomParams( const c3d::ConstSolidSPtr & solid, const std::vector<MbEdgeFunction> & edges,
                         const SmoothValues & params )
    : _initSolid     ( solid )
    , _initFunctions( edges )
    , _params( params )
  {}

public:
  /// \ru Получить исходное тело. \en Get an initial solid. \~
  const c3d::ConstSolidSPtr & GetSolid() const { return _initSolid; }

  /// \ru Получить набор скругляемых ребер. \en Get the edges to create chamfer on.
  const c3d::EdgesSPtrVector & GetCurves() const { return _initCurves; }

  /// \ru Получить набор скругляемых ребер тела с функциями изменения радиуса. \en Get the edges to fillet together with the radius laws.
  const std::vector<MbEdgeFunction> & GetEdgeFunctions() const { return _initFunctions; }

  OBVIOUS_PRIVATE_COPY( MbSmoothPhantomParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры построения последовательности гладко стыкующихся рёбер.
           \en Parameters for creating a sequence of smoothly connected edges. \~
  \details \ru Параметры построения последовательности гладко стыкующихся рёбер и поверхностей скругления/фаски.
           \en Parameters for creating a sequence of smoothly connected edges and fillet/chamfer surfaces. \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbSmoothSequenceParams : public MbSmoothPhantomParams
{
public:
  bool  _createSurfaces; ///< \ru Флаг создания поверхностей скругления/фаски. \en A flag for creating a fillet / chamfer surfaces.

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по множеству рёбер.
             \en Constructor by an array of edges. \~
  \param[in]  solid -          \ru Исходное тело.
                               \en The initial solid. \~
  \param[in]  edges -          \ru Множество выбранных ребер для скругления/фаски.
                               \en An array of edges for fillet/chamfer. \~
  \param[in]  params         - \ru Параметры операции скругления/фаски.
                               \en Parameters of the fillet/chamfer operation. \~
  \param[in]  createSurfaces - \ru Создавать ли поверхности скругления/фаски для фантома?
                               \en Create a fillet/chamfer surfaces for phantom. \~
  */
  MbSmoothSequenceParams( const c3d::ConstSolidSPtr & solid, const c3d::EdgesSPtrVector & edges,
                          const SmoothValues & params, bool createSurfaces )
    : MbSmoothPhantomParams( solid, edges, params )
    , _createSurfaces( createSurfaces )
  {}

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по множеству рёбер и функций изменения радиуса.
             \en Constructor by an array of edges and radius laws. \~
  \param[in]  solid -          \ru Исходное тело.
                               \en The initial solid. \~
  \param[in]  edges -          \ru Множество выбранных ребер и функций изменения радиуса для скругления/фаски.
                               \en An array of edges and radius laws for fillet/chamfer. \~
  \param[in]  params         - \ru Параметры операции скругления/фаски.
                               \en Parameters of the fillet/chamfer operation. \~
  \param[in]  createSurfaces - \ru Создавать ли поверхности скругления/фаски для фантома?
                               \en Create a fillet/chamfer surfaces for phantom. \~
  */
  MbSmoothSequenceParams( const c3d::ConstSolidSPtr & solid, const std::vector<MbEdgeFunction> & edges,
                          const SmoothValues & params, bool createSurfaces )
    : MbSmoothPhantomParams( solid, edges, params )
    , _createSurfaces( createSurfaces )
  {}


  OBVIOUS_PRIVATE_COPY( MbSmoothSequenceParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Выходные данные построения фантомных поверхностей скругления/фаски.
           \en The output data of creating phantom surfaces of fillet/chamfer. \~
  \details \ru Выходные данные построения фантомных поверхностей скругления/фаски.
           \en The output data of creating phantom surfaces of fillet/chamfer. \~
\ingroup Curve_Building_Parameters
*/
// ---
class MATH_CLASS MbSmoothPhantomResult
{
protected:
  c3d::SurfacesSPtrVector _surfaces;  ///< \ru Поверхности скругления/фаски. \en The fillet / chamfer surfaces.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. \~
  MbSmoothPhantomResult() : _surfaces() {}

public:

  /// \ru Получить массив поверхностей скругления/фаски. \en Get the array of fillet/chamfer surfaces.
  const c3d::SurfacesSPtrVector & GetSurfaces() const { return _surfaces; }

  /// \ru Получить доступ к массиву поверхностей скругления/фаски. \en Get an access to the array of fillet/chamfer surfaces.
  c3d::SurfacesSPtrVector & SetSurfaces() { return _surfaces; }

  /// \ru Переложить поверхности скругления/фаски в контейнер. \en Put the fillet/chamfer surfaces to a container. \~
  template<class PtrVector>
  void DetachSurfaces( PtrVector & surfaces ) {
    surfaces.reserve( _surfaces.size() );
    for ( size_t i = 0, iCount = _surfaces.size(); i < iCount; ++i )
      surfaces.push_back( _surfaces[i].detach() );
  }

  OBVIOUS_PRIVATE_COPY( MbSmoothPhantomResult )
}; // MbSmoothPhantomResult


//------------------------------------------------------------------------------
/** \brief \ru Выходные данные построения последовательности гладко стыкующихся рёбер.
           \en The output data of creating a sequence of smoothly connected edges. \~
  \details \ru Выходные данные построения последовательности гладко стыкующихся рёбер содержат 
               массив последовательностей гладко стыкующихся рёбер и массив поверхностей скругления/фаски.
           \en The output data of creating a sequence of smoothly connected edges contains 
               an array of sequences of smooth mating edges and an array of fillet/chamfer surfaces. \~
\ingroup Curve_Building_Parameters
*/
// ---
class MATH_CLASS MbSmoothSequenceResult : public MbSmoothPhantomResult
{
private:
  std::vector<MbEdgeSequence>  _edgeSequences; ///< \ru Последовательности гладко стыкующихся рёбер. \en Sequences of smooth mating edges.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. \~
  MbSmoothSequenceResult() : MbSmoothPhantomResult(), _edgeSequences() {}

public:

  /// \ru Получить массив последовательностей гладко стыкующихся рёбер. \en Get the array of sequences of smooth mating edges.
  const std::vector<MbEdgeSequence> & GetSequences() const { return _edgeSequences; }

  /// \ru Получить доступ к массиву последовательностей гладко стыкующихся рёбер. \en Get an access to the array of sequences of smooth mating edges.
  std::vector<MbEdgeSequence> & SetSequences() { return _edgeSequences; }

  /// \ru Получить массив последовательностей гладко стыкующихся рёбер. Освобождать созданные объекты должна вызывающая функция.
  /// \en Get the array of sequences of smooth mating edges. Destruction of created objects is a responsibility of the function caller.
  template<class PtrArray>
  void GetSequences( PtrArray & seq ) const {
    seq.clear();
    seq.reserve( _edgeSequences.size() );
    for ( auto & s : _edgeSequences )
      seq.push_back( new MbEdgeSequence( s ) );
  }

  OBVIOUS_PRIVATE_COPY( MbSmoothSequenceResult )
}; // MbSmoothSequenceResult


//------------------------------------------------------------------------------
/** \brief \ru Параметры для построения опорных точек размеров операции скругления/фаски.
           \en Parameters for creating support points of fillet/chamfer operation sizes. \~
  \details \ru Параметры для построения опорных точек размеров операции скругления/фаски.
           \en Parameters for creating support points of fillet/chamfer operation sizes. \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbSmoothPositionDataParams : public MbSmoothPhantomParams
{
private:
  c3d::ConstEdgeSPtr  _dimensionEdge; ///< \ru Ребро, на котором дать опорные точки. \en The edge on which the support points are to be created.
  double              _edgeParam;     ///< \ru Параметр точки на ребре (0 <= edgeParam <= 1). \en The parameter of a point on the edge( 0 <= edgeParam <= 1 ).

public:
  /** \brief \ru Конструктор по множеству выбранных ребер для скругления/фаски.
             \en Constructor by an array of edges for fillet/chamfer. \~
    \details \ru Конструктор по множеству выбранных ребер для скругления/фаски.
             \en Constructor by an array of edges for fillet/chamfer. \~
    \param[in]  solid         - \ru Исходное тело.
                                \en The initial solid. \~
    \param[in] initCurves     - \ru Множество скругляемых ребер тела.
                                \en A set of edges of the solid to fillet. \~
    \param[in] params         - \ru Параметры скругления рёбер.
                                \en Parameters of edges fillet. \~
    \param[in]  dimensionEdge - \ru Ребро, на котором дать опорные точки.
                                \en The edge on which the support points are to be created. \~
    \param[in]  edgeParam     - \ru Параметр точки на ребре (0 <= edgeParam <= 1).
                                \en The parameter of a point on the edge (0 <= edgeParam <= 1). \~
  */
  MbSmoothPositionDataParams( const c3d::ConstSolidSPtr & solid,
                              const c3d::EdgesSPtrVector & initCurves,
                              const SmoothValues & params,
                              const c3d::ConstEdgeSPtr & dimensionEdge = nullptr,
                              double edgeParam = 0.5 )
    : MbSmoothPhantomParams( solid, initCurves, params )
    , _dimensionEdge( dimensionEdge )
    , _edgeParam( edgeParam ) {}

  /** \brief \ru Конструктор по множеству выбранных ребер для скругления/фаски и функций изменения радиуса для скругления/фаски.
             \en Constructor by an array of edges for fillet/chamfer and radius laws for fillet/chamfer. \~
    \details \ru Конструктор по множеству выбранных ребер для скругления/фаски и функций изменения радиуса для скругления/фаски.
             \en Constructor by an array of edges for fillet/chamfer and radius laws for fillet/chamfer. \~
    \param[in] solid          - \ru Исходное тело.
                                \en The initial solid. \~
    \param[in] initFunctions  - \ru Множество скругляемых ребер тела с функциями изменения радиуса.
                                \en An array of edges of the solid to fillet together with the radius laws. \~
    \param[in] params         - \ru Параметры скругления рёбер.
                                \ en Parameters of edges fillet. \~
    \param[in] dimensionEdge  - \ru Ребро, на котором дать опорные точки.
                                \en The edge on which the support points are to be created. \~
    \param[in] edgeParam      - \ru Параметр точки на ребре (0 <= edgeParam <= 1).
                                \en The parameter of a point on the edge (0 <= edgeParam <= 1). \~
  */
  MbSmoothPositionDataParams( const c3d::ConstSolidSPtr & solid,
                              const std::vector<MbEdgeFunction> & initFunctions,
                              const SmoothValues & params,
                              const c3d::ConstEdgeSPtr & dimensionEdge = nullptr,
                              double edgeParam = 0.5 )
    : MbSmoothPhantomParams( solid, initFunctions, params )
    , _dimensionEdge( dimensionEdge )
    , _edgeParam( edgeParam ) {}

public:
  /// \ru Получить ребро, на котором дать опорные точки. \en Get the edge on which the support points are to be created.
  const c3d::ConstEdgeSPtr & GetDimensionEdge() const { return _dimensionEdge; }

  /// \ru Получить параметр точки на ребре (0 <= edgeParam <= 1). \en Get the parameter of a point on the edge( 0 <= edgeParam <= 1 ).
  double GetEdgeParam() const { return _edgeParam; }

  OBVIOUS_PRIVATE_COPY( MbSmoothPositionDataParams )
}; // MbSmoothPositionDataParams


//------------------------------------------------------------------------------
/** \brief \ru Выходные данные построения опорных точек размеров операции скругления/фаски.
           \en The output data of creating support points of fillet/chamfer operation sizes. \~
  \details \ru Выходные данные построения опорных точек размеров операции скругления/фаски.
               Содержат опорные точки размеров операции скругления/фаски.
           \en The output data of creating support points of fillet/chamfer operation sizes.
               Contains support points of the fillet/chamfer operation sizes.\~
  \ingroup Curve_Building_Params
*/
// ---
class MATH_CLASS MbSmoothPositionDataResults
{
private:
  std::vector<MbPositionData *>  _positionData; ///< \ru Опорные точки размеров операции скругления/фаски. \en Support points of the fillet / chamfer operation sizes.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. \~
  MbSmoothPositionDataResults() : _positionData() {}

public:

  /// \ru Получить массив опорных точек размеров операции скругления/фаски. \en Get the array of the support points of the fillet / chamfer operation sizes.
  const std::vector<MbPositionData *> & GetPositionData() const { return _positionData; }

  /// \ru Получить доступ к массиву опорных точек размеров операции скругления/фаски. \en Get an access to the array of the support points of the fillet / chamfer operation sizes.
  std::vector<MbPositionData *> & SetPositionData() { return _positionData; }

  /// \ru Переложить массив опорных точек в указанный массив. \en Put the support points to the specified array.
  template<class PtrArray>
  void GetPositionData( PtrArray & data ) {
    data.reserve( _positionData.size() );
    for ( size_t i = 0; i < _positionData.size(); i++ )
      data.push_back( _positionData[i] );
  }

  OBVIOUS_PRIVATE_COPY( MbSmoothPositionDataResults )
}; // MbSmoothPositionDataResults


//------------------------------------------------------------------------------
/** \brief \ru Класс параметров создания симметричного тела относительно плоскости XY локальной системы координат.
           \en Parameter class by create a symmetric solid relative to a plane. \~
  \details \ru Класс параметров создания симметричного тела относительно плоскости XY локальной системы координат.
           \en Parameter class by create a symmetric solid relative to a plane. \~
  \ingroup Shell_Building_Params
*/
// ---
class MATH_CLASS MbSymmetrySolidParams 
{
private:
  MbPlacement3D _place;     ///< \ru Система координат плоскости симметрии.\en The symmetry plane coordinate system. \~;   
  ThreeStates   _side;      ///< \ru Сохраняемая часть исходной оболочки (+1, 0, -1).\en A part of the source shell to be kept (+1, 0, -1). \~
  MbSNameMaker  _operNames; ///< \ru Именователь.\en An object for naming the new objects. \~ 

public:
  /** \brief \ru Конструктор по параметрам операции.
             \en Constructor by the operation parameters. \~
  \details   \ru Конструктор параметров создания симметричного тела относительно плоскости XY локальной системы координат.\~ 
             \en Constructor parameter class by create a symmetric solid relative to a plane. \n \~ 
  \param[in] place   - \ru Система координат плоскости симметрии.
                       \en The symmetry plane coordinate system. \~
  \param[in] side    - \ru Сохраняемая часть исходной оболочки (+1, 0, -1).
                       \en A part of the source shell to be kept (+1, 0, -1). \~
  \param[in] operNames - \ru Именователь.
                         \en An object for naming the new objects. \~ 
  */
  MbSymmetrySolidParams( const MbPlacement3D & place,
                         const MbSNameMaker &  operNames,
                               ThreeStates     side );
  /// \ru Конструктор копирования. \en Copy constructor. \~
  MbSymmetrySolidParams( const MbSymmetrySolidParams & other );

public:
  ///\ru Получить именователь. \en Obtain an object for naming the new objects.
  const MbSNameMaker & GetNameMaker() const { return _operNames; }
  /// \ru Получить систему координат плоскости симметрии. \en Get the symmetry plane coordinate system. \~
  const MbPlacement3D & GetPlace() const { return _place; }
  /// \ru Получить сохраняемую часть исходной оболочки (+1, 0, -1). \en Get a part of the source shell to be kept (+1, 0, -1). \~
  ThreeStates GetSide() const { return _side; }
  /// \ru Оператор копирования. \en Copy operator. \~
  void operator = ( const MbSymmetrySolidParams & other );
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры создания тела, разрезанного силуэтным контуром.
           \en Parameters for creating a solid cutted by a silhouette contour. \~
  \details \ru Параметры построения оболочки, полученной в результате разрезания тела его силуэтным контуром.
           \en Parameters for creating a solid cutted by its silhouette contour. \~
  \ingroup Shell_Building_Params
*/
// ---
class MATH_CLASS MbCutShellSilhouetteParams : public MbPrecision
{
private:
  c3d::SolidSPtr       _solid;     ///< \ru Исходное тело.\en Initial solid. \~ 
  MbeCopyMode          _sameShell; ///< \ru Способ передачи данных при копировании оболочек. \en Methods of transferring data while copying shells. \~
  MbVector3D           _eye;       ///< \ru Направление взгляда. \en Eye's direction. \~
  const MbSNameMaker & _operNames; ///< \ru Именователь.\en An object for naming the new objects. \~ 

public:
  /** \brief \ru Конструктор по параметрам операции.
             \en Constructor by the operation parameters. \~
  \details   \ru Конструктор параметров создания оболочки, полученной в результате разрезания тела его силуэтным контуром.
             \en Constructor parameter for creating a solid cutted by its silhouette contour. \~
  \param[in] solid     - \ru Исходное тело. 
                         \en The solid. \~
  \param[in] sameShell - \ru Способ передачи данных при копировании оболочек. 
                         \en Methods of transferring data while copying shells. \~
  \param[in] eye       - \ru Направление взгляда.
                         \en Eye's direction. \~
  \param[in] operNames - \ru Именователь с версией.
                         \en An object defining the names with the version. \~
  */
  MbCutShellSilhouetteParams( MbSolid &            solid, 
                              MbeCopyMode          sameShell, 
                              const MbVector3D &   eye,
                              const MbSNameMaker & operNames );
  /// \ru Конструктор копирования. \en Copy constructor. \~
  MbCutShellSilhouetteParams( const MbCutShellSilhouetteParams & other );

  /// \ru Деструктор. \en Destructor. \~
  ~MbCutShellSilhouetteParams() {}

public:
  ///\ru Получить исходное тело.\en Obtain the initial solid. \~ 
  c3d::SolidSPtr     GetSolid() const { return _solid; }
  MbSolid *          DetachSolid() { return _solid.detach(); }
  /// \ru Получить способ передачи данных при копировании оболочек. \en Obtain the method of transferring data while copying shells. \~
  MbeCopyMode        GetCopyMode() const { return _sameShell; }
  void               SetCopyMode( MbeCopyMode mode ) { _sameShell = mode; }
  /// \ru Получить направление взгляда. \en Obtain eye's direction. \~
  const MbVector3D & GetEye() const { return _eye; }
  void               SetEye( const MbVector3D & e ) { _eye = e; }
  ///\ru Получить именователь. \en Obtain an object for naming the new objects. \~
  const MbSNameMaker & GetNameMaker() const { return _operNames; }
  /// \ru Оператор копирования. \en Copy operator. \~
  void operator = ( const MbCutShellSilhouetteParams & other );
};


//------------------------------------------------------------------------------
/** \brief \ru Результаты построения тела, разрезанного силуэтным контуром.
           \en Results of creating a solid cutted by a silhouette contour. \~
    \details \ru Результаты построения оболочки, полученной в результате разрезания тела его силуэтным контуром.
             \en Results of creating a solid cutted by its silhouette contour. \~
    \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbCutShellSilhouetteResult
{
public:
  c3d::SpaceCurvesSPtrVector _outlineCurves; ///< \ru Кривые, входящие в силуэтный контур. \en Curves of the silhouette contour. \~
  c3d::SolidsSPtrVector      _cuttedSolids;  ///< \ru Тела, полученные в результате операции. \en The resultant solids. \~

  /// \ru Конструктор. \en Constructor. \~
  MbCutShellSilhouetteResult()
  : _outlineCurves()
  , _cuttedSolids()
  {}

  /// \ru Деструктор. \en Destructor. \~
  ~MbCutShellSilhouetteResult() {}

  ///< \ru Кривые, входящие в силуэтный контур. \en Curves of the silhouette contour. \~
  c3d::SpaceCurvesSPtrVector & OutlineCurves() { return _outlineCurves; }
  ///< \ru Тела, полученные в результате операции. \en The resultant solids. \~
  c3d::SolidsSPtrVector &      CuttedSolids()  { return _cuttedSolids; }

  /// \ru Переложить построенные кривые в контейнер. \en Put result curves to container. \~
  template<class CurvesVector>
  void DetachResultCurves( CurvesVector & curves ) {
    curves.reserve( _outlineCurves.size() );
    for ( size_t i = 0, iCount = _outlineCurves.size(); i < iCount; ++i ) {
      MbCurve3D * cur = _outlineCurves[i].detach();
      curves.push_back( cur );
    }
  }
  /// \ru Переложить построенные тела в контейнер. \en Put result solids to container. \~
  template<class SolidsVector>
  void DetachResultSolids( SolidsVector & solids ) {
    solids.reserve( _cuttedSolids.size() );
    for ( size_t i = 0, iCount = _cuttedSolids.size(); i < iCount; ++i ) {
      MbSolid * sol = _cuttedSolids[i].detach();
      solids.push_back( sol );
    }
  }

  OBVIOUS_PRIVATE_COPY( MbCutShellSilhouetteResult )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры операции "Уклон".
           \en Draft operation parameters. \~
    \details \ru Параметры операции "Уклон". \n
                 Содержат информацию об уклоняемых гранях, нейтральных ребрах, нейтральной плоскости, угле уклона, базовом направлении, именователе операции. \n
             \en Draft operation parameters. \n 
                 Contains information about drafts faces, neutral edges, neutral plane, drafts angle, basic direction, object defining names generation in the operation. \n \~
    \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbDraftSolidParams: public MbPrecision
{
private:
  c3d::FacesSPtrVector  _faces;          ///< \ru Уклоняемые грани. \en Drafts faces.
  c3d::EdgesSPtrVector  _edges;          ///< \ru Нейтральные ребра. \en Neutral edges.

  MbPlacement3D         _neutralPlane;   ///< \ru Нейтральная плоскость. \en Neutral plane.
  MbVector3D            _dir;            ///< \ru Базовое направление. \en Basic direction.

  SPtr<MbSNameMaker>    _names;          ///< \ru Именователь. \en An object for naming the new objects.
  double                _angle;          ///< \ru Угол уклона. \en Drafts angle.
  MbeFacePropagation    _faceProp;       ///< \ru Признак захвата граней. \en Face propagation.
  bool                  _reverse;        ///< \ru Обратить базовое направление. \en Inverse basic direction.
  bool                  _rebuildFillets; ///< \ru Перестраивать ли скругления. \en Whether to rebuild the fillets.
  bool                  _createPhantom;  ///< \ru Построить фантом. \en Create fantom.

private:
  MbDraftSolidParams(); ///< \ru Конструктор по умолчанию. Запрещён. \en Default constructor. Forbidden.

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по нейтральной плоскости и именователю операции. \n
             \en Constructor by neutral plane and object defining names generation in the operation. \n \~
    \param[in] faces - \ru Уклоняемые грани.
                       \en Drafts faces. \~
    \param[in] neutralPlane - \ru Нейтральная плоскость.
                    \en A neutral plane. \~
    \param[in] names - \ru Именователь операции.
                       \en An object defining names generation in the operation. \~
    \param[in] angle - \ru Угол уклона.
                       \en Draft angle. \~  
    \param[in] faceProp - \ru Признак захвата граней.
                          \en Face propagation. \~
    \param[in] reverse - \ru Обратить базовое направление.
                         \en Inverse basic direction. \~  
  */
  MbDraftSolidParams( const c3d::FacesSPtrVector & faces,
                      const MbPlacement3D &        neutralPlane,
                      const MbSNameMaker &         names,
                            double                 angle,
                            MbeFacePropagation     faceProp,
                            bool                   reverse );
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по нейтральной плоскости и именователю операции. \n
             \en Constructor by neutral plane and object defining names generation in the operation. \n \~
    \param[in] faces - \ru Уклоняемые грани.
                       \en Drafts faces. \~
    \param[in] neutralPlane - \ru Нейтральная плоскость.
                    \en A neutral plane. \~
    \param[in] names - \ru Именователь операции.
                       \en An object defining names generation in the operation. \~
    \param[in] angle - \ru Угол уклона.
                       \en Draft angle. \~  
    \param[in] faceProp - \ru Признак захвата граней.
                          \en Face propagation. \~
    \param[in] reverse - \ru Обратить базовое направление.
                         \en Inverse basic direction. \~  
    \param[in] rebuildFillets - \ru Перестраивать ли прилежащие скругления.
                                \en Whether to rebuild the adjacent fillets. \~
  */
  MbDraftSolidParams( const c3d::FacesSPtrVector & faces,
                      const MbPlacement3D &        neutralPlane,
                      const MbSNameMaker &         names,
                            double                 angle,
                            MbeFacePropagation     faceProp,
                            bool                   reverse,
                            bool                   rebuildFillets );
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по нейтральной плоскости и именователю. \n
             \en Constructor by neutral plane and object defining names generation in the operation. \n \~
    \param[in] faces - \ru Уклоняемые грани.
                       \en Drafts faces. \~
    \param[in] neutralPlane - \ru Нейтральная плоскость.
                              \en A neutral plane. \~
    \param[in] names - \ru Именователь операции.
                       \en An object defining names generation in the operation. \~
    \param[in] angle - \ru Угол уклона.
                       \en Draft angle. \~
    \param[in] faceProp - \ru Признак захвата граней.
                          \en Face propagation. \~     
    \param[in] reverse - \ru Обратить базовое направление.
                         \en Inverse basic direction. \~  
  */
  MbDraftSolidParams( const RPArray<MbFace> &  faces,
                      const MbPlacement3D &    neutralPlane,
                      const MbSNameMaker &     names,
                            double             angle,
                            MbeFacePropagation faceProp,
                            bool               reverse );
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по набору ребер и именователю. \n
             \en Constructor by edges and object defining names generation in the operation. \n \~
    \param[in] faces - \ru Уклоняемые грани.
                       \en Drafts faces. \~
    \param[in] edges - \ru Нейтральные ребра.
                       \en A neutral edges. \~
    \param[in] dir - \ru Базовое направление.
                    \en A basic direction. \~
    \param[in] names - \ru Именователь.
                       \en An object defining names generation in the operation. \~
    \param[in] angle - \ru Угол уклона.
                       \en Draft angle. \~
    \param[in] faceProp - \ru Признак захвата граней.
                          \en Face propagation. \~
    \param[in] reverse - \ru Обратить базовое направление.
                         \en Inverse basic direction. \~
  */
  MbDraftSolidParams( const c3d::FacesSPtrVector & faces,
                      const c3d::EdgesSPtrVector & edges,   
                      const MbVector3D &           dir,
                      const MbSNameMaker &         names, 
                            double                 angle,       
                            MbeFacePropagation     faceProp,
                            bool                   reverse );
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по набору ребер и именователю. \n
             \en Constructor by edges and object defining names generation in the operation. \n \~
    \param[in] faces - \ru Уклоняемые грани.
                       \en Drafts faces. \~
    \param[in] edges - \ru Нейтральные ребра.
                       \en A neutral edges. \~
    \param[in] dir - \ru Базовое направление.
                    \en A basic direction. \~
    \param[in] names - \ru Именователь.
                       \en An object defining names generation in the operation. \~
    \param[in] angle - \ru Угол уклона.
                       \en Draft angle. \~
    \param[in] faceProp - \ru Признак захвата граней.
                          \en Face propagation. \~
    \param[in] reverse - \ru Обратить базовое направление.
                         \en Inverse basic direction. \~
    \param[in] rebuildFillets - \ru Перестраивать ли прилежащие скругления.
                                \en Whether to rebuild the adjacent fillets. \~
  */
  MbDraftSolidParams( const c3d::FacesSPtrVector & faces,
                      const c3d::EdgesSPtrVector & edges,   
                      const MbVector3D &           dir,
                      const MbSNameMaker &         names, 
                            double                 angle,       
                            MbeFacePropagation     faceProp,
                            bool                   reverse,
                            bool                   rebuildFillets );
  /// \ru Конструктор копирования. \en Copy constructor.
  MbDraftSolidParams( const MbDraftSolidParams & other );
  /// \ru Получить набор граней. \en Get the faces.
  void GetFaces( c3d::FacesVector & faces ) const;
  /// \ru Получить набор ребер. \en Get the edges.
  void GetEdges( c3d::EdgesVector & edges ) const;
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker & GetNameMaker()  const { return *_names; }
  /// \ru Получить базовое направление. \en Get basic direction.
  const MbVector3D & GetDir() const { return _dir; }
  /// \ru Получить нейтральную плоскость. \enGet neutral plane.
  const MbPlacement3D & GetPlace() const { return _neutralPlane; }
  /// \ru Получить угол уклона. \en Drafts angle.
  double GetAngle() const { return _angle; }
  /// \ru Получить признак захвата граней. \en Get face propagation.
  MbeFacePropagation GetFacePropagetion() const { return _faceProp; }
  /// \ru Получить обратное базовое направление. \en Get inverse basic direction.
  bool GetReverse() const { return _reverse; }
  /// \ru Перестраивать ли прилежащие скругления. \en Whether to rebuild the adjacent fillets.
  bool DoRebuildFillets() const { return _rebuildFillets; }
  /// \ru Создавать ли фантом. \en Whether to create fantom.
  bool IsPhantom() const { return _createPhantom; }
  /// \ru Установить флаг создания фантома. \en Set flag making of fantom.
  void SetFantom( bool createPhantom ) { _createPhantom = createPhantom; }
  /// \ru Оператор присваивания. \en Assignment operator.
  const MbDraftSolidParams & operator = ( const MbDraftSolidParams & );

private:
  // \ru Поменять на противоположный угол уклона и флаг обращения базового направления. \en Invert drafts angle and basic directions flag.
  void Invert();  
  // \ru Поменять на противоположный угол уклона и флаг обращения базового направления c проверкой версии. \en Invert drafts angle and basic directions flag with version check.
  void InvertWithCheckVersion();
};


//------------------------------------------------------------------------------
/** \brief \ru Результаты построения тела уклона.
           \en Results of creating a draft's solid. \~
  \details \ru Результаты построения тела уклона содержат тело уклона и атрибут хот-точки уклона.
           \en Results of creating a draft's solid contains draft's solid and hot-point's attribute.  \~
\ingroup ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbDraftSolidResults : public MbOperationResults
{
private:
  c3d::SolidSPtr  _resultSolid; ///< \ru Результирующее тело. \en The resulting solid.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. \~
  MbDraftSolidResults() 
    : MbOperationResults()
    , _resultSolid      () 
  {}
  ~MbDraftSolidResults() {}
  /// \ru Функция инициализации. \en Initialization function.
  void Init( const c3d::SolidSPtr & resultSolid ) {  
    _resultSolid = resultSolid;
  }
  /// \ru Получить результирующее тело. \en Get resulting solid.
  c3d::SolidSPtr GetResultSolid() const { return _resultSolid; }
  /// \ru Получить результирующее тело. \en Get resulting solid.
  c3d::SolidSPtr & SetResultSolid() { return _resultSolid; }
  /** \brief \ru Получить хот-точку операции.
             \en Get operation hot point. \~
    \param[in]  pnt -  \ru Хот-точка угла наклона.
                       \en Hot point of draft angle. \~
    \param[in]  axis - \ru Ось вращения.
                       \en Axis of rotation. \~
    \return \ru Возвращает "true" в случае успеха.
            \en Returns "true" in case of success. \~
  */
  bool           GetDraftHotPoint( MbCartPoint3D & pnt, MbAxis3D & axis ) const;
  OBVIOUS_PRIVATE_COPY( MbDraftSolidResults )
}; // MbDraftSolidResults


//------------------------------------------------------------------------------
/** \brief \ru Результаты построения оболочки с переменным коническим сечением.
           \en Results of construction of a shell on swept mutable section surface. \~
  \details \ru Результаты построения оболочки с переменным коническим сечением.
           \en Results of construction of a shell on swept mutable section surface.  \~
  \ingroup ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbSectionResults : public MbOperationResults {
private:
  c3d::SolidSPtr _solid;
public:
  MbSectionResults() 
    : MbOperationResults()
    , _solid            ()
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbSectionResults( const MbSectionResults & other )
    : MbOperationResults( other        )
    , _solid            ( other._solid )
  {}
  ~MbSectionResults() {}
  /// \ru Оператор присваивания. \en Assignment operator.
  MbSectionResults & operator = ( const MbSectionResults & other ) {
    MbOperationResults::operator =( static_cast<const MbOperationResults &>(other) );
    _solid = other._solid;
    return *this;
  }
  /// \ru Получить результирующее тело. \en Get resulting solid.
  c3d::SolidSPtr GetResultSolid() const { return _solid; }
  /// \ru Получить результирующее тело. \en Get resulting solid.
  c3d::SolidSPtr & SetResultSolid() { return _solid; }
  /** \brief \ru Выдать хот-точки для заданного расположения сечения.
             \en Get hot points for given layout. \~
    \param[in] id  - \ru Идентификатор положения сечения: 
                         0 - начальный край сечения поверхности,
                         1 - конечный край сечения поверхности,
                         2 - середина сечения поверхности.
                     \en Hot-points layout identifier:
                         0 - initial cross-section of the surface,
                         1 - end cross-section of the surface,
                         2 - middle cross-section of the surface. \~
    \param[out] p1 - \ru Точка пересечения первой направляющей кривой и плоскости XY. 
                     \en The intersection point of the first guide curve and XY plane. \~
    \param[out] v1 - \ru Вектор направления от первой направляющей к вершине охватывающего треугольника. 
                     \en The direction vector from the first guide point to the vertex of the enclosing triangle. \~
    \param[out] p2 - \ru Точка пересечения второй направляющей кривой и плоскости XY.
                     \en The intersection point of the second guide curve and XY plane. \~
    \param[out] v2 - \ru Вектор направления от второй направляющей к вершине охватывающего треугольника.
                     \en The direction vector from the second guide point to the vertex of the enclosing triangle. \~
    \param[out] p3 - \ru Точка пересечения медианы из вершины охватывающего треугольника и поперечного сечения. 
                     \en The intersection point of median from vertex the enclosing triangle and cross section curve. \~
    \param[out] v3 - \ru Вектор направления медианы из вершины охватывающего треугольника.
                     \en The direction vector of median from vertex of the enclosing triangle. \~
    \param[out] pl - \ru Плоскость охватывающего треугольника (XY) в сечении c центром в точке пересечения прямых, ортогональных векторам dir1 и dir2.
                     \en The plane of the enclosing triangle (XY) at the section with center at intersection of lines which are orthogonal to dir1 and dir2.\~
    \param[out] a  - \ru Точка пересечения вершинной кривой и плоскости XY - является вершиной охватывающего треугольника (может быть в бесконечности).
                     \en The intersection point of the apex curve ans XY plane is the vertex of the enclosing triangle (may be on infinity). \~
    \return \ru true, в случае успеха.
            \en true if success. \~
  */
  bool GetHotPointsLayout( size_t id, 
                           MbCartPoint3D & p1, MbVector3D & v1,
                           MbCartPoint3D & p2, MbVector3D & v2,
                           MbCartPoint3D & p3, MbVector3D & v3,
                           MbPlacement3D & pl, MbCartPoint3D & a ) const;
};


//------------------------------------------------------------------------------
/** \brief \ru Входные параметры операции "Балочный элемент".
           \en Input parameters of "Beam element" operation. \~
  \details \ru Входные параметры операции "Балочный элемент".
           \en Input parameters of "Beam element" operation. \~
    \ingroup Shell_Building_Parameters
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbBeamElementParams : public MbPrecision
{
private:
  c3d::SNameMakerSPtr _names; ///< \ru Именователь. \en An object for naming the new objects.
  double              _sectionRatio;  ///< \ru Определяет положение плоского сечения на балочной кривой. Задается значением между 0 и 1, где 0 - в начале кривой, 1 - в конце. \en Determines the place of a planar section on the beam curve. Its value is between 0 and 1 where 0 is a begin point of the curve and 1 is an end of the curve.

private:
  ///< \ru Конструктор по умолчанию (не реализован). \en Default constructor (not implemented).
  MbBeamElementParams();

public:
  ///< \ru Конструктор. \en Constructor.
  MbBeamElementParams( const MbSNameMaker & names );
  
  ///< \ru Конструктор копирования. \en Copy constructor.
  MbBeamElementParams( const MbBeamElementParams & other ); 

  ///< \ru Деструктор. \en Destructor.
  virtual ~MbBeamElementParams();

public:
  /** \brief \ru Задать положение плоского сечения на балочной кривой.
             \en Set the place of the planar section on the beam curve. \~
    \details \ru Задать положение плоского сечения на балочной кривой.
             \en Set the place of the planar section on the beam curve. \~
    \param[in] ratio - \ru Значение отношения, в котором эта точка делит кривую. В диапазоне от 0 до 1. ratio=0 соответствует началу кривой, ratio=1 - концу.
                       \en A value of the ratio, which this point divides the curve in. In the range 0...1. ratio=0 means beginning of the curve , ratio=1 means end of the curve. \~
  */
  void SetSectionRatio( double ratio );

  /** \brief \ru Получить информацию о положении сечения.
             \en Get the information about section placement. \~
    \details \ru Получить информацию о положении сечения. Т. е. значение отношения, в котором эта точка делит кривую. В диапазоне от 0 до 1. ratio=0 соответствует началу кривой, ratio=1 - концу. 
             \en Get the information about section placement. I. e. a value of the ratio, which this point divides the curve in. In the range 0...1. ratio=0 means beginning of the curve , ratio=1 means end of the curve.\~
    \return \ru Значение отношения, в котором эта точка делит кривую.
            \en A value of the ratio, which this point divides the curve in. \~
  */
  double GetSectionRatio() const { return _sectionRatio; };

  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation. \~
  const MbSNameMaker & GetNameMaker() const { return *_names; }

KNOWN_OBJECTS_RW_REF_OPERATORS( MbBeamElementParams ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//------------------------------------------------------------------------------
/** \brief \ru Выходные параметры операции "Балочный элемент".
           \en Output parameters of "Beam element" operation. \~
  \details \ru Выходные параметры операции "Балочный элемент".
           \en Output parameters of "Beam element" operation. \~
    \ingroup Shell_Building_Parameters
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbBeamElementResults : public MbOperationResults
{
private:
  c3d::WireFrameSPtr _resWireFrame;   ///< \ru Результирующая балочная кривая. \en Resulting beam curve.

public:
  ///< \ru Конструктор. \en Constructor.
  MbBeamElementResults();

  ///< \ru Деструктор. \en Destructor.
  virtual ~MbBeamElementResults();

public:
  /** \brief \ru Переопределить поля класса в соответствии с заданными параметрами.
             \en Reinitialize class members according to the input parameters. \~
    \details \ru Переопределить поля класса в соответствии с заданными параметрами.
             \en Reinitialize class members according to the input parameters. \~
    \param[in] edges - \ru Ребра балочных кривых.
                       \en The edges of the beam curves. \~
    \param[in] creator - \ru Строитель операции "Балочный элемент".
                         \en The creator of the operation "Beam element". \~
  */
  void Init( const c3d::WireEdgesSPtrVector & edges, const SPtr<MbBeamCreator> & creator );

  /// \ru Получить результирующий каркас. \en Get the wire frame.
  const c3d::WireFrameSPtr & GetWireFrame() const { return _resWireFrame; };

  /// \ru Получить количество балочных элементов. \en Get count of beam elements.
  size_t GetElementsCount() const;

  /** \brief \ru Получить балочный элемент с заданным индексом.
             \en Get the beam element with the given index. \~
    \details \ru Получить балочный элемент с заданным индексом.
             \en Get the beam element with the given index. \~
    \param[in] idx - \ru Индекс элемента.
                     \en The index of the element. \~
    \param[out] beamCurve - \ru Балочная кривая.
                            \en The beam curve. \~
    \param[out] section - \ru Плоское сечение.
                          \en The planar section. \~
    \return \ru true, в случае успеха.
            \en true if success. \~
  */
  bool GetBeamElement( size_t idx, c3d::SpaceCurveSPtr & beamCurve, SPtr<MbCurveBoundedSurface> & section ) const;
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры построения оболочки по поверхности.
           \en Parameters for creating the shell by a surface. \~
  \details \ru Параметры построения оболочки по поверхности.
           \en Parameters for creating the shell by a surface. \~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS MbSurfaceShellParams
{
public:
  const MbSurface &     _surface;     ///< \ru Передаваемая поверхность. \en The surface.
  SPtr<MbSNameMaker>    _operNames;   ///< \ru Именователь операции. \en An object defining names generation in the operation.
  bool                  _sameSurface; ///< \ru Использовать копию поверхности, если параметр равен false. \en Use a copy of the surface if the flag is false.
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbSurfaceShellParams( const MbSurface & surface, const MbSNameMaker & operNames, bool sameSurface )
    : _surface     ( surface )
    , _operNames   ( &operNames.Duplicate() )
    , _sameSurface ( sameSurface ) {};
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker &  GetNameMaker() const { return *_operNames; }
  /// \ru Получить переданную поверхность. \en Get the surface.
  const MbSurface & GetSurface() const { return _surface; }
  /// \ru Использовать поверхность или ее копию. \en. Use the surface or a copy of it.
  bool  IsSameSurface() const { return _sameSurface; }

OBVIOUS_PRIVATE_COPY( MbSurfaceShellParams )
};


#endif // __OP_SHELL_PARAMETERS_H
