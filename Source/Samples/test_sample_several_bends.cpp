//////////////////////////////////////////////////////////////////////////////////////////
//
//    The file contains source codes demonstrating samples how to fold several bends.
//
//    WARNING: This code is only an example. The real application can require more 
//             functionality and data types for implementation.
// 
//////////////////////////////////////////////////////////////////////////////////////////

#include <test_samples.h>
#include <test_variables.h>
#include <test_manager.h>
#include <test_service.h>
#include <templ_sptr.h>
#include <cur_line_segment.h>
#include <cur_arc.h>
#include <action_sheet.h>
#include <action_solid.h>
#include <alg_draw.h>
#include <last.h>

using namespace c3d;

//------------------------------------------------------------------------------
// \ru Поиск сгибаемой грани. \en Finding the bending face.
// ---
static MbFace * FindBendingFace( const MbSolid &       solid,
                                 const MbCartPoint3D & centerPoint ) {
  MbFace * resFace = nullptr;

  for ( size_t faceIndex = solid.GetFacesCount(); faceIndex--; ) {
    MbFace * face = solid.GetFace( faceIndex );
    if ( face != nullptr ) {
      const MbSurface & surface = face->GetSurface();
      MbCartPoint projPoint;
      if ( surface.NearPointProjection(centerPoint, projPoint.x, projPoint.y, false/*ext*/) ) {
        MbCartPoint3D projPoint3D;
        surface._PointOn( projPoint, projPoint3D );
        if ( ::EqualPoints(centerPoint, projPoint3D, METRIC_REGION) ) {
          resFace = face;
          break;
        }
      }
    }
  }

  return resFace;
}


//------------------------------------------------------------------------------
// \ru Поиск внутренней и внешней граней сгиба. \en Finding the inner and the outer bend faces.
// ---
static bool FindBendFaces( const MbSolid &  solid,
                           const SimpleName mainName,
                                 MbFace *&  innerFace,
                                 MbFace *&  outerFace ) {
  innerFace = outerFace = nullptr;

  for ( size_t faceIndex = solid.GetFacesCount(); faceIndex--; ) {
    MbFace * face = solid.GetFace( faceIndex );
    if ( face != nullptr ) {
      const MbName & name = face->GetName();
      if ( name.GetMainName() == mainName ) {
        if ( name.IsInnerBend() ) {
          innerFace = face;
          if ( outerFace != nullptr )
            break;
        }
        if ( name.IsOuterBend() ) {
          outerFace = face;
          if ( innerFace != nullptr )
            break;
        }
      }
    }
  }

  return ( (innerFace != nullptr) && (outerFace != nullptr) );
}


//------------------------------------------------------------------------------
// \ru Создание сгиба по линии. \en Sketched bend creation.
// ---
static MbResultType CreateOneBend ( const SimpleName              mainName,
                                    const MbBendValues &          bendParams,
                                    const bool                    leftSideFixed,
                                    const bool                    unfolded,
                                    const SPtr<MbLineSegment3D> & line,
                                          SPtr<MbSolid> &         solid ) {
  MbResultType res = rt_Error;

  MbCartPoint3D center;
  RPArray<MbFace> bendingFaces( 1, 0 );
  MbFace * bendingFace = FindBendingFace( *solid, center );
  if ( bendingFace != nullptr ) {
    bendingFaces.Add( bendingFace );
    MbDisplacementCalculator displCalc( 1.0/*thick*/, bendParams.radius, bendParams.angle, bendParams.k );
    MbBendOverSegValues params( bendParams.k, bendParams.radius, bendParams.angle, bendParams.coneAngle,
                                displCalc.CalcDisplacement( MbDisplacementCalculator::dt_Center ),
                                leftSideFixed, rt_No/*MbeReleaseType*/ );
    MbSNameMaker nameMaker( mainName, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
    MbSolid * resSolid = nullptr;
    res = ::BendSheetSolidOverSegment( *solid, cm_Same, bendingFaces, *line, unfolded, params, nameMaker, resSolid );
    solid = resSolid;
  }

  return res;
}


//------------------------------------------------------------------------------
// \ru Создание нескольких сгибов по линии. \en The creation of several sketched bends.
// ---
void SampleSeveralBends() {
  MbResultType res = rt_Error;
  SPtr<MbSolid> resultSolid;

  // \ru Реализовано два способа построения сгибов по линии. Если unfolded равно true, сгибы строятся в разогнутом состоянии,
  // а последняя операция их сгибает в любой конфигурации (в примере сгибаются все сгибы, но можно сгибать выборочно).
  // Если unfolded равно false, то сгибы сразу строятся в согнутом состоянии.
  // \en There are two ways of constructing sketched bends. The first one, if unfolded is equal to true,
  // the bends are created unfolded and the last operation folds them in any desired combination (in the example all bends are folded, but you can choose only some of them).
  // The second way, if unfolded is equal to false, the bends are created folded at once.
  const bool unfolded = true;

  SetWaitCursor( true );

  { // \ru Создаём круглые пластины. \en Create round plates.
    RPArray<MbContour> contours( 5, 0 );
    RPArray<MbSNameMaker> nameMakers( 5, 0 );

    SPtr<MbContour> contour1( new MbContour() );
    MbCartPoint center1( 0.0, 0.0 );
    contour1->AddSegment( new MbArc(center1, 50.0/*radius*/) );
    contours.Add( contour1 );
    SPtr<MbSNameMaker> nameMaker1( new MbSNameMaker(0/*mainName*/, MbSNameMaker::i_SideNone/*sideAdd*/, 1/*buttAdd*/) );
    nameMaker1->Add( 11/*segName*/ );
    nameMakers.Add( nameMaker1 );

    SPtr<MbContour> contour2( new MbContour() );
    MbCartPoint center2( -120.0, 0.0 );
    contour2->AddSegment( new MbArc(center2, 50.0/*radius*/) );
    contours.Add( contour2 );
    SPtr<MbSNameMaker> nameMaker2( new MbSNameMaker(0/*mainName*/, MbSNameMaker::i_SideNone/*sideAdd*/, 2/*buttAdd*/) );
    nameMaker2->Add( 12/*segName*/ );
    nameMakers.Add( nameMaker2 );

    SPtr<MbContour> contour3( new MbContour() );
    MbCartPoint center3( 0.0, 60.0 );
    contour3->AddSegment( new MbArc(center3, 5.0/*radius*/) );
    contours.Add( contour3 );
    SPtr<MbSNameMaker> nameMaker3( new MbSNameMaker(0/*mainName*/, MbSNameMaker::i_SideNone/*sideAdd*/, 3/*buttAdd*/) );
    nameMaker3->Add( 13/*segName*/ );
    nameMakers.Add( nameMaker3 );

    SPtr<MbContour> contour4( new MbContour() );
    MbCartPoint center4( -20.0, 60.0 );
    contour4->AddSegment( new MbArc(center4, 5.0/*radius*/) );
    contours.Add( contour4 );
    SPtr<MbSNameMaker> nameMaker4( new MbSNameMaker(0/*mainName*/, MbSNameMaker::i_SideNone/*sideAdd*/, 4/*buttAdd*/) );
    nameMaker4->Add( 14/*segName*/ );
    nameMakers.Add( nameMaker4 );

    SPtr<MbContour> contour5( new MbContour() );
    MbCartPoint center5( 20.0, 60.0 );
    contour5->AddSegment( new MbArc(center5, 5.0/*radius*/) );
    contours.Add( contour5 );
    SPtr<MbSNameMaker> nameMaker5( new MbSNameMaker(0/*mainName*/, MbSNameMaker::i_SideNone/*sideAdd*/, 5/*buttAdd*/) );
    nameMaker5->Add( 15/*segName*/ );
    nameMakers.Add( nameMaker5 );

    MbSheetMetalValues parameters;
    parameters.side2.scalarValue = 5.0; // thickness
    parameters.k = 0.4; // K-factor value

    MbPlacement3D placement;
    RPArray<MbSMBendNames> bends( 0, 1 ); // no bends are going to be created
    MbSolid * result = nullptr;
    res = CreateSheetSolid( placement, contours, false/*unbended*/, parameters, &nameMakers, bends, result );
    if ( (res == rt_Success) && (result != nullptr) )
      resultSolid = result;
    else
      ::DeleteItem( result );
  }

  if ( (res == rt_Success) && (resultSolid != nullptr) ) { // \ru Создаём шлейфы. \en Create flexible cords.
    RPArray<MbContour> contours( 4, 0 );
    RPArray<MbSNameMaker> nameMakers( 4, 0 );

    SPtr<MbContour> contour1( new MbContour() );
    SPtr<MbSNameMaker> nameMaker1( new MbSNameMaker(1/*mainName*/, MbSNameMaker::i_SideNone/*sideAdd*/, 1/*buttAdd*/) );
    contour1->AddSegment( new MbLineSegment(-80.0/*u1*/, -5.0/*v1*/, -40.0/*u2*/, -5.0/*v2*/) );
    nameMaker1->Add( 11/*segName*/ );
    contour1->AddSegment( new MbLineSegment(-40.0/*u1*/, -5.0/*v1*/, -40.0/*u2*/, 5.0/*v2*/) );
    nameMaker1->Add( 12/*segName*/ );
    contour1->AddSegment( new MbLineSegment(-40.0/*u1*/, 5.0/*v1*/, -80.0/*u2*/, 5.0/*v2*/) );
    nameMaker1->Add( 13/*segName*/ );
    contour1->AddSegment( new MbLineSegment(-80.0/*u1*/, 5.0/*v1*/, -80.0/*u2*/, -5.0/*v2*/) );
    nameMaker1->Add( 14/*segName*/ );
    contours.Add( contour1 );
    nameMakers.Add( nameMaker1 );

    SPtr<MbContour> contour2( new MbContour() );
    SPtr<MbSNameMaker> nameMaker2( new MbSNameMaker(1/*mainName*/, MbSNameMaker::i_SideNone/*sideAdd*/, 2/*buttAdd*/) );
    contour2->AddSegment( new MbLineSegment(-2.5/*u1*/, 40.0/*v1*/, 2.5/*u2*/, 40.0/*v2*/) );
    nameMaker2->Add( 21/*segName*/ );
    contour2->AddSegment( new MbLineSegment(2.5/*u1*/, 40.0/*v1*/, 2.5/*u2*/, 57.5/*v2*/) );
    nameMaker2->Add( 22/*segName*/ );
    contour2->AddSegment( new MbLineSegment(2.5/*u1*/, 57.5/*v1*/, -2.5/*u2*/, 57.5/*v2*/) );
    nameMaker2->Add( 23/*segName*/ );
    contour2->AddSegment( new MbLineSegment(-2.5/*u1*/, 57.5/*v1*/, -2.5/*u2*/, 40.0/*v2*/) );
    nameMaker2->Add( 24/*segName*/ );
    contours.Add( contour2 );
    nameMakers.Add( nameMaker2 );

    SPtr<MbContour> contour3( new MbContour() );
    SPtr<MbSNameMaker> nameMaker3( new MbSNameMaker(1/*mainName*/, MbSNameMaker::i_SideNone/*sideAdd*/, 3/*buttAdd*/) );
    contour3->AddSegment( new MbLineSegment(-20.0/*u1*/, 57.5/*v1*/, -4.0/*u2*/, 57.5/*v2*/) );
    nameMaker3->Add( 31/*segName*/ );
    contour3->AddSegment( new MbLineSegment(-4.0/*u1*/, 57.5/*v1*/, -4.0/*u2*/, 62.5/*v2*/) );
    nameMaker3->Add( 32/*segName*/ );
    contour3->AddSegment( new MbLineSegment(-4.0/*u1*/, 62.5/*v1*/, -20.0/*u2*/, 62.5/*v2*/) );
    nameMaker3->Add( 33/*segName*/ );
    contour3->AddSegment( new MbLineSegment(-20.0/*u1*/, 62.5/*v1*/, -20.0/*u2*/, 57.5/*v2*/) );
    nameMaker3->Add( 34/*segName*/ );
    contours.Add( contour3 );
    nameMakers.Add( nameMaker3 );

    SPtr<MbContour> contour4( new MbContour() );
    SPtr<MbSNameMaker> nameMaker4( new MbSNameMaker(1/*mainName*/, MbSNameMaker::i_SideNone/*sideAdd*/, 5/*buttAdd*/) );
    contour4->AddSegment( new MbLineSegment(4.0/*u1*/, 57.5/*v1*/, 20.0/*u2*/, 57.5/*v2*/) );
    nameMaker4->Add( 41/*segName*/ );
    contour4->AddSegment( new MbLineSegment(20.0/*u1*/, 57.5/*v1*/, 20.0/*u2*/, 62.5/*v2*/) );
    nameMaker4->Add( 42/*segName*/ );
    contour4->AddSegment( new MbLineSegment(20.0/*u1*/, 62.5/*v1*/, 4.0/*u2*/, 62.5/*v2*/) );
    nameMaker4->Add( 43/*segName*/ );
    contour4->AddSegment( new MbLineSegment(4.0/*u1*/, 62.5/*v1*/, 4.0/*u2*/, 57.5/*v2*/) );
    nameMaker4->Add( 44/*segName*/ );
    contours.Add( contour4 );
    nameMakers.Add( nameMaker4 );

    MbSheetMetalValues parameters;
    parameters.side2.scalarValue = 1.0; // thickness
    parameters.k = 0.4; // K-factor value

    MbPlacement3D placement;
    RPArray<MbSMBendNames> bends( 0, 1 ); // no bends are going to be created
    MbSolid * result = nullptr;
    res = CreateSheetSolid( placement, contours, false/*unbended*/, parameters, &nameMakers, bends, result );
    if ( (res == rt_Success) && (result != nullptr) ) {
      // \ru Соединяем круглые платы гибкими шлейфами. \en Connect round boards with flexible cords.
      SPtr<MbSolid> resultSolid2( result );
      MbBooleanFlags flags;
      MbSNameMaker nameMaker( 3/*mainName*/, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
      MbBooleanOperationParams params( bo_Union, flags, nameMaker );
      SPtr<MbSolid> unionSolid;
      res = ::BooleanResult( resultSolid, cm_Same, resultSolid2, cm_Same, params, unionSolid );
      if ( (res == rt_Success) && (unionSolid != nullptr) )
        resultSolid = unionSolid;
      else
        resultSolid = nullptr;
    }
    else {
      ::DeleteItem( result );
      resultSolid = nullptr;
    }
  }

  // \ru Создание сгибов на шлейфах, сгнутых или разогнутых в зависимости от переменной unfolded.
  // \en Creation of bends on the flexible cords, folded or unfolded depending on the variable "unfolded".
  const MbBendValues bendParams1( 0.4/*coef*/,  5.0/*rad*/,   M_PI/*ang*/, 0.0/*coneAng*/ );
  const MbBendValues bendParams2( 0.4/*coef*/, 20.0/*rad*/,   0.35/*ang*/, 0.0/*coneAng*/ );
  const MbBendValues bendParams3( 0.4/*coef*/,  2.0/*rad*/, M_PI_2/*ang*/, 0.0/*coneAng*/ );

  if ( (res == rt_Success) && (resultSolid != nullptr) ) {
    MbCartPoint3D lineBeg( -60.0, -4.0, 0.0 ), lineEnd( -60.0, 4.0, 0.0 );
    SPtr<MbLineSegment3D> line( new MbLineSegment3D(lineBeg, lineEnd) );
    res = CreateOneBend( 4/*mainName*/, bendParams1, false/*leftSideFixed*/, unfolded, line, resultSolid );
  }

  if ( (res == rt_Success) && (resultSolid != nullptr) ) {
    MbCartPoint3D lineBeg( -10.0, 57.0, 0.0 ), lineEnd( -10.0, 62.0, 0.0 );
    SPtr<MbLineSegment3D> line( new MbLineSegment3D(lineBeg, lineEnd) );
    res = CreateOneBend( 5/*mainName*/, bendParams2, false/*leftSideFixed*/, unfolded, line, resultSolid );
  }

  if ( (res == rt_Success) && (resultSolid != nullptr) ) {
    MbCartPoint3D lineBeg( 10.0, 57.0, 0.0 ), lineEnd( 10.0, 62.0, 0.0 );
    SPtr<MbLineSegment3D> line( new MbLineSegment3D(lineBeg, lineEnd) );
    res = CreateOneBend( 6/*mainName*/, bendParams2, true/*leftSideFixed*/, unfolded, line, resultSolid );
  }

  if ( (res == rt_Success) && (resultSolid != nullptr) ) {
    MbCartPoint3D lineBeg( -2.0, 52.5, 0.0 ), lineEnd( 2.0, 52.5, 0.0 );
    SPtr<MbLineSegment3D> line( new MbLineSegment3D(lineBeg, lineEnd) );
    res = CreateOneBend( 7/*mainName*/, bendParams3, false/*leftSideFixed*/, unfolded, line, resultSolid );
  }

  if ( unfolded && (res == rt_Success) && (resultSolid != nullptr) ) {
    // \ru Сгиб сгибов, если они были построены разогнутыми.
    // \en Folding of the bends, if they were created unfolded.
    PArray<MbSheetMetalBend> bends( 4, 0, true );
    const MbCartPoint3D center;
    const MbFace * fixedFace = FindBendingFace( *resultSolid, center );
    if ( fixedFace != nullptr ) {
      MbFace * innerFace = nullptr, * outerFace = nullptr;
      if ( ::FindBendFaces(*resultSolid, 4/*mainName*/, innerFace, outerFace) )
        bends.Add( new MbSheetMetalBend(innerFace, outerFace, bendParams1.k, bendParams1.radius, bendParams1.angle, bendParams1.coneAngle) );
      if ( ::FindBendFaces(*resultSolid, 5/*mainName*/, innerFace, outerFace) )
        bends.Add( new MbSheetMetalBend(innerFace, outerFace, bendParams2.k, bendParams2.radius, bendParams2.angle, bendParams2.coneAngle) );
      if ( ::FindBendFaces(*resultSolid, 6/*mainName*/, innerFace, outerFace) )
        bends.Add( new MbSheetMetalBend(innerFace, outerFace, bendParams2.k, bendParams2.radius, bendParams2.angle, bendParams2.coneAngle) );
      if ( ::FindBendFaces(*resultSolid, 7/*mainName*/, innerFace, outerFace) )
        bends.Add( new MbSheetMetalBend(innerFace, outerFace, bendParams3.k, bendParams3.radius, bendParams3.angle, bendParams3.coneAngle) );
    }

    if ( bends.Count() == 4 ) {
      const MbCartPoint fixedPoint;
      MbSNameMaker nameMaker( 8/*mainName*/, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
      MbSolid * result = nullptr;
      res = ::BendSheetSolid( *resultSolid, cm_Same, bends, *fixedFace, fixedPoint, nameMaker, result );
      resultSolid = result;
    }
    else {
      resultSolid = nullptr;
      res = rt_Error;
    }
  }

  SetWaitCursor( false );

  if ( resultSolid )
    TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, resultSolid );
  else
    ::PutErrorMessage( res, IDS_TITLE_ERR );
}

