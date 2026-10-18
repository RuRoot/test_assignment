////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Параметры операций над кривыми.
         \en Parameters of operations on the curves. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __OP_CURVE_PARAMETERS_H
#define __OP_CURVE_PARAMETERS_H

#include <math_version.h>
#include <math_define.h>
#include <templ_dptr.h>
#include <templ_sptr.h>
#include <mb_vector3d.h>
#include <mb_axis3d.h>
#include <curve.h>
#include <curve3d.h>
#include <topology.h>
#include <wire_frame.h>
#include <op_binding_data.h>
#include <mb_smooth_nurbs_fit_curve.h>

class  MbSurfaceCurve;
class  MbCurveExtensionParameters;

//------------------------------------------------------------------------------
/** \brief \ru Базовый класс для параметров создания кривой пересечения поверхностей.
           \en The base class for parameters of a surface intersection curve creation.\n \~
  \details \ru Базовый класс для параметров создания кривой пересечения поверхностей.
           \en The base class for parameters of a surface intersection curve creation.\n \~
  \ingroup Curve3D_Building_Parameters
*/ // ---
class MATH_CLASS MbIntCurveValues
{
protected:
  /// \ru Конструктор без параметров закрыт. \en The default constructor is protected.
  explicit MbIntCurveValues() {};
public:
  virtual ~MbIntCurveValues() {};
  OBVIOUS_PRIVATE_COPY( MbIntCurveValues )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры создания кривой пересечения поверхностей по начальной точке.
           \en Parameters of a surface intersection curve creation by the initial point. \~
  \details \ru Параметры создания кривой пересечения поверхностей по начальной точке и вектору направления. \n
           \en Parameters of a surface intersection curve creation by the initial point and the direction vector. \n \~
  \ingroup Curve3D_Building_Parameters
*/ // ---
class MATH_CLASS MbIntCurveOnePointValues : public MbIntCurveValues
{
protected:
  MbRect2D    _surface1Domain; ///< \ru Область ограничения кривой на первой поверхности. \en The curve bound area on the first surface. \~
  MbRect2D    _surface2Domain; ///< \ru Область ограничения кривой на второй поверхности.\en The curve bound area on the second surface. \~
  MbCartPoint _uvInitOnSurface1; ///< \ru Начальная точка на первой поверхности. \en The curve starting point on the first surface. \~
  MbCartPoint _uvInitOnSurface2; ///< \ru Начальная точка на второй поверхности. \en The curve starting point on the second surface. \~
  MbVector3D  _initDirection; ///< \ru Начальное направление создания линии пересечения. \en The initial direction for the intersection curve creation. \~

private:
  /// \ru Конструктор без параметров запрещён. \en The default constructor is forbidden.
  explicit MbIntCurveOnePointValues();

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
  \details \ru Конструктор по параметрам.
           \en Constructor by parameters. \~
  \param[in] surface1Domain - \ru Область ограничения кривой на первой поверхности.
                              \en The curve bound area on the first surface. \~
  \param[in] surface2Domain - \ru Область ограничения кривой на второй поверхности.
                              \en The curve bound area on the second surface. \~
  \param[in] uvInitOnSurface1 - \ru Начальная точка на первой поверхности.
                                \en The curve starting point on the first surface. \~
  \param[in] uvInitOnSurface2 - \ru Начальная точка на второй поверхности.
                                \en The curve starting point on the second surface. \~
  \param[in] initDirection - \ru Начальное направление создания линии пересечения.
                             \en The initial direction for the intersection curve creation. \~
    */
  MbIntCurveOnePointValues( const MbRect2D &    surface1Domain,
                            const MbRect2D &    surface2Domain,
                            const MbCartPoint & uvInitOnSurface1,
                            const MbCartPoint & uvInitOnSurface2,
                            const MbVector3D &  initDirection )
    : _surface1Domain   ( surface1Domain   )
    , _surface2Domain   ( surface2Domain   )
    , _uvInitOnSurface1 ( uvInitOnSurface1 )
    , _uvInitOnSurface2 ( uvInitOnSurface2 )
    , _initDirection    ( initDirection    )
  {}

  /// \ru Получить область ограничения кривой на первой поверхности. \en Get the curve bound area on the first surface. 
  const MbRect2D & GetSurf1Rect() const { return _surface1Domain; }
  /// \ru Получить область ограничения кривой на второй поверхности. \en Get the curve bound area on the second surface. 
  const MbRect2D & GetSurf2Rect() const { return _surface2Domain; }
  /// \ru Получить начальную точку кривой на первой поверхности. \en Get the curve starting point on the first surface. 
  const MbCartPoint & GetSurf1InitPoint() const { return _uvInitOnSurface1; }
  /// \ru Получить начальную точку кривой на второй поверхности. \en Get the curve starting point on the second surface. 
  const MbCartPoint & GetSurf2InitPoint() const { return _uvInitOnSurface2; }
  /// \ru Получить начальное направление создания линии пересечения. \en Get the initial direction for the intersection curve creation
  const MbVector3D & GetDirection() const { return _initDirection; }

OBVIOUS_PRIVATE_COPY( MbIntCurveOnePointValues )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры создания кривой пересечения поверхностей по начальной и конечной точкам.
           \en Parameters of a surface intersection curve creation by the initial and final points. \~
  \details \ru Параметры создания кривой пересечения поверхностей по начальной точке и конечной точкам. \n
           \en Parameters of a surface intersection curve creation by the initial and final points. \n \~
  \ingroup Curve3D_Building_Parameters
*/ // ---
class MATH_CLASS MbIntCurveLimitPointsValues : public MbIntCurveValues
{
protected:
  MbCartPoint _uv1beg; ///< \ru Начальная точка на первой поверхности. \en The start point on the first surface. \~
  MbCartPoint _uv1end; ///< \ru Конечная точка на первой поверхности. \en The end point on the first surface. \~
  MbCartPoint _uv2beg; ///< \ru Начальная точка на второй поверхности. \en The start point on the second surface. \~
  MbCartPoint _uv2end; ///< \ru Конечная точка на второй поверхности. \en The end point on the second surface. \~
  MbVector3D _direction; ///< \ru Начальное направление создания линии пересечения. \en The start direction for intersection curve creation. \~
  const MbCurve3D * _guideCurve; ///< \ru Направляющая кривая, приближенно описывающая искомую кривую. \en The guide curve that approximates the desired curve. \~
  bool _ext1; ///< \ru На расширенной первой поверхности. \en Whether to create on the extended surface. \~
  bool _ext2; ///< \ru На расширенной второй поверхности. \en Whether to create on the extended second surface. \~
  bool _useRedetermination; ///< \ru Флаг, определяющий нужно ли уточнять шаг построения следующей точки по сравнению с функцией DeviationStep. \en The flag that determines whether it is necessary to specify the next point build step as compared to the DeviationStep function. \~
  bool _checkPoles; ///< \ru Флаг необходимости проверки и корректировки полюсных точек. \en The flag that determines whether it is necessary to check and correct pole points. \~

private:
  /// \ru Конструктор без параметров запрещён. \en The default constructor is forbidden.
  explicit MbIntCurveLimitPointsValues();

public:
/** \brief \ru Конструктор.
           \en Constructor. \~
  \details \ru Конструктор по параметрам.
           \en Constructor by parameters. \~
  \param[in] uv1beg - \ru Начальная точка на первой поверхности.
                      \en The start point on the first surface. \~
  \param[in] uv1end - \ru Конечная точка на первой поверхности.
                      \en The end point on the first surface. \~
  \param[in] ext1 - \ru На расширенной первой поверхности.
                    \en Whether to create on the extended surface. \~
  \param[in] uv2beg - \ru Начальная точка на второй поверхности.
                      \en The start point on the second surface. \~
  \param[in] uv2end - \ru Конечная точка на второй поверхности.
                      \en The end point on the second surface. \~
  \param[in] ext2 - \ru На расширенной второй поверхности.
                    \en Whether to create on the extended second surface. \~
  \param[in] dir - \ru Начальное направление создания линии пересечения.
                   \en The start direction for intersection curve creation. \~
*/
  MbIntCurveLimitPointsValues( const MbCartPoint & uv1beg,
                               const MbCartPoint & uv1end,
                                     bool          ext1,
                               const MbCartPoint & uv2beg,
                               const MbCartPoint & uv2end,
                                     bool          ext2, 
                               const MbVector3D &  dir )
    : _uv1beg ( uv1beg )
    , _uv1end ( uv1end )
    , _uv2beg ( uv2beg )
    , _uv2end ( uv2end )
    , _direction ( dir )
    , _guideCurve ( nullptr )
    , _ext1   ( ext1   )
    , _ext2   ( ext2   )
    , _useRedetermination ( false )
    , _checkPoles ( false )
  {}

/** \brief \ru Конструктор.
           \en Constructor. \~
  \details \ru Конструктор по параметрам.
           \en Constructor by parameters. \~
  \param[in] uv1beg - \ru Начальная точка на первой поверхности.
                      \en The start point on the first surface. \~
  \param[in] uv1end - \ru Конечная точка на первой поверхности.
                      \en The end point on the first surface. \~
  \param[in] ext1 - \ru На расширенной первой поверхности.
                    \en Whether to create on extended surface. \~
  \param[in] uv2beg - \ru Начальная точка на второй поверхности.
                      \en The start point on the second surface. \~
  \param[in] uv2end - \ru Конечная точка на второй поверхности.
                      \en The end point on the second surface. \~
  \param[in] ext2 - \ru На расширенной второй поверхности.
                    \en Whether to create on the extended second surface. \~
  \param[in] guideCurve - \ru Направляющая кривая, приближенно описывающая искомую кривую.
                          \en The guide curve that approximates the desired curve. \~
  \param[in] useRedetermination - \ru Флаг, определяющий нужно ли уточнять шаг построения следующей точки по сравнению с функцией DeviationStep.
                                  \en The flag that determines whether it is necessary to specify the next point build step as compared to the DeviationStep function. \~
  \param[in] checkPoles - \ru Флаг необходимости проверки и корректировки полюсных точек.
                          \en The flag that determines whether it is necessary to check and correct pole points. \~ 
*/
  MbIntCurveLimitPointsValues( const MbCartPoint & uv1beg,
                               const MbCartPoint & uv1end,
                                     bool          ext1,
                               const MbCartPoint & uv2beg,
                               const MbCartPoint & uv2end,
                                     bool          ext2, 
                               const MbCurve3D *   guideCurve,
                                     bool          useRedetermination,
                                     bool          checkPoles )
    : _uv1beg ( uv1beg )
    , _uv1end ( uv1end )
    , _uv2beg ( uv2beg )
    , _uv2end ( uv2end )
    , _guideCurve ( guideCurve )
    , _ext1   ( ext1   )
    , _ext2   ( ext2   )
    , _useRedetermination ( useRedetermination )
    , _checkPoles ( checkPoles )
  {}

  /// \ru Получить начало кривой на первой поверхности. \en Get the curve beginning on the first surface. 
  const MbCartPoint & GetSurf1Beg() const { return _uv1beg; }
  /// \ru Получить конец кривой на первой поверхности. \en Get the curve end on the first surface. 
  const MbCartPoint & GetSurf1End() const { return _uv1end; }
  /// \ru Получить начало кривой на второй поверхности. \en Get the curve beginning on the second surface. 
  const MbCartPoint & GetSurf2Beg() const { return _uv2beg; }
  /// \ru Получить конец кривой на второй поверхности. \en Get the curve end on the second surface. 
  const MbCartPoint & GetSurf2End() const { return _uv2end; }

  /// \ru Строить ли кривую на расширении первой поверхности. \en Whether to build the curve on the first surface extension.
  bool IfSurf1Ext () const { return _ext1; }
  /// \ru Строить ли кривую на расширении второй поверхности. \en Whether to build the curve on the second surface extension.
  bool IfSurf2Ext () const { return _ext2; }
  /// \ru Уточнять ли шаг построения следующей точки по сравнению с функцией DeviationStep. \en Whether to improve precision of the next point building step comparing to the DeviationStep function. \~
  bool UseRedetermination () const { return _useRedetermination; }
  /// \ru Нужно ли проверять и корректировать полюсные точки. \en Whether to check abd correct the pole points.
  bool CheckPoles () const { return _checkPoles; }

  /// \ru Получить начальное направление создания линии пересечения. \en Get the initial direction for the intersection curve creation
  const MbVector3D & GetDirection() const { return _direction; }
  /// \ru Получить направляющую кривую, приближенно описывающую искомую кривую. \en Get the guide curve that approximates the desired curve.
  const MbCurve3D * GetGuideCurve() const { return _guideCurve; }

OBVIOUS_PRIVATE_COPY( MbIntCurveLimitPointsValues )
};


//------------------------------------------------------------------------------
/** \brief \ru Тип параметров создания кривой пересечения поверхностей.
           \en The type of parameters for surface intersection curve creation. \~
  \details \ru Тип параметров создания кривой пересечения поверхностей.
           \en The type of parameters for surface intersection curve creation. \~
*/ // ---
enum MbeIntCurveBuildType {
    ict_General = 0,       ///< \ru Общий тип. \en General type. 
    ict_OnePoint = 1,      ///< \ru Пересечение по точке и направлению. \en Intersection by a point and a direction. 
    ict_TwoPoints = 2,     ///< \ru Пересечение по начальной и конечной точкам. \en Intersection by the initial and final points. 
    ict_TwoPointsGuide = 3 ///< \ru Пересечение по начальной и конечной точкам и направлению. \en Intersection by the initial and final points and a direction. 
  };


//------------------------------------------------------------------------------
/** \brief \ru Параметры кривой пересечения поверхностей.
           \en Parameters of a surface intersection curve. \~
  \details \ru Параметры кривой пересечения поверхностей. Варианты работы см. в #MbeIntCurveBuildType. \n
           \en Parameters of a surface intersection curve. Work cases are in #MbeIntCurveBuildType. \n \~
  \ingroup Curve3D_Building_Parameters
*/ // ---
class MATH_CLASS MbIntCurveParams : public MbPrecision {
public:
  bool mergeCurves;  ///< \ru Объединять кривые, разрезанные швом. \en Merge curves cut by a surface seam.
  bool cutCurves;    ///< \ru Разрезать кривые в точках пересечения. \en Cut curves at intersection points.

protected:
  MbeIntCurveBuildType   _type;    ///< \ru Тип параметров создания кривой пересечения поверхностей. \en The type of parameters for surface intersection curve creation.
  SPtr<MbSNameMaker>     _snMaker; ///< \ru Именователь с версией операции. \en Names maker with operation version.
  DPtr<MbIntCurveValues> _values;  ///< \ru Параметры создания кривой пересечения поверхностей. \en Parameters for surface intersection curve creation.

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \param[in] _mergeCurves - \ru Объединять кривые, разрезанные швом.
                              \en Merge curves cut by a surface seam. \~
    \param[in] _cutCurves   - \ru Разрезать кривые в точках пересечения.
                              \en Cut curves at intersection points. \~
    \param[in] _snMaker     - \ru Именователь с версией операции.
                              \en Names maker with operation version. \~
  */
  MbIntCurveParams( const MbSNameMaker & snMaker )
    : MbPrecision(                      )
    , mergeCurves( true                 )
    , cutCurves  ( false                )
    , _type      ( ict_General          )
    , _snMaker   ( &snMaker.Duplicate() )
  {
    if ( snMaker.GetMathVersion() > MATH_19_VERSION ) // KOMPAS-39273 + KOMPAS-40408
      cutCurves = true;
  }
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \param[in] _cutCurves   - \ru Разрезать кривые в точках пересечения.
                              \en Cut curves at intersection points. \~
    \param[in] _snMaker     - \ru Именователь с версией операции.
                              \en Names maker with operation version. \~
  */
  MbIntCurveParams( bool _cutCurves, const MbSNameMaker & snMaker )
    : MbPrecision(                      )
    , mergeCurves( true                 )
    , cutCurves  ( _cutCurves           )
    , _type      ( ict_General          )
    , _snMaker   ( &snMaker.Duplicate() )
  {}
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \param[in] _mergeCurves - \ru Объединять кривые, разрезанные швом.
                              \en Merge curves cut by a surface seam. \~
    \param[in] _cutCurves   - \ru Разрезать кривые в точках пересечения.
                              \en Cut curves at intersection points. \~
    \param[in] _snMaker     - \ru Именователь с версией операции.
                              \en Names maker with operation version. \~
  */
  MbIntCurveParams( bool _mergeCurves, bool _cutCurves, const MbSNameMaker & snMaker )
    : MbPrecision(                      )
    , mergeCurves( _mergeCurves         )
    , cutCurves  ( _cutCurves           )
    , _type      ( ict_General          )
    , _snMaker   ( &snMaker.Duplicate() )
  {}

  /** \brief \ru Конструктор.
             \en Constructor. \~
  \details \ru Конструктор по параметрам.
           \en Constructor by parameters. \~
  \param[in] nameMaker - \ru Именователь с версией операции.
                         \en Names maker with operation version. \~
  \param[in] surface1Domain - \ru Область ограничения кривой на первой поверхности.
                              \en The curve bound area on the first surface. \~
  \param[in] surface2Domain - \ru Область ограничения кривой на второй поверхности.
                              \en The curve bound area on the second surface. \~
  \param[in] uvInitOnSurface1 - \ru Начальная точка на первой поверхности.
                                \en The curve starting point on the first surface. \~
  \param[in] uvInitOnSurface2 - \ru Начальная точка на второй поверхности.
                                \en The curve starting point on the second surface. \~
  \param[in] initDirection - \ru Начальное направление создания линии пересечения.
                             \en The initial direction for the intersection curve creation. \~
    */
  MbIntCurveParams( const MbRect2D &     surface1Domain,
                    const MbRect2D &     surface2Domain,
                    const MbCartPoint &  uvInitOnSurface1,
                    const MbCartPoint &  uvInitOnSurface2,
                    const MbVector3D &   initDirection,
                    const MbSNameMaker & nameMaker )
    : MbPrecision(                                                                                    )
    , mergeCurves( true                                                                               )
    , cutCurves  ( true                                                                               )
    , _type      ( ict_OnePoint                                                                       )
    , _snMaker   ( &nameMaker.Duplicate()                                                             )
    , _values    ( new MbIntCurveOnePointValues(
                   surface1Domain, surface2Domain, uvInitOnSurface1, uvInitOnSurface2, initDirection) )
  {}

  /** \brief \ru Конструктор.
           \en Constructor. \~
  \details \ru Конструктор по параметрам.
           \en Constructor by parameters. \~
  \param[in] uv1beg - \ru Начальная точка на первой поверхности.
                      \en The start point on the first surface. \~
  \param[in] uv1end - \ru Конечная точка на первой поверхности.
                      \en The end point on the first surface. \~
  \param[in] ext1 - \ru На расширенной первой поверхности.
                    \en Whether to create on the extended surface. \~
  \param[in] uv2beg - \ru Начальная точка на второй поверхности.
                      \en The start point on the second surface. \~
  \param[in] uv2end - \ru Конечная точка на второй поверхности.
                      \en The end point on the second surface. \~
  \param[in] ext2 - \ru На расширенной второй поверхности.
                    \en Whether to create on the extended second surface. \~
  \param[in] dir - \ru Начальное направление создания линии пересечения.
                   \en The start direction for intersection curve creation. \~
  \param[in] nameMaker - \ru Именователь с версией операции.
                         \en Names maker with operation version. \~
*/
  MbIntCurveParams( const MbCartPoint &  uv1beg, 
                    const MbCartPoint &  uv1end,
                          bool           ext1, 
                    const MbCartPoint &  uv2beg,
                    const MbCartPoint &  uv2end,
                          bool           ext2, 
                    const MbVector3D &   dir,
                    const MbSNameMaker & nameMaker )
    : MbPrecision(                                                                                  )
    , mergeCurves( true                                                                             )
    , cutCurves  ( true                                                                             )
    , _type      ( ict_TwoPoints                                                                    )
    , _snMaker   ( &nameMaker.Duplicate()                                                           )
    , _values    ( new MbIntCurveLimitPointsValues(uv1beg, uv1end, ext1, uv2beg, uv2end, ext2, dir) )
  {}


/** \brief \ru Конструктор.
           \en Constructor. \~
  \details \ru Конструктор по параметрам.
           \en Constructor by parameters. \~
  \param[in] uv1beg - \ru Начальная точка на первой поверхности.
                      \en The start point on the first surface. \~
  \param[in] uv1end - \ru Конечная точка на первой поверхности.
                      \en The end point on the first surface. \~
  \param[in] ext1 - \ru На расширенной первой поверхности.
                    \en Whether to create on extended surface. \~
  \param[in] uv2beg - \ru Начальная точка на второй поверхности.
                      \en The start point on the second surface. \~
  \param[in] uv2end - \ru Конечная точка на второй поверхности.
                      \en The end point on the second surface. \~
  \param[in] ext2 - \ru На расширенной второй поверхности.
                    \en Whether to create on the extended second surface. \~
  \param[in] guideCurve - \ru Направляющая кривая, приближенно описывающая искомую кривую.
                          \en The guide curve that approximates the desired curve. \~
  \param[in] useRedetermination - \ru Флаг, определяющий нужно ли уточнять шаг построения следующей точки по сравнению с функцией DeviationStep.
                                  \en The flag that determines whether it is necessary to specify the next point build step as compared to the DeviationStep function. \~
  \param[in] checkPoles - \ru Флаг необходимости проверки и корректировки полюсных точек.
                          \en The flag that determines whether it is necessary to check and correct pole points. \~ 
  \param[in] nameMaker - \ru Именователь с версией операции.
                         \en Names maker with operation version. \~
*/
  MbIntCurveParams( const MbCartPoint &  uv1beg,
                    const MbCartPoint &  uv1end,
                          bool           ext1, 
                    const MbCartPoint &  uv2beg,
                    const MbCartPoint &  uv2end,
                          bool           ext2, 
                    const MbCurve3D *    guideCurve,
                          bool           useRedetermination,
                          bool           checkPoles,
                    const MbSNameMaker & nameMaker )
    : MbPrecision(                                                                                         )
    , mergeCurves( true                                                                                    )
    , cutCurves  ( true                                                                                    )
    , _type      ( ict_TwoPointsGuide                                                                      )
    , _snMaker   ( &nameMaker.Duplicate()                                                                  )
    , _values    ( new MbIntCurveLimitPointsValues(
                   uv1beg, uv1end, ext1, uv2beg, uv2end, ext2, guideCurve, useRedetermination, checkPoles) )
  {}

  /// \ru Получить параметры создания кривой по начальной точке. \en Get the parameters for the curve creation by the initial point. 
  const MbIntCurveValues * GetValues() const { return _values; }

  /// \ru Получить тип создания кривой пересечения. \en Get the type of the intesection curve creation. 
  MbeIntCurveBuildType GetType() const { return _type; }
  /// \ru Получить именователь. \en Get names maker. 
  const MbSNameMaker & GetNameMaker() const { return *_snMaker; }

OBVIOUS_PRIVATE_COPY( MbIntCurveParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры кривых пересечения граней двух оболочек.
           \en Parameters for intersection of curves of two shells faces. \~
  \details \ru Параметры кривых пересечения граней двух оболочек. \n
           \en Parameters for intersection of curves of two shells faces. \n \~
  \ingroup Curve3D_Building_Parameters
*/ // ---
class MATH_CLASS MbIntCurveShellParams : public MbIntCurveParams {
private:
  c3d::IndicesVector _faces1;  ///< \ru Номера граней в первой оболочке. \en The numbers of faces in the first shell.
  c3d::IndicesVector _faces2;  ///< \ru Номера граней во второй оболочке. \en The numbers of faces in the second shell.
  bool               _same1;   ///< \ru Использовать ли тот же журнал построителей первого тела. \en Flag whether to use the same creators of the first body.
  bool               _same2;   ///< \ru Использовать ли тот же журнал построителей второго тела. \en Flag whether to use the same creators of the second body.
  bool               _useCreators; ///< \ru Использовать ли построители. \en Flag of using creators.

public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по номерам граней с использованием построителей и установкой параметров объединения и разрезания кривых.
             \en Constructor by faces numbers with use of creators and definition of curves merge and cut parameters. \~
    \param[in] faceIndices1 - \ru Номера граней в первой оболочке.
                              \en The numbers of faces in the first shell. \~
    \param[in] same1        - \ru Использовать ли тот же журнал построителей первого тела или сделать копию.
                              \en Flag whether to use the same creators of the first body or make a copy. \~
    \param[in] faceIndices2 - \ru Номера граней во второй оболочке.
                              \en The numbers of faces in the second shell. \~
    \param[in] same2        - \ru Использовать ли тот же самый журнал построителей второго тела или сделать копию.
                              \en Flag whether to use the same creators of the second body or make a copy. \~
    \param[in] mergeCrvs    - \ru Объединять кривые, разрезанные швом.
                              \en Merge curves cut by a surface seam. \~
    \param[in] cutCrvs      - \ru Разрезать кривые в точках пересечения.
                              \en Cut curves at intersection points. \~
    \param[in] _snMaker     - \ru Именователь с версией операции.
                              \en Names maker with operation version. \~
  */
  MbIntCurveShellParams( const c3d::IndicesVector & faceIndices1, bool same1,
                         const c3d::IndicesVector & faceIndices2, bool same2,
                         bool mergeCrvs, bool cutCrvs,
                         const MbSNameMaker & snMaker )
    : MbIntCurveParams( mergeCrvs, cutCrvs, snMaker )
    , _faces1     ( faceIndices1 )
    , _faces2     ( faceIndices2 )
    , _same1      ( same1 )
    , _same2      ( same2 )
    , _useCreators( true ) {}

  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по номерам граней с использованием построителей.
             \en Constructor by faces numbers with use of creators. \~
    \param[in] faceIndices1 - \ru Номера граней в первой оболочке.
                              \en The numbers of faces in the first shell. \~
    \param[in] same1        - \ru Использовать ли тот же журнал построителей первого тела или сделать копию.
                              \en Flag whether to use the same creators of the first body or make a copy. \~
    \param[in] faceIndices2 - \ru Номера граней во второй оболочке.
                              \en The numbers of faces in the second shell. \~
    \param[in] same2        - \ru Использовать ли тот же самый журнал построителей второго тела или сделать копию.
                              \en Flag whether to use the same creators of the second body or make a copy. \~
    \param[in] _snMaker     - \ru Именователь с версией операции.
                              \en Names maker with operation version. \~
  */
  MbIntCurveShellParams( const c3d::IndicesVector & faceIndices1, bool same1,
                         const c3d::IndicesVector & faceIndices2, bool same2,
                         const MbSNameMaker & snMaker )
    : MbIntCurveParams( snMaker )
    , _faces1     ( faceIndices1 )
    , _faces2     ( faceIndices2 )
    , _same1      ( same1 )
    , _same2      ( same2 )
    , _useCreators( true ) {}

  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по номерам граней с установкой параметров объединения и разрезания кривых.
             \en Constructor by faces numbers with definition of curves merge and cut parameters. \~
    \param[in] faceIndices1 - \ru Номера граней в первой оболочке.
                              \en The numbers of faces in the first shell. \~
    \param[in] faceIndices2 - \ru Номера граней во второй оболочке.
                              \en The numbers of faces in the second shell. \~
    \param[in] mergeCrvs    - \ru Объединять кривые, разрезанные швом.
                              \en Merge curves cut by a surface seam. \~
    \param[in] cutCrvs      - \ru Разрезать кривые в точках пересечения.
                              \en Cut curves at intersection points. \~
    \param[in] _snMaker     - \ru Именователь с версией операции.
                              \en Names maker with operation version. \~
  */
  MbIntCurveShellParams( const c3d::IndicesVector & faceIndices1,
                         const c3d::IndicesVector & faceIndices2,
                         bool mergeCrvs, bool cutCrvs,
                         const MbSNameMaker & snMaker )
    : MbIntCurveParams( mergeCrvs, cutCrvs, snMaker )
    , _faces1     ( faceIndices1 )
    , _faces2     ( faceIndices2 )
    , _same1      ( false )
    , _same2      ( false )
    , _useCreators( false ) {}

  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameter \~
    \details \ru Конструктор по номерам граней.
             \en Constructor by faces numbers. \~
    \param[in] faceIndices1 - \ru Номера граней в первой оболочке.
                              \en The numbers of faces in the first shell. \~
    \param[in] faceIndices2 - \ru Номера граней во второй оболочке.
                              \en The numbers of faces in the second shell. \~
    \param[in] _snMaker     - \ru Именователь с версией операции.
                              \en Names maker with operation version. \~
  */
  MbIntCurveShellParams( const c3d::IndicesVector & faceIndices1,
                         const c3d::IndicesVector & faceIndices2,
                         const MbSNameMaker & snMaker )
    : MbIntCurveParams( snMaker )
    , _faces1     ( faceIndices1 )
    , _faces2     ( faceIndices2 )
    , _same1      ( false )
    , _same2      ( false )
    , _useCreators( false ) {}

  /// \ru Получить номера граней в первой оболочке. \en Get the numbers of faces in the first shell.
  const c3d::IndicesVector & GetFaceIndices1() const { return _faces1; }
  /// \ru Получить номера граней во второй оболочке. \en Get the numbers of faces in the second shell.
  const c3d::IndicesVector & GetFaceIndices2() const { return _faces2; }

  /// \ru Получить флаг использования того же журнал построителей для первого тела. \en Get the flag of using the same creators for the first body. 
  bool Same1() const { return _same1; }
  /// \ru Получить флаг использования того же журнал построителей для второго тела. \en Get the flag of using the same creators for the second body. 
  bool Same2() const { return _same2; }

  /// \ru Получить флаг использования построителей. \en Get the flag of using creators. 
  bool UseCreators() const { return _useCreators; }

OBVIOUS_PRIVATE_COPY( MbIntCurveShellParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Результаты кривой пересечения поверхностей.
           \en Results of the surface intersection curve creation. \~
  \details \ru Результаты кривой пересечения поверхностей.
           \en Results of the surface intersection curve creation. \~
  \ingroup Curve3D_Building_Parameters
*/
// ---
struct MATH_CLASS MbIntCurveResults {
public:
  SPtr<MbCurve>     _curve1;    ///< \ru Кривая на первой поверхности. \en The curve on the first surface.
  SPtr<MbCurve>     _curve2;    ///< \ru Кривая на второй поверхности. \en The curve on the second surface.
  SPtr<MbWireFrame> _wireFrame; ///< \ru Каркас с построенными кривыми. \en The frame with the constructed curves.
  MbeCurveBuildType _label;     ///< \ru Тип полученной кривой пересечения. \en The type of the resulting intersection curve.

  /// \ru Конструктор. \en Constructor.
  MbIntCurveResults() : _label( cbt_Ordinary ) {}

  /** \brief \ru Выдать 2д-кривую из ребра каркаса с заданным номером на заданной поверхности.
             \en Get the 2d-curve from the wire frame's edge with the specified number on the given surface.\~
  \param[in] surf - \ru Указатель на поверхность, на которой должна лежать кривая.
                    \en Pointer to the surface on which the curve is located \~
  \param[in] edgeIndex - \ru Номер ребра в каркасе, из которого берётся кривая.
                         \en The number of the edge inside wire frame from which the curve is taken. \~
  */
  MbCurve * SetCurve( const MbSurface & surf, size_t edgeIndex = 0 );

  /// \ru Обнулить данные. \en Reset the data.
  void Reset()
  {
    _curve1.reset();
    _curve2.reset();
    _wireFrame.reset();
  }
OBVIOUS_PRIVATE_COPY( MbIntCurveResults )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры эквидистантной кривой в пространстве.
           \en Parameters of an offset curve in space. \~
  \details \ru Параметры эквидистантной кривой в пространстве по трехмерной кривой и вектору направления. \n
           \en Parameters of an offset curve in space from a three-dimensional curve and a direction vector. \n \~
  \ingroup Curve3D_Building_Parameters
*/ // ---
//DEPRECATE_DECLARE_REPLACE ( MbOffsetWireFrameParams )
struct MATH_CLASS MbSpatialOffsetCurveParams {
public:
  MbVector3D            offsetVect;       ///< \ru Вектор, задающий смещение в точке кривой. \en The displacement vector at a point of the curve.
  bool                  useFillet;        ///< \ru Если true, то разрывы заполнять скруглением, иначе продолженными кривыми. \en If 'true', the gaps are to be filled with fillet, otherwise with the extended curves.
  bool                  keepRadius;       ///< \ru Если true, то в существующих скруглениях сохранять радиусы. \en If 'true', the existent fillet radii are to be kept.
  bool                  bluntAngle;       ///< \ru Если true, то в притуплять острые углы. \en If 'true', sharp corners are to be blunt.
  bool                  fromBeg;          ///< \ru Вектор смещения привязан к началу (если true). \en The translation vector is associated with the beginning (if true).
protected:
  bool                  useSurfaceNormal; ///< \ru Эквидистанта согласована с нормалью к поверхности. \en Offset point is moved according to surface normal. 
  c3d::ConstSurfaceSPtr surface;          ///< \ru Поверхность кривой или подобная ей. \en Curve's surface or similar to such surface. 
  c3d::SNameMakerSPtr   snMaker;          ///< \ru Именователь кривых каркаса. \en An object defining the frame curves names.

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbSpatialOffsetCurveParams( const MbVector3D & v, const MbSNameMaker & nm )
    : offsetVect      ( v               )
    , useFillet       ( false           )
    , keepRadius      ( false           )
    , bluntAngle      ( false           )
    , fromBeg         ( true            )
    , useSurfaceNormal( false           )
    , surface         ( nullptr         )
    , snMaker         ( &nm.Duplicate() )
  {}
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbSpatialOffsetCurveParams( const MbVector3D & offset, bool useFllt, bool keepRds, bool bluntAngl, bool beg, bool useNrm, c3d::ConstSurfaceSPtr & surf, const MbSNameMaker & nm )
    : offsetVect      ( offset          )
    , useFillet       ( useFllt         )
    , keepRadius      ( keepRds         )
    , bluntAngle      ( bluntAngl       )
    , fromBeg         ( beg             )
    , useSurfaceNormal( useNrm          )
    , surface         ( nullptr         )
    , snMaker         ( &nm.Duplicate() )
  {
    if ( surf != nullptr )
      SetBySurfaceNormal( useSurfaceNormal, &surf );
  }
  /// \ru Конструктор копирования. \en Copy constructor.
  MbSpatialOffsetCurveParams( const MbSpatialOffsetCurveParams & other );
  /// \ru Конструктор для чтения. \en Constructor for reading.
  MbSpatialOffsetCurveParams( TapeInit tapeInit );

public:
        /// \ru Преобразовать элемент согласно матрице. \en Transform element by matrix.
        void Transform( const MbMatrix3D & matrix );
        /// \ru поворот точкек вокруг произвольной оси \en rotate points around an arbitrary axis.
        void Rotate( const MbAxis3D & axis, double angle );
        /// \ru Сдвиг точкек на произвольный вектор \en Move points  on arbitrary vector.
        void Move( const MbVector3D & to );
        /// \ru Проверка на равенство. \en Check if *this == other.
        bool IsEqual( const MbSpatialOffsetCurveParams & other, double accuracy ) const;
        /// \ru Установка режима по поверхности, переносится ли эквидистантная точка согласованно с нормалью к поверхности. \en Setting the mode whether offset point is moved according to surface normal or not. 
        void SetBySurfaceNormal( bool set, c3d::ConstSurfaceSPtr * s = nullptr )
        {
          if ( set ) {
            useSurfaceNormal = set;
            if ( s != nullptr )
              surface = *s;
          }
          else {
            useSurfaceNormal = false;
            surface = nullptr;
          }
        }
        /// \ru Получить флаг согласованности с нормалью к поверхности. \en Get the surface normal according flag.
        bool                    BySurfaceNormal() const { return useSurfaceNormal;  }
        /// \ru Получить поверхность. \en Get surface.
  const c3d::ConstSurfaceSPtr & GetSurface()      const { return surface; }
        /// \ru Получить ссылку на именователь. \en Get names maker reference.
  const MbSNameMaker &          GetNameMaker()    const { return *snMaker; }
        /// \ru Задать именователь. \en Set names maker.
        void                    SetNameMaker( const MbSNameMaker & newName ) const { snMaker->SetNameMaker( newName, true ); }

  /// \ru Оператор присваивания. \en Assignment operator.
  MbSpatialOffsetCurveParams & operator=( const MbSpatialOffsetCurveParams & );

  KNOWN_OBJECTS_RW_REF_OPERATORS( MbSpatialOffsetCurveParams ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class. \~
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры эквидистантной кривой на поверхности грани (набора граней).
           \en Parameters of an offset curve on a face (set of the faces). \~
  \details \ru Параметры эквидистантной кривой на поверхности грани по поверхностной кривой и значению смещения. \n
           \en Parameters of an offset curve on a face from a curve on the surface and a shift value. \n \~
\ingroup Curve3D_Building_Parameters
*/ // ---
//DEPRECATE_DECLARE_REPLACE ( MbOffsetWireFrameParams )
struct MATH_CLASS MbSurfaceOffsetCurveParams {
public:
  c3d::ConstFaceSPtr  face;    ///< \ru Грань, на которой строится эквидистанта. \en The face on which to build the offset curve.
  MbAxis3D            dirAxis; ///< \ru Направление смещения с точкой приложения. \en The offset direction with a reference point.
  double              dist;    ///< \ru Величина смещения. \en The offset distance.
protected:
  c3d::SNameMakerSPtr snMaker; ///< \ru Именователь кривых каркаса. \en An object defining the frame curves names.

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbSurfaceOffsetCurveParams( const MbFace & f, const MbAxis3D & a, double d, const MbSNameMaker & nm )
    : face   ( &f )
    , dirAxis( a  )
    , dist   ( d  )
    , snMaker( &nm.Duplicate() )
  {}
  /// \ru Конструктор копирования. \en Copy constructor.
  MbSurfaceOffsetCurveParams( const MbSurfaceOffsetCurveParams & other );
  /// \ru Конструктор для чтения. \en Constructor for reading.
  MbSurfaceOffsetCurveParams( TapeInit tapeInit );

public:
  /// \ru Преобразовать элемент согласно матрице. \en Transform element by matrix.
  void Transform( const MbMatrix3D & matrix );
  /// \ru Поворот вокруг произвольной оси \en Rotate around an arbitrary axis.
  void Rotate( const MbAxis3D & axis, double angle );
  /// \ru Сдвиг на произвольный вектор \en Move  on arbitrary vector.
  void Move( const MbVector3D & to );
  /// \ru Проверка на равенство. \en Check if *this == other.
  bool IsEqual( const MbSurfaceOffsetCurveParams & other, double accuracy ) const;
  /// \ru Установка поверхности. \en Setting the surface.
  void SetFace( c3d::ConstFaceSPtr * f = nullptr )
  {
    if ( f != nullptr )
      face = *f;
  }
  /// \ru Получить ссылку на именователь. \en Get names maker reference.
  const MbSNameMaker & GetNameMaker() const { return *snMaker; }
  /// \ru Задать именователь. \en Set names maker.
  void SetNameMaker( const MbSNameMaker & newName ) const { snMaker->SetNameMaker( newName, true ); }

  /// \ru Оператор присваивания. \en Assignment operator.
  MbSurfaceOffsetCurveParams & operator=( const MbSurfaceOffsetCurveParams & );

  KNOWN_OBJECTS_RW_REF_OPERATORS( MbSurfaceOffsetCurveParams ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class. \~
};


//-------------------------------------------------------------------------------
/** \brief \ru Параметры продления одного конца кривой.
           \en Parameters of extension of the one curve end. \~
  \details \ru Параметры продления одного конца кривой.
           \en Parameters of extension of the one curve end. \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbCurveExtensionEnds
{
private:
  MbeCurveExtensionWays _extensionWay;  ///< \ru Способ продления. \en The way to extend. \~
  double                _extensionLength;  ///< \ru Длина продления (в метрическом диапазоне). Всегда неотрицательна \en The length to extend (in metric range). It is always not negative. \~
  
public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по умолчанию. Инициализирует параметрами, при которых само удлинение невозможно. Для инициализации корректными параметрами использовать конструкторы классов-наследников.
             \en Default constructor. It initializes by parameters, which any extension is impossible for. Use constructors of inherit classes in order to initialize by appropriate parameters.\~
  */
  MbCurveExtensionEnds();

protected:
//------------------------------------------------------------------------------
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \param[in] extWay - \ru способ продления кривой.
                        \en The way to extend the curve. \~
    \param[in] length - \ru Длина (в метрическом пространстве), на которую продлевается кривая.
                        \en The length (in metric space) the curve extended to. \~
  */
// ---
  MbCurveExtensionEnds( const MbeCurveExtensionWays extWay, const double length );

public:
  /// \ru Конструктор копирования. \en Copy constructor. \~
  MbCurveExtensionEnds( const MbCurveExtensionEnds & other );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор для чтения.
             \en Constructor for reading. \~
  */
  MbCurveExtensionEnds( TapeInit tapeInit );

  /// \ru Деструктор. \en Destructor. \~
  ~MbCurveExtensionEnds();

  /// \ru Минимально допустимая величина продления. \en Minimal extension length supported. \~
  static double MinLengthAllowed() { return Math::minLength; }

  /// \ru Максимально допустимая величина продления. \en Maximal extension length supported. \~
  static double MaxLengthAllowed() { return Math::maxLength; }

  /// \ru Текущий способ продления кривой. \en The Current way to extend the curve. \~
  MbeCurveExtensionWays GetWayToExtend() const { return _extensionWay; }

  /// \ru Длина (в метрическом пространстве), на которую продлевается кривая. \en The length (in metric space) the curve extended to. \~
  double GetExtensionLength() const { return _extensionLength; }

  /// \ru Проверка на равенство. \en Check if *this == other. \~
  bool IsEqual( const MbCurveExtensionEnds & other, double acuracy = Math::metricPrecision ) const;

  /// \ru Оператор присваивания. \en Assignment operator. \~
  MbCurveExtensionEnds & operator=( const MbCurveExtensionEnds & other );

KNOWN_OBJECTS_RW_REF_OPERATORS( MbCurveExtensionEnds ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};

//-------------------------------------------------------------------------------
/** \brief \ru Параметры продления одного конца кривой по касательной.
           \en Parameters of extension of the one curve end by tangent. \~
  \details \ru Параметры продления одного конца кривой по линейному закону.
           \en Parameters of extension of the one curve end by linear way. \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbCurveExtensionLinear : public MbCurveExtensionEnds
{
private:
//------------------------------------------------------------------------------
/** \brief \ru Конструктор по умолчанию.
           \en Default constructor. \~
  \details \ru Конструктор по умолчанию (не реализован).
           \en Default constructor (not implemented). \~
*/
// ---
  MbCurveExtensionLinear();

public:
//------------------------------------------------------------------------------
/** \brief \ru Конструктор.
           \en Constructor. \~
  \details \ru Конструктор по параметрам.
           \en Constructor by parameters. \~
  \param[in] length - \ru Длина (в метрическом пространстве), на которую продлевается кривая.
                      \en The length (in metric space) the curve extended to. \~
*/
// ---
  MbCurveExtensionLinear( const double length );
};

//-------------------------------------------------------------------------------
/** \brief \ru Параметры продления одного конца кривой по соприкасающейся окружности.
           \en Parameters of extension of the one curve end by osculating circle. \~
  \details \ru Параметры продления одного конца кривой по соприкасающейся окружности.
           \en Parameters of extension of the one curve end by osculating circle. \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbCurveExtensionCircular : public MbCurveExtensionEnds
{
private:
//------------------------------------------------------------------------------
/** \brief \ru Конструктор по умолчанию.
           \en Default constructor. \~
  \details \ru Конструктор по умолчанию (не реализован).
           \en Default constructor (not implemented). \~
*/
// ---
  MbCurveExtensionCircular();

public:
//------------------------------------------------------------------------------
/** \brief \ru Конструктор.
           \en Constructor. \~
  \details \ru Конструктор по параметрам.
           \en Constructor by parameters. \~
  \param[in] length - \ru Длина (в метрическом пространстве), на которую продлевается кривая.
                      \en The length (in metric space) the curve extended to. \~
*/
// ---
  MbCurveExtensionCircular( const double length );
};

//-------------------------------------------------------------------------------
/** \brief \ru Параметры продления одного конца кривой по натуральному закону.
           \en Parameters of extension of the one curve end by natural way. \~
  \details \ru Входные параметры продления одного конца кривой по натуральному закону.
           \en Input parameters of extension of the one curve end by natural way. \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbCurveExtensionNatural : public MbCurveExtensionEnds
{
private:
//------------------------------------------------------------------------------
/** \brief \ru Конструктор по умолчанию.
           \en Default constructor. \~
  \details \ru Конструктор по умолчанию (не реализован).
           \en Default constructor (not implemented). \~
*/
// ---
  MbCurveExtensionNatural();

public:
//------------------------------------------------------------------------------
/** \brief \ru Конструктор.
           \en Constructor. \~
  \details \ru Конструктор по параметрам.
           \en Constructor by parameters. \~
  \param[in] length - \ru Длина (в метрическом пространстве), на которую продлевается кривая.
                      \en The length (in metric space) the curve extended to. \~
*/
// ---
  MbCurveExtensionNatural( const double length );
};

//-------------------------------------------------------------------------------
/** \brief \ru Параметры продления кривой.
           \en Parameters of the curve extension. \~
  \details \ru Параметры продления кривой.
           \en Parameters of the curve extension. \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbCurveExtensionParameters : public MbPrecision
{
protected:
  MbCurveExtensionEnds          _paramStart;  ///< \ru Параметры продления начала. \en The parameters to extend to the start point. \~
  MbCurveExtensionEnds          _paramEnd;  ///< \ru Параметры продления конца. \en The parameters to extend to the end point. \~
  c3d::SNameMakerSPtr           _operName;  ///< \ru Именователь операции. Всегда не ноль. \en An object defining names generation in the operation. It is always not nullptr. \~
  bool                          _allowClosure;  ///< \ru Разрешено ли замыкание. По умолчанию разрешено. \en Whether closure is allowed. It is allowed by default. \~
  
public:
  //------------------------------------------------------------------------------
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \param[in] paramStart   - \ru Параметры продления начала кривой.
                              \en Parameters of extension to the start point. \~
    \param[in] paramEnd     - \ru Параметры продления конца кривой.
                              \en Parameters of extension from the end point. \~
    \param[in] allowClosure - \ru Допустимо ли замыкание удлиненной кривой.
                              \en Whether closure of the extended curve is allowed. \~
    \param[in] operName     - \ru Именователь операции.
                              \en An object defining names generation in the operation. \~
  */
  // ---
  MbCurveExtensionParameters( const MbCurveExtensionEnds & paramStart,
                              const MbCurveExtensionEnds & paramEnd,
                              const bool                   allowClosure,
                              const MbSNameMaker &         operName );

  //------------------------------------------------------------------------------
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор копирования.
             \en Copy constructor. \~
  */
  // ---
  MbCurveExtensionParameters( const MbCurveExtensionParameters & other );

  //------------------------------------------------------------------------------
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор для чтения.
             \en Constructor for reading. \~
  */
  // ---
  MbCurveExtensionParameters( TapeInit tapeInit );

private:

  //------------------------------------------------------------------------------
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
    \details \ru Конструктор по умолчанию (не реализован).
             \en Default constructor (not implemented). \~
  */
  // ---
  MbCurveExtensionParameters();

public:
  /// \ru Деструктор. \en Destructor. \~
  ~MbCurveExtensionParameters();

  /// \ru Вернуть true, если *this == other. \en Return true if *this == other. \~
  bool IsEqual( const MbCurveExtensionParameters & other ) const;

  /// \ru Поменять местами параметры концов кривой. \en Swap the parameters of curve's ends. \~
  void SwapEnds();

  //------------------------------------------------------------------------------
  /** \brief \ru Текущие параметры продления.
             \en The Current parameters to extend. \~
    \details \ru Текущие параметры продления на конце кривой.
             \en The Current parameters to extend at he curve end. \~
    \param[in] inStart - \ru true для начала кривой, false для ее конца.
                         \en It is true for start point of the curve and false for its end point. \~
  */
  // ---
  const MbCurveExtensionEnds & GetEndsParameters( const bool inStart ) const { return inStart? _paramStart : _paramEnd; }

  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation. \~
  const MbSNameMaker & GetNameMaker() const { return *_operName; }

  /// \ru Получить информацию о разрешении замкнутости. \en Get the information about closure allowing. \~
  bool IsClosureAllowed() const { return _allowClosure; }

  /// \ru Версия операции. \en Version of the operation. \~
  VERSION GetVersion() const { return _operName->GetMathVersion(); }

  /// \ru Минимальная величина зазора (в параметрическом пространстве) для случая, когда запрещено создания замкнутых кривых. \en Minimal gap value (in parametric space) for case when closed result curves are forbidden. \~
  double GetMinUnclosedGap() const { return Math::metricPrecision; }

  /// \ru Оператор присваивания. \en Assignment operator. \~
  MbCurveExtensionParameters & operator=( const MbCurveExtensionParameters & other );
KNOWN_OBJECTS_RW_REF_OPERATORS( MbCurveExtensionParameters ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};

//-------------------------------------------------------------------------------
/** \brief \ru Параметры продления кривой.
           \en Parameters of the curve extension. \~
  \details \ru Параметры продления кривой.
           \en Parameters of the curve extension. \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbCurveExtensionParameters3D : public MbCurveExtensionParameters
{
private:
  bool _extendAlongSurface;  ///< \ru Удлинять вдоль поверхности. Действует только на поверхностные кривые. \en Extend along the surface. It works only with curves on surface. \~
  
public:

  //------------------------------------------------------------------------------
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \param[in] paramStart   - \ru Параметры продления начала кривой.
                              \en Parameters of extension to the start point. \~
    \param[in] paramEnd     - \ru Параметры продления конца кривой.
                              \en Parameters of extension from the end point. \~
    \param[in] allowClosure - \ru Допустимо ли замыкание удлиненной кривой.
                              \en Whether closure of the extended curve is allowed. \~
    \param[in] alongSurface - \ru Флаг, определяющий, продлевается ли поверхностная кривая, оставаясь лежать на поверхности.
                              \en The flag determining whether the curve on surface is extended, staying to lie in the surface.
    \param[in] operName     - \ru Именователь операции.
                              \en An object defining names generation in the operation. \~
  */
  // ---
  MbCurveExtensionParameters3D( const MbCurveExtensionEnds & paramStart,
                                const MbCurveExtensionEnds & paramEnd,
                                const bool                   allowClosure,
                                const bool                   alongSurface,
                                const MbSNameMaker &         operName );

  //------------------------------------------------------------------------------
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор копирования.
             \en Copy constructor. \~
  */
  // ---
  MbCurveExtensionParameters3D( const MbCurveExtensionParameters3D & other );

  //------------------------------------------------------------------------------
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор для чтения.
             \en Constructor for reading. \~
  */
  // ---
  MbCurveExtensionParameters3D( TapeInit tapeInit );

private:

  //------------------------------------------------------------------------------
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
    \details \ru Конструктор по умолчанию (не реализован).
             \en Default constructor (not implemented). \~
  */
  // ---
  MbCurveExtensionParameters3D();

public:

  /// \ru Деструктор. \en Destructor. \~
  ~MbCurveExtensionParameters3D();

  /// \ru Вернуть true, если *this == other. \en Return true if *this == other. \~
  bool IsEqual( const MbCurveExtensionParameters3D & other ) const;

  /// \ru Если true, то поверхностные кривые после удлинения останутся лежать на поверхности. В противном случае, удлиненная часть может покинуть поверхность. Действует только на поверхностные кривые. \en If it is true then curves on surface will lie in the surface after extension. Otherwise, extending part can leave the surface. It works only with curves on surface. \~
  bool IsExtensionAlongSurface() const { return _extendAlongSurface; }

  /// \ru Оператор присваивания. \en Assignment operator. \~
  MbCurveExtensionParameters3D & operator=( const MbCurveExtensionParameters3D & other );
KNOWN_OBJECTS_RW_REF_OPERATORS( MbCurveExtensionParameters3D ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//-------------------------------------------------------------------------------
/** \brief \ru Параметры создания фаски.
           \en Parameters for the chamfer creation. \~
  \details \ru Параметры создания фаски для пары соседних сегментов контура (или полилинии).
         \en Parameters for the chamfer creation between two adjacent contour (or polyline) segments. \n \~
  \ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbCornerChamferParams {
public:
  /// \ru Расстояние от пересечения фаски с первым сегментом до узла контура.  \en Distance between the chamfer intersection with the first segment to the contour joint.
  double distance;
  /** 
  \ru (type == true ) Угол между первым сегментом и фаской.
      (type == false) Расстояние от пересечения фаски со вторым сегментом до узла контура.
  \en (type == true ) Angle between the first segment and the chamfer.
      (type == false) Distance between the chamfer intersection with the second segment to the contour joint.
  */
  double distAngle;
  /// \ru Номер первого сегмента из пары \en The number of the first segment of the pair.
  size_t index;
  /** 
    \ru Тип создания фаски: true для типа расстояние-угол, false для типа расстояние-расстояние.
    \en The type of chamfer creation: true for the distance-angle type; false for the distance-distance type.
  */
  bool type;

  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbCornerChamferParams( size_t _index, double _distance, double _distAngle, bool _type = false )
    : distance ( _distance  )
    , distAngle( _distAngle  )
    , index    ( _index )
    , type     ( _type )
  {}
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbCornerChamferParams()
    : distance ( 0     )
    , distAngle( 0     )
    , index    ( -1    )
    , type     ( false )
  {}
  /// \ru Конструктор копирования. \en Copy constructor.
  MbCornerChamferParams(const MbCornerChamferParams & other)
    : distance ( other.distance  )
    , distAngle( other.distAngle )
    , index    ( other.index     )
    , type     ( other.type      )
  {}
  /// \ru Оператор присваивания. \en Assignment operator. \~
  MbCornerChamferParams & operator=( const MbCornerChamferParams & other )
  {
    index     = other.index;
    distance  = other.distance;
    distAngle = other.distAngle;
    type      = other.type;
    return *this;
  }
};


//-------------------------------------------------------------------------------
/** \brief \ru Параметры создания фасок на 3д контуре.
           \en Parameters for the 3d-contour chamfers creation. \~
  \details \ru Параметры создания фасок на 3д контуре. \n
            Конструктор принимает на вход указатель на исходную кривую и/или вектор параметров <MbCornerChamferParams>.
            Возможно задать одинаковую фаску для всех узлов контура, в этом случае вместо вектора подаётся набор параметров без индекса.
            Исходная кривая может быть следующих типов: 3д-контур, 3д-полилиния, контур на поверхности, полилиния на поверхности.
            При прочих типах успешное создание фаски не гарантируется.
            Во время инициализации входная кривая преобразуется в контур.
           \en Parameters for the 3d-contour chamfers creation. \n
            Constructor takes in the pointer to the source curve and/or the vector of parameters <MbCornerChamferParams>.
            One can set the same chamfers for all the joints by passing the set of parameters without index instead of the parameter vector.
            The source curve can be one of the following types: 3d contour, 3d polyline, contour on a surface, polyline on a surface.
            For other types the successful chamfer creation is not guaranteed.
            A polyline is transformed into a contour during the chamfering process.\~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbContourChamferParams {
protected:
  SPtr<MbCurve3D> curve;                     ///< \ru Кривая, на которой делается фаска. \en The curve to be chamfered.
  std::vector<MbCornerChamferParams> params; ///< \ru Набор параметров \en The set of parameters.
  size_t segmentCount;                       ///< \ru Число сегментов кривой \en Numver of curve segments

private:
  /// \ru Конструктор без параметров запрещён. \en The default constructor is forbidden.
  explicit MbContourChamferParams();

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbContourChamferParams(const MbCurve3D & initCurve, const std::vector<MbCornerChamferParams> & _params);
  /// \ru Конструктор по кривой. \en Constructor by the curve.
  MbContourChamferParams(const MbCurve3D & initCurve);
  /// \ru Конструктор для создания контура на плоскости. \en Constructor for the creation of a plane contour.
  MbContourChamferParams(const MbCurve & initCurve, const MbPlacement3D & _placement, const std::vector<MbCornerChamferParams> & _params);
  /// \ru Конструктор для создания контура на плоскости. \en Constructor for the creation of a plane contour.
  MbContourChamferParams(const MbCurve & initCurve, const MbPlacement3D & _placement);
  /// \ru Конструктор по кривой с одинаковыми фасками во всех узлах. \en Constructor by the curve with same chamfers on each joint.
  MbContourChamferParams(const MbCurve3D & initCurve, double _distance, double _distAngle, bool _type = false);
  /// \ru Конструктор для создания контура на плоскости с одинаковыми фасками во всех узлах. \en Constructor by a plane contour with same chamfers on each joint.
  MbContourChamferParams(const MbCurve & initCurve, const MbPlacement3D & _placement, double _distance, double _distAngle, bool _type = false);

private:
  /// \ru Инициализируем трёхмерную кривую. \en 3d curve initialisation.
  void Init(const MbCurve3D & initCurve);
  /// \ru Инициализируем трёхмерную кривую. \en 3d curve initialisation.
  void Init(const MbCurve & initCurve, const MbPlacement3D & _placement);
  /// \ru Случай одинаковой фаски для всех узлов. \en Same chamfer for all the joints.
  void MakeChamfersSame(double _distance, double _distAngle, bool _type);

public:
  /// \ru Добавить в вектор параметров. \en Add to the vector of parameters.
  void Add(const MbCornerChamferParams & par);
  /// \ru Добавить в вектор параметров. \en Add to the vector of parameters.
  void Add(size_t _index, double _distance, double _distAngle, bool _type = false);
  /// \ru Очистить вектор параметров. \en Clear the vector of parameters.
  void Clear();

  /// \ru Получить число сегментов. \en Get the number of segments.
        size_t GetSegmentsCount()                           const { return segmentCount;}
  /// \ru Получить указатель на кривую. \en Get the curve pointer.
  const SPtr<MbCurve3D> & GetCurve()                         const { return curve;  }
  /// \ru Получить вектор параметров. \en Get the set of parameters.
  const std::vector<MbCornerChamferParams> & GetParameters() const { return params; }
  /// \ru Получить вектор параметров для изменения. \en Get the set of parameters for modification.
  std::vector<MbCornerChamferParams> & SetParameters()             { return params; }

  OBVIOUS_PRIVATE_COPY( MbContourChamferParams )
};

//------------------------------------------------------------------------------
/** \brief \ru Параметры, определяющие взаимное расположение поверхности и плоскости в операциях
               сворачивания/разворачивания кривых и точек.
           \en Parameters determining mutual location of the surface and the plane in curves/points
               wrap/unwrap operation. \~
  \details \ru Параметры, определяющие взаимное расположение поверхности и плоскости в операциях
               сворачивания/разворачивания кривых и точек. \n
               В алгоритме считается, что соблюдены следующие условия: плоскость (_place) в точке _xy
               касается поверхности (_surface) в точке _uv; во всех точках касания нормаль к поверхности
               сонаправлена с нормалью к плоскости; направление оси Y для _place совпадает с направлением
               нулевой кривизны поверхности; у _place правая тройка векторов. \n
               В случае, если данные условия не соблюдаются, либо _angle != 0 (угол поворота плоскости),
               либо _sense = false (_place является левой системой координат), то будет выполнено линейное
               преобразование результата операции с учетом соответствующих факторов. \~
           \en Parameters determining mutual location of the surface and the plane in curves/points
               wrap/unwrap operation. \n
               The following conditions are considered to be satisfied in the algorithm: the plane (_place) is tangent
               to the surface (_surface) at the points _xy (_place) and _uv (surface); surface normals are co-directional
               vectors with the plane normal in every tangent point; direction of Y-axis of _place coincides with zero-curvature
               direction of _surface; _place is right-handed coordinate system. \n
               If these conditions are not satisfied or _angle != 0 (the rotation angle of the plane) or _sense = false
               (_place is left-handed coordinate system) then the operation result will be linearly transformed 
               according to corresponding parameters. \~
  \ingroup Curve3D_Building_Parameters
*/ 
// ---
class MATH_CLASS MbWrapValues {
private:
  MbPlacement3D    _place;      ///< \ru Локальная система координат (ЛСК) плоскости. \en The local coordinate system (LCS) of the plane. \~
  c3d::SurfaceSPtr _surface;    ///< \ru Поверхность. Всегда не nullptr. \en The surface. It is always not nullptr. \~

  MbCartPoint      _xy;         ///< \ru Точка привязки на _place. \en The anchor point on _place. \~
  MbCartPoint      _uv;         ///< \ru Точка привязки в параметрической области поверхности. \en The anchor point in the parametric region of the surface. \~
  double           _angle;      ///< \ru Угол поворота _place вокруг его оси Z. \en The rotation angle of _place around its Z-axis. \~
  bool             _sense;      ///< \ru Признак левой или правой системы координат для _place. \en Indicator of right-/left-handed coordinate system of _place. \~
  bool             _keepLength; ///< \ru Должны ли длины кривых на другом носителе соответствовать оригиналам? \en Should the lengths of the curves on another carrier match the originals? \~

public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \param[in] place       - \ru Локальная система координат (ЛСК). XY - плоскость сворачивания/разворачивания.
                             \en The local coordinate system (LCS). XY - is the wrapping/unwrapping plane. \~
    \param[in] xy          - \ru Точка привязки на плоскости.
                             \en The anchor point on the plane. \~
    \param[in] surf        - \ru Поверхность для разворачивания.
                             \en The surface to unwrap. \~
    \param[in] uv          - \ru Точка привязки в параметрической области поверхности.
                             \en The anchor point in the parametric region of the surface. \~
    \param[in] copySurface - \ru Сохранить ли в классе параметров копию поверхности.
                             \en Whether to save the surface copy in this parameter class. \~
    \param[in] angle       - \ru Угол поворота плоскости place вокруг его оси Z.
                             \en The rotation angle of the plane place around its Z-axis. \~
    \param[in] sense       - \ru Признак левой или правой системы координат для плоскости.
                             \en Indicator of right-/left-handed coordinate system of the plane. \~
    \param[in] keepLength  - \ru Должны ли длины кривых на другом носителе соответствовать оригиналам.
                             \en Should the lengths of the curves on another carrier match the originals? \~
  */
  MbWrapValues( const MbPlacement3D & place,
                const MbCartPoint &   xy,
                const MbSurface &     surf,
                const MbCartPoint &   uv,
                      bool            copySurface,
                      double          angle,
                      bool            sense,
                      bool            keepLength );

  // Конструктор копирования.
  MbWrapValues( const MbWrapValues & other, bool copySurface, MbRegDuplicate * iReg );

  /// \ru Конструктор для чтения. \en Constructor for reading.
  MbWrapValues( TapeInit tapeInit );

private:
  // Конструктор по умолчанию. Не реализован.
  MbWrapValues();

  // Конструктор копирования. Не реализован.
  MbWrapValues( const MbWrapValues & other );

public:
  /** \brief \ru Инициализация параметров.
             \en Initialize parameters. \~
    \details \ru Инициализация параметров.
             \en Initialize parameters. \~
    \param[in] place       - \ru Локальная система координат (ЛСК). XY - плоскость сворачивания/разворачивания.
                             \en The local coordinate system (LCS). XY - is the wrapping/unwrapping plane. \~
    \param[in] xy          - \ru Точка привязки на плоскости.
                             \en The anchor point on the plane. \~
    \param[in] surf        - \ru Поверхность для разворачивания.
                             \en The surface to unwrap. \~
    \param[in] uv          - \ru Точка привязки в параметрической области поверхности.
                             \en The anchor point in the parametric region of the surface. \~
    \param[in] copySurface - \ru Сохранить ли в классе параметров копию поверхности.
                             \en Whether to save the surface copy in this parameter class. \~
    \param[in] angle       - \ru Угол поворота плоскости place вокруг его оси Z.
                             \en The rotation angle of the plane place around its Z-axis. \~
    \param[in] sense       - \ru Признак левой или правой системы координат для плоскости.
                             \en Indicator of right-/left-handed coordinate system of the plane. \~
    \param[in] keepLength  - \ru Должны ли длины кривых на другом носителе соответствовать оригиналам.
                             \en Should the lengths of the curves on another carrier match the originals? \~
  */
  void Init( const MbPlacement3D & place,
             const MbCartPoint &   xy,
             const MbSurface &     surf,
             const MbCartPoint &   uv,
                   bool            copySurface,
                   double          angle,
                   bool            sense,
                   bool            keepLength );

  /// \ru Преобразовать поверхность согласно матрице. \en Transform the surface according to the matrix. \~
  void TransformSurface( const MbMatrix3D & matr, MbRegTransform * iReg = nullptr );

  /// \ru Преобразовать плоскость согласно матрице. \en Transform the plane according to the matrix. \~
  void TransformPlane( const MbMatrix3D & matr );

  /// \ru Сдвинуть поверхность вдоль вектора. \en Move the surface along a vector. \~
  void MoveSurface( const MbVector3D & to, MbRegTransform * iReg = nullptr );

  /// \ru Сдвинуть плоскость вдоль вектора. \en Move the plane along a vector. \~
  void MovePlane( const MbVector3D & to );

  /// \ru Повернуть поверхность вокруг оси на заданный угол. \en Rotate the surface around an axis at a given angle. \~
  void RotateSurface( const MbAxis3D & axis, double angle, MbRegTransform * iReg = nullptr );

  /// \ru Повернуть плоскость вокруг оси на заданный угол. \en Rotate the plane around an axis at a given angle. \~
  void RotatePlane( const MbAxis3D & axis, double angle );

  /// \ru Дать локальную систему координат. \en Get the local coordinate system. \~
  const MbPlacement3D & GetPlacement() const { return _place; }

  /// \ru Дать поверхность. \en Get the surface. \~
  const MbSurface & GetSurface( bool base = false ) const { return base? _surface->GetSurface() : *_surface; }

  /// \ru Дать точку привязки на плоскости. \en Get the anchor point in the plane. \~
  const MbCartPoint & GetPlacePoint() const { return _xy; }

  /// \ru Дать точку привязки в параметрической области поверхности. \en Get the anchor point in the parametric region of the surface. \~
  const MbCartPoint & GetSurfacePoint() const { return _uv; }

  /// \ru Дать угол поворота плоскости вокруг нормали. \en Get the rotation angle of plane around the normal. \~
  double GetAngle() const { return _angle; }

  /// \ru Является ли система координат плоскости правой? \en Whether the coordinate system of the plane is right-handed. \~
  bool IsSense() const { return _sense; }

  /// \ru Должна ли совпадать длина итоговых кривых оригинальным. \en Should the result curve length be equal to the original curve length? \~
  bool IsLengthKept() const { return _keepLength; }

  /// \ru Сравнение параметров. \en The comparison with other parameters. \~
  bool IsSame( const MbWrapValues & other, double eps ) const;

private:
  /** \brief \ru Установить поверхность.
              \en Set the surface. \~
    \details \ru Установить поверхность.
              \en Set the surface. \~
    \param[in] surf - \ru Устанавливаемая поверхность (заменяющая исходную).
                      \en The surface for set (replacing the source one). \~
    \param[in] copy - \ru Если true, будет установлена копия заданной поверхности.
                      \en If it is true the copy of surf will be set. \~
    \param[in] iReg - \ru Регистратор.
                      \en Registrator. \~
  */
  void SetSurface( MbSurface & surf, bool copy, MbRegDuplicate * iReg = nullptr ) { _surface.assign( copy ? &( static_cast<MbSurface &>( surf.Duplicate( iReg ) ) ) : &surf ); }

  KNOWN_OBJECTS_RW_REF_OPERATORS( MbWrapValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class. \~
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры для переноса копий двумерных кривых и точек на другой носитель.
           \en Parameters for transferring copies of two-dimensional curves and points on another carrier. \~
  \details \ru Параметры для переноса копий двумерных кривых и точек на другой носитель.
               В случае периодических поверхностей необходимо указать значение _surfBreakParam. Для корректной
               работы алгоритма (на данный момент это касается только операции разворачивания) необходимо разомкнуть
               замкнутую поверхность. Место размыкания (положение линии разреза) определяется данным параметром.
               Задается в долях [0, 1] от диапазона поверхности, соответствующего параметрическому направлению
               (U или V, совпадает с периодическим направлением). В случае U:
                                _surfBreakParam = ( U - Umin )/( Umax - Umin ),
               где U - фактический U-параметр поверхности, соответствующий месту размыкания. Umin и Umax - границы 
               диапазона задания U-параметра поверхности (Umax - Umin = Uperiod). Для V-параметра аналогично.
               Если значение параметра не принадлежит указанному диапазону, оно будет искусственно в него вписано
               (к минимальной или максимальной границе).
               Если значение этого параметра равно UNDEFINED_DBL, он не учитывается.
               Для непериодических поверхностей данный параметр не играет никакой роли и может принимать любое значение.
               Если поверхность периодическая одновременно по U- и V-направлению, то она будет неразворачиваемой.
               В этом случае значение данного параметра также не играет никакой роли.
               Прохождение линии разреза через точку привязки поверхности (MbWrapValues::_uv) приводит к
               неопределенному поведению и поэтому является недопустимым. В этом случае линия разреза будет искусственно
               сдвинута на малую величину в сторону увеличения U/V-параметра поверхности. При этом, если она проходит через
               Umax (Vmax), тогда сдвиг будет в сторону уменьшения параметра.\n
               Также к неопределенному поведению относится ситуация, когда разворачиваемая точка или кривая целиком лежит на
               линии разреза. Такой ситуации следует избегать путем выбора другого положения линии разреза.
           \en Parameters for transferring copies of two-dimensional curves and points on another carrier. \~
               The parameter _surfBreakParam is necessary to be determined for periodic surfaces. In order to for the algorithm
               (at the moment it touches unwrap operation only) work properly, the closed surface should be made unclosed.
               The breaking place (position of cutting line) is determined by this parameter. It is set in the ratio [0, 1] of
               the surface range for the corresponding parametric direction (either U or V, which matches the periodic direction). For U:
                                _surfBreakParam = ( U - Umin )/( Umax - Umin ),
               where U is an actual U-parameter of the surface, correspond to the breaking place. Umin and Umax are bounds
               of the surface U-range (Umax - Umin = Uperiod). Analogically for V-parameter.
               If the parameter value is out of this range it will be automatically adjusted to it (to min or max boundary).
               If this parameter is equal to UNDEFINED_DBL it is not take into account at all.
               In case of non-periodic surfaces, the parameter does not play any role and can take any value.
               In case of the surface is periodic both U- and V-direction, it is undevelopable. The parameter does not
               play any role, too.
               The situation when the cutting line goes through the anchor point of the surface (MbWrapValues::_uv) leads to
               indeterminate behavior, therefore it is not allowed. In this case, the cutting line will be artificially shifted
               by a small epsilon to the side of increase U/V surface parameter. At that, if the cutting line goes through Umax (Vmax)
               then shift will be made to the side of decrease parameter.\n \~
               The situation is also indeterminate when an unwrapped point or curve lies entirely on the cutting line.
               This case should be avoided by choosing another position of the cutting line.
  \ingroup Curve3D_Building_Parameters
*/ 
// ---
class MATH_CLASS MbCurvesWrappingParams : public MbPrecision {
private:
  c3d::ConstPlaneCurvesSPtrVector _curves;  ///< \ru Двумерные кривые, копии которых будут перенесены на другой носитель. \en Two-dimensional curves, copies of which will be transferred to another carrier. \~
  c3d::ParamPointsVector          _points;  ///< \ru Двумерные точки, копии которых будут перенесены на другой носитель. \en Two-dimensional points, copies of which will be transferred to another carrier. \~
  MbWrapValues                    _wrpValues; ///< \ru Параметры операции сворачивания/разворачивания. \en The parameters of wrap/unwrap operation.
  c3d::SNameMakerSPtr             _snMaker; ///< \ru Именователь с версией операции. Всегда не nullptr. \en Names maker with operation version. It cannot be nullptr.
  double                          _surfBreakParam; ///< \ru Задает место резки периодической поверхности. Актуален только для разворачивания. \en It sets the place, where the periodic surface is to be cut. It is used for unwrap algorithm only. \~
  bool                            _cutByBounds;    ///< \ru Нужно ли обрезать полученные кривые по границам поверхности? Актуален только для сворачивания. \en Whether the result curves to be cut with the surface bounds? It is used for wrap algorithm only. \~
  bool                            _isWrap;         ///< \ru Тип операции: сворачивание (true), разворачивание (false). \en Operation type: wrap (true), unwrap (false). \~

private:
  /** \brief \ru Конструктор по-умолчанию. Не реализован.
             \en Default private constructor. Not implemented. \~
    \details \ru Конструктор по-умолчанию. Не реализован.
             \en Default private constructor. Not implemented. \~
  */
  MbCurvesWrappingParams();

  /** \brief \ru Конструктор без кривых.
             \en Constructor with no curves.\~
    \details \ru Конструктор без кривых.
             \en Constructor  with no curves.\~
    \param[in] vals      - \ru Параметры операции сворачивания/разворачивания.
                           \en The parameters of wrap/unwrap operation.\~
    \param[in] operNames - \ru Именователь с версией операции.
                           \en Names maker with operation version. \~
    \param[in] accuracy  - \ru Точность построения.
                           \en The accuracy. \~
  */
  MbCurvesWrappingParams( const MbWrapValues & vals,
                          const MbSNameMaker & operNames,
                                double         surfBreakParam,
                                bool           cutByBounds );

public:
  /** \brief \ru Конструктор по параметрам для сворачивания.
             \en Constructor by parameters for wrapping. \~
    \details \ru Конструктор по параметрам для сворачивания.
             \en Constructor by parameters for wrapping. \~
    \param[in] curves      - \ru Двумерные кривые, копии которых будут свёрнуты.
                             \en Two-dimensional curves, copies of which will be wrapped. \~
    \param[in] points      - \ru Двумерные точки, которые будут свёрнуты.
                             \en Two-dimension points to be wrapped. \~
    \param[in] vals        - \ru Параметры операции сворачивания.
                             \en The parameters of wrap operation.\~
    \param[in] operNames   - \ru Именователь с версией операции.
                             \en Names maker with operation version. \~
    \param[in] cutByBounds - \ru Нужно ли обрезать полученные кривые по границам поверхности?
                             \en Whether the result curves to be cut with the surface bounds? \~
    \param[in] copyCurves  - \ru Сохранить ли в этом классе параметров копии кривых.
                             \en Whether to save the curves copies in this parameter class. \~
  */
  MbCurvesWrappingParams( const c3d::ConstPlaneCurvesSPtrVector & curves,
                          const c3d::ParamPointsVector &          points,
                          const MbWrapValues &                    vals,
                          const MbSNameMaker &                    operNames,
                                bool                              cutByBounds,
                                bool                              copyCurves );

  /** \brief \ru Конструктор по параметрам для сворачивания.
             \en Constructor by parameters for wrapping. \~
    \details \ru Конструктор по параметрам для сворачивания.
             \en Constructor by parameters for wrapping. \~
    \param[in] curves      - \ru Трёхмерные кривые, копии которых будут свёрнуты. Ожидается, что внутри -- двумерные кривые на поверхности или плоскости.
                             \en Three-dimension curves, copies of which will be wrapped. It is expected that there are two-dimension curves on a surface or a plane inside.\~
    \param[in] points      - \ru Двумерные точки, которые будут свёрнуты.
                             \en Two-dimension points to be wrapped. \~
    \param[in] vals        - \ru Параметры операции сворачивания.
                             \en The parameters of wrap operation.\~
    \param[in] operNames   - \ru Именователь с версией операции.
                             \en Names maker with operation version. \~
    \param[in] cutByBounds - \ru Нужно ли обрезать полученные кривые по границам поверхности?
                             \en Whether the result curves to be cut with the surface bounds? \~
    \param[in] copyCurves  - \ru Сохранить ли в этом классе параметров копии кривых.
                             \en Whether to save the curves copies in this parameter class. \~
  */
  MbCurvesWrappingParams( const c3d::ConstSpaceCurvesSPtrVector & curves,
                          const c3d::ParamPointsVector &          points,
                          const MbWrapValues &                    vals,
                          const MbSNameMaker &                    operNames,
                                bool                              cutByBounds,
                                bool                              copyCurves );

  /** \brief \ru Конструктор по параметрам для разворачивания.
             \en Constructor by parameters for unwrapping. \~
    \details \ru Конструктор по параметрам для разворачивания.
             \en Constructor by parameters for unwrapping. \~
    \param[in] curves         - \ru Двумерные кривые, копии которых будут развёрнуты.
                                \en Two-dimensional curves, copies of which will be unwrapped. \~
    \param[in] points         - \ru Двумерные точки, которые будут перенесены на новый носитель.
                                \en Two-dimension points to be translated to a new carrier. \~
    \param[in] vals           - \ru Параметры операции разворачивания.
                                \en The parameters of unwrap operation.\~
    \param[in] operNames      - \ru Именователь с версией операции.
                                \en Names maker with operation version. \~
    \param[in] surfBreakParam - \ru Место резки периодической поверхности.
                                \en The place, where the periodic surface is to be cut. \~
    \param[in] copyCurves     - \ru Сохранить ли в этом классе параметров копии кривых.
                                \en Whether to save the curves copies in this parameter class. \~
  */
  MbCurvesWrappingParams( const c3d::ConstPlaneCurvesSPtrVector & curves,
                          const c3d::ParamPointsVector &          points,
                          const MbWrapValues &                    vals,
                          const MbSNameMaker &                    operNames,
                                double                            surfBreakParam,
                                bool                              copyCurves );

  /** \brief \ru Конструктор по параметрам для разворачивания.
             \en Constructor by parameters for unwrapping. \~
    \details \ru Конструктор по параметрам для разворачивания.
             \en Constructor by parameters for unwrapping. \~
    \param[in] curves         - \ru Трёхмерные кривые, копии которых будут развёрнуты. Ожидается, что внутри -- двумерные кривые на поверхности или плоскости.
                                \en Three-dimension curves, copies of which will be unwrapped. It is expected that there are two-dimension curves on a surface or a plane inside.\~
    \param[in] points         - \ru Двумерные точки, которые будут перенесены на новый носитель.
                                \en Two-dimension points to be translated to a new carrier. \~
    \param[in] vals           - \ru Параметры операции разворачивания.
                                \en The parameters of unwrap operation.\~
    \param[in] operNames      - \ru Именователь с версией операции.
                                \en Names maker with operation version. \~
    \param[in] surfBreakParam - \ru Место резки периодической поверхности.
                                \en The place, where the periodic surface is to be cut. \~
    \param[in] copyCurves     - \ru Сохранить ли в этом классе параметров копии кривых.
                                \en Whether to save the curves copies in this parameter class. \~
  */
  MbCurvesWrappingParams( const c3d::ConstSpaceCurvesSPtrVector & curves,
                          const c3d::ParamPointsVector & points,
                          const MbWrapValues &           vals,
                          const MbSNameMaker &           operNames,
                                double                   surfBreakParam,
                                bool                     copyCurves );

  /** \brief \ru Конструктор копирования.
             \en Copy constructor.\~
    \details \ru Конструктор копирования. Старые кривые и точки не выбрасываются (новые добавляются поверх).
             \en Copy constructor. Previous curves and points are not eliminated. New ones are added on top of existing. \~
    \param[in] copyCurves - \ru Создать копии кривых.
                            \en Create copies of the curves. \~
    \param[in] copySurface - \ru Создать копию поверхности.
                             \en Create a copy of the surface. \~
  */
  MbCurvesWrappingParams( const MbCurvesWrappingParams & other, bool copyCurves, bool copySurface, MbRegDuplicate * iReg = nullptr );
  /// \ru Конструктор для чтения. \en Constructor for reading.
  MbCurvesWrappingParams( TapeInit tapeInit );
  /// \ru Деструктор. \ en Destructor.
  ~MbCurvesWrappingParams() {}

public:
  /** \brief \ru Инициализация параметров для алгоритма сворачивания.
             \en Initialize parameters for wrapping algorithm. \~
    \details \ru Инициализация параметров для алгоритма сворачивания.
             \en Initialize parameters for wrapping algorithm. \~
    \param[in] curves             - \ru Кривые, копии которых будут свёрнуты.
                                    \en Curves, copies of which will be wrapped. \~
    \param[in] points             - \ru Двумерные точки, которые будут свёрнуты.
                                    \en Two-dimension points to be wrapped. \~
    \param[in] vals               - \ru Параметры операции сворачивания.
                                    \en The parameters of wrap operation.\~
    \param[in] operNames          - \ru Именователь с версией операции.
                                    \en Names maker with operation version. \~
    \param[in] cutByBounds        - \ru Нужно ли обрезать полученные кривые по границам поверхности?
                                    \en Whether the result curves to be cut with the surface bounds? \~
    \param[in] copyCurves         - \ru Сохранить ли в этом классе параметров копии кривых?
                                    \en Whether to save the curves copies in this parameter class? \~
    \param[in] removeExistCurves  - \ru Если true, ранее добавленные кривые будут удалены.
                                    \en If it is true the earlier added curves will be deleted. \~
    \param[in] removeExistPoints  - \ru Если true, ранее добавленные точки будут удалены.
                                    \en If it is true the earlier added points will be deleted. \~
  */
  template<class CurvesVector> // c3d::ConstPlaneCurvesSPtrVector, c3d::ConstSpaceCurvesSPtrVector
  void InitWrap( const CurvesVector &           curves,
                 const c3d::ParamPointsVector & points,
                 const MbWrapValues &           vals,
                 const MbSNameMaker &           operNames,
                       bool                     cutByBounds,
                       bool                     copyCurves,
                       bool                     removeExistCurves,
                       bool                     removeExistPoints );

  /** \brief \ru Инициализация параметров для алгоритма разворачивания.
             \en Initialize parameters for unwrapping algorithm. \~
    \details \ru Инициализация параметров для алгоритма разворачивания.
             \en Initialize parameters for unwrapping algorithm. \~
    \param[in] curves             - \ru Кривые, копии которых будут развёрнуты.
                                    \en Curves, copies of which will be unwrapped. \~
    \param[in] points             - \ru Двумерные точки, которые будут развёрнуты.
                                    \en Two-dimension points to be unwrapped. \~
    \param[in] vals               - \ru Параметры операции разворачивания.
                                    \en The parameters of unwrap operation.\~
    \param[in] operNames          - \ru Именователь с версией операции.
                                    \en Names maker with operation version. \~
    \param[in] surfBreakParam     - \ru Место резки периодической поверхности.
                                    \en The place, where the periodic surface is to be cut. \~
    \param[in] copyCurves         - \ru Сохранить ли в этом классе параметров копии кривых.
                                    \en Whether to save the curves copies in this parameter class. \~
    \param[in] removeExistCurves  - \ru Если true, ранее добавленные кривые будут удалены.
                                    \en If it is true the earlier added curves will be deleted. \~
    \param[in] removeExistPoints  - \ru Если true, ранее добавленные точки будут удалены.
                                    \en If it is true the earlier added points will be deleted. \~
  */
  template<class CurvesVector> // c3d::ConstPlaneCurvesSPtrVector, c3d::ConstSpaceCurvesSPtrVector
  void InitUnwrap( const CurvesVector &           curves,
                   const c3d::ParamPointsVector & points,
                   const MbWrapValues &           vals,
                   const MbSNameMaker &           operNames,
                         double                   surfBreakParam,
                         bool                     copyCurves,
                         bool                     removeExistCurves,
                         bool                     removeExistPoints );

  /// \ru Дать двумерные кривые. \en Get two-dimensional curves. \~
  const c3d::ConstPlaneCurvesSPtrVector & GetCurves() const { return _curves; }
  /// \ru Дать количество кривых. \en Get two-dimensional curves count. \~
        size_t                  GetCurvesCount()                       const { return _curves.size(); }

  /// \ru Дать двумерные точки. \en Get the two-dimensional points. \~
  const c3d::ParamPointsVector & GetPoints() const { return _points; }
  /// \ru Дать количество точек. \en Get points count. \~
        size_t                  GetPointsCount() const { return _points.size(); }

  /// \ru Получить параметры операции. \en Get an operation parameters. \~
  const MbWrapValues &          GetParamValues() const { return _wrpValues; }

  /// \ru Изменить параметры операции. \en Modify an operation parameters. \~
        MbWrapValues &          SetParamValues() { return _wrpValues; }

  /// \ru Вернуть тип операции: сворачивание (true), разворачивание (false)? \en Return the operation type: wrap (true), unwrap (false). \~
        bool               IsWrap() const { return _isWrap; }

  /// \ru Нужно ли обрезать полученные кривые по границам поверхности? \en Whether the result curves to be cut with the surface bounds?. \~
        bool               IsBoundsCut() const { return _cutByBounds; }

  /// \ru Получить именователь. \en Get names maker. 
  const MbSNameMaker & GetNameMaker() const { return *_snMaker; }

  /// \ru Дать параметр (в долях [0, 1]) поверхности, соответствующий ее месту резки. \en Return the parameter (in ratio) of the surface, where the periodic surface is to be cut. \~
        double GetSurfaceBreakingParam() const { return _surfBreakParam; }

  /// \ru Сравнение параметров. \en The comparison with other parameters. \~
        bool               IsSame   ( const MbCurvesWrappingParams & other, double accuracy ) const;
  /// \ru Сравнение параметров. \en The comparison with other parameters. \~
        bool               IsSimilar( const MbCurvesWrappingParams & other )                  const;

  /// \ru Преобразовать объект согласно матрице. \en Transform an object according to the matrix. \~
  void Transform( const MbMatrix3D & matr, MbRegTransform * iReg = nullptr );

  /// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector. \~
  void Move( const MbVector3D & to, MbRegTransform * iReg = nullptr );

  /// \ru Повернуть объект вокруг оси на заданный угол. \en Rotate an object around an axis at a given angle. \~
  void Rotate( const MbAxis3D & axis, double angle, MbRegTransform * iReg = nullptr );

  /** \brief \ru Создать копию текущих параметров.
             \en Make a copy of current parameters.\~
    \details \ru Создать копию текущих параметров.
             \en Make a copy of current parameters.\~
    \param[in] copyCurves - \ru Создать копии кривых.
                            \en Create copies of the curves. \~
    \param[in] copySurface - \ru Создать копию поверхности.
                             \en Create a copy of the surface. \~
  */
  MbCurvesWrappingParams * Duplicate( bool copyCurves, bool copySurface );

  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  void        GetProperties( MbProperties & );
  /// \ru Записать свойства объекта. \en Set properties of the object.
  void        SetProperties( const MbProperties & );

  /** \brief \ru Сделать объекты равными.
             \en Make the objects equal.\~
    \details \ru Сделать объекты равными, если они подобны.
             \en Make the objects equal if they are similar.\~
    \param[in] other       - \ru Копируемый объект.
                             \en Object to be copied. \~
    \param[in] copyCurves  - \ru Создать копии кривых.
                             \en Create copies of the curves. \~
    \param[in] iReg        - \ru Регистратор.
                             \en Registrator. \~
  */
  void        SetEqual( const MbCurvesWrappingParams & other,
                              bool                     copyCurves,
                              MbRegDuplicate *         iReg = nullptr );

private:
  /// \ru Добавить двумерные или трехмерные кривые с проверкой на нули. \en Add two- or three-dimension curves with nullptr check. \~
  template<class CurveVector>
  void AddCurves( const CurveVector & curves, bool copyCurves, MbRegDuplicate * iReg );
  /// \ru Добавить кривую без копирования с проверкой на nullptr. \en Add curve without copy and with nullptr check. \~ 
  void AddCurve( const MbCurve3D * curve );
  ///< \ru Добавить кривую без копирования с проверкой на nullptr. \en Add curve without copy and with nullptr check. \~ 
  void AddCurve( const MbCurve *   curve );
  ///< \ru Добавить кривую с проверкой на nullptr. \en Add curve with nullptr check. \~ 
  void AddCurve( const MbCurve3D * curve, bool copyCurve /*= false */, MbRegDuplicate * iReg );
  ///< \ru Добавить кривую с проверкой на nullptr. \en Add curve with nullptr check. \~ 
  void AddCurve( const MbCurve *   curve, bool copyCurve /*= false */, MbRegDuplicate * iReg );

  ///< \ru Добавить точки. \en Add points. \~ 
  void AddPoints( const c3d::ParamPointsVector & points_ );

  /// \ru Добавить точку. \en Add point. \~
  void AddPoint( const MbCartPoint & uv_ );

KNOWN_OBJECTS_RW_REF_OPERATORS( MbCurvesWrappingParams ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class. \~
OBVIOUS_PRIVATE_COPY( MbCurvesWrappingParams )
};

//------------------------------------------------------------------------------
/** \brief \ru Результат операции переноса копий двумерных кривых и точек на другой носитель.
           \en Result of operation to transfer copies of two-dimensional curves and points on another carrier. \~
    \details \ru Результат операции переноса копий двумерных кривых и точек на другой носитель.
             \en Result of operation to transfer copies of two-dimensional curves and points on another carrier. \~
    \ingroup Curve3D_Building_Parameters
*/
// ---
class MATH_CLASS MbCurvesWrappingResults : public MbOperationResults {
private:
  c3d::WireFrameSPtr _resWire;   ///< \ru Каркас кривых. \en Wire-frame. \~
  c3d::WireFrameSPtr _resPoints;  ///< \ru Каркас для хранения точек. \en Frame to store points. \~

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbCurvesWrappingResults();

  /// \ru Деструктор. \en Destructor.
  ~MbCurvesWrappingResults() {};

  /// \ru Переинициализировать хранилище результирующих кривых. Для внутреннего использования. \en Reinitialize container of result curves. For internal use.
  void InitFrames( const c3d::WireFrameSPtr & curves, const c3d::WireFrameSPtr & points ) { _resWire = curves; _resPoints = points; };

  /// \ru Выдать количество кривых. \en Get number of curves.
  size_t GetCurvesCount() const { return _resWire.is_null() ? 0 : _resWire->GetEdgesCount(); }

  /// \ru Выдать количество точек. \en Get number of points.
  size_t GetPointsCount() const { return _resPoints.is_null() ? 0 : _resPoints->GetEdgesCount(); }

  /// \ru Получить 3D-кривые. \en Get 3D-curves.
  bool GetCurves( c3d::SpaceCurvesSPtrVector & curves ) const;

  /// \ru Получить двумерные кривые. \en Get two-dimension curves.
  bool GetCurves( c3d::PlaneCurvesSPtrVector & curves ) const;

  /// \ru Отцепить кривые из владеющего указателя. \en Detach curves from owning pointer.
  bool DetachCurves( c3d::SpaceCurvesSPtrVector & curves );

  /// \ru Отцепить кривые из владеющего указателя. \en Detach curves from owning pointer.
  bool DetachCurves( c3d::PlaneCurvesSPtrVector & curves );

  /// \ru Получить трехмерные точки. \en Get two-dimension points.
  bool GetPoints( c3d::SpacePointsVector & points ) const;

  /// \ru Получить двумерные точки. \en Get three-dimension points.
  bool GetPoints( c3d::ParamPointsVector & points ) const;

  /// \ru Отцепить точки из владеющего указателя. \en Detach points from owning pointer.
  bool DetachPoints( c3d::SpacePointsVector & points );

  /// \ru Отцепить точки из владеющего указателя. \en Detach points from owning pointer.
  bool DetachPoints( c3d::ParamPointsVector & points );

  /// \ru Выдать результирующий проволочный каркас со строителем. \en Get result wireframe with creator.
  const c3d::WireFrameSPtr & GetWireFrameCurves() const { return _resWire; };

  /// \ru Выдать результирующий точечный каркас со строителем. \en Get result pointframe with creator.
  const c3d::WireFrameSPtr & GetWireFramePoints() const { return _resPoints; };

  /// \ru Отцепить каркас кривых из владеющего указателя. \en Detach wireframe of curves from owning pointer.
  MbWireFrame * DetachWireFrameCurves() { return ::DetachItem( _resWire ); }

  /// \ru Отцепить каркас точек из владеющего указателя. \en Detach wireframe of points from owning pointer.
  MbWireFrame * DetachWireFramePoints() { return ::DetachItem( _resPoints ); }
};

//-------------------------------------------------------------------------------
/** \brief \ru Параметры создания клотоиды.
           \en Parameters for a clothoid creation. \~
\details \ru Параметры создания клотоиды.
         \en Parameters for a clothoid creation. \n \~
\ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbClothoidParams
{
public:
  double _radiusMin;  ///< \ru Радиус кривизны на конце начального участка клотоиды. \en Curvature radius of the end of the initial part of Clothoid. \~
  double _lengthMax;  ///< \ru Максимальная длина начального участка клотоиды. \en Max length of the initial part of Clothoid. \~
  size_t _segmsNum;   ///< \ru Количество сегментов аппроксимирующей клотоиду кривой. \en Number of segments of a curve approximating Clothoid. \~

public:
  /// \ru Конструктор. \en Constructor. \~
  MbClothoidParams(double r, double l, size_t s) : _radiusMin( r ), _lengthMax( l ), _segmsNum( s ) {}
  /// \ru Конструктор. \en Constructor. \~
  MbClothoidParams() : _radiusMin( 50.0 ), _lengthMax( 200.0 ), _segmsNum( 10 ) {}

  // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool IsEqual( const MbClothoidParams & other, double accuracy ) const {
    return  ::fabs( _radiusMin - other._radiusMin ) < accuracy &&
            ::fabs( _lengthMax - other._lengthMax ) < accuracy &&
            _segmsNum == other._segmsNum;
  }


KNOWN_OBJECTS_RW_REF_OPERATORS( MbClothoidParams ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры построения гладких кривых на базе трехмерной ломаной.
           \en The parameters of the construction of smooth curves based on a three-dimensional polyline. \~
  \details \ru Данные содержат параметры построения сплайнов с плавным изменением кривизны. \n
           \en The data contains parameters for constructing splines with smooth curvature changes. \n \~
  \ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbFairCreateData : public MbPrecision
{
public:
  typedef std::vector<std::pair<size_t, MbVector3D> >  IndexVectorArray;

private:
  IndexVectorArray    _fixData; /// \ru Массив данных для фиксации точек и касательных. \en Data array for fixing points and tangents.
public:
  // \ru Общие параметры построения плавной кривой. \en The common parameters for a fire curve creation.
  MbeFairSmoothing    _fairing;               ///< \ru Сглаживание. \en Smoothing of curve. \~
  bool                _arrange;               ///< \ru Перераспределение точек по контуру. \en Redistribution of points. \~
  MbeFairSubdivision  _subdivision;           ///< \ru Коэффициент уплотнения кривой. \en Curve subdivision coefficient . \~
  MbeFairCurvature    _accountCurvature;      ///< \ru Учет кривизны в концевых точках. \en Accounting for curvature at end points. \~
  MbeFairVector       _accountVectorInflect;  ///< \ru Учет вектора в точке перегиба при построении NURBzS кривой. \en How to take into account the vector at the inflection point at creating NURBzS. \~
  MbeFixPntTng        _fixPointTangent;       ///< \ru Фиксировать точки на касательных / касательные в точках.       \en Fix the points on tangents / the tangents on points.

  // \ru Параметры построения B-сплайновой кривой. \en The Parameters for B-Spline curve creation.
  size_t              _degree;            ///< \ru Степень B-сплайновой кривой m ( 3 <= m <= 10). \en The degree m (3 <= m <= 10) of B-Spline curve. \~ 
  MbeFairApprox       _approx;            ///< \ru Метод аппроксимации. \en Approx method. \~
  bool                _tangentCorrect;    ///< \ru Сохранение вектора в точке перегиба при построении B-кривой. \en Save the vector at the inflection point at creating B-curve. \~
  bool                _smoothTorsion;     ///< \ru Флаг сглаживания кручения пространственной кривой (false - незамкнут, true - замкнут). \en Sign of smoothing of torsion of curve.
  size_t              _iterationsNumber;  ///< \ru Количество итераций построения B-сплайна (заданное и фактическое). \en The number of iterations for building the B-spline (given and actual).

  // \ru Параметры сглаживания зашумленных точек. \en Parameters for smoothing noisy points.
  double              _noisyDeviation;    ///< \ru Величина отклонения сглаживающей кривой от зашумленных точек.  \en Value of smoothing curve deviation from noisy points.
  size_t              _noisyIterations;   ///< \ru Количество итераций сглаживания зашумленных точек.  \en Number of iterations of smoothing noisy points.
  bool                _fixFirstPointNoisy; ///< \ru Флаг фиксации сплайна в начальной точке. \en Flag of fixing a spline at the first point.
  bool                _fixLastPointNoisy;  ///< \ru Флаг фиксации сплайна в конечной точке. \en Flag of fixing a spline at the last point.

  const MbNurbs3D *   _referenceCurve;     ///< \ru Кривая для сравнения с результатом аппроксимации. \en Curve for result comparing.

  // \ru Диагностика. \en The diagnostics.
  MbeFairWarning      _warning; ///< \ru Предупреждение о работе. \en The operation warning. \~
  MbResultType        _error; ///< \ru Ошибка о работе. \en The operation error. \~

#ifdef C3D_DEBUG_FAIR_CURVES
  /*DEBUG*/  FILE * prt;
#endif

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbFairCreateData() : MbPrecision( METRIC_ACCURACY * 0.1, ANGLE_REGION ),
    _fairing( fairSmooth_Yes ), _arrange( false ), _subdivision( fairSubdiv_Single ),
    _accountCurvature( fairCur_No ), _accountVectorInflect( fairVector_Tangent ), _fixPointTangent( fixPntTng_NotFix ),
    _degree( 8 ), _approx( fairApprox_IsoSpline ), _tangentCorrect( true ),
    _smoothTorsion( false ),  _iterationsNumber( 1000 ),
    _noisyDeviation( 0.002 ), _noisyIterations( 10 ),
    _fixFirstPointNoisy( false ), _fixLastPointNoisy( false ),
    _referenceCurve( nullptr ),
#ifdef C3D_DEBUG_FAIR_CURVES
    prt( nullptr ),
#endif
    _warning( fwarn_Success ), _error( rt_Success ) {}

  ~MbFairCreateData() {}

  // \ru Инициализировать данные для фиксации точек и касательных. \en Initialize данные для фиксации точек и касательных..
  void InitFixConstraints( const std::vector<std::pair<size_t, MbVector3D *>> & constraints );

  //< \ru Получить данные для фиксации точек и касательных. \en Get данные для фиксации точек и касательных.
  const IndexVectorArray & GetFixConstraints() const { return _fixData; }

  ///< \ru Получить кривую для сравнения. \en Get curve for result comparing.
  const MbNurbs3D * GetReferenceCurve() const { return _referenceCurve; }

  ///< \ru Установить кривую для сравнения. \en Set curve for result comparing.
  void SetReferenceCurve( const MbNurbs3D * pCurve ) { _referenceCurve = pCurve; }


  /// \ru Оператор присваивания. \en Assignment operator. 
  MbFairCreateData & operator = ( const MbFairCreateData & other );

  // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool IsEqual( const MbFairCreateData &, double accuracy ) const;

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbFairCreateData, MATH_FUNC_EX ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class. 

}; // MbFairCreateData


//------------------------------------------------------------------------------
/** \brief \ru Параметры построения гладких кривых сопряжения на базе трехмерной ломаной.
           \en The parameters of the construction of smooth fillet curves based on a three-dimensional polyline. \~
  \details \ru Параметры построения гладких кривых сопряжения на базе трехмерной ломаной. \n
           \en The parameters of the construction of smooth fillet curves based on a three-dimensional polyline. \n \~
  \ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbFairFilletData : public MbPrecision
{
public:
  bool                _arrange;      ///< \ru Перераспределение точек по контуру. \en Redistribution of points. \~
  MbeFairSubdivision  _subdivision;  ///< \ru Коэффициент уплотнения кривой. \en Curve subdivision coefficient . \~
  bool                _tangent;      ///< \ru Метод (на опорной ломаной или на касательных). \en Method (on base polyline or tangents).

  bool                _switchEndTangents;  ///< \ru Флаги учета значений концевых касательных. \en Flags accounting tangents values.
  MbVector3D          _firstTangent;       ///< \ru Касательная в начальной точке. \en Tangent in the first point.
  MbVector3D          _lastTangent;        ///< \ru Касательная в конечной точке. \en Tangent in the last point.

  bool                _switchEndCurvature; ///< \ru Флаги учета значений кривизны. \en Flags accounting curvature values.
  double              _firstCurvature;     ///< \ru Значение кривизны в начальной точке. \en Curvature in the first point.
  double              _lastCurvature;      ///< \ru Значение кривизны в конечной точке. \en Curvature in the last point.

  MbeFairWarning      _warning;      ///< \ru Предупреждение. \en The operation warning. \~
  MbResultType        _error;        ///< \ru Ошибка. \en The operation error. \~

#ifdef C3D_DEBUG_FAIR_CURVES
  /*DEBUG*/  FILE * prt;
#endif

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbFairFilletData() : MbPrecision(),
    _arrange( false ), _subdivision( fairSubdiv_Single ), _tangent( false ),
    _switchEndTangents( false ), _switchEndCurvature( false ),
    _firstCurvature( 0.0 ), _lastCurvature( 0.0 ),
#ifdef C3D_DEBUG_FAIR_CURVES
    prt( nullptr ),
#endif
    _warning( fwarn_Success ), _error( rt_Success ) {}

  ~MbFairFilletData() {}

  /// \ru Оператор присваивания. \en Assignment operator. 
  MbFairFilletData & operator = ( const MbFairFilletData & other );

  // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool IsEqual( const MbFairFilletData &, double accuracy ) const;

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbFairFilletData, MATH_FUNC_EX ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class. 

}; // MbFairFilletData


//------------------------------------------------------------------------------
/** \brief \ru Параметры изменения гладких кривых.
           \en The parameters for modifying smooth curves. \~
  \details \ru Данные содержат параметры изменения гладких кривых. \n
           \en The data contains parameters for modifying smooth curves. \n \~
  \ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbFairChangeData : public MbPrecision
{
public:
  MbeFairSplineFormat _outFormat;  ///< \ru Выходной формат сплайна. \en Output format of spline. \~
  size_t              _nSegments;  ///< \ru Количество сегментов сплайна. \en Number of segments of spline. \~
  size_t              _numSegment; ///< \ru Номер сегмента. \en Number of segment. \~
  double              _tParam;     ///< \ru Внутренний параметр точки сегмента сплайна. \en Point internal param on segment of spline. \~

  MbeFairSmoothing    _fairing;    ///< \ru Сглаживание. \en Smoothing of curve. \~
  size_t              _degree;     ///< \ru Степень B-сплайновой кривой m ( 3 <= m <= 10). \en The degree m (3 <= m <= 10) of B-Spline curve. \~ 
  double              _scaleParam; ///< \ru Параметр репараметризации. \en Scaling parameter.    
  MbeFairCurvature    _accountCurvature;  ///< \ru Учет кривизны в концевых точках. \en Accounting for curvature at end points. \~

  bool                _fixFirstPointNoisy; ///< \ru Флаг фиксации сплайна в начальной точке. \en Flag of fixing a spline at the first point.
  bool                _fixLastPointNoisy;  ///< \ru Флаг фиксации сплайна в конечной точке. \en Flag of fixing a spline at the last point.

  SArray<size_t>        _arrayFixNoisyNum;   ///< \ru Номера точек точных значений зашумленных точек. \en Signs of exactly noisy points. 

  MbeFairWarning      _warning;    ///< \ru Предупреждение о работе. \en The operation warning. \~
  MbResultType        _error;      ///< \ru Ошибка о работе. \en The operation error. \~

#ifdef C3D_DEBUG_FAIR_CURVES
  /*DEBUG*/  FILE * prt;
#endif

public:
  /// \ru Пустой конструктор. \en Empty constructor. 
  MbFairChangeData() : MbPrecision(),
    _outFormat( fairFormat_Close ), _nSegments( 4 ), _numSegment( 0 ), _tParam( 0.5 ),
    _fairing( fairSmooth_Yes ), _degree ( 8 ), _scaleParam( 1.0 ), _accountCurvature( fairCur_No ),
    _fixFirstPointNoisy( false ), _fixLastPointNoisy( false ),
#ifdef C3D_DEBUG_FAIR_CURVES
    prt( nullptr ),
#endif
    _warning( fwarn_Success ), _error( rt_Success ) {}

  ~MbFairChangeData() {}

  /// \ru Оператор присваивания. \en Assignment operator. 
  MbFairChangeData & operator = ( const MbFairChangeData & other );

  // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool IsEqual( const MbFairChangeData &, double accuracy ) const;

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbFairChangeData, MATH_FUNC_EX ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class. 

}; // MbFairChangeData


//------------------------------------------------------------------------------
/** \brief \ru Параметры построения или изменения гладкой кривой.
           \en The parameters of creating or changing a fair curve. \~
  \details \ru Параметры построения или изменения гладкой кривой. \n
           \en The parameters of creating or changing a fair curve. \n \~
  \ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbFairCurveParams
{
  c3d::WireFrameSPtr   _initCurve;  ///< \ru Исходная кривая. \en An initial curve.
  const MbSNameMaker & _snMaker;    ///< \ru Именователь кривых каркаса. \en An object defining the frame curves names.

public:
  /// \ru Конструктор по кривой. \en Constructor by a curve. 
  MbFairCurveParams( const MbCurve3D & crv, const MbSNameMaker & maker );
  /// \ru Конструктор по каркасу. \en Constructor by a frame. 
  MbFairCurveParams( const MbWireFrame & crv, const MbSNameMaker & maker );
  /// \ru Конструктор по каркасу. \en Constructor by a frame. 
  MbFairCurveParams( const c3d::WireFrameSPtr & crv, const MbSNameMaker & maker );

  /// \ru Деструктор. \en Destructor. 
  ~MbFairCurveParams();

  /// \ru Получить исходную кривую. \en Get the initial curve. 
  c3d::WireFrameSPtr & GetCurve() { return _initCurve; }

  /// \ru Получить исходную кривую. \en Get the initial curve. 
  const c3d::WireFrameSPtr & GetCurve() const { return _initCurve; }

  /// \ru Получить именователь. \en Get names maker. 
  const MbSNameMaker & GetNameMaker() const { return _snMaker; }

OBVIOUS_PRIVATE_COPY( MbFairCurveParams )

}; // MbFairCurveParams


//------------------------------------------------------------------------------
/** \brief \ru Результат построения или изменения гладкой кривой.
           \en The result of creating or changing a fair curve. \~
  \details \ru Результат построения или изменения гладкой кривой. \n
           \en The result of creating or changing a fair curve. \n \~
  \ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbFairCurveResult : public MbApproxError
{
public:
  c3d::WireFrameSPtr _resultCurve;
  MbeFairWarning     _warning;

public:
  /// \ru Пустой конструктор. \en Empty constructor. 
  MbFairCurveResult() : MbApproxError(), _warning( fwarn_Success ) {}
  ~MbFairCurveResult() {}

  /// \ru Получить результирующую кривую. \en Get the result curve. 
  const c3d::WireFrameSPtr & GetCurve() const { return _resultCurve; }

  /// \ru Получить результирующую кривую. \en Get the result curve. 
  c3d::WireFrameSPtr & SetCurve() { return _resultCurve; }

  /// \ru Забрать кривую, удалив ее из каркаса. \en Extract a curve, removing it from wireframe.
  c3d::SpaceCurveSPtr ExtractCurve() {
    c3d::SpaceCurvesSPtrVector curves;
    ExtractCurves( _resultCurve, curves );
    return curves.size() ? curves[0] : nullptr;
  }

  /// \ru Сведения об ошибке аппроксимации. \en Approximation error information.
  const MbApproxError & GetApproxError() const { return *this; }

  /// \ru Сведения об ошибке аппроксимации. \en Approximation error information.
  MbApproxError & SetApproxError() { return *this; }

  /// \ru Сведения о предупреждении. \en Warning information.
  MbeFairWarning GetWarning() { return _warning; }

  /// \ru Сбросить данные. \en Reset data. 
  void Reset() {
    MbApproxError::Reset();
    _resultCurve.reset();
  }


OBVIOUS_PRIVATE_COPY( MbFairCurveResult )

}; // MbFairCurveResult


//------------------------------------------------------------------------------
/** \brief \ru Входные параметры функции поиска особых точек кривизны и вычисления потенциальной энергии кривой.
           \en Input parameters of the function for searching special points of curvature and calculation of potential curve energy. \~
    \ingroup Algorithms_3D
    \warning \ru В разработке.
             \en Under development. \~
*/ // ---
class MATH_CLASS MbCurveMinMaxCurvatureParams
{
private:
  /// \ru Опции, определяющие расчет особых точек функции кривизны и потенциальной энергии кривой.
  /// \en Options defining calculation of special points of the curvature function and  potential energy of the curve. 
  enum class SpecialPointsFlags : uint8
  {
    bendingPoints       = 0x01,    ///< \ru Флаг расчета точек перегиба. \en Flag for calculation of curvature bending points. 
    localMinimumPoints  = 0x02,    ///< \ru Флаг расчета точек локальных минимумов кривизны. \en Flag for calculation of curvature local minimum points.
    localMaximumPoints  = 0x04,    ///< \ru Флаг расчета точек локальных максимумов кривизны. \en Flag for calculation of curvature local maximum points.
    breakPoints         = 0x08,    ///< \ru Флаг расчета точек разрыва кривизны. \en Flag for calculation of curvature break points.
    curveEnergy         = 0x10     ///< \ru Флаг расчета потенциальной энергии кривой. \en Flag for calculation of potential energy of the curve.
  };

private:
  uint8 _options{ 0x0 };   ///< \ru Параметры расчета особых точек кривизны. \en Parameters of curvature special points calculation.

public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
  \param[in] calculateBending  - \ru Флаг, определяющий, искать ли точки перегиба кривизны.
                                 \en Flag defining whether to search for curvature bending points.\~
  \param[in] calculateLocalMin - \ru Флаг, определяющий, искать ли точки локального минимума кривизны.
                                 \en Flag defining whether to search for curvature local minimum points.\~
  \param[in] calculateLocalMax - \ru Флаг, определяющий, искать ли точки локального максимума кривизны.
                                 \en Flag defining whether to search for curvature local maximum points.\~
  \param[in] calculateBreaks   - \ru Флаг, определяющий, искать ли точки разрыва кривизны.
                                 \en Flag defining whether to search for curvature break points.\~
  \param[in] calculateEnergy   - \ru Флаг, определяющий, будет ли вычислено значение потенциальной энергии кривой.\n
                                     Энергия вычисляется как интеграл от квадрата кривизны по длине кривой. \n
                                 \en Flag defining if the potential energy of the curve will be calculated.\n
                                     The energy is calculated as the integral of the square curvature along the length of the curve.\n \~
  */
  MbCurveMinMaxCurvatureParams( bool calculateBending = false, bool calculateLocalMin = false, bool calculateLocalMax = false, 
                                bool calculateBreaks = false, bool calculateEnergy = false );

  /// \ru Деструктор. \en Destructor.
  ~MbCurveMinMaxCurvatureParams() = default;

public:
  /// \ru Искать ли точки перегиба кривизны? \en Are curvature bending points searched for?
  bool GetBendingPointsFlag() const;

  /// \ru Искать ли точки локального минимума кривизны? \en Are curvature local minimum points searched for?
  bool GetLocalMinimumFlag() const;

  /// \ru Искать ли точки локального максимума кривизны? \en Are curvature local maximum points searched for?
  bool GetLocalMaximumFlag() const;

  /// \ru Искать ли точки разрыва кривизны? \en Are curvature break points searched for?
  bool GetBreakPointsFlag() const;

  /// \ru Вычисляется ли потенциальная энергия кривой? \en Is potential energy of curve calculated?
  bool GetCurveEnergyFlag() const;

  OBVIOUS_PRIVATE_COPY( MbCurveMinMaxCurvatureParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Выходные параметры функции поиска особых точек кривизны и вычисления потенциальной энергии кривой.
           \en Output parameters of the function for searching special points of curvature and calculation of potential curve energy. \~
    \ingroup Algorithms_3D
    \warning \ru В разработке.
             \en Under development. \~
*/ // ---
class MATH_CLASS MbCurveMinMaxCurvatureResults
{
private:
  double _maxCurvature;                ///< \ru Наибольшее значение кривизны. \en The greatest value of curvature.
  double _maxCurvatureParam;           ///< \ru Параметр, в которой кривизна принимает наибольшее значение. \en Parameter where the curvature takes the largest value.
  double _minCurvature;                ///< \ru Наименьшее значение кривизны. \en The smallest value of curvature.
  double _minCurvatureParam;           ///< \ru Параметр, в которой кривизна принимает наименьшее значение. \en Parameter where the curvature takes the smallest value.
  double _curveEnergy;                 ///< \ru Значение потенциальной энергии кривой. \en Curve potential energy.
  c3d::DoubleVector _bendingPoints;    ///< \ru Параметры точек перегиба. \en Parameters of bending points.
  c3d::DoubleVector _maxPoints;        ///< \ru Параметры, в которых достигается локальный максимум кривизны по модулю. \en Parameters where the local maximum curvature modulo is reached.
  c3d::DoubleVector _minPoints;        ///< \ru Параметры, в которых достигается локальный минимум кривизны по модулю. \en Parameters where the local minimum curvature modulo is reached.
  c3d::DoublePairsVector _breakPoints; ///< \ru Параметры, в которых кривизна терпит разрыв, содержит точки до и после разрыва. \en Parameters in which curvature breaks. Contains points before and after break.

public:
  /// \ru Конструктор. \en Constructor.
  MbCurveMinMaxCurvatureResults();

  /// \ru Деструктор \en Destructor.
  ~MbCurveMinMaxCurvatureResults() = default;

public:
  /// \ru Получить максимальное значение кривизны. \en Get maximum curvature value.
  double GetMaxCurvatureValue() const { return _maxCurvature; }
  /// \ru Получить параметр кривой с максимальным значением кривизны. \en Get curve parameter of curvature maximum.
  double GetMaxCurvatureParam() const { return _maxCurvatureParam; }
  /// \ru Задать максимальное значение кривизны и соответствующий параметр кривой. \en Get maximum curvature value and corresponding curve parameter.
  void SetMaxCurvature( double t, double maxCurvature ) { _maxCurvatureParam = t; _maxCurvature = maxCurvature; }

  /// \ru Получить минимальное значение кривизны. \en Get minimum curvature value.
  double GetMinCurvatureValue() const { return _minCurvature; }
  /// \ru Получить параметр кривой с минимальным значением кривизны. \en Get curve parameter of curvature minimum.
  double GetMinCurvatureParam() const { return _minCurvatureParam; }
  /// \ru Задать минимальное значение кривизны и соответствующий параметр кривой. \en Get minimum curvature value and corresponding curve parameter.
  void SetMinCurvature( double t, double minCurvature ) { _minCurvatureParam = t; _minCurvature = minCurvature; }

  /// \ru Получить значение потенциальной энергии кривой. \en Get potential energy of curve.
  double GetCurveEnergy() const { return _curveEnergy; }
  /// \ru Задать значение потенциальной энергии кривой. \en Set potential energy of curve.
  void SetCurveEnergy( double energy ) { _curveEnergy = energy; }

  /// \ru Получить параметры точек перегиба кривизны. \en Get curvature bending points parameters.
  const c3d::DoubleVector & GetBendingPoints() const { return _bendingPoints; }
  /// \ru Добавить параметр точки перегиба кривизны. \en Add curvature bending point parameter.
  void AddBendingPoint( double bendingPoint ) { _bendingPoints.push_back( bendingPoint ); }

  /// \ru Получить параметры локальных минимумов кривизны. \en Get curvature local minimum parameters.
  const c3d::DoubleVector & GetLocalMinimumPoints() const { return _minPoints; }
  /// \ru Добавить параметр точки локального минимума кривизны. \en Add curvature local minimum point parameter.
  void AddLocalMinimumPoint( double minPoint ) { _minPoints.push_back( minPoint ); }

  /// \ru Получить параметры локальных максимумов кривизны. \en Get curvature local maximum parameters.
  const c3d::DoubleVector & GetLocalMaximumPoints() const { return _maxPoints; }
  /// \ru Добавить параметр точки локального максимума кривизны. \en Add curvature local maximum point parameter.
  void AddLocalMaximumPoint( double maxPoint ) { _maxPoints.push_back( maxPoint ); }

  /// \ru Получить параметры кривой до и после точек разрыва кривизны. \en Get parameters of curvature break points before and after break.
  const c3d::DoublePairsVector & GetBreakPoints() const { return _breakPoints; }
  /// \ru Добавить параметры до и после точки разрыва кривизны. \en Add parameters before and after curvature break point.
  void AddBreakPoint( double pointBeforeBreak, double pointAfterBreak ) { _breakPoints.emplace_back( pointBeforeBreak, pointAfterBreak ); }

  /// \ru Очистка данных. \en Data cleaning.
  void Clear();

  OBVIOUS_PRIVATE_COPY( MbCurveMinMaxCurvatureResults )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры операции удаления самопересечений.
           \en The parameters for removing self-intersections. \~
  \details \ru Параметры операции удаления самопересечений. \n
           \en The parameters for removing self-intersections. \n \~
  \ingroup Data_Structures
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
struct MATH_CLASS MbRemoveSelfIntParams
{
private:
  MbRect1D              _checksRect;       ///< \ru Область для поиска самопересечений. \en Area to search for self-intersections. \~
  double                _xEpsilon;         ///< \ru Точность построения по первой координате. \en Accuracy of construction by the first coordinate. \~
  double                _yEpsilon;         ///< \ru Точность построения по второй координате. \en Accuracy of construction by the second coordinate. \~
  VERSION               _version;          ///< \ru Версия операции. \en Version of the operation. \~

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbRemoveSelfIntParams()
    : _checksRect (                            )
    , _xEpsilon   ( PARAM_EPSILON              )
    , _yEpsilon   ( PARAM_EPSILON              )
    , _version    ( Math::DefaultMathVersion() )
  {}

  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbRemoveSelfIntParams( double xEps, double yEps )
    : _checksRect (                            )
    , _xEpsilon   ( xEps                       )
    , _yEpsilon   ( yEps                       )
    , _version    ( Math::DefaultMathVersion() )
  {}

  /// \ru Конструктор по области поиска самопересечений. \en Self-intersection search area constructor.
  MbRemoveSelfIntParams( const MbRect1D & initChecksRect, double xEps, double yEps )
    : _checksRect ( initChecksRect             )
    , _xEpsilon   ( xEps                       )
    , _yEpsilon   ( yEps                       )
    , _version    ( Math::DefaultMathVersion() )
  {}

  /// \ru Конструктор для проверки половины эквидистантного контура. \en Constructor for checking half of an equidistant contour.
  MbRemoveSelfIntParams( double xEps, double yEps, VERSION initVersion )
    : _checksRect (             )
    , _xEpsilon   ( xEps        )
    , _yEpsilon   ( yEps        )
    , _version    ( initVersion )
  {}

  /// \ru Деструктор. \en Destructor.
  ~MbRemoveSelfIntParams()
  {}

  /// \ru Получить точность построения по первой координате. \en Get the accuracy of construction by the first coordinate. \~
  double GetXEpsilon() const { return _xEpsilon; }
  /// \ru Получить точность построения по второй координате. \en Get the accuracy of construction by the second coordinate. \~
  double GetYEpsilon() const { return _yEpsilon; }
  /// \ru Получить область для поиска самопересечений. \en Get the area to search for self-intersections. \~
  const MbRect1D & GetChecksRect() const { return _checksRect; }
  /// \ru Версия операции. \en Version of the operation. \~
  VERSION GetVersion() const { return _version; }
  /// \ru Установить версию операции. \en Set the operation version. \~
  void SetVersion( VERSION ver ) { _version = ver; }

OBVIOUS_PRIVATE_COPY( MbRemoveSelfIntParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры операции построения срединной кривой.
           \en The parameters for creating median curve. \~
  \details \ru Параметры операции построения срединной кривой. \n
           \en The parameters for creating median curve. \n \~
  \ingroup Data_Structures
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbMedianCurveParams : public MbPrecision {
private:
  c3d::BoolPair _invNormals; ///< \ru Каждый флаг из пары задает признак инвертирования нормали для соответствующей кривой. \en Each flag from pair indicates inversion of normal for corresponding curve.
  bool _respCurveExt; ///< \ru Флаг определяет, будет ли при необходимости продолжена ответная кривая. \en A flag defines whether the response curve may be extended when necessary.
  bool _smoothing; ///< \ru Флаг определяет, будет ли использовано сглаживание для срединной кривой. \en A flag defines whether smoothing of median curve will be used.
  double _smoothingTolerance; ///< \ru Желаемая точность аппроксимации при использовании сглаживания. \en Desired approximation accuracy when smoothing is used.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbMedianCurveParams()
    : _invNormals   ( false, false )
    , _respCurveExt ( false )
    , _smoothing ( false )
    , _smoothingTolerance ( 0.0 )
  {}

  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbMedianCurveParams( const c3d::BoolPair & invNormals, const bool respCurveExt )
    : _invNormals   ( invNormals )
    , _respCurveExt ( respCurveExt )
    , _smoothing    ( false )
    , _smoothingTolerance ( 0.0 )
  {}

  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbMedianCurveParams( const c3d::BoolPair & invNormals,
                       const bool            respCurveExt,
                       const bool            smoothing,
                       const double          smoothingTolerance )
    : _invNormals   ( invNormals )
    , _respCurveExt ( respCurveExt )
    , _smoothing    ( smoothing )
    , _smoothingTolerance ( smoothingTolerance )
  {}

  /// \ru Выдать значение параметра. \en Get the value of parameter.
  const c3d::BoolPair & GetInvNormals() const { return _invNormals; }

  /// \ru Установить значение параметра. \en Set the value of parameter.
  void GetInvNormals( const c3d::BoolPair & invNormals ) { _invNormals = invNormals; }

  /// \ru Выдать значение параметра. \en Get the value of parameter.
  bool GetRespCurveExt() const { return _respCurveExt; }

  /// \ru Установить значение параметра. \en Set the value of parameter.
  void SetRespCurveExt( const bool respCurveExt ) { _respCurveExt = respCurveExt; }

  /// \ru Выдать значение параметра. \en Get the value of parameter.
  bool GetSmoothing() const { return _smoothing; }

  /// \ru Установить значение параметра. \en Set the value of parameter.
  void SetSmoothing( const bool smoothing ) { _smoothing = smoothing; }

  /// \ru Выдать значение параметра. \en Get the value of parameter.
  double GetSmoothingTolerance() const { return _smoothingTolerance; }

  /// \ru Установить значение параметра. \en Set the value of parameter.
  void SetSmoothingTolerance( const double smoothingTolerance ) { _smoothingTolerance = smoothingTolerance; }
};


//------------------------------------------------------------------------------
/** \brief \ru Результаты построения срединной кривой.
           \en The results of creating median curve. \~
  \details \ru Результаты построения срединной кривой. \n
           \en The results of creating median curve. \n \~
  \ingroup Data_Structures
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbMedianCurveResults : public MbOperationResults {
private:
  c3d::PlaneCurveSPtr _medianCurve; ///< \ru Срединная кривая. \en Median curve.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbMedianCurveResults()
    : _medianCurve ( nullptr )
  {}

  /// \ru Выдать срединную кривую. \en Get the median curve.
  const c3d::PlaneCurveSPtr & GetMedianCurve() const { return _medianCurve; }

  /// \ru Инициализировать данные. \en Initialize data.
  void Init( const c3d::PlaneCurveSPtr & medianCurve, double tolerance ) { _medianCurve = medianCurve; _tolerance = tolerance; }
};
#endif // __OP_CURVE_PARAMETERS_H
