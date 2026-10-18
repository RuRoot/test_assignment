////////////////////////////////////////////////////////////////////////////////
//
// Edpmltln.cpp - редактирование плоской мультилинии
// Edpmltln.cpp - Edit of flat multiline
//
////////////////////////////////////////////////////////////////////////////////

#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <multiline.h>
#include <plane_instance.h>
#include <last.h>

using namespace c3d;


//-------------------------------------------------------------------------------
// Заменить базовую кривую мультилинии.
// Change the base curve of multiline.
// ---
bool ReplaceMltLinesBasisCurve()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    MbItem * cntrObj;
    if ( ::GetMObj(IDS_SELECT_CNTR_ON_PLANE, pt_Contour, &cntrObj) ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      MbContour * contour = (MbContour*)((MbPlaneInstance *)cntrObj)->GetPlaneItem();
      multiline->ReplaceBasisCurve( *contour, StVertexOfMultilineInfo() );
      TestVariables::viewManager->RefreshObjectModel( mltlnObj );
      res = true;
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Вставить вершину в мультилинию.
// Insert a vertex in multiline.
// ---
bool InsertMltLinesVertex()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    MbCartPoint pnt;
    if ( ::GetPoint(IDS_SELECT_POINT, pnt) ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      const MbContour & basisCurve = multiline->GetBasisCurve();
      double t = basisCurve.PointProjection( pnt );
      size_t newInd = multiline->InsertVertex( t, StVertexOfMultilineInfo() );
      if ( newInd != SYS_MAX_T ) {
        TestVariables::viewManager->RefreshObjectModel( mltlnObj );
        res = true;
        // Отрисовка результата.  // Drawing result.
        MbCartPoint newVert;
        basisCurve.GetSegment( newInd )->GetEndPoint( newVert );
        Style vertStyle( 5, TestVariables::RED ); // Стиль вершины мультилинии. // Style of the vertex of multiline.
        MbItem * vert = TestVariables::viewManager->AddObject( vertStyle, /*new MbPoint(*/newVert );
        TCHAR message[40];
        _sntprintf( message, 40, RT_INSERTED_VERTEX_IN_INDEX F_TD, newInd );
  
        MessageBoxEx( message, IDS_EDIT_RESULT );
        TestVariables::viewManager->DeleteObject( vert );        
      }
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Удалить вершину мультилинии.
// Remove the multiline vertex.
// ---
bool RemoveMltLinesVertex()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    ptrdiff_t ind = 0;
    if ( ::GetInt(IDS_ENTER_POINT_INDEX, ind) && ind >= 0 ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      if ( multiline->RemoveVertex(ind) ) {
        TestVariables::viewManager->RefreshObjectModel( mltlnObj );
        res = true;
      }
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Сместить hot-точку базовой кривой мультилинии.
// Move the hot-point of the base curve of multiline.
// ---
bool SetMltLinesHotPoint()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    ptrdiff_t segInd = 0;
    if ( ::GetInt(IDS_ENTER_SEGMENT_INDEX, segInd) && segInd >= 0 ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      const MbContour & basisCurve = multiline->GetBasisCurve();
      ptrdiff_t segmentsC = basisCurve.GetSegmentsCount();
      if ( segInd <= segmentsC ) {
        MbItem * seg = nullptr;
        if ( segInd < segmentsC ) {
          MbCurve * segment = static_cast<MbCurve *>( &basisCurve.GetSegment( segInd )->Duplicate() );
          seg = TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, segment );
        }
        ptrdiff_t subInd = 0;
        if ( ::GetInt(IDS_ENTER_POINT_INDEX, subInd) && subInd >= 0 ) {
          MbCartPoint pnt;
          if ( ::GetPoint(IDS_SELECT_POINT, pnt) ) {
            if ( multiline->SetBasisCurvesGeoHotPoint(segInd, subInd, pnt) ) {
              TestVariables::viewManager->RefreshObjectModel( mltlnObj );
              res = true;
            }
          }
        }
        if ( seg != nullptr )
          TestVariables::viewManager->DeleteObject( seg );
      }
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Удалить hot-точку базовой кривой мультилинии.
// Remove the hot-point of the base curve of multiline.
// ---
bool DelMltLinesHotPoint()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    ptrdiff_t segInd = 0;
    if ( ::GetInt(IDS_ENTER_SEGMENT_INDEX, segInd) && segInd >= 0 ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      const MbContour & basisCurve = multiline->GetBasisCurve();
      ptrdiff_t segmentsC = basisCurve.GetSegmentsCount();
      if ( segInd <= segmentsC ) {
        MbItem * seg = nullptr;
        if ( segInd < segmentsC ) {
          MbCurve * segment = static_cast<MbCurve *>( &basisCurve.GetSegment( segInd )->Duplicate() );
          seg = TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, segment );
        }
        ptrdiff_t subInd = 0;
        if ( ::GetInt(IDS_ENTER_POINT_INDEX, subInd) && subInd >= 0 ) {
          if ( multiline->DelBasisCurvesGeoHotPoint(segInd, subInd) ) {
            TestVariables::viewManager->RefreshObjectModel( mltlnObj );
            res = true;
          }
        }
        if ( seg != nullptr )
          TestVariables::viewManager->DeleteObject( seg );
      }
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Выставить флаг гладкий стык в вершине мультилинии.
// Set the flag smooth joint at the multiline vertex.
// ---
bool SetMltLinesVertSmoothJoint()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    ptrdiff_t ind = 0;
    if ( ::GetInt(IDS_ENTER_POINT_INDEX, ind) && ind >= 0 ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      if ( ind < (ptrdiff_t)multiline->GetVerticesCount() ) {
        MbCartPoint newVert;
        multiline->GetBasisCurve().GetSegment( ind )->GetEndPoint( newVert );
        Style vertStyle( 5, TestVariables::RED ); // Стиль вершины мультилинии. // Style of the vertex of multiline.
        MbItem * vert = TestVariables::viewManager->AddObject( vertStyle, /*new MbPoint(*/newVert );
        int smooth = 0;
        if ( ::GetInt(IDS_ENTER_SMOOTH_JOINT, smooth) ) {
          multiline->SetSmoothJoint( ind, smooth > 0 );
          TestVariables::viewManager->RefreshObjectModel( mltlnObj );
          res = true;
        }
        TestVariables::viewManager->DeleteObject( vert );
      }
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Выставить тип обхода угла в вершине мультилинии.
// Set type of bypass of the angle at the vertex of multiline.
// ---
bool SetMltLinesVertTracingType()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    ptrdiff_t ind = 0;
    if ( ::GetInt(IDS_ENTER_POINT_INDEX, ind) && ind >= 0 ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      if ( ind < (ptrdiff_t)multiline->GetVerticesCount() ) {
        MbCartPoint newVert;
        multiline->GetBasisCurve().GetSegment( ind )->GetEndPoint( newVert );
        Style vertStyle( 5, TestVariables::RED ); // Стиль вершины мультилинии. // Style of the vertex of multiline.
        MbItem * vert = TestVariables::viewManager->AddObject( vertStyle, /*new MbPoint(*/newVert );
        EnMLVertexTracingType tracingType;
        int type = 0;
        if ( ::GetInt(IDS_ENTER_TRACING_TYPE, type) ) {
          switch ( type ) {
            case 0  : tracingType = mvt_ShearType;      break;
            case 1  : tracingType = mvt_FilletType;     break;
            case 2  : tracingType = mvt_SpecFilletType; break;
            default : tracingType = mvt_ShearType;      break;
          }
          multiline->SetTracingType( ind, tracingType );
          TestVariables::viewManager->RefreshObjectModel( mltlnObj );
          res = true;
        }
        TestVariables::viewManager->DeleteObject( vert );
      }
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Выставить радиус специального скругления в вершине мультилинии.
// Set radius of the special fillet at the vertex of multiline.
// ---
bool SetMltLinesVertSpecFilletRad()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    ptrdiff_t ind = 0;
    if ( ::GetInt(IDS_ENTER_POINT_INDEX, ind) && ind >= 0 ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      if ( ind < (ptrdiff_t)multiline->GetVerticesCount() ) {
        MbCartPoint newVert;
        multiline->GetBasisCurve().GetSegment( ind )->GetEndPoint( newVert );
        Style vertStyle( 5, TestVariables::RED ); // Стиль вершины мультилинии. // Style of the vertex of multiline.
        MbItem * vert = TestVariables::viewManager->AddObject( vertStyle, /*new MbPoint(*/newVert );
        double radius = 10.0;
        if ( ::GetDouble(IDS_ENTER_RADIUS, radius) ) {
          multiline->SetSpecFilletRad( ind, radius );
          TestVariables::viewManager->RefreshObjectModel( mltlnObj );
          res = true;
        }
        TestVariables::viewManager->DeleteObject( vert );
      }
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Выставить тип законцовки в вершине мультилинии.
// Set type of ending at the vertex of multiline.
// ---
bool SetMltLinesVertTipType()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    ptrdiff_t ind = 0;
    if ( ::GetInt(IDS_ENTER_POINT_INDEX, ind) && ind >= 0 ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      if ( ind < (ptrdiff_t)multiline->GetVerticesCount() ) {
        MbCartPoint newVert;
        multiline->GetBasisCurve().GetSegment( ind )->GetEndPoint( newVert );
        Style vertStyle( 5, TestVariables::RED ); // Стиль вершины мультилинии. // Style of the vertex of multiline.
        MbItem * vert = TestVariables::viewManager->AddObject( vertStyle, /*new MbPoint(*/newVert );
        EnMLInnerTipType inTipType;
        int type = 0;
        if ( ::GetInt(IDS_ENTER_IN_TIP_TYPE, type) ) {
          switch ( type ) {
            case 0  : inTipType = mit_UndefTip;    break;
            case 1  : inTipType = mit_VerticesTip; break;
            case 2  : inTipType = mit_LinearTip;   break;
            case 3  : inTipType = mit_ArcTip;      break;
            default : inTipType = mit_UndefTip;    break;
          }
          multiline->SetTipType( ind, inTipType );
          TestVariables::viewManager->RefreshObjectModel( mltlnObj );
          res = true;
        }
        TestVariables::viewManager->DeleteObject( vert );
      }
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Выставить направление законцовки в вершине мультилинии.
// Set direction of ending at the vertex of multiline.
// ---
bool SetMltLinesVertTipDirection()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    ptrdiff_t ind = 0;
    if ( ::GetInt(IDS_ENTER_POINT_INDEX, ind) && ind >= 0 ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      if ( ind < (ptrdiff_t)multiline->GetVerticesCount() ) {
        MbCartPoint newVert;
        multiline->GetBasisCurve().GetSegment( ind )->GetEndPoint( newVert );
        Style vertStyle( 5, TestVariables::RED ); // Стиль вершины мультилинии. // Style of the vertex of multiline.
        MbItem * vert = TestVariables::viewManager->AddObject( vertStyle, /*new MbPoint(*/newVert );
        int sense = 1;
        if ( ::GetInt(IDS_ENTER_SENSE, sense) ) {
          multiline->SetTipDirection( ind, sense > 0 );
          TestVariables::viewManager->RefreshObjectModel( mltlnObj );
          res = true;
        }
        TestVariables::viewManager->DeleteObject( vert );
      }
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Выставить радиус эквидистанты мультилинии.
// Set radius of the offset of multiline.
// ---
bool SetMltLinesRadius()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    ptrdiff_t ind = 0;
    if ( ::GetInt(IDS_ENTER_CURVE_INDEX, ind) && ind >= 0 ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      if ( ind < (ptrdiff_t)multiline->GetEquidRadiiCount() ) {
        const MbContour * curve = multiline->GetCurve( ind );
        MbItem * curveObj = TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, (MbContour*)curve );
        double radius = multiline->GetEquidRadius( ind );
        if ( ::GetDouble(IDS_ENTER_RADIUS, radius) ) {
          size_t newIndex = 0;
          multiline->SetRadius( ind, radius, newIndex );
          TestVariables::viewManager->RefreshObjectModel( mltlnObj );
          res = true;
        }
        TestVariables::viewManager->DeleteObject( curveObj );
      }
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Изменить радиус эквидистанты мультилинии.
// Change radius of the offset of multiline.
// ---
bool ChangeMltLinesRadius()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    double oldRad = 0.0, newRad = 0.0;
    if ( ::GetDouble(IDS_ENTER_RADIUS, oldRad) &&
         ::GetDouble(IDS_ENTER_RADIUS, newRad) ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      multiline->ChangeRadius( oldRad, newRad );
      TestVariables::viewManager->RefreshObjectModel( mltlnObj );
      res = true;
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Добавить радиус эквидистанты мультилинии.
// Add radius of the offset of multiline.
// ---
bool AddMltLinesRadius()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    double radius = 0.0;
    if ( ::GetDouble(IDS_ENTER_RADIUS, radius) ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      size_t newInd = multiline->AddRadius( radius );
      if ( newInd != SYS_MAX_T ) {
        TestVariables::viewManager->RefreshObjectModel( mltlnObj );
        res = true;
        // Отрисовка результата.  // Drawing result.
        const MbContour * curve = multiline->GetCurve( newInd );
        MbItem * curveObj = TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, (MbContour*)curve );
        TCHAR message[40];
        _sntprintf( message, 40, RT_RADIUS_CURVE_TO_INDEX F_TD, newInd );
  
        MessageBoxEx( message, IDS_EDIT_RESULT );
        TestVariables::viewManager->DeleteObject( curveObj );        
      }
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Удалить радиус эквидистанты мультилинии.
// Remove radius of the offset of multiline.
// ---
bool RemoveMltLinesRadius()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    double radius = 0.0;
    if ( ::GetDouble(IDS_ENTER_RADIUS, radius) ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      if ( multiline->RemoveRadius(radius) ) {
        TestVariables::viewManager->RefreshObjectModel( mltlnObj );
        res = true;
      }
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Выставить тип законцовки в начале мультилинии.
// Set type of the ending at the begin of multiline.
// ---
bool SetMltLinesBegTipType()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    int type = 0;
    if ( ::GetInt(IDS_ENTER_OUT_TIP_TYPE, type) ) {
      EnMLTipType tipType;
      switch ( type ) {
        case 0  : tipType = mtt_UndefTip;    break;
        case 1  : tipType = mtt_LinearTip;   break;
        case 2  : tipType = mtt_ArcTip;      break;
        case 3  : tipType = mtt_PolylineTip; break;
        case 4  : tipType = mtt_ObliqueTip;  break;
        default : tipType = mtt_UndefTip;    break;
      }
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      multiline->SetBegTipType( tipType );
      TestVariables::viewManager->RefreshObjectModel( mltlnObj );
      res = true;
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Выставить параметр законцовки в начале мультилинии.
// Set parameter of the ending at the begin of multiline.
// ---
bool SetMltLinesBegTipParam()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    double param = 0.0;
    if ( ::GetDouble(IDS_ENTER_INITIAL_PRM, param) ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      multiline->SetBegTipParam( param );
      TestVariables::viewManager->RefreshObjectModel( mltlnObj );
      res = true;
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Выставить тип законцовки в конце мультилинии.
// Set type of the ending at the end of multiline.
// ---
bool SetMltLinesEndTipType()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    int type = 0;
    if ( ::GetInt(IDS_ENTER_OUT_TIP_TYPE, type) ) {
      EnMLTipType tipType;
      switch ( type ) {
        case 0  : tipType = mtt_UndefTip;    break;
        case 1  : tipType = mtt_LinearTip;   break;
        case 2  : tipType = mtt_ArcTip;      break;
        case 3  : tipType = mtt_PolylineTip; break;
        case 4  : tipType = mtt_ObliqueTip;  break;
        default : tipType = mtt_UndefTip;    break;
      }
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      multiline->SetEndTipType( tipType );
      TestVariables::viewManager->RefreshObjectModel( mltlnObj );
      res = true;
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Выставить параметр законцовки в конце мультилинии.
// Set parameter of the ending at the end of multiline.
// ---
bool SetMltLinesEndTipParam()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    double param = 0.0;
    if ( ::GetDouble(IDS_ENTER_FINAL_PRM, param) ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      multiline->SetEndTipParam( param );
      TestVariables::viewManager->RefreshObjectModel( mltlnObj );
      res = true;
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Выставить использование замкнутости.
// Set the use of closeness.
// ---
bool SetMltLinesProcessClosed()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    int processClosed = 0;
    if ( ::GetInt(IDS_ENTER_USE_OR_NOT, processClosed) ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      multiline->SetProcessClosed( processClosed != 0 );
      TestVariables::viewManager->RefreshObjectModel( mltlnObj );
      res = true;
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Построить скругление базовой кривой мультилинии.
// Build the fillet of the base curve of multiline.
// ---
bool FilletMltLinesBasisCurve()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    double radius = 0.0;
    if ( ::GetDouble(IDS_ENTER_RADIUS, radius) ) {
      int allCorners = 0;
      if ( ::GetInt(IDS_ENTER_ALL_CORNERS, allCorners) ) {
        MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
        MbCartPoint pnt;
        bool nodeFlag = allCorners != 0;
        if ( !nodeFlag ) {
          if ( ::GetPoint(IDS_SELECT_POINT, pnt) ) {
            const MbContour & basisCurve = multiline->GetBasisCurve();
            double t = basisCurve.PointProjection( pnt );
            basisCurve.PointOn( t, pnt );
          }
          else
            return false;
        }        
        if ( ::FilletMultiline(*multiline, radius, nodeFlag, pnt, StVertexOfMultilineInfo()) ) {
          TestVariables::viewManager->RefreshObjectModel( mltlnObj );
          res = true;
        }
      }
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Построить фаску базовой кривой мультилинии.
// Build the chamfer of the base curve of multiline.
// ---
bool ChamferMltLinesBasisCurve()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    // Получаем параметры фаски. // Get chamfer parameters.
    double length = 0.0, par = 0.0;
    int type = 0;
    if ( ::GetDouble(IDS_ENTER_FACET_LENGHT, length) &&
         ::GetDouble(IDS_ENTER_PARAMETER,    par   ) &&
         ::GetInt   (IDS_ENTER_CHAMFER_TYPE, type  ) ) {
      if ( type == 0 )
        par *= Math::DEGRAD;
      int allCorners = 0;
      if ( ::GetInt(IDS_ENTER_ALL_CORNERS, allCorners) ) {
        MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
        MbCartPoint pnt;
        bool nodeFlag = allCorners != 0;
        if ( !nodeFlag ) {
          if ( ::GetPoint(IDS_SELECT_POINT, pnt) ) {
            const MbContour & basisCurve = multiline->GetBasisCurve();
            double t = basisCurve.PointProjection( pnt );
            basisCurve.PointOn( t, pnt );
          }
          else
            return false;
        }        
        if ( ::ChamferMultiline(*multiline, length, par, type > 0, nodeFlag, pnt, StVertexOfMultilineInfo()) ) {
          TestVariables::viewManager->RefreshObjectModel( mltlnObj );
          res = true;
        }
      }
    }
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Разбить мультилинию на две части.
// Break multiline into two parts.
// ---
bool BreakMultiline()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );    
    MbCartPoint pnt1;
    if ( ::GetPoint(IDS_SELECT_POINT, pnt1) ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      MbCartPoint pnt2;
      bool ok = true;
      if ( multiline->IsClosed() )
        ok = ::GetPoint( IDS_SELECT_POINT, pnt2 );
      if ( ok ) {
        PArray<MbMultiline> parts( 0, 1, false );
        if ( ::BreakMultiline(*multiline, pnt1, pnt2, parts) && parts.Count() == 2 ) {
          res = true;
          MbPlacement3D placement( ((MbPlaneInstance *)mltlnObj)->SetPlacement().GetOrigin() );
          TestVariables::viewManager->DeleteObject( mltlnObj );
          for ( size_t i = 0, count = parts.Count(); i < count; i++ )
            TestVariables::viewManager->AddObject( TestVariables::CONTOUR_Style, parts[i], &placement );
        }
      }
    }
  }
  
  return res;
}


//-------------------------------------------------------------------------------
// Разбить мультилинию на N частей.
// Break multiline into N parts.
// ---
bool BreakMultilineNParts()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );    
    ptrdiff_t partsCount = 2;
    if ( ::GetInt(IDS_ENTER_QUANTITY, partsCount) ) {
      MbMultiline * multiline = (MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
      MbCartPoint pnt;
      bool ok = true;
      if ( multiline->IsClosed() )
        ok = ::GetPoint( IDS_SELECT_POINT, pnt );
      if ( ok ) {
        PArray<MbMultiline> parts( 0, 1, false );
        if ( ::BreakMultilineNParts(*multiline, partsCount, pnt, parts) ) {
          res = true;
          MbPlacement3D placement( ((MbPlaneInstance *)mltlnObj)->SetPlacement().GetOrigin() );
          TestVariables::viewManager->DeleteObject( mltlnObj );
          for ( size_t i = 0, count = parts.Count(); i < count; i++ )
            TestVariables::viewManager->AddObject( TestVariables::CONTOUR_Style, parts[i], &placement );
        }
      }
    }
  }
  
  return res;
}


//-------------------------------------------------------------------------------
// Удалить последний сегмент мультилинии.
// Delete last segment of multiline.
// ---
bool DelMltLinesLastBasisSegment()
{
  bool res = false;
  MbItem * mltlnObj;
  if ( ::GetMObj(IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj) ) {
    TestVariables::viewManager->ToggleObject( mltlnObj );
    ((MbMultiline*)((MbPlaneInstance *)mltlnObj)->GetPlaneItem())->DeleteLastBasisSegment();
    TestVariables::viewManager->RefreshObjectModel( mltlnObj );
    res = true;
    TestVariables::viewManager->ToggleObject( mltlnObj );
  }

  return res;
}


//-------------------------------------------------------------------------------
// Удалить часть мультилинии между точками.
// Delete part of the multiline between points.
// ---
bool DelMltPartP1P2()
{
  bool res = false;
  MbItem * mltlnObj; 
  MbPlaneItem * cnt;
  MbCartPoint pnt[3];

  if ( ::GetMObj ( IDS_SELECT_MULTILINE, pt_Multiline, &mltlnObj, &cnt ) &&   
       ::GetPoint( IDS_SELECT_POINT, pnt[0] ) &&
       ::GetPoint( IDS_SELECT_POINT, pnt[1] ) &&
       ::GetPoint( IDS_SELECT_POINT, pnt[2] ) ) 
  {
    if( cnt && mltlnObj )
    {
      TestVariables::viewManager->ToggleObject( mltlnObj );

      MbContourWithBreaks * contourWBr  = dynamic_cast<MbContourWithBreaks *>(cnt);
      if( contourWBr )
      {           
        const MbPlaneItem * planeItem = ((MbPlaneInstance *)mltlnObj)->GetPlaneItem();
        if( planeItem != nullptr && ((MbMultiline*)planeItem)->DeletePartP1P2( contourWBr, pnt[0], pnt[1], pnt[2] ) )
        {
          TestVariables::viewManager->RefreshObjectModel( mltlnObj );
          res = true;
        }  
      }

      TestVariables::viewManager->ToggleObject( mltlnObj );
    }    
  }

  return res;
}
