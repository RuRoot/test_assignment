////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Отрисовка объектов.
         \en Objects drawing. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ALG_DRAW_H
#define __ALG_DRAW_H

#include <templ_s_array.h>
#include <templ_sptr.h>
#include <math_define.h>
#include <mesh.h>
#include <cur_contour.h>


#define TRGB_BLACK          0,   0,   0   ///< \ru Черный цвет. \en Black color. \~ \ingroup Drawing
#define TRGB_BLUE           0,   0, 192   ///< \ru Синий цвет. \en Blue color. \~ \ingroup Drawing
#define TRGB_GREEN          0, 128,   0   ///< \ru Зеленый цвет. \en Green color. \~ \ingroup Drawing
#define TRGB_CYAN           0, 128, 128   ///< \ru Голубой цвет. \en Cyan color. \~ \ingroup Drawing
#define TRGB_RED          192,   0,   0   ///< \ru Красный цвет. \en Red color. \~ \ingroup Drawing
#define TRGB_MAGENTA       96,   0, 192   ///< \ru Пурпурный цвет. \en Magenta color. \~ \ingroup Drawing
#define TRGB_BROWN        192, 128,   0   ///< \ru Коричневый цвет. \en Brown color. \~ \ingroup Drawing
#define TRGB_LIGHTGRAY    192, 192, 192   ///< \ru Светло-серый цвет. \en Light gray color \~ \ingroup Drawing
#define TRGB_DARKGRAY     128, 128, 128   ///< \ru Темно-серый цвет. \en Dark gray color \~ \ingroup Drawing
#define TRGB_LIGHTBLUE      0,   0, 255   ///< \ru Ярко-синий цвет. \en Light blue color. \~ \ingroup Drawing
#define TRGB_LIGHTGREEN     0, 255,   0   ///< \ru Ярко-зеленый цвет. \en Light green color. \~ \ingroup Drawing
#define TRGB_LIGHTCYAN      0,  96, 255   ///< \ru Светло-голубой цвет. \en Light cyan color \~ \ingroup Drawing
#define TRGB_LIGHTRED     255,   0,   0   ///< \ru Ярко-красный цвет. \en Light red color. \~ \ingroup Drawing
#define TRGB_LIGHTMAGENTA  96,   0, 255   ///< \ru Светло-пурпурный цвет. \en Light magenta color \~ \ingroup Drawing
#define TRGB_ORANGE       255, 128,   0   ///< \ru Оранжевый цвет. \en Orange color. \~ \ingroup Drawing
#define TRGB_YELLOW       255, 255,   0   ///< \ru Желтый цвет. \en Yellow color. \~ \ingroup Drawing
#define TRGB_WHITE        255, 255, 255   ///< \ru Белый цвет. \en White color. \~ \ingroup Drawing

#define TRGB_PURPLE       255,   0, 255   ///< \ru Светло-фиолетовый цвет. \en Purple color \~ \ingroup Drawing
#define TRGB_AZURE          0, 125, 255   ///< \ru Лазурный цвет. \en Azure color. \~ \ingroup Drawing
#define TRGB_TANGERINE    255, 136,   0   ///< \ru Мандариновый цвет. \en Tangerine color. \~ \ingroup Drawing
#define TRGB_CERISE       255,   0, 125   ///< \ru Светло-вишневый цвет. \en Cerise color. \~ \ingroup Drawing
#define TRGB_OLIVE        128, 128,   0   ///< \ru Оливковый цвет. \en Olive color. \~ \ingroup Drawing
#define TRGB_SPRINGGREEN  0,   255, 127   ///< \ru Весенне-зеленый цвет. \en Spring green color. \~ \ingroup Drawing
#define TRGB_AVIATION     125, 225, 255   ///< \ru Авиационный цвет. \en Aviation color. \~ \ingroup Drawing
#define TRGB_GOLD         205, 255,  25   ///< \ru Золотой цвет. \en Gold color. \~ \ingroup Drawing

#define TRGB_DELETE       TRGB_GREEN      // \ru цвет удаляемых объектов \en A color of deleted objects 
#define TRGB_NEW          TRGB_RED        // \ru цвет вновь создаваемых объектов \en A color of new objects 
#define TRGB_CLEAR        TRGB_LIGHTGRAY  // \ru Закрасить \en Clear 
#define TRGB_SHOW         TRGB_BLACK      // \ru Показать \en Show 


class MbSpaceItem;
class MbPlaneItem;
class MbTopItem;
class MbVertex;
class MbEdge;
class MbFace;
class MbCartPoint;
class MbPlaneItem;
class MbCartPoint;
class MbParamCurvePatch;
class MbMatrix3D;
class MbCartPoint3D;
class MbFloatPoint3D;
class MbMesh;
class MbCube;
class MbCurve3D;
class MbPolygon3D;
class MbVector3D;
class MbPlacement3D;
class MbVector;
class MbCartPoint;
class MbCurve;
class MbSurface;
class MbSurfaceCurve;
class MbContourOnSurface;
class MbSurfaceIntersectionCurve;
class MbCurveBoundedSurface;
class MbGrid;
class MbPlanarGrid;
class MbCollection;


//------------------------------------------------------------------------------
/** \brief \ru Интерфейс отладочной отрисовки приложения.
           \en Interface of debug drawing of application. \~
  \details \ru Интерфейс отладочной отрисовки для CallBack связи с вызывающим приложением.
           \en Interface of debug drawing for connection with the calling application using callbacks. \~
  \ingroup Drawing
*/
// ---
class IfDrawGI {
public:
           IfDrawGI() {}
  virtual ~IfDrawGI() {}

public:
  /// \ru Отрисовать объект. \en Draw an any object. 
  virtual void DrawItem( const MbRefItem * ri, int R, int G, int B, const MbMatrix3D & from = MbMatrix3D::identity, int width = 1 ) = 0;
  /// \ru Отрисовать трехмерный геометрический объект. \en Draw a three-dimensional geometric object. 
  virtual void DrawItem( const MbSpaceItem * gi, int R, int G, int B, int width = 1 ) = 0;
  // Отрисовать трехмерный геометрический объект с размещением по матрице.
  virtual void DrawItem( const MbSpaceItem * gi, int R, int G, int B, const MbMatrix3D & from, int width = 1 ) = 0;
  /// \ru Отрисовать полигональный геометрический объект. \en Draw a polygonal geometric object. 
  virtual void DrawMesh( const MbMesh * ms, int R, int G, int B, int width = 1 ) = 0;
  /// \ru Отрисовать полигон. \en Draw a polygon. 
  virtual void DrawPolygon( const MbPolygon3D * polygon, int R, int G, int B, int width = 1 ) = 0;
  /// \ru Отрисовать трехмерную точку. \en Draw a three-dimensional point. 
  virtual void DrawItem( const MbCartPoint3D & gi, int R, int G, int B, int width = 2 ) = 0;
  /// \ru Отрисовать трехмерную точку. \en Draw a three-dimensional point. 
  virtual void DrawPoint( const MbCartPoint3D * gi, int R, int G, int B, int width = 2 ) = 0;
  /// \ru Отрисовать трехмерный отрезок. \en Draw a three-dimensional segment. 
  virtual void DrawLine( const MbCartPoint3D & q1, const MbCartPoint3D & q2, int R, int G, int B, int width = 1 ) = 0;
  /// \ru Отрисовать полилинию. \en Draw a polyline. 
  virtual void DrawPolyline( SArray<MbCartPoint3D> & points, int R, int G, int B, int width = 1 ) = 0;
  /// \ru Отрисовать трехмерный отрезок. \en Draw a three-dimensional segment. 
  virtual void DrawLine( const MbCartPoint3D & p, const MbVector3D & v, int R, int G, int B, int width = 1 ) = 0;
  /// \ru Отрисовать трехмерный отрезок. \en Draw a three-dimensional segment. 
  virtual void DrawLine( const MbFloatPoint3D & q1, const MbFloatPoint3D & q2, int R, int G, int B, int width = 1 ) = 0;
  /// \ru Отрисовать участок трехмерной кривой. \en Draw a piece of a three-dimensional curve. 
  virtual void DrawCurve( const MbCurve3D & curve, double t1, double t2, int R, int G, int B, int width = 1 ) = 0; 
  /// \ru Отрисовать часть двумерной кривой на плоскости. \en Draw a piece of a two-dimensional curve on a plane. 
  virtual void DrawCurve( const MbCurve & curve, const MbPlacement3D & place, double t1, double t2, int R, int G, int B, int width = 1 ) = 0; 
  /// \ru Отрисовать двумерный геометрический объект. \en Draw a two-dimensional geometric object. 
  virtual void DrawItem( const MbPlaneItem * gi, int R, int G, int B, const MbMatrix3D & from, int width = 1 ) = 0;
  /// \ru Отрисовать двумерный геометрический объект. \en Draw a two-dimensional geometric object. 
  virtual void DrawItem( const MbPlaneItem * gi, int R, int G, int B, int width = 1 ) = 0;
  /// \ru Отрисовать двумерную точку. \en Draw a two-dimensional point. 
  virtual void DrawPoint( const MbCartPoint * gi, int R, int G, int B, const MbMatrix3D & from, int width = 1 ) = 0;
  /// \ru Отрисовать двумерную точку. \en Draw a two-dimensional point. 
  virtual void DrawPoint( const MbCartPoint * gi, int R, int G, int B, int width = 1 ) = 0;
  /// \ru Отрисовать двумерный отрезок. \en Draw a two-dimensional segment. 
  virtual void DrawLine( const MbCartPoint & q1, const MbCartPoint & q2, int R, int G, int B, const MbMatrix3D & from, int width = 1 ) = 0; 
  /// \ru Отрисовать двумерный отрезок. \en Draw a two-dimensional segment. 
  virtual void DrawLine( const MbCartPoint & q1, const MbCartPoint & q2, int R, int G, int B, int width = 1 ) = 0;
  /// \ru Отрисовать топологический объект. \en Draw a topological object. 
  virtual void DrawItem( const MbTopItem * ti, int R, int G, int B, const MbMatrix3D & from = MbMatrix3D::identity, int width = 1 ) = 0;
  /// \ru Отрисовать ребро. \en Draw an edge. 
  virtual void DrawEdge( const MbEdge * edge, int r, int g, int b, bool drawVerts, int width = 1 ) = 0;
  /// \ru Отрисовать патч двумерной кривой. \en Draw a patch of a two-dimensional curve. 
  virtual void PutPatch( const MbCartPoint & pnt, const MbVector & dir, double a, double b, int R, int G, int B, const MbMatrix3D & mapInto ) = 0; 
  /// \ru Отрисовать габаритный куб. \en Draw a bounding box. 
  virtual void PutCube( const MbCube & gab, int width = 1, bool bDrawRed = true ) = 0;
  /// \ru Отрисовать систему координат. \en Draw a coordinate system. 
  virtual void DrawPlacement3D( const MbPlacement3D & place, double lenAxes, int width = 1 ) = 0;
  /// \ru Очистить текущее окно. \en Clear the active window. 
  virtual void DrawClearMap() = 0;
  /// \ru Отрисовать двумерную кривую на поверхности. \en Draw a two-dimensional curve on a surface. 
  virtual void DrawItem( const MbCurve * curve, const MbSurface * surface, int R, int G, int B, int width = 1 ) = 0;
  /// \ru Отрисовать карту кривой на поверхности. \en Draw a map of a curve on a surface. 
  virtual void DrawCurveMap( const MbCurve * curve, const MbSurface * surface, int R, int G, int B ) = 0;
  // \ru Отрисовать карту кривой на поверхности в масштабе, определяемом так, чтобы активная область окна покрывалась заданным прямоугольником rect.
  // Флаг isotropic отвечает за отрисовку объектов равных масштабах по осям. Если isotropic==false, окружность может отобразиться как эллипс, квадрат как прямоугольник и т.п.
  // \en Draw a map of a curve on a surface with a specified scale determined as a way of covering an active window by the given rectangle rect.
  // The flag isotropic is responsible for drawing in equal scales along the axes. If isotropic==false then a circle can be shown as an ellipsis, a square as an rectangle etc.
  virtual void DrawCurveMap( const MbCurve * curve, const MbSurface * surface, const MbRect & rect, int R, int G, int B, const int boundaryWidth, const int curveWidth, const bool isotropic, const bool showSurfaceBoundaries ) = 0;
  /// \ru Отрисовать кривую на поверхности. \en Draw a curve on a surface. 
  virtual void DrawSurfaceCurveMap( const MbSurfaceCurve * scurve, int R, int G, int B ) = 0;
  /// \ru Отрисовать точку на поверхности. \en Draw a point on a surface. 
  virtual void DrawPointMap( const MbCartPoint * pnt, const MbSurface * surface, int R, int G, int B ) = 0;
  /// \ru Отрисовать кривую пересечения на параметрической плоскости. \en Draw an intersection curve on a parametric plane. 
  virtual void DrawSurfaceIntersectionMap( const MbSurfaceIntersectionCurve * gi, int R1, int G1, int B1, int R2, int G2, int B2 ) = 0;
  /// \ru Отрисовать контур на поверхности в параметрической плоскости. \en Draw a contour on a surface in a parametric plane. 
  virtual void DrawContourOnSurfaceMap( const MbContourOnSurface * gi, int R, int G, int B ) = 0;
  /// \ru Отрисовать ограничивающие кривые усеченной поверхности в ее параметрической плоскости. \en Draw bounding curves of a trimmed surface in its parametric plane. 
  virtual void DrawCurveBoundedSurfaceMap( const MbCurveBoundedSurface * bnds, int R, int G, int B ) = 0;
  /// \ru Отрисовать параметрическую плоскость поверхности. \en Draw a parametric plane of a surface. 
  virtual void DrawSurfaceMap( const MbSurface * surface, int R, int G, int B ) = 0;
  /// \ru Отрисовать ограничивающие кривые грани в ее параметрической плоскости. \en Draw bounding curves of a face in its parametric plane. 
  virtual void DrawFaceMap( const MbFace *, int R, int G, int B ) = 0;
  /// \ru Отрисовать параметрическую точку поверхности. \en Draw a parametric point of a surface. 
  virtual void DrawPoint( const MbSurface & surface, const MbCartPoint & uv, int R, int G, int B, int width = 2 ) = 0;
  /// \ru Отрисовать массив 3d-точек по массиву 2d-точек и поверхности. \en Draw an array of 3D-points by an array of 2D-points and a surface. 
  virtual void DrawPoints( const MbSurface & surface, const SArray<MbCartPoint> & uvArr, int R, int G, int B, int width = 2 ) = 0;
  /// \ru Отрисовать массив 3d-точек по массиву параметров и кривой. \en Draw an array of 3D-points by an array of parameters and a curve. 
  virtual void DrawPoints( const MbCurve3D & curve, const SArray<double> & tArr, int R, int G, int B, int width = 2 ) = 0;
  /// \ru Отрисовать массив 3d-точек. \en Draw an array of 3D-points. 
  virtual void DrawPoints( const SArray<MbCartPoint3D> & pnts, int R, int G, int B, int width = 2 ) = 0;
  /// \ru Отрисовать триангуляцию. \en Draw a triangulation. 
  virtual void PutGrid( const MbGrid & grid, int R, int G, int B, int width = 1 ) = 0;
  /// \ru Отрисовать треугольник. \en Draw a triangle. 
  virtual void PutTriangle( const MbGrid & grid, ptrdiff_t index, int R, int G, int B ) = 0;
  /// \ru Отрисовать треугольник. \en Draw a triangle. 
  virtual void PutTriangle( const MbSurface & surface,
                            const MbGrid & grid, ptrdiff_t index, int R, int G, int B ) = 0;
  /// \ru Отрисовать треугольник. \en Draw a triangle. 
  virtual void PutTriangle( const MbCollection & coll, ptrdiff_t index, int R, int G, int B ) = 0;
  /// \ru Отрисовать треугольник. \en Draw a triangle. 
  virtual void PutTriangle( const MbSurface & surface, 
                            const MbCartPoint & uv0, const MbCartPoint & uv1, const MbCartPoint & uv2,
                            int R, int G, int B ) = 0;
  /// \ru Отрисовать четырёхугольник. \en Draw a quadrangle. 
  virtual void PutQuadrangle( const MbGrid & grid, ptrdiff_t index, int R, int G, int B ) = 0;
  /// \ru Отрисовать четырёхугольник. \en Draw a quadrangle. 
  virtual void PutQuadrangle( const MbSurface & surface,
                              const MbGrid & grid, ptrdiff_t index, int R, int G, int B ) = 0;
  /// \ru Отрисовать двумерную триангуляцию. \en Draw a two-dimensional triangulation. 
  virtual void PutPlanarGrid( MbPlanarGrid & grid, const MbPlacement3D & place, int R, int G, int B, int width = 1 ) = 0;
  /// \ru Отрисовать триангуляционную сетку на поверхности. \en Draw a triangular mesh on a surface. 
  virtual void DrawGridMap( const MbGrid & grid, const MbSurface & surface, int R, int G, int B ) = 0; 
  /// \ru Стереть модель. \en Delete model.
  virtual void EraseModel() = 0;
  /// \ru Перерисовать модель. \en Redraw the model.
  virtual void RedrawModel() = 0;

  // Получить плейсмент активного окна (например для прорисовки плоской кривой).
  virtual void GetWindowPlace( MbPlacement3D & place ) = 0;

OBVIOUS_PRIVATE_COPY( IfDrawGI )
};


#if defined(_DRAWGI)

//------------------------------------------------------------------------------
/** \brief \ru Функции отладочной отрисовки объектов приложения.
           \en Functions of debug drawing of application objects. \~
  \details \ru Функции отладочной отрисовки геометрических объектов заданным цветом в текущем окне
    используют интерфейс IfDrawGI, полученный через функцию SetDrawGI.
           \en Functions of debug drawing of geometric objects by a given color in an active window
    use the interface 'IfDrawGI' obtained by the function 'SetDrawGI'. \~
  \ingroup Drawing
*/
// ---
class MATH_CLASS DrawGI {
public:

////////////////////// \ru Отрисовка трехмерных объектов. //////////////////////////
////////////////////// \en Drawing of three-dimensional objects. ////////////////////////// 

  /** \brief \ru Отрисовать трехмерный геометрический объект.
             \en Draw a three-dimensional geometric object. \~
    \details \ru Функция геометрического объекта заданным цветом в текущем окне.
             \en The function of geometric object drawing by a given color in an active window. \~
    \param[in] gi - \ru Пространственный объект.
                    \en The spatial object. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawItem( const MbSpaceItem * gi, int R, int G, int B, int width = 1 );

  /** \brief \ru Отрисовать полигональный геометрический объект.
             \en Draw a polygonal geometric object. \~
    \details \ru Функция отрисовки полигонального геометрического объекта заданным цветом в текущем окне.
             \en The function of the polygonal geometric object drawing by a given color in an active window. \~
    \param[in] ms - \ru Фасетный объект.
                    \en The mesh. \~ 
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawMesh( const MbMesh * ms, int R, int G, int B, int width = 1 );

  /** \brief \ru Отрисовать полигон.
             \en Draw a polygon. \~
    \details \ru Функция отрисовки полигона заданным цветом в текущем окне.
             \en The function of polygon drawing by a given color in an active window. \~
    \param[in] polygon - \ru Полигон.
                         \en The polygon. \~ 
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawPolygon( const MbPolygon3D * polygon, int R, int G, int B, int width = 1 );

  /** \brief \ru Отрисовать трехмерную точку.
             \en Draw a three-dimensional point. \~
    \details \ru Функция отрисовки точки заданным цветом в текущем окне.
             \en The function of point drawing by a given color in an active window. \~
    \param[in] gi - \ru Исходная точка.
                    \en The initial point. \~ 
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawItem( const MbCartPoint3D & gi, int R, int G, int B, int width = 2 );
  static void DrawPoint( const MbCartPoint3D * gi, int R, int G, int B, int width = 2 );

  /** \brief \ru Отрисовать трехмерный отрезок.
             \en Draw a three-dimensional segment. \~
    \details \ru Функция отрисовки отрезка заданным цветом в текущем окне. Отрезок задается 
      крайними точками.
             \en The function of segment drawing by a given color in an active window. A segment is specified 
      by the end points. \~
    \param[in] q1, q2 - \ru Исходные точки.
                        \en Initial points. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawLine( const MbCartPoint3D & q1, const MbCartPoint3D & q2, int R, int G, int B, int width = 1 );


  /** \brief \ru Отрисовать полилинию.
             \en Draw a polyline. \~
    \details \ru Функция отрисовки полилинии заданным цветом в текущем окне. Полилиния задается 
      массивом трехмерных точек.
             \en The function of polyline drawing by a given color in an active window. Polyline is specified 
      by an array of three-dimensional points. \~
    \param[in] points - \ru Исходный массив точек.
                        \en The initial array of points. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawPolyline( SArray<MbCartPoint3D> & points, int R, int G, int B, int width = 1 );

  /** \brief \ru Отрисовать трехмерный отрезок.
             \en Draw a three-dimensional segment. \~
    \details \ru Функция отрисовки отрезка заданным цветом в текущем окне. Отрезок задается 
      точкой и вектором.
             \en The function of segment drawing by a given color in an active window. A segment is specified 
      by point and vector. \~
    \param[in] p - \ru Исходная точка.
                   \en The initial point. \~
    \param[in] v - \ru Исходный вектор.
                   \en The initial vector. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawLine( const MbCartPoint3D & p, const MbVector3D & v, int R, int G, int B, int width = 1 );

  /** \brief \ru Отрисовать трехмерный отрезок.
             \en Draw a three-dimensional segment. \~
    \details \ru Функция отрисовки отрезка заданным цветом в текущем окне. Отрезок задается 
      крайними точками.
             \en The function of segment drawing by a given color in an active window. A segment is specified 
      by the end points. \~
    \param[in] q1, q2 - \ru Исходные точки.
                        \en Initial points. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \ingroup Drawing
  */
  static void DrawLine( const MbFloatPoint3D & q1, const MbFloatPoint3D & q2, int R, int G, int B, int width = 1 );

  /** \brief \ru Отрисовать участок трехмерной кривой.
             \en Draw a piece of a three-dimensional curve. \~
    \details \ru Функция отрисовки участка кривой заданным цветом в текущем окне. Участок
      кривой задается параметрами начала и конца участка.
             \en The function of curve piece drawing by a given color in an active window. A piece
      of a curve is specified by start and end parameters of a piece. \~
    \param[in] curve - \ru Исходная кривая.
                       \en An initial curve. \~
    \param[in] t1, t2 - \ru Начало и конец участка кривой, который требуется отрисовать.
                        \en The start parameter and the end parameter of a curve piece which is required to draw. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawCurve( const MbCurve3D & curve, double t1, double t2, int R, int G, int B, int width = 1 ); 

  /** \brief \ru Отрисовать часть двумерной кривой на плоскости.
             \en Draw a piece of a two-dimensional curve on a plane. \~
    \details \ru Функция отрисовки участка кривой заданным цветом в текущем окне. Участок
      кривой задается параметрами начала и конца участка.
             \en The function of curve piece drawing by a given color in an active window. A piece
      of a curve is specified by start and end parameters of a piece. \~
    \param[in] curve - \ru Исходная кривая.
                       \en An initial curve. \~
    \param[in] place - \ru Исходный плейсмент. Задает плоскость для отрисовки.
                       \en The initial placement. It specifies the plane for drawing. \~
    \param[in] t1, t2 - \ru Начало и конец участка кривой, который требуется отрисовать.
                        \en The start parameter and the end parameter of a curve piece which is required to draw. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawCurve( const MbCurve & curve, const MbPlacement3D & place, double t1, double t2, int R, int G, int B, int width = 1 ); 

////////////////////// \ru Отрисовка двумерных объектов. /////////////////////////// \en Drawing of two-dimensional objects. /////////////////////////// 

  /** \brief \ru Отрисовать двумерный геометрический объект.
             \en Draw a two-dimensional geometric object. \~
    \details \ru Функция геометрического объекта заданным цветом в текущем окне.
             \en The function of geometric object drawing by a given color in an active window. \~
    \param[in] gi - \ru Исходный объект.
                    \en The initial object. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] from - \ru Матрица перехода из плейсмента объекта в глобальную систему координат.
                      \en The matrix of transformation from the object placement to the global coordinate system. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawItem( const MbPlaneItem * gi, int R, int G, int B, const MbMatrix3D & from, int width = 1 );

  /** \brief \ru Отрисовать двумерный геометрический объект.
             \en Draw a two-dimensional geometric object. \~
    \details \ru Функция геометрического объекта заданным цветом в текущем окне.
             \en The function of geometric object drawing by a given color in an active window. \~
    \param[in] gi - \ru Исходный объект.
                    \en The initial object. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawItem( const MbPlaneItem * gi, int R, int G, int B, int width = 1 );

  /** \brief \ru Отрисовать двумерную точку.
             \en Draw a two-dimensional point. \~
    \details \ru Функция отрисовки точки заданным цветом в текущем окне.
             \en The function of point drawing by a given color in an active window. \~
    \param[in] gi - \ru Исходная точка.
                    \en The initial point. \~ 
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] from - \ru Матрица перехода из плейсмента объекта в глобальную систему координат.
                      \en The matrix of transformation from the object placement to the global coordinate system. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawPoint( const MbCartPoint * gi, int R, int G, int B, const MbMatrix3D & from, int width = 1 );

  /** \brief \ru Отрисовать двумерную точку.
             \en Draw a two-dimensional point. \~
    \details \ru Функция отрисовки точки заданным цветом в текущем окне.
             \en The function of point drawing by a given color in an active window. \~
    \param[in] gi - \ru Исходная точка.
                    \en The initial point. \~ 
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawPoint( const MbCartPoint * gi, int R, int G, int B, int width = 1 );

  /** \brief \ru Отрисовать двумерный отрезок.
             \en Draw a two-dimensional segment. \~
    \details \ru Функция отрисовки отрезка заданным цветом в текущем окне. Отрезок задается 
      крайними точками.
             \en The function of segment drawing by a given color in an active window. A segment is specified 
      by the end points. \~
    \param[in] q1, q2 - \ru Исходные точки.
                        \en Initial points. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] from - \ru Матрица перехода из плейсмента объекта в глобальную систему координат.
                      \en The matrix of transformation from the object placement to the global coordinate system. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawLine( const MbCartPoint & q1, const MbCartPoint & q2, int R, int G, int B, const MbMatrix3D & from, int width = 1 ); 

  /** \brief \ru Отрисовать трехмерный отрезок.
             \en Draw a three-dimensional segment. \~
    \details \ru Функция отрисовки отрезка заданным цветом в текущем окне. Отрезок задается 
      крайними точками.
             \en The function of segment drawing by a given color in an active window. A segment is specified 
      by the end points. \~
    \param[in] q1, q2 - \ru Исходные точки.
                        \en Initial points. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawLine( const MbCartPoint & q1, const MbCartPoint & q2, int R, int G, int B, int width = 1 );

////////////////////////// \ru Отрисовка топологических объектов. ////////////////// \en Drawing of topological objects. ////////////////// 

  /** \brief \ru Отрисовать топологический объект.
             \en Draw a topological object. \~
    \details \ru Топологический объект отрисовывается заданным цветом в текущем окне.
             \en A topological object is drawn by a given color in an active window. \~
    \param[in] ti - \ru Исходный топологический объект.
                    \en The initial topological object. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] from - \ru Матрица перехода из плейсмента объекта в глобальную систему координат.
                      \en The matrix of transformation from the object placement to the global coordinate system. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawItem( const MbTopItem * ti, int R, int G, int B, const MbMatrix3D & from = MbMatrix3D::identity, int width = 1 );

  /** \brief \ru Отрисовать ребро.
             \en Draw an edge. \~
    \details \ru Ребро отрисовывается заданным цветом в текущем окне. При установке соответствующего 
      флага отрисовываются вершины.
             \en An edge is drawn by a given color in an active window. If the corresponding flag is set, 
      the vertices are drawn. \~
    \param[in] edge - \ru Исходное ребро.
                      \en The initial edge. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] drawVerts - \ru Флаг отрисовки вершин ребра.
                           \en Whether edge vertices are drawn. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawEdge( const MbEdge * edge, int R, int G, int B, bool drawVerts, int width = 1 );

////////////////////// \ru Отрисовка вспомогательных объектов. ///////////////////// \en Drawing of assisting items. ///////////////////// 

  /** \brief \ru Отрисовать патч двумерной кривой.
             \en Draw a patch of a two-dimensional curve. \~
    \details \ru Патч отрисовывается заданным цветом в текущем окне.
             \en A patch is drawn by a given color in an active window. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] mapInto - \ru Матрица перехода в локальную систему координат.
                         \en The matrix of transformation to the local coordinate system. \~
    \ingroup Drawing
  */
  static void PutPatch( const MbCartPoint & pnt, const MbVector & dir, double a, double b, int R, int G, int B, const MbMatrix3D & mapInto ); 

  /** \brief \ru Отрисовать габаритный куб в текущем окне.
             \en Draw a bounding box in an active window. \~
    \details \ru Куб отрисовывается либо красным, либо синим цветом в текущем окне.
             \en A bounding box is drawn by the red color or by the blue color in an active window. \~
    \param[in] gab - \ru Исходный габаритный куб.
                     \en The initial bounding box. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \param[in] bDrawRed - \ru Флаг отрисовки красным цветом. Если false, отрисовывается 
      синим цветом.
                          \en Whether the red color is used. If the value is 'false' then to draw by 
      the blue color. \~
    \ingroup Drawing
  */
  static void PutCube( const MbCube & gab, int width = 1, bool bDrawRed = true );

  /** \brief \ru Отрисовать систему координат.
             \en Draw a coordinate system. \~
  \details \ru Система координат отрисовывается с заданными длинами осей.
           \en A coordinate system is drawn with given axes lengths. \~
  \param[in] place - \ru Исходная система координат.
                     \en The initial coordinate system. \~
  \param[in] lenAxes - \ru Длина осей.
                       \en Axes lengths. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
  \ingroup Drawing
  */
  static void DrawPlacement3D( const MbPlacement3D & place, double lenAxes, int width = 1 );

////////// \ru Отрисовка объектов в пространстве параметров поверхности. /////////// \en Drawing of objects in a surface parameter space. /////////// 

  /// \ru Очистить текущее окно. \en Clear the active window. 
  static void DrawClearMap();

  /** \brief \ru Отрисовать двумерную кривую на поверхности.
             \en Draw a two-dimensional curve on a surface. \~
    \details \ru Отрисовка заданной двумерной кривой на заданной поверхности заданным цветом  
      в текущем окне.
             \en Drawing of a given two-dimensional curve on a given surface by a given color  
      in an active window. \~
    \param[in] curve - \ru Исходная двумерная кривая.
                       \en An initial two-dimensional curve. \~
    \param[in] surface - \ru Исходная поверхность.
                         \en The initial surface. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawItem( const MbCurve * curve, const MbSurface * surface, int R, int G, int B, int width = 1 );

  /** \brief \ru Отрисовать карту кривой на поверхности.
             \en Draw a map of a curve on a surface. \~
    \details \ru Отрисовка карты заданной кривой на заданной поверхности заданным цветом 
      в текущем окне.
             \en Drawing of a map of a given curve on a given surface by a given color 
      in an active window. \~
    \param[in] curve - \ru Исходная кривая.
                       \en An initial curve. \~
    \param[in] surface - \ru Исходная поверхность.
                         \en The initial surface. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \ingroup Drawing
  */
  static void DrawCurveMap( const MbCurve * curve, const MbSurface * surface, int R, int G, int B );

  /** \brief \ru Отрисовать кривую на поверхности.
             \en Draw a curve on a surface. \~
    \details \ru Отрисовка кривой на поверхности в параметрической плоскости заданным цветом 
      в текущем окне.
             \en Drawing of a curve on a surface in a parametric plane by a given color 
      in an active window. \~
    \param[in] scurve - \ru Исходная кривая на поверхности.
                        \en The initial curve on a surface. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \ingroup Drawing
  */
  static void DrawSurfaceCurveMap( const MbSurfaceCurve * scurve, int R, int G, int B );

  /** \brief \ru Отрисовать точку на поверхности.
             \en Draw a point on a surface. \~
    \details \ru Отрисовка точки на поверхности заданным цветом в текущем окне.
             \en Drawing of point on a surface by a given color in an active window. \~
    \param[in] pnt - \ru Исходная точка.
                     \en The initial point. \~
    \param[in] surface - \ru Исходная поверхность.
                         \en The initial surface. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \ingroup Drawing
  */
  static void DrawPointMap( const MbCartPoint * pnt, const MbSurface * surface, int R, int G, int B );

  /** \brief \ru Отрисовать кривую пересечения на параметрической плоскости.
             \en Draw an intersection curve on a parametric plane. \~
    \details \ru Отрисовывается кривая 1 цветом 1 на поверхности 1 и кривая 2 цветом 2 на поверхности 2.
             \en The curve 1 is drawn by the color 1 on the surface 1 and the curve 2 is drawn by the color 2 on the surface 2. \~
    \param[in] gi - \ru Кривая пересечения.
                    \en The intersection curve \~
    \param[in] R1, R2 - \ru Красный цвет [0,255].
                        \en Red color [0,255]. \~
    \param[in] G1, G2 - \ru Зеленый цвет [0,255].
                        \en Green color [0,255]. \~
    \param[in] B1, B2 - \ru Синий цвет [0,255].
                        \en Blue color [0,255]. \~
    \ingroup Drawing
  */
  static void DrawSurfaceIntersectionMap( const MbSurfaceIntersectionCurve * gi, int R1, int G1, int B1, int R2, int G2, int B2 );

  /** \brief \ru Отрисовать контур на поверхности в параметрической плоскости.
             \en Draw a contour on a surface in a parametric plane. \~
    \details \ru Отрисовывается заданным цветом в текущем окне.
             \en Drawing by a given color in an active window. \~
    \param[in] gi - \ru Контур на поверхности.
                    \en The contour on a surface. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \ingroup Drawing
  */
  static void DrawContourOnSurfaceMap( const MbContourOnSurface * gi, int R, int G, int B );

  /** \brief \ru Отрисовать ограничивающие кривые усеченной поверхности в ее параметрической плоскости.
             \en Draw bounding curves of a trimmed surface in its parametric plane. \~
    \details \ru Отрисовывается заданным цветом в текущем окне.
             \en Drawing by a given color in an active window. \~
    \param[in] bnds - \ru Ограниченная кривыми поверхность.
                      \en The surface bounded by curves. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \ingroup Drawing
  */
  static void DrawCurveBoundedSurfaceMap( const MbCurveBoundedSurface * bnds, int R, int G, int B );

  /** \brief \ru Отрисовать параметрическую плоскость поверхности.
             \en Draw a parametric plane of a surface. \~
    \details \ru Отрисовывается заданным цветом в текущем окне.
             \en Drawing by a given color in an active window. \~
    \param[in] surface - \ru Исходная поверхность.
                         \en The initial surface. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \ingroup Drawing
  */
  static void DrawSurfaceMap( const MbSurface * surface, int R, int G, int B );

  /** \brief \ru Отрисовать параметрическую точку поверхности.
             \en Draw a parametric point of a surface. \~
    \details \ru Отрисовывается заданным цветом в текущем окне.
             \en Drawing by a given color in an active window. \~
    \param[in] surface - \ru Исходная поверхность.
                         \en The initial surface. \~
    \param[in] uv - \ru Исходная точка.
                    \en The initial point. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawPoint( const MbSurface & surface, const MbCartPoint & uv, int R, int G, int B, int width = 2 );

  /** \brief \ru Отрисовать массив 3d-точек по массиву 2d-точек и поверхности.
             \en Draw an array of 3D-points by an array of 2D-points and a surface. \~
    \details \ru Отрисовывается заданным цветом в текущем окне.
             \en Drawing by a given color in an active window. \~
    \param[in] surface - \ru Исходная поверхность.
                         \en The initial surface. \~
    \param[in] uvArr - \ru Исходный массив двумерных точек.
                       \en The initial array of two-dimensional points. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawPoints( const MbSurface & surface, const SArray<MbCartPoint> & uvArr, int R, int G, int B, int width = 2 );

  /** \brief \ru Отрисовать массив 3d-точек по массиву параметров и кривой.
             \en Draw an array of 3D-points by an array of parameters and a curve. \~
    \details \ru Отрисовывается заданным цветом в текущем окне.
             \en Drawing by a given color in an active window. \~
    \param[in] curve - \ru Исходная кривая.
                       \en The initial curve. \~
    \param[in] tArr - \ru Исходный массив точек на кривой.
                      \en The initial array of points on a curve. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawPoints( const MbCurve3D & curve, const SArray<double> & tArr, int R, int G, int B, int width = 2 );

  /** \brief \ru Отрисовать массив 3d-точек.
             \en Draw an array of 3D-points. \~
    \details \ru Отрисовывается заданным цветом в текущем окне.
             \en Drawing by a given color in an active window. \~
    \param[in] pnts - \ru Исходный массив трехмерных точек.
                      \en The initial array of three-dimensional points. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void DrawPoints( const SArray<MbCartPoint3D> & pnts, int R, int G, int B, int width = 2 );

///////////////////////// \ru Отрисовка триангуляции. ////////////////////////////// \en Drawing of a triangulation. ////////////////////////////// 

  /** \brief \ru Отрисовать триангуляцию.
             \en Draw a triangulation. \~
    \details \ru Отрисовывается заданным цветом в текущем окне.
             \en Drawing by a given color in an active window. \~
    \param[in] grid - \ru Триангуляционная сетка.
                      \en The triangular mesh. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void PutGrid( const MbGrid & grid, int R, int G, int B, int width = 1 );

  /** \brief \ru Отрисовать треугольник.
             \en Draw a triangle. \~
    \details \ru Отрисовывается заданным цветом в текущем окне. Нужный треугольник из триангуляции
      задается индексом.
             \en Drawing by a given color in an active window. A necessary triangle from triangulation
      is specified by an index. \~
    \param[in] grid - \ru Триангуляционная сетка.
                      \en The triangular mesh. \~
    \param[in] index - \ru Индекс треугольника.
                       \en The index of triangle. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \ingroup Drawing
  */
  static void PutTriangle( const MbGrid & grid, ptrdiff_t index, int R, int G, int B );

  /** \brief \ru Отрисовать треугольник.
             \en Draw a triangle. \~
    \details \ru Отрисовывается заданным цветом в текущем окне. Нужный треугольник из триангуляции
      задается индексом.
             \en Drawing by a given color in an active window. A necessary triangle from triangulation
      is specified by an index. \~
    \param[in] surface - \ru Поверхность.
                         \en The surface. \~
    \param[in] grid -    \ru Триангуляционная сетка.
                         \en The triangular mesh. \~
    \param[in] index -   \ru Индекс треугольника.
                         \en The index of triangle. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \ingroup Drawing
  */
  static void PutTriangle( const MbSurface & surface,
                           const MbGrid & grid, ptrdiff_t index, int R, int G, int B );

  /** \brief \ru Отрисовать треугольник.
             \en Draw a triangle. \~
    \details \ru Отрисовывается заданным цветом в текущем окне. Нужный треугольник из триангуляции
      задается индексом.
             \en Drawing by a given color in an active window. A necessary triangle from triangulation
      is specified by an index. \~
    \param[in] grid - \ru Триангуляционная сетка.
                      \en The triangular mesh. \~
    \param[in] index - \ru Индекс треугольника.
                       \en The index of triangle. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \ingroup Drawing
  */
  static void PutTriangle( const MbCollection & coll, ptrdiff_t index, int R, int G, int B );

  /** \brief \ru Отрисовать треугольник.
             \en Draw a triangle. \~
    \details \ru Отрисовывается заданным цветом в текущем окне.
             \en Drawing by a given color in an active window. \~
    \param[in] surface - \ru Поверхность.
                         \en The surface. \~
    \param[in] uv0, uv1, uv2 - \ru Точки в параметрах поверхности.
                               \en Points in surface parameters. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \ingroup Drawing
  */
  static void PutTriangle( const MbSurface & surface, 
                           const MbCartPoint & uv0, const MbCartPoint & uv1, const MbCartPoint & uv2,
                           int R, int G, int B );

  /** \brief \ru Отрисовать четырёхугольник.
             \en Draw a quadrangle. \~
    \details \ru Отрисовывается заданным цветом в текущем окне. Нужный четырёхугольник из триангуляции
      задается индексом.
             \en Drawing by a given color in an active window. A necessary quadrangle from triangulation
      is specified by an index. \~
    \param[in] grid - \ru Триангуляционная сетка.
                      \en The triangular mesh. \~
    \param[in] index - \ru Индекс четырёхугольник.
                       \en The index of quadrangle. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \ingroup Drawing
  */
  static void PutQuadrangle( const MbGrid & grid, ptrdiff_t index, int R, int G, int B );

  /** \brief \ru Отрисовать четырёхугольник.
             \en Draw a quadrangle. \~
    \details \ru Отрисовывается заданным цветом в текущем окне. Нужный четырёхугольник из триангуляции
      задается индексом.
             \en Drawing by a given color in an active window. A necessary quadrangle from triangulation
      is specified by an index. \~
    \param[in] surface - \ru Поверхность.
                         \en The surface. \~
    \param[in] grid - \ru Триангуляционная сетка.
                      \en The triangular mesh. \~
    \param[in] index - \ru Индекс четырёхугольник.
                       \en The index of quadrangle. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \ingroup Drawing
  */
  static void PutQuadrangle( const MbSurface & surface,
                             const MbGrid & grid, ptrdiff_t index, int R, int G, int B );

  /** \brief \ru Отрисовать двумерную триангуляцию.
             \en Draw a two-dimensional triangulation. \~
    \details \ru Отрисовывается заданным цветом в текущем окне.
             \en Drawing by a given color in an active window. \~
    \param[in] grid - \ru Триангуляционная сетка.
                      \en The triangular mesh. \~
    \param[in] place - \ru Исходная плоскость.
                       \en The initial plane. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \param[in] width - \ru Ширина линий рисования.
                       \en The width of the draw lines. \~
    \ingroup Drawing
  */
  static void PutPlanarGrid( MbPlanarGrid & grid, const MbPlacement3D & place, int R, int G, int B, int width = 1 );

  /** \brief \ru Отрисовать триангуляционную сетку на поверхности.
             \en Draw a triangular mesh on a surface. \~
    \details \ru Отрисовывается заданным цветом в текущем окне.
             \en Drawing by a given color in an active window. \~
    \param[in] grid - \ru Триангуляционная сетка.
                      \en The triangular mesh. \~
    \param[in] surface - \ru Исходная поверхность.
                         \en The initial surface. \~
    \param[in] R - \ru Красный цвет [0,255].
                   \en Red color [0,255]. \~
    \param[in] G - \ru Зеленый цвет [0,255].
                   \en Green color [0,255]. \~
    \param[in] B - \ru Синий цвет [0,255].
                   \en Blue color [0,255]. \~
    \ingroup Drawing
  */
  static void DrawGridMap( const MbGrid & grid, const MbSurface & surface, int R, int G, int B ); 

  /** \brief Стереть модель.
    \ingroup Drawing
  */
  static void EraseModel();

  /** \brief Перерисовать модель.
    \ingroup Drawing
  */
  static void RedrawModel();

  /** \brief Получить плейсмент активного окна (например для прорисовки плоской кривой).
    \ingroup Drawing
  */
  static void GetWindowPlace( MbPlacement3D & place );
}; // class DrawGI


//------------------------------------------------------------------------------
/** \brief \ru Отрисовать объект.
           \en Draw an object. \~
  \details \ru Отрисовать объект заданным цветом в текущем окне.
           \en Drawing by a given color in an active window. \~
  \ingroup Drawing
*/
// ---
template <class Type>
void DrawItem( SPtr<Type> & item, int R, int G, int B ) {
  DrawGI::DrawItem( item.get(), R, G, B );
}


//------------------------------------------------------------------------------
/** \brief \ru Отрисовать множество объектов.
           \en Draw a set of objects. \~
  \details \ru Отрисовать множество объектов заданным цветом в текущем окне.
           \en Draw a set of objects by a given color in an active window. \~
  \ingroup Drawing
*/
// ---
template <class PtrArray>
void DrawItems( const PtrArray & items, int R, int G, int B )
{
  for ( size_t k = 0, cnt = items.size(); k < cnt; ++k )
    DrawGI::DrawItem( items[k], R, G, B );
}


//------------------------------------------------------------------------------
/** \brief \ru Отрисовать вершину с прилегающими ребрами.
           \en Draw a vertex with adjacent edges. \~
  \details \ru Отрисовать вершину с прилегающими ребрами заданным цветом в текущем окне.
           \en Draw a vertex with adjacent edges by a given color in an active window. \~
  \ingroup Drawing
*/
// ---
template <class Vertex, class Edges>
void DrawVertexEdges( const Vertex * vertex, int vR, int vG, int vB, 
                      const Edges & edges, int eR, int eG, int eB )
{
  DrawGI::DrawItem( vertex, vR, vG, vB );
  MbMesh edgeMesh;
  MbStepData stepData( ist_SpaceStep, Math::visualSag );
  MbFormNote note(true, false);
  for ( size_t k = 0, cnt = edges.size(); k < cnt; ++k ) {
    if ( edges[k] != nullptr ) {
      edges[k]->GetCurve().CalculateMesh( stepData, note, edgeMesh );
      DrawGI::DrawMesh( &edgeMesh, TRGB_WHITE );
      DrawGI::DrawMesh( &edgeMesh, eR, eG, eB );
      edgeMesh.Flush();
    }
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Функция имплементации интерфейса IfDrawGI.
           \en The function of 'IfDrawGI' interface implementarion. \~
  \details \ru Функция установки указателя на имплементацию интерфейса IfDrawGI.
           \en The function of setting a pointer to the 'IfDrawGI' interface implementarion. \~
  \ingroup Drawing
*/
// ---
MATH_FUNC (void) SetDrawGI( const IfDrawGI * iDrawGIImpl );


//------------------------------------------------------------------------------
// отрисовка контуров прямоугольной области
//---
//------------------------------------------------------------------------------
/** \brief \ru Функция отрисовки контуров прямоугольной области.
           \en Rectangular area contour drawing function. \~
  \details \ru Отрисовка прямоугольной области на поверхности по четырем точкам.
           \en Drawing a rectangular area on the surface by four points. \~
  \ingroup Drawing
*/
// ---
template <class Rect>
static void DrawRectangle( const Rect & rect, const MbSurface & surf, int r, int g, int b )
{
  SArray<MbCartPoint> corners( 4, 1 );
  MbCartPoint corner;
  rect.GetVertex( 0, corner );
  corners.Add( corner );
  rect.GetVertex( 1, corner );
  corners.Add( corner );
  rect.GetVertex( 2, corner );
  corners.Add( corner );
  rect.GetVertex( 3, corner );
  corners.Add( corner );
  MbContour contour;
  contour.InitByPoints( corners );
  DrawGI::DrawItem( &contour, &surf, r, g, b );
}

#endif // defined(_DRAWGI)


#endif // __ALG_DRAW_H
