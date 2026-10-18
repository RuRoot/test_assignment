////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Константы и переменные.
         \en Constants and variables. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_VARIABLES_H
#define __MB_VARIABLES_H

#include <math_define.h>
#include <tool_uuid.h>
#include <tool_mutex.h>
#include <iostream>


/** \ru \name Общие константы
    \en \name Common constants
    \{ */

constexpr double MB_INFINITY = std::numeric_limits<double>::infinity();  ///< \ru Значение, обозначающее бесконечность для double. \en Value representing infinity for double.
constexpr double MB_QNAN     = std::numeric_limits<double>::quiet_NaN(); ///< \ru "Тихое" не число. \en Quiet Not-A-Number.

constexpr double MB_MAXDOUBLE = 1.0E+300; ///< \ru Максимальное значение double 1.7976931348623158E+308. \en Maximum value of double 1.7976931348623158E+308.
constexpr double MB_MINDOUBLE = 1.0E-300; ///< \ru Минимальное значение double  2.2250738585072014E-308. \en Minimum value of double  2.2250738585072014E-308.

// \ru Математические константы, округленные до 21 значащей цифры, определены в math.h \en Mathematical constants rounded to 21 significant digits, defined in math.h  
#if !defined( _MATH_DEFINES_DEFINED )
#ifdef C3D_WINDOWS// _MSC_VER // \ru Константы определены в math.h [Linux] \en Constants defined in math.h [Linux] 
constexpr double M_E         = 2.71828182845904523536;   ///< \ru Экспонента. \en Exponent.
constexpr double M_LOG2E     = 1.44269504088896340736;   ///< \ru Логарифм M_E по основанию 2. \en Logarithm M_E to base 2.
constexpr double M_LOG10E    = 0.434294481903251827651;  ///< \ru Логарифм M_E по основанию 10. \en Logarithm M_E to base 10.
constexpr double M_LN2       = 0.693147180559945309417;  ///< \ru Натуральный логарифм 2. \en Natural logarithm 2.
constexpr double M_PI_4      = 0.785398163397448309616;  ///< M_PI / 4.0
constexpr double M_1_PI      = 0.318309886183790671538;  ///< 1.0 / M_PI
constexpr double M_2_PI      = 0.636619772367581343076;  ///< 2.0 / M_PI
constexpr double M_SQRT1_2   = 0.707106781186547524401;  ///< \ru Корень из одной второй, sqrt(1/2). \en Root of one half, sqrt(1/2).
#else // C3D_WINDOWS
#ifndef   M_E 
constexpr double M_E         = 2.71828182845904523536;   ///< \ru Экспонента. \en Exponent.
#endif //  M_E
#ifndef   M_LOG2E
constexpr double M_LOG2E     = 1.44269504088896340736;   ///< \ru Логарифм M_E по основанию 2. \en Logarithm M_E to base 2.
#endif //  M_LOG2E
#ifndef   M_LOG10E
constexpr double M_LOG10E    = 0.434294481903251827651;  ///< \ru Логарифм M_E по основанию 10. \en Logarithm M_E to base 10.
#endif //  M_LOG10E
#ifndef   M_LN2
constexpr double M_LN2       = 0.693147180559945309417;  ///< \ru Натуральный логарифм 2. \en Natural logarithm 2.
#endif //  M_LN2
#ifndef   M_PI_4
constexpr double M_PI_4      = 0.785398163397448309616;  ///< M_PI / 4.0
#endif //   M_PI_4
#ifndef  M_1_PI
constexpr double M_1_PI      = 0.318309886183790671538;  ///< 1.0 / M_PI
#endif //  M_1_PI
#ifndef   M_2_PI 
constexpr double M_2_PI      = 0.636619772367581343076;  ///< 2.0 / M_PI
#endif //  M_2_PI 
#ifndef  M_SQRT1_2
constexpr double M_SQRT1_2   = 0.707106781186547524401;  ///< \ru Корень из одной второй, sqrt(1/2). \en Root of one half, sqrt(1/2).
#endif //  M_SQRT1_2
#endif // C3D_WINDOWS

#ifndef M_LN10
constexpr double M_LN10      = 2.30258509299404568402;   ///< \ru Натуральный логарифм 10, ln(10). \en Natural logarithm 10, ln(10).
#endif
#ifndef M_PI
constexpr double M_PI        = 3.14159265358979323846;   ///< \ru Отношение длины окружности к её диаметру, pi. \en Relation between circle length and its diameter, pi.
#endif
#ifndef M_PI_2
constexpr double M_PI_2      = 1.57079632679489661923;   ///< M_PI / 2.0
#endif
#ifndef M_2_SQRTPI
constexpr double M_2_SQRTPI  = 1.12837916709551257390;   ///< \ru Два разделить на корень из числа пи, 2/sqrt(pi). \en Two divided by root of pi, 2/sqrt(pi).
#endif
#ifndef M_SQRT2
constexpr double M_SQRT2     = 1.41421356237309504880;   ///< \ru Корень из двух, sqrt(2). \en Root of two, sqrt(2).
#endif
#endif // _MATH_DEFINES_DEFINED

constexpr double M_1_SQRTPI      = 0.564189583547756286948;  ///< \ru Единица, деленная на корень из числа пи, 1/sqrt(pi). \en One divided by root of pi, 1/sqrt(pi).
constexpr double M_FI            = 1.61803398874989484;      ///< \ru Число золотого сечения, 1/M_FI = 0.6180339887499. \en Golden ratio, 1/M_FI = 0.6180339887499.
constexpr double MM_INCH         = 25.4;                     ///< \ru Количество миллиметров в дюйме. \en Millimeters per inch.


constexpr double EPSILON         = 1E-10;  ///< \ru Погрешность. \en Tolerance.
constexpr double MAXIMON         = 1E+10;  ///< \ru 10 в 10-й степени. \en 10 to the power of 10.
constexpr double MAX_OVERALL_DIM = 1E+12;  ///< \ru Максимальное значение габарита. \en Maximal value of bounding box.

constexpr double DETERMINANT_MAX = 1E+137; ///< \ru Максимальная величина. \en Maximal value.
constexpr double DETERMINANT_MIN = 1E-171; ///< \ru Минимальная величина. \en Minimal value.

constexpr double NULL_EPSILON      = 1E-30; ///< \ru Погрешность для проверки на равенство нулю. \en Tolerance for equality to zero.
constexpr double NULL_REGION       = 1E-20; ///< \ru Погрешность для проверки на равенство нулю. \en Tolerance for equality to zero.

constexpr double DOUBLE_EPSILON    = 1E-16; ///< \ru Погрешность. \en Tolerance.
constexpr double DOUBLE_REGION     = 1E-15; ///< \ru Погрешность. \en Tolerance.
constexpr double EXTENT_EQUAL      = 1E-14; ///< \ru Погрешность. \en Tolerance.
constexpr double EXTENT_EPSILON    = 1E-12; ///< \ru Погрешность. \en Tolerance.
constexpr double EXTENT_REGION     = 1E-11; ///< \ru Погрешность. \en Tolerance.
constexpr double LENGTH_EPSILON    = 1E-10; ///< \ru Погрешность длины. \en Tolerance for length.
constexpr double LENGTH_REGION     = 1E-9;  ///< \ru Погрешность региона. \en Tolerance for region.

constexpr double METRIC_EPSILON    = 1E-8;  ///< \ru Погрешность расстояния в итерационных функциях. \en Tolerance for distance in iterative functions.
constexpr double METRIC_REGION     = 1E-7;  ///< \ru Неразличимая метрическая область. \en Indistinguishable metric region.
constexpr double METRIC_PRECISION  = 1E-6;  ///< \ru Метрическая погрешность. \en Metric tolerance.
constexpr double METRIC_ACCURACY   = 1E-5;  ///< \ru Наибольшая метрическая погрешность (абсолютная точность в мм ("размер" атома 5e-8 мм)). \en The largest metric tolerance (absolute tolerance expressed in mm ("size" of atom is 5e-8 mm)).
constexpr double METRIC_NEAR       = 1E-4;  ///< \ru Метрическая близость. \en Metric proximity tolerance.

constexpr double PARAM_EPSILON     = 1E-8;  ///< \ru Погрешность параметра в итерационных функциях. \en Tolerance for parameter in iterative functions.
constexpr double PARAM_REGION      = 1E-7;  ///< \ru Неразличимая параметрическая область. \en Indistinguishable parametric region.
constexpr double PARAM_PRECISION   = 1E-6;  ///< \ru Параметрическая погрешность. \en Parametric tolerance.
constexpr double PARAM_ACCURACY    = 1E-5;  ///< \ru Наибольшая параметрическая погрешность. \en The largest parametric tolerance.
constexpr double PARAM_NEAR        = 1E-4;  ///< \ru Параметрическая близость. \en Parametric proximity.

constexpr ptrdiff_t UNDEFINED_INT_T  = SYS_MIN_ST;  ///< \ru Неопределенный int. \en Undefined int.
constexpr size_t    FAIR_MAX_DEGREE  = 11;          ///< \ru Максимальный порядок NURBS при аппроксимации. \en Maximum degree of the NURBS approximation.

constexpr double M_PI2         = M_PI * 2.0;           ///< \ru Отношение длины окружности к её радиусу, 2.0 * M_PI, 6.28318530717958647692 \en Relation between circle length and its radius, 2.0 * M_PI, 6.28318530717958647692
constexpr double M_DEGRAD      = M_PI / 180.0;         ///< \ru Коэффициент перевода градусов в радианы. \en Factor of conversion from degrees to radians.
constexpr double M_RADDEG      = 180.0 / M_PI;         ///< \ru Коэффициент перевода радиан в градусы. \en Factor of conversion from radians to degrees.
constexpr double UNDEFINED_DBL = -MB_MAXDOUBLE;        ///< \ru Неопределенный double. \en Undefined double.
constexpr double DEVIATION_SAG = M_PI * 0.04;          ///< \ru Угловая толерантность. \en Angular tolerance.
constexpr double ANGLE_EPSILON = PARAM_EPSILON * M_PI; ///< \ru Погрешность угла. \en Angular tolerance.
constexpr double ANGLE_REGION  = ANGLE_EPSILON * 40;   ///< \ru Погрешность угла, при которой углы считаются равными. \en Angular tolerance for equality of angles.


namespace c3d // namespace C3D
{

constexpr double  METRIC_PORTION  = 0.075;   ///< \ru Доля расстояния между контрольными точками. \en Proportion of distance between control points.
constexpr double  METRIC_DELTA    = 0.05;    ///< \ru Величина отшагивания. \en Metric offset.
constexpr double  PARAM_DELTA_MIN = 0.005;   ///< \ru Минимальная  доля приращения параметра. \en Minimal portion of parameter increment.
constexpr double  PARAM_DELTA_MAX = 1.0;     ///< \ru Максимальная доля приращения параметра. \en Maximal portion of parameter increment.

constexpr double  MIN_LENGTH      = 1.0E-4;  ///< \ru Минимальная  длина объекта. \en Minimal object length.
constexpr double  MAX_LENGTH      = 5.0E+7;  ///< \ru Максимальная длина объекта. \en Maximal object length.
constexpr double  MIN_RADIUS      = 1.0E-4;  ///< \ru Минимальный  радиус объекта. \en Minimal object radius.
constexpr double  MAX_RADIUS      = 2.5E+7;  ///< \ru Максимальный радиус объекта. \en Maximal object radius.

constexpr double  DELTA_MIN       = 1E-3; ///< \ru Коэффициент уменьшения. \en Reduction factor.
constexpr double  DELTA_MID       = 1E-2; ///< \ru Коэффициент уменьшения. \en Reduction factor.
constexpr double  DELTA_MOD       = 1E-1; ///< \ru Коэффициент уменьшения. \en Reduction factor.
constexpr double  DELTA_MAX       = 1E+3; ///< \ru Коэффициент увеличения. \en Magnification factor.
constexpr double  POWER_1         = 1E+1; ///< \ru Коэффициент увеличения. \en Magnification factor.
constexpr double  POWER_2         = 1E+2; ///< \ru Коэффициент увеличения. \en Magnification factor.
constexpr double  POWER_3         = 1E+3; ///< \ru Коэффициент увеличения. \en Magnification factor.
constexpr double  POWER_4         = 1E+4; ///< \ru Коэффициент увеличения. \en Magnification factor.
constexpr double  POWER_5         = 1E+5; ///< \ru Коэффициент увеличения. \en Magnification factor.

constexpr double  ONE_THIRD       = 0.33333333333333333333;  ///< 1/3.
constexpr double  TWO_THIRD       = 0.66666666666666666666;  ///< 2/3.
constexpr double  ONE_SIXTH       = 0.166666666666666666667; ///< 1/6.
constexpr double  ONE_FIFTH       = 0.2;                     ///< 1/5.
constexpr double  TWO_FIFTH       = 0.4;                     ///< 2/5.
constexpr double  ONE_QUARTER     = 0.25;                    ///< 1/4.
constexpr double  ONE_HALF        = 0.5;                     ///< 1/2.
constexpr double  ONE_SEVENTH     = 0.14285714285714285714;  ///< 1/7.
constexpr double  ONE_EIGHTH      = 0.125;                   ///< 1/8.

// \ru Способы построения поверхности сопряжения (скругления или фаски). \en Ways for construction of smooth surface (fillet or chamfer). 
constexpr double  _CONIC_MIN_     = 0.05; ///< \ru Минимальный  коэффициент полноты сечения поверхности сопряжения (при 0.5 - парабола, меньше - эллипс). \en Minimum factor of smooth surface section completeness (0.5 for parabola, less for ellipse).
constexpr double  _CONIC_MAX_     = 0.95; ///< \ru Максимальный коэффициент полноты сечения поверхности сопряжения (при 0.5 - парабола, больше - гипербола). \en Maximum factor of smooth surface section completeness (0.5 for parabola, greater for hyperbola).
constexpr double  _ARC_           = 0.0;  ///< \ru Коэффициент полноты сечения поверхности скругления при u = const соответствует дуге окружности. \en Factor of smooth surface section completeness in case of u = const corresponds to circle arc.

constexpr int32   TEN             =   10; ///< \ru Число 10. \en Number 10.
constexpr int32   TWENTY          =   20; ///< \ru Число 20. \en Number 20.
constexpr int32   TESSERA_MAX     = 4000; ///< \ru Максимальное количество ячеек в строке и ряду триангуляционной сетки. \en Maximum count of cell in rows and columns for triangulation grid.
constexpr int32   COUNT_MAX       =  512; ///< \ru Коэффициент увеличения. \en Magnification factor.
constexpr int32   COUNT_MID       =  256; ///< \ru Коэффициент увеличения. \en Magnification factor.
constexpr int32   COUNT_MIN       =  128; ///< \ru Коэффициент увеличения. \en Magnification factor.
constexpr int32   COUNT_BIN       =   64; ///< \ru Уровень вложенности. \en Inclusion level.
constexpr int32   WIRE_MAX        =  256; ///< \ru Максимальное количество линий отрисовочной сетки. \en The maximum number of mesh lines.

constexpr int32   ITERATE_COUNT   =   16; ///< \ru Число приближений в итерационном методе. \en Number of approximations in iterative method.
constexpr int32   ITERATE_LIMIT   =   32; ///< \ru Количество итераций для построения касательных окружностей. \en Count of iterations for construction of tangent circles.

constexpr int32   NEWTON_COUNT    =    8; ///< \ru Число приближений в итерационном методе. \en Number of approximations in iterative method.
constexpr int32   NEWTON_COUNT_2X =   16; ///< \ru Число приближений в итерационном методе. \en Number of approximations in iterative method.
constexpr int32   NEWTON_COUNT_3X =   24; ///< \ru Число приближений в итерационном методе. \en Number of approximations in iterative method.
constexpr int32   NEWTON_COUNT_4X =   32; ///< \ru Число приближений в итерационном методе. \en Number of approximations in iterative method.
constexpr int32   NEWTON_COUNT_8X =   64; ///< \ru Число приближений в итерационном методе. \en Number of approximations in iterative method.

constexpr int32   LIMIT_COUNT     =    4; ///< \ru Число приближений в итерационном методе. \en Number of approximations in iterative method.
constexpr int32   COUNT_DELTA     =   10; ///< \ru Коэффициент увеличения или уменьшения. \en Reduction or magnification factor.
constexpr int32   COUNT_DELTA_2X  =   20; ///< \ru Коэффициент увеличения или уменьшения. \en Reduction or magnification factor.
constexpr int32   ITEMS_COUNT     =   12; ///< \ru Число точек в шаговом методе. \en Number of points in step method.

constexpr int32   BEZIER_DEGREE   =    4; ///< \ru Порядок Безье-сплайна по умолчанию. \en Default degree of Bezier-spline.
constexpr int32   NURBS_DEGREE    =    4; ///< \ru Порядок NURBS по умолчанию. \en Degree of NURBS.

constexpr int32   NURBS_POINTS_COUNT      =   6; ///< \ru Число точек для NURBS по умолчанию для прямого редактирования. \en Default number of points for NURBS direct editing.
constexpr int32   NURBS_POINTS_MAX_COUNT  = 100; ///< \ru Максимальное число точек для NURBS по умолчанию для прямого редактирования. \en Default maximum number of points for NURBS direct editing.
constexpr int32   APPROX_POINTS_MUL_COEFF =   3; ///< \ru Коэффициент увеличения количества точек для метода наименьших квадратов. \en Factor of points count incrementing in method of least squares.

constexpr int32   SPACE_DIM       =    3;   ///< \ru Размерность 3D-пространства. \en Dimension of 3D space.

constexpr int32   TRT_FREE        =    0; ///< \ru Сопряжение отсутствует. \en No conjugation.
constexpr int32   TRT_TANGENT     =    1; ///< \ru Сопряжение по касательной. \en Tangent conjugation.
constexpr int32   TRT_NORMAL      =    2; ///< \ru Сопряжение по нормали. \en Normal conjugation.


/** \ru \name Способ информирования о нарушении требований.~
    \en \name Assert violation notification.
*/
enum eAssertViolationNotify {
  avn_Mute,         ///< \ru Не сообщать о нарушении требований. \en Mute assert violations.
  avn_CERR,         ///< \ru Выводить сообщение в поток ошибок. \en Write message into error stream.
  avn_ASSERT        ///< \ru Обработка макросом ASSERT. \en ASSERT macro application.
};

} // namespace C3D


constexpr float MB_AMBIENT     = 0.4f;     ///< \ru Коэффициент рассеянного освещения (фон). \en Coefficient of backlighting.
constexpr float MB_DIFFUSE     = 0.7f;     ///< \ru Коэффициент диффузного отражения. \en Coefficient of diffuse reflection.
constexpr float MB_SPECULARITY = 0.8f;     ///< \ru Коэффициент зеркального отражения. \en Coefficient of specular reflection.
constexpr float MB_SHININESS   = 50.0f;    ///< \ru Блеск (показатель степени в законе зеркального отражения). \en Shininess (index according to the law of specular reflection).
constexpr float MB_OPACITY     = 1.0f;     ///< \ru Коэффициент суммарного отражения (коэффициент непрозрачности). \en Coefficient of total reflection (opacity coefficient).
constexpr float MB_EMISSION    = 0.0f;     ///< \ru Коэффициент излучения. \en Emissivity coefficient.

constexpr uint32 MB_DEFCOLOR   = 0x7F7F7F; ///< \ru Цвет по умолчанию при импорте и экспорте (серый). \en Default color for import and export (grey).
constexpr uint32 MB_C3DCOLOR   = 0xFF7F00; ///< \ru Цвет по умолчанию для геометрических объектов. \en Default color for geometric objects.

constexpr uint32 MB_AMBIENT_UINT32     = 0xFF666666; ///< \ru Коэффициент рассеянного освещения (фон). Эквивалент MB_AMBIENT. \en Coefficient of backlighting (equivalent of MB_AMBIENT).
constexpr uint32 MB_SPECULARITY_UINT32 = 0xFFCCCCCC; ///< \ru Коэффициент зеркального отражения (эквивалент MB_SPECULARITY). \en Coefficient of specular reflection (equivalent of MB_SPECULARITY).
constexpr uint32 MB_EMISSION_UINT32    = 0xFF000000; ///< \ru Коэффициент излучения (эквивалент MB_SPECULARITY). \en Emissivity coefficient (equivalent of MB_SPECULARITY).

constexpr uint8 MB_OPACITY_UINT8    = 255; ///< \ru Коэффициент суммарного отражения (коэффициент непрозрачности). Эквивалент MB_OPACITY. \en Coefficient of total reflection (equivalent of MB_OPACITY).
constexpr uint8 MB_SHININESS_UINT8  = 50;  ///< \ru Блеск (показатель степени в законе зеркального отражения). Эквивалент MB_SHININESS. \en Shininess (index according to the law of specular reflection). Equivalent of MB_SHININESS.

/// \ru Битовые флаги для матрицы и локальной системы координат. \en Bit flags for matrix and local coordinate system.  
constexpr uint8 MB_IDENTITY    = 0x00;     ///< \ru Единичная матрица. \en Identity.
constexpr uint8 MB_TRANSLATION = 0x01;     ///< \ru Присутствует смещение. \en Translation.
constexpr uint8 MB_ROTATION    = 0x02;     ///< \ru Присутствует вращение. \en Rotation.
constexpr uint8 MB_SCALING     = 0x04;     ///< \ru Присутствует масштабирование (компонент не 1.0). \en Scaling (factor is not equal to 1.0).
constexpr uint8 MB_REFLECTION  = 0x08;     ///< \ru Присутствует зеркальная инверсия. \en Reflection.
constexpr uint8 MB_LEFT        = 0x08;     ///< \ru Присутствует зеркальная инверсия (признак левой системы координат). \en Reflection (left coordinate system attribute).
constexpr uint8 MB_ORTOGONAL   = 0x10;     ///< \ru Присутствует ортогональность, взводится только в случае аффинности. \en Orthogonality, is set up in case of affinity.
constexpr uint8 MB_AFFINE      = 0x20;     ///< \ru Отсутствует ортогональность и нормированность (аффинное преобразование). \en Absence of orthogonality and normalization (affine transformation).
constexpr uint8 MB_PERSPECTIVE = 0x40;     ///< \ru Присутствует вектор перспективы (не нулевой). \en Vector of perspective (non-zero).
constexpr uint8 MB_UNSET       = 0x80;     ///< \ru Битовые флаги не установлены. \en Bit flags not set.

/** \} */


class  MbRefItem;
class            VersionContainer;

// \ru Управление реализацией переменных в Math. \en Managing variables implementation in Math. 
#define USE_VAR_CLASSES

//------------------------------------------------------------------------------
/** \brief \ru Общие статические данные алгоритмов и функций.
           \en Common static data of algorithms and functions. \~
  \details \ru Общие статические данные содержат константы, которые используются 
            в вычислениях как предельные величины. \n
            Статические данные не подлежат изменению. \n
           \en Common static data contains constants used 
            in computations as limit quantities. \n
            Static data cannot be changed. \n \~
  \ingroup Base_Items
*/
// ---
class MATH_CLASS Math {
public:

  /// \ru Установить значения переменных по умолчанию. \en Set default values of variables. 
  static void   SetDefaultValues();

  /// \ru Установить значение переменной.
  /// Необходимо учитывать, что изменение глобальных переменных может привести
  /// к непредсказуемым результатам при распараллеливании вычислений.
  /// \en Set value of variable. 
  /// It is necessary to keep in mind that modification of global variable could lead to
  /// unpredictable results when using parallel calculations.
  static void   SetUserValue( int index, double value );

#ifdef USE_VAR_CLASSES
  //------------------------------------------------------------------------------
  // \ru Классы переменных, значения которых могут меняться с помощью функций SetVarValue(), RestoreVarValue() и SetDefaultValues().
  // \en Classes of variables which can be changed using SetVarValue(), RestoreVarValue() and SetDefaultValues() functions.
  //---
  class MATH_CLASS DoubleVariable
  {
  protected:
    std::vector<double> _values; ///< \ru Значения переменной для потоков. \en The variable values for threads.
  public:
    DoubleVariable( double value );

    const double & operator()()            { if (_values.size() == 1 ) return _values[0]; return Get(); }
          double   operator()()      const { if (_values.size() == 1 ) return _values[0]; return Get(); }
                   operator double() const { if (_values.size() == 1 ) return _values[0]; return Get(); }

    void      SetVarValue( double val );
    void      RestoreVarValue();

    friend void Math::SetDefaultValues();

  private:
    DoubleVariable();
    DoubleVariable & operator=( double v );
    DoubleVariable & operator=( const DoubleVariable & v );
    const double & Get() const;
    double & Get();
  };

  class MATH_CLASS SizeVariable
  {
  protected:
    std::vector<size_t> _values; ///< \ru Значения переменной для потоков. \en The variable values for threads.
  public:
    SizeVariable( size_t val );

    const size_t & operator()()            { if ( _values.size() == 1 ) return _values[0]; return Get(); }
          size_t   operator()()      const { if ( _values.size() == 1 ) return _values[0]; return Get(); }
                   operator size_t() const { if ( _values.size() == 1 ) return _values[0]; return Get(); }

    void      SetVarValue( size_t val );
    void      RestoreVarValue();

    friend void Math::SetDefaultValues();

  private:
    SizeVariable();
    SizeVariable & operator=( size_t v );
    SizeVariable & operator=( const SizeVariable & v );
    const size_t & Get() const;
    size_t & Get();
  };

  //------------------------------------------------------------------------------
  // \ru Класс переменных, которые могут изменяться только в группе с помощью функций SetUserValue() и SetDefaultValues().
  // \en Class of variables which can be changed only in a group using SetUserValue() and SetDefaultValues() functions.
  //---
  class MATH_CLASS GroupVariable
  {
    DoubleVariable _var;
  public:
    GroupVariable( double val ) : _var( val ) {}

    const double & operator()()            { return _var(); }
          double   operator()()      const { return _var(); }
                   operator double() const { return _var(); }

    friend void Math::SetUserValue( int index, double value );
    friend void Math::SetDefaultValues();

  protected:
    void SetVarValue( double val ) { _var.SetVarValue( val ); }
    void RestoreVarValue();

  private:
    GroupVariable();
    GroupVariable & operator=( double v );
    GroupVariable & operator=( const GroupVariable & v );
  };
#endif

  //------------------------------------------------------------------------------
  // \ru Константы. \en Constants. 
  //---
public:
  static const double PI2;                ///< \ru Отношение длины окружности к её радиусу. \en Relation between circle length and its radius. 
  static const double invPI2;             ///< \ru Отношение радиуса окружности к её длине. \en Relation between circle radius and its length. 
  static const double RADDEG;             ///< \ru Количество угловых градусов в радиане. \en Count of angular degrees in radian. 
  static const double DEGRAD;             ///< \ru Количество радиан в угловом градусе. \en Count of radians in angular degree. 

  static const double doubleRegion;       ///< \ru Относительная погрешность double. \en Relative tolerance for double. 
  static const double region;             ///< \ru Погрешность (PARAM_REGION). \en Tolerance (PARAM_REGION). 
  static const double precision;          ///< \ru Погрешность аппроксимации (PARAM_PRECISION). \en Approximation tolerance (PARAM_PRECISION). 
  static const double accuracy;           ///< \ru Погрешность (PARAM_ACCURACY). \en Tolerance (PARAM_ACCURACY). 

  static const double determinantMax;     ///< \ru Максимально возможное значение определителя (DETERMINANT_MAX). \en Maximum possible value of determinant (DETERMINANT_MAX). 
  static const double determinantMin;     ///< \ru Минимально возможное значение определителя (DETERMINANT_MIN). \en Minimum possible value of determinant (DETERMINANT_MIN). 

  static const double lengthEpsilon;      ///< \ru Погрешность длины. \en Tolerance for length. 
  static const double lengthRegion;       ///< \ru Погрешность региона. \en Tolerance for region. 

  static const double metricEpsilon;      ///< \ru Погрешность расстояния в итерационных функциях. \en Tolerance for distance in iterative functions. 
  static const double metricRegion;       ///< \ru Неразличимая метрическая область. \en Indistinguishable metric region. 
  static const double metricPrecision;    ///< \ru Метрическая погрешность. \en Metric tolerance. 
  static const double metricAccuracy;     ///< \ru Наибольшая метрическая погрешность. \en Maximum metric tolerance. 
  static const double metricNear;         ///< \ru Метрическая близость. \en Metric proximity tolerance. 

  static const double angleEpsilon;       ///< \ru Минимальный различимый угол. \en Minimum distinguishable angle. 
  static const double angleRegion;        ///< \ru Неразличимая угловая область. \en Indistinguishable angular region. 

  static const double lengthMin;          ///< \ru Квадрат минимальной различимой длины. \en Square of minimum distinguishable length. 
  static const double lengthMax;          ///< \ru Максимальная метрическая длина в системе. \en Maximum metric length in system. 

  static const double minLength;          ///< \ru Минимально  допустимая длина. \en Minimum legal length. 
  static const double maxLength;          ///< \ru Максимально допустимая длина. \en Maximum legal length. 
  static const double minRadius;          ///< \ru Минимально  допустимый радиус. \en Minimum legal radius. 
  static const double maxRadius;          ///< \ru Максимально допустимый радиус. \en Maximum legal radius. 

  static const double metricDelta;        ///< \ru Величина отшагивания. \en Metric offset. 
  static const double paramDeltaMin;      ///< \ru Минимальное  приращение параметра. \en Minimum increment of parameter. 
  static const double paramDeltaMax;      ///< \ru Максимальное приращение параметра. \en Maximum increment of parameter. 

  static const double deltaMin;           ///< \ru Минимальное  приращение. \en Minimum increment. 
  static const double deltaMax;           ///< \ru Максимальное приращение. \en Maximum increment. 


  //------------------------------------------------------------------------------
  // \ru Изменяемые переменные. \en Controlled variables.
  //---
public:
#ifdef USE_VAR_CLASSES
  // \ru Изменяются только в группе с помощью функций SetUserValue() и SetDefaultValues().
  // \en Can be changed only in a group using functions SetUserValue() and SetDefaultValues(). 
  static GroupVariable  LengthEps;        ///< \ru Точность вычисления длины (PARAM_PRECISION). \en Length calculation tolerance (PARAM_PRECISION). 
  static GroupVariable  AngleEps;         ///< \ru Точность вычисления угла. \en Angular tolerance. 
  static GroupVariable  NewtonEps;        ///< \ru Точность численного решения уравнений. \en Tolerance of numerical solution of equation. 
  static GroupVariable  NewtonReg;        ///< \ru Точность проверки решения уравнений. \en Solution of equation checking tolerance. 

  static GroupVariable  paramEpsilon;     ///< \ru Точность параметра кривой. \en Curve parameter tolerance. 
  static GroupVariable  paramRegion;      ///< \ru Точность проверки параметра кривой. \en Curve parameter checking tolerance. 
  static GroupVariable  paramPrecision;   ///< \ru Параметрическая погрешность. \en Parametric tolerance. 
  static GroupVariable  paramAccuracy;    ///< \ru Наибольшая параметрическая погрешность. \en The largest parametric tolerance. 
  static GroupVariable  paramNear;        ///< \ru Параметрическая близость. \en Parametric proximity. 

  // \ru Изменяются с помощью метода SetVarValue().
  // \en Can be changed using SetVarValue method(). 
  static DoubleVariable lowRenderAng;   ///< \ru Угол для минимального количества отображаемых сегментов. \en Angle for minimum mapping segments count. 
  static DoubleVariable higRenderAng;   ///< \ru Угол для максимального количества отображаемых сегментов. \en Angle for maximum mapping segments count. 

  static DoubleVariable deviateSag;     ///< \ru Угловая толерантность. \en Angular tolerance. 
  static DoubleVariable visualSag;      ///< \ru Величина стрелки прогиба для визуализации. \en Value of sag for visualization. 

  static SizeVariable newtonCount;    ///< \ru Число приближений в итерационном методе. \en Number of approximations in iterative method. 
  static SizeVariable newtonLimit;    ///< \ru Количество итераций решения системы уравнений методом Newton. \en Iterations count for solving system of equations by Newton method. 
  static SizeVariable curveDegree;    ///< \ru Порядок кривой (NURBS_DEGREE). \en Curve degree (NURBS_DEGREE). 
  static SizeVariable uSurfaceDegree; ///< \ru Порядок поверхности по U. \en Surface degree by U. 
  static SizeVariable vSurfaceDegree; ///< \ru Порядок поверхности по V. \en Surface degree by V. 
#else

  static double  LengthEps;        ///< \ru Точность вычисления длины (PARAM_PRECISION). \en Length calculation tolerance (PARAM_PRECISION). 
  static double  AngleEps;         ///< \ru Точность вычисления угла. \en Angular tolerance. 
  static double  NewtonEps;        ///< \ru Точность численного решения уравнений. \en Tolerance of numerical solution of equation. 
  static double  NewtonReg;        ///< \ru Точность проверки решения уравнений. \en Solution of equation checking tolerance. 

  static double  paramEpsilon;     ///< \ru Точность параметра кривой. \en Curve parameter tolerance. 
  static double  paramRegion;      ///< \ru Точность проверки параметра кривой. \en Curve parameter checking tolerance. 
  static double  paramPrecision;   ///< \ru Параметрическая погрешность. \en Parametric tolerance. 
  static double  paramAccuracy;    ///< \ru Наибольшая параметрическая погрешность. \en The largest parametric tolerance. 
  static double  paramNear;        ///< \ru Параметрическая близость. \en Parametric proximity. 

  static double lowRenderAng;   ///< \ru Угол для минимального количества отображаемых сегментов. \en Angle for minimum mapping segments count. 
  static double higRenderAng;   ///< \ru Угол для максимального количества отображаемых сегментов. \en Angle for maximum mapping segments count. 

  static double deviateSag;     ///< \ru Угловая толерантность. \en Angular tolerance. 
  static double visualSag;      ///< \ru Величина стрелки прогиба для визуализации. \en Value of sag for visualization. 

  static size_t newtonCount;    ///< \ru Число приближений в итерационном методе. \en Number of approximations in iterative method. 
  static size_t newtonLimit;    ///< \ru Количество итераций решения системы уравнений методом Newton. \en Iterations count for solving system of equations by Newton method. 
  static size_t curveDegree;    ///< \ru Порядок кривой (NURBS_DEGREE). \en Curve degree (NURBS_DEGREE). 
  static size_t uSurfaceDegree; ///< \ru Порядок поверхности по U. \en Surface degree by U. 
  static size_t vSurfaceDegree; ///< \ru Порядок поверхности по V. \en Surface degree by V. 
#endif

  //------------------------------------------------------------------------------
  // \ru Временные переменные. \en Temporary variables. 
  //---
public:
  static size_t            tempIndex;     ///< \ru Временный коэффициент\индекс. \en Temporary coefficient\index. 
  static const MbRefItem * selectCurve;   ///< \ru Запомненный объект (для отладки). \en Stored object (for debug). 
  static const MbRefItem * selectSurface; ///< \ru Запомненный объект (для отладки). \en Stored object (for debug). 
  static const MbRefItem * selectEdge;    ///< \ru Запомненный объект (для отладки). \en Stored object (for debug). 
  static const MbRefItem * selectFace;    ///< \ru Запомненный объект (для отладки). \en Stored object (for debug). 
  static const MbRefItem * selectSolid;   ///< \ru Запомненный объект (для отладки). \en Stored object (for debug). 


  //------------------------------------------------------------------------------
  // \ru Функции статических данных. \en Functions of static data. 
  //---
public: 
  /// \ru Математическая версия по умолчанию. \en Default mathematical version.  
  static const VersionContainer & DefaultVersion();
  /// \ru Математическая версия по умолчанию. \en Default mathematical version.  
  static VERSION DefaultMathVersion();
  /// \ru Идентификатор ядра как приложения. \en Kernel ID.
  static const MbUuid & MathID();
  /// \ru Идентификатор ядра как приложения (устаревший). \en Kernel ID (deprecated).
  static const MbUuid & DeprecatedMathID();

  /** \brief \ru Необходимо ли полное именование объекта.
             \en Is it necessary to full object naming. \~
    \details \ru Необходимо ли полное именование объекта.
             Если возвращает true,  то после создания оболочки именуются грани, рёбра, вершины,
             если возвращает false, то после создания оболочки именуются только грани. \n
             \en Is it necessary to full object naming. 
             If returns true then faces, edges, vertices are named after shell creation,
             If returns false then only faces are named after shell creation. \n \~
    \ingroup Base_Items
  */
  static bool   NamesComplete(); 

  /** \brief \ru Установить необходимость полного именования объекта.
             \en Set flag of full object naming. \~
    \details \ru Установить необходимость полного именования объекта.
             Если передано true, то после создания оболочки будут именоваться грани, рёбра, вершины,
             если передано false, то после создания оболочки будут именоваться только грани. \n
             \en Set flag of full object naming. 
             If passed true then faces, edges, vertices are named after shell creation,
             if passed false then only faces are named after shell creation. \n \~
    \ingroup Base_Items
  */
  static void   SetNamesComplete( bool b ); 

  /** \brief \ru Необходимо ли отключить работу построителей.
               \en Is it necessary to disable the work of constructors. \~
    \details \ru Необходимо ли отключить работу построителей. \n
               \en Is it necessary to disable the work of constructors. \n \~
    \ingroup Base_Items
  */
  static bool   SupressCreators();

  /** \brief \ru Установить флаг отключения работы построителей.
             \en Set the flag to disable the constructors. \~
    \details \ru Установить флаг отключения работы построителей. \n
             \en Set the flag to disable the constructors. \~
    \ingroup Base_Items
  */
  static void   SetSupressCreators( bool b );

  /** \brief \ru Используются ли многопоточные вычисления?
             \en Are multithreaded calculations used? \~
    \details \ru Используются ли многопоточные вычисления? \n
             \en Are multithreaded calculations used? \n \~
    \ingroup Base_Items
  */
  static bool   Multithreaded();

  /** \brief \ru Разрешить использовать многопоточные вычисления.
             \en Set flag for use multithreaded calculations. \~
    \details \ru Разрешить использовать многопоточные вычисления. \n
             Если передано true, то будут использоваться многопоточные вычисления. Устанавливается стандартный режим
             если передано false, то не будут использоваться многопоточные вычисления. \n
             \en Set flag for use multithreaded calculations.
             If passed true then will use multithreaded calculations, multithreaded mode will be set to the standard mode
             if passed false then will not use multithreaded calculations. \n \~
    \ingroup Base_Items
  */
  static void   SetMultithreaded( bool b );
  
  /** \brief \ru Режим многопоточных вычислений
             \en Multithreaded mode \~
    \details \ru Режим многопоточных вычислений \n
             \en Multithreaded mode \n \~
    \ingroup Base_Items
  */  
  static MbeMultithreadedMode   MultithreadedMode();

  /** \brief \ru Проверить режим многопоточных вычислений
             \en Check multithreaded mode \~
    \details \ru Проверить режим  многопоточных вычислений \n
             \en Check multithreaded mode \n \~
    \ingroup Base_Items
  */  
  static bool   CheckMultithreadedMode( MbeMultithreadedMode );

  /** \brief \ru Установить режим многопоточных вычислений.
             \en Set flag for mode of multithreaded calculations. \~
    \details \ru Установить режим многопоточных вычислений. \n
             \en Set flag for mode of multithreaded calculations. \n \~
    \ingroup Base_Items
  */  
  static void    SetMultithreadedMode( MbeMultithreadedMode );

  /** \brief \ru Получить режим оповещения о нарушении требований.
             \en Get the mode of assert violations notification. \~  
  \ingroup Base_Items
  */  
  static c3d::eAssertViolationNotify CheckAssertNotify();

  /** \brief \ru Установить режим оповещения о нарушении требований.
             \en Set the mode of assert violations notification. \~  
  \ingroup Base_Items
  */  
  static void SetAssertNotify( c3d::eAssertViolationNotify );

}; // Math

#ifndef USE_VAR_CLASSES

#define LengthEps()       LengthEps
#define deviateSag()      deviateSag
#define AngleEps()        AngleEps
#define paramRegion()     paramRegion
#define paramEpsilon()    paramEpsilon
#define NewtonEps()       NewtonEps
#define NewtonReg()       NewtonReg
#define paramPrecision()  paramPrecision
#define paramAccuracy()   paramAccuracy
#define paramNear()       paramNear
#define lowRenderAng()    lowRenderAng
#define higRenderAng()    higRenderAng
#define deviateSag()      deviateSag
#define visualSag()       visualSag
#define newtonCount()     newtonCount
#define newtonLimit()     newtonLimit
#define curveDegree()     curveDegree
#define uSurfaceDegree()  uSurfaceDegree
#define vSurfaceDegree()  vSurfaceDegree

#endif

//------------------------------------------------------------------------------
// Оставить от пути только имя файла.
// ---
MATH_FUNC(const char *) C3DFileNameOnly( const char * path );


#if defined( C3D_WINDOWS )
  #define C3D_ASSERT_AS_CERR(expr) std::cerr << "C3D ASSERT VIOLATION in " << C3DFileNameOnly(__FILE__) << "@" << __LINE__ << std::endl;
#else
  #define C3D_ASSERT_AS_CERR(expr) fprintf(stderr, "C3D ASSERT VIOLATION in file %s, %d:\n `%s' in function: %s.\n", C3DFileNameOnly(__FILE__), __LINE__, #expr, __PRETTY_FUNCTION__);
#endif


// Suppress a warning "unreferenced formal parameter"
#define C3D_UNUSED_PARAMETER( param ) (std::ignore = param)


#ifdef C3D_DEBUG
#define C3D_ASSERT_UNCONDITIONAL(expr) \
          { const c3d::eAssertViolationNotify notify = ::Math::CheckAssertNotify(); \
            if ( c3d::avn_ASSERT == notify ) { _ASSERT(false); } \
            else if ( c3d::avn_CERR == notify ) { C3D_ASSERT_AS_CERR(expr) } \
          }
//------------------------------------------------------------------------------
// Не рекомендуется использовать с параметром-константой
// (при сборке в VS2012 выдается - warning C4127: conditional expression is constant).
// ---
#define C3D_ASSERT(expr) \
          if (!(expr)) \
          C3D_ASSERT_UNCONDITIONAL(expr)

#else
  #define C3D_ASSERT_UNCONDITIONAL(expr) ((void)0)
  #define C3D_ASSERT(expr) ((void)0)
#endif

extern "C"
{
  /** \brief \ru Получить информацию о версии файла c3d.
           \en Get information about the c3d file version. \~
  \details \ru Функция копирует в указанный буфер строку с информацией о версии файла c3d. \n
           \en The function copies a string with information about the c3d file version to the specified buffer. \n \~
  \param[in] buffer     - \ru Указатель на начало буфера.
                          \en Pointer to the beginning of the buffer. \~
  \param[in] bufferSize - \ru Размер буфера, в символах.
                          \en The size of the buffer in characters. \~
  \return \ru Возвращает количество скопированных символов, без учета null-символа. Если buffer == nullptr возвращается необходимый размер буфера без учета null-символа.
          \en Returns the number of copied characters, excluding the null character. If buffer = = nullptr returns the required buffer size without the null character. \~
  */  

  extern MATH_FUNC(size_t) GetC3dVersionInfo( char * const buffer, size_t bufferSize );  ///< \ru Информация о версии c3d.dll \en c3d.dll version information

  /** \brief \ru Получить информацию о варианте сборки файла c3d.
           \en Get information about the c3d file build option. \~
  \details \ru Функция копирует в указанный буфер строку с информацией о варианте сборки файла c3d. \n
           \en The function copies a string with information about the c3d file build option to the specified buffer. \n \~
  \param[in] buffer     - \ru Указатель на начало буфера.
                          \en Pointer to the beginning of the buffer. \~
  \param[in] bufferSize - \ru Размер буфера, в символах.
                          \en The size of the buffer in characters. \~
  \return \ru Возвращает количество скопированных символов, без учета null-символа. Если buffer == nullptr возвращается необходимый размер буфера без учета null-символа.
          \en Returns the number of copied characters, excluding the null character. If buffer = = nullptr returns the required buffer size without the null character. \~
  */  

  extern MATH_FUNC(size_t) GetC3dBuildInfo( char * const buffer, size_t bufferSize );  ///< \ru Информация о сборке c3d.dll \en c3d.dll building information
}

  /** \brief \ru Получить информацию о файле c3d.
           \en Get information about the c3d file. \~
  \details \ru Эта функция - обертка для функций GetC3dVersionInfo и GetC3dBuildInfo. Возвращает строку с информацией о версии файла или варианте сборки файла. \n
           \en This function is a wrapper for the GetC3dVersionInfo and GetC3dBuildInfo functions. Returns a string with information about the file version or build version of the file. \n \~
  \param[in] needVersionInfo - \ru needVersionInfo == true - возвращается информация о версии файла, иначе информация о варианте сборки файла.
                               \en need Version Info == true - returns information about the file version, otherwise information about the file build option. \~
  \return \ru Возвращает строку с запрошенной информацией.
          \en Returns a string with the requested information. \~
  */  

inline std::string GetC3dLibInfo( bool needVersionInfo = true )
{
  size_t( *GetLibInfo )( char * const buffer, size_t bufferSize );
  GetLibInfo = ( needVersionInfo ) ? GetC3dVersionInfo : GetC3dBuildInfo;

  std::vector<char> buffer( GetLibInfo( nullptr, 1 ) );

  GetLibInfo( &buffer[0], buffer.size() );

  return std::string(buffer.begin(), buffer.end());
}

#endif // __MB_VARIABLES_H
