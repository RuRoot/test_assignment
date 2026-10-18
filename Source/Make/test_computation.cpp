#include <test_computation.h>
#include <test_manager.h>
#include <test.h>
#include <test_draw.h>
#include <test_service.h>
#include <test_rendering.h>
#include <test_converter.h>

#include <mb_class_traits.h>
#include <mb_placement.h>
#include <mb_axis3d.h>
#include <mb_class_traits.h>

#include <cur_hermit.h>
#include <cur_nurbs.h>
#include <cur_line_segment.h>
#include <cur_polyline.h>
#include <curve3d.h>
#include <cur_line_segment3d.h>
#include <cur_surface_intersection.h>
#include <cur_contour3d.h>
#include <cur_polycurve3d.h>
#include <cur_plane_curve.h>
#include <surf_plane.h>
#include <surf_spline_surface.h>
#include <surf_ruled_surface.h>
#include <surf_cylinder_surface.h>
#include <surf_cone_surface.h>
#include <surf_curve_bounded_surface.h>

#include <plane_instance.h>
#include <space_instance.h>
#include <point_frame.h>
#include <wire_frame.h>
#include <point3d.h>
#include <region.h>

#include <alg_silhouette_hide.h>
#include <alg_dimension.h>
#include <alg_max_distance.h>
#include <alg_base.h>
#include <action_direct.h>
#include <action_general.h>
#include <action_analysis.h>
#include <action_curve3d.h>
#include <action_point.h>
#include <action_phantom.h>
#include <action_surface.h>
#include <action_surface_curve.h>
#include <action_solid.h>
#include <action_mesh.h>
#include <check_geometry.h>
#include <tri_face.h>
#include <mip_curve_properties.h>
#include <mip_solid_mass_inertia.h>
#include <mip_solid_area_volume.h>
#include <cdet_bool.h>
#include <cdet_data.h>
#include <cdet_utility.h>
#include <map_thread.h>
#include <tool_time_test.h>
#include <mb_cross_point.h>
#include <time.h>
#include <tool_progress_indicator.h>
#include <test_gr_draw.h>
#include <sstream>
#include <last.h>


using namespace c3d;


#ifdef _DRAWGI  // включатели отрисовки во время отладки
namespace DrawComputation {
  bool drawItem  = false;
  bool drawDefault = true;
}
#endif // _DRAWGI


//------------------------------------------------------------------------------
// Габарит кривой
// Dimensions of the curve
// ---
bool PutCurveGabarit()
{
  MbItem * nearest = nullptr;

  if ( GetAnyCurve( IDS_SELECT_CURVE, &nearest ) ) 
  {
    TestVariables::viewManager->ToggleObject(nearest);
    
    if ( const MbSpaceInstance * inst = isa_cast<const MbSpaceInstance *>(nearest) ) 
    {
      // Дать трёхмерный геометрический объекта
      // Get the spatioal object
      if ( const MbSpaceItem * spaceItem = inst->GetSpaceItem() )  
      {
        MbCube gab;
        spaceItem->AddYourGabaritTo( gab );
        TestVariables::viewManager->ToggleObject(nearest);
//#ifdef _DRAWGI
        // Нарисовать габарит
        // Draw the dimensions
        TestVariables::drawImp->PutCube(gab);
//#endif // _DRAWGI
        return true;
      }
    }
    if ( nearest->IsA() == st_PlaneInstance ) {
      // Дать двумерный геометрический объект
      // Get the spatial object
      const MbPlaneItem * planeItem = ((MbPlaneInstance *)nearest)->GetPlaneItem(); 
      if ( planeItem != nullptr ) {
        MbRect rect;
        planeItem->AddYourGabaritTo( rect );
//#ifdef _DRAWGI
        // Дать плейсемент
        // Get the location
        const MbPlacement3D & pl = TestVariables::viewManager->GetCplane();
        MbCartPoint3D q1, q2;
        pl.PointOn( rect.left, rect.bottom, q1 ); 
        pl.PointOn( rect.right, rect.bottom, q2 ); 
        TestVariables::drawImp->DrawLine( q1, q2, TRGB_LIGHTRED );
        pl.PointOn( rect.left, rect.top, q1 ); 
        pl.PointOn( rect.right, rect.top, q2 ); 
        TestVariables::drawImp->DrawLine( q1, q2, TRGB_LIGHTRED );
        pl.PointOn( rect.left, rect.bottom, q1 ); 
        pl.PointOn( rect.left, rect.top, q2 ); 
        TestVariables::drawImp->DrawLine( q1, q2, TRGB_LIGHTRED );
        pl.PointOn( rect.right, rect.bottom, q1 ); 
        pl.PointOn( rect.right, rect.top, q2 ); 
        TestVariables::drawImp->DrawLine( q1, q2, TRGB_LIGHTRED );
//#endif // _DRAWGI
      }
    }
    if ( nearest->IsA() == st_WireFrame ) {
      const MbEdge * edge = ((MbWireFrame *)nearest)->GetEdge( 0 ); 
      if ( edge != nullptr ) {
        MbCube gab;
        edge->AddYourGabaritTo( gab );
        TestVariables::viewManager->ToggleObject(nearest);
//#ifdef _DRAWGI
        // Нарисовать габарит
        // Draw the dimensions
        TestVariables::drawImp->PutCube(gab);
//#endif // _DRAWGI
        return true;
      }
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Габарит поверхности
// Dimensions of the surface
// ---
bool PutSurfaceGabarit()
{
  MbItem * nearest = nullptr;
  PathMatrix pathFrom;

  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &nearest, pathFrom ) ) {
    TestVariables::viewManager->ToggleObject(nearest);

    MbCube gab;
    nearest->AddYourGabaritTo( gab );

    TestVariables::viewManager->ToggleObject(nearest);

//#ifdef _DRAWGI
    // Нарисовать габарит
    // Draw the dimensions
    TestVariables::drawImp->PutCube(gab);
//#endif // _DRAWGI

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Габарит грани тела
// Dimensions of the solid's face
// ---
bool PutFaceGabarit()
{
  MbItem *obj = nullptr;
  MbTopItem *surface;

  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &surface ) ) {
    MbFace * face = (MbFace *)surface;

//#ifdef _DRAWGI
    // Нарисовать грань
    // Draw the face
    TestVariables::drawImp->DrawItem( face, TRGB_WHITE );
//#endif // _DRAWGI

    MbCube gab;
    face->AddYourGabaritTo( gab );

//#ifdef _DRAWGI
    // Нарисовать габарит
    // Draw the dimensions
    TestVariables::drawImp->PutCube(gab);
//#endif // _DRAWGI

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Габарит тела
// dimensions of the solid
// ---
bool PutSolidGabarit()
{
  MbItem * nearest = nullptr;
  PathMatrix pathFrom;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &nearest, pathFrom ) ) {
    TestVariables::viewManager->ToggleObject(nearest);

    MbCube gab;
    nearest->AddYourGabaritTo( gab );

    TestVariables::viewManager->ToggleObject(nearest);

//#ifdef _DRAWGI
    // Нарисовать габарит
    // Draw the dimensions
    TestVariables::drawImp->PutCube(gab);
//#endif // _DRAWGI

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Габарит относительно локальной с.к.
// Dimensions of the planar curve relative to the placement
// ---
bool PutCurve2DLocalGabarit()
{
  bool bRes = false;
  MbItem * obj = nullptr;

  if ( GetMObj( IDS_SELECT_CRV_ON_PLANE, pt_Curve, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    const MbCurve * curve = static_cast<const MbCurve *>( static_cast<const MbPlaneInstance *>(obj)->GetPlaneItem() );
    
    MbCartPoint p0, px, py;
    p0.Init( 0, 0 );
    px.Init( 1, 0 );
    py.Init( 0, 1 );

    if ( GetPoint( IDS_SELECT_POINT, p0 ) ) {

//#ifdef _DRAWGI
      TestVariables::drawImp->DrawPoint( &p0, TRGB_LIGHTGREEN );
//#endif // _DRAWGI

      if ( GetPoint( IDS_SELECT_POINT, px ) ) {

//#ifdef _DRAWGI
        TestVariables::drawImp->DrawPoint( &px, TRGB_LIGHTGREEN );
//#endif // _DRAWGI

        MbPlacement place( p0, px );

        ptrdiff_t iLeftPlace = 0;
        GetInt( IDS_ENTER_PLACE_TYPE, iLeftPlace );
        if ( iLeftPlace != 0 )
          place.InvertAxisY();
        
        double d = std_max( 1.0, p0.DistanceToPoint( px ) );
        MbVector vx( place.GetAxisX() );
        MbVector vy( place.GetAxisY() );
        px.Set( p0, vx, d );
        py.Set( p0, vy, d );
//#ifdef _DRAWGI
        TestVariables::drawImp->DrawLine( p0, px, 192, 0, 0 );
        TestVariables::drawImp->DrawLine( p0, py, 0, 192, 0 );
        TestVariables::drawImp->DrawPoint( &p0, TRGB_BLACK );
//#endif // _DRAWGI
        
        MbRect rect;
        MbMatrix matrInto;
        MbMatrix matrFrom;

        place.GetMatrixInto( matrInto );
        place.GetMatrixFrom( matrFrom );

        curve->CalculateLocalGabarit( matrInto, rect );

        ptrdiff_t n;
        const ptrdiff_t nCorners = 4;              //-V112
        MbCartPoint pCorners[nCorners];

        pCorners[0].Init( rect.left,  rect.bottom );
        pCorners[1].Init( rect.right, rect.bottom );
        pCorners[2].Init( rect.right, rect.top );
        pCorners[3].Init( rect.left,  rect.top );
                
        for ( n = 0; n < nCorners; n++ )
          pCorners[n].Transform( matrFrom );

        MbContour * cntr = new MbContour();
        for ( n = 0; n < nCorners; n++ ) {
          const MbCartPoint & p1 = pCorners[n];
          const MbCartPoint & p2 = pCorners[(n+1)%nCorners];
          MbLineSegment * ls = new MbLineSegment( p1, p2 );
          cntr->SegmentsAdd( *ls );
        }
        TestVariables::viewManager->AddObject( TestVariables::LINE_Style, cntr );

        bRes = true;
      }
    }    
    TestVariables::viewManager->UnselectObject( obj );
  }

  return bRes;
}


//------------------------------------------------------------------------------
// Создать контура локального габаритного куба в глобальной с.к.
// Create the contour of the dimensions volume in the world location
// ---
static void CreateLocalCubeContours( const MbCube & cube, const MbMatrix3D & matrFrom, Style st = TestVariables::LINE_Style )
{
  ptrdiff_t n = 0;
  const ptrdiff_t nCorners = 8;
  MbCartPoint3D pCorners[nCorners];

  Style style = matrFrom.IsSingle() ? TestVariables::WIRE_Style : TestVariables::LINE_Style;
  if ( st != TestVariables::LINE_Style )
    style = st;
            
  for ( n = 0; n < nCorners; n++ ) {
    MbCartPoint3D pCorner;
    cube.GetVertex( n, pCorner );
    pCorner.Transform( matrFrom );
    pCorners[n].Init( pCorner );
  }

  MbContour3D * cntr = nullptr;

  cntr = new MbContour3D;
  for ( n = 0; n < 4; n++ ) {              //-V112
    const MbCartPoint3D & p1 = pCorners[n];
    const MbCartPoint3D & p2 = ((n < 3) ? pCorners[n+1] : pCorners[0]);
    MbLineSegment3D * ls = new MbLineSegment3D( p1, p2 );
    cntr->AddSegment( *ls, true );
  }
  TestVariables::viewManager->AddObject( style, cntr );

  cntr = new MbContour3D;
  for ( n = 4; n < 8; n++ ) {              //-V112
    const MbCartPoint3D & p1 = pCorners[n];
    const MbCartPoint3D & p2 = ((n < 7) ? pCorners[(n+1)%8] : pCorners[4]); //-V112
    MbLineSegment3D * ls = new MbLineSegment3D( p1, p2 );
    cntr->AddSegment( *ls, true );
  }
  TestVariables::viewManager->AddObject( style, cntr );

  MbLineSegment3D * ls1, * ls2, * ls3, * ls4;
  ls1 = ls2 = ls3 = ls4 = nullptr;
            
  cntr = new MbContour3D;
  ls1 = new MbLineSegment3D( pCorners[0], pCorners[4] );              //-V112
  ls2 = new MbLineSegment3D( pCorners[4], pCorners[7] );              //-V112
  ls3 = new MbLineSegment3D( pCorners[7], pCorners[3] );
  ls4 = new MbLineSegment3D( pCorners[3], pCorners[0] );
  cntr->AddSegment( *ls1, true );
  cntr->AddSegment( *ls2, true );
  cntr->AddSegment( *ls3, true );
  cntr->AddSegment( *ls4, true );
  TestVariables::viewManager->AddObject( style, cntr );

  cntr = new MbContour3D;
  ls1 = new MbLineSegment3D( pCorners[1], pCorners[5] );
  ls2 = new MbLineSegment3D( pCorners[5], pCorners[6] );
  ls3 = new MbLineSegment3D( pCorners[6], pCorners[2] );
  ls4 = new MbLineSegment3D( pCorners[2], pCorners[1] );
  cntr->AddSegment( *ls1, true );
  cntr->AddSegment( *ls2, true );
  cntr->AddSegment( *ls3, true );
  cntr->AddSegment( *ls4, true );
  TestVariables::viewManager->AddObject( style, cntr );
}


//------------------------------------------------------------------------------
// 
// ---
static bool SelectLocalPlacementByPoints( MbPlacement3D & localPlace )
{
  MbCartPoint3D p0, px, py, pz;
  p0.Init( 0, 0, 0 );
  px.Init( 1, 0, 0 );
  py.Init( 0, 1, 0 );
  pz.Init( 0, 0, 1 );

  if ( GetPoint( IDS_SELECT_POINT, p0 ) ) {
//#ifdef _DRAWGI
    TestVariables::drawImp->DrawPoint( &p0, TRGB_LIGHTGREEN );
//#endif // _DRAWGI

    if ( GetPoint( IDS_SELECT_POINT, px ) ) {
//#ifdef _DRAWGI
      TestVariables::drawImp->DrawPoint( &px, TRGB_LIGHTGREEN );
//#endif // _DRAWGI

      if ( GetPoint( IDS_SELECT_POINT, py ) ) {
//#ifdef _DRAWGI
        TestVariables::drawImp->DrawPoint( &py, TRGB_LIGHTGREEN );
//#endif // _DRAWGI

        ptrdiff_t iLeftPlace = 0;
        GetInt( IDS_ENTER_PLACE_TYPE, iLeftPlace );
        bool bLeft = (iLeftPlace != 0);

        MbPlacement3D place( p0, px, py, bLeft );

        double s = 1.0;
        place.Scale( s );

        localPlace.Init( place );

//#ifdef _DRAWGI
        double d = std_max( 1.0, std_max( p0.DistanceToPoint( px ), p0.DistanceToPoint( py ) ) );
        TestVariables::drawImp->DrawPlacement3D( localPlace, d );
//#endif // _DRAWGI

        return true;
      }
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Габарит относительно локальной с.к.
// Dimensions of the spatial curve relative to the placement
// ---
bool PutCurve3DLocalGabarit()
{
  bool bRes = false;
  MbItem * obj = nullptr;
  PathMatrix pathFrom;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj, pathFrom ) && (obj->IsA() == st_SpaceInstance) ) {
    TestVariables::viewManager->ToggleObject(obj);

    MbCurve3D * curve = (MbCurve3D *)((MbSpaceInstance *)obj)->GetSpaceItem();
    MbPlacement3D place;

    if ( ::SelectLocalPlacementByPoints( place ) ) {
      MbCube cube;
      MbMatrix3D matrInto;
      MbMatrix3D matrFrom;

      place.GetMatrixInto( matrInto );
      place.GetMatrixFrom( matrFrom );

      curve->CalculateLocalGabarit( matrInto, cube );
      ::CreateLocalCubeContours( cube, matrFrom );

      bRes = true;
    }

    TestVariables::viewManager->UnselectObject( obj );
  }

  return bRes;
}


//------------------------------------------------------------------------------
// Габарит относительно локальной с.к.
// Dimensions of the surface relative to the placement
// ---
bool PutSurfaceLocalGabarit()
{
  bool bRes = false;
  MbItem * obj = nullptr;
  PathMatrix pathFrom;

  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &obj, pathFrom ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    MbSurface * surface = (MbSurface *)((MbSpaceInstance *)obj)->GetSpaceItem();
    MbPlacement3D place;

    if ( ::SelectLocalPlacementByPoints( place ) ) {
      MbCube cube;
      MbMatrix3D matrInto;
      MbMatrix3D matrFrom;

      // Габарит относительно глобальной с.к.
      // Dimensions in the world placement
      surface->CalculateGabarit( cube );
      ::CreateLocalCubeContours( cube, matrFrom );

      place.GetMatrixInto( matrInto );
      place.GetMatrixFrom( matrFrom );

      surface->CalculateLocalGabarit( matrInto, cube );
      ::CreateLocalCubeContours( cube, matrFrom );

      bRes = true;
    }

    TestVariables::viewManager->UnselectObject( obj );
  }

  return bRes;
}


//------------------------------------------------------------------------------
// Габарит относительно локальной с.к.
// Dimensions of the edge relative to the placement
// ---
bool PutEdgeLocalGabarit()
{
  bool bRes = false;
  MbItem * obj = nullptr;
  MbTopItem * item = nullptr;

  if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &item ) ) {
    MbCurveEdge * edge = (MbCurveEdge *)item;
    const MbSurfaceIntersectionCurve * curve = &edge->GetIntersectionCurve();
    MbPlacement3D place;

    if ( ::SelectLocalPlacementByPoints( place ) ) {
      MbCube cube;
      MbMatrix3D matrInto;
      MbMatrix3D matrFrom;

      place.GetMatrixInto( matrInto );
      place.GetMatrixFrom( matrFrom );

      curve->CalculateLocalGabarit( matrInto, cube );
      ::CreateLocalCubeContours( cube, matrFrom );

      bRes = true;
    }
  }

  return bRes;
}


//------------------------------------------------------------------------------
// Габарит относительно локальной с.к.
// Dimensions of the solids's face relative to the placement
// ---
bool PutFaceLocalGabarit()
{
  bool bRes = false;
  MbItem * obj = nullptr;
  MbTopItem * item = nullptr;

  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &item ) ) {
    MbFace * face = (MbFace *)item;
    const MbSurface * surface = &face->GetSurface();
    MbPlacement3D place;

    if ( ::SelectLocalPlacementByPoints( place ) ) {
      MbCube cube;
      MbMatrix3D matrInto;
      MbMatrix3D matrFrom;

      place.GetMatrixFrom( matrFrom );
      place.GetMatrixInto( matrInto );

      surface->CalculateLocalGabarit( matrInto, cube );
      ::CreateLocalCubeContours( cube, matrFrom );

      bRes = true;
    }
  }

  return bRes;
}


//------------------------------------------------------------------------------
// Рассчитать габарит относительно л.с.к. трансформированием поверхностей
// Calculate the dimensions of the transformed surfaces relative to the placement
// ---
static void CalculateShellCubeRelativeToPlace( const MbFaceShell   & shell,      // Оболочка   shell
                                               const MbPlacement3D & place,      // Л.с.к.     placement
                                               MbCube        & cubeLocal ) // Габарит относительно л.с.к.   Dimensions relative to the placement
{
  MbPlacement3D placeLocal( place );
  cubeLocal.SetEmpty();

  // Создание регистраторов дублированных и трансфорированных объектов
  // Registries of the trancformed replica
  MbRegDuplicate * regDuplicate = nullptr;
  MbRegTransform * regTransform = nullptr;
  // NES 7.12.2011 Сняла экспортность с ф-ий CreateRegDuplicate, CreateRegTransform
  //MbAutoRegDuplicate autoRegDuplicate( regDuplicate );
  //MbAutoRegTransform autoRegTransform( regTransform );

  MbMatrix3D matrInto;
  place.GetMatrixInto( matrInto ); // Дать

  size_t i, cnt = shell.GetFacesCount();

  // Владеющий множество для копий поверхностей
  // Owner container of the replica
  PArray<MbSurface> surfArr( cnt, 1, true );

  // Создадим копии поверхностей оболочки, складывая их в множество
  // Create replica of the shell's surfaces
  for ( i = 0; i < cnt; i++ ) {
    const MbSurface * surf = shell.GetSurface( i );
    if ( surf != nullptr ) {
      MbSurface * surfCopy = &(MbSurface &)surf->Duplicate( regDuplicate );
      surfArr.Add( surfCopy );
    }
  }
  // Количество поверхностей, попавших в множество
  // Count of replica
  cnt = surfArr.Count();
  // Трансформируем по матрице копии поверхностей оболочки
  // Transform the replica using the matrix
  for ( i = 0; i < cnt; i++ ) {
    MbSurface * surfCopy = surfArr[i];
    surfCopy->Transform( matrInto, regTransform );
    // Для правильной работы необходимо скорректировать функции CalculateGabarit,
    // У объектов MbContourOnPlane, MbContourOnSurface, MbSurfaceCurve, MbPlaneCurve
    // Т.к. там в результате габарит будет рассчитан неправильно (больше, чем на самом деле)
    // А делать этого уже нельзя, испортятся построенные модели
    // Поэтому надо пользоваться новой функцией у объектов CalculateLocalGabarit

    // CalculateGabarit methods of the MbContourOnPlane, MbContourOnSurface, MbSurfaceCurve, 
    // MbPlaneCurve objects should be corrected to provide the right result.
    // That isn't done because of specific subtilties of some models and algorythms.
    // 
    // Right way is using the CalculateLocalGabarit method of these objects
  }
  // Посчитаем общий габарит поверхостнй
  // Sum the gabarit
  for ( i = 0; i < cnt; i++ ) {
    MbSurface * surfCopy = surfArr[i];
    surfCopy->AddYourGabaritTo( cubeLocal );
  }

  placeLocal.Transform( matrInto );
}


//------------------------------------------------------------------------------
// Габарит относительно локальной с.к. (проверить BUG_20639)
// ---
bool PutSolidLocalGabarit()
{
  bool bRes = false;
  MbItem * objSolid = nullptr;
  PathMatrix pathFrom;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &objSolid, pathFrom ) ) {

    MbItem * objFace = nullptr;
    MbItem * objSurf = nullptr;
    MbTopItem * topFace = nullptr;

    MbPlacement3D place;
    bool bProcess = false;

    if ( GetTObj(IDS_SELECT_PLANE_FACE, st_Solid, tt_Face, &objFace, &topFace) && objFace && topFace ) {
      const MbSurface * surf = &((MbFace*)topFace)->GetSurface().GetSurface();
      if ( surf->IsA() == st_Plane ) {
        const MbPlane * plane = (const MbPlane *)surf;
        place.Init( plane->GetPlacement() );
        bProcess = true;
      }
    }
    else
    if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &objSurf ) ) {
      const MbSurface * surf = &((MbSurface *)((MbSpaceInstance *)objSurf)->GetSpaceItem())->GetSurface();
      if ( surf->IsA() == st_Plane ) {
        const MbPlane * plane = (const MbPlane *)surf;
        place.Init( plane->GetPlacement() );
        bProcess = true;
      }
    }
    else if ( ::SelectLocalPlacementByPoints( place ) ) {
      bProcess = true;
    }

    if ( bProcess ) {
      TestVariables::viewManager->ToggleObject(objSolid);

      MbSolid * solid = (MbSolid *)objSolid;

      MbCube cube;
      MbMatrix3D matrInto;
      MbMatrix3D matrFrom;

      solid->CalculateGabarit( cube ); // Габарит относительно глобальной с.к.
      ::CreateLocalCubeContours( cube, matrFrom );

      place.GetMatrixInto( matrInto );
      place.GetMatrixFrom( matrFrom );
      
      solid->CalculateLocalGabarit( matrInto, cube ); // Габарит относительно локальной с.к.
      ::CreateLocalCubeContours( cube, matrFrom );
            
      ::CalculateShellCubeRelativeToPlace( *solid->GetShell(), place, cube );
      ::CreateLocalCubeContours( cube, matrFrom, TestVariables::BACKGROUND_Style );

      TestVariables::viewManager->UnselectObject( objSolid );

      SPtr<MbSolid> localCubeSolid ( nullptr );
      MbSNameMaker operNames;
      operNames.SetVersion( MbNameVersion() );
      MbResultType res = ::LocalCubePhantom( *solid, MbLocalCubePhantomParams (place, false, operNames), localCubeSolid );
      if ( res == rt_Success && localCubeSolid != nullptr ) {
        TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, localCubeSolid );
      }
      else {
        MessageBoxEx( RT_CUBE_PHANTOME_ERROR, IDS_TITLE_ERR );
      }

      bRes = true;
    }
  }

  return bRes;
}


//------------------------------------------------------------------------------
// Показать нормаль
// ---
inline
void PutNormal( const MbCartPoint3D & pnt, MbVector3D & norm, bool isSameSense,
                MbPointFrame * pFrame, MbWireFrame * wFrame )
{
  if ( !isSameSense )
    norm.Invert();

//#ifdef _DRAWGI
  if ( isSameSense ) {
    TestVariables::drawImp->DrawPoint( &pnt, TRGB_RED );
    TestVariables::drawImp->DrawLine( pnt, norm, TRGB_RED  );
  }
  else {
    TestVariables::drawImp->DrawPoint( &pnt, TRGB_BLUE );
    TestVariables::drawImp->DrawLine( pnt, norm, TRGB_BLUE );
  }
//#endif //_DRAWGI

  if ( pFrame != nullptr && wFrame != nullptr ) {
    pFrame->AddVertex( pnt );
    pFrame->SetColor( (isSameSense ? TestVariables::RED : TestVariables::BLUE) );

    MbLineSegment3D * ls = new MbLineSegment3D( pnt, pnt+norm );
    wFrame->AddEdge( *ls, true );
    wFrame->SetColor( (isSameSense ? TestVariables::RED : TestVariables::BLUE) );
    ::DeleteItem( ls );
  }
}


//------------------------------------------------------------------------------
// Нормали поверхности
// ---
bool PutSurfNormals( const MbSurface * surf, bool isSameSense )
{
  PathMatrix pathFrom;

  if ( surf == nullptr ) {
    MbItem * obj = nullptr;
    if ( ::GetGObj( IDS_SELECT_SURFACE, st_Surface, &obj, pathFrom ) )
      surf = ((MbSurface *)((MbSpaceInstance *)obj)->GetSpaceItem());
    isSameSense = true;
  }

  if ( surf != nullptr ) {
    double len = METRIC_DELTA * surf->GetGabDiagonal(); // Размер нормали

    const double sag_ = std_max( Math::deviateSag, Math::visualSag ); // Угловой шаг
    double uSag = sag_; 
    double vSag = sag_;
    
    double u, v;
    MbCartPoint3D pnt;
    MbVector3D norm;

    MbPointFrame * pFrame = new MbPointFrame;
    MbWireFrame * wFrame = new MbWireFrame;

    if ( surf->IsPlanar() ) {
      u = surf->GetUMid();
      v = surf->GetVMid();
      surf->PointOn( u, v, pnt );
      surf->Normal( u, v, norm );
      norm *= len;
      ::PutNormal( pnt, norm, isSameSense, pFrame, wFrame );
    }
    else {
      double uMin = surf->GetUMin();
      double uMax = surf->GetUMax();
      double vMin = surf->GetVMin();
      double vMax = surf->GetVMax();

      MbRect1D uRange( uMin, uMax );
      MbRect1D vRange( vMin, vMax );

      double pEps = Math::paramEpsilon;

      ptrdiff_t iChangeParams = 0;
      if ( GetInt( IDS_ENTER_USE_OR_NOT, iChangeParams ) && iChangeParams > 0 ) {
        u = uMin;
        if ( GetDouble( IDS_ENTER_PARAMETER, u, &uRange ) ) {
          if ( u > uMin + pEps && u < uMax - pEps )
            uMin = u;
        }
        u = uMax;
        if ( GetDouble( IDS_ENTER_PARAMETER, u, &uRange ) ) {
          if ( u > uMin + pEps && u < uMax - pEps )
            uMax = u;
        }
        v = vMin;
        if ( GetDouble( IDS_ENTER_PARAMETER, v, &vRange ) ) {
          if ( v > vMin + pEps && v < vMax - pEps )
            vMin = v;
        }
        v = vMax;
        if ( GetDouble( IDS_ENTER_PARAMETER, v, &vRange ) ) {
          if ( v > vMin + pEps && v < vMax - pEps )
            vMax = v;
        }

        MbRect1D uSagRange( PARAM_NEAR, uSag );
        MbRect1D vSagRange( PARAM_NEAR, vSag );

        double sag = Math::deviateSag;
        if ( GetDouble( IDS_ENTER_PARAMETER, sag, &uSagRange ) ) {
          if ( sag >= PARAM_NEAR && sag <= uSag )
            uSag = sag;
        }
        sag = Math::deviateSag;
        if ( GetDouble( IDS_ENTER_PARAMETER, sag, &vSagRange ) ) {
          if ( sag >= PARAM_NEAR && sag <= vSag )
            vSag = sag;
        }
      }

      SArray<double> uArr( 0, 1 );
      SArray<double> vArr( 0, 1 );

      if ( uSag != vSag ) {
        SArray<double> uTmpArr( 0, 1 );
        SArray<double> vTmpArr( 0, 1 );
        surf->GetTesselation( MbStepData(ist_DeviationStep, uSag), uMin, uMax, vMin, vMax, uTmpArr, vTmpArr );
        uArr += uTmpArr;

        uTmpArr.Flush();
        vTmpArr.Flush();
        surf->GetTesselation( MbStepData(ist_DeviationStep, vSag), uMin, uMax, vMin, vMax, uTmpArr, vTmpArr );
        vArr += vTmpArr;
      }
      else {
        surf->GetTesselation( MbStepData(ist_DeviationStep, uSag), uMin, uMax, vMin, vMax, uArr, vArr );
      }

      if ( uArr.Count() == 2 ) {
        double uMid = (uArr[0] + uArr[1])*0.5;
        uArr.InsertInd( 1, uMid );
      }
      if ( vArr.Count() == 2 ) {
        double vMid = (vArr[0] + vArr[1])*0.5;
        vArr.InsertInd( 1, vMid );
      }

      for ( size_t i = 0, icnt = uArr.Count(); i < icnt; i++ ) {
        u = uArr[i];
        for ( size_t j = 0, jcnt = vArr.Count(); j < jcnt; j++ ) {
          v = vArr[j];

          surf->PointOn( u, v, pnt );

          /*
          MbVector3D v1, v2;
          surf->TangentU( u, v, v1 );
          surf->TangentV( u, v, v2 );
          v1 *= len;
          v2 *= len;
          ::PutNormal( pnt, v1, isSameSense, pFrame, wFrame );
          ::PutNormal( pnt, v2, isSameSense, pFrame, wFrame );
          // */
          
          surf->Normal( u, v, norm );
          norm *= len;
          ::PutNormal( pnt, norm, isSameSense, pFrame, wFrame );
        }
      }
    }   

    if ( pFrame != nullptr &&  wFrame != nullptr ) {
      Style style;
      style.SetColor( wFrame->GetColor() );
      TestVariables::viewManager->AddObject( style, pFrame );
      TestVariables::viewManager->AddObject( style, wFrame );
      return true;
    }
    else {
      ::DeleteItem( pFrame );
      ::DeleteItem( wFrame );
    }    
  }

  return false;
}


//------------------------------------------------------------------------------
// Нормали грани
// ---
bool PutFaceNormals( const MbFace * face )
{
  if ( face == nullptr ) {
    MbItem * obj = nullptr;
    MbTopItem * item = nullptr;
    if ( ::GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &item ) && item->IsAFace() )
      face = (const MbFace *)item;
  }

  if ( face != nullptr ) {
    const MbSurface * surf = &face->GetSurface();
    bool bSense = face->IsSameSense();
    return ::PutSurfNormals( surf, bSense );
  }

  return false;
}


//------------------------------------------------------------------------------
// Нормали сетки
// ---
bool PutMeshNormals( const MbMesh * mesh )
{
  PathMatrix pathFrom;
  MbPointFrame * pFrame = new MbPointFrame;
  MbWireFrame * wFrame = new MbWireFrame;

  if ( mesh == nullptr ) {
    MbItem * obj = nullptr;
    if ( ::GetGObj( IDS_SELECT_MESH, st_Mesh, &obj, pathFrom ) )
      mesh = ( MbMesh * )obj;
  }

  if ( mesh != nullptr )
  {
    size_t gCount = mesh->GridsCount();
    for ( size_t i = 0; i < gCount; i++ ) {
      const MbGrid * grid = mesh->GetGrid( i );
      if ( grid != nullptr ) {
        double len = DELTA_MIN * DELTA_MIN * grid->GetCube().GetDiagonal(); // Размер нормали
        size_t tCount = grid->TrianglesCount();
        for ( size_t triangleInd = 0; triangleInd < tCount; triangleInd++ )
        {
          MbCartPoint3D pnt;
          MbVector3D n;
          grid->GetPoint( triangleInd * 3, pnt );
          grid->GetNormal( triangleInd * 3, n );
          n *= len;
          ::PutNormal( pnt, n, true, pFrame, wFrame );
        }
      }
    }

    if ( pFrame != nullptr && wFrame != nullptr ) {
      Style style;
      style.SetColor( wFrame->GetColor() );
      TestVariables::viewManager->AddObject( style, pFrame );
      TestVariables::viewManager->AddObject( style, wFrame );
      return true;
    }
    else {
      ::DeleteItem( pFrame );
      ::DeleteItem( wFrame );
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Создать операционную разбивку поверхности
// ---
bool CreateSurfaceOperTesselation()
{
  MbItem * item = nullptr;
  PathMatrix pathFrom;

  if ( ::GetGObj( IDS_SELECT_SURFACE, st_Surface, &item, pathFrom ) ) {
    const MbSpaceItem * sItem = ((MbSpaceInstance *)item)->GetSpaceItem(); // Дать трёхмерный геометрический объекта
    if ( sItem != nullptr && (sItem->Family() == st_Surface) ) {
      const MbSurface * surf = &((MbSurface *)sItem)->GetSurface();
      bool sExt = false;

      size_t cntu = surf->GetUCount();
      size_t cntv = surf->GetVCount();

      double uMin = surf->GetUMin();
      double uMax = surf->GetUMax();
      double vMin = surf->GetVMin();
      double vMax = surf->GetVMax();

      const double uRng = uMax - uMin;
      const double vRng = vMax - vMin;

      const double du = uRng / (double)cntu;
      const double dv = vRng / (double)cntv;

      const bool uCls = surf->IsUClosed();
      const bool vCls = surf->IsVClosed();

      if ( !uCls )
        cntu++;
      if ( !vCls )
        cntv++;

      if ( cntu > 0 && cntv > 0 ) {
        //double cellDmin = MB_MAXDOUBLE;
        SArray<MbCartPoint3D> firstPnts( cntu, 1 ); 
        SArray<MbCartPoint3D> * prevPntsPtr = new SArray<MbCartPoint3D>( cntu, 1 );
        SArray<MbCartPoint3D> * thisPntsPtr = new SArray<MbCartPoint3D>( cntu, 1 );
        MbCartPoint3D pOn;
        pOn.x = UNDEFINED_DBL;
        firstPnts.Fill( cntu, pOn );
        prevPntsPtr->Fill( cntu, pOn );
        thisPntsPtr->Fill( cntu, pOn );
        MbCartPoint3D p11, p12, p21, p22;

        size_t icnt = vCls ? cntv+1 : cntv;
        size_t jcnt = uCls ? cntu+1 : cntu;

        double v0 = vMin;
        for ( size_t i = 0; i < icnt; i++ ) {
          double u0 = uMin;
          for ( size_t j = 0; j < jcnt; j++ ) {
            SArray<MbCartPoint3D> & prevPnts = *prevPntsPtr;
            SArray<MbCartPoint3D> & thisPnts = *thisPntsPtr;

            if ( i < cntv && j < cntu ) {
              if ( sExt ) surf->_PointOn( u0, v0, pOn );
              else        surf-> PointOn( u0, v0, pOn );
              thisPnts[j % cntu] = pOn;
            }

            if ( i > 0 && j > 0 ) {
              ptrdiff_t jPrev = j - 1;
              ptrdiff_t jThis = j % cntu;

              p11 = prevPnts[jPrev];
              p12 = prevPnts[jThis];

              if ( i < cntv ) {
                p21 = thisPnts[jPrev];
                p22 = thisPnts[jThis];
              }
              else {
                p21 = firstPnts[jPrev];
                p22 = firstPnts[jThis];
              }

              MbLineSegment3D * ls1 = new MbLineSegment3D( p11, p12 );
              MbLineSegment3D * ls2 = new MbLineSegment3D( p21, p22 );
              MbRuledSurface * cellSurf = new MbRuledSurface( 0.0, 1.0, *ls1, *ls2 );

              TestVariables::viewManager->AddObject( TestVariables::SURFACE_Style, cellSurf, false );

              ::DeleteItem( ls1 );
              ::DeleteItem( ls2 );
            }

            u0 += du;
          }

          if ( i < 1 )
            firstPnts = *thisPntsPtr;

          SArray<MbCartPoint3D> * tmpPntsPtr = prevPntsPtr;
          prevPntsPtr = thisPntsPtr;
          thisPntsPtr = tmpPntsPtr;

          v0 += dv;
        }

        if ( prevPntsPtr != nullptr ) {
          delete prevPntsPtr;
          prevPntsPtr = nullptr;
        }

        if ( thisPntsPtr != nullptr ) {
          delete thisPntsPtr;
          thisPntsPtr = nullptr;
        }

        return true;
      }
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Проверить скорость работы поверхности
// ---
bool CheckSurfaceSpeed()
{
  MbItem * obj = nullptr;
  MbTopItem * item = nullptr;
  MbSurface * surf = nullptr;
  PathMatrix pathFrom;

  if ( ::GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &item ) && item->IsAFace() ) {
    surf = &((MbFace *)item)->SetSurface();
  }
  if ( surf == nullptr ) {
    if ( ::GetGObj( IDS_SELECT_SURFACE, st_Surface, &obj, pathFrom ) )
      surf = &((MbSurface *)((MbSpaceInstance *)obj)->GetSpaceItem())->SetSurface();
  }

  if ( surf != nullptr ) {
    if ( surf->IsA() == st_CurveBoundedSurface )
      surf = &surf->SetBasisSurface();

    bool checkPointAndDerivatives = false;

    {
      int iRes = 0;
      ::GetInt( IDS_ENTER_CALC_POINTS_DERS, iRes );
      if ( iRes != 0 )
        checkPointAndDerivatives = true;
    }

    bool checkGabarit = true;
    bool checkTesselation = true;
    bool checkDirectPointProjection = checkGabarit;
    MbCube surfCube;

    //int threadPriority = 0;
    uint usedTime = 0;

    TCHAR buf[1024];
    size_t len = 0;
    len += (size_t)_sntprintf( buf+len, 1024, RT_TEST_RESULTS );

    if ( checkPointAndDerivatives ) {
      double urng = surf->GetUMax() - surf->GetUMin();
      double vrng = surf->GetVMax() - surf->GetVMin();
      double umid = surf->GetUMid();
      double vmid = surf->GetVMid();
      double umov = 0.25 * urng;
      double vmov = 0.25 * vrng;

      SArray<MbCartPoint> uvArr( 5, 1 );
      uvArr.Add()->Init( umid,      vmid );
      uvArr.Add()->Init( umid-umov, vmid-vmov );
      uvArr.Add()->Init( umid+umov, vmid-vmov );
      uvArr.Add()->Init( umid+umov, vmid+vmov );
      uvArr.Add()->Init( umid-umov, vmid+vmov );

      size_t cntLoops = 200000;
      size_t cntPnts = uvArr.Count();
      size_t cntChecks = 0;

      MbCartPoint3D pnt;
      MbVector3D derU, derV;

      //threadPriority = ::GetThreadPriority( ::GetCurrentThread() );
      //::SetThreadPriority( ::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL );
      ThreadPriority( false /*normalPriority*/ );

      usedTime = ::GetTickCountEx();

      for ( size_t i = 0; i < cntLoops; i++ ) {
        for ( size_t k = 0; k < cntPnts; k++ ) {
          cntChecks++;
          const MbCartPoint & uv = uvArr[k];
          surf->_PointOn( uv.x, uv.y, pnt );
          surf->_DeriveU( uv.x, uv.y, derU );
          surf->_DeriveV( uv.x, uv.y, derV );
        }
      }

      usedTime = ::GetTickCountEx() - usedTime;

      //::SetThreadPriority( ::GetCurrentThread(), threadPriority );
      ThreadPriority( true /*normalPriority*/ );

      cntChecks /= 1000; // В тысячах
      double speed = ((double)usedTime) / (double)cntChecks;

      len += (size_t)_sntprintf( buf+len, 1024-len, RT_CALCULATE_POINT_AND_DERIVE );
      len += (size_t)_sntprintf( buf+len, 1024-len, RT_REQUESTS_COUNT, LoUint32(cntChecks) );
      len += (size_t)_sntprintf( buf+len, 1024-len, RT_TIME_MSEC, usedTime );
      len += (size_t)_sntprintf( buf+len, 1024-len, RT_AVERAGE_SPEED_MSEC, speed );
    }

    if ( checkGabarit ) {
      //threadPriority = ::GetThreadPriority( ::GetCurrentThread() );
      //::SetThreadPriority( ::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL );
      ThreadPriority( false /*normalPriority*/ );

      usedTime = ::GetTickCountEx();

      surf->SetDirtyGabarit();
      surf->AddYourGabaritTo( surfCube );

      usedTime = ::GetTickCountEx() - usedTime;

      len += (size_t)_sntprintf( buf+len, 1024-len, _T("\n") );
      len += (size_t)_sntprintf( buf+len, 1024-len, RT_CALCULATE_SURFACE_GAB );
      len += (size_t)_sntprintf( buf+len, 1024-len, RT_TIME_MSEC, usedTime );

      //::SetThreadPriority( ::GetCurrentThread(), threadPriority );
      ThreadPriority( true /*normalPriority*/ );
    }

    if ( checkTesselation ) {
      //threadPriority = ::GetThreadPriority( ::GetCurrentThread() );
      //::SetThreadPriority( ::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL );
      ThreadPriority( false /*normalPriority*/ );

      usedTime = ::GetTickCountEx();

      double u1 = surf->GetUMin();
      double u2 = surf->GetUMax();
      double v1 = surf->GetVMin();
      double v2 = surf->GetVMax();
      SArray<double> uArr( 0, 1 );
      SArray<double> vArr( 0, 1 );

      surf->SetDirtyGabarit();
      surf->GetTesselation( MbStepData(ist_SpaceStep, Math::visualSag), u1, u2, v1, v2, uArr, vArr );

      uArr.Flush();
      vArr.Flush();

      surf->SetDirtyGabarit();
      surf->GetTesselation( MbStepData(ist_DeviationStep, Math::deviateSag), u1, u2, v1, v2, uArr, vArr );
      
      usedTime = ::GetTickCountEx() - usedTime;

      len += (size_t)_sntprintf( buf+len, 1024-len, _T("\n") );
      len += (size_t)_sntprintf( buf+len, 1024-len, RT_CALCULATE_SURFACE_TESSELATION );
      len += (size_t)_sntprintf( buf+len, 1024-len, RT_TIME_MSEC, usedTime );

      //::SetThreadPriority( ::GetCurrentThread(), threadPriority );
      ThreadPriority( true /*normalPriority*/ );
    }

    if ( checkDirectPointProjection ) {
      size_t k = 0;
      MbCartPoint3D pnt0, pnt;
      surfCube.GetCentre( pnt0 );

      const size_t cubeFacesCnt = 6;
      const size_t cubeEdgesCnt = 12;
      const size_t cubeCheckCnt = cubeFacesCnt + cubeEdgesCnt;
      SArray<MbCartPoint3D> pnts( cubeCheckCnt, 1 );
      SArray<MbVector3D>    dirs( cubeCheckCnt, 1 );

      for ( k = 0; k < cubeFacesCnt; k++ ) {
        surfCube.GetFaceCentre( k, pnt );
        pnts.Add( pnt );
        dirs.Add()->Init( pnt0, pnt );
      }
      for ( k = 0; k < cubeEdgesCnt; k++ ) {
        surfCube.GetEdgeCentre( k, pnt );
        pnts.Add( pnt );
        dirs.Add()->Init( pnt0, pnt );
      }

      SArray<MbCartPoint> uvPnts( cubeCheckCnt, 1 );

      {
        //threadPriority = ::GetThreadPriority( ::GetCurrentThread() );
        //::SetThreadPriority( ::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL );
        ThreadPriority( false /*normalPriority*/ );

        usedTime = ::GetTickCountEx();

        surf->SetDirtyGabarit();
        for ( k = 0; k < cubeCheckCnt; k++ )
          surf->DirectPointProjection( pnts[k], dirs[k], uvPnts, false );

        usedTime = ::GetTickCountEx() - usedTime;

        //::SetThreadPriority( ::GetCurrentThread(), threadPriority );
        ThreadPriority( true /*normalPriority*/ );

        len += (size_t)_sntprintf( buf+len, 1024-len, _T("\n") );
        len += (size_t)_sntprintf( buf+len, 1024-len, RT_PROJECTING_BOUNDING_POINTS_IN_DIRECTION );
        len += (size_t)_sntprintf( buf+len, 1024-len, RT_TIME_MSEC, usedTime );
      }

      {
        //threadPriority = ::GetThreadPriority( ::GetCurrentThread() );
        //::SetThreadPriority( ::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL );
        ThreadPriority( false /*normalPriority*/ );

        usedTime = ::GetTickCountEx();

        surf->SetDirtyGabarit();
        double u, v;
        for ( k = 0; k < cubeCheckCnt; k++ )
          surf->NearPointProjection( pnts[k], u, v, false );

        usedTime = ::GetTickCountEx() - usedTime;

        //::SetThreadPriority( ::GetCurrentThread(), threadPriority );
        ThreadPriority( true /*normalPriority*/ );

        len += (size_t)_sntprintf( buf+len, 1024-len, _T("\n") );
        len += (size_t)_sntprintf( buf+len, 1024-len, RT_PROJECTING_BOUNDING_POINTS );
        len += (size_t)_sntprintf( buf+len, 1024-len, RT_TIME_MSEC, usedTime );
      }
    }

    MessageBoxEx( buf, RT_SURFACE_SPEED );

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Проверить скорость работы пространственной кривой
// ---
bool CheckCurve3DSpeed()
{
  bool bRes = false;

  MbItem * obj = nullptr;
  MbTopItem * item = nullptr;
  const MbCurve3D * curve = nullptr;
  PathMatrix pathFrom;

  if ( ::GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &item ) && item->IsAnEdge() ) {
    const MbCurveEdge * cEdge = static_cast<const MbCurveEdge * >(item);
    if ( cEdge != nullptr )
      curve = &cEdge->GetCurve();
  }
  if ( curve == nullptr ) {
    if ( ::GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj, pathFrom ) && (obj->IsA() == st_SpaceInstance) ) {
      const MbSpaceItem * spaceItem =  static_cast<const MbSpaceInstance *>(obj)->GetSpaceItem();
      if ( spaceItem ) {
        if ( spaceItem->Family() == st_Curve3D ) {
          curve = static_cast<const MbCurve3D *>( spaceItem );
        }
      }
    }
  }

  if ( curve != nullptr ) {
    double tmin = curve->GetTMin();
    double tmax = curve->GetTMax();
    double trng = curve->GetTRange();
    double tmid = curve->GetTMid();
    double tmov = 0.25 * trng;

    SArray<double> tArr( 5, 1 );
    tArr.Add( tmin );
    tArr.Add( (tmid - tmov) );
    tArr.Add( tmid );
    tArr.Add( (tmid + tmov) );
    tArr.Add( tmax );

    size_t cntLoops = 200000;
    size_t cntPnts = tArr.Count();
    size_t cntChecks = 0;

    MbCartPoint3D pnt;
    MbVector3D der;

    //int threadPriority = ::GetThreadPriority( ::GetCurrentThread() );
    //::SetThreadPriority( ::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL );
    ThreadPriority( false /*normalPriority*/ );

    uint usedTime = ::GetTickCountEx();

    for ( size_t i = 0; i < cntLoops; i++ ) {
      for ( size_t k = 0; k < cntPnts; k++ ) {
        cntChecks++;
        const double & t = tArr[k];
        curve->_PointOn( t, pnt );
        curve->_FirstDer( t, der );
      }
    }

    usedTime = ::GetTickCountEx() - usedTime;

    //::SetThreadPriority( ::GetCurrentThread(), threadPriority );
    ThreadPriority( true /*normalPriority*/ );

    cntChecks /= 1000; // В тысячах
    double speed = ((double)usedTime) / (double)cntChecks;

    TCHAR buf[1024];
    size_t len = 0;

    len += (size_t)_sntprintf( buf+len, 1024-len, RT_TEST_RESULTS );
    len += (size_t)_sntprintf( buf+len, 1024-len, RT_CALCULATE_POINT_AND_DERIVE );
    len += (size_t)_sntprintf( buf+len, 1024-len, RT_REQUESTS_COUNT, LoUint32(cntChecks) );
    len += (size_t)_sntprintf( buf+len, 1024-len, RT_TIME_MSEC, usedTime );
    len += (size_t)_sntprintf( buf+len, 1024-len, RT_AVERAGE_SPEED_MSEC, speed );

    MessageBoxEx( buf,RT_CURVE3D_SPEED );

    bRes = true;
  }

  return bRes;
}


//------------------------------------------------------------------------------
// Проверить скорость работы двумерной кривой
// ---
bool CheckCurve2DSpeed()
{
  bool bRes = false;

  MbItem * obj = nullptr;

  if ( GetMObj( IDS_SELECT_CRV_ON_PLANE, pt_Curve, &obj ) ) 
  {
    TestVariables::viewManager->ToggleObject( obj );

    if ( const MbCurve * curve = static_cast<const MbCurve *>( static_cast<const MbPlaneInstance *>(obj)->GetPlaneItem() ) )
    {
      //int threadPriority = ::GetThreadPriority( ::GetCurrentThread() );
      //int iThreadPriority = ::SetThreadPriority( ::GetCurrentThread(), THREAD_BASE_PRIORITY_MAX );
      //if ( iThreadPriority == 0 )
      //  MessageBoxEx( _T("Не получилось выставить приоритет потока!"), _T("Внимание") );
      ThreadPriority( false /*normalPriority*/ );

      MbRect rect;
      curve->AddYourGabaritTo( rect );
      double diag = rect.GetDiagonal();

      size_t pointsCnt;
      uint usedTime; 
      const size_t cnt_ = 100;
      double usedSag;
      MbPolygon polygon;

      SArray<uint>   usedTimes( 0, 1 );
      SArray<double> usedSags ( 0, 1 );
      SArray<size_t>   usedCnts ( 0, 1 );
     
      // Очередная стрелка прогиба
      usedTime = ::GetTickCountEx();
      usedSag = METRIC_NEAR;
      usedSag = std_max( usedSag, METRIC_PRECISION );
      for ( size_t k = 0; k < cnt_; k++ ) {
        curve->CalculatePolygon( usedSag, polygon );
        pointsCnt = polygon.Count();
        polygon.SimpleFlushPoints();
      }
      usedTime = ::GetTickCountEx() - usedTime;
      usedTimes.Add( usedTime );
      usedSags.Add( usedSag );
      usedCnts.Add( pointsCnt );

      // Очередная стрелка прогиба
      usedTime = ::GetTickCountEx();
      usedSag = diag;
      usedSag = std_max( usedSag, METRIC_PRECISION );
      for ( size_t k = 0; k < cnt_; k++ ) {
        curve->CalculatePolygon( usedSag, polygon );
        pointsCnt = polygon.Count();
        polygon.SimpleFlushPoints();
      }
      usedTime = ::GetTickCountEx() - usedTime;
      usedTimes.Add( usedTime );
      usedSags.Add( usedSag );
      usedCnts.Add( pointsCnt );

      // Очередная стрелка прогиба
      usedTime = ::GetTickCountEx();
      usedSag = DELTA_MID * diag;
      usedSag = std_max( usedSag, METRIC_PRECISION );
      for ( size_t k = 0; k < cnt_; k++ ) {
        curve->CalculatePolygon( usedSag, polygon );
        pointsCnt = polygon.Count();
        polygon.SimpleFlushPoints();
      }
      usedTime = ::GetTickCountEx() - usedTime;
      usedTimes.Add( usedTime );
      usedSags.Add( usedSag );
      usedCnts.Add( pointsCnt );

      // Очередная стрелка прогиба
      usedTime = ::GetTickCountEx();
      usedSag = DELTA_MIN * diag;
      usedSag = std_max( usedSag, METRIC_PRECISION );
      for ( size_t k = 0; k < cnt_; k++ ) {
        curve->CalculatePolygon( usedSag, polygon );
        pointsCnt = polygon.Count();
        polygon.SimpleFlushPoints();
      }
      usedTime = ::GetTickCountEx() - usedTime;
      usedTimes.Add( usedTime );
      usedSags.Add( usedSag );
      usedCnts.Add( pointsCnt );

      // Очередная стрелка прогиба
      usedTime = ::GetTickCountEx();
      usedSag = METRIC_NEAR * diag;
      usedSag = std_max( usedSag, METRIC_PRECISION );
      for ( size_t k = 0; k < cnt_; k++ ) {
        curve->CalculatePolygon( usedSag, polygon );
        pointsCnt = polygon.Count();
        polygon.SimpleFlushPoints();
      }
      usedTime = ::GetTickCountEx() - usedTime;
      usedTimes.Add( usedTime );
      usedSags.Add( usedSag );
      usedCnts.Add( pointsCnt );

      // Очередная стрелка прогиба
      usedTime = ::GetTickCountEx();
      usedSag = METRIC_ACCURACY * diag;
      usedSag = std_max( usedSag, METRIC_PRECISION );
      for ( size_t k = 0; k < cnt_; k++ ) {
        curve->CalculatePolygon( usedSag, polygon );
        pointsCnt = polygon.Count();
        polygon.SimpleFlushPoints();
      }
      usedTime = ::GetTickCountEx() - usedTime;
      usedTimes.Add( usedTime );
      usedSags.Add( usedSag );
      usedCnts.Add( pointsCnt );

      //::SetThreadPriority( ::GetCurrentThread(), threadPriority );
      ThreadPriority( true /*normalPriority*/ );

      const size_t usedTimesCnt = usedTimes.Count();

      const size_t bufSize = (512 + 256 * usedTimesCnt);
      TCHAR * buf = new TCHAR[bufSize];
      size_t len = 0;
      len += (size_t)_sntprintf( buf+len, bufSize-len, RT_TEST_RESULTS);
      len += (size_t)_sntprintf( buf+len, bufSize-len, RT_CALCULATE_POLYGON );
      len += (size_t)_sntprintf( buf+len, bufSize-len, RT_GAB_DIAGONAL, diag );
      len += (size_t)_sntprintf( buf+len, bufSize-len, RT_REQUESTS_COUNT, LoUint32(cnt_) );

      for ( size_t k = 0; k < usedTimesCnt; k++ ) {
        len += (size_t)_sntprintf( buf+len, bufSize-len, RT_SIZE_DEFLECTION, usedSags[k] );
        len += (size_t)_sntprintf( buf+len, bufSize-len, RT_TIME_MSEC, usedTimes[k] );
        len += (size_t)_sntprintf( buf+len, bufSize-len, RT_POLYGON_POINTS_COUNT, LoUint32(usedCnts[k]) );
      }

      MessageBoxEx( buf, RT_CURVE2D_SPEED );

      delete [] buf;
      buf = nullptr;

      bool createPolyLine = false;

      if ( createPolyLine ) { // Создание ломаной для сравнения
        const size_t cnt = (size_t)POWER_4;
        double tmin = curve->GetTMin();
        double tmax = curve->GetTMax();
        double dt = (tmax - tmin) / (double)cnt;
        double t = tmin;
        MbCartPoint pnt;
        SArray<MbCartPoint> pnts( cnt, 1 );
        for ( size_t k = 1; k <= cnt; k++ ) {
          curve->PointOn( t, pnt );
          pnts.Add( pnt );
          t += dt;
        }
        MbCurve * newCurve = new MbPolyline( pnts, curve->IsClosed() );
        TestVariables::viewManager->AddObject( TestVariables::POLYLINE_Style, newCurve );
      }
    }

    TestVariables::viewManager->ToggleObject( obj );
  }

  return bRes;
}


//------------------------------------------------------------------------------
// Проверить двумерную кривую на самопересечение
// ---
bool CheckCurve2DSelfInt()
{
  bool res = false;

  MbItem * obj = nullptr;

  if ( GetMObj( IDS_SELECT_CURVE, pt_Curve, &obj ) ) {
    TestVariables::viewManager->ToggleObject( obj );

    MbeSpaceType objIs = obj->IsA();

    const MbPlaneInstance * pInst = nullptr;
    const MbSpaceInstance * sInst = nullptr;

    if ( objIs == st_PlaneInstance )
      pInst = static_cast<const MbPlaneInstance *>( obj );
    else if ( objIs == st_SpaceInstance )
      sInst = static_cast<const MbSpaceInstance *>( obj );

    c3d::ConstPlaneCurveSPtr pCurve;
    MbPlacement3D place;

    if ( pInst != nullptr ) {
      const MbPlaneItem * pItem = pInst->GetPlaneItem();
      if ( pItem->Family() == pt_Curve )
        pCurve = static_cast<const MbCurve *>( pItem );
      else if ( pItem->Family() == pt_Region ) {
        const MbRegion & region = static_cast<const MbRegion &>( *pItem );
        if ( region.GetContoursCount() > 0 ) {
          pCurve = region.GetContour(0);
        }
      }
      place.Init( pInst->GetPlacement() );
    }
    else if ( sInst != nullptr ) {
      const MbSpaceItem * spaceItem = sInst->GetSpaceItem();
      if ( spaceItem != nullptr ) {
        if ( spaceItem->Family() == st_Curve3D ) {
          const MbCurve3D * spaceCurve = static_cast<const MbCurve3D *>(spaceItem);
          if ( !spaceCurve->GetPlaneCurve( pCurve, place, true ) ) {
            ConstSurfaceSPtr surface;
            spaceCurve->GetSurfaceCurve( pCurve, surface );
          }
        }
      }
    }

    if ( pCurve != nullptr ) {
      bool isSelfInt = false;

      double xyEps = METRIC_PRECISION;
      SArray<double> tCrossArr;
      SArray<double> tCrossArr1;
      ::CurveSelfIntersect( *pCurve, xyEps, xyEps, tCrossArr, tCrossArr1 );
      size_t count = tCrossArr.size();

      /* KOMPAS-35615
      SArray<MbCrossPoint> crossPoints;
      pCurve->SelfIntersect( crossPoints, xyEps );
      C3D_ASSERT( count < crossPoints.size() );
      // */

      if ( count < 1 ) {
        if ( pCurve->Type() == pt_Contour ) {
          const MbContour * pContour = &static_cast<const MbContour &>(*pCurve);
          if ( pContour->IsSelfIntersect( xyEps, true ) ) {
            C3D_ASSERT_UNCONDITIONAL( false );
            isSelfInt = true;
          }
        }
        else { // KOMPAS-42206
          SArray<MbCrossPoint> crossPoints;
          pCurve->SelfIntersect( crossPoints, xyEps );
          if ( !crossPoints.empty() ) {
            C3D_ASSERT_UNCONDITIONAL( false );
            for ( size_t i = 0; i < crossPoints.size(); ++i ) {
              tCrossArr.push_back( crossPoints[i].on1.t );
              tCrossArr1.push_back( crossPoints[i].on2.t );
            }
            count = tCrossArr.size();
            isSelfInt = true;
          }

          PlaneContourSPtr pContour( new MbContour( *pCurve, false ) );

          if ( pContour->IsSelfIntersect( xyEps, true ) ) {
            C3D_ASSERT_UNCONDITIONAL( false );
            isSelfInt = true;
          }
        }
      }

      if ( isSelfInt || ((count > 0) && (count == tCrossArr1.size())) ) {  
        res = true;
        for( size_t i = 0; i < count; ++i ) {
          MbCartPoint point = pCurve->PointOn( tCrossArr[i] );
          TestVariables::viewManager->AddObject( TestVariables::PPOINT_Style, /*new MbPoint( */point , &place );
          MbCartPoint point1 = pCurve->PointOn( tCrossArr1[i] );
          TestVariables::viewManager->AddObject( TestVariables::PPOINT_Style, /*new MbPoint( */point1 , &place );
        }  
        ::MessageBoxEx( RT_IS_SELFINTERSECTION, RT_CURVE_SELFINTERSECTION );
      } 
      else {  
        ::MessageBoxEx( RT_NO_SELFINTERSECTION, RT_CURVE_SELFINTERSECTION );
      }  
    }

    TestVariables::viewManager->ToggleObject( obj );
  }

  return res;
}



//------------------------------------------------------------------------------
// Проверить трехмерную кривую на самопересечение
// ---
bool CheckCurve3DSelfInt()
{
  bool res = false;

  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) ) {
    TestVariables::viewManager->ToggleObject( obj );

    if ( obj->IsA() == st_SpaceInstance ) {
      const MbSpaceInstance * sInst = static_cast<const MbSpaceInstance *>( obj );
      if ( sInst != nullptr ) {
        const MbSpaceItem * spaceItem = sInst->GetSpaceItem();
        if ( spaceItem != nullptr ) {
          if ( spaceItem->Family() == st_Curve3D ) {
            const MbCurve3D * spaceCurve = static_cast<const MbCurve3D *>( spaceItem );
            bool isSelfInt = ::IsSelfIntersect( *spaceCurve );
            if ( isSelfInt )
              ::MessageBoxEx( RT_IS_SELFINTERSECTION, RT_CURVE_SELFINTERSECTION );
            else
              ::MessageBoxEx( RT_NO_SELFINTERSECTION, RT_CURVE_SELFINTERSECTION );
          }
        }
      }
    }

    TestVariables::viewManager->ToggleObject( obj );
  }

  return res;
}


//------------------------------------------------------------------------------
// Проверить трехмерную кривую на самопересечение.
// ---
bool CheckSurfaceSelfInt()
{
  bool isSelfInt = false;
  SurfaceSPtr surf( SelectSurface() );
  if ( surf != nullptr ) {
    std::unique_ptr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyCommon ) );
    int mode = 0;
    MbAccurateTimer ac;
    ::GetInt( IDS_ENTER_SUFRACE_SELFINT_MODE, mode );
    ac.Begin();
    switch ( mode ) {
      case 1: { // Проверка на самопересечение поверхности путем сегментированного пересечения. Checking for self - intersection of surface by segmented intersection.
        MbSurfaceSelfIntParams params( surf->GetUMin(), surf->GetUMax(), surf->GetVMin(), surf->GetVMax(), progBar.get() );
        isSelfInt = ::IsSelfIntersectSurface( *surf, params );
      } break;
      case 2: { // Проверка на самопересечение поверхности вдоль вдоль uv линий. Checking for self - intersection of surface along uv lines.
        DoubleVector uPar{ surf->GetUMin(), surf->GetUMax() };
        DoubleVector vPar{ surf->GetVMin(), surf->GetVMax() };
        MbSurfaceSelfIntParams params( uPar, vPar, false, nullptr, Math::DefaultMathVersion(), progBar.get() );
        isSelfInt = ::IsSelfIntersectSurface( *surf, params );
      } break;
      case 0:
      default : { // Автоматический режим. Automatic mode.
        MbSurfaceSelfIntParams params( Math::DefaultMathVersion(), progBar.get() );
        isSelfInt = ::IsSelfIntersectSurface( *surf, params );
      } break;
    }
    ac.End();

    if ( TestVariables::checkOperationTime ) {
      const size_t bufSize = 256;
      TCHAR buf[bufSize];
      size_t len = 0;
      len += (size_t)_sntprintf( buf + len, bufSize - len, RT_TEST_RESULTS );
      len += (size_t)_sntprintf( buf + len, bufSize - len, isSelfInt ? RT_IS_SELFINTERSECTION : RT_NO_SELFINTERSECTION );
      len += (size_t)_sntprintf( buf + len, bufSize - len, _T( "\n\n" ) );
      len += (size_t)_sntprintf( buf + len, bufSize - len, RT_TIME_SEC1, ac.GetLast() );
      ::MessageBoxEx( buf, RT_SURFACE_SELFINTERSECTION );
    }
    else {
      ::MessageBoxEx( isSelfInt ? RT_IS_SELFINTERSECTION : RT_NO_SELFINTERSECTION, RT_SURFACE_SELFINTERSECTION );
    }
  }

  return isSelfInt;
}


//------------------------------------------------------------------------------
// Проверить оболочку на самопересечение.
// ---
bool CheckShellSelfInt()
{
  bool isSelfInt = false;
  SolidSPtr solid( SelectSolid() );
  if ( !solid.is_null() ) {
    const auto * shell = solid->GetShell();
    if ( shell != nullptr ) {
      MbAccurateTimer ac;
      std::unique_ptr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyCommon ) );
      MbShellSelfIntResults results;

      if ( GetBool( IDS_SHELL_BOOL_USE_MESH, false ) ) {
        int mode = TestVariables::operateType;
        MbStepData stepData;
        ::InitStepData( stepData, false, mode );
        MbShellSelfIntParams params( stepData );
        ac.Begin();
        isSelfInt = ::IsSelfIntersectShell( *shell, params, results );
        ac.End();
      }
      else {
        int mode = 0;
        ::GetInt( IDS_ENTER_SHELL_SELFINT_MODE, mode );
        MbShellSelfIntParams params( static_cast<MbShellSelfIntParams::MbeCheckSelfIntMode>( mode ),
                                     Math::DefaultMathVersion(), progBar.get() );
        ac.Begin();
        isSelfInt = ::IsSelfIntersectShell( *shell, params, results );
        ac.End();
      }

      size_t idx = 0;
      size_t faceIdx1, faceIdx2;
      size_t selfIntFacesCount = 0, intFacesCount = 0, curvesCount = 0;
      SpaceCurvesSPtrVector intCurves;
      while ( results.GetResult(idx++, faceIdx1, faceIdx2, &intCurves) ) {
        if ( faceIdx1 == faceIdx2 )
          ++selfIntFacesCount;
        else {
          for ( auto & cur : intCurves ) {
            TestVariables::viewManager->AddObject( *cur );
            ++curvesCount;
          }
          ++intFacesCount;
        }
      }
      const size_t bufSize = 256;
      TCHAR buf[bufSize];
      size_t len = 0;
      len += (size_t)_sntprintf( buf + len, bufSize - len, RT_TEST_RESULTS );
      len += (size_t)_sntprintf( buf + len, bufSize - len, isSelfInt ? RT_IS_SELFINTERSECTION : RT_NO_SELFINTERSECTION );
      if ( selfIntFacesCount > 0 ) {
        len += (size_t)_sntprintf( buf + len, bufSize - len, _T( "\n\n" ) );
        len += (size_t)_sntprintf( buf + len, bufSize - len, RT_SELFINTERSECTING_FACES_COUNT _T( "\t" ) F_TD, selfIntFacesCount);
      }
      if ( intFacesCount > 0 ) {
        len += (size_t)_sntprintf( buf + len, bufSize - len, _T( "\n" ) );
        len += (size_t)_sntprintf( buf + len, bufSize - len, RT_INTERSECTING_FACES_COUNT _T( "\t" ) F_TD, intFacesCount );
      }
      if ( curvesCount > 0 ) {
        len += (size_t)_sntprintf( buf + len, bufSize - len, _T( "\n" ) );
        len += (size_t)_sntprintf( buf + len, bufSize - len, RT_INTERSECTING_CURVES_COUNT _T( "\t" ) F_TD, curvesCount );
      }
      len += (size_t)_sntprintf( buf + len, bufSize - len, _T( "\n\n" ) );
      len += (size_t)_sntprintf( buf + len, bufSize - len, RT_TIME_SEC1, ac.GetLast() );
      MessageBoxEx( buf, RT_SHELL_SELFINTERSECTION );
    }
  }

  return isSelfInt;
}


//------------------------------------------------------------------------------
// Проверить эскиз
// ---
bool CheckSketch()
{
  bool res = false;

  MbItem * obj = nullptr;

  if ( GetMObj( IDS_SELECT_CURVE, pt_Curve, &obj ) ) {
    TestVariables::viewManager->ToggleObject( obj );

    MbeSpaceType objIs = obj->IsA();
    const MbPlaneInstance * pInst = nullptr;

    if ( objIs == st_PlaneInstance )
      pInst = static_cast< const MbPlaneInstance * >( obj );

    if ( pInst != nullptr ) {
      res = true;
      MbSketchErrors errors;
      MbSketchCheckParameters params;
      ::CheckSketch( *pInst, params, errors );

      TCHAR buf[1024];
      size_t len = 0;

      len += ( size_t ) _sntprintf( buf + len, 1024 - len, RT_TEST_RESULTS ); 
      if ( errors.IsAnyError( rt_InvalidType ) ) {
        len += ( size_t ) _sntprintf( buf + len, 1024 - len, RT_INVALID_TYPE );
        len += ( size_t ) _sntprintf( buf + len, 1024 - len, _T( "\n" ) );
      }
      if ( errors.IsAnyError( rt_SelfIntersection ) ) {
        len += ( size_t ) _sntprintf( buf + len, 1024 - len, RT_SELFINTERSECTION );
        len += ( size_t ) _sntprintf( buf + len, 1024 - len, _T( "\n" ) );
      }
      if ( errors.IsAnyError( rt_Intersection ) ) {
        len += ( size_t ) _sntprintf( buf + len, 1024 - len, RT_INTERSECTION );
        len += ( size_t ) _sntprintf( buf + len, 1024 - len, _T( "\n" ) );
      }
      if ( errors.IsAnyError( rt_MustBeClosed ) ) {
        len += ( size_t ) _sntprintf( buf + len, 1024 - len, RT_MUST_BE_CLOSED );
        len += ( size_t ) _sntprintf( buf + len, 1024 - len, _T( "\n" ) );
      }
      if ( errors.IsAnyError( rt_MustBeOpen ) ) {
        len += ( size_t ) _sntprintf( buf + len, 1024 - len, RT_MUST_BE_OPEN );
        len += ( size_t ) _sntprintf( buf + len, 1024 - len, _T( "\n" ) );
      }
      if ( errors.IsAnyError( rt_ContourGapError ) ) {
        len += ( size_t ) _sntprintf( buf + len, 1024 - len, RT_CONTOUR_GAB_ERROR );
        len += ( size_t ) _sntprintf( buf + len, 1024 - len, _T( "\n" ) );
      }
      if ( errors.IsAnyError( rt_ContourSegmentsOverlapError ) ) {
        len += ( size_t ) _sntprintf( buf + len, 1024 - len, RT_CONTOUR_SEGMENTS_OVERLAP_ERROR );
        len += ( size_t ) _sntprintf( buf + len, 1024 - len, _T( "\n" ) );
      }

      MessageBoxEx( buf, RT_SKETCH_CHECK );
    }
  }

  return res;
}


//------------------------------------------------------------------------------
// Расстояние между осями поверхностей, имеющих оси
// ---
bool SurfAxesDistance()
{
  MbItem * obj = nullptr;
  MbSurface * surf1 = nullptr;
  MbSurface * surf2 = nullptr;

  // Выбрать первую поверхность
  if ( surf1 == nullptr && ::GetGObj( IDS_SELECT_SURFACE, st_Surface, &obj ) )
    surf1 = (MbSurface *)((MbSpaceInstance *)obj)->GetSpaceItem();
  
  // Выбрать вторую поверхность
  if ( surf2 == nullptr && ::GetGObj( IDS_SELECT_SURFACE, st_Surface, &obj ) )
    surf2 = (MbSurface *)((MbSpaceInstance *)obj)->GetSpaceItem();

  obj = nullptr;

  if ( surf1 != nullptr && surf2 != nullptr && surf1 != surf2 ) {
    MbAxis3D axis1, axis2;
    MbCartPoint3D pnt1, pnt2;
    double dist = UNDEFINED_DBL;
    double angleRad = UNDEFINED_DBL;
    bool bAxis1 = false;
    bool bAxis2 = false;

    MbeSurfAxesMeasureRes res = ::SurfAxesDistAngle( *surf1, true, *surf2, true,
                                                     axis1, bAxis1,
                                                     axis2, bAxis2,
                                                     pnt1, pnt2, angleRad, dist );
    PRECONDITION( dist > -NULL_EPSILON );

    if ( res > samr_Undefined )
    {
      MbSpaceItem * gItem = nullptr;
      Style style;

      gItem = new MbPointFrame( pnt1 );
      style.Set( 0, 1, TestVariables::BLUE );
      TestVariables::viewManager->AddObject( style, gItem );

      gItem = new MbPointFrame( pnt2 );
      style.Set( 0, 1, TestVariables::BLUE );
      TestVariables::viewManager->AddObject( style, gItem );

      if ( dist > METRIC_PRECISION || (bAxis1 != bAxis2) ) {
        gItem = new MbLineSegment3D( pnt1, pnt2 );
        style.Set( 0, 1, TestVariables::RED );
        TestVariables::viewManager->AddObject( style, gItem );

        const MbCartPoint3D & orig1 = axis1.GetOrigin();
        const MbCartPoint3D & orig2 = axis2.GetOrigin();

        if ( !c3d::EqualPoints( pnt1, orig1, METRIC_PRECISION ) ) {
          gItem = new MbPointFrame( orig1 );
          style.Set( 0, 1, TestVariables::BROWN );
          TestVariables::viewManager->AddObject( style, gItem );
        }
        if ( !c3d::EqualPoints( pnt2, orig2, METRIC_PRECISION ) ) {
          gItem = new MbPointFrame( orig2 );
          style.Set( 0, 1, TestVariables::BROWN );
          TestVariables::viewManager->AddObject( style, gItem );
        }

        double len = UNDEFINED_DBL;
        MbVector3D vect;

        if ( bAxis1 ) {
          len = 0.5 * surf1->GetGabDiagonal();
          vect.Init( axis1.GetAxisZ() );
          vect.Normalize();
          vect *= len;
          gItem = new MbLineSegment3D( orig1 - vect, orig1 + 2*vect );
          style.Set( 0, 1, TestVariables::BROWN );
          TestVariables::viewManager->AddObject( style, gItem );
        }
        
        if ( bAxis2 ) {
          len = 0.5 * surf2->GetGabDiagonal();
          vect.Init( axis2.GetAxisZ() );
          vect.Normalize();
          vect *= len;
          gItem = new MbLineSegment3D( orig2 - vect, orig2 + 2*vect );
          style.Set( 0, 1, TestVariables::BROWN );
          TestVariables::viewManager->AddObject( style, gItem );
        }
      }

      TCHAR msgDist[256];
      size_t/*OV_x64 ulong*/ len = 0;
      len += (size_t)_sntprintf( msgDist+len, 256-len, RT_DISTANCE_BETWEEN_AXES, dist );
      double angleDeg = 180.0 * angleRad / M_PI;
      len += (size_t)_sntprintf( msgDist+len, 256-len, RT_ANGLE_BETWEEN_AXES, angleDeg );

      MessageBoxEx( msgDist, IDS_TITLE_WRN );

      gItem = nullptr;
    }
    else {
      bool b1 = surf1->GetCylinderAxis( axis1 );
      bool b2 = surf2->GetCylinderAxis( axis2 );
      TCHAR msgNoAxis1[] = RT_FIRST_SURFACE_NO_AXIS;
      TCHAR msgNoAxis2[] = RT_SECOND_SURFACE_NO_AXIS;
      TCHAR msgNoAxes[] = RT_BOTH_SURFACES_NO_AXES;
      TCHAR * msgFailed = nullptr;
      if ( !b1 && !b2)
        msgFailed = msgNoAxes;
      else if ( !b1 )
        msgFailed = msgNoAxis1;
      else if ( !b2 )
        msgFailed = msgNoAxis2;

      MessageBoxEx( msgFailed, IDS_TITLE_WRN );
    }

    return true;
  }
  else {
    if ( surf1 != nullptr && surf1 == surf2 )
    {      
      MessageBoxEx( RT_SELECT_SAME_SURFACE, IDS_TITLE_ERR );
    }      
    else if ( surf1 == nullptr || surf2 == nullptr ) {
      if ( surf1 == nullptr && surf2 == nullptr )
        MessageBoxEx( RT_SURFACES_ARE_NOT_SELECTED, IDS_TITLE_ERR );
      else
        MessageBoxEx( RT_NOT_SELECTED_ONE_OF_THE_SURFACES, IDS_TITLE_ERR );
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Расстояния между поверхностями по сетке
// ---
bool GriddedSurfMinMaxDistances()
{
  bool bRes = false;

  // Выбрать первую поверхность
  SurfaceSPtr surf1( SelectSurface() ), surf2;
  // Выбрать вторую поверхность
  if ( surf1 != nullptr )
    surf2 = SelectSurface();

  if ( surf1 != nullptr && surf2 != nullptr && !surf1->IsSame( *surf2 ) ) {
    ptrdiff_t u1cnt = surf1->GetUCount();
    if ( ::GetInt( IDS_ENTER_SPLIT_COUNT_U, u1cnt ) ) {
      u1cnt = std_max( u1cnt, (ptrdiff_t)1 );
      u1cnt = std_min( u1cnt, (ptrdiff_t)c3d::ARRAY_MAX_COUNT );
      ptrdiff_t v1cnt = surf1->GetVCount();
      if ( ::GetInt( IDS_ENTER_SPLIT_COUNT_V, v1cnt ) ) {
        v1cnt = std_max( v1cnt, (ptrdiff_t)1 );
        v1cnt = std_min( v1cnt, (ptrdiff_t)c3d::ARRAY_MAX_COUNT );

        ptrdiff_t nMin = 1; // Кол-во минимумов
        ptrdiff_t nMax = 1; // Кол-во максимумов
        ptrdiff_t cntMax = (ptrdiff_t)( (u1cnt * v1cnt) / 2 );
        nMin = std_max( nMin, cntMax );
        nMax = std_max( nMax, cntMax );

        if ( ::GetInt( IDS_ENTER_QUANTITY, nMin ) &&
             ::GetInt( IDS_ENTER_QUANTITY, nMax ) ) {
          nMin = std_max( nMin, (ptrdiff_t)1 );
          nMax = std_max( nMax, (ptrdiff_t)1 );
          nMin = std_min( nMin, (ptrdiff_t)c3d::ARRAY_MAX_COUNT );
          nMax = std_min( nMax, (ptrdiff_t)c3d::ARRAY_MAX_COUNT );

          ptrdiff_t iUseDir = 0; // Не использовать, по умолчанию
          MbVector3D * dir = nullptr;

          if ( ::GetInt( IDS_ENTER_USE_OR_NOT, iUseDir ) ) {
            if ( iUseDir > 0 ) iUseDir = 1;
            else               iUseDir = 0;

            if ( iUseDir > 0 ) {
              MbItem * obj = nullptr;
              if ( ::GetGObj( IDS_SELECT_LINE, st_Curve3D, &obj ) && (obj->IsA() == st_SpaceInstance) ) {
                const MbSpaceItem * sItem = ((MbSpaceInstance *)obj)->GetSpaceItem(); // Дать трёхмерный геометрический объекта
                if ( (sItem != nullptr) && (sItem->Family() == st_Curve3D) ) {
                  const MbCurve3D & dirCurve = *(static_cast<const MbCurve3D *>(sItem));
                  if ( dirCurve.IsStraight() ) {
                    dir = new MbVector3D;
                    dirCurve.GetLimitTangent( 1, *dir );
                    bRes = true;
                  }
                  else if ( dirCurve.Type() == st_Contour3D ) {
                    const MbContour3D & dirCntr = static_cast<const MbContour3D &>(dirCurve);
                    if ( dirCntr.GetSegmentsCount() == 1 && dirCntr.GetSegment( 0 ) != nullptr ) {
                      const MbCurve3D & dirSegment = *dirCntr.GetSegment( 0 );
                      if ( dirSegment.IsStraight() ) {
                        dir = new MbVector3D;
                        dirSegment.GetLimitTangent( 1, *dir );
                        bRes = true;
                      }
                    }
                  }
                }
              }
              else {
                MbCartPoint3D dirPnt;
                if ( GetPoint( IDS_ENTER_DIR, dirPnt ) ) {
                  dir = new MbVector3D( dirPnt );
                  bRes = true;
                }
              }

              if ( !bRes )
                MessageBoxEx( RT_NOT_SELECT_GUIDE_SEGMENT, IDS_TITLE_ERR );
            }
            else
              bRes = true;
          }
          else
            bRes = true;

          if ( bRes ) {
            bRes = false;

            MbeProcessState eMinMax = mps_Error;
            static bool useEqual = true; // Использовать равные расстояния

            MbeSenseValue sense = orient_BOTH;
            {
              ptrdiff_t iSense = 0;
              if ( ::GetInt( IDS_ENTER_SENSE, iSense ) ) {
                if ( iSense > 0 )      sense = orient_FORWARD;
                else if ( iSense < 0 ) sense = orient_BACK;
              }
            }

            std::unique_ptr<IProgressIndicator> progBar( &::CreateProgressIndicator(nullptr) );

            MbMinMaxGridDistancesParams params( *surf1, u1cnt, v1cnt, dir, sense, useEqual, *surf2, nMin, nMax );
            params.SetProgressIndicator( progBar.get() );

            MbMinMaxGridDistancesResults<MbCartPoint, MbCartPoint> results;
          
            eMinMax = ::MinMaxSurfaceSurfaceGridDistances( params, results );

            if ( eMinMax == mps_Stop )
            {        
              MessageBoxEx( RT_PROCESS_IS_STOPPED, IDS_TITLE_MSG );
            }        

            if ( (eMinMax == mps_Success || eMinMax == mps_Stop) &&
                 (results.allResults.GetCount() > 0 || results.minResults.GetCount() > 0 || results.maxResults.GetCount() > 0) ) {
              double d = UNDEFINED_DBL;
              MbCartPoint uv1, uv2;
              MbCartPoint3D p1, p2;

              size_t k(0), kcnt(0);
              Style style;

              for ( k = 0, kcnt = results.minResults.GetCount(); k < kcnt; k++ ) {
                if ( results.minResults.GetItemDistance( k, d, uv1, uv2 ) ) {
                  surf1->PointOn( uv1, p1 );
                  surf2->PointOn( uv2, p2 );
                  MbLineSegment3D * ls = new MbLineSegment3D( p1, p2 );
                  style.Set( 0, 1, TestVariables::BLUE );
                  TestVariables::viewManager->AddObject( style, ls );
                }
              }

              for ( k = 0, kcnt = results.maxResults.GetCount(); k < kcnt; k++ ) {
                if ( results.maxResults.GetItemDistance( k, d, uv1, uv2 ) ) {
                  surf1->PointOn( uv1, p1 );
                  surf2->PointOn( uv2, p2 );
                  MbLineSegment3D * ls = new MbLineSegment3D( p1, p2 );
                  style.Set( 0, 1, TestVariables::RED );
                  TestVariables::viewManager->AddObject( style, ls );
                }
              }

              bRes = true;
            }
            else
            {        
              MessageBoxEx( RT_ERROR_IN_PARAMS, IDS_TITLE_ERR );
            }      
          }

          if ( dir != nullptr ) {
            delete dir;
            dir = nullptr;
          }
        }
      }
    }
  }

  return bRes;
}


//------------------------------------------------------------------------------
//  Максимальные по модулю кривизны поверхности.
//------------------------------------------------------------------------------
void SurfaceMaxMinCurvatures()
{
  MbItem * obj  = nullptr;
  MbSurface  * surf = nullptr;
  
  // Выбрать поверхность
  if ( ::GetGObj(IDS_SELECT_SURFACE, st_Surface, &obj) ) {
    TestVariables::viewManager->ToggleObject( obj );
    surf = (MbSurface *)((MbSpaceInstance *)obj)->GetSpaceItem();
    ptrdiff_t curvType = 0, algType = 2;
    if ( GetInt(IDS_ENTER_SURF_CURVATURE_TYPE, curvType) && (curvType >= 0 && curvType <= 6) ) {
      if ( GetInt(IDS_ENTER_EXTREMES_SEARCH_ALG, algType) && (algType >= 1 && algType <= 2) ) {
        MbCartPoint neg, pos;
        double negC, posC;
        if ( curvType == 0 )
          SurfaceMinMaxCurvature( *surf, negC, neg, posC, pos, (MbeExtremsSearchingMethod)algType );
        else {
          SurfaceFunction funcs[6] = { MinSurfaceCurvature, MaxSurfaceCurvature, GaussCurvature, MeanCurvature, UNormalCurvature, VNormalCurvature };
          SurfaceMinMaxCurvature( *surf, funcs[curvType - 1], negC, neg, posC, pos, (MbeExtremsSearchingMethod)algType );
        }
        MbCartPoint3D neg3D, pos3D;
        if ( negC != 0.0 ) {
          surf->_PointOn( neg, neg3D );
          TestVariables::viewManager->AddObject( Style(5, RGB(0, 0, 250)), SPtr<MbPointFrame>(new MbPointFrame(neg3D)) );
        }
        if ( posC != 0.0 ) {
          surf->_PointOn( pos, pos3D );
          TestVariables::viewManager->AddObject( Style(5, RGB(250, 0, 0)), SPtr<MbPointFrame>(new MbPointFrame(pos3D)) );
        }
        TCHAR s[128];
        _sntprintf( s, 128, RT_MAX_AND_MIN_CURVATURE, posC, negC );
        MessageBoxEx( s, IDS_RESULTS );
      }
    }
  }
}


//------------------------------------------------------------------------------
//  Максимальные по модулю кривизны оболочки.
//------------------------------------------------------------------------------
void FacesMaxMinCurvatures()
{ 
  MbItem * obj = nullptr;
  MbTopItem * surface = nullptr;
  ConstFacesVector faces;
  const MbFace * face = nullptr;
  size_t i0;

  while ( GetTObj(IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &surface) ) {
    TestVariables::viewManager->ToggleObject( obj );
    face = static_cast<const MbFace *>(surface);
    i0 = c3d::DirectSearch( faces, face );
    if ( i0 != SYS_MAX_T )
      faces.erase( faces.begin() + i0 );
    else
      faces.push_back( face );
  }
  if( faces.size() > 0 ) {
    ptrdiff_t curvType = 0, algType = 3;
    if ( GetInt(IDS_ENTER_SURF_CURVATURE_TYPE, curvType) && (curvType >= 0 && curvType <= 6) ) {
      if ( GetInt(IDS_ENTER_EXTREMES_SEARCH_ALG, algType) && (algType >= 1 && algType <= 3) ) {
        MbCartPoint neg, pos;
        double negC, posC;
        const MbFace *negF = 0, *posF = 0;
        if ( curvType == 0 )
          FacesMinMaxCurvature( faces, negC, negF, neg, posC, posF, pos, true, (MbeExtremsSearchingMethod)algType );
        else {
          SurfaceFunction funcs[6] = { MinSurfaceCurvature, MaxSurfaceCurvature, GaussCurvature, MeanCurvature, UNormalCurvature, VNormalCurvature };
          FacesMinMaxCurvature( faces, funcs[curvType - 1], negC, negF, neg, posC, posF, pos, true, (MbeExtremsSearchingMethod)algType );
        }
        MbCartPoint3D neg3D, pos3D;
        if ( negC != 0.0 ) {
          negF->GetSurface().GetBasisSurface()._PointOn( neg, neg3D );
          TestVariables::viewManager->AddObject( Style(5, RGB(0, 0, 250)), SPtr<MbPointFrame>(new MbPointFrame(neg3D )) );
        }
        if ( posC != 0.0 ) {
          posF->GetSurface().GetBasisSurface()._PointOn( pos, pos3D );
          TestVariables::viewManager->AddObject( Style(5, RGB(250, 0, 0)), SPtr<MbPointFrame>(new MbPointFrame(pos3D)) );
        }
        TCHAR s[128];
        _sntprintf( s, 128, RT_MAX_AND_MIN_CURVATURE, posC, negC );
        MessageBoxEx( s, IDS_RESULTS );
      }
    }
  }
}


//------------------------------------------------------------------------------
//  Максимальные по модулю кривизны кривой.
//------------------------------------------------------------------------------
void CurveMaxMinCurvatures()
{
  MbItem * obj = nullptr;
  SPtr<const MbCurve3D> curv;

  // Выбрать поверхность
  if ( ::GetGObj(IDS_SELECT_CURVE, st_Curve3D, &obj) ) {
    TestVariables::viewManager->ToggleObject( obj );

    if ( obj->IsA() == st_SpaceInstance ) {
      const MbSpaceInstance * sInst = static_cast<MbSpaceInstance *>( obj );
      const MbSpaceItem * sItem = sInst->GetSpaceItem();
      if ( sItem != nullptr && sItem->Family() == st_Curve3D )
        curv = static_cast<const MbCurve3D *>( sItem );
    }
    else if ( obj->IsA() == st_PlaneInstance ) {
      const MbPlaneInstance * pInst = static_cast<MbPlaneInstance *>( obj );
      const MbPlaneItem * pItem = pInst->GetPlaneItem();
      if ( pItem != nullptr && pItem->Family() == pt_Curve )
        curv = new MbPlaneCurve( pInst->GetPlacement(), static_cast<const MbCurve &>(*pItem), false );
    }
    if ( curv != nullptr ) {
      MbCurveMinMaxCurvatureParams param( true, true, true, true );
      MbCurveMinMaxCurvatureResults result;
      ::CurveMinMaxCurvature( *curv, param, result );
      double posC = result.GetMaxCurvatureValue();
      double pos = result.GetMaxCurvatureParam();
      double negC = result.GetMinCurvatureValue();
      double neg = result.GetMinCurvatureParam();

      MbCartPoint3D neg3D, pos3D;
      curv->_PointOn( pos, pos3D );
      TestVariables::viewManager->AddObject( Style(5, RGB(250, 0, 0)), SPtr<MbPointFrame>(new MbPointFrame(pos3D)) );
      curv->_PointOn( neg, neg3D );
      TestVariables::viewManager->AddObject( Style(5, RGB(0, 0, 250)), SPtr<MbPointFrame>(new MbPointFrame(neg3D)) );
      /*
      c3d::DoubleVector bends = result.GetBendingPoints();
      c3d::DoubleVector maxs = result.GetLocalMaximumPoints();
      c3d::DoubleVector mins = result.GetLocalMinimumPoints();
      c3d::DoublePairsVector raps = result.GetBreakPoints();
    SPtr<MbPointFrame> bendPoints( new MbPointFrame() );
    for ( size_t i = 0, icount = bends.size(); i < icount; i++ ) {
      MbCartPoint3D p;
      curv->_PointOn( bends[i], p );
      bendPoints->AddVertex( p );
    }
    TestVariables::viewManager->AddObject( Style(2, RGB(0, 255, 0)), bendPoints );
    
    SPtr<MbPointFrame> maxPoints( new MbPointFrame() );
    for ( size_t i = 0, icount = maxs.size(); i < icount; i++ ) {
      MbCartPoint3D p;
      curv->_PointOn( maxs[i], p );
      maxPoints->AddVertex( p );
    }
    TestVariables::viewManager->AddObject( Style( 2, RGB( 255, 0, 0 ) ), maxPoints );
    
    SPtr<MbPointFrame> minPoints( new MbPointFrame() );
    for ( size_t i = 0, icount = mins.size(); i < icount; i++ ) {
      MbCartPoint3D p;
      curv->_PointOn( mins[i], p );
      minPoints->AddVertex( p );
    }
    TestVariables::viewManager->AddObject( Style( 2, RGB( 0, 0, 255) ), minPoints );

    SPtr<MbPointFrame> rapPoints( new MbPointFrame() );
    for ( size_t i = 0, icount = raps.size(); i < icount; i++ ) {
      MbCartPoint3D p;
      curv->_PointOn( raps[i].first, p );
      rapPoints->AddVertex( p );
    }
    TestVariables::viewManager->AddObject( Style( 2, RGB( 0, 0, 0 ) ), rapPoints );
*/
      TCHAR s[128];
      _sntprintf( s, 128, RT_MAX_AND_MIN_CURVATURE, posC, negC );
      MessageBoxEx( s, IDS_RESULTS );
    }
  }
}


//------------------------------------------------------------------------------
// Максимальное расстояние между геометрическими объектами
// ---
bool GeomItemsMaxDistance( const MbSpaceItem * gitem1, const MbSpaceItem * gitem2 )
{
  bool bRes = false;
  
  TCHAR msgInfoTitle[] = IDS_TITLE_MSG;

  if ( gitem1 == nullptr || gitem2 == nullptr ) {
    MbItem * obj1 = nullptr;
    MbItem * obj2 = nullptr;

    TCHAR msgRightTypes[] = RT_SELECT_POINT_CURVE_OR_SURFACE;
    //TCHAR msgNoObject1[] = RT_NOT_SELECTED_FIRTS_OBJECT;
    TCHAR msgNoObject2[] = RT_NOT_SELECTED_SECOND_OBJECT;

    while ( !bRes && GetGObj( IDS_SELECT_GEOM_OBJECT, st_SpaceItem, &obj1 ) ) {
      if ( obj1->IsA() == st_SpaceInstance ) 
        gitem1 = ((MbSpaceInstance *)obj1)->SetSpaceItem(); // Дать трёхмерный геометрический объекта

      if ( gitem1 != nullptr && ((gitem1->Family() == st_Point3D) || (gitem1->Family() == st_Curve3D) || (gitem1->Family() == st_Surface)) ) {
        TestVariables::viewManager->SelectObject(obj1);

        while ( !bRes && GetGObj( IDS_SELECT_GEOM_OBJECT, st_SpaceItem, &obj2 ) ) {
          if ( obj2->IsA() == st_SpaceInstance ) 
            gitem2 = ((MbSpaceInstance *)obj2)->SetSpaceItem(); // Дать трёхмерный геометрический объекта

          if ( gitem1 != gitem2 ) {
            if ( gitem2 != nullptr && ((gitem2->Family() == st_Point3D) || (gitem2->Family() == st_Curve3D) || (gitem2->Family() == st_Surface)) ) {
              TestVariables::viewManager->SelectObject(obj2);
              bRes = true;
              TestVariables::viewManager->UnselectObject( obj1 );
              TestVariables::viewManager->UnselectObject( obj2 );
            }
            else
            {        
              MessageBoxEx( msgRightTypes, msgInfoTitle );
            }        
          }

          if ( !bRes )
            TestVariables::viewManager->UnselectObject( obj1 );
        }

        if ( obj2 == nullptr )
        {    
          MessageBoxEx( msgNoObject2, msgInfoTitle );
        }    
      }
      else
      {  
        MessageBoxEx( msgRightTypes, msgInfoTitle );
      }  
    }

    if ( obj1 == nullptr )
    {      
      MessageBoxEx( msgNoObject2, msgInfoTitle );
    }  
  }

  if ( gitem1 != nullptr && ((gitem1->Family() == st_Point3D) || (gitem1->Family() == st_Curve3D) || (gitem1->Family() == st_Surface)) &&
       gitem2 != nullptr && ((gitem2->Family() == st_Point3D) || (gitem2->Family() == st_Curve3D) || (gitem2->Family() == st_Surface)) ) {
    bRes = true;
    double dMax = -LENGTH_EPSILON;
    MbCartPoint3D pnt1, pnt2;
    MbCurve3D * curve1 = nullptr;
    MbCurve3D * curve2 = nullptr;
    MbSurface * surface1 = nullptr;
    MbSurface * surface2 = nullptr;

    if ( gitem1->IsA() == st_Point3D )
      ((MbPoint3D *)gitem1)->GetCartPoint( pnt1 );
    else if ( gitem1->Family() == st_Curve3D )
      curve1 = (MbCurve3D *)gitem1;
    else if ( gitem1->Family() == st_Surface )
      surface1 = (MbSurface *)gitem1;

    if ( gitem2->IsA() == st_Point3D )
      ((MbPoint3D *)gitem2)->GetCartPoint( pnt2 );
    else if ( gitem2->Family() == st_Curve3D )
      curve2 = (MbCurve3D *)gitem2;
    else if ( gitem2->Family() == st_Surface )
      surface2 = (MbSurface *)gitem2;

    PRECONDITION( curve1 == nullptr || surface1 == nullptr );
    PRECONDITION( curve2 == nullptr || surface2 == nullptr );

    if ( (curve1 == nullptr && surface1 == nullptr) &&
         (curve2 == nullptr && surface2 == nullptr) ) { // Точка-точка
      dMax = pnt1.DistanceToPoint( pnt2 );
    }
    else if ( (curve1 != nullptr) && (curve2 == nullptr && surface2 == nullptr) ) { // Точка-кривая
      double t = UNDEFINED_DBL;
      if ( ::MaxDistance( pnt2, *curve1, t, dMax ) )
        curve1->_PointOn( t, pnt1 );
    }
    else if ( (curve2 != nullptr) && (curve1 == nullptr && surface1 == nullptr) ) { // Точка-кривая
      double t = UNDEFINED_DBL;
      if ( ::MaxDistance( pnt1, *curve2, t, dMax ) )
        curve2->_PointOn( t, pnt2 );
    }
    else if ( (curve1 != nullptr) && (curve2 != nullptr) ) { // Кривая-кривая
      double t1, t2;
      t1 = t2 = UNDEFINED_DBL;

      bool isCurveAxis = false;
      if ( curve2->IsA() == st_Line3D ) { //if ( curve2->IsStraight() ) {
        ptrdiff_t iCurveIsAxis = 0;
        GetInt( IDS_ENTER_CURVE_IS_AXIS, iCurveIsAxis );
        if ( iCurveIsAxis > 0 )
          isCurveAxis = true;
      }

      if ( isCurveAxis ) {
        MbCartPoint3D pnt;
        MbVector3D tau;
        curve2->GetLimitPoint( 1, pnt );
        curve2->GetLimitTangent( 1, tau );
        MbAxis3D axis( pnt, tau );
        if ( ::MaxDistance( axis, *curve1, t1, dMax ) ) {
          curve1->_PointOn( t1, pnt1 );
          axis.PointProjection( pnt1, pnt2 );
          curve2->NearPointProjection( pnt2, t2, true );
          curve2->_PointOn( t2, pnt2 );
        }
      }
      else if ( ::MaxDistance( *curve1, *curve2, t1, t2, dMax ) ) {
        curve1->_PointOn( t1, pnt1 );
        curve2->_PointOn( t2, pnt2 );
      }
    }
    else if ( (surface1 != nullptr) && (surface2 == nullptr && curve2 == nullptr) ) { // Точка-поверхность
      MbCartPoint uv;
      if ( ::MaxDistance( pnt2, *surface1, uv, dMax ) )
        surface1->_PointOn( uv.x, uv.y, pnt1 );
    }
    else if ( (surface2 != nullptr) && (surface1 == nullptr && curve1 == nullptr) ) { // Точка-поверхность
      MbCartPoint uv;
      if ( ::MaxDistance( pnt1, *surface2, uv, dMax ) )
        surface2->_PointOn( uv.x, uv.y, pnt2 );
    }
    else if ( (curve1 != nullptr) && (surface2 != nullptr) ) { // Кривая-поверхность
      double t1;
      MbCartPoint uv2;
      if ( ::MaxDistance( *curve1, *surface2, t1, uv2, dMax ) ) {
        curve1->_PointOn( t1, pnt1 );
        surface2->_PointOn( uv2.x, uv2.y, pnt2 );
      }
    }
    else if ( (curve2 != nullptr) && (surface1 != nullptr) ) { // Кривая-поверхность
      double t2;
      MbCartPoint uv1;
      if ( ::MaxDistance( *curve2, *surface1, t2, uv1, dMax ) ) {
        curve2->_PointOn( t2, pnt2 );
        surface1->_PointOn( uv1.x, uv1.y, pnt1 );
      }
    }
    else if ( (surface1 != nullptr) && (surface2 != nullptr) ) { // Поверхность-поверхность
      MbCartPoint uv1, uv2;
      if ( ::MaxDistance( *surface1, *surface2, uv1, uv2, dMax ) ) {
        surface1->_PointOn( uv1.x, uv1.y, pnt1 );
        surface2->_PointOn( uv2.x, uv2.y, pnt2 );
      }
    }

    if ( dMax > 0 ) {
      MbSpaceItem * gitem = nullptr;
      Style style;

      if ( gitem1->IsA() != st_Point3D ) {
        gitem = new MbPoint3D( pnt1 );
        style.Set( 0, 1, TestVariables::RED );
        TestVariables::viewManager->AddObject( style, gitem );
      }

      if ( gitem2->IsA() != st_Point3D ) {
        gitem = new MbPoint3D( pnt2 );
        style.Set( 0, 1, TestVariables::RED );
        TestVariables::viewManager->AddObject( style, gitem );
      }
      
      if ( dMax > METRIC_PRECISION ) {
        gitem = new MbLineSegment3D( pnt1, pnt2 );
        style.Set( 0, 1, TestVariables::BROWN );
        TestVariables::viewManager->AddObject( style, gitem );

        TCHAR s[128];
        _sntprintf( s, 128, RT_MAX_DISTANCE, dMax );

        MessageBoxEx( s, IDS_RESULTS );
      }
      else
      {  
        MessageBoxEx( RT_DISTANCE_LESS_THAN_ERROR, msgInfoTitle );
      }  
    }
    else
    {      
      MessageBoxEx( RT_DISTANCE_IS_DEGENERATE, msgInfoTitle );
    }      
  }

  return bRes;
}


//------------------------------------------------------------------------------
// Максимальное расстояние между топологическими объектами
// ---
bool TopoItemsMaxDistance()
{
  bool bRes = false;
  MbItem * obj1 = nullptr;
  MbItem * obj2 = nullptr;
  MbTopItem * titem1 = nullptr;
  MbTopItem * titem2 = nullptr;
  MbMatrix3D from1, from2;

  if ( GetTObj( IDS_SELECT_TOPO_OBJECT, st_Solid, tt_TopItem, &obj1, &titem1, from1 ) ) {
    if ( titem1 != nullptr ) {
      const MbSpaceItem * gitem1 = nullptr;

      if ( titem1->IsAVertex() ) {
        gitem1 = new MbPoint3D( static_cast<const MbVertex *>(titem1)->GetCartPoint() );
        if ( !from1.IsSingle() ) 
          ((MbSpaceItem *)gitem1)->Transform( from1 ); // SD_7163577
      }
      else if ( titem1->IsAWireEdge() ) {
        gitem1 = &static_cast<const MbEdge *>( titem1 )->GetCurve();
      }
      else if ( titem1->IsAnEdge() ) {
        if ( titem1->IsA() == tt_CurveEdge )
          gitem1 = &static_cast<const MbCurveEdge *>(titem1)->GetCurve();
      }
      else if ( titem1->IsAFace() ) {
        gitem1 = &static_cast<const MbFace *>(titem1)->GetSurface();
      }

      if ( gitem1 != nullptr ) {
        if ( !from1.IsSingle() && gitem1->IsA() != st_Point3D ) { // SD_7163577
          MbSpaceItem * copy1 = &gitem1->Duplicate();
          copy1->Transform( from1 );
          gitem1 = copy1;
        }

        gitem1->AddRef();

        if ( GetTObj( IDS_SELECT_TOPO_OBJECT, st_Solid, tt_TopItem, &obj2, &titem2, from2 ) && (titem1 != titem2) ) {
          if ( titem2 != nullptr ) {
            const MbSpaceItem * gitem2 = nullptr;

            if ( titem2->IsAVertex() ) {
              gitem2 = new MbPoint3D( static_cast<const MbVertex *>(titem2)->GetCartPoint() );
              if ( !from2.IsSingle() ) 
                ((MbSpaceItem *)gitem2)->Transform( from2 ); // SD_7163577
            }
            else if ( titem2->IsAWireEdge() ) {
              gitem2 = &static_cast<const MbEdge *>( titem2 )->GetCurve();
            }
            else if ( titem2->IsAnEdge() ) {
              if ( titem2->IsA() == tt_CurveEdge )
                gitem2 = &static_cast<const MbCurveEdge *>(titem2)->GetCurve();
            }
            else if ( titem2->IsAFace() ) {
              gitem2 = &static_cast<const MbFace *>(titem2)->GetSurface();
            }

            if ( gitem2 != nullptr ) {
              if ( !from2.IsSingle() && gitem2->IsA() != st_Point3D ) { // SD_7163577
                MbSpaceItem * copy2 = &gitem2->Duplicate();
                copy2->Transform( from2 );
                gitem2 = copy2;
              }

              gitem2->AddRef();

              bRes = ::GeomItemsMaxDistance( gitem1, gitem2 );

              ::ReleaseItem( gitem2 );
            }
          }
        }

        ::ReleaseItem( gitem1 );
      }      
    }
  }

  return bRes;
}


//------------------------------------------------------------------------------
// Положение точки относительно тела
// ---
bool PointLocation()
{
  bool res = false;
  MbItem * obj1 = nullptr;
  MbItem * obj2 = nullptr;

  if ( GetGObj( IDS_SELECT_POINT, st_Point3D, &obj1 ) ) {
    TestVariables::viewManager->ToggleObject( obj1 );
    if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj2 ) && (obj2->IsA() == st_Solid) ) { 
      TestVariables::viewManager->ToggleObject( obj2 );
      MbCartPoint3D pnt;

      if ( obj1->IsA() == st_PointFrame ) {
        MbPointFrame * point = static_cast<MbPointFrame *>(obj1);
        point->GetCartPoint( pnt );
      }
      else if ( obj1->IsA() == st_SpaceInstance ) {
        MbSpaceInstance * spaceInst = static_cast<MbSpaceInstance *>( obj1 );
        if ( spaceInst->GetSpaceItem() != nullptr ) {
          if ( spaceInst->GetSpaceItem()->IsA() == st_Point3D ) {
            MbPoint3D * point = static_cast<MbPoint3D *>( spaceInst->SetSpaceItem() );
            point->GetCartPoint( pnt );
          }
          else if ( spaceInst->GetSpaceItem()->IsA() == st_Marker ) {
            MbMarker * point = static_cast<MbMarker *>( spaceInst->SetSpaceItem() );
            pnt = point->GetOrigin();
          }
        }
      }

      MbSolid * solid = static_cast<MbSolid *>( obj2 );

      MbeItemLocation iLoc = solid->PointClassification( pnt );

      TCHAR s[255];

      switch ( iLoc )
      {
        case iloc_Undefined : _sntprintf( s, 255, RT_LOC_UNDEF   ); break;
        case iloc_Unknown   : _sntprintf( s, 255, RT_LOC_UNKNOWN ); break;
        case iloc_OutOfItem : _sntprintf( s, 255, RT_LOC_OUT     ); break;
        case iloc_OnItem    : _sntprintf( s, 255, RT_LOC_ON      ); break;
        case iloc_InItem    : _sntprintf( s, 255, RT_LOC_IN      ); break;
        case iloc_ByItem    : _sntprintf( s, 255, RT_LOC_BY      ); break;
        default             : PRECONDITION( false );          break;
      }
      TestVariables::viewManager->ToggleObject( obj2 );
      MessageBoxEx( s, IDS_RESULTS );
      res = true;
    }
    TestVariables::viewManager->ToggleObject( obj1 );
  }

  return res;
}


//------------------------------------------------------------------------------
// Расстояние от точки до точки
// ---
bool PointDistanceToPoint()
{
  bool res = false;
  MbItem * obj1 = nullptr;
  MbItem * obj2 = nullptr;

  if ( GetGObj( IDS_SELECT_POINT, st_Point3D, &obj1 ) ) {
    if ( GetGObj( IDS_SELECT_OBJECT, st_SpaceItem, &obj2 ) ) { 
      MbCartPoint3D pnt;

      if ( obj1->IsA() == st_PointFrame ) {
        MbPointFrame * point = static_cast<MbPointFrame *>(obj1);
        point->GetCartPoint( pnt );
      }
      else if ( obj1->IsA() == st_SpaceInstance ) {
        MbSpaceInstance * spaceInst = static_cast<MbSpaceInstance *>( obj1 );
        if ( spaceInst->GetSpaceItem() != nullptr ) {
          if ( spaceInst->GetSpaceItem()->IsA() == st_Point3D ) {
            MbPoint3D * point = static_cast<MbPoint3D *>( spaceInst->SetSpaceItem() );
            point->GetCartPoint( pnt );
          }
          else if ( spaceInst->GetSpaceItem()->IsA() == st_Marker ) {
            MbMarker * point = static_cast<MbMarker *>( spaceInst->SetSpaceItem() );
            pnt = point->GetOrigin();
          }
        }
      }

      double d = obj2->DistanceToPoint( pnt );
      TCHAR s[110];
      _sntprintf( s, 110, RT_POINT_OBJECT_DISTANCE, d );

      MessageBoxEx( s, IDS_RESULTS );
      res = true;
    }
  }

  return res;
}


//------------------------------------------------------------------------------
// Расстояние от точки до объекта
// ---
bool DistanceToPointObject()
{
  bool res = false;
  MbTopItem * item = nullptr;
  MbItem * obj = nullptr;

  if ( GetTObj( IDS_SELECT_POINT, st_Solid, tt_Vertex, &obj, &item ) ) {
    MbCartPoint3D point( static_cast<const MbVertex *>(item)->GetCartPoint() );

    double d = UNDEFINED_DBL;
    if ( GetTObj( IDS_SELECT_GEOM_OBJECT, st_Solid, tt_TopItem, &obj, &item ) ) {
      if ( item->IsAVertex() ) {
        d = static_cast<const MbVertex *>(item)->GetCartPoint().DistanceToPoint( point );
        res = true;
      }
      else if ( item->IsAWireEdge() ) {
        d = static_cast<const MbEdge *>(item)->GetCurve().DistanceToPoint( point );
        res = true;
      }
      else if ( item->IsAnEdge() ) {
        d = static_cast<const MbCurveEdge *>(item)->GetCurve().DistanceToPoint( point );
        res = true;
      }
      else if ( item->IsAFace() ) {
        d = static_cast<const MbFace *>(item)->GetSurface().GetSurface().DistanceToPoint( point );
        res = true;
      }
    }

    if ( res ) {
      TCHAR s[110];
      _sntprintf( s, 110, RT_POINT_OBJECT_DISTANCE, d );

      MessageBoxEx( s, IDS_RESULTS );
    }
  }

  return res;
}


//------------------------------------------------------------------------------
// Угол между гранями ребра
// ---
bool GetEdgeFacesAngle()
{
  bool res = false;

  ConstEdgeSPtr edge, edge1, edge2;
  FaceSPtr face1, face2;
  ConstIntersectionCurveSPtr intCurve;

  double spaceAccuracy = METRIC_PRECISION;

  PathMatrix pathFrom, pathFrom1, pathFrom2;
  {
    MbItem * obj = nullptr;
    MbTopItem * item = nullptr;
    if ( ::GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &item, pathFrom ) ) {
      if ( item->IsAnEdge() ) {
        edge = static_cast<const MbCurveEdge *>(item);
        if ( edge ) {
          intCurve = &edge->GetIntersectionCurve();
        }
      }
    }
    else if ( ::GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &item, pathFrom1 ) ) {
      if ( item->IsAFace() ) {
        face1 = static_cast<MbFace *>(item);
        if ( face1 ) {
          if ( ::GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &item, pathFrom2 ) ) {
            if ( item->IsAFace() ) {
              face2 = static_cast<MbFace *>(item);
              if ( face2 ) {
                if ( face1->IsConnectedWith( face2 ) ) {
                  if ( ::GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &item, pathFrom ) ) {
                    if ( item->IsAnEdge() ) {
                      edge = static_cast<const MbCurveEdge *>(item);
                      if ( edge ) {
                        intCurve = &edge->GetIntersectionCurve();
                      }
                    }
                  }
                }
                else {
                  edge1 = nullptr;
                  edge2 = nullptr;

                  bool sameMtr = c3d::EqualMatrices( pathFrom1.second, pathFrom2.second, PARAM_EPSILON );

                  ConstEdgesSet edgesSet1, edgesSet2;

                  if ( !sameMtr ) {
                    face1 = face1->DataDuplicate();
                    face2 = face2->DataDuplicate();
                    face1->Transform( pathFrom1.second );
                    face2->Transform( pathFrom2.second );
                  }

                  face1->GetEdgesSet( edgesSet1 );
                  face2->GetEdgesSet( edgesSet2 );

                  spaceAccuracy = DELTA_MOD;// DELTA_MIN;
                  GetDouble( IDS_ENTER_ACCURACY, spaceAccuracy );

                  ConstEdgesSet::iterator it1, it2;
                  MbCube cube1, cube2;
                  for ( it1 = edgesSet1.begin(); it1 != edgesSet1.end() && (edge1 == nullptr); it1++ ) {
                    const MbCurveEdge * e1 = *it1;
                    cube1.SetEmpty();
                    e1->AddYourGabaritTo( cube1 );
                    for ( it2 = edgesSet2.begin(); it2 != edgesSet2.end() && (edge2 == nullptr); it2++ ) {
                      const MbCurveEdge * e2 = *it2;
                      cube2.SetEmpty();
                      e2->AddYourGabaritTo( cube2 );

                      if ( cube1.Intersect( cube2, spaceAccuracy ) ) {
                        const MbSurfaceIntersectionCurve & ic1 = e1->GetIntersectionCurve();
                        const MbSurfaceIntersectionCurve & ic2 = e2->GetIntersectionCurve();

                        double dmin = MB_MAXDOUBLE;
                        double t1 = UNDEFINED_DBL;
                        double t2 = UNDEFINED_DBL;
                        if ( ::NearestPoints( ic1, false, ic2, false, t1, t2, dmin ) != nr_Failure ) {
                          if ( dmin <= spaceAccuracy ) {
                            if ( ic1.IsSpaceNear( ic2, spaceAccuracy, false ) && ic2.IsSpaceNear( ic1, spaceAccuracy, false ) ) {
                              edge1 = e1;
                              edge2 = e2;
                            }
                          }
                        }
                      }
                    }
                  }
                  if ( edge1 && edge2 ) {
                    edge = nullptr;
                    intCurve = nullptr;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( ::GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj, pathFrom ) ) {
      if ( obj->IsA() == st_SpaceInstance ) {
        const MbCurve3D * spaceCurve = static_cast<const MbCurve3D *>((static_cast<MbSpaceInstance *>(obj))->GetSpaceItem());
        if ( spaceCurve != nullptr ) {
          if ( spaceCurve->IsA() == st_SurfaceIntersectionCurve ) {
            intCurve = static_cast<const MbSurfaceIntersectionCurve *>(spaceCurve);
          }
        }
      }
    }
  }

  if ( intCurve || (edge1 && edge2) ) {
    TestVariables::viewManager->ToggleObject( pathFrom.first );

    MbCartPoint3D pnt;

    double t = 0.5;
    if ( intCurve != nullptr && GetDouble( IDS_ENTER_PAR_ON_CURVE, t ) ) {
      if ( edge == nullptr ) {
        edge = new MbCurveEdge( *intCurve, true );
        face1 = new MbFace( intCurve->GetCurveOneSurface(), true );
        face2 = new MbFace( intCurve->GetCurveTwoSurface(), true );
      }

      edge->Point( t, pnt );

      TestVariables::drawImp->DrawPoint( &pnt, TRGB_BLACK );
      
      double fa = edge->FacesAngle( t );
      double fda = fa * M_RADDEG;

      TCHAR s[210];
      
      if ( edge->IsSeam() )
        _sntprintf( s, 210, RT_POINT_ANGLE_AND_SEAM, t, fda );
      else if ( edge->IsSplit() )
        _sntprintf( s, 210, RT_POINT_ANGLE_AND_LINE_OF_PARTITION, t, fda );
      else if ( edge->IsSmooth() )
        _sntprintf( s, 210, RT_POINT_ANGLE_AND_TRANSITION_LINE, t, fda );
      else if ( edge->IsPole() )
        _sntprintf( s, 210, RT_POINT_ANGLE_AND_DEGENERATE_EDGE, t, fda );
      else
        _sntprintf( s, 210, RT_POINT_AND_ANGLE, t, fda );

      MessageBoxEx( s, IDS_RESULTS );
      res = true;
    }
    else {
      MbSNameMaker snMaker;      
      MbSurfacesJointAnalysisResults results;

      MbSurfacesJointAnalysisParams::OperationMode operMode = MbSurfacesJointAnalysisParams::om_SurfacesNormalsDeviation;
      {
        int ns = 1;
        GetInt( IDS_ENTER_JOINT_ANALYSIS, ns );
        if ( ns != 0 )
          operMode = MbSurfacesJointAnalysisParams::om_SurfacesCrossCurvatureDeviation; // Поиск экстремальных отклонений кривизны поверхностей. Search for extreme deviations of surfaces curvatures.
      }

      if ( edge1 && edge2 && face1 && face2 ) {
        MbSurfacesJointAnalysisParams params( *edge1, *face1, *edge2, *face2, spaceAccuracy, snMaker );
        params.SetOperationMode( operMode );

        if ( intCurve == nullptr )
          intCurve = params.GetCurve();

        res = ::SurfacesJointMinMaxValues( params, results );
      }
      else if ( edge ) {
        MbSurfacesJointAnalysisParams params( *edge, snMaker );
        params.SetOperationMode( operMode );
        res = ::SurfacesJointMinMaxValues( params, results );
      }
      else if ( intCurve ) {
        MbSurfacesJointAnalysisParams params( *intCurve, snMaker );
        params.SetOperationMode( operMode );
        res = ::SurfacesJointMinMaxValues( params, results );
      }

      if ( res ) {
        bool showResults = false;

        const size_t bufLen = 1024;
        TCHAR * buf = new TCHAR[bufLen];

        size_t len = 0;

        if ( results.IsDefinedMinValue() ) {
          const MbSurfacesJointAnalysisResults::Data & minParamValue = results.minParamValue;
          t = minParamValue.t;
          double fv = minParamValue.v;

          if ( results.GetOperationMode() == MbSurfacesJointAnalysisParams::om_SurfacesNormalsDeviation ) {
            if ( ::fabs( fv ) > M_PI_2 )
              fv = M_PI - ::fabs( fv );
            double fda = fv * M_RADDEG;
            len += (size_t)_sntprintf( buf + len, bufLen - len, RT_MIN_ANGLE_AND_PARAM, fda, t );
          }
          else if ( results.GetOperationMode() == MbSurfacesJointAnalysisParams::om_SurfacesCrossCurvatureDeviation ) {
            len += (size_t)_sntprintf( buf + len, bufLen - len, RT_MIN_CURVATURE_CHAR_VALUE, fv );
            len += (size_t)_sntprintf( buf + len, bufLen - len, _T( " (t = %f)" ), t );
          }
          else {
            C3D_ASSERT_UNCONDITIONAL( false );
          }

          len += (size_t)_sntprintf( buf + len, bufLen - len, _T( "\n\n" ) );

          if ( edge != nullptr )
            edge->GetCurve()._PointOn( t, pnt );
          else if ( intCurve != nullptr )
            intCurve->_PointOn( t, pnt );
          else {
            C3D_ASSERT_UNCONDITIONAL( false );
          }

          SPtr<MbPoint3D> itemPnt( new MbPoint3D( pnt ) );
          Style style;
          style.SetColor( RGB( 0, 0, 255 ) );

          TestVariables::viewManager->AddObject( style, itemPnt );
          showResults = true;
        }

        if ( results.IsDefinedMaxValue() ) {
          const MbSurfacesJointAnalysisResults::Data & maxParamValue = results.maxParamValue;
          t = maxParamValue.t;
          double fv = maxParamValue.v;

          if ( results.GetOperationMode() == MbSurfacesJointAnalysisParams::om_SurfacesNormalsDeviation ) {
            if ( ::fabs( fv ) > M_PI_2 )
              fv = M_PI - ::fabs( fv );
            double fda = fv * M_RADDEG;
            len += (size_t)_sntprintf( buf + len, bufLen - len, RT_MAX_ANGLE_AND_PARAM, fda, t );
          }
          else if ( results.GetOperationMode() == MbSurfacesJointAnalysisParams::om_SurfacesCrossCurvatureDeviation ) {
            len += (size_t)_sntprintf( buf + len, bufLen - len, RT_MAX_CURVATURE_CHAR_VALUE, fv );
            len += (size_t)_sntprintf( buf + len, bufLen - len, _T(" (t = %f)"), t );
          }
          else {
            C3D_ASSERT_UNCONDITIONAL( false );
          }

          len += (size_t)_sntprintf( buf + len, bufLen - len, _T( "\n\n" ) );

          if ( edge != nullptr )
            edge->GetCurve()._PointOn( t, pnt );
          else
            intCurve->_PointOn( t, pnt );

          SPtr<MbPoint3D> itemPnt( new MbPoint3D( pnt ) );
          Style style;
          style.SetColor( RGB( 255, 0, 0 ) );

          TestVariables::viewManager->AddObject( style, itemPnt );
          showResults = true;
        }

        if ( showResults )
          MessageBoxEx( buf, IDS_RESULTS );

        delete[] buf;
        buf = nullptr;
      }
      else {
        MessageBoxEx( RT_ERROR_IN_PARAMS, IDS_ERROR );
      }
    }

    TestVariables::viewManager->ToggleObject( pathFrom.first );
  }     

  return res;
}


//------------------------------------------------------------------------------
// Угол между двумя ребрами. // Angle between two edges.
// ---
bool GetEdgeEdgeAngle() 
{
  MbItem * obj1 = nullptr;
  MbItem * obj2 = nullptr;
  MbTopItem * curve1 = nullptr;
  MbTopItem * curve2 = nullptr;

  if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj1, &curve1 ) ) {
    TestVariables::drawImp->DrawItem( curve1, TRGB_WHITE );

    if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj2, &curve2 ) ) {
      TestVariables::drawImp->DrawItem( curve2, TRGB_WHITE );

      if ( curve1 != nullptr && curve2 != nullptr && curve2 != curve1 ) {
        MbeTopologyType type1 = curve1->IsA();
        MbeTopologyType type2 = curve2->IsA();
        if ( (type1 == tt_Edge || type1 == tt_CurveEdge) &&
             (type2 == tt_Edge || type2 == tt_CurveEdge) ) {
          MbEdge * edge1 = static_cast<MbEdge *>( curve1 );
          MbEdge * edge2 = static_cast<MbEdge *>( curve2 );

          const MbCartPoint3D & pBeg1 = edge1->GetBegVertex().GetCartPoint();
          const MbCartPoint3D & pEnd1 = edge1->GetEndVertex().GetCartPoint();
          const MbCartPoint3D & pBeg2 = edge2->GetBegVertex().GetCartPoint();
          const MbCartPoint3D & pEnd2 = edge2->GetEndVertex().GetCartPoint();

          double dBeg1Beg2 = pBeg1.DistanceToPoint( pBeg2 );
          double dBeg1End2 = pBeg1.DistanceToPoint( pEnd2 );
          double dEnd1Beg2 = pEnd1.DistanceToPoint( pBeg2 );
          double dEnd1End2 = pEnd1.DistanceToPoint( pEnd2 );

          MbVector3D tau1, tau2;

          if ( dBeg1Beg2 <= dBeg1End2 && dBeg1Beg2 <= dEnd1Beg2 && dBeg1Beg2 <= dEnd1End2 ) {
            edge1->GetBegTangent( tau1 );
            edge2->GetBegTangent( tau2 );
          }
          if ( dBeg1End2 <= dBeg1Beg2 && dBeg1End2 <= dEnd1Beg2 && dBeg1End2 <= dEnd1End2 ) {
            edge1->GetBegTangent( tau1 );
            edge2->GetEndTangent( tau2 );
            tau2.Invert();
          }
          if ( dEnd1Beg2 <= dBeg1Beg2 && dEnd1Beg2 <= dBeg1End2 && dEnd1Beg2 <= dEnd1End2 ) {
            edge1->GetEndTangent( tau1 );
            edge2->GetBegTangent( tau2 );
            tau1.Invert();
          }
          if ( dEnd1End2 <= dBeg1Beg2 && dEnd1End2 <= dBeg1End2 && dEnd1End2 <= dEnd1Beg2 ) {
            edge1->GetEndTangent( tau1 );
            edge2->GetEndTangent( tau2 );
            tau1.Invert();
            tau2.Invert();
          }
      
          double fa = tau1.Angle( tau2 );
          TCHAR s[210];
          _sntprintf( s, 210, RT_EDGES_ANGLE, fa*180/M_PI );

          MessageBoxEx( s, IDS_RESULTS );

          return true;
        }
      }
      TestVariables::drawImp->DrawItem( curve2, TRGB_LIGHTGREEN );
    }
    TestVariables::drawImp->DrawItem( curve1, TRGB_LIGHTGREEN );
  }
  return false;
}


//------------------------------------------------------------------------------
// Минимальное расстояние между геометрическими объектами
// ---
bool GeomItemsMinDistance()
{
  bool bRes = false;

  TCHAR msgInfoTitle[] = IDS_TITLE_MSG;

  SPtr<MbSpaceItem> gitem1;
  SPtr<MbSpaceItem> gitem2;

  if ( gitem1 == nullptr || gitem2 == nullptr ) {
    MbItem * obj1 = nullptr;
    MbItem * obj2 = nullptr;

    TCHAR msgRightTypes[] = RT_SELECT_POINT_CURVE_OR_SURFACE;
    //TCHAR msgNoObject1[] = RT_NOT_SELECTED_FIRTS_OBJECT;
    TCHAR msgNoObject2[] = RT_NOT_SELECTED_SECOND_OBJECT;

    while ( !bRes && GetGObj( IDS_SELECT_GEOM_OBJECT, st_SpaceItem, &obj1 ) ) {
      if ( obj1->IsA() == st_SpaceInstance ) 
        gitem1 = ((MbSpaceInstance *)obj1)->SetSpaceItem(); // Дать трёхмерный геометрический объекта
      else if ( obj1->IsA() == st_PointFrame ) {
        MbPointFrame * pf = static_cast<MbPointFrame *>(obj1);
        if ( pf->GetVerticesCount() == 1 ) {
          MbCartPoint3D pnt;
          if ( pf->GetCartPoint( pnt ) )
            gitem1 = new MbPoint3D( pnt );
        }
      }
      else if ( obj1->IsA() == st_WireFrame ) {
        MbWireFrame * wf = static_cast<MbWireFrame *>(obj1);
        if ( wf->GetEdgesCount() == 1 ) {
          std::vector< SPtr<MbCurve3D> > wfCurves;
          wf->GetCurves( wfCurves );
          gitem1 = wfCurves[0];
        }
      }
      else if ( obj1->IsA() == st_PlaneInstance ) {
        SPtr<MbCurve3D> crv1;
        GetSpaceCurve( *obj1, true, crv1, nullptr );
        gitem1 = crv1;
      }

      if ( gitem1 != nullptr && ((gitem1->Family() == st_Point3D) || (gitem1->Family() == st_Curve3D) || (gitem1->Family() == st_Surface)) ) {
        TestVariables::viewManager->SelectObject(obj1);

        while ( !bRes && GetGObj( IDS_SELECT_GEOM_OBJECT, st_SpaceItem, &obj2 ) ) {
          if ( obj2->IsA() == st_SpaceInstance ) 
            gitem2 = ((MbSpaceInstance *)obj2)->SetSpaceItem(); // Дать трёхмерный геометрический объекта
          else if ( obj2->IsA() == st_PointFrame ) {
            MbPointFrame * pf = static_cast<MbPointFrame *>(obj2);
            if ( pf->GetVerticesCount() == 1 ) {
              MbCartPoint3D pnt;
              if ( pf->GetCartPoint( pnt ) )
                gitem2 = new MbPoint3D( pnt );
            }
          }
          else if ( obj2->IsA() == st_WireFrame ) {
            MbWireFrame * wf = static_cast<MbWireFrame *>(obj2);
            if ( wf->GetEdgesCount() == 1 ) {
              std::vector< SPtr<MbCurve3D> > wfCurves;
              wf->GetCurves( wfCurves );
              gitem2 = wfCurves[0];
            }
          }
          else if ( obj2->IsA() == st_PlaneInstance ) {
            SPtr<MbCurve3D> crv2;
            GetSpaceCurve( *obj2, true, crv2, nullptr );
            gitem2 = crv2;
          }

          if ( gitem1 != gitem2 ) {
            if ( gitem2 != nullptr && ((gitem2->Family() == st_Point3D) || (gitem2->Family() == st_Curve3D) || (gitem2->Family() == st_Surface)) ) {
              TestVariables::viewManager->SelectObject(obj2);
              bRes = true;
              TestVariables::viewManager->UnselectObject( obj1 );
              TestVariables::viewManager->UnselectObject( obj2 );
            }
            else
            {        
              MessageBoxEx( msgRightTypes, msgInfoTitle );
            }
          }

          if ( !bRes )
            TestVariables::viewManager->UnselectObject( obj1 );
        }

        if ( obj2 == nullptr )
        {    
          MessageBoxEx( msgNoObject2, msgInfoTitle );
        }    
      }
      else
      {  
        MessageBoxEx( msgRightTypes, msgInfoTitle );
      }  
    }

    if ( obj1 == nullptr )
    {      
      MessageBoxEx( msgNoObject2, msgInfoTitle );
    }      
  }

  if ( gitem1 != nullptr && ((gitem1->Family() == st_Point3D) || (gitem1->Family() == st_Curve3D) || (gitem1->Family() == st_Surface)) &&
       gitem2 != nullptr && ((gitem2->Family() == st_Point3D) || (gitem2->Family() == st_Curve3D) || (gitem2->Family() == st_Surface)) )
  {
    bRes = true;
    double dMin = UNDEFINED_DBL;
    MbCartPoint3D pnt1, pnt2;
    MbCurve3D * curve1 = nullptr;
    MbCurve3D * curve2 = nullptr;
    MbSurface * surface1 = nullptr;
    MbSurface * surface2 = nullptr;

    if ( gitem1->IsA() == st_Point3D )
      ((const MbPoint3D &)(*gitem1)).GetCartPoint( pnt1 );
    else if ( gitem1->Family() == st_Curve3D )
      curve1 = (MbCurve3D *)gitem1.get();
    else if ( gitem1->Family() == st_Surface )
      surface1 = (MbSurface *)gitem1.get();

    if ( gitem2->IsA() == st_Point3D )
      ((const MbPoint3D &)(*gitem2)).GetCartPoint( pnt2 );
    else if ( gitem2->Family() == st_Curve3D )
      curve2 = (MbCurve3D *)gitem2.get();
    else if ( gitem2->Family() == st_Surface )
      surface2 = (MbSurface *)gitem2.get();

    PRECONDITION( curve1 == nullptr || surface1 == nullptr );
    PRECONDITION( curve2 == nullptr || surface2 == nullptr );

    double t1, t2;
    MbCartPoint uv1, uv2;

    MbAccurateTimer ac;
    ac.Begin();

    if ( (curve1 == nullptr && surface1 == nullptr) &&
         (curve2 == nullptr && surface2 == nullptr) ) // Точка-точка
      dMin = pnt1.DistanceToPoint( pnt2 );
    else if ( (curve1 != nullptr) && (curve2 == nullptr && surface2 == nullptr) ) { // Точка-кривая
      curve1->NearPointProjection( pnt2, t1, false );
      curve1->PointOn( t1, pnt1 );
      dMin = pnt1.DistanceToPoint( pnt2 );
    }
    else if ( (curve2 != nullptr) && (curve1 == nullptr && surface1 == nullptr) ) { // Точка-кривая
      curve2->NearPointProjection( pnt1, t2, false );
      curve2->PointOn( t2, pnt2 );
      dMin = pnt1.DistanceToPoint( pnt2 );
    }
    else if ( (curve1 != nullptr) && (curve2 != nullptr) ) { // Кривая-кривая
      double tt1, tt2;
      ::NearestPoints( *curve1, false, *curve2, false, tt1, tt2, dMin );
      curve1->PointOn( tt1, pnt1 );
      curve2->PointOn( tt2, pnt2 );
    }
    else if ( (surface1 != nullptr) && (surface2 == nullptr && curve2 == nullptr) ) { // Точка-поверхность
      surface1->NearPointProjection( pnt2, uv1.x, uv1.y, false );
      surface1->PointOn( uv1, pnt1 );
      dMin = pnt1.DistanceToPoint( pnt2 );
    }
    else if ( (surface2 != nullptr) && (surface1 == nullptr && curve1 == nullptr) ) { // Точка-поверхность
      surface2->NearPointProjection( pnt1, uv2.x, uv2.y, false );
      surface2->PointOn( uv2, pnt2 );
      dMin = pnt1.DistanceToPoint( pnt2 );
    }
    else if ( (curve1 != nullptr) && (surface2 != nullptr) ) { // Кривая-поверхность
      ::NearestPoints( *surface2, false, *curve1, false, uv2.x, uv2.y, t1, dMin );
      surface2->PointOn( uv2, pnt2 );
      curve1->PointOn( t1, pnt1 );
    }
    else if ( (curve2 != nullptr) && (surface1 != nullptr) ) { // Кривая-поверхность
      ::NearestPoints( *surface1, false, *curve2, false, uv1.x, uv1.y, t2, dMin );
      surface1->PointOn( uv1, pnt1 );
      curve2->PointOn( t2, pnt2 );
    }
    else if ( (surface1 != nullptr) && (surface2 != nullptr) ) { // Поверхность-поверхность
      ::NearestPoints( *surface1, false, *surface2, false, uv1.x, uv1.y, uv2.x, uv2.y, dMin, true );
      surface1->PointOn( uv1, pnt1 );
      surface2->PointOn( uv2, pnt2 );
    }

    ac.End();

    if ( dMin != UNDEFINED_DBL ) {
      const size_t bufLen = 1024;
      TCHAR buf[bufLen];
      size_t len = 0;
      len += (size_t)_sntprintf( buf + len, bufLen - len, RT_MIN_DISTANCE, dMin );

      if ( TestVariables::checkOperationTime ) {
        len += ( size_t ) _sntprintf( buf + len, bufLen - len, _T( "\n\n" ) );
        len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
      }
      ::MessageBoxEx( buf, IDS_RESULTS );

      TestVariables::drawImp->DrawPoint( &pnt1, TRGB_RED );
      TestVariables::drawImp->DrawPoint( &pnt2, TRGB_BLUE );

      PRECONDITION( ::fabs(pnt1.DistanceToPoint( pnt2 ) - dMin) < METRIC_EPSILON );

      if ( dMin > METRIC_PRECISION ) {
        SPtr<MbLineSegment3D> seg( new MbLineSegment3D( pnt1, pnt2 ) );
        TestVariables::drawImp->DrawItem( seg, TRGB_BROWN );
      }
    }
    else {
      ::MessageBoxEx( RT_DISTANCE_IS_NOT_FOUND, msgInfoTitle );
    }
  }

  return bRes;
}


//------------------------------------------------------------------------------
// Преобразовать топ. объект в вершину, ребро или грань
// ---
inline
bool ConvertTopoItem( const MbTopItem &         titem,
                      const MbMatrix3D &        from,
                      SPtr<const MbVertex> &    vert,
                      SPtr<const MbCurveEdge> & edge,
                      SPtr<const MbFace> &      face )
{
  bool isDone = false;

  if ( titem.IsAVertex() ) {
    vert = static_cast<const MbVertex *>(&titem);
    if ( !from.IsSingle() ) {
      MbVertex * vertCopy = vert->DataDuplicate();
      vertCopy->Transform( from );
      vert = vertCopy;
    }
    isDone = true;
  }
  else if ( titem.IsAnEdge() ) {
    edge = static_cast<const MbCurveEdge *>(&titem);
    if ( !from.IsSingle() ) {
      MbCurveEdge * edgeCopy = edge->DataDuplicate();
      edgeCopy->Transform( from );
      edge = edgeCopy;
    }
    isDone = true;
  }
  else if ( titem.IsAFace() ) {
    face = static_cast<const MbFace *>(&titem);
    if ( !from.IsSingle() ) {
      MbFace * faceCopy = face->DataDuplicate();
      faceCopy->Transform( from );
      face = faceCopy;
    }
    isDone = true;
  }

  return isDone;
}


//------------------------------------------------------------------------------
// Минимальное расстояние между топологическими объектами
// ---
bool TopoItemsMinDistance()
{
  bool isDone = true;

  while ( isDone ) {
    isDone = false;

    MbItem * obj1 = nullptr;
    MbItem * obj2 = nullptr;
    MbTopItem * titem1 = nullptr;
    MbTopItem * titem2 = nullptr;
    MbMatrix3D from1, from2;

    if ( GetTObj( IDS_SELECT_TOPO_OBJECT, st_Solid, tt_TopItem, &obj1, &titem1, from1 ) && (titem1 != nullptr) ) {
      titem1->AddRef();
      TestVariables::drawImp->DrawItem( titem1, TRGB_WHITE );

      ConstVertexSPtr vert1;
      ConstEdgeSPtr   edge1;
      ConstFaceSPtr   face1;

      if ( ::ConvertTopoItem( *titem1, from1, vert1, edge1, face1 ) ) {
        if ( GetTObj( IDS_SELECT_TOPO_OBJECT, st_Solid, tt_TopItem, &obj2, &titem2, from2 ) && (titem1 != titem2) && (titem2 != nullptr) ) {
          titem2->AddRef();
          TestVariables::drawImp->DrawItem( titem2, TRGB_WHITE );

          ConstVertexSPtr vert2;
          ConstEdgeSPtr   edge2;
          ConstFaceSPtr   face2;

          MbAccurateTimer ac;

          if ( ::ConvertTopoItem( *titem2, from2, vert2, edge2, face2 ) ) {
            double d = MB_MAXDOUBLE;
            MbCartPoint3D p1, p2;

            ac.Begin();

            if ( face1 != nullptr && face2 != nullptr ) { // Две грани
              d = face1->DistanceToFace( *face2, p1, p2 );
              isDone = true;
            }
            else if ( edge1 != nullptr && edge2 != nullptr ) { // Два ребра
              d = edge1->DistanceToEdge( *edge2, p1, p2 );
              isDone = true;
            }
            else if ( vert1 != nullptr && vert2 != nullptr ) { // Две вершины
              d = vert1->DistanceToPoint( vert2->GetCartPoint() );
              isDone = true;
            }
            else if ( edge1 != nullptr && face2 != nullptr ) { // Ребро и грань
              d = edge1->DistanceToFace( *face2, p1, p2 );
              isDone = true;
            }
            else if ( edge2 != nullptr && face1 != nullptr ) { // Ребро и грань
              d = edge2->DistanceToFace( *face1, p1, p2 );
              isDone = true;
            }
            else if ( vert1 != nullptr && face2 != nullptr ) { // Вершина и грань
              d = vert1->DistanceToFace( *face2, p1, p2 );
              isDone = true;
            }
            else if ( vert2 != nullptr && face1 != nullptr ) { // Вершина и грань
              d = vert2->DistanceToFace( *face1, p1, p2 );
              isDone = true;
            }
            else if ( vert1 != nullptr && edge2 != nullptr ) { // Вершина и ребро
              d = vert1->DistanceToEdge( *edge2, p1, p2 );
              isDone = true;
            }
            else if ( vert2 != nullptr && edge1 != nullptr ) { // Вершина и ребро
              d = vert2->DistanceToEdge( *edge1, p1, p2 );
              isDone = true;
            }
            else {
              PRECONDITION( isDone );
            }

            ac.End();

            if ( isDone ) {
              const size_t bufLen = 1024;
              TCHAR buf[bufLen];
              size_t len = 0;
              len += (size_t)_sntprintf( buf + len, bufLen - len, RT_DISTANCE, d );

              if ( TestVariables::checkOperationTime ) {
                len += (size_t)_sntprintf( buf + len, bufLen - len, _T( "\n\n" ) );
                len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
              }
              ::MessageBoxEx( buf, IDS_RESULTS );

              TestVariables::drawImp->DrawPoint( &p1, TRGB_RED );
              TestVariables::drawImp->DrawPoint( &p2, TRGB_BLUE );

              PRECONDITION( ::fabs( p1.DistanceToPoint( p2 ) - d ) < METRIC_EPSILON );

              if ( d > METRIC_PRECISION ) {
                SPtr<MbLineSegment3D> seg( new MbLineSegment3D( p1, p2 ) );
                TestVariables::drawImp->DrawItem( seg, TRGB_BROWN );
              }
            }
            else {
              ::MessageBoxEx( RT_ERROR_RESULT_NOT_OBTAINED, IDS_TITLE_MSG );
            }
          }

          titem2->DecRef();
        }
      }

      titem1->DecRef();
    }
  }

  return isDone;
}

//------------------------------------------------------------------------------
// Минимальное расстояние между телами
// ---
bool SolidsMinDistance() 
{
  bool isDone = false;
  MbItem * item1 = nullptr, * item2 = nullptr;
  PathMatrix pathFrom1, pathFrom2;

  if ( GetGObj(IDS_SELECT_FIRST_SOLID, st_Solid, &item1, pathFrom1) ) {
    SPtr<MbSolid> solid1;
    if ( item1->IsA() == st_Solid ) {
      solid1 = static_cast< MbSolid * >( item1 );
      if ( !pathFrom1.second.IsSingle() ) {
        MbSolid * result1 = nullptr;
        TransformValues tv( pathFrom1.second );
        MbSNameMaker tn;
        tn.SetMainName( ct_TransformedSolid );
        ::TransformedSolid( *solid1, cm_Copy, tv, tn, result1 );
        if ( result1 != nullptr ) {
          solid1 = result1;
        }
      }
    }
    TestVariables::viewManager->ToggleObject( item1 );

    if ( GetGObj(IDS_SELECT_SECOND_SOLID, st_Solid, &item2, pathFrom2) ) {
      SPtr<MbSolid> solid2;
      if ( item2->IsA() == st_Solid ) {
        solid2 = static_cast< MbSolid * >( item2 );
        if ( !pathFrom2.second.IsSingle() ) {
          MbSolid * result2 = nullptr;
          TransformValues tv( pathFrom2.second );
          MbSNameMaker tn;
          tn.SetMainName( ct_TransformedSolid );
          ::TransformedSolid( *solid2, cm_Copy, tv, tn, result2 );
          if ( result2 != nullptr ) {
            solid2 = result2;
          }
        }
      }
      TestVariables::viewManager->ToggleObject( item2 );

      if ( (solid1 != nullptr) && (solid2 != nullptr) ) {
        MbAccurateTimer ac;
        std::vector<MbShellsDistanceData> shellsDistanceData;
        double eps = Math::metricPrecision;
        double lowerLimit = eps;
        GetDouble( IDS_ENTER_LOWERLIMIT, lowerLimit );
        int tillFirst = 1;
        GetInt( IDS_TILL_FIRST, tillFirst );

        SetWaitCursor( true );
        ac.Begin();
        isDone = solid1->GetShell()->DistanceToShell( *solid2->GetShell(), lowerLimit, tillFirst == 1, eps, shellsDistanceData );
        ac.End();
        SetWaitCursor( false );

        if ( isDone ) {
          for ( size_t i = 0, iSize = shellsDistanceData.size(); i < iSize; i++ ) {
            double d = shellsDistanceData[i].GetMinDistanse();
            MbCartPoint3D p1, p2;
            const MbFace * face1 = solid1->GetShell()->GetFace( shellsDistanceData[i].GetFaceIndex(1) );
            const MbSurface * srf1 = &face1->GetSurface();
            MbCartPoint pnt1 = shellsDistanceData[i].GetPoint( 1 );
            srf1->_PointOn( pnt1.x, pnt1.y, p1 );

            const MbFace * face2 = solid2->GetShell()->GetFace( shellsDistanceData[i].GetFaceIndex(2) );
            const MbSurface * srf2 = &face2->GetSurface();
            MbCartPoint pnt2 = shellsDistanceData[i].GetPoint( 2 );
            srf2->_PointOn( pnt2.x, pnt2.y, p2 );

            const size_t bufLen = 1024;
            TCHAR buf[bufLen];
            size_t len = 0;
            len += (size_t)_sntprintf( buf + len, bufLen - len, RT_DISTANCE, d );

            if ( TestVariables::checkOperationTime ) {
              len += (size_t)_sntprintf( buf + len, bufLen - len, _T( "\n\n" ) );
              len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
            }
            ::MessageBoxEx( buf, IDS_RESULTS );

            TestVariables::drawImp->DrawPoint( &p1, TRGB_RED );
            TestVariables::drawImp->DrawPoint( &p2, TRGB_BLUE );

            PRECONDITION( ::fabs(p1.DistanceToPoint(p2) - d) < METRIC_EPSILON );

            if ( d > METRIC_PRECISION ) {
              SPtr<MbLineSegment3D> seg( new MbLineSegment3D(p1, p2) );
              TestVariables::drawImp->DrawItem( seg, TRGB_BROWN );
            }
          }
        }
        else {
          ::MessageBoxEx( RT_ERROR_RESULT_NOT_OBTAINED, IDS_TITLE_MSG );
        }
      }
    }
  }

  return isDone;
}

//------------------------------------------------------------------------------
// Вычислить длину кривой.
// ---
bool CurveLength()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    double d = -1.0;
    uint32 miliSec0(0), miliSec1(0);

    if ( obj->IsA() == st_SpaceInstance ) {
      const MbSpaceInstance * sInst = static_cast<MbSpaceInstance *>(obj);
      const MbSpaceItem * sItem = sInst->GetSpaceItem();
      if ( sItem->Family() == st_Curve3D ) {
        const MbCurve3D * curve = static_cast<const MbCurve3D *>(sItem);
        miliSec0 = ::GetTickCountEx();
        d = curve->CalculateMetricLength();
        miliSec1 = ::GetTickCountEx();
        //KOMPAS-33643 double tMin = curve->GetTMin();
        //KOMPAS-33643 double tMax = curve->GetTMax();
        //KOMPAS-33643 double t1 = tMin, t2 = tMax;
        //KOMPAS-33643 double d1 = curve->CalculateLength( tMin, tMax );
        //KOMPAS-33643 curve->DistanceAlong( t1, d,  1 );
        //KOMPAS-33643 curve->DistanceAlong( t2, d, -1 );
        //KOMPAS-33643 double w1 = tMin, w2 = tMax;
        //KOMPAS-33643 double d2 = curve->MbCurve3D::CalculateLength( tMin, tMax );
        //KOMPAS-33643 curve->MbCurve3D::DistanceAlong( w1, d,  1 );
        //KOMPAS-33643 curve->MbCurve3D::DistanceAlong( w2, d, -1 );
        //KOMPAS-33643 if ( ::fabs(d1 - d2) > 0.0 ) 
        //KOMPAS-33643   d1 = d2;
      }
    }
    else if ( obj->IsA() == st_PlaneInstance ) {
      const MbPlaneInstance * pInst = static_cast<MbPlaneInstance *>(obj);
      const MbPlaneItem * pItem = pInst->GetPlaneItem();
      if ( pItem->Family() == pt_Curve ) {
        const MbCurve * curve = static_cast<const MbCurve *>(pItem);
        miliSec0 = ::GetTickCountEx();
        d = curve->CalculateMetricLength();
        miliSec1 = ::GetTickCountEx();
        //KOMPAS-33643 double tMin = curve->GetTMin();
        //KOMPAS-33643 double tMax = curve->GetTMax();
        //KOMPAS-33643 double t1 = tMin, t2 = tMax;
        //KOMPAS-33643 double d1 = curve->CalculateLength( tMin, tMax );
        //KOMPAS-33643 curve->DistanceAlong( t1, d,  1 );
        //KOMPAS-33643 curve->DistanceAlong( t2, d, -1 );
        //KOMPAS-33643 double w1 = tMin, w2 = tMax;
        //KOMPAS-33643 double d2 = curve->MbCurve::CalculateLength( tMin, tMax );
        //KOMPAS-33643 curve->MbCurve::DistanceAlong( w1, d,  1 );
        //KOMPAS-33643 curve->MbCurve::DistanceAlong( w2, d, -1 );
        //KOMPAS-33643 if ( ::fabs(d1 - d2) > 0.0 ) 
        //KOMPAS-33643   d1 = d2;
      }
    }

    TestVariables::viewManager->ToggleObject(obj);

    miliSec1 = miliSec1 - miliSec0;

    if ( d >= 0.0 ) {
      TCHAR s[110];
      _sntprintf( s, 110, RT_CURVE_LENGTH_THE_ARC, d );
      MessageBoxEx( s, IDS_RESULTS );
    }

    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// Вычислить длину каркаса.
// ---
bool WireFrameLength()
{
  MbItem * wireFrameItem = nullptr;

  if ( GetGObj(IDS_SELECT_WIREFRAME, st_SpaceItem, &wireFrameItem) && (wireFrameItem != nullptr) ) {
    if ( wireFrameItem->IsA() == st_WireFrame ) {
      MbWireFrame * wireFrame = static_cast<MbWireFrame *>( wireFrameItem );

      double sumLength( 0.0 ); // Общая длина каркаса. // Total length of the wire frame.
      for ( size_t i = wireFrame->GetEdgesCount(); i-- > 0; ) {
        const MbEdge * edge = wireFrame->GetEdge( i );
        if( edge != nullptr )
          sumLength += edge->GetMetricLength();
      }

      TCHAR s[110];
      _sntprintf( s, 110, RT_CURVE_LENGTH_THE_ARC, sumLength );
      MessageBoxEx( s, IDS_RESULTS );
      
      return true;
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Вычислить радиус кривизны кривой
// ---
bool CurveRadius()
{
  MbItem * obj = nullptr;
  SPtr<MbCurve3D> curve;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) && GetSpaceCurve( *obj, true, curve ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    MbCartPoint3D point;

    if ( GetPoint( IDS_SELECT_POINT, point ) ) {
      double t;
      curve->NearPointProjection( point, t, false );
      double d, k = curve->Curvature( t );
      if ( k != 0 ) d = 1.0 / k;
      else          d = MB_MAXDOUBLE;

      TestVariables::viewManager->ToggleObject(obj);

      TCHAR s[110];
      _sntprintf( s, 110, RT_CURVATURE_OF_CURVE, d, k );

      MessageBoxEx( s, IDS_RESULTS );

      return true;
    }
  }
  else {
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    if ( wind != nullptr ) {
      MbTopItem * item( nullptr );
      MbCurveEdge * edge( nullptr );
      MbMatrix3D matr;

      if ( wind->FindNearestItem(st_Solid, tt_CurveEdge, TestVariables::cursorAxis, SENSE_RADIUS, &obj, &item, matr) &&
           (item != nullptr) && (item->IsA() == tt_CurveEdge)) {

        TestVariables::viewManager->ToggleObject( obj );

        edge = static_cast<MbCurveEdge *>( item );
        const MbCurve3D & intCurve = edge->GetCurve();
        double w = 0.5;
        if ( GetDouble( IDS_ENTER_PAR_ON_CURVE, w ) ) {
          double tMin = intCurve.GetTMin(), tMax = intCurve.GetTMax();
          double t = edge->IsSameSense() ? ( (1.0 - w) * tMin + w * tMax ) : 
                                           ( (1.0 - w) * tMax + w * tMin );
          TestVariables::viewManager->ToggleObject(obj);
          MbCartPoint3D p;
          intCurve._PointOn( t, p );

#ifdef _DRAWGI
          TestVariables::drawImp->DrawPoint( &p, TRGB_AZURE );
#endif

          double d, k = intCurve.Curvature( t );
          if ( k != 0 ) d = 1.0 / k;
          else          d = MB_MAXDOUBLE;

          TCHAR s[110];
          _sntprintf( s, 110, RT_CURVATURE_OF_CURVE, d, k );

          MessageBoxEx(s, IDS_RESULTS);

          return true;
        }
      }
    }
  }
  return false;
}


//------------------------------------------------------------------------------
// Изменилось ли что-нибудь в теле? Has it changes?
// ---
bool Dirty( const MbSolid * solid )
{
  if ( solid != nullptr )
  {
    RPArray<MbTopologyItem> items;
    solid->GetItems( items );
    for ( size_t i = 0, iCount = items.size(); i < iCount; i++ ) {
      if ( items[i]->GetOwnChanged() )
        return true;
    }
  }
  return false;
}


//------------------------------------------------------------------------------
// Установить флаг неизменности. Set unchanged flag.
// ---
void Clear( const MbSolid * solid )
{
  if ( solid != nullptr )
  {
    RPArray<MbTopologyItem> items;
    solid->GetItems( items );
    for ( size_t i = 0, iCount = items.size(); i < iCount; i++ ) {
      items[i]->SetOwnChanged( tct_Unchanged );
    }
  }
}


//----------------------------------------------------------------------------------------
// Поиск до первого найденного элемента
//---
template<class ItemPred>
struct MbFindFirstItem: public ItModelVisitor
{  
  const MbItem *                result;
  std::pair<MbPath,MbMatrix3D>  pathMatrix;
  
  MbFindFirstItem() : result( nullptr ), pathMatrix() {}

public:
  bool ExamineSubItem( const MbRefItem *, const MbItem * )
  {
    return result == nullptr;
  }

  void VisitItem( const MbItem * item )
  {
    MbMatrix3D lcsM; // The matrix of the item in a local space.
    item->GetMatrixFrom( lcsM );
    pathMatrix.first.push_back( item->GetItemName() );
    if ( !lcsM.IsSingle() )
    {
      pathMatrix.second = lcsM.Transform( pathMatrix.second );
    }
    ItemPred pred;
    if ( pred(item) )
      result = item;
  }
  void FinishItem( const MbItem * ) {}
  void ExamineInstance( const MbInstance *, const MbItem * ) {}
  OBVIOUS_PRIVATE_COPY( MbFindFirstItem );
};

//----------------------------------------------------------------------------------------
//
//---
struct _IsASolid
{
  bool operator()( const MbItem * item ) 
  { 
    return isa_cast<const MbSolid*>(item) != nullptr ;
  }
};

//----------------------------------------------------------------------------------------
// Number of nonempty parts of the model. 
// Returns no more than 3.
//---
template<class Model>
size_t _NumParts( const Model & model )
{
  /*
    Тут бы уточнить подсчет не любых, а именно непустых сборок или объектов.
    Тут непустой значит имеет хотя бы одно тело или сетку.
  */
  size_t nonEmptyParts = 0;
  for ( size_t itemIdx = 0; itemIdx<model.ItemsCount(); ++itemIdx )
  {
    MbFindFirstItem<_IsASolid> _findSolid;
    Traverse( model.GetItem(itemIdx), _findSolid );
    if ( _findSolid.result != nullptr )
    {
      ++nonEmptyParts;
    }
    if ( nonEmptyParts == 3 )
      break;  // it's enough.
  }

  return nonEmptyParts;
}

//----------------------------------------------------------------------------------------
// Select a part of the model, one of the main subassemblies, instancies etc...
// Returns a rest of the part that you need to cut off.
/*
  pathRng - The route leading from the one of subitems of the model into the depth of the 
            model graph, up to the final node selected by the user.
  returns the sub-route from the separated subassembly to the final node.
*/
//---
template<class Assembly, class PathIter>
c3d::range<PathIter> SelectAPart( const Assembly & model, c3d::range<PathIter> pathRng )
{
  if ( pathRng.empty() )
  {
    return pathRng;
  }
  /*
    Descent into the subitem recursively until the fork is founded. 
    The fork is a model or an assembly having two or more nonempty subassemblies.
    The goal is to find a sub-route (path) from the fork to a final node.
  */
  if ( const MbItem * subItem = model.SubItem(pathRng.front()) )
  {    
    ++pathRng.first;
    if ( _NumParts(model) == 1 )
    {      
      switch ( subItem->IsA() )
      {
        case st_Assembly:
        {
          const MbAssembly & subAsm = static_cast<const MbAssembly&>( *subItem );
          return SelectAPart( subAsm, pathRng );
        }
        case st_Instance:
        case st_Solid:
          return pathRng;
        default: break;
      }
    }
    else if ( _NumParts(model) > 1 )
    {
      switch ( subItem->IsA() )
      {
        case st_Assembly:
        case st_Instance:
        case st_Solid:
          return pathRng;
        default: break;
      }
    }
  }   
  MessageBoxEx( _T("The model should be have two or more subitems."), IDS_TITLE_WRN );
  return pathRng;
}
  
//----------------------------------------------------------------------------------------
// Select a part of the model, one of the main subassemblies, instancies etc...
//---
const MbItem * SelectAPart( const TCHAR * nStr, PathMatrix & path )
{
  MbItem * obj = nullptr;
  if ( GetObj(nStr, &obj, path) )
  {
    if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() )
    {
      const MbModel & model = wind->GetGeomModel();
      c3d::range<const SimpleName*> pathRng( path.first.begin(), path.first.end() );
      c3d::range<const SimpleName*> subPath = SelectAPart( model, pathRng );
      // Cut-off a final sub-path to select the separated subassembly.
      path.first.erase( subPath.begin(), subPath.end() );
      if ( path.first.empty() )
      {
        MessageBoxEx( _T( "The model should be have two or more subitems." ), IDS_TITLE_WRN );
        return nullptr;
      }
      return model.GetItemByPath(path.first, path.second );
    }
  }

  return nullptr;
}

//----------------------------------------------------------------------------------------
/** \brief  \ru Структура запроса для поиска граней столкновения.
            \en The structure of the query to find collision faces.
*/
//---
struct collision_faces: public cdet_query
{ 
  typedef std::pair<cdet_app_item,const MbRefItem*> item_face;  // represents a face of app item
  typedef std::set<item_face> collided_faces;
  collided_faces              faces;
  cdet_app_item               firstChecked, secondChecked;  // A pair to check collisions.
  size_t                      facePairsCount;

public:
  collision_faces()
    : cdet_query( _QueryFunc )
    , faces()
    , firstChecked( CDET_APP_NULL )
    , secondChecked( CDET_APP_NULL )
    , facePairsCount(0)
  {}  

  /** \brief  \ru Сбросить данные в исходное состояние.
              \en Set the internal data to the default state.
  */
  void Reset()
  {
    faces.clear();
    facePairsCount = 0;
  }

private:
  static cback_res _QueryFunc( cdet_query * query, message code, cback_data & cData )
  {
    if ( collision_faces * q = static_cast<collision_faces*>(query) )
    {
      switch( code )
      {      
      case CDET_QUERY_STARTED:
      {
        q->Reset();
        return CBACK_VOID;
      }
      case CDET_STARTED:
      {
        if ( q->firstChecked != CDET_APP_NULL && q->secondChecked != CDET_APP_NULL )
        {
          if ( cData.first.appItem == q->firstChecked && cData.second.appItem == q->secondChecked )
          {
            return CBACK_VOID;
          }
          if ( cData.first.appItem == q->secondChecked && cData.second.appItem == q->firstChecked )
          {
            return CBACK_VOID;
          }
          return CBACK_SKIP;
        }

        return CBACK_VOID;
      }

      case CDET_FINISHED: // a pair of solids was finished.
        return CBACK_SEARCH_MORE;

      case CDET_INTERSECTED:
      { 
        ++q->facePairsCount;
        if ( cData.first.refItem )
        {
          TestVariables::drawImp->DrawItem( cData.first.refItem, TRGB_ORANGE, *cData.first.wMatrix );
          q->faces.insert( item_face(cData.first.appItem,cData.first.refItem) );
        }
        if ( cData.second.refItem )
        {
          TestVariables::drawImp->DrawItem( cData.second.refItem, TRGB_ORANGE, *cData.second.wMatrix );
          q->faces.insert( item_face(cData.second.appItem,cData.second.refItem) );
        }
        return CBACK_SEARCH_MORE;
      }            
      default:
        return CBACK_VOID;
      }
    }
    return CBACK_VOID;
  }

  OBVIOUS_PRIVATE_COPY( collision_faces );
};

//----------------------------------------------------------------------------------------
// Traversing the model subtree to assembly the collision detection component.
//---
class CDetAssembler: protected ItModelVisitor
{
  typedef std::pair<const MbItem*, MbMatrix3D> stack_elem;
  typedef std::map<const MbSolid*, cdet_item>  solids_map;
  CdCollisionDetection & cDet;   // Detector of collisions and proximity computations.  
  mutable solids_map            solids; // Registered solids.
  std::vector<stack_elem>       stack;  // Stack of transformations.
  cdet_item                     m_comp; // The assembling component in the traversing process.

public:
  CDetAssembler( CdCollisionDetection & _cDet )
    : cDet( _cDet )
    , solids()
    , stack()
    , m_comp( CDET_NULL )
  {}

public:  
  // Register new solid item or return existing item of the solid.
  cdet_item CdSolid( const MbSolid * sol ) const 
  { 
    if ( sol->GetFacesCount() < 2 ) // Exclude the one-face shells.
      return CDET_NULL;

    solids_map::const_iterator sIter = solids.find( sol );
    if ( sIter != solids.end() )
    {
      return sIter->second;
    }    
    return solids[sol] = cDet.AddItem( *sol, MbPlacement3D::global );
  }
  
  /*
    Add the model part into the collision detection tool.
    Добавить часть модели в аппарат контроля соударений.
  */
  // Create new component item and its subtree of solids (assembly).
  cdet_item AddComponent( const MbItem * item, const MbPlacement3D & pl )
  {
    C3D_ASSERT( m_comp == nullptr );
    cdet_item comp = m_comp = cDet.AddComponent( (cdet_app_item)item );
    stack.push_back( std::make_pair(item, pl.GetMatrixFrom()) );
    Traverse( item, *this );
    stack.pop_back();
    m_comp = nullptr;
    return comp;
  }

private: // Inherited from ItModelVisitor, the calls to process the assembly process.
  bool ExamineSubItem( const MbRefItem * /*owner*/, const MbItem * /*subItem*/ ) 
  { 
    return true; 
  }

  void VisitItem( const MbItem * item )
  {
    MbPlacement3D place;
    item->GetPlacement( place );
    C3D_ASSERT( !stack.empty() );
    place.Transform( stack.back().second ); // transform to the global space.    
    stack.push_back( std::make_pair(item,place.GetMatrixFrom()) );
    if ( const MbSolid * sol = isa_cast<const MbSolid*>(item) )
    {
      cDet.AddInstance( m_comp, CdSolid(sol), stack.back().second );
    }
  }

  void FinishItem( const MbItem * item )
  {
    C3D_UNUSED_PARAMETER( item );
    C3D_ASSERT( stack.back().first == item );
    stack.pop_back();
  }

  void ExamineInstance( const MbInstance * inst, const MbItem * srcItem )
  {
    C3D_UNUSED_PARAMETER( inst );
    C3D_UNUSED_PARAMETER( srcItem );
    C3D_ASSERT( stack.back().first == inst );
  }

  OBVIOUS_PRIVATE_COPY( CDetAssembler )
};

//------------------------------------------------------------------------------
// 
//---
static SPtr<const MbMesh> _CalculateMesh( const MbItem* item )
{
  if ( item == nullptr )
  {
    return nullptr;
  }

  SPtr<const MbMesh> trueMesh( isa_cast<const MbMesh*>(item) );
  if ( trueMesh )
  {
    return trueMesh;
  }

  SPtr<MbMesh> mesh( new MbMesh );
  item->CalculateMesh( MbStepData(ist_CollisionStep, Math::visualSag), MbFormNote(), *mesh );

  return mesh;
}

//------------------------------------------------------------------------------
// Function declaration for adding an object to the collision detector.
//---
typedef cdet_item(*add2det_fn)( CdCollisionDetection &, SPtr<const MbItem>&, const PathMatrix& );

//------------------------------------------------------------------------------
// Add mesh to the detector.
//---
cdet_item AddMesh( CdCollisionDetection & cdet, SPtr<const MbItem>& obj, const PathMatrix& pathFrom )
{
  SPtr<const MbMesh> mesh( _CalculateMesh(obj) );
  if ( mesh )
  {
    return cdet.AddMesh( *mesh, pathFrom.second, (void*)obj.get() );
  }

  return  CDET_NULL;
}

//------------------------------------------------------------------------------
// Add solid to the detector.
//---
cdet_item AddSolid( CdCollisionDetection & cdet, SPtr<const MbItem>& obj, const PathMatrix& pathFrom )
{
  SPtr<const MbSolid> solid( isa_cast<const MbSolid*>(obj) );
  if ( solid )
  {
    return cdet.AddItem( *solid, pathFrom.second, (void*)obj.get() );
  }

  return  CDET_NULL;
}

//------------------------------------------------------------------------------
// Расчет параметров близости двух подсборок.
// Calculate the proximity parameters between two assemblies.
//---
void CollisionDistanceQuery()
{
  PathMatrix pathFrom1, pathFrom2;
  if ( const MbItem * obj1 = SelectAPart( IDS_SELECT_FIRST_SOLID, pathFrom1 ) )
  {
    TestVariables::viewManager->ToggleObject( obj1 );
    if ( const MbItem * obj2 = SelectAPart( IDS_SELECT_SECOND_SOLID, pathFrom2 ) )
    {
      TestVariables::viewManager->ToggleObject( obj2 );

      TimeTest * testResult = GetTimeTestResults();
      uint miliSec = GetTickCountEx();

      CdCollisionDetection cdet;
      CDetAssembler cDetAssm( cdet );

      const MbPlacement3D pl1( pathFrom1.second );
      const MbPlacement3D pl2( pathFrom2.second );
      collision_faces colQuery;
      colQuery.firstChecked = (cdet_app_item)obj1;
      colQuery.secondChecked = (cdet_app_item)obj2;

      cdet_item cdItem1 = cDetAssm.AddComponent( obj1, pl1 );
      cdet_item cdItem2 = cDetAssm.AddComponent( obj2, pl2 );

      BEGIN_TIME_REL( _T("ComputeBVTree"), _T("CollisionDetection") );
      cdet._ComputeBVTree( cdItem1 );
      cdet._ComputeBVTree( cdItem2 );
      END_TIME_REL( _T("ComputeBVTree") );

      if ( (cdItem1 == CDET_NULL) || (cdItem2 == CDET_NULL) )
      {
        MessageBoxEx( IDS_TITLE_ERR, _T("A component wasn't registered.") );
      }
      BEGIN_TIME_REL( _T("CheckCollisions"), _T("CollisionDetection") );
      cdet_result res = cdet.CheckCollisions( colQuery );
      END_TIME_REL( _T("CheckCollisions") );

      if ( res == CDM_RESULT_NoCollisionDetected )
      {
        BEGIN_TIME_REL( _T("DistanceQuery"), _T("CollisionDetection") );
        MbProximityParameters dPars;
        cdet.SetApproxDistanceComputation( true );
        cdet.DistanceQuery( cdItem1, cdItem2, dPars );
        TestVariables::drawImp->DrawLine( dPars.fstPnt, dPars.sndPnt, TRGB_ORANGE );
        END_TIME_REL( _T("DistanceQuery") );
      }
      ShowTimeTestResult( testResult, miliSec, IDS_COLLISION_DETECTION );
      SET_TIME_TEST_REL( false );
    }
  }
}

//------------------------------------------------------------------------------
// Тест соударений для чистых полигональных объектов.
// Calculate collisions between a pair of meshes.
//---
static void TwoObjectsProximity( add2det_fn Add2Detector )
{
  PathMatrix pathFrom1, pathFrom2;
  SPtr<const MbItem> obj1 = GetGObj( IDS_SELECT_FIRST_SOLID, st_SpaceItem, pathFrom1 );
  if ( obj1 == nullptr )
    return;
  TestVariables::viewManager->ToggleObject( obj1 );

  SPtr<const MbItem> obj2 = GetGObj( IDS_SELECT_SECOND_SOLID, st_SpaceItem, pathFrom2 );
  if ( obj2 == nullptr )
    return;
  TestVariables::viewManager->ToggleObject( obj2 );

  CdCollisionDetection cdet;
  {
    SET_TIME_TEST_REL( true );

    uint miliSec = GetTickCountEx();

    collision_faces colQuery;
    BEGIN_TIME_REL( _T("1-Prepare"), _T("CollisionDetection") );
    cdet_item hSol1 = Add2Detector( cdet, obj1, pathFrom1 );
    cdet_item hSol2 = Add2Detector( cdet, obj2, pathFrom2 );
    END_TIME_REL( _T("1-Prepare") );

    if ( (hSol1 == CDET_NULL) || (hSol2 == CDET_NULL) )
    {
      TestVariables::viewManager->ToggleObject( obj1 );
      TestVariables::viewManager->ToggleObject( obj2 );
      return;
    }
    static double tolDelta = -0.01;
    GetDouble( IDS_ENTER_LEN_TOLERANCE, tolDelta );
    cdet.SetToleranceDelta( hSol1, tolDelta );
    cdet.SetToleranceDelta( hSol2, tolDelta );

    BEGIN_TIME_REL( _T("2-ComputeBVTree"), _T("CollisionDetection") );
    // Можно не вызывать заранее, но время BVT перейдет в запросы CheckCollisions.
    cdet._ComputeBVTree( hSol1 );
    cdet._ComputeBVTree( hSol2 );
    END_TIME_REL( _T("2-ComputeBVTree") );

    if ( (hSol1 == CDET_NULL) || (hSol2 == CDET_NULL) )
    {
      MessageBoxEx( IDS_TITLE_ERR, _T("A mesh wasn't registered.") );
    }
    BEGIN_TIME_REL( _T("3-CheckCollisions"), _T("CollisionDetection") );
    cdet_result res = cdet.CheckCollisions( colQuery );
    END_TIME_REL( _T("3-CheckCollisions") );

    if ( res == CDM_RESULT_NoCollisionDetected )
    {
      BEGIN_TIME_REL( _T("4-DistanceQuery"), _T("CollisionDetection") );
      MbProximityParameters dPars;
      cdet.DistanceQuery( hSol1, hSol2, dPars );
      TestVariables::drawImp->DrawLine( dPars.fstPnt, dPars.sndPnt, TRGB_ORANGE );
      END_TIME_REL( _T("4-DistanceQuery") );
    }

    miliSec = GetTickCountEx() - miliSec;

    CHECK_TIME_TEST();

    TimeTest * testResult = nullptr;

    testResult = GetTimeTestResults();

    PutResultMessage( res, _T("Collision Detection:") );
    ShowInterferenceResult( testResult, colQuery.facePairsCount, miliSec );

    SET_TIME_TEST_REL( false );
  }

  TestVariables::viewManager->ToggleObject( obj1 );
  TestVariables::viewManager->ToggleObject( obj2 );
}

//------------------------------------------------------------------------------
// Тест на соударение двух тел.
// Test for collision of two solids.
//---
void CollisionInterferenceSolids()
{
  return TwoObjectsProximity( AddSolid );
}

//------------------------------------------------------------------------------
// Тест на соударение двух полигональных сеток.
// Test for collision of two polygonal meshes.
//---
void CollisionInterferenceMeshes()
{
  return TwoObjectsProximity( AddMesh );
}

//------------------------------------------------------------------------------
// Тесты на интерференцию геометрических объектов.
// Interference tests of solids and subassemblies.
// (!) Ряд кейсов выделен в отдельные тесты, чтобы не усложнять читаемость кода.
//---
void InterferenceDetection( InterferenceTest test )
{
  PathMatrix pathFrom1, pathFrom2;
  SPtr<MbItem> obj1 = GetGObj( IDS_SELECT_FIRST_SOLID, st_Solid, pathFrom1 );
  SPtr<MbItem> obj2 = nullptr;  

  if ( obj1 ) 
  {
    TestVariables::viewManager->ToggleObject( obj1 );
    obj2 = GetGObj( IDS_SELECT_SECOND_SOLID, st_Solid, pathFrom2 );
    if ( obj2 )
      TestVariables::viewManager->ToggleObject( obj2 );
  }
  else 
  {
    RPArray<MbItem> items;
    SArray<MbMatrix3D> matrs;

    TestVariables::viewManager->GetObjects( st_Solid, items, matrs ); // Дать все объекты указанного типа. // Give all objects of the specified type.

    if ( items.size() == 2 && matrs.size() == 2 ) 
    {
      obj1 = items[0];
      obj2 = items[1];
      pathFrom1.second = matrs[0];
      pathFrom2.second = matrs[1];
      TestVariables::viewManager->ToggleObject(obj1);
      TestVariables::viewManager->ToggleObject(obj2);
    }
  }

  if ( obj1 == nullptr || obj2 == nullptr )
    return;

  { 
    SPtr<MbSolid> solid1( isa_cast<MbSolid*>(obj1.get()) );
    SPtr<MbSolid> solid2( isa_cast<MbSolid*>(obj2.get()) );

    if ( solid1 && solid2 ) 
    {
      c3d::IndicesPairsVector facesNumbers;
      bool b1 = false, b2 = false;

      SET_TIME_TEST_REL( true );

      const uint startTicks = GetTickCountEx();

      MbResultType res = rt_Error;
      

      /*
        Поиск контактирующих граней двух тел с противоположными нормалями. 
        Find coincident faces with opposite normals. SD#7165647
      */
      if ( test == IT_FindTouchedFaces || test == IT_SplitTouchedFaces )
      { 
        double precision = TestVariables::accuracy;
        ::GetDouble( IDS_ENTER_ACCURACY, precision ); 
        TestVariables::accuracy = precision;
            
        if ( test == IT_SplitTouchedFaces )
        {
          facesNumbers.clear();
          solid1->SetOwnChangedThrough( tct_Unchanged );
          solid2->SetOwnChangedThrough( tct_Unchanged );

          if ( ::SplitTouchedFaces( *solid1, *solid2, precision, facesNumbers ) )
            res = rt_Success;
          else
            res = rt_Empty;
          b1 = ::Dirty( solid1 );
          b2 = ::Dirty( solid2 );

          TestVariables::viewManager->RefreshDrawItem( obj1 );
          TestVariables::viewManager->RefreshDrawItem( obj2 );
          TestVariables::viewManager->RedrawObject( obj2 );
          TestVariables::viewManager->RedrawObject( obj1 );

          if ( !b1 && !b2 && res != rt_Success ) { // Для отладки и поиска ошибок.
            ::Clear( solid1 );
            ::Clear( solid2 );
            ::SplitTouchedFaces( *solid1, *solid2, precision, facesNumbers );
            bool bb1 = ::Dirty( solid1 );
            bool bb2 = ::Dirty( solid2 );
            if ( bb1 || bb2 ) {
              ::PutErrorMessage( res, _T("---Solids is changed---") );
            }
          }
        }

        if ( test == IT_FindTouchedFaces )
        {
          facesNumbers.clear();
          ::FindTouchedFaces( *solid1, *solid2, precision, facesNumbers );
        }
      }

      if ( test == IT_TouchedSolidsMerging )
      { // Объединить тела, имеющие контактирующие грани.
        double precision = TestVariables::precision;
        ::GetDouble( IDS_ENTER_ACCURACY, precision );
        TestVariables::precision = precision;

        MbeCopyMode sameShell1 = cm_Copy;
        MbeCopyMode sameShell2 = cm_Copy;
        MbSNameMaker names;

        MbSolid * result( nullptr );

        res = ::TouchedSolidsMerging( *solid1, sameShell1, *solid2, sameShell2, names, precision, result );

        if ( result != nullptr )
        {
          Style ns( 1, MB_C3DCOLOR );
          if ( obj1 != nullptr )
            ns.Init( obj1->GetWidth(), obj1->GetColor() );
          TestVariables::viewManager->DeleteObject( obj2 );
          TestVariables::viewManager->DeleteObject( obj1 );
          TestVariables::viewManager->AddObject( ns, result );
        }
      }

      const auto miliSec = GetTickCountEx() - startTicks;

      CHECK_TIME_TEST();

      TimeTest * testResult = GetTimeTestResults();

      size_t operationCount = solid1->GetCreatorsCount()+solid2->GetCreatorsCount();
      size_t facesCount = solid1->GetFacesCount() + solid2->GetFacesCount();


      

      if ( test == IT_FindTouchedFaces )
      {
        operationCount = facesNumbers.size();
        for ( size_t i = 0, iCount = facesNumbers.size(); i < iCount; ++i ) {
          IndicesPair pair = facesNumbers[i];
          const MbFace * face1 = solid1->GetFace( pair.first );
          const MbFace * face2 = solid2->GetFace( pair.second );
          res = rt_Success;

//#ifdef _DRAWGI
          TestVariables::drawImp->DrawItem( face1, TRGB_ORANGE );
          TestVariables::drawImp->DrawItem( face2, TRGB_AZURE );
          TestVariables::drawImp->DrawItem( face1, TRGB_AZURE );
          TestVariables::drawImp->DrawItem( face2, TRGB_ORANGE );
//#endif // _DRAWGI

        }
      }
      if ( test == IT_SplitTouchedFaces )
      {
        operationCount = facesNumbers.size();
        if ( res == rt_Success && facesNumbers.size() > 0 ) {
          for ( size_t i = 0, iCount = facesNumbers.size(); i < iCount; i++ ) {
            IndicesPair pair = facesNumbers[i];
            const MbFace * face1 = solid1->GetFace( pair.first );
            const MbFace * face2 = solid2->GetFace( pair.second );
//#ifdef _DRAWGI
            TestVariables::drawImp->DrawItem( face1, TRGB_ORANGE, MbMatrix3D::identity, 3 );
            TestVariables::drawImp->DrawItem( face2, TRGB_AZURE, MbMatrix3D::identity, 2 );
//#endif // _DRAWGI
          }

          if ( b1 && b2 )
            PutErrorMessage( res, _T("Ok! Both solids are changed") );
          else
          if ( b1 && !b2 )
            PutErrorMessage( res, _T("Ok! First solid is changed") );
          else
          if ( !b1 && b2 )
            PutErrorMessage( res, _T("Ok! Second solid is changed") );
          else
            PutErrorMessage( res, _T("Ok! There are no changes") );
        }
        else {
          PutErrorMessage( res, _T("---Error---") );
        }
      }
      PutErrorMessage( res, _T("Interference Detection:") );
      ShowTimeTestResult( testResult, operationCount, facesCount, GetTickCountEx() - miliSec, st_Solid, IDS_COLLISION_DETECTION );

      SET_TIME_TEST_REL( false );
    }
  }

  TestVariables::viewManager->ToggleObject( obj1 );
  TestVariables::viewManager->ToggleObject( obj2 );
}

//-----------------------------------------------------------------------------+
// Тесты на интерференцию геометрических объектов.
// Interference tests of solids.
//---
void InterferenceSolids()
{
  PathMatrix pathFrom1, pathFrom2;
  SPtr<MbItem> obj1 = GetGObj( IDS_SELECT_FIRST_SOLID, st_Solid, pathFrom1 );
  SPtr<MbItem> obj2 = nullptr;  

  if ( obj1 ) 
  {
    TestVariables::viewManager->ToggleObject( obj1 );
    obj2 = GetGObj( IDS_SELECT_SECOND_SOLID, st_Solid, pathFrom2 );
    if ( obj2 )
      TestVariables::viewManager->ToggleObject( obj2 );
  }
  else 
  {
    RPArray<MbItem> items;
    SArray<MbMatrix3D> matrs;

    TestVariables::viewManager->GetObjects( st_Solid, items, matrs ); // Дать все объекты указанного типа. // Give all objects of the specified type.

    if ( items.size() == 2 && matrs.size() == 2 ) 
    {
      obj1 = items[0];
      obj2 = items[1];
      pathFrom1.second = matrs[0];
      pathFrom2.second = matrs[1];
      TestVariables::viewManager->ToggleObject(obj1);
      TestVariables::viewManager->ToggleObject(obj2);
    }
  }

  if ( obj1 == nullptr || obj2 == nullptr )
    return;

  { 
    SPtr<MbSolid> solid1( isa_cast<MbSolid*>(obj1.get()) );
    SPtr<MbSolid> solid2( isa_cast<MbSolid*>(obj2.get()) );

    if ( solid1 && solid2 ) 
    {
      c3d::IndicesPairsVector facesNumbers;

      SET_TIME_TEST_REL( true );

      TimeTest * testResult = GetTimeTestResults();

      const uint startTicks = GetTickCountEx();

      MbResultType res = rt_Error;
      
      {
        // Пересечение посредством аппарата булевой операции
        std::vector<MbCurveEdge *> dummyCurveEdges;
        c3d::IndicesPairsVector intersectedFaces;
        c3d::IndicesPairsVector similarFaces;
        c3d::IndicesPairsVector touchedFaces;

        if ( !pathFrom1.second.IsSingle() ) {
          c3d::ShellSPtr shell( solid1->GetShell()->Duplicate() );
          solid1 = new MbSolid( shell, nullptr );
          solid1->Transform( pathFrom1.second );
        }

        if ( !pathFrom2.second.IsSingle() ) {
          c3d::ShellSPtr shell( solid2->GetShell()->Duplicate() );
          solid2 = new MbSolid( shell, nullptr );
          solid2->Transform( pathFrom2.second );
        }

        res = InterferenceSolids( *solid1, *solid2, &dummyCurveEdges, &intersectedFaces, &similarFaces, &touchedFaces );
        const uint resTicks = GetTickCountEx() - startTicks;

        std::sort(intersectedFaces.begin(),intersectedFaces.end());
        auto last = std::unique(intersectedFaces.begin(),intersectedFaces.end());
        intersectedFaces.erase(last,intersectedFaces.end());        

        Style st( TestVariables::SURFACECURVE_Style );
        for ( size_t n = 0, nCount = dummyCurveEdges.size(); n < nCount; n++ ) { // SD_7165647
          TestVariables::viewManager->AddObject( st, &dummyCurveEdges[n]->SetCurve() );
        }
        DeleteItems( dummyCurveEdges );
        PutErrorMessage( res, _T("Interference Detection:") );
        CHECK_TIME_TEST();
        ShowInterferenceResult( testResult, intersectedFaces.size(), resTicks );
      }

      SET_TIME_TEST_REL( false );
    }
  }

  TestVariables::viewManager->ToggleObject( obj1 );
  TestVariables::viewManager->ToggleObject( obj2 );
}

//------------------------------------------------------------------------------
//
//---
#ifdef _DRAWGI
void DrawOrientedBox( const MbMatrix3D & trans, int red, int g, int b )
{  
  static_array<MbCartPoint3D, 8> r( MbCartPoint3D::origin );

  const MbCartPoint3D& q = trans.GetOrigin();
  const MbVector3D a1 = trans.GetAxisX();
  const MbVector3D a2 = trans.GetAxisY();
  const MbVector3D a3 = trans.GetAxisZ();
  const MbCartPoint3D org( q - a1 / 2.0 - a2 / 2.0 - a3 / 2.0 );

  r[0] = org;
  r[1] = org + a1;
  r[2] = r[1] + a2;
  r[3] = org + a2;

  r[4] = r[0] + a3;
  r[5] = r[1] + a3;
  r[6] = r[2] + a3;
  r[7] = r[3] + a3; // диагональный

  DrawGI::DrawLine( r[0], r[1], red, g, b );
  DrawGI::DrawLine( r[1], r[2], red, g, b );
  DrawGI::DrawLine( r[2], r[3], red, g, b );
  DrawGI::DrawLine( r[3], r[0], red, g, b );

  DrawGI::DrawLine( r[4], r[5], red, g, b );  
  DrawGI::DrawLine( r[5], r[6], red, g, b );
  DrawGI::DrawLine( r[6], r[7], red, g, b );
  DrawGI::DrawLine( r[7], r[4], red, g, b );  

  DrawGI::DrawLine( r[4], r[0], red, g, b );  
  DrawGI::DrawLine( r[5], r[1], red, g, b );
  DrawGI::DrawLine( r[6], r[2], red, g, b );
  DrawGI::DrawLine( r[7], r[3], red, g, b );  
}
#endif // _DRAWGI

//------------------------------------------------------------------------------
// Get the mesh representation of parallepiped specified by the marix.
//---
static SPtr<MbMesh> BoxMesh( const MbMatrix3D & trans )
{
  SPtr<MbMesh> mesh;
  CreateBoxMesh( trans, mesh );
  return mesh;
}

//------------------------------------------------------------------------------
//
//---
static SPtr<MbMesh> BoxWire( const MbMatrix3D & trans )
{
  SPtr<MbMesh> mesh;
  CreateBoxWire( trans, mesh );
  return mesh;
}


//----------------------------------------------------------------------------------------
// Обход дерева объемов в ширину.
// Breadth first search in the bounding volume tree.
//---
template<class _Visitor>
void LevelOrderTraversal( cdet_item cdItem, cdet_bvt_node root, _Visitor& visit, size_t bvtLevel )
{
  typedef std::pair<cdet_bvt_node, size_t> node_level;

  std::queue<node_level> Q;
  Q.push( node_level( root, 0 ) );

  while ( !Q.empty() )
  {
    node_level node = Q.front();
    Q.pop();
    if ( visit(cdItem,node, bvtLevel) )
    {
      cdet_bvt_pair subNodes = BvtSubNodes( cdItem, node.first );
      Q.push( node_level(subNodes.first,  node.second + 1) );
      Q.push( node_level(subNodes.second, node.second + 1) );
    }
  }
}

//------------------------------------------------------------------------------
// It shows a bounding box
//---
bool DrawBoundingVolume( cdet_item cdItem, std::pair<cdet_bvt_node, size_t> nodeLevel, size_t bvtLevel )
{
  if ( nodeLevel.second == bvtLevel )
  {
    MbMatrix3D obbTrans; // trasformation an unit cube to oriented parallelepiped.
    GetOrientedBox( cdItem, nodeLevel.first, obbTrans );
#ifdef _DRAWGI
    DrawOrientedBox( obbTrans, TRGB_RED );
#endif // _DRAWGI
  }
  return nodeLevel.second <= bvtLevel; // Stop after the aimed nodal level was achieved.
}

//------------------------------------------------------------------------------
// It makes the bounding box visualization.
//---
bool AddBndVolumeMesh( cdet_item cdItem, std::pair<cdet_bvt_node, size_t> nodeLevel, size_t bvtLevel )
{
  if ( nodeLevel.second == bvtLevel )
  {
    MbMatrix3D obbTrans; // trasformation an unit cube to oriented parallelepiped.
    GetOrientedBox( cdItem, nodeLevel.first, obbTrans );
      
    if ( SPtr<MbMesh> mesh = BoxMesh(obbTrans) )
    {
      mesh->SetVisual( 0.5f, 1.0f, 0.1f, 0.5f/*b*/, 0.3f, 0.1f );

      AttrVector attrs;
      mesh->GetAttributes( attrs, at_Visual );
      if ( !attrs.empty() )
      {        
        MbVisual * visProps = static_cast<MbVisual*>( attrs.front() );
        visProps->SetAmbient( MbRGBA(0.0f,0.5f,1.0f) );
        visProps->SetColor( MbRGBA(0.0f,0.5f,1.0f) ); // Установка цвета.
      }

      TestVariables::viewManager->AddObject( *mesh );
    }
    if ( SPtr<MbMesh> mesh = BoxWire(MbMatrix3D::identity) )
    {
      mesh->Transform( obbTrans );
      mesh->SetColor( 0, 128, 255 );
      TestVariables::viewManager->AddObject( *mesh );
    }
  }

  return nodeLevel.second <= bvtLevel; // Stop after the aimed nodal level was achieved.
}

//------------------------------------------------------------------------------
// Расчет элементов дерева объемов
// Compute elements of the bounding volume tree
//---
void ComputeBVTree()
{
  // The level of the bounding volume tree.
  int bvtLevel = 1;

  /*
    Задается уровень дерева объемов для отображения.
    It is specifying the level of the bounding volume tree.
  */
  if ( !GetInt( IDS_BVT_LEVEL, bvtLevel ) ) 
    return;

  PathMatrix pathFrom1;
  SPtr<MbItem> gObj = GetGObj( IDS_SELECT_FIRST_SOLID, st_Solid, pathFrom1 );

  if ( gObj == nullptr )
    return;

  TestVariables::viewManager->ToggleObject( gObj );
  
  SPtr<const MbSolid> solid( isa_cast<const MbSolid*>(gObj) );
  if ( solid ) 
  {
    CdCollisionDetection cdet;

    const MbPlacement3D pl1( pathFrom1.second );

    collision_faces colQuery;
    cdet_item cdItem = cdet.AddItem( *solid, pl1, (void*)gObj );
           
    // LevelOrderTraversal( cdItem, cdet._ComputeBVTree(cdItem), DrawBoundingVolume, bvtLevel );
    LevelOrderTraversal( cdItem, cdet._ComputeBVTree(cdItem), AddBndVolumeMesh, bvtLevel );
    TestVariables::viewManager->ShowModel();

    if ( cdItem == CDET_NULL  )
    {
      MessageBoxEx( IDS_TITLE_ERR, _T("A solid wasn't registered.") );
    }
  }
  TestVariables::viewManager->ToggleObject( gObj );
}

//------------------------------------------------------------------------------
// Вывод на экран массо-центровочных характеристик
// ---
static
void OutputMassInertia( InertiaProperties & mp, double deviateAngle, 
                        clock_t end_time, clock_t start_time, double diagonal )
{
  TCHAR s[2048];
  TCHAR s0[512];
  double density = 0.0;
  if ( ::fabs(mp.volume) > NULL_EPSILON ) 
     density = mp.mass / mp.volume;
  else {
    if ( ::fabs(mp.area) > NULL_EPSILON ) 
       density = mp.mass / mp.area;
  }
  _sntprintf( s, 2048, RT_AREA_VOLUME_MASS_AND_DENSITY, mp.area, mp.volume, mp.mass, density );
  _sntprintf( s0, 512, RT_WEIGHT_CENTRE, mp.center.x, mp.center.y, mp.center.z );
  _tcscat(s,s0);
  _sntprintf( s0, 512, RT_MATRIX_OF_MOMENTS_OF_INERTIA_IN_CURRENT_PLACE,
             mp.initial[0][0], mp.initial[0][1], mp.initial[0][2],
             mp.initial[1][0], mp.initial[1][1], mp.initial[1][2],
             mp.initial[2][0], mp.initial[2][1], mp.initial[2][2] );
  _tcscat(s,s0);
  _sntprintf( s0, 512, RT_MATRIX_OF_CENTRAL_MOMENTS_OF_INERTIA,
             mp.moments[0][0], mp.moments[0][1], mp.moments[0][2],
             mp.moments[1][0], mp.moments[1][1], mp.moments[1][2],
             mp.moments[2][0], mp.moments[2][1], mp.moments[2][2] );
  _tcscat(s,s0);
  if ( mp.direction[1].Length()>Math::lengthEpsilon ) {
    TCHAR s1[512];
    _sntprintf( s0, 512, RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA, mp.general[0], mp.general[1], mp.general[2] );
    _sntprintf( s1, 512, RT_DIRECTIONS_OF_PRINCIPAL_AXES,
               mp.direction[0].x, mp.direction[0].y, mp.direction[0].z,
               mp.direction[1].x, mp.direction[1].y, mp.direction[1].z,
               mp.direction[2].x, mp.direction[2].y, mp.direction[2].z );
    _tcscat(s0,s1);
  }
  else {
    if ( mp.direction[0].Length()>Math::lengthEpsilon )
      _sntprintf( s0, 512, RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA_AND_AXIS_1,
                 mp.general[0], mp.general[1], mp.general[2],
                 mp.direction[0].x, mp.direction[0].y, mp.direction[0].z );
    else
    if ( mp.direction[2].Length()>Math::lengthEpsilon )
      _sntprintf( s0, 512, RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA_AND_AXIS_3,
                 mp.general[0], mp.general[1], mp.general[2],
                 mp.direction[2].x, mp.direction[2].y, mp.direction[2].z );
    else
      _sntprintf( s0, 512, RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA_AND_ANY_AXIS,
                 mp.general[0], mp.general[1], mp.general[2] );
  }
  _tcscat(s,s0);
  _sntprintf( s0, 512, RT_TIME_AND_TOLERANCE, end_time - start_time, deviateAngle );
  _tcscat(s,s0);

  MbCartPoint3D p;
  Style ns;
  double l0 = mp.direction[0].Length();
  double l1 = mp.direction[1].Length();
  double l2 = mp.direction[2].Length();

  diagonal *= ONE_THIRD;

  if ( l0>Math::lengthEpsilon ) {
    ns = TestVariables::SURFACECURVE_Style;
    ns.SetWidth( 4 );              //-V112
    p.Set( mp.center, mp.direction[0], diagonal );
    TestVariables::viewManager->AddObject( ns, new MbLineSegment3D( mp.center, p ) );
  }
  if ( l1>Math::lengthEpsilon ) {
    ns = TestVariables::SWEPT_Style;
    ns.SetWidth( 4 );              //-V112
    p.Set( mp.center, mp.direction[1], diagonal*mp.general[1]/mp.general[0] );
    TestVariables::viewManager->AddObject( ns, new MbLineSegment3D( mp.center, p ) );
  }
  if ( l2>Math::lengthEpsilon ) {
    ns = TestVariables::LINE_SEG_Style;
    ns.SetWidth( 4 );              //-V112
    p.Set( mp.center, mp.direction[2], diagonal*mp.general[2]/mp.general[0] );
    TestVariables::viewManager->AddObject( ns, new MbLineSegment3D( mp.center, p ) );
  }

  MessageBoxEx( s, IDS_RESULTS );
}


//------------------------------------------------------------------------------
// Индикатор.
// ---
class ProgressIndicator : public IfProgressIndicator
{
  ptrdiff_t m_cur;
  ptrdiff_t m_max;

public:
  ProgressIndicator() : m_cur(0), m_max(0) {}

  /// \ru Инициализация. \en Initialization. 
  virtual void StartProgress ( ptrdiff_t minValue, ptrdiff_t maxValue, const TCHAR* , bool, bool ) {
    m_cur = minValue;
    m_max = maxValue;
  }
  /// \ru Установка текущего значения. \en Setting of the current value. 
  virtual void SetProgress ( ptrdiff_t nCurr, const TCHAR*, bool, bool ) {
    if ( m_max == 0 || m_cur > m_max )
      return;
    m_cur = nCurr;
    std::wstringstream s;
    s << _T("Processing face ") << nCurr << _T(" of ") << m_max;
    string_t ss = c3d::ToC3Dstring( s.str() );
    ::ShowString( ss.c_str() );
  }
  /// \ru Остановить индикатор. \en Stop the indicator. 
  virtual void StopProgress ( const TCHAR*, bool, bool ) { m_cur = m_max = 0; }
  virtual const TCHAR * GetStrBuild () const { return _T(""); }
  /// \ru Установить строку в индиктор. \en Set the string to the indicator. 
  virtual void StBarSetMessageText( const TCHAR*, bool ) {}
  virtual bool IsProgressStarted() const { return m_max > 0; }
};


#define   ASSEMBLY_MI_ATTIRE
//------------------------------------------------------------------------------
// Определение массо-центровочных характеристик тел
// ---
void SolidMassInertia()
{
  MbItem * obj = nullptr;
  Style ns = TestVariables::SOLID_Style;
  double d = 1.0; //2.5;
  double deviateAngle = Math::deviateSag; // Параметр управления точностью расчёта (0.35 - 0.01)

  GetDouble( IDS_ENTER_ASSEMBLYDENSITY, d );  
  GetDouble( IDS_ENTER_ANG_TOLERANCE, deviateAngle );
  RPArray<MbItem> items(3,1);         //-V112
  SArray<MbMatrix3D> matrs(3,1);        //-V112
  SArray<double> densities(3,1);        //-V112
  MbMatrix3D matr;

  ProgressIndicator indicator;
  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    MbItem * pObj = nullptr;

    items.Add( obj );
    matrs.Add( matr );
    densities.Add( d );
    while ( GetGObj( IDS_SELECT_SOLID, st_Solid, &pObj ) ) {
      TestVariables::viewManager->ToggleObject(obj);
      TestVariables::viewManager->ToggleObject(pObj);
      items.Add( pObj );
      matrs.Add( matr );
      densities.Add( d );
      obj = pObj;
    }
    if ( pObj != nullptr )
      TestVariables::viewManager->ToggleObject(pObj);
    else
      TestVariables::viewManager->ToggleObject(obj);
  }
  else {
    int i = 1;
    if ( GetInt( IDS_ENTER_QUANTITY, i ) && (i > 0) ) {  
      TestVariables::viewManager->GetObjects( st_Solid, items, matrs ); // Дать все объекты указанного типа.
      densities.Reserve( items.Count() );
      for ( size_t j = 0, jCount = items.Count(); j < jCount; j++ ) 
        densities.Add( d );
    }
  }
  if ( (items.Count() > 0) && (items.Count() == matrs.Count()) && (items.Count() == densities.Count()) ) 
  {
    PArray<InertiaProperties> mpSolids( 0, 1, true ); // Имеющиеся характеристики тел (или nullptr)
    InertiaProperties mp;

//#if defined(C3D_DEBUG)
//      SET_TIME_TEST(true);
//#endif // C3D_DEBUG

#ifndef   ASSEMBLY_MI_ATTIRE
    // Обычный расчёт свойств сборки
    uint miliSec = ::GetTickCountEx();
    clock_t start_time = clock();
    SetWaitCursor( true );
    ::MassInertiaProperties( solids, densities, matrs, mpSolids, deviateAngle, mp, &indicator );
    SetWaitCursor( false );
#else
    // Используем предустановку свойств сборки
    ptrdiff_t sCount = items.Count();
    RPArray<AssemblyMIAttire> assembleAttire(0,1);
    RPArray<SolidMIAttire>    solidsAttire(sCount,1);
    bool bSet = true;
    double m2 = 0.0;
    MbCube cube;

    for ( ptrdiff_t i = 0; i < sCount; i++ ) {
      MbItem * item = items[i];
      if ( item->IsA() == st_Solid ) { 
        MbSolid * sol = (MbSolid *)item;
        matr = matrs[i];
        double dens = densities[i];
        SolidMIAttire * sAttire = new SolidMIAttire( *sol, dens, matr );
        
        sol->AddYourGabaritTo( cube );

#ifdef C3D_DEBUG // отрисовка под release искажает профилирование
        TestVariables::drawImp->DrawItem( sol, TRGB_LIGHTRED );
#endif
        if ( bSet ) {
          double m1 = 0.0;
//          if ( GetDouble( IDS_ENTER_SOLIDMASS, m1 ) ) {
            if ( m1 > METRIC_REGION ) {
              InertiaProperties prop;
              prop.mass    = std_max( 0.0, m1 ); // Масса
              sAttire->SetProperties( prop );
              m2 += m1;
            }
            else 
              bSet = false; // Больше не спрашивать
//          }
//          else 
//            bSet = false; // Больше не спрашивать
        }

#ifdef C3D_DEBUG // отрисовка под release искажает профилирование
        TestVariables::drawImp->DrawItem( sol, TRGB_LIGHTGREEN );
#endif

        solidsAttire.Add( sAttire );
      }
    }
    MbMatrix3D matrA;
    AssemblyMIAttire assemblyMI( assembleAttire, solidsAttire, matrA );

//    if ( GetDouble( IDS_ENTER_ASSEMBLYMASS, m2 ) ) {
    if ( m2 > METRIC_NEAR ) {
      InertiaProperties prop;
      prop.mass    = std_max( 0.0, m2 ); // Масса
      assemblyMI.SetProperties( prop );
    }
//    }

    uint miliSec = ::GetTickCountEx();
    clock_t start_time = clock();

    SetWaitCursor( true );
    ::MassInertiaProperties( assemblyMI, deviateAngle, mp, &indicator ); 
    SetWaitCursor( false );
#endif // ASSEMBLY_MI_ATTIRE

    clock_t end_time = clock();
    miliSec = ::GetTickCountEx() - miliSec;

//#if defined(C3D_DEBUG)
//      CHECK_TIME_TEST();
//#endif // C3D_DEBUG

//      TimeTest * testResult = nullptr;
//#if defined(C3D_DEBUG)
//      testResult = ::GetTimeTestResults();
//#endif // C3D_DEBUG
//      uint solidsCount = solids.Count();
//      uint facesCount = 0;
//      for (uint j=0;j<solidsCount;j++) 
//        facesCount += solids[j]->GetFacesCount();
//      ShowTimeTestResult( testResult, solidsCount, facesCount, miliSec, _T("Рсчёт массо-центровочных характеристик тел") );

//#if defined(C3D_DEBUG)
//      SET_TIME_TEST(false);
//#endif // C3D_DEBUG

    ::OutputMassInertia( mp, deviateAngle, end_time, start_time, cube.GetDiagonal() ); // Вывод характеристик.

/*
    // Отладка преобразования координат
    MbCartPoint3D p( 0, 0, 10 );
    MbVector3D    z( 0, 1, 0 ), x( 0, 0, 1 );
    MbPlacement3D place( p, z, x );
    MbMatrix3D from;
    place.GetMatrixFrom( from );
    mp.Transform( from );
    OutputMassInertia( mp, deviateAngle, end_time, start_time );
*/

    bool bm = false;// true;//

    if ( bm ) { // Проверка на полигональных обънетах. // SD_7149606
      InertiaProperties mpMesh;
      std::vector<const MbMesh *> meshs; 
      std::vector<double> densitys; 
      std::vector<MbMatrix3D> matrix; 
      MbStepData stepData( ist_DeviationStep, Math::deviateSag);
      MbFormNote note( false, true, true, true, true );
      meshs.reserve( sCount );
      densitys.reserve( sCount );
      matrix.reserve( sCount );
      for ( ptrdiff_t i = 0; i < sCount; i++ ) {
        MbItem * item = items[i];
        if ( item->IsA() == st_Solid ) { 
          MbSolid * sol = (MbSolid *)item;
          matr = matrs[i];
          double dens = densities[i];
          MbMesh * mesh = new MbMesh( true );
          sol->CalculateMesh( stepData, note, *mesh );
          meshs.push_back( mesh );
          densitys.push_back( dens );
          matrix.push_back( matr );
        }
      }
      miliSec = ::GetTickCountEx();
      start_time = clock();

      // Вычисление масс-центровочных характеристик. Calculation of mass-inertial properties.
      ::MassInertiaProperties( meshs, densitys, matrix, mpMesh );

      end_time = clock();
      miliSec = ::GetTickCountEx() - miliSec;

      ::DeleteItems( meshs );

      ::OutputMassInertia( mpMesh, deviateAngle, end_time, start_time, cube.GetDiagonal() ); // Вывод характеристик.
    }

  }
}
#undef   ASSEMBLY_MI_ATTIRE


//------------------------------------------------------------------------------
// Определение массо-центровочных характеристик полигональных объектов.
// Calculation of mass-inertia characteristics of polygonal objects.
// ---
void MeshMassInertia()
{
  MbItem * obj = nullptr;
  double d = 1.0;
  // Параметр управления точностью расчёта. Parameter controlling calculation accuracy.
  double deviateAngle = Math::deviateSag;

  GetDouble( IDS_ENTER_ASSEMBLYDENSITY, d );  
  GetDouble( IDS_ENTER_ANG_TOLERANCE, deviateAngle );
  RPArray<MbItem> items( 3, 1 );
  SArray<MbMatrix3D> matrs( 3, 1 );
  SArray<double> densities( 3, 1 );
  MbMatrix3D matr;

  ProgressIndicator indicator;
  if ( GetGObj(IDS_SELECT_MESH, st_Mesh, &obj) ) {
    TestVariables::viewManager->ToggleObject(obj);
    items.Add( obj );
    matrs.Add( matr );
    densities.Add( d );

    MbItem * pObj = nullptr;
    while ( GetGObj(IDS_SELECT_MESH, st_Mesh, &pObj) ) {
      TestVariables::viewManager->ToggleObject( obj );
      TestVariables::viewManager->ToggleObject( pObj );
      items.Add( pObj );
      matrs.Add( matr );
      densities.Add( d );
      obj = pObj;
    }
    if ( pObj != nullptr )
      TestVariables::viewManager->ToggleObject( pObj );
    else
      TestVariables::viewManager->ToggleObject( obj );
  }
  else {
    int i = 1;
    if ( GetInt(IDS_ENTER_QUANTITY, i) && i > 0 ) {  
      // Дать все объекты указанного типа. Get all objects of this type.
      TestVariables::viewManager->GetObjects( st_Mesh, items, matrs );
      densities.Reserve( items.Count() );
      for ( size_t j = 0, jCount = items.Count(); j < jCount; j++ ) 
        densities.Add( d );
    }
  }

  if ( items.Count() > 0 && items.Count() == matrs.Count() && items.Count() == densities.Count() ) {
    InertiaProperties mp;
    MbCube cube;
    std::vector<double> dens;
    std::vector<MbMatrix3D> matrix;
    std::vector<const MbMesh *> meshes;
    for ( size_t i = 0; i < items.size(); i++ ) {
      MbItem * item = items[i];
      if ( item->IsA() == st_Mesh ) {
        item->AddYourGabaritTo( cube );
        meshes.push_back( static_cast<MbMesh *>(item) );
        dens.push_back( densities[i] );
        matrix.push_back( matrs[i] );
      }
    }

    clock_t start_time = clock();
    // Вычисление масс-центровочных характеристик. Calculation of mass-inertial properties.
    ::MassInertiaProperties( meshes, dens, matrix, mp );

    // Вывод характеристик. Output result.
    ::OutputMassInertia( mp, deviateAngle, clock(), start_time, cube.GetDiagonal() );
  }
}


//------------------------------------------------------------------------------
// Вывод на экран массо-центровочных характеристик.  Output result.
// ---
static
void OutputArea( clock_t end_time, clock_t start_time, double area, clock_t end_time1, clock_t start_time1, double area1 )
{
  TCHAR s[2048];
  TCHAR s1[512];

  _sntprintf( s, 2048, RT_TIME_AND_AREA, end_time - start_time, area );
  _sntprintf( s1, 512, RT_TIME_AND_AREA, end_time1 - start_time1, area1 );
  _tcscat(s,s1);

  MessageBoxEx( s, IDS_RESULTS );
}


//------------------------------------------------------------------------------
// Рссчет центра поверхности грани.
// ---
void FaceCentre() {
  MbItem * obj = nullptr;
  MbTopItem * item = nullptr;

  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &item ) ) {
/*
    MbFace * face = (MbFace *)item;
    MbCartPoint3D centre;
    double angle = Math::deviateSag;
    bool byOuter = true;
    ::CalculateAreaCentre( *face, angle, byOuter, Math::DefaultMathVersion(), centre ); 

    TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPoint3D(centre) );
*/
    MbSolid * solid = (MbSolid *)obj;
    double deviateAngle = Math::deviateSag; //0.01;
    MbCartPoint3D centre;
    double area = 0, area1 = 0;
    RPArray<MbFace> faces;
    solid->GetFaces( faces );
    size_t i, iCount = TestVariables::lineWidth;

    ::GetDouble( IDS_ENTER_ANG_TOLERANCE, deviateAngle );

    SetWaitCursor( true );
    clock_t start_time = clock();
    for ( i = 0; i < iCount; i++ )
      area = ::CalculateArea( faces, deviateAngle ); 
    clock_t end_time = clock();

    size_t fCount = solid->GetFacesCount();
    bool byOuter = false;
    VERSION version = Math::DefaultMathVersion();
    clock_t start_time1 = clock();
    if ( fCount == 1 )
      area1 = ::CalculateAreaCentre( *solid->GetFace(0), deviateAngle, byOuter, version, centre ); 
    else
      for ( i = 0; i < iCount; i++ )
        area1 = ::CalculateAreaCentre( *solid->GetShell(), deviateAngle, centre ); 
    clock_t end_time1 = clock();
    SetWaitCursor( false );

    TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, new MbPoint3D(centre) );
    if ( ::fabs(area - area1) > (area + area1)*PARAM_REGION ) {
      MessageBoxEx( RT_ERROR_RESULT_NOT_OBTAINED, IDS_WERE_ERRORS );
    }

    ::OutputArea( end_time, start_time, area, end_time1, start_time1, area1 );

  }
}


//------------------------------------------------------------------------------
// Определение площади замкнутой кривой
// ---
void ContourArea()
{
  MbItem * obj = nullptr;

  if ( GetAnyCurve( IDS_SELECT_CURVE, &obj ) ) {
    MbPlacement3D place( TestVariables::viewManager->GetCplane() );
    const MbCurve * curve = nullptr;
    TestVariables::viewManager->ToggleObject(obj);

    if ( obj->IsA() == st_SpaceInstance ) {
      const MbSpaceItem * spaceItem = static_cast<const MbSpaceInstance *>(obj)->GetSpaceItem(); // Дать трёхмерный геометрический объекта
      if ( (spaceItem != nullptr) && (spaceItem->Family() == st_Curve3D) ) {
        MbMatrix3D into;
        place.GetMatrixInto( into );
        curve = static_cast<const MbCurve3D *>( spaceItem )->GetMap( into );
      }
    }
    if ( obj->IsA() == st_PlaneInstance ) {
      const MbPlaneItem * planeItem = static_cast<const MbPlaneInstance *>(obj)->GetPlaneItem(); // Дать двумерный геометрический объект
      if ( (planeItem != nullptr) && (planeItem->Family() == pt_Curve) ) {
        place.Init( static_cast<const MbPlaneInstance *>(obj)->GetPlacement());
        curve = static_cast<const MbCurve *>(planeItem);
      }
    }

    TestVariables::viewManager->ToggleObject(obj);

    if ( curve != nullptr ) {
      MIProperties mp;
      SetWaitCursor( true );
      double len = curve->GetMetricLength();

      MassInertiaProperties( curve, mp, Math::deviateSag );

      int n(6); // Число оставляемых значащих цифр.
      GetInt( IDS_ENTER_NUMBER, n );

      ::RoundedValue( len,     (uint8)n );
      ::RoundedValue( mp.f,    (uint8)n );
      ::RoundedValue( mp.xc,   (uint8)n );
      ::RoundedValue( mp.yc,   (uint8)n );
      ::RoundedValue( mp.mxx,  (uint8)n );
      ::RoundedValue( mp.myy,  (uint8)n );
      ::RoundedValue( mp.mxy,  (uint8)n );
      ::RoundedValue( mp.mxx0, (uint8)n );
      ::RoundedValue( mp.myy0, (uint8)n );
      ::RoundedValue( mp.a,    (uint8)n );

      SetWaitCursor( false );

      TCHAR s[1024];
      TCHAR s0[512];
      _sntprintf( s, 1024, RT_LENGTH_AREA_WC_AND_CENTRAL_MOMENTS_OF_INERTIA, len, mp.f, mp.xc, mp.yc, mp.myy, mp.mxx, mp.mxy );

      if ( fabs(mp.mxx0-mp.myy0)>Math::lengthEpsilon*(fabs(mp.mxx0)+fabs(mp.myy0)) )
        _sntprintf( s0, 512, RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA_AND_ANGLE_X, mp.mxx0, mp.myy0, mp.a*180/M_PI );
      else
        _sntprintf( s0, 512, RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA_AND_ANGLE, mp.mxx0, mp.myy0, mp.a*180/M_PI );
      _tcscat(s,s0);

      MbCartPoint p(mp.xc,mp.yc);
      Style ns = TestVariables::SURFACECURVE_Style;
      TestVariables::viewManager->AddObject( ns, /*new MbPoint( */p , &place );

      MessageBoxEx( s, IDS_RESULTS );
    }
  }
}


//------------------------------------------------------------------------------
// Замена поверхностей кривых на копии.
// ---
void SetDuplicateSurfaces( RPArray<MbCurve3D> & faceOutlineCurves )
{
  for ( size_t i = 0; i < faceOutlineCurves.Count(); ++i ) {
    // Все кривые должны быть кривыми на поверхности.
    if ( faceOutlineCurves[i] != nullptr && faceOutlineCurves[i]->Type() == st_SurfaceCurve ) {
      MbSurfaceCurve * otherCurve = static_cast<MbSurfaceCurve *>( faceOutlineCurves[i] );
      // Заменим поверхность на копию.
      MbSurface & copySurface = static_cast<MbSurface &>( otherCurve->GetSurface().Duplicate() );
      otherCurve->ChangeSurface( copySurface );
    }
  }
}


//------------------------------------------------------------------------------
// Получение очерка поверхности
// ---
bool SurfacesHide()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    MbSurface * surf = (MbSurface *)((MbSpaceInstance *)obj)->GetSpaceItem();
    if ( surf != nullptr ) {
      RPArray<MbCurve3D> outlineCurves( 2, 2 );
      MbVector3D z( (TestVariables::viewManager->GetActiveWindow())->GetAxisZ() );

      int iEnterViewDir = 0;
      GetInt( IDS_ENTER_USE_OR_NOT, iEnterViewDir );
      if ( iEnterViewDir != 0 )
        GetExactPoint( (MbCartPoint3D &)z );

      int s( 0 ); 
      ::GetInt( IDS_ENTER_PROJECTION_TYPE, s );
      bool perspective = ( s > 0 );
      if ( perspective ) {
        MbCube cube;
        obj->AddYourGabaritTo( cube );
        double diagonal = cube.GetDiagonal();
        ::GetDouble( IDS_ENTER_DISTANCE, diagonal );
        MbCartPoint3D centre;
        cube.GetCentre( centre );
        centre.Add( z, diagonal );
        z = centre;
      }
      bool removeOnSurfaceBounds = true;

      ::SilhouetteCurve( *surf, z, perspective, removeOnSurfaceBounds, outlineCurves, Math::DefaultMathVersion() ); // Для параллельной отрисовки.
      ::SetDuplicateSurfaces( outlineCurves );

      // KYA K13 Объединяем в контура, как в Компасе
      ::CreateContours( outlineCurves, Math::metricPrecision );

      TestVariables::viewManager->ToggleObject(obj);

      for (size_t i = 0; i < outlineCurves.Count(); i++ )
        TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, outlineCurves[i] );

      return true;
    }

    TestVariables::viewManager->ToggleObject(obj);
  }

  return false;
}


//------------------------------------------------------------------------------
// Получение очерка грани
// ---
bool FaceHide()
{
  MbItem    * obj     = nullptr;
  MbTopItem * surface = nullptr;

  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &surface ) ) {
    TestVariables::viewManager->ToggleObject( obj );
    TestVariables::drawImp->DrawItem( surface, TRGB_WHITE );

    MbFace * face = static_cast<MbFace *>( surface );
    WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();
    MbPlacement3D place( currentView3D->GetNullPlacement() );

    RPArray<MbCurve3D> curves;
    int s( 0 ); 
    ::GetInt( IDS_ENTER_PROJECTION_TYPE, s );
    bool perspective = ( s > 0 );
    MbVector3D eye( place.GetAxisZ() );
    if ( perspective ) {
      MbCube cube;
      obj->AddYourGabaritTo( cube );
      double diagonal = cube.GetDiagonal();
      ::GetDouble( IDS_ENTER_DISTANCE, diagonal );
      MbCartPoint3D centre;
      cube.GetCentre( centre );
      centre.Add( eye, diagonal );
      eye = centre;
    }
    
    ::SilhouetteCurve( *face, eye, perspective, curves, Math::DefaultMathVersion() ); // Для параллельной отрисовки.
    ::SetDuplicateSurfaces( curves );

    for (size_t i = 0; i < curves.Count(); i++ )
      TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, curves[i] );
    TestVariables::viewManager->ToggleObject(obj);

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Получение очерка тела
// ---
bool SolidsHide()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    MbSolid * solid = (MbSolid *)obj;
    if ( solid != nullptr ) {
      RPArray<MbCurve3D> outlineCurves( 2, 2 );
      MbVector3D z( (TestVariables::viewManager->GetActiveWindow())->GetAxisZ() );

      int iEnterViewDir = 0;
      GetInt( IDS_ENTER_USE_OR_NOT, iEnterViewDir );
      if ( iEnterViewDir != 0 )
        GetExactPoint( (MbCartPoint3D &)z );

      bool perspective = false;

      ptrdiff_t i0 = solid->GetFacesCount();
      for( ptrdiff_t j = 0; j < i0; j++ ) {
        MbFace * face = solid->GetFace( j );
        if ( face != nullptr ) {
          RPArray<MbCurve3D> faceOutlineCurves( 0, 1 );
          ::SilhouetteCurve( face->GetSurface(), z, perspective, false, faceOutlineCurves, Math::DefaultMathVersion() );
          ::SetDuplicateSurfaces( faceOutlineCurves ); // Для параллельной отрисовки.
          // KYA K13 Объединяем в контура, как в Компасе
          ::CreateContours( faceOutlineCurves, Math::metricPrecision );
          outlineCurves.AddArray( faceOutlineCurves );
          faceOutlineCurves.DetachAll();
        }
      }

      TestVariables::viewManager->ToggleObject(obj);

      for ( size_t i = 0; i < outlineCurves.Count();i++)
        TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, outlineCurves[i] );

      return true;
    }

    TestVariables::viewManager->ToggleObject(obj);
  }

  return false;
}


//------------------------------------------------------------------------------
// Аппроксимировать поверхность треугольными пластинами
// ---
bool TriangulateSurface()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    MbItem * meshItem = ((MbSpaceInstance *)obj)->CreateMesh( MbStepData(ist_SpaceStep, Math::visualSag), MbFormNote(false, true, true, TestVariables::exactMesh), nullptr );

    TestVariables::viewManager->ToggleObject(obj);
    
    if ( (meshItem != nullptr) && (meshItem->IsA() == st_Mesh) ) {
      MbMesh * mesh = (MbMesh *)meshItem;
      for ( ptrdiff_t i = 0, iCount = mesh->GridsCount(); i < iCount; i++ ) {
        TestVariables::drawImp->PutGrid( *(mesh->GetGrid(i)), TRGB_LIGHTRED ); // Нарисовать триангуляцию
      }
    }

    ::DeleteItem( meshItem );
    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Запросить параметры для триангуляции
// ---
bool InitStepData( MbStepData & resStepData, bool createParams, int defaultMode )
{
  bool res = false;
  MbStepData stepData;
  double visSag = TestVariables::visualSag;
  double devSag = TestVariables::deviateAngle;
  double length = TestVariables::maxLength;
  
  int mode = defaultMode;
  mode = std_max( mode, 0 );
  mode = std_min( mode, 6 ); // см. ниже

  if ( GetInt( IDS_ENTER_STEP_TYPE, mode ) ) {
    TestVariables::operateType = mode;
    switch( mode ) {
      case 0  : { 
        ::GetDouble( IDS_VISUAL_TOL, visSag );
        stepData.Init( ist_SpaceStep, visSag, devSag, length );  
        TestVariables::visualSag = visSag;
      } break;
      case 1  : { 
        ::GetDouble( IDS_DEVIATE_TOL, devSag );
        stepData.Init( ist_DeviationStep, visSag, devSag, length );
        TestVariables::deviateAngle = devSag;
      } break;
      case 2  : { 
        ::GetDouble( IDS_VISUAL_TOL, visSag );
        ::GetDouble( IDS_DEVIATE_TOL, devSag );
        stepData.Init( ist_SpaceStep, visSag, devSag, length );  
        stepData.SetStepType( ist_DeviationStep );
        TestVariables::visualSag = visSag;
        TestVariables::deviateAngle = devSag;
      } break;
      case 3  : { 
        ::GetDouble( IDS_METRIC_TOL, length );
        stepData.Init( ist_MetricStep, visSag, devSag, length ); 
        TestVariables::maxLength = length;
      } break;
      case 4 : { 
        ::GetDouble( IDS_VISUAL_TOL, visSag );
        ::GetDouble( IDS_DEVIATE_TOL, devSag );
        ::GetDouble( IDS_METRIC_TOL, length );
        stepData.Init( ist_SpaceStep, visSag, devSag, length );  
        stepData.SetStepType( ist_DeviationStep );
        stepData.SetStepType( ist_MetricStep );
        TestVariables::visualSag = visSag;
        TestVariables::deviateAngle = devSag;
        TestVariables::maxLength = length;
      } break;
      case 5 : { 
        ::GetDouble( IDS_DEVIATE_TOL, devSag );
        stepData.Init( ist_MipStep, visSag, devSag, length );  
        TestVariables::deviateAngle = devSag;
      } break;
      case 6 : { // SD_7190542
        stepData.Init(ist_MetricStep, 0.01f, 0.05f, 0.5f);
      } break;
      default: {
        stepData.Init( ist_SpaceStep, visSag, devSag, length );  
      } break;
    }

    if ( createParams )
      stepData.SetStepType( ist_ParamStep );

    resStepData = stepData;
    res = true;
  }

  return res;
}

//------------------------------------------------------------------------------
// Аппроксимировать тело треугольными пластинами.
// ---
bool TriangulateSolid()
{
  MbItem * obj = nullptr;
  PathMatrix pathFrom;

  bool createParams = false;
  {
    int iCreateParams = createParams ? 1 : 0;
    GetInt( IDS_USE_PARAM_POINTS, iCreateParams );
    if ( iCreateParams == 0 )
      createParams = false;
  }
  MbStepData stepData;
  int mode = TestVariables::operateType;
  ::InitStepData( stepData, createParams, mode );

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj, pathFrom ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    const MbSolid * sol = (const MbSolid *)obj;

    if ( sol != nullptr ) {
      SetWaitCursor( true );

#ifdef C3D_DEBUG
      SET_TIME_TEST(true);
#endif // C3D_DEBUG

      uint miliSec = ::GetTickCountEx();

      MbItem * meshItem = ((MbSpaceInstance *)obj)->CreateMesh( stepData, MbFormNote(false, true, true, TestVariables::exactMesh), nullptr );

      miliSec = ::GetTickCountEx() - miliSec;

#ifdef C3D_DEBUG
      CHECK_TIME_TEST();
#endif // C3D_DEBUG

      SetWaitCursor( false );
      TimeTest * testResult = nullptr;

#ifdef C3D_DEBUG
      testResult = ::GetTimeTestResults();
#endif // C3D_DEBUG

      size_t solidsCount( 1 ); 
      size_t facesCount = sol->GetFacesCount();
      size_t pointsCount( 0 );
      size_t trianglesCount( 0 );
      size_t quadranglesCount( 0 );
      if ( (meshItem != nullptr) && (meshItem->IsA() == st_Mesh) ) {
        MbMesh * mesh = (MbMesh *)meshItem;
        pointsCount =  mesh->AllPointsCount();
        trianglesCount = mesh->AllTrianglesCount();
        quadranglesCount = mesh->AllQuadranglesCount();
      }
      ShowTimeTestResult( testResult, solidsCount, facesCount, pointsCount, trianglesCount, quadranglesCount, 
                          miliSec, IDS_BODY_TRIANGULATION );
      //size_t operationCount =  sol->GetCreatorsCount();
      //ShowTimeTestResult( testResult, operationCount, facesCount, miliSec, true, IDS_BODY_TRIANGULATION );

#if defined(C3D_DEBUG)
      SET_TIME_TEST(false);
#endif // C3D_DEBUG

//#ifdef _DRAWGI
      // Нарисовать триангуляцию
      if ( (meshItem != nullptr) && (meshItem->IsA() == st_Mesh) ) {
        MbMesh * mesh = (MbMesh *)meshItem;
        if ( !pathFrom.second.IsSingle() )
          mesh->Transform( pathFrom.second );
        for ( size_t i = 0, icnt = mesh->GridsCount(); i < icnt; i++ ) {
          if ( mesh->GetGrid( i ) != nullptr )
            TestVariables::drawImp->PutGrid( *mesh->GetGrid( i ), TRGB_LIGHTRED );
        }
      }
//#endif // _DRAWGI

      ::DeleteItem( meshItem );
    }

    TestVariables::viewManager->ToggleObject(obj);

    return true;
  }

  return false;

}


//------------------------------------------------------------------------------
// Аппроксимировать грань тела треугольными пластинами
// ---
bool TriangulateFace()
{
  bool res = false;

  MbItem  * obj = nullptr;
  MbTopItem * face = nullptr;

  bool createParams = false;
  {
    int iCreateParams = createParams ? 1 : 0;
    GetInt( IDS_USE_PARAM_POINTS, iCreateParams );
    if ( iCreateParams == 0 )
      createParams = false;
  }
  MbStepData stepData;
  int mode = TestVariables::operateType;
  ::InitStepData( stepData, createParams, mode );

  while ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &face ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    MbFloatGrid grid; // MbExactGrid grid;
    bool seam = true;  // \ru Дублировать точки триангуляции на швах (true) замкнутых поверхностей, не дублировать точки триангуляции на швах (false). \en Flag for not ignore the seam edges. \~
    bool quad = false; // \ru Строить четырёхугольники (true) при триангуляции поверхностей (по возможности). \en Build quadrangles (true) in triangulations of surfaces (if possible). \~
    bool fair = false; // \ru Удалить вырожденные треугольники (true). \en Degenerate triangles removing (if surface has pole). \~
    bool mere = true;  // \ru Использовать специальный алгоритм для плоской поверхности (true). \en Use a special algorithm for a flat surface. \~

    ::CalculateGrid( *((MbFace *)face), stepData, grid, seam, quad, fair, mere );

    TestVariables::viewManager->ToggleObject(obj);

    size_t pointsCount = grid.PointsCount();
    size_t paramsCount = grid.ParamsCount();
    if ( pointsCount == 0 && paramsCount > 0 ) { // Для отрисовки нужны точки, а не параметры
      const MbSurface &surface = ((MbFace *)face)->GetSurface();
      MbCartPoint p;
      MbCartPoint3D pnt;
      MbVector3D nor;
      grid.PointsReserve ( paramsCount );
      grid.NormalsReserve( paramsCount );
      for ( size_t i = 0; i < paramsCount; i++ ) {
        grid.GetParam ( i, p );
        surface._PointOn( p.x, p.y, pnt );
        grid.AddPoint( pnt );
        surface._Normal( p.x, p.y, nor );
        grid.AddNormal( nor );
      }
    }

    TestVariables::drawImp->PutGrid( grid, TRGB_LIGHTRED ); // Нарисовать триангуляцию
    pointsCount = grid.PointsCount();
    size_t normalCount = grid.NormalsCount();
    if ( pointsCount == normalCount ) { 
      MbCartPoint3D pnt, pnt1;
      MbVector3D nor;
      double len = 5 / TestVariables::viewManager->GetActiveWindow()->GetScale();
      for ( size_t i = 0; i < pointsCount; i++ ) {
        grid.GetPoint( i, pnt );
        grid.GetNormal( i, nor );
        pnt1 = pnt;
        pnt1.Add( nor, len ); 
        TestVariables::drawImp->DrawLine( pnt, pnt1, TRGB_BLACK );
      }
    }
    res = true;
  }

  return res;
}

//------------------------------------------------------------------------------
// Построить триангуляцию по облаку точек.
// ---
bool TriangulatePointCloud()  
{
  MbItem  * obj = nullptr;
  if( GetGObj(IDS_SELECT_GEOM_OBJECT, st_Collection, &obj) ) {

    TestVariables::viewManager->ToggleObject(obj);
 
    double rad = 0.0;
    GetDouble( IDS_ENTER_RADIUS, rad );
    double angleGrad = 90.0;
    GetDouble( IDS_ENTER_ANGLE, angleGrad );
    double angleRad = angleGrad * M_PI / 180.0;
    MbMesh * mesh = nullptr;
    ::CalculateBallPivotingGrid( *(MbCollection *)obj, rad, 0.2, angleRad, mesh );

    if ( mesh != nullptr && mesh->IsComplete() ) {
      Style st;
      TestVariables::viewManager->AddObject( st, mesh );
    }
    ::DeleteItem( mesh );
  }
  return true;
}


//------------------------------------------------------------------------------
// Аппроксимировать грань тела треугольными пластинами и нарисовать а области параметров
// ---
bool ParamTriangulateFace()
{
  MbItem  * obj = nullptr;
  MbTopItem * face = nullptr;

  bool createParams = true;
  MbStepData stepData;
  int mode = TestVariables::operateType;
  ::InitStepData( stepData, createParams, mode );

  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &face ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    MbFloatGrid grid; // MbExactGrid grid;
    ::CalculateGrid( *((MbFace *)face), stepData, grid, true, false, false );

    TestVariables::viewManager->ToggleObject(obj);

    const MbSurface & surface = ((MbFace *)face)->GetSurface();
    TestVariables::drawImp->DrawGridMap( grid, surface, TRGB_LIGHTRED ); // Нарисовать триангуляцию

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Упростить триангуляцию поверхности
// ---
bool SimplifySolidTrgn()
{
  MbItem * obj = nullptr;
  PathMatrix pathFrom;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj, pathFrom ) && (obj->IsA() == st_Solid) ) {
    TestVariables::viewManager->ToggleObject(obj);
    MbSolid * solid = static_cast<MbSolid *>(obj);

    bool doWire = false;
    bool doGrid = true;
    double sag = Math::visualSag;

    SPtr<MbItem> meshItem( solid->CreateMesh( MbStepData(ist_SpaceStep, sag), MbFormNote(doWire, doGrid, true, TestVariables::exactMesh), (MbRegDuplicate *)nullptr ) );

    if ( meshItem != nullptr && meshItem->IsA() == st_Mesh ) {
      const MbMesh & mesh = static_cast<MbMesh &>( *meshItem );

      if ( mesh.GridsCount() > 0 ) {
        SArray<MbFloatPoint3D> points( 0, 1 );
        MbFloatPoint3D pnt;
        for ( size_t j = 0, jCount = mesh.GridsCount(); j < jCount; j++ ) {
          const MbGrid * grid = mesh.GetGrid( j );
          if ( grid != nullptr ) {
            points.Reserve( grid->PointsCount() );
            for ( size_t k = 0, kCount = grid->PointsCount(); k < kCount; k++ ) {
              grid->GetPoint(k, pnt);
              points.Add( pnt );
            }
          }
        }
        MbMesh * convexMesh( nullptr );
        ::CreateConvexPolyhedron( points, convexMesh );
        TestVariables::viewManager->SelectObject( obj );
        if ( convexMesh != nullptr ) {
          TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, convexMesh );
          return true;
        }
      }
    }
  }

  return false;
}



//------------------------------------------------------------------------------
// Раскрасить поверхности в отдельном окне
// ---
void SurfaceRendering()
{
  RPArray<MbItem> objs( 10, 10 );
  MbItem * pObj = nullptr;
  while ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &pObj ) ) {
    objs.Add( pObj );
  }
  ScanLine( objs ); // Раскрасить объекты
}


//------------------------------------------------------------------------------
// Раскрасить твердые тела в отдельном окне
// ---
void SolidRendering()
{
  RPArray<MbItem> objs( 10, 10 );
  MbItem * pObj = nullptr;
  while ( GetGObj( IDS_SELECT_SOLID, st_Solid, &pObj ) ) {
    objs.Add( pObj );
  }
  ScanLine( objs ); // Раскрасить объекты

}


//------------------------------------------------------------------------------
// Раскрасить модель в отдельном окне
// ---
void ModelRendering()
{
  RPArray<MbItem> objs( 10, 10 );
  TestVariables::viewManager->GetRenderingObjects( objs ); // Выдать указатели на все объекты
  ScanLine( objs ); // Раскрасить объекты
}


//------------------------------------------------------------------------------
// Развертка кривых на поверхности
// ---
void CurveOnSurfaceMap()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) ) {
    MbeSpaceType family = obj->Type();
    switch ( family ) {
      case st_PlaneInstance : {
        const MbPlaneInstance * pInst = static_cast<const MbPlaneInstance *>(obj);
        const MbPlacement3D & regionPlace = pInst->GetPlacement();
        SPtr<MbPlane> regionPlane( new MbPlane( regionPlace ) );
        const MbPlaneItem * pItem0 = pInst->GetPlaneItem();
        if ( pItem0->Family() == pt_Curve ) {
          const MbCurve * pCurve0 = static_cast<const MbCurve *>( pItem0 );
          TestVariables::drawImp->DrawClearMap();
          TestVariables::drawImp->DrawCurveMap( pCurve0, regionPlane, TRGB_LIGHTRED );
        }
        else if ( pItem0->Family() == pt_Region ) {
          const MbRegion * pRegion0 = static_cast<const MbRegion *>( pItem0 );
          const MbContour * pContour0 = pRegion0->GetOutContour();
          TestVariables::drawImp->DrawClearMap();
          TestVariables::drawImp->DrawCurveMap( pContour0, regionPlane, TRGB_LIGHTRED );
        }
      } break;
      case st_SpaceInstance : {
        const MbSpaceItem * spaceItem = (static_cast<MbSpaceInstance *>(obj))->GetSpaceItem();
        if ( spaceItem != nullptr ) {
          MbeSpaceType type = spaceItem->IsA();
          switch ( type ) {
            case st_SilhouetteCurve :
            case st_SurfaceCurve :
              TestVariables::drawImp->DrawClearMap();
              TestVariables::drawImp->DrawSurfaceCurveMap( static_cast<const MbSurfaceCurve *>(spaceItem), TRGB_LIGHTRED );
            break;
            case st_SurfaceIntersectionCurve :
              TestVariables::drawImp->DrawClearMap();
              TestVariables::drawImp->DrawSurfaceIntersectionMap( static_cast<const MbSurfaceIntersectionCurve *>(spaceItem), TRGB_LIGHTRED, TRGB_LIGHTBLUE );
            break;
            case st_ContourOnSurface :
              TestVariables::drawImp->DrawClearMap();
              TestVariables::drawImp->DrawContourOnSurfaceMap( static_cast<const MbContourOnSurface *>(spaceItem), TRGB_LIGHTRED );
            break;
            default: break;
          }
        }
      } break;
      case st_WireFrame : {
        const MbWireFrame * wire = (MbWireFrame *)obj;
        for ( size_t i = 0, iCount = wire->GetEdgesCount(); i < iCount; i++ ) {
          const MbEdge * edge = wire->GetEdge( i ); 
          if ( edge != nullptr ) {
            const MbCurve3D * spaceItem = &edge->GetCurve();
            MbeSpaceType type = spaceItem->IsA();
            switch ( type ) {
              case st_SilhouetteCurve :
              case st_SurfaceCurve :
                TestVariables::drawImp->DrawClearMap();
                TestVariables::drawImp->DrawSurfaceCurveMap( (MbSurfaceCurve *)spaceItem, TRGB_LIGHTRED );
              break;
              case st_SurfaceIntersectionCurve :
                TestVariables::drawImp->DrawClearMap();
                TestVariables::drawImp->DrawSurfaceIntersectionMap( (MbSurfaceIntersectionCurve *)spaceItem, TRGB_LIGHTRED, TRGB_LIGHTBLUE );
              break;
              case st_ContourOnSurface :
                TestVariables::drawImp->DrawClearMap();
                TestVariables::drawImp->DrawContourOnSurfaceMap( (MbContourOnSurface *)spaceItem, TRGB_LIGHTRED );
              break;
              default: break;
            }
          }
        }
      } break;
      default: break;
    }
    WaitingKeyboardEvent();
  }
}

// Соответствие кривой и поверхности, на которой эта кривая построена
typedef std::pair<c3d::ConstPlaneCurveSPtr, c3d::ConstSurfaceSPtr> ParametricCurveOnSurface;
typedef std::vector<ParametricCurveOnSurface>                      ParametricCurveOnSurfaceVect;


//------------------------------------------------------------------------------
// Сравнение объектов.
// ---
template<class CompareType>
class Comparator
{
private:
  const CompareType & wantedElement;  // Искомый элемент

public:
  // Конструктор. Инициализируется элементом, который в последствие должен быть найден.
  Comparator( const CompareType & wanted ) : wantedElement( wanted ) {};

  // Универсальная функция сравнения.
  bool operator()(const CompareType & otherElem) const
  {
    return Compare( otherElem );
  }

private:
  // Запрет оператора присваивания
  Comparator & operator=( const Comparator & );

  // Функция сравнения для элементов заданного типа.
  bool Compare( const ParametricCurveOnSurface & otherElem ) const
  {
    if ( typeid(wantedElement) == typeid(otherElem) ) {
      const ParametricCurveOnSurface & curElem = static_cast<const ParametricCurveOnSurface &>( wantedElement );
      return ( (curElem.first == otherElem.first) && (curElem.second->IsSimilar(*(otherElem.second))) );
    }
    else
      return false;
  }

  // Функция сравнения для элементов заданного типа.
  bool Compare( const c3d::ConstIntersectionCurveSPtr & otherElem ) const
  {
    if ( typeid(wantedElement) == typeid(otherElem) ) {
      const c3d::ConstIntersectionCurveSPtr & curElem = static_cast<const c3d::ConstIntersectionCurveSPtr &>( wantedElement );
      return curElem->IsSame( *otherElem );
    }
    else
      return false;
  }
};


//------------------------------------------------------------------------------
// Интерфейс активного окна.
// ---
class ActiveWindowsInfo
{
private:
  WorkWindow * activeWindow;  // Активное окно.

public:
  ActiveWindowsInfo( WorkWindow * wnd): activeWindow(wnd) { }
  ~ActiveWindowsInfo()
  {
    if ( activeWindow != nullptr ) {
      activeWindow->ClearArraySelectedElements();
    }
    activeWindow = nullptr;
  }

  const WorkWindow * GetWindow() const { return activeWindow; }
  WorkWindow * ChangeWindow() const { return activeWindow; }

private:
  // Запрет оператора присваивания
  ActiveWindowsInfo & operator=( const ActiveWindowsInfo & );
};


//------------------------------------------------------------------------------
// Добавление заданной кривой в результат пользовательского выбора.
// ---
static
inline void AddPCurve( const ParametricCurveOnSurface &     curveOnSurf,
                             ParametricCurveOnSurfaceVect & arrCurves,
                             MbRect &                       winRect,
                       const bool                           add )     // Добавить или исключить кривую из списка.
{
  if ( (curveOnSurf.first != nullptr) && (curveOnSurf.second != nullptr) ) {
    Comparator<ParametricCurveOnSurface> cmp(curveOnSurf);
    ParametricCurveOnSurfaceVect::iterator foundPtr = std::find_if(arrCurves.begin(), arrCurves.end(), cmp );

    if (arrCurves.empty() || (foundPtr == arrCurves.end()) ) {  // pCurve не была ранее добавлена
      if ( add ) {
        arrCurves.push_back( curveOnSurf );
        curveOnSurf.first->AddYourGabaritTo( winRect );
      }
    }
    else if ( !add ) {
      // Исключаем кривую из результата и пересчитываем габариты оставшихся кривых.
      arrCurves.erase( foundPtr );
      winRect.SetNull();
      for ( foundPtr = arrCurves.begin(); foundPtr != arrCurves.end(); foundPtr++ ) {
        ( (*foundPtr).first )->AddYourGabaritTo( winRect );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Статическая функция обработки плоских кривых, используемая в функции SetCurvesOnSurfaceMap.
// ---
static
bool AddPlanarCurve( const MbPlaneItem &                  pli,
                     const MbPlacement3D &                place,
                           ParametricCurveOnSurfaceVect & arrCurves,
                           MbRect &                       winRect,
                     const bool                           add )
{
  bool retVal = true;
  if ( pli.Family() == pt_Curve ) {
    const c3d::ConstSurfaceSPtr plane( new MbPlane(place) );
    AddPCurve( std::make_pair(c3d::ConstPlaneCurveSPtr(&static_cast<const MbCurve &>(pli)), plane), arrCurves, winRect, add );
  }
  else if ( pli.Family() == pt_Region ) {
    const c3d::ConstSurfaceSPtr plane( new MbPlane(place) );
    const MbRegion & pRegion0 = static_cast<const MbRegion &>( pli );
    for ( size_t i = pRegion0.GetContoursCount(); i--; ) {
      AddPCurve( std::make_pair(c3d::ConstPlaneCurveSPtr(pRegion0.GetContour(i)), plane), arrCurves, winRect, add );
    }
  }
  else
    retVal = false;

  return retVal;
}


//------------------------------------------------------------------------------
// Обработка SurfaceIntersectionCurves.
// ---
static 
bool ProcessSurfaceIntersectionCurves( const MbSurfaceIntersectionCurve &             sic,
                                             c3d::ConstIntersectionCurvesSPtrVector & tmpArrSIC,
                                             ParametricCurveOnSurfaceVect &           arrCurves, 
                                             MbRect &                                 winRect,
                                       const bool                                     add )
{
  c3d::ConstSurfaceSPtr carrierSurf;
  if ( !arrCurves.empty() )
    carrierSurf = arrCurves.cbegin()->second;

  if ( (&(sic.GetSurfaceOne()->GetSurface()) == (&(sic.GetSurfaceTwo()->GetSurface()))) ||
       sic.GetSurfaceOne()->GetSurface().IsSame(sic.GetSurfaceTwo()->GetSurface()) ) { // Обработка шва/линии разъема.
    const MbSurface & surf = sic.GetSurfaceOne()->GetSurface();
    if ( (carrierSurf != nullptr) && (carrierSurf != &surf) && (!carrierSurf->IsSame(surf)) ) // Кривая взята с другой поверхности.
      return false;

    carrierSurf = &surf;
    for ( c3d::ConstIntersectionCurvesSPtrVector::const_iterator it = tmpArrSIC.cbegin(); it != tmpArrSIC.cend(); it++ ) {
      // Ставим faceSense=curveSense=true, т.к. массив tmpArrSIC не содержит швов/линий разъема (на что могут повлиять эти параметры).
      AddPCurve( std::make_pair(c3d::ConstPlaneCurveSPtr((*it)->ChooseCurve(*carrierSurf, true, true)), carrierSurf), arrCurves, winRect, add );
    }

    tmpArrSIC.clear();
    AddPCurve( std::make_pair(c3d::ConstPlaneCurveSPtr(&(sic.GetCurveOne().GetCurve())), carrierSurf), arrCurves, winRect, add );
    if ( sic.GetBuildType() != cbt_Boundary )
      AddPCurve( std::make_pair(c3d::ConstPlaneCurveSPtr(&(sic.GetCurveTwo().GetCurve())), carrierSurf), arrCurves, winRect, add ); // Добавляем вторую кривую шва.
  }
  else if ( carrierSurf == nullptr ) { // С поверхностью еще не определились. Попробуем сопоставить новую кривую с ранее рассмотренными SurfaceIntersectionCurves.
    const MbSurface & surf1 = sic.GetSurfaceOne()->GetSurface();
    const MbSurface & surf2 = sic.GetSurfaceTwo()->GetSurface();
    const MbCurve * pCurve = nullptr;
    for ( c3d::ConstIntersectionCurvesSPtrVector::const_iterator itInersCurves = tmpArrSIC.cbegin(); itInersCurves != tmpArrSIC.cend(); itInersCurves++ ) {
      if ( ((&surf1 == &((*itInersCurves)->GetSurfaceOne()->GetSurface())) && (&surf2 == &((*itInersCurves)->GetSurfaceTwo()->GetSurface()))) ||
           ((&surf1 == &((*itInersCurves)->GetSurfaceTwo()->GetSurface())) && (&surf2 == &((*itInersCurves)->GetSurfaceOne()->GetSurface()))) ||
           ((surf1.IsSame((*itInersCurves)->GetSurfaceOne()->GetSurface()) && surf2.IsSame((*itInersCurves)->GetSurfaceTwo()->GetSurface()))) ||
           ((surf1.IsSame((*itInersCurves)->GetSurfaceTwo()->GetSurface()) && surf2.IsSame((*itInersCurves)->GetSurfaceOne()->GetSurface()))) )
        // Выбрана еще одна кривая пересечения тех же самых пар поверхностей. Определиться с поверхностью пока не получается.
        break;

      if ( (&surf1 == &((*itInersCurves)->GetSurfaceOne()->GetSurface())) || (&surf1 == &((*itInersCurves)->GetSurfaceTwo()->GetSurface())) ||
            surf1.IsSame((*itInersCurves)->GetSurfaceOne()->GetSurface()) ||  surf1.IsSame((*itInersCurves)->GetSurfaceTwo()->GetSurface()) ) {
        carrierSurf = &surf1;
        pCurve = &(sic.GetCurveOne().GetCurve());
        break;
      }

      if ( (&surf2 == &((*itInersCurves)->GetSurfaceOne()->GetSurface())) || (&surf2 == &((*itInersCurves)->GetSurfaceTwo()->GetSurface())) ||
            surf2.IsSame((*itInersCurves)->GetSurfaceOne()->GetSurface()) ||  surf2.IsSame((*itInersCurves)->GetSurfaceTwo()->GetSurface()) ) {
        carrierSurf = &surf2;
        pCurve = &(sic.GetCurveTwo().GetCurve());
        break;
      }
    }

    if ( carrierSurf == nullptr ) { // Так и не определились с поверхностью. Добавляем еще одного претендента в массив.
      if( add )
        tmpArrSIC.push_back( c3d::ConstIntersectionCurveSPtr(&sic) );
      else {
        c3d::ConstIntersectionCurveSPtr wantedSptr( &sic );
        Comparator<c3d::ConstIntersectionCurveSPtr> cmp( wantedSptr );
        c3d::ConstIntersectionCurvesSPtrVector::iterator it = std::find_if( tmpArrSIC.begin(), tmpArrSIC.end(), cmp );
        if ( it != tmpArrSIC.end() )
          tmpArrSIC.erase( it );
      }
    }
    else {
      for ( c3d::ConstIntersectionCurvesSPtrVector::const_iterator it = tmpArrSIC.cbegin(); it != tmpArrSIC.cend(); it++ ) {
        // Ставим faceSense=curveSense=true, т.к. массив tmpArrSIC не содержит швов/линий разъема (на что могут повлиять эти параметры).
        AddPCurve( std::make_pair(c3d::ConstPlaneCurveSPtr((*it)->ChooseCurve(*carrierSurf, true, true)), carrierSurf), arrCurves, winRect, add );
      }
      tmpArrSIC.clear();
      AddPCurve( std::make_pair(c3d::ConstPlaneCurveSPtr(pCurve), carrierSurf), arrCurves, winRect, add );
    }
  }
  else { // Берем кривую с нужной поверхности. Кривая не является швом, т.к. он должен был быть обработан ранее.
    AddPCurve( std::make_pair(c3d::ConstPlaneCurveSPtr(sic.ChooseCurve(*carrierSurf, true, true)), carrierSurf), arrCurves, winRect, add );
  }

  return true;
}


//------------------------------------------------------------------------------
// Обработка случаев выбора пространственных кривых.
// ---
static
bool ProcessSpatialCurves( const MbCurve3D &                              selectedCurve,
                                 c3d::ConstIntersectionCurvesSPtrVector & tmpArrSIC,
                                 ParametricCurveOnSurfaceVect &           arrCurves,
                                 MbRect &                                 winRect,
                           const bool                                     add )
{
  c3d::ConstSurfaceSPtr carrierSurf;
  if ( !arrCurves.empty() )
    carrierSurf = arrCurves.cbegin()->second;

  // Получить 2D-кривые c поверхности
  if ( selectedCurve.IsA() == st_SurfaceIntersectionCurve ) {
    if ( !ProcessSurfaceIntersectionCurves(static_cast<const MbSurfaceIntersectionCurve &>(selectedCurve), tmpArrSIC, arrCurves, winRect, add) )
      return false;
  }
  else {
    c3d::PlaneCurveSPtr pCurve;
    c3d::SurfaceSPtr    surf;

    if ( (selectedCurve.GetSurfaceCurve(pCurve, surf)) && (surf != nullptr) && (pCurve != nullptr) &&
         ((carrierSurf == nullptr) || (surf == carrierSurf) || surf->IsSame(*carrierSurf)) ) {
      carrierSurf = surf;
      AddPCurve( std::make_pair(pCurve, carrierSurf), arrCurves, winRect, add );
    }
  }
  return true;
}


//------------------------------------------------------------------------------
// Развертка множества кривых на поверхности.
// ---
void SetCurvesOnSurfaceMap()
{
  // Массив для временного хранения MbSurfaceIntersectionCurve на случай, если они будут выбраны в самом начале
  // (до того, как мы определились с поверхностью).
  c3d::ConstIntersectionCurvesSPtrVector tmpArrSIC;
  tmpArrSIC.reserve( 1 );

  ParametricCurveOnSurfaceVect arrCurves;

  MbItem * obj = nullptr;
  MbRefItem * edgeCrv = nullptr;

  MbRect winRect;
  bool part = false;

  ActiveWindowsInfo wnd( TestVariables::viewManager->GetActiveWindow() );
  while ( GetCurveObject(IDS_SELECT_GEOM_OBJECT, part, st_Curve3D, st_Solid, tt_CurveEdge, &obj, &edgeCrv) && (obj != nullptr) && (edgeCrv != nullptr) ) {
    // Необходимо добавить или исключить объект.
    const bool add = wnd.ChangeWindow()->ToggleElements( obj, part ? edgeCrv : nullptr );
    if ( (edgeCrv->RefType() != rt_TopItem) || !part ) {
      if ( obj->IsA() == st_SpaceInstance ) {
        const MbSpaceItem * spi( (static_cast<MbSpaceInstance *>(obj))->GetSpaceItem() );
        if ( spi != nullptr ) {
          const MbCurve3D * selectedCurve = static_cast<const MbCurve3D *>( spi );
          if ( selectedCurve != nullptr ) {
            ProcessSpatialCurves( *selectedCurve, tmpArrSIC, arrCurves, winRect, add );
          }
        }
      }
      else if ( obj->IsA() == st_PlaneInstance ) {
        const MbPlaneInstance * pInst = static_cast<const MbPlaneInstance *>( obj );
        const MbPlaneItem * pli = pInst->GetPlaneItem();
        if ( pli != nullptr ) {
          if ( !arrCurves.empty() ) {
            c3d::ConstSurfaceSPtr carrierSurf( arrCurves.cbegin()->second );
            if ( carrierSurf->GetSurface().IsA() == st_Plane ) {
              MbPlane foundPlane( pInst->GetPlacement() );
              if ( foundPlane.IsSimilar(static_cast<const MbPlane &>(*carrierSurf)) ) {
                if ( !AddPlanarCurve(*pli, pInst->GetPlacement(), arrCurves, winRect, add) )
                  continue;
              }
            }
          }
          else if ( !AddPlanarCurve(*pli, pInst->GetPlacement(), arrCurves, winRect, add) )
            continue;
        }
      }
      else if ( obj->IsA() == st_WireFrame ) {
        const MbWireFrame * wf = static_cast<const MbWireFrame *>( obj );
        for ( size_t eIdx = wf->GetEdgesCount(); eIdx--; ) {
          ProcessSpatialCurves( wf->GetEdge( eIdx )->GetCurve(), tmpArrSIC, arrCurves, winRect, add );
        }
      }
    }
    else {
      if ( edgeCrv->RefType() == rt_TopItem ) {
        MbTopItem * ti = static_cast<MbTopItem *>( edgeCrv );
        if ( (ti->IsA() == tt_Edge) || (ti->IsA() == tt_CurveEdge) ) {
          if ( !ProcessSurfaceIntersectionCurves(static_cast<MbCurveEdge *>(edgeCrv)->GetIntersectionCurve(), tmpArrSIC, arrCurves, winRect, add) )
            continue;
        }
      }
    }
  }

  TestVariables::viewManager->UnselectModel();
  TestVariables::ResetHLighted();

  if ( !tmpArrSIC.empty() ) {
    // С поверхностью определиться не удалось (т.е. пользователь выбрал только кривые пересечения одних и тех же пар поверхностей).
    // В этом случае номер поверхности спрашиваем у пользователя.
    const int surfId = GetBool(IDS_BOOL_USE_SURFACE_ONE, true) ? 0 : 1;

    const c3d::ConstSurfaceSPtr carrierSurf ( &((*(tmpArrSIC.cbegin()))->GetSurface(surfId)) );
    for ( c3d::ConstIntersectionCurvesSPtrVector::const_iterator itC = tmpArrSIC.cbegin(); itC != tmpArrSIC.cend(); itC++ ) {
      // Ставим faceSense=curveSense=true, т.к. массив tmpArrSIC не содержит швов/линий разъема (на что могут повлиять эти параметры).
      AddPCurve( std::make_pair(c3d::ConstPlaneCurveSPtr((*itC)->ChooseCurve(*carrierSurf, true, true)), carrierSurf), arrCurves, winRect, true );
    }

    tmpArrSIC.clear();
  }

  // Отображаем кривые на текущем экранном виде
  if ( !arrCurves.empty() ) {
    // Добавить габарит поверхности к уже вычисленному габариту.
    MbRect localRect;
    const c3d::ConstSurfaceSPtr carrierSurf( arrCurves.cbegin()->second );
    carrierSurf->GetRect( localRect );
    winRect |= localRect;

    TestVariables::ResetHLighted();
    const bool isIsotropic = !GetBool(IDS_BOOL_LOCK_ASPECT_RATIO, true);
    
    // Отрисовка кривых
    TestVariables::drawImp->DrawClearMap();
    for ( ParametricCurveOnSurfaceVect::iterator it = arrCurves.begin(); it != arrCurves.end(); it++ ) {
      TestVariables::drawImp->DrawCurveMap( ::DetachItem(it->first), carrierSurf, winRect, TRGB_RED,
                                            1, 2, isIsotropic, it == arrCurves.begin() );
    }
  }
  
  arrCurves.clear();
}


//------------------------------------------------------------------------------
// Развертка поверхности
// ---
void SurfaceBorderMap()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &obj ) ) {
    TestVariables::drawImp->DrawClearMap();
    if ( obj->Type() == st_SpaceInstance ) {
      const MbSpaceItem * spaceItem = ((MbSpaceInstance *)obj)->GetSpaceItem();
      if ( spaceItem != nullptr ) {
        if ( spaceItem->IsA() == st_CurveBoundedSurface )
          TestVariables::drawImp->DrawCurveBoundedSurfaceMap( (MbCurveBoundedSurface *)spaceItem, TRGB_LIGHTRED );
        else
          TestVariables::drawImp->DrawSurfaceMap( (MbSurface *)spaceItem, TRGB_LIGHTRED );
      }

      WaitingKeyboardEvent();
    }
  }
}


//------------------------------------------------------------------------------
// Развертка граней твердого тела
// ---
void SolidFaceLoopMap()
{
  MbItem * obj = nullptr;
  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {
    const MbSolid * sol = static_cast<const MbSolid *>(obj);

    if ( sol != nullptr ) {
      for ( size_t k = 0, facesCnt = sol->GetFacesCount(); k < facesCnt; k++ ) {
        const MbFace * face = sol->GetFace( k );
        FaceLoopMap( face );
        if ( WaitingKeyboardEvent() )
          break; // Если нажата клавиша [Esc], прекратить отрисовку
      }
    }
  }
}


//------------------------------------------------------------------------------
// Развертка грани твердого тела
// ---
bool FaceLoopMap( const MbFace * face )
{
  if ( face == nullptr ) {
    MbItem * obj = nullptr;
    MbTopItem * tItem = nullptr;

    if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &tItem ) && tItem != nullptr && tItem->IsAFace() )
      face = static_cast<const MbFace *>( tItem );
  }

  if ( face != nullptr ) {
    TestVariables::drawImp->DrawClearMap();

    const size_t loopsCnt = face->GetLoopsCount();

    if ( loopsCnt > 0 ) { // Нужно сравнить развертку по ребрам с разверткой по границам поверхности
      const MbSurface * surf = &face->GetSurface();    
      if ( surf->IsA() == st_CurveBoundedSurface )
        TestVariables::drawImp->DrawCurveBoundedSurfaceMap( (MbCurveBoundedSurface *)surf, TRGB_LIGHTRED );
      else
        TestVariables::drawImp->DrawSurfaceMap( surf, TRGB_LIGHTRED );

      // если нужна задержка
      // WaitingKeyboardEvent();

      /* BUG_83313
      for ( size_t k = 0; k < loopsCnt; k++ ) {
        const MbLoop * loop = face->_GetLoop( k );
        if ( loop != nullptr && loop->GetEdgesCount() > 0 ) {
          SPtr<MbContourOnSurface> loopBound( &loop->MakeContourOnSurface( face->GetSurface().GetSurface(), face->IsSameSense() ) );
          drawImp->DrawContourOnSurfaceMap( loopBound, TRGB_LIGHTBLUE );
        }
        else {
          PRECONDITION( false );
        }
        if ( WaitingKeyboardEvent() )
          break; // Если нажата клавиша [Esc], прекратить отрисовку
      } // */
      TestVariables::drawImp->DrawFaceMap( face, TRGB_LIGHTBLUE );

      WaitingKeyboardEvent();
    }
    else {
      const MbSurface * surf = &face->GetSurface();    
      if ( surf->IsA() == st_CurveBoundedSurface )
        TestVariables::drawImp->DrawCurveBoundedSurfaceMap( (MbCurveBoundedSurface *)surf, TRGB_LIGHTRED );
      else
        TestVariables::drawImp->DrawSurfaceMap( surf, TRGB_LIGHTRED );
      WaitingKeyboardEvent();
    }

    return true;
  }
  return false;
}


/*
//------------------------------------------------------------------------------
// Расчёт точек около вершин грани на расстоянии distance
// ---
static void CalcGridPoints( const MbFace & face, SArray<MbCartPoint3D> & points )
{
  size_t loopsCount = face.GetLoopsCount();

  if (loopsCount>0) {
    MbSurface & surface = face.GetSurface();
    bool faceSense = face.IsSameSense();
    MbCartPoint r1;
    MbVector tau1, tau2;
    MbCartPoint3D p;

    // Расчёт расстояния
    double diagonal = 0; // Диагональ габарита внешнего цикла
    double koeff = 0.05; // Коэффициент смещения
    MbLoop *loop = face.GetLoop( 0 ); // Внешний цикл
    if ( loop!=nullptr ) {
      MbRect rect;
      rect.SetEmpty();
      loop->GetGabarit( surface.GetSurface(), faceSense, rect );
      diagonal = rect.GetDiagonal(); 
    }

    for ( size_t k=0; k<loopsCount; k++ ) {
      MbLoop *loop = face.GetLoop( k );

      if ( loop!=nullptr ) {
        double d = diagonal;

        if ( k > 0 ) { // Внутренний цикл
          MbRect rect0;
          rect0.SetEmpty();
          loop->GetGabarit( surface.GetSurface(), faceSense, rect0 );
          d = fabs(diagonal - rect0.GetDiagonal()); 
        }

        double distance = d * koeff; // Смещение в параметрической области на величину

        for ( size_t i=0,segmentsCount=loop->GetEdgesCount(); i<segmentsCount; i++ ) {
          size_t i1 = (segmentsCount+i-1) % segmentsCount, i2 = i;
          MbCurveEdge & elem1 = loop->GetOrientedEdge( i1 )->GetCurveEdge();
          MbCurveEdge & elem2 = loop->GetOrientedEdge( i2 )->GetCurveEdge();
          bool orient1 = loop->GetOrientedEdge( i1 )->GetOrientation();
          bool orient2 = loop->GetOrientedEdge( i2 )->GetOrientation();
          MbCurve *seg1 = elem1.ChooseCurve( &face, orient1 ); // Предшествующая вершине кривая на поверхности грани 
          MbCurve *seg2 = elem2.ChooseCurve( &face, orient2 ); // Следующая за вершиной кривая на поверхности грани

          if ( seg1 && seg2 ) {
            bool sense1 = (orient1 == elem1.IsSameSense()); // Ориентация предшествующей вершине кривой
            bool sense2 = (orient2 == elem2.IsSameSense()); // Ориентация следующей за вершиной кривой
            double t1 = sense1 ? seg1->GetTMax() : seg1->GetTMin();  
            double t2 = sense2 ? seg1->GetTMin() : seg2->GetTMax();
            seg1->FirstDer( t1, tau1 );
            seg2->FirstDer( t2, tau2 );
            if ( !sense1 ) // Касательный вектор ориентируем по циклу (к вершине)
              tau1.Invert();
            if ( !sense2 ) // Касательный вектор ориентируем по циклу (от вершины)
              tau2.Invert();
            tau1.Normalize(); // Нормируем
            tau2.Normalize(); // Нормируем
            // Поворачиваем внутрь цикла
            tau1.Perpendicular(); 
            tau2.Perpendicular(); 
            if (!faceSense) {
              tau1.Invert();
              tau2.Invert();
            }
            tau1 += tau2; // Вектор направлен примерно по биссектрисе внутрь грани
            tau1.Normalize(); // Нормируем
            tau1 *= distance; 
            seg1->PointOn( t1, r1 ); // Точка в вершине
            r1 += tau1; // Двумерная точка, смещённая от вершины на величину distance внутрь грани

            surface._PointOn( r1.x, r1.y, p ); // Трёхмерная точка, смещённая от вершины на некоторую величину примерно внутрь грани

            points.Add( p );
            
            if ( segmentsCount == 1 ) { // Одно ребро, берём точку в середине 
              t1 = 0.5 * (seg1->GetTMax() + seg1->GetTMin());  
              seg1->FirstDer( t1, tau1 );
              if ( sense1 != faceSense ) // Касательный вектор ориентируем по циклу (к вершине)
                tau1.Invert();
              tau1.Normalize(); // Нормируем
              // Поворачиваем внутрь цикла
              tau1.Perpendicular(); 
              tau1 *= distance; 
              seg1->PointOn( t1, r1 ); // Точка в вершине
              r1 += tau1; // Двумерная точка, смещённая от вершины на величину distance внутрь грани

              surface._PointOn( r1.x, r1.y, p ); // Трёхмерная точка, смещённая от вершины на некоторую величину примерно внутрь грани
              points.Add( p );
            }
          }
        }
      }
    }
  }
}
*/


//------------------------------------------------------------------------------
// Построение характеристической ломаной NURBS
// ---
void LineSegmentNURBSCurve()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &obj ) && (obj->IsA() == st_SpaceInstance) ) {
    MbCurve3D * curv = (MbCurve3D *)((MbSpaceInstance *)obj)->GetSpaceItem();
    if (curv != nullptr && (curv->IsA()==st_Nurbs3D || curv->IsA()==st_Bezier3D || curv->IsA()==st_Hermit3D) ) {
      MbPolyCurve3D *poly = (MbPolyCurve3D *)curv;
      PArray <MbCurve3D> segments(poly->GetPointsCount(),2,false);//true);
      poly->GetLineSegments( segments );
      for ( ptrdiff_t i = 0; i < (ptrdiff_t)segments.Count(); i++ )
        TestVariables::viewManager->AddObject( TestVariables::TEXT_Style, segments[i] );
    }
    else
      MessageBoxEx( RT_OBJECT_NAS_NO_POLYCURVE, RT_INACCURACY );
  }
}


//------------------------------------------------------------------------------
// Построение характеристической ломаной NURBS
// ---
void LineSegmentNURBSSurface()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &obj ) ) {
    const MbSpaceItem * spaceItem = ((MbSpaceInstance *)obj)->GetSpaceItem();
    if ( spaceItem != nullptr  ) {
      MbSurface * surf = (MbSurface *)spaceItem;
      PArray <MbCurve3D> segments(8,2,false);//true);
      /*
      if (surf->IsA()==st_SplineSurface || surf->IsA()==st_BezierSurface ||
          surf->IsA()==st_TriBezierSurface || surf->IsA()==st_SimplexSurface ) {

        if ( surf->IsA()==st_TriBezierSurface )
          ((MbTriBezierSurface *)surf)->GetLineSegments( segments );
        else
        if ( surf->IsA()==st_SimplexSurface )
          ((MbSimplexSurface *)surf)->GetLineSegments( segments );
        else
          ((MbPolySurface *)surf)->GetLineSegments( segments );

        for ( ptrdiff_t i = 0; i < (ptrdiff_t)segments.Count(); i++ )
          viewManager->AddObject( TEXT_Style, segments[i] );
      }
      */
      if ( ::GetLineSegmentNURBSSurface(*surf, segments) ) 
      {
        for ( ptrdiff_t i = 0; i < (ptrdiff_t)segments.Count(); i++ )
          TestVariables::viewManager->AddObject( TestVariables::TEXT_Style, segments[i] );
      }
      else
        MessageBoxEx( RT_OBJECT_NAS_NO_POLYCURVE, RT_INACCURACY );
    }
  }
}



//------------------------------------------------------------------------------
// Построение вокселей.
// ---
//static 
void MakeVoxels( const MbItem & item, int n ) 
{
  if ( n > 2 && n <= TESSERA_MAX ) {
    MbCube r;
    item.AddYourGabaritTo( r );
    double x = r.GetLengthX();
    double y = r.GetLengthY();
    double z = r.GetLengthZ();
    double a = ::pow( x * y * z, ONE_THIRD ) / n;
    if ( a > EPSILON ) {
      size_t iCount = (size_t)(x / a) + 1;
      size_t jCount = (size_t)(y / a) + 1;
      size_t kCount = (size_t)(z / a) + 1;
      MbVector3D dx( x / iCount, 0.0, 0.0 );
      MbVector3D dy( 0.0, y / jCount, 0.0 );
      MbVector3D dz( 0.0, 0.0, z / kCount );
      MbCartPoint3D p, p0, p1, p2, p3;
      SpacePointsVector points;
      Style st( item.GetWidth(), item.GetColor() );
      MbFaceShell * faces = new MbFaceShell();
        
      a *= TWO_THIRD;
      r.GetVertex( 0, p );
      for ( size_t k = 0; k < kCount+1; k++ ) {
        for ( size_t j = 0; j < jCount+1; j++ ) {
          for ( size_t i = 0; i < iCount+1; i++ ) {
            p0 = p;
            p0.Add( dx, (double)i, dy, (double)j, dz, (double)k ); 
            double d = item.DistanceToPoint( p0 );
            if ( d < a ) {
              p0.Add( dx, -ONE_HALF, dy, -ONE_HALF, dz, -ONE_HALF ); 
              p1.Set( p0, dx, 1.0 ); 
              p2.Set( p0, dy, 1.0 ); 
              p3.Set( p0, dz, 1.0 ); 
              points.clear();
              points.push_back( p0 );
              points.push_back( p1 );
              points.push_back( p2 );
              points.push_back( p3 );
              MbSNameMaker names( (SimpleName)(i*j*k), MbSNameMaker::i_SideNone, 0 );
              MbElementarySolidResults results;
              ::ElementarySolid( MbElementarySolidParams( et_Block, points, names ), results ); // ::ElementarySolid( points, et_Block, names, result );
              
              if ( results.GetResultSolid() != nullptr )
                faces->UnionWith( *(results.GetResultSolid()->GetShell()));
            }
          }
        }
      }
      if ( faces->GetFacesCount() > 0 ) {
        TestVariables::viewManager->AddObject( st, new MbSolid( faces, nullptr ) );
      }
      ::DeleteItem( faces );
    }
  }
}


//------------------------------------------------------------------------------
// Построение каркаса
// ---
void MakeSpaceWire()
{
  MbItem * obj = nullptr;
  PathMatrix pathFrom;

  if ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj, pathFrom ) ) {
    TestVariables::viewManager->ToggleObject(obj);
    {
//      int n( 10 );
//      ::GetInt( IDS_ENTER_BRICK_COUNT_SIDE, n ); 
//      ::MakeVoxels( *obj, n ); // Построение вокселей. 
    }
    PArray<MbCurve3D> wire(50,50,false);
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    if ( wind != nullptr ) {
      wind->GetSpaceWire( obj, wire );
    }
    if ( wire.Count() > 0 ) {
      Style st( obj->GetWidth(), obj->GetColor() );
      TestVariables::viewManager->DeleteObject( obj );
      for ( ptrdiff_t i = 0; i < (ptrdiff_t)wire.Count(); i++ )
        TestVariables::viewManager->AddObject( st, wire[i] );
    }
    else
      MessageBoxEx( RT_OBJECT_HAS_NOT_GRID, RT_INACCURACY );
  }
}


//------------------------------------------------------------------------------
// Построение плоского каркаса
// ---
void MakePlaneWire()
{
  MbItem * obj = nullptr;

  if ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);
    PArray <MbCurve3D> wire(50,50,false);
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    if ( wind != nullptr )
      wind->GetPlaneWire( obj, wire );
    if (wire.Count()>0) {
      Style st( obj->GetWidth(), obj->GetColor() );
      TestVariables::viewManager->DeleteObject( obj );
      for ( ptrdiff_t i = 0; i < (ptrdiff_t)wire.Count(); i++ )
        TestVariables::viewManager->AddObject( st, wire[i] );
    }
    else
      MessageBoxEx( RT_OBJECT_HAS_NOT_GRID, RT_INACCURACY );
  }
}


//------------------------------------------------------------------------------
// Построение каркаса на плоскости
// ---
void MakeWireOnPlane()
{
  if ( TestVariables::viewManager->GetActiveWindow() == nullptr )
    return;

  MbItem * obj = nullptr;
  if ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);
    PArray<MbCurve> wire(50,50,false);
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    if ( wind != nullptr )
      wind->GetPlaneWire( obj, wire );
    if (wire.Count()>0) {
      Style st( obj->GetWidth(), obj->GetColor() );
      TestVariables::viewManager->DeleteObject( obj );
      MbPlacement3D pl( wind->GetOwnPlace() );
      for ( ptrdiff_t i = 0; i < (ptrdiff_t)wire.Count(); i++ )
        TestVariables::viewManager->AddObject( st, wire[i], &pl );
    }
    else
      MessageBoxEx( RT_OBJECT_HAS_NOT_GRID, RT_INACCURACY );
  }
}


//------------------------------------------------------------------------------
// Построение каркаса из линий триангуляции
// ---
void MakeTriangulationPoints()
{
  MbItem * obj = nullptr;

  if ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);
    RPArray<MbPointFrame> points( 0, 1 );
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    if ( wind != nullptr )
      wind->GetTriangulationPoints( obj, points );
    if ( points.size() > 0 ) {
      Style st( obj->GetWidth(), obj->GetColor() );
      for ( size_t i = 0, icnt = points.size(); i < icnt; i++ )
        TestVariables::viewManager->AddObject( st, points[i], false );
      TestVariables::viewManager->ShowModel();
    }
    else
      MessageBoxEx( RT_OBJECT_HAS_NOT_GRID, RT_INACCURACY );
  }
}


//------------------------------------------------------------------------------
// Построение каркаса из линий триангуляции
// ---
void MakeTriangulationWire()
{
  MbItem * obj = nullptr;

  if ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);
    RPArray<MbWireFrame> wires( 0, 1 );
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    if ( wind != nullptr )
      wind->GetTriangulationWires( obj, wires );
    if ( wires.size() > 0 ) {
      Style st( obj->GetWidth(), obj->GetColor() );
      for ( size_t i = 0, icnt = wires.size(); i < icnt; i++ )
        TestVariables::viewManager->AddObject( st, wires[i], false );
      TestVariables::viewManager->ShowModel();
    }
    else
      MessageBoxEx( RT_OBJECT_HAS_NOT_GRID, RT_INACCURACY );
  }
}


//------------------------------------------------------------------------------
// Построение каркаса из пластин триангуляции
// ---
void MakeTriangulationPlates()
{
  MbItem * obj = nullptr;

  if ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);
    RPArray<MbSpaceItem> plates( 0, 1 );
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    if ( wind != nullptr )
      wind->GetTriangulationPlates( obj, plates );
    if ( plates.size() > 0 ) {
      Style st( obj->GetWidth(), obj->GetColor() );
      for ( size_t i = 0, icnt = plates.size(); i < icnt; i++ )
        TestVariables::viewManager->AddObject( st, plates[i], false );
      TestVariables::viewManager->ShowModel();
    }
    else
      MessageBoxEx( RT_OBJECT_HAS_NOT_GRID, RT_INACCURACY );
  }
}


//------------------------------------------------------------------------------
// Без привязки к телу
// ---
void CreateNewThreadInSpace()
{
  MbPlacement3D place;

  int iEnterPlace = 0;
  GetInt( IDS_ENTER_USE_OR_NOT, iEnterPlace );

  bool bEnterPlace = (iEnterPlace > 0);
  if ( bEnterPlace ) {
    MbCartPoint3D pOrigin, pAxisZ, pAxisX;

    if ( GetPoint( IDS_SELECT_POINT, pOrigin ) ) {
//#ifdef _DRAWGI
      TestVariables::drawImp->DrawPoint( &pOrigin, TRGB_LIGHTGREEN );
//#endif // _DRAWGI

      if ( GetPoint( IDS_SELECT_POINT, pAxisZ ) ) {
//#ifdef _DRAWGI
        TestVariables::drawImp->DrawPoint( &pAxisZ, TRGB_LIGHTGREEN );
//#endif // _DRAWGI

        if ( GetPoint( IDS_SELECT_POINT, pAxisX ) ) {
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawPoint( &pAxisX, TRGB_LIGHTGREEN );
//#endif // _DRAWGI

          int iLeftPlace = 0;
          GetInt( IDS_ENTER_PLACE_TYPE, iLeftPlace );

          MbVector3D axisZ( pOrigin, pAxisZ );
          MbVector3D axisX( pOrigin, pAxisX );
          bool bLeft = (iLeftPlace != 0);
          place.Init( pOrigin, axisZ, axisX, bLeft );
          bEnterPlace = false;
        }
      }
    }
    if ( bEnterPlace )
      return;
  }

  double radBeg = 50.0;
  GetDouble( IDS_ENTER_RADIUS, radBeg );
  double radEnd = 40.0;
  GetDouble( IDS_ENTER_RADIUS, radEnd );
  double length = 100.0;
  GetDouble( IDS_ENTER_THREAD_LENGTH, length );
  double angleGrad = 0.0;
  GetDouble( IDS_ENTER_ANGLE, angleGrad );
  double angleRad = angleGrad * M_PI / 180.0;

  MbThread * thr = new MbThread( place, radBeg, radEnd, length, angleRad );
  TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, thr );
}


//------------------------------------------------------------------------------
// Сопрягаются ли резьбы
// ---
void ModifyThreadIsMatedThreads()
{
  MbItem * obj1 = nullptr;
  if ( GetGObj( IDS_SELECT_THREAD, st_Thread, &obj1 ) ) {
    TestVariables::viewManager->ToggleObject( obj1 );
    MbItem * obj2 = nullptr;
    if ( GetGObj( IDS_SELECT_THREAD, st_Thread, &obj2 ) ) {
      TestVariables::viewManager->ToggleObject( obj2 );

      const MbThread * thr1 = static_cast<const MbThread *>(static_cast<const MbSpaceInstance *>(obj1)->GetSpaceItem());
      const MbThread * thr2 = static_cast<const MbThread *>(static_cast<const MbSpaceInstance *>(obj2)->GetSpaceItem());
      TCHAR msgTitle[] = IDS_TITLE_MSG;
      TCHAR msgResult[64];

      static bool collectAllErrors = true;

      MbThreadedJointCheckParameters checkParams;
      MbThread::ThreadedJointErrors thrJointErrors;
      if ( collectAllErrors ) {
        checkParams.SetRotationsMatchingCheck( true );
        checkParams.SetCollectErrors( true );
      }
      if ( thr1->IsMatedTo( *thr2, checkParams, &thrJointErrors ) )
        _sntprintf( msgResult, 64, RT_THREADS_CONJUGATE );
      else if ( thr1->IsSame( *thr2) )
        _sntprintf( msgResult, 64, RT_THREADS_IDENTICAL );
      else {
        _sntprintf( msgResult, 64, RT_THREADS_NOT_CONJUGATE );
      }
      MessageBoxEx( msgResult, msgTitle );
    }
  }
}

//------------------------------------------------------------------------------
// Принадлежит ли резьба телу
// ---
void ModifyThreadIsBodyThread()
{
  MbItem * objThr = nullptr;
  if ( GetGObj( IDS_SELECT_THREAD, st_Thread, &objThr ) ) {
    MbItem * objSol = nullptr;
    if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &objSol ) ) {
      MbThread * thr = (MbThread *)((MbSpaceInstance *)objThr)->GetSpaceItem();
      MbSolid  * sol = (MbSolid  *)objSol;
      MbMatrix3D matrFrom;
      bool bIsBodyThread = thr->IsBodyThread( *sol, matrFrom );
      TCHAR msgTitle[] = IDS_TITLE_MSG;
      if ( bIsBodyThread )
      {  
        MessageBoxEx( RT_THREAD_ON_BODY, msgTitle );
      }  
      else
      {  
        MessageBoxEx( RT_THREAD_NOT_ON_BODY, msgTitle );
      }  
    }
  }
  // in progress
}


//------------------------------------------------------------------------------
// 
// ---
inline
const MbSpaceItem * GetSpaceItem( const MbRefItem & refItem )
{
  const MbSpaceItem * result = nullptr;

  MbeRefType refType = refItem.RefType();

  switch ( refType ) {
    case rt_TopItem : {
      const MbTopItem & topItem = static_cast<const MbTopItem &>( refItem );
      if ( topItem.IsAWireEdge() || topItem.IsAnEdge() ) {
        const MbEdge & edge = static_cast<const MbEdge &>( topItem );
        result = &edge.GetCurve();
      }
      else if ( topItem.IsAFace() ) {
        const MbFace & face = static_cast<const MbFace &>( topItem );
        result = &face.GetSurface();
      }
    } break;
    case rt_SpaceItem : {
      const MbSpaceItem & spaceItem = static_cast<const MbSpaceItem &>( refItem );
      MbeSpaceType groupType = spaceItem.Family();
      if ( groupType == st_Curve3D )
        result = &spaceItem;
      else if ( groupType == st_Surface )
        result = &spaceItem;
      else if ( groupType == st_Item  ) {
        MbeSpaceType isA = spaceItem.IsA();
        if ( isA == st_Solid )
          result = &spaceItem;
        else if ( isA == st_Point3D )
          result = &spaceItem;
        else if ( isA == st_PointFrame ) {
          result = &spaceItem;
        }
      }
    } break;
    case rt_PlaneItem : {
      PRECONDITION( false );
    } break;
    default:
      break;
  }

  return result;
}


//------------------------------------------------------------------------------
// Адаптировать резьбу к телу
// ---
void ModifyThreadAdaptToBody()
{
  MbItem * objThr = nullptr;
  PathMatrix thrPathFrom;
  if ( GetGObj( IDS_SELECT_THREAD, st_Thread, &objThr, thrPathFrom ) && objThr ) {
    TestVariables::viewManager->ToggleObject( objThr );
    PRECONDITION( thrPathFrom.second.IsSingle() );
    
    MbItem * objBody = nullptr;
    PathMatrix bodyPathFrom;
    if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &objBody, bodyPathFrom ) && objBody && (objBody->IsA() == st_Solid) ) {
      TestVariables::viewManager->ToggleObject( objBody );
      MbThread * thr = static_cast<MbThread *>( static_cast<MbSpaceInstance *>(objThr)->SetSpaceItem() );
      const MbSolid * body = static_cast<MbSolid *>( objBody );

      MbItem * begItem = nullptr;
      MbItem * endItem = nullptr;

      MbThread::ThreadLimiter begLimiter( begItem, MbMatrix3D::identity );
      MbThread::ThreadLimiter endLimiter( endItem, MbMatrix3D::identity );

      static bool selectThreadLimiters = true;

      PathMatrix begPathFrom;
      PathMatrix endPathFrom;

      if ( selectThreadLimiters ) {
        MbRefItem * refItem = nullptr;
        if ( GetObj( IDS_SELECT_OBJECT, &begItem, &refItem, begPathFrom ) && begItem ) {
          if ( refItem == nullptr )
            begLimiter = std::make_pair( begItem, begPathFrom.second );
          else {
            begLimiter.first = ::GetSpaceItem( *refItem );
            begLimiter.second = begPathFrom.second;
          }
        }
        refItem = nullptr;
        if ( GetObj( IDS_SELECT_OBJECT, &endItem, &refItem, endPathFrom ) && endItem ) {
          if ( refItem == nullptr )
            endLimiter = std::make_pair( endItem, endPathFrom.second );
          else {
            endLimiter.first = ::GetSpaceItem( *refItem );
            endLimiter.second = endPathFrom.second;
          }
        }
      }

      MbThread::ThreadLimiters thrLimiters( begLimiter, endLimiter );

      MbeThrAdapt thrAdapt = ta_UsingSolid; //ta_UsingSurfaces; //ta_UsingGabarit;

      bool bAdapt = thr->AdaptToBody( *body, bodyPathFrom.second, thrAdapt, &thrLimiters );

      TCHAR msgTitle[] = IDS_TITLE_MSG;
      if ( bAdapt ) {
        TestVariables::viewManager->RedrawObject( objThr );
        ::MessageBoxEx( RT_THREAD_ON_BODY_AND_ADAPTED_TO_IT, msgTitle );
      }
      else
        ::MessageBoxEx( RT_THREAD_NOT_ON_BODY_OR_CAN_NOT_ADAPT, msgTitle );

      TestVariables::viewManager->ToggleObject( objBody );
    }
    TestVariables::viewManager->ToggleObject( objThr );
  }
}


//------------------------------------------------------------------------------
// 
// ---
inline bool SetThreadOrigin( const MbContour & cntr, MbPlacement3D & placeThr )
{
  if ( cntr.GetSegmentsCount() == 1 ) {
    const MbCurve & curv = cntr.GetSegment( 0 )->GetSubstrate();
    if ( curv.IsA() == pt_Arc && curv.IsClosed() ) {
      MbCartPoint wc2d;
      curv.GetWeightCentre( wc2d );
      MbCartPoint3D wc3d;
      placeThr.PointOn( wc2d, wc3d );
      if ( !c3d::EqualPoints( wc3d, placeThr.GetOrigin(), METRIC_PRECISION ) ) {
        placeThr.SetOrigin( wc3d );
        return true;
      }
    }
  }
  return false;
}

//------------------------------------------------------------------------------
// Установить начало плоскости резьбы при использовании ограниченной кривыми поверхности
// ---
static void SetThreadOrigin( const MbSurface & surf, MbPlacement3D & placeThr )
{
  if ( surf.IsA() == st_CurveBoundedSurface ) {
    const MbCurveBoundedSurface & surfBnd = (const MbCurveBoundedSurface &)surf;
    const ptrdiff_t cntCurves = surfBnd.GetCurvesCount();
    const MbContourOnSurface * cntrOnSurf = surfBnd.GetCurve( 0 );
    const MbContour * cntr = &cntrOnSurf->GetContour();

    bool bSet = ::SetThreadOrigin( *cntr, placeThr );

    if ( !bSet && cntCurves == 2 ) {
      cntrOnSurf = surfBnd.GetCurve( 1 );
      cntr = &cntrOnSurf->GetContour();
      bSet = ::SetThreadOrigin( *cntr, placeThr );
    }

    if ( bSet ) {
      MessageBoxEx( RT_NOT_IMPLEMENTED, IDS_TITLE_MSG );
    }
  }
}


//------------------------------------------------------------------------------
// С привязкой к телу
// ---
void CreateNewThreadOnSolid()
{
  TCHAR msgTitle[] = IDS_TITLE_MSG;
  MbItem * objSol = nullptr;
  MbTopItem * itemObj = nullptr;

  // Попросим указать поверхность резьбы
  if ( GetTObj(IDS_SELECT_CYLCONE_FACE, st_Solid, tt_Face, &objSol, &itemObj) ) {
    MbSolid * solid = static_cast<MbSolid *>(objSol);
    MbFace * faceObj = static_cast<MbFace *>(itemObj); // Несущая грань для резьбы
    const MbSurface & surfObj = faceObj->GetSurface().GetSurface();
    MbeSpaceType typeObj = surfObj.IsA();

    if ( typeObj == st_CylinderSurface ||  typeObj == st_ConeSurface ) {
      bool isCylinder = (typeObj == st_CylinderSurface);

      MbPlacement3D placeObj;
      double objRadius; // objHeight;

      MbAxis3D rotAxis;
      surfObj.GetCylinderAxis( rotAxis );
      
      if ( isCylinder ) {
        const MbCylinderSurface & cylObj = static_cast<const MbCylinderSurface &>(surfObj);
        placeObj.Init( cylObj.GetPlacement() );
        objRadius = cylObj.GetRadius();
        //objHeight = cylObj.GetHeight();
      }
      else {
        const MbConeSurface & conObj = static_cast<const MbConeSurface &>(surfObj);
        placeObj.Init( conObj.GetPlacement() );
        objRadius = conObj.GetRadius();
        //objHeight = conObj.GetHeight();
      }

      // Плейсмент начала резьбы
      MbPlacement3D placeThr;
      bool isPlaceThr = false;

      int iBuildFrom = 2;
      GetInt( IDS_ENTER_BUILD_FROM_PFE, iBuildFrom );

      if ( iBuildFrom < 0 || iBuildFrom > 2 )
        iBuildFrom = 0;

      switch ( iBuildFrom )
      {
        case 0 : {
          MbItem * objSurf = nullptr;
          if ( GetGObj( IDS_SELECT_ORTO_PLANE, st_Plane, &objSurf ) ) {
            const MbSpaceItem * sItem = (static_cast<const MbSpaceInstance *>(objSurf))->GetSpaceItem(); // Дать трёхмерный геометрический объекта
            if ( (sItem != nullptr) && sItem->IsA() == st_Plane ) {
              placeThr.Init( (static_cast<const MbPlane *>(sItem))->GetPlacement() );
              if ( placeThr.GetAxisZ().Colinear( placeObj.GetAxisZ() ) )
                isPlaceThr = true;
            }
          }
          break;
        }
        case 1 : {
          MbItem * objFace = nullptr;
          MbTopItem * itemFace = nullptr;
          if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &objFace, &itemFace ) && (objSol == objFace) ) {
            MbFace * face = (MbFace *)itemFace;
            const MbSurface * surf = &face->GetSurface();
            const MbSurface * surfBase = &surf->GetSurface();

            if ( surfBase->IsA() == st_Plane ) {
              placeThr.Init( ((MbPlane *)surfBase)->GetPlacement() );
              if ( face->IsSameSense() )
                placeThr.Invert();
              if ( placeThr.GetAxisZ().Colinear( placeObj.GetAxisZ() ) )
                isPlaceThr = true;
            }
            if ( isPlaceThr )
              ::SetThreadOrigin( *surf, placeThr );
          }
          break;
        }
        case 2 : {
          MbItem * objEdge = nullptr;
          MbTopItem * itemEdge = nullptr;
          if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &objEdge, &itemEdge ) && (objSol == objEdge) ) {
            const MbCurveEdge * edge = static_cast<const MbCurveEdge *>(itemEdge);
            const MbFace * faceP = edge->GetFacePlus();
            const MbFace * faceM = edge->GetFaceMinus();
            if ( faceP != nullptr && faceM != nullptr && faceP != faceM ) {
              const MbSurface * surfP = &faceP->GetSurface();
              const MbSurface * surfM = &faceM->GetSurface();
              const MbSurface * surfBaseP = &surfP->GetSurface();
              const MbSurface * surfBaseM = &surfM->GetSurface();
              MbeSpaceType typeP = (MbeSpaceType)surfBaseP->IsA();
              MbeSpaceType typeM = (MbeSpaceType)surfBaseM->IsA();
              if ( typeP == st_Plane || typeM == st_Plane ) {

                /*
                if ( typeP == st_Plane && !isPlaceThr ) {
                  placeThr.Init( (static_cast<const MbPlane *>(surfBaseP))->GetPlacement() );
                  if ( faceP->IsSameSense() )
                    placeThr.Invert();

                  if ( placeThr.GetAxisZ().Colinear( placeObj.GetAxisZ() ) ) {
                    ::SetThreadOrigin( *surfP, placeThr );
                    isPlaceThr = true;
                  }
                }
                if ( typeM == st_Plane && !isPlaceThr ) {
                  placeThr.Init( (static_cast<const MbPlane *>(surfBaseM))->GetPlacement() );
                  if ( faceM->IsSameSense() )
                    placeThr.Invert();

                  if ( placeThr.GetAxisZ().Colinear( placeObj.GetAxisZ() ) ) {
                    ::SetThreadOrigin( *surfM, placeThr );
                    isPlaceThr = true;
                  }
                }
                // */

                if ( !isPlaceThr ) { // BUG_77930: коническое под наклоном
                  MbCartPoint3D pnt0;
                  if ( ::GetThreadEdgeCentre( *edge, pnt0 ) ) {
                    placeThr.Init( placeObj );
                    placeThr.SetOrigin( pnt0 );

                    const MbSurface & faceSurf = faceObj->GetSurface();

                    MbCartPoint3D pnt;
                    faceSurf._PointOn( faceSurf.GetUMid(), faceSurf.GetVMid(), pnt );

                    if ( !c3d::EqualPoints( pnt, pnt0, METRIC_PRECISION ) ) {
                      MbVector3D dir( pnt0, pnt );
                      dir.Normalize();
                      if ( !placeThr.GetAxisZ().Orthogonal( dir ) ) {
                        if ( placeThr.GetAxisZ() * dir < -ANGLE_REGION ) {
                          placeThr.AxisZInvert();
                        }
                      }
                    }
                    else {
                      PRECONDITION( false ); // как-нибудь по другому
                    }

                    isPlaceThr = true;
                  }
                }
              }
              else if ( faceP == faceObj || faceM == faceObj ) {
                placeThr.Init( placeObj );
                const MbSurfaceIntersectionCurve & intCurve = edge->GetIntersectionCurve();
                                
                const MbSurface * pSurf1 = intCurve.GetSurfaceOne();
                const MbSurface * pSurf2 = intCurve.GetSurfaceTwo();
                const MbCurve * uvCurve = nullptr;
                if ( pSurf1 == &surfObj )
                  uvCurve = &intCurve.GetCurveOneCurve();
                else if ( pSurf2 == &surfObj )
                  uvCurve = &intCurve.GetCurveTwoCurve();

                if ( uvCurve != nullptr ) {
                  MbRect rectCurv;
                  uvCurve->AddYourGabaritTo( rectCurv );
                  double vCurvMin = rectCurv.bottom;
                  double vCurvMax = rectCurv.top;

                  const MbSurface & cbSurfObj = faceObj->GetSurface();
                  double vSurfMin = cbSurfObj.GetVMin();
                  double vSurfMax = cbSurfObj.GetVMax();

                  double vThr = MB_MAXDOUBLE;
                  bool bNearMin = ::fabs(vSurfMin - vCurvMin) < 0.25 * cbSurfObj.GetVRange();
                  bool bNearMax = ::fabs(vSurfMax - vCurvMax) < 0.25 * cbSurfObj.GetVRange();
                  
                  if ( bNearMin != bNearMax ) {
                    if ( bNearMin ) vThr = vSurfMin;
                    if ( bNearMax ) vThr = vSurfMax;

                    double uThr = (cbSurfObj.GetUMin() + cbSurfObj.GetUMax()) * 0.5;
                    MbCartPoint3D pntOn, pnt;
                    surfObj._PointOn( uThr, vThr, pntOn );

                    MbAxis3D axisThr( placeThr.GetOrigin(), placeThr.GetAxisZ() );
                    axisThr.PointProjection( pntOn, pnt );

                    MbVector3D vect( placeThr.GetOrigin(), pnt );
                    if ( vect.Length() > LENGTH_EPSILON ) {
                      placeThr.Move( vect );
                      if ( bNearMax )
                        placeThr.AxisZInvert();
                    }
                    isPlaceThr = true;
                  }
                }
              }
            }
          }
          break;
        }
        default: break;
      }

      if ( isPlaceThr ) { // Плейсмент резьбы получен
        MbCartPoint3D thrOrigin;
        rotAxis.PointProjection( placeThr.GetOrigin(), thrOrigin );
        placeThr.SetOrigin( thrOrigin );

        double length = 100.0;
        double step   = 4.0;
        if ( GetDouble( IDS_ENTER_THREAD_LENGTH, length) && GetDouble( IDS_ENTER_THREAD_STEP, step  ) ) {
          int iLeftPlace = 0;
          GetInt( IDS_ENTER_PLACE_TYPE, iLeftPlace );
          bool doLeft = (iLeftPlace != 0);
          if ( doLeft != placeThr.IsLeft() )
            placeThr.AxisYInvert();

          double angle = 0.0;
          double radObj = objRadius;

          if ( !isCylinder ) {
            const MbConeSurface & conObj = static_cast<const MbConeSurface &>(surfObj);

            angle = conObj.GetAngle();
            if ( placeThr.GetAxisZ() * placeObj.GetAxisZ() < 0 )
              angle = -angle;

            MbCartPoint3D pnt( placeThr.GetOrigin() );
            MbVector3D vect( placeThr.GetAxisX() );
            SArray<MbCartPoint> uvArr( 2, 2 );
            surfObj.DirectPointProjection( pnt, vect, uvArr, true );
            if ( uvArr.Count() > 0 ) {
              double & vProj = uvArr[0].y;
              radObj = conObj.GetR( vProj );
            }
            else {
              MessageBoxEx( RT_THREAD_IS_NOT_ADDED, msgTitle );
              return;
            }
          }
          
          bool isOut = false;
          
          //if ( (!!(placeObj.IsLeft()) == (objHeight > 0.0)) == (objRadius > 0.0) ) 
          //  isOut = !faceObj->IsSameSense();
          //else
          //  isOut = faceObj->IsSameSense();

          { // определим вид резьбы: внешняя и внутренняя?
            double u = faceObj->GetSurface().GetUMid();
            double v = faceObj->GetSurface().GetVMid();
            MbCartPoint3D objPnt, axisPnt;
            surfObj._PointOn( u, v, objPnt );
            rotAxis.PointProjection( objPnt, axisPnt );
            double len = objPnt.DistanceToPoint( axisPnt );

            if ( len > LENGTH_EPSILON ) {
              MbVector3D dir( axisPnt, objPnt );
              dir /= len;
              MbVector3D norm;
              surfObj._Normal( u, v, norm );
              if ( !faceObj->IsSameSense() )
                norm.Invert();
              isOut = norm * dir > ANGLE_EPSILON;
            }
            else {
              MessageBoxEx( RT_THREAD_IS_NOT_ADDED, msgTitle );
              return;
            }
          }

          double radThr = radObj + step * (isOut ? -0.5 : 0.5);

          SPtr<MbThread> thr( new MbThread( placeThr, radObj, radThr, length, angle ) );

          if ( thr->IsValid() ) {
            MbMatrix3D matrFrom;

            if ( thr->IsBodyThread( *solid, matrFrom ) ) {
              bool doAdapt = true;
              ::GetBoolean( IDS_ENTER_ADAPT_THREAD, doAdapt, false );

              if ( doAdapt ) {
                MbeThrAdapt thrAdapt = ta_UsingSolid;

                int iAdapt = thrAdapt;
                GetInt( IDS_ENTER_ADAPT_TYPE, iAdapt );
                if ( iAdapt > ta_UsingNothing && iAdapt < ta_TypesCount )
                  thrAdapt = (MbeThrAdapt)iAdapt;
                else {
                  MessageBoxEx( RT_ERROR_IN_PARAMS, msgTitle );
                }

                if ( thr->AdaptToBody( *solid, matrFrom, thrAdapt ) ) {
#ifdef C3D_DEBUG // проверка расчета положений начала и конца модифицированной резьбы относительно исходной.
                  SPtr<MbThread> thr0( new MbThread( placeThr, radObj, radThr, length, angle ) );
                  double begPos, endPos;
                  PRECONDITION( thr->GetLimitPositions( *thr0, begPos, endPos ) );
#endif
                  MessageBoxEx( RT_THREAD_IS_ADAPTED, msgTitle );
                }
                else
                  MessageBoxEx( RT_THREAD_IS_NOT_ADAPTED, msgTitle );
              }
            }
            
            if ( thr->IsBodyThread( *solid, matrFrom ) )
              MessageBoxEx( RT_THREAD_ADDED, msgTitle );
            else
              MessageBoxEx( RT_THREAD_NOT_ON_BODY, msgTitle );

            TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, thr );
          }
          else {
            MessageBoxEx( RT_DEGENERATED_THREAD, msgTitle );
          }      
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Построение отображения условного обозначения резьбы на плоскость
// ---
void CreateNewThreadMappingByThread()
{
  TCHAR msgTitle[] = IDS_TITLE_MSG;

  MbItem * obj = nullptr;

  SolidSPtr solid;
  SPtr<MbThread> thread;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);
    solid = static_cast<MbSolid *>(obj);
    obj = nullptr;
  }

  if ( solid != nullptr && GetGObj( IDS_SELECT_THREAD, st_Thread, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);
    thread = static_cast<MbThread *>( static_cast<MbSpaceInstance *>(obj)->SetSpaceItem() );
    obj = nullptr;
  }

  if ( solid != nullptr && thread != nullptr ) {
    MbMatrix3D matrFrom;
    if ( thread->IsBodyThread( *solid, matrFrom ) ) {
      // Выберем плоскость вида
      if ( GetGObj(IDS_SELECT_VIEW_PLANE, st_Plane, &obj) ) {
        const MbSpaceItem * sItem = static_cast<MbSpaceInstance *>(obj)->GetSpaceItem(); // Дать трёхмерный геометрический объекта
        if ( (sItem != nullptr) && sItem->IsA() == st_Plane ) {
          const MbSurface * surfView = static_cast<const MbSurface *>(sItem); // Плоскость вида
          MbPlacement3D placeView; // Расположение вида
          const MbPlacement3D & placeThr = thread->GetPlacement();
          surfView->GetPlacement( &placeView );
          if ( placeView.GetAxisZ().Orthogonal( placeThr.GetAxisZ() ) || 
               placeView.GetAxisZ().Colinear ( placeThr.GetAxisZ() ) ) {
            // Выберем тип отображения
            int tmType = 0;
            if ( GetInt( IDS_ENTER_THR_MAP_TYPE, tmType ) ) {
              if ( tmType > 0 && tmType < 3 )
                tmType = (tmType == 1) ? tmt_CuttedView : tmt_SectionView;
              else
                tmType = tmt_CompleteView;

              if ( tmType == tmt_CuttedView || tmType == tmt_SectionView ) {
                SolidSPtr cutSolid;

                MbResultType res = rt_Error;
                {
                  SurfaceSPtr cutSurface( &static_cast<MbSurface &>(surfView->Duplicate()) );

                  MbSNameMaker snMaker( ct_CuttingSolid, MbSNameMaker::i_SideNone, 0 );
                  MbMergingFlags mergeFlags( TestVariables::mergeFaces, TestVariables::mergeEdges );
                  MbShellCuttingParams cutParams( *cutSurface, true, ts_negative, mergeFlags, solid->IsClosed(), snMaker );

                  SolidsSPtrVector cutSolids;
                  res = ::SolidCutting( solid, cm_Copy, cutParams, cutSolids );
                  if ( !cutSolids.empty() )
                    cutSolid = cutSolids.front();
                }

                if ( res == rt_Success && cutSolid != nullptr && cutSolid->GetFacesCount() > 0 )
                  solid = cutSolid;
                else {
                  PRECONDITION( false );
                }
              }

              ThreadMapperStruct tmData( *thread, *solid, matrFrom );
              tmData.placeView  = placeView;
              tmData.thrMapType = (MbeThrMapType)tmType;

              MbName annCurvesName;
              MbAnnCurves annCurves( annCurvesName, 0/*nullptr*/, 0 );
              PerformThreadMapping( tmData, annCurves );

              if ( annCurves.GetCurvesCount() > 0 ) {
                for ( size_t i = 0, icnt = annCurves.GetCurvesCount(); i < icnt; i++ ) {
                  const CurveWType * wtCurve = annCurves.GetCurve(i);
                  MbCurve3D * curve = static_cast<MbCurve3D *>( &wtCurve->GetCurve().Duplicate() );
                  TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, curve );
                }              
              }
              else 
                MessageBoxEx( RT_NOT_MAP_CURVES, msgTitle );
            }
          }
          else 
            MessageBoxEx( RT_INCORRECT_VIEW_PLANE, msgTitle );
        }
      }
    }
    else 
      MessageBoxEx( RT_THREAD_NOT_ON_BODY, msgTitle );
  }  
}


//------------------------------------------------------------------------------
// Построение отображения условного обозначения резьбы на плоскость
// ---
void CreateNewThreadMappingByParams()
{
  MbItem      * obj1 = nullptr;
  MbTopItem * item1 = nullptr;

  // Первым делом попросим указать поверхность
  if ( GetTObj(IDS_SELECT_CYLCONE_FACE, st_Solid, tt_Face, &obj1, &item1) ) {
    MbSolid * solid = (MbSolid*)obj1;
    MbFace * elem1 = (MbFace *)item1; // Несущая грань для резьбы

    int elem1IsA = elem1->GetSurface().GetSurface().IsA();
    // Причем желательно цилиндрическую или коническую
    if ( elem1IsA == st_CylinderSurface || elem1IsA == st_ConeSurface ) {
      // Вроде успешно, теперь нужна плоскость

      bool bIsCylinder = ( elem1IsA == st_CylinderSurface );

      MbCylinderSurface * cylSurf = nullptr;
      MbConeSurface     * conSurf = nullptr;
      MbPlacement3D       surfPlace; // Расположение несущей поверхности резьбы

      // Несущая поверхность резьбы
      if ( bIsCylinder ) {
        cylSurf = (MbCylinderSurface*)&elem1->GetSurface().GetSurface();
        surfPlace.Init( cylSurf->GetPlacement() ); // Расположение несущей поверхности резьбы
      }
      else {
        conSurf = (MbConeSurface*)&elem1->GetSurface().GetSurface();
        surfPlace.Init( conSurf->GetPlacement() ); // Расположение несущей поверхности резьбы
      }

      MbItem * obj2 = nullptr;
      if ( GetGObj(IDS_SELECT_ORTO_PLANE, st_Plane, &obj2) ) {
        const MbSpaceItem * sItem = ((MbSpaceInstance *)obj2)->GetSpaceItem(); // Дать трёхмерный геометрический объекта
        if ( (sItem != nullptr) && sItem->IsA() == st_Plane ) {
          const MbSurface * baseSurf = (const MbSurface *)sItem; // Плоскость началы резьбы
          MbPlacement3D threadPlace;                  // Расположение начала резьбы
          baseSurf->GetPlacement( &threadPlace );
          if ( threadPlace.GetAxisZ().Colinear(surfPlace.GetAxisZ()) ) {   

            /*if ( threadPlace.IsRight() == surfPlace.IsRight() ) {
              MbCartPoint3D origin = threadPlace.GetOrigin();
              origin.x = surfPlace.GetOrigin().x;
              origin.y = surfPlace.GetOrigin().y;
              threadPlace.SetOrigin( origin );
            }*/

            MbItem * obj3 = nullptr;
            if ( GetGObj(IDS_SELECT_VIEW_PLANE, st_Plane, &obj3) ) {
              sItem = ((MbSpaceInstance *)obj3)->GetSpaceItem(); // Дать трёхмерный геометрический объекта
              if ( (sItem != nullptr) && sItem->IsA() == st_Plane ) {
                const MbSurface * viewSurf = (const MbSurface *)sItem; // Плоскость вида
                MbPlacement3D viewPlace;              // Расположение вида
                viewSurf->GetPlacement( &viewPlace );
                if ( viewPlace.GetAxisZ().Orthogonal(threadPlace.GetAxisZ()) || 
                     viewPlace.GetAxisZ().Colinear(threadPlace.GetAxisZ()) ) 
                {
                  int  threadMapType =  0;
                  double threadLength = .0;
                  double threadStep   = .0;
                  if ( GetInt(IDS_ENTER_THR_MAP_TYPE, threadMapType)   &&
                       GetDouble(IDS_ENTER_THREAD_LENGTH, threadLength) &&
                       GetDouble(IDS_ENTER_THREAD_STEP, threadStep)        ) 
                  {
                    if ( threadMapType >= 0 && threadMapType <= 2 ) {
                      // Подготовим данные для резьбы
                      MbeThrMapType thrMapType = threadMapType == 0 ? tmt_CompleteView 
                                               : threadMapType == 1 ? tmt_CuttedView
                                                                    : tmt_SectionView;
                      bool   outTread   = false;
                      double surfRad    = bIsCylinder ? cylSurf->GetRadius() : conSurf->GetRadius();
                      double surfHeight = bIsCylinder ? cylSurf->GetHeight() : conSurf->GetHeight();

                      if ( (!!(surfPlace.IsLeft()) == (surfHeight > 0.0)) == (surfRad > 0.0) ) 
                        outTread = !elem1->IsSameSense();
                      else
                        outTread = elem1->IsSameSense();

                      double thrRad  = outTread ? surfRad - threadStep * 0.5
                                                : surfRad + threadStep * 0.5;

                      MbMatrix3D fromSolid; // Единичная
                      MbLump lump( *solid, fromSolid );
                      threadPlace.Transform( fromSolid );

                      double ang = bIsCylinder ? 0.0 : -conSurf->GetAngle(); // Угол коничности резьбы (он же угол коничности грани под резьбой)

                      MbThread * thr = new MbThread( threadPlace,   // Система координат резьбы ( в координатах тела(детали) на котором она нарезана ) 
                                                     surfRad,       // Начальный радиус конической поверхности под резьбой
                                                     thrRad,        // Начальный радиус резьбы
                                                     threadLength,  // Длина на которую пытаемся нарезать
                                                     ang );         // Угол коничности резьбы
                      if ( thr->IsValid() ) {
                        MbName annCurvesName;
                        MbAnnCurves annCurves( annCurvesName, 0/*nullptr*/, 0 );

                        ThreadMapperStruct tmData( *thr, *solid, fromSolid );
                        tmData.placeView  = viewPlace;
                        tmData.thrMapType = thrMapType;

                        ::PerformThreadMapping( tmData, annCurves );
                      }
                      ::DeleteItem( (MbSpaceItem *&)thr );
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
}


//-----------------------------------------------------------------------------
// Коррекция точности отрисовки в соответствии с габаритным кубом
// ИР K7+ drawPrecisionFactor - это не точность и не стрелка прогиба и не deviationStep - это множитель, загрубляющий
// ИР K7+                       и точность, и стрелку прогиба, и deviationStep   
//---
//double ComponentBasePropertes::CorrectDrawPrecision( double drawPrecisionFactor, const MbCube &gabarit ) 
double CalculateSag( double diagonal ) 
{
  static const double st_minRatio = 0.000209; // Минимальное отношение шага и диаметра (6642 треугольников)

  if ( diagonal < Math::metricRegion )
    diagonal = 50;

//  double drawPrecisionFactor = 0.064;
//  double sag = drawPrecisionFactor * diagonal * 0.023;
  double sag = diagonal * /*0.00270644416601401;*/0.000906956657777336;/*0.000209682291303;*/

  if ( sag/diagonal < st_minRatio ) 
    sag = st_minRatio * diagonal;

  return sag;
}



//------------------------------------------------------------------------------
/** \brief \ru Построить массив триангуляций.
           \en Create array of triangulations. \~
  \details \ru Построить массив триангуляций для массива твердых тел.
    Если массив триангуляций пуст, то он наполняется.
    Имеющаяся в элементах массива триангуляция будет удалена.
           \en Create array of triangulations for array of solid solids.
    If array of triangulations is empty then it is filled.
    Triangulation in elements of array will be deleted. \~
  \note    \ru В многопоточном режиме выполняется параллельно. 
           \en In multithreaded mode runs in parallel. \~
  \param[in]  solids    - \ru Набор тел.
                          \en A set of solids. \~
  \param[in]  stepData  - \ru Тип триангуляции и величина стрелки прогиба. Параметр, управляющий густотой триангуляции.
                          \en Type of triangulation and value of sag. Value of triangulation density. \~
  \param[out] gridirons - \ru Результат - полигональный геометрический объект.
                          \en Polygonal geometric mesh object as a result. \~
  \ingroup Triangulation
*/
// ---
static
void CalculateMesh( const RPArray<MbSolid> & solids, 
                    const MbStepData & stepData, // параметр, управляющий густотой триангуляции
                          RPArray<MbMesh> & meshs ) 
{
  size_t kCount = solids.Count(), gCount = meshs.Count();

  // Если массив триангуляций пуст, то он наполняется. Имеющаяся в элементах массива триангуляция будет удалена
  if ( gCount < kCount ) {
    meshs.Reserve( kCount - gCount );
    for ( size_t k = gCount; k < kCount; k++ ) {
      MbMesh * gridiron = new MbMesh();
      meshs.Add( gridiron );
    }
  }

  bool useParallel = ( (kCount > 1) && Math::Multithreaded() );
  CSSArray<const MbSolid *> uniqueSolids( kCount, 1 );
  if ( useParallel ) {
    for ( size_t k = 0; k < kCount; k++ )
      uniqueSolids.AddNoSort( solids[k] );
    uniqueSolids.Sort( nullptr );
    if ( uniqueSolids.Count() < kCount )
      useParallel = false;
  }

  ENTER_PARALLEL( useParallel );
#pragma omp parallel for if ( useParallel )
  for ( ptrdiff_t k = 0; k < (ptrdiff_t)kCount; k++ ) {
    MbSolid * solid = solids[k];
    if ( solid != nullptr )
      solid->CalculateGrid( stepData, *(meshs[k]) ); 
  } 
  EXIT_PARALLEL( useParallel );
}


//------------------------------------------------------------------------------
// Построить треангуляцию сборки тел
// ---
bool TriangulateAllSolids()
{
  MbMatrix3D from;
  MbItem * obj = nullptr;

  PArray<MbItem> objs( 0, 1, false );
  TestVariables::viewManager->GetRenderingObjects( objs );
  SetWaitCursor( true );

  RPArray<MbSolid> solids( 0, 1 );
  RPArray<MbMesh> gridirons( 0, 1 );
  MbCube cube;
  MbRegDuplicate * iReg = nullptr;
  MbAutoRegDuplicate autoReg( iReg );  

  for ( ptrdiff_t i = 0; i < (ptrdiff_t)objs.Count(); i++ ) {
    obj = objs[i];
    if ( obj->IsA() == st_Solid ) {
      MbSolid * sol = static_cast<MbSolid *>(obj);
      if ( sol != nullptr ) {
        MbItem * meshItem = obj->CreateMesh( MbStepData(ist_SpaceStep, Math::visualSag), MbFormNote(false, true, true, TestVariables::exactMesh), iReg );
        if ( (meshItem != nullptr) && (meshItem->IsA() == st_Mesh) ) {
          MbMesh * mesh = (MbMesh *)meshItem;
          obj->AddYourGabaritTo( cube );
          solids.Add( sol );
          gridirons.Add( mesh );
        }
      }
    }
  }

  double sag = ::CalculateSag( cube.GetDiagonal() );
  MbeStepType stepType = ist_SpaceStep;
  
#if defined(C3D_DEBUG)
  SET_TIME_TEST(true);
#endif // C3D_DEBUG
  uint miliSec = ::GetTickCountEx();

  ::CalculateMesh( solids, MbStepData(stepType, sag), gridirons );

  miliSec = ::GetTickCountEx() - miliSec;
#if defined(C3D_DEBUG)
  CHECK_TIME_TEST();
#endif // C3D_DEBUG

  SetWaitCursor( false );
  TimeTest * testResult = nullptr;
#if defined(C3D_DEBUG)
  testResult = ::GetTimeTestResults();
#endif // C3D_DEBUG

  ptrdiff_t pointsCount = 0;
  ptrdiff_t trianglesCount = 0;
  ptrdiff_t quadranglesCount = 0;
  ptrdiff_t gridsCount = 0;

  for ( size_t ind = 0; ind < gridirons.Count(); ind++ ) {
    MbMesh * gridiron = gridirons[ind]; 
    gridsCount += gridiron->GridsCount();
    for ( size_t i0 = 0; i0 < gridiron->GridsCount(); i0++ ) {
      const MbGrid * grid = gridiron->GetGrid( i0 );
      pointsCount += grid->PointsCount();
      trianglesCount += grid->TrianglesCount();
      quadranglesCount += grid->QuadranglesCount();
    }
    ::DeleteItem( gridirons[ind] );
  }

  ShowTimeTestResult( testResult, solids.size(), gridsCount, pointsCount, 
                      trianglesCount, quadranglesCount, miliSec, IDS_ASSEMBLY_TRIANGULATION );

#if defined(C3D_DEBUG)
  SET_TIME_TEST(false);
#endif // C3D_DEBUG
    
  return true;
}


//------------------------------------------------------------------------------
// Построение эпюры кривизны кривой на самой кривой.
// Plotting curvature curve on the curve itself.
// ---
void CurveCurvatureProfile()
{
  MbItem * nearest = nullptr;
  MbCartPoint3D point;

  //MbItem * objEdge = nullptr;
  //MbTopItem * itemEdge = nullptr;
  //if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &objEdge, &itemEdge ) ) {
  //  const MbCurveEdge * edge = static_cast<const MbCurveEdge *>( itemEdge );
  //  ConstSpaceCurveSPtr pcurve( &edge->GetIntersectionCurve() );

  if ( GetGObj(IDS_SELECT_CURVE, st_Curve3D, &nearest) ) {
    TestVariables::viewManager->ToggleObject( nearest );
    SPtr<MbCurve3D> pcurve( GetCurve3D(nearest, false) );
    std::vector<c3d::DoublePair> points;
    pcurve->GetCurvatureSpecialPoints( points );
    TestVariables::viewManager->ToggleObject( nearest );

    MbCurve3D & curve = *pcurve;
    const size_t n = 40;                                // Минимальное количество точек на кривой.    
    const double deviationSag = 0.1 * Math::deviateSag; // Отклонение по углу нормали за один шаг.
    double tmin = curve.GetTMin(), tmax = curve.GetTMax(), t = tmin;
    const double dT = ( tmax - tmin ) / ( double(n) + M_SQRT1_2 ); // Величина шага.
    const double mindT = dT * DELTA_MID;                // Минимальная величина шага.    
    std::map<double, double> plot;                      // Набор точек на построение.
    //double maxCurv = 0.0;
    // Проходимся по кривой с шагом, регулируемым отклонением нормали по углу.
    while ( true ) {
      double curvature = curve.Curvature( t );
      //maxCurv = std_max( maxCurv, curvature );
      if ( t == tmin ) {
        if ( points.empty() || ::fabs(t - points.front().first) > PARAM_PRECISION )
          plot[t] = curvature;
      }
      else if ( t == tmax ) {
        if ( points.empty() || ::fabs(t - points.back().first) > PARAM_PRECISION )
          plot[t] = curvature;
      }
      else
        plot[t] = curvature;
      if ( t == tmax )
        break;
      double dt = dT;
      if ( curvature > NULL_EPSILON )                    // Ограничение величины шага отклонением нормали на заданный угол.
        dt = std_min( dt, std_max(mindT, deviationSag / (curvature * curve.FirstDer(t).Length())) );
      t += dt;
      if ( t > tmax )
        t = tmax;
    }
    // Вставляем особые точки в общий набор.
    for ( size_t i = 0, icount = points.size(); i < icount; i++ ) {
      if ( points[i].second == MB_MAXDOUBLE || points[i].second == MB_MINDOUBLE ) { // Для точек разрыва кривизны, вставляем значение до и после разрыва.
        t = points[i].first - PARAM_PRECISION;
        double curvature = curve.Curvature(t);
        //maxCurv = std_max( maxCurv, curvature );
        plot[t] = curvature;
        plot[points[i].first] = MB_MAXDOUBLE;
        t = points[i].first + PARAM_PRECISION;
        curvature = curve.Curvature( t );
        //maxCurv = std_max( maxCurv, curvature );
        plot[t] = curvature;
      }
      else {
        //maxCurv = std_max( maxCurv, points[i].second );
        plot[points[i].first] = ::fabs( points[i].second );
      }
    }
    MbCube cube;
    curve.AddYourGabaritTo( cube );
    double scale = TestVariables::curvatureScale; //0.1 * cube.GetDiagonal() / maxCurv; // Масштаб отображения кривизны общий, чтобы можно было сравнивать кривые.
    std::map<double, double>::iterator it = plot.begin();
    SPtr<MbContour3D> plotCont( new MbContour3D() );
    // Заполняем отрисовываемый профиль.
    while ( it != --plot.end() ) {
      while ( it->second == MB_MAXDOUBLE )
        it++;
      std::map<double, double>::iterator it2 = it;
      it2++;
      MbCartPoint3D p, p1, p2;
      MbVector3D norm;
      curve._PointOn( it->first, p );
      curve._Normal( it->first, norm );
      p1 = p - norm * it->second * scale;
      MbLineSegment3D s1( p, p1 );
      plotCont->AddSegment( s1, false );
      while ( it2->second != MB_MAXDOUBLE ) {
        curve._PointOn( it2->first, p );
        curve._Normal( it2->first, norm );
        p2 = p - norm * it2->second * scale;
        MbLineSegment3D s2( p1, p2 );
        MbLineSegment3D s3( p, p2 );
        MbLineSegment3D s4( p, p2 );
        plotCont->AddSegment( s2, false );
        plotCont->AddSegment( s3, false );
        plotCont->AddSegment( s4, false );
        p1 = p2;
        if ( it2 == --plot.end() )
          break;
        it2++;
      }
      it = it2;
    }
    TestVariables::viewManager->AddObject( Style(1, RGB(0, 150, 0)), plotCont );
  }
}


//------------------------------------------------------------------------------
// Построить график кривизны кривой.
// The curve curvature plotting.
// ---
void PlotCurveCurvature() {
  MbItem * nearest = nullptr;
  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest ) ) 
    if ( nearest && (nearest->IsA() == st_SpaceInstance) ) {
      const MbCurve3D * curve = (MbCurve3D *)((MbSpaceInstance *)nearest)->GetSpaceItem();
      if ( curve != nullptr ) {
        double scale_factor = curve->GetMetricLength();
        
        if ( GetDouble ( IDS_ENTER_SCALE, scale_factor ) ) {
          SArray<MbCartPoint> crvPoints; // Набор точек кривизна-значение параметра
          bool break_flag = false;
          double v = curve->GetTMin();
          while ( !break_flag ) { // Проходим всю направляющую
            MbCartPoint point_v;
            point_v.x = curve->CalculateLength ( curve->GetTMin(), v );
            point_v.y = scale_factor * curve->Curvature ( v );
            crvPoints.Add ( point_v );
            v += curve->DeviationStep ( v, Math::deviateSag  );  // Шаг, полученный по малому отклонению
            if ( v >= curve->GetTMax() ) {
              v = curve->GetTMax();
              break_flag = true;
            }
          }
          MbHermit * curvature  = MbHermit::Create ( crvPoints, false /*closed*/ ); 
          MbPlacement3D place; // Глобальная система координат
          if ( curvature != nullptr )
            TestVariables::viewManager->AddObject ( TestVariables::BSPLINE_Style, curvature, &place );
        }
      }
    }
}


//------------------------------------------------------------------------------
// Построить график кручения кривой.
// The curve torsion plotting.
// ---
void PlotCurveTorsion()
{
  MbItem * nearest = nullptr;
  if ( GetGObj( IDS_SELECT_CURVE, st_Curve3D, &nearest ) ) 
    if ( nearest && (nearest->IsA() == st_SpaceInstance) ) {
      const MbCurve3D * curve = (MbCurve3D *)((MbSpaceInstance *)nearest)->GetSpaceItem();
      if ( curve != nullptr ) {
        double length = curve->GetMetricLength();
        double scale_factor = length;

        if ( GetDouble ( IDS_ENTER_SCALE, scale_factor ) ) {
          SArray<MbCartPoint> crvPoints; // Набор точек кручение-значение параметра
          bool break_flag = false;
          double v = curve->GetTMin();
          while ( !break_flag ) { // Проходим всю направляющую
            MbCartPoint point_v;
            point_v.x = curve->CalculateLength ( curve->GetTMin(), v );
            MbVector3D first, second, third;
            curve->FirstDer  ( v, first );
            curve->SecondDer ( v, second );
            curve->ThirdDer  ( v, third );
            MbVector3D dom_vector ( first | second );
            double nom = dom_vector * third;
            double dom = dom_vector * dom_vector;
            if ( ::fabs( dom ) < NULL_REGION ) 
              nom = 0.0;
            else
              nom /= ( - dom );

            point_v.y = scale_factor * nom;
            crvPoints.Add ( point_v );
            v += curve->DeviationStep ( v, Math::deviateSag  );  // Шаг, полученный по малому отклонению
            if ( v >= curve->GetTMax() ) {
              v = curve->GetTMax();
              break_flag = true;
            }
          }
          MbHermit * torsion  = MbHermit::Create( crvPoints, false /*closed*/ );
          MbPlacement3D * place = new MbPlacement3D; // Глобальная система координат
          TestVariables::viewManager->AddObject ( TestVariables::BSPLINE_Style, torsion, place );
        }
      }
    }
}


//------------------------------------------------------------------------------
// Построение эпюр производных поверхности по кривой на этой поверхности.
// The surface derivative plotting by the curve on this surface.
// ---
void SurfaceDerivativeProfile()
{
  MbItem * nearest = nullptr;
  MbCartPoint3D point;

  if ( GetGObj(IDS_SELECT_CURVE, st_Curve3D, &nearest) ) {
    TestVariables::viewManager->ToggleObject( nearest );

    if ( nearest != nullptr ) {
      if ( nearest->IsA() == st_SpaceInstance ) {
        MbSpaceItem * spaceItem = static_cast<MbSpaceInstance *>( nearest )->SetSpaceItem(); 
        MbSurfaceCurve * sCurve( nullptr );
        if ( (spaceItem != nullptr) && (spaceItem->Family() == st_Curve3D) ) {
          MbCurve3D * curve = static_cast<MbCurve3D *>( spaceItem );
          MbeSpaceType type = curve->IsA(); 
          if ( type == st_SurfaceCurve )
            sCurve = static_cast<MbSurfaceCurve *>( curve );
          if ( type == st_SurfaceIntersectionCurve ) {
            MbSurfaceIntersectionCurve * intCurve = static_cast<MbSurfaceIntersectionCurve *>( curve );
            MbItem * obj = nullptr;
            if ( intCurve != nullptr ) {
              if ( GetGObj(IDS_SELECT_SURFACE, st_Surface, &obj) ) {
                if ( obj->IsA() == st_SpaceInstance ) {
                  // Дать трёхмерный геометрический объект
                  spaceItem = static_cast<MbSpaceInstance *>( obj )->SetSpaceItem(); 
                  if ( (spaceItem != nullptr) && (spaceItem->Family() == st_Surface) ) {
                    MbSurface * surface = static_cast<MbSurface *>( spaceItem );
                    if ( surface != nullptr && 
                         &surface->GetSurface() == intCurve->GetSurfaceOne() )
                      sCurve = &intCurve->SetCurveOne();
                    else
                    if ( surface != nullptr && 
                         &surface->GetSurface() == intCurve->GetSurfaceTwo() )
                      sCurve = &intCurve->SetCurveTwo();
                  }
                }
              }
              else {
                MbTopItem * tObj = nullptr;
                if ( GetTObj(IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &tObj) ) {
                  if ( tObj != nullptr && tObj->IsA() == tt_Face ) {
                    MbFace * face = static_cast<MbFace *>( tObj );
                    MbSurface * surface = &face->SetSurface().SetSurface();
                    if ( &surface->GetSurface() == intCurve->GetSurfaceOne() )
                      sCurve = &intCurve->SetCurveOne();
                    else
                    if ( &surface->GetSurface() == intCurve->GetSurfaceTwo() )
                      sCurve = &intCurve->SetCurveTwo();
                  }
                }
              }
            }
          }
        }
        if ( sCurve != nullptr ) {
          std::vector<std::pair<MbCartPoint, MbVector3D> > derives;
          int derNumber = TestVariables::kind;
          int n = TestVariables::count; // Минимальное количество точек на кривой.
          GetInt( IDS_ENTER_DERIVATIVE_NUMBER, derNumber );
          GetInt( RT_POINTS_COUNT, n );
          if ( n < 2 )
            n = 2;
          if ( n > COUNT_MAX )
            n = COUNT_MAX;
          TestVariables::count = n;
          TestVariables::kind = derNumber;
          double tmin = sCurve->GetTMin(), tmax = sCurve->GetTMax(), t = tmin;
          double d = 0.0, dMax = 0.0;
          double dT = ( tmax - tmin ) / double(n); // Величина шага.
          MbVector3D vect;
          MbCartPoint uv;
          const MbCurve & pCurve = sCurve->GetCurve();
          const MbSurface & surface = sCurve->GetSurface();
          size_t i, iCount;
          double scale = TestVariables::curvatureScale;

          derives.reserve( n + 1 );
          for ( i = 0, iCount = n + 1; i < iCount; ++i ) {
            t = dT * i + tmin;
            pCurve.PointOn( t, uv );
            if ( derNumber == 1 )
              surface._DeriveU( uv.x, uv.y, vect );
            else if ( derNumber == 2 )
              surface._DeriveV( uv.x, uv.y, vect );
            else if ( derNumber == 3 )
              surface._DeriveUU( uv.x, uv.y, vect );
            else if ( derNumber == 4 )
              surface._DeriveVV( uv.x, uv.y, vect );
            else if ( derNumber == 5 )
              surface._DeriveUV( uv.x, uv.y, vect );
            else if ( derNumber == 6 )
              surface._DeriveUUU( uv.x, uv.y, vect );
            else if ( derNumber == 7 )
              surface._DeriveUUV( uv.x, uv.y, vect );
            else if ( derNumber == 8 )
              surface._DeriveUVV( uv.x, uv.y, vect );
            else if ( derNumber == 9 )
              surface._DeriveVVV( uv.x, uv.y, vect );
            else 
              surface._Normal( uv.x, uv.y, vect );
#ifdef _DRAWGI
            if ( DrawComputation::drawDefault ) {
              MbCartPoint3D pnt1, pnt2;
              surface._PointOn( uv.x, uv.y, pnt1 );
              pnt2 = vect * scale * DELTA_MID + pnt1;
              TestVariables::drawImp->DrawLine( pnt1, pnt2, TRGB_LIGHTRED );
            }
#endif // _DRAWGI

            std::pair<MbCartPoint, MbVector3D> tVector( uv, vect );
            derives.push_back( tVector );
            d = vect.Length();
            if ( dMax < d )
              dMax = d;
          }
          d = 1.0;
          if ( dMax > EPSILON )
            d = scale / dMax;
          for ( i = 0, iCount = derives.size(); i < iCount; ++i ) {
            derives[i].second *= d;
          }

          if ( derives.size() > 1 ) {
            SPtr<MbContour3D> plotCont( new MbContour3D() );
            MbCartPoint3D p0, q0, p1, q1;
            
            uv = derives[0].first;
            surface._PointOn( uv.x, uv.y, p0 );
            q0.Set( p0, derives[0].second, 1.0 );
            for ( i = 1, iCount = derives.size(); i < iCount; ++i ) {
              uv = derives[i].first;
              surface._PointOn( uv.x, uv.y, p1 );
              q1.Set( p1, derives[i].second, 1.0 );
              MbLineSegment3D s0( p0, q0 );
              MbLineSegment3D s1( q0, q1 );
              MbLineSegment3D s2( q1, p1 );
              plotCont->AddSegment( s0, false );
              plotCont->AddSegment( s1, false );
              plotCont->AddSegment( s2, false );
              p0 = p1;
              q0 = q1;
            }

            TestVariables::viewManager->AddObject( Style(1, RGB(0, 150, 0)), plotCont );
          }
        }
      }
    }
    TestVariables::viewManager->ToggleObject( nearest );
  }
}


//------------------------------------------------------------------------------
// Построение объема заполненного сосуда.
// The construction of the volume of flood fill.
// ---
void FloodFill()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj, TestVariables::itemPathFrom ) && obj->IsA() == st_Solid ) {
    TestVariables::viewManager->ToggleObject( obj );

    MbSolid * vessel = (MbSolid *)obj;
    MbCartPoint3D origin;

    if ( GetPoint( IDS_SELECT_POINT, origin ) ) {
      MbItem * spaceObj = nullptr;
      if ( GetGObj( IDS_SELECT_OBJECT, st_SpaceItem, &spaceObj ) ) {
        MbSolid * solid = nullptr;
        MbSurface * surface = nullptr;

        if ( spaceObj->IsA() == st_Solid ) {
          solid = (MbSolid *)spaceObj;
        }
        else
        if ( spaceObj->IsA() == st_SpaceInstance ) {
          MbSpaceItem * spaceItem = ((MbSpaceInstance *)spaceObj)->SetSpaceItem();
          if ( spaceItem != nullptr && spaceItem->Family() == st_Surface ) {
            surface = static_cast<MbSurface *>(spaceItem);
          }
        }
        if ( solid != nullptr || surface != nullptr ) {

          MbSNameMaker names( ct_ReverseCreator, MbSNameMaker::i_SideNone, 0 );
          MbeCopyMode sameShell( cm_Copy ); 
          RPArray<MbContour> contours( 0, 1 );
          RPArray<MbContour3D> contours3d( 0, 1 );
          MbSweptData bungData( surface, contours, contours3d, solid );
          MbSolid * result( nullptr );

          SetWaitCursor( true );

          MbResultType res = ::FloodFillResult( *vessel, sameShell, bungData, origin, names, result );

          SetWaitCursor( false );

          if ( res == rt_Success || result != nullptr ) {
            TestVariables::viewManager->DeleteObject( obj );
            TestVariables::viewManager->DeleteObject( spaceObj );
            obj = nullptr;
            TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, result );
          }
        }
      }
    }
    TestVariables::viewManager->ToggleObject( obj );
  }
}


//----------------------------------------------------------------------------------------
// \ru Класс для сбора тестовой информации. \en The class for collecting testing data.
// ---
class TestWriter : public writer {
public:
  typedef std::unique_ptr<TestWriter>     TestWriter_ptr;
private:
  std::map<int, size_t> _counters;

  /// \ru Конструктор. \en Constructor. 
  TestWriter( iobuf_Seq & buf, bool ownBuf, bool openSys, uint16 om, TapeRegistrator * reg )
    : writer( buf, ownBuf, openSys, om, reg )
    , tape( buf, ownBuf, openSys, uint8( ( om | io::out ) & ~io::in ), reg ) {
  }
public:
  virtual ~TestWriter() {}

  /// \ru Создать экзепляр. \en Create an instance.
  static TestWriter_ptr CreateWriterTest( membuf & sb, uint8  om ) {
    sb.mode( uint8( io::out ) );
    sb.clearState( io::closed );
    sb.fresh( !sb.sysFile().Count() );
    return TestWriter_ptr( new TestWriter( sb, false, true, om, nullptr ) );
  }

  // \ru Тестовые функции. \en Testing functions.
  void SetTestData( const TapeBase * obj ) override {
    const MbSurface * srf = dynamic_cast<const MbSurface *>( obj );
    if ( srf != nullptr )
      _counters[srf->Family()]++;
  }

  // \ru Получить собранные данные. \en Get collected data.
  const std::map<int, size_t> & GetCounters() const { return _counters; }

  OBVIOUS_PRIVATE_COPY( TestWriter )
};

//------------------------------------------------------------------------------
// /ru Количество уникальных объектов заданного типа. /en Number of unique objects of specified type.
// ---
void NumberOfUniqueObjects( MbeSpaceType type )
{
  std::vector<MbItem *> modelItems;
  MbItem * obj = nullptr;
  while ( GetObj(IDS_SELECT_GEOM_OBJECT, &obj) && obj != nullptr ) {
    modelItems.push_back( obj );
    TestVariables::viewManager->ToggleObject( obj );
  }

  if ( modelItems.size() == 0 ) {
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    if ( wind ) {
      const MbModel & model = wind->GetGeomModel();
      model.GetItems( modelItems );
      for ( auto item : modelItems )
        TestVariables::viewManager->ToggleObject( item );
    }
  }

  if ( modelItems.size() ) {
    membuf memBuf;
    TestWriter::TestWriter_ptr out = TestWriter::TestWriter_ptr( TestWriter::CreateWriterTest(memBuf, 0) );
    for ( auto item : modelItems )
      ( *out ) << item;

    const std::map<int, size_t> & counters = out->GetCounters();
    std::map<int, size_t>::const_iterator itr = counters.find( (int)type );
    if ( itr != counters.end() ) {
      const size_t bufLen = 1024;
      TCHAR buf[bufLen];
      size_t len = 0;
      switch ( type ) {
        case st_Surface: {
          len += (size_t)_sntprintf( buf + len, bufLen - len, IDS_NUMBER_UQINUE_SURFACE );
          len += (size_t)_sntprintf( buf + len, bufLen - len, _T(": %zd\n"), itr->second );
        }
        default: {}
      }
      if( len > 0 )
        ::MessageBoxEx( buf, IDS_RESULTS );
    }

    for ( auto item : modelItems )
      TestVariables::viewManager->ToggleObject( item );
  }
  else {
    ::PutErrorMessage( 0, RT_NOTHING_SELECTED );
  }
}
