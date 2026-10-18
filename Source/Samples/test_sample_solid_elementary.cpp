//////////////////////////////////////////////////////////////////////////////////////////
//
//    The file contains source codes demonstrating samples how to create elementary solids.
//
//    WARNING: This code is only an example. The real application can require more 
//             functionality and data types for implementation.
// 
//////////////////////////////////////////////////////////////////////////////////////////

#include <test_samples.h>
#include <test_variables.h>
#include <test_manager.h>
#include <test_service.h>
//
#include <templ_sptr.h>
#include <surface.h>
#include <cur_surface_intersection.h>
#include <topology_faceset.h>
#include <solid.h>
#include <action_surface.h>
//
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// \ru Добавление точек квадрата. \en Points Adding.
// ---
static
void AddTorusPoints( double r, double z, SArray<MbCartPoint3D> & points,
                     double w, SArray<double> & weights ) 
{
  MbCartPoint3D p( r, 0.0, z );
  MbVector3D toX( 1.0, 0.0, 0.0 ), toY( 0.0, 1.0, 0.0 );
  double wI( w ), wA = w * ::cos( M_PI_4 );
  //     y
  //     ^
  //     | 
  // 3   2   1
  //     | 
  // 4---+---0->x
  //     | 
  // 5   6   7 
  //     | 
  points.Add( p ); // 0
  weights.Add( wI );
  p.Add( toY, r );
  points.Add( p ); // 1
  weights.Add( wA );
  p.Add( toX,-r );
  points.Add( p ); // 2
  weights.Add( wI );
  p.Add( toX,-r );
  points.Add( p ); // 3
  weights.Add( wA );
  p.Add( toY,-r );
  points.Add( p ); // 4
  weights.Add( wI );
  p.Add( toY,-r );
  points.Add( p ); // 5
  weights.Add( wA );
  p.Add( toX, r );
  points.Add( p ); // 6
  weights.Add( wI );
  p.Add( toX, r );
  points.Add( p ); // 7
  weights.Add( wA );
}

//------------------------------------------------------------------------------
// \ru Построение точек тора. \en Torus Points Adding.
// ---
static
void GetTorusPoints( double majorR, double minorR, 
                     SArray<MbCartPoint3D> & points,
                     SArray<double> & weights ) 
{
  double zLavel( 0.0 );
  double w0( 1.0 ), wA = ::cos( M_PI_4 );
  //  z
  //  ^
  //  |    7   6   5
  //  | 
  //  +----0---+---4--->x
  //  | 
  //  |    1   2   3 
  //  | 
  ::AddTorusPoints( majorR - minorR, zLavel, points, w0, weights ); // 0
  ::AddTorusPoints( majorR - minorR,-minorR, points, wA, weights ); // 1 
  ::AddTorusPoints( majorR         ,-minorR, points, w0, weights ); // 2 
  ::AddTorusPoints( majorR + minorR,-minorR, points, wA, weights ); // 3 
  ::AddTorusPoints( majorR + minorR, zLavel, points, w0, weights ); // 4
  ::AddTorusPoints( majorR + minorR, minorR, points, wA, weights ); // 5 
  ::AddTorusPoints( majorR         , minorR, points, w0, weights ); // 6 
  ::AddTorusPoints( majorR - minorR, minorR, points, wA, weights ); // 7 
}

//------------------------------------------------------------------------------
// \ru Построение точек тора. \en Torus Points Adding.
// ---
static
void GetAightKnots( double tBeg, double tEnd, SArray<double> & knots ) 
{
  double dt = ( tEnd - tBeg ) / 4;  
  double t = tBeg - dt;
  knots.Add( t );
  t = tBeg;
  knots.Add( t );
  knots.Add( t ); // 0
  t += dt;
  knots.Add( t );
  knots.Add( t ); // 1
  t += dt;
  knots.Add( t );
  knots.Add( t ); // 2
  t += dt;
  knots.Add( t );
  knots.Add( t ); // 3
  t = tEnd;
  knots.Add( t );
  knots.Add( t ); // 0
  t += dt;
  knots.Add( t );
  knots.Add( t ); // 1
}

//------------------------------------------------------------------------------
// \ru Построение поверхности тора. \en Torus surface creation.
// ---
static
MbSurface * CreateToroidalSplineSurface( double majorR, double minorR ) 
{
  MbSurface * result = nullptr;
  MbResultType res = rt_Error;

  if ( majorR > METRIC_NEAR && majorR > minorR - EPSILON ) {
    SArray<MbCartPoint3D> points( 64, 1 );
    SArray<double> weights( 64, 1 );
    SArray<double> uKnots( 13, 1 );
    SArray<double> vKnots( 13, 1 );
    size_t uDegree( 3 ), vDegree( 3 );
    size_t uCount( 8 ), vCount( 8 );
    bool uClosed( true ), vClosed( true );

    // get control points
    ::GetTorusPoints( majorR, minorR, points, weights );
    // get u-knots
    ::GetAightKnots( 0.0,  M_PI2, uKnots ); 
    // get v knots
    ::GetAightKnots( -M_PI, M_PI, vKnots ); 

    // create spline surface
    res = ::SplineSurface( points, weights, uCount, vCount, 
                           uDegree, uKnots, uClosed, 
                           vDegree, vKnots, vClosed, result );
  }  
  return ( res == rt_Success ) ? result : nullptr;
}

//------------------------------------------------------------------------------
// \ru Построение тела в форме тора. \en Torus solid creation.
// ---
static
MbSolid * CreateTorusSolid( MbSurface & surface, const MbSNameMaker & names, SimpleName name )
{  
  MbSolid * solid = nullptr;

  if ( surface.IsUClosed() && surface.IsVClosed() ) { // if surface is like torus
    MbLoop * loop = nullptr;
    // get parametric limits
    double umin = surface.GetUMin();
    double umax = surface.GetUMax();
    double vmin = surface.GetVMin();
    double vmax = surface.GetVMax();
    // create corner 2D-points by parametric limits
    MbCartPoint p1(umin, vmin);
    MbCartPoint p2(umax, vmin);
    MbCartPoint p3(umax, vmax);
    MbCartPoint p4(umin, vmax);

    MbSurfaceIntersectionCurve * intCurve1 = new MbSurfaceIntersectionCurve( surface, p1,p2, surface, p4,p3 );
    MbSurfaceIntersectionCurve * intCurve2 = new MbSurfaceIntersectionCurve( surface, p2,p3, surface, p1,p4 );

    MbCartPoint3D c0;
    surface.PointOn( umin, vmin, c0 );
    MbVertex * vert = new MbVertex( c0 );
    MbCurveEdge * edge1 = new MbCurveEdge( *vert, *vert, *intCurve1, true );
    MbCurveEdge * edge2 = new MbCurveEdge( *vert, *vert, *intCurve2, true );

    RPArray<MbOrientedEdge> loopList(4, 1);
    loopList.Add( new MbOrientedEdge( *edge1, true ) );
    loopList.Add( new MbOrientedEdge( *edge2, true ) );
    loopList.Add( new MbOrientedEdge( *edge1, false ) );
    loopList.Add( new MbOrientedEdge( *edge2, false ) );
    loop = new MbLoop( loopList );
    // create face by loop and surface
    MbFace * newFace = new MbFace( *loop, surface, true );
    names.MakeFaceName( *newFace, 1, name );
    // create shell
    MbFaceShell * shell = new MbFaceShell( *newFace );
    // create solid
    solid = new MbSolid( shell, nullptr );
  }

  return solid;
}

//------------------------------------------------------------------------------
// \ru Построение тора. \en Torus creation.
// ---
void SampleTorusSplineSolid() 
{
  // Пример построения NURBS поверхности или тела в форме тора.
  double majorR = 80.0;
  double minorR = 35.0;
  if ( ::GetDouble(IDS_ENTER_TORUS_RAD_BG, majorR) && 
       ::GetDouble(IDS_ENTER_TORUS_RAD_SM, minorR) ) {
    SPtr<MbSurface> surface( ::CreateToroidalSplineSurface( majorR, minorR ) ); 
    if ( surface != nullptr ) {
      int doSolid = 1; 
      ::GetInt( IDS_ENTER_OBJECT_TYPE, doSolid );
      if ( doSolid > 0 ) { // solid
        MbSNameMaker names;
        SimpleName name = st_TorusSurface;
        SPtr<MbSolid> solid( ::CreateTorusSolid( *surface, names, name ) );

        TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, solid );
      }
      else { // surface
        TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, surface );
      }
    }
  }
}


// eof