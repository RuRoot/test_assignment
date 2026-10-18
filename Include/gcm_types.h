//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Типы данных геометрического решателя.
         \en Data types of geometric solver. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GCM_TYPES_H
#define __GCM_TYPES_H

#include <system_types.h>
#include <math_define.h>

class  MbPlacement3D;     // Local coordinate system that represents position and orientation of 3D object.
struct MtSystemHolder {}; // An internal data object that provides the constraint system.

#define GCM_ID_TYPE     1 // 1 - MtObjectId is a pod struct,      0 - MtObjectId is simple integer.
#define GCM_SYSTEM_TYPE 0 // 1 - GCM_system is ptr <MtRefItem *>, 0 - GCM_system is a <MtSystemHolder*> ptr.

#if ( GCM_ID_TYPE == 1 )

struct GCM_CLASS MtObjectId { uint32 id; };
const MtObjectId _GCM_NULL = { SYS_MAX_UINT32 };
const MtObjectId _GCM_GROUND = { 0 };

#else // GCM_ID_TYPE

typedef uint32  MtObjectId;
const MtObjectId _GCM_NULL = SYS_MAX_UINT32;
const MtObjectId _GCM_GROUND = 0;

#endif // GCM_ID_TYPE


/** \addtogroup GCM_3D_API
    \{
*/
#if ( GCM_SYSTEM_TYPE == 1 )
class   MtRefItem;
/// \ru Система геометрических ограничений. \en System of geometric constraints. \~
typedef MtRefItem* GCM_system;
#else // GCM_SYSTEM_TYPE
/// \ru Система геометрических ограничений. \en System of geometric constraints. \~
typedef struct MtSystemHolder* GCM_system;
#endif // GCM_SYSTEM_TYPE

/// \ru Тип целочисленной степени свободы. \en Type of integer-valued degree of freedom.
typedef ptrdiff_t   GCM_dof_number;
/// \ru Дескриптор геометрического объекта, зарегистрированного в контексте решателя. \en Descriptor of geometrical object registered in the constraint system. 
typedef MtObjectId  GCM_object;
/// \ru Дескриптор геометрического объекта, зарегистрированного в контексте решателя. \en Descriptor of geometrical object registered in the constraint system. 
typedef GCM_object  GCM_geom;
/// \ru Дескриптор числовой переменной, зарегистрированной в системе ограничений. \en Descriptor of numeric variable registered in the constraint system.
typedef GCM_object  GCM_var;
/// \ru Дескриптор ограничения, зарегистрированного в решателе. \en Descriptor of a constraint registered in the solver. 
typedef GCM_object  GCM_constraint;
/// \ru Дескриптор паттерна, зарегистрированного в решателе. \en Descriptor of a pattern registered in the solver. 
typedef GCM_object  GCM_pattern;
/// \ru Дескриптор пустого объекта или ограничения. \en Descriptor of empty object or constraint. \~
const   GCM_object  GCM_NULL = _GCM_NULL;
/** \brief \ru Дескриптор неподвижного подмножества объектов, заданных в глобальной системой координат. 
           \en Descriptor of rigid subset of objects which are given in global coordinate system. \~
*/
const   GCM_geom    GCM_GROUND = _GCM_GROUND;

//----------------------------------------------------------------------------------------
/** \brief \ru Словарь типов аргументов геометрических ограничений.
           \en Dictionary of geometric argument types. \~
*/
// ---
typedef enum
{
  GCM_ARG_UNDEFINED = 0,  ///< \ru Аргумент не определен. \en Undefined argument.
  GCM_ARG_INTERVAL,       ///< \ru Интервальное число. \en Interval number.
  GCM_ARG_DESCRIPTOR,     ///< \ru Дескриптор переменной или геометрического объекта. \en Descriptor of variable or geometric object.
  GCM_ARG_NUMBER,         ///< \ru Число с плавающей точкой. \en A floating point number.
  GCM_ARG_ENUM,           ///< \ru Значение из перечислительного типа (дискретное множество). \en Value of some enumeration type variable.
} GCM_argument_type;

//----------------------------------------------------------------------------------------
/** \brief \ru Словарь типов геометрических примитивов.
           \en Dictionary of geometric primitives types. \~
*/
// ---
typedef enum 
{
  /*
    (!) Do not change the integral constants (they are written to file permanently).
  */
    GCM_NULL_GTYPE = 0  ///< \ru Пустой геометрический объект. \en Empty geometric object.
  , GCM_POINT       ///< \ru Точка.     \en Point.
  , GCM_LINE        ///< \ru Прямая.    \en Line.
  , GCM_PLANE       ///< \ru Плоскость. \en Plane.
  , GCM_CYLINDER    ///< \ru Цилиндр.   \en Cylinder.
  , GCM_CONE        ///< \ru Конус.     \en Cone.
  , GCM_SPHERE      ///< \ru Сферическая поверхность.  \en Spherical surface.
  , GCM_TORUS       ///< \ru Тороидальная поверхность. \en Toroidal surface.
  , GCM_CIRCLE      ///< \ru Окружность.        \en Circle.
  , GCM_LCS         ///< \ru Система координат. \en Coordinate system.
  , GCM_MARKER      ///< \ru Точка и пара ортонормированных векторов. \en Point and pair of orthonormalized vectors.
  , GCM_AXIS        ///< \ru Ось, заданная точкой и единичным вектор. \en Axis specified by point and unite vector.
  , GCM_SPLINE      ///< \ru Сплайновая кривая. \en Spline curve.
  , GCM_VECTOR      // Unit vector (internal use only)  
  , GCM_UNKNOWN_GTYPE // \ru Геометрический тип, не поддерживаемый решателем. \en Some geometric type, which is not supported by the solver. \~  
  , GCM_LAST_GTYPE    // \ru Количество типов. \en The count of types.
} GCM_g_type;

//----------------------------------------------------------------------------------------
/** \brief \ru Словарь типов ограничения.
           \en Dictionary of constraint types. \~

  \note \ru Значения этого перечисления могут быть использованы для постоянного 
            хранения и останутся неизменными в следующих версиях.
        \en Values of this enum can be used for permanent storage 
            and will be kept in the future versions. \~
*/
//---
typedef enum
{
  /*
    (!) Do not change the integral constants (they are written to file permanently).
  */
    GCM_UNKNOWN            = -1  ///< \ru Не определенный тип. \en Unknown type.
  , GCM_COINCIDENT         =  0  ///< \ru Геометрическое совпадение. \en Coincidence of loci.
  , GCM_PARALLEL           =  1  ///< \ru Параллельность двух объектов, имеющих направление. \en Parallelism of two objects which have a direction vector.
  , GCM_PERPENDICULAR      =  2  ///< \ru Перпендикулярность двух объектов, имеющих направление. \en Perpendicularity of two objects which have a direction vector.
  , GCM_TANGENT            =  3  ///< \ru Касание двух поверхностей или кривых. \en Tangency of two objects, surfaces and curves.
  , GCM_CONCENTRIC         =  4  ///< \ru Концентричность двух объектов, имеющих ось или центр. \en Concentricity of two objects having a center or an axis.
  , GCM_DISTANCE           =  5  ///< \ru Линейное размер между объектами. \en Linear dimension between objects.
  , GCM_ANGLE              =  6  ///< \ru Угловой размер между векторными объектами. \en Angular dimension between directed objects (vectors).
  , GCM_TRANSMISSION       =  9  ///< \ru Механическая передача. \en Mechanical transmission.
  , GCM_CAM_MECHANISM      = 10  ///< \ru Кулачковый механизм. \en Cam mechanism.
  , GCM_SYMMETRIC          = 11  ///< \ru Симметричность. \en Symmetry.
  , GCM_DEPENDENT          = 14  ///< \ru Зависимый объект. \en Dependent object.
  , GCM_PATTERNED          = 15  ///< \ru Элемент паттерна. \en Patterned object.
  , GCM_LINEAR_PATTERN     = 16  ///< \ru Линейный паттерн. \en Linear pattern.
  , GCM_ANGULAR_PATTERN    = 17  ///< \ru Угловой паттерн. \en Angular pattern.
  , GCM_CS_PATTERN         = 19  ///< \ru Паттерн - Система Координат (СК-паттерн). \en Coordinate System pattern.
  , GCM_PATTERN_COORDINATE = 20  ///< \ru Координата СК-паттерна. \en Coordinate of the Coordinate System patten.
  , GCM_RADIUS             = 18  ///< \ru Радиальный размер. \en Radial dimension.
  , GCM_LAST_CTYPE         = 100
  , GCM_IN_PLACE = 7 // Deprecated
} GCM_c_type;

//----------------------------------------------------------------------------------------
/** \brief  \ru Варианты выравнивания направлений. 
            \en Variants of alignment. \~
  \note \ru Значения этого перечисления могут быть использованы для постоянного 
            хранения и останутся неизменными в следующих версиях.
        \en Values of this enum can be used for permanent storage 
            and will be kept in the future versions. \~
*/
//---
typedef enum
{
  GCM_MIN_ALIGNMENT = -1,  // Minimum value of alignment.
  /*
    (!) Do not change the constants (they are written to file permanently).
  */  
  GCM_OPPOSITE     = -1,  ///< \ru Противонаправленные. \en Anti-align the directions. \~
  GCM_CLOSEST      =  0,  ///< \ru Ориентация согласно ближайшего решения. \en Orientation according to the nearest solution. \~
  GCM_COORIENTED   =  1,  ///< \ru Сонаправленные. \en Cooriented directions. \~  
  GCM_NO_ALIGNMENT =  2,  ///< \ru Нет определенной ориентации. \en No defined orientation. \~  
  /*
    Additional variants of alignment (they are used for tangency variants)
  */
  GCM_ALIGNED_0 = GCM_COORIENTED,
  GCM_ALIGNED_1 = 3,
  GCM_ALIGNED_2 = 4,
  GCM_ALIGNED_3 = 5,
  GCM_REVERSE_0 = GCM_OPPOSITE,
  GCM_REVERSE_1 = 6,
  GCM_REVERSE_2 = 7,
  GCM_REVERSE_3 = 8,
  /*
    Additional variants of alignment (they are used for patterns and symmetry).
  */  
  GCM_ALIGNED   = 1,  ///< \ru ЛСК с одинаковой ориентацией. \en Axis aligned local coordinate systems. \~
  GCM_ROTATED   = 9,  ///< Ротационное (вращательной) выравнивание элементов паттерна.
  GCM_ALIGN_WITH_AXIAL_GEOM = 10,  ///< Выровнять с объектом, задающим ось.  
  GCM_MAX_ALIGNMENT,  // Maximum value of alignment.
  GCM_ANY_ALIGNMENT   // It is used internally for matching to any alignment value.
  
} GCM_alignment;

//----------------------------------------------------------------------------------------
/** \brief  \ru Вариант углового размера. 
            \en Variant of angular dimension. \~
    \note   \ru Значения этого перечисления могут быть использованы для постоянного
                хранения данных приложения и останутся неизменными в следующих версиях.
            \en Values of this enum can be used for permanent storing of app data
                and will be kept in the future versions. \~
*/
//---
typedef enum
{
  GCM_NONE_ANGLE  = 0,  ///< \ru Неопределен \en Undefined
  GCM_2D_ANGLE    = 1,  ///< \ru Угол для планарных соединений (0 .. 360 градусов) \en Angle of planar joints (0 .. 360 degrees) 
  GCM_3D_ANGLE    = 2,  ///< \ru Угол в пространстве (0 .. 180 градусов) \en Angle in space (0 .. 180 degrees)   
  GCM_PLANAR_ANGLE = GCM_2D_ANGLE
} GCM_angle_type;

//----------------------------------------------------------------------------------------
/** \brief  \ru Варианты касания поверхностей или кривых. 
            \en Variants of tangency of surfaces or curves. \~
    \note \ru Значения этого перечисления могут быть использованы для постоянного
              хранения данных приложения и останутся неизменными в следующих версиях.
          \en Values of this enum can be used for permanent storage of app data and will 
              be kept in the future versions. \~
*/
//---
typedef enum
{
  /*
    (!) Do not change the constants
  */
    GCM_TAN_NONE   =  0x00  ///< \ru Не выбрано. \en Not chosen.
  , GCM_TAN_POINT  =  0x01  ///< \ru Касание в общем случае (контакт точкой). \en Tangency in general case (contact at a point). 
  , GCM_TAN_LINE   =  0x02  ///< \ru Касание по образующей прямой (например два цилиндра с параллельными осями). \en Tangency by a generating line (for instance, two cylinders with parallel axes). 
  , GCM_TAN_CIRCLE =  0x04  ///< \ru Касание по окружности (например сфера в конусе). \en Tangency by a circle (for instance, a sphere inside a cone).
} GCM_tan_choice;


//----------------------------------------------------------------------------------------
/** \brief  \ru Диагностические коды 3d-решателя. 
            \en Diagnostic codes of 3D-solver. \~
    \details \ru GCM_result перечисляет значения, возвращаемые вызовами API компонента GCM, 
  включая диагностические коды решения геометрических ограничений. Значения данного типа 
  возвращаются такими функциями, как GCM_Evaluate и GCM_EvaluationResult.
             \en GCM_result enumerates the values returned by the GCM API calls including 
  the diagnostic codes of solving geometric constraints. Values of this type are returned 
  by functions such as GCM_Evaluate and GCM_EvaluationResult.
    \note \ru Значения этого перечисления могут быть использованы для постоянного
              хранения данных приложения и останутся неизменными в следующих версиях.
          \en Values of this enum can be used for permanent storage of app data and will 
              be kept in the future versions. \~
*/
//---
typedef enum
{
    GCM_RESULT_None = 0                 ///< \ru  Код неопределенного результата или состояния. \en Code of undefined result or status. \~
  , GCM_RESULT_Ok = 1                   ///< \ru  Успешный результат вызова API компонента GCM. \en The successful result of GCM API call. \~
  , GCM_RESULT_Satisfied = GCM_RESULT_Ok ///< \ru  Ограничение или система ограничения решены. \en Constraint or system of constraints are fulfilled. \~
  , GCM_RESULT_Overconstrained = 2      ///< \ru  Ограничение переопределяет систему и противоречит другим условиям. \en Constraint is redundant and contradicts the other conditions. \~
  , GCM_RESULT_MatedFixation = 3        ///< \ru  Заданы ограничения для пары фиксированных объектов. \en Constraints are specified for pair of fixed objects. \~
  , GCM_RESULT_DraggingFailed = 4       ///< \ru  Неудачная попытка перемещения фиксированного объекта (равно, как объекта жестко-связанного с фиксированным). \en Failed attempt to move a fixed object (as the object rigidly connected with fixed). \~
  , GCM_RESULT_Not_Satisfied = 5         ///< \ru  Ограничение(я) не решено (по неизвестным причинам). \en Constraint(s) has not been solved (for unknown reasons). \~   
  , GCM_RESULT_Unsolvable = 6           ///< \ru  Ограничение(я) не разрешимо. \en Constraint(s) is not solvable. \~
  , GCM_RESULT_DependentConstraintUnsolved = 7 /**< \ru Ограничение GCM_DEPENDENT не вычислено или ее независимые аргументы находятся вне области решений. 
                                                    \en The GCM_DEPENDENT constraint is not solved or its independent arguments are out of the solution domain. \~  
                                                    \note  \ru Ситуация #GCM_RESULT_DependentConstraintUnsolved возникает, когда функция GCM_dependent_func возвращает false.
                                                    \en The situation #GCM_RESULT_DependentConstraintUnsolved occurs when the GCM_dependent_func function returns false. \~
                                               */
  , GCM_RESULT_Error = 8                    ///< \ru  Неизвестная ошибка, как правило, не связанная с процессом решения. \en  Unknown error is usually not related to the solving. \~  
  , GCM_RESULT_InappropriateAlignment = 9   ///< \ru  Опция выравнивания не подходит для данного типа ограничения. \en The alignment option is inappropriate to a given constraint type. \~
  , GCM_RESULT_InappropriateArgument = 10   ///< \ru  Геометрический тип аргумента не подходит для данного ограничения. \en Geometric type of an argument is inappropriate to the constraint. \~

  /*
    Additional message codes.
  */

  , GCM_RESULT_IncompatibleArguments = 3001   ///< \ru Несовместные типы аргументов ограничения. \en  Inconsistent types of constraint arguments. \~  
  , GCM_RESULT_InconsistentAlignment = 3003   ///< \ru Величина ориентации несовместна с другими сопряжениями. \en The orientation value is inconsistent with other mates. \~
  , GCM_RESULT_Duplicated                     ///< \ru Ограничение дублирует другое. \en Constraint duplicates another.
  , GCM_RESULT_CyclicDependence               ///< \ru Неразрешимая циклическая зависимость. \en Unsolvable cyclic dependence.
  , GCM_RESULT_MultiDependedGeom              ///< \ru Объект является зависимым от двух и более ограничений 'GCM_DEPENDED'. \en A geometric object is dependent on two or more constraints of 'GCM_DEPENDED' type.
  , GCM_RESULT_OverconstrainingDependedGeoms  ///< \ru Избыточное ограничение между зависимыми объектами. \en A redundancy constraint between depended geoms. \~  
  , GCM_RESULT_DependedGeomCantBeFixed        ///< \ru Зависимый аргумент ограничения 'GCM_DEPENDED' не может быть зафиксирован. \en The depended argument of 'GCM_DEPENDED' can't be fixed.  
  , GCM_RESULT_InvalidArguments               ///< \ru В ограничении не заданы аргументы (пустые аргументы). \en Constraint has invalid or undefined (void) arguments.
  , GCM_RESULT_UnsupportedTangencyChoice      ///< \ru Для сопряжения касание - опция выбора по окружности или по образующей не поддреживается \en  For mate the option of tangency choice by circle or generating curve is unsupported.
  , GCM_RESULT_InconsistentTangencyChoice     ///< \ru Для данной пары поверхностей касание по окружности геометрически не возможно \en  For a given pair of surfaces the touching along the circle is geometrically impossible.
  , GCM_RESULT_CoaxialGearTransmissionUnsupported ///< \ru Механическая передача вращения компонентов с совпадающими осями не поддерживается. \en Mechanical transmission of components rotation with the same axis is not supported.
  , GCM_RESULT_OverconstrainedCamMechanism      ///< \ru Имеются ограничения, создающие зависимость движения толкателя от движения кулачка, помимо самого кулачкового механизма. \en There are constraints creating dependence of the follower displacement from the cam in addition to the cam mechanism.
  , GCM_RESULT_CamMechanismsInCyclicDependency  ///< \ru Задана циклическая зависимость для двух или более кулачковых механизмов. \en Given the cyclic dependence for two or more cam mechanisms.
  , GCM_RESULT_InconsistentFollowerAxis         ///< \ru Заданные ограничения для толкателя не соответствую его оси движения. \en Given constraints for the follwer doesn't correspond to its motion axis.
  , GCM_RESULT_InconsistentPlanarAngle          ///< \ru Не соблюдаются условия планарного угла (векторы сторон угла должны быть перпендикулярны оси). \en Planar angle conditions are not met (vectors from the sides of angle should be perpendicular to the axis).
  , GCM_RESULT_UnsupportedFollowerSurface       ///< \ru Контактная поверхность толкателя, выбранная для кинематической пары "кулачек-толкатель", пока не поддерживается решателем. \en The follower contact surface selected for the cam-follower kinematic pair is not yet supported by the solver.
  , GCM_RESULT_UnsupportedDraggingOptions       ///< \ru Сочетание опций драггинга не задает поддерживаемый режим динамического репозиционирования. \en The combination of dragging options does not define a supported dynamic reposition mode.
  /*
    (!) Attention: New error messages should be added only before this line.
  */

  /*
    \ru Сообщения о некорректных результатах вызовов API решателя (не вычислительные). 
    \en Messages about incorrect results of the solver API calls (not computational). \~
  */
  , GCM_RESULT_ItsNotDrivingDimension ///< \ru Данное ограничение должно быть управляющим размером. \en Given constraint should be a driving dimension.
  , GCM_RESULT_IncorrectDimensionInterval  ///< \ru Задан некорректный интервал для размерного ограничения. \en Incorrect interval for interval dimension constraint.
  , GCM_RESULT_IncorrectGeomData  ///< \ru Некорректно заполнена структура `GCM_g_record`. \en Data structure `GCM_g_record` specified incorrectly.
  , GCM_RESULT_Unregistered           ///< \ru Обращение к недействительному объекту. \en Access to invalid object.
  , GCM_RESULT_ImmovableGeom        ///< \ru Геометрический объект не может быть перемещен. \en The geometric object cannot be moved.
  , GCM_RESULT_UnacceptableCluster  ///< \ru Кластер не является допустимым аргументом операции. \en The cluster is not acceptable as an argument of the operation.
  , GCM_RESULT_CyclicMove           ///< \ru Целевой кластер включен в перемещаемый геометрический объект или совпадает с ним. \en The target cluster is included into a geometric object subject to move or is the same one.
  , GCM_RESULT_InternalError
  , GCM_RESULT_ImmutableData
  , GCM_RESULT_Aborted  ///< \ru Процесс вычислений был прерван по запросу приложения. \en The evaluation process aborted by the application. \~
  , GCM_RESULT_Last_    // The last error code of user for mates (adding before this line)
} GCM_result;

//----------------------------------------------------------------------------------------
/** \brief  \ru Код означивает, какая имеется зависимость между аргументами ограничения.
            \en Code denotes what kind of dependency there is between arguments of constraint.
*/
// ---
typedef enum 
{
    GCM_NO_DEPENDENCY = 0 ///< \ru Нет односторонней зависимости. \en It means no one-directed dependency.
  , GCM_1ST_DEPENDENT = 2 ///< \ru Первый аргумент зависит от остальных. \en The first argument is dependent on the others.  
  , GCM_2ND_DEPENDENT = 1 ///< \ru Второй аргумент зависит от остальных. \en The second argument is dependent on the others.
} GCM_dependency;

//----------------------------------------------------------------------------------------
/// \ru Тип связи между элементами в паттерне. \en The type of relationship between elements in the pattern.
// ---
typedef enum
{
  GCM_NO_SCALE = 0,
  GCM_RIGID = 1,        ///< \ru Шаг между элементами константен. Паттерн не масштабируется (не растягивается). \en Distance between elements is constant. The pattern is not scaled.
  GCM_LINEAR_SCALE = 2, ///< \ru Шаг между элементами линейно масштабируется при растяжениях. \en Distance between elements is linearly scaled when stretching.
} GCM_scale;

//----------------------------------------------------------------------------------------
//  \ru Координаты объектов в трёхмерном пространстве. \en Coordinates of objects in 3D space.
// ---
typedef enum
{
  GCM_X_CRD       = 1,  ///< \ru Декартова или аффинная Х координата. \en Cartesian or affine X coordinate.
  GCM_Y_CRD       = 2,  ///< \ru Декартова или аффинная Y координата. \en Cartesian or affine Y coordinate.
  GCM_Z_CRD       = 3,  ///< \ru Декартова или аффинная Z координата. \en Cartesian or affine Z coordinate.
  GCM_RADIAL_CRD  = 4,  ///< \ru Радиальная координата. \en Radial distance.
  GCM_AZIMUTH_CRD = 5,  ///< \ru Азимут. \en Azimuth angle.
  GCM_POLAR_CRD   = 6,  ///< \ru Полярный угол. \en Polar angle.
  GCM_NULL_CRD = 1000,
} GCM_coord_name;

//----------------------------------------------------------------------------------------
/**
  \brief  \ru Режим поведения при манипулировании недоопределенной системой.
          \en Mode of the behavior when manipulating the undeconstrained system. \~
*/
// ---
typedef enum  
{
  /*
    Произвольное поведение (arbitrary behavior).
  */
    GCM_REPOSITION_FreeRotation = 1 ///< \ru Произвольная репозиция с преимуществом вращения. \en Arbitrary reposition with predominant rotation. \~
  , GCM_REPOSITION_FreeMoving   = 2 ///< \ru Произвольная репозиция с преимуществом перемещения. \en Arbitrary reposition with predominant moving. \~

  /*
    Строгое поведение (strict behavior).
  */
  , GCM_REPOSITION_Dragging     = 3///<  \ru Перетаскивание в плоскости "экрана". \en Dragging in the plane of the screen. \~
  , GCM_REPOSITION_Rotation     = 4///<  \ru Вращение вокруг неподвижной оси. \en Rotation around fixed axis. \~
  , GCM_REPOSITION_Transfer     = 5/**<  \ru Перенос только для одного твердого тела. \en Shift only one solid. \~
                                    \note \ru Этот режим #GCM_REPOSITION_Transfer был задуман для процессов вставки нового тела в сборку САПР.
                                    \en This mode #GCM_REPOSITION_Transfer have been intended for insertion processes of a new solid in the CAD assembly. \~
                                    */
  , GCM_REPOSITION_Inactive     = 1000
} GCM_reposition;

//----------------------------------------------------------------------------------------
/** \brief  \ru Основной вид движения, используемый для репозиции.
            \en Main kind of motion used for reposition. \~
    \note  \ru Это перечисление является одной из настроек драггинга (см. структуру данных #GCM_dragging).
           \en This enumeration is one of the dragging options (see data structure #GCM_dragging). \~
*/
//---
typedef enum
{
    GCM_DRAGGING_Undefined = 0  ///< \ru Вид движения не задан. \en Motion kind is not defined. \~
  , GCM_DRAGGING_Translation    ///< \ru Поступательное движение. \en Translational motion. \~
  , GCM_DRAGGING_TranslationXY  ///< \ru Поступательное движение в плоскости. \en Translational motion in plane. \~
} GCM_drag_motion;

//----------------------------------------------------------------------------------------
/** \brief  \ru Способ управления приводящими ограничениями для реализации движения.
            \en Manner of controlling driving constraints for performing motion. \~
    \note  \ru Это перечисление является одной из настроек драггинга (см. структуру данных #GCM_dragging).
           \en This enumeration is one of the dragging options (see data structure #GCM_dragging). \~
*/
//---
typedef enum
{
    GCM_DRAGGING_Adaptive = 0
  , GCM_DRAGGING_Specific
} GCM_drag_control;

//----------------------------------------------------------------------------------------
/** \brief  \ru Распространение влияния движения на связанные геометрические объекты.
            \en Propagation of motion influence onto connected geometric objects. \~
    \note  \ru Это перечисление является одной из настроек драггинга (см. структуру данных #GCM_dragging).
           \en This enumeration is one of the dragging options (see data structure #GCM_dragging). \~
*/
//---
typedef enum
{
    GCM_DRAGGING_Expanded = 0
  , GCM_DRAGGING_Relaxed
} GCM_drag_influence;

//----------------------------------------------------------------------------------------
/** \brief  \ru Набор опций драггинга.
            \en Composition of dragging options.
*/
//---
struct GCM_CLASS GCM_dragging
{
  GCM_drag_motion    motion;     ///< \ru Основной вид движения, используемый для репозиции. \en Main kind of motion used for reposition. \~
  GCM_drag_control   control;    ///< \ru Способ управления приводящими ограничениями для реализации движения. \en Manner of controlling driving constraints for performing motion. \~
  GCM_drag_influence influence;  ///< \ru Распространение влияния движения на связанные геометрические объекты. \en Propagation of motion influence onto connected geometric objects. \~

  bool operator == ( const GCM_dragging & other ) const
  { 
    return motion    == other.motion  &&
           control   == other.control &&
           influence == other.influence;
  }
  bool operator != ( const GCM_dragging& other ) const { return !(*this == other); }
};

//----------------------------------------------------------------------------------------
/// \ru Координаты 3D-вектора. \en Coordinates of 3D-vector.
//---
struct GCM_CLASS GCM_vec3d { double x, y, z; };

//----------------------------------------------------------------------------------------
/// \ru Координаты точки 3D пространства. \en Coordinates of point in three-dimensional space.
//---
struct GCM_CLASS GCM_point { double x, y, z; };

//----------------------------------------------------------------------------------------
/// \ru Интервал допустимых значений линейного или углового интервального размера. \en The range of allowable values for a linear or angular interval dimension.
//---
struct GCM_CLASS GCM_interval { double lBnd, rBnd; };

//----------------------------------------------------------------------------------------
/** \brief  \ru Структура полей данных, представляющих геометрический объект.
            \en Structure of data fields representing a geometric object. \~
  \details  \ru Эта простая структура данных представляет варианты геометрических типов, 
                с которыми работает решатель.\n
            \en This plain data structure represents variants of geometric data types that 
                the solver works with.\n 
            \~
  \par
  \ru Кортежи, соответствующие типам геометрии:\n
  \en Corresponding tuples of geometric types:\n

  \~ { GCM_POINT    origin } - simple point;\n
     { GCM_SPHERE   origin radiusA } - center and radius of a sphere;\n
     { GCM_LINE     origin axisZ } - point and direction of a line;\n
     { GCM_PLANE    origin axisZ } - point and normal of a plane;\n     
     { GCM_CIRCLE   origin axisZ radiusA } - center, rotation axis and radius;\n
     { GCM_CYLINDER origin axisZ radiusA } - center, rotation axis and radius;\n
     { GCM_CONE     origin axisZ radiusA radiusB } - center, rotation axis and two radiuses;\n
     { GCM_TORUS    origin axisZ radiusA radiusB };\n
     { GCM_LCS      origin axisZ axisX axisY } - local coordinate system that specify a solid position.\n
*/
//---
struct GCM_CLASS GCM_g_record
{
  GCM_g_type  type;     ///< \ru Тип геометрии. \en Type of geometric object.
  GCM_point   origin;   ///< \ru Точка позиционирования геометрического объекта. \en Location of a geometric object.
  GCM_vec3d   axisZ;    ///< \ru Направляющий вектор прямой или вектор нормали плоскости. \en Direction of line, normal of plane, Z-axis of a local coordinate frame.
  GCM_vec3d   axisX;    ///< \ru Ось X локальной системы координат. \en X-axis of local coordinate frame .
  GCM_vec3d   axisY;    ///< \ru Ось Y локальной системы координат. \en Y-axis of local coordinate frame.
  double      radiusA;  ///< \ru Радиус окружности, сферы или цилиндра либо радиус основания конуса, "большой" радиус тора. \en Radius of circle, sphere and cylinder or major radius of cone and torus.
  double      radiusB;  ///< \ru "Малый" радиус тора или конуса. \en Minor radius of cone and torus.
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Результат вычисления степени свободы геометрического объекта.
            \en Resulting code of the geometric degree of freedom computation.
  \attention Internal use only. The feature is under development.
*/
//---
typedef enum
{  
    GCM_DOF_RESULT_WellDefined   ///< \ru Геометрический объект полностью определен или заморожен (DOF=0). \en An geometric object is well defined, fixed or frozen (dof=0).
  , GCM_DOF_RESULT_UnderDefined  ///< \ru Геометрический объект имеет переносную или поворотную подвижность (DOF>0). \en An geometric object has translational or rotational freedom (dof>0).
  , GCM_DOF_RESULT_Unknown       ///< \ru Степень свободы геометрического объекта не вычислена. \en The geometric object state of freedom is not calculated.
} GCM_dof_result;

//----------------------------------------------------------------------------------------
/** \brief  \ru Количественная оценка степеней свободы геометрического объекта.
            \en An amount estimation for degrees of freedom of a geometric object.
    \note Internal use only. The feature is under development.
*/
//---
struct GCM_CLASS GCM_dof_estimation
{
  GCM_dof_number lowerBound;
  GCM_dof_number upperBound;
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Характеристика для значения величины, описывающей степень свободы геометрического объекта.
            \en A feature for a value of a quantity describing a degree of freedom of a geometric object.
    \note  \ru Только для внутреннего использования. Данный функционал находится в разработке.
           \en For internal use only. This feature is under development.
*/
//---
typedef enum
{   
    GCM_DOF_FEATURE_Unused = 0     ///< \ru Значение не используется в представлении. \en The value is not used for representation.
  , GCM_DOF_FEATURE_Instantaneous  ///< \ru Значение определено в мгновенном смысле. \en The value is defined in the instantaneous sense.
  , GCM_DOF_FEATURE_Free
  , GCM_DOF_FEATURE_Static
} GCM_dof_feature;

//----------------------------------------------------------------------------------------
/** \brief  \ru Точка c характеристикой, описывающая степень свободы.
            \en A featured point describing a degree of freedom.
    \note  \ru Только для внутреннего использования. Данный функционал находится в разработке.
           \en For internal use only. This feature is under development.
*/
//---
struct GCM_CLASS GCM_dof_point
{
  GCM_point value;
  GCM_dof_feature feature;
  
  operator GCM_point&() { return value; }
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Вектор c характеристикой, описывающий степень свободы.
            \en A featured vector describing a degree of freedom.
    \note  \ru Только для внутреннего использования. Данный функционал находится в разработке.
           \en For internal use only. This feature is under development.
*/
//---
struct GCM_CLASS GCM_dof_vector
{
  GCM_vec3d value;
  GCM_dof_feature feature;

  operator GCM_vec3d&() { return value; }
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Развернутая информация о трансляционных степенях свободы
                жесткого тела или точки в трехмерном пространстве.
            \en Detailed info on translational degrees of freedom
                of a rigid body or a point in the 3D-space.
    \details \ru Данная структура представляет результат работы функций анализа степеней свободы.
                 В случае успеха в `num` записывается число трансляционных степеней свободы,
                 а `dir1` и `dir2` опционально определяются соответствующими векторами возможных движений.
                 В случае неудачи в `num` записывается -1.
                 Если `num` равен 1, то определяется только `dir1`.
                 Если `num` равен 2, то определяются оба `dir1` и `dir2`.
                 Для вектора, который не был определен, гарантируется нулевое значение,
                 характеризуемое кодом `GCM_DOF_FEATURE_Unused`.
             \en This structure represents a result obtainable by functions implementing degrees-of-freedom analysis.
                 On success, `num` is written with number of translational degrees of freedom
                 while `dir1` and `dir2` are optionally defined by corresponding vectors of possible motions.
                 On failure, `num` is written with -1.
                 If `num` equals to `1`, then only `dir1` is defined.
                 If `num` equals to `2`, then both `dir1` and `dir2` are defined.
                 A vector left undefined is guaranted to be zero-valued and featured by code `GCM_DOF_FEATURE_Unused`.
    \note  \ru Только для внутреннего использования. Данный функционал находится в разработке.
           \en For internal use only. This feature is under development.
*/
//---
struct GCM_CLASS GCM_tdof_record
{
  GCM_dof_number num;   ///< \ru Число трансляционных степеней свободы. \en Number of translational degrees of freedom.
  GCM_dof_vector dir1;  ///< \ru Направляющий вектор первой допустимой трансляции. \en Direction vector of first permitted translation.
  GCM_dof_vector dir2;  ///< \ru Направляющий вектор второй допустимой трансляции. \en Direction vector of second permitted translation.
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Развернутая информация о ротационных степенях свободы
                жесткого тела в трехмерном пространстве.
            \en Detailed info on translational degrees of freedom
                of a rigid body in the 3D-space.
    \note  \ru Только для внутреннего использования. Данный функционал находится в разработке.
           \en For internal use only. This feature is under development.
*/
//---
struct GCM_CLASS GCM_rdof_record
{
  GCM_dof_number num;      ///< \ru Число ротационных степеней свободы. \en Number of rotational degrees of freedom..
  GCM_dof_point  center1;  ///< \ru Центральная точка первой допустимой ротации. \en Center point of first permitted rotation.
  GCM_dof_vector axis1;    ///< \ru Осевой вектор первой допустимой ротации. \en Axis vector of first permitted rotation
  GCM_dof_point  center2;  ///< \ru Центральная точка второй допустимой ротации. \en Center point of second permitted rotation.
  GCM_dof_vector axis2;    ///< \ru Осевой вектор второй допустимой ротации. \en Axis vector of second permitted rotation.
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Развернутая информация о степенях свободы
                жесткого тела в трехмерном пространстве.
            \en Detailed info on degrees of freedom
                of a rigid body in the 3D-space.
    \details \ru 
    \note  \ru Только для внутреннего использования. Данный функционал находится в разработке.
           \en For internal use only. This feature is under development.
*/
//---
struct GCM_CLASS GCM_dof_record
{  
  GCM_dof_number  num;   ///< \ru Формальное число степеней свободы. \en Formal number of degrees of freedom.
  GCM_tdof_record tdof;  ///< \ru Записанная информация о трансляционных степенях свободы. \en Recorded info on translational degrees of freedom.
  GCM_rdof_record rdof;  ///< \ru Записанная информация о ротационных степенях свободы. \en Recorded info on rotational degrees of freedom.
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Статус геометрического объекта.
            \en Status code of geometric object.
    \note Internal use only. The feature is under development.
*/
//---
typedef enum
{  
    GCM_G_STATUS_Frozen         ///<  \ru Геометрический объект лишен всех степеней свобод методом #GCM_FreezeGeom.
  , GCM_G_STATUS_WellDefined    /**<  \ru Состояние, когда все ограничения и размеры, определяющие объект, выполнены и полностью 
                                          устранили все его степени свободы.
                                      \en A state in which all constraints and dimensions that define an object have been 
                                          fullfilled so that all of the degrees of freedom are eliminated. \~
                                */    
  , GCM_G_STATUS_UnderDefined   ///< \ru Геометрический объект имеет переносную или поворотную подвижность (DOF>0).
  , GCM_G_STATUS_NotConsistent  ///< 
  , GCM_G_STATUS_NotSolved      ///<
  , GCM_G_STATUS_Unknown        ///< \ru Статус геометрического объекта не определен. \en The geomeric status is undefined.
} GCM_g_status;

//----------------------------------------------------------------------------------------
/** \brief  \ru Дополнительный параметр для функций типа #GCM_dependent_func.
            \en Additional parameter for functions of type #GCM_dependent_func. \~
    \sa #GCM_dependent_geom_func, #GCM_dependent_func
*/
//---
struct GCM_CLASS GCM_extra_param
{
  size_t  funcId;   // Integral identifier of a user-defined callback.
  void *  funcData; // Pointer to an application data structure.
  GCM_extra_param() { funcId = 0, funcData = 0; }
};

//----------------------------------------------------------------------------------------
// Resulting code is returned by callback ItGCBlackbox::CalculateDependent.
//
typedef enum
{
  GCM_DEP_RESULT_Ok,    // The dependent geom calculated successfully. The calculated value satisfies GCM_DEPENDENT constraint.
  GCM_DEP_RESULT_None,  // No result.
  GCM_DEP_RESULT_NoSolution, // There is no solution to the dependent object for the given the values of the independent objects.  
  GCM_DEP_RESULT_InputListInappropriate, // C3D Solver sent an inappropriate or deprecated list of independent geoms via callback. C3D Solver прислал неверный список независимых аргументов в функцию обратного вызова..
  GCM_DEP_RESULT_InternalError,
} GCM_dependent_result;

//----------------------------------------------------------------------------------------
/** \brief  \ru Результирующий код назначения журнала по вызову #GCM_SetJournal.
            \en Result code of the journal assignment by calling #GCM_SetJournal.
*/
//---
typedef enum
{     
    GCM_JRN_Success       ///< \ru Успешное исполнение GCM_SetJournal.
                          ///< \en Successful execution of GCM_SetJournal.
  , GCM_JRN_MissedCalls   ///< \ru GCM_SetJournal вызван слишком поздно, часть данных в системе ограничений осталась без журналирования.
                          ///< \en GCM_SetJournal was called too late; some constraint system data remained unlogged.                          
  , GCM_JRN_InvalidSystem ///< \ru Система ограничений не действительна.
                          ///< \en The constraint system is invalid.
  , GCM_JRN_Error         ///< \ru GCM_SetJournal не исполнен по неизвестной причине.
                          ///< \en GCM_SetJournal failed for an unknown reason.
} GCM_journal_result;

//----------------------------------------------------------------------------------------
// The function calculates position of a dependent geom regarding to other independent geoms.
/*
  Note: The dependent geom is first element of argument list inGeoms, and others are independent.
  argNb Number of arguments of dependency constraint, equals to size of inGeoms.
  g1 = f( g2 g3 ... gn );
*/
//---
typedef bool (*GCM_dependent_func) ( MbPlacement3D   gPlaces[]
                                   , size_t          gPlacesSize
                                   , GCM_extra_param exPar );

//----------------------------------------------------------------------------------------
/// \~ Alternative typename of #GCM_dependent_func.
//---                                        
typedef GCM_dependent_func GCM_dependent_geom_func;

/** \} */ // GCM_3D_API

//----------------------------------------------------------------------------------------
/// \~ Argument of constraint to record in 'GCM_c_record' data.
//--
struct GCM_CLASS GCM_c_arg
{
  union
  {
    GCM_object var;  // Numeric variable. 
    GCM_object geom; // Geometric object.
  };

  union
  {    
    GCM_alignment   alignVal;   // Variant of alignment.
    GCM_tan_choice  tanChoice;  // Option for tangency constraint only.
    GCM_angle_type  angType;    // Option for angular constraint only.
    GCM_scale       scale;      // Option for pattern constraint only.
    GCM_coord_name  crd;
    double          dimValue;   // Numeric value of a dimension.
    int64           enumVal;
    GCM_interval    dimInterval;
  };

  GCM_argument_type tag;  // Constraint argument type.

  GCM_c_arg & operator = ( double val )
  {
    tag = GCM_ARG_NUMBER;
    var = GCM_NULL;
    dimValue = val;
    return *this;
  }
  template<class _Enum>
  GCM_c_arg & operator = ( const _Enum & val )
  {
    tag = GCM_ARG_ENUM;
    var = GCM_NULL;
    enumVal = static_cast<int>( val );
    return *this; 
  }
  GCM_c_arg & operator = ( const GCM_object & varId ) 
  {
    tag = GCM_ARG_DESCRIPTOR;
    var = varId;
    dimValue = 0.0;
    return *this;
  }
  GCM_c_arg & operator = ( const GCM_interval & interval )
  {
    tag = GCM_ARG_INTERVAL;
    var = GCM_NULL;
    dimInterval = interval;
    return *this;
  }

  GCM_c_arg() { tag = GCM_ARG_UNDEFINED; dimValue = 0.0; var = GCM_NULL; }
};

//----------------------------------------------------------------------------------------
/** \brief  \en Structure of geometric constraint record.
            \ru Структура записи геометрического ограничения. \~
*/
/*
  The argument tuples of each constraint type:
  { GCM_c_type          GCM_c_arg  ...    GCM_c_arg }
  -------------------|-------------------------------
  { GCM_COINCIDENT      GCM_geom GCM_geom GCM_alignment }
  { GCM_CONCENTRIC      GCM_geom GCM_geom GCM_alignment }
  { GCM_PARALLEL        GCM_geom GCM_geom GCM_alignment }
  { GCM_PERPENDICULAR   GCM_geom GCM_geom GCM_alignment }  
  { GCM_IN_PLACE        GCM_geom GCM_geom GCM_NO_ALIGNMENT }
  { GCM_DISTANCE        GCM_geom GCM_geom double(or GCM_interval) GCM_alignment }
  { GCM_TANGENT         GCM_geom GCM_geom GCM_alignment GCM_tan_choice }
  { GCM_ANGLE           GCM_geom GCM_geom GCM_geom double GCM_alignment } - planar kind of angle
  { GCM_ANGLE           GCM_geom GCM_geom GCM_NULL double GCM_alignment } - 3d kind of angle  
  { GCM_SYMMETRIC       GCM_geom GCM_geom GCM_geom GCM_alignment }
  { GCM_PATTERNED       GCM_geom GCM_geom GCM_geom double GCM_alignment GCM_scale }
  { GCM_LINEAR_PATTERN  GCM_geom GCM_geom GCM_geom GCM_alignment }
  { GCM_ANGULAR_PATTERN GCM_geom GCM_geom GCM_geom GCM_alignment }
  { GCM_PATTERN_COORDINATE GCM_pattern, GCM_coord_name, GCM_geom }
  { GCM_TRANSMITTION    not specified }
  { GCM_CAM_MECHANISM   not specified }
  { GCM_RADIUS          GCM_geom double }
  { GCM_UNKNOWN }
  Sample of the journal line: (GCM_AddConstraint (GCM_COINCIDENT #1 #2 GCM_CLOSEST) #3)
*/
struct GCM_CLASS GCM_c_record
{
  static constexpr size_t argsN = 5;
  GCM_c_type type;        // \ru Тип ограничения. \en Type of constraint.
  GCM_c_arg  args[argsN]; // \ru Аргументы ограничения. \en Arguments of constraint.
};

#if ( GCM_ID_TYPE == 1 )

inline bool operator == ( const MtObjectId & f, const MtObjectId & s ) { return f.id == s.id; }
inline bool operator != ( const MtObjectId & f, const MtObjectId & s ) { return f.id != s.id; }
inline bool operator <  ( const MtObjectId & f, const MtObjectId & s ) { return f.id < s.id;  }
inline       uint32 & _id( MtObjectId & obj ) { return obj.id; }
inline const uint32 & _id( const MtObjectId & obj ) { return obj.id; }

#else // GCM_ID_TYPE

inline       uint32 & _id( MtObjectId & obj ) { return obj; }
inline const uint32 & _id( const MtObjectId & obj ) { return obj; }

#endif // GCM_ID_TYPE

//----------------------------------------------------------------------------------------
/** \brief  \ru Скалярные параметры геометрического ограничения или размера.
            \en Scalar parameters of geometric constraint or dimension.
*/
//---
struct GCM_CLASS GCM_closest_params
{
  GCM_result      result;   // The result of a query GCM_ClosestParameters.
  GCM_c_arg       dimValue; // Measured dimension value.
  GCM_alignment   alignVal; // Estimated alignment option.
  GCM_tan_choice  tanChoice;// Estimated tangency choice (it is yet always = GCM_TAN_NONE, i.e. yet not used in the query of closest params).
  GCM_closest_params()
    : result( GCM_RESULT_None )
    , dimValue()
    , alignVal( GCM_NO_ALIGNMENT )
    , tanChoice( GCM_TAN_NONE ) {}
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
  GCM_extra_param     extra;  // Callback extra data interpreted in the application side.
} GCM_journal;

//----------------------------------------------------------------------------------------
// Type of log string.
//---
typedef enum
{    
    GCM_LOG_JournalLine /// \ru Строка отчета о вызове Solver API. \en Reporting string about the Solver API call.
  , GCM_LOG_Message     /// \ru Строка передает некоторое информативное сообщение. \en The string notifies that something informative happend.
  , GCM_LOG_Error       /// \ru Строка извещает об ошибочной ситуации. \en Error notification.
} GCM_log_type;

//----------------------------------------------------------------------------------------
// Callback function for logging messages from the solver.
//---
typedef void ( *GCM_log_func )( GCM_journal, GCM_log_type logType, const char* recStr );

//----------------------------------------------------------------------------------------
// 
//---
typedef GCM_alignment MtAlignType;
typedef GCM_g_type    MtGeometryType;
typedef GCM_result    MtResultCode3D;

static const GCM_c_type GCM_TRANSMITTION = GCM_TRANSMISSION;
static const GCM_result mtResCode_Ok          = GCM_RESULT_Ok;
static const GCM_result mtResCode_SystemError = GCM_RESULT_Error;
static const GCM_result mtResCode_Error       = GCM_RESULT_Error;
static const GCM_result mtResCode_MovingOfFixedGeom = GCM_RESULT_DraggingFailed;
static const GCM_result mtResCode_InvalidAxisOfPlanarAngle = GCM_RESULT_InconsistentPlanarAngle;
static const GCM_result mtResCode_InconsistentFollowerAxis = GCM_RESULT_InconsistentFollowerAxis; // deprecated 2020.
static const GCM_result mtResCode_NoSeparatedSolutionForCamGear = GCM_RESULT_OverconstrainedCamMechanism; // deprecated 2020.
static const GCM_result GCM_RESULT_UndefinedAngleType = static_cast<GCM_result>(3002);  // deprecated 2023.
static const GCM_result GCM_RESULT_InconsistentAngleType = GCM_RESULT_UndefinedAngleType; // deprecated 2020.
static const GCM_result mtResCode_UnsupportedTangencyChoice = GCM_RESULT_UnsupportedTangencyChoice; // deprecated 2023.
static const GCM_result mtResCode_IsNoPossibleForCircTanChoice = GCM_RESULT_InconsistentTangencyChoice; // deprecated 2023.
static const GCM_result mtResCode_CoaxialMtGearTransmissionIsNotAvalable = GCM_RESULT_CoaxialGearTransmissionUnsupported; // deprecated 2023.
static const GCM_result mtResCode_CyclicDependenceForTwoOrMoreCamGears = GCM_RESULT_CamMechanismsInCyclicDependency; // deprecated 2023.


const GCM_dependency GCM_2ST_DEPENDENT = GCM_2ND_DEPENDENT;

#endif

// eof
