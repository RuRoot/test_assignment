//////////////////////////////////////////////////////////////////////////////////////////
//
//    The file contains source codes demonstrating samples how to create lofted bends.
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
#include <cur_arc.h>
//#include <cur_contour.h>
//#include <cur_surface_intersection.h>
//#include <surf_curve_bounded_surface.h>
//#include <topology.h>
//#include <creator.h>
//#include <solid.h>
//#include <action_surface.h>
#include <action_sheet.h>
//
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// \ru Построение обечайки. \en A lofted solid creation.
// ---
void SampleLoftedBend() {
  // \ru Создаём квадрат. \en Create a quadrangle.
  MbPlacement3D placement1;
  SPtr<MbContour> contour1( new MbContour() );
  contour1->AddSegment( new MbLineSegment(-50.0/*u1*/, -50.0/*v1*/, 50.0/*u2*/, -50.0/*v2*/) );
  contour1->AddSegment( new MbLineSegment(50.0/*u1*/, -50.0/*v1*/, 50.0/*u2*/, 50.0/*v2*/) );
  contour1->AddSegment( new MbLineSegment(50.0/*u1*/, 50.0/*v1*/, -50.0/*u2*/, 50.0/*v2*/) );
  contour1->AddSegment( new MbLineSegment(-50.0/*u1*/, 50.0/*v1*/, -50.0/*u2*/, -50.0/*v2*/) );

  // \ru Создаём окружность. \en Create a circle.
  MbCartPoint3D origin( 0.0/*x*/, 0.0/*y*/, 100.0/*z*/ );
  MbPlacement3D placement2( origin );
  SPtr<MbContour> contour2;
  int createTwoContoursBend = 1;
  ::GetInt( IDS_TWO_CONTOURS_BEND, createTwoContoursBend );
  if ( createTwoContoursBend > 0 ) {
    contour2 = new MbContour();
    contour2->AddSegment( new MbArc(40.0/*raduis*/) );
  }

  double radius = 10.0; // \ru Радиус скругления контура. \en The contour fillet radius.
  double thickness = 1.0; // \ru Толщина листа. \en Thickness of the sheet metal.
  int segmentationNumber = 4; // \ru Умолчательное количество сегментов разбиения дуг. \en Quantity of segments to split arcs into by default.
  double gapValue = 1.0; // \ru Ширина зазора. \en The gap width.
  double gapShift = -1.0; // \ru Смещение зазора. -1.0 - автоматический выбор смещения. \en The shift of the gap position. -1.0 - the shift is calculated by the lofted bend operation.
  MbRuledSolidValues::MbeGapShiftType shiftType( MbRuledSolidValues::gsSegmentRatio ); // \ru Тип смещения. \en The shift type.
  int generSidesByNorm = 1; // \ru Боковины зазора построить по нормали к листовым граням. 0 - компланарно к нормали контура в точке центра зазора. \en Create side faces of the gap by normal to the sheet faces.
  int guideSidesByNorm = 1; // \ru Нижнюю и верхнююграни обечайки построить по нормали к листовым граням. 0 - в плоскостях эскизов. \en Create top and bottom side faces of the lofted bend by normal to the sheet faces.
  bool cylindricBends = false; // \ru На дугах при уклоне строить конические сгибы. true - цилиндрические. \en Create conical bends where the arcs in the contour are. true - cylindrical bends.
  bool joinByVertices = false; // \ru Разбиение контуров проводить по вершинам. \en Split contours by vertices.
  ::GetDouble( IDS_ENTER_RADIUS, radius );
  ::GetDouble( IDS_ENTER_THICKNESS, thickness );
  ::GetInt( IDS_ENTER_SEGMENTATION_NUBER, segmentationNumber );
  if ( ::fabs(thickness) > METRIC_REGION ) {
    ::GetInt( IDS_ENTER_END_DIRECTION, generSidesByNorm );
    if ( contour1->IsClosed() ) {
      ::GetDouble( IDS_ENTER_GAP_VALUE, gapValue );
      if ( ::fabs(gapValue) > METRIC_REGION ) 
        ::GetDouble( IDS_ENTER_DISTANCE_RATIO, gapShift );
    }
  }
  double slopeAngle = 0.0; // \ru Угол уклона обечайки по одному эскизу. \en The tilt of sheet faces of the lofted bend when it is created by one contour.
  double height = 30.0; // \ru Высота обечайки по одному контуру. \en The height of the lofted bend when it is created by one contour.
  double surfDistance =  0.0; // \ru Отступ от поверхности до которой строить обечайку по одному контуру. \en The gap between the surface and the top side face of the lofted bend when it is created by one contour.
  MbSurface * surface = nullptr; // \ru Поверхность, до которой вытягивать обечайку по одному контуру. \en The surface to create the upper side face of the lofted bend.

  if ( contour2 == nullptr ) {
    ::GetDouble( IDS_ENTER_HEIGHT, height );
    if ( ::fabs(height) > METRIC_REGION ) 
      ::GetDouble( IDS_ENTER_SLOPE_ANGLE, slopeAngle );
    ::GetInt( IDS_ENTER_END_DIRECTION, guideSidesByNorm );
  }

  SetWaitCursor( true );

  // \ru Сегментация контура для построения обечайки по одному контуру. \en The segmentation of the contour if the bend is created by one contour.
  if ( (contour2 == nullptr) && (segmentationNumber > 0) ) {
    MbContour * resultContour = new MbContour();
    for ( size_t i = 0, count = contour1->GetSegmentsCount(); i < count; i++ ) {
      const MbCurve * curveSegment = contour1->GetSegment( i );
      if ( curveSegment != nullptr ) {
        bool addSegment = false;
        if ( curveSegment->IsA() == pt_Arc ) {
          MbContour * segmentContour = nullptr;
          MbResultType codeError = ::SplitContourIntoSegments( *curveSegment, segmentationNumber, segmentContour );
          if ( codeError == rt_Success )
            addSegment = resultContour->AddSegment( segmentContour );
          ::DeleteItem( segmentContour );
        }

        if ( !addSegment )
          resultContour->AddSegment( reinterpret_cast<MbCurve*>(&curveSegment->Duplicate()) );
      }
    }

    if ( resultContour != nullptr )
      contour1 = resultContour;
  }

  SArray<double> breaks1( 0, 1 ), breaks2( 0 , 1 ); // \ru Параметры разбиения контуров \en Contours splitting parameters.
  MbRuledSolidValues parameters( placement1,
                                 *contour1,
                                 (contour2 != nullptr) ? &breaks1 : nullptr,
                                 (contour2 != nullptr) ? &placement2 : nullptr,
                                 contour2,
                                 (contour2 != nullptr) ? &breaks2 : nullptr,
                                 thickness,
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
  // построение фантома с получением умолчательного разбиения контуров (bendParams, breaks1 и breaks2 пустые)
  // create a phantom body to get default splitting of the contours (bendParams, breaks1, and breaks2 are empty)
  MbResultType res = ::CreateRuledSolid( parameters, nameMaker, bendParams, filletedContour, resultSolid );
  ::DeleteItem( filletedContour );

  if ( contour2 != nullptr ) {
    res = rt_Success;
    ::DeleteItem( resultSolid );

    // сегментация
    // segmentation
    SArray<size_t> segmNumbers1( 0, 1 ); // \ru Индивидуальные параметры сегментации. \en The individual slitting parameters.
    SArray<size_t> segmNumbers2( 0, 1 ); // \ru Индивидуальные параметры сегментации. \en The individual slitting parameters.
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
    TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, resultSolid );
  else
    ::PutErrorMessage( res, IDS_TITLE_ERR );
}

