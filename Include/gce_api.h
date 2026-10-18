//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file     
  \brief \ru Программный интерфейс решателя геометрических ограничений.
         \en Program interface of geometric constraints solver. \~
  \details \ru Программный  интерфейс  геометрического  решателя представляет
            собой набор типов данных и функций, необходимых для решения 
            задачи геометрических ограничений. Предметная область решателя
            предусматривает такие типы, как "геометрический объект", 
            "геометрическое ограничение", "система ограничений". Базовые типы
            решателя объявлены в заголовочном файле <gce_types.h>. Вызовы функций 
            и их аргументы подобраны таким образом, что бы наиболее удобно 
            осуществлять формулировку задачи для решателя в терминах объектов 
            и ограничений. Названия многих типов данных и вызовов API начинаются 
            префиксом GCE, сокращенно Geometric Constraint Engine. \n

            Функции API решателя можно подразделить на такие группы:
            1)  Функции #GCE_CreateSystem, #GCE_ClearSystem, #GCE_RemoveSystem 
                  позволяют создавать и удалять систему ограничений в целом
                  (должны вызываться в однопоточном режиме);\n
            2)  С помощью функций вида GCE_Add_XXXXXXX осуществляется формулировка 
                  задачи ограничений, с их помощью в систему добавляются объекты и 
                  ограничения (могут использоваться в параллельном режиме);\n
            3)  Функции вида GCE_Change_XXXXXXX, GCE_Set_XXXXXXX позволяют менять 
                  размеры и состояние объектов (могут использоваться в параллельном режиме);\n
            4)  Функции для запросов такие, как GCE_Get_XXXXXXX, #GCE_SplinePoint, 
                  GCE_IsXXXXX, #GCE_PointDOF и т.д. позволяют осуществлять запросы 
                  о состоянии объектов или их свойств, узнать степень свободы объектов 
                  и прочие характеристики (могут использоваться в параллельном режиме);\n
            5)  Метод #GCE_Evaluate вычисляет состояния системы ограничений, в котором 
                  все ограничения удовлетворены, или возвращает код ошибки при невозможности 
                  найти решение (должна вызываться в однопоточном режиме).\n 
            6)  Другая группа вызовов отвечает за способы управления недоопределенной 
                  системой ограничений. Вызовы #GCE_PrepareDraggingPoint, #GCE_MovePoint
                  обеспечивают интерактивную манипуляцию объектами чертежа/эскиза.\n
           \en A program interface of geometric solver represents
            a set of data types and functions necessary for solution 
            of a problem of geometric constraints. Subject area of the solver
            provides such types as "geometric object", "geometric constraint", 
            "constraint system". Base types of the solver are declared in the header 
            file <gce_types.h>. Calls of functions and their arguments are chosen 
            in such way that the formulation of the problem for the solver in terms 
            of objects and constraints could be performed by the most convenient way. 
            The names of many data types and API calls begins with a prefix 'GCE', 
            abbreviation for Geometric Constraint Engine. \n

            API functions of solver can be subdivided into the following groups:
            1) Functions #GCE_CreateSystem, GCE_ClearSystem, GCE_RemoveSystem 
                allow to create and delete the system of constraints in general
                (should be called in sequential code);\n
            2) The problems of constraints are formulated with a function of a kind GCE_Add_XXXXXXX, 
                by using them the objects and constraints are added to the system
                (could be called in multi-threaded mode);\n 
            3) Functions of a kind GCE_Change_XXXXXXX, GCE_Set_XXXXXXX allow to change dimensions 
               and objects states (could be called in multi-threaded mode);\n
            4) Functions for requests, such as #GCE_Get_XXXXXXX, #GCE_SplinePoint, 
                #GCE_IsXXXXX, #GCE_PointDOF etc, allow to perform requests 
                about the states of objects or their properties, find the objects degree of freedom 
                and other characteristics could be called in multi-threaded mode;\n
            5) The method #GCE_Evaluate calculates the state of the constraint system, where 
                all constraints are satisfied or returns an error code if it is not possible 
                to find a solution (should be called in sequential code). \n 
            6) Other group of calls responses for the ways of control of underdetermined 
                system of constraints. Calls of  #GCE_PrepareDraggingPoint, #GCE_MovePoint
                provide interactive manipulation with objects of drawing/sketch.\n \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GCE_API_H
#define __GCE_API_H

#include <math_define.h>
#include <gce_types.h>
#include <vector>

class  MbMatrix;
class  MbCurve;

/**
  \addtogroup Constraints2D_API   
  \{
*/

//----------------------------------------------------------------------------------------
/** \brief    \ru Создать пустую систему ограничений.
              \en Create a simple constraint system. \~  
    \details  \ru Вызов создает пустую систему ограничений. Кроме того, в памяти 
  создаются внутренние структуры данных геометрического решателя, обслуживающего 
  систему ограничений. Функция возвращает специальный дескриптор, по которому
  система ограничений доступна для различных манипуляций: добавление или удаление 
  геометрических объектов, ограничений, варьирование размеров, драггинг недоопределенных 
  объектов и т.д.
              \en The call creates a simple constraint system. Besides, inside the memory 
  there are created internal data structures of geometric solver maintaining 
  the system of constraints. The functions returns a special descriptor by which
  the constraint system is available for various manipulations: addition and deletion 
  of geometric objects, constraints, variation of sizes, dragging underconstrained objects 
  etc. \~
          
    \return \ru Дескриптор системы ограничений.
            \en Descriptor of  constraint system. \~
*/
//---
GCE_FUNC(GCE_system) GCE_CreateSystem();

//----------------------------------------------------------------------------------------
/** \brief \ru Сделать систему ограничений пустой.
           \en Make the constraint system empty. \~
  \details \ru Данный метод делает систему ограничений пустой при этом 
  дескриптор gSys остается действительным, т.е. можно осуществлять дальнейшую 
  работу с системой ограничений.
           \en This method makes the constraint system empty while 
  the descriptor gSys remains valid, i.e. it is possible to perform the further 
  work with the constraint system. \~

  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \sa #GCE_RemoveSystem
*/
//---
GCE_FUNC(void) GCE_ClearSystem( GCE_system gSys );

//----------------------------------------------------------------------------------------
/** \brief \ru Удалить систему ограничений.
           \en Delete system of constraints. \~    
    \details  \ru Данный метод освобождает память от внутренних структур данных, обслуживающих 
    систему ограничений. Удаляемая система ограничений становится недействительной после 
    данного вызова.
              \en This method releases memory from internal data structures maintaining 
   the constraint system. The removed constraint system is invalidated after this call.             
              \~
    \param[in] gSys - \ru Система ограничений.
                      \en System of constraints. \~
    \sa #GCE_ClearSystem
*/
//---
GCE_FUNC(void) GCE_RemoveSystem( GCE_system gSys );

//----------------------------------------------------------------------------------------
/** \brief \ru Добавить в систему ограничений точку.
           \en Add point to the constraint system. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] pVal - \ru Координаты точки.
                    \en Point coordinates. \~
  \return \ru Дескриптор зарегистрированной точки.
          \en Descriptor of registered point. \~
*/
//---
GCE_FUNC(geom_item) GCE_AddPoint( GCE_system gSys, GCE_point pVal );

//----------------------------------------------------------------------------------------
/** \brief \ru Добавить в систему ограничений ось.
           \en Add axis to the constraint system. \~
  \param[in] \ru gSys Система ограничений.
             \en gSys System of constraints. \~
  \param[in] \ru eVal Координаты начала отсчета оси.
             \en eVal Axis coordinates. \~
  \return \ru Дескриптор зарегистрированной оси.
          \en Descriptor of registered axis. \~
*/
//---
GCE_FUNC(geom_item) GCE_AddAxis( GCE_system gSys, const GCE_axis & eVal );

//----------------------------------------------------------------------------------------
/** \brief \ru Добавить в систему ограничений прямую.
           \en Add line to the constraint system. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] lVal - \ru Координаты прямой.
                    \en Line coordinates. \~
  \return \ru Дескриптор зарегистрированной прямой.
          \en Descriptor of registered line. \~
*/
//---
GCE_FUNC(geom_item) GCE_AddLine( GCE_system gSys, const GCE_line & lVal );

//----------------------------------------------------------------------------------------
/** \brief \ru Добавить в систему ограничений отрезок прямой, заданный парой концевых точек.
           \en Add a line segment specified by pair of end points to the constraint system. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] p    - \ru Дескрипторы конечных точек отрезка.
                    \en Descriptors of end points of the line segment. \~
  \return \ru Дескриптор зарегистрированного отрезка.
          \en Descriptor of registered segment. \~
  \details \ru Для отрезка, созданного через данный вызов, действительны все типы 
            ограничений, которые применимы для прямой, создаваемой вызовом GCE_AddLine.
           \en All types of constraints which are applicable to the line created by GCE_AddLine 
            are valid for the segment created by this call. \~
*/
//---
GCE_FUNC(geom_item) GCE_AddLineSeg( GCE_system gSys, geom_item p[2] );

//----------------------------------------------------------------------------------------
/** \brief \ru Добавить в систему ограничений окружность.
           \en Add circle to the constraint system. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] cVal - \ru Координаты окружности.
                    \en Coordinates of a circle. \~
  \return \ru Дескриптор зарегистрированной окружности.
          \en Descriptor of the registered circle. \~
*/
//---
GCE_FUNC(geom_item) GCE_AddCircle( GCE_system gSys, const GCE_circle & cVal );

//----------------------------------------------------------------------------------------
/** \brief \ru Добавить в систему ограничений эллипс.
           \en Add ellipse to the constraint system. \~
  \param[in] \ru gSys Система ограничений.
             \en gSys System of constraints. \~
  \param[in] \ru eVal Координаты эллипса.
             \en eVal Ellipse coordinates. \~
  \return \ru Дескриптор зарегистрированного эллипса.
          \en Descriptor of registered ellipse. \~
*/
//---
GCE_FUNC(geom_item) GCE_AddEllipse( GCE_system gSys, const GCE_ellipse & eVal );

//----------------------------------------------------------------------------------------
/** \brief \ru Добавить в систему ограничений сплайн (NURBS)
           \en Add spline (NURBS) to the constraint system \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] spl  - \ru Координаты сплайна.
                    \en Spline coordinates. \~
  \return \ru Дескриптор зарегистрированного сплайна.
          \en Descriptor of registered spline. \~
*/
//---
GCE_FUNC(geom_item) GCE_AddSpline( GCE_system gSys, const GCE_spline & spl );

//----------------------------------------------------------------------------------------
/** \brief \ru Добавить в систему ограничений параметрическую кривую.
           \en Add parametric curve to the constraint system. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] crv  - \ru Математическое описание параметрической кривой.
                    \en Mathematical description of parametric curve. \~
  \return \ru Дескриптор зарегистрированной параметрической кривой.
          \en Descriptor of registered parametric curve. \~
  \attention \ru Время жизни экземпляра класса crv опирается на счетчик ссылок, т.е.
              решатель его увеличивает при добавлении параметрической кривой и
              декрементирует при удалении кривой из решателя.
             \en The lifetime of the instance of the class 'crv' is based on the reference counter, i.e.
              the solver increases it when adding a parametric curve and
              decreases when deleting a curve from the solver. \~
*/
//---
GCE_FUNC(geom_item) GCE_AddParametricCurve( GCE_system gSys, const MbCurve & crv );

//----------------------------------------------------------------------------------------
/** \brief \ru Добавить в систему граничную кривую, ограниченную парой точек.
           \en Add a curve bounded by a pair of points to the constraint system. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] crv  - \ru Дескриптор базовой геометрической кривой. Базовой кривой может
                    быть только кривая одного из следующих типов: прямая, окружность,
                    эллипс, сплайн или параметрическая кривая.
                    \en Descriptor of base geometric curve. Base curve may
                  be only one curve from the following types: line, circle, 
                  ellipse, spline or parametric curve. \~
  \param[in] p    - \ru Пара дескрипторов начальной и конечной точек участка кривой.
                    \en A pair of descriptors of the beginning and ending points of curve piece. \~
  \return \ru Дескриптор зарегистрированной ограниченной кривой.
          \en Descriptor of registered bounded curve. \~
*/
//---
GCE_FUNC(geom_item) GCE_AddBoundedCurve( GCE_system gSys, geom_item curve, geom_item p[2] );

//----------------------------------------------------------------------------------------
/** \brief \ru Добавить в систему ограничений эквидистантную кривую.
           \en Add an equidistant curve to the constraint system. \~
    \details \ru Базовой кривой может быть только кривая одного из следующих типов: точка, прямая, отрезок, окружность,
                 эллипс, сплайн, ограниченная кривая, параметрическая кривая или эквидистантная кривая.
             \en Base curve may be only a curve of one of the following types: point, line, segment, circle,
                 ellipse, spline, bounded curve, parametric curve, or equidistant curve. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] crv  - \ru Дескриптор базовой геометрической кривой. Базовой кривой может
                    быть только кривая одного из следующих типов: точка, прямая, отрезок, окружность,
                    эллипс, сплайн, ограниченная кривая, параметрическая кривая или эквидистантная кривая.
                    \en Descriptor of base geometric curve. Base curve may
                    be only a curve of one of the following types: point, line, segment, circle,
                    ellipse, spline, bounded curve, parametric curve, or equidistant curve. \~
  \param[in] offset - \ru Величина смещения эквидистантной кривой относительно базовой.
                      \en Offsets of the equidistant curve relative to the base curve. \~
  \return \ru Дескриптор зарегистрированной эквидистантной кривой.
          \en Descriptor of registered equidistant curve. \~
*/
//---
GCE_FUNC(geom_item) GCE_AddOffsetCurve( GCE_system gSys, geom_item curve, double offset );

//----------------------------------------------------------------------------------------
/** \brief \ru Объявить паттерн с направлением вдоль прямой и заданным смещением или с центром в точке и заданным углом.
           \en Declare a pattern with a direction along a line and a given offset or with the center at a point and a given angle. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] geom   - \ru Дескриптор точки или прямой.
                      \en Descriptor of line. \~
  \param[in] step   - \ru Величина смещения паттерн.
                      \en Pattern offset. \~
  \return \ru Дескриптор зарегистрированного паттерна.
          \en Descriptor of registered pattern. \~

  \details \ru Метод создает паттерн. Если объект прямая или отрезок, то создается линейный паттерн с направлением
               вдоль данной прямой или отрезком и шагом step. Если объект точка, окружность или эллипс,
               то создается угловой паттерн с центром в данной точке или центре окружности или эллипса и углом step.
           \en The method creates a pattern. If the object is a line or a segment, then a linear pattern
               is created with the direction along this line or segment and a step. If the object is a point,
               circle or ellipse, then an angular pattern is created with the center at the given point
               or center of the circle or ellipse and the step angle. \~
*/
//---
GCE_FUNC(pattern_item) GCE_AddPattern( GCE_system gSys, geom_item geom, double step );

//----------------------------------------------------------------------------------------
/** \brief \ru Объявить линейный паттерн с шагом смещения, заданным вектором трансляции.
           \en Declare a linear pattern with the step given by the translation vector. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] trans  - \ru Вектор трансляции.
                      \en Offset vector. \~
  \return \ru Дескриптор зарегистрированного паттерна.
          \en Descriptor of registered pattern. \~

  \details \ru Метод создает линейный паттерн со смещением, заданным данным вектором трансляции.
           \en The method creates a linear pattern with the step given by this translation vector. \~
*/
//---
GCE_FUNC(pattern_item) GCE_AddLinearPattern( GCE_system gSys, GCE_vec2d trans );

//----------------------------------------------------------------------------------------
/** \brief \ru Объявить угловой паттерн c центром и углом.
           \en Declare an angular pattern with a center and an angle. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] point  - \ru Точка - центр паттерна.
                      \en The point is the center of the pattern. \~
  \param[in] angle  - \ru Угол поворота.
                      \en Angle of rotation. \~

  \return \ru Дескриптор зарегистрированного паттерна.
          \en Descriptor of registered pattern. \~

  \details \ru Метод создает угловой паттерн, заданный центром и углом.
           \en The method creates an angular pattern defined by a center and an angle. \~
*/
//---
GCE_FUNC(pattern_item) GCE_AddAngularPattern( GCE_system gSys, GCE_point point, double angle );

//----------------------------------------------------------------------------------------
/** \brief \ru Объявить паттерн отражения по линейному объекту.
           \en Declare a reflection pattern on a linear object. \~
  \param[in] gSys   -  \ru Система ограничений.
                       \en System of constraints. \~
  \param[in] geom  -   \ru Дескриптор линейного объекта - ось зеркальной симметрии.
                       \en Linear object descriptor, i.e. reflection symmetry axis. \~ 

  \return \ru Дескриптор зарегистрированного паттерна.
          \en Descriptor of registered pattern. \~

  \details \ru Метод создает паттерн отражения относительно линейного объекта.
           \en The method creates a reflection pattern relative to a linear object. \~
*/
//---
GCE_FUNC(pattern_item) GCE_AddSymmetricPattern( GCE_system gSys, geom_item geom );

//----------------------------------------------------------------------------------------
/** \brief \ru Объявить составной паттерн по двум данным.
           \en Declare a composition pattern of two patterns. \~
  \param[in] gSys   -  \ru Система ограничений.
                       \en System of constraints. \~
  \param[in] pItem1  - \ru Первый паттерн.
                       \en The first pattern. \~
  \param[in] pItem2  - \ru Второй паттерн.
                       \en The second patttern. \~

  \return \ru Дескриптор зарегистрированного паттерна.
          \en Descriptor of registered pattern. \~

  \details \ru Метод создает паттерн - композицию двух паттернов.
           \en The method creates an composition pattern defined by two patterns. \~
*/
//---
GCE_FUNC(pattern_item) GCE_AddCompositionPattern( GCE_system gSys, pattern_item pItem1, pattern_item pItem2 );

//----------------------------------------------------------------------------------------
/** \brief \ru Создать k-й экземпляр образца в данном паттерне.
           \en Create k-th instance of the sample in a given pattern. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] pItem  - \ru Дескриптор паттерна.
                      \en Descriptor of pattern. \~
  \param[in] sample - \ru Дескриптор образца.
                      \en Descriptor of sample. \~
  \param[in] k      - \ru Номер экземпляра.
                      \en Copy number. \~
  \return \ru Дескриптор зарегистрированного экземпляра.
          \en Descriptor of registered instance. \~

  \details \ru Метод создает k-й экземпляр образца в данном паттерне.
               При k = 0 возвращает идентификатор образца.
           \en The method creates the k-th instance of the sample in the given pattern.
               It returns the sample descriptor if k = 0. \~
*/
//---
GCE_FUNC(geom_item) GCE_AddInstance( GCE_system gSys, pattern_item pItem, geom_item sample, int k );

//----------------------------------------------------------------------------------------
/** \brief \ru Создать (i, j) экземпляр образца в данном составном паттерне.
           \en Create (i, j) instance of the sample in a given composition pattern. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] pItem  - \ru Дескриптор паттерна.
                      \en Descriptor of pattern. \~
  \param[in] sample - \ru Дескриптор образца.
                      \en Descriptor of sample. \~
  \param[in] i      - \ru Номер экземпляра.
                      \en Copy number. \~
  \param[in] j      - \ru Номер экземпляра.
                      \en Copy number. \~

  \return \ru Дескриптор зарегистрированного экземпляра.
          \en Descriptor of registered instance. \~

  \details \ru Метод создает (i, j) экземпляр образца в данном составном паттерне.
               При i = 0, j = 0 возвращает идентификатор образца.
           \en The method creates the (i, j) instance of the sample in the given pattern.
               It returns the sample descriptor if i = 0 and j = 0. \~
*/
//---
GCE_FUNC(geom_item) GCE_AddInstance( GCE_system gSys, pattern_item pItem, geom_item sample, int i, int j );

//----------------------------------------------------------------------------------------
/** \brief \ru Добавить в систему жёсткое множество геометрических объектов.
           \en Add a rigid set of geometric objects to the system. \~
    \details \ru Жёсткое множество - это массив геометрических объектов, зафиксированных друг относительно друга.
    Жёсткое множество представляет собой геометрический объект, для которого доступен весь функционал
    работы с геометрическими объектами. Например, у него можно спросить тип (#GCE_GeomType -> GCE_SET) или запросить
    положение. С помощью вызовов #GCE_GetPointXY и #GCE_GetCoordValue можно получить начало координат и направление оси OX
    ЛСК жёсткого множества. Чтобы удалить жёсткое множество, надо, как и для любого другого геометрического объекта,
    вызвать функцию #GCE_RemoveGeom. При этом составляющие жёсткое множество объекты (geoms) при удалении жёсткого
    множества не удаляются и могут далее быть использованы в решателе. С геометрическими объектами, образующими
    жёсткое множество, нужно работать точно так же, как и до их добавления в жёсткое множество. Например, для наложения
    ограничения между элементом жёсткого множества и любым другим геометрическим объектом необходимо в
    качестве аргумента ограничения указывать не дескриптор жёсткого множества, которому данный объект принадлежит, а
    дескриптор самого геометрического объекта из массива geoms, на который накладывается ограничение.
    \en  A rigid set is an array of geometric objects which are fixed relative to each other. It is considered as a
    geometric object and hence all the functionality for working with geometric objects is available for it. For
    example, it's possible to request its type (#GCE_GeomType -> GCE_SET) or get its position invoking #GCE_GetPointXY
    and #GCE_GetCoordValue to get the origin and the direction of the OX axis of the LCS of the rigid set. To remove
    a rigid set it's necessary to call the function #GCE_RemoveGeom. Geometric objects (geoms) are not deleted together
    with a rigid set and can be used in the solver after it will be deleted. With geometric objects that have been
    included in a rigid set it is necessary to continue to work just as before adding them to a rigid set. For
    instance, to specify a constraint between an element of a rigid set and any other geometric object, it is necessary
    to specify as the constraint argument not the descriptor of the rigid set to which the object belongs but the
    descriptor of the geometric object from the geoms array on which the constraint is specified.\~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] geoms  - \ru Массив дескрипторов геометрических объектов, образующих жёсткое множество.
                      \en \~
  \return \ru Дескриптор зарегистрированного жёсткого множества объектов.
          \en Descriptor of registered bounded curve. \~
*/
// ---
GCE_FUNC(geom_item) GCE_AddRigidSet( GCE_system gSys, const std::vector<geom_item> & geoms );

//----------------------------------------------------------------------------------------
/** \brief \ru Добавить в систему ограничений переменную.
           \en Add a variable to the constraint system. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] val  - \ru Начальное значение переменной.
                    \en A start value of the variable. \~
  \return \ru Дескриптор зарегистрированной переменной.
          \en Descriptor of registered variable. \~
*/
//---
GCE_FUNC(var_item) GCE_AddVariable( GCE_system gSys, double val );

//----------------------------------------------------------------------------------------
/** \brief \ru Получить переменную из системы ограничений.
           \en Get a variable from the constraint system. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g - \ru Идентификатор геометрического объекта.
                 \en Descriptor of geometric object. \~
  \param[in] coordType - \ru Тип переменной геометрического объекта.
                         \en Type of variable of geometric object. \~
  \return \ru Дескриптор зарегистрированной переменной геометрического объекта.
          \en Descriptor of registered variable of geometric object. \~
*/
//---
GCE_FUNC(var_item) GCE_VariableOf( GCE_system gSys, geom_item g, coord_type coordType );

//----------------------------------------------------------------------------------------
/** \brief \ru Ограничить значение переменной интервалом.
           \en Limit the value of a variable to an interval. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] var - \ru Идентификатор переменной.
                   \en Descriptor of variable. \~
  \param[in] GCE_interval - \ru Границы изменения переменной.
                            \en Boundaries for changing a variable. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddBound( GCE_system gSys, var_item var, GCE_interval dim );

//----------------------------------------------------------------------------------------
/** \brief \ru Тип геометрического объекта.
           \en A type of geometric object. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~  
  \param[in] g    - \ru Дескриптор геометрического объекта.
                    \en Descriptor of geometric object. \~
  \return \ru Тип геометрического объекта.
          \en A type of geometric object. \~
*/
//---
GCE_FUNC(geom_type) GCE_GeomType( GCE_system gSys, geom_item g );

//----------------------------------------------------------------------------------------
/** \brief \ru Тип геометрической кривой.
           \en A type of geometric curve. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~  
  \param[in] g    - \ru Дескриптор кривой.
                    \en Descriptor of curve \~
  \return \ru Тип геометрического объекта.
          \en A type of geometric object. \~
  \details \ru The function returns geometric type of a curve 'crv' or type 
               of a base curve if 'crv' has type #GCE_BOUNDED_CURVE.
           \en Функция вернет геометрический тип кривой 'crv' либо тип базовой кривой, 
               если 'crv' имеет тип #GCE_BOUNDED_CURVE. \~
*/
//---
GCE_FUNC(geom_type) GCE_BaseCurveType( GCE_system gSys, geom_item crv );

//----------------------------------------------------------------------------------------
/** \brief \ru Удалить переменную из системы ограничений.
           \en Delete variable from the constraint system. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] var  - \ru Дескриптор переменной.
                    \en Descriptor of variable. \~
  \return \ru true, если переменная var действительно удалена.
          \en it equals true if the variable var is actually deleted. \~
*/
//---
GCE_FUNC(bool) GCE_RemoveVariable( GCE_system gSys, var_item var );

//----------------------------------------------------------------------------------------
/** \brief \ru Передать управление временем жизни переменной решателю.
           \en Control variable's lifetime by solver. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] var  - \ru Дескриптор переменной.
                    \en Descriptor of variable. \~
*/
//---
GCE_FUNC(void) GCE_ReleaseVariable( GCE_system gSys, var_item var );

//----------------------------------------------------------------------------------------
/** \brief \ru  Удалить геометрический объект из системы ограничений.
           \en  Delete geometric object from the constraint system. \~
  \details \ru  Функция удаляет геометрический объект, если он не связан ограничениями,
                наличие которых можно проверить вызовом IsConstrainedGeom. Функцию можно
                применить к геометрическим узлам, подчиненным другим геометрическим объектам,
                таким как GCE_BOUNDED_CURVE, GCM_OFFSET_CURVE, GCM_LINE_SEGMENT.
                В этом случае вызов исполняется успешно, однако действительное удаление
                может произойти позже в автоматическом порядке после удаления
                всех геометрических объектов, которые его использует.
           \en  The function removes a geometric object if it isn't connected to constraints,
                which can be checked by calling IsConstrainedGeom. The function can be applied to
                geometric nodes subordinated to other geometric objects, such as
                GCE_BOUNDED_CURVE, GCM_OFFSET_CURVE, GCM_LINE_SEGMENT. In this case,
                the call succeeds but the actual deletion can happen later in an automatic
                order after all the owning geometry has released it.
   \note   \ru  Этот вызов применяется только для геометрии добавленной пользователем с помощью
                вызовов GCE_Add***(...).
           \en  The call only applies to geometry added by the user with calls GCE_Add***(...).

  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g    - \ru Дескриптор геометрического объекта.
                    \en Descriptor of geometric object \~
  \return \ru true, если вызов исполнен успешно.
          \en it equals true if the function was performed successfully. \~
*/
//---
GCE_FUNC(bool) GCE_RemoveGeom( GCE_system gSys, geom_item g );

//----------------------------------------------------------------------------------------
/** \brief \ru Передать управление временем жизни геометрического объекта решателю.
           \en Control geometry object's lifetime by solver. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g    - \ru Дескриптор геометрического объекта.
                    \en Descriptor of geometric object. \~
*/
//---
GCE_FUNC(void) GCE_ReleaseGeom( GCE_system gSys, geom_item g );

//----------------------------------------------------------------------------------------
/** \brief \ru Удалить ограничение из системы.
           \en Delete a constraint from the system. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] con  - \ru Дескриптор ограничения.
                    \en Descriptor of constraint. \~
  \return \ru true, если ограничение con действительно удалено.
          \en it equals true if the constraint con is actually deleted. \~
*/
//---
GCE_FUNC(bool) GCE_RemoveConstraint( GCE_system gSys, constraint_item con );

//----------------------------------------------------------------------------------------
/** \brief \ru Запросить дескриптор контрольной точки объекта.
           \en Request of the object control point descriptor. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g    - \ru Дескриптор объекта.
                    \en Descriptor of object. \~
  \param[in] pnt  - \ru Имя контрольной точки объекта.
                    \en Name of the object control point. \~
  \return \ru Дескриптор контрольной точки объекта.
          \en Descriptor of the object control point. \~

  \details \ru Дескриптор, полученный по значению этой функции, имеет автоматическое 
  время жизни, т.е. нет необходимости вызывать для него метод #GCE_RemoveGeom.
           \en Descriptor obtained by the value of this function, it has automatical 
  lifetime, i.e. there is no reason to call the method #GCE_RemoveGeom for it. \~
*/
//---
GCE_FUNC(geom_item) GCE_PointOf( GCE_system gSys, geom_item g, point_type pnt );

//----------------------------------------------------------------------------------------
/** \brief \ru Получить текущие координаты вектора.
           \en Get the current coordinates of vector. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] g      - \ru Дескриптор вектора или иного геометрического объекта.
                      \en Descriptor of vector or other geometric object. \~
  \param[in] vType  - \ru Идентификатор вектора, принадлежащего объекту (в настоящий момент равен или GCE_DIRECTION, или GCE_ORIENTATION).
                          GCE_DIRECTION возвращает направляющую прямой, отрезка или главной полуоси эллипса.
                      \en Identifier of vector belonging to the object (currently it equals GCE_DIRECTION or GCE_ORIENTATION).
                          In case of GCE_DIRECTION function returns direction vector for line, line segment or ellipse major axis.\~
  \return \ru Координаты вектора.
          \en Vector coordinates. \~
*/
//---
GCE_FUNC(GCE_vec2d) GCE_GetVectorValue( GCE_system gSys, geom_item g, subgeom_type vType );

//----------------------------------------------------------------------------------------
/** \brief \ru Получить текущие координаты точки.
           \en Get the current coordinates of point. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] g      - \ru Дескриптор точки или иного геометрического объекта.
                      \en Descriptor of point or other geometric object. \~
  \param[in] pName  - \ru Идентификатор точки, принадлежащей объекту.
                      \en Identifier of a point belonging to the object. \~
  \return \ru Координаты точки.
          \en Point coordinates \~
*/
//---
GCE_FUNC(GCE_point) GCE_GetPointXY( GCE_system gSys, geom_item g, point_type pName = GCE_PROPER_POINT );

//----------------------------------------------------------------------------------------
/** \brief \ru Получить текущее значение координаты геометрического объекта.
           \en Get the current value of geometric object's coordinate. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] g      - \ru Дескриптор объекта.
                      \en Descriptor of object. \~
  \param[in] cName  - \ru Обозначение параметра объекта.
                      \en Denotation of object parameter. \~
  \return \ru Координаты точки.
          \en Point coordinates \~
  \details \ru Получить текущее значение координаты геометрического объекта. Например,
    с помощью данной функции можно узнать текущее значение большой или малой 
    полуоси эллипса, радиус окружности и т.д.
           \en Get the current value of geometric object's coordinate. For example,
    by using this function one can find the current value of the major or the minor 
    semi-axis of ellipse, circle radius etc. \~
*/
//---
GCE_FUNC(double) GCE_GetCoordValue( GCE_system gSys, geom_item g, coord_name cName );

//----------------------------------------------------------------------------------------
/** \brief \ru Получить текущее значение переменной.
           \en Get the current value of variable. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] var  - \ru Дескриптор переменной.
                    \en Descriptor of variable. \~
  \return \ru Значение переменной.
          \en A value of variable. \~
*/
//---
GCE_FUNC(double) GCE_GetVarValue( GCE_system gSys, var_item var );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать текущие координаты точки.
           \en Set the current coordinates of point. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] g      - \ru Дескриптор точки или иного геометрического объекта.
                      \en Descriptor of point or other geometric object. \~
  \param[in] pName  - \ru Идентификатор точки, принадлежащей объекту.
                      \en Identifier of a point belonging to the object. \~
  \param[in] xyVal  - \ru Новое значение координат точки.
                      \en New value of point coordinates. \~
  \return \ru true, если операция выполнена успешно.
          \en true if operation succeeded. \~

  \details \ru Метод присваивает точке или контрольной точке объекта g c 
  атрибутом pName новое состояние координат (параметр xyVal). Следует учитывать, 
  что вызов #GCE_SetPointXY не решает системы ограничений, а только меняет состояние 
  геометрического объекта. При этом система ограничений может стать неудовлетворенной. 
  Состояние точки, присвоенное вызовом GCE_SetPointXY не обязано сохранятся после 
  вызова #GCE_Evaluate, если точка не фиксированная или не замороженная.
           \en The method assigns to the point or the control point of the object g with 
  the attribute pName a new state of coordinates (the parameter xyVal). It should be taken into account 
  that the call of #GCE_SetPointXY doesn't solve the constraint system but only changes the state 
  of geometric object. At the same time the constraint system may become unsatisfied. 
  The state of a point assigned by the call of GCE_SetPointXY should not be saved after 
  the call of #GCE_Evaluate if the point is not fixed or not frozen. \~
*/
//---
GCE_FUNC(bool) GCE_SetPointXY( GCE_system gSys, geom_item g, point_type pName, GCE_point xyVal );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать текущее значение координаты геометрического объекта.
           \en Set the current value of geometric object's coordinate. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] g      - \ru Дескриптор объекта.
                      \en Descriptor of object. \~
  \param[in] cName  - \ru Обозначение параметра объекта.
                      \en Denotation of object parameter. \~
  \param[in] crdVal - \ru Новое значение координаты.
                      \en New value of coordinate. \~
  \return \ru true, если операция выполнена успешно.
          \en true if operation succeeded. \~

  \details \ru Метод присваивает координате объекта g c атрибутом cName новое значение.
  Следует учитывать, что вызов #GCE_SetCoordValue не решает системы ограничений, 
  а только меняет состояние геометрического объекта. При этом система ограничений 
  может стать неудовлетворенной. Состояние координаты, присвоенное этим методом 
  не обязано сохранятся после вызова #GCE_Evaluate, если точка не фиксированная 
  или не замороженная.
           \en The method assigns a new value to the coordinate of the object g with the attribute cName.
  It should be taken into account that the call of #GCE_SetCoordValue doesn't solve the constraint system 
  but only changes the state of geometric object. At the same time the constraint system 
  may become unsatisfied. The state of coordinate assigned by this method 
  should not be saved after the call of #GCE_Evaluate if the point is not fixed 
  or not frozen. \~
*/
//---
GCE_FUNC(bool) GCE_SetCoordValue( GCE_system gSys, geom_item g, coord_name cName, double crdVal );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать текущее значение переменной.
           \en Set the current value of variable. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] var  - \ru Дескриптор переменной.
                    \en Descriptor of variable. \~
  \param[in] val  - \ru Новое значение переменной.
                    \en New value of variable. \~
  \return \ru true, если операция выполнена успешно.
          \en true if operation succeeded. \~
*/
//---
GCE_FUNC(bool) GCE_SetVarValue( GCE_system gSys, var_item var, double val );

//----------------------------------------------------------------------------------------
/** \brief \ru Изменить вес контрольной точки сплайна.
           \en Change the weight of the spline control point. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] spline - \ru Сплайн или его инстанс.
                      \en Spline or its instances. \~
  \param[in] pntIdx - \ru Индекс контрольной точки сплайна.
                      \en Index of control point of the spline. \~
  \param[in] weight - \ru Вес контрольной точки сплайна.
                      \en Weight of spline control point. \~

  \details \ru Метод изменяет вес контрольной точки сплайна и всех его экземпляров, связанных друг с
               другом одним или более паттернами (GCE_PATTERN) образованными с помощью методов
               #GCE_AddPatterned, #GCE_AddInstance.
           \en The method changes the weight of the spline control point and all its instances
               connected to each other by one or more patterns (GCE_PATTERN) formed using the
               methods #GCE_AddPatterned, #GCE_AddInstance. \~

  \note    \ru Функция действует на сплайны (GCE_SPLINE), а также на кривые типов GCE_BOUNDED_CURVE,
               GCE_OFFSET_CURVE, GCE_INSTANCE, основанных на сплайне. Если сплайн является элементом
               паттерна, то вместе с ним поменяется вес остальных сплайнов, связанных друг с другом
               с помощью методов #GCE_AddPatterned, #GCE_AddInstance.
           \en The function is applied to splines (GCE_SPLINE), as well as to curves of types
               GCE_BOUNDED_CURVE, GCE_OFFSET_CURVE, GCE_INSTANCE, based on a spline.
               If a spline is an element of a pattern, the weight of other splines connected with each other
               using methods #GCE_AddPatterned, #GCE_AddInstance will change together with it. \~
*/
//---
GCE_FUNC(void) GCE_SetWeight( GCE_system gSys, geom_item spline, size_t pntIdx, double weight );

//----------------------------------------------------------------------------------------
/** \brief \ru Выдать вес контрольной точки сплайна.
           \en Get the weight of the spline control point. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] spline - \ru Сплайн или его инстанс.
                      \en Spline or its instances. \~ 
  \param[in] pntIdx - \ru Индекс контрольной точки сплайна.
                      \en Index of control point of the spline. \~

  \details \ru Метод возвращает вес контрольной точки сплайна.
           \en The method returns the weight of the control point of the spline. \~
*/
//--- 
GCE_FUNC(double) GCE_WeightOf( GCE_system gSys, geom_item spline, size_t pntIdx ); 
 
//----------------------------------------------------------------------------------------
/** \brief \ru Дескриптор контрольной точки сплайна по индексу.
           \en Descriptor of spline control point by index \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] spl    - \ru Дескриптор сплайна.
                      \en Descriptor of spline. \~
  \param[in] pntIdx - \ru Индекс контрольной точки.
                      \en A control point index. \~
  \return \ru Дескриптор контрольной точки сплайна.
          \en Descriptor of spline control point. \~

  \details \ru Дескриптор, полученный по значению этой функции, имеет автоматическое
  время жизни, т.е. нет необходимости вызывать для него метод #GCE_RemoveGeom.
           \en Descriptor obtained by the value of this function, it has automatical
  lifetime, i.e. there is no reason to call the method #GCE_RemoveGeom for it. \~
*/
//---
GCE_FUNC(geom_item) GCE_SplinePoint( GCE_system gSys, geom_item spl, size_t pntIdx );

//----------------------------------------------------------------------------------------
/** \brief \ru Добавить контрольную точку к сплайну (NURBS)
           \en Add control point to spline (NURBS) \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] spl - \ru Дескриптор сплайна, к которому добавляется контрольная точка.
                   \en Descriptor of spline to which control point is added. \~
  \param[in] pntIdx - \ru Индекс контрольной точки.
                      \en A control point index. \~
  \param[in] pnt - \ru Добавляемая контрольная точка.
                   \en The control point to add. \~
  \return \ru Дескриптор добавленной контрольной точки.
          \en Descriptor of the added control point. \~
  \details \ru Если сплайн является интерполяционным или spl не является сплайном, то возвращается GCE_NULL_G.
           \en If the spline is interpolation or spl is not a spline, then GCE_NULL_G is returned. \~
*/
//---
GCE_FUNC(geom_item) GCE_AddSplinePoint( GCE_system gSys, geom_item spl, size_t pntIdx, GCE_point pnt );

//----------------------------------------------------------------------------------------
/** \brief \ru Удалить контрольную точку сплайна (NURBS)
           \en Remove spline (NURBS) control point \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] spl - \ru Дескриптор сплайна, из которого удаляется контрольная точка.
                   \en Descriptor of spline from which control point is to be removed. \~
  \param[in] pntIdx - \ru Индекс удаляемой контрольной точки.
                      \en The index of the point to remove. \~
  \return \ru Удалена ли контрольная точка.
          \en Is control point removed. \~
  \details \ru Если сплайн является интерполяционным или spl не является сплайном, то возвращается false.
           \en If the spline is interpolation or spl is not a spline, then false is returned. \~
*/
//---
GCE_FUNC(bool) GCE_RemoveSplinePoint( GCE_system gSys, geom_item spl, size_t pntIdx );

//----------------------------------------------------------------------------------------
/** \brief \ru Заморозить геометрический объект.
           \en Freeze geometric object. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g    - \ru Геометрический объект.
                    \en Geometric object. \~
  \return \ru true, если операция выполнена успешно.
          \en true if operation succeeded. \~

  \details

    \ru Функция лишает объект всей степени свободы. Отдельно можно заметить, что 
  функция GCE_IsConstrainedGeom для замороженного объекта вернет false, если 
  объект не был связан другими ограничениями. Т.е. заморозка не считается 
  ограничением.\n  
    Решатель не может менять замороженную геометрию, но её может поменять 
  клиентское приложение методами GCE_SetCoordValue или GCE_SetPointXY.
  Замороженные объекты следует рассматривать в качестве независимых входных
  параметров системы ограничений.
    \en The function deprives the object of all degrees of freedom. Note that 
  the function GCE_IsConstrainedGeom returns false for the frozen object if 
  the object was not connected with other constraints. I.e. the freezing is not considered 
  as a constraint.\n  
    The solver cannot change the frozen geometry but the user application can change it 
  with the methods GCE_SetCoordValue or GCE_SetPointXY. Frozen objects should be 
  considered as independent input parameters of constraint system. \~

  \note 
  \ru Обычно на стороне САПР эта команда применяется для фиксации проекционной геометрии 
      в ассоциативных чертежах или в эскизах с проекциями трехмерных объектов.
  \en Usually, in CAD applications this command is used only for fixation of projection 
      geometry in associative drawings or sketches with projections of 3D-objects. \~
*/
//---
GCE_FUNC(bool) GCE_FreezeGeom( GCE_system gSys, geom_item g );

//----------------------------------------------------------------------------------------
/**
  \brief \ru Задать ограничение "Фиксированное смещение".
         \en Set the constraint "Fixation of offset" \~
  \details \ru Зафиксировать смещение эквидистантной кривой (#GCE_OFFSET_CURVE).
           \en Fix offset of equidistant curve (#GCE_OFFSET_CURVE). \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] curve - \ru Дескриптор эквидистантной кривой.
                     \en Descriptor of equidistant curve. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
*/
// ---
GCE_FUNC(geom_item) GCE_FixOffset( GCE_system gSys, geom_item curve );

//----------------------------------------------------------------------------------------
/** \brief \ru Функция отвечает на вопрос: Связан ли геометрический объект ограничениями?
           \en The function answers the question: Is geometric object connected with constraints? \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g    - \ru Дескриптор геометрического объекта.
                    \en Descriptor of geometric object. \~
  \return \ru true, если для объекта g задано хотя бы одно ограничение.
          \en true if at least one constraint is set for the object g. \~
  \sa GCE_RemoveGeom, GCE_RemoveConstraint
*/
//---
GCE_FUNC(bool) GCE_IsConstrainedGeom( GCE_system gSys, geom_item g );

//----------------------------------------------------------------------------------------
/** \brief \ru Определить связано ли ограничение с геометрическим семейством объекта.
           \en Determine whether the constraint is related to the geometric family of the object. \~
  \details \ru Функция обходит семейство геометрических объектов, в которое входит геометрический объект `g`, 
            его контрольные точки, базовые объекты, а также всю геометрию построенную на базе объекта `g`.
            Возвращаемое значение равно `true`, если ограничение `cItem` связано с любым из объектов указанного семейства.
           \en The function traverses the family of geometric objects, which includes the geometric object `g`,
            its control points, basic objects, as well as the entire geometry based on the object `g`.
            The return value is `true` if the `cItem' constraint is associated with any of the objects in the specified family. \~
  \note \ru Эта функция может быть использована для того, чтобы освободить геометрический объект, 
            его контрольные точки, скалярные переменные, базовые кривые, а также все зависимые от него
            геометрические объекты от всех смежных ограничений данного семейства.
        \en This function can be used to release a geometric object, its control points, scalar variables, basic curves,
            as well as all geometric objects dependent on it from all adjacent constraints of this family. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] g    - \ru Дескриптор геометрического объекта.
                    \en Descriptor of geometric object. \~
  \param[in] cItem  - \ru Дескриптор ограничения.
                      \en Descriptor of constraint. \~
  \return \ru true, если ограничение связано с семейством геометрического объекта.
          \en true if the constraint is related to the geometric family of the object. \~
*/
//---
GCE_FUNC(bool) GCE_IsAdjacentConstraint( GCE_system gSys, geom_item g, constraint_item cItem );

//----------------------------------------------------------------------------------------
/** \brief \ru Функция отвечает на вопрос: Имеется ли хотя бы один экземпляр паттерна?
           \en The function answers the question: Is there an instance of the pattern? \~
  \param[in] gSys    - \ru Система ограничений.
                       \en System of constraints. \~
  \param[in] pattern - \ru Дескриптор паттерна.
                       \en Descriptor of pattern. \~
  \return \ru true, если для паттерна p существуют экземпляры какого-либо объекта.
          \en true if there are instances of any object for pattern p. \~
  \sa GCE_RemovePattern, GCE_ReleasePattern
*/
//---
GCE_FUNC(bool) GCE_HasInstance( GCE_system gSys, pattern_item p );

//----------------------------------------------------------------------------------------
/** \brief \ru Выполнить проверку удовлетворенности ограничения.
           \en Perform a check that a constraint is satisfied. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] cItem  - \ru Дескриптор ограничения.
                      \en Descriptor of constraint. \~
  \return \ru true, если ограничение удовлетворено.
          \en true if a constraint is satisfied. \~
*/
//---
GCE_FUNC(bool) GCE_IsSatisfied( GCE_system gSys, constraint_item cItem );

//----------------------------------------------------------------------------------------
/**
  \brief \ru Статус ограничения в системе. 
         \en Status of constraint inn the system.
  \details  
    \ru Вызов показывает результат диагностики, которая выделяет в системе ограничений
  хорошо-обусловленные части и части, содержащие переопределения и противоречия. В результате 
  диагностики или попытки решения каждое ограничение помечается одним из статусов,
  перечисленных в наборе GCE_c_status.  
    \en The call shows the result of the diagnostic, which highlights the constraint system
  well-conditioned parts and parts containing redundancies and inconsistencies. As a result
  diagnosing or evaluating each constraint is marked with one of the statuses enumerated by 
  GCE_c_status enum.
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] cItem  - \ru Дескриптор ограничения.
                      \en Descriptor of constraint. \~
  \return \ru Статус ограничения в результате диагностики на противоречия или переопределения.
          \en The status of the constraint as a result of diagnostics on inconsistence and overdefining. \~

*/
// ---
GCE_FUNC(GCE_c_status) GCE_ConstraintStatus( GCE_system gSys, constraint_item cItem );

//----------------------------------------------------------------------------------------
/** \brief \ru Выдать состояние определенности системы ограничений.
           \en Get constraint system definition state.

    \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
    \details 
          \ru Функция вернет состояние #GCE_STATE_Underconstrained, если имеется хотя бы 
  один геометрический объект с ненулевой степенью свободы. 
  Состояние #GCE_STATE_WellConstrained означает, что геометрия полностью определена, а другое 
  состояние #GCE_STATE_UnresolvedRedundancy означает, что в модели имеются нерешенные
  избыточные ограничения.
          \en The function will return #GCE_STATE_Underconstrained if there is at least
  one geometric object with nonzero degree of freedom. State code #GCE_STATE_WellConstrained
  means that the geometry is fully-defined. And the other state is #GCE_STATE_UnresolvedRedundancy
  means there are unresolved redundant constraints (non solved overdefinition). \n

  \return \ru Одно из состояний, перечисленного набором: "Система недоопределена", 
              "Полностью определена" или "Есть не решенные избыточные ограничения".
          \en One of the states enumerated by a set: "Under-defined", "Well-defined" and 
              "There are unsolved redundant constraints (non solved overdefinition)". \~
  \note \ru Сложность выполнения функции сопоставима с GCE_Evaluate.
        \en Function complexity is comparable to GCE_Evaluate. 
*/
//---
GCE_FUNC(GCE_s_state) GCE_DefinitionState( GCE_system gSys );

//----------------------------------------------------------------------------------------
/**
  brief \ru Выполнить диагностику геометрических объектов.
        \en Diagnose geometry. \~
  \details
  \ru Если в ходе решения системы ограничений вырождаются какие-то геометрические объекты 
      (функция #GCE_Evaluate возвращает GCE_RESULT_InvalidGeometry ), то данная функция возвращает 
      массив индексов, под которыми эти объекты зарегистрированы в решателе.
      Если вырождающихся объектов нет, то функция вернет пустой массив.
  \en If some geometrical objects are degenerate in the course of solving the system of constraints
      (function #GCE_Evaluate returns GCE_RESULT_InvalidGeometry ), this function returns an array of indices 
      by which these geometric objects are registered in the solver.
      If geometrical objects do not degenerate, then the function returns an empty array. \~
  \param[in] gcSys  - \ru Система ограничений.
                      \en System of constraints. \~
  \return \ru Вектор индексов объектов с вырожденной геометрией.
          \en Vector of indices of objects with invalid geometry. \~
*/// ---
GCE_FUNC(std::vector<geom_item>) GCE_DiagnoseGeometry( GCE_system gcSys );

//----------------------------------------------------------------------------------------
/**
  \brief \ru Вычислить степень свободы точки.
         \en Calculate point's degree of freedom. \~
  \param[in]  gSys  - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in]  g     - \ru Дескриптор геометрического объекта.
                      \en Descriptor of geometric object \~
  \param[in]  cp    - \ru Код контрольной точки объекта g.
                      \en The code of control point of the object g. \~
  \param[out] dofDir- \ru Угловое направление свободы перемещения точки в радианах.
                      \en Angular direction of point moving freedom in radians. \~
  \return \ru Функция возвращает степень свободы точки; Если возвращается значение < 0, 
                      то вычислить степень свободы не удалось.
          \en The function returns degree of freedom of the point; If a negative value is returned, 
                      then it is failed to calculate the degree of freedom. \~
  \details \ru Данная функция возвращает степень свободы точки и может принимать 
  одно из следующих значений:\n
  (-1)  - Означает, что функция не определила степень свободы;\n
   0    - Означает, что точка неподвижна в системе ограничений;\n
   1    - Означает, что точка имеет свободу перемещения вдоль некоторой траектории, причем 
          через параметр dofDir возвращается направление тангенциального вектора перемещения 
          точки;\n
   2    - Означает, что точка имеет свободу перемещения в некоторой 2D-области.\n
  Если направление перемещения определить не удалось, то dofDir принимает значение < 0.
           \en This function returns the point's degree of freedom and may take 
  one of the following values:\n
  (-1)  - It means that the function didn't determine the degree of freedom;\n
   0    - It means that the point is fixed in constraint system.
   1    - It means that the point has a freedom of movement along some trajectory, besides 
          the direction of point movement tangent vector is returned via the parameter dotDir;\n 
   2    - It means that the point has a movement freedom inside some two-dimensional region.\n
  If the direction of movement was not determined, then dotDir takes a negative value. \~
*/
//--
GCE_FUNC(ptrdiff_t) GCE_GetPointDOF( GCE_system gSys, geom_item g, point_type cp, double & dofDir );

//----------------------------------------------------------------------------------------
/**
  \brief \ru Вычислить степень свободы точки.
         \en Calculate point's degree of freedom. \~
  \param[in]  gSys  - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in]  pnt   - \ru Дескриптор точки.
                      \en Descriptor of point \~
  \return \ru Функция возвращает структуру #GCE_point_dof, которая описывает 
              степень свободы точки, её целочисленное значение и вектор перемещения.
              Если возвращается значение dof < 0, то вычислить степень свободы не удалось.
          \en The function returns a structure #GCE_point_dof, which describes degree 
              of freedom of the point, namely its integral value (dof) and direction 
              vector of point moving freedom (dir). If a negative value (dof) is returned, 
              then it is failed to calculate  the degree of freedom. \~

  \details \ru Данная функция возвращает степень свободы точки и может принимать 
  одно из следующих значений:\n
  dof = (-1)  - Означает, что функция не определила степень свободы;\n
  dof =  0    - Означает, что точка неподвижна в системе ограничений;\n
  dof =  1    - Означает, что точка имеет свободу перемещения вдоль некоторой траектории, 
                причем через параметр "dir" (в структуре #GCE_point_dof ) возвращается направление 
                тангенциального вектора перемещения точки;\n
  dof =  2    - Означает, что точка имеет свободу перемещения в некоторой 2D-области.\n
  Если направление перемещения определить не удалось, то "dof" принимает значение < 0.
           \en This function returns the point's degree of freedom and may take 
  one of the following values:\n
  dof = (-1)  - It means that the function didn't determine the degree of freedom;\n
  dof = 0     - It means that the point is fixed in constraint system.
  dof = 1     - It means that the point has a freedom of movement along some trajectory, besides 
                the direction of point movement tangent vector is returned via the parameter "dir" 
                of data structure  #GCE_point_dof;\n 
  dof = 2     - It means that the point has a movement freedom inside some two-dimensional region.\n
  If the direction of movement was not determined, then "dof" takes a negative value. \~
*/
//--
GCE_FUNC(GCE_point_dof) GCE_PointDOF( GCE_system gSys, geom_item pnt );

//----------------------------------------------------------------------------------------
/** \brief \ru Выдать степень свободы геометрической координаты.
           \en Get the degree of freedom of geometric coordinate.

  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g - \ru Дескриптор геометрического объекта.
                 \en Descriptor of a geometric object. \~
  \param[in] cName  - \ru Обозначение геометрической координаты.
                      \en Denotation of geometric coordinate. \~
  \return \ru Степень свободы координаты: 1-для недоопределенной координаты, 0-для полно-заданной координаты.
          \en Degree of freedom: 1 for underdefined coordinate, 0 for well-defined coordinate.\~

  \details  
    \ru Функция возвращает степень свободы координаты, а именно одно из возможных 
        значений: 1, 0 и -1. Если возвращается значение < 0, то вычислить степень 
        свободы не удалось.
    \en The function returns degree of freedom of the coordinate, namely one of the 
        possible values: 1, 0 and -1. If a negative  value is returned, then it is 
        failed to calculate the degree of freedom. \~

*/
//---
GCE_FUNC(GCE_dof_type) GCE_CoordDOF( GCE_system gSys, geom_item g, coord_name cName );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение для одного объекта (унарное ограничение).
           \en Set a constraint on single object (unary constraint). \~

  \param[in] gSys -   \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] cType -  \ru Значение одного из следующих типов ограничений: GCE_FIX_GEOM; GCE_VERTICAL; GCE_HORIZONTAL; GCE_ANGLE_OX; GCE_LENGTH.
                      \en The value of one of the following types of constraints: GCE_FIX_GEOM; GCE_VERTICAL; GCE_HORIZONTAL; GCE_ANGLE_OX; GCE_LENGTH. \~
  \param[in] g     -  \ru Дескриптор геометрического объекта.
                      \en Descriptor of geometric object \~  
  \return             \ru Дескриптор нового ограничения.
                      \en Descriptor of a new constraint. \~
  \details
  \ru Функция задает унарное ограничение, а именно ограничение, относящееся к одному из типов, 
      действительных для одного геометрического объекта.
  \en The function specifies an unary constraint, namely constraint which has one of types
      that are valid for single geometric object. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddUnaryConstraint( GCE_system gSys, constraint_type cType, geom_item geom );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Совпадение".
           \en Set the constraint "Coincidence". \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g    - \ru Дескрипторы пары геометрических объектов.
                    \en Descriptors of geom objects pair. \~
  \details
  \ru Если ограничение совпадение задано для точки и кривой, то предполагается, что точка 
      лежит на кривой. Если совпадение задано для геометрических объектов одного и того 
      же типа, то совпадение подразумевает, что они равны.    
  \en If a coincident constraint is defined between a point and a curve then this implies 
      that the point lies on the curve. A coincident constraint defined between two 
      geometries of the same type implies that they are equal.
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~

  \attention 
  \ru В текущей версии решателя применение этого ограничения возможно 
      только для двух точек либо для точки и кривой. На будущее планируется расширить 
      его применения для других типов.
  \en In the current version of solver using of this constraint is possible only for 
      two points either for a point and a curve. It is planned to extend its application 
      area for other types. \~
*/
// ---
GCE_FUNC(constraint_item) GCE_AddCoincidence( GCE_system gSys, geom_item g[2] );

//----------------------------------------------------------------------------------------
/** \brief \ru Ограничение "Точка на участке кривой по коэффициенту его параметрической длины".
           \en The constraint "Point on a piece of a curve by the coefficient of its parametric range". \~

  \param[in] gSys    - \ru Система ограничений.
                       \en System of constraints. \~
  \param[in] curve   - \ru Дескриптор кривой.
                       \en Descriptor of a curve. \~
  \param[in] pnt     - \ru Дескрипторы точек: две крайние точки участка и точка между.
                       \en Descriptors of points: two boundary points of a piece and a point between. \~
  \param[in] k       - \ru Долевой коэффициент от параметрической длины участка.
                       \en Coefficient for a part of parametric range of piece. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~

  \details \ru Предполагается, что для кривой curve и точек pnt[0], pnt[1], обеспечивается 
  инцидентность другими ограничениями, зарегистрированными в решателе, или эти 
  точки априори принадлежат кривой. Для точки pnt[2] инцидентность с кривой задавать 
  не требуется, т.к. данное ограничение уже обеспечивает это. Если pnt[0] = pnt[1] = GCE_NULL_G, то участок 
  кривой, для которого исчисляется процент k, совпадает со всей параметрической
  областью кривой. Например, для окружности параметрическая область равна
  интервалу [-PI ... PI]. Область значений k из интервала от 0 до 1 отображается
  на параметрическую область участка кривой, соответственно k = 0 прикрепит точку
  pnt[2] к началу участка, а k = 1.0 к концу участка.
           \en It is assumed that for the curve 'curve' and the points pnt[0], pnt[1] an incidence 
  is provided with the other constraints registered in solver or these 
  points a priori belong to a curve. An incidence between pnt[2] and 'curve' is not required because
  this constraint already provides an incidence. If pnt[0] = pnt[1] = GCE_NULL_G, then the piece 
  of a curve for which the percentage k is calculated coincides with the whole parametric
  region of a curve. For example, in a case of circle the parametric range is equal
  to the interval [-PI ... PI]. The range of values of k from the interval from 0 to 1 is mapped
  to the parametric region of curve's piece, k = 0 attaches the point
  pnt[2] to the beginning of the piece and k = 1.0 - to the end of the piece. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddPointOnPercent( GCE_system gSys, geom_item curve, geom_item pnt[3], double k );
GCE_FUNC(constraint_item) GCE_AddPointOnPercent( GCE_system gSys, geom_item curve, geom_item pnt[3], var_item k );

//----------------------------------------------------------------------------------------
/** \brief \ru Ограничение "Точка на участке кривой по коэффициенту его длины".
           \en The constraint "Point on a piece of a curve by the coefficient of its length". \~

  \param[in] gSys    - \ru Система ограничений.
                       \en System of constraints. \~
  \param[in] curve   - \ru Дескриптор кривой.
                       \en Descriptor of a curve. \~
  \param[in] pnt     - \ru Дескрипторы точек: две крайние и точка между ними.
                       \en Descriptors of points: two boundary points and a point between them. \~
  \param[in] k       - \ru Значение доли от метрической длины между заданными точками.
                       \en Value of a part (proportion) of the arc length between the two points. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~

  \details \ru Метод создает в системе ограничение, задающее положение точки на 
  участке кривой, заданное коэффициентом от его длины. Предполагается, 
  что для кривой curve и точек pnt[0], pnt[1], обеспечивается инцидентность другими, 
  зарегистрированными в решателе, ограничениями или эти точки априори принадлежат 
  кривой. Если pnt[0] = pnt[1] = GCE_NULL_G, то участок кривой, для которого 
  исчисляется процент k, совпадает со всей параметрической областью кривой. 
  Например, для окружности параметрическая область равна интервалу [-PI ... PI].
  Если k = 0, то ограничение прикрепит точку pnt[2] к началу участка, если k = 1.0,
  то ограничение прикрепит точку pnt[2] к концу участка.
  Переменное значение коэффициента k поддерживается для сплайнов.

           \en The method creates a constraint specifying the point location on 
  a piece of a curve which is set by the coefficient (proportional) of its arc length. 
  It is assumed that for the curve 'curve' and the points pnt[0], pnt[1] an incidence 
  is provided with the other constraints registered in the solver or these points belong to 
  the curve a priori. If pnt[0] = pnt[1] = GCE_NULL_G, then the piece of a curve for which 
  the percentage k is calculated coincides with the whole parametric region of a curve. 
  For example, in a case of circle the parametric range is equal to the interval [-PI ... PI].
  If k = 0, then the constraint attaches the point pnt[2] to the beginning of the piece, if k = 1.0,
  then the constraint attaches the point pnt[2] to the end of the piece. 
  The percentage k as a variable is supported for splines.\~
*/
//---
GCE_FUNC(constraint_item) GCE_AddPointByMetricPercent( GCE_system gSys, geom_item curve, geom_item pnt[3], double k );
GCE_FUNC(constraint_item) GCE_AddPointByMetricPercent( GCE_system gSys, geom_item curve, geom_item pnt[3], var_item k );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Фиксация положения точки, лежащей на кривой".
           \en Set the constraint "Fixation of location of the point lying on a curve". \~

  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] curve  - \ru Дескриптор кривой.
                      \en Descriptor of a curve. \~
  \param[in] pnt    - \ru Дескрипторы точки.
                      \en Descriptors of a point. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~

  \details \ru Данная функция создает ограничение, прикрепляющее точку к кривой в текущем 
  месте. Точка локализуется, опираясь на параметрическое представление кривой, с помощью
  параметра вдоль кривой, где она расположена. Требуется, что бы к моменту вызова функции,
  точка лежала на кривой, а для точки и кривой curve должна обеспечиваться инцидентность
  с помощью других ограничений или эта точка априори должна принадлежать кривой.
           \en This function creates a constraint attaching a point to the curve in the current 
  location. The point is localized according to the parametric representation of a curve with a help of
  parameter along a curve where it is located. It is required that at the moment when the function is called
  the point is lying on the curve, and coincidence between the point and the curve should be provided
  by other constraints or this point should belong to the curve a priori. \~

  \attention \ru В Cad-системе КОМПАС данная функция применяется только для фиксации концов 
  участка (bounded curve) параметрической кривой, полученной проецированием из 3D-модели. 
  Для таких ограничений, как "средняя точка" рекомендуется применять более 
  нативную функцию #GCE_AddMiddlePoint.
             \en In CAD system KOMPAS this function is used only for fixation of ends of 
  a piece ('bounded curve') of a parametric curve obtained by projecting from 3D model. 
  It is recommended to apply the more native function 
  #GCE_AddMiddlePoint for such constraints as "middle point". \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddFixCurvePoint( GCE_system gSys, geom_item curve, geom_item pnt );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Точка на параметрическом эллипсе".
           \en Set the constraint "Point on parametric ellipse". \~
  \param[in] gSys     - \ru Система ограничений.
                        \en System of constraints. \~
  \param[in] pnt      - \ru Дескриптор точки.
                        \en Descriptor of a point. \~
  \param[in] ellipse  - \ru Дескриптор эллипса.
                        \en Descriptor of ellipse. \~
  \param[in] \ru t          Значение параметра на эллипсе из области [-PI,PI].
             \en t          The value of parameter on ellipse from the region [-PI,PI]. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
  \details \ru Данная функция действительна только для кривых, относящихся типу 
  "эллипс", функция создает ограничение, обеспечивающее совпадение точки pnt с 
  точкой эллипса, заданной параметром t из параметрической области эллипса, 
  равной интервалу [-PI,PI].
           \en This function is valid only for curves of the type 
  "ellipse", the function creates a constraint which provides coincidence between the point pnt and 
  the ellipse point set by the parameter t from the ellipse parametric region 
  which is equal to the interval [-PI,PI]. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddPointOnParEllipse( GCE_system gSys, geom_item pnt, geom_item ellipse, double t );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Точка на кривой по параметру".
           \en Specify a constraint "Point on curve at a given parameter". \~
 \param[in] gSys - \ru Система ограничений.
                   \en System of constraints. \~
 \param[in] pnt - \ru Дескриптор точки.
                  \en Descriptor of a point. \~
 \param[in] curve - \ru Дескриптор кривой.
                    \en Descriptor of a curve. \~
 \param[in] t - \ru Дескриптор параметра кривой.
                \en Descriptor of a curve parameter. \~
 \return \ru Дескриптор нового ограничения.
         \en Descriptor of a new constraint. \~
 \details \ru Функция отличается от #GCE_AddCoincidence тем, что позволяет связать точку с параметрической
              кривой через значение параметра и управлять её положением на кривой через этот параметр.
              Ограничение доступно для следующих типов кривой: #GCE_ELLIPSE, #GCE_SPLINE,
              #GCE_PARAMETRIC_CURVE и #GCE_BOUNDED_CURVE, основанной на кривой одного из перечисленных
              типов.
          \en This function differs from #GCE_AddCoincidence in that it allows to link a point with a
              parametric curve through a parameter value and control its position on the curve through this
              parameter. The constraint is available for the following curve types: #GCE_ELLIPSE,
              #GCE_SPLINE, #GCE_PARAMETRIC_CURVE and #GCE_BOUNDED_CURVE, based on the curve of one of the
              listed types.
*/
// ---
GCE_FUNC(constraint_item) GCE_AddParPointOnCurve( GCE_system gSys, geom_item pnt, geom_item curve, var_item t );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Выравнивание точек вдоль заданного направления".
           \en Set the constraint "Alignment of points along the given direction". \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] p    - \ru Дескрипторы пары точек.
                    \en Descriptors of point pair. \~
  \param[in] ang  - \ru Угол, задающий направление выравнивания, радианы.
                    \en An angle specifying alignment direction, in radians. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddAlignPoints( GCE_system gSys, geom_item p[2], double ang );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Выравнивание точек вдоль заданного направления".
           \en Set the constraint "Alignment of points along the given direction". \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] p    - \ru Дескрипторы пары точек.
                    \en Descriptors of point pair. \~
  \param[in] dir  - \ru Направляющий вектор.
                    \en Direction vector. \~
  \return  \ru Дескриптор нового ограничения.
           \en Descriptor of a new constraint. \~
  \details \ru В качестве направления можно задавать любой геометрический объект, для которого корректен вызов
           #GCE_GetVectorValue c идентификатором вектора #GCE_DIRECTION.
           \en Any geometric object for which the #GCE_GetVectorValue function call with the #GCE_DIRECTION vector identifier is valid can be specified as the direction. \~
  \note \ru Это ограничение можно использовать вместе с #GCE_AddLinearDimension для за-
        дания линейного размера вместо метода #GCE_AddDistance. Данный способ пред-
        почтительнее, поскольку дает возможность более точно настроить привязки вспомогательных точек.
        \en This constraint can be used in conjunction with #GCE_AddLinearDimension to set a linear dimension as an alternative to the #GCE_AddDistance method. This method is preferable as it enables the help points of the dimension to be configured more definitely. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddAlignPoints( GCE_system gSys, geom_item p[2], geom_item dir );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Угловой размер между двумя прямыми".
           \en Set the constraint "Angular dimension between two lines". \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] l1     - \ru Дескриптор первого линейного геометрического объекта.
                      \en Descriptor of the first linear geometric object. \~
  \param[in] l2     - \ru Дескриптор второго линейного геометрического объекта.
                      \en Descriptor of the second linear geometric object. \~
  \param[in] dPars  - \ru Параметры углового размера (подробности см.#GCE_adim_pars).
                      \en Parameters of angular dimension (see #GCE_adim_pars). \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
  \details \ru Угловой размер для пары линейных геометрических объектов. Аргументами
  ограничения могут быть объекты, принадлежащие типам: "прямая", "отрезок" или
  "Bounded curve", основанной на прямой.
           \en Angular dimension for a pair of linear geometric objects. Arguments
  of constraint are objects of the following types: "line", "segment" or
  "Bounded curve" based on line. \~

*/
//---
GCE_FUNC(constraint_item) GCE_AddAngle( GCE_system gSys, geom_item l1, geom_item l2
                                      , const GCE_adim_pars & dPars );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Биссектриса".
           \en Set the constraint "Bisector of angle". \~
  \param[in] \ru gSys     Система ограничений.
             \en gSys     System of constraints. \~
  \param[in] \ru bl       отрезок, биссектриса между двумя прямыми.
             \en bl       a segment, bisector of angle between two lines. \~
  \param[in] \ru l1, l2   прямые или отрезки, между которыми устанавливается биссектриса.
             \en l1, l2   lines or segments a bisector of angle is set between. \~
  \param[in] \ru variant  вариант решения для биссектрисы.
             \en variant  variant of solution for bisector of angle. \~
  \return \ru дескриптор нового ограничения.
          \en descriptor of new constraint. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddAngleBisector( GCE_system gSys
                                              , geom_item l1, geom_item l2
                                              , geom_item bl
                                              , GCE_bisec_variant variant );

//----------------------------------------------------------------------------------------
/// \ru Задать угловой размер для четырех точек. \en Specify angular dimension for four points.
/**\ru Конструируется угловой размер для двух отрезков с точками p1-p2, p3-p4.
   \en Angular dimension is constructed for two segments with points p1-p2, p3-p4. \~
   \param[in] \ru gSys - Система ограничений.
              \en gSys - System of constraints. \~
   \param[in] \ru fPair - Первая пара точек (первый отрезок).
              \en sPair - First pair of points (first segment).\~
   \\param[in] \ru fPair - Вторая пара точек (второй отрезок).
               \en sPair - Second pair of points (second segment).\~
   \param[in] dPars  - \ru Параметры углового размера (подробности см.#GCE_adim_pars).
                       \en Parameters of angular dimension (see #GCE_adim_pars). \~
   \return \ru Дескриптор нового ограничения.
           \en Descriptor of a new constraint. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddAngle4P( GCE_system gSys, geom_item fPair[2]
                                        , geom_item sPair[2], const GCE_adim_pars & dPars );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Коллинеарность".
           \en Set the constraint "Colinearity". \~  
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g    - \ru Дескрипторы пары линейных объектов.
                    \en Descriptors of a pair of linear objects. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
  \details \ru Ограничение делает пару объектов, принадлежащими общей прямой, применяется 
               для прямых или отрезков.
           \en Constraint makes a pair of objects belonging to the common line, it is used 
               for lines or segments. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddColinear( GCE_system gSys, geom_item g[2] );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Коллинеарность трех точек".
           \en Set the constraint "Colinearity of three points". \~
  \details \ru Задать для трех точек отношение, такое что точки лежат на одной прямой.
           \en Set such a constraint for three points that points should lie on the same line. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] pnt  - \ru Тройка точек, лежащих на одной прямой.
                    \en A triplet of points lying on the same line. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddColinear3Points( GCE_system gcSys, geom_item pnt[3] );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Равенство длин" кривых.
           \en Set the constraint "Equality of lengths" for curves. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] curve1  - \ru Дескриптор первой кривой.
                    \en Descriptor of the first curve. \~
  \param[in] curve2  - \ru Дескриптор второй кривой.
                    \en Descriptor of the second curve. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
  \details \ru Ограничение поддерживается для тех же кривых, для которых поддерживается ограничение 
           #GCE_AddLength.
           \en The constraint is applicable for the same curves for which the #GCE_AddLength constraint is applicable.\~
*/
//---
GCE_FUNC(constraint_item) GCE_AddEqualLength( GCE_system gSys, geom_item curve1, geom_item curve2 );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Равенство радиусов" для двух окружностей (дуг)
           \en Set the constraint "Equality of radii" for two circles (arcs) \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] c1   - \ru Дескриптор первой окружности.
                    \en Descriptor of the first circle. \~
  \param[in] c2   - \ru Дескриптор второй окружности.
                    \en Descriptor of the second circle. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~

*/
//---
GCE_FUNC(constraint_item) GCE_AddEqualRadius( GCE_system gSys, geom_item c1, geom_item c2 );

//----------------------------------------------------------------------------------------
/**
  \brief \ru Задать ограничение "Равенство смещений эквидистантных кривых".
         \en Set the constraint "Equal of offset of equidistant curves" \~
  \details \ru Ограничение применимо для приравнивания смещений эквидистантных кривых.
           \en The constraint is applicable to equal offsets of equidistant curves. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] curve1 - \ru Дескриптор эквидистантной кривой.
                      \en Descriptor of equidistant curve. \~
  \param[in] curve2 - \ru Дескриптор эквидистантной кривой.
                      \en Descriptor of equidistant curve. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
*/
// ---
GCE_FUNC(geom_item) GCE_AddEqualOffset( GCE_system gSys, geom_item curve1, geom_item curve2 );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Равенство кривизны двух кривых в заданных точках".
           \en Specify a constraint "Equality of curvature of two curves at given points". \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] curves - \ru Дескрипторы пары кривых.
                      \en Descriptors of a pair of curves. \~
  \param[in] tPars - \ru Дескрипторы параметров параметрических кривых, в которых должно выполняется
                         равенство кривизны.
                     \en Descriptors of parameters of parametric curves in which the equality of curvature
                         must be satisfied. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~

  \note \ru Если тип кривой #GCE_CIRCLE или #GCE_BOUNDED_CURVE, базовой кривой которой является окружность,
            то соответствующее этой кривой значение tPars[i] может равняться #GCE_NULL_V, т.к. кривизна
            окружности одинакова во всех её точках.
        \en If curve has a type #GCE_CIRCLE or #GCE_BOUNDED_CURVE which is based on circle then the
            corresponding value of tPars[i] may be equal to #GCE_NULL_V because the curvature of circle
            is the same in all its points. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddEqualCurvature( GCE_system gSys, geom_item curves[2], var_item tPars[2] );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Радиусный размер".
           \en Specify a "Radius dimension" constraint. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] cir  - \ru Дескриптор окружности.
                    \en Descriptor of circle. \~
  \param[in] dPar - \ru Параметры линейного размера (подробности см.#GCE_dim_pars).
                    \en Parameters of linear dimension (see #GCE_dim_pars). \~

  \return \ru Дескриптор радиусного ограничения.
          \en Descriptor of radius constraint. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddRadiusDimension( GCE_system gSys, geom_item cir, GCE_dim_pars dPar );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Диаметральный размер".
           \en Specify a "Diameter dimension" constraint. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] cir  - \ru Дескриптор окружности.
                    \en Descriptor of circle. \~
  \param[in] dPar - \ru Параметры линейного размера (подробности см.#GCE_dim_pars).
                    \en Parameters of linear dimension (see #GCE_dim_pars). \~

  \return \ru Дескриптор диаметрального ограничения.
          \en Descriptor of diameter constraint. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddDiameter( GCE_system gSys, geom_item cir, GCE_dim_pars dPar );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Длина кривой".
           \en Specify a "Curve Length" constraint. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] curve - \ru Дескриптор кривой.
                     \en Descriptor of curve. \~
  \param[in] dPar - \ru Параметры линейного размера (подробности см.#GCE_dim_pars).
                    \en Parameters of linear dimension (see #GCE_dim_pars). \~

  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~

  \details \ru Ограничение поддерживается для отрезка, окружности, сплайна, а также 
           ограниченной кривой (#GCE_BOUNDED_CURVE), созданной на основе прямой, окружности и сплайна.
           \en Constraint is supported for line segment, circle, spline, 
           and bounded curve (#GCE_BOUNDED_CURVE) created from a line, circle and spline.\~
*/
//---
GCE_FUNC(constraint_item) GCE_AddLength( GCE_system gSys, geom_item curve, GCE_dim_pars dPar );

//----------------------------------------------------------------------------------------
/** \brief \ru Связать ограничением паттерна два геометрических объекта.
           \en Bind two geometric objects by a pattern constraint. \~
  \param[in] gSys     - \ru Система ограничений.
                        \en System of constraints. \~
  \param[in] pItem    - \ru Дескриптор паттерна.
                        \en Descriptor of pattern. \~
  \param[in] sample   - \ru Дескриптор образца.
                        \en Descriptor of sample. \~
  \param[in] instance - \ru Дескриптор экземпляра.
                        \en Descriptor of instance. \~
  \param[in] k        - \ru Номер экземпляра.
                        \en Copy number. \~
  \return \ru Дескриптор зарегистрированного ограничения.
          \en Descriptor of registered constrained. \~

  \details \ru Метод связывает два объекта ограничением паттерна.
           \en The method binds the two objects by a pattern constraint. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddPatterned( GCE_system gSys, pattern_item pItem, geom_item sample, geom_item instance, int k );

//----------------------------------------------------------------------------------------
/** \brief \ru Связать ограничением паттерна два геометрических объекта.
           \en Bind two geometric objects by a pattern constraint. \~
  \param[in] gSys     - \ru Система ограничений.
                        \en System of constraints. \~
  \param[in] pItem   -  \ru Дескриптор составного паттерна.
                        \en Descriptor of composition pattern. \~
  \param[in] sample   - \ru Дескриптор образца.
                        \en Descriptor of sample. \~
  \param[in] instance - \ru Дескриптор экземпляра.
                        \en Descriptor of instance. \~
  \param[in] i        - \ru Номер экземпляра в первом паттерне.
                        \en Copy number in first pattern. \~
  \param[in] j        - \ru Номер экземпляра во втором паттерне.
                        \en Copy number in second pattern. \~
  \return \ru Дескриптор зарегистрированного ограничения.
          \en Descriptor of registered constrained. \~

  \details \ru Метод связывает два объекта ограничением паттерна.
           \en The method binds the two objects by a pattern constraint. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddPatterned( GCE_system gSys, pattern_item pItem, geom_item sample, geom_item instance, int i, int j );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Управляющий параметр" или "Фиксация переменной"
           \en Set the constraint "Driving parameter" or "Fixation of variable" \~
  
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] var  - \ru Дескриптор переменной.
                    \en Descriptor of variable. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
  \note \ru Созданное с помощью этой функции, ограничение может управляться 
        через вызов GCE_ChangeDrivingDimension.
        \en A constraint created with this function can be driven 
        via the call of GCE_ChangeDrivingDimension. \~
*/
//---
GCE_FUNC(constraint_item) GCE_FixVariable( GCE_system gSys, var_item var );

//----------------------------------------------------------------------------------------
/**
  \brief \ru Задать ограничение "Фиксация геометрического объекта".
         \en Set the constraint "Fixation of geom" \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g    - \ru Дескриптор геометрического объекта.
                    \en Descriptor of geometric object \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
*/
// ---
GCE_FUNC(constraint_item) GCE_FixGeom( GCE_system gSys, geom_item g );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Фиксированная длина кривой"
           \en Set the constraint "Fixation of curve length" \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] curve   - \ru Дескриптор кривой.
                    \en Descriptor of curve. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~

  \details \ru Ограничение поддерживается для тех же кривых, для которых поддерживается ограничение 
           #GCE_AddLength.
           \en The constraint is applicable for the same curves for which the #GCE_AddLength constraint is applicable.\~
*/
//---
GCE_FUNC(constraint_item) GCE_FixLength( GCE_system gSys, geom_item curve );

//----------------------------------------------------------------------------------------
/**
  \brief \ru Задать ограничение "Фиксированный радиус".
         \en Set the constraint "Fixation of radius" \~
  \details \ru Ограничение применимо для фиксации радиуса окружности или полуоси эллипса.
           \en The constraint is applicable to fix radius of circle or semiaxis of ellipse. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] circ - \ru Дескриптор окружности или эллипса.
                    \en Descriptor of circle or ellipse. \~
  \param[in] cName- \ru Тип фиксируемой координаты. Может быть радиус,  большая или малая полуось эллипса.
                    \en Type of fixed coordinate. It can be #GCE_RADIUS, 
                        #GCE_MAJOR_RADIUS or #GCE_MINOR_RADIUS.\~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
*/
// ---
GCE_FUNC(constraint_item) GCE_FixRadius( GCE_system gSys, geom_item circ, coord_name cName = GCE_RADIUS );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать фиксацию координаты параметрического объекта.
           \en Specify fixation of a parametric object coordinate. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] g      - \ru Дескриптор объекта.
                      \en Descriptor of object. \~
  \param[in] cName  - \ru Обозначение параметра объекта.
                      \en Denotation of object parameter. \~
  \return \ru Дескриптор зарегистрированного ограничения.
          \en Descriptor of registered constrained. \~

  \details \ru Задать фиксацию координаты параметрического объекта по типу координаты.
           \en Set the fixation of the parametric object coordinate by coordinate type. \~
*/
//---
GCE_FUNC(constraint_item) GCE_FixCoordValue( GCE_system gSys, geom_item g, coord_name cName );

//----------------------------------------------------------------------------------------
/**
  \brief 
    \ru Задать ограничение "Зафиксировать производную сплайна в заданной точке".
    \en Set the constraint "Fixation of derivative vector of the spline at a given point". \~
    \param[in] gSys - \ru Система ограничений. \en System of constraints. \~
    \param[in] spline - \ru Дескриптор сплайна. \en Descriptor of spline. \~
    \param[in] par - \ru Значение параметра, в котором надо зафиксировать производную.
                     \en Parameter value in which it is necessary to record a derivative. \~
    \param[in] derOrder - \ru Порядок производной, которую надо зафиксировать. \en Order of the derivative which must be fixed. \~
    \param[in] fixVal - \ru Значение, к которому надо приравнять производную. \en The value to which it is necessary to equate the derivative. \~
    \return \ru Дескриптор нового ограничения. \en Descriptor of a new constraint. \~
  \details
    \ru Точка фиксации задается через значение параметра, соответствующего ей. \n
      Порядок фиксируемой производной может равняться 0 (фиксация точки), 1, 2 или 3. \n
      Если fixVal равен nullptr, будет зафиксировано текущее значение производной,
      иначе фиксируемой производной будет присвоено значение fixVal.
    \en
      Fixation point is specified via the parameter value corresponding to it. \n
      The order of a fixed derivative can be equal 0 (point fixing), 1, 2 or 3. \n
      If fixVal is nullptr current value of the derivative vector will be fixed.
      Otherwise the derivative vector will be fixed at fixVal value. \~
 */
//---
GCE_FUNC(constraint_item) GCE_FixSplineDerivative(  GCE_system gSys, geom_item spline
                                                  , double par, uint derOrder, GCE_vec2d * fixVal = nullptr );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Средняя точка".
           \en Set the constraint "Middle point". \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] pnt  - \ru Дескрипторы трех точек.
                    \en Descriptors of point triplet. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
  \details \ru Для данных трех точек, задать отношение, связывающее тройку точек, 
  так, что третья точка лежит на середине отрезка между pnt[0] и pnt[1].
           \en For the given three points set the relation connecting the point triplet 
  in such way that the third point lies in the middle of points pnt[0] and pnt[1]. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddMiddlePoint( GCE_system gcSys, geom_item pnt[3] );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Параллельность".
           \en Set the constraint "Parallelism". \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g    - \ru Дескрипторы пары линейных объектов.
                    \en Descriptors of a pair of linear objects. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
  \details \ru Ограничение применяется для прямых или отрезков.
           \en The constraint is used for lines or segments. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddParallel( GCE_system gSys, geom_item g[2] );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Перпендикулярность".
           \en Set the constraint "Perpendicularity". \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g    - \ru Дескрипторы пары линейных объектов.
                    \en Descriptors of a pair of linear objects. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
  \details \ru Ограничение применяется для прямых или отрезков.
           \en The constraint is used for lines or segments. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddPerpendicular( GCE_system gSys, geom_item g[2] );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Симметрия относительно линейного объекта".
           \en Set the constraint "Symmetry relative to the linear object". \~

    \param[in] gSys - \ru Система ограничений.
               gSys - \en System of constraints. \~
    \param[in] g    - \ru Дескрипторы пары симметричных объектов.
               g    - \en Descriptors pair of symmetrical objects.. \~
    \param[in] lObj - \ru Дескриптор оси симметрии.
               lObj - \en Descriptor of the axis of symmetry. \~
    \return \ru Дескриптор нового ограничения.
            \en Descriptor of a new constraint. \~
    \details \ru Ограничение применимо для симметрии любых геометрических объектов, определённых в типе #geom_type.
             \en The constraint applies to symmetry of any geometric objects defined in #geom_type enum. \~
 */
//---
GCE_FUNC(constraint_item) GCE_AddSymmetry( GCE_system gSys, geom_item g[2], geom_item lObj );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Касание двух кривых".
           \en Set the constraint "Tangency of two curves". \~
  \param[in] \ru gSys Система ограничений.
             \en gSys System of constraints. \~
  \param[in] \ru g    Дескрипторы пары кривых или прямых.
             \en g    Descriptors of a pair of curves or lines. \~
  \param[in] \ru tPar Дескрипторы параметров касания для параметрических кривых.
             \en tPar Descriptors of parameters of tangency for parametric curves. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
  \details
  \par \ru Вспомогательные параметры касания
       \en Help parameters of tangency
  \ru Дескрипторы переменных tPar[0] и tPar[1] задают вспомогательные значения, параметризующие
      точку касания на первой и второй кривой. Одна или обе tPar могут быть равными GCE_NULL_V, 
      если соответствующая кривая не является сплайном или параметрической кривой, либо 
      пользователь согласен, что точка касания будет локализована автоматически по ближайшему решению.
  \en 
      Variable descriptors tPar[0], tPar[1] specify help values parametrizing a tangent point
      on the first and the second curve. One of both tPar can be equal GCE_NULL_V.

  \note \ru tPar[0] или tPar[1] могут быть равными GCE_NULL_V, если параметры 
        касания не предусмотрены или кривые не имеют параметрического представления. 
        Параметрическое представление имеют пока только два типа: 
        GCE_SPLINE и GCE_PARAMETRIC_CURVE.
        \en tPar[0] or tPar[1] may be equal to GCE_NULL_V if parameters 
        of tangency are not provided or curves have no parametric representation. 
        There are only two types having parametric representation: 
        GCE_SPLINE and GCE_PARAMETRIC_CURVE. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddTangent( GCE_system gSys, geom_item g[2], var_item tPar[2] );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать размерное ограничение "Расстояние между объектами".
           \en Set the dimensional constraint "Distance between objects". \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] g      - \ru Дескрипторы пары геометрических объектов.
                      \en Descriptors of a pair of geometric objects. \~
  \param[in] dPars  - \ru Параметры линейного размера (подробности см.#GCE_ldim_pars).
                      \en Parameters of linear dimension (see #GCE_adim_ldim). \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
  \details \ru Задать линейный размер для пары геометрических объектов.
           \en Set linear dimension for a pair of geometric objects. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddDistance( GCE_system gSys, geom_item g[2], const GCE_ldim_pars & dPars );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Расстояние между точками".
           \en Set the constraint "Distance between points". \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] p      - \ru Дескрипторы пары точек.
                      \en Descriptors of point pair. \~
  \param[in] dPars  - \ru Параметры размерного ограничения.
                      \en Parameters of dimensional constraint. \~
  
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddDistance2P( GCE_system gSys, geom_item p[2], const GCE_dim_pars & dPars );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Расстояние от точки до отрезка".
           \en Set the constraint "Distance from a point to a segment". \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] p      - \ru Дескрипторы тройки точек.
                      \en Descriptors of point triplet. \~
  \param[in] dPars  - \ru Параметры размерного ограничения.
                      \en Parameters of dimensional constraint. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
  \details \ru Линейный размер от точки p1 до отрезка <p0,p2>. Размер чувствителен к знаку 
               величины размера.
           \en Linear dimension from the point p1 to the segment <p0,p2>. 
               The dimension is sensitive to a sign of its value. \~   
*/
//---
GCE_FUNC(constraint_item) GCE_AddDistancePLs( GCE_system gSys, geom_item p[3]
                                            , const GCE_dim_pars & dPars );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Ориентированное расстояние между точками".
           \en Set the constraint "Directed distance between points". \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] p      - \ru Дескрипторы пары точек.
                      \en Descriptors of point pair. \~
  \param[in] dPars  - \ru Параметры размерного ограничения.
                      \en Parameters of dimensional constraint. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
  \details  \ru 
            Параметр dPars.dirAngle задает направление размера в радианах.
            Управляя dPars.dirAngle, можно задать вертикальный или 
            горизонтальный размеры. Так размер параметром dPars.dirAngle = 0
            создаст "горизонтальный размер", а dPars.dirAngle, равный PI/2 радиан
            будет соответствовать "вертикальному" размеру.
            \en 
            The constraint represents the dimension type that dimensions the 
            distance between two points in plane when they are projected onto a line 
            at an angle specified by parameter dPars dirAngle, which sets the direction 
            of dimension in radians. With driving dPars.dirAngle it is possible to 
            set the vertical or the horizontal dimension. So a dimension with an angle 
            equal to 0 radians specifies a "horizontal". The angle equal to PI/2 radians 
            corresponds to "vertical" dimension. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddDirectedDistance( GCE_system gSys, geom_item p[2]
                                                 , const GCE_ldim_pars & dPars );

//----------------------------------------------------------------------------------------
/** \brief \ru Добавить в систему ограничений линейный размер.
           \en Add the linear dimension to the constraint system. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] g      - \ru Дескрипторы пары зависимых объектов размера
                      \en Descriptors of a pair of the dependent geometric objects. \~
  \param[in] dPars  - \ru Параметры линейного размера (подробности см.#GCE_ldim_config).
                      \en Parameters of linear dimension (see #GCE_ldim_config). \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
  \details \ru Задать линейный размер для пары геометрических объектов.
           \en Set linear dimension for a pair of geometric objects. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddLinearDimension( GCE_system gSys, geom_item g[2]
                                                , const GCE_ldim_config & dPars );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать линейное уравнение.
           \en Set the linear equation. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] a      - \ru Вектор коэффициентов линейного уравнения.
                      \en Vector of coefficients of linear equation. \~
  \param[in] v      - \ru Дескрипторы переменных уравнения.
                      \en Descriptors of variables of equation. \~
  \param[in] n      - \ru Количество переменных.
                      \en Quantity of variables.\~
  \param[in] c      - \ru Коэффициент без переменной.
                      \en Free coefficient. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
  \details \ru Задать линейное уравнение в виде a1*v1 + a2*v2 + .. + an*vn + c = 0.
           \en Set a linear equation in form of a1*v1 + a2*v2 + .. + an*vn + c = 0. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddLinearEquation(  GCE_system gSys, const double * a
                                                , const var_item * v, size_t n, double c );

//----------------------------------------------------------------------------------------
/** \brief  \ru Задать ориентацию касания.
            \en Set the tangent orientation.
  \param[in] \ru gSys Система ограничений.
             \en gSys System of constraints. \~
  \param[in] \ru constraint Дескриптор ограничения.
             \en constraint Constraint's descriptor. \~
  \param[in] \ru alignment Опция выравнивания касания кривых.
             \en alignment Curve tangency alignment option. \~
  \return \ru В случае успешного вызова функция вернет новое значение выравнивания, заданного вызовом.
          \en If the call is successful, the function will return the new alignment value specified by the call. \~
  \details 
  \ru В настоящее время функция применяется к ограничению касания (GCE_TANGENT). Данный вызов устанавливает 
      сонаправленность или противонаправленность касательных (GCE_COORIENTED, GCE_OPPOSITE),
      либо делает выбор автоматически, если задать опцию GCE_AUTO_ALIGNMENT. Когда выбрана опцию GCE_CLOSEST, 
      солвер будет поддерживать взаимную ориентацию согласно текущего размещения геометрии.
  \en The function currently applies to the tangency constraint (GCE_TANGENT). The call sets whether 
      the direction of the geometry tangents are cooriented/opposite or makes the selection automatically 
      if you set the GCE_AUTO_ALIGNMENT option. When the option GCE_CLOSEST is selected the C3D Solver 
      should maintain the current geometry positions. \~
*/
//---
GCE_FUNC(GCE_alignment) GCE_SetAlignment( GCE_system gSys, constraint_item constraint, GCE_alignment alignment = GCE_AUTO_ALIGNMENT );

//----------------------------------------------------------------------------------------
/** \brief \ru Изменить значение управляющего размера.
           \en Change the value of driving dimension. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] dItem  - \ru Дескриптор размерного ограничения.
                      \en Descriptor of dimensional constraint. \~
  \param[in] dVal   - \ru Требуемое значение размера.
                      \en Required value of constraint. \~
  \return \ru Код результата операции.
          \en Operation result code. \~
  \details \ru Функция применяется только для управляющих размеров или управляющих параметров. 
  Если управляющий размер или параметр является угловым, то параметр dVal задается в радианах.\n
  Следует учитывать, что настоящая функция не осуществляет вычислений, а только 
  подготавливает изменение размера. Что бы изменения вступили в силу, необходимо вызвать 
  функцию #GCE_Evaluate.
           \en The function is used only for driving dimensions or driving parameters. 
  If the driving dimension or parameter is angular, then the parameter dVal is specified in radians. \n
  It should be taken into account that the function doesn't perform computations but only 
  prepares the changing of dimension. For the changes to take effect it is required 
  to call the function #GCE_Evaluate. \~
*/
//---
GCE_FUNC(GCE_result) GCE_ChangeDrivingDimension( GCE_system gSys, constraint_item dItem, double dVal );

//----------------------------------------------------------------------------------------
/** \brief \ru Отклонить ограничение от точки решения.
           \en Deviate the constraint from the point of solution. \~
  \param[in]  dItem   - \ru Дескриптор геометрического ограничения.
                        \en Descriptor of geometric constraint. \~
  \param[in]  delta   - \ru Величина отклонения.
                        \en Deviation value. \~
  \return errCode - \ru Результат решения системы ограничений.
                    \en Solution of constraint system. \~
  
  \details \ru Функция применяется для диагностики избыточности ограничения, основанной 
  на отклонении области решений ограничения. Работает только для размерных ограничений
  и некоторых типов геометрических ограничений, таких как "Выравнивание точек", 
  "Горизонтальность" и т.д. Применимость к тому или иному типу не задокументирована и 
  определяется опытным путем.
  Если было возвращено значение GCE_RESULT_None, то ограничение не отклонялось.
           \en The function is used for the diagnostics of constraints redundancy based 
  on the deviation of the region of solution of the constraint. It works only for dimensional constraints
  and other types of geometric constraints such as "Points alignment", 
  "Horizontality" etc. The applicability to a certain type was not documented and 
  it is defined only empirically.
  If GCE_RESULT_None is returned, the constraint wasn't deviated. \~
*/
// ---
GCE_FUNC(GCE_result) GCE_DeviateDimension( GCE_system gSys, constraint_item dItem, double delta );

//----------------------------------------------------------------------------------------
/** \brief \ru Тест избыточности ограничения, основанный на отклонении его от точки решения.
           \en Test for redundancy of constraint based on the deviation the constraint from the point of solution. \~
  \param[in]  dItem   - \ru Дескриптор геометрического ограничения.
                        \en Descriptor of geometric constraint. \~
  \param[in]  delta   - \ru Величина отклонения.
                        \en Deviation value. \~
  \return - \ru Результат решения системы ограничений.
            \en Solution of constraint system. \~
  
  \details \ru Функция применяется для диагностики избыточности ограничения, основанной 
  на отклонении области решений ограничения. Работает только для размерных ограничений
  и некоторых типов геометрических ограничений, таких как "Выравнивание точек", 
  "Горизонтальность" и т.д. Применимость к тому или иному типу (кроме размерных) не 
  задокументирована и определяется опытным путем. 
  Если было возвращено значение GCE_RESULT_None, то ограничение не отклонялось.
           \en The function is used for the diagnostics of constraints redundancy based 
  on the deviation of the region of solution of the constraint. It works only for dimensional constraints
  and other types of geometric constraints such as "Points alignment", 
  "Horizontality" etc. The applicability to a certain type (for non dimensional) 
  was not documented and it is defined only empirically. 
  If GCE_RESULT_None is returned, the constraint wasn't deviated. \~

  \note \ru В отличии от #GCE_DeviateDimension не меняется состояние системы ограничений.
        \en Unlike #GCE_DeviateDimension this function does not change state of geometric constraint system.
*/
// ---
GCE_FUNC(GCE_result) GCE_DeviationTest( GCE_system gSys, constraint_item dItem, double delta );

//----------------------------------------------------------------------------------------
/** \brief    \ru Текущее значение размерного параметра.
              \en A current value of the dimension parameter.
    \details  \ru Функция выдает текущее значение размерного параметра ограничения. Если 
  ограничение не размерное, то функция вернет GCE_UNDEFINED_DBL. Для управляющих размеров 
  будет выдано значение управляющего параметра, которое было задано при создании размера 
  или последним вызовом GCE_ChangeDrivingDimension.
              \en The function returns a value of dimension parameter of the constraint. 
  If the constraint is a driving dimension, the function returns a value of dimension 
  parameter specified when creating the constraint or last call of #GCE_ChangeDrivingDimension.    
*/
//---
GCE_FUNC(double) GCE_DimensionParameter( GCE_system gSys, constraint_item dItem );
                                       
//----------------------------------------------------------------------------------------
/** \brief \ru Вычислить систему ограничений.
           \en Calculate the constraint system. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \return \ru Код результата вычислений.
          \en Calculation result code. \~
  \details \ru Функция решает задачу ограничений. Задача ограничений формулируется 
  функциями API геометрического решателя; функции вида GCE_Add_XXXXXXX добавляют новые 
  объекты, функции вида GCE_Change_XXXXXXX, GCE_Set_XXXXXXX изменяют состояние 
  объектов. Таким образом, что бы все такие изменения вступили в силу, нужно 
  вызвать метод #GCE_Evaluate.\n
    Алгоритмы GCE_Evaluate учитывают удовлетворенность систем ограничений; если
  все ограничения уже решены, то функция не тратит время на вычисления, а
  состояние геометрических объектов остается неизменным.
           \en The function solves problem of constraints. The problem of constraint is formulated 
  by API functions of geometric solver; the functions of a kind GCE_Add_XXXXXXX add a new 
  object, the functions of kinds GCE_Change_XXXXXXX and GCE_Set_XXXXXXX change a state 
  of objects. Thus, for all changes to take effect it is necessary 
  to call the method #GCE_Evaluate.\n
    The algorithms GCE_Evaluate take into account whether constraint systems are satisfied, if
  all constraints have been already solved, then the function doesn't spend time for calculations, and
  the state of geometric objects remains unchanged. \~
*/
//---
GCE_FUNC(GCE_result) GCE_Evaluate( GCE_system gSys );

//----------------------------------------------------------------------------------------
/** \brief \ru Инициализировать режим драггинга контрольной точки объекта.
           \en Initialize the dragging mode of the object control point. \~
  \param[in] \ru gSys   Система ограничений.
             \en gSys   System of constraints. \~
  \param[in] \ru obj    Геометрический объект.
             \en obj    Geometric object. \~
  \param[in] \ru pntId  Обозначение передвигаемой контрольной точки объекта.
             \en pntId  Denotation of the dragged control point of the object. \~
  \param[in] \ru curXY  Координаты курсора, куда следует перемещаемая точка.
             \en curXY  Coordinates of cursor where the moving point follows. \~
  \return \ru Код ошибки. Вернет код GCE_RESULT_Ok, если подготовка режима драггина прошла успешна.
          \en Error code. Returns GCE_RESULT_Ok if preparing the dragging mode was successful.\~
*/
//---
GCE_FUNC(GCE_result) GCE_PrepareMovingOfPoint( GCE_system gSys, geom_item obj
                                             , point_type pntId, GCE_point curXY );

//----------------------------------------------------------------------------------------
/** \brief \ru Инициализировать режим драггинга контрольной точки объекта.
           \en Initialize the dragging mode of the object control point. \~

  \param[in] gSys    - \ru Система ограничений.
                       \en System of constraints. \~
  \param[in] drgPnt  - \ru Контрольная точка объекта.
                       \en A geom control point. \~
  \param[in] curXY   - \ru Координаты точки драггинга.
                       \en Coordinates of dragging point. \~
  \return \ru Код ошибки. Вернет код GCE_RESULT_Ok, если подготовка режима драггина прошла успешна.
          \en Error code. Returns GCE_RESULT_Ok if preparing the dragging mode was successful.\~
*/
//---
GCE_FUNC(GCE_result) GCE_PrepareDraggingPoint( GCE_system gSys, GCE_dragging_point drgPnt
                                             , GCE_point curXY );

//----------------------------------------------------------------------------------------
/** \brief \ru Инициализировать режим драггинга контрольной точки множества объектов.
           \en Initialize the dragging mode of the control point of object set. \~
    \details \ru Этот метод предназначен для группового редактирования (драггинг) 
  нескольких объектов с "общей" hot-точкой. Под "общей" hot-точкой подразумевается 
  не обязательно одна точка (с одним дескриптором), а множество точек с разными 
  дескрипторами, но имеющих одинаковые координаты.
             \en This method is intended for the group dragging of few objects with 
  the "common" hot-point. A "common" hot-point is not necessarily the only point 
  (with the only descriptor), but a set of points with different descriptors but 
  with equal coordinates. \~

  \param[in] gSys     - \ru Система ограничений.
                        \en System of constraints. \~
  \param[in] cPntArr  - \ru Множество геометрически одинаковых контрольных точек.
                        \en A set of geometrically equal control points. \~
  \param[in] curXY    - \ru Координаты точки драггинга.
                        \en Coordinates of dragging point. \~
  \return \ru Код ошибки. Вернет код GCE_RESULT_Ok, если подготовка режима драггина прошла успешна.
          \en Error code. Returns GCE_RESULT_Ok if preparing the dragging mode was successful.\~
*/
//---
GCE_FUNC(GCE_result) GCE_PrepareDraggingPoint(  GCE_system gSys
                                              , const std::vector<GCE_dragging_point> & cPntArr
                                              , GCE_point curXY );

//----------------------------------------------------------------------------------------
/**
  \brief \ru Инициализировать режим перетаскивания множества объектов.
         \en Initialize mode of moving a set of objects.
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param geoms -    \ru Множество геометрических объектов.
                    \en Set of geometric objects. \~
  \param curXY -    \ru Координаты точки драггинга.
                    \en Coordinates of dragging point. \~
  \return \ru Код ошибки. Вернет код GCE_RESULT_Ok, если подготовка режима драггина прошла успешна.
          \en Error code. Returns GCE_RESULT_Ok if preparing the dragging mode was successful.\~

*/
//---
GCE_FUNC(GCE_result) GCE_PrepareMovingGeoms( GCE_system gSys
                                           , std::vector<geom_item> & geoms
                                           , GCE_point curXY );

//----------------------------------------------------------------------------------------
/** \brief \ru Переместить точку драггинга.
           \en Move a dragging point. \~
  \param[in] gcSys  - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] curXY  - \ru Текущие координаты курсора.
                      \en Current coordinates of cursor. \~
  \return \ru Код ошибки. Вернет код GCE_RESULT_Ok, если подготовка режима драггина прошла успешна.
          \en Error code. Returns GCE_RESULT_Ok if preparing the dragging mode was successful.\~

  \details \ru Процедура обслуживает режим драггинга, с ее помощью геометрический решатель
  отслеживает положение курсора. Этот вызов позволяет осуществлять двух-координатное
  управление не доопределенной моделью. Если функция возвращает код ошибки, 
  не равный #GCE_RESULT_Ok, то гарантируется, что состояние геометрических объектов 
  останется неизменным. Если функция вернула #GCE_RESULT_Ok, то решатель содержит 
  новое состояние геометрических объектов, удовлетворяющее всем ранее наложенным 
  ограничениям (новое решение). В этом случае вызывать #GCE_Evaluate для приведения 
  объектов в решенное состояние не требуется.
           \en The procedure services the dragging mode, with a help of it a geometric solver
  tracks the cursor location. This call allows to perform a two-coordinate
  control of an underdetermined model. If the function returns an error code, 
  which is not #GCE_RESULT_Ok, then it is guaranteed that the state of geometric objects 
  will remain the same. If the function returned #GCE_RESULT_Ok, then the solver contains 
  a new state of geometric objects satisfying to all the constraints created before 
  (a new solution). In this case it not required to call #GCE_Evaluate for the conversion
  of objects to the solved state. \~
*/
//---
GCE_FUNC(GCE_result) GCE_MovePoint( GCE_system gcSys, GCE_point curXY );

//----------------------------------------------------------------------------------------
/**
  brief \ru Трансформировать геометрические объекты согласно заданной матрице.
        \en Transform geometric objects according to a given matrix. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] geoms - \ru Множество геометрических объектов.
                     \en Set of geometric objects. \~
  \param[in] mat  - \ru Матрица преобразования.
                    \en Transformation matrix. \~
  \return \ru Код ошибки. \en Error code. \~

  \details \ru Режим динамической трансформации для данного набора геометрических объектов
  включается при первом вызове функции #GCE_DynamicTransform. При этом запоминаются начальные
  положения геометрических объектов и далее трансформации при каждом новом вызове
  #GCE_DynamicTransform выполняются относительно их первоначального положения до тех пор, пока
  режим динамической трансформации не будет выключен. Выключается режим динамической
  трансформации геометрических объектов при вызове любой другой функции API или при вызове
  функции #GCE_DynamicTransform для другого набора геометрических объектов.
           \en The dynamic transformation mode for given set of geometric objects is turned on
  after the first time the #GCE_DynamicTransform function is called. In this case, the initial
  positions of geometric objects are remembered and then the transformations for each new call
  of the #GCE_DynamicTransform are performed relative to their initial position until the dynamic
  transformation mode is turned off. The mode of dynamic transformation of geometric objects is
  turned off when calling any other API function or when calling the #GCE_DynamicTransform
  function for another set of geometric objects. \~
*/// ---
GCE_FUNC(GCE_result) GCE_DynamicTransform( GCE_system gSys, const std::vector<geom_item> & geoms, const MbMatrix & mat );

//----------------------------------------------------------------------------------------
/**
  brief \ru Трансформировать геометрию системы ограничений согласно заданной матрице.
        \en Transform the geometry of the constraints system according to a given matrix. \~
  \details \ru Если преобразование несобственное, т.е. включает в себя отражение, то изменится
  направление кривых, ограниченных двумя точками, у которых базовая кривая замкнутая.
  Это значит, что после преобразования, запрос #GCE_PointOf для таких кривых в качестве #GCE_FIRST_END
  будет возвращать точку, которую до преобразования выдавал в качестве #GCE_SECOND_END и наоброт.
           \en  If the transformation is improper, i.e. includes reflection, direction of bounded curves
 for which the base curve is closed will be changed. This means that after transformation,
 the request #GCE_PointOf for such curves will return as a #GCE_FIRST_END a point that
 before transformation returned as #GCE_SECOND_END and vice versa.\~


  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] mat  - \ru Матрица преобразования.
                    \en Transformation matrix. \~
  \return \ru Код ошибки. \en Error code. \~
*/// ---
GCE_FUNC(GCE_result) GCE_Transform( GCE_system gSys, const MbMatrix & mat );

//----------------------------------------------------------------------------------------
/**
  \brief  \ru Включить журналирование и назначить файл для записи журнала вызовов API.
          \en Switch on the journalling and specify the file for recording a journal of GCE API calls. \~
  \param[in] gSys -   \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] fName -  \ru Имя файла назначения с полным путем.
                      \en Name of destination file with a full path. \~
  \return true, if journalling has been successfully switched on.
  \attention  
          \ru Файл журнала будет записан только после завершения сеанса работы с системой
              ограничений, а именно сразу после вызова GCE_RemoveSystem.
          \en The journal file will be written only when a session of work with the
              constraint system is finished, i.e. immediately after calling the
              GCE_RemoveSystem method.
*/
//---
GCE_FUNC(bool) GCE_SetJournal( GCE_system gSys, const char * fName );

//----------------------------------------------------------------------------------------
/** \brief  \ru Выдать журнал записывающий вызовы Solver API, а также другие события, имеющие отношение к взаимодействием с солвером.
            \en Get a logger that recording API calls, as well as other events related to interaction with the solver. \~
  \note \ru Вызов используется пока только для тестовых целей. Может быть изменен или удален из API в следующих ревизиях.
        \en The call is used for testing purposes only. It may be changed or removed in future revisions. \~
*/
//---
GCE_FUNC(GCE_journal) GCE_SubscribeJournal( GCE_system gSys, GCE_log_func logFunc, GCE_extra_param extParam );

//----------------------------------------------------------------------------------------
/**
  \brief \ru Устаревшая функция. Вызов будет удален в одной из следующих версий.
             Рекомендуется использовать новую функцию: #GCE_DeviateDimension( GCE_system gSys, constraint_item dItem, double delta )
         \en An obsolete function. The call will be removed in one of the next versions. 
             It's recommended to use new version of this function: #GCE_DeviateDimension( GCE_system gSys, constraint_item dItem, double delta )\~
 */
// ---
GCE_FUNC(bool) GCE_DeviateDimension( GCE_system gSys, constraint_item dItem
                                   , double delta, GCE_result & errCode );

//----------------------------------------------------------------------------------------
/**
  \brief \ru Устаревшая функция. Вызов будет удален в одной из следующих версий.
             Рекомендуется использовать новую функцию: #GCE_DeviationTest( GCE_system gSys, constraint_item dItem, double delta )
         \en An obsolete function. The call will be removed in one of the next versions. 
             It's recommended to use new version of this function: #GCE_DeviationTest( GCE_system gSys, constraint_item dItem, double delta )\~
 */
// ---
GCE_FUNC(bool) GCE_DeviationTest( GCE_system gSys, constraint_item dItem
                                , double delta, GCE_result & errCode );

//----------------------------------------------------------------------------------------
/** \brief \ru Устаревшая функция. Вызов будет удален в одной из следующих версий.
           \en An obsolete function. The call will be removed in one of the next versions. \~

  \attention \ru Время жизни экземпляра класса crv опирается на счетчик ссылок, т.е. 
  решатель его увеличивает при добавлении кривой и декрементирует при удалении кривой из решателя.
             \en The lifetime of the instance of the class 'crv' is based on the reference counter, i.e. 
  the solver increases it when adding a curve and decreases when deleting a curve from the solver. \~
*/
//---
class MbPolyCurve;
GCE_FUNC(geom_item) GCE_AddSpline( GCE_system gSys, const MbPolyCurve & crv );

//----------------------------------------------------------------------------------------
/** 
  \attention \ru Устаревшая функция. Вызов будет удален в одной из следующих версий.
             \en An obsolete function. The call will be removed in one of the next versions. \~
*/
//---
inline geom_item GCE_AddPoint( GCE_system gSys, GCE_point pVal, int ) 
{
  return GCE_AddPoint( gSys, pVal );
}

//----------------------------------------------------------------------------------------
/** 
  \attention \ru Устаревшая функция. Вызов будет удален в одной из следующих версий (2016).
             \en An obsolete function. The call will be removed in one of the next versions (2016). \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddDirectedDistance2P( GCE_system gSys, geom_item p[2]
                                                   , const GCE_ldim_pars & dPars );

//----------------------------------------------------------------------------------------
/** 
  \attention \ru Устаревшая функция. Вызов будет удален в одной из следующих версий.
             \en An obsolete function. The call will be removed in one of the next versions. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddAlignPoints( GCE_system gSys, geom_item p[2], bool hor );

//----------------------------------------------------------------------------------------
/**
  \attention \ru Функция устарела. Вместо неё применять #GCE_FixLength.
             \en The function is obsolete. Use #GCE_FixLength instead. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddFixedLength( GCE_system, geom_item );

//----------------------------------------------------------------------------------------
/**
  \attention \ru Функция устарела. Вместо неё применять #GCE_FixVariable.
             \en The function is obsolete. Use #GCE_FixVariable instead. \~
*/
//---
GCE_FUNC(constraint_item) GCE_AddFixVariable( GCE_system, var_item );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение "Точка на кривой".
           \en Set the constraint "Point on curve". \~
    \attention This call is deprecated. Call #GCE_AddCoincidence instead.
*/
//---
GCE_FUNC(constraint_item) GCE_AddIncidence( GCE_system, geom_item, geom_item );

/** \} */

#endif // __GCE_API_H

// eof
