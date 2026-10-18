//////////////////////////////////////////////////////////////////////////////////////////
//
//    The file contains source codes demonstrating samples how to create closed corners.
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
#include <alg_draw.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// \ru Замыкание угла. \en A closed corner creation.
// ---
void SampleCloseCorner() {
  MbResultType res = rt_Error;
  SPtr<MbSolid> resultSolid = nullptr;

  std::vector<MbSMBendNames *> resultBends;

  SetWaitCursor( true );

  { // \ru Создаём квадратную пластину. \en Create a square plate.
    MbPlacement3D placement;
    SPtr<MbContour> contour( new MbContour() );
    contour->AddSegment( new MbLineSegment(-50.0/*u1*/, -50.0/*v1*/, 50.0/*u2*/, -50.0/*v2*/) );
    contour->AddSegment( new MbLineSegment(50.0/*u1*/, -50.0/*v1*/, 50.0/*u2*/, 50.0/*v2*/) );
    contour->AddSegment( new MbLineSegment(50.0/*u1*/, 50.0/*v1*/, -50.0/*u2*/, 50.0/*v2*/) );
    contour->AddSegment( new MbLineSegment(-50.0/*u1*/, 50.0/*v1*/, -50.0/*u2*/, -50.0/*v2*/) );
    RPArray<MbContour> contours( 1, 0 );
    contours.Add( contour );

    MbSheetMetalValues parameters;
    parameters.side1.scalarValue = 1.0; // thickness
    parameters.k = 0.5; // K-factor value

    RPArray<MbSNameMaker> nameMakers( 0, 1 );
    nameMakers.Add( new MbSNameMaker(0/*mainName*/, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/) );

    RPArray<MbSMBendNames> operation0Bends( 0, 1 ); // no bends are going to be created
    MbSolid * resSolid = nullptr;
    res = CreateSheetSolid( placement, contours, false/*unbended*/, parameters, &nameMakers, operation0Bends, resSolid );
    resultSolid = resSolid;
    delete nameMakers[0];
  }

  if ( (resultSolid != nullptr) && (res == rt_Success) ) { // \ru Создаём сгибы по периметру. \en Create bends around the perimeter.
    ConstEdgesVector edgesForFlange;
    MbFace * sheetFace = nullptr;
    for ( size_t faceIndex = resultSolid->GetFacesCount(); faceIndex--; ) {
      MbFace * face = resultSolid->GetFace( faceIndex );
      if ( (face != nullptr) && face->GetName().IsSheet() ) {
        sheetFace = face;
        break;
      }
    }

    if ( sheetFace != nullptr ) {
      const MbLoop * outerLoop = sheetFace->GetLoop( 0 );
      if ( outerLoop != nullptr ) {
        for ( size_t edgeIndex = outerLoop->GetEdgesCount(); edgeIndex--; ) {
          MbOrientedEdge * orEdge = outerLoop->_GetOrientedEdge( edgeIndex );
          if ( orEdge != nullptr )
            edgesForFlange.push_back( &orEdge->GetCurveEdge() );
        }
      }
    }

    if ( edgesForFlange.size() == 4 ) {
      MbSNameMaker nameMaker( 1/*mainName*/, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
      MbBendByEdgeValues values( 0.5/*k*/, 1.0/*radius*/, M_PI_2/*bendAngle*/, 0.0/*coneAngle*/, 30.0/*length*/, 0.0/*deepness*/, rt_No );
      values.SideLeftInit( 0.0/*distance*/, 0.0 * Math::DEGRAD/*angle*/, 0.0 * Math::DEGRAD/*deviation*/, 0.0/*widening*/ );
      values.SideRightInit( 0.0/*distance*/, 0.0 * Math::DEGRAD/*angle*/, 0.0 * Math::DEGRAD/*deviation*/, 0.0/*widening*/ );
      values.SlotInit( 0.0/*width*/, 0.0/*depth*/, 0.0/*radius*/ );
      values.miterMiddle.allow = false; // don't close corner
      //values.miterMiddle.cornerWay = MbClosedCornerValues::ccTight;
      //values.miterMiddle.gap = 0.5;
      //values.miterMiddle.bendsWay.push_back( MbClosedCornerValues::cbEdge );
      std::vector<MbSMBendNames *> operation1Bends; // individual parameters of four bends to be created
      operation1Bends.push_back( new MbSMBendNames(0.5/*k*/, 1.0/*radius*/, M_PI_2/*angle*/, SimpleName(SIMPLENAME_MAX)/*SimpleName1*/, SimpleName(SIMPLENAME_MAX)/*SimpleName2*/) );
      operation1Bends.push_back( new MbSMBendNames(0.5/*k*/, 1.0/*radius*/, M_PI_2/*angle*/, SimpleName(SIMPLENAME_MAX)/*SimpleName1*/, SimpleName(SIMPLENAME_MAX)/*SimpleName2*/) );
      operation1Bends.push_back( new MbSMBendNames(0.5/*k*/, 1.0/*radius*/, M_PI_2/*angle*/, SimpleName(SIMPLENAME_MAX)/*SimpleName1*/, SimpleName(SIMPLENAME_MAX)/*SimpleName2*/) );
      operation1Bends.push_back( new MbSMBendNames(0.5/*k*/, 1.0/*radius*/, M_PI_2/*angle*/, SimpleName(SIMPLENAME_MAX)/*SimpleName1*/, SimpleName(SIMPLENAME_MAX)/*SimpleName2*/) );
      MbBendSheetSolidByEdgesParams params( edgesForFlange, false/*unbended*/, values, nameMaker );
      SPtr<MbSolid> newSolid = nullptr;
      res = BendSheetSolidByEdges( resultSolid, cm_Copy, params, operation1Bends, newSolid );
      if ( newSolid != nullptr ) {
        resultSolid = newSolid;
        resultBends.insert(resultBends.end(), operation1Bends.begin(), operation1Bends.end()); // save created bends data in the common array
      }
      else {
        for ( MbSMBendNames * bendNames : operation1Bends )
          delete bendNames;
        operation1Bends.clear();
      }
    }
  }

  // \ru Создаём два сгиба на продолжении предыдущих. \en Create two bends on the extension of the previous ones.
  if ( (resultSolid != nullptr) && (res == rt_Success) && (resultBends.size() > 1) ) {
    MbSMBendNames * bend0 = resultBends[0];
    MbSMBendNames * bend1 = resultBends[1];
    if ( (bend0 != nullptr) && (bend1 != nullptr) ) {
      const MbFace * innerBendFace0 = resultSolid->FindFaceByName( bend0->innerFaceName );
      const MbFace * innerBendFace1 = resultSolid->FindFaceByName( bend1->innerFaceName );
      if ( (innerBendFace0 != nullptr) && (innerBendFace1 != nullptr) ) {
        ConstEdgesVector edgesForFlange;
        {
          const MbLoop * loop = innerBendFace0->GetLoop( 0 );
          if ( (loop != nullptr) && (loop->GetEdgesCount() == 4) ) {
            for ( size_t edgeIndex = 4; edgeIndex--; ) {
              const MbOrientedEdge * orEdge = loop->_GetOrientedEdge( edgeIndex );
              if ( orEdge != nullptr ) {
                const MbFace * neighborFace = orEdge->GetFaceMinus();
                if ( (neighborFace != nullptr) && (neighborFace->GetName().GetMainName() == 0) ) {
                  const MbOrientedEdge * oppositeOrEdge = loop->_GetOrientedEdge( (edgeIndex + 2) % 4 );
                  if ( oppositeOrEdge != nullptr ) {
                    const MbFace * extensionFace = oppositeOrEdge->GetFaceMinus();
                    if ( extensionFace != nullptr ) {
                      const MbLoop * extensionLoop = extensionFace->GetLoop( 0 );
                      if ( (extensionLoop != nullptr) && (extensionLoop->GetEdgesCount() == 4) ) {
                        for ( size_t extEdgeIndex = 4; extEdgeIndex--; ) {
                          const MbOrientedEdge * extOrEdge = extensionLoop->_GetOrientedEdge( extEdgeIndex );
                          if ( (extOrEdge != nullptr) && (&extOrEdge->GetCurveEdge() == &oppositeOrEdge->GetCurveEdge()) ) {
                            extOrEdge = extensionLoop->GetOrientedEdge( (extEdgeIndex + 2) % 4 );
                            edgesForFlange.push_back( &extOrEdge->GetCurveEdge() );
                            edgeIndex = extEdgeIndex = 0;
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }

        {
          const MbLoop * loop = innerBendFace1->GetLoop( 0 );
          if ( (loop != nullptr) && (loop->GetEdgesCount() == 4) ) {
            for ( size_t edgeIndex = 4; edgeIndex--; ) {
              const MbOrientedEdge * orEdge = loop->_GetOrientedEdge( edgeIndex );
              if ( orEdge != nullptr ) {
                const MbFace * neighborFace = orEdge->GetFaceMinus();
                if ( (neighborFace != nullptr) && (neighborFace->GetName().GetMainName() == 0) ) {
                  MbOrientedEdge * oppositeOrEdge = loop->_GetOrientedEdge( (edgeIndex + 2) % 4 );
                  if ( oppositeOrEdge != nullptr ) {
                    const MbFace * extensionFace = oppositeOrEdge->GetFaceMinus();
                    if ( extensionFace != nullptr ) {
                      const MbLoop * extensionLoop = extensionFace->GetLoop( 0 );
                      if ( (extensionLoop != nullptr) && (extensionLoop->GetEdgesCount() == 4) ) {
                        for ( size_t extEdgeIndex = 4; extEdgeIndex--; ) {
                          const MbOrientedEdge * extOrEdge = extensionLoop->_GetOrientedEdge( extEdgeIndex );
                          if ( (extOrEdge != nullptr) && (&extOrEdge->GetCurveEdge() == &oppositeOrEdge->GetCurveEdge()) ) {
                            extOrEdge = extensionLoop->GetOrientedEdge( (extEdgeIndex + 2) % 4 );
                            edgesForFlange.push_back( &extOrEdge->GetCurveEdge() );
                            edgeIndex = extEdgeIndex = 0;
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }

        if ( edgesForFlange.size() == 2 ) {
          MbSNameMaker nameMaker( 2/*mainName*/, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
          MbBendByEdgeValues values( 0.5/*k*/, 1.0/*radius*/, M_PI_2/*bendAngle*/, 0.0/*coneAngle*/, 30.0/*length*/, 0.0/*deepness*/, rt_No );
          values.SideLeftInit( 0.0, 0.0 * Math::DEGRAD, 0.0 * Math::DEGRAD, 0.0 );
          values.SideRightInit( 0.0, 0.0 * Math::DEGRAD, 0.0 * Math::DEGRAD, 0.0 );
          values.SlotInit( 0.0/*width*/, 0.0/*depth*/, 0.0/*radius*/ );
          values.miterMiddle.allow = false;
          //params.miterMiddle.cornerWay = MbClosedCornerValues::ccTight;
          //params.miterMiddle.gap = 0.5;
          //params.miterMiddle.bendsWay.push_back( MbClosedCornerValues::cbEdge );
          std::vector<MbSMBendNames *> operation2Bends; // individual parameters for two more bends
          operation2Bends.push_back( new MbSMBendNames(0.5/*k*/, 1.0/*radius*/, M_PI_2/*angle*/, SimpleName(SIMPLENAME_MAX)/*SimpleName1*/, SimpleName(SIMPLENAME_MAX)/*SimpleName2*/) );
          operation2Bends.push_back( new MbSMBendNames(0.5/*k*/, 1.0/*radius*/, M_PI_2/*angle*/, SimpleName(SIMPLENAME_MAX)/*SimpleName1*/, SimpleName(SIMPLENAME_MAX)/*SimpleName2*/) );
          MbBendSheetSolidByEdgesParams params( edgesForFlange, false/*unbended*/, values, nameMaker );
          SPtr<MbSolid> newSolid = nullptr;
          res = BendSheetSolidByEdges( resultSolid, cm_Copy, params, operation2Bends, newSolid );
          if ( newSolid != nullptr ) {
            resultSolid = newSolid;
            resultBends.insert(resultBends.end(), operation2Bends.begin(), operation2Bends.end()); // save created bends data in the common array
          }
          else {
            for ( MbSMBendNames * bendNames : operation2Bends )
              delete bendNames;
            operation2Bends.clear();
          }
        }
      }
    }
  }

  // \ru Создаём замыкание угла. \en Close corner.
  if ( (resultSolid != nullptr) && (res == rt_Success) && (resultBends.size() > 1) ) {
    MbCloseCornerResults cornerResult;
    {
      MbSMBendNames * bendPlus = resultBends[resultBends.size() - 1];
      MbSMBendNames * bendMinus = resultBends[resultBends.size() - 2];
      if ( (bendPlus != nullptr) && (bendMinus != nullptr) ) {
        FaceSPtr farBendFacePlus( resultSolid->FindFaceByName( bendPlus->innerFaceName ) );
        FaceSPtr farBendFaceMinus( resultSolid->FindFaceByName( bendMinus->innerFaceName ) );
        if ( (farBendFacePlus != nullptr) && (farBendFaceMinus != nullptr) ) {
          FacesSPtrVector facesPlus, facesMinus;
          facesPlus.push_back( farBendFacePlus );
          facesMinus.push_back( farBendFaceMinus );
          MbCloseCornerParams cornerParams( facesPlus, facesMinus );
          // We may use GetParamsForCloseCorner variant with faces arrays like in this example
          // facesPlus array must contain either side face/faces that move when the corner is closing, or a sheet face neighbor to one of them
          // facesMinus array must contain either same faces as in facesPlus array (in this case the corner will be defined automatically)
          // or similar faces from the other side of the corner if automatic selection is not possible, like in this example,
          // because two corners is suitable for closing in this example.
          // 
          // There is also another variant of GetParamsForCloseCorner that takes two edges instead of faces.
          // The edges must either be the ones moving when closing the corner or the elongating ones.
          // If the plus edge is equal to the minus edge, the choice of the corner is automatic.
          // In order to clarify one of the two possible corners we must select minus edge from the other side of the corner.
          ::GetParamsForCloseCorner( cornerParams, cornerResult );
        }
      }
    }

    if ( ( cornerResult._edgePlus != nullptr) && ( cornerResult._edgeMinus != nullptr) ) {
      MbSNameMaker nameMaker( 3/*mainName*/, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
      MbSolid * newSolid = nullptr;
      MbClosedCornerValues params( MbClosedCornerValues::ccTight,
                                   MbClosedCornerValues::cbEdge,
                                   MbClosedCornerValues::cpBend,
                                   1.0/*gap*/,
                                   0.0/*diameter*/,
                                   0.0/*shift*/,
                                   0.5/*kPlus*/,
                                   0.5/*kMinus*/,
                                   M_PI_4/*angle*/,
                                   true/*plus*/,
                                   true/*prolong*/,
                                   false/*acrossBend*/ );
      res = ::CloseCorner( *resultSolid, cm_Copy, cornerResult._edgePlus, cornerResult._edgeMinus,
                           params, nameMaker, newSolid );
      if ( newSolid != nullptr )
        resultSolid = newSolid;
    }
  }

  for ( MbSMBendNames * bendNames : resultBends )
    delete bendNames;
  resultBends.clear();

  SetWaitCursor( false );

  if ( resultSolid )
    TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, resultSolid );
  else
    ::PutErrorMessage( res, IDS_TITLE_ERR );
}

