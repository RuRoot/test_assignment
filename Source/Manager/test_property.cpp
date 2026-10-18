#include <test_property.h>
#include <test_manager.h>
#include <test_service.h>
#include <test.h>
#include <test_property_title.h>
#include <mb_property.h>
#include <mb_placement.h>
#include <mb_placement3d.h>
#include <mb_point_mating.h>
#include <function.h>
#include <curve.h>
#include <curve3d.h>
#include <cur_polyline3d.h>
#include <surface.h>
#include <solid.h>
#include <instance_item.h>
#include <assembly.h>
#include <assisting_item.h>
#include <collection.h>
#include <mesh.h>
#include <point_frame.h>
#include <wire_frame.h>
#include <point3d.h>
#include <marker.h>
#include <mb_symbol.h>
#include <mb_thread.h>
#include <plane_instance.h>
#include <space_instance.h>
#include <attribute_item.h>
#include <cr_attribute_provider.h>
#include <region.h>
#include <generic_utility.h>
#include <alg_draw.h>
#include <cur_contour3d.h>
#include <polymesh_topo.h>
#include <polymesh.h>
#include <action_polymesh.h>
#include <halfedge_segm_iterators.h>

#include <last.h>

using namespace c3d;


#define PROMPT_LENGTH 82 ///< \ru Длина подсказки \en Length of hint

#ifdef C3D_WINDOWS
  #define PROPERTY_WINDOW_TITLE_FORMAT _T("%s")
  #define PROPERTY_NAME_SPACE_FORMAT _T("%s \t%s")
  #define PROPERTY_NAME_FORMAT _T("%s \t%s")
#else
  #ifdef _UNICODE
    #define PROPERTY_WINDOW_TITLE_FORMAT _T("%S")
    #define PROPERTY_NAME_SPACE_FORMAT _T("%S \t%S")
    #define PROPERTY_NAME_FORMAT _T("%S \t%S")
  #else
    #define PROPERTY_WINDOW_TITLE_FORMAT _T("%s")
    #define PROPERTY_NAME_SPACE_FORMAT _T("%s \t%s")
    #define PROPERTY_NAME_FORMAT _T("%s \t%s")
  #endif
#endif

//----------------------------------------------------------------------------------------
// Конструктор
// ---
PropertyWindow::PropertyWindow( MbProperty & rootProp, PropertyWindow * initParent ) 
  : MbRefItem()
  , propTool( nullptr )
  , parent( initParent )
  , parentProp( &rootProp )
  , child( nullptr )
  , obj( nullptr )
  , type( rootProp.IsA() )
  , properties()
  , changeable( rootProp.IsChangeable() && (initParent == nullptr || initParent->IsChangeable()) )
  , changed( false )
{
  rootProp.GET_PROPERTY_VALUE( obj );

  PRECONDITION( initParent != nullptr );
  if ( parent != nullptr ) // Блокируем родительское окно
  { 
    parent->child = this;
    parent->AddRef();
  }

  if ( obj != nullptr )
  {
    switch ( type ) 
    {
      case pt_ModelProp: // Отдельно рассмотрен случай
        _GetProperties( (MbModel*)obj, properties );
        break;

      case pt_TransactionsProp: // Отдельно рассмотрен случай
        _GetProperties( static_cast<MbTransactions*>(obj), properties ); // -->MbTransactions
        break;

      default: {
        // свойства грани не редактируемые, но атрибуты при этом можно редактировать
        if ( rootProp.IsChangeable() )
          changeable = true;
        rootProp.GetProperties( properties );
        break;
      }
    }
  }
  else {
    PRECONDITION( false ); // Нет объекта
  }

  CreatePropertyWindow(); // Собственно создание оконной части
}


//------------------------------------------------------------------------------
// Деструктор
// ---
PropertyWindow::~PropertyWindow()
{
  if( TestVariables::useHighlightObj )
      TestVariables::viewManager->UnselectModel();

  if ( parent != nullptr ) { // Снимаем флаг блокировки родительского окна
    if ( this == parent->child )
      parent->child = nullptr;
    parent->Release();
  }

  if ( changed )
    SetOwnProperties();

  delete propTool;
}


//------------------------------------------------------------------------------
// Установить признак измененности
// ---
void PropertyWindow::SetChanged()
{
  if ( parent != nullptr && type != pt_AttributeContainerProp )
    parent->SetChanged();
  changed = true;
}


//------------------------------------------------------------------------------
// Записать новые свойства в объект
// ---
void PropertyWindow::SetOwnProperties()
{
  if ( changeable && obj != nullptr )
  {
    if ( child != nullptr )
      child->SetOwnProperties();

    // MA 2015.06 (!) Есть возможность устранить этот switch. Использовать виртуальность MbProperty.
    switch ( type )
    {
      // Комплексные свойства двумерных объектов 
      case pt_CartPointProp :   // Точка-свойство
        ((MbCartPoint *)obj)->SetProperties( properties );
        break;
      case pt_VectorProp :      // Вектор-свойство
        ((MbVector *)obj)->SetProperties( properties );
        break;
      case pt_DirectionProp :   // Вектор-свойство
        ((MbDirection *)obj)->SetProperties( properties );
        break;
      case pt_PlacementProp :   // Система-свойство
        ((MbPlacement *)obj)->SetProperties( properties );
        break;
      case pt_MatrixProp :      // Матрица-свойство
        ((MbMatrix *)obj)->SetProperties( properties );
        break;
      case pt_CurveProp :  // Свойство плоской кривой
        static_cast<MbCurve *>(obj)->SetProperties( properties ); 
        break;

      // Комплексные свойства трёхмерных объектов 
      case pt_CartPoint3DProp :     // Точка-свойство
        ((MbCartPoint3D *)obj)->SetProperties( properties );
        break;
      case pt_Vector3DProp :    // Вектор-свойство
        ((MbVector3D *)obj)->SetProperties( properties );
        break;
      case pt_Placement3DProp : // Система-свойство
        ((MbPlacement3D *)obj)->SetProperties( properties );
        break;
      case pt_Matrix3DProp :    // Матрица-свойство
        ((MbMatrix3D *)obj)->SetProperties( properties );
        break;
      case pt_FloatPointProp : // Параметр-свойство
        ((MbFloatPoint *)obj)->SetProperties( properties );
        break;
      case pt_FloatPoint3DProp : // Вершина-свойство
        ((MbFloatPoint3D *)obj)->SetProperties( properties );
        break;
      case pt_FloatVector3DProp : // Нормаль-свойство
        ((MbFloatVector3D *)obj)->SetProperties( properties );
        break;
      case pt_TriangleProp : // Треугольник-свойство
        ((MbTriangle *)obj)->SetProperties( properties );
        break;
      case pt_QuadrangleProp : // Четырехугольник-свойство
        ((MbQuadrangle *)obj)->SetProperties( properties );
        break;
      case pt_ElementProp : // Элемент-свойство
        ((MbElement *)obj)->SetProperties( properties );
        break;
      case pt_GridProp : // Триангуляция-свойство
        ((MbGrid *)obj)->SetProperties( properties );
        break;
      case pt_PolymeshRegionProp: // Триангуляция-свойство
        ( (MbPolymeshRegionProperty *)obj )->SetProperties( properties );
        break;
      case pt_PolymeshSubProp: // Триангуляция-свойство
        ( (MbPolymeshSubRegionProperty *)obj )->SetProperties( properties );
        break;
      case pt_PolymeshLoopProp: // Триангуляция-свойство
        ( (MbPolymeshLoopProperty *)obj )->SetProperties( properties );
        break;
      case pt_PolymeshEdgeProp: // Триангуляция-свойство
        ( (MbPolymeshEdgeProperty *)obj )->SetProperties( properties );
        break;
      case pt_PolymeshVertexProp: // Триангуляция-свойство
        ( (MbPolymeshVertexProperty *)obj )->SetProperties( properties );
        break;
        
      // Комплексные свойства геометрических объектов 
      case pt_FunctionProp :   // Свойство функции
        ((MbFunction *)obj)->SetProperties( properties );
        break;
      case pt_Curve3DProp:       // Свойство кривой
        ((MbCurve3D *)obj)->SetProperties( properties );
        break;
      case pt_SurfaceProp :    // Свойство поверхности
        ((MbSurface *)obj)->SetProperties( properties );
        break;
      case pt_Point3DProp:        // Свойство точки
        ((MbPoint3D *)obj)->SetProperties( properties );
        break;
      case pt_MarkerProp:          // Свойство маркера ("точка присоединения").
        ((MbMarker *)obj)->SetProperties( properties );
        break;
      case pt_SymbolProp:          // Свойство условного обозначения.
        ((MbSymbol *)obj)->SetProperties( properties );
        break;
      case pt_ThreadProp:          // Свойство резьбы.
        ((MbThread *)obj)->SetProperties( properties );
        break;

      // Комплексные свойства топологических объектов 
      case pt_VertexProp:      // Свойство вершины
        ((MbVertex *)obj)->SetProperties( properties );
        break;
      case pt_CurveEdgeProp:    // Свойство ребра
        ((MbCurveEdge *)obj)->SetProperties( properties );
        break;
      case pt_EdgeProp:        // Свойство ребра
        ((MbEdge *)obj)->SetProperties( properties );
        break;
      case pt_OrientedEdgeProp: // Свойство ребра
        ((MbOrientedEdge *)obj)->SetProperties( properties );
        break;
      case pt_LoopProp:         // Свойство цикла
        ((MbLoop *)obj)->SetProperties( properties );
        break;
      case pt_FaceProp:         // Свойство грани
        ((MbFace *)obj)->SetProperties( properties );
        break;
      case pt_FaceShellProp:    // Обоболочк-свойство
        ((MbFaceShell *)obj)->SetProperties( properties );
        break;

      // Комплексные свойства строителей объектов 
      case pt_CreatorProp:        // Свойство оболочки
        ((MbCreator *)obj)->SetProperties( properties );
        break;
      case pt_NameProp:         // Свойство имени
        ((MbName *)obj)->SetProperties( properties );
        break;

      // Комплексные свойства объектов геометрической модели 
      case pt_ItemProp :  // Свойство объекта
        if ( changed )
          ((MbItem *)obj)->SetProperties( properties );
        break;
      case pt_SolidProp:        // Свойство тела
        if ( changed )
          ((MbSolid *)obj)->SetProperties( properties );
        break;
      case pt_InstanceProp:        // Свойство вставки
        if ( changed )
          ((MbInstance *)obj)->SetProperties( properties );
        break;
      case pt_AssemblyProp:        // Свойство тела
        if ( changed )
          ((MbAssembly *)obj)->SetProperties( properties );
        break;
      case pt_WireFrameProp:    // Свойство каркаса
        if ( changed )
          ((MbWireFrame *)obj)->SetProperties( properties );
        break;
      case pt_CollectionProp:   // Свойство тела
        if ( changed )
          ((MbCollection *)obj)->SetProperties( properties );
        break;
      case pt_MeshProp:        // Свойство полигональной модели
        if ( changed )
          ((MbMesh *)obj)->SetProperties( properties );
        break;

      // Комплексные свойства объектов 
      case pt_ModelProp:
        ((MbModel*)obj)->SetProperties( properties );
        break;
      case pt_TransactionsProp:
        ((MbTransactions*)obj)->SetProperties( properties );
        break;
      case pt_AttributeContainerProp:
        ((MbAttributeContainer*)obj)->SetProperties( properties );
        break;
      case pt_AttributeProp:
        ((MbAttribute*)obj)->SetProperties( properties );
        break;
      case pt_NamedAttributeContainerProp:
        ((MbNamedAttributeContainer*)obj)->SetProperties( properties );
        break;
      case pt_AttributeActionProp:
        ((MbAttributeAction*)obj)->SetProperties( properties );
        break;

      // Комплексные свойства геометрических ограничений
      case pt_PntMatingProp :
        ((PntMatingData2D *)obj)->SetProperties( properties );
        break;
      case pt_Pnt3DMatingProp :
        ((PntMatingData3D *)obj)->SetProperties( properties );
        break;

      default :
        if ( parentProp )
        {
          parentProp->SetProperties( properties );
          break;
        }       
        C3D_ASSERT_UNCONDITIONAL( "The parent property is unknown." );
        break;
    }

    if ( parent != nullptr && type != pt_AttributeContainerProp )
      parent->SetChanged();

    changed = false;
  }
}


//------------------------------------------------------------------------------
// Редактировать свойство
// ---
PropertyWindow * PropertyWindow::CreateProperty( size_t index )
{
  auto * pWnd = TestVariables::viewManager->GetActiveWindow();
  if ( pWnd != nullptr )
    pWnd->ShowOGL();
  MbProperty * indexProp = properties[index];
  PRECONDITION( indexProp != nullptr );
  return new PropertyWindow( *indexProp, this );
}


//------------------------------------------------------------------------------
// Создать окно свойств
// ---
void PropertyWindow::CreatePropertyWindow()
{
  const int npcMax = 17; // Максимальное количество строк в окне свойств
  TCHAR title[STRINGLENGTH];
  int npc = min_of( (int)(GetProperties().Count() + 2), npcMax );

  TCHAR name[PROMPT_LENGTH]; 
  ::GetPromptValue( const_cast<MbProperties &>(GetProperties()).SetName(), name ); // Получить строку по её значению в перечислении.
  _sntprintf( title, STRINGLENGTH, PROPERTY_WINDOW_TITLE_FORMAT, name ); // Имя объекта

#ifndef __USE_QT__ 
  HWND hprop = 0;
  HWND hlist = 0;
  HWND hedit = 0;
  if ( CreatePropertyClient( hprop, hlist, hedit, title, npc ) )
    propTool = CreatePropertyTool( hprop, hlist, hedit );
#else // __USE_QT__ 
  QWidget * hprop = nullptr;
  if ( CreatePropertyClient( hprop, title, npc ) )
    propTool = CreatePropertyTool( hprop );
#endif // __USE_QT__

  TCHAR s[STRINGLENGTH];
  // Окно для изменения численного значения
  if ( GetPropCount() > 0 ) 
  {
    Properties()[0]->GetCharValue( s );
    GetPropTool().SetEditText( s );
  }

  // Окно с перечнем свойств
  PArray<string_t> a;
  a.Reserve( GetPropCount() ); // C3D-289
  for ( size_t i = 0; i < GetPropCount(); i++ ) {
    TCHAR ss[STRINGLENGTH];
    TCHAR prompt[PROMPT_LENGTH]; 
    ::GetPromptValue( Properties()[i]->SetPrompt(), (TCHAR*)prompt ); // Получить строку по её значению в перечислении.
    Properties()[i]->GetCharValue( ss );
    _sntprintf( s, STRINGLENGTH, PROPERTY_NAME_SPACE_FORMAT, (TCHAR*)prompt, (TCHAR*)ss );
    a.Add( new string_t(s) );
  }

  GetPropTool().SetListTexts( a );
  GetPropTool().SetCurSelList( 0 );
  GetPropTool().Layout();
  GetPropTool().SetFocusList();
}


//------------------------------------------------------------------------------
// Записать новое свойство по индексу в списке
// ---
void PropertyWindow::SetString( TCHAR * s )
{
  TCHAR * p = _tcsstr( s, _T("\r\n") );
  if ( !p )
    p = _tcsstr( s, _T("\n\r") );
  if ( p )
    *p = 0;

  size_t currpos = GetPropTool().GetCurSelList();

  MbProperties & mp = Properties();
  mp[currpos]->SetPropertyValue( s );
  mp[currpos]->GetCharValue( s );

  TCHAR ss[STRINGLENGTH];
  TCHAR prompt[PROMPT_LENGTH]; 
  ::GetPromptValue( (GetProperties())[currpos]->SetPrompt(), (TCHAR*)prompt ); 
  _sntprintf( ss, STRINGLENGTH, PROPERTY_NAME_FORMAT, (TCHAR*)prompt, (TCHAR*)s );

  GetPropTool().SetEditText( s );
  GetPropTool().SetListText( currpos, ss );  
  GetPropTool().SetCurSelList( currpos );
  GetPropTool().SetFocusList();
}


//------------------------------------------------------------------------------
// Записать новое свойство по индексу в списке
// ---
void PropertyWindow::SetBoolean( bool & b )
{
  TCHAR s[STRINGLENGTH];

  size_t currpos = GetPropTool().GetCurSelList();

  MbProperties & mp = Properties();
  TCHAR bc = b;
  mp[currpos]->SetPropertyValue( &bc );
  mp[currpos]->GetCharValue( s );

  TCHAR ss[STRINGLENGTH];
  TCHAR prompt[PROMPT_LENGTH]; 
  ::GetPromptValue( (GetProperties())[currpos]->SetPrompt(), (TCHAR*)prompt ); 
  _sntprintf( ss, STRINGLENGTH, PROPERTY_NAME_FORMAT, (TCHAR*)prompt, (TCHAR*)s );

  GetPropTool().SetEditText( s );
  GetPropTool().SetListText( currpos, ss );  
  GetPropTool().SetCurSelList( currpos );
  GetPropTool().SetFocusList();
}


//------------------------------------------------------------------------------
// Записать новые свойства в объект
// ---
void PropertyWindow::SetProperties()
{
  TCHAR s[STRINGLENGTH];

  SetWaitCursor( true );
  TestVariables::viewManager->SetProperty( this );
  SetWaitCursor( false );

  // Переустановить свойства родителя
  if ( (parent != nullptr) && TestVariables::viewManager->IsMyProperty( parent ) )
  {
    size_t currpos = parent->GetPropTool().GetCurSelList();

    PArray<string_t> a;
    for ( size_t i = 0, icnt = parent->GetPropCount(); i < icnt; i++ ) {
      TCHAR ss[STRINGLENGTH];
      TCHAR prompt[PROMPT_LENGTH]; 
      ::GetPromptValue( parent->Properties()[i]->SetPrompt(), (TCHAR*)prompt ); 
      parent->Properties()[i]->GetCharValue( ss );
      _sntprintf( s, STRINGLENGTH, PROPERTY_NAME_FORMAT, (TCHAR*)prompt, (TCHAR*)ss );
      a.Add( new string_t(s) );
    }
    parent->GetPropTool().SetListTexts( a );
    parent->GetPropTool().SetCurSelList( currpos );
  }
}


//------------------------------------------------------------------------------
// Сменить текст в редакторе при изменении позиции в листе
// ---
void PropertyWindow::ChangeProperty()
{
  size_t currpos = GetPropTool().GetCurSelList();
  if ( currpos < Properties().Count() ) {
    MbProperty * prop = Properties()[currpos];
    TCHAR s[STRINGLENGTH];
    prop->GetCharValue( s );
    GetPropTool().SetEditText( s );

    if ( TestVariables::useHighlightObj )
      HighlightObj( *prop, s );
  }
}


//------------------------------------------------------------------------------
// Редактировать свойство
// ---
void PropertyWindow::EditProperty()
{
  size_t currpos = GetPropTool().GetCurSelList();
  if ( currpos < Properties().Count() )
  {
    switch ( Properties()[currpos]->IsA() ) 
    {
      case pt_BoolProp: // Логическое значение
      {
        if ( IsChangeable() ) 
        {
          bool value = false;
          Properties()[currpos]->GET_PROPERTY_VALUE( value );
          value = !value;
          SetBoolean( value );
        }
      } break;
      case pt_IntProp:      // Целое значение
      case pt_UIntProp:     // Целое значение
      case pt_DoubleProp:   // Действительное значение
      case pt_NDoubleProp:  // Действительное значение
      case pt_CharProp:     // Строковое значение
      case pt_StringProp:   // Строковое значение
      case pt_VersionProp:  // Версия
      {
        if ( IsChangeable() ) 
        {
          GetPropTool().SelectEditText();
          GetPropTool().SetFocusEdit();
        }
      } break;

      default:
      {        
        if ( PropertyWindow * np = CreateProperty(currpos) )
          TestVariables::viewManager->AddProperty( np );
      }
      break;
    }
  }
}

//------------------------------------------------------------------------------
//
// ---
bool PropertyWindow::PropertyCommand( uint command, uint subCommand, bool isReturn )
{
  switch ( command ) {
    case BUTTON_SET :       // Установка свойств в объект
    case BUTTON_SET_EXIT :  // Установка свойств в объект и выход
    { 
      SetProperties(); // Записать новые свойства в объект
      if ( command == BUTTON_SET_EXIT ) 
      {      
        GetPropTool().ClosePropertyWindow();
      }
    } break;

    case BUTTON_EXIT : // Выход без записи
    {
      GetPropTool().ClosePropertyWindow();
    } break;

    case H_LIST : {
      switch ( subCommand ) {
        case LBN_SELCHANGE : {
          ChangeProperty();
        } break;

        case LBN_DBLCLK : {
          if ( !IsBlocked() )
            EditProperty(); // Редактировать свойство
        } break;
      }
    } break;

    case H_EDIT : {
      if ( subCommand == EN_UPDATE && isReturn ) {
        TCHAR s[STRINGLENGTH];
        _tcscpy( s, _T("") );
        GetPropTool().GetEditText( s, STRINGLENGTH );
        SetString( s ); // Записать новое свойство
      }
    } break;

    default : 
      return false; // Не обработано
  }

  return true;
}

//------------------------------------------------------------------------------
//
// ---
bool PropertyWindow::KeyboardCommand( uint command, bool shift, bool ctrl )
{
  switch ( command ) {
    case VK_RETURN : {
      if ( ctrl || shift ) {
        SetProperties(); // Записать новые свойства в объект
        GetPropTool().ClosePropertyWindow();
      }
      else {
        if ( !IsBlocked() )
          EditProperty(); // Редактировать свойство
      }
    } break;

    case VK_ESCAPE : { 
      GetPropTool().ClosePropertyWindow();
    } break;

    default :
      return false; // Не обработанно
  }

  return true;
}


//------------------------------------------------------------------------------
//
// ---
void PropertyWindow::PreDestroyCommand( bool redraw )
{
  if ( child != nullptr ) {
    child->GetPropTool().ClosePropertyWindow();
    child->PreDestroyCommand( false );
  }

  if ( (parent != nullptr) && TestVariables::viewManager->IsMyProperty( parent ) )
    parent->GetPropTool().SetFocusList();

  TestVariables::viewManager->RemoveProperty( this, redraw );
}


//------------------------------------------------------------------------------
// Выделить начальную и конечную точку кривой
// ---
static
void DrawBoundaryPt( const MbCurve3D & crv, const MbMatrix3D & from, bool sense )
{
  MbCartPoint3D begPt;
  MbCartPoint3D endPt;

  crv.GetLimitPoint( sense ? 1 : 2, begPt );
  crv.GetLimitPoint( sense ? 2 : 1, endPt );

  begPt.Transform( from );
  endPt.Transform( from );
  TestVariables::drawImp->DrawPoint( &begPt, TRGB_LIGHTRED, 7 );
  TestVariables::drawImp->DrawPoint( &endPt, TRGB_LIGHTBLUE, 4 );
}


//------------------------------------------------------------------------------
// Выделить начальную и конечную точку каркаса
// ---
static
void DrawBoundaryPt( const MbWireFrame & wf )
{
  TestVariables::drawImp->DrawPoint( &wf.GetBegVertex()->GetCartPoint(), TRGB_LIGHTRED, 7 );
  TestVariables::drawImp->DrawPoint( &wf.GetEndVertex()->GetCartPoint(), TRGB_LIGHTBLUE, 4 );
}


//------------------------------------------------------------------------------
// Найти матрицу преобразования, если объект находится в Instance
// ---
static
void FindMatrix( const PropertyWindow * win, MbMatrix3D & from ) {

  if ( win ) {
    const PropertyWindow * cWin = win;

    const MbItem * prevObj = nullptr;
    MbPath path;
    MbMatrix3D from_;

    while ( cWin->GetParent() != nullptr ) {

      if ( cWin->GetPropType() == pt_InstanceProp || cWin->GetPropType() == pt_ItemProp ) {
        const MbItem * item = static_cast<const MbItem *>( cWin->GetObj() );

        if ( item->IsA() == st_Instance ) {
          item->FindItem( prevObj, path, from_ );

          if ( !from_.IsSingle() )
            from = from_;
        }
      }
      prevObj = static_cast<const MbItem *>( cWin->GetObj() );
      cWin = cWin->GetParent();
    }
  }
}


//------------------------------------------------------------------------------
// Нарисовать сетку для подсветки.
// ---
static
void DrawHighlightedMesh( const MbMatrix3D & matr, WorkWindow * pWnd, MbMesh * pMesh )
{
  if ( pWnd == nullptr || pMesh == nullptr )
    return;

  SPtr<MbVisual> aColor( MbVisual::Create( TRGB_LIGHTGREEN ) );
  pMesh->AddAttribute( aColor );

  SPtr<MbItem> itemMesh( pMesh );
  if ( !matr.IsSingle() )
    itemMesh->Transform( matr );
  itemMesh->SetSelected( true );
  std::vector< SPtr<MbItem> > addItems;
  addItems.push_back( itemMesh );
  pWnd->ShowOGL( &addItems );
}


//------------------------------------------------------------------------------
// Найти указатель на полигональную сетку в родительских окнах.
// ---
static
const MbItem * FindPolymesh( const PropertyWindow * win ) 
{
  const MbItem * pPoly = nullptr;
  if ( win ) {
    const PropertyWindow * cWin = win;

    while ( cWin->GetParent() != nullptr ) {

      if ( cWin->GetPropType() == pt_InstanceProp || cWin->GetPropType() == pt_ItemProp ) {
        const MbItem * item = static_cast<const MbItem *>( cWin->GetObj() );

        if ( item->IsA() == st_Polymesh ) {
          pPoly = item;
          break;
        }
      }
      cWin = cWin->GetParent();
    }
  }
  return pPoly;
}


//------------------------------------------------------------------------------
// Выделить выбранный в меню объект
// ---
void PropertyWindow::HighlightObj( MbProperty & prop, TCHAR * s )
{
  auto * pWnd = TestVariables::viewManager->GetActiveWindow();
  MbMatrix3D from_;
  MbPath path;

  if ( pWnd != nullptr ) {
    switch ( type ) {
    case pt_ModelProp: // Свойство модели.
    {
      if ( prop.IsA() == pt_ItemProp ) {
        const MbItem * item = nullptr;
        prop._GetPropertyValue( &item, 0 );

        if ( item != nullptr ) {
          if ( item->IsA() == st_Polymesh ) {
            MbMatrix3D matr;
            auto * pDraw = pWnd->FindDrawItem( item, matr );
            if ( pDraw != nullptr ) {
              const MbMesh * pMeshDraw = static_cast<const MbMesh *>( pDraw );
              MbMesh * pMesh = static_cast<MbMesh *>( &pMeshDraw->Duplicate() );
              DrawHighlightedMesh( matr, pWnd, pMesh );
            }
          }
          else {
            TestVariables::viewManager->UnselectModel();
            TestVariables::drawImp->DrawItem( item, TRGB_LIGHTGREEN );
            if ( item->IsA() == st_SpaceInstance ) {
              const MbSpaceItem * spItem = ( static_cast<const MbSpaceInstance *>( item ) )->GetSpaceItem();

              if ( spItem->Family() == st_Curve3D ) {
                const MbCurve3D * crv = static_cast<const MbCurve3D *>( spItem );
                if ( crv )
                  DrawBoundaryPt( *crv, MbMatrix3D::identity, true );
              }
            }
            else if ( item->IsA() == st_WireFrame ) {
              const MbWireFrame * wf = static_cast<const MbWireFrame *>( item );

              if ( wf )
                DrawBoundaryPt( *wf );
            }
          }
        }
      }
    }
    break;
    case pt_ItemProp: // Свойство объект геометрической модели.
    {
      auto pType = prop.IsA();

      if ( pType == pt_EdgeProp ) {
        const MbItem * find = pWnd->GetGeomModel().FindItem( static_cast<MbItem *>(obj), path, from_ );

        if ( find != nullptr && find->IsA() == st_WireFrame ) {
          pWnd->PutObject( find, false, true );
          size_t fNum = _tcstoui64( s, nullptr, 10 ) - 1;

          const  MbEdge * edge = ( static_cast<MbWireFrame *>(obj) )->GetEdge( fNum );
          if ( fNum < SYS_MAX_T && edge ) {
            pWnd->PutObjectPart( find, edge, true );
            DrawBoundaryPt( edge->GetCurve(), MbMatrix3D::identity, edge->IsSameSense() );
          }
        }
      }
      else if ( pType == pt_SolidProp ) {
        const MbItem * find = pWnd->GetGeomModel().FindItem( static_cast<MbItem *>(obj), path, from_ );

        if ( find != nullptr ) {
          const MbItem * item = nullptr;
          prop._GetPropertyValue( &item, 0 );

          if ( item != nullptr && find->FindItem( item, path, from_ ) != nullptr ) {
            TestVariables::viewManager->UnselectModel();
            TestVariables::drawImp->DrawItem( item, TRGB_LIGHTGREEN, from_, 2 );
          }
        }
      }
      else if ( pType == pt_GridProp ) {
        const MbItem * find = pWnd->GetGeomModel().FindItem( static_cast<MbItem *>(obj), path, from_ );

        if ( find != nullptr && find->IsA() == st_Mesh ) {
          TestVariables::viewManager->UnselectModel();
          size_t fNum = _tcstoui64( s, nullptr, 10 ) - 1;

          if ( fNum < SYS_MAX_T && (static_cast<MbMesh *>(obj))->GetGrid( fNum ) )
            TestVariables::drawImp->PutGrid( *(static_cast<MbMesh *>(obj))->GetGrid(fNum), TRGB_LIGHTGREEN );
        }
      }
      else if ( pType == pt_AssemblyProp ) {

        const MbAssembly * assembly;
        prop._GetPropertyValue( &assembly, 0 );

        const MbItem * parentObj = static_cast<MbItem *>( obj );

        if ( parentObj->IsA() == st_Instance ) {
          parentObj->FindItem( assembly, path, from_ );

          RPArray<const MbItem> items;
          assembly->GetItems( items );

          TestVariables::viewManager->UnselectModel();

          for ( const auto & item : items ) {
            TestVariables::drawImp->DrawItem( item, TRGB_LIGHTGREEN, from_, 2 );
          }
        }
      }
      else if ( pType == pt_PolymeshRegionProp )
      {
        const auto * pPoly = FindPolymesh( this );
        if ( pPoly != nullptr ) {
          const MbPolymeshRegionProperty * propReg = static_cast<const MbPolymeshRegionProperty *>( &prop );
          if ( propReg != nullptr && parentProp != nullptr ) {
            auto * pDraw = pWnd->FindDrawItem( pPoly, from_ );
            if ( pDraw != nullptr )
            {
              const MbMesh * pMeshDraw = static_cast<const MbMesh *>( pDraw );
              const MapPolymeshToMbMesh * pMap = pMeshDraw->GetPolymeshMap();
              if ( pMap != nullptr )
              {
                MbMesh * pMesh = new MbMesh;
                for ( SubregionRegionIterator it( *propReg->GetTopo().GetTopo(), propReg->GetNumber() ); !it.Done(); ++it )
                {
                  const size_t iGrid = pMap->GetGridIdBySubRegion( it.Subregion() );
                  const auto * pGrid = pMeshDraw->GetGrid( iGrid );
                  if ( pGrid != nullptr )
                  {
                    auto newGrid = pGrid->Clone();
                    pMesh->AddGrid( static_cast<MbGrid &>( *newGrid ) );
                  }
                }

                DrawHighlightedMesh( from_, pWnd, pMesh );
              }
            }
          }
        }
      }
    }
    break;
    case pt_Curve3DProp:    // Свойство кривой.
    {
      if ( prop.IsA() == pt_Curve3DProp ) {
        TestVariables::viewManager->UnselectModel();
        size_t fNum = _tcstoui64( s, nullptr, 10 ) - 1;
        const MbCurve3D * crv = static_cast<MbCurve3D *>( obj );

        if ( crv->IsA() == st_Contour3D ) {
          const MbCurve3D * contour = ( static_cast<const MbContour3D *>(crv) )->GetSegment( fNum );
         
          if ( fNum < SYS_MAX_T && contour ) {
            TestVariables::drawImp->DrawItem( contour, TRGB_LIGHTGREEN );
            DrawBoundaryPt( *contour, MbMatrix3D::identity, true );
          }
        }
      }
    }
    break;
    case pt_LoopProp:      // Свойство цикла.
    {
      if ( prop.IsA() == pt_OrientedEdgeProp ) {

        FindMatrix( this, from_ );

        TestVariables::viewManager->UnselectModel();
        size_t fNum = _tcstoui64( s, nullptr, 10 ) - 1;

        const  MbOrientedEdge * oEdge = ( static_cast<MbLoop *>(obj) )->GetOrientedEdge( fNum );
        if ( fNum < SYS_MAX_T && oEdge ) {
          TestVariables::drawImp->DrawItem( oEdge, TRGB_LIGHTGREEN, from_, 2 );
          DrawBoundaryPt( oEdge->GetCurve(), from_, oEdge->IsSameSense() );
        }
      }
    }
    break;
    case pt_FaceProp:       // Свойство грани.
      if ( prop.IsA() == pt_LoopProp ) {

        FindMatrix( this, from_ );

        TestVariables::viewManager->UnselectModel();
        size_t fNum = _tcstoui64( s, nullptr, 10 ) - 1;

        TestVariables::drawImp->DrawItem( (static_cast<MbFace *>(obj))->GetLoop( fNum ), TRGB_LIGHTGREEN, from_, 2 );
      }
      break;
    case pt_FaceShellProp:  // Свойство облочки.
    {
      if ( prop.IsA() == pt_FaceProp ) {
        size_t fNum = _tcstoui64( s, nullptr, 10 ) - 1;

        FindMatrix( this, from_ );
        
        TestVariables::viewManager->UnselectModel();
        TestVariables::drawImp->DrawItem( (static_cast<MbFaceShell *>(obj))->GetFace(fNum), TRGB_LIGHTGREEN, from_, 2 );
      }     
    }
    break;
    case pt_AssemblyProp:
    {
      FindMatrix( this, from_ );

      auto pType = prop.IsA();
      if ( pType == pt_ItemProp || pType == pt_SolidProp
           || pType == pt_InstanceProp || pType == pt_AssemblyProp || pType == pt_SpaceInstanceProp ) {
       
        const MbItem * item = nullptr;
        prop._GetPropertyValue( &item, 0 );

        if ( item != nullptr ) {
          TestVariables::viewManager->UnselectModel();
          TestVariables::drawImp->DrawItem( item, TRGB_LIGHTGREEN, from_, 2 );
        }
      }

    }
    break;
    case pt_InstanceProp:
    {     
      FindMatrix( this, from_ );
     
      auto pType = prop.IsA();

      if ( pType == pt_ItemProp || pType == pt_SolidProp
           || pType == pt_InstanceProp || pType == pt_AssemblyProp ) {

        const MbItem * item = nullptr;
        prop._GetPropertyValue( &item, 0 );

        if ( item != nullptr ) {
          TestVariables::viewManager->UnselectModel();
          TestVariables::drawImp->DrawItem( item, TRGB_LIGHTGREEN, from_, 2 );
        }
      }
    }
    break;
    case pt_PolymeshLoopProp:      // Свойство ребра сегментации.
    {
      if ( prop.IsA() == pt_PolymeshEdgeProp ) {
        FindMatrix( this, from_ );

        const MbPolymeshEdgeProperty * propEdge = static_cast<const MbPolymeshEdgeProperty *>( &prop );
        if ( propEdge != nullptr ) {
          c3d::SpacePointsVector points;
          GetPolymeshEdgePolyline( propEdge->GetTopo(), propEdge->GetNumber(), points );
          if ( !points.empty() ) {
            MbMesh * pMesh = new MbMesh;
            
            MbMesh & wrkMesh = *pMesh;
            auto * p = wrkMesh.AddPolygon();
            for ( const auto & pt : points )
              p->AddPoint( pt );

            SPtr<MbWidth> aWidth( new MbWidth( 4 ) );
            wrkMesh.AddAttribute( aWidth );
            DrawHighlightedMesh( from_, pWnd, pMesh );
          }
        }
      }
    }
    break;
    case pt_PolymeshRegionProp:      // Свойство ребра сегментации.
    {
      if ( prop.IsA() == pt_PolymeshSubProp ) {

        const auto * pPoly = FindPolymesh( this );
        if ( pPoly != nullptr ) {
          const MbPolymeshSubRegionProperty * propSub = static_cast<const MbPolymeshSubRegionProperty *>( &prop );
          if ( propSub != nullptr ) {
            auto * pDraw = pWnd->FindDrawItem( pPoly, from_ );
            if ( pDraw != nullptr ) {
              const MbMesh * pMeshDraw = static_cast<const MbMesh *>( pDraw );
              const MapPolymeshToMbMesh * pMap = pMeshDraw->GetPolymeshMap();
              if ( pMap != nullptr ) {
                const size_t iGrid = pMap->GetGridIdBySubRegion( propSub->GetNumber() );
                const auto * pGrid = pMeshDraw->GetGrid( iGrid );
                if ( pGrid != nullptr ) {
                  MbMesh * pMesh = new MbMesh;
                  auto newGrid = pGrid->Clone();
                  pMesh->AddGrid( static_cast<MbGrid &>( *newGrid ) );
                  DrawHighlightedMesh( from_, pWnd, pMesh );
                }
              }
            }
          }
        }
      }
    }
    break;
    case pt_PolymeshSubProp:      // Свойство ребра сегментации.
    {
      if ( prop.IsA() == pt_PolymeshLoopProp ) {
        FindMatrix( this, from_ );
        const MbPolymeshLoopProperty * propLoop = static_cast<const MbPolymeshLoopProperty *>( &prop );
        if ( propLoop != nullptr ) {
          c3d::SpacePointsVector points;
          GetPolymeshLoopPolyline( propLoop->GetTopo(), propLoop->GetNumber(), points );
          if ( !points.empty() ) {
            MbMesh * pMesh = new MbMesh;

            MbMesh & wrkMesh = *pMesh;
            auto * p = wrkMesh.AddPolygon();
            for ( const auto & pt : points )
              p->AddPoint( pt );

            SPtr<MbWidth> aWidth( new MbWidth( 4 ) );
            wrkMesh.AddAttribute( aWidth );

            DrawHighlightedMesh( from_, pWnd, pMesh );
          }
        }
      }
    }
    break;
    case pt_PolymeshEdgeProp:      // Свойство ребра сегментации.
    {
      if ( prop.IsA() == pt_PolymeshVertexProp ) {
        FindMatrix( this, from_ );
        const MbPolymeshVertexProperty * propVx = static_cast<const MbPolymeshVertexProperty *>( &prop );
        if ( propVx != nullptr ) {
          auto * pAttrVx = propVx->GetTopo().GetTopo()->GetMeshHalfedgeAttribute<MbHalfedgeAttrPoint3D>();
          if ( pAttrVx != nullptr ) {
            MbMesh * pMesh = new MbMesh;
            auto * pApex = pMesh->AddApex();
            pApex->Init( pAttrVx->GetPoint( propVx->GetTopo().GetTopo()->GetTopoVertex( propVx->GetNumber() ) ) );
            SPtr<MbWidth> aWidth( new MbWidth( 5 ) );
            pMesh->AddAttribute( aWidth );
            DrawHighlightedMesh( from_, pWnd, pMesh );
          }
        }
      }
    }
    break;

    default:
      break;
    }
  }
}
