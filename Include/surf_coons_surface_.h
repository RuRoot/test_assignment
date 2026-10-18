////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Бикубическая поверхность Кунса на четырех кривых и их поперечных производных.
             Временные данные.
         \en Bicubic Coons surface on four curves and its transverse derivatives.
             Temporary data. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_COONS_SURFACE__H
#define __SURF_COONS_SURFACE__H

#include <mb_cart_point3d.h>


#define COONS_COUNT 4 ///< \ru Число кривых, используемых для построения поверхности Кунса \en Count of curves used to construct Coons surface. 


//------------------------------------------------------------------------------
/** \brief \ru Временные данные поверхности Кунса.
           \en Temporary data of Coons surface. \~
  \details \ru Временные данные поверхности Кунса для патча Грегори. \n
           \en Temporary data of Coons surface for Gregory patch. \n \~
*/
// ---
class MbCoonsWorkingData {
  friend class MbCoonsPatchSurface;
private:
  double calcU;
  double calcV;

  // Зависят от u и v.
  MbCartPoint3D    twist[COONS_COUNT];     // Смешанные производные. Если r''uv=r''vu, то twist равен им обеим и не зависит от u и v.
  MbCartPoint3D    twistU[COONS_COUNT];    // Производная по U.
  MbCartPoint3D    twistV[COONS_COUNT];    // Производная по V.
  MbCartPoint3D    twistUU[COONS_COUNT];   // Производная по UU.
  MbCartPoint3D    twistUV[COONS_COUNT];   // Производная по UV.
  MbCartPoint3D    twistVV[COONS_COUNT];   // Производная по VV.
  MbCartPoint3D    twistUUU[COONS_COUNT];  // Производная по UUU.
  MbCartPoint3D    twistUUV[COONS_COUNT];  // Производная по UUV.
  MbCartPoint3D    twistUVV[COONS_COUNT];  // Производная по UVV.
  MbCartPoint3D    twistVVV[COONS_COUNT];  // Производная по VVV.

public:
  /// \ru Конструктор. \en Constructor. 
  MbCoonsWorkingData()
    : calcU ( UNDEFINED_DBL )
    , calcV ( UNDEFINED_DBL )
  {
    for ( size_t i = 0; i < COONS_COUNT; ++i ) {
      twist[i]   .SetZero();
      twistU[i]  .SetZero();
      twistV[i]  .SetZero();
      twistUU[i] .SetZero();
      twistUV[i] .SetZero();
      twistVV[i] .SetZero();
      twistUUU[i].SetZero();
      twistUUV[i].SetZero();
      twistUVV[i].SetZero();
      twistVVV[i].SetZero();
    }
  }

  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbCoonsWorkingData( const MbCoonsWorkingData & init )
    : calcU( init.calcU )
    , calcV( init.calcV )
  {
    for ( size_t i = 0; i < COONS_COUNT; ++i ) {
      twist[i]    = init. twist[i]   ;
      twistU[i]   = init. twistU[i]  ;
      twistV[i]   = init. twistV[i]  ;
      twistUU[i]  = init. twistUU[i] ;
      twistUV[i]  = init. twistUV[i] ;
      twistVV[i]  = init. twistVV[i] ;
      twistUUU[i] = init. twistUUU[i];
      twistUUV[i] = init. twistUUV[i];
      twistUVV[i] = init. twistUVV[i];
      twistVVV[i] = init. twistVVV[i];
    }
  }

  // Деструктор.
  ~MbCoonsWorkingData() {}

  /// \ru Обнулить координаты. \en Set coordinates to zero. 
  void SetZeroVectors() {
    for ( size_t i = 0; i < COONS_COUNT; ++i ) {
      twist[i]   .SetZero();
      twistU[i]  .SetZero();
      twistV[i]  .SetZero();
      twistUU[i] .SetZero();
      twistUV[i] .SetZero();
      twistVV[i] .SetZero();
      twistUUU[i].SetZero();
      twistUUV[i].SetZero();
      twistUVV[i].SetZero();
      twistVVV[i].SetZero();
    }
  }

  /// \ru Присвоить значение другого объекта. \en Assign a value of another object.
  MbCoonsWorkingData & operator = ( const MbCoonsWorkingData & init ) {
    calcU = init.calcU;
    calcV = init.calcV;
    for ( size_t i = 0; i < COONS_COUNT; ++i ) {
      twist[i] = init.twist[i];
      twistU[i] = init.twistU[i];
      twistV[i] = init.twistV[i];
      twistUU[i] = init.twistUU[i];
      twistUV[i] = init.twistUV[i];
      twistVV[i] = init.twistVV[i];
      twistUUU[i] = init.twistUUU[i];
      twistUUV[i] = init.twistUUV[i];
      twistUVV[i] = init.twistUVV[i];
      twistVVV[i] = init.twistVVV[i];
    }
    return *this;
  }

  /// \ru Проверить объекты на равенство. \en Check objects for equality. 
  bool operator == ( const MbCoonsWorkingData & init ) {
    bool res = calcU == init.calcU && calcV == init.calcV;
    for ( size_t i = 0; i < COONS_COUNT && res; ++i ) {
      res &= twist[i] == init.twist[i];
      res &= twistU[i] == init.twistU[i];
      res &= twistV[i] == init.twistV[i];
      res &= twistUU[i] == init.twistUU[i];
      res &= twistUV[i] == init.twistUV[i];
      res &= twistVV[i] == init.twistVV[i];
      res &= twistUUU[i] == init.twistUUU[i];
      res &= twistUUV[i] == init.twistUUV[i];
      res &= twistUVV[i] == init.twistUVV[i];
      res &= twistVVV[i] == init.twistVVV[i];
    }
    return res;
  }
};


#endif // __SURF_COONS_SURFACE__H
