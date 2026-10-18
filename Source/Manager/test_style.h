#ifndef __TEST_STYLE_H
#define __TEST_STYLE_H


#include <math_define.h>
#include <mb_variables.h>
#if defined(C3D_WINDOWS)
  #include <windows.h>
#endif


#define MAXNAME           8
#define TYPENAMELENGTH   64


//------------------------------------------------------------------------------
// Способы отображения OpenGL. Modes of drawing by OpenGL.
// ---
enum OGLViewMode {
  ovm_NormalRender          = 0, // Нормальный - "Класический полутон"
  ovm_NormalRenderWithEdges = 1, // Нормальный - "Класический полутон" с ребрами
  ovm_PrimitiveRender       = 2, // Цветные треугольники и квадраты (полосы разбираются)
  ovm_StripePrimRender      = 3, // Цветные полосы треугольники и квадраты (+ одиночные примитивы)
  ovm_StripesRender         = 4, // Только полосы - без одиночных примитивов
  ovm_IsoparamMesh          = 5, // Поверхности - изопараметрическими линиями
  ovm_TriangleMesh          = 6, // Поверхности - триангуляционными решетками (примитивами - квадраты+треугольники)
  ovm_IsoparTriMesh         = 7, // Поверхности - изопараметрическими линиями + триангуляция (примитивами)
  ovm_ParamTriangleMesh     = 8, // Поверхность - триангуляция в параметрической области
  ovm_LastViewType          = 9,   
};


//------------------------------------------------------------------------------
// Объекты для отображения. The objects for drawing.
// ---
enum WorkViewSort {
  wvs_All  = 0,  // All entity.
  wvs_Geometry = 1, // Geometric items only.
  wvs_Constraints = 2, // Geometric constraints only.
};


//------------------------------------------------------------------------------
// Используемый инструмент для отображения. Drawing tool.
// ---
enum WorkRenderType {
  wwrt_GDI  = 0, // WindowsGDI
  wwrt_OGL  = 1, // OpenGL
  wwrt_RED  = 2, // Использование RedSDK
};


//------------------------------------------------------------------------------
// Стиль элемента
// ---
class Style {

protected :
  uint32 color; // Цвет элемента
  int    width; // Толщина линии
  int    style; // Стиль линии

public:
  Style ( int w = 1, uint32 c = MB_C3DCOLOR ) {
    style = 0;
    width = w;
    color = c;
  }
  Style ( const Style & init ) {
    style = init.style;
    width = init.width;
    color = init.color;
  }
  virtual ~Style () {}

public:
  // Проверка на равенство
  bool          operator == ( const Style & with ) const { return (style == with.style && width == with.width && color == with.color); }
  // Проверка на неравенство
  bool          operator != ( const Style & with ) const { return !( *this == with ); }
  // Присвоение значений
  void          operator = ( const Style & init ) { Set( init ); }
  void          Set( const Style & init ) { style = init.style; width = init.width; color = init.color; }
  void          Set( int s, int w, uint32 c ) { style = s; width = w; color = c; }
  void          SetColor( uint32 c ) { color = c; }
  void          SetWidth( int w ) { width = w; }
  void          SetStyle( int s ) { style = s; }
  // Получение значений
  uint32        GetColor() const { return color; }
  int           GetWidth() const { return width; }
  int           GetStyle() const { return style; }
  // Выдача на модификация
  uint32      & SetColor() { return color; }
  int         & SetWidth() { return width; }
  int         & SetStyle() { return style; }
  void          Init( int w = 1, uint32 c = 0 ) { width = w; color = c; }

}; // Style


//------------------------------------------------------------------------------
// Структуры, определенные в windows.h
//
#ifndef C3D_WINDOWS // _MSC_VER
typedef struct tagPOINT
  {
    int32 x;
    int32 y;
  } POINT;
  
  
typedef struct tagRECT
  {
    int32 left;
    int32 top;
    int32 right;
    int32 bottom;
  } RECT; 
  
  
#define VK_TAB         0x09
#define VK_INSERT      0x2D
#define VK_DELETE      0x2E
#define VK_UP          0x26
#define VK_DOWN        0x28
#define VK_LEFT        0x25
#define VK_RIGHT       0x27 
#define VK_NEXT        0x22
#define VK_PRIOR       0x21
#define VK_END         0x23
#define VK_HOME        0x24
#define VK_F1          0x70
#define VK_F2          0x71 
#define VK_F3          0x72 
#define VK_F4          0x73
#define VK_F5          0x74
#define VK_F6          0x75
#define VK_F7          0x76
#define VK_F8          0x77
#define VK_F9          0x78
#define VK_F10         0x79
#define VK_F11         0x7A
#define VK_F12         0x7B
#define VK_BACK        0x08
#define VK_MULTIPLY    0x6A
#define VK_SUBTRACT    0x6D
#define VK_ADD         0x6B
#define VK_RETURN      0x0D
#define VK_ESCAPE      0x1B
    
#define LBN_SELCHANGE  1  
#define LBN_DBLCLK     2
  
#define EN_UPDATE      0x0400
  
#define WM_LBUTTONDOWN     0x0201
#define WM_LBUTTONUP       0x0202
#define WM_LBUTTONDBLCLK   0x0203
#define WM_RBUTTONDOWN     0x0204
#define WM_RBUTTONUP       0x0205
#define WM_RBUTTONDBLCLK   0x0206
#define WM_MBUTTONDOWN     0x0207
#define WM_MBUTTONUP       0x0208
#define WM_MBUTTONDBLCLK   0x0209
  
#endif // _WIN32

#define VK_0               0x30
#define VK_1               0x31
#define VK_2               0x32
#define VK_3               0x33
#define VK_4               0x34
#define VK_5               0x35
#define VK_6               0x36
#define VK_7               0x37
#define VK_8               0x38
#define VK_9               0x39

#define VK_A               0x41
//...
#define VK_Z               0x5A


#endif // __TEST_STYLE_H
