#include <test_window.h>
#include <test_gr_draw.h>
#include <test_service.h>
#include <test_variables.h>
#include <test_converter.h>
#include <tool_time_test.h>
#include <templ_dptr.h>
#include <cur_polyline3d.h>
#include <cur_polyline.h>
#include <cur_contour.h>
#include <cur_line_segment.h>
#include <cur_line_segment3d.h>
#include <cur_plane_curve.h>
#include <cur_surface_intersection.h>
#include <surf_cone_surface.h>
#include <surf_plane.h>
#include <surf_sector_surface.h>
#include <surf_ruled_surface.h>
#include <surf_curve_bounded_surface.h>
#include <solid.h>
#include <point_frame.h>
#include <space_instance.h>
#include <plane_instance.h>
#include <instance_item.h>
#include <assembly.h>
#include <wire_frame.h>
#include <point_frame.h>
#include <action_solid.h>
#include <action_mesh.h>
#include <attr_geometric_attribute.h>
#include <test_variables.h>
#include <test_manager.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <alg_draw.h>
#include <tool_err_handling.h>
#include <last.h>
#include <topology_iterator.h>

using namespace c3d;


//#ifdef _DRAWGI
namespace DrawTeasWindowAdd {
  bool drawItems = false;
}
//#endif //_DRAWGI


//------------------------------------------------------------------------------
// Добавить в модель атрибуты с данными о виде.
// ---
void WorkWindow::AddViewData( bool change, MbModel * model )
{
  if ( model == nullptr ) 
    model = &geomModel;

  { // Сохранение системы координат экрана.
    std::vector<double> values = { place.GetOrigin().x, place.GetOrigin().y, place.GetOrigin().z,
                                   place.GetAxisX().x,  place.GetAxisX().y,  place.GetAxisX().z,
                                   place.GetAxisY().x,  place.GetAxisY().y,  place.GetAxisY().z,
                                   place.GetAxisZ().x,  place.GetAxisZ().y,  place.GetAxisZ().z };

    c3d::AttrVector attrs;
    MbDoubleVectorAttribute * vecAttr = nullptr;
    model->GetCommonAttributes( attrs, c3d::c3dStr_SavedViewPlacement, at_DoubleVectorAttribute );
    if ( attrs.size() ) {
      vecAttr = static_cast<MbDoubleVectorAttribute *>( attrs.front() );
      if ( vecAttr != nullptr && change )
        vecAttr->SetValue( values );
    }
    if ( vecAttr == nullptr ) {
      vecAttr = new MbDoubleVectorAttribute( c3d::c3dStr_SavedViewPlacement, true, values );
      model->AddAttribute( vecAttr );
    }
  }

  { // Сохранение UP-вектора.
  }

  { // Сохранение масштаба.
    c3d::AttrVector attrs;
    MbDoubleAttribute * doubleAttr = nullptr;
    model->GetCommonAttributes( attrs, c3d::c3dStr_SavedViewScale, at_DoubleAttribute );
    if ( attrs.size() ) {
      doubleAttr = static_cast<MbDoubleAttribute *>( attrs.front() );
      if ( doubleAttr != nullptr && change )
        doubleAttr->SetValue( scale );
    }
    if ( doubleAttr == nullptr ) {
      doubleAttr = new MbDoubleAttribute( c3d::c3dStr_SavedViewScale, true, scale );
      model->AddAttribute( doubleAttr );
    }
  }
  { // Сохранение типа вида ортогональный/перспективный.
    bool value = false; // ортогональный
    c3d::AttrVector attrs;
    MbBoolAttribute * boolAttr = nullptr;
    model->GetCommonAttributes( attrs, c3d::c3dStr_SavedViewPerspective, at_BoolAttribute );
    if ( attrs.size() ) {
      boolAttr = static_cast<MbBoolAttribute *>( attrs.front() );
      if ( boolAttr != nullptr && change )
        boolAttr->SetValue( value );
    }
    if ( boolAttr == nullptr ) {
      boolAttr = new MbBoolAttribute( c3d::c3dStr_SavedViewPerspective, true, value );
      model->AddAttribute( boolAttr );
    }
  }
}


//------------------------------------------------------------------------------
// Применить атрибуты с данными о виде, если они есть в текущей модели.
// ---
bool WorkWindow::SetViewData()
{
  // Очистить установки вида, если включена адаптация ЛСК окна.
  OrientateWindow( v_Isometry );
  SetScale( 1.0 );
  bool res = false;

  c3d::AttrVector attrsVec12, attrsVec3, attrsDouble, attrsBool;

  geomModel.GetCommonAttributes( attrsVec12, c3d::c3dStr_SavedViewPlacement, at_DoubleVectorAttribute );
  if ( attrsVec12.size() ) {
    MbDoubleVectorAttribute * vecAttr = static_cast<MbDoubleVectorAttribute *>( *attrsVec12.begin() );
    const std::vector<double> & values = vecAttr->GetValue();
    if ( values.size() == 12 ) {
      place.SetOrigin( MbCartPoint3D( values[0], values[1], values[2] ) );
      place.SetAxisX( MbVector3D( values[3], values[4], values[5] ) );
      place.SetAxisY( MbVector3D( values[6], values[7], values[8] ) );
      place.SetAxisZ( MbVector3D( values[9], values[10], values[11] ) );
      res = true;
    }
  }
  geomModel.GetCommonAttributes( attrsVec3, c3d::c3dStr_SavedViewUpVector, at_DoubleVectorAttribute );
  if ( attrsVec3.size() ) {
    MbDoubleVectorAttribute * vecAttr = static_cast<MbDoubleVectorAttribute *>( *attrsVec3.begin() );
    const std::vector<double> & values = vecAttr->GetValue();
    if ( values.size() == 3 ) {
    }
  }
  geomModel.GetCommonAttributes( attrsBool, c3d::c3dStr_SavedViewPerspective, at_BoolAttribute );
  if ( attrsBool.size() ) {
    res = true;
  }
  geomModel.GetCommonAttributes( attrsDouble, c3d::c3dStr_SavedViewScale, at_DoubleAttribute );
  if ( attrsDouble.size() ) {
    MbDoubleAttribute * doubleAttr = static_cast<MbDoubleAttribute *>( *attrsDouble.begin() );
    SetScale( doubleAttr->GetValue() );
    res = true;
  }
  return res;
}


//------------------------------------------------------------------------------
// Убрать из модели атрибуты с данными о виде.
// ---
void WorkWindow::RemoveViewData( MbModel * model )
{
  if ( model == nullptr )
    model = &geomModel;
  c3d::AttrVector attrsVec12, attrsVec3, attrsDouble, attrsBool;

  model->GetCommonAttributes( attrsVec12, c3d::c3dStr_SavedViewPlacement, at_DoubleVectorAttribute );
  if ( attrsVec12.size() ) {
    MbDoubleVectorAttribute * vecAttr = static_cast<MbDoubleVectorAttribute *>( *attrsVec12.begin() );
    model->RemoveAttribute( vecAttr );
  }
  model->GetCommonAttributes( attrsVec3, c3d::c3dStr_SavedViewUpVector, at_DoubleVectorAttribute );
  if ( attrsVec3.size() ) {
    MbDoubleVectorAttribute * vecAttr = static_cast<MbDoubleVectorAttribute *>( *attrsVec3.begin() );
    model->RemoveAttribute( vecAttr );
  }
  model->GetCommonAttributes( attrsBool, c3d::c3dStr_SavedViewPerspective, at_BoolAttribute );
  if ( attrsBool.size() ) {
    model->RemoveAttribute( *attrsBool.begin() );
  }
  model->GetCommonAttributes( attrsDouble, c3d::c3dStr_SavedViewScale, at_DoubleAttribute );
  if ( attrsDouble.size() ) {
    MbDoubleAttribute * doubleAttr = static_cast<MbDoubleAttribute *>( *attrsDouble.begin() );
    model->RemoveAttribute( doubleAttr );
  }
}


//------------------------------------------------------------------------------
// Запись модели в поток
// ---
void WorkWindow::WriteModel( writer & out ) 
{
  if ( cutModel == nullptr ) {
    ::WriteModelItems( out, geomModel );
  }
  else {
    MbModel * mixModel = &geomModel.Duplicate();
    mixModel->AddModel( drawModel );
    MbPlane * plane = new MbPlane( cutPlace );
    double d = cube.GetDiagonal() * ONE_THIRD;
    plane->SetLimit( d );
    MbItem * item = new MbSpaceInstance( *plane );
    item->SetColor( MB_DEFCOLOR );
    item->SetWidth( 1 );
    item->SetStyle( 1 );
    mixModel->AddItem( *item, UNDEFINED_SNAME );
    ::WriteModelItems( out, *mixModel );
    ::DeleteItem( item );
    ::DeleteItem( mixModel );
  }
}


//------------------------------------------------------------------------------
// Чтение модели из потока
// ---
void WorkWindow::ReadModel( reader & in )
{
  FatalErrorHandler::Reset();
  DeleteModel();

  bool success = ::ReadModelItems( in, geomModel ) || geomModel.ItemsCount() > 0;

#ifdef PLATFORM_32
  if ( success )
    if ( !FatalErrorHandler::IsEnoughMemory(0.65) ) {
      MessageBoxEx( RT_OUT_OF_MEM_WARN, IDS_TITLE_WRN );
      success = false;
    }

#endif

  if ( success ) {
    ProgressBarWrapper * parentProgress = in.GetProgress();
    SPtr<ProgressBarWrapper> myProgress;
    IProgressIndicator * commonProg = nullptr;
    if ( parentProgress != nullptr ) {
      StrData<MbeProgBarId_Common> strData( pbarId_Draw_Data );
      myProgress = &parentProgress->CreateChild( strData );
      in.InitProgress( *myProgress.get() );
      commonProg = parentProgress->GetCommonProgress();
    }
    if ( TestVariables::fitOnFirstLoading ) { // Включена адаптация ЛСК окна для текущей модели.
      bool appliedAttrs = SetViewData(); // Применить атрибуты с данными о виде, если они есть.
      if ( IsFirstLoading() && !appliedAttrs ) {
        FitModelToWindow();
        SetVisualSag();
      }
    }
    FatalErrorHandler::Reset();
    FillDrawModel( commonProg );
    if ( myProgress.get() )
      myProgress->SetProgress( 50 );
    CalculateGabarit();
    if ( myProgress.get() )
      myProgress->SetProgress( 75 );
    ShowModel();
    ::FinishProgressBar( myProgress.get() );
    if ( parentProgress != nullptr )
      in.ResetProgress(); // возвратиться к родительскому прогрессу

    MbResultType res = FatalErrorHandler::GetError();
    if ( res == rt_NotEnoughMemory ) {
      MessageBoxEx( RT_VIEW_MEMORY_ERROR, IDS_TITLE_WRN );
    }
    else if ( res != rt_Success ) {
      MessageBoxEx( RT_VIEW_ERROR, IDS_TITLE_WRN );
    }
  }

  FatalErrorHandler::Reset();
}

//------------------------------------------------------------------------------
// Чтение модели из потока (без визуализации).
// ---
bool WorkWindow::ReadModelItems( reader & in, std::vector<MbeSpaceType> & types, CSSArray<const MbItem *> & items )
{
  if ( ::ReadModelItems( in, geomModel ) ) {
    for ( size_t i = 0; i < types.size(); ++i )
      geomModel.GetUniqItems ( types[i], items );
    items.Sort();
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// Functions for debugging
//------------------------------------------------------------------------------
static bool DecomposeAssembly ( SPtr<MbItem> & object, std::vector< SPtr<MbItem> > & resItems )
{
  bool changed = false;

  if ( object != nullptr ) {
    std::vector< SPtr<MbItem> > subItems;

    MbMatrix3D matr;

    if ( object->IsA() == st_Assembly )
    {
      MbAssembly & assembly = static_cast<MbAssembly &>(*object);
      matr = MbMatrix3D::identity;

      if ( assembly.ItemsCount() > 0 )
      {
        for ( size_t i = 0, icnt = assembly.ItemsCount(); i < icnt; ++i ) {
          SPtr<MbItem> subItem(assembly.SetItem(i));
          if ( subItem != nullptr )
            ::DecomposeAssembly ( subItem, subItems );
        }
      }
    }
    else if ( object->IsA() == st_Instance )
    {
      MbInstance & instance = static_cast<MbInstance &>(*object);
      instance.GetPlacement().GetMatrixFrom ( matr );

      SPtr<MbItem> subItem(instance.SetItem());
      if ( subItem != nullptr )
        ::DecomposeAssembly ( subItem, subItems );
    }
    else if ( object->IsA() == st_PlaneInstance )
    {
      MbPlaneInstance & pInstance = static_cast<MbPlaneInstance &>(*object);
      const MbPlacement3D & place = pInstance.GetPlacement();

      if ( pInstance.PlaneItemsCount() > 1 )
      {
        for ( size_t i = 0, icnt = pInstance.PlaneItemsCount(); i < icnt; ++i ) {
          MbPlaneItem * pItem = pInstance.SetPlaneItem ( i );
          if ( pItem != nullptr ) {
            SPtr<MbItem> item(new MbPlaneInstance(*pItem, place));
            subItems.push_back ( item );
          }
        }
      }
      else {
        subItems.push_back ( object );
      }

    }
    else {
      subItems.push_back ( object );
    }

    if ( !subItems.empty() )
    {
      bool doTransform = !matr.IsSingle();
      size_t addCnt = subItems.size();
      for ( size_t k = 0; k < addCnt; ++k ) {
        SPtr<MbItem> & subItem = subItems[k];

        if ( doTransform )
          subItem = static_cast<MbItem *>(&subItem->Duplicate());
        else if ( std::find(resItems.begin(), resItems.end(), subItem) != resItems.end() )
          subItem = static_cast<MbItem *>(&subItem->Duplicate());
        else {
          for ( size_t l = k + 1; l < addCnt; ++l ) {
            if ( subItems[l] == subItem ) {
              subItem = static_cast<MbItem *>(&subItem->Duplicate());
              break;
            }
          }
        }
        if ( doTransform )
          subItem->Transform ( matr );

        resItems.push_back ( subItem );
        changed = true;
      }
    }
  }

  return changed;
}


//------------------------------------------------------------------------------
//
// ---
void DecomposeModel( MbModel & model, std::vector< SPtr<MbItem> > & resItems )
{
  RPArray<MbItem> items_all;
  model.GetItems ( items_all );
  for ( size_t i = 0; i < items_all.Count(); ++i ) {
    SPtr<MbItem> subItem(items_all[i]);
    ::DecomposeAssembly ( subItem, resItems );
  }
}


//------------------------------------------------------------------------------
//
// ---
void DecomposeItems ( std::vector< SPtr<MbItem> > & items, std::vector< SPtr<MbItem> > & resItems )
{
  for ( size_t i = 0; i < items.size(); ++i ) {
    ::DecomposeAssembly ( items[i], resItems );
  }
}


//------------------------------------------------------------------------------
// Search the same objects for 'items' in 'geomItems':
// missing will contain objects from 'items' which are missing in 'geomItems'
// similarMap will contain similar objects found for missing objects in 'geomItems'
// ---
//static 
void CompareItems ( /*[IN]*/std::vector< SPtr<MbItem> > & geomItems,
                    /*[IN]*/std::vector< SPtr<MbItem> > & items,
                    /*[OUT]*/std::vector<const MbItem *> & missing, 
                    /*[OUT]*/std::map<const MbItem *, std::vector<const MbItem *> > & similarMap )
{
  for ( size_t kk = 0; kk < items.size(); ++kk ) {
    MbItem* newItem = items[kk];
    bool found = false, similar = false;
    for ( size_t i = 0; i < geomItems.size(); ++i ) {
      const MbItem* oldItem = geomItems[i];
      if ( newItem->IsSame(*oldItem) ) {
        found = true;
        break;
      }
      else if ( newItem->IsSimilar(*oldItem) ) {
        similar = true;
        std::map<const MbItem*, std::vector<const MbItem *> >::iterator itr = similarMap.find(newItem);
        if ( itr != similarMap.end() ) {
          itr->second.push_back ( oldItem );
        }
        else {
          std::vector<const MbItem *> vec;
          vec.push_back ( oldItem );
          similarMap[newItem] = vec;
        }
      }
    }
    if ( !found ) {
      if ( !similar )
        missing.push_back ( newItem );
    }
    else {
      std::map<const MbItem *, std::vector<const MbItem *> >::iterator itr = similarMap.find(newItem);
      if ( itr != similarMap.end() )
        similarMap.erase ( itr );
    }
  }
}


//------------------------------------------------------------------------------
// Чтение модели из потока
// ---
void WorkWindow::ReadAddModel( reader & in )
{
  if ( geomModel.ItemsCount() > 0 ) {
    RefreshOGL();

    MbModel addModel;
    bool success = ::ReadModelItems( in, addModel ) || addModel.ItemsCount() > 0;
    if ( success ) {
#ifdef __DEBUG_TEST_TM__
      // TEST_TM compare objects from two models
      std::vector<const MbItem *> missing;
      std::map<const MbItem *, std::vector<const MbItem *> > similarMap;
      std::vector< SPtr<MbItem> > geomItems, items;

      CSSArray<const MbItem *> mitems( 0, 1 );
      geomModel.GetUniqItems ( st_Solid, mitems ); 
      mitems.Sort();
      for ( ptrdiff_t i = 0; i < mitems.Count(); ++i ) {
        SPtr<MbItem> subItem(const_cast<MbItem *>(mitems[i]));
        geomItems.push_back ( subItem );
      }

      CSSArray<const MbItem*> nitems( 0, 1 );
      addModel.GetUniqItems ( st_Solid, nitems ); 
      //nitems.Sort();
      for ( ptrdiff_t i = 0; i < nitems.Count(); ++i ) {
        SPtr<MbItem> subItem(const_cast<MbItem *>(nitems[i]));
        items.push_back ( subItem );
      }
      //::DecomposeModel(geomModel, geomItems);
      //::DecomposeModel(addModel, items);

      CompareItems ( geomItems, items, missing, similarMap );

#endif //__DEBUG_TEST_TM__

      bool wire = true;  // Векторное отображение
      bool grid = (GetRenderType() != wwrt_GDI) || make_grid; // Растровое отображение
      double sag = Math::visualSag;
      MbRegDuplicate * iReg = nullptr;
      MbAutoRegDuplicate autoReg( iReg );  

      MbModel::ItemConstIterator addIter( addModel.CBegin() );
      MbModel::ItemConstIterator addEnd ( addModel.CEnd() );
      for ( ; addIter != addEnd; ++addIter ) {
        const MbItem * geomItem = *addIter;
        if ( geomItem != nullptr ) {
          geomModel.AddItem( (MbItem &)*geomItem );
          MbItem * drawItem = geomItem->CreateMesh( MbStepData(ist_SpaceStep, sag), MbFormNote(wire, grid, true, TestVariables::exactMesh), iReg );
          if ( drawItem != nullptr ) {
            drawModel.AddItem( *drawItem, geomItem->GetItemName() );
          }
        }
      }
      ShowModel();
    }
  }
  else
    ReadModel( in );
}

//------------------------------------------------------------------------------
// 
//---
void WorkWindow::AddObjectsToModel( std::vector< SPtr<MbItem> >& ready )
{
  DeleteModel();

  for ( size_t i = 0; i < ready.size(); ++i ) {
    MbItem * item = ready[i];
    if ( item )
      geomModel.AddItem( (MbItem &)*item );
  }
  if ( geomModel.ItemsCount() > 0 ) {
    SetWaitCursor ( true );
    FillDrawModel();
    CalculateGabarit();
    ShowModel();
    SetWaitCursor ( false );
  }
}


//------------------------------------------------------------------------------
// Временная структура  - объект с координатным базисом
//---
struct MbGeometricalItem {
  SPtr<MbSpaceItem> gItem;   //  3D-объект
  MbMatrix3D        from;    //  матрица преобразования в мировую СК
  uint32            color;   //  цвет объекта
  bool              visible; //  видимость

public:
  MbGeometricalItem( MbSpaceItem & item, const MbMatrix3D & matToWorld, uint32 col, bool vis )
    : gItem  ( &item       )
    , from   ( matToWorld  )
    , color  ( col         )
    , visible( vis         )
  {}

  MbGeometricalItem(  const MbGeometricalItem & other )
    : gItem  ( other.gItem   )
    , from   ( other.from    )
    , color  ( other.color   )
    , visible( other.visible )
  {}
};


//------------------------------------------------------------------------------
// Чтение объекта в список
// ---
static bool ReadSpaceObjectToList( reader & in, MbMatrix3D & fromLocal, uint32 color, List<MbGeometricalItem> & geomItems )
{
  MbSpaceItem * item = nullptr;
  // Прочитать
  in >> item;

  bool visible = true;
  if ( in.MathVersion() >= 0x0E000010L )
    in >> visible;     

  // Добавить   
  if ( item != nullptr ) {
    if ( in.good() ) {
      if ( item->IsA() == st_Solid ) {
        MbSolid * solid = static_cast<MbSolid *>(item);    
        if ( solid->GetFacesCount() < 1 && solid->GetCreatorsCount() < 1 )
          ::DeleteItem( item ); // Пустышки не добавляем
      }
      if ( item != nullptr )
        geomItems.Add( new MbGeometricalItem( *item, fromLocal, color, visible ) );
      return true;
    }
    else {
      ::DeleteItem( item );
    }
  }
  else if ( in.good() ) // KYA K13+ Могут быть сломанные операции в дереве и у них нет математических объектов
    return true;
    
  return false;
}


//------------------------------------------------------------------------------
// Вставка объекта в модель посредством менеджера
// ---
static bool InsertSpaceObject( MbGeometricalItem & gi, WorkWindow & workWindow )
{
  Style cstyle( 1, RGB( 200, 125, 0   ));
  Style ustyle( 1, RGB( 0,   125, 0   ));
  Style sstyle( 1, RGB( 0,   0,   255 ));
  Style style;

  if ( gi.gItem != nullptr ) {
  
    gi.gItem->Transform( gi.from );

    if ( gi.gItem->IsA() == st_Solid )
      style = sstyle;
    else if ( gi.gItem->Family() == st_Surface )
      style = ustyle;
    else
      style = cstyle;

    if ( gi.color != 0 )
      style.Set( 0, 1, gi.color );

    MbItem * newObj = workWindow.AddObject( style, gi.gItem, true );
    if ( newObj != nullptr )
      newObj->SetVisible( gi.visible );
    
    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Рекуррентная функция чтения подмножества тел
// ---
static
bool ReadSubassembly( reader & in, MbMatrix3D & fromLocal, WorkWindow & workWindow, uint32 color, 
                      List<MbGeometricalItem> & geomItems, bool assembly )
{
  in >> color;

  if ( !in.good() )
    return false;

  // 1 // Чтение плейсментов и двумерных кривых на них
  {
    Style style( 1, RGB( 255, 0, 0 ) );  
    bool isSingle = fromLocal.IsSingle();

    MbPlacement3D place;
    MbPlaneItem * mathItem = nullptr;
    size_t sketchsCount = ReadCOUNT( in ); //OV_x64 in >> sketchsCount;
    for ( size_t i = 0; i < sketchsCount; i++ ) {
      in >> place; // Чтение плейсмента
      in >> mathItem; // Чтение двумерной кривой
      if ( mathItem != nullptr && in.good() ) {
        if ( !isSingle )
          place.Transform( fromLocal );
        workWindow.AddObject( style, mathItem, &place );
      }
      else {
        ::DeleteItem( mathItem );
        return false;
      }
    }
  }

  if ( !in.good() )
    return false;

  // 2 // Чтение пространственных объектов
  {
    size_t objsCount = ReadCOUNT( in ); //OV_x64 in >> objsCount; 
    for ( size_t i = 0; i < objsCount; i++ ) {
      if ( !ReadSpaceObjectToList( in, fromLocal, color, geomItems ) )
        return false;
    }
  }

  if ( !in.good() )
    return false;

  // 3 // Чтение тел
  size_t solidsCount = ReadCOUNT( in ); //OV_x64 in >> solidsCount;
  {
    for ( size_t i = 0; i < solidsCount ; i++ ) {
      if ( !ReadSpaceObjectToList( in, fromLocal, color, geomItems ) )
        return false;
    }
  }

  if ( in.MathVersion() >= 0x0D000013L ) { // ST 4.06.10
    // В конце базовой функции ComponentBase::WriteItem
    int separator;    
    in >> separator;
  }
  
  bool instance = assembly;
  if ( in.MathVersion() < 0x0D000013L ) { // ST 4.06.10
    if( !instance )
      instance = ( solidsCount == 0 );
  }

  if ( instance ) {

    // Этот блок должен работать только для сборки // ST для вставки    

    if ( in.MathVersion() < 0x0D000013L ) { // ST 4.06.10
      int separator;    // В соответствии с порядком записи
      in >> separator;
    }

    if ( solidsCount == 0 && in.eof() ) // Если тел нет и это конец файла
      return true; // То выходим

    {
      size_t mainSolidsCount = ReadCOUNT( in );  //OV_x64 in >> mainSolidsCount;
      for ( size_t i = 0; i < mainSolidsCount ; i++ ) {
        if ( !ReadSpaceObjectToList( in, fromLocal, color, geomItems ) )
          return false;
      }
    }

    if ( in.MathVersion() >= 0x0D000013L ) { // ST 4.06.10
      // В конце функции PartInstanceImp::WriteItemImp
      int separator;    
      in >> separator;
    }
  }
  
  if ( !in.good() )
    return false;

  // 4 // Чтение вложенных подсборок  
  {
    size_t itemsCount = ReadCOUNT( in ); //OV_x64 in >> itemsCount;
    for ( size_t i = 0; i < itemsCount; i++ ) {
      MbMatrix3D curr;
      in >> curr;
      if ( !ReadSubassembly( in, curr, workWindow, color, geomItems, true ) ) {
        return false;
      }
    }
  }
  
  return true;
}


//------------------------------------------------------------------------------
// Чтение множества объектов из *.mat
// ---
bool WorkWindow::ReadAssembly( reader & in )
{
  // 1 // Прочитать модель
  List<MbGeometricalItem> geomItems( true ); // Список пространственных объектов и их базисов
  MbMatrix3D fromLocal;
  fromLocal.Init(); // Сделать единичной матрицу 
  uint32 color = 0x56;
  bool isDone = ReadSubassembly( in, fromLocal, *this, color, geomItems, false );

  if ( isDone ||
       geomItems.Count() > 0 ) // KYA Если хоть что-то смогли прочитать, то добавляем
  { 
    // 2 // Создать дубликаты для повторяющихся объектов в сборке
    LIterator<MbGeometricalItem> iterator( geomItems );
    iterator.Restart();

    List<MbGeometricalItem> prepared( true ); // Список обработанных объектов

    while ( iterator.IsOK() ) {
      prepared.Add( iterator() );
      SPtr<MbSpaceItem> gItem = iterator()->gItem;
      iterator.Detach();  // Отцепить и продвинуть итератор

      while ( iterator.IsOK() ) {
        if ( iterator()->gItem->IsSame( *gItem ) ) {

          if ( gItem->IsA() == st_Solid ) {
            // Дублируем тело
            MbSolid     * solid = (MbSolid *)(MbSpaceItem *) gItem;
            MbFaceShell * shell = solid->GetShell();

            if ( shell == ((MbSolid *)(MbSpaceItem *)iterator()->gItem)->GetShell() ) {
              shell = shell->Copy( cm_Copy, nullptr );
              // Переписываем дубликатом тела без истории - при необходимости сдублировать и внести историю 
              if ( shell != nullptr )
                iterator()->gItem = new MbSolid ( shell, nullptr );
            }
          }
          else if ( iterator()->gItem == gItem ) {
            // Дублируем геометрический объект
            iterator()->gItem = &gItem->Duplicate();
          }

          prepared.Add( iterator() );
          iterator.Detach();  // Продвинуть и отцепить
        }
        else {
          iterator.Go();      // Продвинуть
        }
      } 
      iterator.Restart();
    }

    // 3 // Удалить одинаковые объекты
    List<MbGeometricalItem> ready ( true ); // Список обработанных объектов
    iterator.Set( prepared );
    iterator.Restart();

    while ( iterator.IsOK() ) {
      ready.Add( iterator() );
      MbMatrix3D & fromLocal_ = iterator()->from;
      SPtr<MbSpaceItem> gItem = iterator()->gItem;
      iterator.Detach();  // Отцепить и продвинуть итератор

      while ( iterator.IsOK() ) {
        if ( iterator()->gItem->IsSame(*gItem) && iterator()->from == fromLocal_ ) {
          // KYA удалять некому iterator.Detach();  // Продвинуть и отцепить
          iterator.Remove(); // Продвинуть и удалить
        }
        else
          iterator.Go();      // Продвинуть
      } 
      iterator.Restart();
    }

    // 3 // Внести обработанные объекты в модель
    iterator.Set( ready );
    for ( iterator.Restart(); iterator.IsOK(); iterator.Go() )
      InsertSpaceObject( *iterator(), *this );
  }

  return isDone;
}


//------------------------------------------------------------------------------
// Чтение объектов из потока
// ---
bool WorkWindow::ReadItem( reader & in )
{
  bool isRead = false;
  DeleteModel();

  if ( in.MathVersion() > 0x05900017L ) {
    // Чтение нового формата
    isRead = ReadAssembly( in );
  }
  else {
    isRead = true;
    // Чтение ранних версий
    Style pstyle( 1, RGB( 255,  0,    0   ));
    Style cstyle( 1, RGB( 200,  125,  0   ));
    Style ustyle( 1, RGB( 0,    125,  0   ));
    Style sstyle( 1, RGB( 0,    0,    255 ));
    Style style;

    if ( isRead ) {
      size_t n = ReadCOUNT( in );  //OV_x64 in >> n;
      for ( size_t i = 0; i < n; i++ ) {
        MbPlacement3D place_;
        MbPlaneItem * mathItem = nullptr;
        in >> place_;
        in >> mathItem;
        if ( mathItem != nullptr && in.good() )
          AddObject( pstyle, mathItem, &place_ );
        else {
          ::DeleteItem( mathItem );
          isRead = false;
          break;
        }
      }
    }

    if ( isRead ) {
      size_t n = ReadCOUNT( in );  //OV_x64 in >> n;
      for ( size_t i = 0; i < n; i++ ) {
        MbSpaceItem * item = nullptr;
        in >> item;
        if ( item && in.good() ) {
          MbSpaceItem * geomItem = item;
          if ( geomItem->IsA() == st_Solid )
            style = sstyle;
          else
          if ( geomItem->Family() == st_Surface )
            style = ustyle;
          else
            style = cstyle;
          AddObject( style, geomItem, true );
        }
        else  {
          ::DeleteItem( item );
          isRead = false;
          break;
        }
      }
    }
  }

  if ( geomModel.ItemsCount() > 0 ) {
    FillDrawModel();
    CalculateGabarit();
    ShowModel();
  }

  return isRead;
}


//------------------------------------------------------------------------------
// Запись объектов в поток
// ---
void WorkWindow::WriteItem( writer & out ) 
{
  MbPlacement3D place_;
  SArray<MbPlacement3D>  places    ( 0, 1 );  // Множество плоскостей
  RPArray<MbPlaneItem>   planeItems( 0, 1 );  // Множество двумерных объектов
  RPArray<MbSpaceItem>   spaceItems( 0, 1 );  // Множество пространственных объектов
  RPArray<MbSpaceItem>   solidItems( 0, 1 );  // Множество тел
  uint32 col = 0;

  MbModel::ItemConstIterator geomIter( geomModel.CBegin() );
  MbModel::ItemConstIterator geomEnd ( geomModel.CEnd() );
  for ( ; geomIter != geomEnd; ++geomIter ) 
  {
    const MbItem * geomItem = *geomIter;
    if ( geomItem->IsA() == st_SpaceInstance ) {
      MbSpaceItem * spaceItem = ((MbSpaceInstance *)geomItem)->SetSpaceItem();
      if ( spaceItem != nullptr ) {
        if ( spaceItem->IsA() == st_Solid ) {
          solidItems.Add( spaceItem );
          col = geomItem->GetColor();
        }
        else
          spaceItems.Add( spaceItem );
      }
    }
    else if ( geomItem->IsA() == st_PlaneInstance ) {
      place_ = ((MbPlaneInstance *)geomItem)->GetPlacement();
      MbPlaneItem * planeItem = ((MbPlaneInstance *)geomItem)->SetPlaneItem();
      if ( planeItem != nullptr ) {
        places.Add( place_ );
        planeItems.Add( planeItem );
      }
    }
  }

  out << col;
  // Запись плейсментов и двумерных кривых на них
  size_t i, n = planeItems.Count();
  WriteCOUNT( out, n ); //OV_x64 out << n;
  for ( i = 0; i < n; i++ ) {
    place_ = places[i];
    MbPlaneItem * planeItem = planeItems[i];
    out << place_;
    out << planeItem;
  }

  // Запись пространственных объектов
  n = spaceItems.Count();
  WriteCOUNT( out, n ); //OV_x64 out << n;
  for ( i = 0; i < n; i++ ) {
    MbSpaceItem * spaceItem = spaceItems[i];
    out << spaceItem;
  }

  // Запись тел
  n = solidItems.Count();
  WriteCOUNT( out, n ); //OV_x64 out << n;
  for ( i = 0; i < n; i++ ) {
    MbSpaceItem * solidItem = solidItems[i];
    out << solidItem;
  }

  // Запись вложенных подсборок
  n = 0;
  WriteCOUNT( out, n ); //OV_x64 out << n;
  WriteCOUNT( out, n ); //OV_x64 out << n;
  WriteCOUNT( out, n ); //OV_x64 out << n;
}


//------------------------------------------------------------------------------
// Заменить объект.
// ---
bool WorkWindow::ReplaceItem( MbItem & geomOld, MbItem & geomNew, bool saveName )
{
  bool result = false;
  bool wire = true;  // Векторное отображение
  bool grid = (GetRenderType() != wwrt_GDI) || make_grid; // Растровое отображение
  double sag = Math::visualSag;
  MbMatrix3D from_;

  RefreshOGL();

  geomOld.AddRef();
  MbItem * drawOld = (MbItem *)FindDrawItem( &geomOld, from_ );
  MbItem * drawNew = geomNew.CreateMesh( MbStepData(ist_SpaceStep, sag), MbFormNote(wire, grid, true, TestVariables::exactMesh), nullptr );

  if ( (drawOld != nullptr ) && (drawNew != nullptr) ) {
    result = geomModel.ReplaceItem( geomOld, geomNew, saveName );
    drawModel.ReplaceItem( *drawOld, *drawNew, saveName );
    if ( result ) {
      geomNew.AddYourGabaritTo( cube ); // Добавь свой габарит в куб
      UndoObject( &geomOld, at_DeleteType );
      UndoObject( &geomNew, at_AddType );
      if ( grid )
        ShowModel();
      else
        ShowObject( &geomNew );//drawNew );
    }
  }
  ::DeleteItem( drawNew );
  geomOld.DecRef();
  return result;
}


//------------------------------------------------------------------------------
// Обновить сетку для геометрического объекта.
// ---
void WorkWindow::RefreshDrawItem( MbItem & geomItem )
{
  MbMatrix3D from_;
  MbItem * drawItem = (MbItem *)FindDrawItem( &geomItem, from_ );

  if ( drawItem != nullptr && (drawItem->IsA() == st_Mesh) ) {
    MbMesh * drawMesh = static_cast<MbMesh *>( drawItem );
    drawMesh->Flush();
    bool wire = true;  // Векторное отображение
    bool grid = (GetRenderType() != wwrt_GDI) || make_grid; // Растровое отображение
    double sag = Math::visualSag;
    geomItem.AddYourMesh( MbStepData(ist_SpaceStep, sag), MbFormNote(wire, grid, true, TestVariables::exactMesh), *drawMesh ); 
  }
}


//------------------------------------------------------------------------------
// Добавление геометрического объекта
// ---
void WorkWindow::AddItem( MbItem & geomItem, const Style * style, bool showItem )
{
  geomModel.AddItem( geomItem );
  if ( style != nullptr ) {
    geomItem.SetColor( style->GetColor() );
    geomItem.SetWidth( style->GetWidth() );
    geomItem.SetStyle( style->GetStyle() );
  }
  geomItem.AddYourGabaritTo( cube ); // Добавь свой габарит в куб
  UndoObject( &geomItem, at_AddType );
  bool wire = true;  // Векторное отображение
  bool grid = (GetRenderType() != wwrt_GDI) || make_grid; // Растровое отображение
  double sag = Math::visualSag;

  RefreshOGL();

  MbItem * drawItem( nullptr );

  if ( geomItem.IsA() == st_Solid ) { // Напряженно деформированное состояние в виде набора треугольных граней. 
    MbSolid & solid = static_cast<MbSolid &>( geomItem );
    if ( !solid.IsClosed() && (solid.GetFacesCount() == 1) ) {
      AttrVector attrs;
      solid.GetAttributes( attrs, at_GeomAttribute ); // Выдать атрибуты заданного типа. Get attributes of a given type. 
      if ( attrs.size() == 1 && (attrs[0]->AttributeType() == at_GeomAttribute) ) {
        MbGeomAttribute * attr = (MbGeomAttribute *)attrs[0];
        if ( attr != nullptr ) {
          const MbSpaceItem * item = attr->GetSpaceItem();
          if ( item != nullptr && item->IsA() == st_Solid ) {
            drawItem = ((MbSolid *)item)->CreateMesh( MbStepData(ist_SpaceStep, sag), MbFormNote(wire, grid, true, TestVariables::exactMesh), nullptr );
          }
        }
      }
    }
  }

  if ( drawItem == nullptr )
    drawItem = geomItem.CreateMesh( MbStepData(ist_SpaceStep, sag), MbFormNote(wire, grid, true, TestVariables::exactMesh), nullptr );

  if ( drawItem != nullptr ) {
    drawModel.AddItem( *drawItem, geomItem.GetItemName() ); // Добавить объект 
    if ( showItem ) {
      if ( grid )
        ShowModel();
      else
        ShowObject( &geomItem );//drawItem );
    }
  }
  else 
    ::DeleteItem( drawItem );
}


//------------------------------------------------------------------------------
// Добавление геометрического объекта
// ---
MbItem * WorkWindow::AddObject( MbSpaceItem & sitem, bool showItem )
{
  MbItem * newObj = nullptr;
  {
    if ( sitem.Family() == st_Item ) {
      newObj = (MbItem *)&sitem;
    }
    else {
      switch ( sitem.Family() ) {
        case st_Point3D: {
          newObj = new MbSpaceInstance( (MbPoint3D &)sitem );
        } break;
        case st_Curve3D: {
          newObj = new MbSpaceInstance( (MbCurve3D &)sitem );
        } break;
        case st_Surface: {
          newObj = new MbSpaceInstance( (MbSurface &)sitem );
        } break;
        case st_Legend: {
          newObj = new MbSpaceInstance( (MbLegend &)sitem );
        } break;
        default: break;
      }
    }
    if ( newObj != nullptr )
      AddItem( *newObj, nullptr, showItem );
  }
  return newObj;
}


//------------------------------------------------------------------------------
// Добавление геометрического объекта
// ---
MbItem * WorkWindow::AddObject( const Style & style, MbSpaceItem * sitem, bool showItem )
{
  MbItem * newObj = nullptr;

  if ( sitem != nullptr ) {
    if ( sitem->Family() == st_Item ) {
      newObj = (MbItem *)sitem;
    }
    else {
      switch ( sitem->Family() ) {
        case st_Point3D : {
          newObj = new MbSpaceInstance( (MbPoint3D &)*sitem );
        } break;
        case st_Curve3D : {
          newObj = new MbSpaceInstance( (MbCurve3D &)*sitem );
        } break;
        case st_Surface : {
          newObj = new MbSpaceInstance( (MbSurface &)*sitem );
        } break;
        case st_Legend : {
          newObj = new MbSpaceInstance( (MbLegend &)*sitem );
        } break;
        default: break;
      }
    }
    if ( newObj != nullptr )
      AddItem( *newObj, &style, showItem );
  }
  return newObj;
}


//------------------------------------------------------------------------------
// Добавление плоского объекта
// ---
MbItem * WorkWindow::AddObject( const Style & style, MbPlaneItem * item, const MbPlacement3D * p, bool showItem )
{
  MbItem * newObj = nullptr;

  if ( item != nullptr ) {
    MbPlacement3D pl( GetOwnPlace() );
    //MbPlacement3D pl;

    if ( p != nullptr )
      pl.Init( *p );

    newObj = new MbPlaneInstance( *item, pl );
    AddItem( *newObj, &style, showItem );
  }
  return newObj;
}


//------------------------------------------------------------------------------
// Добавление плоского объекта
// ---
MbItem * WorkWindow::AddObject( const Style & style, const MbCartPoint & p0, const MbPlacement3D * p, bool showItem )
{
  MbPlacement3D pl( GetOwnPlace() );
  if ( p != nullptr )
    pl.Init( *p );

  MbCartPoint3D pnt;
  pl.PointOn( p0, pnt );

  MbPointFrame * newObj = new MbPointFrame( pnt );
  AddItem( *newObj, &style, showItem );

  return newObj;
}


//------------------------------------------------------------------------------
// Разрушить сборки с подсборками на составляющие
// ---
void WorkWindow::DecomposeAssemblies()
{
  EraseModel();
  drawModel.DeleteItems();
  geomModel.DecomposeAssemblies();
  RefreshModel();

  // Запомненный объект и его элемент
  TestVariables::ResetHLighted();   
}


//----------------------------------------------------------------------------------------
// Удалить визуальные свойства у тела
// ---
static
void RemoveColorAndVisualAttributesForFaces( MbSolid & solid )
{
  solid.RemoveSimpleAttribute( at_Color );
  solid.RemoveSimpleAttribute( at_Visual );
  for ( SolidFacesIterator faceItr( solid ); faceItr.undone(); faceItr.next() ) {
    MbTopologyItem & item( *faceItr );
    item.RemoveSimpleAttribute( at_Color );
    item.RemoveSimpleAttribute( at_Visual );
  }
  for ( SolidEdgesIterator edgeItr( solid ); edgeItr.undone(); edgeItr.next() ) {
    MbTopologyItem & item( *edgeItr );
    item.RemoveSimpleAttribute( at_Visual );
    item.RemoveSimpleAttribute( at_Color );
  }
}


//----------------------------------------------------------------------------------------
// Удалить визуальные свойства у сборки/детали
// ---
static
void RemoveColorAndVisualAttributesForCompexItem( MbItem & object )
{
  object.RemoveSimpleAttribute( at_Color );
  object.RemoveSimpleAttribute( at_Visual );

#ifdef _DRAWGI
  if ( DrawTeasWindowAdd::drawItems ) {
    DrawGI::DrawItem( &object, TRGB_AZURE );
    DrawGI::DrawItem( &object, TRGB_WHITE );
  }
#endif // _DRAWGI

  if ( object.IsA() == st_Assembly ) {
    MbAssembly & assembly = static_cast< MbAssembly & >( object );
    for ( AssemblyItemsIterator assmItr( assembly ); assmItr.undone(); assmItr.next() ) {
      MbItem & subItem( *assmItr );
      ::RemoveColorAndVisualAttributesForCompexItem( subItem );
    }
  }
  else if ( object.IsA() == st_Instance ) {
    MbInstance & instance = static_cast< MbInstance & >( object );
    MbItem & item( *instance.SetItem() );
    ::RemoveColorAndVisualAttributesForCompexItem( item );
  }
  else if ( object.IsA() == st_Solid ) {
    MbSolid & solid = static_cast< MbSolid & >( object );
    ::RemoveColorAndVisualAttributesForFaces( solid );
  }
  else if ( object.IsA() == st_WireFrame ) {
    MbWireFrame & frame = static_cast< MbWireFrame & >( object );
    for ( size_t i = 0, icnt = frame.GetEdgesCount(); i < icnt; i++ ) {
      MbEdge & edge( *frame.SetEdge( i ) );
      edge.RemoveSimpleAttribute( at_Color );
      edge.RemoveSimpleAttribute( at_Visual );
    }
  }
  else if ( object.IsA() == st_PointFrame ) {
    MbPointFrame & frame = static_cast< MbPointFrame & >( object );
    for ( size_t i = 0, icnt = frame.GetVerticesCount(); i < icnt; i++ ) {
      MbVertex & vert( *frame.SetVertex( i ) );
      vert.RemoveSimpleAttribute( at_Color );
      vert.RemoveSimpleAttribute( at_Visual );
    }
  }
}


//----------------------------------------------------------------------------------------
// Удалить все визуальные свойства
// ---
static
void RemoveAllColorAndVisualAttributes( RPArray<MbItem> & items )
{
  for ( size_t i = 0; i < items.size(); i++ ) {
    MbItem & item = *items[i];
    if ( item.IsA() == st_Solid ) {
      MbSolid & solid = static_cast< MbSolid & >( item );
      ::RemoveColorAndVisualAttributesForFaces( solid );
    }
    else if ( item.IsA() == st_Assembly || item.IsA() == st_Instance ||
              item.IsA() == st_WireFrame || item.IsA() == st_PointFrame ) {
      ::RemoveColorAndVisualAttributesForCompexItem( item );
    }
  }
}


//------------------------------------------------------------------------------
// Удалить все визуальные свойства объектов
// ---
void WorkWindow::RemoveColorAndVisualAttributes()
{
  EraseModel();
  drawModel.DeleteItems();
  RPArray<MbItem> items;
  GetAllObjects( items );
  RemoveAllColorAndVisualAttributes( items );
  RefreshModel();

  // Запомненный объект и его элемент
  TestVariables::ResetHLighted();   
}


//------------------------------------------------------------------------------
// Удалить все объекты
// ---
void WorkWindow::DeleteModel() 
{
  EraseModel();

  RefreshOGL();
  UndoFlush();

  drawModel.DeleteItems();
  geomModel.DeleteItems();
  drawModel.RemoveAttributes();
  geomModel.RemoveAttributes();
  cube.SetEmpty();
  ::ReleaseItem( cutModel );

  // Запомненный объект и его элемент
  TestVariables::ResetHLighted();
   

  if ( meshProcessor != nullptr )
    meshProcessor.reset();
}


//------------------------------------------------------------------------------
// Удалить все объекты
// ---
void WorkWindow::DeleteModelAttributes()
{
  EraseModel();

  RefreshOGL();
  UndoFlush();

  drawModel.RemoveAttributes();
  geomModel.RemoveAttributes();
  cube.SetEmpty();
  ::ReleaseItem( cutModel );

  // Запомненный объект и его элемент
  TestVariables::ResetHLighted();

  if ( meshProcessor != nullptr )
    meshProcessor.reset();
}


//------------------------------------------------------------------------------
// Удалить все плоские объекты модели
// ---
void WorkWindow::DeletePlaneItems() 
{
  DeleteByType( pt_PlaneItem );
}


//------------------------------------------------------------------------------
// Удалить все плоские объекты модели
// ---
void WorkWindow::DeleteSpaceItems() 
{
  DeleteByType( st_SpaceItem );
}


//------------------------------------------------------------------------------
// Дать все объекты указанного типа
// ---
void WorkWindow::GetObjects( MbeSpaceType type, std::vector<SPtr<MbItem>> & items, std::vector<MbMatrix3D> & matrs )
{
  geomModel.GetItems( type, items, matrs );
}

//------------------------------------------------------------------------------
// Дать все объекты указанного типа
// ---
void WorkWindow::GetObjects( MbeSpaceType type, RPArray<MbItem> & items, SArray<MbMatrix3D> & matrs )
{
  geomModel.GetItems( type, items, matrs );
/*

  for ( ptrdiff_t i = modelItems.MaxIndex(); i >= 0; i-- ) {
    MbItem * item_i = modelItems[i];
    if ( (type == st_Item)        || // Любой объект
         (type == st_SpaceItem)   || // Любой объект
         (type == item_i->IsA())  || // Объект данного класса
         (type == item_i->Type()) || // Объект данного семейства
         (type == item_i->Family()) ) { // Объект данного семейства
      items.Add( item_i );
    }
    if ( item_i->IsA() == st_SpaceInstance ) {
      const MbSpaceItem * spaceItem = ((MbSpaceInstance *)item_i)->GetSpaceItem(); // Дать трёхмерный геометрический объекта
      if ( spaceItem != nullptr ) {
        if ( (type == spaceItem->IsA())  || // Объект данного класса
             (type == spaceItem->Type()) || // Объект данного семейства
             (type == spaceItem->Family()) ) { // Объект данного типа
          items.Add( item_i );
        }
      }
    }
  }
*/
}


//------------------------------------------------------------------------------
// Дать все поверхности, тела и оболочки. Get all surfaces, bodies, and shells.
// ---
void WorkWindow::GetRenderingObjects( RPArray<MbItem> & renderItems )
{
  RPArray<MbItem> geomItems( 0, 1 );
  geomModel.GetItems( geomItems );

  for ( size_t i = 0; i < geomItems.Count(); i++ ) {
    MbItem * geomItem = geomItems[i];
    if ( geomItem != nullptr ) {
      if ( geomItem->IsA() == st_Solid ) 
        renderItems.Add( geomItem );
      else {
        if ( geomItem->IsA() == st_SpaceInstance ) {
          const MbSpaceItem * spaceItem = ((MbSpaceInstance *)geomItem)->GetSpaceItem(); // Дать трёхмерный геометрический объекта
          if ( spaceItem != nullptr && (spaceItem->Family() == st_Surface) )
            renderItems.Add( geomItem );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Дать все кривые и точки. Get all curves and points.
// ---
void WorkWindow::GetLineDrawingObjects( RPArray<MbItem> & wireItems )
{
  RPArray<MbItem> geomItems( 0, 1 );
  geomModel.GetItems( geomItems );

  for ( size_t i = 0, icnt = geomItems.Count(); i < icnt; i++ ) {
    MbItem * geomItem = geomItems[i];
    if ( geomItem != nullptr ) {
      if ( geomItem->IsA() == st_PointFrame )
        wireItems.Add( geomItem );
      else {
        if ( geomItem->IsA() == st_SpaceInstance ) {
          const MbSpaceItem * spaceItem = ((MbSpaceInstance *)geomItem)->GetSpaceItem(); // Дать трёхмерный геометрический объекта
          if ( (spaceItem != nullptr) && ((spaceItem->Family() == st_Curve3D) || (spaceItem->IsA() == st_Point3D)) )
            wireItems.Add( geomItem );
        }
        if ( geomItem->IsA() == st_PlaneInstance ) {
          const MbPlaneItem * planeItem = ((MbPlaneInstance *)geomItem)->GetPlaneItem(); // Дать двумерный геометрический объект
          if ( (planeItem != nullptr) && (planeItem->Family() == pt_Curve /*|| planeItem->IsAPoint()*/) ) {
            wireItems.Add( geomItem );
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Дать все объекты. Get all objects.
// ---
void WorkWindow::GetAllObjects( RPArray<MbItem> & items )
{
  geomModel.GetItems( items );
}


//------------------------------------------------------------------------------
// Добавить кривую в список кривых с разбивкой на составные части
// ---
static bool AddCurveArray( MbCurve & curve, bool breakCurve, RPArray<MbCurve> & curveList )
{
  bool res( false );

  if ( breakCurve ) // Разрушать сложные кривые
    switch ( curve.IsA() ) {

      case pt_ContourWithBreaks : 
      case pt_Contour : { // Контур
        MbContour * contour = (MbContour *)&curve;
        size_t segCount = contour->GetSegmentsCount(); // Выдать количество сегментов контура
        for ( size_t i = 0; i < segCount; i++ ) { // Выдать сегмент контура по индексу
          MbCurve * segm = contour->SetSegment( i );
          MbCurve * copy = static_cast<MbCurve *>( &segm->Duplicate() );
          curveList.Add( copy );
        }
        res = true;
      } break;

      case pt_Polyline : { // Полилиния
        MbPolyline * poly = (MbPolyline *)&curve;
        ptrdiff_t ip1, count = poly->GetPointListCount();
        for ( ptrdiff_t i = 0; i < poly->GetSegmentsCount(); i++ ) {
          ip1 = (i + 1) % count;
          if ( poly->GetPointList(i) != poly->GetPointList(ip1) )
            curveList.Add( new MbLineSegment( poly->GetPointList(i), poly->GetPointList(ip1) ) );
        }
        res = true;
      } break;

      default :
        curveList.Add( &curve );
    }
  else
    curveList.Add( &curve );

  return res;
}

//------------------------------------------------------------------------------
// Создать список математических кривых из объектов вида
// ---
void WorkWindow::CreateCurveArray( MbItem * obj, RPArray<MbCurve> & curves, bool breakCurve )
{
  MbPlacement3D place_; // Запоминаем ЛСК.
  if ( obj->IsA() == st_PlaneInstance && geomModel.ContainsItem( obj ) && obj->GetPlacement(place_) ) {
    MbMatrix toWindPlace;
    MbModel::ItemIterator geomIter( geomModel.Begin() );
    MbModel::ItemIterator geomEnd ( geomModel.End() );
    for ( ; geomIter != geomEnd; ++geomIter ) { 
      MbItem * item_i = *geomIter; // Берем объект модели.
      if ( item_i->IsA() == st_PlaneInstance ) { // Плоский объект.
        TestVariables::viewManager->SelectObject( item_i );

        MbPlaneInstance * planeInstance = static_cast<MbPlaneInstance *>(item_i);
        const MbPlacement3D & curvesPlace = planeInstance->GetPlacement(); // Берем ЛСК.
        if ( curvesPlace.GetMatrixToPlace( place_, toWindPlace ) ) { // Если ЛСК компланарны.
          size_t iCount = planeInstance->PlaneItemsCount(); // Соличество кривых в плоском объекте.
          const MbPlaneItem * planeItem( nullptr );
          for ( size_t i = 0; i < iCount; i++ ) {
            planeItem = planeInstance->GetPlaneItem( i ); // Двумерный геометрический объект.
            if ( (planeItem != nullptr) && (planeItem->Family() == pt_Curve) ) {
              const MbCurve * curve = static_cast<const MbCurve *>(planeItem);
              MbCurve & dupl = static_cast<MbCurve &>(curve->Duplicate()); // Копируем объект.
              dupl.Transform(toWindPlace); // Переводим копию в ЛСК исходного объекта.
              if ( ::AddCurveArray( dupl, breakCurve, curves ) ) { // Add объект.
                MbCurve * c = &dupl;
                ::DeleteItem( c );
              }
            }
          }
        }
      }
    }
//#ifdef _DRAWGI  
    if ( DrawTeasWindowAdd::drawItems ) {
      MbCartPoint3D p;
      for ( size_t j = 0, jCount = curves.Count(); j < jCount; j++ ) {
        MbCurve * pCurve = curves[j];
        MbPlaneCurve planeCurve( place_, *pCurve, false );
        double tMin = planeCurve.GetTMin(); 
        double tMax = planeCurve.GetTMax(); 
        double t = (0.1 * tMin) + (0.9 * tMax);
        planeCurve.PointOn( t, p );
        TestVariables::drawImp->DrawItem( &planeCurve, TRGB_LIGHTRED );
        TestVariables::drawImp->DrawItem( &planeCurve, TRGB_OLIVE );
        TestVariables::drawImp->DrawPoint( &p, TRGB_BLUE );
      }
    }
//#endif // _DRAWGI
  }
}


//------------------------------------------------------------------------------
// Найти объект по математическому объекту
// ---
const MbItem * WorkWindow::FindObject( const MbSpaceItem * item ) const
{
  MbMatrix3D from_;
  MbPath path;
  const MbItem * find = geomModel.FindItem( item, path, from_ );
  return find;
}


//------------------------------------------------------------------------------
// Найти объект по математическому объекту
// ---
const MbItem * WorkWindow::FindObject( const MbPlaneItem * item ) const
{
  MbMatrix3D from_;
  MbPath path;
  const MbItem * find = geomModel.FindItem( item, path, from_ );
  return find;
}


//------------------------------------------------------------------------------
// Вычислить габарит
// ---
void WorkWindow::CalculateGabarit() const
{
  cube.SetEmpty();
  geomModel.CalculateGabarit( cube );
}


//------------------------------------------------------------------------------
// Построить кривые
// ---
void WorkWindow::GetWire( const MbCartPoint3D & vista_, RPArray<MbCurve> & wire, SArray<Style> & styles )
{
  MbPlacement3D place_( GetOwnPlace());
  MbVector3D z( GetAxisZ() );
  MbRegDuplicate * iReg = nullptr;
  MbAutoRegDuplicate autoReg( iReg );  

  MbModel::ItemConstIterator geomIter( geomModel.CBegin() );
  MbModel::ItemConstIterator geomEnd ( geomModel.CEnd() );
  for ( ; geomIter != geomEnd; ++geomIter ) {
    const MbItem * geomItem = *geomIter;
  //for ( size_t j = 0; j < geomModel.ItemsCount(); j++ ) {
  //  const MbItem * geomItem = geomModel.GetModelItem(j);
    if ( geomItem != nullptr ) {
      MbItem * drawItem = geomItem->CreateMesh( MbStepData(ist_SpaceStep, Math::visualSag), MbFormNote(true, false, true, TestVariables::exactMesh), iReg );
      if ( drawItem != nullptr ) {
        size_t n = wire.Count();
        MakePlaneVistaWireFrame( *drawItem, place_, vista_, wire );
        for ( size_t k = n; k < wire.Count(); k++ )
          styles.Add( Style( geomItem->GetWidth(), geomItem->GetColor() ) );
      }
      ::DeleteItem( drawItem );
    }
  }
}


//------------------------------------------------------------------------------
// Построить каркас объекта
// ---
void WorkWindow::GetSpaceWire( const MbItem * geomItem, RPArray<MbCurve3D> & wire )
{
  if ( geomItem != nullptr ){
    //MbPlacement3D place( GetOwnPlacement());
    //MbVector3D z( GetAxisZ() );

    MbItem * drawItem = geomItem->CreateMesh( MbStepData(ist_SpaceStep, Math::visualSag), MbFormNote(true, false, true, TestVariables::exactMesh), nullptr );

    if ( drawItem != nullptr ) {
      MakeSpaceWireFrame( *drawItem, wire );
    }
    ::DeleteItem( drawItem );
  }
}


//------------------------------------------------------------------------------
// Построить каркас объекта
// ---
void WorkWindow::GetPlaneWire( const MbItem * geomItem, RPArray<MbCurve3D> & wire )
{
  if ( geomItem != nullptr ){
    MbPlacement3D place_( GetOwnPlace());
    //MbVector3D z( GetAxisZ() );

    MbItem * drawItem = geomItem->CreateMesh( MbStepData(ist_SpaceStep, Math::visualSag), MbFormNote(true, false, true, TestVariables::exactMesh), nullptr );

    if ( drawItem != nullptr ) {
      MbCartPoint3D vist( GetVista() );

      if (vist.z>DELTA_MIN) {
        double xFactor = GetDrawTool().GetXPixelPerMM();
        double yFactor = GetDrawTool().GetYPixelPerMM();
        double zFactor = ONE_HALF * ( xFactor + yFactor );
        vist.x /= xFactor;
        vist.y /= yFactor;
        vist.z /= zFactor;
        vist.y = -vist.y;
        MakePlaneVistaWireFrame( *drawItem, place_, vist, wire );
      }
      else
        MakePlaneWireFrame( *drawItem, place_, wire );
    }
    ::DeleteItem( drawItem );
  }
}


//------------------------------------------------------------------------------
// Построить каркас объекта
// ---
void WorkWindow::GetPlaneWire( const MbItem * geomItem, RPArray<MbCurve> & wire )
{
  if ( geomItem != nullptr ){
    MbPlacement3D place_( GetOwnPlace());
    //MbVector3D z( GetAxisZ() );

    MbItem * drawItem = geomItem->CreateMesh( MbStepData(ist_SpaceStep, Math::visualSag), MbFormNote(true, false, true, TestVariables::exactMesh), nullptr );

    if ( drawItem != nullptr ) {
      MbCartPoint3D vist( GetVista() );

      if (vist.z > DELTA_MIN) {
        double xFactor = GetDrawTool().GetXPixelPerMM();
        double yFactor = GetDrawTool().GetYPixelPerMM();
        double zFactor = ONE_HALF * ( xFactor + yFactor );
        vist.x /= xFactor;
        vist.y /= yFactor;
        vist.z /= zFactor;
        vist.y = -vist.y;
        MakePlaneVistaWireFrame( *drawItem, place_, vist, wire );
      }
      else
        MakePlaneWireFrame( *drawItem, place_, wire );
    }
    ::DeleteItem( drawItem );
  }
}


//------------------------------------------------------------------------------
// 
// ---
inline bool EqualGridPoints( MbCartPoint3D & pp1, MbCartPoint3D & pp2 )
{
  if ( c3d::EqualPoints( pp1, pp2, METRIC_EPSILON ) )
    return true;
  return false;
}


//------------------------------------------------------------------------------
// Построить каркасы триангуляционной сетки.
// ---
void WorkWindow::GetTriangulationPoints( const MbItem * obj, RPArray<MbPointFrame> & pointsData )
{
  RPArray<MbGrid> gridiron( 0, 1 );
  ::AddGrid( obj, Math::visualSag, gridiron );
  size_t gridsCnt = gridiron.Count();

  for ( size_t k = 0; k < gridsCnt; k++ ) {
    const MbGrid * grid = gridiron[k];
    if ( (grid == nullptr) || (!grid->IsComplete()) )
      continue;
    //    grid->DetachStrips(); // Связанные объекты превратить в обычные
    //    grid->ConvertQuadranglesToTriangles();

    MbCartPoint3D p0, p1, p2, p3;
    MbCartPoint3D pm;

    size_t tCount = grid->TrianglesCount();
    size_t qCount = grid->QuadranglesCount();
    size_t i;

    std::vector<MbCartPoint3D> gridPoints;
    gridPoints.reserve( tCount * 3 + qCount * 4 );

    for ( i = 0; i < tCount; i++ ) {
      if ( grid->GetTrianglePoints( i, p0, p1, p2 ) ) {
        gridPoints.push_back( p0 );
        gridPoints.push_back( p1 );
        gridPoints.push_back( p2 );
      }
    }
    for ( i = 0; i < qCount; i++ ) {
      if ( grid->GetQuadranglePoints( i, p0, p1, p2, p3 ) ) {
        gridPoints.push_back( p0 );
        gridPoints.push_back( p1 );
        gridPoints.push_back( p2 );
        gridPoints.push_back( p3 );
      }
    }
    size_t gridPointsCnt = gridPoints.size();

    std::sort( gridPoints.begin(), gridPoints.end() );
    gridPoints.erase( std::unique( gridPoints.begin(), gridPoints.end(), ::EqualGridPoints ), gridPoints.end() );
    gridPointsCnt = gridPoints.size();

    pointsData.Reserve( gridPointsCnt );

    MbPointFrame * pointsItem = new MbPointFrame( gridPoints );
    pointsData.Add( pointsItem );
  }
  ::DeleteItems( gridiron );
}


typedef std::pair<MbCartPoint3D,MbCartPoint3D> PointsPair;
typedef std::pair<MbCartPoint3D,PointsPair> SegmentData;


//------------------------------------------------------------------------------
// 
// ---
inline bool EqualGridEdges( SegmentData & pp1, SegmentData & pp2 )
{
  if ( c3d::EqualPoints( pp1.second.first, pp2.second.first, METRIC_EPSILON ) && c3d::EqualPoints( pp1.second.second, pp2.second.second, METRIC_EPSILON ) )
    return true;
  else if ( c3d::EqualPoints( pp1.second.first, pp2.second.second, METRIC_EPSILON ) && c3d::EqualPoints( pp1.second.second, pp2.second.first, METRIC_EPSILON ) )
    return true;

  return false;
}


//------------------------------------------------------------------------------
// Построить каркасы триангуляционной сетки.
// ---
void WorkWindow::GetTriangulationWires( const MbItem * obj, RPArray<MbWireFrame> & wires )
{
  RPArray<MbGrid> gridiron( 0, 1 );
  ::AddGrid( obj, Math::visualSag, gridiron );
  size_t gridsCnt = gridiron.Count();

  for ( size_t k = 0; k < gridsCnt; k++ ) {
    const MbGrid * grid = gridiron[k];
    if ( (grid == nullptr) || (!grid->IsComplete()) )
      continue;
//    grid->DetachStrips(); // Связанные объекты превратить в обычные
//    grid->ConvertQuadranglesToTriangles();

    MbCartPoint3D p0, p1, p2, p3;
    MbCartPoint3D pm;

    size_t tCount = grid->TrianglesCount();
    size_t qCount = grid->QuadranglesCount();
    size_t i;
    
    std::vector<SegmentData> segments;
    segments.reserve( tCount * 3 + qCount * 4 );

    for ( i = 0; i < tCount; i++ ) {
      if ( grid->GetTrianglePoints( i, p0, p1, p2 ) ) {
        pm.Set( p0, 0.5, p1, 0.5 );
        segments.push_back( std::make_pair( pm, std::make_pair( p0, p1 ) ) );
        pm.Set( p1, 0.5, p2, 0.5 );
        segments.push_back( std::make_pair( pm, std::make_pair( p1, p2 ) ) );
        pm.Set( p2, 0.5, p3, 0.5 );
        segments.push_back( std::make_pair( pm, std::make_pair( p2, p0 ) ) );
      }
    }
    for ( i = 0; i < qCount; i++ ) {
      if ( grid->GetQuadranglePoints( i, p0, p1, p2, p3 ) ) {
        pm.Set( p0, 0.5, p1, 0.5 );
        segments.push_back( std::make_pair( pm, std::make_pair( p0, p1 ) ) );
        pm.Set( p1, 0.5, p2, 0.5 );
        segments.push_back( std::make_pair( pm, std::make_pair( p1, p2 ) ) );
        pm.Set( p2, 0.5, p3, 0.5 );
        segments.push_back( std::make_pair( pm, std::make_pair( p2, p3 ) ) );
        pm.Set( p3, 0.5, p0, 0.5 );
        segments.push_back( std::make_pair( pm, std::make_pair( p3, p0 ) ) );
      }
    }
    size_t segmentsCnt = segments.size();

    std::sort( segments.begin(), segments.end() );
    segments.erase( std::unique( segments.begin(), segments.end(), ::EqualGridEdges ), segments.end() );
    segmentsCnt = segments.size();

    wires.Reserve( segmentsCnt );

    MbWireFrame * wire = new MbWireFrame;
    SPtr<MbLineSegment3D> segment;
    for ( i = 0; i < segmentsCnt; i++ ) {
      segment = new MbLineSegment3D( segments[i].second.first, segments[i].second.second );
      wire->AddEdge( *segment, true );
    }
    wires.Add( wire );
  }
  ::DeleteItems( gridiron );
}


#define CHECK_MESH_TO_BREP_TIME
//------------------------------------------------------------------------------
// Построить пластины триангуляционной сетки.
// ---
void WorkWindow::GetTriangulationPlates( const MbItem * obj, RPArray<MbSpaceItem> & plates )
{

  RPArray<MbGrid> gridiron(0,1);
  ::AddGrid( obj, Math::visualSag, gridiron );
  ptrdiff_t gridsCnt = gridiron.Count();

  bool meshToSolid = false;//true;

  if ( meshToSolid ) {
    DPtr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyPointsSurface ) );

#ifdef CHECK_MESH_TO_BREP_TIME
    size_t pointsCount = 0;
    size_t trianglesCount = 0;
    size_t quadranglesCount = 0;
    size_t facesCount = 0;
#endif

    MbSNameMaker snMaker( SimpleName(-1), MbSNameMaker::i_SideNone, 0 );

    SetWaitCursor( true );
    uint miliSec = ::GetTickCountEx();

    // Тест на многопоточность.
    bool useParallel = Math::CheckMultithreadedMode( mtm_Items ) && gridsCnt > 1;
    ENTER_PARALLEL( useParallel );
#pragma omp parallel for if (useParallel)
    for ( ptrdiff_t k = 0; k < gridsCnt; k++ ) {
      const MbGrid * grid = gridiron[k];
      if ( (grid == nullptr) || (!grid->IsComplete()) )
        continue;

      c3d::SolidSPtr solid;
      MbGridShellParams param( snMaker, progBar );
      ::GridSolid( *grid, param, solid );
#ifdef CHECK_MESH_TO_BREP_TIME

#pragma omp atomic
      pointsCount += grid->PointsCount();

#pragma omp atomic
      trianglesCount += grid->TrianglesCount();

#pragma omp atomic
      quadranglesCount += grid->QuadranglesCount();

      if ( k == gridsCnt -1 )
        facesCount = solid->GetFacesCount();
#endif

#pragma omp critical(GetTriangulationPlates)
      plates.Add( solid );
    }
    EXIT_PARALLEL( useParallel );

    miliSec = ::GetTickCountEx() - miliSec;
    SetWaitCursor( false );

#ifdef CHECK_MESH_TO_BREP_TIME
    size_t solidsCount = 1; 
    ::ShowTimeTestResult( nullptr, solidsCount, facesCount, pointsCount, trianglesCount, quadranglesCount,
                          miliSec, IDS_GRIDS_TO_SHELL );
#endif
  }
  else {
    for ( size_t k = 0; k < gridiron.Count(); k++ ) {
      const MbGrid * grid = gridiron[k];
      if ( (grid == nullptr) || (!grid->IsComplete()) )
        continue;
      //grid->DetachStrips(); // Связанные объекты превратить в обычные
      //grid->ConvertQuadranglesToTriangles();

      MbCartPoint3D p0, p1, p2, p3;
      MbCartPoint r0, r1, r2, r3;
      size_t i, count = grid->TrianglesCount();
      double accuracy = METRIC_PRECISION;
      for ( i = 0; i < count; i++ ) {
        if ( grid->GetTrianglePoints( i, p0, p1, p2 ) ) {
          //MbLineSegment3D * seg1 = new MbLineSegment3D( p0, p1 );
          //MbSurface * surf = new MbSectorSurface( *seg1, p2, true );
          //PRECONDITION( surf->IsPlanar() );
          //plates.Add( surf );
          if ( !p0.IsSame(p1, accuracy) && !p0.IsSame(p2, accuracy) && !p2.IsSame(p1, accuracy) ) {
            MbPlane * plane = new MbPlane( p0, p1, p2 );
            plane->NearPointProjection( p0, r0.x, r0.y, true, nullptr );
            plane->NearPointProjection( p1, r1.x, r1.y, true, nullptr );
            plane->NearPointProjection( p2, r2.x, r2.y, true, nullptr );
            MbLineSegment * seg0 = new MbLineSegment( r0, r1 );
            MbLineSegment * seg1 = new MbLineSegment( r1, r2 );
            MbLineSegment * seg2 = new MbLineSegment( r2, r0 );
            MbContour * cntr = new MbContour();
            cntr->AddSegment( seg0 );
            cntr->AddSegment( seg1 );
            cntr->AddSegment( seg2 );
            RPArray<MbContour> cntrs( 1, 1 );
            cntrs.push_back( cntr );
            MbCurveBoundedSurface * surf = new MbCurveBoundedSurface( *plane, cntrs, true );
            MbSurfaceIntersectionCurve * cur0 = new MbSurfaceIntersectionCurve( *plane, *seg0, *plane, *seg0, cbt_Boundary, false, false, nullptr );
            MbSurfaceIntersectionCurve * cur1 = new MbSurfaceIntersectionCurve( *plane, *seg1, *plane, *seg1, cbt_Boundary, false, false, nullptr );
            MbSurfaceIntersectionCurve * cur2 = new MbSurfaceIntersectionCurve( *plane, *seg2, *plane, *seg2, cbt_Boundary, false, false, nullptr );
            MbVertex * ver0 = new MbVertex( p0 );
            MbVertex * ver1 = new MbVertex( p1 );
            MbVertex * ver2 = new MbVertex( p2 );
            MbCurveEdge * edge0 = new MbCurveEdge( ver0, ver1, *cur0, true );
            MbCurveEdge * edge1 = new MbCurveEdge( ver1, ver2, *cur1, true );
            MbCurveEdge * edge2 = new MbCurveEdge( ver2, ver0, *cur2, true );
            MbLoop * loop = new MbLoop();
            loop->_AddEdge( *(new MbOrientedEdge(*edge0, true)) );
            loop->_AddEdge( *(new MbOrientedEdge(*edge1, true)) );
            loop->_AddEdge( *(new MbOrientedEdge(*edge2, true)) );
            MbFace * face = new MbFace( *loop, *surf, true );
            MbFaceShell * shell = new MbFaceShell( *face );
            MbSolid * solid = new MbSolid( shell, nullptr );
            plates.Add( solid );
          }
        }
      }
      count = grid->QuadranglesCount();
      for ( i = 0; i < count; i++ ) {
        if ( grid->GetQuadranglePoints( i, p0, p1, p2, p3 ) ) {
          MbLineSegment3D * seg1 = new MbLineSegment3D(p0,p1);
          MbLineSegment3D * seg2 = new MbLineSegment3D(p3,p2);
          MbSurface * surf = new MbRuledSurface( *seg1, *seg2, true );
          plates.Add( surf );
        }
      }
    }
  }

  ::DeleteItems( gridiron );

}


//------------------------------------------------------------------------------
// Дать габарит полигональной модели
// ---
void WorkWindow::GetDrawModelGabarit( MbCube & r )
{
  MbModel::ItemConstIterator drawIter( drawModel.CBegin() );
  MbModel::ItemConstIterator drawEnd ( drawModel.CEnd() );
  for ( ; drawIter != drawEnd; ++drawIter ) {
    const MbItem * drawItem = *drawIter;
    if ( (drawItem != nullptr) && (drawItem->IsA() == st_Mesh) )
      drawItem->AddYourGabaritTo( r );
  }
}


//------------------------------------------------------------------------------
// Разрезать модель
// ---
bool WorkWindow::CutModel( const MbPlacement3D & place_, double distance, size_t & operationCount ) 
{
  ::ReleaseItem( cutModel );

  RefreshOGL();

  cutPlace.Init( place_ );
  operationCount = drawModel.ItemsCount();

  cutModel = drawModel.CutMeshModel( place_, distance );
  if ( cutModel != nullptr ) {
    cutModel->AddRef();
    TestVariables::distance = distance;
  }
  return ( cutModel != nullptr );
}


//------------------------------------------------------------------------------
// Число треугольнико в сеточной модели.
// ---
size_t WorkWindow::GetTrianglesCount() const 
{ 
  MbMesh mesh;
  bool wire = true;  // Векторное отображение
  bool grid = (GetRenderType() != wwrt_GDI) || make_grid; // Растровое отображение
  double sag = Math::visualSag;
  drawModel.AddYourMesh( MbStepData(ist_SpaceStep, sag), MbFormNote(wire, grid, true, TestVariables::exactMesh), mesh ); 
  size_t trianglesCount = mesh.AllTrianglesCount() + mesh.AllQuadranglesCount();
  return trianglesCount; 
}


//------------------------------------------------------------------------------
// Наполнить плоскогранную модель
// ---
void WorkWindow::FillDrawModel( IProgressIndicator * prog )
{
  bool wire = true;  // Векторное отображение
  bool grid = ( (GetRenderType() != wwrt_GDI) || make_grid ); // Растровое отображение
  double sag = Math::visualSag;

  drawModel.RemoveAttributes();

  MbStepData stepData( ist_SpaceStep, sag, prog );
  //stepData.SetStepType( ist_DeviationStep );

  MbFormNote formNote(wire, grid, true, TestVariables::exactMesh);

  geomModel.FillMeshModel( stepData, formNote, drawModel );
  make_grid = grid;

  drawModel.SetModelName( geomModel.GetModelName() );
  geomModel.AttributesConvert( drawModel );
}


//------------------------------------------------------------------------------
// Найти соответствующий геометрический объект для полигонального объекта.
// ---
const MbItem * WorkWindow::FindGeomItem( const MbItem * drawItem, MbMatrix3D & from_ ) const
{
  const MbItem * findItem = nullptr;

  if ( drawItem != nullptr ) {
    MbPath path;
    if ( drawModel.MakePath( *drawItem, path, from_ ) ) {
      findItem = geomModel.GetItemByPath( path, from_ );
    }
  }
  return findItem;
}


//------------------------------------------------------------------------------
// Найти соответствующий полигональный объект для геометрического объекта.
// ---
const MbItem * WorkWindow::FindDrawItem( const MbItem * geomItem, MbMatrix3D & from_ ) const
{
  if ( geomItem != nullptr )
  {
    MbPath path;
    if ( geomModel.MakePath( *geomItem, path, from_ ) ) 
    {
      return drawModel.GetItemByPath( path, from_ );
    }
  }
  return nullptr;
}

//------------------------------------------------------------------------------
// Нарисовать объект в окне
// ---
void WorkWindow::PutObject( const MbItem * obj, bool s, bool eraseVertex ) 
{
  MbMatrix3D matr; 
  const MbItem * drwItem = FindDrawItem( obj, matr );
  if ( GetRenderType() == wwrt_GDI ) {
    if ( drwItem != nullptr ) {
      PutItemGDI( drwItem, matr, nullptr, nullptr, select, s, eraseVertex );
    }
#ifdef __USE_QT__
    InvalidateWindow( true );
#endif // __USE_QT__
  }
}

//------------------------------------------------------------------------------
// Нарисовать объект в окне
// ---
void WorkWindow::PutObject( const MbPath & obj, bool s, bool eraseVertex )
{
  MbMatrix3D matr; 
  const MbItem * drwItem = FindDrawItem( obj, matr );
  if ( GetRenderType() == wwrt_GDI ) {
    if ( drwItem != nullptr ) {
      PutItemGDI( drwItem, matr, nullptr, nullptr, select, s, eraseVertex );
    }
#ifdef __USE_QT__
    InvalidateWindow( true );
#endif // __USE_QT__
  }
}

//------------------------------------------------------------------------------
// Нарисовать объект в окне
/*
  obj     - Объект геометрической модели.
  drwObj  - Полигональный объект (отображение obj).
  drwMatr - Отображение полигонального объекта в Мир.
  item    - Частица объекта 'obj' (вершина,ребро,грань...).
*/
// ---
void WorkWindow::_PutObjectPart( const MbItem * obj, const MbItem * drwObj
                               , const MbMatrix3D & drwMatr, const MbRefItem * item, bool s )
{
  if ( GetRenderType() == wwrt_GDI ) 
  {    
    if ( drwObj != nullptr )
    {
      // KYA K17: Зачем копируем?
      //MbItem * drawItem = find->CreateMesh( MbStepData(ist_SpaceStep, Math::visualSag), MbFormNote(true, false, true, TestVariables::exactMesh), nullptr );
      //PutItemGDI( drawItem, matr, item, select, s );
      //::DeleteItem( drawItem );
      PutItemGDI( drwObj, drwMatr, item, nullptr, select, s );
    }
#ifdef __USE_QT__
    InvalidateWindow( true );
#endif // __USE_QT__
  }
  else if ( GetRenderType() == wwrt_OGL ) { // Подсветка части объекта в OpenGL
    if ( drawTool != nullptr ) {
      bool useSelectedColor = false;
      
      if ( drwObj != nullptr )
        useSelectedColor = !drwObj->IsSelected();
      else if ( (item != nullptr) && (obj != nullptr) )
        drwObj = obj;

      if ( (drwObj != nullptr) && (drwObj->IsA() == st_Mesh) ) {
        bool isTopItem = false;
        bool isSpaceItem = false;

        if ( item != nullptr ) {
          if ( item->RefType() == rt_TopItem )
            isTopItem = true;
          else if ( item->RefType() == rt_SpaceItem )
            isSpaceItem = true;
        }
        if ( isTopItem || isSpaceItem ) {
          const MbMesh * mesh = static_cast<const MbMesh *>( drwObj );
          SPtr<MbItem> itemMesh( mesh->CreateMeshByExistingItem( *item ) );
          if ( itemMesh != nullptr ) {
            if ( !drwMatr.IsSingle() ) // SD_7163577
              itemMesh->Transform( drwMatr ); // SD_7163577
            itemMesh->SetSelected( useSelectedColor );
            std::vector< SPtr<MbItem> > addItems;
            addItems.push_back( itemMesh );
            drawTool->ShowOGL( drawModel, select, &addItems );
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Добавить/Исключить (работает по принципу переключателя) элемент в массив выбранных пользователем объектов.
// ---
bool WorkWindow::ToggleElements( const MbItem * object, const MbRefItem * part )
{
  bool retVal = false;
  if ( (object != nullptr) ) {
    const MbeRefType rt = ( part == nullptr ) ? rt_PlaneItem : part->RefType();
    if ( (rt == rt_PlaneItem) || (rt == rt_SpaceItem) || (rt == rt_TopItem) ) {
      std::vector< PairObjectParts >::const_iterator it = vecSelElements.cbegin();
      for ( ; it != vecSelElements.cend(); it++ ) {
        if ( (it->first == object) && (it->second == part) )
          break;
      }
      
      if ( it == vecSelElements.cend() ) {  // Данных объектов в массиве нет ==> добавляем.
        vecSelElements.push_back( std::make_pair(SPtr<const MbItem>(object), SPtr<const MbRefItem>(part)) );
        retVal = true;
      }
      else {  // Удаляем найденные объекты из массива.
        DeleteObjectFromSelectedList( object, part );
        RedrawObject( object );
      }
    }
  }
  return retVal;
}


//------------------------------------------------------------------------------
// Нарисовать выбранные пользователем объекты в окне.
// ---
void WorkWindow::PutObjectsParts( )
{
  if ( !vecSelElements.empty() ) {
    if ( GetRenderType() == wwrt_GDI ) {
      MbMatrix3D drwMatr;
      for ( std::vector<PairObjectParts>::iterator it = vecSelElements.begin(); it != vecSelElements.end(); it++ ) {
        const MbItem * drwObj = FindDrawItem( ( it->first ), drwMatr );
        PutItemGDI( drwObj, drwMatr, it->second, nullptr, select, !it->first->IsSelected() );
      }
    }
    else if ( GetRenderType() == wwrt_OGL ) {
      MbMatrix3D drwMatr;
      std::vector<SPtr<MbItem> > arrItems;
      arrItems.reserve( vecSelElements.size() );

      for ( std::vector<PairObjectParts>::iterator it = vecSelElements.begin(); it != vecSelElements.end(); it++ ) {
        const MbItem * drwObj = FindDrawItem( it->first, drwMatr );

        if ( drwObj == nullptr )
          drwObj = it->first;

        if ( drwObj->IsA() == st_Mesh ) {
          if ( it->second == nullptr ) {
            SPtr<MbItem> added( const_cast<MbItem *>( drwObj ) );
            added->SetSelected( true );
            arrItems.push_back( added );
          }
          else {
            const MbMesh * mesh = static_cast<const MbMesh *>( drwObj );
            SPtr<MbItem> itemMesh( mesh->CreateMeshByExistingItem(*(it->second)) );
            if ( itemMesh ) {
              itemMesh->SetSelected( !drwObj->IsSelected() );
              arrItems.push_back( itemMesh );
            }
          }
        }
      }
      drawTool->ShowOGL( drawModel, select, &arrItems );
    }
  }
}


//------------------------------------------------------------------------------
// Нарисовать объект в окне.
// ---
void WorkWindow::PutObjectPart( const ItemElement & elem, bool s )
{
  MbMatrix3D drwMatr, m;
  const MbItem * obj = geomModel.GetItemByPath( elem.first, m );
  return _PutObjectPart( obj, FindDrawItem(elem.first, drwMatr), drwMatr, elem.second, s );
}

//------------------------------------------------------------------------------
// Нарисовать объект в окне
// ---
void WorkWindow::PutObjectPart( const MbItem * obj, const MbRefItem * item, bool s )
{
  MbMatrix3D drwMatr;
  return _PutObjectPart( obj, FindDrawItem(obj, drwMatr), drwMatr, item, s );
}

//------------------------------------------------------------------------------
// Нарисовать объект во всех окнах
// ---
void WorkWindow::ShowObject( const MbItem * obj )
{
  MbMatrix3D matr;   
  if ( GetRenderType() == wwrt_GDI ) {
    if ( const MbItem * find = FindDrawItem( obj, matr ) ) {
#ifndef __USE_QT__
      PutItemGDI( find, matr, nullptr, nullptr, select, find->IsSelected() );
#else // __USE_QT__
      InvalidateWindow( true );
#endif // __USE_QT__
    }
  }
  else if ( GetRenderType() == wwrt_OGL ) {
    ShowOGL();
  }
}


//------------------------------------------------------------------------------
// Стереть объект во всех окнах
// ---
void WorkWindow::EraseObject( const MbItem * obj ) 
{
#ifndef __USE_QT__
  MbMatrix3D matr;
  const MbItem * find = FindDrawItem( obj, matr );
#endif // __USE_QT__
  if ( GetRenderType() == wwrt_GDI ) {
#ifndef __USE_QT__
    PutItemGDI( find, matr, nullptr, nullptr, GetDrawTool().GetBackground(), true );
#else // __USE_QT__
    InvalidateWindow( true );
#endif // __USE_QT__
  }

  DeleteObjectFromSelectedList( obj );
}

//------------------------------------------------------------------------------
// Перерисовать объект
// ---
void WorkWindow::RedrawObject( const MbItem * geom ) 
{
  SetWaitCursor( true );
  MbMatrix3D matr;
  SPtr<const MbItem> draw( FindDrawItem(geom, matr) );
  SetWaitCursor( false );

  if ( (geom != nullptr) && (draw != nullptr) )
  {
    if ( GetRenderType() == wwrt_GDI ) {
#ifndef __USE_QT__
      PutItemGDI( draw, matr, nullptr, nullptr, GetDrawTool().GetBackground(), true );
#else // __USE_QT__
      InvalidateWindow( true );
#endif // __USE_QT__
    }
    else if ( GetRenderType() == wwrt_OGL ) {
      RefreshOGL();
    }

    bool wire = true;  //mesh->ApexCount()  > 0 || mesh->PolygonsCount() > 0;
    bool grid = (GetRenderType() != wwrt_GDI) || make_grid; // Растровое отображение
    SPtr<MbItem> newItem( geom->CreateMesh( MbStepData(ist_SpaceStep, Math::visualSag), MbFormNote(wire, grid, true, TestVariables::exactMesh), nullptr ) );

    if ( newItem != nullptr ) {
      if ( drawModel.ReplaceItem(*draw, *newItem) )
      {
        draw = newItem;
      }
    }
    if ( !grid )
      make_grid = false;
    if ( GetRenderType() == wwrt_GDI ) {
#ifndef __USE_QT__
      PutItemGDI( draw, matr, nullptr, nullptr, select, draw->IsSelected() );
#else // __USE_QT__
      InvalidateWindow( true );
#endif // __USE_QT__
    }
    else if ( GetRenderType() == wwrt_OGL ) 
    {
      ShowOGL();
    }
  }
}


//------------------------------------------------------------------------------
// Перерисовать объект
// ---
void WorkWindow::RefreshObjectModel( const MbItem * obj )
{
  RefreshOGL();
  RedrawObject( obj ); 
}


//------------------------------------------------------------------------------
// Выделить объект
// ---
void WorkWindow::SelectObject( const MbItem * geom, bool refreshDrawing )
{
  if ( geom != nullptr && !geom->IsSelected() ) {
    SetWaitCursor( true );
    MbMatrix3D matr;
    const MbItem * draw = FindDrawItem( geom, matr );
    SetWaitCursor( false );

    if ( refreshDrawing )
      RefreshOGL();
    if ( geom != nullptr )
      ((MbItem *)geom)->SetSelected( true );
    if ( draw != nullptr ) {
      ((MbItem *)draw)->SetSelected( true );
      if ( refreshDrawing )
        ShowObject( geom );
    }
  }
}

//------------------------------------------------------------------------------
// Выделить объект
// ---
void WorkWindow::SelectObject( const MbPath & geomPath, bool refreshDrawing )
{
  if ( !geomPath.empty() ) {
    SetWaitCursor( true );

    MbMatrix3D matr;
    const MbItem * geom = nullptr;
    const MbItem * draw = nullptr;
    if ( FindGeomDrawItem( geomPath, geom, draw, matr ) && !geom->IsSelected() ) {
      SetWaitCursor( false );

      if ( refreshDrawing )
        RefreshOGL();
      if ( geom != nullptr )
        ((MbItem *)geom)->SetSelected( true );
      if ( draw != nullptr ) {
        ((MbItem *)draw)->SetSelected( true );
        if ( refreshDrawing )
          ShowObject( geom );
      }
    }
    else {
      SetWaitCursor( false );
    }
  }
}

//------------------------------------------------------------------------------
// Выделить объект
// ---
void WorkWindow::UnselectObject( MbItem * geom, bool refreshDrawing )
{
  if ( geom != nullptr && geom->IsSelected() ) {
    SetWaitCursor( true );
    MbMatrix3D matr;
    const MbItem * draw = FindDrawItem( geom, matr );
    SetWaitCursor( false );

    if ( refreshDrawing )
      RefreshOGL();
    if ( geom != nullptr )
      ((MbItem *)geom)->SetSelected( false );
    if ( draw != nullptr ) {
      ((MbItem *)draw)->SetSelected( false );
      if ( refreshDrawing )
        ShowObject( geom );
    }

    DeleteObjectFromSelectedList( geom );
  }
}

//------------------------------------------------------------------------------
// Выделить объект
// ---
void WorkWindow::UnselectObject( const MbPath & geomPath, bool refreshDrawing )
{
  if ( !geomPath.empty() ) {
    SetWaitCursor( true );

    MbMatrix3D matr;
    const MbItem * geom = nullptr;
    const MbItem * draw = nullptr;
    if ( FindGeomDrawItem( geomPath, geom, draw, matr ) && geom->IsSelected() ) {
      SetWaitCursor( false );

      DeleteObjectFromSelectedList( geom );

      if ( refreshDrawing )
        RefreshOGL();
      if ( geom != nullptr )
        ((MbItem *)geom)->SetSelected( false );
      if ( draw != nullptr ) {
        ((MbItem *)draw)->SetSelected( false );
        if ( refreshDrawing )
          ShowObject( geom );
      }
    }
    else {
      SetWaitCursor( false );
    }
  }
}

//------------------------------------------------------------------------------
// Инвертировать выделение объекта
// ---
void WorkWindow::ToggleObject( const MbItem * geom, bool refreshDrawing )
{
  if ( geom != nullptr ) {
    SetWaitCursor( true );
    MbMatrix3D matr;
    const MbItem * draw = FindDrawItem( geom, matr );
    SetWaitCursor( false );
    bool s = geom->IsSelected();

    if ( refreshDrawing )
      RefreshOGL();
    ((MbItem *)geom)->SetSelected( !s );
    if ( draw != nullptr ) {
      ((MbItem *)draw)->SetSelected( !s );
      if ( refreshDrawing )
        ShowObject( geom );
    }
  }
}

//------------------------------------------------------------------------------
// Инвертировать выделение объекта
// ---
void WorkWindow::ToggleObject( const MbPath & geomPath, bool refreshDrawing )
{
  if ( !geomPath.empty() ) {
    SetWaitCursor( true );

    MbMatrix3D matr;
    const MbItem * geom = nullptr;
    const MbItem * draw = nullptr;
    if ( FindGeomDrawItem( geomPath, geom, draw, matr ) ) {
      SetWaitCursor( false );
      bool s = geom->IsSelected();

      if ( refreshDrawing )
        RefreshOGL();
      if ( geom != nullptr ) {
        ( (MbItem *)geom )->SetSelected( !s );
        if ( s )
          DeleteObjectFromSelectedList( geom );
      }
      if ( draw != nullptr ) {
        ( (MbItem *)draw )->SetSelected( !s );
        if ( s )
          DeleteObjectFromSelectedList( draw );
        if ( refreshDrawing )
          ShowObject( geom );
      }
    }
    else {
      SetWaitCursor( false );
    }
  }

  PutObjectsParts();
}


//------------------------------------------------------------------------------
// Изменить стиль линий объекта
// ---
void WorkWindow::SetObjectStyle( MbItem * geom, int w )
{
  if ( geom != nullptr ) {
    SetWaitCursor( true );
    MbMatrix3D matr;
    MbItem * draw = (MbItem * )FindDrawItem( geom, matr );
    SetWaitCursor( false );
    RefreshOGL();
    if ( geom != nullptr )
      geom->SetStyle( w );
    if ( draw != nullptr ) {
      EraseObject( geom );//draw );
      draw->SetStyle( w );
      ShowObject( geom );//draw );
    }
  }
}


//------------------------------------------------------------------------------
// Изменить толщину линий объекта
// ---
void WorkWindow::SetObjectWidth( MbItem * geom, int w )
{
  if ( geom != nullptr ) {
    SetWaitCursor( true );
    MbMatrix3D matr;
    MbItem * draw = (MbItem * )FindDrawItem( geom, matr );
    SetWaitCursor( false );
    RefreshOGL();
    if ( geom != nullptr )
      geom->SetWidth( w );
    if ( draw != nullptr ) {
      EraseObject( geom );
      draw->SetWidth( w );
      ShowObject( geom );
    }
  }
}


//------------------------------------------------------------------------------
// Изменить цвет объекта
// ---
void WorkWindow::SetObjectColor( MbItem * geom, COLORREF newColor )
{
  if ( geom != nullptr ) {
    SetWaitCursor( true );
    MbMatrix3D matr;
    MbItem * draw = (MbItem * )FindDrawItem( geom, matr );
    SetWaitCursor( false );
    RefreshOGL();
    if ( geom != nullptr )
      geom->SetColor( newColor );
    if ( draw != nullptr ) {
      draw->SetColor( newColor );
      ShowObject( geom );
    }
  }
}


//------------------------------------------------------------------------------
// Изменить видимость объекта
// ---
void WorkWindow::SetObjectVisible( MbItem * geom, bool visible )
{
  if ( geom != nullptr ) {
    SetWaitCursor( true );
    MbMatrix3D matr;
    MbItem * draw = (MbItem * )FindDrawItem( geom, matr );
    SetWaitCursor( false );
    RefreshOGL();
    if ( geom != nullptr )
      geom->SetVisible( visible );
    if ( draw != nullptr ) {
      draw->SetVisible( visible );
      ShowObject( geom );
    }
  }
}


//------------------------------------------------------------------------------
// Преобразование объекта согласно матрице
// ---
void WorkWindow::TransformObject( MbItem * geom, const MbMatrix3D & matr ) 
{
  if ( geom != nullptr ) {
    SetWaitCursor( true );
    MbMatrix3D from_;
    MbItem * draw = (MbItem * )FindDrawItem( geom, from_ );
    SetWaitCursor( false );
    if ( (geom != nullptr) && (draw != nullptr) ) {
      EraseModel();
      RefreshOGL();
      geom->Transform( matr );
      draw->Transform( matr );
      ShowModel();
    }
  }
}

//#define CHECK_MOVE_TIME
//------------------------------------------------------------------------------
// Сдвиг объекта
// ---
void WorkWindow::MoveObject( MbItem * geom, const MbVector3D & to ) 
{
  if ( geom != nullptr ) {
    SetWaitCursor( true );
    MbMatrix3D matr;
    MbItem * draw = (MbItem * )FindDrawItem( geom, matr );
    SetWaitCursor( false );
    if ( (geom != nullptr) && (draw != nullptr) ) {
      EraseModel();
      RefreshOGL();

#ifdef CHECK_MOVE_TIME
      TCHAR buf[1024];
      size_t len = 0;
      len += (size_t)_stprintf( buf+len, _T("Check results: \n\n") );
      size_t usedTime = ::GetTickCountEx();
#endif
      geom->Move( to );
#ifdef CHECK_MOVE_TIME
      usedTime = ::GetTickCountEx() - usedTime;
#ifdef  C3D_WINDOWS // _MSC_VER
      double usedTimeD = usedTime / 1000.0;
      len += (size_t)_stprintf( buf+len, RT_TIME_SEC, usedTimeD );
      ::MessageBoxEx( buf, IDS_TITLE_INFO );
#endif
#endif
      draw->Move( to );
      ShowModel();
    }
  }
}

//------------------------------------------------------------------------------
// Сдвиг объекта
// ---
void WorkWindow::MoveObject( const MbPath & geomPath, const MbVector3D & to ) 
{
  if ( !geomPath.empty() ) {
    SetWaitCursor( true );

    MbMatrix3D matr;
    const MbItem * geom = nullptr;
    const MbItem * draw = nullptr;
    if ( FindGeomDrawItem( geomPath, geom, draw, matr ) ) {
      SetWaitCursor( false );

      if ( (geom != nullptr) && (draw != nullptr) ) {
        EraseModel();
        RefreshOGL();

#ifdef CHECK_MOVE_TIME
        TCHAR buf[1024];
        size_t len = 0;
        len += (size_t)_stprintf( buf+len, RT_TEST_RESULTS );
        size_t usedTime = ::GetTickCountEx();
#endif
        const_cast<MbItem *>(geom)->Move( to );
#ifdef CHECK_MOVE_TIME
        usedTime = ::GetTickCountEx() - usedTime;
#ifdef  C3D_WINDOWS // _MSC_VER
        double usedTimeD = usedTime / 1000.0;
        len += (size_t)_stprintf( buf+len, RT_TIME_SEC, usedTimeD );
        ::MessageBoxEx( buf, IDS_TITLE_INFO );
#endif
#endif
        const_cast<MbItem *>(draw)->Move( to );
        ShowModel();
      }
    }
    else {
      SetWaitCursor( false );
    }
  }
}


//------------------------------------------------------------------------------
// Повернуть вокруг оси объекта
// ---
void WorkWindow::RotateObject( MbItem * geom, const MbAxis3D & axis, double angle ) 
{
  if ( geom != nullptr ) {
    SetWaitCursor( true );
    MbMatrix3D matr;
    MbItem * draw = (MbItem * )FindDrawItem( geom, matr );
    SetWaitCursor( false );
    if ( (geom != nullptr) && (draw != nullptr) ) {
      EraseModel();
      if ( drawTool != nullptr )
        drawTool->RefreshOGL();
      geom->Rotate( axis, angle );
      draw->Rotate( axis, angle );
      ShowModel();
    }
  }
}


//------------------------------------------------------------------------------
// Удаление объекта
// ---
void WorkWindow::DeleteObject( MbItem * geom ) 
{
  if ( geom != nullptr ) {
    SetWaitCursor( true );
    MbMatrix3D matr;
    MbItem * draw = (MbItem * )FindDrawItem( geom, matr );
    if ( (TestVariables::hLightedObj == geom) || (TestVariables::hLightedObj == draw) ) {
      TestVariables::ResetHLighted();
       
    }
    SetWaitCursor( false );
    if ( (geom != nullptr) && (draw != nullptr) ) {
      RefreshOGL();

      geom->AddRef();
      EraseObject( geom );//draw );
      if ( geomModel.DetachItem( (MbItem *)geom ) ) {
        drawModel.DeleteItem( (MbItem *)draw );
        UndoObject( geom, at_DeleteType );
      }
      geom->Release();
    }

    DeleteObjectFromSelectedList( geom );
  }
}

//------------------------------------------------------------------------------
// Удаление объекта
// ---
void WorkWindow::DeleteObject( const MbPath & geomPath ) 
{
  if ( !geomPath.empty() ) {
    SetWaitCursor( true );

    MbMatrix3D matr;
    const MbItem * geom = nullptr;
    const MbItem * draw = nullptr;
    if ( FindGeomDrawItem( geomPath, geom, draw, matr ) ) {
      SetWaitCursor( false );

      if ( (TestVariables::hLightedObj == geom) || (TestVariables::hLightedObj == draw) ) {
        TestVariables::ResetHLighted();
         
      }
      
      RefreshOGL();

      geom->AddRef();
      EraseObject( geom );
      if ( geomModel.DetachItem( (MbItem *)geom ) ) {
        drawModel.DeleteItem( (MbItem *)draw );
        UndoObject( (MbItem *)geom, at_DeleteType );
      }
      geom->Release();
    }
  }
}


//#define CHECK_DUPLICATE_TIME
//------------------------------------------------------------------------------
// Дублировать объект
// ---
void WorkWindow::DuplicateObject( const MbItem * geom ) 
{
  if ( geom != nullptr ) {
    MbRegDuplicate * iReg = nullptr;

#ifdef CHECK_DUPLICATE_TIME
    TCHAR buf[1024];
    size_t len = 0;
    len += (size_t)_stprintf( buf+len, _T("Check results: \n\n") );
    size_t usedTime = ::GetTickCountEx();
#endif

    MbItem & geomDup = (MbItem &)geom->Duplicate( iReg );

#ifdef CHECK_DUPLICATE_TIME
    usedTime = ::GetTickCountEx() - usedTime;
#ifdef  C3D_WINDOWS // _MSC_VER 
    double usedTimeD = usedTime / 1000.0;
    len += (size_t)_stprintf( buf+len, RT_TIME_SEC, usedTimeD );
    ::MessageBoxEx( buf, IDS_TITLE_INFO );
#endif
#endif

    geomDup.SetSelected( false );
    Style style( geom->GetWidth(), geom->GetColor() );
    AddItem( geomDup, &style, true );
  }
}


//------------------------------------------------------------------------------
// Дублировать объект
// ---
void WorkWindow::TransformDuplicateObject( const MbItem * geom, const MbMatrix3D & matr ) 
{
  if ( geom != nullptr ) {
    MbRegDuplicate * iReg = nullptr;

    MbItem & geomDup = (MbItem &)geom->Duplicate( iReg );

    geomDup.SetSelected( false );
    geomDup.Transform( matr );
    Style style( geom->GetWidth(), geom->GetColor() );
    AddItem( geomDup, &style, true );
  }
}


//------------------------------------------------------------------------------
// Найти вставку объекта или сам объект
// ---
bool WorkWindow::FindGeomDrawItem( const MbPath & path, const MbItem *& geom, const MbItem *& draw, MbMatrix3D & matr ) const
{
  geom = draw = nullptr;
  MbPath wrkPath( path );

  if ( path.size() > 1 ) {
    wrkPath.erase( wrkPath.begin() + wrkPath.size() - 1 );
    geom = FindGeomItem( wrkPath, matr );
    if ( geom != nullptr && geom->IsA() != st_Instance ) {
      wrkPath = path;
      geom = nullptr;
    }
  }
  if ( geom == nullptr )
    geom = FindGeomItem( wrkPath, matr );

  if ( geom != nullptr ) {
    MbMatrix3D drawMatr;
    draw = FindDrawItem( wrkPath, drawMatr );
    if ( draw != nullptr ) {
      if ( matr == drawMatr ) {
        return true;
      }
    }
  }

  PRECONDITION( false );
  return false;
}
