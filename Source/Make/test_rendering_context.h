#ifndef __TEST_RENDERING_CONTEXT_H
#define __TEST_RENDERING_CONTEXT_H


#include <system_types.h>
#include <system_dependency.h>
#if defined(C3D_WINDOWS)
  #include <windows.h>
#endif


//------------------------------------------------------------------------------
// Цвет
// ---
struct RGBColor {
protected:
  uint8 b;
  uint8 g;
  uint8 r;

public:
  explicit RGBColor( uint8 _r, uint8 _g, uint8 _b );

  void             operator = ( COLORREF  p );
  const RGBColor & operator = ( const RGBColor & p );
                   operator COLORREF () const;
  void             set( uint8 r, uint8 g, uint8 b );
};


//------------------------------------------------------------------------------
//          Контекст битового поля для записи  *.BMP  файла
// ---
class BMPContext {

public :
  enum Mode { bmp, dib };

protected :
  uint16            width;
  uint16            height;
  uint16            lineLength;   // real line byte length
#ifdef C3D_WINDOWS // _MSC_VER 
  HGLOBAL           handle;
#endif // _MSC_VER 
  RGBColor*         pixels;       // Указатель на начало битового поля
  uint16            colPointer;   // current pos on a line
  int16             linePointer;  // current line
  RGBColor*         pointer;      // Указатель на текущее место битового поля
  static uint8      clrBits;      // number of color bits
  static uint32     clrSize;      // size of color tables

public :
  BMPContext( uint16 xDim, uint16 yDim );
  BMPContext( uint16 xDim, uint16 yDim, COLORREF bkColor );
  ~BMPContext();

  void              Fill( COLORREF bkColor ); // Зополнить цветом фона

  const RGBColor &  GetPix( uint16 x, uint16 y ) const;
        RGBColor &  SetPix( uint16 x, uint16 y );

  void              Get( uint16 & x, uint16 & y ) const;
  uint16            GetW()                        const;
  uint16            GetH()                        const;

  void              Set( uint16 x, uint16 y, uint8 r, uint8 g, uint8 b ); // set pixel
  void              Set( uint16 x, uint16 y, const RGBColor & p ); // set pixel
  void              Set( uint16 x, uint16 y );                     // set current pointer
  void              Set( uint8  r, uint8  g, uint8 b );            // set pixel
  void              Set( const RGBColor & pixel );                 // set pixel

  void              operator = ( const RGBColor & pix );           // set pixel
  void              operator +=( const RGBColor & pix );           // set pixel and advance pointer
  void              operator = ( COLORREF pix );                   // set pixel
  void              operator +=( COLORREF pix );                   // set pixel and advance pointer
  RGBColor*         operator ++();                                 // prefix
  RGBColor*         operator ++(int);                              // postfix
  RGBColor*         operator ->();                                 // return current pointer

  void              Save( const TCHAR* name );

  RGBColor*         inc();
  RGBColor*         dec();

private :
  RGBColor*         go( uint16 x, uint16 y ) const;
  RGBColor*         goToLine( uint16 y );
  RGBColor*         goToEndOfLine( uint16 y );

private:
  BMPContext  ( const BMPContext & ); // Не реализовано
  void operator = ( const BMPContext & ); // Не реализовано
};


//------------------------------------------------------------------------------
// Ctor
// ---
inline RGBColor::RGBColor( uint8 _r, uint8 _g, uint8 _b ) {
  r = _r;
  g = _g;
  b = _b;
}

//------------------------------------------------------------------------------
//
// ---
inline void RGBColor::operator = ( COLORREF c )
{
  r = GetRValue( c );
  g = GetGValue( c );
  b = GetBValue( c );
}

//------------------------------------------------------------------------------
//
// ---
inline const RGBColor & RGBColor::operator = ( const RGBColor & p )
{
  memcpy( this, &p, sizeof(RGBColor) );
  return *this;
}

//------------------------------------------------------------------------------
//
// ---
inline RGBColor::operator COLORREF() const {
  return RGB(r,g,b);
}

//------------------------------------------------------------------------------
//
// ---
inline void RGBColor::set( uint8 _r, uint8 _g, uint8 _b ) {
  r = _r;
  g = _g;
  b = _b;
}

//------------------------------------------------------------------------------
//
// ---
inline const RGBColor & BMPContext::GetPix( uint16 x, uint16 y ) const {
  return *go( x, uint16(height - y - 1) );
}

//------------------------------------------------------------------------------
//
// ---
inline RGBColor & BMPContext::SetPix( uint16 x, uint16 y ) {
  return *go( x, uint16(height - y - 1) );
}

//------------------------------------------------------------------------------
//
// ---
inline void BMPContext::Get( uint16& x, uint16& y ) const {
  x = colPointer;
  y = uint16(height - linePointer - 1);
}

//------------------------------------------------------------------------------
//
// ---
inline uint16 BMPContext::GetW() const {
  return width;
}

//------------------------------------------------------------------------------
//
// ---
inline uint16 BMPContext::GetH() const {
  return height;
}

//------------------------------------------------------------------------------
// set pixel
// ---
inline void BMPContext::Set( uint16 x, uint16 y, uint8 r, uint8 g, uint8 b ) {
  SetPix(x,y).set(r,g,b);
}

//------------------------------------------------------------------------------
// set pixel
// ---
inline void BMPContext::Set( uint16 x, uint16 y, const RGBColor & p ) {
  SetPix(x,y) = p;
}

//------------------------------------------------------------------------------
// set current pointer
// ---
inline void BMPContext::Set( uint16 x, uint16 y ) {
  colPointer  = x;
  linePointer = int16(height - y - 1);
  pointer = go( x, linePointer );
}

//------------------------------------------------------------------------------
// set pixel
// ---
inline void BMPContext::Set( uint8 r, uint8 g, uint8 b ) {
  pointer->set(r,g,b);
}

//------------------------------------------------------------------------------
// set pixel
// ---
inline void BMPContext::Set( const RGBColor & p ) {
  (*pointer) = p;
}

//------------------------------------------------------------------------------
// set pixel
// ---
inline void BMPContext::operator = ( const RGBColor & p ) {
  (*pointer) = p;
}

//------------------------------------------------------------------------------
// set pixel
// ---
inline void BMPContext::operator += ( const RGBColor & p ) {
  (*pointer) = p;
  inc();
}

//------------------------------------------------------------------------------
// set pixel
// ---
inline void BMPContext::operator = ( COLORREF p ) {
  *pointer = p;
}

//------------------------------------------------------------------------------
// set pixel
// ---
inline void BMPContext::operator += ( COLORREF p ) {
  *pointer = p;
  inc();
}

//------------------------------------------------------------------------------
//
// ---
inline RGBColor /*huge*/* BMPContext::operator++() { // prefix
  RGBColor /*huge*/* back = pointer;
  return inc(), back;
}

//------------------------------------------------------------------------------
//
// ---
inline RGBColor /*huge*/* BMPContext::operator++(int) { // postfix
  return inc();
}

//------------------------------------------------------------------------------
//
// ---
inline RGBColor /*huge*/* BMPContext::operator->() {
  return pointer;
}

//------------------------------------------------------------------------------
// draw BMP
// ---
inline RGBColor /*huge*/* BMPContext::inc() {
  if ( colPointer < width-1 )
    return colPointer++, pointer++;
  else
    return
      linePointer == 0 ?
        0 :
        colPointer = 0, pointer = goToLine( --linePointer );
}

//------------------------------------------------------------------------------
// 
// ---
inline RGBColor /*huge*/* BMPContext::dec() {
  if ( colPointer > 0 )
    return colPointer--, pointer--;
  else
    return
      linePointer == height -1 ?
        0 :
        colPointer = uint16(width-1), pointer = goToEndOfLine( uint16(++linePointer) );
}

//------------------------------------------------------------------------------
// 
// ---
inline RGBColor /*huge*/* BMPContext::go( uint16 x, uint16 y ) const {
  size_t off = (size_t)y * (size_t)lineLength + (size_t)x * sizeof(RGBColor);
  return (RGBColor*)((char /*huge*/*)pixels + off);
}

//------------------------------------------------------------------------------
// 
// ---
inline RGBColor /*huge*/* BMPContext::goToLine( uint16 y ) {
  size_t off = (size_t)y * (size_t)lineLength;
  return (RGBColor /*huge*/*)((char /*huge*/*)pixels + off);
}

//------------------------------------------------------------------------------
// 
// ---
inline RGBColor /*huge*/* BMPContext::goToEndOfLine( uint16 y ) {
  size_t off =  (size_t)y * (size_t)lineLength + (size_t)(width-1) * sizeof(RGBColor);
  return (RGBColor /*huge*/*)((char /*huge*/*)pixels + off);
}


#endif // __TEST_RENDERING_CONTEXT_H
