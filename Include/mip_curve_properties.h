////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Массо-центровочные характеристики.
         \en Mass-inertial properties. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MIP_CURVE_PROPERTIES_H
#define __MIP_CURVE_PROPERTIES_H


#include <templ_p_array.h>
#include <templ_s_array.h>
#include <mb_enum.h>


class  MbCurve;


//------------------------------------------------------------------------------
/** \brief \ru Массо-центровочные характеристики кривой.
           \en Mass-inertial properties of curve. \~
  \details \ru Массо-центровочные характеристики кривой.\n
           \en Mass-inertial properties of curve.\n \~
  \ingroup Inertia_Computation
*/
// ---
class MATH_CLASS MIProperties {
public:
  double xc;   ///< \ru Координата х центра тяжести. \en X-coordinate of center of gravity. 
  double yc;   ///< \ru Координата y центра тяжести. \en Y-coordinate of center of gravity. 
  double f;    ///< \ru Площадь. \en Area. 
  double lxx;  ///< \ru Осевой момент инерции относительно оси координат x. \en Centroidal moment of inertia relative to x coordinate axis. 
  double lyy;  ///< \ru Осевой момент инерции относительно оси координат y. \en Centroidal moment of inertia relative to y coordinate axis. 
  double lxy;  ///< \ru Центробежный момент инерции относительно исходных осей координат x и y. \en Product of inertia relative to source coordinate axes x and y. 
  double mxx;  ///< \ru Осевой момент инерции относительно оси координат x (относительно оси, параллельной исходной оси и проходящей через центр тяжести). \en Centroidal moment of inertia relative to x coordinate axis (relative to axis, that is parallel to source axis and is passed through center of gravity). 
  double myy;  ///< \ru Осевой момент инерции относительно оси координат y (относительно оси, параллельной исходной оси и проходящей через центр тяжести). \en Centroidal moment of inertia relative to y coordinate axis (relative to axis, that is parallel to source axis and is passed through center of gravity). 
  double mxy;  ///< \ru Центробежный момент инерции относительно центральных осей (относительно осей, параллельных исходных осям и проходящих через центр тяжести). \en Centroidal moment of inertia relative to central coordinate axes (relative to axes, that are parallel to source axes and are passed through center of gravity). 
  double mxx0; ///< \ru Главные центральные моменты инерции относительно оси координат x. \en Principal central moments of inertia relative to x coordinate axis. 
  double myy0; ///< \ru Главные центральные моменты инерции относительно оси координат y. \en Principal central moments of inertia relative to y coordinate axis. 
  double a;    ///< \ru Угол между первой главной осью и осью x. \en Angle between first principal axis and x-axis. 
        
public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор массо-центровочных характеристик кривой.\n
      Создает объект с нулевыми полями.
             \en Constructor by mass-inertial properties of curve.\n
      Creates an object with the zero fields. \~
  */
  MIProperties() { Init(); }

  /** \brief \ru Инициализация.
             \en Initialization. \~
    \details \ru Инициализация массо-центровочных характеристик кривой.\n
      Обнуляет поля объекта.
             \en Initialize mass-inertial properties of curve.\n
      Reset fields of an object to zero. \~
  */
  void      Init();

  /// \ru Расчет главных моментов инерции. \en Calculate principal moments of inertia. 
  bool      CalculateGeneral(); 

  /// \ru Добавить к полям объекта полей другого объекта. \en Add fields of other object to current.  
  void      operator += ( const MIProperties & other );

  /// \ru Вычесть из полей объекта полей другого объекта. \en Subtract fields of other object from corresponding fields of current one.  
  void      operator -= ( const MIProperties & other );
};


//------------------------------------------------------------------------------
// \ru инициализация \en initialization 
// ---
inline void MIProperties::Init() {
  xc = yc = f = 0.0;
  mxx = myy = mxy = 0.0;
  mxx0 = myy0 = a = 0.0;
  lxx = lyy = lxy = 0.0;
}


//------------------------------------------------------------------------------
/** \brief \ru Определение массо-центровочных характеристик.
           \en Determination of mass-inertial properties. \~
  \details \ru Определение массо-центровочных характеристик кривой.\n
    x0, y0 - координаты центра тяжести,\n
    f      - площадь,\n
    mxx, myy - моменты инерции относительно осей, параллельных осям x и y
               и проходящих через центр тяжести,\n
    mxy - центробежный момент инерции относительно осей, параллельных осям x и y
          и проходящих через центр тяжести,\n
    mxx0, myy0 - главные центральные моменты инерции,\n
    a - угол между первой главной осью и осью x.
           \en Determination of mass-inertial properties of curve.\n
    x0, y0 - center of gravity coordinates,\n
    f      - area,\n
    mxx, myy - moments of inertia relative axes parallel to x and y axes
               and passes through center of gravity,\n
    mxy - product of inertia relative to axes parallel to x and y axes
          and passes through center of gravity,\n
    mxx0, myy0 - principal central moments of inertia,\n
    a - angle between first principal axis and x-axis. \~
  \param[in] curve - \ru Кривая.
                     \en Curve. \~
  \param[out]mp    - \ru Результат - массоцентровочные характеристики кривой.
                     \en Mass-inertial properties of curve as result. \~
  \param[in] deviateAngle - \ru Угловое отклонение касательных кривой в соседних точках на участке численного интегрирования.
                            \en The angular deviation of the curve in the neighboring points on the region of numerical integration. \~
  \ingroup Inertia_Computation
*/
// ---
MATH_FUNC (void) MassInertiaProperties( const MbCurve *      curve, 
                                              MIProperties & mp, 
                                              double         deviateAngle = Math::deviateSag );


//------------------------------------------------------------------------------
/** \brief \ru Определение массо-центровочных характеристик.
           \en Determination of mass-inertial properties. \~
  \details \ru Определение массо-центровочных характеристик кривых.
           \en Determine mass-inertial properties of curves. \~
  \param[in]  curves - \ru Набор кривых.
                       \en A set of curves. \~
  \param[in]  bodies - \ru Набор флагов для каждой кривой:\n
                       если true  - массо-центровочные характеристики кривой прибавляются к общему результату,\n
                       если false - массо-центровочные характеристики кривой вычитаются из общего результата.\n
                       Количество флагов должно совпадать с количеством кривых.
                       \en Set of flags for each curve:\n
                       if true then mass-inertial properties of curve are added to total result,\n
                       if false then mass-inertial properties of curve are subtracted from total result.\n
                       Count of flags must be equal to count of curves. \~
  \param[out] mp     - \ru Результат - суммарные массо-центровочные характеристики.
                       \en Total mass-inertial properties as result. \~
  \param[in] deviateAngle - \ru Угловое отклонение касательных кривой в соседних точках на участке численного интегрирования.
                            \en The angular deviation of the curve in the neighboring points on the region of numerical integration. \~
  \ingroup Inertia_Computation
*/
// ---
MATH_FUNC (void) MassInertiaProperties( const RPArray<const MbCurve> & curves, 
                                        const c3d::BoolVector &        bodies,
                                              MIProperties &           mp, 
                                              double                   deviateAngle = Math::deviateSag );


//------------------------------------------------------------------------------
/** \brief \ru Объемные массо-центровочные характеристки.
           \en Volume mass-inertial properties. \~
  \details \ru Объемные массо-центровочные характеристки. \n
           \en Volume mass-inertial properties. \n \~
  \ingroup Inertia_Computation
*/
// ---
class MATH_CLASS MI3DProperties {
public :
  double r;   ///< \ru Плотность. \en Density. 
  double m;   ///< \ru Масса. \en Mass. 
  double v;   ///< \ru Объем. \en Volume. 
  double xc;  ///< \ru Статический момент относительно плоскости YOZ (после CalculateGeneral - координата X центра тяжести). \en Static moment relative to YOZ plane (after CalculateGeneral - X-coordinate of center of gravity). 
  double yc;  ///< \ru Статический момент относительно плоскости XOZ (после CalculateGeneral - координата Y центра тяжести). \en Static moment relative to XOZ plane (after CalculateGeneral - Y-coordinate of center of gravity). 
  double zc;  ///< \ru Статический момент относительно плоскости XOY (после CalculateGeneral - координата Z центра тяжести). \en Static moment relative to XOY plane (after CalculateGeneral - Z-coordinate of center of gravity). 
  double lx;  ///< \ru Осевой момент инерции относительно оси координат x. \en Centroidal moment of inertia relative to x coordinate axis. 
  double ly;  ///< \ru Осевой момент инерции относительно оси координат y. \en Centroidal moment of inertia relative to y coordinate axis. 
  double lz;  ///< \ru Осевой момент инерции относительно оси координат z. \en Centroidal moment of inertia relative to z coordinate axis. 
  double lxy; ///< \ru Центробежный момент инерции в плоскости XOY. \en Product of inertia in XOY plane. 
  double lxz; ///< \ru Центробежный момент инерции в плоскости XOZ. \en Product of inertia in XOZ plane. 
  double lyz; ///< \ru Центробежный момент инерции в плоскости YOZ. \en Product of inertia in YOZ plane. 
  double jxx; ///< \ru Плоскостной момент инерции (интегралы инерции) относительно оси координат x. \en Planar moment of inertia (integrals of inertia) relative to x coordinate axis. 
  double jyy; ///< \ru Плоскостной момент инерции (интегралы инерции) относительно оси координат y. \en Planar moment of inertia (integrals of inertia) relative to y coordinate axis. 
  double jzz; ///< \ru Плоскостной момент инерции (интегралы инерции) относительно оси координат z. \en Planar moment of inertia (integrals of inertia) relative to z coordinate axis. 
  double jx;  ///< \ru Осевой момент инерции относительно оси координат x. \en Centroidal moment of inertia relative to x coordinate axis. 
  double jy;  ///< \ru Осевой момент инерции относительно оси координат y. \en Centroidal moment of inertia relative to y coordinate axis. 
  double jz;  ///< \ru Осевой момент инерции относительно оси координат z. \en Centroidal moment of inertia relative to z coordinate axis. 
  double jxy; ///< \ru Центробежный момент инерции в плоскости XOY. \en Product of inertia in XOY plane. 
  double jxz; ///< \ru Центробежный момент инерции в плоскости XOZ. \en Product of inertia in XOZ plane. 
  double jyz; ///< \ru Центробежный момент инерции в плоскости YOZ. \en Product of inertia in YOZ plane. 

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор объемных массо-центровочных характеристик.\n
      Создает объект с нулевыми полями.
             \en Constructor of mass-inertial properties.\n
      Creates an object with the zero fields. \~
  */
  MI3DProperties() { Init(); }

  /** \brief \ru Инициализация.
             \en Initialization. \~
    \details \ru Инициализация объемных массо-центровочных характеристик.\n
      Создает объект с нулевыми полями.
             \en Initialization of mass-inertial properties.\n
      Creates an object with the zero fields. \~
  */
  void      Init();

  ///< \ru Расчет главных моментов инерции. \en Calculate principal moments of inertia. 
  void      CalculateGeneral(); 

  /// \ru Добавить к полям объекта полей другого объекта. \en Add fields of other object to current.  
  void      operator += ( const MI3DProperties & other );
  /// \ru Вычесть из полей объекта полей другого объекта. \en Subtract fields of other object from corresponding fields of current one.  
  void      operator -= ( const MI3DProperties & other );
};


//------------------------------------------------------------------------------
// \ru инициализация \en initialization 
// ---
inline void MI3DProperties::Init() {
  r = 1;
  m = v = 0.0;
  xc  = yc  = zc  = 0.0; // \ru центр тяжести \en center of gravity 
  jx  = jy  = jz  = 0.0; // \ru осевые моменты инерции \en axial moments of inertia 
  jxx = jyy = jzz = 0.0; // \ru плоскостные моменты инерции \en planar moments of inertia 
  jxy = jxz = jyz = 0.0; // \ru центробежные моменты инерции \en product of inertia 
  lx  = ly  = lz  = 0.0; // \ru осевые моменты инерции \en axial moments of inertia 
  lxy = lxz = lyz = 0.0; // \ru центробежные моменты инерции \en product of inertia 
}

 
//------------------------------------------------------------------------------
/** \brief \ru Описание формы контура.
           \en Contour shape description. \~
  \details \ru Описание формы контура. \n
           \en Contour shape description. \n \~
  \ingroup Inertia_Computation
*/
// ---
struct MATH_CLASS FormDefinition {
  bool      body;     ///< \ru Признак тела. \en Solid attribute. 
  double    density;  ///< \ru Плотность. \en Density. 
  double    par;      ///< \ru Параметр (угол раствора или толщина). \en Parameter (apex angle or thickness). 

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по признаку тела,
      плотности и параметру.
             \en Constructor by solid attribute,
      density and parameter. \~
    \param[in] b - \ru Признак тела.\n         
               если true  - массо-центровочные характеристики кривой прибавляются к общему результату,\n
               если false - массо-центровочные характеристики кривой вычитаются из общего результата.\n
                   \en Solid attribute.\n         
               if true then mass-inertial properties of curve are added to total result,\n
               if false then mass-inertial properties of curve are subtracted from total result.\n \~
    \param[in] d - \ru Плотность.
                   \en Density. \~
    \param[in] p - \ru Параметр (угол раствора или толщина).
                   \en Parameter (apex angle or thickness). \~
  */
  FormDefinition( bool b, double d, double p ) 
    : body( b )
    , density( d )
    , par( p )
  {  
  }

  /// \ru Конструктор копирования. \en Copy constructor. 
  FormDefinition( const FormDefinition & other ) 
    : body( other.body )
    , density( other.density )
    , par( other.par )
  {   
  }
};


//------------------------------------------------------------------------------
/** \brief \ru Расчет объемных массо-центровочных характеристик.
           \en Calculate volume mass-inertial properties. \~
  \details \ru Расчет объемных массо-центровочных характеристик.\n
           \en Calculate volume mass-inertial properties.\n \~
  \param[in]  revolution - \ru Если true - параметр описания формы контура считается углом,\n
                           если false - толщиной.
                           \en If true then shape description parameter treats as angle,\n
                           If false then treats as thickness. \~
  \param[in]  curves     - \ru Набор кривых.
                           \en Set of curves. \~
  \param[in]  formes     - \ru Описание формы для каждой кривой.\n
                           Количество элементов должно совпадать с количеством кривых.
                           \en Each curve shape description.\n
                           Count of elements must be equal to count of curves. \~
  \param[out] mp         - \ru Результат - массо-центровочные характеристики.
                           \en Mass-inertial properties as result. \~
  \param[in] deviateAngle - \ru Угловое отклонение касательных кривой в соседних точках на участке численного интегрирования.
                            \en The angular deviation of the curve in the neighboring points on the region of numerical integration. \~
  \ingroup Inertia_Computation
*/
// ---
MATH_FUNC (void) MassInertia3DProperties( bool revolution, 
                                          const RPArray<MbCurve> & curves, 
                                          const SArray<FormDefinition> & formes,
                                          MI3DProperties & mp,
                                          double deviateAngle = Math::deviateSag );

             
#endif // __MIP_CURVE_PROPERTIES_H
