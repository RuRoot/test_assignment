//////////////////////////////////////////////////////////////////////////////////////////
//
//    The file contains source codes demonstrating samples how to split solids.
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
#include <cur_line_segment.h>
#include <cur_contour.h>
#include <cur_surface_intersection.h>
#include <surf_curve_bounded_surface.h>
#include <topology.h>
#include <creator.h>
#include <solid.h>
#include <action_surface.h>
#include <action_solid.h>
//
#include <last.h>

using namespace c3d;

//------------------------------------------------------------------------------
// \ru Разделить шар на 8 частей. \en Split the ball into 8 pieces.
// ---
void SampleSplitBallIntoEightPieces()
{
  SPtr<MbSolid> solid;
  MbElementarySolidResults results;

  SpacePointsVector points;
  { // create ball
    double radius = 100;
    points.push_back( MbCartPoint3D( 0, 0, 0 ) ); // p0
    points.push_back( MbCartPoint3D( 0, 0, radius ) ); // pz
    points.push_back( MbCartPoint3D( radius, 0, 0 ) ); // px
    MbSNameMaker snMaker;
    /*MbResultType res = */::ElementarySolid( MbElementarySolidParams( et_Sphere, points, snMaker ), results ); // ::ElementarySolid( points, et_Sphere, snMaker, sol );
    solid = results.GetResultSolid();
  }

  c3d::SurfacesSPtrVector cutters;
  { // create 3 cutting planes
    MbSurface * cutter = nullptr;
    points[0].SetZero(); // p0
    points[1].Init( 1, 0, 0 ); // px
    points[2].Init( 0, 1, 0 ); // py
    ::ElementarySurface( points[0], points[1], points[2], st_Plane, cutter );
    if ( cutter != nullptr )
      cutters.push_back( SPtr<MbSurface>( cutter ) );

    cutter = nullptr;
    points[0].SetZero(); // p0
    points[1].Init( 0, 1, 0 ); // px
    points[2].Init( 0, 0, 1 ); // py
    ::ElementarySurface( points[0], points[1], points[2], st_Plane, cutter );
    if ( cutter != nullptr )
      cutters.push_back( SPtr<MbSurface>( cutter ) );

    cutter = nullptr;
    points[0].SetZero(); // p0
    points[1].Init( 0, 0, 1 ); // px
    points[2].Init( 1, 0, 0 ); // py
    ::ElementarySurface( points[0], points[1], points[2], st_Plane, cutter );
    if ( cutter != nullptr )
      cutters.push_back( SPtr<MbSurface>( cutter ) );
  }

  std::vector< SPtr<MbSolid> > parts;
  parts.push_back( solid );
  solid = nullptr;

  // copy mode: full
  MbeCopyMode copyMode = cm_Copy; // don't use other copy modes if you are going to move parts

  const bool doClosed = true;
  const MbMergingFlags mergeFlags( true, true );

  size_t i, cuttersCnt = cutters.size();
  for ( i = 0; i < cuttersCnt; ++i ) { // cut by planes
    size_t partsCnt = parts.size();
    for ( size_t j = 0; j < partsCnt; ++j ) { // process parts
      {
        SolidsSPtrVector newParts;
        newParts.reserve( 2 );

        MbSNameMaker snMaker;

        MbShellCuttingParams cuttingParams( *cutters[i], false, mergeFlags, doClosed, snMaker );
        MbResultType res = ::SolidCutting( parts[j], copyMode, cuttingParams, newParts );

        if ( res == rt_Success ) {
          size_t addCnt = newParts.size();
          for ( size_t k = 0; k < addCnt; ++k ) {
            if ( newParts[k] != nullptr ) {
              parts.push_back( SolidSPtr( newParts[k] ) );
              // below (optional): you can delete solid history if it's not necessary or some memory's required
              parts.back()->DeleteCreators();
            }
          }
        }
        parts[j] = nullptr;
      }
    }
    // delete processed parts
    for ( size_t k = partsCnt; k--; )
      parts.erase( parts.begin() + k );
  }

  // add resulting parts to the model
  size_t partsCnt = parts.size();
  bool showRightNow = false;
  for ( i = 0; i < partsCnt; ++i ) {
    showRightNow = (i == partsCnt-1);
    TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, parts[i], showRightNow );
  }
}


//------------------------------------------------------------------------------
// Add the edge to the loop with vertices processing
//---
static
void AddNextEdge( MbLoop & loop, MbOrientedEdge & oEdge, bool isLastEdge, double mEps = METRIC_EPSILON )
{
  if ( loop.GetEdgesCount() > 0 ) {
    ptrdiff_t lastInd = loop.GetEdgesCount()-1;
    const MbVertex * v1 = &loop._GetOrientedEdge( lastInd )->GetEndVertex();
    const MbVertex * v2 = &oEdge.GetBegVertex();
    if ( v1 != v2 && c3d::EqualPoints( v1->GetCartPoint(), v2->GetCartPoint(), mEps ) ) // Set common vertices
      oEdge.SetBegVertex( *v1 );

    if ( isLastEdge ) {
      v1 = &loop._GetOrientedEdge( 0 )->GetBegVertex();
      v2 = &oEdge.GetEndVertex();
      if ( v1 != v2 && c3d::EqualPoints( v1->GetCartPoint(), v2->GetCartPoint(), mEps ) ) // Set common vertices
        oEdge.SetEndVertex( *v1 );
    }
  }

  loop.AddEdge( oEdge );
}

//------------------------------------------------------------------------------
// Add segment's copy to the contour
// ---
inline
bool AddSegmentCopy( const MbCurve & segment, MbContour & contour, double eps = PARAM_EPSILON )
{
  SPtr<MbCurve> segmentCopy( static_cast<MbCurve *>( &segment.Duplicate() ) );
  if ( contour.AddCurveWithRuledCheck( *segmentCopy, eps, true ) ) // true - add to the end of contour
    return true;
  PRECONDITION( false );
  return false;
}

//------------------------------------------------------------------------------
// Create standalone face
// ---
static
MbFace * CreateIsolatedFace( const MbSurface & surface0, SPtr<MbContour> & contour, bool sameContour )
{
  MbFace * face = nullptr;

  if ( contour && contour->IsClosed() ) {
    SPtr<MbSurface> surface( static_cast<MbSurface *>( &surface0.GetSurface().Duplicate() ) );

    std::vector< SPtr<MbContour> > contours;
    contours.push_back( contour );
    surface = new MbCurveBoundedSurface( *surface, contours, sameContour );

    SPtr<MbLoop> loop( new MbLoop );
    size_t edgesCnt = contour->GetSegmentsCount();

    SPtr<MbSurfaceIntersectionCurve> intCurve;
    for ( size_t i = 0; i < edgesCnt; ++i ) {
      MbCurve * segment = contour->SetSegment( i );

      intCurve = new MbSurfaceIntersectionCurve( surface->GetSurface(), *segment, 
                                                 surface->GetSurface(), *segment, 
                                                 cbt_Boundary, true, true );
      MbOrientedEdge * edge = new MbOrientedEdge( *new MbCurveEdge( *intCurve, true ), true );

      ::AddNextEdge( *loop, *edge, (i == edgesCnt-1) );
    }

    if ( loop->GetEdgesCount() == edgesCnt ) {
      face = new MbFace( *loop, *surface, true );
    }
  }

  return face;
}


//------------------------------------------------------------------------------
// Stitch common edges of two isolated faces with same surfaces
// ---
static
bool StitchIsolatedFaces( MbFace & face0, MbFace & face1 )
{
  bool changed = false;
  PRECONDITION( face0.GetLoopsCount() == 1 && face1.GetLoopsCount() == 1 );
  const MbSurface * surface0 = &face0.GetSurface().GetSurface();
  const MbSurface * surface1 = &face1.GetSurface().GetSurface();

  if ( surface0 != surface1 && surface0->IsSame( *surface1 ) ) {
    MbLoop * loop0 = face0._GetLoop( 0 );
    for ( size_t i0 = loop0->GetEdgesCount(); i0--; ) {
      MbOrientedEdge * orientEdge0 = loop0->_GetOrientedEdge( i0 );
      MbCurveEdge * edge0 = &orientEdge0->GetCurveEdge();

      const MbeCurveBuildType buildType = edge0->GetBuildType();
      if ( buildType != cbt_Boundary )
        continue;

      MbLoop * loop1 = face1._GetLoop( 0 );
      const size_t edgesCnt1 = loop1->GetEdgesCount();
      for ( size_t i1 = edgesCnt1; i1--; ) {
        MbOrientedEdge * orientEdge1 = loop1->_GetOrientedEdge( i1 );
        MbCurveEdge * edge1 = &orientEdge1->GetCurveEdge();

        if ( buildType == edge1->GetBuildType() ) { // Check build type
          const MbCurve * pCurve0 = &edge0->GetIntersectionCurve().GetCurveOneCurve();
          const MbCurve * pCurve1 = &edge1->GetIntersectionCurve().GetCurveOneCurve();

          if ( pCurve0->IsInverseSame( *pCurve1 ) ) { // Check that curves are inverse same
            MbSurfaceIntersectionCurve & intCurve0 = edge0->SetIntersectionCurve();
            MbSurfaceCurve & surfCurve = intCurve0.SetCurveTwo();

            bool failed = true;
            if ( surfCurve.ChangeSurface( *surface1 ) ) { // Replace second surface
              SPtr<MbCurve> pCurveCopy0( static_cast<MbCurve *>( &pCurve0->Duplicate() ) );
              if ( surfCurve.ChangeCurve( *pCurveCopy0 ) ) { // Replace second curve
                intCurve0.SetBuildType( cbt_Ordinary ); // Set build type as exact
                intCurve0.Refresh(); // Reset internal data of the curve

                // Set neighbor face of the edge
                if ( orientEdge0->GetOrientation() )
                  edge0->SetFaceMinus( &face1 );
                else
                  edge0->SetFacePlus( &face1 );

                orientEdge1->InitEdge( *edge0, !orientEdge0->GetOrientation() ); // Replace edge of the second face

                size_t nextInd = (i1 + 1) % edgesCnt1;
                size_t prevInd = (edgesCnt1 + i1 - 1) % edgesCnt1;

                MbOrientedEdge * prevEdge1 = loop1->GetOrientedEdge( prevInd );
                MbOrientedEdge * nextEdge1 = loop1->GetOrientedEdge( nextInd );

                // Set common vertices
                if ( prevEdge1 != orientEdge1 ) {
                  if ( &prevEdge1->GetEndVertex() != &orientEdge1->GetBegVertex() ) {
                    prevEdge1->SetEndVertex( orientEdge1->GetBegVertex() );
                  }
                }
                if ( nextEdge1 != orientEdge1 ) {
                  if ( &nextEdge1->GetBegVertex() != &orientEdge1->GetEndVertex() ) {
                    nextEdge1->SetBegVertex( orientEdge1->GetEndVertex() );
                  }
                }

                // Now the faces are stitched upon this edge
                failed = false;
                changed = true;
              }
            }
            PRECONDITION( !failed );
          }
        }
      }
    }
  }

  return changed;
}

//------------------------------------------------------------------------------
// Stitch face with two inner split faces
// ---
static
bool StitchWithInnerSplitFaces( MbFace & dstFace, const MbContour & dstContour, MbFace & face0, MbFace & face1 )
{
  PRECONDITION( dstFace.GetLoopsCount() == 1 && face0.GetLoopsCount() == 1 && face1.GetLoopsCount() == 1 );
  PRECONDITION( &dstFace != &face0 && &dstFace != &face1 );

  MbLoop * loop0 = face0.GetLoop( 0 );
  MbLoop * loop1 = face1.GetLoop( 0 );
  const size_t edgesCnt0 = loop0->GetEdgesCount();
  const size_t edgesCnt1 = loop1->GetEdgesCount();

  std::vector<MbOrientedEdge *> orientEdges;  
  orientEdges.reserve( edgesCnt0 + edgesCnt1 - 2 );

  size_t i;
  for ( i = 0; i < edgesCnt0; ++i ) {
    MbOrientedEdge * orientEdge = loop0->_GetOrientedEdge( i );
    if ( orientEdge->GetCurveEdge().GetBuildType() == cbt_Boundary )
      orientEdges.push_back( orientEdge );
  }
  for ( i = 0; i < edgesCnt1; ++i ) {
    MbOrientedEdge * orientEdge = loop1->_GetOrientedEdge( i );
    if ( orientEdge->GetCurveEdge().GetBuildType() == cbt_Boundary )
      orientEdges.push_back( orientEdge );
  }
  PRECONDITION( orientEdges.size() == edgesCnt0 + edgesCnt1 - 2 );

  const MbSurface & dstSurface = dstFace.GetSurface().GetSurface();

  SPtr<MbLoop> dstLoop( new MbLoop );

  const size_t edgesCnt = orientEdges.size();
  const size_t segmentsCnt = dstContour.GetSegmentsCount();

  size_t usedCnt = 0;
  for ( i = 0; i < segmentsCnt; ++i ) {
    const MbCurve * dstSegment = dstContour.GetSegment( i );
    for ( size_t j = 0; j < edgesCnt; j++ ) {
      MbOrientedEdge * orientEdge0 = orientEdges[j];
      MbCurveEdge & edge0 = orientEdge0->GetCurveEdge();
      bool orient0 = orientEdge0->GetOrientation();

      MbSurfaceIntersectionCurve & intCurve0 = edge0.SetIntersectionCurve();
      const MbCurve * segment0 = &intCurve0.GetCurveOneCurve();

      if ( dstSegment->IsSame( *segment0 ) || dstSegment->IsInverseSame( *segment0 ) ) {
        MbSurfaceCurve & surfCurve = intCurve0.SetCurveTwo();

        if ( surfCurve.ChangeSurface( dstSurface ) ) { // Replace second surface
          SPtr<MbCurve> segmentCopy0( static_cast<MbCurve *>( &segment0->Duplicate() ) );
          if ( surfCurve.ChangeCurve( *segmentCopy0 ) ) { // Replace second curve
            intCurve0.SetBuildType( cbt_Ordinary ); // Set build type as exact
            intCurve0.Refresh(); // Reset internal data of the curve

            // Set neighbor face of the edge
            if ( orient0 )
              edge0.SetFaceMinus( &dstFace );
            else
              edge0.SetFacePlus( &dstFace );

            // Create oriented edge for inner loop of dstFace
            SPtr<MbOrientedEdge> orientEdge( new MbOrientedEdge( edge0, !orient0 ) );
            if ( dstFace.IsSameSense() )
              dstLoop->AddEdge( *orientEdge );
            else {
              dstLoop->InsertEdge( 0, *orientEdge );
            }
            usedCnt++;
          }
        }
      }
    }
  }
  PRECONDITION( usedCnt == segmentsCnt );
  if ( usedCnt == segmentsCnt ) {
    dstFace.AddLoop( *dstLoop );
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
// \ru Создать грани разъема на цилиндрической грани. \en Create split faces on cylinder face.
// ---
void SampleSplitCylinderManualSplit()
{
  SPtr<MbSolid> solid;
  MbElementarySolidResults results;

  SpacePointsVector points;
  { // create cylinder
    double radius = 20;
    double height = 100;
    points.push_back( MbCartPoint3D( 0, 0, 0 ) ); // p0
    points.push_back( MbCartPoint3D( 0, 0, height ) ); // pz
    points.push_back( MbCartPoint3D( radius, 0, 0 ) ); // px
    MbSNameMaker snMaker;
    /*MbResultType res = */::ElementarySolid( MbElementarySolidParams( et_Cylinder, points, snMaker ), results );  // ::ElementarySolid( points, et_Cylinder, snMaker, sol );
    solid = results.GetResultSolid();
  }

  static bool testInvertedShell = false;
  if ( testInvertedShell )
    solid->GetShell()->Reverse();

  if ( solid != nullptr ) {
    TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, results.GetResultSolid(), true );

    MbFace * dstFace = nullptr;
    { // select cylinder face
      dstFace = solid->GetFace(0);
      if ( dstFace->GetSurface().GetSurface().IsA() != st_CylinderSurface ) {
        dstFace = solid->GetFace(1);
        if ( dstFace->GetSurface().GetSurface().IsA() != st_CylinderSurface ) {
          dstFace = solid->GetFace(2);
          if ( dstFace->GetSurface().GetSurface().IsA() != st_CylinderSurface ) {
            PRECONDITION( false );
            dstFace = nullptr;
          }
        }
      }
    }

    if ( dstFace != nullptr ) {
      //
      // +--------------------------+ 
      // |                          | vmax=1
      // | y1 +---5-->-+---6-->-+   |
      // |    ^        ^        ^   |
      // |    |2       |3       |4  |
      // | y0 +---0-->-+---1-->-+   | 
      // |    x0       x1       x2  | vmax=0
      // +--------------------------+ 
      // umin=0              umax=2Pi

      std::vector< SPtr<MbCurve> > segments;
      segments.reserve( 7 );
      {
        double x0 = M_PI_2;
        double x1 = x0 + M_PI_2;
        double x2 = x1 + M_PI_2;

        double y0 = 0.25;
        double y1 = 0.75;

        segments.push_back( SPtr<MbCurve>( new MbLineSegment( MbCartPoint( x0, y0 ), MbCartPoint( x1, y0 ) ) ) ); // 0
        segments.push_back( SPtr<MbCurve>( new MbLineSegment( MbCartPoint( x1, y0 ), MbCartPoint( x2, y0 ) ) ) ); // 1

        segments.push_back( SPtr<MbCurve>( new MbLineSegment( MbCartPoint( x0, y0 ), MbCartPoint( x0, y1 ) ) ) ); // 2
        segments.push_back( SPtr<MbCurve>( new MbLineSegment( MbCartPoint( x1, y0 ), MbCartPoint( x1, y1 ) ) ) ); // 3
        segments.push_back( SPtr<MbCurve>( new MbLineSegment( MbCartPoint( x2, y0 ), MbCartPoint( x2, y1 ) ) ) ); // 4

        segments.push_back( SPtr<MbCurve>( new MbLineSegment( MbCartPoint( x0, y1 ), MbCartPoint( x1, y1 ) ) ) ); // 5
        segments.push_back( SPtr<MbCurve>( new MbLineSegment( MbCartPoint( x1, y1 ), MbCartPoint( x2, y1 ) ) ) ); // 6
      }

      bool goNext = true;

      // Contour of split face 0
      SPtr<MbContour> contour0( new MbContour ); // 0 - 3 - 5 - 2
      {
        goNext &= ::AddSegmentCopy( *segments[0], *contour0 );
        goNext &= ::AddSegmentCopy( *segments[3], *contour0 );
        goNext &= ::AddSegmentCopy( *segments[5], *contour0 );
        goNext &= ::AddSegmentCopy( *segments[2], *contour0 );
        contour0->CheckClosed();
        PRECONDITION( contour0->GetArea() > LENGTH_EPSILON );
      }
      // Contour of split face 1
      SPtr<MbContour> contour1( new MbContour ); // 1 - 4 - 6 - 3
      {
        goNext &= ::AddSegmentCopy( *segments[1], *contour1 );
        goNext &= ::AddSegmentCopy( *segments[4], *contour1 );
        goNext &= ::AddSegmentCopy( *segments[6], *contour1 );
        goNext &= ::AddSegmentCopy( *segments[3], *contour1 );
        contour1->CheckClosed();
        PRECONDITION( contour1->GetArea() > LENGTH_EPSILON );
      }
      // Inner contour of dstFace
      SPtr<MbContour> dstContour( new MbContour ); // 0 - 1 - 4 - 6 - 5 - 2
      {
        goNext &= ::AddSegmentCopy( *segments[0], *dstContour );
        goNext &= ::AddSegmentCopy( *segments[1], *dstContour );
        goNext &= ::AddSegmentCopy( *segments[4], *dstContour );
        goNext &= ::AddSegmentCopy( *segments[6], *dstContour );
        goNext &= ::AddSegmentCopy( *segments[5], *dstContour );
        goNext &= ::AddSegmentCopy( *segments[2], *dstContour );
        dstContour->CheckClosed();
        dstContour->Inverse(); // Contour has to be inverted because it's used as inner contour
        PRECONDITION( dstContour->GetArea() < -LENGTH_EPSILON );
      }

      if ( goNext ) {
        SPtr<MbFace> face0( ::CreateIsolatedFace( dstFace->GetSurface().GetSurface(), contour0, true ) );
        SPtr<MbFace> face1( ::CreateIsolatedFace( dstFace->GetSurface().GetSurface(), contour1, true ) );
        PRECONDITION( face0 && face1 );

        if ( face0 && face1 ) {
          if ( !dstFace->IsSameSense() ) { // Invert faces if the orientation of dstFace is negative relative to its surface carrier
            face0->TotalReverse();
            face1->TotalReverse();
          }

          if ( ::StitchIsolatedFaces( *face0, *face1 ) ) {
            { // Add as shell of stitched patch faces only.
              c3d::ShellSPtr testShell( new MbFaceShell );
              testShell->AddFace( *face0 );
              testShell->AddFace( *face1 );
              testShell->CheckClosed();
              testShell = testShell->Duplicate();
              SPtr<MbSolid> testSolid( new MbSolid( testShell, nullptr ) );
              TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, testSolid );
            }

            if ( ::StitchWithInnerSplitFaces( *dstFace, *dstContour, *face0, *face1 ) ) {
              MbFaceShell * shell = solid->GetShell();
              shell->AddFace( *face0 );
              shell->AddFace( *face1 );
              shell->CheckClosed();
              PRECONDITION( shell->IsClosed() );

              SimpleName testMainName = ct_SplitShell;
              MbSNameMaker snMaker( testMainName );
              MbCheckTopologyParams ctPars( true, snMaker );

#ifdef C3D_DEBUG
              MbResultType res = shell->CheckTopology( ctPars );
              PRECONDITION( res == rt_Success );
#else
              shell->CheckTopology( ctPars ); // Check shell topology
#endif
              shell->SetShellNames( snMaker ); // Set names for items without names

              ::SleepCP( 250 ); // wait 0.25 sec.
              TestVariables::viewManager->RefreshModel();
            }
          }
        }
      }
    }
  }
}


// eof