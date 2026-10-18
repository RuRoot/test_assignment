#include <test_draw.h>
#include <test_gr_draw.h>
#include <test_variables.h>
#include <test_window.h>
#include <test_manager.h>
#include <cur_line_segment.h>
#include <cur_plane_curve.h>
#include <cur_surface_intersection.h>
#include <surf_curve_bounded_surface.h>
#include <mb_homogeneous3d.h>
#include <region.h>
#include <mesh_plane_grid.h>
#include <mesh_triangle.h>
#include <solid.h>
#include <point_frame.h>
#include <collection.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Преобразование по матрице
// ---
static void TransformP( const MbMatrix3D & matr, /*int32 * viewPort,*/ MbCartPoint3D & pnt )
{
  MbHomogeneous3D p( pnt.x, pnt.y, pnt.z, 1 );
  p.Transform( matr );

  if ( p.w != 0 ) {
    double kw = 1.0 / p.w;
    p.x *= kw;
    p.y *= kw;
  }

  int32 viewPort[4];  //-V112
  viewPort[0] = -1;
  viewPort[1] = -1;
  viewPort[2] =  2;
  viewPort[3] = -2;

  pnt.x = ( 1 + p.x ) * ( viewPort[2] * 0.5 ) + viewPort[0];
  pnt.y = ( 1 - p.y ) * ( viewPort[3] * 0.5 ) - viewPort[1];
}


//------------------------------------------------------------------------------
// Преобразование по матрице
// ---
static void TransformP( const MbMatrix3D & matr, /*int32 * viewPort,*/ MbCartPoint & pnt )  //-V112
{
  double x = pnt.x * matr.El( 0, 0 ) + pnt.y * matr.El( 1, 0 ) + matr.El( 3, 0 );
  double y = pnt.x * matr.El( 0, 1 ) + pnt.y * matr.El( 1, 1 ) + matr.El( 3, 1 );
  double w = pnt.x * matr.El( 0, 3 ) + pnt.y * matr.El( 1, 3 ) + matr.El( 3, 3 );

  if ( w != 0 ) {
    double kw = 1.0 / w;
    x *= kw;
    y *= kw;
  }

  int32 viewPort[4];  //-V112
  viewPort[0] = -1;
  viewPort[1] = -1;
  viewPort[2] =  2;
  viewPort[3] = -2;

  pnt.x = ( 1 + x ) * ( viewPort[2] * 0.5 ) + viewPort[0];
  pnt.y = ( 1 - y ) * ( viewPort[3] * 0.5 ) - viewPort[1];
}


////////////////////////////////////////////////////////////////////////////////
//
//                     Отрисовка геометрических объектов
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Функция отрисовки трёхмерных объектов
// ---
static void DrawSpaceItem( IGrDraw & tool, const MbSpaceItem * pObj, const MbMatrix3D & into, MbMesh & mesh )
{
  if ( pObj != nullptr ) {
    MbCartPoint3D pnt;
    POINT pd;

    //MbMesh mesh;
    if ( !mesh.IsComplete() || (mesh.GetRefItem() != pObj) ) {
      mesh.Flush();
      mesh.SetRefItem( nullptr );
      MbStepData stepData( ist_SpaceStep, Math::visualSag );
      pObj->CalculateMesh( stepData, MbFormNote(true, false, true, TestVariables::exactMesh), mesh );
    }
    size_t i = 0, icnt = 0;

    for ( i = 0, icnt = mesh.PolygonsCount(); i < icnt; i++ ) {
      const MbPolygon3D * polygon = mesh.GetPolygon( i );
      if ( polygon->Count() ) {
        polygon->GetPoint( 0, pnt );
        TransformP( into, /*viewPort,*/ pnt ); // Перевод в экранную СК
        pd.x = (int32)pnt.x;
        pd.y = (int32)pnt.y;
        tool.MoveTo( pd.x, pd.y );
        for ( size_t j = 1, jcnt = polygon->Count(); j < jcnt; j++ ) {
          polygon->GetPoint( j, pnt );
          TransformP( into, /*viewPort,*/ pnt ); // Перевод в экранную СК
          pd.x = (int32)pnt.x;
          pd.y = (int32)pnt.y;
          tool.LineTo( pd.x, pd.y );
        }
      }
    }

    for ( i = 0, icnt = mesh.ApexesCount(); i < icnt; i++ ) {
      const MbApex3D * apex = mesh.GetApex( i );
      apex->GetPoint( pnt );
      TransformP( into, /*viewPort,*/ pnt ); // Перевод в экранную СК

      pd.x = (int32)pnt.x;
      pd.y = (int32)pnt.y;
      pd.x -= (int32)( TestVariables::drawFactor/2.0 );
      pd.y -= (int32)( TestVariables::drawFactor/2.0 );
      tool.MoveTo( pd.x, pd.y );

      pd.y += TestVariables::drawFactor;
      tool.LineTo( pd.x, pd.y );

      pd.x += TestVariables::drawFactor;
      tool.LineTo( pd.x, pd.y );

      pd.y -= TestVariables::drawFactor;
      tool.LineTo( pd.x, pd.y );

      pd.x -= TestVariables::drawFactor;
      tool.LineTo( pd.x, pd.y );
    }
    tool.UpdateWindow();
  }
}

//------------------------------------------------------------------------------
// Отрисовка трёхмерных геометрических объектов заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawItem( const MbRefItem * ri, int R, int G, int B, const MbMatrix3D & from, int width )
{
  if ( ri != nullptr ) 
  {
    switch ( ri->RefType() ) 
    {
      case rt_PlaneItem : {
        DrawItem( (MbPlaneItem *)ri, R, G, B, from, width );
      } break;
      case rt_SpaceItem : {
        DrawItem( (MbSpaceItem *)ri, R, G, B, from, width );
      } break;
      case rt_TopItem : {
        DrawItem( (MbTopItem *)ri, R, G, B, from, width );
      } break;
      default:break;
    }
  }
}


//------------------------------------------------------------------------------
// Отрисовка трёхмерных геометрических объектов заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawItem( const MbSpaceItem * gi, int R, int G, int B, int width )
{
  DrawItem( gi, R, G, B, MbMatrix3D::identity, width );
}

//------------------------------------------------------------------------------
// Отрисовка трёхмерных геометрических объектов заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawItem( const MbSpaceItem * gi, int R, int G, int B, const MbMatrix3D & from, int width )
{
  LockDraw();
  DrawItemUnsafe( gi, R, G, B, from, width );
  ReleaseDraw();
}

//------------------------------------------------------------------------------
// Функция отрисовки сетки
// ---
static void DrawMeshEx( IGrDraw & tool/*HDC dc*/, const MbMesh * mesh, const MbMatrix3D & into/*, int32 * viewPort*/ )
{
  if ( mesh != nullptr ) {
    MbCartPoint3D p0, p1, p2, p3;
    POINT pd;
    size_t i( 0 ), icnt( 0 );

    for ( i = 0, icnt = mesh->PolygonsCount(); i < icnt; i++ ) {
      const MbPolygon3D * polygon = ((MbMesh *)mesh)->GetPolygon( i );

      if ( polygon->Count() ) {
        polygon->GetPoint( 0, p0 );
        TransformP( into, /*viewPort,*/ p0 ); // Перевод в экранную СК
        pd.x = (int32)p0.x;
        pd.y = (int32)p0.y;
        tool.MoveTo( pd.x, pd.y );

        for ( size_t j = 1, jCount = polygon->Count(); j < jCount; j++ ) {
          polygon->GetPoint( j, p0 );
          TransformP( into, /*viewPort,*/ p0 ); // Перевод в экранную СК
          pd.x = (int32)p0.x;
          pd.y = (int32)p0.y;
          tool.LineTo( pd.x, pd.y );
        }
      }
    }
    for ( i = 0, icnt = mesh->GridsCount(); i < icnt; i++ ) {
      const MbGrid * grid = ((MbMesh *)mesh)->GetGrid( i );
      size_t j( 0 ), jCount( 0 );
      // Треугольники.
      for ( j = 0, jCount = grid->TrianglesCount(); j < jCount; j++ ) {
        if ( grid->GetTrianglePoints( j, p0, p1, p2 ) ) {
          TransformP( into, /*viewPort,*/ p0 ); // Перевод в экранную СК
          TransformP( into, /*viewPort,*/ p1 ); // Перевод в экранную СК
          TransformP( into, /*viewPort,*/ p2 ); // Перевод в экранную СК
          pd.x = (int32)p0.x;
          pd.y = (int32)p0.y;
          tool.MoveTo( pd.x, pd.y );

          pd.x = (int32)p1.x;
          pd.y = (int32)p1.y;
          tool.LineTo( pd.x, pd.y );

          pd.x = (int32)p2.x;
          pd.y = (int32)p2.y;
          tool.LineTo( pd.x, pd.y );

          pd.x = (int32)p0.x;
          pd.y = (int32)p0.y;
          tool.LineTo( pd.x, pd.y );
        }
      }
      // Четырехугольники.
      for ( j = 0, jCount = grid->QuadranglesCount(); j < jCount; j++ ) {
        if ( grid->GetQuadranglePoints ( j, p0, p1, p2, p3 ) ) {
          TransformP( into, /*viewPort,*/ p0 ); // Перевод в экранную СК
          TransformP( into, /*viewPort,*/ p1 ); // Перевод в экранную СК
          TransformP( into, /*viewPort,*/ p2 ); // Перевод в экранную СК
          TransformP( into, /*viewPort,*/ p3 ); // Перевод в экранную СК
          pd.x = (int32)p0.x;
          pd.y = (int32)p0.y;
          tool.MoveTo( pd.x, pd.y );

          pd.x = (int32)p1.x;
          pd.y = (int32)p1.y;
          tool.LineTo( pd.x, pd.y );

          pd.x = (int32)p2.x;
          pd.y = (int32)p2.y;
          tool.LineTo( pd.x, pd.y );

          pd.x = (int32)p3.x;
          pd.y = (int32)p3.y;
          tool.LineTo( pd.x, pd.y );

          pd.x = (int32)p0.x;
          pd.y = (int32)p0.y;
          tool.LineTo( pd.x, pd.y );
        }
      }
    }
    tool.UpdateWindow();
  }
}


//------------------------------------------------------------------------------
// Отрисовка сетки геометрических объектов заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawMesh( const MbMesh * ms, int R, int G, int B, int width )
{
  LockDraw();
  DrawMeshUnsafe( ms, R, G, B, width );
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Отрисовка полигона заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawPolygon( const MbPolygon3D * polygon, int R, int G, int B, int width )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( (polygon != nullptr) && (wnd != nullptr) ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw & tool = wnd->GetDrawTool();
    tool.SelectPen( width, R, G, B );

    if ( polygon->Count() > 0 ) {
      MbCartPoint3D p0, p1, p2, p3;
      POINT pd;

      polygon->GetPoint( 0, p0 );
      TransformP( into, /*viewPort,*/ p0 ); // Перевод в экранную СК
      pd.x = (int32)p0.x;
      pd.y = (int32)p0.y;
      tool.MoveTo( pd.x, pd.y );
      for ( size_t j = 1, jCount = polygon->Count(); j < jCount; j++ ) {
        polygon->GetPoint( j, p0 );
        TransformP( into, /*viewPort,*/ p0 ); // Перевод в экранную СК
        pd.x = (int32)p0.x;
        pd.y = (int32)p0.y;
        tool.LineTo( pd.x, pd.y );
      }
    }

    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки геометрических объектов
// Заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawItem( const MbCartPoint3D & gi, int R, int G, int B, int width )
{
  LockDraw();
  DrawPointUnsafe( &gi, R, G, B, width );
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки геометрических объектов
// Заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawPoint( const MbCartPoint3D * gi, int R, int G, int B, int width )
{
  LockDraw();
  DrawPointUnsafe( gi, R, G, B, width );
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки отрезка заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawLine( const MbCartPoint3D & q1, const MbCartPoint3D & q2, int R, int G, int B, int width )
{
  LockDraw();
  DrawLineUnsafe( q1, q2, R, G, B, width );
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки полилинии заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawPolyline( SArray<MbCartPoint3D> & points, int R, int G, int B, int width )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if (wnd != nullptr) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw& tool = wnd->GetDrawTool();
    tool.SelectPen( width, R, G, B );

    POINT pb, pd;

    if ( points.Count() > 1 ) {
      MbCartPoint3D p0;

      p0 = points[0];
      TransformP( into, /*viewPort,*/ p0 );
      pb.x = (int32)p0.x;
      pb.y = (int32)p0.y;

      tool.MoveTo( pb.x, pb.y );

      for ( size_t i = 1, iCount = points.Count(); i < iCount; i++ ) {
        p0 = points[i];
        TransformP( into, /*viewPort,*/ p0 );
        pd.x = (int32)p0.x;
        pd.y = (int32)p0.y;

        tool.LineTo( pd.x, pd.y );
      }
    }

    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки отрезка заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawLine( const MbCartPoint3D & p1, const MbVector3D & v, int R, int G, int B, int width )
{
  LockDraw();
  MbCartPoint3D p2( p1 );
  p2 += v;
  DrawLineUnsafe( p1, p2, R, G, B, width );
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки отрезка заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawLine( const MbCartPoint & q1, const MbCartPoint & q2, int R, int G, int B, const MbMatrix3D & from, int width )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();
  if ( wnd != nullptr ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw& tool = wnd->GetDrawTool();
    tool.SelectPen( width, R, G, B );

    MbCartPoint3D p1(q1.x,q1.y,0), p2(q2.x,q2.y,0);
    POINT pb, pd;

    MbMatrix3D into_( from*into );

    TransformP( into_, /*viewPort,*/ p1 );
    TransformP( into_, /*viewPort,*/ p2 );

    pb.x = (int32)p1.x;
    pb.y = (int32)p1.y;
    pd.x = (int32)p2.x;
    pd.y = (int32)p2.y;

    tool.MoveTo( pb.x, pb.y );
    tool.LineTo( pd.x, pd.y );

    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки отрезка заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawLine( const MbFloatPoint3D & q1, const MbFloatPoint3D & q2, int R, int G, int B, int width )
{
  LockDraw();
  MbCartPoint3D p1, p2;
  q1.GetCartPoint( p1 );
  q2.GetCartPoint( p2 );
  DrawLineUnsafe( p1, p2, R, G, B, width );
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки полигона
// ---
static void DrawPolygonEx( IGrDraw& tool/*HDC dc*/, const MbPolygon3D & poly, const MbMatrix3D & into/*, int32 * viewPort*/ )
{
  POINT pd;
  MbCartPoint3D cp;

  poly.GetPoint( 0, cp );
  TransformP( into, /*viewPort,*/ cp );
  pd.x = (int32)cp.x;
  pd.y = (int32)cp.y;
  tool.MoveTo( pd.x, pd.y );

  size_t n = poly.Count();
  for ( size_t i = 1; i < n; i++ ) {
    poly.GetPoint( i, cp );
    TransformP( into, /*viewPort,*/ cp );
    pd.x = (int32)cp.x;
    pd.y = (int32)cp.y;
    tool.LineTo( pd.x, pd.y );
  }
}


//------------------------------------------------------------------------------
// Функция отрисовки части кривой
// ---
void IfDrawGIImplement::DrawCurve( const MbCurve3D & curve, double t1, double t2, int R, int G, int B, int width )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( wnd != nullptr ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw& tool = wnd->GetDrawTool();
    tool.SelectPen( width, R, G, B );

    // Рассчитать полигон по параметру
    MbFloatPolygon3D polygon;
    {
      if ( t2 < t1 )
        std::swap( t1, t2 );

      MbCartPoint3D pnt;
      double sag = Math::visualSag;
      double t = t1;
      curve._PointOn( t, pnt );
      polygon.AddPoint( pnt );

      bool addNext = true;
      while ( addNext ) {
        t += curve.Step ( t, sag );
        if ( t >= t2-Math::paramEpsilon ) {
          t = t2;
          addNext = false;
        }
        curve._PointOn( t, pnt );
        polygon.AddPoint( pnt );
      }
      polygon.Adjust();
    }

    DrawPolygonEx( tool, polygon, into/*, viewPort*/ );

    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки части двумерной кривой на плоскости
// ---
void IfDrawGIImplement::DrawCurve( const MbCurve & curve, const MbPlacement3D & place, double t1, double t2, int R, int G, int B, int width )
{
//  LockDraw();
  // Сделать трехмерную кривую
  MbPlaneCurve * planeCurve = new MbPlaneCurve( place, curve, false );
  DrawCurve( *planeCurve, t1, t2, R, G, B, width );
  delete planeCurve;
//  ::ReleaseDraw();
}


////////////////////////////////////////////////////////////////////////////////
//
//                     Отрисовка двумерных объектов
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Функция отрисовки двумерных объектов
// ---
static void DrawPlaneItem( IGrDraw& tool/*HDC dc*/, const MbPlaneItem * pObj, const MbMatrix3D & into/*, int32 * viewPort*/ )
{
  if ( pObj != nullptr ) {
    MbCartPoint3D pnt;
    MbCartPoint pp;
    POINT pd;
    MbePlaneType type = pObj->IsA();

    switch ( type ) {
      case pt_ContourWithBreaks :
      case pt_Contour : { // Контур
        const MbContour * contour = (const MbContour *)pObj;
        for ( size_t i = 0; i < contour->GetSegmentsCount(); i++ ) {
          const MbCurve * curve = contour->GetSegment( i );

          MbPolygon polygon;
          curve->CalculatePolygon( Math::visualSag, polygon );

          polygon.GetPoint( 0, pp );
          pnt.Init(pp.x,pp.y,0);
          TransformP( into, /*viewPort,*/ pnt ); // Перевод в экранную СК
          pd.x = (int32)pnt.x;
          pd.y = (int32)pnt.y;
          tool.MoveTo( pd.x, pd.y );
          for ( size_t j = 1, jcnt = polygon.Count(); j < jcnt; j++ ) {
            polygon.GetPoint( j, pp );
            pnt.Init( pp.x, pp.y, 0 );
            TransformP( into, /*viewPort,*/ pnt ); // Перевод в экранную СК
            pd.x = (int32)pnt.x;
            pd.y = (int32)pnt.y;
            tool.LineTo( pd.x, pd.y );
          }
        }
      } break;

      case pt_Region : { // Контур
        const MbRegion * region = (const MbRegion *)pObj;
        size_t i, count = region->GetContoursCount();

        for ( i = 0; i < count; i++ ) {
          MbPolygon polygon;
          region->GetContour(i)->CalculatePolygon( Math::visualSag, polygon );

          polygon.GetPoint( 0, pp );
          pnt.Init(pp.x,pp.y,0);
          TransformP( into, /*viewPort,*/ pnt ); // Перевод в экранную СК
          pd.x = (int32)pnt.x;
          pd.y = (int32)pnt.y;
          tool.MoveTo( pd.x, pd.y );

          for ( size_t j = 1, jcnt = polygon.Count(); j < jcnt; j++ ) {
            polygon.GetPoint( j, pp );
            pnt.Init(pp.x,pp.y,0);
            TransformP( into, /*viewPort,*/ pnt ); // Перевод в экранную СК
            pd.x = (int32)pnt.x;
            pd.y = (int32)pnt.y;
            tool.LineTo( pd.x, pd.y );
          }
        }
      } break;

      default : {
        if ( type>=pt_Curve && type<=pt_FreeCurve ) {
          const MbCurve * curve = (const MbCurve *)pObj;

          MbPolygon polygon;
          curve->CalculatePolygon( Math::visualSag, polygon );

          polygon.GetPoint( 0, pp );
          pnt.Init(pp.x,pp.y,0);
          TransformP( into, /*viewPort,*/ pnt ); // Перевод в экранную СК
          pd.x = (int32)pnt.x;
          pd.y = (int32)pnt.y;
          tool.MoveTo( pd.x, pd.y );

          for ( size_t j = 1, jcnt = polygon.Count(); j < jcnt; j++ ) {
            polygon.GetPoint( j, pp );
            pnt.Init(pp.x,pp.y,0);
            TransformP( into, /*viewPort,*/ pnt ); // Перевод в экранную СК
            pd.x = (int32)pnt.x;
            pd.y = (int32)pnt.y;
            tool.LineTo( pd.x, pd.y );
          }
        }
      } break;
    }
  }
}


//------------------------------------------------------------------------------
// Отрисовка двумерных геометрических объектов заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawItem( const MbPlaneItem * gi, int R, int G, int B, const MbMatrix3D & from, int width )
{
  LockDraw();
  DrawItemUnsafe( gi, R, G, B, from, width );
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки геометрических объектов
// Заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawItem( const MbPlaneItem * gi, int R, int G, int B, int width )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( wnd != nullptr ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw& tool = wnd->GetDrawTool();
    tool.SelectPen( width, R, G, B );

    DrawPlaneItem( tool, gi, into/*, viewPort*/ );

    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки геометрических объектов
// Заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawPoint( const MbCartPoint * gi, int R, int G, int B, const MbMatrix3D & from, int width )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( gi != nullptr && wnd != nullptr ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw& tool = wnd->GetDrawTool();
    tool.SelectPen( width, R, G, B );

    MbCartPoint3D pnt(gi->x,gi->y,0);
    POINT pd;

    MbMatrix3D into_( from*into );

    TransformP( into_, /*viewPort,*/ pnt ); // Перевод в экранную СК

    pd.x = (int32)pnt.x;
    pd.y = (int32)pnt.y;
    pd.x -= (int32)( TestVariables::drawFactor/2.0 );
    pd.y -= (int32)( TestVariables::drawFactor/2.0 );
    tool.MoveTo( pd.x, pd.y );

    pd.y += TestVariables::drawFactor;
    tool.LineTo( pd.x, pd.y );

    pd.x += TestVariables::drawFactor;
    tool.LineTo( pd.x, pd.y );

    pd.y -= TestVariables::drawFactor;
    tool.LineTo( pd.x, pd.y );

    pd.x -= TestVariables::drawFactor;
    tool.LineTo( pd.x, pd.y );

    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки геометрических объектов
// Заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawPoint( const MbCartPoint * gi, int R, int G, int B, int width )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( wnd != nullptr ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw& tool = wnd->GetDrawTool();
    tool.SelectPen( width, R, G, B );

    MbCartPoint pnt(gi->x,gi->y);
    POINT pd;

    TransformP( into, /*viewPort,*/ pnt ); // Перевод в экранную СК
    double delta = TestVariables::drawFactor/2.0;
    pd.x = (int32)pnt.x;
    pd.y = (int32)pnt.y;
    pd.x -= (int32)(delta/2.0);
    pd.y -= (int32)(delta/2.0);
    tool.MoveTo( pd.x, pd.y );

    pd.y += (int32)delta;
    tool.LineTo( pd.x, pd.y );

    pd.x += (int32)delta;
    tool.LineTo( pd.x, pd.y );

    pd.y -= (int32)delta;
    tool.LineTo( pd.x, pd.y );

    pd.x -= (int32)delta;
    tool.LineTo( pd.x, pd.y );

    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки отрезка заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawLine( const MbCartPoint & q1, const MbCartPoint & q2, int R, int G, int B, int width )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( wnd != nullptr ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw& tool = wnd->GetDrawTool();
    tool.SelectPen( width, R, G, B );

    MbCartPoint p1(q1), p2(q2);
    POINT pb, pd;

    // KYA K13 Перестало быть нужным // ::ViewMatrixMultiply( into );

    TransformP( into, /*viewPort,*/ p1 );
    TransformP( into, /*viewPort,*/ p2 );
    pb.x = (int32)p1.x;
    pb.y = (int32)p1.y;
    pd.x = (int32)p2.x;
    pd.y = (int32)p2.y;

    tool.MoveTo( pb.x, pb.y );
    tool.LineTo( pd.x, pd.y );

    tool.RestorePen();
  }
  ReleaseDraw();
}


////////////////////////////////////////////////////////////////////////////////
//
//                         Отрисовка топологических объектов
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Функция отрисовки топологических объектов заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawItem( const MbTopItem * ti, int R, int G, int B, const MbMatrix3D & from, int width )
{
  LockDraw();
  DrawItemUnsafe( ti, R, G, B, from, width );
  ReleaseDraw();
}

//------------------------------------------------------------------------------
// Отрисовать ребро (с вершинами)
// ---
void IfDrawGIImplement::DrawEdge( const MbEdge * edge, int r, int g, int b, bool drawVerts, int width )
{
  LockDraw();
  if ( edge != nullptr ) {

    DrawItemUnsafe( edge, r, g, b, MbMatrix3D::identity, width );
    if ( drawVerts ) { // Отрисовка начала и конца ребра, чтобы видеть короткие ребра
      MbCartPoint3D pShow1, pShow2;
      edge->GetBegVertexPoint( pShow1 );
      edge->GetEndVertexPoint( pShow2 );
      DrawPointUnsafe( &pShow1, r, g, b, 2*width );
      DrawPointUnsafe( &pShow2, r, g, b, 2*width );
    }
  }
  ReleaseDraw();
}


////////////////////////////////////////////////////////////////////////////////
//
//                     Отрисовка вспомогательных объектов
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Функция отрисовки отрезка
// ---
static void DrawLineEx( IGrDraw& tool/*HDC dc*/, MbCartPoint3D & p1, MbCartPoint3D & p2 )
{
  POINT pb, pd;

  pb.x = (int32)p1.x;
  pb.y = (int32)p1.y;
  pd.x = (int32)p2.x;
  pd.y = (int32)p2.y;

  tool.MoveTo( pb.x, pb.y );
  tool.LineTo( pd.x, pd.y );
}


//------------------------------------------------------------------------------
// Нарисовать патч двумерной кривой
// ---
void IfDrawGIImplement::PutPatch( const MbCartPoint & pnt, const MbVector & dir, double a, double b,
                                  int R, int G, int B, const MbMatrix3D & mapInto )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( wnd != nullptr ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw& tool = wnd->GetDrawTool();
    tool.SelectPen( 1/*width*/, R, G, B );

    MbMatrix3D mapFrom;
    mapInto.Div( mapFrom );
    MbMatrix3D into_( mapFrom*into );

    MbVector    axisA(dir);
    MbVector    axisB(dir);
    axisB.Perpendicular();
    axisA *= a;
    axisB *= b;
    MbCartPoint q1(pnt), q2, q3(pnt), q4;

    q1.Move( axisA );
    q2 = q1;
    q1.Move( axisB );
    q2.Move(-axisB );
    q3.Move(-axisA );
    q4 = q3;
    q3.Move(-axisB );
    q4.Move( axisB );

    MbCartPoint3D p1(q1.x,q1.y,0), p2(q2.x,q2.y,0), p3(q3.x,q3.y,0), p4(q4.x,q4.y,0);

    TransformP( into_, /*viewPort,*/ p1 );
    TransformP( into_, /*viewPort,*/ p2 );
    TransformP( into_, /*viewPort,*/ p3 );
    TransformP( into_, /*viewPort,*/ p4 );

    DrawLineEx( tool, p1, p2 );
    DrawLineEx( tool, p2, p3 );
    DrawLineEx( tool, p3, p4 );
    DrawLineEx( tool, p4, p1 );

    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Нарисовать габарит
// ---
void IfDrawGIImplement::PutCube( const MbCube & gab, int width, bool bDrawRed )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( wnd != nullptr ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw& tool = wnd->GetDrawTool();
    if ( bDrawRed )
      tool.SelectPen( width, TRGB_LIGHTRED );
    else
      tool.SelectPen( width, TRGB_LIGHTBLUE );

    MbCartPoint3D p1;
    MbCartPoint3D p2;

    p1 = gab.pmin;
    TransformP( into, /*viewPort,*/ p1 ); // Перевод в экранную СК

    p2 = gab.pmin;
    p2.x = gab.pmax.x;
    TransformP( into, /*viewPort,*/ p2 );
    DrawLineEx( tool, p1, p2 );

    p2 = gab.pmin;
    p2.y = gab.pmax.y;
    TransformP( into, /*viewPort,*/ p2 );
    DrawLineEx( tool, p1, p2 );

    p2 = gab.pmin;
    p2.z = gab.pmax.z;
    TransformP( into, /*viewPort,*/ p2 );
    DrawLineEx( tool, p1, p2 );

    p2 = gab.pmax;
    TransformP( into, /*viewPort,*/ p2 ); // Перевод в экранную СК

    p1 = gab.pmax;
    p1.x = gab.pmin.x;
    TransformP( into, /*viewPort,*/ p1 );
    DrawLineEx( tool, p1, p2 );

    p1 = gab.pmax;
    p1.y = gab.pmin.y;
    TransformP( into, /*viewPort,*/ p1 );
    DrawLineEx( tool, p1, p2 );

    p1 = gab.pmax;
    p1.z = gab.pmin.z;
    TransformP( into, /*viewPort,*/ p1 );
    DrawLineEx( tool, p1, p2 );

    p1 = gab.pmin;
    p1.x = gab.pmax.x;
    TransformP( into, /*viewPort,*/ p1 ); // Перевод в экранную СК

    p2 = gab.pmax;
    p2.y = gab.pmin.y;
    TransformP( into, /*viewPort,*/ p2 );
    DrawLineEx( tool, p1, p2 );

    p2 = gab.pmax;
    p2.z = gab.pmin.z;
    TransformP( into, /*viewPort,*/ p2 );
    DrawLineEx( tool, p1, p2 );

    p1 = gab.pmin;
    p1.y = gab.pmax.y;
    TransformP( into, /*viewPort,*/ p1 ); // Перевод в экранную СК

    p2 = gab.pmax;
    p2.z = gab.pmin.z;
    TransformP( into, /*viewPort,*/ p2 );
    DrawLineEx( tool, p1, p2 );

    p2 = gab.pmax;
    p2.x = gab.pmin.x;
    TransformP( into, /*viewPort,*/ p2 );
    DrawLineEx( tool, p1, p2 );

    p1 = gab.pmin;
    p1.z = gab.pmax.z;
    TransformP( into, /*viewPort,*/ p1 ); // Перевод в экранную СК

    p2 = gab.pmax;
    p2.x = gab.pmin.x;
    TransformP( into, /*viewPort,*/ p2 );
    DrawLineEx( tool, p1, p2 );

    p2 = gab.pmax;
    p2.y = gab.pmin.y;
    TransformP( into, /*viewPort,*/ p2 );
    DrawLineEx( tool, p1, p2 );

    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Отрисовать с.к.
// ---
void IfDrawGIImplement::DrawPlacement3D( const MbPlacement3D & place, double lenAxes, int width )
{
  LockDraw();
  lenAxes = ::fabs(lenAxes);
  if ( lenAxes < LENGTH_EPSILON )
    lenAxes = LENGTH_EPSILON;

  MbCartPoint3D p0( place.GetOrigin() );
  MbVector3D vx( place.GetAxisX() );
  MbVector3D vy( place.GetAxisY() );
  MbVector3D vz( place.GetAxisZ() );

  MbCartPoint3D px, py, pz;
  px.Set( p0, vx, lenAxes );
  py.Set( p0, vy, lenAxes );
  pz.Set( p0, vz, lenAxes );

  DrawPointUnsafe( &p0, TRGB_DARKGRAY, 2*width );
  DrawLineUnsafe( p0, px, TRGB_RED  , width );
  DrawLineUnsafe( p0, py, TRGB_GREEN, width );
  DrawLineUnsafe( p0, pz, TRGB_BLUE , width );
  ReleaseDraw();
}


////////////////////////////////////////////////////////////////////////////////
//
//         Отрисовка объектов в пространстве параметров поверхности
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Очистить текущее окно
// ---
void IfDrawGIImplement::DrawClearMap()
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( wnd != nullptr ) {
    wnd->GetDrawTool().FillRectangle();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Расчет полигона кривой на поверхности
// ---
static void CalculatePolygon( const MbCurve * curve, const MbSurface * surface, double sag, MbPolygon3D * polygon )
{
  if ( curve == nullptr || surface == nullptr )
    return;

  double t    = curve->GetTMin();
  double tmax = curve->GetTMax();

  // KYA K13 зачем сбрасывать?
  // KYA K13 if ( curve->IsA() >= pt_Polyline && curve->IsA() <= pt_Nurbs )
  // KYA K13   static_cast<MbPolyCurve *>(curve)->Refresh();

  curve->AddRef(); // Чтобы scurve ее не удалила
  surface->GetSurface().AddRef(); // Чтобы scurve ее не удалила
  {
    MbSurfaceCurve scurve( *surface, *curve, true ); //same

    polygon->SetItem( const_cast<MbCurve *>(curve) );
    polygon->SetPrimitiveType( rt_PlaneItem );
    polygon->SetStepBySag( sag );

    MbCartPoint3D pnt;
    scurve._PointOn( t, pnt );
    polygon->AddPoint( pnt );

    bool addNext = true;
    while ( addNext ) {
      double dt = scurve.Step( t, sag );
      t += dt;
      if ( t >= tmax - Math::paramEpsilon ) {
        t = tmax;
        addNext = false;
      }
      scurve._PointOn( t, pnt );
      polygon->AddPoint( pnt );
    }
  }

  curve->DecRef() ; // Восстановить счетчик
  surface->GetSurface().DecRef(); // Восстановить счетчик
}


//------------------------------------------------------------------------------
// Функция отрисовки двумерных кривых на поверхности
// ---
void IfDrawGIImplement::DrawItem( const MbCurve * curve, const MbSurface * surface, int R, int G, int B, int width )
{
  if ( curve == nullptr || surface == nullptr )
    return;
  if ( curve->Type() == pt_Contour ) { // Контур
    const MbContour * contour = static_cast<const MbContour *>(curve);
    for ( size_t i = 0; i < contour->GetSegmentsCount(); i++ )
      DrawItem( contour->GetSegment( i ), surface, R, G, B, width );
  }
  else {
    SPtr<MbPolygon3D> poly( new MbFloatPolygon3D() );
    ::CalculatePolygon( curve, surface, Math::visualSag, poly );
    poly->SetItem( const_cast<MbCurve *>(curve) );
    poly->SetPrimitiveType( rt_PlaneItem );
    poly->SetStepBySag( Math::visualSag );
    LockDraw();
    DrawItemUnsafe( curve, surface, R, G, B, poly, width );
    ReleaseDraw();
  }
}


//------------------------------------------------------------------------------
// Функция отрисовки кривой на поверхности в текущем окне
// ---
static void DrawPlaneCurve( IGrDraw & tool/*HDC dc*/,
                            const MbCurve & curve,
                            const MbVector & del,
                            POINT & p0,
                            double scaleX,
                            double scaleY )
{
  MbCartPoint pnt;
  MbVector vec;
  POINT pd;
  double t, t0;

  switch ( curve.IsA() ) {

   case pt_LineSegment : {
      t = curve.GetTMin();
      curve.PointOn( t, pnt );
      pnt.Move( del );
      pd.x = (int32)( p0.x + pnt.x * scaleX );
      pd.y = (int32)( p0.y + pnt.y * scaleY );
      tool.MoveTo( pd.x, pd.y );

      t = curve.GetTMax();
      curve.PointOn( t, pnt );
      pnt.Move( del );
      pd.x = (int32)( p0.x + pnt.x * scaleX );
      pd.y = (int32)( p0.y + pnt.y * scaleY );
      tool.LineTo( pd.x, pd.y );
    } break;

    case pt_Polyline : {
      t = curve.GetTMin();
      curve.PointOn( t, pnt );
      pnt.Move( del );
      pd.x = (int32)( p0.x + pnt.x * scaleX );
      pd.y = (int32)( p0.y + pnt.y * scaleY );
      tool.MoveTo( pd.x, pd.y );

      t0 = curve.GetTMax();
      while (t < t0-Math::paramEpsilon) {
        t += 1;
        if (t>t0) t = t0;
        curve.PointOn( t, pnt );
        pnt.Move( del );
        pd.x = (int32)( p0.x + pnt.x * scaleX );
        pd.y = (int32)( p0.y + pnt.y * scaleY );
        tool.LineTo( pd.x, pd.y );
      }
    } break;

    case pt_ContourWithBreaks :
    case pt_Contour : {
      MbContour &cntr = (MbContour &)curve;
      for ( size_t i = 0; i < cntr.GetSegmentsCount(); i++ )
        DrawPlaneCurve( tool, *cntr.SetSegment(i), del, p0, scaleX, scaleY );
    } break;

    default : {
      double koef = 1.0;
      double scale = std_max( scaleX, scaleY );
      if ( scale > 1.0 )
        koef = 1.0 / scale;

      t = curve.GetTMin();
      curve.PointOn( t, pnt );
      pnt.Move( del );
      pd.x = (int32)( p0.x + pnt.x * scaleX );
      pd.y = (int32)( p0.y + pnt.y * scaleY );
      tool.MoveTo( pd.x, pd.y );

      t0 = curve.GetTMax();
      while ( t < t0 - Math::paramEpsilon ) {
        double vs = curve.Step( t, koef*Math::visualSag );
        double ds = curve.Step( t, koef*Math::deviateSag );
        t += (( vs > ds ) ? vs : ds);
        if ( t > t0)
          t = t0;
        curve.PointOn( t, pnt );
        pnt.Move( del );
        pd.x = (int32)( p0.x + pnt.x * scaleX );
        pd.y = (int32)( p0.y + pnt.y * scaleY );
        tool.LineTo( pd.x, pd.y );
      }
    } break;
  }
}


//------------------------------------------------------------------------------
// Функция отрисовки кривой на поверхности в текущем окне
// ---
static void DrawSurfacePlane( IGrDraw& tool/*HDC dc*/,
                              const MbSurface & surface,
                              const MbVector & del,
                              POINT & p0,
                              double scaleX,
                              double scaleY )
{
  MbCartPoint pnt;
  MbVector vec;
  POINT pd;

  double umin = surface.GetUMin();
  double vmin = surface.GetVMin();
  double umax = surface.GetUMax();
  double vmax = surface.GetVMax();

  pnt.Init(umin,vmin);
  pnt.Move( del );
  pd.x = (int32)( p0.x + pnt.x * scaleX );
  pd.y = (int32)( p0.y + pnt.y * scaleY );
  tool.MoveTo( pd.x, pd.y );

  pnt.Init(umax,vmin);
  pnt.Move( del );
  pd.x = (int32)( p0.x + pnt.x * scaleX );
  pd.y = (int32)( p0.y + pnt.y * scaleY );
  tool.LineTo( pd.x, pd.y );

  pnt.Init(umax,vmax);
  pnt.Move( del );
  pd.x = (int32)( p0.x + pnt.x * scaleX );
  pd.y = (int32)( p0.y + pnt.y * scaleY );
  tool.LineTo( pd.x, pd.y );

  pnt.Init(umin,vmax);
  pnt.Move( del );
  pd.x = (int32)( p0.x + pnt.x * scaleX );
  pd.y = (int32)( p0.y + pnt.y * scaleY );
  tool.LineTo( pd.x, pd.y );

  pnt.Init(umin,vmin);
  pnt.Move( del );
  pd.x = (int32)( p0.x + pnt.x * scaleX );
  pd.y = (int32)( p0.y + pnt.y * scaleY );
  tool.LineTo( pd.x, pd.y );
}


//------------------------------------------------------------------------------
// Дать параметры пересчета в систему координат окна
// ---
static void GetScales( const WorkWindow * w, const MbCurve * curve, const MbSurface & surface,
                       MbVector & del, POINT & p0, double & scaleX, double & scaleY )
{
  RECT r;
  w->GetDrawTool().GetClientRect( r/*, true DPtoLP*/ );

  POINT pp;

  pp.x = (int32)( (r.right - r.left) * 0.1 );
  pp.y = (int32)( (r.top - r.bottom) * 0.1 );
  p0.x = r.left + pp.x;
  p0.y = r.bottom + pp.y;

  double umin = surface.GetUMin();
  double vmin = surface.GetVMin();
  double umax = surface.GetUMax();
  double vmax = surface.GetVMax();

  if ( curve != nullptr ) {
    MbRect rect;
    curve->AddYourGabaritTo( rect );
    umin = std_min(umin,rect.left);
    umax = std_max(umax,rect.right);
    vmin = std_min(vmin,rect.bottom);
    vmax = std_max(vmax,rect.top);
  }

  del.Init( -umin, -vmin);
  scaleX = double((r.right - r.left)-2*pp.x);
  scaleY = double((r.top - r.bottom)-2*pp.y);

  if ( ::fabs(umax - umin) > PARAM_EPSILON )
    scaleX /= umax - umin;
  if ( ::fabs(vmax - vmin) > PARAM_EPSILON )
    scaleY /= vmax - vmin;

}


//------------------------------------------------------------------------------
// Функция отрисовки карты кривой на поверхности в текущем окне
// ---
void IfDrawGIImplement::DrawCurveMap( const MbCurve * curve, const MbSurface * surface, int R, int G, int B )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( curve != nullptr && surface != nullptr && wnd != nullptr ) {
    IGrDraw& tool = wnd->GetDrawTool();

    POINT p0;
    MbVector del;
    double scaleX = 1.0;
    double scaleY = 1.0;
    GetScales( wnd, curve, *surface, del, p0, scaleX, scaleY );

    int C = surface->IsA()==st_Plane ? 50 : 126;
    tool.SelectPen( 1/*width*/, C, C, C );
    DrawSurfacePlane( tool, *surface, del, p0, scaleX, scaleY );
    tool.RestorePen();

    tool.SelectPen( 1/*width*/, R, G, B );
    DrawPlaneCurve( tool, *curve, del, p0, scaleX, scaleY );
    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Дать параметры пересчета в систему координат окна (чтобы покрыть окно заданным
// прямоугольником rect).
// Если isotropic==true, значит |scaleX|==|scaleY|.
// Радиус-вектор r точки в окне в зависимости от координат точки p на кривой
// будет вычисляться по формуле:
//                  [ scaleX ]
//      r(p) = p0 + [        ] * (p + del).
//                  [ scaleY ]
// ---
static void GetScale( const WorkWindow * w,
                      const MbRect &     rect,
                      const bool         isotropic,
                            MbVector &   del,
                            POINT &      p0,
                            double &     scaleX,
                            double &     scaleY )
{
  RECT r;
  w->GetDrawTool().GetClientRect( r );

  POINT pp;
  pp.x = static_cast<int32>( (r.right - r.left) * DELTA_MOD );
  pp.y = static_cast<int32>( (r.top - r.bottom) * DELTA_MOD );
  p0.x = r.left + pp.x;
  p0.y = r.bottom + pp.y;

  del.Init( -rect.left, -rect.bottom );
  scaleX = ( static_cast<double>(r.right) - r.left ) - 2.0 * pp.x;
  scaleY = ( static_cast<double>(r.top) - r.bottom ) - 2.0 * pp.y;

  const double du = rect.GetLengthX();
  const double dv = rect.GetLengthY();
  if ( du > PARAM_EPSILON )
    scaleX /= du;
  if ( dv > PARAM_EPSILON )
    scaleY /= dv;

  if ( isotropic ) {
    const double minScale = std_min( ::fabs(scaleX), ::fabs(scaleY) );
    scaleX = minScale * c3d::Sign( scaleX );
    scaleY = minScale * c3d::Sign( scaleY );
  }
}


//------------------------------------------------------------------------------
// Отрисовать карту кривой на поверхности в заранее заданном масштабе.
// Масштаб выбирается таким образом, чтобы активная область окна покрывалась заданным прямоугольником rect.
// Значения "R", "G" и "B" определяют цвет отрисованной кривой (в RGB-формате).
// Если isotropic==true, значит объект будет рисоваться в равных масштабах по осям. В противном случае окружность может отобразиться как эллипс, квадрат как прямоугольник и т.п.
// Если showSurfaceBoundaries==true, будут отрисовываться прямоугольные границы поверхности.
// ---
void IfDrawGIImplement::DrawCurveMap( const MbCurve *   curve,
                                      const MbSurface * surface,
                                      const MbRect &    rect,
                                      const int         R,
                                      const int         G,
                                      const int         B,
                                      const int         boundaryWidth,
                                      const int         curveWidth,
                                      const bool        isotropic,
                                      const bool        showSurfaceBoundaries )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( curve != nullptr && surface != nullptr && wnd != nullptr ) {
    IGrDraw& tool = wnd->GetDrawTool();

    POINT p0;
    MbVector del;
    double scaleX = 1.0;
    double scaleY = 1.0;
    GetScale( wnd, rect, isotropic, del, p0, scaleX, scaleY );

    if ( showSurfaceBoundaries ) {
      int C = surface->IsA() == st_Plane ? 50 : 126;
      tool.SelectPen( boundaryWidth, C, C, C );
      DrawSurfacePlane( tool, *surface, del, p0, scaleX, scaleY );
      tool.RestorePen();
    }

    tool.SelectPen( curveWidth, R, G, B );
    DrawPlaneCurve( tool, *curve, del, p0, scaleX, scaleY );
    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки карты кривой на поверхности в текущем окне
// ---
void IfDrawGIImplement::DrawSurfaceCurveMap( const MbSurfaceCurve * scurve, int R, int G, int B )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( scurve != nullptr && wnd != nullptr ) {
    IGrDraw& tool = wnd->GetDrawTool();

    POINT p0;
    MbVector del;
    double scaleX = 1.0;
    double scaleY = 1.0;
    GetScales( wnd, &scurve->GetCurve(), scurve->GetSurface(), del, p0, scaleX, scaleY );

    int C = scurve->GetSurface().IsA()==st_Plane ? 50 : 126;
    tool.SelectPen( 1/*width*/, C, C, C );
    DrawSurfacePlane( tool, scurve->GetSurface(), del, p0, scaleX, scaleY );
    tool.RestorePen();

    tool.SelectPen( 1/*width*/, R, G, B );
    DrawPlaneCurve( tool, scurve->GetCurve(), del, p0, scaleX, scaleY );
    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки точки в текущем окне
// ---
void IfDrawGIImplement::DrawPointMap( const MbCartPoint * pnt, const MbSurface * surface, int R, int G, int B ) {
  LockDraw();
  const uint/*OV_x64 size_t*/ halfsize = 2;
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( wnd != nullptr ) {
    IGrDraw& tool = wnd->GetDrawTool();

    MbLineSegment seg ( *pnt, *pnt );
    POINT p0;
    MbVector del;
    double scaleX = 1.0;
    double scaleY = 1.0;
    GetScales( wnd, &seg, *surface, del, p0, scaleX, scaleY );

    int C = surface->IsA()==st_Plane ? 50 : 126;
    tool.SelectPen( 1/*width*/, C, C, C );
    DrawSurfacePlane( tool, *surface, del, p0, scaleX, scaleY );
    tool.RestorePen();

    tool.SelectPen( 1/*width*/, R, G, B );

    MbVector vec;
    POINT pd;

    MbCartPoint pt = *pnt;

    pt.Move( del );
    pd.x = (int32)( p0.x + pt.x * scaleX );
    pd.y = (int32)( p0.y + pt.y * scaleY );
    tool.MoveTo( pd.x - halfsize, pd.y - halfsize );

    pd.x = (int32)( p0.x + pt.x * scaleX );
    pd.y = (int32)( p0.y + pt.y * scaleY );
    tool.LineTo( pd.x, pd.y + halfsize );

    pd.x = (int32)( p0.x + pt.x * scaleX );
    pd.y = (int32)( p0.y + pt.y * scaleY );
    tool.LineTo( pd.x + halfsize, pd.y + halfsize );

    pd.x = (int32)( p0.x + pt.x * scaleX );
    pd.y = (int32)( p0.y + pt.y * scaleY );
    tool.LineTo( pd.x + halfsize, pd.y );

    pd.x = (int32)( p0.x + pt.x * scaleX );
    pd.y = (int32)( p0.y + pt.y * scaleY );
    tool.LineTo( pd.x - halfsize, pd.y - halfsize );

    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки карты кривой пересечения поверхностей в текущем окне
// ---
void IfDrawGIImplement::DrawSurfaceIntersectionMap( const MbSurfaceIntersectionCurve * scurve, int R1, int G1, int B1, int R2, int G2, int B2 )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( scurve != nullptr && wnd != nullptr ) {
    IGrDraw& tool = wnd->GetDrawTool();

    POINT p1;
    MbVector del1;
    double scaleX1 = 1.0, scaleY1 = 1.0;
    POINT p2;
    MbVector del2;
    double scaleX2 = 1.0, scaleY2 = 1.0;

    GetScales( wnd, &scurve->GetCurveOneCurve(), scurve->GetCurveOneSurface(), del1, p1, scaleX1, scaleY1 );

    tool.SelectPen( 1/*width*/, 156, 126, 156 );
    DrawSurfacePlane( tool, scurve->GetCurveOneSurface(), del1, p1, scaleX1, scaleY1 );
    tool.RestorePen();

    GetScales( wnd, &scurve->GetCurveTwoCurve(), scurve->GetCurveTwoSurface(), del2, p2, scaleX2, scaleY2 );

    tool.SelectPen( 1/*width*/, 126, 156, 156 );
    DrawSurfacePlane( tool, scurve->GetCurveTwoSurface(), del2, p2, scaleX2, scaleY2 );
    tool.RestorePen();

    tool.SelectPen( 1/*width*/, R1, G1, B1 );
    DrawPlaneCurve( tool, scurve->GetCurveOneCurve(), del1, p1, scaleX1, scaleY1 );
    tool.RestorePen();

    tool.SelectPen( 1/*width*/, R2, G2, B2 );
    DrawPlaneCurve( tool, scurve->GetCurveTwoCurve(), del2, p2, scaleX2, scaleY2 );
    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки карты контура на поверхности в текущем окне
// ---
void IfDrawGIImplement::DrawContourOnSurfaceMap( const MbContourOnSurface * scurve, int R, int G, int B )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( scurve != nullptr && wnd != nullptr ) {
    IGrDraw& tool = wnd->GetDrawTool();

    POINT p0;
    MbVector del;
    double scaleX = 1.0;
    double scaleY = 1.0;
    GetScales( wnd, &scurve->GetContour(), scurve->GetSurface(), del, p0, scaleX, scaleY );

    int C = scurve->GetSurface().IsA()==st_Plane ? 50 : 126;
    tool.SelectPen( 1/*width*/, C, C, C );
    DrawSurfacePlane( tool, scurve->GetSurface(), del, p0, scaleX, scaleY );
    tool.RestorePen();

    tool.SelectPen( 1/*width*/, R, G, B );
    DrawPlaneCurve( tool, scurve->GetContour(), del, p0, scaleX, scaleY );
    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки карты усеченной кривыми поверхности в текущем окне
// ---
void IfDrawGIImplement::DrawCurveBoundedSurfaceMap( const MbCurveBoundedSurface * surface, int R, int G, int B )
{
  LockDraw();
  DrawCurveBoundedSurfaceMapUnsafe( surface, R, G, B );
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки карты поверхности в текущем окне
// ---
void IfDrawGIImplement::DrawSurfaceMap( const MbSurface * surface, int R, int G, int B )
{
  LockDraw();
  if ( surface == nullptr )
    return;

  if (surface->IsA() == st_CurveBoundedSurface) {
    DrawCurveBoundedSurfaceMapUnsafe( (MbCurveBoundedSurface *)surface, R, G, B );
    return;
  }

  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();
  if ( wnd != nullptr ) {
    IGrDraw& tool = wnd->GetDrawTool();

    POINT p0;
    MbVector del;
    double scaleX = 1.0;
    double scaleY = 1.0;
    GetScales( wnd, nullptr, *surface, del, p0, scaleX, scaleY );

    tool.SelectPen( 1/*width*/, R, G, B );
    DrawSurfacePlane( tool, *surface, del, p0, scaleX, scaleY );
    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки карты грани в текущем окне
// ---
void IfDrawGIImplement::DrawFaceMap( const MbFace * face, int R, int G, int B )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( face != nullptr && wnd != nullptr ) {
    size_t k, loopsCnt = face->GetLoopsCount();
    std::vector< SPtr<MbContourOnSurface> > bounds;
    bounds.reserve( loopsCnt );

    for ( k = 0; k < loopsCnt; k++ ) {
      const MbLoop * loop = face->_GetLoop( k );
      if ( loop != nullptr && loop->GetEdgesCount() > 0 ) {
        SPtr<MbContourOnSurface> bound( &loop->MakeContourOnSurface( face->GetSurface().GetSurface(), face->IsSameSense() ) );
        bounds.push_back( bound );
      }
      else {
        PRECONDITION( false );
        if ( k == 0 ) {
          bounds.clear();
          break;
        }
      }
    }

    loopsCnt = bounds.size();

    if ( loopsCnt > 0 ) {
      MbContourOnSurface * bound0 = bounds[0];

      IGrDraw & tool = wnd->GetDrawTool();

      POINT p0;
      MbVector del;
      double scaleX = 1.0;
      double scaleY = 1.0;
      GetScales( wnd, &bound0->GetContour(), bound0->GetSurface(), del, p0, scaleX, scaleY );

      int C = bound0->GetSurface().IsA() == st_Plane ? 50 : 126;
      tool.SelectPen( 1/*width*/, C, C, C );
      DrawSurfacePlane( tool, bound0->GetSurface(), del, p0, scaleX, scaleY );
      tool.RestorePen();

      tool.SelectPen( 1/*width*/, R, G, B );
      DrawPlaneCurve( tool, bound0->GetContour(), del, p0, scaleX, scaleY );
      for ( k = 1; k < loopsCnt; k++ ) {
        DrawPlaneCurve( tool, bounds[k]->GetContour(), del, p0, scaleX, scaleY );
      }
      tool.RestorePen();
    }
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Отрисовать параметрическую точку поверхности
//---
void IfDrawGIImplement::DrawPoint( const MbSurface & surface, const MbCartPoint & uv, int R, int G, int B, int width )
{
  LockDraw();
  MbCartPoint3D pnt;
  surface._PointOn( uv.x, uv.y, pnt );
  DrawPointUnsafe( &pnt, R, G, B, width );
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Показать множество 3d-точек по множеству 2d-точек и поверхности
// ---
void IfDrawGIImplement::DrawPoints( const MbSurface & surface, const SArray<MbCartPoint> & uvArr, int R, int G, int B, int width )
{
  LockDraw();
  MbCartPoint3D pnt;
  for ( size_t i = 0, icnt = uvArr.Count(); i < icnt; i++ ) {
    surface._PointOn( uvArr[i].x, uvArr[i].y, pnt );
    DrawPointUnsafe( &pnt, R, G, B, width );
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Показать множество 3d-точек по множеству параметров и кривой
// ---
void IfDrawGIImplement::DrawPoints( const MbCurve3D & curve, const SArray<double> & tArr, int R, int G, int B, int width )
{
  LockDraw();
  MbCartPoint3D pnt;
  for ( size_t i = 0, icnt = tArr.Count(); i < icnt; i++ ) {
    curve._PointOn( tArr[i], pnt );
    DrawPointUnsafe( &pnt, R, G, B, width );
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Показать множество 3d-точек
// ---
void IfDrawGIImplement::DrawPoints( const SArray<MbCartPoint3D> & pnts, int R, int G, int B, int width )
{
  LockDraw();
  for ( size_t i = 0, icnt = pnts.Count(); i < icnt; i++ ) {
    DrawPointUnsafe( &pnts[i], R, G, B, width );
  }
  ReleaseDraw();
}


////////////////////////////////////////////////////////////////////////////////
//
//                         Отрисовка триангуляции
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Нарисовать триангуляцию
// ---
void IfDrawGIImplement::PutGrid( const MbGrid & grid, int R, int G, int B, int width )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( wnd != nullptr ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw& tool = wnd->GetDrawTool();
    tool.SelectPen( width, R, G, B );

    MbCartPoint3D p0, p1, p2, p3;
    size_t i, count;

    count = grid.TrianglesCount();
    for ( i = 0; i < count; i++ ) {
      if ( grid.GetTrianglePoints  ( i, p0, p1, p2 ) ) {
        TransformP( into, /*viewPort,*/ p0 );
        TransformP( into, /*viewPort,*/ p1 );
        TransformP( into, /*viewPort,*/ p2 );
        DrawLineEx( tool, p0, p1 );
        DrawLineEx( tool, p1, p2 );
        DrawLineEx( tool, p2, p0 );
      }
    }

    count = grid.QuadranglesCount();
    for ( i = 0; i < count; i++ ) {
      if ( grid.GetQuadranglePoints( i, p0, p1, p2, p3 ) ) {
        TransformP( into, /*viewPort,*/ p0 );
        TransformP( into, /*viewPort,*/ p1 );
        TransformP( into, /*viewPort,*/ p2 );
        TransformP( into, /*viewPort,*/ p3 );
        DrawLineEx( tool, p0, p1 );
        DrawLineEx( tool, p1, p2 );
        DrawLineEx( tool, p2, p3 );
        DrawLineEx( tool, p3, p0 );
      }
    }
    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Нарисовать треугольник
// ---
void IfDrawGIImplement::PutTriangle( const MbGrid & grid, ptrdiff_t index, int R, int G, int B )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( wnd != nullptr ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw& tool = wnd->GetDrawTool();
    tool.SelectPen( 1/*width*/, R, G, B );

    MbCartPoint3D p0, p1, p2, q;

    if ( grid.GetTrianglePoints( index, p0, p1, p2 ) ) {
      TransformP( into, /*viewPort,*/ p0 );
      TransformP( into, /*viewPort,*/ p1 );
      TransformP( into, /*viewPort,*/ p2 );
      DrawLineEx( tool, p0, p1 );
      DrawLineEx( tool, p1, p2 );
      DrawLineEx( tool, p2, p0 );

      q = (p0+p1+p2)/3;
      p0 = (p0+q)*0.5;
      p1 = (p1+q)*0.5;
      p2 = (p2+q)*0.5;
      DrawLineEx( tool, p0, q );
      DrawLineEx( tool, p1, q );
      DrawLineEx( tool, p2, q );
    }

    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Нарисовать треугольник
// ---
void IfDrawGIImplement::PutTriangle( const MbSurface & surface,
                                     const MbGrid & grid, ptrdiff_t index, int R, int G, int B )
{
  MbCartPoint3D p0, p1, p2;
  if ( grid.GetTrianglePoints( index, p0, p1, p2 ) ) {
    LockDraw();
    const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

    if ( wnd != nullptr ) {
      MbCartPoint3D q;
      const MbMatrix3D & into = wnd->GetOwnMatrixInto();
      IGrDraw & tool = wnd->GetDrawTool();
      tool.SelectPen( 1/*width*/, R, G, B );

      TransformP( into, /*viewPort,*/ p0 );
      TransformP( into, /*viewPort,*/ p1 );
      TransformP( into, /*viewPort,*/ p2 );
      DrawLineEx( tool, p0, p1 );
      DrawLineEx( tool, p1, p2 );
      DrawLineEx( tool, p2, p0 );

      q = ( p0 + p1 + p2 ) / 3;
      p0 = ( p0 + q ) * 0.5;
      p1 = ( p1 + q ) * 0.5;
      p2 = ( p2 + q ) * 0.5;
      DrawLineEx( tool, p0, q );
      DrawLineEx( tool, p1, q );
      DrawLineEx( tool, p2, q );

      tool.RestorePen();
    }
    ReleaseDraw();
  }
  else {
    const MbTriangle & triangle = grid.GetTriangle( index );
    uint i0 = 0, i1 = 0, i2 = 0;
    if ( triangle.GetTriangle(i0,i1,i2) ) {
      MbCartPoint r0, r1, r2;
      grid.GetParam( i0, r0 );
      grid.GetParam( i1, r1 );
      grid.GetParam( i2, r2 );
      PutTriangle( surface, r0, r1, r2, R, G, B );
    }
  }
}


//------------------------------------------------------------------------------
// Нарисовать треугольник
// ---
void IfDrawGIImplement::PutTriangle( const MbCollection & coll, ptrdiff_t index, int R, int G, int B )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( wnd != nullptr ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw & tool = wnd->GetDrawTool();
    tool.SelectPen( 1/*width*/, R, G, B );

    MbCartPoint3D p0, p1, p2, q;

    if ( coll.GetTrianglePoints( index, p0, p1, p2 ) ) {
      TransformP( into, /*viewPort,*/ p0 );
      TransformP( into, /*viewPort,*/ p1 );
      TransformP( into, /*viewPort,*/ p2 );
      DrawLineEx( tool, p0, p1 );
      DrawLineEx( tool, p1, p2 );
      DrawLineEx( tool, p2, p0 );

      q = ( p0 + p1 + p2 ) / 3;
      p0 = ( p0 + q ) * 0.5;
      p1 = ( p1 + q ) * 0.5;
      p2 = ( p2 + q ) * 0.5;
      DrawLineEx( tool, p0, q );
      DrawLineEx( tool, p1, q );
      DrawLineEx( tool, p2, q );
    }

    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Нарисовать треугольник
// ---
void IfDrawGIImplement::PutTriangle( const MbSurface & surface,
                                     const MbCartPoint & uv0, const MbCartPoint & uv1, const MbCartPoint & uv2,
                                     int R, int G, int B )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( wnd != nullptr ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw& tool = wnd->GetDrawTool();
    tool.SelectPen( 1/*width*/, R, G, B );

    MbCartPoint3D p0, p1, p2, q;

    surface._PointOn( uv0, p0 );
    surface._PointOn( uv1, p1 );
    surface._PointOn( uv2, p2 );

    TransformP( into, /*viewPort,*/ p0 );
    TransformP( into, /*viewPort,*/ p1 );
    TransformP( into, /*viewPort,*/ p2 );
    DrawLineEx( tool, p0, p1 );
    DrawLineEx( tool, p1, p2 );
    DrawLineEx( tool, p2, p0 );

    q = (p0+p1+p2)/3;
    p0 = (p0+q)*0.5;
    p1 = (p1+q)*0.5;
    p2 = (p2+q)*0.5;
    DrawLineEx( tool, p0, q );
    DrawLineEx( tool, p1, q );
    DrawLineEx( tool, p2, q );

    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Нарисовать четырёхугольник
// ---
void IfDrawGIImplement::PutQuadrangle( const MbGrid & grid, ptrdiff_t index, int R, int G, int B )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( wnd != nullptr ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw& tool = wnd->GetDrawTool();
    tool.SelectPen( 1/*width*/, R, G, B );

    MbCartPoint3D p0, p1, p2, p3, q;

    if ( grid.GetQuadranglePoints( index, p0, p1, p2, p3 ) ) {
      TransformP( into, /*viewPort,*/ p0 );
      TransformP( into, /*viewPort,*/ p1 );
      TransformP( into, /*viewPort,*/ p2 );
      TransformP( into, /*viewPort,*/ p3 );
      DrawLineEx( tool, p0, p1 );
      DrawLineEx( tool, p1, p2 );
      DrawLineEx( tool, p2, p3 );
      DrawLineEx( tool, p3, p0 );

      q = (p0+p1+p2+p3)/4;   //-V112
      p0 = (p0+q)*0.5;
      p1 = (p1+q)*0.5;
      p2 = (p2+q)*0.5;
      p3 = (p3+q)*0.5;
      DrawLineEx( tool, p0, q );
      DrawLineEx( tool, p1, q );
      DrawLineEx( tool, p2, q );
      DrawLineEx( tool, p3, q );
    }

    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Нарисовать четырёхугольник
// ---
void IfDrawGIImplement::PutQuadrangle( const MbSurface & surface,
                                       const MbGrid & grid, ptrdiff_t index, int R, int G, int B )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( wnd != nullptr ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw & tool = wnd->GetDrawTool();
    tool.SelectPen( 1/*width*/, R, G, B );

    MbCartPoint3D p0, p1, p2, p3, q;
    bool toDraw = grid.GetQuadranglePoints( index, p0, p1, p2, p3 );
    if ( !toDraw ) {
      uint i0 = 0, i1 = 0, i2 = 0, i3 = 0;
      const MbQuadrangle & quadrangle = grid.GetQuadrangle( index );
      if ( quadrangle.GetQuadrangle( i0, i1, i2, i3 ) ) {
        MbCartPoint r0, r1, r2, r3;
        grid.GetParam( i0, r0 );
        grid.GetParam( i1, r1 );
        grid.GetParam( i2, r2 );
        grid.GetParam( i3, r3 );
        surface._PointOn( r0, p0 );
        surface._PointOn( r1, p1 );
        surface._PointOn( r2, p2 );
        surface._PointOn( r3, p3 );
        toDraw = true;
      }
    }
    if ( toDraw ) {
      TransformP( into, /*viewPort,*/ p0 );
      TransformP( into, /*viewPort,*/ p1 );
      TransformP( into, /*viewPort,*/ p2 );
      TransformP( into, /*viewPort,*/ p3 );
      DrawLineEx( tool, p0, p1 );
      DrawLineEx( tool, p1, p2 );
      DrawLineEx( tool, p2, p3 );
      DrawLineEx( tool, p3, p0 );

      q = ( p0 + p1 + p2 + p3 ) / 4;   //-V112
      p0 = ( p0 + q ) * 0.5;
      p1 = ( p1 + q ) * 0.5;
      p2 = ( p2 + q ) * 0.5;
      p3 = ( p3 + q ) * 0.5;
      DrawLineEx( tool, p0, q );
      DrawLineEx( tool, p1, q );
      DrawLineEx( tool, p2, q );
      DrawLineEx( tool, p3, q );
    }
    tool.RestorePen();
  }

  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Нарисовать двумерную триангуляцию.
// ---
void IfDrawGIImplement::PutPlanarGrid( MbPlanarGrid & grid, const MbPlacement3D & place, int R, int G, int B, int width )
{
  LockDraw();
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( wnd != nullptr ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw& tool = wnd->GetDrawTool();
    tool.SelectPen( width, R, G, B );

    MbCartPoint   p0,   p1,   p2;
    MbCartPoint3D p03d, p13d, p23d, q;

    for ( size_t i = 0, count = grid.GetTrianglesCount(); i < count; i++ ) {
      if ( grid.GetTrianglePoints(i, p0, p1, p2) ) {
        place.PointOn( p0, p03d );
        place.PointOn( p1, p13d );
        place.PointOn( p2, p23d );
        TransformP( into, /*viewPort,*/ p03d );
        TransformP( into, /*viewPort,*/ p13d );
        TransformP( into, /*viewPort,*/ p23d );
        DrawLineEx( tool, p03d, p13d );
        DrawLineEx( tool, p13d, p23d );
        DrawLineEx( tool, p23d, p03d );

        q = (p03d+p13d+p23d)/3;
        p03d = (p03d+q)*0.5;
        p13d = (p13d+q)*0.5;
        p23d = (p23d+q)*0.5;
        DrawLineEx( tool, p03d, q );
        DrawLineEx( tool, p13d, q );
        DrawLineEx( tool, p23d, q );

      }
    }

    tool.RestorePen();
  }
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Функция отрисовки карты кривой на поверхности в текущем окне
// ---
void IfDrawGIImplement::DrawGridMap( const MbGrid & grid, const MbSurface & surface, int R, int G, int B )
{
  LockDraw();
  size_t paramsCount = grid.ParamsCount();

  if ( paramsCount < 3 )
    return;

  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( wnd != nullptr ) {
    IGrDraw& tool = wnd->GetDrawTool();

    POINT centre0;
    MbVector del;
    double scaleX, scaleY;
    GetScales( wnd, nullptr, surface, del, centre0, scaleX, scaleY );

    int C = 100;
    tool.SelectPen( 1/*width*/, C, C, C );
    DrawSurfacePlane( tool, surface, del, centre0, scaleX, scaleY );
    tool.RestorePen();

    tool.SelectPen( 1/*width*/, R, G, B );

    MbCartPoint p0, p1, p2, p3;
    MbLineSegment curve( p0, p1 );
    size_t i, count;

    count = grid.TrianglesCount();
    for ( i = 0; i < count; i++ ) {
      if ( grid.GetTriangleParams( i, p0, p1, p2 ) ) {
        curve.Init( p0, p1 );
        DrawPlaneCurve( tool, curve, del, centre0, scaleX, scaleY );
        curve.Init( p1, p2 );
        DrawPlaneCurve( tool, curve, del, centre0, scaleX, scaleY );
        curve.Init( p2, p0 );
        DrawPlaneCurve( tool, curve, del, centre0, scaleX, scaleY );
      }
    }

    count = grid.QuadranglesCount();
    for ( i = 0; i < count; i++ ) {
      if ( grid.GetQuadrangleParams( i, p0, p1, p2, p3 ) ) {
        curve.Init( p0, p1 );
        DrawPlaneCurve( tool, curve, del, centre0, scaleX, scaleY );
        curve.Init( p1, p2 );
        DrawPlaneCurve( tool, curve, del, centre0, scaleX, scaleY );
        curve.Init( p2, p3 );
        DrawPlaneCurve( tool, curve, del, centre0, scaleX, scaleY );
        curve.Init( p3, p0 );
        DrawPlaneCurve( tool, curve, del, centre0, scaleX, scaleY );
      }
    }
    tool.RestorePen();
  }
  ReleaseDraw();
}

//------------------------------------------------------------------------------
// Стереть модель.
// ---
void IfDrawGIImplement::EraseModel()
{
  LockDraw();
  TestVariables::viewManager->EraseModel();
  ReleaseDraw();
}

//------------------------------------------------------------------------------
// Перерисовать модель.
// ---
void IfDrawGIImplement::RedrawModel()
{
  LockDraw();
  TestVariables::viewManager->EraseModel();
  TestVariables::viewManager->ShowModel();
  ReleaseDraw();
}


//------------------------------------------------------------------------------
// Получить плейсмент активного окна (например для прорисовки плоской кривой).
// ---
void IfDrawGIImplement::GetWindowPlace( MbPlacement3D & place )
{
  place = TestVariables::viewManager->GetActiveWindow()->GetOwnPlace();
}


//------------------------------------------------------------------------------
// Отрисовка трёхмерных геометрических объектов заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawItemUnsafe( const MbSpaceItem * gi, int R, int G, int B, const MbMatrix3D & from, int width )
{
  if ( WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow() )
  {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw & tool = wnd->GetDrawTool();

    MbMatrix3D into_( from*into );

    bool calcGrid = false;
    // KYA: Пока криво, отключаю до реализации поиска по триангуляции
    //if ( wnd->GetRenderType() == wwrt_OGL ) {
    //  if ( gi != nullptr && gi->Family() == st_Surface )
    //    calcGrid = true;
    //}

    if ( !calcGrid ) {
      tool.SelectPen( width, R, G, B );
#ifdef USE_WRK_MESH
      DrawSpaceItem( tool, gi, into_, wrkMesh );
#else
      MbMesh wrkMesh;
      DrawSpaceItem( tool, gi, into_, wrkMesh );
#endif
      tool.RestorePen();
    }
    else {
      const MbSurface & surface = static_cast<const MbSurface &>( *gi );

      MbMesh wrkMesh;
      MbStepData stepData( ist_SpaceStep, Math::visualSag );
      MbFormNote formNote( false, true, true, TestVariables::exactMesh );
      surface.CalculateMesh( stepData, formNote, wrkMesh );

      SPtr<MbVisual> aColor( MbVisual::Create( RGB( R, G, B ) ) );
      wrkMesh.AddAttribute( aColor );
      SPtr<MbWidth> aWidth( new MbWidth( width ) );
      wrkMesh.AddAttribute( aWidth );

      wnd->PutObjectPart( &wrkMesh, gi, false );
    }
  }
}


//------------------------------------------------------------------------------
// Отрисовка двумерных геометрических объектов заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawItemUnsafe( const MbPlaneItem * gi, int R, int G, int B, const MbMatrix3D & from, int width )
{
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( wnd != nullptr ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw& tool = wnd->GetDrawTool();
    tool.SelectPen( width, R, G, B );

    MbMatrix3D into_( from*into );

    DrawPlaneItem( tool, gi, into_/*, viewPort*/ );

    tool.RestorePen();
  }
}

//------------------------------------------------------------------------------
// Отрисовка топологических объектов заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawItemUnsafe( const MbTopItem * ti, int R, int G, int B, const MbMatrix3D & from, int width )
{
  if ( ti == nullptr )
    return;

  switch ( ti->IsA() ) {
    case tt_Vertex : {       // Вершина
      MbCartPoint3D gi( ((MbVertex *)ti)->GetCartPoint() );
      DrawPointUnsafe( &gi, R, G, B, 2*width );
    } break;
    case tt_Edge :
    case tt_CurveEdge : { // Ребро проходящее по кривой
      const MbSpaceItem * gi = static_cast<const MbSpaceItem *>(&(static_cast<const MbEdge *>(ti))->GetCurve());
      DrawItemUnsafe( gi, R, G, B, from, width );
    } break;
    case tt_OrientedEdge : { // Ориентированное ребро
      const MbSpaceItem * gi = static_cast< const MbSpaceItem *>(&(static_cast<const MbOrientedEdge *>(ti))->GetCurve());
      DrawItemUnsafe( gi, R, G, B, from, width );
    } break;
    case tt_Loop : { // Цикл грани
      MbLoop * loop = (MbLoop *)ti;
      for ( size_t k = 0, kcnt = loop->GetEdgesCount(); k < kcnt; k++ ) {
        DrawItemUnsafe( loop->GetOrientedEdge( k ), R, G, B, from, width );
      }
    } break;
    case tt_Face : {         // Грань
      const MbSpaceItem * gi = static_cast<const MbSpaceItem *>(&(static_cast<const MbFace *>(ti))->GetSurface());
      DrawItemUnsafe( gi, R, G, B, from, width );
    } break;
    case tt_FaceShell : {  // Связное множество граней
      const MbFaceShell * fs = static_cast< const MbFaceShell *>(ti);
      for ( size_t i = 0, count = fs->GetFacesCount(); i < count; i++ ) 
      {
        const MbSpaceItem * gi = static_cast<const MbSpaceItem *>(fs->GetSurface( i ));
        DrawItemUnsafe( gi, R, G, B, from, width );
      }
    } break;

    default: break;
  }
}

//------------------------------------------------------------------------------
// Нарисовать точку.
// ---
void IfDrawGIImplement::DrawPointUnsafe( const MbCartPoint3D * gi, int R, int G, int B, int width )
{
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( gi != nullptr && wnd != nullptr ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw& tool = wnd->GetDrawTool();
    tool.SelectPen( width, R, G, B );

    MbCartPoint3D pnt(gi->x,gi->y,gi->z);
    POINT pd;

    TransformP( into, /*viewPort,*/ pnt ); // Перевод в экранную СК
    pd.x = (int32)pnt.x;
    pd.y = (int32)pnt.y;
    pd.x -= (int32)( TestVariables::drawFactor/2.0 );
    pd.y -= (int32)( TestVariables::drawFactor/2.0 );
    tool.MoveTo( pd.x, pd.y );

    pd.y += TestVariables::drawFactor;
    tool.LineTo( pd.x, pd.y );

    pd.x += TestVariables::drawFactor;
    tool.LineTo( pd.x, pd.y );

    pd.y -= TestVariables::drawFactor;
    tool.LineTo( pd.x, pd.y );

    pd.x -= TestVariables::drawFactor;
    tool.LineTo( pd.x, pd.y );

    tool.RestorePen();
    tool.UpdateWindow();
  }
}


//------------------------------------------------------------------------------
// Функция отрисовки отрезка заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawLineUnsafe( const MbCartPoint3D & q1
                                      , const MbCartPoint3D & q2, int R, int G, int B, int width )
{  
  if ( const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow() )
  {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw& tool = wnd->GetDrawTool();
    tool.SelectPen( width, R, G, B );

    MbCartPoint3D p1(q1), p2(q2);
    POINT pb, pd;

    TransformP( into, /*viewPort,*/ p1 );
    TransformP( into, /*viewPort,*/ p2 );
    pb.x = (int32)p1.x;
    pb.y = (int32)p1.y;
    pd.x = (int32)p2.x;
    pd.y = (int32)p2.y;

    tool.MoveTo( pb.x, pb.y );
    tool.LineTo( pd.x, pd.y );

    tool.RestorePen();
  }
}


//------------------------------------------------------------------------------
// Функция отрисовки двумерных кривых на поверхности
// ---
void IfDrawGIImplement::DrawItemUnsafe( const MbCurve * curve, const MbSurface * surface, int R, int G, int B, MbPolygon3D * extraPoly, int width )
{
  if ( curve != nullptr && surface != nullptr ) {
    if ( curve->Type() == pt_Contour ) { // Контур
      const MbContour * contour = (const MbContour *)curve;
      for ( size_t i = 0; i < contour->GetSegmentsCount(); i++ )
        DrawItemUnsafe( contour->GetSegment( i ), surface, R, G, B, extraPoly, width );
    }
    else {
      const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

      if ( wnd != nullptr ) {
        const MbMatrix3D & into = wnd->GetOwnMatrixInto();
        IGrDraw& tool = wnd->GetDrawTool();
        tool.SelectPen( width, R, G, B );

        MbMesh mesh;
        PRECONDITION( extraPoly != nullptr );
        if ( extraPoly != nullptr )
          mesh.AddPolygon( *extraPoly );
        mesh.SetRefItem( curve );
        mesh.SetMeshType( st_Curve3D );

        DrawMeshEx( tool, &mesh, into/*, viewPort*/ );

        //DeleteObject( SelectObject( dc, hpen ) );
        tool.RestorePen();

        if ( extraPoly != nullptr ) {
          size_t pntsCnt = extraPoly->Count();
          if ( pntsCnt > 0 && pntsCnt < 3 ) {
            MbCartPoint3D pnt1, pnt2;
            extraPoly->GetPoint( 0, pnt1 );
            extraPoly->GetPoint( (pntsCnt-1), pnt2 );
            if ( c3d::EqualPoints( pnt1, pnt2, EXTENT_EQUAL ) )
              DrawPointUnsafe( &pnt1, R, G, B, 2*width );
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Функция отрисовки карты усеченной кривыми поверхности в текущем окне
// ---
void IfDrawGIImplement::DrawCurveBoundedSurfaceMapUnsafe( const MbCurveBoundedSurface * surface, int R, int G, int B, int width )
{
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( surface != nullptr && wnd != nullptr ) {
    IGrDraw& tool = wnd->GetDrawTool();
    POINT p0;
    MbVector del;
    double scaleX = 1.0;
    double scaleY = 1.0;

    if ( surface->GetCurvesCount()>0 ) {
      GetScales( wnd, &surface->GetCurve(0)->GetContour(), surface->GetSurface(), del, p0, scaleX, scaleY );

      int C = surface->GetSurface().IsA()==st_Plane ? 50 : 126;
      tool.SelectPen( width, C, C, C );
      DrawSurfacePlane( tool, surface->GetSurface(), del, p0, scaleX, scaleY );
      tool.RestorePen();
    }
    else {
      GetScales( wnd, nullptr, *surface, del, p0, scaleX, scaleY );

      tool.SelectPen( width, TRGB_WHITE );
      DrawSurfacePlane( tool, *surface, del, p0, scaleX, scaleY );
      tool.RestorePen();
    }

    tool.SelectPen( width, R, G, B );

    for ( size_t i = 0; i < surface->GetCurvesCount(); i++ )
      DrawPlaneCurve( tool, surface->GetCurve(i)->GetContour(), del, p0, scaleX, scaleY );

    tool.RestorePen();
  }
}


//------------------------------------------------------------------------------
// Отрисовка сетки геометрических объектов заданным цветом в текущем окне
// ---
void IfDrawGIImplement::DrawMeshUnsafe( const MbMesh * ms, int R, int G, int B, int width )
{
  const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();

  if ( (ms != nullptr) && (wnd != nullptr) ) {
    const MbMatrix3D & into = wnd->GetOwnMatrixInto();
    IGrDraw & tool = wnd->GetDrawTool();
    tool.SelectPen( width, R, G, B );

    DrawMeshEx( wnd->GetDrawTool(), ms, into/*, viewPort*/ );

    tool.RestorePen();
  }
}


//------------------------------------------------------------------------------
// Захватить отрисовочный мьютекс в многопоточном режиме
// ---
void IfDrawGIImplement::LockDraw() {
  if (Math::Multithreaded())
    wndSec.Lock();
}


//------------------------------------------------------------------------------
// Освободить отрисовочный мьютекс в многопоточном режиме
// ---
void IfDrawGIImplement::ReleaseDraw() {
  if (Math::Multithreaded())
    wndSec.Unlock();
}

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Активное окно
// ---
bool InstAddress() {
  TestVariables::drawImp = new IfDrawGIImplement;
#ifdef _DRAWGI
  ::SetDrawGI( TestVariables::drawImp );
#endif // _DRAWGI
  return true;
}

bool b = InstAddress();

