////////////////////////////////////////////////////////////////////////////////
//
// Примеры использования функций работы с листовыми телами.
// Examples of using sheet metal operations.
//
////////////////////////////////////////////////////////////////////////////////

#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <test_draw.h>
#include <cur_arc.h>
#include <cur_contour.h>
#include <cur_contour3d.h>
#include <cur_line_segment3d.h>
#include <surf_cone_surface.h>
#include <surf_plane.h>
#include <creator.h>
#include <action_general.h>
#include <action_sheet.h>
#include <action_shell.h>
#include <action_solid.h>
#include <action_point.h>
#include <solid.h>
#include <plane_instance.h>
#include <space_instance.h>
#include <math_namespace.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Сгиб листового тела вдоль отрезка
// Sketched bend
// ---
void BendSheetSolidOverSegment() {

  // Запрос исходного тела и плоской грани
  MbItem * solidObj = nullptr;
  MbTopItem * faceTop = nullptr;
  if ( ::GetTObj(IDS_SELECT_PLANE_FACE, st_Solid, tt_Face, &solidObj, &faceTop) && solidObj && faceTop ) {
    MbFace * face = (MbFace *)faceTop;
    MbSolid * solid = (MbSolid *)solidObj;
    if ( face != nullptr && solid != nullptr ) {
      RPArray<MbFace> bendingFaces( 1, 1 );
      bendingFaces.Add( face );

      SPtr<MbCurve3D> curveToBendOver;
      // Запрос отрезка на плоской грани
      // Get a segment on a planar face
      MbItem * curveObj = nullptr;
      MbTopItem * edgeTop = nullptr;
      if ( ::GetMObj(IDS_SELECT_LNSEG_PLFACE, pt_Curve, &curveObj) && curveObj) {
        TestVariables::viewManager->ToggleObject( curveObj );
        const MbPlacement3D & placement = ((const MbPlaneInstance *)curveObj)->GetPlacement();
        const MbCurve * curve2D = static_cast<const MbCurve *>( ((const MbPlaneInstance *)curveObj)->GetPlaneItem() );
        MbCartPoint beg2D, end2D;
        curve2D->_PointOn( curve2D->GetTMin(), beg2D );
        curve2D->_PointOn( curve2D->GetTMax(), end2D );
        MbCartPoint3D beg3D, end3D;
        placement.PointOn( beg2D, beg3D );
        placement.PointOn( end2D, end3D );
        curveToBendOver = new MbLineSegment3D( beg3D, end3D );
      }
      else if ( ::GetTObj(IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &solidObj, &edgeTop) && edgeTop ) {
        const MbCurveEdge * curveEdge = static_cast<const MbCurveEdge *>(edgeTop);
        curveToBendOver = new MbLineSegment3D( curveEdge->GetBegVertex().GetCartPoint(),
                                               curveEdge->GetEndVertex().GetCartPoint() );
      }

      if ( curveToBendOver != nullptr ) {
        // Запрос угла гиба
        double angle = 90.0;
        if ( ::GetDouble(IDS_ENTER_ANGLE, angle) ) {
          angle *= Math::DEGRAD; // Перевод в радианы. Translate into radians

          SetWaitCursor( true );

          MbSNameMaker nameMaker( ct_BendOverSegSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
          MbBendOverSegValues params( 0.4/*k*/, 5.0/*radius*/, angle, 0.0/*coneAngle*/, 0.0, true, rt_Only );
          MbSolid * resultSolid = nullptr;
          MbResultType res = ::BendSheetSolidOverSegment( *solid, cm_Copy, bendingFaces, *(MbCurve3D *)curveToBendOver, false/*unbended*/, params, nameMaker, resultSolid );
          if ( resultSolid ) {
            Style ns( solidObj->GetWidth(), solidObj->GetColor() );
            TestVariables::viewManager->DeleteObject( solidObj );
            TestVariables::viewManager->AddObject( ns, resultSolid );
          }
          else
            ::PutErrorMessage( res, IDS_TITLE_ERR );

          SetWaitCursor( false );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Подсечка листового тела
// Jog
// ---
void SheetSolidJog() {
  static SimpleName operName = 0;

  // Запрос исходного тела и плоской грани
  // Get a solid body and a planar face
  MbItem *solidObj = nullptr;
  MbTopItem *faceTop = nullptr;
  if ( ::GetTObj(IDS_SELECT_PLANE_FACE, st_Solid, tt_Face, &solidObj, &faceTop) && solidObj && faceTop ) {
    MbFace *face = (MbFace *)faceTop;
    MbSolid *solid = (MbSolid *)solidObj;

    // Запрос отрезка на плоской грани
    // Get a segment of the planar face
    MbItem *curveObj;
    if ( ::GetMObj(IDS_SELECT_LNSEG_PLFACE, pt_Curve, &curveObj) && curveObj) {
      TestVariables::viewManager->ToggleObject( curveObj );
      const MbPlacement3D & placement = ((const MbPlaneInstance *)curveObj)->GetPlacement();
      const MbCurve *curve2D = static_cast<const MbCurve *>( ((const MbPlaneInstance *)curveObj)->GetPlaneItem() );
      MbCartPoint beg2D, end2D;
      curve2D->_PointOn( curve2D->GetTMin(), beg2D );
      curve2D->_PointOn( curve2D->GetTMax(), end2D );
      MbCartPoint3D beg3D, end3D;
      placement.PointOn( beg2D, beg3D );
      placement.PointOn( end2D, end3D );
      SPtr<MbCurve3D> curveToBendOver( new MbLineSegment3D(beg3D, end3D) );

      // Запрос угла гиба
      // Get an angle
      double angle = 90.0;
      if ( ::GetDouble(IDS_ENTER_ANGLE, angle) ) {
        angle *= Math::DEGRAD; // Перевод в радианы. Translate into radians

        // Запрос высоты подсечки
        // Get a height of the jog
        double elevation = 10.0;
        if ( ::GetDouble(IDS_ENTER_HEIGHT, elevation) ) {

          SetWaitCursor( true );

          if ( solid != nullptr ) {
            MbSNameMaker nameMaker( ++operName, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
            RPArray<MbFace> bendingFaces( 1, 1 );
            bendingFaces.Add( face );
            MbJogValues jogParams( 0.4/*k*/, 5.0/*radius*/, angle, 0.0, true, rt_Only, elevation, true/*addMaterial*/, true/*byInnerSide*/ );
            MbBendValues secondBendParams( 0.4/*k*/, 5.0/*radius*/, angle, 0.0/*coneAngle*/ );
            MbSolid *resultSolid = nullptr;
            RPArray<MbFace> firstBendFaces( 0, 1 ), secondBendFaces( 0, 1 );
            MbResultType res = ::SheetSolidJog( *solid, cm_Copy, bendingFaces, *(MbCurve3D *)curveToBendOver, false/*unbended*/,
                                                jogParams, secondBendParams, nameMaker, firstBendFaces, secondBendFaces, resultSolid );
            if ( resultSolid ) {
              Style ns( solidObj->GetWidth(), solidObj->GetColor() );
              TestVariables::viewManager->DeleteObject( solidObj );
              TestVariables::viewManager->AddObject( ns, resultSolid );
            }
            else
              ::PutErrorMessage( res, IDS_TITLE_ERR );
          }

          SetWaitCursor( false );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Сгиб любого тела
// Fold
// ---
void BendAnySolid() {
  MbItem * solidItem = nullptr;

  if ( GetGObj(IDS_SELECT_SOLID, st_Solid, &solidItem) && (solidItem != nullptr) ) {
    MbSolid * solid = (MbSolid *)solidItem;

    if ( solid != nullptr ) {
      SetWaitCursor( true );

      MbSNameMaker nameMaker( ct_BendAnySolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
      MbSolid * resultSolid = nullptr;

      MbCube gab;
      solid->CalculateGabarit( gab );
      MbVector3D axisZ( 1.0, 0.0, 0.0 );
      MbVector3D axisX( 0.0, 1.0, 0.0 );
      MbCartPoint3D origin;
      gab.GetFaceCentre( 0, origin );
      MbPlacement3D cutPlacement( origin, axisZ, axisX );
      MbPlane cutPlane( cutPlacement );

      SArray<MbAnyBend> bends;
      MbAnyBend bend;
      bend.wideness = 0.2 * ( gab.GetXMax() - gab.GetXMin() );
      double neutralRadius = 2.0 * ( gab.GetZMax() - gab.GetZMin() );

      bend.neutralRadius = neutralRadius;

      bend.origin.Init( 0.0, neutralRadius );
      bend.vector.Init( -1.0, 0.0 );
      bends.Add( bend );

      bend.origin.Init( 0.0, -neutralRadius );
      bend.vector.Init( 1.0, 0.0 );
      bends.Add( bend );

      bend.neutralRadius = 0.0;
      bend.origin.Init( 0.0, 0.0 );
      bend.vector.Init( 0.0, 0.0 );
      bends.Add( bend );

      bend.neutralRadius = neutralRadius;
      bend.origin.Init( 0.0, -neutralRadius );
      bend.vector.Init( 1.0, 0.0 );
      bends.Add( bend );

      bend.origin.Init( 0.0, neutralRadius );
      bend.vector.Init( -1.0, 0.0 );
      bends.Add( bend );

      //SArray<MbAnyBend> bends;
      //MbAnyBend bend;
      //MbCartPoint3D origin( -2048.7477653496103, 0.00000000000000000, 0.00000000000000000 );
      //MbVector3D axisX( 0.00000000000000000, 1.0000000000000000, 0.00000000000000000 );
      //MbVector3D axisZ( 1.0000000000000000, 0.00000000000000000, 0.00000000000000000 );
      //MbPlacement3D cutPlacement( origin, axisZ, axisX );
      //MbPlane cutPlane( cutPlacement );
      //bend.origin.Init( 0.00000000000000000, 0.00000000000000000 );
      //bend.vector.Init( 0.00000000000000000, 0.00000000000000000 );
      //bend.wideness = 961.00002807634394;
      //bend.neutralRadius = 0.00000000000000000;
      //bends.Add( bend );

      //bend.origin.Init( 0.00000000000000000, 267.00000000000000 );
      //bend.vector.Init( -1.0000000000000000, 0.00000000000000000 );
      //bend.wideness = 838.80522568885181;
      //bend.neutralRadius = 267.00000000000000;
      //bends.Add( bend );

      MbResultType res = ::BendAnySolid( *solid, cm_Copy, cutPlane, bends, nameMaker, resultSolid );
      if ( (resultSolid != nullptr) && resultSolid->GetShell() ) {
        Style ns( solidItem->GetWidth(), solidItem->GetColor() );
        TestVariables::viewManager->DeleteObject( solidItem );
        TestVariables::viewManager->AddObject( ns, resultSolid );
      }
      else
        ::PutErrorMessage( res, IDS_TITLE_ERR );

      SetWaitCursor( false );
    }
  }
}


//------------------------------------------------------------------------------
// Упрощение развёртки листового тела
// Simplify sheet metal flat pattern
// ---
void SimplifySheetPattern() {
  MbItem * solidItem = nullptr;

  if ( GetGObj(IDS_SELECT_SOLID, st_Solid, &solidItem) && (solidItem != nullptr) ) {
    MbSolid * solid = (MbSolid *)solidItem;

    if ( solid != nullptr ) {
      SetWaitCursor( true );

      MbSimplifyFlatPatternValues params;
      params._uniteFaces = false;

      SSArray<SimpleName> reliefsToRestore( 1, 1 );
      for ( size_t faceIndex = solid->GetFacesCount(); faceIndex--; ) {
        const MbFace * face = solid->GetFace( faceIndex );
        if ( (face != nullptr) && (face->GetSurface().GetSurface().IsA() == st_CylinderSurface) )
          reliefsToRestore.Add( face->GetMainName() );
      }

      params._keepCircularReliefs.reserve( reliefsToRestore.Count() );
      for ( size_t i = reliefsToRestore.Count(); i--; )
        params._keepCircularReliefs.push_back( reliefsToRestore[i] );

      MbSNameMaker nameMaker( ct_SimplifyFlatSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
      MbSolid * resultSolid = nullptr;

      MbResultType res = ::SimplifyFlatPattern( *solid, cm_Copy, params, nameMaker, resultSolid );
      if ( (resultSolid != nullptr) && resultSolid->GetShell() ) {
        Style ns( solidItem->GetWidth(), solidItem->GetColor() );
        TestVariables::viewManager->DeleteObject( solidItem );
        TestVariables::viewManager->AddObject( ns, resultSolid );
      }
      else
        ::PutErrorMessage( res, IDS_TITLE_ERR );

      SetWaitCursor( false );
    }
  }
}


//------------------------------------------------------------------------------
// Нормализация вырезов листового тела
// Normalization of holes in sheet solid
//---
void NormalizeSheetHoles() {
  MbItem * solidItem = nullptr;
  MbTopItem * faceItem = nullptr;

  if ( ::GetTObj(IDS_SELECT_FACE, st_Solid, tt_Face, &solidItem, &faceItem) && solidItem && faceItem ) {
    MbSolid * solid = static_cast<MbSolid*>( solidItem );
    MbFace * face = static_cast<MbFace*>( faceItem );  
    SetWaitCursor( true );
    MbSNameMaker nameMaker( ct_NormalizeHolesSolid, MbSNameMaker::i_SideNone, 0 );
    nameMaker.SetMainName( face->GetMainName() );
    MbSolid * resultSolid = nullptr;
    MbNormalizeCutSidesParams params( nameMaker, false );
    SolidSPtr resultS;
    MbResultType res = ::NormalizeCutSides( *solid, cm_Copy, params, resultS );
    resultSolid = resultS.detach();
    if ( (resultSolid != nullptr) && resultSolid->GetShell() ) {
      Style ns( solidItem->GetWidth(), solidItem->GetColor() );
      TestVariables::viewManager->DeleteObject( solidItem );
      TestVariables::viewManager->AddObject( ns, resultSolid );
    }
    else
      ::PutErrorMessage(res, IDS_TITLE_ERR);
    SetWaitCursor(false);
    
  }
}


//------------------------------------------------------------------------------
// Построение трёхмерных линий сгиба.
// Bends 3D axis lines construction.
// ---
void BuildBends3DAxisLines() {
  MbItem * solidItem = nullptr;

  if ( GetGObj(IDS_SELECT_SOLID, st_Solid, &solidItem) && (solidItem != nullptr) ) {
    MbSolid * solid = (MbSolid *)solidItem;

    if ( solid != nullptr ) {
      MbFaceShell * faceShell = solid->GetShell();
      if ( faceShell != nullptr ) {
        SetWaitCursor( true );

        ConstFacesSPtrVector bendFaces;
        for ( size_t faceIndex = faceShell->GetFacesCount(); faceIndex--; ) {
          MbFace * face = faceShell->_GetFace( faceIndex );
          if ( (face != nullptr) && (face->GetSurface().GetSurface().IsA() == st_Plane) && face->GetName().IsInnerBend() )
            bendFaces.push_back( ConstFaceSPtr(face) );
        }

        MbBends3DLinesResult result;
        if ( ::BuildBends3DCenterlines(MbBends3DLinesParams(bendFaces), result) ) {
          for ( size_t curveIndex = result.GetLines().size(); curveIndex--; ) {
            if ( result.GetLines()[curveIndex] != nullptr )
              TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, result.GetLines()[curveIndex] );
          }
        }
        else
          ::PutErrorMessage( rt_Error, IDS_TITLE_ERR );

        SetWaitCursor( false );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Сгиб тела из листового материала
// Fold
// ---
void BendSheetSolid() {

  PArray<MbSheetMetalBend> bends( 1, 1 );
  double innerRadius = 3.0;
  double coneAngle = 0.0;

  // Запрос листовых граней для сгиба
  // Get sheet faces for the fold
  RPArray<MbFace> innerFaces( 1, 1 );
  RPArray<MbFace> outerFaces( 1, 1 );
  MbItem * solidObj = nullptr;
  MbTopItem * innerFaceTop = nullptr, * outerFaceTop = nullptr;
  bool success = true;
  while ( success ) {
    success = false;
    innerFaces.DetachAll();
    outerFaces.DetachAll();
    while ( ::GetTObj(IDS_SELECT_INNER_FACE, st_Solid, tt_Face, &solidObj, &innerFaceTop) &&
            ::GetTObj(IDS_SELECT_OUTER_FACE, st_Solid, tt_Face, &solidObj, &outerFaceTop) &&
            solidObj != nullptr && innerFaceTop != nullptr && outerFaceTop != nullptr ) {
      innerFaces.Add( (MbFace *)innerFaceTop );
      outerFaces.Add( (MbFace *)outerFaceTop );
    }

    const size_t segmentsCount = innerFaces.Count();
    if ( segmentsCount > 0 ) {
      if ( segmentsCount == outerFaces.Count() &&
           ::GetDouble(IDS_ENTER_RADIUS, innerRadius) &&
           ::GetDouble(IDS_ENTER_HOLE_ANGLE, coneAngle) ) {
        coneAngle *= Math::DEGRAD;
        bends.Add( new MbSheetMetalBend(innerFaces, outerFaces, 0.4/*k*/, innerRadius, 0.0/*angle*/, /*0.74711174846604456*/coneAngle) );
        success = true;
      }
      else 
        bends.Flush();
    }
    //else {
    //  bends.Flush();
    //  if ( (solidObj != nullptr) && (solidObj->IsA() == st_Solid) ) {
    //    MbSolid * solid = (MbSolid *)solidObj;
    //    if ( solid->GetShell() != nullptr ) {
    //      solid->GetShell()->Reverse();
    //      viewManager->RedrawObject( solidObj );
    //    }
    //  }
    //}
  }

  // Запрос неподвижной грани
  // Get a fixed face
  MbTopItem *fixedFaceTop;
  if ( bends.Count() && ::GetTObj(IDS_SELECT_PLANE_FACE, st_Solid, tt_Face, &solidObj, &fixedFaceTop) && solidObj && fixedFaceTop ) {
    MbSolid *solid = (MbSolid *)solidObj;
    MbFace *fixedFace = (MbFace *)fixedFaceTop;

    // Запрос точки касания на неподвижной грани
    // Get a point on the fixed face that specifies a tangent plane to fold from
    double uParam = 0.0;
    if ( ::GetDouble(IDS_ENTER_PAR_ON_CURVE, uParam) ) {
      MbCartPoint surfaceParam;
      fixedFace->GetSurfaceParam( uParam, 0.5, surfaceParam.x, surfaceParam.y );
      MbLoop * outerLoop = fixedFace->_GetLoop( 0 );
      if ( outerLoop != nullptr ) {
        MbRegDuplicate * iReg = nullptr;
        MbAutoRegDuplicate autoReg( iReg );  
        SPtr<MbContour> contour( &outerLoop->MakeContour(fixedFace->GetSurface().GetSurface(), fixedFace->IsSameSense(), false/*doExact*/, iReg) );
        MbLine line( surfaceParam, M_PI_2 );
        SArray<double> tcross1;
        SArray<double> tcross2;
        if ( 1 < ::CurveCurveIntersection(line, *contour, tcross1, tcross2, PARAM_REGION, PARAM_REGION, false/*touchInclude*/, true/*allowInaccuracy*/) ) {
          tcross1.Sort();
          MbCartPoint p1, p2;
          line._PointOn( tcross1[0], p1 );
          line._PointOn( tcross1[1], p2 );
          surfaceParam.Set( p1, 0.5, p2, 0.5 );
        }
      }

      SetWaitCursor( true );

      if ( solid != nullptr ) {
        MbSNameMaker nameMaker( ct_BendUnbendSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
        MbSolid *resultSolid = nullptr;
        MbResultType res = ::BendSheetSolid( *solid, cm_Copy, bends, *fixedFace, surfaceParam, nameMaker, resultSolid );
        if ( resultSolid && resultSolid->GetShell() ) {
          Style ns( solidObj->GetWidth(), solidObj->GetColor() );
          TestVariables::viewManager->DeleteObject( solidObj );
          TestVariables::viewManager->AddObject( ns, resultSolid );
        }
        else
          ::PutErrorMessage( res, IDS_TITLE_ERR );
      }
      SetWaitCursor( false );
    }
  }
}


//------------------------------------------------------------------------------
// Выпрямление сгиба тела из листового материала
// Unfold
// ---
void UnbendSheetSolid() {

  PArray<MbSheetMetalBend> bends( 1, 1 );
  double innerRadius = 3.0;
  double coneAngle = 0.0;

  // Запрос листовых граней для сгиба
  // Get sheet faces for the unfolding
  RPArray<MbFace> innerFaces( 1, 1 );
  RPArray<MbFace> outerFaces( 1, 1 );
  MbItem * solidObj = nullptr;
  MbTopItem * innerFaceTop = nullptr, * outerFaceTop = nullptr;
  bool success = true;
  while ( success ) {
    success = false;
    innerFaces.DetachAll();
    outerFaces.DetachAll();
    while ( ::GetTObj(IDS_SELECT_INNER_FACE, st_Solid, tt_Face, &solidObj, &innerFaceTop) &&
            ::GetTObj(IDS_SELECT_OUTER_FACE, st_Solid, tt_Face, &solidObj, &outerFaceTop) &&
            solidObj != nullptr && innerFaceTop != nullptr && outerFaceTop != nullptr ) {
      innerFaces.Add( (MbFace *)innerFaceTop );
      outerFaces.Add( (MbFace *)outerFaceTop );
    }

    const size_t segmentsCount = innerFaces.Count();
    if ( segmentsCount > 0 ) {
      if ( segmentsCount == outerFaces.Count() &&
           ::GetDouble(IDS_ENTER_RADIUS, innerRadius) &&
           ::GetDouble(IDS_ENTER_HOLE_ANGLE, coneAngle) ) {
        coneAngle *= Math::DEGRAD;
        bends.Add( new MbSheetMetalBend(innerFaces, outerFaces, 0.4/*k*/, innerRadius, 0.0/*angle*/, /*0.74711174846604456*/coneAngle) );
        success = true;
      }
      else
        bends.Flush();
    }
  }

  // Запрос неподвижной грани
  // Get a fixed face
  MbTopItem *fixedFaceTop;
  if ( bends.Count() && ::GetTObj(IDS_SELECT_PLANE_FACE, st_Solid, tt_Face, &solidObj, &fixedFaceTop) && solidObj && fixedFaceTop ) {
    MbSolid *solid = (MbSolid *)solidObj;
    MbFace *fixedFace = (MbFace *)fixedFaceTop;

    // Запрос точки касания на неподвижной грани
    // Get a point on the fixed face that specifies a tnagent plane to unfold on
    double uParam = 0.0;
    if ( ::GetDouble(IDS_ENTER_PAR_ON_CURVE, uParam) ) {
      MbCartPoint surfaceParam;
      fixedFace->GetSurfaceParam( uParam, 0.5, surfaceParam.x, surfaceParam.y );

      SetWaitCursor( true );

      if ( solid != nullptr ) {
        MbSNameMaker nameMaker( ct_BendUnbendSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
        MbSolid *resultSolid = nullptr;
        RPArray<MbContour3D> ribContours;
        MbResultType res = ::UnbendSheetSolid( *solid, cm_Copy, bends, *fixedFace, surfaceParam, nameMaker, resultSolid, &ribContours );
        if ( resultSolid && resultSolid->GetShell() ) {
          Style ns( solidObj->GetWidth(), solidObj->GetColor() );
          TestVariables::viewManager->DeleteObject( solidObj );
          TestVariables::viewManager->AddObject( ns, resultSolid );
          if ( ribContours.Count() > 0 ) {
            for ( size_t i = ribContours.Count(); i--; ) {
              TestVariables::viewManager->AddObject( ns, ribContours[i] );
            }
          }
        }
        else
          ::PutErrorMessage( res, IDS_TITLE_ERR );
      }

      SetWaitCursor( false );
    }
  }
}


//------------------------------------------------------------------------------
// Выпрямление всех сгибов тела из листового материала.
// Unbend all bends.
// ---
void UnbendAllBends() {
  PArray<MbSheetMetalBend> bends( 1, 1 );
  MbItem * solidObj = nullptr;
  // Запрос неподвижной грани
  // Get a fixed face
  MbTopItem * fixedFaceTop = nullptr;
  if ( ::GetTObj(IDS_SELECT_PLANE_FACE, st_Solid, tt_Face, &solidObj, &fixedFaceTop) &&
       solidObj != nullptr &&
       fixedFaceTop != nullptr ) {
    MbSolid * solid = static_cast< MbSolid * >( solidObj );
    MbFace * fixedFace = static_cast< MbFace *>( fixedFaceTop );

    // Запрос точки касания на неподвижной грани
    // Get a point on the fixed face that specifies a tnagent plane to unfold on
    double uParam = 0.0;
    if ( ::GetDouble(IDS_ENTER_PAR_ON_CURVE, uParam) ) {
      MbCartPoint surfaceParam;
      fixedFace->GetSurfaceParam( uParam, 0.5, surfaceParam.x, surfaceParam.y );

      SetWaitCursor( true );

      for ( size_t fInd = 0, fCount = solid->GetFacesCount(); fInd < fCount; fInd++ ) {
        MbFace * curFace = solid->GetFace( fInd );
        if ( curFace != nullptr  && curFace->GetName().IsInnerBend() &&
             (curFace->GetSurface().GetSurface().IsA() == st_CylinderSurface || curFace->GetSurface().GetSurface().IsA() == st_ConeSurface) ) {
          MbFace * outerFace = FindPairBendFace( *curFace );
          if ( outerFace != nullptr ) {
            double rad = curFace->GetSurface().GetRadius();
            double coneAngle = curFace->GetSurface().GetSurface().IsA() != st_ConeSurface ? 0.0 : static_cast<const MbConeSurface &>(curFace->GetSurface().GetSurface()).GetAngle();
            bends.Add( new MbSheetMetalBend(curFace, outerFace, 0.4/*k*/, rad, 0.0/*angle*/, coneAngle) );
          }
        }
      }
      if ( bends.Count() > 0 ) {
        MbSNameMaker nameMaker( ct_BendUnbendSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
        MbSolid *resultSolid = nullptr;
        RPArray<MbContour3D> ribContours;
        MbResultType res = ::UnbendSheetSolid( *solid, cm_Copy, bends, *fixedFace, surfaceParam, nameMaker, resultSolid, &ribContours );
        if ( resultSolid != nullptr && resultSolid->GetShell() != nullptr ) {
          Style ns( solidObj->GetWidth(), solidObj->GetColor() );
          TestVariables::viewManager->DeleteObject( solidObj );
          TestVariables::viewManager->AddObject( ns, resultSolid );
          if ( ribContours.Count() > 0 ) {
            for ( size_t i = ribContours.Count(); i--; ) {
              TestVariables::viewManager->AddObject( ns, ribContours[i] );
            }
          }
        }
        else
          ::PutErrorMessage( res, IDS_TITLE_ERR );
      }

      SetWaitCursor( false );
    }
  }
}


//------------------------------------------------------------------------------
// Создание тела из листового материала, 
// Create a sheet solid body
// Операция "Пластина" тела из листового материала, 
// Sheet metal tab
// Операция "Отверстие" или "Вырез" тела из листового материала 
// Sheet metal hole or cut
// ---
void CreateSheetSolid( int type ) {
  MbItem  * curve;
  MbItem  * curve0 = nullptr;
  double   depth = 50;
  double   thickness_ = 3.0;
  double   radius = 5;
  MbSheetMetalValues param;
  MbSolid * resultSolid = nullptr;
  PArray <MbContour> c(2,2,true);
  MbPlacement3D pl;
  Style ns = TestVariables::SWEPT_Style;

  while ( GetMObj(IDS_SELECT_GENER_CURVE, pt_Curve, &curve) ) {
    if ( curve0 != nullptr ) {
      TestVariables::viewManager->ToggleObject( curve0 );
      if ( !pl.Colinear(((MbPlaneInstance *)curve)->GetPlacement()) || curve==curve0 )
        break;
    }
    else {
      pl.Init(((MbPlaneInstance *)curve)->GetPlacement());
    }
    TestVariables::viewManager->ToggleObject(curve);
    MbPlaneInstance* planeInst = (MbPlaneInstance*)curve;
    MbContour* addCrv = nullptr;
    for ( size_t i = 0, itemSize = planeInst->PlaneItemsCount(); i < itemSize; ++i ) {
      const MbPlaneItem * planeItem = ( static_cast<MbPlaneInstance*>(curve) )->GetPlaneItem( i );
      if ( planeItem != nullptr ) {
        if ( planeItem->Type() == pt_Contour ) {
          const MbContour * crv = static_cast<const MbContour*>( planeItem );
          c.Add( static_cast<MbContour*>(&crv->Duplicate()) );
        }
        else {
          if ( addCrv == nullptr )
            addCrv = new MbContour();
          addCrv->AddSegment( static_cast<MbCurve*>(&planeItem->Duplicate()) );
        }
      }    
    }
    if ( addCrv != nullptr )
        c.Add( addCrv );
    curve0 = curve;
  }
  if ( curve0 != nullptr ) 
    TestVariables::viewManager->ToggleObject( curve0 );

  if ( c.Count() > 0 ) {
    PArray <MbSNameMaker> names( 0, 1, true );
    names.Add( new MbSNameMaker(1, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/) );

    switch (type) {
      case  1 : // Объединение. Union.
      case -1 : // Вычитание. Difference.
      case -2 : // Пересечение. Intersection.
      {
        MbItem  *obj;
        MbTopItem *face;
        MbSolid *solid = nullptr;
        int cs = 0;

        if ( GetTObj(IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &face) &&
            ((type<0) ? GetInt(IDS_ENTER_BUILD_TYPE, cs) : GetDouble(IDS_ENTER_THICKNESS, thickness_)) ) {
          ns.Init( obj->GetWidth(), obj->GetColor() );
          solid = (MbSolid *)obj;
          TestVariables::viewManager->ToggleObject(obj);
          switch ( cs ) {
            case 0 : {
              param.side2.way = sw_scalarValue;
              param.side2.scalarValue = thickness_;
              param.side1.scalarValue = 0.0;
            } break;
            case 1 : {
              param.side1.way = sw_shell;
              GetDouble( IDS_ENTER_STRETCHING, depth );
              param.side1.scalarValue = depth;
            } break;
            case 2 : {
              param.side1.way = sw_surface;
              param.SetSurface1( &((MbFace *)face)->SetSurface() );
//#ifdef _DRAWGI
              TestVariables::drawImp->DrawItem( &((MbFace *)face)->GetSurface(), TRGB_WHITE );
//#endif // _DRAWGI
            } break;
          }
          MbResultType res = rt_Error;
          if ( solid!=nullptr ) {
            SetWaitCursor( true );
            if (type==1)
              res = SheetSolidPlate ( *solid, cm_Copy, pl, c, param, &names, resultSolid );
            else 
              res = SheetSolidHole  ( *solid, cm_Copy, pl, c, param, type==-1, &names, resultSolid );
            SetWaitCursor( false );
          }
          TestVariables::viewManager->ToggleObject(obj);

          if ( resultSolid==nullptr )
            ::PutErrorMessage( res, IDS_TITLE_ERR );
          else {
            TestVariables::viewManager->DeleteObject( obj );
          }
        }
      } break;

      default : {
        bool b = false;
        if ( c[0]->IsClosed() ) {
          if ( GetDouble(IDS_ENTER_THICKNESS, thickness_) ) {
            param.side1.scalarValue = thickness_;
            param.side2.scalarValue = 0;
            b = true;
          }
        }
        else {
          if ( GetDouble(IDS_ENTER_RADIUS, thickness_) && GetDouble(IDS_ENTER_RADIUS, radius) && GetDouble(IDS_ENTER_STRETCHING, depth) ) {
            param.k = 0.5;
            param.thickness = thickness_;
            param.radius = radius;
            param.side1.scalarValue = depth;
            if ( GetDouble(IDS_ENTER_STRETCHING, depth) )
              param.side2.scalarValue = depth;
            b = true;
          }
        }
        if ( b ) {
          SetWaitCursor( true );
          // Заполнение параметров сгибов
          // Fill bends parameters
          PArray<MbSMBendNames> resultBends( 0, 1, true );
          MbContour *contour = c[0];
          bool segFound = false;
          for ( size_t i = contour->GetSegmentsCount(); i--; ) {
            const MbCurve * curve_ = contour->GetSegment( i );
            if ( curve_ ) {
              uint curveType = curve_->IsA();
              if ( curveType == pt_Arc ) {
                resultBends.Add( new MbSMBendNames(0.5/*coef*/, curve_->GetRadius()/*rad*/, 90.0/*ang*/, SimpleName(i), SimpleName(SIMPLENAME_MAX) ) );
                segFound = false;
              }
              else if ( curveType == pt_LineSegment ) {
                if ( segFound ) {
                  resultBends.Add( new MbSMBendNames(0.5/*coef*/, radius/*rad*/, 90.0/*ang*/, SimpleName(i), SimpleName(i+1)) );
                }
                segFound = true;
              }
              else
                segFound = false;
            }
          }

          MbResultType res = CreateSheetSolid( pl, c, false, param, &names, resultBends, resultSolid );
          SetWaitCursor( false );

          if ( resultSolid==nullptr )
            ::PutErrorMessage( res, IDS_TITLE_ERR );
        }
      } break;

    }
  }

  if ( resultSolid != nullptr )
    TestVariables::viewManager->AddObject( ns, resultSolid );
}


//------------------------------------------------------------------------------
// Сгиб по ребру тела из листового материала
// An edge flange
// ---
void BendSheetSolidByEdges() {

  // Запрос исходного тела и ребра для сгиба
  // Get a sheet solid body and an edge for a flange
  MbItem *solidObj;
  MbTopItem *edgeTop;
  ConstEdgesVector edgesArray;
  SPtr<MbSolid> solid = nullptr;
  while( ::GetTObj(IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &solidObj, &edgeTop) && solidObj && edgeTop ) {
    MbSolid *sld = (MbSolid *)solidObj;
    if ( solid && solid != sld ) {
      solid = nullptr;
      break;
    }
    else
      solid = sld;

    MbCurveEdge *curveEdge = (MbCurveEdge *)edgeTop;
    std::vector<const MbCurveEdge *>::iterator edgeIndex = std::find( edgesArray.begin(), edgesArray.end(), curveEdge );
    if ( edgeIndex != edgesArray.end() )
      edgesArray.erase( edgeIndex );
    else
      edgesArray.push_back( curveEdge );
  }

  if ( solid != nullptr ) {
    // Запрос угла гиба
    // Get a bend angle
    double angle = 90.0;
    if ( ::GetDouble(IDS_ENTER_ANGLE, angle) ) {
      angle *= Math::DEGRAD; // Перевод в радианы. Translate into radians

      SetWaitCursor( true );

      MbSNameMaker nameMaker( ct_BendsByEdgesSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
      MbBendByEdgeValues values( 0.4/*k*/, 5.0/*radius*/, angle, 0.0/*coneAngle*/, 30.0, 0.0/*deepness*/, rt_No );
      values.SideLeftInit( 0.0, 0.0 * Math::DEGRAD, 0.0 * Math::DEGRAD, 0.0 );
      values.SideRightInit( 0.0, 0.0 * Math::DEGRAD, 0.0 * Math::DEGRAD, 0.0 );
      values.SlotInit( 0.0/*width*/, 0.0/*depth*/, 2.0/*radius*/ );
      values.miterMiddle.allow = true;
      values.miterMiddle.cornerWay = MbClosedCornerValues::ccTight;
      values.miterMiddle.gap = 0.5;
      values.miterMiddle.bendsWay.push_back( MbClosedCornerValues::cbEdge );
      std::vector<MbSMBendNames *> bendsParams;
      MbBendSheetSolidByEdgesParams params( edgesArray, false/*unbended*/, values, nameMaker );
      SPtr<MbSolid> resultSolid = nullptr;
      MbResultType res = ::BendSheetSolidByEdges( solid, cm_Copy, params, bendsParams, resultSolid );
      for ( MbSMBendNames * bendNames : bendsParams )
        delete bendNames;
      bendsParams.clear();
      if ( resultSolid != nullptr ) {
        Style ns( solidObj->GetWidth(), solidObj->GetColor() );
        TestVariables::viewManager->DeleteObject( solidObj );
        TestVariables::viewManager->AddObject( ns, resultSolid );
      }
      else
        ::PutErrorMessage( res, IDS_TITLE_ERR );

      SetWaitCursor( false );
    }
  }
}


//------------------------------------------------------------------------------
// Комбинированный сгиб (сгиб по эскизу)
// A miter flange
// ---
void SheetSolidJointBend() {
  MbItem * curve = nullptr; // Образующая. Generatrix curve
  if ( ::GetMObj(IDS_SELECT_GENER_CURVE, pt_Curve, &curve) && curve ) {
    TestVariables::viewManager->ToggleObject( curve );
    SPtr<MbContour> contour( nullptr );
    MbPlacement3D placement( ((const MbPlaneInstance *)curve)->GetPlacement() );
    const MbPlaneItem * planeItem = ((const MbPlaneInstance *)curve)->GetPlaneItem();
    if ( planeItem != nullptr ) {
      if ( planeItem->Type() == pt_Contour ) {
        const MbContour * crv = static_cast<const MbContour *>( planeItem );
        contour = static_cast<MbContour *>( &crv->Duplicate() );
      }
      else {
        contour = new MbContour();
        contour->AddSegment( static_cast<MbCurve *>( &planeItem->Duplicate() ) );
      }
    }
    // Запрос исходного тела и ребра для сгиба
    // Get a sheet solid and an edge for a flange
    MbItem * solidObj = nullptr;
    MbTopItem * edgeTop = nullptr;
    ConstEdgesVector edgesArray;
    SPtr<MbSolid> solid( nullptr );
    while( ::GetTObj(IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &solidObj, &edgeTop) && solidObj && edgeTop ) {
      MbSolid *sld = (MbSolid *)solidObj;
      if ( solid && solid != sld ) {
        solid = nullptr;
        break;
      }
      else
        solid = sld;

      MbCurveEdge *curveEdge = (MbCurveEdge *)edgeTop;
      std::vector<const MbCurveEdge *>::iterator edgeIndex = std::find( edgesArray.begin(), edgesArray.end(), curveEdge );
      if ( edgeIndex != edgesArray.end() )
        edgesArray.erase( edgeIndex );
      else
        edgesArray.push_back( curveEdge );
    }

    if ( solid != nullptr ) {
      SetWaitCursor( true );

      MbSNameMaker nameMaker( ct_JointBendSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
      std::vector<std::vector<MbSMBendNames *>> resultBends;
      SPtr<MbSolid> resultSolid( nullptr );
      MbJointBendValues params( 0.4/*k*/,
                                5.0/*bendRadius*/,
                                0.0/*distBeg*/,
                                0.0/*distEnd*/,
                                true/*plus*/,
                                MbClosedCornerValues::ccRip/*CloseCornerWay begin*/,
                                MbClosedCornerValues::cbChord/*CloseBendsWay begin*/,
                                MbClosedCornerValues::cpBend/*CloseBendsCirclePos begin*/,
                                0.0/*gap begin*/,
                                0.0/*diameter begin*/,
                                0.0/*shift begin*/,
                                M_PI_4/*angle begin*/,
                                false/*allow begin*/,
                                false/*acrossBend begin*/,
                                MbClosedCornerValues::ccRip/*CloseCornerWay end*/,
                                MbClosedCornerValues::cbChord/*CloseBendsWay end*/,
                                MbClosedCornerValues::cpBend/*CloseBendsCirclePos end*/,
                                0.0/*gap end*/,
                                0.0/*diameter end*/,
                                0.0/*shift end*/,
                                M_PI_4/*angle end*/,
                                false/*allow end*/,
                                false/*acrossBend end*/,
                                MbClosedCornerValues::ccRip/*CloseCornerWay middle*/,
                                MbClosedCornerValues::cbChord/*CloseBendsWay middle*/,
                                MbClosedCornerValues::cpBend/*CloseBendsCirclePos middle*/,
                                0.0/*gap middle*/,
                                0.0/*diameter middle*/,
                                0.0/*shift middle*/,
                                false/*allow middle*/,
                                2.0/*width*/,
                                3.0/*depth*/,
                                1.0/*relRad*/,
                                rt_Only/*ReleaseType*/ );
      MbSheetSolidJointBendParams parameters( placement, *contour, edgesArray, false/*unbended*/, params, nameMaker );
      MbResultType res = ::SheetSolidJointBend( solid,        // Исходное тело. An initial solid
                                                cm_Copy,       // Флаг удаления оболочки исходного тела. Whether to delete the shell of the source solid.
                                                parameters,    // Параметры сгиба. Parameters of the flange.
                                                resultBends,   // Формируемые сгибы. Created bends.
                                                resultSolid ); // Результирующее тело. The result solid.
      if ( resultSolid != nullptr ) {
        Style ns( solidObj->GetWidth(), solidObj->GetColor() );
        TestVariables::viewManager->DeleteObject( solidObj );
        TestVariables::viewManager->AddObject( ns, resultSolid );
      }
      else
        ::PutErrorMessage( res, IDS_TITLE_ERR );

      SetWaitCursor( false );
    }
    TestVariables::viewManager->ToggleObject( curve );
  }
}


//------------------------------------------------------------------------------
// Замыкание сгиба
// A closed corner
// ---
void CloseCorner() {

  // Запрос исходного тела и ребра для замыкания
  // Get a sheet solid and corners for closing
  MbItem *solidObj = nullptr;
  MbTopItem *topItem = nullptr;
  if ( ::GetTObj(IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &solidObj, &topItem) && solidObj && topItem ) {
    MbSolid *solid = (MbSolid *)solidObj;
    MbCurveEdge *edge1 = (MbCurveEdge *)topItem;
    if ( solid && edge1 ) {
      MbCurveEdge *edge2 = nullptr;
      MbFace *sideFace = nullptr;
      topItem = nullptr;
      if ( ::GetTObj(IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &solidObj, &topItem) && solidObj && topItem )
        edge2 = (MbCurveEdge *)topItem;
      else if ( ::GetTObj(IDS_SELECT_FACE, st_Solid, tt_Face, &solidObj, &topItem) && solidObj && topItem )
        sideFace = (MbFace *)topItem;

      MbCurveEdge *edgePlus = nullptr, *edgeMinus = nullptr;
      if ( edge2 ) {
        edgePlus = edge1;
        edgeMinus = edge2;
      }
      else if ( edge1->GetFacePlus() == sideFace )
        edgePlus = edge1;
      else if ( edge1->GetFaceMinus() == sideFace )
        edgeMinus = edge1;

      if ( edgePlus || edgeMinus ) {
        // Запрос величины зазора
        // Get gap value
        double gap = 0.0;
        if ( ::GetDouble(IDS_ENTER_DISTANCE, gap) ) {

          SetWaitCursor( true );

          MbSNameMaker nameMaker( ct_ClosedCornerSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
          MbSolid *resultSolid = nullptr;
          MbClosedCornerValues params( MbClosedCornerValues::ccTight,
                                       MbClosedCornerValues::cbNone,
                                       MbClosedCornerValues::cpBend,
                                       gap,
                                       0.0/*diameter*/,
                                       0.0/*shift*/,
                                       0.4/*kPlus*/,
                                       0.4/*kMinus*/,
                                       M_PI_4/*angle*/,
                                       true/*plus*/,
                                       false/*prolong*/,
                                       false/*acrossBend*/ );
          MbResultType res = ::CloseCorner( *solid, cm_Copy, edgePlus, edgeMinus, params, nameMaker, resultSolid );
          if ( resultSolid ) {
            Style ns( solidObj->GetWidth(), solidObj->GetColor() );
            TestVariables::viewManager->DeleteObject( solidObj );
            TestVariables::viewManager->AddObject( ns, resultSolid );
          }
          else
            ::PutErrorMessage( res, IDS_TITLE_ERR );

          SetWaitCursor( false );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Штамповка листового тела
// Stamp
// ---
void SheetSolidStamp() {
  // Запрос исходного тела и плоской грани
  // Get a sheet solid and its planar face
  MbItem *solidObj = nullptr;
  MbTopItem *faceTop = nullptr;
  if ( ::GetTObj(IDS_SELECT_PLANE_FACE, st_Solid, tt_Face, &solidObj, &faceTop) && solidObj && faceTop ) {
    SolidSPtr solid( static_cast<MbSolid *>(solidObj) );
    FaceSPtr face( static_cast<MbFace *>(faceTop) );

    // Запрос контура на плоской грани
    // Get a contour on the planar face
    MbItem *curveObj;
    if ( ::GetMObj(IDS_SELECT_GENER_CURVE, pt_Curve, &curveObj) && curveObj ) {
      TestVariables::viewManager->ToggleObject( curveObj );
      SPtr<MbContour> contour;
      MbPlaneItem * planeItem = ((MbPlaneInstance *)curveObj)->SetPlaneItem();
      if ( planeItem != nullptr ) {
        if ( planeItem->Type() == pt_Contour )
          contour = static_cast<MbContour *>(planeItem);
        else {
          contour = new MbContour();
          contour->AddSegment( static_cast<MbCurve *>(planeItem) );
        }
      }
      const MbPlacement3D &placement = ((MbPlaneInstance *)curveObj)->GetPlacement();

      // Запрос угла подъёма штамповки
      // Get a slope angle
      double angle = 0.0;
      if ( ::GetDouble(IDS_ENTER_ANGLE, angle) ) {
        angle *= Math::DEGRAD; // Перевод в радианы. Translate into radians

        // Запрос высоты штамповки
        // Get the stamp height
        double elevation = 30.0;
        if ( ::GetDouble(IDS_ENTER_HEIGHT, elevation) ) {

          int open = 0;
          if ( ::GetInt(IDS_ENTER_STAMP_TYPE, open) ) {
            SetWaitCursor( true );

            if ( solid && face && contour ) {
              MbSNameMaker nameMaker( ct_StampSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
              MbStampingValues params( elevation, angle, -5.0/*sketchFilletRadius*/, -2.0/*baseFilletRadius*/,
                                       -2.0/*bottomFilletRadius*/, true/*wallInside*/,
                                       true/*leftFixed*/, false/*reverse*/, open == 1 );
              SolidSPtr resultSolid;
              MbStampParams parameters( face, placement, contour, params, nameMaker );
              MbResultType res = ::Stamp( solid, cm_Copy, parameters, resultSolid );
              if ( resultSolid != nullptr) {
                Style ns( solidObj->GetWidth(), solidObj->GetColor() );
                TestVariables::viewManager->DeleteObject( solidObj );
                TestVariables::viewManager->AddObject( ns, resultSolid );
              }
              else
                ::PutErrorMessage( res, IDS_TITLE_ERR );
            }

            SetWaitCursor( false );
          }
        }
      }
    }
    else { // Сферическая штамповка. Spherical stamp.
      // Запрос угла подъёма штамповки
      // Get a slope angle
      double angle = 0.0;
      if ( ::GetDouble(IDS_ENTER_ANGLE, angle) ) {
        angle *= Math::DEGRAD; // Перевод в радианы. Translate into radians

        // Запрос высоты штамповки
        // Get the stamp height
        double elevation = 30.0;
        if ( ::GetDouble(IDS_ENTER_HEIGHT, elevation) ) {
          SetWaitCursor( true );

          if ( solid && face ) {
            MbSNameMaker nameMaker( ct_StampSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
            MbStampingValues params( elevation, angle, -5.0/*sketchFilletRadius*/, -2.0/*baseFilletRadius*/,
                                      20.0/*bottomFilletRadius*/, false/*wallInside*/,
                                      false/*leftFixed*/, false/*reverse*/, false/*open*/ );
            PRECONDITION( face->GetSurface().GetSurface().IsA() == st_Plane );
            const MbSurface & boundedSurface = face->GetSurface();
            MbCartPoint center( 0.5 * (boundedSurface.GetUMin() + boundedSurface.GetUMax()), 0.5 * (boundedSurface.GetVMin() + boundedSurface.GetVMax()) );
            const MbPlacement3D & placement = static_cast<const MbPlane &>( boundedSurface.GetSurface() ).GetPlacement();

            SolidSPtr resSol;
            MbSphericalStampParams sphericalStampParams( face, placement, center, params, nameMaker );
            MbResultType res = ::SphericalStamp( solid, cm_Copy, sphericalStampParams, resSol );
            if ( resSol != nullptr ) {
              Style ns( solidObj->GetWidth(), solidObj->GetColor() );
              TestVariables::viewManager->DeleteObject( solidObj );
              TestVariables::viewManager->AddObject( ns, resSol );
            }
            else
              ::PutErrorMessage( res, IDS_TITLE_ERR );
          }

          SetWaitCursor( false );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Буртик
// Bead
// ---
void SheetSolidBead() {
  // Запрос исходного тела и плоской грани
  // Get a sheet solid and its planar face
  MbItem *solidObj = nullptr;
  MbTopItem *faceTop = nullptr;
  if ( ::GetTObj(IDS_SELECT_PLANE_FACE, st_Solid, tt_Face, &solidObj, &faceTop) && solidObj && faceTop ) {
    MbFace *face = (MbFace *)faceTop;
    MbSolid *solid = (MbSolid *)solidObj;

    // Запрос контура на плоской грани
    // Get a contour on the planar face
    MbItem *curveObj = nullptr;
    PArray<MbContour> contours( 1, 1, true );
    while ( ::GetMObj(IDS_SELECT_GENER_CURVE, pt_Curve, &curveObj) && curveObj ) {
      TestVariables::viewManager->ToggleObject( curveObj );
      MbPlaneItem * planeItem = ((MbPlaneInstance *)curveObj)->SetPlaneItem();
      if ( planeItem != nullptr ) {
        if ( planeItem->Type() == pt_Contour ) 
          contours.Add( static_cast<MbContour *>( &planeItem->Duplicate() ) );
        else {
          MbContour * contour = new MbContour();
          contour->AddSegment( static_cast<MbCurve *>(planeItem) );
          contours.Add( contour );
        }
      }
    }

    if ( contours.Count() && curveObj ) {
      const MbPlacement3D &placement = ((MbPlaneInstance *)curveObj)->GetPlacement();

      // Запрос угла
      // Get a solid angle
      double angle = 20.0;
      if ( ::GetDouble(IDS_ENTER_ANGLE, angle) ) {
        angle *= Math::DEGRAD; // Перевод в радианы

        // Запрос высоты
        // Get the height
        double elevation = 5.0;
        if ( ::GetDouble(IDS_ENTER_HEIGHT, elevation) ) {

          int type = 0;
          if ( ::GetInt(IDS_ENTER_BEAD_TYPE, type) ) {
            SetWaitCursor( true );

            if ( solid && face ) {
              MbBeadValues params( elevation,
                                   5.0,   // bottomWidth
                                   1.0,   // baseRadius
                                   2.0,   // bottomRadius
                                   angle,
                                   2.0,   // gap
                                   true,  // reverse
                                   (MbBeadValues::MbeBeadType)type,
                                   MbBeadValues::betClosed );
              MbSolid *resultSolid = nullptr;
              SArray<MbCartPoint> centers;
              MbSNameMaker nameMaker( ct_BeadSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
              MbResultType res = ::CreateBead( *solid, cm_Copy, *face, placement, contours,
                                                centers, params, nameMaker, resultSolid );
              if ( resultSolid ) {
                Style ns( solidObj->GetWidth(), solidObj->GetColor() );
                TestVariables::viewManager->DeleteObject( solidObj );
                TestVariables::viewManager->AddObject( ns, resultSolid );
              }
              else
                ::PutErrorMessage( res, IDS_TITLE_ERR );
            }

            SetWaitCursor( false );
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Жалюзи
// A louver
// ---
void SheetSolidJalousie() {
  // Запрос исходного тела и плоской грани
  // Get a sheet solid and its planar face
  MbItem *solidObj = nullptr;
  MbTopItem *faceTop = nullptr;
  if ( ::GetTObj(IDS_SELECT_PLANE_FACE, st_Solid, tt_Face, &solidObj, &faceTop) && solidObj && faceTop ) {
    MbFace *face = (MbFace *)faceTop;
    MbSolid *solid = (MbSolid *)solidObj;

    // Запрос отрезка на плоской грани
    // Get a segment on the planar face
    MbItem *curveObj = nullptr;
    RPArray<MbLineSegment> segments( 1, 1 );
    while ( ::GetMObj(IDS_SELECT_LNSEG_PLFACE, pt_LineSegment, &curveObj) &&
            curveObj &&
            ((MbPlaneInstance *)curveObj)->GetPlaneItem()->IsA() == pt_LineSegment ) {
      MbLineSegment *lineSegment = (MbLineSegment *)((MbPlaneInstance *)curveObj)->GetPlaneItem();
      if ( lineSegment ) {
        segments.Add( lineSegment );
        TestVariables::viewManager->ToggleObject( curveObj );
      }
    }

    if ( segments.Count() && curveObj ) {
      const MbPlacement3D &placement = ((MbPlaneInstance *)curveObj)->GetPlacement();

      // Запрос высоты
      // Get the height
      double elevation = 5.0;
      if ( ::GetDouble(IDS_ENTER_HEIGHT, elevation) ) {

        SetWaitCursor( true );

        MbJalousieValues params( 10.0,      // Ширина. Width.
                                 elevation, // Высота. Height.
                                 2.0,       // Радиус скругления. Fillet radius.
                                 0.4,       // Коэффициент нейтрального слоя. Coefficient of neutral layer.
                                 false,     // Направление построения жалюзи. Direction of stamping
                                 false,     // Сторона отрезка, по которой строятся жалюзи. Side ot the segment to stamp
                                 true,      // Вытяжка. Stretch
                                 false );   // По нормали к толщине. Normal to the thickness
        MbSolid *resultSolid = nullptr;
        MbSNameMaker nameMaker( ct_JalousieSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
        MbResultType res = ::CreateJalousie( *solid, cm_Copy, *face, placement, segments,
                                              params, nameMaker, resultSolid );
        if ( resultSolid ) {
          Style ns( solidObj->GetWidth(), solidObj->GetColor() );
          TestVariables::viewManager->DeleteObject( solidObj );
          TestVariables::viewManager->AddObject( ns, resultSolid );
        }
        else
          ::PutErrorMessage( res, IDS_TITLE_ERR );

        SetWaitCursor( false );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание обечайки
// Lofted bends
// ---
void CreateRuledSolid() {
  MbItem * curve = nullptr; // Образующая. Generatrix curve.

  if ( ::GetMObj(IDS_SELECT_GENER_CURVE, pt_Curve, &curve) && curve ) {
    TestVariables::viewManager->ToggleObject( curve );
    SPtr<MbContour> contour1;
    MbPlacement3D placement1( ((MbPlaneInstance *)curve)->GetPlacement() );
    const MbPlaneItem * planeItem1 = ((MbPlaneInstance *)curve)->GetPlaneItem();
    if ( planeItem1 != nullptr ) {
      if ( planeItem1->Type() == pt_Contour )
        contour1 = static_cast<MbContour *>( &planeItem1->Duplicate() );
      else {
        contour1 = new MbContour();
        contour1->AddSegment( static_cast<MbCurve *>( &planeItem1->Duplicate() ) );
      }
    }
    if ( contour1 != nullptr ) {
      SPtr<MbContour> contour2;
      MbPlacement3D placement2;
      if ( ::GetMObj(IDS_SELECT_GENER_CURVE, pt_Curve, &curve) && curve ) {
        TestVariables::viewManager->ToggleObject( curve );
        placement2.Init( ((const MbPlaneInstance *)curve)->GetPlacement() );
        const MbPlaneItem * planeItem2 = ((const MbPlaneInstance *)curve)->GetPlaneItem();
        if ( planeItem2 != nullptr ) {
          if ( planeItem2->Type() == pt_Contour )
            contour2 = static_cast<MbContour *>( &planeItem2->Duplicate() );
          else {
            contour2 = new MbContour();
            contour2->AddSegment( static_cast<MbCurve *>( &planeItem2->Duplicate() ) );
          }
        }
      }

      double radius = 10.0;
      double thickness_ = 1.0;
      int segmentationNumber = 0;
      double gapValue = 1.0;
      double gapShift = -1.0;
      MbRuledSolidValues::MbeGapShiftType shiftType( MbRuledSolidValues::gsSegmentRatio );
      int generSidesByNorm = 1;
      int guideSidesByNorm = 1;
      bool cylindricBends( false );
      bool joinByVertices = false;
      ::GetDouble( IDS_ENTER_RADIUS, radius );
      ::GetDouble( IDS_ENTER_THICKNESS, thickness_ );
      ::GetInt( IDS_ENTER_SEGMENTATION_NUBER, segmentationNumber );
      if ( ::fabs(thickness_) > METRIC_REGION ) {
        ::GetInt( IDS_ENTER_END_DIRECTION, generSidesByNorm );
        if ( contour1->IsClosed() ) {
          ::GetDouble( IDS_ENTER_GAP_VALUE, gapValue );
          if ( ::fabs(gapValue) > METRIC_REGION ) 
            ::GetDouble( IDS_ENTER_DISTANCE_RATIO, gapShift );
        }
      }
      double slopeAngle = 0.0;
      double height = 30.0;
      double surfDistance( 0.0 );
      MbSurface * surface( nullptr );

      if ( contour2 == nullptr ) {
        ::GetDouble( IDS_ENTER_HEIGHT, height );
        if ( ::fabs(height) > METRIC_REGION ) 
          ::GetDouble( IDS_ENTER_SLOPE_ANGLE, slopeAngle );
        ::GetInt( IDS_ENTER_END_DIRECTION, guideSidesByNorm );
      }

      SetWaitCursor( true );

      if ( (contour2 == nullptr) && (segmentationNumber > 0) ) {
        MbContour * resultContour = nullptr;
        MbResultType codeError = ::SplitContourIntoSegments( *contour1, segmentationNumber, resultContour );
        if ( (codeError == rt_Success) && (resultContour != nullptr) )
          contour1 = resultContour;
        else
          ::DeleteItem( resultContour );
      }

      SArray<double> breaks1( 0, 1 ), breaks2( 0 , 1 );
      MbRuledSolidValues parameters( placement1,
                                     *contour1,
                                     (contour2 != nullptr) ? &breaks1 : nullptr,
                                     (contour2 != nullptr) ? &placement2 : nullptr,
                                     contour2,
                                     (contour2 != nullptr) ? &breaks2 : nullptr,
                                     thickness_,
                                     radius,
                                     slopeAngle * M_DEGRAD,
                                     height,
                                     gapValue,
                                     gapShift,
                                     shiftType,
                                     guideSidesByNorm > 0,
                                     generSidesByNorm > 0,
                                     cylindricBends,
                                     joinByVertices,
                                     surfDistance,
                                     surface );
      MbSolid * resultSolid = nullptr;
      MbSNameMaker nameMaker( ct_RuledSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
      PArray<MbSMBendNames> bendParams( 0, 1, true );

      MbContour * filletedContour = nullptr;
      // построение фантома с получением умолчательного разбиения контуров (bendParams пустой)
      // create a phantom body to get default splitting of the contours (bendParams is empty)
      MbResultType res = ::CreateRuledSolid( parameters, nameMaker, bendParams, filletedContour, resultSolid );
      ::DeleteItem( filletedContour );

      if ( contour2 != nullptr ) {
        res = rt_Success;
        ::DeleteItem( resultSolid );

        // сегментация
        // segmentation
        SArray<size_t> segmNumbers1( 0, 1 );
        SArray<size_t> segmNumbers2( 0, 1 );
        MbSNameMaker names;
        res = ::SplitContoursIntoSegments(*parameters.contour1, *parameters.breaks1,
                                          *parameters.contour2, *parameters.breaks2,
                                           names, segmNumbers1, segmNumbers2, segmentationNumber, parameters.gapValue );

        if ( res == rt_Success ) {
          res = rt_Error;
          // заполнение массива сгибов
          // fill the array of resulting bends parameters
          if ( FillBendNamesArray(*parameters.contour1,  parameters.placement1, *parameters.breaks1,
                                  *parameters.contour2, *parameters.placement2, *parameters.breaks2,
                                   nameMaker, bendParams) ) {
            for ( size_t bendParamsIndex = bendParams.Count(); bendParamsIndex--; ) {
              MbSMBendNames * bendNames = bendParams[bendParamsIndex];
              PRECONDITION( bendNames != nullptr );
              if ( bendNames->segName2 != SimpleName(SIMPLENAME_MAX) )
                bendNames->radius = radius;
            }

            // построение обечайки
            // create the lofted bends
            parameters.gapShift = -1.0;
            res = ::CreateRuledSolid( parameters, nameMaker, bendParams, filletedContour, resultSolid );
            ::DeleteItem( filletedContour );
          }
        }
      }

      SetWaitCursor( false );

      if ( resultSolid )
        TestVariables::viewManager->AddObject( TestVariables::SWEPT_Style, resultSolid );
      else
        ::PutErrorMessage( res, IDS_TITLE_ERR );
    }
  }
}


//------------------------------------------------------------------------------
// Получить от пользователя параметры для конструирования ребра жесткости листового тела.
// Get parameters to create ribs for metal sheet.
//---
static bool GetSheetRibValues( const MbSolid  &       sourceSolid, // Исходное тело.
                               const MbPlacement3D &  place, // Локальная система координат, в плоскости которой расположен двумерный контур.
                                     MbContour &      cntr, // Двумерный контур ребра жесткости.
                                     SheetRibValues & pars, // Параметры операции.
                                     ptrdiff_t &      index ) // Индекс сегмента в контуре, устанавливающий вектор уклона.
{
  bool res = false;
  MbCartPoint3D origin;
  MbVector3D    dir3D;

  if ( ::GetInt(IDS_ENTER_SEGMENT_INDEX,index) ) {
    RibValues::ExtrudeSide ribExtrSide = ( RibValues::ExtrudeSide )TestVariables::ribPart;
    if ( sourceSolid.GetShell() )
      ::GetAutoReference( *sourceSolid.GetShell(), place, cntr, index, ribExtrSide, origin, dir3D );

    TestVariables::ribPart = ( int )ribExtrSide;
    if ( ::GetInt( IDS_ENTER_SIDE, TestVariables::ribPart ) ) {
      if ( ::GetDouble( IDS_ENTER_THICKNESS, TestVariables::thickness1 ) ) {
        TestVariables::thickness2 = TestVariables::thickness1;
        if ( ::GetDouble( IDS_ENTER_THICKNESS, TestVariables::thickness2 ) ) {
          double angle = TestVariables::angle1 * 180 / M_PI;
          if ( ::GetDouble(IDS_ENTER_SLOPE_ANGLE,angle) ) {
            angle *= M_PI / 180;
            TestVariables::angle1 = angle;
            TestVariables::angle2 = angle;
            double radFilletRib = 0.0, radFilletCrossRib = 0.0;
            if ( GetDouble(IDS_ENTER_RADIUS,radFilletRib) ) {
              radFilletCrossRib = radFilletRib;
              res = GetDouble (IDS_ENTER_RADIUS,radFilletCrossRib );
              pars.Init( SheetRibValues( TestVariables::thickness1, TestVariables::thickness2, TestVariables::angle1, TestVariables::angle2, TestVariables::ribPart, radFilletRib, radFilletCrossRib ) );
              res = true;
            }
          }
        }
      }
    }
  }

  return res;
} // GetSheetRibValues


//------------------------------------------------------------------------------
// Создание листового тела c ребром жесткости.
// Create a sheet rib solid.
// ---
void CreateSheetRib()
{
  MbItem * solid;
  MbItem * planeCurve;

  if ( ::GetGObj(IDS_SELECT_SOLID,st_Solid,&solid, TestVariables::itemPathFrom ) ) {

    TestVariables::viewManager->ToggleObject( solid );

    if ( ::GetMObj(IDS_SELECT_CRV_ON_PLANE,pt_Curve,&planeCurve)  ) {
      MbCartPoint3D p;

      p = TestVariables::viewManager->GetActiveWindow()->GetOrigin();
      MbPlaneInstance * pobj = ( MbPlaneInstance * )planeCurve;
      TestVariables::viewManager->ToggleObject( planeCurve );

      MbContour c;
      MbContour * cntr;
      const MbPlaneItem * geomItem = ( ( MbPlaneInstance * )planeCurve )->GetPlaneItem();
      if ( geomItem != nullptr ) {
        if ( geomItem->Type() == pt_Contour )
          cntr = ( MbContour * )geomItem;
        else {
          c.AddSegment( ( MbCurve * )&( ( MbCurve * )geomItem )->Duplicate() );
          cntr = &c;
        }
        MbPlacement3D place( pobj->GetPlacement() );
        MbCartPoint mapPoint;
        place.PointProjection( p, mapPoint.x, mapPoint.y );

        ptrdiff_t index = cntr->GetSegmentsCount() / 2;

        MbSolid  & sourceSolid = *( MbSolid * )solid;
        SheetRibValues pars;
        if ( ::GetSheetRibValues(sourceSolid,place,*cntr, pars,index) ) {

          SetWaitCursor( true );

          MbSNameMaker name( (SimpleName)ct_RibSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
          name.Add( 1 );

          SolidSPtr result;
          MbResultType res = ::SheetRibSolid( SolidSPtr(&sourceSolid), cm_Copy, MbSheetRibSolidParams(place, *cntr, index, pars, name), result );
          if ( res != rt_Success )
            ::PutErrorMessage( res, IDS_RIB_ERROR );
          if ( result ) {
            TestVariables::viewManager->DeleteObject( solid );

            Style ns = TestVariables::SOLID_Style;
            TestVariables::viewManager->AddObject( ns, result );
          }
          else {
            TestVariables::viewManager->ToggleObject( solid );
          }
          SetWaitCursor( false );
        }
      }
      TestVariables::viewManager->ToggleObject( planeCurve );
    }
  }
} // CreateSheetRibSolid()


//------------------------------------------------------------------------------
// Получить от пользователя параметры для конструирования штамповки листового тела с помощью другого тела.
// Get parameters to create stamp by solid.
//---
static bool GetSheetUserStampValues( const MbSolid &              sourceSolid, // Листовое тело,
                                     const MbFace &               face,        // грань штамповки,
                                     const MbSolid &              toolSolid,   // тело инструмента,
                                           bool &                 isPunch,     // является тело-инструмент пуансоном(true) или матрицей(false),
                                           FacesSPtrVector &      pierceFaces, // набор граней для вырубки,
                                           MbToolStampingValues & pars )       // параметры операции.
{
  bool res = false;
  MbItem * obj = nullptr;
  MbTopItem * topFace = nullptr;
  MbFace * pairFace = ::GetPairPlanarFace( sourceSolid.GetShell(), face );
  double stampThickness = ::GetDistanceIfSameAndOpposite( face, pairFace );
  double radFilletPunch( 0.0 ), radFilletTool( 0.0 );

  while ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &topFace, TestVariables::itemPathFrom.second ) ) {
    if ( static_cast<MbSolid *>(obj) == &toolSolid ) {
      TestVariables::drawImp->DrawItem( topFace, TRGB_WHITE );
      MbFace * selFace = static_cast< MbFace * >( topFace );
      FacesSPtrVector::iterator i0 = std::find( pierceFaces.begin(), pierceFaces.end(), selFace );
      if ( i0 != pierceFaces.end() ) {
        TestVariables::drawImp->DrawItem( topFace, TRGB_LIGHTGREEN );
        pierceFaces.erase(i0);
      }
      else
        pierceFaces.push_back( FaceSPtr(selFace) );
    }
  }
  if ( ::GetDouble(IDS_ENTER_THICKNESS, stampThickness) &&
       ::GetDouble(IDS_POCKET_FLOOR, radFilletPunch),
       ::GetDouble(IDS_POCKET_FILLET, radFilletTool) ) {
    isPunch = ::GetBool( IDS_STAMP_TYPE_BOOL, isPunch );
    pars.Init( MbToolStampingValues(radFilletPunch, radFilletPunch - stampThickness, radFilletTool, stampThickness,true) );
    res = true;
  }
  return res;
} // GetSheetUserStampValues


//------------------------------------------------------------------------------
// Создание штамповки листового тела другим телом.
// Create a sheet solid stamp with a punch or a die.
// ---
void SheetSolidStampWithTool()
{
  MbItem * solid1( nullptr ), * solid2( nullptr );
  MbTopItem * topItem ( nullptr );
  if ( ::GetTObj(IDS_SELECT_FACE, st_Solid, tt_Face, &solid1, &topItem) && solid1 != nullptr && topItem != nullptr ) {
    SolidSPtr sheetSolid( static_cast<MbSolid *>(solid1) );
    FaceSPtr targetFace( static_cast<MbFace *>(topItem) );
    TestVariables::viewManager->ToggleObject( sheetSolid );
    if ( targetFace->GetName().IsSheet() ) {
      if ( ::GetGObj(IDS_SELECT_SOLID, st_Solid, &solid2, TestVariables::itemPathFrom) ) {
        SolidSPtr toolSolid (static_cast<MbSolid *>(solid2) );
        TestVariables::viewManager->ToggleObject( toolSolid );
        MbToolStampingValues pars;
        FacesSPtrVector pierceFaces;
        bool isPunch = true;
        if ( ::GetSheetUserStampValues(*sheetSolid, *targetFace, *toolSolid, isPunch, pierceFaces, pars) ) {
          SetWaitCursor( true );
          MbSNameMaker name( (SimpleName)ct_UserStampSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
          name.Add( 1 );
          MbStampWithToolParams parameters( targetFace, pierceFaces, isPunch, pars, name );
          SolidSPtr result;
          MbResultType res = ::StampWithToolSolid( sheetSolid, cm_Copy, toolSolid, cm_Copy, parameters, result );
          if ( res != rt_Success )
            ::PutErrorMessage( res, IDS_TITLE_ERR );
          if ( result ) {
            TestVariables::viewManager->DeleteObject( solid1 );
            TestVariables::viewManager->DeleteObject( solid2 );
            Style ns = TestVariables::SOLID_Style;
            TestVariables::viewManager->AddObject( ns, result );
          }
          else
            TestVariables::viewManager->ToggleObject( solid1 );
          SetWaitCursor( false );
        }
      }
    }
    else
      MessageBoxEx( RT_PARAMETER_ERROR, IDS_TITLE_ERR );
  }
} // SheetSolidStampWithTool()


//------------------------------------------------------------------------------
// Построить листовое тела на основе произвольного тела
// Build sheet metal solid based on an arbitary solid.
// ---
void BuildSheetMetalByArbitarySolid()
{
  MbItem * solid1( nullptr );
  MbTopItem * topItem( nullptr );
  if ( ::GetTObj(IDS_SELECT_FACE, st_Solid, tt_Face, &solid1, &topItem) &&
       solid1 != nullptr && topItem != nullptr ) {
    SolidSPtr sourceSolid( static_cast<MbSolid *>(solid1) );
    ConstFaceSPtr targetFace( static_cast<MbFace *>(topItem) );
    MbSolidToSheetMetalValues pars;
    MbItem * solidObj = nullptr;
    MbTopItem * edgeTop = nullptr;
    RPArray<MbCurveEdge> bendEdgesArray( 1, 1 );
    while ( ::GetTObj(IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &solidObj, &edgeTop) && // Выбор ребер сгиба.
            solidObj != nullptr && edgeTop != nullptr ) {
      if ( solid1 == solidObj ) {
        MbCurveEdge * curveEdge = static_cast<MbCurveEdge *> (edgeTop);
        size_t edgeIndex = bendEdgesArray.FindIt( curveEdge );
        if ( edgeIndex != SYS_MAX_T )
          bendEdgesArray.DetachInd( edgeIndex );
        else
          bendEdgesArray.Add( curveEdge );
      }
    }
    size_t edgesCount = bendEdgesArray.Count();
    SArray<MbEdgeFacesIndexes> indicies;
    if ( edgesCount > 0 ) {
      pars.bendEdges.reserve( edgesCount );
      sourceSolid->FindFacesIndexByEdges( bendEdgesArray, indicies );
    }
      
    bool sense = true;
    double sheetThickness( 1.0 ), bendRadius( 5.0 );
    int s = sense ? 1 : -1;
    if ( ::GetDouble(IDS_ENTER_THICKNESS, sheetThickness) &&
         ::GetDouble(IDS_ENTER_RADIUS, bendRadius ) &&
         ::GetInt(IDS_ENTER_SENSE, s) ) {
      pars.sheetThickness = sheetThickness;
      pars.k = 0.4;
      sense = s > 0 ? true : false;

      SetWaitCursor( true );

      for ( size_t eInd = 0, eCount = indicies.Count(); eInd < eCount; eInd++ )
        pars.bendEdges.push_back( MbSolidToSheetMetalValues::MbBendEdgeValues(0.4, bendRadius, indicies[eInd]) );
      MbClosedCornerValues defaultCornerParams( MbClosedCornerValues::ccRip,
                                                MbClosedCornerValues::cbNone,
                                                MbClosedCornerValues::cpBend,
                                                0.0, // gap
                                                0.0/*diameter*/,
                                                0.0/*shift*/,
                                                0.4/*kPlus*/,
                                                0.4/*kMinus*/,
                                                M_PI_4/*angle*/,
                                                true/*plus*/,
                                                true/*prolong*/,
                                                false/*acrossBend*/ );
      pars.defaultCornerValues = defaultCornerParams;
      MbSNameMaker name( (SimpleName)ct_UserStampSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
      name.Add( 1 );
      SolidSPtr result;

      MbSolidToSheetMetalParams params( targetFace, sense, pars, name );
      MbResultType res = ::ConvertSolidToSheetMetal( sourceSolid, cm_Copy, params, result );
      
      if ( result != nullptr && res == rt_Success ) {
        Style ns = TestVariables::SOLID_Style;
        TestVariables::viewManager->AddObject( ns, result );
      }
      else {
        ::PutErrorMessage( res, IDS_ERROR );
        TestVariables::viewManager->ToggleObject( solid1 );
      }

      SetWaitCursor( false );
    }
  }
} // BuildSheetMetalBySolid()
