#include <test_temporal.h>
#include <test_window.h>
#include <test_gr_draw.h>
#include <space_instance.h>
#include <plane_instance.h>
#include <solid.h>
#include <last.h>

using namespace c3d;


const Style Temporal::groupStyle ( 5, RGB( 0, 0, 255 ) );
const Style Temporal::singleStyle( 5, RGB( 255, 0, 0 ) );


//------------------------------------------------------------------------------
// Удалить все
// ---
void Temporal::Delete()
{
  oTarget = nullptr;
  plane   = nullptr;
  sPoints.HardFlush();
  pPoints.HardFlush();

  meshs.clear();
  whole = 0;
}

//------------------------------------------------------------------------------
// Инициализация по объекту
// ---
void Temporal::Init( MbItem * obj, const MbMatrix3D & initMatr )
{
  Delete();

  if ( obj->IsA() == st_SpaceInstance ) {
    MbSpaceItem * spaceItem = (static_cast<MbSpaceInstance *>(obj))->SetSpaceItem();
    spaceItem->GetBasisPoints( sPoints );
  }
  else if ( obj->IsA() == st_PlaneInstance ) {
    MbPlaneItem * planeItem = (static_cast<MbPlaneInstance *>(obj))->SetPlaneItem();
    planeItem->GetBasisPoints( pPoints );
    if ( PointsCount() > 0 )
      plane = &(static_cast<MbPlaneInstance *>(obj))->SetPlacement();
  }
  else {
    obj->GetBasisPoints( sPoints );
  }

  size_t pointsCount = PointsCount();

  if ( pointsCount > 0 ) {
    oTarget = obj;
    matr.Init( initMatr );
    matr.Div( divMatr );
    meshs.reserve( pointsCount );
    whole = TotalCount();

    if ( whole > 0 )
      meshs.push_back( c3d::make_sptr<MbMesh>() );
    for ( size_t i = whole; i < pointsCount; i++ )
      meshs.push_back( c3d::make_sptr<MbMesh>() );
  }
}


//------------------------------------------------------------------------------
// Перемещение в зависимости от расстояния до перемещенной точке.
// ---
static double Distance( double length, double distance )
{
  double result( 0.0 );

  if ( (length > EPSILON) && (length > distance) ) {
    //result = ONE_HALF * (1.0 + ::cos(M_PI * distance / length) ); // Косинусоидальный закон.
    //result = 1.0 - ( distance / length ); // Линейный закон.
    result = 1.0 - ::sqrt( distance / length ); // Степенной закон.
  }

  return result;
}


//------------------------------------------------------------------------------
// Сдвиг
// ---
ptrdiff_t Temporal::Move( ptrdiff_t index, const MbVector3D & to, const MbVector & t, bool target )
{
  ptrdiff_t i, j, k = -1;

  if ( !pPoints.IsEmpty() ) // Работаем с планарными точками
  {
    if ( whole > 0 && index == 0 ) {
      if ( oTarget != nullptr && oTarget->IsA() == st_PlaneInstance ) {
        pPoints.Move( t );
        (static_cast<MbPlaneInstance &>(*oTarget)).Move( t );
        k = 0;
      }
    }
    else {
      i = index;
      if ( whole > 0 )
        i += whole-1;

      bool moveSame = true;

      if ( oTarget != nullptr ) {
        if ( oTarget->IsA() == st_PlaneInstance ) {
          const MbPlaneInstance * sInst = &static_cast<const MbPlaneInstance &>( *oTarget );
          if ( (sInst->GetPlaneItem() != nullptr) && (sInst->GetPlaneItem()->Family() == pt_Curve ) ) {
            if ( sInst->GetPlaneItem()->Type() != pt_Contour )
              moveSame = false; // не размыкался сплайн с совпадающими концами
          }
        }
      }
      if ( moveSame ) {
        for ( j = 0; j < (ptrdiff_t)pPoints.Count(); j++ ) {
          if ( j != i && pPoints[j] == pPoints[i] )
            pPoints[j].Move( t ); // Сдвиг совпадающих точек (для контура)
        }
      }

      pPoints[i].Move( t );
      if ( oTarget != nullptr ) {
        Refresh( target );
        k = i;
      }
    }
  }
  else {
    MbVector3D moveVec( to );
    if ( !divMatr.IsSingle() )  // вставка или сборка
      moveVec.Transform( divMatr );
    if ( whole > 0 && index == 0 ) {
      if ( oTarget != nullptr ) {
        sPoints.Move( moveVec );
        oTarget->Move( moveVec );
        k = 0;
      }
    }
    else {
      i = index;
      if ( whole > 0 )
        i += whole-1;

      bool moveSame = true;
      double koeff( ONE_THIRD );
      if ( oTarget != nullptr ) {
        if ( oTarget->IsA() == st_SpaceInstance ) {
          const MbSpaceInstance * sInst = &static_cast<const MbSpaceInstance &>( *oTarget );
          if ( (sInst->GetSpaceItem() != nullptr) && (sInst->GetSpaceItem()->Family() == st_Curve3D ) ) {
            if ( sInst->GetSpaceItem()->Type() == st_Contour3D )
              koeff = DELTA_MIN;
            else
              moveSame = false; // не размыкался сплайн с совпадающими концами
          }
        }
      }
      if ( moveSame ) {
        if ( oTarget != nullptr ) { // Сдвиг близких точек по закону удаления.
          MbCube cube;
          oTarget->AddYourGabaritTo( cube );
          double length = koeff * cube.GetDiagonal();
          for ( size_t n = 0; n < sPoints.Count(); n++ ) {
            if ( n != (size_t)i ) {
              double distance = sPoints[i].DistanceToPoint( sPoints[n] );
              double dd = ::Distance( length, distance );
              if ( dd > EPSILON ) {
                MbVector3D vec( to );
                vec *= dd;
                sPoints[n].Move( vec ); 
              }
            }
          }
        }
      }
      /*if ( moveSame ) {
        for ( j = 0; j < (ptrdiff_t)sPoints.Count(); j++ ) {
          if ( j != i && *(sPoints[j]) == *(sPoints[i]) )
            sPoints[j]->Move( moveVec ); // Сдвиг совпадающих точек ( для контура)
        }
      }*/
      
      sPoints[i].Move( moveVec );
      if ( oTarget!=nullptr ) {
        Refresh( target );
        k = i;
      }
    }
  }
  return k;
}


//------------------------------------------------------------------------------
// Сдвиг
// ---
ptrdiff_t Temporal::Move( const MbVector3D & to, const MbVector & t )
{
  ptrdiff_t k = -1;
  if ( PointsCount() > 0 && pPoints.Count() == PointsCount() ) {
    if ( oTarget != nullptr && oTarget->IsA() == st_PlaneInstance ) {
      pPoints.Move( t );
      (static_cast<MbPlaneInstance &>(*oTarget)).Move( t );
      k = 0;
    }
  }
  else if ( oTarget != nullptr ) {
    MbVector3D moveVec( to );
    if ( !divMatr.IsSingle() )  // вставка или сборка
      moveVec.Transform( divMatr );
    sPoints.Move( moveVec );
    oTarget->Move( moveVec );
    k = 0;
  }
  return k;
}


//------------------------------------------------------------------------------
// Дать точку
// ---
ptrdiff_t Temporal::GetPoint( ptrdiff_t index, ptrdiff_t n, MbCartPoint3D & p3, MbCartPoint & p2 ) const
{
  if ( PointsCount() > 0 && pPoints.Count() == PointsCount() ) {
    if ( whole > 0 ) p2 = pPoints[n];
    else             p2 = pPoints[index];
    return 2;
  }
  if ( PointsCount() > 0 && sPoints.Count() == PointsCount() ) {
    if ( whole > 0 ) p3 = sPoints[n];
    else             p3 = sPoints[index];
    return 3;
  }
  return 0;
}


//------------------------------------------------------------------------------
// Установить точку
// ---
ptrdiff_t Temporal::SetPoint( ptrdiff_t index, ptrdiff_t n, MbCartPoint3D & p ) {
  ptrdiff_t i, k = -1;
  if ( PointsCount()>0 && sPoints.Count()==PointsCount() ) {
    MbVector3D to;
    if ( whole>0 && index == 0 ) {
      to = p - sPoints[n];
      if ( oTarget!=nullptr ) {
        oTarget->Move( to );
        k = n;
      }
    }
    else {
      to = p - sPoints[index];
      i = index;
      if ( whole>0 )
        i += whole-1;
      sPoints[i].Move( to );
      if ( oTarget!=nullptr ) {
        Refresh( true );
        k = i;
      }
    }
  }
  return k;
}


//------------------------------------------------------------------------------
// Установить точку
// ---
ptrdiff_t Temporal::SetPoint( ptrdiff_t index, ptrdiff_t n, MbCartPoint & p ) {
  ptrdiff_t i, k = -1;
  if ( PointsCount()>0 && pPoints.Count()==PointsCount() ) {
    MbVector to;
    if ( whole>0 && index == 0 ) {
      to.x = p.x - pPoints[n].x;
      to.y = p.y - pPoints[n].y;
      for ( i = 0; i < (ptrdiff_t)pPoints.Count(); i++ )
        pPoints[i].Move( to );
      if ( oTarget!=nullptr ) {
        Refresh( true );
//        k = 0;
      }
    }
    else {
      to.x = p.x - pPoints[index].x;
      to.y = p.y - pPoints[index].y;
      i = index;
      if ( whole>0 )
        i += whole-1;
      pPoints[i].Move( to );
      if ( oTarget!=nullptr ) {
        Refresh( true );
        k = i;
      }
    }
  }
  return k;
}


//------------------------------------------------------------------------------
// Сдвинуть отображение в окне
// ---
void Temporal::Move( ptrdiff_t i, const MbVector3D & to, WorkWindow * wind, const COLORREF & col )
{
  MbMatrix3D from;
  wind->PutMeshGDI( meshs[i], from, nullptr, nullptr, col, true, false );
  meshs[i]->Move( to );
  from.Init();
  wind->PutMeshGDI( meshs[i], from, nullptr, nullptr, col, false, false );
}


//------------------------------------------------------------------------------
// Сдвинуть отображение в окне
// ---
void Temporal::Move( const MbVector3D & to, WorkWindow * wind, const COLORREF & col  )
{
  MbMatrix3D from;
  ptrdiff_t i, n = meshs.size();
  for ( i = 0; i < n; i++ ) {
    from.Init();
    wind->PutMeshGDI( meshs[i], from, nullptr, nullptr, col, true, false );
  }
  for ( i = 0; i < n; i++ )
    meshs[i]->Move( to );
  for ( i = 0; i < n; i++ ) {
    from.Init();
    wind->PutMeshGDI( meshs[i], from, nullptr, nullptr, col, false, false );
  }
}


//------------------------------------------------------------------------------
// Стереть в окне
// ---
void Temporal::Erase( WorkWindow * wind  )
{
  if ( wind != nullptr ) {
    if ( wind->GetRenderType() != wwrt_OGL ) {
      MbMatrix3D from;
      for ( size_t i = 0, icnt = meshs.size(); i < icnt; i++ ) {
        wind->PutMeshGDI( meshs[i], from, nullptr, nullptr, wind->GetDrawTool().GetBackground(), true, false );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Нарисовать в окне
// ---
void Temporal::Show( WorkWindow * wind ) const
{
  PRECONDITION( wind != nullptr );
  if ( wind != nullptr ) {
    MbMatrix3D from;
    COLORREF col( MB_C3DCOLOR );

    if ( wind->GetRenderType() == wwrt_OGL ) {
      std::vector< SPtr<MbItem> > items;
      items.reserve( meshs.size() );
      for ( size_t i = 0, icnt = meshs.size(); i < icnt; i++ )
        items.push_back( meshs[i] );
      wind->ShowOGL( &items );
    }
    else {
      for ( size_t i = 0, icnt = meshs.size(); i < icnt; i++ ) {
        wind->PutMeshGDI( meshs[i], from, nullptr, nullptr, col, false, false );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Построить карту
// ---
bool Temporal::CalculateMesh()
{
  size_t meshsCount = meshs.size();

  // У тела без построителей нет контрольных точек
  //PRECONDITION( meshsCount > 0 );

  if ( meshsCount > 0 ) {
    bool transform = !matr.IsSingle();
    SArray<MbCartPoint3D> spacePoints( sPoints.Count(), 2 );
    for ( size_t i = 0, cnt = sPoints.Count(); i < cnt; i++ ) {
      MbCartPoint3D pt = sPoints[i];
      if ( transform )   // вставка или сборка
        pt.Transform( matr );
      spacePoints.Add( pt );
    }

    size_t i0 = 0;
    if ( whole > 0 ) {
      meshs[0]->Flush();
      meshs[0]->SetColor( groupStyle.GetColor() );     // Цвет 
      meshs[0]->SetWidth( groupStyle.GetWidth() );     // Толщина
      meshs[0]->SetStyle( groupStyle.GetStyle() );     // Стиль 
      i0++;
    }
    for ( size_t i = i0; i < meshsCount; i++ ) {
      meshs[i]->Flush();
      meshs[i]->SetColor( singleStyle.GetColor() );     // Цвет 
      meshs[i]->SetWidth( singleStyle.GetWidth() );     // Толщина
      meshs[i]->SetStyle( singleStyle.GetStyle() );     // Стиль 
    }

    if ( PointsCount() > 0 && pPoints.Count() == PointsCount() ) {
      MbCartPoint pp;
      MbCartPoint3D pnt;
      MbMatrix3D mfrom( plane->GetMatrixFrom() );
      ptrdiff_t i, j = 0;
      if ( whole > 0 && meshsCount > 0 ) {
        j = 1-whole;
        for ( i = 0; i < whole; i++ ) {
          MbApex3D * apex = meshs[0]->AddApex();
          pp = pPoints[i];
          pnt.Init(pp.x,pp.y,0.0);
          pnt.Transform( mfrom );
          apex->Init( pnt );
        }
      }
      for ( i = whole; i < (ptrdiff_t)PointsCount() && j+i < (ptrdiff_t)meshsCount; i++ ) {
        MbApex3D * apex = meshs[j+i]->AddApex();
        pp = pPoints[i];
        pnt.Init(pp.x,pp.y,0.0);
        pnt.Transform( mfrom );
        apex->Init( pnt );
      }
    }
    else {
      ptrdiff_t i = 0, j = 0;
      if ( whole > 0 && meshsCount > 0 ) {
        j = 1-whole;
        for ( i = 0; i < std_min( whole, (ptrdiff_t)spacePoints.Count() ); i++ ) {
          MbApex3D * apex = meshs[0]->AddApex();
          apex->Init( spacePoints[i] );
        }
      }
      for ( i = whole; i < (ptrdiff_t)PointsCount() && j+i < (ptrdiff_t)meshsCount; i++ ) {
        MbApex3D * apex = meshs[j+i]->AddApex();
        apex->Init( spacePoints[i] );
      }
    }
    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Обновить форму оболочки
// ---
void Temporal::Refresh( bool target )
{
  if ( oTarget != nullptr ) {
    pPoints.ResetIndex();
    sPoints.ResetIndex();
    //oTarget->Refresh();

    if ( oTarget->IsA() == st_SpaceInstance ) {
      MbSpaceInstance & spaceInstance = (static_cast<MbSpaceInstance &>(*oTarget));
      spaceInstance.SetSpaceItem()->SetBasisPoints( sPoints );
      spaceInstance.SetSpaceItem()->Refresh();
    }
    else if ( oTarget->IsA() == st_PlaneInstance ) {
      MbPlaneInstance & planeInstance = (static_cast<MbPlaneInstance &>(*oTarget));
      planeInstance.SetPlaneItem()->SetBasisPoints( pPoints );
      planeInstance.SetPlaneItem()->Refresh();
    }
    else {
      if ( !pPoints.IsEmpty() )
        oTarget->SetBasisPoints( pPoints );
      if ( !sPoints.IsEmpty() )
        oTarget->SetBasisPoints( sPoints );
      if ( target && sPoints.ShareCount() > 0 && (oTarget->IsA() == st_Solid || oTarget->IsA() == st_Mesh) ) 
        (static_cast<MbSolid &>(*oTarget)).RebuildItem( cm_Same, nullptr, nullptr );

      oTarget->Refresh();
    }
  }
}


//------------------------------------------------------------------------------
// Рассстояние до объекта
// ---
float DistanceToPoint( const MbMesh * mesh, const MbFloatAxis3D & from, float maxDistance, ptrdiff_t & j )
{
  float dMin = MB_MAXFLOAT;

  if ( mesh != nullptr ) {
    size_t mc = mesh->ApexesCount();
    for ( size_t i = 0; i < mc; i++ ) {
      const MbApex3D * apex = mesh->GetApex( i );
      MbFloatPoint3D fp;
      apex->GetPoint( fp );
      float d = from.DistanceToPoint( fp );
      if ( d < dMin && d < maxDistance ) {
        dMin = d;
        j = i;
      }
    }
  }

  return dMin;
}

