//////////////////////////////////////////////////////////////////////////////////////////
// Методы создания и операции над двухмерными контурами.
// Creation methods and operations on two-dimensional contours.
//////////////////////////////////////////////////////////////////////////////////////////

#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <test_window.h>
#include <test_draw.h>

#include <cur_arc.h>
#include <cur_line_segment.h>
#include <cur_polyline.h>
#include <cur_trimmed_curve.h>
#include <alg_curve_equid.h>
#include <alg_curve_envelope.h>
#include <alg_curve_fillet.h>
#include <alg_polyline.h>
#include <contour_graph.h>
#include <region.h>
#include <mesh_plane_grid.h>
#include <action_curve.h>
#include <plane_instance.h>
#include <space_instance.h>
#include <templ_dptr.h>
#include <templ_sptr.h>
#include <last.h>

using namespace c3d;

std::unique_ptr<MpGraph> graph;


//----------------------------------------------------------------------------------------
// Новый контур. New contour.
// ---
bool NewContour()
{
  MbContour * contour = nullptr;
  bool first = true;
  MbCartPoint currPoint, startPoint;
  size_t deg = Math::curveDegree;
  ptrdiff_t res = IDC_PUSHBUTTON_SPLINE;

  do {
    res = ExecuteContourDlg();
    switch ( res ) {
      case IDC_PUSHBUTTON_SPLINE : // Сплайн. A spline.
      {
        SArray<MbCartPoint> pList;

        if ( first )
          contour = new MbContour();
        else
          pList.push_back( currPoint );

        while ( GetPoint( IDS_SELECT_POINT, currPoint ) ) {
          TestVariables::drawImp->DrawPoint( &currPoint, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
          pList.push_back( currPoint );

          if ( first ) {
            first = false;
            startPoint = currPoint;
          }
        }

        if ( pList.size() >= deg )
        {
          SArray<double> knots;
          SArray<double> wList;
          MbCurve * seg = nullptr;
          ::NurbsCurve( pList, wList, deg, knots, false, seg );
          contour->AddSegment( seg );
          TestVariables::drawImp->DrawItem( seg, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
        }
        pList.clear();
      }
      break;

      case IDC_PUSHBUTTON_SEGMENT: // Отрезок прямой. A line segment.
      {
        ptrdiff_t idx = 1;
        MbCartPoint pt[2];

        if ( first ) {
          contour = new MbContour;
          idx = 0;
        }
        else
          pt[0] = currPoint;

        for ( ptrdiff_t i = idx; i < 2; i++ ) 
        {
          GetPoint( IDS_SELECT_POINT, currPoint );
          TestVariables::drawImp->DrawPoint( &currPoint, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
          pt[i] = currPoint;

          if ( first ) 
          {
            first = false;
            startPoint = currPoint;
          }
        }

        MbLineSegment * seg = new MbLineSegment( pt[0], pt[1] );
        contour->AddSegment( seg );
        TestVariables::drawImp->DrawItem( seg, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
      }
      break;

      case IDC_PUSHBUTTON_POLYLINE : // Полилиния. A polyline.
      {
        SArray<MbCartPoint> pList( 4, 2 );

        if ( first )
          contour = new MbContour;
        else
          pList.Add( currPoint );

        while ( GetPoint( IDS_SELECT_POINT, currPoint ) ) {
          TestVariables::drawImp->DrawPoint( &currPoint, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
          pList.Add( currPoint );

          if ( first ) {
            first = false;
            startPoint = currPoint;
          }
        }

        if ( pList.Count() > 1 ) 
        {
          MbPolyline * seg = new MbPolyline( pList, false );
          contour->AddSegment( seg );
          TestVariables::drawImp->DrawItem( seg, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
        }
        pList.Flush();
      }
      break;

      case IDC_PUSHBUTTON_ARC : // Дуга. An arc.
      {
        ptrdiff_t idx = 1;
        MbCartPoint pt[3];

        if ( first ) 
        {
          contour = new MbContour();
          idx = 0;
        }
        else
          pt[0] = currPoint;

        for ( ptrdiff_t i = idx; i < 3; i++ ) {
          GetPoint( IDS_SELECT_POINT, currPoint );
          TestVariables::drawImp->DrawPoint( &currPoint, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
          pt[i] = currPoint;

          if ( first ) {
            first = false;
            startPoint = currPoint;
          }
        }
        MbCurve * seg = MbArc::Create( pt[0], pt[1], pt[2] );
        if ( seg != nullptr ) {
          contour->AddSegment( seg );
          TestVariables::drawImp->DrawItem( seg, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
        }
      }
      break;

      case IDC_PUSHBUTTON_CLOSE : // Замкнуть
      {
        if ( contour != nullptr ) {
          MbLineSegment *seg = new MbLineSegment( currPoint, startPoint );
          contour->AddSegment( seg );
          contour->CalculateParamLength(); // Вычислить параметрическую длину
        }
      }
      break;

      case IDC_PUSHBUTTON_EXIT : // Выход
      {
        if ( contour != nullptr )
          contour->CalculateParamLength(); // Вычислить параметрическую длину
      }

      break;
    }

  } while ( ( res != IDC_PUSHBUTTON_CLOSE ) && ( res != IDC_PUSHBUTTON_EXIT ) );

  if ( contour != nullptr )
  {
    TestVariables::viewManager->AddObject( TestVariables::ELEMENTARY_Style, contour );
    {
      bool contLength = true, contDirect = true;
      contour->IsContinuousDerivative( contLength, contDirect ); // C3D-1701
      if ( !contLength ) {
        contour->SetContinuousDerivativeLength( Math::DefaultMathVersion() );
        contour->IsContinuousDerivative( contLength, contDirect );
        if ( contLength ) {
        }
      }
    }
    return true;
  }
  
  return false;
}


//----------------------------------------------------------------------------------------
// Построить эквидистанту к указанному контуру (устаревший).
// Build the offset of pointed contour (outdated method).
// ---
bool ContourEquid()
{
  MbCartPoint gPnt;
  MbItem * gObj = nullptr;

  if ( GetPoint(IDS_SELECT_CNTR_ON_PLANE, gPnt) &&
       FindNearestObj( gPnt, pt_Curve, &gObj ) )
  {
    TestVariables::viewManager->ToggleObject(gObj);

    double radius = TestVariables::offset0;;
    int mode = 0, side = 0;

    if ( GetDouble( IDS_ENTER_RADIUS, radius ) &&
         GetInt( IDS_ENTER_BYPASS_TYPE, mode ) &&
         GetInt( IDS_ENTER_BYPASS_SIDE, side ) )
    {
      TestVariables::offset0 = radius;
      PArray<MbCurve> equLeft ( 0, 1, false );
      PArray<MbCurve> equRight( 0, 1, false );

      MbPlacement3D place;
      SPtr<MbCurve> initCurve( ::GetCurveOnPlace(gObj, place) );

      MbAccurateTimer ac;

      ac.Begin();
      ::Equid( initCurve, radius, radius, side, mode == 0, 0, equLeft, equRight ); // Outdated method.
      ac.End();

      if ( TestVariables::checkOperationTime ) {
        const size_t bufLen = 1024;
        TCHAR buf[bufLen];
        size_t len = 0;
        len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
        ::MessageBoxEx( buf, IDS_RESULTS );
      }

      for ( size_t i = 0; i < equLeft.Count(); i++ )
        TestVariables::viewManager->AddObject( TestVariables::OFFSET_Style, equLeft[i], &place );

      for ( size_t i = 0; i < equRight.Count(); i++ )
        TestVariables::viewManager->AddObject( TestVariables::OFFSET_Style, equRight[i], &place );
    }

    TestVariables::viewManager->ToggleObject( gObj );

    return true;
  }

  return false;
}


//----------------------------------------------------------------------------------------
// Построение эквидистантных контуров.
// Building offset contours.
// ---
bool MakeOffsetContour()
{
  MbItem * obj = nullptr;
  MbCartPoint point;

  if ( GetPoint(IDS_SELECT_OBJ_ON_PLANE, point) && (FindNearestObj(point, pt_Curve, &obj )) )
  {
    MbPlacement3D pl;
    SPtr<MbContour> initContour( ::GetContourOnPlace(obj, pl) );

    // Кривая контур, будем скруглять его элементы.
    // A contour curve, will do fillets of its segments.
    if ( initContour != nullptr )
    { 
      double radius = TestVariables::offset0;
      if ( GetDouble(IDS_ENTER_RADIUS, radius) ) 
      {        
        TestVariables::offset0 = radius;

        MbAccurateTimer ac;

        ac.Begin();
        MbContour * cntr = OffsetContour( *initContour, radius, pl.GetXEpsilon(), pl.GetYEpsilon(), true );
        ac.End();

        if ( cntr != nullptr )
          TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, cntr, &pl );

        if ( TestVariables::checkOperationTime ) {
          const size_t bufLen = 1024;
          TCHAR buf[bufLen];
          size_t len = 0;
          len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
          ::MessageBoxEx( buf, IDS_RESULTS );
        }

        //{ // KOMPAS-35826
        //  bool arcMode = true, degState = false;
        //  PArray<MbCurve> equLeft( 10, 1 ); 
        //  PArray<MbCurve> equRight( 10, 1 );

        //  ::Equid( initContour, radius, radius, (radius<0) ? 1 : 0, arcMode, degState, equLeft, equRight );

        //  for ( size_t i = 0, iCount = equLeft.size(); i < iCount; i++ )
        //    TestVariables::viewManager->AddObject( TestVariables::CIRCLE_Style, equLeft[i], &pl );
        //  for ( size_t j = 0, jCount = equRight.size(); j < jCount; j++ )
        //    TestVariables::viewManager->AddObject( TestVariables::CIRCLE_Style, equRight[j], &pl );

        //  equLeft.Flush( noDelete );
        //  equRight.Flush( noDelete );
        //} // KOMPAS-35826

        return true;
      }
    }
  }
  /*
  if ( GetPoint(IDS_SELECT_OBJ_ON_PLANE, point) && (FindNearestObj(point, pt_Curve, &obj )) )
  { // SD_7239168
    MbPlacement3D pl;

    if ( obj!= nullptr && obj->IsA() == st_PlaneInstance ) {
      MbPlaneItem * pItem = static_cast<MbPlaneInstance *>( obj )->SetPlaneItem(); // Дать двумерный геометрический объект

      if ( pItem != nullptr ) {
        MbePlaneType pType = pItem->IsA();
        if ( pType == pt_Contour ) {
          MbContour * pContour = static_cast<MbContour *>( pItem );
          pl.Init( static_cast<const MbPlaneInstance *>( obj )->GetPlacement() );
          double radius = TestVariables::offset0;
          if ( GetDouble(IDS_ENTER_RADIUS, radius) ) {        
            TestVariables::offset0 = radius;
            int side = 0;
            int arcMode = 1;
            ::GetInt( IDS_ENTER_BYPASS_SIDE, side ); 
            ::GetInt( IDS_ENTER_BYPASS_TYPE, arcMode ); 
            PArray<MbCurve> equLeft( 0, 1 ); 
            PArray<MbCurve> equRight( 0, 1 );
  
            pContour->Equid( radius, radius, side, arcMode == 0, equLeft, equRight );
            size_t i = 0, iCount;

            for ( i = 0, iCount = equLeft.size(); i < iCount; ++i ) {
              TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, equLeft[i], &pl );
            }

            for ( i = 0, iCount = equRight.size(); i < iCount; ++i ) {
              TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, equRight[i], &pl );
            }

            equLeft.Flush( noDelete );
            equRight.Flush( noDelete );

            return true;
          }
        }
      }
    }
  }
  */
  return false;
}


//----------------------------------------------------------------------------------------
// Построение эквидистантных контуров для выдавливания под углом
// Build offset contours for the extrusion at an angle.
// ---
bool MakeSlopeContour() 
{
  MbItem * curve = nullptr; // Образующая. The forming curve.
  MbItem * curve0 = nullptr;
  double  thicknes1 = 1;
  double  thicknes2 = 0;
  PArray<MbContour> c( 2, 2, true ); // The owning array.
  MbPlacement3D pl;

  while ( GetMObj( IDS_SELECT_GENER_CURVE, pt_Curve, &curve ) ) 
  {
    if ( curve0 != nullptr )
    {
      TestVariables::viewManager->ToggleObject( curve0 );
      if ( !pl.Colinear(static_cast<MbPlaneInstance*>(curve)->GetPlacement()) 
        || ( curve==curve0 ) )
      {
        break;
      }
    }
    else {
      pl.Init(static_cast<MbPlaneInstance*>(curve)->GetPlacement());
    }
    TestVariables::viewManager->ToggleObject( curve );
    
    if ( const MbPlaneItem * planeItem = static_cast<const MbPlaneInstance *>(curve)->GetPlaneItem() ) 
    { 
      if ( planeItem->Type() == pt_Contour ) // Check class type.
      {
        const MbContour *crv = static_cast<const MbContour *>(planeItem);
        c.Add( static_cast<MbContour *>(&crv->Duplicate()) );
      }
      else {
        MbContour * crv = new MbContour;
        crv->AddSegment( static_cast<MbCurve *>( &planeItem->Duplicate()) );
        c.Add( crv );
      }
    }
    curve0 = curve;
  }
  if ( curve0 != nullptr )
    TestVariables::viewManager->ToggleObject(curve0);

  if ( c.Count() > 0 ) 
  {
    if ( GetDouble( IDS_ENTER_THICKNESS, thicknes1 ) 
      && GetDouble( IDS_ENTER_THICKNESS, thicknes2 ) ) 
    {
      SetWaitCursor( true );
      MbContour * contour = nullptr;

      for ( ptrdiff_t i = 0, cnt = c.Count(); i < cnt; i++ ) 
      {
        // SD#778768 — Как убрать самопересечение (наложение) контура if ( thicknes1 != 0.0 ) 
        {
          contour = ::OffsetContour( *c[i], thicknes1, pl.GetXEpsilon(), pl.GetYEpsilon(), true );
          if ( contour!=nullptr )
            TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, contour, &pl );
        }
        // SD#778768 — Как убрать самопересечение (наложение) контура if ( thicknes2 != 0.0 )
        if ( ::fabs(thicknes1) != ::fabs(thicknes2) ) {
          contour = ::OffsetContour( *c[i],-thicknes2, pl.GetXEpsilon(), pl.GetYEpsilon(), true );
          if ( contour!=nullptr )
            TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, contour, &pl );
        }
      }

      SetWaitCursor( false );
      return true;
    }
  }

  return false;
}


//----------------------------------------------------------------------------------------
// Построение эквидистантных контуров для вращения вокруг оси.
// Create offset contours to prepare the rotation solid operation.
// ---
bool MakeOffsetThinContour() 
{
  MbItem * curve = nullptr; // Образующая. The forming curve.
  MbItem * curve0 = nullptr;
  double   thickness1 = 1;
  double   thickness2 = 0;
  PArray<MbContour> c( 2, 2, true ); // The owning array.
  MbPlacement3D pl;

  while ( GetMObj(IDS_SELECT_GENER_CURVE, pt_Curve, &curve) ) 
  {
    if ( curve0 != nullptr )
    {
      TestVariables::viewManager->ToggleObject( curve0 );
      if ( !pl.Colinear(((MbPlaneInstance *)curve)->GetPlacement()) || (curve==curve0) )
        break;
    }
    else 
    {
      pl.Init(((MbPlaneInstance *)curve)->GetPlacement());
    }
    TestVariables::viewManager->ToggleObject( curve );
    if ( const MbPlaneItem * planeItem = ((MbPlaneInstance*)curve)->GetPlaneItem() ) 
    { 
      if ( planeItem->Type() == pt_Contour ) 
      {
        const MbContour * crv = static_cast<const MbContour *>(planeItem);
        c.Add( static_cast<MbContour *>( &crv->Duplicate() ) );
      }
      else 
      {
        MbContour * crv = new MbContour;
        crv->AddSegment( static_cast<MbCurve *>( &planeItem->Duplicate() ) );
        c.Add( crv );
      }
    }
    curve0=curve;
  }
  if ( curve0 != nullptr )
    TestVariables::viewManager->ToggleObject( curve0 );

  if ( c.Count() > 0 ) 
  {
    MbCartPoint point1, point2;
    if ( GetDouble( IDS_ENTER_THICKNESS, thickness1 ) 
      && GetDouble( IDS_ENTER_THICKNESS, thickness2 ) )
    {
      if ( GetPoint(IDS_SELECT_POINT, point1) && GetPoint(IDS_SELECT_POINT, point2) ) 
      {
        SetWaitCursor( true );

        for ( ptrdiff_t i = 0, cnt = c.Count(); i < cnt; i++ ) 
        {
          if ( thickness1!=0 ) 
          {
            if ( c[i]->IsClosed() && AreaSign( *c[i], Math::visualSag, true ) < 0 )
              c[i]->Inverse();
            MbContour * contour = nullptr;
            if ( CurveRelative( *c[i], point1, point2 ) > 0 )
              contour = AxisOffsetOpenContour( *c[i], point1, point2, thickness1, pl.GetXEpsilon(), pl.GetYEpsilon(), Math::DefaultMathVersion() );
            else
              contour = AxisOffsetOpenContour( *c[i], point2, point1, thickness1, pl.GetXEpsilon(), pl.GetYEpsilon(), Math::DefaultMathVersion() );
            if ( contour!=nullptr )
              TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, contour, &pl );
          }
          if ( thickness2 != 0 ) 
          {
            if ( c[i]->IsClosed() && AreaSign( *c[i], Math::visualSag, true ) < 0 )
              c[i]->Inverse();
            MbContour * contour = nullptr;
            if ( CurveRelative( *c[i], point1, point2 ) > 0 )
              contour = AxisOffsetOpenContour( *c[i], point1, point2, thickness2, pl.GetXEpsilon(), pl.GetYEpsilon(), Math::DefaultMathVersion() );
            else
              contour = AxisOffsetOpenContour( *c[i], point2, point1, thickness2, pl.GetXEpsilon(), pl.GetYEpsilon(), Math::DefaultMathVersion() );
            if ( contour != nullptr )
              TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, contour, &pl );
          }
        }

        SetWaitCursor( false );
        return true;
      }
    }
  }

  return false;
}


//----------------------------------------------------------------------------------------
// Скруглить контур.
// Fillet a contour.
// ---
bool ContourFillet()
{
  MbItem * gObj[2] = { nullptr, nullptr };
  MbCartPoint p1, p2;
  double rad = 5;

  if (  GetDouble( IDS_ENTER_RADIUS, rad ) &&
        GetPoint( IDS_SELECT_CRV_ON_PLANE, p1 ) &&
      ( FindNearestObj(p1, pt_Curve, &gObj[0]) ))
  {
    MbPlacement3D place;
    SPtr<MbCurve> curve( ::GetCurveOnPlace(gObj[0], place) );

    if ( (curve != nullptr) && (curve->Type() == pt_Contour) )
    { // Кривая контур, будем скруглять его элементы.
      // A contour curve, will do fillets between its segments.
      static_cast<MbContour &>(*curve).Fillet( fabs(rad) );

      //TestVariables::viewManager->RedrawObject( gObj[0] );
      TestVariables::viewManager->DeleteObject( gObj[0] );
      TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, curve );
      return true;
    }

    if (  GetPoint( IDS_SELECT_SECOND_CURVE, p2 ) &&
        ( FindNearestObj( p2, pt_Curve, &gObj[1] ) ) )
    {
      MbPlacement3D place1, place2;
      SPtr<MbCurve> curve1( ::GetCurveOnPlace( gObj[0], place1 ) );
      SPtr<MbCurve> curve2( ::GetCurveOnPlace( gObj[1], place2 ) );

      MbMatrix mtr;
      if ( curve1 != nullptr && curve2 != nullptr && (place2.GetMatrixToPlace( place1, mtr )) )
      {
        if ( !mtr.IsSingle() ) 
        {
          curve2 = ::DuplicateIfUsed( curve2 );
          curve2->Transform( mtr );
        }

        bool trim1 = true;
        bool trim2 = true;
        MbeState  state1, state2;
        MbArc * arc = nullptr;

        // Функция скругления постоянным радиусом между двумя кривыми
        // The function of constant radius fillet between two curves.
        bool ok = Fillet( curve1, p1, trim1, curve2, p2, trim2, rad,
                          state1, state2, arc );
        if ( ok ) 
        {
          TestVariables::viewManager->AddObject( TestVariables::OFFSET_Style, arc, &place1 );
        }

        //TestVariables::viewManager->RedrawObject( gObj[0] );
        //TestVariables::viewManager->RedrawObject( gObj[1] );
        TestVariables::viewManager->DeleteObject( gObj[0] );
        TestVariables::viewManager->DeleteObject( gObj[1] );
        TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, curve1 );
        TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, curve2 );
      }
    }

    return true;
  }

  return false;
}


//----------------------------------------------------------------------------------------
// Снять фаски по углам контура.
// Cut chamfers on the contour.
// ---
bool ContourChamfer()
{
  MbItem * gObj[2] = { nullptr, nullptr };
  MbCartPoint p1, p2;
  double len1 = 5, angle = 45;

  if (  GetDouble( IDS_ENTER_FACET_LENGHT, len1 ) &&
        GetDouble( IDS_ENTER_ANGLE, angle ) &&
        GetPoint( IDS_SELECT_CRV_ON_PLANE, p1 ) &&
      ( FindNearestObj( p1, pt_Curve, &gObj[0] ) ) )
  {
    MbPlacement3D place;
    SPtr<MbCurve> curve( ::GetCurveOnPlace( gObj[0], place ) );

    if ( curve != nullptr && curve->IsA() == pt_Polyline )
    { 
      // Кривая полилиния, будем делать фаски между сегментами.
      // A polyline curve, will do the chamfer between the segments.
      static_cast<MbPolyline &>( *curve ).Chamfer( len1, angle, false );
      //TestVariables::viewManager->RedrawObject(gObj[0]);
      TestVariables::viewManager->DeleteObject( gObj[0] );
      TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, curve );
      return true;
    }
    if ( (curve != nullptr) && (curve->Type() == pt_Contour) )
    { 
      // Кривая контур, будем делать фаски между сегментами.
      // A contour curve, will do the chamfer between the segments.
      static_cast<MbContour &>( *curve ).Chamfer( len1, angle, false );
      //TestVariables::viewManager->RedrawObject( gObj[0] );
      TestVariables::viewManager->DeleteObject( gObj[0] );
      TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, curve );
      return true;
    }

    if (  GetPoint( IDS_SELECT_SECOND_CURVE, p2 ) &&
        ( FindNearestObj( p2, pt_Curve, &gObj[1] ) ) )
    {
      MbPlacement3D place1, place2;
      SPtr<MbCurve> curve1( ::GetCurveOnPlace( gObj[0], place1 ) );
      SPtr<MbCurve> curve2( ::GetCurveOnPlace( gObj[1], place2 ) );

      MbMatrix mtr;
      if ( curve1 != nullptr && curve2 != nullptr && ( place2.GetMatrixToPlace(place1, mtr) ) )
      {
        if ( !mtr.IsSingle() ) 
        {
          curve2 = ::DuplicateIfUsed( curve2 );
          curve2->Transform( mtr );
        }

        MbLineSegment * lSeg = nullptr;

        bool trim1 = true;
        bool trim2 = true;

        MbeState state1 = dp_NoChanged, state2 = dp_NoChanged;

        // Функция построения фаски между двумя кривыми.
        // The function cutting a chamfer between two curves.
        bool ok = Chamfer( curve1, p1, trim1, curve2, p2, trim2,
                           len1, angle, false, state1, state2, lSeg );
        if ( ok ) 
        {
          TestVariables::viewManager->AddObject( TestVariables::OFFSET_Style, lSeg );
        }

        //TestVariables::viewManager->RedrawObject( gObj[0] );
        //TestVariables::viewManager->RedrawObject( gObj[1] );
        TestVariables::viewManager->DeleteObject( gObj[0] );
        TestVariables::viewManager->DeleteObject( gObj[1] );
        TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, curve1 );
        TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, curve2 );
      }
    }

    return true;
  }

  return false;
}


//----------------------------------------------------------------------------------------
// Построение контуров вокруг точки.
// Build contours enclosing the point.
// ---
bool CreateEncloseContours() 
{
  bool res = false;

  PArray <MbCurve> curveList( 5, 5, false );
  PArray <MbContour> contourArray( 5, 5, false );
  PArray <MbCurve> usedCurves(5,5,false);
  MbCartPoint gPnt;
  MbItem * obj = nullptr;

  if ( GetPoint(IDS_SELECT_POINT, gPnt) &&
       FindNearestObj(gPnt, pt_Curve, &obj) ) {
    double accuracy( METRIC_ACCURACY );
    GetDouble( IDS_ENTER_ACCURACY, accuracy );
    int strict( 1 );
    GetInt( IDS_ENTER_BAN_CORRECTION, strict );

    MbPlacement3D place;
    obj->GetPlacement( place ); // Запоминаем ЛСК. Save the local coord system.
    // Собрать кривые в плоскости объекта obj в координатах экрана.
    // Collect curves of the obj's flat in the screen coordinates.
    TestVariables::viewManager->CreateCurveArray( obj, curveList, true );
    ::AddRefItems( curveList );

    //GetPoint( IDS_SELECT_POINT, gPnt, &place );

    graph.reset( ::EncloseContoursBuilder( curveList, gPnt, usedCurves, contourArray, accuracy, strict > 0, Math::DefaultMathVersion() ) );

    size_t count = contourArray.Count();
    if ( contourArray.Count() > 0 ) {
      Style style( 2, RGB2uint32(0.0, 0.5, 1.0) );
      for ( size_t i = 0; i < count; ++i ) {
        TestVariables::viewManager->AddObject( style, contourArray[i], &place );
      }
      res = true;
    }    

    // Перестроение контура.
    // if ( MpGraph * g = ContoursReconstructor( usedCurves, graph, contourArray, accuracy, false, Math::DefaultMathVersion() ) )
    //   graph.reset( g );

    ::ReleaseItems( curveList );
  }

  return res;
}


//----------------------------------------------------------------------------------------
// Перестроить контуры, построенные ранее вокруг точки.
// Reconstruct contours constructed around the point before.
// ---
bool CreateOuterContours() 
{
  bool res = false;

  PArray<MbCurve> curveList( 5, 5, false );
  PArray<MbContour> contourArray( 5, 5, false );
  MbCartPoint gPnt;
  MbItem * obj = nullptr;

  if ( GetPoint(IDS_SELECT_OBJ_ON_PLANE, gPnt) &&
       FindNearestObj(gPnt, pt_Curve, &obj) ) 
  {
    double accuracy( METRIC_ACCURACY );
    GetDouble( IDS_ENTER_ACCURACY, accuracy );
    int strict( 1 );
    GetInt( IDS_ENTER_BAN_CORRECTION, strict );

    MbPlacement3D place;
    obj->GetPlacement( place ); // Запоминаем ЛСК. Save the obj's local coord system.
    TestVariables::viewManager->CreateCurveArray( obj, curveList, true );
    ::AddRefItems( curveList );

    graph.reset( ::OuterContoursBuilder( curveList, contourArray, accuracy, strict > 0, Math::DefaultMathVersion() ) );

    const size_t count = contourArray.Count();
    if ( count > 0 ) {
      Style style( 2, RGB2uint32(0.0, 0.5, 1.0) );
      for ( size_t i = 0; i < count; ++i ) {
        TestVariables::viewManager->AddObject( style, contourArray[i], &place );
      }
      res = true;
    }
    ::ReleaseItems( curveList );

    graph.reset( nullptr );
  }

  return res;
}


//----------------------------------------------------------------------------------------
// Перестроить контуры, построенные ранее вокруг точки.
// Reconstruct contours constructed around the point before.
// ---
bool ReconstructContours() 
{
  bool res = false;

  PArray<MbCurve> curveList( 5, 5, false );
  PArray<MbContour> contourArray( 5, 5, false );
  MbCartPoint gPnt;
  MbItem * obj = nullptr;

  if ( graph != nullptr &&
       GetPoint(IDS_SELECT_OBJ_ON_PLANE, gPnt) &&
       FindNearestObj(gPnt, pt_Curve, &obj) ) 
  {
    double accuracy( METRIC_ACCURACY );
    GetDouble( IDS_ENTER_ACCURACY, accuracy );

    MbPlacement3D place;
    obj->GetPlacement( place ); // Запоминаем ЛСК.
    TestVariables::viewManager->CreateCurveArray( obj, curveList, true );
    ::AddRefItems( curveList );    

    if ( MpGraph * g = ContoursReconstructor( curveList, graph.get(), contourArray, accuracy, false, Math::DefaultMathVersion() ) )
      graph.reset( g );

    const size_t count = contourArray.Count();
    if ( count > 0 ) {
      for ( size_t i = 0; i < count; ++i )
        TestVariables::viewManager->AddObject( TestVariables::POINT_Style, contourArray[i], &place );
      res = true;
    }
    ::ReleaseItems( curveList );
  }

  return res;
}


//----------------------------------------------------------------------------------------
// Разбить контур кривой на несколько новых контуров.
// Break the contour to several new contours.
// ---
bool ContourBreak() 
{
  /*
    Yet not implemented.
  */
  PRECONDITION( false );
  return false;
}


//----------------------------------------------------------------------------------------
// Объединение контуров. Unit contours.
// ---
bool ContourUnite() 
{
  /*
    Yet not implemented.
  */
  PRECONDITION( false );
  return false;
}


//----------------------------------------------------------------------------------------
// Пересечение контуров. Cross contours.
// ---
bool ContourCross() 
{
  /*
    Yet not implemented.
  */
  PRECONDITION( false );
  return false;
}


//----------------------------------------------------------------------------------------
// Вычитание контуров.
// Substract operation for areas enclosed by contours.
// ---
bool ContourSubstr() 
{
  /*
    Yet not implemented.
  */
  PRECONDITION( false );
  return false;
}


//----------------------------------------------------------------------------------------
// Построение всех контуров.
// Building all contours.
// ---
bool CreateAllEmbracingContours() 
{
  bool res = false;

  PArray<MbCurve> curveArray( 10, 10, false );
  PArray<MbContour> contourArray( 10, 10, false );
  MbCartPoint points[1];
  MbItem *obj;

  if ( GetPoint(IDS_SELECT_OBJ_ON_PLANE, points[0]) &&
       FindNearestObj(points[0], pt_Curve, &obj) ) 
  {
    double accuracy( METRIC_ACCURACY );
    GetDouble( IDS_ENTER_ACCURACY, accuracy );
    MbPlacement3D place;
    obj->GetPlacement( place ); // Запоминаем ЛСК. Save the local coordinate frame.
    TestVariables::viewManager->CreateCurveArray( obj, curveArray, true );
    ::AddRefItems( curveArray );

    graph.reset( ::OuterContoursBuilder( curveArray, contourArray, accuracy, false, Math::DefaultMathVersion() ) ); 

    size_t count = contourArray.Count();
    if ( contourArray.Count() > 0 ) {
      Style style( 2, RGB2uint32(0.0, 0.5, 1.0) );
      for ( size_t i = 0; i < count; ++i ) {
        TestVariables::viewManager->AddObject( style, contourArray[i], &place );
      }
      res = true;
    }
    ::ReleaseItems( curveArray );
  }

  return res;
}


//----------------------------------------------------------------------------------------
// Собрать множество замкнутых контуров.
// Collect a set of closed contours. An auxiliary subroutine.
// ---
static
bool GetRegionsContours( const MbPlaneItem & planeItem
                       , const MbMatrix & mtr
                       , RPArray<MbContour> & contours
                       , bool & isRegion )
{
  bool res = false;
  isRegion = false;

  MbePlaneType type = planeItem.Type();

  if ( type == pt_Region ) 
  {
    const MbRegion & region = static_cast<const MbRegion &>( planeItem );
    const size_t contoursCnt = region.GetContoursCount();
    if ( contoursCnt > 0 ) {
      for ( size_t k = 0; k < contoursCnt; k++ ) 
      {
        if ( region.GetContour( k ) != nullptr ) {
          MbContour * contour = static_cast<MbContour *>( &region.GetContour( k )->Duplicate() );
          contours.push_back( contour );
          if ( !mtr.IsSingle() )
            contour->Transform( mtr );
          isRegion = true;
          res = true;
        }
      }
    }
  }
  else if ( type == pt_Contour ) 
  {
    const MbContour & initContour = static_cast<const MbContour &>( planeItem );

    if ( initContour.IsClosed() ) 
    {
      //MbContour * contour = ::DuplicateContour( initContour, true ); // Duplicate and simplify
      MbContour * contour = static_cast<MbContour *>( &initContour.Duplicate() );

      contours.push_back( contour );
      if ( !mtr.IsSingle() )
        contour->Transform( mtr );
      res = true;
    }
    else {
      MessageBoxEx( RT_MUST_BE_CLOSED, nullptr );
    }
  }
  else if ( type == pt_Curve )
  {
    const MbCurve & initCurve = static_cast<const MbCurve &>(planeItem);

    if ( initCurve.IsClosed() )
    {
      MbContour * contour = new MbContour;
      contour->SegmentsAdd( static_cast<MbCurve &>( initCurve.Duplicate() ) );
      contour->CheckClosed();
      contours.push_back( contour );
      if ( !mtr.IsSingle() )
        contour->Transform( mtr );
      res = true;
    }
    else {
      MessageBoxEx( RT_MUST_BE_CLOSED, nullptr );
    }
  }

  return res;
}


//----------------------------------------------------------------------------------------
// Собрать множество замкнутых контуров.
// Collect a set of closed contours. An auxiliary subroutine.
// ---
static
bool GetRegionsContours( RPArray<MbContour> & contours, DPtr<MbPlacement3D> & place )
{
  size_t cnt0 = contours.size();

  MbItem * gObj = nullptr;
  while ( GetMObj( IDS_SELECT_CNTR_ON_PLANE, pt_PlaneItem, &gObj ) ) 
  {
    if ( gObj->IsA() == st_PlaneInstance ) 
    {
      MbPlaneInstance * planeInst = static_cast<MbPlaneInstance *>(gObj);
      size_t planeItemsCnt = planeInst->PlaneItemsCount();

      TestVariables::viewManager->ToggleObject( gObj );

      MbMatrix mtr;
      if ( place == nullptr )
        place = new MbPlacement3D( planeInst->GetPlacement() );
      else
        planeInst->GetPlacement().GetMatrixToPlace( *place, mtr );

      bool isRegion = false;

      for ( size_t k = 0; k < planeItemsCnt; k++ ) 
      {
        MbPlaneItem * planeItem = planeInst->SetPlaneItem( k );
        if ( planeItem != nullptr )
        {
          ::GetRegionsContours( *planeItem, mtr, contours, isRegion );
        }
      }

      if ( contours.size() <= cnt0 )
        TestVariables::viewManager->ToggleObject( gObj );
      else if ( isRegion && (planeItemsCnt == 1) ) {
        break;
      }
    }
    else if ( gObj->IsA() == st_SpaceInstance ) 
    {
      TestVariables::viewManager->ToggleObject( gObj );

      MbPlacement3D contourPlace;
      SPtr<MbContour> contour( ::GetContourOnPlace( gObj, contourPlace ) );
      contour = ::DuplicateIfUsed( contour );

      if ( contour != nullptr )
      {
        MbMatrix mtr;
        if ( place == nullptr )
          place = new MbPlacement3D( contourPlace );
        else {
          contourPlace.GetMatrixToPlace( *place, mtr );
          contour->Transform( mtr );
        }
        contours.Add( ::DetachItem( contour ) );
      }
    }
  }

  return contours.size() > cnt0;
}


//----------------------------------------------------------------------------------------
// Выполнение булевой операции над регионами
// Perform the boolean operations on the 2D-regions enclosed by contours.
// ---
bool RegionsBooleanOper()
{
  bool res = false;
  DPtr<MbPlacement3D> place;

  RPArray<MbContour> contList1( 0, 1 );
  if ( ::GetRegionsContours( contList1, place ) && (place != nullptr) )
  {
    RPArray<MbContour> contList2( 0, 1 );
    if ( ::GetRegionsContours( contList2, place ) && (place != nullptr) )
    {
      ptrdiff_t operType = 0;
      if ( ::GetInt(IDS_ENTER_BOOLEANOPERTYPE, operType) ) 
      {
        RegionOperationType boolSide;
        switch ( operType ) {
          case 0  : boolSide = rbo_Union;      break;
          case 1  : boolSide = rbo_Intersect;  break;
          case 2  : boolSide = rbo_Difference; break;
          default : boolSide = rbo_Unknown;    break;
        }
        RPArray<MbRegion> regions( 0, 1 );
        // Вычисляем результирующие регионы.
        // Calculate the resulting regions.
        MbResultType resInfo = rt_Error;
        bool allowSelfTouch = true;
        bool mergeCurves = true;
        MbRegionBooleanParams params( boolSide, allowSelfTouch, mergeCurves, Math::LengthEps, Math::LengthEps, Math::AngleEps );
        if ( ::CreateBooleanResultRegions( contList1, contList2, params, regions, &resInfo ) ) 
        {
          TestVariables::viewManager->DeleteModel();
          for ( size_t k = 0, kcnt = regions.size(); k < kcnt; k++ ) 
          {            
            if ( MbRegion * region = regions[k] ) 
            {
              TestVariables::viewManager->AddObject( TestVariables::BOOLEAN_Style, region, place );
              // C3D-227
              //for ( size_t j = 0, cntrC = region->GetContoursCount(); j < cntrC; ++j ) 
              //  TestVariables::viewManager->AddObject( TestVariables::CONTOUR_Style, region->SetContour(j), place );
            }
          }
          res = true;
        }
        else {
          if ( resInfo == rt_SelfIntersect )
            ::MessageBoxEx( RT_SELFINTERSECT, IDS_TITLE_ERR );
          else
            ::MessageBoxEx( RT_BOOLEANERROR, IDS_TITLE_ERR );
        }
        ::DeleteItems( regions );
      }      
    }
    ::DeleteItems( contList2 );
  }
  ::DeleteItems( contList1 );

  return res;
}


//----------------------------------------------------------------------------------------
// Объединение регионов (контуров)
// ---
bool MakeUnionRegions()
{
  bool res = false;
  MbPlacement3D contoursPlace;

  RPArray<MbContour> contours;
  std::vector<SPtr<MbRegion>> regions;

  { // собираем селектированные контура
    RPArray<MbItem> selItems;
    TestVariables::viewManager->GetSelectedObjects( selItems );
  
    if ( selItems.size() > 0 ) {
      MbItem * contourItem0 = nullptr;
      MbPlacement3D cntrPlace;
      for ( size_t i = 0, selCnt = selItems.size(); i < selCnt; ++i ) {
        const MbItem * selItem = selItems[i];

        MbRegion * region = nullptr;

        if ( selItem->IsA() == st_PlaneInstance ) {
          const MbPlaneInstance * planeInst = static_cast<const MbPlaneInstance *>(selItem);
          const MbPlaneItem * planeItem = planeInst->GetPlaneItem();
          if ( planeItem->Family() == pt_Region ) {
            const MbRegion * region0 = static_cast<const MbRegion *>(planeItem);
            region = &static_cast<MbRegion &>( region0->Duplicate() );
            cntrPlace = planeInst->GetPlacement();

            if ( contourItem0 != nullptr ) {
              if ( contoursPlace.Colinear( cntrPlace ) ) {
                MbMatrix mtr;
                if ( cntrPlace.GetMatrixToPlace( contoursPlace, mtr ) && !mtr.IsSingle() )
                  region->Transform( mtr );
              }
            }
            else {
              contoursPlace.Init( cntrPlace );
              contourItem0 = selItems[i];
            }
          }
        }

        if ( region != nullptr ) {
          regions.emplace_back( region );
        }
        else {
          MbContour * contour = GetContourOnPlace( selItem, cntrPlace );
          if ( contour != nullptr ) {
            contours.push_back( contour );
            if ( contourItem0 != nullptr ) {
              if ( contoursPlace.Colinear( cntrPlace ) ) {
                MbMatrix mtr;
                if ( cntrPlace.GetMatrixToPlace( contoursPlace, mtr ) && !mtr.IsSingle() )
                  contour->Transform( mtr );
              }
            }
            else {
              contoursPlace.Init( cntrPlace );
              contourItem0 = selItems[i];
            }
          }
        }
      }
    }
  }

  ::AddRefItems( contours );

  if ( (regions.size() > 0) || (contours.size() > 0) ) {
    MbAccurateTimer ac;

    ac.Begin();

    RPArray<MbRegion> contoursRegions;
    ::GetCorrectRegions( contours, true, contoursRegions );
    regions.insert( regions.end(), contoursRegions.begin(), contoursRegions.end() );

    bool allowSelfTouch = true;
    bool allowMergeCurves = true;
    MbRegionBooleanParams unionParams( RegionOperationType::rbo_Union, allowSelfTouch, allowMergeCurves, Math::LengthEps, Math::LengthEps, Math::AngleEps );
    res = ::MakeUnionRegions( regions, unionParams );

    ac.End();

    if ( res ) {
      if ( WorkWindow * activeWindow = TestVariables::viewManager->GetActiveWindow() )
        activeWindow->DeleteSelected();
      for ( size_t k = 0, regLast = regions.size()-1; k < regLast; ++k ) {
        TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, regions[k], &contoursPlace, false );
      }
      TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, regions.back(), &contoursPlace, true );
    }

    if ( TestVariables::checkOperationTime ) {
      const size_t bufLen = 1024;
      TCHAR buf[bufLen];
      size_t len = 0;
      len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
      ::MessageBoxEx( buf, IDS_RESULTS );
    }
  }

  ::ReleaseItems( contours );

  return res;
}


//----------------------------------------------------------------------------------------
// Триангуляция региона.
// Triangulate a region.
// ---
bool TriangulateRegion()
{
  bool res = false;

  DPtr<MbPlacement3D> place;
  RPArray<MbContour> contList( 0, 1 );

  if ( ::GetRegionsContours( contList, place ) && (place != nullptr) )
  {
    double sag = Math::deviateSag;
    if ( ::GetDouble( IDS_ENTER_NUMBER, sag ) ) 
    {
      SPtr<MbRegion> region ( new MbRegion( contList, false/*sameContour*/ ) );
      if ( region->SetCorrect() ) 
      {
        MbPlanarGrid grid;
        ::TriangulateRegion( *region, sag, grid );
        TestVariables::drawImp->PutPlanarGrid( grid, *place, TRGB_LIGHTRED );
        res = true;
      }
      else
        MessageBoxEx( RT_INCORRECT_REGION, IDS_TITLE_ERR );
    }
  }
  ::DeleteItems( contList );

  return res;
}


//----------------------------------------------------------------------------------------
// Собрать множество всех замкнутых двумерных контуров модели
// Collect a set of all closed 2D contours. An auxiliary subroutine.
// ---
static bool GetAllContoursOnPlace( RPArray<MbContour> & contours
                                 , DPtr<MbPlacement3D> & place
                                 , RPArray<MbItem> & contoursOwners )
{  
  if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() )
  {
    MbModel & model = wind->SetGeomModel();
    RPArray<MbItem> modelItems( 0, 1 );
    model.GetItems( modelItems );

    for ( ptrdiff_t i = modelItems.MaxIndex(); i >= 0; --i ) 
    {
      MbItem * modelItem = modelItems[i];
      if ( modelItem->IsA() == st_PlaneInstance ) 
      {
        MbPlaneInstance * planeInst = static_cast<MbPlaneInstance *>(modelItem);

        const MbPlacement3D & instPlace = planeInst->GetPlacement();
        size_t itemsCnt = planeInst->PlaneItemsCount();

        if ( itemsCnt > 0 ) {
          for ( size_t k = 0; k < itemsCnt; ++k ) {
            MbPlaneItem * planeItem = planeInst->SetPlaneItem( k );
            if ( planeItem != nullptr && planeItem->Type() == pt_Contour )
            {
              TestVariables::viewManager->ToggleObject( modelItem );

              MbContour * contour = static_cast<MbContour *>(planeItem);
              if ( contour->IsClosed() ) 
              {
                bool addContour = false;
                if ( place == nullptr ) {
                  place = new MbPlacement3D( instPlace );
                  addContour = true;
                }
                else if ( place->GetAxisZ().Colinear( instPlace.GetAxisZ() ) )
                  addContour = true;
                else {
                  PRECONDITION( false ); // The other Z-axis
                }
                if ( addContour ) {
                  contours.push_back( contour );
                  if ( contoursOwners.FindIt( modelItem ) == SYS_MAX_T )
                    contoursOwners.Add( modelItem );
                }
              }
              else {
                ::MessageBoxEx( RT_MUST_BE_CLOSED, IDS_INFORMATION );
              }

              TestVariables::viewManager->ToggleObject( modelItem );
            }
          }
        }
      }
      else if ( modelItem->IsA() == st_SpaceInstance ) {
        MbSpaceInstance * spaceInst = static_cast<MbSpaceInstance *>( modelItem );
        MbSpaceItem * spaceItem = spaceInst->SetSpaceItem();

        if ( spaceItem != nullptr ) {
          MbeSpaceType itemFamily = spaceItem->Family();
          if ( itemFamily == st_Curve3D ) {
            MbCurve3D * spaceCurve = static_cast<MbCurve3D *>( spaceItem );

            bool addContour = false;
            MbCurve * curve = nullptr;
            MbPlacement3D curvePlace;
            if ( spaceCurve->GetPlaneCurve( curve, curvePlace, false ) ) {
              if ( curve->IsClosed() ) {
                if ( place == nullptr ) {
                  place = new MbPlacement3D( curvePlace );
                  addContour = true;
                }
                else if ( place->GetAxisZ().Colinear( curvePlace.GetAxisZ() ) )
                  addContour = true;
                else {
                  PRECONDITION( false ); // The other Z-axis
                }

                if ( curve->Type() == pt_Contour )
                  contours.push_back( static_cast<MbContour *>(curve) );
                else {
                  curve = new MbContour( *curve, true );
                  contours.push_back( static_cast<MbContour *>(curve) );
                }
              }
              else {
                ::MessageBoxEx( RT_MUST_BE_CLOSED, IDS_INFORMATION );
              }
            }
            if ( addContour ) {
              if ( contoursOwners.FindIt( modelItem ) == SYS_MAX_T )
                contoursOwners.Add( modelItem );
            }
            else {
              ::DeleteItem( curve );
            }
          }
        }
      }
    }
  }
  return contours.size() > 0;
}


//----------------------------------------------------------------------------------------
/*
  Разбиение произвольного набора контуров на связные группы контуров.
  Все контуры модели должны находиться на одной плоскости.

  Split an arbitrary set of contours on the connected groups.
  All contours of the model should be on same plane.
*/
// ---
bool MakeAllRegions()
{
  bool res = false;
  RPArray<MbContour> contList( 0, 1 );
  RPArray<MbItem> contoursOwners( 0, 1 );
  DPtr<MbPlacement3D> place;
  if ( ::GetAllContoursOnPlace( contList, place, contoursOwners ) ) 
  {
    ::AddRefItems( contList );
    {
      for ( size_t i = contoursOwners.Count(); i--; )
        TestVariables::viewManager->DeleteObject( contoursOwners[i] );
    }

    RPArray<MbRegion> regions( 0, 1 );
    // Вычисляем результирующие регионы. 
    // Calculate the resulting region.
    ::MakeRegions( contList, true/*useSelfIntCntrs*/, false/*sameContours*/, regions );

    DPtr<MbPlacement3D> placement;
    if ( place != nullptr )
      placement = new MbPlacement3D( place->GetOrigin() );

    Style style = TestVariables::CONTOUR_Style;
    uint r = 0, g = 0, b = 0;
    ::srand( 1 );
    for ( size_t i = 0, regC = regions.Count(); i < regC; i++ ) 
    {      
      if ( MbRegion * region = regions[i] )
      {
        // Определяем цвет для данной группы.
        // Define color of the group.
        r = ::rand() % 256;
        g = ::rand() % 256;
        b = ::rand() % 256;
        style.SetColor( RGB( r, g, b ) );

        if ( region->SetCorrect() )
          TestVariables::viewManager->AddObject( style, region, placement );
        else {
          for ( ptrdiff_t j = 0, cntrC = region->GetContoursCount(); j < cntrC; j++ )
            TestVariables::viewManager->AddObject( style, region->SetContour(j), placement );
        }
      }
    }
    ::DeleteItems( regions );

    res = true;
    ::ReleaseItems( contList );
  }

  return res;
}
