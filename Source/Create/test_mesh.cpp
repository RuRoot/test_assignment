#include <test_mesh.h>
#include <test_draw.h>
#include <test_manager.h>
#include <test_service.h>
#include <collection.h>
#include <point3d.h>
#include <solid.h>
#include <surface.h>
#include <surf_plane.h>
#include <space_instance.h>
#include <plane_instance.h>
#include <action_mesh.h>
#include <attr_color.h>
#include <m2b_mesh_curvature.h>
#include <surface_fitting.h>
#include <last.h>


using namespace c3d;


//------------------------------------------------------------------------------
// Получить цвет сегмента по типу распознанной поверхности
// Get segment color by reconstructed surface type.
// ---
static uint32 GetSegmentColorByType( MbeSpaceType surfaceType )
{
  double k = 1.0 / 255.0;
  switch ( surfaceType ) {
    case st_Plane:            return RGB2uint32( 31 * k, 117 * k, 255 * k );  // Синий.
    case st_ConeSurface:      return RGB2uint32( 252 * k, 232 * k, 131 * k );  // Желтый.
    case st_CylinderSurface:  return RGB2uint32( 237 * k, 31 * k, 76 * k );  // Красный.
    case st_SphereSurface:    return RGB2uint32( 27 * k, 171 * k, 118 * k );  // Зеленый.
    case st_TorusSurface:     return RGB2uint32( 146 * k, 109 * k, 173 * k );  // Фиолетовый.
    case st_RevolutionSurface:return RGB2uint32( 181 * k, 103 * k, 79 * k );  // Коричнево-оранжевый.
    case st_SplineSurface:    return RGB2uint32( 128 * k, 128 * k, 0 );  // Оливковый цвет.
    case st_GridSurface:      return RGB2uint32( 255 * k, 129 * k, 0 );  // Оранжевый.
    case st_FreeSurface:      return RGB2uint32( 128 * k, 128 * k, 128 * k );  // Темно-серый.
    default:                  return RGB2uint32( 148 * k, 144 * k, 139 * k );  // Серый.
  }
}


//------------------------------------------------------------------------------
// Инициализировать или получить существующий объект MbMeshProcessor.
// Initialize or get existing object MbMeshProcessor.
// ---
MbMeshProcessor * GetMeshProcessor()
{
  MbItem * obj = nullptr;
  if ( TestVariables::viewManager->GetActiveWindow()->GetMeshProcessor() == nullptr ) {
    if ( GetObj(IDS_SELECT_GEOM_OBJECT, &obj) ) {
      TestVariables::viewManager->ToggleObject( obj );
      SPtr<MbMesh> mesh;
      SPtr<MbCollection> collection;
      if ( obj->IsA() == st_Mesh ) {
        mesh = &static_cast< MbMesh & >( *obj );
        collection = new MbCollection( *mesh );
      }
      else if ( obj->IsA() == st_Collection ) {
        collection = &static_cast< MbCollection & >( *obj );
      }
      if ( collection != nullptr ) {
        TestVariables::viewManager->GetActiveWindow()->InitMeshProcessor( MbMeshProcessor::Create(*collection) );
        if ( TestVariables::viewManager->GetActiveWindow()->GetMeshProcessor() != nullptr ) {
          double tolerance = 0.001;
          bool useRelativeTolerance = false;
          int mode = srm_Default;
          useRelativeTolerance = GetBool( IDS_ENTER_MESH2BREP_RELTOLERANCE, useRelativeTolerance );
          if ( GetDouble(IDS_ENTER_PARAM_VALUE, tolerance) &&
               GetInt(IDS_ENTER_MESH2BREP_MODE, mode) ) {

            if ( useRelativeTolerance )
              TestVariables::viewManager->GetActiveWindow()->GetMeshProcessor()->SetRelativeTolerance( tolerance );
            else
              TestVariables::viewManager->GetActiveWindow()->GetMeshProcessor()->SetTolerance( tolerance );
            TestVariables::viewManager->GetActiveWindow()->GetMeshProcessor()->SetReconstructionMode( static_cast<MbeSurfReconstructMode>(mode) );
          }
        }
      }
    }
  }
  return TestVariables::viewManager->GetActiveWindow()->GetMeshProcessor();
}


//------------------------------------------------------------------------------
// Удалить вспомогательную сетку.
// Delete a temporal mesh.
// ---
void DeleteTemporalMesh( bool isWire )
{
  RPArray<MbItem> items( 3, 1 );
  SArray<MbMatrix3D> matrs( 3, 1 );
  SPtr<MbMesh> mesh;
  TestVariables::viewManager->GetObjects( st_Mesh, items, matrs ); // Дать все объекты указанного типа.
  for ( size_t i = items.Count(); i--; ) {
    if ( items[i] != nullptr && items[i]->IsA() == st_Mesh ) {
      mesh = &static_cast< MbMesh & >( *items[i] );
      if ( (isWire && mesh->GridsCount() == 0 && (mesh->PolygonsCount() > 0 || mesh->ApexesCount() > 0) ) || // Удалить вспомогательную сетку, содержащую либо апексы, либо полигоны,
           (!isWire && mesh->GridsCount() > 0) ) // либо удалить триангуляцию.
        TestVariables::viewManager->DeleteObject( items[i] );
    }
  }
  TestVariables::viewManager->ReDrawGL();
}


//------------------------------------------------------------------------------
// Рассчитать и отрисовать поле кривизн или главных направлений по сетке.
// Calculate and draw mesh curvature or mesh curvature direction map.
// ---
void CreateMeshCurvatureMap( bool isCurvatureDir )
{
  MbMeshProcessor * proc = GetMeshProcessor();
  if ( proc != nullptr ) {
    int type = 0;
    if ( ::GetInt(isCurvatureDir ? IDS_ENTER_CURVATURE_DIR_TYPE : IDS_ENTER_CURVATURE_TYPE, type) &&
         (((type >= 0 && type <= 3) && !isCurvatureDir) || ( (type >= 0 && type <= 1) && isCurvatureDir)) ) {
      SetWaitCursor( true );
      DeleteTemporalMesh( true );
      const MbCollection & workCollection = proc->GetSimplifiedMesh();
      const std::vector<MbCurvature> & curvatures = proc->CalculateCurvatures();
      const double max_c = 0.1;
      double crvValue = 0.0;
      MbVector3D dir;
      SPtr<MbMesh> curvatureMesh( new MbMesh() );
      for ( size_t i = 0; i < workCollection.PointsCount(); i++ ) {
        const MbCurvature & vv = curvatures[i];
        if ( !isCurvatureDir ) { // Кривизны.
          MbApex3D* pApex = curvatureMesh->AddApex();
          pApex->Init( workCollection.GetPoint(i) );
          switch ( type ) {
            case 0:
              crvValue = vv.k_h;
            break;
            case 1:
              crvValue = vv.k_g;
            break;
            case 2:
              crvValue = vv.k1;
            break;
            case 3:
              crvValue = vv.k2;
            break;
            default:
              crvValue = vv.k_h;
            break;
          }
          double coef = 0.5 * (crvValue + max_c) / max_c;
          coef = std_min( std_max(coef, 0.0), 1.0 );
          uint32 color = ::HSV2uint32( 240 - 240 * coef, 1.0, 1.0 );
          pApex->SetColor( color );
        }
        else { // Главные направления.
          MbPolygon3D * p = curvatureMesh->AddPolygon();
          MbFloatPoint3D vBeg( workCollection.GetPoint(i) );
          p->AddPoint( vBeg );
          double m = 0.1;
          if ( type == 0 )
            vBeg.Move( m * vv.cdir1 );
          else
            vBeg.Move( m * vv.cdir2 );
          p->AddPoint( vBeg );
        }
      }
      TestVariables::viewManager->AddObject( TestVariables::WIRE_Style, curvatureMesh );
      TestVariables::viewManager->RefreshObjectModel( curvatureMesh );
      SetWaitCursor( false );
    }
  }
}


//------------------------------------------------------------------------------
// Рассчитать и отрисовать карту сегментации.
// Calculate and draw segmentation map.
// ---
void CreateSegmentMap()
{
  MbMeshProcessor * proc = GetMeshProcessor();
  if ( proc != nullptr ) {
    SetWaitCursor( true );
    DeleteTemporalMesh( false );
    MbResultType res = proc->SegmentMesh( true );
    if ( res == rt_Success ) {
      const MbCollection & segmentation = proc->GetSegmentedMesh();
      MbStepData stepData;
      MbMesh * mesh;
      MbItem * meshItem = segmentation.CreateMesh( stepData, MbFormNote(true, true, true, TestVariables::exactMesh), nullptr );
      bool colorByType = true;
      colorByType = GetBool( _T( "Draw segment by type of surface" ), colorByType );
      if ( (meshItem != nullptr) && (meshItem->IsA() == st_Mesh) ) {
        mesh = &static_cast< MbMesh & >( *meshItem );
        mesh->SetMeshType( st_Mesh );
        mesh->SetRefItem( nullptr );
        if ( mesh->GridsCount() > 0 ) {
          uint32 c; // Светло-серый цвет.
          for ( size_t i = 0; i < mesh->GridsCount(); i++ ) {
            if ( colorByType ) {
              if ( proc->GetSegmentSurface(i) != nullptr )
                c = GetSegmentColorByType( proc->GetSegmentSurface( i )->IsA() ); //
              else
                c = RGB2uint32( 192 / 255, 192 / 255, 192 / 255 );
            }
            else 
              c = RGB2uint32( double(rand() % 255) / 255, double(rand() % 255) / 255, double(rand() % 255) / 255 );
            if ( mesh->SetGrid( i ) != nullptr )
              mesh->SetGrid( i )->SetColor( c );
          }
          mesh->SetColor( 0 );
        }
        TestVariables::viewManager->AddObject( TestVariables::SHELL_Style, meshItem );
        TestVariables::viewManager->ShowModel();
      }
    }
    SetWaitCursor( false );
  }
}


//------------------------------------------------------------------------------
// Преобразовать полигональную модель в тело.
// Convert the polygonal model into the solid.
// ---
void ConvertMeshToBRep()
{
  MbMeshProcessor * proc = GetMeshProcessor();
  MbResultType res = rt_Success;
  if ( proc != nullptr ) {
    SetWaitCursor( true );
    if ( !proc->GetSegmentedMesh().SegmentsCount() )
      res = proc->SegmentMesh( true );
    if ( res == rt_Success ) {
      DeleteTemporalMesh( false );
      MbFaceShell * shell = nullptr;
      res = proc->CreateBRepShell( shell, true, M_PI_2 );

      if ( shell != nullptr ) {
        MbCreator * creator = nullptr;
        MbSNameMaker snMaker( SimpleName(-1), MbSNameMaker::i_SideNone, 0 );
        SPtr<MbSolid> pSolid( new MbSolid( shell, creator ) );
        Style st;
        TestVariables::viewManager->AddObject( st, pSolid, false );
        TestVariables::viewManager->ShowModel();
      }
    }
    SetWaitCursor( false );
  }
}


//------------------------------------------------------------------------------
// Впиасть поверхность в множество полигонов триангуляции.
// Fit the surface to polygon set.
// ---
void FitSurfaceToGrid()
{
  MbItem * obj( nullptr );
  PathMatrix solidPathFrom;
  if ( GetGObj( IDS_SELECT_MESH, st_Mesh, &obj, solidPathFrom ) &&
       obj->IsA() == st_Mesh ) {
    MbMesh * mesh = static_cast<MbMesh *> ( obj );
    const auto gridsCount = mesh->GridsCount();
    const MbGrid * pGrid( nullptr );
    if ( gridsCount == 1 )
      pGrid = mesh->GetGrid( 0 );
    else {
      ptrdiff_t gridId( 0 );
      if( GetInt( IDS_ENTER_NUMBER_N, gridId ) && 0 <= gridId && size_t( gridId ) < mesh->GridsCount() )
        pGrid = mesh->GetGrid( gridId );
    }

    if ( pGrid != nullptr ) {
      double tolerance = DELTA_MIN;
      ptrdiff_t type( -1 );
      ptrdiff_t typeReject( 0 );
      if ( GetInt( IDS_ENTER_SURFACE_TYPE, type ) && GetDouble( IDS_ENTER_PRECISION, tolerance ) ) {
        MbeSpaceType surfType = st_Undefined;
        switch ( type ) {
        case 0: surfType = st_Plane;             break;
        case 1: surfType = st_CylinderSurface;   break;
        case 2: surfType = st_SphereSurface;     break;
        case 3: surfType = st_ConeSurface;       break;
        case 4: surfType = st_TorusSurface;      break;
        case 5: surfType = st_SplineSurface;     break;
        case 6: surfType = st_RevolutionSurface; break;
        case 7: surfType = st_ExtrusionSurface;  break;
        default: break;
        }

        double angleConeMin( 1. );
        double angleConeMax( 89. );
        double radiusAnalyticShapeMax( pGrid->GetCube().GetDiagonal() * 10 );

        size_t orderNurbs( c3d::NURBS_DEGREE );
        size_t countCpMax( c3d::NURBS_POINTS_MAX_COUNT );
        double smoothCoef( PARAM_ACCURACY );
        double angleDelta = 0.;
        bool closed( false );

        MbeRejectOutliersType rejType = rot_NoReject;
        double rejectValue( 0. );

        bool bContinue( false );
        if ( surfType == st_SplineSurface ) {
          int order = static_cast<int>( orderNurbs );
          int cpCountMax = static_cast<int>( countCpMax );
          double smooCoef = smoothCoef;
          if ( GetInt( IDS_ENTER_DEGREE, order ) && GetInt( IDS_ENTER_NUMBER_N, cpCountMax ) &&
               GetDouble( IDS_SMOOTH_COEF, smooCoef ) && GetDouble( IDS_ENTER_ROTATE_ANGLE, angleDelta ) ) {
            orderNurbs = order;
            countCpMax = cpCountMax;
            smoothCoef = smooCoef;
            bContinue = true;
          }
        }
        else if ( surfType == st_RevolutionSurface || surfType == st_ExtrusionSurface ) {
          int cpCountMax = static_cast<int>( countCpMax );
          if ( GetInt( IDS_ENTER_NUMBER_N, cpCountMax ) ) {
            countCpMax = cpCountMax;
            if ( surfType == st_ExtrusionSurface ) {
              int iCls = 0;
              if ( GetInt( IDS_ENTER_CLOSED, iCls ) ) {
                closed = iCls > 0;
                bContinue = true;
              }
            }
            else
              bContinue = true;
          }
        }
        else if( GetInt( IDS_ENTER_OUTLIER_REJECT_TYPE, typeReject ) ) {
          switch ( typeReject ) {
          case 0: rejType = rot_NoReject;         break;
          case 1: rejType = rot_Sigma;       break;
          case 2: rejType = rot_Percent;   break;
          case 3: rejType = rot_MaxDistance;   break;
          default: break;
          }

          if ( rejType == rot_Sigma )
            rejectValue = 3.;
          else if ( rejType == rot_Percent )
            rejectValue = 0.9;
          else if ( rejType == rot_MaxDistance )
            rejectValue = pGrid->GetCube().GetDiagonal() * 0.05;

          if ( rejType == rot_NoReject || GetDouble( IDS_ENTER_PARAMETER, rejectValue ) )
            bContinue = true;
        }
        
        if ( bContinue ) {
          SetWaitCursor( true );

          IndicesVector indicies;
          std::unique_ptr<MbSurfaceFitter> surfaceFitter( MbSurfaceFitter::Create( *pGrid, !indicies.empty() ? &indicies : nullptr ) );
          MbSurfaceFitToGridParameters params( surfType, tolerance, indicies, rejType, rejectValue );
          params.SetAnalyticBounds( angleConeMin, angleConeMax, radiusAnalyticShapeMax );
          params.SetNurbsFitParams( orderNurbs, countCpMax, smoothCoef );
          params.SetNurbsAngleDelta( angleDelta );
          params.SetClosed( closed );
          MbSurfaceFitToGridResults results;
          if ( rt_Success == surfaceFitter->FitSurface( params, results ) ) {
            TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, results.GetSurface(), false );
            TestVariables::viewManager->RefreshModel();
            const size_t bufLen = 1024;
            TCHAR buf[bufLen];
            _sntprintf( buf, bufLen, _T( "Maximum deviation:  %e" ), results.GetTolerance() );
            ::MessageBoxEx( buf, IDS_RESULTS );
          }
          else
            ::PutErrorMessage( rt_Error, IDS_TITLE_ERR );
          SetWaitCursor( false );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Вписать поверхность в множество полигонов триангуляции с XYW ограничением.
// Fit the surface to polygon set with XYW constraint.
// ---
void FitSurfaceToGridXYW()
{
  MbItem * obj( nullptr );
  PathMatrix solidPathFrom;
  if ( GetGObj( IDS_SELECT_MESH, st_Mesh, &obj, solidPathFrom ) &&
       obj->IsA() == st_Mesh ) {
    MbMesh * mesh = static_cast<MbMesh *> ( obj );
    const auto gridsCount = mesh->GridsCount();
    const MbGrid * pGrid( nullptr );
    if ( gridsCount == 1 )
      pGrid = mesh->GetGrid( 0 );
    else {
      ptrdiff_t gridId( 0 );
      if ( GetInt( IDS_ENTER_NUMBER_N, gridId ) && 0 <= gridId && size_t( gridId ) < mesh->GridsCount() )
        pGrid = mesh->GetGrid( gridId );
    }

    if ( pGrid != nullptr ) {
      double tolerance = DELTA_MIN;
      ptrdiff_t type( -1 );
      if ( GetInt( IDS_ENTER_SURFACE_TYPE_CONSTRAINED, type ) && GetDouble( IDS_ENTER_PRECISION, tolerance ) ) {
        MbeSpaceType surfType = st_Undefined;
        switch ( type ) {
        case 0: surfType = st_Plane;           break;
        case 1: surfType = st_CylinderSurface; break;
        case 2: surfType = st_SphereSurface;   break;
        case 3: surfType = st_ConeSurface;     break;
        case 4: surfType = st_TorusSurface;    break;
        default: break;
        }
        if ( surfType != st_Undefined ) {
          double angleConeMin( 1. );
          double angleConeMax( 89. );
          double radiusAnalyticShapeMax( pGrid->GetCube().GetDiagonal() * 10 );
          if ( GetGObj( IDS_SELECT_PLANE, st_Plane, &obj, solidPathFrom ) ) {
            if ( obj->IsA() == st_SpaceInstance ) {
              MbSpaceInstance * spInst = static_cast<MbSpaceInstance *>( obj );
              if ( spInst->GetSpaceItem() != nullptr ) {
                MbSpaceItem * spItem = static_cast<MbSpaceItem *>( spInst->SetSpaceItem() );
                if ( spItem->IsA() == st_Plane ) {
                  MbPlane * plane = static_cast<MbPlane *>( spItem );
                  MbPlacement3D place( plane->GetPlacement() );
                  IndicesVector indicies;
                  MbSurfaceFitToGridParameters params( surfType, tolerance, indicies );
                  params.SetAnalyticBounds( angleConeMin, angleConeMax, radiusAnalyticShapeMax );
                  MbSurfaceFitToGridResults results;
                  MbConstraintFixedValues fixedValues;
                  double z( 0. ), theta( 0. ), size1( -1. ), size2( -1. );
                  if ( ( surfType == st_SphereSurface && GetDouble( IDS_ENTER_SPHERE_Z_COORDINATE, z ) ||
                         surfType == st_TorusSurface && GetDouble( IDS_ENTER_THETA_PARAMETER, theta ) && GetDouble( IDS_ENTER_SPHERE_Z_COORDINATE, z ) ||
                         surfType != st_SphereSurface && surfType != st_TorusSurface && GetDouble( IDS_ENTER_THETA_PARAMETER, theta ) ) &&
                       GetDouble( IDS_ENTER_CONSTRAINT_SIZE1, size1 ) &&
                       GetDouble( IDS_ENTER_CONSTRAINT_SIZE2, size2 ) ) {
                    if ( surfType == st_SphereSurface )
                      fixedValues._z = z;
                    else if ( surfType == st_TorusSurface ) {
                      fixedValues._z = z;
                      fixedValues._theta = theta * M_DEGRAD;
                    }
                    else
                      fixedValues._theta = theta * M_DEGRAD;
                    if ( size1 > EPSILON )
                      fixedValues._size1 = size1;
                    if ( size2 > EPSILON )
                      fixedValues._size2 = size2;
                    MbeDimensionalConstraintType dimType = MbeDimensionalConstraintType::none;
                    if ( size1 > EPSILON && size2 > EPSILON )
                      dimType = MbeDimensionalConstraintType::both;
                    else if ( size1 > EPSILON )
                      dimType = MbeDimensionalConstraintType::first;
                    else if ( size2 > EPSILON )
                      dimType = MbeDimensionalConstraintType::second;
                    if ( params.SetFitConstraintXYW( surfType, dimType, fixedValues, place ) == rt_Success ) {
                      SetWaitCursor( true );
                      std::unique_ptr<MbSurfaceFitter> surfaceFitter( MbSurfaceFitter::Create( *pGrid, !indicies.empty() ? &indicies : nullptr ) );
                      if ( rt_Success == surfaceFitter->FitSurface( params, results ) ) {
                        TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, results.GetSurface(), false );
                        TestVariables::viewManager->RefreshModel();
                        const size_t bufLen = 1024;
                        TCHAR buf[bufLen];
                        _sntprintf( buf, bufLen, _T( "Maximum deviation:  %e" ), results.GetTolerance() );
                        ::MessageBoxEx( buf, IDS_RESULTS );
                      }
                      else
                        ::PutErrorMessage( rt_Error, IDS_TITLE_ERR );
                      SetWaitCursor( false );
                    }
                    else
                      MessageBoxEx( RT_PARAMETER_ERROR, IDS_TITLE_ERR );                    
                  }
                }
              }
            }
          }
        }
        else
          MessageBoxEx( RT_PARAMETER_ERROR, IDS_TITLE_ERR );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Вписать поверхность в множество полигонов триангуляции с Z ограничением.
// Fit the surface to polygon set with Z constraint.
// ---
void FitSurfaceToGridZ()
{
  MbItem * obj( nullptr );
  PathMatrix solidPathFrom;
  if ( GetGObj( IDS_SELECT_MESH, st_Mesh, &obj, solidPathFrom ) &&
       obj->IsA() == st_Mesh ) {
    MbMesh * mesh = static_cast<MbMesh *> ( obj );
    const auto gridsCount = mesh->GridsCount();
    const MbGrid * pGrid( nullptr );
    if ( gridsCount == 1 )
      pGrid = mesh->GetGrid( 0 );
    else {
      ptrdiff_t gridId( 0 );
      if ( GetInt( IDS_ENTER_NUMBER_N, gridId ) && 0 <= gridId && size_t( gridId ) < mesh->GridsCount() )
        pGrid = mesh->GetGrid( gridId );
    }

    if ( pGrid != nullptr ) {
      double tolerance = DELTA_MIN;
      ptrdiff_t type( -1 );
      if ( GetInt( IDS_ENTER_SURFACE_TYPE_CONSTRAINED, type ) && GetDouble( IDS_ENTER_PRECISION, tolerance ) ) {
        MbeSpaceType surfType = st_Undefined;
        switch ( type ) {
        case 0: surfType = st_Plane;           break;
        case 1: surfType = st_CylinderSurface; break;
        case 2: surfType = st_SphereSurface;   break;
        case 3: surfType = st_ConeSurface;     break;
        case 4: surfType = st_TorusSurface;    break;
        default: break;
        }
        if ( surfType != st_Undefined ) {
          double angleConeMin( 1. );
          double angleConeMax( 89. );
          double radiusAnalyticShapeMax( pGrid->GetCube().GetDiagonal() * 10 );
          if ( GetGObj( IDS_SELECT_PLANE, st_Plane, &obj, solidPathFrom ) ) {
            if ( obj->IsA() == st_SpaceInstance ) {
              MbSpaceInstance * spInst = static_cast<MbSpaceInstance *>( obj );
              if ( spInst->GetSpaceItem() != nullptr ) {
                MbSpaceItem * spItem = static_cast<MbSpaceItem *>( spInst->SetSpaceItem() );
                if ( spItem->IsA() == st_Plane ) {
                  MbPlane * plane = static_cast<MbPlane *>( spItem );
                  MbPlacement3D place( plane->GetPlacement() );
                  IndicesVector indicies;
                  MbSurfaceFitToGridParameters params( surfType, tolerance, indicies );
                  params.SetAnalyticBounds( angleConeMin, angleConeMax, radiusAnalyticShapeMax );
                  MbSurfaceFitToGridResults results;
                  MbConstraintFixedValues fixedValues;
                  double x( 0. ), y( 0. ), phi( 0. ), theta( 0. ), size1( -1. ), size2( -1. );
                  if ( ( surfType == st_SphereSurface && GetDouble( IDS_ENTER_SPHERE_X_COORDINATE, x ) && GetDouble( IDS_ENTER_SPHERE_Y_COORDINATE, y ) ||
                         surfType == st_Plane && GetDouble( IDS_ENTER_PHI_PARAMETER, phi ) && GetDouble( IDS_ENTER_THETA_PARAMETER, theta ) ||
                         ( surfType != st_SphereSurface && surfType != st_Plane &&
                           GetDouble( IDS_ENTER_AXIS_X_COORDINATE, x ) && GetDouble( IDS_ENTER_AXIS_Y_COORDINATE, y ) &&
                           GetDouble( IDS_ENTER_PHI_PARAMETER, phi ) && GetDouble( IDS_ENTER_THETA_PARAMETER, theta ) ) ) &&
                       GetDouble( IDS_ENTER_CONSTRAINT_SIZE1, size1 ) &&
                       GetDouble( IDS_ENTER_CONSTRAINT_SIZE2, size2 ) ) {
                    if ( surfType == st_SphereSurface ) {
                      fixedValues._x = x;
                      fixedValues._y = y;
                    }
                    else if ( surfType == st_Plane ) {
                      fixedValues._phi = phi * M_DEGRAD;
                      fixedValues._theta = theta * M_DEGRAD;
                    }
                    else {
                      fixedValues._x = x;
                      fixedValues._y = y;
                      fixedValues._phi = phi * M_DEGRAD;
                      fixedValues._theta = theta * M_DEGRAD;
                    }
                    if ( size1 > EPSILON )
                      fixedValues._size1 = size1;
                    if ( size2 > EPSILON )
                      fixedValues._size2 = size2;
                    MbeDimensionalConstraintType dimType = MbeDimensionalConstraintType::none;
                    if ( size1 > EPSILON && size2 > EPSILON )
                      dimType = MbeDimensionalConstraintType::both;
                    else if ( size1 > EPSILON )
                      dimType = MbeDimensionalConstraintType::first;
                    else if ( size2 > EPSILON )
                      dimType = MbeDimensionalConstraintType::second;
                    if ( params.SetFitConstraintZ( surfType, dimType, fixedValues, place ) == rt_Success ) {
                      SetWaitCursor( true );
                      std::unique_ptr<MbSurfaceFitter> surfaceFitter( MbSurfaceFitter::Create( *pGrid, !indicies.empty() ? &indicies : nullptr ) );
                      if ( rt_Success == surfaceFitter->FitSurface( params, results ) ) {
                        TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, results.GetSurface(), false );
                        TestVariables::viewManager->RefreshModel();
                        const size_t bufLen = 1024;
                        TCHAR buf[bufLen];
                        _sntprintf( buf, bufLen, _T( "Maximum deviation:  %e" ), results.GetTolerance() );
                        ::MessageBoxEx( buf, IDS_RESULTS );
                      }
                      else
                        ::PutErrorMessage( rt_Error, IDS_TITLE_ERR );
                      SetWaitCursor( false );
                    }
                    else
                      MessageBoxEx( RT_PARAMETER_ERROR, IDS_TITLE_ERR );
                  }
                }
              }
            }
          }
        }
        else
          MessageBoxEx( RT_PARAMETER_ERROR, IDS_TITLE_ERR );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Вписать поверхность в множество полигонов триангуляции с XYZ ограничением.
// Fit the surface to polygon set with XYZ constraint.
// ---
void FitSurfaceToGridXYZ()
{
  MbItem * obj( nullptr );
  PathMatrix solidPathFrom;
  if ( GetGObj( IDS_SELECT_MESH, st_Mesh, &obj, solidPathFrom ) &&
       obj->IsA() == st_Mesh ) {
    MbMesh * mesh = static_cast<MbMesh *> ( obj );
    const auto gridsCount = mesh->GridsCount();
    const MbGrid * pGrid( nullptr );
    if ( gridsCount == 1 )
      pGrid = mesh->GetGrid( 0 );
    else {
      ptrdiff_t gridId( 0 );
      if ( GetInt( IDS_ENTER_NUMBER_N, gridId ) && 0 <= gridId && size_t( gridId ) < mesh->GridsCount() )
        pGrid = mesh->GetGrid( gridId );
    }

    if ( pGrid != nullptr ) {
      double tolerance = DELTA_MIN;
      ptrdiff_t type( -1 );
      if ( GetInt( IDS_ENTER_SURFACE_TYPE_CONSTRAINED, type ) && GetDouble( IDS_ENTER_PRECISION, tolerance ) ) {
        MbeSpaceType surfType = st_Undefined;
        switch ( type ) {
        case 0: surfType = st_Plane;           break;
        case 1: surfType = st_CylinderSurface; break;
        case 2: surfType = st_SphereSurface;   break;
        case 3: surfType = st_ConeSurface;     break;
        case 4: surfType = st_TorusSurface;    break;
        default: break;
        }
        if ( surfType != st_Undefined ) {
          double angleConeMin( 1. );
          double angleConeMax( 89. );
          double radiusAnalyticShapeMax( pGrid->GetCube().GetDiagonal() * 10 );
          if ( GetGObj( IDS_SELECT_PLANE, st_Plane, &obj, solidPathFrom ) ) {
            if ( obj->IsA() == st_SpaceInstance ) {
              MbSpaceInstance * spInst = static_cast<MbSpaceInstance *>( obj );
              if ( spInst->GetSpaceItem() != nullptr ) {
                MbSpaceItem * spItem = static_cast<MbSpaceItem *>( spInst->SetSpaceItem() );
                if ( spItem->IsA() == st_Plane ) {
                  MbPlane * plane = static_cast<MbPlane *>( spItem );
                  MbPlacement3D place( plane->GetPlacement() );
                  IndicesVector indicies;
                  MbSurfaceFitToGridParameters params( surfType, tolerance, indicies );
                  params.SetAnalyticBounds( angleConeMin, angleConeMax, radiusAnalyticShapeMax );
                  MbSurfaceFitToGridResults results;
                  MbConstraintFixedValues fixedValues;
                  double phi( 0. ), theta( 0. ), size1( -1. ), size2( -1. );
                  if ( ( surfType == st_SphereSurface ||
                         GetDouble( IDS_ENTER_PHI_PARAMETER, phi ) && GetDouble( IDS_ENTER_THETA_PARAMETER, theta ) ) &&
                       GetDouble( IDS_ENTER_CONSTRAINT_SIZE1, size1 ) &&
                       GetDouble( IDS_ENTER_CONSTRAINT_SIZE2, size2 ) ) {
                    if ( surfType != st_SphereSurface ) {
                      fixedValues._phi = phi * M_DEGRAD;
                      fixedValues._theta = theta * M_DEGRAD;
                    }
                    if ( size1 > EPSILON )
                      fixedValues._size1 = size1;
                    if ( size2 > EPSILON )
                      fixedValues._size2 = size2;
                    MbeDimensionalConstraintType dimType = MbeDimensionalConstraintType::none;
                    if ( size1 > EPSILON && size2 > EPSILON )
                      dimType = MbeDimensionalConstraintType::both;
                    else if ( size1 > EPSILON )
                      dimType = MbeDimensionalConstraintType::first;
                    else if ( size2 > EPSILON )
                      dimType = MbeDimensionalConstraintType::second;
                    if ( params.SetFitConstraintXYZ( surfType, dimType, fixedValues, place ) == rt_Success ) {
                      SetWaitCursor( true );
                      std::unique_ptr<MbSurfaceFitter> surfaceFitter( MbSurfaceFitter::Create( *pGrid, !indicies.empty() ? &indicies : nullptr ) );
                      if ( rt_Success == surfaceFitter->FitSurface( params, results ) ) {
                        TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, results.GetSurface(), false );
                        TestVariables::viewManager->RefreshModel();
                        const size_t bufLen = 1024;
                        TCHAR buf[bufLen];
                        _sntprintf( buf, bufLen, _T( "Maximum deviation:  %e" ), results.GetTolerance() );
                        ::MessageBoxEx( buf, IDS_RESULTS );
                      }
                      else
                        ::PutErrorMessage( rt_Error, IDS_TITLE_ERR );
                      SetWaitCursor( false );
                    }
                    else
                      MessageBoxEx( RT_PARAMETER_ERROR, IDS_TITLE_ERR );
                  }
                }
              }
            }
          }
        }
        else
          MessageBoxEx( RT_PARAMETER_ERROR, IDS_TITLE_ERR );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Вписать поверхность в множество полигонов триангуляции с ZW ограничением.
// Fit the surface to polygon set with ZW constraint.
// ---
void FitSurfaceToGridZW()
{
  MbItem * obj( nullptr );
  PathMatrix solidPathFrom;
  if ( GetGObj( IDS_SELECT_MESH, st_Mesh, &obj, solidPathFrom ) &&
       obj->IsA() == st_Mesh ) {
    MbMesh * mesh = static_cast<MbMesh *> ( obj );
    const auto gridsCount = mesh->GridsCount();
    const MbGrid * pGrid( nullptr );
    if ( gridsCount == 1 )
      pGrid = mesh->GetGrid( 0 );
    else {
      ptrdiff_t gridId( 0 );
      if ( GetInt( IDS_ENTER_NUMBER_N, gridId ) && 0 <= gridId && size_t( gridId ) < mesh->GridsCount() )
        pGrid = mesh->GetGrid( gridId );
    }

    if ( pGrid != nullptr ) {
      double tolerance = DELTA_MIN;
      ptrdiff_t type( -1 );
      if ( GetInt( IDS_ENTER_SURFACE_TYPE_CONSTRAINED, type ) && GetDouble( IDS_ENTER_PRECISION, tolerance ) ) {
        MbeSpaceType surfType = st_Undefined;
        switch ( type ) {
        case 0: surfType = st_Plane;           break;
        case 1: surfType = st_CylinderSurface; break;
        case 2: surfType = st_SphereSurface;   break;
        case 3: surfType = st_ConeSurface;     break;
        case 4: surfType = st_TorusSurface;    break;
        default: break;
        }
        if ( surfType != st_Undefined ) {
          double angleConeMin( 1. );
          double angleConeMax( 89. );
          double radiusAnalyticShapeMax( pGrid->GetCube().GetDiagonal() * 10 );
          if ( GetGObj( IDS_SELECT_PLANE, st_Plane, &obj, solidPathFrom ) ) {
            if ( obj->IsA() == st_SpaceInstance ) {
              MbSpaceInstance * spInst = static_cast<MbSpaceInstance *>( obj );
              if ( spInst->GetSpaceItem() != nullptr ) {
                MbSpaceItem * spItem = static_cast<MbSpaceItem *>( spInst->SetSpaceItem() );
                if ( spItem->IsA() == st_Plane ) {
                  MbPlane * plane = static_cast<MbPlane *>( spItem );
                  MbPlacement3D place( plane->GetPlacement() );
                  IndicesVector indicies;
                  MbSurfaceFitToGridParameters params( surfType, tolerance, indicies );
                  params.SetAnalyticBounds( angleConeMin, angleConeMax, radiusAnalyticShapeMax );
                  MbSurfaceFitToGridResults results;
                  MbConstraintFixedValues fixedValues;
                  double phi( 0. ), theta( 0. ), dist( -1. ), size1( -1. ), size2( -1. );
                  if ( ( surfType == st_SphereSurface ||
                         GetDouble( IDS_ENTER_PHI_PARAMETER, phi ) && GetDouble( IDS_ENTER_THETA_PARAMETER, theta ) ) &&
                       GetDouble( IDS_ENTER_DIST_PARAMETER, dist ) &&
                       GetDouble( IDS_ENTER_CONSTRAINT_SIZE1, size1 ) &&
                       GetDouble( IDS_ENTER_CONSTRAINT_SIZE2, size2 ) ) {
                    if ( surfType != st_SphereSurface ) {
                      fixedValues._phi = phi * M_DEGRAD;
                      fixedValues._theta = theta * M_DEGRAD;
                    }
                    if ( dist > EPSILON )
                      fixedValues._dist = dist;
                    if ( size1 > EPSILON )
                      fixedValues._size1 = size1;
                    if ( size2 > EPSILON )
                      fixedValues._size2 = size2;
                    MbeDimensionalConstraintType dimType = MbeDimensionalConstraintType::none;
                    if ( size1 > EPSILON && size2 > EPSILON )
                      dimType = MbeDimensionalConstraintType::both;
                    else if ( size1 > EPSILON )
                      dimType = MbeDimensionalConstraintType::first;
                    else if ( size2 > EPSILON )
                      dimType = MbeDimensionalConstraintType::second;
                    if ( params.SetFitConstraintZW( surfType, dimType, fixedValues, place ) == rt_Success ) {
                      SetWaitCursor( true );
                      std::unique_ptr<MbSurfaceFitter> surfaceFitter( MbSurfaceFitter::Create( *pGrid, !indicies.empty() ? &indicies : nullptr ) );
                      if ( rt_Success == surfaceFitter->FitSurface( params, results ) ) {
                        TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, results.GetSurface(), false );
                        TestVariables::viewManager->RefreshModel();
                        const size_t bufLen = 1024;
                        TCHAR buf[bufLen];
                        _sntprintf( buf, bufLen, _T( "Maximum deviation:  %e" ), results.GetTolerance() );
                        ::MessageBoxEx( buf, IDS_RESULTS );
                      }
                      else
                        ::PutErrorMessage( rt_Error, IDS_TITLE_ERR );
                      SetWaitCursor( false );
                    }
                    else
                      MessageBoxEx( RT_PARAMETER_ERROR, IDS_TITLE_ERR );
                  }
                }
              }
            }
          }
        }
        else
          MessageBoxEx( RT_PARAMETER_ERROR, IDS_TITLE_ERR );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Вписать поверхность в множество полигонов триангуляции с ограничениями.
// Fit the surface to polygon set with constraints.
// ---
void FitSurfaceToGridWithConstraints()
{
  MbItem * obj( nullptr );
  PathMatrix solidPathFrom;
  if ( GetGObj( IDS_SELECT_MESH, st_Mesh, &obj, solidPathFrom ) &&
       obj->IsA() == st_Mesh ) {
    MbMesh * mesh = static_cast<MbMesh *> ( obj );
    const auto gridsCount = mesh->GridsCount();
    const MbGrid * pGrid( nullptr );
    if ( gridsCount == 1 )
      pGrid = mesh->GetGrid( 0 );
    else {
      ptrdiff_t gridId( 0 );
      if ( GetInt( IDS_ENTER_NUMBER_N, gridId ) && 0 <= gridId && size_t( gridId ) < mesh->GridsCount() )
        pGrid = mesh->GetGrid( gridId );
    }

    if ( pGrid != nullptr ) {
      double tolerance = DELTA_MIN;
      ptrdiff_t type( -1 );
      if ( GetInt( IDS_ENTER_SURFACE_TYPE_CONSTRAINED, type ) && GetDouble( IDS_ENTER_PRECISION, tolerance ) ) {
        MbeSpaceType surfType = st_Undefined;
        switch ( type ) {
          case 0: surfType = st_Plane;           break;
          case 1: surfType = st_CylinderSurface; break;
          case 2: surfType = st_SphereSurface;   break;
          case 3: surfType = st_ConeSurface;     break;
          case 4: surfType = st_TorusSurface;    break;
          default: break;
        }
        if ( surfType != st_Undefined ) {
          double angleConeMin( 1. );
          double angleConeMax( 89. );
          double radiusAnalyticShapeMax( pGrid->GetCube().GetDiagonal() * 10 );
          if ( GetGObj( IDS_SELECT_PLANE, st_Plane, &obj, solidPathFrom ) ) {
            if ( obj->IsA() == st_SpaceInstance ) {
              MbSpaceInstance * spInst = static_cast<MbSpaceInstance *>( obj );
              if ( spInst->GetSpaceItem() != nullptr ) {
                MbSpaceItem * spItem = static_cast<MbSpaceItem *>( spInst->SetSpaceItem() );
                if ( spItem->IsA() == st_Plane ) {
                  MbPlane * plane = static_cast<MbPlane *>( spItem );
                  const MbPlacement3D place( plane->GetPlacement() );
                  IndicesVector indicies;
                  MbSurfaceFitToGridParameters params( surfType, tolerance, indicies );
                  params.SetAnalyticBounds( angleConeMin, angleConeMax, radiusAnalyticShapeMax );
                  double theta( 0. ), size1( 0. ), size2( 0. );
                  if ( surfType == st_Plane && GetDouble( IDS_ENTER_THETA_PARAMETER, theta ) ||
                       surfType == st_ConeSurface && GetDouble( IDS_ENTER_THETA_PARAMETER, theta ) && GetDouble( IDS_ENTER_CONE_ANGLE, size1 ) ||
                       surfType == st_CylinderSurface && GetDouble( IDS_ENTER_THETA_PARAMETER, theta ) && GetDouble( IDS_ENTER_CYLINDER_RADIUS, size1 ) ||
                       surfType == st_SphereSurface && GetDouble( IDS_ENTER_SPHERE_RADIUS, size1 ) ||
                       ( surfType == st_TorusSurface && GetDouble( IDS_ENTER_THETA_PARAMETER, theta ) &&
                         GetDouble( IDS_ENTER_TORUS_RAD_BG, size1 ) && GetDouble( IDS_ENTER_TORUS_RAD_SM, size2 ) ) ) {
                    if ( surfType != st_SphereSurface )
                      params.AddAxisConstraint( place.GetAxisZ(), theta *= M_DEGRAD );
                    switch ( surfType ) {
                      case st_ConeSurface:     params.AddConeAngleConstraint( size1 *= M_DEGRAD ); break;
                      case st_CylinderSurface: params.AddCylinderRadiusConstraint( size1 );        break;
                      case st_SphereSurface:   params.AddSphereRadiusConstraint( size1 );          break;
                      case st_TorusSurface:    params.AddTorusRadiiConstraint( size1, size2 );     break;
                      default:                 break;
                    }
                    SetWaitCursor( true );
                    MbSurfaceFitToGridResults results;
                    std::unique_ptr<MbSurfaceFitter> surfaceFitter( MbSurfaceFitter::Create( *pGrid, !indicies.empty() ? &indicies : nullptr ) );
                    if ( rt_Success == surfaceFitter->FitSurface( params, results ) ) {
                      TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, results.GetSurface(), false );
                      TestVariables::viewManager->RefreshModel();
                      const size_t bufLen = 1024;
                      TCHAR buf[bufLen];
                      _sntprintf( buf, bufLen, _T( "Maximum deviation:  %e" ), results.GetTolerance() );
                      ::MessageBoxEx( buf, IDS_RESULTS );
                    }
                    else
                      ::PutErrorMessage( rt_Error, IDS_TITLE_ERR );
                    SetWaitCursor( false );
                  }
                }
              }
            }
          }
        }
        else
          MessageBoxEx( RT_PARAMETER_ERROR, IDS_TITLE_ERR );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создание разрезанной полигональной модели.
// Cut a mesh model off by a plane.
// ---
void CutMeshByPlane()
{
  MbItem * obj;
  PathMatrix surfacePathFrom;

  if ( GetGObj( IDS_SELECT_PLANE, st_Plane, &obj, surfacePathFrom ) ) {
    TestVariables::viewManager->ToggleObject( surfacePathFrom.first );
    bool res = false;
    MbPlacement3D place;
    if ( obj->IsA() == st_SpaceInstance ) {
      MbSpaceInstance * sInst = static_cast<MbSpaceInstance *>(obj);
      if ( sInst->GetSpaceItem() != nullptr ) {
        const MbSpaceItem * sItem = sInst->GetSpaceItem();
        if ( sItem->IsA() == st_Plane ) {
          const MbPlane * sPlane = static_cast<const MbPlane *>(sItem);
          place.Init( sPlane->GetPlacement() );
          if ( !surfacePathFrom.second.IsSingle() ) {
            place.Transform( surfacePathFrom.second );
          }
          res = true;
        }
      }
    }

    if ( res /*&& GetInt( IDS_ENTER_NUMBER_N, TestVariables::part )*/ ) {
      WorkWindow * wind = TestVariables::viewManager->GetActiveWindow(); // Вернуть указатель на активное окно.

      if ( wind != nullptr ) {
        MbAccurateTimer at;

        SetWaitCursor( true );

        at.Begin();
        SPtr<MbModel> cutModel( wind->GetGeomModel().CutMeshModel( place, 0.0 ) );
        at.End();

        if ( cutModel != nullptr ) {
          TestVariables::viewManager->ReplaceModel( *cutModel );
        }

        SetWaitCursor( false );

        if ( TestVariables::checkOperationTime ) {
          const size_t bufLen = 1024;
          TCHAR buf[bufLen];
          size_t len = 0;
          len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, at.GetLast() );
          ::MessageBoxEx( buf, IDS_RESULTS );
        }
      }
    }
  }
} // CutMeshByPlane


//------------------------------------------------------------------------------
// Создание разрезанной полигональной модели.
// Cut a mesh object by a plane.
// ---
void CutMeshByLine( bool poly )
{
  bool cutterIsReady = false;
  MbPlacement3D cutterPlace;
  {
    MbCartPoint3D points[2];
    MbVector3D to;

    if ( GetPoint( IDS_SELECT_START_POINT, points[0] ) ) {
      if ( GetPoint( IDS_SELECT_FINAL_POINT, points[1] ) ) {
        const WorkWindow * vc = TestVariables::viewManager->GetActiveWindow();

        MbVector3D axisX( points[0], points[1] ), axisY( -vc->GetNullPlacement().GetAxisZ() );
        MbPlacement3D place( axisX, axisY, points[0] );

        cutterPlace.Init( place );
        cutterIsReady = true;
      }
    }
  }

  if ( !cutterIsReady ) {
    PathMatrix pathFrom;
    SurfaceSPtr surface( SelectSurface( &pathFrom.second ) );

    if ( surface != nullptr && surface->GetSurface().IsA() == st_Plane ) {
      const MbPlane & plane = static_cast<const MbPlane &>(surface->GetSurface());
      cutterPlace.Init( plane.GetPlacement() );

      if ( !pathFrom.second.IsSingle() ) {
        cutterPlace.Transform( pathFrom.second );
      }
      cutterIsReady = true;
    }
  }

  if ( cutterIsReady ) {
    MbItem * obj = nullptr;
    PathMatrix pathFrom;

    if ( GetGObj( IDS_SELECT_MESH, st_Mesh, &obj, pathFrom ) && (obj->IsA() == st_Mesh) )
    {
      C3D_ASSERT( pathFrom.second.IsSingle() ); // Нужно учитывать матрицу ЛСК!!!

      MbResultType res = rt_Error;
      Style st( 1, obj->GetColor() );

      MbAccurateTimer at;

      //TestVariables::viewManager->ToggleObject( pathFrom.first );

      MbMesh * mesh = static_cast<MbMesh *>( obj );

      if ( !poly ) { // Создание разрезанной полигональной модели. Cut a mesh object by a plane.
        if ( GetInt( IDS_ENTER_NUMBER_N, TestVariables::part ) ) {
          bool onlySection = false;
          if ( TestVariables::part == 0 )
            onlySection = true;

          MbMesh * result( nullptr );
          MbSNameMaker names( (SimpleName)ct_CuttingSolid, MbSNameMaker::i_SideNone, 0 );
          MbeCopyMode sameShell = cm_Copy;

          SetWaitCursor( true );
          at.Begin();
          res = ::MeshCutting( *mesh, sameShell, cutterPlace, TestVariables::part, names, onlySection, result );
          at.End();
          SetWaitCursor( false );

          //TestVariables::viewManager->ToggleObject( pathFrom.first );

          if ( result != nullptr ) {
            SetWaitCursor( true );
            TestVariables::viewManager->DeleteObject( obj );
            TestVariables::viewManager->AddObject( st, result );
            SetWaitCursor( false );
          }
          else {
            ::PutErrorMessage( res, IDS_TITLE_ERR );
          }
        }
      }
      else { // Создание контура сечения полигонального объекта. Cut a section contour of mesh object by a line.
        SpaceCurvesSPtrVector polylines;

        SetWaitCursor( true );
        at.Begin();
        res = ::MeshSection( *mesh, cutterPlace, polylines );
        at.End();
        SetWaitCursor( false );

        //TestVariables::viewManager->ToggleObject( pathFrom.first );

        if ( res == rt_Success ) {
          Style st2( 2, RGB2uint32( 1.0, 0.0, 0.0 ) );

          SetWaitCursor( true );
          for( size_t i = 0, iCount = polylines.size(); i < iCount; i++ ) {
            TestVariables::viewManager->AddObject( st2, polylines[i] );
          }
          SetWaitCursor( false );
        }
        else {
          ::PutErrorMessage( res, IDS_TITLE_ERR );
        }
      }

      if ( TestVariables::checkOperationTime ) {
        const size_t bufLen = 1024;
        TCHAR buf[bufLen];
        size_t len = 0;
        len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, at.GetLast() );
        ::MessageBoxEx( buf, IDS_RESULTS );
      }
    }
  }
} // CutMeshByLine


//------------------------------------------------------------------------------
// Выбрать и подсветить объект. Select and toggle an object.
// ---
static inline
MbItem * GetToggleObject( const TCHAR * nStr, MbeSpaceType sType, MbItem *& obj,
                          PathMatrix & pathFrom = TestVariables::itemPathFrom )
{
  if ( GetGObj( nStr, sType, &obj, pathFrom ) && obj != nullptr )
    TestVariables::viewManager->ToggleObject( obj );
  return obj;
}

//------------------------------------------------------------------------------
// Создание контуров пересечения двух полигональных объектов.
// Create contours of intersection of two polygonal objects.
// ---
void MeshMeshIntersection()
{
  MbItem * obj1 = nullptr, * obj2 = nullptr;

  PathMatrix solidPathFrom1, solidPathFrom2;

  if ( GetToggleObject( IDS_SELECT_MESH, st_Mesh, obj1, solidPathFrom1 ) != nullptr && obj1->IsA() == st_Mesh &&
       GetToggleObject( IDS_SELECT_MESH, st_Mesh, obj2, solidPathFrom2 ) != nullptr && obj2->IsA() == st_Mesh )
  {
    C3D_ASSERT( solidPathFrom1.second.IsSingle() && solidPathFrom2.second.IsSingle() ); // Нужно учитывать матрицу ЛСК!!!

    MbMesh * mesh1 = static_cast<MbMesh *> ( obj1 );
    MbMesh * mesh2 = static_cast<MbMesh *> ( obj2 );

    if ( mesh1 != nullptr && mesh2 != nullptr ) {
      MbAccurateTimer at;

      SpaceCurvesSPtrVector polylines;

      at.Begin();
      MbResultType res = ::MeshMeshIntersection( *mesh1, *mesh2, polylines );
      at.End();

      if ( res == rt_Success && polylines.size() > 0 ) {
        Style st2( 2, RGB2uint32( 1.0, 0.0, 0.0 ) );
        for ( size_t i = 0, iCount = polylines.size(); i < iCount; i++ ) {
          TestVariables::viewManager->AddObject( st2, polylines[i] );
        }
      }
      else if ( res == rt_NoIntersect ) {
        ::PutErrorMessage( res, ERR_NOINTERSECT );
      }
      else {
        ::PutErrorMessage( res, RT_CONTOUR_CREATE_ERROR );
      }

      if ( TestVariables::checkOperationTime ) {
        const size_t bufLen = 1024;
        TCHAR buf[bufLen];
        size_t len = 0;
        len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, at.GetLast() );
        ::MessageBoxEx( buf, IDS_RESULTS );
      }
    }

    TestVariables::viewManager->ToggleObject( obj1 );
    TestVariables::viewManager->ToggleObject( obj2 );
  }
}


//------------------------------------------------------------------------------
//  Исправить несогласованную ориентацию сетки.
//  Repair inconsistent orientation.
// ---
void RepairInconsistentMesh()
{
  MbItem * obj = nullptr;
  if ( TestVariables::viewManager->GetActiveWindow()->GetMeshProcessor() == nullptr ) {
    if ( GetObj(IDS_SELECT_GEOM_OBJECT, &obj) ) {
      int orientOutwards = 0;
      double eps = METRIC_PRECISION;
      double angSharp = M_PI_4;
     
      if ( GetInt( IDS_POLYGONAL_ORIENT_NORMALS_OUTWARD, orientOutwards ) &&
           GetDouble( IDS_ENTER_LEN_TOLERANCE, eps ) &&
           GetDouble( IDS_ENTER_ANGLE, angSharp ) )
      {
        bool bNmAveraging = GetBool( IDS_POLYGONAL_NORMAL_AVERAGING, true );
        TestVariables::viewManager->ToggleObject( obj );
        SPtr<MbMesh> mesh;
        SetWaitCursor( true );
        if ( obj->IsA() == st_Mesh ) {
          mesh = &static_cast<MbMesh &>(*obj);
          RepairInconsistentMesh( MbRepairInconsistentMeshParams( eps, orientOutwards > 0, angSharp, bNmAveraging ), *mesh );
          TestVariables::viewManager->RefreshModel();
        }
        SetWaitCursor( false );
        TestVariables::viewManager->ToggleObject( obj );
      }
    }
  }
}

//------------------------------------------------------------------------------
// Вывернуть полигональный объект.
// Reverse a polygonal object.
// ---
void ReverseMesh()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_MESH, st_Mesh, &obj ) ) {
    TestVariables::viewManager->ToggleObject( obj );

    SetWaitCursor( true );

    MbMesh * mesh = static_cast<MbMesh *>( obj );
    mesh->Reverse();

    MessageBoxEx( IDS_DONE, IDS_TITLE_MSG );
    SetWaitCursor( false );
    TestVariables::viewManager->ToggleObject( obj );
    TestVariables::viewManager->RedrawObject( obj );
  }
}

//------------------------------------------------------------------------------
// Удалить полигональный объект, оставив базовые объекты.
// Remove the polygonal object, leaving the base objects.
// ---
bool RemoveMeshBasis()
{
  MbItem * nearest = nullptr;

  if ( GetGObj( IDS_SELECT_MESH, st_Mesh, &nearest ) ) {
    TestVariables::viewManager->ToggleObject( nearest );
    SetWaitCursor( true );

    RPArray<MbSpaceItem> s( 0, 1 );
    ( (MbMesh *)nearest )->GetBasisItems( s );
    ::AddRefItems( s );

    ptrdiff_t n = s.Count();
    TestVariables::viewManager->ToggleObject( nearest );

    if ( n > 0 ) {
      Style ns( nearest->GetWidth(), nearest->GetColor() );
      TestVariables::viewManager->DeleteObject( nearest ); // Удаление элемента.  // Delete element.
      for ( ptrdiff_t i = 0; i < n; i++ ) {
        Style st = ns;
        switch ( i ) {
          case 0: st = ns; break;
          case 1: st = TestVariables::ELEMENTARY_Style; break;
          case 2: st = TestVariables::BOOLEAN_Style; break;
          default: st = TestVariables::SOLID_Style; break;
        }
        TestVariables::viewManager->AddObject( st, s[i] );
      }
    }

    ::ReleaseItems( s );

    SetWaitCursor( false );

    return true;
  }

  return false;
}

//------------------------------------------------------------------------------
// Выполнить операцию объединения/пересечения/вычитания для двух полигональных объектов.
// Perform a Boolean operation on two polygonal objects.
// ---
void MeshBoolean( OperationType op )
{
  MbItem * obj1 = nullptr, * obj2 = nullptr;
  PathMatrix solidPathFrom1, solidPathFrom2;
  if ( GetToggleObject( IDS_SELECT_MESH, st_Mesh, obj1, solidPathFrom1 ) != nullptr &&
       obj1->IsA() == st_Mesh &&
       GetToggleObject( IDS_SELECT_MESH, st_Mesh, obj2, solidPathFrom2 ) != nullptr &&
       obj2->IsA() == st_Mesh ) {
    MbMesh * mesh1 = static_cast<MbMesh *> ( obj1 );
    MbMesh * mesh2 = static_cast<MbMesh *> ( obj2 );
    if ( mesh1 != nullptr && mesh2 != nullptr ) {
      Style st1( 1, obj1->GetColor() );
      Style st2( 2, obj2->GetColor() );
      SPtr<MbMesh> newMesh = nullptr;
      MbAccurateTimer ac;

      SetWaitCursor( true );
      ac.Begin();
      MbResultType res = ::CreateBoolean( *mesh1, *mesh2, op, newMesh );
      ac.End();
      SetWaitCursor( false );

      if ( res == rt_Success && newMesh != nullptr ) {
        TestVariables::viewManager->DeleteObject( obj1 );
        TestVariables::viewManager->DeleteObject( obj2 );

        TestVariables::viewManager->AddObject( st1, newMesh );
        TestVariables::viewManager->ToggleObject( newMesh );
      }
      else {
        ::PutErrorMessage( res, _T( "Operation error" ) );
        TestVariables::viewManager->ToggleObject( obj1 );
        TestVariables::viewManager->ToggleObject( obj2 );
      }

      if ( TestVariables::checkOperationTime ) {
        const size_t bufLen = 1024;
        TCHAR buf[bufLen];
        _sntprintf( buf, bufLen, RT_TIME_SEC1, ac.GetLast() );
        ::MessageBoxEx( buf, IDS_RESULTS );
      }
    }
  }
}

//------------------------------------------------------------------------------
// Перестроить полигональный объект. Rebuild a polygonal object.
// ---
void RebuildMesh()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_MESH, st_Mesh, &obj ) ) {
    TestVariables::viewManager->ToggleObject( obj );

    MbAccurateTimer ac;
    SetWaitCursor( true );
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    if ( wind != nullptr )
      wind->EraseObject( obj );

    RPArray<MbSpaceItem> spaceItems( 0, 1 );
    MbeCopyMode copyMode = cm_Copy;

    SetWaitCursor( true );
    ac.Begin();
    bool rebuildRes = obj->RebuildItem( copyMode, &spaceItems, nullptr );
    ac.End();
    SetWaitCursor( false );

    if( !rebuildRes )
      ::PutErrorMessage( 1, _T( "Operation error" ) );
    else {
      if ( TestVariables::checkOperationTime ) {
        const size_t bufLen = 1024;
        TCHAR buf[bufLen];
        _sntprintf( buf, bufLen, RT_TIME_SEC1, ac.GetLast() );
        ::MessageBoxEx( buf, IDS_RESULTS );
      }
    }

    TestVariables::viewManager->RefreshObjectModel( obj );
  }
}


//------------------------------------------------------------------------------
// Нарисовать ребра.
// ---
void DrawMeshEdges( const MbMesh & mesh, std::vector<MeshInfo::Edges> & edges )
{
  for ( auto & edge : edges ) {
    const MbGrid * grid = mesh.GetGrid( edge.first );
    if ( grid != nullptr ) {
      MbCartPoint3D p1, p2;
      for ( auto & i : edge.second ) {
        grid->GetPoint( size_t( i._vert1._data ), p1 );
        grid->GetPoint( size_t( i._vert2._data ), p2 );
        TestVariables::drawImp->DrawLine( p1, p2, TRGB_BLACK );
        TestVariables::drawImp->DrawLine( p1, p2, TRGB_LIGHTRED );
        TestVariables::drawImp->DrawPoint( &p1, TRGB_LIGHTRED );
        TestVariables::drawImp->DrawPoint( &p2, TRGB_LIGHTRED );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Нарисовать ребра.
// ---
void DrawMeshFacets( const MbMesh & mesh, std::vector<MeshInfo::Facets> & facets )
{
  MbCartPoint3D p0, p1, p2, q;
  for ( auto & facet : facets ) {
    const MbGrid * grid = mesh.GetGrid( facet.first );
    if ( grid != nullptr ) {
      for ( auto & i : facet.second ) {
        if ( i.size() == 3 ) { // Треугольник.
          grid->GetPoint( i[0], p0 );
          grid->GetPoint( i[1], p1 );
          grid->GetPoint( i[2], p2 );
          TestVariables::drawImp->DrawLine( p0, p1, TRGB_ORANGE );
          TestVariables::drawImp->DrawLine( p1, p2, TRGB_ORANGE );
          TestVariables::drawImp->DrawLine( p2, p0, TRGB_ORANGE );
          q = ( p0 + p1 + p2 ) / 3;
          p0 = ( p0 + q ) * 0.5;
          p1 = ( p1 + q ) * 0.5;
          p2 = ( p2 + q ) * 0.5;
          TestVariables::drawImp->DrawLine( p0, q, TRGB_ORANGE );
          TestVariables::drawImp->DrawLine( p1, q, TRGB_ORANGE );
          TestVariables::drawImp->DrawLine( p2, q, TRGB_ORANGE );
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
// Проверить замкнутость полигонального объекта. 
// Check closeness of a polygonal object
// ---
void CheckMeshClosed()
{
  MbItem * obj = nullptr;

  if ( GetGObj(IDS_SELECT_MESH, st_Mesh, &obj) && obj->IsA() == st_Mesh ) {
    TestVariables::viewManager->ToggleObject( obj );
    MbMesh * mesh = static_cast<MbMesh *>( obj );
    MbAccurateTimer ac;
    MeshInfo info;

    SetWaitCursor( true );
    ac.Begin();
    bool closed = ::InspectMeshClosure( *mesh, info );
    ac.End();
    SetWaitCursor( false );

    TestVariables::viewManager->ToggleObject( obj );

    if ( closed ) {
      ::MessageBoxEx( RT_CLOSED_MESH, IDS_TITLE_DIAG_WRN );
    }
    else {
      ::MessageBoxEx( RT_OPEN_MESH, IDS_TITLE_DIAG_WRN );
      if ( info._edges.size() > 0 ) {
        ::MessageBoxEx( RT_BOUNDARY_EDGES_OPEN_MESH, IDS_TITLE_DIAG_WRN );
        ::DrawMeshEdges( *mesh, info._edges );
      }
      if ( info._facets.size() > 0 ) {
        ::MessageBoxEx( RT_BOUNDARY_POLY_OPEN_MESH, IDS_TITLE_DIAG_WRN );
        ::DrawMeshFacets( *mesh, info._facets );
      }
    }
    for ( size_t i = 0; i < info._states.size(); ++i ) {
      switch ( info._states[i] ) {
        case MeshInfo::es_DegeneratePoly: ::MessageBoxEx( RT_DEGENERATE_POLY_MESH, IDS_TITLE_DIAG_WRN ); break;
        case MeshInfo::es_InconsistPolys: ::MessageBoxEx( RT_INCONSINSTENT_POLY_MESH, IDS_TITLE_DIAG_WRN ); break;
        case MeshInfo::es_InconsistGrids: ::MessageBoxEx( RT_INCONSINSTENT_GRID_MESH, IDS_TITLE_DIAG_WRN ); break;
        default: {}
      }
    }

    if ( TestVariables::checkOperationTime ) {
      size_t len = 0;
      const size_t bufLen = 512;
      TCHAR buf[bufLen];
      len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC, ac.GetLast() );
      ::MessageBoxEx( buf, IDS_RESULTS );
    }
  }
}
