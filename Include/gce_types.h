//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file     
  \brief \ru Типы данных двумерного геометрического решателя.
         \en Data types of the two-dimensional geometric solver. \~
  \details \ru Этот файл представляет собой набор типов данных, необходимых для 
               взаимодействия геометрического решателя с клиентским приложением.
           \en This file contains set of data types necessary for interaction 
               of the geometrical solver with user application. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GCE_TYPES_H
#define __GCE_TYPES_H

#include <memory.h>
#include <system_types.h>
#include <math_define.h>
#include <mb_variables.h>
#include <mb_enum.h>
#include <vector>
#ifndef C3D_WINDOWS //_MSC_VER
#include <cstring>
#endif //C3D_WINDOWS

class  MbNurbs;

/** 
  \addtogroup Constraints2D_API   
  \{
*/

//----------------------------------------------------------------------------------------
/** \brief \ru Система геометрических ограничений.
           \en Geometric constraints system. \~
  \details \ru GCE_system является типом данных, который обозначает систему ограничений, 
  которая создается с помощью вызова #GCE_CreateSystem. Реально, этот тип является 
  указателем на внутреннюю структуру данных, где содержится система ограничений и различные 
  рабочие данные, определяющие её внутреннее состояние. Время жизни системы ограничений 
  заканчивается только, когда к ней будет применен вызов API #GCE_RemoveSystem, после чего 
  значение GCE_system становится недействительным.
           \en GCE_system is data type which denotes a system of constraints created by 
  call #GCE_CreateSystem. Actually this type is a pointer to an internal data structure with 
  a system of constraints and various working data determining its internal state. Lifetime 
  of the constraint system finishes only when a call of API #GCE_RemoveSystem 
  is applied to it, thereafter the value of GCE_system becomes invalid. \~
*/
typedef void *  GCE_system;

//----------------------------------------------------------------------------------------
// \ru Типы данных. \en Data types.
//---
/// \ru Дескриптор геометрического объекта, зарегистрированного в контексте решателя. \en Descriptor of geometrical object registered in the solver context. 
typedef size_t  geom_item;        
/// \ru Дескриптор ограничения, зарегистрированного в решателе. \en Descriptor of a constraint registered in the solver. 
typedef size_t  constraint_item;  
/// \ru Дескриптор переменной, зарегистрированной в решателе. \en Descriptor of a variable registered in the solver. 
typedef size_t  var_item;
/// \ru Дескриптор паттерна, зарегистрированного в контексте решателя. \en Descriptor of pattern registered in the solver context.
typedef geom_item pattern_item;
/// \ru Тип целочисленной степени свободы. \en Type of integer-valued degree of freedom.
typedef ptrdiff_t   GCE_dof_type;

//----------------------------------------------------------------------------------------
// \ru Константы. \en Constants.
//---
/// \ru Неопределенное значение дескриптора или пустого объекта. \en Undefined value of descriptor or an empty object. 
const size_t          GCE_NULL   = SYS_MAX_T;
/// \ru Неопределенное значение дескриптора типа #geom_item. \en Undefined value of #geom_item type. 
const geom_item       GCE_NULL_G = GCE_NULL; 
/// \ru Неопределенное значение дескриптора типа #var_item. \en Undefined value of #var_item type. 
const var_item        GCE_NULL_V = GCE_NULL; 
/// \ru Неопределенное значение дескриптора типа #constraint_item. \en Undefined value of #constraint_item type. 
const constraint_item GCE_NULL_C = GCE_NULL;
/// \ru Неопределенное значение дескриптора типа #pattern_item. \en Undefined value of #pattern_item type.
const pattern_item    GCE_NULL_P = GCE_NULL;
/// \ru Не определенное значение числа double. \en An undefined value of double.
const double          GCE_UNDEFINED_DBL = UNDEFINED_DBL;

//----------------------------------------------------------------------------------------
/// \ru Словарь геометрических типов данных. \en Dictionary of а geometric data types.
//---
typedef enum
{ 
  GCE_ANY_GEOM,         ///< \ru Неизвестный тип. \en Unknown type.

  // \ru Основные типы. \en Basic types.
  GCE_POINT,            ///< \ru Точка на плоскости. \en Point on plane.
  GCE_AXIS,             ///< \ru Ось. \en Axis.
  GCE_LINE,             ///< \ru Прямая на плоскости. \en Line on plane.
  GCE_CIRCLE,           ///< \ru Окружность на плоскости. \en Circle on plane.
  GCE_ELLIPSE,          ///< \ru Эллипс на плоскости. \en Ellipse on plane.
  GCE_SPLINE,           ///< \ru Сплайн на плоскости. \en Spline on plane. 
  GCE_PARAMETRIC_CURVE, ///< \ru Параметрическая кривая на плоскости. \en Parametric curve on plane.
  GCE_BOUNDED_CURVE,    ///< \ru Ограниченная двумя точками, кривая. \en Curve bounded by two points.
  GCE_OFFSET_CURVE,     ///< \ru Эквидистантная кривая. \en Equidistant curve.

  // \ru Дополнительные типы. \en Additional types.
  GCE_LINE_SEGMENT,     ///< \ru Отрезок прямой. \en Line segment.
  GCE_SET,              ///< \ru Подмножество геометрических объектов. \en Subset of geometric objects.

  // \ru Производные типы. \en Derived types.
  GCE_INSTANCE,         ///< \ru Экземпляр базового объекта. \en An instance of the base object.
  GCE_PATTERN,          ///< \ru Геометрический паттерн. \en A geometrical pattern.
} geom_type;


//----------------------------------------------------------------------------------------
/** \brief  \ru Идентификаторы, означающие контрольные точки и другие элементы, составляющие 
                запись (tuple) геометрического объекта.
            \en IDs denoting control points and other elements that form a record (tuple) 
                of a geometric object. ~\
    \details  \ru Это перечисление применяется для запроса дескриптора характерных точек объекта,
                  таких как центр окружности, концевая точка кривой и т.д...
              \en This enum is used to request a descriptor of control point of an object, 
                  such as center of circle, bounding point of a curve etc... ~\
    \see #GCE_PointOf
*/
//---
typedef enum
{
  /*
    (!) Don't change the integer values of these names. It may be written to a file permanently.
  */
    GCE_FIRST_PTYPE = 0     ///< \ru Значение начинающее последовательность вариантов. \en The value of beginning of the sequence.
  , GCE_IMPROPER_POINT = 0  ///< \ru Точка, не принадлежащая объекту. \en Point not belonging to the object.  
  , GCE_FIRST_END           ///< \ru Первый конец ограниченной кривой. \en The first end of bounded curve.
  , GCE_SECOND_END          ///< \ru Второй конец ограниченной кривой. \en The second end of bounded curve. 
  , GCE_CENTRE              ///< \ru Точка центра окружности, дуги или эллипса. \en Central point of circle, arc or ellipse.
  , GCE_PROPER_POINT        ///< \ru Собственно точка. \en Proper point.
  , GCE_Q1  ///< \ru Квадрантная точка эллипса (3 часа). \en Quadrant point of ellipse (3 o'clock).
  , GCE_Q2  ///< \ru Квадрантная точка эллипса (12 часов). \en Quadrant point of ellipse (12 o'clock).
  , GCE_Q3  ///< \ru Квадрантная точка эллипса (9 часов). \en Quadrant point of ellipse (9 o'clock).
  , GCE_Q4  ///< \ru Квадрантная точка эллипса (6 часов). \en Quadrant point of ellipse (6 o'clock).
  , GCE_LOCATION_POINT  ///< \ru Точка размещения геометрического объекта. \en Location point of geometric object.
  , GCE_ORIGIN = GCE_LOCATION_POINT ///< \ru Точка начала оси. \en Axis origin point.
  , GCE_LAST_PTYPE      ///< \ru Значение завершающее последовательность вариантов. \en The value of variants completes the sequence.
  /*
    The values below are used only within the solver.
  */
  , GCE_DIRECTION ///< \ru Направляющий вектор эллипса (направление "большой" полуоси ) или паттерна. \en Vector of ellipse direction (direction of "major" semiaxis) or pattern.
  , GCE_L_NORMAL  ///< \ru Вектор нормали линейного объекта. \en Normal vector of a linear geometry entity.
  /** \brief \ru Единичный вектор ориентации: Нормаль прямой, направление "большой" полуоси эллипса.    
             \en Unit vector of orientation: Normal of a line, direction of "major" semiaxis of ellipse. \~
  */
  , GCE_ORIENTATION
  , GCE_BASE_CURVE
  , GCE_BASE_PATTERN
  , GCE_BASE_GEOM
  , GCE_FIRST_SUB_PATTERN
  , GCE_SECOND_SUB_PATTERN
} subgeom_type;

//----------------------------------------------------------------------------------------
/** \brief \ru Тип запрашиваемой точки (используется, как подмножество значений subgeom_type).
           \en Type of the requested point (used as subset of values subgeom_type).
*/
//---
typedef subgeom_type point_type;

//----------------------------------------------------------------------------------------
/** \brief  \ru Перечисление координат геометрических объектов.
            \en Enumeration of geom's coordinates.
*/
//---
typedef enum
{ 
    GCE_X, GCE_Y      ///< \ru Координаты точки или вектора. \en Coordinates of a point or a vector.
  , GCE_ACRD          ///< \ru Угол нормали прямой, угол наклона эллипса. \en Angle of line normal, slope angle of ellipse.
  , GCE_DCRD          ///< \ru Координата смещения прямой, расстояние от начала координат до прямой. \en Coordinate of line shift, distance from CS origin to line.
  , GCE_RADIUS        ///< \ru Радиус окружности. \en Circle radius.
  , GCE_MAJOR_RADIUS  ///< \ru "Главная" полуось эллипса. \en "Major" semiaxis of ellipse.
  , GCE_MINOR_RADIUS  ///< \ru "Малая" полуось эллипса. \en "Minor" semiaxis of ellipse.
  , GCE_OFFSET        ///< \ru Смещение эквидистантной кривой. \en Offset of equidistant curve.
  , GCE_STEP          ///< \ru Линейное или угловое смещение паттерна. \en Linear or angular pattern shift.  
  , GCE_NULL_CRD      ///< \ru Пустая (несуществующая) координата. \en Empty (nonexistent) coordinate.
} coord_name;  

typedef coord_name coord_type;

//----------------------------------------------------------------------------------------
/** \brief  \ru Идентификатор типа 2D ограничения.
            \en Identifier of 2D constraint type. \~
  \attention \ru На самом деле решатель поддерживает другие ограничения, кроме перечисленных.
                 См. вызовы API в 'gce_api.h'
             \en Actually, the solver provides more constraint types than is given in the enum. 
                 See the API calls in 'gce_api.h' \~
*/
typedef enum
{
  // \ru Унарные геометрические ограничения: \en Unary geometric constraints:
    GCE_FIX_GEOM        ///< \ru Фиксация геометрического объекта. \en Fixation of geometric object.
  , GCE_HORIZONTAL      ///< \ru Горизонтальность прямой или отрезка. \en Horizontality of a linear object.
  , GCE_VERTICAL        ///< \ru Вертикальность прямой или отрезка. \en Verticality of a linear object.
  , GCE_ANGLE_OX
  
  // \ru Бинарные геометрические ограничения: "constr( geom1, geom2 )" \en Binary geometric constraints: "constr( geom1, geom2 )" 
  , GCE_COINCIDENT      ///< \ru Совпадение пары геометрических объектов. \en Coincidence of a pair of geometric objects.
  , GCE_EQUAL_CURVATURE ///< \ru Равенство кривизны кривых в точках. \en Equality of curves curvature in given points.
  , GCE_EQUAL_LENGTH    ///< \ru Равенство длин пары кривых. \en Equality of two curves lengths.
  , GCE_EQUAL_RADIUS    ///< \ru Равенство радиусов пары окружностей. \en Equality of two circles lengths.
  , GCE_EQUAL_OFFSET    ///< \ru Равенство смещений пары эквидистантных кривых. \en Equality of offset of two equidistant curves.
  , GCE_PARALLEL        ///< \ru Параллельность пары прямых или отрезков. \en Parallelism of two lines or segments.
  , GCE_PERPENDICULAR   ///< \ru Перпендикулярность пары прямых или отрезков. \en Perpendicularity of two lines or segments.
  , GCE_TANGENT         ///< \ru Касание пары кривых. \en Tangency of two curves.
  , GCE_COLINEAR        ///< \ru Коллинеарность пары прямых или отрезков. \en Collinearity of a pair of lines or segments.
  , GCE_ALIGN_2P        ///< \ru Выравнивание пары точек вдоль направления. \en Alignment of a pair of points along the direction.  

  // \ru Тернарные геометрические ограничения. \en Ternary geometric constraints.
  , GCE_BISECTOR        ///< \ru Биссектриса угла. \en Bisector of angle.
  , GCE_MIDDLE_POINT    ///< \ru Средняя точка. \en Middle point.
  , GCE_COLINEAR_3P     ///< \ru Коллинеарность тройки точек. \en Collinearity of a point triple.
  , GCE_SYMMETRIC       ///< \ru Симметричность. \en Symmetry.
  , GCE_PERCENT_POINT   ///< \ru \en
  , GCE_EQUATION        ///< \ru Уравнение. \en Equation.
  , GCE_PATTERNED       ///< \ru Связать паттерном пару кривых. \en Bind a pair of curves in a pattern.

  // \ru Размерные геометрические ограничения. \en Dimensional geometric constraints.
  , GCE_DISTANCE
  , GCE_DIAMETER
  , GCE_LENGTH          ///< \ru Фиксация длины отрезка или дуги. \en Fixation of length of a line segment or arc.
  , GCE_LINEAR_DIM
  , GCE_RADIUS_DIM  
  , GCE_OFFSET_DIM
  , GCE_ANGLE
  , GCE_INTERVAL
  /*
    The values below are used only within the solver.
  */
  , GCE_IDENTITY
  , GCE_CONSTRAINTS_COUNT               ///< \ru Количество типов. \en Number of types.
  , GCE_UNKNOWN = GCE_CONSTRAINTS_COUNT ///< \ru Неизвестный тип ограничения. \en Unknown constraint type.
  , GCE_UNKNOWN_CON = GCE_UNKNOWN
} constraint_type;

//----------------------------------------------------------------------------------------
/** \brief  \ru Диагностические коды выполнения функций геометрического решателя. 
            \en Diagnostic codes of the geometric solver functions execution. \~
*/
//---
typedef enum
{
  /*
    Evaluation results.
  */
    GCE_RESULT_None                      ///< \ru Нет результата (пустое сообщение). \en No result (empty message). 
  , GCE_RESULT_Ok                        ///< \ru Успешный результат. \en Successful result. 
  , GCE_RESULT_Satisfied = GCE_RESULT_Ok ///< \ru Ограничение удовлетворено. \en The constraint is satisfied. 
  , GCE_RESULT_Not_Satisfied             ///< \ru Система ограничений не решена. \en The system of constraints is not solved. 
  , GCE_RESULT_Overconstrained           ///< \ru Переопределенная (несовместная) система ограничений. \en Overdetermined (inconsistent) system of constraints. 
  , GCE_RESULT_InvalidGeometry           ///< \ru Решение привело к нарушению геометрии. \en Solution leaded to violation of geometry.
  
  /*
    Data validation errors.
  */
  , GCE_RESULT_MovingOfFixedGeom         ///< \ru Попытка перемещения фиксированного объекта. \en Attempt of a fixed object translation.   
  , GCE_RESULT_CircleCantStretched       ///< \ru Окружность не может быть масштабирована с разными коэффициентами по осям (растяжение). \en The circle can't be scaled with different scaling factors for each axis (stretching).
  , GCE_RESULT_SingularMatrix            ///< \ru Прислали вырожденную матрицу трансформации. \en A singular transform matrix was received.
  , GCE_RESULT_DegenerateScalingFactor   ///< \ru Вырожденный коэффициент масштабирования. \en Degenerate scaling factor.
  , GCE_RESULT_InvalidDimensionTransform ///< \ru Неудачное преобразование размера. \en Invalid dimension transformation.  
  , GCE_RESULT_IsNotDrivingDimension     ///< \ru Данное ограничение должно быть управляющим размером. \en Given constraint should be a driving dimension.
  , GCE_RESULT_UnsupportedConstraint     ///< \ru На геометрические объекты было наложено невозможное ограничение. \en An impossible constraint was set on geometric objects.
  , GCE_RESULT_AnisotropicScaling        ///< \ru Анизотропное масштабирование. \en Anisotropic scaling.
  , GCE_RESULT_OverconstrainedInstance   ///< \ru Попытка подчинить экземпляр более, чем одному паттерну. \en An attempt to make an instance patterned on more than one pattern.
  , GCE_RESULT_HelpGeomRequired          ///< \ru Не хватает вспомогательного объекта. \en There is a missing help geometric object.

  /*
    System errors.
  */
  , GCE_RESULT_SystemError               ///< \ru Внутренняя системная ошибка. \en Internal system error. 
  , GCE_RESULT_NullSystem                ///< \ru Обращение к недействительной системе ограничений. \en Access to invalid system of constraints. 
  , GCE_RESULT_Unregistered              ///< \ru Обращение к недействительному объекту. \en Access to invalid object.
  , GCE_RESULT_Aborted                   ///< \ru Процесс вычислений был прерван по запросу приложения. \en The evaluation process aborted by the application. \~

} GCE_result;

//----------------------------------------------------------------------------------------
/**
  \brief  \ru Статус геометрического ограничения. 
          \en Status of a geometric constraint.
  \details  
          \ru Статус ограничения подразумевает деление системы ограничения на подмножества,
  которые маркируются следующим образом:\n
    Ограничения, помеченные GCE_STATUS_WellTreated и GCE_STATUS_WellConditioned образуют 
  хорошо-обрабатываемую решателем часть системы ограничений, не содержащую переопределений 
  и обычно вычисляемую без противоречий.\n
    Ограничения, помеченные статусами GCE_STATUS_WellConditioned и GCE_STATUS_IllConditioned
  вместе образуют группу взаимосвязанных ограничений, которые обычно решаются без противоречий,
  но потенциально могут противоречить друг другу при участии размеров.
    Те из них, что помечены GCE_STATUS_IllConditioned создают условия для решаемости хуже,
  чем GCE_STATUS_WellConditioned.\n
    Статусом GCE_STATUS_Redundant решатель помечает лишние ограничения, которые можно удалить
  из системы ограничений. Обычно такие ограничения исполняются за счет других ограничений 
  или создают ситуацию переопределенности (несовместная система ограничений).\n
    Статусом GCE_STATUS_OverConstrained помечаются те из ограничений, которые остались не 
  решенными по причине несовместной переопределенности. Ограничения со статусами
  GCE_STATUS_Redundant и GCE_STATUS_OverConstrained могут находится в противоречии с другими
  ограничениями кроме тех, что помечены GCE_STATUS_WellTreated.

    The status of a constraint implies the division of the system into subsets, which are 
  labeled as follows: \n
    Constraints marked by GCE_STATUS_WellTreated and GCE_STATUS_WellConditioned together form
  well-resolved part that does not contain overdefining constraints and usually computed 
  without contradiction. \n
    Constraints marked with GCE_STATUS_WellConditioned and GCE_STATUS_IllConditioned statuses
  together form a group of interrelated constraints that are usually resolved without
  contradiction, but potentially contradictory with dimensions.
  Those that are labeled GCE_STATUS_IllConditioned make conditions for evaluating worse,
  than GCE_STATUS_WellConditioned. \n
    The status of GCE_STATUS_Redundant solver marks the extra constraints that can be removed.
  from the constraint system. Usually such constraints are enforced by other constraints.
  or create an overdefined situation (incompatible constraint system). \n
  The status GCE_STATUS_OverConstrained marks those of constraints that was not solved 
  due to inconsistent overdefining. Constraints with GCE_STATUS_Redundant and 
  GCE_STATUS_OverConstrained statuses may conflict with other constraints except those 
  marked with GCE_STATUS_WellTreated.
*/
//---
typedef enum 
{
    GCE_STATUS_Undefined = 0
  /*
    Statuses indicating which of constraints belong to well-treated 
    or redundancy parts of the constraint system.
  */    
  , GCE_STATUS_WellTreated      = 1 // Ограничение принадлежит рабочей части системы ограничений без переопределений.
  , GCE_STATUS_WellConditioned  = 2 // Ограничение принадлежит хорошо-обусловленной части уравнений.
  , GCE_STATUS_IllConditioned   = 3 ///< /ru Ограничения из плохо-обусловленной части. /en A constraint of ill-condition
  , GCE_STATUS_Redundant        = 4 ///< /ru Ограничение игнорируется решателем по причине избыточности. // en A constraint is ignored by the solving process because of the redundancy.

  /*
    Statuses resulting the evaluation (call GCE_Evaluate).
  */
  , GCE_STATUS_NotSolved        // Не решено по каким-то причинам
  , GCE_STATUS_NotConsistent    // Не решено из-за противоречия с другими ограничениями.
  , GCE_STATUS_OverConstrained  // Не решено избыточное ограничение, противоречащее другим.

} GCE_c_status;

//----------------------------------------------------------------------------------------  
/** \brief \ru Состояние определенности системы геометрических ограничений. 
           \en Difinition State of geometric constraints system. 
*/ //---
typedef enum
{
    GCE_STATE_Unknown = 0           ///< \ru О состоянии ничего не известно. \en State is unknown.
  , GCE_STATE_WellConstrained       ///< \ru Полностью определенная система - все степени свободы нулевые. \en Well-constrained system: all degrees of freedom are zero.
  , GCE_STATE_UnderConstrained      ///< \ru Недоопределенная система - имеются ненулевые степени свободы. \en Underconstrained system: there are non-zero degrees of freedom.
  , GCE_STATE_UnresolvedRedundancy  ///< \ru Имеются не удовлетворенные избыточные ограничения. There are unresolved redundant constraints.
   /*
    \ru Идентификаторы не менять (возможна запись в файлы)!
    \en Don't change identifiers (record to files is possible)!
  */
} GCE_s_state;

//----------------------------------------------------------------------------------------
/// \ru Вернет 'true' в случае успешного результата. \en Return true, if the result code is successful.
// ---
inline GCE_FUNC(bool) OK( GCE_result resCode )
{
  return resCode == GCE_RESULT_Ok;
}

//----------------------------------------------------------------------------------------
/// \ru Вариант решения биссектрисы для двух прямых. \en Variant of a bisector for two lines.
/*
  \ru Идентификаторы не менять (возможна запись в файлы)!
  \en Don't change identifiers (record to files is possible)!
*/
//---
typedef enum
{
    GCE_BISEC_CLOSEST = 0 ///< \ru Неопределенное направление (ближайшее решение). \en Undefined direction (nearest solution).  
  , GCE_BISEC_MINUS   = 1 ///< \ru Биссектриса вдоль разности направлений векторов прямых/отрезков. \en Bisector along vector difference of the line directions.
  , GCE_BISEC_PLUS    = 2 ///< \ru Биссектриса вдоль суммы направляющих векторов прямых/отрезков. \en Bisector along vector sum of the line directions.

} GCE_bisec_variant;

//----------------------------------------------------------------------------------------
/** \brief  \ru Варианты выравнивания направлений.
            \en Variants of alignment. \~
  \details \en Значение опции выравнивания используется для выбора из альтернативных решений
               ограничения, такого как `GCE_TANGENT`.
           \en The alignment value is used to alternate between solutions of a constraint such as `GCE_TANGENT`.
  \note \ru Значения этого перечисления могут быть использованы для постоянного хранения и
            останутся неизменными в следующих версиях.
        \en Values of this enum can be used for permanent storage and will be kept
            in the future versions. \~
*/
//---
typedef enum
{
  GCE_NO_ALIGNMENT = 0, ///< \ru Неопределенное значение выравнивания (=не применима к данному ограничению). \en Undefined alignment value (=not applicable to this constraint).
  GCE_COORIENTED,       ///< \ru Для касания это сонаправленные касательные вектора (=нормали). \en For tangency, these are co-directional tangent vectors (=normal).
  GCE_OPPOSITE,         ///< \ru Для касания это противонаправленные касательные вектора (=нормали). \en For tangency, these are the opposing tangent vectors (=normal).

  /** \brief \ru Поддерживать способ выравнивания согласно начальному или текущему положению геометрии (соответствует поведению прежних версий).
             \en Maintain the alignment according to the initial or current position of the geometry (reproduces behaviour from previous versions). \~ */
  GCE_CLOSEST,

  /** \brief \ru Автоматически выбрать опцию выравнивания, используя начальное приближение геометрии.
             \en Automatically determine alignment options using initial geometry approximation. \~ */
  GCE_AUTO_ALIGNMENT,  
                       
} GCE_alignment;


//----------------------------------------------------------------------------------------
/// \ru Координаты вектора. \en Vector coordinates.
//---
struct GCE_CLASS GCE_vec2d
{
  double x, y;
  GCE_vec2d() { x = y = 0.; }
};

//----------------------------------------------------------------------------------------
/// \ru Координаты вектора n-й размерности. \en Coordinates of n-dimensional vector.
//---
struct GCE_CLASS GCE_vecNd
{
  size_t    size;
  double *  arg;
  GCE_vecNd(): size(0), arg(0) {}
};

//----------------------------------------------------------------------------------------
/// \ru Координаты точки на плоскости. \en Coordinates of a point on plane.
//---
struct GCE_CLASS GCE_point
{
  double x, y;
  GCE_point() { x = y = 0; }
};

//----------------------------------------------------------------------------------------
/// \ru Степень свободы точки. \en Degree of freedom of a point.
//---
struct GCE_CLASS GCE_point_dof
{
  GCE_dof_type dof;  ///< Degree of freedom of the point.
  GCE_vec2d    dir;  ///< Direction of point moving freedom.
  GCE_point_dof(): dof(-1), dir() {}
};

//----------------------------------------------------------------------------------------
/// \ru Координаты прямой на плоскости. \en Coordinate of a line on the plane.
//---
struct GCE_CLASS GCE_line
{
  GCE_point p;
  GCE_vec2d norm;
  GCE_line() : p(), norm() {}
};

//----------------------------------------------------------------------------------------
/// \ru Координаты оси. \en Axis coordinates.
//---
struct GCE_CLASS GCE_axis
{
  GCE_point origin;   ///< \ru Начало оси. \en Axis origin.
  GCE_vec2d dir; ///< \ru Направление оси. \en Axis direction.

  GCE_axis() : origin(), dir() {}
};

//----------------------------------------------------------------------------------------
/// \ru Координаты окружности. \en Coordinates of a circle. 
//---
struct GCE_CLASS GCE_circle
{
  GCE_point centre; ///< \ru Центр окружности. \en Circle center.
  double    radius; ///< \ru Радиус окружности. \en Circle radius.
  GCE_circle() : centre(), radius( 0.0 ) {}
};

//----------------------------------------------------------------------------------------
/// \ru Координаты эллипса. \en Coordinates of an ellipse.
//---
struct GCE_CLASS GCE_ellipse
{
  GCE_point centre; ///< \ru Центр эллипса. \en Ellipse center.
  GCE_vec2d direct; ///< \ru Направляющий вектор главной полуоси. \en Vector of the major semiaxis direction.
  double    majorR; ///< \ru Главная полуось. \en Major semiaxis.
  double    minorR; ///< \ru Вторая полуось. \en Second semiaxis.
  
  GCE_ellipse() 
    : centre()
    , direct()
    , majorR( 0.0 )
    , minorR( 0.0 )
  {}
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Координаты и характеристики сплайна. 
            \en Coordinates and characteristics of a spline.\~
  \details  \ru 
      Сплайн можно задавать тремя способами: \n
      1) По уже существующему объекту MbNurbs. \n
      2) По уже существующему объекту MbNurbs и набору интерполяционных точек. \n
      3) По набору интерполяционных точек, соответствующих им параметров, порядку и признаку замкнутости.
            \en
      The spline can be specified in three ways: \n
      1) Using already existing object of MbNurbs. \n
      2) Using already existing object of MbNurbs and a set of interpolation points. \n
      3) Using a set of interpolation points, corresponding parameters, order and closedness attribute.\~
  \ingroup Constraints2D_API
*/
// ---
struct GCE_CLASS GCE_spline
{
  size_t                 degree;          ///< \ru Порядок В-сплайна. \en Order of B-spline. 
  bool                   isClosed;        ///< \ru Признак замкнутости кривой. \en An attribute of curve closedness.
  std::vector<GCE_point> controlPoints;   ///< \ru Множество контрольных точек. \en Set of control points.
  std::vector<double>    controlWeights;  ///< \ru Множество весов контрольных точек. \en Set of weights of the control points.
  std::vector<double>    controlKnots;    ///< \ru Узловой вектор. \en Knot vector.
  std::vector<geom_item> interpPoints;    ///< \ru Множество интерполяционных точек. \en Set of interpolation points.
  std::vector<double>    interpParams;    ///< \ru Множество значений параметров, соответствующих интерполяционным точкам. \en Set of the parameter values corresponding to interpolation points.
  MbeNurbsCurveForm      form;            ///< \ru Форма кривой. \en Form of curve. 

  GCE_spline()
    : degree( Math::curveDegree )
    , isClosed( false )
    , controlPoints()
    , controlWeights()
    , controlKnots()
    , interpPoints()
    , interpParams()
    , form( ncf_Unspecified )
  {}
  explicit GCE_spline( const MbNurbs & nurbs );
  GCE_spline( const MbNurbs & nurbs, const std::vector<geom_item> & interp );
  GCE_spline( size_t deg, bool cls, const std::vector<geom_item> & interp, const std::vector<double> & pars );

  OBVIOUS_PRIVATE_COPY( GCE_spline );
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Параметры размерного ограничения.
            \en Parameters of dimensional constraint. \~
  \details 
      \ru Все размерные ограничения задаются над геометрическими объектами; кроме них
  размер определяется дополнительными параметрами, которые передаются через
  структуру #GCE_dim_pars.
      \en All dimensional constraints are specified on geometrical objects; in addition
  dimension is specified by additional parameters which are passed via
  the structure #GCE_dim_pars. \~

  \par 
    \ru Функции, в которые GCE_dim_pars передается в качестве аргумента:
  #GCE_AddDistance, #GCE_AddDistance2P, #GCE_AddDistancePLs, #GCE_AddDistancePLs, 
  #GCE_AddDirectedDistance, #GCE_FormCirDimension.
    \en Functions into which GCE_dim_pars is passed as argument:
  #GCE_AddDistance, #GCE_AddDistance2P, #GCE_AddDistancePLs, #GCE_AddDistancePLs, 
  #GCE_AddDirectedDistance, #GCE_FormCirDimension. \~  
    
  \par \ru Размеры

    Размер - это числовая функция, аргументами которой являются геометрические объекты, 
  а возвращаемым значением является число. На основе размеров определяются
  'размерные ограничения'. Все 'размерные ограничения' связывают геометрические 
  объекты с числом, называемым значением размера. Если ограничение удовлетворено, 
  то его числовой параметр равен значению размера.  Числовой  параметр размера
  задается либо фиксированным числом (константой), либо числовой переменной.\n
  Решатель ограничений обрабатывает два типа размеров: Управляющие и вариационные.\n

       \en Dimensions

    Dimension is a numerical function which arguments are geometric 
  objects and return value is a number. 'Dimensional constraints' are defined
  on the base of dimensions. All 'dimensional constraints' associate geometric 
  objects with a number called a value of dimension. If the constraint is satisfied, 
  then its numerical parameter is equal to the value of dimension.  Numerical parameter of the dimension
  is specified by a fixed number (independent parameter) or by a numerical variable.\n
  The solver of constraints treats two types of dimensions: Driving and variational.\n \~

  \par \ru Виды размеров

    "Управляющий" размер - это размерное ограничение, задающее положение
  геометрическим объектам согласно константного значения размера;\n
    "Вариационный" размер - это ограничение, связывающее геометрические 
  объекты и переменную, равную значению размера. Под воздействием вариационного 
  размера может меняться и геометрия и переменная размера.\n
    Размеры могу быть направленные, например, расстояние между 
  точками по горизонтали или по вертикали (функция #GCE_AddDirectedDistance).

       \en Kinds of dimension

    "Driving" dimension is a dimensional constraint specifying position of 
  geometric objects subject to a constant value of dimension;\n
    "Variational" dimension is a constraint associating geometric 
  objects and a variable which is equal to the dimension value. Both geometry and variable of dimension 
  can vary under the influence of variational dimension.\n
    Dimensions can be directed, for instance, horizontal or vertical distance between 
  points(function #GCE_AddDirectedDistance). \~
    
  
  \par \ru Параметры
  var - дескриптор переменной, задающей значение размера (градусы, если размер угловой); \n
  dimValue - параметр, задающий значение размера; \n

  Если var != GCE_NULL_V, это означает, что размер "вариационный".
  Если var == GCE_NULL_V, то значение размера = dimValue, иначе значение размера
  тождественно равно числовой переменной 'var', т.е. размер управляющий.

       \en Parameters
  
  var - descriptor of variable specifying the value of dimension (degrees if the dimension is angular); \n
  dimValue - parameter specifying the value of dimension; \n

  If var != GCE_NULL_V, it means that the dimension is "variational"
  If var == GCE_NULL_V, then the value of dimension equals dimValue, else the value of dimension 
  is identically equal to a numerical variable 'var', i.e. the dimension is driving. \~
*/
//---
struct GCE_CLASS GCE_dim_pars
{
  var_item  var;           ///< \ru Значение размера, заданное переменной. \en Value of dimension specified by the variable.
  double    dimValue;      ///< \ru Значение размера, когда var == GCE_NULL_V. \en Value of dimension when var == GCE_NULL_V.

  GCE_dim_pars()
    : var( GCE_NULL_V )
    , dimValue( 0.0 )
    
  {}
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Параметры углового размерного ограничения.
            \en Parameters of angular dimensional constraint.

  \details  \ru Структура данных передает значение и тип измерения угловых размеров, создаваемых
  с помощью вызовов GCE_AddAngle, GCE_AddAngle4P. \n
  
  dPars    - задает величину углового размера и определяет тип размера, управляющий или вариационный.\n

  factor   - множитель для пересчета из присланного угла в переменную. 
             Используется для создания кратных углов, например двойного.\n
  adjacent -  смежный угол. Соответствует углу (M_PI - a), где 'a' - угол 
              между векторами, задающими направление линейного объекта.\n

  Значение поля данных dPars.dValue равно произведению factor*alpha, где alpha - угол между 
  линейными объектами, измеряемый в радианах. Поле данных dPars.var задает дескриптор переменной 
  вариационного размера, заданной в градусах. Если dPars.var == GCE_NULL_V, то размер является 
  управляющим, иначе - вариационным.

            \en The data structure passes specific parameters to create angular dimensions.
  In addition to the general settings passed via structure #GCE_dim_pars there is a flag
  of angle type and factor of conversion of angle to variable here. \n

  'dPars' specifies the value of the angular dimension and defines the type of dimension, driving or variational. \ n
  
  'factor' is a factor for converting from a given angle to variable. 
           It is used for creation of multiple angles, for instance, double angle.\n
  'adjacent' - adjacent angle. It corresponds to angle (M_PI - a), where 'a' is an angle 
              between vectors specifying the direction of a linear object.\n
  
  The value of the data field 'dPars.dValue' is equal to the product 'factor * alpha', where alpha 
  is the angle between linear objects measured in radians. The 'dPars.var' data field specifies 
  the variable defining variatinal dimension. Its value is given in degrees. 
  If dPars.var == GCE_NULL_V then the dimendion is driving, otherwise - variational.\n

  Angular dimension can be specified for any combination of linear objects. \~
*/
//---
struct GCE_CLASS GCE_adim_pars
{
  GCE_dim_pars dPars;     ///< \ru Общие настройки размера. \en General settings of dimension. 
  double       factor;    ///< \ru Множитель для пересчета из угла в переменную. \en Factor for converting from angle to variable. 
  bool         adjacent;  ///< \ru Смежный угол. \en Adjacent angle. 
  
  GCE_adim_pars() 
    : dPars()
    , factor( 1.0 )
    , adjacent( false )
  {}
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Параметры линейного размерного ограничения.
            \en Parameters of distance constraint.

  \details
  \ru dirAngle -  значение угла в радианах, задающее направление ориентируемых размеров. Пока 
                  используется только для #GCE_AddDirectedDistance2P. \n
  \en dirAngle -  value of angle in radians specifying the direction of oriented dimensions.
                  Now it is used only for #GCE_AddDirectedDistance2P. \n \~
*/
//---
struct GCE_CLASS GCE_ldim_pars
{
  GCE_dim_pars dPars;    ///< \ru Числовое значение размера, заданное переменной или числом double. \en Numeric value of dimension specified as a variable or simple double.
  double       dirAngle; ///< \ru Направление измерения (Используется только для ориентируемых размеров) \en Direction of dimension (It is used for oriented dimensions only) 
  geom_item    hp[2];    ///< \ru Пара дескрипторов вспомогательных точек размера. \en A pair of descriptors of help points of dimension.

  GCE_ldim_pars() : dPars(), dirAngle( 0.0 )
  {
    hp[0] = hp[1] = GCE_NULL_G;
  }
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Параметры линейного размера.
            \en Parameters of the linear dimension.
*/
//---
struct GCT_CLASS GCE_ldim_config
{
  GCE_dim_pars  parameter;  ///< \ru Числовое значение размера, заданное переменной или числом double. \en Numeric value of dimension specified as a variable or simple double.
  geom_item     direction;  ///< \ru Направляющая ось размера. \en A direction object that is usually has type `GCE_AXIS`.
  geom_item     hp[2];      ///< \ru Пара дескрипторов вспомогательных точек размера. \en A pair of descriptors of help points of dimension.
  GCE_alignment tanAlignment[2];  ///< Tangencial vector alignments.

  GCE_ldim_config()
    : parameter()
    , direction(GCE_NULL_G)
    , hp()
    , tanAlignment()
  {
    hp[0] = hp[1] = GCE_NULL_G;
    tanAlignment[0] = tanAlignment[1] = GCE_NO_ALIGNMENT;
  }
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Параметры интервального ограничения.
            \en Parameters of interval constraint.
    \details \ru Нижняя граница lBnd должна быть не больше верхней границы rBnd.
             \en The lower bound lBnd must be no more than the upper bound rBnd.
*/
//---
struct GCE_CLASS GCE_interval
{
  double lBnd; ///< \ru Минимальное значение переменной. \en Minimum value of a variable.
  double rBnd; ///< \ru Максимальное значение переменной. \en Maximum value of a variable.
  GCE_interval() : lBnd(0.0), rBnd(0.0) {};
};

//----------------------------------------------------------------------------------------
/** \brief \ru Контрольная точка драггинга. 
           \en Control point of the dragging.
    \details 
    \ru GCE_dragging_point::geom - Дескриптор геометрического объекта, выбранного для воздействия 
                                   с помощью функции драггинга ( #GCE_PrepareDraggingPoint).\n
        GCE_dragging_point::point - Дескриптор контрольной точки геометрического объекта драггинга.

    \en GCE_dragging_point::geom - Descriptor of a geometric object chosen to interact through 
                                   a dragging function (#GCE_PrepareDraggingPoint).\n
        GCE_dragging_point::point - Descriptor of control point of the dragging geometric object.
    \~
    \see #GCE_PrepareDraggingPoint, #GCE_MovePoint.
*/
//---
struct GCE_CLASS GCE_dragging_point
{
  geom_item geom;  ///< \ru Дескриптор геометрического объекта. \en Descriptor of the geometric object.
  geom_item point; ///< \ru Дескриптор контрольной точки геометрического объекта. \en Descriptor of the geometric object control point.
  GCE_dragging_point() : geom( GCE_NULL ), point( GCE_NULL ) {}
  GCE_dragging_point( geom_item g, geom_item pnt ) : geom( g ), point( pnt ) {}
};

//----------------------------------------------------------------------------------------
/** \brief \ru Диагностические характеристики системы ограничений.
           \en Diagnostic characteristics of constraint system. \~
    \note Used only for testing
*/
//---
struct GCE_CLASS GCE_diagnostic_pars
{
  size_t  consCount;    // A number of registered constraints.
  size_t  inConsCount;  // A number of internal constraints.
  double  reductCoef;   // Reduction ration of decomposition methods [percentage].
  GCE_diagnostic_pars() : consCount( 0 ), inConsCount( 0 ), reductCoef( 0. ) {}
};

//----------------------------------------------------------------------------------------
/** \brief \ru Диагностические характеристики системы ограничений.
           \en Diagnostic characteristics of constraint system. \~
    \note Used only for testing
*/
//---
struct GCT_CLASS GCT_diagnostic_pars
{
  size_t    consCount;    // A number of registered constraints.
  size_t    inConsCount;  // A number of internal constraints.
  double    reductCoef;   // Reduction ratio of decomposition methods [percentage].
  ptrdiff_t dof;          // Degree of freedom of a constraint system.
  GCT_diagnostic_pars() 
    : consCount(0)
    , inConsCount(0)
    , reductCoef(0)
    , dof(0) 
  {}
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Дополнительные данные колбэка #GCE_log_func, интерпретируемые на стороне приложения.
            \en Additional #GCE_log_func callback data interpreted on the application side. \~
    \sa #GCE_log_func
*/
//---
struct GCE_CLASS GCE_extra_param
{
  size_t  funcId;   // Integral identifier of a user-defined callback.
  void *  funcData; // Pointer to an application data structure.
  GCE_extra_param() { funcId = 0, funcData = 0; }
};

//----------------------------------------------------------------------------------------
// It represents a journal that logs Solver API transactions.
//---
struct ItJrnLogger;

//----------------------------------------------------------------------------------------
// It represents a journal that logs Solver API transactions.
//---
typedef struct
{
  const ItJrnLogger * logger; // Internal C3D Solver logger.
  GCE_extra_param     extra;  // Callback extra data interpreted in the application side.
} GCE_journal;

//----------------------------------------------------------------------------------------
// Type of log string.
//---
typedef enum
{
    GCE_LOG_JournalLine /// \ru Строка отчета о вызове Solver API. \en Reporting string about the Solver API call.
  , GCE_LOG_Message     /// \ru Строка передает некоторое информативное сообщение. \en The string notifies that something informative happend.
  , GCE_LOG_Error       /// \ru Строка извещает об ошибочной ситуации. \en Error notification.
} GCE_log_type;

//----------------------------------------------------------------------------------------
// Callback function for logging messages from the solver.
//---
typedef void ( *GCE_log_func )( GCE_journal, GCE_log_type logType, const char* recStr );

/**
  \}
  Constraints2D_API
*/


using query_geom_type = subgeom_type; // // Deprecated alias

#endif // __GCE_TYPES_H

// eof
