#include <test_window.h>
#include <test_gr_draw.h>
#include <test_manager.h>
#include <test_property.h>
#include <test_temporal_plane.h>
#include <test_temporal.h>
#include <test_variables.h>
#include <test_service.h>
#include <test_rendering.h>
#include <test_computation.h>
#include <test_space.h>
#include <test_edit_space.h>
#include <test_converter.h>
#include <test.h>
#include <test_draw.h>
#include <mb_axis3d.h>
#include <attr_selected.h>
#include <cur_contour3d.h>
#include <cur_line_segment3d.h>
#include <topology_faceset.h>
#include <model_item.h>
#include <model_entity.h>
#include <mesh.h>
#include <assembly.h>
#include <instance_item.h>
#if defined(C3D_MacOS) // mac os X
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
#else
  #include <GL/gl.h>
  #include <GL/glu.h>
#endif // mac os X
#include <attr_color.h>
#include <time.h>
#include <last.h>
#include <study_geometry.h>

using namespace c3d;


#define MAXMAP         32000000000 // Максимальнное значание целочисленной координаты
#define MAX_SCALE      2.0e+6      // Максимальный масштаб
#define MIN_SCALE      0.5e-6      // Минимальный масштаб


//------------------------------------------------------------------------------
// Попадание в область рисования на экране
// ---
inline bool IsCoordGood( const POINT & p, const RECT * rect )
{
  #ifdef C3D_WINDOWS
  bool isGood = -MAXMAP < (ptrdiff_t)p.x && (ptrdiff_t)p.x < MAXMAP &&
                -MAXMAP < (ptrdiff_t)p.y && (ptrdiff_t)p.y < MAXMAP;
  #else
  bool isGood {true};
  // На Windows коордиаты POINT - long, в наших реализациях под другие системы - int32,
  // в который MAXMAP не умещается, и выражение истинно всегда
  #endif

  if ( isGood && rect != nullptr ) {
    int32 rectLimit = std_min( rect->left, rect->right );
    if ( p.x < rectLimit )
      isGood = false;
    else {
      rectLimit = std_max( rect->left, rect->right );
      if ( p.x > rectLimit )
        isGood = false;
      else {
        rectLimit = std_min( rect->bottom, rect->top );
        if ( p.y < rectLimit )
          isGood = false;
        else {
          rectLimit = std_max( rect->bottom, rect->top );
          if ( p.y > rectLimit )
            isGood = false;
        }
      }
    }
  }

  return isGood;
}


//------------------------------------------------------------------------------
// Конструктор
// ---
WorkWindow::WorkWindow( wProjections initProj, COLORREF sc, COLORREF bc )
  : geomModel()
  , drawModel()
  , cube      (      )
  , tmpObjects( 0, 10, true )
  , hideline  ( false )
  , cutPlace  ()
  , cutModel  ( nullptr )
  , undoItems ()
  , select    ( sc )  // Цвет селектирования.
  , backColor ( bc )
  , firstLoading( true )
  , place     (     )
  , into      (     )
  , from      (     )
  , scale     ( 1.0 )
  , vista     ( 0.0, 0.0, 0.0 )
  , proj      ( initProj )
  , drawTool  ( nullptr )
  , renderType( wwrt_OGL )
  , viewSort  ( wvs_Geometry )
  , make_grid ( false )
  , meshProcessor ( nullptr )
  , mouseRotation( false )
  , enableDrawRect( true )
  , lastBase  ()
{
}


//------------------------------------------------------------------------------
//
// ---
void WorkWindow::CreateWorkWindow( const TCHAR* title, COLORREF bkgColor, const MbVector3D & lt )
{
  drawTool = CreateDrawTool( ::CreateMdiClient(title), *this, lt );

  MbVector3D z( 0, 0, 1 );
  MbVector3D x( 1, 0, 0 );

  switch ( proj ) {
    case v_Front :  // Спереди
      z.x =  0; z.y =  0; z.z =  1;
      x.x =  1; x.y =  0; x.z =  0;
      break;

    case v_Rear :   // Сзади
      z.x =  0; z.y =  0; z.z = -1;
      x.x = -1; x.y =  0; x.z =  0;
      break;

    case v_Up :     // Сверху
      z.x =  0; z.y =  1; z.z =  0;
      x.x =  1; x.y =  0; x.z =  0;
      break;

    case v_Down :   // Снизу
      z.x =  0; z.y = -1; z.z =  0;
      x.x =  1; x.y =  0; x.z =  0;
      break;

    case v_Left :   // Слева
      z.x = -1; z.y =  0; z.z =  0;
      x.x =  0; x.y =  0; x.z =  1;
      break;

    case v_Right :  // Справа
      z.x =  1; z.y =  0; z.z =  0;
      x.x =  0; x.y =  0; x.z = -1;
      break;

    case v_Isometry :    // Изометрия
      z.x =  1; z.y =  1; z.z =  1;
      x.x =  1; x.y =  0; x.z = -1;
      break;

    default: break;
  }

  IGrDraw & tool = GetDrawTool();
  TestVariables::drawFactor = (int)( tool.GetZPixelPerMM() + EPSILON );

  tool.SetBackground( bkgColor );

  RECT rc;
  tool.GetClientRect( rc );

  POINT coord;
  coord.x = (rc.left+rc.right) / 2;
  coord.y = (rc.top+rc.bottom) / 2;
  MbVector3D to = - GetCartPoint( coord );

  MbCartPoint3D p( 0.0, 0.0, 0.0 );
  p.Move( to );
  place.Init( p, z, x );
  CalculateMatrix();

  study_area::create_specific_initial_scene(*this);
}


//------------------------------------------------------------------------------
// Деструктор
// ---
WorkWindow::~WorkWindow()
{
  //EraseModel();
  ::ReleaseItem( cutModel );
  undoItems.clear();

  delete drawTool;
  drawTool = nullptr;
  
  // Сбросим подсвеченный объект, что бы не держал память (SPtr).
  TestVariables::ResetHLighted();   
}


//------------------------------------------------------------------------------
// Установить способ отображения.
// ---
void WorkWindow::SetRenderType( WorkRenderType newRendType ) 
{
  renderType = newRendType;
}


//------------------------------------------------------------------------------
// Получить имя модели.
// ---
const TCHAR * WorkWindow::GetModelName() const 
{
  return modelName.c_str();
}


//------------------------------------------------------------------------------
// Установить имя модели.
// ---
void WorkWindow::SetModelName( const TCHAR * buf )
{
  modelName.assign( buf );

  if ( !modelName.empty() ) { // оставляем только имя модели, если прислали полный путь
    TCHAR sep = _T('\\');
    size_t pos = modelName.rfind( sep );
    if ( pos != SYS_MAX_T )
      modelName.erase( 0, pos+1 );
  }

  SetWindowTitle();
}


//------------------------------------------------------------------------------
// Длина имени модели.
// ---
bool WorkWindow::IsModelNameEmpty() const {
  return modelName.empty();
}

bool WorkWindow::CheckFirstLoading( const TCHAR * filename)
{
  c3d::string_t loadingModelName( filename );

  if ( !loadingModelName.empty() ) { // оставляем только имя модели, если прислали полный путь
    TCHAR sep = _T( '\\' );
    size_t pos = loadingModelName.rfind( sep );
    if ( pos != SYS_MAX_T )
      loadingModelName.erase( 0, pos + 1 );
  }

  firstLoading = false;
  if ( modelName.compare( loadingModelName ) != 0 )
    firstLoading = true;
  return firstLoading;
}


//------------------------------------------------------------------------------
// Получить заголовок для окна.
// ---
void WorkWindow::SetWindowTitle() const
{
  if ( drawTool != nullptr ) {
    string_t title;

    switch ( proj ) {
      case v_Arbitrary : title.assign( IDS_TITLE_VIEWARBITRARY ); break; // Произвольный вид
      case v_Front     : title.assign( IDS_TITLE_VIEWFRONT     ); break; // Спереди
      case v_Rear      : title.assign( IDS_TITLE_VIEWREAR      ); break; // Сзади
      case v_Up        : title.assign( IDS_TITLE_VIEWUP        ); break; // Сверху
      case v_Down      : title.assign( IDS_TITLE_VIEWDOWN      ); break; // Снизу
      case v_Left      : title.assign( IDS_TITLE_VIEWLEFT      ); break; // Слева
      case v_Right     : title.assign( IDS_TITLE_VIEWRIGHT     ); break; // Справа
      case v_Isometry  : title.assign( IDS_TITLE_VIEWISOMETRY  ); break; // Изометрия XYZ
    }

    if ( !modelName.empty() ) {
      if ( !title.empty() )
        title.append( _T(" : ") );
      title.append( modelName );
    }

    drawTool->SetWindowTitle( title.c_str() );
  }
}


//------------------------------------------------------------------------------
// Соответствует ли окно контенту?
// ---
bool WorkWindow::IsMyWnd( const void * wndId ) const
{
  return ((drawTool != nullptr) ? drawTool->IsMyWnd( wndId ) : false);
}


//------------------------------------------------------------------------------
// Нарисовать проекцию элемента
// ---
inline void PutPlus( POINT & p0, int32 & _drwFactor, IGrDraw & tool )
{
  if ( ::IsCoordGood( p0, nullptr ) )
  {
    p0.x -= _drwFactor*2;
    tool.MoveTo( p0.x, p0.y );
    p0.x += _drwFactor * 4;      //-V112
    tool.LineTo( p0.x, p0.y );
    p0.x -= _drwFactor*2;

    p0.y -= _drwFactor*2;
    tool.MoveTo( p0.x, p0.y );
    p0.y += _drwFactor * 4;      //-V112
    tool.LineTo( p0.x, p0.y );
    p0.y -= _drwFactor*2;
  }
}


//------------------------------------------------------------------------------
// Нарисовать проекцию элемента
// ---
inline void PutRect( POINT & p0, int32 a, int32 b, IGrDraw & tool )
{
  if ( ::IsCoordGood( p0, nullptr ) ) {
    tool.MoveTo( p0.x, p0.y );
    p0.x -= a;
    p0.y -= a;
    tool.LineTo( p0.x, p0.y );
    p0.y += b;
    tool.LineTo( p0.x, p0.y );
    p0.x += b;
    tool.LineTo( p0.x, p0.y );
    p0.y -= b;
    tool.LineTo( p0.x, p0.y );
    p0.x -= b;
    tool.LineTo( p0.x, p0.y );
    p0.x += b;
    p0.y += b;
    tool.LineTo( p0.x, p0.y );
    p0.y -= b;
    tool.LineTo( p0.x, p0.y );
    p0.x -= b;
    p0.y += b;
    tool.LineTo( p0.x, p0.y );
    p0.y -= b;
    tool.LineTo( p0.x, p0.y );
  }
}


//------------------------------------------------------------------------------
//
// ---
inline
void ChangeRBGComponent( double & c )
{
  if ( c >= 0.0 && c <= 1.0 ) {
    double dc1 = 0.5 * ::fabs(c - 0.0);
    double dc2 = 0.5 * ::fabs(c - 1.0);
    c += ( dc1 > dc2 ) ? -dc1 : dc2;
  }
}


//------------------------------------------------------------------------------
// Нарисовать объект.
// ---
void WorkWindow::PutMeshGDI( const MbMesh * mesh, const MbMatrix3D & _from, const MbRefItem * item,
                             const MbAttributeContainer * hostColor, COLORREF other, bool useOther, bool eraseVertex ) const
{
  if ( mesh != nullptr && mesh->IsVisible() )
  {
    bool eraseMesh = useOther && (other == GetDrawTool().GetBackground());

    int style = mesh->GetStyle();
    int width = mesh->GetWidth();
    width = std_max( width, 1 );

    COLORREF col = (COLORREF)MB_C3DCOLOR; // Цвет элемента по умолчанию.
    if ( useOther )
    {
      col = other;
    }
    else 
    {
      if ( !UpdateColorByVisualAttribute( *mesh, col, false ) )
      {
        if ( hostColor )
          UpdateColorByVisualAttribute( *hostColor, col, false );
      }
    }

    IGrDraw & tool = GetDrawTool();
    bool single = _from.IsSingle();

    if ( !useOther && col == GetBackgrColor() ) { // цвет модели совпал с фоном
      double r0, g0, b0;
      ::uint322RGB( col, r0, g0, b0 );
      ::ChangeRBGComponent( r0 );
      ::ChangeRBGComponent( g0 );
      ::ChangeRBGComponent( b0 );
      col = ::RGB2uint32( r0, g0, b0 );
    }

    bool skipDraw = false;

    if ( mesh->IsComplete() ) { // SD#739012 — ошибки/исключения при чтении асис файлов
      RECT clientRect;
      tool.GetClientRect( clientRect );
      MbCube meshCube;
      mesh->AddYourGabaritTo( meshCube );

      if ( !meshCube.IsEmpty() ) {
        int32 xmin =  SYS_MAX_INT32;
        int32 xmax = -SYS_MAX_INT32;
        int32 ymin =  SYS_MAX_INT32;
        int32 ymax = -SYS_MAX_INT32;

        MbCartPoint3D dpnt;
        for ( size_t k = 0; k < meshCube.GetVerticesCount(); k++ ) {
          meshCube.GetVertex( k, dpnt );
          if ( !single )
            dpnt.Transform( _from );
          POINT p0 = GetDevicePoint( dpnt ); // Выдать точку по декартовой
          xmin = std_min( xmin, p0.x );
          xmax = std_max( xmax, p0.x );
          ymin = std_min( ymin, p0.y );
          ymax = std_max( ymax, p0.y );
        }

        if ( clientRect.left > clientRect.right )
          std::swap( clientRect.left, clientRect.right );
        if ( clientRect.bottom > clientRect.top )
          std::swap( clientRect.bottom, clientRect.top );

        int32 crLimit = clientRect.right;
        if ( xmin >= crLimit )
          skipDraw = true;
        crLimit = clientRect.left;
        if ( xmax <= crLimit )
          skipDraw = true;
        crLimit = clientRect.top;
        if ( ymin >= crLimit )
          skipDraw = true;
        crLimit = clientRect.bottom;
        if ( ymax <= crLimit )
          skipDraw = true;
        if ( skipDraw ) // пропускаем отрисовку за пределами окна вывода (BUG_76177: ускорение отрисовки)
          return;
      }
    }

    MbFloatPoint3D pnt0;

    int32 drawFactor_ = TestVariables::drawFactor; // Коэффициент отображения
    int32 a = (int32)(ONE_HALF * drawFactor_);
    int32 b = 2 * a;
    MbeSpaceType meshType = mesh->GetMeshType();

    if ( (eraseVertex && col != tool.GetBackground() && item == nullptr) &&
         (meshType == st_Solid || meshType == st_Surface) )
    { // Стирание вершин оболочки
      tool.SelectPen( style, width, tool.GetBackground() );

      for ( size_t i = 0, icnt = mesh->ApexesCount(); i < icnt; i++ ) { // Отдельные вершины
        const MbApex3D * apex = mesh->GetApex( i );
        if ( apex != nullptr ) {
          apex->GetPoint( pnt0 );
          if ( !single )
            pnt0.Transform( _from );
          POINT p0 = GetDevicePoint( pnt0 ); // Выдать точку по декартовой
          ::PutPlus( p0, drawFactor_, tool );
        }
      }

      tool.RestorePen();
    }

    { // Отрисовка полигонов
      bool isCurve = mesh->IsACurveMesh();

      bool prevSmall = false;

      for ( size_t i = 0, icnt = mesh->PolygonsCount(); i < icnt; i++ ) {
        const MbPolygon3D * poly = mesh->GetPolygon( i );
        if ( poly == nullptr )
          continue;
        if ( !poly->IsVisible() && !eraseMesh ) {
          bool skipIt = true;
          const MbTopItem * polyParent = poly->TopItem();
          if ( polyParent != nullptr && polyParent->IsA() == tt_CurveEdge ) {
            // Если упадет, то:
            // или 1. Найти, где прочистить указатели на убитые объекты
            // или 2. Начать владеть родительскими объектами
            // или 3. Найти способ не рисовать швы в режиме тонирования, но рисовать в режиме каркаса по единой схеме (битное поле в MbVisible???)
            if ( static_cast<const MbCurveEdge *>(polyParent)->IsSeam() )
              skipIt = false;
          }
          if ( skipIt )
            continue;
        }

        const MbWidth * widthAttr = static_cast<const MbWidth *>( poly->GetSimpleAttribute(at_Width) );
        COLORREF colorPoly = col;
        if ( !useOther )
        {
          UpdateColorByVisualAttribute( *poly, colorPoly, false );
        }
        int p_width = width;
        if ( widthAttr != nullptr ) // Цвет полигона.
          p_width = widthAttr->Width();
        tool.SelectPen( style, p_width, colorPoly );
        
        if ( poly != nullptr && poly->Count() > 0 && (item == nullptr || (item == poly->GetItem())) ) {
          if ( item != nullptr && poly->Count() == 1 ) { // Полигон вершины рисуется в виде знака "плюс"
            poly->GetPoint( 0, pnt0 );
            if ( !single )
              pnt0.Transform( _from );
            POINT p0 = GetDevicePoint( pnt0 ); // Выдать точку по декартовой
            ::PutPlus( p0, drawFactor_, tool );
          }
          else { // Отрисовка обычных полигонов
            bool down = false;

            size_t polyCnt = poly->Count();

            MbCube polyCube;
            poly->AddYourGabaritTo( polyCube );
            double polyDiag = polyCube.GetDiagonal();
            int64 dd = GetDeviceDistance( polyDiag );

            if ( dd < 1 )
              down = true;
            else if ( (dd < 2) && !isCurve )
              down = true;

            if ( down && isCurve ) {
              if ( prevSmall ) {
                prevSmall = false;
                down = false;
              }
              else {
                prevSmall = true;
              }
            }

            if ( !down ) { // невырожденный полигон
              POINT p0, p;
              p0.x = p0.y = p.x = p.y = 0;

              for ( size_t j = 0; j < polyCnt; j++ ) {
                poly->GetPoint( j, pnt0 );
                if ( !single )
                  pnt0.Transform( _from );

                p0 = GetDevicePoint( pnt0 ); // Выдать точку по декартовой

                if ( j > 0 && down ) {
                  if ( p0.x == p.x && p0.y == p.y ) // пропускаем совпадающую точку (BUG_76177: ускорение отрисовки)
                    continue;
                }
                if ( ::IsCoordGood( p0, nullptr ) ) {
                  if ( down )
                    tool.LineTo( p0.x, p0.y );
                  else {
                    tool.MoveTo( p0.x, p0.y );
                    down = true;
                  }
                }
                else {
                  down = false;
                }

                p = p0;
              }
            }
            else if ( polyDiag < Math::metricPrecision ) { // вырожденный полигон (BUG_76177: ускорение отрисовки)
              poly->GetPoint( 0, pnt0 );
              if ( !single )
                pnt0.Transform( _from );
              POINT p0 = GetDevicePoint( pnt0 ); // Выдать точку по декартовой
              ::PutRect( p0, a, b, tool ); // если вырожден, то рисуем как вершину, чтобы было видно
            }
          }
        }
        tool.RestorePen();
      }
    }

    width = mesh->GetWidth();
    tool.SelectPen( style, width, col );

    { // Рисуются отдельные вершины
      for ( size_t i = 0, icnt = mesh->ApexesCount(); i < icnt; i++ ) {
        const MbApex3D * apex = mesh->GetApex( i );
        if ( apex != nullptr && (item == apex->GetItem() ||
             ((item == nullptr) && (meshType != st_Solid) && (meshType != st_Surface))) ) { // Отрисовка точечных объектов не тел
          int p_width = apex->GetWidth();
          p_width = std_max( p_width, 1 );
          if ( width != p_width )
            glLineWidth((GLfloat)p_width);
          apex->GetPoint( pnt0 );
          if ( !single )
            pnt0.Transform( _from );
          POINT p0 = GetDevicePoint( pnt0 ); // Выдать точку по декартовой
          //::PutPlus( p0, drawFactor_, tool );
          ::PutRect( p0, a, b, tool );
          if ( item != nullptr )
            break;
        }
      }
    }

    tool.RestorePen();

    bool needLines = drawTool != nullptr && drawTool->GetRenderMode() == ovm_PrimitiveRender; // Цветные треугольники и квадраты (полосы разбираются)

    if ( (meshType == st_Mesh) || needLines ||
         (mesh->ApexesCount() == 0 && mesh->PolygonsCount() == 0) ) {
      // Рисуются отдельные треугольники
      for ( size_t i = 0, icnt = mesh->GridsCount(); i < icnt; i++ ) {
        const MbGrid * grid = mesh->GetGrid( i );

        //const MbColor * colorAttr = static_cast<const MbColor *>( grid->GetSimpleAttribute(at_Color) );
        COLORREF colorGrid = col;
        if ( !useOther )
        {
          UpdateColorByVisualAttribute( *grid, colorGrid, false );
        }
        tool.SelectPen( style, width, colorGrid );

        if ( grid != nullptr && grid->TrianglesCount() > 0 && (item == nullptr || (item == grid->GetItem())) ) {
          MbCartPoint3D pnt0_, pnt1_, pnt2_;
          for ( size_t j = 0, jcnt = grid->TrianglesCount(); j < jcnt; j++ ) {
            if ( grid->GetTrianglePoints( j, pnt0_, pnt1_, pnt2_ ) ) {
              if ( !single ) {
                pnt0_.Transform( _from );
                pnt1_.Transform( _from );
                pnt2_.Transform( _from );
              }
              POINT p0 = GetDevicePoint( pnt0_ ); // Выдать точку по декартовой
              POINT p1 = GetDevicePoint( pnt1_ ); // Выдать точку по декартовой
              POINT p2 = GetDevicePoint( pnt2_ ); // Выдать точку по декартовой
              if ( ::IsCoordGood( p0, nullptr ) ) {
                tool.MoveTo( p0.x, p0.y );
                tool.LineTo( p1.x, p1.y );
                tool.LineTo( p2.x, p2.y );
                tool.LineTo( p0.x, p0.y );
              }
            }
          }
        }

        if ( grid != nullptr && grid->QuadranglesCount() > 0 && (item == nullptr || (item == grid->GetItem())) ) {
          MbCartPoint3D pnt0_, pnt1_, pnt2_, pnt3_;
          for ( size_t j = 0, jcnt = grid->QuadranglesCount(); j < jcnt; j++ ) {
            if ( grid->GetQuadranglePoints( j, pnt0_, pnt1_, pnt2_, pnt3_ ) ) {
              if ( !single ) {
                pnt0_.Transform( _from );
                pnt1_.Transform( _from );
                pnt2_.Transform( _from );
                pnt3_.Transform( _from );
              }
              POINT p0 = GetDevicePoint( pnt0_ ); // Выдать точку по декартовой
              POINT p1 = GetDevicePoint( pnt1_ ); // Выдать точку по декартовой
              POINT p2 = GetDevicePoint( pnt2_ ); // Выдать точку по декартовой
              POINT p3 = GetDevicePoint( pnt3_ ); // Выдать точку по декартовой
              if ( ::IsCoordGood( p0, nullptr ) ) {
                tool.MoveTo( p0.x, p0.y );
                tool.LineTo( p1.x, p1.y );
                tool.LineTo( p2.x, p2.y );
                tool.LineTo( p3.x, p3.y );
                tool.LineTo( p0.x, p0.y );
              }
            }
          }
        }

        tool.RestorePen();
      }
    }
  }
}


//----------------------------------------------------------------------------------------
// Нарисовать объект.
// ---
void WorkWindow::PutItemGDI( const MbItem * obj, const MbMatrix3D & matr
                           , const MbRefItem * item, const MbAttributeContainer * hostColor, COLORREF other
                           , bool useOther, bool eraseVertex ) const
{
  if ( obj != nullptr )
  {
    MbMatrix3D from_( matr );
    MbeSpaceType type = obj->IsA();
    //const MbColor * colorAttr = static_cast<const MbColor *>(obj->GetSimpleAttribute(at_Color));
    //if ( colorAttr != nullptr )
    //  hostColor = colorAttr;

    switch ( type ) {

      case st_Mesh : {
        PutMeshGDI( static_cast<const MbMesh *>(obj), from_, item, hostColor, other, useOther, eraseVertex );
      } break;

      case st_Instance : { 
        const MbInstance * inst = static_cast<const MbInstance *>(obj);
        if ( inst != nullptr ) {
          const MbMatrix3D _from( matr, inst->GetPlacement().GetMatrixFrom() );
          bool useOtherNext = useOther;
          if ( !useOtherNext && inst->IsSelected() )
            useOtherNext = true;
          PutItemGDI( inst->GetItem(), _from, item, hostColor, other, useOtherNext, eraseVertex );
        }
      } break;

      case st_Assembly : {
        const MbAssembly * asse = static_cast<const MbAssembly *>(obj);
        if ( asse != nullptr ) {
          // MA 2016-03-28 - Теперь сборка имеет всегда единичную матрицу.
          //from.Multiply( asse->GetPlacement().GetMatrixFrom() );
          const MbMatrix3D _from( matr, MbMatrix3D::identity );
          if ( viewSort == wvs_All || viewSort == wvs_Geometry ) { // Drawing the geometric items.
            for ( size_t i = 0, iCount = asse->ItemsCount(); i < iCount; i++ ) {
              const MbItem * asseItem = asse->GetItem( i );
              bool useOtherNext = useOther;
              if ( !useOtherNext && asseItem != nullptr  && asseItem->IsSelected() )
                useOtherNext = true;
              PutItemGDI( asseItem, _from, item, hostColor, other, useOtherNext, eraseVertex );
            }
          }
          if ( viewSort == wvs_All || viewSort == wvs_Constraints ) { // Drawing the geometric constraints.
            bool useOtherNext = useOther;
            if ( !useOtherNext && asse->IsSelected() )
              useOtherNext = true;
            std::vector<const MbMesh *> meshes;
            asse->GetConstraintMesh( meshes ); // Get all polygonal objects for drawing the geometric constraints.
            for ( size_t i = 0, iCount = meshes.size(); i < iCount; i++ ) {
              PutMeshGDI( meshes[i], _from, item, hostColor, other, useOtherNext, eraseVertex );
            }
          }
        }
      } break;
      default:
        break;
    }
  }
}


//------------------------------------------------------------------------------
// Нарисовать
// ---
void WorkWindow::ShowModel()
{
  if ( renderType == wwrt_GDI ) {
    if ( cutModel != nullptr )
      ShowGDI( *cutModel, select );
    else
      ShowGDI( drawModel, select );
  }
  else {
    GetDrawTool().ShowOGL( drawModel, select );
  }
}


//------------------------------------------------------------------------------
// Стереть
// ---
void WorkWindow::EraseModel()
{
  if ( renderType == wwrt_GDI ) {
    EraseGDI();
  }
  if ( renderType == wwrt_OGL ) {
    if ( cutModel != nullptr )
      GetDrawTool().DeleteGL( *cutModel );
    else
      GetDrawTool().DeleteGL( drawModel );
  }
  EraseBackground();
}


//------------------------------------------------------------------------------
// Обновить объекты модели
// ---
void WorkWindow::RefreshModel( bool wire /*= true*/ )
{
  EraseModel();
  CalculateGabarit();
  ::ReleaseItem( cutModel );
  RefreshOGL();

  // Пересчитать сетку соответствующих объектов
  bool grid = ( (GetRenderType() != wwrt_GDI) || make_grid ); // Растровое отображение
  double sag = SetVisualSag();

  DPtr<IProgressIndicator> progBar( &::CreateProgressIndicator(new StrSpyCommon) );

  ///* Наполнить плоскогранную модель.
  geomModel.FillMeshModel( MbStepData(ist_SpaceStep, sag, progBar), MbFormNote(wire, grid, true, TestVariables::exactMesh), drawModel );
  //*/

  /* Тест триангуляции МЦХ
  sag = Math::deviateSag;
  MbStepData ss( ist_DeviationStep, sag );
  //ss.SetStepType( ist_ParamStep );
  geomModel.FillMeshModel( ss, MbFormNote( wire, grid, true, TestVariables::exactMesh ), drawModel );
  //*/

  make_grid = grid;

  // KYA K15SP1 // ShowModel();
  // KYA K15SP1: нужно перерисовать не только модель, но и вспомогательные объекты (оси системы координат)
  InvalidateWindow( false );
  SetFocus( false );
}


//------------------------------------------------------------------------------
// Установить величину стрелки прогиба для визуализации
// ---
double WorkWindow::SetVisualSag()
{
  double s = 0.0;
  if ( GetScale() > s )
    s = GetScale();
  if ( s != 0.0 ) {
    // SD#748311 - детали небольших размеров
    // if (s > 1000)  s = 1000;
    // if (s < 0.001) s = 0.001;
    if ( s > MAX_SCALE ) s = MAX_SCALE;
    if ( s < MIN_SCALE ) s = MIN_SCALE;

    if (s > 10)    s /= 2;
    if (s < 0.1)   s *= 2;
    double sag = 0.15 / s; // Величина стрелки прогиба для визуализации
#ifdef USE_VAR_CLASSES
    Math::visualSag.SetVarValue( sag );
#else
    Math::visualSag = sag;
#endif
  }
  return Math::visualSag;
}


//------------------------------------------------------------------------------
// Стереть содержимое окна
// ---
void WorkWindow::EraseWindow()
{
  if ( renderType == wwrt_GDI ) {
    IGrDraw& tool = GetDrawTool();
    tool.Invalidate();
    tool.BeginPaint();
    ShowAxisGDI();
    tool.EndPaint();
  }
}


//------------------------------------------------------------------------------
// Нарисовать
// ---
void WorkWindow::ShowGDI( MbModel & model, COLORREF select_ )
{
  if ( renderType == wwrt_GDI ) {
    MbModel::ItemConstIterator drawIter( model.CBegin() );
    MbModel::ItemConstIterator drawEnd ( model.CEnd() );
    //const MbColor * hostColor = static_cast<const MbColor *>(drawModel.GetSimpleAttribute(at_Color));

    for ( ; drawIter != drawEnd; ++drawIter ) {
      const MbItem * obj = *drawIter;
    //for ( size_t i = 0, iCount = drawModel.ItemsCount(); i < iCount; i++ ) {
    //  const MbItem * obj = drawModel.GetModelItem( i ); // Выдать объект по индексу
      if ( obj != nullptr ) {
        MbMatrix3D matr;
        PutItemGDI( obj, matr, nullptr, &drawModel, select_, obj->IsSelected() );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Стереть
// ---
void WorkWindow::EraseGDI()
{
  if ( renderType == wwrt_GDI ) {
    MbModel::ItemConstIterator drawIter( drawModel.CBegin() );
    MbModel::ItemConstIterator drawEnd ( drawModel.CEnd() );
    for ( ; drawIter != drawEnd; ++drawIter ) {
      const MbItem * obj = *drawIter;
    //for ( size_t i = 0, iCount = drawModel.ItemsCount(); i < iCount; i++ ) {
    //  const MbItem * obj = drawModel.GetModelItem( i ); // Выдать объект по индексу
      if ( obj != nullptr ) {
        MbMatrix3D matr;
        PutItemGDI( obj, matr, nullptr, nullptr, GetDrawTool().GetBackground(), true );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Удалить временные данные (листы) OpenGL.
// ---
void WorkWindow::RefreshOGL()
{
  if ( drawTool != nullptr )
    drawTool->RefreshOGL();
}


//------------------------------------------------------------------------------
// Отрисовать через OpenGL
// ---
void WorkWindow::ShowOGL( std::vector< SPtr<MbItem> > * addItems )
{
  if ( drawTool != nullptr )
    drawTool->ShowOGL( drawModel, select, addItems );
}


//------------------------------------------------------------------------------
// Выдать точку
// ---
MbFloatPoint3D WorkWindow::GetFloatPoint( const POINT & mouseCoord ) const
{
  //OV Lnx_DP/LP GetDrawTool().DPtoLP( mouseCoord );
  MbFloatPoint3D pnt( (float)mouseCoord.x, (float)mouseCoord.y, 0.0 );
  pnt.Transform( GetOwnMatrixFrom() ); // Переводим точку в мировую СК
  return pnt;
}


//------------------------------------------------------------------------------
// Выдать точку
// ---
MbCartPoint3D WorkWindow::GetCartPoint( const POINT & mouseCoord ) const
{
  //OV Lnx_DP/LP GetDrawTool().DPtoLP( mouseCoord );
  MbCartPoint3D pnt( (double)mouseCoord.x, (double)mouseCoord.y, 0.0 );
  pnt.Transform( GetOwnMatrixFrom() ); // Переводим точку в мировую СК
  return pnt;
}


//------------------------------------------------------------------------------
// Выдать точку
// ---
MbCartPoint WorkWindow::GetPlanePoint( const POINT & mouseCoord ) const
{
  MbCartPoint p;
  GetDrawTool().DPtoMM( mouseCoord, p, scale );
  return p;
}


//------------------------------------------------------------------------------
// Выдать расстояние
// ---
double WorkWindow::GetDistance( int64 mouseDist ) const
{
  //double factor = ::sqrt( factorX * factorY );
  //float f = (float)( mouseDist / (factor*scale) );
  //return f;
  return (double)( mouseDist / (GetDrawTool().GetZPixelPerMM() * scale) );
}


//------------------------------------------------------------------------------
// Дать точку карты
// ---
POINT WorkWindow::InitPoint( MbCartPoint3D & pnt, const MbMatrix3D & matr, const MbCartPoint3D & vista_ )
{
  pnt.Transform( matr ); // Перевод в экранную СК
  if ( vista_.x > DELTA_MIN ) {
    double z = vista_.z - pnt.z;

    if (z > DELTA_MIN)
      z = vista_.z / z;
    else
      z = vista_.z / DELTA_MIN;

    pnt.x -= vista_.x;
    pnt.y -= vista_.y;
    pnt.x *= z;
    pnt.y *= z;
    pnt.x += vista_.x;
    pnt.y += vista_.y;
  }

  POINT p;

  if ( pnt.x >= SYS_MAX_INT32 )
    p.x = SYS_MAX_INT32;
  else if ( pnt.x <= SYS_MIN_INT32 )
    p.x = SYS_MIN_INT32;
  else
    p.x = (int32)pnt.x;

  if ( pnt.y >= SYS_MAX_INT32 )
    p.y = SYS_MAX_INT32;
  else if ( pnt.y <= SYS_MIN_INT32 )
    p.y = SYS_MIN_INT32;
  else
    p.y = (int32)pnt.y;

  //OV Lnx_DP/LP GetDrawTool().LPtoDP( p );
  return p;
}


//------------------------------------------------------------------------------
// Выдать точку по декартовой
// ---
POINT WorkWindow::GetDevicePoint( const MbFloatPoint3D & pnt ) const
{
  MbFloatPoint3D cp(pnt);
  cp.Transform(GetOwnMatrixInto());

  POINT p;
  p.x = (int32)cp.x;
  p.y = (int32)cp.y;

  //OV Lnx_DP/LP GetDrawTool().LPtoDP( p );
  return p;
}


//------------------------------------------------------------------------------
// Выдать точку по декартовой
// ---
POINT WorkWindow::GetDevicePoint( const MbCartPoint3D & pnt ) const
{
  MbCartPoint3D cp(pnt);
  cp.Transform(GetOwnMatrixInto());

  POINT p;

  cp.x = std_min( cp.x, (double)SYS_MAX_INT32 );
  cp.y = std_max( cp.y, (double)SYS_MIN_INT32 );

  p.x = (int32)cp.x;
  p.y = (int32)cp.y;

  //OV Lnx_DP/LP GetDrawTool().LPtoDP( p );
  return p;
}


//------------------------------------------------------------------------------
// Выдать точку по декартовой
// ---
POINT WorkWindow::GetDevicePoint( const MbCartPoint & pnt ) const // Точка дана в системе окна
{
  POINT p;
  GetDrawTool().MMtoDP( pnt, p, scale );
  return p;
}


//------------------------------------------------------------------------------
// Выдать точку по декартовой
// ---
int64 WorkWindow::GetDeviceDistance( double d ) const // Точка дана в системе окна
{
//  double factor = ::sqrt( factorX * factorY );
//  int32 l = (int32)( d * factor * scale );
//  return l;

  double dd = d * GetDrawTool().GetZPixelPerMM() * scale;
  return (int64)( dd );
}


//------------------------------------------------------------------------------
// Установить положение СК
// ---
void WorkWindow::SetOrigin( const MbCartPoint3D & origin )
{
  place.SetOrigin( origin );
  CalculateMatrix();
}


//------------------------------------------------------------------------------
// Установить новый масштаб вида
// ---
void WorkWindow::SetScale( double newScale ) {
  if ( MIN_SCALE <= newScale && newScale <= MAX_SCALE ) {
    scale = newScale;
    CalculateMatrix();
  }
}


//------------------------------------------------------------------------------
// Установить точку наблюдения
// ---
void WorkWindow::SetVista( double newVista )
{
  RECT rc;
  GetDrawTool().GetClientRect( rc/*, false DPtoLP*/ );

  if ( fabs(newVista) < DELTA_MIN )
    vista.SetZero();
  else
    vista.Init( rc.right/2, rc.bottom/2, fabs(newVista) * GetDrawTool().GetZPixelPerMM() );

  if ( renderType == wwrt_OGL )
    GetDrawTool().SetSizeGL( rc.right, rc.bottom, scale );
}


//------------------------------------------------------------------------------
// Дать координату  точки наблюдения
// ---
double WorkWindow::GetZVista() const {
  double newVista = vista.z / GetDrawTool().GetZPixelPerMM() /*0.5*(factorX+factorY)*/;
  if ( fabs(newVista) < DELTA_MIN )
    newVista = 0;

  return newVista;
}


//------------------------------------------------------------------------------
// Выдать контекст рисования в окне.
// ---
IGrDraw & WorkWindow::GetDrawTool() const
{
  PRECONDITION(drawTool); return *drawTool;
}


//------------------------------------------------------------------------------
// Установить точку наблюдения
// ---
void WorkWindow::SetSize( int cx, int cy )
{
  if ( vista.z > DELTA_MIN )
  {
    vista.x = cx/2;
    vista.y = cy/2;
  }

  //if ( renderType == wwrt_OGL )
  GetDrawTool().SetSizeGL( cx, cy, scale );
}


//------------------------------------------------------------------------------
// Увеличение масштаба
// ---
void WorkWindow::EnlargeScale()
{
  EraseWindow(); // Стереть

  double k = sqrt(sqrt(2.0));
  MbVector3D to;
  MbCartPoint3D p0, p1;
  POINT c;
  RECT rc;
  rc.bottom = rc.top = rc.left = rc.right = 0;
  if ( scale*k <= MAX_SCALE ) {
    GetDrawTool().GetClientRect( rc/*, false DPtoLP*/ );
    c.x = (rc.right + rc.left)/2;
    c.y = (rc.top + rc.bottom)/2;
    p0 = GetCartPoint( c ); // Старый центр экрана должен сохраниться
    scale *= k;
    CalculateMatrix();
    p1 = GetCartPoint( c ); // Новый центр экрана
    to = p0 - p1;
    place.Move(to);
    CalculateMatrix();
  }
  GetDrawTool().SetSizeGL( rc.right, rc.bottom, scale );

  //if ( renderType != wwrt_OGL )
  //  make_grid = false;

  InvalidateWindow( false );
  SetFocus( false );
}


//------------------------------------------------------------------------------
// Увеличение масштаба
// ---
void WorkWindow::EnlargeScale( MbCartPoint3D & p0 )
{
  EraseWindow(); // Стереть

  if ( cutModel != nullptr ) {
    MbPlacement3D place_( cutPlace );
    MbVector3D to( cutPlace.GetAxisZ() );
    double s = GetScale();
    to *= TestVariables::delta_range / ( NEWTON_COUNT * s );
    place_.Move( to );
    size_t operationCount = 0;
    CutModel( place_, TestVariables::distance, operationCount );
  }
  else {
    double k = sqrt(sqrt(2.0));
    MbVector3D to;
    MbCartPoint3D p1;
    POINT c;

    if ( scale*k <= MAX_SCALE ) {
      scale *= k;
      c = GetDevicePoint( p0 ); // Положение курсора
      CalculateMatrix();
      p1 = GetCartPoint( c ); // Новое положение курсора
      to = p0 - p1;
      place.Move(to);
      CalculateMatrix();
    }

    RECT rc;
    GetDrawTool().GetClientRect( rc/*, false DPtoLP*/ );
    GetDrawTool().SetSizeGL( rc.right, rc.bottom, scale );

    //if ( renderType != wwrt_OGL )
    //  make_grid = false;
  }

  InvalidateWindow( false );
  SetFocus( false );
}


//------------------------------------------------------------------------------
// Уменьшение масштаба
// ---
void WorkWindow::ReduceScale()
{
  EraseWindow(); // Стереть

  double k = 1.0/sqrt(sqrt(2.0));
  MbVector3D to;
  MbCartPoint3D p0, p1;
  POINT c;
  RECT rc;
  rc.bottom = rc.top = rc.left = rc.right = 0;

  if ( scale*k >= MIN_SCALE ) {
    GetDrawTool().GetClientRect( rc/*, false DPtoLP*/ );
    c.x = (rc.right + rc.left)/2;
    c.y = (rc.top + rc.bottom)/2;
    p0 = GetCartPoint( c ); // Старый центр экрана должен сохраниться
    scale *= k;
    CalculateMatrix();
    p1 = GetCartPoint( c ); // Новый центр экрана
    to = p0 - p1;
    place.Move(to);
    CalculateMatrix();
  }

  GetDrawTool().SetSizeGL( rc.right, rc.bottom, scale );

  //if ( renderType != wwrt_OGL )
  //  make_grid = false;

  InvalidateWindow( false );
  SetFocus( false );
}


//------------------------------------------------------------------------------
// Уменьшение масштаба
// ---
void WorkWindow::ReduceScale( MbCartPoint3D & p0 )
{
  EraseWindow(); // Стереть

  if ( cutModel != nullptr ) {
    MbPlacement3D place_( cutPlace );
    MbVector3D to( cutPlace.GetAxisZ() );
    double s = GetScale();
    to *= -TestVariables::delta_range / ( NEWTON_COUNT * s );
    place_.Move( to );
    size_t operationCount = 0;
    CutModel( place_, TestVariables::distance, operationCount );
  }
  else {
    double k = 1.0/sqrt(sqrt(2.0));
    MbVector3D to;
    MbCartPoint3D p1;
    POINT c;

    if ( scale*k >= MIN_SCALE ) {
      scale *= k;
      c = GetDevicePoint( p0 ); // Положение курсора
      CalculateMatrix();
      p1 = GetCartPoint( c ); // Новое положение курсора
      to = p0 - p1;
      place.Move(to);
      CalculateMatrix();
    }

    RECT rc;
    GetDrawTool().GetClientRect( rc/*, false DPtoLP*/ );
    GetDrawTool().SetSizeGL( rc.right, rc.bottom, scale );

    //if ( renderType != wwrt_OGL )
    //  make_grid = false;
  }

  InvalidateWindow( false );
  SetFocus( false );
}


//------------------------------------------------------------------------------
// Установка нуля в центр вида
// ---
void WorkWindow::SetOriginIntoCentre( bool b )
{
  EraseWindow(); // KYA медленно // currentView->EraseModel( drawModel, backgr ); // Стереть

  MbCartPoint3D c(0,0,0);
  if ( b && !cube.IsEmpty() ) {
    cube.GetCentre(c);
    wProjections proj_ = GetProjection();
    double dx, dy;
    switch ( proj_ ) {
      case v_Front :
      case v_Rear : {
        dx = cube.pmax.x-cube.pmin.x;
        dy = cube.pmax.y-cube.pmin.y;
      } break;
      case v_Up :
      case v_Down : {
        dx = cube.pmax.x-cube.pmin.x;
        dy = cube.pmax.z-cube.pmin.z;
      } break;
      case v_Left :
      case v_Right : {
        dx = cube.pmax.z-cube.pmin.z;
        dy = cube.pmax.y-cube.pmin.y;
      } break;
      default : {
//        dx = 0.7 * cube.GetDiagonal();
//        dy = dx;
        dx = (cube.pmax.y-cube.pmin.y) +
             (cube.pmax.x-cube.pmin.x) +
             (cube.pmax.z-cube.pmin.z);
        dx *= 0.4;
        dy = dx;
      } break;
    }
    CalculateScaleFor(dx,dy);
  }
  SetCentreIntoPoint(c,b);
  // KYA обновится по WM_PAINT // currentView->ShowModel( drawModel, select ); // Нарисовать
  InvalidateWindow( false );
  SetFocus( false );
}


//------------------------------------------------------------------------------
// Установить масштаб для физического размера отображаемого объекта
// ---
void WorkWindow::CalculateScaleFor( double dx, double dy ) {
  MbVector3D to;
  MbCartPoint3D p0, p1;//, p2;
  MbCartPoint q1, q2;
  POINT c;
  RECT rc;

  if (dx==0 || dy==0)
    return;

  GetDrawTool().GetClientRect( rc/*, false DPtoLP*/ );

  c.x = rc.left;
  c.y = rc.top;
  q1 = GetPlanePoint( c );      // Верхний левый угол экрана
//  p1 = GetCartPoint( c );       // Верхний левый угол экрана
  c.x = rc.right;
  c.y = rc.bottom;
  q2 = GetPlanePoint( c );      // Нижний правый угол экрана
//  p2 = GetCartPoint( c );       // Нижний правый угол экрана
  c.x = (rc.right + rc.left)/2;
  c.y = (rc.top + rc.bottom)/2;
  p0 = GetCartPoint( c );       // Центр экрана
  double kx = scale*0.9*fabs(q2.x-q1.x) / dx; // Новый масштаб
  double ky = scale*0.9*fabs(q2.y-q1.y) / dy; // Новый масштаб
  double k = std_min( kx, ky );            // Новый масштаб
//  double s = p1.DistanceToPoint(p2);   // Размер видимого на экране объекта
//  double k = scale*0.7*s/d;            // Новый масштаб
  if ( k < MIN_SCALE )
    k = MIN_SCALE;
  if ( k > MAX_SCALE )
    k = MAX_SCALE;
  scale = k;
  CalculateMatrix();
  p1 = GetCartPoint( c );       // Новый центр экрана
  to = p0 - p1;
  place.Move(to);
  CalculateMatrix();

  if ( renderType == wwrt_OGL )
    GetDrawTool().SetSizeGL( rc.right, rc.bottom, scale );
}


//------------------------------------------------------------------------------
// Установка центра вида в точку
// ---
void WorkWindow::SetCentreIntoPoint( MbCartPoint3D & p, bool b )
{
  RECT rc;
  POINT coord;
  MbVector3D to;

  GetDrawTool().GetClientRect( rc/*, false DPtoLP*/ );

  coord.x = (rc.left+rc.right) / 2;
  coord.y = (rc.top+rc.bottom) / 2;
  to = p - GetCartPoint( coord );
  if ( !b ) // По z тоже совместить
    to = to - place.GetAxisZ() * (to * place.GetAxisZ());

  place.Move( to );
  CalculateMatrix();
}


//------------------------------------------------------------------------------
// Инициализация плейсмента
// ---
void WorkWindow::InitPlacement( MbCartPoint3D & p0, MbVector3D & vz, MbVector3D & vx ) {
  place.Init( p0, vz, vx );
  CalculateMatrix();
}


//------------------------------------------------------------------------------
// Вычислить матрицы
// ---
void WorkWindow::CalculateMatrix()
{
  from = place.GetMatrixFrom();
  from.ScaleX( 1.0 / scale / GetDrawTool().GetXPixelPerMM() /*factorX*/ );
  from.ScaleY( 1.0 / scale / GetDrawTool().GetYPixelPerMM() /*factorY*/ );
  from.ScaleZ( 1.0 / scale / GetDrawTool().GetZPixelPerMM() /*(0.5*(factorX+factorY))*/ );
  from.Invert(1); // Инвертировать ось Y для режима MM_Text
  from.SetOffset( place.GetOrigin() ); // Сдвиг не масштабируется
  from.Div( into );
}


//------------------------------------------------------------------------------
// Выдать локальную СК без учета сдвига начала и масштаба
// ---
MbPlacement3D WorkWindow::GetNullPlacement( bool b ) const
{
  MbCartPoint3D p(0,0,0);
  MbPlacement3D placeTmp( place );
  if ( b )
    placeTmp.SetOrigin( p );
  return placeTmp;
}


//------------------------------------------------------------------------------
// Выдать матрицу преобразования в локальную СК без учета сдвига начала и масштаба
// ---
MbMatrix3D WorkWindow::GetNullMatrixInto( bool b ) const
{
  MbMatrix3D from0; // from - прямая матрица(A)    R = r * A;
  MbMatrix3D into0; // into - обратная матрица(A~) r = R * A~;
  MbCartPoint3D p(0,0,0);

  from0 = place.GetMatrixFrom();
  if ( b )
    from0.SetOffset( p );

  from0.Div( into0 );

  return into0;
}


//------------------------------------------------------------------------------
// Выдать матрицу преобразования из локальной СК без учета сдвига начала и масштаба
// ---
MbMatrix3D WorkWindow::GetNullMatrixFrom( bool b ) const
{
  MbMatrix3D from0;
  MbCartPoint3D p(0,0,0);

  from0 = place.GetMatrixFrom();
  if ( b )
    from0.SetOffset( p );

  return from0;
}


//------------------------------------------------------------------------------
// Выдать матрицу преобразования в локальную СК
// ---
MbMatrix3D WorkWindow::GetScaleMatrixInto() const
{
  MbMatrix3D from0; // from - прямая матрица(A)    R = r * A;
  MbMatrix3D into0; // into - обратная матрица(A~) r = R * A~;

  from0 = place.GetMatrixFrom();
  from0.Scale( 1.0 / scale );
  from0.SetOffset( place.GetOrigin() );
  from0.Div( into0 );

  return into0;
}


//------------------------------------------------------------------------------
// Выдать матрицу преобразования из локальной СК
// ---
MbMatrix3D WorkWindow::GetScaleMatrixFrom() const
{
  MbMatrix3D from0;

  from0 = place.GetMatrixFrom();
  from0.Scale( 1.0 / scale );
  from0.SetOffset( place.GetOrigin() );

  return from0;
}


//------------------------------------------------------------------------------
// Сдвинуть окно по местной оси номер numb на расстояние range
// ---
void WorkWindow::Move( int numb, double range ) {
  MbVector3D to;

  switch ( numb ) {
    case 1 :
      to = place.GetAxisX()*range/scale;
    break;
    case 2 :
      to = place.GetAxisY()*range/scale;
    break;
    default :
      to = place.GetAxisZ()*range/scale;
      if (vista.z>DELTA_MIN)
        if (vista.z + range * GetDrawTool().GetZPixelPerMM() > DELTA_MIN)
          vista.z += range * GetDrawTool().GetZPixelPerMM();
  }

  to.Invert();
  place.Move( to );
  CalculateMatrix();
}


//------------------------------------------------------------------------------
// Повернуть окно вокруг своей оси номер numb на угол angle, сохранив неподвижной точку центра окна
// ---
void WorkWindow::Rotate( MbCartPoint3D &wc, int numb, double angle ) {
  MbAxis3D axis0;

  switch ( numb ) {
    case 1 :
      axis0.SetAxisZ() = place.GetAxisX();
    break;
    case 2 :
      axis0.SetAxisZ() = place.GetAxisY();
    break;
    default :
      axis0.SetAxisZ() = place.GetAxisZ();
  }

  axis0.SetOrigin() = wc;
  place.Rotate( axis0, -angle );
  CalculateMatrix();
}


//------------------------------------------------------------------------------
// Установить плайсемент окна
// ---
void WorkWindow::Orientate( const MbPlacement3D & pl )
{
  MbCartPoint3D p0, p1;
  MbVector3D to;
  POINT c;
  RECT rc;

  GetDrawTool().GetClientRect( rc/*, false DPtoLP*/ );
  c.x = (rc.right + rc.left)/2;
  c.y = (rc.top + rc.bottom)/2;
  p0 = GetCartPoint( c ); // Старый центр экрана должен сохраниться

  place.Init( pl );
  CalculateMatrix();
  p1 = GetCartPoint( c ); // Новый центр экрана
  to = p0 - p1;
  place.Move(to);
  CalculateMatrix();
}


//------------------------------------------------------------------------------
// Дать вектор сдвига начало местной системы в центр окна
// ---
bool WorkWindow::GetMoveVector( POINT & c, MbVector3D & to ) const {
  RECT rc;
  GetDrawTool().GetClientRect( rc/*, false DPtoLP*/ );

  c.x = (rc.right + rc.left)/2;
  c.y = (rc.top + rc.bottom)/2;

  POINT c0;
  c0.x = 0;
  c0.y = 0;
  //OV Lnx_DP/LP GetDrawTool().GetVPOrg( c0 );

  if ( ::abs_t(c.x - c0.x)>1 || ::abs_t(c.y - c0.y)>1 ) {
    to = GetCartPoint( c ) - GetCartPoint( c0 );
    to = to - place.GetAxisZ() * (to * place.GetAxisZ());
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// Инвертировать объект
// ---
void WorkWindow::InverseObject( MbItem * obj, const MbMatrix3D & matr, COLORREF select_, bool showItem )
{
  if ( obj != nullptr ) {
    MbMatrix3D from_;
    const MbItem * geomItem = FindGeomItem( obj, from_ );
    if ( geomItem != nullptr ) {
      bool useSelect = !obj->IsSelected();
      obj->SetSelected( useSelect );
      ((MbItem *)geomItem)->SetSelected( useSelect );
      if ( showItem ) {
        if ( renderType == wwrt_GDI )
          PutItemGDI( obj, matr, nullptr, nullptr, select_, useSelect );
        else if ( renderType == wwrt_OGL ) {
          GetDrawTool().RefreshOGL();
          GetDrawTool().ShowOGL( drawModel, select_ );
        }
        else {
          PRECONDITION( false );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Находится ли объект полностью в прямоугольной области
// ---
inline
bool IsMeshInRect( const WorkWindow & wnd, const POINT & p1, const POINT & p2, const MbItem * obj, const MbMatrix3D & matr )
{
  bool result = false;

  if ( (obj != nullptr) && obj->IsA() == st_Mesh ) {
    const MbMesh * mesh = static_cast<const MbMesh *>(obj); // Полигонный плоскогранный объект
    MbCartPoint r1( wnd.GetPlanePoint( p1 ) );
    MbCartPoint r2( wnd.GetPlanePoint( p2 ) );
    MbRect rect;

    rect |= r1;
    rect |= r2;

    MbPlacement3D transPlace( wnd.GetOwnPlacement() );
    if ( !matr.IsSingle() ) {
      MbMatrix3D divMatr;
      matr.Div( divMatr );
      transPlace.Transform( divMatr );
    }

    result = mesh->InsideLocation( transPlace, rect, METRIC_REGION );
  }
  return result;
}

//------------------------------------------------------------------------------
// Инвертировать объект, если он в прямоугольнике
// ---
void WorkWindow::InverseInRect( POINT p1, POINT p2, MbItem * obj, const MbMatrix3D & matr, COLORREF select_, bool showItem )
{
  if ( obj != nullptr )
  {
    if ( obj->IsA() == st_Mesh ) {
      if ( ::IsMeshInRect( *this, p1, p2, obj, matr ) )
        InverseObject( obj, matr, select_, showItem );
    }
    else if ( obj->IsA() == st_Instance ) {
      MbInstance * inst = static_cast<MbInstance *>( obj );
      MbItem * item = inst->SetItem();
      if ( item != nullptr ) {
        const MbMatrix3D from_( matr, inst->GetPlacement().GetMatrixFrom() );
        if ( item->IsA() == st_Mesh ) 
        {
          if ( ::IsMeshInRect( *this, p1, p2, item, from_ ) )
            InverseObject( obj, matr, select_, showItem );
        }
        else
          InverseInRect( p1, p2, item, from_, select_, showItem );
      }
    }
    else if ( obj->IsA() == st_Assembly )
    {
      MbAssembly * assembly = static_cast<MbAssembly *>( obj );
      // MA 2016-03-28 - Теперь сборка имеет всегда единичную матрицу.
      const MbMatrix3D from_( matr, MbMatrix3D::identity );
      for ( size_t i = 0, iCount = assembly->ItemsCount(); i < iCount; i++ ) {
        InverseInRect( p1, p2, assembly->SetItem(i), from_, select_, showItem );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Селектировать объекты,  находящиеся полностью в прямоугольной области
// ---
void WorkWindow::InverseInRect( POINT p1, POINT p2, COLORREF select_ )
{
  MbModel::ItemConstIterator drawIter( drawModel.CBegin() );
  MbModel::ItemConstIterator drawEnd ( drawModel.CEnd() );
  for ( ; drawIter != drawEnd; ++drawIter ) {
    const MbItem * obj = *drawIter;
  //for ( size_t i = 0, iCount = drawModel.ItemsCount(); i < iCount; i++ ) {
  //  const MbItem * obj = drawModel.GetModelItem( i ); // Выдать объект по индексу
    if ( obj != nullptr ) {
      const MbMatrix3D matr;
      if ( renderType == wwrt_GDI )
        InverseInRect( p1, p2, (MbItem *)obj, matr, select_, true );
      else if ( renderType == wwrt_OGL ) {
        InverseInRect( p1, p2, (MbItem *)obj, matr, select_, false );
      }
    }
  }

  RefreshOGL();
  if ( renderType == wwrt_OGL )
    ShowOGL();
}

//------------------------------------------------------------------------------
// Перерисовать содержимое окна
// ---
void WorkWindow::InvalidateWindow( bool bErase )
{
  GetDrawTool().Invalidate( bErase );
  PutObjectsParts();
}


//------------------------------------------------------------------------------
// Очистить окно
// ---
void WorkWindow::EraseBackground()
{
  if ( GetRenderType() != wwrt_RED )
    GetDrawTool().FillRectangle(); // Залить прямоугольник (все окно) цветом фона
}

//------------------------------------------------------------------------------
// Установить фокус на окно
// ---
void WorkWindow::SetFocus(bool mdiActivate)
{
  GetDrawTool().SetWindowFocus( mdiActivate );
}


//------------------------------------------------------------------------------
// Нарисовать окно
// ---
void WorkWindow::DrawContentWindow( Temporal & temporal, Templane & cplane, COLORREF select_, bool isShowMapping )
{
  MbModel & model = ( cutModel != nullptr ) ? *cutModel : drawModel;
  IGrDraw & tool = GetDrawTool();

  switch ( GetRenderType() )
  {
    case wwrt_GDI :
      {
        //OV_Lnx tool.BeginPaint();

        ShowAxisGDI();

        if ( isShowMapping )
          ShowMapping();
        else
          ShowGDI( model, select_ );

        if ( temporal.Exist() )
          temporal.Show( this );

        if ( cplane.Exist() )
          cplane.Show( this );

        //OV_Lnx tool.EndPaint();
      }
      break;
    case wwrt_OGL:
      {
        // в режиме OGL temporal ее вызывает
        // tool.ShowOGL( model, select_ );

        if ( temporal.Exist() )
          temporal.Show( this );
        else
          tool.ShowOGL( model, select_ );
      }
      break;
    default :
      PRECONDITION( false );
      break;
  }
}


//------------------------------------------------------------------------------
// Отрисовать оси координат
// ---
void WorkWindow::ShowAxisGDI( int length, int thick )
{
  if ( TestVariables::enableWCS ) {
    PRECONDITION( renderType == wwrt_GDI );

    double l = length / scale;
    MbCartPoint3D p0( 0, 0, 0 ),
                  px( l, 0, 0 ),
                  py( 0, l, 0 ),
                  pz( 0, 0, l ); // Точки в мировой СК
    POINT pd0;
    POINT pdx;
    POINT pdy;
    POINT pdz;

    // Перевод в экранную СК
    pd0 = InitPoint( p0, into, vista );
    pdx = InitPoint( px, into, vista );
    pdy = InitPoint( py, into, vista );
    pdz = InitPoint( pz, into, vista );

    int m = (int)( 0.8 * TestVariables::drawFactor );
    int x = 2*m;
    int y = 2*x;
    int k = x;

    //COLORREF col;
    //GetDrawTool().GetBackground(col);

    POINT c000;
    c000.x = (int32)( GetDrawTool().GetXPixelPerMM() * length * 0.6 );
    c000.y = (int32)( GetDrawTool().GetYPixelPerMM() * length * 0.6 );

    IGrDraw& tool = GetDrawTool();
    tool.FillRectangle(); // Залить прямоугольник цветом фона

    POINT c0;
    c0.x = 0;
    c0.y = 0;

    RECT rc;
    tool.GetClientRect( rc/*, false DPtoLP*/ );
    rc.left = rc.left - c0.x;
    rc.top  = rc.top  - c0.y;
    rc.right  = rc.right  - c0.x;
    rc.bottom = rc.bottom - c0.y;

    if ( ::IsCoordGood( pd0, nullptr ) )
    {
      int b = 192;

      // Отрисовка осей глобальной системы координат
      if ( ::IsCoordGood( pdx, nullptr ) ) {
        tool.SelectPen( thick, b, 0, 0, true/*xor*/ );

        tool.MoveTo( pd0.x, pd0.y );
        tool.LineTo( pdx.x, pdx.y );

        tool.RestorePen();
      }

      if ( ::IsCoordGood( pdy, nullptr ) ) {
        tool.SelectPen( thick, 0, b, 0, true/*xor*/ );

        tool.MoveTo( pd0.x, pd0.y );
        tool.LineTo( pdy.x, pdy.y );

        tool.RestorePen();
      }

      if ( ::IsCoordGood( pdz, nullptr ) ) {
        tool.SelectPen( thick, 0, 0, b, true/*xor*/ );

        tool.MoveTo( pd0.x, pd0.y );
        tool.LineTo( pdz.x, pdz.y );

        tool.RestorePen();
      }

      if ( (pd0.x < 0) || (pd0.x > (rc.right-rc.left)) ||
           (pd0.y < 0) || (pd0.y > (rc.bottom-rc.top)) ) {
        // Отрисовка осей ориентации глобальной системы координат (тонкими в углу)
        tool.SelectPen( 1, b, 0, 0, true/*xor*/ );

        tool.MoveTo( c000.x, c000.y );
        tool.LineTo( (int)(0.5*(pdx.x-pd0.x)+c000.x), (int)(0.5*(pdx.y-pd0.y)+c000.y) );

        tool.RestorePen();

        tool.SelectPen( 1, 0, b, 0, true/*xor*/ );

        tool.MoveTo( c000.x, c000.y );
        tool.LineTo( (int)(0.5*(pdy.x-pd0.x)+c000.x), (int)(0.5*(pdy.y-pd0.y)+c000.y) );

        tool.RestorePen();

        tool.SelectPen( 1, 0, 0, b, true/*xor*/ );

        tool.MoveTo( c000.x, c000.y );
        tool.LineTo( (int)(0.5*(pdz.x-pd0.x)+c000.x), (int)(0.5*(pdz.y-pd0.y)+c000.y) );

        tool.RestorePen();
      }


      tool.SelectPen( 1, 0, 0, 0, true/*xor*/ );

      // Отрисовка обозначения координатных осей
      pd0 = pdx; pd0.x += k+m; pd0.y += k+x;
      if ( ::IsCoordGood( pd0, nullptr ) ) {
        tool.MoveTo( pd0.x,   pd0.y );
        tool.LineTo( pd0.x-m, pd0.y-x );
        tool.MoveTo( pd0.x-m, pd0.y+x-1 );
        tool.LineTo( pd0.x+m, pd0.y-x );
        tool.MoveTo( pd0.x,   pd0.y );
        tool.LineTo( pd0.x+m, pd0.y+x );
      }

      pd0 = pdy; pd0.x += k+m; pd0.y += k+x;
      if ( ::IsCoordGood( pd0, nullptr ) ) {
        tool.MoveTo( pd0.x,   pd0.y );
        tool.LineTo( pd0.x-m, pd0.y-x );
        tool.MoveTo( pd0.x,   pd0.y );
        tool.LineTo( pd0.x+m, pd0.y-x );
        tool.MoveTo( pd0.x,   pd0.y );
        tool.LineTo( pd0.x,   pd0.y+x );
      }

      pd0 = pdz;
      if ( ::IsCoordGood( pd0, nullptr ) ) {
        tool.MoveTo( pd0.x+k+1, pd0.y+k+1 );
        tool.LineTo( pd0.x+k+x, pd0.y+k+1 );
        tool.LineTo( pd0.x+k,   pd0.y+k+y-1 );
        tool.LineTo( pd0.x+k+x, pd0.y+k+y-1 );
      }

      tool.RestorePen();
    }
  }
}

//------------------------------------------------------------------------------
// Послать сообщение о выводе координат и масштаба
// ---
void WorkWindow::PutCoordinates( POINT p )
{
  //WorkWindow * v = viewManager->GetActiveWindow();
  //if ( v ) {
  //  cursorPoint = v->GetCartPoint( p );
  //  SetStatusBarPoint( cursorPoint );
  //}
  TestVariables::cursorPoint = GetCartPoint( p );
  ::SetStatusBarPoint( TestVariables::cursorPoint );
}


//------------------------------------------------------------------------------
// Нарисовать прямоугольник
// ---
void WorkWindow::PutRectangleMap( ptrdiff_t index, POINT& p1, POINT& p2 )
{
  if ( !enableDrawRect )
    return;

  WorkWindow * w = TestVariables::viewManager->GetWindow( index );
  if ( w ) {
    if ( renderType == wwrt_GDI )
      w->GetDrawTool().PutRectangle( p1, p2, RGB(126, 126, 126), true/*xor*/ );
    else if ( renderType == wwrt_OGL ) {
      MbCartPoint3D pnt1, pnt2, pnt3, pnt4;
      pnt1 = GetCartPoint( p1 );
      pnt3 = GetCartPoint( p2 );
      POINT p;
      p.x = p1.x;
      p.y = p2.y;
      pnt2 = GetCartPoint( p );
      p.x = p2.x;
      p.y = p1.y;
      pnt4 = GetCartPoint( p );

      SPtr<MbContour3D> contour( new MbContour3D );
      contour->SegmentsReserve( 4 );
      contour->SegmentsAdd( *new MbLineSegment3D( pnt1, pnt2 ) );
      contour->SegmentsAdd( *new MbLineSegment3D( pnt2, pnt3 ) );
      contour->SegmentsAdd( *new MbLineSegment3D( pnt3, pnt4 ) );
      contour->SegmentsAdd( *new MbLineSegment3D( pnt4, pnt1 ) );
      contour->CheckClosed( Math::metricPrecision );
      SPtr<MbMesh> frameMesh( new MbMesh );
      MbStepData stepData( ist_SpaceStep, Math::visualSag );
      MbFormNote note(true, false, true, TestVariables::exactMesh);
      contour->CalculateMesh( stepData, note, *frameMesh );
      frameMesh->SetColor( RGB(126, 126, 126) );
      std::vector< SPtr<MbItem> > addItems;
      addItems.push_back( frameMesh );

      w->GetDrawTool().ShowOGL( drawModel, select, &addItems );
    }
#ifdef __USE_QT__
    InvalidateWindow( true );
#endif // __USE_QT__
  }
}

//------------------------------------------------------------------------------
// Выделить объект или связь
// ---
void WorkWindow::LeftBottonDown( POINT & p )
{
  MbItem * nearest = nullptr;

  TestVariables::cursorFlag = -1;
  TestVariables::cursorPoint0 = p;
  TestVariables::cursorPoint1 = p;
  MbFloatAxis3D cAxis( TestVariables::viewManager->GetActiveWindow()->GetCartPoint( p ), TestVariables::viewManager->GetActiveWindow()->GetAxisZ() );
  PathMatrix pathFrom;

  if ( ::isShiftPressed() ) {
    MbTopItem * findGeom = nullptr;
    if ( FindNearestItem( st_Solid, tt_Face, cAxis, SENSE_RADIUS, &nearest, &findGeom, pathFrom.second ) ) {
      if ( TestVariables::viewManager->GetCplaneObject()==nearest && TestVariables::viewManager->GetCplaneItem()==findGeom )
        TestVariables::viewManager->DeleteCplane();
      else
        TestVariables::viewManager->InitCplane(nearest, findGeom, pathFrom.second, 0.0);
    }
  }
  else {
    if ( TestVariables::viewManager->FindNearestTemporal( cAxis, SENSE_RADIUS, TestVariables::_tindex, TestVariables::_rindex ) ) {
      TestVariables::rememPoint = TestVariables::viewManager->GetActiveWindow()->GetCartPoint( p );
      TestVariables::moveVector.SetZero();
      TestVariables::tflag = 1;
    }
    else {
      TestVariables::tflag =-1;

      MbRefItem * geom = nullptr;
      if ( FindGeometricItem( st_SpaceItem, tt_Undefined, pt_PlaneItem, cAxis, SENSE_RADIUS, &nearest, &geom, pathFrom ) ) {
        if ( nearest != nullptr ) {
          if ( nearest == TestVariables::viewManager->GetObjectTarget() ) {
            TestVariables::_tindex = 0;
            TestVariables::rememPoint = TestVariables::viewManager->GetActiveWindow()->GetCartPoint( p );
            TestVariables::moveVector.SetZero();
            TestVariables::tflag = 2;
          }
          else {
            //TestVariables::viewManager->ToggleObject( nearest );
            TestVariables::viewManager->ToggleObject( pathFrom.first );
          }
        }
      }
      else {
        nearest = TestVariables::viewManager->GetObjectTarget();
        ::AddRefItem( nearest );

        TestVariables::viewManager->DeleteTemporal();
        if ( nearest != nullptr && nearest->IsSelected() )
          TestVariables::viewManager->ToggleObject( nearest );

        ::ReleaseItem( nearest );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Выдать свойства или параметры связи
// ---
void WorkWindow::RightBottonDown( POINT & p, bool shift )
{
  MbItem * nearest = nullptr;
  WorkWindow* wnd = TestVariables::viewManager->GetActiveWindow();
  PRECONDITION( wnd );
  MbFloatAxis3D cAxis( wnd->GetCartPoint( p ), wnd->GetAxisZ() );
  PathMatrix pathFrom;

  if ( shift ) 
  {
    MbTopItem * item = nullptr;
    if ( FindNearestItem( st_Solid, tt_TopItem, cAxis, SENSE_RADIUS, &nearest, &item, pathFrom.second ) && (item != nullptr) )
    {      
      switch ( item->IsA() ) 
      {
        case tt_Vertex : // Вершина.
          TestVariables::viewManager->CreateProperty( static_cast<MbVertex*>(item) );
        break;
        case tt_Edge :   // Ребро, проходящее по кривой.
          TestVariables::viewManager->CreateProperty( static_cast<MbEdge*>(item) );
        break;
        case tt_CurveEdge : // Ребро, проходящее по кривой пересечения поверхностей.
          TestVariables::viewManager->CreateProperty( static_cast<MbCurveEdge*>(item) );
        break;
        case tt_OrientedEdge : // Ориентированное ребро.
          TestVariables::viewManager->CreateProperty( static_cast<MbOrientedEdge*>(item) );
        break;
        case tt_Loop :      // Цикл.
          TestVariables::viewManager->CreateProperty( static_cast<MbLoop*>(item) );
        break;
        case tt_Face :      // Грань.
          TestVariables::viewManager->CreateProperty( static_cast<MbFace*>(item) );
        break;
        case tt_FaceShell :     // Связное множество граней.
          TestVariables::viewManager->CreateProperty( static_cast<MbFaceShell*>(item) );
        break;
        default: break;
      }
      return;
    }
  }

  MbRefItem * geom = nullptr;
  FindGeometricItem( st_SpaceItem, tt_Undefined, pt_PlaneItem, cAxis, SENSE_RADIUS, &nearest, &geom, pathFrom );
  {
    if ( nearest != nullptr )
      TestVariables::viewManager->CreateProperty( nearest );
    else
      TestVariables::viewManager->CreateProperty( &geomModel );
  }
}


//------------------------------------------------------------------------------
// Реакция на нажатие клавиатуры
// ---
bool WorkWindow::KeyboardCommand( uint key, bool ctrl, bool shift )
{
  RECT rc;

  switch ( key )
  {
    case VK_TAB :
      LeftBottonDown( TestVariables::posit );

      if ( ctrl ) { // если Ctrl+Tab, то переключить на следующее окно
        ptrdiff_t pIndex = TestVariables::viewManager->GetActiveWindowIndex();

        if ( pIndex != -1 ) {
          ptrdiff_t cnt = TestVariables::viewManager->GetWindowsCount();
          ptrdiff_t ind = TestVariables::viewManager->GetActiveWindowIndex();
          ptrdiff_t nin = (ind+1) % cnt;
          TestVariables::viewManager->SetFocus( nin, true );
        }
      }
      break;

    case VK_INSERT :
      RightBottonDown( TestVariables::posit, shift );
      break;

    case VK_DELETE :
       
      DeleteSelected();
      break;

    case VK_UP :
      if ( ctrl ) // Толко для 16 разрядного случая
        RotateWindow( 1, -TestVariables::delta_angle ); // Повернуть окно
      else {
        if ( shift )
          MoveWindow( 3, -TestVariables::delta_range ); // Сдвинуть окно
        else
          MoveWindow( 2,  TestVariables::delta_range ); // Сдвинуть окно
      }
      break;

    case VK_DOWN :
      if ( ctrl )
        RotateWindow( 1, TestVariables::delta_angle ); // Повернуть окно
      else {
        if ( shift )
          MoveWindow( 3,  TestVariables::delta_range ); // Сдвинуть окно
        else
          MoveWindow( 2, -TestVariables::delta_range ); // Сдвинуть окно
      }
      break;

    case VK_LEFT :
      if ( ctrl )
        RotateWindow( 2, -TestVariables::delta_angle ); // Повернуть окно
      else {
        if ( shift )
          RotateWindow( 3, TestVariables::delta_angle ); // Повернуть окно
        else
          MoveWindow( 1, -TestVariables::delta_range ); // Сдвинуть окно
      }
      break;

    case VK_RIGHT :
      if ( ctrl )
        RotateWindow( 2, TestVariables::delta_angle ); // Повернуть окно
      else {
        if ( shift )
          RotateWindow( 3, -TestVariables::delta_angle ); // Повернуть окно
        else
          MoveWindow( 1, TestVariables::delta_range ); // Сдвинуть окно
      }
      break;

    case VK_NEXT :
      if ( ctrl )
        RotateWindow( 1, -6*TestVariables::delta_angle ); // Повернуть окно
      else
        MoveWindow( 2, 10*TestVariables::delta_range ); // Сдвинуть окно
      break;

    case VK_PRIOR :
      if ( ctrl )
        RotateWindow( 1, 6*TestVariables::delta_angle ); // Повернуть окно
      else {
        GetDrawTool().GetClientRect( rc );
        MoveWindow( 2, -10*TestVariables::delta_range ); // Сдвинуть окно
      }
      break;

    case VK_END :
      if ( ctrl )
        RotateWindow( 2, -6*TestVariables::delta_angle ); // Повернуть окно
      else {
        GetDrawTool().GetClientRect( rc );
        MoveWindow( 1, -10*TestVariables::delta_range ); // Сдвинуть окно
      }
      break;

    case VK_HOME :
      if ( ctrl )
        RotateWindow( 2, 6*TestVariables::delta_angle ); // Повернуть окно
      else {
        GetDrawTool().GetClientRect( rc );
        MoveWindow( 1, 10*TestVariables::delta_range ); // Сдвинуть окно
      }
      break;

    case VK_F1 :        // Выход из программы
      if ( shift && ctrl ) {
        Quit();
      }
      break;

    case VK_F2 :
      if ( ctrl )
        ::WriteItem();    // Записать модель в файл
      else
        ::WriteModel( nullptr, GetCurrentMathFileVersion() );   // Записать модель в файл
      break;

    case VK_F3 :
      if ( ctrl )
        ::ReadItem();        // Прочитать модель из файла
      else
        ::ReadModel( nullptr ); // Прочитать модель из файла
      break;

    case VK_F4 : {     // Тонировать OpenGL в активном окне
      if ( shift ) {
        SetColorCmd(); // Изменить цвет элементов. Change objects color.
      }
      else {
        SetWaitCursor( true );
        if ( ctrl )
          Math::tempIndex++;

        RPArray<MbItem> objs( 0, 1 );
        TestVariables::viewManager->GetRenderingObjects( objs ); // Выдать указатели на все объекты
        ScanLine( objs ); // Раскрасить объекты

        SetWaitCursor( false );
      }
    } break;

    case VK_F5 : {     // Поменять активное окно
      if ( shift ) {
        SolidEndMoveCmd(); // Сдвинуть конец дерева построения. Move the end of the build log. 
      }
      else {
        WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();
        if ( wnd ) {
          if ( ctrl )
            SetOriginIntoCentre( true ); // Установка нуля в центр вида
          else
            wnd->GetDrawTool().ChangeWindowState();
        }
      }
    } break;

    case VK_F6 :  {    // Поменять активное окно
      if ( shift ) {
        SolidFinishCmd(); // Выполнить пропущенную олперацию. Build an omitted operation
      }
      else {
        ptrdiff_t pIndex = TestVariables::viewManager->GetActiveWindowIndex();

        if ( pIndex != -1 ) {
          ptrdiff_t cnt = TestVariables::viewManager->GetWindowsCount();
          ptrdiff_t nin, ind = TestVariables::viewManager->GetActiveWindowIndex();
          if ( ctrl )
            nin = (ind-1+cnt) % cnt;
          else
            nin = (ind+1) % cnt;

          TestVariables::viewManager->SetFocus( nin, true );
        }
      }
    } break;

    case VK_F7 : {      // Повторить предыдущую команду
      if ( ctrl ) {
        SetWaitCursor( true );
        TestVariables::viewManager->RefreshModel();
        SetWaitCursor( false );
      }
      else if ( shift ) {
        RebuildShellCmd(); // Перестроить тело. Rebuild solid or shell.
      }
      else if ( TestVariables::lPrev == 0 ) {
        RepeatPrevCmd();
      }

    } break;

    case VK_F8 : {     // Тонировать в активном окне
      SetWaitCursor( true );

      if ( ctrl )
        ChangeRenderTypeOGL();
      else
      if ( shift ) {
        EraseModel();
        ChangeViewSort( cutModel == nullptr );
        ShowModel();
        //RPArray<MbItem> objs( 0, 1 );
        //TestVariables::viewManager->GetRenderingObjects( objs ); // Выдать указатели на все объекты
        //ScanLine( objs ); // Раскрасить объекты
      }
      else {
        bool convexHull = false;
        if ( isCapitalPressed() )
          convexHull = true;
        ChangeRenderType( convexHull );
      }

      SetWaitCursor( false );
    } break;

    case VK_F9 : {     // Перерисорвать
      SetWaitCursor( true );
      if ( ctrl )
        SetVisualSag();
      else
      if ( shift ) {
        TurnOverMapping();
      }
      TestVariables::viewManager->EvaluateConstraints();
      TestVariables::viewManager->RefreshModel();
      SetWaitCursor( false );
      break;
    }

    case VK_F10 : {    // Решить параметрическую систему и перерисорвать
      SetWaitCursor( true );
      PRECONDITION( false );
      SetWaitCursor( false );
      break;
    }

    case VK_F11 : {    // Перспективная проекция
      double d = TestVariables::viewManager->GetActiveWindow()->GetVista().z;
      if (d > DELTA_MIN)
        TestVariables::viewManager->SetViewPoint( 0 );
      else {
        d = 333;
        if ( GetDouble( IDS_ENTER_DISTANCE, d ) )
          TestVariables::viewManager->SetViewPoint( d );
      }
      break;
    }

    case VK_BACK : {   // Отменить предыдущую команду
      // Запомненный объект и его элемент
      TestVariables::ResetHLighted();
      ActionType at = UndoAction(); // Тип восстановления
      if ( at == at_AddType ) {
        if ( GetBoolean( IDS_DELETE_LASTITEM_BOOL ) )
          Undo();
        else
          UndoSkeep( at );
      }
      else
        Undo();
      break;
    }

    //case 72: { //'h' // empty;
    //  break;
    //}
    //case 73: { //'i' // empty;
    //  break;
    //}
    //case 0x47: { // 'g' // empty;
    //  break;
    //} 

    case 0x4f:{ // 'o'  
      if ( ctrl ) // Открыть файл
        ::ReadModel( nullptr ); 
      break;
    }

    case 83:{ // 's'
      if ( ctrl ) // Сохранить файл.
        ::WriteModel( nullptr, GetCurrentMathFileVersion() );
      break;
    }

    case VK_MULTIPLY :  // Центрировать начало координат
      SetOriginIntoCentre( ctrl );
      break;

    case VK_SUBTRACT :  // Уменьшить масштаб
      ReduceScale();
      break;

    case VK_ADD :       // Увеличить масштаб
      EnlargeScale();
      break;

    case VK_A : {
       if ( isCtrlPressed() ) { // Ctrl+A - select all.
         SelectModel();
       }
      }
      break;

    default :
      return false; // Не обработано
  }

  return true;
}


//----------------------------------------------------------------------------------------
// Реакция на нажатие клавиатуры в режиме указания точки
// ---
bool WorkWindow::KeyboardCommandFindPoint( uint key, bool ctrl, bool shift, const POINT & p )
{
  switch ( key )
  {
    case VK_RETURN :
    {
      //POINT p;
      //GetCursorPos( &p );
      TestVariables::cursorFlag = -1;
      TestVariables::cursorPoint0 = p;
      TestVariables::cursorPoint1 = p;
      TestVariables::cursorPoint = /*viewManager->GetActiveWindow()->*/ GetCartPoint( p );
      TestVariables::cursorAxis.Init( TestVariables::cursorPoint, /*viewManager->GetActiveWindow()->*/ GetAxisZ() );
      TestVariables::mappingPoint = p;//viewManager->GetActiveWindow()->GetMapPoint( p );
      if ( shift )
        TestVariables::getPointResult = PNT_RES_SHIFT;
      else if ( ctrl )
        TestVariables::getPointResult = PNT_RES_CTRL;   //-V112
      else
        TestVariables::getPointResult = PNT_RES_USUAL;
    } break;

    case VK_ESCAPE :
    {
      TestVariables::getPointResult = PNT_RES_ESC;
    }  break;

    case VK_TAB : // Точный ввод координат
    {
      if ( GetExactPoint( TestVariables::cursorPoint ) )
        TestVariables::getPointResult = PNT_RES_USUAL;
    } break;

    default :
      return false; // Не обработано
  }

  return true;
}

//------------------------------------------------------------------------------
// Обработать событие от мыши
// ---
bool WorkWindow::MouseCommand( uint command, POINT & p, bool shiftDown, int paramCount )
{
  switch ( command )
  {
    case WM_LBUTTONDOWN : { // Выделение элементов при нажатии левой кнопки мыши
      if ( shiftDown )
      {
        TestVariables::posit = p;
        WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();
        if ( wnd ) {
          wnd->mouseRotation = true;
          wnd->lastBase = p;
          //TestVariables::tflag = -1;
          return true;
        }
      } // Вращение
      else
        LeftBottonDown( p );
      TestVariables::posit = p;
    } break;

    case WM_LBUTTONUP : { // Сдвиг объекта или связи по суммарному сдвигу временных точек
      POINT mp = p;

      TestVariables::posit = p;
      if ( TestVariables::_tindex > -1 ) {
        MbFloatAxis3D cAxis( TestVariables::viewManager->GetActiveWindow()->GetCartPoint( mp ), TestVariables::viewManager->GetActiveWindow()->GetAxisZ() );
        if ( TestVariables::tflag == 1 && TestVariables::viewManager->FindNearestTemporal( cAxis, SENSE_RADIUS, TestVariables::_tindex, TestVariables::_rindex ) )
          TestVariables::viewManager->MoveTemporal( TestVariables::_tindex, TestVariables::moveVector, true );
        else
        if ( TestVariables::tflag == 2 )
          TestVariables::viewManager->MoveTemporal( TestVariables::moveVector, true );
      }

      TestVariables::tflag = -1;

      if ( TestVariables::cursorFlag > 0 ) {
        PutRectangleMap( TestVariables::viewManager->GetActiveWindowIndex(), TestVariables::cursorPoint0, TestVariables::cursorPoint1 );
        TestVariables::cursorFlag = -1;
        if ( ::abs_t( TestVariables::cursorPoint0.x - TestVariables::cursorPoint1.x ) > 2 &&
             ::abs_t( TestVariables::cursorPoint0.y - TestVariables::cursorPoint1.y ) > 2 ) {
          if ( TestVariables::plotFlag == 1 ) {
            PutVect( TestVariables::cursorPoint0, TestVariables::cursorPoint1 ); // Векторный вывод в растр
            TestVariables::plotFlag = -1;
          }
          else
          if ( TestVariables::rastrFlag == 1 ) {
            PutRastr( TestVariables::cursorPoint0, TestVariables::cursorPoint1 );
            TestVariables::rastrFlag = -1;
          }
          else
            InverseInRect( TestVariables::cursorPoint0, TestVariables::cursorPoint1, select );

          TestVariables::cursorPoint0 = TestVariables::cursorPoint1;
        }
        else {
          TestVariables::plotFlag = -1;
          TestVariables::rastrFlag = -1;
        }
      }
      TestVariables::cursorPoint0 = p;
      TestVariables::cursorPoint1 = p;
    } break;

    case WM_LBUTTONDBLCLK : { // Создание временных точек
      MbItem * nearest = nullptr;
      POINT mp = p;
      MbFloatAxis3D cAxis( TestVariables::viewManager->GetActiveWindow()->GetCartPoint( mp ), TestVariables::viewManager->GetActiveWindow()->GetAxisZ() );

      TestVariables::posit = p;
      if ( shiftDown ) {
        MbTopItem * findGeom = nullptr;
        MbMatrix3D matr;
        if ( FindNearestItem( st_Solid, tt_Face, cAxis, SENSE_RADIUS, &nearest, &findGeom, matr ) ) {
          if ( TestVariables::viewManager->GetCplaneObject() == nearest && TestVariables::viewManager->GetCplaneItem() == findGeom )
            OrientateWindow( TestVariables::viewManager->GetCplane() );
          else
            TestVariables::viewManager->InitCplane(nearest, findGeom, matr, 0.0);
        }
      }
      else {
        ptrdiff_t ix, n;
        if ( TestVariables::viewManager->FindNearestTemporal( cAxis, SENSE_RADIUS, ix, n ) ) {
          MbCartPoint3D p3d;
          MbCartPoint   p2d;
          ptrdiff_t np;
          np = TestVariables::viewManager->GetTemporalPoint( ix, n, p3d, p2d ); // Дать точку
          if ( np == 3 ) {
            if ( GetExactPoint( p3d ) )
              TestVariables::viewManager->SetTemporalPoint( ix, n, np, p3d, p2d ); // Установить точку
          }
          else
          if ( np == 2 ) {
            if ( GetExactPlanePoint( p2d ) )
              TestVariables::viewManager->SetTemporalPoint( ix, n, np, p3d, p2d ); // Установить точку
          }
        }
        else {
          nearest = TestVariables::viewManager->GetObjectTarget();
          TestVariables::viewManager->DeleteTemporal();
          if ( nearest!=nullptr && nearest->IsSelected() )
            TestVariables::viewManager->ToggleObject( nearest );
          MbRefItem * geom = nullptr;
          PathMatrix pathFrom;

          if ( FindGeometricItem( st_SpaceItem, tt_Undefined, pt_PlaneItem, cAxis, SENSE_RADIUS, &nearest, &geom, pathFrom ) ) {
            if ( nearest!=nullptr ) {
              if ( !nearest->IsSelected() )
                TestVariables::viewManager->SelectObject( nearest );
              TestVariables::viewManager->InitTemporal( nearest, pathFrom.second );
            }
          }
        }
      }
    } break;

    case WM_RBUTTONDOWN : { // Выдать свойства или значение временной связи
      // Создать окно свойств
      TestVariables::posit = p;
      RightBottonDown( p, shiftDown );
    } break;

    case WM_MBUTTONDOWN : {
      TestVariables::posit = p;
      WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();
      if ( wnd ) {
        wnd->mouseRotation = true;
        wnd->lastBase = p;
      }
    } break;

    case WM_MBUTTONUP : {
      TestVariables::posit = p;
      WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();
      if ( wnd ) {
        wnd->mouseRotation = false;
        wnd->lastBase = p;
      }
    } break;

    case 0x020A /*WM_MOUSEWHEEL*/ : { // Пересчет координат и сдвиг временных точек
      if ( paramCount < 0 )
        ReduceScale( TestVariables::cursorPoint );
      else
        EnlargeScale( TestVariables::cursorPoint );
    } break;

    default :
      return false; // Не обработанно
  }

  return true;
}

//------------------------------------------------------------------------------
// Обработать событие от мыши в режиме указания точки
// ---
bool WorkWindow::MouseCommandFindPoint( uint command, POINT & p, bool ctrl, bool shift )
{
  switch ( command )
  {
    case WM_LBUTTONDOWN :
    {
      //POINT p = { LOWORD(lParam), HIWORD(lParam) };
      TestVariables::cursorFlag = -1;
      TestVariables::cursorPoint0 = p;
      TestVariables::cursorPoint1 = p;
      TestVariables::cursorPoint = /*viewManager->GetActiveWindow()->*/ GetCartPoint( p );
      TestVariables::cursorAxis.Init( TestVariables::cursorPoint, /*viewManager->GetActiveWindow()->*/ GetAxisZ() );
      TestVariables::mappingPoint = p;//viewManager->GetActiveWindow()->GetMapPoint( p );
      if ( shift )
        TestVariables::getPointResult = PNT_RES_SHIFT;
      else if ( ctrl )
        TestVariables::getPointResult = PNT_RES_CTRL;   //-V112
      else
        TestVariables::getPointResult = PNT_RES_USUAL;
    } break;

    case WM_RBUTTONDOWN :
    {
      TestVariables::getPointResult = PNT_RES_ESC;
    } break;

    default :
      return false; // Не обработано
  }

  return true;
}


//------------------------------------------------------------------------------
// Удалить объект из массива выбранных элементов.
// ---
void WorkWindow::DeleteObjectFromSelectedList( const MbItem * obj, const MbRefItem * part )
{
  for ( std::vector<PairObjectParts>::iterator it = vecSelElements.begin();
        it != vecSelElements.end(); it++ ) {
    if ( it->first == obj && ((part==nullptr) || (part == it->second)) ) {
      const_cast<MbItem *>( &(*it->first) )->SetSelected( false );
      vecSelElements.erase( it );
      break;
    }
  }

  PutObjectsParts();
}


//------------------------------------------------------------------------------
// СМЕЩЕНИЕ МОДЕЛИ В ОКНЕ
// Нажата (и удерживается) средняя кнопка мыши либо Ctrl и левая кнопка
// ---
static void MoveWithMouse( const POINT & p, WorkWindow & )
{
  WorkWindow* wnd = TestVariables::viewManager->GetActiveWindow();
  if ( wnd )
  {
    POINT dif;

    dif.x = p.x - wnd->lastBase.x;
    dif.y = p.y - wnd->lastBase.y;

    if ( dif.y != 0 )
      wnd->MoveWindow( 2, -dif.y / wnd->GetDrawTool().GetYPixelPerMM() ); // Вертикально
    if ( dif.x != 0 )
      wnd->MoveWindow( 1, dif.x / wnd->GetDrawTool().GetXPixelPerMM() ); // Горизонтально

    wnd->lastBase = p;
  }
}


//------------------------------------------------------------------------------
// ВРАЩЕНИЕ МОДЕЛИ В ОКНЕ
// Нажата (и удерживается) средняя кнопка мыши либо Shoft и левая кнопка
// ---
static void RotateWithMouse( const POINT & p, WorkWindow & wkWindow )
{
  // ВРАЩЕНИЕ МОДЕЛИ В ОКНЕ
  // Нажата (и удерживается) средняя кнопка мыши
  WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();
  if ( wnd ) {
    int32 xDif = p.x - wnd->lastBase.x;
    int32 yDif = p.y - wnd->lastBase.y;

    if ( yDif != 0 )
      wkWindow.RotateWindow( 1, yDif*TestVariables::delta_angle/10. ); // Вертикально
    if ( xDif != 0 )
      wkWindow.RotateWindow( 2, xDif*TestVariables::delta_angle/10. ); // Горизонтально

    wnd->lastBase = p;
  }
}


//------------------------------------------------------------------------------
//
// ---
bool WorkWindow::MouseMoveCommand( POINT & p, bool shift, bool ctrl, bool lbtn, bool mbtn )
{
  //PutCoordinates( p ); - Перенесено наружу метода

  if ( ::abs_t( TestVariables::posit.x - p.x) < 1 && ::abs_t( TestVariables::posit.y - p.y) < 1 )
    return false;
  
  const POINT posit0( TestVariables::posit );
  TestVariables::posit = p;
  if ( lbtn ) {
    if ( TestVariables::tflag > 0 ) {
      TestVariables::moveVector.Init( TestVariables::rememPoint, TestVariables::cursorPoint );
      if ( TestVariables::tflag == 1 )
        TestVariables::viewManager->MoveTemporal( TestVariables::_tindex, TestVariables::moveVector, false );
      else
        TestVariables::viewManager->MoveTemporal( TestVariables::moveVector, false );
      TestVariables::rememPoint = TestVariables::cursorPoint;
    }
    else {
      if ( TestVariables::cursorFlag > 0 ) {
        PutRectangleMap( TestVariables::viewManager->GetActiveWindowIndex(), TestVariables::cursorPoint0, TestVariables::cursorPoint1 );
        TestVariables::cursorPoint1 = p;
        PutRectangleMap( TestVariables::viewManager->GetActiveWindowIndex(), TestVariables::cursorPoint0, TestVariables::cursorPoint1 );
      }
      else {
        if (shift)
        {
          RotateWithMouse(p, *this);
        } 
        else if ( ctrl ) 
        {
          MoveWithMouse( p, *this );
        }
        else
        {
          TestVariables::cursorFlag = 1;
          TestVariables::cursorPoint1 = p;
          PutRectangleMap( TestVariables::viewManager->GetActiveWindowIndex(), TestVariables::cursorPoint0, TestVariables::cursorPoint1 );
        }
      }
    }
  }
  else if ( mbtn )
  {
    if ( shift )
    {
      // СМЕЩЕНИЕ МОДЕЛИ В ОКНЕ
      // Нажаты (и удерживаются) средняя кнопка мыши и клавша SHIFT
      MoveWithMouse( p, *this );
    }
    else
    {
      // ВРАЩЕНИЕ МОДЕЛИ В ОКНЕ
      // Нажата (и удерживается) средняя кнопка мыши
      RotateWithMouse(p, *this);
    }

  }
  else {
    // ПОДСВЕТКА объекта в окне
    POINT mp = p;
    MbItem * nearest = nullptr;
    MbRefItem * item = nullptr;
    MbFloatAxis3D cAxis( TestVariables::viewManager->GetActiveWindow()->GetCartPoint( mp ), TestVariables::viewManager->GetActiveWindow()->GetAxisZ() );
    PathMatrix pathFrom;

    // не реагируем на перемещение в области размера ловушки, т.к. тормозит на моделях с большим количеством элементов
    int senseDelta = SENSE_RADIUS / 2;
    if ( posit0.x - senseDelta < p.x && p.x < posit0.x + senseDelta ) {
      if ( posit0.y - senseDelta < p.y && p.y < posit0.y + senseDelta ) {
        TestVariables::posit = posit0;
        return false;
      }
    }

    PutObjectsParts();

    const ItemElement nullElem;

    if ( FindGeometricItem(st_SpaceItem, tt_Undefined, pt_PlaneItem, cAxis, SENSE_RADIUS, &nearest, &item, pathFrom) ) {
      const ItemElement nearElem( pathFrom.first, SPtr<MbRefItem>(item) );
      C3D_ASSERT( nullElem != nearElem );
      
      if ( shift ) {
        // Подстветка детали объекта (т.е. с примитивом объекта)
        if ( TestVariables::hLightedItem == nullElem ) {
          TestVariables::hLightedObj = nearest;
          TestVariables::hLightedItem = nearElem;
          if ( !nearest->IsSelected() )
            PutObjectPart( TestVariables::hLightedItem, true );
        }
        else if ( TestVariables::hLightedItem != nearElem ) {
          if ( !TestVariables::hLightedObj->IsSelected() )
            PutObject( TestVariables::hLightedItem.first, false, true );
          TestVariables::hLightedObj = nearest;
          TestVariables::hLightedItem = nearElem;
          if ( !nearest->IsSelected() )
            PutObjectPart( TestVariables::hLightedItem, true );
        }
      }
      else { 
        // Подсветка объекта модели целиком
        if ( TestVariables::hLightedItem == nullElem ) {
          TestVariables::hLightedObj = nearest;
          TestVariables::hLightedItem = nearElem;
          if ( !nearest->IsSelected() )
            PutObject( TestVariables::hLightedItem.first, true );
        }
        else if ( TestVariables::hLightedItem.first != pathFrom.first ) {
          if ( !TestVariables::hLightedObj->IsSelected() )
            PutObject( TestVariables::hLightedItem.first, false );

          TestVariables::hLightedObj = nearest;
          TestVariables::hLightedItem = nearElem;
             
          if ( !nearest->IsSelected() )
            PutObject( TestVariables::hLightedItem.first, true );
        }
      }
    }
    else if ( TestVariables::hLightedItem != nullElem ) {
      /*
        Погасить. Turn off the highlight.
      */
      if ( !TestVariables::hLightedObj->IsSelected() )
        PutObject( TestVariables::hLightedItem.first, false );
      TestVariables::ResetHLighted();       
      TestVariables::hLightedItem = nullElem;
    }
  }

  return true;
}

#undef MAXMAP
#undef MAX_SCALE
#undef MIN_SCALE
