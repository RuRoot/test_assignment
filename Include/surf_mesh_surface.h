////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Поверхность на двух семействах кривых (на сетке кривых).
         \en The surface passing through two families of curves. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_MESH_SURFACE_H
#define __SURF_MESH_SURFACE_H


#include <surface.h>
#include <curve3d.h>
#include <math_define.h>
#include <templ_dptr.h>
#include <system_types.h>
#include <function.h>
#include <tool_multithreading.h>
#include <surf_tessellation.h>
#include <map>


class   MbSurfaceCurve;
class   MbFunction;
class   MbSurfaceContiguousData;
class             MbRectPatchBaseData;
class             MbCoonsPatchData;
struct  MeshSurfaceValues;


typedef std::map<const MbCurve3D *, double> MapCurveParam;
typedef std::map<const MbCurve3D *, MapCurveParam> MapCrosses;


//------------------------------------------------------------------------------
/** \brief \ru Версия реализации поверхности на сетке кривых.
           \en Version of implementation of surface constructed by the grid curves. \~
  \details \ru Версия реализации поверхности на сетке кривых. \n
           \en Version of implementation of surface constructed by the grid curves. \n \~
  \ingroup Surfaces
*/ 
// ---
enum MbeMeshSurfaceVersion {
  msv_Ver0 = 0, ///< \ru Нулевая версия. \en The first version. 
  msv_Ver1,     ///< \ru Первая версия. \en The first version. 
  msv_Ver2,     ///< \ru Вторая версия. \en The second version. 
  msv_Ver3,     ///< \ru Третья версия. \en The third version.
  msv_Ver4,     ///< \ru Четвертая версия. \en The fourth version.
  msv_Ver5,     ///< \ru Пятая версия. \en The fifth version.
  msv_Ver6,     ///< \ru Шестая версия. \en The sixs version.
  msv_Count     ///< \ru Количество версий. \en Count of versions. 
};


//------------------------------------------------------------------------------
/** \brief \ru Способ расчета трансверсального вектора на границе сопряжения.
           \en Method of calculation the transversal vector at the mating bound. \~
*/ 
// ---
enum MbeTransversCalculationType
{
  tct_auto   =-1, ///< \ru Автоматический подбор метода для каждого интервала.
                  ///< \en Automatic method choosing for each interval.
  tct_local  = 0, ///< \ru Интерполяция в локальной системе координат.
                  ///< \en Interpolation in local coordinate system.
  tct_global = 1  ///< \ru Интерполяция в глобальной системе координат.
                  ///< \en Interpolation in global coordinate system.
};


//------------------------------------------------------------------------------
/** \brief \ru Поверхность на сетке кривых.
           \en The surface constructed by the grid curves. \~
  \details \ru Поверхность, проходящая через два семейства кривых.
    Первое семейство определяет форму поверхности при изменении первого параметра и неподвижном втором параметре,
    Второе семейство определяет форму поверхности при изменении второго параметра и неподвижном первом параметре.
    Каждая кривая первого семейства должна пересекаться или иметь точки скрещивания с каждой кривой второго семейства.
           \en The surface passing through two families of curves.
    The first family determines a surface form at change of the first parameter and fixed second parameter.
    The second family determines a surface form at change of the second parameter and fixed first parameter.
    Each curve of first family has to be intersected or has intersection points with each curve of the second family. \~
  \ingroup Surfaces
*/ 
// ---
class MATH_CLASS MbMeshSurface : public MbSurface {

//          t2min    curve2    t2max
//         P3______________________P2
//    t3max |                      | t1max
//          |                      |
//          |                      |
//   curve3 |           R          | curve1
//          |                      |
//          |                      |
//    t3min |______________________|t1min
//         P0                      P1
//         t0min    curve0    t0max
// \ru Состыкованные между собой поверхности Кунса \en Coons surfaces joined among themselves 
// \ru В пределах одного патча выражается как булева сумма двух Loft -ов \en Within one patch it is expressed as the Boolean sum of two Lofts 
// R(u,v) = P(u,v) + P(v,u) - P(u,v)P(v,u) 
// \ru где \en Where 
// H1(w) =  2*w*w*w - 3*w*w + 1 
// H2(w) =    w*w*w - 2*w*w + w 
// H3(w) = -2*w*w*w + 3*w*w 
// H4(w) =    w*w*w -   w*w
// \ru P(u,v) = curve0(t0)     * (H1(v)) +     - Эрмитова кубическая интерполяция \en P(u,v) = curve0(t0)     * (H1(v)) +     - cubic Hermite interpolation 
// \ru d(curve0(t0))/dv * (H2(v)) +     - аппроксимация выводящей производной с кривой \en D(curve0(t0))/dv * (H2(v)) +     - approximation of leading out derivative from curve 
//          curve2(t2)     * (H3(v)) + 
//        d(curve2(t2))/dv * (H4(v))
// P(v,u) = curve3(t3)     * (H1(u)) +              
//        d(curve3(t3))/du * (H2(u)) +
//          curve1(t1)     * (H3(u)) + 
//        d(curve1(t1))/du * (H3(u))
// P(u,v)P(v,u) = 
//                             |R(0,0)          d(R(0,0))/(dv)   R(0,1)          d(R(0,1))/(dv)  |   |H1(v)|
// [H1(u) H2(u) H3(u) H4(u)] x |d(R(0,0))/(du) d2(R(0,0))/(dudv) d(R(0,1))/(du) d2(R(0,1))/(dudv)|   |H2(v)| 
//                             |R(1,0)          d(R(1,0))/(dv)   R(1,1)          d(R(1,1))/(dv)  | x |H3(v)|
//                             |d(R(1,0))/(du) d2(R(1,0))/(dudv) d(R(1,1))/(du) d2(R(1,1))/(dudv)|   |H4(v)|
// R(I,J) === curveI(tI(J))
private:
  size_t                nu;      ///< \ru Количество кривых вдоль первого параметра u. \en Count of curves along first parameter u. 
  size_t                nv;      ///< \ru Количество кривых вдоль второго параметра v. \en Count of curves along second parameter v. 
  RPArray<MbCurve3D>    uCurves; ///< \ru Множество кривых первого семейства, которые направлены вдоль параметра u. \en Set of curves of first family which are directed along parameter u. 
  RPArray<MbCurve3D>    vCurves; ///< \ru Множество кривых второго семейства, которые направлены вдоль параметра v. \en Set of curves of second family which are directed along parameter v. 
  RPArray<MbFunction>   tuParams; ///< \ru Множество функций перехода к параметрам первого семейства. \en Set of transformations to parameters of first set. 
  RPArray<MbFunction>   tvParams; ///< \ru Множество функций перехода к параметрам второго семейства. \en Set of transformations to parameters of second set.
  SArray<double>        uParams; ///< \ru Множество параметров u для задающих кривых. \en Set of parameters u for driving curves. 
  SArray<double>        vParams; ///< \ru Множество параметров v для задающих кривых. \en Set of parameters v for driving curves. 
  
  SArray<double>        tuCurve; ///< \ru Множество параметров uCurves[i] точек пересечения кривых. \en Set of parameters of uCurves[i] intersection points of curves. 
  SArray<double>        tvCurve; ///< \ru Множество параметров vCurves[j] точек пересечения кривых. \en Set of parameters of vCurves[j] intersection points of curves. 

  SArray<MbCartPoint3D> points;      ///< \ru Множество узловых точек поверхности. \en The surface nodes set. 
  SArray<MbVector3D>    uFirstDers;  ///< \ru Множество первых производных поверхности в направлении u в узлах. \en Set of first derivatives by u at the surface nodes. 
                                     // \ru Множество первых производных uCurves[i] точек пересечения кривых для версий ранее msv_Ver2. \en Set of first derivatives of uCurves[i] intersection points of curves for versions less than msv_Ver2. 
  SArray<MbVector3D>    vFirstDers;  ///< \ru Множество первых производных поверхности в направлении v в узлах. \en Set of first derivatives by v at the surface nodes.
                                     // \ru Множество первых производных vCurves[j] точек пересечения кривых для версий ранее msv_Ver2. \en Set of first derivatives of vCurves[j] intersection points of curves for versions less than msv_Ver2. 
  SArray<MbVector3D>    uSecondDers; ///< \ru Множество вторых производных поверхности в направлении u в узлах. \en Set of second derivatives by u at the surface nodes.
                                     // \ru Множество вторых производных uCurves[i] точек пересечения кривых для версий ранее msv_Ver2. \en Set of second derivatives of uCurves[i] intersection points of curves for versions less than msv_Ver2. 
  SArray<MbVector3D>    vSecondDers; ///< \ru Множество вторых производных поверхности в направлении v в узлах. \en Set of second derivatives by v at the surface nodes.
                                     // \ru Множество вторых производных vCurves[j] точек пересечения кривых для версий ранее msv_Ver2. \en Set of second derivatives of vCurves[j] intersection points of curves for versions less than msv_Ver2. 

  SArray<MbVector3D>    twists;       ///< \ru Множество смешанных производных (сначала по v, потом по u) в узлах сетки. \en Set of mixed derivatives (at first by u, then by v) at grid nodes. 
                                      // \ru Последовательность точек пересечения кривых: \en Sequence of intersection points of curves: 
                                      // \ru uCurves[0] и vCurves[0], uCurves[0] и vCurves[1], ... \en UCurves[0] and vCurves[0], uCurves[0] and vCurves[1], ... 
                                      // \ru uCurves[1] и vCurves[0], uCurves[1] и vCurves[1], ... \en UCurves[1] and vCurves[0], uCurves[1] and vCurves[1], ... 
  SArray<MbVector3D>    boundTwists[4];///< \ru Для границ сопряжения трансверсальные вектора и их производные, выраженные в СК связанной с границей.
                                       ///<\ ru For conjugation boundaries, transverse vectors and their derivatives, expressed in the coordinate system associated with the boundary.
  SArray<MbVector3D>    cornerTwists; ///< \ru Множество смешанных производных (сначала по u, потом по v) в угловых узлах сетки. \en Set of mixed derivatives (at first by v, then by u) at corner grid nodes. 
  SArray<bool>          cornerRegular;///< \ru Регулярность в углах поверхности. \en Regularity in the surface corners.
                                      //    3 x-------x 2
                                      //      |       |
                                      //      |       |
                                      //    0 x-------x 1

  double                umin;     ///< \ru Минимальное значение параметра u. \en Minimal value of parameter u. 
  double                vmin;     ///< \ru Минимальное значение параметра v. \en Minimal value of parameter v. 
  double                umax;     ///< \ru Максимальное значение параметра u. \en Maximal value of parameter u. 
  double                vmax;     ///< \ru Максимальное значение параметра v. \en Maximal value of parameter v.
  bool                  uclosed;  ///< \ru Признак замкнутости по параметру u. \en Attribute of closedness by parameter u. 
  bool                  vclosed;  ///< \ru Признак замкнутости по параметру v. \en Attribute of closedness by parameter v. 
  // \ru Сопряжение на границе определяет способ вычисления выводящих производных в прилегающих ячейках. \en Conjugation on boundary determines a way of calculation of leading out derivatives at adjacent cells. 
  uint                  type0;    ///< \ru Вид сопряжения заданный на curvesU[0]. \en Type of conjugation given on curvesU[0]. 
  uint                  type1;    ///< \ru Вид сопряжения заданный на curvesV[0]. \en Type of conjugation given on curvesV[0]. 
  uint                  type2;    ///< \ru Вид сопряжения, заданный на curvesU[nu-1]. \en Type of conjugation given on curvesU[nu-1]. 
  uint                  type3;    ///< \ru Вид сопряжения, заданный на curvesV[nv-1]. \en Type of conjugation given on curvesV[nv-1].
  MbeTransversCalculationType    transTypes[4];///< \ru Способ расчета трансверсального вектора. \en Method of calculating the transversal vector.
  c3d::BoolVector       mapTTypes[4]; ///< \ru Схема интерполяции при transTypes=tct_auto. \en Interpolation scheme for transTypes=tct_auto
  uint8                 coonsOrd[2];///< \ru Использовать форму Кунса второго порядка гладкости. \en Use the Koons form of the second order of smoothness.
  
  MbeMeshSurfaceVersion version;  ///< \ru Версия реализации определяет форму поверхности. \en Version of implementation determines a shape of surface. 

  MbCurve3D           * extCurves[4]; ///< \ru Дополнительные кривые с соседних патчей. \en Additional curves from adjacent patches.
  double                extParams[4]; ///< \ru Параметры поверхности для дополнительных кривых с соседних патчей. \en Surface parameters for additional curves from adjacent patches.
  bool                  extFlags [4]; ///< \ru Есть ли соседний патч через границу. \en Is there a adjacent patch across the border.
  MbFunction          * extFuncs [4]; ///< \ru Функция перехода к параметрам кривых соседних патчей. \en Mapping functions for curves from adjacent patches.

public:
  //------------------------------------------------------------------------------
  /** \brief \ru Вспомогательные данные. 
             \en Auxiliary data. \~
    \details \ru Вспомогательные данные служат для ускорения работы объекта.
             \en Auxiliary data are used for fast calculations. \n \~
  */
  // ---
  class MbMeshSurfaceAuxiliaryData : public AuxiliaryData {
  public:
    DPtr<MbSurfaceContiguousData> data; ///< \ru Дополнительные данные о поверхности. \en Additional data about a surface. 
    DPtr<MbRectPatchBaseData>     mp;   ///< \ru Дополнительные временные данные для ускорения вычислений. \en Additional temporary data to speed up computations. 
    MbMeshSurfaceAuxiliaryData();
    MbMeshSurfaceAuxiliaryData( const MbMeshSurfaceAuxiliaryData & init );
    virtual ~MbMeshSurfaceAuxiliaryData();
  };
  mutable CacheManager<MbMeshSurfaceAuxiliaryData> cache;

private:
  /** \brief \ru Конструктор поверхности.
             \en Constructor of surface. \~
  \details \ru Конструктор поверхности по двум семействам кривых. Каждая кривая семейства U должна пересекаться или
  иметь точки скрещивания с каждой кривой семейства V.
           \en Constructor of surface by two families of curves. Each curve of family U has to be intersected or
  has intersection points with each curve of family V. \~
      \param[in] initU - \ru Множество кривых в направлении параметра u.
                         \en Set of curves at direction of parameter u. \~
      \param[in] initV - \ru Множество кривых в направлении параметра v.
                         \en Set of curves at direction of parameter v. \~
      \param[in] uClosed - \ru Замкнута ли поверхность по параметру u.
                           \en Whether the surface is closed by parameter u. \~
      \param[in] vClosed - \ru Замкнута ли поверхность по параметру v.
                           \en Whether the surface is closed by parameter v. \~
      \param[in] same - \ru Определяет, надо ли делать копии кривых:\n
                 true - использовать в объекте пришедшие в конструктор кривые не дублируя,\n
                 false - использовать копии кривых.
                        \en Determines whether to copy curves:\n
                 true - use curves given in the constructor in object without copying,\n
                 false - use copies of curves. \~
      \param[in] types - \ru Ссылка на массив с типами сопряжений на границах.
                         \en Reference to array with types of conjugations at boundaries. \~
      \param[in] vers - \ru Версия реализации поверхности.
                        \en Version of surface implementation. \~
  */
  MbMeshSurface( RPArray<MbCurve3D> & initU, RPArray<MbCurve3D> & initV, 
                 bool uClosed, bool vClosed, 
                 bool same, const SArray<uint> * types = nullptr,
                 MbeMeshSurfaceVersion vers = msv_Ver3 );
  /** \brief \ru Конструктор поверхности.
             \en Constructor of surface. \~
  \details \ru Конструктор поверхности по двум семействам кривых. Каждая кривая семейства U должна пересекаться или
  иметь точки скрещивания с каждой кривой семейства V.
           \en Constructor of surface by two families of curves. Each curve of family U has to be intersected or
  has intersection points with each curve of family V. \~
      \param[in] initU - \ru Множество кривых в направлении параметра u.
                         \en Set of curves at direction of parameter u. \~
      \param[in] initV - \ru Множество кривых в направлении параметра v.
                         \en Set of curves at direction of parameter v. \~
      \param[in] uClosed - \ru Замкнута ли поверхность по параметру u.
                           \en Whether the surface is closed by parameter u. \~
      \param[in] vClosed - \ru Замкнута ли поверхность по параметру v.
                           \en Whether the surface is closed by parameter v. \~
      \param[in] g2 -   \ru Использовать форму Кунса второго порядка гладкости.
                        \en Use the Koons shape of the second order of smoothness. \~
      \param[in] same - \ru Определяет, надо ли делать копии кривых:\n
                 true - использовать в объекте пришедшие в конструктор кривые не дублируя,\n
                 false - использовать копии кривых.
                        \en Determines whether to copy curves:\n
                 true - use curves given in the constructor in object without copying,\n
                 false - use copies of curves. \~
      \param[in] types - \ru Ссылка на массив с типами сопряжений на границах.
                         \en Reference to array with types of conjugations at boundaries. \~
      \param[in] vers - \ru Версия реализации поверхности.
                        \en Version of surface implementation. \~
  */
  MbMeshSurface( RPArray<MbCurve3D> & initU, RPArray<MbCurve3D> & initV, 
                 bool uClosed, bool vClosed, bool g2,
                 bool same, const SArray<uint> * types,
                 MbeMeshSurfaceVersion vers );
 
public:
  /** \brief \ru Конструктор поверхности.
             \en Constructor of surface. \~
  \details \ru Конструктор поверхности по двум семействам кривых. Каждая кривая семейства U должна пересекаться или
  иметь точки скрещивания с каждой кривой семейства V.
           \en Constructor of surface by two families of curves. Each curve of family U has to be intersected or
  has intersection points with each curve of family V. \~
      \param[in] initU - \ru Множество кривых в направлении параметра u.
                         \en Set of curves at direction of parameter u. \~
      \param[in] initV - \ru Множество кривых в направлении параметра v.
                         \en Set of curves at direction of parameter v. \~
      \param[in] parsU - \ru Множество параметров u для задающих кривых.
                         \en Set of parameters u for driving curves. \~
      \param[in] parsV - \ru Множество параметров v для задающих кривых.
                         \en Set of parameters v for driving curves. \~
      \param[in] uClosed - \ru Замкнута ли поверхность по параметру u.
                           \en Whether the surface is closed by parameter u. \~
      \param[in] vClosed - \ru Замкнута ли поверхность по параметру v.
                           \en Whether the surface is closed by parameter v. \~
      \param[in] same - \ru Определяет, надо ли делать копии кривых:\n
                 true - использовать в объекте пришедшие в конструктор кривые не дублируя,\n
                 false - использовать копии кривых.
                        \en Determines whether to copy curves:\n
                 true - use curves given in the constructor in object without copying,\n
                 false - use copies of curves. \~
      \param[in] types - \ru Ссылка на массив с типами сопряжений на границах.
                         \en Reference to array with types of conjugations at boundaries. \~
      \param[in] vers - \ru Версия реализации поверхности.
                        \en Version of surface implementation. \~
  */
  MbMeshSurface( RPArray<MbCurve3D> & initU, RPArray<MbCurve3D> & initV,
                 SArray<double>     & parsU, SArray<double>     & parsV, 
                 bool uClosed, bool vClosed,
                 bool same, const SArray<uint> * types = nullptr,
                 MbeMeshSurfaceVersion vers = msv_Ver3 );
  
private:
  /** \brief \ru Конструктор поверхности.
             \en Constructor of surface. \~
  \details \ru Конструктор поверхности по двум семействам кривых. Каждая кривая семейства U должна пересекаться или
  иметь точки скрещивания с каждой кривой семейства V.
           \en Constructor of surface by two families of curves. Each curve of family U has to be intersected or
  has intersection points with each curve of family V. \~
      \param[in] initU - \ru Множество кривых в направлении параметра u.
                         \en Set of curves at direction of parameter u. \~
      \param[in] initV - \ru Множество кривых в направлении параметра v.
                         \en Set of curves at direction of parameter v. \~
      \param[in] parsU - \ru Множество параметров u для задающих кривых.
                         \en Set of parameters u for driving curves. \~
      \param[in] parsV - \ru Множество параметров v для задающих кривых.
                         \en Set of parameters v for driving curves. \~
      \param[in] uClosed - \ru Замкнута ли поверхность по параметру u.
                           \en Whether the surface is closed by parameter u. \~
      \param[in] vClosed - \ru Замкнута ли поверхность по параметру v.
                           \en Whether the surface is closed by parameter v. \~
      \param[in] g2 -   \ru Использовать форму Кунса второго порядка гладкости.
                        \en Use the Koons shape of the second order of smoothness. \~
      \param[in] same - \ru Определяет, надо ли делать копии кривых:\n
                 true - использовать в объекте пришедшие в конструктор кривые не дублируя,\n
                 false - использовать копии кривых.
                        \en Determines whether to copy curves:\n
                 true - use curves given in the constructor in object without copying,\n
                 false - use copies of curves. \~
      \param[in] types - \ru Ссылка на массив с типами сопряжений на границах.
                         \en Reference to array with types of conjugations at boundaries. \~
      \param[in] vers - \ru Версия реализации поверхности.
                        \en Version of surface implementation. \~
  */
  MbMeshSurface( RPArray<MbCurve3D> & initU, RPArray<MbCurve3D> & initV,
                 SArray<double>     & parsU, SArray<double>     & parsV, 
                 bool uClosed, bool vClosed, bool g2, 
                 bool same, const SArray<uint> * types,
                 MbeMeshSurfaceVersion vers );

  public:
  /** \brief \ru Функция создания поверхности.
             \en Surface creation function. \~
  \details \ru Функция создания поверхности по сети кривых. Возвращает созданную поверхность
               в случае корректно переданных данных, в противном случае - нулевой указатель.
               Результат операции синхронизован с работой строителя оболочки по сети кривых.
           \en Function for creating a surface from a mesh of curves. 
               Returns the created surface if the data is passed correctly, otherwise a null pointer.
               The result of the operation is synchronized with the work of the mesh shell creator.\~
      \param[in] pars - \ru Параметры создания поверхности.
                         \en Surface creation parameters. \~
      \param[in] vers - \ru Версия операции создания поверхности.
                         \en Version of the surface creation operation. \~
      \return \ru Указатель на созданную поверхность, либо нулевой указатель.
              \en A pointer to the created surface, or a null pointer. \~
  */
  static 
  MbMeshSurface * Create( const MeshSurfaceValues & pars, VERSION vers = Math::DefaultMathVersion() );

  /** \brief \ru Функция создания поверхности.
             \en Surface creation function. \~
  \details \ru Функция создания поверхности по сети кривых. Возвращает созданную поверхность
               в случае корректно переданных данных, в противном случае - нулевой указатель.
               Результат операции синхронизован с работой строителя оболочки по сети кривых.
           \en Function for creating a surface from a mesh of curves. 
               Returns the created surface if the data is passed correctly, otherwise a null pointer.
               The result of the operation is synchronized with the work of the mesh shell creator.\~
      \param[in] pars - \ru Параметры создания поверхности.
                         \en Surface creation parameters. \~
      \param[in] acc - \ru Точность пересечения кривых.
                       \en Accuracy of curves intersection. \~
      \param[in] vers - \ru Версия операции создания поверхности.
                         \en Version of the surface creation operation. \~
      \return \ru Указатель на созданную поверхность, либо нулевой указатель.
              \en A pointer to the created surface, or a null pointer. \~
  */
  static 
  MbMeshSurface * Create( const MeshSurfaceValues & pars, double acc, VERSION vers );
private:
  friend class CompositeMeshShellCreator;
  friend class CompositeMeshShellCreatorV19;
  friend class MbPatchBuilder;
  /** \brief \ru Конструктор поверхности с дополнительными задающими кривыми.
             \en Surface constructor with additional defining curves. \~
  \details \ru Конструктор вызывается из генератора CompositeMeshShellCreator для создания
               фрагментов составной поверхности по сети кривых с возможностью тангенциального
               сопряжения их между собой.
           \en The constructor is called from the CompositeMeshShellCreator generator to create 
               fragments of the composite surface by mesh of curves with the possibility of 
               tangential mating between them. \~
      \param[in] initU, initV - \ru Множество кривых в направлении параметра u и v.
                                \en Set of curves at direction of parameter u and v. \~
      \param[in] initFU, initFV - \ru Множество функций репараметризации для соответствующих кривых.
                                  \en Set of reparametrization functions for the corresponding curves. \~
      \param[in] parsU, parsV - \ru Множество параметров u и v для задающих кривых.
                                \en Set of parameters u and v for driving curves. \~
      \param[in] uClosed, vclosed - \ru Замкнута ли поверхность по параметру u и v.
                                    \en Whether the surface is closed by parameter u and v. \~
      \param[in] adjPatch - \ru Признаки сопряжения с соседними фрагментами через соответствующие границы.
                            \en Flags of mating with neighboring fragments through the corresponding borders.
      \param[in] same - \ru Определяет, надо ли делать копии кривых:\n
                 true - использовать в объекте пришедшие в конструктор кривые не дублируя,\n
                 false - использовать копии кривых.
                        \en Determines whether to copy curves:\n
                 true - use curves given in the constructor in object without copying,\n
                 false - use copies of curves. \~
      \param[in] types - \ru Ссылка на массив с типами сопряжений на границах.
                         \en Reference to array with types of conjugations at boundaries. \~
      \param[in] vers - \ru Версия реализации поверхности.
                        \en Version of surface implementation. \~
      \param[in] orders - \ru Порядок формы Кунса в направлени u и v.
                          \en The order of the Coons form in the u and v directions.  \~
  */
  MbMeshSurface( c3d::SpaceCurvesSPtrVector & initU, c3d::SpaceCurvesSPtrVector & initV,
                 c3d::FunctionSPtrVector * initFU, c3d::FunctionSPtrVector * initFV,
                 c3d::DoubleVector   & parsU, c3d::DoubleVector & parsV,
                 c3d::DoubleVector   * tuCurvePars, c3d::DoubleVector * tvCurvePars,
                 bool uClosed, bool vClosed, 
                 const bool (&adjPatch)[4],
                 const MbeMatingType( &types )[4],
                 MbeMeshSurfaceVersion vers, uint8( &orders )[2], MbeTransversCalculationType (&ttypes)[4] );
#ifdef C3D_DEBUG
  // \ru Проверить согласованность производых. \en Check the consistency of the derivatives.
  bool    TestSurfaceDerivatives() const;
#endif // C3D_DEBUG
protected:
  /// \ru Конструктор-копия. \en Copy constructor. 
  MbMeshSurface( const MbMeshSurface &, MbRegDuplicate * ); 
public:
  virtual ~MbMeshSurface();

public:
  VISITING_CLASS( MbMeshSurface );

  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */
  MbeSpaceType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbeSpaceType  Type() const override; // \ru Групповой тип элемента. \en Group element type. 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Make a copy of element
  bool    IsSame( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Является ли объект копией. \en Whether the object is a copy. 
  bool    SetEqual( const MbSpaceItem & init ) override;       // \ru Сделать равным \en Make equal 
  bool    IsSimilar( const MbSpaceItem & init ) const override;  // \ru Являются ли объекты подобными \en Whether the objects are similar 
  void    Transform( const MbMatrix3D & matr, MbRegTransform * = nullptr ) override;         // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void    Move( const MbVector3D &to, MbRegTransform * = nullptr ) override;                 // \ru Сдвиг \en Translation
  void    Rotate( const MbAxis3D &axis, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis
  void    Refresh() override; // \ru Сбросить все временные данные \en Flush all the temporary data 

  void    GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void    SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void    GetBasisItems ( RPArray<MbSpaceItem>   & ) override;  // \ru Дать базовые объекты \en Get the base objects 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  /** \} */

  /** \ru \name Функции описания области определения поверхности
      \en \name Functions for surface domain description
      \{ */
  double  GetUMin() const override;
  double  GetVMin() const override;
  double  GetUMax() const override;
  double  GetVMax() const override;

  bool    IsUClosed() const override;  // \ru Замкнута ли гладко поверхность по параметру u без учета граничного контура. \en Whether the surface is smoothly closed by parameter u without regard to the boundary contour. 
  bool    IsVClosed() const override;  // \ru Замкнута ли гладко поверхность по параметру v без учета граничного контура. \en Whether the surface is smoothly closed by parameter v without regard to the boundary contour. 
  bool    IsUTouch() const override;  // \ru Замкнута ли фактически поверхность по параметру u независимо от гладкости. \en Whether the surface is actually closed by parameter u regardless of the smoothness. 
  bool    IsVTouch() const override;  // \ru Замкнута ли фактически поверхность по параметру v независимо от гладкости. \en Whether the surface is actually closed by parameter v regardless of the smoothness. 
  bool    IsUPeriodic() const override;  // \ru Замкнута ли гладко поверхность по параметру u. \en Whether the surface is smoothly closed by parameter u. 
  bool    IsVPeriodic() const override;  // \ru Замкнута ли гладко поверхность по параметру v. \en Whether the surface is smoothly closed by parameter v. 
  double  GetUPeriod() const override; // \ru Вернуть период для замкнутой поверхности или 0. \en Return period for closed surface or 0. 
  double  GetVPeriod() const override; // \ru Вернуть период для замкнутой поверхности или 0. \en Return period for closed surface or 0. 

  size_t  GetUCount() const override;
  size_t  GetVCount() const override;
  // \ru Существует ли полюс на границе параметрической области \en Whether there is pole on boundary of parametric region 
  bool    GetPoleUMin() const override;
  bool    GetPoleUMax() const override;
  bool    GetPoleVMin() const override;
  bool    GetPoleVMax() const override;
  bool    IsPole( double u, double v, double paramPrecision = PARAM_PRECISION ) const override; // \ru Является ли точка особенной \en Whether the point is special 
  /** \} */

  /** \ru \name Функции для работы в области определения поверхности
            Функции PointOn, Derive... поверхностей корректируют параметры
            при выходе их за пределы прямоугольной области определения параметров.\n
      \en \name Functions for working at surface domain
            Functions PointOn, Derive... of surfaces correct parameters
            when they are out of bounds of rectangular domain of parameters.\n 
      \{ */
  void    PointOn  ( double & u, double & v, MbCartPoint3D & p ) const override; // \ru Точка на поверхности \en Point on the surface 
  void    DeriveU  ( double & u, double & v, MbVector3D & p ) const override; // \ru Первая производная по u \en First derivative with respect to u 
  void    DeriveV  ( double & u, double & v, MbVector3D & p ) const override; // \ru Первая производная по v \en First derivative with respect to v 
  void    DeriveUU ( double & u, double & v, MbVector3D & p ) const override; // \ru Вторая производная по u \en Second derivative with respect to u 
  void    DeriveVV ( double & u, double & v, MbVector3D & p ) const override; // \ru Вторая производная по v \en Second derivative with respect to v 
  void    DeriveUV ( double & u, double & v, MbVector3D & p ) const override; // \ru Вторая производная по uv \en Second derivative with respect to uv 
  void    DeriveUUU( double & u, double & v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative 
  void    DeriveUUV( double & u, double & v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative 
  void    DeriveUVV( double & u, double & v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative 
  void    DeriveVVV( double & u, double & v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative 
  void    Normal   ( double & u, double & v, MbVector3D & p ) const override; // \ru Нормаль \en Normal 
  /** \} */

  /** \ru \name Функции для работы внутри и вне области определения поверхности
            функции _PointOn, _Derive... поверхностей не корректируют
            параметры при выходе их за пределы прямоугольной области определения параметров.
      \en \name Functions for working inside and outside the surface's domain
            functions _PointOn, _Derive... of surfaces don't correct
            parameters when they are out of bounds of rectangular domain of parameters.
      \{ */
  void   _PointOn  ( double u, double v, MbCartPoint3D & p ) const override; // \ru Точка на поверхности \en Point on the surface
  void   _DeriveU  ( double u, double v, MbVector3D & p ) const override; // \ru Первая производная по u \en First derivative with respect to u
  void   _DeriveV  ( double u, double v, MbVector3D & p ) const override; // \ru Первая производная по v \en First derivative with respect to v
  void   _DeriveUU ( double u, double v, MbVector3D & p ) const override; // \ru Вторая производная по u \en Second derivative with respect to u
  void   _DeriveVV ( double u, double v, MbVector3D & p ) const override; // \ru Вторая производная по v \en Second derivative with respect to v
  void   _DeriveUV ( double u, double v, MbVector3D & p ) const override; // \ru Вторая производная по uv \en Second derivative with respect to uv
  void   _DeriveUUU( double u, double v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative
  void   _DeriveUUV( double u, double v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative
  void   _DeriveUVV( double u, double v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative
  void   _DeriveVVV( double u, double v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative
  void   _Normal   ( double u, double v, MbVector3D & p ) const override; // \ru Нормаль \en Normal
  /** \} */

  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметров поверхности.
      \en \name Functions for get of the group of data inside and outside the surface's domain of parameters.
      \{ */
  void    Explore( double & u, double & v, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer,
                   MbVector3D * uuDer, MbVector3D * vvDer, MbVector3D * uvDer, MbVector3D * nor ) const override;
  /** \} */

  /** \ru \name Функции движения по поверхности
      \en \name Functions of moving along the surface
      \{ */
  double  StepU         ( double u, double v, double sag ) const override; // \ru Вычисление шага параметра u по по величине прогиба \en Calculation of parameter u step by the value of sag  
  double  StepV         ( double u, double v, double sag ) const override; // \ru Вычисление шага параметра v по по величине прогиба \en Calculation of parameter v step by the value of sag 
  double  DeviationStepU( double u, double v, double ang ) const override; // \ru Вычисление шага параметра u по углу отклонения нормали \en Calculation of parameter u step by the angle of deviation of normal 
  double  DeviationStepV( double u, double v, double ang ) const override; // \ru Вычисление шага параметра v по углу отклонения нормали \en Calculation of parameter v step by the angle of deviation of normal 
  /** \} */

  /** \ru \name Общие функции поверхности
      \en \name Common functions of surface
      \{ */
  size_t  GetUMeshCount() const override; // \ru Выдать количество полигонов по u. \en Get the count of polygons by u. 
  size_t  GetVMeshCount() const override; // \ru Выдать количество полигонов по v. \en Get the count of polygons by v. 

  void    ChangeCarrier( const MbSpaceItem & item, MbSpaceItem & init ) override; // \ru Изменение носителя \en Changing of carrier 
  bool    IsLineU() const override; // \ru Если true все производные по U выше первой равны нулю \en If true, then all the derivatives by U higher the first one are equal to zero 
  bool    IsLineV() const override; // \ru Если true все производные по V выше первой равны нулю \en If true, then all the derivatives by V higher the first one are equal to zero 

  // \ru Найти ближайшую проекцию точки на поверхность или ее продолжение по заданному начальному приближению. \en Find the neares projection of a point onto the surface. 
  bool    NearPointProjection  ( const MbCartPoint3D & pnt, double & u, double & v, bool ext, MbRect2D * uvRange = nullptr ) const override;
  // \ru Найти все проекции точки на поверхность вдоль вектора в любом из двух направлений. \en Find all a point projection onto the surface along a vector in either of two directions. 
  void    DirectPointProjection( const MbCartPoint3D & pnt, const MbVector3D & vect, SArray<MbCartPoint> & uv, bool ext, MbRect2D * uvRange = nullptr ) const override;

  MbCurve3D * CurveU( double v, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии v = const. \en Spatial copy of 'v = const'-line. 
  MbCurve3D * CurveV( double u, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии u = const. \en Spatial copy of 'u = const'-line. 

  // \ru Оффсет на поверхность. \en Offset to the surface 
  MbSurface * Offset( double d, bool same ) const override;

  /** \brief \ru Проверить параметры. Аналог глобальной функции _CheckParams, оптимизированный под использование кэшей.
             \en Check parameters. Analogue of the global function _CheckParams, optimized for caches usage.  \~
    \details \ru Проверить параметры и загнать в область определения, если параметр вышел за полюс.
             \en Check parameters and move them inside domain if parameter is out of pole. \~
    \param[in] surface - \ru Поверхность. \en Surface. \~
    \param[in] u -       \ru Первый параметр. \en First parameter. \~
    \param[in] v -       \ru Второй параметр. \en Second parameter. \~
  */
  void CheckSurfParams( double & u, double & v ) const override;

  /// \ru Вернуть количество кривых в первом семействе. \en Get count of curves of first family. 
  size_t GetUCurvesCount() const { return uCurves.Count(); } 
  /// \ru Вернуть количество кривых во втором семействе. \en Get count of curves of second family.  
  size_t GetVCurvesCount() const { return vCurves.Count(); } 

  /** \brief \ru Получить кривую с индексом  ind из первого семейства.
             \en Get curve with 'ind' index from first family. \~
      \details \ru Получить кривую с индексом  ind из первого семейства. \n
               \en Get curve with 'ind' index from first family. \n \~
      \param[in] ind - \ru Номер запрашиваемой кривой в массиве.
                       \en Index of required curve in array. \~
      \return \ru Кривая или nullptr, если значение ind выходит за диапазон возможных индексов массиве кривых.
              \en Curve or nullptr if value of 'ind' is out of range of possible indices of array of curves. \~
  */
  const MbCurve3D * GetUCurve( size_t ind ) const { return ( ind < uCurves.Count()) ? uCurves[ind] : nullptr; }

  /** \brief \ru Получить кривую с индексом  ind из второго семейства.
             \en Get curve with 'ind' index from second family. \~
      \details \ru Получить кривую с индексом  ind из второго семейства. \n
               \en Get curve with 'ind' index from second family. \n \~
      \param[in] ind - \ru Номер запрашиваемой кривой в массиве.
                       \en Index of required curve in array. \~
      \return \ru Кривая или nullptr, если значение ind выходит за диапазон возможных индексов массиве кривых.
              \en Curve or nullptr if value of 'ind' is out of range of possible indices of array of curves. \~
  */
  const MbCurve3D * GetVCurve( size_t ind ) const { return ( ind < vCurves.Count()) ? vCurves[ind] : nullptr; }
  
  /** \brief \ru Получить значение параметра, соответствующего кривой с индексом  ind из первого семейства.
             \en Get value of parameter corresponding to curve with 'ind' index from first family. \~
      \details \ru Получить значение параметра, соответствующего кривой с индексом  ind из первого семейства.\n
               \en Get value of parameter corresponding to curve with 'ind' index from first family.\n \~
      \param[in] ind - \ru Номер кривой в массиве.
                       \en Index of curve in array. \~
      \return \ru Значение параметра или 0, если значение ind выходит за диапазон возможных индексов массиве кривых.
              \en Value of parameter or 0 if value of 'ind' is out of range of possible indices of array of curves. \~ 
  */
  double      GetUParam( size_t ind ) const { return ( ind < uParams.Count()) ? uParams[ind] : 0; }

  /** \brief \ru Получить значение параметра, соответствующего кривой с индексом  ind из второго семейства.
             \en Get value of parameter corresponding to curve with 'ind' index from second family. \~
      \details \ru Получить значение параметра, соответствующего кривой с индексом  ind из второго семейства.\n
               \en Get value of parameter corresponding to curve with 'ind' index from second family.\n \~
      \param[in] ind - \ru Номер кривой в массиве.
                       \en Index of curve in array. \~
      \return \ru Значение параметра или 0, если значение ind выходит за диапазон возможных индексов массиве кривых.
              \en Value of parameter or 0 if value of 'ind' is out of range of possible indices of array of curves. \~ 
  */
  double      GetVParam( size_t ind ) const { return ( ind < vParams.Count()) ? vParams[ind] : 0; }

  /** \brief \ru Получить указатель на функцию перехода к параметрам первого семейства, соответствующую кривой с 
                 индексом ind из первого семейства.
             \en Get a transformation to parameters of first family corresponding to curve with 'ind' index from 
                 first family. \~
      \details \ru Получить указатель на функцию перехода к параметрам первого семейства, соответствующую кривой с 
                   индексом ind из первого семейства.\n
               \en Get a transformation to parameters of first family corresponding to curve with 'ind' index from 
                   first family\n \~
      \param[in] ind - \ru Номер кривой в массиве.
                       \en Index of curve in array. \~
      \return \ru Указатель на функцию перехода к параметру или nullptr, если значение ind выходит за диапазон
                  возможных индексов массива функций перехода к параметрам первого семейства.
              \en Pointer to transformation to parameter or nullptr if value of 'ind' is out of range of possible
                  indices of array of transformation to parameters of first family. \~ 
  */
  const MbFunction * GetTUParam( size_t ind ) const { return ( ind < tuParams.Count()) ? tuParams[ind] : nullptr; }
  /** \brief \ru Получить указатель на функцию перехода к параметрам второго семейства, соответствующую кривой с 
                 индексом ind из второго семейства.
             \en Get a transformation to parameters of second family corresponding to curve with 'ind' index from 
                 second family. \~
      \details \ru Получить указатель на функцию перехода к параметрам второго семейства, соответствующую кривой с 
                   индексом ind из второго семейства.\n
               \en Get a transformation to parameters of second family corresponding to curve with 'ind' index from 
                   second family\n \~
      \param[in] ind - \ru Номер кривой в массиве.
                       \en Index of curve in array. \~
      \return \ru Указатель на функцию перехода к параметру или nullptr, если значение ind выходит за диапазон
                  возможных индексов массива функций перехода к параметрам второго семейства.
              \en Pointer to transformation to parameter or nullptr if value of 'ind' is out of range of possible
                  indices of array of transformation to parameters of second family. \~ 
  */
  const MbFunction * GetTVParam( size_t ind ) const { return ( ind < tvParams.Count()) ? tvParams[ind] : nullptr; }
  /** \brief \ru Заполнить массив параметров по u.
             \en Fill array of parameters by u. \~
      \details \ru Заполнить массив параметров по u.\n
               \en Fill array of parameters by u.\n \~
      \param[in,out] params - \ru Множество параметров.
                              \en Set of parameters. \~
  */
  void        GetUParams( SArray<double> & params ) const { params = uParams; }

  /** \brief \ru Заполнить массив параметров по v.
             \en Fill array of parameters by v. \~
      \details \ru Заполнить массив параметров по v.\n
               \en Fill array of parameters by v.\n \~
      \param[in,out] params - \ru Множество параметров.
                              \en Set of parameters. \~
  */
  void        GetVParams( SArray<double> & params ) const { params = vParams; }

   /** \brief \ru Проверить выставленный тип сопряжения.
              \en Check mating type. \~
      \details \ru Проверить выставленный тип сопряжения. \n
               \en Check mating type.\n \~
      \param[in] t - \ru Проверяемый тип сопряжения.
                     \en Mating type to check. \~
      \param[in] n - \ru Номер сопряжения (от 0 до 3).
                     \en Mating type number (0 to 3). \~
      \return \ru Возвращает true, если такой тип сопряжения установлен.
              \en Returns true if this mating type is set. \~
  */
  bool        IsMatingType( MbeMatingType t, size_t n ) const;

  /** \brief   \ru Получить версию алгоритма расчета поверхности.
               \en Get version of the algorithm for calculating the surface. \~
      \details \ru Получить версию алгоритма расчета поверхности.\n
               \en Get version of the algorithm for calculating the surface.\n \~
      \return  \ru Версию алгоритма расчета поверхности.
               \en The version of the algorithm for calculating the surface. \~ 
  */
  MbeMeshSurfaceVersion GetSurfaceVersion() const { return version; }

  /** \brief   \ru Проверить точку поверхности на неопределенность смешанных производных.
               \en Check the surface point for the uncertainty of mixed derivatives. \~
      \details \ru Имеется ввиду рассогласование смешанных производных в вершинах Грэгори. \n
               \en This means the mismatch of mixed derivatives at the Gregory vertices.\n \~
      \param[in] pnt -        \ru Проверяемый точка. \en Point to check. \~
      \param[in] uEps, vEps - \ru Точность сравнения с сингулярной точкой. 
                              \en Accuracy of comparison with a singular point. \~
      \return  \ru Возвращает true для точки сингулярности.
               \en Returns true for the singularity point. \~ 
  */
  bool        IsSingularTwists( const MbCartPoint & pnt, double uEps, double vEps ) const;

private:
  void    AddCurvesRef();
  void    ReleaseCurves();
  // \ru Инициализация из конструктора. \en Initialization from the constructor.
  void    Init( RPArray<MbCurve3D> & initU, RPArray<MbCurve3D> & initV, SArray<double> * parsU, SArray<double> * parsV,
                bool uClosed, bool vClosed, bool g2, bool same, const SArray<uint> * types, MbeMeshSurfaceVersion vers );
  void    Init( bool calcParams, bool callFromMultiPatchGenerator, 
                c3d::DoubleVector * tuCurvePars, c3d::DoubleVector * tvCurvePars );
  bool    CheckPoles( MbMeshSurfaceAuxiliaryData * ) const; // \ru Инициализировать полюсы на границе параметрической области. \en Initialize poles on the border of parameters area.
         
  // \ru Аналоги публичных функций для внутреннего использования (используют присланный кэш). \en Analongs of public functions for internal use (use the sent cache).
  bool    GetPoleUMin( MbMeshSurfaceAuxiliaryData * ) const; // \ru Существует ли полюс на границе параметрической области \en Whether there is pole on boundary of parametric region 
  bool    GetPoleUMax( MbMeshSurfaceAuxiliaryData * ) const; // \ru Существует ли полюс на границе параметрической области \en Whether there is pole on boundary of parametric region 
  bool    GetPoleVMin( MbMeshSurfaceAuxiliaryData * ) const; // \ru Существует ли полюс на границе параметрической области \en Whether there is pole on boundary of parametric region 
  bool    GetPoleVMax( MbMeshSurfaceAuxiliaryData * ) const; // \ru Существует ли полюс на границе параметрической области \en Whether there is pole on boundary of parametric region 
  void   _DeriveU( double u, double v, MbVector3D & p, MbMeshSurfaceAuxiliaryData * ) const; // \ru Первая производная по u \en First derivative with respect to u 
  void   _DeriveV( double u, double v, MbVector3D & p, MbMeshSurfaceAuxiliaryData * ) const; // \ru Первая производная по v \en First derivative with respect to v 
  void    DeriveU( double & u, double & v, MbVector3D & p, MbMeshSurfaceAuxiliaryData * ) const; // \ru Первая производная по u \en First derivative with respect to u 
  void    DeriveV( double & u, double & v, MbVector3D & p, MbMeshSurfaceAuxiliaryData * ) const; // \ru Первая производная по v \en First derivative with respect to v 
  void    DeriveUU( double & u, double & v, MbVector3D & p, MbMeshSurfaceAuxiliaryData * ) const; // \ru Вторая производная по u \en Second derivative with respect to u 
  void    DeriveVV( double & u, double & v, MbVector3D & p, MbMeshSurfaceAuxiliaryData * ) const; // \ru Вторая производная по v \en Second derivative with respect to v 
  void    DeriveUV( double & u, double & v, MbVector3D & p, MbMeshSurfaceAuxiliaryData * ) const; // \ru Вторая производная по uv \en Second derivative with respect to uv 
  void    DeriveUUU( double & u, double & v, MbVector3D & p, MbMeshSurfaceAuxiliaryData * ) const; // \ru Третья производная \en Third derivative 
  void    DeriveUUV( double & u, double & v, MbVector3D & p, MbMeshSurfaceAuxiliaryData * ) const; // \ru Третья производная \en Third derivative 
  void    DeriveUVV( double & u, double & v, MbVector3D & p, MbMeshSurfaceAuxiliaryData * ) const; // \ru Третья производная \en Third derivative 
  void    DeriveVVV( double & u, double & v, MbVector3D & p, MbMeshSurfaceAuxiliaryData * ) const; // \ru Третья производная \en Third derivative 
  bool    IsLineU( MbMeshSurfaceAuxiliaryData * ) const; // \ru Если true все производные по U выше первой равны нулю \en If true, then all the derivatives by U higher the first one are equal to zero 
  bool    IsLineV( MbMeshSurfaceAuxiliaryData * ) const; // \ru Если true все производные по V выше первой равны нулю \en If true, then all the derivatives by V higher the first one are equal to zero 
  bool    IsLine( bool dirU ) const; // \ru Проверка поверхности на линейность в указанном направлении.\en Checking the surface for linearity in the specified direction.
  void   _PointOn( double u, double v, MbCartPoint3D & p, MbMeshSurfaceAuxiliaryData * ) const; // \ru Точка на поверхности \en Point on the surface 
  void    PointOn( double & u, double & v, MbCartPoint3D & p, MbMeshSurfaceAuxiliaryData * ) const; // \ru Точка на поверхности \en Point on the surface 

  // \ru Определить местные координаты области поверхности. \en Determine local coordinates of surface region. 
  void    LocalCoordinate( double u, double v, double & ul, double & vl, size_t & i0,size_t & j0,size_t & i1, size_t & j1, MbMeshSurfaceAuxiliaryData * ucache = nullptr ) const;
  void    LocalCoordinate_v2( double u, double v, double & ul, double & vl, size_t & i0, size_t & j0, size_t & i1, size_t & j1, size_t ord, MbMeshSurfaceAuxiliaryData * ucache ) const;
  void    LocalCoordinate_v4( double u, double v, size_t ordU, size_t ordV, MbMeshSurfaceAuxiliaryData * ucache ) const;
  void    LocalCoordinate_v5( double u, double v, size_t ordU, size_t ordV, MbMeshSurfaceAuxiliaryData * ucache ) const;
          // \ru Рассчитать параметры для границы патча. \en Calculate parameters for the patch boundary.
  void    PatchBoundExplore_v4( const MbCurve3D * curve, const MbFunction * fn,
                                double par, size_t bnd, MbeMatingType tp, MbeTransversCalculationType ttp,
                                MbCoonsPatchData & pd, ptrdiff_t ord, ptrdiff_t tOrd ) const;
  // \ru Вернуть изолинию на поверхности для указанного параметра.
  MbCurve3D * CurveUV_v4( bool dirU, double sPar, MbRect1D * pRgn, bool bApprox ) const; // \ru Пространственная копия линии u(v) = const. \en Spatial copy of line u(v) = const. 

  // \ru Вычислить вспомогательные вектора производных вдоль U кривых патча. \en Calculate auxiliary vectors of derivatives along U curves of patch. 
  void    CalculateAlongU( const double & ul, const size_t & j0, const size_t & j1, MbMeshSurfaceAuxiliaryData * ucache ) const;
  void    CalculateAlongU_v2( const double & u, const size_t & j0, const size_t & j1, size_t indP, MbMeshSurfaceAuxiliaryData * ucache ) const;
  // \ru Вычислить вспомогательные вектора производных вдоль V кривых патча. \en Calculate auxiliary vectors of derivatives along V curves of patch. 
  void    CalculateAlongV( const double & vl, const size_t & i0, const size_t & i1, MbMeshSurfaceAuxiliaryData * ucache ) const;
  void    CalculateAlongV_v2( const double & v, const size_t & i0, const size_t & i1, size_t indP, MbMeshSurfaceAuxiliaryData * ucache ) const;
  // \ru Вычислить вспомогательные вектора производных в узлах кривых. \en Calculate auxiliary vectors of derivatives at nodes of curves. 
  void    CalculateVertex( const size_t & i0, const size_t & j0, const size_t & i1, const size_t & j1, MbMeshSurfaceAuxiliaryData * ucache ) const;
  void    CalculateVertex_v2( const size_t & i0, const size_t & j0, const size_t & i1, const size_t & j1, MbMeshSurfaceAuxiliaryData * ucache ) const;
  // \ru Вычислить дополнительные вспомогательные вектора производных в узлах кривых. Для 1-й версии поверхности. \en Calculate additional auxiliary vectors of derivatives at nodes of curves. For 1-st version of surface. 
  void    AdditionalCalculateVertex( size_t i0, size_t j0, size_t i1, size_t j1, MbMeshSurfaceAuxiliaryData * ucache ) const;
  // \ru Создать массив смешанных производных. \en Create an array of mixed derivatives. 
  void    CreateTwists   ();
  void    CreateTwists_v1( const MapCrosses & crosses, const MapCrosses & outCrosses );
  void    CreateTwists_v4( const MapCrosses & crosses, const MapCrosses & outCrosses );
  // \ru Создать массив смешанных производных. \en Create an array of mixed derivatives. 
  void    InitTwistsArrays();
  // \ru Подготовить смешанные производные на границах сопряжения. \en Prepare mixed derivatives at the boundaries of the mating.
  void    PrepareBoundaryTwists_v4( bool g2, bool read );
  void    PrepareBoundaryTwists_v5( MbMeshSurface *( *adjSurf )[3][3], size_t ord );
  // \ru Определить тип интерполяции для границ сопряжения. \en Set interpolation type for mating boundaries.
  void    SetTransversalTypes_v5( size_t ord );
  // \ru Рассчитать смешанные производные более высокого пояркда
  //     по разностной схеме с поверхности более низкого порядка.
  void    CalculateTwists_v4( MbMeshSurface *(*adjSurf)[3], SArray<MbVector3D> (*extBounds)[4], size_t ord );
  void    CalculateTwists_v5( MbMeshSurface *( *adjSurf )[3][3], SArray<MbVector3D>( *extBounds )[4], bool g2 );
  // \ru Выполнить преобразование по матрице векторов boundTwists.
  // \en Perform a transformation on the matrix of vectors boundTwists.
  void    TransformTwists_v5( const MbMatrix3D & matr );
  // \ru Проверить гладкость стыковки с соседней поверхностью по указанной кривой.
  // \en Check the smoothness of the connection with the adjacent surface according to the specified curve.
  bool    CheckSmoothnessOfCurve( bool byU, size_t curveInd, bool isBegin, const MbMeshSurface & adjSurf, bool checkC2 ) const;
  // \ru Получить данные кэша. \en Get cache data.
  MbCoonsPatchData * GetPatchData_v4();
  const MbCoonsPatchData * GetPatchData_v4() const;

  // \ru Аппроксимировать смешанную производную. \en Approximate mixed derivative. 
  void    ApproxTwistBilinear   ( size_t iL, size_t iR, size_t jD, size_t jU, size_t iCent, size_t jCent, MbVector3D & resTwist );
  void    ApproxTwistBilinear_v1( size_t iCent, size_t jCent, MbVector3D & resTwist );
  void    ApproximateOneCornerTwist_v1( size_t iL, size_t iR, size_t jD, size_t jU, size_t corner, MbVector3D & resTwist ) const;
  void    ApproxTwistBilinear_v3( ptrdiff_t i, ptrdiff_t j, MbVector3D & resTwist, const MapCrosses & crosses, const MapCrosses & outCrosses );
  // \ru Рассчитать частную производную в вершине ячейки. \en Calculate the partial derivative at the top of the cell.
  void    CalculateCellDerivative_v3( MbVector3D & res, bool dirU, ptrdiff_t uind, ptrdiff_t vind, bool isOut,
                                      const MapCrosses & crosses, const MapCrosses & outCrosses ) const;
  // \ru Рассчитать координаты вершины ячейки. \en Calculate cell vertex coordinates.
  void    CalculateCellPoint_v3( MbCartPoint3D & res, ptrdiff_t uind, ptrdiff_t vind, const MapCrosses & crosses ) const;
  // \ru Вычислить вспомогательные массивы трансверсальных производных. \en Calculate auxiliary arrays of transversal derivatives. 
  void    CalculateTransDiffs   ( double ul, double vl, size_t i0, size_t j0, size_t i1, size_t j1, MbMeshSurfaceAuxiliaryData * ucache ) const;
  // \ru Вычислить выводящую производную с линии V = const и ее первые производные по U. \en Calculate leading out derivative from 'V = const'-line and its first derivatives by U. 
  void    CalculateVDiffs   ( double ul, double vl, size_t i0, size_t j0, bool dir,
                              MbVector3D & res,         
                              MbVector3D & resDer1,
                              MbVector3D & resDer2 ) const;
  // \ru Вычислить выводящую производную с линии U = const и ее первые производные по V. \en Calculate leading out derivative from 'U = const'-line and its first derivatives by V. 
  void    CalculateUDiffs   ( double ul, double vl, size_t i0, size_t j0, bool dir,
                              MbVector3D & res,         
                              MbVector3D & resDer1,
                              MbVector3D & resDer2 ) const;
  // \ru Вычислить выводящую производную с линии U (V) = const и ее первые производные по V (U). \en Calculate leading out derivative from 'U (V) = const'-line and its first derivatives by V (U). 
  void    CalcTransvDiffs_v1( bool uDir, double par, size_t ind, bool leftOrDown,
                              MbVector3D & res,         
                              MbVector3D & resDer1,
                              MbVector3D & resDer2,
                              MbVector3D & resDer3, MbMeshSurfaceAuxiliaryData * ucache ) const;
  void    CalcTransvDiffs_v2( bool uDir, double par, double apar, size_t ind, size_t indt, bool leftOrDown,
                              MbVector3D & res,         
                              MbVector3D & resDer1,
                              MbVector3D & resDer2,
                              MbVector3D & resDer3, size_t ord, MbMeshSurfaceAuxiliaryData * ucache ) const;
  // \ru Аппроксимация нормали вдоль U - линии. \en Approximation of normal along U - line. 
  bool    NormalAlongV   ( double ul, double vl, size_t i0, size_t j0, bool dir,
                           MbVector3D & res,         
                           MbVector3D & resDer1,
                           MbVector3D & resDer2 ) const;
  // \ru Аппроксимация нормали вдоль V - линии. \en Approximation of normal along V - line. 
  bool    NormalAlongU   ( double ul, double vl, size_t i0, size_t j0, bool dir,
                           MbVector3D & res,         
                           MbVector3D & resDer1,
                           MbVector3D & resDer2 ) const;
  // \ru Вычислить выводящую производную с линии V = const и ее первые производные по U. \en Calculate leading out derivative from 'V = const'-line and its first derivatives by U. 
  void    NormalVDiffs   ( double ul, double vl, size_t i0, size_t j0, bool dir,
                           MbVector3D & res,         
                           MbVector3D & resDer1,
                           MbVector3D & resDer2 ) const;
  // \ru Вычислить выводящую производную с линии U = const и ее первые производные по V. \en Calculate leading out derivative from 'U = const'-line and its first derivatives by V. 
  void    NormalUDiffs   ( double ul, double vl, size_t i0, size_t j0, bool dir,
                           MbVector3D & res,         
                           MbVector3D & resDer1,
                           MbVector3D & resDer2 ) const;
  // \ru Вычислить выводящие производные с учетом сопряжения к поверхности. \en Calculate leading out derivatives with consideration of conjugation to surface. 
  void    SurfaceDiff    ( const MbCurve3D & srfCrv, uint type, double ul, double vl, 
                           size_t i0, size_t j0, size_t i1, size_t j1, 
                           bool         leftOrDown, // \ru Где происходит сам стык. \en Where is a joint. 
                           bool         uDir,       // \ru Вдоль какого направления направлена кривая. \en Which direction the curve is directed along. 
                           MbVector3D & first,
                           MbVector3D & secnd,
                           MbVector3D & third, MbMeshSurfaceAuxiliaryData * ucache ) const;
  // \ru Вычислить выводящую производную и ей сопутствующие производные вдоль кривой. \en Calculate leading out derivative and its associated derivatives along curve. 
  void    SurfaceTangent ( const MbCurve3D  & surfCrv,    // \ru Кривая, к поверхности которой вычисляется производная \en Curve which surface the derivative is calculated to   
                           const MbVector3D * coons,      // \ru Массив выводящих производных обычного патча Кунса \en Array of leading out derivatives of ordinary Coons patch 
                                 size_t       border,     // \ru Порядковый номер сопрягаемой границы \en Serial number of conjugated boundary 
                                 double       tCurve,     // \ru Параметр на кривой \en Parameter on the curve 
                                 double       paramLoc,   // \ru Параметр патча, соответствующий параметру на кривой \en Patch parameter corresponding to parameter on curve 
                                 double       dt,         // (dt / d(paramLoc))
                                 MbVector3D & res,        // \ru Сам вектор \en Vector 
                                 MbVector3D & resDiff,    // \ru Его первая производная \en Its first derivative        
                                 MbVector3D & resDiff2, MbMeshSurfaceAuxiliaryData * ucache ) const;
  void    PureSurfTangent( const MbCurve3D  & surfCrv, 
                           const MbVector3D * coons,
                                 size_t       border,
                                 double       tCurve,     
                                 double       dt,         
                                 MbVector3D & res,        
                                 MbVector3D & resDiff,   
                                 MbVector3D * resDiff2 ) const;
  void    SurfaceNormal  ( const MbCurve3D  & srfCrv,
                           const MbVector3D * coons,   // \ru Массив выводящих производных обычного патча Кунса \en Array of leading out derivatives of ordinary Coons patch 
                                 size_t       border,  // \ru Порядковый номер сопрягаемой границы \en Serial number of conjugated boundary 
                                 double       tCurve,  
                                 double       paramLoc,
                                 double       dt,
                                 MbVector3D & res,     // \ru Сам вектор \en Vector 
                                 MbVector3D & resDiff, // \ru Производная вектора вдоль кривой ( по paramLoc ) \en Derivative of vector along curve ( by paramLoc ) 
                                 MbVector3D & resDiff2, MbMeshSurfaceAuxiliaryData * ucache ) const ;
  void    PureSurfNormal ( const MbCurve3D & surfCrv, 
                           const MbVector3D * coons,
                                 size_t       border,
                                 double       tCurve,     
                                 double       dt,         
                                 MbVector3D & res,        
                                 MbVector3D & resDiff,   
                                 MbVector3D * resDiff2 ) const;
  // \ru Нормализовать массивы пересечений \en Normalize arrays of intersections 
  void    NormalizeIntersection(); 

  // \ru Определить индексы в массиве точек пересечения для кривых по направлению U \en Determine indices in array of intersection points for curves by U direction 
  void    DefineEndTUIndices( size_t i0, size_t j0, size_t i1, size_t j1,
                              size_t & k0min, size_t & k0max,
                              size_t & k2min, size_t & k2max ) const;
  // \ru Определить индексы в массиве точек пересечения для кривых по направлению V \en Determine indices in array of intersection points for curves by V direction 
  void    DefineEndTVIndices( size_t i0, size_t j0, size_t i1, size_t j1,
                              size_t & k1min, size_t & k1max, 
                              size_t & k3min, size_t & k3max ) const;

  // \ru Определить параметры пересечений для U направления \en Determine parameters of intersections for U direction 
  void    DefineEndTUPars( size_t i0, size_t j0, size_t i1, size_t j1,
                           double & t0min, double & t0max,
                           double & t2min, double & t2max, bool dir = true ) const;          
  // \ru Определить параметры пересечений для V направления \en Determine parameters of intersections for V direction 
  void    DefineEndTVPars( size_t i0, size_t j0, size_t i1, size_t j1, 
                           double & t1min, double & t1max,
                           double & t3min, double & t3max, bool dir = true ) const;
          
  void    ExactNormal( double u, double v, const MbVector3D & uDer, const MbVector3D & vDer, MbVector3D & nor, MbMeshSurfaceAuxiliaryData * ) const;

  // \ru Проверить параметры и в случае выхода за пределы загнать в область определения.
  // \en Check parameters and if it is out of limits, then drive it to domain 
  void    CheckParams( double & u, double & v ) const;
  // \ru Проверить параметры и в случае захода за полюс или выходе за период загнать в область определения.
  // \en Check parameters and if it is out of pole or it is out of period, then drive it to the domain region. 
  void   CheckParamsEx( double & u, double & v, MbMeshSurfaceAuxiliaryData * ucache ) const;
  // \ru Попытаться вычислить шаг по U, исходя из шагов по соответствующим операторам Loft-ов \en Try to calculate step by U through steps of corresponding Loft operators  
  bool    SurfDeviationStepU( double & u, double & v, double ang, double & resStep, MbMeshSurfaceAuxiliaryData * ucache ) const; 
  // \ru Попытаться вычислить шаг по V, исходя из шага по соответствующим операторам Loft-ов \en Try to calculate step by V through steps of corresponding Loft operators  
  bool    SurfDeviationStepV( double & u, double & v, double ang, double & resStepv, MbMeshSurfaceAuxiliaryData * ucache ) const;
  // \ru Создать таблицу пересечений для дополнительных кривых. \en Create an intersection table for additional curves.
  void    CreateCurvesCrossTable( MapCrosses & crosses, MapCrosses & outCrosses );
  // \ru Создать функции перехода к параметрам кривых. \en Create functions for mapping to curve parameters.
  void    CreateParamFunctions( bool dirU, const MapCrosses & crosses );
  // \ru Расчет производных в точке для образующей кривой. \en Calculation of derivatives at a point for a generating curve.
  void    GeneratrixCurveExplore_v3( bool dirU, size_t ind, double t, 
                                      MbCartPoint3D & p, MbVector3D & fir, MbVector3D & sec, MbVector3D & thir ) const;
  // \ru Инициализировать массивы расширения. \en Init expansion arrays.
  void    InitExtArrays( const bool (*adjPatch)[4] = nullptr );

  /** \} */

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbMeshSurface )
OBVIOUS_PRIVATE_COPY( MbMeshSurface )
}; // MbMeshSurface

IMPL_PERSISTENT_OPS( MbMeshSurface )


//------------------------------------------------------------------------------
// \ru Проверить параметры и в случае выхода за пределы загнать в область определения \en Check parameters and if it is out of limits, then drive it to domain 
// ---
inline void MbMeshSurface::CheckParams( double & u, double & v ) const
{
  if ( (u < umin) || (u > umax) ) {
    if ( uclosed ) { 
      double rgn = umax - umin;
      u -= ::floor( (u - umin) / rgn ) * rgn;
    }
    else if ( u < umin )
      u = umin;
    else if ( u > umax )
      u = umax;
  }
  if ( (v < vmin) || (v > vmax) ) {
    if ( vclosed ) {
      double rgn = vmax - vmin;
      v -= ::floor( (v - vmin) / rgn ) * rgn;
    }
    else if ( v < vmin )
      v = vmin;
    else if ( v > vmax )
      v = vmax;
  }
}


//------------------------------------------------------------------------------
// \ru Проверить параметры и в случае захода за полюс или выходе за период загнать в область определения. \en Check parameters and if it is out of pole or it is out of period, then drive it to the domain region. 
// ---
inline void MbMeshSurface::CheckParamsEx( double & u, double & v, MbMeshSurfaceAuxiliaryData * ucache ) const
{
  MbMeshSurfaceAuxiliaryData * loccache = ucache != nullptr ? ucache : cache();

  if ( loccache->data == nullptr )
    loccache->data = new MbSurfaceContiguousData;

  if ( loccache->data->PoleUMin() == spt_Undefined )
    CheckPoles( loccache );
  if ( loccache->data->PoleUMin() == spt_Point ) {
    const double & umin_ = uParams[0];
    if ( u < umin_ )
      u = umin_;
  }
  if ( loccache->data->PoleUMax() == spt_Point ) {
    const double & umax_ = uParams[uParams.MaxIndex()];
    if ( u > umax_ )
      u = umax_;
  }
  if ( loccache->data->PoleVMin() == spt_Point ) {
    const double & vmin_ = vParams[0];
    if ( v < vmin_ )
      v = vmin_;
  }
  if ( loccache->data->PoleVMax() == spt_Point ) {
    const double & vmax_ = vParams[vParams.MaxIndex()];
    if ( v > vmax_ )
      v = vmax_;
  }
  if ( uclosed && (u > umax || u < umin) ) {
    double period = umax - umin;
    if ( period < Math::paramAccuracy )
      period = Math::paramAccuracy;
    u -= period * ::floor( (u - umin)/period );
  }
  if ( vclosed && (v > vmax || v < vmin) ) {
    double period = vmax - vmin;
    if ( period < Math::paramAccuracy )
      period = Math::paramAccuracy;
    v -= period * ::floor( (v - vmin)/period );
  }
}


#endif // __SURF_MESH_SURFACE_H
