//////////////////////////////////////////////////////////////////////////////////////////
// Создание проекций и операции над ними
// Construction of projections and operations with projections
//////////////////////////////////////////////////////////////////////////////////////////

#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <test_draw.h>
#include <tool_time_test.h>
#include <templ_p_array.h>
#include <mb_cross_point.h>
#include <cur_arc.h>
#include <cur_line_segment.h>
#include <cur_contour.h>
#include <cur_plane_curve.h>
#include <cur_surface_intersection.h>
#include <surf_plane.h>
#include <alg_silhouette_hide.h>
#include <alg_polyline.h>
#include <alg_dimension.h>
#include <assisting_item.h>
#include <solid.h>
#include <mesh.h>
#include <space_instance.h>
#include <point_frame.h>
#include <wire_frame.h>
#include <action_point.h>
#include <action_curve3d.h>
#include <action_surface_curve.h>
#include <mesh.h>
#include <map_section.h>
#include <map_section_complex.h>       
#include <map_thread.h>
#include <map_create.h>
#include <map_lump.h>
#include <last.h>

using namespace c3d;

#ifdef _DRAWGI  // включатели отрисовки во время отладки
namespace DrawTestMap {
  bool drawItem  = false;
}
#endif // _DRAWGI


//------------------------------------------------------------------------------
// Создание прямоугольника на плоскости
// Create a rectangle on a plane
// ---
void CreateRectangleOnPlane()
{
  MbCartPoint3D currPoint;
  MbCartPoint   p0, p1, c;
  WorkWindow  * vc = TestVariables::viewManager->GetActiveWindow();
  MbMatrix3D    into( vc->GetNullMatrixInto() );

  if ( ::GetPoint( IDS_SELECT_POINT, currPoint )) {
    currPoint.Transform( into ); p0.Init(currPoint.x,currPoint.y);
    if ( ::GetPoint( IDS_SELECT_POINT, currPoint )) {
      currPoint.Transform( into ); p1.Init(currPoint.x,currPoint.y);

      double radius = 10.0;
      if ( ::GetDouble( IDS_ENTER_STRETCHING, radius )) {
        double x0 = std_min(p0.x, p1.x);
        double x1 = std_max(p0.x, p1.x);
        double y0 = std_min(p0.y, p1.y);
        double y1 = std_max(p0.y, p1.y);
        double d  = std_min(x1 - x0, y1 - y0) / 2 - 2 * PARAM_DELTA_MIN;
        MbContour    *contour = new MbContour();
        MbPlacement3D place( vc->GetNullPlacement() );
        MbLineSegment *seg = nullptr;
        MbArc * arc = nullptr;

        radius = fabs(radius);
        radius = (d < radius)? d : radius;

        p0.Init(x0 + radius, y0);
        p1.Init(x1 - radius, y0);
        seg = new MbLineSegment( p0, p1 );
        contour->AddSegment( seg );
        p0.Init(x1, y0 + radius);
        if (radius>0) {
          c.Init(x1 - radius, y0 + radius);
          arc = new MbArc();
          arc->Init(c, p1, p0, 1);
          contour->AddSegment( arc );
        }
        p1.Init(x1, y1 - radius);
        seg = new MbLineSegment( p0, p1 );
        contour->AddSegment( seg );
        p0.Init(x1 - radius, y1);
        if (radius>0) {
          c.Init(x1 - radius, y1 - radius);
          arc = new MbArc();
          arc->Init(c, p1, p0, 1);
          contour->AddSegment( arc );
        }
        p1.Init(x0 + radius, y1);
        seg = new MbLineSegment( p0, p1 );
        contour->AddSegment( seg );
        p0.Init(x0, y1 - radius);
        if (radius>0) {
          c.Init(x0 + radius, y1 - radius);
          arc = new MbArc();
          arc->Init(c, p1, p0, 1);
          contour->AddSegment( arc );
        }
        p1.Init(x0, y0 + radius);
        seg = new MbLineSegment( p0, p1 );
        contour->AddSegment( seg );
        p0.Init(x0 + radius, y0);
        if (radius>0) {
          c.Init(x0 + radius, y0 + radius);
          arc = new MbArc();
          arc->Init(c, p1, p0, 1);
          contour->AddSegment( arc );
        }
        MbCurve3D * cr = nullptr;
        ::PlaneCurve( place, *contour, cr );
        TestVariables::viewManager->AddObject( TestVariables::ELEMENTARY_Style, cr );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание проекции кривой
// Create curve's projection
// ---
bool MakeCurveHideMap()
{
  MbItem * obj = nullptr;
  const MbCurve3D * curve = nullptr;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) ) {
    if ( obj->IsA() == st_SpaceInstance )
      curve = (MbCurve3D *)(((MbSpaceInstance *)obj)->GetSpaceItem());
    else if ( obj->IsA() == st_WireFrame ) {
      MbWireFrame * wf = static_cast<MbWireFrame *>(obj);
      if ( wf->GetEdgesCount() == 1 ) {
        curve = (wf->GetEdge(0) != nullptr) ? &wf->SetEdge(0)->GetCurve() : nullptr;
      }
    }
  }
  else {
    MbTopItem *edge = nullptr;
    if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &edge ) ) {
      curve = &(((MbCurveEdge *)edge)->GetCurve());
    }
  }
  if ( (obj != nullptr) && (curve != nullptr) ) {
    TestVariables::viewManager->ToggleObject(obj);

    WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();
    
    SetWaitCursor( true );
    MbMatrix3D into;
    currentView3D->GetNullPlacement().GetMatrixInto( into );
    MbCurve * init = curve->GetMap( into );
    MbCurve3D * cr = nullptr;
    ::PlaneCurve( currentView3D->GetNullPlacement(), *init, cr );
    TestVariables::viewManager->AddObject( TestVariables::BACKGROUND_Style, cr );

    SetWaitCursor( false );
    TestVariables::viewManager->ToggleObject(obj);
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// Создание плоской проекции поверхности
// Create a flat projection of a surface
// ---
void MakeSurfaceHideMap()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    PArray<MbCurve> hide( 6, 2, false );
    WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();
    MbPlacement3D pl( currentView3D->GetNullPlacement() );

    SetWaitCursor( true );

    ::CreateSurfaceHide( *(MbSurface *)((MbSpaceInstance *)obj)->GetSpaceItem(),
                         pl, Math::deviateSag, hide, Math::DefaultMathVersion() );

    SetWaitCursor( false );
    TestVariables::viewManager->ToggleObject(obj);

    for ( size_t i = 0; i < hide.Count(); ++i ) {
      TestVariables::viewManager->AddObject( TestVariables::BACKGROUND_Style, hide[i], &pl );
    }
  }
}


//------------------------------------------------------------------------------
// Добавить следы ребер в модель
// Add vestiges of edges to a model
// ---
static
void AddOriginalEdgeVestiges( MbVEFVestiges & vestiges, bool isVisible, MbPlacement3D & place, size_t & edgeCurveCount )
{
  for ( size_t i = 0, count = vestiges.EdgeVestigesCount(); i < count; ++i ) { // Следы ребер, владеет
    MbCurveVestige & vest = vestiges.GetEdgeVestige( i )->SetCurveInfo(); // Информация о следе кривой
    MbBaseVestige::Type vesType = vestiges.GetEdgeVestige( i )->GetType(); // Тип отображения

    Style ns;
    if ( isVisible )
      ns = (vesType == MbBaseVestige::vt_SmoothEdge) ? TestVariables::SURFACE_Style : TestVariables::BACKGROUND_Style;
    else
      ns = (vesType == MbBaseVestige::vt_SmoothEdge) ? TestVariables::DIM_Style : TestVariables::HIDE_Style;

    size_t cnt = isVisible ? vest.GetVisiblePartsCount()  // видимые проекции 
                           : vest.GetHiddenPartsCount();  // не видимые проекции 
    if ( cnt > 0 ) {
      for ( size_t j = 0; j < cnt; ++j ) {
        MbCurve * curve = isVisible ? vest._PickupVisibleCurve( j ) : vest._PickupHiddenCurve( j );
        TestVariables::viewManager->AddObject( ns, curve, &place, false ); // не рисуем сразу, медленно при большом количестве
        edgeCurveCount++;
      }
    }
  }
}


//------------------------------------------------------------------------------
//  Добавить следы граней в модель 
//  Add vestiges of faces to a model
// ---
static
void AddOriginalFaceVestiges( MbVEFVestiges & vestiges, bool isVisible, MbPlacement3D & place, size_t & faceCurveCount )
{
  Style ns;
  if ( isVisible ) ns = TestVariables::BACKGROUND_Style;
  else             ns = TestVariables::HIDE_Style;

  for ( size_t i = 0, count = vestiges.FaceVestigesCount(); i < count; i++ ) { // Следы граней, владеет
    PArray<MbCurveVestige> & curveInfos = vestiges.GetFaceVestige( i )->curveInfos;

    for ( size_t k = 0, c = curveInfos.size(); k < c; ++k ) { // Следы граней, владеет
      MbCurveVestige * vest = curveInfos[k]; // Информация о следе кривой

      size_t cnt = isVisible ? vest->GetVisiblePartsCount()  // видимые проекции 
                             : vest->GetHiddenPartsCount();  // не видимые проекции 
      if ( cnt > 0 ) {
        for ( size_t j = 0; j < cnt; ++j ) {
          MbCurve * curve = isVisible ? vest->_PickupVisibleCurve( j ) : vest->_PickupHiddenCurve( j );
          TestVariables::viewManager->AddObject( ns, curve, &place, false );
          faceCurveCount++;
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Добавить следы условного изображения резьбы в модель 
// Add vestiges of edges to a model
// ---
static
void AddOriginalAnnVestiges( MbVEFVestiges & vestiges, bool isVisible, MbPlacement3D & place, size_t & annCurveCount )
{
  Style ns;
  if ( isVisible ) ns = TestVariables::WIRE_Style;
  else             ns = TestVariables::HIDE_Style;

  for ( size_t i = 0, count = vestiges.AnnotateVestigesCount(); i < count; ++i ) { // Следы ребер, владеет
    MbCurveVestige & vest = vestiges.GetAnnotationVestige( i )->SetCurveInfo(); // Информация о следе кривой

    size_t cnt = isVisible ? vest.GetVisiblePartsCount() // Видимые проекции 
                           : vest.GetHiddenPartsCount(); // Не видимые проекции 
    if ( cnt > 0 ) {
      for ( size_t j = 0; j < cnt; ++j ) {
        MbCurve * crv = isVisible ? vest._PickupVisibleCurve( j ) : vest._PickupHiddenCurve( j );
        TestVariables::viewManager->AddObject( ns, crv, &place, false );
        annCurveCount++;
      }
    }
  }
}


//------------------------------------------------------------------------------
// Добавить следы пространственных кривых в модель 
// Add vestiges of space curves to a model
// ---
static
void AddOriginalCurvesVestiges( MbVEFVestiges & vestiges, bool isVisible, MbPlacement3D & place, size_t & spaceCurvesCount )
{
  Style ns;
  if ( isVisible ) ns = Style( 1, TestVariables::CYAN );
  else             ns = Style( 1, TestVariables::YELLOW );

  for ( size_t i = 0, count = vestiges.CurveVestigesCount(); i < count; ++i ) { // Следы ребер, владеет
    MbEdgeVestige * cVestige = vestiges.GetCurveVestige( i );
    MbCurveVestige & vest = cVestige->SetCurveInfo(); // Информация о следе кривой
    
    size_t cnt = isVisible ? vest.GetVisiblePartsCount() // Видимые проекции 
                           : vest.GetHiddenPartsCount(); // Не видимые проекции 
    if ( cnt > 0 ) {
      for ( size_t j = 0; j < cnt; ++j ) {
        MbCurve * crv = isVisible ? vest._PickupVisibleCurve( j ) : vest._PickupHiddenCurve( j );
        TestVariables::viewManager->AddObject( ns, crv, &place, false );
        spaceCurvesCount++;
      }
    }
  }
}


//------------------------------------------------------------------------------
// Добавить следы пространственных точек в модель 
// Add vestiges of space points to a model
// ---
static
void AddOriginalPointsVestiges( MbVEFVestiges & vestiges, bool isVisible, MbPlacement3D & place, size_t & spacePointsCount )
{
  Style nsVisible = Style( 1, TestVariables::BLACK );
  Style nsHidden  = Style( 1, TestVariables::DARKGRAY );

  for ( size_t i = 0, count = vestiges.PointVestigesCount(); i < count; ++i ) { // Следы ребер, владеет
    MbVertexVestige * pVestige = vestiges.GetPointVestige( i );
    const MbCartPoint & uv = pVestige->GetPoint(); // Информация о следе кривой
    if ( isVisible == pVestige->IsVisible() ) {
      MbCartPoint3D sPnt;
      place.PointOn( uv, sPnt );
      MbPointFrame * gsPnt = new MbPointFrame( sPnt );

      TestVariables::viewManager->AddObject( (isVisible ? nsVisible : nsHidden), gsPnt, false );
      spacePointsCount++;
    }
  }
}

//------------------------------------------------------------------------------
// Добавить в модель полные проекционные следы
// Add total vestiges to the model
// ---
static
void AddOrignalTotalVestiges( MbVEFVestiges & vestiges, const MbPlacement3D & viewPlace, const Style & style )
{
  // Следы ребер
  size_t i, count;
  for ( i = 0, count = vestiges.EdgeVestigesCount(); i < count; ++i ) { // Следы ребер, владеет
    MbCurveVestige & vest = vestiges.GetEdgeVestige( i )->SetCurveInfo();        // Информация о следе кривой         
    MbCurve * totalPrj = vest.DetachTotalProjection();
    TestVariables::viewManager->AddObject( style, totalPrj, &viewPlace, false );
  }
  // Следы граней
  for ( i = 0, count = vestiges.FaceVestigesCount(); i < count; i++ ) {   // Следы граней, владеет
    PArray<MbCurveVestige> & curveInfos = vestiges.GetFaceVestige( i )->curveInfos;
    for ( size_t k = 0, c = curveInfos.Count(); k < c; k++ ) {          // Следы граней, владеет
      MbCurveVestige * vest = curveInfos[k];                            // Информация о следе кривой
      MbCurve * totalPrj = vest->DetachTotalProjection();
      TestVariables::viewManager->AddObject( style, totalPrj, &viewPlace, false );
    }
  }
  // Следы аннотационных объектов (линий резьбы)
  for ( i = 0, count = vestiges.AnnotateVestigesCount(); i < count; i++ ) { // Следы ребер, владеет
    MbCurveVestige & vest = vestiges.GetAnnotationVestige( i )->SetCurveInfo();        // Информация о следе кривой     
    MbCurve * totalPrj = vest.DetachTotalProjection();                      // Видимые проекции 
    TestVariables::viewManager->AddObject( style, totalPrj, &viewPlace, false );
  }
}


//------------------------------------------------------------------------------
// Добавить в модель копии проекционных следов
// Add copy vestiges to the model
//---
static
void AddCopySolidVestiges( const MbVEFVestiges & vestiges, const MbPlacement3D & viewPos, 
                           const Style & visibleStyle, const Style & hiddenStyle, const Style & sectionStyle,
                           bool showItemsRightNow )
{
  // Отобразить следы рёбер
  size_t i, count;
  for ( i = 0, count = vestiges.EdgeVestigesCount(); i < count; ++i ) { // Следы ребер, владеет
    const MbCurveVestige & track = vestiges.GetEdgeVestige( i )->SetCurveInfo();
    if ( track.GetAllPartsCount() > 0 ) {
      if ( track.GetVisiblePartsCount() > 0 ) { // Видимые проекции рёбер
        bool sectionLine = vestiges.GetEdgeVestige( i )->GetType() == MbEdgeVestige::vt_SectionLine;
        for ( size_t j = track.GetVisiblePartsCount(); j--; ) {
          const MbCurve * crv = track._GetVisibleCurve( j );
          TestVariables::viewManager->AddObject( sectionLine ? sectionStyle : visibleStyle, &crv->Duplicate(), &viewPos, showItemsRightNow );
        }
      }
      if ( track.GetHiddenPartsCount() > 0 ) { // Не видимые проекции рёбер
        for ( size_t j = track.GetHiddenPartsCount(); j--; ) {
          const MbCurve * crv = track._GetHiddenCurve( j );
          TestVariables::viewManager->AddObject( hiddenStyle, &crv->Duplicate(), &viewPos, showItemsRightNow );
        }
      }
    }
  }
  // Отобразить следы граней
  for ( i = 0, count = vestiges.FaceVestigesCount(); i < count; ++i ) {
    const MbFaceVestige * vf = vestiges.GetFaceVestige( i );
    for ( size_t k = 0, cNb = vf->curveInfos.size(); k < cNb; ++k ) {
      const MbCurveVestige * track = vf->curveInfos[k];
      if ( track->GetAllPartsCount() > 0 ) {
        if ( track->GetVisiblePartsCount() > 0 ) { // Видимые линии очерка граней
          for ( size_t j = track->GetVisiblePartsCount(); j--; ) {
            const MbCurve * crv = track->_GetVisibleCurve( j );
            TestVariables::viewManager->AddObject( visibleStyle, &crv->Duplicate(), &viewPos, showItemsRightNow );
          }
        }
        if ( track->GetHiddenPartsCount() > 0 ) { // Невидимые линии очерка граней
          for ( size_t j = track->GetHiddenPartsCount(); j--; ) {
            const MbCurve * crv = track->_GetHiddenCurve( j );
            TestVariables::viewManager->AddObject( hiddenStyle, &crv->Duplicate(), &viewPos, showItemsRightNow );
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Добавить в модель проекционные следы
// Add vestiges to a model
// ---
inline
void AddAllOriginalVestiges( MbVEFVestiges & vestiges, bool isVisible, MbPlacement3D & place, size_t & count )
{
  // Следы ребер
  ::AddOriginalEdgeVestiges  ( vestiges, isVisible, place, count );
  // Следы граней
  ::AddOriginalFaceVestiges  ( vestiges, isVisible, place, count );
  // Следы условного изображения резьбы
  ::AddOriginalAnnVestiges   ( vestiges, isVisible, place, count );
  // Следы пространственных кривых
  ::AddOriginalCurvesVestiges( vestiges, isVisible, place, count );
  // Следы пространственных точек
  ::AddOriginalPointsVestiges( vestiges, isVisible, place, count );
}


//------------------------------------------------------------------------------
// Добавить в модель копии проекционных следов
// Add copy vestiges to the model
//---
inline static
void AddCopySolidVestiges( const MbVEFVestiges & vestiges, bool isVisible, RPArray<MbCurve> & resCurves )
{
  // Отобразить следы рёбер
  size_t i, count;
  for ( i = 0, count = vestiges.EdgeVestigesCount(); i < count; ++i ) { // Следы ребер, владеет
    const MbCurveVestige & track = vestiges.GetEdgeVestige( i )->SetCurveInfo();
    if ( track.GetAllPartsCount() > 0 ) {
      size_t cnt = isVisible ? track.GetVisiblePartsCount()
                             : track.GetHiddenPartsCount();
      if ( cnt > 0 ) {
        for ( size_t j = 0; j < cnt; ++j ) {
          const MbCurve * crv = isVisible ? track._GetVisibleCurve( j ) : track._GetHiddenCurve( j );
          resCurves.push_back( static_cast<MbCurve *>( &crv->Duplicate() ) );
        }
      }
    }
  }
  // Отобразить следы граней
  for ( i = 0, count = vestiges.FaceVestigesCount(); i < count; ++i ) {
    const MbFaceVestige * vf = vestiges.GetFaceVestige( i );
    for ( size_t k = 0, cNb = vf->curveInfos.size(); k < cNb; ++k ) {
      const MbCurveVestige * track = vf->curveInfos[k];
      if ( track->GetAllPartsCount() > 0 ) {
        size_t cnt = isVisible ? track->GetVisiblePartsCount()
                               : track->GetHiddenPartsCount();
        if ( cnt > 0 ) {
          for ( size_t j = 0; j < cnt; ++j ) {
            const MbCurve * crv = isVisible ? track->_GetVisibleCurve( j ) : track->_GetHiddenCurve( j );
            resCurves.push_back( static_cast<MbCurve *>( &crv->Duplicate() ) );
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Собрать тела c матрицами
// Gather solids with matrices
// ---
template <class Lumps>
size_t AddLumps( ThreeStates selectOne, Lumps & lumps )
{
  MbItem * obj = nullptr;

  if ( selectOne == ts_positive || selectOne == ts_neutral  ) { // Одно тело
    PathMatrix pathFrom;
    
    if ( GetGObj(IDS_SELECT_OBJECT, st_SpaceItem, &obj, pathFrom) ) {
    TestVariables::viewManager->ToggleObject( obj );
      MbSpaceItem * spaceItem = nullptr;
      if ( obj->IsA() == st_SpaceInstance )
        spaceItem = (static_cast<MbSpaceInstance *>(obj))->SetSpaceItem();
      if ( spaceItem == nullptr )
        spaceItem = obj;

      if ( spaceItem != nullptr ) {
        if ( (spaceItem->IsA() == st_Solid) ||
             (spaceItem->IsA() == st_Mesh) ) {
          MbLump * newLump( nullptr );
          if ( spaceItem->IsA() == st_Solid )
            newLump = new MbLump( *static_cast<MbSolid *>(spaceItem), pathFrom.second, 0/*nullptr*/ );
          else
            newLump = new MbLump( *static_cast<MbMesh *>(spaceItem), pathFrom.second, 0/*nullptr*/ );
          lumps.push_back( newLump );
        }
      }
    }
  }
  if ( (obj == nullptr) && (selectOne == ts_negative || selectOne == ts_neutral) ) {
    RPArray<MbItem> objs( 0, 1 );
    SArray<MbMatrix3D> matrs( 0, 1 );

    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    
    if ( wind != nullptr )
      wind->GetObjects( st_SpaceItem, objs, matrs );

    size_t objsCnt = objs.size();
    lumps.reserve( lumps.size() + objsCnt );

    bool refreshDrawing = false; // KOMPAS-18629

    for ( size_t i = 0; i < objsCnt; ++i ) {
      obj = objs[i];
      if ( obj != nullptr ) {
        TestVariables::viewManager->ToggleObject( obj, refreshDrawing );
        MbSpaceItem * spaceItem = nullptr;
        if ( obj->IsA() == st_SpaceInstance )
          spaceItem = (static_cast<MbSpaceInstance *>(obj))->SetSpaceItem();
        if ( spaceItem == nullptr )
          spaceItem = obj;
        if ( (spaceItem != nullptr) && ((spaceItem->IsA() == st_Solid) || (spaceItem->IsA() == st_Mesh)) ) {
          if ( obj->IsA() == st_Solid )
            lumps.push_back( new MbLump( *static_cast<MbSolid *>(obj), matrs[i], 0/*nullptr*/ ) );
          else
            lumps.push_back( new MbLump( *static_cast<MbMesh *>(obj), matrs[i], 0/*nullptr*/ ) );
        }
      }
    }
  }

  return lumps.size();
}

//------------------------------------------------------------------------------
// Собрать тела c матрицами
// Gather solids with matrices
// ---
template <class Lumps>
size_t AddMappingLumps( ThreeStates selectOne, bool isSectionLump, Lumps & lumps )
{
  MbItem * obj = nullptr;

  if ( selectOne == ts_positive || selectOne == ts_neutral  ) { // Одно тело
    PathMatrix pathFrom;
    if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj, pathFrom ) ) {
      TestVariables::viewManager->ToggleObject(obj);
      MbSolid * solid = static_cast<MbSolid *>(obj);
      lumps.push_back( new MbMappingLumps( *solid, pathFrom.second, isSectionLump, 0/*nullptr*/ ) );
    }
  }
  if ( (obj == nullptr) && (selectOne == ts_negative || selectOne == ts_neutral) ) {
    RPArray<MbItem> objs( 0, 1 );
    SArray<MbMatrix3D> matrs( 0, 1 );

    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();

    if ( wind != nullptr )
      wind->GetObjects( st_Solid, objs, matrs );

    size_t objsCnt = objs.size();
    lumps.reserve( lumps.size() + objsCnt );

    bool refreshDrawing = false; // KOMPAS-18629

    for ( size_t i = 0; i < objsCnt; ++i ) {
      obj = objs[i];
      if ( (obj != nullptr) && (obj->IsA() == st_Solid) ) {
        if ( objsCnt - 1 == i )
          refreshDrawing = true;
        TestVariables::viewManager->ToggleObject( obj, refreshDrawing );
        lumps.push_back( new MbMappingLumps( *static_cast<MbSolid *>(obj), matrs[i], isSectionLump, 0/*nullptr*/ ) );
      }
    }
  }

  return lumps.size();
}


//------------------------------------------------------------------------------
// Собрать все резьбы, принадлежащие телам
// Gather all threads belonging to bodies
// ---
template <class Lumps, class Threads>
size_t AddBodiesThreads( const Lumps & initBodies, Threads & threads )
{
  if ( initBodies.size() > 0 ) {
    c3d::ConstLumpsVector bodies;
    bodies.reserve( initBodies.size() );
    std::copy( initBodies.begin(), initBodies.end(), std::back_inserter( bodies ) );

    RPArray<MbItem> objs( 0, 1 );
    SArray<MbMatrix3D> matrs( 0, 1 );
    TestVariables::viewManager->GetObjects( st_Thread, objs, matrs );

    size_t i, objsCnt = objs.size();

    std::vector<const MbThread *> constThreads;
    constThreads.reserve( objsCnt );

    for ( i = 0; i < objsCnt; ++i ) {
      MbItem * obj = objs[i];
      if ( obj->IsA() == st_SpaceInstance ) {
        const MbSpaceItem * spaceItem = (static_cast<MbSpaceInstance *>(obj))->GetSpaceItem();
        if ( spaceItem != nullptr && spaceItem->IsA() == st_Thread ) {
          TestVariables::viewManager->ToggleObject( obj, false );
          constThreads.push_back( static_cast<const MbThread *>( spaceItem ) );
        }
      }
    }
    const size_t constThreadsCnt = constThreads.size();
    threads.reserve( threads.size() + constThreadsCnt );

    const size_t bodiesCnt = bodies.size();

    MbCube cube;
    std::vector<MbCube> solidCubes;
    solidCubes.reserve( bodiesCnt );
    for ( i = 0; i < bodiesCnt; ++i ) {
      cube.SetEmpty();
      bodies[i]->GetItem().AddYourGabaritTo( cube );
      cube.Transform( bodies[i]->GetMatrixFrom() );
      solidCubes.push_back( cube );
    }

    RPArray<MbLump> threadBodies;
    threadBodies.reserve( bodiesCnt );

    for ( i = 0; i < constThreadsCnt; ++i ) {
      const MbThread * constThread = constThreads[i];
      SPtr<MbThread> thread( static_cast<MbThread *>( &constThread->Duplicate() ) );
      if ( !matrs[i].IsSingle() )
        thread->Transform( matrs[i] );

      MbCube threadCube;
      thread->AddYourGabaritTo( threadCube );

      bool add = false;
      threadBodies.clear();
      for ( size_t j = 0; j < bodiesCnt; ++j ) {
        if ( threadCube.Intersect( solidCubes[j] ) ) {
          const MbLump * body = bodies[j];
          if ( body->GetItem().IsA() == st_Solid && thread->IsBodyThread( static_cast<const MbSolid &>(body->GetItem()), body->GetMatrixFrom() ) ) {
            threadBodies.push_back( body );
            add = true;
            break;
          }
        }
      }
      if ( add ) {
        threads.push_back( thread );
        thread->AddBodies( threadBodies );
        ::DetachItem( thread );
      }
    }    
  }
  return threads.size();
}


//------------------------------------------------------------------------------
// 
// ---
class ThreadsStore : public ItAnnObjectStore {

  // \ru Получить массив резьб. \en Get the array of threads.
  virtual bool GetMathThreads( const ConstLumpsMultiSet & bodies, RPArray<MbThread> & threads, bool )
  {
    size_t threadsCnt = threads.size();
    ::AddBodiesThreads( bodies, threads );
    return (threads.size() > threadsCnt);
  }
  // \ru Получить массив резьб. \en Get the array of threads.
  virtual bool GetMathThreads( const ConstLumpsMultiSet & bodies, std::vector< SPtr<MbThread> > & threads, bool )
  {
    size_t threadsCnt = threads.size();
    ::AddBodiesThreads( bodies, threads );
    return (threads.size() > threadsCnt);
  }
};


//------------------------------------------------------------------------------
// Собрать все пространственные кривые
// Gather all space curves
// ---
static size_t AddSpaceCurves( std::vector< SPtr<MbCurve3D> > & curves )
{
  RPArray<MbItem> objs( 0, 1 );
  SArray<MbMatrix3D> matrs( 0, 1 );
  TestVariables::viewManager->GetObjects( st_Curve3D, objs, matrs );
  for ( size_t i = 0, objsCnt = objs.size(); i < objsCnt; i++ ) {
    const MbItem * obj = objs[i];
    if ( obj->IsA() == st_SpaceInstance ) {
      const MbSpaceItem * spaceItem = (static_cast<const MbSpaceInstance *>(obj))->GetSpaceItem();
      if ( spaceItem != nullptr && (spaceItem->Family() == st_Curve3D) ) {
        SPtr<MbCurve3D> curve( static_cast<MbCurve3D *>( &spaceItem->Duplicate() ) );
        curve->Transform( matrs[i] );
        curves.push_back( curve );
      }
    }
  }
  return curves.size();
}


//------------------------------------------------------------------------------
// Собрать все пространственные точки
// Gather all space points
// ---
static size_t AddSpacePoints( std::vector<MbCartPoint3D> & points )
{
  RPArray<MbItem> objs( 0, 1 );
  SArray<MbMatrix3D> matrs( 0, 1 );
  TestVariables::viewManager->GetObjects( st_PointFrame, objs, matrs );
  MbCartPoint3D point;
  for ( size_t i = 0, objsCnt = objs.size(); i < objsCnt; i++ ) {
    MbItem * obj = objs[i];
    if ( obj != nullptr && obj->IsA() == st_PointFrame ) {
      MbMatrix3D matr;
      if ( i < matrs.size() )
        matr = matrs[i];
      const MbPointFrame * spacePoint = static_cast<MbPointFrame *>( obj );
      size_t addCnt = spacePoint->GetVerticesCount();
      for ( size_t j = 0; j < addCnt; ++j ) {
        spacePoint->GetCartPoint( j, point );
        point.Transform( matr );
        points.push_back( point );
      }
    }
  }
  return points.size();
}


//------------------------------------------------------------------------------
// Наполнить множество данных о пространственных точках
// Fill sets of data about space points
// ---
static
void FillPointsDataAddRef( const std::vector<MbCartPoint3D> & points, const MbMatrix3D & from,
                           RPArray<MbSpacePoints> & pointsData, PArray<MbName> & namesStore )
{
  size_t cnt = points.size();
  if ( cnt > 0 ) {
    MbSpacePoints * pointsGroup = new MbSpacePoints( 0/*nullptr*/, 0, from );
    RPArray<MbName> pointsNames( cnt, 1 );
    for ( size_t m = 0; m < cnt; m++ ) {
      MbName * pointName = new MbName();
      pointName->SetMainName( SimpleName(m) );
      pointsNames.Add( pointName );
    }
    pointsGroup->AddNamedPoints( points, pointsNames );
    pointsGroup->AddRef();
    pointsData.Add( pointsGroup );
    namesStore.AddArray( pointsNames );
  }
}


//------------------------------------------------------------------------------
// Наполнить множество данных о пространственных кривых
// Fill sets of data about space curves
// ---
static
void FillCurvesDataAddRef( const std::vector< SPtr<MbCurve3D> > & curves, const MbMatrix3D & from,
                           RPArray<MbSpaceCurves> & curvesData, PArray<MbName> & namesStore )
{
  size_t cnt = curves.size();
  if ( cnt > 0 ) {
    MbSpaceCurves * curvesGroup = new MbSpaceCurves( 0/*nullptr*/, 0, from );
    RPArray<MbName> curvesNames( cnt, 1 );
    for ( size_t m = 0; m < cnt; m++ ) {
      MbName * curveName = new MbName();
      curveName->SetMainName( SimpleName(m) );
      curvesNames.Add( curveName );
    }
    curvesGroup->AddNamedCurves( curves, curvesNames );
    curvesGroup->AddRef();
    curvesData.Add( curvesGroup );
    namesStore.AddArray( curvesNames );
  }
}


//------------------------------------------------------------------------------
// Создание плоской проекции тела
// Create a flat projection of a solid
// ---
void MakeSolidHideMap( bool one )
{
  //Math::SetDefaultValues()
  //Math::SetUserValue( 0, PARAM_NEAR );

  PArray<MbLump> lumps  ( 1, 1, true );
  PArray<MbName> namesStore( 0, 1, true ); // Хранилище имен

  std::vector< SPtr<MbThread> > threads;
  std::vector< SPtr<MbCurve3D> > curves;
  std::vector<MbCartPoint3D> points;

  int invisible = 1; // Проецировать невидимые линии
  ::GetInt( IDS_ENTER_HIDE_EDGE, invisible );

  bool skipVertices = false; // вершины добавлять
  MbMapVisibilityMode visMode( (invisible != 0), skipVertices );

  ThreeStates selectOne = one ? ts_positive : ts_negative;
  
  const size_t    lumpsCnt   = ::AddLumps( selectOne, lumps );        // Собрать тела
  const ptrdiff_t threadsCnt = ::AddBodiesThreads( lumps, threads );  // Собрать все резьбы, принадлежащие телам   
  const size_t    curvesCnt  = ::AddSpaceCurves( curves );            // Собрать все пространственные кривые      
  const size_t    pointsCnt  = ::AddSpacePoints( points );            // Собрать все пространственные точки

  if ( (lumpsCnt > 0) || (curvesCnt > 0) || (pointsCnt > 0) ) {
    WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();
    MbPlacement3D place( currentView3D->GetOwnPlace() );

    double znear = currentView3D->GetZVista(); 
    ::GetDouble( IDS_ENTER_DISTANCE, znear );        
    znear = - znear;
    bool shadeCurvesBySolids( false );
    if ( curvesCnt > 0 ) 
      shadeCurvesBySolids = GetBool( IDS_ENTER_SHADE_SPCURVES, shadeCurvesBySolids );
    bool addCenterLines( false );
    if ( lumpsCnt > 0 )
      addCenterLines = GetBool( IDS_ENTER_ADD_CENTERLINES, addCenterLines );
    int visDepthModeInt = 0;
    ::GetInt( IDS_CHOOSE_LIMIT_DEPTH_MODE, visDepthModeInt );
    auto visDepthMode = static_cast<MbMapVisibilityMode::LimitDepthMode>( std::clamp( visDepthModeInt, 0, 3 ) );
    double visDepth = 0.0;
    if ( visDepthMode != MbMapVisibilityMode::LimitDepthMode::off )
      ::GetDouble( IDS_ENTER_VISIBILITY_DEPTH, visDepth );
    visMode.Init( ( invisible != 0 ), skipVertices, shadeCurvesBySolids, false, addCenterLines, visDepthMode, visDepth );
    MbVEFVestiges vestiges;

    ::SetWaitCursor( true );

#if defined(C3D_DEBUG)
    SET_TIME_TEST(true);
#endif // C3D_DEBUG
    uint miliSec = ::GetTickCountEx();

    // Построение плоской проекции тела
    MbProjectionsObjects prObjects;

    RPArray<MbAnnCurves> annCurvesData( 0, 1 ); // Наборы линий отображения резьбы
    const MbName dummyAnnCurvesName;

    RPArray<MbSpacePoints> pointsData( pointsCnt, 1 );
    RPArray<MbSpaceCurves> curvesData( curvesCnt, 1 );

    static bool parallelThreadsProcessing = false;
    MbResultType res = rt_Success;

    if ( (threadsCnt > 0) || (curvesCnt > 0) || (pointsCnt > 0) ) {
      for ( size_t i = 0; i < lumpsCnt; i++ ) {
        MbLump * lump = lumps[i];
        if ( lump == nullptr && lump->GetItem().IsA() != st_Solid )
          continue;
        const MbSolid & solid = static_cast<const MbSolid &>( lump->GetItem() );
        uint comp = lump->GetComponent();
        const MbMatrix3D & matrFrom = lump->GetMatrixFrom();

        MbeThrMapType tmType = tmt_CompleteView;

        if ( parallelThreadsProcessing ) {
          // PerformThreadMapping working in parallel
          ENTER_PARALLEL( parallelThreadsProcessing );
#pragma omp parallel for schedule(dynamic)
          for ( ptrdiff_t k = 0; k < threadsCnt; k++ ) {
            const MbThread * thread = threads[k];
            if ( thread && thread->IsBodyThread( solid, matrFrom ) ) {
              MbAnnCurves * annCurves = new MbAnnCurves( dummyAnnCurvesName, comp, lump->GetIdentifier(), &solid, matrFrom );
              ::AddRefItem( annCurves );
              ThreadMapperStruct tmData( *thread, solid, matrFrom, place, tmType );
              ::PerformThreadMapping( tmData, *annCurves );
              if ( annCurves->GetCurvesCount() > 0 ) {
#pragma omp critical(PerformThreadMapping)
                annCurvesData.Add( annCurves );
              }
              else {
                ::ReleaseItem( annCurves );
              }
            }
          }
          EXIT_PARALLEL( parallelThreadsProcessing );
        }
        else {
          for ( ptrdiff_t k = 0; k < threadsCnt; ++k ) {
            const MbThread * thread = threads[k];
            if ( thread && thread->IsBodyThread( solid, matrFrom ) ) {
#ifdef _DRAWGI
              if ( DrawTestMap::drawItem ) {
                DrawGI::DrawItem( thread, TRGB_YELLOW );
                DrawGI::DrawItem( thread, TRGB_AZURE );
              }
#endif
              MbAnnCurves * annCurves = new MbAnnCurves( dummyAnnCurvesName, comp, lump->GetIdentifier(), &solid, matrFrom );
              ::AddRefItem( annCurves );
              ThreadMapperStruct tmData( *thread, solid, matrFrom, place, tmType );
              ::PerformThreadMapping( tmData, *annCurves );
              if ( annCurves->GetCurvesCount() > 0 ) {
                annCurvesData.Add( annCurves );
#ifdef _DRAWGI
                if ( DrawTestMap::drawItem ) {
                  SPtr<MbCurve3D> annCurveCopy;
                  for ( size_t l = annCurves->GetCurvesCount(); l--; ) {
                    annCurveCopy = static_cast<MbCurve3D *>( &annCurves->GetCurve( l )->GetCurve().Duplicate() );
                    annCurveCopy->Transform( matrFrom );
                    DrawGI::DrawItem( annCurveCopy, TRGB_YELLOW );
                    DrawGI::DrawItem( annCurveCopy, TRGB_BLACK );
                  }
                }
#endif
              }
              else {
                ::ReleaseItem( annCurves );
              }
            }
          }
        }
      }
    
      MbMatrix3D from; // Матрица пересчета в мир
      
      ::FillPointsDataAddRef( points, from, pointsData, namesStore );
      ::FillCurvesDataAddRef( curves, from, curvesData, namesStore );       
                
      prObjects.annCurves  = &annCurvesData;  // Добавление в проекции вида условного изображения резьбы      
      prObjects.pointsData = &pointsData;     // Добавление пространственных точек       
      prObjects.curvesData = &curvesData;     // Добавление пространственных кривых    

      res = ::GetVestiges( place, znear, lumps, prObjects, vestiges, visMode, Math::DefaultMathVersion() );
    }
    else {
      res = ::GetVestiges( place, znear, lumps, prObjects, vestiges, visMode, Math::DefaultMathVersion() );
    }

    miliSec = ::GetTickCountEx() - miliSec;
#if defined(C3D_DEBUG)
    CHECK_TIME_TEST();
#endif // C3D_DEBUG

    ::SetWaitCursor( false );

    size_t visCount = 0, hideCount = 0;

    // Не видимые проекции
    if ( invisible > 0 )
      ::AddAllOriginalVestiges( vestiges, false, place, hideCount );
    // Видимые проекции 
    ::AddAllOriginalVestiges( vestiges, true, place, visCount );

    // Полные проекции
    if ( invisible > 10 ) {
     ::AddOrignalTotalVestiges( vestiges, place, TestVariables::SURFACECURVE_Style );
    }

    TestVariables::viewManager->ShowModel();

    // Удаление (освобождение) элементов множеств
    ::ReleaseItems( annCurvesData );
    ::ReleaseItems( pointsData );
    ::ReleaseItems( curvesData );
    
    if ( res == rt_Success ) {
      TimeTest * testResult = nullptr;
#if defined(C3D_DEBUG)
      testResult = ::GetTimeTestResults();
#endif // C3D_DEBUG
      ShowTimeTestResult( testResult, visCount, hideCount, miliSec, st_PlaneInstance, IDS_PROJECTION_BODIES );
#if defined(C3D_DEBUG)
      SET_TIME_TEST( false );
#endif // C3D_DEBUG
    }
    else {
      ::PutErrorMessage( res, IDS_PROJECTION_BODIES );
    }

    prObjects.Relinquish(); // Был вылет - нужно отпустить, т.к. массивы были созданы локально
  }
}


//------------------------------------------------------------------------------
// Создание плоской проекции всех тел
// Create a flat projection of all solids
// ---
void MakeAllSolidsHideMap()
{
  PArray<MbLump> lumps( 1, 1, true );

  int invisible = 0; // Проецировать невидимые линии
  ::GetInt( IDS_ENTER_HIDE_EDGE, invisible );

  bool skipVertices = false; // вершины добавлять
  MbMapVisibilityMode visMode( (invisible != 0), skipVertices );

  /// Собрать все Item'ы
  const size_t lumpsCnt = ::AddLumps( ts_negative, lumps ); // Добавить тела

  if ( lumpsCnt > 0 ) {
    WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();
    MbPlacement3D place( currentView3D->GetOwnPlace() );
    double znear = currentView3D->GetZVista(); 
    znear = -znear;
    MbVEFVestiges vestiges;

    ::SetWaitCursor( true );

#if defined(C3D_DEBUG)
//    SET_TIME_TEST(true);
#endif // C3D_DEBUG
    uint miliSec = ::GetTickCountEx();

    // Построение плоской проекции тела
    PArray<MbAnnCurves> annCurvesData( 0, 1, true ); // Наборы линий отображения резьбы
    MbProjectionsObjects prObjects;

    MbResultType res = ::GetVestiges( place, znear, lumps, prObjects, vestiges, visMode, Math::DefaultMathVersion() );
    
    miliSec = ::GetTickCountEx() - miliSec;
#if defined(C3D_DEBUG)
//    CHECK_TIME_TEST();
#endif // C3D_DEBUG

    SetWaitCursor( false );

    size_t visCount = 0, hideCount = 0;

    // Не видимые проекции
    if ( invisible > 0 )
      ::AddAllOriginalVestiges( vestiges, false, place, hideCount );
    // Видимые проекции 
    ::AddAllOriginalVestiges( vestiges, true, place, visCount );

    // Полные проекции
    if ( invisible > 10 ) {
      ::AddOrignalTotalVestiges( vestiges, place, TestVariables::SURFACECURVE_Style );
    }

    TestVariables::viewManager->ShowModel();
    
    if ( res == rt_Success ) {
      TimeTest * testResult = nullptr;
#if defined(C3D_DEBUG)
      //    testResult = ::GetTimeTestResults();
#endif // C3D_DEBUG
      ::ShowTimeTestResult( testResult, visCount, hideCount, miliSec, st_PlaneInstance, IDS_PROJECTION_BODIES );
#if defined(C3D_DEBUG)
      //    SET_TIME_TEST(false);
#endif // C3D_DEBUG
    }
    else {
      ::PutErrorMessage( res, IDS_PROJECTION_BODIES );
    }

    prObjects.Relinquish(); // Был вылет, нужно отпустить, т.к. массивы были созданы локально
  }
}


//------------------------------------------------------------------------------
// Рассчитать шаг на который можно отступить от точки на кривой, не перешагнув ни одну другую кривую.
// Calculate step can be taken from a point on a curve without having stepped over any of the other curve
// ---
static
bool CalcStepInDir( const MbCartPoint & p, const MbVector & dir, const RPArray<MbCurve> & curves, const double initStep, double & step, double eps, const MbCurve * /*except*/ )
{
  bool stepFound = false;
  step = initStep; /// из начально шаг устанавливаем как радиус кривизны в точке
  for( ptrdiff_t j = 0, count = curves.Count(); j < count && step > eps; j++ )
  {
    // if ( except == curves[j] ) continue; // с самой собой тоже надо анализировать, т.к. кривые бывают очень кривыми
    MbCurve * crv = curves[j];
    MbDirection d (dir.x, dir.y );
    MbCartPoint pp;
    if ( crv != nullptr && crv->DirectPointProjection( p, d, pp ) )
      step = std_min( step , pp.DistanceToPoint( p ) / 2 );
  }
  if ( step > eps )
    stepFound = true;
  return stepFound;
}


//------------------------------------------------------------------------------
// Определяем от какой точки кривой будут построены две "проверочные" точки
// Выбираем такую точку на кривой чтобы не "перешагнуть" через одну из других кривых.
// Define a point from which two verification points will be created
// Choose the point in order to not step over any of the other curve
// ---
static
bool StepPoints ( const MbCurve & curve, const RPArray<MbCurve> & curves, MbCartPoint & _p, 
                  MbCartPoint & p_, bool & _stepFound, bool & stepFound_ )
{
  MbCartPoint p;
  MbVector norm;
  double eps = METRIC_NEAR; // минимальный шаг
  
  ptrdiff_t iter = 0;
  const ptrdiff_t maxSteps = 5;
  // опасно анализировать конечные точки, т.к. могут быть стыки с острыми углами
  double ONE_PI = 1.0 / M_PI;
  double dt[maxSteps] = { ONE_PI, 1.0 - ONE_PI, 0.5, 0.5 - ONE_PI, 0.5 + ONE_PI };
  do
  {
    double t = curve.GetTMin() + (curve.GetTMax() - curve.GetTMin() ) * dt[iter];
    double step_, _step;
    curve.PointOn( t, p );
    curve.Normal( t, norm );
    double absCur = ::fabs(curve.CurvatureRadius(t));
    if ( !stepFound_ )
    {
      stepFound_ = ::CalcStepInDir( p, norm, curves, absCur, step_, eps, &curve );
      if ( stepFound_ )
        p_ = p + norm * step_;
    }
    if ( !_stepFound )
    {
      _stepFound = ::CalcStepInDir( p, -norm, curves, absCur, _step, eps, &curve );
      if ( _stepFound )
        _p = p - norm * _step;
    }
    
  } while ( (!_stepFound || !stepFound_) && ++iter < maxSteps ); /// останавливаемся если просмотрели все варианты или нашли точку с подходящим шагом
  return _stepFound || stepFound_;
}

//------------------------------------------------------------------------------
// Проецируем точки на тела, чтобы определить попадают ли они в "тень силуэта" нет
// Возвращает true если обе точки смогли проецироваться, т.е попали в "тень", иначе - false.
// Project points on solids in order to define are projections get into a shadow of a silhouette or not
// Return 'true' if both points are projected in other words get into the shadow, otherwise 'false
// ---
static
bool ProjectStepPointsOnLumps( MbCartPoint3D &_p3d, MbCartPoint3D p3d_, MbPlacement3D place, RPArray<MbLump> & lumps, bool & _cp, bool & cp_)
{
  bool _outOfShadow(_cp), outOfShadow_(cp_);
  /// проецируем точки отступа на тела по направлению -place.AxisZ, если одна из точек не проецируется ни на одно тело, то кривая будет в очерке
  for( size_t j = 0; j < lumps.Count() && (_outOfShadow || outOfShadow_); j++ )
  {
    MbAxis3D _l ( _p3d, -place.GetAxisZ() ) ;
    MbAxis3D l_ ( p3d_, -place.GetAxisZ() ) ;
    MbMatrix3D into;
    lumps[j]->GetMatrixFrom().Div( into );
    _l.Transform( into );
    l_.Transform( into );
    MbFloatPoint3D crossPnt;
    float res;
    if ( !_outOfShadow || 
         (lumps[j]->GetItem().IsA() == st_Solid && static_cast<const MbSolid &>(lumps[j]->GetItem()).GetShell()->DirectPointProjection(_l.GetOrigin(), _l.GetAxisZ())) ||
         (lumps[j]->GetItem().IsA() == st_Mesh && static_cast<const MbMesh &>(lumps[j]->GetItem()).LineIntersection(MbFloatAxis3D(_l.GetOrigin(), _l.GetAxisZ()), crossPnt, res)))
      _outOfShadow = false;
    if ( !_outOfShadow && 
         (!outOfShadow_ || 
          (lumps[j]->GetItem().IsA() == st_Solid && static_cast<const MbSolid &>(lumps[j]->GetItem()).GetShell()->DirectPointProjection( l_.GetOrigin(), l_.GetAxisZ())) ||
          (lumps[j]->GetItem().IsA() == st_Mesh && static_cast<const MbMesh&>(lumps[j]->GetItem()).LineIntersection(MbFloatAxis3D(l_.GetOrigin(), l_.GetAxisZ()), crossPnt, res))) )
      outOfShadow_ = false;
  }
  return !_outOfShadow && !outOfShadow_;
}


//------------------------------------------------------------------------------
// Выбрать кривые которые образуют отчерк силуэт
// В отчерк попадут те кривые, при малом отступе о которых попадем в область вне "тени силуэта"
// Choose curves which form an outline of a silhouette
// If after small step from a curve we get out from domain of the shadow of the silhouette then the outline contains the curve 
// ---
static
void ChooseOutlineCurves( RPArray<MbCurve> & curves, MbPlacement3D & place, RPArray<MbLump> & lumps )
{
  /// для каждой кривой выбираем одну точку на ней и строим две другие, как отступы от первой по направлению нормали и в противоположную
  /// определяем есть ли проекция этих точек по направлению -place.AxiZ на какие-то тела из lumps если нет, то кривая будет в очерке
  for ( ptrdiff_t i = curves.Count() - 1; i >= 0 && curves.Count() > 1; i-- )
  {
    MbCurve * curve = curves[i];

    MbCartPoint p_, _p;
    bool stepFound_(false), _stepFound(false);
    if ( curve != nullptr )
      ::StepPoints( *curve, curves, _p, p_, _stepFound, stepFound_ );
    
    if ( _stepFound || stepFound_ )
    {
      /// соответствующие p_ и _p трехмерные точки
      MbCartPoint3D _p3d(_p.x,_p.y,0), p3d_(p_.x,p_.y,0);
      MbMatrix3D m (place);
      _p3d.Transform( m );
      p3d_.Transform( m );

      if ( ::ProjectStepPointsOnLumps(_p3d, p3d_, place, lumps, _stepFound, stepFound_) )
      {
        ::DeleteItem( curves[i] );
        curves.DetachInd( i );
      }
    }
  }
}

//------------------------------------------------------------------------------
// Разрезаем все кривые друг другом
// Cut all curves by each other
// ---
static
void CutAllCurves( RPArray<MbCurve> & curves, RPArray<MbCurve> & cutcurves )
{
  std::vector< SArray<double> > crosspoints( curves.Count() );
  // ищем точки
  for ( size_t i = 0; i < curves.size(); i++ ) {
    for ( size_t j = i+1; j < curves.size(); j++ ) {
      SArray<double> curcross1(2,2);      
      SArray<double> curcross2(2,2);
      double eps = PARAM_ACCURACY;
      if ( ::CurveCurveIntersection( *curves[i], *curves[j], curcross1, curcross2, eps, eps, true, false ) ) {
        for ( size_t k = 0; k < curcross1.Count(); k++ )
          crosspoints[i].push_back( curcross1[k] );
        for ( size_t k = 0; k < curcross2.Count(); k++ )
          crosspoints[j].push_back( curcross2[k] );
      }
    }
    if( !curves[i]->IsClosed() ) { // для незамкнутых кривых добавляем начало и конец
      crosspoints[i].push_back( curves[i]->GetTMin() );
      crosspoints[i].push_back( curves[i]->GetTMax() );
    }
  }
  // режем на части
  for( size_t i = 0; i < curves.Count(); i++ ) {
    crosspoints[i].Sort(); // упорядочиваем точки пересечения
    ptrdiff_t jend = 0;

    for ( size_t j = 1; j < crosspoints[i].size(); j++ ) {
      bool match = ( curves[i]->PointOn( crosspoints[i][jend] ) == curves[i]->PointOn( crosspoints[i][j] ) );

      if ( !match ) {
        cutcurves.push_back( curves[i]->Trimmed( crosspoints[i][jend], crosspoints[i][j], 1 ) );
        jend = j; // если текущий осколок слишком мал, следующий будет включать и его
      }
    }
    if ( curves[i]->IsClosed() ) { // последний осколок для замкнутой образующей
      if( (crosspoints[i].Count() > 1) && (jend != 0) ) {
        bool match = ( curves[i]->PointOn( crosspoints[i][jend] ) == curves[i]->PointOn( crosspoints[i][0] ) );
        if ( !match ) {
          cutcurves.push_back( curves[i]->Trimmed( crosspoints[i][jend], crosspoints[i][0], 1 ) );
        }
      }
      else { // замкнутая и без пересечений
        cutcurves.push_back(  curves[i]->Trimmed( curves[i]->GetTMin(), curves[i]->GetTMax(), 1 ) );
      }
    }
  }
}

//------------------------------------------------------------------------------
// Определить какие кривые из vestiges будут очерками силуэта
// Define which curves will be an outline of a silhouette
// ---
static
void ChooseSilhouetteOutlines( MbVEFVestiges & vestiges, MbPlacement3D & place, RPArray<MbLump> & lumps, RPArray<MbCurve> & curves )
{
  /// Собираем следы видимых ребер и граней в curves
  PArray<MbCurve> crvs( 0, 1, true );

  for ( size_t i = 0, count = vestiges.EdgeVestigesCount(); i < count; i++ ) { // Следы ребер, владеет
    MbCurveVestige & vest = vestiges.GetEdgeVestige( i )->SetCurveInfo(); // Информация о следе кривой
    size_t cnt = vest.GetVisiblePartsCount();
    if ( cnt > 0 ) {
      for ( size_t j = 0; j < cnt; ++j )
        crvs.Add( vest._PickupVisibleCurve( j ) );
    }
  }
  for ( size_t i = 0, count = vestiges.FaceVestigesCount(); i < count; i++ ) { // Следы граней, владеет
    PArray<MbCurveVestige> & curveInfos = vestiges.GetFaceVestige( i )->curveInfos;
    for ( ptrdiff_t k = 0, c = curveInfos.Count(); k < c; k++ ) { // Следы граней, владеет
      MbCurveVestige * vest = curveInfos[k]; 
      size_t cnt = vest->GetVisiblePartsCount();
      if ( cnt > 0 ) {
        for ( size_t j = 0; j < cnt; ++j )
          crvs.Add( vest->_PickupVisibleCurve( j ) );
      }
    }
  }

  // мы должны работать с нарезанными друг другом кривыми, т.к. часть кривой может быть границей, а часть - нет
  {
    PArray<MbCurve> cutcurves( 0, 1, true );
    ::CutAllCurves( crvs, cutcurves );
    crvs.Flush();
    cutcurves.OwnsElem( false );
    crvs.AddArray( cutcurves );
  }

  ::ChooseOutlineCurves( crvs, place, lumps );

  curves.SetSize( crvs.Count() );
  crvs.OwnsElem( false );
  curves.AddArray( crvs );
}

//------------------------------------------------------------------------------
// Построение очертаний силуэта тел
// place     - Проекционная плоскость.
// lumps     - Проецируемые объекты.
// curves    - Построенный кривые
// Building an outline of a silhouette
// place     - Projection plane
// lumps     - Projection items.
// curves    - Built curves
// ---
static
void SilhouetteOutline( MbPlacement3D & place, RPArray<MbLump> & lumps, RPArray<MbCurve> & curves )
{
  MbProjectionsObjects prObjects;
  MbVEFVestiges vestiges;
  bool merge = false; // Должно быть merge = false, иначе могут быть объединены участки, один из которых является очерком, а другой расположен над телом на переднем плане 
  MbMapVisibilityMode visMode( false, false );
  MbResultType res = ::GetVestiges( place, 0, lumps, prObjects, vestiges, visMode, Math::DefaultMathVersion(), merge );
  if ( res == rt_Success )
    ::ChooseSilhouetteOutlines( vestiges, place, lumps, curves );
  else
    ::PutErrorMessage( res, IDS_PROJECTION_BODIES );
  prObjects.Relinquish();
}

//------------------------------------------------------------------------------
// Создание очерка силуэтов тел
// Creation an outline of a silhouette
//---
void MakeOutlineOfSolids()
{
  PArray<MbLump> lumps( 1, 1, true );
  const size_t lumpsCnt = ::AddLumps( ts_negative, lumps ); // Добавить тела

  if ( lumpsCnt > 0 ) {
    MbPlacement3D place( TestVariables::viewManager->GetActiveWindow()->GetOwnPlace() );
    RPArray<MbCurve> curves;

    SetWaitCursor( true );
    uint miliSec = ::GetTickCountEx();
    {
      ::SilhouetteOutline( place, lumps, curves );
    }
    miliSec = ::GetTickCountEx() - miliSec;
    SetWaitCursor( false );

    size_t count = curves.Count();
    for ( size_t i = 0; i < count; i++ )
      TestVariables::viewManager->AddObject( TestVariables::BACKGROUND_Style, curves[i], &place );
    TimeTest * testResult = nullptr;
    ::ShowTimeTestResult( testResult, count, 0, miliSec, st_PlaneInstance, IDS_PROJECTION_BODIES );
  }
}


//------------------------------------------------------------------------------
// Создание вида контурного сечения
// Creation of a view of a contour section
//---
void MakeSolidContourSectionMap()
{
  c3d::SolidsSPtrVector solids;
  std::vector<MbMatrix3D> matrices;

  Style ns = TestVariables::SOLID_Style;
  MbCartPoint3D p0;

  bool getSolid = true;
  bool refreshDrawing = false; // KOMPAS-18629
  while ( getSolid ) {
    MbItem * solidItem = nullptr;
    PathMatrix pathFrom;
    if ( ::GetGObj( IDS_SELECT_SOLID, st_Solid, &solidItem, pathFrom ) ) {
      SPtr<MbSolid> solid( &static_cast<MbSolid &>(*solidItem) );
      solids.push_back( solid );
      matrices.push_back( pathFrom.second );
      TestVariables::viewManager->ToggleObject( solid, refreshDrawing );
    }
    else
      getSolid = false;
  }

  if ( !getSolid && solids.empty() ) {
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();

    if ( wind != nullptr ) {
      RPArray<MbItem> solidItems;
      SArray<MbMatrix3D> solidMatrices;
      wind->GetObjects( st_Solid, solidItems, solidMatrices );
      size_t solidsCnt = solidItems.size();
      solids.reserve( solidsCnt );
      matrices.reserve( solidsCnt );
      for ( size_t k = 0; k < solidsCnt; ++k ) {
        SPtr<MbSolid> solid( &static_cast<MbSolid &>(*solidItems[k]) );
        solids.push_back( solid );
        matrices.push_back( solidMatrices[k] );
      }
    }
  }

  // Собрать все пространственные кривые
  std::vector< SPtr<MbCurve3D> > curves;
  const size_t curvesCnt = ::AddSpaceCurves( curves );

  // Собрать все пространственные точки
  std::vector<MbCartPoint3D> points;
  const size_t pointsCnt = ::AddSpacePoints( points );

  PArray<MbName> namesStore( 0, 1, true ); // Хранилище имен
  RPArray<MbSpacePoints> pointsData( pointsCnt, 1 );
  RPArray<MbSpaceCurves> curvesData( curvesCnt, 1 );

  bool getPoints = true;
  bool getCurves = true;
  {
    MbMatrix3D from;
    if ( getPoints )
      ::FillPointsDataAddRef( points, from, pointsData, namesStore );
    if ( getCurves )
      ::FillCurvesDataAddRef( curves, from, curvesData, namesStore );
  }

  if ( !solids.empty() || !pointsData.empty() || !curvesData.empty() ) {
    MbItem * contItem = nullptr;
    if ( ::GetAnyCurve( IDS_SELECT_CNTR_ON_PLANE, &contItem ) ) {
      SPtr<MbItem> contourItem( contItem );
      MbPlacement3D pl;
      SPtr<MbContour> cntr( GetContourOnPlace(contItem, pl) );
      int side = 1;
      if ( cntr != nullptr && GetInt( IDS_ENTER_NUMBER_N, side ) ) {
        int smap_prop = 3;

        if ( GetInt( IDS_STRING_SMAP_PROP, smap_prop ) ) {
          int map_ = smap_prop>>1;          // Признак отображения
          int sec_ = smap_prop - (map_<<1); // Признак сечения

          PArray<MbMappingLumps> lumps( 0,1,true );
          for ( size_t i = 0, size = solids.size(); i < size; ++i ) {
            Style st( solids[i]->GetWidth(), solids[i]->GetColor() );
            lumps.push_back( new MbMappingLumps( *solids[i], matrices[i], sec_!=0, 0/*nullptr*/ ) );
          }

          ::SetWaitCursor( true );
          MbSNameMaker n( (SimpleName)ct_CuttingSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
          n.Add(1);

          SPtr<MbContour> copyContour( static_cast<MbContour *>(&cntr->Duplicate()) );
          bool shadeCurvesBySolids( false );
          if ( curvesCnt > 0 )
            shadeCurvesBySolids = GetBool( IDS_ENTER_SHADE_SPCURVES, shadeCurvesBySolids );
          MbMapVisibilityMode visMode( true, false, shadeCurvesBySolids );

          VERSION operVersion = Math::DefaultMathVersion();
          bool doCommonNaming = false;
          if ( operVersion >= MATH_20_TR_VERSION )
            doCommonNaming = true;

          MbSectionMapIteratorAbs & maps = MbSectionMapIterator::Create( lumps, pl, map_!=0, false, *copyContour, n, side, visMode, doCommonNaming, operVersion );

          maps.SetSpacePoints( pointsData );
          maps.SetSpaceCurves( curvesData );

          while ( maps.More() ) {
            MbSectionMap * secData = maps.Next();
            MbResultType res = maps.GetLastResult();

            if ( res == rt_Success ) {
              MbPlacement3D viewPos;

              PArray<MbCompContourArray> & secBounds = secData->GetSectionBounds();
              PArray<MbContour> arContour(1,1,false);
              ptrdiff_t i;
              for ( i = secBounds.MaxIndex(); i >= 0; i-- ) {
                arContour.Flush();
                secBounds[i]->GetContoursArray(arContour);
                secBounds[i]->DetachContours();

                for ( ptrdiff_t j = arContour.MaxIndex(); j >= 0; j-- )
                  TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, arContour[j], &viewPos );
              }

              MbVEFVestiges & vestiges = secData->GetSectionMap();

              const Style & visibleStyle = TestVariables::SURFACECURVE_Style;
              const Style & hiddenStyle  = TestVariables::HIDE_Style;
              const Style & sectionStyle = TestVariables::SURFACECURVE_Style;

              // Добавить следы проецирования тел в менеджер
              ::AddCopySolidVestiges( vestiges, viewPos, visibleStyle, hiddenStyle, sectionStyle, false );

              size_t spaceCnt = 0;
              ::AddOriginalPointsVestiges( vestiges, false, viewPos, spaceCnt );
              ::AddOriginalPointsVestiges( vestiges, true, viewPos, spaceCnt );
              ::AddOriginalCurvesVestiges( vestiges, false, viewPos, spaceCnt );
              ::AddOriginalCurvesVestiges( vestiges, true, viewPos, spaceCnt );
            }
            else
              ::PutErrorMessage( res, IDS_CURRENT_SECTION );
          }

          if ( maps.GetLastResult() != rt_Success )
            ::PutErrorMessage( maps.GetLastResult(), IDS_CURRENT_SECTION );

          maps.Release();

          ::SetWaitCursor( false );
        }
      }
    }
  }

  for ( size_t i = 0, solidsCnt = solids.size(); i < solidsCnt; ++i ) {
    bool drawRightNow = (i == solidsCnt-1);
    TestVariables::viewManager->ToggleObject( solids[i], drawRightNow );
  }

  // Удаление (освобождение) элементов множеств
  ::ReleaseItems( pointsData );
  ::ReleaseItems( curvesData );
}


//#define TEST_JUST_CUT_LIMIT_CURVE  // KOMPAS-22255, KOMPAS-41324

//------------------------------------------------------------------------------
// Создание вида местного разреза
// Creation of a view of a local section
//---
void MakeSolidLocalSectionMap()
{
  bool sec = true;
  MbMatrix3D from;
  PArray<MbMappingLumps> mapLumps( 1, 1, true );

  ::AddMappingLumps( ts_neutral, sec, mapLumps ); // Собрать тела

  SPtr<ThreadsStore> threadsStore( new ThreadsStore );

  // Собрать все пространственные кривые
  std::vector< SPtr<MbCurve3D> > curves;
  const size_t curvesCnt = ::AddSpaceCurves( curves );

  // Собрать все пространственные точки
  std::vector<MbCartPoint3D> points;
  const size_t pointsCnt = ::AddSpacePoints( points );

  PArray<MbName> namesStore( 0, 1, true ); // Хранилище имен
  RPArray<MbSpacePoints> pointsData( pointsCnt, 1 );
  RPArray<MbSpaceCurves> curvesData( curvesCnt, 1 );
  ::FillPointsDataAddRef( points, from, pointsData, namesStore );
  ::FillCurvesDataAddRef( curves, from, curvesData, namesStore );

  if ( (mapLumps.size() > 0) || (pointsData.size() > 0) || (curvesData.size() > 0) ) {
    MbItem * conObj = nullptr;

    if ( ::GetAnyCurve( IDS_SELECT_CNTR_ON_PLANE, &conObj ) ) {
      MbPlacement3D pl;
      SPtr<MbContour> cntr( ::GetContourOnPlace( conObj, pl ) );

      int side = 1;
      if ( (cntr != nullptr) && ::GetInt( IDS_ENTER_NUMBER_N, side ) ) {
        if ( side < 0 ) {
          MbMatrix matr;
          pl.Invert( &matr ); // Повернуть вокруг оси X на M_PI
          cntr->Transform( matr );
        }

#ifdef TEST_JUST_CUT_LIMIT_CURVE // KOMPAS-22255, KOMPAS-41324
        {
          double znear = 0.0;
          MbProjectionsObjects projObjects;
          MbMapVisibilityMode visMode( false, true );
          MbVEFVestiges vestiges;
          size_t k, lumpsCnt = mapLumps.size();

          MbAccurateTimer ac;
          
          RPArray<MbLump> lumps;
          lumps.reserve( lumpsCnt );
          for ( k = 0; k < lumpsCnt; ++k ) {
            // mapLumps[k]->SetCuttingType( false ); // Для KOMPAS-57756.
            lumps.push_back( mapLumps[k] );
          }

          ac.Begin();
          ::GetVestiges( pl, znear, lumps, projObjects, vestiges, visMode );

          RPArray<MbCurve> limits;
          ::AddCopySolidVestiges( vestiges, true, limits );

          if ( limits.size() > 0 ) {
            size_t limitsCnt = limits.size();
            MbRect rect;
            cntr->AddYourGabaritTo( rect );
            SArray<MbCrossPoint> crossPoints;
            for ( k = 0; k < limitsCnt; ++k ) {
              MbRect rect2;
              limits[k]->AddYourGabaritTo( rect2 );
              if ( rect.Intersect( rect2 ) ) {
                ::IntersectTwoCurves( *limits[k], *cntr, crossPoints, true );
              }
            }

            MbMapViewInfo baseViewInfo( mvt_View, pl );
            MbMapViewInfo localViewInfo( mvt_Cut, pl );

            RPArray<MbCurve> cutCurves;
            ::JustCutLimitCurve( lumps, *cntr, crossPoints, baseViewInfo, localViewInfo, cutCurves );

            MbPlacement3D dummyPlace( pl );
            MbVector3D to( pl.GetAxisZ() );
            to *= COUNT_DELTA;
            dummyPlace.Move( to );
            for ( k = cutCurves.size(); k--; ) {
              TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, cutCurves[k], &pl, false );
            }

            ::DeleteItems( cutCurves );
          }
          ac.End();

          ::DeleteItems( limits );
          TestVariables::viewManager->ShowModel();

          if ( TestVariables::checkOperationTime ) {
            const size_t bufLen = 1024;
            TCHAR buf[bufLen];
            size_t len = 0;
            len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
            ::MessageBoxEx( buf, IDS_RESULTS );
          }
        }
#else
        MbSNameMaker n( (SimpleName)ct_CuttingSolid, MbSNameMaker::i_SideNone, 0 );
        n.Add(1);

        MbAccurateTimer ac;

        MbResultType secResult = rt_Error;
        MbMapVisibilityMode visMode( true, false );

        VERSION operVersion = Math::DefaultMathVersion();
        bool doCommonNaming = false;
        if ( operVersion >= MATH_20_TR_VERSION )
          doCommonNaming = true;

        ::SetWaitCursor( true );
        ac.Begin();
        MbSectionMapIteratorAbs & iter = MbLocalSectionMapIterator::Create( mapLumps, pl, sec, false, *cntr, n, visMode, doCommonNaming );

        iter.SetAnnObjectStore( threadsStore ); // C3D-695 : KOMPAS-25125
        iter.SetSpacePoints( pointsData );
        iter.SetSpaceCurves( curvesData );

        MbSectionMap * secData =  iter.Next();
        ac.End();
        ::SetWaitCursor( false );

        if ( secData != nullptr ) {
          ptrdiff_t i, count;
          // Отобразить отсеченные тела
          RPArray<MbFaceShell> shells(5,5);
          secData->DetachShellArray( shells );

          bool showItemsRightNow = false; // KOMPAS-18629

          for ( i = 0, count = shells.Count(); i < count; i++ ) {
            TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, new MbSolid( shells[i], nullptr ), showItemsRightNow );
          }

          MbPlacement3D viewPos(pl);
          MbVEFVestiges & vestiges = secData->GetSectionMap();

          const Style & visibleStyle = TestVariables::BACKGROUND_Style;
          const Style & hiddenStyle  = TestVariables::HIDE_Style;
          const Style & sectionStyle = TestVariables::PPOINT_Style;

          // Добавить следы проецирования тел в менеджер
          ::AddCopySolidVestiges( vestiges, viewPos, visibleStyle, hiddenStyle, sectionStyle, showItemsRightNow );

          size_t spaceCnt = 0;
          ::AddOriginalPointsVestiges( vestiges, false, viewPos, spaceCnt );
          ::AddOriginalPointsVestiges( vestiges, true,  viewPos, spaceCnt );
          ::AddOriginalCurvesVestiges( vestiges, false, viewPos, spaceCnt );
          ::AddOriginalCurvesVestiges( vestiges, true,  viewPos, spaceCnt );
          size_t annCnt = 0;
          ::AddOriginalAnnVestiges( vestiges, true,  viewPos, annCnt );
          ::AddOriginalAnnVestiges( vestiges, false, viewPos, annCnt );

          // Отобразить сечение
          PArray<MbCompContourArray> & secBounds = secData->GetSectionBounds();
          PArray<MbContour> arContour(1,1,false);

          for ( i = 0, count = secBounds.Count(); i < count;i++ ) {
            arContour.Flush();
            secBounds[i]->GetContoursArray(arContour);
            secBounds[i]->DetachContours();

            if ( arContour.Count() > 0 ) {
              ptrdiff_t lastInd = arContour.Count() - 1;
              for ( ptrdiff_t j = 0; j < lastInd; j++ )
                TestVariables::viewManager->AddObject( TestVariables::CONIC_Style, arContour[j], &viewPos, showItemsRightNow );
              TestVariables::viewManager->AddObject( TestVariables::CONIC_Style, arContour[lastInd], &viewPos, true );
            }
          }
        }              
        else {
          secResult = iter.GetLastResult();
          ::PutErrorMessage( secResult, IDS_LOCAL_SECTION_ERROR );
        }

        iter.Release();

        if ( TestVariables::checkOperationTime ) {
          const size_t bufLen = 1024;
          TCHAR buf[bufLen];
          size_t len = 0;
          len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
          ::MessageBoxEx( buf, IDS_RESULTS );
        }
#endif  // TEST_JUST_CUT_LIMIT_CURVE
      }
      TestVariables::viewManager->ToggleObject( conObj );
    }
  }

  // Удаление (освобождение) элементов множеств
  ::ReleaseItems( pointsData );
  ::ReleaseItems( curvesData );
}


//------------------------------------------------------------------------------
// Проецирование ребра на плоскую грань
// Projection of an edge on a flat face
// ---
bool CreateEdgeProjection( bool plan )
{
  bool res = false;
  MbItem * faceObj = nullptr;
  MbItem * edgeObj = nullptr;

  MbTopItem * faceItem = nullptr;
  MbTopItem * edgeItem = nullptr;

  if ( ::GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &faceObj, &faceItem ) ) {
    MbPlacement3D place;

    if ( static_cast<const MbFace *>(faceItem)->GetPlanePlacement( place ) ) {
      TestVariables::drawImp->DrawItem( faceItem, TRGB_WHITE );

      place.Update( MbPlacement3D::global, Math::DefaultMathVersion() ); // Метод инициализации для привязки плейсмента к другим координатам

      MbMatrix3D into;
      place.GetMatrixInto( into );

      if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &edgeObj, &edgeItem ) ) {
        SetWaitCursor( true );

        const MbCurve3D & curve = static_cast<const MbCurveEdge *>(edgeItem)->GetCurve();

        TestVariables::drawImp->DrawItem( edgeItem, TRGB_LIGHTBLUE );

        // Дать плоскую проекцию кривой ((матрица преобразования в видовую плоскость, параметрическая область)
        SPtr<MbCurve> pCurve( ::GetFlatCurve( curve, into, nullptr ) ); // MbCurve * pCurve = curve.GetMap( into, nullptr );

#ifdef C3D_DEBUG
        { // Тест соответствия начальной точки кривой и ее проекции (SD#7117319)
          MbCartPoint3D pnt1, pnt2;
          curve.GetLimitPoint( 1, pnt1 );
          MbCartPoint uv;
          pCurve->GetLimitPoint( 1, uv );
          place.PointOn( uv, pnt2 );
          MbVector3D n1( pnt1, pnt2 ), n2;
          place.Normal( n2 );
          PRECONDITION( n1.Colinear( n2 ) );
        }
#endif
        SetWaitCursor( false );

        if ( pCurve != nullptr ) {
          if ( plan )
            TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, pCurve, &place );
          else
            TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, new MbPlaneCurve( place, *pCurve, true ) );
          res = true;
        }

        TestVariables::drawImp->DrawItem( edgeItem, TRGB_LIGHTGREEN );
      }

      TestVariables::drawImp->DrawItem( faceItem, TRGB_LIGHTGREEN );
    }
  }

  return res;
}


//------------------------------------------------------------------------------
// Создание вспомогательного объекта по трем точкам
// Construct an assisting item by three points
// ---
bool CreateConstuctiveByPoints()
{
  bool result = false;

  MbCartPoint3D p[3];

  while ( GetPoint(IDS_SELECT_POINT, p[0]) ) {
    TestVariables::drawImp->DrawPoint( &p[0], TRGB_LIGHTGREEN );
    if ( GetPoint(IDS_SELECT_POINT, p[1]) ) {
      TestVariables::drawImp->DrawPoint( &p[1], TRGB_LIGHTGREEN );
      if ( GetPoint(IDS_SELECT_POINT, p[2]) ) {
        TestVariables::drawImp->DrawPoint( &p[2], TRGB_LIGHTGREEN );

        MbPlacement3D tmpPlace( p[0], p[1], p[2], false );
        MbAssistingItem * newConstr = new MbAssistingItem( tmpPlace );

        result = TestVariables::viewManager->AddObject( TestVariables::ELEMENTARY_Style, newConstr ) != nullptr;
      }
    }
  }
  return result;
}


//------------------------------------------------------------------------------
// Построение линий пересечения между поверхностью и телом
// Create an intersection of surface and solid
// ---
void CreateSurfaceSolidIntersection()
{
  MbItem * obj1 = nullptr;
  MbItem * obj2 = nullptr;
  MbTopItem * face = nullptr;
  RPArray<MbCurve3D> cross( 4, 4 );    //-V112

  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &obj1 ) ) {
    TestVariables::viewManager->ToggleObject(obj1);
    if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj2, &face ) ) {
      TestVariables::viewManager->ToggleObject(obj2);
      SetWaitCursor( true );

      MbSurface * surf1 = (MbSurface *)((MbSpaceInstance *)obj1)->GetSpaceItem();
      MbSolid * solid = (MbSolid *)obj2;

      MbSNameMaker snMaker;
      MbIntCurveParams params( snMaker );

      for ( ptrdiff_t i = 0; i < (ptrdiff_t)solid->GetFacesCount(); i++ ) {
        const MbSurface & surf2 = solid->GetFace(i)->GetSurface();
        MbWireFrame * tmpFrame = nullptr;
        MbIntCurveResults intCurveResult;
        ::IntersectionCurve( *surf1, surf2, params, intCurveResult );
        tmpFrame = intCurveResult._wireFrame.detach();
        ::ExtractCurvesDeleteFrame( tmpFrame, cross );
      }

      SetWaitCursor( false );
      TestVariables::viewManager->ToggleObject(obj2);
    }
    TestVariables::viewManager->ToggleObject(obj1);
  }
  for ( ptrdiff_t i = 0; i < (ptrdiff_t)cross.Count(); i++ )
    TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, cross[i] );

}


//------------------------------------------------------------------------------
// Построение линий пересечения между двумя телами
// Create an intersection of two solids
// ---
void CreateSolidIntersectionCurve()
{
  MbItem * obj1 = nullptr;
  MbItem * obj2 = nullptr;

  static bool keepWireFrame = false;

  if ( GetGObj( IDS_SELECT_FIRST_SOLID, st_Solid, &obj1 ) ) {
    TestVariables::viewManager->ToggleObject(obj1);

    if ( GetGObj( IDS_SELECT_SECOND_SOLID, st_Solid, &obj2 ) ) {
      TestVariables::viewManager->ToggleObject(obj2);
      SetWaitCursor( true );

      MbSolid * solid1 = static_cast<MbSolid *>(obj1);
      MbSolid * solid2 = static_cast<MbSolid *>(obj2);

      if ( solid1 != nullptr && solid2 != nullptr ) {
        MbSNameMaker snMaker( SimpleName(-1), MbSNameMaker::i_SideNone, 0 );

        c3d::IndicesVector faceIndices1, faceIndices2;

        for ( size_t i = 0, iCnt = solid1->GetFacesCount(); i < iCnt; ++i )
          faceIndices1.push_back( i );
        for ( size_t i = 0, iCnt = solid2->GetFacesCount(); i < iCnt; ++i )
          faceIndices2.push_back( i );

        MbAccurateTimer ac;

        MbWireFrame * resFrame = nullptr;
        bool same1 = false;
        bool same2 = false;

        MbIntCurveShellParams shellParams( faceIndices1, same1, faceIndices2, same2, snMaker );
        MbIntCurveResults intCurveResults;

        ac.Begin();
        ::IntersectionCurve( *solid1, *solid2, shellParams, intCurveResults );
        ac.End();

        resFrame = intCurveResults._wireFrame.detach();

        keepWireFrame = GetBool( IDS_SELECT_WIREFRAME, keepWireFrame );

        if ( keepWireFrame ) {
          TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, resFrame );
        }
        else {
          SpaceCurvesSPtrVector cross;
          ::ExtractCurvesDeleteFrame( resFrame, cross );

          size_t crossCnt = cross.size();

          if ( crossCnt > 0 ) {
            TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, cross[0], false );
            for ( size_t i = 1; i < crossCnt; ++i ) {
              cross[i] = static_cast<MbCurve3D *>(&cross[i]->Duplicate()); // недостатки паралелльной отрисовки с блокировкой только по MbItem
              if ( i + 1 < crossCnt )
                TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, cross[i], false );
              else
                TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, cross[i], true );
            }
          }
        }
      }
      else
      {  
        MessageBoxEx( RT_CROSS_LINE_NOT_FOUND, IDS_TITLE_ERR );
      }
      SetWaitCursor( false );
      TestVariables::viewManager->ToggleObject(obj2);
    }

    TestVariables::viewManager->ToggleObject(obj1);
  }
}


//------------------------------------------------------------------------------
// Построение линий пересечения между гранями тел
// Create an intersection of faces of solids
// ---
bool CreateFacesIntersectionCurve()
{
  bool r = false;
  MbItem * obj1, * obj2;
  obj1 = obj2 = nullptr;
  MbTopItem * titem1, * titem2;
  titem1 = titem2 = nullptr;
  MbWireFrame * resFrame = nullptr;

  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj1, &titem1 ) ) {
    TestVariables::viewManager->ToggleObject( obj1 );
    if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj2, &titem2 ) ) {
      TestVariables::viewManager->ToggleObject( obj2 );
      SetWaitCursor( true );

      if ( titem1->IsAFace() && titem2->IsAFace() ) {
        MbFace * face1 = (MbFace *)titem1;
        MbFace * face2 = (MbFace *)titem2;
        bool bCopy = false;

        MbSNameMaker snMaker;
        MbIntCurveParams params( snMaker );
        
        MbIntCurveResults results;
        if ( bCopy ) {
          face1 = face1->DataDuplicate();
          face2 = face2->DataDuplicate();
          ::IntersectionCurve( *face1, *face2, params, results );
          ::DeleteItem( face1 );
          ::DeleteItem( face2 );
        }
        else
          ::IntersectionCurve( *face1, *face2, params, results );
        resFrame = results._wireFrame.detach();
      }
      
      SetWaitCursor( false );
      TestVariables::viewManager->ToggleObject(obj2);
    }
    TestVariables::viewManager->ToggleObject(obj1);
  }

  if ( resFrame != nullptr ) { // проблемы с распараллеливанием
    MbWireFrame * rf = static_cast<MbWireFrame *>( &resFrame->Duplicate() );
    ::DeleteItem( resFrame );
    resFrame = rf;
  }
  TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, resFrame );

  return r;
}


//------------------------------------------------------------------------------
//  Построить окружность или дугу для радиального размера к поверхности по точке.
//  Construct a circle or an arc for radial dimension of surface by point. 
// ---
void CreateRadiusDimension3D()
{
  MbItem * obj = nullptr;
  MbTopItem * item = nullptr;
  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &item ) ) {
    MbFace * face = (MbFace *)item;
    const MbSurface * surface = &face->GetSurface();
    MbCartPoint3D point3d;
    while ( GetPoint( IDS_SELECT_POINT, point3d ) ) {
      if ( surface != nullptr ) {
        MbCartPoint point2d;
        if ( surface->NearPointProjection(point3d, point2d.x, point2d.y, false) ) {
          MbPlaneCurve * curve3d = nullptr;
          MbPlacement3D place;
          ::RadiusDimension3D( *surface, point2d, curve3d );
          TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, curve3d );
        } 
        else
          PRECONDITION(false);
      }
      else
        PRECONDITION(false);
    }
  }
  else
    PRECONDITION(false);
}
