#ifndef __TEST_TEMPORAL_PLANE_H
#define __TEST_TEMPORAL_PLANE_H


#include <test_style.h>
#include <math_define.h>
#include <templ_sptr.h>
#include <mb_placement3d.h>
#include <system_dependency.h>


class    MbVector;
class    MbVector3D;
class    MbCartPoint;
class    MbCartPoint3D;
class    MbMatrix3D; 
class    MbMatrix; 
class    MbTopItem;
class    MbItem;
class    MbMesh;
class              TimeTest;
class              IGrDraw;
class              WorkWindow;


//------------------------------------------------------------------------------
// Временная конструкция точек привязки объекта 
//---
class Templane {
private :
  const MbItem *    oTarget; // Объект
  const MbTopItem * fTarget;
  MbMatrix3D        from; // SD_7163577
  double            d;
  MbPlacement3D     plane;
  Style             style;
  SPtr<MbMesh>      mesh; 
public:
  Templane()
    : oTarget( nullptr )
    , fTarget( nullptr )
    , from   (      )
    , d      ( 0.0  )
    , plane  (      )
    , style  (      )
    , mesh   ( nullptr )
  {
    style.SetWidth( 1 );
    style.SetColor( RGB( 127, 127, 127 ) ); //style.SetColor( RGB( 255, 255, 255 ) );
  }
  virtual ~Templane();

public:
  const MbItem * GetObject() const { return oTarget; }
  const MbTopItem * GetTopoItem() const { return fTarget; }

  bool        Exist() const { return (oTarget != nullptr); }
  void        Delete();
  bool        Init( const MbItem * obj, const MbTopItem * f, const MbMatrix3D & m, double a = 0 );
  void        Reset();

  const MbPlacement3D & SetPlacement( const WorkWindow * wind ); // Дать плейсемент

  void        Erase( const WorkWindow * wind ) const; // Стереть в окне
  void        Show ( const WorkWindow * wind ) const; // Нарисовать в окне
  void        CalculateMesh(); // Построить карту

OBVIOUS_PRIVATE_COPY( Templane )
}; // Templane


#endif // __TEST_TEMPORAL_PLANE_H
