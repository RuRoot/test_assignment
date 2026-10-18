//////////////////////////////////////////////////////////////
//
// Реализация интерфейса отладочной отрисовки приложения.
//
//////////////////////////////////////////////////////////////

#ifndef __TEST_DRAW_H
#define __TEST_DRAW_H


#include <alg_draw.h>
#include <mesh.h>


// Использовать при необходимости, т.к. ускоряет отрисовку, но неверно работает, если подаётся другой объект по тому же адресу памяти.
//#define USE_WRK_MESH

//------------------------------------------------------------------------------
// Реализация интерфейса отладочной отрисовки приложения.
// ---
class IfDrawGIImplement : public IfDrawGI {
private:
#ifdef USE_WRK_MESH
  MbMesh wrkMesh;
#endif
  CommonMutex wndSec;
public:
#ifdef USE_WRK_MESH
  IfDrawGIImplement() : wndSec(  ),  wrkMesh() {}
#else
  IfDrawGIImplement() : IfDrawGI(),  wndSec() { }
#endif

  ~IfDrawGIImplement() {}
  
public:
  // Отрисовать объект.
  virtual void DrawItem( const MbRefItem * ri, int R, int G, int B, const MbMatrix3D & from = MbMatrix3D::identity, int width = 1 );
  // Отрисовать трехмерный геометрический объект.
  virtual void DrawItem( const MbSpaceItem * gi, int R, int G, int B, int width = 1 );
  // Отрисовать трехмерный геометрический объект с размещением по матрице.
  virtual void DrawItem( const MbSpaceItem * gi, int R, int G, int B, const MbMatrix3D & from, int width = 1 );
  // Отрисовать сетку геометрических объектов.
  virtual void DrawMesh( const MbMesh * ms, int R, int G, int B, int width = 1 );
  // Отрисовать полигон.
  virtual void DrawPolygon( const MbPolygon3D * polygon, int R, int G, int B, int width = 1 );
  // Отрисовать трехмерную точку.
  virtual void DrawItem( const MbCartPoint3D & gi, int R, int G, int B, int width = 2 );
  // Отрисовать трехмерную точку.
  virtual void DrawPoint( const MbCartPoint3D * gi, int R, int G, int B, int width = 2 );
  // Отрисовать трехмерный отрезок.
  virtual void DrawLine( const MbCartPoint3D & q1, const MbCartPoint3D & q2, int R, int G, int B, int width = 1 );
  // Отрисовать полилинию.
  virtual void DrawPolyline( SArray<MbCartPoint3D> & points, int R, int G, int B, int width = 1 );
  // Отрисовать трехмерный отрезок.
  virtual void DrawLine( const MbCartPoint3D & p, const MbVector3D & v, int R, int G, int B, int width = 1 );
  // Отрисовать трехмерный отрезок.
  virtual void DrawLine( const MbFloatPoint3D & q1, const MbFloatPoint3D & q2, int R, int G, int B, int width = 1 );
  // Отрисовать участок трехмерной кривой.
  virtual void DrawCurve( const MbCurve3D & curve, double t1, double t2, int R, int G, int B, int width = 1 );
  // Отрисовать часть двумерной кривой на плоскости.
  virtual void DrawCurve( const MbCurve & curve, const MbPlacement3D & place, double t1, double t2, int R, int G, int B, int width = 1 );
  // Отрисовать двумерный геометрический объект.
  virtual void DrawItem( const MbPlaneItem * gi, int R, int G, int B, const MbMatrix3D & from, int width = 1 );
  // Отрисовать двумерный геометрический объект.
  virtual void DrawItem( const MbPlaneItem * gi, int R, int G, int B, int width = 1 );
  // Отрисовать двумерную точку.
  virtual void DrawPoint( const MbCartPoint * gi, int R, int G, int B, const MbMatrix3D & from, int width = 1 );
  // Отрисовать двумерную точку.
  virtual void DrawPoint( const MbCartPoint * gi, int R, int G, int B, int width = 1 );
  // Отрисовать двумерный отрезок.
  virtual void DrawLine( const MbCartPoint & q1, const MbCartPoint & q2, int R, int G, int B, const MbMatrix3D & from, int width = 1 );
  // Отрисовать трехмерный отрезок.
  virtual void DrawLine( const MbCartPoint & q1, const MbCartPoint & q2, int R, int G, int B, int width = 1 );
  /// \ru Отрисовать топологический объект. \en Draw a topological object. 
  virtual void DrawItem( const MbTopItem * ti, int R, int G, int B, const MbMatrix3D & from = MbMatrix3D::identity, int width = 1 );
  // Отрисовать ребро.
  virtual void DrawEdge( const MbEdge * edge, int r, int g, int b, bool drawVerts, int width = 1 );
  // Отрисовать патч двумерной кривой.
  virtual void PutPatch( const MbCartPoint & pnt, const MbVector & dir, double a, double b, int R, int G, int B, const MbMatrix3D & mapInto );
  // Отрисовать габаритный куб в текущем окне.
  virtual void PutCube( const MbCube & gab, int width = 1, bool bDrawRed = true );
  // Отрисовать систему координат.
  virtual void DrawPlacement3D( const MbPlacement3D & place, double lenAxes, int width = 1 );
  // Очистить текущее окно.
  virtual void DrawClearMap();
  // Отрисовать двумерную кривую на поверхности.
  virtual void DrawItem( const MbCurve * curve, const MbSurface * surf, int R, int G, int B, int width = 1 );
  // Отрисовать карту кривой на поверхности.
  virtual void DrawCurveMap( const MbCurve * curve, const MbSurface * surface, int R, int G, int B );
  // Отрисовать карту кривой на поверхности в заранее заданном масштабе. Масштаб выбирается таким образом, чтобы активная область окна покрывалась заданным прямоугольником rect. Если isotropic==true, значит объект будет рисоваться в равных масштабах по осям. В противном случае окружность может отобразиться как эллипс, квадрат как прямоугольник и т.п.
  virtual void DrawCurveMap( const MbCurve * curve, const MbSurface * surface, const MbRect & rect, int R, int G, int B, const int boundaryWidth, const int curveWidth, const bool isotropic, const bool showSurfaceBoundaries );
  // Отрисовать кривую на поверхности.
  virtual void DrawSurfaceCurveMap( const MbSurfaceCurve * scurve, int R, int G, int B );
  // Отрисовать точку на поверхности.
  virtual void DrawPointMap( const MbCartPoint * pnt, const MbSurface * surface, int R, int G, int B );
  // Отрисовать кривую пересечения на параметрической плоскости.
  virtual void DrawSurfaceIntersectionMap( const MbSurfaceIntersectionCurve * gi, int R1, int G1, int B1, int R2, int G2, int B2 );
  // Отрисовать контур на поверхности в параметрической плоскости.
  virtual void DrawContourOnSurfaceMap( const MbContourOnSurface * gi, int R, int G, int B );
  // Отрисовать ограничивающие кривые усеченной поверхности в ее параметрической плоскости.
  virtual void DrawCurveBoundedSurfaceMap( const MbCurveBoundedSurface * bnds, int R, int G, int B );
  // Отрисовать параметрическую плоскость поверхности.
  virtual void DrawSurfaceMap( const MbSurface * surface, int R, int G, int B );
  // Отрисовать ограничивающие кривые грани в ее параметрической плоскости.
  virtual void DrawFaceMap( const MbFace *, int R, int G, int B );
  // Отрисовать параметрическую точку поверхности.
  virtual void DrawPoint( const MbSurface & surface, const MbCartPoint & uv, int R, int G, int B, int width = 2 );
  // Отрисовать множество 3d-точек по множеству 2d-точек и поверхности.
  virtual void DrawPoints( const MbSurface & surface, const SArray<MbCartPoint> & uvArr, int R, int G, int B, int width = 2 );
  // Отрисовать множество 3d-точек по множеству параметров и кривой.
  virtual void DrawPoints( const MbCurve3D & curve, const SArray<double> & tArr, int R, int G, int B, int width = 2 );
  // Отрисовать множество 3d-точек.
  virtual void DrawPoints( const SArray<MbCartPoint3D> & pnts, int R, int G, int B, int width = 2 );
  // Отрисовать триангуляцию.
  virtual void PutGrid( const MbGrid & grid, int R, int G, int B, int width = 1 );
  // Отрисовать треугольник.
  virtual void PutTriangle( const MbGrid & grid, ptrdiff_t index, int R, int G, int B );
  // Отрисовать треугольник.
  virtual void PutTriangle( const MbSurface & surface,
                            const MbGrid & grid, ptrdiff_t index, int R, int G, int B );
  // Отрисовать треугольник.
  virtual void PutTriangle( const MbCollection & coll, ptrdiff_t index, int R, int G, int B );
  // Отрисовать треугольник.
  virtual void PutTriangle( const MbSurface & surface, 
                            const MbCartPoint & uv0, const MbCartPoint & uv1, const MbCartPoint & uv2, 
                            int R, int G, int B );
  // Отрисовать четырёхугольник.
  virtual void PutQuadrangle( const MbGrid & grid, ptrdiff_t index, int R, int G, int B );
  // Отрисовать четырёхугольник.
  virtual void PutQuadrangle( const MbSurface & surface,
                              const MbGrid & grid, ptrdiff_t index, int R, int G, int B );
  // Отрисовать двумерную триангуляцию.
  virtual void PutPlanarGrid( MbPlanarGrid & grid, const MbPlacement3D & place, int R, int G, int B, int width = 1 );
  // Отрисовать триангуляционную сетку на поверхности.
  virtual void DrawGridMap( const MbGrid & grid, const MbSurface & surface, int R, int G, int B ); 
  // Стереть модель
  virtual void EraseModel();
  // Перерисовать модель.
  virtual void RedrawModel();

  // Получить плейсмент активного окна (например для прорисовки плоской кривой).
  virtual void GetWindowPlace( MbPlacement3D & place );

private:

  // Отрисовать трехмерный геометрический объект.
  void DrawItemUnsafe( const MbSpaceItem * gi, int R, int G, int B, const MbMatrix3D & from, int width );

  // Отрисовка двумерных геометрических объектов заданным цветом в текущем окне.
  void DrawItemUnsafe( const MbPlaneItem * gi, int R, int G, int B, const MbMatrix3D & from, int width );

  // Отрисовка топологических объектов заданным цветом в текущем окне.
  void DrawItemUnsafe( const MbTopItem * ti, int R, int G, int B, const MbMatrix3D & from, int width );
  
  // Отрисовать трехмерный геометрический объект.
  void DrawPointUnsafe( const MbCartPoint3D * gi, int R, int G, int B, int width );

  // Функция отрисовки отрезка заданным цветом в текущем окне
  void DrawLineUnsafe( const MbCartPoint3D & q1, const MbCartPoint3D & q2, int R, int G, int B, int width );
  
  // Функция отрисовки двумерных кривых на поверхности
  void DrawItemUnsafe( const MbCurve * curve, const MbSurface * surface, int R, int G, int B, MbPolygon3D * extraPoly, int width );

  // Функция отрисовки карты усеченной кривыми поверхности в текущем окне
  void DrawCurveBoundedSurfaceMapUnsafe( const MbCurveBoundedSurface * surface, int R, int G, int B, int width = 1 );

  // Отрисовка сетки геометрических объектов заданным цветом в текущем окне.
  void DrawMeshUnsafe( const MbMesh * ms, int R, int G, int B, int width );

  // Захватить отрисовочный мьютекс в многопоточном режиме
  void LockDraw();

  // Освободить отрисовочный мьютекс в многопоточном режиме
  void ReleaseDraw();

OBVIOUS_PRIVATE_COPY( IfDrawGIImplement )
};

#undef USE_WRK_MESH


#endif // __TEST_DRAW_H
