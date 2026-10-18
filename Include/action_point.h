////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Функции создания точек.
         \en Functions for points creation. \~
  \details \ru Функции, использующие в качестве выходных параметров точки или массивы точек.
           \en Functions that take points or arrays of points as input parameters. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ACTION_POINT_H
#define __ACTION_POINT_H


#include <templ_s_array.h>
#include <mb_operation_result.h>
#include <mb_enum.h>
#include <mb_cart_point.h>
#include <mb_cart_point3d.h>
#include <cur_line.h>
#include <mb_variables.h>


class    MbCurve;
class    MbLineSegment;
class    MbLine3D;
class    MbCurve3D;
class    MbSurface;
class    MbFaceShell;


//------------------------------------------------------------------------------
/** \brief \ru Создать массив.
           \en Create an array. \~
  \details \ru Создать массив с контролем выделения памяти. \n
           \en Create an array with memory allocation control. \n \~
  \param[in] cnt - \ru Количество элементов массива.
                   \en Number of elements in the array. \~
  \param[out] res - \ru Результат операции.
                    \en The operation result. \~
  \return \ru Возвращает массив элементов, если он создан, или nullptr в противном случае.
          \en Returns an array of elements if it has been created, otherwise returns nullptr. \~
  \ingroup Algorithms_3D
*/
// ---
template <class Type>
inline SArray<Type> * CreateArray( size_t cnt, MbResultType & res )
{
  SArray<Type> * arr = new SArray<Type> ( cnt, 1 );
  if ( arr != nullptr && arr->GetAddr() == nullptr ) {
    delete arr;
    arr = nullptr;
  }
  if ( arr == nullptr )
    res = rt_TooManyPoints;

  return arr;
}


//------------------------------------------------------------------------------
/** \brief \ru Выделить в массиве память под n элементов.
           \en Allocate memory in the array for n elements. \~
  \details \ru Выделить в массиве память под n элементов с контролем выделения памяти. \n
           \en Allocate memory in the array for n elements with memory allocation control. \n \~
  \param[in, out] arr - \ru Массив.
                        \en An array. \~
  \param[in] n - \ru Количество элементов, под которые нужно выделить память.
                 \en Number of elements for allocation. \~
  \param[out] res - \ru Результат операции.
                    \en The operation result. \~
  \return \ru Возвращает true в случае успешного выделения памяти.
          \en Returns true if the memory has been successfully allocated. \~
  \ingroup Algorithms_3D
*/
// ---
template <class Type>
inline bool ReserveArray( SArray<Type> & arr, size_t n, MbResultType & res )
{
  arr.Reserve( n );
  if ( arr.GetAddr() == nullptr ) {
    res = rt_TooManyPoints;
    return false;
  }
  return true;
}


//------------------------------------------------------------------------------
/** \brief \ru Добавить в массив элемент.
           \en Add an element to the array. \~
  \details \ru Добавить в массив элемент с контролем выделения памяти. \n
           \en Add an element to the array with memory allocation control. \n \~
  \param[in, out] arr - \ru Массив.
                        \en An array. \~
  \param[in] item - \ru Элемент, который нужно добавить.
                    \en The element to add. \~
  \param[out] res - \ru Результат операции.
                    \en The operation result. \~
  \return \ru Возвращает true в случае успешного добавления.
          \en Returns true if the element has been successfully added. \~
  \ingroup Algorithms_3D
*/
// ---
template <class Type>
inline bool AddItem( SArray<Type> & arr, const Type & item, MbResultType & res )
{
  arr.Add( item );
  if ( arr.GetAddr() == nullptr ) {
    res = rt_TooManyPoints;
    return false;
  }
  return true;
}


//------------------------------------------------------------------------------
/** \brief \ru Пространственно-параметрическая точка.
           \en A space-parametric point. \~
  \details \ru Пространственно-параметрическая точка. \n
    Содержит в себе трехмерную и двумерную точки.
           \en A space-parametric point. \n
    Contains a three-dimensional point and a two-dimensional point. \~
  \ingroup Point_Modeling
*/
// ---
class MATH_CLASS MbSpaceParamPnt {
protected:
  MbCartPoint3D spacePnt; ///< \ru Пространственная точка. \en A spatial point. 
  MbCartPoint   paramPnt; ///< \ru Параметрическая точка. \en A parametric point. 

public: // \ru Конструкторы \en Constructors 
  /// \ru Конструктор по пространственной точке. \en A constructor that takes a space point. 
  explicit MbSpaceParamPnt( const MbCartPoint3D & sp ) : spacePnt( sp ), paramPnt( UNDEFINED_DBL, 0.0 ) {}
  /// \ru Конструктор по пространственной и параметрической точкам. \en A constructor that takes a space point and a parametric point. 
  explicit MbSpaceParamPnt( const MbCartPoint3D & sp, const MbCartPoint & pp ) : spacePnt( sp ), paramPnt( pp ) {}
  /// \ru Конструктор по пространственно-параметрической точке. \en A constructor that takes a space-parametric point. 
  explicit MbSpaceParamPnt( const MbSpaceParamPnt & cp ) : spacePnt( cp.spacePnt ), paramPnt( cp.paramPnt ) {}
  ~MbSpaceParamPnt() {}

public: // \ru Инициализация \en The initialization 
  /// \ru Инициализация по пространственно-параметрической точке. \en Initialization with a space-parametric point. 
  void        Init( const MbSpaceParamPnt & cp ) { spacePnt = cp.spacePnt; paramPnt = cp.paramPnt; }
  /// \ru Инициализация по пространственной и параметрической точкам. \en Initialization with a space point and a parametric point. 
  void        Init( const MbCartPoint3D & sp, const MbCartPoint & pp ) { spacePnt = sp; paramPnt = pp; }
public: // \ru Функции \en Functions 
  /// \ru Установлена ли параметрическая точка? \en Whether the parametric point is specified. 
  bool        IsParamPnt() const { return (paramPnt.x != UNDEFINED_DBL); } //-V550
  /// \ru Перевести параметрическую точку в неустановленное состояние. \en Reset a parametric point. 
  void        ResetParamPnt() { paramPnt.x = UNDEFINED_DBL; }
  /// \ru Проверка на равенство параметрических точек по X с заданной погрешностью. \en Check if parametric points are equal by X component with the specified tolerance. 
  bool        IsParamEqualX( const MbSpaceParamPnt & cp, double eps ) const { return (::fabs(paramPnt.x - cp.paramPnt.x) < eps); }
  /// \ru Проверка на равенство параметрических точек по Y с заданной погрешностью. \en Check if parametric points are equal by Y component with the specified tolerance. 
  bool        IsParamEqualY( const MbSpaceParamPnt & cp, double eps ) const { return (::fabs(paramPnt.y - cp.paramPnt.y) < eps); }

  /// \ru Получить ссылку на пространственную точку. \en Get a reference to the space point. 
  const MbCartPoint3D & GetSpacePnt() const { return spacePnt; }
  /// \ru Получить ссылку на параметрическую точку. \en Get a reference to the parametric point. 
  const MbCartPoint   & GetParamPnt() const { return paramPnt; }

private: // \ru Нереализованные \en Not implemented 
  MbSpaceParamPnt();
  MbSpaceParamPnt( const MbCartPoint & );
  void        operator =  ( const MbCartPoint3D   & );
  void        operator =  ( const MbCartPoint     & );
  void        operator =  ( const MbSpaceParamPnt & );
  bool        operator == ( const MbSpaceParamPnt & ) const;
};


typedef std::pair<MbSpaceParamPnt,c3d::UintPair> MbLocPnt; ///< \ru Пространственно-параметрическая точка с индексированным положением. \en A space-parametric point with indexed position. 


//------------------------------------------------------------------------------
/** \brief \ru Создать точки на поверхности.
           \en Create points on a surface. \~
  \details \ru Создать группу точек на поверхности. \n
           \en Create a group of points on a surface. \n \~
  \param[in] surface - \ru Поверхность-источник.
                       \en The source surface. \~
  \param[in] stepType - \ru Тип шага по поверхности.
                        \en Type of spacing on a surface. \~
  \param[in] uValue - \ru Величина шага по u или количество точек по u при шаге по параметру
                      \en U-spacing value or number of points in u-direction while sampling by parameter \~
  \param[in] vValue - \ru Величина шага по v или количество точек по v при шаге по параметру.
                      \en V-spacing value or number of points in v-direction while sampling by parameter. \~
  \param[in] truncateByBounds - \ru Усечь границами поверхности.
                                \en Whether to truncate by surface boundary. \~
  \param[out] result - \ru Индексированные пространственно-параметрические точки.
                       \en Indexed space-parametric points. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (MbResultType) PointsOnSurface( const MbSurface &                   surface,
                                                MbeStepType                   stepType,
                                                double                        uValue,
                                                double                        vValue,
                                                bool                          truncateByBounds,
                                                RPArray< SArray<MbLocPnt> > & result );


//------------------------------------------------------------------------------
/** \brief \ru Создать точки на поверхности.
           \en Create points on a surface. \~
  \details \ru Создать группу точек на поверхности. \n
           \en Create a group of points on a surface. \n \~
  \param[in] surface - \ru Поверхность-источник.
                       \en The source surface. \~
  \param[in] gridType - \ru Тип сетки на поверхности.
                        \en A type of a grid on a surface. \~
  \param[in] uv0 - \ru Центральная точка сетки
                   \en The central point of the grid. \~
  \param[in] angle - \ru Угол поворота сетки относительно направления U (в радианах)
                     \en Rotation angle of the grid relative to U direction (in radians). \~
  \param[in] stepType - \ru Тип шага по поверхности.
                        \en Type of spacing on a surface. \~
  \param[in] step1 - \ru Величина шага по первому направлению
                     \en A spacing value in the first direction \~
  \param[in] step2 - \ru Величина шага по второму направлению
                     \en A spacing value in the second direction \~
  \param[in] truncateByBounds - \ru Усечь границами поверхности.
                                \en Whether to truncate by surface boundary. \~
  \param[out] result - \ru Индексированные пространственно-параметрические точки.
                       \en Indexed space-parametric points. \~
  \param[in] maxPntsCnt - \ru Максимально допустимое количество точек.
                          \en The maximal acceptable number of points. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (MbResultType) PointsOnSurface( const MbSurface &                   surface,
                                                MbeItemGridType &             gridType,
                                          const MbCartPoint &                 uv0,
                                                double                        angle,
                                                MbeStepType                   stepType,
                                                double                        step1,
                                                double                        step2,
                                                bool                          truncateByBounds,
                                                RPArray< SArray<MbLocPnt> > & result,
                                                size_t                        maxPntsCnt = c3d::ARRAY_MAX_COUNT );


//------------------------------------------------------------------------------
/** \brief \ru Определить разбивку поверхности по умолчанию.
           \en Define the default sampling of a surface. \~
  \details \ru Определить разбивку поверхности по умолчанию \n
           (вспомогательная функция для функции PointsOnSurface).
           \en Define the default sampling of a surface \n
           (an auxiliary function for function PointsOnSurface). \~
  \param[in] surface - \ru Исходная поверхность.
                       \en The initial surface. \~
  \param[out] uPntsCnt - \ru Количество разбиений по u.
                         \en The points number in U direction. \~
  \param[out] vPntsCnt - \ru Количество разбиений по v.
                         \en The points number in V direction. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (void) DefinePointsOnSurfaceCounts( const MbSurface & surface, 
                                                    size_t &    uPntsCnt, 
                                                    size_t &    vPntsCnt );


//------------------------------------------------------------------------------
/** \brief \ru Найти точку пересечения трех поверхностей.
           \en Calculate the intersection point of three surfaces. \~
  \details \ru Найти точку пересечения трех поверхностей по начальным приближениям. \n
           \en Calculate the intersection point of three surfaces given the initial estimates. \n \~
  \param[in] surf0 - \ru Первая поверхность.
                     \en The first surface. \~
  \param[in] ext0 - \ru Флаг поиска на продолжении первой поверхности.
                    \en Whether to use the extension of the first surface. \~
  \param[in] surf1 - \ru Вторая поверхность.
                     \en The second surface. \~
  \param[in] ext1 - \ru Флаг поиска на продолжении второй поверхности.
                    \en Whether to use the extension of the second surface. \~
  \param[in] surf2 - \ru Третья поверхность.
                     \en The third surface. \~
  \param[in] ext2 - \ru Флаг поиска на продолжении третьей поверхности.
                    \en Whether to use the extension of the third surface. \~
  \param[in,out] uv0 - \ru Начальное приближение и результат на поверхности surf0.
                       \en The initial approximation and the result on surface surf0. \~
  \param[in,out] uv1 - \ru Начальное приближение и результат на поверхности surf1.
                       \en The initial approximation and the result on surface surf1. \~
  \param[in,out] uv2 - \ru Начальное приближение и результат на поверхности surf2.
                       \en The initial approximation and the result on surface surf2. \~
  \return \ru Возвращает код результата итерационного поиска точки пересечения.
          \en Returns the result code of the intersection point iterative search. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (MbeNewtonResult) IntersectionPoint( const MbSurface & surf0, bool ext0,
                                               const MbSurface & surf1, bool ext1,
                                               const MbSurface & surf2, bool ext2,
                                               MbCartPoint & uv0,
                                               MbCartPoint & uv1,
                                               MbCartPoint & uv2 );


//------------------------------------------------------------------------------
/** \brief \ru Найти все точки пересечения поверхности и кривой.
           \en Calculate all the points of intersection of a surface and a curve. \~
  \details \ru Найти все точки пересечения поверхности и кривой. \n
           \en Calculate all the points of intersection of a surface and a curve. \n \~
  \param[in] surf - \ru Поверхность.
                    \en A surface. \~
  \param[in] surfExt - \ru Искать на продолжении поверхности.
                       \en Use the surface extension. \~
  \param[in] curv - \ru Кривая.
                    \en The curve. \~
  \param[in] curveExt - \ru Искать на продолжении кривой.
                        \en Use the curve extension. \~
  \param[out] uv - \ru Параметры точек пересечения на поверхности.
                   \en Parameters of the intersection points on the surface. \~
  \param[out] tt - \ru Параметры точек пересечения на кривой.
                   \en Parameters of the intersection points on the curve. \~
  \param[in]  touchInclude - \ru Считать касания пересечениями.
                             \en Consider tangencies as intersections. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (void) IntersectionPoints( const MbSurface &           surf, 
                                           bool                  surfExt, 
                                     const MbCurve3D &           curv, 
                                           bool                  curveExt,
                                           SArray<MbCartPoint> & uv, 
                                           SArray<double> &      tt,
                                           bool                  touchInclude = false );


//------------------------------------------------------------------------------
/** \brief \ru Определить параметры ближайших точек прямых.
           \en Determine the parameters of the nearest points of lines. \~
  \details \ru Определить параметры ближайших точек прямых, заданных точкой и вектором направления.
           \en Determine the parameters of the nearest points of lines which are defined by the given point and direction vector. \~
  \param[in] origin1, direction1 - \ru Точка и направление первой прямой.
                                   \en A point and direction of the first line. \~
  \param[in] origin2, direction2 - \ru Точка и направление второй прямой.
                                   \en A point and direction of the second line. \~
  \param[out] t1 - \ru Параметр на первой прямой.
                   \en Parameter on the first line. \~
  \param[out] t2 - \ru Параметр на второй прямой.
                   \en Parameter on the second line. \~
  \return \ru Возвращает true, если есть прямые не параллельны. \n
          \en Returns true, if lines are not parallel. \n \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) LineLineNearestParams( const MbCartPoint3D & origin1, 
                                        const MbVector3D &    direction1,
                                        const MbCartPoint3D & origin2, 
                                        const MbVector3D &    direction2,
                                              double &        t1, 
                                              double &        t2 );


//------------------------------------------------------------------------------
/** \brief \ru Определение расстояния между ближайшими точками p1 и p2 прямых line1 и line2
           \en Determination of the distance between the nearest points p1 and p2 of lines line1 and line2 \~
  \details \ru Определение расстояния между ближайшими точками p1 и p2 прямых line1 и line2
           \en Determination of the distance between the nearest points p1 and p2 of lines line1 and line2 \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (double) LineLineNearestPoints( const MbLine3D &      line1, 
                                          const MbLine3D &      line2,
                                                MbCartPoint3D & p1, 
                                                MbCartPoint3D & p2 );


//------------------------------------------------------------------------------
/** \brief \ru Определить параметры ближайших точек прямых.
           \en Determine the parameters of the nearest points of lines. \~
  \details \ru Определить параметры ближайших точек прямых, заданных точкой и вектором направления.
           \en Determine the parameters of the nearest points of lines which are defined by the given point and direction vector. \~
  \param[in] origin1, direction1 - \ru Точка и направление первой прямой.
                                   \en A point and direction of the first line. \~
  \param[in] origin2, direction2 - \ru Точка и направление второй прямой.
                                   \en A point and direction of the second line. \~
  \param[out] t1 - \ru Параметр на первой прямой.
                   \en Parameter on the first line. \~
  \param[out] t2 - \ru Параметр на второй прямой.
                   \en Parameter on the second line. \~
  \return \ru Возвращает true, если есть прямые не параллельны. \n
          \en Returns true, if lines are not parallel. \n \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) LineLineNearestParams( const MbCartPoint & origin1, const MbVector & direction1,
                                        const MbCartPoint & origin2, const MbVector & direction2,
                                        double & t1, double & t2 );


//------------------------------------------------------------------------------
/** \brief \ru Найти точку пересечения двух прямых.
           \en Calculate the point of two lines intersection. \~
  \details \ru Найти точку пересечения двух точно пересекающихся прямых без проверки параллельности. \n
           \en Calculate the intersection point of two exactly intersecting lines without check. \n \~
  \param[in] line1 - \ru Первая прямая.
                     \en The first line. \~
  \param[in] line2 - \ru Вторая прямая.
                     \en The second line. \~
  \param[out] result - \ru Точка пересечения.
                       \en The intersection point. \~
  \ingroup Point_Modeling
*/
// ---
inline
void FastLineLine( const MbLine &      line1, 
                   const MbLine &      line2, 
                         MbCartPoint & result )
{
  const MbDirection & dir1 = line1.GetDirection();
  const MbDirection & dir2 = line2.GetDirection();
  const MbCartPoint & pnt1 = line1.GetOrigin();
  const MbCartPoint & pnt2 = line2.GetOrigin();

  double t = ( dir1.ax * (pnt2.y - pnt1.y) - dir1.ay * (pnt2.x - pnt1.x )) /
             ( dir1.ay * dir2.ax - dir1.ax * dir2.ay );

  result.x = pnt2.x + dir2.ax * t;
  result.y = pnt2.y + dir2.ay * t;
}


//------------------------------------------------------------------------------
/** \brief \ru Найти точку пересечения двух прямых.
           \en Calculate the point of two lines intersection. \~
  \details \ru Найти точку пересечения двух прямых. \n
    Прямые могут быть параллельны или совпадать. \n
           \en Calculate the point of two lines intersection. \n
    The curves can be parallel or coincident. \n \~
  \param[in] line1 - \ru Первая прямая.
                     \en The first line. \~
  \param[in] line2 - \ru Вторая прямая.
                     \en The second line. \~
  \param[out] result - \ru Точка пересечения.
                       \en The intersection point. \~
  \return \ru Возвращает результат пересечения: \n
    1 - Прямые пересекаются. \n
    0 - Прямые параллельны или совпадают.
          \en Returns the result of intersection: \n
    1 - The lines intersect at a point. \n
    0 - The lines are parallel or coincident. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (int) LineLine( const MbLine &      line1, 
                          const MbLine &      line2, 
                                MbCartPoint & result );


//------------------------------------------------------------------------------
/** \brief \ru Найти точку пересечения двух прямых.
           \en Calculate the point of two lines intersection. \~
  \details \ru Найти точку пересечения двух прямых. \n
    Прямые могут быть параллельны или совпадать. \n
           \en Calculate the point of two lines intersection. \n
    The curves can be parallel or coincident. \n \~
  \param[in] line1 - \ru Первая прямая.
                     \en The first line. \~
  \param[in] line2 - \ru Вторая прямая.
                     \en The second line. \~
  \param[out] result - \ru Точка пересечения.
                       \en The intersection point. \~
  \return \ru Возвращает результат пересечения: \n
    1 : прямые пересекаются; \n
    0 : прямые параллельны; \n
    1 : прямые совпадают - касательная точка пересечения.
          \en Returns the result of intersection: \n    1 : the curves intersect at a point; \n    0 : the curves are parallel; \n    1 : the curves are coincident - the tangent intersection point. \~\ingroup Point_Modeling
*/
// ---
MATH_FUNC (int) LineLine( const MbLine &       line1, 
                          const MbLine &       line2, 
                                MbCrossPoint & result );

//------------------------------------------------------------------------------
/** \brief \ru Найти точку пересечения прямой и отрезка.
           \en Calculate the intersection point of a line and a line segment. \~
  \details \ru Найти точку пересечения прямой и отрезка. \n
    Отрезок может быть параллелен прямой или лежать на ней. \n
           \en Calculate the intersection point of a line and a line segment. \n
    The line segment can be parallel to the curve or lie on it. \n \~
  \param[in] line - \ru Прямая.
                    \en The line. \~
  \param[in] lseg - \ru Отрезок.
                    \en The segment. \~
  \param[out] result - \ru Точка пересечения.
                       \en The intersection point. \~
  \return \ru Возвращает результат пересечения: \n
    1 : прямая и отрезок пересекаются; \n
    0 : прямая и отрезок параллельны; \n
    1 : отрезок лежит на прямой - касательная точка пересечения.
          \en Returns the result of intersection: \n    1 : the line and the line segment intersect at a point; \n    0 : the line and a line segment are parallel; \n    1 : the segment lies on the curve - a tangent intersection point. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (int) LineLineSeg( const MbLine &        line, 
                             const MbLineSegment & lseg, 
                                   MbCrossPoint &  result );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки пересечения прямой и окружности.
           \en Calculate intersection points of a line and a circle. \~
  \details \ru Найти параметры точек пересечения прямой и окружности. \n
           \en Calculate the parameters of intersection points of a line and a circle.  \n \~
  \param[in] line - \ru Прямая.
                    \en The line. \~
  \param[in] centre - \ru Центр окружности.
                      \en The circle center. \~
  \param[in] radius - \ru Радиус окружности.
                      \en The circle radius. \~
  \param[out] result - \ru Точки пересечения (указатель на массив из двух(!) элементов).
                       \en The intersection points (a pointer to the array of two (!) elements). \~
  \return \ru Возвращает количество найденных пересечений.
          \en Returns the number of intersections. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (int) LineCircle( const MbLine &       line, 
                            const MbCartPoint &  centre, 
                                  double         radius, 
                                  MbCrossPoint * result );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки пересечения двух кривых.
           \en Calculate intersection points of two curves. \~
  \details \ru Найти параметры точек пересечения двух произвольных кривых. \n
           Точка касания определяется по коллинеарности касательных векторов к кривым в точке пересечения. \n
           Если точка пересечения совпадает с точкой стыка составной кривой, то касательность будет определяться по \n
           коллинеарности касательных векторов для каждого сегмента составной кривой. \n
           Общий метод вызывается, если нет частной функции пересечения. \n
           \en Calculate the parameters of intersection points of two arbitrary curves. \n
           The touching point is determined by the collinearity of the tangent vectors to the curves at the point of intersection. \n
           If the intersection point coincides with the junction point of the compound curve, then the tangency will be determined \n
           by the collinearity of the tangent vectors for each segment of the compound curve. \n
           The general method is used if there is no special function for intersection. \n \~
  \param[in] pCurve1 - \ru Первая кривая.
                       \en The first curve. \~
  \param[in] pCurve2 - \ru Вторая кривая.
                       \en The second curve. \~
  \param[out] result - \ru Множество точек пересечения.
                       \en The array of intersection points. \~
  \param[in] touchInclude - \ru Считать касания пересечениями.
                            \en Consider tangencies as intersections. \~
  \param[in] epsilon - \ru Точность совпадения точек пересечения кривых.
                       \en The accuracy of coincidence points of intersection. \~
  \param[in] allowInaccuracy - \ru Разрешить понижать входную точность.
                               \en Allow lowering input accuracy. \~
  \return \ru Количество найденных пересечений.
          \en The number of intersections. \~
  \warning \ru Применяется для двумерных построений, аналог CurveCurveIntersection.
           \en Used for two-dimensional constructions, the analogue of CurveCurveIntersection. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (ptrdiff_t) IntersectTwoCurves( const MbCurve &              pCurve1, 
                                          const MbCurve &              pCurve2, 
                                                SArray<MbCrossPoint> & result, 
                                                bool                   touchInclude, // = true,
                                                double                 epsilon = Math::LengthEps*c3d::METRIC_DELTA,
                                                bool                   allowInaccuracy = true );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки пересечения двух кривых.
           \en Calculate intersection points of two curves. \~
  \details \ru Найти параметры точек пересечения двух произвольных кривых. \n
           Общий метод вызывается, если нет частной функции пересечения. \n
           \en Calculate the parameters of intersection points of two arbitrary curves. \n
           The general method is used if there is no special function for intersection. \n \~
  \param[in] curve1 - \ru Первая кривая.
                      \en The first curve. \~
  \param[in] curve2 - \ru Вторая кривая.
                      \en The second curve. \~
  \param[out] result1 - \ru Параметры пересечений первой кривой.
                        \en The parameters of intersections for the first curve. \~
  \param[out] result2 - \ru Параметры пересечений второй кривой.
                        \en The parameters of intersections for the second curve. \~
  \param[in] xEpsilon - \ru Точность по x.
                        \en Tolerance in x direction. \~
  \param[in] yEpsilon - \ru Точность по y.
                        \en Tolerance in y direction. \~
  \param[in] touchInclude - \ru Считать касания пересечениями.
                            \en Consider tangencies as intersections. \~
  \param[in] allowInaccuracy - \ru Разрешить нахождение решения с меньшей точностью при невозможности удовлетворить указанной.
                               \en Allow to find a solution with less precision when we can't get a solution with given precision. \~
  \return \ru Количество найденных пересечений.
          \en The number of intersections. \~
  \warning \ru Применяется для трехмерных построений, аналог IntersectTwoCurves.
           \en Used for three-dimensional constructions, the analogue of IntersectTwoCurves. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (ptrdiff_t) CurveCurveIntersection( const MbCurve &        curve1, 
                                              const MbCurve &        curve2,
                                                    SArray<double> & result1, 
                                                    SArray<double> & result2,
                                                    double           xEpsilon, 
                                                    double           yEpsilon, 
                                                    bool             touchInclude, 
                                                    bool             allowInaccuracy = true );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки самопересечения кривой.
           \en Calculate the points of curve self-intersection. \~
  \details \ru Найти параметры точек самопересечения кривой с заданной точностью. \n
           \en Calculate the self-intersection points parameters with the given tolerance. \n \~
  \param[in] curve - \ru Кривая.
                     \en The curve. \~
  \param[in] xEpsilon - \ru Точность по x.
                        \en Tolerance in x direction. \~
  \param[in] yEpsilon - \ru Точность по y.
                        \en Tolerance in y direction. \~
  \param[out] result1 - \ru Множество параметров самопересечения.
                        \en The self-intersection parameters array. \~
  \param[out] result2 - \ru Множество параметров самопересечения.
                        \en The self-intersection parameters array. \~
  \param[in] version - \ru Версия операции.
                       \en The version of the operation. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (void) CurveSelfIntersect( const MbCurve &        curve, 
                                           double           xEpsilon, 
                                           double           yEpsilon,
                                           SArray<double> & result1, 
                                           SArray<double> & result2, 
                                           VERSION          version = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Удалить точки касания.
           \en Remove touch points. \~
  \details \ru Удалить все точки касания кривых вне зависимости от положения параметра на кривой 
              (внутри области определения или на границах кривой).
           \en Remove all curves touch points regardless the position on the curve 
              (in the domain or on the borders). \~
  \param[in]      curve1 - \ru Первая кривая.
                           \en The first curve. \~
  \param[in]      curve2 - \ru Вторая кривая.
                           \en The second curve. \~
  \param[in, out] result - \ru Множество точек пересечения.
                           \en The array of intersection points. \~
  \param[in]      eps -    \ru Погрешность для функции проверки параллельности касательных RoundColinear.
                           \en Accuracy for the function RoundColinear of testing the parallelism of tangents. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (void) RemoveAllTouchParams( const MbCurve &              curve1, 
                                       const MbCurve &              curve2,
                                             SArray<MbCrossPoint> & result,
                                             double                 eps = PARAM_NEAR );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки пересечения двух кривых.
           \en Calculate intersection points of two curves. \~
  \details \ru Найти параметры точек пересечения двух произвольных кривых. \n
           \en Calculate the parameters of intersection points of two arbitrary curves. \n \~
  \param[in] curve1 - \ru Первая кривая.
                      \en The first curve. \~
  \param[in] curve2 - \ru Вторая кривая.
                      \en The second curve. \~
  \param[out] result1 - \ru Параметры точек пересечения для первой кривой.
                        \en The intersection points parameters for the first curve. \~
  \param[out] result2 - \ru Параметры точек пересечения для второй кривой.
                        \en The intersection points parameters for the second curve. \~
  \param[in] mEps - \ru Возможная максимальная погрешность найденных пересечений.
                    \en The intersection tolerance. \~
  \return \ru Количество найденных пересечений.
          \en The number of intersections. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (ptrdiff_t) CurveCurveIntersection( const MbCurve3D &      curve1, 
                                              const MbCurve3D &      curve2,
                                                    SArray<double> & result1, 
                                                    SArray<double> & result2,
                                                    double           mEps, // = Math::metricRegion,
                                                    VERSION          version = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Проверить кривую на самопересечение.
           \en Determine if the curve has self-intersections. \~
  \details \ru Проверить заданную кривую на самопересечение. \n
           \en Determine if the given curve has self-intersections. \n \~ 
  \param[in] curve - \ru Кривая.
                     \en The curve. \~
  \param[in] mEps - \ru Возможная максимальная погрешность найденных самопересечений.
                    \en The tolerance of self-intersections. \~
  \return \ru Возвращает true, если кривая самопересекается.
          \en Returns true if the curve has self-intersections. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (bool) IsSelfIntersect( const MbCurve3D & curve, double mEps = Math::metricRegion );


//------------------------------------------------------------------------------
/** \brief \ru Убрать касательные точки пересечения.
           \en Remove the tangent intersection points. \~
  \details \ru Убрать параметры касательных точек пересечения внутри областей определения кривых. \n
           \en Remove the tangent intersection points parameters inside the domains of curves. \n \~
  \param[in] curve1 - \ru Первая кривая.
                      \en The first curve. \~
  \param[in] curve2 - \ru Вторая кривая.
                      \en The second curve. \~
  \param[out] result1 - \ru Параметры точек пересечения для первой кривой.
                        \en The intersection points parameters for the first curve. \~
  \param[out] result2 - \ru Параметры точек пересечения для второй кривой.
                        \en The intersection points parameters for the second curve. \~
  \param[in] mEps - \ru Возможная максимальная погрешность найденных пересечений.
                    \en The intersection tolerance. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (void) FilterTouchParams( const MbCurve3D &      curve1, 
                                    const MbCurve3D &      curve2,
                                          SArray<double> & result1, 
                                          SArray<double> & result2,
                                          double           mEps = Math::metricRegion );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки скрещения двух кривых.
           \en Calculate the points of two curves crossing. \~
  \details \ru Найти параметры точек скрещения двух кривых. \n
           \en Calculate parameters of the points of two curves crossing. \n \~
  \param[in] curve1 - \ru Первая кривая.
                      \en The first curve. \~
  \param[in] curve2 - \ru Вторая кривая.
                      \en The second curve. \~
  \param[out] result1 - \ru Параметры точек скрещения для первой кривой.
                        \en Parameters of the points of crossing for the first curve. \~
  \param[out] result2 - \ru Параметры точек скрещения для второй кривой.
                        \en Parameters of the points of crossing for the second curve. \~
  \return \ru Количество найденных скрещений.
          \en The points of crossing number. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (ptrdiff_t) CurveCurveCrossing( const MbCurve3D &      curve1, 
                                          const MbCurve3D &      curve2,
                                                SArray<double> & result1, 
                                                SArray<double> & result2,
                                                double           epsilon = Math::metricRegion );


//------------------------------------------------------------------------------
/** \brief \ru Найти проекцию точки на поверхность относительно внешнего контура поверхности.
           \en Find the projection of a point on a surface relative to the outer contour of the surface. \~
  \details \ru Найти проекцию пространственной точки на поверхность в виде двумерной точки на поверхности
           относительно внешнего контура поверхности. \n
           \en Calculate the projection of a space point on a surface as a two-dimensional point on the surface.
           relative to the outer contour of the surface. \n \~
  \param[in] surface - \ru Поверхность.
                       \en A surface. \~
  \param[in] pnt - \ru Пространственная точка.
                   \en A space point. \~
  \param[in] byOuterRectOnly - \ru Классифицировать проекцию только относительно внешнего габаритного прямоугольника.
                               \en Whether to classify the projection relative to the outer bounding box only. \~
  \param[out] result - \ru Двумерная параметрическая точка на поверхности.
                       \en A two-dimensional parametric point on the surface. \~
  \return \ru Возвращает true, если найдена нормальная проекция точки на поверхность.
          \en Returns true if a normal projection of the point on the surface has been calculated. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (bool) PointProjectionRelativeOuterLoop( const MbSurface &     surface, 
                                                   const MbCartPoint3D & pnt,
                                                         bool            byOuterRectOnly,
                                                         MbCartPoint &   result );


//------------------------------------------------------------------------------
/** \brief \ru Является ли проекция точки точно неоднозначной.
           \en Determine whether the point projection is multiple-valued. \~
  \details \ru Является ли проекция точки неоднозначной при проецировании
           в области определения поверхности. \n
           \en Determine whether the point projection is multiple-valued while projecting
           inside the surface domain. \n \~
  \param[in] surface - \ru Поверхность.
                       \en A surface. \~
  \param[in] result - \ru Пространственная точка.
                      \en A space point. \~
  \return \ru Возвращает true, если проекция точки является неоднозначной.
          \en Returns true if the point projection is multiple-valued \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (bool) IsMultipleProjection( const MbSurface &     surface, 
                                       const MbCartPoint3D & result );


//------------------------------------------------------------------------------
/** \brief \ru Найти точки касания двух поверхностей.
           \en Calculate the touch points of two surfaces. \~
  \details \ru Найти параметры точек касания двух поверхностей. \n
           \en Calculate the parameters of touch points of two surfaces. \n \~
  \param[in] surf1 - \ru Первая поверхность.
                     \en A first surface. \~
  \param[in] ext1 - \ru Искать на продолжении первой поверхности.
                    \en Use the first surface extension. \~
  \param[in] surf2 - \ru Вторая поверхность.
                     \en A second surface. \~
  \param[in] ext2 - \ru Искать на продолжении второй поверхности.
                    \en Use the second surface extension. \~
  \param[in] uv1arr - \ru Параметры точек касания первой поверхности.
                      \en Parameters of touch points of first surface. \~
  \param[in] uv2arr - \ru Параметры точек касания второй поверхности.
                      \en Parameters of touch points of second surface. \~
  \return \ru Возвращает true, если найдены точки касания.
          \en Returns true if the touch points has be calculate. \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC ( bool) TouchIntersectionPoints( const MbSurface &                surf1, 
                                                 bool                       ext1,
                                           const MbSurface &                surf2, 
                                                 bool                       ext2,
                                                 std::vector<MbCartPoint> & uv1arr,
                                                 std::vector<MbCartPoint> & uv2arr );


//------------------------------------------------------------------------------
/** \brief \ru Определить положение точек относительно оболочки.
           \en Determine the position of points relative to the shell. \~
  \details \ru Определить положение точек относительно оболочки. \n
           \en Determine the position of points relative to the shell. \n \~
  \param[in] points - \ru Точки.
                      \en Points. \~
  \param[in] shell - \ru Оболочка.
                     \en Shell. \~
  \param[out] pLocs - \ru Положение точек относительно оболочки.
                      \en The position of points relative to the shell. \~
  \return \ru Возвращает false, если хотя бы одно положение точки не определено, иначе true.
          \en Returns false if at least one point position is not defined, otherwise true. \~
  \ingroup Point_Modeling
*/
// ---
MATH_FUNC (bool) PointsRelativeShell( const std::vector<MbCartPoint3D> &   points,
                                      const MbFaceShell &                  shell,
                                            std::vector<MbeItemLocation> & pLocs );


#endif // __ACTION_POINT_H

