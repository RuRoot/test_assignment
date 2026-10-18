////////////////////////////////////////////////////////////////////////////////
//
// Модель и её окно отображения. 
// Model and Display Window for Model.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEST_WINDOW_H
#define __TEST_WINDOW_H


#include <test_style.h>
#include <test_variables.h>
#include <math_define.h>
#include <mb_cube.h>
#include <mb_matrix3d.h>
#include <mb_placement3d.h>
#include <mesh.h>
#include <action_b_shaper.h>
#include <topology.h>
#include <model_entity.h>


class    MbFloatPoint;
class    MbFloatPoint3D;
class    MbTopItem;
class    MbWireFrame;
class    MbPointFrame;
class    MbMesh;
class    Temporal;
class    Templane;
class    IGrDraw;
class    IProgressIndicator;


//------------------------------------------------------------------------------
// Типы ориентации в окне. 
// Window Orientation Types.
// ---
enum wProjections {
  v_Arbitrary, // Произвольная изометрия
  v_Front,     // Спереди
  v_Rear,      // Сзади
  v_Up,        // Сверху 
  v_Down,      // Снизу
  v_Left,      // Слева
  v_Right,     // Справа
  v_Isometry,  // Изометрия XYZ
};


//------------------------------------------------------------------------------
// Типы операций Undo. 
// Undo Operation Types. 
// ---
enum ActionType {
  at_DeleteType  = -1, // Удаленный элемент из модели
  at_AnyType     =  0, // Элемент модели
  at_AddType     =  1  // Добавленный в  модель элемент
};


//------------------------------------------------------------------------------
// Модель и её окно отображения. 
// Model and Display Window for Model.
// ---
class WorkWindow {
  typedef std::vector< std::pair< SPtr<MbItem>, ActionType > > UndoItems;
  typedef std::pair< SPtr<const MbItem>, SPtr<const MbRefItem> > PairObjectParts;

private :
  MbModel                        geomModel;      // Геометрическая модель. Geometric model.
  MbModel                        drawModel;      // Отрисовочная модель (соответствует геометрической по структуре). Draw model (corresponds Geometric model).
  mutable MbCube                 cube;           // Габарит для вращения изображения. Bounding box.
  PArray<MbItem>                 tmpObjects;     // Временные объекты. 
  bool                           hideline;       // Режим отображения видимых и не видимых линий в активном окне. Visible or Hideline mode.
  mutable MbPlacement3D          cutPlace;       // Режущая плоскость. Plane for model cutting.
  mutable MbModel *              cutModel;       // Урезанная плоскостью отрисовочная модель. Cut model.
  UndoItems                      undoItems;      // Множество удаленных и построенных элементов для undo. Undo container.
  COLORREF                       select;         // Цвет селектирования. Color for select.
  COLORREF                       backColor;      // Цвет фона для новых окон. Background color.
  c3d::string_t                  modelName;      // Имя файла модели. Model name.
  bool                           firstLoading;   // Загружается ли файл впервые
  MbPlacement3D                  place;          // Местная система координат экрана. Local coordinate system of screen (XY plane is the screen place).
  MbMatrix3D                     into;           // Матрица преобразования в экранную систему. Matrix for translation from global space into screen place.
  MbMatrix3D                     from;           // Матрица преобразования в мировую систему.  Matrix for translation from screen place into global space.
  double                         scale;          // Масштаб отображения вида. Scale of view.
  MbCartPoint3D                  vista;          // Расстояние до точки взгляда в экранных единицах. Point of perspective view.
  wProjections                   proj;           // Тип ориентации модели в окне. Window orientation type.
  IGrDraw *                      drawTool;       // Контекст рисования в окне. OpenGL or GDI draw context (tool) of model window.
  WorkRenderType                 renderType;     // Способ отображения. Rendering type.
  WorkViewSort                   viewSort;       // Объекты для отображения. The objects for drawing.
  bool                           make_grid;      // Рассчитывать триангуляцию для визуализационной модели. Mesh for OpenGL or GDI?
  SPtr<MbMeshProcessor>          meshProcessor;  // Конвертер полигональной сетки в Brep. Mesh to Brep converter.
  std::vector< PairObjectParts > vecSelElements; // Массив выбранных пользователем элементов. Первый элемент пары не может быть нулевым указателем. Array of user selected elements. The 1st element of the pair cannot be nullptr.
public :
  bool                           mouseRotation;  // Флаг активности вращения модели мышью.
  bool                           enableDrawRect; // Признак рисования рамки селектирования.
  POINT                          lastBase;       // Последняя (оконная) точка пересчета угла поворота.

public:
  WorkWindow( wProjections initProj, COLORREF sc = TestVariables::sColor, COLORREF bc = TestVariables::bColor );
  ~WorkWindow();

public:

  // Methods of Data Access.

  bool        IsNeedGrid() const { return make_grid; } // Рассчитывать триангуляцию для визуализационной модели
  void        SetNeedGrid( bool g ) { make_grid = g; } // Рассчитывать триангуляцию для визуализационной модели

  // Получить способ отображения.
  WorkRenderType GetRenderType() const { return renderType; }
  // Установить способ отображения.
  void        SetRenderType( WorkRenderType newRendType );
  // Изменение типа отрисовки.
  void        ChangeRenderType( bool convexHull );

  // Получить сорт отображения. Get the sort for drawing.
  WorkViewSort GetViewSort() const { return viewSort; }
  // Установить сорт отображения. Set the sort for drawing.
  void         SetViewSort( WorkViewSort ws ) { viewSort = ws; }
  // Изменение типа отрисовки. Change the sort for drawing.
  void         ChangeViewSort( bool changeSort );

  // Получить тип отображения OGL
  OGLViewMode GetRenderTypeOGL() const;
  // Установить тип отображения OGL
  void        SetRenderTypeOGL( OGLViewMode vm );
  // Изменение типа отрисовки.
  void        ChangeRenderTypeOGL();
  

  const MbModel & GetGeomModel() const { return geomModel; } // Выдать геометрическую модель.
        MbModel & SetGeomModel()       { return geomModel; } // Выдать геометрическую модель.

  // Пуста ли модель?
  bool        IsModelEmpty() const { return ( geomModel.ItemsCount() == 0 ); }
  // Наполнить плоскогранную модель полигональными копиями объектов геометрической модели.
  void        FillDrawModel( IProgressIndicator * prog = nullptr );

  // Найти соответствующий геометрический объект для полигонального объекта.
  const MbItem * FindGeomItem( const MbItem * drawItem, MbMatrix3D & mtrFrom ) const;
  const MbItem * FindGeomItem( const MbPath & path, MbMatrix3D & mtrFrom ) const { return geomModel.GetItemByPath( path, mtrFrom ); }

  // Найти соответствующий полигональный объект для геометрического объекта.
  // Drawing object (poligonal) by the model object.
  const MbItem * FindDrawItem( const MbItem * geomItem, MbMatrix3D & mtrFrom ) const;
  const MbItem * FindDrawItem( const MbPath & path, MbMatrix3D & mtrFrom ) const { return drawModel.GetItemByPath( path, mtrFrom ); }

  const TCHAR * GetModelName() const; // Имя модели.
  void        SetModelName( const TCHAR * ); // Имя модели.
  bool        IsModelNameEmpty() const; // Длина имени модели.
  bool        CheckFirstLoading( const TCHAR * ); // Проверка, совпадает ли файл с предыдущим загруженным
  bool        IsFirstLoading() const { return firstLoading; } // Был ли ранее загружен тот же файл
  void        SetWindowTitle() const; // Заголовок для окна

  bool        IsHideline() const { return hideline; } // Режим отображения видимых и не видимых линий в активном окне.

  bool        IsMyWnd( const void * wndId ) const;
  void        EraseWindow(); // Стереть содержимое окна
  void        ShowModel (); // Нарисовать
  void        EraseModel(); // Стереть
  void        RefreshModel( bool wire = true ); // Обновить объекты модели
  void        TurnOverMapping(); // Переключить режим отображения невидимых линий.
  void        ShowMapping(); // Нарисовать видимые и не видимые линии всех тел в активном окне.
  // Очистить массив выбранных элементов.
  void        ClearArraySelectedElements() { vecSelElements.clear(); };
  // Добавить/Исключить (работает по принципу переключателя) элемент в массив выбранных пользователем объектов.
  bool        ToggleElements( const MbItem * object, const MbRefItem * part );
  // Установить величину стрелки прогиба для визуализации
  double      SetVisualSag();

  // Функции отображения объектов через GDI
  void        ShowAxisGDI( int length = WCS_AXIS_LENGTH, int thick = WCS_AXIS_THICKNESS ); // Отрисовать оси координат
  // Нарисовать объект.
  void        PutMeshGDI( const MbMesh * mesh, const MbMatrix3D & from, const MbRefItem * item,
                          const MbAttributeContainer * hostColor, COLORREF other, bool useOther, bool eraseVertex ) const;
  // Нарисовать объект.
  void        PutItemGDI( const MbItem * obj, const MbMatrix3D & matr, const MbRefItem * item,
                          const MbAttributeContainer * hostColor, COLORREF other, bool useOther, bool eraseVertex = false ) const;
  // Нарисовать объект.
  void        ShowGDI ( MbModel & model, COLORREF select ); // Нарисовать
  void        EraseGDI(); // Стереть
  void        RefreshOGL(); // Удалить временные данные (листы) OpenGL.
  void        ShowOGL( std::vector< SPtr<MbItem> > * addItems = nullptr ); // Отрисовать через OpenGL

  // Функции преобразования точки
  // Methods of Point Translation.

  MbFloatPoint3D  GetFloatPoint( const POINT & mouseCoord ) const;
  MbCartPoint3D   GetCartPoint ( const POINT & mouseCoord ) const;
  MbCartPoint     GetPlanePoint( const POINT & mouseCoord ) const;

  double      GetDistance( int64 mouseDist ) const;
  POINT       InitPoint( MbCartPoint3D & pnt, const MbMatrix3D & matr, const MbCartPoint3D & vista );
  POINT       GetDevicePoint( const MbFloatPoint3D & pnt ) const; // Выдать точку по декартовой
  POINT       GetDevicePoint( const MbCartPoint3D  & pnt ) const; // Выдать точку по декартовой
  POINT       GetDevicePoint( const MbCartPoint    & pnt ) const; // Точка дана в системе окна
  int64       GetDeviceDistance( double d ) const; // Длина в системе окна

  // Функции обмена данными
  // Methods of Scale and Projection.

  void        SetScale( double newScale ); // Установить новый масштаб вида
  double      GetScale() const { return scale; }
  void        SetVista( double newVista ); // Установить точку наблюдения
  double      GetZVista() const; // Дать координату  точки наблюдения
  const MbCartPoint3D & GetVista() const { return vista; }
  void        SetProjection( wProjections newProj ) { proj=newProj; } // Установить новый масштаб вида
  wProjections GetProjection() const { return proj; }

  IGrDraw &   GetDrawTool() const; // Выдать контекст рисования в окне.
  void        SetSize( int cx, int cy );

  // Функции, связанные с положением
  // Methods of Screen Plane Location.

  const MbCartPoint3D & GetOrigin() const { return place.GetOrigin(); }
  const MbVector3D    & GetAxisX() const { return place.GetAxisX(); }
  const MbVector3D    & GetAxisY() const { return place.GetAxisY(); }
  const MbVector3D    & GetAxisZ() const { return place.GetAxisZ(); }
  const MbPlacement3D & GetOwnPlacement() const { return place; }
  MbPlacement3D GetOwnPlace() const { MbPlacement3D p( place ); 
                                      MbCartPoint3D o, r; 
                                      p.PointProjection( o, r ); 
                                      p.SetOrigin( r );
                                      return p; }

  void        SetOrigin( const MbCartPoint3D & origin );

  void        EnlargeScale(); // Увеличение масштаба
  void        EnlargeScale( MbCartPoint3D & p0 ); // Увеличение масштаба
  void        ReduceScale ();  // Уменьшение масштаба
  void        ReduceScale ( MbCartPoint3D & p0 );  // Уменьшение масштаба

  void        SetOriginIntoCentre( bool b );
  void        CalculateScaleFor( double dx, double dy ); // Установить масштаб для физического размера отображаемого объекта
  void        SetCentreIntoPoint( MbCartPoint3D &p, bool b ); // Установка центра вида в точку
  void        InitPlacement( MbCartPoint3D &p0, MbVector3D &vz, MbVector3D &vx );
  void        CalculateMatrix(); // Вычислить матрицы с учетом масштаба окна
  const MbMatrix3D & GetOwnMatrixInto() const { return into; }
  const MbMatrix3D & GetOwnMatrixFrom() const { return from; }
  MbPlacement3D GetNullPlacement ( bool b = true ) const;
  MbMatrix3D  GetNullMatrixInto( bool b = true ) const;
  MbMatrix3D  GetNullMatrixFrom( bool b = true ) const;
  MbMatrix3D  GetScaleMatrixInto() const;
  MbMatrix3D  GetScaleMatrixFrom() const;

  // Методы управления 
  // Methods of Model Control.

  void        Move( int numb, double range ); // Сдвинуть окно по местной оси номер numb на расстояние range
  void        Rotate( MbCartPoint3D & wc, int numb, double angle ); // Повернуть окно вокруг своей оси номер numb на угол angle, сохранив неподвижной точку центра окна
  void        Orientate( const MbPlacement3D & pl );
  bool        GetMoveVector( POINT & c0, MbVector3D & to ) const; // Дать вектор сдвига начало местной системы в центр окна

  void        InvalidateWindow( bool bErase ); // Перерисовать содержимое окна
  void        EraseBackground();  // Очистить окно
  void        SetFocus( bool mdiActivate ); // Установить фокус на окно
  void        DrawContentWindow( Temporal & temporal, Templane & cplane, COLORREF select, bool isShowMapping ); // Нарисовать окно

  void        CreateWorkWindow( const TCHAR* title, COLORREF bkgColor, const MbVector3D & lt ); // Создать окно свойств
  void        ExecuteGetPointProc(); // Выполнить процесс обработки сообщений ввода точки
  void        BreakProc( PNT_result _getPointResult = PNT_RES_ESC );
  void        Quit(); // Завершить работу приложения
  void        RepeatPrevCmd(); // Повторить предыдущую команду. Repead the last command.
  void        RebuildShellCmd(); // Перестроить тело. Rebuild solid or shell.
  void        SolidFinishCmd(); // Выполнить пропущенную олперацию. Build an omitted operation
  void        SolidEndMoveCmd(); // Сдвинуть конец дерева построения. Move the end of the build log. 
  void        SetColorCmd(); // Изменить цвет элементов. Change objects color.

  bool        KeyboardCommand( uint key, bool ctrl, bool shift );  // Реакция на нажатие клавиатуры
  bool        KeyboardCommandFindPoint( uint key, bool ctrl, bool shift, const POINT& p );  // Реакция на нажатие клавиатуры  в режиме поиска точки
  bool        MouseCommand( uint command, POINT& p, bool shift, int paramCount ); // Обработать событие от мыши
  bool        MouseCommandFindPoint( uint command, POINT& p, bool ctrl, bool shift ); // Обработать событие от мыши в режиме поиска точки
  bool        MouseMoveCommand( POINT& p, bool shift, bool ctrl, bool lbtn, bool mbtn ); // Обработать событие от мыши
  void        PutCoordinates( POINT p ); // Послать сообщение о выводе координат и масштаба

  // Действия над формированием моделей
  // Read and Write Model Methods.

  void        ReadModel ( reader & in );  // Чтение модели из потока.
  void        WriteModel( writer & out ); // Запись модели в поток.

  bool        ReadModelItems ( reader & in, std::vector<MbeSpaceType>& types, CSSArray<const MbItem *> & items );  // Чтение модели из потока (без визуализации).

  void        ReadAddModel( reader & in );  // Прочитать и добавить.

  void        AddObjectsToModel ( std::vector< SPtr<MbItem> >& ready );  // добавить объекты в модель.

  bool        ReadAssembly( reader & in );
  bool        ReadItem ( reader & in );   // Чтение объектов из потока.
  void        WriteItem( writer & out );  // Запись объектов в поток.

  bool        ReplaceItem( MbItem & item, MbItem & newItem, bool saveName );  // Заменить объект.
  void        RefreshDrawItem( MbItem & geomItem ); // Обновить сетку для геометрического объекта.
  void        AddItem( MbItem & newObj, const Style * style, bool showItem ); // Добавление геометрического объекта.
  MbItem *    AddObject( MbSpaceItem & item, bool showItem );
  MbItem *    AddObject( const Style & style, MbSpaceItem * item, bool showItem );
  MbItem *    AddObject( const Style & style, MbPlaneItem * item, const MbPlacement3D * p = nullptr, bool showItem = true );
  MbItem *    AddObject( const Style & style, const MbCartPoint & p0, const MbPlacement3D * p = nullptr, bool showItem = true );

  void        DecomposeAssemblies(); // Разрушить сборки с подсборками на составляющие.
  void        RemoveColorAndVisualAttributes(); // Удалить все визуальные свойства объектов.

  void        DeleteModel(); // Удалить все объекты модели.
  void        DeletePlaneItems(); // Удалить все плоские объекты модели.
  void        DeleteSpaceItems(); // Удалить все пространственные объекты модели.
  void        DeleteModelAttributes(); // Удалить атрибуты модели.

  void        GetObjects( MbeSpaceType type, std::vector<SPtr<MbItem>> & items, std::vector<MbMatrix3D> & matrs ); // Дать все объекты указанного типа.
  void        GetObjects( MbeSpaceType type, RPArray<MbItem> & items, SArray<MbMatrix3D> & matrs ); // Дать все объекты указанного типа.
  void        GetRenderingObjects  ( RPArray<MbItem> & objs ); // Дать все поверхности, тела и оболочки. Get all surfaces, bodies, and shells.
  void        GetLineDrawingObjects( RPArray<MbItem> & objs ); // Дать все кривые и точки. Get all curves and points.
  void        GetAllObjects        ( RPArray<MbItem> & objs ); // Дать все объекты. Get all objects.
  void        CreateCurveArray( MbItem * obj, RPArray<MbCurve> & curveList, bool breakCurve ); // Создать список математических кривых из объектов вида.

  const MbItem * FindObject( const MbSpaceItem * item ) const; // Найти объект по математическому объекту.
  const MbItem * FindObject( const MbPlaneItem * item ) const; // Найти объект по математическому объекту.

  void        CalculateGabarit() const; // Вычислить габарит.
  const MbCube & GetCube() const { return cube; }

  void        GetWire( const MbCartPoint3D & vista, RPArray<MbCurve> & wire, SArray<Style> & styles ); // Построить кривые.
  void        GetSpaceWire    ( const MbItem * obj, RPArray<MbCurve3D> & wire ); // Построить каркас объекта.
  void        GetPlaneWire    ( const MbItem * obj, RPArray<MbCurve3D> & wire ); // Построить каркас объекта.
  void        GetPlaneWire    ( const MbItem * obj, RPArray<MbCurve>   & wire ); // Построить каркас объекта.

  void        GetTriangulationPoints( const MbItem * obj, RPArray<MbPointFrame> & items ); // Построить каркасы триангуляционной сетки.
  void        GetTriangulationWires ( const MbItem * obj, RPArray<MbWireFrame>  & items ); // Построить каркасы триангуляционной сетки.
  void        GetTriangulationPlates( const MbItem * obj, RPArray<MbSpaceItem>  & items ); // Построить пластины триангуляционной сетки.

  void        GetDrawModelGabarit( MbCube & r ); // Дать габарит полигональной модели.
  bool        CutModel( const MbPlacement3D & place_, double distance, size_t & operationCount ); // Разрезать модель.
  size_t      GetTrianglesCount() const; // Число треугольников в сеточной модели.

  // Действия над одним объектом
  // Methods on one Object.

  void        PutObject( const MbItem * obj, bool s, bool eraseVertex = false ); // Нарисовать объект в окне.
  void        PutObject( const MbPath &, bool s, bool eraseVertex = false ); // Нарисовать объект в окне.
  void        ShowObject( const MbItem * obj );  // Нарисовать объект во всех окнах.
  void        PutObjectPart( const MbItem * obj, const MbRefItem * item, bool s ); // Нарисовать объект в окне.
  void        PutObjectPart( const ItemElement &, bool s ); // Нарисовать объект в окне.
  void        EraseObject ( const MbItem * obj ); // Стереть объект во всех окнах.
  void        RedrawObject( const MbItem * obj ); // Перерисовать объект.
  void        RefreshObjectModel( const MbItem * obj ); // Перерисовать объекты модели.

  void        SelectObject( const MbItem * obj, bool refreshDrawing = true );       // Выделить объект.
  void        SelectObject( const MbPath & objPath, bool refreshDrawing = true );   // Выделить объект.
  void        UnselectObject( MbItem * obj, bool refreshDrawing = true );           // Снять выделение с объекта.
  void        UnselectObject( const MbPath & objPath, bool refreshDrawing = true ); // Снять выделение с объекта.
  void        ToggleObject( const MbItem * obj, bool refreshDrawing = true );             // Инвертировать выделение.
  void        ToggleObject( const MbPath & objPath, bool refreshDrawing = true );   // Инвертировать выделение.

  void        SetObjectStyle( MbItem * obj, int s ); // Изменить стиль линий объекта.
  void        SetObjectWidth( MbItem * obj, int w ); // Изменить толщину линий объекта.
  void        SetObjectColor( MbItem * obj, COLORREF newColor ); // Изменить цвет объекта.
  void        SetObjectVisible( MbItem * obj, bool visible ); // Изменить видимость объекта.

  void        TransformObject( MbItem * obj, const MbMatrix3D & matr ); // Преобразование объекта согласно матрице.
  void        MoveObject( MbItem * obj, const MbVector3D & to );           // Сдвиг объекта.
  void        MoveObject( const MbPath & objPath, const MbVector3D & to ); // Сдвиг объекта.
  void        RotateObject( MbItem * obj, const MbAxis3D & axis, double angle ); // Повернуть вокруг оси объекта.

  void        DeleteObject( MbItem * obj );    // Удаление объекта.
  void        DeleteObject( const MbPath & objPath );    // Удаление объекта.

  void        DuplicateObject( const MbItem * obj ); // Дублировать объект.
  void        TransformDuplicateObject( const MbItem * obj, const MbMatrix3D & matr ); // Дублировать объект.

  // Нахождение объекта, карта которого ближе к заданной точке на ней.
  // Methods of Looking for Object.

  // Нахождение сборки или вставки.
  bool        FindRigidItem( MbeSpaceType sType, MbItem *& find, PathMatrix & pathFrom );
  // Нахождение объекта заданного типа (и матрица его преобразования в "мир"), ближайшего к прямой line.
  bool        FindGeometricItem( MbeSpaceType sType, MbeTopologyType tType, MbePlaneType pType,
                                 const MbFloatAxis3D & line, double maxDistance,
                                 MbItem ** find, MbRefItem ** item, PathMatrix & pathFrom );
  // Нахождение объекта заданного типа (и матрица его преобразования в "мир"), ближайшего к прямой line.
  bool        FindNearestItem( MbeSpaceType sType, MbeTopologyType tType,
                               const MbFloatAxis3D & line, double maxDistance,
                               MbItem ** find, MbTopItem ** item, MbMatrix3D & from );
  // Нахождение объекта заданного типа (и матрица его преобразования в "мир"), ближайшего к прямой line.
  bool        FindNearestItem( MbeSpaceType sType, MbeTopologyType tType,
                               const MbFloatAxis3D & line, double maxDistance,
                               MbItem ** find, MbTopItem ** item, PathMatrix & pathFrom );
  // Привязка к ближайшей точке объекта.
  bool        FindNearestSpacePoint( MbeSpaceType typeObj, const MbFloatAxis3D & from, double maxDistance, const MbCartPoint3D & cursorPoint,
                                     MbCartPoint3D & pnt );
  // Привязка к ближайшей точке объекта.
  bool        FindNearestPlanePoint( MbePlaneType typeObj, const MbFloatAxis3D & from, double maxDistance, const MbCartPoint3D & cursorPoint,
                                     MbCartPoint & pnt );
  // Привязка к проекции точки на объект.
  bool        FindNearestSpaceProject( MbeSpaceType typeObj, const MbFloatAxis3D & from, double maxDistance, const MbCartPoint3D & cursorPoint,
                                       MbCartPoint3D & pnt );
  // Привязка к проекции точки на объект.
  bool        FindNearestPlaneProject( MbePlaneType typeObj, const MbFloatAxis3D & from, double maxDistance, const MbCartPoint3D & cursorPoint,
                                       MbCartPoint & pnt );

  // Действия над объектами указанного типа.
  // Methods of Action by Object Type.

  void        DeleteByType( MbeSpaceType  typeObj ); // Удалить объект по типу.
  void        DeleteByType( MbePlaneType  typeObj ); // Удалить объект по типу.

  void        SelectByType( MbeSpaceType  typeObj ); // Выделить объекты по типу.
  void        SelectByType( MbePlaneType  typeObj ); // Выделить объекты по типу.

  void        SetVisibleByType( MbeSpaceType typeObj, bool visible ); // Установить видимость объектов по типу.
  void        SetVisibleByType( MbePlaneType typeObj, bool visible ); // Установить видимость объектов по типу.

  void        ModelSelectByObject( const MbItem * obj ); // Выделить объекты модели.
  int         GetModelObject( MbItem * obj, RPArray<MbItem> & objs ); // Нахождение объектов модели.
  void        MoveObjectInPlacement( MbItem * obj, const MbVector3D & to ); // Переместить объект в конструктивной плоскости.
  void        RotateObjectInPlacement( MbItem * obj, const MbCartPoint3D & point, double angle ); // Повернуть объект в конструктивной плоскости.
  void        TransformObjectInPlacement( MbItem * obj, const MbMatrix & matr ); // Трансформировать объекты в конструктивной плоскости. Objects tranforming.

  void        SetObjectPlacement ( MbItem * obj, const MbPlacement3D & pl );
  // Изменить расстояние во масштабу активного окна.
  double      ScaleDistance( double distance ) const;

  // Действия над селектированными объектами моделей.

  void        SelectModel(); // Выделить модель во всех окнах.
  void        UnselectModel(); // Выделить модель во всех окнах.
  void        ToggleModel(); // Выделить модель во всех окнах.

  void        InverseInRect( POINT p1, POINT p2, COLORREF select ); // Селектировать объекты, находящиеся полностью в прямоугольной области

  void        TransformSelected( const MbMatrix3D & matr ); // Преобразование выделенных объектов согласно матрице.
  void        MoveSelected( const MbVector3D & to );        // Сдвиг выделенных объектов.
  void        RotateSelected( const MbAxis3D & axis, double angle ); // Повернуть вокруг оси выделенных объектов.
  void        DuplicateSelected(); // Дублировать выделенные объекты.
  void        DeleteSelected(); // Удалить все выбранные объекты. Delete all selected objects.
  void        DeleteInvisible(); // Удаление все невидимые объекты. Delete all invisible objects.

//  void        SetSelectedObjectStyle( int w ); // Изменить стиль линий выделенных объектов.
  void        SetSelectedObjectWidth( int w ); // Изменить толщину линий выделенных объектов.
  size_t      GetSelectedObjects( RPArray<MbItem> & objs ); // Нахождение выделенных объектов.
  void        SetSelectedObjectColor( COLORREF newColor ); // Изменить цвет выделенных объектов.

  void        SetSelectColor( COLORREF newColor ); // Изменить цвет выделения объектов.
  COLORREF    GetSelectColor() const { return select; } // Получить цвет выделения объектов.

  void        SetBackgrColor( COLORREF newColor ); // Изменить цвет фона окон.
  COLORREF    GetBackgrColor() const { return backColor; } // Получить цвет фона окон.

  // Возврат к предыдущему состоянию.
  // Undo Methods.

  void        UndoObject( MbItem * item, ActionType act ); // Запомнить объект.
  void        UndoObjects( RPArray<MbItem> & items, ActionType act ); // Запомнить объект.
  ptrdiff_t   UndoCount() const;  // Количество элементов восстановления.
  ActionType  UndoAction() const; // Тип восстановления.
  void        Undo(); // Восстановить предыдущее.
  void        UndoSkeep( ActionType act ); // Пропустить восстановление предыдущего.
  void        UndoFlush(); // Очистить.

  // Сдвинуть окно по местной оси номер numb на расстояние range
  void        MoveWindow( int numb, double range );
  // Повернуть окно вокруг своей оси номер numb на угол angle, сохранив неподвижной точку центра окна
  void        RotateWindow( int numb, double angle );
  // Установить плейсемент окна
  void        OrientateWindow( const MbPlacement3D & plane );
  // Установить ориентацию окна
  void        OrientateWindow( wProjections proj );
  // Вписать модель по размеру окна.
  void        FitModelToWindow();
  // Удалить свойство
  void        RemoveProperty( MbItem * geom );

  // Действия над конвертером преобразования "mesh -> Brep".
  void InitMeshProcessor( MbMeshProcessor * p ) { meshProcessor = p; }
  MbMeshProcessor * GetMeshProcessor() { return meshProcessor; }

  // Добавить в модель атрибуты с данными о виде (или изменить их, если атрибуты есть и change == true).
  void        AddViewData( bool change, MbModel * model =  nullptr );
  // Убрать из модели атрибуты с данными о виде.
  void        RemoveViewData( MbModel * model = nullptr );

protected:
  void        PutRectangleMap( ptrdiff_t index, POINT & p1, POINT & p2 ); // Нарисовать прямоугольник
  void        LeftBottonDown( POINT & p ); // Выделить объект или связь
  void        RightBottonDown( POINT & p, bool shift ); // Выдать свойства или параметры связи
protected:
  void        InverseObject( MbItem * obj, const MbMatrix3D & matr, COLORREF select, bool showItem ); // Инвертировать объект
  void        InverseInRect( POINT p1, POINT p2, MbItem * obj, const MbMatrix3D & matr, COLORREF select, bool showItem );  // Инвертировать объект, если он в прямоугольнике

private:
  bool        FindGeomDrawItem( const MbPath &, const MbItem *& geom, const MbItem *& draw, MbMatrix3D & ) const;
  void       _PutObjectPart( const MbItem * obj, const MbItem * drwObj
                           , const MbMatrix3D & drwMatr, const MbRefItem * item, bool s ); // Нарисовать объект в окне.
  void        PutObjectsParts(); // Нарисовать выбранные пользователем объекты в окне.
  void        DeleteObjectFromSelectedList( const MbItem * obj, const MbRefItem * part = nullptr ); // Удалить объект из массива выбранных элементов.

  // Применить к окну атрибуты с данными о виде, если они есть в текущей модели.
  bool        SetViewData();

OBVIOUS_PRIVATE_COPY( WorkWindow )
}; // WorkWindow


//------------------------------------------------------------------------------
// Linux эмуляция функций GetKeyState()
// ---
bool isShiftPressed();


//------------------------------------------------------------------------------
// Linux эмуляция функций GetKeyState()
// ---
bool isCtrlPressed();


//------------------------------------------------------------------------------
// Linux эмуляция функций GetKeyState()
// ---
bool isCapitalPressed();


//------------------------------------------------------------------------------
// Добавить сетку объекта.
// ---
bool AddGrid( const MbItem * obj, double sag, RPArray<MbGrid> & grids );


//------------------------------------------------------------------------------
// Дать двумерную кривую по объекту-кривой.
// ---
MbCurve * GetCurveOnPlace( const MbItem * curve, MbPlacement3D & crvPlace );


//------------------------------------------------------------------------------
// Дать двумерный контур по объекту-кривой.
// ---
MbContour * GetContourOnPlace( const MbItem * curve, MbPlacement3D & crvPlace );


//------------------------------------------------------------------------------
// Дать пространственную кривую по объекту-кривой
// ---
SPtr<MbCurve3D> GetCurve3D( MbItem * obj, bool sameCurve );


#endif // __TEST_WINDOW_H
