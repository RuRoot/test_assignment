////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Атрибуты. Цвет. Толщина линий отрисовки. Стиль линий отрисовки. Свойства для OpenGL.
         \en Attributes. Color. Thickness of drawing lines. Style of drawing lines. Properties for OpenGL. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ATTR_COLOR_H
#define __ATTR_COLOR_H


#include <attr_elementary_attribute.h>
#include <mb_variables.h>
#include <tool_uuid.h>
#include <array>

constexpr uint __RGB__ = 3;

//------------------------------------------------------------------------------
/** \brief \ru Значение цвета в цветовой модели RGBA.
           \en Color value in the RGBA color model. \~
  \details \ru Значение цвета в цветовой модели RGBA.
           \en Color value in the RGBA color model. \n \~
  \ingroup Model_Attributes
*/
// ---
class MATH_CLASS MbRGBA
{
private:
  std::array<uint8, 4>  _value;   ///< \ru Четыре  компонента (красный, зеленый, синий, альфа). \en Four components (red, green, blue, alpha). 

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbRGBA();

  /// \ru Конструктор по значениям компонентов. \en Constructor by components values.
  MbRGBA( uint8 red, uint8 green, uint8 blue, uint8 alpha = SYS_MAX_UINT8 );

  /// \ru Конструктор по uint32. \en Constructor by uint32.
  MbRGBA( uint32 );

  /// \ru Конструктор по значениям компонентов. \en Constructor by components values.
  MbRGBA( float red, float green, float blue, float alpha = 1.0 );

  /// \ru Конструктор копирования. \en Copy constructor.
  MbRGBA( const MbRGBA & other );

public:
  /// \ru Получить красный компонент. \en Get the red component.
  int   GetRed() const;
  /// \ru Получить красный компонент. \en Get the red component.
  float GetRedFloat() const;

  /// \ru Установить красный компонент. \en Sets the red component.
  void SetRed( int red );
  /// \ru Установить красный компонент. \en Sets the red component.
  void SetRed( float red );

  /// \ru Получить зеленый компонент. \en Get the green component.
  int   GetGreen() const;
  /// \ru Получить зеленый компонент. \en Get the green component.
  float GetGreenFloat() const;

  /// \ru Установить зеленый компонент. \en Sets the green component.
  void SetGreen( int green );
  /// \ru Установить зеленый компонент. \en Sets the green component.
  void SetGreen( float green );

  /// \ru Получить синий компонент. \en Get the blue component.
  int GetBlue() const;
  /// \ru Получить синий компонент. \en Get the blue component.
  float GetBlueFloat() const;

  /// \ru Установить синий компонент. \en Sets the blue component.
  void SetBlue( int blue );
  /// \ru Установить синий компонент. \en Sets the blue component.
  void SetBlue( float blue );

  /// \ru Получить компонент альфа. \en Get the alpha component.
  int   GetAlpha() const;
  /// \ru Получить компонент альфа. \en Get the alpha component.
  float GetAlphaFloat() const;

  /// \ru Установить компонент альфа. \en Sets the alpha component.
  void SetAlpha( int alpha );
  /// \ru Установить компонент альфа. \en Sets the alpha component.
  void SetAlpha( float alpha );

  /// \ru Получить RGBA цвет как uint32. \en Get RGBA color as uint32.
  uint32 Get() const;

  /// \ru Инициализация по uint32. \en Initialization by uint32.
  void Init( uint32 );
  /// \ru Инициализация по значениям компонентов. \en Initialization by components values.
  void Init( uint8 red, uint8 green, uint8 blue, uint8 alpha = SYS_MAX_UINT8 );
  /// \ru Инициализация по значениям компонентов. \en Initialization by components values.
  void Init( float red, float green, float blue, float alpha = 1.0 );

  /// \ru Доступ к компоненту по индексу. \en Access to a component by the index.
  uint8 & operator []( size_t ind );

  /// \ru Доступ к компоненту по индексу. \en Access to a component by the index.
  uint8 operator []( size_t ind ) const;

  /// \ru Проверка на равенство. \en Checking for equality.
  bool operator == ( const MbRGBA & c ) const;

  /// \ru Оператор присваивания. \en Assignment operator. 
  MbRGBA & operator = ( const MbRGBA & c );

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbRGBA, MATH_FUNC_EX ) // \ru Чтение и запись объекта класса. \en Reading and writing an object of the class. 

};  // MbRGBA


//------------------------------------------------------------------------------
/** \brief \ru Цвет.
           \en Color. \~
  \details \ru Цвет. \n
           \en Color. \n \~
  \deprecated \ru Класс устарел и не рекомендуется к использованию. Взамен использовать класс #MbVisual.
              \en The class is deprecated and not recommended for use. Instead use the class #MbVisual. \~
  \ingroup Model_Attributes
*/
// ---
class MATH_CLASS MbColor : public MbElementaryAttribute {
private:
  MbRGBA color; ///< \ru Цвет. \en Color. 

protected :
  /// \ru Конструктор копирования. \en Copy constructor. 
  MbColor( const MbColor & init ); 
public :
  /// \ru Конструктор. \en Constructor. 
  MbColor( uint32 init ); 
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbColor();

  // \ru Общие функции объекта \en Common functions of object. 

  MbeAttributeType AttributeType() const override; // \ru Дать подтип атрибута. \en Get subtype of an attribute. 

  MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool    Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data. 

          /// \ru Установить цвет. \en Set a color. 
          void    Init( uint32 init ) { color.Init( init ); }
          /// \ru Установить цвет. \en Set a color. 
          void    Init( const MbRGBA & init ) { color = init; }
          /// \ru Дать цвет. \en Get a color. 
          uint32  Color() const { return color.Get(); }
          /// \ru Дать цвет. \en Get a color. 
          MbRGBA  Get() const { return color; }

  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t  SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object. 
  MbePrompt GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property.

private:
          void        operator = ( const MbColor & ); // \ru Не реализовано \en Not implemented 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbColor ) 
}; // MbColor

IMPL_PERSISTENT_OPS( MbColor )


//------------------------------------------------------------------------------
/** \brief \ru Толщина линий отрисовки.
           \en Thickness of drawing lines. \~
  \details \ru Толщина линий отрисовки. \n
           \en Thickness of drawing lines. \n \~
  \ingroup Model_Attributes
*/
// ---
class MATH_CLASS MbWidth : public MbElementaryAttribute {
protected :
  int width; ///< \ru Толщина линий отрисовки. \enThickness of drawing lines. 

protected :
  /// \ru Конструктор копирования. \en Copy constructor. 
  MbWidth( const MbWidth & init ); 
public :
  /// \ru Конструктор. \en Constructor. 
  MbWidth( int init );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbWidth();

  // \ru Общие функции объекта \en Common functions of object. 

  MbeAttributeType AttributeType() const override; // \ru Дать подтип атрибута. \en Get subtype of an attribute. 
  MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool    Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data. 

          /// \ru Установить толщину. \en Set a thickness. 
          void    Init( int init ) { width = init; }
          /// \ru Дать толщину. \en Get a thickness. 
          int     Width() const  { return width; }

  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t  SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object. 
  MbePrompt GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property.

private:
          void        operator = ( const MbWidth & ); // \ru Не реализовано \en Not implemented 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbWidth ) 
}; // MbWidth

IMPL_PERSISTENT_OPS( MbWidth )


//------------------------------------------------------------------------------
/** \brief \ru Стиль линий отрисовки.
           \en Style of drawing lines. \~
  \details \ru Стиль линий отрисовки. \n
           \en Style of drawing lines. \n \~
  \ingroup Model_Attributes
*/
// ---
class MATH_CLASS MbStyle : public MbElementaryAttribute {
protected :
  int style; ///< \ru Стиль линий отрисовки. \en Style of drawing lines. 

protected :
  /// \ru Конструктор копирования. \en Copy constructor. 
  MbStyle( const MbStyle & init ); 
public :
  /// \ru Конструктор. \en Constructor. 
  MbStyle( int init );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbStyle();

  // \ru Общие функции объекта \en Common functions of object. 

  MbeAttributeType AttributeType() const override; // \ru Дать подтип атрибута. \en Get subtype of an attribute. 
  MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool    Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data. 

          /// \ru Установить стиль линий отрисовки. \en Set style of drawing lines. 
          void    Init( int init ) { style = init; }
          /// \ru Дать стиль линий отрисовки. \en Get style of drawing lines. 
          int     Style() const  { return style; }

  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t  SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object. 
  MbePrompt GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property.

private:
          void        operator = ( const MbStyle & ); // \ru Не реализовано \en Not implemented 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbStyle ) 
}; // MbStyle

IMPL_PERSISTENT_OPS( MbStyle )


//------------------------------------------------------------------------------
/** \brief \ru Свойства материала.
           \en Material properties. \~
  \details \ru Свойства материала (свойства для OpenGL). Содержит цвет и коэффициенты. \n
           Компонент _ambient хранит цвет RED, GREEN, BLUE и коэффициент общего фона в диапазоне 0.0 - 1.0 (0 - 255 в uint8). \n
           Компонент _diffuse хранит цвет RED, GREEN, BLUE и коэффициент диффузного отражения в диапазоне 0.0 - 1.0 (0 - 255 в uint8). \n
           Компонент _specular хранит цвет RED, GREEN, BLUE и коэффициент зеркального отражения света в диапазоне 0.0 - 1.0 (0 - 255 в uint8). \n
           Компонент _emission хранит цвет RED, GREEN, BLUE и коэффициент излучения в диапазоне 0.0 - 1.0 (0 - 255 в uint8). \n
           Компонент _shininess хранит блеск (показатель степени в законе зеркального отражения) в диапазоне 0 - 128 (uint8). \n
           Компонент _opacity хранит коэффициент непрозрачности (коэффициент суммарного отражения) в диапазоне 0.0 - 1.0 (0 - 255 в uint8). \n
           Компонент _chrom хранит коэффициент зеркального отражения объектов в диапазоне 0.0 - 1.0 (0 - 255 в uint8).
           \en Material properties (properties for OpenGL). Contains color and coefficients. \n 
           Component _ambient contains RED, GREEN, BLUE of a color and background coefficient in range 0.0 - 1.0 (0 - 255 in uint8). \n
           Component _diffuse contains RED, GREEN, BLUE of a color and coefficient of diffuse reflection in range 0.0 - 1.0 (0 - 255 in uint8). \n
           Component _specular contains RED, GREEN, BLUE of a color and coefficient of specular reflection in range 0.0 - 1.0 (0 - 255 in uint8). \n
           Component _emission contains RED, GREEN, BLUE of a color and emissivity coefficient in range 0.0 - 1.0 (0 - 255 in uint8). \n
           Component _shininess contains shininess coefficient (index according to the law of specular reflection) in range 0 - 128 (in uint8). \n
           Component _opacity contains opacity coefficient (coefficient of total reflection) in range 0.0 (transparent) - 1.0 (opaque) (range 0 - 255 in uint8). \n
           Component _chrom contains coefficient of specular reflection in range 0.0 - 1.0 (0 - 255 in uint8). \~
  \ingroup Model_Attributes
*/
// ---
class MATH_CLASS MbVisual : public MbElementaryAttribute {
private :
  MbRGBA _ambient;     ///< \ru Коэффициенты общего фона для трех цветов: RED, GREEN, BLUE. \en Coefficients of ambient background for colors: RED, GREEN, BLUE, range 0.0 - 1.0. 
  MbRGBA _diffuse;     ///< \ru Коэффициенты диффузного отражения для трех цветов: RED, GREEN, BLUE. \en Coefficients of diffuse reflection for colors: RED, GREEN, BLUE, range 0.0 - 1.0. 
  MbRGBA _specular;    ///< \ru Коэффициенты зеркального отражения света трех цветов: RED, GREEN, BLUE. \en Coefficients of specular reflection for light colors: RED, GREEN, BLUE, range 0.0 - 1.0. 
  MbRGBA _emission;    ///< \ru Коэффициент излучения. \en Emissivity coefficient, range 0.0 - 1.0. 
  uint8  _shininess;   ///< \ru Блеск (показатель степени в законе зеркального отражения). \en Shininess (index according to the law of specular reflection), range 0 - 128. 
  uint8  _opacity;     ///< \ru Коэффициент непрозрачности (коэффициент суммарного отражения). \en Opacity coefficient (coefficient of total reflection), range 0.0 (transparent) - 1. (opaque). 
  uint8  _chrom;       ///< \ru Коэффициент зеркального отражения объектов. \en Coefficient of specular reflection for objects, range 0.0 - 1.0. 
  MbUuid _uuid;        ///< \ru Идентификатор. \en Identifier.

protected :
  /// \ru Конструктор копирования. \en Copy constructor. 
  MbVisual( const MbVisual & init ); 
public :
  /// \ru Конструктор. \en Constructor. 
  MbVisual( float a = MB_AMBIENT, float d = MB_DIFFUSE, float s = MB_SPECULARITY,
            float h = MB_SHININESS, float t = MB_OPACITY, float e = MB_EMISSION );
  /// \ru Конструктор. \en Constructor. 
  MbVisual( uint8 a, uint8 d, uint8 s, uint8 h, uint8 t, uint8 e );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbVisual();

  /// \ru Создать по составляющей цвета. \en Create an object by a color component. 
  static SPtr<MbVisual> Create( uint32 c );

  /// \ru Создать по составляющей цвета. \en Create an object by a color component. 
  static SPtr<MbVisual> Create( int iR, int iG, int iB );

  /// \ru Создать по составляющей цвета. \en Create an object by a color component. 
  static SPtr<MbVisual> Create( const MbRGBA & c );

  // \ru Общие функции объекта \en Common functions of object. 

  MbeAttributeType AttributeType() const override; // \ru Дать подтип атрибута. \en Get subtype of an attribute. 
  MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame( const MbAttribute &, double accuracy = PARAM_ACCURACY ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool    Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data. 

          /// \ru Установить свойства для OpenGL. \en Set properties for OpenGL. 
          void    Init( float a = MB_AMBIENT, float d = MB_DIFFUSE, float s = MB_SPECULARITY, 
                        float h = MB_SHININESS, float t = MB_OPACITY, float e = MB_EMISSION, uint rgb = 0 );
          /// \ru Установить свойства для OpenGL. \en Set properties for OpenGL. 
          void    Init( const MbRGBA & ambient, const MbRGBA & diffuse, const MbRGBA & specular,
                        const MbRGBA & emission, uint8 shininess, uint8 opacity, uint8  chrom );
          /// \ru Дать свойства для OpenGL. \en Get properties for OpenGL. 
          void    Get( float & a, float & d, float & s, float & h, float & t, float & e, uint rgb = 0 ) const;
          /// \ru Дать свойства для OpenGL. \en Get properties for OpenGL. 
          void    Get( MbRGBA & ambient, MbRGBA & diffuse, MbRGBA & specular, MbRGBA & emission,
                       uint8 & shininess, uint8 & opacity, uint8 & chrom ) const;

          /// \ru Дать отдельные свойства для OpenGL. \en Get some properties for OpenGL. 
          float   Ambient     ( uint rgb = 0 ) const; // \ru Дать коэффициент общего фона. \en Get a coefficient of ambient background. 
          float   Diffuse     ( uint rgb = 0 ) const; // \ru Дать коэффициент диффузного отражения. \en Get a coefficient of diffuse reflection. 
          float   Specularity ( uint rgb = 0 ) const; // \ru Дать коэффициент зеркального отражения света. \en Get a coefficient of specular reflection for light. 
          float   Emission    ( uint rgb = 0 ) const; // \ru Дать коэффициент излучения. \en Get a coefficient of emissivity. 
          float   Shininess   () const; // \ru Дать блеск (показатель степени в законе зеркального отражения). \en Get shininess (index according to the law of specular reflection). 
          float   Opacity     () const; // \ru Дать коэффициент непрозрачности. \en Get an opacity coefficient. 
          float   Chrom       () const; // \ru Дать коэффициент зеркального отражения объектов. \en Get a coefficient of specular reflection for objects. 

          /// \ru Дать отдельные свойства для OpenGL. \en Get some properties for OpenGL. 
    const MbRGBA & GetAmbient()     const { return _ambient; }    // \ru Дать коэффициент общего фона. \en Get a coefficient of ambient background. 
    const MbRGBA & GetDiffuse()     const { return _diffuse; }    // \ru Дать коэффициент диффузного отражения. \en Get a coefficient of diffuse reflection. 
    const MbRGBA & GetSpecularity() const { return _specular; }// \ru Дать коэффициент зеркального отражения света. \en Get a coefficient of specular reflection for light. 
    const MbRGBA & GetEmission()    const { return _emission; }   // \ru Дать коэффициент излучения. \en Get a coefficient of emissivity. 
    const MbUuid & GetIdentifier()  const { return _uuid; }       // \ru Дать идентификатор. \en Get identifier.
          uint8    GetShininess()   const { return _shininess; }  // \ru Дать блеск (показатель степени в законе зеркального отражения). \en Get shininess (index according to the law of specular reflection). 
          uint8    GetOpacity()     const { return _opacity; }    // \ru Дать коэффициент непрозрачности. \en Get an opacity coefficient. 
          uint8    GetChrom()       const { return _chrom; }      // \ru Дать коэффициент зеркального отражения объектов. \en Get a coefficient of specular reflection for objects.

          /// \ru Установить отдельные свойства для OpenGL. \en Set some properties for OpenGL. 
          void    SetAmbient     ( float v, uint rgb = 0 );  // \ru Установить коэффициент общего фона. \en Set a coefficient of ambient background. 
          void    SetDiffuse     ( float v, uint rgb = 0  ); // \ru Установить коэффициент диффузного отражения. \en Set a coefficient of diffuse reflection. 
          void    SetSpecularity ( float v, uint rgb = 0  ); // \ru Установить коэффициент зеркального отражения света. \en Set a coefficient of specular reflection for light. 
          void    SetEmission    ( float v, uint rgb = 0 );  // \ru Установить коэффициент излучения. \en Set a coefficient of emissivity. 
          void    SetShininess   ( float v ); // \ru Установить блеск (показатель степени в законе зеркального отражения). \en Set shininess (index according to the law of specular reflection). 
          void    SetOpacity     ( float v ); // \ru Установить коэффициент непрозрачности. \en Set an opacity coefficient. 
          void    SetChrom       ( float v ); // \ru Установить коэффициент зеркального отражения объектов. \en Set a coefficient of specular reflection for objects. 

          /// \ru Установить отдельные свойства для OpenGL. \en Set some properties for OpenGL. 
          void    SetAmbient      ( const MbRGBA & v ) { _ambient = v; }  // \ru Установить коэффициент общего фона. \en Set a coefficient of ambient background. 
          void    SetDiffuse      ( const MbRGBA & v ) { _diffuse = v; }  // \ru Установить коэффициент диффузного отражения. \en Set a coefficient of diffuse reflection. 
          void    SetSpecularity  ( const MbRGBA & v ) { _specular = v; } // \ru Установить коэффициент зеркального отражения света. \en Set a coefficient of specular reflection for light. 
          void    SetEmission     ( const MbRGBA & v ) { _emission = v; } // \ru Установить коэффициент излучения. \en Set a coefficient of emissivity. 
          void    SetIdentifier   ( const MbUuid & v ) { _uuid = v; }     // \ru Установить идентификатор. \en Set identifier.
          void    SetShininessInt ( uint8 v )  { _shininess = v; }  // \ru Установить блеск (показатель степени в законе зеркального отражения). \en Set shininess (index according to the law of specular reflection). 
          void    SetOpacityInt   ( uint8 v )  { _opacity = v; }    // \ru Установить коэффициент непрозрачности. \en Set an opacity coefficient. 
          void    SetChromInt     ( uint8 v )  { _chrom = v; }      // \ru Установить коэффициент зеркального отражения объектов. \en Set a coefficient of specular reflection for objects. 

          /// \ru Методы работы с составляющей цвета (использовать в случае, если используется только цвет). \en Methods working with a color component (use in case only the color is used).
          uint32  GetColor()     const { return MbRGBA(_diffuse[0], _diffuse[1], _diffuse[2], 0).Get(); } // \ru Выдать составляющую цвета. \en Get a color component. 
          void    SetColor( uint32 c );               // \ru Установить составляющую цвета. \en Set a color component. 
          void    SetColor( const MbRGBA & c );       // \ru Установить составляющую цвета. \en Set a color component. 
          void    SetColor( int iR, int iG, int iB ); // \ru Установить составляющую цвета. \en Set a color component. 

          void      GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object.
          size_t    SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object.
          MbePrompt GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property.

private:
          bool      IsConverted() const;
          void      operator = ( const MbVisual & ); // \ru Не реализовано \en Not implemented 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbVisual ) 
}; // MbVisual

IMPL_PERSISTENT_OPS( MbVisual )


//------------------------------------------------------------------------------
/** \brief \ru Количество u-линий и v-линий отрисовочной сетки.
           \en The number of u-mesh and v-mesh drawing lines. \~
  \details \ru Количество u-линий и v-линий отрисовочной сетки. \n
           \en The number of u-mesh and v-mesh drawing lines. \n \~
  \ingroup Model_Attributes
*/
// ---
class MATH_CLASS MbWireCount : public MbElementaryAttribute {
protected :
  size_t uMeshCount; ///< \ru Количество u-линий отрисовочной сетки. \en The number of u-mesh lines. 
  size_t vMeshCount; ///< \ru Количество v-линий отрисовочной сетки. \en The number of v-mesh lines. 

protected :
  /// \ru Конструктор копирования. \en Copy constructor. 
  MbWireCount( const MbWireCount & init ); 
public :
  /// \ru Конструктор. \en Constructor. 
  MbWireCount( size_t uCount, size_t vCount );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbWireCount();

  // \ru Общие функции объекта \en Common functions of object. 

  MbeAttributeType AttributeType() const override; // \ru Дать подтип атрибута. \en Get subtype of an attribute. 
  MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool    Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data. 

          /// \ru Установить количество линий отрисовки. \en Set count of drawing lines. 
          void    Init( size_t uCount, size_t vCount ) { uMeshCount = uCount, vMeshCount = vCount; }
          /// \ru Выдать количество разбиений по u и v. \en The the number of splittings in u-direction and v-direction. 
          void    Get( size_t & uCount, size_t & vCount ) const { uCount = uMeshCount; vCount = vMeshCount; }

  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t  SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object. 
  MbePrompt GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property.

private:
          void        operator = ( const MbWireCount & ); // \ru Не реализовано \en Not implemented 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbWireCount ) 
}; // MbWireCount

IMPL_PERSISTENT_OPS( MbWireCount )


//------------------------------------------------------------------------------
/** \brief \ru Преобразовать атрибут MbColor в атрибут MbVisual.
           \en Convert MbColor attribute into MbVisual attribute. \~
  \details \ru Создать атрибут MbVisual, в котором компонент diffuse содержит
           значение атрибута MbColor, а остальные значения по умолчанию. \n
           \en Create a MbVisual attribute where the diffuse component contains
           the value of MbColor attribute and the rest contain default values. \n \~
  \param[in] cAttr  - \ru Атрибут MbColor.
                      \en MbColor attribute. \~
  \result \ru Возвращает указатель на созданный атрибут MbVisual.
          \en Returns a pointer to the created MbVisual attribute. \~
  \ingroup Model_Attributes
*/
// ---
MATH_FUNC( SPtr<MbVisual> ) ColorToVisual( const MbColor & cAttr );


//------------------------------------------------------------------------------
/** \brief \ru Преобразовать атрибуты цвета в атрибут MbVisual.
           \en Convert color attributes into MbVisual attribute. \~
  \details \ru Создать новый атрибут MbVisual на базе заданного атрибута MbVisual,
           в котором компонент diffuse содержит заданный MbColor. \n
           \en Create a MbVisual attribute on base of the MbVisual attribute
           where the diffuse component contains the value of the MbColor attribute. \n \~
  \param[in] cAttr -    \ru Атрибут MbColor.
                        \en MbColor attribute. \~
  \param[in] vAttr -    \ru Атрибут MbVisual.
                        \en MbVisual attribute. \~
  \result \ru Возвращает указатель на созданный атрибут MbVisual.
          \en Returns a pointer to the created MbVisual attribute. \~
  \ingroup Model_Attributes
*/
// ---
MATH_FUNC( SPtr<MbVisual> ) ColorToVisual( const MbColor & cAttr, const MbVisual & vAttr );


//------------------------------------------------------------------------------
/** \brief \ru Преобразовать атрибуты цвета в атрибут MbVisual.
           \en Convert color attributes into MbVisual attribute. \~
  \details \ru Создать новый атрибут MbVisual на базе заданного атрибута MbVisual,
           в котором компонент diffuse содержит заданный MbColor. \n
           \en Create a MbVisual attribute on base of the MbVisual attribute
           where the diffuse component contains the value of the MbColor attribute. \n \~
  \param[in] cAttr -    \ru Указатель на атрибут MbColor.
                        \en MbColor attribute pointer. \~
  \param[in] vAttr -    \ru Указатель на атрибут MbVisual.
                        \en MbVisual attribute pointer. \~
  \result \ru Возвращает указатель на созданный атрибут MbVisual.
          \en Returns a pointer to the created MbVisual attribute. \~
  \ingroup Model_Attributes
*/
// ---
MATH_FUNC( SPtr<MbVisual> ) ColorToVisual( const SPtr<MbColor> & cAttr, const SPtr<MbVisual> & vAttr );


//------------------------------------------------------------------------------
/** \brief \ru Преобразовать цвет по трём компонентам в uint32.
           \en Convert a color by 3 components in uint32. \~
  \details
  \warning \ru Значения компонент цвета должны лежать в диапазоне [ 0; 1 ].
           \en Values of color components should belong to the range [ 0; 1 ]. \~
  \ingroup Model_Attributes
*/
// ---
inline
uint32 RGB2uint32( double r, double g, double b )
{
  const double f1 = 255.0 / 256.0;
  uint32 uinturgb[3];
  const uint32 bt = 256;
  uinturgb[0] = uint32( 256.0 * r * f1 );
  uinturgb[1] = uint32( 256.0 * g * f1 );
  uinturgb[2] = uint32( 256.0 * b * f1 );
  for ( int n = 0; n < 3; n++ )
    if ( uinturgb[n] >= bt ) {
      uinturgb[n] = bt - 1;
      C3D_ASSERT_UNCONDITIONAL( false );
    }
  return uinturgb[0] + bt * ( uinturgb[1] + bt * uinturgb[2] );
}


//------------------------------------------------------------------------------
/** \brief \ru Преобразовать цвет по трём компонентам в uint32.
           \en Convert a color by 3 components in uint32. \~
  \details
  \warning \ru Значения компонент цвета должны лежать в диапазоне [ 0; 1 ].
           \en Values of color components should belong to the range [ 0; 1 ]. \~
  \ingroup Model_Attributes
*/
// ---
inline
uint32 RGB2uint32( float r, float g, float b, float a )
{
  const float f1 = 255.0 / 256.0;
  uint32 uinturgb[4];
  const uint32 bt = 256;
  uinturgb[0] = uint32( 256.0 * r * f1 );
  uinturgb[1] = uint32( 256.0 * g * f1 );
  uinturgb[2] = uint32( 256.0 * b * f1 );
  uinturgb[3] = uint32( 256.0 * a * f1 );
  for ( int n = 0; n < 4; n++ )
    if ( uinturgb[n] >= bt ) {
      uinturgb[n] = bt - 1;
      //C3D_ASSERT_UNCONDITIONAL( false );
    }
  return uinturgb[0] + bt * ( uinturgb[1] + bt * ( uinturgb[2] + bt * uinturgb[3] ) );
}


//------------------------------------------------------------------------------
/** \brief \ru Преобразовать unit32 в три компоненты цвета.
           \en Convert unit32 to 3 components of color. \~
  \details
  \warning \ru Компоненты цветов лежат в диапазоне [ 0; 1 ].
           \en Color components belong to the range [ 0; 1 ]. \~
  \ingroup Model_Attributes
*/
// ---
template<typename float_t>
void uint322RGB( uint32 color, float_t & r, float_t & g, float_t & b )
{
  const float_t r255 = float_t( 1.0 / 255.0 );
  const uint32 u256 = (uint32)SYS_MAX_UINT8 + 1;
  r = float_t( color % u256 );
  g = float_t( ( color / 256 ) % u256 );
  b = float_t( ( color / 65536 ) % u256 );
  r *= r255; g *= r255; b *= r255;
}


//------------------------------------------------------------------------------
/** \brief \ru Преобразовать цвет из модели HSV в uint32.
           \en Convert a color from HSV model in uint32. \~
  \details \ru Преобразовать цвет из модели HSV в uint32. \n
           \en Convert a color from HSV model in uint32. \n \~
  \ingroup Model_Attributes
*/
// ---
inline
uint32 HSV2uint32( double h, double s, double v )
{
  double      hh, p, q, t, ff;
  long        i;
  double      r, g, b;
  if ( s <= 0.0 ) {
    r = v;
    g = v;
    b = v;
    return ::RGB2uint32( r, g, b );
  }
  hh = h;
  if ( hh >= 360.0 )
    hh = 0.0;
  hh /= 60.0;
  i = (long)hh;
  ff = hh - i;
  p = v * ( 1.0 - s );
  q = v * ( 1.0 - ( s * ff ) );
  t = v * ( 1.0 - ( s * ( 1.0 - ff ) ) );

  switch ( i ) {
    case 0:
    {
      r = v;
      g = t;
      b = p;
    } break;
    case 1:
    {
      r = q;
      g = v;
      b = p;
    } break;
    case 2:
    {
      r = p;
      g = v;
      b = t;
    } break;
    case 3:
    {
      r = p;
      g = q;
      b = v;
    } break;
    case 4:
    {
      r = t;
      g = p;
      b = v;
    } break;
    default:
    {
      r = v;
      g = p;
      b = q;
    } break;
  }
  return ::RGB2uint32( r, g, b );
}


#endif // __ATTR_COLOR_H
