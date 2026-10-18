////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Функции создания кривых для внешнего использования.
         \en Functions to create curves for external use. \~

*/
////////////////////////////////////////////////////////////////////////////////


#ifndef __ALG_POLYLINE_H
#define __ALG_POLYLINE_H


#include <templ_s_array.h>
#include <mb_cart_point.h>
#include <mb_cart_point3d.h>
#include <mb_operation_result.h>
#include <mb_enum.h>
#include <mb_rect1d.h>
#include <plane_item.h>


class    MbVector;
class    MbCurve;
class    MbCartPoint;
class    MbPlacement3D;
class    MbBezier;
class    MbNurbs;
class    MbCurve3D;
class    MbMatrix3D;
class    MbNurbs3D;
class    MbCurve3D;
class    MbContour3D;
class    MbCubicSpline3D;


//-------------------------------------------------------------------------------
/** \brief \ru Параметры точки для создания полилинии.
           \en Point parameters for creation of a polyline. \~
  \details \ru Часть точек может быть удалена при построении, поэтому вводится старый индекс, 
    который заполняется и используется в модели. Параметрами точки являются координаты точки и 
    радиус скругления в этой точке. При создании заполняются поля m_lineSeg и m_arcSeg.
    m_lineSeg - это прямолинейный сегмент из этой точки в следующую. Для последней точки
    и замкнутой ломаной - из последней в первую. m_arcSeg - дуга скругления в данной точке.
    Если какой-то сегмент был полностью удален или не создан, то его указатель должен быть nullptr.
    Объектами m_lineSeg и m_arcSeg не владеет, поэтому и не удаляет их. Объекты из полилинии.
           \en Some points may be deleted while the construction, therefore the old index is entered, 
    it is filled and used in a model. Parameters of a point are its coordinates and 
    fillet radius in this point. In a time of creation the fields 'm_lineSeg' and 'm_lineSeg' are being filled.
    'm_lineSeg' is the straight-line segment from this point to the next point. For the last point
    and a closed polyline - from the last point to the first point. 'm_arcSeg'is the arc of a fillet in the given point.
    If a segment has been fully deleted or it was not created then the pointer should be nullptr.
    Object 'm_lineSeg' and 'm_arcSeg' are not owned, therefore they are not deleted. Objects from a polyline. \~
  \ingroup Data_Structures
*/
// ---
struct MATH_CLASS Polyline3DPoint {
public:
  size_t              m_oldIndex; ///< \ru Исходный индекс в модели. \en The initial index of a model. 
  MbCartPoint3D       m_point;    ///< \ru Координаты вершины ломаной. \en The coordinates of a polyline vertex. 
  double              m_radius;   ///< \ru Радиус скругления в вершине. \en The fillet radius in a vertex. 
  const MbCurve3D *   m_lineSeg;  ///< \ru Прямолинейный сегмент из этой вершины в следующую. \en The straight-line segment from this vertex to the next. 
  const MbCurve3D *   m_arcSeg;   ///< \ru Дуга скругления в этой вершине (если m_radius > 0). \en the arc of a fillet in this vertex (if 'm_radius' > 0) 

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  Polyline3DPoint()
    : m_oldIndex( SYS_MAX_T )
    , m_point   ()
    , m_radius  ( 0.0 ) 
    , m_lineSeg ( nullptr )
    , m_arcSeg  ( nullptr )
  {}
  /// \ru Конструктор копирования. \en Copy constructor. 
  Polyline3DPoint( const Polyline3DPoint & other )
    : m_oldIndex( other.m_oldIndex )
    , m_point   ( other.m_point    ) 
    , m_radius  ( other.m_radius   )
    , m_lineSeg ( other.m_lineSeg  )
    , m_arcSeg  ( other.m_arcSeg   )
  {}
  /// \ru Деструктор. \en Destructor. 
  ~Polyline3DPoint()
  {}
public:
  /// \ru Оператор присваивания. \en Assignment operator. 
  void operator = ( const Polyline3DPoint & other ) {
    m_oldIndex = other.m_oldIndex; 
    m_point    = other.m_point; 
    m_radius   = other.m_radius; 
    m_lineSeg  = other.m_lineSeg; 
    m_arcSeg   = other.m_arcSeg;
  }
};


//------------------------------------------------------------------------------
/** \brief \ru Построить контур из отрезков по заданным точкам.
           \en Construct a contour from segments by given points. \~
  \details \ru Вершины сочленения скругляются. Каждой вершине соответствует радиус скругления. \n
    Если две вершины совпадают, то одна из них и соответствующий ей радиус удаляются.
           \en Vertices of joint are rounded. Some fillet radius corresponds to every vertex. \n
    If two vertices are coincident then one of them and the corresponding radius are deleted. \~
  \param[out] contour - \ru Контур.
                        \en The countour. \~
  \param[in] closed - \ru Флаг замкнутости контура.
                      \en Whether the contour is closed. \~
  \param[in] initList - \ru Множество точек полилинии.
                        \en The array of points of a polyline. \~
  \param[out] errorIndexes - \ru Множество индексов сегментов, сочленение которых со следующим прошло с ошибками.
                             \en The array of segments indices, each of which has been jointed with the next one with errors. \~
  \param[in] lengthEpsilon - \ru Погрешность построения элементов полилинии.
                             \en The tolerance of polyline elements construction. \~
  \return \ru true, если сегментов больше нуля.
          \en true if the number of segments is greater than zero. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (bool) InitContour3D( MbContour3D & contour, bool closed,
                                SArray<Polyline3DPoint> & initList, 
                                SArray<ptrdiff_t> & errorIndexes,
                                double lengthEpsilon = Math::lengthEpsilon ); 


//------------------------------------------------------------------------------
/** \brief \ru Вычислить погрешность точки на кривой.
           \en Calculate the tolerance of a point on a curve. \~
  \details \ru Погрешностью считается ограничивающая сфера точки.
           \en The tolerance is a sphere bounding a point. \~
  \param[in] crv - \ru Исходная кривая.
                   \en The initial curve. \~
  \param[in] t - \ru Координата точки на кривой.
                 \en A coordinate of a point on a curve. \~
  \param[out] pnt - \ru Трехмерная координата точки на кривой.
                    \en A three-dimensional coordinate of a point on a curve. \~
  \param[out] eps - \ru Погрешность точки.
                    \en The tolerance of a point. \~
  \param[in] version - \ru Версия.
                       \en Version. \~
  \ingroup Algorithms_3D
*/
//---
MATH_FUNC (void) GetEpsilonBound( const MbCurve3D & crv, double t, 
                                  MbCartPoint3D & pnt, double & eps, 
                                  VERSION version /*= Math::DefaultMathVersion()*/ ); // \ru KVA K13+ 6.5.2011 Версия нужна обязательно \en KVA K13+ 6.5.2011 A version is absolutely necessary. 


//------------------------------------------------------------------------------
/** \brief \ru Проверить инцидентность двух вершин.
           \en Check the coincidence of two vertices. \~
  \details \ru Кривые рассматриваются как ребра).
           \en Curves are considered as edges. \~
  \param[in] crv1 - \ru Кривая 1.
                    \en The curve 1. \~
  \param[in] t1 - \ru Если t1 == 1, рассматривается конец кривой, иначе начало.
                  \en If 't1' equals 1 then the end of a curve is considered, the start of a curve is considered otherwise. \~
  \param[in] crv2 - \ru Кривая 2.
                    \en The curve 2. \~
  \param[in] t2 - \ru Если t2 == 1, рассматривается конец кривой, иначе начало.
                  \en If 't2' equals 1 then the end of a curve is considered, the start of a curve is considered otherwise. \~
  \param[in] version - \ru Версия.
                       \en Version. \~
  \return \ru true, если вершины инцидентны.
          \en true if vertices are coincident. \~
  \ingroup Algorithms_3D
*/
//---
MATH_FUNC (bool) IsIncidence( const MbCurve3D & crv1, int t1, 
                              const MbCurve3D & crv2, int t2, 
                                    VERSION version /*= Math::DefaultMathVersion()*/ ); // \ru KVA K13+ 6.5.2011 Версия нужна обязательно \en KVA K13+ 6.5.2011 A version is absolutely necessary. 


//------------------------------------------------------------------------------
/** \brief \ru Дать ближайший к лучу параметр кривой.
           \en Get the curve parameter which is nearest to the ray. \~
  \details \ru Луч проходит через точку point в направлении вектора direct
           \en The ray is passed through the point 'point' in the direction of the vector 'direct' \~
  \param[in] curve - \ru Исходная кривая.
                     \en The initial curve. \~
  \param[in] point - \ru Точка луча.
                     \en The point of a ray. \~
  \param[in] direct - \ru Вектор направления луча.
                      \en The vector of ray direction. \~
  \return \ru Ближайший к лучу параметр кривой.
          \en The nearest curve parameter to the ray. \~
  \ingroup Algorithms_3D
*/
//---
MATH_FUNC (double) GetNearCurveParam( const MbCurve3D & curve, 
                                      const MbCartPoint3D & point, const MbVector3D & direct );


//------------------------------------------------------------------------------
/** \brief \ru Дать ближайший к лучу параметр кривой.
           \en Get the curve parameter which is nearest to the ray. \~
  \details \ru Луч проходит через точку point в направлении вектора direct. \n
    setOnSide == true принуждает установить параметр кривой к ближайшему концу и 
    вычислить флаг isBegin, определяющий близость к началу (true) или концу (false) кривой.
           \en The ray is passed through the point 'point' in the direction of the vector 'direct'. \n
    if 'setOnSide' equals true then the parameter of curve should be set for the nearest end and 
    the flag 'isBegin' determines the proximity to the curve start (true) or to the curve end (false). \~
  \param[in] curve - \ru Исходная кривая.
                     \en The initial curve. \~
  \param[in] point - \ru Точка луча.
                     \en The point of a ray. \~
  \param[in] direct - \ru Вектор направления луча.
                      \en The vector of a ray direction. \~
  \param[in] setOnSide - \ru Надо ли приравнять параметр к ближайшему концу кривой.
                         \en Whether the parameter should be equated to the nearest end of a curve. \~
  \param[out] isBegin - \ru если true, то параметр находится ближе к началу кривой. \n
    Если false, то параметр находится ближе к концу кривой.
                        \en if true than the parameter is located closer to the start of a curve. \n
    if false than the parameter is located closer to the end of a curve. \~
  \return \ru Ближайший к лучу параметр кривой.
          \en The nearest curve parameter to the ray. \~
  \ingroup Algorithms_3D
*/
//---
MATH_FUNC (double) GetNearCurveParam( const MbCurve3D & curve, 
                                      const MbCartPoint3D & point, const MbVector3D & direct,
                                      bool setOnSide, bool & isBegin );


//------------------------------------------------------------------------------
/** \brief \ru Создать гладкую кривую из кривой Безье.
           \en Create a smooth curve from a Bezier curve. \~
  \details \ru По исходной кривой Безье создается NURBS 4-го порядка. После NURBS разбивается
  в трижды кратных внутренних узлах, если они существуют. \n 
  Если bline принимает значение true, то проверяется вырожденность в линию. Если рассматриваемый 
  сегмент или кривая целиком - линия, то выполняется преобразование в линию.
           \en A NURBS of the fourth degree is created by an initial Bezier curve . Thereafter the NURBS is splitted
  in internal knots of triple multiplicity if they exist \n 
  If 'bline' is true then the degeneration into a line is checked. If the considered 
  segment or the entire curve is a line then it is trandformed into a line. \~
  \param[in] bez - \ru Кривая Безье.
                   \en Bezier curve \~
  \param[out] arCurve - \ru Множество созданных кривых.
                        \en The array of created curves. \~
  \param[in] bline - \ru Флаг проверки вырожденности в линию.
                     \en The flag for the check of degeneration into a line. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) CreateSmoothFromBezier( const MbBezier & bez, RPArray<MbCurve> & arCurve,
                                         bool bline );


//------------------------------------------------------------------------------
/** \brief \ru Создать кривую заданного типа базе NURBS-кривой.
           \en Create a curve of a given type as NURBS-curve. \~
  \details \ru Работает для двух типов: pt_LineSegment и pt_Arc. Если не удалось 
    аппроксимировать с заданной точностью функция вернет nullptr.
           \en It works for the two types: 'pt_LineSegment' and 'pt_Arc'. If approximation with the given tolerance has failed 
    then the function returns nullptr. \~
  \param[in] nurbs - \ru Исходная NURBS-кривая.
                     \en The initial NURBS-curve. \~
  \param[in] type - \ru Тип кривой, которую требуется создать.
                    \en The type of a curve which is required to create. \~
  \param[in] eps - \ru Точность аппроксимации.
                   \en The tolerance of approximation. \~
  \return \ru Указатель на кривую, если она была создана.
          \en The pointer to the curve if it has been created. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (MbCurve *) ConvertNurbsToCurveOfType( const MbNurbs & nurbs, MbePlaneType type, double eps );


//------------------------------------------------------------------------------
/** \brief \ru Построить плоскую проекцию кривой.
           \en Construct a planar projection of a curve. \~
  \details \ru Построить двумерную кривую - проекцию кривой на плоскость XY локальной системы координат, заданной матрицей преобразования. 
    Двумерные кубические сплайны Эрмита (MbHermit) и кубические сплайны (MbCubicSpline) заменяются на NURBS (MbNurbs).
           \en Construct a two-dimensional curve - projection of a curve to the plane XY of a coordinate system which is set by the matrix of transformation. 
    Two-dimensional cubic splines of Hermite ('MbHermit') and cubic splines ('MbCubicSpline') are replaced by NURBS ('MbNurbs'). \~ 
  \param[in] curve3D - \ru Исходная кривая.
                       \en The initial curve. \~
  \param[in] into - \ru Матрица преобразования из глобальной системы координат в видовую плоскость.
                    \en The transformation matrix from the global coordinate system into a plane of view. \~
  \param[in] pRgn - \ru Параметрическая область кривой для создания проекции.
                    \en The parametric region of a curve for the creation of a projection. \~
  \param[in] version - \ru Версия построения.
                       \en The version of construction. \~
  \return \ru Указатель на полученную кривую.
          \en The pointer to the obtained curve. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (MbCurve *) GetFlatCurve( const MbCurve3D & curve3D, const MbMatrix3D & into, 
                                    MbRect1D * pRgn = nullptr, VERSION version = Math::DefaultMathVersion() );


//------------------------------------------------------------------------------
/** \brief \ru Получить плоскую проекцию кривой.
           \en Get a planar projection of a curve. \~
  \details \ru Кривая проецируется на заданную плоскость.
           \en A curve is projected onto a given plane. \~
  \param[in] curve3D - \ru Исходная кривая.
                       \en The initial curve. \~
  \param[in] place - \ru Плоскость, на которую требуется спроецировать кривую.
                     \en The plane the curve should be projected on. \~
  \return \ru Указатель на полученную проекционную кривую.
          \en The pointer to the obtained projection curve. \~

  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (MbCurve *) GetFlatProjection( const MbCurve3D & curve3D, 
                                         const MbPlacement3D & place, 
                                         VERSION version = Math::DefaultMathVersion() ); 


//------------------------------------------------------------------------------
/** \brief \ru Положение кривой относительно точек оси.
           \en The location of a curve relative to axis points. \~
  \details \ru Для определения направления оси вращения.
           \en For the definition of the rotation axis direction. \~
  \param[in] curve - \ru Исходная кривая.
                     \en The initial curve. \~
  \param[in] p1 - \ru Первая точка оси.
                  \en The first point of an axis. \~
  \param[in] p2 - \ru Вторая точка оси.
                  \en The second point of an axis. \~
  \return \ru 0 в случае сбоя при работе программы, \n
    иначе возвращается векторное произведение нормализованного вектора оси (p1, p2) и 
    вектора (p1, w), где w - координаты центра тяжести кривой.
          \en 0 in a case of failure, \n
    otherwise the vector product of the normalized axis vector ('p1', 'p2') and 
    the vector ('p1', 'w') is returned. ('w' is the coordinates of the curve's center of gravity). \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (double) CurveRelative( const MbCurve & curve, const MbCartPoint & p1, const MbCartPoint & p2 );


//------------------------------------------------------------------------------
/** \brief \ru Знак площади тени кривой на отрезок.
           \en A sign of area of a curve's shadow on a segment. \~
  \details \ru Требуется для определения направления контура заметания. 
    Если кривая не замкнута, то она замыкается через ось.
           \en This is required for the definition of the sweep contour direction. 
    If a curve is not closed then it becomes closed through an axis. \~
  \param[in] curve - \ru Исходная кривая.
                     \en The initial curve. \~
  \param[in] p1 - \ru Первая точка отрезка.
                  \en The first point of a segment. \~
  \param[in] p2 - \ru Вторая точка отрезка.
                  \en The second point of a segment. \~
  \param[in] sag - \ru Угол отклонения. Используется для расчета шага по кривой.
                   \en The deviation angle. Used for calculation of the step by a curve. \~
  \return \ru Площадь тени со знаком.
          \en The area of the shadow with a sign. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (double) ContourRelative( const MbCurve & curve, const MbCartPoint & p1, const MbCartPoint & p2, double sag );


#endif // __ALG_POLYLINE_H

