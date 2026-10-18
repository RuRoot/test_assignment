#include <test_window.h>
#include <test_gr_draw.h>
#include <mb_data.h>
#include <surface.h>
#include <space_instance.h>
#include <plane_instance.h>
#include <assembly.h>
#include <instance_item.h>
#include <cur_contour.h>
#include <cur_contour3d.h>
#include <point_frame.h>
#include <solid.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Нахождение сборки или вставки.
// ---
bool WorkWindow::FindRigidItem( MbeSpaceType sType, MbItem *& find, PathMatrix & pathFrom )
{
  bool res = false;

  if ( (find != nullptr) && (pathFrom.first.size() > 1) && (sType != find->IsA()) ) {
    MbPath path = pathFrom.first;
    size_t pCount = path.size();
    MbMatrix3D iFrom;
    path.RemoveInd( pCount - 1 );
    const MbItem * item = geomModel.GetItemByPath( path, iFrom );
    if ( item != nullptr && item->IsA() == sType ) {
      find = (MbItem *)item;
      pathFrom.first = path;
      pathFrom.second = iFrom;
      res = true;
    }
  }
  return res;
}


//------------------------------------------------------------------------------
// Нахождение объекта item заданного типа xType и матрицы from его преобразования в "мир", ближайшего к прямой line
// Флаг creationMode сделан для корректного создания и селектирования сборок: 
// когда сборка уже создана мы должны "заглядывать" на уровень глубже.
// ---
bool WorkWindow::FindGeometricItem( MbeSpaceType sType, MbeTopologyType tType, MbePlaneType pType,
                                    const MbFloatAxis3D & line, double maxDistance, 
                                    MbItem ** find, MbRefItem ** item, PathMatrix & pathFrom )
{
  const MbFloatPoint3D & p = line.GetOrigin();
  const MbFloatVector3D & v = line.GetAxisZ();

  MbCartPoint3D axisPnt( p.x, p.y, p.z );
  MbVector3D axisDir( v.x, v.y, v.z );
  axisDir.Invert(); // KYA: Нужно инвертировать, т.к. см работу фукнций WorkWindow::OrientateWindow и LineGridIntersect
  MbAxis3D axis( axisPnt, axisDir ); 

  *find = nullptr;
  SimpleName findName;
  *item = nullptr;
  SimpleName elementName;

  double distance = ScaleDistance( maxDistance );

  bool byGrid = (renderType == wwrt_OGL);

  if ( drawModel.NearestMesh( sType, tType, pType, axis, distance, byGrid,
                              *find, findName, *item, elementName, pathFrom.first, pathFrom.second ) ) 
  {
    if ( *find == nullptr )
      *find = const_cast<MbItem *>( geomModel.GetItemByPath( pathFrom.first, pathFrom.second ) );
    if ( sType == st_Assembly || sType == st_Instance )
      FindRigidItem( sType, *find, pathFrom );
  }
  return ( *find != nullptr );
}


//------------------------------------------------------------------------------
// Нахождение объекта item заданного типа xType и матрицы from его преобразования в "мир", ближайшего к прямой line
// ---
bool WorkWindow::FindNearestItem( MbeSpaceType sType, MbeTopologyType tType, 
                                  const MbFloatAxis3D & line, double maxDistance, 
                                  MbItem ** find, MbTopItem ** geom, MbMatrix3D & from_ )
{
  MbRefItem * item = nullptr;
  PathMatrix pathFrom;
  if ( FindGeometricItem( sType, tType, pt_Undefined, line, maxDistance, find, &item, pathFrom ) ) {
    if ( (item != nullptr) && (item->RefType() == rt_TopItem) )
      *geom = static_cast<MbTopItem *>(item);
    if ( *geom != nullptr && tType != tt_Undefined && tType != tt_TopItem ) {
      if ( (*geom)->IsA() != tType ) // Нашли не то, что запрашивали
        *geom = nullptr;
    }
  }
  from_.Init( pathFrom.second );
  return ( (*find != nullptr) && (*geom != nullptr) );
}


//------------------------------------------------------------------------------
// Нахождение объекта item заданного типа xType и матрицы from его преобразования в "мир", ближайшего к прямой line
// ---
bool WorkWindow::FindNearestItem( MbeSpaceType sType, MbeTopologyType tType,
                                  const MbFloatAxis3D & line, double maxDistance,
                                  MbItem ** find, MbTopItem ** geom, PathMatrix & pathFrom_ )
{
  MbRefItem * item = nullptr;
  PathMatrix pathFrom;
  if ( FindGeometricItem( sType, tType, pt_Undefined, line, maxDistance, find, &item, pathFrom ) ) {
    if ( (item != nullptr) && (item->RefType() == rt_TopItem) )
      *geom = static_cast<MbTopItem *>(item);
    if ( *geom != nullptr && tType != tt_Undefined && tType != tt_TopItem ) {
      if ( (*geom)->IsA() != tType ) // Нашли не то, что запрашивали
        *geom = nullptr;
    }
  }

  pathFrom_.first = pathFrom.first;
  pathFrom_.second = pathFrom.second;
  return ((*find != nullptr) && (*geom != nullptr));
}


//------------------------------------------------------------------------------
// Привязка к ближайшей точке объекта
// ---
bool WorkWindow::FindNearestSpacePoint( MbeSpaceType typeObj, 
                                        const MbFloatAxis3D & from_, double maxDistance, 
                                        const MbCartPoint3D & cursorPoint,
                                        MbCartPoint3D & pnt ) 
{
  MbItem * find = nullptr;
  MbRefItem * item = nullptr;

  PathMatrix pathFrom;
  if ( FindGeometricItem( typeObj, tt_Undefined, pt_Undefined, from_, maxDistance, &find, &item, pathFrom ) ) {
    const MbMatrix3D & matr = pathFrom.second;

    size_t index = SYS_MAX_T;
    double d, dx, dy, dmin = MB_MAXDOUBLE;
    MbMatrix3D into_(GetNullMatrixInto(false));
    MbCartPoint3D pp0, pp1(cursorPoint);

    if ( find->IsA() == st_PlaneInstance ) {
      MbControlData s2;
      MbCartPoint p2;
      s2.ReserveTotal( 10 );
      s2.ReserveShare( 10 );
      MbMatrix3D from__( ((MbPlaneInstance *)find)->GetPlacement().GetMatrixFrom() );
      find->GetBasisPoints( s2 ); // Дать точки
      pp1.Transform( into_ ); // Перевод в экранную СК
      for ( size_t j = 0, jCnt = s2.Count(); j < jCnt; j++ ) {
        s2.GetPoint( j, p2 );
        pp0.x = p2.x;   pp0.y = p2.y;   pp0.z = 0;
        pp0.Transform( from__ );
        if ( !matr.IsSingle() )
          pp0.Transform( matr );
        pp0.Transform( into_ ); // Перевод в экранную СК
        dx = pp0.x-pp1.x;
        dy = pp0.y-pp1.y;
        d = dx*dx + dy*dy;
        if ( d<dmin ) {
          dmin = d;
          index = j;
        }
      }
      if ( index != SYS_MAX_T ) {
        s2.GetPoint( index, p2 );
        pnt.x = p2.x;   pnt.y = p2.y;   pnt.z = 0;
        pnt.Transform( from__ );
        if ( !matr.IsSingle() )
          pnt.Transform( matr );
        return true;
      }
    }
    else // MbSpaceInstance
    {  
      MbControlData3D s3;
      s3.ReserveTotal( 10 );
      s3.ReserveShare( 10 );
      find->GetBasisPoints( s3 ); // Дать точки
      pp1.Transform( into_ ); // Перевод в экранную СК
      for ( size_t j = 0, jCnt = s3.Count(); j < jCnt; j++ ) {
        s3.GetPoint( j, pp0 );
        if ( !matr.IsSingle() )
          pp0.Transform( matr );
        pp0.Transform( into_ ); // Перевод в экранную СК
        dx = pp0.x-pp1.x;
        dy = pp0.y-pp1.y;
        d = dx*dx + dy*dy;
        if ( d<dmin ) {
          dmin = d;
          index = j;
        }
      }
      if ( index != SYS_MAX_T ) {
        s3.GetPoint( index, pnt );
        if ( !matr.IsSingle() )
          pnt.Transform( matr );
        return true;
      }
    }
  }

  pnt = cursorPoint;
  return false;
}


//------------------------------------------------------------------------------
// Привязка к ближайшей точке объекта
// ---
bool WorkWindow::FindNearestPlanePoint( MbePlaneType typeObj, 
                                        const MbFloatAxis3D & from_, double maxDistance, 
                                        const MbCartPoint3D & cursorPoint,
                                        MbCartPoint & pnt )
{
  MbItem * find = nullptr;
  MbRefItem * item = nullptr;
  MbMatrix3D into_( GetNullMatrixInto(false) );
  MbCartPoint3D pp0, pp1(cursorPoint);

  PathMatrix pathFrom;
  if ( FindGeometricItem( st_Undefined, tt_Undefined, typeObj, from_, maxDistance, &find, &item, pathFrom ) ) {
    const MbMatrix3D & matr = pathFrom.second;

    size_t index = SYS_MAX_T;
    double d, dx, dy, dmin = MB_MAXDOUBLE;

    if ( find->IsA() == st_PlaneInstance ) {
      MbControlData s2;
      MbCartPoint p2;
      s2.ReserveTotal( 10 );
      s2.ReserveShare( 10 );
      MbMatrix3D from__( ((MbPlaneInstance *)find)->GetPlacement().GetMatrixFrom() );
      find->GetBasisPoints( s2 ); // Дать точки
      pp1.Transform( into_ ); // Перевод в экранную СК
      for ( size_t j = 0, jCnt = s2.Count(); j < jCnt; j++ ) {
        s2.GetPoint( j, p2 );
        pp0.x = p2.x;   pp0.y = p2.y;   pp0.z = 0;
        pp0.Transform( from__ );
        if ( !matr.IsSingle() )
          pp0.Transform( matr );
        pp0.Transform( into_ ); // Перевод в экранную СК
        dx = pp0.x-pp1.x;
        dy = pp0.y-pp1.y;
        d = dx*dx + dy*dy;
        if ( d<dmin ) {
          dmin = d;
          index = j;
        }
      }
      if ( index != SYS_MAX_T ) {
        s2.GetPoint( index, p2 );
        pp0.x = p2.x;   pp0.y = p2.y;   pp0.z = 0;
        pp0.Transform( from__ );
        if ( !matr.IsSingle() )
          pp0.Transform( matr );
        MbPlacement3D pl(GetOwnPlacement());
        if ( !pl.DirectPointProjection(pp0,GetAxisZ(),pnt.x,pnt.y) ) {
          pp0.Transform( into_ );
          pnt.x = pp0.x;
          pnt.y = pp0.y;
        }
        return true;
      }
    }
    else { // MbSpaceInstance
      MbControlData3D s3;
      s3.ReserveTotal( 10 );
      s3.ReserveShare( 10 );
      find->GetBasisPoints( s3 ); // Дать точки
      pp1.Transform( into_ ); // Перевод в экранную СК
      for ( size_t j = 0, jCnt = s3.Count(); j < jCnt; j++ ) {
        s3.GetPoint( j, pp0 );
        if ( !matr.IsSingle() )
          pp0.Transform( matr );
        pp0.Transform( into_ ); // Перевод в экранную СК
        dx = pp0.x-pp1.x;
        dy = pp0.y-pp1.y;
        d = dx*dx + dy*dy;
        if ( d<dmin ) {
          dmin = d;
          index = j;
        }
      }
      if ( index != SYS_MAX_T ) {
        s3.GetPoint( index, pp0 );
        MbPlacement3D pl(GetOwnPlacement());
        if ( !pl.DirectPointProjection(pp0,GetAxisZ(),pnt.x,pnt.y) ) {
          if ( !matr.IsSingle() )
            pp0.Transform( matr );
          pp0.Transform( into_ );
          pnt.x = pp0.x;
          pnt.y = pp0.y;
        }
        return true;
      }
    }
  }

  pp0 = cursorPoint;
  MbPlacement3D pl(GetOwnPlacement());
  if ( !pl.DirectPointProjection(pp0,GetAxisZ(),pnt.x,pnt.y) ) {
    pp0.Transform( into_ );
    pnt.x = pp0.x;
    pnt.y = pp0.y;
  }
  return false;
}


//------------------------------------------------------------------------------
// Привязка к проекции точки на объект
// ---
bool WorkWindow::FindNearestSpaceProject( MbeSpaceType typeObj, 
                                          const MbFloatAxis3D & from_, double maxDistance, 
                                          const MbCartPoint3D & cursorPoint,
                                          MbCartPoint3D & pnt )
{
  MbItem * find = nullptr;
  MbRefItem * item_ = nullptr;

  PathMatrix pathFrom;
  if ( FindGeometricItem( typeObj, tt_Undefined, pt_Undefined, from_, maxDistance, &find, &item_, pathFrom ) ) {
    const MbMatrix3D & matr = pathFrom.second;

    MbCartPoint3D pp1(cursorPoint);

    if ( !matr.IsSingle() ) {
      MbMatrix3D into_;
      matr.Div( into_ );
      pp1.Transform( into_ );
    }
    if ( find->IsA() == st_PointFrame ) {
      MbPointFrame * item = (MbPointFrame *)find;

      size_t verticesCnt = item->GetVerticesCount();

      if ( verticesCnt > 1 ) {
        double dmin = MB_MAXDOUBLE;
        MbCartPoint3D pp2;
        for ( size_t k = 0; k < verticesCnt; k++ ) {
          item->GetCartPoint( k, pp2 );
          double dcur = pp1.DistanceToPoint( pp2 );
          if ( dcur < dmin ) {
            dmin = dcur;
            pnt = pp2;
          }
        }
      }
      else {
        item->GetCartPoint( pnt );
      }

      return true;
    }
    if ( find->IsA() == st_Solid ) {
      const MbSolid * item = (const MbSolid *)find;
      const MbFaceShell * shell = item->GetShell();
      if ( shell != nullptr ) {
        size_t i = SYS_MAX_T;
        double u, v;
        u = v = UNDEFINED_DBL;
        bool b = shell->NearPointProjection( pp1, i, u, v );
        shell->PointOn( i, u, v, pnt );
        return b;
      }
    }
    if ( find->IsA() == st_PlaneInstance ) {
      const MbPlaneItem * planeItem = ((MbPlaneInstance *)find)->GetPlaneItem();
      MbMatrix3D from__( ((MbPlaneInstance *)find)->GetPlacement().GetMatrixFrom() );
      MbMatrix3D into_(GetNullMatrixInto(false));
      MbPlacement3D pl(((MbPlaneInstance *)find)->GetPlacement());
      MbCartPoint p1, p0;
      if ( !pl.DirectPointProjection(pp1,GetAxisZ(),p1.x,p1.y) ) {
        pp1.Transform( into_ ); // Перевод в экранную СК
        p1.x = pp1.x;   p1.y = pp1.y;
      }
      //if ( planeItem->IsAPoint() ) {
      //  MbPoint *item = (MbPoint *)planeItem;
      //  pnt.x = item->point.x;   pnt.y = item->point.y;   pnt.z = 0;
      //  pnt.Transform( from__ );
      //  return true;
      //}
      if ( planeItem->Family() == pt_Curve ) {
        const MbCurve * item = static_cast<const MbCurve *>( planeItem );
        double t = item->PointProjection( p1 );
        item->PointOn( t, p0 );
        pnt.x = p0.x;   pnt.y = p0.y;   pnt.z = 0;
        pnt.Transform( from__ );
        return true;
      }
    }
    if ( find->IsA() == st_SpaceInstance ) {
      const MbSpaceItem * spaceItem = ((MbSpaceInstance *)find)->GetSpaceItem();
      if ( spaceItem->Family() == st_Curve3D ) {
        double t = UNDEFINED_DBL;
        const MbCurve3D *item = (const MbCurve3D *)spaceItem;
        bool b = item->NearPointProjection( pp1, t, false );
        item->PointOn( t, pnt );
        return b;
      }
      if ( spaceItem->Family() == st_Surface ) {
        double u, v;
        u = v = UNDEFINED_DBL;
        const MbSurface * item = (const MbSurface *)spaceItem;
        bool b = item->NearPointProjection( pp1, u, v, false );
        item->PointOn( u, v, pnt );
        return b;
      }
    }
  }

  pnt = cursorPoint;
  return false;
}


//------------------------------------------------------------------------------
// Привязка к проекции точки на объект
// ---
bool WorkWindow::FindNearestPlaneProject( MbePlaneType typeObj, 
                                          const MbFloatAxis3D & from_, double maxDistance, 
                                          const MbCartPoint3D & cursorPoint,
                                          MbCartPoint & pnt )
{
  MbItem * find = nullptr;
  MbRefItem * item_ = nullptr;
  MbMatrix3D into_(GetNullMatrixInto(false));
  MbCartPoint3D pp0, pp1(cursorPoint);

  PathMatrix pathFrom;
  if ( FindGeometricItem( st_Undefined, tt_Undefined, typeObj, from_, maxDistance, &find, &item_, pathFrom ) ) {
    const MbMatrix3D & matr = pathFrom.second;

    if ( !matr.IsSingle() ) {
      MbMatrix3D into__;
      matr.Div( into__ );
      pp1.Transform( into__ );
    }
    if ( find->IsA() == st_Solid ) {
      const MbSolid * item = (const MbSolid *)find;
      const MbFaceShell * shell = item->GetShell();
      bool b = false;
      if ( shell != nullptr ) {
        size_t i;
        double u, v;
        u = v = UNDEFINED_DBL;
        shell->NearPointProjection( pp1, i, u, v );
        shell->PointOn( i, u, v, pp0 );
        b = true;
      }
      if (b) {
        MbPlacement3D pl(GetOwnPlacement());
        if ( !pl.DirectPointProjection(pp0,GetAxisZ(),pnt.x,pnt.y) ) {
          pp0.Transform( into_ );
          pnt.x = pp0.x;
          pnt.y = pp0.y;
          b = false;
        }
        return b;
      }
    } 
    if ( find->IsA() == st_PlaneInstance ) {
      const MbPlaneItem * planeItem = ((MbPlaneInstance *)find)->GetPlaneItem();
      MbPlacement3D plf(((MbPlaneInstance *)find)->GetPlacement());
      MbCartPoint p1;
      if ( !plf.DirectPointProjection(pp1,GetAxisZ(),p1.x,p1.y) ) {
        pp1.Transform( into_ ); // Перевод в экранную СК
        p1.x = pp1.x;   p1.y = pp1.y;
      }
      bool b = false;
      if ( planeItem->Family() == pt_Curve ) {
        const MbCurve * item = static_cast<const MbCurve *>(planeItem);
        double t = item->PointProjection( p1 );
        item->PointOn( t, pnt );
        b = true;
      }
      if (b) {
        MbMatrix3D from__( ((MbPlaneInstance *)find)->GetPlacement().GetMatrixFrom() );
        MbPlacement3D pl(GetOwnPlacement());
        pp0.x = pnt.x;   pp0.y = pnt.y;   pp0.z = 0;
        pp0.Transform( from__ );
        if ( !pl.DirectPointProjection(pp0,GetAxisZ(),pnt.x,pnt.y) ) {
          pp0.Transform( into_ );
          pnt.x = pp0.x;
          pnt.y = pp0.y;
          b = false;
        }
        return b;
      }
    }
    if ( find->IsA() == st_SpaceInstance ) {
      const MbSpaceItem * spaceItem = ((MbSpaceInstance *)find)->GetSpaceItem();
      bool b = false;
      if ( spaceItem->Family() == st_Curve3D ) {
        double t = UNDEFINED_DBL;
        const MbCurve3D *item = (const MbCurve3D *)spaceItem;
        item->NearPointProjection( pp1, t, false );
        item->PointOn( t, pp0 );
        b = true;
      } 
      if ( spaceItem->Family() == st_Surface ) {
        double u, v;
        u = v = UNDEFINED_DBL;
        const MbSurface * item = (const MbSurface *)spaceItem;
        item->NearPointProjection( pp1, u, v, false );
        item->PointOn( u, v, pp0 );
        b = true;
      }

      if (b) {
        MbPlacement3D pl(GetOwnPlacement());
        if ( !pl.DirectPointProjection(pp0,GetAxisZ(),pnt.x,pnt.y) ) {
          pp0.Transform( into_ );
          pnt.x = pp0.x;
          pnt.y = pp0.y;
          b = false;
        }
        return b;
      }
    }
  }

  pp0 = cursorPoint;
  MbPlacement3D pl(GetOwnPlacement());
  if ( !pl.DirectPointProjection(pp0,GetAxisZ(),pnt.x,pnt.y) ) {
    pp0.Transform( into_ );
    pnt.x = pp0.x;
    pnt.y = pp0.y;
  }
  return false;
}


//------------------------------------------------------------------------------
// Удалить объект по типу
// ---
void WorkWindow::DeleteByType( MbeSpaceType type ) 
{
  //DeleteTemporal();
  RefreshOGL();

  RPArray<MbItem> geomItems( 0, 1 );
  geomModel.GetItems( geomItems );

  for ( intptr_t i = geomItems.MaxIndex(); i >= 0; i-- ) {
    MbItem * item_i = geomItems[i];
    if ( (type == st_Item)        || // Любой объект
         (type == st_SpaceItem)   || // Любой объект
         (type == item_i->IsA())  || // Объект данного класса
         (type == item_i->Type()) || // Объект данного семейства
         (type == item_i->Family()) ) { // Объект данного семейства
      DeleteObject( item_i );
    }
    if ( item_i->IsA() == st_SpaceInstance ) {
      const MbSpaceItem * spaceItem = ((MbSpaceInstance *)item_i)->GetSpaceItem(); // Дать трёхмерный геометрический объекта
      if ( spaceItem != nullptr ) {
        if ( (type == spaceItem->IsA())  || // Объект данного класса
             (type == spaceItem->Type()) || // Объект данного типа
             (type == spaceItem->Family()) ) { // Объект данного семейства
          DeleteObject( item_i );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Удалить объект по типу
// ---
void WorkWindow::DeleteByType( MbePlaneType type )
{
  //DeleteTemporal();
  RefreshOGL();

  RPArray<MbItem> geomItems( 0, 1 );
  geomModel.GetItems( geomItems );

  for ( intptr_t i = geomItems.MaxIndex(); i >= 0; i-- ) {
    MbItem * item_i = geomItems[i];
    if ( item_i->IsA() == st_PlaneInstance ) {
      const MbPlaneItem * planeItem = ((MbPlaneInstance*)item_i)->GetPlaneItem();
      if ( planeItem != nullptr ) {
        if ( (type == pt_PlaneItem)      || // Любой объект
             (type == planeItem->IsA())  || // Объект данного класса
             (type == planeItem->Type()) || // Объект данного класса
             (type == planeItem->Family()) ) { // Объект данного класса
          DeleteObject( item_i );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Выделить объекты по типу
// ---
static void SelectByType( WorkWindow & workWindow, MbItem * item_i, MbeSpaceType type ) 
{
  if ( item_i != nullptr ) {
    if ( (type == st_Item)        || // Любой объект
         (type == st_SpaceItem)   || // Любой объект
         (type == item_i->IsA())  || // Объект данного класса
         (type == item_i->Type()) || // Объект данного семейства
         (type == item_i->Family()) ) { // Объект данного семейства
      workWindow.SelectObject( item_i, false );
    }
    if ( item_i->IsA() == st_SpaceInstance ) {
      const MbSpaceItem * spaceItem = ((MbSpaceInstance *)item_i)->GetSpaceItem(); // Дать трёхмерный геометрический объекта
      if ( spaceItem != nullptr ) {
        if ( (type == spaceItem->IsA())  || // Объект данного класса
             (type == spaceItem->Type()) || // Объект данного семейства
             (type == spaceItem->Family()) ) { // Объект данного семейства
          workWindow.SelectObject( item_i, false );
        }
      }
    }
    else {
      if ( item_i->IsA() == st_Instance ) {
        MbInstance * instance = static_cast<MbInstance *>( item_i );
        ::SelectByType( workWindow, instance->SetItem(), type );
      }
      else {
        if ( item_i->IsA() == st_Assembly ) {
          MbAssembly * assembly = static_cast<MbAssembly *>( item_i );
          RPArray<MbItem> assemblyItems;
          assembly->GetItems( assemblyItems ); 
          for ( intptr_t j = assemblyItems.MaxIndex(); j >= 0; j-- ) {
            MbItem * item_j = assemblyItems[j];
            ::SelectByType( workWindow, item_j, type );
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Выделить объекты по типу
// ---
void WorkWindow::SelectByType( MbeSpaceType type ) 
{
  RPArray<MbItem> geomItems( 0, 1 );
  geomModel.GetItems( geomItems );

  for ( intptr_t i = geomItems.MaxIndex(); i >= 0; i-- ) {
    MbItem * item_i = geomItems[i];
    ::SelectByType( *this, item_i, type ); 
  }
  RefreshOGL();
  ShowModel();
}


//------------------------------------------------------------------------------
// Выделить объекты по типу 
// ---
static void SelectByType( WorkWindow & workWindow, MbItem * item_i, MbePlaneType type )
{
  if ( item_i != nullptr ) {
    if ( item_i->IsA() == st_PlaneInstance ) {
      const MbPlaneItem * planeItem = ((MbPlaneInstance*)item_i)->GetPlaneItem();
      if ( planeItem != nullptr ) {
        if ( (type == pt_PlaneItem)      || // Любой объект
             (type == planeItem->IsA())  || // Объект данного класса
             (type == planeItem->Type()) || // Объект данного класса
             (type == planeItem->Family()) ) { // Объект данного класса
          workWindow.SelectObject( item_i, false );
        }
      }
    }
    else {
      if ( item_i->IsA() == st_Instance ) {
        MbInstance * instance = static_cast<MbInstance *>( item_i );
        ::SelectByType( workWindow, instance->SetItem(), type );
      }
      else {
        if ( item_i->IsA() == st_Assembly ) {
          MbAssembly * assembly = static_cast<MbAssembly *>( item_i );
          RPArray<MbItem> assemblyItems;
          assembly->GetItems( assemblyItems ); 
          for ( intptr_t j = assemblyItems.MaxIndex(); j >= 0; j-- ) {
            MbItem * item_j = assemblyItems[j];
            ::SelectByType( workWindow, item_j, type );
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Выделить объекты по типу
// ---
void WorkWindow::SelectByType( MbePlaneType type ) 
{
  RPArray<MbItem> geomItems( 0, 1 );
  geomModel.GetItems( geomItems );

  for ( intptr_t i = geomItems.MaxIndex(); i >= 0; i-- ) {
    MbItem * item_i = geomItems[i];
    ::SelectByType( *this, item_i, type ); 
  }
  RefreshOGL();
  ShowModel();
}


//------------------------------------------------------------------------------
// Установить видимость объектов по типу
// ---
void WorkWindow::SetVisibleByType( MbeSpaceType type, bool visible )
{
  RPArray<MbItem> geomItems( 0, 1 );
  geomModel.GetItems( geomItems );

  for ( intptr_t i = geomItems.MaxIndex(); i >= 0; i-- ) {
    MbItem * item_i = geomItems[i];
    if ( (type == st_Item)        || // Любой объект
         (type == st_SpaceItem)   || // Любой объект
         (type == item_i->IsA())  || // Объект данного класса
         (type == item_i->Type()) || // Объект данного семейства
         (type == item_i->Family()) ) { // Объект данного семейства
      SetObjectVisible( item_i, visible );
    }
    if ( item_i->IsA() == st_SpaceInstance ) {
      const MbSpaceItem * spaceItem = ((MbSpaceInstance *)item_i)->GetSpaceItem(); // Дать трёхмерный геометрический объекта
      if ( spaceItem != nullptr ) {
        if ( (type == spaceItem->IsA())  || // Объект данного класса
             (type == spaceItem->Type()) || // Объект данного семейства
             (type == spaceItem->Family()) ) { // Объект данного семейства
          SetObjectVisible( item_i, visible );
        }
      }
    }
  }
} 


//------------------------------------------------------------------------------
// Установить видимость объектов по типу
// ---
void WorkWindow::SetVisibleByType( MbePlaneType type, bool visible )
{
  RPArray<MbItem> geomItems( 0, 1 );
  geomModel.GetItems( geomItems );

  for ( intptr_t i = geomItems.MaxIndex(); i >= 0; i-- ) {
    MbItem * item_i = geomItems[i];
    if ( item_i->IsA() == st_PlaneInstance ) {
      const MbPlaneItem * planeItem = ((MbPlaneInstance*)item_i)->GetPlaneItem();
      if ( planeItem != nullptr ) {
        if ( (type == pt_PlaneItem)      || // Любой объект
             (type == planeItem->IsA())  || // Объект данного класса
             (type == planeItem->Type()) || // Объект данного класса
             (type == planeItem->Family()) ) { // Объект данного класса
          SetObjectVisible( item_i, visible );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Выделить объекты модели
// ---
void WorkWindow::ModelSelectByObject( const MbItem * obj ) 
{
  if ( geomModel.ContainsItem( obj ) )
    SelectModel();
}


//------------------------------------------------------------------------------
// Переместить объект в конструктивной плоскости.
// ---
void WorkWindow::MoveObjectInPlacement( MbItem * obj, const MbVector3D & to )
{
  //DeleteTemporal();
  if ( obj != nullptr && obj->IsA() == st_PlaneInstance ) {
    if ( geomModel.ContainsItem( obj ) ) {
      RefreshOGL();

      MbPlacement3D place_( ((MbPlaneInstance *)obj)->GetPlacement() );
      MbVector to2;
      place_.VectorProjection( to, to2.x, to2.y );
      MbVector3D to3;
      to3.Set( place_.GetAxisX(), to2.x, place_.GetAxisY(), to2.y );
      MbModel::ItemConstIterator drawIter( drawModel.CBegin() );
      MbModel::ItemConstIterator drawEnd ( drawModel.CEnd() );
      EraseModel();
      for ( ; drawIter != drawEnd; ++drawIter ) {
        const MbItem * drawItem = *drawIter;
        if ( drawItem != nullptr ) {
          MbMatrix3D from__;
          MbPath path;
          const MbItem * geomItem = geomModel.GetItemByName( drawItem->GetItemName(), path, from__ );
          if ( (geomItem != nullptr) && geomItem->IsA() == st_PlaneInstance ) {
            const MbPlaneItem * planeItem = ((MbPlaneInstance*)geomItem)->GetPlaneItem();
            if ( planeItem != nullptr ) {
              if ( place_ == ((MbPlaneInstance *)geomItem)->GetPlacement() ) {
                ((MbPlaneItem *)planeItem)->Move( to2, nullptr );
                ((MbItem *)drawItem)->Move( to3, nullptr );
              }
            }
          }
        }
      }
      ShowModel();
    }
  }
}


//------------------------------------------------------------------------------
// Повернуть объект в конструктивной плоскости.
// ---
void WorkWindow::RotateObjectInPlacement( MbItem * obj, const MbCartPoint3D & point, double angle )
{
  //DeleteTemporal();
  if ( obj != nullptr && obj->IsA() == st_PlaneInstance ) {
    if ( geomModel.ContainsItem( obj ) ) {

      MbPlacement3D place_( ((MbPlaneInstance *)obj)->GetPlacement() );
      MbCartPoint p2;
      if ( place_.DirectPointProjection( point, GetAxisZ(), p2.x, p2.y ) ) {
        RefreshOGL();
        MbAxis3D axis( point, GetAxisZ() );
        MbModel::ItemConstIterator drawIter( drawModel.CBegin() );
        MbModel::ItemConstIterator drawEnd ( drawModel.CEnd() );
        EraseModel();
        for ( ; drawIter != drawEnd; ++drawIter ) {
          const MbItem * drawItem = *drawIter;
          if ( drawItem != nullptr ) {
            MbMatrix3D from__;
            MbPath path;
            const MbItem * geomItem = geomModel.GetItemByName( drawItem->GetItemName(), path, from__ );
            if ( (geomItem != nullptr) && geomItem->IsA() == st_PlaneInstance ) {
              const MbPlaneItem * planeItem = ((MbPlaneInstance*)geomItem)->GetPlaneItem();
              if ( planeItem != nullptr ) {
                if ( place_ == ((MbPlaneInstance *)geomItem)->GetPlacement() ) {
                  ((MbPlaneItem *)planeItem)->Rotate( p2, angle, nullptr );
                  ((MbItem *)drawItem)->Rotate( axis, angle, nullptr );
                }
              }
            }
          }
        }
        ShowModel();
      }
    }
  }
}


//------------------------------------------------------------------------------
// Трансформировать объекты в конструктивной плоскости.
// ---
void WorkWindow::TransformObjectInPlacement( MbItem * obj, const MbMatrix & matr )
{
  //DeleteTemporal();
  if ( obj != nullptr && obj->IsA() == st_PlaneInstance ) {
    if ( geomModel.ContainsItem( obj ) ) {

      MbPlacement3D place_( ((MbPlaneInstance *)obj)->GetPlacement() );
      RefreshOGL();
      MbModel::ItemConstIterator drawIter( drawModel.CBegin() );
      MbModel::ItemConstIterator drawEnd ( drawModel.CEnd() );
      EraseModel();
      for ( ; drawIter != drawEnd; ++drawIter ) {
        const MbItem * drawItem = *drawIter;
        if ( drawItem != nullptr ) {
          MbMatrix3D from__;
          MbPath path;
          const MbItem * geomItem = geomModel.GetItemByName( drawItem->GetItemName(), path, from__ );
          if ( (geomItem != nullptr) && geomItem->IsA() == st_PlaneInstance ) {
            MbPlaneInstance * planInstance = (MbPlaneInstance *)geomItem;
            if ( planInstance != nullptr ) {
              if ( place_ == planInstance->GetPlacement() ) {
                planInstance->Transform( matr, nullptr );
                //((MbItem *)drawItem)->Rotate( axis, angle, nullptr );
              }
            }
          }
        }
      }
      ShowModel();
    }
  }
}


//------------------------------------------------------------------------------
// Установить конструктивную плоскость
// ---
void WorkWindow::SetObjectPlacement( MbItem * obj, const MbPlacement3D & pl )
{
  if ( obj != nullptr && obj->IsA() == st_PlaneInstance )
  {
    if ( geomModel.ContainsItem(obj) )
    {
      RefreshOGL();

      MbPlacement3D place_( ((MbPlaneInstance *)obj)->GetPlacement() );
      MbMatrix3D matr;
      place_.GetMatrixToPlace( pl, matr ); // Дать матрицу преобразования в pl
      MbModel::ItemConstIterator drawIter( drawModel.CBegin() );
      MbModel::ItemConstIterator drawEnd ( drawModel.CEnd() );
      EraseModel();
      for ( ; drawIter != drawEnd; ++drawIter )
      {        
        if ( const MbItem * drawItem = *drawIter )
        {
          MbMatrix3D from__;
          MbPath path;
          const MbItem * geomItem = geomModel.GetItemByName( drawItem->GetItemName(), path, from__ );
          if ( (geomItem != nullptr) && (geomItem->IsA() == st_PlaneInstance) )
          {
            if ( ((MbPlaneInstance*)geomItem)->GetPlaneItem() != nullptr ) 
            {
              if ( place_ == ((MbPlaneInstance *)geomItem)->GetPlacement() ) 
              {
                ((MbPlaneInstance *)geomItem)->SetPlacement( pl );
                ((MbItem *)drawItem)->Transform( from__, nullptr );
              }
            }
          }
        }
      }
      ShowModel();
    }
  }
}


//------------------------------------------------------------------------------
// Изменить расстояние во масштабу активного окна.
// ---
double WorkWindow::ScaleDistance( double distance ) const 
{
  double result = distance;
  double scale_  = GetScale();
  result = ( distance / scale_ );

  return result;
}

