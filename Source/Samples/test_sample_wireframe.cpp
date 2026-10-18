//////////////////////////////////////////////////////////////////////////////////////////
//
//    The file contains source codes demonstrating samples 
//    how to cut an edge of a wireframe.
//
//    WARNING: This code is only an example. The real application can require more 
//             functionality and data types for implementation.
// 
//////////////////////////////////////////////////////////////////////////////////////////

#include <test_samples.h>
#include <test_variables.h>
#include <test_manager.h>
#include <test_service.h>

#include <function_factory.h>
#include <wire_frame.h>
#include <cur_line_segment3d.h>
#include <cur_arc3d.h>
#include <cur_nurbs3d.h>
#include <cur_plane_curve.h>
#include <cur_surface_curve.h>
#include <cur_contour_on_surface.h>
#include <cur_surface_intersection.h>
#include <cur_character_curve3d.h>
#include <cur_character_curve3d.h>
#include <surf_grid_surface.h>
#include <surf_curve_bounded_surface.h>
#include <solid.h>

#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// \ru Разрезать ребро каркаса. \en Cut an edge of a wireframe.
//
// 1. Рассматривается каркас в виде квадрата.
// 2. Одно из ребер каркаса ( с индексом 1 ) разрезается на 6 частей (5 разрезов).
// 3. Остаток исходного ребра окрашивается в черный цвет, новые ребра - в красный и синий.
// ---
void SampleCuttingEdgeWireframe() 
{
  MbCartPoint3D p;
  std::vector<MbCartPoint3D> points;
  std::vector<MbVertex *> verts;
  MbWireFrame * wireframe = new MbWireFrame();

  MbVector3D offset( 10.0, 10.0, 0.0 );

  double squareSize = 10.0;
  p.Init( 0.0, 0.0, 0.0 );
  p += offset;
  points.push_back( p );
  p.Init( squareSize, 0.0, 0.0 );
  p += offset;
  points.push_back( p );
  p.Init( squareSize, squareSize, 0.0 );
  p += offset;
  points.push_back( p );
  p.Init( 0.0, squareSize, 0.0 );
  p += offset;
  points.push_back( p );
  
  size_t pointsCount = points.size();

  MbVertex * v = nullptr;
  for ( size_t i = 0; i < pointsCount; i++ ) {
    v = new MbVertex ( points[i] );
    verts.push_back( v );
  }

  size_t vertsCount = verts.size(); // = pointsCount
  MbLineSegment3D * ls = nullptr;
  for ( size_t i = 0; i < pointsCount; i++ ) {
    ls = new MbLineSegment3D( points[i], points[(i + 1) % pointsCount] );
    MbEdge edge( *ls, true, verts[i], verts[(i + 1) % vertsCount] );
    wireframe->AddEdge( edge, false );
  }

  MbItem * item = nullptr;
  item = TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, wireframe );

  SArray<double> params;
  RPArray<MbEdge> edges;
  MbEdge * cedge = wireframe->SetEdge( 1 );     // берем ребро с индексом 1
  const MbCurve3D & curve = cedge->GetCurve();

  double tMin = curve.GetTMin();
  double tMax = curve.GetTMax();
  double len = tMax - tMin;

#define CUTS_COUNT 5                            // 5 разрезов - 6 частей
  for ( size_t i = 0; i < CUTS_COUNT; i++ ) {
    double param = tMin + ( i + 1 ) * len / ( CUTS_COUNT + 1 );
    params.Add( param );
  }

  wireframe->CuttingEdge( cedge, params, true, PARAM_ACCURACY, edges );

#define EDGE_WIDTH 3
  if ( cedge != nullptr ) {
    cedge->SetColor( RGB( 0, 0, 0 ) ); // остаток исходного ребра окрашиваем в черный цвет
    cedge->SetWidth( EDGE_WIDTH );
  }
  for ( size_t i = 0, edgesCount = edges.Count(); i < edgesCount; i++ ) {
    if ( i % 2 == 0 )
      edges[i]->SetColor( RGB( 255, 0, 0 ) );  // красный
    else
      edges[i]->SetColor( RGB( 0, 0, 255 ) );  // синий

    edges[i]->SetWidth( EDGE_WIDTH );
  }

  TestVariables::viewManager->RedrawObject( item );
}


//------------------------------------------------------------------------------
// \ru Создать грань на поверхности по триангуляции по точкам пирамиды с четырёхугольным основанием.
// \en Create a face on a surface by triangulating the points of a pyramid with a quadrangular base.
// ---
void SampleGridSurface() 
{ 
  MbSolid * result( nullptr );
  MbVector3D axisX( 1.0, 0.0, 0.0 ), axisY( 0.0, 1.0, 0.0 ), axisZ( 0.0, 0.0, 1.0 );
  MbVector aX( 1.0, 0.0 ), aY( 0.0, 1.0 );
  double a = 1.0, d = 50.0;
  MbCartPoint3D centre( 0.0, 0.0, 0.0 );
  std::vector<MbCartPoint>   params( 5 );    // \ru Множество точек на параметрической области поверхности. \en Set of points in parametric space of surface. 
  std::vector<MbCartPoint3D> points( 5 );    // \ru Множество точек поверхности. \en Set of points of surface. 
  std::vector<MbVector3D>    normals( 5 );   // \ru Множество нормалей поверхности. \en Set of normals of surface. 
  std::vector<MbTriangle>    triangles( 4 ); // \ru Множество треугольников. \en Set of triangles. 
  bool sense = true; 

  params[0].Init( 0.0, 0.0 );
  params[1].Set( params[0], 1.0, aX, a );
  params[2].Set( params[0], 1.0, aY, a );
  params[3].Set( params[0], 1.0, aX,-a );
  params[4].Set( params[0], 1.0, aY,-a );
  // Построение точек и нормалей грани на vertex1, vertex2, vertex3.
  points[0].Set( centre, 1.0, axisZ, d );
  points[1].Set( centre, 1.0, axisX, d );
  points[2].Set( centre, 1.0, axisY, d );
  points[3].Set( centre, 1.0, axisX,-d );
  points[4].Set( centre, 1.0, axisY,-d );

  normals[0] = axisZ;
  normals[1] = axisX;
  normals[2] = axisY;
  normals[3] =-axisX;
  normals[4] =-axisY;

  // Построение грани.
  triangles[0].Init( 0, 1, 2, sense );
  triangles[1].Init( 0, 2, 3, sense );
  triangles[2].Init( 0, 3, 4, sense );
  triangles[3].Init( 0, 4, 1, sense );

  std::vector<MbPolyline *> bounds;
  MbGridSurface * surface = MbGridSurface::Create( params, points, normals, triangles, bounds );

  if ( surface != nullptr ) {
    RPArray<MbCurve> boundary( 4, 1 );
    boundary.Add( new MbLineSegment( params[1], params[2] ) );
    boundary.Add( new MbLineSegment( params[2], params[3] ) );
    boundary.Add( new MbLineSegment( params[3], params[4] ) );
    boundary.Add( new MbLineSegment( params[4], params[1] ) );

    MbContour * contour = new MbContour( boundary, true );
    MbContourOnSurface * init = new MbContourOnSurface( *surface, *contour, true );
    MbSurface * rSurface = new MbCurveBoundedSurface( *init, nullptr ); // Конструктор по контурам, берет за базовую поверхность поверхность первого контура.

    MbLoop * loop = new MbLoop();
    const MbVertex * begV( nullptr ), * endV( nullptr );
    MbCartPoint3D pnt( points[1] );
    MbSurfaceIntersectionCurve * intCurve( nullptr );

    begV = new MbVertex( pnt );
    for ( size_t i = 0, iCount = boundary.Count(); i < iCount;i++ ) {
      intCurve = new MbSurfaceIntersectionCurve( *surface, *(boundary[i]), *surface, *(boundary[i]), cbt_Boundary, true, true, nullptr );
      intCurve->GetLimitPoint( 2, pnt );
      endV = new MbVertex( pnt );
      MbCurveEdge * edge = new MbCurveEdge( begV, endV, *intCurve, true );
      MbOrientedEdge * orEdge = new MbOrientedEdge( *edge, true );
      loop->AddEdge( *orEdge ); 
      begV = endV;
    }

    std::vector<MbLoop *> loops;
    loops.push_back( loop );
    MbFace * face = new MbFace( loops, *rSurface, sense ); // Тест на использование шаблонного конструктора.
    //MbFace * face = new MbFace( *loop, *rSurface, sense );
    MbSNameMaker names;
    names.MakeFaceName( *face, 1, 0 );

    MbFaceShell * shell = new MbFaceShell( *face );
    shell->SetClosed( false );

    result = new MbSolid( shell, nullptr );

    TestVariables::viewManager->AddObject( TestVariables::SHELL_Style, result );
  }
}


//------------------------------------------------------------------------------
// \ru Создать кривые по аналитическим функциям координат. \en Create the curve by analytical functions of coordinates.
// ---
void SampleCharacterCurve() 
{
  string_t arg = c3d::ToC3Dstring("t");
  string_t fx = c3d::ToC3Dstring("100*t"); 
  string_t fy1 = c3d::ToC3Dstring("100*(1-(3*t*t)+(2*t*t*t))");
  string_t fy2 = c3d::ToC3Dstring("100*((3*t*t)-(2*t*t*t))");
  string_t fy3 = c3d::ToC3Dstring("100*(t-(2*t*t)+(t*t*t))");
  string_t fy4 = c3d::ToC3Dstring("100*((t*t*t)-(t*t))");
  string_t fz = c3d::ToC3Dstring("0");
  double t1 = 0, t2 = 1;
  MbFunctionFactory factory;
  // Конвертация. Conversion.
  MbFunction * x  = factory.CreateAnalyticalFunction( fx, arg, t1, t2, true );
  MbFunction * z  = factory.CreateAnalyticalFunction( fz, arg, t1, t2, true );
  MbFunction * y1 = factory.CreateAnalyticalFunction( fy1, arg, t1, t2, true );
  MbFunction * y2 = factory.CreateAnalyticalFunction( fy2, arg, t1, t2, true );
  MbFunction * y3 = factory.CreateAnalyticalFunction( fy3, arg, t1, t2, true );
  MbFunction * y4 = factory.CreateAnalyticalFunction( fy4, arg, t1, t2, true );
  if ( x != nullptr && y1 != nullptr && y2 != nullptr && y3 != nullptr && y4 != nullptr && z != nullptr ) {
    MbPlacement3D place;
    MbeLocalSystemType3D cs = ls_CartesianSystem;

    MbCurve3D * result1 = new MbCharacterCurve3D( *x, *y1, *z, cs, place, 0.0, 1.0 );
    MbCurve3D * result2 = new MbCharacterCurve3D( *x, *y2, *z, cs, place, 0.0, 1.0 );
    MbCurve3D * result3 = new MbCharacterCurve3D( *x, *y3, *z, cs, place, 0.0, 1.0 );
    MbCurve3D * result4 = new MbCharacterCurve3D( *x, *y4, *z, cs, place, 0.0, 1.0 );

    Style st( 3, 0 );

    TestVariables::viewManager->AddObject( st, result1 );
    TestVariables::viewManager->AddObject( st, result2 );
    TestVariables::viewManager->AddObject( st, result3 );
    TestVariables::viewManager->AddObject( st, result4 );
  }
}
