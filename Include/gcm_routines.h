//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru API процедур и функций 3D решателя.
         \en API of procedures and functions of 3D solver. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GCM_ROUTINES_H
#define __GCM_ROUTINES_H

#include <math_define.h>
//
#include "gcm_constraint.h"
#include "gcm_manager.h"
#include "gcm_types.h"

class  MbProperties;
struct           CNodeIterator;

/**
  \addtogroup GCM_3D_Routines
  \{
*/

//----------------------------------------------------------------------------------------
/** \brief \ru Запись о степени свободы перемещения.
           \en Record about the degree of freedom of movement. \~
  \details \ru Структура для записи информации о степени свободы линейного перемещения 
  для трехмерной точки.
           \en Structure for record the information about the degree of freedom of a linear displacement 
  for a three-dimensional point. \~
  \par \ru Как интерпретировать данные.
  Если dof = 0, то точка неподвижна;\n
  Если dof = 1, точка имеет свободу перемещения вдоль прямой с направление MtTransDof::vector;\n
  Если dof = 2, точка имеет свободу перемещения вдоль плоскости с нормалью MtTransDof::vector;\n
  Если dof = 3, точка имеет полную свободу перемещения, значение MtTransDof::vector не имеет значения.\n
       \en How to interpret data.
  If dof = 0, then the point is fixed;\n
  If dof = 1, then the point has a freedom of movement along a curve with the direction MtTransDof::vector;\n
  If dof = 2, then the point has a freedom of movement along the plane with the normal MtTransDof::vector;\n
  If dof = 3, then the point has a complete freedom of movement, a value of MtTransDof::vector is not significant.\n \~
*/
//---
struct GCM_CLASS MtTransDof
{
  int         dof;    ///< \ru Степень свободы. \en Degree of freedom. 
  MbVector3D  vector; ///< \ru Направление прямой или нормаль плоскости, в зависимости от значения dof. \en Direction of line or normal of plane depending on the value of dof. 
  MtTransDof() : dof(0), vector() {}

public:
  bool operator == ( const MtTransDof & tDof ) const
  {
    if ( dof == tDof.dof )
    {     
      return ( dof == 0 || dof == 3 ) && (vector == tDof.vector);
    }
    return false;
  }
};

//----------------------------------------------------------------------------------------
/** \brief \ru Параметры близости к решению для геометрического ограничения.
           \en Parameters of proximity to solution for geometric constraint. \~
  \details \ru Эта структура данных применяется только, как результат работы 
            функции #GetClosestParameters.
           \en This data structure is used only as result 
            of the function  #GetClosestParameters. \~
*/
//---
struct GCM_CLASS MtMateParameters
{
  double        myDimVal;       ///< \ru Числовой параметр размерного сопряжения. \en The numerical parameter of dimensional mate. 
  GCM_alignment myAlignVal;     ///< \ru Условие выравнивания сопрягаемой геометрии. \en The alignment condition of mating geometry. 
  bool          myDimEvaluated; ///< \ru Означает, что параметр myDimVal оценен. \en Means that the parameter myDimVal is evaluated. 
  bool          myDimSigned;    ///< \ru Означает, что сопряжение чувствительно к знаку myDimVal. \en Means that the mate is sensitive to the sign of myDimVal. 

public:
  MtMateParameters()
    : myDimVal( 0.0 )
    , myAlignVal( GCM_NO_ALIGNMENT )
    , myDimEvaluated( false )
    , myDimSigned( false )
  {}
};

//----------------------------------------------------------------------------------------
/** \brief \ru Характеристические данные линейного размера.
           \en Characteristic data of linear dimension. \~
  \details \ru Структура данных содержит характерную информацию, необходимую, 
  для визуализации линейного размера, а именно:\n
    1) Пару точек, заданных в общей СК модели, сооствественно лежащих на первом 
  и втором геометрических объектах сопряжения;\n
    2) Пара перечислителей #MtGeometryType, означающих типы сопрягаемой геометрии, 
  которым принадлежат точки;\n
    3) Степень свободы перемещения пары точек, в рамках которой сохраняется 1-е 
  условие (принадлежность геометрическим объектам, для которых задан размер);\n
    4) Значение размера - величина по сути равная расстоянию между точками.\n
           \en The data structure contains characteristic information needed 
  for rendering a linear dimension, namely:\n
    1) A pair of points defined in a general CS model which lie on the first 
  and second geometric objects of mate; \n
    2) A pair of enumerators #MtGeometryType meaning the mating types of geometry 
  which owns the points; \n
    3) The degree of freedom of movement of a pair of points in which is stored the 1st 
  condition (belonging to geometric objects which have the dimension);\n
    4) The dimension value is equal to the distance between points. \n \~
*/
//---
struct GCM_CLASS MtLDimensionTraits
{  
  MbCartPoint3D firstPoint;   ///< \ru Первая точка линейного размера. \en The first point of linear dimension. 
  MbCartPoint3D secondPoint;  ///< \ru Вторая точка линейного размера. \en The second point of linear dimension.  
  MtGeomType    firstType;    ///< \ru Тип геометрии, которой принадлежит firstPoint. \en The geometry type which owns firstPoint. 
  MtGeomType    secondType;   ///< \ru Тип геометрии, которой принадлежит secondPoint. \en The geometry type which owns secondPoint. 
  MtTransDof    translateDof; ///< \ru Степень свободы перемещения точек размера. \en The degree freedom of movement of dimension points. 
  double        value;        ///< \ru Текущее значение размера. \en Current value of the dimension. 
  
  MtLDimensionTraits()
    : firstPoint()
    , secondPoint()
    , firstType( GCM_NULL_GTYPE )
    , secondType( GCM_NULL_GTYPE )
    , translateDof() 
    , value( 0.0 )
  {}
};


//----------------------------------------------------------------------------------------
/** \brief \ru Характеристические данные углового размера.
           \en Characteristic data of angular dimension. \~
  \details \ru Структура данных содержит характерную информацию, необходимую, для 
  визуализации углового размера, а именно:\n
    1) Вектор вращения, он же нормаль измерительной плоскости;\n
    2) Пара векторов, из которых второй вектор получается из первого вращением 
  его вокруг вектора вращения (axisZ) на угол, равный значению размера;\n
    3) Значение размера - величина, которая равна углу вращения первого вектора 
  вокруг оси axisZ для совпадения его со вторым вектором.\n
           \en The data structure contains characteristic information needed for  
  rendering an angular dimension, namely:\n
    1) The rotation vector, it is normal of measuring plane; \n
    2) A pair of vectors where the second vector is obtained from the first by rotation 
  around the rotation vector (axisZ) by an angle which is equal to the value of dimension; \n
    3) Dimension value is equal to the rotation angle of the first vector 
  around axis axisZ to coincide it with the second vector.\n \~
*/
//---
struct GCM_CLASS MtADimensionTraits
{ 
  MbVector3D  firstVector;  ///< \ru Первый вектор. \en The first vector. 
  MbVector3D  secondVector; ///< \ru Второй вектор. \en The second vector. 
  MbVector3D  axisZ;        ///< \ru Ось вращения первого вектора до совпадения со вторым. \en Axis of rotation of the first vector to coincide with the second. 
  double      value;        ///< \ru Значение размера - угол вращения. \en Dimension value - the angle of rotation. 
  
  MtADimensionTraits()
    : firstVector()
    , secondVector()
    , axisZ()
    , value( 0.0 ) 
  {}
};

//----------------------------------------------------------------------------------------
/** \brief \ru Автоматически назначить тип сопряжению для его аргументов.
           \en Automatically assign the mate type for its arguments. \~
  \param cItem[in]   - \ru Геометрическое ограничение с неизвестным типом: mct_Unknown (см. #MtMateType).
                       \en Geometric constraint with unknown type: mct_Unknown (see #MtMateType). \~
  \param forMove[in] - \ru Способ оценки для тел с поведением перемещения, иначе вращения.
                       \en The evaluation method for solids with the behavior of moving or expression. \~
  \return \ru Тип, который следует назначить этому сопряжению (см. #MtMateType).
          \en Type which should be assigned to this mate (see #MtMateType). \~
  \details \ru Неизвестное сопряжение cItem этой функцией рассматривается, как пара 
  объектов сопряжения, для которой подбирается наиболее подходящий вариант 
  для стыковки. Например, для двух цилиндров это будет соосность. Флаг forMove
  помогает подобрать вариант стыковки с наиболее естественным поведением при 
  динамическом изменении объектов (например, указателем мыши).
           \en The unknown mate cItem of this feature is considered as a pair  
  of objects of mate for which to select the most suitable variant 
  for connection. For example: for the two cylinders it is coaxiality. The flag forMove
  helps to determine variant of connections to the most natural behavior 
   in dynamic changing objects (for example: the mouse cursor). \~
*/
//---
GCM_FUNC(MtMateType) GetAutoMateType( const ItConstraintItem & cItem, bool forMove );

//----------------------------------------------------------------------------------------
/** \brief \ru Измерить параметры близости к решению для геометрического ограничения.
           \en Measure parameters of proximity to solution for geometric constraint. \~
  \param[in]  cItem - \ru Рассматриваемое ограничение.
                      \en Considered constraint. \~
  \param[out] cPars - \ru Результирующие параметры оценки.
                      \en The resulting evaluation parameters. \~
  \return \ru Если возвращается true, то cPars содержит правильный ответ на запрос.
          \en If it returns true, then cPars contains the correct answer to the query. \~

  \details \ru Сопряжение #ItConstraintItem характеризуется целочисленными и 
  вещественных параметрами (опция выравнивания и размер). Функция 
  #GetClosestParameters вычисляет такое состояние параметров, при котором 
  ограничение было бы удовлетворено или наиболее близко к решению.
           \en The mate #ItConstraintItem is characterized by integer and 
  real parameters (alignment option and size). The function 
  #GetClosestParameters calculates the state of parameters in which 
  the mate would be satisfied or the closest to the solution. \~
*/
//---
GCM_FUNC(bool) GetClosestParameters( const ItConstraintItem & cItem, MtMateParameters & cPars );

//----------------------------------------------------------------------------------------
/** \brief \ru Определить характеристические данные размера.
           \en Determine the characteristic data of dimension. \~
  \param[in] dCon   - \ru Размерное геометрическое ограничение.
                      \en Dimensional geometric constraint. \~
  \param[out] dPars - \ru Структура характеристических параметров размера.
                      \en Structure of the characteristic parameters of dimension. \~
  \return \ru Если возвращается true, то dPars содержит правильный ответ на запрос.
          \en If it returns true, then dPars contains the correct answer to the query. \~
*/
//---
GCM_FUNC(bool) GetDimensionTraits( const ItConstraintItem & dCon, MtLDimensionTraits & dPars );

//----------------------------------------------------------------------------------------
/** \brief \ru Определить характеристические данные углового размера.
           \en Determine the characteristic data of angular dimension. \~
  \param[in]  dCon  - \ru Размерное геометрическое ограничение.
                      \en Dimensional geometric constraint. \~
  \param[out] dPars - \ru Структура характеристических параметров размера.
                      \en Structure of the characteristic parameters of dimension. \~
  \return \ru Если возвращается true, то dPars содержит правильный ответ на запрос.
          \en If it returns true, then dPars contains the correct answer to the query. \~
*/
//---
GCM_FUNC(bool) GetDimensionTraits( const ItConstraintItem & dCon, MtADimensionTraits & dPars );

//----------------------------------------------------------------------------------------
/** \brief \ru Оценить возможно ли применить вариант касания к данному ограничению.
           \en Whether it is possible to apply the variant of tangency to this constraint. \~  
  \param[in] cItem    - \ru Сопряжение, которое несет пару сопрягаемых объектов или хотя бы один (первый или второй).
                        \en Mate which carries a pair of mating objects or at least one (first or second). \~
  \param[in] tChoice  - \ru Вариант касания, который хотелось бы применить к сопряжению cItem.
                        \en Tangency variant which was to be applied to mate cItem. \~
  \return \ru Вернет true, если опция касания применима для данного ограничения.
          \en Will return true if the tangency option is applicable to this mate. \~
  \note \ru Сопряжение cItem может быть не полностью задано, т.е. выбран только один 
        геометрический объект, тогда функция ответит о применимости tChoice к 
        одному объекту без выбранного второго.
        \en The mate cItem can not be fully specified, i.e. is selected only one  
        geometric object, then the function will respond about applicability tChoice to  
        the one object without selecting the second. \~
*/
// ---
GCM_FUNC(bool) EstimateTangencyChoice( const ItConstraintItem & cItem, GCM_tan_choice tChoice );

//----------------------------------------------------------------------------------------
/** \brief \ru Определить для данного ограничения совместимость геометрических объектов.
           \en Determine the compatibility of geometric objects to this constraint. \~
  \details \ru Если в ограничении заданы и первый и второй сопрягаемые объекты, то 
  функция отвечает о возможности сопряжения заданных геометрических объектов, 
  иначе если в ограничении задано только один из объектов (первый либо второй), 
  то функция отвечает о применимости данного геом.объекта для данного сопряжения.
           \en If in the constraint given the first and second mating objects, then 
  a function provides about the possibility mating of given geometric objects, 
  otherwise if only one of the objects (either first or second) is given in the mate, 
  then function provides for the applicability of this geometric object for the given mate. \~
  \return \ru Вернет true, если данная пара геометрических объектов применима к данному типу сопряжения.
          \en Returns true if given pair of geometric objects is applicable to this type of mate. \~
*/
//---
GCM_FUNC(bool) IsCompatibleMatingGeometry( const ItConstraintItem & cItem );

//----------------------------------------------------------------------------------------
/** \brief  \ru Вычислить "Относительное размещение".
            \en Calculate "Relative placement". \~
  \details  \ru Функция реализует тип GCM_dependent_func, предполагающий зависимость 
  g1 = f( g2, g3, g4 ), а именно вычисляет положение первого объекта (g1), размещаемого
  относительно второго объекта g2 также, как g3 размещено относительно g4. Таким образом, 
  объект g1 является зависимым от остальных.
            \en  The function implements a type of GCM_dependent_func considering 
  dependency g1 = f( g2, g3, g4 ), namely calculates placement of first geom (g1)
  that is related to second geom (g2) in same way as g3 was placed relative to g4. 
  In this way, g1 is dependent on others.

  \param[in] gPlaces -  \ru Массив текущих положений геометрических объектов g1, g2 .. g4.
                        \en Array of current placements of geometric objects g1, g2 .. g4. \~  
  \param[out] gPlaces - \ru Вычисленное значение зависимого объекта g1 (возвращается в элемент gPlaces[0]).
                        \en Calculated placement of first geom g1 (it is returned to the element gPlaces[0]). \~  
  \param[in] gPlacesSize - \ru Известный размер принимаемого массива gPlaces.
                           \en Known size of received array gPlaces.\~
  \return \ru true, если функция выполнена успешно.
          \en true if the function is performed successfully. \~

  \par    \ru Реализация
          \en Implementation \~
  gPlaces[0] = gPlaces[2];\n
  gPlaces[0].Transform( gPlaces[3].GetMatrixInto() );\n
  gPlaces[0].Transform( gPlaces[1].GetMatrixFrom() );\n
*/
//---
GCM_FUNC(bool)  GCM_RelativePlacement(  MbPlacement3D   gPlaces[]
                                      , size_t          gPlacesSize
                                      , GCM_extra_param );

//----------------------------------------------------------------------------------------
/** \brief \ru Выдать первый или второй геометрический объект сопряжения.
           \en Get the first or second geometric object of mate. \~
  \param[in]  cItem   - \ru Геометрическое ограничение.
                        \en Geometric constraint. \~
  \param[in]  geomNb  - \ru 1-й или 2-й номер геометрического объекта.
                        \en 1st or 2nd index of geometric object. \~
  \param[in]  inWCS   - \ru Если = true, то функция вернет объект, 
                        заданный в МСК, иначе в ЛСК соответственно 1-го или 2-го тела.
                        \en If = true, then the function returns object 
                        given in GCS otherwise in LCS of 1st or 2nd solid respectively. \~
  
  \param[out] gType   - \ru Тип геометрического объекта.
                        \en A type of geometric object. \~
  \param pc, vec, r1, r2 - \ru Кортеж параметров объекта (описание ниже).
                           \en A tuple of object parameters (described below). \~
  \return \ru Если возвращается true, то кортеж {pc, vec, r1, r2} 
          содержит правильный ответ на запрос.
          \en If returns true the tuple {pc, vec, r1, r2} 
          contains the correct answer to the query. \~

  \par \ru Формат записи кортежа {pc, vec, r1, r2}
  Кортеж значений <b>{pc, vec, r1, r2}</b> унифицированно описывает все типы 
  геометрических объектов, перечисленных в #MtGeometryType.\n
  Точка задается единственным значением <b>{pc}</b>;\n
  Прямая задается парой <b>{pc, vec}</b>;\n
  Плоскость задается парой {\b pc, \b vec}, где \b vec - нормаль плоскости,
  \b pc - точка на плоскости;\n
  Цилиндр задается четверкой {\b pc, \b vec, \b r1, \b r2},
  где {\b pc,\b vec}-ось цилиндра, \b r1=\b r2 - радиус цилиндра;\n
  Окружность задается тройкой {\b pc, \b vec, \b r1},
  где {\b pc,\b vec}-ось окружности с центром в начале оси, \b r1 - радиус окружности, \b r2 = 0;\n
  Конус - <b>{pc, vec, r1, r2}</b>, где <b>{pc,vec}</b>-ось конуса,
  \b r1 - радиус основания, \b r2 - радиус сечения. Конус имеет высоту, равную единице,
  т.е. расстояние между основанием и сечением равно единице, точка \b pc лежит в 
  плоскости основания.\n
  Сфера - задана точкой \b pc -центр сферы и <b>r1=r2</b> -радиус сферы;\n  
  Тор задан точкой \b pc - центр тора, <b>{pc,vec}</b>-ось тора,
  \b r1 -большой радиус тора, \b r2 -радиус малый (половина толщины бублика).\n
       \en Writing format of the tuple {pc, vec, r1, r2}
  Tuple of values <b>{pc, vec, r1, r2}</b> unified describes all the types 
  of geometric objects listed in the #MtGeometryType. \n
  Point is given by a single value <b>{pc}</b>;\n
  Line is given by a pair <b>{pc, vec}</b>;\n
  Plane is given by a pair {\b pc, \b vec} where \b vec - normal of plane
  \b pc - point on the plane; \n
  Cylinder is given by {\b pc, \b vec, \b r1, \b r2}
  where {\b pc,\b vec} - the cylinder axis,  \b r1=\b r2 -  the cylinder radius; \n
  Circle is given by triplet: {\b pc, \b vec, \b r1, \b r2}
  where {\b pc,\b vec} - the circle axis with origin in circle center, \b r1 - the circle radius, \b r2=0; \n
  Cone - <b>{pc, vec, r1, r2}</b>, where <b>{pc,vec}</b>-the cone axis,
  \b r1 - the bottom radius, \b r2 - the section radius. Cone has a height equal to one
  i.e. the distance between the base and section is equal to one, the point \b pc lies in  
  the plane of the base. \n
  Sphere is given by a point \b pc - the sphere center and <b>r1=r2</b> -the sphere radius; \n  
  Torus is given by a point \b pc - the torus center, <b>{pc,vec}</b>-the torus axis, 
  \b r1 -the major radius of torus, \b r2 -the minor radius (half the thickness of a torus).\n \~
*/
//---
GCM_FUNC(bool) GetMatingGeometry( const ItConstraintItem & cItem, int geomNb, bool inWCS,
                                  MtGeomType & gType, MbCartPoint3D & pc, MbVector3D & vec,
                                  double & r1, double & r2 );

//----------------------------------------------------------------------------------------
/** \brief \ru Получить геометрический объект типа MtGeomVariant из геометрии MbSpaceItem.
           \en Get a geometric object of type MtGeomVariant from geometry of type MbSpaceItem. \~
    \details \en The function extracts geometry data of the solver type from geometric object presented class MbSpaceItem.
*/
//--
GCM_FUNC(MtGeomVariant) GCM_GeomArgument( const MbSpaceItem *, bool orient );

//----------------------------------------------------------------------------------------
/** \brief \ru Получить список редактируемых и нередактируемых свойств на систему ограничений целиком.
           \en Get a list of editable and non-editable properties for the entire constraint system. \~
    \note \ru Эта функция применяется в качестве аналитического инстумента разработчика.
          \en The function is used as an analytical tool for the developer.
*/
//---
GCM_FUNC(void) GCM_GetProperties( GCM_system gSys, MbProperties & props );

//----------------------------------------------------------------------------------------
/** \brief \ru Специфическая диагностика объекта, зависимого от истории построения.
           \en Specific diagnostics of a geometric object dependent on the construction history. \~
    \param[in] gSys -  \ru Система геометрических ограничений, в которой вычисляется объект диагностики gPtr.
                       \en The system of geometric constraints in which the diagnostic object 'gPtr' is evaluated. \~
    \param[in] gPtr -  \ru Указатель на геометрический объект CAD-модели, текущее состояние
                           которого вычислено в истории построения сборки САПР.
                       \en A pointer to a CAD model object whose current state is 
                           computed in the build history of the CAD assembly. \~
    \result \ru Результирующий код ошибки в ситуации противоречия.
            \en Resulting error code in the contradiction case. \~

    \details \ru Вызов API нацелен на диагностику геометрического объекта, который одновременно 
                 подчинен истории построения CAD-сборки и в то же время вычисляется в системе 
                 ограничений. Алгоритм выявляет ситуацию, когда текущее состояние объекта истории 
                 построения противоречит состоянию, вычисленному в системе ограничений. 
                 Результатом работы является код ошибки, который раздается всем смежным ограничениям, 
                 реализованным на стороне приложения в типе ItConstraintItem.
             \en The API-call is aimed at diagnosing a geometric object which at the same time
                 subordinate to the CAD-assembly built history and at the same time evaluated in 
                 the constraint system. The algorithm detects the situation when the current 
                 state of the history-based object contradicts the state evaluated in the solver.
                 The result of the call is an error code that is distributed to all adjacent constraints
                 inherited from ItConstraintItem inside the application. \~
    \note \ru Корректный результат предполагается только после попытки решить систему 
              ограничений (т.е. вызов GCM_Evaluate или MtGeomSolver::Evaluate).
          \en The correct result is assumed only after the evaluating call (ie GCM_Evaluate or MtGeomSolver::Evaluate). \~
*/
//---
GCM_FUNC(GCM_result) GCM_DiagnoseHistoryDependent( GCM_system gSys, const ItGeom * gPtr );

//----------------------------------------------------------------------------------------
/** \brief \ru Импортировать систему геометрических ограничений в модель C3D.
           \en Import the constraint system into C3D-model. \~
    \details  \ru Алгоритм импорта распознает каркасные структуры в системе ограничений и 
                  записывает их в файл формата C3D. Обнаруженные структуры конвертируются
                  в проволочное представление MbWireFrame.
              \en The algorithm of import recognises a framework structures in the constraint 
                  system and writes their into C3D model format. The recognized structures
                  are converted into wire-frame representation (see MbWireFrame).      
    \note For testing purposes.
*/
//--
GCM_FUNC(void) GCM_ImportToC3D( GCM_system gSys, const TCHAR * c3dFile );

//----------------------------------------------------------------------------------------
/** 
  \brief  \ru Число способов выравнивания (GCM_alignment) для заданного сопряжения.
          \en The number of alignment options (GCM_alignment) for a given mate.
*/
// ---
GCM_FUNC(size_t) VolumeOfAlignOption( const ItConstraintItem & );

//----------------------------------------------------------------------------------------
/** \brief \ru Вычислить степени свободы, включая соответствующие им возможные трансляции, 
               для точки геометрического объекта в системе ограничений.
           \en Evaluate degrees of freedom including their corresponding possible translations 
               for a point of a geometric object in a system of constraints. \~
    \param[in] gSys    - \ru Система ограничений.
                         \en System of constraints. \~
    \param[in] g       - \ru Дескриптор геометрического объекта.
                         \en Descriptor of geometric object. \~
    \param[in] ctrlPnt - \ru Контрольная точка (начало координат по умолчанию), заданная в ЛСК геометрического объекта.
                       - \en Control point (origin by default) specified with respect to LCS of geometric object. \~
    \result \ru Запись о вычисленных степенях свободы (см. #GCM_tdof_record).
            \en Record on evaluated degrees of freedom (see #GCM_tdof_record). \~
     \note \ru Вычисление результата опирается на линейный анализ системы ограничений.
               Текущая реализация может предоставить только мгновенные характеристики.
           \en Evaluation of the result relies on linear analysis of the constraint system.
               The current implementation can provide only instantaneous features. \~
*/
//---
GCM_FUNC(GCM_tdof_record) GCM_PointDOF( GCM_system gSys, GCM_geom g, const MbCartPoint3D& ctrlPnt = MbCartPoint3D::origin );

/** \} */ // GCM_3D_Routines

//----------------------------------------------------------------------------------------
// for testing only.
//---
GCT_FUNC(bool) CheckSatisfaction( GCM_system );

//----------------------------------------------------------------------------------------
// Get a range to traverse constraints of the system.
//---
GCT_FUNC(void) GCM_GetConstraints( GCM_system gSys, CNodeIterator & begIter, CNodeIterator & endIter );

//----------------------------------------------------------------------------------------
// The function evaluates a closest parameters that tend to satisfy dimension or constraint.
// For testing purposes.
//---
GCM_FUNC(GCM_closest_params) GCM_ClosestParameters( GCM_system gSys, const GCM_c_record & cRec );

//----------------------------------------------------------------------------------------
// The function evaluates a closest parameters that tend to satisfy dimension or constraint.
// For testing purposes.
//---
GCM_FUNC(GCM_closest_params) GCM_ClosestParameters( GCM_system gSys, GCM_constraint conId );

//----------------------------------------------------------------------------------------
// Change a constraint option of alignment.
// For testing purposes.
//---
GCM_FUNC(GCM_result) GCM_ChangeAlignment( GCM_system gSys, GCM_constraint conId, GCM_alignment aVal );

//----------------------------------------------------------------------------------------
/** \brief \ru Оценить снизу и сверху количество формальных степеней свободы
               геометрического объекта. Определяются минимальным набором линейно независимых
               параметров, задающих мгновенное движение жесткого тела (в системе ограничений).
           \en Estimate from below and above the amount of formal degrees of freedom
               for a geometric object. It is defined by a minimal set of linearly independent
               parameters determining instanteneous motion of a rigid (constrained) body. \~
    \param[in] gSys - \ru Система ограничений.
                      \en System of constraints. \~
    \param[in] g    - \ru Дескриптор геометрического объекта.
                      \en Descriptor of geometric object. \~
    \result \ru Количественная оценка (нижняя и верхняя границы с включением) формальных степеней свободы.
            \en Quantitative estimation (lower and upper bounds with inclusion) on formal degrees of freedom. \~
    \note \ru Для целей тестирования.
          \en For testing purposes. \~
**/
//---
GCM_FUNC( GCM_dof_estimation ) GCM_FormalDOF( GCM_system gSys, GCM_geom g );

//----------------------------------------------------------------------------------------
/** \brief \ru Вычислить чистые трансляционные степени свободы геометрического объекта.
               Такие степени остаются у тела (в системе ограничений) после устранения любых возможностей вращения.
           \en Compute pure translational degrees of freedom of a geometric object.
               Such degrees remains for a (constrained) body after eliminating any possibilities of rotation.
    \param[in] gSys - \ru Система ограничений.
                      \en System of constraints. \~
    \param[in] g    - \ru Дескриптор геометрического объекта.
                      \en Descriptor of geometric object. \~
    \result \ru Запись о вычисленных чистых трансляционных степенях свободы.
            \en Record on evaluated pure translational degrees of freedom. \~
    \note \ru Для целей тестирования.
          \en For testing purposes. \~
**/
//---
GCM_FUNC( GCM_tdof_record ) GCM_PureTDOF( GCM_system gSys, GCM_geom g );

#endif // __GCM_ROUTINES_H

// eof
