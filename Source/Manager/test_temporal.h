#ifndef __TEST_TEMPORAL_H
#define __TEST_TEMPORAL_H


#include <test_style.h>
#include <templ_sptr.h>
#include <mb_placement3d.h>
#include <mesh.h>
#include <vector>


class              WorkWindow;


//------------------------------------------------------------------------------
// Временная конструкция точек привязки объекта 
//---
class Temporal {
private:
  SPtr<MbItem>                oTarget; // Объект-источник
  MbControlData3D             sPoints; // Список пространственных точек
  MbControlData               pPoints; // Список двумерных точек
  MbPlacement3D *             plane;
  ptrdiff_t                   whole; // Количество горячих точек, перемещаемых вместе
  std::vector< SPtr<MbMesh> > meshs;
  MbMatrix3D                  matr;     // Матрица преобразования объекта в глобальную систему координат
  MbMatrix3D                  divMatr;  // Обратная матрица для matr

public:
  static const Style groupStyle;
  static const Style singleStyle;

public:
  Temporal()
    : oTarget( nullptr )
    , sPoints()
    , pPoints()
    , plane  ()
    , whole  ( 0 )
    , meshs  ()
    , matr   ()
    , divMatr()
  {
  }
  ~Temporal() {}

public:
  size_t      GetTenderCount() const { return meshs.size(); }
  // Общее количество горячих точек (пока считаем так: хот-точки либо пространственные либо плоские )
  size_t      PointsCount() const{ return sPoints.IsEmpty() ? pPoints.Count() : sPoints.Count(); }
  size_t      TotalCount() const { PRECONDITION( !pPoints.IsEmpty() || !sPoints.IsEmpty() ); 
                                   return sPoints.IsEmpty() ? pPoints.TotalCount() : sPoints.TotalCount(); }
  size_t      ShareCount() const { PRECONDITION( !pPoints.IsEmpty() || !sPoints.IsEmpty() ); 
                                   return sPoints.IsEmpty() ? pPoints.ShareCount() : sPoints.ShareCount(); }

  const MbItem * GetObject() const { return oTarget; }
        MbItem * SetObject() { return oTarget; }
  const MbMesh * GetTender( size_t ind ) const { return ((ind < meshs.size()) ? &(*meshs[ind]) : nullptr); }

  bool        Exist() const { return (meshs.size() > 0); }
  void        Delete(); // Удалить все объекты
  void        Init( MbItem * obj, const MbMatrix3D & initMatr );

  ptrdiff_t   Move( ptrdiff_t index, const MbVector3D & to, const MbVector & t, bool target ); // Сдвиг
  ptrdiff_t   Move(                  const MbVector3D & to, const MbVector & t ); // Сдвиг

  ptrdiff_t   GetPoint( ptrdiff_t tindex, ptrdiff_t n, MbCartPoint3D & p3, MbCartPoint & p2 ) const;
  ptrdiff_t   SetPoint( ptrdiff_t tindex, ptrdiff_t n, MbCartPoint3D & p );
  ptrdiff_t   SetPoint( ptrdiff_t tindex, ptrdiff_t n, MbCartPoint & p );

  void        Move( ptrdiff_t i, const MbVector3D & to, WorkWindow * wind, const COLORREF & col ); // Сдвинуть отображение в окне
  void        Move(              const MbVector3D & to, WorkWindow * wind, const COLORREF & col ); // Сдвинуть отображение в окне

  void        Erase( WorkWindow * wind ); // Стереть в окне
  void        Show ( WorkWindow * wind ) const; // Нарисовать в окне

  bool        CalculateMesh(); // Построить карту
  void        Refresh( bool target );

OBVIOUS_PRIVATE_COPY( Temporal )
}; // Temporal


//------------------------------------------------------------------------------
// Расстояние до объекта
// ---
float DistanceToPoint( const MbMesh * mesh, const MbFloatAxis3D & from, float maxDistance, ptrdiff_t & j );


#endif // __TEST_TEMPORAL_H
