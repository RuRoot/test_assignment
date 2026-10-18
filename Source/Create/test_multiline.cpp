////////////////////////////////////////////////////////////////////////////////
//
// Создание плоской мультилинии
// Creation of plane multiline
//
////////////////////////////////////////////////////////////////////////////////

#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <test_draw.h>
#include <multiline.h>
#include <cur_line_segment.h>
#include <cur_arc.h>
#include <cur_polyline.h>
#include <action_curve.h>
#include <plane_instance.h>
#include <last.h>

using namespace c3d;


//-------------------------------------------------------------------------------
/// Покомпонентная отрисовка мультилинии
/// Draw multiline by components
// ---
void DrawMultiline( const MbMultiline & multiline, MbPlacement3D & placement )
{
  MbItem * gobj;
  size_t i, count;
  uint32 color;

  gobj = TestVariables::viewManager->AddObject( TestVariables::CONTOUR_Style, (MbContour*)&multiline.GetBasisCurve(), &placement );
  if ( gobj != nullptr )
    gobj->SetColor( RGB(192, 0, 0) );                          // Базовую кривую отрисовываем красным
  
  color = RGB( 0, 0, 192 );                                    // Линии отрисовываем синим
  for ( i = 0, count = multiline.GetCurvesCount(); i < count; i++ ) {
    gobj = TestVariables::viewManager->AddObject( TestVariables::CONTOUR_Style, (MbContour*)multiline.GetCurve(i), &placement );
    if ( gobj != nullptr )
      gobj->SetColor( color );
  }

  color = RGB( 0, 128, 0 );                                    // Внутренние законцовки отрисовываем зеленым
  for ( i = 0, count = multiline.GetTipCurvesCount(); i < count; i++ ) {
    gobj = TestVariables::viewManager->AddObject( TestVariables::CONTOUR_Style, (MbContour*)multiline.GetTipCurve(i), &placement );
    if ( gobj != nullptr ) {
      gobj->SetColor( color );
      gobj->SetWidth( 2 * gobj->GetWidth() );                  // Делаем в два раза шире
    }
  }

  color = RGB( 255, 0, 0 );                                    // Внешние законцовки отрисовываем светло-красным
  gobj = TestVariables::viewManager->AddObject( TestVariables::CONTOUR_Style, (MbContour*)multiline.GetBegTipCurve(), &placement );
  if ( gobj != nullptr ) {
    gobj->SetColor( color );
    gobj->SetWidth( 2 * gobj->GetWidth() );                    // Делаем в два раза шире
  }
  gobj = TestVariables::viewManager->AddObject( TestVariables::CONTOUR_Style, (MbContour*)multiline.GetEndTipCurve(), &placement );
  if ( gobj != nullptr ) {
    gobj->SetColor( color );
    gobj->SetWidth( 2 * gobj->GetWidth() );                    // Делаем в два раза шире
  }
}


//-------------------------------------------------------------------------------
/// Задание параметров вершины мультилинии
/// Set of parameters of a vertex of a multiline
// ---
static
void SetMltLinesVertexInfo( MbMultiline & multiline, size_t i, MbItem & mltLineObj )
{    
  // Ищем точку, соответствующую данной вершине мультилинии
  MbCartPoint vertex;
  multiline.GetBasisCurve().GetSegment( i )->GetEndPoint( vertex );
  Style vertStyle( 5, TestVariables::RED ); // Стиль вершины мультилинии
  MbItem * vert = TestVariables::viewManager->AddObject( vertStyle, /*new MbPoint(*/vertex );

  int smooth = 0;
  if ( ::GetInt(IDS_ENTER_SMOOTH_JOINT, smooth) ) {
    bool smoothJoint = smooth > 0;
    multiline.SetSmoothJoint( i, smoothJoint );
    TestVariables::viewManager->RefreshObjectModel( &mltLineObj );
  }

  int type = 0;
  EnMLVertexTracingType tracingType = mvt_ShearType;
  if ( ::GetInt(IDS_ENTER_TRACING_TYPE, type) ) {
    switch ( type ) {
      case 0  : tracingType = mvt_ShearType;      break;
      case 1  : tracingType = mvt_FilletType;     break;
      case 2  : tracingType = mvt_SpecFilletType; break;
      default : tracingType = mvt_ShearType;      break;
    }
  }

  if ( tracingType == mvt_ShearType ) {                      // При обходе срезом
    type = 0;
    EnMLInnerTipType inTipType = mit_UndefTip;
    if ( ::GetInt(IDS_ENTER_IN_TIP_TYPE, type) ) {           // Получаем параметры законцовки в вершине
      switch ( type ) {
        case 0  : inTipType = mit_UndefTip;    break;
        case 1  : inTipType = mit_VerticesTip; break;
        case 2  : inTipType = mit_LinearTip;   break;
        case 3  : inTipType = mit_ArcTip;      break;
        default : inTipType = mit_UndefTip;    break;
      }
    }

    if ( inTipType == mit_LinearTip ||
         inTipType == mit_ArcTip ) {                         // При линейной и дуговой законцовках                                                
      int sense = 1;
      if ( ::GetInt(IDS_ENTER_SENSE, sense) ) {              // Получаем направление законцовки
        bool firstSegTip = sense > 0;
        multiline.SetTipDirection( i, firstSegTip );
        TestVariables::viewManager->RefreshObjectModel( &mltLineObj );
      }
    }

    multiline.SetTipType( i, inTipType );
    TestVariables::viewManager->RefreshObjectModel( &mltLineObj );
  }
  else
  if ( tracingType == mvt_SpecFilletType ) {
    double radius = 10.0;
    if ( ::GetDouble(IDS_ENTER_RADIUS, radius) ) {           // Получаем радиус специального скругления
      multiline.SetSpecFilletRad( i, radius );
      TestVariables::viewManager->RefreshObjectModel( &mltLineObj );
    }
    else
      tracingType = mvt_ShearType;
  }

  multiline.SetTracingType( i, tracingType );
  TestVariables::viewManager->RefreshObjectModel( &mltLineObj );
  TestVariables::viewManager->DeleteObject( vert );
}


//-------------------------------------------------------------------------------
/// Получить параметры внешней законцовки мультилинии
/// Get parameters of multiline tip
// ---
void GetMltLinesTipParams( EnMLTipType & tipType, double & param, bool beg )
{
  int type = 0;
  tipType = mtt_UndefTip;
  if ( ::GetInt(IDS_ENTER_OUT_TIP_TYPE, type) ) {              // Получаем тип законцовки
    switch ( type ) {
      case 0  : tipType = mtt_UndefTip;    break;
      case 1  : tipType = mtt_LinearTip;   break;
      case 2  : tipType = mtt_ArcTip;      break;
      case 3  : tipType = mtt_PolylineTip; break;
      case 4  : tipType = mtt_ObliqueTip;  break;
      default : tipType = mtt_UndefTip;    break;
    }
  }

  param = 0.0;
  if ( tipType != mtt_UndefTip ) {                             // При наличии законцовки
    if ( beg )
      ::GetDouble( IDS_ENTER_INITIAL_PRM, param );             // Получаем параметр
    else
      ::GetDouble( IDS_ENTER_FINAL_PRM,   param );
  }
}


//-------------------------------------------------------------------------------
/// Создание мультилинии на базе контура
/// Create a multiline by contour
// ---
bool CreateCntrsMultiline()
{
  bool res = false;
  MbItem * obj;
  if ( ::GetMObj(IDS_SELECT_CNTR_ON_PLANE, pt_Contour, &obj) ) { // Получаем базовый контур
    TestVariables::viewManager->ToggleObject( obj );    

    MbPlacement3D pl;
    SPtr<MbContour> contour( ::GetContourOnPlace( obj, pl ) );

    if ( contour != nullptr ) {
      MbPlacement3D placement( pl );

      SArray<double> radiuses( 0, 1 );
      double radius = 10.0;
      while ( ::GetDouble(IDS_ENTER_RADIUS, radius) ) {            // Получаем радиусы линий
        radiuses.Add( radius );
        radius *= 2;                                               // Умолчательное значение
      }

      EnMLTipType tipType;
      double param;    
      ::GetMltLinesTipParams( tipType, param, true  );             // Получаем параметры законцовки в начале
      StMLTipParams begTipParams( tipType, param );
      ::GetMltLinesTipParams( tipType, param, false );             // Получаем параметры законцовки в конце
      StMLTipParams endTipParams( tipType, param );

      MbMultiline * multiline = new MbMultiline( *contour, StVertexOfMultilineInfo(), radiuses, 
                                                 begTipParams, endTipParams,
                                                 false/*_processClosed*/, false/*_isTransparent*/ );
      // Кладем мультилинию
      MbItem * mltLineObj = TestVariables::viewManager->AddObject( TestVariables::CONTOUR_Style, multiline, &placement );
      if ( mltLineObj ) {
        // Выставляем параметры вершин
        for ( ptrdiff_t i = 0, count = multiline->GetVerticesCount(); i < count; i++ ) {
          ::SetMltLinesVertexInfo( *multiline, i, *mltLineObj );
        }
      }
    }

    TestVariables::viewManager->ToggleObject( obj );
    obj->SetColor( RGB(192, 192, 192) );                         // Исходный контур отрисовываем светло серым
  }

  return res;
}


//-------------------------------------------------------------------------------
/// Трансформировать из старого плейсмента в новый
/// Transform from old placement to new
// ---
static void Transform( const MbPlacement3D & oldPlace, 
                       const MbPlacement3D & newPlace, 
                             MbCartPoint &   point )
{
  MbMatrix toNewPlace;
  oldPlace.GetMatrixToPlace( newPlace, toNewPlace );
  point.Transform( toNewPlace );
}


//-------------------------------------------------------------------------------
/// Создание мультилинии построением базовой кривой
/// Create a multiline by constructing a base curve
// ---
bool CreateMultiline()
{
  SArray<double> radiuses( 0, 1 );
  double radius = 10.0;
  while ( ::GetDouble(IDS_ENTER_RADIUS, radius) ) {   // Получаем радиусы линий
    radiuses.Add( radius );
    radius *= 2;                                      // Умолчательное значение
  }
  if ( radiuses.Count() ) {
    MbPlacement3D pl( TestVariables::viewManager->GetActiveWindow()->GetOwnPlace() );
    // Создаем мультилинию с пустой базовой кривой
    MbMultiline * multiline = new MbMultiline();
    // Кладем радиусы эквидистант
    for ( ptrdiff_t i = 0, count = radiuses.Count(); i < count; i++ )
      multiline->AddRadius( radiuses[i] );

    MbItem * mltLineObj  = TestVariables::viewManager->AddObject( TestVariables::CONTOUR_Style, multiline, &pl );
    const MbPlacement3D & mltLinePlace = static_cast<MbPlaneInstance *>( mltLineObj )->GetPlacement();

    Style basisCrvStyle( 2, TestVariables::RED );
    MbItem * basisCrvObj = TestVariables::viewManager->AddObject( basisCrvStyle, (MbContour*)&multiline->GetBasisCurve(), nullptr );
    MbCartPoint firstPoint;

    // Получаем начальную точку базовой кривой
    if ( ::GetPoint(IDS_SELECT_START_POINT, firstPoint) ) {
      TestVariables::drawImp->DrawPoint( &firstPoint, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );

      MbCartPoint currPoint( firstPoint );
      StVertexOfMultilineInfo vertInfo;               // С умолчательными параметрами
      SArray<MbCartPoint> pList;
      MbCartPoint pt[3];
      ptrdiff_t deg = Math::curveDegree;
      bool isOk;
      ptrdiff_t res = IDC_PUSHBUTTON_SPLINE;

      do {
        res = ExecuteContourDlg();
        switch ( res ) {
          case IDC_PUSHBUTTON_SPLINE : {  // Сплайн
            pList.push_back( currPoint );
            while ( ::GetPoint(IDS_SELECT_POINT, currPoint) ) {
              TestVariables::drawImp->DrawPoint( &currPoint, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() ); 
              ::Transform( pl, mltLinePlace, currPoint );
              pList.push_back( currPoint );
            }

            if ( ((ptrdiff_t)pList.size()) >= deg ) {
              SArray<double> knots; 
              SArray<double> wList;
              MbCurve * seg = nullptr;
              ::NurbsCurve( pList, wList, deg, knots, false, seg );
              if ( multiline->AddBasisSegment(seg, vertInfo) ) {
                TestVariables::viewManager->RefreshObjectModel( mltLineObj  );
                TestVariables::viewManager->RefreshObjectModel( basisCrvObj );
              }
              else
                delete seg;
            }
            pList.clear();
          }
          break;

          case IDC_PUSHBUTTON_SEGMENT : {  // Отрезок прямой
            pt[0] = currPoint;
            if ( ::GetPoint(IDS_SELECT_POINT, currPoint) ) {
              TestVariables::drawImp->DrawPoint( &currPoint, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
              ::Transform( pl, mltLinePlace, currPoint );
              pt[1] = currPoint;

              MbLineSegment * seg = new MbLineSegment( pt[0], pt[1] );
              if ( multiline->AddBasisSegment(seg, vertInfo) ) {
                TestVariables::viewManager->RefreshObjectModel( mltLineObj  );
                TestVariables::viewManager->RefreshObjectModel( basisCrvObj );
              }
              else
                delete seg;
            }
          }
          break;

          case IDC_PUSHBUTTON_POLYLINE : {  // Полилиния
            pList.push_back( currPoint );
            while ( ::GetPoint(IDS_SELECT_POINT, currPoint) ) {
              TestVariables::drawImp->DrawPoint( &currPoint, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
              ::Transform( pl, mltLinePlace, currPoint );
              pList.push_back( currPoint );
            }

            if ( pList.size() > 1 ) {
              MbCurve * seg = nullptr;
              ::SplineCurve( pList, false, pt_Polyline, seg ); 
              if ( multiline->AddBasisSegment(seg, vertInfo) ) {
                TestVariables::viewManager->RefreshObjectModel( mltLineObj  );
                TestVariables::viewManager->RefreshObjectModel( basisCrvObj );
              }
              ::DeleteItem( seg );
            }
            pList.clear();
          }
          break;

          case IDC_PUSHBUTTON_ARC : {  // Дуга
            pt[0] = currPoint;
            isOk = true;
            for ( ptrdiff_t j = 1; j < 3; j++ ) {
              if ( ::GetPoint(IDS_SELECT_POINT, currPoint) ) {
                TestVariables::drawImp->DrawPoint( &currPoint, TRGB_LIGHTGREEN, TestVariables::viewManager->GetCplaneMatrixFrom() );
                ::Transform( pl, mltLinePlace, currPoint );
                pt[j] = currPoint;
              }
              else
                isOk = false;
            }
            if ( isOk ) {
              MbArc * seg = MbArc::Create( pt[0], pt[1], pt[2] );
              if ( multiline->AddBasisSegment(seg, vertInfo) ) {
                TestVariables::viewManager->RefreshObjectModel( mltLineObj  );
                TestVariables::viewManager->RefreshObjectModel( basisCrvObj );
              }
              else if ( seg != nullptr )
                delete seg;
            }
          }
          break;

          case IDC_PUSHBUTTON_CLOSE : {  // Замкнуть
            if ( multiline->GetBasisCurve().GetSegmentsCount() > 1 ) {
              MbLineSegment * seg = new MbLineSegment( currPoint, firstPoint );
              currPoint = firstPoint;
              if ( multiline->AddBasisSegment(seg, vertInfo) ) {
                TestVariables::viewManager->RefreshObjectModel( mltLineObj  );
                TestVariables::viewManager->RefreshObjectModel( basisCrvObj );
              }
              else
                delete seg;
            }
          }
          break;

         default :
         break;
        }
      } while ( res != IDC_PUSHBUTTON_EXIT );
    }

    TestVariables::viewManager->DeleteObject( basisCrvObj );

    if ( multiline->GetBasisCurve().GetSegmentsCount() )
      return true;
    else
      TestVariables::viewManager->DeleteObject( mltLineObj );
  }

  return false;
}


//-------------------------------------------------------------------------------
/// Создание контура с разрывами на базе контура
/// Create a contour with breaks based on a contours
// ---
bool CreateContourWithBreaks()
{
  bool res = false;

  MbItem * obj;
  if ( ::GetMObj(IDS_SELECT_CNTR_ON_PLANE, pt_Contour, &obj) ) // Получаем базовый контур
  { 
    if( obj )
    {    
      MbContour * contour = (MbContour*)((MbPlaneInstance*)obj)->GetPlaneItem();
      MbPlacement3D * placement = new MbPlacement3D( ((MbPlaneInstance *)obj)->GetPlacement().GetOrigin() );

      // Создать 
      MbContourWithBreaks * contourWBr = contour ? new MbContourWithBreaks( *contour ) : 0;

      // Кладем контур
      /*MbItem * contWBrObj =*/
      TestVariables::viewManager->AddObject( TestVariables::CONTOUR_Style, contourWBr, placement );
    
      // Удалить исходный объект
      TestVariables::viewManager->DeleteObject( obj ); 
      delete placement;
    }
  }

  return res;
}


//-------------------------------------------------------------------------------
/// Добавить невидимый участок в контур с разрывами
/// Add an invisible part to a contour with breaks
// ---
bool AddBreakInContour()
{
  bool res = true;

  MbItem * obj;
  MbCartPoint pnt[3];
  // Указать контур с разрывами
  // Указать три точки: первые две - интервал разрыва, тертья - часть, которую надо удалить (для замкнутого контура)

  if ( ::GetMObj ( IDS_SELECT_CNTR_ON_PLANE, pt_ContourWithBreaks, &obj ) && // Получаем базовый контур
       ::GetPoint( IDS_SELECT_POINT, pnt[0] ) &&
       ::GetPoint( IDS_SELECT_POINT, pnt[1] ) &&
       ::GetPoint( IDS_SELECT_POINT, pnt[2] ) ) 
  { 
    if( obj && pnt[0] != pnt[1] && pnt[1] != pnt[2] )
    {
      MbContourWithBreaks * contour = (MbContourWithBreaks*)( ((MbPlaneInstance*)obj)->GetPlaneItem() );
      contour->AddBreak( pnt[0], pnt[1], pnt[2] );
      TestVariables::viewManager->RefreshObjectModel( obj );
    }
  }


  return res;
}
