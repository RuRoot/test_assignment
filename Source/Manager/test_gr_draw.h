#ifndef __TEST_GR_DRAW_H
#define __TEST_GR_DRAW_H


#include <test_style.h>
#include <mb_cube.h>
#include <mb_matrix3d.h>
#include <mb_placement3d.h>
#include <mesh.h>
#include <topology.h>
#include <model_entity.h>
#include <tool_cstring.h>


#define LOGIC_INCH     25.4     // Число миллиметров в дюйме


class    MbTopItem;
class    MbItem;
class    MbModel;
class    MbMesh;
class    MbVisual;
class              WorkWindow;


//------------------------------------------------------------------------------
// Инструмент рисования в конкретном окне. Связка с Windows API (GDI)
// A drawing tool in a specific window. Bundling with the Windows API (GDI)
// ---
class IGrDraw
{
protected:
  WorkWindow & parent;
  double       xPixelPerMM;    // Количество экранных единиц в мировой единице измерения: pixel_per_mm
  double       yPixelPerMM;    // Количество экранных единиц в мировой единице измерения: pixel_per_mm
  COLORREF     backgr;         // Цвет фона
  MbVector3D   lightLocal;     // Направление на источник света
  MbVector3D   lightSide;      // Направление на источник света
  float        lightAmbient;   // Общий фон среды
  float        lightDiffuse;   // Диффузия среды
  float        lightSpecular;  // Зеркальность среды
  bool         oglInitialized; // Инициализирована ли OGL?
  bool         oglModelReady;  // Передана ли модель в OGL
  OGLViewMode  renderMode;     // Режим отображения OGL

public:
  IGrDraw( WorkWindow & _parent, const MbVector3D & lt );
  virtual ~IGrDraw() {}

public:
  virtual bool    InitializeGL();                                                                                    // Инициализация для работы окна в полутоновом режиме
  virtual void    SetSizeGL( int cx, int cy, double scale );                                                         // Установить сцену
  virtual void    ShowOGL( const MbModel & drawModel, COLORREF select, std::vector< SPtr<MbItem> > * addItems = nullptr ) = 0; // Отрисовать через OpenGL
  virtual void    RefreshOGL() = 0;                                                                                  // Удалить временные данные (листы) OpenGL.
  virtual void    SetGLProperty( const MbModel & drawModel, double la, double ld, double ls, MbVector3D &ll );       // Визуальные свойства
  virtual void    DeleteGL( const MbModel & drawModel );                                                             // Удалить данные

          // Очистить OpenGL.
          void    ClearGL() const;
          // Загрузить OpenGL видовую матрицу преобразования.
          void    MatrixGL() const;
          // Подготовить данные.
          void    PrepareGL( const MbModel & drawModel, COLORREF select ) const;
          // Очистить OpenGL
          void    FinishGL() const;
          // Нарисовать полигональный объект.
          void    RenderMeshGL( const MbMesh * mesh, const MbMatrix3D & from, const MbAttributeContainer & hostVisual, 
                                COLORREF select, bool useSelect ) const;
          // Нарисовать объект.
          void    RenderItemGL( const MbItem * obj,  const MbMatrix3D & matr, const MbAttributeContainer & hostVisual, 
                                COLORREF select, bool useSelect ) const;
          // Перерисовать.
          void    ResetGL();
          // Визуальные свойства.
          void    GetGLProperty( double & la, double & ld, double & ls, MbVector3D & ll );
          // Выдать мировую точку по экранной.
          //MbCartPoint3D GetGLCartPoint( POINT mouseCoord );

          const MbVector3D & GetLight() const      { return lightLocal; }
          void    SetLight( const MbVector3D & v ) { lightLocal = v; lightSide = v; }

          OGLViewMode GetRenderMode() const      { return renderMode; }  // Получить режим отображения
          void    SetRenderMode( OGLViewMode m ) { renderMode = m; }     // Установить режим отображения

public:
  virtual void    SelectPen( int cWidth, int R, int G, int B, bool _xor = false ) = 0;
  virtual void    SelectPen( int cStyle, int cWidth, COLORREF c ) = 0;
  virtual void    RestorePen() = 0;

  virtual void    MoveTo  ( int x, int y ) = 0;
  virtual void    LineTo  ( int x, int y ) = 0;
  virtual void    SetPixel( int x, int y, int R, int G, int B ) = 0;
  virtual void    DPtoMM  ( const POINT & p, MbCartPoint & cp, double scale ) = 0;
  virtual void    MMtoDP  ( const MbCartPoint & cp, POINT & p, double scale ) = 0;

  virtual void    PutRectangle ( POINT & p1, POINT & p2, COLORREF, bool _xor ) = 0; // Нарисовать прямоугольник
  virtual void    FillRectangle( RECT * rc = nullptr ) = 0; // Залить прямоугольник цветом фона
  virtual void    GetClientRect( RECT & r ) = 0;

  virtual void    Invalidate( bool bErase = true ) = 0;
  virtual void    BeginPaint() = 0;
  virtual void    EndPaint() = 0;

  virtual void    SetWindowFocus( bool mdiActivate ) = 0;
  virtual void    SetWindowTitle( const TCHAR * ) = 0;
  virtual void    ChangeWindowState() = 0;
  virtual bool    IsMyWnd( const void * wndId ) const = 0;
  virtual void    UpdateWindow() {}

          double  GetXPixelPerMM() const       { return xPixelPerMM; }
          double  GetYPixelPerMM() const       { return yPixelPerMM; }
          double  GetZPixelPerMM() const       { return ( c3d::ONE_HALF * (xPixelPerMM + yPixelPerMM) ); }
          void    SetBackground( COLORREF b ) { backgr = b; }
          COLORREF GetBackground() const       { return backgr; }

private:
          void    operator = ( const IGrDraw & ); // Не реализовано
};

//------------------------------------------------------------------------------
// Определить цвет на основе атрибута MbVisual в контейнере атрибутов.
// ---
bool UpdateColorByVisualAttribute( const MbAttributeContainer & ac, COLORREF & toUpdate, bool useAlpha = true );


#endif // __TEST_GR_DRAW_H
