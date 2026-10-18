#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <test_draw.h>
#include <test_converter.h>
#include <function_factory.h>
#include <func_line_function.h>
#include <func_sinus_function.h>
#include <func_cubic_function.h>
#include <cur_line3d.h>
#include <cur_contour3d.h>
#include <cur_plane_curve.h>
#include <cur_point_curve.h>
#include <cur_projection_curve.h>
#include <cur_surface_intersection.h>
#include <cur_contour.h>
#include <cur_contour_on_plane.h>
#include <surf_spline_surface.h>
#include <surf_plane.h>
#include <surf_spine.h>
#include <alg_base.h>
#include <creator.h>
#include <name_check.h>
#include <attr_color.h>
#include <action_general.h>
#include <check_geometry.h>
#include <action_phantom.h>
#include <action_direct.h>
#include <action_solid.h>
#include <action_shell.h>
#include <action_curve3d.h>
#include <attr_hotpoint_attribute.h>
#include <op_direct_mod_parameter.h>
#include <solid.h>
#include <space_instance.h>
#include <plane_instance.h>
#include <wire_frame.h>
#include <point_frame.h>
#include <mesh.h>
#include <action_mesh.h>
#include <point3d.h>
#include <assembly.h>
#include <templ_dptr.h>
#include <tool_time_test.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <op_mesh_parameter.h>
#include <op_shell_parameter.h>
#include <test_varsect.h>
#include <last.h>

using namespace c3d;


// Включатели отрисовки во время отладки.
// State of debug drawing.
namespace DrawTestSolid {
  bool drawItems = false;
  bool checkPhantom = false;
  bool checkPosition = false;
  bool drawHotPoints = false;
}


//------------------------------------------------------------------------------
// Создание элементарного тела по точкам.
// Creation of an elementary solid by points.
// ---
void CreateElementarySolid( int solidType ) 
{
  MbCartPoint3D p[4];
  ElementaryShellType type = ElementaryShellType(solidType);
  if ( GetPoint( IDS_SELECT_POINT, p[0] ) ) {
    TestVariables::drawImp->DrawPoint( &p[0], TRGB_LIGHTGREEN );
    if ( GetPoint( IDS_SELECT_POINT, p[1] ) ) {
      TestVariables::drawImp->DrawPoint( &p[1], TRGB_LIGHTGREEN );
      if ( GetPoint( IDS_SELECT_POINT, p[2] ) ) {
        TestVariables::drawImp->DrawPoint( &p[2], TRGB_LIGHTGREEN );
        SpacePointsVector points;
        points.push_back( p[0] );
        points.push_back( p[1] );
        points.push_back( p[2] );
        MbSNameMaker names( ct_ElementarySolid, MbSNameMaker::i_SideNone, 0 );

        if ( (type == et_Cone) && GetPoint(IDS_SELECT_POINT, p[3]) ) {
          points.push_back( p[3] );
          TestVariables::drawImp->DrawPoint( &p[3], TRGB_LIGHTGREEN );
        }

        if ( (type == et_Prism) || (type == et_Pyramid) ) {
          while ( GetPoint( IDS_SELECT_POINT, p[3] ) ) {
            points.push_back( p[3] );
          }
        }
        if ( (type == et_Block) || (type == et_Wedge) || (type == et_Plate) ||
             (type == et_Prism) || (type == et_Pyramid) ) {
          double h = p[0].DistanceToPoint( p[1] );
          GetDouble( IDS_ENTER_THICKNESS, h );
          if ( h < METRIC_PRECISION )
            h = METRIC_PRECISION;
          MbVector3D to;
          MbCartPoint3D p0;

          if ( (type == et_Prism) || (type == et_Pyramid) ) {
            for ( size_t i = 0, iCount = points.size(); i < iCount; i++ )
              p0 += points[i];
            p0 /= (double)points.size();
          }
          to = MbVector3D( p[0], p[1] ) | MbVector3D( p[0], p[2] );
          to.Normalize();
          to *= h;
          p0.Move( to );
          points.push_back( p0 );
        }
        MbElementarySolidResults results;
        ::ElementarySolid( MbElementarySolidParams(type, points, names), results );
        if ( DrawTestSolid::drawHotPoints ) { // Отрисовка хот-точек. Hot points drawing.
          std::vector<SpacePointVector> hotPoints;
          if ( results.GetElementarySolidHotPoints(hotPoints) ) {
            for ( auto & hp : hotPoints ) {
              TestVariables::viewManager->AddObject( Style(), new MbLineSegment3D(hp.first, hp.second) );
              TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPoint3D(hp.first) );
            }
          }
        }
        if ( results.GetResultSolid() != nullptr )
          TestVariables::viewManager->AddObject( TestVariables::ELEMENTARY_Style, results.GetResultSolid() );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание элементарного тела по элементарной поверхности.
// Creation of an elementary solid by an elementary surface.
// ---
void CreateElementarySolid() {
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_ELEM_SURFACE, st_Surface, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    MbSurface *surface = (MbSurface *)((MbSpaceInstance *)obj)->GetSpaceItem(); 
    MbeSpaceType type = (MbeSpaceType)surface->Type();
    if ( type == st_ElementarySurface ) {
      SPtr<MbElementarySurface> elementary( (MbElementarySurface *)surface );
      MbSNameMaker names( ct_ElementarySolid, MbSNameMaker::i_SideNone, 0 );
      MbElementarySolidResults results;
      ::ElementarySolid( MbElementarySolidParams(elementary, names), results );
      if ( DrawTestSolid::drawHotPoints ) { // Отрисовка хот-точек. Hot points drawing.
        std::vector<SpacePointVector> hotPoints;
        if ( results.GetElementarySolidHotPoints(hotPoints) ) {
          for ( auto & hp : hotPoints ) {
            TestVariables::viewManager->AddObject( Style(), new MbLineSegment3D(hp.first, hp.second) );
            TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPoint3D(hp.first) );
          }
        }
      }
      if ( results.GetResultSolid() != nullptr )
        TestVariables::viewManager->AddObject( TestVariables::ELEMENTARY_Style, results.GetResultSolid() );
    }
    TestVariables::viewManager->ToggleObject(obj);
  }
}


/*
//------------------------------------------------------------------------------
// Создание тела по плоскому контуру
// ---
void CreatePlateSolid() {
  MbItem * curve = nullptr; // Образующая

  while ( GetAnyCurve ( IDS_SELECT_GENER_CURVE, &curve ) ) {
    MbPlacement3D placement;
    MbContour * contour = GetContourOnPlace( curve, placement );
    if ( contour != nullptr ) {
      viewManager->ToggleObject(curve);

      double h = 1;
      if ( GetDouble( IDS_ENTER_THICKNESS, h ) ) {
        MbSNameMaker names( ct_ElementarySolid, MbSNameMaker::i_SideNone, 0 );
        MbSolid * solid = nullptr;
        Style ns = SWEPT_Style;

        //MbResultType res = 
        //::PlateSolid( placement, *contour, h, names, solid );
        if ( solid!=nullptr )
          viewManager->AddObject( ns, solid );
      }
      viewManager->ToggleObject(curve);
    }
    ::DeleteItem( contour );
  }
}
*/


//------------------------------------------------------------------------------
// Создание икосаэдра.
// Creation of icosahedron.
// ---
void CreateIcosahedron( bool doSolid )
{
  double radius = 100.0;
  GetDouble( IDS_ENTER_RADIUS, radius );

  MbPlacement3D place;
  {
    MbCartPoint3D pnt0;
    ::GetPoint( IDS_SELECT_PLACEMENT, pnt0 );
    place.SetOrigin( pnt0 );
  }

  MbMesh * mesh( nullptr );
  MbFormNote formNote( true, true, true, TestVariables::exactMesh );
  ::CreateIcosahedron( place, radius, formNote, mesh );

  if ( mesh != nullptr && mesh->IsComplete() ) {
    Style st;
    if ( doSolid ) {
      MbSNameMaker snMaker;
      MbGridShellParams params( snMaker );
      SolidSPtr solid;
      ::MeshSolid( *mesh, params, solid );
      if ( solid != nullptr )
        TestVariables::viewManager->AddObject( st, solid );
    }
    else {
      TestVariables::viewManager->AddObject( st, mesh );
    }
  }
  ::DeleteItem( mesh );
}


//------------------------------------------------------------------------------
// Создание полиэдра, аппроксимирующего сферу.
// Creation of a polyhedron approximating a sphere.
// ---
void CreateSpherePolyhedron( bool doSolid )
{
  double radius = 100.0;
  GetDouble( IDS_ENTER_RADIUS, radius );

  MbPlacement3D place;
  {
    MbCartPoint3D pnt0;
    ::GetPoint( IDS_SELECT_PLACEMENT, pnt0 );
    place.SetOrigin( pnt0 );
  }
  double sag = 0.1;
  GetDouble( IDS_ENTER_ACCURACY, sag );

  MbMesh * mesh( nullptr );
  ::CreateSpherePolyhedron( place, radius, sag, mesh );

  if ( mesh != nullptr && mesh->IsComplete() ) {
    Style st;
    if ( doSolid ) {
      MbSNameMaker snMaker;
      MbGridShellParams params( snMaker );
      SolidSPtr solid;
      ::MeshSolid( *mesh, params, solid );
      if ( solid != nullptr )
        TestVariables::viewManager->AddObject( st, solid );
    }
    else {
      TestVariables::viewManager->AddObject( st, mesh );
    }
  }
  ::DeleteItem( mesh );
}


//------------------------------------------------------------------------------
// Создание выпуклой оболочки.
// Creation of a convex hull.
// ---
void CreateConvexHull()
{
  std::vector<MbMesh *> meshes;
  MbItem * obj = nullptr;
  while ( GetGObj( IDS_SELECT_MESH, st_Mesh, &obj ) ) {
    TestVariables::viewManager->ToggleObject( obj );
    meshes.push_back( static_cast<MbMesh *>( obj ) );
  }

  if ( !meshes.empty() )
  {
    double tol = EXTENT_EPSILON;
    if ( GetDouble( IDS_ENTER_ACCURACY, tol ) )
    {
      SpacePointsVector points;
      for ( const auto * pMesh : meshes )
      {
        if ( pMesh->GridsCount() > 0 )
        {
          for ( size_t j = 0, jCount = pMesh->GridsCount(); j < jCount; j++ )
          {
            const MbGrid * grid = pMesh->GetGrid( j );
            if ( grid != nullptr )
            {
              if ( grid->IsA() == pt_ExactGrid )
              {
                MbCartPoint3D point;
                for ( size_t k = 0, kCount = grid->PointsCount(); k < kCount; k++ ) {
                  grid->GetPoint( k, point );
                  points.push_back( point );
                }
              }
              else
              {
                MbFloatPoint3D point;
                for ( size_t k = 0, kCount = grid->PointsCount(); k < kCount; k++ ) {
                  grid->GetPoint( k, point );
                  points.emplace_back( point );
                }
              }
            }
          }
        }
      }

      if ( !points.empty() )
      {
        MbConvexHullParams3D params( points, tol );
        SPtr<MbMesh> result;
        if ( CreateConvexPolyhedron( params, result ) == rt_Success && result != nullptr )
        {
          Style ns = TestVariables::CONIC_Style;
          TestVariables::viewManager->AddObject( ns, result );
        }
      }
    }
  }

  for ( auto * pObj : meshes )
    TestVariables::viewManager->ToggleObject( pObj );
}


//------------------------------------------------------------------------------
// Создание прямоугольного параллелепипеда с NURBS-гранями по трем точкам.
// Creation of a rectangular parallelepiped with NURBS faces by three points.
// ---
void CreateNurbsBlockSolid3Points()
{
  MbCartPoint3D p[3];

  while ( GetPoint( IDS_SELECT_POINT, p[0] ) ) {
    TestVariables::drawImp->DrawPoint( &p[0], TRGB_LIGHTGREEN );
    if ( GetPoint( IDS_SELECT_POINT, p[1] ) ) {
      TestVariables::drawImp->DrawPoint( &p[1], TRGB_LIGHTGREEN );
      if ( GetPoint( IDS_SELECT_POINT, p[2] ) ) {
        TestVariables::drawImp->DrawPoint( &p[2], TRGB_LIGHTGREEN );
        double h = 50;
        if ( GetDouble( IDS_ENTER_STRETCHING, h )) {
          MbVector3D v1(p[0],p[1]), v2(p[0],p[2]);
          MbPlacement3D position(v1,v2,p[0]);
          double x = fabs( position.GetAxisX() * v1 );
          double y = fabs( position.GetAxisY() * v2 );
          double z = fabs( h );
          MbSNameMaker names( ct_ElementarySolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
          SimpleName name = 0;

          const ptrdiff_t minDegree = 2;
          // Порядок сплайна по u,v для поверхностей объекта.
          ptrdiff_t degree = Math::curveDegree; // Splines order by u,v for surfaces of the object.
          // Ввести порядок сплайнов. Input a splines order.
          GetInt( IDS_ENTER_SPLINE_ORDER, degree );
          if ( degree < minDegree )
            degree = minDegree;

          ptrdiff_t count = degree;
          // Введите число разбиений по u и по v. Input split count by u or by v.
          GetInt( IDS_ENTER_SPLIT_COUNT_U, count );
          if ( count < degree )
            count = degree;

          // По умолчанию поверхность ориентирована наружу ( для правой системы координат ).
          // By default a surface has outer orientation ( for right-handed coordinate system ) .
          bool bOutDir = true;  

          NurbsBlockValues param; // Параметры. Parameters.
          param.udeg[0] = degree; param.vdeg[0] = degree;
          param.udeg[1] = degree; param.vdeg[1] = degree;
          param.udeg[2] = degree; param.vdeg[2] = degree;
          param.ucnt[0] = count; param.vcnt[0] = count;
          param.ucnt[1] = count; param.vcnt[1] = count;
          param.ucnt[2] = count; param.vcnt[2] = count;

          MbSolid * solid = nullptr;

          MbResultType const res = ::NurbsBlockSolid( position, x, y, z, bOutDir, names, name, param, solid );
          if ( res == rt_Success && solid != nullptr ) {
            TestVariables::viewManager->AddObject( TestVariables::ELEMENTARY_Style, solid );
          }

        }
      }
    }
  }
} // CreateNurbsBlockSolid3Points()


//------------------------------------------------------------------------------
// Создать оболочку на основе NURBS-поверхности по точкам.
// Creation of a shell based on NURBS surface by points.
// ---
void CreateNurbsSurfacesSolid()
{
  ptrdiff_t uCount = 0;
  ptrdiff_t vCount = 0;
  MbCartPoint3D p;
  SArray<MbCartPoint3D> pList( 0, 1 );
  MbItem * objItem = nullptr;
  RPArray<MbItem> objItems( 10, 10 );

  bool isPointsCloud = false;

  TestVariables::viewManager->GetSelectedObjects( objItems );
  if ( objItems.Count() > 0 ) {
    pList.Reserve( objItems.Count() );
    for ( size_t k = 0, kcnt = objItems.Count(); k < kcnt; k++ ) {
      objItem = objItems[k];
      if ( objItem == nullptr )
        continue;

      if ( objItem->IsA() == st_PointFrame ) {
        MbPointFrame * pf = static_cast<MbPointFrame *>(objItem);
        size_t pCnt = pf->GetVerticesCount();
        pList.Reserve( pCnt );
        for ( size_t m = 0; m < pCnt; ++m ) {
          pf->GetCartPoint( m, p );
          pList.Add( p );
        }
      }
      else if ( objItem->IsA() == st_SpaceInstance ) {
        MbSpaceInstance * sInst = static_cast<MbSpaceInstance *>(objItem);
        if ( sInst->GetSpaceItem() != nullptr ) {
          const MbSpaceItem * sItem = sInst->GetSpaceItem();
          if ( sItem->IsA() == st_Point3D ) {
            const MbPoint3D & sPnt = static_cast<const MbPoint3D &>(*sItem);
            pList.Add( sPnt.GetCartPoint() );
          }
        }
      }
    }

    uCount = pList.Count();

    if ( uCount > 2 ) {
      vCount = pList.Count() / uCount;
      isPointsCloud = true;
    }
  }

  if ( !isPointsCloud ) {
    while( GetGObj( IDS_SELECT_POINT, st_Point3D, &objItem ) ) {
      bool getPoint = false;

      if ( objItem->IsA() == st_PointFrame ) {
        MbPointFrame * pf = static_cast<MbPointFrame *>(objItem);
        if ( pf->GetVerticesCount() == 1 )
          getPoint = pf->GetCartPoint( p );
      }
      else {
        const MbSpaceItem * geomItem = ((MbSpaceInstance *)objItem)->GetSpaceItem();  
        if ( geomItem != nullptr && geomItem->IsA() == st_Point3D ) {
          const MbPoint3D * po = static_cast<const MbPoint3D *>(geomItem);
          po->GetCartPoint( p );
          getPoint = true;
        }
      }

      if ( getPoint ) {
        TestVariables::viewManager->SelectObject( objItem );
        objItems.Add( objItem );
        pList.Add( p );
      }
    }

    uCount = pList.Count();

    if ( uCount > 0 ) {
      while( GetGObj( IDS_SELECT_POINT, st_Point3D, &objItem ) ) {
        bool getPoint = false;

        if ( objItem->IsA() == st_PointFrame ) {
          MbPointFrame * pf = static_cast<MbPointFrame *>(objItem);
          if ( pf->GetVerticesCount() == 1 )
            getPoint = pf->GetCartPoint( p );
        }
        else {
          const MbSpaceItem * geomItem = ((MbSpaceInstance *)objItem)->GetSpaceItem();  
          if ( geomItem != nullptr && geomItem->IsA() == st_Point3D ) {
            const MbPoint3D * po = static_cast<const MbPoint3D *>(geomItem);
            po->GetCartPoint( p );
            getPoint = true;
          }
        }

        if ( getPoint ) {
          TestVariables::viewManager->SelectObject( objItem );
          objItems.Add( objItem );
          pList.Add( p );
        }
      }
      vCount = pList.Count() / uCount;  
    }
  }

  if ( uCount > 1 && vCount == 1 )
    isPointsCloud = true;

  if ( (uCount > 1 && vCount > 1) || (isPointsCloud && uCount > 2 && vCount == 1) ) 
  {
    ptrdiff_t uDegree = Math::uSurfaceDegree;
    ptrdiff_t vDegree = Math::uSurfaceDegree;

    if ( !isPointsCloud ) {
      uDegree = std_min( uDegree, uCount );
      vDegree = std_min( vDegree, vCount );
      GetInt( IDS_ENTER_SPLINE_ORDER, uDegree );
      GetInt( IDS_ENTER_SPLINE_ORDER, vDegree );
    }
    else {
      uDegree = (ptrdiff_t)::floor(::sqrt( ((double)(uCount * vCount)) ));
      uDegree = std_max( uDegree, (ptrdiff_t)2 );
      uDegree = std_min( uDegree, (ptrdiff_t)4 );         //-V112
      vDegree = uDegree;
      if ( GetInt( IDS_ENTER_SPLINE_ORDER, uDegree ) )
        vDegree = uDegree;
      else {
        uDegree = vDegree = -1;
      }
    }
    
    if ( !isPointsCloud ) {
      uDegree = std_max( uDegree, (ptrdiff_t)2 );
      vDegree = std_max( vDegree, (ptrdiff_t)2 );
    }

    uDegree = std_min( uDegree, (ptrdiff_t)DELTA_MAX );
    vDegree = std_min( vDegree, (ptrdiff_t)DELTA_MAX );

    MbCartPoint3D midPnt;

    Array2<MbCartPoint3D> initPoints;
    
    if ( initPoints.SetSize( std_max( vCount, (ptrdiff_t)1 ), std_max( uCount, (ptrdiff_t)1 ) ) ) {
      for ( ptrdiff_t i = 0; i < vCount; i++ ) {
        for ( ptrdiff_t j = 0; j < uCount; j++ ) 
        {
          initPoints.Init( i, j, pList[i*uCount+j] );
          midPnt += initPoints( i, j );
        }
      }
      midPnt /= (double)(vCount * uCount);
    }
    else {
      PRECONDITION( false );
    }

    ptrdiff_t iUcls = 0;
    ptrdiff_t iVcls = 0;
    if ( !isPointsCloud ) {
      GetInt( IDS_ENTER_CLOSED, iUcls );
      GetInt( IDS_ENTER_CLOSED, iVcls );
      if ( iUcls != 0 ) iUcls = 1;
      if ( iVcls != 0 ) iVcls = 1;
    }

    ptrdiff_t iThroughPoints = 0;
    if ( uDegree > 1 ) {
      GetInt( IDS_ENTER_THROUGH_ITEMS, iThroughPoints );
      if ( iThroughPoints != 0 )
        iThroughPoints = 1;
    }

    ptrdiff_t iCheckSelfInt = 0;
    GetInt( IDS_ENTER_CHECKSELFINT, iCheckSelfInt );
    if ( iCheckSelfInt != 0 ) 
      iCheckSelfInt = 1;

    NurbsSurfaceValues nsParams;

    if ( isPointsCloud ) {
      ptrdiff_t iUseScreenPlace = 0;
      if ( uDegree > 1 ) {
        GetInt( IDS_ENTER_USE_OR_NOT, iUseScreenPlace );
      }

      MbPlacement3D * screenPlace = nullptr;
      if ( iUseScreenPlace != 0 ) {
        screenPlace = new MbPlacement3D( TestVariables::viewManager->GetCplane() );
        MbVector3D move( screenPlace->GetOrigin(), midPnt );
        screenPlace->Move( move );
      }

      nsParams.InitCloud( uDegree, initPoints, screenPlace, (iCheckSelfInt != 0) );

      if ( screenPlace != nullptr ) {
        delete screenPlace;
        screenPlace = nullptr;
      }
    }
    else
      nsParams.InitMesh( uDegree, (iUcls != 0), vDegree, (iVcls != 0), initPoints, (iCheckSelfInt != 0) );

    nsParams.SetThroughPoints( (iThroughPoints != 0) );

    MbSNameMaker snMaker( ct_NurbsBlockSolid, MbSNameMaker::i_SideNone, 0 );
    SolidSPtr solid;

    SetWaitCursor( true );
    uint miliSec = ::GetTickCountEx();

    bool isPhantom = false;

    DPtr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyPointsSurface ) );

    MbNurbsSurfacesShellParams shellParams( nsParams, snMaker, isPhantom );
    MbResultType res = ::NurbsSurfacesShell( shellParams, solid, progBar );

    miliSec = ::GetTickCountEx() - miliSec;
    SetWaitCursor( false );

    ::ShowTimeTestResult( nullptr, miliSec, IDS_PBAR_BUILD_SHELL_BY_POINTS_MESH );

    if ( res == rt_Success && solid != nullptr )
      TestVariables::viewManager->AddObject( TestVariables::ELEMENTARY_Style, solid );
    else {
      if ( res == rt_Success )
        res = rt_Error;

      if ( progBar->IsCancel() )
        ::MessageBoxEx( RT_PROCESS_IS_STOPPED, IDS_PBAR_BUILD_SHELL_BY_POINTS_MESH );
      else
        ::PutErrorMessage( res, IDS_TITLE_ERR );
    }
  }

  TestVariables::viewManager->UnselectModel();
}


//------------------------------------------------------------------------------
// Создание тела выдавливания по плоской или поверхностной кривой.
// Creation of an extrusion solid by a plane curve or curve on a surface. 
// ---
void CreateCurveExtrusionSolid( int type, bool closed )
{
  MbItem * curve = nullptr; // Образующая. Generatrix.
  MbItem * curve0 = nullptr;

  ExtrusionValues param;
  param.shellClosed = closed;
  param.SetMergingFaces( TestVariables::mergeFaces );
  param.SetMergingEdges( TestVariables::mergeEdges );

  MbSolid * resultSolid = nullptr;

  RPArray<MbContour> contours( 0, 1 );
  MbPlacement3D contoursPlace;

  SPtr<MbSurface> surface;
  SPtr<MbFace> contoursFace;
  MbVector3D direction;

  Style ns = TestVariables::SWEPT_Style;
  MbResultType res = rt_Error;

  while ( ::GetAnyCurve( IDS_SELECT_GENER_CURVE, &curve ) ) {
    if ( curve != curve0 ) {
      MbPlacement3D cntrPlace;
      MbContour * contour = ::GetContourOnPlace( curve, cntrPlace );
      if ( contour != nullptr ) {
        contours.push_back( contour );
        if ( curve0 != nullptr ) {
          TestVariables::viewManager->ToggleObject( curve0 );
          if ( !contoursPlace.Colinear( cntrPlace ) )
            break;
          MbMatrix mtr;
          if ( cntrPlace.GetMatrixToPlace( contoursPlace, mtr ) && !mtr.IsSingle() )
            contour->Transform( mtr );
        }
        else {
          contoursPlace.Init( cntrPlace );
          surface = new MbPlane( contoursPlace );
          direction = contoursPlace.Normal();
        }
        TestVariables::viewManager->ToggleObject(curve);
        curve0 = curve;
      }
    }
  }
  if ( curve0 != nullptr )
    TestVariables::viewManager->ToggleObject(curve0);

  if ( contours.size() < 1 ) {
    MbItem * obj = nullptr;
    MbTopItem * topItem = nullptr;
    if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &topItem, TestVariables::itemPathFrom.second ) ) {
      contoursFace = static_cast<MbFace *>(topItem);
      surface = contoursFace->GetSurfaceCurvesData( contours );
      surface->_Normal( contoursFace->GetSurface().GetUMid(), contoursFace->GetSurface().GetVMid(), direction );
      MbCartPoint3D p( direction.x, direction.y, direction.z );
      ::GetExactPoint( p );
      direction = p;
    }
  }

  if ( contours.size() < 1 ) { // Для большого количества выделенных кривых. For a lot of selected curves.
    RPArray<MbItem> selItems( 0, 1 );
    TestVariables::viewManager->GetSelectedObjects( selItems ); // Нахождение выделенных объектов. Find selected objects.
    for ( size_t i = 0, iCount = selItems.size(); i < iCount; i++ ) {
      MbPlacement3D cntrPlace;
      MbContour * contour = GetContourOnPlace( selItems[i], cntrPlace );
      if ( contour != nullptr ) {
        contours.push_back( contour );
        if ( curve0 != nullptr ) {
          if ( contoursPlace.Colinear( cntrPlace ) ) {
            MbMatrix mtr;
            if ( cntrPlace.GetMatrixToPlace( contoursPlace, mtr ) && !mtr.IsSingle() )
              contour->Transform( mtr );
          }
        }
        else {
          contoursPlace.Init( cntrPlace );
          surface = new MbPlane( contoursPlace );
          direction = contoursPlace.Normal();
          curve0 = selItems[i];
        }
      }
    }
  }

  ::AddRefItems( contours );

  if ( contours.size() > 0 && surface != nullptr ) {
    if ( GetDouble( IDS_ENTER_STRETCHING, TestVariables::depth1 ) && GetDouble( IDS_ENTER_ANGLE, TestVariables::angle1 ) &&
         GetDouble( IDS_ENTER_STRETCHING, TestVariables::depth2 ) && GetDouble( IDS_ENTER_ANGLE, TestVariables::angle2 ) ) {
      if ( GetDouble( IDS_ENTER_THICKNESS, TestVariables::thickness1 ) && GetDouble( IDS_ENTER_THICKNESS, TestVariables::thickness2 ) ) {
        param.side1.scalarValue = TestVariables::depth1;
        param.side1.rake        = TestVariables::angle1*M_PI/180;
        param.side2.scalarValue = TestVariables::depth2;
        param.side2.rake        = TestVariables::angle2*M_PI/180;
        param.thickness1 = TestVariables::thickness1;
        param.thickness2 = TestVariables::thickness2;

        MbSNameMaker operNames( ct_CurveExtrusionSolid, MbSNameMaker::i_SideNone, 0 );
        operNames.SetVersion( MbNameVersion() );

        PArray<MbSNameMaker> cNames( 0, 1, false );

        MbSweptData curvesData( *surface, contours );

        bool checkIntersection = false;

        switch ( type ) {

          case  1 : // Объединение. Union
          case -1 : // Вычитание. Difference.
          case -2 : // Пересечение. Intersection.
          {
            MbItem * obj( nullptr ), * obj1( nullptr ), * faceObj( nullptr );
            MbTopItem * face( nullptr );
            MbSolid * solid( nullptr );
            int cs = -1;

            if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj, TestVariables::itemPathFrom ) &&
                 GetInt ( IDS_ENTER_BUILD_METHOD, cs ) &&
                 ((cs == 0) || GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &faceObj, &face, TestVariables::itemPathFrom.second )) &&
                 (cs >= 0) ) {
              TestVariables::viewManager->ToggleObject(obj);
              TestVariables::drawImp->DrawItem( face, TRGB_WHITE );
              TestVariables::viewManager->ToggleObject(obj);
              ns.Init( obj->GetWidth(), obj->GetColor() );
              solid = static_cast<MbSolid *>(obj);

              if ( cs == 0 ) {
                param.side1.way = sw_shell; // Выдавить до объекта. Extrude to an object.
              }
              else {
                double dist = 0;
                GetDouble( IDS_ENTER_DISTANCE, dist );
                param.SetSurface1( &((MbFace *)face)->SetSurface() );
                param.side1.way = sw_surface; // Выдавить до поверхности. Extrude to a surface.
                param.side1.distance = dist;
              }
            }
            cs = -1;
            if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj1, TestVariables::itemPathFrom ) &&
                 GetInt ( IDS_ENTER_BUILD_METHOD, cs ) &&
                 ((cs == 0) || GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &faceObj, &face, TestVariables::itemPathFrom.second )) &&
                 (cs >= 0) &&
                 (obj==obj1 || solid==nullptr) ) {
              obj = obj1;
              TestVariables::viewManager->ToggleObject(obj);
              TestVariables::drawImp->DrawItem( face, TRGB_WHITE );
              TestVariables::viewManager->ToggleObject(obj);
              ns.Init( obj->GetWidth(), obj->GetColor() );
              solid = static_cast<MbSolid *>(obj);

              if ( cs == 0 ) {
                param.side2.way = sw_shell; // Выдавить до объекта. Extrude to an object.
              }
              else {
                double dist = 0;
                GetDouble( IDS_ENTER_DISTANCE, dist );
                param.SetSurface2( &((MbFace *)face)->SetSurface() );
                param.side2.way = sw_surface; // Выдавить до поверхности. Extrude to a surface.
                param.side2.distance = dist;
              }
            }

            if ( solid == nullptr && obj != nullptr && (obj1 == obj || obj1 == nullptr)) {
              solid = static_cast<MbSolid *>(obj);
            }

            if ( solid != nullptr ) {
              static MbeCopyMode solidCopyMode = cm_KeepHistory; // cm_Copy;

              SetWaitCursor( true );
              if ( type == 1 ) {
                res = ::ExtrusionResult( *solid, solidCopyMode, curvesData, direction, param, bo_Union, operNames, cNames, resultSolid );
              }
              else {
                if ( type == -1 ) {
                  res = ::ExtrusionResult( *solid, solidCopyMode, curvesData, direction, param, bo_Difference, operNames, cNames, resultSolid );
                }
                else {
                  res = ::ExtrusionResult( *solid, solidCopyMode, curvesData, direction, param, bo_Intersect, operNames, cNames, resultSolid );
                }
              }
              SetWaitCursor( false );
            }

            if ( resultSolid != nullptr ) {
              TestVariables::viewManager->ToggleObject( obj );
              TestVariables::viewManager->DeleteObject( obj );
            }
          } break;

          default : { // Построение тела. Creation of a solid.
            bool make = true;
            int way( 2 );

            GetInt ( IDS_ENTER_BUILD_METHOD, way ); 

            if ( way == 0 ) { // Исходное. Original.
              MbSolid * solid1 = nullptr;
              MbSolid * solid2 = nullptr;
              MbItem * obj( nullptr );

              if ( GetGObj ( IDS_SELECT_SOLID, st_Solid, &obj, TestVariables::itemPathFrom ) ) {
                solid1 = (MbSolid *)obj;
                if ( solid1 != nullptr ) {
                  param.side1.way = sw_shell; // Выдавить до объекта. Extrude to an object.
                }
              }
              if ( GetGObj ( IDS_SELECT_SOLID, st_Solid, &obj, TestVariables::itemPathFrom ) ) {
                solid2 = static_cast<MbSolid *>(obj);
                if ( solid2 != nullptr ) {
                  param.side2.way = sw_shell; // Выдавить до объекта. Extrude to an object.
                }
              }
              if ( (solid1 != nullptr) || (solid2 != nullptr) ) {
                SetWaitCursor( true );

                res = ::ExtrusionSolid( curvesData, direction, solid1, solid2, checkIntersection, param, operNames, cNames, resultSolid );

                SetWaitCursor( false );
                make = false;
              }
            }
            if ( way == 1 ) { // Original.
              MbSurface * surface1 = nullptr;
              MbSurface * surface2 = nullptr;
              MbItem * obj( nullptr );

              if ( GetGObj ( IDS_SELECT_SURFACE, st_Surface, &obj, TestVariables::itemPathFrom ) ) {
                surface1 = (MbSurface *)((MbSpaceInstance *)obj)->GetSpaceItem();
                if ( surface1 != nullptr ) {
                  double dist = 0;
                  GetDouble( IDS_ENTER_DISTANCE, dist );
                  param.SetSurface1( &surface1->SetSurface() );
                  param.side1.way = sw_surface; // Выдавить до поверхности. Extrude to a surface.
                  param.side1.distance = dist;
                }
              }
              if ( GetGObj ( IDS_SELECT_SURFACE, st_Surface, &obj, TestVariables::itemPathFrom ) ) {
                surface2 = (MbSurface *)((MbSpaceInstance *)obj)->GetSpaceItem();
                if ( surface2 != nullptr ) {
                  double dist = 0;
                  GetDouble( IDS_ENTER_DISTANCE, dist );
                  param.SetSurface2( &surface2->SetSurface() );
                  param.side2.way = sw_surface; // Выдавить до поверхности. Extrude to a surface.
                  param.side2.distance = dist;
                }
              }
              if ( (surface1 != nullptr) || (surface2 != nullptr) ) {
                SetWaitCursor( true );

                res = ::ExtrusionSolid( curvesData, direction, nullptr, nullptr, checkIntersection, param, operNames, cNames, resultSolid );

                SetWaitCursor( false );
                make = false;
              }
            }
            if ( make && resultSolid == nullptr) {
              SetWaitCursor( true );
              res = ::ExtrusionSolid( curvesData, direction, nullptr, nullptr, checkIntersection, param, operNames, cNames, resultSolid );
              SetWaitCursor( false );
            }
          } break;

        }
      }
    }
  }

  ::ReleaseItems( contours );

  if ( resultSolid == nullptr )
    ::PutErrorMessage( res, IDS_TITLE_ERR );

  if ( resultSolid != nullptr ) {
    TestVariables::viewManager->AddObject( ns, resultSolid );
  }
}


//------------------------------------------------------------------------------
// Создание тела выдавливания по пространственной кривой.
// Creation of an extrusion solid by a spatial curve.
// ---
void CreateCurve3DExtrusionSolid( int type, bool closed, bool bWireFrame )
{
  OperationType oType = (OperationType)type;

  ExtrusionValues param;
  param.shellClosed = closed;
  param.SetMergingFaces( TestVariables::mergeFaces );
  param.SetMergingEdges( TestVariables::mergeEdges );

  MbSolid * resultSolid = nullptr;

  Style ns = TestVariables::SWEPT_Style;
  MbResultType res = rt_Error;
  MbVector3D direction;

  DPtr<MbSweptData> curvesData( ::SelectSweptData( bWireFrame ) );

  {
    MbItem * directionObj = nullptr;
    if ( GetAnyCurve ( IDS_ENTER_DIRECTION_SEG, &directionObj ) &&
         GetCurve3D( directionObj, true )->IsStraight() ) {
      GetCurve3D( directionObj, true )->_FirstDer( 0, direction );
      TestVariables::viewManager->ToggleObject( directionObj );
    }
    else {
      MbCartPoint3D dirPoint;
      if ( GetPoint( IDS_ENTER_DIR, dirPoint ) ) {
        direction.Init( dirPoint );
      }
    }
  }

  if ( !direction.IsZero() && curvesData != nullptr ) {
    if ( GetDouble( IDS_ENTER_STRETCHING, TestVariables::depth1 ) && 
         GetDouble( IDS_ENTER_STRETCHING, TestVariables::depth2 ) ) {
      if ( GetDouble( IDS_ENTER_THICKNESS, TestVariables::thickness1 ) && 
           GetDouble( IDS_ENTER_THICKNESS, TestVariables::thickness2 ) ) {
        param.side1.scalarValue = TestVariables::depth1;
        param.side1.rake        = 0;
        param.side2.scalarValue = TestVariables::depth2;
        param.side2.rake        = 0;
        param.thickness1 = TestVariables::thickness1;
        param.thickness2 = TestVariables::thickness2;

        switch ( type ) {   
          case  1 : // Объединение. Union.
          case -1 : // Вычитание. Difference.
          case -2 : // Пересечение. Intersection.
          {
            MbItem * obj( nullptr ), * obj1( nullptr ), * faceObj( nullptr );
            MbTopItem * face( nullptr );
            MbSolid * solid = nullptr;
            int cs = -1;

            if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj, TestVariables::itemPathFrom ) &&
                 GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &faceObj, &face, TestVariables::itemPathFrom.second ) &&
                 GetInt ( IDS_ENTER_BUILD_METHOD, cs ) &&
                 cs >= 0 ) {
              TestVariables::viewManager->ToggleObject(obj);
              TestVariables::drawImp->DrawItem( face, TRGB_WHITE );

              TestVariables::viewManager->ToggleObject(obj);
              ns.Init( obj->GetWidth(), obj->GetColor() );
              solid = (MbSolid *)obj;

              if ( cs == 0 ) {
                param.side1.way = sw_shell; // Выдавить до объекта. Extrude to an object.
              }
              else {
                double dist = 0;
                GetDouble( IDS_ENTER_DISTANCE, dist );
                param.SetSurface1( &((MbFace *)face)->SetSurface() );
                param.side1.way = sw_surface; // Выдавить до поверхности. Extrude to a surface.
                param.side1.distance = dist;
              }
            }
            cs = -1;
            if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj1, TestVariables::itemPathFrom ) &&
                 GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &faceObj, &face, TestVariables::itemPathFrom.second ) &&
                 GetInt ( IDS_ENTER_BUILD_METHOD, cs ) && 
                 cs>=0 &&
                 (obj==obj1 || solid==nullptr) ) {
              obj = obj1;
              TestVariables::viewManager->ToggleObject(obj);
              TestVariables::drawImp->DrawItem( face, TRGB_WHITE );

              TestVariables::viewManager->ToggleObject(obj);
              ns.Init( obj->GetWidth(), obj->GetColor() );
              solid = (MbSolid *)obj;

              if ( cs==0 ) {
                param.side2.way = sw_shell; // Выдавить до объекта. Extrude to an object.
              }
              else {
                double dist = 0;
                GetDouble( IDS_ENTER_DISTANCE, dist );
                param.SetSurface2( &((MbFace *)face)->SetSurface() );
                param.side2.way = sw_surface; // Выдавить до поверхности. Extrude to a surface.
                param.side2.distance = dist;
              }
            }

            if ( solid!=nullptr ) {
              MbSNameMaker operNames( ct_CurveExtrusionSolid, 
                                      MbSNameMaker::i_SideNone, 0 );
              operNames.SetVersion( MbNameVersion() );

              MbSNameMaker cName( ct_CurveExtrusionSolid, MbSNameMaker::i_SideNone, 0 );
              PArray<MbSNameMaker> cNames( 0, 1, false );
              cNames.Add( &cName );

              SetWaitCursor( true );
              res = ::ExtrusionResult( *solid, cm_Copy, 
                                       *curvesData, direction, param, oType,  
                                       operNames, cNames, 
                                       resultSolid );

              SetWaitCursor( false );
            }

            if (resultSolid!=nullptr) {
              TestVariables::viewManager->ToggleObject(obj);
              TestVariables::viewManager->DeleteObject( obj );
            }
          } break;

          default : {
            bool make = true;

            MbSNameMaker operNames( ct_CurveExtrusionSolid, 
                                    MbSNameMaker::i_SideNone, 0 );
            operNames.SetVersion( MbNameVersion() );

            MbSNameMaker cName( ct_CurveExtrusionSolid, MbSNameMaker::i_SideNone, 0 );
            PArray<MbSNameMaker> cNames( 0, 1, false );
            cNames.Add( &cName );

            if ( type == 2 ) { // Исходное. Original.
              MbItem  * obj( nullptr );
              MbSolid * solid = nullptr;
              if ( GetGObj ( IDS_SELECT_SOLID, st_Solid, &obj, TestVariables::itemPathFrom ) ) {
                solid = static_cast<MbSolid *>(obj);

                if ( solid != nullptr ) {
                  param.side1.way = sw_shell; // Выдавить до объекта. Extrude to an object.
                  param.side2.way = sw_shell; // Выдавить до объекта. Extrude to an object.

                  SetWaitCursor( true );
                  res = ::ExtrusionResult( *solid, cm_Copy, 
                                           *curvesData, direction, param, oType, 
                                           operNames, cNames, 
                                           resultSolid );
                  SetWaitCursor( false );
                  make = false;
                }
              }
            }
            if ( make && resultSolid==nullptr) {
              SetWaitCursor( true );
              MbSolid *dummySolid(0);
              bool dummyBool(false);
              res = ::ExtrusionSolid( *curvesData, direction, 
                                      dummySolid, dummySolid, 
                                      dummyBool, param, operNames, 
                                      cNames, resultSolid );
              SetWaitCursor( false );
            }
          } break;

        }
      }
    }
  }

  if ( resultSolid == nullptr )
    ::PutErrorMessage( res, IDS_TITLE_ERR );

  if ( resultSolid != nullptr )
    TestVariables::viewManager->AddObject( ns, resultSolid );
}


//------------------------------------------------------------------------------
// Создание тела выдавливания по пространственной кривой.
// Creation of an extrusion solid by a spatial curve.
// ---
void CreateCurve3DExtrusionSolid( int type, bool closed )
{
  CreateCurve3DExtrusionSolid( type, closed, false );
}


//------------------------------------------------------------------------------
// Создание тела выдавливания.
// Creation of an extrusion solid.
// ---
void CreateSolidExtrusionSolid()
{
  MbItem * obj(0); 

  ExtrusionValues params;
  params.SetMergingFaces( TestVariables::mergeFaces );
  params.SetMergingEdges( TestVariables::mergeEdges );

  MbResultType res = rt_Error;
  MbVector3D direction;
  PathMatrix objPathFrom;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj, objPathFrom ) ) {
    TestVariables::viewManager->ToggleObject( objPathFrom.first );

    MbSolid * resultSolid = nullptr;
    MbSolid * solid = static_cast<MbSolid *>(obj);

    MbCartPoint3D pnt1, pnt2;
    Style ns( obj->GetWidth(), obj->GetColor() );

    MbCartPoint3D p[2];
    if ( GetPoint( IDS_SELECT_START_POINT, p[0] ) && GetPoint( IDS_SELECT_FINAL_POINT, p[1] ) ) {
      direction.Init( p[0], p[1] );
      params.side1.scalarValue = direction.Length();
      params.side1.rake        = 0;
      params.side2.scalarValue = 0;
      params.side2.rake        = 0;
      params.thickness1 = 0;
      params.thickness2 = 0;

      MbSweptData sweptData( *solid );

      MbSNameMaker operNames( ct_CurveExtrusionSolid, MbSNameMaker::i_SideNone, 0 );
      operNames.SetVersion( MbNameVersion() );

      MbSNameMaker cName( ct_CurveExtrusionSolid, MbSNameMaker::i_SideNone, 0 );
      PArray<MbSNameMaker> cNames( 0, 1, false );
      cNames.Add( &cName );

      SetWaitCursor( true );
      
      res = ::ExtrusionSolid( sweptData, direction, 
                              nullptr, nullptr, false,
                              params, operNames, 
                              cNames, resultSolid );

      SetWaitCursor( false );

      if ( resultSolid == nullptr ) {
        TestVariables::viewManager->ToggleObject( objPathFrom.first );
        ::PutErrorMessage( res, IDS_TITLE_ERR );
      }
      else
      {
        TestVariables::viewManager->DeleteObject( obj );
        TestVariables::viewManager->AddObject( ns, resultSolid );
      }
    }
    else {
      TestVariables::viewManager->ToggleObject( objPathFrom.first );
    }
  }
}


//------------------------------------------------------------------------------
// Создание тела вращения.
// Creation of a revolution solid.
// ---
void CreateCurveRevolutionSolid( int type, bool closed )
{
  MbItem * curve( nullptr );
  MbItem * curve0 = nullptr;

  RevolutionValues param;
  param.shellClosed = closed;
  param.SetMergingFaces( TestVariables::mergeFaces );
  param.SetMergingEdges( TestVariables::mergeEdges );

  MbSolid * resultSolid = nullptr;
  RPArray<MbContour> c( 2, 1 );
  MbSurface * surface = nullptr;
  MbAxis3D axis;
  MbPlacement3D pl;
  Style ns = TestVariables::SWEPT_Style;
  MbResultType res = rt_Error;

  while ( GetAnyCurve ( IDS_SELECT_GENER_CURVE, &curve ) ) {
    MbPlacement3D crvPlace;
    MbContour * crv = GetContourOnPlace( curve, crvPlace );
    if ( crv!=nullptr ) {
      c.Add( crv );
      if (curve0!=nullptr) {
        TestVariables::viewManager->ToggleObject(curve0);
        if ( !pl.Colinear(crvPlace) || curve==curve0 )
          break;
        MbMatrix m;
        if ( crvPlace.GetMatrixToPlace( pl, m ) && !m.IsSingle() )
          crv->Transform( m );
      }
      else {
        pl.Init(crvPlace);
        surface = new MbPlane(pl);
        surface->AddRef();
      }
      TestVariables::viewManager->ToggleObject(curve);
      curve0=curve;
    }
  }
  if (curve0!=nullptr)
    TestVariables::viewManager->ToggleObject(curve0);

  if ( c.Count() == 0 ) {
    MbItem * obj = nullptr;
    MbTopItem * top = nullptr;
    if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &top, TestVariables::itemPathFrom.second ) ) {
      MbFace * face = (MbFace *)top;
      const MbSurface & faceSurface = face->GetSurface().GetSurface();
      surface = (MbSurface *)&faceSurface.Duplicate();
      surface->AddRef();
      for ( size_t iLoop = 0, cntLoops = face->GetLoopsCount(); iLoop < cntLoops; iLoop++ ) {
        MbContour * crv = &face->GetLoop(iLoop)->MakeContour( faceSurface, face->IsSameSense() );
        c.Add( crv );
      }
    }
  }

  if ( c.Count() ==  0 ) {
    RPArray<MbItem> objs( 20, 1 );
    TestVariables::viewManager->GetSelectedObjects( objs );
    for ( size_t i = 0, iCount = objs.Count(); i < iCount; i++ ) {
      MbPlacement3D crvPlace;
      MbContour * crv = GetContourOnPlace( objs[i], crvPlace );
      if ( crv != nullptr ) {
        c.Add( crv );
        if (curve0 != nullptr) {
          if ( pl.Colinear(crvPlace) ) {
            MbMatrix m;
            if ( crvPlace.GetMatrixToPlace( pl, m ) && !m.IsSingle() )
              crv->Transform( m );
          }
        }
        else {
          pl.Init(crvPlace);
          surface = new MbPlane(pl);
          surface->AddRef();
          curve0 = objs[i];
        }
      }
    }
  }

  ::AddRefItems( c );

  if ( c.Count() > 0 && surface != nullptr ) {
    MbCartPoint3D pnt1, pnt2;

    if ( GetPoint( IDS_SELECT_POINT, pnt1 ) && GetPoint( IDS_SELECT_POINT, pnt2 ) ) {
      MbCartPoint3D surfP[2] = { pnt1, pnt2 };
      if ( surface->IsA() == st_Plane ) {
        MbCartPoint p1, p2;
        pl.PointProjection( pnt1, p1.x, p1.y );
        pl.PointProjection( pnt2, p2.x, p2.y );
        surface->_PointOn( p1[0], p1[1], surfP[0] );
        surface->_PointOn( p2[0], p2[1], surfP[1] );
      }
      axis.Init( surfP[0], surfP[1] ); 
      if ( GetDouble( IDS_ENTER_ANGLE, TestVariables::revolve1 ) && 
           GetDouble( IDS_ENTER_ANGLE, TestVariables::revolve2 ) ) {
        if ( GetDouble( IDS_ENTER_THICKNESS, TestVariables::thickness1 ) && 
             GetDouble( IDS_ENTER_THICKNESS, TestVariables::thickness2 ) ) {
          param.side1.scalarValue = TestVariables::revolve1*M_PI/180;
          param.side2.scalarValue = TestVariables::revolve2*M_PI/180;
          param.thickness1 = TestVariables::thickness1;
          param.thickness2 = TestVariables::thickness2;

          if ( !c[0]->IsClosed() )
            GetInt( IDS_ENTER_SHAPE, param.shape );

          MbSNameMaker operNames( ct_CurveRevolutionSolid, MbSNameMaker::i_SideNone, 0 );
          operNames.SetVersion( MbNameVersion() );

          PArray<MbSNameMaker> cNames( 0, 1, false );

          MbSweptData curvesData( *surface, c );

          switch ( type ) {

            case  1 : // Объединение. Union.
            case -1 : // Вычитание. Difference.
            case -2 : // Пересечение. Intersection.
            {
              MbItem * obj( nullptr ), * obj1( nullptr ), * faceObj( nullptr );
              MbTopItem * face( nullptr );
              MbSolid * solid( nullptr );
              int cs = -1;

              if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj, TestVariables::itemPathFrom ) ) {
                ns.Init( obj->GetWidth(), obj->GetColor() );
                solid = (MbSolid *)obj;
                if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &faceObj, &face, TestVariables::itemPathFrom.second ) &&
                     GetInt ( IDS_ENTER_BUILD_METHOD, cs ) &&
                     cs>=0 ) {
                  TestVariables::viewManager->ToggleObject(obj);
                  TestVariables::drawImp->DrawItem( face, TRGB_WHITE );
                  TestVariables::viewManager->ToggleObject(obj);

                  if ( cs==0 ) {
                    param.side1.way = sw_shell;
                  }
                  else {
                    double dist = 0;
                    GetDouble( IDS_ENTER_DISTANCE, dist );
                    param.SetSurface1( &((MbFace *)face)->SetSurface() );
                    param.side1.way = sw_surface;
                    param.side1.distance = dist;
                  }
                }
              }
              cs = -1;
              if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj1, TestVariables::itemPathFrom ) ) {
                ns.Init( obj1->GetWidth(), obj1->GetColor() );
                solid = (MbSolid *)obj1;
                  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &faceObj, &face, TestVariables::itemPathFrom.second ) &&
                     GetInt ( IDS_ENTER_BUILD_METHOD, cs ) && 
                     cs>=0 &&
                     (obj==obj1 || solid==nullptr) ) {
                  obj = obj1;
                  TestVariables::viewManager->ToggleObject(obj);
                  TestVariables::drawImp->DrawItem( face, TRGB_WHITE );
                  TestVariables::viewManager->ToggleObject(obj);

                  if ( cs==0 ) {
                    param.side2.way = sw_shell;
                  }
                  else {
                    double dist = 0;
                    GetDouble( IDS_ENTER_DISTANCE, dist );
                    param.SetSurface2( &((MbFace *)face)->SetSurface() );
                    param.side2.way = sw_surface;
                    param.side2.distance = dist;
                  }
                }
              }

              if ( solid != nullptr ) {
                SetWaitCursor( true );
                if (type==1)
                  res = ::RevolutionResult( *solid, cm_Copy, curvesData, axis, param, bo_Union, operNames, cNames, resultSolid );
                else {
                  if (type==-1)
                    res = ::RevolutionResult( *solid, cm_Copy, curvesData, axis, param, bo_Difference, operNames, cNames, resultSolid );
                  else 
                    res = ::RevolutionResult( *solid, cm_Copy, curvesData, axis, param, bo_Intersect, operNames, cNames, resultSolid );
                }
                SetWaitCursor( false );
              }

              if (resultSolid != nullptr) {
                TestVariables::viewManager->ToggleObject(obj);
                TestVariables::viewManager->DeleteObject( obj );
              }
            } break;

            default : {
              int way( 2 );

              GetInt ( IDS_ENTER_BUILD_METHOD, way ); 

              if ( way == 1 ) {
                MbSurface * surface1 = nullptr;
                MbSurface * surface2 = nullptr;
                MbItem * obj( nullptr );

                if ( GetGObj ( IDS_SELECT_SURFACE, st_Surface, &obj, TestVariables::itemPathFrom ) ) {
                  surface1 = (MbSurface *)((MbSpaceInstance *)obj)->GetSpaceItem();
                  if ( surface1 != nullptr ) {
                    double dist = 0;
                    GetDouble( IDS_ENTER_DISTANCE, dist );
                    param.SetSurface1( &surface1->SetSurface() );
                    param.side1.way = sw_surface;
                    param.side1.distance = dist;
                  }
                }
                if ( GetGObj ( IDS_SELECT_SURFACE, st_Surface, &obj, TestVariables::itemPathFrom ) ) {
                  surface2 = (MbSurface *)((MbSpaceInstance *)obj)->GetSpaceItem();
                  if ( surface2 != nullptr ) {
                    double dist = 0;
                    GetDouble( IDS_ENTER_DISTANCE, dist );
                    param.SetSurface2( &surface2->SetSurface() );
                    param.side2.way = sw_surface;
                    param.side2.distance = dist;
                  }
                }
              }
              SetWaitCursor( true );
              res = ::RevolutionSolid( curvesData, axis, param, operNames, cNames, resultSolid );
              SetWaitCursor( false );
            } break;
          }
        }
      }
    }
  }
  ::ReleaseItem( surface );
  ::ReleaseItems( c );

  if ( resultSolid == nullptr )
    ::PutErrorMessage( res, IDS_TITLE_ERR );

  if ( resultSolid!=nullptr )
    TestVariables::viewManager->AddObject( ns, resultSolid );
}


//------------------------------------------------------------------------------
// Создание тела вращения по трехмерной кривой.
// Creation of a revolution solid by a spatial curve.
// ---
void CreateCurve3DRevolutionSolid( int type, bool closed, bool bWireFrame )
{
  OperationType oType = (OperationType)type;
  MbVector3D vect(0,0,0);
  MbCartPoint3D orig, p;

  RevolutionValues param;
  param.shellClosed = closed;
  param.SetMergingFaces( TestVariables::mergeFaces );
  param.SetMergingEdges( TestVariables::mergeEdges );

  MbSolid * resultSolid = nullptr;
  Style ns = TestVariables::SWEPT_Style;
  MbResultType res = rt_Error;
  MbVector3D direction;

  DPtr<MbSweptData> curvesData( SelectSweptData( bWireFrame ) );

  {
    MbCartPoint3D pnt1, pnt2;
    if ( GetPoint( IDS_SELECT_POINT, orig ) && GetPoint( IDS_SELECT_POINT, p ) ) {
      vect.Init( orig, p ); 
    }
  }

  if ( !vect.IsZero() && curvesData != nullptr ) {

    if ( GetDouble( IDS_ENTER_ANGLE, TestVariables::revolve1 ) && 
         GetDouble( IDS_ENTER_ANGLE, TestVariables::revolve2 ) ) {
      if ( GetDouble( IDS_ENTER_THICKNESS, TestVariables::thickness1 ) && 
           GetDouble( IDS_ENTER_THICKNESS, TestVariables::thickness2 ) ) {
        param.side1.scalarValue = TestVariables::revolve1*M_PI/180;
        param.side2.scalarValue = TestVariables::revolve2*M_PI/180;
        param.thickness1 = TestVariables::thickness1;
        param.thickness2 = TestVariables::thickness2;
        param.side1.rake        = 0;
        param.side2.rake        = 0;

        MbAxis3D axis_(orig, vect);

        MbSNameMaker operNames( ct_CurveRevolutionSolid, MbSNameMaker::i_SideNone, 0 );
        operNames.SetVersion( MbNameVersion() );

        MbSNameMaker cName( ct_CurveRevolutionSolid, MbSNameMaker::i_SideNone, 0 );
        PArray<MbSNameMaker> cNames( 0, 1, false );
        cNames.Add( &cName );

        switch ( type ) {   
          case  1 : // Объединение. Union.
          case -1 : // Вычитание. Difference.
          case -2 : // Пересечение. Intersection.
          {
            MbItem * obj( nullptr );
            MbSolid * solid = nullptr;

            if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj, TestVariables::itemPathFrom ) ) {
              TestVariables::viewManager->ToggleObject(obj);
              solid = (MbSolid *)obj;
            }

            if ( solid!=nullptr ) {
              SetWaitCursor( true );
              res = ::RevolutionResult( *solid, cm_Copy, *curvesData, axis_, param, oType, operNames, cNames, resultSolid );
              SetWaitCursor( false );
            }

            if (resultSolid!=nullptr) {
              TestVariables::viewManager->ToggleObject(obj);
              TestVariables::viewManager->DeleteObject( obj );
            }
          } break;

          default : {
            SetWaitCursor( true );
            res = ::RevolutionSolid( *curvesData, axis_, param, operNames, cNames, resultSolid );
            SetWaitCursor( false );
          } break;

        }
      }
    }
  }

  if ( resultSolid==nullptr )
    ::PutErrorMessage( res, IDS_TITLE_ERR );

  if (resultSolid!=nullptr) TestVariables::viewManager->AddObject( ns, resultSolid );
}


//------------------------------------------------------------------------------
// Создание тела вращения по трехмерной кривой.
// Creation of a revolution solid by a spatial curve.
// ---
void CreateCurve3DRevolutionSolid( int type, bool closed )
{
   CreateCurve3DRevolutionSolid( type, closed, false );
}


//------------------------------------------------------------------------------
// Создание тела выдавливания по проволочному каркасу.
// Creation of a extrusion solid by a wireFrame.
// ---
void CreateWireFrameExtrusionSolid( bool closed )
{
  CreateCurve3DExtrusionSolid( 0, closed, true );
}


//------------------------------------------------------------------------------
// Создание тела вращения по проволочному каркасу.
// Creation of a revolution solid by a wireFrame.
// ---
void CreateWireFrameRevolutionSolid( bool closed )
{
  CreateCurve3DRevolutionSolid( 0, closed, true );
}


//------------------------------------------------------------------------------
// Создание тела вращения.
// Creation of a revolution solid.
// ---
void CreateSolidRevolutionSolid()
{
  MbItem * obj(0); 

  RevolutionValues params;
  params.SetMergingFaces( TestVariables::mergeFaces );
  params.SetMergingEdges( TestVariables::mergeEdges );

  MbResultType res = rt_Error;
  MbVector3D direction;
  if ( GetGObj ( IDS_SELECT_SOLID, st_Solid, &obj, TestVariables::itemPathFrom ) ) 
  {
    MbSolid * resultSolid = nullptr;
    MbSolid * solid = (MbSolid *)obj;
    Style ns( obj->GetWidth(), obj->GetColor() );
    if ( obj ) TestVariables::viewManager->ToggleObject(obj);

    MbVector3D vect;
    MbCartPoint3D orig, p;

    if ( GetPoint( IDS_SELECT_POINT, orig ) && GetPoint( IDS_SELECT_POINT, p ) ) 
      vect.Init( orig, p ); 
    else {
      MbItem * gCurve = nullptr;
      if ( GetAnyCurve( IDS_SELECT_GUIDE_CURVE, &gCurve ) ) {
        c3d::SpaceCurveSPtr gCrv( ::GetCurve3D( gCurve, false ) );
        if ( gCrv != nullptr ) {
          MbAxis3D rotAxis;
          if ( gCrv->GetCircleAxis( rotAxis ) ) {
            orig = rotAxis.GetOrigin();
            vect = rotAxis.GetAxisZ();
          }
        }
      }
    }

    if ( !vect.IsZero() && (solid != nullptr ) )
    {
      if ( GetDouble( IDS_ENTER_ANGLE, TestVariables::revolve1 ) && 
           GetDouble( IDS_ENTER_ANGLE, TestVariables::revolve2 ) ) 
      {
        params.side1.scalarValue = TestVariables::revolve1*M_PI/180;
        params.side2.scalarValue = TestVariables::revolve2*M_PI/180;
        params.thickness1 = .0;
        params.thickness2 = .0;
        params.side1.rake        = 0;
        params.side2.rake        = 0;

        MbSweptData curvesData( *solid );
        MbAxis3D axis_(orig, vect);

        MbSNameMaker operNames( ct_CurveRevolutionSolid, MbSNameMaker::i_SideNone, 0 );
        operNames.SetVersion( MbNameVersion() );

        MbSNameMaker cName( ct_CurveRevolutionSolid, MbSNameMaker::i_SideNone, 0 );
        PArray<MbSNameMaker> cNames( 0, 1, false );
        cNames.Add( &cName );

        SetWaitCursor( true );
        res = ::RevolutionSolid( curvesData, axis_, params, operNames, cNames, resultSolid );
        SetWaitCursor( false );
      }
    }

    if ( resultSolid == nullptr )
      ::PutErrorMessage( res, IDS_TITLE_ERR );
    else
    {
      TestVariables::viewManager->DeleteObject( obj );
      TestVariables::viewManager->AddObject( ns, resultSolid );
    }
  }
}


//------------------------------------------------------------------------------
// Создание кинематического тела.
// Creation of an evolution solid.
// ---
void CreateCurveEvolutionSolid( int type, bool closed, bool bWireFrame )
{
  EvolutionValues param;
  param.shellClosed = closed;
  param.SetMergingFaces( TestVariables::mergeFaces );
  param.SetMergingEdges( TestVariables::mergeEdges );

  SolidSPtr resultSolid;
  Style ns = TestVariables::SWEPT_Style;

  DPtr<MbSweptData> sweptData( SelectSweptData( bWireFrame ) );

  // Выбор направляющей
  MbItem * spineItem = nullptr;
  WireFrameSPtr spineFrame;
  ConstSpaceCurveSPtr spn;
  if ( sweptData != nullptr ) {
    if ( GetGObj( IDS_SELECT_WIREFRAME, st_SpaceItem, &spineItem) && (spineItem != nullptr) ) {
      if ( spineItem->IsA() == st_WireFrame ) {
        spineFrame = static_cast<MbWireFrame *>( spineItem );
        TestVariables::viewManager->ToggleObject( spineItem );
      }
    }
    if ( spineFrame == nullptr && GetGObj( IDS_SELECT_GUIDE_CURVE, st_Curve3D, &spineItem ) ) {
      TestVariables::viewManager->ToggleObject( spineItem );
      spn = ::GetCurve3D( spineItem, false );
    }
  }

  {
    if ( GetDouble( IDS_ENTER_THICKNESS, TestVariables::thickness1 ) && GetDouble( IDS_ENTER_THICKNESS, TestVariables::thickness2 ) ) {
      param.thickness1 = TestVariables::thickness1;
      param.thickness2 = TestVariables::thickness2;

      int pr = 1;
      if ( GetInt( IDS_ENTER_METHOD, pr ) ) {
        if ( pr < 1 ) pr = 0;
        if ( pr > 1 ) pr = 2;

        if ( pr == 0 )
          param.SetParallel();
        else if ( pr == 1 )
          param.SetKeepingAngle();
        else if ( pr == 2 )
          param.SetOrthogonal();
        pr = 0;

        if ( spn != nullptr ) {
          {
            MbItem * surfaceSolid = nullptr;
            MbTopItem * surfaceObj = nullptr;
            if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &surfaceSolid, &surfaceObj, TestVariables::itemPathFrom.second ) ) {
              if ( surfaceObj->IsAFace() ) {
                const MbFace * face = static_cast<const MbFace *>(surfaceObj);
                param.SetSurface( face->GetSurface().GetSurface() );
                param.SetBySurfaceNormal( true );
              }
            }
          }
          if ( param.BySurfaceNormal() ) {
            ConstSurfaceSPtr spnSurface;
            const MbSurface * selSurface = param.GetSurface();
            if ( param.GetSurface() != nullptr ) // KOMPAS-37267
              spnSurface = selSurface;

            ConstPlaneCurveSPtr pCurve;
            if ( spn->GetSurfaceCurve( pCurve, spnSurface ) ) {
              if ( (selSurface == nullptr) || (selSurface->IsSimilarToSurface( *spnSurface, Math::DefaultMathVersion() )) ) {
                MbCurve3D * spnPtr = nullptr;
                ::SurfaceCurve( *spnSurface, *pCurve, spnPtr );
                if ( spnPtr != nullptr ) {
                  spn = spnPtr;
                  param.SetSurface( *spnSurface );
                }
              }
            }
            else {
              spnSurface = selSurface;
            }
          }
        }

        if ( spn != nullptr || spineFrame != nullptr ) {
          MbSNameMaker operNames( ct_CurveEvolutionSolid, MbSNameMaker::i_SideNone, 0 );
          operNames.SetVersion( MbNameVersion() );

          MbSNameMaker cNames( ct_CurveSweptSolid, MbSNameMaker::i_SideNone, 0 );
          MbSNameMaker sNames( ct_CurveSweptSolid, MbSNameMaker::i_SideNone, 0 );

          PArray<MbSNameMaker> contoursNames( 1, 0, false );
          contoursNames.Add( &cNames );

          switch ( type ) {

            case  1 : // Объединение. Union.
            case -1 : // Вычитание. Difference.
            case -2 : // Пересечение. Intersection.
            {
              MbItem  * obj = nullptr;
              SolidSPtr solid;

              if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj, TestVariables::itemPathFrom ) ) {
                TestVariables::viewManager->ToggleObject(obj);
                ns.Init( obj->GetWidth(), obj->GetColor() );
                solid = (MbSolid *)obj;

                MbResultType res = rt_Error;
                SetWaitCursor( true );

                DPtr<MbEvolutionShellParams> evolParams = ( spn != nullptr ) 
                  ? new MbEvolutionShellParams( *sweptData, *spn, param, operNames, contoursNames, sNames )
                  : new MbEvolutionShellParams( *sweptData, *spineFrame, param, operNames, contoursNames, sNames );

                if ( type == 1 )
                  res = ::EvolutionResult( solid, cm_Copy, bo_Union, *evolParams, resultSolid );
                else {
                  if ( type == -1 )
                    res = ::EvolutionResult( solid, cm_Copy, bo_Difference, *evolParams, resultSolid );
                  else
                    res = ::EvolutionResult( solid, cm_Copy, bo_Intersect, *evolParams, resultSolid );
                }
                SetWaitCursor( false );

                if ( resultSolid==nullptr )
                  ::PutErrorMessage( res, IDS_TITLE_ERR );
                if (resultSolid!=nullptr) {
                  TestVariables::viewManager->ToggleObject(obj);
                  TestVariables::viewManager->DeleteObject( obj );
                }
              }
            } break;

            default : {
              int iSetDir = 0;
              GetInt( IDS_SPECIFY_DIRECTION, iSetDir );

              MbResultType res = rt_Error;
              MbVector3D userDir;

              if ( iSetDir > 1 ) {
                MbCartPoint3D userPnt;
                if ( GetPoint( IDS_ENTER_DIR, userPnt ) ) // SD_7128788 C3D-612
                  userDir = userPnt;
              }

              SetWaitCursor( true );

              if ( iSetDir == 1 ) {
                MbItem * directionItem( 0 ); // Направляющая. directionCurve.
                if ( spn != nullptr && GetGObj( IDS_SELECT_GUIDE_CURVE, st_Curve3D, &directionItem ) ) {
                  TestVariables::viewManager->ToggleObject(directionItem);
                  ConstSpaceCurveSPtr direction( ::GetCurve3D( directionItem, false ) );
                  if ( direction != nullptr ) {
                    SpineSPtr spine( MbSpine::Create( *spn, true, *direction, false, param.IsParallel() ) );
                    MbEvolutionShellParams evolParams( *sweptData, *spine, param, operNames, contoursNames, sNames );
                    res = ::EvolutionShell( evolParams, resultSolid );
                  }
                }
                else {
                  MbCartPoint3D userPnt;
                  if ( GetPoint( IDS_ENTER_DIR, userPnt ) ) // SD_7128788 C3D-612
                    userDir = userPnt;
                  if ( userDir.MaxFactor() > EPSILON ) {
                    SpineSPtr spine( ( spn != nullptr ) ? MbSpine::Create( *spn, true, userDir, param.IsParallel() )
                                                        : MbSpine::Create( *spineFrame, true, &userDir, param.IsParallel(), Math::DefaultMathVersion() ) );
                    MbEvolutionShellParams evolParams( *sweptData, *spine, param, operNames, contoursNames, sNames );
                    res = ::EvolutionShell( evolParams, resultSolid );
                  }
                  else if ( spn != nullptr) {
                    MbEvolutionShellParams evolParams( *sweptData, *spn, param, operNames, contoursNames, sNames );
                    res = ::EvolutionShell( evolParams, resultSolid );
                  }
                  else {
                    MbEvolutionShellParams evolParams( *sweptData, *spineFrame, param, operNames, contoursNames, sNames );
                    res = ::EvolutionShell( evolParams, resultSolid );
                  }
                }
              }
              else {
                if ( iSetDir == 2 ) { // SD_7124934 // SD_132147
                  double scale1 = 1.0, scale2 = 2.0, angle1 = 0.0, angle2 = M_PI;
                  GetDouble( IDS_ENTER_SCALE, scale1 );
                  GetDouble( IDS_ENTER_SCALE, scale2 );
                  GetDouble( IDS_ENTER_ANGLE, angle1 );
                  GetDouble( IDS_ENTER_ANGLE, angle2 );
                  double st1 = 0.0, st2 = 1.0;
                  double wt1 = 0.0, wt2 = 1.0;
                  if ( ::fabs(scale1 - 1.0) > EPSILON ||
                        ::fabs(scale2 - 1.0) > EPSILON ||
                        ::fabs(angle1) > EPSILON ||
                        ::fabs(angle2) > EPSILON ) {
                    MbFunction * scaling = new MbLineFunction( scale1, scale2, st1, st2 ); 
                    string_t arg = c3d::ToC3Dstring("t");
                    string_t f = c3d::ToC3Dstring("3.1415926926*sin(1.570796326795*t)");
                    MbFunctionFactory factory;
                    MbFunction * winding = nullptr;//factory.CreateAnalyticalFunction( f, arg, wt1, wt2 );
                    if ( winding == nullptr )
                      winding = new MbLineFunction( angle1, angle2, wt1, wt2 ); 
                    param.AddData( *scaling, *winding );
                  }
                }
                if ( iSetDir == 3 ) { // SD_7128788 C3D-612
                  double rangeX = 0.0;
                  GetDouble( IDS_ENTER_OFFSET, rangeX );
                  if ( ::fabs(rangeX) > EPSILON ) {
                    param.SetRange( rangeX );
                  }
                }

                int vsType = 0;
                GetInt( IDS_SPECIFY_VS_PROTOTYPE, vsType );

                MbSpine * spine( nullptr );
                std::unique_ptr<MbEvolutionShellParams> evShellParams;
                std::unique_ptr<VariableSectionValues> varSectVals;
                std::unique_ptr<IVariableSectionData> sectionData;
                const ConstFunctionSPtrVector * funcs = nullptr;

                if ( userDir.MaxFactor() > EPSILON ) {
                  if ( spn != nullptr )
                    spine = MbSpine::Create( *spn, true, userDir, param.IsParallel() );
                  else
                    spine = MbSpine::Create( *spineFrame, true, &userDir, param.IsParallel(), Math::DefaultMathVersion() );
                }
                if ( spine != nullptr ) {
                  evShellParams.reset( new MbEvolutionShellParams(*sweptData, *spine, param, operNames,
                                                                  contoursNames, sNames) );
                }
                else if ( spn != nullptr ) {
                  if ( spn->IsA() == st_ContourOnPlane ) {
                    MbContourOnPlane & plCont = static_cast<MbContourOnPlane&>( spn->Duplicate() );
                    MbPlaneCurve * plCurve = new MbPlaneCurve(plCont.GetPlacement(), plCont.GetContour(), true);
                    evShellParams.reset( new MbEvolutionShellParams(*sweptData, *plCurve, param, operNames,
                                                                    contoursNames, sNames) );
                  }
                  else {
                    evShellParams.reset( new MbEvolutionShellParams(*sweptData, *spn, param, operNames,
                                                                    contoursNames, sNames) );
                  }
                  spine = &MbSpine::Create( *spn, param.IsParallel(), true );
                }
                else {
                  evShellParams.reset( new MbEvolutionShellParams(*sweptData, *spineFrame, param, operNames,
                                                                   contoursNames, sNames) );
                  spine = MbSpine::Create( *spineFrame, false, nullptr, param.IsParallel(), true );
                }

                if ( (vsType != 0) && spine ) {
                  if ( vsType == 1) {
                    sectionData.reset( new VariableSectionSinScale(spine->GetTMin(), spine->GetTMax(),
                                                                   sweptData->GetContours()) );
                    funcs = static_cast<VariableSectionSinScale *>( sectionData.get() )->GetFuncs();
                  }
                  if ( sectionData != nullptr ) {
                    varSectVals.reset( new VariableSectionValues(*sectionData) );
                    if ( funcs != nullptr ) {
                      for ( const auto & f : *funcs )
                        varSectVals->AddConstraintFunction( f.get() );
                    }
                    evShellParams.reset( new MbEvolutionShellParams(*sweptData, *spine, param, operNames,
                                                                    contoursNames, sNames, *varSectVals.get()) );
                  }
                }
                if ( evShellParams != nullptr )
                  res = ::EvolutionShell( *evShellParams, resultSolid );

                ::DeleteItem( spine );
              }

              SetWaitCursor( false );

              if ( resultSolid == nullptr )
                ::PutErrorMessage( res, IDS_TITLE_ERR );
            } break;

          }
        }
      }
    }
  }
  TestVariables::viewManager->ToggleObject( spineItem );

  if ( resultSolid != nullptr )
    TestVariables::viewManager->AddObject( ns, resultSolid );
} // CreateCurveEvolutionSolid


//------------------------------------------------------------------------------
// Создание кинематического тела.
// Creation of an evolution solid.
// ---
void CreateCurveEvolutionSolid( int type, bool closed )
{
  CreateCurveEvolutionSolid( type, closed, false );
}


//------------------------------------------------------------------------------
// Создание тела движения проволочного каркаса.
// Creation of a evolution solid by a wireFrame generatrix.
// ---
void CreateWireFrameEvolutionSolid( bool closed )
{
  CreateCurveEvolutionSolid( 0, closed, true );
}


//------------------------------------------------------------------------------
// Создание кинематического тела.
// Creation of an evolution solid.
// ---
void CreateSolidEvolutionSolid()
{
  MbItem * solidObj = nullptr;
  MbItem * spineObj = nullptr;

  PathMatrix solidPathFrom;
  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &solidObj, solidPathFrom ) ) 
  {
    TestVariables::viewManager->ToggleObject( solidPathFrom.first );

    MbSolid * solid = static_cast<MbSolid *>( solidObj );
   
    PathMatrix spinePathFrom;
    if ( GetGObj( IDS_SELECT_GUIDE_CURVE, st_Curve3D, &spineObj, spinePathFrom ) ) 
    {
      TestVariables::viewManager->ToggleObject( spinePathFrom.first );

      EvolutionValues params;
      params.thickness1 = 0;
      params.thickness2 = 0;
      params.SetMergingFaces( TestVariables::mergeFaces );
      params.SetMergingEdges( TestVariables::mergeEdges );

      ConstSpaceCurveSPtr spn( ::GetCurve3D( spineObj, false ) );

      if ( spn != nullptr )
      {
        int pr = 1;
        if ( GetInt(IDS_ENTER_METHOD, pr) )
        {
          if ( pr < 1 ) pr = 0;
          if ( pr > 1 ) pr = 2;

          if ( pr == 0 )
            params.SetParallel();
          else if ( pr == 1 )
            params.SetKeepingAngle();
          else
            params.SetOrthogonal();
          pr = 0;
        }

        MbSweptData sweptData( *solid );
        {
          MbItem * otherSolid = nullptr;
          MbTopItem * surfaceObj = nullptr;

          if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &otherSolid, &surfaceObj, TestVariables::itemPathFrom.second ) ) {
            if ( surfaceObj->IsAFace() ) {
              const MbFace * face = static_cast<const MbFace *>(surfaceObj);
              params.SetSurface( face->GetSurface().GetSurface() );
              params.SetBySurfaceNormal( true );
            }
          }
        }

        MbSNameMaker operNames( ct_CurveEvolutionSolid, MbSNameMaker::i_SideNone, 0 );
        operNames.SetVersion( MbNameVersion() );

        MbSNameMaker cNames( ct_CurveSweptSolid, MbSNameMaker::i_SideNone, 0 );
        MbSNameMaker sNames( ct_CurveSweptSolid, MbSNameMaker::i_SideNone, 0 );

        PArray<MbSNameMaker> contoursNames( 1, 0, false );
        contoursNames.Add( &cNames );

        TestVariables::viewManager->ToggleObject( solidPathFrom.first );

        SetWaitCursor( true );

        SolidSPtr resultSolid;
        const MbEvolutionShellParams evShellParams{ sweptData, *spn, params, operNames, contoursNames, sNames };
        MbResultType res = ::EvolutionShell( evShellParams, resultSolid );

        SetWaitCursor( false );

        if ( resultSolid == nullptr )
          ::PutErrorMessage( res, IDS_TITLE_ERR );
        else
        {
          Style ns( solidObj->GetWidth(), solidObj->GetColor() );
          TestVariables::viewManager->DeleteObject( solidPathFrom.first );
          TestVariables::viewManager->AddObject( ns, resultSolid.detach() );
        }
      }
      TestVariables::viewManager->ToggleObject( spinePathFrom.first );
    }
  }
}


//------------------------------------------------------------------------------
// Установить систему координат для сечения в виде точки Loft поверхности.
// Set placement for the section as a point of the Loft surface.
//---
template<class PlacesContainer> // Контейнер из MbPlacement3D. Container of MbPlacement3D
static
void SetPlacementLoftContour( bool isInitialSection, PlacesContainer & pls )
{
  int           indSelf  = isInitialSection ? 0 : static_cast<int>( pls.size() ) - 1;
  int           indOther = isInitialSection ? 1 : static_cast<int>( pls.size() ) - 2;
  const TCHAR * message  = isInitialSection ? IDS_ENTER_START_PLANE : IDS_ENTER_END_PLANE;

  MbItem * objSurf = nullptr;
  if ( GetGObj(message, st_Surface, &objSurf) ) { // Если указана плоскость. If a plane is specified.
    MbSurface & surf = static_cast<MbSurface *>( (static_cast<MbSpaceInstance *>(objSurf))->SetSpaceItem() )->SetSurface();
    if ( surf.IsA() == st_Plane ) {
      TestVariables::viewManager->ToggleObject( objSurf );
      MbPlacement3D placePlane;
      surf.GetPlacement( &placePlane );
      MbCartPoint3D origSelf = pls[indSelf].GetOrigin();
      MbCartPoint3D origOther = placePlane.GetOrigin();
      MbVector3D vecMove( origOther, origSelf );
      pls[indSelf] = placePlane.Move( vecMove );
    }
  }
  else { // Взять систему координат с ближнего сечения. Take placement from near section.
    MbCartPoint3D origSelf = pls[indSelf].GetOrigin();
    MbCartPoint3D origOther = pls[indOther].GetOrigin();
    MbVector3D vecMove( origOther, origSelf );
    MbPlacement3D placePoint = pls[indOther];
    pls[indSelf] = placePoint.Move( vecMove );
  }
}


//------------------------------------------------------------------------------
// Ввод контуров для построения тела по сечениям.
// Input contours to create a lofted solid.
//---
static
void InputLoftContour( const bool                         isPointContour,
                             std::vector<MbPlacement3D> & pls,
                             PlaneContoursSPtrVector &    crvs )
{
  if ( isPointContour ) { // Если контур ввиде точки. If contour is in the form of a point.
    MbItem * obj( nullptr );
    MbTopItem * titem = nullptr;
    MbMatrix3D from;
    if ( GetTObj(IDS_SELECT_VERTEX, st_Solid, tt_TopItem, &obj, &titem, from) ) {
      if ( titem->IsAVertex() ) {
        TestVariables::viewManager->ToggleObject( obj );
        MbCartPoint3D pnt = static_cast<const MbVertex *>( titem )->GetCartPoint();
        if ( !from.IsSingle() )
          pnt.Transform( from );
        MbPlacement3D place( pnt );
        MbCurve * pointCrv = new MbPointCurve( MbCartPoint(0.0, 0.0), false );
        PlaneContourSPtr pointContour( new MbContour() );
        pointContour->AddSegment( pointCrv );
        crvs.push_back( pointContour );
        pls.push_back( place );
      }
    }
  }
  else { // Если в виде кривой.  If in the form of a curve.
    MbItem * curve = nullptr; // Образующая. Generatrix.
    MbItem * curve0 = nullptr;
    while ( GetAnyCurve(IDS_SELECT_GENER_CURVE, &curve) ) {
      MbPlacement3D crvPlace;
      PlaneContourSPtr crv( GetContourOnPlace(curve, crvPlace) );
      if ( crv != nullptr ) {
        crvs.push_back( crv );
        pls.push_back( crvPlace );
        if ( curve0 != nullptr ) {
          TestVariables::viewManager->ToggleObject( curve0 );
          if ( curve == curve0 )
            break;
        }
        TestVariables::viewManager->ToggleObject( curve );
        curve0 = curve;
      }
    }
    if ( curve0 )
      TestVariables::viewManager->ToggleObject( curve0 );
  }
}


//------------------------------------------------------------------------------
// Ввод основных данных для построения тела по сечениям.
// Input base data to create a lofted solid.
//---
static
bool InputCurveLoftedParameters( std::vector<MbPlacement3D> & pls,
                                 PlaneContoursSPtrVector &    crvs,
                                 LoftedValues &               param )
{
  bool res = false;

  ::InputLoftContour( true, pls, crvs ); // Ввести точку, как первый контур. Input point as the first contour.

  ::InputLoftContour( false, pls, crvs ); // Ввод контуров из линий. Input curve contours.

  ::InputLoftContour( true, pls, crvs ); // Ввести точку, как последний контур. Input point as the last contour.

  // Параметры тонкостенности. Parameters of thickness.
  if ( crvs.size() > 1 ) {
    if ( GetDouble(IDS_ENTER_THICKNESS, TestVariables::thickness1) && GetDouble(IDS_ENTER_THICKNESS, TestVariables::thickness2) ) {
      param.thickness1 = TestVariables::thickness1;
      param.thickness2 = TestVariables::thickness2;

      int cl = 0;
      if ( GetInt(IDS_ENTER_CLOSED, cl) ) {
        param.closed = cl > 0;

        int startDir = 0, endDir = 0;
        int setStartNormal = 0;
        int setEndNormal   = 0;
        if ( GetInt(IDS_ENTER_START_DIRECTION, startDir) && startDir > 0 ) // Задать примыкание по нормали в начале. Set the abutment along the normal at the beginning.
          param.vector1 = pls[0].GetAxisZ();
        else if ( crvs[0]->GetSegment(0)->IsA() == pt_PointCurve && // Если первое сечение в виде точки. if first section is in the form of a point.
                  GetInt(IDS_SET_START_NORMAL, setStartNormal) ) {
          if ( setStartNormal > 0 ) { // Установить нормаль в начале тела (купол). Set the normal at the beginning of the body (dome).
            param.setNormal1 = true;
            ::SetPlacementLoftContour( true, pls ); // Установить систему координат для контура в виде точки. Set placement for contour as a point.
            double derFactor1 = 1.0;
            if ( GetDouble(IDS_DERIVE_FACTOR_AT_START, derFactor1) ) {
              param.derFactor1 = derFactor1;
            }
          }
        }
        if ( GetInt(IDS_ENTER_END_DIRECTION, endDir) && (endDir > 0) ) // Задать примыкание по нормали в конце. Set the abutment along the normal at the end.
          param.vector2 = pls.back().GetAxisZ();
        else if ( crvs.back()->GetSegment(0)->IsA() == pt_PointCurve && // Если последнее сечение в виде точки. if last section is in the form of a point.
                  GetInt(IDS_SET_END_NORMAL, setEndNormal) ) {
          if ( setEndNormal > 0 ) { // Установить нормаль в конце тела (купол). Set the normal at the end of the body (dome).
            param.setNormal2 = true;
            ::SetPlacementLoftContour( false, pls ); // Установить систему координат для контура в виде точки. Set placement for contour as a point.
            double derFactor2 = 1.0;
            if ( GetDouble(IDS_DERIVE_FACTOR_AT_END, derFactor2) ) {
              param.derFactor2 = derFactor2;
            }
          }
        }
        res = true;
      }
    }
  }

  return res;
} // InputCurveLoftedParameters


//------------------------------------------------------------------------------
// Создание тела по плоским сечениям.
// Creation a lofted solid by planar sections.
// ---
void CreateCurveLoftedSolid( int type, bool closed )
{
  LoftedValues param;
  param.shellClosed = closed;
  param.SetMergingFaces( TestVariables::mergeFaces );
  param.SetMergingEdges( TestVariables::mergeEdges );

  SolidSPtr resultSolid( nullptr );
  std::vector<MbPlacement3D> pls;
  PlaneContoursSPtrVector crvs;
  crvs.reserve( 4 );         //-V112

  Style ns = TestVariables::SWEPT_Style;

  if ( ::InputCurveLoftedParameters(pls, crvs, param) ) {
    MbSNameMaker operNames( ct_CurveLoftedSolid, MbSNameMaker::i_SideNone, 0 );
    operNames.SetVersion( MbNameVersion() );

    SNamesMakerSPtrVector nms;
    nms.reserve( crvs.size() );
    for ( ptrdiff_t i = 0; i < (ptrdiff_t)crvs.size(); i++ ) {
      nms.push_back( SNameMakerSPtr(new MbSNameMaker()) );
      for ( ptrdiff_t k = crvs[i]->GetSegmentsCount() - 1; k >= 0; k-- ) {
        nms.back()->Add( SimpleName() );
      }
    }

    const OperationType oType = 
      ( type == 1 ) ? bo_Union : 
      ( type == -1 ) ? bo_Difference : 
      ( type == -2 ) ? bo_Intersect : bo_Unknown;

    switch ( oType ) {
      case bo_Union: // Объединение. Union.
      case bo_Difference: // Вычитание. Difference.
      case bo_Intersect: // Пересечение. Intersection.
      {
        MbItem * obj( nullptr );

        if ( GetGObj(IDS_SELECT_SOLID, st_Solid, &obj, TestVariables::itemPathFrom) ) {
          TestVariables::viewManager->ToggleObject( obj );
          ns.Init( obj->GetWidth(), obj->GetColor() );

          SolidSPtr solid( static_cast<MbSolid *>(obj) );
          MbLoftedSolidParams loftSolParam( pls, crvs, nullptr, param,
                                            oType, nullptr, nullptr, operNames, nms );

          SetWaitCursor( true );
          MbResultType res = ::LoftedResult( solid, cm_Copy, loftSolParam, resultSolid );
          SetWaitCursor( false );

          if ( resultSolid == nullptr )
            ::PutErrorMessage( res, IDS_TITLE_ERR );
          if ( resultSolid != nullptr ) {
            TestVariables::viewManager->ToggleObject( obj );
            TestVariables::viewManager->DeleteObject( obj );
          }
        }
      } break;
      default:
      {
        MbLoftedSolidParams loftSolParam( pls, crvs, nullptr, param,
                                          nullptr, nullptr, operNames, nms );

        SetWaitCursor( true );
        MbResultType res = ::LoftedSolid( loftSolParam, resultSolid );
        SetWaitCursor( false );

        if ( resultSolid == nullptr )
          ::PutErrorMessage( res, IDS_TITLE_ERR );
      }
      break;
    }
  }

  if ( resultSolid != nullptr )
    TestVariables::viewManager->AddObject( ns, resultSolid );
} // CreateCurveLoftedSolid


//------------------------------------------------------------------------------
// Создание тела по плоским сечениям вдоль осевой линии
// Creation a lofted solid by planar sections along a centre line.
//---
void CreateCurveLoftedSolidCentralLine( int type, bool closed )
{
  LoftedValues param;
  param.shellClosed = closed;
  param.SetMergingFaces( TestVariables::mergeFaces );
  param.SetMergingEdges( TestVariables::mergeEdges );

  SolidSPtr resultSolid( nullptr );
  std::vector<MbPlacement3D> pls;
  PlaneContoursSPtrVector crvs;
  crvs.reserve( 4 );         //-V112

  SpaceCurvesSPtrVector guideCrvs;
  guideCrvs.reserve( 4 );

  Style ns = TestVariables::SWEPT_Style;

  if ( ::InputCurveLoftedParameters(pls, crvs, param) ) {
    // Ввести осевую линию. Input centre line.
    MbItem * curveObj( nullptr );
    ::GetAnyCurve( IDS_SELECT_CENTER_LINE, &curveObj );
    SpaceCurveSPtr curve( (curveObj != nullptr) ? ::GetCurve3D(curveObj, false) : nullptr );
    TestVariables::viewManager->ToggleObject( curveObj );

    // Ввести направляющие кривые. Input guide curves.
    MbItem * gCurve = nullptr;
    MbItem * gCrv0 = nullptr;
    while ( GetAnyCurve(IDS_SELECT_GUIDE_CURVE, &gCurve) ) {
      SpaceCurveSPtr gCrv = ::GetCurve3D( gCurve, false );
      if ( gCrv != nullptr ) {
        guideCrvs.push_back( gCrv );
        if ( gCrv0 != nullptr ) {
          TestVariables::viewManager->ToggleObject( gCrv0 );
          if ( gCurve == gCrv0 )
            break;
        }
        else {
          TestVariables::viewManager->ToggleObject( curveObj );
        }
        TestVariables::viewManager->ToggleObject( gCurve );
        gCrv0 = gCurve;
      }
    }
    if ( gCrv0 )
      TestVariables::viewManager->ToggleObject( gCrv0 );

    SpacePointsVector pnts; //Не используется. // Useless.

    MbSNameMaker operNames( ct_CurveLoftedSolid, MbSNameMaker::i_SideNone, 0 );
    operNames.SetVersion( MbNameVersion() );

    SNamesMakerSPtrVector nms;
    nms.reserve( crvs.size() );
    for ( ptrdiff_t i = 0; i < (ptrdiff_t)crvs.size(); i++ ) {
      nms.push_back( SNameMakerSPtr(new MbSNameMaker) );
      for ( ptrdiff_t k = crvs[i]->GetSegmentsCount() - 1; k >= 0; k-- ) {
        nms.back()->Add( SimpleName() );
      }
    }

    const OperationType oType = ( type == 1 ) ? bo_Union : ( type == -1 ) ? bo_Difference : ( type == -2 ) ? bo_Intersect : bo_Unknown;
    switch ( oType ) {
      case bo_Union: // Объединение. Union.
      case bo_Difference: // Вычитание. Difference.
      case bo_Intersect: // Пересечение. Intersection.
      {
        MbItem * solidObj( nullptr );

        if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &solidObj, TestVariables::itemPathFrom ) ) {
          TestVariables::viewManager->ToggleObject( solidObj );
          ns.Init( solidObj->GetWidth(), solidObj->GetColor() );

          SolidSPtr solid( static_cast<MbSolid *>(solidObj) );
          MbLoftedSolidParams loftSolParam( pls, crvs, curve, param, oType,
                                            (guideCrvs.size() > 0) ? &guideCrvs : nullptr,
                                            pnts.size() > 0 ? &pnts : nullptr,
                                            operNames, nms );

          SetWaitCursor( true );
          MbResultType res = ::LoftedResult( solid, cm_Copy, loftSolParam, resultSolid );
          SetWaitCursor( false );

          if ( resultSolid == nullptr )
            ::PutErrorMessage( res, IDS_LOFT_ERROR );
          if ( resultSolid != nullptr ) {
            TestVariables::viewManager->ToggleObject( solidObj );
            TestVariables::viewManager->DeleteObject( solidObj );
          }
        }
      } break;
      default:
      {
        MbLoftedSolidParams loftSolParam( pls, crvs, curve, param,
                                          (guideCrvs.size() > 0) ? &guideCrvs : nullptr,
                                          (pnts.size() > 0) ? &pnts : nullptr,
                                          operNames, nms );

        SetWaitCursor( true );
        MbResultType res = ::LoftedSolid( loftSolParam, resultSolid );
        SetWaitCursor( false );

        if ( resultSolid == nullptr )
          ::PutErrorMessage( res, IDS_LOFT_ERROR );
      } break;
    }

    TestVariables::viewManager->ToggleObject( curveObj );
  }

  if ( resultSolid != nullptr )
    TestVariables::viewManager->AddObject( ns, resultSolid );

} // CreateCurveLoftedSolidCentralLine


//------------------------------------------------------------------------------
// Создание тонкого листового тела по поверхности и толщине.
// Create a shell of the solid from a surface and a thickness.
// ---
bool CreateSurfaceSolid( bool thin )
{
  MbItem * surface( nullptr ); // Образующая. Generatrix.
  MbSolid * solid( nullptr );
  MbAccurateTimer ac;

  if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &surface ) ) {
    TestVariables::viewManager->ToggleObject(surface);

    double length = 0;
    if ( thin || GetDouble( IDS_ENTER_DISTANCE, length )) {
      LoftedValues p;
      p.thickness1 = length;
      p.thickness2 = length;
      MbSNameMaker names( ct_ThinShellCreator, MbSNameMaker::i_SideNone, 0/*buttAdd*/ );
      SimpleName name(0); 
      
      SetWaitCursor( true );
      ac.Begin();

      MbResultType res = ::ThinSolid( (MbSurface &)*((MbSpaceInstance *)surface)->GetSpaceItem(), true, p, names, name, solid ); 

      ac.End();
      SetWaitCursor( false );

      if ( solid == nullptr )
        ::PutErrorMessage( res, IDS_TITLE_ERR );
    }

    TestVariables::viewManager->ToggleObject(surface);
  }

  if ( solid != nullptr ) {
    TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, solid );
    if ( TestVariables::checkOperationTime ) {
      const size_t bufLen = 1024;
      TCHAR buf[bufLen];
      size_t len = 0;
      len += (size_t)_sntprintf( buf + len, bufLen - len, RT_THICKENING_SOLID );
      len += (size_t)_sntprintf( buf + len, bufLen - len, _T( "\n\n" ) );
      len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
      ::MessageBoxEx( buf, IDS_RESULTS );
    }

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Создание разрезанного тела выдавленным плоским контуром.
// Cut a part of a solid off with an extruded planar contour.
// ---
void CutSolidByContour( bool closed )
{
  MbItem * solidObj = nullptr;
  MbItem * curveObj = nullptr;

  MbCartPoint3D p0;

  PathMatrix solidPathFrom;
  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &solidObj, solidPathFrom ) ) {
    MbResultType res = rt_Error;

    SolidsSPtrVector resSolids;
    resSolids.reserve( 2 );

    TestVariables::viewManager->ToggleObject( solidPathFrom.first );

    PathMatrix curvePathFrom;
    if ( GetAnyCurve( IDS_SELECT_CNTR_ON_PLANE, &curveObj, curvePathFrom ) ) {
      TestVariables::viewManager->ToggleObject( curvePathFrom.first );

      MbPlacement3D pl;
      c3d::PlaneContourSPtr cntr( GetContourOnPlace( curveObj, pl ) );
      pl.Transform( curvePathFrom.second );

      if ( cntr != nullptr && GetInt( IDS_ENTER_NUMBER_N, TestVariables::part ) ) {
        MbVector3D der;

        SetWaitCursor( true );

        MbSNameMaker n( (SimpleName)ct_CuttingSolid, MbSNameMaker::i_SideNone, 0/*buttAdd*/ );
        TestVariables::viewManager->ToggleObject( solidPathFrom.first );
        n.Add( 1 );

        MbeCopyMode sameShell = cm_Copy;

        SolidSPtr solid( static_cast<MbSolid *>(solidObj) );
        solid = ::GetTransformedSolid( solid, sameShell, solidPathFrom.second );

        MbMergingFlags mergeFlags( TestVariables::mergeFaces, TestVariables::mergeEdges );

        if ( TestVariables::part == 0 ) {
          MbShellCuttingParams cuttingParams( pl, *cntr, false, der, mergeFlags, closed, n );

          static bool prolongContour = false;
          if ( prolongContour )
            cuttingParams.AddSurfaceProlongType( cspt_Contour );

          res = ::SolidCutting( solid, sameShell, cuttingParams, resSolids );

          if ( sameShell == cm_KeepHistory ) {
            for ( size_t i = 0; i < solid->GetFacesCount(); ++i ) {
              MbFace * face0 = solid->GetFace( i );
//#ifdef _DRAWGI
              if ( DrawTestSolid::drawItems ) {
                TestVariables::drawImp->DrawItem( face0, TRGB_RED );
                TestVariables::drawImp->DrawItem( face0, TRGB_LIGHTGREEN );
              }
//#endif // _DRAWGI
              for ( size_t k = 0; k < resSolids.size(); k++ ) {
                MbSolid * resSolid = resSolids[k]; 
                for ( size_t j = 0; j < resSolid->GetFacesCount(); j++ ) {
                  MbFace * face = resSolid->GetFace( j );
                  if ( face0 == face && face != nullptr ) {
                    face->AddAttribute( MbVisual::Create(RGB2uint32(1, 0, 0)), true );
                  }
                }
              }
            }
          }
        }
        else {
          MbShellCuttingParams cutParams( pl, *cntr, true, der, TestVariables::part, mergeFlags, closed, n );

          res = ::SolidCutting( solid, sameShell, cutParams, resSolids );
        }
        SetWaitCursor( false );
      }

      TestVariables::viewManager->ToggleObject( curvePathFrom.first );
      if ( resSolids.size() > 0 ) 
        TestVariables::viewManager->DeleteObject( curvePathFrom.first );
    }

    Style st( solidObj->GetWidth(), solidObj->GetColor() );

    if ( resSolids.size() > 0 ) {
      TestVariables::viewManager->DeleteObject( solidPathFrom.first );
      for ( size_t i = 0; i < resSolids.size(); i++ ) {
        TestVariables::viewManager->AddObject( st, resSolids[i] );
        st.SetColor( st.GetColor() / 2 );
      }
    }
    else {
      TestVariables::viewManager->ToggleObject( solidPathFrom.first );
      ::PutErrorMessage( res, IDS_TITLE_ERR );
    }
  }
}


//------------------------------------------------------------------------------
// Создание разрезанного поверхностью тела.
// Cut a solid off by a surface.
// ---
void CutSolidBySurface( bool closed )
{
  MbItem * solidObj = nullptr;
  MbItem * surfaceObj = nullptr;
  MbCartPoint3D p0;

  PathMatrix solidPathFrom;
  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &solidObj, solidPathFrom ) ) {
    TestVariables::viewManager->ToggleObject( solidPathFrom.first );

    MbResultType res = rt_Error;

    SolidsSPtrVector resSolids;
    resSolids.reserve( 2 );

    PathMatrix surfacePathFrom;
    if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &surfaceObj, surfacePathFrom ) ) {
      TestVariables::viewManager->ToggleObject( surfacePathFrom.first );

      if ( GetInt( IDS_ENTER_NUMBER_N, TestVariables::part ) ) {
        SetWaitCursor( true );

        MbeCopyMode copyMode = cm_KeepHistory;
        SolidSPtr solid( static_cast<MbSolid *>(solidObj) );
        solid = ::GetTransformedSolid( solid, copyMode, solidPathFrom.second );

        SurfaceSPtr surface( static_cast<MbSurface *>( static_cast<MbSpaceInstance *>(surfaceObj)->SetSpaceItem() ) );
        surface = ::GetTransformedItem( surface, surfacePathFrom.second );

        MbSNameMaker n( (SimpleName)ct_CuttingSolid, MbSNameMaker::i_SideNone, 0 );
        MbMergingFlags mergeFlags( TestVariables::mergeFaces, TestVariables::mergeEdges );

        //TestVariables::viewManager->ToggleObject( solidPathFrom.first );

        MbShellCuttingParams cuttingParams( *surface, true, TestVariables::part, mergeFlags, closed, n );

        static bool prolongPlanar                = false;
        static bool prolongRevolutionAxis        = false;
        static bool prolongRevolutionAngle       = false;
        static bool prolongExtrusionGeneratrix   = false;
        static bool prolongExtrusionDistance     = false;
        static bool prolongRuledAlongGeneratrix  = false;
        static bool prolongRuledAcrossGeneratrix = false;

        if ( prolongPlanar )
          cuttingParams.AddSurfaceProlongType( cspt_Planar ); // Default
        if ( prolongRevolutionAxis )
          cuttingParams.AddSurfaceProlongType( cspt_RevolutionAxis ); // Prolong revolution along axis
        if ( prolongRevolutionAngle  )
          cuttingParams.AddSurfaceProlongType( cspt_RevolutionAngle ); // Prolong revolution along rotation
        if ( prolongExtrusionGeneratrix )
          cuttingParams.AddSurfaceProlongType( cspt_ExtrusionGeneratrix ); // Prolong extrusion along generatrix
        if ( prolongExtrusionDistance )
          cuttingParams.AddSurfaceProlongType( cspt_ExtrusionDistance ); // Prolong extrusion along distance;
        if ( prolongRuledAlongGeneratrix )
          cuttingParams.AddSurfaceProlongType( cspt_RuledAlongGeneratrix ); // Prolong ruled along generatrix
        if ( prolongRuledAcrossGeneratrix )
          cuttingParams.AddSurfaceProlongType( cspt_RuledAcrossGeneratrix ); // Prolong ruled across generatrix;

        if ( TestVariables::part == 0 ) {
          if ( copyMode != cm_Same )
            copyMode = cm_Copy;
        }

        res = ::SolidCutting( solid, copyMode, cuttingParams, resSolids );

        SetWaitCursor( false );
      }

      TestVariables::viewManager->ToggleObject( surfacePathFrom.first );
      if ( resSolids.size() > 0 )
        TestVariables::viewManager->DeleteObject( surfacePathFrom.first );
    }

    Style st( solidObj->GetWidth(), solidObj->GetColor() );

    if ( resSolids.size() > 0 ) {
      TestVariables::viewManager->DeleteObject( solidPathFrom.first );
      for ( size_t i = 0; i < resSolids.size(); ++i ) {
        TestVariables::viewManager->AddObject( st, resSolids[i] );
        st.SetColor( st.GetColor() / 2 );
      }
      if ( res != rt_Success ) {
        ::MessageBoxEx( RT_DONE_IN_PART, IDS_TITLE_ERR );
      }
    }
    else {
      TestVariables::viewManager->ToggleObject( solidPathFrom.first );
      ::MessageBoxEx( RT_BODY_NOT_CUT, IDS_TITLE_ERR );
    }
  }
} // CutSolidBySurface


//------------------------------------------------------------------------------
// Создание разрезанного оболочкой тела.
// Cut a solid off by a shell.
// ---
void CutSolidByShell( bool closed )
{
  MbItem * targetObj = nullptr;

  PathMatrix targetPathFrom;
  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &targetObj, targetPathFrom ) ) {
    TestVariables::viewManager->ToggleObject( targetPathFrom.first );

    Style st( targetObj->GetWidth(), targetObj->GetColor() );

    SolidsSPtrVector resSolids;
    resSolids.reserve( 2 );

    MbItem * cutterObj = nullptr;
    MbAccurateTimer ac;

    PathMatrix cutterPathFrom;
    if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &cutterObj, cutterPathFrom ) ) {
      TestVariables::viewManager->ToggleObject( cutterPathFrom.first );

      if ( GetInt( IDS_ENTER_NUMBER_N, TestVariables::part ) ) {
        SetWaitCursor( true );

        MbeCopyMode targetCopyMode = cm_KeepHistory;
        SolidSPtr targetSolid( static_cast<MbSolid *>(targetObj) );
        targetSolid = ::GetTransformedSolid( targetSolid, targetCopyMode, targetPathFrom.second );

        MbeCopyMode cutterCopyMode = cm_KeepHistory;
        SolidSPtr cutterSolid( static_cast<MbSolid *>(cutterObj) );
        cutterSolid = ::GetTransformedSolid( cutterSolid, cutterCopyMode, cutterPathFrom.second );

        MbSNameMaker n( (SimpleName)ct_CuttingSolid, MbSNameMaker::i_SideNone, 0 );
        MbMergingFlags mergeFlags( TestVariables::mergeFaces, TestVariables::mergeEdges );

        MbShellCuttingParams cuttingParams( *cutterSolid, (cutterCopyMode != cm_Same), TestVariables::part, mergeFlags, closed, n );

        if ( TestVariables::part == 0 ) {
          if ( targetCopyMode != cm_Same )
            targetCopyMode = cm_Copy;
        }

        ac.Begin();
        ::SolidCutting( targetSolid, targetCopyMode, cuttingParams, resSolids );
        ac.End();

        SetWaitCursor( false );
      }

      TestVariables::viewManager->ToggleObject( cutterPathFrom.first );
      if ( resSolids.size() > 0 )
        TestVariables::viewManager->DeleteObject( cutterPathFrom.first );
    }

    if ( resSolids.size() > 0 ) {
      TestVariables::viewManager->DeleteObject( targetPathFrom.first );
      for ( size_t i = 0; i < resSolids.size(); ++i ) {
        TestVariables::viewManager->AddObject( st, resSolids[i] );
        st.SetColor( st.GetColor() / 2 );
      }
      if ( TestVariables::checkOperationTime ) {
        const size_t bufLen = 1024;
        TCHAR buf[bufLen];
        size_t len = 0;
        len += (size_t)_sntprintf( buf + len, bufLen - len, RT_CUTTING_SOLID );
        len += (size_t)_sntprintf( buf + len, bufLen - len, _T("\n\n"));
        len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
        ::MessageBoxEx( buf, IDS_RESULTS );
      }
    }
    else {
      TestVariables::viewManager->ToggleObject( targetPathFrom.first );
      ::MessageBoxEx( RT_BODY_NOT_CUT, IDS_TITLE_ERR );
    }


  }
} // CutSolidByShell


//------------------------------------------------------------------------------
// Усечение незамкнутой оболочки.
// Truncate a shell by objects.
//---
void CutShellByObjects()
{
  MbItem * solObj( nullptr );

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &solObj, TestVariables::itemPathFrom ) && (solObj != nullptr) ) {
    const MbSolid * initSolid = static_cast<const MbSolid *>(solObj);
    SPtr<MbSolid> wrkSolid( static_cast<MbSolid *>(&initSolid->Duplicate( nullptr )) );

    if ( wrkSolid != nullptr ) {
      IndicesVector selIndices;
      {
        bool isFacesSelected = true;
        while( isFacesSelected ) {
          MbItem * faceObj = nullptr;
          MbTopItem * tItem = nullptr;

          isFacesSelected = GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &faceObj, &tItem, TestVariables::itemPathFrom.second ) && tItem->IsAFace();

          if ( isFacesSelected ) {
            const MbFaceShell * initShell = initSolid->GetShell();
            size_t selInd = initShell->Find( (MbFace *)tItem );

            if ( selInd != SYS_MAX_T ) {
              if ( c3d::DirectSearch( selIndices, selInd ) == SYS_MAX_T )
                selIndices.push_back( selInd );

              TestVariables::drawImp->DrawItem( initShell, TRGB_LIGHTGREEN );
              for ( size_t m = 0, mcnt = selIndices.size(); m < mcnt; m++ )
                TestVariables::drawImp->DrawItem( &initShell->GetFace( selIndices[m] )->GetSurface(), TRGB_WHITE );
            }
            else
              MessageBoxEx( RT_FACE_NOT_BELONG_BODY, IDS_TITLE_ERR );
          }
        }
      }

      TestVariables::viewManager->ToggleObject( solObj );

      std::vector< SPtr<MbItem> > cutObjs;
      RPArray<MbSpaceItem> cutItems;
      MbItem * cutObj = nullptr;

      bool cutSplitCurves = false;

      while( GetGObj( IDS_SELECT_OBJECT, st_SpaceItem, &cutObj ) && (cutObj != nullptr) ) {
        SPtr<MbSpaceItem> cutItem;

        if ( cutObj->IsA() == st_SpaceInstance ) {
          cutItem = (static_cast<MbSpaceInstance *>(cutObj))->SetSpaceItem();
          cutSplitCurves = true;
        }

        if ( cutObj->IsA() == st_PlaneInstance ) {
          MbPlaneInstance * planeInst = (MbPlaneInstance *)cutObj;
          MbPlaneItem * planeItem = planeInst->SetPlaneItem();

          if ( planeItem != nullptr && planeItem->Family() == pt_Curve ) {
            MbCurve * pCurve = static_cast<MbCurve *>(planeItem);
            MbCurve3D * curve = nullptr;
            ::PlaneCurve( planeInst->GetPlacement(), *pCurve, curve );
            cutItem = curve;
          }
        }

        if ( cutItem == nullptr ) {
          cutItem = cutObj;
          cutSplitCurves = true;
        }

        if ( cutItem == initSolid ) {
          MbCartPoint3D checkPnt( TestVariables::cursorPoint );
          MbItem * tmpObj = nullptr;
          if ( FindNearestObj( TestVariables::cursorPoint, st_Curve3D, &tmpObj ) && tmpObj != nullptr ) {
            cutObj = tmpObj;
            if ( cutObj->IsA() == st_SpaceInstance ) {
              cutItem = (static_cast<MbSpaceInstance *>(cutObj))->SetSpaceItem();
            }
          }
        }

        if ( cutItem != nullptr ) {
          if ( (cutItem->Family() == st_Curve3D) || 
               (cutItem->Family() == st_Surface) ||
               (cutItem->IsA() == st_Solid) ||
               (cutItem->IsA() == st_WireFrame ) ){
            TestVariables::viewManager->ToggleObject( cutObj );
            cutObjs.push_back( SPtr<MbItem>(cutObj) );

            if ( (cutItem->Family() == st_Curve3D) ) {
              ::AddTruncatingCurve( static_cast<MbCurve3D &>(*cutItem), cutItems );
              cutItem = nullptr;
            }
            else if ( (cutItem->Family() == st_Surface) ) {
              cutItems.push_back( cutItem );
              cutItem = nullptr;
            }
            else if ( (cutItem->IsA() == st_WireFrame) ) {
              cutItems.push_back( cutItem );
              cutItem = nullptr;
            }
            else if ( cutItem->IsA() == st_Solid ) {
              MbItem * cutFacesObj = nullptr;
              RPArray<MbFace> cutFaces( 0, 1 );
              MbTopItem * tItem = nullptr;


              while( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &cutFacesObj, &tItem, TestVariables::itemPathFrom.second ) && 
                     cutObj == cutFacesObj && tItem->IsAFace() )
              {
                if ( cutFaces.FindIt( (MbFace *)tItem ) == SYS_MAX_T ) {
                  if ( ::CheckFacesConnection( cutFaces ) )
                    cutFaces.Add( (MbFace *)tItem );
                }
              }
              if ( cutFaces.size() > 0 ) {
                c3d::ShellSPtr tmpShell( new MbFaceShell( cutFaces ) );
                MbFaceShell * newShell = tmpShell->Copy( cm_Copy, nullptr );
                if ( newShell != nullptr )
                  cutItem = new MbSolid( newShell, nullptr );
              }
              cutItems.push_back( ::DetachItem( cutItem ) );
            }
          }
        }
        cutObj = nullptr;
      }

      bool cutOrient = true;
      {
        int iInverse = 0;
        GetInt( IDS_ENTER_INVERSE, iInverse );
        cutOrient = (iInverse != 0) ? false : true;
      }
      size_t cutCnt = cutItems.size();
      c3d::BoolVector cutOrients;
      cutOrients.assign( cutCnt, cutOrient );

      Style st( solObj->GetWidth(), solObj->GetColor() );

      SetWaitCursor( true );

      MbeCopyMode copyMode = cm_KeepHistory;
      MbSNameMaker names( (SimpleName)ct_TruncatedShell, MbSNameMaker::i_SideNone, 0 );
      names.SetVersion( MbNameVersion() );
      MbSolid * resSolid = nullptr;
      MbeCopyMode itemsCopyMode = cm_KeepHistory;

      MbResultType res = rt_Error;

      MbPlacement3D resDir;

      MbMergingFlags mergeFlags( TestVariables::mergeFaces, TestVariables::mergeEdges );

      MbAccurateTimer ac;
      ac.Begin();
    
      MbTruncateShellParams trancateParams( selIndices, cutItems, cutOrients, itemsCopyMode, cutSplitCurves, mergeFlags, names );
      double prec = METRIC_PRECISION;
      if ( GetDouble( IDS_ENTER_ACCURACY, prec ) ) { // Точность операции.
        trancateParams.SetPrecision( prec );
      }
      MbTruncateShellResults trancateResult;

      res = ::TruncateShell( *wrkSolid, copyMode, trancateParams, trancateResult );
      resSolid = trancateResult.GetResultSolid();
      resDir = trancateResult.GetResultPlace();

      ac.End();
      SetWaitCursor( false );

      if ( resSolid != nullptr ) {
        MbVector3D tmpDir( resDir.GetAxisZ() );
        MbCube resSolCube;
        resSolid->AddYourGabaritTo( resSolCube );
        tmpDir *= (DELTA_MOD * resSolCube.GetDiagonal());
        TestVariables::drawImp->DrawPoint( &resDir.GetOrigin(), TRGB_BLACK  );
        TestVariables::drawImp->DrawLine( resDir.GetOrigin(), tmpDir, TRGB_BLACK );

        TestVariables::viewManager->DeleteObject( solObj );
        TestVariables::viewManager->AddObject( st, resSolid );

        if ( TestVariables::checkOperationTime ) {
          const size_t bufLen = 1024;
          TCHAR buf[bufLen];
          size_t len = 0;
          len += (size_t)_sntprintf( buf + len, bufLen - len, RT_CUTTING_SHELL );
          len += (size_t)_sntprintf( buf + len, bufLen - len, _T( "\n\n" ) );
          len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
          ::MessageBoxEx( buf, IDS_RESULTS );
        }
      }
      else {
        TestVariables::viewManager->UnselectObject( solObj );
        ::PutErrorMessage( res, IDS_TITLE_ERR );  
      }

      if ( cutObjs.size() > 0 ) {
        if ( resSolid != nullptr ) {
          for ( ptrdiff_t i = (ptrdiff_t)cutObjs.size()-1; i >= 0; i-- )
            TestVariables::viewManager->DeleteObject( cutObjs[i] );
        }
        else {
          for ( ptrdiff_t i = (ptrdiff_t)cutObjs.size()-1; i >= 0; i-- )
            TestVariables::viewManager->UnselectObject( cutObjs[i] );
        }
      }

      ::DeleteItems( cutItems );
    }
  }
}


//------------------------------------------------------------------------------
// Получить от пользователя параметры для конструирования ребер жесткости.
// Get parameters to create ribs.
//---
static bool GetRibValues( const MbSolid       & sourceSolid,
                          const MbPlacement3D & place,
                          MbContour           & cntr,
                          RibValues           & pars,
                          ptrdiff_t & index )
{
  bool res = false;
  MbCartPoint3D origin;
  MbVector3D    dir3D;

  if ( ::GetInt( IDS_ENTER_SEGMENT_INDEX, index ) ) {
    RibValues::ExtrudeSide ribExtrSide = (RibValues::ExtrudeSide)TestVariables::ribPart;
    if ( sourceSolid.GetShell() )
      ::GetAutoReference( *sourceSolid.GetShell(), place, cntr, index, ribExtrSide, origin, dir3D );

    TestVariables::ribPart = (int)ribExtrSide;
    if ( ::GetInt( IDS_ENTER_SIDE, TestVariables::ribPart ) ) {
      if ( ::GetDouble( IDS_ENTER_THICKNESS, TestVariables::thickness1 ) ) {
        TestVariables::thickness2 = TestVariables::thickness1;
        if ( ::GetDouble( IDS_ENTER_THICKNESS, TestVariables::thickness2 ) ) {
          double angle = TestVariables::angle1*180/M_PI;
          if ( ::GetDouble( IDS_ENTER_SLOPE_ANGLE, angle ) ) {
            angle *= M_PI/180;
            TestVariables::angle1 = angle;
            TestVariables::angle2 = angle;
            pars.Init( RibValues (TestVariables::thickness1, TestVariables::thickness2, TestVariables::angle1, TestVariables::angle2, TestVariables::ribPart) );
            res = true;
          }
        }
      }
    }
  }

  return res;
} // GetRibValues


//------------------------------------------------------------------------------
// Создание тела c ребром жесткости.
// Create a rib solid.
// ---
void CreateRibSolid()
{
  MbItem * solid;
  MbItem * planeCurve;

  if ( ::GetGObj( IDS_SELECT_SOLID, st_Solid, &solid, TestVariables::itemPathFrom ) ) {
    TestVariables::viewManager->ToggleObject( solid );

    if ( ::GetMObj( IDS_SELECT_CRV_ON_PLANE, pt_Curve, &planeCurve ) ) {
      MbCartPoint3D p;

      p = TestVariables::viewManager->GetActiveWindow()->GetOrigin();
      MbPlaneInstance * pobj = (MbPlaneInstance *)planeCurve;
      TestVariables::viewManager->ToggleObject(planeCurve);

      MbContour c;
      MbContour * cntr;
      const MbPlaneItem * geomItem = ((MbPlaneInstance *)planeCurve)->GetPlaneItem();
      if ( geomItem != nullptr ) {
        if ( geomItem->Type() == pt_Contour )
          cntr = (MbContour *)geomItem;
        else {
          c.AddSegment( static_cast<MbCurve *>( &geomItem->Duplicate() ) );
          cntr = &c;
        }
        MbPlacement3D place( pobj->GetPlacement() );
        MbCartPoint mapPoint;
        place.PointProjection( p, mapPoint.x, mapPoint.y );

        ptrdiff_t index = cntr->GetSegmentsCount()/2;

        MbSolid  & sourceSolid  = *(MbSolid *)solid;
        RibValues pars;
        if ( ::GetRibValues( sourceSolid, place, *cntr, pars, index ) ) {

          SetWaitCursor( true );

          MbSNameMaker name( (SimpleName)ct_RibSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
          name.Add( 1 );

          MbSolid  * result;
          SolidSPtr resSPtr;
          MbResultType res = ::RibSolid( sourceSolid, cm_Copy, 
                                         MbRibSolidParameters(place, *cntr, index, pars, name), 
                                         resSPtr );
          result = resSPtr.detach();
          if ( res != rt_Success )
            ::PutErrorMessage( res, IDS_RIB_ERROR );
          if ( result ) {
            TestVariables::viewManager->DeleteObject( solid );

            Style ns = TestVariables::SOLID_Style;
            TestVariables::viewManager->AddObject( ns, result );
          }
          else {
            TestVariables::viewManager->ToggleObject(solid);
          }
          SetWaitCursor( false );
        }
      }
      TestVariables::viewManager->ToggleObject( planeCurve );
    }
  }
} // CreateRibSolid()


//------------------------------------------------------------------------------
// Создание оболочки РЖ без булевого результата.
// Create a rib shell without union with initial solid.
// ---
void CreateRibOpenShell()
{
  MbItem * solid = nullptr;
  MbItem * planeCurve = nullptr;
  // Запросить исходное тело.
  // Select an initial solid.
  if ( ::GetGObj( IDS_SELECT_SOLID, st_Solid, &solid, TestVariables::itemPathFrom ) ) {
    TestVariables::viewManager->ToggleObject( solid );
    // Запросить формообразующую кривую на плоскости.
    // Select generatrix on plane.
    if ( ::GetMObj( IDS_SELECT_CRV_ON_PLANE, pt_Curve, &planeCurve ) ) {
      MbCartPoint3D p;

      p = TestVariables::viewManager->GetActiveWindow()->GetOrigin();
//      viewManager->GetActiveWindow()->GetMapPoint( p );
      MbPlaneInstance * pobj = (MbPlaneInstance *)planeCurve;
      TestVariables::viewManager->ToggleObject(planeCurve);

      MbContour c;
      MbContour * cntr;

      const MbPlaneItem * geomItem = ((MbPlaneInstance *)planeCurve)->GetPlaneItem();

      if ( geomItem != nullptr ) {
        if ( geomItem->Type() == pt_Contour )
          cntr = (MbContour *)geomItem;
        else {
          c.AddSegment( static_cast<MbCurve *>( &geomItem->Duplicate() ) );
          cntr = &c;
        }
        MbPlacement3D place( pobj->GetPlacement() );
        MbCartPoint mapPoint;
        place.PointProjection( p, mapPoint.x, mapPoint.y );

        // Номер сегмента, определяющего своей серединой начало вектора уклона и место определения толщины РЖ
        ptrdiff_t index = cntr->GetSegmentsCount()/2;

        MbSolid  & sourceSolid  = *(MbSolid *)solid;


        RibValues pars;
        if ( ::GetRibValues( sourceSolid, place, *cntr, pars, index ) ) {

          SetWaitCursor( true );
          MbSNameMaker name( (SimpleName)ct_RibSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
          name.Add( 1 );
          MbSolid * result= nullptr;
          SolidSPtr resSPtr;
          MbResultType res = ::RibElement( sourceSolid, MbRibSolidParameters(place, *cntr, index, pars, name), resSPtr );
          result = resSPtr.detach();
          if ( res != rt_Success )
            ::PutErrorMessage( res, IDS_RIB_ERROR );

          if ( result ) {
            //TestVariables::viewManager->DeleteObject( solid ); 
            Style ns = TestVariables::GOLDEN_Style;
            TestVariables::viewManager->AddObject( ns, result );
            TestVariables::viewManager->ToggleObject(result);
          }
          else {
            TestVariables::viewManager->ToggleObject(solid);
          }

          SetWaitCursor( false );
        }
      }
      TestVariables::viewManager->ToggleObject( planeCurve );
    }
  }
} // CreateRibOpenShell


//------------------------------------------------------------------------------
// Создание тела c уклоном от нейтральной плоскости.
// Slope the specified faces of the solid at the specified angle relative to the neutral isoplane.
// ---
void CreateDraftSolidNP()
{
  // Запросить исходное тело.
  // Select an initial solid.
  MbItem * solid = nullptr;

  if ( ::GetGObj( IDS_SELECT_SOLID, st_Solid, &solid, TestVariables::itemPathFrom ) ) {
    TestVariables::viewManager->ToggleObject( solid );

    // Запросить грань тела нейтральной плоскости
    // Select a face for the neutral isoplane.
    MbSurface * np = nullptr;
    MbTopItem * topItem = nullptr;
    MbItem * obj = nullptr;
    bool sense = true;

    if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &topItem, TestVariables::itemPathFrom.second ) ) {
      TestVariables::drawImp->DrawItem( topItem, TRGB_WHITE );

      MbFace * face = static_cast<MbFace *>(topItem);
      if ( face->IsPlanar() ) {
        np = &face->SetSurface().SetSurface();
        sense = face->IsSameSense();
      }
    }
    else {
      MbSpaceItem * spaceItem = nullptr;
      if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &obj, &spaceItem, TestVariables::itemPathFrom ) ) {
        if ( spaceItem != nullptr && spaceItem->Family() == st_Surface ) {
          TestVariables::drawImp->DrawItem( spaceItem, TRGB_WHITE );
          np = &static_cast<MbSurface *>(spaceItem)->SetSurface();
          if ( !np->IsPlanar() )
            np = nullptr;
        }
      }
    }

    if ( (np != nullptr) && (np->IsA() == st_Plane) ) {
      MbPlacement3D neutral( static_cast<const MbPlane *>(np)->GetPlacement() );
      if ( !sense )
        neutral.Invert();

      MbSolid & sourceSolid = *(MbSolid *)solid;
      const double bnd1 = 5;
      const double bnd2 = 15;
                           
      ::srand( 1 );
      int rnd = ::rand( /*fabs( bnd2 - bnd1) + 1*/ );

      double angle = std_min( bnd1, bnd2 ) + rnd; // Угол уклона РЖ

      if ( ::GetDouble( IDS_ENTER_SLOPE_ANGLE, angle ) ) { // "Введите угол наклона"
        angle *= M_PI/180;

        // Ввод уклоняемых граней
        // Select faces for slope.
        RPArray<MbFace> faces( 5, 1 );

        while ( GetTObj( IDS_SELECT_INCLINE_FACE, st_Solid, tt_Face, &obj, &topItem, TestVariables::itemPathFrom.second ) ) {
          MbFace * face = static_cast<MbFace *>(topItem);
          size_t i0 = faces.FindIt( face );

          if ( i0 != SYS_MAX_T ) {
            TestVariables::drawImp->DrawItem( topItem, TRGB_LIGHTGREEN );
            faces.DetachInd( i0 );
          }
          else {
            TestVariables::drawImp->DrawItem( topItem, TRGB_WHITE );
            faces.Add(face);
          }
        }
        TestVariables::viewManager->ToggleObject(obj);
       
        // Ввод уклоняемых ребер
        RPArray<MbCurveEdge> edges( 5 );

        while ( GetTObj( IDS_SELECT_EDGE , st_Solid, tt_CurveEdge, &obj, &topItem, TestVariables::itemPathFrom.second ) ) {
          MbCurveEdge * edge = static_cast<MbCurveEdge *>(topItem);
          size_t i0= edges.FindIt( edge );

          if ( i0 != SYS_MAX_T ) {
            TestVariables::drawImp->DrawItem( topItem, TRGB_LIGHTBLUE );
            edges.DetachInd( i0 );
          }
          else {
            TestVariables::drawImp->DrawItem( topItem, TRGB_WHITE );
            edges.Add(edge);
          }
        }
        TestVariables::viewManager->ToggleObject(obj);

        int fp = 2;//fp_Along;  // Признак захвата граней ( face propagation )
        if ( ::GetInt( IDS_ENTER_NUMBER_N, fp ) ) { // "Укажите номер"

          int reverse = 1;   // Обратное направление тяги
          if ( ::GetInt(  IDS_ENTER_SENSE, reverse ) ) { // "Введите направление ( +1 или -1 )"
            reverse = reverse < 0;
            bool rebuildFillets = true;
            bool  isCreatePhantom( false );
            if ( ::GetBool(  IDS_SPECIFY_REBUILD_FILLETS, rebuildFillets ) ) { // Перестроение скруглений true/false
              ::GetBoolean( IDS_SPECIFY_CREATE_PHANTOM, isCreatePhantom, false );
              SetWaitCursor( true );
              MbSNameMaker name( (SimpleName)ct_DraftSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
              name.Add( 1 );
              SolidSPtr result;
              MbDraftSolidResults draftSolidResults;
              // KYA K11 чтобы увеличить нужное место перед запуском (if you want to magnify specific place before starting)
              //MbItem * obj = nullptr;
              //::GetGObj( IDS_SELECT_SOLID, st_Solid, &obj, TestVariables::itemPathFrom.second );
              FacesSPtrVector faces1;
              EdgesSPtrVector edges1;
              InitSPtrArray< RPArray<MbFace>, MbFace >( faces, faces1 );
              InitSPtrArray< RPArray<MbCurveEdge>, MbCurveEdge >( edges, edges1 );
              MbVector3D dir( neutral.GetAxisZ() );
              MbDraftSolidParams draftParams( faces1, edges1, dir, name, angle, (MbeFacePropagation)fp, reverse != 0, rebuildFillets );
              draftParams.SetFantom( isCreatePhantom );
              MbResultType res = ::DraftSolid( sourceSolid, cm_Copy, draftParams, draftSolidResults );
              if ( res != rt_Success )
                ::PutErrorMessage( res, IDS_CREATING_ANGLE );
              TestVariables::viewManager->DeleteObject( solid );
              Style ns = TestVariables::SOLID_Style;
              TestVariables::viewManager->AddObject( ns, draftSolidResults.GetResultSolid().get() );
              MbCartPoint3D hotPoint;
              MbAxis3D axis;
              if ( draftParams.IsPhantom() && draftSolidResults.GetDraftHotPoint(hotPoint, axis) ) {
                TestVariables::viewManager->AddObject( ns, new MbLineSegment3D(axis.GetOrigin(), axis.GetAxisZ()) );
                TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPoint3D(hotPoint) );
              }       
              else {
                TestVariables::viewManager->ToggleObject( solid );
              }
              SetWaitCursor( false );
              
            }
          }
        }
      }
    }
  }
} // CreateDraftSolidNP


//------------------------------------------------------------------------------
// Получить набор плоских контуров для линии разъема.
// Form a set of plane contours for the face-splitting operation.
// ---
static
bool AddSplitSketch( MbPlacement3D & basePlace, RPArray<MbContour> & pCntrs, bool & isFirst )
{
  bool isAdded = false;
  MbItem * obj = nullptr;
  MbContour * pCntr = nullptr;

  MbPlacement3D objPlace;

  if ( GetObj( IDS_SELECT_CRV_ON_PLANE, &obj) && obj != nullptr ) {
    if ( obj->IsA() == st_PlaneInstance ) {
      MbPlaneItem * planeItem = ((MbPlaneInstance *)obj)->SetPlaneItem(); // Get plane object.
      if ( (planeItem != nullptr) && (planeItem->Family() == pt_Curve) ) {
        MbCurve * curve = static_cast<MbCurve *>(planeItem);

        if ( curve->Type() == pt_Contour )
          pCntr = static_cast<MbContour *>(curve);
        else {
          MbContour * pCntrNew = new MbContour;
          pCntrNew->AddSegment( static_cast<MbCurve *>(&curve->Duplicate()) );
          pCntr = pCntrNew;
        }
        objPlace = ((const MbPlaneInstance *)obj)->GetPlacement();
      }
    }
    if ( obj->IsA() == st_SpaceInstance ) {
      const MbSpaceItem * spaceItem = ((MbSpaceInstance *)obj)->GetSpaceItem(); // Get space object.
      if ( (spaceItem != nullptr) && (spaceItem->Family() == st_Curve3D) ) {
        MbCurve3D * sCurve = (MbCurve3D *)spaceItem;
        MbCurve * pCurve = nullptr;
        if ( sCurve->GetPlaneCurve( pCurve, objPlace, true ) ) {
          if ( pCurve->Type() == pt_Contour )
            pCntr = (MbContour *)pCurve;
          else {
            MbContour * pCntrNew = new MbContour;
            pCntrNew->AddSegment( static_cast<MbCurve *>(&pCurve->Duplicate()) );
            pCntr = pCntrNew;
          }
        }
      }
    }
  }

  if ( pCntr != nullptr ) {
    TestVariables::viewManager->ToggleObject( obj );

    if ( isFirst ) {
      basePlace.Init( objPlace );
      ::DeleteItems( pCntrs );
      pCntrs.Add( pCntr );
      isAdded = true;
      isFirst = false;
    }
    else {
      if ( basePlace.GetAxisZ().Colinear( objPlace.GetAxisZ() ) ) {
        MbMatrix matr;
        objPlace.GetMatrixToPlace( basePlace, matr );

        if ( !matr.IsSingle() ) {
          MbContour * pCntrNew = (MbContour *)&pCntr->Duplicate();
          ::DeleteItem( pCntr );
          pCntr = pCntrNew;
          pCntr->Transform( matr );
        }

        pCntrs.Add( pCntr );
        isAdded = true;
      }
    }

    if ( !isAdded )
      TestVariables::viewManager->ToggleObject( obj );
  }

  return isAdded;
}


//------------------------------------------------------------------------------
// Получить набор пространственных кривых для линии разъема.
// Form a set of space curves for the face-splitting operation.
// ---
static
bool AddSplitItems( RPArray<MbSpaceItem> & spaceItems )
{
  MbItem * spaceObj = nullptr;
  RPArray<MbItem> spaceObjs( 2, 2 );

  while( GetGObj( IDS_SELECT_OBJECT, st_SpaceItem, &spaceObj ) ) {
    MbSpaceItem * spaceItem = nullptr;

    if ( spaceObj->IsA() == st_SpaceInstance ) 
      spaceItem = (static_cast<MbSpaceInstance *>(spaceObj))->SetSpaceItem();
    if ( spaceItem == nullptr )
      spaceItem = spaceObj;

    if ( spaceItem != nullptr ) {
      if ( (spaceItem->Family() == st_Curve3D) || 
            (spaceItem->Family() == st_Surface) ||
            (spaceItem->IsA() == st_WireFrame) ||
            (spaceItem->IsA() == st_Solid) ) {
        TestVariables::viewManager->ToggleObject( spaceObj );
        spaceObjs.Add( spaceObj );

        if ( spaceItem->Family() == st_Curve3D ) {
          ::AddTruncatingCurve( *static_cast<MbCurve3D *>(spaceItem), spaceItems );
          ::DeleteItem( spaceItem );
        }
        else if ( spaceItem->IsA() == st_WireFrame ) {
          MbWireFrame * wf = static_cast<MbWireFrame *>(spaceItem);
          c3d::ConstSpaceCurvesSPtrVector curves;
          wf->GetCurves( curves );
          for ( size_t m = 0, addCnt = curves.size(); m < addCnt; ++m )
            ::AddTruncatingCurve( *curves[m], spaceItems );
        }
        else {
          if ( spaceItem->IsA() == st_Solid ) {
            MbItem * spaceFacesObj = nullptr;
            RPArray<MbFace> cutFaces( 0, 1 );
            MbTopItem * tItem = nullptr;

            while( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &spaceFacesObj, &tItem, TestVariables::itemPathFrom.second ) && 
                    spaceObj == spaceFacesObj && tItem->IsAFace() )
            {
              if ( cutFaces.FindIt( (MbFace *)tItem ) == SYS_MAX_T ) {
                if ( ::CheckFacesConnection( cutFaces ) )
                  cutFaces.Add( (MbFace *)tItem );
              }
            }
            if ( cutFaces.Count() > 0 ) {
              MbFaceShell * tmpShell = new MbFaceShell( cutFaces );
              MbFaceShell * newShell = tmpShell->Copy( cm_Copy, nullptr );
              if ( newShell != nullptr ) {
                ::DeleteItem( tmpShell );
                spaceItem = new MbSolid( newShell, nullptr );
              }
            }
          }
          spaceItems.Add( spaceItem );
        }
      }
    }
  }

  return spaceItems.size() > 0;
}


//------------------------------------------------------------------------------
// Создание тела c подразбиением грани.
// Perform splitting of the shell faces.
// ---
void SplitSolidFaces()
{
  MbItem * solid = nullptr;

  // Запросить исходное тело
  // Select an initial solid.
  if ( ::GetGObj( IDS_SELECT_SOLID, st_Solid, &solid, TestVariables::itemPathFrom ) ) {

    TestVariables::viewManager->ToggleObject( solid );
    MbSolid  & sourceSolid  = *(MbSolid *)solid;
    
    MbPlacement3D place;
    RPArray<MbContour> pContours( 0, 1 );
    RPArray<MbSpaceItem> spaceItems( 0, 1 );

    // Запросить формообразующую кривую для линии разъема.
    // Select generatrix for splitting operation.
    bool isFirst = true;
    while( ::AddSplitSketch( place, pContours, isFirst ) );

    if ( pContours.Count() < 1 ) {
      ::AddSplitItems( spaceItems ); // Вынесен код в функцию.
    }

    ::AddRefItems( pContours );
    ::AddRefItems( spaceItems );

    // Запросить формообразующую кривую для линии разъема
    if ( (pContours.Count() > 0) != (spaceItems.Count() > 0) ) {
      // Ввод разбиваемых граней уклоняемых граней
      RPArray<MbFace>   faces( 5, 1 ); 
      SArray<ptrdiff_t> nFace( 5, 1 ); // Номера множества уклоняемых граней. 

      MbTopItem * ti = nullptr;
      MbItem * obj = nullptr;

      while ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, & ti, TestVariables::itemPathFrom.second ) ) {
        MbFace * face = static_cast<MbFace *>(ti);
        size_t const i0 = faces.FindIt( face );

        if ( i0 != SYS_MAX_T ) {
          TestVariables::drawImp->DrawItem( ti, TRGB_LIGHTGREEN );
          faces.RemoveInd( i0, noDelete );
        }
        else {
          TestVariables::drawImp->DrawItem( ti, TRGB_WHITE );
          faces.Add(face);
        }
      }

      if ( faces.Count() < 1 ) {
        sourceSolid.GetFaces( faces );
      }

      int spType = 0;
      if ( spaceItems.Count() > 0 || ::GetInt(  IDS_STRING_SENSE_VALUE, spType ) ) { // "Введите направление" ("Enter a direction")
        SetWaitCursor( true );
        MbSNameMaker name( (SimpleName)ct_DraftSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
        name.Add( 1 );
        MbSolid * result = nullptr;

        MbResultType res = rt_Error;

        MbMergingFlags mergeFlags( TestVariables::mergeFaces, TestVariables::mergeEdges );

        TestVariables::viewManager->ToggleObject( solid );
        
        if ( pContours.Count() > 0 )
          res = ::SplitSolid( sourceSolid, cm_Copy, place, (MbeSenseValue)spType, pContours, false, faces, mergeFlags, name, result );
        else
          res = ::SplitSolid( sourceSolid, cm_Copy, spaceItems, false, faces, mergeFlags, name, result );

        if ( res != rt_Success && res != rt_SplitWireNotAllFaces )
          ::PutErrorMessage( res, IDS_FACE_SUBDIVISION );
        else if ( res == rt_SplitWireNotAllFaces ) {
          MessageBoxEx( RT_DONE_IN_PART, IDS_FACE_SUBDIVISION );
        }

        if ( result ) {
          TestVariables::viewManager->DeleteObject( solid );
          TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, result );
        }

        SetWaitCursor( false );
      }
    }

    ::ReleaseItems( pContours );
    ::ReleaseItems( spaceItems );
  }
} 


//------------------------------------------------------------------------------
// Объединение/Вычитание/Пересечение двух тел
// Perform a Boolean operation. The function performs the specified Boolean operation on two solids.
// ---
void CreateBooleanSolid( OperationType operType )
{
  MbItem * item0 = nullptr;
  PathMatrix pathFrom0;

  if ( GetGObj( IDS_SELECT_FIRST_SOLID, st_Solid, &item0, pathFrom0 ) ) {
    SolidSPtr result;
    Style ns( item0->GetWidth(), item0->GetColor() );

    MbeCopyMode copyMode0 = cm_Copy;
    bool canToggleBack0 = (copyMode0 == cm_Copy || copyMode0 == cm_KeepHistory);
    TestVariables::viewManager->ToggleObject( item0 );

    MbItem * item1 = nullptr;
    PathMatrix pathFrom1;
    MbResultType res = rt_SolidError;

    MbAccurateTimer ac;

    if ( GetGObj( IDS_SELECT_SECOND_SOLID, st_Solid, &item1, pathFrom1 ) ) {
      MbeCopyMode copyMode1 = cm_Copy;
      bool canToggleBack1 = (copyMode0 == cm_Copy || copyMode0 == cm_KeepHistory);

      if ( item0 == item1 ) {
        RPArray<MbItem> allSolids( 0, 1 );
        SArray<MbMatrix3D> matrs( 0, 1 );
        TestVariables::viewManager->GetObjects( st_Solid, allSolids, matrs );
        if ( allSolids.Count() > 1 ) {
          size_t ind = allSolids.FindIt( item0 );
          if ( ind != SYS_MAX_T )
            allSolids.DetachInd( ind );
          if ( allSolids.Count() == 1 )
            item1 = allSolids[0];
        }
      }
      TestVariables::viewManager->ToggleObject( item1 );

      // KYA K11 активировать пустышку, если после выбора операндов нужно увеличение в проблемном месте
      // KYA K11 if you want to magnify specific place before starting
      // KYA K11 MbItem * dummyForScaling = nullptr;
      // KYA K11 GetGObj( IDS_SELECT_SOLID, st_Solid, &dummyForScaling, TestVariables::itemPathFrom.second );

      SolidSPtr solid0;

      if ( item0->IsA() == st_Solid ) {
        solid0 = static_cast<MbSolid *>(item0);
        if ( !pathFrom0.second.IsSingle() ) {
          MbSolid * result0 = nullptr;
          TransformValues tv( pathFrom0.second );
          MbSNameMaker tn;
          tn.SetMainName( ct_TransformedSolid );
          ::TransformedSolid( *solid0, cm_Copy, tv, tn, result0 );
          if ( result0 != nullptr ) {
            solid0 = result0;
            copyMode0 = cm_Same;
          }
        }
      }
      else if ( item0->IsA() == st_Assembly ) {
        MbAssembly * assembly = static_cast<MbAssembly *>(item0);
        if ( assembly->ItemsCount() == 1 ) {
          MbItem * subItem = assembly->SetItem(0);
          if ( (subItem != nullptr) && (subItem->IsA() == st_Solid) ) {
            MbMatrix3D matr;
            assembly->GetPlacement().GetMatrixFrom( matr );
            if ( matr.IsSingle() )
              solid0 = static_cast<MbSolid *>(subItem);
          }
        }
      }

      SolidSPtr solid1;

      if ( item1->IsA() == st_Solid ) {
        solid1 = static_cast<MbSolid *>(item1);
        if ( !pathFrom1.second.IsSingle() ) {
          MbSolid * result1 = nullptr;
          TransformValues tv( pathFrom1.second );
          MbSNameMaker tn;
          tn.SetMainName( ct_TransformedSolid );
          ::TransformedSolid( *solid1, cm_Copy, tv, tn, result1 );
          if ( result1 != nullptr ) {
            solid1 = result1;
            copyMode1 = cm_Same;
          }
        }
      }
      else if ( item1->IsA() == st_Assembly ) {
        MbAssembly * assembly = static_cast<MbAssembly *>(item1);
        if ( assembly->ItemsCount() == 1 ) {
          MbItem * subItem = assembly->SetItem(0);
          if ( (subItem != nullptr) && (subItem->IsA() == st_Solid) ) {
            MbMatrix3D matr;
            assembly->GetPlacement().GetMatrixFrom( matr );
            if ( matr.IsSingle() )
              solid1 = static_cast<MbSolid *>(subItem);
          }
        }
      }

      if ( (solid0 != nullptr) && (solid1 != nullptr) ) {
        SetWaitCursor( true );

        MbSNameMaker operNames( ct_BooleanSolid, MbSNameMaker::i_SideNone, 0 );
        operNames.SetVersion( MbNameVersion() );      

        MbBooleanFlags flags;
        flags.InitBoolean( TestVariables::closedItem );
        flags.SetMergingFaces( TestVariables::mergeFaces );
        flags.SetMergingEdges( TestVariables::mergeEdges );

        solid0->SetOwnChangedThrough( tct_Unchanged );
        solid1->SetOwnChangedThrough( tct_Unchanged );

        DPtr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyCommon ) );

        ac.Begin();
        try {
          res = ::BooleanResult( solid0, copyMode0, solid1, copyMode1, MbBooleanOperationParams( operType, flags, operNames, progBar ), result );
        }
        catch ( const std::bad_alloc & ) {
          MessageBoxEx( RT_ERROR_BAD_ALLOC, IDS_TITLE_ERR );
          return;
        }
        catch ( ... ) {
          MessageBoxEx( RT_ERROR_GENERAL_EXCEPTION, IDS_TITLE_ERR );
          return;
        }
        ac.End();

        SetWaitCursor( false );

        if ( res == rt_NotEnoughMemory || res ==  rt_InternalError ) {
          canToggleBack0 = false;
          canToggleBack1 = false;
        }
      }
      if ( canToggleBack1 )
        TestVariables::viewManager->ToggleObject( item1 );
    }
    if ( canToggleBack0 )
      TestVariables::viewManager->ToggleObject( item0 );

    if ( result != nullptr ) {
      MbSolid * resultSolid = ::DetachItem( result );
      TestVariables::viewManager->DeleteObject( item0 );
      TestVariables::viewManager->DeleteObject( item1 );
      TestVariables::viewManager->AddObject( ns, resultSolid );
      TestVariables::viewManager->ToggleObject( resultSolid );

      if ( TestVariables::checkOperationTime ) {
        const size_t bufLen = 1024;
        TCHAR buf[bufLen];
        size_t len = 0;
        len += (size_t)_sntprintf( buf + len, bufLen - len, RT_BOOLEAN_OPERATION );
        len += (size_t)_sntprintf( buf + len, bufLen - len, _T( "\n\n" ) );
        len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
        ::MessageBoxEx( buf, IDS_RESULTS );
      }
    }
    else {
      switch ( res ) {
        case rt_NoIntersect:
          MessageBoxEx( RT_NOINTERSECT, IDS_TITLE_ERR );
          break;
        case rt_BooleanError:
          MessageBoxEx( RT_BOOLEANERROR, IDS_TITLE_ERR );
          break;
        case rt_NoObjectForDirection:
          MessageBoxEx( RT_NO_OBJECT_TO_DIRECTION, IDS_TITLE_ERR );
          break;
        case rt_ParameterError:
          MessageBoxEx( RT_PARAMETER_ERROR, IDS_TITLE_ERR );
          break;
        case rt_TopologyError:
          MessageBoxEx( RT_TOPOLOGY_ERROR, IDS_TITLE_ERR );
          break;
        case rt_ProcessIsStopped:
          MessageBoxEx( RT_PROCESS_INTERRUPTED, IDS_TITLE_ERR );
          break;
        case rt_NotEnoughMemory:
          MessageBoxEx( RT_OUT_OF_MEM_ERR, IDS_TITLE_ERR );
          break;
        default :
          MessageBoxEx( RT_UNKNOWN_ERROR, IDS_TITLE_ERR );
          break;
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание симметричного тела.
// Create a mirror copy of a solid relative to a plane.
// ---
void CreateMirrorSolid()
{
  MbItem * obj = nullptr;
  MbItem * sobj = nullptr;
  MbItem * pln = nullptr;
  MbSolid * result = nullptr;
 
  MbTopItem * surface = nullptr;
  Style ns = TestVariables::SOLID_Style;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj, TestVariables::itemPathFrom ) ) {
    TestVariables::viewManager->ToggleObject(obj);
    MbCartPoint3D origin, p0, p1;
    MbVector3D vx, vy;

    if ( GetPoint( IDS_SELECT_START_POINT, p0 ) &&
        (FindNearestObj( p0, st_Plane, &pln ) ||
         FindNearestObj( p0, st_Solid, tt_Face, &sobj, &surface ) ||
         GetPoint( IDS_SELECT_FINAL_POINT, p1 )) )
    {
      int side = 1;

      if ( pln != nullptr ) {
        MbPlane *p = (MbPlane *)((MbSpaceInstance *)pln)->GetSpaceItem();
        origin = p->GetPlacement().GetOrigin();
        vx = p->GetPlacement().GetAxisX();
        vy = p->GetPlacement().GetAxisY();
        TestVariables::viewManager->ToggleObject(pln);
      }
      else
      if ( surface != nullptr ) {
        MbPlacement3D place;
        ((MbFace *)surface)->GetPlacement( &place );
        origin = place.GetOrigin();
        vx = place.GetAxisX();
        vy = place.GetAxisY();
        side = -1;
        TestVariables::drawImp->DrawItem( surface, TRGB_WHITE );
      }
      else {
        MbLine3D line( p0, p1 );
        TestVariables::drawImp->DrawItem( &line, TRGB_WHITE );
        const WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();
        origin = p0;
        vx.Init( p0, p1 );
        vy = currentView3D->GetAxisZ();
      }

      if ( GetInt( IDS_ENTER_NUMBER_N, side ) ) {
        Style st( obj->GetWidth(), obj->GetColor() );
        if (ns==st)
          ns = TestVariables::SWEPT_Style;
        MbSNameMaker n( (SimpleName)ct_CuttingSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );

        SetWaitCursor( true );

        MbPlacement3D pl(vx,vy,origin);
        MbResultType res;
        if ( side == 0 ) { // Создание зеркальной копии тела относительно плоскости
          SolidSPtr resultSPtr, copySolid( &static_cast<MbSolid &>( obj->Duplicate() ) );
          MbSymmetrySolidParams symParams( pl, n, ts_neutral );
          res = ::SymmetrySolid( *copySolid, cm_Copy, symParams, resultSPtr ); //res = ::MirrorSolid( *(MbSolid *)obj, pl, n, result );
          result = resultSPtr.detach();
        }
        else {    
          SolidSPtr solid;
          res = ::SymmetrySolid( *static_cast<MbSolid *>(obj), cm_Copy,  MbSymmetrySolidParams(pl, n, static_cast<ThreeStates>(side)), solid );  
          result = solid.detach();
        }

        if ( result==nullptr )
          ::PutErrorMessage( res, IDS_TITLE_ERR );

        SetWaitCursor( false );
      }
      if (pln!=nullptr)
        TestVariables::viewManager->ToggleObject(pln);
    }
    if (result!=nullptr)
      TestVariables::viewManager->DeleteObject( obj );
    else
      TestVariables::viewManager->ToggleObject(obj);

  }

  if ( result != nullptr )
    TestVariables::viewManager->AddObject( ns, result );
}


//------------------------------------------------------------------------------
// Проименовать фантом.
// Set names for a phantom shell.
// ---
inline
void SetPhantomNames( MbFaceShell * phantom, SimpleName mainName )
{
  if ( phantom != nullptr ) {
    MbSNameMaker name( ct_AttributeProvider, MbSNameMaker::i_SideNone, 0/*buttAdd*/ );
    name.Flush();
    name.SetMainName( mainName ); // Set the mainName

    RPArray <MbCurveEdge> edges( 0, 1 );
    RPArray<MbFace>       faces( 0, 1 );

    phantom->GetEdges( edges );
    phantom->GetFaces( faces );

    ::SetShellNames( edges, faces, name, true );
  }
}


//------------------------------------------------------------------------------
// Создание эквидистантного тела.
// Create an offset solid.
// ---
void CreateOffsetSolid()
{
  MbItem  * obj = nullptr;
  MbSolid * solid = nullptr;
  Style ns = TestVariables::BOOLEAN_Style;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj, TestVariables::itemPathFrom ) ) {
    MbSolid * initSolid = static_cast<MbSolid *>(obj);

    TestVariables::viewManager->ToggleObject(obj);

    if ( initSolid != nullptr && GetDouble( IDS_ENTER_DISTANCE, TestVariables::thickness ) ) {
      Style st( obj->GetWidth(), obj->GetColor() );
      if (ns == st)
        ns = TestVariables::SWEPT_Style;

      MbSNameMaker operNames( ct_ShellSolid, MbSNameMaker::i_SideNone, 0 );
      operNames.SetVersion( MbNameVersion() );

      MbResultType res = rt_Error;
      MbeCopyMode copyMode = cm_Copy;

      TestVariables::viewManager->ToggleObject(obj);

      SetWaitCursor( true );

      // Create an offset solid.
      res = ::OffsetSolid( *initSolid, copyMode, TestVariables::thickness, operNames, solid );

      SetWaitCursor( false );

      if ( solid == nullptr ) {
        MessageBoxEx( RT_BODY_IS_NOT_BUILD, IDS_TITLE_ERR );
      }  
      else if ( res == rt_Success ) {
        TestVariables::viewManager->DeleteObject( obj );
        TestVariables::viewManager->AddObject( ns, solid );
        obj = nullptr;
      } 
    }
  }
}


//------------------------------------------------------------------------------
// Создание однотолщинного эквидистантного тела.
// Create a thin-walled solid.
// ---
void CreateMonoThickOffsetSolid( bool closed )
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj, TestVariables::itemPathFrom ) ) {
    MbSolid * initSolid = static_cast<MbSolid *>(obj);
    TestVariables::viewManager->ToggleObject(obj);

    if ( GetDouble( IDS_ENTER_DISTANCE, TestVariables::thickness ) ) {
      Style ns = TestVariables::BOOLEAN_Style;
      Style st( obj->GetWidth(), obj->GetColor() );
      if ( ns == st )
        ns = TestVariables::SWEPT_Style;

      SweptValues p;
      p.SetMergingFaces( TestVariables::mergeFaces );
      p.SetMergingEdges( TestVariables::mergeEdges );

      bool bothSides = false;
      if ( !closed ) {
        int iBoth = 0;
        GetInt( IDS_ENTER_BOTH_SIDES, iBoth );
        bothSides = (iBoth != 0);
      }
      if ( bothSides ) {
        p.thickness1 = ::fabs(TestVariables::thickness);
        p.thickness2 = p.thickness1;
      }
      else {
        double thickness0 = 0.0;
        if ( TestVariables::thickness < thickness0 ) {
          p.thickness1 = thickness0;
          p.thickness2 = -TestVariables::thickness;
        }
        else {
          p.thickness1 = TestVariables::thickness;
          p.thickness2 = thickness0;
        }
      }

      MbSNameMaker operNames( ct_ShellSolid, MbSNameMaker::i_SideNone, 0 );
      operNames.SetVersion( MbNameVersion() );

      RPArray<MbFace> outFaces;

      if ( DrawTestSolid::checkPhantom ) {
        SweptValues params(p);

        MbFaceShell * phantom = nullptr;

        RPArray<MbFace> offFaces;
        SArray<double>  offDists;

        size_t hpShellFaceInd = SYS_MAX_T;

        SetWaitCursor( true );

        ::OffsetPhantom( *initSolid, outFaces, offFaces, offDists, params, operNames, phantom, &hpShellFaceInd ); 

        SetWaitCursor( false );

        if ( phantom != nullptr ) {
          TestVariables::drawImp->DrawItem( phantom, TRGB_WHITE );
          TestVariables::drawImp->DrawItem( phantom, TRGB_PURPLE );
          if ( hpShellFaceInd != SYS_MAX_T )
            TestVariables::drawImp->DrawItem( initSolid->GetShell()->GetFace( hpShellFaceInd ), TRGB_AZURE );
        }

        SetWaitCursor( true );

        ::SetPhantomNames( phantom, initSolid->GetItemName() );

        SetWaitCursor( false );

        ::DeleteItem( phantom );
      }

      MbResultType res = rt_Error;

      MbSolid * wrkSolid = initSolid;
      MbSolid * resSolid = nullptr;

      MbeCopyMode copyMode = cm_Copy;
      bool copyAttrs = TestVariables::copyAttributesOnDemand;
      TestVariables::viewManager->ToggleObject(obj);

      if ( !closed ) {
        SweptValues offsetShellParams;
        offsetShellParams.shellClosed = false;
        offsetShellParams.SetMergingFaces( TestVariables::mergeFaces );
        offsetShellParams.SetMergingEdges( TestVariables::mergeEdges );

        bool checkFacesConnection = false; // BUG_43154+BUG_43156+BUG_43165
        RPArray<MbFace> selectedFaces( 0, 1 );
        initSolid->GetFaces( selectedFaces );

        SetWaitCursor( true );

        // Create an open offset solid on the basis of the faces 'initFaces'
        res = ::OffsetShell( *initSolid, copyMode, selectedFaces, checkFacesConnection, offsetShellParams, 
                             operNames, copyAttrs, wrkSolid );
        SetWaitCursor( false );

        if ( res != rt_Success )
          ::DeleteItem( wrkSolid );
        if ( wrkSolid != nullptr ) {
          SetWaitCursor( true );

          // Create a thin-walled solid by exclusion of faces
          res = ::ThinSolid( *wrkSolid, cm_Same, outFaces, p, operNames, copyAttrs, resSolid );

          SetWaitCursor( false );
        }
      }
      else if ( wrkSolid != nullptr ) {
        SetWaitCursor( true );

        // Create a thin-walled solid by exclusion of faces
        res = ::ThinSolid( *wrkSolid, copyMode, outFaces, p, operNames, copyAttrs, resSolid );

        SetWaitCursor( false );
      }

      if ( wrkSolid != resSolid )
        ::DeleteItem( wrkSolid );

      if ( resSolid == nullptr ) {
        MessageBoxEx( RT_BODY_IS_NOT_BUILD, IDS_TITLE_ERR );
      }  
      else if ( copyMode == cm_Copy && res == rt_Success ) {
        TestVariables::viewManager->DeleteObject( obj );
        obj = nullptr;
      }

      if ( resSolid != nullptr ) {
        TestVariables::viewManager->AddObject( ns, resSolid );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание однотолщинного тонкостенного тела с удалением граней
// Create a thin-walled solid by exclusion of faces (single thin-walled value)
// ---
void CreateMonoThickThinSolid( bool closed )
{
  bool drawItems = true;

  if ( !closed )
    TestVariables::thickness = 0.0;

  if ( closed && ::fabs( TestVariables::thickness ) < EXTENT_EPSILON )
    TestVariables::thickness = 5;

  TCHAR msgTitleInfo[] = IDS_TITLE_MSG;
  TCHAR msgTitleError[] = IDS_TITLE_ERR;

  // Выбираем грани
  if ( !closed ) {
    TCHAR msgSelectFaces[] = RT_SELECT_FACES;
    MessageBoxEx( msgSelectFaces, msgTitleInfo );
  }
  else {
    TCHAR msgSelectFaces[] = RT_SELECT_OPENED_FACES;
    MessageBoxEx( msgSelectFaces, msgTitleInfo );
  }

  RPArray<MbFace> outFaces;

  MbSolid * initSolid = ::SelectShellFaces( outFaces, &TestVariables::itemPathFrom, drawItems );

  if ( initSolid != nullptr ) {
    if ( !closed || GetDouble( IDS_ENTER_THICKNESS, TestVariables::thickness )) {
      SweptValues params;
      params.shellClosed = closed;
      params.SetMergingFaces( TestVariables::mergeFaces );
      params.SetMergingEdges( TestVariables::mergeEdges );

      if ( params.shellClosed ) {
        if ( TestVariables::thickness < 0 ) {
          params.thickness1 = 0;
          params.thickness2 = -TestVariables::thickness;
        }
        else {
          params.thickness1 = TestVariables::thickness;
          params.thickness2 = 0;
        }
      }

      if ( !closed && initSolid != nullptr ) { // Инверсный выбор удаляемых граней
        int iInverse = 0;
        GetInt( IDS_ENTER_INVERSE, iInverse );

        if ( iInverse != 0 ) {
          ConstFacesSet delFaces;
          delFaces.insert( outFaces.begin(), outFaces.end() );

          RPArray<MbFace> invFaces( 0, 1 );
          initSolid->GetFaces( invFaces );
          for ( size_t i = invFaces.MaxIndex(); i--; ) {
            if ( delFaces.find( invFaces[i] ) != delFaces.end() )
              invFaces.erase( invFaces.begin() + i );
          }
          outFaces.clear();
          std::copy( invFaces.begin(), invFaces.end(), std::back_inserter( outFaces ) );
        }
      }

      MbSNameMaker operNames( ct_ShellSolid, MbSNameMaker::i_SideNone, 0 );
      operNames.SetVersion( MbNameVersion() );

      if ( DrawTestSolid::checkPhantom ) {
        MbFaceShell * phantom = nullptr;

        RPArray<MbFace> offFaces;
        SArray<double>  offDists;

        size_t hpShellFaceInd = SYS_MAX_T;

        SetWaitCursor( true );

        ::OffsetPhantom( *initSolid, outFaces, offFaces, offDists, params, operNames, phantom, &hpShellFaceInd );

        SetWaitCursor( false );

        if ( phantom != nullptr ) {
          TestVariables::drawImp->DrawItem( phantom, TRGB_WHITE );
          TestVariables::drawImp->DrawItem( phantom, TRGB_PURPLE );
          if ( hpShellFaceInd != SYS_MAX_T )
            TestVariables::drawImp->DrawItem( initSolid->GetShell()->GetFace( hpShellFaceInd ), TRGB_AZURE );
        }

        SetWaitCursor( true );

        ::SetPhantomNames( phantom, initSolid->GetItemName() ); // Отрисовать и проименовать фантом 

        SetWaitCursor( false );

        ::DeleteItem( phantom );
      }

      MbSolid * solid = nullptr;
      Style ns( initSolid->GetWidth(), initSolid->GetColor() );

      bool copyAttrs = TestVariables::copyAttributesOnDemand;

      if ( drawItems )
        TestVariables::viewManager->ToggleObject( initSolid );

      SetWaitCursor( true );

      // Create a thin-walled solid by exclusion of faces
      MbResultType res = ::ThinSolid( *initSolid, cm_Copy, outFaces, params, 
                                      operNames, copyAttrs, solid );
      SetWaitCursor( false );

      if ( solid == nullptr )
        ::PutErrorMessage( res, msgTitleError );

      if ( solid != nullptr ) {
        TestVariables::viewManager->DeleteObject( TestVariables::itemPathFrom.first );
        TestVariables::viewManager->AddObject( ns, solid );
      }
    }
    else {
      TestVariables::viewManager->ToggleObject( TestVariables::itemPathFrom.first );
    }
  }

}


//------------------------------------------------------------------------------
// Создание разнотолщинного тонкостенного тела с удалением граней
// Create a thin-walled solid by exclusion of faces (multiple thin-walled values)
// ---
void CreatePolyThickThinSolid()
{
  bool drawItems = true;

  TCHAR msgTitleInfo[] = IDS_TITLE_MSG;
  TCHAR msgTitleError[] = IDS_TITLE_ERR;

  // Выбираем вскрываемые грани
  TCHAR msgSelectOutFaces[] = RT_SELECT_OPENED_FACES;
  MessageBoxEx( msgSelectOutFaces, msgTitleInfo );

  RPArray<MbFace> outFaces( 10, 1 );

  PathMatrix pathFromInit;

  MbSolid * solidInit = ::SelectShellFaces( outFaces, &pathFromInit, drawItems );
  
  // Выбираем тело
  if ( solidInit != nullptr ) {
    if ( outFaces.size() < 1 ) { // Вскрываемые грани не выбраны
      TCHAR msgNoSelectedOutFaces[] = RT_OPENED_FACES_NOT_SELECTED;
      ::ShowString( msgNoSelectedOutFaces );
    }

    // Выбираем толщину оболочки
    if ( GetDouble( IDS_ENTER_THICKNESS, TestVariables::thickness )) {
      SweptValues params;
      params.thickness1 = 0.0;
      params.thickness2 = 0.0;
      params.shellClosed = solidInit->IsClosed();
      params.SetMergingFaces( TestVariables::mergeFaces );
      params.SetMergingEdges( TestVariables::mergeEdges );

      if ( params.shellClosed ) {
        if ( TestVariables::thickness < 0.0 ) {
          params.thickness1 = 0.0;
          params.thickness2 = -TestVariables::thickness;
        }
        else if ( TestVariables::thickness > 0.0 ) {
          params.thickness1 = TestVariables::thickness;
          params.thickness2 = 0.0;
        }
      }

      TCHAR msgSelectOffFaces[] = RT_SELECT_FACES_TO_MODIFY;
      MessageBoxEx( msgSelectOffFaces, msgTitleInfo );

      RPArray<MbFace> offFaces( 10, 1 );
      SArray<double>  offDists( 10, 1 );

      PathMatrix pathFromCur;
      MbItem * objCur = nullptr;
      MbTopItem * topItem = nullptr;

      while ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &objCur, &topItem, pathFromCur ) && (pathFromInit == pathFromCur) ) {
        MbFace * face = static_cast<MbFace *>(topItem);
        size_t indOff = c3d::DirectSearch( offFaces, face );
        size_t indOut = outFaces.size() ? c3d::DirectSearch(outFaces, face ) : SYS_MAX_T;

        bool bInOff = (indOff != SYS_MAX_T);
        bool bInOut = (indOut != SYS_MAX_T);

        if ( !bInOff && !bInOut ) { // Нет в обоих массивах
          
          // Выбираем толщину для конкретной грани
          double dist = TestVariables::thickness;
          GetDouble( IDS_ENTER_THICKNESS, dist );
          //if ( GetDouble( IDS_ENTER_THICKNESS, dist )) {
            //if ( dist * TestVariables::thickness <= 0 )
            //  dist = TestVariables::thickness;
          //}
          offFaces.push_back( face );
          offDists.push_back( dist );
          for ( auto & offFace_i : offFaces )
            TestVariables::drawImp->DrawItem( offFace_i, TRGB_LIGHTCYAN );
        }
        else if ( bInOff ) { // Есть в множестве модфицируемых
          offFaces.erase( offFaces.begin() + indOff );
          offDists.erase( offDists.begin() + indOff );
          TestVariables::drawImp->DrawItem( face, TRGB_LIGHTGREEN );
        }        
      }

      if ( offFaces.size() < 1 ) { // Модифицируемые грани не выбраны
        TCHAR msgNoSelectedOffFaces[] = RT_FACES_TO_MODIFY_NOT_SELECTED;
        ::ShowString( msgNoSelectedOffFaces );
      }

      if ( DrawTestSolid::checkPhantom ) {
        MbSNameMaker operNames( ct_ShellSolid, MbSNameMaker::i_SideNone, 0 );
        operNames.SetVersion( MbNameVersion() );

        MbFaceShell * phantom = nullptr;

        size_t hpShellFaceInd = SYS_MAX_T;

        SetWaitCursor( true );

        ::OffsetPhantom( *solidInit, outFaces, offFaces, offDists, params, operNames, phantom, &hpShellFaceInd );

        SetWaitCursor( false );

        if ( phantom != nullptr ) {
          TestVariables::drawImp->DrawItem( phantom, TRGB_WHITE );
          TestVariables::drawImp->DrawItem( phantom, TRGB_PURPLE );
          if ( hpShellFaceInd != SYS_MAX_T )
            TestVariables::drawImp->DrawItem( solidInit->GetShell()->GetFace( hpShellFaceInd ), TRGB_AZURE );
        }

        SetWaitCursor( true );

        ::SetPhantomNames( phantom, solidInit->GetItemName() ); // Отрисовать и проименовать фантом 

        SetWaitCursor( false );

        ::DeleteItem( phantom );
      }

      MbSNameMaker snameMaker( ct_ShellSolid, MbSNameMaker::i_SideNone, 0 );
      MbSolid * solidThin = nullptr;

      bool copyAttrs = TestVariables::copyAttributesOnDemand;

      SetWaitCursor( true );

      // Create a thin-walled solid by exclusion of faces
      MbResultType res = ::ThinSolid( *solidInit, cm_Copy, outFaces, offFaces, offDists, params, 
                                      snameMaker, copyAttrs, solidThin );
      SetWaitCursor( false );
                  
      if ( solidThin == nullptr )
        ::PutErrorMessage( res, msgTitleError );

      if ( solidThin != nullptr ) {
        Style objStyle( solidInit->GetWidth(), solidInit->GetColor() );
        TestVariables::viewManager->DeleteObject( pathFromInit.first );
        TestVariables::viewManager->AddObject( objStyle, solidThin );
      }  
    }    
  }

  if ( solidInit == nullptr ) {
    TCHAR msgNoSelectedSolid[] = RT_NOT_SELECTED_BODY;
    ::ShowString( msgNoSelectedSolid );
  }
}


//------------------------------------------------------------------------------
// Выбор тел.
// ---
static
void GetSolids( std::vector< std::pair<c3d::SolidSPtr, MbMatrix3D> > & selItems )
{
  { // Select solids.
    MbItem * item = nullptr;
    PathMatrix pathFrom;

    while ( GetGObj( IDS_SELECT_SOLID, st_Solid, &item, pathFrom ) ) {
      if ( item->IsA() == st_Solid ) {
        std::pair<c3d::SolidSPtr, MbMatrix3D> selItem( c3d::SolidSPtr( static_cast<MbSolid *>( item ) ), pathFrom.second );

        std::vector< std::pair<c3d::SolidSPtr, MbMatrix3D> >::iterator it = std::find( selItems.begin(), selItems.end(), selItem );

        TestVariables::viewManager->ToggleObject( item );

        if ( it == selItems.end() )
          selItems.push_back( selItem );
        else
          selItems.erase( it );
      }
    }
  }

  if ( selItems.size() < 1 ) { // SD#7116738
    RPArray<MbItem> allSelectedItems;
    MbMatrix3D dummyMtr;
    TestVariables::viewManager->GetSelectedObjects( allSelectedItems );
    if ( allSelectedItems.size() > 0 ) {
      for ( size_t k = 0; k < allSelectedItems.size(); ++k ) {
        MbItem * item = allSelectedItems[k];
        if ( item->IsA() == st_Solid ) {
          std::pair<c3d::SolidSPtr, MbMatrix3D> selItem( c3d::SolidSPtr( static_cast<MbSolid *>( item ) ), dummyMtr );
          selItems.push_back( selItem );
        }
      }
    }
  }
}


//#define UNION_SOLIDS_MULTITHREADED
#ifndef UNION_SOLIDS_MULTITHREADED
//------------------------------------------------------------------------------
// Создание составного тела.
// Create a solid with or without union of the intersecting solids.
// ---
void CreateUnionSolids( OperationType opType )
{
  Style ns = TestVariables::SOLID_Style;

  std::vector< std::pair<c3d::SolidSPtr, MbMatrix3D> > selItems;

  { // Select solids.
    GetSolids( selItems );
  }

  if ( selItems.size() > 0 ) {
    RPArray<MbSolid> wrkSolids( selItems.size(), 1 );
    {
      for ( size_t k = 0, kcnt = selItems.size(); k < kcnt; k++ ) {
        if ( selItems[k].first != nullptr ) {
          ::SetWaitCursor( true );
          MbSolid * wrkSolid = static_cast<MbSolid *>( &selItems[k].first->Duplicate() );
          if ( !selItems[k].second.IsSingle() )
            wrkSolid->Transform( selItems[k].second );
          ::SetWaitCursor( false );
          wrkSolids.push_back( wrkSolid );
          ::AddRefItem( wrkSolid );
        }
      }
    }

    c3d::SolidSPtr resSolid;
    MbAccurateTimer ac;

    switch ( opType ) {
    case bo_Union: // Union
    case bo_Difference: // Difference
    case bo_Intersect: // Intersection
    {
      MbItem * item0 = nullptr;
      PathMatrix pathFrom0;

      if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &item0, pathFrom0 ) ) {
        TestVariables::viewManager->ToggleObject( item0 );

        ns.Init( item0->GetWidth(), item0->GetColor() );

        ::SetWaitCursor( true );
        c3d::SolidSPtr wrkSolid0( static_cast<MbSolid *>( &static_cast<MbSolid *>( item0 )->Duplicate() ) );
        if ( !pathFrom0.second.IsSingle() ) {
          MbSolid * result0 = nullptr;
          MbSNameMaker tn;
          tn.SetMainName( ct_TransformedSolid );
          TransformValues tv( pathFrom0.second );
          ::TransformedSolid( *wrkSolid0, cm_Same, tv, tn, result0 );
          if ( result0 != nullptr ) {
            wrkSolid0 = result0;
          }
        }
        ::SetWaitCursor( false );

        bool checkIntersection = true;
        MbSNameMaker operNames( ct_UnionSolid, MbSNameMaker::i_SideNone, 0 );
        bool isArray = false;

        MbResultType resType = rt_Error;

        MbeCopyMode copyMode0 = cm_Same;

        //#define CHECK_INDEPENDENCE // Проверка SD#788768

#ifdef CHECK_INDEPENDENCE
        copyMode0 = cm_Copy;
        MbCube cube0;
        if ( copyMode0 == cm_Copy )
          wrkSolid0->AddYourGabaritTo( cube0 );
#endif
        MbeCopyMode copyMode = copyMode0;

        ::SetWaitCursor( true );
        {
          MbSolid * unionSolid = nullptr;

          MbMergingFlags mergeFlags( TestVariables::mergeFaces, TestVariables::mergeEdges );

          ac.Begin();
          resType = ::UnionResult( wrkSolid0, copyMode0, wrkSolids, copyMode, opType,
                                   checkIntersection, mergeFlags, operNames, isArray,
                                   unionSolid );
          ac.End();
          resSolid = unionSolid;

#ifdef CHECK_INDEPENDENCE
          if ( copyMode0 == cm_Copy ) {
            MbMatrix3D matr;
            matr.Rotate( 0, 1.0 );
            matr.Rotate( 1, 2.0 );
            matr.Rotate( 2, 3.0 );
            matr.Move( 1000.0, 2000.0, 3000.0 );
            resSolid->Transform( matr );
            MbCube cube1;
            wrkSolid0->AddYourGabaritTo( cube1 );
            PRECONDITION( cube0 == cube1 );
            MbMatrix3D divMatr;
            matr.Div( divMatr );
            resSolid->Transform( divMatr );
          }
#endif
        }
        ::SetWaitCursor( false );

        if ( resSolid == nullptr )
          ::PutErrorMessage( resType, IDS_TITLE_ERR );

        if ( resSolid != nullptr ) {
          TestVariables::viewManager->ToggleObject( item0 );
          if ( copyMode0 != cm_Copy )
            TestVariables::viewManager->DeleteObject( item0 );
        }
      }
    } break;

    default: {
      bool checkIntersection = true;
      { // C3D-1246 : KOMPAS-26716
        int iCheck = 1;
        GetInt( IDS_ENTER_CHECKINT, iCheck );
        checkIntersection = ( iCheck > 0 );
      }
      MbSNameMaker operNames( ct_UnionSolid, MbSNameMaker::i_SideNone, 0 );
      bool isArray = false;

      RPArray<MbSolid> notGluedSolids;

      ::SetWaitCursor( true );
      {
        MbSolid * unionSolid = nullptr;
        ac.Begin();
        if ( checkIntersection )
          ::UnionSolid( wrkSolids, cm_Same, checkIntersection, operNames, isArray, unionSolid, &notGluedSolids );
        else
          ::UnionSolid( wrkSolids, operNames, unionSolid );
        ac.End();

        resSolid = unionSolid;
      }
      ::SetWaitCursor( false );

      ::DeleteItems( notGluedSolids );

    } break;
    }

    ::ReleaseItems( wrkSolids );

    if ( resSolid != nullptr ) {
      for ( size_t i = selItems.size(); i--; ) {
        TestVariables::viewManager->DeleteObject( selItems[i].first );
      }
      TestVariables::viewManager->AddObject( ns, resSolid );
    }
    else {
      MessageBoxEx( RT_COMPOSITE_BODY_NOT_CREATED, IDS_TITLE_ERR );
    }

    if ( TestVariables::checkOperationTime ) {
      const size_t bufLen = 1024;
      TCHAR buf[bufLen];
      size_t len = 0;
      len += (size_t)_sntprintf( buf + len, bufLen - len, RT_BOOLEAN_SOLID );
      len += (size_t)_sntprintf( buf + len, bufLen - len, _T( "\n\n" ) );
      len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, ac.GetLast() );
      ::MessageBoxEx( buf, IDS_RESULTS );
    }
  }
  else {
    MessageBoxEx( RT_NOT_SELECTED_BODY, IDS_TITLE_ERR );
  }
}
#else

//------------------------------------------------------------------------------
// Тест на многопоточное создание составных тел.
// Create solids with or without union of the intersecting solids.
// ---
void CreateUnionSolids( OperationType opType )
{
  Style ns = TestVariables::SOLID_Style;

  std::vector< std::pair<c3d::SolidSPtr, MbMatrix3D> > selItems, selItems2;
  GetSolids( selItems );
  GetSolids( selItems2 );

  Math::SetSupressCreators( false );

  if ( selItems.size() > 0 && selItems2.size() > 0 ) {
    RPArray<MbSolid> firstSolids;
    for ( size_t k = 0, kcnt = selItems.size(); k < kcnt; k++ ) {
      if ( selItems[k].first != nullptr ) {
        ::SetWaitCursor( true );
        MbSolid * wrkSolid0 = selItems[k].first;
        if ( !selItems[k].second.IsSingle() ) {
          wrkSolid0->Transform( selItems2[k].second );
        }
        ::SetWaitCursor( false );
        firstSolids.push_back( wrkSolid0 );
        ::AddRefItem( wrkSolid0 );
      }
    }

    RPArray<MbSolid> wrkSolids2( selItems2.size(), 1 );
    for ( size_t k = 0, kcnt = selItems2.size(); k < kcnt; k++ ) {
      if ( selItems2[k].first != nullptr ) {
        ::SetWaitCursor( true );
        MbSolid * wrkSolid = selItems2[k].first; // static_cast<MbSolid *>( &selItems2[k].first->Duplicate() );
        if ( !selItems2[k].second.IsSingle() )
          wrkSolid->Transform( selItems2[k].second );
        ::SetWaitCursor( false );
        wrkSolids2.push_back( wrkSolid );
        ::AddRefItem( wrkSolid );
      }
    }

    c3d::SolidsSPtrVector resSolids;

    switch ( opType ) {
      case bo_Union      : // Union
      case bo_Difference : // Difference
      case bo_Intersect  : // Intersection
      {
        {
          bool checkIntersection = true;
          MbSNameMaker operNames( ct_UnionSolid, MbSNameMaker::i_SideNone, 0 );
          bool isArray = false;
          MbeCopyMode copyMode0 = cm_Copy;
          MbeCopyMode copyMode = copyMode0;
          MbMergingFlags mergeFlags( TestVariables::mergeFaces, TestVariables::mergeEdges );

          ptrdiff_t sCount = firstSolids.size();
          resSolids.resize( sCount );
          bool useParallel = sCount > 1 && Math::CheckMultithreadedMode( mtm_Items );

          ::SetWaitCursor( true );

          ENTER_PARALLEL( useParallel );
#pragma omp parallel for schedule(dynamic) if(useParallel)
          for ( ptrdiff_t i = 0; i < sCount; i++ ) {
            MbSolid * unionSolid = nullptr;
            ::UnionResult( firstSolids[i], copyMode0, wrkSolids2, copyMode, opType,
                                     checkIntersection, mergeFlags, operNames, isArray,
                                     unionSolid );
            resSolids[i] = unionSolid;
          }
          EXIT_PARALLEL( useParallel );
          ::SetWaitCursor( false );

          bool result = true;
          for ( auto & resSolid : resSolids )
            if ( resSolid == nullptr )
              result = false;
          if ( !result )
            MessageBoxEx( RT_COMPOSITE_BODY_NOT_CREATED, IDS_TITLE_ERR );
          else {
            for ( auto & resSolid : resSolids ) {
              TestVariables::viewManager->AddObject( ns, resSolid.get() );
              TestVariables::viewManager->ToggleObject( resSolid.get() );
            }
            for ( size_t i = selItems.size(); i--; ) {
              TestVariables::viewManager->DeleteObject( selItems[i].first );
            }
            for ( size_t i = selItems2.size(); i--; ) {
              TestVariables::viewManager->DeleteObject( selItems2[i].first );
            }
          }
        }
      } break;

      default : {
      } break;
    }

    ::ReleaseItems( firstSolids );
    ::ReleaseItems( wrkSolids2 );
    TestVariables::viewManager->RefreshModel();
  }
  else {
    MessageBoxEx( RT_NOT_SELECTED_BODY, IDS_TITLE_ERR );
  }
}
#endif


//------------------------------------------------------------------------------
// Создание тела с фаской ребра.
// Create chamfers for edges of the solid.
// ---
void CreateSolidChamfer( int form )
{
  MbItem  * obj( nullptr ), * obj0( nullptr );
  SolidSPtr solid;
  MbTopItem * curve( nullptr );
  MbTopItem * surface( nullptr );
  Style ns;
  c3d::EdgesSPtrVector initCurves;
  MbCurveEdge * edge( nullptr );
  MbMatrix3D matr;
  WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();

  if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &curve, matr ) ) {
    TestVariables::viewManager->ToggleObject(obj);
//#ifdef _DRAWGI
//    drawImp->DrawItem( curve, TRGB_WHITE );
//#endif
    if ( currentView3D != nullptr )
      currentView3D->PutObjectPart( obj, curve, true );
    initCurves.push_back( EdgeSPtr(static_cast<MbCurveEdge *>(curve)) );
    while ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj0, &curve ) ) {
      if (obj==obj0) {
//#ifdef _DRAWGI
//        drawImp->DrawItem( curve, TRGB_WHITE );
//#endif
        if ( currentView3D != nullptr )
          currentView3D->PutObjectPart( obj, curve, true );
        edge = static_cast<MbCurveEdge *>( curve );
        auto itr = std::find( initCurves.begin(), initCurves.end(), edge );
        if (itr != initCurves.end() ) {
//#ifdef _DRAWGI
//          drawImp->DrawItem( curve, TRGB_LIGHTGREEN );
//#endif
          if ( currentView3D != nullptr )
            currentView3D->PutObjectPart( obj, curve, false );
          initCurves.erase( itr );
        }
        else
          initCurves.push_back( EdgeSPtr(edge) );
      }
    }
    TestVariables::viewManager->ToggleObject(obj);
  }
  else {
    if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &surface, matr ) ) {
      TestVariables::viewManager->ToggleObject(obj);
//#ifdef _DRAWGI
//      drawImp->DrawItem( surface, TRGB_WHITE );
//#endif
      if ( currentView3D != nullptr )
        currentView3D->PutObjectPart( obj, surface, true );
      ((MbFace *)surface)->GetEdges( initCurves );
      while ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj0, &surface ) ) {
        if (obj==obj0) {
//#ifdef _DRAWGI
//          drawImp->DrawItem( surface, TRGB_WHITE );
//#endif
          if ( currentView3D != nullptr )
            currentView3D->PutObjectPart( obj, surface, true );
          ((MbFace *)surface)->GetEdges( initCurves );
        }
      }
      TestVariables::viewManager->ToggleObject(obj);
    }
  }

  if ( initCurves.size() > 0 ) {
    bool res = false;

    if ( form == st_Slant1 ) { // Фаска по катету и углу 
      double angl = ::atan2( TestVariables::distance2, TestVariables::distance1 );
      if ( GetDouble( IDS_ENTER_SLANT, TestVariables::distance1 ) ) {
        angl *= 180/M_PI;
        res = GetDouble( IDS_ENTER_SLOPE_ANGLE, angl );
        TestVariables::distance2 = angl;
      }
    }
    else
    if ( form == st_Slant2 ) { // Фаска по углу и катету
      double angl = ::atan2( TestVariables::distance1, TestVariables::distance2 );
      angl *= 180/M_PI;
      if ( GetDouble( IDS_ENTER_SLOPE_ANGLE, angl ) ) {
        TestVariables::distance1 = angl;
        res = GetDouble( IDS_ENTER_SLANT, TestVariables::distance2 );
      }
    }
    else { // Фаска по двум катетам
      if ( GetDouble( IDS_ENTER_FIRST_SRF_DST, TestVariables::distance1 ) ) {
        if ( ::fabs(TestVariables::distance2 - 1.0) < EPSILON )
          TestVariables::distance2 = TestVariables::distance1;
        res = GetDouble( IDS_ENTER_SECOND_SRF_DST, TestVariables::distance2 );
      }
    }
    if ( res && 
         GetInt( IDS_TANGENT_PROLONG, TestVariables::prolong ) && 
         GetInt( IDS_ENTER_KEEP_EDGE, TestVariables::keepKerb ) ) {
      ns.Init( obj->GetWidth(), obj->GetColor() );
      SetWaitCursor( true );
      MbSNameMaker n( ct_ChamferSolid, MbSNameMaker::i_SideNone, 0 );
      SmoothValues params;
      if ( form == st_Chamfer )
        params.InitChamfer( TestVariables::distance1, TestVariables::distance2 );
      else if ( form == st_Slant1 )
        params.InitOneSideChamfer( true, TestVariables::distance1, TestVariables::distance2 );
      else if ( form == st_Slant2 )
        params.InitOneSideChamfer( false, TestVariables::distance2, TestVariables::distance1 );
      params.SetProperties( _ARC_, TestVariables::prolong!=0,
                           SmoothValues::ec_uniform,//SmoothValues::ec_pointed, 
                           TestVariables::keepKerb >= 0, TestVariables::keepKerb > 0, true, false/*equable*/ ); 

      if ( DrawTestSolid::checkPhantom ) { // Create a phantom
        ConstSolidSPtr solidPtr( static_cast<MbSolid *>(obj) );
        MbSmoothPhantomResult result;
        ::SmoothPhantom( MbSmoothPhantomParams (solidPtr, initCurves, params), result );
        for ( size_t i = 0, count = result.GetSurfaces().size(); i < count; i++ ) 
          TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, result.GetSurfaces()[i] );
        ::DetachItem( solidPtr );
      }
      if ( DrawTestSolid::checkPosition ) {
        MbSmoothPositionDataResults data;
        c3d::ConstSolidSPtr solidPtr( static_cast<MbSolid *>( obj ) );
        MbSmoothPositionDataParams posParams( solidPtr, initCurves, params );
        ::SmoothPositionData( posParams, data );
        ::DetachItem( solidPtr );
        for ( size_t j = 0, jCount = data.GetPositionData().size(); j < jCount; j++ ) {
          MbPositionData * dat = data.SetPositionData()[j];
          if ( dat != nullptr ) {
            TestVariables::drawImp->DrawPoint( &dat->point1, TRGB_BLUE );
            TestVariables::drawImp->DrawPoint( &dat->point2, TRGB_BLUE );
            TestVariables::drawImp->DrawPoint( &dat->origin, TRGB_BLACK );
          }
          ::DeleteMatItem( dat );
        }
      }

      // Построение фаски
      MbResultType resType = ::ChamferSolid( *(MbSolid *)obj, cm_Copy, MbShellChamferParams( initCurves, params, n), solid );

//      for (uint i = 0, count = surfaces.Count(); i < count; i++) 
//        viewManager->AddObject( SURFACECURVE_Style, surfaces[i] );

      if ( solid != nullptr ) {
        TestVariables::viewManager->DeleteObject( obj );
        if ( !matr.IsSingle() )
          solid->Transform( matr );
        TestVariables::viewManager->AddObject( ns, solid.detach() );
      }
      else {
        ::PutErrorMessage( resType, IDS_TITLE_ERR );
      }
      SetWaitCursor( false );
    }
  }
} // CreateSolidChamfer


//------------------------------------------------------------------------------
// Создание тела со скруглением ребра.
// Fillet edges with a constant radius.
// ---
void CreateSolidFillet( int form )
{
  MbItem * obj( nullptr ), * obj0( nullptr );
  MbTopItem * curve( nullptr );
  MbTopItem * surface( nullptr );
  Style ns;
  c3d::EdgesSPtrVector initCurves;
  MbCurveEdge * edge( nullptr );
  MbMatrix3D matr;
  WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();

  if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &curve, matr ) ) {
//    viewManager->ToggleObject(obj);
//#ifdef _DRAWGI
//    drawImp->DrawItem( curve, TRGB_WHITE );
//#endif
    if ( currentView3D != nullptr )
      currentView3D->PutObjectPart( obj, curve, true );
    initCurves.push_back( EdgeSPtr(static_cast<MbCurveEdge *>(curve)) );
    while ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj0, &curve, TestVariables::itemPathFrom.second ) ) {
      if ( obj == obj0 ) {
//#ifdef _DRAWGI
//        drawImp->DrawItem( curve, TRGB_WHITE );
//#endif
        if ( currentView3D != nullptr )
          currentView3D->PutObjectPart( obj, curve, true );
        edge = (MbCurveEdge *)curve;
        auto itr = std::find( initCurves.begin(), initCurves.end(), edge );
        if ( itr != initCurves.end() ) {
          //#ifdef _DRAWGI
//          drawImp->DrawItem( curve, TRGB_LIGHTGREEN );
//#endif
          if ( currentView3D != nullptr )
            currentView3D->PutObjectPart( obj, curve, false );
          initCurves.erase( itr );
        }
        else
          initCurves.push_back( EdgeSPtr(edge) );
      }
    }
//    viewManager->ToggleObject(obj);
  }
  else { // Выбрать все ребра указанных граней.
    if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &surface, matr ) ) {
//      viewManager->ToggleObject(obj);
//#ifdef _DRAWGI
//      drawImp->DrawItem( surface, TRGB_WHITE );
//#endif
      if ( currentView3D != nullptr )
        currentView3D->PutObjectPart( obj, surface, false );
      ((MbFace *)surface)->GetEdges( initCurves );
      while ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj0, &surface, TestVariables::itemPathFrom.second ) ) {
        if ( obj==obj0 ) {
//#ifdef _DRAWGI
//          drawImp->DrawItem( surface, TRGB_WHITE );
//#endif
          if ( currentView3D != nullptr )
            currentView3D->PutObjectPart( obj, surface, false );
          ((MbFace *)surface)->GetEdges( initCurves );
        }
      }
//      viewManager->ToggleObject(obj);
      while ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj0, &curve, TestVariables::itemPathFrom.second ) ) {
        if ( obj == obj0 ) {
          if ( currentView3D != nullptr )
            currentView3D->PutObjectPart( obj, curve, true );
          edge = (MbCurveEdge *)curve;
          auto itr = std::find( initCurves.begin(), initCurves.end(), edge );
          if ( itr != initCurves.end() ) {
            if ( currentView3D != nullptr )
              currentView3D->PutObjectPart( obj, curve, false );
            initCurves.erase( itr );
          }
          else
            initCurves.push_back( EdgeSPtr(edge) );
        }
      }
    }
    else { // Выбрать все ребра, равные указанному по длине, радиусу, выпуклости. // BUG_95819
      if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &curve, TestVariables::itemPathFrom.second ) ) {
        if ( obj->IsA() == st_Solid && curve->IsA() == tt_CurveEdge ) {
          obj0 = obj;
          MbSolid * solid = (MbSolid *)obj;
          MbCurveEdge * cedge = (MbCurveEdge *)curve;
          initCurves.push_back( EdgeSPtr(cedge) );
          double l, len = cedge->GetLengthEvaluation();
          double r, rad = cedge->GetCurve().GetRadius();
          bool s, isStraight = cedge->GetCurve().IsStraight();
          ThreeStates c, convex = cedge->IsConvex();
          RPArray<MbCurveEdge> edges( 0, 1 );
          solid->GetEdges( edges );
          for ( size_t i = 0, iCount = edges.size(); i < iCount; i++ ) {
            cedge = edges[i];
            l = cedge->GetLengthEvaluation();
            r = cedge->GetCurve().GetRadius();
            s = cedge->GetCurve().IsStraight();
            c = cedge->IsConvex();
            if ( (::fabs(l - len) < METRIC_REGION) &&
                 (::fabs(r - rad) < METRIC_REGION) &&
                 (s == isStraight) &&
                 (c == convex) )
              initCurves.push_back( EdgeSPtr(cedge) );
          }
        }
      }
    }
  }

  if ( initCurves.size() > 0 ) {
    bool res = false;

    if ( form == st_Span ) { // Скругление с заданной хордой     
      res = GetDouble( IDS_ENTER_SPAN, TestVariables::radius1 );
      TestVariables::radius2 = TestVariables::radius1;
    }
    else {
      if ( GetDouble( IDS_ENTER_RADIUS, TestVariables::radius1 ) ) {
        TestVariables::radius2 = TestVariables::radius1;
        res = GetDouble( IDS_ENTER_RADIUS, TestVariables::radius2 );
      }
    }
    double begLength = -1.0;
    double endLength = -1.0;
    if ( res &&
         GetDouble( IDS_SELECT_CONIC, TestVariables::conic ) &&
         GetInt( IDS_TANGENT_PROLONG, TestVariables::prolong ) &&
         GetInt( IDS_ENTER_KEEP_EDGE, TestVariables::keepKerb ) ) {

      c3d::FacesSPtrVector initBounds; // Грани для обрезки краёв скруглений.
      MbFace * bound1( nullptr ); // Внешняя грань остановки скругления в начале.
      MbFace * bound2( nullptr ); // Внешняя грань остановки скругления в конце.

      if ( GetDouble(IDS_BEG_FILLET_LENGTH, begLength) ) {
        endLength = begLength;
        GetDouble( IDS_END_FILLET_LENGTH, endLength );

        int addStopFace = 0;
        GetInt( IDS_FILLET_STOP_FACES, addStopFace );

        if ( addStopFace == 1 ) {
          MbTopItem * top( nullptr );
          if ( GetTObj(IDS_SELECT_FACE, st_Solid, tt_Face, &obj0, &top, TestVariables::itemPathFrom.second) ) {
            if ( top->IsA() == tt_Face && obj != obj0 ) {
              TestVariables::drawImp->DrawItem( top, TRGB_WHITE );
              bound1 = static_cast<MbFace *>( top );
            }
          }
          if ( GetTObj(IDS_SELECT_FACE, st_Solid, tt_Face, &obj0, &top, TestVariables::itemPathFrom.second) ) {
            if ( top->IsA() == tt_Face && obj != obj0 ) {
              TestVariables::drawImp->DrawItem( top, TRGB_WHITE );
              bound2 = static_cast<MbFace *>( top );
            }
          }
        }

        addStopFace = 0;
        GetInt( IDS_FILLET_VERTEX_FACES, addStopFace );
        if ( addStopFace == 1 ) {
          MbTopItem * top( nullptr );
          if ( GetTObj(IDS_SELECT_FACE, st_Solid, tt_Face, &obj0, &top, TestVariables::itemPathFrom.second) ) {
            if ( top->IsA() == tt_Face ) {
              MbFace * face = static_cast<MbFace *>( top );
              initBounds.push_back( FaceSPtr(face) );
            }
            while ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj0, &top, TestVariables::itemPathFrom.second ) ) {
              if ( top->IsA() == tt_Face ) {
                MbFace * face = static_cast<MbFace *>( top );
//#ifdef _DRAWGI
                TestVariables::drawImp->DrawItem( top, TRGB_WHITE );
//#endif
                if ( std::find(initBounds.begin(), initBounds.end(), face) == initBounds.end() ) // Если еще не добавляли такую грань,
                  initBounds.push_back( FaceSPtr(face) ); // то добавить.
              }
            }
          }
        }
      }
      SetWaitCursor( true );

      ns.Init( obj->GetWidth(), obj->GetColor() );
      MbSNameMaker n( ct_FilletSolid, MbSNameMaker::i_SideNone, 0 );
      SmoothValues::CornerForm cForm = SmoothValues::ec_uniform; 
//      SmoothValues::CornerForm cForm = SmoothValues::ec_sharp; 
//      SmoothValues::CornerForm cForm = SmoothValues::ec_pointed; 
      SmoothValues params;
      if ( form == st_Fillet )
        params.InitEquidistantFillet( TestVariables::radius1, TestVariables::radius2 );
      else if ( form == st_Ellipse )
        params.InitEllipseFillet( TestVariables::radius1, TestVariables::radius2 );
      else if ( form == st_Span )
        params.InitChordFillet( TestVariables::radius1 );
      params.SetProperties( TestVariables::conic, TestVariables::prolong != 0,
                           cForm, TestVariables::keepKerb >= 0, TestVariables::keepKerb > 0, true, false/*equable*/ );
//      params.equable = true; // Плавный переход через угол
      if ( begLength >= 0.0 )
        params.SetBegLength() = begLength;
      if ( endLength >= 0.0 )
        params.SetEndLength() = endLength;
      if ( bound1 != nullptr ) {
        MbSurface * surf1 = static_cast<MbSurface *>( &bound1->GetSurface().Duplicate() );
        params.SetStopObjectAtBeg( surf1, true );
      }
      if ( bound2 != nullptr ) {
        MbSurface * surf2 = static_cast<MbSurface *>( &bound2->GetSurface().Duplicate() );
        params.SetStopObjectAtEnd( surf2, true );
      }

      if ( DrawTestSolid::checkPhantom ) {
        // Фантом поверхностей.
        ConstSolidSPtr solidPtr( static_cast<MbSolid *>( obj ) );
        MbSmoothPhantomResult result;
        ::SmoothPhantom( MbSmoothPhantomParams (solidPtr, initCurves, params), result );
        for ( size_t i = 0, count = result.GetSurfaces().size(); i < count; i++ ) {
          TestVariables::drawImp->DrawItem( result.GetSurfaces()[i], TRGB_BLUE );
          TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, result.GetSurfaces()[i] );
        }
        ::DetachItem( solidPtr );
      }
      if ( DrawTestSolid::checkPosition ) {
        { // Позиция размера.
          MbSmoothPositionDataResults data;
          c3d::ConstSolidSPtr solidPtr( static_cast<MbSolid *>( obj ) );
          MbSmoothPositionDataParams posParams( solidPtr, initCurves, params );
          ::SmoothPositionData( posParams, data );
          ::DetachItem( solidPtr );
          for ( size_t j = 0, jCount = data.GetPositionData().size(); j < jCount; j++ ) {
            MbPositionData * dat = data.SetPositionData()[j];
            if ( dat != nullptr ) {
              TestVariables::drawImp->DrawPoint( &dat->point1, TRGB_BLUE );
              TestVariables::drawImp->DrawPoint( &dat->point2, TRGB_BLUE );
              TestVariables::drawImp->DrawPoint( &dat->origin, TRGB_BLACK );
            }
            ::DeleteMatItem( dat );
          }
        }
        { // Последовательность ребер и фантом поверхностей.
          bool createSurfaces = true;
          ConstSolidSPtr solidPtr( static_cast<MbSolid *>( obj ) );
          MbSmoothSequenceResult result;

          ::SmoothSequence( MbSmoothSequenceParams( solidPtr, initCurves, params, createSurfaces ), result );
          ::DetachItem( solidPtr );

          MbCartPoint3D p;
          for ( size_t j = 0, jCount = result.GetSequences().size(); j < jCount; j++ ) {
            const MbEdgeSequence & seq = result.GetSequences()[j];
            for ( size_t i = 0, iCount = seq.edges.Count(); i < iCount; i++ ) {
              const MbCurveEdge * seqEdge = seq.edges[i];
              bool s = seq.sense[i];
              TestVariables::drawImp->DrawItem( seqEdge, TRGB_PURPLE );
              seqEdge->Point( s ? 0.9 : 0.1, p );
              TestVariables::drawImp->DrawPoint( &p, TRGB_BLUE );
              seqEdge->Point( 0.95, p );
              TestVariables::drawImp->DrawPoint( &p, TRGB_RED );
            }
          }
          for ( size_t i = 0, iCount = result.GetSurfaces().size(); i < iCount; i++ ) {
            TestVariables::drawImp->DrawItem( result.GetSurfaces()[i], TRGB_BLUE );
            if ( !DrawTestSolid::checkPhantom )
              TestVariables::viewManager->AddObject( TestVariables::SHELL_Style, result.GetSurfaces()[i] );
            //::DeleteItem( surfaces[i] );
          }
        }
      }

      c3d::SolidSPtr solid;
      IProgressIndicator * progBar = &::CreateProgressIndicator( new StrSpyCommon );

      MbResultType resType = ::FilletSolid( *(MbSolid *)obj, cm_Copy,
                                            MbShellFilletValues(initCurves, initBounds, params, n),
                                            solid, progBar );
      if ( progBar != nullptr ) {
        delete progBar;
        progBar = nullptr;
      }

      if ( solid != nullptr ) {
        TestVariables::viewManager->DeleteObject( obj );
        if ( !matr.IsSingle() )
          solid->Transform( matr );
        TestVariables::viewManager->AddObject( ns, solid.detach() );
      }
      else {
        ::PutErrorMessage( resType, IDS_TITLE_ERR );
      }
      SetWaitCursor( false );
    }
  }
} // CreateSolidFillet


//------------------------------------------------------------------------------
// Создание тела со скруглением ребра переменного радиуса
// Fillet edges with a variable radius.
// ---
void CreateSolidChannel( int form )
{
  MbItem  * obj( nullptr ), * obj0( nullptr );
  MbTopItem * curve( nullptr );
  Style ns;
  RPArray<MbCurveEdge> initCurves( 4, 1 );         //-V112
  RPArray<MbFace> initBounds( 0, 1 ); 
  MbCurveEdge * edge( nullptr );
  MbMatrix3D matr;
  WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();

  if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &curve, matr ) ) {
    TestVariables::viewManager->ToggleObject(obj);
//#ifdef _DRAWGI
//    drawImp->DrawItem( curve, TRGB_WHITE );
//#endif
    if ( currentView3D != nullptr )
      currentView3D->PutObjectPart( obj, curve, true );
    initCurves.Add( (MbCurveEdge *)curve );
    while ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj0, &curve, TestVariables::itemPathFrom.second ) ) {
      if (obj==obj0) {
//#ifdef _DRAWGI
//        drawImp->DrawItem( curve, TRGB_WHITE );
//#endif
        if ( currentView3D != nullptr )
          currentView3D->PutObjectPart( obj, curve, true );
        edge = (MbCurveEdge *)curve;
        size_t const i0 = initCurves.FindIt(edge);
        if (i0 != SYS_MAX_T) {
//#ifdef _DRAWGI
//          drawImp->DrawItem( curve, TRGB_LIGHTGREEN );
//#endif
          if ( currentView3D != nullptr )
            currentView3D->PutObjectPart( obj, curve, false );
          initCurves.DetachInd( i0 );
        }
        else {
          initCurves.Add( edge );
        }
      }
    }
    TestVariables::viewManager->ToggleObject(obj);
  }

  if (initCurves.Count()>0) {
    if ( GetDouble( IDS_ENTER_RADIUS, TestVariables::radius1 ) ) {
      TestVariables::radius2 = TestVariables::radius1;
      if ( GetDouble( IDS_ENTER_RADIUS, TestVariables::radius2 ) && GetInt( IDS_TANGENT_PROLONG, TestVariables::prolong ) && GetInt( IDS_ENTER_KEEP_EDGE, TestVariables::keepKerb ) ) {

        ns.Init( obj->GetWidth(), obj->GetColor() );
        SetWaitCursor( true );
        MbSNameMaker n( ct_FilletSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );

        size_t i, initCount = initCurves.Count();
        SArray <MbEdgeFunction> initEdgeFunctions(initCount,4);         //-V112
        SArray <double> params(5,2);
        SArray <double> values(5,2);
        double radius = 0.5*(fabs(TestVariables::radius1)+fabs(TestVariables::radius2));

        for (i=0;i<initCount;i++) {
          edge = initCurves[i];

          // Отрисовка
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawItem( edge, TRGB_LIGHTRED );
          TestVariables::drawImp->DrawItem( edge->GetBegVertexPointer(), TRGB_LIGHTRED );
//#endif

          double par = 0;
          double rad = radius;
          if ( GetDouble( IDS_ENTER_PARAMETER, par ) && GetDouble( IDS_ENTER_RADIUS, rad ) ) {
            params.Flush();
            values.Flush();
            params.Add(par);
            values.Add(rad/radius);
            par = std_max( 1.0, par + 0.1 );
            while ( GetDouble( IDS_ENTER_PARAMETER, par ) && GetDouble( IDS_ENTER_RADIUS, rad ) ) {
              params.Add(par);
              values.Add(rad/radius);
              if ( par > 1.0 - PARAM_EPSILON )
                break;

              par = std_max( 1.0, par + 0.1 );
            }
          }
          // Сортировка по возрастанию параметра по первому множеству
          ::SortSynchroArrays ( params, values, PARAM_ACCURACY, true );
          while ( values.Count() < 2 ) {
            size_t count = params.Count();
            if ( count > 0 )
              par = params[count-1];
            params.Add(par+1.0);
            values.Add(1.0);
          }

          MbFunction * function( nullptr );

          if ( values.size() == 2 && params.size() == 2 ) {
            function = new MbCubicFunction( values[0], 0.0, params[0], values[1], 0.0, params[1] );
          }
          else {
            function = new MbCubicFunction(values, params, edge->GetCurve().IsClosed() );
          }
          
          initEdgeFunctions.Add( MbEdgeFunction(edge, function) );

          // Отрисовка
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawItem( edge, TRGB_LIGHTGREEN );
          TestVariables::drawImp->DrawItem( edge->GetBegVertexPointer(), TRGB_LIGHTGREEN );
//#endif
        }
        SmoothValues param;
        if ( form == st_Fillet )
          param.InitEquidistantFillet( TestVariables::radius1, TestVariables::radius2 );
        else if ( form == st_Ellipse )
          param.InitEllipseFillet( TestVariables::radius1, TestVariables::radius2 );
        else if ( form == st_Span )
          param.InitChordFillet( TestVariables::radius1 );
        param.SetProperties( _ARC_, TestVariables::prolong!=0,
                            SmoothValues::ec_uniform, TestVariables::keepKerb >= 0, TestVariables::keepKerb > 0, true, false/*equable*/ ); 

        IProgressIndicator * progBar = &::CreateProgressIndicator( new StrSpyCommon );
        c3d::SolidSPtr solid;
        std::vector<MbEdgeFunction> curvesVec;
        curvesVec.insert( curvesVec.end(), initEdgeFunctions.begin(), initEdgeFunctions.end() );
        c3d::FacesSPtrVector sptrBounds;
        InitSPtrArray<RPArray<MbFace>, MbFace>( initBounds, sptrBounds ); // TODO: перевести тест на использование векторов SPtr.
        MbResultType res = ::FilletSolid( *(MbSolid *)obj, cm_Copy, 
                                          MbShellFilletValues(curvesVec, sptrBounds, n, param),
                                          solid, progBar );
        if ( progBar != nullptr ) {
          delete progBar;
          progBar = nullptr;
        }
        ::DetachItems( sptrBounds );

        for ( i=0;i<initEdgeFunctions.Count();i++ )
          delete initEdgeFunctions[i].Function();

        if (solid!=nullptr) {
          TestVariables::viewManager->DeleteObject( obj );
           if ( !matr.IsSingle() )
             solid->Transform( matr );
           TestVariables::viewManager->AddObject( ns, solid.detach() );
        }
        else
          ::PutErrorMessage( res, IDS_TITLE_ERR );
        SetWaitCursor( false );
      }
    }
  }
} // CreateSolidChannel


//------------------------------------------------------------------------------
// Создание тела со скруглением по опорной кривой (переменного радиуса).
// Fillet edges with a supporting curves (variable radius).
// ---
void CreateSolidSmooth()
{
  MbItem  * obj( nullptr ), * obj1( nullptr ), * obj0( nullptr );
  MbTopItem * curve( nullptr ); //, * curve1( nullptr );
  Style ns;
  RPArray<MbCurveEdge> initCurves( 10, 1 );
  RPArray<MbCurve3D> initSlides( 10, 1 );
  FacesSPtrVector initBounds;
  MbCurveEdge * edge( nullptr );
  MbMatrix3D matr, matr1;
  WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();

  if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &curve, matr ) ) {
    //TestVariables::viewManager->ToggleObject(obj);
    if ( currentView3D != nullptr )
      currentView3D->PutObjectPart( obj, curve, true );
    CommonGetObj( IDS_SELECT_CURVE );
    if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() ) {
    //  if ( wind->FindNearestItem( st_Solid, tt_CurveEdge, TestVariables::cursorAxis, SENSE_RADIUS, &obj1, &curve1, matr1 ) ) {
    //    if ( curve != curve1 ) {
    //      initCurves.Add( (MbCurveEdge *)curve );
    //      initSlides.Add( &((MbCurveEdge *)curve1)->SetCurve() );
    //    }
    //  }
    //  else {
        MbRefItem * geom = nullptr;
        PathMatrix pathFrom;
        if ( wind->FindGeometricItem( st_Curve3D, tt_Undefined, pt_Curve, TestVariables::cursorAxis, SENSE_RADIUS, &obj1, &geom, pathFrom ) ) {
          if ( obj1->IsA() == st_SpaceInstance ) {
            initCurves.Add( (MbCurveEdge *)curve );
            initSlides.Add( (MbCurve3D *)((MbSpaceInstance *)obj1)->SetSpaceItem() );
          }
        }
      //}
    }

    while ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj0, &curve, TestVariables::itemPathFrom.second ) ) {
      if ( currentView3D != nullptr )
        currentView3D->PutObjectPart( obj, curve, true );
      edge = (MbCurveEdge *)curve;
      size_t const i0 = initCurves.FindIt(edge);
      if ( i0 != SYS_MAX_T ) {
        initCurves.DetachInd( i0 );
        initSlides.DetachInd( i0 );
      }
      else {
        CommonGetObj( IDS_SELECT_CURVE );
        if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() ) {
          //if ( wind->FindNearestItem( st_Solid, tt_CurveEdge, TestVariables::cursorAxis, SENSE_RADIUS, &obj1, &curve1, matr1 ) ) {
          //  if ( curve != curve1 ) {
          //    initCurves.Add( (MbCurveEdge *)curve );
          //    initSlides.Add( &((MbCurveEdge *)curve1)->SetCurve() );
          //  }
          //}
          //else {
            MbRefItem * geom = nullptr;
            PathMatrix pathFrom;
            if ( wind->FindGeometricItem( st_Curve3D, tt_Undefined, pt_Curve, TestVariables::cursorAxis, SENSE_RADIUS, &obj1, &geom, pathFrom ) ) {
              if ( obj1->IsA() == st_SpaceInstance ) {
                initCurves.Add( (MbCurveEdge *)curve );
                initSlides.Add( (MbCurve3D *)((MbSpaceInstance *)obj1)->SetSpaceItem() );
              }
            }
          //}
        }
      }
    }
    //TestVariables::viewManager->ToggleObject(obj);
  }

  size_t cCount = initCurves.size();

  if ( cCount > 0 && (cCount == initSlides.size()) ) {
    if ( GetInt( IDS_TANGENT_PROLONG, TestVariables::prolong ) ) {
      if ( GetInt( IDS_ENTER_KEEP_EDGE, TestVariables::keepKerb ) ) {
        double begLength = -1.0;
        double endLength = -1.0;
        if ( GetDouble( IDS_BEG_FILLET_LENGTH, begLength ) ) { 
          endLength = begLength;
          GetDouble( IDS_END_FILLET_LENGTH, endLength );
        }        
        ns.Init( obj->GetWidth(), obj->GetColor() );
        SetWaitCursor( true );
        MbSNameMaker n( ct_FilletSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
        std::vector<MbEdgeFunction> initEdgeFunctions;

        for ( size_t i = 0; i < cCount; i++ ) {
          edge = initCurves[i];
          initEdgeFunctions.push_back( MbEdgeFunction(edge, initSlides[i]) );

          // Отрисовка
//#ifdef _DRAWGI
          TestVariables::drawImp->DrawItem( edge, TRGB_LIGHTRED );
          TestVariables::drawImp->DrawItem( edge->GetBegVertexPointer(), TRGB_LIGHTRED );
//#endif
        }
        SmoothValues param;
        param.InitEquidistantFillet( 1.0 );
        param.SetProperties( _ARC_, TestVariables::prolong!=0,
                            SmoothValues::ec_uniform, TestVariables::keepKerb >= 0, TestVariables::keepKerb > 0, true, false/*equable*/ ); 

        if ( begLength >= 0.0 )
          param.SetBegLength() = begLength;
        if ( endLength >= 0.0 )
          param.SetEndLength() = endLength;

        if ( DrawTestSolid::checkPhantom ) {
          // Фантом поверхностей.
          ConstSolidSPtr solidPtr( (MbSolid *)obj );
          MbSmoothPhantomResult result;
          ::SmoothPhantom( MbSmoothPhantomParams(solidPtr, initEdgeFunctions, param), result );
          for ( size_t i = 0, count = result.GetSurfaces().size(); i < count; i++ ) 
            TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, result.GetSurfaces()[i] );
          ::DetachItem( solidPtr );
        }
        if ( DrawTestSolid::checkPosition ) {
          { // Позиция размера.
            MbSmoothPositionDataResults data;
            c3d::ConstSolidSPtr solidPtr( static_cast<MbSolid *>( obj ) );
            MbSmoothPositionDataParams posParams( solidPtr, initEdgeFunctions, param );
            ::SmoothPositionData( posParams, data );
            ::DetachItem( solidPtr );
            for ( size_t j = 0, jCount = data.GetPositionData().size(); j < jCount; j++ ) {
              MbPositionData * dat = data.SetPositionData()[j];
              if ( dat != nullptr ) {
                TestVariables::drawImp->DrawPoint( &dat->point1, TRGB_BLUE );
                TestVariables::drawImp->DrawPoint( &dat->point2, TRGB_BLUE );
                TestVariables::drawImp->DrawPoint( &dat->origin, TRGB_BLACK );
              }
              ::DeleteMatItem( dat );
            }
          }
          { // Последовательность ребер и фантом поверхностей.
            ConstSolidSPtr solidPtr( (MbSolid *)obj );
            bool createSurfaces = true;
            MbSmoothSequenceResult result;

            ::SmoothSequence( MbSmoothSequenceParams(solidPtr, initEdgeFunctions, param, createSurfaces), result );
            ::DetachItem( solidPtr );

            MbCartPoint3D p;
            for ( size_t j = 0, jCount = result.GetSequences().size(); j < jCount; j++ ) {
              const MbEdgeSequence & seq = result.GetSequences()[j];
                for ( size_t i = 0, iCount = seq.edges.Count(); i < iCount; i++ ) {
                  const MbCurveEdge * seqEdge = seq.edges[i];
                  bool s = seq.sense[i];
                  TestVariables::drawImp->DrawItem( seqEdge, TRGB_PURPLE );
                  seqEdge->Point( s ? 0.9 : 0.1, p );
                  TestVariables::drawImp->DrawPoint( &p, TRGB_BLUE );
                  seqEdge->Point( 0.95, p );
                  TestVariables::drawImp->DrawPoint( &p, TRGB_RED );
                }
            }
            for ( size_t i = 0, iCount = result.GetSurfaces().size(); i < iCount; i++ ) {
              TestVariables::drawImp->DrawItem( result.GetSurfaces()[i], TRGB_BLUE );
              if ( !DrawTestSolid::checkPhantom )
                TestVariables::viewManager->AddObject( TestVariables::SHELL_Style, result.GetSurfaces()[i] );
              //::DeleteItem( surfaces[i] );
            }
          }
        }

        IProgressIndicator * progBar = &::CreateProgressIndicator( new StrSpyCommon );
        c3d::SolidSPtr solid;
        MbResultType res = ::FilletSolid( *(MbSolid *)obj, cm_Copy,
                                          MbShellFilletValues(initEdgeFunctions, initBounds, n, param),
                                          solid, progBar );

        if ( progBar != nullptr ) {
          delete progBar;
          progBar = nullptr;
        }

        if ( solid != nullptr ) {
          TestVariables::viewManager->DeleteObject( obj );
           if ( !matr.IsSingle() )
             solid->Transform( matr );
           TestVariables::viewManager->AddObject( ns, solid.detach() );
        }
        else
          ::PutErrorMessage( res, IDS_TITLE_ERR );
        SetWaitCursor( false );
      }
    }
  }
} // CreateSolidSmooth


//------------------------------------------------------------------------------
// Создание тела со скругленной вершиной
// Create fillets on vertices and the edges adjacent to these vertices with a constant radius.
// ---
void CreateSolisVertexFillet() 
{
  MbItem * obj( nullptr ), * obj0( nullptr );
  MbTopItem * curve( nullptr );
  MbTopItem * vert( nullptr );
  Style ns;
  c3d::EdgesSPtrVector initCurves;
  VerticesSPtrVector initVertices;
  MbCurveEdge * edge( nullptr );
  MbVertex * vertex( nullptr );
  MbMatrix3D matr;
  WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();

  while ( GetTObj( IDS_SELECT_VERTEX, st_Solid, tt_Vertex, &obj0, &vert, matr ) ) {
    if ( obj == nullptr )
      obj = obj0;
    if ( obj == obj0 ) {
//#ifdef _DRAWGI
//      drawImp->DrawItem( vert, TRGB_LIGHTGREEN );
//#endif
      if ( currentView3D != nullptr )
        currentView3D->PutObjectPart( obj0, vert, true );
      if ( vert != nullptr && vert->IsA() == tt_Vertex ) {
        vertex = (MbVertex * )vert;
        if ( std::find(initVertices.begin(), initVertices.end(), vertex) == initVertices.end() )
          initVertices.push_back( VertexSPtr(vertex) );
      }
      if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj0, &curve, TestVariables::itemPathFrom.second ) ) {
        if ( obj == obj0 ) {
//#ifdef _DRAWGI
//          drawImp->DrawItem( curve, TRGB_LIGHTGREEN );
//#endif
          if ( currentView3D != nullptr )
            currentView3D->PutObjectPart( obj0, curve, true );
          if ( curve != nullptr && curve->IsA() == tt_CurveEdge ) {
            edge = (MbCurveEdge *)curve;
            if ( std::find(initCurves.begin(), initCurves.end(), edge) == initCurves.end() )
              initCurves.push_back( EdgeSPtr(edge) );
          }
        }
      }
    }
  }

  if ( initVertices.size() > 0 ) {
    bool res = false;
    double begLength = -1.0;
    double endLength = -1.0;

    if ( GetDouble( IDS_ENTER_RADIUS, TestVariables::radius1 ) &&
         GetDouble( IDS_ENTER_RADIUS, TestVariables::radius2 ) &&
         GetDouble( IDS_ENTER_RADIUS, TestVariables::radius3 ) ) 
      res = true;

    //conic = _ARC_;
    if ( res && 
         GetInt( IDS_CORNER_TYPE, TestVariables::cornerForm ) 
//      && GetDouble( IDS_SELECT_CONIC, conic ) && 
//      && GetInt( IDS_TANGENT_PROLONG, iProlong ) && 
//      && GetInt( IDS_ENTER_KEEP_EDGE, iKeepKerb ) 
    ) {
      int iProlong = 1;
      int iKeepKerb = 0;
      double radius0 = 0.0;
      if ( TestVariables::cornerForm == 0 )
        ::GetDouble( IDS_ENTER_RADIUS, radius0 );

      FacesSPtrVector initBounds; // Грани для обрезки краёв скруглений.

      SetWaitCursor( true );

      ns.Init( obj->GetWidth(), obj->GetColor() );
      MbSNameMaker n( ct_FilletSolid, MbSNameMaker::i_SideNone, 0 );
      SmoothValues::CornerForm cForm = SmoothValues::ec_uniform; 
      SmoothValues params;
      params.InitEquidistantFillet( TestVariables::radius1 );
      params.SetProperties( TestVariables::conic, iProlong != 0,
                           cForm, iKeepKerb >= 0, TestVariables::keepKerb > 0, true, false ); 
      if ( begLength >= 0.0 )
        params.SetBegLength() = begLength;
      if ( endLength >= 0.0 )
        params.SetEndLength() = endLength;

      CornerValues cornerData( radius0, TestVariables::radius1, TestVariables::radius2, TestVariables::radius3, (CornerValues::CornerForm)TestVariables::cornerForm );

      if ( DrawTestSolid::checkPhantom ) {
        ConstSolidSPtr solidPtr( (MbSolid *)obj );
        MbSmoothPhantomResult result;
        ::SmoothPhantom( MbSmoothPhantomParams(solidPtr, initCurves, params), result );
        ::DetachItem( solidPtr );

        for ( size_t i = 0, count = result.GetSurfaces().size(); i < count; i++ ) 
          TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, result.GetSurfaces()[i] );
      }
      if ( DrawTestSolid::checkPosition ) {
        MbSmoothPositionDataResults data;
        c3d::ConstSolidSPtr solidPtr( static_cast<MbSolid *>( obj ) );
        MbSmoothPositionDataParams posParams( solidPtr, initCurves, params );
        ::SmoothPositionData( posParams, data );
        ::DetachItem( solidPtr );
        for ( size_t j = 0, jCount = data.GetPositionData().size(); j < jCount; j++ ) {
          MbPositionData * dat = data.SetPositionData()[j];
          if ( dat != nullptr ) {
            TestVariables::drawImp->DrawPoint( &dat->point1, TRGB_BLUE );
            TestVariables::drawImp->DrawPoint( &dat->point2, TRGB_BLUE );
            TestVariables::drawImp->DrawPoint( &dat->origin, TRGB_BLACK );
          }
          ::DeleteMatItem( dat );
        }
      }

      IProgressIndicator * progBar = &::CreateProgressIndicator( new StrSpyCommon );
      c3d::SolidSPtr solid;
      MbResultType resType = ::FilletSolid( *(MbSolid *)obj, cm_Copy,
                                            MbShellFilletValues(initCurves, initBounds, initVertices, cornerData, params, n),
                                            solid, progBar );
      if ( progBar != nullptr ) {
        delete progBar;
        progBar = nullptr;
      }
      
      if ( solid != nullptr ) {
        TestVariables::viewManager->DeleteObject( obj );
         if ( !matr.IsSingle() )
           solid->Transform( matr );
         TestVariables::viewManager->AddObject( ns, solid.detach() );
      }
      else {
        ::PutErrorMessage( resType, IDS_TITLE_ERR );
      }
      SetWaitCursor( false );
    }
  }
} // CreateSolisVertexFillet


//------------------------------------------------------------------------------
// Создание тела со скруглением грани.
// Create a fillet on the specified faces of the solid.
// ---
void CreateFullFilletSolid()
{
  MbItem * obj ( nullptr ), *obj0 ( nullptr );
  MbSolid * solid = nullptr;   // Создаваемое тело.
  MbTopItem * face;  // Указатель для грани.
  Style ns;
  RPArray<MbFace> initFaces0, initFacesL, initFacesR;  // Набор граней.
  MbVector3D dir;

  while ( GetTObj(IDS_SELECT_FACE, st_Solid, tt_Face, &obj0, &face) ) {
    if ( obj == nullptr )
      obj = obj0;
    if ( obj == obj0 ) {
      if ( face != nullptr && face->IsA() == tt_Face ) {
        TestVariables::drawImp->DrawItem( face, 255, 255, 255 );
        if ( initFaces0.FindIt(static_cast<MbFace *>(face)) == SYS_MAX_T )
          initFaces0.Add( static_cast<MbFace *>(face) );
      }
    }
  }

  if ( initFaces0.Count() > 0 ) {
    while ( GetTObj(IDS_SELECT_FACE, st_Solid, tt_Face, &obj0, &face) ) {
      if ( obj == nullptr )
        obj = obj0;
      if ( obj == obj0 ) {
        if ( face != nullptr && face->IsA() == tt_Face ) {
          TestVariables::drawImp->DrawItem( face, 255, 255, 255 );
          if ( initFacesL.FindIt( static_cast<MbFace *>(face) ) == SYS_MAX_T )
            initFacesL.Add( static_cast<MbFace *>(face) );
        }
      }
    }
    while ( GetTObj(IDS_SELECT_FACE, st_Solid, tt_Face, &obj0, &face) ) {
      if ( obj == nullptr )
        obj = obj0;
      if ( obj == obj0 ) {
        if ( face != nullptr && face->IsA() == tt_Face ) {
          TestVariables::drawImp->DrawItem( face, 255, 255, 255 );
          if ( initFacesR.FindIt(static_cast<MbFace *>(face)) == SYS_MAX_T )
            initFacesR.Add( static_cast<MbFace *>(face) );
        }
      }
    }
  }

  MbResultType res = rt_Error;

  if ( initFaces0.Count() > 0 && initFacesL.Count() > 0 && initFacesR.Count() > 0 ) {
    int iProlong = 1;
    if ( !GetInt(IDS_TANGENT_PROLONG, iProlong) )
      iProlong = 1;
    MbSNameMaker n( ct_FullFilletSolid, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );
    c3d::FacesSPtrVector sptrBounds, sptrBoundsLeft, sptrBoundsRight;
    InitSPtrArray<RPArray<MbFace>, MbFace>( initFaces0, sptrBounds );
    InitSPtrArray<RPArray<MbFace>, MbFace>( initFacesL, sptrBoundsLeft );
    InitSPtrArray<RPArray<MbFace>, MbFace>( initFacesR, sptrBoundsRight ); // TODO: перевести тест на использование векторов SPtr.

    MbShellFullFilletParams ffilletParams( sptrBounds, sptrBoundsLeft, sptrBoundsRight, FullFilletValues( !!iProlong ), n );
    c3d::SolidSPtr solidPtr;

    res = ::FullFilletSolid( *( static_cast<MbSolid *>(obj) ), cm_Copy, ffilletParams, solidPtr );
    ::DetachItems( sptrBounds );
    ::DetachItems( sptrBoundsLeft );
    ::DetachItems( sptrBoundsRight );
    solid = solidPtr.detach();
  }
  if ( solid != nullptr ) {
    ns.Init( obj->GetWidth(), obj->GetColor() );
    TestVariables::viewManager->DeleteObject( obj );
    TestVariables::viewManager->AddObject( ns, solid );
  }
  else
    ::PutErrorMessage( res, IDS_TITLE_ERR );
  SetWaitCursor( false );
}


//------------------------------------------------------------------------------
// Создание тела с отверстием
// Create a hole, a pocket, a groove in the solid or create a drill, a boss if 'solid' == nullptr
// ---
void CreateHoleSolid( int ind )
{
  bool b = false;
  HoleValues * params = nullptr;
  MbeHoleType holeType = (MbeHoleType)ind;
  
  switch ( holeType ) {
    case ht_BorerValues : { // Отверстие
      BorerValues * par = new BorerValues();
      params = par;
      int type = (int)par->type;

      if ( GetInt( IDS_ENTER_HOLE_TYPE, type ) ) {
        par->type = (BorerValues::BorerType)type;
        par->capAngle   *= Math::RADDEG;
        par->angle      *= Math::RADDEG;
        par->spikeAngle *= Math::RADDEG;

        switch ( type ) {
          case 0 : { // Простое цилиндрическое отверстие
            b = ( GetDouble( IDS_ENTER_HOLE_DIAMETER, par->diameter ) &&
                  GetDouble( IDS_ENTER_HOLE_DIPTH,    par->depth ) &&
                  GetDouble( IDS_ENTER_SPIKE_ANGLE,   par->spikeAngle ) ); 
          } break;
          case 1 : { // Двойное цилиндрическое отверстие
            b = ( GetDouble( IDS_ENTER_CAP_DIAMETER,  par->capDiameter ) &&
                  GetDouble( IDS_ENTER_CAP_DIPTH,     par->capDepth ) &&
                  GetDouble( IDS_ENTER_HOLE_DIAMETER, par->diameter ) &&
                  GetDouble( IDS_ENTER_HOLE_DIPTH,    par->depth ) &&
                  GetDouble( IDS_ENTER_SPIKE_ANGLE,   par->spikeAngle ) ); 
          } break;
          case 2 : { // Цилиндрическое отверстие с фаской
            b = ( GetDouble( IDS_ENTER_CAP_DIAMETER,  par->capDiameter ) &&
                  GetDouble( IDS_ENTER_HOLE_ANGLE,    par->capAngle ) &&
                  GetDouble( IDS_ENTER_HOLE_DIAMETER, par->diameter ) &&
                  GetDouble( IDS_ENTER_HOLE_DIPTH,    par->depth ) &&
                  GetDouble( IDS_ENTER_SPIKE_ANGLE,   par->spikeAngle ) ); 
          } break;
          case 3 : { // Двойное цилиндрическое отверстие с переходом
            b = ( GetDouble( IDS_ENTER_CAP_DIAMETER,  par->capDiameter ) &&
                  GetDouble( IDS_ENTER_CAP_DIPTH,     par->capDepth ) &&
                  GetDouble( IDS_ENTER_HOLE_ANGLE,    par->capAngle ) &&
                  GetDouble( IDS_ENTER_HOLE_DIAMETER, par->diameter ) &&
                  GetDouble( IDS_ENTER_HOLE_DIPTH,    par->depth ) &&
                  GetDouble( IDS_ENTER_SPIKE_ANGLE,   par->spikeAngle ) ); 
          } break;
          case 4 : { // Простое коническое отверстие         //-V112
            par->angle /= 9;
            b = ( GetDouble( IDS_ENTER_HOLE_DIAMETER, par->diameter ) &&
                  GetDouble( IDS_ENTER_HOLE_DIPTH,    par->depth ) &&
                  GetDouble( IDS_ENTER_HOLE_ANGLE,    par->angle ) &&
                  GetDouble( IDS_ENTER_SPIKE_ANGLE,   par->spikeAngle ) ); 
          } break;
          case 5 : { // Простое коническое отверстие         //-V112
            b = ( GetDouble( IDS_ENTER_HOLE_DIAMETER, par->diameter ) &&
                  GetDouble( IDS_ENTER_HOLE_DIPTH,    par->depth ) &&
                  GetDouble( IDS_ENTER_RADIUS,        par->arcRadius ) );
          } break;
          case 6 : { // Двойное цилиндрическое отверстие
            par->arcRadius *= ONE_HALF;
            b = ( GetDouble( IDS_ENTER_CAP_DIAMETER,  par->capDiameter ) &&
                  GetDouble( IDS_ENTER_CAP_DIPTH,     par->capDepth ) &&
                  GetDouble( IDS_ENTER_HOLE_DIAMETER, par->diameter ) &&
                  GetDouble( IDS_ENTER_HOLE_DIPTH,    par->depth ) &&
                  GetDouble( IDS_ENTER_FILLET_RADIUS, par->arcRadius ) &&
                  GetDouble( IDS_ENTER_SPIKE_ANGLE,   par->spikeAngle ) ); 
          } break;
        }
        par->capAngle   *= Math::DEGRAD;
        par->angle      *= Math::DEGRAD;
        par->spikeAngle *= Math::DEGRAD;
        bool bProlong     = true;
        GetBoolean( IDS_ENTER_HOLE_PROLONG, bProlong, false );
        par->prolong     = bProlong;
        bool down        = true;
        GetBoolean( IDS_ENTER_DOWN_BOOL, down, false );
        par->down        = down;
      } 
    } break;

    case ht_PocketValues : { // Карман
      PocketValues * par = new PocketValues(); 
      params = par; 
      // Вводим параметры кармана-бобышки
      int choice = 1;
      b = GetInt( IDS_POCKET_TYPE, choice );
      par->depth = 5.0;
      if ( b ) {
        par->type = choice > 0 ? true : false ;
        b = ( GetDouble( IDS_POCKET_LENGTH, par->length       ) &&
              GetDouble( IDS_POCKET_WIDTH,  par->width        ) &&
              GetDouble( IDS_POCKET_FILLET, par->cornerRadius ) &&
              GetDouble( IDS_POCKET_DEPTH,  par->depth        ) &&
              GetDouble( IDS_POCKET_ANGLE,  par->taperAngle   ) &&
              GetDouble( IDS_POCKET_FLOOR,  par->floorRadius  ) );
      }
    } break;

    case ht_SlotValues : { // Паз
      SlotValues * par = new SlotValues(); 
      params = par; 
      // Вводим параметры паза
/*
        b = ( GetDouble( IDS_POCKET_LENGTH, par->length       ) &&
              GetDouble( IDS_POCKET_WIDTH,  par->width        ) &&
              GetDouble( IDS_POCKET_DEPTH,  par->depth        ) &&
              GetDouble( IDS_POCKET_WIDTH,  par->bottomWidth  ) &&
              GetDouble( IDS_POCKET_DEPTH,  par->bottomDepth  ) &&
              GetDouble( IDS_POCKET_FLOOR,  par->floorRadius  ) );

      par->tailAngle;   ///< угол уклона стенок паза типа st_DoveTail (отклонение от вертикали в радианах)
      par->type;        ///< тип паза
*/
    } break;
  }

  if ( b && params != nullptr ) {
    MbItem * obj = nullptr;
    MbTopItem * surface = nullptr;
    MbFace * face = nullptr;
    WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();
    MbPlacement3D viewPlace( currentView3D->GetOwnPlace() );
    MbSNameMaker name( ct_HoleSolid, MbSNameMaker::i_SideNone, 0 );
    Style ns = TestVariables::SWEPT_Style;
    MbResultType res;
    SolidSPtr solid;

    if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &surface, TestVariables::itemPathFrom.second ) ) {
      TestVariables::viewManager->ToggleObject(obj);
//#ifdef _DRAWGI
      TestVariables::drawImp->DrawItem( surface, TRGB_WHITE );
//#endif
      GetDouble( IDS_ENTER_SLOPE_ANGLE, params->placeAngle );
      params->placeAngle *= Math::DEGRAD;
      GetDouble( IDS_ENTER_ROTATE_ANGLE, params->azimuthAngle );
      params->azimuthAngle *= Math::DEGRAD;
      ns.Init( obj->GetWidth(), obj->GetColor() );
      face = (MbFace *)surface;
      MbPlacement3D place;
      face->GetPlanePlacement( place );
      MbVector3D dir(viewPlace.GetAxisZ());
      double u, v;
      u = v = UNDEFINED_DBL;
      if ( face->GetSurface().NearDirectPointProjection( TestVariables::cursorPoint, dir, u, v, true ) ) {
        MbVector3D norm;
        face->GetSurface().Normal( u, v, norm );
        MbCartPoint3D orig;
        face->GetSurface().PointOn( u, v, orig );
        place.Init( orig, norm, false );
      }
      res = ::HoleSolid( SolidSPtr(static_cast<MbSolid *>(obj)), cm_Copy, MbHoleSolidParams(place, *params, name), solid );
    }
    else {
      MbPlacement3D place( viewPlace );
      place.SetOrigin( TestVariables::cursorPoint );
      res = ::HoleSolid( SolidSPtr(), cm_Copy, MbHoleSolidParams(place, *params, name), solid );
    }
    if (solid!=nullptr) {
      if (obj!=nullptr)
        TestVariables::viewManager->DeleteObject( obj );
      TestVariables::viewManager->AddObject( ns, solid );
    }
    else
      ::PutErrorMessage( res, IDS_TITLE_ERR );
  }
  delete params;
}


//------------------------------------------------------------------------------
// Трансформация тела по матрице.
// Transform a copy of the initial solid using a given matrix.
// ---
void TransformedSolid()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_SOLID, st_Solid, &obj, TestVariables::itemPathFrom ) ) {
    PRECONDITION( TestVariables::itemPathFrom.second.IsSingle() ); // прописать (см. булеву и сечение тела поверхностью)
    SetWaitCursor( true );

    MbSolid *solid = (MbSolid *)obj;

    if ( solid != nullptr ) {
      bool isotropy = false;
      int cs = 0;
//      GetInt ( IDS_ENTER_BUILD_METHOD, cs );
      if ( cs > 0 )
        isotropy = true;

      MbMatrix3D matr;
      MbCartPoint3D pnt;
      bool fixedPnt = false;
      TransformValues p( matr, pnt, fixedPnt, isotropy );
      MbSNameMaker names( ct_TransformedSolid, MbSNameMaker::i_SideNone, 0 );

      TestVariables::viewManager->EraseObject(obj);

      MbSolid * result = nullptr;
      MbResultType res = ::TransformedSolid( *solid, cm_Copy, p, names, result );

      if ( result != nullptr ) {
        Style ns = TestVariables::ELEMENTARY_Style;
        if ( obj != nullptr ) {
          ns.Init( obj->GetWidth(), obj->GetColor() );
          TestVariables::viewManager->DeleteObject( obj );
        }
        TestVariables::viewManager->AddObject( ns, result );
      }
      else
        ::PutErrorMessage( res, IDS_TITLE_ERR );
    }
    SetWaitCursor( false );
  }
}


//------------------------------------------------------------------------------
// Прямое моделирование (модификация) тела удалением рёбер.
// Direct modeling (modification) of a body by removing edges. 
// ---
void EdgeModifiedSolid()
{
  MbItem  * obj( nullptr ), * obj0( nullptr );
  MbTopItem * topoItem = nullptr;
  RPArray<MbCurveEdge> edges( 4, 1 );         //-V112
  MbCurveEdge * edge( nullptr );
  MbMatrix3D matr;
  MbSolid * resSolid = nullptr;
  WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();

  if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &topoItem, matr ) ) {
    TestVariables::viewManager->ToggleObject(obj);
    if ( currentView3D != nullptr )
      currentView3D->PutObjectPart( obj, topoItem, true );
    edge = (MbCurveEdge *)topoItem;
    edges.Add( edge );
    while ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj0, &topoItem ) ) {
      if (obj==obj0) {
        if ( currentView3D != nullptr )
          currentView3D->PutObjectPart( obj, topoItem, true );
        edge = static_cast<MbCurveEdge *>(topoItem);
        size_t const i0 = edges.FindIt( edge );
        if (i0 != SYS_MAX_T) {
          if ( currentView3D != nullptr )
            currentView3D->PutObjectPart( obj, topoItem, false );
          edges.DetachInd( i0 );
        }
        else
          edges.Add(edge);
      }
    }
    TestVariables::viewManager->ToggleObject(obj);
  }

  if ( obj != nullptr && edges.size() > 0 ) {
    SetWaitCursor( true );
    EdgeFacesIndices edgeIndices;
    MbSolid * solid = static_cast<MbSolid*>( obj );
    MbFaceShell * shell = solid->GetShell();

    if ( shell != nullptr ) 
      shell->FindFacesIndexByEdges( edges, edgeIndices, true );

    BoolVector beginOrEnd;

    for ( const MbCurveEdge * edg : edges ) {
      TestVariables::drawImp->DrawItem( edg, TRGB_LIGHTGREEN );
      TestVariables::drawImp->DrawItem( &edg->GetVertex( 0 ), TRGB_RED ); // Вершина начала ребра
      TestVariables::drawImp->DrawItem( &edg->GetVertex( 1 ), TRGB_LIGHTGREEN ); // Вершина конца ребра
      beginOrEnd.push_back( GetBool( IDS_BEGIN_OR_END, true ) );
    }
    MbSNameMaker names( ct_FaceModifiedSolid, MbSNameMaker::i_SideNone, 0 );
    MbModifiedSolidParams params = MbModifiedSolidParams( dmm_DeleteEdges, names, edgeIndices, beginOrEnd );
    double prec = params.GetPrecision();
    GetDouble( IDS_ENTER_METRIC_PRECISION, prec );
    params.SetPrecision( prec );
    MbModifiedSolidResults results;
    MbResultType res = ::ModifiedSolid( *solid, cm_Copy, params, results );
    resSolid = results._resSolid.detach();
    
    if ( resSolid != nullptr ) {
      Style ns( TestVariables::SOLID_Style );
      ns.Init( obj->GetWidth(), obj->GetColor() );
      TestVariables::viewManager->DeleteObject( obj );
      TestVariables::viewManager->AddObject( ns, resSolid );
    }
    else
      ::PutErrorMessage( res, IDS_TITLE_ERR );
    SetWaitCursor( false );
  }
}


//------------------------------------------------------------------------------
// Отрисовка массива граней. Draw faces array.
// ---
template <class FArray> static
void DrawFaces( const FArray & faces, int R, int G, int B )
{
  for ( auto && drawFace : faces )
    TestVariables::drawImp->DrawItem( drawFace, R, G, B );
}


//------------------------------------------------------------------------------
// Прямое моделирование тела на новых интерфейсах.
// Direct solid modification with the new api.
// ---
bool ModifySolid( MbeModifyingMode modifyMode )
{
  MbItem * objSolid1  = nullptr, * objSolid2  = nullptr;
  MbTopItem * topoItem = nullptr;
  std::shared_ptr<MbModifiedSolidParams> params;
  MbModifiedSolidResults results;
  MbSolid * resSolid = nullptr;
  MbResultType res( rt_ErrorTotal );
  if ( modifyMode != dmm_DeleteEdges &&
       GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &objSolid1, &topoItem, TestVariables::itemPathFrom.second ) &&
       objSolid1 != nullptr && // Выбрали тело и одну грань
       topoItem != nullptr ) {
    SetWaitCursor( true );
    TestVariables::drawImp->DrawItem( topoItem, TRGB_WHITE );
    MbSolid * solid = static_cast<MbSolid*>( objSolid1 );
    MbFace * face = static_cast<MbFace*>( topoItem );
    RPArray<MbFace> faces( 1, 1 );

    faces.Add( face );
    MbItem * obj0 = nullptr;
    // Продолжаем выбирать грани
    while ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj0, &topoItem, TestVariables::itemPathFrom.second ) ) {
      if ( objSolid1 != obj0 ) // Проверка на то же самое тело.
        continue;
      TestVariables::drawImp->DrawItem( topoItem, TRGB_WHITE );
      face = static_cast<MbFace*>( topoItem );
      size_t i0 = faces.FindIt( face );
      if ( i0 != SYS_MAX_T ) { // Отмена этой грани
        TestVariables::drawImp->DrawItem( topoItem, TRGB_LIGHTGREEN );
        faces.DetachInd( i0 );
      }
      else
        faces.Add( face );
      ::DrawFaces( faces, TRGB_WHITE ); // Draw all the chosen faces once again.
    }
    ::DrawFaces( faces, TRGB_PURPLE ); // Draw all the chosen faces once again.

    std::vector<MbItemIndex> faceIndices;
    solid->GetShell()->FindIndexByFaces( faces, faceIndices );

    MbSNameMaker names( ct_FaceModifiedSolid, MbSNameMaker::i_SideNone, 0 );

    if ( modifyMode == dmm_MoveFacesByVector || 
         modifyMode == dmm_MoveFacesByVectorNoAdaptation ) { // Запрос смещения
      MbVector3D & shift = TestVariables::moveVector;
      MbCartPoint3D p( shift.x, shift.y, shift.z );
      GetExactPoint( p ); // Ввод параметра модификации (точных координат вектора)
      shift.Init( p );
      bool defValue = ( modifyMode == dmm_MoveFacesByVector );
      modifyMode = GetBool( IDS_TRANSLATION_MODE, defValue ) ? dmm_MoveFacesByVector : dmm_MoveFacesByVectorNoAdaptation;
      params = std::make_shared<MbModifiedSolidParams>( modifyMode, names, faceIndices, shift );
    }
    else if ( modifyMode == dmm_OffsetFaces ||
              modifyMode == dmm_FilletsRadiusChange ||
              modifyMode == dmm_FilletsRadiusSet ||
              modifyMode == dmm_FilletsRadiusSetSingle ||
              modifyMode == dmm_CylinderRadiusSet ||
              modifyMode == dmm_SphereRadiusSet ||
              modifyMode == dmm_CylinderRadiusSetNoAdaptation ||
              modifyMode == dmm_SphereRadiusSetNoAdaptation ) { // Запрос смещения
      if ( modifyMode != dmm_OffsetFaces ) { // Set current value for the radius change
        if ( faceIndices.size() > 0 ) {
          MbFace * tmpface = solid->GetFace( faceIndices[0].GetIndex() );
          if ( tmpface != nullptr ) {
            MbCartPoint3D centerPoint;
            tmpface->Point( .5, .5, centerPoint );
            double rad = tmpface->GetSurface().GetFilletRadius( centerPoint );
            if ( rad > METRIC_PRECISION )
              TestVariables::offset0 = rad;
          }
        }
      }
      if ( modifyMode == dmm_CylinderRadiusSet || modifyMode == dmm_SphereRadiusSet ) {
        bool adaptFillets( true );
        if ( !GetBool( IDS_TRANSLATION_MODE, adaptFillets ) )
          modifyMode = modifyMode == dmm_CylinderRadiusSet ? dmm_CylinderRadiusSetNoAdaptation : dmm_SphereRadiusSetNoAdaptation;
      }
      GetDouble( IDS_ENTER_NUMBER, TestVariables::offset0 );
      params = std::make_shared<MbModifiedSolidParams>( modifyMode, names, faceIndices, TestVariables::offset0 );
    }
    else if ( modifyMode == dmm_RotateFaces ) { // Выберем в качестве оси какое-нибудь ребро, для тестов хватит.
      bool foundAxis = false;
      MbMatrix3D dummyMatr; // Выбираем ребро.
      MbAxis3D rotationAxis;
      if ( GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &objSolid1, &topoItem, dummyMatr ) ) {
        WorkWindow * currentView3D = TestVariables::viewManager->GetActiveWindow();
        if ( currentView3D != nullptr ) // Отрисовка ребра
          currentView3D->PutObjectPart( objSolid1, topoItem, true );
        // Например, возьмём ось, касательную в середине ребра.
        const MbCurve3D & curve = static_cast<MbCurveEdge *>( topoItem )->GetCurve();
        double tMid = curve.GetTMid();
        rotationAxis.Init( curve.PointOn( tMid ), curve.FirstDer( tMid ) );
        foundAxis = true;
      }
      else { // Без ребра ввод по точкам
        MbCartPoint3D point1, point2;
        if ( GetExactPoint( point1 ) && GetExactPoint( point2 ) ) {
          rotationAxis.Init( point1, point2 );
          foundAxis = true;
        }
      }
      if ( foundAxis ) {
        ::GetDouble( IDS_ENTER_ANGLE, TestVariables::angle2 ); // градусы
        double radAngle = TestVariables::angle2 / 180. * M_PI; // радианы
        params = std::make_shared<MbModifiedSolidParams>( modifyMode, names, faceIndices, rotationAxis, radAngle );
      }

    }
    else if ( modifyMode == dmm_ConvertFacesToNurbs ) { // Редактировать как NURBS
      ptrdiff_t uDegree = NURBS_DEGREE;
      ptrdiff_t vDegree = NURBS_DEGREE;
      GetInt ( IDS_ENTER_U_DEGREE, uDegree );
      GetInt ( IDS_ENTER_V_DEGREE, vDegree );

      if ( uDegree > 1 && vDegree > 1 ) { // Редактировать как NURBS
        ptrdiff_t uCount = ITERATE_COUNT;
        ptrdiff_t vCount = ITERATE_COUNT;
        GetInt ( IDS_ENTER_U_POINT_NUMBER, uCount );
        vCount = uCount;
        GetInt ( IDS_ENTER_V_POINT_NUMBER, vCount );
        if ( uCount < uDegree )
          uCount = uDegree;
        if ( vCount < vDegree )
          vCount = vDegree;

        int uApprox = 1; //Использовать аппроксимацию при построении кривой
        int vApprox = 1;
        GetInt ( IDS_ENTER_U_USE_APPROX, uApprox );
        GetInt ( IDS_ENTER_V_USE_APPROX, vApprox );
        NurbsValues p( static_cast<size_t>(uDegree), static_cast<size_t>(uCount), 0.0, 1.0, uApprox == 1,
                       static_cast<size_t>(vDegree), static_cast<size_t>(vCount), 0.0, 1.0, vApprox == 1 ); // Параметры NURBS-поверхностей

        // Замена выбранных граней тела деформируемыми гранями (превращение в NURBS для редактирования)
        res = ::ModifiedNurbsItem( *solid, cm_Copy, p, faces, names, resSolid );
        if ( res == rt_ErrorTotal ) {
          c3d::SurfacesVector dummyS;
          params = std::make_shared<MbModifiedSolidParams>( modifyMode, names, faceIndices, dummyS );
        }
      }    
    }
    else if ( modifyMode == dmm_ReplaceFaces ) { // Заменить грани
      SurfacesVector surfaces;
      MbFace * face_ = ::SelectFace();
      if ( face_ != nullptr ) {
        while ( face_ != nullptr ) {
          TestVariables::drawImp->DrawItem( face_, TRGB_WHITE );
          surfaces.emplace_back( &face_->SetSurface() );
          face_ = ::SelectFace();
        }
      }
      else { // Если грани найти не получилось, то ищем поверхности
        MbSurface * surface = ::SelectSurface();
        while ( surface != nullptr ) {
          TestVariables::drawImp->DrawItem( surface, TRGB_WHITE );
          surfaces.emplace_back( surface );
          surface = ::SelectSurface();
        }
      }

      NumbersVector surfaceParts( faceIndices.size(), 0 ); // Номера частей заменяющих поверхностей
      DoubleVector offsets( faceIndices.size(), 0. );      // Сдвиги от заменяющих поверхностей

      params = std::make_shared<MbModifiedSolidParams>( modifyMode, names, faceIndices, surfaces, true, surfaceParts, offsets );
    }
    else if ( modifyMode == dmm_ReplaceGroupFaces ) {
      MbItem * curSolid = nullptr;
      MbTopItem * faceItem2 = nullptr;
      FacesVector replacementFaces;
      ::DrawFaces( faces, TRGB_PURPLE ); // Draw all the chosen faces once again.

      while ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &curSolid, &faceItem2, TestVariables::itemPathFrom.second ) ) {
        if ( objSolid2 == nullptr )
          objSolid2 = curSolid;
        else if ( objSolid2 != curSolid ) { // Replacement faces must belong to the same solid.
          replacementFaces.clear();
          break;
        }
        TestVariables::drawImp->DrawItem( faceItem2, TRGB_WHITE );
        face = static_cast<MbFace*>( faceItem2 );
        auto found = std::find( replacementFaces.begin(), replacementFaces.end(), face );
        if ( found != replacementFaces.end() ) {
          TestVariables::drawImp->DrawItem( face, TRGB_LIGHTGREEN );
          replacementFaces.erase( found );
        }
        else
          replacementFaces.push_back( face );
        ::DrawFaces( faces, TRGB_PURPLE ); // Draw all the chosen faces once again.
        ::DrawFaces( replacementFaces, TRGB_WHITE ); // Draw all the chosen faces once again.
      }
      ::DrawFaces( faces, TRGB_PURPLE ); // Draw all the chosen faces once again.
      ::DrawFaces( replacementFaces, TRGB_AZURE ); // Draw all the chosen faces once again.
      if ( !replacementFaces.empty() && objSolid2 != nullptr ) {
        double & offset = TestVariables::offset0;
        GetDouble( IDS_ENTER_NUMBER, offset );
        MbSolid * replacementSolid = static_cast<MbSolid*>( objSolid2 );
        ItemIndices replacementIndices;
        replacementSolid->GetShell()->FindIndexByFaces( replacementFaces, replacementIndices );
        params = std::make_shared<MbModifiedSolidParams>( modifyMode, names, *replacementSolid, faceIndices, replacementIndices, offset );
      }
    }
    else { // remove, purify, unite, etc.
      params = std::make_shared<MbModifiedSolidParams>( modifyMode, names, faceIndices );
    }
    if ( params != nullptr ) { // default
      params->SetHotPointsOn( true );
      res = ::ModifiedSolid( *solid, cm_Copy, *params, results );
      if ( 0 ) {
        if ( modifyMode == dmm_FilletsRadiusSet || modifyMode == dmm_FilletsRadiusSetSingle ) {
          MbCartPoint3D hotPoint; // Draw Hot-Point
          MbAxis3D place;
          double alpha;
          if ( results.GetFilletRadiusSetHotPoint( hotPoint, place, alpha ) ) {
            TestVariables::viewManager->AddObject( TestVariables::PPOINT_Style, new MbLineSegment3D( place.GetOrigin(), place.GetAxisZ() * 50. ) );
            TestVariables::viewManager->AddObject( TestVariables::SELECT_Style, new MbLineSegment3D( place.GetOrigin(), hotPoint ) );
            TestVariables::viewManager->AddObject( TestVariables::POINT_Style,  new MbPoint3D( hotPoint ) );
            TestVariables::viewManager->AddObject( TestVariables::POINT_Style,  new MbPoint3D( place.GetOrigin() ) );
          }
        }
        else if ( modifyMode == dmm_ReplaceFaces || modifyMode == dmm_OffsetFaces || modifyMode == dmm_ReplaceGroupFaces ) {
          MbCartPoint3D hotPoint; // Draw Hot-Point
          MbVector3D dir;
          bool resHP( false );
          switch ( modifyMode ) {
            case dmm_ReplaceFaces:
              resHP = results.GetReplaceFacesHotPoint( hotPoint, dir );
              break;
            case dmm_OffsetFaces:
              resHP = results.GetOffsetFacesHotPoint( hotPoint, dir );
              break;
            case dmm_ReplaceGroupFaces:
              resHP = results.GetReplaceFacesGroupHotPoint( hotPoint, dir );
              break;
            default:
              break;
          }
          if ( resHP ) {
            TestVariables::viewManager->AddObject( TestVariables::LINE_Style, new MbLineSegment3D( hotPoint, hotPoint + dir ) );
            TestVariables::viewManager->AddObject( TestVariables::POINT_Style, new MbPoint3D( hotPoint ) );
          }
        }
      }
      resSolid = results._resSolid.detach();
    }

    if ( resSolid != nullptr ) {
      TestVariables::viewManager->RefreshModel(); // otherwise crash is possible because new and old solids have same faces
      Style ns = TestVariables::ELEMENTARY_Style;
      if ( objSolid1 != nullptr ) {
        ns.Init( objSolid1->GetWidth(), objSolid1->GetColor() );
        TestVariables::viewManager->DeleteObject( objSolid1 );
      }
      if ( objSolid2 != nullptr )
        TestVariables::viewManager->DeleteObject( objSolid2 );

      PArray<MbSolid> parts(2,2,false);
      // Отделение частей тела
      MbSNameMaker operNames( ct_DetachSolid, MbSNameMaker::i_SideNone, 0 );
      ::DetachParts( *resSolid, parts, true, operNames );
      TestVariables::viewManager->AddObject( ns, resSolid );
      for ( ptrdiff_t i = 0; i < (ptrdiff_t)parts.Count(); i++ )
        TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, parts[i] );
    }
    else
      ::PutErrorMessage( res, IDS_TITLE_ERR );
    SetWaitCursor( false );
  }
  else if ( modifyMode == dmm_DeleteEdges ) {
    EdgeModifiedSolid();
  }
  return res == rt_Success;
}


//------------------------------------------------------------------------------
// Модификация тела c измененными гранями установкой сопряжений по ребрам
// Modify a shell by the methods of direct modeling.
// ---
void FaceModifiedSolidConj()
{
  MbItem * obj  = nullptr;
  MbItem * obj0 = nullptr;
  MbTopItem * topoItem = nullptr;
  MbTopItem * curve = nullptr;

  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &topoItem, TestVariables::itemPathFrom.second ) ) {
      SetWaitCursor( true );
      MbSolid * solid = (MbSolid *)obj;
      MbFace * face = (MbFace *)topoItem;

      if ( (solid != nullptr) && (face != nullptr) && ( face->GetSurface().GetSurface().IsA() == st_SplineSurface ) ) {
        MbSNameMaker names( ct_FaceModifiedSolid, MbSNameMaker::i_SideNone, 0 );
        MbResultType res = rt_Success;
        MbSolid * result = nullptr;

        RPArray<const MbSurfaceIntersectionCurve> curves(0, 1);
        SArray<bool> sences(0, 1);
        SArray<bool> faceSences(0, 1);
        //while (GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj0, &curve, TestVariables::itemPathFrom.second ) ) {
        if (GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj0, &curve, TestVariables::itemPathFrom.second ) ) {
          MbCurveEdge * crv = (MbCurveEdge *)curve;
          MbFace * faceP = crv->GetFacePlus();
          MbFace * faceM = crv->GetFaceMinus();
          const MbSurfaceIntersectionCurve * intCrv = nullptr;
          bool sence = true;
          bool faceSence = true;
          MbFace * resFace = nullptr;
          MbFace * otherFace = nullptr;
          if ( faceP == face ) {
            resFace = faceP;
            otherFace = faceM;
          }
          else if ( faceM == face ){
            resFace = faceM;
            otherFace = faceP;
          }

          if ( resFace != nullptr && otherFace != nullptr ) {
            faceSence = otherFace->IsSameSense();
            MbLoop * loop = resFace->GetLoop(0);
            for ( size_t i = 0; i < loop->GetEdgesCount(); i++ ){
              MbOrientedEdge * oEdge = loop->GetOrientedEdge(i);
              if ( &oEdge->GetCurveEdge() == crv ) {
                intCrv = &oEdge->GetIntersectionCurve();
                sence = oEdge->IsSameSense();
              }
            }
            if ( intCrv != nullptr && !resFace->IsSameSense() )
              sence = !sence;
          }
          
          if ( intCrv != nullptr ) {
            curves.Add( intCrv );
            sences.Add( sence );
            faceSences.Add( faceSence );
          }
        }

        if ( curves.Count() > 0 ) {
          // Замена выбранных граней тела деформируемыми гранями (превращение в NURBS для редактирования)
          MbSplineSurface * faceSurface = (MbSplineSurface *)&face->GetSurface().GetSurface().Duplicate();
          faceSurface->AddRef();

          const MbContour * cntr = nullptr;
          const MbSurface & surf = face->GetSurface();
          cntr = &surf.MakeContour( true );
          cntr->AddRef();


          Array2<bool> fPoints;
          double tension = 0.5;
          int conType = 2;
          int insertRows = 4;
          GetInt ( IDS_ENTER_CONN_TYPE, conType );
          GetDouble( IDS_ENTER_TENSION, tension );
          GetInt ( IDS_ENTER_QUANTITY, insertRows );

          MbSplineSurface * resSurf = nullptr;
          res = NurbsFaceConjugation( *curves[0], sences[0], faceSences[0], *faceSurface, tension, (MbeConjugationType)conType, insertRows, resSurf );

          if (resSurf != nullptr )
            res = ::NurbsModification( *solid, cm_Copy, face, *((MbSurface *)resSurf), fPoints, names, result );

          ::ReleaseItem( resSurf );
          ::ReleaseItem( faceSurface );
          ::ReleaseItem( cntr );
        }

        if ( result != nullptr ) {
          Style ns = TestVariables::ELEMENTARY_Style;
          if ( obj != nullptr ) {
            ns.Init( obj->GetWidth(), obj->GetColor() );
            TestVariables::viewManager->DeleteObject( obj );
          }

          PArray<MbSolid> parts(2,2,false);
          // Отделение частей тела
          MbSNameMaker operNames( ct_DetachSolid, MbSNameMaker::i_SideNone, 0 );
          ::DetachParts( *result, parts, true, operNames );

          TestVariables::viewManager->AddObject( ns, result );
          for ( ptrdiff_t i = 0; i < (ptrdiff_t)parts.Count(); i++ )
            TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, parts[i] );
        }
        else
          ::PutErrorMessage( res, IDS_TITLE_ERR );
      }
      SetWaitCursor( false );
  }
}


//------------------------------------------------------------------------------
// Модификация тела c измененными гранями подобием указанной поверхности.
// Modify a shell by the methods of direct modeling.
// ---
void FaceModifiedSolidSimilarity() {
  MbItem * obj  = nullptr;
  MbTopItem * topoItem = nullptr;

  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &topoItem, TestVariables::itemPathFrom.second ) ) {
    SetWaitCursor( true );
    MbSolid * solid = (MbSolid *)obj;
    MbFace * face = (MbFace *)topoItem;

    if ( (solid != nullptr) && (face != nullptr) && ( face->GetSurface().GetSurface().IsA() == st_SplineSurface ) ) {
      MbSNameMaker names( ct_FaceModifiedSolid, MbSNameMaker::i_SideNone, 0 );
      MbResultType res = rt_Success;
      MbSolid * result = nullptr;

      MbSurface * originSurf = nullptr;
      MbItem *surface;
      bool uToU = true;
      bool normSence = true;

      if ( GetGObj( IDS_SELECT_SURFACE, st_Surface, &surface ) ) {
        TestVariables::viewManager->ToggleObject(surface);
        originSurf = (MbSurface *)((MbSpaceInstance *)surface)->GetSpaceItem();

        int toU = 1;
        GetInt( IDS_ENTER_SURFACE_PARAM, toU );
        if ( toU != 1 )
          uToU = false;

        int sence = 1;
        GetInt( IDS_ENTER_SURFACE_NORMAL, sence );
        if ( sence != 1 )
          normSence = false;
      }

      if ( originSurf != nullptr ) {
        // Замена выбранных граней тела деформируемыми гранями (превращение в NURBS для редактирования)
        MbSplineSurface * faceSurface = (MbSplineSurface *)&face->GetSurface().GetSurface().Duplicate();
        faceSurface->AddRef();

        MbSplineSurface * resSurf = nullptr;
        res = NurbsFaceSimilarity( *originSurf, *faceSurface, uToU, normSence, resSurf );

        if (resSurf != nullptr ) {
           Array2<bool> fPoints;
          res = ::NurbsModification( *solid, cm_Copy, face, *((MbSurface *)resSurf), fPoints, names, result );
        }

        ::ReleaseItem( resSurf );
        ::ReleaseItem( faceSurface );
      }

      if ( result != nullptr ) {
        Style ns = TestVariables::ELEMENTARY_Style;
        if ( obj != nullptr ) {
          ns.Init( obj->GetWidth(), obj->GetColor() );
          TestVariables::viewManager->DeleteObject( obj );
        }

        PArray<MbSolid> parts(2,2,false);
        // Отделение частей тела
        MbSNameMaker operNames( ct_DetachSolid, MbSNameMaker::i_SideNone, 0 );
        ::DetachParts( *result, parts, true, operNames );

        TestVariables::viewManager->AddObject( ns, result );
        for ( ptrdiff_t i = 0; i < (ptrdiff_t)parts.Count(); i++ )
          TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, parts[i] );
      }
      else
        ::PutErrorMessage( res, IDS_TITLE_ERR );
    }
    SetWaitCursor( false );
  }
}


//------------------------------------------------------------------------------
// Модификация тела c измененными гранями сглаживанием
// Modify a shell by the methods of direct modeling.
// ---
void FaceModifiedSolidSmoothing() {
  MbItem * obj  = nullptr;
  MbTopItem * topoItem = nullptr;

  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &topoItem, TestVariables::itemPathFrom.second ) ) {
    SetWaitCursor( true );
    MbSolid * solid = (MbSolid *)obj;
    MbFace * face = (MbFace *)topoItem;

    if ( (solid != nullptr) && (face != nullptr) && ( face->GetSurface().GetSurface().IsA() == st_SplineSurface ) ) {
      MbSNameMaker names( ct_FaceModifiedSolid, MbSNameMaker::i_SideNone, 0 );
      MbResultType res = rt_Success;
      MbSolid * result = nullptr;

      int udegree = 4;
      int vdegree = 4;

      if ( GetInt(IDS_ENTER_U_DEGREE, udegree) && GetInt(IDS_ENTER_V_DEGREE, vdegree) ){
        MbSplineSurface * faceSurface = (MbSplineSurface *)&face->GetSurface().GetSurface().Duplicate();
        faceSurface->AddRef();

        MbSplineSurface * resSurf = nullptr;
        res = SplineSurfaceSmoothing( *faceSurface, udegree, vdegree, resSurf );

        if (resSurf != nullptr ) {
          Array2<bool> fPoints;
          res = ::NurbsModification( *solid, cm_Copy, face, *((MbSurface *)resSurf), fPoints, names, result );
        }

        ::ReleaseItem( resSurf );
        ::ReleaseItem( faceSurface );
      }

      if ( result != nullptr ) {
        Style ns = TestVariables::ELEMENTARY_Style;
        if ( obj != nullptr ) {
          ns.Init( obj->GetWidth(), obj->GetColor() );
          TestVariables::viewManager->DeleteObject( obj );
        }

        PArray<MbSolid> parts(2,2,false);
        // Отделение частей тела
        MbSNameMaker operNames( ct_DetachSolid, MbSNameMaker::i_SideNone, 0 );
        ::DetachParts( *result, parts, true, operNames );

        TestVariables::viewManager->AddObject( ns, result );
        for ( ptrdiff_t i = 0; i < (ptrdiff_t)parts.Count(); i++ )
          TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, parts[i] );
      }
      else
        ::PutErrorMessage( res, IDS_TITLE_ERR );
    }
    SetWaitCursor( false );
  }
}


//------------------------------------------------------------------------------
// Collect edges for JoinShell operation.
//---
static
void CollectEdgesForJoinShell( ConstEdgesSPtrVector & edges, BoolVector & orients, double accuracy )
{
  bool firstSurf = true;

  ptrdiff_t cnt = 1;
  const MbFace * face = nullptr;

  if ( GetInt( IDS_ENTER_QUANTITY, cnt ) ) {
    MbItem * obj  = nullptr;
    MbTopItem * edgeTopItem = nullptr;
    while ( firstSurf && GetTObj( IDS_SELECT_EDGE, st_Solid, tt_CurveEdge, &obj, &edgeTopItem, TestVariables::itemPathFrom.second ) ) {
      MbCurveEdge * edge = static_cast<MbCurveEdge *>(edgeTopItem);
      TestVariables::drawImp->DrawItem( edge, TRGB_AZURE );

      if ( face == nullptr ) { // Указать грань
        if ( edge->IsBoundaryFace() ) {
          face = (edge->GetFacePlus() != nullptr) ? edge->GetFacePlus() : edge->GetFaceMinus();
          TestVariables::drawImp->DrawItem( face, TRGB_YELLOW );
          TestVariables::drawImp->DrawItem( edge, TRGB_AZURE );
        }
        if ( face == nullptr ) {
          MbItem * faceObj = nullptr;
          MbTopItem * faceTopItem = nullptr;
          if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &faceObj, &faceTopItem, TestVariables::itemPathFrom.second ) ) {
            face = static_cast<MbFace *>(faceTopItem);
            TestVariables::drawImp->DrawItem( face, TRGB_YELLOW );
            TestVariables::drawImp->DrawItem( edge, TRGB_AZURE );
          }
        }
      }
      auto itEgdes = std::find( edges.begin(), edges.end(), edge );
      if ( itEgdes != edges.end() ) {
        size_t ind = itEgdes - edges.begin();
        orients.erase( orients.begin() + ind );
        TestVariables::drawImp->DrawItem( edge, TRGB_LIGHTGREEN );
        edges[ind].detach();
        edges.erase( itEgdes );
        edge->Release();
      }
      else if ( face != nullptr ) {
        bool isConnect = false;
        if ( edge->GetFacePlus() != nullptr && edge->GetFacePlus()->IsSame( *face, accuracy ) ) {
          orients.push_back( true );
          isConnect = true;
        }
        else if ( edge->GetFaceMinus() != nullptr && edge->GetFaceMinus()->IsSame( *face, accuracy ) ) {
          orients.push_back( false );
          isConnect = true;
        }
        if ( isConnect ) {
          edges.push_back( EdgeSPtr(edge) );
          TestVariables::drawImp->DrawItem( edge, TRGB_AZURE );
          edge->AddRef();
        }
       }
       if ( (ptrdiff_t)edges.size() == cnt )
         firstSurf = false;
    }
  }
}


//------------------------------------------------------------------------------
// Создание нового тела на основе поверхности соединения.
// c from two sets of edges.
//---
void CreateJoinShell()
{
  ConstEdgesSPtrVector curves1, curves2;
  BoolVector orients1, orients2;
  const double accuracy = LENGTH_EPSILON;

  ::CollectEdgesForJoinShell( curves1, orients1, accuracy );
  ::CollectEdgesForJoinShell( curves2, orients2, accuracy );

  MbResultType res = rt_Success;

  if ( (curves1.size() > 0) && (curves2.size() > 0) ) {
    int pb1 = 3;
    double pn1 = 0.5; 
    if ( GetInt( IDS_ENTER_SURF_CONN_TYPE, pb1 ) && 
         GetDouble( IDS_ENTER_TENSION, pn1 ) ) 
    {
      int pb2 = 3;
      double pn2 = 0.5; 
      if ( GetInt( IDS_ENTER_SURF_CONN_TYPE, pb2 ) && 
           GetDouble( IDS_ENTER_TENSION, pn2 ) ) 
      {
        ptrdiff_t edgeConn1 = 1;
        ptrdiff_t edgeConn2 = 1;
        if ( GetInt( IDS_ENTER_EDGE_CONNECTING, edgeConn1 ) && GetInt( IDS_ENTER_EDGE_CONNECTING, edgeConn2 ) ) { 
          SArray<double> breaks1( 0, 1 );
          SArray<double> breaks2( 0, 1 );
          JoinSurfaceValues p;
          p.Init( breaks1, breaks2, false, 
                  (JoinSurfaceValues::JoinConnType)pb1, pn1, (edgeConn1 > 0), nullptr, nullptr,
                  (JoinSurfaceValues::JoinConnType)pb2, pn2, (edgeConn1 > 0), nullptr, nullptr );

          MbSNameMaker name( ct_JoinShell, MbSNameMaker::i_SideNone, 0 );

          const MbMatrix3D e1;
          e1.IsSingle();
          const MbMatrix3D e2;
          e2.IsSingle();

          MbJoinShellResults shellResults;
          res = ::JoinShell( MbJoinShellParams(curves1, orients1, curves2, orients2, e1, e2, p, name), shellResults );

          if ( shellResults._solid != nullptr ) {
            TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, shellResults._solid );
          }
          else {
            ::PutErrorMessage( res, IDS_TITLE_ERR );
          }
        }
      }
    }
  }
  else {
    ::PutErrorMessage( res, IDS_TITLE_ERR );
  }
}


//------------------------------------------------------------------------------
// Размножение тела.
// Duplicate the solid by the parameters and unite copies in one solid.
// ---
void DuplicationSolid( bool byMatrix )
{
  MbCartPoint3D p[4];

  const TCHAR msgTitleInfo[] = IDS_TITLE_MSG;
  const TCHAR msgTitleError[] = IDS_TITLE_ERR;

  FacesVector shellFaces;
  PathMatrix pathFrom;

  MbSolid * solid = ::SelectShellFaces( shellFaces, &pathFrom, true );

  MbResultType resCode = rt_SolidError;

  MbAccurateTimer aTimer;

  if ( solid != nullptr ) {
    SolidSPtr resSolid;

    resCode = rt_ParameterError;

    TestVariables::viewManager->ToggleObject( pathFrom.first );

    MbCube solidCube;
    solid->AddYourGabaritTo( solidCube );
    double diagCoeff = solidCube.GetDiagonal() * DELTA_MOD;

    bool adaptMode = false;
    IndicesVector facesIndices;

    MbCartPoint3D refPoint;
    solidCube.GetCenter( refPoint );

    if ( !shellFaces.empty() ) {
      solid->GetShell()->FindFacesIndexByFaces( shellFaces, facesIndices );
      if ( !facesIndices.empty() ) {
        MbCube shellCube;
        for ( const auto & faceIndex : facesIndices ) {
          solid->GetShell()->_GetFace( faceIndex )->AddYourGabaritTo( shellCube );
        }
        shellCube.GetCenter( refPoint );

        adaptMode = true;
      }
    }

    MbSNameMaker operNames( ct_DuplicationSolid, MbSNameMaker::i_SideNone, 0 );
    DPtr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyCommon ) );

    if ( byMatrix ) { // Размножение тела по набору матриц.
      if ( GetPoint( IDS_SELECT_POINT, p[0] ) ) {
        TestVariables::drawImp->DrawPoint( &p[0], TRGB_LIGHTGREEN );

        if ( GetPoint( IDS_SELECT_POINT, p[1] ) ) {
          TestVariables::drawImp->DrawPoint( &p[1], TRGB_LIGHTGREEN );

          MbAxis3D rotAxis( p[0], p[1] );
          {
            MbCartPoint3D pAxis( p[0] );
            pAxis.Add( p[1], diagCoeff );
            TestVariables::drawImp->DrawLine( p[0], pAxis, TRGB_AVIATION, 2 );
            TestVariables::drawImp->DrawPoint( &p[0], TRGB_AVIATION, 2 );
          }

          MbVector3D radDir;
          {
            radDir.Init( rotAxis.GetOrigin(), refPoint );
            radDir.NormalComponent( rotAxis.GetAxisZ() );
          }
          
          int numStep1 = 2;
          double angle = M_PI_2;

          GetDouble( IDS_ENTER_ROTATE_ANGLE, angle );
          if ( ::abs_t( angle ) > ANGLE_EPSILON ) {
            angle = std_min( angle, M_PI2 );
            angle = std_max( angle, -M_PI2 );
            GetInt( IDS_ENTER_QUANTITY, numStep1 );
            numStep1 = std_max( numStep1, 1 );
          }

          int numStep2 = 1;
          double radStep = radDir.Length();

          if ( radStep > METRIC_PRECISION ) {
            GetDouble( IDS_ENTER_STEP, radStep );

            if ( radStep > METRIC_PRECISION ) {
              radDir.Normalize();

              GetInt( IDS_ENTER_QUANTITY, numStep2 );
              numStep2 = std_max( numStep2, 1 );
            }
          }

          MbMatrix3D matrix; 
          SArray<MbMatrix3D> matrices( 0, 1 );
          MbVector3D radMove;

          for ( int i1 = 0; i1 < numStep1; ++i1 ) {
            double rotAngle = angle * static_cast<double>(i1);
            for ( int i2 = 0; i2 < numStep2; ++i2 ) {
              matrix.Init();
              radMove.Set( radDir, radStep * static_cast<double>(i2) );
              matrix.Move( radMove );
              matrix.Rotate( rotAxis, rotAngle );
              matrices.Add( matrix );
            }
          }
        
          MbDuplicationSolidParams parameters ( matrices, progBar );

          if ( !facesIndices.empty() ) {
            parameters.SetFacesIndices( facesIndices );
          }
          parameters.SetAdaptMode( adaptMode );
          parameters.SetNameMaker( operNames );

          SetWaitCursor( true );
          {
            aTimer.Begin();
            resCode = ::DuplicationSolid( *solid, parameters, resSolid );
            aTimer.End();
          }
          SetWaitCursor( false );
        }
      }
    }
    else { // Размножение тела по двумерной сетке.
      bool isPolar = false;
      bool isAlongAxis = false;
      bool isCenterGiven = false;

      isPolar = GetBool( IDS_IS_POLAR, isPolar );

      if ( isPolar ) 
        isAlongAxis = GetBool( IDS_IS_ALONG_AXIS, isAlongAxis );

      isCenterGiven = GetBool( IDS_IS_CENTER_GIVEN, isCenterGiven );

      bool goNext = false;

      if ( isCenterGiven ) {
        if ( GetPoint( IDS_SELECT_POINT, p[0] ) ) {
          TestVariables::drawImp->DrawPoint( &p[0], TRGB_LIGHTGREEN, 2 );
          goNext = true;
        }
      }
      else {
        solidCube.GetCentre( p[0] );
        TestVariables::drawImp->DrawPoint( &p[0], TRGB_LIGHTGREEN, 2 );
        goNext = true;
      }

      if ( goNext && GetPoint( IDS_SELECT_VECTOR, p[1] ) && p[1].MaxFactor() > LENGTH_EPSILON ) {
        MbCartPoint3D px( p[0] );
        px.Add( p[1], diagCoeff );
        TestVariables::drawImp->DrawLine( p[0], px, TRGB_LIGHTGREEN, 2 );
        TestVariables::drawImp->DrawPoint( &p[1], TRGB_LIGHTGREEN, 2 );

        bool isSecondDir = false;

        if ( GetPoint( IDS_SELECT_VECTOR, p[2] ) && p[2].MaxFactor() > LENGTH_EPSILON ) {
          MbCartPoint3D py( p[0] );
          py.Add( p[2], diagCoeff );
          TestVariables::drawImp->DrawLine( p[0], py, TRGB_LIGHTGREEN, 2 );
          TestVariables::drawImp->DrawPoint( &p[2], TRGB_LIGHTGREEN, 2 );
          isSecondDir = true;
          goNext = true;
        }
        else if ( !isPolar ) {
          p[2].SetZero();
          goNext = true;
        }
          
        if ( goNext ) {
          // MbVector3D vect1( p[0], p[1] );
          // MbVector3D vect2( p[0], p[2] );
          MbVector3D vect1( p[1] );
          MbVector3D vect2( p[2] );

          MbVector3D vect3( vect1 | vect2 );
          double angle = vect1.Angle( vect2 );

          const MbVector3D & dir1 = vect1;
          const MbVector3D & dir2 = isPolar ? vect3 : vect2;

          double step1 = dir1.Length();
          double step2 = isPolar ? angle : vect2.Length();

          GetDouble( IDS_ENTER_STEP, step1 );

          if ( isSecondDir ) {
            if ( isPolar )
              GetDouble( IDS_ENTER_ANGLE, step2 );
            else
              GetDouble( IDS_ENTER_STEP, step2 );
          }

          int numStep1 = 2;
          GetInt( IDS_ENTER_QUANTITY, numStep1 );

          int numStep2 = 0;
          if ( isSecondDir && ::fabs(step2) > METRIC_PRECISION ) {
            numStep2 = 2;
            GetInt( IDS_ENTER_QUANTITY, numStep2 );
          }

          MbDuplicationSolidParams parameters ( isPolar, dir1, step1, ::abs_t(numStep1),
                                                         dir2, step2, ::abs_t(numStep2),
                                                         &p[0], isAlongAxis, progBar );
          if ( !facesIndices.empty() ) {
            parameters.SetFacesIndices( facesIndices );
            adaptMode = true;
          }
          parameters.SetAdaptMode( adaptMode );
          parameters.SetNameMaker( operNames );

          SetWaitCursor( true );
          {
            aTimer.Begin();
            resCode = ::DuplicationSolid( *solid, parameters, resSolid );
            aTimer.End();
          }
          SetWaitCursor( false );
        }
      }
    }

    if ( resSolid != nullptr ) {
      TestVariables::viewManager->ToggleObject( solid );
      TestVariables::viewManager->RefreshObjectModel( solid );
      TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, resSolid );

      if ( resCode != rt_Success )
        ::PutErrorMessage( resCode, msgTitleInfo );
    }
    else {
      ::PutErrorMessage( resCode, msgTitleError );
    }
    
    if ( TestVariables::checkOperationTime ) {
      double durTime = aTimer.GetLast();

      const size_t bufLen = 512;
      TCHAR buf[bufLen];
      size_t len = 0;
      len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, durTime );
      ::MessageBoxEx( buf, IDS_RESULTS );
    }
  }
}


//------------------------------------------------------------------------------
// Создание тела слиянием граней в одну грань.
// Create a solid with one face instead selected faces
// ---
void CreateFacesMergingSolid()
{
  MbItem * obj  = nullptr;
  MbItem * obj0 = nullptr;
  MbTopItem * surface = nullptr;
  FacesVector faces;
  MbFace * face = nullptr;

  if ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj, &surface, TestVariables::itemPathFrom.second ) ) {
    TestVariables::viewManager->ToggleObject(obj);
    TestVariables::drawImp->DrawItem( surface, TRGB_WHITE );
    face = (MbFace *)surface;

    faces.push_back(face);
    while ( GetTObj( IDS_SELECT_FACE, st_Solid, tt_Face, &obj0, &surface, TestVariables::itemPathFrom.second ) ) {
      if ( obj == obj0 ) {
        TestVariables::drawImp->DrawItem( surface, TRGB_WHITE );
        face = static_cast<MbFace *>(surface);
        bool isAbsent = ( std::find(faces.begin(), faces.end(), face) == faces.end() );
        if ( isAbsent ) 
          faces.push_back(face);
      }
    }

    MbSolid * initSolid = (obj->IsA() == st_Solid) ? (MbSolid *)obj : nullptr;

    if ( initSolid != nullptr ) { // Инверсный выбор удаляемых граней
      int uDegree = 4, vDegree = 4;
      if ( GetInt( IDS_ENTER_SPLINE_ORDER, uDegree ) && GetInt( IDS_ENTER_SPLINE_ORDER, vDegree ) ) {

        SetWaitCursor( true );

        MbNurbsParameters uParam( uDegree, 0, 0.0, 1.0, true );
        MbNurbsParameters vParam( vDegree, 0, 0.0, 1.0, true );
        MbSNameMaker operNames( ct_ShellSolid, MbSNameMaker::i_SideNone, 0 );
        operNames.SetVersion( MbNameVersion() );
        bool prolong = true;
        MbSolid * solid = nullptr;
        Style ns( obj->GetWidth(), obj->GetColor() );
        double tolerance = METRIC_PRECISION;

        TestVariables::viewManager->ToggleObject(obj);

        MbResultType res = ::CreateMerging( *initSolid, cm_Copy, faces, uParam, vParam, 
                                            tolerance, operNames, prolong, solid );

        if ( solid == nullptr )
          ::PutErrorMessage( res, IDS_TITLE_ERR );
        if (solid != nullptr) {
          TestVariables::viewManager->DeleteObject( obj );
          TestVariables::viewManager->AddObject( ns, solid );
        }  

        SetWaitCursor( false );
      }
    }
  }

}