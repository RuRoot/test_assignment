////////////////////////////////////////////////////////////////////////////////
//
// Менеджер моделей и их окон отображения, управляющий работой тестового приложения.
// Models (and Model Display Windows) Manager, controlling the operation of the test application.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEST_MANAGER_H
#define __TEST_MANAGER_H

#include <templ_ifc_array.h>
#include <gcm_routines.h>
#include <constraint_item.h>
#include <comanager.h>
#include <test_window.h>
#include <test_temporal_plane.h>
#include <test_temporal.h>
#include <test_property.h>
#include <test_comanager.h>
#include <test_tree.h>
#include <test_tree_filter.h>

//------------------------------------------------------------------------------
// Менеджер модели и окон отображения. 
// Manager of Model and Display Windows.
// ---
class Manager
{
private :
  PArray<WorkWindow>        workWindows;         // Множество окон отображения. Models and Display Windows.
  IFC_Array<PropertyWindow> propertyWindows;     // Множество окон свойств. Property Windows (windows called by right mouse botton on object). 
  IFC_Array<IModelTreeWindow>   treeWindows;     // Множество окон. Model tree nodes. 
  IFC_Array<ObjectFilterWindow> filterWindows;   // Множество окон фильтров. Filter Windows.
  MbConstraintSystem2D      constraintModel2D;   // Система ограничений для плоских объектов. Constraint system.
  ConstraintManager2D       constraintManager2D; // Менеджер ограничений. 2D constraint manager.
  ConstraintManager         constraintManager3D; // Менеджер сопряжений.  3D constraint manager.
  Templane                  cplane;              // Конструктивная плоскость. Active plane (scren).
  Temporal                  temporal;            // Горячие точки объекта. Hot points of object.
  ptrdiff_t                 activeIndex;         // Индекс активного окна отображения. Index of active model (window).
  MbVector3D                lightDirection;      // Направление падающего света на экран для новых окон. Direction of light.
  bool                      enableDraw;          // Разрешение перерисовать активное окно. Redraw active model (window).

private :
  Manager( const Manager & ); // Не реализовано. Not implemented.
public :
  Manager();
 ~Manager();

public :

  // Действия над формированием моделей. 
  // Action on Model.

  void        ReplaceItem( MbItem & item, MbItem & newItem, bool saveName );
  void        RefreshDrawItem( MbItem * geomItem ); // Обновить сетку для геометрического объекта. Update the grid for a geometric object.
  bool        ReplaceModel( const MbModel & );
  void        AddItem( MbItem & newObj, bool showItem = true ); // Добавление геометрического объекта. Adding a geometric object.
  void        AddItem( MbItem & newObj, const Style & style, bool showItem = true ); // Добавление геометрического объекта. Adding a geometric object.
  MbItem *    AddObject( MbSpaceItem & item, bool showItem = true );
  MbItem *    AddObject( const Style & style, MbSpaceItem * item, bool showItem = true );
  MbItem *    AddObject( const Style & style, MbPlaneItem * item, const MbPlacement3D * p = nullptr, bool showItem = true );
  MbItem *    AddObject( const Style & style, const MbCartPoint & p0, const MbPlacement3D * p = nullptr, bool showItem = true );

  void        DecomposeAssemblies(); // Разрушить сборки с подсборками на составляющие. Break assemblies with subassemblies into components.
  void        RemoveColorAndVisualAttributes(); // Удалить все визуальные свойства объектов. Remove all visual properties of objects.

  void        DeleteModel(); // Удалить все объекты модели. Delete all model objects.
  void        DeletePlaneItems(); // Удалить все плоские объекты модели. Delete all flat objects in the model.
  void        DeleteSpaceItems(); // Удалить все пространственные объекты модели. Remove all space objects in the model.
  void        DeleteModelAttributes(); // Удалить атрибуты модели.

  void        GetObjects( MbeSpaceType type, std::vector<SPtr<MbItem>> & items, std::vector<MbMatrix3D> & matrs ); // Дать все объекты указанного типа. // Give all objects of the specified type.
  void        GetObjects( MbeSpaceType type, RPArray<MbItem> & items, SArray<MbMatrix3D> & matrs ); // Дать все объекты указанного типа. // Give all objects of the specified type.
  void        GetRenderingObjects  ( RPArray<MbItem> & objs ); // Дать все поверхности, тела и оболочки. Get all surfaces, bodies, and shells.
  void        GetLineDrawingObjects( RPArray<MbItem> & objs ); // Дать все кривые и точки. Get all curves and points.
  void        GetAllObjects        ( RPArray<MbItem> & objs ); // Дать все объекты. Get all objects.
  void        CreateCurveArray( MbItem * obj, RPArray<MbCurve> & curveList, bool breakCurve ); // Выдать двумерные кривые из объекта модели. Output two-dimensional curves from the model object.

  const MbItem * FindObject( const MbSpaceItem * item ) const; // Найти объект модели по трёхмерному объекту. Find a model object based on a three-dimensional object.
  const MbItem * FindObject( const MbPlaneItem * item ) const; // Найти объект модели по двумерному объекту. Find a model object based on a two-dimensional object.

  void        GetDrawModelGabarit( MbCube & r ); // Дать габарит полигональной модели. Give a bounding box of the polygon model.
  bool        CutModel( const MbPlacement3D & place, double distance, size_t & operationCount ); // Разрезать модель. Split the model.
  size_t      GetTrianglesCount() const; // Число треугольников в сеточной модели. The number of triangles in the grid model.

  // Действия над одним объектом. 
  // Action on Object.

  void        EraseObject ( const MbItem * obj ); // Стереть объект во всех окнах. Erase the item in all windows.
  void        RedrawObject( const MbItem * obj ); // Перерисовать объект. Redraw the item.
  void        RefreshObjectModel( const MbItem * obj ); // Перерисовать объекты модели. Redraw the objects in the model.

  void        SelectObject( MbItem * obj, bool refreshDrawing = true );             // Выделить объект. Selected object.
  void        SelectObject( const MbPath & objPath, bool refreshDrawing = true );   // Выделить объект. Selected object.
  void        UnselectObject( MbItem * obj, bool refreshDrawing = true );           // Снять выделение с объекта. Deselect an item.
  void        UnselectObject( const MbPath & objPath, bool refreshDrawing = true ); // Снять выделение с объекта. Deselect an item.
  void        ToggleObject( const MbItem * obj, bool refreshDrawing = true );       // Инвертировать выделение. Invert the selection.
  void        ToggleObject( const MbPath & objPath, bool refreshDrawing = true );   // Инвертировать выделение. Invert the selection.

  void        SetObjectWidth( MbItem * obj, int w ); // Изменить толщину линий объекта. Change the thickness of the object's lines.

  void        TransformObject( MbItem * obj, MbMatrix3D & matr ); // Преобразование объекта согласно матрице. The transformation of the object according to the matrix.
  void        MoveObject( MbItem * obj, const MbVector3D & to );           // Сдвиг объекта. The shift of the object.
  void        MoveObject( const MbPath & objPath, const MbVector3D & to ); // Сдвиг объекта. The shift of the object.
  void        RotateObject( MbItem * obj, MbAxis3D & axis, double angle ); // Повернуть объект вокруг оси. Rotate the object around the axis.

  void        DeleteObject( MbItem * obj );           // Удалить объекта. Delete an object.
  void        DeleteObject( const MbPath & objPath ); // Удалить объекта. Delete an object by path.

  void        DuplicateObject( const MbItem * obj ); // Дублировать объект. Duplicate an item.
  void        TransformDuplicateObject( const MbItem * obj, const MbMatrix3D & matr ); // Дублировать объект. Duplicate and transform an item.

  // Действия над объектами указанного типа. 
  // Action on Object by type.

  void        SelectByType( MbeSpaceType  typeObj ); // Выделить объекты по типу. Select objects by type.
  void        SelectByType( MbePlaneType  typeObj ); // Выделить объекты по типу. Select objects by type.

  void        SetVisibleByType( MbeSpaceType typeObj, bool visible ); // Установить видимость объектов по типу. To set the visibility of objects by type.
  void        SetVisibleByType( MbePlaneType typeObj, bool visible ); // Установить видимость объектов по типу. To set the visibility of objects by type.

  void        ModelSelectByObject( const MbItem * obj ); // Выделить объекты модели.
  void        MoveObjectInPlacement( MbItem * obj, const MbVector3D & to ); // Переместить объект в конструктивной плоскости.
  void        RotateObjectInPlacement( MbItem * obj, const MbCartPoint3D & point, double angle ); // Повернуть объект в конструктивной плоскости.
  void        TransformObjectInPlacement( MbItem * obj, const MbMatrix & matr ); // Трансформировать объекты в конструктивной плоскости. Objects tranforming.
  void        SetObjectPlacement ( MbItem * obj, const MbPlacement3D & pl );

  // Функции отображения моделей.
  // Visualization Methods.

  void        RefreshModel(); // Обновить объекты модели
  void        ShowModel();    // Нарисовать модель во всех окнах.
  void        EraseModel();   // Стереть модель во всех окнах.

  void        SelectModel();   // Выделить модель во всех окнах.
  void        UnselectModel(); // Выделить модель во всех окнах.
  void        ToggleModel();   // Выделить модель во всех окнах.

  // Действия над селектированными объектами моделей.
  // Actions on Selected Objects.

  void        TransformSelected( const MbMatrix3D & matr ); // Преобразование выделенных объектов согласно матрице.
  void        MoveSelected( const MbVector3D & to );        // Сдвиг выделенных объектов.
  void        RotateSelected( const MbAxis3D & axis, double angle ); // Повернуть вокруг оси выделенных объектов.
  void        SetSelectedObjectWidth( int w ); // Изменить толщину линий выделенных объектов.
  size_t      GetSelectedObjects( RPArray<MbItem> & objs ); // Нахождение выделенных объектов.
  void        SetSelectedObjectColor( COLORREF newColor ); // Изменить цвет выделенных объектов.
  COLORREF    GetSelectedObjectColor() const; // Получить цвет выделенных объектов.

  void        SetSelectColor( COLORREF newColor ); // Изменить цвет выделения объектов.
  COLORREF    GetSelectColor() const; // Получить цвет выделения объектов активного окна.

  void        SetBackgrColor( COLORREF newColor ); // Изменить цвет фона окон.
  COLORREF    GetBackgrColor() const; // Получить цвет фона активного окна.

  void        SetEnableDrawToActiveWindow( bool b ) { enableDraw = b; }
  bool        IsEnableDrawToActiveWindow() const { return enableDraw; }

  const MbVector3D & GetLightDirection() const { return lightDirection; }
  void        SetLightDirection( const MbVector3D & v ) { lightDirection = v; }
  void        SetLightDirection( const MbCartPoint3D cPoint ) {
    lightDirection = cPoint;
    if ( !lightDirection.Normalize() )
      lightDirection.Init( 0.1, 0.1, 1.0 );
  }

  // Действия над рабочими окнами.
  // Actions on Windows.

  void        SetViewPoint( double d ); // Установить точку наблюдения.
  void        DrawContentWindow( ptrdiff_t index );   // Нарисовать модель в окне.
  void        EraseWindow( ptrdiff_t index );  // Очистить окно.
  void        InvalidateWindow( ptrdiff_t index, bool bErase );  // Перерисовать содержимое окна.
  void        SetFocus( ptrdiff_t index, bool mdiActivate ); // Установить фокус на окно.
  void        EraseBackground( ptrdiff_t index ); // Очистить окно.
  void        RefreshWindow( ptrdiff_t index ); // Очистить карту окна.
  bool        IsWindowRenderType() const; // Наличие окна с растровым изображением.

  ptrdiff_t   FindWindowIndex( const void * wndId, bool setActive = true ); // Найти в множестве окно по HWND и (при setActive == true) установить его текущим.
  ptrdiff_t   AddWindow( WorkWindow * wnd ); // Добавить окно.
  WorkWindow* GetWindow( ptrdiff_t index ); // Вернуть указатель на окно с заданным HWND.
  void        DeleteWindow( ptrdiff_t index ); // Удалить окно окончательно.
  WorkWindow* GetActiveWindow() const; // Вернуть указатель на активное окно.
  WorkWindow* SetActiveWindow( ptrdiff_t ind ); // Установить индекс активного окна.
  ptrdiff_t   GetWindowsCount() const { return (ptrdiff_t)workWindows.Count(); } // Выдать количество окон.
  ptrdiff_t   GetActiveWindowIndex() const { return activeIndex; }

  double      GetVisualSag() const; // Дать прогиб для расчета отрисовочной сетки.

  void        SetOriginIntoCentre( bool b ); // Установка нуля в центр вида.
  double      GetWindowScale(); // Дать масштаб активного окна.
  void        SetWindowScale( const MbCube & gab ); // Установка масштаба для габарита.
  void        SetSize( ptrdiff_t ind, int cx, int cy ); // Установить размер видимой области.
  void        ReDrawGL();
  void        DeleteGL();
  OGLViewMode GetWindowOGLViewType();      // Дать способ отображения OGL активного окна.
  void        SetWindowOGLViewType( OGLViewMode viewMode ); // Установить способ отображения OGL активного окна.

  // Действия над окнами дерева модели
  // Actions on model tree windows.

  TreeWindow*           CreateTree     ( reader_ex* reader );                                 // Создать окно дерева модели.
  EmbodimentTreeWindow* CreateEmbodimentTree ( reader_ex* reader );                           //Создать окно дерева исполнений.
  NodeWindow*         CreateTreeNode ( const c3d::IModelTreeNode* node, ModelTreeWindow* parent );                // Создать окно узла модели.
  NodeEmbodimentWindow* CreateEmbodimentNode ( const c3d::IEmbodimentNode* node, IEmbodimentTreeWindow* parent ); // Создать окно узла дерева исполнений.
  IModelTreeWindow*   FindTree       ( const void * wndId ) const;                            // Выдать окно дерева модели.
  bool                IsMyTree       ( const IModelTreeWindow * wp ) const;                   // Проверка на правильность.
  void                RemoveTree     ( IModelTreeWindow * wp );                               // Удалить окно дерева модели.
  ObjectFilterWindow* CreateTypeFilter    ( reader_ex* reader );                              // Создать окно фильтра по типу.
  ObjectFilterWindow* CreateGabaritFilter ( reader_ex* reader );                              // Создать окно фильтра по габариту.
  ObjectAttrWindow*   CreateAttributeFilter ( reader_ex* reader );                            // Создать окно фильтра по атрибуту.
  ObjectFilterWindow* FindFilter     ( const void * wndId ) const;                            // Выдать окно фильтра.
  void                RemoveFilter   ( ObjectFilterWindow * wp );                             // Удалить окно фильтра.

  // Действия над окнами свойств объектов
  // Actions on Property Windows.

  template<class _Item>
  PropertyWindow * CreateProperty( _Item * ); // Создать и добавить новое свойство.  
  PropertyWindow * FindProperty( const void * wndId ) const ; // Выдать окно свойств.
  bool        IsMyProperty( const PropertyWindow * wp ) const; // Проверка на правильность.
  void        SetProperty( PropertyWindow * wp ); // Записать свойство.
  void        AddProperty( PropertyWindow * newProp ); // Добавить новое окно свойств.
  void        RemoveProperty( PropertyWindow * wp, bool redraw ); // Удалить окно свойств.

  // Действия над конструктивной плоскостью.
  // Actions on Constructive Planes.

  void        InitCplane( MbItem * obj, MbTopItem * item, const MbMatrix3D & m, double a = 0 );
  const MbItem *    GetCplaneObject() const ; // Дать временные объекты.
  const MbTopItem * GetCplaneItem() const;   // Дать временные объекты.
  void        DeleteCplane();
  void        RedrawCplane(); // Перерисовать временный объект.

  const MbPlacement3D & GetCplane(); // Дать плейсемент.
  MbMatrix3D  GetCplaneMatrixFrom(); // Дать матрицу.
  bool        GetCplanePoint( const MbCartPoint3D & cursorPoint, MbCartPoint & pnt ); // Дать точку конструктивной плоскости.
  void        GetCplaneMoveVector( const MbItem * obj, const MbVector3D & vect, MbVector & t0 ); // Дать вектор сдвига в конструктивной плоскости.
  void        GetCplaneRotateAxis( const MbAxis3D & axis, double angle, MbCartPoint & pnt, MbDirection & ang ); // Дать неподвижную точку и угол в конструктивной плоскости.
  void        GetCplaneTransformMatrix( const MbMatrix3D & matr, MbMatrix & m ); // Дать неподвижную точку и угол в конструктивной плоскости.

  // Действия над точками управления объектами и связями.
  // Actions on Hot Points.

  void        InitTemporal( MbItem * obj, const MbMatrix3D & matr );

  MbItem *    GetObjectTarget(); // Дать временные объекты.

  void        DeleteTemporal(); // Удалить.
  void        RedrawTemporal(); // Перерисовать временный объект.
  void        MoveTemporal( ptrdiff_t index, const MbVector3D & to, bool target ); // Сдвиг.
  void        MoveTemporal( const MbVector3D & to, bool target ); // Сдвиг.
  ptrdiff_t   GetTemporalPoint( ptrdiff_t tindex, ptrdiff_t n, MbCartPoint3D & p3, MbCartPoint & p2 ); // Дать точку.
  void        SetTemporalPoint( ptrdiff_t tindex, ptrdiff_t n, ptrdiff_t np, MbCartPoint3D & p3, MbCartPoint & p2 ); // Установить точку.
  bool        FindNearestTemporal( const MbFloatAxis3D & from, double maxDistance, ptrdiff_t & index, ptrdiff_t & n ) const;

  // Действия над геометрическими ограничениями.
  // Actions on Geometric Constraints.

  bool            AddConstraint( const MbConstraint & );
  // Задать сопряжение для пары объектов
  bool            AddConstraint( MtMateType, const MtGeomArgument &, const MtGeomArgument &, MtParVariant );
  // Recalculate the constraint systems of assemblies (with no redrawing)
  MtResultCode3D  EvaluateConstraints();

private:
  Manager & operator = ( const Manager & );
}; // Manager


//------------------------------------------------------------------------------
// Установить индекс активного окна.
// ---
inline WorkWindow * Manager::SetActiveWindow( ptrdiff_t ind )
{
  if ( ind >= 0 && ind < (ptrdiff_t)workWindows.Count() ) {
    activeIndex = ind;
    return workWindows[ind];
  }
  return nullptr;
}


//------------------------------------------------------------------------------
// Удалить временную плоскость.
// ---
inline void Manager::DeleteCplane()
{
  if ( cplane.Exist() ) {
    WorkWindow * wind = GetActiveWindow();

    cplane.Erase( wind );
    cplane.Delete();

    const MbItem * obj = cplane.GetObject();
    if ( obj != nullptr && wind != nullptr )
      wind->ShowObject( obj );
  }
}


//------------------------------------------------------------------------------
// Удалить временный объект для редактирования.
// ---
inline void Manager::DeleteTemporal()
{
  if ( temporal.Exist() ) {
    MbItem * obj = temporal.SetObject();
    WorkWindow * wind = GetActiveWindow();
    temporal.Erase( wind );
    temporal.Delete();
    if ( obj != nullptr && wind != nullptr )
      wind->ShowObject( obj );
  }
}

//------------------------------------------------------------------------------
// Создать и добавить новое свойство
// ---
template<class _Item>
PropertyWindow * Manager::CreateProperty( _Item * obj )
{
  DeleteTemporal();
  PropertyWindow * newProp = new PropertyWindow( obj, true );
  propertyWindows.Add( newProp );
  return newProp;
}

//------------------------------------------------------------------------------
// Создать окно фильтра по типу
// ---
inline ObjectFilterWindow* Manager::CreateTypeFilter ( reader_ex* reader )
{
  ObjectFilterWindow* tw = new ObjectTypeWindow ( reader );
  filterWindows.Add ( tw );
  return tw;
}

//------------------------------------------------------------------------------
//  Создать окно фильтра по габариту
// ---
inline ObjectFilterWindow* Manager::CreateGabaritFilter ( reader_ex* reader )
{
  ObjectFilterWindow* tw = new ObjectGabaritWindow ( reader );
  filterWindows.Add ( tw );
  return tw;
}

//------------------------------------------------------------------------------
//  Создать окно фильтра по атрибуту
// ---
inline ObjectAttrWindow* Manager::CreateAttributeFilter ( reader_ex* reader )
{
  ObjectAttrWindow* tw = new ObjectAttrWindow ( reader );
  filterWindows.Add ( tw );
  return tw;
}

//------------------------------------------------------------------------------
//  Создать окно дерева модели
// ---
inline TreeWindow* Manager::CreateTree ( reader_ex* reader )
{
  TreeWindow* tw = new TreeWindow ( reader );
  treeWindows.Add ( tw );
  return tw;
}

//------------------------------------------------------------------------------
//  Создать окно дерева исполнений
// ---
inline EmbodimentTreeWindow* Manager::CreateEmbodimentTree( reader_ex* reader )
{
  EmbodimentTreeWindow* tw = new EmbodimentTreeWindow( reader );
  treeWindows.Add ( tw );
  return tw;
}
//------------------------------------------------------------------------------
//  Создать окно узла модели
// ---
inline NodeWindow* Manager::CreateTreeNode( const c3d::IModelTreeNode* node, ModelTreeWindow* parent)
{
  NodeWindow* tw = new NodeWindow(node, parent);
  treeWindows.Add(tw);
  return tw;
}

//------------------------------------------------------------------------------
// Создать окно узла дерева исполнений
// ---
inline NodeEmbodimentWindow* Manager::CreateEmbodimentNode( const c3d::IEmbodimentNode* node, IEmbodimentTreeWindow* parent )
{
  NodeEmbodimentWindow* tw = new NodeEmbodimentWindow(node, parent);
  treeWindows.Add(tw);
  return tw;
}

//------------------------------------------------------------------------------
// Копия кривой в системе координат окна.
// item - Кривая на плоскости.
// ---
const MbCurve * GetCurveCopyOnWindPlace( const Manager & viewManager, const MbItem & item );

//------------------------------------------------------------------------------
// Чтение пользовательских настроек из config файла.
// ---
bool ApplyC3dTestConfig();

#endif // __TEST_MANAGER_H
