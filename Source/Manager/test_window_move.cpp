#include <test_window.h>
#include <test_gr_draw.h>
#include <test_variables.h>
#include <test_service.h>
#include <cur_polyline3d.h>
#include <cur_point_curve.h> 
#include <cur_plane_curve.h> 
#include <cur_surface_intersection.h>
#include <cur_contour_on_plane.h>   
#include <cur_contour3d.h>
#include <surf_curve_bounded_surface.h>
#include <action_mesh.h>
#include <solid.h>
#include <space_instance.h>
#include <plane_instance.h>
#include <wire_frame.h>
#include <action_curve3d.h>
#include <map_create.h>
#include <space_instance.h>
#include <instance_item.h>
#include <assembly.h>
#include <region.h>
#include <utility>
#include <set>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Сдвинуть окно по местной оси номер numb на расстояние range
// ---
void WorkWindow::MoveWindow( int numb, double range ) 
{
  EraseWindow(); // KYA медленно // currentView->EraseModel( drawModel, backgr ); // Стереть 
  Move( numb, range );
  InvalidateWindow( false );
}


//------------------------------------------------------------------------------
// Повернуть окно вокруг своей оси номер numb на угол angle, сохранив неподвижной точку центра окна
// ---
void WorkWindow::RotateWindow( int numb, double angle ) 
{
  if ( proj != v_Arbitrary ) {
    proj = v_Arbitrary;
    SetWindowTitle();
  }

  MbCartPoint3D p;
  p = (cube.pmin + cube.pmax) * 0.5;
  EraseWindow(); // KYA медленно // currentView->EraseModel( drawModel, backgr ); // Стереть 
  Rotate( p, numb, angle );
  // KYA обновится по WM_PAINT // currentView->ShowModel ( drawModel, select ); // Нарисовать
  InvalidateWindow( false );
}


//------------------------------------------------------------------------------
// Установить плейсемент окна
// ---
void WorkWindow::OrientateWindow( const MbPlacement3D & plane )
{
  EraseWindow(); // KYA медленно // currentView->EraseModel( drawModel, backgr ); // Стереть 
  Orientate( plane );
  InvalidateWindow( false );
}


//------------------------------------------------------------------------------
// Установить плейсемент окна
// ---
void WorkWindow::OrientateWindow( wProjections newProj )
{
  proj = newProj;
  EraseWindow(); 

  MbVector3D z( 0, 0, 1 );
  MbVector3D x( 1, 0, 0 );

  switch ( proj ) {
    case v_Front : // Спереди
      z.x =  0; z.y =  0; z.z =  1;
      x.x =  1; x.y =  0; x.z =  0;
      break;

    case v_Rear :  // Сзади
      z.x =  0; z.y =  0; z.z = -1;
      x.x = -1; x.y =  0; x.z =  0;
      break;

    case v_Up :    // Сверху
      z.x =  0; z.y =  1; z.z =  0;
      x.x =  1; x.y =  0; x.z =  0;
      break;

    case v_Down :  // Снизу
      z.x =  0; z.y = -1; z.z =  0;
      x.x =  1; x.y =  0; x.z =  0;
      break;

    case v_Left :  // Слева
      z.x = -1; z.y =  0; z.z =  0;
      x.x =  0; x.y =  0; x.z =  1;
      break;

    case v_Right : // Справа
      z.x =  1; z.y =  0; z.z =  0;
      x.x =  0; x.y =  0; x.z = -1;
      break;

    case v_Isometry : // Изометрия
      z.x =  1; z.y =  1; z.z =  1;
      x.x =  1; x.y =  0; x.z = -1;
      break;

    default: break;
  }

  if ( cube.IsEmpty() )
    CalculateGabarit();

  MbCartPoint3D p;
  cube.GetCentre( p );
  place.Init( p, z, x );

  CalculateMatrix();

  POINT c;
  RECT rc;

  GetDrawTool().GetClientRect( rc );
  c.x = (rc.right + rc.left) / 2;
  c.y = (rc.top + rc.bottom) / 2;
  p = GetCartPoint( c ); // Старый центр экрана должен сохраниться

  MbVector3D to( p );
  to.Invert();
  place.Move( to );
  CalculateMatrix();

  //if ( renderType != wwrt_OGL )
  //  make_grid = false;

  InvalidateWindow( false );
  SetFocus( false );
}


//------------------------------------------------------------------------------
// Вписать модель по размеру окна.
// Fit model to window.
// ---
void WorkWindow::FitModelToWindow() {
  EraseWindow();
  if ( cube.IsEmpty() )
    CalculateGabarit();

  if ( !cube.IsEmpty() ) {
    MbCartPoint3D p;
    cube.GetCentre( p );
    place.SetOrigin( p );
    CalculateMatrix();

    MbRect cubeRect;
    cube.ProjectionRect( place, cubeRect );
    
    // Автомасштабирование
    // Autoscale
    if ( !cubeRect.IsDegenerateX() && !cubeRect.IsDegenerateY() ) {
      RECT rc;
      GetDrawTool().GetClientRect( rc );

      if ( rc.right != rc.left && rc.bottom != rc.top ) {
        double scaleX = (rc.right - rc.left) / (cubeRect.GetLengthX() * GetDrawTool().GetXPixelPerMM());
        double scaleY = (rc.bottom - rc.top) / (cubeRect.GetLengthY() * GetDrawTool().GetYPixelPerMM());
        SetScale( 0.98 * std_min( scaleX, scaleY ) );
      }
    }

    // Автопозиционирование в текущем виде.
    // Autoposition in current view
    MbVector3D to;
    place.VectorOn( MbVector(-cubeRect.GetLengthX(), cubeRect.top), to );
    place.Move( to );

    CalculateMatrix();
  }
  InvalidateWindow( false );
  SetFocus( false );
}

//------------------------------------------------------------------------------
// Переключить режим отображения невидимых линий
// ---
void WorkWindow::TurnOverMapping() 
{
  hideline = !hideline;
  if ( !hideline ) {
    tmpObjects.Flush( defDelete );
  }
//  ShowModel();
}


//-------------------------------------------------------------------------------
// Создать ломаную по ломаной
// ---
static MbPolyline3D & CreatePolyline( const MbPolygon3D * poly, bool closed = false )
{
  SArray<MbCartPoint3D> initList( poly->Count(), 1 );
  MbCartPoint3D pnt;
  for ( ptrdiff_t i = 0, icount = poly->Count(); i < icount; i++ ) {
    poly->GetPoint( i, pnt );
    initList.Add( pnt );
  }
  return *new MbPolyline3D( initList, closed );
}


//------------------------------------------------------------------------------
// Нарисовать видимые и не видимые линии тел в активном окне приложения
// ---
void WorkWindow::ShowMapping() 
{
  if ( tmpObjects.Count()==0 ) {

    MbPlacement3D tmpPlace( GetOwnPlacement() );    // Временная система
    tmpObjects.Flush();
    RPArray<MbItem> objs( 10, 1 );
    SArray<MbMatrix3D> matrs( 10, 1 );
    MbItem * newobj( nullptr );
    double znear = 0;

    GetObjects( st_Solid, objs, matrs ); // Дать все объекты указанного типа

    for ( size_t k = 0; k < objs.Count(); k++ ) {
      if ( objs[k] != nullptr ) {
        MbSolid * solid = (MbSolid *)objs[k];
        PArray<MbPolygon3DSolid> visibles ( 0, 1, true ); //    1. видимые   линии
        PArray<MbPolygon3DSolid> hides    ( 0, 1, true ); //    2. невидимые линии
        PArray<MbPolygon3DSolid> transfers( 0, 1, true ); //    3. видимые   линии перехода
        PArray<MbPolygon3DSolid> outs     ( 0, 1, true ); //    4. невидимые линии перехода
        PArray<MbLump> lumps( 1, 1, true );

        lumps.Add( new MbLump(*solid, matrs[k]) );

        // Получить данные и создать массивы для видимых и невидимых линий
        ::HiddenLinesMapping( lumps, tmpPlace, znear, 0.1, visibles, hides, transfers, outs );

        ptrdiff_t i = 0, count = hides.Count();
        for ( ; i < count; i++ ) { // Невидимые линии
          newobj = new MbSpaceInstance( ::CreatePolyline(hides[i]->GetPolygon()) );
          newobj->SetColor( RGB(255, 255, 255) );
          tmpObjects.Add( newobj );
        }

        for ( i = 0, count = transfers.Count(); i < count; i++ ) { // Линии перехода
          newobj = new MbSpaceInstance( ::CreatePolyline(transfers[i]->GetPolygon()) );
          newobj->SetColor( RGB(150, 150, 150) );
          tmpObjects.Add( newobj );
        }

        for ( i = 0, count = outs.Count(); i < count; i++ ) { // Линии очерка
          newobj = new MbSpaceInstance( ::CreatePolyline(outs[i]->GetPolygon()) );
          newobj->SetColor( RGB(200, 200, 200) );
          tmpObjects.Add( newobj );
        }

        for ( i = 0, count = visibles.Count(); i < count; i++ ) { // Видимые линии
          newobj = new MbSpaceInstance( ::CreatePolyline(visibles[i]->GetPolygon()) );
          newobj->SetColor( RGB(0, 0, 0) );
          tmpObjects.Add( newobj );
        }
      }
    }
  }

  if ( GetRenderType() == wwrt_GDI ) {
    EraseWindow(); // KYA медленно // EraseModel( drawModel, backgr );
    double sag = Math::visualSag;
    MbRegDuplicate * iReg = nullptr;
    MbAutoRegDuplicate autoReg( iReg );  

    for ( ptrdiff_t i = 0, count = tmpObjects.Count(); i < count; i++ ) {
      MbItem * geomItem = tmpObjects[i];
      MbItem * drawItem = geomItem->CreateMesh( MbStepData(ist_SpaceStep, sag), MbFormNote(true, false, true, TestVariables::exactMesh), iReg );
      MbMatrix3D matr; 
      //const MbColor * hostColor = static_cast<const MbColor *>(geomItem->GetSimpleAttribute(at_Color));

      PutItemGDI( drawItem, matr, nullptr, geomItem, geomItem->GetColor(), false ); // Нарисовать проекцию элемента
      ::DeleteItem( drawItem );
    }
  }
}


//------------------------------------------------------------------------------
// Селектировать элементы для случая сборки и вставки
// ---
static void SelectModelItem( MbItem * item, bool selected )
{
  if ( item != nullptr ) {
    if ( item->IsA() == st_Assembly ) {
      MbAssembly * assembly = static_cast<MbAssembly *>( item );
      for ( size_t i = 0, iCount = assembly->ItemsCount(); i < iCount; i++ ) {
        ::SelectModelItem( assembly->SetItem(i), selected );
      }
    }
    else if ( item->IsA() == st_Instance ) {
      MbInstance * inst = static_cast<MbInstance *>( item );
      if ( inst->GetItem() != nullptr && inst->GetItem()->IsA() == st_Assembly )
        ::SelectModelItem( inst->SetItem(), selected );
      else
        item->SetSelected( selected );
    }
    else
      item->SetSelected( selected );
  }
}


//------------------------------------------------------------------------------
// Инвертировать элементы для случая сборки и вставки
// ---
static void ToggleModelItem( MbItem * item, std::set<const MbItem *> * changedItems )
{
  bool noChangedItems = (changedItems == nullptr);

  if ( noChangedItems )
    changedItems = new std::set<const MbItem *>;

  if ( item != nullptr ) {
    if ( item->IsA() == st_Assembly ) {
      MbAssembly * assembly = static_cast<MbAssembly *>( item );
      for ( size_t i = 0, iCount = assembly->ItemsCount(); i < iCount; i++ ) {
        ::ToggleModelItem( assembly->SetItem(i), changedItems );
      }
    }
    else if ( item->IsA() == st_Instance ) {
      MbInstance * inst = static_cast<MbInstance *>( item );
      if ( inst->GetItem() != nullptr && inst->GetItem()->IsA() == st_Assembly )
        ::ToggleModelItem( inst->SetItem(), changedItems );
      else {
        std::set<const MbItem *>::iterator it = changedItems->find( item );
        if ( it == changedItems->end() ) { // в сборках и вставках могут быть одни и те же тела
          item->SetSelected( !item->IsSelected() );
          changedItems->insert( item );
        }
      }
    }
    else {
      std::set<const MbItem *>::iterator it = changedItems->find( item );
      if ( it == changedItems->end() ) { // в сборках и вставках могут быть одни и те же тела
        item->SetSelected( !item->IsSelected() );
        changedItems->insert( item );
      }
    }
  }

  if ( noChangedItems )
    delete changedItems;
}



//------------------------------------------------------------------------------
// Выделить модель во всех окнах
// ---
void WorkWindow::SelectModel()
{
  // KYA так медленно
  // for ( uint j = 0, count = geomModel.ItemsCount(); j < count; j++ )
  //   SelectObject( geomModel.SetItem( j ) );

  SetWaitCursor( true );

  RPArray<MbItem> geomItems( 0, 1 );
  RPArray<MbItem> drawItems( 0, 1 );
  geomModel.GetItems( geomItems );
  drawModel.GetItems( drawItems );
  PRECONDITION( geomItems.Count() == drawItems.Count() ); // Рассогласование между моделям

  size_t k, cnt;
  for ( k = 0, cnt = geomItems.Count(); k < cnt; k++ ) {
    if ( geomItems[k] != nullptr )
      ::SelectModelItem( geomItems[k], true );
  }
  for ( k = 0, cnt = drawItems.Count(); k < cnt; k++ ) {
    if ( drawItems[k] != nullptr )
      ::SelectModelItem( drawItems[k], true );
  }

  SetWaitCursor( false );

  RefreshOGL();
  ShowModel();
}


//------------------------------------------------------------------------------
// Выделить модель во всех окнах
// ---
void WorkWindow::UnselectModel()
{
  // KYA так медленно
  // for ( uint j = 0, count = geomModel.ItemsCount(); j < count; j++ )
  //   UnselectObject( geomModel.SetItem( j ) );

  SetWaitCursor( true );

  RPArray<MbItem> geomItems( 0, 1 );
  RPArray<MbItem> drawItems( 0, 1 );
  geomModel.GetItems( geomItems );
  drawModel.GetItems( drawItems );
  PRECONDITION( geomItems.Count() == drawItems.Count() ); // Рассогласование между моделям

  size_t k, cnt;
  for ( k = 0, cnt = geomItems.Count(); k < cnt; k++ ) {
    if ( geomItems[k] != nullptr )
      ::SelectModelItem( geomItems[k], false );
  }
  for ( k = 0, cnt = drawItems.Count(); k < cnt; k++ ) {
    if ( drawItems[k] != nullptr )
      ::SelectModelItem( drawItems[k], false );
  }

  SetWaitCursor( false );

  RefreshOGL();
  ShowModel();
}


//------------------------------------------------------------------------------
// Выделить модель во всех окнах
// ---
void WorkWindow::ToggleModel()
{
  // KYA так медленно
  // for ( uint j = 0, count = geomModel.ItemsCount(); j < count; j++ )
  //   ToggleObject( geomModel.SetItem( j ) );

  SetWaitCursor( true );

  RPArray<MbItem> geomItems( 0, 1 );
  RPArray<MbItem> drawItems( 0, 1 );
  geomModel.GetItems( geomItems );
  drawModel.GetItems( drawItems );
  PRECONDITION( geomItems.Count() == drawItems.Count() ); // Рассогласование между моделям

  size_t k, cnt;
  for ( k = 0, cnt = geomItems.Count(); k < cnt; k++ ) {
    if ( geomItems[k] != nullptr )
      ::ToggleModelItem( geomItems[k], nullptr );
  }
  for ( k = 0, cnt = drawItems.Count(); k < cnt; k++ ) {
    if ( drawItems[k] != nullptr )
      ::ToggleModelItem( drawItems[k], nullptr );
  }

  SetWaitCursor( false );

  RefreshOGL();
  ShowModel();
}


//------------------------------------------------------------------------------
// Преобразование выделенных объектов согласно матрице
// ---
void WorkWindow::TransformSelected( const MbMatrix3D & matr ) 
{
  EraseModel();

  MbRegTransform * iReg = nullptr;
  geomModel.TransformSelected( matr, iReg );
  MbRegTransform * iReg_ = nullptr;
  drawModel.TransformSelected( matr, iReg_ );

  RefreshOGL();
  ShowModel();
}


//------------------------------------------------------------------------------
// Сдвиг выделенных объектов
// ---
void WorkWindow::MoveSelected( const MbVector3D & to )
{
  EraseModel();

  MbRegTransform * iReg = nullptr;
  geomModel.MoveSelected( to, iReg );
  MbRegTransform * iReg_ = nullptr;
  drawModel.MoveSelected( to, iReg_ );

  RefreshOGL();
  ShowModel();
}


//------------------------------------------------------------------------------
// Повернуть вокруг оси выделенных объектов
// ---
void WorkWindow::RotateSelected( const MbAxis3D & axis, double angle ) 
{
  EraseModel();

  MbRegTransform * iReg = nullptr;
  geomModel.RotateSelected( axis, angle, iReg );
  MbRegTransform * iReg_ = nullptr;
  drawModel.RotateSelected( axis, angle, iReg_ );

  RefreshOGL();
  ShowModel();
}


//------------------------------------------------------------------------------
// Дублировать выделенные объекты
// ---
void WorkWindow::DuplicateSelected() 
{
  RPArray<MbItem> geomItems( 0, 1 );
  geomModel.GetItems( geomItems );

  for ( size_t k = 0, kcnt = geomItems.Count(); k < kcnt; k++ ) {
    MbItem * geomItem = geomItems[k];
    if ( geomItem != nullptr && geomItem->IsSelected() )
      DuplicateObject( geomItem );
  }

  RefreshOGL();
  ShowModel();
}


//------------------------------------------------------------------------------
// Удалить все выбранные объекты. Delete all selected objects.
// ---
void WorkWindow::DeleteSelected() 
{
  SetWaitCursor( true );

  TestVariables::ResetHLighted();   

  RPArray<MbItem> items;
  SArray<MbMatrix3D> matrs;

  geomModel.DetachSelected( items, matrs, true ); 

  for ( size_t j = 0, jCount = items.Count(); j < jCount; j++ ) {
    items[j]->AddRef();
    items[j]->Transform( matrs[j] );
  }
  UndoObjects( items, at_DeleteType );
  ::ReleaseItems( items );

  items.DetachAll();
  matrs.Flush();

  EraseModel();
  RefreshOGL();
  drawModel.DetachSelected( items, matrs, true ); 
  ::DeleteItems( items );
  ClearArraySelectedElements();
  ShowModel();

  SetWaitCursor( false );
}


//------------------------------------------------------------------------------
// Удаление все невидимые объекты. Delete all invisible objects.
// ---
void WorkWindow::DeleteInvisible() 
{
  SetWaitCursor( true );

  //DeleteTemporal();
  //DeleteCplane();

  // Запомненный объект и его элемент
  TestVariables::ResetHLighted();
   

  RPArray<MbItem> items( 0, 1 );
  SArray<MbMatrix3D> matrs( 0, 1 );

  geomModel.DetachInvisible( items, matrs, true ); 

  for ( size_t j = 0, jCount = items.Count(); j < jCount; j++ ) {
    items[j]->AddRef();
    items[j]->Transform( matrs[j] );
  }
  UndoObjects( items, at_DeleteType );
  ::ReleaseItems( items );

  items.DetachAll();
  matrs.Flush();

  EraseModel();
  RefreshOGL();
  drawModel.DetachInvisible( items, matrs, true ); 
  ::DeleteItems( items );
  ClearArraySelectedElements();
  ShowModel();

  SetWaitCursor( false );
}

/*
//------------------------------------------------------------------------------
// Изменить стиль линий выделенных объектов
// ---
void WorkWindow::SetSelectedObjectStyle( int w )
{
  //DeleteTemporal();
  RPArray<MbItem> geomItems( 0, 1 );
  geomModel.GetItems( geomItems );

  for ( size_t k = 0, kcnt = geomItems.Count(); k < kcnt; k++ ) {
    MbItem * geomItem = geomItems[k];
    if ( geomItem != nullptr && geomItem->IsSelected() )
      SetObjectStyle( geomItem, w );
  }
}
*/

//------------------------------------------------------------------------------
// Изменить толщину линий выделенных объектов
// ---
void WorkWindow::SetSelectedObjectWidth( int w )
{
  //DeleteTemporal();
  RefreshOGL();
  RPArray<MbItem> geomItems( 0, 1 );
  geomModel.GetItems( geomItems );

  for ( size_t k = 0, kcnt = geomItems.Count(); k < kcnt; k++ ) {
    MbItem * geomItem = geomItems[k];
    if ( geomItem != nullptr && geomItem->IsSelected() )
      SetObjectWidth( geomItem, w );
  }
}


//------------------------------------------------------------------------------
// Изменить цвет выделенных объектов
// ---
void WorkWindow::SetSelectedObjectColor( COLORREF newColor )
{
  RPArray<MbItem> geomItems( 0, 1 );
  geomModel.GetItems( geomItems );

  RPArray<MbItem> drawItems( 0, 1 );
  drawModel.GetItems( drawItems );

  size_t k, cnt;
  for ( k = 0, cnt = geomItems.Count(); k < cnt; k++ ) {
    MbItem * item = geomItems[k];
    if ( item != nullptr && item->IsSelected() ) {
      //if ( item == temporal.GetObject() )
      //  DeleteTemporal();
      item->SetColor( newColor );
    }
  }

  bool showItems = false;
  for ( k = 0, cnt = drawItems.Count(); k < cnt; k++ ) {
    MbItem * item = drawItems[k];
    if ( item != nullptr && item->IsSelected() ) {
      //if ( item == temporal.GetObject() )
      //  DeleteTemporal();
      item->SetColor( newColor );
      showItems = true;
    }
  }

  RefreshOGL();
  UnselectModel(); // Сбросим выделение

  if ( showItems )
    ShowModel();
}


//------------------------------------------------------------------------------
// Нахождение выделенных объектов
// ---
size_t WorkWindow::GetSelectedObjects( RPArray<MbItem> & modelItems ) 
{
  geomModel.GetSelected( modelItems );
  return modelItems.Count();
}


//------------------------------------------------------------------------------
// Цвет выделенния объектов
// ---
void WorkWindow::SetSelectColor( COLORREF newColor ) {
  select = newColor;
  ShowModel();
}


//------------------------------------------------------------------------------
// Изменить цвет окон
// ---
void WorkWindow::SetBackgrColor( COLORREF newColor ) 
{
  backColor = newColor; // Сохраняем для создания новых окон
  GetDrawTool().SetBackground( newColor );
#ifndef __USE_QT__
  //DrawContentWindow( Temporal & temporal, Templane & cplane, COLORREF select, bool isShowMapping ) 
#else // __USE_QT__
  InvalidateWindow( true );
#endif // __USE_QT__
  ShowModel();
}


//------------------------------------------------------------------------------
// Создать упрощенную триангуляцию
// ---
static void MakeConvexHull( const MbModel & drawModel ) 
{
  MbModel::ItemConstIterator drawIter( drawModel.CBegin() );
  MbModel::ItemConstIterator drawEnd ( drawModel.CEnd() );
  for ( ; drawIter != drawEnd; ++drawIter ) {
    const MbItem * drawItem = *drawIter;
    if ( drawItem != nullptr ) {
      if ( drawItem->IsA() == st_Mesh ) {
        MbMesh * mesh = (MbMesh *)drawItem;
        if ( mesh->GridsCount() > 0 ) {
          SArray<MbFloatPoint3D> points( 0, 1 );
          MbFloatPoint3D pnt;
          size_t j = 0, jCount = mesh->GridsCount();
          for ( j = 0; j < jCount; j++ ) {
            const MbGrid * grid = mesh->GetGrid( j );
            if ( grid != nullptr ) {
              for ( size_t k = 0, kCount = grid->PointsCount(); k < kCount; k++ ) {
                grid->GetPoint(k, pnt);
                points.Add( pnt );
              }
            }
          }
          for ( j = 0; j < jCount; j++ ) {
            const MbGrid * grid = mesh->DetachGrid( j );
            delete grid;
          }
          ::CreateConvexPolyhedron( points, mesh );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Изменение типа отрисовки
// ---
void WorkWindow::ChangeRenderType( bool convexHull ) 
{
  if ( GetRenderType() == wwrt_GDI ) {
    if ( !make_grid ) {
      bool wire = true;
      bool grid = true; // Растровое отображение
      double sag = Math::visualSag;
      // Наполнить плоскогранную модель.
      geomModel.FillMeshModel( MbStepData(ist_SpaceStep, sag), MbFormNote(wire, grid, true, TestVariables::exactMesh), drawModel );
      make_grid = true;
    }

    SetRenderType( wwrt_OGL );
    if ( convexHull ) {
      ::MakeConvexHull( drawModel ); 
      RefreshOGL();
    }
  }
  else if ( GetRenderType() == wwrt_OGL ) {
    SetRenderType( wwrt_GDI );
  }

  InvalidateWindow( false );
}


//------------------------------------------------------------------------------
// Изменение типа отрисовки. Change the sort for drawing.
// ---
void WorkWindow::ChangeViewSort( bool changeSort )
{
  if ( changeSort ) {
    switch ( viewSort ) {
      case wvs_Geometry : {
        viewSort = wvs_Constraints;
      } break;
      case wvs_Constraints : {
        viewSort = wvs_All;
      } break;
      default : {
        viewSort = wvs_Geometry;
      } break;
    }
  }
}


//------------------------------------------------------------------------------
// Изменение типа отрисовки
// ---
void WorkWindow::ChangeRenderTypeOGL() 
{
  OGLViewMode viewMode = ovm_NormalRenderWithEdges;

  if ( GetRenderType() == wwrt_OGL ) {
    viewMode = GetRenderTypeOGL();

    if ( viewMode == ovm_NormalRenderWithEdges )
      viewMode = ovm_IsoparamMesh;
    else if ( viewMode == ovm_IsoparamMesh )
      viewMode = ovm_NormalRenderWithEdges;
  }
  else {
    SetRenderType( wwrt_OGL );
  }

  SetRenderTypeOGL( viewMode );
  EraseModel();
  InvalidateWindow( false );
}


//------------------------------------------------------------------------------
// Получить тип отображения OGL
// ---
OGLViewMode WorkWindow::GetRenderTypeOGL() const { 
  return GetDrawTool().GetRenderMode();
}


//------------------------------------------------------------------------------
// Установить тип отображения OGL
// ---
void WorkWindow::SetRenderTypeOGL( OGLViewMode vm ) { 
  GetDrawTool().SetRenderMode( vm );
}


//------------------------------------------------------------------------------
// Удалить свойство
// ---
void WorkWindow::RemoveProperty( MbItem * geom )
{
  if ( geom != nullptr && geom->IsA() == st_Polymesh )
    return;

  SetWaitCursor( true );
  MbMatrix3D matr;
  MbItem * draw = (MbItem *)FindDrawItem( geom, matr );
  SetWaitCursor( false );

  if ( geom != nullptr && draw != nullptr )
  {
    EraseObject( geom );//draw );
    double sag = Math::visualSag;
    bool wire = true;  //mesh->ApexCount()  > 0 || mesh->PolygonsCount() > 0;
    bool grid = ( (GetRenderType() != wwrt_GDI) || make_grid ); // Растровое отображение
    MbRegDuplicate * iReg = nullptr;
    MbAutoRegDuplicate autoReg( iReg );
   
    if ( MbItem * newItem = geom->CreateMesh( MbStepData(ist_SpaceStep, sag), MbFormNote(wire, grid, true, TestVariables::exactMesh), iReg ) ) 
    {
      draw->AddRef();
      if ( drawModel.ReplaceItem( *draw, *newItem ) ) 
      {
        ShowObject( geom );//newItem );
      }
      ::DeleteItem( newItem );
      ::ReleaseItem( draw );
    }
  }
}

//------------------------------------------------------------------------------
// Запомнить объект
// ---
void WorkWindow::UndoObject( MbItem * item, ActionType act )
{
  if ( item != nullptr ) {
    undoItems.push_back( std::make_pair( SPtr<MbItem>(item), act ) );

    if ( act == at_DeleteType )
      CalculateGabarit();
    else if ( act == at_AddType )
      item->AddYourGabaritTo( cube ); // Добавь свой габарит в куб
  }
}


//------------------------------------------------------------------------------
// Запомнить объект
// ---
void WorkWindow::UndoObjects( RPArray<MbItem> & items, ActionType act )
{
  for ( size_t k = 0, kcnt = items.Count(); k < kcnt; k++ ) {
    MbItem * item = items[k];
    if ( item != nullptr ) {
      undoItems.push_back( std::make_pair( SPtr<MbItem>( item ), act ) );

      if ( act == at_AddType )
        item->AddYourGabaritTo( cube ); // Добавь свой габарит в куб
    }
  }
  if ( act == at_DeleteType )
    CalculateGabarit();
}


//------------------------------------------------------------------------------
// Количество элементов восстановления
// ---
ptrdiff_t WorkWindow::UndoCount() const
{
  return (ptrdiff_t)undoItems.size();
}


//------------------------------------------------------------------------------
// Тип восстановления
// ---
ActionType WorkWindow::UndoAction() const 
{
  ActionType actType = (undoItems.size() > 0 ) ? undoItems[undoItems.size()-1].second : at_AnyType;
  return actType;
}


//------------------------------------------------------------------------------
// Восстановить предыдущее
// ---
void WorkWindow::Undo()
{
  if ( undoItems.size() > 0 ) {
    ptrdiff_t i = (ptrdiff_t)undoItems.size() - 1;
    ActionType undoAction = undoItems[i].second;

    if ( undoAction == at_DeleteType ) { // Восстановление ранее удаленных объектов
      MbItem * geomItem = undoItems[i].first;
      geomItem->SetSelected( false );
      if ( geomItem->IsA() == st_Solid )
        ((MbSolid *)geomItem)->MakeRight(); // Установить правильную топологическую связь (указатели ребер на грани)
      geomModel.AddItem( *geomItem );
      geomItem->AddYourGabaritTo( cube ); // Добавь свой габарит в куб
      double sag = Math::visualSag;
      bool wire = true;  //mesh->ApexCount()  > 0 || mesh->PolygonsCount() > 0;
      bool grid = (GetRenderType() != wwrt_GDI) || make_grid; // Растровое отображение
      MbItem * drawItem = geomItem->CreateMesh( MbStepData(ist_SpaceStep, sag), MbFormNote(wire, grid, true, TestVariables::exactMesh), nullptr );
      if ( drawItem != nullptr ) {
        drawModel.AddItem( *drawItem, geomItem->GetItemName() );
        ShowObject( geomItem );//drawItem );
      }
    }
    else if ( undoAction == at_AddType ) { // Удаление ранее созданных объектов
      MbItem * geomItem = undoItems[i].first;
      MbMatrix3D matr;
      EraseObject( geomItem );
      MbItem * drawItem = (MbItem *)FindDrawItem( geomItem, matr );
      if ( drawItem != nullptr ) {
        geomModel.DetachItem( geomItem );
        drawModel.DeleteItem( drawItem );
        CalculateGabarit();
      }
    }

    undoItems.erase( undoItems.begin() + i );
  }
}


//------------------------------------------------------------------------------
// Пропустить восстановление предыдущего
// ---
void WorkWindow::UndoSkeep( ActionType act )
{
  ptrdiff_t i = (ptrdiff_t)undoItems.size() - 1;
  if ( i >= 0 ) {
    ActionType at = undoItems[i].second;
    if ( (at == at_DeleteType && act == at_DeleteType) ||
         (at == at_AddType    && act == at_AddType) ) 
    {
      undoItems.erase( undoItems.begin() + i );
    }
  }
}


//------------------------------------------------------------------------------
// Очистить
// ---
void WorkWindow::UndoFlush()
{
  // Инициализация селекторов
  Math::selectCurve   = nullptr;
  Math::selectSurface = nullptr;
  Math::selectEdge    = nullptr;
  Math::selectFace    = nullptr;
  Math::selectSolid   = nullptr;

  undoItems.clear();
}


////////////////////////////////////////////////////////////////////////////////
//
//                             Неклассные функции
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Добавить сетку объекта
// ---
bool AddGrid( const MbItem * geomItem, double sag, RPArray<MbGrid> & grids )
{
  bool result = ( geomItem != nullptr );
  if ( result ) {
    MbItem * drawItem = geomItem->CreateMesh( MbStepData(ist_SpaceStep, sag), MbFormNote(false, true, true, TestVariables::exactMesh), nullptr );
    result = false;
    if ( (drawItem != nullptr) && (drawItem->IsA() == st_Mesh) ) {
      MbMesh * meshItem = (MbMesh *)drawItem;
      result = ( meshItem->IsComplete() );
      if ( result ) {
        for ( size_t j = 0, jCount = meshItem->GridsCount(); j < jCount; j++ ) 
          grids.Add( meshItem->DetachGrid( 0 ) );
      }
    }
    ::DeleteItem( drawItem );
  }
  return result;
}


//------------------------------------------------------------------------------
// Дать кривую по объекту-кривой
// ---
static
MbCurve * GetCurveOnPlace( const MbItem * modelItem, MbPlacement3D & crvPlace, bool getContour )
{
  MbCurve * resCurve = nullptr;

  if ( modelItem != nullptr ) {
    if ( modelItem->IsA() == st_PlaneInstance ) {
      const MbPlaneItem * pItem = static_cast<const MbPlaneInstance *>( modelItem )->GetPlaneItem(); // Дать двумерный геометрический объект

      if ( pItem != nullptr ) {
        MbePlaneType pType = pItem->Family();
        if ( pType == pt_Curve ) {
          const MbCurve * pCurve = static_cast<const MbCurve *>( pItem );
          crvPlace.Init( static_cast<const MbPlaneInstance *>( modelItem )->GetPlacement() );
          if ( (pCurve->Type() == pt_Contour) || !getContour ) {
            resCurve = static_cast<MbCurve *>( &pCurve->Duplicate() );
          }
          else {
            resCurve = new MbContour();
            static_cast<MbContour *>(resCurve)->AddSegment( static_cast<MbCurve *>( &pCurve->Duplicate() ) );
          }
        }
        else if ( pType == pt_Region ) {
          const MbRegion * region = static_cast<const MbRegion *>( pItem );
          crvPlace.Init( static_cast<const MbPlaneInstance *>( modelItem )->GetPlacement() );
          PRECONDITION( region->GetContoursCount() < 2 );
          const MbContour * pContour = region->GetOutContour();
          if ( pContour != nullptr ) {
            resCurve = static_cast<MbContour *>( &pContour->Duplicate() );
          }
        }
      }
    }
    else 
    if ( modelItem->IsA() == st_SpaceInstance ) {
      const MbSpaceItem * sItem = static_cast<const MbSpaceInstance *>( modelItem )->GetSpaceItem(); // Дать трёхмерный геометрический объекта

      if ( (sItem != nullptr) && (sItem->Family() == st_Curve3D) ) {
        const MbCurve3D * spaceCurve = static_cast<const MbCurve3D *>( sItem );

        MbCurve * pCurve = nullptr;
        if ( spaceCurve->GetPlaneCurve( pCurve, crvPlace, false ) ) {
          if ( pCurve->IsA() == pt_ReparamCurve ) {
            pCurve = &pCurve->SetBasisCurve();
          }
          if ( (pCurve->Type() == pt_Contour) || !getContour )
            resCurve = static_cast<MbCurve *>( &::DuplicateIfUsed( *pCurve ) );
          else {
            resCurve = new MbContour;
            static_cast<MbContour *>(resCurve)->AddSegment( static_cast<MbCurve *>( &::DuplicateIfUsed( *pCurve ) ) );
          }
        }
      }
    }
  }

  return resCurve;
}


//------------------------------------------------------------------------------
// Дать двумерный контур по объекту-кривой
// ---
MbCurve * GetCurveOnPlace( const MbItem * modelItem, MbPlacement3D & crvPlace ) {
  return ::GetCurveOnPlace( modelItem, crvPlace, false );
}


//------------------------------------------------------------------------------
// Дать двумерный контур по объекту-кривой
// ---
MbContour * GetContourOnPlace( const MbItem * modelItem, MbPlacement3D & crvPlace )
{
  MbContour * resContour = nullptr;

  MbCurve * resCurve = ::GetCurveOnPlace( modelItem, crvPlace, true );

  if ( (resCurve != nullptr) && (resCurve->Type() == pt_Contour) )
    resContour = static_cast<MbContour *>( resCurve );
  else {
    ::DeleteItem( resCurve );
  }

  return resContour;
}


//------------------------------------------------------------------------------
// Дать пространственную кривую по объекту-кривой
// ---
SPtr<MbCurve3D> GetCurve3D( MbItem * obj, bool sameCurve )
{
  SPtr<MbCurve3D> curve;

  if ( obj != nullptr ) {
    if ( obj->IsA() == st_SpaceInstance ) {
      // Дать трёхмерный геометрический объект
      MbSpaceItem * spaceItem = static_cast<MbSpaceInstance *>( obj )->SetSpaceItem(); 
      if ( (spaceItem != nullptr) && (spaceItem->Family() == st_Curve3D) )
        curve = static_cast<MbCurve3D *>(spaceItem);
    }
    if ( obj->IsA() == st_PlaneInstance ) {
      // Дать двумерный геометрический объект
      MbPlaneItem * planeItem = static_cast<MbPlaneInstance *>( obj )->SetPlaneItem(); 

      if ( (planeItem != nullptr) && (planeItem->Family() == pt_Curve) ) {
        const MbPlacement3D & curvePlace = ((MbPlaneInstance *)obj)->GetPlacement();
        const MbCurve * c = static_cast<const MbCurve *>(planeItem);

        if ( c->Type() == pt_Contour ) { // BUG_74660
          const MbContour * contour = static_cast<const MbContour *>( c );
          if ( contour->GetSegmentsCount() > 0 ) {
            if ( contour->GetSegmentsCount() > 1 ) {
              SPtr<MbContour3D> cntr( new MbContour3D );
              for ( size_t k = 0, kcnt = contour->GetSegmentsCount(); k < kcnt; k++ ) {
                const MbCurve * segment = contour->GetSegment(k);
                if ( segment != nullptr ) {
                  SPtr<MbCurve3D> segm( new MbPlaneCurve( curvePlace, *segment, true ) );
                  if ( !cntr->AddCurveWithRuledCheck( *segm, METRIC_PRECISION ) ) {
                    cntr->SegmentsAdd( *segm );
                    PRECONDITION( false ); // контур-то с разрывами
                  }
                }
              }
              curve = ::DetachItem( cntr );
            }
            else if ( contour->GetSegment( 0 ) != nullptr ) {
              curve = new MbPlaneCurve( curvePlace, *contour->GetSegment(0), true );
            }
          }
        }
        else {
          curve = new MbPlaneCurve( curvePlace, *c, true );
        }
      }
    }
    if ( obj->IsA() == st_WireFrame ) {
      MbWireFrame * wireFrame = static_cast<MbWireFrame *>( obj );
      std::vector<SPtr<MbCurve3D> > curves;
      wireFrame->GetCurves( curves );
      if ( curves.size() == 1 ) {
        curve = curves[0];
      }
    }
  }

  if ( !sameCurve ) {
    if ( curve != nullptr )
      curve = static_cast<MbCurve3D *>(&curve->Duplicate());
  }

  return curve;
}

