//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file \brief  \ru Программный интерфейс 3D решателя геометрических ограничений.
                \en Program interface of three-dimensional geometric constraints solver. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GCM_API_H
#define __GCM_API_H

#include <math_define.h>
#include <mb_cart_point3d.h>
#include <mb_placement3d.h>

#include <gcm_types.h>

class reader;
class writer;

/**
  \addtogroup GCM_3D_API
  \{
*/

/*
  Constructing and deleting a constraint system
*/

//----------------------------------------------------------------------------------------
/** \brief \ru Создать пустую систему ограничений.
           \en Create a simple constraint system. \~  
    \details \ru Вызов создает пустую систему ограничений.  Кроме того, в памяти создаются 
  внутренние структуры данных геометрического решателя, обслуживающего систему ограничений. 
  Функция возвращает специальный дескриптор,  по которому система ограничений доступна для 
  различных манипуляций:  добавление или удаление геометрических объектов,  ограничений, 
  варьирование размеров, драггинг недоопределенных объектов и т.д.
             \en The call creates a simple constraint system.  Besides,  there are created 
  internal data structures of geometric solver maintaining the system of constraints. 
  The function returns a special descriptor by which the constraint system is available 
  for various manipulations: addition and deletion of geometric objects, constraints, 
  variation of sizes, dragging underconstrained objects etc. \~
          
  \return \ru Дескриптор системы ограничений.
          \en Descriptor of  constraint system. \~
*/
//---
GCM_FUNC(GCM_system) GCM_CreateSystem();

//----------------------------------------------------------------------------------------
/** \brief \ru Сделать систему ограничений пустой.
           \en Make the constraint system empty. \~
  \details \ru Данный метод делает систему ограничений пустой при этом дескриптор gSys 
  остается действительным, т.е. можно осуществлять дальнейшую работу с системой ограничений.
           \en This method makes the constraint system empty while the descriptor gSys 
  remains valid, i.e. it is possible to perform the further work with the constraint system. \~

  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \sa #GCM_RemoveSystem
*/
//---
GCM_FUNC(void) GCM_ClearSystem( GCM_system gSys );

//----------------------------------------------------------------------------------------
/** \brief \ru Удалить систему ограничений.
           \en Delete system of constraints. \~    
    \details \ru Данный метод делает систему ограничений недействительной. Осуществляется 
    освобождение ОЗУ от внутренних структур данных, обслуживающих систему ограничений.
             \en This method makes the constraint system invalid. Deallocation of RAM 
    from the internal data structures maintaining the system of constraints is performed. \~
    \param[in] gSys - \ru Система ограничений.
                      \en System of constraints. \~
    \sa #GCM_ClearSystem
*/
//---
GCM_FUNC(void) GCM_RemoveSystem( GCM_system gSys );

//----------------------------------------------------------------------------------------
/** \brief \ru Читать систему ограничений из потока
           \en Read constraint system from stream. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] in -   \ru Поток для чтения.
                    \en Stream for reading. \~
*/
//---
GCM_FUNC(bool) GCM_ReadSystem( GCM_system gSys, reader & in );

//----------------------------------------------------------------------------------------
/** \brief \ru Запись системы ограничений в поток
           \en Write constraint system to stream. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] out -  \ru Поток для записи.
                    \en Stream for writing. \~
*/
//---
GCM_FUNC(bool) GCM_WriteSystem( GCM_system gSys, writer & out );

//----------------------------------------------------------------------------------------
/// Query to interrupt calculations.
//---
typedef bool ( *GCM_abort )(); 

//----------------------------------------------------------------------------------------
/** \brief \ru Назначить функцию прерывания вычислений.
           \en Set a callback to interrupt the calculations. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] cbFunc -   \ru Функция обратного вызова для прерывания операций.
                        \en A callback to interrupt the calculation. \~
*/
//---
GCM_FUNC(void) GCM_SetCallback( GCM_system gSys, GCM_abort cbFunc );


/*
  Specifying geometry data structures (GCM_g_record)
*/

//----------------------------------------------------------------------------------------
/** \brief  \ru Выдать запись пустого геометрического объекта.
            \en Give the record of empty geometric object. \~
*/
//---
GCM_FUNC(GCM_g_record) GCM_NullGeom();

//----------------------------------------------------------------------------------------
/** \brief  \ru Выдать запись точки из типа MbCartPoint3D в типе GCM_g_record.
            \en Get a record of point from the type MbCartPoint3D to the type GCM_g_record. \~
*/
//---
GCM_FUNC(GCM_g_record) GCM_Point( const MbCartPoint3D & );


//----------------------------------------------------------------------------------------
/** \brief  \ru Запись оси, заданной её точкой и направляющим вектором.
            \en Record of axis specified by the point and direction vector. \~
*/
//---
GCM_FUNC(GCM_g_record) GCM_Axis( const MbCartPoint3D & org
                               , const MbVector3D & axisZ );

//----------------------------------------------------------------------------------------
/** \brief  \ru Запись прямой, заданной её точкой и направляющим вектором.
            \en Record of line specified by the point and direction vector. \~
*/
//---
GCM_FUNC(GCM_g_record) GCM_Line( const MbCartPoint3D & org
                               , const MbVector3D & axisZ );

//----------------------------------------------------------------------------------------
/** \brief  \ru Запись плоскости, заданной точкой и нормалью.
            \en Record of plane specified by the point and normal vector. \~
*/
//---
GCM_FUNC(GCM_g_record) GCM_Plane( const MbCartPoint3D & org, const MbVector3D & axisZ );

//----------------------------------------------------------------------------------------
/**  \brief  \ru Получить запись конуса по заданному набору параметров.
             \en Get data record of cone for the given set of parameters. \~
  \param[in] centre -  \ru Центр окружности-основания конуса.
                       \en Center of base circle of the cone. \~
  \param[in] axis   -  \ru Направляющий вектор оси конуса.
                       \en Direction vector of the cone axis. \~
  \param[in] radiusA - \ru Радиус основания конуса.
                       \en Radius of the base circle. \~
  \param[in] radiusB - \ru Радиус сечения конуса ("малый" радиус).
                       \en Radius of section of circle ("minor" radius). \~
  \return \ru Запись конуса.
          \en Record of cone. \~

  \details  \ru Предполагается, что параметры конуса описывают воображаемый усеченный конус, 
                высота которого всегда равна единице длины. При этом radiusA - это радиус 
                основания конуса, а radiusB - радиус его сечения.
            \en It is assumed that the parameters describe the imaginary cone frustum, 
                whose height is always unit of length. In this radiusA - is the radius of 
                the base of the cone, and radiusB - the radius of its cross-section.
*/
//---
GCM_FUNC(GCM_g_record) GCM_Cone(  const MbCartPoint3D & centre, const MbVector3D & axis
                                , double radiusA, double radiusB );

//----------------------------------------------------------------------------------------
/**  \brief  \ru Получить запись цилиндра по заданному набору параметров.
             \en Get data record of cylinder for the given set of parameters. \~
  \param[in] centre -  \ru Центр окружности-основания цилиндра.
                       \en Center of base circle of the cylinder. \~
  \param[in] axis   -  \ru Направляющий вектор оси цилиндра.
                       \en Direction vector of the cylinder axis. \~
  \param[in] radius - \ru Радиус основания цилиндра.
                      \en Radius of the base circle. \~
  \return \ru Запись цилиндра.
          \en Record of cylinder. \~
*/
//---
GCM_FUNC(GCM_g_record) GCM_Cylinder( const MbCartPoint3D & centre, const MbVector3D & axis
                                   , double radius );

//----------------------------------------------------------------------------------------
/**  \brief \ru Получить запись окружности, заданной набором параметров.
            \en Get record of circle specified by the set of parameters. \~
  \param[in] centre - \ru Центр окружности.
                      \en Center of the circle. \~
  \param[in] axis   - \ru Направляющий вектор оси окружности.
                      \en Direction vector of the circle axis. \~
  \param[in] radius - \ru Радиус окружности.
                      \en Radius of the circle. \~
  \return \ru Запись данных об окружности.
          \en Data record of the circle. \~
*/
//---
GCM_FUNC(GCM_g_record) GCM_Circle( const MbCartPoint3D & centre, const MbVector3D & axis, double radius );

//----------------------------------------------------------------------------------------
/**  \brief  \ru Получить запись тороида по заданному набору параметров.
             \en Get data record of torus for the given set of parameters. \~
  \param[in] centre -  \ru Центр тора.
                       \en Center of torus. \~
  \param[in] axis   -  \ru Направляющий вектор оси вращения.
                       \en Direction vector of the rotation axis. \~
  \param[in] majorR -  \ru "Большой" радиус тора - радиус окружности, описывающей вращение центра сечения.
                       \en "Major" radius is the radius of circle sweeping center of the rotating section.                            
                       \~
  \param[in] minorR - \ru Радиус окружности вращения ("малый" радиус).
                      \en Radius of section of circle ("minor" radius). \~

  \details  \ru Таким образом предполагается, что тор это воображаемая поверхность вращения, образованная 
                вращением окружности с радиусом minorR, лежащей в одной плоскости с осью вращения и центром, 
                расположенном на расстоянии majorR от оси тора.
            \en Thus, it is assumed that the torus is an imaginary surface formed by 
                rotation of a circle of "minor radius" lying in the same plane as the axis 
                of rotation and the center located at a distance of majorR from the axis.
*/
//---
GCM_FUNC(GCM_g_record) GCM_Torus( const MbCartPoint3D & centre, const MbVector3D & axis
                                , double majorR, double minorR );

//----------------------------------------------------------------------------------------
/** \brief  \ru Получить запись данных о сфере, заданной центром и радиусом.
            \en Get data record of sphere specified by center and radius. \~
  \param[in] centre - \ru Центр сферы.
                      \en Center of the sphere. \~
  \param[in] radius - \ru Радиус сферы.
                      \en Radius of the sphere. \~
  \return \ru Запись данных о сфере.
          \en Data record of the sphere. \~
*/
//---
GCM_FUNC(GCM_g_record) GCM_Sphere( const MbCartPoint3D & centre, double radius );

//----------------------------------------------------------------------------------------
/** \brief  \ru Получить запись данных твердого тела, заданной началом координат и осями Z, X.
            \en Get a data record of solid specified by its origin of coordinates, Z-axis and X-axis. \~
  \details 
          \ru Результат, который возвращает данная функция, используется для задания 
              в системе ограничений твердого тела (кластера) с помощью вызовов 
              GCM_AddGeom или #GCM_SubGeom.
          \en The result, which returns this function, is used to specify a rigid body
              (cluster) in the system by calling #GCM_AddGeom or #GCM_SubGeom. \~
*/
//---
GCM_FUNC(GCM_g_record) GCM_SolidLCS(  const MbCartPoint3D & org
                                    , const MbVector3D & axisZ = MbVector3D::zAxis
                                    , const MbVector3D & axisX = MbVector3D::xAxis );

//----------------------------------------------------------------------------------------
/** \brief  \ru Получить запись данных системы координат твердого тела.
            \en Get a data record of the solid coordinate system by its placement. \~
  \details 
          \ru Результат, который возвращает данная функция, используется для задания 
              в системе ограничений твердого тела (кластера) с помощью вызовов
              GCM_AddGeom или GCM_SubGeom.
          \en The result, which returns this function, is used to specify a rigid body
              (cluster) in the system by calling #GCM_AddGeom or #GCM_SubGeom. \~
*/
//---
GCM_FUNC(GCM_g_record) GCM_SolidLCS( const MbPlacement3D & );

/*
  Defining geometry of constraint system
*/

//----------------------------------------------------------------------------------------
/** \brief \ru Добавить в систему ограничений точку.
           \en Add point to the constraint system. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] pVal - \ru Координаты точки.
                    \en Coordinates of a point. \~
  \return \ru Дескриптор зарегистрированной точки.
          \en Descriptor of registered point. \~
*/
//---
GCM_FUNC(GCM_geom) GCM_AddPoint( GCM_system gSys, const MbCartPoint3D & pVal );

//----------------------------------------------------------------------------------------
/** \brief \ru Добавить в систему ограничений геометрический объект.
           \en Add geometric object to the constraint system. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] gRec - \ru Запись геометрического объекта.
                    \en Record of geometric record. \~
  \return \ru Дескриптор зарегистрированного объекта.
          \en Descriptor of registered object. \~
*/
//---
GCM_FUNC(GCM_geom) GCM_AddGeom( GCM_system gSys, const GCM_g_record & gRec );

//----------------------------------------------------------------------------------------
/** \brief \ru Добавить в систему ограничений геометрический объект.
           \en Add geometric object to the constraint system. \~
  \param[in] gSys  - \ru Система ограничений.
                     \en System of constraints. \~
  \param[in] gType - \ru Тип геометрического объекта.
                     \en Type of geometric object. \~
  \param[in] gMat  - \ru Прямая матрица ЛСК объекта.
                     \en Direct matrix of geometric object. \~
  \param[in] radiusA -  \ru Радиус окружности, цилиндра, сферы, а также "мажорный" радиус конуса и тора.
                        \en Radius of circle, cylinder, sphere, also "major" radius of cone and torus. \~
  \param[in] radiusB -  \ru "Минорный" радиус конуса или тора.
                        \en "Minor" radius of cone and torus. \~
  \return            \ru Дескриптор зарегистрированного объекта.
                     \en Descriptor of registered object. \~
*/
//--
GCM_FUNC(GCM_geom) GCM_AddGeom( GCM_system gSys, GCM_g_type gType
                              , const MbMatrix3D & gMat
                              , double radiusA, double radiusB );

//----------------------------------------------------------------------------------------
/** \brief \ru Добавить в подсистему твердого тела (кластера) подчиненный геометрический объект.
           \en Include a geometric sub-object to the subsystem of a solid (rigid cluster). \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] sol  - \ru Твердое тело или кластер.
                    \en Solid or rigid cluster. \~
  \param[in] gRec - \ru Запись геометрического подчиненного объекта, заданного в ЛСК тела.
                    \en Record of geometric sub-object, which is given in LCS of the solid. \~
  \return \ru Дескриптор подчиненного объекта из подмножества тела.
          \en Descriptor of sub-object in subset of the solid. \~
*/
//---
GCM_FUNC(GCM_geom) GCM_SubGeom( GCM_system gSys, GCM_geom sol, const GCM_g_record & gRec );

//----------------------------------------------------------------------------------------
/** \brief \ru Выдать кластер (тело), в который включен данный геометрический объект.
           \en Give a cluster (solid) in which a geometric object is included. \~
  \param[in] gSys -   \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] subGeom -\ru Геометрический объект, принадлежащий кластеру.
                      \en A geometric object belonging to the cluster.. \~
  \return \ru Дескриптор кластера, которому принадлежит данный геометрический объект.
          \en Descriptor of the cluster that owns this geometric object. \~
*/
//---
GCM_FUNC(GCM_geom) GCM_Parent( GCM_system gSys, GCM_geom subGeom );

//----------------------------------------------------------------------------------------
/** \brief \ru Тип геометрического объекта.
           \en A type of geometric object. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~  
  \param[in] g    - \ru Дескриптор геометрического объекта.
                    \en Descriptor of geometric object \~
  \return \ru Геометрический тип объекта.
          \en Geometric type of an object. \~
*/
//---
GCM_FUNC(GCM_g_type) GCM_GeomType( GCM_system gSys, GCM_geom g );

//----------------------------------------------------------------------------------------
/** \brief \ru Удалить геометрический объект из системы ограничений.
           \en Delete a geometric object from the constraint system. \~
  \param gSys - \ru Система ограничений.
                \en System of constraints. \~
  \param g    - \ru Дескриптор геометрического объекта.
                \en Descriptor of geometric object \~
  \details  \ru После применения этой функции дескриптор объекта становится недействительным. 
  Надо заметить, что удаляемый геометрический объект может все еще участвовать в других 
  объектах и ограничениях. В этом случае удаляемый объект, хотя и считается удаленным,
  фактически продолжает действовать до тех пор, пока другие объекты, связанные с ним, 
  не будут удалены.
                \en After using this function the object descriptor 'g' will be invalidated.
  It should be noted that the removed geometric object can still involved in other
  objects and constraints. In this case, the object to be deleted, although it is considered 
  removed actually remains in effect until other objects connected with will be deleted.
*/
//---
GCE_FUNC(void) GCM_RemoveGeom( GCM_system gSys, GCM_geom g );

//----------------------------------------------------------------------------------------
/** \brief \ru Перенести геометрический объект системы ограничений в подсистему твврдого тела
               (сделать подчиненным объектом жесткого кластера).
           \en Move a geometric object of a constraint system into a solid's subsystem
               (make it a subobject of a rigid cluster). \~
  \param gSys  - \ru Система ограничений.
                 \en System of constraints. \~
  \param g     - \ru Дескриптор геометрического объекта.
                 \en Descriptor of geometric object. \~
  \param tgtCl - \ru Дескриптор целевого кластера.
                 \en Descriptor of target cluster.
  \return \ru Код результата операции.
          \en Resulting code of operation. \~
  \details \ru Для переноса геометрического объекта в глобальную систему
               необходимо в качестве целевого кластера передать дескриптор GCM_NULL.
               В случае успеха функция возвращает код GCM_RESULT_Ok.
               Если геометрический объект имеет дескриптор GCM_GROUND
               или был добавлен функцией #GCM_SubGeom,
               то функция оставляет иерархию системы без изменений
               и возвращает специальный код GCM_RESULT_ImmovableGeom.
               Если целевой кластер был добавлен функцией #GCM_SubGeom,
               то функция оставляет иерархию системы без изменений
               и возвращает специальный код GCM_RESULT_UnacceptableCluster.
               Если целевой кластер вложен в геометрический объект (в том числе совпадает с ним),
               то функция оставляет иерархию системы без изменений
               и возвращает специальный код GCM_RESULT_CyclicMove.
           \en In order to move the geometric object into the global system,
               descriptor GCM_NULL should be passed as the target cluster.
               On success, the function returns code GCM_RESULT_Ok.
               If the geometric object is represented by descriptor GCM_GROUND
               or has been added by function #GCM_SubGeom,
               then the function leaves the system hierarchy unchanged
               and returns special code GCM_RESULT_ImmovableGeom.
               If the target cluster has been added by function #GCM_SubGeom,
               then the function leaves the system hierarchy unchanged
               and returns special code GCM_RESULT_UnacceptableCluster.
               If the target cluster is nested into the geometric object (including when it is the same one),
               then the function leaves the system hierarchy unchanged
               and returns special code GCM_RESULT_CyclicMove.
  \note \ru При удалении кластера функцией #GCM_RemoveGeom
            все перенесенные в него геометрические объекты автоматически переносятся
            в его надкластер, если такой имеется, иначе -- в глобальную систему.
        \en On removal of a cluster by function #GCM_RemoveGeom,
            all the geometric objects moved into it automatically move into
            its supercluster if such one is present, and to the global system otherwise.
*/
//---
GCE_FUNC(GCM_result) GCM_MoveGeom( GCM_system gSys, GCM_geom g, GCM_geom tgtCl );

//----------------------------------------------------------------------------------------
/** \brief \ru Вернет true, если объект все еще действительный.
           \en Returns true if the object is still valid. \~
*/
//---
//GCE_FUNC(bool) GCM_IsValid( GCM_system gSys, GCM_geom g );

/*
  Defining a system of constraints
*/

//----------------------------------------------------------------------------------------
/** \brief \ru Задать бинарное ограничение для пары геометрических объектов.
           \en Set a binary constraint for two geometric objects. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g1   - \ru Дескриптор первого объекта.
                    \en Descriptors of first object. \~
  \param[in] g2   - \ru Дескриптор второго объекта.
                    \en Descriptors of second object. \~
  \param[in] aVal - \ru Опция выравнивания.
                    \en Alignment option. \~
  \param[in] tVar - \ru Вариант касания для ограничения c типом 'GCM_TANGENT'.
                    \en Variant of tangency for constraint of type 'GCM_TANGENT'. \~

  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
  \details 
          \ru Эта функция применяется для задания в системе бинарного ограничения любого 
  типа кроме размерных, а именно ограничения следующих типов: GCM_COINCIDENT, GCM_PARALLEL, 
  GCM_PERPENDICULAR, GCM_TANGENT, GCM_CONCENTRIC, GCM_IN_PLACE. В случае неудавшегося вызова, 
  функция вернет дескриптор пустого объекта GCM_NULL.

          \en The function is used to set a binary constraint of any type except 
  dimensional constraints, namely one of the following types: GCM_COINCIDENT, GCM_PARALLEL,
  GCM_PERPENDICULAR, GCM_TANGENT, GCM_CONCENTRIC, GCM_IN_PLACE. In a case of failure, 
  the function returns a handle to an empty object GCM_NULL. \~
*/
//---
GCM_FUNC(GCM_constraint) GCM_AddBinConstraint(  GCM_system gSys, GCM_c_type cType
                                              , GCM_geom g1, GCM_geom g2, GCM_alignment aVal = GCM_CLOSEST
                                              , GCM_tan_choice tVar = GCM_TAN_POINT );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение, устанавливающее расстояние между парой геометрических объектов.
           \en Set a constraint which specifies distance between a pair of geometric objects. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g1   - \ru Дескриптор первого объекта.
                    \en Descriptors of first object. \~
  \param[in] g2   - \ru Дескриптор второго объекта.
                    \en Descriptors of second object. \~
  \param[in] dVal - \ru Значение размера.
                    \en The value of dimension. \~
  \param[in] aVal - \ru Опция выравнивания.
                    \en Alignment option. \~
  \return \ru Дескриптор нового ограничения c типом GCM_DISTANCE.
          \en Descriptor of the created constraint of type GCM_DISTANCE. \~

  \details  \ru Эта функция создает в системе размерное ограничение с типом GCM_DISTANCE, 
                которое задает линейный размер между двумя геометрическими объектами.
                В случае неудачного вызова, функция вернет дескриптор пустого объекта GCM_NULL.
            \en The function creates a dimensional constraint of type GCM_DISTANCE, which 
                specifies linear dimension between two geometric objects. 
                In a failed call, the function returns a handle to an empty object GCM_NULL.\~

  \note \ru Значение dVal может быть знакопеременным для ориентируемых объектов.
        \en Value of dVal can be positive as well as negative for oriented objects. \~
*/
//---
GCM_FUNC(GCM_constraint) GCM_AddDistance( GCM_system gSys, GCM_geom g1, GCM_geom g2
                                        , double dVal, GCM_alignment aVal = GCM_CLOSEST );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение, устанавливающее допустимый интервал расстояний между парой геометрических объектов.
           \en Set a constraint that specifies the allowed distance interval between a pair of geometric objects. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g1   - \ru Дескриптор первого объекта.
                    \en Descriptors of first object. \~
  \param[in] g2   - \ru Дескриптор второго объекта.
                    \en Descriptors of second object. \~
  \param[in] iVal - \ru Значение допустимого интервала расстояний.
                    \en The value of the allowed distance interval. \~
  \param[in] aVal - \ru Опция выравнивания.
                    \en Alignment option. \~
  \return \ru Дескриптор нового ограничения c типом GCM_DISTANCE.
          \en Descriptor of the created constraint of type GCM_DISTANCE. \~

  \details  \ru Эта функция создает в системе размерное ограничение с типом GCM_DISTANCE,
                которое задает интервальный линейный размер между двумя геометрическими объектами.
                В случае неудачного вызова, функция вернет дескриптор пустого объекта GCM_NULL.
            \en The function creates a dimensional constraint of type GCM_DISTANCE, which
                specifies a linear interval dimension between two geometric objects.
                In a failed call, the function returns a handle to an empty object GCM_NULL.\~

  \note \ru Значение iVal может быть знакопеременным для ориентируемых объектов. Установка опции
            выравнивания подчиняется тем же правилам, что и для управляющего размера.
        \en Value of iVal can be positive as well as negative for oriented objects. Setting the alignment option
            follows the same rules as for driving dimensions.\~
*/
//---
GCM_FUNC(GCM_constraint) GCM_AddDistance( GCM_system gSys, GCM_geom g1, GCM_geom g2,
                                          const GCM_interval & iVal, GCM_alignment aVal = GCM_CLOSEST );

//----------------------------------------------------------------------------------------
/** \brief  \ru Задать ограничение, устанавливающее угол между двумя геометрическими объектами.
            \en Set a constraint which specifies angle between a pair of geometric objects. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g1   - \ru Дескриптор первого объекта.
                    \en Descriptors of first object. \~
  \param[in] g2   - \ru Дескриптор второго объекта.
                    \en Descriptors of second object. \~
  \param[in] axis - \ru Дескриптор объекта, задающего ось вращения угла. Может быть = GCM_NULL.
                    \en Descriptor of an object that specifying the rotation axis of angle. It can be GCM_NULL. \~
  \param[in] dVal - \ru Значение размера (радианы).
                    \en The value of dimension (radians). \~
  \return \ru Дескриптор нового ограничения c типом GCM_ANGLE.
          \en Descriptor of the created constraint of type GCM_ANGLE. \~

  \details \ru Эта функция создает в системе размерное ограничение с типом GCM_ANGLE,
               которое задает угол между направлениями двух геометрических объектов.
               Если ось вращения axis задана (т.е. != GCM_NULL), то угол имеет планарный 
               способ измерения (0 ... 2пи). В этом случае направления 'g1' и 'g2' обязаны 
               лежать в плоскости с нормалью заданной осью axis (оба направления перпендикулярны оси).
               Угол типа плоскость-прямая отмеряется от плоскости вне зависимости от порядка пришедших аргументов.
               В случае неудавшегося вызова, функция вернет дескриптор пустого объекта GCM_NULL.
           \en The function creates a dimensional constraint of type GCM_ANGLE, which 
               specifies angle between the directions of two geometric objects.
               If the rotational axis is specified (i.e. != GCM_NULL), the angle has an 
               planar method of measurement (0 ... 2пи). In this case directions of 
               'g1' and 'g2' must lie on a plane which has a normal specified by 
               the 'axis' parameter (both directions perpendicular to the axis ).
               A plane-line angle is measured from the plane regardless of the order of the arguments.
               In a failed call, the function returns a handle to an empty object GCM_NULL. \~
*/
//---
GCM_FUNC(GCM_constraint) GCM_AddAngle( GCM_system gSys, GCM_geom g1, GCM_geom g2, GCM_geom axis, double dVal );
GCM_FUNC(GCM_constraint) GCM_AddAngle( GCM_system gSys, GCM_geom g1, GCM_geom g2, double dVal );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение, устанавливающее допустимый интервал углов между парой геометрических объектов.
           \en Set a constraint that specifies the allowed angle interval between a pair of geometric objects. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g1   - \ru Дескриптор первого объекта.
                    \en Descriptor of first object. \~
  \param[in] g2   - \ru Дескриптор второго объекта.
                    \en Descriptor of second object. \~
  \param[in] axis - \ru Дескриптор оси вращения (может быть равен GCM_NULL).
                    \en Descriptor of rotation axis (may be equal to GCM_NULL). \~
  \param[in] iVal - \ru Значение допустимого интервала углов.
                    \en The value of the allowed angle interval. \~
  \param[in] aVal - \ru Опция выравнивания.
                    \en Alignment option. \~
  \return \ru Дескриптор нового ограничения c типом GCM_ANGLE.
          \en Descriptor of the created constraint of type GCM_ANGLE. \~

  \details  \ru Эта функция создает в системе размерное ограничение с типом GCM_ANGLE,
                которое задает интервальный угловой размер между двумя геометрическими объектами.
                В случае неудачного вызова, функция вернет дескриптор пустого объекта GCM_NULL.
            \en The function creates a dimensional constraint of type GCM_ANGLE, which
                specifies an angular interval dimension between two geometric objects.
                In a failed call, the function returns a handle to an empty object GCM_NULL.\~

  \note \ru Длина интервала допустимых углов (iVal) не может превышать 2*Пи.
        \en The length of the interval of allowable angles (iVal) cannot exceed 2*pi.\~
*/
//---
GCM_FUNC(GCM_constraint) GCM_AddAngle( GCM_system gSys, GCM_geom g1, GCM_geom g2, GCM_geom axis, GCM_interval iVal );
GCM_FUNC(GCM_constraint) GCM_AddAngle( GCM_system gSys, GCM_geom g1, GCM_geom g2, GCM_interval iVal );

//----------------------------------------------------------------------------------------
/** \brief  \ru Задать ограничение, устанавливающее радиус геометрического объекта.
            \en To create a constraint which specifies a radius of geometric objects. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g1   - \ru Дескриптор геометрического объекта, обладающего ненулевым радиусом.
                    \en Descriptor of the first object which has nonzero radius. \~
  \return \ru Дескриптор нового ограничения c типом GCM_RADIUS.
          \en Descriptor of the created constraint which has a type GCM_RADIUS. \~

  \details \ru Эта функция позволяет задать радиус геометрического объекта. Изменить величину 
               радиуса можно с помощью функции #GCM_ChangeDrivingDimension. Удаляется 
               радиальный размер вызовом функции #GCM_RemoveConstraint.
               В случае неудачного вызова, функция вернет дескриптор пустого объекта GCM_NULL.
           \en This function allows to specify a radius of the geometric object. To change
               radius value use the function #GCM_ChangeDrivingDimension. To remove limitation 
               on radius of the geometric object use the function #GCM_RemoveConstraint.
               In a case of failure, the function returns a handle to an empty object GCM_NULL. \~
*/
//---
GCM_FUNC(GCM_constraint) GCM_FixRadius( GCM_system gSys, GCM_geom g1 );

//----------------------------------------------------------------------------------------
/** \brief  \ru Задать управляющий планарный угол между двумя геометрическими объектами.
            \en Set a driving planar angle between a pair of geometric objects. \~
    \details \ru Функция аналогична вызову GCM_AddAngle, однако требует ось 'axis', 
                 задающую плоскость откладывания угла.
             \en This is the same call GCM_AddAngle, but requires an axis, which defines 
                 a plane in which the angle is measured. \~
*/
//---
GCM_FUNC(GCM_constraint) GCM_AddPlanarAngle( GCM_system gSys, GCM_geom g1, GCM_geom g2
                                           , GCM_geom axis, double dVal );

//----------------------------------------------------------------------------------------
// Not yet documented
//---
GCM_FUNC(GCM_constraint) GCM_AddSymmeric( GCM_system gSys, GCM_geom  g1, GCM_geom g2
                                         , GCM_geom plane, GCM_alignment aVal = GCM_NO_ALIGNMENT );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать линейный паттерн.
           \en Create a linear pattern constraint. \~
    \details \ru   Ограничение "Линейный Паттерн" задаёт закон, согласно которому группа
                 геометрических объектов, добавленных в этот паттерн с помощью функции
                 #GCM_AddGeomToPattern, располагается на заданной прямой. Кроме направляющей
                 прямой, для создания Линейного Паттерна требуется задать геометрический объект,
                 называемый образцом. Этот объект определяет начало координат (нулевую точку)
                 направляющей прямой. Таким образом в системе координат направляющей прямой
                 Линейного Паттерна образец всегда остаётся неподвижным относительно любых
                 трансляций, поворотов и деформаций. Положение любого добавляемого в паттерн
                 объекта (копии) определяется его положением на прямой, направленной вдоль
                 заданной оси, началом координат которой является начало координат ЛСК образца.
              \en  The Linear Pattern constraint defines the law under which geometric objects 
                 added to this pattern using #GCM_AddGeomToPattern function are located on the 
                 given line (guide line). In addition to the guide line to create a Linear 
                 Pattern constraint it's necessary to specify a geometric object called a 
                 Sample. This object defines the starting point of the guide line of the Linear 
                 Pattern. Thus, Sample always remains stationary relative to any translations, 
                 rotations and deformations in the coordinate system of the Linear Pattern guide 
                 line. The position of any object (called a Copy) to be added to the pattern is 
                 determined by its position on the guide line with the origin coinciding with the
                 origin of the LCS of the Sample.\~
    \par      \ru Порядок удаления
                   Чтобы удалить Линейный Паттерн целиком нужно воспользоваться функцией
                 #GCM_RemoveConstraint. При этом не требуется удалять ограничения, созданные при
                 добавлении новых элементов в паттерн с помощью функции #GCM_AddGeomToPattern:
                 они будут удалены автоматически.
              \en Removal procedure
                    To remove the Linear Pattern completely It's necessary to use the 
                  #GCM_RemoveConstraint function. There is no need to remove constraints that were
                  created by the addition of new Copies to the pattern using the function 
                  #GCM_AddGeomToPattern. They will be deleted automatically. \~
    \param[in] gSys   - \ru Система ограничений.
                        \en System of constraints. \~
    \param[in] g1     - \ru Дескриптор образца.
                        \en Descriptor of the sample. \~
    \param[in] g2     - \ru Дескриптор направляющей оси линейного паттерна.
                        \en Descriptor of the direction axis of the Linear Pattern. \~
    \param[in] align  - \ru Опция выравнивания образца относительно направляющей оси. Если задана
                            опция GCM_ALIGN_WITH_AXIAL_GEOM, то образец g1 будет лежать на направлеющей
                            прямой(оси) линейного паттерна.
                        \en Option of alignment of a sample g1 relative to the direction axis. If the
                            option #GCM_ALIGN_WITH_AXIAL_GEOM is given the sample g1 will be coincident
                            with the direction line(axis). \~
    \return \ru Дескриптор нового ограничения c типом GCM_LINEAR_PATTERN.
            \en Descriptor of the created constraint which has a type GCM_LINEAR_PATTERN. \~
*/
// ---
GCM_FUNC(GCM_pattern) GCM_AddLinearPattern( GCM_system gSys, GCM_geom g1, GCM_geom g2, GCM_alignment align=GCM_NO_ALIGNMENT );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать угловой паттерн.
           \en Create an angular pattern constraint. \~
    \details \ru   Ограничение "Угловой Паттерн" задаёт закон, согласно которому группа 
                 геометрических объектов, добавленных в этот паттерн с помощью функции 
                 #GCM_AddGeomToPattern, располагается на некоторой окружности. Окружность эта 
                 лежит в плоскости перпендикулярной заданной оси, а центр окружности лежит на 
                 этой оси. Кроме оси вращения для создания Углового Паттерна требуется задать 
                 геометрический объект, называемый образцом. Этот объект определяет нулевой угол 
                 и начальный радиус окружности. Таким образом положение любого добавляемого в 
                 паттерн объекта (копии) определяется вращением вокруг заданной оси, начиная от 
                 образца. При этом радиус окружности (расстояние от копии или образца до оси) не 
                 не является константой и может варьироваться (изменяться) в ходе решения. 
              \en   The Angular Pattern constraint defines the law under which geometric objects 
                  added to this pattern using #GCM_AddGeomToPattern function are located on the 
                  given circle. This circle lies in a plane that is perpendicular to the given 
                  axis, and the center of this circle lies on this axis. In addition to the axis 
                  to create an Angular Pattern constraint it's necessary to specify a geometric 
                  object called a Sample. This object defines the zero angle and the initial 
                  radius of the circle for the Angular Pattern. The position of any object (called 
                  a Copy) to be added to the pattern is determined by the rotation around the 
                  given axis, starting from the Sample. The radius of the circle (the distance 
                  from the Copy or the Sample to the axis) is not constant and can vary in the 
                  process of solving the system of equations. \~

   \par      \ru Порядок удаления
                   Чтобы удалить Угловой Паттерн целиком нужно воспользоваться функцией 
                 #GCM_RemoveConstraint. При этом не требуется удалять ограничения, созданные при 
                 добавлении новых элементов в паттерн с помощью #GCM_AddGeomToPattern: они будут 
                 удалены автоматически.
             \en Removal procedure
                    To remove the Angular Pattern completely It's necessary to use the 
                  #GCM_RemoveConstraint function. There is no need to remove constraints that were
                  created by the addition of new Copies to the pattern using the function 
                  #GCM_AddGeomToPattern. They will be deleted automatically. \~

    \param[in] gSys   - \ru Система ограничений.
                        \en System of constraints. \~
    \param[in] smp    - \ru Дескриптор образца.
                        \en Descriptor of the sample. \~
    \param[in] axial  - \ru Дескриптор оси вращения углового паттерна.
                        \en Descriptor of the rotation axis of the Angular Pattern. \~
    \param[in] align  - \ru Опция выравнивания образца относительно направляющей оси. Если задана
                            опция GCM_ALIGN_WITH_AXIAL_GEOM, то образец 'smp' будет лежать в плоскости
                            XY направляющего обекта (оси вращения), и если направляющий объект имеет
                            радиус (например, это окружность), то расстояние от объектов Углового
                            Паттерна до оси вращения будет равно радиусу направляющего объекта
                            (например, радиусу окружности).
                        \en Option of alignment of a sample relative to the direction axis.
                            If the GCM_ALIGN_WITH_AXIAL_GEOM option is specified sample g1 will lie in
                            the XY plane of the direction axis object (rotation axis) and if the
                            direction axis object has a radius (for example, this is a circle) the
                            distance from the Angle Pattern objects to the rotation axis will be equal
                            to the radius of the direction axis object (for example, radius of a circle). \~
    \return \ru Дескриптор нового ограничения c типом GCM_ANGULAR_PATTERN.
            \en Descriptor of the created constraint which has a type GCM_ANGULAR_PATTERN. \~
*/
// ---
GCM_FUNC(GCM_pattern) GCM_AddAngularPattern( GCM_system gSys, GCM_geom smp, GCM_geom axial, GCM_alignment align=GCM_NO_ALIGNMENT );

//----------------------------------------------------------------------------------------
/**  brief \ru Задать паттерн - Система Координат.
           \en Create the constraint the coordinate system pattern. \~
    \brief \ru Задать координату паттерна - Системы Координат.
           \en Create an coordinate of the coordiёnate system pattern constraint. \~
    \details \ru   Ограничение "Паттерн - Система координат" (СК-паттерн) задаёт закон, согласно которому
                 группа геометрических объектов, добавленных в этот паттерн с помощью функции
                 #GCM_AddGeomToPattern, располагается в пространстве.

                 ... Для создания СК-паттерна требуется задать геометрический объект, называемый образцом.
                 Этот объект определяет нулевой угол и начальный радиус окружности. Таким образом положение любого добавляемого в
                 паттерн объекта (копии) отсчитывается от образца, а не от начала системы координат паттерна. От положения начала
                 системы координат паттерна отсчитывается положение самого образца.
              \en  \~

    \param[in] gSys   - \ru Система ограничений.
                        \en System of constraints. \~
    \param[in] sample - \ru Дескриптор образца.
                        \en Descriptor of the sample. \~
    \param[in] origin - \ru Дескриптор начала координат паттерна, относительно которого определяется
                            положение образца (sample) в пространстве паттерна.
                        \en Descriptor of the origin of the Coordinate System Pattern. It is used
                            to specify the position of the sample in the pattern space. \~
    \return \ru Дескриптор нового ограничения c типом GCM_CS_PATTERN.
            \en Descriptor of the created constraint which has a type GCM_CS_PATTERN. \~
*/
// ---
GCM_FUNC(GCM_pattern) GCM_AddCoordSystemPattern( GCM_system gSys, GCM_geom sample, GCM_geom origin );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать координату паттерна - Система Координат.
           \en Create the constraint coordinate of the coordinate system pattern. \~
    \details \ru ...
              \en ... \~

    \param[in] gSys   - \ru Система ограничений.
                        \en System of constraints. \~
    \param[in] sample - \ru Дескриптор СК-паттерна.
                        \en Descriptor of the Coordinate System pattern. \~
    \param[in] crd    - \ru Имя задаваемой координаты СК-паттерна.
                        \en Name of the adding coordinate of the Coordinate System pattern. \~
    \return \ru Дескриптор нового ограничения c типом GCM_PATTERN_COORDINATE.
            \en Descriptor of the created constraint which has a type GCM_PATTERN_COORDINATE. \~
*/
// ---
GCM_FUNC(GCM_pattern) GCM_SubPattern( GCM_system gSys, GCM_pattern csPattern, GCM_coord_name crd, GCM_geom controlObj );

//----------------------------------------------------------------------------------------
/** \brief  \ru Добавить геометрический объект в паттерн.
            \en Add geometric object to the pattern. \~
    \details \ru   Объект, добавляемый в паттерн, назовём копией.
                   Если копия добавляется в Линейный Паттерн, то требуется указать расстояние от
                 копии до образца. Оно может быть положительным или отрицательным и определяется
                 требуемым положением копии относительно образца с учётом направляющей оси. Так
                 же можно опционально задать выравнивание ЛСК копии относительно ЛСК образца.
                   Если копия добавляется в Угловой Паттерн, то требуется указать угол поворота
                 копии относительно образца, вокруг оси вращения паттерна. Так же можно
                 опционально задать выравнивание копии относительно образца. Возможны 2 типа
                 выравнивания: GCM_ALIGNED - выравнивание ЛСК копии и образца и GCM_ROTATED -
                 выравнивание ЛСК копии с ЛСК образца, повёрнутого вокруг оси вращения на тот же
                 угол, что и копия.
                   Расстояние (или угол поворота) от образца до копии по умолчанию фиксировано,
                 но может быть варьируемым при задании соответствующей опции #GCM_scale.
             \en   Let's call a Copy the object that is added to the pattern.
                   If the Copy is added to the Linear Pattern it's necessary to specify the
                 distance from the Copy to the Sample. It can be positive or negative and is
                 determined by the required position of the Copy relative to the Sample taking
                 into account the guide axis. It's optionally possible to specify alignment of
                 the Copy LCS relative to the Sample LCS.
                   If the Copy is added to the Angular Pattern it's necessary to specify the
                 angle of rotation of the Copy relative to the Sample around the pattern rotation
                 axis. It's optionally possible to specify alignment of the Copy relative to the
                 Sample. There are 2 types of alignment: GCM_ALIGNED - alignment of the local
                 coordinate systems of the Copy and the Sample, GCM_ROTATED - the alignment of
                 the local coordinate system of the Copy with the local coordinate system of the
                 Sample that is rotated around the axis of rotation at the same angle as the Copy.
                   The distance (or angle of rotation) from the sample to the copy is fixed by default,
                 but can be varied by specifying the appropriate #GCM_scale option.\~

    \par     \ru Порядок удаления.
                 Чтобы удалить копию из паттерна используйте функцию #GCM_RemoveConstraint. Если 
                 же вам надо удалить паттерн целиком, то вам не требуется удалять каждую копию из 
                 паттерна, просто удалите паттерн.
             \en Removal procedure 
                 To remove a Copy from the pattern use the function #GCM_RemoveConstraint. If 
                 it's necessary to remove the pattern completely there is no need to remove each
                 copy from the pattern. Just remove the pattern constraint. \~

    \param[in] gSys - \ru Система ограничений.
                      \en System of constraints. \~
    \param[in] ptrn - \ru Дескриптор паттерна, в который добавляем копию.
                      \en Descriptor of the pattern. \~
    \param[in] geom - \ru Дескриптор добавляемого геометрического объекта (копии).
                    \en Descriptor of the copy. \~
    \param[in] position - \ru Переменная, задающая положение добавляемой копии в паттерне (расстояние или угол).
                          \en Variable that specifies the position of the copy in the pattern (distance or angle). \~
    \param[in] align - \ru Опция, задающая выравнивание копии по отношению к образцу.
                      \en  Option that specifies the alignment of copy relative to the sample. \~
    \param[in] scale - \ru Тип масштабирования элемента паттерна.
                       \en Scaling type of pattern element. \~
    \return \ru Дескриптор нового ограничения c типом GCM_PATTERNED.
            \en Descriptor of the created constraint which has a type GCM_PATTERNED. \~
*/
// ---
GCM_FUNC(GCM_constraint) GCM_AddGeomToPattern( GCM_system gSys, GCM_pattern ptrn, GCM_geom geom, double position, 
                                               GCM_alignment align = GCM_NO_ALIGNMENT, GCM_scale scale = GCM_RIGID );

//----------------------------------------------------------------------------------------
/** \brief \ru Задать ограничение.
           \en Set a constraint. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] cRec - \ru Унифицированная запись ограничения.
                    \en Uniform record of a constraint. \~
  \return \ru Дескриптор нового ограничения.
          \en Descriptor of a new constraint. \~
  \details 
  \ru Эта функция применяется только для автоматического тестирования решателя,
      поэтому подробно не документировалась.
  \en This function is used only for the automated testing of the solver therefore not documented.  
*/
//---
GCM_FUNC(GCM_constraint) GCM_AddConstraint(GCM_system gSys, const GCM_c_record& cRec);

//----------------------------------------------------------------------------------------
/** \brief \ru Выдать зависимый объект данного ограничения. \ru Get a dependent object of a constraint.
    \details  \ru Зависимый объект определен естественным образом для ограничения типа `GCM_DEPENDENT` 
                  или назначается с помощью вызова #GCM_SetDependent.
              \en The dependent object is defined naturally for the `GCM_DEPENDENT` constraint 
                  or is assigned using a #GCM_SetDependent call.
*/
//---
GCM_FUNC(GCM_geom) GCM_DependentGeom(GCM_system gSys, GCM_constraint con);

//----------------------------------------------------------------------------------------
/// \ru Задать для данного ограничения зависимый объект. \ru Set a dependent object of constraint.
//---
GCM_FUNC(GCM_geom) GCM_SetDependent(GCM_system gSys, GCM_constraint con, GCM_geom g);

//----------------------------------------------------------------------------------------
/** \brief \ru Удалить ограничение из системы.
           \en Delete a constraint from the system. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] con  - \ru Дескриптор ограничения.
                    \en Descriptor of constraint. \~
*/
//---
GCE_FUNC(void) GCM_RemoveConstraint( GCM_system gSys, GCM_constraint con );


/*
  Fixation and freeing of a geometry
*/

//----------------------------------------------------------------------------------------
// Create fixing constraint of the geom
//---
GCM_FUNC(GCM_constraint) GCM_FixGeom_( GCM_system gSys, GCM_geom g );

//----------------------------------------------------------------------------------------
/** \brief  \ru Сделать геометрический объект неподвижным.
            \en Set a geometric object fixed. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \param[in] g    - \ru Дескриптор геометрического объекта.
                    \en Descriptors of geometric object. \~

  \details 
  \ru Эта функция делает объект неподвижным лишая его всех степеней свобод. Если геометрический
      объект является суб-объектом тела (кластера), то объект замораживается только в рамках кластера,
      однако в глобальной системе координат объект имеет такую же свободу как и кластер, 
      которому он принадлежит.
  \en The function makes the object fixed depriving it of all degrees of freedom. If the geometric object 
      is a sub geom of a solid (cluster), the object is frozen only in the framework of the cluster, 
      but in the global coordinate system the object has the same freedom as the cluster 
      to which it belongs.
  \~
  \note    \ru На будущее планируется, что данная функция будет возвращать дескриптор ограничения.
           \en In the future this function will be returning a descriptor of constraint, i.e. will create a fixing constraint. \~
  \sa GCM_FreeGeom
*/
//---
GCM_FUNC(bool) GCM_FreezeGeom( GCM_system gSys, GCM_geom g );

//----------------------------------------------------------------------------------------
/** \brief  \ru Освободить объект, зафиксированный методом GCM_FreezeGeom.
            \en Set free geometric object fixed by GCM_FreezeGeom call. \~
  \sa GCM_FreezeGeom
*/
//---
GCM_FUNC(void) GCM_FreeGeom( GCM_system gSys, GCM_geom g );

/*
  Evaluating methods
*/

//----------------------------------------------------------------------------------------
/** \brief \ru Вычислить систему ограничений.
           \en Calculate the constraint system. \~
  \param[in] gSys - \ru Система ограничений.
                    \en System of constraints. \~
  \return \ru Код результата вычислений.
          \en Calculation result code. \~
  \details \ru Функция решает задачу ограничений. Задача ограничений формулируется 
  функциями API геометрического решателя; функции вида GCM_Add_XXXXXXX добавляют новые 
  объекты, функции вида GCM_Change_XXXXXXX, GCM_Set_XXXXXXX изменяют состояние объектов. 
  Таким образом, что бы все такие изменения вступили в силу, нужно вызвать 
  метод #GCM_Evaluate.\n
    Алгоритмы GCM_Evaluate учитывают удовлетворенность систем ограничений; если
  все ограничения уже решены, то функция не тратит время на вычисления, а
  состояние геометрических объектов остается неизменным.
           \en The function solves problem of constraints. The problem of constraint is 
  formulated by API functions of geometric solver; the functions of a kind GCM_Add_XXXXXXX 
  add a new object, the functions of kinds GCM_Change_XXXXXXX and GCM_Set_XXXXXXX change 
  a state of objects. Thus, for all changes to take effect it is necessary to call the
  method #GCM_Evaluate.\n
    The algorithms GCM_Evaluate take into account whether constraint systems are satisfied, 
  if all constraints have been already solved, then the function does not spend time 
  for calculations, and the state of geometric objects remains unchanged. \~
*/
//---
GCM_FUNC(GCM_result) GCM_Evaluate( GCM_system gSys );

//----------------------------------------------------------------------------------------
/** \brief \ru Получить код результата вычисления ограничения.
           \en Get result code of the evaluation of constraint. \~
  \param[in] gSys   - \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] cItem  - \ru Дескриптор ограничения, принадлежащего системе gSys.
                      \en Descriptor of constraint belonging to the system gSys. \~
  \note   \ru Если система еще не вычислялась, то функция вернет код GCM_RESULT_None.
          \en If the system has not yet been evaluated then the function will return 
              the code GCM_RESULT_None. \~
  \return \ru Диагностический код хранящийся в системе после последней вызова GCM_Evaluate.
          \en Diagnostic code stored in the system after the last call GCM_Evaluate. \~
*/
//---
GCM_FUNC(GCM_result) GCM_EvaluationResult( GCM_system gSys, GCM_constraint cItem );

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
GCM_FUNC(bool) GCM_IsSatisfied( GCM_system gSys, GCM_constraint cItem );

//----------------------------------------------------------------------------------------
/** \brief  \ru Является ли геометрический объект полностью определенным или недоопределенным.
            \en Whether the geometric entity is fully defined or underdefined.
  \return \ru Вернет значение GCM_DOF_RESULT_WellDefined, если установлено, что 
              геометрический объект принадлежит полностью-определенной части модели.
          \en Returns GCM_DOF_RESULT_WellDefined, if the solver determines a geometric entity 
              belongs to a well-defined part of the model. \~
*/
// ---
GCM_FUNC(GCM_dof_result) GCM_IsWellDefined( GCM_system gSys, GCM_geom g );

//----------------------------------------------------------------------------------------
/** \brief  \ru Выдать текущее положение (решение) геометрического объекта в мировой СК.
            \en Get current placement (solution) of the geometric object in the world coordinate system.
*/
//---
GCM_FUNC(MbPlacement3D) GCM_Placement( GCM_system gSys, GCM_geom g );

//----------------------------------------------------------------------------------------
/** \brief  \ru Выдать начало СК геометрического объекта.
            \en Get an CS origin of the geometric object.
    \details  
      \ru Функция вернет координаты начала ЛСК объекта. Возвращаемая точка задается
    в мировой системе координат. Данный вызов может быть использован для любых типов геометрии. 
    Например, 
    для окружности данный вызов вернет ее центр, 
    для плоскости - точку, лежащую на плоскости, 
    для цилиндра - центр основания цилиндра и т.д.

      \en The function returns coordinates of the origin of the LCS. The returned point 
    is specified in the world coordinate system. The call can be applied to any type of geometry. 
    For example, 
    for a circle the call will return its center, 
    for a plane it is a point laying on the plane,
    for a cylinder it is a center of its foundation circle and so on.
*/
//---
GCM_FUNC(MbCartPoint3D) GCM_Origin( GCM_system gSys, GCM_geom g );

//----------------------------------------------------------------------------------------
/** \brief  \ru Текущее значение радиуса геометрического объекта.
            \en Current radius value of the geometric object.
*/
//---
GCM_FUNC(double) GCM_Radius( GCM_system gSys, GCM_geom g );

//----------------------------------------------------------------------------------------
/** \brief  \ru Текущее значение "большого" радиуса тора или конуса.
            \en Current "major" radius value of torus or cone.
*/
//---
GCM_FUNC(double) GCM_RadiusA( GCM_system gSys, GCM_geom g );

//----------------------------------------------------------------------------------------
/** \brief  \ru Текущее значение "малого" радиуса тора или конуса.
            \en Current "minor" radius value of torus or cone.
*/
//---
GCM_FUNC(double) GCM_RadiusB( GCM_system gSys, GCM_geom g );

/*
  Changing methods
*/

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
  \details \ru Функция применяется только для управляющих размеров. Если управляющий размер 
  является угловым, то параметр dVal задается в радианах.\n
  Следует учитывать, что настоящая функция не осуществляет вычислений, а только подготавливает 
  изменение размера. Что бы изменения вступили в силу, необходимо вызвать функцию #GCE_Evaluate.
           \en The function is used only for driving dimensions. If the driving dimension 
  is angular, then the parameter dVal is specified in radians. \n
  It should be noted that the function doesn't perform computations but only prepares 
  the changing of dimension. For the changes to take effect it is required to call 
  the function #GCM_Evaluate. \~
*/
//---
GCM_FUNC(GCM_result) GCM_ChangeDrivingDimension( GCM_system gSys, GCM_constraint dItem, double dVal );

//----------------------------------------------------------------------------------------
/** \brief  \ru Задать текущее положение геометрического объекта.
            \en Set current placement of the geometric object.
    \note   \ru Эта функция только придает объекту новое состояние без переоценки системы 
                ограничений. Вызов GCM_Evaluate может поменять заданное состояние, если 
                имеются не удовлетворенные ограничения.
            \en The function only impart new state of the object without the revaluation 
                of constraints. Call GCM_Evaluate can change the given state to satisfy 
                constraints of this object. \~
*/
//---
GCM_FUNC(void) GCM_SetPlacement( GCM_system gSys, GCM_geom g, const MbPlacement3D & place );


/*
  Dragging functions
*/

//----------------------------------------------------------------------------------------
/**
    \brief  \ru Инициализировать режим перетаскивания объектов в плоскости экрана.
            \en Initialize mode of object moving in the screen plane.
    \param[in] gSys      - \ru Система ограничений.
                           \en System of constraints. \~
    \param[in] movGeom   - \ru Компонент, деталь, которой манипулируют.
                           \en Component, part which is manipulated. \~
    \param[in] projPlane - \ru Плоскость экрана, заданная в ГСК сборки.
                           \en Plane of the screen given in the WCS of assembly. \~
    \param[in] curPnt    - \ru Точка, принадлежащая компоненту, которая проецируется на плоскость
                               экрана в положение курсора, и за которую осуществляется 'перетаскивание'.
                               curPnt задана в ЛСК геом.объекта movGeom.
                           \en Point of the component which is projected onto plane of the screen to
                               cursor position and is 'dragging'. curPnt given in the LCS of
                               the geometric object movGeom; \~
    \return \ru Код результата. \en Result code. \~

    \details
      \ru Функция запускается однократно перед входом в режим перетаскивания компонент, который управляется
          (по движению мыши) через команду #GCM_SolveReposition(GCM_system, const MbCartPoint3D &). Режим
          прекращается вызовом любой иной команды, кроме #GCM_PrepareReposition. Также есть специальная
          функция для выхода из режима "перетаскивания" - #GCM_FinishReposition, для явного сбрасывания
          режима перемещения.
      \en The function runs once to start the dragging mode of components, which is controlled
          (by movement of the mouse) by the command #GCM_SolveReposition(GCM_system, const MbCartPoint3D &).
          Mode is stopped by the calling any other command except #GCM_PrepareReposition. There is also
          the special function to exit from the dragging mode explicitly - #GCM_FinishReposition. \~
*/
//---
GCM_FUNC(GCM_result) GCM_PrepareReposition( GCM_system gSys, GCM_geom movGeom,
                                            const MbPlacement3D & projPlane, const MbCartPoint3D & curPnt );

/** \brief  \ru Инициализировать режим вращения компонента вокруг фиксированной оси.
            \en To initialize the rotation mode of the component around a fixed axis.
*/
GCM_FUNC(GCM_result) GCM_PrepareReposition( GCM_system gSys, GCM_geom rotGeom, const MbCartPoint3D & org, const MbVector3D & axis );

/// \ru Завершить режим "перетаскивания". \en Finish the dragging mode.
GCM_FUNC(void) GCM_FinishReposition( GCM_system gSys );

/** \brief  \ru Выдать объект манипуляции, с которым работает решатель, находясь в режиме вращения/перемещения объектом (драггинг).
            \en Get manipulation object with which the Solver works when being in the dragging mode (rotating or moving).
*/
GCM_FUNC(GCM_geom) GCM_GetMovingGeom( GCM_system gSys );

/**
    \brief  \ru Решить систему для произвольного изменения положения одного тела.
            \en Solve the system for an arbitrary change of position of one solid. \~
    \param[in] gSys    - \ru Система ограничений.
                         \en System of constraints. \~
    \param[in] g       - \ru Тело, положение которого меняется.
                         \en Solid, the position of which is changed. \~
    \param[in] newPos  - \ru Новое пололожение тела.
                         \en New position of a solid. \~
    \param[in] movType - \ru Код желаемого поведения
                         \en Code of the desired behavior \~
    \return \ru Код результата. \en Result code. \~

    \note \ru Эта функция не позволяет вывести систему сопряжений из состояния решаемости,
              кроме случаев, когда до вызова функции система уже находилась в нерешенном
    состоянии. Если новое положение 'newPos' не позволяет удовлетворять системе сопряжений,
    то новое положение тела окажется наиболее близким к newPos (при сохранении решаемости).
          \en This function doesn't allow to take out constraint system from decided state,
    except when before call of function the system was already unsolved. If new position
    'newPos' doesn't allow to satisfy the system of constraints, then new position of solid
    will be the most nearest to newPos (while preserving solvability). \~
*/
GCM_FUNC(GCM_result) GCM_SolveReposition( GCM_system gSys, GCM_geom g,
                                          const MbPlacement3D & newPos, GCM_reposition movType );

//----------------------------------------------------------------------------------------
/**
    \brief  \ru Решить систему для произвольного изменения положения тела в заданной плоскости.
            \en Solve the system for an arbitrary change of position of one solid in a given plane.
    \param[in] gSys      - \ru Система ограничений.
                           \en System of constraints. \~
    \param[in] movGeom   - \ru Компонент, деталь, которой манипулируют.
                           \en Component, part which is manipulated. \~
    \param[in] projPlane - \ru Нормаль к плоскости, в которой происходит перетаскивание, заданная в ГСК сборки.
                           \en The normal to the plane in which the dragging occurs, specified in the assembly WCS. \~
    \param[in] cursor    - \ru Текущее положение курсора в ГСК..
                           \en Current position of a cursor in the WCS. \~
    \return \ru Код результата. \en Result code. \~

    \details
      \ru Процедура, управляющая режимом перетаскивания. При использовании данной функции не требуется предварительно
          делать соответствующий ей вызов GCM_PrepareReposition, а так же не требуется вызывать GCM_FinishReposition
          по окончании её использования.
      \en The procedure that controls the drag and drop mode. When using this function, there is no need to make
          a preliminary invoke of GCM_PrepareReposition, and there is no need to invoke GCM_FinishReposition
          after using it. \~
*/
//---
GCM_FUNC(GCM_result) GCM_SolveReposition( GCM_system gSys, GCM_geom movGeom, const MbCartPoint3D & cursor,
                                          const MbVector3D & projPlane );

//----------------------------------------------------------------------------------------
/**
    \brief  \ru Перетащить геометрический объект с помощью манипулятора.
            \en Drag a geometric object by a manipulator. \~
    \param[in] gSys  - \ru Система ограничений.
                       \en System of constraints. \~
    \param[in] g     - \ru Перетаскиваемый геометрический объект.
                       \en Geometric object subject to dragging. \~
    \param[in] manip - \ru Изначальное или целевое ГСК-положение манипулятора.
                       \en Initial or target WCS-placement of manipulator. \~
    \param[in] mode  - \ru Режим перетаскивания.
                       \en Dragging mode. \~
    \return  \ru Код результата.
             \en Result code. \~

    \details  \ru Процедура предназначена для серийных вызовов, динамически перетаскивающих геометрический объект.
                  Манипулятор связывается с объектом при первом вызове и приводит его в движение при последующих.
              \en The procedure is intended for serial calls which drag the geometric object in a dynamic manner.
                  The manipulator is bound to the object on the first call and drives it into motion on subsequent ones. \~

    \note  \ru Функционал в разработке.
               Текущая реализация поддерживает все представленные варианты влияния (#GCM_drag_influence)
               и аннотированные ниже значения управления (#GCM_drag_control) в зависимости от опции движения (#GCM_drag_motion).
           \en The feauture is under development.
               The current implementation supports all the presented influence variants (#GCM_drag_influence)
               and the below-noted control values in dependence on the motion option (#GCM_drag_motion). \~

    \note  #GCM_DRAGGING_Translation:   #GCM_DRAGGING_Adaptive, #GCM_DRAGGING_Specific \n
           #GCM_DRAGGING_TranslationXY: #GCM_DRAGGING_Adaptive

    \note  \ru Во избежание неожиданного поведения целевое положение манипулятора между последовательными вызовами
               рекомендуется изменять соответственно заказанному виду движения.
           \en To avoid unexpected behavior, it is recommended to change the target placement of the manipulator
               between successive calls in accordance with the ordered kind of motion. \~
*/
//---
GCM_FUNC(GCM_result) GCM_DynamicReposition( GCM_system gSys, GCM_geom g,
                                            const MbPlacement3D & manip, const GCM_dragging & mode );

/**
    \brief  \ru Решить систему сопряжений для новой позиции курсора в режиме драггинга.
            \en Solve the system of constraints for new position of cursor in the dragging mode.
    \param[in] gSys   - \ru Система ограничений.
                        \en System of constraints. \~
    \param[in] curPos - \ru Текущее положение курсора в ГСК.
                        \en Current position of a cursor in the WCS. \~
    \return \ru Код результата. \en Result code. \~

    \details  \ru Процедура, управляющая режимом перетаскивания, который прекращается вызовом  любой иной команды.
              \en Procedure that controls dragging mode which are stopped after calling any other command. \~
*/
GCM_FUNC(GCM_result) GCM_SolveReposition( GCM_system gSys, const MbCartPoint3D & curPos );

/**
    \brief \ru Решить систему в режиме драггинга с одно-параметрическим управлением.
           \en Solve the system under one-parametric driving in the dragging mode. \~
    \param[in] gSys  - \ru Система ограничений.
                       \en System of constraints. \~
    \param[in] alpha - \ru Управляющий параметр (зачастую задается в радианах).
                       \en Driving parameter (this is ussualy an angle given in radians). \~
    \return \ru Код результата. \en Result code. \~

    \details  \ru Это функция, управляющая режимом динамического перепозиционирования 
  (см. #GCM_PrepareReposition), в котором положение тела управляется изменением одной 
  координаты, например, угла вращения вокруг оси. Режим прекращается вызовом 
  #GCM_FinishReposition или любой иной командой, меняющей состояние решетеля, например, 
  #GCM_AddConstraint. 
              \en This function controls dynamic reposition mode (see #GCM_PrepareReposition), 
  in which the position of the solid is driven by changing one coordinate. For example 
  the angle of rotation around an axis. Mode is stopped by calling #GCM_FinishReposition 
  or any other command, which is changes state of the Solver, for example #GCM_AddConstraint.
              \~    
*/
GCM_FUNC(GCM_result) GCM_SolveReposition( GCM_system gSys, double alpha );

/*
  Journaling functions
*/



//----------------------------------------------------------------------------------------
/**
  \brief  \ru Включить журналирование и назначить файл для записи журнала вызовов API.
          \en Switch on the journaling and specify the file for recording a journal of GCE API calls. \~
  \param[in] gSys -   \ru Система ограничений.
                      \en System of constraints. \~
  \param[in] fName -  \ru Имя файла назначения с полным путем.
                      \en Name of destination file with a full path. \~
  \return true, if journaling has been successfully switched on.

  \attention  
          \ru Файл журнала будет записан только после завершения сеанса работы с системой
              ограничений, а именно сразу после вызова GCM_RemoveSystem.
          \en The journal file will be written only when a session of work with the
              constraint system is finished, i.e. immediately after calling the
              GCM_RemoveSystem method.
          \ru Добавление записей в журнал из параллельного кода не происходит.
          \en Adding records to the journal from parallel code does not occur.
*/
//---
GCM_FUNC(GCM_journal_result) GCM_SetJournal( GCM_system gSys, const char * fName );

//----------------------------------------------------------------------------------------
/** \brief  \ru Выдать журнал записывающий вызовы Solver API, а также другие события, имеющие отношение к взаимодействием с солвером.
            \en Get a logger that recording API calls, as well as other events related to interaction with the solver. \~
  \note \ru Вызов используется пока только для тестовых целей. Может быть изменен или удален из API в следующих ревизиях.
        \en The call is used for testing purposes only. It may be changed or removed in future revisions. \~
*/
//---
GCM_FUNC(GCM_journal) GCM_SubscribeJournal( GCM_system gSys, GCM_log_func logFunc, GCM_extra_param extParam );

/** \} */ // GCM_3D_API

struct GCT_diagnostic_pars;

//----------------------------------------------------------------------------------------
/*
  It's used for testing purposes only.
*/
//---
GCM_FUNC(GCT_diagnostic_pars) GCM_DiagnosticPars( GCM_system gSys );

//----------------------------------------------------------------------------------------
// Deprecated (2021). Use GCM_GeometryStatus to query about GCM_G_STATUS_Frozen.
//---
GCM_FUNC(bool) GCM_IsFixed( GCM_system gSys, GCM_geom g );

//----------------------------------------------------------------------------------------
// Статус геометрического объекта в системе.
/**
  \note Internal use only. The func is under development.
*/
// ---
GCM_FUNC(GCM_g_status) GCM_GeometryStatus( GCM_system gSys, GCM_geom g );

#endif // __GCM_API_H

// eof
