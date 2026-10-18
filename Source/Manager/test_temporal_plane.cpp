#include <test_temporal_plane.h>
#include <test_window.h>
#include <test_gr_draw.h>
#include <topology_item.h>
#include <model_item.h>
#include <mesh.h>
#include <plane_instance.h>
#include <mb_rect.h>
#include <surface.h>
#include <topology.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Деструктор
// ---
Templane::~Templane() {
}


//------------------------------------------------------------------------------
// Удалить все
// ---
void Templane::Delete()
{
  oTarget = nullptr;
  fTarget = nullptr;
  d = 0;
  mesh = nullptr;
}


//------------------------------------------------------------------------------
// Инициализация по объекту
// ---
bool Templane::Init( const MbItem * obj, const MbTopItem * f, const MbMatrix3D & m, double a )
{
  if ( obj != nullptr ) {
    from.Init( m ); // SD_7163577
    if ( f != nullptr && f->IsA() == tt_Face ) {
      const MbFace * face = static_cast<const MbFace *>(f);
      if ( face->GetSurface().GetSurface().IsA() == st_Plane) { // Является ли грань плоскостью?
        if ( face->GetPlacement( &plane ) ) { // Дать плоскость
          oTarget = obj;
          fTarget = f;
          d = a;
          if ( d != 0 )
            plane.SetOrigin() = plane.GetOrigin() + plane.GetAxisZ() * d;
          return true;
        }
      }
    }
    else {
      if ( obj->IsA() == st_PlaneInstance ) {
        plane.Init( (static_cast<const MbPlaneInstance *>(obj))->GetPlacement() );
        oTarget = obj;
        fTarget = nullptr;
        d = a;
        if ( d != 0 )
          plane.SetOrigin() = plane.GetOrigin() + plane.GetAxisZ() * d;
        return true;
      }
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// Инициализация
// ---
void Templane::Reset()
{
  if ( oTarget != nullptr ) {
    if ( fTarget != nullptr && fTarget->IsA() == tt_Face) {
      const MbFace * face = static_cast<const MbFace *>(fTarget);
      if ( face->GetSurface().GetSurface().IsA() == st_Plane ) { // Является ли грань плоскостью?
        if (face->GetPlacement( &plane )) { // Дать плоскость
          if ( d != 0 )
            plane.SetOrigin() = plane.GetOrigin() + plane.GetAxisZ() * d;
        }
      }
    }
    else {
      if ( oTarget->IsA() == st_PlaneInstance ) {
        plane.Init(((MbPlaneInstance *)oTarget)->GetPlacement());
        if ( d != 0 )
          plane.SetOrigin() = plane.GetOrigin() + plane.GetAxisZ() * d;
      }
    }
  }
}


//------------------------------------------------------------------------------
// Дать плейсемент
// ---
const MbPlacement3D & Templane::SetPlacement( const WorkWindow * wind )
{
  if ( !Exist() ) {
    plane.Init( wind->GetOwnPlace() );
  }
//#ifdef _DRAWGI
  // Заполняем матрицу для рисования двумерных объектов в активном плейсементе
//  ::memcpy( viewMatrix, &(plane.GetMatrixFrom()), sizeof(viewMatrix) );
//#endif // _DRAWGI
  return plane;
}


//------------------------------------------------------------------------------
// Стереть в окне
// ---
void Templane::Erase( const WorkWindow * wind ) const
{
  if ( mesh != nullptr && wind != nullptr ) {
    wind->PutMeshGDI( mesh, from, nullptr, nullptr, wind->GetDrawTool().GetBackground(), true, false );
  }
}


//------------------------------------------------------------------------------
// Нарисовать в окне
// ---
void Templane::Show( const WorkWindow * wind ) const
{
  if ( mesh != nullptr && wind != nullptr ) {
    COLORREF col( MB_C3DCOLOR );
    wind->PutMeshGDI( mesh, from, nullptr, nullptr, col, false, false );
  }
}


//------------------------------------------------------------------------------
// Построить карту
// ---
void Templane::CalculateMesh()
{
  if ( mesh == nullptr )
    mesh = new MbMesh();

  mesh->Flush();
  mesh->SetColor( style.GetColor() );     // Цвет 
  mesh->SetWidth( style.GetWidth() );     // Толщина
  mesh->SetStyle( style.GetStyle() );     // Стиль 
  MbPolygon3D * poly = mesh->AddPolygon(); // Добавить новый пустой объект
  MbCartPoint3D p0[4];       //-V112
  bool res = false;
  ptrdiff_t i, j;

  if ( fTarget!=nullptr && fTarget->IsA()==tt_Face) {
    const MbSurface *surf = &((MbFace *)fTarget)->GetSurface().GetSurface();
    bool orient = ((MbFace *)fTarget)->IsSameSense();

    for (i=0;i<4;i++) {       //-V112
      if (orient)   j = i+1;
      else          j = 4-i;       //-V112
      surf->GetLimitPoint(j,p0[i]);
      if (d!=0)
        p0[i] = p0[i] + plane.GetAxisZ() * d;
      poly->AddPoint( p0[i] );
    }
    res = true;
  }
  else {
    if (oTarget!=nullptr) {
      if ( oTarget->IsA() == st_PlaneInstance ) {
        const MbPlaneItem * mobj = ((MbPlaneInstance *)oTarget)->GetPlaneItem();  // Дать 2D объект
        if (mobj!=nullptr) {
          MbRect r;
          mobj->AddYourGabaritTo( r );
          p0[0] = plane.GetOrigin() + plane.GetAxisX()*r.left  + plane.GetAxisY()*r.bottom;
          p0[1] = plane.GetOrigin() + plane.GetAxisX()*r.right + plane.GetAxisY()*r.bottom;
          p0[2] = plane.GetOrigin() + plane.GetAxisX()*r.right + plane.GetAxisY()*r.top;
          p0[3] = plane.GetOrigin() + plane.GetAxisX()*r.left  + plane.GetAxisY()*r.top;
          for (i=0;i<4;i++) {       //-V112
            poly->AddPoint( p0[i] );
          }
          res = true;
        }
      }
    }
  }
  if ( res ) {
    MbCartPoint3D pp, p;
    double l  = 0.5;
    double l0 = ITERATE_COUNT * l;
    MbVector3D vx(p0[0],p0[1]), vy(p0[0],p0[3]);
    vx.Normalize();
    vy.Normalize();

    poly = mesh->AddPolygon(); // Добавить новый пустой объект

    p = p0[0]+vx*l+vy*l;           poly->AddPoint( p );
    p = p  +vx*(2*l0-l);           poly->AddPoint( p );
    p = p  +vy*(l);                poly->AddPoint( p );
    p = p  +vx*(3*l)-vy*(2*l);     poly->AddPoint( p );
    p = p  -vx*(3*l)-vy*(2*l);     poly->AddPoint( p );
    p = p  +vy*(l);                poly->AddPoint( p );
    p = p  -vx*(2*l0+l);           poly->AddPoint( p );
    p = p  +vy*(l0+l);             poly->AddPoint( p );
    p = p  -vx*(l);                poly->AddPoint( p );
    p = p  +vy*(3*l)+vx*(2*l);     poly->AddPoint( p );
    p = p  -vy*(3*l)+vx*(2*l);     poly->AddPoint( p );
    p = p  -vx*(l);                poly->AddPoint( p );
    p = p  -vy*(l0-l);             poly->AddPoint( p );
  }
}

