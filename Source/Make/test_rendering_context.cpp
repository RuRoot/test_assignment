#include <test_rendering_context.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
//
// ---
int32 NColors( uint16 bitCount ) {
  if (bitCount == 1 || bitCount == 4 || bitCount == 8)      //-V112
    return 1 << bitCount;
  else if (bitCount == 24)
    return 0;
  return -1;
}


//------------------------------------------------------------------------------
//
// ---
inline int ScanBytes( int pixWidth, int bitsPixel ) {
  return int(((uint32(pixWidth)*bitsPixel+31) / 32) * 4);      //-V112
}


//------------------------------------------------------------------------------
//
// ---
uint8  BMPContext::clrBits = 24;                   // current number of color bits
uint32 BMPContext::clrSize = NColors( clrBits );   // size of color tables


//------------------------------------------------------------------------------
// Ctor
// ---
BMPContext::BMPContext( uint16 xDim, uint16 yDim )
  : width( xDim ),
    height( yDim ),
    lineLength( (uint16)ScanBytes(xDim,clrBits) ),
#ifdef C3D_WINDOWS // _MSC_VER
    handle( ::GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,uint32(lineLength)*yDim) ),
    pixels( (RGBColor*)::GlobalLock(handle) ),
#else // _WIN32 
    pixels( (RGBColor*)::malloc( uint32(lineLength)*yDim ) ),
#endif // _WIN32 
    colPointer( 0 ),
    linePointer( (uint16)(yDim-1) ),
    pointer( goToLine((uint16)(yDim-1)) ) 
{
}


//------------------------------------------------------------------------------
// Ctor
// ---
BMPContext::BMPContext( uint16 xDim, uint16 yDim, COLORREF bkColor )
  : width( xDim ),
    height( yDim ),
    lineLength( (uint16)ScanBytes(xDim,clrBits) ),
#ifdef C3D_WINDOWS // _MSC_VER
    handle( ::GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,uint32(lineLength)*yDim) ),
    pixels( (RGBColor*)::GlobalLock(handle) ),
#else // _WIN32 
    pixels( (RGBColor*)::malloc( uint32(lineLength)*yDim ) ),
#endif // _WIN32 
    colPointer( 0 ),
    linePointer( (uint16)(yDim-1) ),
    pointer( goToLine((uint16)(yDim-1)) ) 
{
  Fill( bkColor );
}

//------------------------------------------------------------------------------
// 
// ---
BMPContext::~BMPContext() {
#ifdef C3D_WINDOWS // _MSC_VER
  if ( handle ) {
    ::GlobalUnlock( handle );
    ::GlobalFree( handle );
  }
#else // _WIN32
  ::free( pixels );
#endif // _WIN32
}

//------------------------------------------------------------------------------
// Заполнить цветом фона
// ---
void BMPContext::Fill( COLORREF bkColor ) {
  RGBColor * p1 = goToLine((uint16)(height-1));
  RGBColor * p2 = p1;
  int i;
  for( i = 0; i<width; i++ )
    memcpy( p2++, &bkColor, sizeof(RGBColor) );
  for( p2=goToLine((uint16)(i=height-2)); i>=0; p2=goToLine((uint16)--i) )
    memcpy( p2, p1, lineLength );
}


// //------------------------------------------------------------------------------
// // Вывод в окно
// // ---
// void BMPContext::CopyFromDC( uint16 x, uint16 y/*, Mode mode*/ ) 
// {
// #ifdef _MSC_VER // COMPILE
//   HDC hdc = ::CreateCompatibleDC( nullptr );
// 
//   POINT nc = { x, y };
//   int old_mode = ::SetMapMode( hdc, MM_TEXT );
// 
//   HBITMAP hbmp;
// // Не удалять   if ( mode == bmp ) {
// // Не удалять     BITMAP bm = {
// // Не удалять       0,          // bmType
// // Не удалять       width,      // bmWidth
// // Не удалять       height,     // bmHeight
// // Не удалять       lineLength, // bmWidthBytes
// // Не удалять       1,          // bmPlanes;
// // Не удалять       clrBits,    // bmBitsPixel
// // Не удалять       pixels      // bmBits
// // Не удалять     };
// // Не удалять     hbmp = ::CreateBitmapIndirect( &bm );
// // Не удалять   }
// // Не удалять   else {
//     BITMAPINFO info = {
//       { sizeof(BITMAPINFOHEADER), // biSize
//         width,                    // biWidth
//         height,                   // biHeight
//         1,                        // biPlanes
//         clrBits,                  // biBitCount
//         BI_RGB,                   // biCompression
//         uint32(lineLength)*height,// biSizeImage
//         0,                        // biXPelsPerMeter
//         0,                        // biYPelsPerMeter
//         clrSize,                  // biClrUsed
//         0,                        // biClrImportant
//       },
//       {{ 0, 0, 0, 0 }}
//     };
// 
//     hbmp = ::CreateCompatibleBitmap( hdc, width, height );
//     if ( 0 == ::SetDIBits(hdc, hbmp, 0, height, pixels, &info, DIB_RGB_COLORS) )
//     {
//       ::DeleteObject( hbmp );
//       hbmp = 0;
//     }
// // Не удалять   }
// 
//   if ( hbmp ) 
//   {
//     HDC mem  = ::CreateCompatibleDC( hdc );
//     hbmp = HBITMAP( ::SelectObject(mem,hbmp) );
//     ::BitBlt( hdc, nc.x, nc.y, width, height, mem, 0, 0, SRCCOPY );
//     ::DeleteObject( ::SelectObject(mem,hbmp) );
//     ::DeleteDC( mem );
//   }
// 
//   ::SetMapMode( hdc, old_mode );
//   ::DeleteDC( hdc );
// #endif // _MSC_VER // COMPILE
// }
 

#ifndef C3D_WINDOWS // _MSC_VER
#pragma pack(push,2)

typedef struct tagBITMAPINFOHEADER {  // Определено в WinGDI.h
        uint32 /*DWORD*/      biSize;
        int32  /*LONG */      biWidth;
        int32  /*LONG */      biHeight;
        uint16 /*WORD */      biPlanes;
        uint16 /*WORD */      biBitCount;
        uint32 /*DWORD*/      biCompression;
        uint32 /*DWORD*/      biSizeImage;
        int32  /*LONG */      biXPelsPerMeter;
        int32  /*LONG */      biYPelsPerMeter;
        uint32 /*DWORD*/      biClrUsed;
        uint32 /*DWORD*/      biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagBITMAPFILEHEADER {  // Определено в WinGDI.h
        uint16 /*WORD */      bfType;
        uint32 /*DWORD*/      bfSize;
        uint16 /*WORD */      bfReserved1;
        uint16 /*WORD */      bfReserved2;
        uint32 /*DWORD*/      bfOffBits;
} BITMAPFILEHEADER;

#pragma pack(pop)

  #define SIZE_WRITE  size_t
  #define HANDLE      FILE* 
  #define CloseHandle fclose
  static void WriteFile( HANDLE      hFile,
                         const void *lpBuffer,
                         SIZE_WRITE  nNumberOfBytesToWrite,
                         SIZE_WRITE  *lpNumberOfBytesWritten,
                         const void *lpOverlapped )
  {
    size_t size = fwrite( lpBuffer, nNumberOfBytesToWrite, 1/*count*/,  hFile );
  }
#else // _WIN32 
  #define SIZE_WRITE DWORD
#endif // _WIN32 



//------------------------------------------------------------------------------
// Вывод в файл
// ---
void BMPContext::Save( const TCHAR* name ) 
{
  BITMAPINFOHEADER bmih = {
    sizeof(bmih),             // biSize
    width,                    // biWidth
    height,                   // biHeight
    1,                        // biPlanes
    clrBits,                  // biBitCount
    0 /*BI_RGB*/,             // biCompression
    uint32(lineLength)*height,// biSizeImage
    0,                        // biXPelsPerMeter
    0,                        // biYPelsPerMeter
    clrSize,                  // biClrUsed
    0,                        // biClrImportant
  };

  BITMAPFILEHEADER bmfh = {
    0x4D42,                                         // bfType = 'BM'; // Сигнатура BMP файла.
    uint32 /*DWORD*/(sizeof(bmfh) + sizeof(bmfh) + (size_t)bmih.biSizeImage), // bfSize     //-V119
    0,                                              // bfReserved1 = 0;
    0,                                              // bfReserved2 = 0;
    sizeof(bmfh) + sizeof(bmih)                     // bfOffBits          //-V119
  };

  HANDLE hFile;
#ifdef C3D_WINDOWS // _MSC_VER
    hFile = CreateFile( name,                 // Имя файла 
                        GENERIC_WRITE,        // Создание на запись
                        0,                    // Флаг доступа для другого процесса
                        nullptr,              // Атрибуты безопасности
                        CREATE_ALWAYS,        // Открываем файл
                        FILE_ATTRIBUTE_NORMAL,// Атрибуты файла
                        nullptr);
#else // _WIN32 
    hFile = fopen( c3d::C3DToPathstring(string_t(name)).c_str(), "wt" );
#endif // _WIN32 


  if ( hFile ) {
    SIZE_WRITE written;
    ::WriteFile( hFile, &bmfh,   sizeof(bmfh),     &written, 0 );
    ::WriteFile( hFile, &bmih,   sizeof(bmih),     &written, 0 );
    ::WriteFile( hFile, pixels,  bmih.biSizeImage, &written, 0 );

    ::CloseHandle( hFile );
  }
}

#ifndef C3D_WINDOWS // _MSC_VER
  #undef SIZE_WRITE
  #undef HANDLE
  #undef CloseHandle
#else // _WIN32 
  #undef SIZE_WRITE
#endif // _WIN32 

