#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <curve3d.h>
#include <surf_plane.h>
#include <space_instance.h>
#include <plane_instance.h>
#include <cur_line_segment3d.h>
#include <point_frame.h>
#include <wire_frame.h>
#include <conv_topo_mesh.h>
#include <alg_draw.h>
#include <last.h>
#include <alg_draw.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Повернуть плоскость вида в плоскость объекта.
// Rotate the view plane in the object plane.
// ---
bool ViewRotateInObjectPlane() {
  MbItem * obj = nullptr;

  if ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj ) ) 
  {
    WorkWindow *currentView = TestVariables::viewManager->GetActiveWindow();
    MbPlacement3D place;
    MbPlacement3D place0(currentView->GetOwnPlacement());
    MbCartPoint3D p0;
    MbVector3D vx, vy;
    bool bFlag = false;

    if ( obj->IsA() == st_SpaceInstance ) {
      const MbSpaceItem * spaceItem = ((MbSpaceInstance *)obj)->GetSpaceItem();
      if ( (spaceItem != nullptr) && (spaceItem->Family() == st_Curve3D) ) {
        MbCurve3D *curv = (MbCurve3D *)spaceItem;
        double t = curv->GetTMin();
        
        curv->PointOn(t,p0);
        curv->FirstDer(t,vx);
        vy = currentView->GetAxisZ() | vx;
        place.InitXY( p0, vx, vy, true );
        bFlag = true;
      }
    }
    if ( obj->IsA() == st_PlaneInstance ) {
      place.Init( ((MbPlaneInstance *)obj)->GetPlacement() );
      bFlag = true;
    }

    if ( bFlag ) 
    {
      MbPlane plane0(place0);
      MbPlane plane1(place);
      MbAxis3D axis;
      
      if (plane0.GetPlacement().PlanesIntersection( plane1.GetPlacement(), axis.SetOrigin(), axis.SetAxisZ() )) {
        vx = plane0.GetPlacement().GetAxisZ() | plane1.GetPlacement().GetAxisZ();
        double b = vx.Length();
        double a = plane0.GetPlacement().GetAxisZ() * plane1.GetPlacement().GetAxisZ();
        double ang = 0;
        if (a!=0 || b!=0)
          ang = atan2(b,a);
        place0.Rotate( axis, ang );
        currentView->OrientateWindow( place0 );

        if ( obj->IsA() == st_PlaneInstance ) {
          const MbPlaneItem * planeItem = ((MbPlaneInstance *)obj)->GetPlaneItem();
          if ( planeItem != nullptr ) {
            place.Init( ((MbPlaneInstance *)obj)->GetPlacement() );
            place0.Init(currentView->GetOwnPlacement());
            vx = place.GetAxisX() | place0.GetAxisX();
            b = vx.Length();
            a = place.GetAxisX() * place0.GetAxisX();
            ang = 0;
            if (a!=0 || b!=0)
              ang = atan2(b,a);
            axis.Init( place0.GetOrigin(), place0.GetAxisZ() );  
            currentView->RotateWindow( 3, ang ); // Повернуть окно вокруг своей оси номер numb на угол angle, сохранив неподвижной точку центра окна
          }
        }        
        return true;
      }  
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Показать вырожденные треугольники сетки. 
// Show the degenerate triangles of the mesh.  
// ---
bool ShowDegeneratedTrianglesAndGaps() {
  MbItem * meshItem = nullptr;

  if (  GetGObj( IDS_SELECT_MESH, st_Mesh, &meshItem ) ) {
    MbMesh * mesh = static_cast<MbMesh *>( meshItem );
    MbCartPoint3D p0, p1, p2;
    JTC::TopoMesh tM;
    tM.Init( *mesh, true );
    std::vector<size_t> degeneratedTriangles;
    std::map< size_t, std::vector<size_t> > mapTringle = tM.GetDegeneratedTriangles();
    std::map< size_t, std::vector<size_t> >:: iterator it = mapTringle.begin();
    for ( int ii = 0; it != mapTringle.end(); it++, ii++ ) {
      degeneratedTriangles = it->second;
      const MbGrid* mGrid = mesh->GetGrid( it->first );
      if ( degeneratedTriangles.size() > 0 ) {
        MbWireFrame* wFrame = new MbWireFrame;
        for (size_t j = 0; j < degeneratedTriangles.size(); ++j ) { 
          const MbTriangle& curTri =  mGrid->GetTriangle( degeneratedTriangles[j] );
          mGrid->GetPoint( curTri.GetIndex(0), p0 );
          mGrid->GetPoint( curTri.GetIndex(1), p1 );
          mGrid->GetPoint( curTri.GetIndex(2), p2 );
          #ifdef _DRAWGI
          DrawGI:: DrawLine( p0, p1, TRGB_RED );
          DrawGI:: DrawLine( p1, p2, TRGB_RED );
          DrawGI:: DrawLine( p2, p0, TRGB_RED );
          #endif // _DRAWGI
          TestVariables::drawImp->DrawLine( p0, p1, TRGB_RED  );
          TestVariables::drawImp->DrawLine( p1, p2, TRGB_RED  );
          TestVariables::drawImp->DrawLine( p2, p0, TRGB_RED  );
          MbLineSegment3D* is0 = new MbLineSegment3D( p0, p1 );
          wFrame->AddEdge( *is0, true );
          wFrame->SetColor( TestVariables::RED );
          DeleteItem( is0 );
          MbLineSegment3D* is1 = new MbLineSegment3D( p1, p2 );
          wFrame->AddEdge( *is1, true );
          wFrame->SetColor( TestVariables::RED );
          DeleteItem( is1 );
          MbLineSegment3D* is2 = new MbLineSegment3D( p2, p0 );
          wFrame->AddEdge( *is2, true );
          wFrame->SetColor( TestVariables::RED );
          DeleteItem( is2 );
        } 
        Style style;
        style.SetColor( wFrame->GetColor() );
        TestVariables::viewManager->AddObject( style, wFrame );
      } 
    }
     
    std::vector<MbCartPoint3D> bP=tM.GetBoundaryPoints();
    if ( bP.size() > 0 ) {
      MbPointFrame* pFrame = new MbPointFrame;
      Style style;
       
      for ( size_t h=0; h < bP.size(); ++h ){
        #ifdef _DRAWGI
        DrawGI:: DrawPoint( &bP[h], TRGB_RED );     
        #endif // _DRAWGI
        MbCartPoint3D* pp = new MbCartPoint3D( bP[h] );
        TestVariables::drawImp->DrawPoint( pp, TRGB_RED, 4  );
        pFrame->AddVertex( *pp );
        pFrame->SetColor( TestVariables::LIGHTMAGENTA );
        pFrame->SetWidth(5);
        style.SetColor( pFrame->GetColor() );
        TestVariables::viewManager->AddObject( style, pFrame );
      }
    } 
  }
  return false;
}
